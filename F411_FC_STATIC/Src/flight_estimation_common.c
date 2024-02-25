/*
 * flight_estimation_common.c
 *
 *  Created on: Feb 21, 2024
 *      Author: konrad
 */

#include "assert.h"
#include "env.h"
#include "drone.h"
#include "timer.h"
#include "math.h"
#include "matrix.h"
#include "ahrs_common.h"
#include "flight_estimation_common.h"

static UKF_t ukf_filter;

static void estimation_ukf_predict() {
	ukf_filter.state.angular_vel.x += ukf_filter.state.angular_acc.x
			* ukf_filter.dt;
	ukf_filter.state.angular_vel.y += ukf_filter.state.angular_acc.y
			* ukf_filter.dt;
	ukf_filter.state.angular_vel.z += ukf_filter.state.angular_acc.z
			* ukf_filter.dt;

	ukf_filter.state.attitude.x += ukf_filter.state.angular_vel.x
			* ukf_filter.dt;
	ukf_filter.state.attitude.y += ukf_filter.state.angular_vel.y
			* ukf_filter.dt;
	ukf_filter.state.attitude.z += ukf_filter.state.angular_vel.z
			* ukf_filter.dt;

	ukf_filter.state.vel.x += ukf_filter.state.acc.x * ukf_filter.dt;
	ukf_filter.state.vel.y += ukf_filter.state.acc.y * ukf_filter.dt;
	ukf_filter.state.vel.z += ukf_filter.state.acc.z * ukf_filter.dt;

	ukf_filter.state.pos.x += ukf_filter.state.vel.x * ukf_filter.dt;
	ukf_filter.state.pos.y += ukf_filter.state.vel.y * ukf_filter.dt;
	ukf_filter.state.pos.z += ukf_filter.state.vel.z * ukf_filter.dt;
}

// AxesRaw_t[2] accel, AxesRaw_t[2] gyro, AxesRaw_t[2] magnet
static void estimation_ukf_update(const AxesRaw_t *accel, const AxesRaw_t *gyro,
		const AxesRaw_t *magnet) {
	//accelx =  1g * sin(theta) * cos(phi)
	//accely = -1g * sin(theta) * sin(phi)
	//accelz =  1g * cos(theta)
	//accely/accelx = -tan(phi)

	ukf_filter.state.acc.x = (float) accel->AXIS_X;
	ukf_filter.state.acc.y = (float) accel->AXIS_Y;
	ukf_filter.state.acc.z = (float) accel->AXIS_Z;

	float acc_magnitude = math_vec_mag(&ukf_filter.state.acc);
	ukf_filter.state.acc.x /= acc_magnitude;
	ukf_filter.state.acc.y /= acc_magnitude;
	ukf_filter.state.acc.z /= acc_magnitude;

	float pitch = asinf(-ukf_filter.state.acc.x);
	float roll = atan2f(ukf_filter.state.acc.y, ukf_filter.state.acc.z);

	Vector3D_t mag = { (float) magnet->AXIS_X, (float) magnet->AXIS_Y,
			(float) magnet->AXIS_Z };
	float magnet_magnitude = math_vec_mag(&mag);
	mag.x /= magnet_magnitude;
	mag.y /= magnet_magnitude;
	mag.z /= magnet_magnitude;

	float mx = mag.x * cosf(pitch) + mag.z * sinf(pitch);
	float my = mag.x * sinf(roll) * sinf(pitch) + mag.y * cosf(roll)
			- mag.z * sinf(roll) * cosf(pitch);

	float yaw = atan2f(my, mx);
	// normalize yaw to -pi .. pi
	if (yaw > M_PI)
		yaw -= 2.f * M_PI;
	if (yaw < -M_PI)
		yaw += 2.f * M_PI;

	ukf_filter.state.angular_vel.x = (float) gyro->AXIS_X;
	ukf_filter.state.angular_vel.y = (float) gyro->AXIS_Y;
	ukf_filter.state.angular_vel.z = (float) gyro->AXIS_Z;

	float phi = -atan2f((float) accel->AXIS_Y, (float) accel->AXIS_X);
	float theta = acosf((float) (accel->AXIS_Z - 2025) / get_geo_g());
	ukf_filter.state.attitude.x = phi * 180 / MAX_RAD;
	//ukf_filter.state.attitude.y = psi * 180 / MAX_RAD;
	ukf_filter.state.attitude.z = theta * 180 / MAX_RAD;

//	float check_x = get_geo_g() + sinf(theta) * cosf(phi);
//	//assert(accel->x == check_x);
//	float check_y = get_geo_g() + sinf(theta) * sinf(phi);
//	//assert(accel->y == check_y);
//	float check_z = get_geo_g() + cosf(theta);
//	//assert(accel->z == check_z);
}

void estimation_ukf(const AxesRaw_t *accel, const AxesRaw_t *gyro,
		const AxesRaw_t *magnet) {
	estimation_ukf_predict();
	estimation_ukf_update(accel, gyro, magnet);
}

// BLUE - best linear unbiased estimate
void estimation_blue(const float time[3], const Vector3D_t observations) {

	Matrix3D_t blue_model;
	Matrix3D_t blue_covariance;

	for (int i = 0; i < 3; i++) {

		blue_model.matrix[i][0] = 1.0;
		blue_model.matrix[i][1] = time[i];
		blue_model.matrix[i][2] = 0.5 * time[i] * time[i];

		for (int j = 0; j < 3; j++) {
			if (i == j) {
				blue_covariance.matrix[i][j] = time[i];
			} else {
				blue_covariance.matrix[i][j] = 0.0;
			}
		}
	}

	Matrix3D_t cov_inv = matrix_inverse(&blue_covariance);
	Matrix3D_t cov_model_prod = matrix_multiply_matrix(&cov_inv, &blue_model);
	Matrix3D_t model_trans = blue_model;
	matrix_transpose(&model_trans);
	Matrix3D_t prod1 = matrix_multiply_matrix(&model_trans, &cov_model_prod);
	Matrix3D_t prod2 = matrix_multiply_matrix(&prod1, &model_trans);
	Matrix3D_t prod3 = matrix_multiply_matrix(&prod2, &cov_inv);
	Vector3D_t xhat;
	matrix_vector_product(&prod3, &observations, &xhat);
	Vector3D_t result;
	matrix_vector_product(&blue_model, &xhat, &result);
}

UKF_t* get_ukf_filter() {
	return &ukf_filter;
}
