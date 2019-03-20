
#include"open_adress_hash.h"


void* dummy(struct hash_node* cur_node, void * outsidenull);


int main()
{
	struct hash_table* hash = create_hash(6);

	char* str1p = calloc(20, sizeof(char));
	char* str2p = calloc(20, sizeof(char));
	char* str3p = calloc(20, sizeof(char));
	char* str4p = calloc(20, sizeof(char));

	str1p[0] = 'L';	str1p[1] = 'o';	str1p[2] = 'o';	str1p[3] = 'p';
	str2p[0] = 'W';	str2p[1] = 'h'; str2p[2] = 'i'; str2p[3] = 'l'; str2p[4] = 'e';
	str3p[0] = 'i';	str3p[1] = 'f';
	str4p[0] = 'e';	str4p[1] = 'l'; str4p[2] = 's'; str4p[3] = 'e';

	add_to_hash(hash, str1p);
	add_to_hash(hash, str2p);
	add_to_hash(hash, str3p);
	add_to_hash(hash, str4p);

	for_each_element(hash, NULL, dummy, 0, ALL_M);

	destroy_hash(hash);
	return 0;
}

void* dummy(struct hash_node* cur_node, void * outsidenull)
{
	printf(" - %s\n", cur_node->str_p);

	return NULL;
}
