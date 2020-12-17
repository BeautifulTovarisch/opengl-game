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

void print_matrix(Mat4 mat) {
  for (int i = 0; i < 16; i++) {
    if (i % 4 == 0) {
      printf("\n");
    }
    printf("%f ", mat[i]);
  }
  printf("\n");
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

  zero_array(mat);

  //  Guard against divide by zero
  if (!(right - left) || !(top - bottom) || !(far - near)) {
    return;
  }

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

/* Apply translation to matrix
 * ---------
 * |1 0 0 x|
 * |0 1 0 y|
 * |0 0 1 z|
 * |0 0 0 1|
 * ---------
 */
void M_Trans(Vector v, Mat4 mat) {
  Mat4 trans;

  M_Ident(trans);

  trans[3] = v.x;
  trans[7] = v.y;
  trans[11] = v.z;

  M_Mult(mat, trans);

  copy_matrix(trans, mat);
};

/* Apply scale operation to matrix
 * ---------
 * |x 0 0 0|
 * |0 y 0 0|
 * |0 0 z 0|
 * |0 0 0 1|
 * ---------
 */
void M_Scale(Vector v, Mat4 mat) {
  Mat4 scale = {0};

  scale[0] = v.x;
  scale[5] = v.y;
  scale[10] = v.z;
  scale[15] = 1;

  M_Mult(mat, scale);

  copy_matrix(scale, mat);
}
