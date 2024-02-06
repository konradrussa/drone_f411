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

extern int16_t gTHR;
extern float ahrs_kp;
extern const float AHRS_KP_BIG;
extern const float AHRS_KP_NORM;

const float sf_lambda = 0.1; //	sliding surface convergence rate

const short MIN_THROTTLE_ONESHOT42 = 4210; //or 5200
const short MIN_THROTTLE_EDF = 1421;
const short MIN_THROTTLE = 51;

static PidVariable_t pidVar = { 0.0, 0.0, 0.0, 0.0, 0.0, &update_pid_fun };
static SmVariable_t smVar = { 0.0, 0.0, 0.0, 0.0, &update_sm_fun };
volatile bool pidInitialized = false;
volatile bool smInitialized = false;

static float constrain(float input, float negative_min, float positive_max) {
	if (input < negative_min)
		return negative_min;
	if (input > positive_max)
		return positive_max;
	return input;
}

static float differentiate(void) {
	return pidVar.new_val_p / pidVar.dt;
}

static float integrate(void) {
	return pidVar.new_val_p * pidVar.dt;
}

static void update_pid_fun(float input) {
	if (pidInitialized) {
		pidVar.new_val_p = pidVar.prev_val - input;
		pidVar.new_val_i += integrate();
		pidVar.new_val_d = differentiate();
	} else {
		pidInitialized = true; //TODO grab dt
	}
	pidVar.prev_val = input;
}

static void update_sm_fun(float input) {
	if (smInitialized) {
		float error = smVar.prev_val - input;
		smVar.sliding_surface += sf_lambda * error * smVar.dt;
		smVar.sliding_surface = sign1(smVar.sliding_surface);
		smVar.sliding_mode = constrain(error, -AHRS_KP_NORM, AHRS_KP_BIG)
				* smVar.sliding_surface * smVar.dt;
	} else {
		smInitialized = true; //TODO grab dt
	}
	smVar.prev_val = input;
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

