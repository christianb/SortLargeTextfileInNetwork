#include "mergesort.hpp"
#include <list>

using namespace std;

list<_histogram_data*>* Mergesort::sort(list<_histogram_data*> *l){
	list<_histogram_data*> *leftList = new list<_histogram_data*>();
	list<_histogram_data*> *rightList = new list<_histogram_data*>();
	list<_histogram_data*> *result = new list<_histogram_data*>();;
	
	// Gesamtlänge und Mitte der Liste bestimmen
	int listLength = l->size();
	int middle = listLength / 2;		
	
	// Wenn Liste nur ein Element hat, dies zurückgeben
	if (l->size() <= 1){
		return l;
	} else {
		
		list<_histogram_data*>::iterator it;
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

list<_histogram_data*>* Mergesort::merge(list<_histogram_data*> *left, list<_histogram_data*> *right){	
	list<_histogram_data*>* newList = new list<_histogram_data*>(); // mergedList
	list<_histogram_data*>::iterator leftIt;
    list<_histogram_data*>::iterator rightIt;
	
	
	while (left->size() > 0 || right->size() > 0){
		
		// Pointer bei jedem durchlauf auf erstes Element setzen
		leftIt = left->begin();
		rightIt = right->begin();
		
				
		if (left->size() > 0 && right->size() > 0) {		// Beide Listen beinhalten noch Elemente

			_histogram_data* _leftHistogram = *leftIt;
            unsigned char *leftHistogram = _leftHistogram->array;

			_histogram_data* _rightHistogram = *rightIt;
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
