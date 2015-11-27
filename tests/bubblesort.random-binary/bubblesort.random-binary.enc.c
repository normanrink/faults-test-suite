
long ___enc_bubblesort(long *, long) __attribute__((noinline));

long ___enc_bubblesort(long *array, long size) {
  long swapped;

  for (long i = 1; i < size; i++) {
    swapped = 0;    //this flag is to check if the array is already sorted

    for (long j = 0; j < size - i; j++) {
      if (array[j] > array[j+1]) {
        long temp = array[j];
        array[j] = array[j+1];
        array[j+1] = temp;
        swapped = 1;
      }
    }
    if (!swapped) {
      break; //if it is sorted then stop
    }
  }
  return 0;
}


long ___enc_get(long *a, long i) {
  return a[i];
}
