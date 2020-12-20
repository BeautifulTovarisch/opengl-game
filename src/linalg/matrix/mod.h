#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <cblas.h>

#include "../vector/mod.h"

#define ROWS 4
#define COLS 4

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

void M_Ident(Mat4 mat);
void M_Ortho(float left, float right, float bottom, float top, float near,
             float far, Mat4 mat);
void M_Frust(float left, float right, float bottom, float top, float near,
             float far, Mat4 mat);
void M_Mult(Mat4 m1, Mat4 out);
void M_Trans(Vector v, Mat4 mat);
void M_Scale(Vector v, Mat4 mat);

#endif
