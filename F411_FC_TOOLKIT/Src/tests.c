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

#include "unity.h"

void test_quaternion() {
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
void test_euler_angles() {

}
void test_rotation_matrix() {

}

