
#include "my_semaphore.h"

int recive(int shm_id, int sem_id);

int main(int argc, char* argv[])
{
	if(argc > 1)
		EXIT_AND_SAY(">>> ERROR: Invalid amount of args!");

	int shm_id = -1; 
	int sem_id = -1;

    sem_id = semget(Shared_semaphore, 1, IPC_CREAT|0666);
	if(sem_id == -1)
		EXIT_AND_SAY(">>> ERROR: Can't get semaphore in reciver!");

    shm_id = shmget(Shared_semaphore, sizeof(struct shared_buff), IPC_CREAT|0666);
	if(sem_id == -1)
		EXIT_AND_SAY(">>> ERROR: Can't get shared memory ID in reciver!");

	Shared_mem = shmat(shm_id, NULL, 0);
	if(Shared_mem == -1)
		EXIT_AND_SAY(">>> ERROR: Can't get shared memory in reciver!");

	recive(shm_id, sem_id);

	return 0;
}


int recive(int shm_id, int sem_id)
{


	return 0;
}




