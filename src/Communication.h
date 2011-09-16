#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#include "Histogram.h"


void sendHistogram(int node, Histogram **data, int size, MPI_Datatype *HISTOGRAM_TYPE); // Sende Histogram an Prozess.
Histogram* receiveHistogram(int node, unsigned int *size_received, Histogram *master, unsigned int size_master, MPI_Datatype *HISTOGRAM_TYPE, Histogram** ref_data_sorted); // Empfange Histogram von Prozess

// Empfange Sortierte Daten um diese parallel wegzuschreiben
Histogram* receiveSortedHistogram(int node, unsigned int *size_received, MPI_Datatype *HISTOGRAM_TYPE);
void sendSortedHistogram(int node, int ranks, Histogram* data, int size, MPI_Datatype *HISTOGRAM_TYPE);

#endif /* COMMUNICATION_H */
