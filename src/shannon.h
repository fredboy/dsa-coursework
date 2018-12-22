#ifndef DSA_COURSEWORK_SHANNON_H
#define DSA_COURSEWORK_SHANNON_H

#include "record.h"

#define ABC 256

int L[ABC];

int *count_codes(FILE *file);

void write_compressed(FILE *source);

#endif //DSA_COURSEWORK_SHANNON_H
