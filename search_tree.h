#pragma once
#ifndef SDLIB_SEARCH_TREE_H
#define SDLIB_SEARCH_TREE_H

typedef struct search_tree search_tree;
typedef struct tree_node tree_node;
struct search_tree {
	tree_node *root;
};

struct tree_node {
	int element;
	struct tree_node *lchild;
	struct tree_node *rchild;
};

search_tree *init_search_tree();
void make_empty_search_tree(search_tree *tree);
void destory_search_tree(search_tree *tree);
tree_node *find_search_tree(search_tree *tree, int element);
int insert_search_tree(search_tree *tree, int element);
void delete_search_tree(search_tree *tree, int element);
int find_min_search_tree(search_tree *tree, int *element);
int find_max_search_tree(search_tree *tree, int *element);
void first_order_print(search_tree *tree);
void middle_order_print(search_tree *tree);
void last_order_print(search_tree *tree);

#endif