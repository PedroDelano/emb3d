#ifndef LINALG_H
#define LINALG_H

#include "matrix.h"

matrix *matrix_add(matrix *a, matrix *b);
matrix *matrix_sub(matrix *a, matrix *b);
matrix *matrix_scalar_mult(matrix *a, float value);
matrix *matrix_transpose(matrix *a);
matrix *matrix_mult(matrix *a, matrix *b);
double dot_product(matrix *a, matrix *b);

#endif
