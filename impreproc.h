#ifndef IMPREPROC_H_
#define IMPREPROC_H_

#include <stdlib.h>

unsigned char** thin(unsigned char** sobel_output, int w, int h);
unsigned char** thin2(unsigned char** sobel_output, int w, int h);

#endif /* IMPREPROC_H_ */
