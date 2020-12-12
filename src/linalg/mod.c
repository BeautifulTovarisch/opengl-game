#include "mod.h"

// TODO :: Write utility to map 2d to 1d array indices
// TODO :: Consider combining vector + quaternion functions

// Utility to guard against unintialized matrices
void zero_array(Mat4 mat) {
  for (int i = 0; i < ROWS * COLS; i++) {
    mat[i] = 0.0;
  }
}

// M in copied to matrix out
void copy_matrix(Mat4 in, Mat4 out) {
  for (int i = 0; i < ROWS * COLS; i++) {
    out[i] = in[i];
  }
}

Vector array_to_vec(float vec[]) {
  return (Vector){.x = vec[0], .y = vec[1], .z = vec[2], .w = vec[3]};
}

void print_matrix(Mat4 mat) {
  for (int i = 0; i < 16; i++) {
    if (i % 4 == 0) {
      printf("\n");
    }
    printf("%f ", mat[i]);
  }
  printf("\n");
}

float vector_mag(Vector v, float len) {
  float vec[] = {v.x, v.y, v.z, v.w};

  // len determines how many components are included (e.g 3 -> xyz ignoring w)
  return cblas_snrm2(len, vec, 1);
}

/* Utilities
 * -----------------------------------------------------------------------------
 */
float To_Rad(float deg) { return deg * ((float)M_PI / 180); }

/* Axis Angle -> Quaternion conversion
 * NOTE :: angle (Θ) must be in radians
 */
Vector To_Quat(Vector v, float angle) {
  return (Vector){.x = v.x * sin(angle * 0.5f),
                  .y = v.y * sin(angle * 0.5f),
                  .z = v.z * sin(angle * 0.5f),
                  .w = cos(angle * 0.5f)};
}

/* Vectors
 * -----------------------------------------------------------------------------
 */
Vector V_Add(Vector a, Vector b) {
  float v1[] = {a.x, a.y, a.z};
  float v2[] = {b.x, b.y, b.z};

  cblas_saxpy(3, 1, v1, 1, v2, 1);

  return array_to_vec(v2);
}

Vector V_Scale(Vector v, float scalar) {
  float vec[] = {v.x, v.y, v.z};

  cblas_sscal(3, scalar, vec, 1);

  return array_to_vec(vec);
}

Vector V_Sub(Vector a, Vector b) {
  float v1[] = {a.x, a.y, a.z};
  float v2[] = {b.x, b.y, b.z};

  // Scalar is -1 here . a + (-b)
  cblas_saxpy(3, -1, v1, 1, v2, 1);

  return array_to_vec(v2);
}

float V_Dot(Vector a, Vector b) {
  float v1[] = {a.x, a.y, a.z};
  float v2[] = {b.x, b.y, b.z};

  return cblas_sdot(3, v1, 1, v2, 1);
};

Vector V_Norm(Vector v) {
  // Divide by magnitude
  return V_Scale(v, 1.0 / vector_mag(v, 3));
};

Vector V_Cross(Vector a, Vector b) {
  return (Vector){.x = (a.y * b.z) - (a.z * b.y),
                  .y = (a.z * b.x) - (a.x * b.z),
                  .z = (a.x * b.y) - (a.y * b.x)};
}

/* Matrices
 * -----------------------------------------------------------------------------
 */

/* Identity M
 * ---------
 * |1 0 0 0|
 * |0 1 0 0|
 * |0 0 1 0|
 * |0 0 0 1|
 * ---------
 */
void M_Ident(Mat4 mat) {
  zero_array(mat);

  mat[0] = 1;
  mat[5] = 1;
  mat[10] = 1;
  mat[15] = 1;
}

// Orthographic projection
// Projects visible coordinates without perspective
void M_Ortho(float left, float right, float bottom, float top, float near,
             float far, Mat4 mat) {

  //  Guard against divide by zero
  if (!(right - left) || !(top - bottom) || !(far - near)) {
    Log(ERROR, "Expression resulting in division by zero.");
    return;
  }

  zero_array(mat);

  mat[0] = 2.0f / (right - left);
  mat[3] = -(right + left) / (right - left);
  mat[5] = 2.0f / (top - bottom);
  mat[7] = -(top + bottom) / (top - bottom);
  mat[10] = -2.0f / (far - near);
  mat[11] = -(far + near) / (far - near);
  mat[15] = 1.0f;
}

void M_Mult(Mat4 m1, Mat4 m2) {
  Mat4 result = {0};

  cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 4, 4, 4, 1.0, m1, 4,
              m2, 4, 0.0f, result, 4);

  copy_matrix(result, m2);
}

// Apply translation to matrix
void M_Trans(Vector v, Mat4 mat) {
  Mat4 trans;

  M_Ident(trans);

  trans[3] = v.x;
  trans[7] = v.y;
  trans[11] = v.z;

  M_Mult(mat, trans);

  copy_matrix(trans, mat);
};

// Apply scale operation to matrix
void M_Scale(Vector v, Mat4 mat) {
  Mat4 scale = {0};

  scale[0] = v.x;
  scale[5] = v.y;
  scale[10] = v.z;
  scale[15] = 1;

  M_Mult(mat, scale);

  copy_matrix(scale, mat);
}

/* Quaternions
 * -----------------------------------------------------------------------------
 */

// TODO :: Examine correctness of algorithm
/* Vector Q_Rot(Vector q, Vector pos) { */
/*   Vector v1 = V_Scale(q, 2.0f * V_Dot(q, v)); */
/*   Vector v2 = V_Scale(v, q.w * q.w - V_Dot(q, q)); */
/*   Vector v3 = V_Scale(V_Cross(q, v), 2.0f * q.w); */

/*   return V_Add(V_Add(v1, v2), v3); */
/* } */

Vector Q_Mult(Vector q1, Vector q2) {
  return (Vector){.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
                  .y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
                  .z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w,
                  .w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z};
}

Vector Q_Scale(Vector q, float scl) {
  float vec[] = {q.x, q.y, q.z, q.w};

  cblas_sscal(4, scl, vec, 1);

  return array_to_vec(vec);
}

Vector Q_Norm(Vector q) {
  float vec[] = {q.x, q.y, q.z, q.w};
  const float mag = cblas_snrm2(4, vec, 1);

  if (mag > 0) {
    cblas_sscal(4, 1 / mag, vec, 1);

    return array_to_vec(vec);
  }

  return (Vector){0};
}

Vector Q_Inverse(Vector q) {
  float scalar = 1 / (q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);

  return (Vector){.x = -q.x * scalar,
                  .y = -q.y * scalar,
                  .z = -q.z * scalar,
                  .w = q.w * scalar};
}

float Q_Dot(Vector q1, Vector q2) {
  float v1[] = {q1.x, q1.y, q1.z, q1.w};
  float v2[] = {q2.x, q2.y, q2.z, q2.w};

  return cblas_sdot(4, v1, 1, v2, 1);
}

/* Dual Quaternions
 * -----------------------------------------------------------------------------
 */

DualQuat DQ_Create(Vector r, Vector t) {
  Vector real = Q_Norm(r);

  return (DualQuat){.real = real, .dual = Q_Scale(Q_Mult(t, real), 0.5f)};
}

DualQuat DQ_Scale(DualQuat dq, float scl) {
  return (DualQuat){.real = Q_Scale(dq.real, scl),
                    .dual = Q_Scale(dq.dual, scl)};
}

DualQuat DQ_Norm(DualQuat dq) {
  const float scl = 1.0f / vector_mag(dq.real, 4);

  return (DualQuat){.real = Q_Scale(dq.real, scl),
                    .dual = Q_Scale(dq.dual, scl)};
}
