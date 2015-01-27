#ifndef PIXEL_H_
#define PIXEL_H_

#include <stdlib.h>
#include <stdio.h>

typedef struct Pixel {

	int x;
	int y;
} Pixel;

typedef struct Node {

	Pixel* pixel;
	double b_ellipse;
	struct Node* next;
}Node;

typedef struct List {

	Node* head;
	int size;
}List;

List* init_list();
void add_first(List* list, int x, int y, double val);
void add_sort(List* list, int x, int y);
void print_list(List* head);
void removeEllipseFromImage(Node* start, Node* end, double val);

#endif /* PIXEL_H_ */
