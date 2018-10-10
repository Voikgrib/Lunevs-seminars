
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

int is_stream_going(void);
int pipe_worker(char *file_name);

int main( int argc, char** argv)
{
	if(argc != Amount_of_param)
		assert(0 && "ERROR - Invalid amount of args!");

	int pfdw = 0;
	char* name = argv[1];
	pid_t parent_pid = 0;

//
	printf("> Can I stream?\n");
	int need_write = is_stream_going();
	printf("> Yes, I can!\n");

	if(Err_code == 0)
		Err_code = pipe_worker(name);

	if(need_write == 1)
	{
		printf("\n ----> ----> ----> IN WRITER <---- <---- <---- \n");
		if((pfdw = open("pipe_checker2", O_WRONLY, 0644)) && pfdw == -1)
			assert(1 == 0 && "ERR Can't open pipe_checker for write!");
		write(pfdw, "W", 1);
		close(pfdw);
	}
//

	return 0;
}

int is_stream_going(void)
{
	int pfdr = 0;
	char num[2] = {};
	int num_of_read = -1;

	if((pfdr = open("pipe_checker2", O_RDONLY, 0644)) && pfdr == -1)
		assert(1 == 0 && "ERR Can't open pipe_checker for read!");

	num_of_read = read(pfdr, &num, 2);
	close(pfdr);

	if(num_of_read == 0)
		is_stream_going();
	else if(num_of_read == 2)
		return 1;

	return 0;		
}

int pipe_worker(char *file_name)
{
	int pfdw = 0;
	if((pfdw = open("pipe_checker", O_WRONLY, 0644)) && pfdw == -1)
		assert(1 == 0 && "ERR Can't open pipe_checker2 for write!");

	write(pfdw, "R", 1);	
	close(pfdw);

	int pfd = open("pipe2", O_WRONLY, 0644);
	int ifd = -1;
	assert((ifd = open(file_name, O_RDONLY)) != -1 && "ERROR - In file opening!");
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



