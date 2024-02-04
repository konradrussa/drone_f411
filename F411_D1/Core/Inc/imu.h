/*
 * imu.h
 *
 *  Created on: Oct 27, 2023
 *      Author: konrad
 */

#ifndef INC_IMU_H_
#define INC_IMU_H_

#include <stdbool.h>
#include "bmi160.h"
#include "stm32f4xx.h"

#define IMU_FRAMES	(10)
#define IMU_FREQUENCY_1600HZ (1600)

// BMI160
typedef struct IMU {
	struct bmi160_sensor_data accel_data[IMU_FRAMES];
	struct bmi160_sensor_data gyro_data[IMU_FRAMES];
	int8_t result, result_data;
	volatile bool running;
	struct bmi160_dev *bmi;
} IMU_t;

IMU_t* get_imu(void);
int8_t imu_init(I2C_HandleTypeDef *i2c);
int8_t imu_get_data(void);
int8_t imu_i2c_read(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len);
int8_t imu_i2c_write(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len);
void imu_delay_ms(uint32_t period);

#endif /* INC_IMU_H_ */
