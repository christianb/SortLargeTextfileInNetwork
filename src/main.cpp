#include <cstdlib>
#include <iostream>
#include <mpi.h>
#include <fstream>
#include <iostream>
#include <list>
#include <string.h>
#include <vector>

#include "mergesort.hpp"

using namespace std;

void sendIntegerListToNode(int node, list<int*> *data) {
	cout << "call send int list" << endl;
	int size = data->size();
	// First send the total number of elements in the vector
	MPI_Send (&size,1,MPI_INT,node,0,MPI_COMM_WORLD);
	
	
	int c = 1;
	for (list<int*>::iterator l_it = data->begin(); l_it != data->end(); l_it++) {		
		// send the char*
		MPI_Send (*l_it, 1,MPI_INT,node,0,MPI_COMM_WORLD);
		
		if (c % 100000 == 0) {cout << c << endl;}
		c++;
	}
}

void sendHistogram(int node, list<_histogram_data*> *data) {
	int size = data->size();
	
	// First send the total number of elements in the vector
	MPI_Send (&size,1,MPI_INT,node,0,MPI_COMM_WORLD);
	
	cout << "send size = " << size << endl;
	
	int c = 1;
	for (list<_histogram_data*>::iterator l_it = data->begin(); l_it != data->end(); l_it++) {		
		// send the int*
		_histogram_data* _element = *l_it;
		unsigned char* _array = _element->array;
		MPI_Send(_array, 52,MPI_CHAR,node,0,MPI_COMM_WORLD);
		
		int _cursor = _element->cursor;
		MPI_Send(&_cursor, 1,MPI_INT,node,0,MPI_COMM_WORLD);
		
		if (c % 100000 == 0) {cout << c << endl;}
		c++;
	}
	
	cout << "done send." << endl;
}

void sendListToNode(int node, list<char*> *data) {
	cout << "call sendListToNode(" << node << ")" << endl;
	int size = data->size();
	
	cout << "send size of list: " << size << endl;
	// First send the total number of elements in the vector
	MPI_Send (&size,1,MPI_INT,node,0,MPI_COMM_WORLD);
	
	int c = 1;
	for (list<char*>::iterator l_it = data->begin(); l_it != data->end(); l_it++) {
		char *buffer = *l_it;
		// send the size of the data
		int length = strlen(buffer)+1;
		// send the length of the string
		MPI_Send (&length,1,MPI_INT,node,0,MPI_COMM_WORLD);
		
		// send the char*
		MPI_Send (buffer,strlen(buffer)+1,MPI_CHAR,node,0,MPI_COMM_WORLD);
		
		if (c % 100000 == 0) {cout << c << endl;}
		c++;
	} 
	
	cout << "send done!" << endl;
	
}

list<_histogram_data*>* receiveHistogram(int node) {
	cout << "call receiveHistogram(" << node << ")" << endl;
	MPI_Status status;
	// First receive the size of the vector
	int size = 0;
	MPI_Recv (&size,1,MPI_INT,node,0,MPI_COMM_WORLD,&status);
	
	cout << "received that " << size << " elements will come..." << endl;
	
	int c = 1;
	list<_histogram_data*>* histogram_list = new list<_histogram_data*>();
	for(int i = 0; i < size; i++) {
		unsigned char *value = (unsigned char*) malloc(sizeof(unsigned char) * 52);
		
		MPI_Recv (value,52,MPI_CHAR,node,0,MPI_COMM_WORLD,&status);
		
		int cursor;
		MPI_Recv (&cursor,1,MPI_INT,node,0,MPI_COMM_WORLD,&status);
		
		_histogram_data* _new_histogram = (_histogram_data*) malloc (sizeof(_histogram_data*));
		_new_histogram->array = value;
		_new_histogram->cursor = cursor;
		
		//cout << "received int: " << *value << endl;
		histogram_list->push_back(_new_histogram);
		
		if (c % 100000 == 0) {cout << c << endl;}
		c++;	
	}
	
	cout << "done receive." << endl;
	return histogram_list;
	
}

list<int*>* receiveIntegerListFromNode(int node) {
	cout << "call receiveIntegerListFromNode(" << node << ")" << endl;
	MPI_Status status;
	// First receive the size of the vector
	int size = 0;
	MPI_Recv (&size,1,MPI_INT,node,0,MPI_COMM_WORLD,&status);
	cout << "receive length: " << size << endl;
	
	
	int c = 1;
	list<int*>* int_list = new list<int*>();
	for(int i = 0; i < size; i++) {
		int *value = (int*) malloc(sizeof(int)); 
		MPI_Recv (value,1,MPI_INT,node,0,MPI_COMM_WORLD,&status);
		//cout << "received int: " << *value << endl;
		int_list->push_back(value);
		
		if (c % 100000 == 0) {cout << c << endl;}
		c++;	
	}
}

list<char*>* receiveListFromNode(int node) {
	cout << "call receiveListFromNode(" << node << ")" << endl;
	MPI_Status status;
	// First receive the size of the vector
	int size = 0;
	MPI_Recv (&size,1,MPI_INT,node,0,MPI_COMM_WORLD,&status);
	
	cout << "received List size: " << size << endl;
	
	list<char*> *data = new list<char*>();
	
	int c = 1;
	for(int i = 0; i < size; i++) {
		// get the length of the char*
		int length = 0;
		MPI_Recv (&length,1,MPI_INT,node,0,MPI_COMM_WORLD,&status);
		//if (c  >= 900000) { cout << "receive length: " << length << endl; }
		
		char *buffer = (char *)malloc(sizeof(char) * length);
		//if (c  >= 900000) { cout << "malloc()" << endl; }
		MPI_Recv (buffer,length,MPI_CHAR,node,0,MPI_COMM_WORLD,&status);
		//if (c  >= 900000) { cout << "l->pushback()" << endl; }
		data->push_back(buffer);
		
		if (c % 100000 == 0) {cout << c << endl;}
		c++;	
	}
	
	cout << "receive done!" << endl;
	
	return data;
}

void printHistogram(_histogram_data* _histogram) {
	unsigned char* histogram = _histogram->array;
	for (int i = 0 ; i < 26 ; i++) {
		int c = 'A' + i;
		cout << "[" << (char) c << "] = " <<  (int) histogram[i] << endl;
	}

	for (int i = 26; i < 52; i++) {
		int c = 'A' + i + 6;
		cout << "[" << (char) c << "] = " << (int) histogram[i] << endl;
	}
	
	cout << "cursor position = " << _histogram->cursor << endl << endl;
 }

/**
 * Get the index of the given node in vector.
 * @param node
 * @param activeNodes
 * @return int The Index of the node in the vector.
 */
int getIndexOfNode(int node, vector<int> * activeNodes) {
    for (int i = 0; i < activeNodes->size() ; i++) {
        int element = activeNodes->at(i);
        if (node == element) {
            return i;
        }
    }
} 

/**
 * Get the Predecessor of the given node from vector.
 * @param node
 * @param activeNodes A Pointer of
 * @return unsigned char A pointer to the Predecessor node.
 */
int getPredecessorOfNode(int node, vector<int> * activeNodes) {
    for (int i = 0; i < activeNodes->size(); i++) {
        int element = activeNodes->at(i);
        if (node == element) {
            // get Predecessor
            return activeNodes->at(i-1);
        }
    }
}

/**
 * Get the successor of the given node from vector.
 * @param node
 * @param activeNodes A Pointer of
 * @return unsigned char A pointer to the successor node.
 */
int getSuccessorOfNode(int node, vector<int> * activeNodes) {
    for (int i = 0; i < activeNodes->size(); i++) {
        int element = activeNodes->at(i);
        if (node == element) {
            // get Successor
            if (i == activeNodes->size()-1) { // wenn i am letzten index
                // dann gibt es keinen nachfolger
                return -1;
            }
            
            return activeNodes->at(i+1);
            
        }
    }
}

vector<int>* deleteAllOddNodes(vector<int> * activeNodes) {
    vector<int> *other_nodes = new vector<int>();
    
    for (int i = 0; i < activeNodes->size(); i++) {    
        if (i % 2 == 0){
            other_nodes->push_back(activeNodes->at(i));
        }
    }
    
    delete activeNodes;
    return other_nodes;
}

void printHistogramAsString(list<_histogram_data*> *histogram) {
	for (list<_histogram_data*>::iterator it = histogram->begin(); it != histogram->end(); it++) {
		_histogram_data* _element = *it;
		unsigned char* element = _element->array;
		for (int i = 0 ; i < 26; i++) {
			int numberOfLettersUpperCase = element[i];
			// print n times upper case letter
			for (int n = 0; n < numberOfLettersUpperCase ; n++) {
				cout << (char) (i+65);
			}
			
			int numberOfLettersLowerCase = element[i+26];
			// print n times lower case letter
			for (int n = 0; n < numberOfLettersLowerCase ; n++) {
				cout << (char) (i+97);
			}
		}
		cout << endl;
		
	}
}

int main (int argc, char *argv[]) {
	int myrank;
 	int size;

	// init mpi
	MPI_Init(&argc, &argv);

	// get rank of this prozess
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	// get number of prozesses
 	MPI_Comm_size(MPI_COMM_WORLD, &size);
 	
    vector<int> *activeNodes = new vector<int>();
    
    // init list with active nodes
    for (int i = 0; i < size; i++){
        activeNodes->push_back(i);
    }
    
    FileUtil *futil = new FileUtil(myrank, size);
    // read content from file
    list<_histogram_data*> *histogram_list = futil->readFile();
	
	//printHistogram(histogram_list->front());

	Mergesort *mergesort = new Mergesort(); 
    	list<_histogram_data*> *sorted_Histogram = mergesort->sort(histogram_list);
	//cout << "sorted list size = " << sorted_Histogram->size() << endl;
	//printHistogramAsString(sorted_Histogram);
	
    	//list<_histogram_data*> *sorted_Histogram = histogram_list;
    

    /** Am Anfang hat jeder Knoten seine Daten eingelesen und einmal sortiert. **/
    
    // solange wie mehr als 1 Element im Vector ist tue...
    while (activeNodes->size() > 1) {
        // jetzt müssen wir bestimmen ob dieser Prozess empfangen oder senden soll?
	    // suche diese Prozessnummer in den activeNodes und gib den Index zurück
        int indexOfNode = getIndexOfNode(myrank, activeNodes);
        
        // wenn Index gerade dann empfange vom Nachfolger
	    if (indexOfNode % 2 == 0) {
	        // empfange vom Nachfolger
            int successor = getSuccessorOfNode(myrank, activeNodes);
            
            if (successor != -1) { // wenn es einen nachfolger gibt emofange und merge
				cout << "node: " << myrank << " empfange von node:" << successor << endl;
                list<_histogram_data*> *received_Histogram = receiveHistogram(successor);

    	        // do merge
    	        sorted_Histogram = mergesort->merge(sorted_Histogram, received_Histogram);
            }
            
            // in jedem fall lösche knoten, die gesendet haben aus liste
	        
	        // Lösche alle nodes aus liste mit ungeradem index
            //cout << "size of activeNodes = " << activeNodes->size();
            activeNodes = deleteAllOddNodes(activeNodes);
            //cout << "deleteAllOddNodes(), size is = " << activeNodes->size();
            
	    } else {  // wenn Index ungerade dann empfange vom Vorgänger 
	        // sende an Vorgänger
            int predecessor = getPredecessorOfNode(myrank, activeNodes);
            
			sendHistogram(predecessor, sorted_Histogram);
            cout << "node: " << myrank << " sende an node:" << predecessor << endl;
            
            // DONE!
            MPI_Finalize();
        	return EXIT_SUCCESS;
	    }
    }
    
    cout << myrank << " Nur noch ein Element in Liste! FERTIG" << endl;
	printHistogramAsString(sorted_Histogram);    
	cout << sorted_Histogram->size() << "words are sorted." << endl;
    // hier ist jetzt nur noch ein Element in der Liste
    // nun liegt die sortierte List vor...
    
    // bestimmte welche wörter wo in der sortierten liste stehen und schreibe die wörter (sortiert) in datei.
    
    
    /*

	// if this is process zero than this is the master node, controlling all others
	if (myrank == 0) {
		// time variables
		double startTime, endTime, timeUsed;
		startTime = MPI_Wtime(); // set start time
		
		
		
		//list<int*> *cursors = futil->readFilePositions();
		//list<char*> *words = futil->readAllPosition(cursors);
		
		
		// do mergesort
		// list<unsigned char*> *merge(histogram_list)
	
	    
	    
	
	    // just for debugging
		//printHistogram(histogram_list->back());

		endTime = MPI_Wtime();
		timeUsed = endTime - startTime;
		cout << "time used to read file:" << timeUsed << endl;
		startTime = MPI_Wtime();

		//cout << "histogramme in list: " << histogram_list->size() << endl;

		// Zeit messen wie lange das einlesen der Datei benötigt.
		
		// Anschließend an Knoten 1 das Array senden
    	//sendListToNode(1, words);
		//sendIntegerListToNode(1, int_list);
		sendHistogram(1, histogram_list);
		endTime = MPI_Wtime();
		timeUsed = endTime - startTime;
		
		cout << "time used to send file:" << timeUsed << endl;
	} else {
		if (myrank == 1) {
		// warten auf anazhl der wörter
		// SChleife jedes wort einlesen
		// wie lange ist das wort
		// lese zeichen ein
		//receiveIntegerListFromNode(0);
		//receiveListFromNode(0);
			list<unsigned char*> *histogram = receiveHistogram(0);
		
			printHistogram(histogram->back());
		}
	}*/

 	MPI_Finalize();

	
	return EXIT_SUCCESS;
}
