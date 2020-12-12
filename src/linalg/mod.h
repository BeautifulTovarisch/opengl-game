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

typedef struct DualQuat {
  Vector real;
  Vector dual;
} DualQuat;

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

Vector V_Add(Vector v1, Vector v2);
Vector V_Sub(Vector v1, Vector v2);
Vector V_Norm(Vector v);
Vector V_Scale(Vector v, float scalar);
Vector V_Cross(Vector v1, Vector v2);

float V_Dot(Vector v1, Vector v2);
float V_Mag(Vector v);

void M_Ident(Mat4 mat);
void M_Ortho(float left, float right, float bottom, float top, float near,
             float far, Mat4 mat);
void M_Mult(Mat4 m1, Mat4 out);
void M_Trans(Vector v, Mat4 mat);
void M_Scale(Vector v, Mat4 mat);

Vector Q_Rot(Vector q, Vector v);
Vector Q_Mult(Vector a, Vector b);
Vector Q_Norm(Vector q);
Vector Q_Scale(Vector q, float scl);
Vector Q_Inverse(Vector a);

float Q_Dot(Vector q1, Vector q2);
float Q_Mag(Vector q);

DualQuat DQ_Create(Vector r, Vector t);

#endif
