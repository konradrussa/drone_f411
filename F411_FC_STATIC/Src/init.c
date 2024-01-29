/*
 * init.c
 *
 *  Created on: Dec 23, 2023
 *      Author: konrad
 */
#include "init.h"
#include <stdbool.h>

static volatile bool initialized = false;
static const memory_order memory_order_drone = memory_order_seq_cst;

inline const memory_order* get_memory_order() {
	return &memory_order_drone;
}

void pre_init() {
	if (!initialized) {
		atomic_signal_fence(memory_order_drone);
		atomic_thread_fence(memory_order_drone);
		initialized = true;
	}
}
