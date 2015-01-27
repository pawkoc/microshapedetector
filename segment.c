#include "segment.h"

Pixel* intersect(Segment* s1, Segment* s2) {

	Pixel* p;

	int x1 = s1->start->x;
	int y1 = s1->start->y;

	int x2 = s1->end->x;
	int y2 = s1->end->y;

	int x3 = s2->start->x;
	int y3 = s2->start->y;

	int x4 = s2->end->x;
	int y4 = s2->end->y;

//	printf("(%d, %d), (%d, %d), (%d, %d), (%d, %d)\n\n", x1, y1, x2, y2, x3, y3, x4, y4);

	int m = (x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4);

//	printf("(%d), (%d)\n", (x1-x2), (y1-y2));

	if(m == 0) {
		return NULL;
	}

    double x=((x1*y2-y1*x2)*(x3-x4)-(x1-x2)*(x3*y4-y3*x4)) / m;
    double y=((x1*y2-y1*x2)*(y3-y4)-(y1-y2)*(x3*y4-y3*x4)) / m;

	if (x1>=x2) {
		if (!(x2<=x&&x<=x1)) {return NULL;}
	} else {
		if (!(x1<=x&&x<=x2)) {return NULL;}
	}
	if (y1>=y2) {
		if (!(y2<=y&&y<=y1)) {return NULL;}
	} else {
		if (!(y1<=y&&y<=y2)) {return NULL;}
	}
	if (x3>=x4) {
		if (!(x4<=x&&x<=x3)) {return NULL;}
	} else {
		if (!(x3<=x&&x<=x4)) {return NULL;}
	}
	if (y3>=y4) {
		if (!(y4<=y&&y<=y3)) {return NULL;}
	} else {
		if (!(y3<=y&&y<=y4)) {return NULL;}
	}

	p = malloc(sizeof(Pixel));
	p->x = x;
	p->y = y;

	return p;
}

SegmentList* init_segment_list() {

	SegmentList* list = malloc(sizeof(SegmentList));

	list->head = NULL;
	list->size = 0;

	return list;
}

void add_first_segment(SegmentList* list, Segment* segment) {

	SegmentNode* new_segment = malloc(sizeof(SegmentNode));
	new_segment->segment = segment;

	new_segment->next = list->head;

	list->head = new_segment;

	list->size++;
}
void print_segment_list(SegmentList* list) {

	SegmentNode* segment = list->head;

	while(segment) {
		printf("SEGMENT: (%d, %d) -> (%d, %d)\n", segment->segment->start->x, segment->segment->start->y,
										segment->segment->end->x, segment->segment->end->y);
		segment = segment->next;
	}

	printf("\n");
}

List* get_all_intersection_pixels(SegmentList* segments) {

	List* results = init_list();
	Pixel* p;

	SegmentNode* i_iter, *j_iter;

	for(i_iter = segments->head; i_iter->next != NULL; i_iter = i_iter->next) {
		for(j_iter = i_iter->next; j_iter != NULL; j_iter = j_iter->next) {

			if((p = intersect(i_iter->segment, j_iter->segment)) != NULL) {

				add_sort(results, p->x, p->y);
				p = malloc(sizeof(Pixel));
			}
		}
	}

	print_list(results);

	return results;
}
