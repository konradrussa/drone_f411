/*
 * imu.c
 *
 *  Created on: Oct 27, 2023
 *      Author: konrad
 */

#include "imu.h"

struct bmi160_dev bmi_ref;

I2C_HandleTypeDef *imu_i2c;

static IMU_t imu = { .running = false };

//IMU
int8_t imu_init(I2C_HandleTypeDef *iic) {
	imu_i2c = iic;
	imu.bmi = &bmi_ref;

	bmi_ref.id = BMI160_I2C_ADDR;
	bmi_ref.chip_id = BMI160_CHIP_ID;
	bmi_ref.intf = BMI160_I2C_INTF;
	bmi_ref.read = &imu_i2c_read;
	bmi_ref.write = &imu_i2c_write;
	bmi_ref.delay_ms = &imu_delay_ms;

	imu.result = bmi160_init(imu.bmi);
	if (BMI160_OK != imu.result) {
		return imu.result;
	}

	bmi_ref.accel_cfg.odr = BMI160_ACCEL_ODR_1600HZ;
	bmi_ref.accel_cfg.range = BMI160_ACCEL_RANGE_16G;
	bmi_ref.accel_cfg.bw = BMI160_ACCEL_BW_NORMAL_AVG4;
	bmi_ref.accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;

	bmi_ref.gyro_cfg.odr = BMI160_GYRO_ODR_1600HZ;
	bmi_ref.gyro_cfg.range = BMI160_GYRO_RANGE_1000_DPS;
	bmi_ref.gyro_cfg.bw = BMI160_GYRO_BW_NORMAL_MODE;
	bmi_ref.gyro_cfg.power = BMI160_GYRO_NORMAL_MODE;

	imu.result = bmi160_set_sens_conf(&bmi_ref);
	if (BMI160_OK != imu.result) {
		return imu.result;
	}
	HAL_Delay(10); //The device is powering up in less then 10ms. Page 13. 2.2 Power modes.
	imu.running = true;
	return BMI160_OK;
}

int8_t imu_get_data() {
	// add previous data to next index
	for (int i = IMU_FRAMES - 2; i >= 0; i--) {
		imu.accel_data[i + 1] = imu.accel_data[i];
		imu.gyro_data[i + 1] = imu.gyro_data[i];
	}
	imu.result_data = bmi160_get_sensor_data(
			(BMI160_ACCEL_SEL | BMI160_GYRO_SEL), &imu.accel_data[0],
			&imu.gyro_data[0], &bmi_ref);
	if (BMI160_OK != imu.result_data) {
		return imu.result_data;
	}
	return BMI160_OK;
}

int8_t imu_i2c_read(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len) {
	int8_t rslt = 0;
	HAL_StatusTypeDef status = HAL_OK;
	status = HAL_I2C_Master_Transmit(imu_i2c, (uint8_t) (id << 1), &reg_addr, 1,
			100);
	if (status != HAL_OK) {
		rslt = status;
	}
	status = HAL_I2C_Master_Receive(imu_i2c, (uint8_t) (id << 1), data, len,
			100);
	if (status != HAL_OK) {
		rslt = status;
	}
	return rslt;
}

int8_t imu_i2c_write(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len) {
	int8_t rslt = 0;
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t tx_buffer[len + 1];
	tx_buffer[0] = reg_addr;
	memcpy(&tx_buffer[1], data, len);
	status = HAL_I2C_Master_Transmit(imu_i2c, (uint8_t) (id << 1), tx_buffer,
			len + 1, 100);
	if (status != HAL_OK) {
		rslt = status;
	}
	return rslt;
}

void imu_delay_ms(uint32_t period) {
	HAL_Delay(period);
}

inline IMU_t* get_imu() {
	return &imu;
}
