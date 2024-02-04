/*
 * bridge.h
 *
 *  Created on: Jan 4, 2024
 *      Author: konrad
 */

#ifndef INC_BRIDGE_H_
#define INC_BRIDGE_H_

#include "bridge_common.h"
#include "flight_control.h"

#define BRIDGE_DATA_SIZE (30)

typedef struct BRIDGE {
	uint8_t rx_data[BRIDGE_DATA_SIZE];
	UART_HandleTypeDef *fc_uart;
	BRIDGE_COMMON_t *port;
} BRIDGE_t;

HAL_StatusTypeDef bridge_init(UART_HandleTypeDef *uart);
HAL_StatusTypeDef bridge_transmit(char *data);
HAL_StatusTypeDef bridge_drone_arm(void);

#endif /* INC_BRIDGE_H_ */
