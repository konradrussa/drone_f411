/*
 * bridge_common.h
 *
 *  Created on: Jan 10, 2024
 *      Author: konrad
 */

#ifndef BRIDGE_COMMON_H_
#define BRIDGE_COMMON_H_

#include <stdatomic.h>
#include <stdbool.h>
#include <ctype.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"

typedef HAL_StatusTypeDef (*bridge_transmit_fptr_t)();
typedef HAL_StatusTypeDef (*bridge_receive_fptr_t)();

typedef struct BRIDGE_COMMON {
	char *data_in;
	char *data_out;
	bridge_transmit_fptr_t transmit;
	bridge_receive_fptr_t receive;
	volatile sig_atomic_t signalStatusReceive, signalStatusTransmit;
} BRIDGE_COMMON_t;

bool bridge_validate_data(char *s, int size);
char* bridge_format_string(char *s);
const short bridge_get_transmit_sigint(void);
const short bridge_get_receive_sigint(void);
const char* bridge_get_space(void);
const char* bridge_get_semicolon(void);

#endif /* BRIDGE_COMMON_H_ */
