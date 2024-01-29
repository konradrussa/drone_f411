/*
 * remote_control.c
 *
 *  Created on: Dec 23, 2023
 *      Author: konrad
 */
#include "remote_control.h"
//rc constraints
const uint32_t RC_VALUE_MAX = 2000;
const uint32_t RC_VALUE_MID = 1500;
const uint32_t RC_VALUE_MIN = 1000;
const uint32_t RC_FRAME_MAX = 24000;
const uint32_t RC_FRAME_MIN = 21000;
//timer constraints
const uint32_t RC_TIMCLOCK = 100000000;
const uint32_t RC_PRESCALER = 100;
const uint32_t TIM_FACTOR_US = RC_TIMCLOCK / RC_PRESCALER / 1000000;

static RemoteControl_t controls_rc[6];

static void rc_control_defaults(RemoteControl_t *control, TIM_TypeDef *timer,
		uint32_t channel, GPIO_TypeDef *port, uint16_t pin) {
	control->rc_timer_instance = timer;
	control->rc_channel = channel;
	control->rc_channel_port = port;
	control->rc_channel_pin = pin;
	control->captured = false;
	control->frame_width_us = 0;
	control->rise = 0;
	control->fall = 0;
}

//65535 16 bit 4 channels 4 values one by one
void rc_calculate(uint32_t capturedValue, RemoteControl_t *control,
		TIM_HandleTypeDef *htim, Queue_t *queue) {
	bool rising_edge = HAL_GPIO_ReadPin(control->rc_channel_port,
			control->rc_channel_pin) == GPIO_PIN_SET;
	if (!control->captured && rising_edge) { // is the first value captured ?
		control->rise = capturedValue; // capture the first value
		control->captured = true; // set the first value captured as true
		if (control->fall != 0) {
			control->frame_width_us = capturedValue + control->difference;
			if (control->frame_width_us > RC_FRAME_MIN
					&& control->frame_width_us < RC_FRAME_MAX) {
				queue_enqueue(queue, control->width_us);
			}
			control->fall = 0; 				// reset fall
			control->frame_width_us = 0;	// reset frame width
		}
	} else if (control->captured && !rising_edge) { // if the first is captured
		uint32_t diff = 0;
		control->fall = capturedValue; // capture the second value
		if (control->fall > control->rise) {
			diff = control->fall - control->rise; // calculate the difference
		} else if (control->fall < control->rise) {
			diff = control->fall + (0xFFFF - control->rise); //0xffff
		}
		if (diff > RC_VALUE_MIN && diff < RC_VALUE_MAX) {
			control->width_us = diff * TIM_FACTOR_US; // calculate pulse width
			control->frequency = RC_TIMCLOCK / diff; 	// calculate frequency
			control->difference = diff;
		}
		control->captured = false; // reset to first captured
		__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
	}
}

void rc_throttle_gear_speed_callback(TIM_HandleTypeDef *htim) {
	switch (htim->Channel) {
	case HAL_TIM_ACTIVE_CHANNEL_1: {
		rc_calculate(htim->Instance->CCR1, &controls_rc[0], htim,
				get_throttle_queue_rc());
		break;
	}
	case HAL_TIM_ACTIVE_CHANNEL_3: {
		rc_calculate(htim->Instance->CCR3, &controls_rc[4], htim,
				get_gear_queue_rc());
		break;
	}
	case HAL_TIM_ACTIVE_CHANNEL_4: {
		rc_calculate(htim->Instance->CCR4, &controls_rc[5], htim,
				get_speed_queue_rc());
		break;
	}
	default: {
	}
	}
}

void rc_pitch_callback(TIM_HandleTypeDef *htim) {
	rc_calculate(htim->Instance->CCR1, &controls_rc[1], htim,
			get_pitch_queue_rc());
}

void rc_roll_callback(TIM_HandleTypeDef *htim) {
	rc_calculate(htim->Instance->CCR1, &controls_rc[2], htim,
			get_roll_queue_rc());
}

void rc_yaw_callback(TIM_HandleTypeDef *htim) {
	rc_calculate(htim->Instance->CCR1, &controls_rc[3], htim,
			get_yaw_queue_rc());
}

HAL_StatusTypeDef rc_init_all() {
	rc_control_defaults(&controls_rc[0], TIM2, TIM_CHANNEL_1,
	THROTTLE_GPIO_Port, THROTTLE_Pin);
	rc_control_defaults(&controls_rc[1], TIM9, TIM_CHANNEL_1, PITCH_GPIO_Port,
	PITCH_Pin);
	rc_control_defaults(&controls_rc[2], TIM10, TIM_CHANNEL_1, ROLL_GPIO_Port,
	ROLL_Pin);
	rc_control_defaults(&controls_rc[3], TIM11, TIM_CHANNEL_1, YAW_GPIO_Port,
	YAW_Pin);
	rc_control_defaults(&controls_rc[4], TIM2, TIM_CHANNEL_3,
	GEAR_GPIO_Port, GEAR_Pin);
	rc_control_defaults(&controls_rc[5], TIM2, TIM_CHANNEL_4,
	SPEED_GPIO_Port, SPEED_Pin);

	HAL_StatusTypeDef status = HAL_OK;
	status = HAL_TIM_RegisterCallback(&htim2, HAL_TIM_IC_CAPTURE_CB_ID,
			rc_throttle_gear_speed_callback);
	if (HAL_OK != status) {
		return status;
	}
	status = HAL_TIM_RegisterCallback(&htim9, HAL_TIM_IC_CAPTURE_CB_ID,
			rc_pitch_callback);
	if (HAL_OK != status) {
		return status;
	}
	status = HAL_TIM_RegisterCallback(&htim10, HAL_TIM_IC_CAPTURE_CB_ID,
			rc_roll_callback);
	if (HAL_OK != status) {
		return status;
	}
	status = HAL_TIM_RegisterCallback(&htim11, HAL_TIM_IC_CAPTURE_CB_ID,
			rc_yaw_callback);
	if (HAL_OK != status) {
		return status;
	}

	status = HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1); // throttle
	if (HAL_OK != status) {
		return status;
	}
	status = HAL_TIM_IC_Start_IT(&htim9, TIM_CHANNEL_1); // pitch
	if (HAL_OK != status) {
		return status;
	}
	status = HAL_TIM_IC_Start_IT(&htim10, TIM_CHANNEL_1); // roll
	if (HAL_OK != status) {
		return status;
	}
	status = HAL_TIM_IC_Start_IT(&htim11, TIM_CHANNEL_1); // yaw
	if (HAL_OK != status) {
		return status;
	}
	status = HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3); // gear
	if (HAL_OK != status) {
		return status;
	}
	status = HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_4); // speed
	if (HAL_OK != status) {
		return status;
	}
	return status;
}
