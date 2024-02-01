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

inline bool math_rotation_matrix_in_range(int16_t det) {
	return (det <= 1 + EPSILON && det >= 1 - EPSILON); // determinant for Rotation Matrix should be 1 +-epsilon
}

int16_t math_rotation_matrix_determinant(Matrix3D_t *matrix) {
	return matrix->row0[0]
			* (matrix->row1[1] * matrix->row2[2]
					- matrix->row1[1] * matrix->row2[2])
			- matrix->row0[1]
					* (matrix->row1[0] * matrix->row2[2]
							- matrix->row2[0] * matrix->row2[2])
			+ matrix->row0[2]
					* (matrix->row1[0] * matrix->row1[1]
							- matrix->row2[0] * matrix->row2[1]);
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
