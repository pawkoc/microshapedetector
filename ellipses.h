#ifndef ELLIPSES_H_
#define ELLIPSES_H_

#include "pixel.h"
#include <math.h>

typedef struct EllipseNode {

	int center_x;
	int center_y;
	double a_axis;
	double b_axis;
	struct EllipseNode* next;

}EllipseNode;

typedef struct EllipseList {

	EllipseNode* head;
	int size;

}EllipseList;

EllipseList* init_ellipse_list();
void add_first_ellipse(EllipseList* list, int x, int y, double a, double b);
void print_ellipse_list(EllipseList* head);

List* get_pixels(unsigned char** sobel_output, int w, int h);
void detect_ellipses(unsigned char** sobel_output, int w, int h, double threshold, int min_dist, EllipseList* ellipses_result_list);
double get_dist(double x1, double y1, double x2, double y2);

void detect_with_start_end(unsigned char** out, double start_x, double start_y, double end_x, double end_y);

#endif /* ELLIPSES_H_ */
