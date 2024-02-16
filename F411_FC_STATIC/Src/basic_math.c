#include "basic_math.h"

inline int16_t math_max(int16_t val1, int16_t val2) {
	return val1 > val2 ? val1 : val2;
}

inline int16_t math_min(int16_t val1, int16_t val2) {
	return val1 < val2 ? val1 : val2;
}

inline int16_t math_abs(int16_t x) {
	return x >= 0 ? x : -x;
}

inline float math_absf(float x) {
	return x >= 0.0 ? x : -x;
}

inline float sign1(float value) {
	if (value > 1.0)
		return 1.0;
	if (value < -1.0)
		return -1.0;
	return value;
}

inline float sign0(float value) {
	if (value > 0.0)
		return 1.0;
	if (value < 0.0)
		return -1.0;
	return 0.0;
}

// determinant for Rotation Matrix should be 1 +-epsilon
inline bool math_rotation_matrix_in_range(float det) {
	return (det <= 1 + EPSILON_4 && det >= 1 - EPSILON_4);
}

float math_rotation_matrix_determinant(const Matrix3D_t *matrix) {
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

float math_sqrt(float x) {
	union {
		int i;
		float x;
	} u;
	u.x = x;
	u.i = (1 << 29) + (u.i >> 1) - (1 << 22); //initial guess number

	// Two Babylonian Steps (simplified from:)
	// u.x = 0.5f * (u.x + x/u.x);
	// u.x = 0.5f * (u.x + x/u.x);
	u.x = u.x + x / u.x;
	u.x = 0.25f * u.x + x / u.x;

	return u.x;
}

//---------------------------------------------------------------------------------------------------
// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root
// fast form of 1/sqrt
float math_inv_sqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*) &y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*) &i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

// power method, matrix order 3 (3x3)
void math_eigen(const Matrix3D_t *matrix, const Vector3D_t *vector,
		Eigen_t *eigen) {
	float zmax = 0.0, emax = 0.0;
	Vector3D_t vec_x = *vector;
	Vector3D_t vec_z = { 0.0, 0.0, 0.0 };
	Vector3D_t vec_e = { 0.0, 0.0, 0.0 };

	do {
		//dot product
		vec_z.x += matrix->row0[0] * vec_x.x;
		vec_z.x += matrix->row0[1] * vec_x.y;
		vec_z.x += matrix->row0[2] * vec_x.z;

		vec_z.y += matrix->row1[0] * vec_x.x;
		vec_z.y += matrix->row1[1] * vec_x.y;
		vec_z.y += matrix->row1[2] * vec_x.z;

		vec_z.z += matrix->row2[0] * vec_x.x;
		vec_z.z += matrix->row2[1] * vec_x.y;
		vec_z.z += matrix->row2[2] * vec_x.z;

		zmax = fabs(vec_z.x);
		if (fabs(vec_z.y) > zmax)
			zmax = fabs(vec_z.y);
		if (fabs(vec_z.z) > zmax)
			zmax = fabs(vec_z.z);

		vec_z.x /= zmax;
		vec_z.y /= zmax;
		vec_z.z /= zmax;

		vec_e.x = fabs(fabs(vec_z.x) - fabs(vec_x.x));
		vec_e.y = fabs(fabs(vec_z.y) - fabs(vec_x.y));
		vec_e.z = fabs(fabs(vec_z.z) - fabs(vec_x.z));

		emax = vec_e.x;
		if (vec_e.y > emax)
			emax = vec_e.y;
		if (vec_e.z > emax)
			emax = vec_e.z;

		vec_x.x = vec_z.x;
		vec_x.y = vec_z.y;
		vec_x.z = vec_z.z;

	} while (emax > EPSILON_2);

	eigen->eigenvector = vec_z;
	eigen->eigenvalue = zmax;
}

//mean
double math_mean(int length, double data, ...) {
	va_list mean_list;
	va_start(mean_list, data);
	double sum = data;
	for (int i = 0; i < length; i++) {
		sum += va_arg(mean_list, double);
	}
	va_end(mean_list);
	return sum / (double) length;
}

//standard deviation
double math_stddev(int length, double mean, double data, ...) {
	va_list stddev_list;
	va_start(stddev_list, data);
	double std = powf(data - mean, 2.0);
	for (int i = 0; i < length; i++) {
		double next_data = va_arg(stddev_list, double);
		std += powf(next_data - mean, 2.0);
	}
	va_end(stddev_list);
	return math_sqrt(std / (double) length);
}

// vector magnitude
float math_vec_mag(Vector3D_t *vec) {
	return math_sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
}
