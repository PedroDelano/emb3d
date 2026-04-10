#include "matrix.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

const int MAX_DISPLAY_COLUMNS = 5;
const int MAX_DISPLAY_ROWS = 10;

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

matrix *matrix_copy(matrix *a) {
  matrix *r = matrix_new(a->num_rows, a->num_cols);
  for (unsigned int i = 0; i < a->num_rows; i++) {
    for (unsigned int j = 0; j < a->num_cols; j++) {
      r->data[j][i] = a->data[i][j];
    }
  }
  return r;
}

matrix *matrix_to_row_vec(double *vec, int vec_size) {
  matrix *m = matrix_new(1, vec_size);
  for (int i = 0; i < vec_size; i++) {
    m->data[0][i] = vec[i];
  }
  return m;
}

matrix *matrix_to_col_vec(double *vec, int vec_size) {
  matrix *m = matrix_new(vec_size, 1);
  for (int i = 0; i < vec_size; i++) {
    m->data[i][0] = vec[i];
  }
  return m;
}

matrix *matrix_new_sqr(unsigned int n) {
  // Creates a square matrix N x N
  return matrix_new(n, n);
}

matrix *matrix_new_rand(unsigned int num_rows, unsigned int num_cols) {

  matrix *m = matrix_new(num_rows, num_cols);

  for (unsigned int i = 0; i < num_rows; i++) {
    for (unsigned int j = 0; j < num_cols; j++) {
      // defines a value between 0 and 1
      double val = (double)rand() / (double)RAND_MAX;
      m->data[i][j] = val;
    }
  }

  return m;
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
  unsigned int disp_rows = min(m->num_rows, MAX_DISPLAY_ROWS);
  unsigned int disp_cols = min(m->num_cols, MAX_DISPLAY_COLUMNS);
  int trunc_rows = m->num_rows > (unsigned int)MAX_DISPLAY_ROWS;
  int trunc_cols = m->num_cols > (unsigned int)MAX_DISPLAY_COLUMNS;

  // Column header
  printf("\n%6s", "");
  for (unsigned int j = 0; j < disp_cols; j++) {
    printf("%9u", j);
  }
  if (trunc_cols) {
    printf("      ...");
  }
  printf("\n");

  // Data rows
  for (unsigned int i = 0; i < disp_rows; i++) {
    printf("%5u ", i);
    for (unsigned int j = 0; j < disp_cols; j++) {
      printf("%9.2f", m->data[i][j]);
    }
    if (trunc_cols) {
      printf("      ...");
    }
    printf("\n");
  }

  if (trunc_rows) {
    printf("  ...\n");
  }

  printf("\n[%u rows x %u cols]\n\n", m->num_rows, m->num_cols);
}

// ************************************************************
// MATRIX FILE FORMAT
// ************************************************************

// void matrix_to_file(){}
// matrix *matrix_from_file(){}
