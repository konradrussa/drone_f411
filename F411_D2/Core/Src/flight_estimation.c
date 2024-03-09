/*
 * flight_estimation.c
 *
 *  Created on: Feb 25, 2024
 *      Author: konrad
 */

#include "flight_estimation.h"
#include "flight_estimation_common.h"

void flight_ukf(const AxesRaw_t *accel, const AxesRaw_t *gyro,
		const AxesRaw_t *magnet, const float diff_us) {
	estimation_ukf(accel, gyro, magnet, diff_us);
}
