
#include <stdio.h>
#include <stdlib.h>

#include "encode.h"
#include "mycheck.h"
#include "mycyc.h"

long *a;
long *b;
long *mat;

extern void ___enc_kernel(long *, long *, long *, long);


int main(int argc, char **argv) {
  unsigned long t1, t2, total = 0;
  const unsigned length = 1024;
  const unsigned repetitions = 1;

  fprintf(stderr, "LENGTH=%d\n", length);
  fprintf(stderr, "REPETITIONS=%d\n", repetitions);

  long *a = (long*)malloc(length * sizeof(long));
  long *b = (long*)malloc(length * sizeof(long));
  long *mat = (long*)malloc(length * length * sizeof(long));

  for (unsigned i = 0; i < length; i++) {
    for (unsigned j = 0; j < length; j++)
      mat[i*length + j] = AN_ENCODE_VALUE(i*j);

    b[i] = AN_ENCODE_VALUE(i);
    a[i] = 0;
  }
  __cs_log(argc, argv);
  __cs_fopen(argc, argv);
  __cs_reset();

  t1 = __cyc_rdtsc();
  ___enc_kernel(a, mat, b, length);
  t2 = __cyc_rdtsc();
  total += t2 - t1;

  for (unsigned i = 0; i < length; i++) {
    __cs_facc(AN_DECODE_VALUE(a[i]));
    __cs_acc(AN_DECODE_VALUE(a[i]));
  }

  __cyc_msg(total);
  __cs_fclose();
  __cs_msg();

  return 0;
}
