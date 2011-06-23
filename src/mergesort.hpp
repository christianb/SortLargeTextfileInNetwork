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
	list<_histogram_data*>* sort(list<_histogram_data*> *l);

	list<_histogram_data*>* merge(list<_histogram_data*> *left, list<_histogram_data*> *right);
	
private:
	int getSumOfLetterCaseInsensitive(int indexLetter, unsigned char* histogram);
};

#endif	/* _MERGESORT_H */