#include "sort.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int is_sorted(const int *array, size_t size) {
    for (size_t i = 1; i < size; ++i) {
        if (array[i - 1] > array[i]) {
            return 0;
        }
    }
    return 1;
}

static int is_permutation(const int *original, const int *sorted, size_t size) {
    int *copy = malloc(size * sizeof(int));
    if (copy == NULL) {
        return 0;
    }

    memcpy(copy, sorted, size * sizeof(int));
    for (size_t i = 0; i < size; ++i) {
        int found = 0;
        for (size_t j = 0; j < size; ++j) {
            if (original[i] == copy[j]) {
                copy[j] = copy[size - 1];
                found = 1;
                break;
            }
        }
        if (!found) {
            free(copy);
            return 0;
        }
    }

    free(copy);
    return 1;
}

static void fill_random(int *array, size_t size, unsigned seed) {
    srand(seed);
    for (size_t i = 0; i < size; ++i) {
        array[i] = rand() % 1000;
    }
}

static void fill_sorted(int *array, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        array[i] = (int)i;
    }
}

static void fill_reverse(int *array, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        array[i] = (int)(size - i);
    }
}

static void fill_duplicates(int *array, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        array[i] = (int)(i % 10);
    }
}

static int run_sort_case(void (*sort_func)(int *, size_t, sort_stats_t *),
                        const char *algorithm_name,
                        int *input,
                        const int *original,
                        size_t size) {
    int *copy = malloc(size * sizeof(int));
    if (copy == NULL) {
        return 1;
    }

    memcpy(copy, input, size * sizeof(int));
    sort_stats_t stats;
    reset_stats(&stats);
    sort_func(copy, size, &stats);

    if (!is_sorted(copy, size) || !is_permutation(original, copy, size)) {
        printf("[ECHEC] %s pour taille %zu\n", algorithm_name, size);
        free(copy);
        return 1;
    }

    printf("[OK] %s | n=%zu | comparaisons=%zu | echanges=%zu\n",
           algorithm_name,
           size,
           stats.comparisons,
           stats.exchanges);

    free(copy);
    return 0;
}

int main(void) {
    int cases[] = {5, 10, 25};
    int original[25];
    int data[25];
    int failed = 0;

    for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
        size_t size = (size_t)cases[i];

        fill_random(original, size, 42u + (unsigned)size);
        memcpy(data, original, size * sizeof(int));
        failed |= run_sort_case(bubble_sort, "bubble_sort", data, original, size);

        memcpy(data, original, size * sizeof(int));
        failed |= run_sort_case(selection_sort, "selection_sort", data, original, size);

        memcpy(data, original, size * sizeof(int));
        failed |= run_sort_case(insertion_sort, "insertion_sort", data, original, size);

        memcpy(data, original, size * sizeof(int));
        failed |= run_sort_case(shell_sort, "shell_sort", data, original, size);

        memcpy(data, original, size * sizeof(int));
        failed |= run_sort_case(merge_sort, "merge_sort", data, original, size);

        memcpy(data, original, size * sizeof(int));
        failed |= run_sort_case(quick_sort, "quick_sort", data, original, size);

        memcpy(data, original, size * sizeof(int));
        failed |= run_sort_case(heap_sort, "heap_sort", data, original, size);
    }

    int data_duplicates[20];
    fill_duplicates(data_duplicates, 20);
    failed |= run_sort_case(counting_sort, "counting_sort", data_duplicates, data_duplicates, 20);

    int data_radix[20];
    fill_random(data_radix, 20, 7u);
    failed |= run_sort_case(radix_sort, "radix_sort", data_radix, data_radix, 20);

    if (failed) {
        printf("Des tests ont echoue.\n");
        return 1;
    }

    printf("Tous les tests de base sont passes.\n");
    return 0;
}
