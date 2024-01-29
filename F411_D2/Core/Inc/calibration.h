/*
 * calibration.h
 *
 *  Created on: Dec 21, 2023
 *      Author: konrad
 */
#ifndef CALIBRATION_H
#define CALIBRATION_H

#include "basic_math.h"
#include "imu.h"

typedef struct Calibration_3D {
	int16_t max_x;
	int16_t min_x;
	int16_t abs_x;
	int16_t max_y;
	int16_t min_y;
	int16_t abs_y;
	int16_t max_z;
	int16_t min_z;
	int16_t abs_z;
} Calibration_3D_t;

typedef struct IMU_Calibration_3D {
	Calibration_3D_t accel_calib;
	Calibration_3D_t gyro_calib;
	Calibration_3D_t mag_calib;
} IMU_Calibration_3D_t;

void calibration(void);

#endif // CALIBRATION_H
