#include "../hdr/includes.h"

int main(void)
{
    ConnectionDetails *connectionP2 = setUpLinkingENC2toP2();
    ConnectionDetails *connectionChannel = setUpLinkingENC2toChannel();
    strncpy(connectionChannel->shmBlock, "P2_CONNECTED", BLOCK_SIZE);
    sem_post(connectionChannel->semProduced); // signal all processed that P2 connected

    interactWithP2andChannel(connectionP2, connectionChannel);

    sem_close(connectionP2->semConsumed);
    sem_close(connectionP2->semProduced);
    detachMemoryBlock(connectionP2->shmBlock);
    free(connectionP2);
    sem_close(connectionChannel->semConsumed);
    sem_close(connectionChannel->semProduced);
    detachMemoryBlock(connectionChannel->shmBlock);
    free(connectionChannel);

    return 0;
}

void interactWithP2andChannel(ConnectionDetails *connectionP2, ConnectionDetails *connectionChannel)
{
    char msgFromChannel[BLOCK_SIZE];
    zeroOutString(msgFromChannel);
    char msgFromP2[BLOCK_SIZE];
    zeroOutString(msgFromP2);
    char clearTextMsg[BLOCK_SIZE];
    zeroOutString(clearTextMsg);
    unsigned char hashedString[BLOCK_SIZE];
    zeroOutHashedString(hashedString);
    unsigned char hashFromMsg[BLOCK_SIZE];
    zeroOutHashedString(hashedString);
    bool shouldBreak = false;
    while (true)
    {
        if (shouldBreak)
        {
            break;
        }
        while (true) // in order to resend the message as many times as needed
        {
            sem_wait(connectionChannel->semConsumed);
            if (strlen(connectionChannel->shmBlock) > 0)
            {
                bool resendLastMsg = (strcmp(connectionChannel->shmBlock, "RESEND") == 0);
                if (isMsgTerm(connectionChannel->shmBlock))
                {
                    strncpy(connectionP2->shmBlock, connectionChannel->shmBlock, BLOCK_SIZE);
                    sem_post(connectionP2->semProduced);
                    shouldBreak = true;
                    break;
                }
                if (resendLastMsg)
                {
                    zeroOutHashedString(hashedString);
                    MD5(msgFromP2, sizeof(msgFromP2), hashedString);
                    zeroOutString(connectionChannel->shmBlock);
                    memcpy(connectionChannel->shmBlock, hashedString, MD5_DIGEST_LENGTH);
                    memcpy(connectionChannel->shmBlock + 16, msgFromP2, strlen(msgFromP2) + 1);
                    sem_post(connectionChannel->semProduced);
                }
                else
                {
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
                    if (memcmp(hashedString, hashFromMsg, 16) != 0)
                    { // msg must be resent
                        strncpy(connectionChannel->shmBlock, "RESEND", BLOCK_SIZE);
                        sem_post(connectionChannel->semProduced);
                    }
                    else
                    {
                        // Send only the clear text message to P2
                        strncpy(connectionP2->shmBlock, msgFromChannel + 16, BLOCK_SIZE);
                        sem_post(connectionP2->semProduced);
                        break;
                    }
                }
            }
            else
            {
                continue;
            }
        }

        sem_wait(connectionP2->semConsumed);
        if (strlen(connectionP2->shmBlock) > 0)
        {
            if (isMsgTerm(connectionP2->shmBlock))
            {
                strncpy(connectionChannel->shmBlock, connectionP2->shmBlock, BLOCK_SIZE);
                sem_post(connectionChannel->semProduced);
                break;
            }
            strncpy(msgFromP2, connectionP2->shmBlock, BLOCK_SIZE);
            zeroOutString(connectionP2->shmBlock);
            zeroOutHashedString(hashedString);
            MD5(msgFromP2, sizeof(msgFromP2), hashedString);
            zeroOutString(connectionChannel->shmBlock);
            memcpy(connectionChannel->shmBlock, hashedString, MD5_DIGEST_LENGTH);
            memcpy(connectionChannel->shmBlock + 16, msgFromP2, strlen(msgFromP2) + 1);
            sem_post(connectionChannel->semProduced);
        }
        else
        {
            continue;
        }
    }
}

ConnectionDetails *setUpLinkingENC2toP2()
{
    // setup semaphores for P2 connection
    sem_t *semP2toENC2 = sem_open(P2toENC2, O_CREAT, 0660, 0);
    if (semP2toENC2 == SEM_FAILED)
    {
        perrorexit("ENC2/sem_open/P2toENC2");
    }
    sem_t *semENC2toP2 = sem_open(ENC2toP2, O_CREAT, 0660, 0);
    if (semENC2toP2 == SEM_FAILED)
    {
        perrorexit("ENC2/sem_open/ENC2toP2");
    }

    // grab the shared memory block with P1
    char *shmBlock = attachMemoryBlock(ENC2_P2_SHM, BLOCK_SIZE);
    if (shmBlock == NULL)
    {
        perrorexit("Couldn't get p2 to enc2 shm block");
    }

    ConnectionDetails *connectionP2 = safe_malloc(sizeof(ConnectionDetails));
    connectionP2->semConsumed = semP2toENC2;
    connectionP2->semProduced = semENC2toP2;
    connectionP2->shmBlock = shmBlock;

    return connectionP2;
}

ConnectionDetails *setUpLinkingENC2toChannel()
{
    // setup semaphores for Channel connection
    sem_t *semENC2toChannel = sem_open(ENC2toCHAN, O_CREAT, 0660, 0);
    if (semENC2toChannel == SEM_FAILED)
    {
        perrorexit("ENC2/sem_open/ENC2toChannel");
    }
    sem_t *semChanneltoENC2 = sem_open(CHANtoENC2, O_CREAT, 0660, 0);
    if (semChanneltoENC2 == SEM_FAILED)
    {
        perrorexit("ENC2/sem_open/ChanneltoENC2");
    }

    // grab the shared memory block with Channel
    char *shmBlock = attachMemoryBlock(CHAN_ENC2_SHM, BLOCK_SIZE);
    if (shmBlock == NULL)
    {
        perrorexit("Couldn't get enc2 to channel shm block");
    }

    ConnectionDetails *connectionChannel = safe_malloc(sizeof(ConnectionDetails));
    connectionChannel->semConsumed = semChanneltoENC2;
    connectionChannel->semProduced = semENC2toChannel;
    connectionChannel->shmBlock = shmBlock;

    return connectionChannel;
}