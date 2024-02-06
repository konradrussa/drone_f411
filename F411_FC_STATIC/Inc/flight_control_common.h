/*
 * flight_control_common.h
 *
 *  Created on: Feb 5, 2024
 *      Author: konrad
 */

#ifndef FLIGHT_CONTROL_COMMON_H_
#define FLIGHT_CONTROL_COMMON_H_

#include <stdint.h>

typedef void (*update_pid_fptr)(float input);
typedef void (*update_sm_fptr)(float input);

typedef struct PidVariable {
	float prev_val;
	float new_val_p;
	float new_val_i;
	float new_val_d;
	float dt;
	update_pid_fptr update_pid;
} PidVariable_t;

typedef struct SmVariable {
	float sliding_mode;
	float sliding_surface;
	float prev_val;
	float dt;
	update_sm_fptr update_sm;
} SmVariable_t;

void flight_set_parameters(void);

PidVariable_t* flight_get_pid_var(void);
static void update_pid_fun(float input);

SmVariable_t* flight_get_sm_var();
static void update_sm_fun(float input);

#endif /* FLIGHT_CONTROL_COMMON_H_ */
