
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

int Sig_get = 0;
int Need_change = 0;
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

	return 0;
}

int copy_worker(char* file_name)
{
	pid_t parent_pid = getpid();
	pid_t child_pid = fork();	
	int error = 0;

	if(child_pid == -1)
	{
		perror("> ERROR I can't fork!");
		return -1;
	}

	if(child_pid != 0) 	// Roditel
		error = parent_func(child_pid);
	else				// Rebenok
		error = child_func(parent_pid, file_name);

	return error;
}


int child_func(pid_t parent_pid, char *name)
{
	int ifd = open(name, O_RDONLY | O_NONBLOCK);

	if(ifd < 0)
	{
		perror("> ERROR Can't open file!\n");
		return -1;
	}
	
	char buffer[16] = {};
	const int c_buff_size = 16;
	int bit_num = 0;
	int max_bit = 8;
	int num_of_read = 0;
	int i = 0;

	struct sigaction act;
	memset(&act, 0, sizeof(act));
	//act.sa_handler = &sig_c;
	act.sa_sigaction = &sig_c;
	sigset_t set, oldset; 
	act.sa_flags = SA_SIGINFO;
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	act.sa_mask = set;
	sigaction(SIGUSR1, &act, 0);
	sigaction(SIGUSR2, &act, 0);
	bit_num = 0;

	while((num_of_read = read(ifd, &buffer, c_buff_size)) && num_of_read > 0)
	{
		//printf("> readed \"%s\" \n", buffer);
		bit_num = 0;

		while(bit_num != max_bit * num_of_read)
		{
			// check is child alive?? NI
			// wait signal usr_sig_2, allow sending NI

			//if(bit_num % 8 == 0)
			//{
			//	printf("%c",Cur_letter);
			//	Cur_letter = 0;
			//}

			//Sig_get = 0;
			sigprocmask (SIG_BLOCK, &set, &oldset);

			while(Sig_get == 0)
				sigsuspend(&oldset); // ???
			//while(Sig_get == 0);
			//kill(parent_pid, SIGUSR1);
			sigprocmask (SIG_UNBLOCK, &set, NULL);
			//signal(SIGUSR2, SIG_IGN);
			Sig_get = 0;

			if((buffer[bit_num / max_bit] & 0x01) == 1)
			{
				// send usr_sig_2 I ?
				//Cur_letter = (Cur_letter >> 1) | 0x80;
				kill(parent_pid, SIGUSR2); 
			}
			else
			{
				// send usr_sig_1 I ?
				//Cur_letter = Cur_letter >> 1;
				kill(parent_pid, SIGUSR1);
			}
			
			buffer[bit_num / max_bit] = buffer[bit_num / max_bit] >> 1;

			bit_num++;
		}
	}	

	return 0;
}

int parent_func(pid_t child_pid)
{
	//wait(NULL);
	//return 0;
	char cur_byte = 0;
	char old_letter = 0;
	int bit_num = 0;
	int max_bit = 8;

	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_sigaction = &sig_p;
	sigset_t ignor_all, set, oldset, oldestset;
	sigemptyset(&ignor_all); 
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	act.sa_mask = set;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, 0);
	sigaction(SIGUSR2, &act, 0);
	sigaction(SIGCHLD, &act, 0);
	bit_num = 0;
	Cur_letter = 1;

//	sigprocmask (SIG_BLOCK, &ignor_all, &oldestset);

	while(Cur_letter != EOF)
	{
		bit_num = 0;
		cur_byte = 0;

		// send usr_sig_2 I ?
		// kill(parent_pid, SIGUSR2);
		// sleep(1);
		Cur_letter = 0;

		while(bit_num != max_bit)
		{
			usleep(600);
			//usleep(10);
			// old_letter = Cur_letter;

			Sig_get = 0;
			Need_change = 0;

			//while(Sig_get == 0)
		    kill(child_pid, SIGUSR2);

			

			//signal(SIGUSR2, SIG_IGN);

			//Sig_get = 0;
			//Need_change = 1;
			sigprocmask(SIG_BLOCK, &set, &oldset);	

			if(Sig_get == 0)
				sigsuspend(&oldset);

			sigprocmask(SIG_UNBLOCK, &set, NULL);
//			Need_change = 0;

//			fflush(stdout);

			bit_num++;
		}

//		putchar(Cur_letter);
		printf("%c", Cur_letter);
		fflush(stdout);
	}

	return 0;
}

//sigprocmask (SIG_BLOCK, &mask, &oldmask);
//while (!usr_interrupt)
//  sigsuspend (&oldmask);
//sigprocmask (SIG_UNBLOCK, &mask, NULL);
/*
int Sig_get = 0;
int Need_change = 0;
*/

static void sig_p(int sig, siginfo_t *siginfo, void *context)
{
	Sig_get = 1;

	if(sig == SIGCHLD)
	{
		printf(">> Child dead\n");
		//exit(0);
	}

//	if(Need_change == 0)
	//	return;

	if(sig == SIGUSR2)	
	{
		//printf("1");
		Cur_letter = (Cur_letter >> 1) | 0x80;
	}
	if(sig == SIGUSR1)
	{
		//printf("0");
		Cur_letter = Cur_letter >> 1;
	}
}

static void sig_c(int sig, siginfo_t *siginfo, void *context)
{
	Sig_get = 1;
	//kill(getppid(), SIGUSR1);
}


