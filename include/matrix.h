#ifndef MATRIX_H
#define MATRIX_H

typedef struct matrix_s {
  unsigned int num_rows;
  unsigned int num_cols;
  double **data;
  int is_square;
} matrix;

matrix *matrix_new(unsigned int num_rows, unsigned int num_cols);
matrix *matrix_new_sqr(unsigned int n);
matrix *matrix_new_eye(unsigned int n);
void    matrix_free(matrix *m);
void    matrix_print(const matrix *m);

#endif
