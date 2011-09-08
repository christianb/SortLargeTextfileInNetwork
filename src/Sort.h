#ifndef SORT_H
#define SORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Histogram.h"

Histogram** sort(Histogram **data, unsigned int *size);
Histogram** merge(Histogram **left, unsigned int *size_left, Histogram **right, unsigned int *size_right, Histogram **merged);
int getSumOfLetterCaseInsensitive(short index_letter, unsigned int index, Histogram **histogram);

#endif /* SORT_H */
