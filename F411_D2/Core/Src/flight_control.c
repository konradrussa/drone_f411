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

//EDF 1400 + 21 - 2000
//ROTOR 4200 - 8400
//SERVO

AhrsState_t ahrsState;
AxesRaw_t accel, gyro, mag;

static volatile float heading = 0.0;

static volatile bool armed = false;

//input variables/channels
static FLIGHT_INPUT_t flight_input = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

// process in main for FC training
void flight_imu_calibration(const uint32_t last_tick, const uint32_t diff_us) {
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

	// AG: X North Y East Z Down, M: Y North X East Z Down NWD
	mag_data[0].AXIS_X = imu->mag_data[0].y;
	mag_data[0].AXIS_Y = imu->mag_data[0].x;
	mag_data[0].AXIS_Z = imu->mag_data[0].z;
	mag_data[1].AXIS_X = imu->mag_data[1].y;
	mag_data[1].AXIS_Y = imu->mag_data[1].x;
	mag_data[1].AXIS_Z = imu->mag_data[1].z;

	calibration(accel_data, gyro_data, mag_data);
}

// process in main
void flight_ahrs(const uint32_t last_tick, const uint32_t diff_us) {
	IMU_t *imu = get_imu();
	accel.AXIS_X = imu->accel_data[0].x;
	accel.AXIS_Y = imu->accel_data[0].y;
	accel.AXIS_Z = imu->accel_data[0].z;

	gyro.AXIS_X = imu->gyro_data[0].x;
	gyro.AXIS_Y = imu->gyro_data[0].y;
	gyro.AXIS_Z = imu->gyro_data[0].z;

	// AG: X North Y East Z Down, M: Y North X East Z Down NWD
	mag.AXIS_X = imu->mag_data[0].y;
	mag.AXIS_Y = imu->mag_data[0].x;
	mag.AXIS_Z = imu->mag_data[0].z;

	flight_set_parameters();
	ahrs_fusion_agm(&accel, &gyro, &mag, &ahrsState);
	quaternion_to_euler(&ahrsState.q, &ahrsState.ea);
	heading = ahrs_get_longitudinal_direction();
	flight_ukf(&accel, &gyro, &mag, diff_us);
}

//"4200;4200;4200;4200;1400;1400;0075;" arming
void flight_data_control(const uint32_t *radio_cmds, uint32_t *motor_cmds) {
	const RADIO_MAP_t *radio_limits = get_radio_map();
	flight_input.throttle = interpolate(
			(float) radio_cmds[0] / (float) (*radio_limits)[0][2]); // normalize between 0 and 1 and interpolate
	flight_input.pitch = -map((float) radio_cmds[1], (*radio_limits)[1][0],
			(*radio_limits)[1][2], -10, 10);
	flight_input.roll = -map((float) radio_cmds[2], (*radio_limits)[2][0],
			(*radio_limits)[2][2], -10, 10);
	flight_input.yaw = map((float) radio_cmds[3], (*radio_limits)[3][0],
			(*radio_limits)[3][2], -180, 180);
	flight_input.gear = map((float) radio_cmds[4], (*radio_limits)[4][0],
			(*radio_limits)[4][2], 2, 0);
	flight_input.speed = map((float) radio_cmds[5], (*radio_limits)[5][0],
			(*radio_limits)[5][2], 2, 0);

	if (!armed) {
		motor_cmds[0] = 4200;
		motor_cmds[1] = 4200;
		motor_cmds[2] = 4200;
		motor_cmds[3] = 4200;
		motor_cmds[4] = 1400;
		motor_cmds[5] = 1400;
		motor_cmds[6] = 75;
		armed = true;
	} else {
		motor_cmds[0] = radio_cmds[0];
		motor_cmds[1] = radio_cmds[1];
		motor_cmds[2] = radio_cmds[2];
		motor_cmds[3] = radio_cmds[3];
		motor_cmds[4] = radio_cmds[4];
		motor_cmds[5] = radio_cmds[5];
		motor_cmds[6] = 0;
	}
}
