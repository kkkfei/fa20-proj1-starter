/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	int r[] = {1, 1, 1, 0, 0, -1, -1, -1};
	int c[] = {0,-1, 1, 1,-1, 1,  0,  -1};
	int v[8];
	int next = 0;

 	Color* color = image->image[row*image->cols+col];
	int cur = color->R | (color->G << 8) | (color->B << 16);

	for(int i=0; i<8; ++i)
	{
		int row2 = (row + r[i] + image->rows)%image->rows;
		int col2 = (col + c[i] + image->cols)%image->cols;

		Color* color2 = image->image[row2*image->cols+col2];
		v[i] = color2->R | (color2->G << 8) | (color2->B << 16);
	}

	for(int k=0; k<24; ++k)
	{
		int cnt = 0;
		int mask = 1<<k;

		for(int i=0; i<8; ++i)
		{
			if(v[i] & mask) {
				++cnt;
			}
		}

		if(cur & mask) {
			//live
			int mask2 = 1<<(cnt+9);
			if(mask2 & rule) {
				next |= mask;
			}

		} else {
			//dead

			int mask2 = 1<<cnt;
			if(mask2 & rule) {
				next |= mask;
			}
		}
	}
	
	Color* cc = (Color *)malloc(sizeof(Color));
	cc->R = next & 0xFF;
	cc->G = (next >> 8) & 0xFF;
	cc->B = (next >> 16) & 0xFF;

	//YOUR CODE HERE
	return cc;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	int col = image->cols;
	int row = image->rows;

	Image* im = (Image *)malloc(sizeof(Image));
	im->cols = col;
	im->rows = row;
	im->image = (Color **)malloc(sizeof(Color *) * col * row);
	for(int i=0; i<row; ++i)
	{
		for(int j=0; j<col; ++j)
		{
			im->image[i*col + j] = evaluateOneCell(image, i, j, rule);
		}
	}
	return im;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{

	//YOUR CODE HERE
	int rule;
	sscanf(argv[2], "0x%x", &rule);

	Image *image = readData(argv[1]);
	Image *nextImg = life(image, rule);
	writeData(nextImg);

	freeImage(nextImg);
	freeImage(image);

	return 0;
}
