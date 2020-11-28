#include "mod.h"

// TODO :: Write utility to map 2d to 1d array indices

// Utility to guard against unintialized matrices
void zero_array(Mat4 mat) {
  for (int i = 0; i < ROWS * COLS; i++) {
    mat[i] = 0.0;
  }
}

Vector array_to_vec(float vec[]) {
  return (Vector){.x = vec[0], .y = vec[1], .z = vec[2]};
}

/* Utilities
 * -----------------------------------------------------------------------------
 */
float To_Rad(float deg) { return deg * ((float)M_PI / 180); }

// NOTE :: Rotation vector must be in radians.
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

/* Vector
 * -----------------------------------------------------------------------------
 */
Vector Vector_Add(Vector *a, Vector *b) {
  float v1[] = {a->x, a->y, a->z};
  float v2[] = {b->x, b->y, b->z};

  cblas_saxpy(3, 1, v1, 1, v2, 1);

  return array_to_vec(v2);
}

Vector Vector_Scale(Vector *v, float scalar) {
  float vec[] = {v->x, v->y, v->z};

  cblas_sscal(3, scalar, vec, 1);

  return array_to_vec(vec);
}

Vector Vector_Sub(Vector *a, Vector *b) {
  float v1[] = {a->x, a->y, a->z};
  float v2[] = {b->x, b->y, b->z};

  // Scalar is -1 here -> a + (-b)
  cblas_saxpy(3, -1, v1, 1, v2, 1);

  return array_to_vec(v2);
}

// TODO :: Consider whether to use complex or real dot product
float Vector_Dot(Vector *a, Vector *b) {
  float v1[] = {a->x, a->y, a->z};
  float v2[] = {b->x, b->y, b->z};

  return cblas_sdot(3, v1, 1, v2, 1);
};

float Vector_Mag(Vector *v) {
  float vec[] = {v->x, v->y, v->z};

  return cblas_snrm2(3, vec, 1);
}

Vector Vector_Norm(Vector *v) {
  // Divide by magnitude
  return Vector_Scale(v, 1.0 / Vector_Mag(v));
};

Vector Vector_Cross(Vector *a, Vector *b) {
  return (Vector){.x = (a->y * b->z) - (a->z * b->y),
                  .y = (a->z * b->x) - (a->x * b->z),
                  .z = (a->x * b->y) - (a->y * b->x)};
}

/* Matrix
 * -----------------------------------------------------------------------------
 */

void Matrix_Rot(Mat4 mat, float rad, Vector v){

};

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

void Matrix_Mult(Mat4 a, Mat4 b){};
void Matrix_Scale(Mat4 mat, Vector v){};

void Matrix_Trans(Mat4 mat, Vector v) {
  Matrix_Ident(mat);

  mat[12] = v.x;
  mat[13] = v.y;
  mat[14] = v.z;
};

void Matrix_ScaleVec(Mat4 mat, Vector v){};