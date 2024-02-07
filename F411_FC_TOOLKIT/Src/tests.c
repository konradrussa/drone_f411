/*
 * tests.c
 *
 *  Created on: Feb 5, 2024
 *      Author: konrad
 */
#include <assert.h>
#include "ahrs_common.h"
#include "basic_math.h"
#include "quaternion.h"
#include "flight_control_common.h"

#include "unity.h"

void test_quaternion_and_euler_angles_and_rotation_matrix() {
	Quaternion_t q = { 0.877111, 0.152270, 0.308935, 0.334738 };
	EulerAngle_t ea;
	quaternion_to_euler(&q, &ea);
	//printf("quaternion_to_euler %9.6f, %9.6f, %9.6f\n", ea.roll_x, ea.pitch_y, ea.yaw_z);
	quaternion_from_euler(&ea, &q);
	//printf("quaternion_from_euler %9.6f, %9.6f, %9.6f, %9.6f\n", q.q0, q.q1, q.q2, q.q3);
	quaternion_to_euler(&q, &ea);
	//printf("quaternion_to_euler %9.6f, %9.6f, %9.6f\n", ea.roll_x, ea.pitch_y, ea.yaw_z);

	Matrix3D_t m1 = *ahrs_get_rotation_matrix(ea.roll_x, ea.pitch_y, ea.yaw_z);
	//printf("Rot mat row 1 %9.6f, %9.6f, %9.6f\n", m1.row0[0], m1.row0[1], m1.row0[2]);
	//printf("Rot mat row 2 %9.6f, %9.6f, %9.6f\n", m1.row1[0], m1.row1[1], m1.row1[2]);
	//printf("Rot mat row 3 %9.6f, %9.6f, %9.6f\n", m1.row2[0], m1.row2[1], m1.row2[2]);
	Vector3D_t v1 = { 1.0, 2.0, 3.0 };
	Eigen_t eigen;
	math_eigen(&m1, &v1, &eigen);
	//printf("Rot mat determinant: %9.6f\n", det);
	//printf("Eigen value %9.6f\n", eigen.eigenvalue);
	//printf("Eigen vector %9.6f %9.6f %9.6f\n", eigen.eigenvector.x, eigen.eigenvector.y, eigen.eigenvector.z);

	float det = math_rotation_matrix_determinant(&m1);
	bool determinant_valid = math_rotation_matrix_in_range(det);
	assert(determinant_valid);
	//assert(det == 1.0);

}

const float test_pid_kp = 0.5;
const float test_pid_ki = 0.2;
const float test_pid_kd = 0.1;

static float dT = 0.001;

void test_pid_control(float start, float goal) {
	flight_get_pid_var()->new_val_p = 0;
	flight_get_pid_var()->new_val_i = 0;
	flight_get_pid_var()->new_val_d = 0;
	flight_get_pid_var()->prev_val = 0;
	float output = 0.0;
	float measured = start;
	flight_get_pid_var()->dt = dT;
	for (int i = 0; i < 100; i++) {
		flight_get_pid_var()->update_pid(goal, measured);

		output = test_pid_kp * flight_get_pid_var()->new_val_p
				+ test_pid_ki * flight_get_pid_var()->new_val_i
				+ test_pid_kd * flight_get_pid_var()->new_val_d;

		measured += output;
		//printf("PID goal=%9.6f measured=%9.6f output=%9.6f\n", goal, measured, output);
		if (measured == goal) {
			break;
		}
	}
}

void test_sm_control(float start, float goal) {
	flight_get_sm_var()->sliding_mode = 0;
	flight_get_sm_var()->sliding_surface = 0;
	flight_get_sm_var()->prev_val = 0;
	float output = 0.0;
	float measured = start;
	flight_get_sm_var()->dt = dT;
	for (int i = 0; i < 100; i++) {
		flight_get_sm_var()->update_sm(goal, measured);
		output = flight_get_sm_var()->sliding_mode;
		measured += output;
		//printf("SM goal=%9.6f measured=%9.6f output=%9.6f\n", goal, measured, output);
		if (measured == goal) {
			break;
		}
	}
}

//int main() {
//test_pid_control(2, 7000);
//test_pid_control(7000, 4200);

//test_sm_control(2, 7000);
//test_sm_control(7000, 4200);
//}
