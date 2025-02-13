#pragma once

#include <stdlib.h>

#define INITIAL_CAPACITY 10

typedef struct vector_str {
    size_t size, capacity;
    char **vector;
} vector_str;

vector_str *init_vect_str();
vector_str *destroy_vect_str(vector_str *v);
void vect_str_clear(vector_str *v);
void vect_str_push(vector_str *v, const char *str);
