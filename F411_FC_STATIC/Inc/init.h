/*
 * init.h
 *
 *  Created on: Dec 23, 2023
 *      Author: konrad
 */

#include <stdatomic.h>

const memory_order* get_memory_order(void);
void pre_init(void);
