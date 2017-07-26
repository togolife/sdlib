#include "stdafx.h"

array_queue *init_array_queue(int capacity) {
	if (capacity <= 0) {
		return NULL;
	}
	array_queue *queue = (array_queue *)malloc(sizeof(array_queue));
	if (queue == NULL) {
		return NULL;
	}
	queue->begin = 0;
	queue->end = 0;
	queue->capacity = capacity;
	queue->elements = (int *)malloc(capacity * sizeof(int));
	if (queue->elements == NULL) {
		free(queue);
		return NULL;
	}
	return queue;
}

void destory_array_queue(array_queue *queue) {
	free(queue->elements);
	free(queue);
}

int is_full_array_queue(array_queue *queue) {
	return (queue->end + 1) % queue->capacity == queue->begin;
}

int is_empty_array_queue(array_queue *queue) {
	return queue->begin == queue->end;
}

static int relarge_array_queue(array_queue *queue) {
	int *new_ele = (int *)malloc(queue->capacity * 2 * sizeof(int));
	if (new_ele == NULL) {
		return -1;
	}
	if (queue->end > queue->begin) {
		memcpy(new_ele + queue->begin, queue->elements + queue->begin, (queue->end - queue->begin) * sizeof(int));
	}
	else {
		memcpy(new_ele, queue->elements, queue->end * sizeof(int));
		memcpy(new_ele + queue->capacity + queue->begin, queue->elements + queue->begin, (queue->capacity - queue->begin) * sizeof(int));
		queue->begin += queue->capacity;
	}
	queue->capacity *= 2;
	free(queue->elements);
	queue->elements = new_ele;
	return 0;
}

int in_array_queue(array_queue *queue, int element) {
	if (is_full_array_queue(queue)) {
		if (relarge_array_queue(queue) != 0) {
			return -1;
		}
	}
	queue->elements[queue->end] = element;
	queue->end = (queue->end + 1) % queue->capacity;
	return 0;
}

int front_array_queue(array_queue *queue, int *element) {
	if (is_empty_array_queue(queue)) {
		return -1;
	}
	*element = queue->elements[queue->begin];
	return 0;
}

int out_array_queue(array_queue *queue, int *element) {
	if (is_empty_array_queue(queue)) {
		return -1;
	}
	*element = queue->elements[queue->begin];
	queue->begin = (queue->begin + 1) % queue->capacity;
	return 0;
}
