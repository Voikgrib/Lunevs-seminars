
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{

	pid_t ppid = fork();
	//pid_t pid = fork();
	system("./streamer streamer.c");
	system("./streamer streamer_1.c");
	//system("./follower pipe2");

	if(ppid != 0)
		wait(NULL);

	//if(pid != 0)
	//	wait(NULL);

	return 0;
}

