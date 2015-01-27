#include "pixel.h"

List* init_list() {

	List* list = malloc(sizeof(List));

	list->head = NULL;
	list->size = 0;

	return list;
}

void add_first(List* list, int x, int y, double val) {

	Pixel* new_pixel = malloc(sizeof(Pixel));
	new_pixel->x = x;
	new_pixel->y = y;

	Node* new_node = malloc(sizeof(Node));

	new_node->pixel = new_pixel;
	new_node->b_ellipse = val;

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
		printf("(%d, %d) ", head->pixel->x, head->pixel->y);
		head = head->next;
	}

	printf("\n");
}

void removeEllipseFromImage(Node* start, Node* end, double val) {

	Node* prev = start;
	Node* tmp;
	double min, max;
	printf("start: (%d, %d), end: (%d, %d)\n", start->pixel->x, start->pixel->y, end->pixel->x, end->pixel->y);

	for(start = start->next; start != end;) {

		if((((start->b_ellipse - val) >= 0.) && (start->b_ellipse - val) < 0.0000001) || (((start->b_ellipse - val) <= 0.) &&(start->b_ellipse - val) > -0.0000001)) {

//			printf("prev: (%d, %d), dif = %lf\n", prev->pixel->x, prev->pixel->y, (start->b_ellipse - val));
			prev->next = start->next;

			tmp = start;
			start = start->next;
			tmp->next = NULL;

			free(tmp);
		} else {
			start = start->next;
			prev = prev->next;
		}
	}
}
