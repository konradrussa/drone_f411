#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include <assert.h>
#include "basic_math.h"

#define MAX_RAD    		3.14159 	//	180 degree
#define EPSILON 		(1e-4)

typedef struct Quaternion {
	float q0;			//scalar part
	float q1, q2, q3;	//vector part
} Quaternion_t;

typedef struct EulerAngle {
	float pitch_x, roll_y, yaw_z;
} EulerAngle_t;

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

#endif /* _QUATERNION_H_ */
