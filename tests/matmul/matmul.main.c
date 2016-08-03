
#include <stdio.h>

#include "mycheck.h"
#include "mycyc.h"


#define SIZE 1000

const unsigned length = (MATMUL_LENGTH < SIZE) ? MATMUL_LENGTH : SIZE;
const unsigned repetitions = REPETITIONS;

extern long vector[length];
extern long matrix[length*length];

extern void ___enc_multiply(long *, long *, long *, long);
extern long ___enc_get(long *, long);


long result[length];


int main(int argc, char **argv) {
  unsigned long t1, t2, total = 0;

#ifdef DEBUG
  fprintf(stderr, "LENGTH=%d\n", length);
  fprintf(stderr, "REPETITIONS=%d\n", repetitions);
#endif

  __cs_fopen(argc, argv);
#if (defined DEBUG) || (defined CHECKSUM)
  __cs_log(argc, argv);
  __cs_reset();
#endif

  for (unsigned k = 0; k < repetitions; k++) {
#if (defined DEBUG) || (defined CYCLES)
    __cyc_warmup();
    t1 = __cyc_rdtsc();
#endif
    ___enc_multiply(&result[0], &matrix[0], &vector[0], length);
#if (defined DEBUG) || (defined CYCLES)
    t2 = __cyc_rdtscp();
    total += __cyc_delta(k, t2, t1);
#endif

    for (unsigned i = 0; i < length; i++) {
      __cs_facc(___enc_get(&result[0], i));
#if (defined DEBUG) || (defined CHECKSUM)
      __cs_acc(___enc_get(&result[0], i));
#endif
    }
  }

  __cs_fclose();
#if (defined DEBUG) || (defined CYCLES)
  __cyc_msg(total);
#endif
#if (defined DEBUG) || (defined CHECKSUM)
  __cs_msg();
#endif

  return 0;
}
