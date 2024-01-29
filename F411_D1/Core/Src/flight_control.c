/*
 * flight_control.c
 *
 *  Created on: Dec 21, 2023
 *      Author: konrad
 */
#include "flight_control.h"

AhrsState_t ahrsState;
AxesRaw_t accel, gyro;

void flight_imu_calibration() {
	calibration();
}

void flight_ahrs() {
	IMU_t *imu = get_imu();
	accel.AXIS_X = imu->accel_data[0].x;
	accel.AXIS_Y = imu->accel_data[0].y;
	accel.AXIS_Z = imu->accel_data[0].z;

	gyro.AXIS_X = imu->gyro_data[0].x;
	gyro.AXIS_Y = imu->gyro_data[0].y;
	gyro.AXIS_Z = imu->gyro_data[0].z;

	ahrs_fusion_ag(&accel, &gyro, &ahrsState);
	quaternion_to_euler(&ahrsState.q, &ahrsState.ea);
}

void flight_recovery() {
}
