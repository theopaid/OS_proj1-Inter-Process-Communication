#ifndef _SHARED_MEMORY_H
#define _SHARED_MEMORY_H

#include <stdbool.h>
#include <semaphore.h>

char *attachMemoryBlock(char *fileName, int size);
bool detachMemoryBlock(char *block);
bool destroyMemoryBlock(char *fileName);
void isBlockNull(char *block);
void semFailed(sem_t *sem, char *errorMsg);

#endif