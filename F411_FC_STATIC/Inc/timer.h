/*
 * timer.h
 *
 *  Created on: Feb 4, 2024
 *      Author: konrad
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

void init_timers_tim4_rtc(void);
uint32_t timer_tim4_us(void);
uint32_t timer_rtc_sec(void);
uint32_t timer_tim4_diff_us(uint32_t last_tick);
uint32_t timer_rtc_diff_sec(uint32_t last_sec);

#endif /* TIMER_H_ */
