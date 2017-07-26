#include "stdafx.h"

array_stack *init_array_stack(int capacity) {
	array_stack *stack = (array_stack *)malloc(sizeof(array_stack));
	if (stack == NULL) {
		return NULL;
	}
	stack->top = -1;
	stack->capacity = capacity;
	stack->elements = (int *)malloc(capacity * sizeof(int));
	if (stack->elements == NULL) {
		free(stack);
		return NULL;
	}
	return stack;
}

void destory_array_stack(array_stack *stack) {
	free(stack->elements);
	free(stack);
}

int is_full_array_stack(array_stack *stack) {
	return stack->top + 1 == stack->capacity;
}

int is_empty_array_stack(array_stack *stack) {
	return stack->top == -1;
}

static int relarge_array_stack(array_stack *stack) {
	int *new_ele = (int *)malloc(stack->capacity * 2 * sizeof(int));
	if (new_ele == NULL) {
		return -1;
	}
	memcpy(new_ele, stack->elements, stack->capacity * sizeof(int));
	stack->capacity *= 2;
	free(stack->elements);
	stack->elements = new_ele;
	return 0;
}

int push_array_stack(array_stack *stack, int element) {
	if (is_full_array_stack(stack)) {
		if (relarge_array_stack(stack) != 0) {
			return -1;
		}
	}
	stack->elements[++(stack->top)] = element;
	return 0;
}

int top_array_stack(array_stack *stack, int *element) {
	if (is_empty_array_stack(stack)) {
		return -1;
	}
	*element = stack->elements[stack->top];
	return 0;
}

int pop_array_stack(array_stack *stack, int *element) {
	if (is_empty_array_stack(stack)) {
		return -1;
	}
	*element = stack->elements[stack->top];
	stack->top -= 1;
	return 0;
}
