#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "improc.h"

unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
    WORD *buf7;
    FILE *filePtr; //our file pointer
    BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
    unsigned char *bitmapImage;  //store image data

    //open filename in read binary mode
    filePtr = fopen(filename,"rb");
    if (filePtr == NULL)
    {
#ifdef DEBUG
        printf("Fail: open\n");
#endif
        return NULL;
    }

    buf7 = (WORD*)malloc(7*sizeof(WORD));

    //read the bitmap file header
    fread(buf7, sizeof(BITMAPFILEHEADER),1,filePtr);

    buf7_to_fheader(&bitmapFileHeader,buf7);

    //verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.bfType !=0x4D42)
    {
        fclose(filePtr);
#ifdef DEBUG
        printf("Fail: type\n");
#endif
        return NULL;
    }
#ifdef DEBUG
    else
    {
        printf("Size: %d\n", bitmapFileHeader.bfSize);
        printf("Reserved1: %hd\n", bitmapFileHeader.bfReserved1);
        printf("Reserved1: %hd\n", bitmapFileHeader.bfReserved2);
        printf("Offset: %d\n", bitmapFileHeader.bfOffBits);
        printf("\n");
    }
#endif

    fseek(filePtr, 14, SEEK_SET);

    //read the bitmap info header
    fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,filePtr); // small edit. forgot to add the closing bracket at sizeof

    //move file point to the begging of bitmap data
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

    //allocate enough memory for the bitmap image data
    bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

    //verify memory allocation
    if (!bitmapImage)
    {
        free(bitmapImage);
        fclose(filePtr);
#ifdef DEBUG
        printf("Fail: malloc\n");
#endif
        return NULL;
    }

    //read in the bitmap image data
    fread(bitmapImage,bitmapInfoHeader->biSizeImage,1,filePtr);

    //make sure bitmap image data was read
    if (bitmapImage == NULL)
    {
        fclose(filePtr);
#ifdef DEBUG
        printf("Fail: read\n");
#endif
        return NULL;
    }

    //close file and return bitmap image data
    fclose(filePtr);
    return bitmapImage;
}

void buf7_to_fheader(BITMAPFILEHEADER *bitmapFileHeader,WORD *buf7)
{
    bitmapFileHeader->bfType = buf7[0];
    bitmapFileHeader->bfSize = buf7[2]*0x10000+buf7[1];
    bitmapFileHeader->bfReserved1 = buf7[3];
    bitmapFileHeader->bfReserved2 = buf7[4];
    bitmapFileHeader->bfOffBits = buf7[6]*0x10000+buf7[5];
    free(buf7);
}


unsigned char **convertBitmapData(unsigned char *bitmapData,LONG width,LONG height)
{
    unsigned char **newBitmapData;
    int i, j, p, tmp;

    newBitmapData = malloc(height*sizeof(unsigned char*));
	if(newBitmapData)
	{
    p = 0;

		for (i = 0; i < height; i++)
		{
			newBitmapData[i] = malloc(width*sizeof(unsigned char));
			for(j = 0; j < width; j++)
			{
			    //printf("%2x:", bitmapData[p]); //<-
				tmp = bitmapData[p++];
				//printf("%2x:", bitmapData[p]); //<-
				tmp += bitmapData[p++];
				//printf("%2x ", bitmapData[p]); //<-
                //printf("%d\n", p); //<-
				tmp += bitmapData[p++];
				newBitmapData[i][j] = tmp/3;
			}
			//printf("---:---:--- ---\n"); //<-
			//p+=2;
		}

    free(bitmapData);
	}

    return newBitmapData;
}

unsigned char **packed_function(char *filename, int *width, int *height)
{
    BITMAPINFOHEADER bitmapInfoHeader;
    unsigned char *bitmapData;

    bitmapData = LoadBitmapFile(filename,&bitmapInfoHeader);

#ifdef DEBUG
    if(bitmapData)
    {
        printf("Image: %d*%d\n", bitmapInfoHeader.biWidth, bitmapInfoHeader.biHeight);
        printf("Size: %d\n", bitmapInfoHeader.biSizeImage);
        printf("R: %hhud ", bitmapData[2]);
        printf("G: %hhud ", bitmapData[1]);
        printf("B: %hhud\n", bitmapData[0]);
    }
    else
    {
        printf("Fail\n");
    }
#endif

    *width = bitmapInfoHeader.biWidth;
    *height = bitmapInfoHeader.biHeight;

    return convertBitmapData(bitmapData,bitmapInfoHeader.biWidth,bitmapInfoHeader.biHeight);
}

unsigned char *getHeader(char *filename, int *header_size)
{
    unsigned char *header;
    WORD *buf7;
    FILE *filePtr; //our file pointer
    BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header

    //open filename in read binary mode
    filePtr = fopen(filename,"rb");
    if (filePtr == NULL)
    {
#ifdef DEBUG
        printf("Fail: open\n");
#endif
        return NULL;
    }

    buf7 = (WORD*)malloc(7*sizeof(WORD));

    //read the bitmap file header
    fread(buf7, sizeof(BITMAPFILEHEADER),1,filePtr);

    buf7_to_fheader(&bitmapFileHeader,buf7);

    //verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.bfType !=0x4D42)
    {
        fclose(filePtr);
#ifdef DEBUG
        printf("Fail: type\n");
#endif
        return NULL;
    }
    else
    {
        header = malloc(bitmapFileHeader.bfOffBits*sizeof(unsigned char));
        fseek(filePtr, 0, SEEK_SET);
        if(fread(header,1,bitmapFileHeader.bfOffBits,filePtr)!=bitmapFileHeader.bfOffBits)
        {
            fclose(filePtr);
#ifdef DEBUG
            printf("Fail: getheader\n");
#endif
            return NULL;
        }
        fclose(filePtr);
#ifdef DEBUG
//        printf("First byte: %08x\n", header[0]);
//        printf("Last byte: %08x\n", header[bitmapFileHeader.bfOffBits-1]);
#endif
    }

    *header_size = bitmapFileHeader.bfOffBits;

    return header;
}

int saveAsBitmap(int header_size, unsigned char *header, unsigned char **bmpData, int width, int height)
{
    FILE *filePtr;
    unsigned char *tmp;
    int i, j, p, tmp_s;

    filePtr = fopen("output_file_gimp.bmp", "wb");
    if(fwrite(header, 1, header_size, filePtr)==0) return 1;

    tmp_s = width*3;//+2;
    tmp = malloc(tmp_s);

	for (i = 0; i < height; i++)
	{
        p = 0;
		for(j = 0; j < width; j++)
		{
			tmp[p++] = bmpData[i][j];
			tmp[p++] = bmpData[i][j];
			tmp[p++] = bmpData[i][j];
		}
		//tmp[p++] = 0;
		//tmp[p++] = 0;
		fwrite(tmp, 1, tmp_s, filePtr);
	}
    fclose(filePtr);

    return 0;
}

void point(unsigned char *buffer, int h, int bs) {
    h = h*3;
    if(h >= 0 && h < bs-2) {
        buffer[h++] = 0;
        buffer[h++] = 0;
        buffer[h] = 255;
    }
}

int drawLine(char *filename, int dist, int angle, int width, int hight) {

    FILE *filePtr;
    unsigned char *buffer;
    unsigned char *header;
    int header_size, bs, w, h, w2, h2;
    long lhs;
    double s, c;

    im_rad = IM_PI/180.0;

    header = getHeader(filename, &header_size);
    lhs = header_size;

    if(header) {
        filePtr = fopen(filename, "rb+");
        if(!filePtr) return 1;

        bs = width*hight*3;
        buffer = malloc(bs);
        fseek(filePtr, lhs, SEEK_SET);
        fread(buffer,bs,1,filePtr);

        w2 = width/2;
        h2 = hight/2;
        if(angle == 90) {
            dist += w2;
            for(w = 0; w < hight; w++) {
                h = dist+w*width;
                point(buffer, h, bs);
            }
        }
        else if(angle == 0 || angle == 180) {
            dist += h2;
            for(w = 0; w < width; w++) {
                h = dist*width+w;
                point(buffer, h, bs);
            }
        }
        else if(angle > 45 && angle < 135) {
            c = im_rad * angle;
            s = cos(c) / sin(c);
            c = dist / sin(c) - w2;
            for(w = 0; w < h2; w++) {
                h = s*w-c;
                if(h >= 0 && h < width) {
                    h += (w+h2)*width;
                    point(buffer, h, bs);
                }
                h = -s*w-c;
                if(h >= 0 && h < width) {
                    h -= (w-h2)*width;
                    point(buffer, h, bs);
                }
            }
        }
        else {
            c = im_rad * angle;
            s = sin(c) / cos(c);
            c = dist / cos(c) + h2;
            for(w = 0; w < w2; w++) {
                h = s*w+c;
                if(h >= 0 && h < hight) {
                    h = h*width+w+w2;
                    point(buffer, h, bs);
                }
                h = -s*w+c;
                if(h >=0 && h < hight) {
                    h = h*width-w+w2;
                    point(buffer, h, bs);
                }
            }
        }

        fseek(filePtr, lhs, SEEK_SET);
        fwrite(buffer,bs,1,filePtr);

        free(buffer);
        fclose(filePtr);
    }
    else return 1;

    free(header);

    return 0;
}

void ellipse_points(unsigned char *buffer, int x0, int y0, int x, int y, int width, int bs) {
    int h, l, r;
    l = x0 - x >= 0;
    r = x0 + x < width;
	h = (y0 + y) * width + x0;
	if(r) point(buffer, h+x, bs);
	if(l) point(buffer, h-x, bs);
	h = (y0 - y) * width + x0;
	if(r) point(buffer, h+x, bs);
	if(l) point(buffer, h-x, bs);
}

void drawPart(unsigned char *buffer, int x0, int y0, int a, int b, int hw, int width, int bs) {

    int a2, b2, d, deltaA, deltaB, limit, x, y;
	a2 = a*a;
	b2 = b*b;

	d = 4*b2 - 4*b*a2 + a2;
	deltaA = 4*3*b2;
	deltaB = 4*(3*b2 - 2*b*a2 + 2*a2);

	limit = (a2*a2)/(a2+b2);

	x = 0;
	y = b;
	while (1) {
		if (hw)
			ellipse_points(buffer, x0, y0, x, y, width, bs);
		else
			ellipse_points(buffer, x0, y0, y, x, width, bs);

		if (x * x >= limit)
			break;

		if (d > 0) {
			d      += deltaB;
			deltaA += 4*2*b2;
			deltaB += 4*(2*b2 + 2*a2);

			x += 1;
			y -= 1;
		}
		else {
			d      += deltaA;
			deltaA += 4*2*b2;
			deltaB += 4*2*b2;

			x += 1;
		}
	}
}

int drawEllipse(char *filename, int x0, int y0, int a, int b, int width, int hight) {

    FILE *filePtr;
    unsigned char *buffer;
    unsigned char *header;
    int header_size, bs;
    long lhs;

    header = getHeader(filename, &header_size);
    lhs = header_size;

    if(header) {
        filePtr = fopen(filename, "rb+");
        if(!filePtr) return 1;

        bs = width*hight*3;
        buffer = malloc(bs);
        fseek(filePtr, lhs, SEEK_SET);
        fread(buffer,bs,1,filePtr);

        drawPart(buffer, x0, y0, b, a, 1, width, bs);
        drawPart(buffer, x0, y0, a, b, 0, width, bs);

        fseek(filePtr, lhs, SEEK_SET);
        fwrite(buffer,bs,1,filePtr);

        free(buffer);
        fclose(filePtr);
    }
    else return 1;

    free(header);

    return 0;
}

int drawCircle(char *filename, int x0, int y0, int r, int width, int hight) {

    FILE *filePtr;
    unsigned char *buffer;
    unsigned char *header;
    int header_size, bs, x, y, d, deltaA, deltaB;
    long lhs;

    header = getHeader(filename, &header_size);
    lhs = header_size;

    if(header) {
        filePtr = fopen(filename, "rb+");
        if(!filePtr) return 1;

        bs = width*hight*3;
        buffer = malloc(bs);
        fseek(filePtr, lhs, SEEK_SET);
        fread(buffer,bs,1,filePtr);


        d = 5 - 4*r;

        x = 0;
        y = r;

        deltaA = (-2*r + 5)*4;
        deltaB = 3*4;

        while (x <= y) {
            ellipse_points(buffer, x0, y0, x, y, width, bs);
            ellipse_points(buffer, x0, y0, y, x, width, bs);

            if (d > 0) {
                d += deltaA;

                y -= 1;
                x += 1;

                deltaA += 4*4;
                deltaB += 2*2;
            }
            else {
                d += deltaB;

                x += 1;

                deltaA += 2*4;
                deltaB += 2*4;
            }
        }


        fseek(filePtr, lhs, SEEK_SET);
        fwrite(buffer,bs,1,filePtr);

        free(buffer);
        fclose(filePtr);
    }
    else return 1;

    free(header);

    return 0;
}
