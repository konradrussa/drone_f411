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

volatile float q0 = 1, q1 = 0, q2 = 0, q3 = 0;
volatile float exInt = 0, eyInt = 0, ezInt = 0;

float ahrs_kp;
float beta = 0.1f;		// 2 * proportional gain;

static EulerAngle_t euler_derivatives;
static Vector3D_t weighted_average;

static Matrix3D_t mat_product = { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0,
		0.0, 0.0 } }; 		//3x3
static Matrix3D_t rot_mat_X = { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0,
		0.0, 0.0 } }; //3x3
static Matrix3D_t rot_mat_Y = { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0,
		0.0, 0.0 } }; 		//3x3
static Matrix3D_t rot_mat_Z = { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0,
		0.0, 0.0 } }; 		//3x3

static Eigen_t eigen;

static float ahrs_weighted_average(int16_t axis1, int priority1, int16_t axis2,
		int priority2) {
	return (axis1 * priority1 + axis2 * priority2) / (priority1 + priority2);
}

static void ahrs_create_rotation_matrix_X(float phi, float theta, float psi) {
	rot_mat_X.row0[0] = 1.0;
	rot_mat_X.row0[1] = 0.0;
	rot_mat_X.row0[2] = 0.0;
	rot_mat_X.row1[0] = 0.0;
	rot_mat_X.row1[1] = cosf(phi);
	rot_mat_X.row1[2] = -sinf(phi);
	rot_mat_X.row2[0] = 0.0;
	rot_mat_X.row2[1] = sinf(phi);
	rot_mat_X.row2[2] = cosf(phi);
}

static void ahrs_create_rotation_matrix_Y(float phi, float theta, float psi) {
	rot_mat_Y.row0[0] = cosf(theta);
	rot_mat_Y.row0[1] = 0.0;
	rot_mat_Y.row0[2] = sinf(theta);
	rot_mat_Y.row1[0] = 0.0;
	rot_mat_Y.row1[1] = 1.0;
	rot_mat_Y.row1[2] = 0.0;
	rot_mat_Y.row2[0] = -sinf(theta);
	rot_mat_Y.row2[1] = 0.0;
	rot_mat_Y.row2[2] = cosf(theta);
}

static void ahrs_create_rotation_matrix_Z(float phi, float theta, float psi) {
	rot_mat_Z.row0[0] = cosf(psi);
	rot_mat_Z.row0[1] = -sinf(psi);
	rot_mat_Z.row0[2] = 0.0;
	rot_mat_Z.row1[0] = sinf(psi);
	rot_mat_Z.row1[1] = cosf(psi);
	rot_mat_Z.row1[2] = 0.0;
	rot_mat_Z.row2[0] = 0.0;
	rot_mat_Z.row2[1] = 0.0;
	rot_mat_Z.row2[2] = 1.0;
}

static void ahrs_multiply_rotation_matrix(const Matrix3D_t *mat1,
		const Matrix3D_t *mat2, Matrix3D_t *mat_prod) {
	//rows * cols
	mat_prod->row0[0] = mat1->row0[0] * mat2->row0[0]
			+ mat1->row0[1] * mat2->row1[0] + mat1->row0[2] * mat2->row2[0];
	mat_prod->row0[1] = mat1->row0[0] * mat2->row0[1]
			+ mat1->row0[1] * mat2->row1[1] + mat1->row0[2] * mat2->row2[1];
	mat_prod->row0[2] = mat1->row0[0] * mat2->row0[2]
			+ mat1->row0[1] * mat2->row1[2] + mat1->row0[2] * mat2->row2[2];

	mat_prod->row1[0] = mat1->row1[0] * mat2->row0[0]
			+ mat1->row1[1] * mat2->row1[0] + mat1->row1[2] * mat2->row2[0];
	mat_prod->row1[1] = mat1->row1[0] * mat2->row0[1]
			+ mat1->row1[1] * mat2->row1[1] + mat1->row1[2] * mat2->row2[1];
	mat_prod->row1[2] = mat1->row1[0] * mat2->row0[2]
			+ mat1->row1[1] * mat2->row1[2] + mat1->row1[2] * mat2->row2[2];

	mat_prod->row2[0] = mat1->row2[0] * mat2->row0[0]
			+ mat1->row2[1] * mat2->row1[0] + mat1->row2[2] * mat2->row2[0];
	mat_prod->row2[1] = mat1->row2[0] * mat2->row0[1]
			+ mat1->row2[1] * mat2->row1[1] + mat1->row2[2] * mat2->row2[1];
	mat_prod->row2[2] = mat1->row2[0] * mat2->row0[2]
			+ mat1->row2[1] * mat2->row1[2] + mat1->row2[2] * mat2->row2[2];
}

Matrix3D_t* ahrs_get_rotation_matrix(float phi, float theta, float psi) {
	//Generate the rotation matrix for the given roll, pitch and yaw angles
	ahrs_create_rotation_matrix_X(phi, theta, psi);
	ahrs_create_rotation_matrix_Y(phi, theta, psi);
	ahrs_create_rotation_matrix_Z(phi, theta, psi);
	//yaw + pitch * roll
	Matrix3D_t pitch_roll_product = { { 0.0 }, { 0.0 }, { 0.0 } }; 		//3x3
	ahrs_multiply_rotation_matrix(&rot_mat_Y, &rot_mat_X, &pitch_roll_product);	//pitch * roll
	ahrs_multiply_rotation_matrix(&rot_mat_Z, &pitch_roll_product,
			&mat_product);		//yaw * pitch * roll
	return &mat_product;
}

EulerAngle_t* ahrs_get_euler_derivatives(float phi, float theta, float p, float q,
		float r) {
	//Euler forward method
	euler_derivatives.roll_x = 1.0 * p + sinf(phi) * tanf(theta) * q
			+ cosf(phi) * tanf(theta) * r; //roll
	euler_derivatives.pitch_y = cosf(phi) * q - sinf(phi) * r; //pitch
	euler_derivatives.yaw_z = sinf(phi) / cosf(theta) * q
			+ cosf(phi) / cosf(theta) * r; //yaw
	return &euler_derivatives;
}

Vector3D_t* ahrs_get_weighted_average(const AxesWeightedAverage_t *avg1,
		const AxesWeightedAverage_t *avg2) {
	weighted_average.x = ahrs_weighted_average(avg1->axis->AXIS_X,
			avg1->priority, avg2->axis->AXIS_X, avg2->priority);
	weighted_average.y = ahrs_weighted_average(avg1->axis->AXIS_Y,
			avg1->priority, avg2->axis->AXIS_Y, avg2->priority);
	weighted_average.z = ahrs_weighted_average(avg1->axis->AXIS_Z,
			avg1->priority, avg2->axis->AXIS_Z, avg2->priority);
	return &weighted_average;
}

// Madgwick AHRS
void ahrs_fusion_ag(const AxesRaw_t *accel, const AxesRaw_t *gyro,
		AhrsState_t *ahrs) {
	float axf, ayf, azf, gxf, gyf, gzf;
	float norm;
	float vx, vy, vz;
	float ex, ey, ez;
	float halfT;
	float q0q0, q0q1, q0q2, /*q0q3,*/q1q1, /*q1q2,*/q1q3, q2q2, q2q3, q3q3;

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
