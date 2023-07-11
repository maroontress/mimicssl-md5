#if defined(__STDC_LIB_EXT1__) && (__STDC_LIB_EXT1__ >= 201112L)
#define __STDC_WANT_LIB_EXT1__ 1
#endif

#include <stdio.h>
#include <string.h>

#include "md5.h"

#include "libext1.h"

struct Unit {
    uint32_t data[16];
};

struct X {
    uint32_t k;
    uint32_t r;
};

static const struct MD5_Hash ABCD = {
    .data = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476}};

static const uint32_t K_ROUND_1[] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821};

static const struct X X_ROUND_2[] = {
    {0xf61e2562, 1}, {0xc040b340, 6}, {0x265e5a51, 11}, {0xe9b6c7aa, 0},
    {0xd62f105d, 5}, {0x2441453, 10}, {0xd8a1e681, 15}, {0xe7d3fbc8, 4},
    {0x21e1cde6, 9}, {0xc33707d6, 14}, {0xf4d50d87, 3}, {0x455a14ed, 8},
    {0xa9e3e905, 13}, {0xfcefa3f8, 2}, {0x676f02d9, 7}, {0x8d2a4c8a, 12}};
static const struct X X_ROUND_3[] = {
    {0xfffa3942, 5}, {0x8771f681, 8}, {0x6d9d6122, 11}, {0xfde5380c, 14},
    {0xa4beea44, 1}, {0x4bdecfa9, 4}, {0xf6bb4b60, 7}, {0xbebfbc70, 10},
    {0x289b7ec6, 13}, {0xeaa127fa, 0}, {0xd4ef3085, 3}, {0x4881d05, 6},
    {0xd9d4d039, 9}, {0xe6db99e5, 12}, {0x1fa27cf8, 15}, {0xc4ac5665, 2}};
static const struct X X_ROUND_4[] = {
    {0xf4292244, 0}, {0x432aff97, 7}, {0xab9423a7, 14}, {0xfc93a039, 5},
    {0x655b59c3, 12}, {0x8f0ccc92, 3}, {0xffeff47d, 10}, {0x85845dd1, 1},
    {0x6fa87e4f, 8}, {0xfe2ce6e0, 15}, {0xa3014314, 6}, {0x4e0811a1, 13},
    {0xf7537e82, 4}, {0xbd3af235, 11}, {0x2ad7d2bb, 2}, {0xeb86d391, 9}};

static const uint32_t S_ROUND_1[] = {7, 12, 17, 22};
static const uint32_t S_ROUND_2[] = {5, 9, 14, 20};
static const uint32_t S_ROUND_3[] = {4, 11, 16, 23};
static const uint32_t S_ROUND_4[] = {6, 10, 15, 21};

static uint32_t f(uint32_t x, uint32_t y, uint32_t z)
{
    return ((x & y) | (~x & z));
}

static uint32_t g(uint32_t x, uint32_t y, uint32_t z)
{
    return ((x & z) | (y & ~z));
}

static uint32_t h(uint32_t x, uint32_t y, uint32_t z)
{
    return (x ^ y ^ z);
}

static uint32_t i(uint32_t x, uint32_t y, uint32_t z)
{
    return (y ^ (x | ~z));
}

static uint32_t rotate(uint32_t x, uint32_t s)
{
    return (x << s) | (x >> (32 - s));
}

static void roundUnit(struct MD5_Hash *hash, const struct Unit *unit)
{
    const uint32_t *const m = unit->data;
    uint32_t *const hashData = hash->data;
    uint32_t a0 = hashData[0];
    uint32_t b0 = hashData[1];
    uint32_t c0 = hashData[2];
    uint32_t d0 = hashData[3];

    do {
        const uint32_t *const s = S_ROUND_1;
        const uint32_t *k = K_ROUND_1;
        const uint32_t *z = m;
        for (uint32_t j = 0; j < 4; ++j) {
            a0 = rotate(a0 + f(b0, c0, d0) + z[0] + k[0], s[0]) + b0;
            d0 = rotate(d0 + f(a0, b0, c0) + z[1] + k[1], s[1]) + a0;
            c0 = rotate(c0 + f(d0, a0, b0) + z[2] + k[2], s[2]) + d0;
            b0 = rotate(b0 + f(c0, d0, a0) + z[3] + k[3], s[3]) + c0;
            z += 4;
            k += 4;
        }
    } while (0);
    do {
        const uint32_t *const s = S_ROUND_2;
        const struct X *x = X_ROUND_2;
        for (uint32_t j = 0; j < 4; ++j) {
            a0 = rotate(a0 + g(b0, c0, d0) + m[x[0].r] + x[0].k, s[0]) + b0;
            d0 = rotate(d0 + g(a0, b0, c0) + m[x[1].r] + x[1].k, s[1]) + a0;
            c0 = rotate(c0 + g(d0, a0, b0) + m[x[2].r] + x[2].k, s[2]) + d0;
            b0 = rotate(b0 + g(c0, d0, a0) + m[x[3].r] + x[3].k, s[3]) + c0;
            x += 4;
        }
    } while (0);
    do {
        const uint32_t *const s = S_ROUND_3;
        const struct X *x = X_ROUND_3;
        for (uint32_t j = 0; j < 4; ++j) {
            a0 = rotate(a0 + h(b0, c0, d0) + m[x[0].r] + x[0].k, s[0]) + b0;
            d0 = rotate(d0 + h(a0, b0, c0) + m[x[1].r] + x[1].k, s[1]) + a0;
            c0 = rotate(c0 + h(d0, a0, b0) + m[x[2].r] + x[2].k, s[2]) + d0;
            b0 = rotate(b0 + h(c0, d0, a0) + m[x[3].r] + x[3].k, s[3]) + c0;
            x += 4;
        }
    } while (0);
    do {
        const uint32_t *const s = S_ROUND_4;
        const struct X *x = X_ROUND_4;
        for (uint32_t j = 0; j < 4; ++j) {
            a0 = rotate(a0 + i(b0, c0, d0) + m[x[0].r] + x[0].k, s[0]) + b0;
            d0 = rotate(d0 + i(a0, b0, c0) + m[x[1].r] + x[1].k, s[1]) + a0;
            c0 = rotate(c0 + i(d0, a0, b0) + m[x[2].r] + x[2].k, s[2]) + d0;
            b0 = rotate(b0 + i(c0, d0, a0) + m[x[3].r] + x[3].k, s[3]) + c0;
            x += 4;
        }
    } while (0);
    hashData[0] += a0;
    hashData[1] += b0;
    hashData[2] += c0;
    hashData[3] += d0;
}

static uint32_t toLE32(const uint8_t *m)
{
    return (uint32_t)m[0] | (uint32_t)m[1] << 8 | (uint32_t)m[2] << 16
        | (uint32_t)m[3] << 24;
}

int MD5_Init(MD5_CTX *c)
{
    c->size = 0;
    c->hash = ABCD;
    return 1;
}

int MD5_Update(MD5_CTX *c, const void *data, size_t len)
{
    struct Unit unit;
    uint32_t *unitData = unit.data;
    const uint8_t *source = data;

    size_t remaining = c->size % 64;
    if (remaining > 0) {
        size_t size = 64 - remaining;
        if (len < size) {
            MEMCPY(c->input + remaining, data, len);
            c->size += len;
            return 0;
        }
        MEMCPY(c->input + remaining, data, size);
        const uint8_t *m = c->input;
        for (uint32_t j = 0; j < 16; ++j) {
            unitData[j] = toLE32(m);
            m += 4;
        }
        roundUnit(&c->hash, &unit);
        c->size += size;
        len -= size;
        source += size;
    }

    while (len >= 64) {
        for (uint32_t j = 0; j < 16; ++j) {
            unitData[j] = toLE32(source);
            source += 4;
        }
        roundUnit(&c->hash, &unit);
        c->size += 64;
        len -= 64;
    }
    if (len > 0) {
        MEMCPY(c->input, source, len);
        c->size += len;
    }
    return 1;
}

int MD5_Final(unsigned char *md, MD5_CTX *c)
{
    uint64_t finalSize = c->size;
    do {
        uint8_t padding[64];
        uint32_t remainder = finalSize % 64;
        uint32_t length = (remainder < 56)
            ? 56 - remainder
            : (56 + 64) - remainder;
        padding[0] = 0x80;
        for (uint32_t k = 1; k < length; ++k) {
            padding[k] = 0;
        }
        MD5_Update(c, padding, length);
    } while (0);
    do {
        uint32_t lowerSize = (uint32_t)(finalSize << 3);
        uint32_t upperSize = (uint32_t)(finalSize >> 29);
        const uint32_t sizeArray[] = {lowerSize, upperSize};
        uint8_t sizeData[8];
        uint8_t *d = sizeData;
        for (uint32_t k = 0; k < 2; ++k) {
            uint32_t size = sizeArray[k];
            d[0] = (uint8_t)size;
            d[1] = (uint8_t)(size >> 8);
            d[2] = (uint8_t)(size >> 16);
            d[3] = (uint8_t)(size >> 24);
            d += 4;
        }
        MD5_Update(c, sizeData, sizeof(sizeData));
    } while (0);

    const uint32_t *hashData = c->hash.data;
    for (uint32_t k = 0; k < 4; ++k) {
        uint32_t h = hashData[k];
        md[0] = (uint8_t)h;
        md[1] = (uint8_t)(h >> 8);
        md[2] = (uint8_t)(h >> 16);
        md[3] = (uint8_t)(h >> 24);
        md += 4;
    }
    return 1;
}
