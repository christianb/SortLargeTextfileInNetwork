#ifndef _MERGESORT_H
#define _MERGESORT_H

#include <iostream>
#include <list>
#include <string>
#include <algorithm>

using namespace std;

class Mergesort {
		
public:
	list<char*>* sort(list<char*> *l);

	list<char*>* merge(list<char*> *left, list<char*> *right);
	
	bool lessThan(char* s1, char *s2);
	
private:
	
};

#endif	/* _MERGESORT_H */