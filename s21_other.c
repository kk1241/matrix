#include "s21_other.h"

int s21_is_incorrect_matrix(matrix_t* A) {
  if (A != NULL && A->matrix != NULL && A->rows > 0 && A->columns > 0) {
    return 0;
  }
  return 1;
}

int s21_matrix_compare(matrix_t* A, matrix_t* B) {
  if (s21_is_incorrect_matrix(A) || s21_is_incorrect_matrix(B)) {
    return 1;
  }
  if (A->rows != B->rows || A->columns != B->columns) {
    return 1;
  }
  return 0;
}

int s21_get_minor(matrix_t* src, int row, int col, matrix_t* minor) {
  if (s21_is_incorrect_matrix(src) || row < 0 || row >= src->rows || col < 0 ||
      col >= src->columns || minor == NULL) {
    return CALCULATION_ERROR;
  }

  if (s21_create_matrix(src->rows - 1, src->columns - 1, minor) != OK) {
    return CALCULATION_ERROR;
  }

  int m_i = 0, m_j = 0;

  for (int i = 0; i < src->rows; i++) {
    if (i == row) continue;

    m_j = 0;
    for (int j = 0; j < src->columns; j++) {
      if (j == col) continue;

      minor->matrix[m_i][m_j] = src->matrix[i][j];
      m_j++;
    }
    m_i++;
  }

  return OK;
}
