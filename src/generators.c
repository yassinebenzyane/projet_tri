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

void generate_all_equal(int *array, size_t size, int value) {
    if (array == NULL || size == 0) {
        return;
    }

    for (size_t i = 0; i < size; ++i) {
        array[i] = value;
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

void generate_nearly_sorted(int *array, size_t size, double disorder_ratio, uint64_t seed) {
    if (array == NULL || size == 0) {
        return;
    }

    generate_sorted(array, size);

    uint64_t state = seed;
    size_t swaps = (size_t)(size * disorder_ratio);
    for (size_t i = 0; i < swaps; ++i) {
        size_t a = (size_t)(xorshift64(&state) % size);
        size_t b = (size_t)(xorshift64(&state) % size);
        int tmp = array[a];
        array[a] = array[b];
        array[b] = tmp;
    }
}

void generate_adversarial_quicksort(int *array, size_t size, uint64_t seed) {
    if (array == NULL || size == 0) {
        return;
    }

    (void)seed;

    for (size_t i = 0; i < size; ++i) {
        array[i] = (int)i;
    }

    for (size_t i = 1; i < size; ++i) {
        size_t j = size - i;
        int tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
    }
}
