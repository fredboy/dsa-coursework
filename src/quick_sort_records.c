#include "quick_sort_records.h"
#include "record_utils.h"

void swap_records(struct record **a, struct record **b) {
    struct record *c = *a;
    *a = *b;
    *b = c;
}

int partition(struct record **rec_arr, int l, int h) {
    struct record *pivot = rec_arr[h];
    int i = l - 1;
    for (int j = l; j <= h - 1; j++) {
        if (compare_records_by_attorney(rec_arr[j], pivot) <= 0) {
            i++;
            swap_records(&rec_arr[i], &rec_arr[j]);
        }
    }
    swap_records(&rec_arr[i + 1], &rec_arr[h]);
    return i + 1;
}

void quick_sort_records(struct record **rec_arr, int l, int h) {
    if (l < h) {
        int pi = partition(rec_arr, l, h);
        quick_sort_records(rec_arr, l, pi - 1);
        quick_sort_records(rec_arr, pi + 1, h);
    }
}