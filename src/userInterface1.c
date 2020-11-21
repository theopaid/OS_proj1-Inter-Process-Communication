#include "../hdr/includes.h"

int validArgs(int argc, char *argv[]);

void getArgs(int *propability, char *argv[]);

int main(int argc, char **argv)
{
    if (!validArgs(argc, argv))
    {
        printf("The arguments given are invalid. Try again.\n");
        return -1;
    }
    int propabiblity;
    getArgs(&propabiblity, argv);
    char *args[] = {
        "./encoder1",
        argv[2],
    };
    execvp("./encoder1", args);
    puts("Waiting for P2 to connect to the Channel ...");

    return 0;
}