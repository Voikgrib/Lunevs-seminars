// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ USING LIBRARYS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DEFINES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// semophores cur_sem

#define MAX_SEMS 	10

#define BUFF_SIZE 	32


#define EXIT_AND_SAY( _STRING_ )						\
		{												\
			perror("\n" _STRING_ "\n");		    		\
			exit(EXIT_FAILURE);							\
		}


#define SEM_PUSH_OP( _WHAT_ , _OPERATION_ , _FLAG_ )		\
		if(1 == 1)										\
		{												\
			semophores[cur_sem].sem_num = _WHAT_;		\
			semophores[cur_sem].sem_op  = _OPERATION_;	\
			semophores[cur_sem].sem_flg = _FLAG_;		\
			cur_sem++;									\
		}

#define SEM_MAKE_OPS(...)										\
		if(1 == 1)												\
		{														\
			if(semop(sem_id, semophores, cur_sem) == -1)		\
				EXIT_AND_SAY(">>> ERROR: Semop return -1!");	\
																\
			cur_sem = 0;										\
		}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ GLOBAL CONSTANTS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

const int Shared_semaphore = 0xDEADEDB0;
char *Buffer_pointer = NULL;

enum semnum_t {
	EMPTY_SEM,
	MUTEX_SEM,
    AMOUNT_SEM
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ FUNTIONS DECLARE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ // 

int send(void *shm_ptr, int sem_id, const char *name_of_file);
int recive(void *shm_ptr, int sem_id);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MAIN ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
int main(int argc, char* argv[])
{
    if(!(argc == 2 || argc == 1))
		EXIT_AND_SAY(">>> ERROR: Invalid amount of args!");

	int shm_id = -1; 
	int sem_id = -1;

    sem_id = semget(Shared_semaphore, AMOUNT_SEM, IPC_CREAT | 0666);
	if(sem_id == -1)
		EXIT_AND_SAY(">>> ERROR: Can't get semaphore");

    shm_id = shmget(Shared_semaphore, sizeof(ssize_t) + BUFF_SIZE, IPC_CREAT|0666);
	if(sem_id == -1)
		EXIT_AND_SAY(">>> ERROR: Can't get shared memory ID");

	void *shm_mem = shmat(shm_id, NULL, 0);
	if(shm_mem == ((void*)(-1)))
		EXIT_AND_SAY(">>> ERROR: Can't get shared memory");


	if(argc == 2)
		send(shm_mem, sem_id, argv[1]);
	else
		recive(shm_mem, sem_id);

	return 0;
}

//
//	Sender
//
int send(void *shm_ptr, int sem_id, const char *name_of_file)
{
	struct sembuf semophores[MAX_SEMS] = {};
	int cur_sem = 0;
	int fd = -1;
	ssize_t read_amount = 1;

	fd = open(name_of_file, O_RDONLY);

	if(fd == -1)
		EXIT_AND_SAY(">>> ERROR: File mistery disapiared or never existed (fd = -1)");

	// Sem senders sync

	while(read_amount > 0)
	{
		// Sem shm sync (+)

        SEM_PUSH_OP(EMPTY_SEM, 0, 0);				// wait(empty == 0)
        SEM_PUSH_OP(MUTEX_SEM, 0, 0);				// wait(mutex == 0)
		SEM_PUSH_OP(MUTEX_SEM, 1, SEM_UNDO);		// Mutex++;
		SEM_MAKE_OPS();

		read_amount = read(fd, shm_ptr + sizeof(ssize_t), BUFF_SIZE);
		write(STDOUT_FILENO, shm_ptr + sizeof(ssize_t), read_amount);
		*((ssize_t*)shm_ptr) = read_amount;

        SEM_PUSH_OP(EMPTY_SEM, 1, 0);				// Empty++;
        SEM_PUSH_OP(MUTEX_SEM, -1, SEM_UNDO);		// Mutex--;
        SEM_MAKE_OPS();
	}

	return 0;
}

//
//	Reciver
//
int recive(void *shm_ptr, int sem_id)
{
	struct sembuf semophores[MAX_SEMS] = {};
	int cur_sem = 0;
	ssize_t read_amount = 1;

	// recive sync

    SEM_PUSH_OP(EMPTY_SEM,  1, SEM_UNDO);		// Empty++;
	SEM_PUSH_OP(EMPTY_SEM, -1, 0);				// Empty--;
	SEM_MAKE_OPS();

	while(read_amount > 0)
	{
		//  shm sync (+)

		SEM_PUSH_OP(EMPTY_SEM, -1, 0);			// Empty--;
        SEM_PUSH_OP(MUTEX_SEM, 0, 0);			// wait(mutex == 0)
        SEM_PUSH_OP(MUTEX_SEM, 1, SEM_UNDO);	// Mutex++;
		SEM_MAKE_OPS();

		read_amount = *((ssize_t*)shm_ptr);
 		write(STDOUT_FILENO, shm_ptr + sizeof(ssize_t), read_amount);

        SEM_PUSH_OP(MUTEX_SEM, -1, SEM_UNDO);	// Mutex--;
		SEM_MAKE_OPS();
	}

	printf("=-=-=-=-=\n");
	return 0;
}










