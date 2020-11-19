#ifndef _SHARED_MEMORY_H
#define _SHARED_MEMORY_H

#include <stdbool.h>
#include <semaphore.h>

#define BLOCK_SIZE 4096
#define FILENAME "../sem_shm_files/shmfile"

//semaphores filenames
#define SEM_PRODUCER_FNAME "/myproducer"
#define SEM_CONSUMER_FNAME "/myconsumer"

char *attachMemoryBlock(char *fileName, int size);
bool detachMemoryBlock(char *block);
bool destroyMemoryBlock(char *fileName);
void isBlockNull(char *block);
void semFailed(sem_t *sem, char *errorMsg);

#endif