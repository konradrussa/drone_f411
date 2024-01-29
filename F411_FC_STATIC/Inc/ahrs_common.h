/*
 * ahrs_common.h
 *
 *  Created on: Jan 14, 2024
 *      Author: konrad
 */

#ifndef AHRS_COMMON_H_
#define AHRS_COMMON_H_

#include <tgmath.h>
#include <stdlib.h>
#include "basic_math.h"
#include "quaternion.h"

typedef struct AhrsState {
	Quaternion_t q;    		// Current altitude
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

typedef struct Vector3D {
	float x;
	float y;
	float z;
} Vector3D_t;

void ahrs_fusion_ag(AxesRaw_t *acc, AxesRaw_t *gyro, AhrsState_t *ahrs);
Vector3D_t* ahrs_get_euler_derivatives(float phi, float theta, float p, float q,
		float r);
Vector3D_t* ahrs_get_weighted_average(AxesWeightedAverage_t *avg1,
		AxesWeightedAverage_t *avg2);

#endif /* AHRS_COMMON_H_ */
