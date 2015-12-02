
void rec_copy(long *, long *, unsigned long) __attribute__((noinline));
// no tail recursion (hence this test case carries the suffix 'head'):
void rec_copy(long *tgt, long *src, unsigned long size) {
  if (size == 0) 
    return;
  
  rec_copy(tgt, src, size-1);
  tgt[size-1] = src[size-1];
}


void ___enc_rec_copy(long *tgt, long *src, unsigned long size) {
  rec_copy(tgt, src, size);
}


long ___enc_get(long *a, unsigned long i) {
  return a[i];
}

