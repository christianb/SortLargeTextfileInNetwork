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



/**
 * Druckt alle Elemente des Histograms (Letter Array und Cursor) auf dem Terminal aus.
 * @param h Histogramme
 * @param size Anzahl der Histogramme
 */
void printHistogramArray(Histogram **h, int size) {
  //printf("call HistogramArray(), size = %d\n", size);
	int index;
	//char *zeile;
	for (index = 0; index < size; index++) {
	  printf("%d", index+1);
	  printHistogramElement(h[index]);
	  printf("\n");
		//zeile = getHistogramAsString(h[index]);
		//if (index <= 255) {
		  //printf("%d: %s\n", index, zeile);
		  //free(zeile);
		//}
	}
}

/** Schreibt alle Buchstaben in korrekter Reihenfolge von einem Histogram Element. */
void printHistogramElement(Histogram *h) {
  int n, i;
  char l;
  
  for (n = 0; n < 26; n++) {
    // Schreibe Großbuchstaben
    l = (char) n+65;
    for (i = 0; i < (*h).letter[n]; i++) {
      printf("%c",l);
    }
    
    // Schreibe kleinbuchstaben
    l = (char) n+97;
    for (i = 0; i < (*h).letter[n+26]; i++) {
      printf("%c",l);
    }
  
  }
  
  printf(" at cursor: %d",(*h).cursor);
}
