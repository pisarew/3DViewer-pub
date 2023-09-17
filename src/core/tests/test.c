#include <check.h>

#include "object_controller.h"

START_TEST(get_object_test) {
  object_t object;
  int error = get_object("./core/tests/triangle.obj", &object);

  ck_assert_int_eq(error, 0);

  ck_assert_float_eq_tol(object.vertexes[0], -0.6, 1e-1);
  ck_assert_float_eq_tol(object.vertexes[1], 0.6, 1e-1);
  ck_assert_float_eq_tol(object.vertexes[2], 0, 1e-0);
  ck_assert_float_eq_tol(object.vertexes[3], 0, 1e-0);
  ck_assert_float_eq_tol(object.vertexes[4], 0, 1e-0);
  ck_assert_float_eq_tol(object.vertexes[5], 0, 1e-0);
  ck_assert_float_eq_tol(object.vertexes[6], -0.6, 1e-1);
  ck_assert_float_eq_tol(object.vertexes[7], 0, 1e-0);
  ck_assert_float_eq_tol(object.vertexes[8], 0, 1e-0);

  ck_assert_uint_eq(object.polygons[0], 0);
  ck_assert_uint_eq(object.polygons[1], 1);
  ck_assert_uint_eq(object.polygons[2], 1);
  ck_assert_uint_eq(object.polygons[3], 2);
  ck_assert_uint_eq(object.polygons[4], 2);
  ck_assert_uint_eq(object.polygons[5], 0);

  free_object(&object);

} END_TEST

START_TEST(rotate_object_test) {
  object_t object;
  int error = get_object("./core/tests/triangle.obj", &object);
  ck_assert_int_eq(error, 0);

  rotate_object(&object, 45, 45, 45);

  ck_assert_float_eq_tol(object.vertexes[0], -0.724264, 1e-3);
  ck_assert_float_eq_tol(object.vertexes[1], 0.185195, 1e-3);
  ck_assert_float_eq_tol(object.vertexes[2], 0.401428, 1e-3);
  ck_assert_float_eq_tol(object.vertexes[3], 0, 1e-0);
  ck_assert_float_eq_tol(object.vertexes[4], 0, 1e-0);
  ck_assert_float_eq_tol(object.vertexes[5], 0, 1e-0);
  ck_assert_float_eq_tol(object.vertexes[6], -0.512132, 1e-3);
  ck_assert_float_eq_tol(object.vertexes[7], -0.293311, 1e-3);
  ck_assert_float_eq_tol(object.vertexes[8], 0.108116, 1e-3);

  free_object(&object);
} END_TEST

START_TEST(move_object_test) {
  object_t object;
  int error = get_object("./core/tests/triangle.obj", &object);
  ck_assert_int_eq(error, 0);

  move_object(&object, 0.1f, 0.1f, 0.1f);

  ck_assert_float_eq_tol(object.vertexes[0], -0.5, 1e-1);
  ck_assert_float_eq_tol(object.vertexes[1], 0.7, 1e-1);
  ck_assert_float_eq_tol(object.vertexes[2], 0.1, 1e-1);
  ck_assert_float_eq_tol(object.vertexes[3], 0.1, 1e-1);
  ck_assert_float_eq_tol(object.vertexes[4], 0.1, 1e-1);
  ck_assert_float_eq_tol(object.vertexes[5], 0.1, 1e-1);
  ck_assert_float_eq_tol(object.vertexes[6], -0.5, 1e-1);
  ck_assert_float_eq_tol(object.vertexes[7], 0.1, 1e-1);
  ck_assert_float_eq_tol(object.vertexes[8], 0.1, 1e-1);

  free_object(&object);
} END_TEST

START_TEST(scale_object_test) {
  object_t object;
  int error = get_object("./core/tests/triangle.obj", &object);
  ck_assert_int_eq(error, 0);

  scale_object(&object, 1.1f);

  ck_assert_float_eq_tol(object.vertexes[0], -0.66, 1e-2);
  ck_assert_float_eq_tol(object.vertexes[1], 0.66, 1e-2);
  ck_assert_float_eq_tol(object.vertexes[2], 0, 1e-0);
  ck_assert_float_eq_tol(object.vertexes[3], 0, 1e-0);
  ck_assert_float_eq_tol(object.vertexes[4], 0, 1e-0);
  ck_assert_float_eq_tol(object.vertexes[5], 0, 1e-0);
  ck_assert_float_eq_tol(object.vertexes[6], -0.66, 1e-2);
  ck_assert_float_eq_tol(object.vertexes[7], 0, 1e-0);
  ck_assert_float_eq_tol(object.vertexes[8], 0, 1e-0);

  free_object(&object);
} END_TEST

START_TEST(error_test) {
  object_t object;
  int error = get_object("triangle.objjjjj", &object);
  ck_assert_int_eq(error, 1);
} END_TEST

Suite *arithmetics_suit(void) {
  Suite *suite = suite_create("Controller");
  TCase *tc_core = tcase_create("Controller");
  suite_add_tcase(suite, tc_core);

  tcase_add_test(tc_core, get_object_test);
  tcase_add_test(tc_core, rotate_object_test);
  tcase_add_test(tc_core, move_object_test);
  tcase_add_test(tc_core, scale_object_test);
  tcase_add_test(tc_core, error_test);

  return suite;
}


int main(void) {
  int no_failed;
  SRunner *runner;

  runner = srunner_create(arithmetics_suit());

  srunner_set_fork_status(runner, CK_NOFORK);
  srunner_run_all(runner, CK_NORMAL);
  no_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return no_failed;
}
