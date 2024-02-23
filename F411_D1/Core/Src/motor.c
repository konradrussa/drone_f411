/*
 * motor.c
 *
 *  Created on: Dec 2, 2023
 *      Author: konrad
 */
#include "motor.h"

//motors signals
const short M_L1_SIGINT = 100;
const short M_L2_SIGINT = 101;
const short M_EDF_L1_SIGINT = 102;
const short M_R1_SIGINT = 103;
const short M_R2_SIGINT = 104;
const short M_EDF_R1_SIGINT = 105;
//motors constraints
const short DSHOT_FRAME_SIZE = 16;

const short MOTOR_START_THROTTLE = 50;
const short MOTOR_START_THROTTLE_EDF = 1400; //to 2000; 100-1 2000-1 1400 500Hz; 2000-1 1000-1 70 50Hz; duty 70%
const short MOTOR_START_THROTTLE_ONESHOT42 = 4200; //to 8400
const short MOTOR_START_THROTTLE_MULTISHOT = 500; //to 2500
const short MOTOR_START_THROTTLE_DSHOT = 548; //0.25 * 2000 + 48 = 548

static MOTOR_t M_L1;
static MOTOR_t M_L2;
static MOTOR_t M_EDF_L1;
static MOTOR_t M_R1;
static MOTOR_t M_R2;
static MOTOR_t M_EDF_R1;

static volatile bool initialized = false;

inline static void M_L1_signal_handler(int status) {
	M_L1.motorSignalStatus = status;
}
inline static void M_L2_signal_handler(int status) {
	M_L2.motorSignalStatus = status;
}
inline static void M_EDF_L1_signal_handler(int status) {
	M_EDF_L1.motorSignalStatus = status;
}
inline static void M_R1_signal_handler(int status) {
	M_R1.motorSignalStatus = status;
}
inline static void M_R2_signal_handler(int status) {
	M_R2.motorSignalStatus = status;
}
inline static void M_EDF_R1_signal_handler(int status) {
	M_EDF_R1.motorSignalStatus = status;
}

static void motors_pre_init() {
	if (!initialized) {
		M_L1.motor_signal = M_L1_SIGINT;
		signal(M_L1_SIGINT, &M_L1_signal_handler);
		M_L2.motor_signal = M_L2_SIGINT;
		signal(M_L2_SIGINT, &M_L2_signal_handler);
		M_EDF_L1.motor_signal = M_EDF_L1_SIGINT;
		signal(M_EDF_L1_SIGINT, &M_EDF_L1_signal_handler);
		M_R1.motor_signal = M_R1_SIGINT;
		signal(M_R1_SIGINT, &M_R1_signal_handler);
		M_R2.motor_signal = M_R2_SIGINT;
		signal(M_R2_SIGINT, &M_R2_signal_handler);
		M_EDF_R1.motor_signal = M_EDF_R1_SIGINT;
		signal(M_EDF_R1_SIGINT, &M_EDF_R1_signal_handler);
		initialized = true;
	}
}

inline static void store_dma_dshot(MOTOR_t *motor, bool value) {
	atomic_store_explicit(&motor->dma_dshot, value, *get_memory_order());
}

inline static void motor_store_started(MOTOR_t *motor, bool value) {
	atomic_store_explicit(&motor->started, value, *get_memory_order());
}

static void fill_buffer_dshot(MOTOR_t *motor, uint16_t frame) {
	for (int i = 0; i < DSHOT_FRAME_SIZE; i++) {
		motor->buffer[i] = (frame >> i) & 1;
	}
	motor->buffer[16] = 0;
	motor->buffer[17] = 0;
}

static uint16_t dshot_frame(uint16_t value) {
	uint16_t frame_telemetry = (value << 1) | 0;
	uint16_t crc = (frame_telemetry ^ (frame_telemetry >> 4)
			^ (frame_telemetry >> 8)) & 0x0F;
	return (frame_telemetry << 4) | (crc & 0x0F);
}

HAL_StatusTypeDef motor_init_all() {
// Pre-Init
	motors_pre_init();
// Initialize Rotors
	HAL_StatusTypeDef status = HAL_OK;
	status = motor_init_pwm(get_M_L1(), &htim1, TIM_CHANNEL_1, MOTOR_ONESHOT42);
	if (HAL_OK != status) {
		return status;
	}
	status = motor_init_pwm(get_M_L2(), &htim1, TIM_CHANNEL_2, MOTOR_ONESHOT42);
	if (HAL_OK != status) {
		return status;
	}
	status = motor_init_pwm(get_M_R1(), &htim1, TIM_CHANNEL_3, MOTOR_ONESHOT42);
	if (HAL_OK != status) {
		return status;
	}
	status = motor_init_pwm(get_M_R2(), &htim1, TIM_CHANNEL_4, MOTOR_ONESHOT42);
	if (HAL_OK != status) {
		return status;
	}
// Initialize EDFs
	status = motor_init_pwm(get_M_EDF_L1(), &htim5, TIM_CHANNEL_1,
			MOTOR_PWM_EDF);
	if (HAL_OK != status) {
		return status;
	}
	status = motor_init_pwm(get_M_EDF_R1(), &htim5, TIM_CHANNEL_2,
			MOTOR_PWM_EDF);
	if (HAL_OK != status) {
		return status;
	}
	return status;
}

HAL_StatusTypeDef motor_init_pwm(MOTOR_t *motor, TIM_HandleTypeDef *tim,
		uint32_t channel, MOTOR_SIGNAL_e type) {
	motor->pwm_tim = tim;
	motor->pwm_channel = channel;
	motor->signal_type = type;
	HAL_StatusTypeDef status = HAL_TIM_PWM_Start(tim, channel);
	if (HAL_OK == status) {
		store_dma_dshot(motor, false);
		motor_store_started(motor, true);
	} else {
		raise(motor->motor_signal);
	}
	return status;
}

HAL_StatusTypeDef motor_init_dma_dshot(MOTOR_t *motor, TIM_HandleTypeDef *tim,
		uint32_t channel, MOTOR_SIGNAL_e type) {
	motor->pwm_tim = tim;
	motor->pwm_channel = channel;
	motor->signal_type = type;
	uint16_t data_size = sizeof(uint32_t) * DSHOT_SENT_SIZE;
	motor->buffer = (uint32_t*) malloc(data_size);
	HAL_StatusTypeDef status = HAL_TIM_PWM_Start_DMA(motor->pwm_tim,
			motor->pwm_channel, motor->buffer, data_size); //TODO CHECK was sizeof(motor->buffer) buffer[18]
	if (HAL_OK == status) {
		store_dma_dshot(motor, true);
		motor_store_started(motor, true);
	} else {
		raise(motor->motor_signal);
	}
	return status;
}

void motor_start_all() {
//	motor_pwm(get_M_L1(), get_start_throttle(get_M_L1()));
//	motor_pwm(get_M_L2(), get_start_throttle(get_M_L2()));
//	motor_pwm(get_M_R1(), get_start_throttle(get_M_R1()));
//	motor_pwm(get_M_R2(), get_start_throttle(get_M_R2()));
	motor_pwm(get_M_EDF_L1(), get_start_throttle(get_M_EDF_L1()));
	motor_pwm(get_M_EDF_R1(), get_start_throttle(get_M_EDF_R1()));
}

const short get_start_throttle(MOTOR_t *motor) {
	if (MOTOR_ONESHOT42 == motor->signal_type) {
		return MOTOR_START_THROTTLE_ONESHOT42;
	} else if (MOTOR_PWM_EDF == motor->signal_type) {
		return MOTOR_START_THROTTLE_EDF;
	} else if (MOTOR_MULTISHOT == motor->signal_type) {
		return MOTOR_START_THROTTLE_MULTISHOT;
	} else if (MOTOR_DSHOT == motor->signal_type) {
		return MOTOR_START_THROTTLE_DSHOT;
	}
	return MOTOR_START_THROTTLE; // MOTOR_PWM_STANDARD
}

void motor_dshot(MOTOR_t *motor, uint16_t value) {
	if (!get_started_motor(motor)) {
		return;
	}
	fill_buffer_dshot(motor, dshot_frame(value));
}

void motor_pwm(MOTOR_t *motor, uint16_t value) {
	if (!get_started_motor(motor)) {
		return;
	}
	switch (motor->pwm_channel) {
	case TIM_CHANNEL_1: {
		motor->pwm_tim->Instance->CCR1 = value;
		break;
	}
	case TIM_CHANNEL_2: {
		motor->pwm_tim->Instance->CCR2 = value;
		break;
	}
	case TIM_CHANNEL_3: {
		motor->pwm_tim->Instance->CCR3 = value;
		break;
	}
	case TIM_CHANNEL_4: {
		motor->pwm_tim->Instance->CCR4 = value;
		break;
	}
	}
}

inline bool get_started_motor(MOTOR_t *motor) {
	return atomic_load_explicit(&motor->started, *get_memory_order());
}

inline bool get_dma_dshot_motor(MOTOR_t *motor) {
	return atomic_load_explicit(&motor->dma_dshot, *get_memory_order());
}

inline HAL_StatusTypeDef motor_deinit(MOTOR_t *motor) {
	if (get_dma_dshot_motor(motor)) {
		HAL_StatusTypeDef status = HAL_TIM_PWM_Stop_DMA(motor->pwm_tim,
				motor->pwm_channel);
		if (HAL_OK == status) {
			free(motor->buffer);
			motor->buffer = NULL;
		}
		return status;
	} else {
		return HAL_TIM_PWM_Stop(motor->pwm_tim, motor->pwm_channel);
	}
}

inline MOTOR_t* get_M_L1() {
	return &M_L1;
}
inline MOTOR_t* get_M_L2() {
	return &M_L2;
}
inline MOTOR_t* get_M_EDF_L1() {
	return &M_EDF_L1;
}
inline MOTOR_t* get_M_R1() {
	return &M_R1;
}
inline MOTOR_t* get_M_R2() {
	return &M_R2;
}
inline MOTOR_t* get_M_EDF_R1() {
	return &M_EDF_R1;
}
