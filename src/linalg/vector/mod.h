#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <stdlib.h>

#include <cblas.h>

typedef struct Vector {
  float x;
  float y;
  float z;
  float w;
} Vector;

float To_Rad(float deg);

Vector V_Add(Vector v1, Vector v2);
Vector V_Sub(Vector v1, Vector v2);
Vector V_Norm(Vector v);
Vector V_Scale(Vector v, float scalar);
Vector V_Cross(Vector v1, Vector v2);

float V_Dot(Vector v1, Vector v2);

#endif
