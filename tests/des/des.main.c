/*
 *  S-DES file encryption program
 *
 * Copyright (c) 2009, AlferSoft (www.alfersoft.com.ar - fvicente@gmail.com)
 * All rights reserved.
 * 
 * BSD License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY AlferSoft ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL AlferSoft BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "mycheck.h"
#include "mycyc.h"

#include <stdio.h>


extern long input[];

extern void ___enc_generate_sub_keys(long, long *, long *);
extern long ___enc_des(long, const long *, const long *);


void display_bits(long num, long bits) {
  long i;

  if (bits < 1 || bits > 32) {
    printf("Invalid bit quantity");
    return;
  }
  /* display binary representation */
  for (i = 1L << (bits-1); i > 0L; i >>= 1L) {
    printf("%s",(i & num) ? "1" : "0");
  }
  printf("\n");
}


/**
 * Main program entry point
 */
int main(int argc, char* argv[]) {
  uint64_t t1, t2, total = 0;
  long sk1 = 0, sk2 = 0;

  const long key = 809;
  // encrypt:
  ___enc_generate_sub_keys(key, &sk1, &sk2);
  // decrypt:
  //___enc_generate_sub_keys(key, &sk2, &sk1);

  __cs_fopen(argc, argv);
#if (defined DEBUG) || (defined CHECKSUM)
  __cs_log(argc, argv);
  __cs_reset();
#endif

  unsigned i = 0;
  while (input[i] != EOF) {
#if (defined DEBUG) || (defined CYCLES)
    __cyc_warmup();
    t1 = __cyc_rdtsc();
#endif
    long ch = ___enc_des(input[i], &sk1, &sk2);
#if (defined DEBUG) || (defined CYCLES)
    t2 = __cyc_rdtsc();
    total += t2 - t1;
#endif

    __cs_facc(ch);
#if (defined DEBUG) || (defined CHECKSUM)
    __cs_acc(ch);
#endif

    ++i;
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
