#include "generators.h"
#include "sort.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <math.h>

#define NB_REPETITIONS 5
#define NB_SIZES 6

#define MAX_SIZE_SLOW 50000
#define MAX_SIZE_FAST 10000000

size_t sizes[NB_SIZES] = {
    1000,
    10000,
    100000,
    1000000,
    5000000,
    10000000
};

typedef void (*sort_function)(
    int *,
    size_t,
    sort_stats_t *
);

int is_sorted(int *array, size_t n)
{
    size_t i;

    for (i = 1; i < n; i++)
    {
        if (array[i - 1] > array[i])
        {
            return 0;
        }
    }

    return 1;
}

double get_time_ms(void)
{
    return (double)clock() * 1000.0 / CLOCKS_PER_SEC;
}

double calculer_moyenne(double temps[])
{
    double somme = 0.0;
    int i;

    for (i = 0; i < NB_REPETITIONS; i++)
    {
        somme = somme + temps[i];
    }

    return somme / NB_REPETITIONS;
}

double calculer_ecart_type(
    double temps[],
    double moyenne
)
{
    double somme = 0.0;
    double difference;
    int i;

    for (i = 0; i < NB_REPETITIONS; i++)
    {
        difference = temps[i] - moyenne;

        somme = somme + difference * difference;
    }

    return sqrt(somme / NB_REPETITIONS);
}

void test_algorithm(
    const char *algorithm_name,
    sort_function sort,
    size_t max_size,
    const char *data_name,
    int *original,
    int *array,
    size_t n,
    FILE *file,
    FILE *stats_file
)
{
    int repetition;

    double temps[NB_REPETITIONS];

    double moyenne;
    double ecart_type;

    if (n > max_size)
    {
        return;
    }

    for (
        repetition = 1;
        repetition <= NB_REPETITIONS;
        repetition++
    )
    {
        sort_stats_t stats;

        double start;
        double end;
        double time_ms;

        memcpy(
            array,
            original,
            n * sizeof(int)
        );

        reset_stats(&stats);

        start = get_time_ms();

        sort(array, n, &stats);

        end = get_time_ms();

        time_ms = end - start;

        temps[repetition - 1] = time_ms;

        if (!is_sorted(array, n))
        {
            printf(
                "ERREUR : %s n'a pas correctement trie le tableau.\n",
                algorithm_name
            );

            return;
        }

        printf(
            "%s | %s | n=%zu | repetition=%d | %.3f ms\n",
            algorithm_name,
            data_name,
            n,
            repetition,
            time_ms
        );

        fprintf(
            file,
            "%s,%s,%zu,%d,%.3f,%llu,%llu\n",
            algorithm_name,
            data_name,
            n,
            repetition,
            time_ms,
            (unsigned long long)stats.comparisons,
            (unsigned long long)stats.exchanges
        );
    }

    moyenne = calculer_moyenne(temps);

    ecart_type = calculer_ecart_type(
        temps,
        moyenne
    );

    printf(
        "   -> Moyenne = %.3f ms | Ecart-type = %.3f ms\n",
        moyenne,
        ecart_type
    );

    fprintf(
        stats_file,
        "%s,%s,%zu,%.3f,%.3f\n",
        algorithm_name,
        data_name,
        n,
        moyenne,
        ecart_type
    );
}

int main(void)
{
    FILE *file;
    FILE *stats_file;

    int *original;
    int *array;

    size_t n;

    int i;

    file = fopen(
        "resultats.csv",
        "w"
    );

    if (file == NULL)
    {
        printf(
            "Erreur : impossible d'ouvrir resultats.csv\n"
        );

        return 1;
    }

    stats_file = fopen(
        "resultats_statistiques.csv",
        "w"
    );

    if (stats_file == NULL)
    {
        printf(
            "Erreur : impossible d'ouvrir resultats_statistiques.csv\n"
        );

        fclose(file);

        return 1;
    }

    fprintf(
        file,
        "algorithm,type_donnees,n,repetition,temps_ms,comparaisons,echanges\n"
    );

    fprintf(
        stats_file,
        "algorithm,type_donnees,n,moyenne_ms,ecart_type_ms\n"
    );

    printf("\n");
    printf("========================================\n");
    printf("       BENCHMARK DES ALGORITHMES\n");
    printf("========================================\n");

    for (i = 0; i < NB_SIZES; i++)
    {
        n = sizes[i];

        printf("\n");
        printf("========================================\n");
        printf("Taille n = %zu\n", n);
        printf("========================================\n");

        original = malloc(
            n * sizeof(int)
        );

        array = malloc(
            n * sizeof(int)
        );

        if (original == NULL || array == NULL)
        {
            printf(
                "Erreur : memoire insuffisante pour n = %zu\n",
                n
            );

            free(original);
            free(array);

            fclose(file);
            fclose(stats_file);

            return 1;
        }

        printf("\n--- Donnees aleatoires ---\n");

        generate_random(
            original,
            n,
            123456789ULL
        );

        test_algorithm(
            "Bubble",
            bubble_sort,
            MAX_SIZE_SLOW,
            "random",
            original,
            array,
            n,
            file,
            stats_file
        );

        test_algorithm(
            "Selection",
            selection_sort,
            MAX_SIZE_SLOW,
            "random",
            original,
            array,
            n,
            file,
            stats_file
        );

        test_algorithm(
            "Insertion",
            insertion_sort,
            MAX_SIZE_SLOW,
            "random",
            original,
            array,
            n,
            file,
            stats_file
        );

        test_algorithm(
            "Shell",
            shell_sort,
            MAX_SIZE_FAST,
            "random",
            original,
            array,
            n,
            file,
            stats_file
        );

        test_algorithm(
            "Merge",
            merge_sort,
            MAX_SIZE_FAST,
            "random",
            original,
            array,
            n,
            file,
            stats_file
        );

        test_algorithm(
            "Quick",
            quick_sort,
            MAX_SIZE_SLOW,
            "random",
            original,
            array,
            n,
            file,
            stats_file
        );

        printf("\n--- Donnees triees ---\n");

        generate_sorted(
            original,
            n
        );

        test_algorithm(
            "Bubble",
            bubble_sort,
            MAX_SIZE_SLOW,
            "sorted",
            original,
            array,
            n,
            file,
            stats_file
        );

        test_algorithm(
            "Selection",
            selection_sort,
            MAX_SIZE_SLOW,
            "sorted",
            original,
            array,
            n,
            file,
            stats_file
        );

        test_algorithm(
            "Insertion",
            insertion_sort,
            MAX_SIZE_SLOW,
            "sorted",
            original,
            array,
            n,
            file,
            stats_file
        );

        test_algorithm(
            "Shell",
            shell_sort,
            MAX_SIZE_FAST,
            "sorted",
            original,
            array,
            n,
            file,
            stats_file
        );

        test_algorithm(
            "Merge",
            merge_sort,
            MAX_SIZE_FAST,
            "sorted",
            original,
            array,
            n,
            file,
            stats_file
        );

        test_algorithm(
            "Quick",
            quick_sort,
            MAX_SIZE_SLOW,
            "sorted",
            original,
            array,
            n,
            file,
            stats_file
        );

        printf("\n--- Donnees inversees ---\n");

        generate_reverse(
            original,
            n
        );

        test_algorithm(
            "Bubble",
            bubble_sort,
            MAX_SIZE_SLOW,
            "reverse",
            original,
            array,
            n,
            file,
            stats_file
        );

        test_algorithm(
            "Selection",
            selection_sort,
            MAX_SIZE_SLOW,
            "reverse",
            original,
            array,
            n,
            file,
            stats_file
        );

        test_algorithm(
            "Insertion",
            insertion_sort,
            MAX_SIZE_SLOW,
            "reverse",
            original,
            array,
            n,
            file,
            stats_file
        );

        test_algorithm(
            "Shell",
            shell_sort,
            MAX_SIZE_FAST,
            "reverse",
            original,
            array,
            n,
            file,
            stats_file
        );

        test_algorithm(
            "Merge",
            merge_sort,
            MAX_SIZE_FAST,
            "reverse",
            original,
            array,
            n,
            file,
            stats_file
        );

        test_algorithm(
            "Quick",
            quick_sort,
            MAX_SIZE_SLOW,
            "reverse",
            original,
            array,
            n,
            file,
            stats_file
        );

        printf("\n--- Donnees avec doublons ---\n");

        generate_duplicates(
            original,
            n,
            100
        );

        test_algorithm(
            "Bubble",
            bubble_sort,
            MAX_SIZE_SLOW,
            "duplicates",
            original,
            array,
            n,
            file,
            stats_file
        );

        test_algorithm(
            "Selection",
            selection_sort,
            MAX_SIZE_SLOW,
            "duplicates",
            original,
            array,
            n,
            file,
            stats_file
        );

        test_algorithm(
            "Insertion",
            insertion_sort,
            MAX_SIZE_SLOW,
            "duplicates",
            original,
            array,
            n,
            file,
            stats_file
        );

        test_algorithm(
            "Shell",
            shell_sort,
            MAX_SIZE_FAST,
            "duplicates",
            original,
            array,
            n,
            file,
            stats_file
        );

        test_algorithm(
            "Merge",
            merge_sort,
            MAX_SIZE_FAST,
            "duplicates",
            original,
            array,
            n,
            file,
            stats_file
        );

        test_algorithm(
            "Quick",
            quick_sort,
            MAX_SIZE_SLOW,
            "duplicates",
            original,
            array,
            n,
            file,
            stats_file
        );

        free(original);
        free(array);
    }

    fclose(file);
    fclose(stats_file);

    printf("\n");
    printf("========================================\n");
    printf("Benchmark termine !\n");
    printf("Les resultats sont dans resultats.csv\n");
    printf("Les statistiques sont dans resultats_statistiques.csv\n");
    printf("========================================\n");

    return 0;
}