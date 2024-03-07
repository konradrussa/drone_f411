/*
 * flight_estimation_common.h
 *
 *  Created on: Feb 21, 2024
 *      Author: konrad
 */

#ifndef FLIGHT_ESTIMATION_COMMON_H_
#define FLIGHT_ESTIMATION_COMMON_H_

#include "basic_math.h"
#include "ahrs_common.h"

struct UKF_State {
	Vector3D_t pos;
	Vector3D_t vel;
	Vector3D_t acc;
	EulerAngle_t angles;
	Vector3D_t attitude; //orientation
	GyroRad_t gyro_angles;
	GyroRad_t gyro_angle_rates;
	Vector3D_t angular_vel;
	float accy_to_accx;
	float accx_to_sin_theta;
	float accy_to_sin_theta;
};

struct UKF_Covariance {
	Vector3D_t acc;
	Vector3D_t gyro;
	Vector3D_t mag;
};

typedef struct UKF {
	struct UKF_State state;
	struct UKF_Covariance covariance;
	float dt;
} UKF_t;

//Kalman filter
void estimation_ukf(const AxesRaw_t *accel, const AxesRaw_t *gyro,
		const AxesRaw_t *magnet);
UKF_t* get_ukf_filter();

//BLUE estimate
void estimation_blue(const float time[3], const Vector3D_t observations);

#endif /* FLIGHT_ESTIMATION_COMMON_H_ */
