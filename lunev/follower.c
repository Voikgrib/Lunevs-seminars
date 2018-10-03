
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
int is_stream_going(void);

int main( int argc, char** argv)
{
	int pfdr = 0;
	int pfdw = 0;
	int num_of_get = 0;
	char num[2] = {};

	if(argc != Amount_of_param)
		assert(0 && "ERROR - Invalid amount of args!");

	char* name = argv[1];
	pid_t parent_pid = 0;

	printf("> Wait for alone streamer...\n");
	//if((pfdr = open("pipe_checker2", O_RDONLY, 0644)) && pfdr == -1)
	//	assert(1 == 0 && "ERR Can't open pipe_checker2 for read!");
	if((pfdw = open("pipe_checker2", O_WRONLY, 0644)) && pfdw == -1)
		assert(1 == 0 && "ERR Can't open pipe_checker2 for write!");
	//while(num_of_get != 1)
	//{
	write(pfdw, "W", 1);
	//num_of_get = read(pfdr, &num, 2);
	//	if(num_of_get == 2)
	//		write(pfdw, " ", 1);		
	//}
	close(pfdw);
	printf("> Printed in pipechecker2");
	//close(pfdr);
	int need_write = is_stream_going();
	printf("> Streamer find!\n");

	if(Err_code == 0)
		Err_code = pipe_worker(name);

/*	if(need_write == 1)
	{
		if((pfdw = open("pipe_checker", O_WRONLY, 0644)) && pfdw == -1)
			assert(1 == 0 && "ERR Can't open pipe_checker for write!");
		write(pfdw, "W", 1);
		close(pfdw);
	}
*/
//

	return 0;
}

int is_stream_going(void)
{
	int pfdr = 0;
	char num[1] = {};
	int num_of_read = -1;

	if((pfdr = open("pipe_checker", O_RDONLY, 0644)) && pfdr == -1)
		assert(1 == 0 && "ERR Can't open pipe_checker for read!");

	num_of_read = read(pfdr, &num, 1);
	close(pfdr);

	if(num_of_read == 0)
		is_stream_going();
	else if(num_of_read == 2)
	{
		printf("!!! Its strange");
		return 1;
	}

	return 0;		
}

/*
{
	int pfdr = open("pipe_checker", O_RDONLY, 0644);
	char num[1] = {};
	int num_of_read = -1;

	num_of_read = read(pfdr, &num, 1);
	close(pfdr);

	if(num_of_read == 0)
		is_stream_going();
}
*/

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




