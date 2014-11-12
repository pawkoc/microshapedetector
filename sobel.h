#ifndef SOBEL_H
#define SOBEL_H

#define MAX_ROWS 3
#define MAX_COLUMNS 3

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void print_matrix(char**);

char** matrix_init(int);
char** sobel(char**);

#endif
