
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DEFINES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

#define TEST_MOVEMENT_1 ; 


#ifdef TEST_BUILD

#define calloc upgrade_calloc

#undef TEST_MOVEMENT_1

#define TEST_MOVEMENT_1			do{											\
								if(size == 13)								\
									{										\
										free(ret_hash);						\
										ret_hash = NULL;					\
									}										\
								}while(0);								 	\


#endif // TEST_BUILD


#define ERR_TABLE_IS_FULL 	-1
#define ERR_EL_IS_EMPTY		-2
#define ERR_IN_MEMORY		-3
#define ERR_IS_EMPTY		-4
#define ERR_NO_ELEMENT		-5

#define ALL_M				6
#define CLASTER_M			7

#define HASH_KEY			42

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ STRUCTS DECLARE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

struct hash_node
{
	int	len;
	char* str_p;
};

struct hash_table
{
	int fill;
	int	size;
	struct hash_node* nodes;	
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ FUNCTIONS DECLARE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //


///=========================================================
//!
//! This function has 1/6 chance to give error 
//!
///=========================================================
void* upgrade_calloc(unsigned int amount, unsigned int size_of_element);


///=========================================================
//!
//! This function creates hash massive in memory
//!
///=========================================================
struct hash_table* create_hash(unsigned int size);


///=========================================================
//!
//! This function deletes hash massive in memory
//!
///=========================================================
int destroy_hash(struct hash_table* hash_p);


///=========================================================
//!
//! This function add string 
//!
///=========================================================
int add_to_hash(struct hash_table* hash_p, char* str_p);


///=========================================================
//!
//! This function delete string
//!
///=========================================================
int delete_in_hash(struct hash_table* hash_p, char* str_p); 
void *node_str_cmp(struct hash_node* node, void *str_p);


///=========================================================
//!
//! This function apply another function to EVERY element
//!
//! When inside function return not NULL, return that pounter
//!
///=========================================================
void* for_each_element(struct hash_table* hash_p, void *outside_info_p, void* (*function)(struct hash_node*, void *), int start_num, int mode); // !!!!


///=========================================================
//!
//! This function search for needed string
//!
///=========================================================
int search_in_hash(struct hash_table* hash_p, char* str_p);


///=========================================================
//!
//! This function calculate hash
//!
//! If you need to get lengh of string, enter adress for len into "len_p", if you not need, enter NULL 
//!
///=========================================================
unsigned int get_hash(const char* str_p, int* len_p);




