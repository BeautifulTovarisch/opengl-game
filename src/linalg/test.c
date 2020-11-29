#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmocka.h>

#include "mod.h"

#define PI 3.14159265358979323846

// Mocks
void Log(Level level, char const *message) {}

void vector_equal(Vector a, Vector b) {
  assert_float_equal(a.x, b.x, 10E-6);
  assert_float_equal(a.y, b.y, 10E-6);
  assert_float_equal(a.z, b.z, 10E-6);
  assert_float_equal(a.w, b.w, 10E-6);
}

void array_equal(float a[16], float b[16]) {
  for (int i = 0; i < 16; i++) {
    assert_float_equal(a[i], b[i], 0);
  }
}

/* Utility Tests
 * -----------------------------------------------------------------------------
 */

// Enumerating 15-30 deg increments to serve as a quick cheatsheet
static void test_to_radians(void **state) {
  assert_float_equal(To_Rad(0), 0, 0);
  assert_float_equal(To_Rad(30), PI / 6, 0);
  assert_float_equal(To_Rad(45), PI / 4, 0);
  assert_float_equal(To_Rad(60), PI / 3, 0);
  assert_float_equal(To_Rad(90), PI / 2, 0);
  assert_float_equal(To_Rad(120), 2 * PI / 3, 0);
  assert_float_equal(To_Rad(135), 3 * PI / 4, 0);
  assert_float_equal(To_Rad(150), 5 * PI / 6, 0);
  assert_float_equal(To_Rad(180), PI, 0);
  assert_float_equal(To_Rad(270), 3 * PI / 2, 0);
  assert_float_equal(To_Rad(360), 2 * PI, 0);
}

static void test_to_quat(void **state) {
  // No Rotation
  vector_equal(To_Quat((Vector){0, 0, 0}), (Vector){0, 0, 0, 1});

  // 90 deg x
  vector_equal(To_Quat((Vector){PI / 2, 0, 0}),
               (Vector){0.707107f, 0, 0, 0.707107f});

  // 45 deg z
  vector_equal(To_Quat((Vector){PI / 4, 0, 0}),
               (Vector){0.382683f, 0, 0, 0.923880f});
}

/* Vector Tests
 * -----------------------------------------------------------------------------
 */
static void test_vector_add(void **state) {
  Vector result = Vector_Add((Vector){0}, (Vector){1, 1});

  assert_float_equal(result.x, 1, 0);
  assert_float_equal(result.y, 1, 0);

  result = Vector_Add((Vector){0}, (Vector){-1, -1});

  assert_float_equal(result.x, -1, 0);
  assert_float_equal(result.y, -1, 0);
}

static void test_vector_divide(void **state) {
  Vector input = {.x = 2, .y = 2, .z = 2};

  Vector result = Vector_Scale(input, 0.5);

  assert_float_equal(result.x, 1.0, 0);
  assert_float_equal(result.y, 1.0, 0);
  assert_float_equal(result.z, 1.0, 0);

  result = Vector_Scale(input, 0);

  assert_float_equal(result.x, 0, 0);
  assert_float_equal(result.y, 0, 0);
  assert_float_equal(result.z, 0, 0);
};

static void test_vector_subtract(void **state) {
  Vector input = {1, 1};
  Vector result = Vector_Sub(input, (Vector){1, 1});

  assert_float_equal(result.x, 0, 0);
  assert_float_equal(result.y, 0, 0);
}

static void test_vector_multiply(void **state) {
  Vector input = {1, 1};

  Vector result = Vector_Scale(input, 2);

  assert_float_equal(result.x, 2.0, 0);
  assert_float_equal(result.y, 2.0, 0);
}

static void test_vector_magnitude(void **state) {
  assert_float_equal(Vector_Mag((Vector){3, 4}), 5.0, 0);
}

static void test_vector_normalize(void **state) {
  Vector input = {3, 4};

  Vector result = Vector_Norm(input);

  assert_float_equal(result.x, 0.60, 0);
  assert_float_equal(result.y, 0.80, 0);
}

static void test_vector_dot(void **state) {
  Vector a = {1, 2, 3};
  Vector b = {4, -5, 6};

  float result = Vector_Dot(a, b);

  assert_float_equal(result, 12.0, 0);
}

static void test_vector_cross(void **state) {
  Vector v1 = {3, -3, 1};
  Vector v2 = {4, 9, 2};

  vector_equal(Vector_Cross(v1, v2), (Vector){-15, -2, 39});
}

/* Matrix Tests
 * -----------------------------------------------------------------------------
 */

/* Identity Matrix
 * ---------
 * |1 0 0 0|
 * |0 1 0 0|
 * |0 0 1 0|
 * |0 0 0 1|
 * ---------
 */
static void test_identity_matrix(void **state) {
  Mat4 result;
  Mat4 expected = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

  Matrix_Ident(result);

  array_equal(expected, result);
}

/* ------------
 * | 2  0  0 0|
 * | 0  2  0 0|
 * | 0  0 -2 0|
 * |-1 -1 -1 1|
 * ------------
 */
static void test_orthographic_matrix(void **state) {
  Mat4 result;
  Mat4 expected = {2, 0, 0, 0, 0, 2, 0, 0, 0, 0, -2, 0, -1, -1, -1, 1};

  Matrix_Ortho(0, 1, 0, 1, 0, 1, result);

  array_equal(expected, result);
}

/* Quaternion Tests
 * -----------------------------------------------------------------------------
 */
static void test_quat_mult(void **state) {
  Vector q1 = {0, 0.707107f, 0, 0.707107f};
  Vector q2 = {0.382683f, 0, 0, 0.923880f};

  vector_equal(Quat_Mult(q2, q1),
               (Vector){0.270598f, 0.653281f, 0.270598f, 0.653281});
}

static void test_quat_inverse(void **state) {
  vector_equal(Quat_Inverse((Vector){0, 1, 0, 1}), (Vector){0, -0.5f, 0, 0.5f});
}

static void test_quat_rotation(void **state) {
  Vector q = {0.270598f, 0.653281f, 0.270598f, 0.653281};

  vector_equal(Quat_Rot(q, (Vector){1, 0, 0}),
               (Vector){0, 0.707107, -0.707107});
}

int main(void) {
  const struct CMUnitTest tests[] = {
      // Utility
      cmocka_unit_test(test_to_quat), cmocka_unit_test(test_to_radians),
      // Vector
      cmocka_unit_test(test_vector_add), cmocka_unit_test(test_vector_divide),
      cmocka_unit_test(test_vector_multiply),
      cmocka_unit_test(test_vector_subtract), cmocka_unit_test(test_vector_dot),
      cmocka_unit_test(test_vector_magnitude),
      cmocka_unit_test(test_vector_normalize),
      cmocka_unit_test(test_vector_cross),
      // Matrix
      cmocka_unit_test(test_identity_matrix),
      cmocka_unit_test(test_orthographic_matrix),
      // Quaternion
      cmocka_unit_test(test_quat_mult), cmocka_unit_test(test_quat_inverse),
      cmocka_unit_test(test_quat_rotation)};

  return cmocka_run_group_tests(tests, NULL, NULL);
}
