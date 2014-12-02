#include "hough.h"
#include "sobel.h"

int** hough(unsigned char** sobel_output, int w, int h) {

	int _accu_h = round(sqrt(2.0)*h);
	int _accu_w = 180;

	printf("Wymiar h: %d\n", _accu_h);

	int** _accu = malloc(_accu_h * sizeof(int*));

	if(_accu == NULL) {
		printf("nieudana alokacja\n");
		exit(-1);
	}

	int i;
	for (i = 0; i < _accu_h; i++) {
		_accu[i] = malloc(_accu_w * sizeof(int));

		if(_accu[i] == NULL) {
			printf("nieudana alokacja\n");
			exit(-1);
		}
	}

	int j;
	for(i=0; i<_accu_h; i++) {
		for(j=0; j<_accu_w; j++){
			_accu[i][j] = 0;
		}
	}

	printf("accu_w: %d, accu_h: %d\n", _accu_w, _accu_h);

	int licznik = 0;

	int x, y, t;
	for (x = 0; x < w; x++) {
		for (y = 0; y < h; y++) {
			if ((int) sobel_output[x][y] > 250) {

				for (t = 0; t < 180; t++) {

					double c_x = x/2.;
					double c_y = y/2.;
					double r = ((double)x - c_x) * cos((double) t * DEG2RAD)
							+ ((double)y - c_y) * sin((double) t * DEG2RAD);


//					_accu[(int)((round(r + hough_h) * 180.0))][t]++;
					int r_int = (int)r + _accu_h/2;

//					printf("OK %d <-- %lf\n", r_int, r);

					_accu[r_int][t]++;
					if(_accu[r_int][t] < 0) {
//						printf("blad\n");
						licznik++;
					}
				}
			}
		}
	}

		for(i=0; i<_accu_h; i++) {
			for(j=0; j<_accu_w; j++){
//				printf("%3d ", _accu[i][j]);
				if(_accu[i][j] > 240) {
					printf("r: %3d, fi: %3d, wart: %d\n", i, j, _accu[i][j]);
				}
			}
//			printf(":%d\n", i);
		}

	return _accu;
}

void reverse_hough(int** accu, int w, int h, int thr) {

	unsigned char** output = matrix_init(0);

	int x, y;
	int r, t;
	for (r = 0; r < h; r++) {
		for (t = 0; t < w; t++) {

			if(accu[r][t] >= thr) {



			}
		}
	}

}
