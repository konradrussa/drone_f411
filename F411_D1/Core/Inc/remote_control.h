/*
 * remote_control.h
 *
 *  Created on: Dec 23, 2023
 *      Author: konrad
 */

#ifndef INC_REMOTE_CONTROL_H_
#define INC_REMOTE_CONTROL_H_

#include "tim.h"
#include "gpio.h"
#include "drone.h"

typedef struct RemoteControl {
	TIM_TypeDef *rc_timer_instance;
	uint32_t rc_channel;
	GPIO_TypeDef *rc_channel_port;
	uint16_t rc_channel_pin;
	bool captured;
	uint32_t rise;
	uint32_t fall;
	uint32_t difference;
	uint32_t frequency;
	uint32_t width_us;
	uint32_t frame_width_us;
} RemoteControl_t;

HAL_StatusTypeDef rc_init_all(void);
void rc_throttle_gear_speed_callback(TIM_HandleTypeDef *htim);
void rc_pitch_callback(TIM_HandleTypeDef *htim);
void rc_roll_callback(TIM_HandleTypeDef *htim);
void rc_yaw_callback(TIM_HandleTypeDef *htim);
void rc_calculate(uint32_t capturedValue, RemoteControl_t *control,
		TIM_HandleTypeDef *htim, Queue_t *queue);

#endif /* INC_REMOTE_CONTROL_H_ */
