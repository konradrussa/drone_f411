/*
 * bmp280.c
 *
 *  Created on: Oct 26, 2023
 *      Author: konrad
 */
#include "i2c.h"
#include "bmp280.h"
#include "bmp2_defs.h"

#include <stdio.h>
#include <string.h>

I2C_HandleTypeDef *bmp_i2c;

/*! Variable that holds the I2C device address or SPI chip selection */
static uint8_t dev_addr;
struct bmp2_status status;

static BMP280_t bmp;

int8_t bmp280_init(I2C_HandleTypeDef *iic) {
	bmp_i2c = iic;
	int8_t rslt = BMP2_OK;
	dev_addr = BMP2_I2C_ADDR_PRIM;
	bmp.bmp.read = &bmp280_i2c_read;
	bmp.bmp.write = &bmp280_i2c_write;
	bmp.bmp.intf = BMP2_I2C_INTF;
	/* Holds the I2C device addr or SPI chip selection */
	bmp.bmp.intf_ptr = &dev_addr;
	/* Configure delay in microseconds */
	bmp.bmp.delay_us = &bmp280_delay_ms;
	rslt = bmp2_init(&bmp.bmp);
	if (BMP2_OK != rslt) {
		return rslt;
	}
	bmp.conf.filter = BMP2_FILTER_OFF;
	/* Over-sampling mode is set as high resolution i.e., os_pres = 8x and os_temp = 1x */
	bmp.conf.os_mode = BMP2_OS_MODE_HIGH_RESOLUTION;
	/* Setting the output data rate */
	bmp.conf.odr = BMP2_ODR_250_MS;
	rslt = bmp2_set_config(&bmp.conf, &bmp.bmp);
	if (BMP2_OK != rslt) {
		return rslt;
	}
	/* Set normal power mode */
	rslt = bmp2_set_power_mode(BMP2_POWERMODE_NORMAL, &bmp.conf,
			&bmp.bmp);
	if (BMP2_OK != rslt) {
		return rslt;
	}
	/* Calculate measurement time in microseconds */
	rslt = bmp2_compute_meas_time(&bmp.meas_time, &bmp.conf,
			&bmp.bmp);
	if (BMP2_OK != rslt) {
		return rslt;
	}
	/* Always read the current settings before writing, especially when all the configuration is not modified */
	rslt = bmp2_get_config(&bmp.conf, &bmp.bmp);
	bmp.result = rslt;
	return rslt;
}

int8_t bmp280_get_data() {
	/* Configuring the over-sampling mode, filter coefficient and output data rate */
	/* Overwrite the desired settings */
	bmp.result_data = BMP2_E_NULL_PTR;
	/* Read pressure and temperature data */
	bmp.result_data = bmp2_get_status(&status, &bmp.bmp);
	if(BMP2_OK != bmp.result_data) {
		return bmp.result_data;
	}
	if (status.measuring == BMP2_MEAS_DONE) {
		/* Delay between measurements */ //FIXME alternated sensing
		bmp.bmp.delay_us((uint32_t) bmp.meas_time / 1000,
				&bmp.bmp.intf_ptr); //us to ms
		/* Read compensated data */
		bmp.result_data = bmp2_get_sensor_data(&bmp.data, &bmp.bmp);
	}
	return bmp.result_data;
}

int8_t bmp280_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t length,
		const void *intf_ptr) {
	dev_addr = *(uint8_t*) intf_ptr;
	int8_t rslt = 0;
	HAL_StatusTypeDef status = HAL_OK;
	status = HAL_I2C_Master_Transmit(bmp_i2c, (uint8_t) (dev_addr << 1),
			&reg_addr, 1, 100);
	if (status != HAL_OK) {
		rslt = (-1);
	}
	status = HAL_I2C_Master_Receive(bmp_i2c, (uint8_t) (dev_addr << 1), reg_data,
			length, 100);
	if (status != HAL_OK) {
		rslt = (-1);
	}
	return rslt;
}

int8_t bmp280_i2c_write(uint8_t reg_addr, const uint8_t *reg_data,
		uint32_t length, const void *intf_ptr) {
	dev_addr = *(uint8_t*) intf_ptr;
	int8_t rslt = 0;
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t tx_buffer[length + 1];
	tx_buffer[0] = reg_addr;
	memcpy(&tx_buffer[1], reg_data, length);
	status = HAL_I2C_Master_Transmit(bmp_i2c, (uint8_t) (dev_addr << 1),
			tx_buffer, length + 1, 100);
	if (status != HAL_OK) {
		rslt = (-1);
	}
	return rslt;

}
void bmp280_delay_ms(uint32_t period_us, void *intf_ptr) {
	(void) intf_ptr;
	HAL_Delay(period_us + 1);
}

inline BMP280_t* get_bmp() {
	return &bmp;
}
