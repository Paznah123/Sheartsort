#include <mpi.h>
#include <math.h>

#include "models.h"
#include "sortUtil.h"

// ======================================================

#define ROW_DIR 0
#define COL_DIR 1

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

#define MAX '+'
#define MIN '-'

// ====================================================== main sort

int shearSort(Rect rect, Bundle mpiBundle)
{
	MPI_Status status;

	int neighbors[4];

	MPI_Cart_shift(mpiBundle.cartComm, 1, 1, &neighbors[LEFT], &neighbors[RIGHT]);
	MPI_Cart_shift(mpiBundle.cartComm, 0, 1, &neighbors[DOWN], &neighbors[UP]);

	for (int phase = 0; phase < log2(mpiBundle.numOfProcs); phase++) // log2(n) iterations
	{
		if (phase % 2 == 0) // even iteration sort rows
			oddEvenSortRow(&rect, neighbors[LEFT], neighbors[RIGHT], mpiBundle, &status);
		else // odd iteration sort columns
			oddEvenSortCol(&rect, neighbors[DOWN], neighbors[UP], mpiBundle, &status);
	}
	// +1 iteration
	oddEvenSortRow(&rect, neighbors[LEFT], neighbors[RIGHT], mpiBundle, &status);

	return rect.id;
}

// ====================================================== sort functions

void oddEvenSortRow(Rect* rect, int left, int right, Bundle mpiBundle, MPI_Status* status)
{
	char compare = ' ';

	for (int i = 0; i < mpiBundle.rowSize; i++)
	{
		if (mpiBundle.coords[COL_DIR] % 2 != 0) // odd column
		{
			if (i % 2 == 0) 
			{
				if (mpiBundle.coords[ROW_DIR] % 2 == 0)		compare = MAX;
				else										compare = MIN;
				swapRects(rect, mpiBundle.rectType, left, status, compare);
			}
			else if (right != -1) // if right neighbor exists
			{
				if (mpiBundle.coords[ROW_DIR] % 2 == 0)		compare = MIN;
				else										compare = MAX;
				swapRects(rect, mpiBundle.rectType, right, status, compare);
			}
		}
		else // even column
		{
			if (i % 2 == 0)
			{
				if (mpiBundle.coords[ROW_DIR] % 2 == 0)		compare = MIN;
				else										compare = MAX;
				swapRects(rect, mpiBundle.rectType, right, status, compare);
			}
			else if (left != -1) // if left neighbor exists
			{
				if (mpiBundle.coords[ROW_DIR] % 2 == 0)		compare = MAX;
				else										compare = MIN;
				swapRects(rect, mpiBundle.rectType, left, status, compare);
			}
		}
	}
}

void oddEvenSortCol(Rect* rect, int up, int down, Bundle mpiBundle, MPI_Status* status)
{
	for (int i = 0; i < mpiBundle.rowSize; i++)
	{
		if (mpiBundle.coords[ROW_DIR] % 2 != 0) // odd row
		{
			if (i % 2 == 0)			swapRects(rect, mpiBundle.rectType, up, status, MAX);
			else if (down != -1)	swapRects(rect, mpiBundle.rectType, down, status, MIN);
		}
		else // even row
		{
			if (i % 2 == 0)			swapRects(rect, mpiBundle.rectType, down, status, MIN);
			else if (up != -1)		swapRects(rect, mpiBundle.rectType, up, status, MAX);
		}
	}
}

// ====================================================== helper functions

// send rects between processes
void swapRects(Rect* rect, MPI_Datatype rectType, int neighbor, MPI_Status* status, char compare)
{
	Rect otherRect;

	MPI_Send(rect, 1, rectType, neighbor, 0, MPI_COMM_WORLD); // send rect
	MPI_Recv(&otherRect, 1, rectType, neighbor, 0, MPI_COMM_WORLD, status); // receive rect

	compareRects(rect, &otherRect, compare); // swap if needed
}

// compare rect areas and determine how to swap
void compareRects(Rect* rect, Rect* otherRect, char compare)
{
	float rectArea = calcRectArea(*rect);
	float otherRectArea = calcRectArea(*otherRect);

	switch (compare)
	{ // choose how to compare
		case MAX:
			if (rectArea < otherRectArea)
				setRectData(rect, otherRect);
			break;
		case MIN:
			if (rectArea > otherRectArea)
				setRectData(rect, otherRect);
			break;
	}
}

// sets rect data
void setRectData(Rect* rect, Rect* otherRect)
{
	rect->id = (*otherRect).id;
	rect->width = (*otherRect).width;
	rect->height = (*otherRect).height;
}

// calculate rect area
float calcRectArea(Rect rect)
{
	return rect.height * rect.width;
}

// ======================================================
