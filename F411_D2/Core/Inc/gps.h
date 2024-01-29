/*
 * gps.h
 *
 *  Created on: Oct 26, 2023
 *      Author: konrad
 */

#include <stdbool.h>
#include "stm32f4xx.h"

#ifndef INC_GPS_H_
#define INC_GPS_H_

// UBX data structure, Common Power Mode (microprocessor noise ?)
typedef struct UBX_DATA {
	int32_t longitudeX1e7;
	int32_t latitudeX1e7;
	int32_t hMSL; 		// Height above mean sea level mm
	uint32_t hAcc; 		// Horizontal Accuracy Estimate mm
	uint32_t vAcc; 		// Vertical Accuracy Estimate mm
	int32_t gSpeed; 	// Ground Speed (2-D) mm/s
	int32_t heading; 	// Heading of motion 2-D deg
} UBX_DATA_t;

// NMEA data structure, Isolated Power Mode (quick fix TTFF ?)
// activated at start-up GGA, GLL, GSA, GSV, RMC, VTG, TXT
typedef struct NMEA_DATA {
	char DataType[6];
	char UtcTime[11];
	char DataStatus[2];
	char Latitude[10];
	char LatitudePole[2];
	char Longitude[11];
	char LongitudePole[2];
	char FixQuality[2];
	char SatelliteTracked[3];
	char HorizontalDilution[6];
	char Altitude[8];
	char AltitudeUnit[2];
	char HeightGeoid[8];
	char HeightGeoidUnit[2];
	char SpeedOverGround[8]; 	// RMC, VTG 	knots
	char CourseOverGround[8]; 	// RMC, VTG 	deg 	detection angle
	char Date[8];

	bool hasFix;
	double latitude;
	double longitude;
	int16_t altitude;
} NMEA_DATA_t;

// GPS structure
typedef struct NEO7M {
	HAL_StatusTypeDef status;
	uint8_t BUFFER[334]; //6 UBX header
	char MESSAGE[92];	// 92 UVC-NAV-PVT

	UBX_DATA_t *ubx;
	NMEA_DATA_t *nmea;
} NEO7M_t;

NEO7M_t* get_gps(void);
void gps_init(UART_HandleTypeDef *uart);
HAL_StatusTypeDef gps_config_ubx(void);
void gps_get_data(void);
#endif /* INC_GPS_H_ */
