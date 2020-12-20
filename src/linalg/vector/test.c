#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmocka.h>

#include "mod.h"

#define PI 3.14159265358979323846

void vector_equal(Vector a, Vector b) {
  assert_float_equal(a.x, b.x, 10E-6);
  assert_float_equal(a.y, b.y, 10E-6);
  assert_float_equal(a.z, b.z, 10E-6);
  assert_float_equal(a.w, b.w, 10E-6);
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

/* Vector Tests
 * -----------------------------------------------------------------------------
 */
static void test_vector_add(void **state) {
  vector_equal(V_Add((Vector){0}, (Vector){1, 1, 1}), (Vector){1, 1, 1});
  vector_equal(V_Add((Vector){1, 2, 3}, (Vector){-1, 3, -5}),
               (Vector){0, 5, -2});
}

static void test_vector_divide(void **state) {
  Vector input = {2, 2, 2};

  vector_equal(V_Scale(input, 3), (Vector){6, 6, 6});
  vector_equal(V_Scale(input, 0.5), (Vector){1, 1, 1});
};

static void test_vector_subtract(void **state) {
  vector_equal(V_Sub((Vector){1, 1}, (Vector){1, 1}), (Vector){0, 0, 0});
  vector_equal(V_Sub((Vector){-1, 3, 2}, (Vector){1, 2, 2}),
               (Vector){-2, 1, 0});
}

static void test_vector_scale(void **state) {
  vector_equal(V_Scale((Vector){1.0f, 1.0f}, 2.0f), (Vector){2.0f, 2.0f});
  vector_equal(V_Scale((Vector){2, 2, 1, 0}, 3.0f),
               (Vector){6.0f, 6.0f, 3.0f, 0});
}

static void test_vector_normalize(void **state) {
  vector_equal(V_Norm((Vector){3, 4}), (Vector){0.6f, 0.8f});
  vector_equal(V_Norm((Vector){5, 8, 2, 0}),
               (Vector){0.518476f, 0.829561f, 0.2074f, 0});
}

static void test_vector_dot(void **state) {
  Vector a = {1, 2, 3};
  Vector b = {4, -5, 6};

  assert_float_equal(V_Dot(a, b), 12.0, 0);
}

static void test_vector_cross(void **state) {
  Vector v1 = {3, -3, 1};
  Vector v2 = {4, 9, 2};

  vector_equal(V_Cross(v1, v2), (Vector){-15, -2, 39});
}

static void test_vector_angle(void **state) {
  Vector v1 = {8, 6, 2};
  Vector v2 = {2, 9, -7};

  assert_float_equal(V_Angle(v1, v2), 1.076546, 0);
  assert_float_equal(V_Angle((Vector){0}, v2), 0, 0);
}

static void test_vector_project(void **state) {
  vector_equal(V_Project((Vector){-1, 4, 2}, (Vector){1, 0, 3}),
               (Vector){0.5f, 0, 1.5f});
  vector_equal(V_Project((Vector){-1, 3, 2}, (Vector){4, 8, 8}),
               (Vector){1, 2, 2});
}

static void test_vector_reject(void **state) {
  vector_equal(V_Reject((Vector){-1, 3, 2}, (Vector){4, 8, 8}),
               (Vector){-2, 1, 0});
}

int main(void) {
  const struct CMUnitTest tests[] = {
      // Utility
      cmocka_unit_test(test_to_radians),
      // Vector
      cmocka_unit_test(test_vector_add), cmocka_unit_test(test_vector_divide),
      cmocka_unit_test(test_vector_scale),
      cmocka_unit_test(test_vector_subtract), cmocka_unit_test(test_vector_dot),
      cmocka_unit_test(test_vector_normalize),
      cmocka_unit_test(test_vector_cross), cmocka_unit_test(test_vector_angle),
      cmocka_unit_test(test_vector_project),
      cmocka_unit_test(test_vector_reject)};

  return cmocka_run_group_tests(tests, NULL, NULL);
}
