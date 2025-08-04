#include "steganography.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include <inttypes.h>

// Create inline function instances
void stega_set_lsb(unsigned char *a, unsigned char bit);
void stega_set_bit(unsigned char *byte, unsigned char pos, unsigned char bit);
int is_little_endian();


png_bytep stega_open(char const *file, png_imagep image) {
    memset(image, 0, sizeof(*image));
    image->version = PNG_IMAGE_VERSION;

    if (!png_image_begin_read_from_file(image, file)) {
        perror(file);
        exit(1);
    }
    png_bytep buffer = malloc(PNG_IMAGE_SIZE(*image));
    // Converts all images to rgba format with 1 byte per channel.
    image->format = PNG_FORMAT_RGBA;
    if (!buffer) {
        perror("memory allocation failed");
        exit(1);
    }
    if (!png_image_finish_read(image, 0, buffer, 0, NULL)) {
        perror("image finish read failed");
        exit(1);
    }

    return buffer;
}


void stega_write_msg(char const *msg, png_bytep buffer, size_t width, size_t height) {
    uint32_t const len = strlen(msg);
    uint count = 0;
    uint byte = 0;
    uint bit = 0;
    printf("len = %"PRIu32"\n", len);
    // it is assumed len cat fit into 4 bytes = 32 bits -> first 8 pixels.
    for (size_t i = 0; i < height && byte < len; i++) {
        for (size_t j = 0; j < width * bytes_per_pixel && byte < len; j++) {
            unsigned char *chan = &buffer[i * width * bytes_per_pixel + j];
            if (count < len_bits) {
                // write msg len;
                const unsigned char target = len >> (31 - count) & 0x01;
                stega_set_lsb(chan, target);
                count++;
            } else {
                unsigned char n_bit = msg[byte] >> (7 - bit) & 0x01;
                stega_set_lsb(chan, n_bit);
                bit++;
                byte += bit / 8;
                bit = bit % 8;
            }
        }
    }
}


void stega_close(png_bytep buffer, png_imagep image, char const *file) {
    if (!png_image_write_to_file(image, file, 0, buffer, 0, NULL)) {
        perror("Writing to file failed");
        exit(1);
    }
}

void stega_read_msg(char *msg, png_bytep buffer, size_t width, size_t height) {
    union {
        uint32_t u32;
        unsigned char bytes[4];
    } len;
    len.u32 = 0;
    uint count = 0;
    size_t i, j = 0;
    for (i = 0; i < height && count < len_bits; i++) {
        for (j = 0; j < width * bytes_per_pixel && count < len_bits; j++) {
            unsigned char chan = buffer[i * width * bytes_per_pixel + j];
            if (is_little_endian()) {
                stega_set_bit(&len.bytes[3 - count / 8], count % 8, chan & 0x01);
            } else {
                stega_set_bit(&len.bytes[count / 8], count % 8, chan & 0x01);
            }
            count++;
        }
    }
    printf("len = %"PRIu32"\n", len.u32);
    // In previous block outer cycle (i) makes 1 excess step when count == len_bits is reached.
    i--;
    count = 0;
    for (size_t k = i; k < height && count < len.u32 * 8; k++) {
        for (size_t l = j; l < width * bytes_per_pixel && count < len.u32 * 8; l++) {
            unsigned char chan = buffer[k * width * bytes_per_pixel + l];
            stega_set_bit((void*)&msg[count / 8], count % 8, chan & 0x01);
            count++;
        }
    }
}

