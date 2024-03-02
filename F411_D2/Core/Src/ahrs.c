#include "ahrs.h"
#include "env.h"
#include "imu.h"

// Sampling time of sensors
const float SENSOR_SAMPLING_TIME = 0.00125; //800 Hz
int16_t gTHR;

extern float beta;
extern const float COE_DPS_TO_RADPS;
extern volatile float q0, q1, q2, q3;

const float ahrs_get_sampling_time() {
	return SENSOR_SAMPLING_TIME;
}

//Madgwick AHRS
void ahrs_fusion_agm(const AxesRaw_t *accel, const AxesRaw_t *gyro,
		const AxesRaw_t *mag, AhrsState_t *ahrs) {

	float axf, ayf, azf, gxf, gyf, gzf, mxf, myf, mzf;
	float norm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float hx, hy;
	float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1,
			_2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2,
			q1q3, q2q2, q2q3, q3q3;

	if ((mag->AXIS_X == 0.0f) && (mag->AXIS_Y == 0.0f)
			&& (mag->AXIS_Z == 0.0f)) {
		ahrs_fusion_ag(accel, gyro, ahrs);
		return;
	}

	axf = (float) accel->AXIS_X;
	ayf = (float) accel->AXIS_Y;
	azf = (float) accel->AXIS_Z;

	// gdps convert to rad/s
	gxf = ((float) gyro->AXIS_X) * ((float) COE_DPS_TO_RADPS);
	gyf = ((float) gyro->AXIS_Y) * ((float) COE_DPS_TO_RADPS);
	gzf = ((float) gyro->AXIS_Z) * ((float) COE_DPS_TO_RADPS);

	// AG: X North Y East Z Down, M: Y North X East Z Down NWD
	mxf = (float) mag->AXIS_X * ((float) COE_DPS_TO_RADPS);
	myf = (float) mag->AXIS_Y * ((float) COE_DPS_TO_RADPS);
	mzf = (float) mag->AXIS_Z * ((float) COE_DPS_TO_RADPS); // TODO check sign and rotation direction

	// Rate of change of quaternion from gyroscope
	qDot1 = (-q1 * gxf - q2 * gyf - q3 * gxf) * 0.5f;
	qDot2 = (q0 * gxf + q2 * gzf - q3 * gyf) * 0.5f;
	qDot3 = (q0 * gyf - q1 * gzf + q3 * gxf) * 0.5f;
	qDot4 = (q0 * gzf + q1 * gyf - q2 * gxf) * 0.5f;

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if (!((axf == 0.0f) && (ayf == 0.0f) && (azf == 0.0f))) {

		// Normalise accelerometer measurement
		norm = math_inv_sqrt(axf * axf + ayf * ayf + azf * azf);
		axf *= norm;
		ayf *= norm;
		azf *= norm;

		// Normalise magnetometer measurement
		norm = math_inv_sqrt(mxf * mxf + myf * myf + mzf * mzf);
		mxf *= norm;
		myf *= norm;
		mzf *= norm;

		// Auxiliary variables to avoid repeated arithmetic
		_2q0mx = 2.0f * q0 * mxf;
		_2q0my = 2.0f * q0 * myf;
		_2q0mz = 2.0f * q0 * mzf;
		_2q1mx = 2.0f * q1 * mxf;
		_2q0 = 2.0f * q0;
		_2q1 = 2.0f * q1;
		_2q2 = 2.0f * q2;
		_2q3 = 2.0f * q3;
		_2q0q2 = 2.0f * q0 * q2;
		_2q2q3 = 2.0f * q2 * q3;
		q0q0 = q0 * q0;
		q0q1 = q0 * q1;
		q0q2 = q0 * q2;
		q0q3 = q0 * q3;
		q1q1 = q1 * q1;
		q1q2 = q1 * q2;
		q1q3 = q1 * q3;
		q2q2 = q2 * q2;
		q2q3 = q2 * q3;
		q3q3 = q3 * q3;

		// Reference direction of Earth's magnetic field
		hx = mxf * q0q0 - _2q0my * q3 + _2q0mz * q2 + mxf * q1q1
				+ _2q1 * myf * q2 + _2q1 * mzf * q3 - mxf * q2q2 - mxf * q3q3;
		hy = _2q0mx * q3 + myf * q0q0 - _2q0mz * q1 + _2q1mx * q2 - myf * q1q1
				+ myf * q2q2 + _2q2 * mzf * q3 - myf * q3q3;
		_2bx = math_sqrt(hx * hx + hy * hy);
		_2bz = -_2q0mx * q2 + _2q0my * q1 + mzf * q0q0 + _2q1mx * q3
				- mzf * q1q1 + _2q2 * myf * q3 - mzf * q2q2 + mzf * q3q3;
		_4bx = 2.0f * _2bx;
		_4bz = 2.0f * _2bz;

		// Gradient decent algorithm corrective step
		s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - axf)
				+ _2q1 * (2.0f * q0q1 + _2q2q3 - ayf)
				- _2bz * q2
						* (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2)
								- axf)
				+ (-_2bx * q3 + _2bz * q1)
						* (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - ayf)
				+ _2bx * q2
						* (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2)
								- azf);
		s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - axf)
				+ _2q0 * (2.0f * q0q1 + _2q2q3 - ayf)
				- 4.0f * q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - axf)
				+ _2bz * q3
						* (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2)
								- mxf)
				+ (_2bx * q2 + _2bz * q0)
						* (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - myf)
				+ (_2bx * q3 - _4bz * q1)
						* (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2)
								- mzf);
		s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - axf)
				+ _2q3 * (2.0f * q0q1 + _2q2q3 - ayf)
				- 4.0f * q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - azf)
				+ (-_4bx * q2 - _2bz * q0)
						* (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2)
								- mxf)
				+ (_2bx * q1 + _2bz * q3)
						* (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - myf)
				+ (_2bx * q0 - _4bz * q2)
						* (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2)
								- mzf);
		s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - axf)
				+ _2q2 * (2.0f * q0q1 + _2q2q3 - ayf)
				+ (-_4bx * q3 + _2bz * q1)
						* (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2)
								- mxf)
				+ (-_2bx * q0 + _2bz * q2)
						* (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - myf)
				+ _2bx * q1
						* (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2)
								- mzf);
		norm = math_inv_sqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
		s0 *= norm;
		s1 *= norm;
		s2 *= norm;
		s3 *= norm;

		// Apply feedback step
		qDot1 -= beta * s0;
		qDot2 -= beta * s1;
		qDot3 -= beta * s2;
		qDot4 -= beta * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	q0 += qDot1 * SENSOR_SAMPLING_TIME;
	q1 += qDot2 * SENSOR_SAMPLING_TIME;
	q2 += qDot3 * SENSOR_SAMPLING_TIME;
	q3 += qDot4 * SENSOR_SAMPLING_TIME;

	// Normalise quaternion
	norm = math_inv_sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= norm;
	q1 *= norm;
	q2 *= norm;
	q3 *= norm;

	ahrs->q.q0 = q0;
	ahrs->q.q1 = q1;
	ahrs->q.q2 = q2;
	ahrs->q.q3 = q3;
}

//TODO include Z rotation
float ahrs_get_longitudinal_direction() {
	IMU_t *imu = get_imu();
	const float *delta_m = get_magnetic_declination();
	float longi_degree = atan2f(imu->mag_data[0].y - *delta_m, imu->mag_data[0].x - *delta_m); // tan(y/x) - *delta_m
	if (longi_degree < 0.0) {
		longi_degree += 2.0 * MAX_RAD;
	} else if (longi_degree > 2.0 * MAX_RAD) {
		longi_degree -= 2.0 * MAX_RAD;
	}
	return longi_degree * 180.0 / MAX_RAD;
}
