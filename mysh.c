#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#include "vector_str.h"

// TODO: Fix memory leaks
// TODO: Write unit testing

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

// Just fprintf to stderr and return NULL for now. further
// error handling can be done later

rl_result *readline(FILE *stream, size_t *buf_size) {
    if (!stream) {
        fprintf(stderr, "Null file stream\n");
        return NULL;
    }

    rl_result *res = (rl_result *) malloc(sizeof(rl_result));

    if (!res) {
        fprintf(stderr, "Malloc failed\n");
        return res;
    }

    const size_t DEFAULT_BUF_SIZE = 1024;

    if (!buf_size || *buf_size == 0) {
        *buf_size = DEFAULT_BUF_SIZE;
    }

    // printf("buf_size: %ld\n", *buf_size);

    // I guess you don't add 1 because it's already
    // a full 8 KB.
    char *buffer = (char *) malloc(sizeof(char) * (*buf_size));

    // Up to the programmer what to do with this
    if (!buffer) {
        fprintf(stderr, "Malloc failed\n");
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
                fprintf(stderr, "Realloc failed\n");
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
    char *cwd = getcwd(NULL, 0);

    vector_str *v = init_vect_str();

    while (true) {
        printf("# ");

        size_t *buf_size = (size_t *) calloc(1, sizeof(size_t));
        rl_result *res = readline(stdin, buf_size);
        vect_str_push(v, res->buffer);

        if (!res || !res->buffer) {
            printf("Null res pointer\n");
            continue;
        }

        const char *command = strtok(res->buffer, " ");
        char *args = strtok(NULL, " ");

        if (!strcmp(command, "movetodir")) {
            DIR *dir = opendir(args);

            // file exists
            if (dir) {
                cwd = args;
                closedir(dir);
            } else if (errno == ENOENT) {
                printf("Not a valid folder path\n");
            } else {
                printf("movetodir failed for some unknown reason\n");
            }
        } else if (!strcmp(command, "whereami")) {
            if (args != NULL) {
                printf("whereami: invalid usage\n");
                printf("usage: whereami\n");
                continue;
            }

            printf("%s\n", cwd);
        } else if (!strcmp(command, "history")) {
            if (args == NULL) {
                for (size_t i = 0; i < v->size; i++) {
                    printf("%ld %s\n", i + 1, v->vector[i]);
                }
            } else if (!strcmp(args, "-c")) {
                vect_str_clear(v);
            } else {
                printf("history: invalid option(s)\n");
                printf("history: usage: history [-c]\n");
            }

        } else if (!strcmp(command, "byebye")) {
            printf("exiting...\n");
            exit(0);
        } else if (!strcmp(command, "replay")) {

        } else if (!strcmp(command, "start")) {

        } else if (!strcmp(command, "background")) {
            
        } else if (!strcmp(command, "dalek")) {

        } else if (!strcmp(command, "repeat")) {

        } else if (!strcmp(command, "dalekall")) {

        } else {
            printf("%s: command not found\n", command);
        }

    }
}