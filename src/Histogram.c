#include "Histogram.h"
/*
char* getHistogramAsString(Histogram *h) {
  int n,i;
  //char *zeile = (char*) malloc (sizeof(char)*126); // Laut Aufgabe hat eine Zeile nur max. 126 Zeichen!
  char zeile[126];
  
	// init chars
	for (i = 0; i < 126; i++) {
		zeile[i] = 0x0;
	}
  char l;

  int index_zeile = 0;
  for (n = 0; n < 26; n++) {
    // Schreibe Großbuchstaben
    l = (char) n+65;
    
    for (i = 0; i < (*h).letter[n]; i++) {
      strncpy((zeile+index_zeile), &l, 1);
      index_zeile++;
    }

    // Schreibe kleinbuchstaben
    l = (char) n+97;
    for (i = 0; i < (*h).letter[n+26]; i++) {
      strncpy((zeile+index_zeile), &l, 1);
      index_zeile++;
    }
  }

  //printf("%d: %s\n", index, zeile);
	return zeile;
}*/
