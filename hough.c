#include "hough.h"
#include "sobel.h"
#include "improc.h"

int local_maxima(int** tab, int w, int h, int val, int a, int b) {

	int i, j, max=0, tmp=5;
	for(i=-tmp; i<=tmp; i++) {
		for(j=-tmp-10; j<=tmp+10; j++) {

			if((a+i) >=0 && (a+i) < w && (b+j) >= 0 && (b+j) < h) {

				if(tab[a+i][b+j] > max) {
					max = tab[a+i][b+j];
				}
			}
		}
	}

	if(max > val) {
		return 0;
	}

	return 1;
}

int** hough(unsigned char** sobel_output, int w, int h, double threshold) {

	int _accu_rr = sqrt(2.0)*h;
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
	for (x = 5; x < w-5; x++) {
		for (y = 5; y < h-5; y++) {
			if ((int) sobel_output[x][y] > 240) {

				for (t = 0; t < 180; t++) {

					int r = (double)x * sin((double) (t) * DEG2RAD)
					+ (double)y * cos((double) (t) * DEG2RAD);

					int r_abs = r+_accu_rr;

					if(abs(t-90) < 2)
						_accu[r_abs][90]++;

					else if(t < 2)
						_accu[r_abs][0]++;

					else if(abs(t-180) < 2)
						_accu[r_abs][0]++;

					else
						_accu[r_abs][t]++;
				}
			}
		}
	}


	Segment* segment = malloc(sizeof(Segment));

	SegmentList* segment_list = init_segment_list();

	for(i=0; i<_accu_r; i++) {
		for(j=0; j<_accu_w; j++){
			if(_accu[i][j] > threshold) {

				if(local_maxima(_accu, _accu_r, _accu_w, _accu[i][j], i, j)) {

					printf("kat = %d, dist. = %d, wart. = %d\n", j, i-_accu_rr, _accu[i][j]);

					double c = sqrt(2.0)*256;

					double k = i*cos(j*DEG2RAD);
					double m = i*sin(j*DEG2RAD);

					double b_kw = (256-k)*(256-k) + (256-m)*(256-m);

					double x = (c*c - i*i - b_kw)/(2*i);

					drawLine("output_ellipse.bmp", x+_accu_rr, j+90, 512, 512, sobel_output, segment);

					if(segment->start != NULL) {
						printf("SEGMENT: (%d, %d) -> (%d, %d)\n", segment->start->x, segment->start->y,
								segment->end->x, segment->end->y);

						add_first_segment(segment_list, segment);

						segment = malloc(sizeof(Segment));
					}
				}
			}
		}
	}

	print_segment_list(segment_list);

	get_all_intersection_pixels(segment_list);

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
