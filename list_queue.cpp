#include "stdafx.h"

list_queue *init_list_queue(int capacity) {
	list_queue *queue = (list_queue *)malloc(sizeof(list_queue));
	if (queue == NULL) {
		return NULL;
	}
	queue->capacity = capacity;
	queue->size = 0;
	queue->head = queue->tail = NULL;
	return queue;
}

void destory_list_queue(list_queue *queue) {
	queue_node *p = queue->head;
	while (p != NULL) {
		queue_node *q = p->next;
		free(p);
		p = q;
	}
	free(queue);
}

int is_full_list_queue(list_queue *queue) {
	return queue->size == queue->capacity;
}

int is_empty_list_queue(list_queue *queue) {
	return queue->size == 0;
}

int in_list_queue(list_queue *queue, int element) {
	queue_node *p = (queue_node *)malloc(sizeof(queue_node));
	if (p == NULL) {
		return -1;
	}
	p->element = element;
	p->before = p->next = NULL;
	if (queue->head == NULL) {
		queue->head = queue->tail = p;
	}
	else {
		p->before = queue->tail;
		queue->tail->next = p;
		queue->tail = p;
	}
	queue->size += 1;
	return 0;
}

int front_list_queue(list_queue *queue, int *element) {
	if (is_empty_list_queue(queue)) {
		return -1;
	}
	*element = queue->head->element;
	return 0;
}

int out_list_queue(list_queue *queue, int *element) {
	if (is_empty_list_queue(queue)) {
		return -1;
	}
	queue_node *p = queue->head;
	*element = p->element;
	queue->size -= 1;
	queue->head = queue->head->next;
	if (queue->size == 0) {
		queue->head = queue->tail = NULL;
	}
	else {
		queue->head->before = NULL;
	}
	free(p);
	return 0;
}