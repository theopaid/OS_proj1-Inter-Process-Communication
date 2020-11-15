#ifndef _UTILS_H
#define _UTILS_H

#include <stdlib.h>

void *safe_malloc(size_t size);

void *safe_calloc(long elementsNum, size_t objectSize);

int validArgs(int argc, char *argv[]);

void getArgs(char **datasetX, char **datasetY, char *argv[]);

void printInsights();

#endif