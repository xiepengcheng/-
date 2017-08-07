
#ifndef _HASH_H_
#define _HASH_H_

/* KETAMA hastrh algorithm */
long KETAMA_HASH(const char *str, int nTime) {
    long rv = ((long) (str[3+nTime*4] & 0xff) << 24)
            | ((long) (str[2+nTime*4] & 0xff) << 16)
            | ((long) (str[1+nTime*4] & 0xff) << 8)
            | (str[0+nTime] & 0xff);
    return rv & 0xffffffffL;
}

#endif
