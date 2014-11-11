#ifndef SOBEL_H
#define SOBEL_H

#define MAX_ROWS 4
#define MAX_COLUMNS 4

#include <stdlib.h>
#include <stdio.h>

void print_matrix(int**);

int** matrix_init(int);
int** sobel(int**);

#endif
