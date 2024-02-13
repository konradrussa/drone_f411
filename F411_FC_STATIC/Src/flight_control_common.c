/*
 * flight_control_common.c
 *
 *  Created on: Feb 5, 2024
 *      Author: konrad
 */
#include <stdbool.h>
#include "env.h"
#include "drone.h"
#include "timer.h"
#include "flight_control_common.h"

// forward declaration
static void update_pid_fun(float setpoint, float input);
static void update_sm_fun(float setpoint, float input);
static void update_mp_fun(struct MpControl *control);

extern int16_t gTHR;
extern float ahrs_kp;
extern const float AHRS_KP_BIG;
extern const float AHRS_KP_NORM;
const float drag_coefficience = 0.1; // TODO calculate model drag force 1/2 * ro * A * Cd * v * uv

const float pid_kp = 0.0003;
const float pid_ki = 0.002;
const float pid_kd = 0.001;

const float sm_kp_norm = 4.2;
const float sm_kp_big = 107.0;

const float sm_surf_lambda = 0.1; //	sliding surface convergence rate

const float mp_horizon = 5;

const short MIN_THROTTLE_ONESHOT42 = 4210; //or 5200
const short MIN_THROTTLE_EDF = 1421;
const short MIN_THROTTLE = 51;

static PidVariable_t pidVar = { 0.0, 0.0, 0.0, 0.0, 0.0, &update_pid_fun };
static SmVariable_t smVar = { 0.0, 0.0, 0.0, 0.0, &update_sm_fun };

struct MpControl control = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
static MpVariable_t mpVar = { { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0,
		0.0, 0.0 } }, 0.0, &control, &update_mp_fun };

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
	smVar.prev_error = error; // TODO add differentiation and integration
}

static float lateral_dynamics(float roll, float upward_thrust) {
	return upward_thrust * sinf(roll);
}

static float longitudinal_dynamics(float pitch, float forward_thrust,
		float upward_thrust) {
	float resultant_force = math_sqrt(
			forward_thrust * forward_thrust + upward_thrust * upward_thrust);
	float upward_force = resultant_force;
	if (resultant_force != 0.0 && upward_thrust != 0.0) {
		float tau = asinf(upward_thrust / resultant_force);
		upward_force = resultant_force * sinf(tau - pitch);
	}
	return upward_force;
}

//TODO incorporate acceleration from cruise mode
static void acceleration_kinematics(float acceleration) {
	mpVar.state.acc.x +=
			(mpVar.control->pitch == 0.0) ?
					0.0 : acceleration / cosf(mpVar.control->pitch);
	mpVar.state.acc.y +=
			(mpVar.control->roll == 0.0) ?
					0.0 : acceleration / cosf(mpVar.control->roll);
	mpVar.state.acc.z += acceleration;
}

static void velocity_position_kinematics() {
	mpVar.state.vel.x += mpVar.state.acc.x * mpVar.dt;
	mpVar.state.vel.y += mpVar.state.acc.y * mpVar.dt;
	mpVar.state.vel.z += mpVar.state.acc.z * mpVar.dt;

	mpVar.state.pos.x += mpVar.state.vel.x * mpVar.dt;
	mpVar.state.pos.y += mpVar.state.vel.y * mpVar.dt;
	mpVar.state.pos.z += mpVar.state.vel.z * mpVar.dt;
}

static void update_mp_fun(struct MpControl *control) {
	mpVar.control = control;
	float gravity_force = get_drone_whole_mass() * get_geo_g();
	float base_drag_force = 1.0 / 2.0 * get_ro() * get_front_area()
			* drag_coefficience; // * v^2 * uv

	float net_vertical_force = longitudinal_dynamics(mpVar.control->pitch,
			mpVar.control->thrust_cruise, mpVar.control->thrust_vtol)
			+ lateral_dynamics(mpVar.control->roll, mpVar.control->thrust_vtol);

	float net_horizontal_force = mpVar.control->thrust_cruise
			* cosf(mpVar.control->pitch);
	mpVar.control->thrust = math_sqrt(
			net_vertical_force * net_vertical_force
					+ net_horizontal_force * net_horizontal_force); // + EDF force + VTOL force + Cruise model force
	float drag_force = 0.0;
	float acceleration = 0.0;
	for (int i = 0; i < mp_horizon; i++) {
		float unit_vec_denominator = math_vec_mag(&mpVar.state.vel);
		if (mpVar.state.vel.x != 0.0 && unit_vec_denominator != 0.0) {
			drag_force = base_drag_force * mpVar.state.vel.x * mpVar.state.vel.x
					* (mpVar.state.vel.x / unit_vec_denominator);
		}
		acceleration = (mpVar.control->thrust - drag_force - gravity_force)
				/ get_drone_whole_mass();

		acceleration_kinematics(acceleration);

		if (i >= 1) {
			velocity_position_kinematics();
		}
	}
}

PidVariable_t* flight_get_pid_var() {
	return &pidVar;
}

SmVariable_t* flight_get_sm_var() {
	return &smVar;
}

MpVariable_t* flight_get_mp_var() {
	return &mpVar;
}

struct MpControl* flight_get_mp_control() {
	return &control;
}

void flight_set_parameters() {
	if (gTHR < MIN_THROTTLE_ONESHOT42) {
		ahrs_kp = AHRS_KP_BIG;
	} else {
		ahrs_kp = AHRS_KP_NORM;
	}
}

