#ifndef _FILETUIL_H
#define _FILEUTIL_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <list>
#include <cstring>


using namespace std;

class FileUtil {
public:	

list<char*>* readAllPosition(list<int*> *coursors);

void printWords(list<char*> *words);

list<int*>* readFilePositions();

list<char*>* readFile();

};

#endif	/* _FILEUTIL_H */
