/*
Copyright (c) 2005 Holger Waechtler
Copyright (c) 2005 Jeremy Fitzhardinge
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The names of the authors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* PNG screenshot code taken from jsgf's PSPGL. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <png.h>
#include <GL/gl.h>

static void writepng(FILE *fp, const unsigned char *image, int width, int height, int invert_alpha)
{
	const unsigned char *rows[height];
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	int row;
	int transforms;

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
		return;
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		goto out_free_write_struct;

	png_init_io(png_ptr, fp);
	png_set_IHDR(png_ptr, info_ptr, width, height,
		     8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
		     PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	for(row = 0; row < height; row++)
		rows[height-1-row] = &image[row * width * 4];

	png_set_rows(png_ptr, info_ptr, (png_byte**)rows);
	transforms = invert_alpha ? PNG_TRANSFORM_INVERT_ALPHA : PNG_TRANSFORM_IDENTITY;
	png_write_png(png_ptr, info_ptr, transforms, NULL);
	png_write_end(png_ptr, info_ptr);

  out_free_write_struct:
	png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
}

void screenshot(const char *basename)
{
	if (basename == NULL)
		basename = "screenshot";
	char name[6 + strlen(basename) + 4 + 4 + 1];
	int count = 0;
	FILE *fp = NULL;
	unsigned char *image;
	int invert_alpha = 0;

	do {
		if (fp)
			fclose(fp);
#if SYS
		sprintf(name, "%s%03d.png", basename, count++);
#else
		sprintf(name, "ms0:/%s%03d.png", basename, count++);
#endif
		fp = fopen(name, "rb");
	} while(fp != NULL && count < 1000);

	if (count == 1000) {
		if (fp)
			fclose(fp);
		return;
	}

	fp = fopen(name, "wb");
	if (fp == NULL)
		return;

	//image = malloc(480*272*4);
	image = memalign(64, 480*272*4);
	glReadPixels(0,0, 480,272, GL_RGBA, GL_UNSIGNED_BYTE, image);

	if (glGetError() != 0)
		goto out;

	/* Check to see if the opacity is 100%. */
	if ((*(unsigned int *)image & 0xff000000) == 0)
	  	invert_alpha = 1;
	writepng(fp, image, 480, 272, invert_alpha);

  out:
	fclose(fp);
	free(image);
}
