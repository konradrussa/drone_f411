/*
 * queue.h
 *
 *  Created on: Dec 23, 2023
 *      Author: konrad
 */

#ifndef INC_QUEUE_H_
#define INC_QUEUE_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdatomic.h>
#include "init.h"

#define QUEUE_SIZE (64)

typedef struct Queue {
	uint16_t pR, pW;
	atomic_uint q[QUEUE_SIZE];
} Queue_t;

bool queue_data_available(Queue_t *queue);
bool queue_enqueue(Queue_t *queue, uint32_t data);
bool queue_dequeue(Queue_t *queue, uint32_t *data);

#endif /* INC_QUEUE_H_ */
