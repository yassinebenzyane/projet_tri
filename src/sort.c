#include "sort.h"

#include <stdlib.h>
#include <string.h>

static void swap_values(int *a, int *b, sort_stats_t *stats) {
    if (*a == *b) {
        return;
    }

    int tmp = *a;
    *a = *b;
    *b = tmp;

    if (stats != NULL) {
        stats->exchanges++;
    }
}

static int compare_values(int left, int right, sort_stats_t *stats) {
    if (stats != NULL) {
        stats->comparisons++;
    }

    if (left < right) {
        return -1;
    }
    if (left > right) {
        return 1;
    }
    return 0;
}

void reset_stats(sort_stats_t *stats) {
    if (stats == NULL) {
        return;
    }

    stats->comparisons = 0;
    stats->exchanges = 0;
}

void bubble_sort(int *array, size_t size, sort_stats_t *stats) {
    if (array == NULL || size < 2) {
        return;
    }

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j + 1 < size - i; ++j) {
            if (compare_values(array[j], array[j + 1], stats) > 0) {
                swap_values(&array[j], &array[j + 1], stats);
            }
        }
    }
}

void selection_sort(int *array, size_t size, sort_stats_t *stats) {
    if (array == NULL || size < 2) {
        return;
    }

    for (size_t i = 0; i < size; ++i) {
        size_t min_index = i;

        for (size_t j = i + 1; j < size; ++j) {
            if (compare_values(array[j], array[min_index], stats) < 0) {
                min_index = j;
            }
        }

        if (min_index != i) {
            swap_values(&array[i], &array[min_index], stats);
        }
    }
}

void insertion_sort(int *array, size_t size, sort_stats_t *stats) {
    if (array == NULL || size < 2) {
        return;
    }

    for (size_t i = 1; i < size; ++i) {
        int key = array[i];
        size_t j = i;

        while (j > 0 && compare_values(array[j - 1], key, stats) > 0) {
            array[j] = array[j - 1];
            if (stats != NULL) {
                stats->exchanges++;
            }
            --j;
        }

        array[j] = key;
    }
}

void shell_sort(int *array, size_t size, sort_stats_t *stats) {
    if (array == NULL || size < 2) {
        return;
    }

    for (size_t gap = size / 2; gap > 0; gap /= 2) {
        for (size_t i = gap; i < size; ++i) {
            int temp = array[i];
            size_t j = i;

            while (j >= gap && compare_values(array[j - gap], temp, stats) > 0) {
                array[j] = array[j - gap];
                if (stats != NULL) {
                    stats->exchanges++;
                }
                j -= gap;
            }

            array[j] = temp;
        }
    }
}

static void merge_arrays(int *array, size_t left, size_t mid, size_t right, sort_stats_t *stats) {
    size_t left_size = mid - left + 1;
    size_t right_size = right - mid;

    int *left_array = malloc(left_size * sizeof(int));
    int *right_array = malloc(right_size * sizeof(int));

    if (left_array == NULL || right_array == NULL) {
        free(left_array);
        free(right_array);
        return;
    }

    memcpy(left_array, array + left, left_size * sizeof(int));
    memcpy(right_array, array + mid + 1, right_size * sizeof(int));

    size_t i = 0, j = 0, k = left;
    while (i < left_size && j < right_size) {
        if (compare_values(left_array[i], right_array[j], stats) <= 0) {
            array[k++] = left_array[i++];
        } else {
            array[k++] = right_array[j++];
            if (stats != NULL) {
                stats->exchanges++;
            }
        }
    }

    while (i < left_size) {
        array[k++] = left_array[i++];
    }

    while (j < right_size) {
        array[k++] = right_array[j++];
        if (stats != NULL) {
            stats->exchanges++;
        }
    }

    free(left_array);
    free(right_array);
}

static void merge_sort_recursive(int *array, size_t left, size_t right, sort_stats_t *stats) {
    if (left >= right) {
        return;
    }

    size_t mid = left + (right - left) / 2;
    merge_sort_recursive(array, left, mid, stats);
    merge_sort_recursive(array, mid + 1, right, stats);
    merge_arrays(array, left, mid, right, stats);
}

void merge_sort(int *array, size_t size, sort_stats_t *stats) {
    if (array == NULL || size < 2) {
        return;
    }

    merge_sort_recursive(array, 0, size - 1, stats);
}

static size_t partition_quick(int *array, size_t left, size_t right, sort_stats_t *stats) {
    int pivot = array[right];
    size_t i = left;

    for (size_t j = left; j < right; ++j) {
        if (compare_values(array[j], pivot, stats) <= 0) {
            swap_values(&array[i], &array[j], stats);
            ++i;
        }
    }

    swap_values(&array[i], &array[right], stats);
    return i;
}

static void quick_sort_recursive(int *array, size_t left, size_t right, sort_stats_t *stats) {
    if (left >= right) {
        return;
    }

    size_t pivot_index = partition_quick(array, left, right, stats);
    if (pivot_index > left) {
        quick_sort_recursive(array, left, pivot_index - 1, stats);
    }
    if (pivot_index + 1 < right) {
        quick_sort_recursive(array, pivot_index + 1, right, stats);
    }
}

void quick_sort(int *array, size_t size, sort_stats_t *stats) {
    if (array == NULL || size < 2) {
        return;
    }

    quick_sort_recursive(array, 0, size - 1, stats);
}

static void heapify(int *array, size_t size, size_t index, sort_stats_t *stats) {
    size_t largest = index;
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;

    if (left < size && compare_values(array[left], array[largest], stats) > 0) {
        largest = left;
    }

    if (right < size && compare_values(array[right], array[largest], stats) > 0) {
        largest = right;
    }

    if (largest != index) {
        swap_values(&array[index], &array[largest], stats);
        heapify(array, size, largest, stats);
    }
}

void heap_sort(int *array, size_t size, sort_stats_t *stats) {
    if (array == NULL || size < 2) {
        return;
    }

    for (size_t i = size / 2; i > 0; --i) {
        heapify(array, size, i - 1, stats);
    }

    for (size_t i = size; i > 1; --i) {
        swap_values(&array[0], &array[i - 1], stats);
        heapify(array, i - 1, 0, stats);
    }
}

void counting_sort(int *array, size_t size, sort_stats_t *stats) {
    if (array == NULL || size < 2) {
        return;
    }

    int max_value = array[0];
    for (size_t i = 1; i < size; ++i) {
        if (array[i] > max_value) {
            max_value = array[i];
        }
    }

    int *count = calloc((size_t)max_value + 1, sizeof(int));
    if (count == NULL) {
        return;
    }

    for (size_t i = 0; i < size; ++i) {
        count[array[i]]++;
        if (stats != NULL) {
            stats->comparisons++;
        }
    }

    size_t index = 0;
    for (int value = 0; value <= max_value; ++value) {
        while (count[value] > 0) {
            array[index++] = value;
            count[value]--;
            if (stats != NULL) {
                stats->exchanges++;
            }
        }
    }

    free(count);
}

void radix_sort(int *array, size_t size, sort_stats_t *stats) {
    if (array == NULL || size < 2) {
        return;
    }

    int max_value = array[0];
    for (size_t i = 1; i < size; ++i) {
        if (array[i] > max_value) {
            max_value = array[i];
        }
    }

    for (int exp = 1; max_value / exp > 0; exp *= 10) {
        int *output = malloc(size * sizeof(int));
        int *count = calloc(10, sizeof(int));

        if (output == NULL || count == NULL) {
            free(output);
            free(count);
            return;
        }

        for (size_t i = 0; i < size; ++i) {
            int digit = (array[i] / exp) % 10;
            count[digit]++;
            if (stats != NULL) {
                stats->comparisons++;
            }
        }

        for (int i = 1; i < 10; ++i) {
            count[i] += count[i - 1];
        }

        for (size_t i = size; i > 0; --i) {
            int digit = (array[i - 1] / exp) % 10;
            int index = --count[digit];
            output[index] = array[i - 1];
            if (stats != NULL) {
                stats->exchanges++;
            }
        }

        memcpy(array, output, size * sizeof(int));
        free(output);
        free(count);
    }
}
