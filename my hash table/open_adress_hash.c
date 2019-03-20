

#include"open_adress_hash.h"

// need func for calculate hash (+)

unsigned int get_hash(const char* str_p, int* len_p)
{
	unsigned int pos = 0;
	unsigned int key = 1;
	int i = 0;

	while(str_p[i] != '\0') 
	{
		pos = pos + str_p[i] * key;
		key = key * HASH_KEY;

		i++;
	}

	if(len_p != NULL)
		(*len_p) = i;

	return pos;
}


int add_to_hash(struct hash_table* hash_p, char* str_p) // good
{
	if(hash_p->fill >= hash_p->size)
		return ERR_TABLE_IS_FULL;
	else
		hash_p->fill++;

	//printf("> add (fill = %d)\n", hash_p->fill);

	int len = 0;

	unsigned int pos = get_hash(str_p, &len);
	pos = pos % hash_p->size;

	//unsigned int last_pos = pos;
	//if(pos == 0)
	//	last_pos = hash_p->size;

	while(hash_p->nodes[pos].len > 0)
	{
		pos++;

		if(pos == hash_p->size)
			pos = 0;
	}

	hash_p->nodes[pos].str_p = str_p;
	hash_p->nodes[pos].len = len;

	return 0;
}



int delete_in_hash(struct hash_table* hash_p, char* str_p)
{
	if(str_p == NULL)
		return ERR_EL_IS_EMPTY;
	if(hash_p->fill == 0)
		return ERR_EL_IS_EMPTY;		

	int find_node = search_in_hash(hash_p, str_p);
	int cur_node = 0;

	if(find_node == -1)
		return ERR_NO_ELEMENT;

	free(hash_p->nodes[find_node].str_p);
	hash_p->nodes[find_node].str_p = NULL;

	if(hash_p->fill > 0)
		hash_p->fill--;


	if(find_node == hash_p->size - 1)				// set current node in next node
		cur_node = 0;
	else
		cur_node = find_node + 1;


	if(hash_p->nodes[cur_node].len == 0)			// if (next node is empty) set cur len = -1
	{	
		hash_p->nodes[find_node].len = 0;

		cur_node = find_node - 1;
		if(cur_node < 0)
			cur_node = hash_p->size - 1;

		while(hash_p->nodes[cur_node].len == -1) 	// check is prev node len = -1 if is, set it to 0
		{
			hash_p->nodes[cur_node].len = 0;

			cur_node--;								// set current node in prev node
				
			if(cur_node < 0)
				cur_node = hash_p->size - 1;
		}

	}
	else

		hash_p->nodes[find_node].len = -1;
	return 0;
}



int search_in_hash(struct hash_table* hash_p, char* str_p)
{
	unsigned int pos = get_hash(str_p, NULL);
	pos = pos % hash_p->size;

	struct hash_node* find_node = (struct hash_node*) for_each_element(hash_p, (void*) str_p, node_str_cmp, pos, CLASTER_M);

	if(find_node == NULL)
		return -1;

	int num = (int) ((struct hash_node*) find_node - (struct hash_node*) &hash_p->nodes[0]);

	return num;
}


void *node_str_cmp(struct hash_node* node, void *str_p)
{
	if(strcmp(node->str_p, (char*) str_p) == 0)
	{
		return (void*) node;
	}

	return NULL;
}



struct hash_table* create_hash(unsigned int size)
{
	if(size == 0)
		return NULL;
	
	struct hash_table* ret_hash = calloc(1, sizeof(struct hash_table));
	
	TEST_MOVEMENT_1;

	if(ret_hash == NULL)
		return NULL;

	ret_hash->size = size;
	ret_hash->fill = 0;
	ret_hash->nodes = calloc(size, sizeof(struct hash_node));

	if(ret_hash->nodes == NULL)
	{
		free(ret_hash);
		return NULL;
	}

	int i = 0;

	while(i < size)
		ret_hash->nodes[i++].len = 0;

	return ret_hash;
}



int destroy_hash(struct hash_table* hash_p)
{
	int i = 0;
	int ret = 0;
	
	if(hash_p == NULL)
		return ERR_IS_EMPTY;

	int max = hash_p->size;
 
	while(i != max)
	{
		if(hash_p->nodes[i].len > 0)
			free(hash_p->nodes[i].str_p);

		i++;
	}

	free(hash_p->nodes);
	free(hash_p);

	return 0;
}



void* for_each_element(struct hash_table* hash_p, void* outside_info_p, void* (*function)(struct hash_node*, void *), int start_num, int mode)
{
	void* ret = NULL;
	
	int pos = start_num;

	if(mode == CLASTER_M)
	{
		while(hash_p->nodes[pos].len > 0 || hash_p->nodes[pos].len == -1)
		{
			if(hash_p->nodes[pos].len != -1)
				ret = (*function)(&hash_p->nodes[pos], outside_info_p);

			if(ret != NULL)
				return ret;

			pos++;

			if(pos >= hash_p->size)
				pos = 0;

			if(start_num == pos)
				break;
		}
	}
	else if(mode == ALL_M)
	{
		do
		{
			if(hash_p->nodes[pos].len > 0)
			{
				ret = (*function)(&hash_p->nodes[pos], outside_info_p);

				if(ret != NULL)
					return ret;
			}

			pos++;

			if(pos >= hash_p->size)
				pos = 0;
		}
		while(start_num != pos);
	}

	return NULL;
}

#ifdef calloc
#undef calloc
#endif

void* upgrade_calloc(unsigned int amount, unsigned int size_of_element) 
{
	if((amount % 6) == 0) 	// *BAM!*
	{
		return NULL;
	}
	else		 			// *Click*
	{
		void* ret = calloc(amount, size_of_element);
		return ret;
	}
}

