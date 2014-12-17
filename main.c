#include "sobel.h"
#include "improc.h"
#include "hough.h"
#include "circles.h"

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

    char* name = "kolko.bmp";

    bmpData = packed_function(name, &width, &height);

    header = getHeader(name, &header_size);

//    if(header) saveAsBitmap(header_size, header, bmpData, width, height, "output_file.bmp");

    unsigned char** wynik = sobel(bmpData);


    unsigned char** kolka = detect_circles(wynik);

//    unsigned char** wynik1 = sobel(wynik);

    int** wynik_hough = hough(wynik, 512, 512);
//    int*** wynik_hough_circle = hough_circles(wynik, 512, 512);

//    drawCircle("bialy2.bmp", 128, 128, 30, 512, 512);

    unsigned char** out;
//    out = wynik;
//    out = bmpData;
    out = (unsigned char**)wynik_hough;

#ifdef DEBUG
    printf("Header size: %d\n", header_size);
    printf("Width: %d\n", width);
    printf("Height: %d\n", height);
#endif

//    if(header) saveAsBitmap(header_size, header, wynik, width, height, "sobel_output_file_n2.bmp");
//    if(header) saveAsBitmap(header_size, header, out, width, height, "output_file.bmp");
#ifdef DEBUG
    else printf("No header\n");
#endif




    clear_2(wynik, 512);
//    clear_2(wynik_hough, 512);
//    clear_3(wynik_hough_circle, 512, 512);

	return 0;
}
