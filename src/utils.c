#include "../hdr/includes.h"

void *safe_malloc(size_t size)
{
    void *ptr = malloc(size);

    if (!ptr && (size > 0))
    {
        perror("malloc failed!");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

void *safe_calloc(long elementsNum, size_t size)
{
    void *ptr = calloc(elementsNum, size);

    if (!ptr && (size > 0) && (elementsNum > 0))
    {
        perror("calloc failed!");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

int validArgs(int argc)
{

    if (argc == 3)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void getArgs(int *propability, char *argv[])
{

    for (int i = 1; i < 3; i = i + 2)
    {
        if (!strcmp(argv[i], "-p"))
        {
            *propability = atoi(argv[i + 1]);
        }
    }
}

void printInsights()
{
    puts(" ");
    puts("|========== Insights ==========|");
    puts(" ");
    //printHashingBenchmarks();
    //printMatchesCount();
    //printVisitedSpecNodesCount();
    //printExecTime();
    puts(" ");
    puts("|==============================|");
}

void perrorexit(char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

void signalP2Connection(ConnectionDetails *connectionP1, ConnectionDetails *connectionChannel)
{
    awaitP2Connection(connectionChannel);
    strncpy(connectionP1->shmBlock, "P2_CONNECTED", BLOCK_SIZE);
    sem_post(connectionP1->semProduced);
}

void awaitP2Connection(ConnectionDetails *connectionENC1)
{
    while (true)
    {
        sem_wait(connectionENC1->semConsumed); // wait for the ENC1 to write a message to P1
        if (strlen(connectionENC1->shmBlock) > 0)
        {
            //printf("[LOG] P1: Reading\"%s\"\n", connectionENC1->shmBlock);
            bool p2_connected = (strcmp(connectionENC1->shmBlock, "P2_CONNECTED") == 0);

            memset(connectionENC1->shmBlock, 0, BLOCK_SIZE);
            if (p2_connected)
            {
                break;
            }
        }
    }
}

void awaitUserInput(char *shmBlock, char *username)
{
    char line[BLOCK_SIZE];
    while (true)
    {
        memset(line, 0, BLOCK_SIZE);
        printf("%s: ", username);
        if (fgets(line, sizeof(line), stdin))
        {
            if (line[0] != '\n' && line[0] != ' ')
            {
                // safe input to use
                line[strcspn(line, "\r\n")] = 0;
                break;
            }
        }
    }
    strncpy(shmBlock, line, BLOCK_SIZE);
}

bool isMsgTerm(char *msg)
{
    bool done = (strcmp(msg, "TERM") == 0);
    return done;
}

void zeroOutString(char *msg)
{
    memset(msg, 0, BLOCK_SIZE);
}

void zeroOutHashedString(unsigned char *msg)
{
    memset(msg, 0, BLOCK_SIZE);
}