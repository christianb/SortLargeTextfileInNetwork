#include "Communication.h"

/** Sende die Histogramme an einen anderen Prozess.
 * @param node Prozess an den die Histogramme gesendet werden sollen.
 * @param data Ein Pointer auf die Histogramme die gesendet werden sollen.
 * @param size Die Anzahl der Histogramme die gesendet werden sollen.
 */
void sendHistogram(int node, Histogram **data, int size, MPI_Datatype *HISTOGRAM_TYPE) {
	
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
  
  MPI_Send(*data, size, *HISTOGRAM_TYPE, node, 0, MPI_COMM_WORLD);
  
  //MPI_Type_free(&HISTOGRAM_TYPE);
  
}

/**
 * Empfängt die Histogramme von einem anderen Prozess.
 * @param node Der Prozess von dem die Histogramme empfangen werden.
 * @param size Referenz auf die Anzahl der Elemente.
 */
Histogram* receiveHistogram(int node, unsigned int *size_received, Histogram *data, unsigned int size_data, MPI_Datatype *HISTOGRAM_TYPE) {
	MPI_Status status;

	// Empfange die Anzahl der Histogramme
	//*size_received = 0;
	MPI_Recv (size_received,1,MPI_INT,node,0,MPI_COMM_WORLD,&status);
	
	// allokiere Speicher für die Histogramme
	//Histogram *data = calloc(*size, sizeof(Histogram));
	// Vergrößern den alten Speicherbereich.
	unsigned int max_size = size_data + (*size_received);
	//Histogram * data_received = malloc (sizeof(Histogram) * (*size_received) );
	data = (Histogram*) realloc (data, sizeof(Histogram) * (size_data + *size_received));     
  
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
  
  //Histogram *_data = data;
  MPI_Recv(data+size_data, *size_received, *HISTOGRAM_TYPE, node, 0, MPI_COMM_WORLD, &status);
  /*
  data = (Histogram*) realloc (data, sizeof(Histogram) * (size_data + *size_received));     
        
  unsigned int i;
  unsigned int n;
  for (i = 0; i < *size_received; i++) {
    for (n = 0; n < 52; n++) {
      data[i+size_data].letter[n] = data_received[i].letter[n];
    }
    data[i+size_data].cursor = data_received[i].cursor;
  }*/
  //free(data_received);
  //MPI_Type_free(&HISTOGRAM_TYPE);
  
  return data;
	
}

