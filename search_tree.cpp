#include "stdafx.h"

search_tree *init_search_tree() {
	search_tree *tree = (search_tree *)malloc(sizeof(search_tree));
	if (tree == NULL) {
		return NULL;
	}
	tree->root = NULL;
	return tree;
}

static void free_tree_node(tree_node *node) {
	if (node == NULL) return;
	if (node->lchild) {
		free_tree_node(node->lchild);
	}
	if (node->rchild) {
		free_tree_node(node->rchild);
	}
	free(node);
}

void make_empty_search_tree(search_tree *tree) {
	if (tree == NULL) return;
	free_tree_node(tree->root);
	tree->root = NULL;
}

void destory_search_tree(search_tree *tree) {
	make_empty_search_tree(tree);
	free(tree);
}

tree_node *find_search_tree(search_tree *tree, int element) {
	if (tree == NULL) return NULL;
	tree_node *p = tree->root;
	while (p) {
		if (p->element == element)
			break;
		if (p->element > element)
			p = p->lchild;
		else
			p = p->rchild;
	}
	return p;
}

int insert_search_tree(search_tree *tree, int element) {
	if (tree == NULL) return -1;
	tree_node *p, *q;
	p = q = tree->root;
	while (p) {
		if (p->element == element)
			return 0;
		q = p;
		if (p->element > element)
			p = p->lchild;
		else
			p = p->rchild;
	}
	tree_node *t = (tree_node *)malloc(sizeof(tree_node));
	if (t == NULL)
		return -1;
	t->element = element;
	t->lchild = t->rchild = NULL;
	if (tree->root == NULL) {
		tree->root = t;
	}
	else {
		if (q->element > element)
			q->lchild = t;
		else
			q->rchild = t;
	}
	return 0;
}

// 删除一个节点，如果是叶子节点则直接删除；如果有一个子节点，则子节点替换该节点；
// 如果有两个子节点，则把右子树的最左节点替换该节点，再删除右子树最左节点
void delete_search_tree(search_tree *tree, int element) {
	if (tree == NULL || tree->root == NULL) return;
	tree_node *p, *q;
	p = q = tree->root;
	int child_flag = 0;
	while (p) {
		if (p->element == element)
			break;
		q = p;
		if (p->element > element) {
			p = p->lchild;
			child_flag = 1;
		}
		else {
			p = p->rchild;
			child_flag = 2;
		}
	}
	if (p) {// find the element
		if (p->lchild == NULL && p->rchild == NULL) {
			if (p == tree->root) {
				tree->root = NULL;
			}
			else {
				child_flag == 1 ? (q->lchild = NULL) : (q->rchild = NULL);
			}
			free(p);
		}
		else if (p->lchild == NULL) {
			if (p == tree->root) {
				tree->root = p->rchild;
			}
			else {
				child_flag == 1 ? (q->lchild = p->rchild) : (q->rchild = p->rchild);
			}
			free(p);
		}
		else if (p->rchild == NULL) {
			if (p == tree->root) {
				tree->root = p->lchild;
			}
			else {
				child_flag == 1 ? (q->lchild = p->lchild) : (q->rchild = p->lchild);
			}
			free(p);
		}
		else {
			// 首先找右子树最左节点
			tree_node *t = p->rchild;
			q = p;
			child_flag = 2;
			while (t->lchild) {
				q = t;
				child_flag = 1;
				t = t->lchild;
			}
			// 值替换
			p->element = t->element;
			// 删除右子树最左节点
			child_flag == 1 ? (q->lchild = t->rchild) : (q->rchild = t->rchild);
			free(t);
		}
	}
	return;
}

int find_min_search_tree(search_tree *tree, int *element) {
	if (tree == NULL || tree->root == NULL) return -1;
	tree_node *p = tree->root;
	while (p->lchild)
		p = p->lchild;
	*element = p->element;
	return 0;
}

int find_max_search_tree(search_tree *tree, int *element) {
	if (tree == NULL || tree->root == NULL) return -1;
	tree_node *p = tree->root;
	while (p->rchild)
		p = p->rchild;
	*element = p->element;
	return 0;
}

static void first_order_tree_node(tree_node *root) {
	if (root == NULL) return;
	printf("%d ", root->element);
	first_order_tree_node(root->lchild);
	first_order_tree_node(root->rchild);
}

void first_order_print(search_tree *tree) {
	if (tree == NULL) return;
	first_order_tree_node(tree->root);
}

static void middle_order_tree_node(tree_node *root) {
	if (root == NULL) return;
	middle_order_tree_node(root->lchild);
	printf("%d ", root->element);
	middle_order_tree_node(root->rchild);
}

void middle_order_print(search_tree *tree) {
	if (tree == NULL) return;
	middle_order_tree_node(tree->root);
}

static void last_order_tree_node(tree_node *root) {
	if (root == NULL) return;
	last_order_tree_node(root->lchild);
	last_order_tree_node(root->rchild);
	printf("%d ", root->element);
}

void last_order_print(search_tree *tree) {
	if (tree == NULL) return;
	last_order_tree_node(tree->root);
}

/*
// 遍历非递归方式
void first_order_print_by_stack(search_tree *tree) {
  if (tree == NULL) return;
  if (tree->root == NULL) return;
  Stack *s = initStack(100);
  if (s == NULL) return;
  pushStack(s, tree->root);
  while (!isempty(s)) {
    ElementType p = topStack(s);
    printf ("%d ", p->element);
    popStack(s);
    if (p->rchild)
      pushStack(s, p->rchild);
    if (p->lchild)
      pushStack(s, p->lchild);
  }
  destoryStack(s);
}

void middle_order_print_by_stack(search_tree *tree) {
  if (tree == NULL) return;
  if (tree->root == NULL) return;
  Stack *s = initStack(100);
  if (s == NULL) return;
  tree_node *p = tree->root;
  while (p) {
    pushStack(s, p);
    p = p->lchild;
  }
  while (!isempty(s)) {
    ElementType p = topStack(s);
    printf ("%d ", p->element);
    popStack(s);
    if (p->rchild) {
      tree_node *q = p->rchild;
      while (q) {
        pushStack(s, q);
        q = q->lchild;
      }
    }
  }
  destoryStack(s);
}

void last_order_print_by_stack(search_tree *tree) {
  if (tree == NULL) return;
  if (tree->root == NULL) return;
  Stack *s = initStack(100);
  if (s == NULL) return;
  tree_node *p = tree->root;
  while (p) {
    pushStack(s, p);
    p = p->lchild;
  }
  tree_node *last_pop = NULL;
  while (!isempty(s)) {
    ElementType p = topStack(s);
    if (p->rchild && last_pop != p->rchild) {
      tree_node *q = p->rchild;
      while (q) {
        pushStack(s, q);
        q = q->lchild;
      }
    } else {
      printf ("%d ", p->element);
      popStack(s);
      last_pop = p;
    }
  }
  destoryStack(s);
}
*/
