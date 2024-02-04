/*
 * bridge.h
 *
 *  Created on: Jan 4, 2024
 *      Author: konrad
 */

#ifndef INC_BRIDGE_H_
#define INC_BRIDGE_H_

#include "bridge_common.h"
#include "drone.h"
#include "imu.h"

#define BRIDGE_DATA_SIZE (35)

typedef struct BRIDGE {
	uint8_t rx_data[BRIDGE_DATA_SIZE];
	UART_HandleTypeDef *fc_uart;
	I2C_HandleTypeDef *imu_i2c;
	BRIDGE_COMMON_t *port;
} BRIDGE_t;

HAL_StatusTypeDef bridge_init(UART_HandleTypeDef *uart, I2C_HandleTypeDef *i2c);
HAL_StatusTypeDef bridge_transmit(char *data);
HAL_StatusTypeDef bridge_drone_arm(void);
char* bridge_get_queues_data(void);

#endif /* INC_BRIDGE_H_ */
