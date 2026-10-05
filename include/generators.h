#ifndef GENERATORS_H
#define GENERATORS_H

#include <stddef.h>
#include <stdint.h>

void generate_random(int *array, size_t size, uint64_t seed);
void generate_sorted(int *array, size_t size);
void generate_reverse(int *array, size_t size);
void generate_duplicates(int *array, size_t size, int max_value);

#endif
