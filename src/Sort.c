#include "Sort.h"

Histogram* sort(Histogram *data, int *size) {
  //printf("call sort(), size = %d\n", *size);
  //printHistogramArray(data, *size);

  if (*size == 1) { // Abbruchbedingung der Rekursion!!
    // Wenn nur ein Element vorhanden ist gebe dieses zurück.
    return data;
  }

  // Elemente in zwei Hälften teilen
  int size_left = *size / 2;
  int size_right = size_left;

  if (*size % 2 != 0) {
    size_right++;
  }
  
  // Speicher für die zwei Hälften allokieren.
  Histogram *left = (Histogram*) malloc (sizeof(Histogram) * size_left);
  Histogram *right = (Histogram*) malloc (sizeof(Histogram) * size_right);

  // Elemente in den neuen Speicher kopieren.
  memcpy (left, data, size_left*sizeof(Histogram)); // kopiere die erste Hälfte
  memcpy (right, (data+size_left), size_right*sizeof(Histogram)); // kopiert die zweite Hälfte der Elemente

  // Speicher Freigeben
  free(data);

  // nun gehts weiter mit Rekursion
  left = sort(left, &size_left);
  right = sort(right, &size_right);

  // mergen und das Ergebnis zurückgeben
  Histogram *result = merge(left, &size_left, right, &size_right);

  free(left);
  free(right);

  return result;
}

Histogram* merge(Histogram *left, int *size_left, Histogram *right, int *size_right) {
  //printf("call merge()\n");
  // Allokiere Speicher, so das beide Hälften (sortiert) hier gespeichert werden können.
  Histogram *merged = (Histogram*) malloc (sizeof(Histogram) * (*size_left + *size_right));
  int index_merged = 0;  

  int index_left = 0;
  int index_right = 0;

  bool didMerge = FALSE;

  while (index_left < (*size_left) || index_right < (*size_right)) { // Solange der Index der jewiligen Hälfte nicht größer / gleich ist wie die size
    if (index_left < (*size_left) && index_right < (*size_right)) { // ...
      // Wir gehen die Häften Elementweise durch
        
      int i;
      for (i = 0 ; i < 26 ; i++) { 
        // Vergleiche die Anzahl der Buchstaben (Groß und Klein).
        int sumLeftHistogram = getSumOfLetterCaseInsensitive(i, index_left, left);
        int sumRightHistogram = getSumOfLetterCaseInsensitive(i, index_right, right);

        if (sumLeftHistogram < sumRightHistogram) { // Rechts hat mehr gleiche Buchstaben
          // ok rechtes Histogram kommt zuertst
          memcpy((merged+index_merged), (right+index_right), sizeof(Histogram));
          index_merged++;
          index_right++;
          didMerge = TRUE;
          break;
        }

        if (sumLeftHistogram > sumRightHistogram) {
          // linkes Histogram kommt zuerst
          memcpy((merged+index_merged), (left+index_left), sizeof(Histogram));
          index_merged++;
          index_left++;
          didMerge = TRUE;
          break;
        }

        if (sumLeftHistogram == sumRightHistogram) {
          // Wenn die gleiche Anzahl an Buchstaben, dann kommt das Histogramm mit den meisten kleineren Buchstaben
          int lowerCaseLetterLeftHistogram = left[index_left].letter[i+26];
          int lowerCaseLetterRightHistogram = right[index_right].letter[i+26];

          if (lowerCaseLetterRightHistogram > lowerCaseLetterLeftHistogram) {
            // ok rechtes Histogram kommt zuertst
            memcpy((merged+index_merged), (right+index_right), sizeof(Histogram));
            index_merged++;
            index_right++;
            didMerge = TRUE;
            break;
          }

          if (lowerCaseLetterRightHistogram < lowerCaseLetterLeftHistogram) {
            // linkes Histogram kommt zuerst;
            memcpy((merged+index_merged), (left+index_left), sizeof(Histogram));
            index_merged++;
            index_left++;
            didMerge = TRUE;
            break;
          }

          if (lowerCaseLetterLeftHistogram == lowerCaseLetterRightHistogram) {
            continue;
          }
        }
      }

      if (didMerge == FALSE) {
         // linkes Histogram kommt zuerst;
         memcpy((merged+index_merged), (left+index_left), sizeof(Histogram));
         index_merged++;
         index_left++;
      }

      didMerge = FALSE;

    } else {
      // Nur linke Häfte enthält Elemente    
      if (index_left < (*size_left)) {
        memcpy((merged+index_merged), (left+index_left), sizeof(Histogram));
        index_merged++;
        index_left++;
      }

      if (index_right < (*size_right)) {
        memcpy((merged+index_merged), (right+index_right), sizeof(Histogram));
        index_merged++;
        index_right++;
      }
    }
  }

  return merged;
}

int getSumOfLetterCaseInsensitive(int index_letter, int index, Histogram *histogram) {
    if (index_letter < 26) {
        int upperCaseLetter = histogram[index].letter[index_letter];
		    int lowerCaseLetter = histogram[index].letter[index_letter+26];
        
        return (upperCaseLetter + lowerCaseLetter);
    }
    
    return 0;
}

