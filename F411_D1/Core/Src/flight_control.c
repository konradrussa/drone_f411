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
static uint32_t m_l1_s = 0.0, m_l2_s = 0.0, m_r1_s = 0.0, m_r2_s = 0.0,
		m_edf_l1_s = 0.0, m_edf_r1_s = 0.0, servo_s = 0.0;

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
	get_ukf_filter()->dt = diff_us * us_to_second(); // us to second
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

void flight_data_control(const uint32_t *radio_channels, const uint32_t *motors_pwm) {
	m_l1_s = motors_pwm[0];
	m_l2_s = motors_pwm[1];
	m_r1_s = motors_pwm[2];
	m_r2_s = motors_pwm[3];
	m_edf_l1_s = motors_pwm[4];
	m_edf_r1_s = motors_pwm[5];
	servo_s = motors_pwm[6];
	//TODO set to motors
}

void flight_recovery() {
	// TODO radio + ahrs + motor control
}
