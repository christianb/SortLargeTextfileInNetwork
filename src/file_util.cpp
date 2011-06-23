#include "file_util.hpp"

using namespace std;

FileUtil::FileUtil(int rank, int size) {
	this->mRank = rank;
	this->mSize = size;
}

_histogram_data* FileUtil::makeHistogram(char *word, int cursor) {
	int len = strlen(word);
	
	unsigned char *histogram = (unsigned char*) malloc(sizeof(unsigned char) * 52);
	for (int i = 0 ; i < 52 ; i++) {
		histogram[i] = 0;
	}
	
	
	for (int i = 0; i< len; i++) {
		if (word[i] >= 'A' && word[i] <= 'Z') {
			histogram[word[i]-'A']++;
		} 
	
		if (word[i] >= 'a' && word[i] <= 'z') {
			histogram[word[i]-'A'-6]++;
		}
	}
	
	_histogram_data* _histogram = (_histogram_data*) malloc (sizeof(_histogram_data));
	_histogram->array = histogram;
	_histogram->cursor = cursor;
	
	return _histogram;
}

// reads all lines at positions (given by a list) within file
// NOT USED, PERHAPS LATER!!!
list<char*>* FileUtil::readAllPosition(list<int*> *coursors) {
	cout << "read word at position" << endl;
	ifstream is;
	string name = "./Euler.txt";
	
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

// read all words from a file
list<_histogram_data*>* FileUtil::readFile() {
	//cout << "call read file" << endl;
	
	string name = "./Euler.txt";
	
	ifstream is (name.c_str(), ifstream::in);

	if (is.fail()) {
		cout << "File: "<< name << " not found!" << endl;
		return new list<_histogram_data*>();
	}

	// 1) determine the length of the file
	is.seekg(0, ios::end);
	const int length = is.tellg();
	//cout << "file contains " << length << " characters" << endl;

	/* 	2) devide the number of characters of the number of prozesses
		rundungsfehler sollten egal sein, da immer bis zur letzten Zeile gesprungen wird!
		int nimmt nur die Zahl vor dem Komma */
	int i = length / this->mSize;
	//cout << "characters to be read by this prozess: " << i << endl;
		
	// 3) calculate from pos
	int from = mRank * i;
	//cout << "from: " << from << endl;
	// 4) calculate to pos
	int to = ((mRank+1) * i);
	//cout << "to: " << to << endl;
	
	int c;
	// 5) if this is not he first prozess
	// determin real from position
	if (mRank != 0) {
		is.seekg(from, ios::beg);
		// read chars while line break has found
		do {
			c = (char) is.get();
		} while (c != 10);
		// after line break has found, the next char is the first char in next line
		// new from position is from here
		from = is.tellg();
	}

	// 6) if this is not the last prozess
	// determine real to position
	if (mRank != (mSize-1)) {
		is.seekg(to, ios::beg);
		// read chars while line break has found
		do {
			c = is.get();
			//cout << c << endl;		
		} while (c != 10);
		// after line break has found, the next char is the first char in next line
		// go back to before the line break.
		to = is.tellg();
		to--;
	} else {
		is.seekg(0, ios::end);
		to = is.tellg();
	}

	// now we have set the from and to position in the file
	//cout << "read from " << from << " to " << to << endl;

	// 7) before we can read the file we must determine how many chars we have to read now,
	// because from and to position has changed.
	//int new_length = to - from;
	//cout << "new length to be read by this prozess: " << new_length << endl;
	
	// 8) read characters
	is.seekg(from, ios::beg); // go where to start
	//cout << "start reading at char pos: " << is.tellg() << endl;
 
  //cout << "start to read from file..." << endl;
  //list<char*> *words = new list<char*>();
	list<_histogram_data*> *histogram_list = new list<_histogram_data*>();
	
	int index = 1;
	do {
		int cursor = is.tellg();
		string s; // temp string, just to determine the line length
		getline(is, s, '\n'); // get the line until line break

    	// alloc an char array with length of string
		char *word = (char*) malloc(sizeof(char) * s.length()+1);
		strcpy(word, s.c_str()); // copy string

    	// insert word in list
		//words->push_back(word);
		histogram_list->push_back(makeHistogram(word, cursor));
		
		free(word);
    
		if (index % 100000 == 0) {
		  cout << index << endl;
			//cout << "length: " << s.length() << endl;
			//cout << "word: " << word << endl;
		}
		
		index++;
	} while (is.tellg() <= to && is.good());

	return histogram_list;
}

// print all words in list
// NOT USED, PERHAPS LATER!!!
void FileUtil::printWords(list<char*> *words) {
	for (list<char*>::iterator it = words->begin(); it != words->end(); it++) {
		cout << *it << endl;
	}
}

// collect the cursor position where every line starts
// NOT USED, PERHAPS LATER!!!
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
