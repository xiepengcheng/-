

#include <stdio.h>
#include <string.h>
#include "md5.h"

//gcc -lssl -lcrypto

int main() {
    unsigned char *data = "127.0.0.1:10000-2";
    char buf[33] = {'\0'};
    MyMD5(data, buf);
    printf("%s\n", buf);
    return 0;
}
