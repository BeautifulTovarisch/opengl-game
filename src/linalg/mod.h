#ifndef LINALG_H
#define LINALG_H

#include <math.h>
#include <stdlib.h>

#include <cblas.h>

#include "../logger/mod.h"

#define ROWS 4
#define COLS 4

typedef struct Vector {
  float x;
  float y;
  float z;
  float w;
} Vector;

/* NOTE :: Using row-order index of 1d array
 * mat[row][col] = mat[COLS * row + col]
 *
 * [0][0] = mat[4 * 0 + 0] = mat[0]
 * [1][1] = mat[4 * 1 + 1] = mat[5]
 * [2][2] = mat[4 * 2 + 2] = mat[10]
 * [3][3] = mat[4 * 3 + 3] = mat[15]
 *
 */
typedef float Mat4[ROWS * COLS];

float To_Rad(float deg);
Vector To_Quat(Vector v, float angle);

Vector Vector_Add(Vector v1, Vector v2);
Vector Vector_Sub(Vector v1, Vector v2);
Vector Vector_Norm(Vector v);
Vector Vector_Scale(Vector v, float scalar);
Vector Vector_Cross(Vector v1, Vector v2);

float Vector_Dot(Vector v1, Vector v2);
float Vector_Mag(Vector v);

void Matrix_Ident(Mat4 mat);
void Matrix_Ortho(float left, float right, float bottom, float top, float near,
                  float far, Mat4 mat);
void Matrix_Mult(Mat4 m1, Mat4 out);
void Matrix_Trans(Vector v, Mat4 mat);
void Matrix_Scale(Vector v, Mat4 mat);

Vector Quat_Rot(Vector q, Vector v);
Vector Quat_Mult(Vector a, Vector b);
Vector Quat_Norm(Vector q);
Vector Quat_Inverse(Vector a);

#endif
