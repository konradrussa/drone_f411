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

// mean
float math_mean(int length, float *data) {
	//int length = sizeof(data) / sizeof(data[0]);
	float sum = 0.0;
	for (int i = 0; i < length; i++) {
		sum += data[i];
	}
	return sum / (float) length;
}

// variance
float math_variance(int length, float mean, float *data) {
	//int length = sizeof(data) / sizeof(data[0]);
	float variance = 0.0;
	for (int i = 0; i < length; i++) {
		variance += powf(data[i] - mean, 2.0);
	}
	return variance / (float) length;
}

// standard deviation
inline float math_stddev(float variance) {
	return math_sqrt(variance);
}

// covariance
float math_covariance(int length, float *x, float mean_x, float *y,
		float mean_y) {
	float covariance = 0.0;
	for (int i = 0; i < length; i++) {
		covariance += (x[i] - mean_x) * (y[i] - mean_y);
	}
	return covariance / (float) length;
}

// correlation
inline float math_correlation(float cov, float stddev_x, float stddev_y) {
	return cov / (stddev_x * stddev_y);
}

// probability density function of a normal distribution
inline float math_pdf_normal(float x, float mean_x, float stddev_x) {
	float exponent = -(powf(x - mean_x, 2.0) / (2 * powf(stddev_x, 2.0)));
	return expf(exponent) / (stddev_x * math_sqrt(2.0 * M_PI));
}

// vector magnitude
inline float math_vec_mag(Vector3D_t *vec) {
	return math_sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
}
