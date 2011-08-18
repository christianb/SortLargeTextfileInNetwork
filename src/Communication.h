#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#include "Histogram.h"


void sendHistogram(int node, Histogram **data, int size, MPI_Datatype *HISTOGRAM_TYPE); // Sende Histogram an Prozess.
Histogram* receiveHistogram(int node, unsigned int *size_received, Histogram *master, unsigned int size_master, MPI_Datatype *HISTOGRAM_TYPE); // Empfange Histogram von Prozess

#endif /* COMMUNICATION_H */
