#ifndef ELLIPSES_H_
#define ELLIPSES_H_

#include "pixel.h"
#include <math.h>

List* get_pixels(unsigned char** sobel_output, int w, int h);
void detect_ellipses(unsigned char** sobel_output, int w, int h, double threshold, int min_dist);

#endif /* ELLIPSES_H_ */
