#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iostream>
#include <list>

using namespace std;

// reads all positions within list from a file
list<char*>* readAllPosition(list<int*> *coursors) {
	ifstream is;
	string name = "./hello.txt";
	
	is.open(name.c_str(), ifstream::in);
	
	list<char*> *words = new list<char*>();
	  
	for (list<int*>::iterator it = coursors->begin(); it != coursors->end(); it++) {
		is.seekg(**it, ios::beg);

		char *word = (char*) malloc(sizeof(char) * 128);			
		is.getline(word, 128);

		words->push_back(word);
	}
	
	is.close();
	
	return words;
}

void printWords(list<char*> *words) {
	for (list<char*>::iterator it = words->begin(); it != words->end(); it++) {
		cout << *it << endl;
	}
}

list<int*>* readFilePositions() {
	ifstream is;
	string name = "./hello.txt";
	
	is.open(name.c_str(), ifstream::in);
	
	// if file not exist
	if (!is) {
		cout << "File not found" << endl;
		return EXIT_SUCCESS;
	}

	// go to first coursor
	is.seekg(0, ios::beg);
	
	// stores for each line the coursor position
	list<int*> *coursors = new list<int*>();
	if (!is.eof()) {
		coursors->push_back(new int(0));
	}
	
	int index = 1;
	//cout << "size: " << l->size() << endl;
	while (!is.eof()) {		
		if ((char) is.get() == '\n') {
			
			coursors->push_back(new int(is.tellg()));
			index++;
		}
	}
	
	is.close();

	return coursors;
}
/*
int main (int argc, char *argv[]) {		
	// get the line numbers (all)
	list<int*> *coursors = readFilePositions();

	// get the words for this line number
	list<char*> *list = readAllPosition(coursors);
	printWords(list);
	
	return EXIT_SUCCESS;
}*/
