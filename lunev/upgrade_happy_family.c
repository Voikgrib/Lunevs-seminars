
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdint.h>

const size_t Msg_len = 10;

struct msg
{
	long mtype;
	uint8_t payload[50];
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

int main( int argc, char** argv)
{
	if(argc != Amount_of_param)
	{
		printf("!!ERR!! Invalid amount of args!\n");
		return 1;
	}

	int num = get_num_function(argv[1]);
	pid_t parent_pid = getpid();
	int cur_i = 0;

	int msg_id = msgget(IPC_PRIVATE, (IPC_CREAT | 0644));

	if(msg_id == -1)
		Err_code = Err_in_msg_open;

	if(Err_code == 0)
		printf("> Amount of childs %s\n", argv[1]);

//
	if(Err_code == 0)
		cur_i = process_creator(num, msg_id);
//

	if(Err_code == 0 && parent_pid != getpid())
		child_happy_printer(msg_id, cur_i);

	if(parent_pid == getpid())
	{
		wait(NULL);
		msgctl(msg_id, IPC_RMID, NULL);
	}

	err_worker();

	return 0;
}


void child_happy_printer(int id, int cur_i)
{
	struct msg my_msg;
	size_t size = msgrcv(id, &my_msg, Msg_len, cur_i, NULL);

	if(size != -1 && size < 11)
		printf("> I am %d\t child with pid = %d, when i born i said \"%s\"\n", cur_i, getpid(), my_msg.payload);
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

	num++;
	int i = 0;
	pid_t parent_pid = getpid();

	while(1 == 1)
	{
		if(parent_pid == getppid())
		{
			struct msg my_msg;
			my_msg.mtype = i;
			size_t size = sprintf(my_msg.payload, "%d-%d\0", i ,getpid());

			if(size > 50)
			{
				Err_code = Err_in_msgwrite;
				printf("Err in write\n");
				return -1;
			}

			if(msgsnd(msg_id, &my_msg, size, NULL) == -1)
			{
				Err_code = Err_in_msgsend;
				printf("Err in msg send\n");
				return -1;
			}

			break;
		}
		else if(i != num - 1)
		{
			fork();
			wait(NULL);
		}
		else
			break;

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
	else if(Err_code = Err_in_msg_open)
	{
		printf("!!ERR!! Ia ne mogy sozdat ochered'\n");
	}
	else 
		printf("!!ERR!! Ia ne znay chto eto za error, ego cod = %d c: \n", Err_code);
}
