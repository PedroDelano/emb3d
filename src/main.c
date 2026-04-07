#include "matrix.h"

int main(void) {
    matrix *m = matrix_new(3, 3);
    m->data[0][0] = 1.0;
    m->data[1][1] = 2.0;
    m->data[2][2] = 3.0;
    matrix_print(m);
    matrix_free(m);
    return 0;
}
