#include <stdio.h>
#include <stdlib.h>

#include "encode.h"
#include "mycheck.h"
#include "mycyc.h"


const unsigned length = LENGTH;
const unsigned repetitions = REPETITIONS;

long a[length];
long b[length];
long mat[length*length];

extern void ___enc_populate(long *, long *, long *, long);
extern void ___enc_multiply(long *, long *, long *, long);
extern long ___enc_get(long *, long);


int main(int argc, char **argv) {
  unsigned long t1, t2, total = 0;

  fprintf(stderr, "LENGTH=%d\n", length);
  fprintf(stderr, "REPETITIONS=%d\n", repetitions);

  __cs_log(argc, argv);
  __cs_fopen(argc, argv);
  __cs_reset();

  for (unsigned k = 0; k < repetitions; k++) {
    ___enc_populate(a, mat, b, length);

    __cyc_warmup();
    t1 = __cyc_rdtsc();
    ___enc_multiply(a, mat, b, length);
    t2 = __cyc_rdtscp();
    total += t2 - t1;

    for (unsigned i = 0; i < length; i++) {
      __cs_facc(___enc_get(a, i));
      __cs_acc(___enc_get(a, i));
    }
  }

  __cyc_msg(total);
  __cs_fclose();
  __cs_msg();

  return 0;
}
