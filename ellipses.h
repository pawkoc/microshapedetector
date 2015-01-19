#ifndef ELLIPSES_H_
#define ELLIPSES_H_

#include "pixel.h"
#include <math.h>

List* get_pixels(unsigned char** sobel_output, int w, int h);
void detect_ellipses(unsigned char** sobel_output, int w, int h, double threshold, int min_dist);
double get_dist(double x1, double y1, double x2, double y2);

void detect_with_start_end(unsigned char** out, double start_x, double start_y, double end_x, double end_y);

#endif /* ELLIPSES_H_ */
