#include "linalg.h"
#include "matrix.h"
#include <stdio.h>

int main(void) {
  matrix *a = matrix_new_eye(4);
  matrix *b = matrix_new_eye(4);

  a->data[0][1] = 2;
  a->data[0][2] = 3;

  b->data[0][1] = 2;
  b->data[0][2] = 3;

  matrix_print(a);
  a = matrix_transpose(a);
  matrix_print(a);

  matrix *c = matrix_add(a, b);
  matrix *d = matrix_sub(a, b);
  matrix *e = matrix_scalar_mult(a, 16);
  matrix *f = matrix_transpose(a);
  matrix *g = matrix_mult(a, b);
  matrix_print(c);
  printf("---");
  matrix_print(d);
  printf("---");
  matrix_print(e);
  printf("---");
  matrix_print(f);
  printf("---");
  matrix_print(g);


  matrix *v1 = matrix_new(1, 3);
  matrix *v2 = matrix_new(1, 3);
  v2 = matrix_transpose(v2);

  v1->data[0][0] = 1;
  v1->data[0][1] = 2;
  v1->data[0][2] = 3;

  v2->data[0][0] = 1;
  v2->data[1][0] = 2;
  v2->data[2][0] = 3;

  matrix_print(v1);
  matrix_print(v2);

  double dp = dot_product(v1, v2);
  printf("%f", dp);

  return 0;
}
