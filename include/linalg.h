#ifndef LINALG_H
#define LINALG_H

#include "matrix.h"

void matrix_add_r(matrix *a, matrix *b);
matrix *matrix_add(matrix *a, matrix *b);
void matrix_sub_r(matrix *a, matrix *b);
matrix *matrix_sub(matrix *a, matrix *b);
void matrix_scalar_mult_r(matrix *a, float value);
matrix *matrix_scalar_mult(matrix *a, float value);
matrix *matrix_transpose(matrix *a);
matrix *matrix_mult(matrix *a, matrix *b);
double dot_product(matrix *a, matrix *b);
matrix *softmax(matrix *m, int apply_rowwise);
double cross_entropy_loss(matrix *logits, int token_id);
double cosine_similarity(matrix *a, matrix *b);

#endif
