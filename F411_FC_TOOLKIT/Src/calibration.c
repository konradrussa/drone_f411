/*
 * calibration.c
 *
 *  Created on: Feb 5, 2024
 *      Author: konrad
 */

#include "calibration.h"

IMU_Calibration_3D_t imu_calibration = { { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0 }, { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, { 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } };
Noise_IMU_t imu_noise = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
Noise_GPS_t gps_noise = { 0.0, 0.0 };

static void calculateMaxima(bool use_magnetometer) {
	imu_calibration.accel_calib.abs_x = math_max(
			math_abs(imu_calibration.accel_calib.max_x),
			math_abs(imu_calibration.accel_calib.min_x));
	imu_calibration.accel_calib.abs_y = math_max(
			math_abs(imu_calibration.accel_calib.max_y),
			math_abs(imu_calibration.accel_calib.min_y));
	imu_calibration.accel_calib.abs_z = math_max(
			math_abs(imu_calibration.accel_calib.max_z),
			math_abs(imu_calibration.accel_calib.min_z));

	imu_calibration.gyro_calib.abs_x = math_max(
			math_abs(imu_calibration.gyro_calib.max_x),
			math_abs(imu_calibration.gyro_calib.min_x));
	imu_calibration.gyro_calib.abs_y = math_max(
			math_abs(imu_calibration.gyro_calib.max_y),
			math_abs(imu_calibration.gyro_calib.min_y));
	imu_calibration.gyro_calib.abs_z = math_max(
			math_abs(imu_calibration.gyro_calib.max_z),
			math_abs(imu_calibration.gyro_calib.min_z));

	if (use_magnetometer) {
		imu_calibration.mag_calib.abs_x = math_max(
				math_abs(imu_calibration.mag_calib.max_x),
				math_abs(imu_calibration.mag_calib.min_x));
		imu_calibration.mag_calib.abs_y = math_max(
				math_abs(imu_calibration.mag_calib.max_y),
				math_abs(imu_calibration.mag_calib.min_y));
		imu_calibration.mag_calib.abs_z = math_max(
				math_abs(imu_calibration.mag_calib.max_z),
				math_abs(imu_calibration.mag_calib.min_z));
	}
}

void calibration(const AxesRaw_t *accel_data, const AxesRaw_t *gyro_data,
		const AxesRaw_t *mag_data, bool use_magnetometer) {
	int16_t accel_AXIS_X = accel_data->AXIS_X;
	imu_calibration.accel_calib.max_x = math_max(accel_AXIS_X,
			imu_calibration.accel_calib.max_x);
	imu_calibration.accel_calib.min_x = math_min(accel_AXIS_X,
			imu_calibration.accel_calib.min_x);

	int16_t accel_AXIS_Y = accel_data->AXIS_Y;
	imu_calibration.accel_calib.max_y = math_max(accel_AXIS_Y,
			imu_calibration.accel_calib.max_y);
	imu_calibration.accel_calib.min_y = math_min(accel_AXIS_Y,
			imu_calibration.accel_calib.min_y);

	int16_t accel_AXIS_Z = accel_data->AXIS_Z;
	imu_calibration.accel_calib.max_z = math_max(accel_AXIS_Z,
			imu_calibration.accel_calib.max_z);
	imu_calibration.accel_calib.min_z = math_min(accel_AXIS_Z,
			imu_calibration.accel_calib.min_z);

	int16_t gyro_AXIS_X = gyro_data->AXIS_X;
	imu_calibration.gyro_calib.max_x = math_max(gyro_AXIS_X,
			imu_calibration.gyro_calib.max_x);
	imu_calibration.gyro_calib.min_x = math_min(gyro_AXIS_X,
			imu_calibration.gyro_calib.min_x);

	int16_t gyro_AXIS_Y = gyro_data->AXIS_Y;
	imu_calibration.gyro_calib.max_y = math_max(gyro_AXIS_Y,
			imu_calibration.gyro_calib.max_y);
	imu_calibration.gyro_calib.min_y = math_min(gyro_AXIS_Y,
			imu_calibration.gyro_calib.min_y);

	int16_t gyro_AXIS_Z = gyro_data->AXIS_Z;
	imu_calibration.gyro_calib.max_z = math_max(gyro_AXIS_Z,
			imu_calibration.gyro_calib.max_z);
	imu_calibration.gyro_calib.min_z = math_min(gyro_AXIS_Z,
			imu_calibration.gyro_calib.min_z);

	int16_t mag_AXIS_X = mag_data->AXIS_X;
	imu_calibration.mag_calib.max_x = math_max(mag_AXIS_X,
			imu_calibration.mag_calib.max_x);
	imu_calibration.mag_calib.min_x = math_min(mag_AXIS_X,
			imu_calibration.mag_calib.min_x);

	int16_t mag_AXIS_Y = mag_data->AXIS_Y;
	imu_calibration.mag_calib.max_y = math_max(mag_AXIS_Y,
			imu_calibration.mag_calib.max_y);
	imu_calibration.mag_calib.min_y = math_min(mag_AXIS_Y,
			imu_calibration.mag_calib.min_y);

	int16_t mag_AXIS_Z = mag_data->AXIS_Z;
	imu_calibration.mag_calib.max_z = math_max(mag_AXIS_Z,
			imu_calibration.mag_calib.max_z);
	imu_calibration.mag_calib.min_z = math_min(mag_AXIS_Z,
			imu_calibration.mag_calib.min_z);

	calculateMaxima(use_magnetometer);
}

void calculate_imu_noise(const AxesRaw_t *accel_data,
		const AxesRaw_t *gyro_data, const AxesRaw_t *mag_data,
		bool use_magnetometer) {
	//gather every tick measurement + cumulative drift
	imu_noise.n_da_x = (accel_data + 1)->AXIS_X - accel_data->AXIS_X;
	imu_noise.n_da_y = (accel_data + 1)->AXIS_Y - accel_data->AXIS_Y;
	imu_noise.n_da_z = (accel_data + 1)->AXIS_Z - accel_data->AXIS_Z;
	imu_noise.n_dg_x = (gyro_data + 1)->AXIS_X - gyro_data->AXIS_X;
	imu_noise.n_dg_y = (gyro_data + 1)->AXIS_Y - gyro_data->AXIS_Y;
	imu_noise.n_dg_z = (gyro_data + 1)->AXIS_Z - gyro_data->AXIS_Z;

	imu_noise.n_ia_x += imu_noise.n_da_x;
	imu_noise.n_ia_y += imu_noise.n_da_y;
	imu_noise.n_ia_z += imu_noise.n_da_z;
	imu_noise.n_ig_x += imu_noise.n_dg_x;
	imu_noise.n_ig_y += imu_noise.n_dg_y;
	imu_noise.n_ig_z += imu_noise.n_dg_z;

	if (use_magnetometer) {
		imu_noise.n_dm_x = (mag_data + 1)->AXIS_X - mag_data->AXIS_X;
		imu_noise.n_dm_y = (mag_data + 1)->AXIS_Y - mag_data->AXIS_Y;
		imu_noise.n_dm_z = (mag_data + 1)->AXIS_Z - mag_data->AXIS_Z;

		imu_noise.n_im_x += imu_noise.n_dm_x;
		imu_noise.n_im_y += imu_noise.n_dm_y;
		imu_noise.n_im_z += imu_noise.n_dm_z;
	}
}

void calculate_imu_stddev() {
	//math_stddev(length, mean, data ...) * math.sqrt(ahrs_get_sampling_time()) // TODO check model /sqrt(Hz)
}

/*
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
 */

void calculate_gps_noise() {

}

void calculate_gps_stddev() {

}

