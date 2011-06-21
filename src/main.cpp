#include <cstdlib>
#include <iostream>
#include <mpi.h>
#include <fstream>
#include <iostream>
#include <list>

using namespace std;

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
		double startTime, endTime, timeUsed;
		startTime = MPI_Wtime();
		
		// Datei einlesen
		ifstream is;
		string name = "./sortMe.txt";
		
		is.open(name.c_str(), ifstream::in);
		
		// if file not exist
		if (!is) {
			cout << "File not found" << endl;
			return EXIT_SUCCESS;
		}
		
		is.seekg(0, ios::beg);
		
		int c = 0;
		list<char*>* l = new list<char*>();
		cout << "size: " << l->size() << endl;
		while (!is.eof()) {
			char *word = (char*) malloc(sizeof(char) * 128);			
			is.getline(word, 128);
			//string *s = new string(word);
			//delete word;
			//cout << " " << *s << " " << endl;	
			l->push_back(word);	
			
			c++;
	
			if (c % 100000 == 0) { break; }
		}
		
		cout << "first:" << l->front() << endl;
		
		cout << "size: " << l->size() << endl;
			endTime = MPI_Wtime();

			//cout << "s_array[0]" << s_array[0] << endl;

			timeUsed = endTime - startTime;
			cout << "time used : " << timeUsed << endl;
		
		// Zeit messen wie lange das einlesen der Datei benötigt.
		
		// Anschließend an Knoten 1 das Array senden
			sendListToNode(1, l);
	} else {
		// warten auf anazhl der wörter
		// SChleife jedes wort einlesen
		// wie lange ist das wort
		// lese zeichen ein
		receiveListFromNode(0);
	}

 	MPI_Finalize();

	
	return EXIT_SUCCESS;
}
