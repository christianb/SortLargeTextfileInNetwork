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
	
	Histogram *h = NULL;
	int size = 0;


  // time variables
	double startTime, endTime, timeUsed;
	startTime = MPI_Wtime(); // set start time
	
  const char* filename = "Euler.txt";
	// Lese Datei und bekomme das die Histogramme zurück.
	h = readFile(filename, myRank, ranks, h, &size);
  
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
	//h = sort(h, &size); // FEHLER IN SORT: Informationen gehen verloren
  printf("h[7].cursor = %d\n", h[7].cursor);
  
  endTime = MPI_Wtime();
	timeUsed = endTime - startTime;
  printf("time used to sort file: %s = %lf \n", filename, timeUsed);

  // h ist nun sortiert.
  // also sollte der inhalt von h mal asugegeben werden
  printHistogramArray(h, size);

	MPI_Finalize();

	return EXIT_SUCCESS;
}
