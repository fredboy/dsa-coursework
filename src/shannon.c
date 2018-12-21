#include <stdlib.h>
#include <math.h>
#include "shannon.h"

int N = 0;
int ALL = 0;
char abc[ABC];
int abc_count[ABC];
double abc_prob[ABC];
double Q[ABC];

char *to_binary(double q, int l) {
    char *res = (char *) malloc((l + 1) * sizeof(char));
    for (int i = 0; i < l; i++) {
        q *= 2;
        if (q >= 1) {
            res[i] = '1';
            q--;
        } else res[i] = '0';
    }
    res[l] = '\0';
    return res;
}

void swap_abc(int a, int b) {
    char c = abc[a];
    int cc = abc_count[a];
    abc[a] = abc[b];
    abc[b] = c;
    abc_count[a] = abc_count[b];
    abc_count[b] = cc;
}

int partition_abc(int l, int h) {
    int pivot = abc_count[h];
    int i = l - 1;
    int j;
    for (j = l; j <= h - 1; j++) {
        if (abc_count[j] >= pivot) {
            i++;
            swap_abc(i, j);
        }
    }
    swap_abc(i + 1, h);
    return i + 1;
}

void quick_sort_abc(int l, int h) {
    if (l < h) {
        int pi = partition_abc(l, h);
        quick_sort_abc(l, pi - 1);
        quick_sort_abc(pi + 1, h);
    }
}

int contains(char ch) {
    for (int i = 0; i < N; i++) if (abc[i] == ch) return i;
    return -1;
}

int *count_codes(struct record **rec_arr, int n) {
    N = 0;
    ALL = 0;
    for (int i = 0; i < ABC; i++) {
        abc[i] = 0;
        abc_count[i] = 0;
        Q[i] = 0;
        L[i] = 0;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 30; j++) {
            if (contains(rec_arr[i]->name[j]) == -1) {
                abc[N] = rec_arr[i]->name[j];
                abc_count[N]++;
                N++;
            } else {
                abc_count[contains(rec_arr[i]->name[j])]++;
            }
            ALL++;
        }
        for (int j = 0; j < 22; j++) {
            if (contains(rec_arr[i]->attorney[j]) == -1) {
                abc[N] = rec_arr[i]->attorney[j];
                abc_count[N]++;
                N++;
            } else {
                abc_count[contains(rec_arr[i]->date[j])]++;
            }
            ALL++;
        }
        for (int j = 0; j < 10; j++) {
            if (contains(rec_arr[i]->date[j]) == -1) {
                abc[N] = rec_arr[i]->date[j];
                abc_count[N]++;
                N++;
            } else {
                abc_count[contains(rec_arr[i]->date[j])]++;
            }
            ALL++;
        }
    }
    quick_sort_abc(0, N - 1);
    printf("--------------------------------------------------\n");
    double avg_L = 0;
    double H = 0;
    for (int i = 0; i < N; i++) {
        abc_prob[i] = (double) abc_count[i] / ALL;
        H += (abc_prob[i] * log2(abc_prob[i]));
        L[i] = ((int) ceil(-log2(abc_prob[i])));
        avg_L += L[i];
        if (i > 0) {
            Q[i] = Q[i - 1] + abc_prob[i - 1];
        }
        printf("|%2d. | %c | %.4f | %.4f | %2d | %15s|\n", i, abc[i], abc_prob[i], Q[i], L[i], to_binary(Q[i], L[i]));
        printf("--------------------------------------------------\n");
    }
    avg_L /= N;
    H = -H;
    printf("Average L = %.4f\nEntropy = %.4f\n", avg_L, H);
}

