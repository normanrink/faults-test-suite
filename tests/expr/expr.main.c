
#include <stdio.h>
#include <stdint.h>

#include "mycheck.h"
#include "mycyc.h"


const unsigned repetitions = REPETITIONS;


extern uint64_t input[];

extern int64_t ___enc_repl(uint64_t*);


int main(int argc, char** argv) {
    uint64_t t1, t2, total = 0;
    unsigned i;

#ifdef DEBUG
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
      int64_t res = ___enc_repl(&input[0]);
#if (defined DEBUG) || (defined CYCLES)
      t2 = __cyc_rdtscp();
      total += __cyc_delta(i, t2, t1);
#endif

      __cs_facc(res);
#if (defined DEBUG) || (defined CHECKSUM)
      __cs_acc(res);
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
