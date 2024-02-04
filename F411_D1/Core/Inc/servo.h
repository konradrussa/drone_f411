/*
 * servo.h
 *
 *  Created on: Nov 8, 2023
 *      Author: konrad
 */

#ifndef INC_SERVO_H_
#define INC_SERVO_H_

#include <signal.h>
#include <stdbool.h>
#include <stdatomic.h>
#include "init.h"
#include "stm32f4xx.h"

typedef struct SERVO {
	uint32_t pwm_channel;
	uint16_t buffer[92];
	atomic_bool cruise; //no volatile
	atomic_bool dma_mode;
	volatile sig_atomic_t servoSignalStatus;
	TIM_HandleTypeDef *pwm_tim;
} SERVO_t;

SERVO_t* get_servo(void);

HAL_StatusTypeDef servo_init_vtol(TIM_HandleTypeDef *tim, uint32_t channel);
HAL_StatusTypeDef servo_deinit(void);

HAL_StatusTypeDef servo_vtol_vehicle(void);
HAL_StatusTypeDef servo_cruise_vehicle(void);

bool servo_get_cruise(void);
bool servo_get_dma(void);

#endif /* INC_SERVO_H_ */
