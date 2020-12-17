#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>
#include <stdlib.h>

#include <cblas.h>

#include "../vector/mod.h"

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
Quaternion Q_Create(Vector v, float angle);
Quaternion Q_Inverse(Quaternion a);

DualQuat DQ_Create(Quaternion r, Vector t);
DualQuat DQ_Scale(DualQuat dq, float scl);
DualQuat DQ_Norm(DualQuat dq);

#endif
