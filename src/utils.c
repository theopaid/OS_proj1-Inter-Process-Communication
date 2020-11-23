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