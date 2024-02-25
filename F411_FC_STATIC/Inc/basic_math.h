#ifndef __BASIC_MATH_H_
#define __BASIC_MATH_H_

//#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <tgmath.h>

#define EPSILON_2		(1e-2)
#define EPSILON_3 		(1e-3)
#define EPSILON_4 		(1e-4)
#define EPSILON_7 		(1e-7)

#define MAX_RAD    		3.141592653589793238462643383279502884197 	//	180 degree

typedef struct Vector2D {
	float x;
	float y;
} Vector2D_t;

typedef struct Vector3D {
	float x;
	float y;
	float z;
} Vector3D_t;

typedef union Matrix2D_u {
	float matrix[2][2];
	struct Matrix2D {
		float row0[2];
		float row1[2];
	} rows;
} Matrix2D_t;

typedef union Matrix3D_u {
	float matrix[3][3];
	struct Matrix3D {
		float row0[3];
		float row1[3];
		float row2[3];
	} rows;
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
float math_absf(float x);
int16_t math_abs(int16_t x);
int16_t math_max(int16_t val1, int16_t val2);
int16_t math_min(int16_t val1, int16_t val2);
float sign0(float value);
float sign1(float value);
float math_mean(int length, float *data);
float math_variance(int length, float mean, float *data);
float math_stddev(float variance);
float math_covariance(int length, float *x, float mean_x, float *y, float mean_y);
float math_correlation(float cov, float stddev_x, float stddev_y);
float math_pdf_normal(float x, float mean_x, float stddev_x);
float math_vec_mag(Vector3D_t *vec);

#endif /* __BASIC_MATH_H_ */
