#pragma once
#ifndef SDLIB_ARRAY_STACK_H
#define SDLIB_ARRAY_STACK_H

typedef struct array_stack array_stack;
struct array_stack {
	int top;
	int capacity;
	int *elements;
};

array_stack *init_array_stack(int capacity);
void destory_array_stack(array_stack *stack);
int is_full_array_stack(array_stack *stack);
int is_empty_array_stack(array_stack *stack);
int push_array_stack(array_stack *stack, int element);
int top_array_stack(array_stack *stack, int *element);
int pop_array_stack(array_stack *stack, int *element);

#endif