#ifndef AHRS_H
#define AHRS_H

#include <stdint.h>
#include "ahrs_common.h"

float ahrs_get_longitudinal_direction(void);
void ahrs_fusion_agm(AxesRaw_t *accel, AxesRaw_t *gyro, AxesRaw_t *mag,
		AhrsState_t *ahrs);

#endif // AHRS_H

