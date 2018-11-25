
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

// If child dead before parent start & obratnaya situacia!	(+)
// Busy-wait												(+/+)
// Last symbol ne yspevaet									(-)
// Idea: slat extra symbol i obazatelno ego terat'

// ~~~~~~~~~~~~~~~~~~~~~~~ GLOBAL CONSTANTS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

sig_atomic_t Sig_get = 0;
sig_atomic_t Is_printed = 0;
sig_atomic_t Is_child_dead = 0;
unsigned char Cur_letter = 0;
const int Amount_of_param = 2;
const int Error_in_init = -1;

// ~~~~~~~~~~~~~~~~~~~~~~~ FUNCTIONS DECLARE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void sig_p(int sig, siginfo_t *siginfo, void *context);
static void sig_c(int sig, siginfo_t *siginfo, void *context);

int copy_worker(char* file_name);
int child_func(pid_t parent_pid, char *name);
int parent_func(pid_t child_pid);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MAIN ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
	else if(error == Error_in_init)
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
		return Error_in_init;
	if(sigaddset(&set, SIGUSR1) == -1)
		return Error_in_init;
	if(sigaddset(&set, SIGUSR2) == -1)
		return Error_in_init;
	if(sigaddset(&set, SIGALRM) == -1)
		return Error_in_init;

	act.sa_mask = set;

	if(sigaction(SIGUSR1, &act, 0) == -1)
		return Error_in_init;
	if(sigaction(SIGUSR2, &act, 0) == -1)
		return Error_in_init;
	if(sigaction(SIGALRM, &act, 0) == -1)
		return Error_in_init;

	act.sa_flags = SA_SIGINFO;
	pid_t child_pid = fork();	

	if(child_pid == -1)
	{
		perror("> ERROR I can't fork!");
		return Error_in_init;
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

        parent_pid = getppid();
        
		if (parent_pid == 1)
		{
			perror("> ERROR Sudden death of parent!\n");
            exit(1);
		}

		error = child_func(parent_pid, file_name);

		if(error == Error_in_init)
			exit(1);
	}

	return error;
}



int child_func(pid_t parent_pid, char *name)
{
	int ifd = open(name, O_RDONLY | O_NONBLOCK);

	if(ifd < 0)
	{
		perror("> ERROR Can't open file!\n");
  		exit(1);
	}
	
	sigset_t sigs;
	char buffer[16] = {};
	const int c_buff_size = 16;
	int bit_num = 0;
	int max_bit = 8;
	int kill_ret = 0;
	int num_of_read = 0;
	int i = 0;

	if(sigemptyset(&sigs) == -1)
	{
		perror("> ERROR Sigempty set in child error!\n");
  		exit(1);
	}
	

	while((num_of_read = read(ifd, &buffer, c_buff_size)) && num_of_read > 0)
	{
		bit_num = 0;

		while(bit_num != max_bit * num_of_read)
		{
			alarm(1);
			while(Sig_get == 0)		// wait for signal from parent
			{
				sigsuspend(&sigs);
				if(errno != EINTR)
				{
					perror("> ERROR Sigsuspend in child error!\n");
					exit(1);
				}
	
				if(getppid() != parent_pid)
				{
					perror("\n>>> ERROR Parent dead!\n");
					exit(1);
				}

			}
			alarm(0);

			Sig_get = 0;			// no signals here
			

			if((buffer[bit_num / max_bit] & 0x01) == 1)
				kill_ret = kill(parent_pid, SIGUSR2);	// after that signal can be resive
			else
				kill_ret = kill(parent_pid, SIGUSR1);	// after that signal can be resive

			if(kill_ret == -1)
			{
				perror("> ERROR Can't send signal to parent!\n");
          		exit(1);
			}		

			buffer[bit_num / max_bit] = buffer[bit_num / max_bit] >> 1;

			bit_num++;
		}

	}

	alarm(1);
	while(Sig_get == 0)		// wait for ready to end signal from parent
	{
		sigsuspend(&sigs);

		if(errno != EINTR)
		{
			perror("> ERROR Sigsuspend in child error!\n");
			exit(1);
		}

		if(getppid() != parent_pid)
		{
			perror("\n>>> ERROR Parent dead!\n");
			exit(1);
		}
	}
	alarm(0);

	//usleep(100);

	return 0;
}



int parent_func(pid_t child_pid)
{
	char cur_byte = 0;
	char old_letter = 0;
	int bit_num = 0;
	int max_bit = 8;
	sigset_t sigs;
	
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_sigaction = &sig_p;
	sigset_t set;

	if(sigemptyset(&set) == -1)
		return Error_in_init;
	if(sigaddset(&set, SIGUSR1) == -1)
		return Error_in_init;
	if(sigaddset(&set, SIGUSR2) == -1)
		return Error_in_init;
	if(sigaddset(&set, SIGCHLD) == -1)
		return Error_in_init;
	if(sigaddset(&set, SIGALRM) == -1)
		return Error_in_init;


	act.sa_mask = set;
	act.sa_flags = SA_SIGINFO;

	if(sigaction(SIGUSR1, &act, 0) == -1)
		return Error_in_init;
	if(sigaction(SIGUSR2, &act, 0) == -1)
		return Error_in_init;
	if(sigaction(SIGCHLD, &act, 0) == -1)
		return Error_in_init;
	if(sigaction(SIGALRM, &act, 0) == -1)
		return Error_in_init;

	bit_num = 0;
	Cur_letter = 0;

	if(sigemptyset(&sigs) == -1)
	{
		perror("> ERROR Sigempty set in parent error!\n");
  		exit(1);
	}


	while(Cur_letter != EOF)
	{
		//usleep(10000); // for sinece!

		bit_num = 0;
		cur_byte = 0;
		Cur_letter = 0;

		while(bit_num != max_bit)
		{
			kill(child_pid, SIGUSR2);	// after that signal can be resive

			alarm(1);
			while(Sig_get == 0)			// wait for signal from child
			{
				sigsuspend(&sigs);
				if(errno != EINTR)
				{
					perror("> ERROR Sigsuspend in parent error!\n");
					exit(1);
				}
			}
			alarm(0);

			Sig_get = 0;		 		// no signals here
			
			bit_num++;
		}

		printf("%c", Cur_letter);
		fflush(stdout);
	}

	return 0;
}



static void sig_p(int sig, siginfo_t *siginfo, void *context)
{
	if(sig == SIGALRM)
		return;


	if(sig == SIGCHLD)
	{
		//Is_child_dead = 1;
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

	Sig_get = 1;
}



static void sig_c(int sig, siginfo_t *siginfo, void *context)
{
	if(sig == SIGALRM)
		return;

	Sig_get = 1;
}


