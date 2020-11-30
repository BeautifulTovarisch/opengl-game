#include "mod.h"

// TODO :: Write utility to map 2d to 1d array indices

// Utility to guard against unintialized matrices
void zero_array(Mat4 mat) {
  for (int i = 0; i < ROWS * COLS; i++) {
    mat[i] = 0.0;
  }
}

// Matrix in copied to matrix out
void copy_matrix(Mat4 in, Mat4 out) {
  for (int i = 0; i < ROWS * COLS; i++) {
    out[i] = in[i];
  }
}

Vector array_to_vec(float vec[]) {
  return (Vector){.x = vec[0], .y = vec[1], .z = vec[2]};
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

/* Utilities
 * -----------------------------------------------------------------------------
 */
float To_Rad(float deg) { return deg * ((float)M_PI / 180); }

/* Vector -> Quaternion conversion
 * NOTE :: Rotation vector must be in radians.
 */
Vector To_Quat(Vector v) {
  float cosy = cos(v.z * 0.5f);
  float siny = sin(v.z * 0.5f);

  float cosp = cos(v.y * 0.5f);
  float sinp = sin(v.y * 0.5f);

  float cosr = cos(v.x * 0.5f);
  float sinr = sin(v.x * 0.5f);

  return (Vector){.x = sinr * cosp * cosy - cosr * sinp * siny,
                  .y = cosr * sinp * cosy + sinr * cosp * siny,
                  .z = cosr * cosp * siny - sinr * sinp * cosy,
                  .w = cosr * cosp * cosy + sinr * sinp * siny};
}

/* Vectors
 * -----------------------------------------------------------------------------
 */
Vector Vector_Add(Vector a, Vector b) {
  float v1[] = {a.x, a.y, a.z};
  float v2[] = {b.x, b.y, b.z};

  cblas_saxpy(3, 1, v1, 1, v2, 1);

  return array_to_vec(v2);
}

Vector Vector_Scale(Vector v, float scalar) {
  float vec[] = {v.x, v.y, v.z};

  cblas_sscal(3, scalar, vec, 1);

  return array_to_vec(vec);
}

Vector Vector_Sub(Vector a, Vector b) {
  float v1[] = {a.x, a.y, a.z};
  float v2[] = {b.x, b.y, b.z};

  // Scalar is -1 here . a + (-b)
  cblas_saxpy(3, -1, v1, 1, v2, 1);

  return array_to_vec(v2);
}

// TODO :: Consider whether to use complex or real dot product
float Vector_Dot(Vector a, Vector b) {
  float v1[] = {a.x, a.y, a.z};
  float v2[] = {b.x, b.y, b.z};

  return cblas_sdot(3, v1, 1, v2, 1);
};

float Vector_Mag(Vector v) {
  float vec[] = {v.x, v.y, v.z};

  return cblas_snrm2(3, vec, 1);
}

Vector Vector_Norm(Vector v) {
  // Divide by magnitude
  return Vector_Scale(v, 1.0 / Vector_Mag(v));
};

Vector Vector_Cross(Vector a, Vector b) {
  return (Vector){.x = (a.y * b.z) - (a.z * b.y),
                  .y = (a.z * b.x) - (a.x * b.z),
                  .z = (a.x * b.y) - (a.y * b.x)};
}

/* Matrices
 * -----------------------------------------------------------------------------
 */

/* Identity Matrix
 * ---------
 * |1 0 0 0|
 * |0 1 0 0|
 * |0 0 1 0|
 * |0 0 0 1|
 * ---------
 */
void Matrix_Ident(Mat4 mat) {
  zero_array(mat);

  mat[0] = 1;
  mat[5] = 1;
  mat[10] = 1;
  mat[15] = 1;
}

// Orthographic projection
// Projects visible coordinates without perspective
void Matrix_Ortho(float left, float right, float bottom, float top, float near,
                  float far, Mat4 mat) {

  //  Guard against divide by zero
  if (!(right - left) || !(top - bottom) || !(far - near)) {
    Log(ERROR, "Expression resulting in division by zero.");
    return;
  }

  zero_array(mat);

  mat[0] = 2.0f / (right - left);
  mat[5] = 2.0f / (top - bottom);
  mat[10] = -2.0f / (far - near);
  mat[12] = -(right + left) / (right - left);
  mat[13] = -(top + bottom) / (top - bottom);
  mat[14] = -(far + near) / (far - near);
  mat[15] = 1.0f;
}

// TODO :: Consider whether updating matrix directly rather than
// multiplying to combine transformations is viable

void Matrix_Mult(Mat4 m1, Mat4 m2) {
  Mat4 result = {0};

  cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 4, 4, 4, 1.0, m1, 4,
              m2, 4, 0.0f, result, 4);

  copy_matrix(result, m2);
}

// Apply translation to matrix
void Matrix_Trans(Vector v, Mat4 mat) {
  Mat4 trans;

  Matrix_Ident(trans);

  trans[3] = v.x;
  trans[7] = v.y;
  trans[11] = v.z;

  Matrix_Mult(mat, trans);

  copy_matrix(trans, mat);
};

// Apply scale operation to matrix
void Matrix_Scale(Vector v, Mat4 mat) {
  Mat4 scale = {0};

  // Construct a translation matrix
  scale[0] = v.x;
  scale[5] = v.y;
  scale[10] = v.z;
  scale[15] = 1;

  Matrix_Mult(mat, scale);

  copy_matrix(scale, mat);
}

/* Quaternions
 * -----------------------------------------------------------------------------
 */
Vector Quat_Rot(Vector q, Vector v) {
  Vector v1 = Vector_Scale(q, 2.0f * Vector_Dot(q, v));
  Vector v2 = Vector_Scale(v, q.w * q.w - Vector_Dot(q, q));
  Vector v3 = Vector_Scale(Vector_Cross(q, v), 2.0f * q.w);

  return Vector_Add(Vector_Add(v1, v2), v3);
}

Vector Quat_Mult(Vector a, Vector b) {
  return (Vector){.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
                  .y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
                  .z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
                  .w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z};
}

Vector Quat_Inverse(Vector q) {
  float scalar = 1 / (q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);

  return (Vector){.x = -q.x * scalar,
                  .y = -q.y * scalar,
                  .z = -q.z * scalar,
                  .w = q.w * scalar};
}
