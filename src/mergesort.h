#ifndef _MERGESORT_H
#define _MERGESORT_H

#include <iostream>
#include <list>
#include <string>
#include <algorithm>

using namespace std;

class Mergesort {
		
public:
	list<unsigned char*>* sort(list<unsigned char*> *l);

	list<unsigned char*>* merge(list<unsigned char*> *left, list<unsigned char*> *right);
	
private:
	int getSumOfLetterCaseInsensitive(int indexLetter, unsigned char* histogram);
};

#endif	/* _MERGESORT_H */