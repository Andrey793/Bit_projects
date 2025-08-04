#ifndef STEGANOGRAPHY_H
#define STEGANOGRAPHY_H
#include <png.h>
#include <sys/types.h>

static const uint bytes_per_pixel = 4;
static const uint len_bits = 32;
#define msg_buf_size  1024 * 128

png_bytep stega_open(char const *file, png_imagep image);

void stega_write_msg(char const *msg, png_bytep buffer, size_t width, size_t height);

void stega_close(png_bytep buffer, png_imagep image, char const *file);

void stega_read_msg(char *msg, png_bytep buffer, size_t width, size_t height);

#endif //STEGANOGRAPHY_H
