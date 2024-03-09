/*
 * flight_control.h
 *
 *  Created on: Dec 21, 2023
 *      Author: konrad
 */

#ifndef INC_FLIGHT_CONTROL_H_
#define INC_FLIGHT_CONTROL_H_

#include "imu.h"
#include "ahrs.h"
#include "drone.h"
#include "calibration.h"

typedef struct FLIGHT_INPUT {
	float throttle;
	float pitch;
	float roll;
	float yaw;
	float gear;
	float speed;
} FLIGHT_INPUT_t;

void flight_imu_calibration(const uint32_t last_tick, const uint32_t diff_us);
void flight_ahrs(const uint32_t last_tick, const uint32_t diff_us);
void flight_data_control(const uint32_t *radio_channels, uint32_t *motors_pwm);

#endif /* INC_FLIGHT_CONTROL_H_ */
