#ifndef __ENCODE_H__
#define __ENCODE_H__

#include <stdint.h>

#ifdef ENCODE
  #define ___CHECK_RANGE(p, n) \
  { \
    for (int64_t i = 0; i < n; i++) \
      __builtin_an_assert_value_i64(p[i]); \
  }

  #define AN_ENCODE_VALUE(x) __builtin_an_encode_value_i64((x))
  #define AN_DECODE_VALUE(x) __builtin_an_decode_value_i64((x))
  #define AN_CHECK_RANGE(p, n) ___CHECK_RANGE((p), (n))
#else
  #define AN_ENCODE_VALUE(x) ((long long)x)
  #define AN_DECODE_VALUE(x) ((long long)x)
  #define AN_CHECK_RANGE(p, n)
#endif

#endif /* __ENCODE_H__ */
