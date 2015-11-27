/* This file is intended for generating a C source file which
   contains the array 'AdjMatrix', which, in turn, is input
   for the 'dijkstra' test case.

   This file is not part of the usual build system.

   Recommended usage:
   generate 100 dijkstra_input.dat

   Adapted from Dmitry Kuvayskiy's test for his
   "AN-Transformer".
 */

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
  unsigned i, j;
  long num_nodes;
  long *AdjMatrix;
  FILE *fmatrix, *fout;

  if (argc < 3)
  {
    fprintf(stderr, "Usage: dijkstra <num_nodes> <filename>\n");
    fprintf(stderr, "Only supports matrix size is #define'd.\n");
  }

  num_nodes = atol(argv[1]);

  /* open the adjacency matrix file: */
  fmatrix = fopen(argv[2], "r");
  if (!fmatrix)
    return -1;

  AdjMatrix = (long*)malloc(sizeof(long)*num_nodes*num_nodes);

  /* generate a symmetric adjacency matrix: */
  for (i = 0; i < num_nodes; i++) {
    for (j = 0; j <= i; j++) {
      long k;
      fscanf(fmatrix, "%ld", &k);
      AdjMatrix[i*num_nodes+j] = k;
      AdjMatrix[j*num_nodes+i] = k;
    }
  }

  fclose(fmatrix);

  /* check symmetry: */
  for (long i = 0; i < num_nodes; i++) {
    for (long j = 0; j < num_nodes; j++) {
      if (AdjMatrix[i*num_nodes+j] != AdjMatrix[j*num_nodes+i])
        return -1;
    }
  }

  fout = fopen("dijkstra.adjacency.c", "w");
  fprintf(fout, "\nlong AdjMatrix[%ld*%ld] = {", num_nodes, num_nodes);
  for (long i = 0; i < num_nodes; i++) {
    for (long j = 0; j < num_nodes; j++) {
      fprintf(fout, "%s%4ld,", ((i*num_nodes+j) % 20) ? "" : "\n",
                               AdjMatrix[i*num_nodes+j]);
    }
  }
  fprintf(fout, "\n};\n");
  fprintf(fout, "\nconst long max_num_nodes = %ld;\n", num_nodes);

  fclose(fout);

  return 0;
}
