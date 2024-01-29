/*
 * queue.c
 *
 *  Created on: Dec 23, 2023
 *      Author: konrad
 */
#include "queue.h"

inline static void queue_store_data(Queue_t *queue, uint32_t data) {
	atomic_store_explicit(&queue->q[queue->pW], data, *get_memory_order());
}

inline static uint32_t queue_get_data(Queue_t *queue) {
	return atomic_load_explicit(&queue->q[queue->pR], *get_memory_order());
}

inline static bool queue_full(Queue_t *queue) {
	return (((queue->pW + 1) % QUEUE_SIZE) == queue->pR);
}

inline static bool queue_empty(Queue_t *queue) {
	return (queue->pW == queue->pR);
}

inline bool queue_data_available(Queue_t *queue) {
	return (queue->pW > queue->pR);
}

bool queue_enqueue(Queue_t *queue, uint32_t data) {
	if (queue_full(queue)) {
		return false;
	} else {
		queue_store_data(queue, data);
		queue->pW = ((queue->pW + 1) == QUEUE_SIZE) ? 0 : queue->pW + 1;
		return true;
	}
}

bool queue_dequeue(Queue_t *queue, uint32_t *data) {
	if (queue_empty(queue))
		return false;
	else {
		*data = queue_get_data(queue);
		queue->pR = ((queue->pR + 1) == QUEUE_SIZE) ? 0 : queue->pR + 1;
		return true;
	}
}
