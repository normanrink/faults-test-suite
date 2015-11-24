/* Adapted from Dmitry Kuvayskiy's test for his
   "AN-Transformer".
 */
#include <stdlib.h>

#include "myargs.h"
#include "mycheck.h"
#include "mycyc.h"


extern long ___enc_init_and_sum(long*, long, long);

const unsigned length = LENGTH;
const unsigned repetitions = REPETITIONS;

long a[length];


int main(int argc, char** argv) {
    uint64_t t1, t2, total = 0;
    unsigned i;

    fprintf(stderr, "LENGTH=%d\n", length);
    fprintf(stderr, "REPETITIONS=%d\n", repetitions);

    __cs_log(argc, argv);
    __cs_fopen(argc, argv);
    __cs_reset();

    for (i = 0; i < repetitions; i++) {
      long sum;

      __cyc_warmup();
      t1 = __cyc_rdtsc();
      sum = ___enc_init_and_sum(&a[0], length, 42);
      t2 = __cyc_rdtscp();
      total += t2 - t1;

      __cs_facc(sum);
      __cs_acc(sum);
    }

    __cyc_msg(total);
    __cs_fclose();
    __cs_msg();

    return 0;
}
