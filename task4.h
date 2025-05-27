#ifndef TASK4_H
#define TASK4_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void freeLines(char** lines, int nrl);
char** readLines(const char* file, int* nrl);

void util_diff(const char* ref, const char* compare, int* D, int* I, int* R);

#endif