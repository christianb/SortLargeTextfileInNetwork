#ifndef TERMINAL_IO_H
#define TERMINAL_IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Histogram.h"

void printHistogram(Histogram *h, int index);
void printHistogramArray(Histogram *h, int size);

#endif /* TERMINAL_H */
