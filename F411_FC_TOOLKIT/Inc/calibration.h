/*
 * calibration.h
 *
 *  Created on: Dec 21, 2023
 *      Author: konrad
 */
#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <stdbool.h>
#include "basic_math.h"
#include "ahrs_common.h"

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

typedef struct Noise_IMU {
	float n_a_x;
	float n_a_y;
	float n_a_z;
	float n_g_x;
	float n_g_y;
	float n_g_z;

	float n_m_x;
	float n_m_y;
	float n_m_z;
} Noise_IMU_t;

typedef struct Noise_GPS {
	float lat;
	float lon;
} Noise_GPS_t;

typedef struct IMU_Calibration_3D {
	Calibration_3D_t accel_calib;
	Calibration_3D_t gyro_calib;
	Calibration_3D_t mag_calib;
} IMU_Calibration_3D_t;

void calibration(const AxesRaw_t *accel_data, const AxesRaw_t *gyro_data,
		const AxesRaw_t *mag_data,
		bool use_magnetometer);
void calculate_imu_noise(const AxesRaw_t *accel_data,
		const AxesRaw_t *gyro_data, const AxesRaw_t *mag_data,
		bool use_magnetometer);
void calculate_imu_stddev(void);
void calculate_gps_noise(void);
void calculate_gps_stddev(void);

#endif // CALIBRATION_H
