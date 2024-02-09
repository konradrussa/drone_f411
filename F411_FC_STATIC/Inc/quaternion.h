#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include <assert.h>
#include "basic_math.h"

void quaternion_normalize(Quaternion_t *q);
void quaternion_multiply(const Quaternion_t *qa, const Quaternion_t *qb,
		Quaternion_t *qo);
void quaternion_rotate(const Quaternion_t *qr, const Quaternion_t *qv,
		Quaternion_t *qo);
void quaternion_conjugate(const Quaternion_t *qa, Quaternion_t *qo);
void quaternion_scale(const float scale, Quaternion_t *qo);
void quaternion_add(const Quaternion_t *q1, const Quaternion_t *q2,
		Quaternion_t *qo);
void quaternion_to_euler(const Quaternion_t *qr, EulerAngle_t *ea);
void quaternion_from_euler(const EulerAngle_t *ea, Quaternion_t *qo);

float quaternion_angular_distance(const Quaternion_t *q1,
		const Quaternion_t *q2);

#endif /* _QUATERNION_H_ */
