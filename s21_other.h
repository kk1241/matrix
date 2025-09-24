#ifndef S21_OTHER_H
#define S21_OTHER_H

#include "s21_matrix.h"

int s21_is_incorrect_matrix(matrix_t* A);
int s21_matrix_compare(matrix_t* A, matrix_t* B);
int s21_get_minor(matrix_t* src, int row, int col, matrix_t* minor);

#endif