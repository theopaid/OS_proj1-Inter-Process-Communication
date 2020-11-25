#include "../hdr/includes.h"

int main(void)
{
    char *args[] = {NULL};
    int childId = fork();
    if (childId == 0)
    {
        if (execvp("./encoder2", args) == -1)
        {
            perrorexit("Execvp on ENC2");
        }
        return 0;
    }
    //wait(NULL);
    puts("P2: Connecting to the channel ...");
    ConnectionDetails *connectionENC2 = setUpLinkingP2withENC2();
    interactWithENC2(connectionENC2);

    return 0;
}

void interactWithENC2(ConnectionDetails *connectionENC2)
{
    while (true)
    {
        sem_wait(connectionENC2->semConsumed); // wait for the ENC2 to write a message to P2
        if (strlen(connectionENC2->shmBlock) > 0)
        {
            //printf("[LOG] P2: Reading \"%s\"\n", connectionENC2->shmBlock);
            if (isMsgTerm(connectionENC2->shmBlock))
            {
                puts("Closing the chat ...");
                memset(connectionENC2->shmBlock, 0, BLOCK_SIZE);
                break;
            }
            printf("P1 says : %s\n", connectionENC2->shmBlock);
        }
        else
        {
            continue;
        }
        memset(connectionENC2->shmBlock, 0, BLOCK_SIZE);
        awaitUserInput(connectionENC2->shmBlock, "P2");
        sem_post(connectionENC2->semProduced); // notify ENC2 to read P2's message
    }
}

ConnectionDetails *setUpLinkingP2withENC2()
{
    sem_unlink(P2toENC2);
    sem_unlink(ENC2toP2);
    // setup semaphores for ENC2 connection
    sem_t *semP2toENC2 = sem_open(P2toENC2, O_CREAT, 0660, 0);
    if (semP2toENC2 == SEM_FAILED)
    {
        perrorexit("P2/sem_open/P2toENC2");
    }
    sem_t *semENC2toP2 = sem_open(ENC2toP2, O_CREAT, 0660, 0);
    if (semENC2toP2 == SEM_FAILED)
    {
        perrorexit("P2/sem_open/ENC2toP2");
    }

    // grab the shared memory block with ENC2
    char *shmBlock = attachMemoryBlock(ENC2_P2_SHM, BLOCK_SIZE);
    if (shmBlock == NULL)
    {
        perrorexit("Couldn't get p2 to enc2 shm block");
    }

    ConnectionDetails *connectionENC2 = safe_malloc(sizeof(ConnectionDetails));
    connectionENC2->semConsumed = semENC2toP2;
    connectionENC2->semProduced = semP2toENC2;
    connectionENC2->shmBlock = shmBlock;

    return connectionENC2;

    // sem_close(semP1);
    // sem_close(semENC1);
    // detachMemoryBlock(shmBlock);
}