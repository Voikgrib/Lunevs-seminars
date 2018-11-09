#include<stdio.h> 
#include<signal.h> 
  
// Handler for SIGINT, caused by 
// Ctrl-C at keyboard 
static void handle_sigint(int sig, siginfo_t *siginfo, void *context)
{ 
    printf(" Sosi pisos 8===D\n"); 

	fflush(stdout);

//	if(sig == SIGINT)
//		exit(0);
} 
  
static void sig_p(int sig, siginfo_t *siginfo, void *context)
{
	if(sig == SIGUSR2)	
		printf(" Sosi pisos 8===D %d\n", sig); 
	if(sig == SIGUSR1)
		printf(" Sosi pisos 8===D %d\n", sig); 
	if(sig == SIGINT)
	{
		printf(" I am dying! \n");
		exit(0);
	}
}

int main() 
{ 
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	int i = 42;
	//act.sa_handler = &sig_c;
	act.sa_sigaction = &handle_sigint;
	sigset_t set, oldset, set_2; 
	act.sa_flags = SA_SIGINFO;
	sigemptyset(&set);
	sigemptyset(&set_2);
	sigaddset(&set_2, SIGUSR2);
	sigaddset(&set, SIGUSR1); 
	sigaddset(&set, SIGUSR2);
	sigaddset(&set, SIGINT);
	act.sa_mask = set;
	sigaction(SIGUSR1, &act, 0);
	sigaction(SIGUSR2, &act, 0);
	sigaction(SIGINT, &act, 0);

	if(fork() == 0)
	{
		sleep(1);
		kill(getppid(), SIGUSR1);
		sleep(1);
		kill(getppid(), SIGUSR2);
		sleep(1);
	}
//    signal(SIGINT, handle_sigint); 

	sigprocmask (SIG_BLOCK, &set_2, &oldset);
	sigsuspend(&oldset); // ???
	sigprocmask (SIG_UNBLOCK, &set_2, NULL);

	printf("After some sig\n");

    while (1) ; 
    return 0; 
}
