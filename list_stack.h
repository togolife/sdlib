#pragma once
#ifndef SDLIB_LIST_STACK_H
#define SDLIB_LIST_STACK_H

typedef struct list_stack list_stack;
typedef struct stack_node stack_node;
struct list_stack {
	int capacity;
	int size;
	struct stack_node *head;
	struct stack_node *tail;
};

struct stack_node {
	int element;
	struct stack_node *before;
	struct stack_node *next;
};

list_stack *init_list_stack(int capacity);
void destory_list_stack(list_stack *stack);
int is_full_list_stack(list_stack *stack);
int is_empty_list_stack(list_stack *stack);
int push_list_stack(list_stack *stack, int element);
int top_list_stack(list_stack *stack, int *element);
int pop_list_stack(list_stack *stack, int *element);

#endif