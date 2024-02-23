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

//variance
double math_variance(int length, double mean, double data, ...) {
	va_list stddev_list;
	va_start(stddev_list, data);
	double variance = powf(data - mean, 2.0);
	for (int i = 0; i < length; i++) {
		double next_data = va_arg(stddev_list, double);
		variance += powf(next_data - mean, 2.0);
	}
	va_end(stddev_list);
	return variance / (double) length;
}

//standard deviation
double math_stddev(int length, double variance) {
	return math_sqrt(variance / (double) length);
}

// vector magnitude
float math_vec_mag(Vector3D_t *vec) {
	return math_sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
}
