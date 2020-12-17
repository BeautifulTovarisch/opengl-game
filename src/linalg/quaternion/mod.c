#include "mod.h"

/* Quaternions
 * -----------------------------------------------------------------------------
 */

float q_mag(Quaternion q) {
  return cblas_snrm2(4, (float[]){q.i, q.j, q.k, q.w}, 1);
}

Quaternion array_to_quat(float vec[]) {
  return (Quaternion){.i = vec[0], .j = vec[1], .k = vec[2], .w = vec[3]};
}

Quaternion Q_Add(Quaternion a, Quaternion b) {
  float v1[] = {a.i, a.j, a.k, a.w};
  float v2[] = {b.i, b.j, b.k, b.w};

  cblas_saxpy(4, 1, v1, 1, v2, 1);

  return array_to_quat(v2);
}

Quaternion Q_Sub(Quaternion a, Quaternion b) {
  float v1[] = {a.i, a.j, a.k, a.w};
  float v2[] = {b.i, b.j, b.k, b.w};
  // Scalar is -1 here . a + (-b)
  cblas_saxpy(4, -1, v2, 1, v1, 1);

  return array_to_quat(v1);
}

// Equivalent to: (w1*w2 - (v1 dot v2), w1v2 + w2v1 + (v1 cross v2))
Quaternion Q_Mult(Quaternion q1, Quaternion q2) {
  return (Quaternion){
      .i = q1.w * q2.i + q1.i * q2.w + q1.j * q2.k - q1.k * q2.j,
      .j = q1.w * q2.j - q1.i * q2.k + q1.j * q2.w + q1.k * q2.i,
      .k = q1.w * q2.k + q1.i * q2.j - q1.j * q2.i + q1.k * q2.w,
      .w = q1.w * q2.w - q1.i * q2.i - q1.j * q2.j - q1.k * q2.k};
}

Quaternion Q_Norm(Quaternion v) {
  float vec[] = {v.i, v.j, v.k, v.w};
  const float mag = cblas_snrm2(4, vec, 1);

  if (mag > 0) {
    cblas_sscal(4, 1 / mag, vec, 1);

    return array_to_quat(vec);
  }

  return (Quaternion){0};
}

Quaternion Q_Scale(Quaternion v, float scalar) {
  float vec[] = {v.i, v.j, v.k, v.w};

  cblas_sscal(4, scalar, vec, 1);

  return array_to_quat(vec);
}

/* Axis Angle -> Quaternion conversion
 * NOTE :: angle (Θ) must be in radians
 */
Quaternion Q_Create(Vector v, float angle) {
  return (Quaternion){.i = v.x * sin(angle * 0.5f),
                      .j = v.y * sin(angle * 0.5f),
                      .k = v.z * sin(angle * 0.5f),
                      .w = cos(angle * 0.5f)};
}

Quaternion Q_Inverse(Quaternion q) {
  float scalar = 1 / (q.w * q.w + q.i * q.i + q.j * q.j + q.k * q.k);

  return (Quaternion){.i = -q.i * scalar,
                      .j = -q.j * scalar,
                      .k = -q.k * scalar,
                      .w = q.w * scalar};
}

/* Dual Quaternions
 * -----------------------------------------------------------------------------
 */
DualQuat DQ_Create(Quaternion r, Vector t) {
  Quaternion real = Q_Norm(r);
  Quaternion dual = (Quaternion){t.x, t.y, t.z, 0};

  return (DualQuat){.real = real, .dual = Q_Scale(Q_Mult(dual, real), 0.5f)};
}

DualQuat DQ_Scale(DualQuat dq, float scl) {
  return (DualQuat){.real = Q_Scale(dq.real, scl),
                    .dual = Q_Scale(dq.dual, scl)};
}

DualQuat DQ_Norm(DualQuat dq) {
  const float scl = 1.0f / q_mag(dq.real);

  return (DualQuat){.real = Q_Scale(dq.real, scl),
                    .dual = Q_Scale(dq.dual, scl)};
}
