#ifndef __BASIC_MATH_H_
#define __BASIC_MATH_H_

#include <stdint.h>
#include <stdbool.h>

#define EPSILON 		(1e-4)

typedef struct Vector3D {
	float x;
	float y;
	float z;
} Vector3D_t;

typedef struct Matrix3D {
	float row0[3];
	float row1[3];
	float row2[3];
} Matrix3D_t;

float math_sqrt(float x);
float math_inv_sqrt(float x);
int16_t math_abs(int16_t x);
int16_t math_max(int16_t val1, int16_t val2);
int16_t math_min(int16_t val1, int16_t val2);
bool math_rotation_matrix_in_range(int16_t det);
int16_t math_rotation_matrix_determinant(Matrix3D_t *matrix);

#endif /* __BASIC_MATH_H_ */
