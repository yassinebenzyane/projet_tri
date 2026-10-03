#include "sort.h"

#include <stdlib.h>
#include <string.h>


static void swap_values(int *a, int *b, sort_stats_t *stats)
{
    if (*a == *b)
    {
        return;
    }

    int tmp = *a;
    *a = *b;
    *b = tmp;

    if (stats != NULL)
    {
        stats->exchanges++;
    }
}


static int compare_values(
    int left,
    int right,
    sort_stats_t *stats
)
{
    if (stats != NULL)
    {
        stats->comparisons++;
    }

    if (left < right)
    {
        return -1;
    }

    if (left > right)
    {
        return 1;
    }

    return 0;
}


void reset_stats(sort_stats_t *stats)
{
    if (stats == NULL)
    {
        return;
    }

    stats->comparisons = 0;
    stats->exchanges = 0;
}


void bubble_sort(
    int *array,
    size_t size,
    sort_stats_t *stats
)
{
    if (array == NULL || size < 2)
    {
        return;
    }

    for (size_t i = 0; i < size; ++i)
    {
        for (size_t j = 0;
             j + 1 < size - i;
             ++j)
        {
            if (compare_values(
                    array[j],
                    array[j + 1],
                    stats
                ) > 0)
            {
                swap_values(
                    &array[j],
                    &array[j + 1],
                    stats
                );
            }
        }
    }
}


void selection_sort(
    int *array,
    size_t size,
    sort_stats_t *stats
)
{
    if (array == NULL || size < 2)
    {
        return;
    }

    for (size_t i = 0; i < size; ++i)
    {
        size_t min_index = i;

        for (size_t j = i + 1;
             j < size;
             ++j)
        {
            if (compare_values(
                    array[j],
                    array[min_index],
                    stats
                ) < 0)
            {
                min_index = j;
            }
        }

        if (min_index != i)
        {
            swap_values(
                &array[i],
                &array[min_index],
                stats
            );
        }
    }
}


void insertion_sort(
    int *array,
    size_t size,
    sort_stats_t *stats
)
{
    if (array == NULL || size < 2)
    {
        return;
    }

    for (size_t i = 1;
         i < size;
         ++i)
    {
        int key = array[i];
        size_t j = i;

        while (
            j > 0 &&
            compare_values(
                array[j - 1],
                key,
                stats
            ) > 0
        )
        {
            array[j] = array[j - 1];

            if (stats != NULL)
            {
                stats->exchanges++;
            }

            --j;
        }

        array[j] = key;
    }
}


void shell_sort(
    int *array,
    size_t size,
    sort_stats_t *stats
)
{
    if (array == NULL || size < 2)
    {
        return;
    }

    for (size_t gap = size / 2;
         gap > 0;
         gap /= 2)
    {
        for (size_t i = gap;
             i < size;
             ++i)
        {
            int temp = array[i];
            size_t j = i;

            while (
                j >= gap &&
                compare_values(
                    array[j - gap],
                    temp,
                    stats
                ) > 0
            )
            {
                array[j] = array[j - gap];

                if (stats != NULL)
                {
                    stats->exchanges++;
                }

                j -= gap;
            }

            array[j] = temp;
        }
    }
}


static void merge_arrays(
    int *array,
    int *temp,
    size_t left,
    size_t mid,
    size_t right,
    sort_stats_t *stats
)
{
    size_t i = left;
    size_t j = mid + 1;
    size_t k = left;

    while (i <= mid && j <= right)
    {
        if (compare_values(
                array[i],
                array[j],
                stats
            ) <= 0)
        {
            temp[k++] = array[i++];
        }
        else
        {
            temp[k++] = array[j++];
        }
    }

    while (i <= mid)
    {
        temp[k++] = array[i++];
    }

    while (j <= right)
    {
        temp[k++] = array[j++];
    }

    for (size_t p = left;
         p <= right;
         ++p)
    {
        array[p] = temp[p];

        if (stats != NULL)
        {
            stats->exchanges++;
        }
    }
}


static void merge_sort_recursive(
    int *array,
    int *temp,
    size_t left,
    size_t right,
    sort_stats_t *stats
)
{
    if (left >= right)
    {
        return;
    }

    size_t mid =
        left + (right - left) / 2;

    merge_sort_recursive(
        array,
        temp,
        left,
        mid,
        stats
    );

    merge_sort_recursive(
        array,
        temp,
        mid + 1,
        right,
        stats
    );

    merge_arrays(
        array,
        temp,
        left,
        mid,
        right,
        stats
    );
}


void merge_sort(
    int *array,
    size_t size,
    sort_stats_t *stats
)
{
    if (array == NULL || size < 2)
    {
        return;
    }

    int *temp =
        malloc(size * sizeof(int));

    if (temp == NULL)
    {
        return;
    }

    merge_sort_recursive(
        array,
        temp,
        0,
        size - 1,
        stats
    );

    free(temp);
}


static void quick_partition(
    int *array,
    size_t left,
    size_t right,
    size_t *less_end,
    size_t *greater_start,
    sort_stats_t *stats
)
{
    int pivot =
        array[left + (right - left) / 2];

    size_t low = left;
    size_t current = left;
    size_t high = right;

    while (current <= high)
    {
        int comparison =
            compare_values(
                array[current],
                pivot,
                stats
            );

        if (comparison < 0)
        {
            swap_values(
                &array[low],
                &array[current],
                stats
            );

            ++low;
            ++current;
        }
        else if (comparison > 0)
        {
            swap_values(
                &array[current],
                &array[high],
                stats
            );

            if (high == 0)
            {
                break;
            }

            --high;
        }
        else
        {
            ++current;
        }
    }

    *less_end = low;
    *greater_start = high + 1;
}


static void quick_sort_recursive(
    int *array,
    size_t left,
    size_t right,
    sort_stats_t *stats
)
{
    while (left < right)
    {
        size_t less_end;
        size_t greater_start;

        quick_partition(
            array,
            left,
            right,
            &less_end,
            &greater_start,
            stats
        );

        size_t left_size =
            (less_end > left)
            ? less_end - left
            : 0;

        size_t right_size =
            (greater_start <= right)
            ? right - greater_start + 1
            : 0;

        if (left_size < right_size)
        {
            if (left_size > 1)
            {
                quick_sort_recursive(
                    array,
                    left,
                    less_end - 1,
                    stats
                );
            }

            left = greater_start;
        }
        else
        {
            if (right_size > 1)
            {
                quick_sort_recursive(
                    array,
                    greater_start,
                    right,
                    stats
                );
            }

            if (less_end == 0)
            {
                break;
            }

            right = less_end - 1;
        }
    }
}


void quick_sort(
    int *array,
    size_t size,
    sort_stats_t *stats
)
{
    if (array == NULL || size < 2)
    {
        return;
    }

    quick_sort_recursive(
        array,
        0,
        size - 1,
        stats
    );
}