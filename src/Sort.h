#ifndef SORT_H
#define SORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Histogram.h"

Histogram* sort(Histogram *data, int *size);

Histogram* merge(Histogram *left, int *size_left, Histogram *right, int *size_right);


#endif /* SORT_H */
