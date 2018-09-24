
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

void is_stream_going(void);
int pipe_worker(char *file_name);

int main( int argc, char** argv)
{
	if(argc != Amount_of_param)
		assert(0 && "ERROR - Invalid amount of args!");

	char* name = argv[1];
	pid_t parent_pid = 0;

//
//	printf("1\n");
//	is_stream_going();
//	printf("2\n");

	if(Err_code == 0)
		Err_code = pipe_worker(name);

//

	return 0;
}

void is_stream_going(void)
{

	int pfdw = open("pipe_checker", O_RDWR, 0644);
	//int pfdr = open("pipe_checker", O_RDONLY, 0644);
	char num[1] = {};
	int num_of_read = 0;
	//write(pfdw, "", 1);

	while((num_of_read = read(pfdw, &num, 1)) && num_of_read > 0)
	{
		printf("readed = %d, %s\n", num_of_read, num);
		write(pfdw, "R", 1);
	}

	close(pfdw);
	//close(pfdr);
}

int pipe_worker(char *file_name)
{
	int pfdw = open("pipe_checker", O_WRONLY, 0644);
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
		//
	}

	//pfdw = open("pipe_checker", O_RDONLY, 0644);
	//read(pfdw, &num, 1);

	//close(pfdw);
	close(ifd);
	close(pfd);

	return 0;
}



