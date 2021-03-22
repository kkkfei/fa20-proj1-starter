/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
	FILE *file;
	if((file = fopen(filename, "r")) == NULL) {
		printf("Error!");
		return NULL;
	}

	char buff[128];
	int col, row;
	int scale;
	if(fscanf(file, "%s%d%d%d", buff, &col, &row, &scale) != 4)
	{
		printf("Error!");
		return NULL;
	}


	Image* im = (Image *)malloc(sizeof(Image));
	im->cols = col;
	im->rows = row;
	im->image = (Color **)malloc(sizeof(Color *) * col * row);
	for(int i=0; i<row; ++i)
	{
		for(int j=0; j<col; ++j)
		{
			Color* cc = malloc(sizeof(Color));
			im->image[i*col + j] = cc;
			int a, b, c;
			fscanf(file, "%d %d %d", &a, &b, &c);
			//printf("[%d..]", a);
			cc->R = (uint8_t)a;
			cc->G = (uint8_t)b;
			cc->B = (uint8_t)c;
		}
	}

	fclose(file);
	return im;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	printf("P3\n%d %d\n255\n", image->cols, image->rows);

	for(int i=0; i<image->rows; ++i)
	{
		for(int j=0; j<image->cols; ++j)
		{
			Color* c = image->image[i*image->cols + j];
			printf("%3d %3d %3d", c->R, c->G, c->B);
			if(j != image->cols-1) printf("   ");
		}
		printf("\n");
	}

}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
	for(int i=0; i<image->rows; ++i)
	{
		for(int j=0; j<image->cols; ++j)
		{
			Color* c = image->image[i*image->cols + j];
			free(c);
		}
	}
	free(image->image);
	free(image);
}