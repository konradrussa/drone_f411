/*
 * flight_control.c
 *
 *  Created on: Dec 21, 2023
 *      Author: konrad
 */
#include "flight_control_common.h"
#include "flight_control.h"

//EDF 1400 + 21 - 2000
//ROTOR 4200 - 8400
//SERVO

AhrsState_t ahrsState;
AxesRaw_t accel, gyro, mag;

float compass_deg;

void flight_imu_calibration(bool use_magnetometer) {
	IMU_t *imu = get_imu();
	AxesRaw_t accel_data[2], gyro_data[2], mag_data[2];
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
	if (use_magnetometer) {
		mag_data[0].AXIS_X = imu->mag_data[0].x;
		mag_data[0].AXIS_Y = imu->mag_data[0].y;
		mag_data[0].AXIS_Z = imu->mag_data[0].z;
		mag_data[1].AXIS_X = imu->mag_data[1].x;
		mag_data[1].AXIS_Y = imu->mag_data[1].y;
		mag_data[1].AXIS_Z = imu->mag_data[1].z;
	}

	calibration(accel_data, gyro_data, mag_data, use_magnetometer);
}

// process in main
void flight_ahrs() {
	IMU_t *imu = get_imu();
	accel.AXIS_X = imu->accel_data[0].x;
	accel.AXIS_Y = imu->accel_data[0].y;
	accel.AXIS_Z = imu->accel_data[0].z;

	gyro.AXIS_X = imu->gyro_data[0].x;
	gyro.AXIS_Y = imu->gyro_data[0].y;
	gyro.AXIS_Z = imu->gyro_data[0].z;

	mag.AXIS_X = imu->mag_data[0].x;
	mag.AXIS_Y = imu->mag_data[0].y;
	mag.AXIS_Z = imu->mag_data[0].z;

	drone_queue_control();
	compass_deg = ahrs_get_longitudinal_direction();
	flight_set_parameters();
	ahrs_fusion_agm(&accel, &gyro, &mag, &ahrsState);
	quaternion_to_euler(&ahrsState.q, &ahrsState.ea);
}
