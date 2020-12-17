#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmocka.h>

#include "mod.h"

// TODO :: Cleanup and write more thorough tests

void matrix_equal(float a[16], float b[16]) {
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

  matrix_equal(expected, result);
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

  matrix_equal(expected, result);
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
  matrix_equal(translate, identity);

  /* Translation * Scale
   *
   * |1 0 0 1|   |2 0 0 0|
   * |0 1 0 2|   |0 2 0 0|
   * |0 0 1 3| * |0 0 2 0|
   * |0 0 0 1|   |0 0 0 1|
   */
  M_Mult(translate, scale);

  matrix_equal(scale, (Mat4){2, 0, 0, 1, 0, 2, 0, 2, 0, 0, 2, 3, 0, 0, 0, 1});

  Mat4 m1 = {3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1};
  Mat4 m2 = {2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1};

  M_Mult(m1, m2);

  matrix_equal(m2, (Mat4){6, 0, 0, 0, 0, 6, 0, 0, 0, 0, 6, 0, 0, 0, 0, 1});
}

static void test_scaling_matrix(void **state) {
  Mat4 scale;
  identity_matrix(scale);

  M_Scale((Vector){2, 3, 4}, scale);

  matrix_equal(scale, (Mat4){2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4, 0, 0, 0, 0, 1});
}

static void test_translation_matrix(void **state) {
  Mat4 trans;
  identity_matrix(trans);

  trans[3] = 1;
  trans[7] = 2;
  trans[11] = 3;

  M_Trans((Vector){1, 1, 1}, trans);

  matrix_equal(trans, (Mat4){1, 0, 0, 2, 0, 1, 0, 3, 0, 0, 1, 4, 0, 0, 0, 1});
}

int main(void) {
  const struct CMUnitTest tests[] = {cmocka_unit_test(test_identity_matrix),
                                     cmocka_unit_test(test_orthographic_matrix),
                                     cmocka_unit_test(test_matrix_multiply),
                                     cmocka_unit_test(test_scaling_matrix),
                                     cmocka_unit_test(test_translation_matrix)};

  return cmocka_run_group_tests(tests, NULL, NULL);
}
