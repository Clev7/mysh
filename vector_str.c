#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector_str.h"

vector_str *init_vect_str(void) {
	vector_str *v = (vector_str *) malloc(sizeof(vector_str));

	if (!v) {
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}

	v->capacity = INITIAL_CAPACITY;
	v->size = 0;

	v->vector = (char **) malloc(sizeof(char *) * v->capacity);
	
	if (!v->vector) {
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}

	return v;
}

vector_str *destroy_vect_str(vector_str *v) {
	if (!v) {
		fprintf(stderr, "vector_str is NULL\n");
		return v;
	}

	for (size_t i = 0; i < v->size; i++) {
		free(v->vector[i]);
	}

	free(v->vector);
	free(v);

	v = NULL;

	return v;
}

void vect_str_clear(vector_str *v) {
	if (!v) {
		fprintf(stderr, "vector_str is NULL");
		return;
	}

	for (size_t i = 0; i < v->size; i++) {
		// Clear the string
		memset(v->vector[i], 0, strlen(v->vector[i]));
	}

	v->size = 0;
}

void vect_str_push(vector_str *v, const char *str) {
	if (!v) {
		fprintf(stderr, "vector_str is NULL");
		return;
	}

	if (!str) {
		fprintf(stderr, "str is NULL");
		return;
	}

	// Empty strings should be ok to push
	if (v->size == v->capacity) {
		v->capacity = v->capacity * 2 + 1;

		v->vector = realloc(v->vector, v->capacity);

		if (!v->vector) {
			fprintf(stderr, "realloc failed");
			return;
		}
	}

	size_t len = strlen(str);
	v->vector[v->size] = (char *) malloc(sizeof(char) * len);
	strcpy(v->vector[v->size++], str);
}