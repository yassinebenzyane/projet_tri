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

void bubble_sort(int *array, size_t size, sort_stats_t *stats); // tri à bulles
void selection_sort(int *array, size_t size, sort_stats_t *stats); // tri par sélection
void insertion_sort(int *array, size_t size, sort_stats_t *stats); // tri par insertion
void shell_sort(int *array, size_t size, sort_stats_t *stats); // tri de Shell
void merge_sort(int *array, size_t size, sort_stats_t *stats); // tri par fusion
void quick_sort(int *array, size_t size, sort_stats_t *stats); // tri rapide
void heap_sort(int *array, size_t size, sort_stats_t *stats); // tri par tas
void counting_sort(int *array, size_t size, sort_stats_t *stats); // tri par comptage
void radix_sort(int *array, size_t size, sort_stats_t *stats); // tri par base (radix sort)

#ifdef __cplusplus
}
#endif

#endif
