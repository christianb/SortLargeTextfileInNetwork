#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <string.h>
#include <stdlib.h>

typedef struct {
	unsigned char letter[52];
	int cursor;
} Histogram;

typedef enum { FALSE = 0, TRUE = 1 } bool;

//char* getHistogramAsString(Histogram *h);

#endif /* HISTOGRAM_H */
