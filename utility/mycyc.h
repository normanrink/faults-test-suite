#ifndef __MYCYC_H__
#define __MYCYC_H__

#include <stdint.h>

void __cyc_warmup() __attribute__((always_inline));
void __cyc_msg(uint64_t cs) __attribute__((always_inline));

uint64_t __cyc_delta(unsigned iteration, uint64_t t2, uint64_t t1) __attribute__((always_inline));

// Use 'rdtsc' for initial reading of the timestamp counter.
// (cf. Intel white paper 324264-001 "How to Benchmark Code Execution
// Times on Intel® IA-32 and IA-64 Instruction Set Architectures")
inline uint64_t __cyc_rdtsc() {
  uint32_t low, high;
  // NOTE: The 'xorl' instruction is not actually needed. The 'cpuid'
  // instruction acts as a barrier in the instruction stream. All that
  // is achieved by the 'xorl' instruction is that 'cpuid' operates on
  // well-defined inputs (i.e. the 'eax' register).
  __asm__ __volatile__ ("xorl %%eax, %%eax\n"
                        "cpuid\n"
                        "rdtsc\n" : "=a" (low), "=d" (high)
                                  : // no inputs
                                  : "%rax", "%rbx", "%rcx", "%rdx");
  return (((uint64_t)high) << 32) | (uint64_t)low;
}

// Use 'rdtscp' for final reading of the timestamp counter.
// (cf. Intel white paper 324264-001 "How to Benchmark Code Execution
// Times on Intel® IA-32 and IA-64 Instruction Set Architectures")
inline uint64_t __cyc_rdtscp() {
  uint32_t low, high;
  __asm__ __volatile__ ("rdtscp\n"
                        "mov %%eax, %0\n"
                        "mov %%edx, %1\n"
  // NOTE: The 'xorl' instruction is not actually needed. The 'cpuid'
  // instruction acts as a barrier in the instruction stream. All that
  // is achieved by the 'xorl' instruction is that 'cpuid' operates on
  // well-defined inputs (i.e. the 'eax' register).
                        "xorl %%eax, %%eax\n"
                        "cpuid\n" : "=r" (low), "=r" (high)
                                  : // no inputs
                                  : "%rax", "%rbx", "%rcx", "%rdx");
  return (((uint64_t)high) << 32) | (uint64_t)low;
}

#endif /* __MYCYC_H__ */
