#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"

int append_node(struct node **list, int data) {
	char *msg = NULL;
	struct node* new_node = NULL;
	struct node *last = NULL;

	if ((new_node = malloc(sizeof(struct node))) == NULL) {
		if (asprintf(&msg, "Failed to compile address regex\n") > 0) {                
			fprintf(stderr, msg);

			free(msg);
			msg = NULL;
		}

		return -1;
	}

	new_node->data  = data;
	new_node->next = NULL;

	/* If the list is empty then add node */
	if (*list == NULL) {
		*list = new_node;
		return 0;
	}

	last = *list;
	while (last->next != NULL) {
		last = last->next;
	}

	last->next = new_node;
	return 0;
}

void delete_nodes(struct node **list) {
	struct node *current = NULL;
	struct node *next = NULL;

	current = *list;

	while (current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}

	*list = NULL;
}

void print_nodes(struct node **tmp) {
	struct node *current = NULL;
	char *message = NULL;

	current = (*tmp);

	if (asprintf(&message, "PORT\tSTATE\n") > 0) {
		fprintf(stdout, message);
		free(message);
		message = NULL;
	}

	while (current != NULL) {
		if (asprintf(&message, "%d\topen\n", current->data) > 0) {
			fprintf(stdout, "%s", message);
		}

		current = current->next;

		free(message);
		message = NULL;
	}
}
