/*
 * buzzer.c
 *
 *  Created on: Dec 14, 2023
 *      Author: konrad
 */
#include "buzzer.h"

static uint16_t counter = 0;
static uint32_t sound = 50;

TIM_HandleTypeDef *pwm_tim;
uint32_t pwm_channel;

HAL_StatusTypeDef buzzer_init(TIM_HandleTypeDef *tim, uint32_t channel) {
	pwm_tim = tim;
	pwm_channel = channel;
	return HAL_TIM_PWM_Start(pwm_tim, pwm_channel);
}

HAL_StatusTypeDef buzzer_deinit() {
	return HAL_TIM_PWM_Stop(pwm_tim, pwm_channel);
}

static void buzzer_sound(uint16_t period) {
	uint32_t value = 0;
	if (counter % period == 0) {
		counter = 0;
		value = sound;
	}
	switch (pwm_channel) {
	case TIM_CHANNEL_1: {
		pwm_tim->Instance->CCR1 = value;
		break;
	}
	case TIM_CHANNEL_2: {
		pwm_tim->Instance->CCR2 = value;
		break;
	}
	case TIM_CHANNEL_3: {
		pwm_tim->Instance->CCR3 = value;
		break;
	}
	case TIM_CHANNEL_4: {
		pwm_tim->Instance->CCR4 = value;
		break;
	}
	}
}

void buzzer_start(uint16_t bottom_distance) {
	uint16_t period = bottom_distance / sound * 5;
	period = period == 0 ? 10 : period;
	buzzer_sound(period);
	counter++;
}

void buzzer_stop() {
	counter = 1;
	buzzer_sound(2);
}
