/*
 * init.c
 *
 *  Created on: Dec 23, 2023
 *      Author: konrad
 */
#include <stdbool.h>
#include "init.h"
#include "stm32f4xx.h"

static volatile bool initialized = false;
static const memory_order memory_order_drone = memory_order_seq_cst;

inline const memory_order* get_memory_order() {
	return &memory_order_drone;
}

void pre_init() {
	if (!initialized) {
		atomic_signal_fence(memory_order_drone);
		atomic_thread_fence(memory_order_drone);
		__HAL_RCC_TIM4_CLK_ENABLE();
		TIM4->PSC = HAL_RCC_GetPCLK1Freq()/1000000 - 1;
		TIM4->CR1 = TIM_CR1_CEN;
		initialized = true;
	}
}
