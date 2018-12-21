#ifndef DSA_COURSEWORK_RECORD_UTILS_H
#define DSA_COURSEWORK_RECORD_UTILS_H

#include "record.h"

int compare_strings(char *a, char *b);

int compare_strings_n(char *a, char *b, int n);

int compare_records_by_attorney(struct record *a, struct record *b);

int binary_search_by_attorney(struct record **rec_arr, int n, char *key);

int compare_records_by_name(struct record *a, struct record *b);

#endif //DSA_COURSEWORK_RECORD_UTILS_H
