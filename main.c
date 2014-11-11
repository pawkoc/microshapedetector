#include "sobel.h"

int main(int argc, char **argv) {

	int **k = matrix_init(0);

	int i, j;
	for(i=0; i<MAX_ROWS; i++) {
		for(j=0; j<MAX_COLUMNS; j++) {
			k[i][j] = 1;
		}
	}

//	print_matrix(k);
	int **w = sobel(k);
	print_matrix(w);

	return 0;
}
