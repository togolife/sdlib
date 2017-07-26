#include "stdafx.h"

list_stack *init_list_stack(int capacity) {
	list_stack *stack = (list_stack *)malloc(sizeof(list_stack));
	if (stack == NULL) {
		return NULL;
	}
	stack->capacity = capacity;
	stack->size = 0;
	stack->head = stack->tail = NULL;
	return stack;
}

void destory_list_stack(list_stack *stack) {
	stack_node *p = stack->head;
	while (p != NULL) {
		stack_node *q = p->next;
		free(p);
		p = q;
	}
	free(stack);
}

int is_full_list_stack(list_stack *stack) {
	return stack->size == stack->capacity;
}

int is_empty_list_stack(list_stack *stack) {
	return stack->size == 0;
}

int push_list_stack(list_stack *stack, int element) {
	stack_node *p = (stack_node *)malloc(sizeof(stack_node));
	if (p == NULL) {
		return -1;
	}
	p->element = element;
	p->before = p->next = NULL;
	if (stack->head == NULL) {
		stack->head = stack->tail = p;
	}
	else {
		p->before = stack->tail;
		stack->tail->next = p;
		stack->tail = p;
	}
	stack->size += 1;
	return 0;
}

int top_list_stack(list_stack *stack, int *element) {
	if (is_empty_list_stack(stack)) {
		return -1;
	}
	*element = stack->tail->element;
	return 0;
}

int pop_list_stack(list_stack *stack, int *element) {
	if (is_empty_list_stack(stack)) {
		return -1;
	}
	stack_node *p = stack->tail;
	*element = p->element;
	stack->size -= 1;
	stack->tail = stack->tail->before;
	if (stack->size == 0) {
		stack->head = stack->tail = NULL;
	}
	else {
		stack->tail->next = NULL;
	}
	free(p);
	return 0;
}
