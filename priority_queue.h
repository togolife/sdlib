#pragma once
#ifndef SDLIB_PRIORITY_QUEUE_H
#define SDLIB_PRIORITY_QUEUE_H

typedef struct priority_queue priority_queue;
struct priority_queue {
	int capacity;
	int size;
	int *elements;
};

priority_queue *init_pri_queue(int capacity);
void destory_pri_queue(priority_queue *p);
void make_empty_pri_queue(priority_queue *p);
int insert_pri_queue(priority_queue *p, int i);
int delete_min_pri_queue(priority_queue *p);
int find_min_pri_queue(priority_queue *p);
int is_empty_pri_queue(priority_queue *p);
int is_full_pri_queue(priority_queue *p);
int relarge_pri_queue(priority_queue *p);

#endif