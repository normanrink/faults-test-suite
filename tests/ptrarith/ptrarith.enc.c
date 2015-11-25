
void ___enc_copy(long *, long *, unsigned long) __attribute__((noinline));
long ___enc_get(long *, unsigned long);


void ___enc_copy(long *tgt, long *src, unsigned long size) {
  for (unsigned long i = 0; i < size; i++) {
      tgt[i] = src[i];
  }
}


long ___enc_get(long *a, unsigned long i) {
  return a[i];
}

