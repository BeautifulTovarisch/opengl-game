#include "mod.h"

// TODO :: Write utility to map 2d to 1d array indices

Vector array_to_vec(float vec[]) {
  return (Vector){.x = vec[0], .y = vec[1], .z = vec[2], .w = 0};
}

// A·B
float dot(Vector a, Vector b) {
  float v1[] = {a.x, a.y, a.z};
  float v2[] = {b.x, b.y, b.z};

  return cblas_sdot(3, v1, 1, v2, 1);
}

// |A|
float mag(Vector v) {
  float vec[] = {v.x, v.y, v.z};

  return cblas_snrm2(3, vec, 1);
}

/* Utilities
 * -----------------------------------------------------------------------------
 */
float To_Rad(float deg) { return deg * ((float)M_PI / 180); }

/* Vectors
 * -----------------------------------------------------------------------------
 */
Vector V_Add(Vector a, Vector b) {
  float v1[] = {a.x, a.y, a.z, 0};
  float v2[] = {b.x, b.y, b.z, 0};

  cblas_saxpy(3, 1, v1, 1, v2, 1);

  return array_to_vec(v2);
}

Vector V_Scale(Vector v, float scalar) {
  float vec[] = {v.x, v.y, v.z};

  cblas_sscal(3, scalar, vec, 1);

  return array_to_vec(vec);
}

Vector V_Sub(Vector a, Vector b) {
  float v1[] = {a.x, a.y, a.z};
  float v2[] = {b.x, b.y, b.z};

  // Scalar is -1 here . a + (-b)
  cblas_saxpy(3, -1, v2, 1, v1, 1);

  return array_to_vec(v1);
}

Vector V_Norm(Vector v) {
  float vec[] = {v.x, v.y, v.z};
  float mv = mag(v);

  if (mag > 0) {
    cblas_sscal(3, 1 / mv, vec, 1);

    return array_to_vec(vec);
  }

  return (Vector){0};
}

Vector V_Cross(Vector a, Vector b) {
  return (Vector){.x = (a.y * b.z) - (a.z * b.y),
                  .y = (a.z * b.x) - (a.x * b.z),
                  .z = (a.x * b.y) - (a.y * b.x)};
}

Vector V_Project(Vector a, Vector b) {
  float v1[] = {a.x, a.y, a.z};
  float v2[] = {b.x, b.y, b.z};

  float mb = mag(b);

  if (!mb) {
    return (Vector){0};
  }

  cblas_sscal(3, dot(a, b) / (mb * mb), v2, 1);

  return array_to_vec(v2);
}

Vector V_Reject(Vector a, Vector b) { return V_Sub(a, V_Project(a, b)); }

float V_Dot(Vector a, Vector b) { return dot(a, b); };

// Θ = cos^-1(A·B / |A||B|)
float V_Angle(Vector a, Vector b) {
  float m1 = mag(a);
  float m2 = mag(b);

  if (m1 * m2 == 0) {
    return 0.0f;
  }

  return acosf(dot(a, b) / (sqrtf(m1) * sqrtf(m2)));
}
