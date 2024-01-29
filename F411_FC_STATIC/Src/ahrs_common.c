/*
 * ahrs_common.c
 *
 *  Created on: Jan 14, 2024
 *      Author: konrad
 */
#include "ahrs_common.h"

extern int16_t gTHR;
extern const float SENSOR_SAMPLING_TIME;

const float AHRS_KP_BIG = 10.0f;         //0.4 is tested, so slow in calibrated.
const float AHRS_KP_NORM = 0.4f;
const float AHRS_KI = 0.1f;
const float COE_DPS_TO_RADPS = 0.0174533;

const short MIN_THROTTLE_ONESHOT42 = 4210; //or 5200
const short MIN_THROTTLE = 51;

volatile float q0 = 1, q1 = 0, q2 = 0, q3 = 0;
volatile float exInt = 0, eyInt = 0, ezInt = 0;

float ahrs_kp;
float beta = 0.1f;		// 2 * proportional gain;

static Vector3D_t euler_derivatives;
static Vector3D_t weighted_average;

static float ahrs_weighted_average(int16_t axis1, int priority1, int16_t axis2,
		int priority2) {
	return (axis1 * priority1 + axis2 * priority2) / (priority1 + priority2);
}

Vector3D_t* ahrs_get_euler_derivatives(float phi, float theta, float p, float q,
		float r) {
//Euler forward method
	euler_derivatives.x = 1.0 * p + sinf(phi) * tanf(theta) * q
			+ cosf(phi) * tanf(theta) * r;
	euler_derivatives.y = cosf(phi) * q - sinf(phi) * r;
	euler_derivatives.z = sinf(phi) / cosf(theta) * q
			+ cosf(phi) / cosf(theta) * r;
	return &euler_derivatives;
}

Vector3D_t* ahrs_get_weighted_average(AxesWeightedAverage_t *avg1,
		AxesWeightedAverage_t *avg2) {
	weighted_average.x = ahrs_weighted_average(avg1->axis->AXIS_X,
			avg1->priority, avg2->axis->AXIS_X, avg2->priority);
	weighted_average.y = ahrs_weighted_average(avg1->axis->AXIS_Y,
			avg1->priority, avg2->axis->AXIS_Y, avg2->priority);
	weighted_average.z = ahrs_weighted_average(avg1->axis->AXIS_Z,
			avg1->priority, avg2->axis->AXIS_Z, avg2->priority);
	return &weighted_average;
}

void ahrs_fusion_ag(AxesRaw_t *accel, AxesRaw_t *gyro, AhrsState_t *ahrs) {
	float axf, ayf, azf, gxf, gyf, gzf;
	float norm;
	float vx, vy, vz;
	float ex, ey, ez;
	float halfT;
	float q0q0, q0q1, q0q2, /*q0q3,*/q1q1, /*q1q2,*/q1q3, q2q2, q2q3, q3q3;

	if (gTHR < MIN_THROTTLE_ONESHOT42) {
		ahrs_kp = AHRS_KP_BIG;
	} else {
		ahrs_kp = AHRS_KP_NORM;
	}

	axf = (float) accel->AXIS_X;
	ayf = (float) accel->AXIS_Y;
	azf = (float) accel->AXIS_Z;

// mdps convert to rad/s
	gxf = ((float) gyro->AXIS_X) * ((float) COE_DPS_TO_RADPS);
	gyf = ((float) gyro->AXIS_Y) * ((float) COE_DPS_TO_RADPS);
	gzf = ((float) gyro->AXIS_Z) * ((float) COE_DPS_TO_RADPS);

// auxiliary variables to reduce number of repeated operations
	q0q0 = q0 * q0;
	q0q1 = q0 * q1;
	q0q2 = q0 * q2;
//q0q3 = q0*q3;
	q1q1 = q1 * q1;
//q1q2 = q1*q2;
	q1q3 = q1 * q3;
	q2q2 = q2 * q2;
	q2q3 = q2 * q3;
	q3q3 = q3 * q3;

// normalise the accelerometer measurement
	norm = math_inv_sqrt(axf * axf + ayf * ayf + azf * azf);

	axf = axf * norm;
	ayf = ayf * norm;
	azf = azf * norm;

// estimated direction of gravity and flux (v and w)
	vx = 2 * (q1q3 - q0q2);
	vy = 2 * (q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;

	ex = (ayf * vz - azf * vy);
	ey = (azf * vx - axf * vz);
	ez = (axf * vy - ayf * vx);

// integral error scaled integral gain
	exInt = exInt + ex * AHRS_KI * SENSOR_SAMPLING_TIME;
	eyInt = eyInt + ey * AHRS_KI * SENSOR_SAMPLING_TIME;
	ezInt = ezInt + ez * AHRS_KI * SENSOR_SAMPLING_TIME;

// adjusted gyroscope measurements
	gxf = gxf + ahrs_kp * ex + exInt;
	gyf = gyf + ahrs_kp * ey + eyInt;
	gzf = gzf + ahrs_kp * ez + ezInt;

// integrate quaternion rate and normalise
	halfT = 0.5f * SENSOR_SAMPLING_TIME;
	q0 = q0 + (-q1 * gxf - q2 * gyf - q3 * gzf) * halfT;
	q1 = q1 + (q0 * gxf + q2 * gzf - q3 * gyf) * halfT;
	q2 = q2 + (q0 * gyf - q1 * gzf + q3 * gxf) * halfT;
	q3 = q3 + (q0 * gzf + q1 * gyf - q2 * gxf) * halfT;

// normalise quaternion
	norm = math_inv_sqrt(q0q0 + q1q1 + q2q2 + q3q3);
	q0 *= norm;
	q1 *= norm;
	q2 *= norm;
	q3 *= norm;

	ahrs->q.q0 = q0;
	ahrs->q.q1 = q1;
	ahrs->q.q2 = q2;
	ahrs->q.q3 = q3;
}
