#include "Communication.h"

/** Sende die Histogramme an einen anderen Prozess.
 * @param node Prozess an den die Histogramme gesendet werden sollen.
 * @param data Ein Pointer auf die Histogramme die gesendet werden sollen.
 * @param size Die Anzahl der Histogramme die gesendet werden sollen.
 */
void sendHistogram(int node, Histogram **data, int size) {
	
	// Sende die Anzahl der Histogramme
	MPI_Send (&size,1,MPI_INT,node,0,MPI_COMM_WORLD);
	
	/*int i;
	for (i = 0; i < size; i++) {
		// Sende das Histogram
		
		// Zuerst das Array mit den 52 Buchstaben
		MPI_Send((*data[i]).letter, 52,MPI_UNSIGNED_CHAR,node,0,MPI_COMM_WORLD);

		// Dann die Cursor position
		MPI_Send(&((*data[i]).cursor), 1,MPI_INT,node,0,MPI_COMM_WORLD);
	}*/
	
	//Histogram     h[NELEM], h_received[NELEM];
  MPI_Datatype HISTOGRAM_TYPE, oldtypes[2]; 
  int          blockcounts[2];

  // MPI_Aint type used to be consistent with syntax of
  // MPI_Type_extent routine 
  MPI_Aint    offsets[2], extent;

  //MPI_Status stat;

  //MPI_Init(&argc,&argv);
  //MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  //MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
   
  // Setup description of the 4 MPI_FLOAT fields x, y, z, velocity
  offsets[0] = 0;
  oldtypes[0] = MPI_UNSIGNED_CHAR;
  blockcounts[0] = 52;

  // Setup description of the 2 MPI_INT fields n, type 
  // Need to first figure offset by getting size of MPI_FLOAT 
  MPI_Type_extent(MPI_UNSIGNED_CHAR, &extent);
  offsets[1] = 52 * extent;
  oldtypes[1] = MPI_INT;
  blockcounts[1] = 4;

  // Now define structured type and commit it
  MPI_Type_struct(2, blockcounts, offsets, oldtypes, &HISTOGRAM_TYPE);
  MPI_Type_commit(&HISTOGRAM_TYPE);
  
  MPI_Send(*data, size, HISTOGRAM_TYPE, node, 0, MPI_COMM_WORLD);
  
  MPI_Type_free(&HISTOGRAM_TYPE);
}

/**
 * Empfängt die Histogramme von einem anderen Prozess.
 * @param node Der Prozess von dem die Histogramme empfangen werden.
 * @param size Referenz auf die Anzahl der Elemente.
 */
Histogram* receiveHistogram(int node, unsigned int *size_received, Histogram *data, unsigned int size_data) {
	MPI_Status status;

	// Empfange die Anzahl der Histogramme
	//*size_received = 0;
	MPI_Recv (size_received,1,MPI_INT,node,0,MPI_COMM_WORLD,&status);
	
	// allokiere Speicher für die Histogramme
	//Histogram *data = calloc(*size, sizeof(Histogram));
	// Vergrößern den alten Speicherbereich.
	data = (Histogram*) realloc(data, sizeof(Histogram)*(size_data+(*size_received)));
	
	/*
	if (data !=NULL) {

	  unsigned int i;
	  for(i = size_data; i < (size_data+(*size_received)); i++) { // Die ursprünglichen Elemente werden nicht übverschrieben. Die neuen Elemente werden am Ende angehängt.
		  // Empfange die Histogramme
		  // Empfange die Buchstaben
		  MPI_Recv (&(data[i].letter),52,MPI_UNSIGNED_CHAR,node,0,MPI_COMM_WORLD,&status);
		
		  // Empfange die Cursor Position
		  MPI_Recv (&(data[i].cursor),1,MPI_INT,node,0,MPI_COMM_WORLD,&status); // TODO: gibt es MPI_UNSIGNED_INT???
	  }
	
	  return data;
	
	} else {
       free (data);
       printf ("Error (re)allocating memory");
       exit (1);
  }*/
  
  //Histogram     h[NELEM], h_received[NELEM];
  MPI_Datatype HISTOGRAM_TYPE, oldtypes[2]; 
  int          blockcounts[2];

  // MPI_Aint type used to be consistent with syntax of
  // MPI_Type_extent routine 
  MPI_Aint    offsets[2], extent;

 // MPI_Status stat;

   
  // Setup description of the 4 MPI_FLOAT fields x, y, z, velocity
  offsets[0] = 0;
  oldtypes[0] = MPI_UNSIGNED_CHAR;
  blockcounts[0] = 52;

  // Setup description of the 2 MPI_INT fields n, type 
  // Need to first figure offset by getting size of MPI_FLOAT 
  MPI_Type_extent(MPI_UNSIGNED_CHAR, &extent);
  offsets[1] = 52 * extent;
  oldtypes[1] = MPI_INT;
  blockcounts[1] = 4;

  // Now define structured type and commit it
  MPI_Type_struct(2, blockcounts, offsets, oldtypes, &HISTOGRAM_TYPE);
  MPI_Type_commit(&HISTOGRAM_TYPE);
  
  MPI_Recv((data+size_data), *size_received, HISTOGRAM_TYPE, node, 0, MPI_COMM_WORLD, &status);
  
  MPI_Type_free(&HISTOGRAM_TYPE);
  
  return data;
	
}

