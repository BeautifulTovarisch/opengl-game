#ifndef VECTOR_H
#define VECTOR_H

#include <cblas.h>
#include <math.h>

#include <stdio.h>

typedef struct Vector {
  float x;
  float y;
  float z;
} Vector;

Vector Vector_Add(Vector *v1, Vector *v2);
Vector Vector_Sub(Vector *v1, Vector *v2);
Vector Vector_Norm(Vector *v);
Vector Vector_Scale(Vector *v, float scalar);
Vector Vector_Cross(Vector *v1, Vector *v2);

float Vector_Dot(Vector *v1, Vector *v2);
float Vector_Mag(Vector *v);

#endif
