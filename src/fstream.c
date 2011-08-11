#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#include "Histogram.h"
#include "File_IO.h"
#include "Terminal_IO.h"
#include "Sort.h"
#include "Communication.h"

int getIndexOfNode(int node, short int *activeNodes, short int size);
int getSuccessorOfNode(int pNr, short int *activeNodes, short int size);
int getPredecessorOfNode(int pNr, short int *activeNodes, short int size);
short int* deleteOddProcessNumber(short int *activeNodes, short int *activeNodes_size);

Histogram** initHistogramArray(Histogram *master, unsigned int *size) {
  // Erstelle ein Array das nur die Adressen auf die Elemente im Histogramm speichert
	Histogram **mixed = (Histogram**) malloc (sizeof(Histogram*)*(*size));

	unsigned int n;
	for (n = 0; n < (*size); n++) { // kopiere die Adressen in das neue Array.	
		mixed[n] = &master[n];
	}
	
	return mixed;
}

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
	unsigned int size_local = 0;

  
  // time variables
	double startTime, endTime, timeUsed;
	
	if (myRank == 0) {
	  startTime = MPI_Wtime(); // set start time
  }
	
  const char* filename = "sortMe.txt";
	// Lese Datei und bekomme das die Histogramme zurück.
	//master = readFile(filename, myRank, ranks, master, &size_local);

  //Histogram **mixed_local = initHistogramArray(master, &size_local);

  

	// Hier haben wir nun das Histogram dieses Prozesses.

  if (myRank == 0) {
    startTime = MPI_Wtime();
  }

	// Das Histogram soll nun sortiert werden.
	//mixed_local = sort(mixed_local, &size_local); // FEHLER IN SORT: Informationen gehen verloren
  
  if (myRank == 0) {
    endTime = MPI_Wtime();
	  timeUsed = endTime - startTime;
    printf("time used to sort file: %s = %lf \n", filename, timeUsed);
  }

  // h ist nun sortiert.
  // also sollte der inhalt von h mal asugegeben werden
  //printHistogramArray(mixed, size);
	//writeFile("out.txt",filename, mixed, &size);

  /* Hier beginnt der MPI Spaß */
  // Jeder Knoten hat seine Elemente die er aus dem File gelesen hat sortiert und kann diese nun an andere Knoten senden.
  short int *activeNodes = (short int*) malloc (sizeof(short int)*ranks);
  int i;
  for (i = 0; i < ranks; i++) {
    activeNodes[i] = 0;
  }

  // Anzahl der Elemente
  short int activeNodes_size = ranks;

  //Histogram **sorted_merged_Histogram = mixed_local;

  // solange wie mehr als 1 Element im activeNodes Array vorhanden ist
  while (activeNodes_size > 1) {
    // jetzt müssen wir bestimmen ob dieser Prozess empfangen oder senden soll?
	  // suche diese Prozessnummer in den activeNodes und gib den Index zurück
    int indexOfThisNodeInArray = getIndexOfNode(myRank, activeNodes, activeNodes_size);
    
    // wenn Index gerade dann empfange vom Nachfolger
	  if (indexOfThisNodeInArray % 2 == 0) {
	    // Jetzt müssen wir raus finden wer der Nachfolger ist
      int successor = getSuccessorOfNode(myRank, activeNodes, activeNodes_size);
      
      if (successor != -1) { // wenn es einen nachfolger gibt empfange und merge
        printf("Prozess: %d empfaengt von Prozess: %d \n", myRank, successor);
        unsigned int size_received; // Speichert die Anzahl der Elemente im Histogram
        /*receiveHistogram(successor, &size_received, master, size_local); 

        Histogram **mixed_received = initHistogramArray((master+size_local), &size_received);

        Histogram **sorted_local_received = (Histogram**) malloc (sizeof(Histogram*)*(size_local+size_received));

        // do merge
    	  sorted_merged_Histogram = merge(mixed_local, &size_local, mixed_received, &size_received,  sorted_local_received);
    	  
    	  free(mixed_local);
    	  free(mixed_received);
    	  
    	  mixed_local = sorted_local_received;
    	  size_local += size_received;*/
      }
      
      // in jedem fall lösche knoten, die gesendet haben aus liste
      // Lösche alle nodes aus liste mit ungeradem index
      short int *rest_of_activeNodes = deleteOddProcessNumber(activeNodes, &activeNodes_size);
      free(activeNodes);
      activeNodes = rest_of_activeNodes; // Hier stehen nun die restlichen activen Knoten (Prozesse) drinn.
	  }
	  
	  // wenn Index ungerade dann sende an Vorgänger
	  if (indexOfThisNodeInArray % 2 != 0) {
	     // sende an Vorgänger
      int predecessor = getPredecessorOfNode(myRank, activeNodes, activeNodes_size);
      //printf("Prozess: %d sendet an Prozess: %d", myRank, predecessor);
      //sendHistogram(predecessor, sorted_merged_Histogram, size_local);

      // DONE!
      MPI_Finalize();
      return EXIT_SUCCESS;
	  }
    
  }
  
  
  /*
  if (myRank == 0) {
    printf("All done! \n");
  
    endTime = MPI_Wtime();
	  timeUsed = endTime - startTime;	

    printf("%d elements in array!\n",size_local);
    printf("time used = %lf \n", timeUsed);
  }*/

	//free(mixed_local); // Array mit Pointern auf Histogramme
	//free(master); // Original

	MPI_Finalize();

	return EXIT_SUCCESS;
}

short int* deleteOddProcessNumber(short int *activeNodes, short int *activeNodes_size) {
  int i;
  short int *rest_of_activeNodes = (short int*) malloc (sizeof(short int));
  int size = 0;
  for (i = 0; i < *activeNodes_size; i++) {
    if (i % 2 == 0) {
      // kopiere Elemente an geradem Index in neues Array;
      size++;
      rest_of_activeNodes = realloc(rest_of_activeNodes, sizeof(short int)*size);
      int index_activeNodes = size-1;
      rest_of_activeNodes[index_activeNodes] = activeNodes[i];
    }
  }
  *activeNodes_size = size;
  return rest_of_activeNodes;
}

/**
 * Get the index of the given node in vector.
 * @param node
 * @param activeNodes
 * @return int The Index of the node in the vector.
 */
int getIndexOfNode(int pNr, short int *activeNodes, short int size) {
    int index;
    for (index = 0; index < size ; index++) {
        if (pNr == activeNodes[index]) { // Wenn die gesuchte Prozessnummer gefunden wurde
            return index; // Gebe den Index, an welcher dieser Prozess im Array steht.
        }
    }
    return -1;
} 

/**
 * Get the successor of the given node from vector.
 * @param node
 * @param activeNodes A Pointer of
 * @return unsigned char A pointer to the successor node.
 */
int getSuccessorOfNode(int pNr, short int *activeNodes, short int size) {
    int i;
    for (i = 0; i < size; i++) {
        if (pNr == activeNodes[i]) {
            // get Successor
            if (i == size-1) { // wenn i am letzten index
                // dann gibt es keinen nachfolger
                return -1;
            }
            return activeNodes[i+1];
        }
    }
    return -1;
}

/**
 * Get the Predecessor of the given node from vector.
 * @param node
 * @param activeNodes A Pointer of
 * @return unsigned char A pointer to the Predecessor node.
 */
int getPredecessorOfNode(int pNr, short int *activeNodes, short int size) {
    int i;
    for (i = 0; i < size; i++) {
        if (pNr == activeNodes[i]) {
            // get Predecessor
            return activeNodes[i-1];
        }
    }
    
    return -1;
}

