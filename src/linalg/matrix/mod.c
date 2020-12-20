#include "mod.h"

// TODO :: Write utility to map 2d to 1d array indices

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

void print_matrix(Mat4 m) {
  for (int i = 0; i < 16; i++) {
    if (i % 4 == 0) {
      printf("\n");
    }
    printf("%f ", m[i]);
  }
  printf("\n");
}

/* Projections and Frustra
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
void M_Ident(Mat4 m) {
  zero_array(m);

  m[0] = 1;
  m[5] = 1;
  m[10] = 1;
  m[15] = 1;
}

// Orthographic projection
// Projects visible coordinates without perspective
void M_Ortho(float left, float right, float bottom, float top, float near,
             float far, Mat4 m) {

  zero_array(m);

  //  Guard against divide by zero
  if (!(right - left) || !(top - bottom) || !(far - near)) {
    return;
  }

  m[0] = 2.0f / (right - left);
  m[5] = 2.0f / (top - bottom);
  m[10] = -2.0f / (far - near);
  m[12] = -(right + left) / (right - left);
  m[13] = -(top + bottom) / (top - bottom);
  m[14] = -(far + near) / (far - near);
  m[15] = 1.0f;
}

// Frustrum
void M_Frust(float left, float right, float bottom, float top, float near,
             float far, Mat4 m) {
  zero_array(m);

  if (left == right || top == bottom || near == far) {
    return;
  }

  m[0] = (2.0f * near) / (right - left);
  m[5] = (2.0f * near) / (top - bottom);
  m[8] = (right + left) / (right - left);
  m[9] = (top + bottom) / (top - bottom);
  m[10] = (-(far + near)) / (far - near);
  m[11] = -1.0f;
  m[14] = (-2.0f * far * near) / (far - near);
}

// FOV, Aspect, and depth projection
void M_Perspective(float fov, float aspect, float near, float far, Mat4 m) {
  float y_max = near * tanf(fov * M_PI / 360.0f);
  float x_max = y_max * aspect;

  M_Frust(-x_max, x_max, -y_max, y_max, near, far, m);
}

/* Matrix operations
 * -----------------------------------------------------------------------------
 */

void M_Mult(Mat4 m1, Mat4 m2) {
  Mat4 result = {0};

  cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 4, 4, 4, 1.0, m1, 4,
              m2, 4, 0.0f, result, 4);

  copy_matrix(result, m2);
}

/* Apply translation to matrix
 * ---------
 * |1 0 0 x|
 * |0 1 0 y|
 * |0 0 1 z|
 * |0 0 0 1|
 * ---------
 */
void M_Trans(Vector v, Mat4 m) {
  Mat4 trans;

  M_Ident(trans);

  trans[3] = v.x;
  trans[7] = v.y;
  trans[11] = v.z;

  M_Mult(m, trans);

  copy_matrix(trans, m);
};

/* Apply scale operation to matrix
 * ---------
 * |x 0 0 0|
 * |0 y 0 0|
 * |0 0 z 0|
 * |0 0 0 1|
 * ---------
 */
void M_Scale(Vector v, Mat4 m) {
  Mat4 scale = {0};

  scale[0] = v.x;
  scale[5] = v.y;
  scale[10] = v.z;
  scale[15] = 1;

  M_Mult(m, scale);

  copy_matrix(scale, m);
}
