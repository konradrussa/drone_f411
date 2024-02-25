/*
 * flight_control.c
 *
 *  Created on: Dec 21, 2023
 *      Author: konrad
 */
#include "flight_control_common.h"
#include "flight_control.h"
#include "flight_estimation.h"

AhrsState_t ahrsState;
AxesRaw_t accel, gyro;

void flight_imu_calibration() {
	IMU_t *imu = get_imu();
	AxesRaw_t accel_data[2], gyro_data[2];
	accel_data[0].AXIS_X = imu->accel_data[0].x;
	accel_data[0].AXIS_Y = imu->accel_data[0].y;
	accel_data[0].AXIS_Z = imu->accel_data[0].z;
	accel_data[1].AXIS_X = imu->accel_data[1].x;
	accel_data[1].AXIS_Y = imu->accel_data[1].y;
	accel_data[1].AXIS_Z = imu->accel_data[1].z;

	gyro_data[0].AXIS_X = imu->gyro_data[0].x;
	gyro_data[0].AXIS_Y = imu->gyro_data[0].y;
	gyro_data[0].AXIS_Z = imu->gyro_data[0].z;
	gyro_data[1].AXIS_X = imu->gyro_data[1].x;
	gyro_data[1].AXIS_Y = imu->gyro_data[1].y;
	gyro_data[1].AXIS_Z = imu->gyro_data[1].z;

	calibration(accel_data, gyro_data, NULL);
	calculate_imu_noise(accel_data, gyro_data, NULL);
	flight_ukf(accel_data, gyro_data);
}

void flight_ahrs() {
	IMU_t *imu = get_imu();
	accel.AXIS_X = imu->accel_data[0].x;
	accel.AXIS_Y = imu->accel_data[0].y;
	accel.AXIS_Z = imu->accel_data[0].z;

	gyro.AXIS_X = imu->gyro_data[0].x;
	gyro.AXIS_Y = imu->gyro_data[0].y;
	gyro.AXIS_Z = imu->gyro_data[0].z;

	flight_set_parameters();
	ahrs_fusion_ag(&accel, &gyro, &ahrsState);
	quaternion_to_euler(&ahrsState.q, &ahrsState.ea);
}

void flight_recovery() {
}
