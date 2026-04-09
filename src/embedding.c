/*********************************
 *
 * Simple embedding implementation
 *
 *********************************/

#include "embedding.h"
#include <assert.h>

const unsigned int EMBEDDING_SIZE = 1024;

matrix *embedding_matrix_new(unsigned int vocab_size) {
  return matrix_new_rand(vocab_size, EMBEDDING_SIZE);
}

double *embedding_retrieve(matrix *embedding_matrix, int token_id) {
  assert(token_id <= (int)embedding_matrix->num_rows &&
         "ERROR: invalid token_id (greater than vocab_size)");
  return embedding_matrix->data[token_id];
}
