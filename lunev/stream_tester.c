
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{

	pid_t pid = fork();
	system("./streamer streamer.c");
	//system("./follower pipe2");

	if(pid != 0)
		wait(NULL);

	return 0;
}

