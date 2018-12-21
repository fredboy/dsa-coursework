#include <string.h>
#include "record_utils.h"

int compare_strings(char *a, char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
        i++;
    }
    if (a[i] == b[i]) return 0;
    else if (a[i] == '\0') return -1;
    else return 1;
}

int compare_strings_n(char *a, char *b, int n) {
    int i = 0;
    while (i < n && a[i] != '\0' && b[i] != '\0') {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
        i++;
    }
    return 0;
}

int compare_records_by_attorney(struct record *a, struct record *b) {
    int comp_att = compare_strings(a->attorney, b->attorney);
    if (comp_att != 0) return comp_att;
    else if (a->sum < b->sum) return -1;
    else if (a->sum > b->sum) return 1;
    else return 0;
}

int compare_records_by_name(struct record *a, struct record *b) {
    int comp_name = compare_strings(a->name, b->name);
    return comp_name;
}

int binary_search_by_attorney_first(struct record **rec_arr, int l, int r, char *key) {
    if (r >= l) {
        int mid = l + (r - l) / 2;
        int is_first = (mid == 0 || compare_strings_n(rec_arr[mid - 1]->attorney, rec_arr[mid]->attorney, 3) != 0);
        if (compare_strings_n(rec_arr[mid]->attorney, key, 3) == 0 && is_first) {
            return mid;
        } else if (!is_first && compare_strings_n(rec_arr[mid]->attorney, key, 3) >= 0) {
            return binary_search_by_attorney_first(rec_arr, l, mid - 1, key);
        } else {
            return binary_search_by_attorney_first(rec_arr, mid + 1, r, key);
        }
    } else {
        return -1;
    }
}

int binary_search_by_attorney(struct record **rec_arr, int n, char *key) {
    return binary_search_by_attorney_first(rec_arr, 0, n - 1, key);
}