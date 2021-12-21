#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <mpi.h>

#include "models.h"
#include "sortUtil.h"
#include "fileUtil.h"
#include "mpiUtil.h"

// ======================================================

#define READ_FILE_NAME "rectangles.dat"
#define WRITE_FILE_NAME "result.dat"

// ======================================================

int main(int argc, char* argv[])
{
	int rank, numOfRects, sendCount = 0;
	int* results = (int*)malloc(sizeof(int));

	Bundle mpiBundle;
	Rect myRect, *rects = NULL;

	InitMPI(&argc, &argv, &rank, &mpiBundle);

	if (rank == 0) 
	{
		rects = readFromFile(READ_FILE_NAME, &numOfRects); // read rects from file
		sendCount = 1;
		printf("starting sort\n");
	}

	MPI_Scatter(rects, sendCount, mpiBundle.rectType, &myRect, 1, mpiBundle.rectType, 0, MPI_COMM_WORLD); // distribute rects 

	int value = shearSort(myRect, mpiBundle); // sort rects

	MPI_Gather(&value, 1, MPI_INT, results, 1, MPI_INT, 0, MPI_COMM_WORLD); // gather results

	if (rank == 0) // master
	{
		printf("finished sort\n");
		writeToFile(WRITE_FILE_NAME, results, numOfRects); // write rects to file
	}

	MPI_Finalize();
	return 0;
}

// ======================================================
