#include <stdio.h>
#include <stdlib.h>

#include "mycheck.h"
#include "mycyc.h"


const unsigned length = (LENGTH < 1000) ? LENGTH : 1000;
const unsigned repetitions = REPETITIONS;

long result[length];

extern long vector[length];
extern long matrix[length*length];

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
    __cyc_warmup();
    t1 = __cyc_rdtsc();
    ___enc_multiply(&result[0], &matrix[0], &vector[0], length);
    t2 = __cyc_rdtscp();
    total += t2 - t1;

    for (unsigned i = 0; i < length; i++) {
      __cs_facc(___enc_get(&result[0], i));
      __cs_acc(___enc_get(&result[0], i));
    }
  }

  __cyc_msg(total);
  __cs_fclose();
  __cs_msg();

  return 0;
}
