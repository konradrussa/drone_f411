#ifndef __BASIC_MATH_H_
#define __BASIC_MATH_H_

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <tgmath.h>

#define EPSILON_2		(1e-2)
#define EPSILON_3 		(1e-3)
#define EPSILON_4 		(1e-4)

#define MAX_RAD    		3.141592653589793238462643383279502884197 	//	180 degree

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

typedef struct Eigen {
	Vector3D_t eigenvector;
	float eigenvalue;
} Eigen_t;

typedef struct Quaternion {
	float q0;			//scalar part
	float q1, q2, q3;	//vector part
} Quaternion_t;

typedef struct EulerAngle {
	float roll_x;
	float pitch_y;
	float yaw_z;
} EulerAngle_t;

float math_sqrt(float x);
float math_inv_sqrt(float x);
int16_t math_abs(int16_t x);
int16_t math_max(int16_t val1, int16_t val2);
int16_t math_min(int16_t val1, int16_t val2);
float sign0(float value);
float sign1(float value);
bool math_rotation_matrix_in_range(float det);
float math_rotation_matrix_determinant(const Matrix3D_t *matrix);
void math_eigen(const Matrix3D_t *matrix, const Vector3D_t *vector,
		Eigen_t *eigen);
double math_mean(int length, double data, ...);
double math_stddev(int length, double mean, double data, ...);

#endif /* __BASIC_MATH_H_ */
