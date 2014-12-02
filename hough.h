
#ifndef HOUGH_H_
#define HOUGH_H_

#ifndef M_PI
#define M_PI 3.1415
#endif

#define DEG2RAD ((M_PI)/(180.0))

int** hough(unsigned char** sobel_output, int w, int h);

#endif /* HOUGH_H_ */
