#include "generators.h"

#include <stdlib.h>

static uint64_t xorshift64(uint64_t *state) {
    uint64_t x = *state;
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    *state = x;
    return x * 2685821657736338717ULL;
}

void generate_random(int *array, size_t size, uint64_t seed) {
    if (array == NULL || size == 0) {
        return;
    }

    uint64_t state = seed;
    for (size_t i = 0; i < size; ++i) {
        array[i] = (int)(xorshift64(&state) % 1000000ULL);
    }
}

void generate_sorted(int *array, size_t size) {
    if (array == NULL || size == 0) {
        return;
    }

    for (size_t i = 0; i < size; ++i) {
        array[i] = (int)i;
    }
}

void generate_reverse(int *array, size_t size) {
    if (array == NULL || size == 0) {
        return;
    }

    for (size_t i = 0; i < size; ++i) {
        array[i] = (int)(size - i);
    }
}


void generate_duplicates(int *array, size_t size, int max_value) {
    if (array == NULL || size == 0) {
        return;
    }

    for (size_t i = 0; i < size; ++i) {
        array[i] = (int)(i % (max_value > 0 ? max_value : 1));
    }
}


