/*
 * bridge.c
 *
 *  Created on: Jan 4, 2024
 *      Author: konrad
 */
#include "init.h"
#include "bridge.h"
#include "buzzer.h"
#include "flight_control.h"

// L1, L2, R1, R2, EDF_L1, EDF_R1, SERVO
static volatile bool arming = false; // motor start throttles
const char *arming_sequence_fc_to_motor = "4200;4200;4200;4200;1400;1400;0075;";

BRIDGE_COMMON_t fc_port =
		{ .signalStatusTransmit = 0, .signalStatusReceive = 0 };
static BRIDGE_t fc_bridge = { .port = &fc_port };

inline static void bridge_store_armed(bool value) {
	atomic_store_explicit(&fc_bridge.armed, value, *get_memory_order());
}

static bool bridge_check_arming(char *data) {
	// signal to arm escs, get motor data from FC, sent 4os42, 2edf, 1servo
	return (strncmp((const char*) data, arming_sequence_fc_to_motor,
	BRIDGE_DATA_SIZE_OUT) == 0) && fc_port.signalStatusReceive == 0
			&& fc_port.signalStatusTransmit == 0;
}

static HAL_StatusTypeDef transmit() {
	return HAL_UART_Transmit_IT(fc_bridge.fc_uart,
			(const uint8_t*) fc_bridge.port->data_out,
			strlen(fc_bridge.port->data_out));
}

static HAL_StatusTypeDef receive() {
	return HAL_UART_Receive_IT(fc_bridge.fc_uart, fc_bridge.rx_data,
			sizeof(fc_bridge.rx_data));
}

static void bridge_signal_transmit_handler(int status) {
	fc_bridge.port->signalStatusTransmit = status;
	HAL_StatusTypeDef status_transmit = fc_bridge.port->transmit();
	if (HAL_OK != status_transmit) {
		//start buzzer
		buzzer_start(20);
	}
}

static void bridge_signal_receive_handler(int status) {
	fc_bridge.port->signalStatusReceive = status;
	HAL_StatusTypeDef status_receive = fc_bridge.port->receive();
	if (HAL_OK != status_receive) {
		//start buzzer
		buzzer_start(40);
	}
}

// process in bridge rx callback
static void bridge_radio_control(char *data) {
	int i = 0;
	char *saveptr = NULL;
	const char *pBufferStart = strtok_r(data, bridge_get_semicolon(), &saveptr);
	uint32_t *radio_channel = bridge_get_radio_commands();
	while (pBufferStart != NULL) {
		//queue_enqueue(get_queues_rc() + i++, atoi(pBufferStart));
		radio_channel[i++] = atoi(pBufferStart);
		pBufferStart = strtok_r(NULL, bridge_get_semicolon(), &saveptr);
	}
}

static char* bridge_get_motors_data() {
	char *uart_data = (char*) malloc(BRIDGE_DATA_SIZE_OUT + 1);
	char char_data;
	const int seq_len = bridge_get_sequence_length();
	for (int i = 0; i < bridge_get_motors_number(); i++) {
		itoa(*(bridge_get_motor_commands() + i), &char_data, 10);
		bridge_fill_sequence(&char_data);
		strncpy(uart_data + i * seq_len, bridge_get_sequence(), seq_len);
	}
	uart_data[BRIDGE_DATA_SIZE_OUT] = '\0';
	return uart_data;
}

static void bridge_fc_rx_callback(UART_HandleTypeDef *huart) {
	//char str[] = "1097;1919;1508;1508;1919;1919;"; //data in
	bool data_valid = bridge_validate_data((char*) fc_bridge.rx_data,
			BRIDGE_DATA_SIZE_IN);
	if (data_valid) {
		bridge_radio_control((char*) fc_bridge.rx_data);
		//TODO 1 processing FC
		flight_data_control(bridge_get_radio_commands(),
				bridge_get_motor_commands());
	}
	//TODO prepare FC data to motors
	char *data = bridge_get_motors_data();
	fc_bridge.port->data_out = data;

	HAL_StatusTypeDef status_transmit = fc_bridge.port->transmit(); // send motor data to motor controller D1
	if (HAL_OK != status_transmit) {
		raise(bridge_get_transmit_sigint());
	}
	HAL_StatusTypeDef status_receive = fc_bridge.port->receive();
	if (HAL_OK != status_receive) {
		raise(bridge_get_receive_sigint());
	}
	if (!arming) {
		arming = bridge_check_arming(data);
		if (arming) {
			bridge_store_armed(true);
		}
	}
	free(data);
	data = NULL;
}

HAL_StatusTypeDef bridge_init(UART_HandleTypeDef *uart) {
	signal(bridge_get_receive_sigint(), &bridge_signal_receive_handler);
	signal(bridge_get_transmit_sigint(), &bridge_signal_transmit_handler);
	fc_bridge.fc_uart = uart;
	fc_bridge.port->transmit = &transmit;
	fc_bridge.port->receive = &receive;
	bridge_store_armed(false);
	HAL_StatusTypeDef status = HAL_UART_RegisterCallback(fc_bridge.fc_uart,
			HAL_UART_RX_COMPLETE_CB_ID, bridge_fc_rx_callback);
	if (HAL_OK != status) {
		return status;
	}
	return fc_bridge.port->receive();
}

HAL_StatusTypeDef bridge_transmit(char *data) {
	HAL_UART_StateTypeDef uart_state = HAL_UART_GetState(fc_bridge.fc_uart);
	HAL_StatusTypeDef status = HAL_BUSY;
	if (HAL_UART_STATE_READY == uart_state
			|| HAL_UART_STATE_BUSY_RX == uart_state) {
		fc_bridge.port->data_out = data; // "4200;4200;4200;4200;1400;1400;0075;"
		status = fc_bridge.port->transmit();
		if (HAL_OK != status) {
			return status;
		}
		return fc_bridge.port->receive();
	}
	return status;
}

inline bool bridge_get_armed() {
	return atomic_load_explicit(&fc_bridge.armed, *get_memory_order());
}
