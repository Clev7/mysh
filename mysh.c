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
    1. movetodir <path> (nargs: 1)
    2. whereami (nargs: 0)
    3. history [-c] (nargs: 0, 1)
    4. byebye (nargs: 0)
    5. replay <number> (nargs: 1)
    6. start <program> [parameters] (nargs: 1+)
    7. background <program> [parameters] (nargs: 1+)
    8. dalek <PID> (nargs: 1)

    Bonuses:
    9. repeat <n> <command> _ (nargs: 2+)
    10. dalekall (nargs: 0)
*/

typedef struct rl_result {
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

typedef struct parse_command_result {
    vector_str *argv;
    char *command;
} parse_command_result;

parse_command_result *parse_command(char *buffer) {
    parse_command_result *res = (parse_command_result *) malloc(sizeof(parse_command_result));
    if (!res) {
        fprintf(stderr, "malloc failed\n");
        return NULL;
    }

    res->argv = init_vect_str();

    if (!res->argv) {
        fprintf(stderr, "malloc failed\n");
        return NULL;
    }

    res->command = strtok(buffer, " ");

    if (!res->command) {
        return res;
    }

    vect_str_push(res->argv, res->command);

    while (true) {
        char *token = strtok(NULL, " ");
        if (!token) {
            break;
        }

        vect_str_push(res->argv, token);
    }

    return res;
}

void move_to_dir(vector_str *argv, char *cwd) {
    // Don't forget the command itself
    if (argv->size != 2) {
        fprintf(stderr, "Usage: movetodir <filepath>\n");
        return;
    }

    DIR *dir = opendir(argv->vector[1]);

    if (dir) {
        strcpy(cwd, argv->vector[1]);
    } else if (errno == ENOENT) {
        fprintf(stderr, "movetodir: %s\n", strerror(errno));
    } else {
        fprintf(stderr, "movetodir: an unknown error occurred\n");
        // fprintf(stderr, "movetodir: %s\n", strerror(errno));
    }
}


void whereami(vector_str *argv, const char *cwd) {
    if (argv->size != 1) {
        fprintf(stderr, "Usage: whereami");
        return;
    }

    printf("%s\n", cwd);
}

void history_command(vector_str *argv, vector_str *history) {
    if (argv->size > 2) {
        fprintf(stderr, "Usage: history [-c]\n");
    } else if (argv->size == 2) {
        if (!strcmp(argv->vector[1], "-c")) {
            vect_str_clear(history);
        } else {
            fprintf(stderr, "Usage: history [-c]");
        }
    } else {
        for (size_t i = 0; i < history->size; i++) {
            printf("%ld %s\n", i + 1, history->vector[i]);
        }
    }
}

int main(void) {
    char *cwd = getcwd(NULL, 0);

    vector_str *history = init_vect_str();

    while (true) {
        printf("# ");

        size_t *buf_size = (size_t *) calloc(1, sizeof(size_t));
        rl_result *res = readline(stdin, buf_size);
        vect_str_push(history, res->buffer);

        if (!res || !res->buffer) {
            printf("Null res pointer\n");
            continue;
        }

        parse_command_result *pcr = parse_command(res->buffer);
        const char *command = pcr->command;
        vector_str *argv = pcr->argv;

        if (!strcmp(command, "movetodir")) {
            move_to_dir(argv, cwd);
        } else if (!strcmp(command, "whereami")) {
            whereami(argv, cwd);
        } else if (!strcmp(command, "history")) {
            history_command(argv, history);
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

        // Clean up after yourself
        vect_str_clear(argv);
    }
}