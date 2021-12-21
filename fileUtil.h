#pragma once

#include "sortUtil.h"

// ======================================================

Rect* readFromFile(const char* fileName, int* numOfRects);
void writeToFile(const char* fileName, int* results, int numOfRects);
void writeRowToFile(FILE* fp, int* results, int offset, int start, int end, int asc);
