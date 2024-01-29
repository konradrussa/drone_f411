/*
 * ultrasonic.c
 *
 *  Created on: Dec 14, 2023
 *      Author: konrad
 */
#include "ultrasonic.h"
#include "usart.h"
#include "i2c.h"
#include "gpio.h"

I2C_HandleTypeDef *ul_i2c;
GPIO_TypeDef *ul_busy_port;
uint16_t ul_busy_pin;

volatile uint16_t cr = 0;
uint8_t cfg = 0, cmd = 0;
volatile bool tx_done = false;
volatile bool rx_done = false;
volatile bool error = false;

uint8_t rxBuf[10] = { 0 };
int16_t distance = 0;
float temperature = 0.0;

static UltrasonicData_t data;

inline UltrasonicData_t* get_data() {
	return &data;
}

inline static bool ultrasonic_i2c_check_busy() {
	return HAL_GPIO_ReadPin(ul_busy_port, ul_busy_pin) == GPIO_PIN_RESET;
}

int8_t ultrasonic_i2c_write(uint8_t dev_addr, UltrasonicRegIndex_e reg_addr,
		uint8_t *reg_data, uint8_t length) {
	int8_t rslt = 0;
	HAL_StatusTypeDef status = HAL_OK;
	while (HAL_I2C_IsDeviceReady(ul_i2c, (uint8_t) (dev_addr << 1), 3, 100)
			!= HAL_OK) {
	}
	uint8_t tx_buffer[length + 1];
	tx_buffer[0] = reg_addr;
	memcpy(&tx_buffer[1], reg_data, length);
	status = HAL_I2C_Master_Transmit(ul_i2c, (uint8_t) (dev_addr << 1),
			tx_buffer, length + 1, 100);
	if (status != HAL_OK) {
		rslt = (-1);
	}
	return rslt;
}

int8_t ultrasonic_i2c_read(uint8_t dev_addr, UltrasonicRegIndex_e reg_addr,
		uint8_t *reg_data, uint8_t length) {
	int8_t rslt = 0;
	HAL_StatusTypeDef status = HAL_OK;
	while (HAL_I2C_IsDeviceReady(ul_i2c, (uint8_t) (dev_addr << 1), 3, 100)
			!= HAL_OK) {
		rslt = 0;
	}
	status = HAL_I2C_Master_Transmit(ul_i2c, (uint8_t) (dev_addr << 1),
			&reg_addr, 1, 100);
	if (status != HAL_OK) {
		rslt = (-1);
	}
	status = HAL_I2C_Master_Receive(ul_i2c, (uint8_t) (dev_addr << 1), reg_data,
			length, 100);
	if (status != HAL_OK) {
		rslt = (-1);
	}
	return rslt;
}

void ultrasonic_init(I2C_HandleTypeDef *iic, GPIO_TypeDef *gpio_port, uint16_t gpio_pin) {
	ul_i2c = iic;
	ul_busy_port = gpio_port;
	ul_busy_pin = gpio_pin;
	cfg &= ~I2C_MEASURE_RANGE_BIT; //clear bit4,long-range ranging mode
	cfg |= I2C_MEASURE_MODE_BIT;	  //Set bit2，i2c passive mode
	cfg &= ~I2C_TEMP_CPT_ENABLE_BIT;//clear bit1,enable temperature compensation
	cfg &= ~I2C_TEMP_CPT_SEL_BIT;//clear bit0,select internal temperature compensation
	ultrasonic_i2c_write(I2C_SLAVE_ADDR, iConfig, &cfg, 1);
	HAL_Delay(100);
}

int16_t ultrasonic_get_data() {
	cmd |= 0x01;	//Set trig bit
	ultrasonic_i2c_write(I2C_SLAVE_ADDR, iCmd, &cmd, 1);//Write command register
	while (ultrasonic_i2c_check_busy())
		;  //Wait for the sensor to start ranging
	while (!ultrasonic_i2c_check_busy())
		;   //Wait for sensor ranging to complete
	ultrasonic_i2c_read(I2C_SLAVE_ADDR, iDistanceH, rxBuf, 2); //Read distance register
	distance = ((uint16_t) rxBuf[0] << 8) + rxBuf[1];
	data.distance = distance;
//	HAL_Delay(10);
//	ultrasonic_i2c_read(I2C_SLAVE_ADDR, iInternalTempretureH, rxBuf, 2); //Read the onboard temperature register
//	temperature = ((uint16_t) rxBuf[0] << 8) + rxBuf[1];
//	temperature /= 10;
//	data.temperature = temperature;
	return distance;
}
