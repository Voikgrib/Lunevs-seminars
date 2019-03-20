
#include"open_adress_hash.h"


int search_in_hash(struct hash_table* hash_p, char* str_p)
{
	unsigned int key = 1;
	unsigned int pos = 0;
	unsigned int i = 0;

	while(str_p[i] != '\0')
	{
		pos = pos + str_p[i] * key;
		key = key * HASH_KEY;

		i++; 
	}

	pos = pos % hash_p->size;
	unsigned int last_pos = pos;

	if(pos == 0)
		last_pos = hash_p->size - 1;

	while(strcmp(str_p, hash_p->nodes[pos].str_p) != 0)
	{
		pos++;

		if(last_pos == pos)
			return -1;

		if(pos == hash_p->size)
			pos = 0;		
	}

	return pos;
}




///=========================================================================//
///=========================== END OF FUCTION ==============================// 
///=========================================================================//


