#include <stdio.h>
#include "FIND.h"

int is_5(int x) {
    return x == 5 ? 1 : 0; 
}

int cmp_5(int x) {
    if (x == 5) return 0;
    return (x < 5) ? -1 : 1;
}

int cmp_range_5_7(int x) {
    if (x >= 5 && x <= 7) return 0;
    return (x < 5) ? -1 : 1;
}

int main() {
    int arr_unsorted[] = {1, 5, 2, 5, 10, 6, 5, 7, 8};
    int n_unsorted = sizeof(arr_unsorted)/sizeof(arr_unsorted[0]);

    int arr_sorted[] = {1, 2, 5, 5, 5, 6, 7};
    int n_sorted = sizeof(arr_sorted)/sizeof(arr_sorted[0]);

    int idx = line_find_one(arr_unsorted, n_unsorted, is_5);
    printf("Линейный поиск одного: количество элементов: %d, итераций: %d\n", idx, find_count);

    int inds[10];
    int cnt = line_find_all(arr_unsorted, n_unsorted, is_5, inds, 10);
    printf("Линейный поиск всех: количество элементов: %d, итераций: %d, индексы: ", cnt, find_count);
    for(int i = 0; i < cnt; ++i) printf("%d ", inds[i]);
    printf("\n");
    idx = bin_find_one(arr_sorted, n_sorted, cmp_5);
    printf("Бинарный поиск одного (отсортированный): количество элементов: %d, итераций: %d\n", idx, find_count);

    int beg, end;
    cnt = bin_find_all(arr_sorted, n_sorted, cmp_range_5_7, &beg, &end);
    printf("Бинарный поиск всех (отсортированный): количество элементов: %d, индексы: [%d, %d], итераций: %d\n", cnt, beg, end, find_count);

    return 0;
}
