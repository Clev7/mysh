#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>

// Passing in n lets the user know the buffer size
ssize_t readline(char **buffer, size_t *buf_size, FILE *stream) {
    if (buffer == NULL || buf_size == NULL || stream == NULL) {
        return -1;
    }

    // I guess you don't add 1 because it's already
    // a full 8 KB.
    *buffer = (char *) malloc(sizeof(char) * (*buf_size));

    // Up to the programmer what to do with this in
    if (*buffer == NULL) {
        return -1;
    }

    size_t len = 0;
    while (true) {
        // Casting makes this clearer
        char c = (char) fgetc(stream);
        if (c == EOF || c == '\n') break;

        len++;

        if (len > buf_size) {
            (*buf_size) *= 2;
            buffer = (char *) realloc(buffer, sizeof(char) * (*buf_size));

            if (buffer == NULL) {
                return -1;
            }
        }
        
    }

    buffer[len-1] = '\0';
    return buffer;
}

int main(void) {
    printf("# ");

    while (1) {

    }
}