#ifndef md5_H
#define md5_H

#include <stddef.h>
#include <stdint.h>

#include "md5_export.h"

#define MD5_LONG uint32_t
#define MD5_CBLOCK 64
#define MD5_LBLOCK (MD5_CBLOCK / 4)
#define MD5_DIGEST_LENGTH 16

struct MD5_Hash {
    uint32_t data[4];
};

typedef struct {
    struct MD5_Hash hash;
    uint8_t input[MD5_CBLOCK];
    uint64_t size;
    uint8_t unused[4];
} MD5_CTX;

#if defined(__cplusplus)
extern "C" {
#endif

int MD5_EXPORT MD5_Init(MD5_CTX *c);
int MD5_EXPORT MD5_Update(MD5_CTX *c, const void *data, size_t len);
int MD5_EXPORT MD5_Final(unsigned char *md, MD5_CTX *c);

#if defined(__cplusplus)
}
#endif

#endif
