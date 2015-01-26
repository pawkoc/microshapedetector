#include "ellipses.h"
#include "improc.h"

#include "hough.h"

#define INODEX i_node->pixel->x
#define INODEY i_node->pixel->y
#define JNODEX j_node->pixel->x
#define JNODEY j_node->pixel->y
#define KNODEX k_node->pixel->x
#define KNODEY k_node->pixel->y

#define BSIZE 512

double get_dist(double x1, double y1, double x2, double y2) {

//	printf("dist: (%lf, %lf), (%lf, %lf) = %lf\n", x1, y1, x2, y2, sqrt(((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1))));

	return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

List* get_pixels(unsigned char** sobel_output, int w, int h) {

	List* list = init_list();

	int i, j;
	for(i=0; i<w; i++) {
		for(j=0; j<h; j++) {
			if(sobel_output[i][j] > 250) {
				if(!(i<10 || j<10 || i>500 || j>500))
					add_sort(list, i, j);
			}
		}
	}

	return list;
}

void detect_with_start_end(unsigned char** out, double start_x, double start_y, double end_x, double end_y) {

	List* pixels = init_list();

	int i, j;
	for(i=0; i<512; i++) {
		for(j=0; j<512; j++) {

			if((int)out[i][j] > 250) {

				printf("%d, %d\n", i, j);

				if(!(start_x == i) && !(start_y == j)) {
					if(!(end_x == i) && !(end_y == j)) {
						if(!(i==0 || j==0))
							add_first(pixels, i, j);
					}
				}
			}
		}
	}

	print_list(pixels);

	if(pixels->size < 3) {

		printf("To few pixels\n");
		return;
	}

//	double acc[BSIZE];
//
//	for(i=0; i<BSIZE; i++) {
//		acc[i] = 0.;
//	}


	double center_x = (start_x+end_x)/2;
	double center_y = (start_y+end_y)/2;

	double a = get_dist(start_x, start_y, end_x, end_y)/2;
	double alpha = atan((end_y-start_y)/(end_x-start_x));


	printf("center: %lf, %lf\n", center_x, center_y);
	printf("a = %lf\n", a);
	printf("alpha = %lf\n", alpha*DEG2RAD);

//	1

	double d = get_dist(center_x, center_y, 3, 3);
	double f = get_dist(end_x, end_y, 3, 3);

	double cosine = (a*a + d*d - f*f)/(2*a*d);
	double cos_kw = cosine*cosine;

	double sin_kw = 1 - cos_kw;

	double tmp = (a*a * d*d * sin_kw)/(a*a - d*d*cos_kw);
	double b = sqrt(tmp);

	printf("b = %lf\n", b);

//	2

	d = get_dist(center_x, center_y, 3, 4);
	f = get_dist(end_x, end_y, 3, 4);

	cosine = (a*a + d*d - f*f)/(2*a*d);
	cos_kw = cosine*cosine;

	sin_kw = 1 - cos_kw;

	tmp = (a*a * d*d * sin_kw)/(a*a - d*d*cos_kw);
	b = sqrt(tmp);

	printf("b = %lf\n", b);

//	3

	d = get_dist(center_x, center_y, 3, 5);
	f = get_dist(end_x, end_y, 3, 5);

	cosine = (a*a + d*d - f*f)/(2*a*d);
	cos_kw = cosine*cosine;

	sin_kw = 1 - cos_kw;

	tmp = (a*a * d*d * sin_kw)/(a*a - d*d*cos_kw);
	b = sqrt(tmp);

	printf("b = %lf\n", b);

//	4

	d = get_dist(center_x, center_y, 1, 5);
	f = get_dist(end_x, end_y, 1, 5);

	cosine = (a*a + d*d - f*f)/(2*a*d);
	cos_kw = cosine*cosine;

	sin_kw = 1 - cos_kw;

	tmp = (a*a * d*d * sin_kw)/(a*a - d*d*cos_kw);
	b = sqrt(tmp);

	printf("b = %lf\n", b);

//	5

	d = get_dist(center_x, center_y, 1, 4);
	f = get_dist(end_x, end_y, 1, 4);

	cosine = (a*a + d*d - f*f)/(2*a*d);
	cos_kw = cosine*cosine;

	sin_kw = 1 - cos_kw;

	tmp = (a*a * d*d * sin_kw)/(a*a - d*d*cos_kw);
	b = sqrt(tmp);

	printf("b = %lf\n", b);

//	6

	d = get_dist(center_x, center_y, 1, 3);
	f = get_dist(end_x, end_y, 1, 3);

	cosine = (a*a + d*d - f*f)/(2*a*d);
	cos_kw = cosine*cosine;

	sin_kw = 1 - cos_kw;

	tmp = (a*a * d*d * sin_kw)/(a*a - d*d*cos_kw);
	b = sqrt(tmp);

	printf("b = %lf\n", b);
}

void detect_ellipses(unsigned char** sobel_output, int w, int h, double threshold, int min_dist) {

	List* pixels = get_pixels(sobel_output, w, h);

	if(pixels->size < 3) {

		printf("To few pixels\n");
		return;
	}

	//minor axis
	double acc[BSIZE];

	int i;
	for(i=0; i<BSIZE; i++) {
		acc[i] = 0.;
	}

	Node *i_node, *j_node, *k_node;

	double center_x, center_y, a, b, d, f, alpha, cosine, cosine_sq, sin_sq, max_acc;

	printf("przed przetw %d\n", pixels->size);

	print_list(pixels);

	for(i_node = pixels->head; i_node->next; i_node = i_node->next) {

		for(j_node = i_node->next; j_node; j_node = j_node->next) {
if(INODEX == JNODEX) {
			if(get_dist(INODEX, INODEY, JNODEX, JNODEY) > min_dist) {

//				printf("Rozwazane punkty: (%d, %d), (%d, %d)\n",
//						INODEX, INODEY, JNODEX, JNODEY);

				center_x = (INODEX + JNODEX)/2;
				center_y = (INODEY + JNODEY)/2;

				a = get_dist(INODEX, INODEY, JNODEX, JNODEY)/2;

				if(JNODEX != INODEX)
					alpha = atan((JNODEY - INODEY)/(JNODEX - INODEX));

//				printf("a = %lf\n", a);

				for(k_node = i_node->next; k_node != j_node; k_node = k_node->next) {

					if((KNODEY != INODEY) && (KNODEY != JNODEY)) {

						if((d = get_dist(KNODEX, KNODEY, center_x, center_y)) > min_dist) {


//							printf("Rozwazane punkty: (%d, %d), (%d, %d), (%d, %d)\n",
//									INODEX, INODEY, JNODEX, JNODEY, KNODEX, KNODEY);

							f = get_dist(KNODEX, KNODEY, JNODEX, JNODEY);

							cosine = (a*a + d*d - f*f)/(2*a*d);

							cosine_sq = cosine*cosine;
							sin_sq = 1 - cosine_sq;

							double tmp2 = (a*a - d*d *cosine_sq);
							double tmp1 = (a*a * d*d * sin_sq);

//							printf("tmp1 = %lf, tmp2=%lf\t", tmp1, tmp2);

							if(tmp1 < 0)
								tmp1 = -tmp1;

							if(tmp2 < 0)
								tmp2 = -tmp2;

							b = sqrt(tmp1/tmp2);

	//						if((tmp2 > 0.000001 || tmp2 < -0.000001) && tmp1 > 0) {
	//							if(tmp2 < 0)
	//								tmp2 = -tmp2;
	//
	//							b = sqrt(tmp1/tmp2);
	//						}

//							printf("b = %lf\n", b);

							int round_b = round(b);

							if(round_b < BSIZE && round_b > 0)
								acc[round_b]++;
							else {
//								printf("BLAD!!\n");
//								printf("Rozwazane punkty: (%d, %d), (%d, %d), (%d, %d)\n",
//										INODEX, INODEY, JNODEX, JNODEY, KNODEX, KNODEY);
//								printf("tmp1 = %lf, tmp2=%lf\n", tmp1, tmp2);
//								printf("a = %lf\n", a);
//								printf("b = %lf\n\n", b);
							}

						}
					}
				}

				max_acc = 0.;
				double max_wart = 0.;

				for(i=0; i<BSIZE; i++) {
					if(acc[i] > max_wart) {
//						printf("%d ", (int)acc[i]);
						max_wart = acc[i];
						max_acc = i;
					}
				}

				double obwod;

				if(abs(a-max_acc) < 4)
					obwod = M_PI*a*2/3;
				else
					obwod = M_PI*(3/2 * (a+max_acc) - sqrt(a*max_acc))*6/7;

				if(max_wart >= obwod*4/5 && max_wart >= threshold && max_wart>0 && (int)max_acc>10) {
					printf("ELLIPSE FOUND!! %d %d %d %d\n", 512-(int)center_x, (int)center_y, (int)a, (int)max_acc);
					printf("(%d, %d), (%d, %d)\n\n", 512-INODEX, INODEY, 512-JNODEX, JNODEY);

					drawEllipse("output_ellipse.bmp", (int)center_y, (int)center_x, (int)max_acc, (int)a, 512, 512);
				}

				for(i=0; i<BSIZE; i++) {
					acc[i] = 0.;
				}
			}

}
		}
	}

//	drawEllipse("output_ellipse.bmp", (512-470), 30, 2, 2, 512, 512);
//	drawEllipse("output_ellipse.bmp", 52, 44, 10, 10, 512, 512);

//	printf("narysowal\n");
}
