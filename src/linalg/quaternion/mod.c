#include "mod.h"

Quaternion array_to_quat(float vec[]) {
  return (Quaternion){.i = vec[0], .j = vec[1], .k = vec[2], .w = vec[3]};
}

// A+B
Quaternion q_add(Quaternion a, Quaternion b) {
  float v1[] = {a.i, a.j, a.k, a.w};
  float v2[] = {b.i, b.j, b.k, b.w};

  cblas_saxpy(4, 1, v1, 1, v2, 1);

  return array_to_quat(v2);
};

// A-B
Quaternion q_sub(Quaternion a, Quaternion b) {
  float v1[] = {a.i, a.j, a.k, a.w};
  float v2[] = {b.i, b.j, b.k, b.w};

  cblas_saxpy(4, -1, v2, 1, v1, 1);

  return array_to_quat(v1);
};

// A*b
Quaternion q_scale(Quaternion q, float scalar) {
  float vec[] = {q.i, q.j, q.k, q.w};

  cblas_sscal(4, scalar, vec, 1);

  return array_to_quat(vec);
};

// Q^e
Quaternion q_pow(Quaternion q, float e) {
  float theta = 2.0f * acosf(q.w);
  Vector axis = V_Norm((Vector){q.i, q.j, q.k});

  float sin_t = sinf(e * theta * 0.5f);

  return (Quaternion){.i = axis.x * sin_t,
                      .j = axis.y * sin_t,
                      .k = axis.z * sin_t,
                      .w = cosf(e * theta * 0.5f)};
}

// A·B
float q_dot(Quaternion a, Quaternion b) {
  float v1[] = {a.i, a.j, a.k, a.w};
  float v2[] = {b.i, b.j, b.k, b.w};

  return cblas_sdot(4, v1, 1, v2, 1);
}

// |Q|
float q_mag(Quaternion q) {
  return cblas_snrm2(4, (float[]){q.i, q.j, q.k, q.w}, 1);
}

// |Q|^2
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

  return mq > 0 ? q_scale(q, 1 / mq) : (Quaternion){0};
}

Quaternion Q_Conj(Quaternion q) { return (Quaternion){-q.i, -q.j, -q.k, q.w}; }

Quaternion Q_Scale(Quaternion q, float scalar) { return q_scale(q, scalar); }

Quaternion Q_Inverse(Quaternion q) {
  float m_sq = q_mag_sq(q);

  if (!m_sq) {
    return (Quaternion){0};
  }

  return m_sq == 1 ? Q_Conj(q) : q_scale(Q_Conj(q), 1.0f / m_sq);
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

Quaternion Q_Mix(Quaternion a, Quaternion b, float t) {
  return q_add(q_scale(a, 1.0f - t), q_scale(b, t));
}

Quaternion Q_Nlerp(Quaternion a, Quaternion b, float t) {
  return Q_Norm(Q_Add(a, q_scale(q_sub(b, a), t)));
}

Quaternion QSlerp(Quaternion a, Quaternion b, float t) {
  if (fabsf(q_dot(a, b)) > 1.0f - Q_EPSILON) {
    return Q_Nlerp(a, b, t);
  }

  Quaternion delta = Q_Mult(Q_Inverse(a), b);
  return Q_Norm(Q_Mult(q_pow(delta, t), a));
}

Quaternion Q_LookAt(Vector direction, Vector up) {
  Vector d = V_Norm(direction);
  Vector u = V_Norm(up);
  Vector r = V_Cross(u, d);

  u = V_Cross(d, r);

  Quaternion world_to_obj = Q_From_Vectors((Vector){0, 0, 1}, d);
  Vector obj_up = Q_MultV(world_to_obj, (Vector){0, 1, 0});

  Quaternion obj_up_to_dup = Q_From_Vectors(obj_up, u);

  return Q_Norm(Q_Mult(world_to_obj, obj_up_to_dup));
}

Vector Q_Axis(Quaternion q) { return V_Norm((Vector){q.i, q.j, q.k}); }

Vector Q_MultV(Quaternion q, Vector v) {
  Vector q_vec = {q.i, q.j, q.k};

  Vector a = V_Scale(q_vec, 2.0f * V_Dot(q_vec, v));
  Vector b = V_Scale(v, q.w * q.w - V_Dot(q_vec, q_vec));
  Vector c = V_Scale(V_Cross(q_vec, v), 2.0f * q.w);

  return V_Add(a, V_Add(b, c));
}

int Q_Eq(Quaternion a, Quaternion b) {
  return fabs(a.j - b.j) <= Q_EPSILON && fabs(a.j - b.j) <= Q_EPSILON &&
         fabs(a.k - b.k) <= Q_EPSILON && fabs(a.w - b.w) <= Q_EPSILON;
}

float Q_Angle(Quaternion q) { return 2.0f * acosf(q.w); }
float Q_Dot(Quaternion a, Quaternion b) { return q_dot(a, b); }

/* Dual Quaternions
 * -----------------------------------------------------------------------------
 */

DualQuat DQ_From_Quaternions(Quaternion r, Quaternion d) {
  return (DualQuat){r, d};
}

DualQuat DQ_From_Translation(Quaternion r, Vector t) {
  Quaternion real = Q_Norm(r);
  Quaternion dual = (Quaternion){t.x, t.y, t.z, 0};

  return (DualQuat){.real = real, .dual = Q_Scale(Q_Mult(dual, real), 0.5f)};
}

DualQuat DQ_Conj(DualQuat dq) {
  return (DualQuat){.real = Q_Conj(dq.real), .dual = Q_Conj(dq.dual)};
}

DualQuat DQ_Mult(DualQuat a, DualQuat b) {
  return (DualQuat){.real = Q_Mult(a.real, b.real),
                    .dual =
                        Q_Add(Q_Mult(a.real, b.dual), Q_Mult(a.dual, b.real))};
}

DualQuat DQ_Norm(DualQuat dq) {
  const float qm = q_mag(dq.real);

  return qm > 0 ? (DualQuat){.real = Q_Scale(dq.real, 1.0f / qm),
                             .dual = Q_Scale(dq.dual, 1.0f / qm)}
                : (DualQuat){0};
}

DualQuat DQ_Scale(DualQuat dq, float scl) {
  return (DualQuat){.real = Q_Scale(dq.real, scl),
                    .dual = Q_Scale(dq.dual, scl)};
}

Vector DQ_GetRotation(DualQuat dq) {
  return (Vector){
      .x = dq.real.i, .y = dq.real.j, .z = dq.real.k, .w = dq.real.w};
}

Vector DQ_GetTranslation(DualQuat dq) {
  // Inverse operation of storing translation as dual component
  Quaternion dual = Q_Mult(Q_Conj(dq.real), q_scale(dq.dual, 2.0f));

  return (Vector){dual.i, dual.j, dual.k, 0};
}

float DQ_Dot(DualQuat a, DualQuat b) { return q_dot(a.real, b.real); }
