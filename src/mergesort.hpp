#ifndef _MERGESORT_H
#define _MERGESORT_H

#include <iostream>
#include <list>
#include <string>
#include <algorithm>
#include "file_util.hpp"

using namespace std;

class Mergesort {
		
public:
	list<Histogram*>* sort(list<Histogram*> *l);

	list<Histogram*>* merge(list<Histogram*> *left, list<Histogram*> *right);
	
private:
	int getSumOfLetterCaseInsensitive(int indexLetter, unsigned char* histogram);
};

#endif	/* _MERGESORT_H */