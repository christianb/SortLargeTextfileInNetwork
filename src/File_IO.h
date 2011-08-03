#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>
#include <stdlib.h>
#include "Histogram.h"

void addCharToHistogram(Histogram *h, int index, char c);
void addCursorToHistogram(Histogram *h, int index, int cursor);

Histogram* readFileFromTo(FILE *datei, const int from, const int to, Histogram *h, int *size);
Histogram* readFile(const int myRank, const int numRank, Histogram *h, int *size);

#endif /* FILE_H */
