
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<pthread.h>

const int Amount_of_param = 3;

unsigned long int Omega_luls_num = 0;

int Err_code = 0;

const int Err_not_num = -1;
const int Err_out_of_range = -2;
const int Err_invalid_n = -3;
const int Err_invalid_k = -4;

int get_num_function(char *str);
void err_worker(void);
pid_t process_creator(int num);

void* tread_func(void *num);

int main( int argc, char** argv)
{
	if(argc != Amount_of_param)
	{
		printf("!!ERR!! Invalid amount of args!\n");
		return 1;
	}

	long int n = get_num_function(argv[1]);
	long int k = get_num_function(argv[2]);

	if(n < 0)
		Err_code = Err_invalid_n;
	if(k < 0)
		Err_code = Err_invalid_k;

	pthread_t* id = (pthread_t*)calloc(sizeof(pthread_t),n);
	int ret = 0;
	int status = 0;

	if(Err_code == 0)
	{
		int i = 0;
		
		while(i != n)
		{
			ret = pthread_create(&id[i], NULL, tread_func, &k); 
			i++;
		}

		i = 0;
		while(i != n)
		{
			pthread_join(id[i], &status);
			usleep(1000);
			i++;
		}
	}

	err_worker();

	if(Err_code == 0)
		printf("> Omega lul num = %ld\n", Omega_luls_num);

	free(id);
	return 0;
}

void* tread_func(void *num)
{
	long int inc_amount = 0;
	inc_amount = *((long int*)(num));
	long int i = 0;
	//printf(">>> %ld \n",inc_amount);

	while(i != inc_amount)
	{
		Omega_luls_num++;
		i++;
	}
}

int get_num_function(char *str)
{
	char *pend;
	long int num = strtol(str, &pend, 0);

	if(*pend != '\0')
		Err_code = Err_not_num;
	else if(errno == ERANGE)
		Err_code = Err_out_of_range;
	else
		return num;

	return 0;
}


void err_worker(void)
{
	if(Err_code == Err_not_num)
	{
		printf("!!ERR!! It's not num! I see not a num symbol! \n");
	}
	else if(Err_code == Err_out_of_range)
	{
		printf("!!ERR!! It's so big or so small! \n");
	}
	else if(Err_code == Err_invalid_n)
	{
		printf("!!ERR!! Ia ne mogy rogit' tread obratno! (n < 0)\n");
	}
	else if(Err_code == Err_invalid_k)
	{
		printf("!!ERR!! I am so lazy C: (n < 0)\n");
	}
}
