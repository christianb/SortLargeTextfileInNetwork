#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Histogram.h"

Histogram* readFile(const char *filename, const int myRank, const int numRank, Histogram *h, unsigned int *size);
int writeFile(const char *filename_out, const char *filename_in, Histogram **h, unsigned int *size);
int _writeFile(const char *filename_out, Histogram **h, unsigned int *size);

#endif /* FILE_H */
