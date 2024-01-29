/*
 * ultrasonic.h
 *
 *  Created on: Dec 14, 2023
 *      Author: konrad
 */

#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "stm32f4xx.h"

#define   I2C_SLAVE_ADDR           			((uint8_t) 0x12)		//I2C communication address
#define   I2C_TEMP_CPT_SEL_BIT         		((uint8_t) 0x01 << 0)
#define   I2C_TEMP_CPT_ENABLE_BIT      		((uint8_t) 0x01 << 1)
#define   I2C_MEASURE_MODE_BIT         		((uint8_t) 0x01 << 2)
#define   I2C_MEASURE_RANGE_BIT        		((uint8_t) 0x01 << 4)

typedef enum UltrasonicRegIndex {
	iAddr = 0,
	iPid,
	iVid,
	iDistanceH,
	iDistanceL,
	iInternalTempretureH,
	iInternalTempretureL,
	iExternalTempretureH,
	iExternalTempretureL,
	iConfig,
	iCmd,
	iNoise,
	iSensitivity,
	iRegNum
} UltrasonicRegIndex_e;

typedef struct UltrasonicData {
	int16_t distance;
	float temperature;
} UltrasonicData_t;

void ultrasonic_init(I2C_HandleTypeDef *iic, GPIO_TypeDef *gpio_port, uint16_t gpio_pin);
int16_t ultrasonic_get_data(void);
UltrasonicData_t* get_data(void);

#endif /* INC_ULTRASONIC_H_ */
