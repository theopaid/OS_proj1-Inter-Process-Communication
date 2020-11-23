#include "../hdr/includes.h"

int main(int argc, char **argv)
{
    int propabiblity;
    getArgs(&propabiblity, argv);
    printf("CHANNEL: propability is %d\n", propabiblity);
    //wait(NULL);
    puts("CHANNEL: Waiting for P2 to connect to the Channel ...");

    return 0;
}