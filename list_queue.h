#pragma once
#ifndef SDLIB_LIST_QUEUE_H
#define SDLIB_LIST_QUEUE_H

typedef struct list_queue list_queue;
typedef struct queue_node queue_node;
struct list_queue {
	int capacity;
	int size;
	struct queue_node *head;
	struct queue_node *tail;
};

struct queue_node {
	int element;
	struct queue_node *before;
	struct queue_node *next;
};

list_queue *init_list_queue(int capacity);
void destory_list_queue(list_queue *queue);
int is_full_list_queue(list_queue *queue);
int is_empty_list_queue(list_queue *queue);
int in_list_queue(list_queue *queue, int element);
int front_list_queue(list_queue *queue, int *element);
int out_list_queue(list_queue *queue, int *element);

#endif