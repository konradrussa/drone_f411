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
#include "remote_control.h"

typedef struct FLIGHT_INPUT {
	uint32_t m_l1;
	uint32_t m_l2;
	uint32_t m_r1;
	uint32_t m_r2;
	uint32_t m_edf_l1;
	uint32_t m_edf_r1;
	uint32_t servo;
} FLIGHT_INPUT_t;

void flight_radio_calibration(void);
void flight_imu_calibration(const uint32_t last_tick, const uint32_t diff_us);
void flight_ahrs(void);
void flight_recovery(void);
void flight_data_control(const uint32_t *radio_cmds, const uint32_t *motor_cmds);

#endif /* INC_FLIGHT_CONTROL_H_ */
