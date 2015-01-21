
#ifndef HOUGH_H_
#define HOUGH_H_

#ifndef M_PI
#define M_PI 3.1415
#endif

#define MIN(X,Y) (X)<(Y)?(X):(Y)

#define DEG2RAD ((M_PI)/(180.0))

int** hough(unsigned char** sobel_output, int w, int h, double threshold);

void clear_2(void** array, int w);
void clear_3(void*** array, int w, int h);

#endif /* HOUGH_H_ */
