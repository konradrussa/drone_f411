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

#define BRIDGE_DATA_SIZE_IN (30)
#define BRIDGE_DATA_SIZE_OUT (35)

typedef struct BRIDGE {
	uint8_t rx_data[BRIDGE_DATA_SIZE_IN];
	UART_HandleTypeDef *fc_uart;
	BRIDGE_COMMON_t *port;
	atomic_bool armed;
} BRIDGE_t;

HAL_StatusTypeDef bridge_init(UART_HandleTypeDef *uart);
HAL_StatusTypeDef bridge_transmit(char *data);
bool bridge_get_armed(void);

#endif /* INC_BRIDGE_H_ */
