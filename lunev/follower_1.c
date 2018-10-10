
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <assert.h>

const int Amount_of_param = 1;

const int Pipe_len = 15;
int Err_code = 0;

int pipe_worker(char *my_pipe_name);
int pipe_catcher(char *my_pipe_name);


int main( int argc, char** argv)
{
	if(argc != Amount_of_param)
	{
		perror("ERROR - Invalid amount of args!\n");
		return -1;
	}

	char pipe_name[100] = {};

	Err_code = pipe_catcher(pipe_name);
	
	if(Err_code == -1)
	{
		perror("Error Can't open pipe_for_pid for write!\n");
		return -1;
	}
	else if(Err_code == -2)
	{
		perror("Error Bad opening\n");
		return -1;
	}
	
	Err_code = pipe_worker(pipe_name);
	if(Err_code == -1)
	{
		perror("Error Maby you turn on stream???\n");
		return -1;
	}
	
	return 0;
}

int pipe_catcher(char *my_pipe_name)
{
	int pfdw = 0;
	int num_of_read = 0;
	if((pfdw = open("pipe_for_pid", O_RDONLY | O_APPEND, 0644)) && pfdw == -1)
		return -1;

	while(num_of_read == 0)
	{
		num_of_read = read(pfdw, my_pipe_name, Pipe_len);

		if(num_of_read != 0 && num_of_read != Pipe_len)
			return -2;
	}

	close(pfdw);
}

int pipe_worker(char *my_pipe_name)
{
//	int error_check = open(my_pipe_name, O_NONBLOCK | O_WRONLY, 0644);
//	int nbpfd = open(my_pipe_name, O_NONBLOCK | O_RDONLY, 0644);
	int pfd = open(my_pipe_name, O_RDONLY, 0644);
//	printf("Error = %d\n", error_check);	
//	close(error_check);
	char buffer[16] = {};
	const int c_buff_size = 16;

//	if(fcntl(nbpfd, F_SETFL, fcntl(nbpfd, F_GETFL, 0) | O_NONBLOCK))
//		return -1;

	if(pfd == -1)// || nbpfd < 0)
		return -1;

	int num_of_get = 0;

/*	while((num_of_get = read(nbpfd, &buffer, c_buff_size)) && errno == EAGAIN)
	{
		if(num_of_get > 0)
			write(STDOUT_FILENO, &buffer, num_of_get);

		errno = 0;
		printf("Z...z...z...\n");
	}
*/
//	printf("\n>> 0 << Z...z...z...\n");
//	write(STDOUT_FILENO, &buffer, num_of_get);
//	printf("\n>> 1 << Z...z...z...\n");
	num_of_get = 0;

	while((num_of_get = read(pfd, &buffer, c_buff_size)) && num_of_get > 0)
	{
		write(STDOUT_FILENO, &buffer, num_of_get);
	}

//	close(nbpfd);
	close(pfd);

	return 0;
}




