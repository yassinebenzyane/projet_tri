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

/* Copie triee (par qsort) des donnees courantes : sert de reference
   pour verifier que chaque tri produit le bon resultat. */
int *reference;

int nb_erreurs = 0;

int compare_int(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;

    return (x > y) - (x < y);
}

void prepare_reference(int *original, size_t n)
{
    memcpy(
        reference,
        original,
        n * sizeof(int)
    );

    qsort(
        reference,
        n,
        sizeof(int),
        compare_int
    );
}

/* Trie ET permutation des donnees d'origine : le tableau doit etre
   identique a la reference. */
int is_correctly_sorted(int *array, size_t n)
{
    return memcmp(array, reference, n * sizeof(int)) == 0;
}

/* timespec_get (C11) : resolution bien meilleure que clock(),
   qui n'avance que par pas de 1 ms sous Windows. */
double get_time_ms(void)
{
    struct timespec ts;

    timespec_get(&ts, TIME_UTC);

    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1000000.0;
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

    /* Ecart-type d'echantillon (n - 1) : les repetitions sont un
       echantillon des temps possibles. */
    return sqrt(somme / (NB_REPETITIONS - 1));
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

        if (!is_correctly_sorted(array, n))
        {
            printf(
                "ERREUR : %s n'a pas correctement trie le tableau (%s, n=%zu).\n",
                algorithm_name,
                data_name,
                n
            );

            nb_erreurs++;

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
        "algorithme,type_donnees,n,repetition,temps_ms,comparaisons,echanges\n"
    );

    fprintf(
        stats_file,
        "algorithme,type_donnees,n,moyenne_ms,ecart_type_ms\n"
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

        reference = malloc(
            n * sizeof(int)
        );

        if (original == NULL || array == NULL || reference == NULL)
        {
            printf(
                "Erreur : memoire insuffisante pour n = %zu\n",
                n
            );

            free(original);
            free(array);
            free(reference);

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

        prepare_reference(original, n);

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
            MAX_SIZE_FAST,
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

        prepare_reference(original, n);

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
            MAX_SIZE_FAST,
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

        prepare_reference(original, n);

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
            MAX_SIZE_FAST,
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

        prepare_reference(original, n);

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
            MAX_SIZE_FAST,
            "duplicates",
            original,
            array,
            n,
            file,
            stats_file
        );

        free(original);
        free(array);
        free(reference);
    }

    fclose(file);
    fclose(stats_file);

    if (nb_erreurs > 0)
    {
        printf(
            "\nATTENTION : %d cas incorrect(s), les resultats ne sont pas valides.\n",
            nb_erreurs
        );

        return 1;
    }

    printf("\n");
    printf("========================================\n");
    printf("Benchmark termine !\n");
    printf("Les resultats sont dans resultats.csv\n");
    printf("Les statistiques sont dans resultats_statistiques.csv\n");
    printf("========================================\n");

    return 0;
}