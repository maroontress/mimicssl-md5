#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "md5.h"

static void
printHash(const uint8_t *p)
{
    for (unsigned int i = 0; i < 16; ++i) {
        printf("%02x", p[i]);
    }
    printf("\n");
}

int
main(int ac, char **av)
{
    if (ac < 2) {
        fprintf(stderr, "usage: %s FILE\n", av[0]);
        exit(1);
    }
    FILE *file = fopen(av[1], "rb");
    if (file == NULL) {
        fprintf(stderr, "file not found\n");
        exit(1);
    }
    MD5_CTX ctx;
    uint8_t md[16];
    char buffer[1024];

    MD5_Init(&ctx);
    for (;;) {
        size_t size = fread(buffer, 1, sizeof(buffer), file);
        if (size == 0) {
            break;
        }
        MD5_Update(&ctx, buffer, size);
    }
    MD5_Final(md, &ctx);

    printHash(md);
    return 0;
}
