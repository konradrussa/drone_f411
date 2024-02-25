/*
 * flight_estimation.h
 *
 *  Created on: Feb 25, 2024
 *      Author: konrad
 */

#ifndef SRC_FLIGHT_ESTIMATION_H_
#define SRC_FLIGHT_ESTIMATION_H_

#include "basic_math.h"
#include "ahrs_common.h"

void flight_ukf(const AxesRaw_t *accel, const AxesRaw_t *gyro,
		const AxesRaw_t *magnet);

#endif /* SRC_FLIGHT_ESTIMATION_H_ */
