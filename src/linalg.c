#include "linalg.h"
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

/***************************************************
 * BASIC MATRIX OPERATIONS
 ***************************************************/

matrix *matrix_add(matrix *a, matrix *b) {

  if (a->num_rows != b->num_rows || a->num_cols != b->num_cols) {
    printf("ERROR: cannot add matrices of different sizes");
    return NULL;
  }

  matrix *c = matrix_new(a->num_rows, a->num_cols);

  for (unsigned int i = 0; i < a->num_rows; i++) {
    for (unsigned int j = 0; j < a->num_cols; j++) {
      c->data[i][j] = a->data[i][j] + b->data[i][j];
    }
  }

  return c;
}

matrix *matrix_sub(matrix *a, matrix *b) {

  if (a->num_rows != b->num_rows || a->num_cols != b->num_cols) {
    printf("ERROR: cannot add matrices of different sizes");
    return NULL;
  }

  matrix *c = matrix_new(a->num_rows, a->num_cols);

  for (unsigned int i = 0; i < a->num_rows; i++) {
    for (unsigned int j = 0; j < a->num_cols; j++) {
      c->data[i][j] = a->data[i][j] - b->data[i][j];
    }
  }

  return c;
}

matrix *matrix_scalar_mult(matrix *a, float value) {

  matrix *r = matrix_new(a->num_rows, a->num_cols);

  for (unsigned int i = 0; i < a->num_rows; i++) {
    for (unsigned int j = 0; j < a->num_rows; j++) {
      r->data[i][j] = a->data[i][j] * value;
    }
  }

  return r;
}



matrix *matrix_transpose(matrix *a) {
  // transpose a matrix
  matrix *r = matrix_new(a->num_cols, a->num_rows);
  for (unsigned int i = 0; i < a->num_rows; i++) {
    for (unsigned int j = 0; j < a->num_cols; j++) {
      r->data[j][i] = a->data[i][j];
    }
  }
  return r;
}

matrix *matrix_mult(matrix *a, matrix *b) {

  if (a->num_cols != b->num_rows) {
    printf("ERROR: cannot multiply a (%d, %d) matrix by a (%d, %d) matrix.",
           a->num_rows, a->num_cols, b->num_rows, b->num_cols);
    return NULL;
  }

  matrix *r = matrix_new(a->num_rows, b->num_cols);

  for (unsigned int i = 0; i < a->num_rows; i++) {
    for (unsigned int j = 0; j < b->num_cols; j++) {

      double sum = 0;

      for (unsigned int k = 0; k < a->num_cols; k++) {
        sum += a->data[i][k] * b->data[k][j];
      }

      r->data[i][j] = sum;
    }
  }

  return r;
}
