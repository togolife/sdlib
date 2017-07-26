#include "stdafx.h"

static int hash(HashTable *p, int element) {
	return element % (p->slots);
}

static int next_prime(int i) {
	if (i <= 3) {
		return 3;
	}
	else {
		while (1) {
			int j = 2, k = i/2;
			for (; j <= k; ++j) {
				if (i % j == 0)
					break;
			}
			if (j > k) {
				return i;
			}
			else {
				++i;
			}
		}
	}
}

HashTable *init_hash_table(int slots) {
	HashTable *p = (HashTable *)malloc(sizeof HashTable);
	if (p == NULL) {
		return NULL;
	}
	p->size = 0;
	p->slots = next_prime(slots);
	p->list = (ListNode **)malloc(p->slots * sizeof (ListNode *));
	if (p->list == NULL) {
		free(p);
		return NULL;
	}
	for (int i = 0; i < p->slots; ++i) {
		p->list[i] = NULL;
	}
	return p;
}
void destory_hash_table(HashTable *p) {
	int i = 0;
	for (; i < p->slots; ++i) {
		ListNode *t = p->list[i];
		while (t != NULL) {
			ListNode *next = t->next;
			free(t);
			t = next;
		}
	}
	free(p->list);
	free(p);
}
ListNode * find_element(HashTable *p, int element) {
	int slot = hash(p, element);
	ListNode *t = p->list[slot];
	while (t != NULL) {
		if (t->element == element) {
			break;
		}
		else {
			t = t->next;
		}
	}
	return t;
}
int insert_hash_table(HashTable *p, int element) {
	int slot = hash(p, element);
	ListNode *t = p->list[slot];
	ListNode *q = t;
	while (t != NULL && t->element != element) {
		q = t;
		t = t->next;
	}
	if (t == NULL) {
		ListNode *n = (ListNode *)malloc(sizeof ListNode);
		if (n == NULL) {
			printf("insert element malloc failed!\n");
			return -1;
		}
		n->element = element;
		n->next = NULL;
		if (q == NULL) {
			p->list[slot] = n;
		}
		else {
			q->next = n;
		}
		p->size += 1;
		if (p->size > p->slots * 2) {
			return rebuild_hash_table(p);
		}
	}
	return 0;
}
int rebuild_hash_table(HashTable *p) {
	int slot = p->slots * 2;
	slot = next_prime(slot);
	ListNode* *new_list = (ListNode**)malloc(slot * sizeof(ListNode *));
	if (new_list == NULL) {
		printf("rebuild hash table malloc failed!\n");
		return -1;
	}
	for (int i = 0; i < slot; ++i) {
		new_list[i] = NULL;
	}
	int org_slot = p->slots;
	p->slots = slot;
	for (int i = 0; i < org_slot; ++i) {
		ListNode *t = p->list[i];
		while (t != NULL) {
			ListNode *q = t->next;
			int pos = hash(p, t->element);
			t->next = new_list[pos];
			new_list[pos] = t;
			t = q;
		}
	}
	free(p->list);
	p->list = new_list;
	return 0;
}
void print_hash_table(HashTable *p) {
	printf("hash table info: size[%d], slots[%d]\n", p->size, p->slots);
	for (int i = 0; i < p->slots; ++i) {
		printf("slot[%d]:", i + 1);
		ListNode *t = p->list[i];
		while (t != NULL) {
			printf("%d ", t->element);
			t = t->next;
		}
		printf("\n");
	}
}
