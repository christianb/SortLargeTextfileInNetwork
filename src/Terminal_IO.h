#ifndef TERMINAL_IO_H
#define TERMINAL_IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Histogram.h"

void printHistogramArray(Histogram **h, int size);
void printHistogramElement(Histogram *h);

#endif /* TERMINAL_H */
