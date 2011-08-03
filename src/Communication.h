#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#include "Histogram.h"


void sendHistogram(int node, Histogram *data, int size); // Sende Histogram an Prozess.
Histogram* receiveHistogram(int node, int *size); // Empfange Histogram von Prozess

#endif /* COMMUNICATION_H */
