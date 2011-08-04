#include "File_IO.h"

#define RAISE 100;

/**
 * Fügt ein Charakter dem Histogram an einer Position hinzu.
 * @param h Zeiger auf das Histogram.
 * @param pos Index welches Histogram geändert werden soll.
 * @param c Charakter das dem Histogram hinzugefügt werden soll.
 */
void addCharToHistogram(Histogram *h, int index, char c) {
	// kleinbuchstaben
	if (c >= 'a' && c <= 'z') {
		h[index].letter[c - 97 + 26]++;	
	}

	// großbuchstaben
	if (c >= 'A' && c <= 'Z') {
		h[index].letter[c - 65]++;	
	}
}

/**
 * Fügt die Cursor Position in der Datei dem Histogram hinzu.
 * @param h
 * @param index
 * @param cursor
 */
void addCursorToHistogram(Histogram *h, int index, int cursor) {
	//h[index].cursor = (int*) malloc (sizeof(int));
	h[index].cursor = cursor;
	//printf("h[%d].cursor = %d\n", index, h[index].cursor);
}

/** 
 * Initialisiert ein Histogram.
 */
void init(Histogram* h, int index) {
	h[index].cursor = -1;
	int i;
	for (i = 0; i < 52; i++) {
		h[index].letter[i] = 0;	
	}
}

/** 
 * Liest den Inhalt einer Datei ein innerhalb eines bestimmten Bereiches.
 * @param datei Pointer auf die Datei die gelesen werden soll.
 * @param from Position an der das erste Zeichen gelesen wird.
 * @param to Position an der das letzte Zeichen gelesen wird.
 * @return int The total Number of Histogramme
*/
Histogram* readFileFromTo(FILE *datei, const int from, const int to, Histogram *h, int *size) {
	//printf("call readFileFromTo()\n");
	fseek(datei, from, SEEK_SET);
	
	*size = RAISE;
	//printf("size = %d\n", *size);
	
	h = (Histogram*) malloc (sizeof(Histogram) * (*size));
	int index = 0; // Zähle die Histogramme hoch, nach jedem Zeilenende.
	char c;	
	init(h, index);
	//h[index].letter = (char) malloc (sizeof(char) * 52);
	
	// Erste Cursor position ist der Wert von From!
	addCursorToHistogram(h, index, from);
	
	bool changeStateOfHistogram = FALSE;

	do { // Lese Zeichen bis \n entdeckt wird
		
		c = fgetc(datei); // Lese Zeichen
		if (isalpha(c) != 0) {
			//if (index == 100)
			//printf("Lese Zeichen: %c\n", c);
			// Wenn kein Zeilenumbruch dann Füge Zeichen dem Histogram hinzu.
			// Jede Zeile ist ein eigenes Histogram
			addCharToHistogram(h, index, c);
			if (changeStateOfHistogram == FALSE) {
				changeStateOfHistogram = TRUE;
			}
			continue;
		}
		
		if (c == '\n') {
			//if (index == 100)
			//printHistogramStruct(h, 100);
			// Wenn c ein Zeilenumbruch ist
			if (changeStateOfHistogram == TRUE) {
				index++;
				changeStateOfHistogram = FALSE;
				if (index >= *size) {
					*size += RAISE; // Erhöhe den Speicher um x
					h = realloc(h, (*size)*sizeof(Histogram));
				}
				init(h, index);
			}
			
			// Füge Cursor dem Histogram hinzu
			addCursorToHistogram(h, index, ftell(datei));
		}
	} while (ftell(datei) < to);
	
	//printf("index = %d\n", index);
	//printf("size = %d\n", *size);
	
	if ( (index+1) <= (*size) ) {
    *size = index;
		printf("Reduziere mit realloc() size = %d\n", index);
		h = realloc(h, (index)*sizeof(Histogram)); // Reduziere ggf. zu viel allokierten Speicher.
	}
	
	return h;
}

/**
 * Liest den Inhalt einer Datei ein, abhängig von dem aktuellen Rang des Prozesses und der Gesamtzahl der Prozesse.
 * @param myrank Der Rang dieses Prozesses.
 * @param numRank Die Gesammtzahl der Prozesse.
*/
Histogram* readFile(const char* filename, const int myRank, const int numRank, Histogram *h, int *size) {
	FILE *datei;

   /* Bitte Pfad und Dateinamen anpassen */
   datei = fopen(filename, "r");
   if(NULL == datei) {
      printf("Konnte Datei %s nicht öffnen!\n", filename);
      return;
   }

	/* Bestimme die Länge der Datei */
	fseek(datei, 0L, SEEK_END); // Setze den Cursor ans Ende der Datei
	int length = ftell(datei);

	//printf("Size of File: %d\n",length);

	/* Teile die Länge der Datei durch die Anzahl der Prozesse */
	int devided = length / numRank;

	/* Berechne From */
	int from = myRank * devided;
	
	/* Berechne To */
	int to = ((myRank+1) * devided);

	/* Wenn das nicht der erste Prozess ist */
	if (myRank != 0) {
		// Bestimme die korrekte From position.
		fseek(datei, from, SEEK_SET);

		// Lese Zeichen bis Zeilenende
		char c;
		while( (c = fgetc(datei)) != '\n')
         ;

		// after line break has found, the next char is the first char in next line
		// new from position is from here
		from = ftell(datei);
	}

	// 6) if this is not the last prozess
	// determine real to position
	if (myRank != (numRank-1)) {
		fseek(datei, to, SEEK_SET);
		
		// read chars while line break has found
		char c;		
		while( (c = fgetc(datei)) != '\n')
         ;

		// after line break has found, the next char is the first char in next line
		// go back to before the line break.
		to = ftell(datei);
		to--;
	} else {
		fseek(datei, 0, SEEK_END);
		to = ftell(datei);
	}
/*
	printf("Prozess: %d von %d \n", myRank, numRank);
	printf("From: %d \n", from);
	printf("TO: %d \n", to);

	*/
	h = readFileFromTo(datei, from, to, h, size);
	printf("%d Elements in HistogramArray.\n",*size);
  
	
	return h;
}
