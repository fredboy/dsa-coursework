#include <stdlib.h>
#include <math.h>
#include <memory.h>
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
    for (int j = l; j <= h - 1; j++) {
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

int *count_codes(FILE *file) {
    char r_ch = 0;
    int tmp = 0;
    N = 0;
    ALL = 0;
    for (int i = 0; i < ABC; i++) {
        abc[i] = 0;
        abc_count[i] = 0;
        Q[i] = 0;
        L[i] = 0;
    }
    while (!feof(file)) {
        fread(&r_ch, sizeof(char), 1, file);
        tmp = contains(r_ch);
        if (tmp == -1) {
            abc[N] = r_ch;
            abc_count[N]++;
            N++;
        } else {
            abc_count[tmp]++;
        }
        ALL++;
    }
    quick_sort_abc(0, N - 1);
    printf("----------------------------------------------------\n");
    double avg_L = 0;
    double coef = 0;
    double H = 0;
    for (int i = 0; i < N; i++) {
        abc_prob[i] = (double) abc_count[i] / ALL;
        H -= (abc_prob[i] * log2(abc_prob[i]));
        L[i] = ((int) ceil(-log2(abc_prob[i])));
        avg_L += L[i] * abc_prob[i];
        coef += (L[i] * abc_count[i]);
        if (i > 0) {
            Q[i] = Q[i - 1] + abc_prob[i - 1];
        }
        printf("|%4d. | %c | %.4f | %.4f | %2d | %15s|\n", i, abc[i], abc_prob[i], Q[i], L[i], to_binary(Q[i], L[i]));
        printf("----------------------------------------------------\n");
    }
    coef = (double) (ALL * 8) / coef;
    printf("Average L = %.4f\nEntropy = %.4f\n", avg_L, H);
    printf("Compression coefficient: %.2f\n", coef);
}

char curr_byte = 0;
char bit_count = 0;

void write_bit(FILE *file, char bit) {
    bit -= '0';
    if (bit < 0 || bit > 1) return;
    curr_byte = curr_byte << 1 | bit;
    bit_count++;
    if (bit_count == 8) {
        fwrite(&curr_byte, sizeof(char), 1, file);
        curr_byte = 0;
        bit_count = 0;
    }
}

void write_compressed(FILE *source) {
    char r_ch = 0;
    char *bin;
    int t = 0;
    FILE *dest = fopen("new_base_3.dat", "wb");
    while (!feof(source)) {
        fread(&r_ch, sizeof(char), 1, source);
        t = contains(r_ch);
        if (t == -1) continue;
        bin = to_binary(Q[t], L[t]);
        for (int i = 0; i < L[t]; i++) write_bit(dest, bin[i]);
    }
    if (bit_count != 0) for (int i = 0; i < 8 - bit_count; i++) write_bit(dest, '0');
    fclose(dest);
}

