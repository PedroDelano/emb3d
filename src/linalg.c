#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "linalg.h"

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

  matrix *r = matrix_new(a->num_rows, a->num_cols);

  for (unsigned int i = 0; i < a->num_rows; i++) {
    for (unsigned int j = 0; j < a->num_rows; j++) {
      r->data[j][i] = a->data[i][j];
    }
  }

  return r;
}


