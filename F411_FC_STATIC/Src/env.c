/*
 * env.c
 *
 *  Created on: Feb 1, 2024
 *      Author: konrad
 */

#include "env.h"

//constants
const float geo_g = 9.80665;     			// geopotential gravity m/s^2
const float mag_declination = 7.0; 			// positive degrees at location
const float mag_field_strength = 50.378; 	// uT - mikro Tesla at location
const float ro = 1.225; 					// air density kg/m

inline const float* get_magnetic_declination() {
	return &mag_declination;
}

inline const float get_geo_g() {
	return geo_g;
}

inline const float get_ro() {
	return ro;
}
