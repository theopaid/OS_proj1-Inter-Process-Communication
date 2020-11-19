#include "../hdr/includes.h"

int main(void)
{
    //setup semaphores
    //sem_unlink(SEM_CONSUMER_FNAME);
    //sem_unlink(SEM_PRODUCER_FNAME);

    sem_t *sem_prod = sem_open(SEM_PRODUCER_FNAME, O_CREAT, 0660, 0);
    semFailed(sem_prod, "sem_open/producer");

    sem_t *sem_cons = sem_open(SEM_CONSUMER_FNAME, O_CREAT, 0660, 1);
    semFailed(sem_cons, "sem_open/consumer");

    char *str = attachMemoryBlock(FILENAME, BLOCK_SIZE);
    isBlockNull(str);
    while (true)
    {
        sem_wait(sem_prod);
        if (strlen(str) > 0)
        {
            bool done = (strcmp(str, "TERM") == 0);
            if (done)
            {
                break;
            }
            puts(str);
            str[0] = 0;
        }
        sem_post(sem_cons);
    }

    sem_close(sem_prod);
    sem_close(sem_cons);
    detachMemoryBlock(str);
    destroyMemoryBlock(FILENAME);

    return 0;
}