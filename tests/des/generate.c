/* This file is used only for generating input arrays. Hence
   this file is not part of the usual build system for the
   test cases.
 */

#include <stdio.h>
#include <assert.h>

int main(int argc, char* argv[]) {
  if (argc < 3) {
    return(1);
  }

  FILE *in  = fopen(argv[1], "rb");
  FILE *out = fopen(argv[2], "w");
  assert(in && out);

  fprintf(out, "\nlong input[] = {");
  unsigned i = 0;
  while (!feof(in)) {
    long ch = fgetc(in);
    fprintf(out, "%s0x%08lx,", (i % 6) ? " " : "\n", ch);
    ++i;
  }
  fprintf(out, "\n};\n");
  fprintf(out, "\nconst long sz_input = %u;\n", i);


  fclose(out);
  fclose(in);

  return 0;
}
