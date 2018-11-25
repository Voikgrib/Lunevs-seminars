//
// This library contain some useful things to make my code beautiful
//

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ USING LIBRARYS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/mman.h> 

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DEFINES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

#define EXIT_AND_SAY( _STRING_ )				\
			{									\
			perror("\n" _STRING_ "\n");		    \
			exit(EXIT_FAILURE);					\
			}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ GLOBAL CONSTANTS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

const int Shared_semaphore = 0xDEAD0EDA;
const int Buff_size = 32;
char *Buffer_pointer = NULL;
const int Sem_buffer_size = 16;
struct Sem_buf_struct sbuf[Sem_buffer_size] = {0};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ STRUCT DESCRIPTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

struct shared_buff
{
	char buff[Buff_size];
	int read_size;
	pid_t read_pid;
	pid_t write_pid;
};

struct shared_buff *Shared_mem = NULL;




