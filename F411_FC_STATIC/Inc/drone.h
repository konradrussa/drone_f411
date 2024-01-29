/*
 * drone.h
 *
 *  Created on: Jan 8, 2024
 *      Author: konrad
 */

#ifndef DRONE_H_
#define DRONE_H_

#include <stdint.h>
#include "queue.h"

typedef float THRUST_MAP_t[10][2];

void drone_queue_control();
float interpolate(float throttle);
float map(float x, int in_min, int in_max, int out_min, int out_max);
const THRUST_MAP_t* get_thrust_map(void);
const uint32_t get_rc_channel_max(void);
const uint32_t get_rc_channel_mid(void);
const uint32_t get_rc_channel_min(void);
const uint32_t get_motor_edf_max(void);
const uint32_t get_motor_edf_min(void);
const uint32_t get_motor_max(void);
const uint32_t get_motor_min(void);
Queue_t* get_throttle_queue_rc(void);
Queue_t* get_pitch_queue_rc(void);
Queue_t* get_roll_queue_rc(void);
Queue_t* get_yaw_queue_rc(void);
Queue_t* get_gear_queue_rc(void);
Queue_t* get_speed_queue_rc(void);
Queue_t* get_queues_rc();

#endif /* DRONE_H_ */
