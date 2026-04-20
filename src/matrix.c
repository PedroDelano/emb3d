#include "matrix.h"
#include "utils.h"
#include <assert.h>
#include <math.h>
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

matrix *matrix_new_rand(unsigned int num_rows, unsigned int num_cols,
                        int norm) {

  assert(norm == 1 || norm == 0);

  matrix *m = matrix_new(num_rows, num_cols);

  for (unsigned int i = 0; i < num_rows; i++) {
    for (unsigned int j = 0; j < num_cols; j++) {
      // defines a value between 0 and 1
      double val = (double)rand() / (double)RAND_MAX;
      m->data[i][j] = val;
    }
  }

  if (norm == 0) {
    return m;
  }

  double row_sum = 0;

  // row-wise normalization
  for (unsigned int i = 0; i < num_rows; i++) {
    row_sum = 0;
    for (unsigned int j = 0; j < num_cols; j++) {
      row_sum += m->data[i][j] * m->data[i][j];
    }
    if (row_sum == 0) {
      continue;
    }
    for (unsigned int j = 0; j < num_cols; j++) {
      m->data[i][j] = m->data[i][j] / sqrt(row_sum);
    }
  }

  return m;
}

// Box-Muller: standard normal sample N(0, 1)
static double rand_normal(void) {
  static const double TWO_PI = 6.28318530717958647692;
  double u1, u2;
  do {
    u1 = (double)rand() / (double)RAND_MAX;
  } while (u1 == 0.0);
  u2 = (double)rand() / (double)RAND_MAX;
  return sqrt(-2.0 * log(u1)) * cos(TWO_PI * u2);
}

// Orthogonal initialization (Saxe et al. 2013).
// Builds a Gaussian random matrix and orthonormalizes via modified Gram-Schmidt
// (the QR path PyTorch's nn.init.orthogonal_ takes). When the requested shape
// is wide (cols > rows), we work on the transpose so the long axis is reduced
// — the result has orthonormal rows instead of orthonormal columns.
matrix *matrix_new_orthogonal(unsigned int num_rows, unsigned int num_cols) {

  unsigned int work_rows = num_rows;
  unsigned int work_cols = num_cols;
  int transposed = 0;
  if (num_cols > num_rows) {
    work_rows = num_cols;
    work_cols = num_rows;
    transposed = 1;
  }

  matrix *a = matrix_new(work_rows, work_cols);
  for (unsigned int i = 0; i < work_rows; i++) {
    for (unsigned int j = 0; j < work_cols; j++) {
      a->data[i][j] = rand_normal();
    }
  }

  // Modified Gram-Schmidt over columns of a.
  for (unsigned int j = 0; j < work_cols; j++) {
    double norm = 0;
    for (unsigned int i = 0; i < work_rows; i++) {
      norm += a->data[i][j] * a->data[i][j];
    }
    norm = sqrt(norm);
    assert(norm > 0 && "degenerate column during orthogonalization");
    for (unsigned int i = 0; i < work_rows; i++) {
      a->data[i][j] /= norm;
    }
    for (unsigned int k = j + 1; k < work_cols; k++) {
      double proj = 0;
      for (unsigned int i = 0; i < work_rows; i++) {
        proj += a->data[i][j] * a->data[i][k];
      }
      for (unsigned int i = 0; i < work_rows; i++) {
        a->data[i][k] -= proj * a->data[i][j];
      }
    }
  }

  // Verify orthonormality of the columns of `a`: a^T a should equal I.
  // Floating-point tolerance scales with the inner-product summation length.
  const double tol = 1e-9 * (double)work_rows;
  for (unsigned int i = 0; i < work_cols; i++) {
    for (unsigned int j = i; j < work_cols; j++) {
      double ip = 0;
      for (unsigned int k = 0; k < work_rows; k++) {
        ip += a->data[k][i] * a->data[k][j];
      }
      double expected = (i == j) ? 1.0 : 0.0;
      assert(fabs(ip - expected) < tol &&
             "orthogonal init failed: columns are not orthonormal");
    }
  }

  if (transposed) {
    matrix *t = matrix_new(num_rows, num_cols);
    for (unsigned int i = 0; i < work_rows; i++) {
      for (unsigned int j = 0; j < work_cols; j++) {
        t->data[j][i] = a->data[i][j];
      }
    }
    matrix_free(a);
    return t;
  }
  return a;
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

  assert(m->num_rows > 0);
  assert(m->num_cols > 0);

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
      printf("%9.4f", m->data[i][j]);
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
// MATRIX FILE FORMAT (binary: rows, cols, then row-major doubles)
// ************************************************************

int matrix_save(matrix *m, const char *fpath) {
  FILE *f = fopen(fpath, "wb");
  if (f == NULL)
    return -1;
  fwrite(&m->num_rows, sizeof(unsigned int), 1, f);
  fwrite(&m->num_cols, sizeof(unsigned int), 1, f);
  for (unsigned int i = 0; i < m->num_rows; i++) {
    fwrite(m->data[i], sizeof(double), m->num_cols, f);
  }
  fclose(f);
  return 0;
}

matrix *matrix_load(const char *fpath) {
  FILE *f = fopen(fpath, "rb");
  if (f == NULL)
    return NULL;
  unsigned int rows, cols;
  fread(&rows, sizeof(unsigned int), 1, f);
  fread(&cols, sizeof(unsigned int), 1, f);
  matrix *m = matrix_new(rows, cols);
  for (unsigned int i = 0; i < rows; i++) {
    fread(m->data[i], sizeof(double), cols, f);
  }
  fclose(f);
  return m;
}
