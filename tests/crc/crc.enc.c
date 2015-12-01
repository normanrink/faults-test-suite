

#define UPDC32(octet,crc) (crc_32_tab[((crc)^(octet)) & 0xff]^ ((crc) >> 8))

extern long crc_32_tab[256];

long crc, charcnt;

long crc32file(long *, long *, long *, long) __attribute__((noinline));


long crc32file(long *input, long *crc, long *charcnt, long maxcnt)
{
    register long oldcrc32;
    register long tmp;

    oldcrc32 = ~0;
    *charcnt = 0;

    while (*input != -1L && *charcnt < maxcnt)
    {
        ++*charcnt;
        tmp = oldcrc32;
        oldcrc32 = UPDC32(*input, tmp);
        ++input;
    }

    tmp = ~oldcrc32;
    oldcrc32 = tmp;
    *crc = oldcrc32;

    return 0;
}

long ___enc_computation(long *input, long maxcnt)
{
    long r = crc32file(input, &crc, &charcnt, maxcnt);

    return r;
}

long ___enc_get_crc() {
  return crc;
}

long ___enc_get_charcnt() {
  return charcnt;
}
