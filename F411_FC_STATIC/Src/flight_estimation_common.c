/*
 * flight_estimation_common.c
 *
 *  Created on: Feb 21, 2024
 *      Author: konrad
 */

#include "env.h"
#include "drone.h"
#include "timer.h"
#include "matrix.h"
#include "ahrs_common.h"
#include "flight_estimation_common.h"

static UKF_t ukf_filter;

static void estimation_ukf_predict() {
	ukf_filter.state.pos.x += ukf_filter.state.vel.x * ukf_filter.dt;
	ukf_filter.state.pos.y += ukf_filter.state.vel.y * ukf_filter.dt;
	ukf_filter.state.pos.z += ukf_filter.state.vel.z * ukf_filter.dt;
}

static void estimation_ukf_update() {
	//accx =  1g * sin(theta) * cos(phi)
	//accy = -1g * sin(theta) * sin(phi)
	//accz =  1g * cos(theta)
	//accy/accx = -tan(phi)
}

void estimation_ukf() {
	estimation_ukf_predict();
	estimation_ukf_update();
}

// BLUE - best linear unbiased estimate
void estimation_blue(const float time[3], const Vector3D_t observations) {

	Matrix3D_t blue_model;
	Matrix3D_t blue_covariance;

	for (int i = 0; i < 3; i++) {

		blue_model.matrix[i][0] = 1.0;
		blue_model.matrix[i][1] = time[i];
		blue_model.matrix[i][2] = 0.5 * time[i] * time[i];

		for (int j = 0; j < 3; j++) {
			if (i == j) {
				blue_covariance.matrix[i][j] = time[i];
			} else {
				blue_covariance.matrix[i][j] = 0.0;
			}
		}
	}

	Matrix3D_t cov_inv = matrix_inverse(&blue_covariance);
	Matrix3D_t cov_model_prod = matrix_multiply_matrix(&cov_inv, &blue_model);
	Matrix3D_t model_trans = blue_model;
	matrix_transpose(&model_trans);
	Matrix3D_t prod1 = matrix_multiply_matrix(&model_trans, &cov_model_prod);
	Matrix3D_t prod2 = matrix_multiply_matrix(&prod1, &model_trans);
	Matrix3D_t prod3 = matrix_multiply_matrix(&prod2, &cov_inv);
	Vector3D_t xhat;
	matrix_vector_product(&prod3, &observations, &xhat);
	Vector3D_t result;
	matrix_vector_product(&blue_model, &xhat, &result);
}

UKF_t* get_ukf_filter() {
	return &ukf_filter;
}
