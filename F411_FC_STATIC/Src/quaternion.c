#include "quaternion.h"
#include <tgmath.h>
#include <stdio.h>

EulerAngle_t ea_pre;

void quaternion_normalize(Quaternion_t *q) {
	float norm;

	norm = math_inv_sqrt(
			q->q0 * q->q0 + q->q1 * q->q1 + q->q2 * q->q2 + q->q3 * q->q3);
	q->q0 *= norm;
	q->q1 *= norm;
	q->q2 *= norm;
	q->q3 *= norm;
}

/*
 * Quaternion Multiplay - qo = qa * qb
 * note - qo can be different from qa/qb, or the same as qa/qb
 */
void quaternion_multiply(const Quaternion_t *qa, const Quaternion_t *qb,
		Quaternion_t *qo) {
	float q0, q1, q2, q3;

	q0 = qa->q0 * qb->q0 - qa->q1 * qb->q1 - qa->q2 * qb->q2 - qa->q3 * qb->q3;
	q1 = qa->q0 * qb->q1 + qa->q1 * qb->q0 + qa->q2 * qb->q3 - qa->q3 * qb->q2;
	q2 = qa->q0 * qb->q2 - qa->q1 * qb->q3 + qa->q2 * qb->q0 + qa->q3 * qb->q1;
	q3 = qa->q0 * qb->q3 + qa->q1 * qb->q2 - qa->q2 * qb->q1 + qa->q3 * qb->q0;
	qo->q0 = q0;
	qo->q1 = q1;
	qo->q2 = q2;
	qo->q3 = q3;
}

/*
 * This function calculate the vector rotaton via quaternion
 * qr - rotation quaternion
 * qv - vector to rotate (qv->q0 = 0)
 * qo - output vector (qo->q0 = 0)
 * qo = qr' * qv * qr
 */
void quaternion_rotate(const Quaternion_t *qr, const Quaternion_t *qv,
		Quaternion_t *qo) {
	float q0q0, q1q1, q2q2, q3q3;
	float dq0, dq1, dq2;
	float dq1q2, dq1q3, dq0q2, dq0q3;
	float dq0q1, dq2q3;

	q0q0 = qr->q0 * qr->q0;
	q1q1 = qr->q1 * qr->q1;
	q2q2 = qr->q2 * qr->q2;
	q3q3 = qr->q3 * qr->q3;
	dq0 = 2 * qr->q0;
	dq1 = 2 * qr->q1;
	dq2 = 2 * qr->q2;
	dq1q2 = dq1 * qr->q2;
	dq1q3 = dq1 * qr->q3;
	dq0q2 = dq0 * qr->q2;
	dq0q3 = dq0 * qr->q3;
	dq0q1 = dq0 * qr->q1;
	dq2q3 = dq2 * qr->q3;

	qo->q0 = 0;
	qo->q1 = (q0q0 + q1q1 - q2q2 - q3q3) * qv->q1 + (dq1q2 + dq0q3) * qv->q2
			+ (dq1q3 - dq0q2) * qv->q3;
	qo->q2 = (dq1q2 - dq0q3) * qv->q1 + (q0q0 + q2q2 - q1q1 - q3q3) * qv->q2
			+ (dq0q1 + dq2q3) * qv->q3;
	qo->q3 = (dq0q2 + dq1q3) * qv->q1 + (dq2q3 - dq0q1) * qv->q2
			+ (q0q0 + q3q3 - q1q1 - q2q2) * qv->q3;
}

void quaternion_conjugate(const Quaternion_t *qa, Quaternion_t *qo) {
	qo->q0 = qa->q0;
	qo->q1 = -qa->q1;
	qo->q2 = -qa->q2;
	qo->q3 = -qa->q3;
}

void quaternion_scale(const float scale, Quaternion_t *qo) {
	qo->q0 *= scale;
	qo->q1 *= scale;
	qo->q2 *= scale;
	qo->q3 *= scale;
}

void quaternion_add(const Quaternion_t *q1, const Quaternion_t *q2,
		Quaternion_t *qo) {
	qo->q0 = q1->q0 + q2->q0;
	qo->q1 = q1->q1 + q2->q1;
	qo->q2 = q1->q2 + q2->q2;
	qo->q3 = q1->q3 + q2->q3;
}

/*
 * Convert Quaternion to Euler Angle
 */
void quaternion_to_euler(const Quaternion_t *qr, EulerAngle_t *ea) {
	float q0q0, q1q1, q2q2, q3q3;
	float dq0, dq1, dq2;
	float dq1q3, dq0q2, dq1q2;
	float dq0q1, dq2q3, dq0q3;

	q0q0 = qr->q0 * qr->q0;
	q1q1 = qr->q1 * qr->q1;
	q2q2 = qr->q2 * qr->q2;
	q3q3 = qr->q3 * qr->q3;
	dq0 = 2 * qr->q0;
	dq1 = 2 * qr->q1;
	dq2 = 2 * qr->q2;
	dq1q2 = dq1 * qr->q2;
	dq1q3 = dq1 * qr->q3;
	dq0q2 = dq0 * qr->q2;
	dq0q3 = dq0 * qr->q3;
	dq0q1 = dq0 * qr->q1;
	dq2q3 = dq2 * qr->q3;

	ea->pitch_x = atan2f(dq0q1 + dq2q3, q0q0 + q3q3 - q1q1 - q2q2);
	ea->roll_y = asinf(dq0q2 - dq1q3);
	ea->yaw_z = atan2f(dq1q2 + dq0q3, q0q0 + q1q1 - q2q2 - q3q3);
}
