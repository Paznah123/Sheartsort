#pragma once

#include <mpi.h>
#include "models.h"

// ======================================================

MPI_Comm InitMPI(int* argc, char** argv[], int* rank, Bundle* mpiBundle);
MPI_Comm createCartComm(int rank, Bundle* mpiBundle);
void createRectType(MPI_Datatype* rectType);
