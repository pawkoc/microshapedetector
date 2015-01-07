#include "hough.h"
#include "sobel.h"
#include "improc.h"
//int** hough(unsigned char** sobel_output, int w, int h) {
//
//	int _accu_r = round(sqrt(2.0)*h);
//	int _accu_w = 180;
//
////	TODO
//	_accu_r = 512;
//
//	printf("Wymiar h: %d\n", _accu_r);
//
//	int** _accu = malloc(_accu_r * sizeof(int*));
//
//	if(_accu == NULL) {
//		printf("nieudana alokacja\n");
//		exit(-1);
//	}
//
//	int i;
//	for (i = 0; i < _accu_r; i++) {
//		_accu[i] = malloc(_accu_w * sizeof(int));
//
//		if(_accu[i] == NULL) {
//			printf("nieudana alokacja\n");
//			exit(-1);
//		}
//	}
//
//	int j;
//	for(i=0; i<_accu_r; i++) {
//		for(j=0; j<_accu_w; j++){
//			_accu[i][j] = 0;
//		}
//	}
//
//	printf("accu_w: %d, accu_h: %d\n", _accu_w, _accu_r);
//
//	int licznik = 0;
//
//	int x, y, t;
//	for (x = 0; x < w; x++) {
//		for (y = 0; y < h; y++) {
//			if ((int) sobel_output[x][y] > 0) {
//
//				for (t = 0; t < 180; t++) {
//
//					double c_x = x/2.;
//					double c_y = y/2.;
//					double r = ((double)x - c_x) * cos((double) t * DEG2RAD)
//							+ ((double)y - c_y) * sin((double) t * DEG2RAD);
//
//
////					_accu[(int)((round(r + hough_h) * 180.0))][t]++;
//					int r_int = (int)r + _accu_r/2;
//
////					printf("OK %d <-- %lf\n", r_int, r);
//
//					_accu[r_int][t]++;
//					if(_accu[r_int][t] < 0) {
////						printf("blad\n");
//						licznik++;
//					}
//				}
//			}
//		}
//	}
//
//		for(i=0; i<_accu_r; i++) {
//			for(j=0; j<_accu_w; j++){
////				printf("%3d ", _accu[i][j]);
//				if(_accu[i][j] > 140) {
//					printf("r: %3d, fi: %3d, wart: %d\n", i, j, _accu[i][j]);
//					drawLine("output_file_n1.bmp", 128, 90, 512, 512);
//				}
//			}
////			printf(":%d\n", i);
//		}
//
////		drawLine("output_file123456789.bmp", 100, 90, 512, 512);
//
//	return _accu;
//}




int** hough(unsigned char** sobel_output, int w, int h) {

//	TODO
	int _accu_rr = sqrt(2.0)*h/2;
	int _accu_r = _accu_rr * 2; // r -> -r
	int _accu_w = 180;

	printf("Wymiar h: %d\n", _accu_r);

	int** _accu = malloc(_accu_r * sizeof(int*));

	if(_accu == NULL) {
		printf("nieudana alokacja\n");
		exit(-1);
	}

	int i;
	for (i = 0; i < _accu_r; i++) {
		_accu[i] = malloc(_accu_w * sizeof(int));

		if(_accu[i] == NULL) {
			printf("nieudana alokacja\n");
			exit(-1);
		}
	}

	int j;
	for(i=0; i<_accu_r; i++) {
		for(j=0; j<_accu_w; j++){
			_accu[i][j] = 0;
		}
	}

	printf("accu_w: %d, accu_h: %d\n", _accu_w, _accu_r);

	int x, y, t;
	int x_pom, y_pom;
	for (x = 0; x < w; x++) {
		for (y = 0; y < h; y++) {
			if ((int) sobel_output[x][y] > 240) {

				x_pom = x-w/2;
				y_pom = y-h/2;

//				printf("x = %d, y = %d\n", x_pom, y_pom);

				for (t = 0; t < 180; t++) {


					int r = (double)x_pom * sin((double) (t) * DEG2RAD)
					+ (double)y_pom * cos((double) (t) * DEG2RAD);


					int r_abs = r+_accu_rr;

//					printf("r = %d\n", r);
					_accu[r_abs][t]++;
				}
			}
		}
	}


	int* pionowe = malloc(sizeof(int)*MAX_COLUMNS);
	int* poziome = malloc(sizeof(int)*MAX_ROWS);

	for(i=0; i<MAX_COLUMNS; i++)
		pionowe[i] = 0;

	for(i=0; i<MAX_ROWS; i++)
		poziome[i] = 0;

		for(i=0; i<_accu_r; i++) {
			for(j=0; j<_accu_w; j++){
//				printf("%3d ", _accu[i][j]);
				if(_accu[i][j] > 50) {
//					printf("r: %3d, fi: %3d, wart: %d\n", i, j, _accu[i][j]);
//					drawLine("bialy512.bmp", i, abs(90-j), 512, 512);



					if(abs(j) < 1) {
						pionowe[(i-_accu_rr)+256]++;
//						printf("Pionowa: %d\n", (i-_accu_rr)+256);
					}
					else if(abs(j-90) < 2) {
						poziome[pionowe[(i-_accu_rr)+256]]++;
//						printf("Pozioma: %d\n", (i-_accu_rr)+256);
						int l;
						for(l=0; l<MAX_COLUMNS; l++) {
							if(pionowe[l] > 0) {
								printf("srodek: (%d, %d)\n", l, (i-_accu_rr)+256);

								drawCircle("bialy512.bmp", l, (i-_accu_rr)+256, 30, 512, 512);
							}
						}

					}
				}
			}
//			printf(":%d\n", i);
		}

//		drawLine("output_file123456789.bmp", 100, 90, 512, 512);

	return _accu;
}

int*** hough_circles(int** sobel_output, int w, int h) {

	printf("Circles\n");

	int _accu_r = round(MIN(h, w)/2);
	int _accu_w = w;
	int _accu_h = h;

	printf("Wymiar r: %d\n", _accu_r);

	int*** _accu = malloc(_accu_w * sizeof(int**));

	if(_accu == NULL) {
		printf("nieudana alokacja\n");
		exit(-1);
	}

	int i, j, k;
	for (i = 0; i < _accu_w; i++) {
		_accu[i] = malloc(_accu_h * sizeof(int*));

		if(_accu[i] == NULL) {
			printf("nieudana alokacja\n");
			exit(-1);
		}

		for(j = 0; j < _accu_h; j++) {
			_accu[i][j] = malloc(_accu_r * sizeof(int));

			if(_accu[i][j] == NULL) {
				printf("nieudana alokacja\n");
				exit(-1);
			}
		}
	}

	for(i = 0; i < _accu_w; i++) {
		for(j = 0; j < _accu_h; j++) {
			for(k = 0; k < _accu_r; k++) {
				_accu[i][j][k] = 0;
			}
		}
	}

	int x, y, t, a, b, r;
	for(a = 0; a < _accu_w; a++) {
		for(b = 0; b < _accu_h; b++) {
			for(r = 0; r < _accu_r; r++) {

				for(t=0; t<180; t++) {

					x = a + (r+1)*cos((double) t * DEG2RAD);
					y = b + (r+1)*sin((double) t * DEG2RAD);

					if(x>=0 && y>=0 && x<w && y<h && (int)sobel_output[x][y] > 240) {
						_accu[a][b][r]++;
					}
		printf("%d\n", a);
				}
			}
		}
	}

	for(i=0; i<_accu_w; i++) {
		for(j=0; j<_accu_h; j++){
			for(k=0; k<_accu_r; k++) {
				if(_accu[i][j][k] > 100) {
					printf("x: %3d, y: %3d, rad: %d\n", i, j, _accu[i][j][k]);
				}
			}
		}
	}

	return _accu;
}

void clear_2(void** array, int w) {

	int i;
	for(i=0; i<w; i++) {
		free(array[i]);
	}

	free(array);
}

void clear_3(void*** array, int w, int h) {

	int i, j;
	for(i=0; i<w; i++) {
		for(j=0; j<h; j++) {
			free(array[i][j]);
		}
		free(array[i]);
	}

	free(array);
}
