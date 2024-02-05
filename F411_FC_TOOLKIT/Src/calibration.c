/*
 * calibration.c
 *
 *  Created on: Feb 5, 2024
 *      Author: konrad
 */

#include "timer.h"
#include "calibration.h"

IMU_Calibration_3D_t imuCalibration;
Noise_IMU_t imu_noise = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
Noise_GPS_t gps_noise = { 0.0, 0.0 };
volatile bool prev_data_exists = false;

volatile uint32_t calib_tick = 0, calib_rtc = 0;

static void calculateMaxima(bool use_magnetometer) {
	imuCalibration.accel_calib.abs_x = math_max(
			math_abs(imuCalibration.accel_calib.max_x),
			math_abs(imuCalibration.accel_calib.min_x));
	imuCalibration.accel_calib.abs_y = math_max(
			math_abs(imuCalibration.accel_calib.max_y),
			math_abs(imuCalibration.accel_calib.min_y));
	imuCalibration.accel_calib.abs_z = math_max(
			math_abs(imuCalibration.accel_calib.max_z),
			math_abs(imuCalibration.accel_calib.min_z));

	imuCalibration.gyro_calib.abs_x = math_max(
			math_abs(imuCalibration.gyro_calib.max_x),
			math_abs(imuCalibration.gyro_calib.min_x));
	imuCalibration.gyro_calib.abs_y = math_max(
			math_abs(imuCalibration.gyro_calib.max_y),
			math_abs(imuCalibration.gyro_calib.min_y));
	imuCalibration.gyro_calib.abs_z = math_max(
			math_abs(imuCalibration.gyro_calib.max_z),
			math_abs(imuCalibration.gyro_calib.min_z));

	if (use_magnetometer) {
		imuCalibration.mag_calib.abs_x = math_max(
				math_abs(imuCalibration.mag_calib.max_x),
				math_abs(imuCalibration.mag_calib.min_x));
		imuCalibration.mag_calib.abs_y = math_max(
				math_abs(imuCalibration.mag_calib.max_y),
				math_abs(imuCalibration.mag_calib.min_y));
		imuCalibration.mag_calib.abs_z = math_max(
				math_abs(imuCalibration.mag_calib.max_z),
				math_abs(imuCalibration.mag_calib.min_z));
	}
}

void calibration(const AxesRaw_t *accel_data, const AxesRaw_t *gyro_data,
		const AxesRaw_t *mag_data, bool use_magnetometer) {
	int16_t accel_AXIS_X = accel_data->AXIS_X;
	imuCalibration.accel_calib.max_x = math_max(accel_AXIS_X,
			imuCalibration.accel_calib.max_x);
	imuCalibration.accel_calib.min_x = math_min(accel_AXIS_X,
			imuCalibration.accel_calib.min_x);

	int16_t accel_AXIS_Y = accel_data->AXIS_Y;
	imuCalibration.accel_calib.max_y = math_max(accel_AXIS_Y,
			imuCalibration.accel_calib.max_y);
	imuCalibration.accel_calib.min_y = math_min(accel_AXIS_Y,
			imuCalibration.accel_calib.min_y);

	int16_t accel_AXIS_Z = accel_data->AXIS_Z;
	imuCalibration.accel_calib.max_z = math_max(accel_AXIS_Z,
			imuCalibration.accel_calib.max_z);
	imuCalibration.accel_calib.min_z = math_min(accel_AXIS_Z,
			imuCalibration.accel_calib.min_z);

	int16_t gyro_AXIS_X = gyro_data->AXIS_X;
	imuCalibration.gyro_calib.max_x = math_max(gyro_AXIS_X,
			imuCalibration.gyro_calib.max_x);
	imuCalibration.gyro_calib.min_x = math_min(gyro_AXIS_X,
			imuCalibration.gyro_calib.min_x);

	int16_t gyro_AXIS_Y = gyro_data->AXIS_Y;
	imuCalibration.gyro_calib.max_y = math_max(gyro_AXIS_Y,
			imuCalibration.gyro_calib.max_y);
	imuCalibration.gyro_calib.min_y = math_min(gyro_AXIS_Y,
			imuCalibration.gyro_calib.min_y);

	int16_t gyro_AXIS_Z = gyro_data->AXIS_Z;
	imuCalibration.gyro_calib.max_z = math_max(gyro_AXIS_Z,
			imuCalibration.gyro_calib.max_z);
	imuCalibration.gyro_calib.min_z = math_min(gyro_AXIS_Z,
			imuCalibration.gyro_calib.min_z);

	int16_t mag_AXIS_X = mag_data->AXIS_X;
	imuCalibration.mag_calib.max_x = math_max(mag_AXIS_X,
			imuCalibration.mag_calib.max_x);
	imuCalibration.mag_calib.min_x = math_min(mag_AXIS_X,
			imuCalibration.mag_calib.min_x);

	int16_t mag_AXIS_Y = mag_data->AXIS_Y;
	imuCalibration.mag_calib.max_y = math_max(mag_AXIS_Y,
			imuCalibration.mag_calib.max_y);
	imuCalibration.mag_calib.min_y = math_min(mag_AXIS_Y,
			imuCalibration.mag_calib.min_y);

	int16_t mag_AXIS_Z = mag_data->AXIS_Z;
	imuCalibration.mag_calib.max_z = math_max(mag_AXIS_Z,
			imuCalibration.mag_calib.max_z);
	imuCalibration.mag_calib.min_z = math_min(mag_AXIS_Z,
			imuCalibration.mag_calib.min_z);

	calculateMaxima(use_magnetometer);
}

void calculate_imu_noise(const AxesRaw_t *accel_data,
		const AxesRaw_t *gyro_data, const AxesRaw_t *mag_data,
		bool use_magnetometer) {
	if (prev_data_exists) {
		//gather every tick measurement + cumulative drift
		calib_tick = timer_tim4_us();
		calib_rtc = timer_rtc_sec();
		imu_noise.n_a_x += (accel_data + 1)->AXIS_X - accel_data->AXIS_X;
		imu_noise.n_a_y += (accel_data + 1)->AXIS_Y - accel_data->AXIS_Y;
		imu_noise.n_a_z += (accel_data + 1)->AXIS_Z - accel_data->AXIS_Z;
		imu_noise.n_g_x += (gyro_data + 1)->AXIS_X - gyro_data->AXIS_X;
		imu_noise.n_g_y += (gyro_data + 1)->AXIS_Y - gyro_data->AXIS_Y;
		imu_noise.n_g_z += (gyro_data + 1)->AXIS_Z - gyro_data->AXIS_Z;
		if (use_magnetometer) {
			imu_noise.n_m_x += (mag_data + 1)->AXIS_X - mag_data->AXIS_X;
			imu_noise.n_m_y += (mag_data + 1)->AXIS_Y - mag_data->AXIS_Y;
			imu_noise.n_m_z += (mag_data + 1)->AXIS_Z - mag_data->AXIS_Z;
		}

	} else {
		prev_data_exists = true;
	}
}

void calculate_imu_stddev() {
	//math_stddev(length, mean, data ...) * math.sqrt(ahrs_get_sampling_time()) // TODO check model /sqrt(Hz)
}

void calculate_gps_noise() {

}

void calculate_gps_stddev() {

}

