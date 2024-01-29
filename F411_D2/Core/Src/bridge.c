/*
 * bridge.c
 *
 *  Created on: Jan 4, 2024
 *      Author: konrad
 */
#include "bridge.h"
#include "buzzer.h"

BRIDGE_COMMON_t fc_port =
		{ .signalStatusTransmit = 0, .signalStatusReceive = 0 };
static BRIDGE_t fc_bridge = { .port = &fc_port };

static HAL_StatusTypeDef transmit() {
	return HAL_UART_Transmit_IT(fc_bridge.fc_uart,
			(const uint8_t*) fc_bridge.port->data_out,
			strlen(fc_bridge.port->data_out));
}

static HAL_StatusTypeDef receive() {
	return HAL_UART_Receive_IT(fc_bridge.fc_uart, fc_bridge.rx_data,
			sizeof(fc_bridge.rx_data));
}

inline static void bridge_signal_transmit_handler(int status) {
	fc_bridge.port->signalStatusTransmit = status;
	HAL_StatusTypeDef status_transmit = fc_bridge.port->transmit();
	if (HAL_OK != status_transmit) {
		//start buzzer
		buzzer_start(20);
	}
}

inline static void bridge_signal_receive_handler(int status) {
	fc_bridge.port->signalStatusReceive = status;
	HAL_StatusTypeDef status_receive = fc_bridge.port->receive();
	if (HAL_OK != status_receive) {
		//start buzzer
		buzzer_start(40);
	}
}

// process in bridge rx callback
static void flight_bridge_queue_control(char *data) {
	int i = 0;
	char *saveptr = NULL;
	const char *pBufferStart = strtok_r(data, bridge_get_semicolon(), &saveptr);
	while (pBufferStart != NULL) {
		queue_enqueue(get_queues_rc() + i++, atoi(pBufferStart));
		pBufferStart = strtok_r(NULL, bridge_get_semicolon(), &saveptr);
	}
}

static void bridge_fc_rx_callback(UART_HandleTypeDef *huart) {
	//char str[] = "1097;1919;1508;1508;1919;1919;"; //data in
	bool data_valid = bridge_validate_data((char*) fc_bridge.rx_data,
	BRIDGE_DATA_SIZE);
	if (data_valid) {
		flight_bridge_queue_control((char*) fc_bridge.rx_data);
	}
	// TODO get motors data from FC
	fc_bridge.port->data_out = "4200;4200;4200;4200;1400;1400;0075;";
	HAL_StatusTypeDef status_transmit = fc_bridge.port->transmit(); // send motor data to motor controller D1
	if (HAL_OK != status_transmit) {
		raise(bridge_get_transmit_sigint());
	}
	HAL_StatusTypeDef status_receive = fc_bridge.port->receive();
	if (HAL_OK != status_receive) {
		raise(bridge_get_receive_sigint());
	}
}

HAL_StatusTypeDef bridge_init(UART_HandleTypeDef *uart) {
	signal(bridge_get_receive_sigint(), &bridge_signal_receive_handler);
	signal(bridge_get_transmit_sigint(), &bridge_signal_transmit_handler);
	fc_bridge.fc_uart = uart;
	fc_bridge.port->transmit = &transmit;
	fc_bridge.port->receive = &receive;
	HAL_StatusTypeDef status = HAL_UART_RegisterCallback(fc_bridge.fc_uart,
			HAL_UART_RX_COMPLETE_CB_ID, bridge_fc_rx_callback);
	if (HAL_OK != status) {
		return status;
	}
	return fc_bridge.port->receive();
}

HAL_StatusTypeDef bridge_drone_arm() {
	// TODO signal to arm escs, get motor data from FC, sent 4os42, 2edf, 1servo
	fc_bridge.port->data_out = "4200;4200;4200;4200;1400;1400;0075;";
	HAL_StatusTypeDef status_transmit = fc_bridge.port->transmit(); //send radio data to flight controller
	if (HAL_OK != status_transmit) {
		return status_transmit;
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
