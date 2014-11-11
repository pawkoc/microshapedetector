#include "sobel.h"

int kernel_x[3][3] = {
		{-1, 0, 1},
		{-2, 0, 2},
		{-1, 0, 1}
};

int kernel_y[3][3] = {
		{1, 2, 1},
		{0, 0, 0},
		{-1, -2, -1}
};

void print_matrix(int** m) {

	int i, j;
	for(i=0; i<MAX_ROWS; i++) {
		for(j=0; j<MAX_COLUMNS; j++) {
			printf("%d ", m[i][j]);
		}
		printf("\n");
	}
}

int** matrix_init(int of) {
	int** matrix = malloc((MAX_ROWS + of) * sizeof(int*));

	if(!matrix) {
		printf("Error malloc\n");
		exit(-1);
	}

	int i, j;
	for(i=0; i<MAX_ROWS+of; i++) {
		if((matrix[i] = malloc((MAX_COLUMNS + of) * sizeof(int))) == NULL)  {
			printf("Error malloc\n");
			exit(-1);
		}
	}

	for(i=0; i<MAX_ROWS+of; i++) {
		for(j=0; j<MAX_COLUMNS+of; j++) {
			matrix[i][j] = 0;
		}
	}

	return matrix;
}

void copy_offset(int** m1, int** m2) {

	int i, j;
	for(i=0; i<MAX_ROWS; i++) {
		for(j=0; j<MAX_COLUMNS; j++) {
			m1[i+1][j+1] = m2[i][j];
		}
	}
}

int** sobel(int** matrix) {

	int i, j, k, l;
//	int** outcome = matrix_init(0);

	int** Gx = matrix_init(0);
	int** Gy = matrix_init(0);

	int** copy_with_offset = matrix_init(2);

	copy_offset(copy_with_offset, matrix);

	for(i=1; i<=MAX_ROWS; i++) {
		for(j=1; j<=MAX_COLUMNS; j++) {

			for(k=-1; k<=1; k++) {
				for(l=-1; l<=1; l++){
					Gx[i-1][j-1] += copy_with_offset[k+i][l+j]*kernel_x[k+1][l+1];
					Gy[i-1][j-1] += copy_with_offset[k+i][l+j]*kernel_y[k+1][l+1];
				}
			}
		}
	}

//	for(i=0; i<MAX_ROWS; i++) {
//		for(j=0; j<MAX_COLUMNS; j++) {
//			outcome[i][j] = Gx[i+1][j+1];
//		}
//	}

	return Gx;
}
