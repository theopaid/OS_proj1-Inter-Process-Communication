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
            perrorexit("Execvp on Channel");
        }
        return 0;
    }
    //wait(NULL);
    //puts("ENC1: Waiting for P2 to connect to the Channel ...");
    ConnectionDetails *connectionP1 = setUpLinkingWithP1();
    ConnectionDetails *connectionChannel = setUpLinkingWithChannel();
    signalP2Connection(connectionP1, connectionChannel);
    interactWithP1andChannel(connectionP1, connectionChannel);

    sem_close(connectionP1->semConsumed);
    sem_close(connectionP1->semProduced);
    detachMemoryBlock(connectionP1->shmBlock);
    free(connectionP1);
    sem_close(connectionChannel->semConsumed);
    sem_close(connectionChannel->semProduced);
    detachMemoryBlock(connectionChannel->shmBlock);
    free(connectionChannel);

    return 0;
}

void interactWithP1andChannel(ConnectionDetails *connectionP1, ConnectionDetails *connectionChannel)
{
    char msgFromP1[BLOCK_SIZE];
    zeroOutString(msgFromP1);
    char msgFromChannel[BLOCK_SIZE];
    zeroOutString(msgFromChannel);
    char clearTextMsg[BLOCK_SIZE];
    zeroOutString(clearTextMsg);
    unsigned char hashedString[BLOCK_SIZE];
    zeroOutHashedString(hashedString);
    unsigned char hashFromMsg[BLOCK_SIZE];
    zeroOutHashedString(hashedString);
    while (true)
    {
        sem_wait(connectionP1->semConsumed);
        if (strlen(connectionP1->shmBlock) > 0)
        {
            //printf("[LOG] ENC1: Reading \"%s\"\n", connectionP1->shmBlock);
            if (isMsgTerm(connectionP1->shmBlock))
            {

                // free and detach
                strncpy(connectionChannel->shmBlock, connectionP1->shmBlock, BLOCK_SIZE);
                sem_post(connectionChannel->semProduced);
                break;
            }
            strncpy(msgFromP1, connectionP1->shmBlock, BLOCK_SIZE);
            zeroOutString(connectionP1->shmBlock);
            zeroOutHashedString(hashedString);
            MD5(msgFromP1, sizeof(msgFromP1), hashedString);
            zeroOutString(connectionChannel->shmBlock);
            memcpy(connectionChannel->shmBlock, hashedString, MD5_DIGEST_LENGTH);
            memcpy((connectionChannel->shmBlock) + 16, msgFromP1, strlen(msgFromP1) + 1);
            sem_post(connectionChannel->semProduced);
        }
        else
        {
            continue;
        }
        while (true) // in order to resend the message as many times as needed
        {
            sem_wait(connectionChannel->semConsumed);
            if (strlen(connectionChannel->shmBlock) > 0)
            {
                //printf("[LOG] ENC1: Reading \"%s\"\n", connectionChannel->shmBlock);
                bool resendLastMsg = (strcmp(connectionChannel->shmBlock, "RESEND") == 0);
                if (isMsgTerm(connectionChannel->shmBlock))
                {
                    strncpy(connectionP1->shmBlock, connectionChannel->shmBlock, BLOCK_SIZE);
                    sem_post(connectionP1->semProduced);
                    break;
                }
                if (resendLastMsg)
                {
                    zeroOutHashedString(hashedString);
                    MD5(msgFromP1, sizeof(msgFromP1), hashedString);
                    zeroOutString(connectionChannel->shmBlock);
                    memcpy(connectionChannel->shmBlock, hashedString, MD5_DIGEST_LENGTH);
                    memcpy(connectionChannel->shmBlock + 16, msgFromP1, strlen(msgFromP1) + 1);
                    sem_post(connectionChannel->semProduced);
                }
                else
                {
                    // Compare the checksum for message validity
                    memcpy(msgFromChannel, connectionChannel->shmBlock, BLOCK_SIZE);
                    zeroOutString(connectionChannel->shmBlock);
                    zeroOutString(clearTextMsg);
                    strcpy(clearTextMsg, msgFromChannel + 16);
                    zeroOutHashedString(hashedString);
                    zeroOutHashedString(hashFromMsg);
                    MD5(clearTextMsg, sizeof(clearTextMsg), hashedString);
                    hashedString[16] = 0;
                    memcpy(hashFromMsg, msgFromChannel, 16);
                    hashFromMsg[16] = 0;
                    //printf("hashNew: %s\n", hashedString);
                    //printf("hashMsg: %s\n", hashFromMsg);
                    //printf("Allmsg : %s\n", msgFromChannel);
                    if (memcmp(hashedString, hashFromMsg, 16) != 0)
                    { // msg must be resent
                        //puts("hmm1");
                        // printf("resending: %s\n", clearTextMsg);
                        // printf("%s\n", hashedString);
                        //printf("%s\n", hashFromMsg);
                        // printf("og msg:\n %s\n", msgFromChannel);
                        strncpy(connectionChannel->shmBlock, "RESEND", BLOCK_SIZE);
                        sem_post(connectionChannel->semProduced);
                    }
                    else
                    {
                        // edit the message from Channel accordingly
                        strncpy(connectionP1->shmBlock, msgFromChannel + 16, BLOCK_SIZE);
                        sem_post(connectionP1->semProduced);
                        break;
                    }
                }
            }
            else
            {
                continue;
            }
        }
    }
}

ConnectionDetails *setUpLinkingWithP1()
{
    // setup semaphores for P1 connection
    sem_t *semP1toENC1 = sem_open(P1toENC1, O_CREAT, 0660, 0);
    if (semP1toENC1 == SEM_FAILED)
    {
        perrorexit("ENC1/sem_open/P1toENC1");
    }
    sem_t *semENC1toP1 = sem_open(ENC1toP1, O_CREAT, 0660, 0);
    if (semENC1toP1 == SEM_FAILED)
    {
        perrorexit("ENC1/sem_open/ENC1toP1");
    }

    // grab the shared memory block with P1
    char *shmBlock = attachMemoryBlock(P1_ENC1_SHM, BLOCK_SIZE);
    if (shmBlock == NULL)
    {
        perrorexit("Couldn't get p1 to enc1 shm block");
    }

    ConnectionDetails *connectionP1 = safe_malloc(sizeof(ConnectionDetails));
    connectionP1->semConsumed = semP1toENC1;
    connectionP1->semProduced = semENC1toP1;
    connectionP1->shmBlock = shmBlock;

    return connectionP1;
    //interactWithENC1(semP1, semENC1, shmBlock);

    // sem_close(semP1);
    // sem_close(semENC1);
    // detachMemoryBlock(shmBlock);
}

ConnectionDetails *setUpLinkingWithChannel()
{
    sem_unlink(ENC1toCHAN);
    sem_unlink(CHANtoENC1);
    // setup semaphores for Channel connection
    sem_t *semENC1toChannel = sem_open(ENC1toCHAN, O_CREAT, 0660, 0);
    if (semENC1toChannel == SEM_FAILED)
    {
        perrorexit("ENC1/sem_open/ENC1toChannel");
    }
    sem_t *semChanneltoENC1 = sem_open(CHANtoENC1, O_CREAT, 0660, 0);
    if (semChanneltoENC1 == SEM_FAILED)
    {
        perrorexit("ENC1/sem_open/ChanneltoENC1");
    }

    // grab the shared memory block with Channel
    char *shmBlock = attachMemoryBlock(ENC1_CHAN_SHM, BLOCK_SIZE);
    if (shmBlock == NULL)
    {
        perrorexit("Couldn't get enc1 to channel shm block");
    }

    ConnectionDetails *connectionChannel = safe_malloc(sizeof(ConnectionDetails));
    connectionChannel->semConsumed = semChanneltoENC1;
    connectionChannel->semProduced = semENC1toChannel;
    connectionChannel->shmBlock = shmBlock;

    return connectionChannel;

    // sem_close(semP1);
    // sem_close(semENC1);
    // detachMemoryBlock(shmBlock);
}