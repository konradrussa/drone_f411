/*
 * flight_control.c
 *
 *  Created on: Dec 21, 2023
 *      Author: konrad
 */
#include "flight_control.h"
#include "flight_control_common.h"
#include "flight_estimation.h"
#include "flight_estimation_common.h"

AhrsState_t ahrsState;
AxesRaw_t accel, gyro;

//input variables/motors
// L1, L2, R1, R2, EDF_L1, EDF_R1, SERVO
static FLIGHT_INPUT_t flight_input = { 0, 0, 0, 0, 0, 0, 0 };

void flight_radio_calibration() {
	calibration_radio();
}

void flight_imu_calibration(const uint32_t last_tick, const uint32_t diff_us) {
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
}

void flight_ahrs(const uint32_t last_tick, const uint32_t diff_us) {
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
	flight_ukf(&accel, &gyro, diff_us);
}

void flight_data_control(const uint32_t *radio_cmds, const uint32_t *motor_cmds) {
	flight_input.m_l1 = motor_cmds[0];
	flight_input.m_l2 = motor_cmds[1];
	flight_input.m_r1 = motor_cmds[2];
	flight_input.m_r2 = motor_cmds[3];
	flight_input.m_edf_l1 = motor_cmds[4];
	flight_input.m_edf_r1 = motor_cmds[5];
	flight_input.servo = motor_cmds[6];
	//TODO set to motors
}

void flight_recovery(const uint32_t last_tick, const uint32_t diff_us) {
	// TODO radio + ahrs + motor control
}
