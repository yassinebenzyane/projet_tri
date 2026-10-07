#ifndef SORT_H
#define SORT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    size_t comparisons;
    size_t exchanges;
} sort_stats_t;

void reset_stats(sort_stats_t *stats);

void bubble_sort(int *array, size_t size, sort_stats_t *stats);
void selection_sort(int *array, size_t size, sort_stats_t *stats);
void insertion_sort(int *array, size_t size, sort_stats_t *stats);
void shell_sort(int *array, size_t size, sort_stats_t *stats);
void merge_sort(int *array, size_t size, sort_stats_t *stats);
void quick_sort(int *array, size_t size, sort_stats_t *stats);

#ifdef __cplusplus
}
#endif

#endif
