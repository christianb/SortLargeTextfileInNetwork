/* fopen1.c */
#include <stdio.h>
#include <stdlib.h>

/** 
 * Liest den Inhalt einer Datei ein innerhalb eines bestimmten Bereiches.
 * @param datei Pointer auf die Datei die gelesen werden soll.
 * @param from Position an der das erste Zeichen gelesen wird.
 * @param to Position an der das letzte Zeichen gelesen wird.
*/
void readFileFromTo(FILE *datei, const int from, const int to) {
	fseek(datei, from, SEEK_SET);
	
	char c;	
	do {
		c = fgetc(datei);
        putchar(c);
	} while (ftell(datei) < to);
}

/**
 * Liest den Inhalt einer Datei ein, abhängig von dem aktuellen Rang des Prozesses und der Gesamtzahl der Prozesse.
 * @param myrank Der Rang dieses Prozesses.
 * @param numRank Die Gesammtzahl der Prozesse.
*/
void readFile(const int myRank, const int numRank) {
	FILE *datei;

   /* Bitte Pfad und Dateinamen anpassen */
   datei = fopen("../Euler.txt", "r");
   if(NULL == datei) {
      printf("Konnte Datei \"Euler.txt\" nicht öffnen!\n");
      return;
   }

	/* Bestimme die Länge der Datei */
	fseek(datei, 0L, SEEK_END); // Setze den Cursor ans Ende der Datei
	int length = ftell(datei);

	printf("Size of File: %d\n",length);

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

	printf("Prozess: %d von %d \n", myRank, numRank);
	printf("From: %d \n", from);
	printf("TO: %d \n", to);

	readFileFromTo(datei, from, to);
}

int main(void) {
	const int myRank = 0;
	const int numRank = 8;
	readFile(myRank, numRank);

	return EXIT_SUCCESS;
}
