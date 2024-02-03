/*
 * motor.h
 *
 *  Created on: Dec 2, 2023
 *      Author: konrad
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include <signal.h>
#include <stdbool.h>
#include <stdatomic.h>
#include "init.h"
#include "tim.h"

#define DSHOT_SENT_SIZE 18

typedef enum MOTOR_SIGNAL {
	MOTOR_PWM = 0, MOTOR_PWM_EDF, MOTOR_ONESHOT42, MOTOR_MULTISHOT, MOTOR_DSHOT
} MOTOR_SIGNAL_e;

// Oneshot42 42-84us 12kHz
// Multishot 5us-25us
// EDF PWM   1.4-2ms means 2ms delay

typedef struct MOTOR {
	TIM_HandleTypeDef *pwm_tim;
	uint32_t pwm_channel;
	atomic_bool started;
	atomic_bool dma_dshot;
	uint32_t buffer[DSHOT_SENT_SIZE]; //FIXME change to pointer and init when chosen Dshot
	volatile short motor_signal;
	volatile sig_atomic_t motorSignalStatus;
	MOTOR_SIGNAL_e signal_type;
} MOTOR_t;

MOTOR_t* get_M_L1(void);
MOTOR_t* get_M_L2(void);
MOTOR_t* get_M_EDF_L1(void);
MOTOR_t* get_M_R1(void);
MOTOR_t* get_M_R2(void);
MOTOR_t* get_M_EDF_R1(void);

const short get_start_throttle(MOTOR_t *motor);

HAL_StatusTypeDef motor_init_all(void);
HAL_StatusTypeDef motor_init_pwm(MOTOR_t *motor, TIM_HandleTypeDef *tim,
		uint32_t channel, MOTOR_SIGNAL_e type);
HAL_StatusTypeDef motor_init_dma_dshot(MOTOR_t *motor, TIM_HandleTypeDef *tim,
		uint32_t channel, MOTOR_SIGNAL_e type);
void motor_start_all(void);
bool get_started_motor(MOTOR_t *motor);
bool get_dma_dshot_motor(MOTOR_t *motor);
void motor_pwm(MOTOR_t *motor, uint16_t value);
void motor_dshot(MOTOR_t *motor, uint16_t value);
HAL_StatusTypeDef motor_deinit(MOTOR_t *motor);

#endif /* INC_MOTOR_H_ */
