#include "Terminal_IO.h"

/**
 * Druckt ein Histogram im Terminal aus.
 */
void printHistogram(Histogram *h, int index) {
	int n;
	// Drucke Cursor
	printf("h[%d].cursor = %d\n", index, h[index].cursor);
	
	for (n = 0; n < 52; n++) {
		printf("h[%d].letter[%d] = %d\n", index, n, h[index].letter[n]);
	}
}

/**
 * Druckt alle Elemente des Histograms (Letter Array und Cursor) auf dem Terminal aus.
 * @param h Histogramme
 * @param size Anzahl der Histogramme
 */
void printHistogramArray(Histogram *h, int size) {
	int index;
	for (index = 0; index < size; index++) {
		printHistogram(h, index);
	}
}