#include "s21_matrix.h"

#include "s21_other.h"

int s21_create_matrix(int rows, int columns, matrix_t* result) {
  if (rows <= 0 || columns <= 0 || result == NULL) {
    return INCORRECT_MATRIX;
  }

  result->rows = rows;
  result->columns = columns;

  result->matrix = (double**)calloc(rows, sizeof(double*));

  if (result->matrix == NULL) {
    return INCORRECT_MATRIX;
  }

  for (int i = 0; i < rows; i++) {
    result->matrix[i] = calloc(columns, sizeof(double));
    if (result->matrix[i] == NULL) {
      for (int j = 0; j < i; j++) {
        free(result->matrix[j]);
      }
      free(result->matrix);
      result->matrix = NULL;
      return INCORRECT_MATRIX;
    }
  }

  return OK;
}

void s21_remove_matrix(matrix_t* A) {
  if (A == NULL || A->matrix == NULL) {
    return;
  }
  for (int i = 0; i < A->rows; i++) {
    free(A->matrix[i]);
  }
  free(A->matrix);

  A->matrix = NULL;
  A->rows = 0;
  A->columns = 0;
}

int s21_eq_matrix(matrix_t* A, matrix_t* B) {
  if (!A || !B || A->rows <= 0 || A->columns <= 0 || B->rows <= 0 ||
      B->columns <= 0) {
    return FAILURE;
  }

  if (A->rows != B->rows || A->columns != B->columns) {
    return FAILURE;
  }

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-6) {
        return FAILURE;
      }
    }
  }
  return SUCCESS;
}

int s21_sum_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  if (s21_is_incorrect_matrix(A) || s21_is_incorrect_matrix(B) ||
      result == NULL) {
    return INCORRECT_MATRIX;
  }

  if (s21_matrix_compare(A, B)) {
    return CALCULATION_ERROR;
  }

  if (s21_create_matrix(A->rows, A->columns, result) != OK)
    return INCORRECT_MATRIX;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
    }
  }

  return OK;
}

int s21_sub_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  if (s21_is_incorrect_matrix(A) || s21_is_incorrect_matrix(B) ||
      result == NULL) {
    return INCORRECT_MATRIX;
  }

  if (s21_matrix_compare(A, B)) {
    return CALCULATION_ERROR;
  }

  if (s21_create_matrix(A->rows, A->columns, result) != OK)
    return INCORRECT_MATRIX;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
    }
  }
  return OK;
}

int s21_mult_number(matrix_t* A, double number, matrix_t* result) {
  if (s21_is_incorrect_matrix(A) || result == NULL) {
    return INCORRECT_MATRIX;
  }

  if (s21_create_matrix(A->rows, A->columns, result) != OK)
    return INCORRECT_MATRIX;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] * number;
    }
  }
  return OK;
}

int s21_mult_matrix(matrix_t* A, matrix_t* B, matrix_t* result) {
  if (s21_is_incorrect_matrix(A) || s21_is_incorrect_matrix(B) ||
      result == NULL) {
    return INCORRECT_MATRIX;
  }

  if (A->columns != B->rows) {
    return CALCULATION_ERROR;
  }

  if (s21_create_matrix(A->rows, B->columns, result) != OK) {
    return INCORRECT_MATRIX;
  }

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < B->columns; j++) {
      result->matrix[i][j] = 0.0;
      for (int k = 0; k < A->columns; k++) {
        result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
      }
    }
  }
  return OK;
}

int s21_transpose(matrix_t* A, matrix_t* result) {
  if (s21_is_incorrect_matrix(A)) {
    return INCORRECT_MATRIX;
  }

  if (s21_create_matrix(A->columns, A->rows, result) != OK)
    return INCORRECT_MATRIX;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[j][i] = A->matrix[i][j];
    }
  }
  return OK;
}

int s21_calc_complements(matrix_t* A, matrix_t* result) {
  if (s21_is_incorrect_matrix(A) || result == NULL) {
    return INCORRECT_MATRIX;
  }

  if (A->rows != A->columns) {
    return CALCULATION_ERROR;
  }

  if (A->rows == 1) {
    if (s21_create_matrix(1, 1, result) != OK) {
      return INCORRECT_MATRIX;
    }
    result->matrix[0][0] = 1.0;
    return OK;
  }

  if (s21_create_matrix(A->rows, A->columns, result) != OK) {
    return INCORRECT_MATRIX;
  }

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      matrix_t minor;
      if (s21_get_minor(A, i, j, &minor) != OK) {
        s21_remove_matrix(result);
        return CALCULATION_ERROR;
      }

      double minor_det = 0.0;
      if (s21_determinant(&minor, &minor_det) != OK) {
        s21_remove_matrix(&minor);
        s21_remove_matrix(result);
        return CALCULATION_ERROR;
      }

      int sign = ((i + j) % 2 == 0) ? 1 : -1;
      result->matrix[i][j] = sign * minor_det;

      s21_remove_matrix(&minor);
    }
  }

  return OK;
}

int s21_determinant(matrix_t* A, double* result) {
  if (s21_is_incorrect_matrix(A) || result == NULL) {
    return INCORRECT_MATRIX;
  }

  if (A->rows != A->columns) {
    return CALCULATION_ERROR;
  }

  int size = A->rows;
  if (size == 1) {
    *result = A->matrix[0][0];
    return OK;
  } else if (size == 2) {
    *result = (A->matrix[0][0] * A->matrix[1][1]) -
              (A->matrix[0][1] * A->matrix[1][0]);
    return OK;
  } else {
    double det = 0.0;
    matrix_t minor;
    for (int j = 0; j < size; j++) {
      s21_get_minor(A, 0, j, &minor);
      double minor_det = 0.0;
      s21_determinant(&minor, &minor_det);

      int sign = (j % 2 == 0) ? 1 : -1;

      det += sign * A->matrix[0][j] * minor_det;
      s21_remove_matrix(&minor);
    }
    *result = det;
  }
  return OK;
}

int s21_inverse_matrix(matrix_t* A, matrix_t* result) {
  if (s21_is_incorrect_matrix(A) || result == NULL) {
    return INCORRECT_MATRIX;
  }

  if (A->rows != A->columns) {
    return CALCULATION_ERROR;
  }

  double det = 0.0;
  if (s21_determinant(A, &det) != OK) {
    return INCORRECT_MATRIX;
  }

  if (fabs(det) < 1e-6) {
    return CALCULATION_ERROR;
  }
  if (A->rows == 1) {
    if (s21_create_matrix(1, 1, result) != OK) {
      return INCORRECT_MATRIX;
    }
    result->matrix[0][0] = 1.0 / A->matrix[0][0];
    return OK;
  }

  matrix_t complements, adjoint;
  if (s21_calc_complements(A, &complements) != OK) {
    return CALCULATION_ERROR;
  }
  if (s21_transpose(&complements, &adjoint) != OK) {
    s21_remove_matrix(&complements);
    return CALCULATION_ERROR;
  }

  if (s21_mult_number(&adjoint, 1.0 / det, result) != OK) {
    s21_remove_matrix(&complements);
    s21_remove_matrix(&adjoint);
    return CALCULATION_ERROR;
  }

  s21_remove_matrix(&complements);
  s21_remove_matrix(&adjoint);

  return OK;
}
