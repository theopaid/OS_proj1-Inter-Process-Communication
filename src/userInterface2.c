#include "../hdr/includes.h"

int main(void)
{
    char *args[] = {NULL};
    int childId = fork();
    if (childId == 0)
    {
        if (execvp("./encoder2", args) == -1)
        {
            perrorexit("ripperoni");
        }
        return 0;
    }
    //wait(NULL);
    puts("P2: Connecting to the channel ...");

    return 0;
}