
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
const int Pipe_len = 15;

int pipe_thrower(char* my_pid, char size);
int pipe_worker(char *file_name, char *my_pipe_name);

int main( int argc, char** argv)
{
	if(argc != Amount_of_param)
		assert(0 && "ERROR - Invalid amount of args!");

	char* name = argv[1];

	char pipe_name[100] = {};
	char kill_string[100] = {};
	char *no_warnings = pipe_name;
	char *pipe_p = NULL;
	pid_t my_pid = getpid();

	char size = sprintf(pipe_name, "%s%d", "pipe_", my_pid);	
	int i = 0;

	while(size != Pipe_len)
	{
		pipe_name[size++] = '_';
	}

	pipe_name[size] = '\0';
	printf(">>> Start write in %s\n", no_warnings);

	assert(size > 0 && "ERROR - Invalid size of pipe name!");

	mkfifo(no_warnings, 0666);
	
	int pfdw = pipe_thrower(no_warnings, size);

	pipe_worker(name, pipe_name);
	sprintf(kill_string, "rm %s", pipe_name);
	system(kill_string);

	printf(">>> End write in %s\n", no_warnings);
	close(pfdw);
	return 0;
}

int pipe_thrower(char* my_pid, char size)
{
	int pfdw = 0;
	printf("@ %s @ pipe_thrower 1\n", my_pid);
	if((pfdw = open("pipe_for_pid", O_WRONLY | O_APPEND, 0644)) && pfdw == -1)
		assert(1 == 0 && "ERR Can't open pipe_for_pid for write!");

	printf("@ %s @ pipe_thrower 2\n", my_pid);
// critical section start
//	assert(write(pfdw, &size, 1) == 1 && "ERROR - I can't throw size!");
	assert(write(pfdw, my_pid, Pipe_len) == Pipe_len && "ERROR - I can't throw pid information!");
// critical section end
	printf("@ %s @ pipe_thrower 3\n", my_pid);

	//close(pfdw);

	return pfdw;
}


int pipe_worker(char *file_name, char *my_pipe_name)
{
	int pfd = -1;
	int ifd = -1;
	printf("@ %s @ pipe_worker 1\n", my_pipe_name);
	assert((pfd = open(my_pipe_name, O_WRONLY)) && pfd != -1 && "ERROR - In pipe opening!");
	printf("@ %s @ pipe_worker 2\n", my_pipe_name);
	assert((ifd = open(file_name, O_RDONLY)) && ifd != -1 && "ERROR - In file opening!");
	printf("@ %s @ pipe_worker 3\n", my_pipe_name);

	char buffer[16] = {};
	const int c_buff_size = 16;
	//int pfdr = open("pipe_checker", O_RDONLY, 0644);
	int num = 1;
	int num_of_read = 0;

	printf("@ %s @ pipe_worker 4\n", my_pipe_name);
	while((num_of_read = read(ifd, &buffer, c_buff_size)) && num_of_read > 0)
	{
		write(pfd, &buffer, num_of_read);
	}
	printf("@ %s @ pipe_worker 5\n", my_pipe_name);

	close(ifd);
	close(pfd);

	return 0;
}



