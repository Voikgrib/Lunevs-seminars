
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

void err_worker(void);
int pipe_worker(char *file_name);

int main( int argc, char** argv)
{
	if(argc != Amount_of_param)
		assert(0 && "ERROR - Invalid amount of args!");

	char* name = argv[1];
	pid_t parent_pid = 0;

//

	if(Err_code == 0)
		Err_code = pipe_worker(name);

//

	return 0;
}


int pipe_worker(char *file_name)
{
	int pipefd[2] = {};
	pid_t cpid = 0;
	char buffer[128] = {};
	const int c_buff_size = 128;	

    if (pipe(pipefd) == -1)
		assert(0 && "ERROR - Pipe not created!");


	cpid = fork();

	if(cpid == -1)
		assert(0 && "ERROR - Sluchilsa vikidish!");


	if(cpid == 0) // Rebonok
	{
		int ifd = -1;
		assert((ifd = open(file_name, O_RDONLY)) != -1 && "ERROR - In file opening!");

		int num_of_read = 0;

		while((num_of_read = read(ifd, &buffer, c_buff_size)) && num_of_read > 0)
		{
			write(pipefd[1], &buffer, num_of_read);
			//
		}

		close(ifd);
		close(pipefd[1]);
	}
	else		  // Schastliviy roditel
	{
		int num_of_get = 0;
		close(pipefd[1]);

		while((num_of_get = read(pipefd[0], &buffer, c_buff_size)) && num_of_get > 0)
		{
			write(STDOUT_FILENO, &buffer, num_of_get);
		}

		wait(NULL);
	}

	return 0;
}




