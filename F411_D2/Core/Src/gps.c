/*
 * gps.c
 *
 *  Created on: Oct 26, 2023
 *      Author: konrad
 */
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "usart.h"
#include "gps.h"
#include "u_ubx_protocol.h"
#include "u_error_common.h"

const char UBX_START = 0xB5; //µ
const char NMEA_START = '$';
const char NMEA_COMMA = ',';
const char NMEA_CRLF[] = "\r\n";

/* receiver default NMEA data GGA, GLL, GSA, GSV, RMC, VTG, TXT */
const char NMEA_GPGGA[] = "GPGGA";
const char NMEA_GPGGL[] = "GPGGL";
const char NMEA_GPGSA[] = "GPGSA";
const char NMEA_GPGSV[] = "GPGSV";
const char NMEA_GPRMC[] = "GPRMC";
const char NMEA_GPVTG[] = "GPVTG";
const char NMEA_GPTXT[] = "GPTXT";

UART_HandleTypeDef *gps_uart;

UBX_DATA_t ubx_data;
NMEA_DATA_t nmea_data;

static NEO7M_t NEO7M = { .ubx = &ubx_data, .nmea = &nmea_data };

// Read the position contained in a UBX-NAV_PVT message
static void gps_getPVT(size_t bufferLength) {
	NEO7M_t *neo7m = get_gps();
	const char *pBuffer = (const char*) &neo7m->BUFFER[0];

	// We have the complete raw message so need to move past the header
	pBuffer += U_UBX_PROTOCOL_HEADER_LENGTH_BYTES;
	if ((bufferLength >= U_UBX_PROTOCOL_HEADER_LENGTH_BYTES + 66)
			&& (*(pBuffer + 21) & 0x01)) {
		neo7m->ubx->longitudeX1e7 = uUbxProtocolUint32Decode(pBuffer + 24);
		neo7m->ubx->latitudeX1e7 = uUbxProtocolUint32Decode(pBuffer + 28);
		neo7m->ubx->hMSL = uUbxProtocolUint32Decode(pBuffer + 36);
		neo7m->ubx->hAcc = uUbxProtocolUint32Decode(pBuffer + 40);
		neo7m->ubx->vAcc = uUbxProtocolUint32Decode(pBuffer + 44);
		neo7m->ubx->gSpeed = uUbxProtocolUint32Decode(pBuffer + 60);
		neo7m->ubx->heading = uUbxProtocolUint32Decode(pBuffer + 64);
	}
}

static void gps_convertNMEA_LatLong() {
	NEO7M_t *neo7m = get_gps();
	int i;

	double valueTmp1;
	double valueTmp2;
	double valueTmp3;
	double valueTmp4;

	// Convert the latitude from ASCII to uint8_t values
	for (i = 0; i < 10; i++) {
		neo7m->nmea->Latitude[i] = neo7m->nmea->Latitude[i] & 0xF;
	}
	// Convert latitude from degree/minute/second (DMS) format into decimal
	valueTmp1 = (double) neo7m->nmea->Latitude[0] * 10.0
			+ (double) neo7m->nmea->Latitude[1];
	valueTmp2 = (double) neo7m->nmea->Latitude[2] * 10.0
			+ (double) neo7m->nmea->Latitude[3];
	valueTmp3 = (double) neo7m->nmea->Latitude[5] * 1000.0
			+ (double) neo7m->nmea->Latitude[6] * 100.0
			+ (double) neo7m->nmea->Latitude[7] * 10.0
			+ (double) neo7m->nmea->Latitude[8];

	neo7m->nmea->latitude = valueTmp1
			+ ((valueTmp2 + (valueTmp3 * 0.0001)) / 60.0);

	if (neo7m->nmea->LatitudePole[0] == 'S') {
		neo7m->nmea->latitude *= -1;
	}

	// Convert the longitude from ASCII to uint8_t values
	for (i = 0; i < 10; i++) {
		neo7m->nmea->Longitude[i] = neo7m->nmea->Longitude[i] & 0xF;
	}
	// Convert longitude from degree/minute/second (DMS) format into decimal
	valueTmp1 = (double) neo7m->nmea->Longitude[0] * 100.0
			+ (double) neo7m->nmea->Longitude[1] * 10.0
			+ (double) neo7m->nmea->Longitude[2];
	valueTmp2 = (double) neo7m->nmea->Longitude[3] * 10.0
			+ (double) neo7m->nmea->Longitude[4];
	valueTmp3 = (double) neo7m->nmea->Longitude[6] * 1000.0
			+ (double) neo7m->nmea->Longitude[7] * 100;
	valueTmp4 = (double) neo7m->nmea->Longitude[8] * 10.0
			+ (double) neo7m->nmea->Longitude[9];

	neo7m->nmea->longitude = valueTmp1 + (valueTmp2 / 60.0)
			+ (((valueTmp3 + valueTmp4) * 0.0001) / 60.0);

	if (neo7m->nmea->LongitudePole[0] == 'W') {
		neo7m->nmea->longitude *= -1;
	}
}

static void gps_parseNMEA(const char *pBufferStart, const char *type) {
	NEO7M_t *neo7m = get_gps();
	int i = 0;
	for (char *pTokenStart = strtok((char*) pBufferStart, &NMEA_COMMA);
			pTokenStart != NULL; pTokenStart = strtok(NULL, &NMEA_COMMA)) {
		if (i == 0) {
			strcpy(neo7m->nmea->DataType, pTokenStart + 1);
			i++;
			continue;
		}

		if (type == NMEA_GPGGA) {
			if (i == 1) {
				strcpy(neo7m->nmea->UtcTime, pTokenStart);
				i++;
				continue;
			}
			if (i == 2) {
				strcpy(neo7m->nmea->Latitude, pTokenStart);
				i++;
				continue;
			}
			if (i == 3) {
				strcpy(neo7m->nmea->LatitudePole, pTokenStart);
				i++;
				continue;
			}
			if (i == 4) {
				strcpy(neo7m->nmea->Longitude, pTokenStart);
				i++;
				continue;
			}
			if (i == 5) {
				strcpy(neo7m->nmea->LongitudePole, pTokenStart);
				i++;
				continue;
			}
			if (i == 6) {
				strcpy(neo7m->nmea->FixQuality, pTokenStart);
				i++;
				continue;
			}
			if (i == 8) {
				strcpy(neo7m->nmea->HorizontalDilution, pTokenStart);
				i++;
				continue;
			}
			if (i == 9) {
				strcpy(neo7m->nmea->Altitude, pTokenStart);
				i++;
				continue;
			}
			if (i == 10) {
				strcpy(neo7m->nmea->AltitudeUnit, pTokenStart);
				i++;
				continue;
			}
		}
		if (type == NMEA_GPGGL) {
			if (i == 6) {
				strcpy(neo7m->nmea->DataStatus, pTokenStart);
				i++;
				continue;
			}
		}
		if (type == NMEA_GPGSA) {

		}
		if (type == NMEA_GPGSV) {
			if (i == 3) {
				strcpy(neo7m->nmea->SatelliteTracked, pTokenStart);
				i++;
				continue;
			}
		}
		if (type == NMEA_GPRMC) {
			if (i == 1) {
				strcpy(neo7m->nmea->UtcTime, pTokenStart);
				i++;
				continue;
			}
			if (i == 2) {
				strcpy(neo7m->nmea->DataStatus, pTokenStart);
				i++;
				continue;
			}
			if (i == 3) {
				strcpy(neo7m->nmea->Latitude, pTokenStart);
				i++;
				continue;
			}
			if (i == 4) {
				strcpy(neo7m->nmea->LatitudePole, pTokenStart);
				i++;
				continue;
			}
			if (i == 5) {
				strcpy(neo7m->nmea->Longitude, pTokenStart);
				i++;
				continue;
			}
			if (i == 6) {
				strcpy(neo7m->nmea->LongitudePole, pTokenStart);
				i++;
				continue;
			}
			if (i == 7) {
				strcpy(neo7m->nmea->SpeedOverGround, pTokenStart);
				i++;
				continue;
			}
			if (i == 8) {
				strcpy(neo7m->nmea->CourseOverGround, pTokenStart);
				i++;
				continue;
			}
			if (i == 9) {
				strcpy(neo7m->nmea->Date, pTokenStart);
				i++;
				continue;
			}
		}
		if (type == NMEA_GPVTG) {
			if (i == 1) {
				strcpy(neo7m->nmea->CourseOverGround, pTokenStart);
				i++;
				continue;
			}
			if (i == 5) {
				strcpy(neo7m->nmea->SpeedOverGround, pTokenStart);
				i++;
				continue;
			}
		}
		if (type == NMEA_GPTXT) {

		}
		i++;
	}
}

// Read the position contained in NMEA messages GPRMC, GPVTG, GPGGA, GPGSA, GPGSV
static void gps_getNMEA() {
	NEO7M_t *neo7m = get_gps();
	if (neo7m->BUFFER[0] != NMEA_START) {
		return;
	}
	char *saveptr = NULL;
	const char *pBufferStart = strtok_r((char*) neo7m->BUFFER, NMEA_CRLF,
			&saveptr);
	while (pBufferStart != NULL) {
		// Parse GPGGA data
		if (strncmp((const char*) pBufferStart + 1, (const char*) NMEA_GPGGA, 5)
				== 0) {
			gps_parseNMEA(pBufferStart, NMEA_GPGGA);
			neo7m->nmea->hasFix =
					(neo7m->nmea->FixQuality[0] > 0x30) ? true : false;
		}
		// Parse GPGGL data
		else if (strncmp((const char*) pBufferStart + 1,
				(const char*) NMEA_GPGGL, 5) == 0) {
			gps_parseNMEA(pBufferStart, NMEA_GPGGL);
		}
		// Parse GPGSA data
		else if (strncmp((const char*) pBufferStart + 1,
				(const char*) NMEA_GPGSA, 5) == 0) {
			gps_parseNMEA(pBufferStart, NMEA_GPGSA);
		}
		// Parse GPGSV data
		else if (strncmp((const char*) pBufferStart + 1,
				(const char*) NMEA_GPGSV, 5) == 0) {
			gps_parseNMEA(pBufferStart, NMEA_GPGSV);
		}
		// Parse GPRMC data
		else if (strncmp((const char*) pBufferStart + 1,
				(const char*) NMEA_GPRMC, 5) == 0) {
			gps_parseNMEA(pBufferStart, NMEA_GPRMC);
			neo7m->nmea->hasFix =
					(neo7m->nmea->DataStatus[0] == 0x41) ? true : false;
		}
		// Parse GPVTG data
		else if (strncmp((const char*) pBufferStart + 1,
				(const char*) NMEA_GPVTG, 5) == 0) {
			gps_parseNMEA(pBufferStart, NMEA_GPVTG);
		}
		// Parse GPTXT data
		else if (strncmp((const char*) pBufferStart + 1,
				(const char*) NMEA_GPTXT, 5) == 0) {
			gps_parseNMEA(pBufferStart, NMEA_GPTXT);
		}

		pBufferStart = strtok_r(NULL, NMEA_CRLF, &saveptr);
	}

	if (neo7m->nmea->hasFix) {
		gps_convertNMEA_LatLong();
		neo7m->nmea->altitude = atoi(neo7m->nmea->Altitude);
	}
}

//GPS
void gps_init(UART_HandleTypeDef *uart) {
	gps_uart = uart;
	NEO7M_t *neo7m = get_gps();
	neo7m->status = HAL_OK;

	neo7m->nmea->hasFix = false;
	neo7m->nmea->latitude = 0.0;
	neo7m->nmea->longitude = 0.0;
	neo7m->nmea->altitude = (int16_t) 0xFFFF;
}

HAL_StatusTypeDef gps_config_ubx() {
	NEO7M_t *neo7m = get_gps();
	unsigned char ackData[200] = { 0 };

	if (HAL_OK != (neo7m->status = HAL_UART_Abort(gps_uart))) {
		return neo7m->status;
	}
	if (HAL_OK != (neo7m->status = HAL_UART_AbortReceive(gps_uart))) {
		return neo7m->status;
	}
	if (HAL_OK != (neo7m->status = HAL_UART_AbortTransmit(gps_uart))) {
		return neo7m->status;
	}

	//set CFG-TP5 (0x06 0x31)
	//gps_getPPS(neo7m);

	//<- UBX CFG-PRT: PORT UBX 115200
	unsigned char pData4[] = { 0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00,
			0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, 0x00, 0xC2, 0x01, 0x00, 0x01,
			0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB8, 0x42 };
	neo7m->status = HAL_UART_Transmit(gps_uart, pData4, sizeof(pData4), 3000);
	//-> ACK
	neo7m->status = HAL_UART_Receive(gps_uart, ackData, sizeof(ackData), 5000);
	HAL_Delay(100);
	//<- UBX CFG
	unsigned char pData5[] = { 0xB5, 0x62, 0x06, 0x00, 0x01, 0x00, 0x01, 0x08,
			0x22 };
	neo7m->status = HAL_UART_Transmit(gps_uart, pData5, sizeof(pData5), 3000);
	//-> ACK
	neo7m->status = HAL_UART_Receive(gps_uart, ackData, sizeof(ackData), 5000);
	HAL_Delay(100);

	//<- UBX CFG-NAVX5 AssistNow Autonomous
	unsigned char pData6[] = { 0xB5, 0x62, 0x06, 0x23, 0x28, 0x00, 0x00, 0x00,
			0x4C, 0x66, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x10, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x9B, 0x06, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0xDC, 0x6E };
	neo7m->status = HAL_UART_Transmit(gps_uart, pData6, sizeof(pData6), 3000);
	//-> ACK
	neo7m->status = HAL_UART_Receive(gps_uart, ackData, sizeof(ackData), 5000);
	HAL_Delay(100);
	//<- UBX CFG
	unsigned char pData7[] = { 0xB5, 0x62, 0x06, 0x23, 0x00, 0x00, 0x29, 0x81 };
	neo7m->status = HAL_UART_Transmit(gps_uart, pData7, sizeof(pData7), 3000);
	//-> ACK
	neo7m->status = HAL_UART_Receive(gps_uart, ackData, sizeof(ackData), 5000);
	HAL_Delay(100);

	//<- UBX CFG: ENABLE UBX NAV-PVT
	unsigned char pData8[] = { 0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0x01, 0x07,
			0x01, 0x13, 0x51 };
	neo7m->status = HAL_UART_Transmit(gps_uart, pData8, sizeof(pData8), 3000);
	//-> ACK
	neo7m->status = HAL_UART_Receive(gps_uart, ackData, sizeof(ackData), 5000);
	HAL_Delay(100);
	//<- UBX CFG
	neo7m->status = HAL_UART_Transmit(gps_uart, pData5, sizeof(pData5), 1000);
	//-> ACK
	neo7m->status = HAL_UART_Receive(gps_uart, ackData, sizeof(ackData), 5000);
	HAL_Delay(100);
	return neo7m->status;
}

void gps_get_data() {
	NEO7M_t *neo7m = get_gps();
	HAL_UART_Receive(gps_uart, neo7m->BUFFER, sizeof(neo7m->BUFFER), 1000);

	const char *pBufferStart = (const char*) &neo7m->BUFFER[0];
	size_t bufferLength = sizeof(neo7m->BUFFER);
	const char *pBufferEnd = pBufferStart;
	int32_t messageClass;
	int32_t messageId;

	for (int32_t x = uUbxProtocolDecode(pBufferStart, bufferLength,
			&messageClass, &messageId, neo7m->MESSAGE, sizeof(neo7m->MESSAGE),
			&pBufferEnd); x > 0;
			x = uUbxProtocolDecode(pBufferStart, bufferLength, &messageClass,
					&messageId, neo7m->MESSAGE, sizeof(neo7m->MESSAGE),
					&pBufferEnd)) {

		if (x > sizeof(neo7m->MESSAGE)) {
			x = sizeof(neo7m->MESSAGE);
		}

		// Handle the message here

		bufferLength -= pBufferEnd - pBufferStart;
		pBufferStart = pBufferEnd;
	}
	if (neo7m->BUFFER[0] == UBX_START && neo7m->MESSAGE[0] != 0) {
		gps_getPVT(bufferLength);
	} else if (neo7m->BUFFER[0] == NMEA_START) {
		gps_getNMEA();
	}
}

inline NEO7M_t* get_gps() {
	return &NEO7M;
}
