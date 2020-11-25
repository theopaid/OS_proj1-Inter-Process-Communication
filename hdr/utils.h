#ifndef _UTILS_H
#define _UTILS_H

#include <stdlib.h>

void *safe_malloc(size_t size);

void *safe_calloc(long elementsNum, size_t objectSize);

int validArgs(int argc);

void getArgs(int *propability, char *argv[]);

void printInsights();

void perrorexit(char *message);

void signalP2Connection(ConnectionDetails *connectionP1, ConnectionDetails *connectionChannel);

void awaitP2Connection(ConnectionDetails *connectionENC1);

void awaitUserInput(char *shmBlock, char *username);

bool isMsgTerm(char *msg);

#endif