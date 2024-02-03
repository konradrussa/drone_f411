#include "ahrs.h"

// Sampling time of sensors
const float SENSOR_SAMPLING_TIME = 0.000625; //1600 Hz
int16_t gTHR;

const float ahrs_get_sampling_time() {
	return SENSOR_SAMPLING_TIME;
}
