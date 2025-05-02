#include <check.h>
#include <stdlib.h>

#include "s21_helpers.c"
#include "s21_matrix.c"
#include "s21_matrix.h"

START_TEST(test_create_matrix_success) {
  matrix_t m;
  int res = s21_create_matrix(2, 3, &m);
  ck_assert_int_eq(res, 0);
  ck_assert_ptr_nonnull(m.matrix);
  ck_assert_int_eq(m.rows, 2);
  ck_assert_int_eq(m.columns, 3);
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_create_matrix_invalid_size) {
  matrix_t m;
  int res = s21_create_matrix(-2, 3, &m);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_create_matrix_null_pointer) {
  int res = s21_create_matrix(2, 2, NULL);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_eq_matrix_success) {
  matrix_t a, b;
  s21_create_matrix(2, 2, &a);
  s21_create_matrix(2, 2, &b);
  a.matrix[0][0] = b.matrix[0][0] = 1.0;
  a.matrix[0][1] = b.matrix[0][1] = 2.0;
  a.matrix[1][0] = b.matrix[1][0] = 3.0;
  a.matrix[1][1] = b.matrix[1][1] = 4.0;
  ck_assert_int_eq(s21_eq_matrix(&a, &b), SUCCESS);
  s21_remove_matrix(&a);
  s21_remove_matrix(&b);
}
END_TEST

START_TEST(test_eq_matrix_different_size) {
  matrix_t a, b;
  s21_create_matrix(2, 2, &a);
  s21_create_matrix(3, 3, &b);
  ck_assert_int_eq(s21_eq_matrix(&a, &b), FAILURE);
  s21_remove_matrix(&a);
  s21_remove_matrix(&b);
}
END_TEST

START_TEST(test_sum_matrix_success) {
  matrix_t a, b, res;
  s21_create_matrix(2, 2, &a);
  s21_create_matrix(2, 2, &b);
  a.matrix[0][0] = b.matrix[0][0] = 1.0;
  a.matrix[0][1] = b.matrix[0][1] = 2.0;
  a.matrix[1][0] = b.matrix[1][0] = 3.0;
  a.matrix[1][1] = b.matrix[1][1] = 4.0;
  ck_assert_int_eq(s21_sum_matrix(&a, &b, &res), 0);
  ck_assert_double_eq(res.matrix[0][0], 2.0);
  ck_assert_double_eq(res.matrix[1][1], 8.0);
  s21_remove_matrix(&a);
  s21_remove_matrix(&b);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_sum_matrix_null_input) {
  matrix_t a = {0}, b = {0}, res = {0};
  ck_assert_int_eq(s21_sum_matrix(&a, &b, &res), 1);
  s21_remove_matrix(&a);
  s21_remove_matrix(&b);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_inverse_a_null) {
  matrix_t res;
  s21_create_matrix(2, 2, &res);
  ck_assert_int_eq(s21_inverse_matrix(NULL, &res), 1);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_inverse_matrix_success_2x2) {
  matrix_t m, inv, expected;
  s21_create_matrix(2, 2, &m);
  s21_create_matrix(2, 2, &expected);
  m.matrix[0][0] = 4;
  m.matrix[0][1] = 7;
  m.matrix[1][0] = 2;
  m.matrix[1][1] = 6;

  expected.matrix[0][0] = 0.6;
  expected.matrix[0][1] = -0.7;
  expected.matrix[1][0] = -0.2;
  expected.matrix[1][1] = 0.4;

  ck_assert_int_eq(s21_inverse_matrix(&m, &inv), 0);
  ck_assert_int_eq(s21_eq_matrix(&inv, &expected), SUCCESS);
  s21_remove_matrix(&m);
  s21_remove_matrix(&inv);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_inverse_matrix_singular) {
  matrix_t m, res;
  s21_create_matrix(2, 2, &m);
  m.matrix[0][0] = 1;
  m.matrix[0][1] = 2;
  m.matrix[1][0] = 2;
  m.matrix[1][1] = 4;

  int code = s21_inverse_matrix(&m, &res);
  ck_assert_int_eq(code, 2);
  s21_remove_matrix(&m);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_sub_matrix_fail) {
  matrix_t a, res;
  s21_create_matrix(2, 2, &a);
  ck_assert_int_eq(s21_sub_matrix(&a, NULL, &res), 1);
  s21_remove_matrix(&a);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_sub_matrix_success) {
  matrix_t a, b, res;
  s21_create_matrix(2, 2, &a);
  s21_create_matrix(2, 2, &b);
  a.matrix[0][0] = 4;
  b.matrix[0][0] = 1;
  a.matrix[0][1] = 3;
  b.matrix[0][1] = 2;
  a.matrix[1][0] = 2;
  b.matrix[1][0] = 2;
  a.matrix[1][1] = 1;
  b.matrix[1][1] = 0;

  ck_assert_int_eq(s21_sub_matrix(&a, &b, &res), 0);
  ck_assert_double_eq(res.matrix[0][0], 3);
  ck_assert_double_eq(res.matrix[1][1], 1);
  s21_remove_matrix(&a);
  s21_remove_matrix(&b);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_sub_matrix_diff_size) {
  matrix_t a, b, res;
  s21_create_matrix(2, 2, &a);
  s21_create_matrix(3, 3, &b);
  ck_assert_int_eq(s21_sub_matrix(&a, &b, &res), 2);
  s21_remove_matrix(&a);
  s21_remove_matrix(&b);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_mult_number_result_null) {
  matrix_t a;
  s21_create_matrix(2, 2, &a);
  ck_assert_int_eq(s21_mult_number(&a, 2.0, NULL), 1);
  s21_remove_matrix(&a);
}
END_TEST

START_TEST(test_mult_number_success) {
  matrix_t a, res;
  s21_create_matrix(2, 2, &a);
  a.matrix[0][0] = 1.5;
  a.matrix[1][1] = 2;
  ck_assert_int_eq(s21_mult_number(&a, 2.0, &res), 0);
  ck_assert_double_eq(res.matrix[0][0], 3.0);
  ck_assert_double_eq(res.matrix[1][1], 4.0);
  s21_remove_matrix(&a);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_mult_number_null) {
  matrix_t m = {0}, res;
  ck_assert_int_eq(s21_mult_number(&m, 2.0, &res), 1);
}
END_TEST

START_TEST(test_transpose_success) {
  matrix_t a, res;
  s21_create_matrix(2, 3, &a);
  a.matrix[0][1] = 5;
  a.matrix[1][2] = 10;
  ck_assert_int_eq(s21_transpose(&a, &res), 0);
  ck_assert_double_eq(res.matrix[1][0], 5);
  ck_assert_double_eq(res.matrix[2][1], 10);
  s21_remove_matrix(&a);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_calc_complements_null_a) {
  matrix_t res;
  s21_create_matrix(2, 2, &res);
  ck_assert_int_eq(s21_calc_complements(NULL, &res), 1);
  s21_remove_matrix(&res);
}

START_TEST(test_calc_complements_1x1) {
  matrix_t a, res;
  s21_create_matrix(1, 1, &a);
  a.matrix[0][0] = 5;
  ck_assert_int_eq(s21_calc_complements(&a, &res), 0);
  ck_assert_double_eq(res.matrix[0][0], 1);
  s21_remove_matrix(&a);
  s21_remove_matrix(&res);
}

START_TEST(test_calc_complements_success) {
  matrix_t a, res;
  s21_create_matrix(2, 2, &a);
  a.matrix[0][0] = 1;
  a.matrix[0][1] = 2;
  a.matrix[1][0] = 3;
  a.matrix[1][1] = 4;
  ck_assert_int_eq(s21_calc_complements(&a, &res), 0);
  ck_assert_double_eq(res.matrix[0][0], 4);
  ck_assert_double_eq(res.matrix[0][1], -3);
  ck_assert_double_eq(res.matrix[1][0], -2);
  ck_assert_double_eq(res.matrix[1][1], 1);
  s21_remove_matrix(&a);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_calc_complements_not_square) {
  matrix_t a, res;
  s21_create_matrix(2, 3, &a);
  ck_assert_int_eq(s21_calc_complements(&a, &res), 2);
  s21_remove_matrix(&a);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_determinant_a_null) {
  double res = 0;
  ck_assert_int_eq(s21_determinant(NULL, &res), 1);
}
END_TEST

START_TEST(test_determinant_1x1) {
  matrix_t a;
  double res = 0;
  s21_create_matrix(1, 1, &a);
  a.matrix[0][0] = 5.0;
  ck_assert_int_eq(s21_determinant(&a, &res), 0);
  ck_assert_double_eq(res, 5.0);
  s21_remove_matrix(&a);
}
END_TEST

START_TEST(test_determinant_2x2) {
  matrix_t a;
  double res = 0;
  s21_create_matrix(2, 2, &a);
  a.matrix[0][0] = 1;
  a.matrix[0][1] = 2;
  a.matrix[1][0] = 3;
  a.matrix[1][1] = 4;
  ck_assert_int_eq(s21_determinant(&a, &res), 0);
  ck_assert_double_eq(res, -2);
  s21_remove_matrix(&a);
}
END_TEST

START_TEST(test_determinant_not_square) {
  matrix_t a;
  double res = 0;
  s21_create_matrix(2, 3, &a);
  ck_assert_int_eq(s21_determinant(&a, &res), 2);
  s21_remove_matrix(&a);
}
END_TEST

START_TEST(test_mnrSprt_basic) {
  matrix_t src, tmp;
  s21_create_matrix(3, 3, &src);
  s21_create_matrix(2, 2, &tmp);

  int val = 1;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      src.matrix[i][j] = val++;
    }
  }

  s21_mnrSprt(&src, &tmp, 0, 0);
  ck_assert_double_eq(tmp.matrix[0][0], 5);
  ck_assert_double_eq(tmp.matrix[1][1], 9);

  s21_remove_matrix(&src);
  s21_remove_matrix(&tmp);
}
END_TEST

START_TEST(test_determinant_1x1_2) {
  matrix_t A;
  s21_create_matrix(1, 1, &A);
  A.matrix[0][0] = 5.5;
  double res = 0;
  ck_assert_int_eq(s21_determinant(&A, &res), 0);
  ck_assert_double_eq(res, 5.5);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_3x3) {
  matrix_t A;
  s21_create_matrix(3, 3, &A);
  double val = 1;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) A.matrix[i][j] = val++;

  double res = 0;
  ck_assert_int_eq(s21_determinant(&A, &res), 0);
  ck_assert_double_eq_tol(res, 0, 1e-6);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_mult_matrix_valid) {
  matrix_t A, B, res, expected;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &B);
  s21_create_matrix(2, 2, &expected);

  double vals_a[2][3] = {{1, 2, 3}, {4, 5, 6}};
  double vals_b[3][2] = {{7, 8}, {9, 10}, {11, 12}};
  double vals_exp[2][2] = {{58, 64}, {139, 154}};

  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 3; j++) A.matrix[i][j] = vals_a[i][j];

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 2; j++) B.matrix[i][j] = vals_b[i][j];

  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 2; j++) expected.matrix[i][j] = vals_exp[i][j];

  ck_assert_int_eq(s21_mult_matrix(&A, &B, &res), 0);
  ck_assert_int_eq(s21_eq_matrix(&res, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&res);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_mult_matrix_null_b) {
  matrix_t a, res;
  s21_create_matrix(2, 2, &a);
  s21_create_matrix(2, 2, &res);
  ck_assert_int_eq(s21_mult_matrix(&a, NULL, &res), 1);
  s21_remove_matrix(&a);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_transpose) {
  matrix_t A, res, expected;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &expected);

  int count = 1;
  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 3; j++) A.matrix[i][j] = count++;

  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 3; j++) expected.matrix[j][i] = A.matrix[i][j];

  ck_assert_int_eq(s21_transpose(&A, &res), 0);
  ck_assert_int_eq(s21_eq_matrix(&res, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&res);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_inverse_matrix_invalid) {
  matrix_t A, result;
  s21_create_matrix(2, 2, &A);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 4;

  int res = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(res, 2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_eq_matrix_not_equal) {
  matrix_t A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 2.0;
  B.matrix[1][0] = 3.0;
  B.matrix[1][1] = 4.0001;

  int res = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(res, FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_create_matrix_large_size) {
  matrix_t m;
  int res = s21_create_matrix(1000, 1000, &m);
  ck_assert_int_eq(res, 0);
  m.matrix[0][0] = 1.0;
  m.matrix[999][999] = 2.0;
  ck_assert_double_eq(m.matrix[0][0], 1.0);
  ck_assert_double_eq(m.matrix[999][999], 2.0);
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_sum_matrix_precision_extremes) {
  matrix_t a, b, res;
  s21_create_matrix(2, 2, &a);
  s21_create_matrix(2, 2, &b);

  a.matrix[0][0] = 1e-9;
  a.matrix[0][1] = 1e9;
  a.matrix[1][0] = -1e-9;
  a.matrix[1][1] = -1e9;

  b.matrix[0][0] = 1e-9;
  b.matrix[0][1] = -1e9;
  b.matrix[1][0] = 1e-9;
  b.matrix[1][1] = 1e9;

  ck_assert_int_eq(s21_sum_matrix(&a, &b, &res), 0);
  ck_assert_double_eq_tol(res.matrix[0][0], 2e-9, 1e-6);
  ck_assert_double_eq_tol(res.matrix[0][1], 0, 1e-6);
  ck_assert_double_eq_tol(res.matrix[1][0], 0, 1e-6);
  ck_assert_double_eq_tol(res.matrix[1][1], 0, 1e-6);

  s21_remove_matrix(&a);
  s21_remove_matrix(&b);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_determinant_4x4) {
  matrix_t m;
  s21_create_matrix(4, 4, &m);
  int cnt = 1;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      m.matrix[i][j] = cnt++;
    }
  }
  double res_val = 0;
  ck_assert_int_eq(s21_determinant(&m, &res_val), 0);
  ck_assert_double_eq_tol(res_val, 0, 1e-6);
  s21_remove_matrix(&m);
}
END_TEST

START_TEST(test_inverse_matrix_success_3x3) {
  matrix_t m, inv, expected;
  s21_create_matrix(3, 3, &m);
  s21_create_matrix(3, 3, &expected);

  m.matrix[0][0] = 4;
  m.matrix[0][1] = 7;
  m.matrix[0][2] = 2;
  m.matrix[1][0] = 3;
  m.matrix[1][1] = 6;
  m.matrix[1][2] = 1;
  m.matrix[2][0] = 2;
  m.matrix[2][1] = 5;
  m.matrix[2][2] = 1;

  expected.matrix[0][0] = 0.333333333;
  expected.matrix[0][1] = 1;
  expected.matrix[0][2] = -1.66666667;
  expected.matrix[1][0] = -0.333333333;
  expected.matrix[1][1] = 0;
  expected.matrix[1][2] = 0.66666667;
  expected.matrix[2][0] = 1;
  expected.matrix[2][1] = -2;
  expected.matrix[2][2] = 1;

  int code = s21_inverse_matrix(&m, &inv);
  ck_assert_int_eq(code, 0);

  ck_assert_int_eq(s21_eq_matrix(&inv, &expected), SUCCESS);

  s21_remove_matrix(&m);
  s21_remove_matrix(&inv);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_operations_invalid_matrix_struct) {
  matrix_t a = {0}, b = {0}, result = {0};

  ck_assert_int_eq(s21_sum_matrix(&a, &b, &result), 1);
  ck_assert_int_eq(s21_mult_matrix(&a, &b, &result), 1);
  ck_assert_int_eq(s21_transpose(&a, &result), 1);
  double det = 0;
  ck_assert_int_eq(s21_determinant(&a, &det), 1);

  s21_remove_matrix(&a);
  s21_remove_matrix(&b);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_mult_matrix_uninitialized_result) {
  matrix_t A, B, res;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  double va[2][2] = {{1, 2}, {3, 4}};
  double vb[2][2] = {{5, 6}, {7, 8}};
  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 2; j++) {
      A.matrix[i][j] = va[i][j];
      B.matrix[i][j] = vb[i][j];
    }
  int code = s21_mult_matrix(&A, &B, &res);
  ck_assert_int_eq(code, 0);
  ck_assert_double_eq_tol(res.matrix[0][0], 19.0, 1e-7);
  ck_assert_double_eq_tol(res.matrix[0][1], 22.0, 1e-7);
  ck_assert_double_eq_tol(res.matrix[1][0], 43.0, 1e-7);
  ck_assert_double_eq_tol(res.matrix[1][1], 50.0, 1e-7);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_calc_complements_uninitialized_result) {
  matrix_t A, res;
  s21_create_matrix(2, 2, &A);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 4;
  int code = s21_calc_complements(&A, &res);
  ck_assert_int_eq(code, 0);
  ck_assert_double_eq_tol(res.matrix[0][0], 4.0, 1e-7);
  ck_assert_double_eq_tol(res.matrix[0][1], -3.0, 1e-7);
  ck_assert_double_eq_tol(res.matrix[1][0], -2.0, 1e-7);
  ck_assert_double_eq_tol(res.matrix[1][1], 1.0, 1e-7);
  s21_remove_matrix(&A);
  s21_remove_matrix(&res);
}
END_TEST

Suite *s21_matrix_suite(void) {
  Suite *s = suite_create("s21_matrix");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_create_matrix_success);
  tcase_add_test(tc_core, test_create_matrix_invalid_size);
  tcase_add_test(tc_core, test_create_matrix_null_pointer);

  tcase_add_test(tc_core, test_eq_matrix_success);
  tcase_add_test(tc_core, test_eq_matrix_different_size);

  tcase_add_test(tc_core, test_sum_matrix_success);
  tcase_add_test(tc_core, test_sum_matrix_null_input);

  tcase_add_test(tc_core, test_inverse_a_null);
  tcase_add_test(tc_core, test_inverse_matrix_success_2x2);
  tcase_add_test(tc_core, test_inverse_matrix_singular);

  tcase_add_test(tc_core, test_sub_matrix_fail);
  tcase_add_test(tc_core, test_sub_matrix_success);
  tcase_add_test(tc_core, test_sub_matrix_diff_size);

  tcase_add_test(tc_core, test_mult_number_result_null);
  tcase_add_test(tc_core, test_mult_number_success);
  tcase_add_test(tc_core, test_mult_number_null);

  tcase_add_test(tc_core, test_transpose_success);

  tcase_add_test(tc_core, test_calc_complements_null_a);
  tcase_add_test(tc_core, test_calc_complements_1x1);
  tcase_add_test(tc_core, test_calc_complements_success);
  tcase_add_test(tc_core, test_calc_complements_not_square);

  tcase_add_test(tc_core, test_determinant_a_null);
  tcase_add_test(tc_core, test_determinant_1x1);
  tcase_add_test(tc_core, test_determinant_2x2);
  tcase_add_test(tc_core, test_determinant_not_square);

  tcase_add_test(tc_core, test_mnrSprt_basic);

  tcase_add_test(tc_core, test_determinant_1x1_2);
  tcase_add_test(tc_core, test_determinant_3x3);
  tcase_add_test(tc_core, test_mult_matrix_null_b);
  tcase_add_test(tc_core, test_mult_matrix_valid);
  tcase_add_test(tc_core, test_transpose);
  tcase_add_test(tc_core, test_inverse_matrix_invalid);

  tcase_add_test(tc_core, test_eq_matrix_not_equal);

  tcase_add_test(tc_core, test_create_matrix_large_size);
  tcase_add_test(tc_core, test_sum_matrix_precision_extremes);
  tcase_add_test(tc_core, test_determinant_4x4);
  tcase_add_test(tc_core, test_inverse_matrix_success_3x3);
  tcase_add_test(tc_core, test_operations_invalid_matrix_struct);

  tcase_add_test(tc_core, test_mult_matrix_uninitialized_result);
  tcase_add_test(tc_core, test_calc_complements_uninitialized_result);

  suite_add_tcase(s, tc_core);
  return s;
}

int main(void) {
  int number_failed;
  Suite *s = s21_matrix_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}