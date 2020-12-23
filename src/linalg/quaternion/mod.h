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
Quaternion Q_Conj(Quaternion q);
Quaternion Q_Scale(Quaternion q, float scalar);
Quaternion Q_Inverse(Quaternion a);
Quaternion Q_From_Axis(Vector v, float angle);
Quaternion Q_From_Vectors(Vector from, Vector to);
Quaternion Q_Mix(Quaternion a, Quaternion b, float t);

Vector Q_Axis(Quaternion q);
Vector Q_MultV(Quaternion q, Vector v);

int Q_Eq(Quaternion a, Quaternion b);

float Q_Dot(Quaternion a, Quaternion b);
float Q_Angle(Quaternion q);

DualQuat DQ_From_Quaternions(Quaternion r, Quaternion d);
DualQuat DQ_From_Translation(Quaternion r, Vector t);

DualQuat DQ_Eq(DualQuat a, DualQuat b);
DualQuat DQ_Add(DualQuat a, DualQuat b);
DualQuat DQ_Conj(DualQuat dq);
DualQuat DQ_Mult(DualQuat a, DualQuat b);
DualQuat DQ_Norm(DualQuat dq);
DualQuat DQ_Scale(DualQuat dq, float scl);

Vector DQ_GetRotation(DualQuat dq);
Vector DQ_GetTranslation(DualQuat dq);

float DQ_Dot(DualQuat a, DualQuat b);

#endif
