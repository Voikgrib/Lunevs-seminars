
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdint.h>

const size_t Msg_len = 2;

struct msg
{
	long mtype;
	int mtext[2];
};

const int Amount_of_param = 2;

int Err_code = 0;

const int Err_not_num = -1;
const int Err_out_of_range = -2;
const int Err_invalid_num = -3;
const int Err_in_msg_open = -4;
const int Err_in_msgsend = -5;
const int Err_in_msgwrite = -6;

int get_num_function(char *str);
int process_creator(int num, int msg_id);
void err_worker(void);
void child_happy_printer(int id, int cur_i);
void waiter(int id, int cur_i, int num);

int main(int argc, char** argv)
{
	if(argc != Amount_of_param)
	{
		printf("!!ERR!! Invalid amount of args!\n");
		return 1;
	}

	int num = get_num_function(argv[1]);
	pid_t parent_pid = getpid();
	int cur_i = 0;

	int msg_id = msgget(IPC_PRIVATE, (IPC_CREAT | 0666));

	if(msg_id == -1)
		Err_code = Err_in_msg_open;

//	if(Err_code == 0)
//		printf("> Amount of childs %s\n", argv[1]);

//
	if(Err_code == 0)
		cur_i = process_creator(num, msg_id);
//

	if(Err_code == 0 && parent_pid != getpid() && cur_i > 0)
		waiter(msg_id, cur_i, num);

	if(parent_pid == getpid())
	{
		int i = 0;

		while(i != cur_i)
		{
			wait(NULL);
			i++;
		}

		if(msgctl(msg_id, IPC_RMID, NULL) == -1)
			perror("Error!!! Can't delete msg!");
		printf("\n\n");

		err_worker();
	}

	return 0;
}


void waiter(int id, int cur_i, int num)
{
	struct msg my_msg;
	size_t size = 0;
	int i = 0;

	if(cur_i == 1)
	{
		while(i != num - 1) // wait all
		{
			size = msgrcv(id, &my_msg, sizeof(my_msg.mtext), num + 1, MSG_NOERROR);

			if(size != -1)
				i++;

			if(i == num - 1)
			{
				printf("1 ");
				fflush(stdout);
			}
		}

		my_msg.mtype = cur_i;
		msgsnd(id, (void *) &my_msg, sizeof(my_msg.mtext), NULL);
	}
	else
	{
		while(msgrcv(id, &my_msg, sizeof(my_msg.mtext), cur_i - 1, MSG_NOERROR) == -1); // prologue

		printf(" %d ", cur_i);													// critical section
		fflush(stdout);															// critical section
		my_msg.mtype = cur_i;													// critical section

		msgsnd(id, (void *) &my_msg, sizeof(my_msg.mtext), NULL);						// epilogue
	}
}


void child_happy_printer(int id, int cur_i)
{
	struct msg my_msg;
	size_t size = msgrcv(id, &my_msg, sizeof(my_msg.mtext), cur_i, MSG_NOERROR);

	if(size != -1)
		printf("> %d-%d = \"%d-%d\"\n", cur_i, getpid(), my_msg.mtext[0], my_msg.mtext[1]);
	else
		printf("> I am %d\t child with pid = %d and i have error T_T\n", cur_i, getpid());
}

int process_creator(int num, int msg_id)
{
	if(num < 0)
	{
		Err_code = Err_invalid_num; 
		return -1;
	}

	long i = 0;
	pid_t parent_pid = getpid();
	pid_t cur_pid = -1;

	while(i < num)
	{
		if(cur_pid == 0) // upgrage happy child
		{
			struct msg my_msg;
			my_msg.mtype = num + 1;
			my_msg.mtext[0] = i;
			my_msg.mtext[1] = getpid();
			size_t size = sizeof(my_msg.mtext);

			if(msgsnd(msg_id, (void *) &my_msg, size, NULL) == -1)
			{
				Err_code = Err_in_msgsend;
				printf("Err in msg send\n");
				return -1;
			}

			return i;
		}
		else 		// Roditel 
		{
			cur_pid = fork();
		}

		i++;
	}

	return i;
}


int get_num_function(char *str)
{
	char *pend;
	long int num = strtol(str, &pend, 0);

	if(*pend != '\0')
		Err_code = Err_not_num;
	else if(errno == ERANGE)
		Err_code = Err_out_of_range;
	else
		return num;

	return 0;
}


void err_worker(void)
{
	if(Err_code == 0)
		return;

	if(Err_code == Err_not_num)
	{
		printf("!!ERR!! It's not num! I see not a num symbol! \n");
	}
	else if(Err_code == Err_out_of_range)
	{
		printf("!!ERR!! It's so big or so small! \n");
	}
	else if(Err_code == Err_invalid_num)
	{
		printf("!!ERR!! Ia ne mogy rogit' ego obratno! (amount of child < 0)\n");
	}
	else if(Err_code == Err_in_msg_open)
	{
		printf("!!ERR!! Ia ne mogy sozdat ochered'\n");
	}
	else 
		printf("!!ERR!! Ia ne znay chto eto za error, ego cod = %d c: \n", Err_code);
}
