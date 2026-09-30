#include "generators.h"
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

static int test_one_case(const char *name,
                        void (*sort_func)(int *, size_t, sort_stats_t *),
                        int *input,
                        size_t size,
                        int *original) {
    int *copy = malloc(size * sizeof(int));
    if (copy == NULL) {
        return 1;
    }

    memcpy(copy, input, size * sizeof(int));

    sort_stats_t stats;
    reset_stats(&stats);
    sort_func(copy, size, &stats);

    if (!is_sorted(copy, size) || !is_permutation(original, copy, size)) {
        printf("[ECHEC] %s\n", name);
        free(copy);
        return 1;
    }

    printf("[OK] %s | n=%zu | comparisons=%zu | exchanges=%zu\n",
           name,
           size,
           stats.comparisons,
           stats.exchanges);

    free(copy);
    return 0;
}

int main(void) {
    int failed = 0;

    int random_data[50];
    int original_random[50];
    generate_random(random_data, 50, 42ULL);
    memcpy(original_random, random_data, sizeof(original_random));
    failed |= test_one_case("random", bubble_sort, random_data, 50, original_random);

    int sorted_data[50];
    int original_sorted[50];
    generate_sorted(sorted_data, 50);
    memcpy(original_sorted, sorted_data, sizeof(original_sorted));
    failed |= test_one_case("sorted", insertion_sort, sorted_data, 50, original_sorted);

    int reverse_data[50];
    int original_reverse[50];
    generate_reverse(reverse_data, 50);
    memcpy(original_reverse, reverse_data, sizeof(original_reverse));
    failed |= test_one_case("reverse", quick_sort, reverse_data, 50, original_reverse);

    int duplicate_data[50];
    int original_duplicate[50];
    generate_duplicates(duplicate_data, 50, 10);
    memcpy(original_duplicate, duplicate_data, sizeof(original_duplicate));
    failed |= test_one_case("duplicates", counting_sort, duplicate_data, 50, original_duplicate);

    if (failed) {
        printf("Validation impossible : au moins un test a echoue.\n");
        return 1;
    }

    printf("Tous les tests de validation de base sont passes.\n");
    return 0;
}
