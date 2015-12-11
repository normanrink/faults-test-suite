/* This program is for generating a C source file that contains the
   input array. Hence this file is not part of the usual build system.
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>

const char input[] = "+ * 5 (/ -100 -20) (- -4 / 10 5)";
// with parenthesis - for better readability:
// const char input[] = "+ (* 5 (/ -100 -20)) (- -4 / 10 5)";

int main() {
  FILE *fout = fopen("expr.input.c", "w");

  fprintf(fout, "\n#include <stdint.h>\n\n");
  fprintf(fout, "\nuint64_t input[%u] = {", strlen(input)+1);
  unsigned i = 0;
  for (; i < strlen(input); i++) {
    uint64_t c = (uint64_t)input[i];
    fprintf(fout, "%s0x%08lx,", (i % 10) ? " " : "\n", c);
  }
  fprintf(fout, "%s0x%08lx,", (i % 10) ? " " : "\n", 0L);
  fprintf(fout, "\n};\n");
  fclose(fout);

  return 0;
}
