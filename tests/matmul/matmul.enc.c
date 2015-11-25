
void ___enc_multiply(long *, long *, long *, long) __attribute__((noinline));

void ___enc_multiply(long *a, long *mat, long *b, long size) {
  for (long i = 0; i < size; i++) {
    for (long j = 0; j < size; j++) {
      a[i] += mat[i*size + j]*b[j];
    }
  }
}

long ___enc_get(long *a, long i) {
  return a[i];
}

