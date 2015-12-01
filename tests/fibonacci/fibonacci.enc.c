
unsigned long fib(unsigned long) __attribute__((noinline));

unsigned long fib(unsigned long n) {
    if (n == 0 || n == 1)
      return 1;
    else
      return fib(n-1) + fib(n-2);
}


unsigned long ___enc_fib(unsigned long n) {
    return fib(n);
}

