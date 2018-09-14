
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

const int Amount_of_param = 2;

const int Err_not_num = -1;
const int Err_out_of_range = -2;


int printf_function(char *str);
int err_worker(int err_code);

int main( int argc, char** argv)
{
	if(argc != Amount_of_param)
	{
		printf("!!ERR!! Invalid amount of args!\n");
		return 1;
	}

	int err_code = printf_function(argv[1]);
	err_code = err_worker(err_code);

	if(err_code == 0)
		printf("> num was = %s\n", argv[1]);

	return 0;
}

int printf_function(char *str)
{
	char *pend;
	long int num = strtol(str, &pend, 0);

	if(*pend != '\0')
		return Err_not_num;
	else if(errno == ERANGE)
		return Err_out_of_range;
	else if(num > 0)
		while(num != 0)
		{
			printf("> %ld\n", num);
			num--;
		}
	else if(num < 0)
		while(num != 0)
		{
			printf("> %ld\n", num);
			num++;
		}

	return 0;
}

int err_worker(int err_code)
{
	if(err_code == Err_not_num)
	{
		printf("!!ERR!! It's not num! I see not a num symbol! \n");
		return Err_not_num;
	}
	else if(err_code == Err_out_of_range)
	{
		printf("!!ERR!! It's so big or so small! \n");
		return Err_not_num;
	}
	else
		printf("> Everything okay c:\n");

	return 0;
}

