#include <cstdlib>
#include <iostream>
#include <mpi.h>
#include <fstream>
#include <iostream>
#include <list>

using namespace std;

void sendListToNode(int node, list<char*> *data) {
	int size = data->size();
	// First send the total number of elements in the vector
	MPI_Send (&size,1,MPI_INT,node,0,MPI_COMM_WORLD);
	
	for (list<char*>::iterator l_it = data->begin(); l_it != data->end(); l_it++) {
		char *buffer = *l_it;
		// send the size of the data
		int length = strlen(buffer)+1;
		// send the length of the string
		MPI_Send (&length,1,MPI_INT,node,0,MPI_COMM_WORLD);
		
		// send the char*
		MPI_Send (buffer,strlen(buffer)+1,MPI_CHAR,node,0,MPI_COMM_WORLD);
	} 
	
}

list<char*>* receiveListFromNode(int node) {
	MPI_Status status;
	// First receive the size of the vector
	int size = 0;
	MPI_Recv (&size,1,MPI_INT,node,0,MPI_COMM_WORLD,&status);
	
	list<char*> *data = new list<char*>();
	
	for(int i = 0; i < size; i++) {
		// get the length of the char*
		int length = 0;
		MPI_Recv (&length,1,MPI_INT,node,0,MPI_COMM_WORLD,&status);
		char *buffer = (char *)malloc(sizeof(char) * length);
		MPI_Recv (buffer,length,MPI_CHAR,node,0,MPI_COMM_WORLD,&status);
		
		data->push_back(buffer);	
	}
	
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
		string name = "sortMe.txt";
		/*is.open(name.c_str());
		
		is.seekg(0, ios::beg);
		
		char* c_str;
		string line;
		int c = 0;
		while(!is.eof()) {
			getline (is, line);
		        c++;
		
				if (c == 1000000) { cout << "1.000.000" << endl;}
				if (c == 2000000) { cout << "2.000.000" << endl;}
				if (c == 3000000) { cout << "3.000.000" << endl;}
				if (c == 4000000) { cout << "4.000.000" << endl;}
				if (c == 5000000) { cout << "5.000.000" << endl;}
				if (c == 6000000) { cout << "6.000.000" << endl;}
				if (c == 7000000) { cout << "7.000.000" << endl;}
				if (c == 8000000) { cout << "8.000.000" << endl;}
				if (c == 9000000) { cout << "9.000.000" << endl;}
				if (c == 10000000) { cout << "10.000.000" << endl;}
				if (c == 11000000) { cout << "11.000.000" << endl;}
				if (c == 12000000) { cout << "12.000.000" << endl;}
				if (c == 13000000) { cout << "13.000.000" << endl;}
				if (c == 14000000) { cout << "14.000.000" << endl;}
				if (c == 15000000) { cout << "15.000.000" << endl;}
				if (c == 16000000) { cout << "16.000.000" << endl;}
				if (c == 17000000) { cout << "17.000.000" << endl;}
				if (c == 18000000) { cout << "18.000.000" << endl;}
		}
		is.close();
		cout << "zeilen: " << c << endl;
		
		endTime = MPI_Wtime();
		timeUsed = endTime - startTime;
		cout << "time used to count: " << timeUsed << endl;
		*/
		is.open(name.c_str());
		
		is.seekg(0, ios::beg);
		
		
		
		//char** s_array = (char**) malloc(sizeof(char*[128]));
		/*s_array[0] = "hello";
		s_array[1] = "world";
		s_array[2] = "test";
		cout << "s_Array[0]" << s_array[0] << endl;
		cout << "s_Array[1]" << s_array[1] << endl;
		cout << "s_Array[1]" << s_array[2] << endl;*/
		
		
		//int len=sizeof(s_array)/sizeof(char);
		//cout << "lenght: " << len << endl;
		/*
		int index = 0;
		int c = 0;
		cout << "Lese Zeichen ins array ein" << endl;
		char *word = "hallo";
		s_array[0] = word;*/
		
		
		/*
		while (!is.eof()) {
			char *word = (char*) malloc(sizeof(char*));		
			is.getline(word, 128);
			s_array[index] = word;
			index++;
			cout << word << endl;
			
			c++;
	
			if (c == 1000000) { cout << "1.000.000" << endl;}
			if (c == 2000000) { cout << "2.000.000" << endl;}
			if (c == 3000000) { cout << "3.000.000" << endl;}
			if (c == 4000000) { cout << "4.000.000" << endl;}
			if (c == 5000000) { cout << "5.000.000" << endl;}
			if (c == 6000000) { cout << "6.000.000" << endl;}
			if (c == 7000000) { cout << "7.000.000" << endl;}
			if (c == 8000000) { cout << "8.000.000" << endl;}
			if (c == 9000000) { cout << "9.000.000" << endl;}
			if (c == 10000000) { cout << "10.000.000" << endl;}
			if (c == 11000000) { cout << "11.000.000" << endl;}
			if (c == 12000000) { cout << "12.000.000" << endl;}
			if (c == 13000000) { cout << "13.000.000" << endl;}
			if (c == 14000000) { cout << "14.000.000" << endl;}
			if (c == 15000000) { cout << "15.000.000" << endl;}
			if (c == 16000000) { cout << "16.000.000" << endl;}
			if (c == 17000000) { cout << "17.000.000" << endl;}
			if (c == 18000000) { cout << "18.000.000" << endl;}
		}*/
		
	
		
			int c = 0;
		list<char*>* l = new list<char*>();
		cout << "size: " << l->size() << endl;
		while (!is.eof()) {
			char *word = (char*) malloc(sizeof(char[128]));			
			is.getline(word, 128);
			//string *s = new string(word);
			//delete word;
			//cout << " " << *s << " " << endl;	
			l->push_back(word);	
			
			c++;
	
			if (c % 1000000 == 0) { cout << "size: " << l->size() << endl;}
		}
		
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
