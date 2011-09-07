#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#include "Histogram.h"


void sendHistogram(int node, Histogram **data, int size, MPI_Datatype *HISTOGRAM_TYPE); // Sende Histogram an Prozess.
void _sendHistogram(int node, Histogram *data, int size, MPI_Datatype *HISTOGRAM_TYPE);
Histogram* receiveHistogram(int node, unsigned int *size_received, MPI_Datatype *HISTOGRAM_TYPE, Histogram *sortedLocalData, unsigned int size_sortedLocalData); // Empfange Histogram von Prozess

#endif /* COMMUNICATION_H */
