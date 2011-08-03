#include "Terminal_IO.h"

/**
 * Druckt ein Histogram im Terminal aus.
 */
void printHistogram(Histogram *h, int index) {
	int n;
	// Drucke Cursor
	printf("h[%d].cursor = %d\n", index, h[index].cursor);
	
	for (n = 0; n < 52; n++) {
		if (n >= 0 && n < 26) { // Großbuchstaben
			printf("h[%d].letter[%c] = %d\n", index, (char) n+65, h[index].letter[n]);
		}

		if (n >= 26 && n < 52) {
			printf("h[%d].letter[%c] = %d\n", index, (char) n-26+97, h[index].letter[n]);
		}
		//printf("h[%d].letter[%d] = %d\n", index, n, h[index].letter[n]);
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
