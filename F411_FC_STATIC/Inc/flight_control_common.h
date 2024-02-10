/*
 * flight_control_common.h
 *
 *  Created on: Feb 5, 2024
 *      Author: konrad
 */

#ifndef FLIGHT_CONTROL_COMMON_H_
#define FLIGHT_CONTROL_COMMON_H_

#include <stdint.h>
#include "basic_math.h"

typedef void (*update_pid_fptr)(float setpoint, float input);
typedef void (*update_sm_fptr)(float setpoint, float input);

typedef struct PidVariable {
	float prev_error;
	float error_p;
	float error_i;
	float error_d;
	float dt;
	update_pid_fptr update_pid;
} PidVariable_t;

typedef struct SmVariable {
	float error_p;
	float sliding_surface;
	float prev_error;
	float dt;
	update_sm_fptr update_sm;
} SmVariable_t;

typedef struct MpVariable {
	struct MpState {
		Vector3D_t pos;
		Vector3D_t vel;
		Vector3D_t acc;
	} state;
	struct MpControl {
		float thrust; // cumulative thrust
		float thrust_vtol; // VTOL + permanent vtol rotors
		float thrust_cruise; // Cruise + EDF or any other combinantion
		float roll;
		float pitch;
		float yaw;
	} control;
	float dt;
} MpVariable_t;

void flight_set_parameters(void);

PidVariable_t* flight_get_pid_var(void);
SmVariable_t* flight_get_sm_var(void);
MpVariable_t* flight_calculate_mp_var(void);


#endif /* FLIGHT_CONTROL_COMMON_H_ */
