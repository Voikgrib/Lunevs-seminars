
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


int main( int argc, char** argv)
{
	if(argc > 0)
	{
		execvp(argv[1], argv + 1);
	}
	else
		printf("!!ERR!! Were is my arguments, Lebovski!?\n");

	return 0;
}

