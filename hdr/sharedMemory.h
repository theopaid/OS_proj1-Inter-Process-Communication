#ifndef _SHARED_MEMORY_H
#define _SHARED_MEMORY_H

#include <stdbool.h>
#include <semaphore.h>

typedef struct ConnectionDetails
{
    sem_t *semProduced;
    sem_t *semConsumed;
    char *shmBlock;
} ConnectionDetails;

char *attachMemoryBlock(char *fileName, int size);
bool detachMemoryBlock(char *block);
bool destroyMemoryBlock(char *fileName);
void isBlockNull(char *block);
void semFailed(sem_t *sem, char *errorMsg);
void freeConnectionDet(ConnectionDetails *connDet);

#endif