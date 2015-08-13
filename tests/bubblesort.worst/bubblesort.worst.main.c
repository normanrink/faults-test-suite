/* Adapted from Dmitry Kuvayskiy's test for his
   "AN-Transformer".
 */

#include <stdlib.h>

#include "encode.h"
#include "mycheck.h"
#include "mycyc.h"

extern long ___enc_bubblesort(long *array, long size);

int main(int argc, char** argv) {
    long *a;
    uint64_t t1, t2, total = 0;
    unsigned i, j;
    const unsigned length = LENGTH;
    const unsigned repetitions = REPETITIONS;

    fprintf(stderr, "LENGTH=%d\n", length);
    fprintf(stderr, "REPETITIONS=%d\n", repetitions);

    __cs_log(argc, argv);
    __cs_fopen(argc, argv);
    __cs_reset();

    a = (long*)malloc(length * sizeof(long));

    srand(0);
    for (i = 0; i < repetitions; i++) {
        for (j = 0; j < length; j++)
          a[j] = AN_ENCODE_VALUE(length - j);

	__cyc_warmup();
        t1 = __cyc_rdtsc();
        ___enc_bubblesort(a, length);
        t2 = __cyc_rdtscp();
        total += t2 - t1;

        for (j = 0; j < length; j++) {
          __cs_facc(AN_DECODE_VALUE(a[j]));
          __cs_acc(AN_DECODE_VALUE(a[j]));
        }
    }

    __cyc_msg(total);
    __cs_fclose();
    __cs_msg();

    return 0;
}
