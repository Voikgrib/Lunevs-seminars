
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

int pipe_worker(char *pipe_name);
void is_stream_going(void);

int main( int argc, char** argv)
{
	if(argc != Amount_of_param)
		assert(0 && "ERROR - Invalid amount of args!");

	char* name = argv[1];
	pid_t parent_pid = 0;


//
	printf("1\n");
	is_stream_going();
	printf("2\n");

	if(Err_code == 0)
		Err_code = pipe_worker(name);

//

	return 0;
}

void is_stream_going(void)
{
	int pfdr = open("pipe_checker", O_RDONLY, 0644);
	char num[1] = {};
	int num_of_read = -1;
	//write(pfdw, " ", 1);

	
	num_of_read = read(pfdr, &num, 1);
	close(pfdr);

	if(num_of_read == 0)
		is_stream_going();

	//while((num_of_read = read(pfdr, &num, 1)) && num_of_read == 0);

	//while((num_of_read = read(pfdw, &num, 1)) && num_of_read == 0);

	//int pfdw = open("pipe_checker", O_WRONLY, 0644);
	//num_of_read = read(pfdw, &num, 1);


	//close(pfdw);
}

int pipe_worker(char *pipe_name)
{
	int pfd = open(pipe_name, O_RDONLY, 0644);
	char buffer[16] = {};
	const int c_buff_size = 16;

	int num_of_get = 0;
	//close(pipefd[1]);

	while((num_of_get = read(pfd, &buffer, c_buff_size)) && num_of_get > 0)
	{
		write(STDOUT_FILENO, &buffer, num_of_get);
	}

	close(pfd);

	return 0;
}




