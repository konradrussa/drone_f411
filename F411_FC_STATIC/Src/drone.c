/*
 * drone.c
 *
 *  Created on: Jan 8, 2024
 *      Author: konrad
 */

#include "drone.h"

//input variables/channels
static float throttle_s = 0.0, roll_s = 0.0, pitch_s = 0.0, yaw_s = 0.0,
		gear_s = 0.0, speed_s = 0.0;

static Queue_t queues_rc[6];

//constants
const float drone_whole_mass = 4.5;
const float front_area = 0.1; //0.3^2 + 0.01
const float drone_center_mass = 4.0; // drone mass kg with 2 EDF, center mass 4 kg and 6 side rotors each 100g
const float motor_mass = 0.1;
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

const uint32_t RC_CHANNELS_MAX = 1919;
const uint32_t RC_CHANNELS_MID = 1508;
const uint32_t RC_CHANNELS_MIN = 1097;

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

void drone_queue_control() {
	uint32_t data;
	if (queue_data_available(get_throttle_queue_rc())) {
		queue_dequeue(get_throttle_queue_rc(), &data);
		throttle_s = ((float) data / (float) get_rc_channel_max()); // normalize between 0 and 1
		throttle_s = interpolate(throttle_s);
	}
	if (queue_data_available(get_pitch_queue_rc())) {
		queue_dequeue(get_pitch_queue_rc(), &data);
		pitch_s = -map(data, get_rc_channel_min(), get_rc_channel_max(), -10,
				10);
	}
	if (queue_data_available(get_roll_queue_rc())) {
		queue_dequeue(get_roll_queue_rc(), &data);
		roll_s = -map(roll_s, get_rc_channel_min(), get_rc_channel_max(), -10,
				10);
	}
	if (queue_data_available(get_yaw_queue_rc())) {
		queue_dequeue(get_yaw_queue_rc(), &data);
		yaw_s = map(yaw_s, get_rc_channel_min(), get_rc_channel_max(), -180,
				180);
	}
	if (queue_data_available(get_gear_queue_rc())) {
		queue_dequeue(get_gear_queue_rc(), &data);
		gear_s = (float) data;
	}
	if (queue_data_available(get_speed_queue_rc())) {
		queue_dequeue(get_speed_queue_rc(), &data);
		speed_s = (float) data;
	}
}

inline Queue_t* get_throttle_queue_rc() {
	return &queues_rc[0];
}

inline Queue_t* get_pitch_queue_rc() {
	return &queues_rc[1];
}

inline Queue_t* get_roll_queue_rc() {
	return &queues_rc[2];
}

inline Queue_t* get_yaw_queue_rc() {
	return &queues_rc[3];
}

inline Queue_t* get_gear_queue_rc() {
	return &queues_rc[4];
}

inline Queue_t* get_speed_queue_rc() {
	return &queues_rc[5];
}

inline Queue_t* get_queues_rc() {
	return queues_rc;
}

inline const THRUST_MAP_t* get_thrust_map() {
	return &thrust_map;
}

inline const uint32_t get_rc_channel_max() {
	return RC_CHANNELS_MAX;
}

inline const uint32_t get_rc_channel_mid() {
	return RC_CHANNELS_MID;
}

inline const uint32_t get_rc_channel_min() {
	return RC_CHANNELS_MIN;
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

inline const float get_front_area() {
	return front_area;
}
