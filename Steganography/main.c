#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "steganography.h"

int main(int argc, char const *argv[]) {
    if (argc < 3) {
        printf("Usage: <mode> <filename> (<message>)\n");
        return 1;
    }
    png_image image;
    png_bytep buffer = stega_open(argv[2], &image);
    if (strcmp(argv[1], "w") == 0) {
        char msg[msg_buf_size] = {0};
        size_t total_read = 0;
        size_t cur_read = 0;
        while (cur_read = read(STDIN_FILENO, msg + total_read, msg_buf_size - total_read)) {
            total_read += cur_read;
        }
        if (total_read == 0) {
            printf("No message received\n");
            return 0;
        }
        printf("writing...\n");
        size_t max_msg_len = image.width * image.height / 2;
        if (max_msg_len <= strlen(msg) + 4) {
            fprintf(stderr, "Message too long: max len = %lu\n", max_msg_len);
            return 1;
        }
        stega_write_msg(msg, buffer, image.width, image.height);
        stega_close(buffer, &image, "output.png");
    } else if (strcmp(argv[1], "r") == 0) {
        char msg[msg_buf_size] = {0};
        stega_read_msg(msg, buffer, image.width, image.height);
        printf("%s\n", msg);
    }
    return 0;
}