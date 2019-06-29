#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct node {
	int data;
	struct node *next;
};

int append_node(struct node **list, int data);
void print_nodes(struct node **tmp);
void delete_nodes(struct node **tmp);

#endif
