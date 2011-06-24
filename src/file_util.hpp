#ifndef _FILETUIL_H
#define _FILEUTIL_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <list>
#include <cstring>
#include <stdlib.h>

using namespace std;

typedef struct {
	unsigned char* array;
	int cursor;
} Histogram;

class FileUtil {
public:	

/**
 	 * Construct a new FileUtil instance.

	 * @param rank The rank of this prozess.
	 * @param size The total number of prozesses.
	 */
	FileUtil(int rank, int size);

	list<char*>* readAllPosition(list<int*> *coursors);

	void printWords(list<char*> *words);

	list<int*>* readFilePositions();

	Histogram** readFile();

	void writeOriginalWordsFromHistogram(list<Histogram*> *histogram_list);
	
	string* printHistogramAsString(Histogram *histogram);
private:
	int mRank;
	int mSize;
	
	Histogram* makeHistogram(char *word, int cursor);
	
	Histogram** copyListToArray(list<Histogram*> *histogram_list, int &size);
};

#endif	/* _FILEUTIL_H */
