#ifndef SOBEL_H
#define SOBEL_H

#define MAX_ROWS 512
#define MAX_COLUMNS 512

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void print_matrix(unsigned char**);
void copy(unsigned char** m1, unsigned char** m2);

unsigned char** matrix_init(int);
unsigned char** sobel(unsigned char**);

#endif
