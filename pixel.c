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
	new_node->next = list->head;
	list->head = new_node;

	list->size++;
}

void print_list(List* list) {

	Node* head = list->head;

	while(head) {
		printf("(%d, %d) ", head->pixel->x, head->pixel->y);
		head = head->next;
	}

	printf("\n");
}
