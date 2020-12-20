#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>
#include <stdlib.h>

#include <cblas.h>

#include "../vector/mod.h"

#define Q_EPSILON 0.000001f

typedef struct Quaternion {
  float i;
  float j;
  float k;
  float w;
} Quaternion;

typedef struct DualQuat {
  Quaternion real;
  Quaternion dual;
} DualQuat;

Quaternion Q_Add(Quaternion q1, Quaternion q2);
Quaternion Q_Sub(Quaternion q1, Quaternion q2);
Quaternion Q_Mult(Quaternion a, Quaternion b);
Quaternion Q_Norm(Quaternion q);
Quaternion Q_Scale(Quaternion q, float scalar);
Quaternion Q_Inverse(Quaternion a);
Quaternion Q_From_Axis(Vector v, float angle);
Quaternion Q_From_Vectors(Vector from, Vector to);

Vector Q_Axis(Quaternion q);

int Q_Eq(Quaternion a, Quaternion b);

float Q_Dot(Quaternion q);
float Q_Angle(Quaternion q);

DualQuat DQ_Create(Quaternion r, Vector t);
DualQuat DQ_Scale(DualQuat dq, float scl);
DualQuat DQ_Norm(DualQuat dq);

#endif
