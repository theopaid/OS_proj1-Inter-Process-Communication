#include "../hdr/includes.h"

int main(int argc, char **argv)
{
    int propabiblity;
    getArgs(&propabiblity, argv);
    //printf("CHANNEL: propability is %d %% \n", propabiblity);
    //wait(NULL);
    //puts("CHANNEL: Waiting for P2 to connect to the Channel ...");
    ConnectionDetails *connectionENC1 = setUpLinkingWithENC1();
    ConnectionDetails *connectionENC2 = setUpLinkingWithENC2();
    signalP2Connection(connectionENC1, connectionENC2);
    interactWithENC1andENC2(connectionENC1, connectionENC2);

    return 0;
}

void interactWithENC1andENC2(ConnectionDetails *connectionENC1, ConnectionDetails *connectionENC2)
{
    while (true)
    {
        sem_wait(connectionENC1->semConsumed);
        if (strlen(connectionENC1->shmBlock) > 0)
        {
            //printf("[LOG] Channel: Reading \"%s\"\n", connectionENC1->shmBlock);
            if (isMsgTerm(connectionENC1->shmBlock))
            {
                // free and detach
            }
            char msgFromENC1[BLOCK_SIZE];
            strncpy(msgFromENC1, connectionENC1->shmBlock, BLOCK_SIZE);
            // edit the message from ENC1 accordingly
            memset(connectionENC1->shmBlock, 0, BLOCK_SIZE);
            strncpy(connectionENC2->shmBlock, msgFromENC1, BLOCK_SIZE);
            sem_post(connectionENC2->semProduced);
        }
        else
        {
            continue;
        }
        sem_wait(connectionENC2->semConsumed);
        if (strlen(connectionENC2->shmBlock) > 0)
        {
            //printf("[LOG] Channel: Reading \"%s\"\n", connectionENC2->shmBlock);
            if (isMsgTerm(connectionENC2->shmBlock))
            {
                // free and detach
            }
            char msgFromENC2[BLOCK_SIZE];
            strncpy(msgFromENC2, connectionENC2->shmBlock, BLOCK_SIZE);
            // edit the message from Channel accordingly
            memset(connectionENC2->shmBlock, 0, BLOCK_SIZE);
            strncpy(connectionENC1->shmBlock, msgFromENC2, BLOCK_SIZE);
            sem_post(connectionENC1->semProduced);
        }
        else
        {
            continue;
        }
    }
}

ConnectionDetails *setUpLinkingWithENC1()
{
    // setup semaphores for ENC1 connection
    sem_t *semENC1toChannel = sem_open(ENC1toCHAN, O_CREAT, 0660, 0);
    if (semENC1toChannel == SEM_FAILED)
    {
        perrorexit("Channel/sem_open/ENC1toChannel");
    }
    sem_t *semChanneltoENC1 = sem_open(CHANtoENC1, O_CREAT, 0660, 0);
    if (semChanneltoENC1 == SEM_FAILED)
    {
        perrorexit("Channel/sem_open/ChanneltoENC1");
    }

    // grab the shared memory block with ENC1
    char *shmBlock = attachMemoryBlock(ENC1_CHAN_SHM, BLOCK_SIZE);
    if (shmBlock == NULL)
    {
        perrorexit("Couldn't get enc1 to channel shm block");
    }

    ConnectionDetails *connectionENC1 = safe_malloc(sizeof(ConnectionDetails));
    connectionENC1->semProduced = semChanneltoENC1;
    connectionENC1->semConsumed = semENC1toChannel;
    connectionENC1->shmBlock = shmBlock;

    return connectionENC1;

    // sem_close(semP1);
    // sem_close(semENC1);
    // detachMemoryBlock(shmBlock);
}

ConnectionDetails *setUpLinkingWithENC2()
{
    sem_unlink(CHANtoENC2);
    sem_unlink(ENC2toCHAN);
    // setup semaphores for ENC2 connection
    sem_t *semENC2toChannel = sem_open(ENC2toCHAN, O_CREAT, 0660, 0);
    if (semENC2toChannel == SEM_FAILED)
    {
        perrorexit("Channel/sem_open/ENC2toChannel");
    }
    sem_t *semChanneltoENC2 = sem_open(CHANtoENC2, O_CREAT, 0660, 0);
    if (semChanneltoENC2 == SEM_FAILED)
    {
        perrorexit("Channel/sem_open/ChanneltoENC2");
    }

    // grab the shared memory block with ENC2
    char *shmBlock = attachMemoryBlock(CHAN_ENC2_SHM, BLOCK_SIZE);
    if (shmBlock == NULL)
    {
        perrorexit("Couldn't get enc1 to channel shm block");
    }

    ConnectionDetails *connectionENC2 = safe_malloc(sizeof(ConnectionDetails));
    connectionENC2->semConsumed = semENC2toChannel;
    connectionENC2->semProduced = semChanneltoENC2;
    connectionENC2->shmBlock = shmBlock;

    return connectionENC2;

    // sem_close(semP1);
    // sem_close(semENC1);
    // detachMemoryBlock(shmBlock);
}