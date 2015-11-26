
void ___enc_multiply(long *, long *, long *, long) __attribute__((noinline));

void ___enc_multiply(long *a, long *mat, long *b, long size) {
  long i = 0;
  while (1) {
    if (!(i < size))
      break;

    long j = 0;
    while (1) {
      if (!(j < size))
        break;
      a[i] += mat[i*size + j]*b[j];
      ++j;
    }
    ++i;
  }
}

long ___enc_get(long *a, long i) {
  return a[i];
}

