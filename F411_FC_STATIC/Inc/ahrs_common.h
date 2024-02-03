/*
 * ahrs_common.h
 *
 *  Created on: Jan 14, 2024
 *      Author: konrad
 */

#ifndef AHRS_COMMON_H_
#define AHRS_COMMON_H_

#include <stdlib.h>
#include "basic_math.h"
#include "quaternion.h"

typedef struct AhrsState {
	Quaternion_t q;    		// Current attitude
	EulerAngle_t ea;        // Current angle rate @ body frame
} AhrsState_t;

typedef struct GyroRad {
	float gx, gy, gz;
} GyroRad_t;

typedef struct AxesRaw {
	int16_t AXIS_X;
	int16_t AXIS_Y;
	int16_t AXIS_Z;
} AxesRaw_t;

typedef struct AxesWeightedAverage {
	AxesRaw_t *axis;
	int priority;
} AxesWeightedAverage_t;

void ahrs_fusion_ag(const AxesRaw_t *acc, const AxesRaw_t *gyro, AhrsState_t *ahrs);
Vector3D_t* ahrs_get_euler_derivatives(float phi, float theta, float p, float q,
		float r);
Vector3D_t* ahrs_get_weighted_average(const AxesWeightedAverage_t *avg1,
		const AxesWeightedAverage_t *avg2);
Matrix3D_t* ahrs_get_rotation_matrix(float phi, float theta, float psi);

#endif /* AHRS_COMMON_H_ */
