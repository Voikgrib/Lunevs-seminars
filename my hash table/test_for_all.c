//#include<stdio.h>
#include"open_adress_hash.h"

//void* printfff( struct hash_node* node, void *outside_info_p);

#define ERROR_COND( cond , str)			\
			do{							\
				if( cond )				\
				{						\
					printf( str );		\
					err_count++;		\
				}						\
			}while(0);


void test_for_create_and_delete(void);
void test_for_search(void);
void test_for_foreach(void);
void test_for_add_and_delete(void);

void* testing_dummy_1(struct hash_node* cur_node, void * outsidenull );
void* testing_dummy_2(struct hash_node* cur_node, void * outsidenull );

int main()
{
	printf("\n\n>------------------ TESTS START ----------------------<\n");
	test_for_create_and_delete();
	test_for_search();
	test_for_foreach();
	test_for_add_and_delete();
	printf(">------------------ TESTS END ------------------------<\n\n\n");

	return 0;
}


void test_for_add_and_delete(void)
{
	int err_count = 0;

	struct hash_table* hash = create_hash(4);

	ERROR_COND((delete_in_hash(hash, NULL) == 0), "<!> Error in empty str delete attemt!\n");

	char* str1p = calloc(20, sizeof(char));
	char* str2p = calloc(20, sizeof(char));
	char* str3p = calloc(20, sizeof(char));
	char* str4p = calloc(20, sizeof(char));
	char* str5p = calloc(20, sizeof(char));

	str2p[0] = 'L';	str2p[1] = 'o';	str2p[2] = 'l';	str2p[3] = 'a';
	add_to_hash(hash, str2p);

	ERROR_COND((delete_in_hash(hash, str2p) != 0), "<!> Error in delete from hash attemt -1!\n");

	str2p = calloc(20, sizeof(char));

	str1p[0] = 'L';	str1p[1] = 'o';	str1p[2] = 'l';	str1p[3] = 'a';
	str2p[0] = 'i';	str2p[1] = 'H';
	str3p[0] = 'h';	str3p[1] = 'h';
	str4p[0] = 'i';	str4p[1] = 'i';
	str5p[0] = 't';	str5p[1] = 'i';

	ERROR_COND((delete_in_hash(hash, str4p) == 0), "<!> Error in delete from empty hash!\n");
	add_to_hash(hash, str2p);
	ERROR_COND((delete_in_hash(hash, str3p) != ERR_NO_ELEMENT), "<!> Error in delete from hash attemt -1!\n");
	add_to_hash(hash, str3p);
	add_to_hash(hash, str4p);
	add_to_hash(hash, str5p);

	ERROR_COND((add_to_hash(hash, str1p) != ERR_TABLE_IS_FULL), "<!> Error in add to full hash attemt!\n");
	ERROR_COND((delete_in_hash(hash, str5p) != 0), "<!> Error in delete from hash attemt  0!\n");
	ERROR_COND((delete_in_hash(hash, str4p) != 0), "<!> Error in delete from hash attemt  1!\n");
	ERROR_COND((delete_in_hash(hash, str3p) != 0), "<!> Error in delete from hash attemt  2!\n");
	ERROR_COND((delete_in_hash(hash, str2p) != 0), "<!> Error in delete from hash attemt  3!\n");
	//printf("<> %d\n",delete_in_hash(hash, str3p));	
	add_to_hash(hash, str1p);
	ERROR_COND((delete_in_hash(hash, str1p) != 0), "<!> Error in delete from hash attemt  4!\n");
	destroy_hash(hash);
	hash = create_hash(4);
		

	str2p = calloc(20, sizeof(char));
	str3p = calloc(20, sizeof(char));
	str4p = calloc(20, sizeof(char));

	str2p[0] = 4;
	str3p[0] = 5;
	str4p[0] = 3;


	add_to_hash(hash, str4p);
	add_to_hash(hash, str3p);
	add_to_hash(hash, str2p);
	
	ERROR_COND((delete_in_hash(hash, str4p) != 0), "<!> Error in delete from hash attemt 5!\n");
	ERROR_COND((delete_in_hash(hash, str3p) != 0), "<!> Error in delete from hash attemt 6!\n");
	ERROR_COND((delete_in_hash(hash, str2p) != 0), "<!> Error in delete from hash attemt 7!\n");

	str2p = calloc(20, sizeof(char));
	str3p = calloc(20, sizeof(char));
	str4p = calloc(20, sizeof(char));

	str2p[0] = 5;
	str3p[0] = 6;
	str4p[0] = 4;

	add_to_hash(hash, str4p);
	add_to_hash(hash, str3p);
	add_to_hash(hash, str2p);

	ERROR_COND((delete_in_hash(hash, str4p) != 0), "<!> Error in delete from hash attemt 9!\n");
	ERROR_COND((delete_in_hash(hash, str3p) != 0), "<!> Error in delete from hash attemt 10!\n");
	ERROR_COND((delete_in_hash(hash, str2p) != 0), "<!> Error in delete from hash attemt 11!\n");

	//*/
	ERROR_COND((delete_in_hash(hash, NULL) != ERR_EL_IS_EMPTY), "<!> Error in empty delete attemt!\n");
	destroy_hash(hash);
	//free(str1p);
	//free(str2p);
	//free(str3p);
	//free(str4p);//


	if(err_count == 0)
		printf("> Add & delete test passed awesome!\n");
	else
		printf("<!> Add & delete test passed with %d mistakes!\n", err_count);
}


void test_for_create_and_delete(void)
{
	int err_count = 0;
	struct hash_table* hash0 = create_hash(0);
	struct hash_table* hash1 = create_hash(5);
	struct hash_table* hash2 = create_hash(6);
	struct hash_table* hash3 = create_hash(13);	

	ERROR_COND((hash0 != NULL), "<!> Bad reaction for 0-size hash!\n");
	ERROR_COND((hash3 != NULL), "<!> Bad pointer in creating first if!\n");
	ERROR_COND((hash2 != NULL), "<!> Bad pointer in creating second if!\n");

	destroy_hash(hash1);
	destroy_hash(hash3);

	ERROR_COND((hash2 != NULL), "<!> Can't commit tactical suicide! (not bad pointer failed)!\n");
	ERROR_COND((destroy_hash(hash0) == 0), "<!> Uncorrect delete for empty element first!\n");
	ERROR_COND((destroy_hash(hash2) == 0), "<!> Uncorrect delete for empty element second!\n");


	if(err_count == 0)
		printf("> Create & destroy test passed good!\n");
	else
		printf("<!> Create & destroy test passed with %d mistakes!\n", err_count);
}


void test_for_search(void)
{
	int err_count = 0;
	struct hash_table* hash = create_hash(5);

	ERROR_COND((hash == NULL), "<!> Suddenly error with memory! <!>\n");

	char* str1p = calloc(20, sizeof(char));
	char* str2p = calloc(20, sizeof(char));
	char* str3p = calloc(20, sizeof(char));
	char* str4p = calloc(20, sizeof(char));
	char* str5p = calloc(20, sizeof(char));
	char* str6p = calloc(20, sizeof(char));
	char* str7p = calloc(20, sizeof(char));

	str1p[0] = 'i';	str1p[1] = 'H';
	str2p[0] = 'i';	str2p[1] = 'H';
	str3p[0] = 'h';	str3p[1] = 'h';
	str4p[0] = 'i';	str4p[1] = 'i';
	str5p[0] = 't';	str5p[1] = 'i';
	str6p[0] = 'L';	str6p[1] = '1';	str6p[2] = '1';	str6p[3] = '1';
	str7p[0] = 'L';	str7p[1] = 'o';	str7p[2] = 'l';	str7p[3] = 'a';

	add_to_hash(hash, str2p);
	add_to_hash(hash, str3p);
	add_to_hash(hash, str4p);
	add_to_hash(hash, str5p);
	add_to_hash(hash, str1p);

	int num = search_in_hash(hash, str4p);
	ERROR_COND((strcmp(hash->nodes[num].str_p, str4p) != 0), "<!> Search test 1 failed! <!>\n");

	delete_in_hash(hash, str2p);
	add_to_hash(hash, str6p);
	num = search_in_hash(hash, str6p);
	ERROR_COND((num == -1) || (strcmp(hash->nodes[num].str_p, str6p) != 0), "<!> Search test 2 failed! <!>\n");
	num = search_in_hash(hash, str7p);
	ERROR_COND((num != -1), "<!> Search test with nonexist element failed! <!>\n");

	destroy_hash(hash);
	free(str7p);
	//free(str6p); // str6p not in hash (?)

	if(err_count == 0)
		printf("> Search test passed so good!\n");
	else
		printf("<!> Search test test passed with %d mistakes!\n", err_count);

	return;
}


void test_for_foreach(void)
{
	int err_count = 0;
	struct hash_table* hash = create_hash(5);

	ERROR_COND((hash == NULL), "<!> Suddenly error with memory! <!>\n");

	char* str1p = calloc(20, sizeof(char));
	char* str2p = calloc(20, sizeof(char));
	char* str3p = calloc(20, sizeof(char));
	char* str4p = calloc(20, sizeof(char));
	char* str5p = calloc(20, sizeof(char));

	str1p[0] = 'L';	str1p[1] = 'o';	str1p[2] = 'l';	str1p[3] = 'a';
	str2p[0] = 'i';	str2p[1] = 'H';
	str3p[0] = 'h';	str3p[1] = 'h';
	str4p[0] = 'i';	str4p[1] = 'i';
	str5p[0] = 't';	str5p[1] = 'i';

	add_to_hash(hash, str2p);
	add_to_hash(hash, str3p);
	add_to_hash(hash, str4p);
	add_to_hash(hash, str5p);
	add_to_hash(hash, str1p);

	void* ret = for_each_element(hash, NULL, testing_dummy_1, 0, ALL_M);
	ERROR_COND((ret != (void*) str1p),"<!> Foreach worked uncorrect in returned test!\n");
	ret = for_each_element(hash, NULL, testing_dummy_2, 0, ALL_M);
	ERROR_COND((ret != NULL),"<!> Foreach worked uncorrect in unreturned test!\n");

	destroy_hash(hash);

	if(err_count == 0)
		printf("> Foreach test passed very good!\n");
	else
		printf("<!> Foreach test test passed with %d mistakes!\n", err_count);

	return;
}


void* testing_dummy_1(struct hash_node* cur_node, void * outsidenull )
{
	if(cur_node->str_p[0] == 'L')
		return (void*) cur_node->str_p;

	return NULL;
}


void* testing_dummy_2(struct hash_node* cur_node, void * outsidenull )
{

	return NULL;
}


