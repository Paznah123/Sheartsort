#pragma once

#include "models.h"

// ======================================================

int shearSort(Rect rect, Bundle mpiBundle);
void oddEvenSortRow(Rect* value, int left, int right, Bundle mpiBundle, MPI_Status* status);
void oddEvenSortCol(Rect* value, int up, int down, Bundle mpiBundle, MPI_Status* status);
void swapRects(Rect* rect, MPI_Datatype rectType, int neighbor, MPI_Status* status, char minMax);
void compareRects(Rect* rect, Rect* otherRect, char minMax);
void setRectData(Rect* rect, Rect* otherRect);
float calcRectArea(Rect rect);
