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

// THROTTLE, PITCH, ROLL, YAW, GEAR, SPEED
static volatile bool arming = false; // radio joy-sticks down to center + D and F \ /
const int arming_sequence_radio_approx[6] =
		{ 1200, 1200, 1900, 1200, 1900, 1900 }; //"1100;1100;1923;1098;1919;1919;"

BRIDGE_COMMON_t fc_port =
		{ .signalStatusTransmit = 0, .signalStatusReceive = 0 };
static BRIDGE_t fc_bridge = { .port = &fc_port };

static bool bridge_check_arming() {
	return (*bridge_get_radio_commands() < arming_sequence_radio_approx[0])
			&& (*(bridge_get_radio_commands() + 1)
					< arming_sequence_radio_approx[1])
			&& (*(bridge_get_radio_commands() + 2)
					> arming_sequence_radio_approx[2])
			&& (*(bridge_get_radio_commands() + 3)
					< arming_sequence_radio_approx[3])
			&& (*(bridge_get_radio_commands() + 4)
					> arming_sequence_radio_approx[4])
			&& (*(bridge_get_radio_commands() + 5)
					> arming_sequence_radio_approx[5]);
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
static void bridge_motor_control(char *data) {
	int i = 0;
	char *saveptr = NULL;
	const char *pBufferStart = strtok_r(data, bridge_get_semicolon(), &saveptr);
	uint32_t *motors = bridge_get_motor_commands();
	while (pBufferStart != NULL) {
		motors[i++] = atoi(pBufferStart);
		pBufferStart = strtok_r(NULL, bridge_get_semicolon(), &saveptr);
	}
}

static char* bridge_get_radio_data() {
	char *uart_data = (char*) malloc(BRIDGE_DATA_SIZE_OUT + 1);
	uint32_t *radio_channel = bridge_get_radio_commands();
	char char_data;
	const int seq_len = bridge_get_sequence_length();
	for (int i = 0; i < bridge_get_radio_channels_number(); i++) {
		itoa(radio_channel[i], &char_data, 10);
		bridge_fill_sequence(&char_data);
		strncpy(uart_data + i * seq_len, bridge_get_sequence(), seq_len);
	}
	if (!arming) {
		arming = bridge_check_arming();
	}
	uart_data[BRIDGE_DATA_SIZE_OUT] = '\0';
	return uart_data;
}

static void bridge_rc_motor_rx_callback(UART_HandleTypeDef *huart) {
	bool data_valid = bridge_validate_data((char*) fc_bridge.rx_data,
			BRIDGE_DATA_SIZE_IN);
	if (data_valid) {
		bridge_motor_control((char*) fc_bridge.rx_data);
		// TODO 2 set to motors
		flight_data_control(bridge_get_radio_commands(),
				bridge_get_motor_commands());
	}
	// get radio data from queues
	char *data = bridge_get_radio_data();
	fc_bridge.port->data_out = data;

	// TODO process Servo channel GEAR
	HAL_StatusTypeDef status_transmit = fc_bridge.port->transmit(); // send radio data of motors to flight controller D2
	free(data);
	data = NULL;
	if (HAL_OK != status_transmit) {
		raise(bridge_get_transmit_sigint());
	}
	HAL_StatusTypeDef status_receive = fc_bridge.port->receive();
	if (HAL_OK != status_receive) {
		raise(bridge_get_receive_sigint());
	}
}

HAL_StatusTypeDef bridge_drone_arm() {
	// TODO signal to arm escs, get radio data from RC, sent TPRY+Gear+Speed
	HAL_StatusTypeDef status = HAL_OK;
	char *data = bridge_get_radio_data();
	if (arming) {
		fc_bridge.port->data_out = data;
		status = fc_bridge.port->transmit(); //send radio data to flight controller
		if (HAL_OK == status) {
			status = fc_bridge.port->receive();
		}
		if (HAL_OK == status) {
			bridge_store_armed(true);
		}
	}
	free(data);
	data = NULL;
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

inline bool bridge_get_armed() {
	return atomic_load_explicit(&fc_bridge.armed, *get_memory_order());
}
