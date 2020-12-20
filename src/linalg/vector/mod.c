#include "mod.h"

Vector array_to_vec(float vec[]) {
  return (Vector){.x = vec[0], .y = vec[1], .z = vec[2], .w = 0};
}

// Various Vector operations implemented with cblas

// A+B
Vector add(Vector a, Vector b) {
  float v1[] = {a.x, a.y, a.z, 0};
  float v2[] = {b.x, b.y, b.z, 0};

  cblas_saxpy(3, 1, v1, 1, v2, 1);

  return array_to_vec(v2);
}

// A-B
Vector sub(Vector a, Vector b) {
  float v1[] = {a.x, a.y, a.z};
  float v2[] = {b.x, b.y, b.z};

  // Scalar is -1 here . a + (-b)
  cblas_saxpy(3, -1, v2, 1, v1, 1);

  return array_to_vec(v1);
}

// A*b
Vector scale(Vector v, float scl) {
  float vec[] = {v.x, v.y, v.z};

  cblas_sscal(3, scl, vec, 1);

  return array_to_vec(vec);
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

// |A|^2
float mag_sq(Vector v) { return dot(v, v); }

/* Utilities
 * -----------------------------------------------------------------------------
 */
float To_Rad(float deg) { return deg * ((float)M_PI / 180); }

/* Vectors
 * -----------------------------------------------------------------------------
 */
Vector V_Add(Vector a, Vector b) { return add(a, b); }
Vector V_Sub(Vector a, Vector b) { return sub(a, b); }
Vector V_Scale(Vector v, float scalar) { return scale(v, scalar); }

Vector V_Norm(Vector v) {
  float mv = mag(v);

  if (mag > 0) {
    return scale(v, 1 / mv);
  }

  return (Vector){0};
}

Vector V_Cross(Vector a, Vector b) {
  return (Vector){.x = (a.y * b.z) - (a.z * b.y),
                  .y = (a.z * b.x) - (a.x * b.z),
                  .z = (a.x * b.y) - (a.y * b.x)};
}

Vector V_Project(Vector a, Vector b) {
  float mb = mag_sq(b);

  if (!mb) {
    return (Vector){0};
  }

  return scale(b, dot(a, b) / mb);
}

Vector V_Reject(Vector a, Vector b) { return sub(a, V_Project(a, b)); }

// A - 2B(A·B / |B|)
Vector V_Reflect(Vector a, Vector b) {
  float mb = mag(b);

  if (!mb) {
    return (Vector){0};
  }

  return sub(a, scale(b, (dot(a, b) / mb) * 2));
}

// Normalized lerp performed when flag is set
Vector V_Lerp(Vector a, Vector b, float t, int normalize) {
  return normalize ? V_Norm((Vector){
                         .x = (a.x - b.x) * t,
                         .y = (a.y - b.y) * t,
                         .z = (a.z - b.z) * t,
                     })
                   : (Vector){
                         .x = (a.x - b.x) * t,
                         .y = (a.y - b.y) * t,
                         .z = (a.z - b.z) * t,
                     };
}

// Useful for constant interpolation of velocity
Vector V_Slerp(Vector a, Vector b, float t) {
  // Fallback to linear interpolation when t is close to 0
  if (t < 0.01f) {
    return V_Lerp(a, b, t, 0);
  }

  Vector to = V_Norm(b);
  Vector from = V_Norm(a);

  float theta = V_Angle(from, to);
  float sin_theta = sinf(theta);
  float s = sinf((1.0f - t) * theta) / sin_theta;
  float e = sinf(t * theta) / sin_theta;

  return add(scale(from, s), scale(to, e));
}

int V_Eq(Vector a, Vector b) { return a.x == b.x && a.y == b.y && a.z == b.z; }

float V_Dot(Vector a, Vector b) { return dot(a, b); };

// Θ = cos^-1(A·B / |A||B|)
float V_Angle(Vector a, Vector b) {
  float m1 = mag(a);
  float m2 = mag(b);

  if (m1 * m2 == 0) {
    return 0.0f;
  }

  return acosf(dot(a, b) / (m1 * m2));
}
