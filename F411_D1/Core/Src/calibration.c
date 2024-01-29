/*
 * calibration.c
 *
 *  Created on: Dec 21, 2023
 *      Author: konrad
 */

#include "calibration.h"

IMU_Calibration_3D_t imuCalibration;

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

	calculateMaxima();
}
