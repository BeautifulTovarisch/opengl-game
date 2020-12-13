#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmocka.h>

#include "mod.h"

#define PI 3.14159265358979323846

// TODO :: Cleanup and write more thorough tests

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

// Separate utility to avoid coupling API method with tests
void identity_matrix(Mat4 m) {
  for (int i = 0; i < 16; i++) {
    m[i] = 0;
  }

  m[0] = 1;
  m[5] = 1;
  m[10] = 1;
  m[15] = 1;
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
  vector_equal(To_Quat((Vector){0, 0, 0}, 0), (Vector){0, 0, 0, 1});

  // 90 deg x
  vector_equal(To_Quat((Vector){1, 0, 0}, PI / 2),
               (Vector){0.707107f, 0, 0, 0.707107f});

  // 45 deg z
  vector_equal(To_Quat((Vector){1, 0, 0}, PI / 4),
               (Vector){0.382683f, 0, 0, 0.923880f});
}

/* Vector Tests
 * -----------------------------------------------------------------------------
 */
static void test_vector_add(void **state) {
  Vector result = V_Add((Vector){0}, (Vector){1, 1});

  assert_float_equal(result.x, 1, 0);
  assert_float_equal(result.y, 1, 0);
  assert_float_equal(result.w, 0, 0);

  result = V_Add((Vector){0}, (Vector){-1, -1});

  assert_float_equal(result.x, -1, 0);
  assert_float_equal(result.y, -1, 0);
  assert_float_equal(result.w, 0, 0);
}

static void test_vector_divide(void **state) {
  Vector input = {.x = 2, .y = 2, .z = 2};

  Vector result = V_Scale(input, 0.5);

  assert_float_equal(result.x, 1.0, 0);
  assert_float_equal(result.y, 1.0, 0);
  assert_float_equal(result.z, 1.0, 0);

  result = V_Scale(input, 0);

  assert_float_equal(result.x, 0, 0);
  assert_float_equal(result.y, 0, 0);
  assert_float_equal(result.z, 0, 0);
};

static void test_vector_subtract(void **state) {
  Vector input = {1, 1};
  Vector result = V_Sub(input, (Vector){1, 1});

  assert_float_equal(result.x, 0, 0);
  assert_float_equal(result.y, 0, 0);
}

static void test_vector_multiply(void **state) {
  vector_equal(V_Scale((Vector){1.0f, 1.0f}, 2.0f), (Vector){2.0f, 2.0f});
  vector_equal(V_Scale((Vector){2, 2, 1, 4}, 3.0f),
               (Vector){6.0f, 6.0f, 3.0f, 12.0f});
}

static void test_vector_normalize(void **state) {
  vector_equal(V_Norm((Vector){3, 4}), (Vector){0.6f, 0.8f});
  vector_equal(V_Norm((Vector){3, 0, 0, 4}), (Vector){0.6f, 0, 0, 0.8f});
}

static void test_vector_dot(void **state) {
  Vector a = {1, 2, 3};
  Vector b = {4, -5, 6};

  float result = V_Dot(a, b);

  assert_float_equal(result, 12.0, 0);
}

static void test_vector_cross(void **state) {
  Vector v1 = {3, -3, 1};
  Vector v2 = {4, 9, 2};

  vector_equal(V_Cross(v1, v2), (Vector){-15, -2, 39});
}

/* Matrix Tests
 * -----------------------------------------------------------------------------
 */

/* Identity Matrix
 *
 * |1 0 0 0|
 * |0 1 0 0|
 * |0 0 1 0|
 * |0 0 0 1|
 *
 */
static void test_identity_matrix(void **state) {
  Mat4 result;
  Mat4 expected = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

  M_Ident(result);

  array_equal(expected, result);
}

/* Othographic Matrix
 *
 * |2  0  0 -1|
 * |0  2  0 -1|
 * |0  0 -2 -1|
 * |0  0  0  1|
 *
 */
static void test_orthographic_matrix(void **state) {
  Mat4 result;
  Mat4 expected = {2, 0, 0, -1, 0, 2, 0, -1, 0, 0, -2, -1, 0, 0, 0, 1};

  // Left, right, bottom, top, near, far
  M_Ortho(0, 1, 0, 1, 0, 1, result);

  array_equal(expected, result);
}

static void test_matrix_multiply(void **state) {
  Mat4 translate = {1, 0, 0, 1, 0, 1, 0, 2, 0, 0, 1, 3, 0, 0, 0, 1};
  Mat4 scale = {2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1};

  Mat4 identity;
  identity_matrix(identity);

  /* Multiplication by identity matrix
   *
   * |1 0 0 1|   |1 0 0 0|
   * |0 1 0 2|   |0 1 0 0|
   * |0 0 1 3| * |0 0 1 0|
   * |0 0 0 1|   |0 0 0 1|
   */
  M_Mult(translate, identity);
  array_equal(translate, identity);

  /* Translation * Scale
   *
   * |1 0 0 1|   |2 0 0 0|
   * |0 1 0 2|   |0 2 0 0|
   * |0 0 1 3| * |0 0 2 0|
   * |0 0 0 1|   |0 0 0 1|
   */
  M_Mult(translate, scale);

  array_equal(scale, (Mat4){2, 0, 0, 1, 0, 2, 0, 2, 0, 0, 2, 3, 0, 0, 0, 1});

  Mat4 m1 = {3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1};
  Mat4 m2 = {2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1};

  M_Mult(m1, m2);

  array_equal(m2, (Mat4){6, 0, 0, 0, 0, 6, 0, 0, 0, 0, 6, 0, 0, 0, 0, 1});
}

static void test_scaling_matrix(void **state) {
  Mat4 scale;
  identity_matrix(scale);

  M_Scale((Vector){2, 3, 4}, scale);

  array_equal(scale, (Mat4){2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4, 0, 0, 0, 0, 1});
}

static void test_translation_matrix(void **state) {
  Mat4 trans;
  identity_matrix(trans);

  trans[3] = 1;
  trans[7] = 2;
  trans[11] = 3;

  M_Trans((Vector){1, 1, 1}, trans);

  array_equal(trans, (Mat4){1, 0, 0, 2, 0, 1, 0, 3, 0, 0, 1, 4, 0, 0, 0, 1});
}

/* Quaternion Tests
 * -----------------------------------------------------------------------------
 */
static void test_quat_mult(void **state) {
  Vector q1 = {1, 2, 3, 1};
  Vector q2 = {2.0f, 0, 0, 0};

  vector_equal(Q_Mult(q1, q2), (Vector){2, 6, -4, -2});
}

static void test_quat_norm(void **state) {

}

static void test_quat_inverse(void **state) {
  vector_equal(Q_Inverse((Vector){0, 1, 0, 1}), (Vector){0, -0.5f, 0, 0.5f});
}

/* Dual Quaternion Tests
 * -----------------------------------------------------------------------------
 */

static void test_dual_quat_create(void **state) {
  DualQuat dq =
      DQ_Create((Vector){0.707107f, 0, 0, 0.707107f}, (Vector){0, 20, 0, 0});

  vector_equal(dq.real, (Vector){0.707107f, 0, 0, 0.707107f});
  vector_equal(dq.dual, (Vector){0, 7.071068f, -7.071068f, 0});
}

static void test_dual_quat_scale(void **state) {
  DualQuat dq = {.real = {1, 1, 2, 1}, .dual = {2, 2, 3, 4}};

  DualQuat scaled = DQ_Scale(dq, 2.0f);

  vector_equal(scaled.real, (Vector){2, 2, 4, 2});
  vector_equal(scaled.dual, (Vector){4, 4, 6, 8});
}

static void test_dual_quat_norm(void **state) {
  DualQuat dq = {.real = {1, 1, 1, 1}, .dual = {4, 1, 6, 8}};

  DualQuat result = DQ_Norm(dq);

  vector_equal(result.real, (Vector){0.5f, 0.5f, 0.5f, 0.5f});
  vector_equal(result.dual, (Vector){2, 0.5f, 3, 4});
}

int main(void) {
  const struct CMUnitTest tests[] = {
      // Utility
      cmocka_unit_test(test_to_quat), cmocka_unit_test(test_to_radians),
      // Vector
      cmocka_unit_test(test_vector_add), cmocka_unit_test(test_vector_divide),
      cmocka_unit_test(test_vector_multiply),
      cmocka_unit_test(test_vector_subtract), cmocka_unit_test(test_vector_dot),
      cmocka_unit_test(test_vector_normalize),
      cmocka_unit_test(test_vector_cross),
      // Matrix
      cmocka_unit_test(test_identity_matrix),
      cmocka_unit_test(test_orthographic_matrix),
      cmocka_unit_test(test_matrix_multiply),
      cmocka_unit_test(test_scaling_matrix),
      cmocka_unit_test(test_translation_matrix),
      // Quaternion
      cmocka_unit_test(test_quat_mult), cmocka_unit_test(test_quat_inverse),
      cmocka_unit_test(test_quat_norm),
      // Dual Quaternion
      cmocka_unit_test(test_dual_quat_create),
      cmocka_unit_test(test_dual_quat_scale),
      cmocka_unit_test(test_dual_quat_norm)};

  return cmocka_run_group_tests(tests, NULL, NULL);
}
