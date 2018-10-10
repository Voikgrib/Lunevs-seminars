
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

// если фолловер сдохнет после прочтения номера пайпа, то стример повиснет навсегда на моменте открытия пайпа   |-|
// asserts заменить на возвращение кода ошибки (s)=(+) (f)=(+)  												|+|
// убивать пайп НЕ через system 																				|+|

//	printf(">>> SLEEP \n");
//	sleep(100);

// посмотреть no way out


int Err_code = 0;
const int Pipe_len = 15;

int pipe_thrower(char* my_pid, char size);
int pipe_worker(char *file_name, char *my_pipe_name);

int main( int argc, char** argv)
{
	if(argc != Amount_of_param)
	{
		perror("Error invalid amount of args!\n");
		return -1;
	}

	char* name = argv[1];

	char pipe_name[100] = {};
	char kill_string[100] = {};
	char *no_warnings = pipe_name;
	pid_t my_pid = getpid();

	char size = sprintf(pipe_name, "%s%d", "pipe_", my_pid);	
	int i = 0;

	if(size <= 0)
	{
		perror("Invalid amount of args!\n");
		return -1;
	}

	while(size != Pipe_len)
	{
		pipe_name[size++] = '_';
	}

	pipe_name[size] = '\0';

	if(mkfifo(no_warnings, 0666) == -1)
	{
		perror("Error with makung fifo!\n");
		return -1;
	}
	

	int pfdw = pipe_thrower(no_warnings, size);

	if(pfdw == -1)
	{
		perror("Error Can't open pipe_for_pid for write or I can't thow pipe info!\n");
		return -1;
	}

	printf(">>> SLEEP \n");
	sleep(10);

	int error = pipe_worker(name, pipe_name);
	if(error == -1)
	{
		perror("Error in pipe opening!\n");
		return -1;
	}
	else if(error == -2)
	{
		perror("Error in file opening!\n");
		return -1;
	}

	close(pfdw);

	if(unlink(pipe_name) == -1)
	{
		perror("Error with unlink!\n");
		return -1;
	}

	return 0;
}

int pipe_thrower(char* my_pid, char size)
{
	int pfdw =  open("pipe_for_pid", O_WRONLY | O_APPEND, 0644);

	if(pfdw == -1)
		return -1;

	if(write(pfdw, my_pid, Pipe_len) != Pipe_len)
		return -1;

	return pfdw;
}


int pipe_worker(char *file_name, char *my_pipe_name)
{
	int pfd = open(my_pipe_name, O_WRONLY);
	int ifd = open(file_name, O_RDONLY);

	if(pfd == -1)
		return -1;
	if(ifd == -1)
		return -2;

	char buffer[16] = {};
	const int c_buff_size = 16;
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



