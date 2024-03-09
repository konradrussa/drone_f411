/*
 * bridge.c
 *
 *  Created on: Jan 4, 2024
 *      Author: konrad
 */
#include "init.h"
#include "bridge.h"
#include "remote_control.h"
#include "flight_control.h"

static bool is_arming = false;

BRIDGE_COMMON_t fc_port =
		{ .signalStatusTransmit = 0, .signalStatusReceive = 0 };
static BRIDGE_t fc_bridge = { .port = &fc_port };

static bool is_armed = false;

// THROTTLE, PITCH, ROLL, YAW, GEAR, SPEED
// "1100;1100;1923;1098;1919;1919;"
// arming - radio joy-sticks down to center + D and F \ /
const int arming_sequence_radio[6] = { 1200, 1200, 1900, 1200, 1900, 1900 };

static bool bridge_check_radio_arming() {
	uint32_t *radio_cmds = bridge_get_radio_commands();
	return (radio_cmds[0] < arming_sequence_radio[0])
			&& (radio_cmds[1] < arming_sequence_radio[1])
			&& (radio_cmds[2] > arming_sequence_radio[2])
			&& (radio_cmds[3] < arming_sequence_radio[3])
			&& (radio_cmds[4] > arming_sequence_radio[4])
			&& (radio_cmds[5] > arming_sequence_radio[5]);
}

inline static void bridge_store_armed(bool value) {
	atomic_store_explicit(&fc_bridge.armed, value, *get_memory_order());
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
	HAL_StatusTypeDef status_transmit = fc_bridge.port->transmit(); // try retransmission
	if (HAL_OK != status_transmit && !get_imu()->running) {
		// Initialize recovery ACCEL + GYRO
		if (BMI160_OK == imu_init(fc_bridge.imu_i2c)) {
			get_imu()->running = true;
		}
	}
}

static void bridge_signal_receive_handler(int status) {
	fc_bridge.port->signalStatusReceive = status;
	HAL_StatusTypeDef status_receive = fc_bridge.port->receive(); // try retransmission
	if (HAL_OK != status_receive && !get_imu()->running) {
		// Initialize recovery ACCEL + GYRO
		if (BMI160_OK == imu_init(fc_bridge.imu_i2c)) {
			get_imu()->running = true;
		}
	}
}

// process in bridge rx callback
static void bridge_fc_feedback(char *data) {
	int i = 0;
	char *saveptr = NULL;
	const char *pBufferStart = strtok_r(data, bridge_get_semicolon(), &saveptr);
	uint32_t *motor_cmds = bridge_get_motor_commands();
	while (pBufferStart != NULL) {
		motor_cmds[i++] = atoi(pBufferStart);
		pBufferStart = strtok_r(NULL, bridge_get_semicolon(), &saveptr);
	}
}

static char* bridge_get_radio_data() {
	char *uart_data = (char*) malloc(BRIDGE_DATA_SIZE_OUT + 1);
	uint32_t *radio_cmds = bridge_get_radio_commands();
	char char_data;
	const int seq_len = bridge_get_sequence_length();
	for (int i = 0; i < bridge_get_radio_channels_number(); i++) {
		itoa(radio_cmds[i], &char_data, 10);
		bridge_fill_sequence(&char_data);
		strncpy(uart_data + i * seq_len, bridge_get_sequence(), seq_len);
	}
	uart_data[BRIDGE_DATA_SIZE_OUT] = '\0';
	return uart_data;
}

static void bridge_rc_motor_rx_callback(UART_HandleTypeDef *huart) {
	bool data_valid = bridge_validate_data((char*) fc_bridge.rx_data,
	BRIDGE_DATA_SIZE_IN);
	if (data_valid) {
		bridge_fc_feedback((char*) fc_bridge.rx_data);
		flight_data_control(bridge_get_radio_commands(),
				bridge_get_motor_commands());
	}
	// get radio data from radio commands
	fc_bridge.port->data_out = bridge_get_radio_data();
	if (HAL_OK != fc_bridge.port->transmit()) { // send radio data of motors to flight controller D2
		raise(bridge_get_transmit_sigint());
	}
	if (HAL_OK != fc_bridge.port->receive()) {
		raise(bridge_get_receive_sigint());
	}
	if (!is_armed) {
		if (bridge_check_fc_arming()) {
			is_armed = true;
			bridge_store_armed(true);
		}
	}
	free(fc_bridge.port->data_out);
	fc_bridge.port->data_out = NULL;
}

HAL_StatusTypeDef bridge_drone_arming() {
	// signal to arm escs, get radio data from RC, sent TPRY+Gear+Speed
	HAL_StatusTypeDef status = HAL_OK;
	fc_bridge.port->data_out = bridge_get_radio_data();
	if (bridge_check_radio_arming()) {
		status = fc_bridge.port->transmit(); //send radio data to flight controller
		if (HAL_OK == status) {
			status = fc_bridge.port->receive();
		}
		if (HAL_OK == status) {
			is_arming = true;
		}
	}
	free(fc_bridge.port->data_out);
	fc_bridge.port->data_out = NULL;
	return status;
}

HAL_StatusTypeDef bridge_init(UART_HandleTypeDef *uart, I2C_HandleTypeDef *i2c) {
	signal(bridge_get_receive_sigint(), &bridge_signal_receive_handler);
	signal(bridge_get_transmit_sigint(), &bridge_signal_transmit_handler);
	fc_bridge.fc_uart = uart;
	fc_bridge.imu_i2c = i2c;
	fc_bridge.port->transmit = &transmit;
	fc_bridge.port->receive = &receive;
	bridge_store_armed(false);
	return HAL_UART_RegisterCallback(fc_bridge.fc_uart,
			HAL_UART_RX_COMPLETE_CB_ID, bridge_rc_motor_rx_callback);
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

inline bool bridge_is_arming() {
	return is_arming;
}

inline bool bridge_get_armed() {
	return atomic_load_explicit(&fc_bridge.armed, *get_memory_order());
}
