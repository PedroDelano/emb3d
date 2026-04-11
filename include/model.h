#ifndef MODEL_H
#define MODEL_H

#include "linkedlist.h"
#include "matrix.h"

matrix *forward_pass(matrix *embedding_matrix, matrix *embedding_vector);
matrix *train(Node **token_map, matrix *embedding_matrix, char *fpath,
              int window_size);

#endif
