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

Vector V_Add(Vector a, Vector b);
Vector V_Sub(Vector a, Vector b);
Vector V_Norm(Vector v);
Vector V_Scale(Vector v, float scalar);
Vector V_Cross(Vector a, Vector b);
Vector V_Project(Vector a, Vector b);
Vector V_Reject(Vector a, Vector b);
Vector V_Reflect(Vector a, Vector b);
Vector V_Lerp(Vector a, Vector b, float t, int normalize);

float V_Dot(Vector a, Vector b);
float V_Angle(Vector a, Vector b);

#endif
