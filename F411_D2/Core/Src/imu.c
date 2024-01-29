/*
 * imu.c
 *
 *  Created on: Oct 27, 2023
 *      Author: konrad
 */

#include "imu.h"

struct bmi160_dev bmi_ref;
struct bmm150_dev bmm_ref;
struct bmm150_settings bmm_settings;

TIM_HandleTypeDef *imu_timer;
I2C_HandleTypeDef *imu_i2c;

static IMU_t imu;

//IMU
int8_t imu_init(TIM_HandleTypeDef *htim, I2C_HandleTypeDef *iic) {
	imu_timer = htim;
	imu_i2c = iic;
	HAL_StatusTypeDef timer_status = HAL_TIM_Base_Start(imu_timer);
	if (HAL_OK != timer_status) {
		return timer_status;
	}
	imu.bmi = &bmi_ref;
	imu.bmm = &bmm_ref;

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

	bmi_ref.aux_cfg.aux_sensor_enable = BMI160_ENABLE;
	bmi_ref.aux_cfg.aux_i2c_addr = BMI160_AUX_BMM150_I2C_ADDR;
	bmi_ref.aux_cfg.aux_rd_burst_len = 2; // 8 bytes
	bmi_ref.aux_cfg.manual_enable = BMI160_ENABLE; // Manual mode

	imu.result = bmi160_aux_init(&bmi_ref);
	if (BMI160_OK != imu.result) {
		return imu.result;
	}

	bmm_ref.intf = BMM150_I2C_INTF;
	bmm_ref.chip_id = BMM150_CHIP_ID;
	bmm_ref.read = &bmm150_aux_read;
	bmm_ref.write = &bmm150_aux_write;
	bmm_ref.delay_us = &bmm150_aux_delay_us;
	bmm_ref.intf_ptr = &bmi_ref;
	bmm_ref.pwr_cntrl_bit = BMM150_POWER_CNTRL_ENABLE;

	imu.result = bmm150_init(&bmm_ref);
	if (BMM150_OK != imu.result) {
		return imu.result;
	}

	bmi_ref.accel_cfg.odr = BMI160_ACCEL_ODR_800HZ;
	bmi_ref.accel_cfg.range = BMI160_ACCEL_RANGE_16G;
	bmi_ref.accel_cfg.bw = BMI160_ACCEL_BW_NORMAL_AVG4;
	bmi_ref.accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;

	bmi_ref.gyro_cfg.odr = BMI160_GYRO_ODR_800HZ;
	bmi_ref.gyro_cfg.range = BMI160_GYRO_RANGE_1000_DPS;
	bmi_ref.gyro_cfg.bw = BMI160_GYRO_BW_NORMAL_MODE;
	bmi_ref.gyro_cfg.power = BMI160_GYRO_NORMAL_MODE;

	imu.result = bmi160_set_sens_conf(&bmi_ref);
	if (BMI160_OK != imu.result) {
		return imu.result;
	}
	HAL_Delay(10); //The device is powering up in less then 10ms. Page 13. 2.2 Power modes.

	// Power up from suspend to sleep mode
	bmm_settings.pwr_mode = BMM150_POWERMODE_SLEEP;
	imu.result = bmm150_set_op_mode(&bmm_settings, &bmm_ref);
	if (BMM150_OK != imu.result) {
		return imu.result;
	}
	HAL_Delay(3);

	bmm_settings.preset_mode = BMM150_PRESETMODE_LOWPOWER;
	imu.result = bmm150_set_presetmode(&bmm_settings, &bmm_ref);
	if (BMM150_OK != imu.result) {
		return imu.result;
	}
	bmm_settings.pwr_mode = BMM150_POWERMODE_NORMAL;
	imu.result = bmm150_set_op_mode(&bmm_settings, &bmm_ref);
	if (BMM150_OK != imu.result) {
		return imu.result;
	}

	uint8_t bmm150_data_start = BMM150_REG_DATA_X_LSB;
	bmi_ref.aux_cfg.aux_odr = BMI160_AUX_ODR_400HZ;
	imu.result = bmi160_set_aux_auto_mode(&bmm150_data_start, &bmi_ref);
	if (BMI160_OK != imu.result) {
		return imu.result;
	}
	return BMI160_OK;
}

int8_t imu_get_data() {
	// add previous data to next index
	for (int i = IMU_FRAMES - 2; i >= 0; i--) {
		imu.accel_data[i + 1] = imu.accel_data[i];
		imu.gyro_data[i + 1] = imu.gyro_data[i];
		imu.mag_data[i + 1] = imu.mag_data[i];
	}
	imu.result_data = bmi160_get_sensor_data(
			(BMI160_ACCEL_SEL | BMI160_GYRO_SEL), &imu.accel_data[0],
			&imu.gyro_data[0], &bmi_ref);
	if (BMI160_OK != imu.result_data) {
		return imu.result_data;
	}
	imu.result_data = bmm150_read_mag_data(&imu.mag_data[0], &bmm_ref);
	if (BMM150_OK != imu.result_data) {
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

/* Auxiliary function definitions */
int8_t bmm150_aux_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t length,
		void *intf_ptr) {
	return bmi160_aux_read(reg_addr, reg_data, length, intf_ptr);
}

int8_t bmm150_aux_write(uint8_t reg_addr, const uint8_t *reg_data,
		uint32_t length, void *intf_ptr) {
	return bmi160_aux_write(reg_addr, (uint8_t*) reg_data, length, intf_ptr);
}

void bmm150_aux_delay_us(uint32_t period, void *intf_ptr) {
	(void) intf_ptr;
	imu_timer->Instance->CNT = 0;
	while (imu_timer->Instance->CNT <= period)
		;
}

inline IMU_t* get_imu() {
	return &imu;
}
