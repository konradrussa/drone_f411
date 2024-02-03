/*
 * calibration.c
 *
 *  Created on: Dec 21, 2023
 *      Author: konrad
 */

#include "calibration.h"

IMU_Calibration_3D_t imuCalibration;
Noise_IMU_t imu_noise = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
Noise_GPS_t gps_noise = { 0.0, 0.0 };
volatile bool prev_data_exists = false;

static void calculateMaxima() {
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

void calibration() {
	IMU_t *imu = get_imu();
	int16_t accel_AXIS_X = imu->accel_data[0].x;
	imuCalibration.accel_calib.max_x = math_max(accel_AXIS_X,
			imuCalibration.accel_calib.max_x);
	imuCalibration.accel_calib.min_x = math_min(accel_AXIS_X,
			imuCalibration.accel_calib.min_x);

	int16_t accel_AXIS_Y = imu->accel_data[0].y;
	imuCalibration.accel_calib.max_y = math_max(accel_AXIS_Y,
			imuCalibration.accel_calib.max_y);
	imuCalibration.accel_calib.min_y = math_min(accel_AXIS_Y,
			imuCalibration.accel_calib.min_y);

	int16_t accel_AXIS_Z = imu->accel_data[0].z;
	imuCalibration.accel_calib.max_z = math_max(accel_AXIS_Z,
			imuCalibration.accel_calib.max_z);
	imuCalibration.accel_calib.min_z = math_min(accel_AXIS_Z,
			imuCalibration.accel_calib.min_z);

	int16_t gyro_AXIS_X = imu->gyro_data[0].x;
	imuCalibration.gyro_calib.max_x = math_max(gyro_AXIS_X,
			imuCalibration.gyro_calib.max_x);
	imuCalibration.gyro_calib.min_x = math_min(gyro_AXIS_X,
			imuCalibration.gyro_calib.min_x);

	int16_t gyro_AXIS_Y = imu->gyro_data[0].y;
	imuCalibration.gyro_calib.max_y = math_max(gyro_AXIS_Y,
			imuCalibration.gyro_calib.max_y);
	imuCalibration.gyro_calib.min_y = math_min(gyro_AXIS_Y,
			imuCalibration.gyro_calib.min_y);

	int16_t gyro_AXIS_Z = imu->gyro_data[0].z;
	imuCalibration.gyro_calib.max_z = math_max(gyro_AXIS_Z,
			imuCalibration.gyro_calib.max_z);
	imuCalibration.gyro_calib.min_z = math_min(gyro_AXIS_Z,
			imuCalibration.gyro_calib.min_z);

	int16_t mag_AXIS_X = imu->mag_data[0].x;
	imuCalibration.mag_calib.max_x = math_max(mag_AXIS_X,
			imuCalibration.mag_calib.max_x);
	imuCalibration.mag_calib.min_x = math_min(mag_AXIS_X,
			imuCalibration.mag_calib.min_x);

	int16_t mag_AXIS_Y = imu->mag_data[0].y;
	imuCalibration.mag_calib.max_y = math_max(mag_AXIS_Y,
			imuCalibration.mag_calib.max_y);
	imuCalibration.mag_calib.min_y = math_min(mag_AXIS_Y,
			imuCalibration.mag_calib.min_y);

	int16_t mag_AXIS_Z = imu->mag_data[0].z;
	imuCalibration.mag_calib.max_z = math_max(mag_AXIS_Z,
			imuCalibration.mag_calib.max_z);
	imuCalibration.mag_calib.min_z = math_min(mag_AXIS_Z,
			imuCalibration.mag_calib.min_z);

	calculateMaxima();
}

void calculate_imu_noise() {
	if (prev_data_exists) {
		IMU_t *imu = get_imu();
		//gather every tick measurement + cumulative drift
		imu_noise.n_a_x += imu->accel_data[1].x - imu->accel_data[0].x;
		imu_noise.n_a_y += imu->accel_data[1].y - imu->accel_data[0].y;
		imu_noise.n_a_z += imu->accel_data[1].z - imu->accel_data[0].z;
		imu_noise.n_g_x += imu->gyro_data[1].x - imu->gyro_data[0].x;
		imu_noise.n_g_y += imu->gyro_data[1].y - imu->gyro_data[0].y;
		imu_noise.n_g_z += imu->gyro_data[1].z - imu->gyro_data[0].z;
		imu_noise.n_m_x += imu->mag_data[1].x - imu->mag_data[0].x;
		imu_noise.n_m_y += imu->mag_data[1].y - imu->mag_data[0].y;
		imu_noise.n_m_z += imu->mag_data[1].z - imu->mag_data[0].z;
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
