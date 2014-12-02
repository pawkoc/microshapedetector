#include <stdio.h>

#include "improc.h"

#define DEBUG

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
				tmp = bitmapData[p++];
				tmp += bitmapData[p++];
				tmp += bitmapData[p++];
				newBitmapData[i][j] = tmp/3;
			}
//			p+=1;
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
        printf("R: %dhud ", bitmapData[2]);
        printf("G: %dhud ", bitmapData[1]);
        printf("B: %dhud\n", bitmapData[0]);
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
//    unsigned char *bitmapImage;  //store image data

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
        printf("First byte: %08x\n", header[0]);
        printf("Last byte: %08x\n", header[bitmapFileHeader.bfOffBits-1]);
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

    filePtr = fopen("output_file12345678.bmp", "wb");
    if(fwrite(header, 1, header_size, filePtr)==0) return 0;

    tmp_s = width*3;
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
//		tmp[p++] = 0;
//		tmp[p++] = 0;
		fwrite(tmp, 1, tmp_s, filePtr);
	}
    fclose(filePtr);

    return 1;
}

//int main(int argc, char **argv) {
//
//    unsigned char **bmpData;
//    unsigned char *header;
//    int header_size, width, height;
//
//    bmpData = packed_function("mypic.bmp", &width, &height);
//
//    header = getHeader("mypic.bmp", &header_size);
//
//#ifdef DEBUG
//    printf("Header size: %d\n", header_size);
//    printf("Width: %d\n", width);
//    printf("Height: %d\n", height);
//#endif
//
//    if(header) saveAsBitmap(header_size, header, bmpData, width, height);
//#ifdef DEBUG
//    else printf("No header\n");
//#endif
//
//    return 0;
//}
