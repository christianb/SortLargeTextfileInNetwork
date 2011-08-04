#include "mergesort.hpp"
#include <list>

using namespace std;

list<Histogram*>* Mergesort::sort(list<Histogram*> *l){
	list<Histogram*> *leftList = new list<Histogram*>();
	list<Histogram*> *rightList = new list<Histogram*>();
	list<Histogram*> *result = new list<Histogram*>();;
	
	// Gesamtlänge und Mitte der Liste bestimmen
	int listLength = l->size();
	int middle = listLength / 2;		
	
	// Wenn Liste nur ein Element hat, dies zurückgeben
	if (l->size() <= 1){
		return l;
	} else {
		
		list<Histogram*>::iterator it;
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

int Mergesort::getSumOfLetterCaseInsensitive(int indexLetter, unsigned char* histogram) {
    if (indexLetter < 26) {
        int upperCaseLetter = histogram[indexLetter];
		    int lowerCaseLetter = histogram[indexLetter+26];
        
        return (upperCaseLetter + lowerCaseLetter);
    }
    
    return 0;
}

list<Histogram*>* Mergesort::merge(list<Histogram*> *left, list<Histogram*> *right){	
	list<Histogram*>* newList = new list<Histogram*>(); // mergedList
	list<Histogram*>::iterator leftIt;
    list<Histogram*>::iterator rightIt;
	
	
	while (left->size() > 0 || right->size() > 0){
		
		// Pointer bei jedem durchlauf auf erstes Element setzen
		leftIt = left->begin();
		rightIt = right->begin();
		
				
		if (left->size() > 0 && right->size() > 0) {		// Beide Listen beinhalten noch Elemente

			Histogram* _leftHistogram = *leftIt;
            unsigned char *leftHistogram = _leftHistogram->array;

			Histogram* _rightHistogram = *rightIt;
            unsigned char *rightHistogram = _rightHistogram->array;
            
            // get sum and compare
			bool didMerge = false;
            for (int i = 0 ; i < 26 ; i++) {
                int sumLeftHistogram = getSumOfLetterCaseInsensitive(i, leftHistogram);
                int sumRightHistogram = getSumOfLetterCaseInsensitive(i, rightHistogram);

                if (sumLeftHistogram < sumRightHistogram) {
                    // ok rechtes Histogram kommt zuertst
                    newList->push_back(_rightHistogram);
					          right->erase(rightIt);
					          didMerge = true;
					          //cout << "merge successfull" << endl;
                    break;  // stop comparing of this both histograms
                }
                
                if (sumLeftHistogram > sumRightHistogram) {
                    // ok linkes Histogram kommt zuerst;
                    newList->push_back(_leftHistogram);
                    left->erase(leftIt);
					didMerge = true;
					//cout << "merge successfull" << endl;
                    break;
                }
                
                if (sumLeftHistogram == sumRightHistogram) {
                    // schauen wer mehr kleine Buchstaben hat
                    
                    // anzahl der klein buchstaben
                    int lowerCaseLetterLeftHistogram = leftHistogram[i+26];
                    int lowerCaseLetterRightHistogram = rightHistogram[i+26];
                    
                    if (lowerCaseLetterRightHistogram > lowerCaseLetterLeftHistogram) {
                        // ok rechtes Histogram kommt zuertst
                        newList->push_back(_rightHistogram);
						right->erase(rightIt);
						didMerge = true;
						//cout << "merge successfull" << endl;
                        break;  // stop comparing of this both histograms
                    }
                    
                    if (lowerCaseLetterRightHistogram < lowerCaseLetterLeftHistogram) {
                        // ok linkes Histogram kommt zuerst;
                        newList->push_back(_leftHistogram);
                        left->erase(leftIt);
						didMerge = true;
						//cout << "merge successfull" << endl;
                        break;
                    }
                    
                    if (lowerCaseLetterLeftHistogram == lowerCaseLetterRightHistogram) {
                        continue;
                    }
                }
            }

			// edge condition if both elements are absolut identical
			if (!didMerge) {
				newList->push_back(*leftIt);					// Dieses einfügen
				left->erase(leftIt);
				//cout << "merge successfull" << endl;
			}
            
			
		} else if ( left->size() > 0){					// Nur linke Liste enthält Elemente
			newList->push_back(*leftIt);					// Dieses einfügen
			left->erase(leftIt);		
			//cout << "merge successfull" << endl;					// und löschen
		} else if (right->size() > 0){					// Nur rechte Liste enthät Elemente
			newList->push_back(*rightIt);				// Element einfügen
			right->erase(rightIt);		
			//cout << "merge successfull" << endl;				// und löschen
		}
	}
	
	return newList;
}
