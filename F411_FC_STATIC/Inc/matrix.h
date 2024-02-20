/*
 * matrix.h
 *
 *  Created on: Feb 20, 2024
 *      Author: konrad
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#include "basic_math.h"

Matrix3D_t* matrix_get_rotation_matrix(float phi, float theta, float psi);
void matrix_rotation_matrix_vector_product(const Matrix3D_t *matrix,
		const Vector3D_t *vector, Vector3D_t *out);
bool matrix_rotation_matrix_in_range(float det);
float matrix_rotation_matrix_determinant(const Matrix3D_t *matrix);
Matrix3D_t matrix_rotation_matrix_IwrtB(Quaternion_t *q);
Matrix3D_t matrix_rotation_matrix_BwrtI(Quaternion_t *q);

#endif /* MATRIX_H_ */