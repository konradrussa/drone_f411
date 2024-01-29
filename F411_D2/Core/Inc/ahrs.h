#ifndef AHRS_H
#define AHRS_H

#include <stdint.h>
#include "ahrs_common.h"

void ahrs_fusion_agm(AxesRaw_t *accel, AxesRaw_t *gyro, AxesRaw_t *mag,
		AhrsState_t *ahrs);

#endif // AHRS_H

