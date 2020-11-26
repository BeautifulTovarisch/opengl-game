#include "mod.h"

// TODO :: Consider mutating vectors directly rather than creating new structs

Vector array_to_vec(float vec[]) {
  return (Vector){.x = vec[0], .y = vec[1], .z = vec[2]};
}

Vector Vector_Add(Vector *a, Vector *b) {
  float v1[] = {a->x, a->y, a->z};
  float v2[] = {b->x, b->y, b->z};

  cblas_saxpy(3, 1, v1, 1, v2, 1);

  return array_to_vec(v2);
}

Vector Vector_Scale(Vector *v, float scalar) {
  float vec[] = {v->x, v->y, v->z};

  cblas_sscal(3, scalar, vec, 1);

  return array_to_vec(vec);
}

Vector Vector_Sub(Vector *a, Vector *b) {
  float v1[] = {a->x, a->y, a->z};
  float v2[] = {b->x, b->y, b->z};

  // Scalar is -1 here -> a + (-b)
  cblas_saxpy(3, -1, v1, 1, v2, 1);

  return array_to_vec(v2);
}

// TODO :: Consider whether to use complex or real dot product
float Vector_Dot(Vector *a, Vector *b) {
  float v1[] = {a->x, a->y, a->z};
  float v2[] = {b->x, b->y, b->z};

  return cblas_sdot(3, v1, 1, v2, 1);
};

float Vector_Mag(Vector *v) {
  float vec[] = {v->x, v->y, v->z};

  return cblas_snrm2(3, vec, 1);
}

Vector Vector_Norm(Vector *v) {
  // Divide by magnitude
  return Vector_Scale(v, 1.0 / Vector_Mag(v));
};

Vector Vector_Cross(Vector *a, Vector *b) {
  return (Vector){.x = (a->y * b->z) - (a->z * b->y),
                  .y = (a->z * b->x) - (a->x * b->z),
                  .z = (a->x * b->y) - (a->y * b->x)};
}
