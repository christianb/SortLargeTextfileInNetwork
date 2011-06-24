#ifndef _FILETUIL_H
#define _FILEUTIL_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <list>
#include <cstring>

using namespace std;

struct _histogram_data {
	unsigned char* array;
	int cursor;
};

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

	list<_histogram_data*>* readFile();

	void writeOriginalWordsFromHistogram(list<_histogram_data*> *histogram_list);
	
	string* printHistogramAsString(_histogram_data *histogram);
private:
	int mRank;
	int mSize;
	
	_histogram_data* makeHistogram(char *word, int cursor);

};

#endif	/* _FILEUTIL_H */
