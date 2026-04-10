#include "embedding.h"
#include "linalg.h"
#include "matrix.h"

matrix *forward_pass(matrix *embedding_matrix, matrix *embedding_vector) {
  matrix *m = matrix_mult(embedding_matrix, embedding_vector);
  m = softmax(m, 1);
  return m;
}
