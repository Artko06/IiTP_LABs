#define _POSIX_C_SOURCE 200809L

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
    int *array;
    size_t start;
    size_t length;
} thread_arg_t;

int cmp(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

void *sort_fragment(void *arg)
{
    thread_arg_t *t = arg;
    qsort(t->array + t->start, t->length, sizeof(int), cmp);
    return NULL;
}

void merge(int *arr, size_t l, size_t m, size_t r)
{
    size_t i = l, j = m, k = 0;
    int *tmp = malloc((r - l) * sizeof(int));

    while (i < m && j < r)
        tmp[k++] = arr[i] < arr[j] ? arr[i++] : arr[j++];

    while (i < m)
        tmp[k++] = arr[i++];
    while (j < r)
        tmp[k++] = arr[j++];

    for (i = 0; i < k; i++)
        arr[l + i] = tmp[i];

    free(tmp);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <array_size> <threads>\n", argv[0]);
        return EXIT_FAILURE;
    }

    size_t n = atoi(argv[1]);
    int tcount = atoi(argv[2]);

    int *array = malloc(n * sizeof(int));
    pthread_t *threads = malloc(tcount * sizeof(pthread_t));
    thread_arg_t *args = malloc(tcount * sizeof(thread_arg_t));

    srand(time(NULL));
    for (size_t i = 0; i < n; i++)
        array[i] = rand() % 100000;

    size_t chunk = n / tcount;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < tcount; i++)
    {
        args[i].array = array;
        args[i].start = i * chunk;
        args[i].length = (i == tcount - 1)
                             ? n - args[i].start
                             : chunk;

        pthread_create(&threads[i], NULL, sort_fragment, &args[i]);
    }

    for (int i = 0; i < tcount; i++)
        pthread_join(threads[i], NULL);

    size_t step = chunk;
    while (step < n)
    {
        for (size_t i = 0; i + step < n; i += 2 * step)
        {
            size_t mid = i + step;
            size_t endp = (i + 2 * step < n) ? i + 2 * step : n;
            merge(array, i, mid, endp);
        }
        step *= 2;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_spent =
        (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Array size: %zu\nThreads: %d\nTime: %.6f sec\n",
           n, tcount, time_spent);

    free(array);
    free(threads);
    free(args);
    return 0;
}

/*
make run1

make run2

make run4

make run8

make run16

make clean
*/