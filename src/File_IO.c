#include "File_IO.h"
#include <stdlib.h> 
#include <stdio.h> 
#include <fcntl.h> 
#include <sys/mman.h> 
#include <sys/stat.h> 
#include <unistd.h> 

#define RAISE 100;

void addCharToHistogram(Histogram *h, unsigned int index, char c);
void addCursorToHistogram(Histogram *h, unsigned int index, unsigned int cursor);

Histogram* readFileFromTo(FILE *datei, const unsigned int from, const unsigned int to, Histogram *h, unsigned int *size);

/**
 * Fügt ein Charakter dem Histogram an einer Position hinzu.
 * @param h Zeiger auf das Histogram.
 * @param pos Index welches Histogram geändert werden soll.
 * @param c Charakter das dem Histogram hinzugefügt werden soll.
 */
void addCharToHistogram(Histogram *h, unsigned int index, char c) {
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
void addCursorToHistogram(Histogram *h, unsigned int index, unsigned int cursor) {
	//h[index].cursor = (int*) malloc (sizeof(int));
	h[index].cursor = cursor;
	//printf("h[%d].cursor = %d\n", index, h[index].cursor);
}

/** 
 * Initialisiert ein Histogram.
 */
void init(Histogram* h, unsigned int index) {
	h[index].cursor = -1;
	short i;
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
Histogram* readFileFromTo(FILE *datei, const unsigned int from, const unsigned int to, Histogram *h, unsigned int *size) {
	//printf("call readFileFromTo()\n");
	fseek(datei, from, SEEK_SET);
	
	*size = RAISE;
	//printf("size = %d\n", *size);
	
	h = (Histogram*) malloc (sizeof(Histogram) * (*size));
	unsigned int index = 0; // Zähle die Histogramme hoch, nach jedem Zeilenende.
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
		
	} while (( (unsigned) ftell(datei) <= to && c != -1));
	
	//printf("index = %d\n", index);
	//printf("size = %d\n", *size);
	
	if ( (index+1) <= (*size) ) {
		*size = index;
		//printf("Reduziere mit realloc() size = %d\n", index+1);
		h = realloc(h, (index)*sizeof(Histogram)); // Reduziere ggf. zu viel allokierten Speicher.
	}

	
	
	return h;
}

/**
 * Liest den Inhalt einer Datei ein, abhängig von dem aktuellen Rang des Prozesses und der Gesamtzahl der Prozesse.
 * @param myrank Der Rang dieses Prozesses.
 * @param numRank Die Gesammtzahl der Prozesse.
*/
Histogram* readFile(const char* filename, const int myRank, const int numRank, Histogram *h, unsigned int *size) {
	FILE *datei;

   /* Bitte Pfad und Dateinamen anpassen */
   datei = fopen(filename, "r");
   if(NULL == datei) {
      printf("Konnte Datei %s nicht öffnen!\n", filename);
      return EXIT_SUCCESS;
   }
	//fseek(datei, 0L, SEEK_SET); // Cursor an den Anfang setzen.
	//printf("Anfang in Datei: %d\n",ftell(datei));

	/* Bestimme die Länge der Datei */
	fseek(datei, 0L, SEEK_END); // Setze den Cursor ans Ende der Datei
	//printf("Ende in Datei: %d \n",ftell(datei));
	
	unsigned int length = ftell(datei);

	//printf("Size of File: %d\n",length);

	/* Teile die Länge der Datei durch die Anzahl der Prozesse */
	unsigned int devided = length / numRank;

	/* Berechne From */
	unsigned int from = myRank * devided;
	
	/* Berechne To */
	unsigned int to = ((myRank+1) * devided);

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
	//printf("%d Elements in HistogramArray.\n",*size);
  
	fclose(datei);
	
	return h;
}

int writeFile(const char *filename_out, const char *filename_in, Histogram **h, unsigned int *size) {
	FILE *out;
	//FILE *in;

  // Bitte Pfad und Dateinamen anpassen
	out = fopen(filename_out, "w+t");
	//in = fopen(filename_in, "r");

   if(NULL == out) {
      printf("Konnte Datei %s nicht öffnen!\n", filename_out);
      return EXIT_FAILURE;
   }
/*
	if (NULL == in) {
		printf("Konnte Datei %s nicht oeffnen!\n", filename_in);
		return EXIT_FAILURE;
	}
*/
	//char* zeile;
	//char original_word[126];

	unsigned int i;
	//unsigned short n;
	int result;
	
	FILE *datei;
   /* Bitte Pfad und Dateinamen anpassen */
   datei = fopen(filename_in, "r");
   
  char* file_memory; 
  int fd;
  
  fseek(datei, 0L, SEEK_END); // Setze den Cursor ans Ende der Datei
	//printf("Ende in Datei: %d \n",ftell(datei));
	
	unsigned int length = ftell(datei);
	fclose(datei);
	
	fd = open (filename_in, O_RDWR, S_IRUSR | S_IWUSR); 
  
  file_memory = mmap (0, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); 
  close (fd);
  
  char *zeile = (char*) malloc (sizeof(char)*126);
  //int integer;
  //sscanf (file_memory+4, "%d", &integer); 
  //printf ("value: %d\n", integer); 
  //sscanf (file_memory+4, "%s", &zeile);
  //printf ("value: %s\n", zeile); 
  
  //
	
  //char *c = (char*) malloc (sizeof(char));
  
	for (i = 0; i < *size; i++) {
		// Hole cursor wo das original wort steht:
		unsigned int cursor = (*h[i]).cursor;
		
		//sscanf (file_memory+cursor, "%s", zeile);
    //printf ("value: %s\n", zeile); 

    sscanf(file_memory+cursor, "%[^\n]", zeile);
		
		
		
		
		// gehe in File in zu der Cursor position
		//fseek(in, cursor, SEEK_SET);
		// lese zeichen bis Zeilenende
		//fgets(original_word, 126, in);
	
		// schreibe OriginalZeile		
		result = fputs(zeile, out);
		fputc('\n', out);
		//fputc(':', out);
		//fputc(' ', out);

		// Hole jedes Histogram	als String
		//zeile = getHistogramAsString(h[i]);
		//for (n = 0; n < 126; n++) {
			//fputc(zeile[n], out);			
		//}
		//fputs(zeile, out);
		//fputc('\n', out);
	}

	fclose(out);		
	//fclose(in);
	free(zeile);
	
	munmap (file_memory, length);

	return 0;
}

int _writeFile(const char *filename_out, Histogram **h, unsigned int *size) {
	FILE *out;
	//FILE *in;

  // Bitte Pfad und Dateinamen anpassen
	out = fopen(filename_out, "w+t");
	//in = fopen(filename_in, "r");

   if(NULL == out) {
      printf("Konnte Datei %s nicht öffnen!\n", filename_out);
      return EXIT_FAILURE;
   }

	/*if (NULL == in) {
		printf("Konnte Datei %s nicht oeffnen!\n", filename_in);
		return EXIT_FAILURE;
	}*/

	//char* zeile;
	//char original_word[126];

	unsigned int i;
	//unsigned short n;
	//int result;

  char *histogram_string;
	for (i = 0; i < *size; i++) {
		// Hole cursor wo das original wort steht:
		//unsigned int cursor = (*h[i]).cursor;
		// gehe in File in zu der Cursor position
		//fseek(in, cursor, SEEK_SET);
		// lese zeichen bis Zeilenende
		//fgets(original_word, 126, in);
	
	  histogram_string = getHistogramAsString(h[i]);
	
		// schreibe OriginalZeile		
		fputs(histogram_string, out);
		fputc('\n', out);
		
		//fputc(':', out);
		//fputc(' ', out);

		// Hole jedes Histogram	als String
		//zeile = getHistogramAsString(h[i]);
		//for (n = 0; n < 126; n++) {
			//fputc(zeile[n], out);			
		//}
		//fputs(zeile, out);
		//fputc('\n', out);
	}

	fclose(out);		
	//fclose(in);

	return 0;
}

void mmapTry(const char* filename) {

    FILE *datei;
   // Bitte Pfad und Dateinamen anpassen
   datei = fopen(filename, "r");
   
  char* file_memory; 
  int fd;
  
  fseek(datei, 0L, SEEK_END); // Setze den Cursor ans Ende der Datei
	//printf("Ende in Datei: %d \n",ftell(datei));
	
	unsigned int length = ftell(datei);
	fclose(datei);
	
	fd = open (filename, O_RDWR, S_IRUSR | S_IWUSR); 
  
  file_memory = mmap (0, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); 
  close (fd);
  
  char *zeile = (char*) malloc (sizeof(char)*126); 
  int integer;
  //sscanf (file_memory+4, "%d", &integer); 
  //printf ("value: %d\n", integer); 
  //sscanf (file_memory+0, "%s", zeile);
  sscanf(file_memory+9, "%[^\n]", zeile);
  printf ("value: %s\n", zeile); 
  /*
  char *c = (char*) malloc (sizeof(char));
  int index = 0;
  
  do {
    // Lese Zeichen bis \n erkannt wurde
    sscanf(file_memory+index, "%c", c);
    if (*c != '\n') {
      printf("%c",*c);
    }
    index++;
  } while (*c != '\n');*/
  
  //printf("\n%c\n",file_memory[1]);
  
  munmap (file_memory, length);
}
