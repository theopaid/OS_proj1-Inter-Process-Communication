#include "../hdr/includes.h"

#define IPC_RESULT_ERROR (-1)

// static, so that it's visible inside this file
static int getSharedBlock(char *fileName, int size)
{
    key_t key;

    // The key is attached to a filename, so that other programs can access it
    key = ftok(fileName, 0);
    if (key == IPC_RESULT_ERROR)
    {
        return IPC_RESULT_ERROR;
    }

    return shmget(key, size, 0644 | IPC_CREAT);
}

char *attachMemoryBlock(char *fileName, int size)
{
    int sharedBlockId = getSharedBlock(fileName, size);
    char *result;

    if (sharedBlockId == IPC_RESULT_ERROR)
    {
        return NULL;
    }

    result = shmat(sharedBlockId, (void *)0, 0);
    if (result == (char *)IPC_RESULT_ERROR)
    {
        return NULL;
    }

    return result;
}

bool detachMemoryBlock(char *block)
{
    return (shmdt(block) != IPC_RESULT_ERROR);
}

bool destroyMemoryBlock(char *fileName)
{
    int sharedBlockId = getSharedBlock(fileName, 0);

    if (sharedBlockId == IPC_RESULT_ERROR)
    {
        return NULL;
    }

    return (shmctl(sharedBlockId, IPC_RMID, NULL) != IPC_RESULT_ERROR);
}

void isBlockNull(char *block)
{
    if (block == NULL)
    {
        printf("ERROR: Couldn't get block\n");
        exit(-1);
    }
}

void semFailed(sem_t *sem, char *errorMsg)
{
    if (sem == SEM_FAILED)
    {
        perror(errorMsg);
        exit(EXIT_FAILURE);
    }
}

int main(void)
{
    return 0;
}