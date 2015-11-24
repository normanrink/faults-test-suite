#ifndef __MYCYC_H__
#define __MYCYC_H__

#include <stdint.h>

uint64_t __cyc_rdtsc() __attribute__((always_inline));
uint64_t __cyc_rdtscp() __attribute__((always_inline));
void __cyc_warmup() __attribute__((always_inline));
void __cyc_msg(uint64_t cs) __attribute__((always_inline));

#endif /* __MYCYC_H__ */
