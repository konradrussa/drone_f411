/*
 * bmp280.h
 *
 *  Created on: Oct 26, 2023
 *      Author: konrad
 */

#ifndef INC_BMP280_H_
#define INC_BMP280_H_

#include "bmp2.h"
#include "stm32f4xx.h"

typedef struct BMP280 {
	struct bmp2_config conf;
	struct bmp2_dev bmp;
	struct bmp2_data data;
	uint32_t meas_time;
	int8_t result, result_data;
} BMP280_t;

int8_t bmp280_init(I2C_HandleTypeDef *iic);
int8_t bmp280_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, const void *intf_ptr);
int8_t bmp280_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, const void *intf_ptr);
void bmp280_delay_ms(uint32_t period_us, void *intf_ptr);
int8_t bmp280_get_data(void);
BMP280_t* get_bmp(void);

#endif /* INC_BMP280_H_ */
