#include "mergesort.h"
#include <list>

using namespace std;

list<char*>* Mergesort::sort(list<char*> *l){
	list<char*> *leftList = new list<char*>();
	list<char*> *rightList = new list<char*>();
	list<char*> *result = new list<char*>();;
	
	// Gesamtlänge und Mitte der Liste bestimmen
	int listLength = l->size();
	int middle = listLength / 2;		
	
	// Wenn Liste nur ein Element hat, dies zurückgeben
	if (l->size() <= 1){
		return l;
	} else {
		
		list<char*>::iterator it;
		it = l->begin();
		
		// Pointer auf Mitte der Liste setzen
		advance(it, middle);
		
		// Liste in zwei hälften teilen
		leftList->splice(leftList->begin(), *l, l->begin(), it);
		rightList->splice(rightList->begin(), *l, it, l->end());
		
		// Rekursiver aufruf
		leftList = this->sort(leftList);
		rightList = this->sort(rightList);
		
		// Sotieren
		result = this->merge(leftList, rightList);
		
		return result;
	}
}

list<char*>* Mergesort::merge(list<char*> *left, list<char*> *right){
	//cout << "call merge()" << endl;
	list<char*>* newList = new list<char*>(); // mergedList
	list<char*>::iterator leftIt;
	list<char*>::iterator rightIt;
	
	
	while (left->size() > 0 || right->size() > 0){
		
		// Pointer bei jedem durchlauf auf erstes Element setzen
		leftIt = left->begin();
		rightIt = right->begin();
		
				
		if (left->size() > 0 && right->size() > 0){		// Beide Listen beinhalten noch Elemente
			if (lessThan(*leftIt, *rightIt)){	
				//cout << "compare: " << *leftIt << " and " << *rightIt << endl;				// Linkes Element als erstes in neue Liste einfügen, falls diese kleiner als rechtes Element ist
				newList->push_back(*leftIt);				// Element einfügen
				left->erase(leftIt);						// Eingefügtes Element aus alter Liste löschen
			} else {									// Falls rechtes Element größer, dieses als erstes in neue Liste einfügen
				newList->push_back(*rightIt);
				right->erase(rightIt);
			}
		} else if ( left->size() > 0){					// Nur linke Liste enthält Elemente
			newList->push_back(*leftIt);					// Dieses einfügen
			left->erase(leftIt);							// und löschen
		} else if (right->size() > 0){					// Nur rechte Liste enthät Elemente
			newList->push_back(*rightIt);				// Element einfügen
			right->erase(rightIt);						// und löschen
		}
	}
	
	return newList;
}

bool Mergesort::lessThan(char* s1, char *s2) {
	if (lexicographical_compare(s1,s1+strlen(s1)-1,s2,s2+strlen(s2)-1)) {
		return true;
	} else {
		return false;
	}
}
/*
bool compare (char c1, char c2)
{ 
	return tolower(c1) < tolower(c2) ;
}
*/
