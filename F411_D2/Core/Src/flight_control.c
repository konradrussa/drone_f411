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
static float throttle_s = 0.0, roll_s = 0.0, pitch_s = 0.0, yaw_s = 0.0,
		gear_s = 0.0, speed_s = 0.0;

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

	get_ukf_filter()->dt = diff_us * us_to_second(); // us to second
	heading = ahrs_get_longitudinal_direction();
	flight_ukf(accel_data, gyro_data, mag_data);
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

	// AG: X North Y East Z Down, M: Y North X East Z Down NWD
	mag.AXIS_X = imu->mag_data[0].y;
	mag.AXIS_Y = imu->mag_data[0].x;
	mag.AXIS_Z = imu->mag_data[0].z;

	flight_set_parameters();
	ahrs_fusion_agm(&accel, &gyro, &mag, &ahrsState);
	quaternion_to_euler(&ahrsState.q, &ahrsState.ea);
}

//"4200;4200;4200;4200;1400;1400;0075;" arming
void flight_data_control(const uint32_t *radio_channels, uint32_t *motors_pwm) {
	throttle_s = ((float) radio_channels[0] / (float) get_rc_channel_max()); // normalize between 0 and 1
	throttle_s = interpolate(throttle_s);
	pitch_s = -map(radio_channels[1], get_rc_channel_min(),
			get_rc_channel_max(), -10, 10);
	roll_s = -map(radio_channels[2], get_rc_channel_min(), get_rc_channel_max(),
			-10, 10);
	yaw_s = map(radio_channels[3], get_rc_channel_min(), get_rc_channel_max(),
			-180, 180);
	gear_s = radio_channels[4];
	speed_s = radio_channels[5];

	if (!armed) {
		motors_pwm[0] = 4200;
		motors_pwm[1] = 4200;
		motors_pwm[2] = 4200;
		motors_pwm[3] = 4200;
		motors_pwm[4] = 1400;
		motors_pwm[5] = 1400;
		motors_pwm[6] = 75;
		armed = true;
	} else {
		motors_pwm[0] = 0;
		motors_pwm[1] = 0;
		motors_pwm[2] = 0;
		motors_pwm[3] = 0;
		motors_pwm[4] = 0;
		motors_pwm[5] = 0;
		motors_pwm[6] = 0;
	}
}
