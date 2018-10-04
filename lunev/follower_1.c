
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include<assert.h>

const int Amount_of_param = 1;

const int Pipe_len = 15;
int Err_code = 0;

int pipe_worker(char *my_pipe_name);
void pipe_catcher(char *my_pipe_name);

int main( int argc, char** argv)
{
	if(argc != Amount_of_param)
		assert(0 && "ERROR - Invalid amount of args!");

	char pipe_name[100] = {};

	pipe_catcher(pipe_name);
	printf(">>> Start read from %s\n", pipe_name);

	pipe_worker(pipe_name);
	printf(">>> End read from %s\n", pipe_name);

	return 0;
}

void pipe_catcher(char *my_pipe_name)
{
	char size = 0;
	int pfdw = 0;
	int num_of_read = 0;
	if((pfdw = open("pipe_for_pid", O_RDONLY | O_APPEND, 0644)) && pfdw == -1)
		assert(1 == 0 && "ERR Can't open pipe_for_pid for write!");

// critical section start
//	assert(read(pfdw, my_pipe_name, Pipe_len) == Pipe_len && "ERROR - I can't read current pipe information! (maby two streamers & one follower)");
	while(num_of_read == 0)
	{
		num_of_read = read(pfdw, my_pipe_name, Pipe_len);
		assert((num_of_read == 0 || num_of_read == Pipe_len) && "ERROR - Bad opening ");
	}
	printf("@ 2 @ Read pid \"%s\"\n", my_pipe_name);
// critical section end

	close(pfdw);
}

int pipe_worker(char *my_pipe_name)
{
	int pfd = open(my_pipe_name, O_RDONLY, 0644);
	char buffer[16] = {};
	const int c_buff_size = 16;

	assert(pfd != -1 && "ERROR Maby you turn on stream???");

	int num_of_get = 0;

	while((num_of_get = read(pfd, &buffer, c_buff_size)) && num_of_get > 0)
	{
		write(STDOUT_FILENO, &buffer, num_of_get);
	}

	close(pfd);

	return 0;
}




