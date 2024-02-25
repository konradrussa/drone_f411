/*
 * calibration.h
 *
 *  Created on: Dec 21, 2023
 *      Author: konrad
 */
#ifndef CALIBRATION_H
#define CALIBRATION_H

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
	float n_da_x;
	float n_da_y;
	float n_da_z;
	float n_dg_x;
	float n_dg_y;
	float n_dg_z;
	float n_dm_x;
	float n_dm_y;
	float n_dm_z;

	float n_ia_x;
	float n_ia_y;
	float n_ia_z;
	float n_ig_x;
	float n_ig_y;
	float n_ig_z;
	float n_im_x;
	float n_im_y;
	float n_im_z;
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
		const AxesRaw_t *mag_data);
void calculate_imu_noise(const AxesRaw_t *accel_data,
		const AxesRaw_t *gyro_data, const AxesRaw_t *mag_data);
void calculate_imu_stddev(void);
void calculate_gps_noise(void);
void calculate_gps_stddev(void);

#endif // CALIBRATION_H
