#include <mpi.h>
#include <math.h>
#include <stddef.h>

#include "models.h"
#include "mpiUtil.h"
#include "sortUtil.h"

// ======================================================

#define NUM_OF_ATTRIBUTES 3 
#define DIMS 2

// ======================================================

// initialize mpi communication
MPI_Comm InitMPI(int* argc, char** argv[], int* rank, Bundle* mpiBundle)
{
	// Init MPI
	MPI_Init(argc, argv);
	MPI_Comm_rank(MPI_COMM_WORLD, rank); // get process rank
	MPI_Comm_size(MPI_COMM_WORLD, &mpiBundle->numOfProcs); // get number of processors

	createRectType(&mpiBundle->rectType); //create rectType

	mpiBundle->rowSize = sqrt(mpiBundle->numOfProcs);
	mpiBundle->cartComm = createCartComm(*rank, mpiBundle); // Init cartComm
}

// ======================================================

// create data type for mpi communication
void createRectType(MPI_Datatype* rectType)
{
	int blockLen[NUM_OF_ATTRIBUTES] = { 1, 1, 1 };
	MPI_Aint disp[NUM_OF_ATTRIBUTES];
	MPI_Datatype types[NUM_OF_ATTRIBUTES] = { MPI_INT, MPI_FLOAT, MPI_FLOAT };

	disp[0] = offsetof(Rect, id);
	disp[1] = offsetof(Rect, height);
	disp[2] = offsetof(Rect, width);

	MPI_Type_create_struct(NUM_OF_ATTRIBUTES, blockLen, disp, types, rectType);
	MPI_Type_commit(rectType);
}

// ======================================================

// create mpi communicator
MPI_Comm createCartComm(int rank, Bundle* mpiBundle)
{
	MPI_Comm cartComm;
	int dim[DIMS], period[DIMS], reorder;

	dim[0] = mpiBundle->rowSize;			// num of rows
	dim[1] = mpiBundle->rowSize;			// num of columns

	period[0] = 0;		// rows are not cyclic
	period[1] = 0;		// cols are not cyclic

	reorder = 1;		// allows changing the order of processes ids

	MPI_Cart_create(MPI_COMM_WORLD, DIMS, dim, period, reorder, &cartComm);
	MPI_Cart_coords(cartComm, rank, DIMS, &mpiBundle->coords);

	return cartComm;
}

// ======================================================
