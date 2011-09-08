#include "Communication.h"

Histogram* referenzToRealData(Histogram** data, int size_data, int total_size) {
  if (total_size < size_data) {
    total_size = size_data;
  }
  
  Histogram *new_memory = (Histogram*) malloc (total_size * sizeof(Histogram));
  int i;
  for (i = 0; i < size_data; i++) {
    memcpy(new_memory+i, data[i], sizeof(Histogram));
    //new_memory[i].cursor = (*data[i]).cursor;
    //new_memory[]
  }
  
 
  return new_memory;
}

/** Sende die Histogramme an einen anderen Prozess.
 * @param node Prozess an den die Histogramme gesendet werden sollen.
 * @param data Ein Pointer auf die Histogramme die gesendet werden sollen.
 * @param size Die Anzahl der Histogramme die gesendet werden sollen.
 */
void sendHistogram(int node, Histogram **data, int size, MPI_Datatype *HISTOGRAM_TYPE) {
	
	// kopiere die Inhalte hinter den referenzen in einen neuen Speicherbereich.
	Histogram *new_memory = referenzToRealData(data, size, size);
	
	/*
	int k;
    for (k = 0; k < size; k++) {
      printf("%d: cursor->%d\n", k, new_memory[k].cursor);
    }
    */
	
	//Histogram *new_memory = data[0];
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
  
  MPI_Send(new_memory, size, *HISTOGRAM_TYPE, node, 0, MPI_COMM_WORLD);
  free(new_memory);
  
  //MPI_Type_free(&HISTOGRAM_TYPE);
  
}

/**
 * Empfängt die Histogramme von einem anderen Prozess.
 * @param node Der Prozess von dem die Histogramme empfangen werden.
 * @param size Referenz auf die Anzahl der Elemente.
 */
Histogram* receiveHistogram(int node, unsigned int *size_received, Histogram *data, unsigned int size_data, MPI_Datatype *HISTOGRAM_TYPE, Histogram** ref_data_sorted) {
	MPI_Status status;

	// Empfange die Anzahl der Histogramme
	//*size_received = 0;
	MPI_Recv (size_received,1,MPI_INT,node,0,MPI_COMM_WORLD,&status);
	
	// allokiere Speicher für die Histogramme
	//Histogram *data = calloc(*size, sizeof(Histogram));
	// Vergrößern den alten Speicherbereich.
	unsigned int max_size = size_data + (*size_received);
	//Histogram * data_received = malloc (sizeof(Histogram) * (*size_received) );
	//data = (Histogram*) realloc (data, sizeof(Histogram) * (size_data + *size_received));     
	
	// statt auf den unsortierten Daten ein realloc zu machen machen wir ein malloc und kopieren die Daten hinter den REferenzen dort hinein
	// um anschließend an das ende die empfangenen Daten zu kopieren
	Histogram *new_memory = referenzToRealData(ref_data_sorted, size_data, max_size);
	free(data); // Data wird jetzt frei gegeben
	data = NULL;
  
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
  MPI_Recv(new_memory+size_data, *size_received, *HISTOGRAM_TYPE, node, 0, MPI_COMM_WORLD, &status);
  /*
  Histogram *ref_data = data+size_data;
  int k;
    for (k = 0; k < (*size_received); k++) {
      printf("%d: cursor->%d\n", k, ref_data[k].cursor);
    }
  */
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
  
  return new_memory;
	
}

