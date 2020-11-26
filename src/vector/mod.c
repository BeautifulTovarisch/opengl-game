#include "mod.h"

// TODO :: Consider mutating vectors directly rather than creating new structs

Vector array_to_vec(float vec[]) {
  return (Vector){.x = vec[0], .y = vec[1], .z = vec[2]};
}

Vector Vector_Add(Vector *a, Vector *b) {
  return (Vector){.x = a->x + b->x, .y = a->y + b->y, .z = a->z + b->z};
}

Vector Vector_Scale(Vector *v, float scalar) {
  float vec[] = {v->x, v->y, v->z};

  cblas_sscal(3, scalar, vec, 1);

  return array_to_vec(vec);
}

Vector Vector_Sub(Vector *a, Vector *b) {
  return (Vector){.x = a->x - b->x, .y = a->y - b->y, .z = a->z - b->z};
};

float Vector_Dot(Vector *a, Vector *b) {
  return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
};

float Vector_Mag(Vector *v) {
  return sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
};

Vector Vector_Norm(Vector *v) {
  float vec[] = {v->x, v->y, v->z};

  cblas_scnrm2(3, vec, 1);

  return (Vector){.x = vec[0], .y = vec[1], .z = vec[2]};
}

Vector Vector_Cross(Vector *a, Vector *b) {
  return (Vector){.x = (a->y * b->z) - (a->z * b->y),
                  .y = (a->z * b->x) - (a->x * b->z),
                  .z = (a->x * b->y) - (a->y * b->x)};
}
