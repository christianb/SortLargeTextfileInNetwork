#include <cstdlib>
#include <iostream>
#include <mpi.h>
#include <fstream>
#include <iostream>
#include <list>
#include <string.h>
#include "file_util.hpp"

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

void sendHistogram(int node, list<unsigned char*> *data) {
	cout << "call sendHistogram(" << node << ")" << endl;
	int size = data->size();
	
	// First send the total number of elements in the vector
	MPI_Send (&size,1,MPI_INT,node,0,MPI_COMM_WORLD);
	
	cout << "send size = " << size << endl;
	
	int c = 1;
	for (list<unsigned char*>::iterator l_it = data->begin(); l_it != data->end(); l_it++) {		
		// send the int*
		MPI_Send(*l_it, 52,MPI_CHAR,node,0,MPI_COMM_WORLD);
		
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

list<unsigned char*>* receiveHistogram(int node) {
	cout << "call receiveHistogram(" << node << ")" << endl;
	MPI_Status status;
	// First receive the size of the vector
	int size = 0;
	MPI_Recv (&size,1,MPI_INT,node,0,MPI_COMM_WORLD,&status);
	
	cout << "received that " << size << " elements will come..." << endl;
	
	int c = 1;
	list<unsigned char*>* int_list = new list<unsigned char*>();
	for(int i = 0; i < size; i++) {
		unsigned char *value = (unsigned char*) malloc(sizeof(unsigned char) * 52);
		
		MPI_Recv (value,52,MPI_INT,node,0,MPI_COMM_WORLD,&status);
		
		//cout << "received int: " << *value << endl;
		int_list->push_back(value);
		
		if (c % 100000 == 0) {cout << c << endl;}
		c++;	
	}
	
	cout << "done receive." << endl;
	return int_list;
	
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

void printHistogram(unsigned char* histogram) {
	for (int i = 0 ; i < 26 ; i++) {
		int c = 'A' + i;
		cout << "[" << (char) c << "] = " <<  (int) histogram[i] << endl;
	}

	for (int i = 26; i < 52; i++) {
		int c = 'A' + i + 6;
		cout << "[" << (char) c << "] = " << (int) histogram[i] << endl;
	}
	
	cout << "line_number = " << histogram[52] << endl;
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

	// if this is process zero than this is the master node, controlling all others
	if (myrank == 0) {
		// time variables
		double startTime, endTime, timeUsed;
		startTime = MPI_Wtime(); // set start time
		
		FileUtil *futil = new FileUtil(myrank, size);
		
		//list<int*> *cursors = futil->readFilePositions();
		//list<char*> *words = futil->readAllPosition(cursors);
		list<unsigned char*> *histogram_list = futil->readFile();
	
		printHistogram(histogram_list->back());

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
	}

 	MPI_Finalize();

	
	return EXIT_SUCCESS;
}
