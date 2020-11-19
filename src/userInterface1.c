#include "../hdr/includes.h"

int main(void)
{
    sem_t *sem_prod = sem_open(SEM_PRODUCER_FNAME, O_CREAT, 0660, 0);
    semFailed(sem_prod, "sem_open/producer");

    sem_t *sem_cons = sem_open(SEM_CONSUMER_FNAME, O_CREAT, 0660, 0);
    semFailed(sem_cons, "sem_open/consumer");

    char *str = attachMemoryBlock(FILENAME, BLOCK_SIZE);
    isBlockNull(str);

    for (int i = 0; i < 10; i++)
    {
        sem_wait(sem_cons);
        strncpy(str, "kalispera sta paidia", BLOCK_SIZE);
        sem_post(sem_prod);
    }

    sem_close(sem_prod);
    sem_close(sem_cons);
    detachMemoryBlock(str);

    return 0;
}