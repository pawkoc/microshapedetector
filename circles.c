#include "circles.h"
#include "sobel.h"
#include "improc.h"

unsigned char** detect_circles(unsigned char** sobel_output) {

	int i, j, k;
	int u;
	unsigned char** output = matrix_init(0);

	for(i=0; i<MAX_ROWS; i++)
		sobel_output[i][0] = 0;

	for(i=0; i<MAX_ROWS; i++)
		sobel_output[i][511] = 0;

	for(i=0; i<MAX_COLUMNS; i++)
		sobel_output[0][i] = 0;

	for(i=0; i<MAX_COLUMNS; i++)
		sobel_output[511][i] = 0;


	for(i=0; i<MAX_COLUMNS; i++) {
		for(j=0; j<MAX_ROWS; j++) {

			if((int)sobel_output[j][i] > 240) {

				for(k=j; k<MAX_ROWS; k++) {
					if((int)sobel_output[k][i] > 240) {
						u = (j+k)/2;
						output[u][i]++;
					}
				}

				break;
			}
		}
	}

	for(i=0; i<MAX_ROWS; i++) {
		for(j=0; j<MAX_COLUMNS; j++) {

			if((int)sobel_output[i][j] > 240) {

				for(k=j; k<MAX_COLUMNS; k++) {
					if((int)sobel_output[i][k] > 240) {
						u = (j+k)/2;
						output[i][u]++;
					}
				}

				break;
			}
		}
	}


	for(i=0; i<MAX_ROWS; i++) {
		for(j=0; j<MAX_COLUMNS; j++) {
//			printf("%3d ", (int)output[i][j]);
			if((int)output[i][j] != 0) {
//				printf("%d, %d\n", i, j);
				sobel_output[i][j] = 255;
				output[i][j] = 255;
			}
		}

//		printf("\n");
	}


	return output;
}

