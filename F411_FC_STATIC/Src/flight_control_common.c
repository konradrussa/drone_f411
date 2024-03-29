/*
 * flight_control_common.c
 *
 *  Created on: Feb 5, 2024
 *      Author: konrad
 */
#include "env.h"
#include "drone.h"
#include "timer.h"
#include "matrix.h"
#include "ahrs_common.h"
#include "flight_control_common.h"

// forward declaration
static void update_pid_fun(float setpoint, float input);
static void update_sm_fun(float setpoint, float input);
static void update_mp_fun(struct MpControl *control);

extern int16_t gTHR;
extern float ahrs_kp;
extern const float AHRS_KP_BIG;
extern const float AHRS_KP_NORM;

const float drag_coefficience = 0.1; // calculate model drag force 1/2 * ro * A * Cd * v * uv

const float pid_kp = 0.0003;
const float pid_ki = 0.002;
const float pid_kd = 0.001;

const float sm_kp_norm = 4.2;
const float sm_kp_big = 107.0;
const float sm_surf_lambda = 0.1; //	sliding surface convergence rate

const float mp_horizon = 5;

const short MIN_THROTTLE_ONESHOT42 = 4210; //or 5200
const short MIN_THROTTLE_EDF = 1421;

static PidVariable_t pidVar = { 0.0, 0.0, 0.0, 0.0, 0.0, &update_pid_fun };
static SmVariable_t smVar = { 0.0, 0.0, 0.0, 0.0, &update_sm_fun };

struct MpControl control = { 0.0, 0.0, 0.0, { 0.0, 0.0, 0.0 } };
static MpVariable_t mpVar = { { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0,
		0.0, 0.0 } }, 0.0, &control, &update_mp_fun };

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

static void acceleration_kinematics(float acceleration_z, float acceleration_y,
		float acceleration_x) {
	mpVar.state.acc.x += acceleration_x;
	mpVar.state.acc.y += acceleration_y;
	mpVar.state.acc.z += acceleration_z;
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
	float base_drag_force = 1.0 / 2.0 * get_ro() * get_drone_front_area()
			* drag_coefficience; // * v^2 * uv

	Matrix3D_t *rot_mat = matrix_get_rotation_matrix(control->angles.roll_x,
			control->angles.pitch_y, control->angles.yaw_z);

	Vector3D_t thrusts = { control->thrust_cruise, 0.0, control->thrust_vtol };

	Vector3D_t forces;
	matrix_vector_product(rot_mat, &thrusts, &forces);

	mpVar.control->thrust = math_vec_mag(&forces); // + EDF force + VTOL force + Cruise model force

	float drag_force = 0.0;
	float acceleration_x = 0.0;
	float acceleration_y = 0.0;
	float acceleration_z = 0.0;
	for (int i = 0; i < mp_horizon; i++) {
		float unit_vec_denominator = math_vec_mag(&mpVar.state.vel);
		if (mpVar.state.vel.x != 0.0 && unit_vec_denominator != 0.0) {
			drag_force = base_drag_force * mpVar.state.vel.x * mpVar.state.vel.x
					* (mpVar.state.vel.x / unit_vec_denominator);
		}
		acceleration_z = (forces.z - drag_force - gravity_force)
				/ get_drone_whole_mass();
		acceleration_y = (forces.y - drag_force) / get_drone_whole_mass();
		acceleration_x = (forces.x - drag_force) / get_drone_whole_mass();
		acceleration_kinematics(acceleration_z, acceleration_y, acceleration_x);

		if (i >= 1) {
			velocity_position_kinematics();
		}
	}
}

inline PidVariable_t* flight_get_pid_var() {
	return &pidVar;
}

inline SmVariable_t* flight_get_sm_var() {
	return &smVar;
}

inline MpVariable_t* flight_get_mp_var() {
	return &mpVar;
}

inline struct MpControl* flight_get_mp_control() {
	return &control;
}

inline void flight_set_parameters() {
	if (gTHR < MIN_THROTTLE_ONESHOT42) {
		ahrs_kp = AHRS_KP_BIG;
	} else {
		ahrs_kp = AHRS_KP_NORM;
	}
}

