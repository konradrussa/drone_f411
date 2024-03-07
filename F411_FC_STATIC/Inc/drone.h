/*
 * drone.h
 *
 *  Created on: Jan 8, 2024
 *      Author: konrad
 */

#ifndef DRONE_H_
#define DRONE_H_

#include <stdint.h>

typedef float THRUST_MAP_t[10][2];
typedef int RADIO_MAP_t[6][3];

float interpolate(float throttle);
float map(float x, int in_min, int in_max, int out_min, int out_max);
const THRUST_MAP_t* get_thrust_map(void);
const RADIO_MAP_t* get_radio_map(void);
const uint32_t get_motor_edf_max(void);
const uint32_t get_motor_edf_min(void);
const uint32_t get_motor_max(void);
const uint32_t get_motor_min(void);
const float get_drone_whole_mass(void);
const float get_drone_front_area(void);

#endif /* DRONE_H_ */
