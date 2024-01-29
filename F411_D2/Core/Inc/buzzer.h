/*
 * buzzer.h
 *
 *  Created on: Dec 14, 2023
 *      Author: konrad
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include "stm32f4xx.h"

HAL_StatusTypeDef buzzer_init(TIM_HandleTypeDef *tim, uint32_t channel);
HAL_StatusTypeDef buzzer_deinit(void);
void buzzer_start(uint16_t bottom_distance);
void buzzer_stop(void);

#endif /* INC_BUZZER_H_ */
