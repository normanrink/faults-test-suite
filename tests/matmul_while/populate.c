
/* This is used for generating C-source files that contain large arrays of input
   data. Hence this file is not part of the build system for the test cases.
 */

#include <stdio.h>

#define SIZE 1000

int main() {

  FILE *vector = fopen("matmul_while.vector.c", "w");
  FILE *matrix = fopen("matmul_while.matrix.c", "w");

  fprintf(vector, "long vector[%d] = {", SIZE);
  fprintf(matrix, "long matrix[%d*%d] = {", SIZE, SIZE);

  for (unsigned i = 0; i < SIZE; i++) {
    for (unsigned j = 0; j < SIZE; j++)
      fprintf(matrix, "%s%3d,", ((i*SIZE+j) % 20) ? " " : "\n", (i+1));


    fprintf(vector, "%s%d,", (i % 40) ? " " : "\n", 1);
  }
  fprintf(vector, "\n};\n");
  fprintf(matrix, "\n};\n");

  fclose(vector);
  fclose(matrix);

  return 0;
}

