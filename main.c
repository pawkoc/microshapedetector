#include "sobel.h"

int main(int argc, char **argv) {

	char **k = matrix_init(0);

	int i, j, ka=1;
	for(i=0; i<MAX_ROWS; i++) {
		for(j=0; j<MAX_COLUMNS; j++) {
			k[i][j] = ka++;
		}
	}

	print_matrix(k);
	char **w = sobel(k);
	print_matrix(w);

	return 0;
}
