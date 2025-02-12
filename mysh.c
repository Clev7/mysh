#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>

/*
    Commands to implement:
    1. movetodir <path>
    2. whereami
    3. history [-c]
    4. byebye
    5. replay <number>
    6. start <program> [parameters]
    7. background <program> [parameters]
    8. dalek <PID>

    Bonuses:
    9. repeat <n> <command> _
    10. dalekall
*/

typedef struct {
    char *buffer;
    size_t bytes_read;
} rl_result;

// Passing in n lets the user know the buffer size
// You reserve ssize_t for the return type, meaning
// the buffer would have to be passed in and returned

// buf_size is actually more like buffer capacity

// Just perror and return NULL for now. further
// error handling can be done later

rl_result *readline(FILE *stream, size_t *buf_size) {
    if (!stream) {
        perror("Null file stream\n");
        return NULL;
    }

    rl_result *res = malloc(sizeof(rl_result));

    if (!res) {
        perror("Malloc failed\n");
        return res;
    }

    const size_t DEFAULT_BUF_SIZE = 1024;

    if (!buf_size || *buf_size == 0) {
        *buf_size = DEFAULT_BUF_SIZE;
    }

    printf("buf_size: %ld\n", *buf_size);

    // I guess you don't add 1 because it's already
    // a full 8 KB.
    char *buffer = (char *) malloc(sizeof(char) * (*buf_size));

    // Up to the programmer what to do with this
    if (!buffer) {
        perror("Malloc failed\n");
        return NULL;
    }

    size_t bytes_read = 0;

    while (true) {
        // Casting makes this clearer
        char c = (char) fgetc(stream);

        if (c == EOF || c == '\n') break;

        buffer[bytes_read++] = c;

        // printf("bytes_read: %ld\n", bytes_read);
        // printf("buf_size: %ld\n", *buf_size);

        if (bytes_read > *buf_size) {
            (*buf_size) *= 2;
            buffer = (char *) realloc(buffer, sizeof(char) * (*buf_size));

            if (buffer == NULL) {
                perror("Realloc failed\n");
                return NULL;
            }
        }
    }

    buffer[bytes_read] = '\0';
    res->buffer = buffer;
    res->bytes_read = bytes_read;

    return res;
}

int main(void) {
    printf("# ");

    while (1) {
        size_t *buf_size = calloc(1, sizeof(size_t));
        rl_result *res = readline(stdin, buf_size);

        if (!res || !res->buffer) {
            printf("Null res pointer\n");
            continue;
        }

        char *curr_token = strtok(res->buffer, " ");

        char *command = curr_token;

        if (command == "movetodir") {

        } else if (command == "whereami") {

        } else if (command == "history") {

        } else if (command == "byebye") {

        } else if (command == "replay") {

        } else if (command == "start") {

        } else if (command == "background") {

        } else if (command == "dalek") {

        } else if (command == "repeat") {

        } else if (command == "dalekall") {

        }


        // printf("command: %s\n", res->buffer);
        // printf("bytes read: %ld\n", res->bytes_read);
        break;
    }
}