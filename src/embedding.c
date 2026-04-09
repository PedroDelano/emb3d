/*********************************
 *
 * Simple embedding implementation
 *
 *********************************/

#include "embedding.h"

const unsigned int EMBEDDING_SIZE = 1024;

matrix *embedding_matrix_new(unsigned int vocab_size) {
  return matrix_new_rand(vocab_size, EMBEDDING_SIZE);
}
