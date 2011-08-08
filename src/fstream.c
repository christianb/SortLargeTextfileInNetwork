#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#include "Histogram.h"
#include "File_IO.h"
#include "Terminal_IO.h"
#include "Sort.h"

int main (int argc, char *argv[]) {
	int myRank;
 	int ranks;

	// init mpi
	MPI_Init(&argc, &argv);

	// get rank of this prozess
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	// get number of prozesses
 	MPI_Comm_size(MPI_COMM_WORLD, &ranks);
	
	Histogram *master = NULL;
	unsigned int size = 0;


  // time variables
	double startTime, endTime, timeUsed;
	startTime = MPI_Wtime(); // set start time
	
  const char* filename = "sortMe_100.txt";
	// Lese Datei und bekomme das die Histogramme zurück.
	master = readFile(filename, myRank, ranks, master, &size);

	// Erstelle ein Array das nur die Adressen auf die Elemente im Histogramm speichert
	Histogram **mixed = (Histogram**) malloc (sizeof(Histogram*)*size);

	unsigned int n;
	for (n = 0; n < size; n++) { // kopiere die Adressen in das neue Array.	
		mixed[n] = &master[n];
	}
  
  endTime = MPI_Wtime();
	timeUsed = endTime - startTime;	

  printf("%d elements in array!\n",size);
  printf("time used to read sortMe.txt = %lf \n", timeUsed);

	/*
	if (myRank == 0) {
		printHistogramArray(h, size);
	}
*/
	// Hier haben wir nun das Histogram dieses Prozesses.

  startTime = MPI_Wtime();

	// Das Histogram soll nun sortiert werden.
	mixed = sort(mixed, &size); // FEHLER IN SORT: Informationen gehen verloren
  //printf("h[7].cursor = %d\n", h[7].cursor);

  /************* TEST *********************/
  /*Histogram *h1 = (Histogram*) malloc (sizeof(Histogram)*4);
  h1[0].letter[0] = 1;
  h1[1].letter[0] = 2;
  h1[2].letter[0] = 3;
  h1[3].letter[0] = 4;

  int size_left = 4;
  int size_right = 1;
  Histogram *result = sort(h1, &size_left);
  printf ("result[0].letter[0] = %d\n", result[0].letter[0]);
  printf ("result[0].letter[0] = %d\n", result[1].letter[0]);
  printf ("result[0].letter[0] = %d\n", result[2].letter[0]);
  printf ("result[0].letter[0] = %d\n", result[3].letter[0]);*/
  /****************************************/

  /************ TEST MEMCPY ***************/
  /*Histogram *h1 = (Histogram*) malloc (sizeof(Histogram));
  h1[0].letter[0] = 45;

  Histogram *h2 = (Histogram*) malloc (sizeof(Histogram));
  memcpy(h2, h1, sizeof(Histogram));

  printf("h2[0].letter[0] = %d\n", h2[0].letter[0]);*/

  /****************************************/


  //printf ("Sort: DONE\n");
  endTime = MPI_Wtime();
	timeUsed = endTime - startTime;
  printf("time used to sort file: %s = %lf \n", filename, timeUsed);

  // h ist nun sortiert.
  // also sollte der inhalt von h mal asugegeben werden
  //printHistogramArray(mixed, size);
	writeFile("out.txt",filename, mixed, &size);

	//printf("h[100] = ");
	//printHistogramStruct(h, 100);

	free(mixed);

	MPI_Finalize();

	return EXIT_SUCCESS;
}
