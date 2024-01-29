/*
 * imu.h
 *
 *  Created on: Oct 27, 2023
 *      Author: konrad
 */

#ifndef INC_IMU_H_
#define INC_IMU_H_

#include "bmm150.h"
#include "bmi160.h"

#include "stm32f4xx.h"

#define IMU_FRAMES	(10)
#define IMU_FREQUENCY_800HZ (800)

// BMI160 + BMM150
typedef struct IMU {
	struct bmi160_dev *bmi;
	struct bmm150_dev *bmm;
	struct bmi160_sensor_data accel_data[IMU_FRAMES];
	struct bmi160_sensor_data gyro_data[IMU_FRAMES];
	struct bmm150_mag_data mag_data[IMU_FRAMES];
	int8_t result, result_data;
} IMU_t;

IMU_t* get_imu(void);
int8_t imu_init(TIM_HandleTypeDef *htim, I2C_HandleTypeDef *i2c);
int8_t imu_get_data(void);
int8_t imu_i2c_read(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len);
int8_t imu_i2c_write(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len);
void imu_delay_ms(uint32_t period);
int8_t bmm150_aux_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t length,
		void *intf_ptr);
int8_t bmm150_aux_write(uint8_t reg_addr, const uint8_t *reg_data,
		uint32_t length, void *intf_ptr);
void bmm150_aux_delay_us(uint32_t period, void *intf_ptr);

#endif /* INC_IMU_H_ */
