#ifndef UTILS_H
#define UTILS_H
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

inline
int is_little_endian() {
    uint16_t const b = 0xAABB;
    uint8_t *p = (uint8_t*)&b;
    return p[0] == 0xBB;
}

inline
void stega_set_lsb(unsigned char *a, unsigned char bit) {
    if (bit) {
        *a = *a | 0x01;
    } else {
        *a = *a & (0xFF - 1);
    }
}

inline
void stega_set_bit(unsigned char *byte, unsigned char pos, unsigned char bit) {
    unsigned char mask = (1 << (7 - pos));
    if (bit) {
        *byte = *byte | mask;
    } else {
        *byte = *byte & ~mask;
    }
}

#endif //UTILS_H
