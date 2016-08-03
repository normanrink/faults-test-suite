
#include <stdint.h>
#include <stdio.h>

#include "mycyc.h"

// Use '__cyc_warmup' to "warm up" the instruction cache (as is done
// in the Intel white paper 324264-001 "How to Benchmark Code Execution
// Times on Intel® IA-32 and IA-64 Instruction Set Architectures").
void __cyc_warmup() {
  __cyc_rdtsc();
  __cyc_rdtscp();
  __cyc_rdtsc();
  __cyc_rdtscp();
  __cyc_rdtsc();
  __cyc_rdtscp();
}

void __cyc_msg(uint64_t cs) {
  fprintf(stderr, "elapsed cycles: %lu\n", cs);
}

uint64_t __cyc_delta(unsigned iteration, uint64_t t2, uint64_t t1) {
#if (defined NUMBER_WARMUP)
  return (iteration < (NUMBER_WARMUP)) ? 0 : t2 - t1;
#else
  return t2 - t1;
#endif
}

