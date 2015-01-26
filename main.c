#include "sobel.h"
#include "improc.h"
#include "hough.h"
#include "circles.h"
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

    char* name = "kostka.bmp";

    bmpData = packed_function(name, &width, &height);

    header = getHeader(name, &header_size);

    if(header) saveAsBitmap(header_size, header, bmpData, width, height);

//    unsigned char** wynik = sobel(bmpData);
//
//
////    unsigned char** kolka = detect_circles(wynik);
//
//    unsigned char** wynik1 = sobel(wynik);
//
////    int** wynik_hough = hough(wynik, 512, 512);
////    int*** wynik_hough_circle = hough_circles(wynik, 512, 512);
//
////    drawCircle("bialy2.bmp", 128, 128, 30, 512, 512);
//
//    unsigned char** out;
////    out = wynik;
////    out = bmpData;
////    out = (unsigned char**)wynik_hough;
//
////    out = wynik;

//    if(header) saveAsBitmap(header_size, header, out2, width, height);

//    unsigned char** out2 = sobel(bmpData);
//    out = thin2(out2, 512, 512);





//    int** hough1 = hough(out2, 512, 512, 450.);

//    detect_ellipses(out2, 512, 512, 50., 0.);

//    unsigned char** out3 = thin2(bmpData, 512, 512);


//    if(header) saveAsBitmap(header_size, header, out2, width, height);
//
//    printf("Zapisalem\n");







////    out = sobel(out);
//
//    printf("debug\n");
//
//#ifdef DEBUG
//    printf("Header size: %d\n", header_size);
//    printf("Width: %d\n", width);
//    printf("Height: %d\n", height);
//#endif
//
////    if(header) saveAsBitmap(header_size, header, wynik, width, height, "sobel_output_file_n2.bmp");
//    if(header) saveAsBitmap(header_size, header, out, width, height);
//#ifdef DEBUG
//    else printf("No header\n");
//#endif

//    printf("detekcja elips\n");
//



//detect_ellipses(out, 512, 512, 35., 0.);



    Graph* g = init_graph(4);

    add_edge(g, 0, 1, 1);
    add_edge(g, 0, 2, 2);
    add_edge(g, 1, 3, 10);
    add_edge(g, 2, 3, 2);
    add_edge(g, 0, 3, 2);

    print_graph(g);

    for(i=0; i<g->vertices_count; i++) {
    	for(j=0; j<=i; j++)
    		printf("%d ", g->matrix[i][j]);
    	printf("\n");
    }


    Floyd_Warshall(g);
//    clear_2(wynik, 512);
//    clear_2(wynik_hough, 512);
//    clear_3(wynik_hough_circle, 512, 512);



	return 0;
}
