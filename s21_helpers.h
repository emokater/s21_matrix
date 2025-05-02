#ifndef S21_HELPERS_H
#define S21_HELPERS_H

#include "s21_matrix.h"

void s21_free(matrix_t *mtr, int j);
int s21_exist(matrix_t *A);
int s21_eqSize(matrix_t *A, matrix_t *B);
int s21_result_check(int rows, int columns, matrix_t *result);
void s21_mnrSprt(matrix_t *A, matrix_t *tmp, int i, int j);

#endif