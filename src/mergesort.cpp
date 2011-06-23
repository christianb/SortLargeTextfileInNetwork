#include "mergesort.h"
#include <list>

using namespace std;

list<unsigned char*>* Mergesort::sort(list<unsigned char*> *l){
	cout << "call sort()" << endl;
	cout << "list size = " << l->size();
	
	list<unsigned char*> *leftList = new list<unsigned char*>();
	list<unsigned char*> *rightList = new list<unsigned char*>();
	list<unsigned char*> *result = new list<unsigned char*>();;
	
	// Gesamtlänge und Mitte der Liste bestimmen
	int listLength = l->size();
	int middle = listLength / 2;		
	
	// Wenn Liste nur ein Element hat, dies zurückgeben
	if (l->size() <= 1){
		return l;
	} else {
		
		list<unsigned char*>::iterator it;
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

list<unsigned char*>* Mergesort::merge(list<unsigned char*> *left, list<unsigned char*> *right){
	cout << "call merge()" << endl;
	cout << "left size = " << left->size();
	cout << "right size = " << right->size() << endl;
	
	list<unsigned char*>* newList = new list<unsigned char*>(); // mergedList
	list<unsigned char*>::iterator leftIt;
    list<unsigned char*>::iterator rightIt;
	
	
	while (left->size() > 0 || right->size() > 0){
		cout << "left and right is not empty!" << endl;
		
		// Pointer bei jedem durchlauf auf erstes Element setzen
		leftIt = left->begin();
		rightIt = right->begin();
		
				
		if (left->size() > 0 && right->size() > 0) {		// Beide Listen beinhalten noch Elemente
            unsigned char *leftHistogram = *leftIt;
            unsigned char *rightHistogram = *rightIt;
            
            // get sum and compare
			bool didMerge = false;
            for (int i = 0 ; i < 26 ; i++) {
                int sumLeftHistogram = getSumOfLetterCaseInsensitive(i, leftHistogram);
                int sumRightHistogram = getSumOfLetterCaseInsensitive(i, rightHistogram);

                if (sumLeftHistogram < sumRightHistogram) {
                    // ok rechtes Histogram kommt zuertst
                    newList->push_back(rightHistogram);
					right->erase(rightIt);
					didMerge = true;
                    break;  // stop comparing of this both histograms
                }
                
                if (sumLeftHistogram > sumRightHistogram) {
                    // ok linkes Histogram kommt zuerst;
                    newList->push_back(leftHistogram);
                    left->erase(leftIt);
					didMerge = true;
                    break;
                }
                
                if (sumLeftHistogram == sumRightHistogram) {
                    // schauen wer mehr kleine Buchstaben hat
                    
                    // anzahl der klein buchstaben
                    int lowerCaseLetterLeftHistogram = leftHistogram[i+26];
                    int lowerCaseLetterRightHistogram = rightHistogram[i+26];
                    
                    if (lowerCaseLetterRightHistogram > lowerCaseLetterLeftHistogram) {
                        // ok rechtes Histogram kommt zuertst
                        newList->push_back(rightHistogram);
						right->erase(rightIt);
						didMerge = true;
                        break;  // stop comparing of this both histograms
                    }
                    
                    if (lowerCaseLetterRightHistogram < lowerCaseLetterLeftHistogram) {
                        // ok linkes Histogram kommt zuerst;
                        newList->push_back(leftHistogram);
                        left->erase(leftIt);
						didMerge = true;
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
