#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#include "Histogram.h"
#include "File_IO.h"
#include "Terminal_IO.h"

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
	
	h = readFile(myRank, ranks, h, &size);
	
	if (myRank == 0) {
		printHistogramArray(h, size);
	}

	// Hier haben wir nun das Histogram dieses Prozesses.

	// Das Histogram soll nun sortiert werden.

	MPI_Finalize();

	return EXIT_SUCCESS;
}
