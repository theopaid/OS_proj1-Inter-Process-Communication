#include "../hdr/includes.h"

int main(void)
{
    //puts("ENC2: Connecting to the channel ...");
    ConnectionDetails *connectionP2 = setUpLinkingENC2toP2();
    ConnectionDetails *connectionChannel = setUpLinkingENC2toChannel();
    strncpy(connectionChannel->shmBlock, "P2_CONNECTED", BLOCK_SIZE);
    sem_post(connectionChannel->semProduced);

    interactWithP2andChannel(connectionP2, connectionChannel);

    return 0;
}

void interactWithP2andChannel(ConnectionDetails *connectionP2, ConnectionDetails *connectionChannel)
{
    while (true)
    {
        sem_wait(connectionChannel->semConsumed);
        if (strlen(connectionChannel->shmBlock) > 0)
        {
            //printf("[LOG] ENC2: Reading \"%s\"\n", connectionChannel->shmBlock);
            if (isMsgTerm(connectionChannel->shmBlock))
            {
                // free and detach
            }
            char msgFromChannel[BLOCK_SIZE];
            strncpy(msgFromChannel, connectionChannel->shmBlock, BLOCK_SIZE);
            memset(connectionChannel->shmBlock, 0, BLOCK_SIZE);
            // edit the message from Channel accordingly
            strncpy(connectionP2->shmBlock, msgFromChannel, BLOCK_SIZE);
            sem_post(connectionP2->semProduced);
        }
        else
        {
            continue;
        }
        sem_wait(connectionP2->semConsumed);
        if (strlen(connectionP2->shmBlock) > 0)
        {
            //printf("[LOG] ENC2: Reading \"%s\"\n", connectionP2->shmBlock);
            if (isMsgTerm(connectionP2->shmBlock))
            {
                // free and detach
            }
            char msgFromP2[BLOCK_SIZE];
            strncpy(msgFromP2, connectionP2->shmBlock, BLOCK_SIZE);
            memset(connectionP2->shmBlock, 0, BLOCK_SIZE);
            // edit the message from Channel accordingly
            strncpy(connectionChannel->shmBlock, msgFromP2, BLOCK_SIZE);
            sem_post(connectionChannel->semProduced);
        }
        else
        {
            continue;
        }
    }
}

ConnectionDetails *setUpLinkingENC2toP2()
{
    // setup semaphores for P2 connection
    sem_t *semP2toENC2 = sem_open(P2toENC2, O_CREAT, 0660, 0);
    if (semP2toENC2 == SEM_FAILED)
    {
        perrorexit("ENC2/sem_open/P2toENC2");
    }
    sem_t *semENC2toP2 = sem_open(ENC2toP2, O_CREAT, 0660, 0);
    if (semENC2toP2 == SEM_FAILED)
    {
        perrorexit("ENC2/sem_open/ENC2toP2");
    }

    // grab the shared memory block with P1
    char *shmBlock = attachMemoryBlock(ENC2_P2_SHM, BLOCK_SIZE);
    if (shmBlock == NULL)
    {
        perrorexit("Couldn't get p2 to enc2 shm block");
    }

    ConnectionDetails *connectionP2 = safe_malloc(sizeof(ConnectionDetails));
    connectionP2->semConsumed = semP2toENC2;
    connectionP2->semProduced = semENC2toP2;
    connectionP2->shmBlock = shmBlock;

    return connectionP2;

    // sem_close(semP1);
    // sem_close(semENC1);
    // detachMemoryBlock(shmBlock);
}

ConnectionDetails *setUpLinkingENC2toChannel()
{
    // setup semaphores for Channel connection
    sem_t *semENC2toChannel = sem_open(ENC2toCHAN, O_CREAT, 0660, 0);
    if (semENC2toChannel == SEM_FAILED)
    {
        perrorexit("ENC2/sem_open/ENC2toChannel");
    }
    sem_t *semChanneltoENC2 = sem_open(CHANtoENC2, O_CREAT, 0660, 0);
    if (semChanneltoENC2 == SEM_FAILED)
    {
        perrorexit("ENC2/sem_open/ChanneltoENC2");
    }

    // grab the shared memory block with Channel
    char *shmBlock = attachMemoryBlock(CHAN_ENC2_SHM, BLOCK_SIZE);
    if (shmBlock == NULL)
    {
        perrorexit("Couldn't get enc2 to channel shm block");
    }

    ConnectionDetails *connectionChannel = safe_malloc(sizeof(ConnectionDetails));
    connectionChannel->semConsumed = semChanneltoENC2;
    connectionChannel->semProduced = semENC2toChannel;
    connectionChannel->shmBlock = shmBlock;

    return connectionChannel;

    // sem_close(semP1);
    // sem_close(semENC1);
    // detachMemoryBlock(shmBlock);
}