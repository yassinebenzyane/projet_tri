#ifndef GENERATORS_H
#define GENERATORS_H

#include <stddef.h>
#include <stdint.h>

void generate_random(int *array, size_t size, uint64_t seed);
void generate_sorted(int *array, size_t size);
void generate_reverse(int *array, size_t size);
void generate_all_equal(int *array, size_t size, int value);
void generate_duplicates(int *array, size_t size, int max_value);
void generate_nearly_sorted(int *array, size_t size, double disorder_ratio, uint64_t seed);
void generate_adversarial_quicksort(int *array, size_t size, uint64_t seed);

#endif
