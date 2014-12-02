#include "sobel.h"
#include "improc.h"
#include "hough.h"

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

    char* name = "contrast.bmp";

    bmpData = packed_function(name, &width, &height);

    header = getHeader(name, &header_size);

    unsigned char** wynik = sobel(bmpData);
//    unsigned char** wynik1 = sobel(wynik);

    int** wynik_hough = hough(wynik, 512, 512);

    unsigned char** out;
    out = wynik;
//    out = bmpData;

#ifdef DEBUG
    printf("Header size: %d\n", header_size);
    printf("Width: %d\n", width);
    printf("Height: %d\n", height);
#endif

    if(header) saveAsBitmap(header_size, header, out, width, height);
#ifdef DEBUG
    else printf("No header\n");
#endif


	return 0;

}
