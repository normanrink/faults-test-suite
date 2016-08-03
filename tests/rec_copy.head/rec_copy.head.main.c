
#include <stdio.h>

#include "mycheck.h"
#include "mycyc.h"


#define SIZE 32000

const unsigned length = (LENGTH < SIZE) ? LENGTH : SIZE;
const unsigned repetitions = REPETITIONS;

extern void ___enc_rec_copy(long *, long *, unsigned long);
extern long ___enc_get(long *, unsigned long);


long target[length];
extern long source[length];


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

  for (unsigned r = 0; r < repetitions; r++) {
#if (defined DEBUG) || (defined CYCLES)
    __cyc_warmup();
    t1 = __cyc_rdtsc();
#endif
    ___enc_rec_copy(&target[0], &source[0], length);
#if (defined DEBUG) || (defined CYCLES)
    t2 = __cyc_rdtscp();
    total += t2 - t1;
    total += __cyc_delta(r, t2, t1);
#endif

    for (unsigned i = 0; i < length; i++) {
      __cs_facc(___enc_get(target, i));
#if (defined DEBUG) || (defined CHECKSUM)
      __cs_acc(___enc_get(target, i));
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
