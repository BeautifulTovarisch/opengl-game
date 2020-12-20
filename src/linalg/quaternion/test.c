#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmocka.h>

#include "mod.h"

#define PI 3.14159265358979323846

void quaternion_equal(Quaternion a, Quaternion b) {
  assert_float_equal(a.i, b.i, 10E-6);
  assert_float_equal(a.j, b.j, 10E-6);
  assert_float_equal(a.k, b.k, 10E-6);
  assert_float_equal(a.w, b.w, 10E-6);
}

// Mocks
// TODO :: Utilize CMocka mocking facilities
int V_Eq(Vector a, Vector b) { return 0; }
Vector V_Norm(Vector v) { return v; }
Vector V_Scale(Vector v, float scalar) { return v; }

/* Utility Tests
 * -----------------------------------------------------------------------------
 */

static void test_quat_from_axis(void **state) {
  // No Rotation
  quaternion_equal(Q_From_Axis((Vector){0, 0, 0}, 0), (Quaternion){0, 0, 0, 1});

  // 90 deg x
  quaternion_equal(Q_From_Axis((Vector){1, 0, 0}, PI / 2),
                   (Quaternion){0.707107f, 0, 0, 0.707107f});

  // 45 deg z
  quaternion_equal(Q_From_Axis((Vector){1, 0, 0}, PI / 4),
                   (Quaternion){0.382683f, 0, 0, 0.923880f});
}

/* Quaternion Tests
 * -----------------------------------------------------------------------------
 */
static void test_quaternion_add(void **state) {
  quaternion_equal(Q_Add((Quaternion){0}, (Quaternion){1, 1, 0, 1}),
                   (Quaternion){1, 1, 0, 1});
  quaternion_equal(Q_Add((Quaternion){1, 2, 3, 4}, (Quaternion){-1, -1, 0, 6}),
                   (Quaternion){0, 1, 3, 10});
}

static void test_quaternion_subtract(void **state) {
  quaternion_equal(Q_Sub((Quaternion){1, 1, 1, 1}, (Quaternion){1, 1, 1, 1}),
                   (Quaternion){0});

  quaternion_equal(Q_Sub((Quaternion){3, 7, 9, 9}, (Quaternion){4, 2, 6, 3}),
                   (Quaternion){-1, 5, 3, 6});
}

static void test_quaternion_normalize(void **state) {
  quaternion_equal(Q_Norm((Quaternion){3, 4}), (Quaternion){0.6f, 0.8f});
  quaternion_equal(Q_Norm((Quaternion){3, 0, 0, 4}),
                   (Quaternion){0.6f, 0, 0, 0.8f});
}

static void test_quaternion_mult(void **state) {
  Quaternion q1 = {1, 2, 3, 1};
  Quaternion q2 = {2.0f, 0, 0, 0};

  quaternion_equal(Q_Mult(q1, q2), (Quaternion){2, 6, -4, -2});
}

static void test_quaternion_inverse(void **state) {
  quaternion_equal(Q_Inverse((Quaternion){0, 1, 0, 1}),
                   (Quaternion){0, -0.5f, 0, 0.5f});
}

/* Dual Quaternion Tests
 * -----------------------------------------------------------------------------
 */

static void test_dual_quat_create(void **state) {
  DualQuat dq = DQ_Create((Quaternion){0.707107f, 0, 0, 0.707107f},
                          (Vector){0, 20, 0, 0});

  quaternion_equal(dq.real, (Quaternion){0.707107f, 0, 0, 0.707107f});
  quaternion_equal(dq.dual, (Quaternion){0, 7.071068f, -7.071068f, 0});
}

static void test_dual_quat_scale(void **state) {
  DualQuat dq = {.real = {1, 1, 2, 1}, .dual = {2, 2, 3, 4}};

  DualQuat scaled = DQ_Scale(dq, 2.0f);

  quaternion_equal(scaled.real, (Quaternion){2, 2, 4, 2});
  quaternion_equal(scaled.dual, (Quaternion){4, 4, 6, 8});
}

static void test_dual_quat_norm(void **state) {
  DualQuat dq = {.real = {1, 1, 1, 1}, .dual = {4, 1, 6, 8}};

  DualQuat result = DQ_Norm(dq);

  quaternion_equal(result.real, (Quaternion){0.5f, 0.5f, 0.5f, 0.5f});
  quaternion_equal(result.dual, (Quaternion){2, 0.5f, 3, 4});
}

int main(void) {
  const struct CMUnitTest tests[] = {
      // Utility
      cmocka_unit_test(test_quat_from_axis),
      // Quaternion
      cmocka_unit_test(test_quaternion_add),
      cmocka_unit_test(test_quaternion_subtract),
      cmocka_unit_test(test_quaternion_normalize),
      cmocka_unit_test(test_quaternion_mult),
      cmocka_unit_test(test_quaternion_inverse),
      // Dual Quaternion
      cmocka_unit_test(test_dual_quat_create),
      cmocka_unit_test(test_dual_quat_scale),
      cmocka_unit_test(test_dual_quat_norm)};

  return cmocka_run_group_tests(tests, NULL, NULL);
}
