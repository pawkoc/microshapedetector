#include "sobel.h"
#include "improc.h"
#include "hough.h"
#include "impreproc.h"
#include "pixel.h"
#include "ellipses.h"
#include "graph.h"

int main(int argc, char **argv) {

	unsigned char **k = matrix_init(0);

	int i, j, ka=1;
	for(i=0; i<MAX_ROWS; i++) {
		for(j=0; j<MAX_COLUMNS; j++) {
			k[i][j] = ka++;
		}
	}

    unsigned char **bmpData;
    unsigned char *header;
    int header_size, width, height;

    char* name = "rozne.bmp";

    bmpData = packed_function(name, &width, &height);

    header = getHeader(name, &header_size);

    if(header) saveAsBitmap(header_size, header, bmpData, width, height);

    unsigned char** out;

    unsigned char** out2 = sobel(bmpData);
//    out = thin2(out2, 512, 512);

    if(header) saveAsBitmap(header_size, header, out2, width, height);

    int** hough1 = hough(out2, 512, 512, 90.);

    EllipseList* ellipses = init_ellipse_list();

    detect_ellipses(out2, 512, 512, 70., 10., ellipses);

    print_ellipse_list(ellipses);

    printf("Terminated\n");

	return 0;
}
