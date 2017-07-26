#include "stdafx.h"

priority_queue *init_pri_queue(int capacity) {
	priority_queue *head = (priority_queue *)malloc(sizeof(priority_queue));
	if (head == NULL) {
		return NULL;
	}
	head->capacity = capacity;
	head->size = 0;
	head->elements = (int *)malloc((capacity + 1) * sizeof(int));
	if (head->elements == NULL) {
		free(head);
		return NULL;
	}
	head->elements[0] = 0; // 0位置不使用
	return head;
}

void destory_pri_queue(priority_queue *p) {
	if (p != NULL) {
		free(p->elements);
		free(p);
	}
}

void make_empty_pri_queue(priority_queue *p) {
	p->size = 0;
}

int insert_pri_queue(priority_queue *p, int element) {
	if (is_full_pri_queue(p)) {
		return -1;
	}
	p->size += 1;
	int pos = p->size;
	int parent = pos / 2;
	while (parent > 0 && p->elements[parent] > element) {
		p->elements[pos] = p->elements[parent];
		pos = parent;
		parent = pos / 2;
	}
	p->elements[pos] = element;
	return 0;
}

int delete_min_pri_queue(priority_queue *p) {
	if (is_empty_pri_queue(p)) {
		return -1;
	}
	int min = p->elements[1];
	int pos = 1;
	int left = pos * 2;
	int right = left + 1;
	while (left < p->size) {
		int child = left;
		if (right < p->size && p->elements[right] < p->elements[left]) {
			child = right;
		}
		if (p->elements[child] > p->elements[p->size])
			break;
		else
			p->elements[pos] = p->elements[child];
		pos = child;
		left = pos * 2;
		right = left + 1;
	}
	p->elements[pos] = p->elements[p->size];
	p->size -= 1;
	return min;
}

int find_min_pri_queue(priority_queue *p) {
	if (is_empty_pri_queue(p)) {
		return 0x7fffffff;
	}
	return p->elements[1];
}

int is_empty_pri_queue(priority_queue *p) {
	return p->size == 0;
}

int is_full_pri_queue(priority_queue *p) {
	return p->size == p->capacity;
}

int relarge_pri_queue(priority_queue *p) {
	int *reptr = (int *)malloc((p->capacity * 2 + 1) * sizeof(int));
	if (reptr == NULL) {
		return -1;
	}
	memcpy(reptr, p->elements, (p->capacity + 1) * sizeof(int));
	p->capacity *= 2;
	free(p->elements);
	p->elements = reptr;
	return 0;
}
