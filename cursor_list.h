#pragma once
#ifndef SDLIB_CURSOR_LIST_H
#define SDLIB_CURSOR_LIST_H

struct cursor_node {
	int value;
	int next;
};

typedef struct cursor_node cursor_list;

cursor_list *init_cursor_list(int space);
void destory_cursor_list(cursor_list *list);
int insert_cursor_list(cursor_list *list, int element);
int find_cursor_list(cursor_list *list, int element);
int delete_cursor_list(cursor_list *list, int element);
void print_cursor_list(cursor_list *list);

#endif