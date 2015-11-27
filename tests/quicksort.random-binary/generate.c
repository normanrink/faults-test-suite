/* This program is for generating a C source file that contains the
   input array. Hence this file is not part of the usual build system.
 */

#include <stdio.h>
#include <stdlib.h>


#define SIZE 32000

long array[SIZE];


int main() {
  FILE *out = fopen("quicksort.random-binary.input.c", "w");

  srand(0);
  for (unsigned i = 0; i < SIZE; i++) {
    array[i] = rand() % 2;
  }

  for (unsigned a = 0; a < 2; a++) {
    fprintf(out, "\nlong a%s[%u] = {", (a == 0) ? "0" : "1", SIZE);
    for (unsigned i = 0; i < SIZE; i++) {
      fprintf(out, "%s0x%08lx,", (i % 10) ? " " : "\n", array[i]);
    }
    fprintf(out, "\n};\n");
  }

  fclose(out);

  return 0;
}
