#include "stdafx.h"

cursor_list *init_cursor_list(int space) {
	cursor_list *p = (cursor_list *)malloc(space * sizeof cursor_list);
	if (p == NULL) {
		return NULL;
	}
	int i = 0;
	for (; i < space - 1; ++i) {
		p[i].value = 0;
		p[i].next = i + 1;
	}
	p[i].value = 0;
	p[i].next = 0;
	return p;
}

void destory_cursor_list(cursor_list *list) {
	free(list);
}

int insert_cursor_list(cursor_list *list, int element) {
	if (list == NULL) {
		return -2;
	}
	if (list[0].next == 0) {
		printf("cursor list is full!\n");
		return -1;
	}
	int pos = list[0].next;
	list[0].next = list[pos].next;
	list[pos].value = element;
	list[pos].next = list[0].value;
	list[0].value = pos;
	return 0;
}

int find_cursor_list(cursor_list *list, int element) {
	if (list == NULL) return -1;
	int pos = list[0].value;
	while (pos != 0) {
		if (list[pos].value == element)
			break;
		else
			pos = list[pos].next;
	}
	return pos;
}

int delete_cursor_list(cursor_list *list, int element) {
	if (list == NULL) return -1;
	int before, pos;
	before = pos = list[0].value;
	while (pos != 0) {
		if (list[pos].value == element)
			break;
		else
		{
			before = pos;
			pos = list[pos].next;
		}
	}
	if (pos == 0) {
		return 0;
	}
	if (list[0].value == pos)
		list[0].value = list[pos].next;
	else
		list[before].next = list[pos].next;
	list[pos].next = list[0].next;
	list[0].next = pos;
	return 0;
}
void print_cursor_list(cursor_list *list) {
	if (list == NULL)
		return;
	printf ("pos 0 values is: value[%d],next[%d]\n", list[0].value,list[0].next);
	printf("empty pos is :");
	int pos = list[0].next;
	while (pos != 0) {
		printf("%d ", pos);
		pos = list[pos].next;
	}
	printf("\n");
	printf("list num is:\n");
	pos = list[0].value;
	while (pos != 0) {
		printf("value[%d],pos[%d]\n", list[pos].value, pos);
		pos = list[pos].next;
	}
	printf("\n");
}