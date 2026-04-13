#include <stdio.h>

int max(int a, int b) {
  if (a >= b) {
    return a;
  }
  return b;
}

int min(int a, int b) {
  if (a <= b) {
    return a;
  }
  return b;
}

int file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1; // Exists
    }
    return 0; // Does not exist
}
