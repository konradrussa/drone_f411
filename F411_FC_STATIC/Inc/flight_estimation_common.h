/*
 * flight_estimation_common.h
 *
 *  Created on: Feb 21, 2024
 *      Author: konrad
 */

#ifndef FLIGHT_ESTIMATION_COMMON_H_
#define FLIGHT_ESTIMATION_COMMON_H_

#include "basic_math.h"

struct UKF_State {
	Vector3D_t pos;
	Vector3D_t vel;
	Vector3D_t acc;
};

struct UKF_Measurement {
	Vector3D_t acc;
	Vector3D_t gyro;
	Vector3D_t mag;
};

typedef struct UKF {
	struct UKF_State state;
	struct UKF_Measurement measurement;
	float dt;
} UKF_t;

//Kalman filter
void estimation_ukf(void);
UKF_t* get_ukf_filter();

//BLUE estimate
void estimation_blue(const float time[3], const Vector3D_t observations);

#endif /* FLIGHT_ESTIMATION_COMMON_H_ */
