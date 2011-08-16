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

Histogram** initHistogramArray(Histogram *data, unsigned int *size) {
  // Erstelle ein Array das nur die Adressen auf die Elemente im Histogramm speichert
	Histogram **mixed = (Histogram**) malloc (sizeof(Histogram*)*(*size));

	unsigned int n;
	for (n = 0; n < (*size); n++) { // kopiere die Adressen in das neue Array.	
		mixed[n] = &data[n];
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
	
	Histogram *data = NULL;
	unsigned int size_data = 0;

  
  // time variables
	double startTime, endTime, timeUsed;
	
	if (myRank == 0) {
	  startTime = MPI_Wtime(); // set start time
  }
	
  const char* filename = "sortMe.txt";
	// Lese Datei und bekomme das die Histogramme zurück.
	data = readFile(filename, myRank, ranks, data, &size_data);

  if (myRank == 0) {
    endTime = MPI_Wtime();
	  timeUsed = endTime - startTime;
    printf("time used to read file: %s = %lf \n", filename, timeUsed);
  }
  
  Histogram **ref_data = initHistogramArray(data, &size_data);
  
	// Hier haben wir nun das Histogram dieses Prozesses.

  if (myRank == 0) {
    startTime = MPI_Wtime();
  }

	// Das Histogram soll nun sortiert werden.
	ref_data = sort(ref_data, &size_data); // FEHLER IN SORT: Informationen gehen verloren
  
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
    activeNodes[i] = i;
  }

  // Anzahl der Elemente
  short int activeNodes_size = ranks;

  //Histogram **sorted_merged_Histogram = ref_data;

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
        
        // Empfange Datan, data wird entprechend erweitert
        data = receiveHistogram(successor, &size_received, data, size_data); 

				// Referenz auf die empfangenen Daten, für die sortierung.
        Histogram **ref_received_data = initHistogramArray((data+size_data), &size_received); 

        // Neuinitalisierung der lokalen referenzen falls diese ungültig geworden sind
        ref_data = initHistogramArray(data, &size_data);

				// In diesem Speicherbereich kommen die Referenzen beider 
        Histogram **sorted = (Histogram**) malloc (sizeof(Histogram*)*(size_data+size_received));

        // do merge
    	  sorted = merge(ref_data, &size_data, ref_received_data, &size_received,  sorted);
    	  
    	  // ref_data zeigt nun auf die sortierten referenzen
    	  ref_data = sorted;
    	  size_data += size_received; // merke die neue anzahl der elemente
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
      sendHistogram(predecessor, ref_data, size_data);

			free(ref_data); // Array mit Pointern auf Histogramme
			free(data); // Original

      // DONE!
      MPI_Finalize();
      return EXIT_SUCCESS;
	  }
    
  }
  
  
  
  if (myRank == 0) {
    printf("All done! \n");
  
    endTime = MPI_Wtime();
	  timeUsed = endTime - startTime;	

    printf("%d elements in array!\n",size_data);
    printf("time used = %lf \n", timeUsed);
  }

	free(ref_data); // Array mit Pointern auf Histogramme
	free(data); // Original

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

