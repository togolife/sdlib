#pragma once
#ifndef SDLIB_HASH_TABLE_H
#define SDLIB_HASH_TABLE_H

typedef struct ListNode ListNode;
struct ListNode {
	int element;
	struct ListNode *next;
};

typedef struct HashTable HashTable;
struct HashTable {
	int slots;
	int size;
	ListNode **list;
};

HashTable *init_hash_table(int slots);
void destory_hash_table(HashTable *p);
ListNode * find_element(HashTable *p, int element);
int insert_hash_table(HashTable *p, int element);
int rebuild_hash_table(HashTable *p);
void print_hash_table(HashTable *p);

#endif