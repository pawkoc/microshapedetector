#ifndef SEGMENT_H_
#define SEGMENT_H_

#include "pixel.h"

typedef struct Segment {

	Pixel* start;
	Pixel* end;

} Segment;

typedef struct SegmentNode {

	Segment* segment;
	struct SegmentNode* next;

} SegmentNode;

typedef struct SegmentList {

	SegmentNode* head;
	int size;

}SegmentList;

typedef struct Intersection {

	Pixel* common;
	Segment* s1;
	Segment* s2;

} Intersection;

SegmentList* init_segment_list();
void add_first_segment(SegmentList* list, Segment* segment);
void print_segment_list(SegmentList* list);

Pixel* intersect(Segment* s1, Segment* s2);
List* get_all_intersection_pixels(SegmentList* segments);

#endif /* SEGMENT_H_ */
