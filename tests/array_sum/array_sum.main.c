/* Adapted from Dmitry Kuvayskiy's test for his
   "AN-Transformer".
 */

#include <stdio.h>

#include "mycheck.h"
#include "mycyc.h"


#define SIZE 32000

const unsigned length = (LENGTH < SIZE) ? LENGTH : SIZE;
const unsigned repetitions = REPETITIONS;

extern long ___enc_sum(long *, long);


extern long a[length];


int main(int argc, char** argv) {
    uint64_t t1, t2, total = 0;
    unsigned i;

#ifdef DEBUG
    fprintf(stderr, "LENGTH=%d\n", length);
    fprintf(stderr, "REPETITIONS=%d\n", repetitions);
#endif

    __cs_fopen(argc, argv);
#if (defined DEBUG) || (defined CHECKSUM)
    __cs_log(argc, argv);
    __cs_reset();
#endif

    for (i = 0; i < repetitions; i++) {
      __cyc_warmup();
      t1 = __cyc_rdtsc();
      long sum = ___enc_sum(&a[0], length);
      t2 = __cyc_rdtscp();
      total += t2 - t1;

      __cs_facc(sum);
#if (defined DEBUG) || (defined CHECKSUM)
      __cs_acc(sum);
#endif
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
