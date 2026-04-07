#ifndef LINALG_H
#define LINALG_H

matrix *matrix_add(matrix *a, matrix *b);
matrix *matrix_sub(matrix *a, matrix *b);
matrix *matrix_scalar_mult(matrix *a, float value);
matrix *matrix_transpose(matrix *a);

#endif
