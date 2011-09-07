#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#include "Histogram.h"
#include "File_IO.h"
#include "Terminal_IO.h"
#include "Sort.h"
#include "Communication.h"

#define SORT_FILE "sortMe_100.txt"

int getIndexOfNode(int node, short int *activeNodes, short int size);
int getSuccessorOfNode(int pNr, short int *activeNodes, short int size);
int getPredecessorOfNode(int pNr, short int *activeNodes, short int size);
short int* deleteOddProcessNumber(short int *activeNodes, short int *activeNodes_size);

Histogram* copyElementsIntoCoherentMemory(Histogram** refData, unsigned int size);

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
  // 1) MPI initialisieren
	int myRank;
 	int ranks;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank); // get rank of this prozess
 	MPI_Comm_size(MPI_COMM_WORLD, &ranks); // get number of prozesses
 	
 	// 2) Datentyp Histogram MPI bekannt machen.
 	MPI_Datatype HISTOGRAM_TYPE, oldtypes[2]; 
  int blockcounts[2];
  // MPI_Aint type used to be consistent with syntax of
  // MPI_Type_extent routine 
  MPI_Aint    offsets[2], extent;
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
	
	// 3) Deklaration lokaler Variablen
	Histogram *unsortedLocalData = NULL; // Pointer auf die Histogramme im Speicher
	unsigned int sizeLocalData = 0; // Anzahl der Histogramme im Speicher
	double startTime = 0, endTime = 0, timeUsed = 0, totalTime = 0;	// time variables
	//const char* filename = "sortMe_100.txt"; // Zu sortierende Datei
	Histogram *sortedLocalData = NULL;
	
	if (myRank == 0) {
	  startTime = MPI_Wtime(); // set start time
	  totalTime = startTime;
  }

	// 4) Lese Datei + Erstelle Histogram
	unsortedLocalData = readFile(SORT_FILE, myRank, ranks, unsortedLocalData, &sizeLocalData); // size_local_data enthält nun die Anzahl der Histogramme

  if (myRank == 0) {
    endTime = MPI_Wtime();
	  timeUsed = endTime - startTime;
    printf("time used to read file: %s = %lf \n", SORT_FILE, timeUsed);
    startTime = MPI_Wtime();
  }

	// 5) Sortiere lokale Daten
	Histogram **ref_unsortedLocalData = initHistogramArray(unsortedLocalData, &sizeLocalData); // erzeuge eine Referenz auf alle Histogramme die noch unsortiert sind
	Histogram **ref_sortedLocalData = sort(ref_unsortedLocalData, &sizeLocalData); // sortiere die Referenzen auf die Daten
	sortedLocalData = copyElementsIntoCoherentMemory(ref_sortedLocalData, sizeLocalData); // kopiere Elemente hinter den Referenzen in neuen Speicher
	
	//free(ref_unsortedLocalData); // ref data auf alten unsortierten Speicher freigeben
	free(ref_sortedLocalData); // Speicher auf die Referenzen der sortierten Daten freigeben
	free(unsortedLocalData); // Speicher auf unsortierte Daten freigeben
	
	// nun steht in sortetLocalData die Sortierten Histogramme

  if (myRank == 0) {
    endTime = MPI_Wtime();
	  timeUsed = endTime - startTime;
    printf("time used to sort file local: %s = %lf \n", SORT_FILE, timeUsed);
    //printHistogramArray(ref_data, size_data);
    startTime = MPI_Wtime(); 
  }

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

        Histogram *local_and_received_data = receiveHistogram(successor, &size_received, &HISTOGRAM_TYPE, sortedLocalData, sizeLocalData);
        // beide sortierte Daten stehen in einem zusammenhängenden Speicherbereich.
        // Da die Anzahl der jeweiligen Histogramme bekannt ist, kann sicher darin navigiert werden.

				// Nun brauchen wir je ein Array mit den Referenzen auf die empfangenen Daten.
        Histogram **ref_received_sortedData = initHistogramArray(local_and_received_data+sizeLocalData, &size_received); // Der Pointer wird also hinter das letzte Element aus den lokalen Daten, also auf das erste Element der Empfangenen Daten gesetzt.
        
        // Nun brauchen wir noch ein Array mit den Referenzen auf die lokalen Daten.
        Histogram **ref_local_sortedData = initHistogramArray(local_and_received_data, &sizeLocalData);
        
				// In diesem Speicherbereich kommen die Referenzen beider 
        Histogram **sorted_references = (Histogram**) malloc (sizeof(Histogram*)*(sizeLocalData+size_received));

        // do merge
    	  sorted_references = merge(ref_local_sortedData, &sizeLocalData, ref_received_sortedData, &size_received, sorted_references);
    	  
    	  sizeLocalData += size_received; // merke die neue anzahl der elemente
    	  
    	  // kopiere die sortierten Referenzen (also die eigentlichen Inhalte) aus zwei verschiedenen Speicherbereichen in einen zusammenhängenden.
    	  Histogram* sortedData = copyElementsIntoCoherentMemory(sorted_references, sizeLocalData);
    	  
    	  
    	  // Gebe alte Speicherbereiche frei;
    	  free(local_and_received_data); 
    	  free(ref_received_sortedData);
    	  free(ref_local_sortedData); 
    	  free(sorted_references);
    	  
    	  sortedLocalData = sortedData; // referenz auf alle Sortierten Daten wird aktualisiert.
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
      printf("Prozess: %d sendet an Prozess: %d\n", myRank, predecessor);
      _sendHistogram(predecessor, sortedLocalData, sizeLocalData, &HISTOGRAM_TYPE);
      
      printf("Prozess: %d hat gesendet\n",myRank);

			free(sortedLocalData);
  
      printf("MPI_Finalize() Prozess: %d \n", myRank);
      
      // DONE!
      MPI_Finalize();
      return EXIT_SUCCESS;
      
      
	  }
    
  }
  
  
  
  if (myRank == 0) {
    printf("All done! \n");
    
    
    
    // Gebe die Zeile 545146 aus!
    //printLine(filename, ref_data, 100);
    
    // Schreibe in Datei
    //writeFile("sorted.txt", filename, ref_data, size_data);
  
    endTime = MPI_Wtime();
	  timeUsed = endTime - totalTime;	

    printf("%d elements in array!\n",sizeLocalData);
    printf("time used to sort everything = %lf \n", timeUsed);
    
    //free(sortedLocalData); // Gebe Histogramme frei
  }

	
  MPI_Type_free(&HISTOGRAM_TYPE);

	MPI_Finalize();
	return EXIT_SUCCESS;
}

/**
 * Die Sortierung erfolgt nur über die Pointer.
 * Um die Struktur jedoch an einen anderen Prozess zu senden müssen die Elemente auch korrekt im Speicher sortiert sein.
 * Darum werden die Originalen Elemente anhand ihrer sortierten Position neu geschrieben.
 */
Histogram* copyElementsIntoCoherentMemory(Histogram** refData, unsigned int size) {
   
   //unsigned int size_data_in_memory = 10;
   Histogram *sortedData = (Histogram*) malloc (sizeof(Histogram) * size);
   
   // Zu Begin haben wir speicher für 10 Histogramme angelegt
   
  // 1. Gehe alle Elemente durch
  int index = 0;
  for (index = 0; index < size; index++) {
    // 1.2 Kopiere das Elemente in den neuen Speicher
    memcpy(sortedData+index, refData[index], sizeof(Histogram));
  }
  
  return sortedData;

  
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

