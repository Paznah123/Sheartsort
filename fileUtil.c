#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#include "sortUtil.h"
#include "fileUtil.h"

// ======================================================

#define READ_FILE_NAME "rectangles.dat"
#define WRITE_FILE_NAME "result.dat"

#define BUFFER_LEN 255

// ====================================================== read data

Rect* readFromFile(const char* fileName, int* numOfRects)
{
	FILE* fp;
	char buffer[BUFFER_LEN];
	int count = 1;

	Rect rect = { 0 };
	Rect* rects = (Rect*)malloc(sizeof(Rect)); // allocate memory

	if (!rects) MPI_Abort(MPI_COMM_WORLD, 2);

	if ((fp = fopen(fileName, "r")) == NULL) // open file
	{
		printf("cannot open file %s for reading\n", fileName);
		MPI_Abort(MPI_COMM_WORLD, 2);
	}

	while (fgets(buffer, BUFFER_LEN, fp)) // read line by line
	{
		rects = (Rect*)realloc(rects, sizeof(Rect) * count); // allocate memory

		if (!rects) MPI_Abort(MPI_COMM_WORLD, 2);

		sscanf(buffer, "%d %f %f", &rect.id, &rect.height, &rect.width); // get rect data
		rects[count - 1] = rect; // insert rect to array

		count++;
	}
		
	*numOfRects = count - 1;

	printf("\nread %d rects from file\n", *numOfRects);

	return rects;
}

// ====================================================== write data

void writeToFile(const char* fileName, int* results, int numOfRects)
{

	FILE* fp;

	if ((fp = fopen(fileName, "w")) == NULL) {  // open file
		printf("cannot open file %s for writing\n", fileName);
		MPI_Abort(MPI_COMM_WORLD, 2);
	}

	int rowSize = sqrt(numOfRects);
	int start = 0, end = 0, asc = 0;

	for (int j = 0; j < rowSize; j++) {

		if (j % 2 == 0)		start = 0, end = rowSize - 1, asc = 1; // if even row ascend
		else				start = rowSize - 1, end = 0, asc = -1; // if odd row descend

		writeRowToFile(fp, results, j * rowSize, start, end, asc);
	}

	printf("wrote %d rects to file\n\n", numOfRects);

	fclose(fp);
}

void writeRowToFile(FILE* fp, int* results, int offset, int start, int end, int asc)
{
	int temp;

	for (int i = start * asc; i <= end; i++)
	{
		temp = *(results + (abs(i) + offset));
		fprintf(fp, "%d ", temp);
	}
}