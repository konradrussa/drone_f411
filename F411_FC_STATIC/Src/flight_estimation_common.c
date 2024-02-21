/*
 * flight_estimation_common.c
 *
 *  Created on: Feb 21, 2024
 *      Author: konrad
 */

#include "flight_estimation_common.h"
#include "matrix.h"

//accx =  1g * sin(theta) * cos(phi)
//accy = -1g * sin(theta) * sin(phi)
//accz =  1g * cos(theta)
//accy/accx = -tan(phi)

//BLUE - best linear unbiased estimate
Matrix3D_t blue_model;
Matrix3D_t blue_covariance;

void estimation_blue(float time[3], Vector3D_t observations) {

	for (int i = 0; i < 3; i++) {

		blue_model.matrix[i][0] = 1.0;
		blue_model.matrix[i][1] = time[i];
		blue_model.matrix[i][2] = 0.5 * time[i] * time[i];

		for (int j = 0; j < 3; j++) {
			if (i == j) {
				float time_sqrt = math_sqrt(time[i]);
				blue_covariance.matrix[i][j] = 1.0 * time_sqrt * time_sqrt;
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
	matrix_rotation_matrix_vector_product(&prod3, &observations, &xhat);
	Vector3D_t result;
	matrix_rotation_matrix_vector_product(&blue_model, &xhat, &result);
}
