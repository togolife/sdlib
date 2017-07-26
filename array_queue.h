#pragma once
#ifndef SDLIB_ARRAY_QUEUE_H
#define SDLIB_ARRAY_QUEUE_H

typedef struct array_queue array_queue;
struct array_queue {
	int begin;
	int end;
	int capacity;
	int *elements;
};

array_queue *init_array_queue(int capacity);
void destory_array_queue(array_queue *queue);
int is_full_array_queue(array_queue *queue);
int is_empty_array_queue(array_queue *queue);
int in_array_queue(array_queue *queue, int element);
int front_array_queue(array_queue *queue, int *element);
int out_array_queue(array_queue *queue, int *element);

#endif