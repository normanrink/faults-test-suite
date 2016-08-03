/* Adapted from Dmitry Kuvayskiy's test for his
   "AN-Transformer".
 */

#include <stdio.h>
#include <string.h>

#include "mycheck.h"
#include "mycyc.h"


#define SIZE 32000

const unsigned length = (LENGTH < SIZE) ? LENGTH : SIZE;
const unsigned repetitions = REPETITIONS;

extern long ___enc_bubblesort(long *, long);
extern long ___enc_get(long *, long);


extern long a0[length];
extern long a1[length];


int main(int argc, char** argv) {
  uint64_t t1, t2, total = 0;

#ifdef DEBUG
  fprintf(stderr, "LENGTH=%d\n", length);
  fprintf(stderr, "REPETITIONS=%d\n", repetitions);
#endif

  __cs_fopen(argc, argv);
#if (defined DEBUG) || (defined CHECKSUM)
  __cs_log(argc, argv);
  __cs_reset();
#endif

  for (unsigned i = 0; i < repetitions; i++) {
    if (i > 0) {
      // Sorting takes place "in-situ". Therefore, after the first
      // repetition, the initial contents in 'a0' must be restored.
      // (The 'memcpy' is obviously never executed if there is only
      // one repetition. This is intended to speed up fault injection
      // runs.)
      memcpy(&a0[0], &a1[0], sizeof(long)*length);
    }
#if (defined DEBUG) || (defined CYCLES)
    __cyc_warmup();
    t1 = __cyc_rdtsc();
#endif
    ___enc_bubblesort(&a0[0], length);
#if (defined DEBUG) || (defined CYCLES)
    t2 = __cyc_rdtscp();
    total += __cyc_delta(i, t2, t1);
#endif

    for (unsigned j = 0; j < length; j++) {
      __cs_facc(___enc_get(a0, j));
#if (defined DEBUG) || (defined CHECKSUM)
      __cs_acc(___enc_get(a0, j));
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
