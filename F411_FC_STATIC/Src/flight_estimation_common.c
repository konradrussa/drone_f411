/*
 * flight_estimation_common.c
 *
 *  Created on: Feb 21, 2024
 *      Author: konrad
 */

#include "flight_estimation_common.h"
#include "assert.h"
#include "env.h"
#include "drone.h"
#include "timer.h"
#include "math.h"
#include "matrix.h"
#include "ahrs_common.h"

static UKF_t ukf_filter;

static void estimation_ukf_predict() {

	ukf_filter.state.gyro_angle_rates.gx += ukf_filter.state.angular_vel.x
			* ukf_filter.dt;
	ukf_filter.state.gyro_angle_rates.gy += ukf_filter.state.angular_vel.y
			* ukf_filter.dt;
	ukf_filter.state.gyro_angle_rates.gz += ukf_filter.state.angular_vel.z
			* ukf_filter.dt;

	ukf_filter.state.gyro_angles.gx += ukf_filter.state.gyro_angle_rates.gx
			* ukf_filter.dt;
	ukf_filter.state.gyro_angles.gy += ukf_filter.state.gyro_angle_rates.gy
			* ukf_filter.dt;
	ukf_filter.state.gyro_angles.gz += ukf_filter.state.gyro_angle_rates.gz
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
	ukf_filter.state.acc.z = -(float) accel->AXIS_Z;

	float acc_magnitude = math_vec_mag(&ukf_filter.state.acc);
	ukf_filter.state.acc.x /= acc_magnitude;
	ukf_filter.state.acc.y /= acc_magnitude;
	ukf_filter.state.acc.z /= acc_magnitude;

	float phi = 2.0
			* atan2f(asinf(ukf_filter.state.acc.y),
					acosf(ukf_filter.state.acc.x)); // 2* 90 , 1* 45
	ukf_filter.state.attitude.z = phi * rad_to_deg();

	float theta = 2.0
			* atan2f(asinf(ukf_filter.state.acc.y),
					acosf(ukf_filter.state.acc.z)); // 2* 90 , 1* 45
	ukf_filter.state.attitude.x = theta * rad_to_deg();

	float psi = 2.0
			* atan2f(asinf(ukf_filter.state.acc.x),
					acosf(ukf_filter.state.acc.z)); // 2* 90 , 1* 45
	ukf_filter.state.attitude.y = psi * rad_to_deg();

	ukf_filter.state.angles.pitch_y = atan2f(ukf_filter.state.acc.x,
			-ukf_filter.state.acc.z); //180
	ukf_filter.state.angles.roll_x = atan2f(ukf_filter.state.acc.y,
			-ukf_filter.state.acc.z); //180

	ukf_filter.state.accy_to_accx = atan2f(ukf_filter.state.acc.y,
			ukf_filter.state.acc.x);
	ukf_filter.state.accx_to_sin_theta = ukf_filter.state.acc.x
			/ sinf(ukf_filter.state.angles.pitch_y);
	ukf_filter.state.accy_to_sin_theta = ukf_filter.state.acc.y
			/ sinf(ukf_filter.state.angles.pitch_y);

	bool use_magnetometer = (NULL != magnet);
	if (use_magnetometer) {
		Vector3D_t mag = { (float) magnet->AXIS_X, (float) magnet->AXIS_Y,
				(float) magnet->AXIS_Z };
		float magnet_magnitude = math_vec_mag(&mag);
		mag.x /= magnet_magnitude;
		mag.y /= magnet_magnitude;
		mag.z /= magnet_magnitude;

		// AG: X North Y East Z Down, M: Y North X East Z Down NWD
		float mx = mag.x * cosf(ukf_filter.state.angles.pitch_y)
				+ mag.z * sinf(ukf_filter.state.angles.pitch_y);
		float my = mag.y * sinf(ukf_filter.state.angles.roll_x)
				* sinf(ukf_filter.state.angles.pitch_y)
				+ mag.y * cosf(ukf_filter.state.angles.roll_x)
				- mag.z * sinf(ukf_filter.state.angles.roll_x)
						* cosf(ukf_filter.state.angles.pitch_y);

		ukf_filter.state.angles.yaw_z = atan2f(mx, my); //y is north; (my, mx) - x is north

		// normalize yaw to -pi .. pi
		if (ukf_filter.state.angles.yaw_z > MAX_RAD) {
			ukf_filter.state.angles.yaw_z -= 2.f * MAX_RAD;
		} else if (ukf_filter.state.angles.yaw_z < -MAX_RAD) {
			ukf_filter.state.angles.yaw_z += 2.f * MAX_RAD;
		}
		ukf_filter.state.angles.yaw_z *= rad_to_deg();
	}

	ukf_filter.state.angles.roll_x *= rad_to_deg();
	ukf_filter.state.angles.pitch_y *= rad_to_deg();

	ukf_filter.state.angular_vel.x = (float) gyro->AXIS_X;
	ukf_filter.state.angular_vel.y = (float) gyro->AXIS_Y;
	ukf_filter.state.angular_vel.z = (float) gyro->AXIS_Z;

	ukf_filter.state.gyro_angle_rates = *ahrs_get_euler_derivatives(phi, theta,
			ukf_filter.state.angular_vel.x, ukf_filter.state.angular_vel.y,
			ukf_filter.state.angular_vel.z);

	//to degree
	ukf_filter.state.gyro_angles.gz = ukf_filter.state.gyro_angle_rates.gz
			* ukf_filter.dt * rad_to_deg();
	ukf_filter.state.gyro_angles.gy = ukf_filter.state.gyro_angle_rates.gy
			* ukf_filter.dt * rad_to_deg();
	ukf_filter.state.gyro_angles.gx = ukf_filter.state.gyro_angle_rates.gx
			* ukf_filter.dt * rad_to_deg();

//	float check_x = get_geo_g() + sinf(theta) * cosf(phi);
//	//assert(accel->x == check_x);
//	float check_y = get_geo_g() + sinf(theta) * sinf(phi);
//	//assert(accel->y == check_y);
//	float check_z = get_geo_g() + cosf(theta);
//	//assert(accel->z == check_z);
}

void estimation_ukf(const AxesRaw_t *accel, const AxesRaw_t *gyro,
		const AxesRaw_t *magnet, const float diff_us) {
	ukf_filter.dt = diff_us * us_to_second(); // us to second
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

inline UKF_t* get_ukf_filter() {
	return &ukf_filter;
}
