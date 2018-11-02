
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <assert.h>
#include <poll.h>

// нет 100% при получении номера пида из пайпа пайп ДОЛЖЕН БЫТЬ ОТКРЫТ СТРИМЕРОМ НА ЧТЕНИЕ
// logic race if follower launch first!

const int Amount_of_param = 1;

const int Pipe_len = 15;
int Err_code = 0;

int pipe_worker(char *my_pipe_name);
int pipe_catcher(char *my_pipe_name);

/*struct pollfd
{
	int fd;
	short events;
	short revents;
};
*/

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
	else if(Err_code == -2)
	{
		perror("Error in poll\n");
		return -1;
	}

	if(unlink(pipe_name) == -1)
	{
		perror("Error with unlink!\n");
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
	/*printf(">>> SLEEP\n");
	fflush(stdout);
	sleep(5);*/
	// if sleep here, error!
	struct pollfd fds[1];
	fds[0].fd = open(my_pipe_name, O_RDONLY | O_NONBLOCK, 0644);
	fds[0].events = POLLIN;
	int pfd = fds[0].fd;
	int timeout = 10000;
	if(pfd == -1)
		return -1;

	int ret = poll(fds, 1, timeout);
	int flags = fcntl(pfd , F_GETFD);
	fcntl(pfd , F_SETFD, flags ^ O_NONBLOCK);

	if(ret <= 0 || (fds[0].revents & POLLIN) == 0)
	{
		if(pfd > 0)
			close(pfd);

		perror("Error: My brother dead, sorry, i can't work T_T\n");
		return -2;
	}

	char buffer[16] = {};
	const int c_buff_size = 16;
	int num_of_get = 0;

	while(num_of_get = read(pfd, &buffer, c_buff_size))
	{
		write(STDOUT_FILENO, &buffer, num_of_get);
	}

	close(pfd);

	return 0;
}




