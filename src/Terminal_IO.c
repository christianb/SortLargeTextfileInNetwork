#include "Terminal_IO.h"

//void printHistogram(Histogram *h, int index);

/**
 * Druckt ein Histogram im Terminal aus.
 */
void printHistogramStruct(Histogram **h, int index) {
	int n;
	// Drucke Cursor
	printf("h[%d].cursor = %d\n", index, (*h[index]).cursor);
	
	for (n = 0; n < 52; n++) {
		if (n >= 0 && n < 26) { // Großbuchstaben
			printf("h[%d].letter[%c] = %d\n", index, (char) n+65, (*h[index]).letter[n]);
		}

		if (n >= 26 && n < 52) {
			printf("h[%d].letter[%c] = %d\n", index, (char) n-26+97, (*h[index]).letter[n]);
		}
		//printf("h[%d].letter[%d] = %d\n", index, n, h[index].letter[n]);
	}
}

void printHistogramReadable(Histogram **h, int index) {
  int n,i;
  char zeile[126] = {' '}; // Laut Aufgabe hat eine Zeile nur max. 126 Zeichen!
  char l;

  int index_zeile = 0;
  for (n = 0; n < 26; n++) {
    // Schreibe Großbuchstaben
    l = (char) n+65;
    
    for (i = 0; i < (*h[index]).letter[n]; i++) {
      strncpy((zeile+index_zeile), &l, 1);
      index_zeile++;
    }

    // Schreibe kleinbuchstaben
    l = (char) n+97;
    for (i = 0; i < (*h[index]).letter[n+26]; i++) {
      strncpy((zeile+index_zeile), &l, 1);
      index_zeile++;
    }
  }

  printf("%d: %s\n", index, zeile);
}

/**
 * Druckt alle Elemente des Histograms (Letter Array und Cursor) auf dem Terminal aus.
 * @param h Histogramme
 * @param size Anzahl der Histogramme
 */
void printHistogramArray(Histogram **h, int size) {
  printf("call HistogramArray(), size = %d\n", size);
	int index;
	for (index = 0; index < size; index++) {
		printHistogramReadable(h, index);
	}
}
