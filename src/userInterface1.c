#include "../hdr/includes.h"

int main(int argc, char **argv)
{
    if (!validArgs(argc))
    {
        printf("The arguments given are invalid. Try again.\n");
        return -1;
    }
    int propabiblity;
    getArgs(&propabiblity, argv);
    char *args[] = {
        "./encoder1",
        "-p",
        argv[2],
        NULL};
    int childId = fork();
    if (childId == 0)
    {
        if (execvp("./encoder1", args) == -1)
        {
            perrorexit("Execvp on ENC1");
        }
        return 0;
    }
    puts("Waiting for P2 to connect to the Channel ...");
    setUpLinking();

    return 0;
}

void setUpLinking()
{
    sem_unlink(P1toENC1);
    sem_unlink(ENC1toP1);
    // setup the semaphores
    sem_t *semP1 = sem_open(P1toENC1, O_CREAT, 0660, 0);
    if (semP1 == SEM_FAILED)
    {
        perrorexit("P1/sem_open/P1toENC1");
    }
    sem_t *semENC1 = sem_open(ENC1toP1, O_CREAT, 0660, 0);
    if (semENC1 == SEM_FAILED)
    {
        perrorexit("P1/sem_open/ENC1toP1");
    }

    // grab the shared memory block
    char *shmBlock = attachMemoryBlock(P1_ENC1_SHM, BLOCK_SIZE);
    if (shmBlock == NULL)
    {
        perrorexit("Couldn't get p1 to enc1 shm block");
    }
    ConnectionDetails *connectionENC1 = safe_malloc(sizeof(ConnectionDetails));
    connectionENC1->semConsumed = semENC1;
    connectionENC1->semProduced = semP1;
    connectionENC1->shmBlock = shmBlock;

    interactWithENC1(connectionENC1);

    sem_close(semP1);
    sem_close(semENC1);
    detachMemoryBlock(shmBlock);
    free(connectionENC1);
}

void interactWithENC1(ConnectionDetails *connectionENC1)
{
    awaitP2Connection(connectionENC1);
    puts("P2 connected! Say hello :)");
    // send first message
    awaitUserInput(connectionENC1->shmBlock, "P1");
    sem_post(connectionENC1->semProduced);
    if (isMsgTerm(connectionENC1->shmBlock))
    {
        puts("Closing the chat ...");
        return;
    }
    while (true)
    {
        sem_wait(connectionENC1->semConsumed); // wait for the ENC1 to write a message to P1
        if (strlen(connectionENC1->shmBlock) > 0)
        {
            if (isMsgTerm(connectionENC1->shmBlock))
            {
                puts("Closing the chat ...");
                memset(connectionENC1->shmBlock, 0, BLOCK_SIZE);
                break;
            }
            printf("P2 says : %s\n", connectionENC1->shmBlock);
        }
        else
        {
            continue;
        }
        memset(connectionENC1->shmBlock, 0, BLOCK_SIZE);
        awaitUserInput(connectionENC1->shmBlock, "P1");
        sem_post(connectionENC1->semProduced); // notify ENC1 to read P1's message
        if (isMsgTerm(connectionENC1->shmBlock))
        {
            puts("Closing the chat ...");
            break;
        }
    }
}