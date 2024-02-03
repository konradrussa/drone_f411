#ifndef AHRS_H
#define AHRS_H

#include <stdint.h>
#include "ahrs_common.h"

const float ahrs_get_sampling_time(void);
float ahrs_get_longitudinal_direction(void);
void ahrs_fusion_agm(const AxesRaw_t *accel, const AxesRaw_t *gyro, const AxesRaw_t *mag,
		AhrsState_t *ahrs);

#endif // AHRS_H

