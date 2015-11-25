
long ___enc_sum(long *, long) __attribute__((noinline));


long ___enc_sum(long *a, long n) {
    long sum = 0;
    for (long i = 0; i < n; i++)
        sum += a[i];

    return sum;
}

