/*
 * timer.c
 *
 *  Created on: Feb 4, 2024
 *      Author: konrad
 */
#include "timer.h"
#include "stm32f4xx.h"

void init_timers_tim4_rtc() {
//TIM4 INIT
	__HAL_RCC_TIM4_CLK_ENABLE();
//	TIM4->PSC = 100 - 1;
	TIM4->CR1 = TIM_CR1_CEN;

//RTC INIT
	RTC->WPR = 0xCA; //from RM0383 reference manual 17.3.5 RTC initialization and configuration
	RTC->WPR = 0x53;
	RTC->ISR |= (1U << 7); //INIT
	RTC->ISR |= (1U << 6); //INITF
	RTC->CR |= (1U << 19); // CSSON Clock security system enable
	RTC->ISR &= ~(1U << 7); //INIT clear
}

inline  uint32_t timer_tim4_us() {
	return TIM4->CNT;
}

inline uint32_t timer_rtc_sec() {
	return RTC->TR;
}

uint32_t timer_tim4_diff_us(uint32_t last_tick) {
	uint32_t now_tick = timer_tim4_us();
	if (now_tick >= last_tick) {
		return now_tick - last_tick;
	} else {
		return now_tick + (TIM4->ARR - last_tick); //0xffff changed to 0x3E8 = 1000 - 1 ms
	}
}

inline uint32_t timer_rtc_diff_sec(uint32_t last_sec) {
	return timer_rtc_sec() - last_sec;
}
