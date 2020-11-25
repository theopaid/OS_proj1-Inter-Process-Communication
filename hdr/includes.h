#ifndef _INCLUDES_H
#define _INCLUDES_H

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "sharedMemory.h"
#include "utils.h"
#include "userInterface.h"
#include "encoder.h"
#include "channel.h"

#define BLOCK_SIZE 4096

// Samaphore file names
#define P1toENC1 "/p1_enc1"
#define ENC1toP1 "/enc1_p1"
#define ENC1toCHAN "/enc1_chan"
#define CHANtoENC1 "/chan_enc1"
#define P2toENC2 "/p2_enc2"
#define ENC2toP2 "/enc2_p2"
#define ENC2toCHAN "/enc2_chan"
#define CHANtoENC2 "/chan_enc2"

// Shared memory file names
#define P1_ENC1_SHM "./sem_shm_files/p1_enc1_shm"
#define ENC1_CHAN_SHM "./sem_shm_files/enc1_chan_shm"
#define CHAN_ENC2_SHM "./sem_shm_files/chan_enc2_shm"
#define ENC2_P2_SHM "./sem_shm_files/enc2_p2_shm"

#endif
