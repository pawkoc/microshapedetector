#include "impreproc.h"
#include "sobel.h"
#include "hough.h"

unsigned char** thin(unsigned char** input, int w, int h) {

	unsigned char** output = matrix_init(0);
	copy(output, input);

	int i, j, iter=1, k;

	int A, B, m1, m2;
	int p[8];

	for(iter=0; iter<=1; iter++) {
		for(i=1; i<w-1; i++) {
			for(j=1; j<h-1; j++) {

				B=0;

					p[0] = (int)input[i-1][j];
					p[1] = (int)input[i-1][j+1];
					p[2] = (int)input[i][j+1];
					p[3] = (int)input[i+1][j+1];
					p[4] = (int)input[i+1][j];
					p[5] = (int)input[i+1][j-1];
					p[6] = (int)input[i][j-1];
					p[7] = (int)input[i-1][j-1];

					A  = (p[0] == 0 && p[1] != 0) + (p[1] == 0 && p[2] != 0) +
							 (p[2] == 0 && p[3] != 0) + (p[3] == 0 && p[4] != 0) +
							 (p[4] == 0 && p[5] != 0) + (p[5]== 0 && p[6] != 0) +
							 (p[6] == 0 && p[7] != 0) + (p[7] == 0 && p[0] != 0);

					for(k=0; k<8; k++) {
						if(p[k] != 0) {
							B++;
						}
					}

					m1 = iter == 0 ? (p[0] * p[2] * p[4]) : (p[0] * p[2] * p[6]);
					m2 = iter == 0 ? (p[2] * p[4] * p[6]) : (p[0] * p[4] * p[6]);

					if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0) {
						output[i][j] = (char)0;
					}




//		            int C  = (!p[0] && (p[1] | p[2])) + (!p[2] && (p[3] | p[4])) +
//		                     (!p[4] && (p[5] | p[6])) + (!p[6] && (p[7] | p[0]));
//		            int N1 = (p[7] | p[0]) + (p[1] | p[2]) + (p[3] | p[4]) + (p[5] | p[6]);
//		            int N2 = (p[0] | p[1]) + (p[2] | p[3]) + (p[4] | p[5]) + (p[6] | p[7]);
//		            int N  = N1 < N2 ? N1 : N2;
//		            int m  = iter == 0 ? ((p[4] | p[5] | !p[7]) & p[6]) : ((p[0] | p[1] | !p[3]) & p[2]);
//
//		            if (C == 1 && (N >= 2 && N <= 3) && m == 0)
//		                output[i][j] = 1;
			}
		}
	}

	return output;
}

unsigned char** thin2(unsigned char** sobel_output, int w, int h) {


	unsigned char** output = matrix_init(0);
	copy(output, sobel_output);

	unsigned char** tmp;

	int i, j, roznica=1;

	while(roznica != 0) {
		roznica = 0;
		tmp = thin(output, w, h);

		for(i=0; i<w; i++) {
			for(j=0; j<h; j++) {

				if(tmp[i][j] != output[i][j]) {
					output[i][j] = 0;
					roznica = 1;
				}
			}
		}
	}

	clear_2((void**)tmp, w);

	return output;
}
