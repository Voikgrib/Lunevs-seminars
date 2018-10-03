
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

const int Amount_of_param = 2;


int Err_code = 0;

int pipe_thrower(char* my_pid, char size);
int pipe_worker(char *file_name, char *my_pipe_name);

int main( int argc, char** argv)
{
	if(argc != Amount_of_param)
		assert(0 && "ERROR - Invalid amount of args!");

	char* name = argv[1];

	char pipe_name[100] = {};
	char *no_warnings = pipe_name;
	pid_t my_pid = getpid();

	char size = sprintf(pipe_name, "%s%d", "pipe_", my_pid);	
	assert(size > 0 && "ERROR - Invalid size of pipe name!");

	mkfifo(no_warnings, 0666);
	
	pipe_thrower(no_warnings, size);

	Err_code = pipe_worker(name, pipe_name);

	return 0;
}

int pipe_thrower(char* my_pid, char size)
{
	int pfdw = 0;
	if((pfdw = open("pipe_for_pid", O_WRONLY, 0644)) && pfdw == -1)
		assert(1 == 0 && "ERR Can't open pipe_checker2 for write!");

	assert(write(pfdw, &size, 1) == 1 && "ERROR - I can't throw size!");
	assert(write(pfdw, my_pid, size) == size && "ERROR - I can't throw pid name!");

	close(pfdw);

	return 0;
}


int pipe_worker(char *file_name, char *my_pipe_name)
{
	int pfd = -1;
	int ifd = -1;
	assert((ifd = open(file_name, O_RDONLY)) != -1 && "ERROR - In file opening!");
	assert((pfd = open(my_pipe_name, O_WRONLY)) != -1 && "ERROR - In pipe opening!");
	char buffer[16] = {};
	const int c_buff_size = 16;
	//int pfdr = open("pipe_checker", O_RDONLY, 0644);
	int num = 1;
	int num_of_read = 0;

	while((num_of_read = read(ifd, &buffer, c_buff_size)) && num_of_read > 0)
	{
		write(pfd, &buffer, num_of_read);
	}

	close(ifd);
	close(pfd);

	return 0;
}



