#include "pixel.h"

List* init_list() {

	List* list = malloc(sizeof(List));

	list->head = NULL;
	list->size = 0;

	return list;
}

void add_first(List* list, int x, int y) {

	Pixel* new_pixel = malloc(sizeof(Pixel));
	new_pixel->x = x;
	new_pixel->y = y;

	Node* new_node = malloc(sizeof(Node));

	new_node->pixel = new_pixel;
	new_node->b_ellipse = 0.;

	new_node->next = list->head;
	list->head = new_node;

	list->size++;
}

void add_sort(List* list, int x, int y) {

	Pixel* new_pixel = malloc(sizeof(Pixel));
	new_pixel->x = x;
	new_pixel->y = y;

	Node* new_node = malloc(sizeof(Node));

	new_node->pixel = new_pixel;
	new_node->b_ellipse = 0.;

	list->size++;

	if(list->head == NULL) {
		new_node->next = NULL;
		list->head = new_node;

		return;
	}

	Node *i_node, *prev_node = NULL;

	i_node = list->head;

	while(i_node && i_node->pixel->y < y) {
		prev_node = i_node;
		i_node = i_node->next;
	}

	if(i_node == NULL) {

		prev_node->next = new_node;
		new_node->next = NULL;

		return;
	}

//	konflikt
	while(i_node && i_node->pixel->y == y && i_node->pixel->x < x) {
		prev_node = i_node;
		i_node = i_node->next;
	}

	if(i_node == list->head) {

		list->head = new_node;
		new_node->next = i_node;

		return;
	}

	if(i_node == NULL) {
		prev_node->next = new_node;
		new_node->next = NULL;

		return;
	}

	new_node->next = prev_node->next;
	prev_node->next = new_node;
}

void print_list(List* list) {

	Node* head = list->head;

	while(head) {
		printf("(%d, %d) / %lf ", head->pixel->x, head->pixel->y, head->b_ellipse);
		head = head->next;
	}

	printf("\n");
}
