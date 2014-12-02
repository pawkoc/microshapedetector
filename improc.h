#ifndef _WINDEF_
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef int LONG;
#endif

#include <stdlib.h>
#include <stdio.h>

#ifndef IMPROC_H_
#define IMPROC_H_

typedef struct tagBITMAPFILEHEADER {
	WORD bfType;  //specifies the file type
	DWORD bfSize;  //specifies the size in bytes of the bitmap file
	WORD bfReserved1;  //reserved; must be 0
	WORD bfReserved2;  //reserved; must be 0
	DWORD bfOffBits; //species the offset in bytes from the bitmapfileheader to the bitmap bits
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	DWORD biSize;  //specifies the number of bytes required by the struct
	LONG biWidth;  //specifies width in pixels
	LONG biHeight;  //species height in pixels
	WORD biPlanes; //specifies the number of color planes, must be 1
	WORD biBitCount; //specifies the number of bit per pixel
	DWORD biCompression; //spcifies the type of compression
	DWORD biSizeImage;  //size of image in bytes
	LONG biXPelsPerMeter;  //number of pixels per meter in x axis
	LONG biYPelsPerMeter;  //number of pixels per meter in y axis
	DWORD biClrUsed;  //number of colors used by th ebitmap
	DWORD biClrImportant;  //number of colors that are important
} BITMAPINFOHEADER;

unsigned char *LoadBitmapFile(char *filename,
		BITMAPINFOHEADER *bitmapInfoHeader);
void buf7_to_fheader(BITMAPFILEHEADER *bitmapFileHeader, WORD *buf7);
unsigned char **convertBitmapData(unsigned char *bitmapData, LONG width,
		LONG height);
unsigned char **packed_function(char *filename, int *width, int *height);
unsigned char *getHeader(char *filename, int *header_size);
int saveAsBitmap(int header_size, unsigned char *header,
		unsigned char **bmpData, int width, int height);
#endif
