#include "generators.h"
#include "sort.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEST_SIZE 200

typedef void (*sort_function)(int *, size_t, sort_stats_t *);

static int is_sorted(const int *array, size_t size) {
    for (size_t i = 1; i < size; ++i) {
        if (array[i - 1] > array[i]) {
            return 0;
        }
    }
    return 1;
}

static int is_permutation(const int *original, const int *sorted, size_t size) {
    if (size == 0) {
        return 1;
    }

    int *copy = malloc(size * sizeof(int));
    if (copy == NULL) {
        return 0;
    }

    memcpy(copy, sorted, size * sizeof(int));
    size_t remaining = size;

    for (size_t i = 0; i < size; ++i) {
        int found = 0;
        for (size_t j = 0; j < remaining; ++j) {
            if (original[i] == copy[j]) {
                /* Retire l'element trouve pour qu'il ne serve qu'une fois. */
                copy[j] = copy[remaining - 1];
                --remaining;
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

static int test_one_case(const char *algorithm_name,
                         sort_function sort_func,
                         const char *data_name,
                         const int *original,
                         size_t size) {
    int copy[MAX_TEST_SIZE];
    memcpy(copy, original, size * sizeof(int));

    sort_stats_t stats;
    reset_stats(&stats);
    sort_func(copy, size, &stats);

    if (!is_sorted(copy, size) || !is_permutation(original, copy, size)) {
        printf("[ECHEC] %s | %s | n=%zu\n", algorithm_name, data_name, size);
        return 1;
    }

    return 0;
}

int main(void) {
    const sort_function algorithms[] = {
        bubble_sort, selection_sort, insertion_sort, shell_sort, merge_sort, quick_sort
    };
    const char *algorithm_names[] = {
        "bubble_sort", "selection_sort", "insertion_sort", "shell_sort", "merge_sort", "quick_sort"
    };
    const char *data_names[] = {"random", "sorted", "reverse", "duplicates"};
    /* Petites tailles (0, 1, 2) pour les cas limites, puis tailles paires et impaires. */
    const size_t sizes[] = {0, 1, 2, 3, 10, 17, 50, MAX_TEST_SIZE};

    const size_t nb_algorithms = sizeof(algorithms) / sizeof(algorithms[0]);
    const size_t nb_data = sizeof(data_names) / sizeof(data_names[0]);
    const size_t nb_sizes = sizeof(sizes) / sizeof(sizes[0]);

    int original[MAX_TEST_SIZE];
    int failed = 0;
    int nb_tests = 0;

    for (size_t s = 0; s < nb_sizes; ++s) {
        size_t size = sizes[s];

        for (size_t d = 0; d < nb_data; ++d) {
            switch (d) {
            case 0: generate_random(original, size, 42ULL + size); break;
            case 1: generate_sorted(original, size); break;
            case 2: generate_reverse(original, size); break;
            default: generate_duplicates(original, size, 10); break;
            }

            for (size_t a = 0; a < nb_algorithms; ++a) {
                failed += test_one_case(algorithm_names[a], algorithms[a], data_names[d], original, size);
                ++nb_tests;
            }
        }
    }

    if (failed) {
        printf("Validation impossible : %d test(s) sur %d ont echoue.\n", failed, nb_tests);
        return 1;
    }

    printf("Tous les tests de validation sont passes (%d cas : %zu algorithmes x %zu types x %zu tailles).\n",
           nb_tests, nb_algorithms, nb_data, nb_sizes);
    return 0;
}
