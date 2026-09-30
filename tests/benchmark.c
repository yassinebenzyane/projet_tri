#include "generators.h"
#include "sort.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static double now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1000000.0;
}

int main(void) {
    const size_t n = 100000;
    int *data = malloc(n * sizeof(int));
    if (data == NULL) {
        fprintf(stderr, "Memoire insuffisante.\n");
        return 1;
    }

    generate_random(data, n, 123ULL);

    sort_stats_t stats;
    reset_stats(&stats);

    double start = now_ms();
    quick_sort(data, n, &stats);
    double elapsed = now_ms() - start;

    printf("algorithme=quick_sort,type=aleatoire,n=%zu,temps_ms=%.3f,comparaisons=%zu,echanges=%zu\n",
           n,
           elapsed,
           stats.comparisons,
           stats.exchanges);

    free(data);
    return 0;
}
