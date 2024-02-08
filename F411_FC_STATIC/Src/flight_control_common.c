/*
 * flight_control_common.c
 *
 *  Created on: Feb 5, 2024
 *      Author: konrad
 */
#include <stdbool.h>
#include "timer.h"
#include "basic_math.h"
#include "flight_control_common.h"


// forward declaration
static void update_pid_fun(float setpoint, float input);
static void update_sm_fun(float setpoint, float input);

extern int16_t gTHR;
extern float ahrs_kp;
extern const float AHRS_KP_BIG;
extern const float AHRS_KP_NORM;

const float pid_kp = 0.0003;
const float pid_ki = 0.002;
const float pid_kd = 0.001;

const float sm_kp_norm = 4.2;
const float sm_kp_big = 107.0;

const float sm_surf_lambda = 0.1; //	sliding surface convergence rate

const short MIN_THROTTLE_ONESHOT42 = 4210; //or 5200
const short MIN_THROTTLE_EDF = 1421;
const short MIN_THROTTLE = 51;

static PidVariable_t pidVar = { 0.0, 0.0, 0.0, 0.0, 0.0, &update_pid_fun };
static SmVariable_t smVar = { 0.0, 0.0, 0.0, 0.0, &update_sm_fun };

static float constrain(float input, float negative_min, float positive_max) {
	if (input < negative_min)
		return negative_min;
	if (input > positive_max)
		return positive_max;
	return input;
}

static float differentiate(void) {
	return (pidVar.error_p - pidVar.prev_error); // / pidVar.dt;
}

static float integrate(void) {
	return pidVar.error_p; // * pidVar.dt;
}

static void update_pid_fun(float setpoint, float input) {
	pidVar.error_p = setpoint - input;
	pidVar.error_i += integrate();
	pidVar.error_d = differentiate();
	pidVar.prev_error = pidVar.error_p;
}

static void update_sm_fun(float setpoint, float input) {
	float error = setpoint - input;
	smVar.sliding_surface += sm_surf_lambda * error; // * smVar.dt;
	smVar.sliding_surface = sign0(smVar.sliding_surface);
	smVar.error_p = error * smVar.sliding_surface; // * smVar.dt; //constrain(error, sm_kp_norm, sm_kp_big)
	smVar.prev_error = error;

}

PidVariable_t* flight_get_pid_var() {
	return &pidVar;
}

SmVariable_t* flight_get_sm_var() {
	return &smVar;
}

void flight_set_parameters() {
	if (gTHR < MIN_THROTTLE_ONESHOT42) {
		ahrs_kp = AHRS_KP_BIG;
	} else {
		ahrs_kp = AHRS_KP_NORM;
	}
}

