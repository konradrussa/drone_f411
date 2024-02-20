/*
 * matrix.c
 *
 *  Created on: Feb 20, 2024
 *      Author: konrad
 */

#include "matrix.h"

static Matrix3D_t mat_product = { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0,
		0.0, 0.0 } }; 		//3x3
static Matrix3D_t rot_mat_X = { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0,
		0.0, 0.0 } }; //3x3
static Matrix3D_t rot_mat_Y = { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0,
		0.0, 0.0 } }; 		//3x3
static Matrix3D_t rot_mat_Z = { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0,
		0.0, 0.0 } }; 		//3x3

static void matrix_create_rotation_matrix_X(float phi, float theta, float psi) {
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

static void matrix_create_rotation_matrix_Y(float phi, float theta, float psi) {
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

static void matrix_create_rotation_matrix_Z(float phi, float theta, float psi) {
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

static void matrix_multiply_rotation_matrix(const Matrix3D_t *mat1,
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

static Matrix3D_t* matrix_add(Matrix3D_t *matrix1, Matrix3D_t *matrix2) {
	matrix1->row0[0] += matrix2->row0[0];
	matrix1->row0[1] += matrix2->row0[1];
	matrix1->row0[2] += matrix2->row0[2];
	matrix1->row1[0] += matrix2->row1[0];
	matrix1->row1[1] += matrix2->row1[1];
	matrix1->row1[2] += matrix2->row1[2];
	matrix1->row1[0] += matrix2->row1[0];
	matrix1->row1[1] += matrix2->row1[1];
	matrix1->row1[2] += matrix2->row1[2];
	return matrix1;
}

static Matrix3D_t* matrix_multiply_scalar(Matrix3D_t *matrix, const float a) {
	matrix->row0[0] *= a;
	matrix->row0[1] *= a;
	matrix->row0[2] *= a;
	matrix->row1[0] *= a;
	matrix->row1[1] *= a;
	matrix->row1[2] *= a;
	matrix->row1[0] *= a;
	matrix->row1[1] *= a;
	matrix->row1[2] *= a;
	return matrix;
}

static Matrix3D_t matrix_identity() {
	Matrix3D_t matrix;
	matrix.row0[0] = 1.0f;
	matrix.row0[1] = 0.0f;
	matrix.row0[2] = 0.0f;
	matrix.row1[0] = 0.0f;
	matrix.row1[1] = 1.0f;
	matrix.row1[2] = 0.0f;
	matrix.row1[0] = 0.0f;
	matrix.row1[1] = 0.0f;
	matrix.row1[2] = 1.0f;
	return matrix;
}

static void matrix_transpose(Matrix3D_t *matrix) {
	float r01 = matrix->row0[1];
	float r10 = matrix->row1[0];
	matrix->row0[1] = r10;
	matrix->row1[0] = r01;
	float r02 = matrix->row0[2];
	float r20 = matrix->row2[0];
	matrix->row0[2] = r20;
	matrix->row2[0] = r02;
	float r12 = matrix->row1[2];
	float r21 = matrix->row2[1];
	matrix->row1[2] = r21;
	matrix->row2[1] = r12;
}

static Matrix3D_t matrix_skew_symmetric_from_scalar(const float a,
		const float b, const float c) {
	Matrix3D_t matrix;
	matrix.row0[0] = 0.0f;
	matrix.row0[1] = -c;
	matrix.row0[2] = b;
	matrix.row1[0] = c;
	matrix.row1[1] = 0.0f;
	matrix.row1[2] = -a;
	matrix.row1[0] = -b;
	matrix.row1[1] = a;
	matrix.row1[2] = 0.0f;
	return matrix;
}

static Matrix3D_t matrix_skew_symmetric_from_vector(const Vector3D_t *vec) {
	return matrix_skew_symmetric_from_scalar(vec->x, vec->y, vec->z);
}

//a^T*b
static Matrix3D_t matrix_outer_product(const Vector3D_t *a, const Vector3D_t *b) {
	Matrix3D_t matrix;
	matrix.row0[0] = a->x * b->x;
	matrix.row0[1] = a->x * b->y;
	matrix.row0[2] = a->x * b->z;
	matrix.row1[0] = a->y * b->x;
	matrix.row1[1] = a->y * b->y;
	matrix.row1[2] = a->y * b->z;
	matrix.row2[0] = a->z * b->x;
	matrix.row2[1] = a->z * b->y;
	matrix.row2[2] = a->z * b->z;
	return matrix;
}

Matrix3D_t* matrix_get_rotation_matrix(float phi, float theta, float psi) {
//Generate the rotation matrix for the given roll, pitch and yaw angles
	matrix_create_rotation_matrix_X(phi, theta, psi);
	matrix_create_rotation_matrix_Y(phi, theta, psi);
	matrix_create_rotation_matrix_Z(phi, theta, psi);
//yaw + pitch * roll
	Matrix3D_t pitch_roll_product = { { 0.0 }, { 0.0 }, { 0.0 } }; 		//3x3
	matrix_multiply_rotation_matrix(&rot_mat_Y, &rot_mat_X,
			&pitch_roll_product); 		//pitch * roll
	matrix_multiply_rotation_matrix(&rot_mat_Z, &pitch_roll_product,
			&mat_product);	//yaw * pitch * roll
	return &mat_product;
}

// determinant for Rotation Matrix should be 1 +-epsilon
inline bool matrix_rotation_matrix_in_range(float det) {
	return (det <= 1 + EPSILON_4 && det >= 1 - EPSILON_4);
}

float matrix_rotation_matrix_determinant(const Matrix3D_t *matrix) {
	return matrix->row0[0]
			* (matrix->row1[1] * matrix->row2[2]
					- matrix->row2[1] * matrix->row1[2])
			- matrix->row0[1]
					* (matrix->row1[0] * matrix->row2[2]
							- matrix->row2[0] * matrix->row1[2])
			+ matrix->row0[2]
					* (matrix->row1[0] * matrix->row2[1]
							- matrix->row2[0] * matrix->row1[1]);
}

void matrix_rotation_matrix_vector_product(const Matrix3D_t *matrix,
		const Vector3D_t *vector, Vector3D_t *out) {
	out->x = matrix->row0[0] * vector->x + matrix->row0[1] * vector->y
			+ matrix->row0[2] * vector->z;
	out->y = matrix->row1[0] * vector->x + matrix->row1[1] * vector->y
			+ matrix->row1[2] * vector->z;
	out->z = matrix->row2[0] * vector->x + matrix->row2[1] * vector->y
			+ matrix->row2[2] * vector->z;
}

Matrix3D_t matrix_rotation_matrix_IwrtB(Quaternion_t *q) {
//transformation matrix of inertial w.r.t. the body
	Vector3D_t qvec = { q->q1, q->q2, q->q3 };
	Matrix3D_t m_sk_v = matrix_skew_symmetric_from_vector(&qvec); // Build the skew symmetric matrix of the imaginary part of quaternion
	Matrix3D_t m_ident = matrix_identity();
	float vec_mag_sq = qvec.x * qvec.x + qvec.y * qvec.y + qvec.z * qvec.z;
	Matrix3D_t mat_outer_prod = matrix_outer_product(&qvec, &qvec);
	return *matrix_add(
			matrix_add(
					matrix_multiply_scalar(&m_ident,
							(q->q0 * q->q0 - vec_mag_sq)),
					matrix_multiply_scalar(&mat_outer_prod, 2)),
			matrix_multiply_scalar(&m_sk_v, 2 * q->q0));
}

Matrix3D_t matrix_rotation_matrix_BwrtI(Quaternion_t *q) {
	//transformation matrix of body w.r.t. inertial
	Matrix3D_t mat = matrix_rotation_matrix_IwrtB(q);
	matrix_transpose(&mat);
	return mat;
}
