#include "s21_helpers.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int res = 0;

  if (rows <= 0 || columns <= 0 || result == NULL) {
    if (result) {
      result->matrix = NULL;
      result->rows = 0;
      result->columns = 0;
    }
    res = 1;
  } else {
    result->rows = rows;
    result->columns = columns;

    if ((result->matrix = calloc(rows, sizeof(double *))) == NULL) {
      result->rows = 0;
      result->columns = 0;
      res = 1;
    } else {
      for (int i = 0; i < rows && !res; i++) {
        if ((result->matrix[i] = calloc(columns, sizeof(double))) == NULL) {
          res = 1;
          s21_free(result, i);
          result->matrix = NULL;
          result->rows = 0;
          result->columns = 0;
        }
      }
    }
  }

  return res;
}

void s21_remove_matrix(matrix_t *A) {
  if (A != NULL) {
    if (s21_exist(A)) {
      s21_free(A, A->rows);
    }
    A->rows = 0;
    A->columns = 0;
    A->matrix = NULL;
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int res = SUCCESS;
  const double EPS = 1e-7;

  if (s21_exist(A) && s21_exist(B) && s21_eqSize(A, B)) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns && res; j++) {
        double diff = A->matrix[i][j] - B->matrix[i][j];
        if (fabs(diff) >= EPS) {
          res = FAILURE;
        }
      }
    }
  } else {
    res = FAILURE;
  }

  return res;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = 0;

  if (!s21_exist(A) || !s21_exist(B) || result == NULL) {
    res = 1;
  } else {
    result->matrix = NULL;
    result->rows = 0;
    result->columns = 0;
    res = s21_create_matrix(A->rows, A->columns, result);
    if (res == 0) {
      res = s21_eqSize(A, B) ? 0 : 2;
      if (res != 0) {
        s21_remove_matrix(result);
      }
    }
  }

  if (res == 0) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  }

  return res;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = 0;

  if (!s21_exist(A) || !s21_exist(B) || result == NULL) {
    res = 1;
  } else {
    result->matrix = NULL;
    result->rows = 0;
    result->columns = 0;
    res = s21_create_matrix(A->rows, A->columns, result);
    if (res == 0) {
      res = s21_eqSize(A, B) ? 0 : 2;
      if (res != 0) {
        s21_remove_matrix(result);
      }
    }
  }

  if (res == 0) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  }

  return res;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int res = 0;

  if (!s21_exist(A) || result == NULL) {
    res = 1;
  } else {
    result->matrix = NULL;
    result->rows = 0;
    result->columns = 0;
    res = s21_create_matrix(A->rows, A->columns, result);
  }

  if (!res) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  }

  return res;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = 0;

  if (!s21_exist(A) || !s21_exist(B) || result == NULL) {
    res = 1;
  } else {
    result->matrix = NULL;
    result->rows = 0;
    result->columns = 0;
    res = s21_create_matrix(A->rows, B->columns, result);
    if (!res) {
      res = (A->columns == B->rows ? 0 : 2);
      if (res != 0) {
        s21_remove_matrix(result);
      }
    }
  }

  if (res == 0) {
    for (int i = 0; i < result->rows; i++) {
      for (int j = 0; j < result->columns; j++) {
        result->matrix[i][j] = 0;
        for (int k = 0; k < A->columns; k++) {
          result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
        }
      }
    }
  }

  return res;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int res = 0;

  if (!s21_exist(A) || result == NULL) {
    res = 1;
  } else {
    result->matrix = NULL;
    result->rows = 0;
    result->columns = 0;
    res = s21_create_matrix(A->columns, A->rows, result);
  }

  if (!res) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[j][i] = A->matrix[i][j];
      }
    }
  }

  return res;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int res = 0;

  if (!s21_exist(A) || result == NULL) {
    res = 1;
  } else {
    result->matrix = NULL;
    result->rows = 0;
    result->columns = 0;
    res = s21_create_matrix(A->rows, A->columns, result);
    if (!res) {
      res = (A->columns == A->rows ? 0 : 2);
      if (res != 0) {
        s21_remove_matrix(result);
      }
    }
  }

  if (res == 0) {
    matrix_t tmp = {};
    if (A->rows == 1) {
      result->matrix[0][0] = 1;
    } else if (!s21_create_matrix(A->rows - 1, A->columns - 1, &tmp)) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          double minor = 0;
          s21_mnrSprt(A, &tmp, i, j);
          s21_determinant(&tmp, &minor);
          result->matrix[i][j] = ((i + j) % 2 == 0 ? 1 : -1) * minor;
        }
      }
      s21_remove_matrix(&tmp);
    } else {
      res = 1;
      s21_remove_matrix(result);
    }
  }

  return res;
}

int s21_determinant(matrix_t *A, double *result) {
  int res = 0;

  if (result == NULL || !s21_exist(A)) {
    res = 1;
  } else {
    *result = 0;
    if (A->rows != A->columns) {
      res = 2;
    }
  }

  if (res == 0) {
    if (A->rows == 1) {
      *result = A->matrix[0][0];
    } else if (A->rows == 2) {
      *result =
          A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
    } else {
      s21_detMore3(A, result, &res);
    }
  }

  return res;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  double det = 0;
  int res = 0;
  matrix_t tmp1 = {0};
  matrix_t tmp2 = {0};

  if (!s21_exist(A) || result == NULL) {
    res = 1;
  } else {
    result->matrix = NULL;
    result->rows = 0;
    result->columns = 0;
  }

  if (!res) {
    s21_determinant(A, &det);
    if (det != 0) {
      res = s21_calc_complements(A, &tmp1);

      if (res == 0) {
        res = s21_transpose(&tmp1, &tmp2);

        if (res == 0) {
          res = s21_mult_number(&tmp2, 1 / det, result);
        }
      }

      s21_remove_matrix(&tmp1);
      s21_remove_matrix(&tmp2);
    } else {
      res = 2;
    }
  }

  return res;
}