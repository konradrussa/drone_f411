/*
 * bridge.c
 *
 *  Created on: Jan 4, 2024
 *      Author: konrad
 */
#include "bridge.h"

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
	HAL_StatusTypeDef status_transmit = fc_bridge.port->transmit(); // try retransmission
	if (HAL_OK != status_transmit && !get_imu()->running) {
		// Initialize recovery ACCEL + GYRO
		if (BMI160_OK == imu_init(fc_bridge.imu_i2c)) {
			get_imu()->running = true;
		}
	}
}

inline static void bridge_signal_receive_handler(int status) {
	fc_bridge.port->signalStatusReceive = status;
	HAL_StatusTypeDef status_receive = fc_bridge.port->receive(); // try retransmission
	if (HAL_OK != status_receive && !get_imu()->running) {
		// Initialize recovery ACCEL + GYRO
		if (BMI160_OK == imu_init(fc_bridge.imu_i2c)) {
			get_imu()->running = true;
		}
	}
}

static void bridge_rc_motor_rx_callback(UART_HandleTypeDef *huart) {
	bool data_valid = bridge_validate_data((char*) fc_bridge.rx_data,
	BRIDGE_DATA_SIZE);
	if (data_valid) {
		fc_bridge.port->data_in = bridge_format_string(
				(char*) fc_bridge.rx_data); // allocate memory
		// TODO 2 set to motors
		free(fc_bridge.port->data_in); // deallocate memory
		fc_bridge.port->data_in = NULL;
	}
	// TODO 1 get radio data from queues
	fc_bridge.port->data_out = bridge_get_queues_data();

	// TODO process Servo channel GEAR
	HAL_StatusTypeDef status_transmit = fc_bridge.port->transmit(); // send radio data of motors to flight controller D2
	free(fc_bridge.port->data_out);
	fc_bridge.port->data_out = NULL;
	if (HAL_OK != status_transmit) {
		raise(bridge_get_transmit_sigint());
	}
	HAL_StatusTypeDef status_receive = fc_bridge.port->receive();
	if (HAL_OK != status_receive) {
		raise(bridge_get_receive_sigint());
	}
}

//TODO create bridge data out
char* bridge_get_queues_data() {
	uint32_t data;
	char *uart_data = (char*) malloc(BRIDGE_DATA_SIZE);
	if (queue_data_available(get_throttle_queue_rc())) {
		queue_dequeue(get_throttle_queue_rc(), &data);
		strcat(itoa(data, uart_data, 10), bridge_get_semicolon());
	}
	if (queue_data_available(get_pitch_queue_rc())) {
		queue_dequeue(get_pitch_queue_rc(), &data);
		strcat(itoa(data, uart_data, 10), bridge_get_semicolon());
	}
	if (queue_data_available(get_roll_queue_rc())) {
		queue_dequeue(get_roll_queue_rc(), &data);
		strcat(itoa(data, uart_data, 10), bridge_get_semicolon());
	}
	if (queue_data_available(get_yaw_queue_rc())) {
		queue_dequeue(get_yaw_queue_rc(), &data);
		strcat(itoa(data, uart_data, 10), bridge_get_semicolon());
	}
	if (queue_data_available(get_gear_queue_rc())) {
		queue_dequeue(get_gear_queue_rc(), &data);
		strcat(itoa(data, uart_data, 10), bridge_get_semicolon());
	}
	if (queue_data_available(get_speed_queue_rc())) {
		queue_dequeue(get_speed_queue_rc(), &data);
		strcat(itoa(data, uart_data, 10), bridge_get_semicolon());
	}
	return uart_data;
}

HAL_StatusTypeDef bridge_init(UART_HandleTypeDef *uart, I2C_HandleTypeDef *i2c) {
	signal(bridge_get_receive_sigint(), &bridge_signal_receive_handler);
	signal(bridge_get_transmit_sigint(), &bridge_signal_transmit_handler);
	fc_bridge.fc_uart = uart;
	fc_bridge.imu_i2c = i2c;
	fc_bridge.port->transmit = &transmit;
	fc_bridge.port->receive = &receive;
	return HAL_UART_RegisterCallback(fc_bridge.fc_uart,
			HAL_UART_RX_COMPLETE_CB_ID, bridge_rc_motor_rx_callback);
}

HAL_StatusTypeDef bridge_drone_arm() {
	// TODO signal to arm escs, get radio data from RC, sent TPRY+Gear+Speed
	fc_bridge.port->data_out = "1100;1100;1923;1098;1919;1919;";
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
		fc_bridge.port->data_out = data; // "1097;1919;1508;1508;1919;1111;";
		status = fc_bridge.port->transmit();
		if (HAL_OK != status) {
			return status;
		}
		return fc_bridge.port->receive();
	}
	return status;
}
