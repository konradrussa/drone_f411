/*
 * flight_estimation.c
 *
 *  Created on: Feb 25, 2024
 *      Author: konrad
 */

#include "flight_estimation.h"
#include "flight_estimation_common.h"

void flight_ukf(const AxesRaw_t *accel, const AxesRaw_t *gyro) {
	estimation_ukf(accel, gyro, NULL);
}
