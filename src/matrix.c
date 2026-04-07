#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

// Constructor: Allocates a new matrix on the heap
// All elements are initialized to 0.0
matrix *matrix_new(unsigned int num_rows, unsigned int num_cols) {

  if (num_rows == 0 || num_cols == 0) {
    printf("ERROR: cannot create matrix with size 0 (row or col)");
    return NULL;
  }

  // allocate matrix in memory
  matrix *m = malloc(sizeof(*m));

  m->num_rows = num_rows;
  m->num_cols = num_cols;

  if (num_rows == num_cols) {
    m->is_square = 1;
  } else {
    m->is_square = 0;
  }

  // allocate the array of row pointers
  m->data = malloc(m->num_rows * sizeof(*m->data));

  // allocate each row
  for (unsigned int i = 0; i < m->num_rows; i++) {
    m->data[i] = malloc(m->num_cols * sizeof(**m->data));
  }

  // assign all elements to 0
  for (unsigned int i = 0; i < num_rows; i++) {
    for (unsigned int j = 0; j < num_cols; j++) {
      m->data[i][j] = 0;
    }
  }

  return m;
}

// Destructor: Safely releases all memory associated with the matrix
void matrix_free(matrix *m) {
  if (m == NULL) {
    return;
  }
  for (unsigned int i = 0; i < m->num_rows; i++) {
    free(m->data[i]);
  }
  free(m->data);
  free(m);
}

matrix *matrix_new_sqr(unsigned int n) {
  // Creates a square matrix N x N
  return matrix_new(n, n);
}

matrix *matrix_new_eye(unsigned int n) {
  // Creates an identity matrix
  matrix *m = matrix_new(n, n);
  for (unsigned int i = 0; i < m->num_rows; i++) {
    m->data[i][i] = 1;
  }
  return m;
}

void matrix_print(const matrix *m) {
  for (unsigned int i = 0; i < m->num_rows; i++) {
    printf("| ");
    for (unsigned int j = 0; j < m->num_cols; j++) {
      printf("%8.2f ", m->data[i][j]);
    }
    printf("|\n");
  }
}

// ************************************************************
// MATRIX FILE FORMAT
// ************************************************************

// void matrix_to_file(){}
// matrix *matrix_from_file(){}
