/*
 * calibration.c
 *
 *  Created on: Feb 5, 2024
 *      Author: konrad
 */

#include "calibration.h"

IMU_Measurement_3D_t imu_measurement;

IMU_Calibration_3D_t imu_calibration = { { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0 }, { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }, { 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } };
Noise_IMU_t imu_noise = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
Noise_GPS_t gps_noise = { 0.0, 0.0 };

static int iteration = 0;

//static void calculate_imu_stddev() {
//	//math_stddev(length, mean, data ...) * math.sqrt(ahrs_get_sampling_time()) // TODO check model /sqrt(Hz)
//}
//
//static void calculate_gps_stddev() {
//
//}

static void calculate_maxima(bool use_magnetometer) {
	imu_calibration.accel.abs_x = math_max(
			math_abs(imu_calibration.accel.max_x),
			math_abs(imu_calibration.accel.min_x));
	imu_calibration.accel.abs_y = math_max(
			math_abs(imu_calibration.accel.max_y),
			math_abs(imu_calibration.accel.min_y));
	imu_calibration.accel.abs_z = math_max(
			math_abs(imu_calibration.accel.max_z),
			math_abs(imu_calibration.accel.min_z));

	imu_calibration.gyro.abs_x = math_max(math_abs(imu_calibration.gyro.max_x),
			math_abs(imu_calibration.gyro.min_x));
	imu_calibration.gyro.abs_y = math_max(math_abs(imu_calibration.gyro.max_y),
			math_abs(imu_calibration.gyro.min_y));
	imu_calibration.gyro.abs_z = math_max(math_abs(imu_calibration.gyro.max_z),
			math_abs(imu_calibration.gyro.min_z));

	if (use_magnetometer) {
		imu_calibration.mag.abs_x = math_max(
				math_abs(imu_calibration.mag.max_x),
				math_abs(imu_calibration.mag.min_x));
		imu_calibration.mag.abs_y = math_max(
				math_abs(imu_calibration.mag.max_y),
				math_abs(imu_calibration.mag.min_y));
		imu_calibration.mag.abs_z = math_max(
				math_abs(imu_calibration.mag.max_z),
				math_abs(imu_calibration.mag.min_z));
	}
}

static void calibration_imu_measurements(const AxesRaw_t *accel_data,
		const AxesRaw_t *gyro_data, const AxesRaw_t *mag_data) {

	bool use_magnetometer = (NULL != mag_data);

	if (iteration != 0 && iteration % SAMPLE_SIZE == 0) {
		imu_measurement.accel.mean_x = math_mean(SAMPLE_SIZE,
				imu_measurement.accel.data_x);
		imu_measurement.accel.mean_y = math_mean(SAMPLE_SIZE,
				imu_measurement.accel.data_y);
		imu_measurement.accel.mean_z = math_mean(SAMPLE_SIZE,
				imu_measurement.accel.data_z);

		imu_measurement.gyro.mean_x = math_mean(SAMPLE_SIZE,
				imu_measurement.gyro.data_x);
		imu_measurement.gyro.mean_y = math_mean(SAMPLE_SIZE,
				imu_measurement.gyro.data_y);
		imu_measurement.gyro.mean_z = math_mean(SAMPLE_SIZE,
				imu_measurement.gyro.data_z);

		imu_measurement.accel.variance_x = math_variance(SAMPLE_SIZE,
				imu_measurement.accel.mean_x, imu_measurement.accel.data_x);
		imu_measurement.accel.variance_y = math_variance(SAMPLE_SIZE,
				imu_measurement.accel.mean_y, imu_measurement.accel.data_y);
		imu_measurement.accel.variance_z = math_variance(SAMPLE_SIZE,
				imu_measurement.accel.mean_z, imu_measurement.accel.data_z);

		imu_measurement.gyro.variance_x = math_variance(SAMPLE_SIZE,
				imu_measurement.gyro.mean_x, imu_measurement.gyro.data_x);
		imu_measurement.gyro.variance_y = math_variance(SAMPLE_SIZE,
				imu_measurement.gyro.mean_y, imu_measurement.gyro.data_y);
		imu_measurement.gyro.variance_z = math_variance(SAMPLE_SIZE,
				imu_measurement.gyro.mean_z, imu_measurement.gyro.data_z);

		imu_measurement.accel.stddev_x = math_stddev(
				imu_measurement.accel.variance_x);
		imu_measurement.accel.stddev_y = math_stddev(
				imu_measurement.accel.variance_y);
		imu_measurement.accel.stddev_z = math_stddev(
				imu_measurement.accel.variance_z);

		imu_measurement.gyro.stddev_x = math_stddev(
				imu_measurement.gyro.variance_x);
		imu_measurement.gyro.stddev_y = math_stddev(
				imu_measurement.gyro.variance_y);
		imu_measurement.gyro.stddev_z = math_stddev(
				imu_measurement.gyro.variance_z);

		imu_measurement.accel.cov_x_y = math_covariance(SAMPLE_SIZE,
				imu_measurement.accel.data_x, imu_measurement.accel.mean_x,
				imu_measurement.accel.data_y, imu_measurement.accel.mean_y);
		imu_measurement.accel.cov_x_z = math_covariance(SAMPLE_SIZE,
				imu_measurement.accel.data_x, imu_measurement.accel.mean_x,
				imu_measurement.accel.data_z, imu_measurement.accel.mean_z);
		imu_measurement.accel.cov_y_z = math_covariance(SAMPLE_SIZE,
				imu_measurement.accel.data_y, imu_measurement.accel.mean_y,
				imu_measurement.accel.data_z, imu_measurement.accel.mean_z);

		imu_measurement.gyro.cov_x_y = math_covariance(SAMPLE_SIZE,
				imu_measurement.gyro.data_x, imu_measurement.gyro.mean_x,
				imu_measurement.gyro.data_y, imu_measurement.gyro.mean_y);
		imu_measurement.gyro.cov_x_z = math_covariance(SAMPLE_SIZE,
				imu_measurement.gyro.data_x, imu_measurement.gyro.mean_x,
				imu_measurement.gyro.data_z, imu_measurement.gyro.mean_z);
		imu_measurement.gyro.cov_y_z = math_covariance(SAMPLE_SIZE,
				imu_measurement.gyro.data_y, imu_measurement.gyro.mean_y,
				imu_measurement.gyro.data_z, imu_measurement.gyro.mean_z);

		imu_measurement.accel.corr_x_y = math_correlation(
				imu_measurement.accel.cov_x_y, imu_measurement.accel.mean_x,
				imu_measurement.accel.mean_y);
		imu_measurement.accel.corr_x_z = math_correlation(
				imu_measurement.accel.cov_x_z, imu_measurement.accel.mean_x,
				imu_measurement.accel.mean_z);
		imu_measurement.accel.corr_y_z = math_correlation(
				imu_measurement.accel.cov_y_z, imu_measurement.accel.mean_y,
				imu_measurement.accel.mean_z);

		imu_measurement.gyro.corr_x_y = math_correlation(
				imu_measurement.gyro.cov_x_y, imu_measurement.gyro.mean_x,
				imu_measurement.gyro.mean_y);
		imu_measurement.gyro.corr_x_z = math_correlation(
				imu_measurement.gyro.cov_x_z, imu_measurement.gyro.mean_x,
				imu_measurement.gyro.mean_z);
		imu_measurement.gyro.corr_y_z = math_correlation(
				imu_measurement.gyro.cov_y_z, imu_measurement.gyro.mean_y,
				imu_measurement.gyro.mean_z);

		if (use_magnetometer) {
			imu_measurement.mag.mean_x = math_mean(SAMPLE_SIZE,
					imu_measurement.mag.data_x);
			imu_measurement.mag.mean_y = math_mean(SAMPLE_SIZE,
					imu_measurement.mag.data_y);
			imu_measurement.mag.mean_z = math_mean(SAMPLE_SIZE,
					imu_measurement.mag.data_z);

			imu_measurement.mag.variance_x = math_variance(SAMPLE_SIZE,
					imu_measurement.mag.mean_x, imu_measurement.mag.data_x);
			imu_measurement.mag.variance_y = math_variance(SAMPLE_SIZE,
					imu_measurement.mag.mean_y, imu_measurement.mag.data_y);
			imu_measurement.mag.variance_z = math_variance(SAMPLE_SIZE,
					imu_measurement.mag.mean_z, imu_measurement.mag.data_z);

			imu_measurement.mag.stddev_x = math_stddev(
					imu_measurement.mag.variance_x);
			imu_measurement.mag.stddev_y = math_stddev(
					imu_measurement.mag.variance_y);
			imu_measurement.mag.stddev_z = math_stddev(
					imu_measurement.mag.variance_z);

			imu_measurement.mag.cov_x_y = math_covariance(SAMPLE_SIZE,
					imu_measurement.mag.data_x, imu_measurement.mag.mean_x,
					imu_measurement.mag.data_y, imu_measurement.mag.mean_y);
			imu_measurement.mag.cov_x_z = math_covariance(SAMPLE_SIZE,
					imu_measurement.mag.data_x, imu_measurement.mag.mean_x,
					imu_measurement.mag.data_z, imu_measurement.mag.mean_z);
			imu_measurement.mag.cov_y_z = math_covariance(SAMPLE_SIZE,
					imu_measurement.mag.data_y, imu_measurement.mag.mean_y,
					imu_measurement.mag.data_z, imu_measurement.mag.mean_z);

			imu_measurement.mag.corr_x_y = math_correlation(
					imu_measurement.mag.cov_x_y, imu_measurement.mag.mean_x,
					imu_measurement.mag.mean_y);
			imu_measurement.mag.corr_x_z = math_correlation(
					imu_measurement.mag.cov_x_z, imu_measurement.mag.mean_x,
					imu_measurement.mag.mean_z);
			imu_measurement.mag.corr_y_z = math_correlation(
					imu_measurement.mag.cov_y_z, imu_measurement.mag.mean_y,
					imu_measurement.mag.mean_z);
		}

		iteration = 0;

	} else {
		imu_measurement.accel.data_x[iteration] = (float) accel_data->AXIS_X;
		imu_measurement.accel.data_y[iteration] = (float) accel_data->AXIS_Y;
		imu_measurement.accel.data_z[iteration] = (float) accel_data->AXIS_Z;

		imu_measurement.gyro.data_x[iteration] = (float) gyro_data->AXIS_X;
		imu_measurement.gyro.data_y[iteration] = (float) gyro_data->AXIS_Y;
		imu_measurement.gyro.data_z[iteration] = (float) gyro_data->AXIS_Z;

		if (use_magnetometer) {
			imu_measurement.mag.data_x[iteration] = (float) mag_data->AXIS_X;
			imu_measurement.mag.data_y[iteration] = (float) mag_data->AXIS_Y;
			imu_measurement.mag.data_z[iteration] = (float) mag_data->AXIS_Z;
		}

		iteration++;
	}
}

static void calibration_imu_noise(const AxesRaw_t *accel_data,
		const AxesRaw_t *gyro_data, const AxesRaw_t *mag_data) {

	bool use_magnetometer = (NULL != mag_data);

	//gather every tick measurement + cumulative drift
	imu_noise.n_da_x = (accel_data + 1)->AXIS_X - accel_data->AXIS_X;
	imu_noise.n_da_y = (accel_data + 1)->AXIS_Y - accel_data->AXIS_Y;
	imu_noise.n_da_z = (accel_data + 1)->AXIS_Z - accel_data->AXIS_Z;
	imu_noise.n_dg_x = (gyro_data + 1)->AXIS_X - gyro_data->AXIS_X;
	imu_noise.n_dg_y = (gyro_data + 1)->AXIS_Y - gyro_data->AXIS_Y;
	imu_noise.n_dg_z = (gyro_data + 1)->AXIS_Z - gyro_data->AXIS_Z;

	imu_noise.n_ia_x += imu_noise.n_da_x;
	imu_noise.n_ia_y += imu_noise.n_da_y;
	imu_noise.n_ia_z += imu_noise.n_da_z;
	imu_noise.n_ig_x += imu_noise.n_dg_x;
	imu_noise.n_ig_y += imu_noise.n_dg_y;
	imu_noise.n_ig_z += imu_noise.n_dg_z;

	if (use_magnetometer) {
		imu_noise.n_dm_x = (mag_data + 1)->AXIS_X - mag_data->AXIS_X;
		imu_noise.n_dm_y = (mag_data + 1)->AXIS_Y - mag_data->AXIS_Y;
		imu_noise.n_dm_z = (mag_data + 1)->AXIS_Z - mag_data->AXIS_Z;

		imu_noise.n_im_x += imu_noise.n_dm_x;
		imu_noise.n_im_y += imu_noise.n_dm_y;
		imu_noise.n_im_z += imu_noise.n_dm_z;
	}
}

static void calibration_imu(const AxesRaw_t *accel_data,
		const AxesRaw_t *gyro_data, const AxesRaw_t *mag_data) {

	bool use_magnetometer = (NULL != mag_data);

	imu_calibration.accel.max_x = math_max(accel_data->AXIS_X,
			imu_calibration.accel.max_x);
	imu_calibration.accel.min_x = math_min(accel_data->AXIS_X,
			imu_calibration.accel.min_x);

	imu_calibration.accel.max_y = math_max(accel_data->AXIS_Y,
			imu_calibration.accel.max_y);
	imu_calibration.accel.min_y = math_min(accel_data->AXIS_Y,
			imu_calibration.accel.min_y);

	imu_calibration.accel.max_z = math_max(accel_data->AXIS_Z,
			imu_calibration.accel.max_z);
	imu_calibration.accel.min_z = math_min(accel_data->AXIS_Z,
			imu_calibration.accel.min_z);

	imu_calibration.gyro.max_x = math_max(gyro_data->AXIS_X,
			imu_calibration.gyro.max_x);
	imu_calibration.gyro.min_x = math_min(gyro_data->AXIS_X,
			imu_calibration.gyro.min_x);

	imu_calibration.gyro.max_y = math_max(gyro_data->AXIS_Y,
			imu_calibration.gyro.max_y);
	imu_calibration.gyro.min_y = math_min(gyro_data->AXIS_Y,
			imu_calibration.gyro.min_y);

	imu_calibration.gyro.max_z = math_max(gyro_data->AXIS_Z,
			imu_calibration.gyro.max_z);
	imu_calibration.gyro.min_z = math_min(gyro_data->AXIS_Z,
			imu_calibration.gyro.min_z);

	if (use_magnetometer) {
		imu_calibration.mag.max_x = math_max(mag_data->AXIS_X,
				imu_calibration.mag.max_x);
		imu_calibration.mag.min_x = math_min(mag_data->AXIS_X,
				imu_calibration.mag.min_x);

		imu_calibration.mag.max_y = math_max(mag_data->AXIS_Y,
				imu_calibration.mag.max_y);
		imu_calibration.mag.min_y = math_min(mag_data->AXIS_Y,
				imu_calibration.mag.min_y);

		imu_calibration.mag.max_z = math_max(mag_data->AXIS_Z,
				imu_calibration.mag.max_z);
		imu_calibration.mag.min_z = math_min(mag_data->AXIS_Z,
				imu_calibration.mag.min_z);
	}

	calculate_maxima(use_magnetometer);
}

void calibration(const AxesRaw_t *accel_data, const AxesRaw_t *gyro_data,
		const AxesRaw_t *mag_data) {

	calibration_imu(accel_data, gyro_data, mag_data);
	calibration_imu_measurements(accel_data, gyro_data, mag_data);
	calibration_imu_noise(accel_data, gyro_data, mag_data);

}
