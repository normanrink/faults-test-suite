/* This program is for generating a C source file that contains the
   input array. Hence this file is not part of the usual build system.
 */

#include <stdio.h>


#define SIZE 32000


int main() {
  FILE *out = fopen("array_sum.input.c", "w");

  fprintf(out, "\nlong a[%u] = {", SIZE);
  for (unsigned long i = 0; i < SIZE; i++)
    fprintf(out, "%s0x%08lx,", (i % 10) ? " " : "\n", i);

  fprintf(out, "\n};\n");
  fclose(out);

  return 0;
}
