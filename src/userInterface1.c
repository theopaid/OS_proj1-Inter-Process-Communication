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
            perrorexit("ripperoni");
        }
        return 0;
    }
    //wait(NULL);
    puts("P1: Waiting for P2 to connect to the Channel ...");

    return 0;
}