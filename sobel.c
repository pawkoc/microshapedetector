#include "sobel.h"

//just another random comment

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

void print_matrix(char** m) {

	int i, j;
	for(i=0; i<MAX_ROWS; i++) {
		for(j=0; j<MAX_COLUMNS; j++) {
			printf("%d ", m[i][j]);
		}
		printf("\n");
	}
}

char** matrix_init(int of) {
	char** matrix = malloc((MAX_ROWS + of) * sizeof(char*));

	if(!matrix) {
		printf("Error malloc\n");
		exit(-1);
	}

	int i, j;
	for(i=0; i<MAX_ROWS+of; i++) {
		if((matrix[i] = malloc((MAX_COLUMNS + of) * sizeof(char))) == NULL)  {
			printf("Error malloc\n");
			exit(-1);
		}
	}

	for(i=0; i<MAX_ROWS+of; i++) {
		for(j=0; j<MAX_COLUMNS+of; j++) {
			matrix[i][j] = '\0';
		}
	}

	return matrix;
}

void copy_offset(char** m1, char** m2) {

	int i, j;
	for(i=0; i<MAX_ROWS; i++) {
		for(j=0; j<MAX_COLUMNS; j++) {
			m1[i+1][j+1] = m2[i][j];
		}
	}
}

char** sobel(char** matrix) {

	int i, j, k, l, sumX, sumY, sum_all;
	char** output = matrix_init(0);

	char** copy_with_offset = matrix_init(2);

	copy_offset(copy_with_offset, matrix);

	for(i=1; i<=MAX_ROWS; i++) {
		for(j=1; j<=MAX_COLUMNS; j++) {

			sumX = sumY = 0;
			for(k=-1; k<=1; k++) {
				for(l=-1; l<=1; l++){
					sumX += (int)copy_with_offset[k+i][l+j]*kernel_x[k+1][l+1];
					sumY += (int)copy_with_offset[k+i][l+j]*kernel_y[k+1][l+1];
				}
			}

			sum_all = abs(sumX) + abs(sumY);

			if(sum_all > 255)
				sum_all = 255;

			else if(sum_all < 0)
				sum_all = 0;

			output[i-1][j-1] = (char)sum_all;
		}
	}

	return output;
}
