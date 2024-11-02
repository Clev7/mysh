#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

char *read_input(FILE *stream) {
    size_t buf_size = 8192;

    // I guess you don't add 1 because it's already
    // a full 8 KB.
    char *buffer = (char *) malloc(sizeof(char) * buf_size);

    if (buffer == NULL) {
        perror("Could not allocate memory to buffer\n");
    }

    char c = fgetc(stream);
    int len = 1;

    bool done = false;

    while (!done) {
        while (c != EOF && len <= buf_size) {
            buffer[len++ - 1] = c;

            c = fgetc(stream);
        }

        
    }


    while (fgets(buffer, buf_size, stdin) != NULL) {
        if (len < buf_size) {
            buffer[len] = '\0';
            return buffer;
        }

        if (len == buf_size) {
            if (buffer[buf_size - 1] == '\n') {
                buffer[buf_size - 1] = '\0';
                return buffer;
            }

            buf_size *= 2;
            buffer = (char *) realloc(buffer, sizeof(char) * buf_size);
        }
    }
}

int main(void) {
    printf("# ");

    while (1) {

    }
}