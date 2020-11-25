#include "../hdr/includes.h"

int main(int argc, char **argv)
{
    int propabiblity;
    getArgs(&propabiblity, argv);
    char *args[] = {
        "./channel",
        "-p",
        argv[2],
        NULL};
    int childId = fork();
    if (childId == 0)
    {
        if (execvp("./channel", args) == -1)
        {
            perrorexit("Execvp on Channel");
        }
        return 0;
    }
    //wait(NULL);
    //puts("ENC1: Waiting for P2 to connect to the Channel ...");
    ConnectionDetails *connectionP1 = setUpLinkingWithP1();
    ConnectionDetails *connectionChannel = setUpLinkingWithChannel();
    signalP2Connection(connectionP1, connectionChannel);
    interactWithP1andChannel(connectionP1, connectionChannel);

    return 0;
}

void interactWithP1andChannel(ConnectionDetails *connectionP1, ConnectionDetails *connectionChannel)
{
    while (true)
    {
        sem_wait(connectionP1->semConsumed);
        if (strlen(connectionP1->shmBlock) > 0)
        {
            //printf("[LOG] ENC1: Reading \"%s\"\n", connectionP1->shmBlock);
            if (isMsgTerm(connectionP1->shmBlock))
            {
                // free and detach
            }
            char msgFromP1[BLOCK_SIZE];
            strncpy(msgFromP1, connectionP1->shmBlock, BLOCK_SIZE);
            memset(connectionP1->shmBlock, 0, BLOCK_SIZE);
            // edit the message from P1 accordingly
            strncpy(connectionChannel->shmBlock, msgFromP1, BLOCK_SIZE);
            sem_post(connectionChannel->semProduced);
        }
        else
        {
            continue;
        }
        sem_wait(connectionChannel->semConsumed);
        if (strlen(connectionChannel->shmBlock) > 0)
        {
            //printf("[LOG] ENC1: Reading \"%s\"\n", connectionChannel->shmBlock);
            if (isMsgTerm(connectionChannel->shmBlock))
            {
                // free and detach
            }
            char msgFromChannel[BLOCK_SIZE];
            strncpy(msgFromChannel, connectionChannel->shmBlock, BLOCK_SIZE);
            // edit the message from Channel accordingly
            memset(connectionChannel->shmBlock, 0, BLOCK_SIZE);
            strncpy(connectionP1->shmBlock, msgFromChannel, BLOCK_SIZE);
            sem_post(connectionP1->semProduced);
        }
        else
        {
            continue;
        }
    }
}

ConnectionDetails *setUpLinkingWithP1()
{
    // setup semaphores for P1 connection
    sem_t *semP1toENC1 = sem_open(P1toENC1, O_CREAT, 0660, 0);
    if (semP1toENC1 == SEM_FAILED)
    {
        perrorexit("ENC1/sem_open/P1toENC1");
    }
    sem_t *semENC1toP1 = sem_open(ENC1toP1, O_CREAT, 0660, 0);
    if (semENC1toP1 == SEM_FAILED)
    {
        perrorexit("ENC1/sem_open/ENC1toP1");
    }

    // grab the shared memory block with P1
    char *shmBlock = attachMemoryBlock(P1_ENC1_SHM, BLOCK_SIZE);
    if (shmBlock == NULL)
    {
        perrorexit("Couldn't get p1 to enc1 shm block");
    }

    ConnectionDetails *connectionP1 = safe_malloc(sizeof(ConnectionDetails));
    connectionP1->semConsumed = semP1toENC1;
    connectionP1->semProduced = semENC1toP1;
    connectionP1->shmBlock = shmBlock;

    return connectionP1;
    //interactWithENC1(semP1, semENC1, shmBlock);

    // sem_close(semP1);
    // sem_close(semENC1);
    // detachMemoryBlock(shmBlock);
}

ConnectionDetails *setUpLinkingWithChannel()
{
    sem_unlink(ENC1toCHAN);
    sem_unlink(CHANtoENC1);
    // setup semaphores for Channel connection
    sem_t *semENC1toChannel = sem_open(ENC1toCHAN, O_CREAT, 0660, 0);
    if (semENC1toChannel == SEM_FAILED)
    {
        perrorexit("ENC1/sem_open/ENC1toChannel");
    }
    sem_t *semChanneltoENC1 = sem_open(CHANtoENC1, O_CREAT, 0660, 0);
    if (semChanneltoENC1 == SEM_FAILED)
    {
        perrorexit("ENC1/sem_open/ChanneltoENC1");
    }

    // grab the shared memory block with Channel
    char *shmBlock = attachMemoryBlock(ENC1_CHAN_SHM, BLOCK_SIZE);
    if (shmBlock == NULL)
    {
        perrorexit("Couldn't get enc1 to channel shm block");
    }

    ConnectionDetails *connectionChannel = safe_malloc(sizeof(ConnectionDetails));
    connectionChannel->semConsumed = semChanneltoENC1;
    connectionChannel->semProduced = semENC1toChannel;
    connectionChannel->shmBlock = shmBlock;

    return connectionChannel;

    // sem_close(semP1);
    // sem_close(semENC1);
    // detachMemoryBlock(shmBlock);
}