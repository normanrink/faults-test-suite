/* Adapted from Dmitry Kuvayskiy's test for his
   "AN-Transformer".
 */

#include <stdio.h>

#include "mycheck.h"
#include "mycyc.h"


const unsigned length = FIBONACCI_LENGTH;
const unsigned repetitions = REPETITIONS;

extern unsigned long ___enc_fib(unsigned long);


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
#if (defined DEBUG) || (defined CYCLES)
      __cyc_warmup();
      t1 = __cyc_rdtsc();
#endif
      unsigned long f = ___enc_fib(length);
#if (defined DEBUG) || (defined CYCLES)
      t2 = __cyc_rdtscp();
      total += __cyc_delta(i, t2, t1);
#endif

      __cs_facc(f);
#if (defined DEBUG) || (defined CHECKSUM)
      __cs_acc(f);
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
