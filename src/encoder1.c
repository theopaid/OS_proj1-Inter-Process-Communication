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
            perrorexit("ripperoni");
        }
        return 0;
    }
    //wait(NULL);
    puts("ENC1: Waiting for P2 to connect to the Channel ...");

    return 0;
}