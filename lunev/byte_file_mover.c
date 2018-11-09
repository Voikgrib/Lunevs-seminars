
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ipc.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

sig_atomic_t Sig_get = 0;
unsigned char Cur_letter = 0;
int Amount_of_param = 2;

static void sig_p(int sig, siginfo_t *siginfo, void *context);
static void sig_c(int sig, siginfo_t *siginfo, void *context);

int copy_worker(char* file_name);
int child_func(pid_t parent_pid, char *name);
int parent_func(pid_t child_pid);

int main( int argc, char** argv)
{
	if(argc != Amount_of_param)
	{
		perror("Error invalid amount of args!\n");
		return -1;
	}

	char* name = argv[1];
	int error = copy_worker(name);

	if(error == -2)
	{
		perror("Error in init of parent function!\n");
		return -1;
	}
	else if(error == -1)
	{
		perror("Error in init of copy worker function!\n");
		return -1;
	}
	else if(error != 0)
	{
		perror("Some error C:\n");
		return -1;
	}

	return 0;
}

int copy_worker(char* file_name)
{
	pid_t parent_pid = getpid();
	int error = 0;

	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_sigaction = &sig_c;
	sigset_t set, oldset;

	if(sigemptyset(&set) == -1)
		return -1;
	if(sigaddset(&set, SIGUSR1) == -1)
		return -1;
	if(sigaddset(&set, SIGUSR2) == -1)
		return -1;

	act.sa_mask = set;

	if(sigaction(SIGUSR1, &act, 0) == -1)
		return -1;
	if(sigaction(SIGUSR2, &act, 0) == -1)
		return -1;

	act.sa_flags = SA_SIGINFO;
	pid_t child_pid = fork();	

	if(child_pid == -1)
	{
		perror("> ERROR I can't fork!");
		return -1;
	}

	if(child_pid != 0) 	// Roditel
	{
		error = parent_func(child_pid);

		if(error == 0)
			wait(NULL);
		else
			kill(child_pid, SIGKILL);
	}
	else				// Rebenok
	{
		error = child_func(parent_pid, file_name);

		if(error == -100)
		{
			kill(parent_pid, SIGKILL); // I ti Brut!
			kill(child_pid, SIGKILL);
		}
	}

	return error;
}

int child_func(pid_t parent_pid, char *name)
{
	int ifd = open(name, O_RDONLY | O_NONBLOCK);

	if(ifd < 0)
	{
		perror("> ERROR Can't open file!\n");
		return -100;
	}
	
	char buffer[16] = {};
	const int c_buff_size = 16;
	int bit_num = 0;
	int max_bit = 8;
	int num_of_read = 0;
	int i = 0;

	while((num_of_read = read(ifd, &buffer, c_buff_size)) && num_of_read > 0)
	{
		bit_num = 0;

		while(bit_num != max_bit * num_of_read)
		{
			while(Sig_get == 0); // wait for signal

			Sig_get = 0;

			if((buffer[bit_num / max_bit] & 0x01) == 1)
				kill(parent_pid, SIGUSR2);
			else
				kill(parent_pid, SIGUSR1);
			
			buffer[bit_num / max_bit] = buffer[bit_num / max_bit] >> 1;

			bit_num++;
		}
	}	

	return 0;
}

int parent_func(pid_t child_pid)
{
	char cur_byte = 0;
	char old_letter = 0;
	int bit_num = 0;
	int max_bit = 8;
	
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_sigaction = &sig_p;
	sigset_t set, oldset;
	
	if(sigemptyset(&set) == -1)
		return -2;
	if(sigaddset(&set, SIGUSR1) == -1)
		return -2;
	if(sigaddset(&set, SIGUSR2) == -1)
		return -2;
	if(sigaddset(&set, SIGCHLD) == -1)
		return -2;

	act.sa_mask = set;
	act.sa_flags = SA_SIGINFO;

	if(sigaction(SIGUSR1, &act, 0) == -1)
		return -2;
	if(sigaction(SIGUSR2, &act, 0) == -1)
		return -2;
	if(sigaction(SIGCHLD, &act, 0) == -1)
		return -2;

	bit_num = 0;
	Cur_letter = 0;

	bool is_first = true;

	while(Cur_letter != EOF)
	{
		bit_num = 0;
		cur_byte = 0;
		Cur_letter = 0;

		while(bit_num != max_bit)
		{
		    kill(child_pid, SIGUSR2);

			while(Sig_get == 0); // wait for signal
			Sig_get = 0;		 // no signals here

			bit_num++;
		}

		printf("%c", Cur_letter);
		fflush(stdout);
	}

	return 0;
}


static void sig_p(int sig, siginfo_t *siginfo, void *context)
{
	Sig_get = 1;

	if(sig == SIGCHLD)
	{
		printf(">>> Child dead\n");
		exit(0);
	}

	if(sig == SIGUSR2)
	{
		Cur_letter = (Cur_letter >> 1) | 0x80;
	}
	else if(sig == SIGUSR1)
	{
		Cur_letter = Cur_letter >> 1;
	}
}

static void sig_c(int sig, siginfo_t *siginfo, void *context)
{
	Sig_get = 1;
}


