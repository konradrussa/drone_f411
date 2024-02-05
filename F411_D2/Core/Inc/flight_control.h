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

void flight_imu_calibration(bool use_magnetometer);
void flight_ahrs(void);

#endif /* INC_FLIGHT_CONTROL_H_ */
