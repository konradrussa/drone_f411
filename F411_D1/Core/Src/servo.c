/*
 * servo.h
 *
 *  Created on: Nov 8, 2023
 *      Author: konrad
 */
#include "servo.h"

//servo signal
const short SERVO_SIGINT = 99;
//servo constraints
const short SERVO_MAX_US = 125;
const short SERVO_MIN_US = 25;
const short SERVO_VTOL = 75;
const short SERVO_VTOL_ANGLE = 90;
const short SERVO_CRUISE = 125;
const short SERVO_CRUISE_ANGLE = 180;
const short SERVO_MAX_ANGLE = 180;
const short SERVO_MIN_ANGLE = 0;

static SERVO_t servo;
const int array_counter = sizeof(servo.buffer) / sizeof(servo.buffer[0]) - 1;

static void servo_signal_handler(int status) {
	servo.servoSignalStatus = status;
	//start buzzer
}

static void servo_swap() {
	for (int n = 0; n < array_counter / 2; n++) {
		uint16_t top = servo.buffer[n];
		servo.buffer[n] = servo.buffer[array_counter - n];
		servo.buffer[array_counter - n] = top;
	}
}

inline static void servo_store_dma(bool value) {
	atomic_store_explicit(&servo.dma_mode, value, *get_memory_order());
}

inline static void servo_store_cruise(bool value) {
	atomic_store_explicit(&servo.cruise, value, *get_memory_order());
}

static uint8_t servo_calculate_angle(uint8_t angle) {
	if (angle < SERVO_MIN_ANGLE) {
		angle = SERVO_MIN_ANGLE;
	} else if (angle > SERVO_MAX_ANGLE) {
		angle = SERVO_MAX_ANGLE;
	}
	return SERVO_MIN_US
			+ (angle * (SERVO_MAX_US - SERVO_MIN_US)) / SERVO_MAX_ANGLE;
}

//static void servo_set_angle(uint8_t angle) {
//	servo.pwm_tim->Instance->CCR1 = servo_calculate_angle(angle);
//}

static void servo_lock_cruise_vehicle() {
	//servo_set_angle(SERVO_CRUISE_ANGLE);
	servo.pwm_tim->Instance->CCR1 = SERVO_CRUISE;
	servo_store_cruise(true);
}

static void servo_lock_vtol_vehicle() {
	//servo_set_angle(SERVO_VTOL_ANGLE);
	servo.pwm_tim->Instance->CCR1 = SERVO_VTOL;
	servo_store_cruise(false);
}

static void servo_dma_error_callback(DMA_HandleTypeDef *hdma) {
	raise(SERVO_SIGINT);
}

static void servo_dma_finishing_callback(DMA_HandleTypeDef *hdma) {
	if (servo_get_dma()) {
		if (HAL_OK != HAL_TIM_PWM_Stop_DMA(servo.pwm_tim, servo.pwm_channel)) {
			raise(SERVO_SIGINT);
		} //STOP PWM DMA
		servo_store_dma(false);
		if (HAL_OK != HAL_TIM_PWM_Start(servo.pwm_tim, servo.pwm_channel)) {
			raise(SERVO_SIGINT);
		} //START PWM STANDARD
	}
	if (!servo_get_cruise()) { 			// from VTOL to Cruise
		servo_lock_cruise_vehicle();
	} else { 						// from Cruise to VTOL
		servo_lock_vtol_vehicle();
	}
}

static HAL_StatusTypeDef servo_start_dma() {
	if (HAL_OK != HAL_TIM_PWM_Stop(servo.pwm_tim, servo.pwm_channel)) { // pwm stop
		return HAL_ERROR;
	}
	if (HAL_OK
			!= HAL_TIM_PWM_Start_DMA(servo.pwm_tim, servo.pwm_channel,
					(uint32_t*) servo.buffer, sizeof(servo.buffer))) { // start circular
		return HAL_ERROR;
	}
	servo_store_dma(true);
	servo.pwm_tim->hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback =
			servo_dma_finishing_callback;
	servo.pwm_tim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback =
			servo_dma_error_callback;
	return HAL_OK;
}

inline bool servo_get_dma() {
	return atomic_load_explicit(&servo.dma_mode, *get_memory_order());
}

inline bool servo_get_cruise() {
	return atomic_load_explicit(&servo.cruise, *get_memory_order());
}

//MANUAL set
HAL_StatusTypeDef servo_init_vtol(TIM_HandleTypeDef *tim, uint32_t channel) {
	servo.pwm_tim = tim;
	servo.pwm_channel = channel;
	servo_store_dma(false);
	servo_store_cruise(false);
	signal(SERVO_SIGINT, &servo_signal_handler);
	servo_lock_vtol_vehicle(); //start middle vtol lock
	return HAL_TIM_PWM_Start(tim, channel);
}

HAL_StatusTypeDef servo_deinit() {
	return HAL_TIM_PWM_Stop(servo.pwm_tim, servo.pwm_channel);
}

HAL_StatusTypeDef servo_cruise_vehicle() {
//	for (int n = array_counter; n >= 0; n--) {
//		servo.buffer[array_counter - n] = calculate_angle(90 + n);
//	}
	for (int n = 0; n <= array_counter; n++) {
		servo.buffer[n] = servo_calculate_angle(90 + n);
	}
	if (!servo_get_dma()) {
		return servo_start_dma();
	}
	return HAL_ERROR;
}

HAL_StatusTypeDef servo_vtol_vehicle() {
	servo_swap();
	if (!servo_get_dma()) {
		return servo_start_dma();
	}
	return HAL_ERROR;
}

inline SERVO_t* get_servo() {
	return &servo;
}
