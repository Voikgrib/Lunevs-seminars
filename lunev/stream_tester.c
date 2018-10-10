
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	pid_t ppid = getpid();
	pid_t cpid = fork();
	//pid_t pid = fork();
	system("./streamer streamer_1.c");
	//system("./follower pipe2");

	if(cpid != 0)
	{
	//	system("./streamer test_text.txt");		
		wait(NULL);
	}
	//else
	//{
	//	sleep(1);
	//	kill(cpid, 9);
	//}
	//if(pid != 0)
	//	wait(NULL);

	return 0;
}

