/*
 * drone.c
 *
 *  Created on: Jan 8, 2024
 *      Author: konrad
 */

#include "drone.h"

//constants
const float drone_whole_mass = 4.5;
const float drone_front_area = 0.1; //0.3^2 + 0.01
const float drone_center_mass = 4.0; // drone mass kg with 2 EDF, center mass 4 kg and 6 side rotors each 100g
const float drone_motor_mass = 0.1;
const float l = 0.2; // 0.20 * 2 * sin(30) no-perpendicular distance to axes, Quad 500, 50 cm drone
const float Ixx = 0.024; // Moments of Inertia, calculations based on paper written by Randal Beard
const float Iyy = 0.032;
const float Izz = 0.04;

//Jsphere = 2*M*R^2/5 = 0.016		//2*4*0.10^2/5
//Jx = Jsphere + 2*l^2*m = 0.024	//Jsphere + 2*0.2^2*0.1
//Jy = Jsphere + 2*l^2*m = 0.032	//Jsphere + 2*0.2^2*0.2
//Jz = Jsphere + 4*l^2*m = 0.04 	//Jsphere + 2*0.2^2*0.2 + 2*0.2^2*0.1

const THRUST_MAP_t thrust_map = { { 0.1, 0.047 }, { 0.2, 0.063 },
		{ 0.3, 0.094 }, { 0.4, 0.125 }, { 0.5, 0.188 }, { 0.6, 0.25 }, { 0.7,
				0.38 }, { 0.8, 0.50 }, { 0.9, 0.75 }, { 1.0, 1.0 } }; //startup power of BlHeliS

//TPRYGS
const RADIO_MAP_t radio_map = { { 1096, 1502, 1925 }, { 1096, 1517, 1924 }, {
		1096, 1517, 1924 }, { 1096, 1518, 1924 }, { 1955, 1542, 1113 }, { 1925,
		1518, 1096 } }; // radio signal ranges

const uint32_t MOTOR_EDF_MAX = 2000;
const uint32_t MOTOR_EDF_MIN = 1400; //+21

const uint32_t MOTOR_MAX = 8400;
const uint32_t MOTOR_MIN = 4200; //+1000 ??? TODO check

const int thrust_length = sizeof(THRUST_MAP_t) / sizeof(thrust_map[0]);

inline float map(float x, int in_min, int in_max, int out_min, int out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float interpolate(float throttle) {
	int i = 1;
	float result = 0.0f, pos = 0.0f;
	for (; i < thrust_length; i++) {
		if (throttle <= thrust_map[i][0]) {
			continue;
		} else {
			break;
		}
	}
	pos = (throttle - thrust_map[i - 1][0])
			/ (thrust_map[i][0] - thrust_map[i - 1][0]);
	result = thrust_map[i - 1][1]
			+ (pos * (thrust_map[i][1] - thrust_map[i - 1][1]));
	return result;
}

inline const THRUST_MAP_t* get_thrust_map() {
	return &thrust_map;
}

inline const RADIO_MAP_t* get_radio_map() {
	return &radio_map;
}

inline const uint32_t get_motor_edf_max() {
	return MOTOR_EDF_MAX;
}

inline const uint32_t get_motor_edf_min() {
	return MOTOR_EDF_MIN;
}

inline const uint32_t get_motor_max() {
	return MOTOR_MAX;
}

inline const uint32_t get_motor_min() {
	return MOTOR_MIN;
}

inline const float get_drone_whole_mass() {
	return drone_whole_mass;
}

inline const float get_drone_front_area() {
	return drone_front_area;
}
