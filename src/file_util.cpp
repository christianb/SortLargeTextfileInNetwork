#include "file_util.hpp"

using namespace std;

// reads all positions within list from a file
list<char*>* FileUtil::readAllPosition(list<int*> *coursors) {
	cout << "read word at position" << endl;
	ifstream is;
	string name = "./sortMe.txt";
	
	is.open(name.c_str(), ifstream::in);
	
	list<char*> *words = new list<char*>();
	  
	int index = 1;
	for (list<int*>::iterator it = coursors->begin(); it != coursors->end(); it++) {
		is.seekg(**it, ios::beg);

		char *word = (char*) malloc(sizeof(char) * 128);			
		is.getline(word, 128);

		words->push_back(word);
		//delete *it; // delete because not used anymore

		if (index % 100000 == 0) {
			cout << index << endl;
		}
		index++;
	}
	
	is.close();
	cout << "done" << endl;
	
	return words;
}

list<char*>* FileUtil::readFile() {
	ifstream is;
	string name = "./sortMe.txt";
	
	is.open(name.c_str(), ifstream::in);

	// stores for each line the coursor position
	list<int*> *coursors = new list<int*>();
	if (!is.eof()) {
		coursors->push_back(new int(0));
	}

	list<char*> *words = new list<char*>();

	int index = 1; 
	while (!is.eof()) {
					
			
		string s;
		getline(is, s, '\n');

		char *word = (char*) malloc(sizeof(char) * s.length()+1);
		strcpy(word, s.c_str());

		words->push_back(word);

		if (index % 100000 == 0) {
			//cout << index << endl;
			//cout << "length: " << s.length() << endl;
			//cout << "word: " << word << endl;
		}
		index++;
	}

	cout << words->front() << endl;

	return words;
}

void FileUtil::printWords(list<char*> *words) {
	for (list<char*>::iterator it = words->begin(); it != words->end(); it++) {
		cout << *it << endl;
	}
}

list<int*>* FileUtil::readFilePositions() {
	cout << "read cursor position from file" << endl;
	ifstream is;
	string name = "./sortMe.txt";
	
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
		if (is.get() == 10) {
			
			coursors->push_back(new int(is.tellg()));
			if (index % 100000 == 0) {
				cout << index << endl;
			}
			index++;
		}
	}
	
	is.close();

	cout << "done" << endl;
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
