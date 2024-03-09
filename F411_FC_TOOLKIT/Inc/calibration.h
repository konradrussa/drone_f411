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
#include "bridge_common.h"

#define SAMPLE_SIZE		(50)

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

typedef struct Measurement_3D {
	float data_x[SAMPLE_SIZE];
	float data_y[SAMPLE_SIZE];
	float data_z[SAMPLE_SIZE];

	float mean_x;
	float mean_y;
	float mean_z;

	float variance_x;
	float variance_y;
	float variance_z;

	float stddev_x;
	float stddev_y;
	float stddev_z;

	float cov_x_y;
	float cov_x_z;
	float cov_y_z;

	float corr_x_y;
	float corr_x_z;
	float corr_y_z;

} Measurement_3D_t;

typedef struct IMU_Calibration_3D {
	Calibration_3D_t accel;
	Calibration_3D_t gyro;
	Calibration_3D_t mag;
} IMU_Calibration_3D_t;

typedef struct RADIO_Calibration {
	int16_t throttle_max;
	int16_t throttle_min;
	int16_t pitch_max;
	int16_t pitch_min;
	int16_t roll_max;
	int16_t roll_min;
	int16_t yaw_max;
	int16_t yaw_min;
	int16_t gear_max;
	int16_t gear_min;
	int16_t speed_max;
	int16_t speed_min;
} RADIO_Calibration_t;

typedef struct IMU_Measurement_3D {
	Measurement_3D_t accel;
	Measurement_3D_t gyro;
	Measurement_3D_t mag;
} IMU_Measurement_3D_t;

void calibration(const AxesRaw_t *accel_data, const AxesRaw_t *gyro_data,
		const AxesRaw_t *mag_data);

void radio_calibration(void);

#endif // CALIBRATION_H
