#include "linalg.h"
#include "matrix.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/***************************************************
 * BASIC MATRIX OPERATIONS
 ***************************************************/

void matrix_add_r(matrix *a, matrix *b) {
  for (unsigned int i = 0; i < a->num_rows; i++) {
    for (unsigned int j = 0; j < a->num_cols; j++) {
      a->data[i][j] = a->data[i][j] + b->data[i][j];
    }
  }
}

matrix *matrix_add(matrix *a, matrix *b) {
  if (a->num_rows != b->num_rows || a->num_cols != b->num_cols) {
    printf("ERROR: cannot add matrices of different sizes");
    return NULL;
  }
  matrix *c = matrix_copy(a);
  matrix_add_r(c, b);
  return c;
}

void matrix_sub_r(matrix *a, matrix *b) {
  for (unsigned int i = 0; i < a->num_rows; i++) {
    for (unsigned int j = 0; j < a->num_cols; j++) {
      a->data[i][j] = a->data[i][j] - b->data[i][j];
    }
  }
}

matrix *matrix_sub(matrix *a, matrix *b) {
  if (a->num_rows != b->num_rows || a->num_cols != b->num_cols) {
    printf("ERROR: cannot subtract matrices of different sizes");
    return NULL;
  }
  matrix *c = matrix_copy(a);
  matrix_sub_r(c, b);
  return c;
}

void matrix_scalar_mult_r(matrix *a, float value) {
  for (unsigned int i = 0; i < a->num_rows; i++) {
    for (unsigned int j = 0; j < a->num_cols; j++) {
      a->data[i][j] = a->data[i][j] * value;
    }
  }
}

matrix *matrix_scalar_mult(matrix *a, float value) {
  matrix *r = matrix_copy(a);
  matrix_scalar_mult_r(r, value);
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

#pragma omp parallel for schedule(static)
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

/*********************************************************
 * VECTOR OPERATIONS
 ********************************************************/

double dot_product(matrix *a, matrix *b) {
  assert(a->num_rows == 1 && "a must be a (1, N) matrix");
  assert(b->num_cols == 1 && "b must be a (N, 1) matrix");
  matrix *dp = matrix_mult(a, b);
  assert(dp->num_rows == 1);
  assert(dp->num_cols == 1);
  double dp_value = dp->data[0][0];
  matrix_free(dp);
  return dp_value;
}

double vec_norm(matrix *a) {
  double sum = 0;
  for (unsigned int i = 0; i < a->num_rows; i++) {
    for (unsigned int j = 0; j < a->num_cols; j++) {
      sum += a->data[i][j] * a->data[i][j];
    }
  }
  return sqrt(sum);
}

double cosine_similarity(matrix *a, matrix *b) {
  matrix *a_t = matrix_transpose(a);
  double dp = dot_product(a_t, b);
  double norm_a = vec_norm(a);
  double norm_b = vec_norm(b);
  double cosim = dp / (norm_a * norm_b);
  matrix_free(a_t);
  return cosim;
}

/*********************************************************
 * ACTIVATION FUNCTION
 ********************************************************/

matrix *softmax(matrix *m, int apply_rowwise) {

  assert((apply_rowwise == 1 || apply_rowwise == 0) &&
         "ERROR: invalid apply_rowwise value");

  if (apply_rowwise == 1) {

    for (unsigned int j = 0; j < m->num_cols; j++) {
      double row_sum = 0;
      double max = -99999;
      double zi = 0;
      for (unsigned int i = 0; i < m->num_rows; i++) {
        zi = m->data[i][j];
        if (zi > max)
          max = zi;
      }
      for (unsigned int i = 0; i < m->num_rows; i++) {
        row_sum += exp(m->data[i][j] - max);
      }
      for (unsigned int i = 0; i < m->num_rows; i++) {
        m->data[i][j] = exp(m->data[i][j] - max) / row_sum;
      }
    }

  } else {

    for (unsigned int i = 0; i < m->num_rows; i++) {
      double col_sum = 0;
      double max = -99999;
      double zi = 0;
      for (unsigned int j = 0; j < m->num_cols; j++) {
        zi = m->data[i][j];
        if (zi > max)
          max = zi;
      }
      for (unsigned int j = 0; j < m->num_cols; j++) {
        col_sum += exp(m->data[i][j] - max);
      }
      for (unsigned int j = 0; j < m->num_cols; j++) {
        m->data[i][j] = exp(m->data[i][j] - max) / col_sum;
      }
    }
  }

  return m;
}

/*********************************************************
 * LOSS FUNCTION
 ********************************************************/

double cross_entropy_loss(matrix *logits, int token_id) {
  assert(logits->num_cols == 1 && "ERROR: logits must be a column-vector");
  double prob = logits->data[token_id][0];
  return (-1) * log(prob);
}
