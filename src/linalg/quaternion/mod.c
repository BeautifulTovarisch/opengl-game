#include "mod.h"

Quaternion array_to_quat(float vec[]) {
  return (Quaternion){.i = vec[0], .j = vec[1], .k = vec[2], .w = vec[3]};
}

Quaternion q_add(Quaternion a, Quaternion b) {
  float v1[] = {a.i, a.j, a.k, a.w};
  float v2[] = {b.i, b.j, b.k, b.w};

  cblas_saxpy(4, 1, v1, 1, v2, 1);

  return array_to_quat(v2);
};

Quaternion q_sub(Quaternion a, Quaternion b) {
  float v1[] = {a.i, a.j, a.k, a.w};
  float v2[] = {b.i, b.j, b.k, b.w};

  cblas_saxpy(4, -1, v2, 1, v1, 1);

  return array_to_quat(v1);
};

Quaternion q_scale(Quaternion q, float scalar) {
  float vec[] = {q.i, q.j, q.k, q.w};

  cblas_sscal(4, scalar, vec, 1);

  return array_to_quat(vec);
};

float q_dot(Quaternion a, Quaternion b) {
  float v1[] = {a.i, a.j, a.k, a.w};
  float v2[] = {b.i, b.j, b.k, b.w};

  return cblas_sdot(4, v1, 1, v2, 1);
}

float q_mag(Quaternion q) {
  return cblas_snrm2(4, (float[]){q.i, q.j, q.k, q.w}, 1);
}

float q_mag_sq(Quaternion q) { return q_dot(q, q); }

/* Quaternions
 * -----------------------------------------------------------------------------
 */

Quaternion Q_Add(Quaternion a, Quaternion b) { return q_add(a, b); }
Quaternion Q_Sub(Quaternion a, Quaternion b) { return q_sub(a, b); }

// Equivalent to: (w1*w2 - (v1 dot v2), w1v2 + w2v1 + (v1 cross v2))
Quaternion Q_Mult(Quaternion q1, Quaternion q2) {
  return (Quaternion){
      .i = q1.w * q2.i + q1.i * q2.w + q1.j * q2.k - q1.k * q2.j,
      .j = q1.w * q2.j - q1.i * q2.k + q1.j * q2.w + q1.k * q2.i,
      .k = q1.w * q2.k + q1.i * q2.j - q1.j * q2.i + q1.k * q2.w,
      .w = q1.w * q2.w - q1.i * q2.i - q1.j * q2.j - q1.k * q2.k};
}

Quaternion Q_Norm(Quaternion q) {
  float mq = q_mag(q);

  if (mq > 0) {
    return q_scale(q, 1 / mq);
  }

  return (Quaternion){0};
}

Quaternion Q_Scale(Quaternion q, float scalar) { return q_scale(q, scalar); }

Quaternion Q_Invert(Quaternion q) { return q_scale(q, -1.0f); }

Quaternion Q_Inverse(Quaternion q) {
  float scalar = 1 / (q.w * q.w + q.i * q.i + q.j * q.j + q.k * q.k);

  return (Quaternion){.i = -q.i * scalar,
                      .j = -q.j * scalar,
                      .k = -q.k * scalar,
                      .w = q.w * scalar};
}

/* Axis Angle -> Quaternion conversion
 * NOTE :: angle (Θ) must be in radians
 */
Quaternion Q_From_Axis(Vector v, float angle) {
  Vector norm = V_Norm(v);
  float sin_theta = sin(angle * 0.5f);

  return (Quaternion){.i = norm.x * sin_theta,
                      .j = norm.y * sin_theta,
                      .k = norm.z * sin_theta,
                      .w = cos(angle * 0.5f)};
}

Quaternion Q_From_Vectors(Vector from, Vector to) {
  Vector f = V_Norm(from);
  Vector t = V_Norm(to);

  // F = T
  if (V_Eq(f, t)) {
    return (Quaternion){0, 0, 0, 1};
  }

  Vector ortho = {1, 0, 0};

  // F = -T
  if (V_Eq(f, V_Scale(t, -1.0f))) {
    ortho = fabsf(f.y) < fabsf(f.x) ? (Vector){0, 1, 0} : ortho;
    ortho = fabsf(f.z) < fabs(f.y) && fabs(f.z) < fabsf(f.x) ? (Vector){0, 0, 1}
                                                             : ortho;

    // Q = cos(0), |F⨯O|
    return Q_From_Axis(V_Norm(V_Cross(f, ortho)), 0);
  }

  // Create half-vector between From and To
  Vector half = V_Norm(V_Add(f, t));

  // Q = cos(F·H), F⨯H
  return Q_From_Axis(V_Cross(f, half), V_Dot(f, half));
}

Vector Q_Axis(Quaternion q) { return V_Norm((Vector){q.i, q.j, q.k}); };

int Q_Eq(Quaternion a, Quaternion b) {
  return fabs(a.j - b.j) <= Q_EPSILON && fabs(a.j - b.j) <= Q_EPSILON &&
         fabs(a.k - b.k) <= Q_EPSILON && fabs(a.w - b.w) <= Q_EPSILON;
}

float Q_Angle(Quaternion q) { return 2.0f * acosf(q.w); }

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
