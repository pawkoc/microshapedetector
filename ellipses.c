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

void detect_ellipses(unsigned char** sobel_output, int w, int h, double threshold, int min_dist, EllipseList* ellipses_result_list) {

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
//
	printf("przed przetw %d\n", pixels->size);
//
//	print_list(pixels);

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

							int round_b = round(b);

							if(round_b < BSIZE && round_b > 0) {

								k_node->b_ellipse = round_b;

								acc[round_b]++;
							} else {
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
					obwod = M_PI*(3/2 * (a+max_acc) - sqrt(a*max_acc))*16/17;

				if(max_wart >= obwod*4/5 && max_wart >= threshold && max_wart>0 && (int)max_acc>10) {
//					printf("ELLIPSE FOUND!! %d %d %d %d\n", 512-(int)center_x, (int)center_y, (int)a, (int)max_acc);
//					printf("(%d, %d), (%d, %d)\n\n", 512-INODEX, INODEY, 512-JNODEX, JNODEY);

					add_first_ellipse(ellipses_result_list, (int)center_y, (int)center_x, (int)max_acc, (int)a);

					drawEllipse("output_ellipse.bmp", (int)center_y, (int)center_x, (int)max_acc, (int)a, 512, 512);


					removeEllipseFromImage(i_node, j_node, max_acc);

					i_node->b_ellipse = 0.;
					j_node->b_ellipse = 0.;
				}

				for(i=0; i<BSIZE; i++) {
					acc[i] = 0.;
				}
			}

}
		}
	}

}

EllipseList* init_ellipse_list() {
	EllipseList* list = malloc(sizeof(EllipseList));

	list->head = NULL;
	list->size = 0;

	return list;
}

void add_first_ellipse(EllipseList* list, int x, int y, double a, double b) {

	EllipseNode* new_ellipse = malloc(sizeof(EllipseNode));
	new_ellipse->center_x = x;
	new_ellipse->center_y = y;
	new_ellipse->a_axis = a;
	new_ellipse->b_axis = b;


	new_ellipse->next = list->head;
	list->head = new_ellipse;

	list->size++;
}

void print_ellipse_list(EllipseList* list) {

	EllipseNode* head = list->head;

	while(head) {
		printf("CENTER: (%d, %d); a_axis: %lf, b_axis: %lf\n", head->center_x, head->center_y,
					head->a_axis, head->b_axis);
		head = head->next;
	}

	printf("\n");
}
