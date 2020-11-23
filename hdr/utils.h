#ifndef _UTILS_H
#define _UTILS_H

#include <stdlib.h>

void *safe_malloc(size_t size);

void *safe_calloc(long elementsNum, size_t objectSize);

int validArgs(int argc);

void getArgs(int *propability, char *argv[]);

void printInsights();

void perrorexit(char *message);

#endif