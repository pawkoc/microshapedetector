#include "ellipses.h"
#include "improc.h"

#define INODEX i_node->pixel->x
#define INODEY i_node->pixel->y
#define JNODEX j_node->pixel->x
#define JNODEY j_node->pixel->y
#define KNODEX k_node->pixel->x
#define KNODEY k_node->pixel->y

#define BSIZE 512

double get_dist(int x1, int y1, int x2, int y2) {

	return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

List* get_pixels(unsigned char** sobel_output, int w, int h) {

	List* list = init_list();

	int i, j;
	for(i=1; i<w; i++) {
		for(j=1; j<h; j++) {
			if(sobel_output[i][j] > 240) {
				add_first(list, i, j);
			}
		}
	}

	return list;
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

	for(i_node = pixels->head; i_node->next->next; i_node = i_node->next) {

		for(j_node = i_node->next; j_node->next; j_node = j_node->next) {

			if(abs(INODEX - JNODEX) > min_dist
					&& abs(INODEY - JNODEY) > min_dist) {

				center_x = (int)(INODEX + JNODEX)/2;
				center_y = (int)(INODEY + JNODEY)/2;

				a = get_dist(INODEX, INODEY, JNODEX, JNODEY)/2;
				alpha = atan((JNODEY - INODEY)/(JNODEX - INODEX));

				for(k_node = j_node->next; k_node; k_node = k_node->next) {

					if((d = get_dist(KNODEX, KNODEY, center_x, center_y)) > min_dist) {

						f = get_dist(KNODEX, KNODEY, JNODEX, JNODEY);

						cosine = (a*a + d*d - f*f)/(2*a*d);

						cosine_sq = cosine*cosine;
						sin_sq = 1 - cosine_sq;

						double tmp2 = (a*a - d*d *cosine_sq);
						double tmp1 = (a*a * d*d * sin_sq);

						if((tmp2 > 0.000001 || tmp2 < -0.000001) && tmp1 > 0) {
							if(tmp2 < 0)
								tmp2 = -tmp2;

							b = sqrt(tmp1/tmp2);
						}

						if((int)b < BSIZE)
							acc[(int)b]++;
					}
				}

				max_acc = 0.;

				for(i=0; i<BSIZE; i++) {
					if(acc[i] > max_acc) {
//						printf("%d ", (int)acc[i]);
						max_acc = acc[i];
					}
				}

				if(max_acc > threshold) {
					printf("ELLIPSE FOUND!! %d %d %d\n", (int)center_x, (int)center_y, (int)alpha);
					drawEllipse("output_ellipse.bmp", (int)center_x, (int)center_y, (int)max_acc, (int)a, 512, 512);
				}

				for(i=0; i<BSIZE; i++) {
					acc[i] = 0.;
				}
			}
		}
	}
}
