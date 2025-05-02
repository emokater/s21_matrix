#include "s21_helpers.h"

void s21_free(matrix_t *A, int j) {
  for (int i = 0; i < j; i++) {
    free(A->matrix[i]);
  }
  free(A->matrix);
}

int s21_exist(matrix_t *A) {
  int res = 1;
  if (A == NULL || A->matrix == NULL || A->rows <= 0 || A->columns <= 0) {
    res = 0;
  }
  return res;
}

int s21_eqSize(matrix_t *A, matrix_t *B) {
  int res = 1;
  if (A->rows != B->rows || A->columns != B->columns) {
    res = 0;
  }
  return res;
}

void s21_mnrSprt(matrix_t *A, matrix_t *tmp, int i, int j) {
  int r = 0, c = 0;
  for (int k = 0; k < A->rows; k++) {
    for (int l = 0; l < A->columns; l++) {
      if (k != i && l != j) {
        tmp->matrix[r][c] = A->matrix[k][l];
        c++;
      }
    }
    if (k != i) {
      r++;
    }
    c = 0;
  }
}

void s21_detMore3(matrix_t *A, double *result, int *res) {
  matrix_t tmp = {};
  double tmpRes = 0.0;
  if (s21_create_matrix(A->rows - 1, A->columns - 1, &tmp) == 0) {
    for (int i = 0; i < A->columns; i++) {
      tmpRes = 0.0;
      s21_mnrSprt(A, &tmp, 0, i);
      s21_determinant(&tmp, &tmpRes);
      if (i % 2 == 0) {
        *result += A->matrix[0][i] * tmpRes;
      } else {
        *result -= A->matrix[0][i] * tmpRes;
      }
    }
    s21_remove_matrix(&tmp);
  } else {
    *res = 1;
  }
}