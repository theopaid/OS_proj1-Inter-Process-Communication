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
#define P1toENC1prod "/p1_enc1_prod"
#define P1toENC1cons "/p1_enc1_cons"
#define ENC1toP1prod "/enc1_p1_prod"
#define ENC1toP1cons "/enc1_p1_cons"
#define ENC1toCHANprod "/enc1_chan_prod"
#define ENC1toCHANcons "/enc1_chan_cons"
#define CHANtoENC1prod "/chan_enc1_prod"
#define CHANtoENC1cons "/chan_enc1_cons"
#define P2toENC2prod "/p2_enc2_prod"
#define P2toENC2cons "/p2_enc2_cons"
#define ENC2toP2prod "/enc2_p2_prod"
#define ENC2toP2cons "/enc2_p2_cons"
#define ENC2toCHANprod "/enc2_chan_prod"
#define ENC2toCHANcons "/enc2_chan_cons"
#define CHANtoENC2prod "/chan_enc2_prod"
#define CHANtoENC2cons "/chan_enc2_cons"

// Shared memory file names
#define P1_ENC1_SHM "../sem_shm_files/p1_enc1_shm"
#define ENC1_CHAN_SHM "../sem_shm_files/enc1_chan_shm"
#define CHAN_ENC2_SHM "../sem_shm_files/chan_enc2_shm"
#define ENC2_P2_SHM "../sem_shm_files/enc2_p2_shm"

#endif
