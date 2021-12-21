#pragma once

#include <mpi.h>

// ======================================================

typedef struct
{
	int numOfProcs;
	int rowSize;
	int coords[2];
	MPI_Datatype rectType;
	MPI_Comm cartComm;

} Bundle;

// ======================================================

typedef struct
{
	int id;
	float width;
	float height;

} Rect;

// ======================================================
