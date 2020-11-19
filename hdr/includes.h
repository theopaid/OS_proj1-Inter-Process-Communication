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
#include "main.h"
#include "userInterface.h"
#include "encoder.h"
#include "channel.h"

#endif
