
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

const int Amount_of_param = 2;

int Err_code = 0;

const int Err_not_num = -1;
const int Err_out_of_range = -2;
const int Err_invalid_num = -3;

int get_num_function(char *str);
void err_worker(void);
pid_t process_creator(int num);

int main( int argc, char** argv)
{
	if(argc != Amount_of_param)
	{
		printf("!!ERR!! Invalid amount of args!\n");
		return 1;
	}

	int num = get_num_function(argv[1]);
	pid_t parent_pid = 0;

	if(Err_code == 0)
		printf("> Amount of childs %s\n", argv[1]);

//

	if(Err_code == 0)
		parent_pid = process_creator(num);

//

	err_worker();

	return 0;
}


pid_t process_creator(int num)
{
	if(num < 0)
	{
		Err_code = Err_invalid_num; 
		return -1;
	}

	num++;
	int i = 0;
	pid_t cur_pid = -1;
	pid_t parent_pid = getpid();

	while(i != num)
	{
		if(parent_pid == getppid())
		{
			printf("> %d child\t\tpid = %d\n", i, getpid());
			break;
		}
		else
			fork();

		i++;
	}

	return parent_pid;
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
}
