#include "stdafx.h"
#include "stdio.h"
#include "math.h"
#include "Gz.h"
#include "rend.h"
/*   CS580 HW   */
#include "stdafx.h"  
#include "Gz.h"
#include "iostream"
#include "stdint.h"
using namespace std;


GzRender::GzRender(int xRes, int yRes)
{
/* HW1.1 create a framebuffer for MS Windows display:
 -- set display resolution
 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
 -- allocate memory for pixel buffer
 */
	xres = xRes;
	yres = yRes;

	int resolution = xres * yres;
	int framebuffersize = 3 * resolution;  /* 3 -> RGB */
	framebuffer = new char[framebuffersize];
	pixelbuffer = new GzPixel[resolution];

}

GzRender::~GzRender()
{
/* HW1.2 clean up, free buffer memory */
	delete [] framebuffer;
	delete [] pixelbuffer;
}

int GzRender::GzDefault()
{
/* HW1.3 set pixel buffer to some default values - start a new frame */
	GzPixel defaultvalue = { 0, 4000, 4000, 1, 0 };

	for (int i = 0; i < xres*yres; i++) 
		pixelbuffer[i] = defaultvalue;

	return GZ_SUCCESS;
}


int GzRender::GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
/* HW1.4 write pixel values into the buffer */
	int in = ARRAY(i,j);

	if( i>=0 && j >=0 && i < xres && j < yres ) 
	{
	//	int in = ARRAY(i, j);
		pixelbuffer[in].red = r;
		pixelbuffer[in].green = g;
		pixelbuffer[in].blue = b;
		pixelbuffer[in].alpha = a;
		pixelbuffer[in].z = z;
	}

	return GZ_SUCCESS;
}


int GzRender::GzGet(int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z)
{
/* HW1.5 retrieve a pixel information from the pixel buffer */
	int in = ARRAY(i,j);

	if ( i>=0 && j>=0 && i < xres && j < yres ) {
	//	int in = ARRAY(i, j);
		*r = pixelbuffer[in].red;
		*g = pixelbuffer[in].green;
		*b = pixelbuffer[in].blue;
		*a = pixelbuffer[in].alpha;
		*z = pixelbuffer[in].z;
	}

	return GZ_SUCCESS;
}


int GzRender::GzFlushDisplay2File(FILE* outfile)
{
/* HW1.6 write image to ppm file -- "P6 %d %d 255\r" */
	fprintf(outfile, "P6 %d %d 255\r", xres, yres);
	
	for (int i = 0; i < xres*yres; i++) 
	{
		if (pixelbuffer[i].red < 0)
			pixelbuffer[i].red = 0;
		if (pixelbuffer[i].red > 4095)
			pixelbuffer[i].red = 4095;

		if (pixelbuffer[i].green < 0)
			pixelbuffer[i].green = 0;
		if (pixelbuffer[i].green > 4095)
			pixelbuffer[i].green = 4095;

		if (pixelbuffer[i].blue < 0)
			pixelbuffer[i].blue = 0;
		if (pixelbuffer[i].blue > 4095)
			pixelbuffer[i].blue = 4095;

		uint8_t redvalue = pixelbuffer[i].red >> 4;

		uint8_t greenvalue = pixelbuffer[i].green >> 4;

		uint8_t bluevalue = pixelbuffer[i].blue >> 4;

		char c[3];
		c[0] = (char) redvalue;
		c[1] = (char) greenvalue;
		c[2] = (char) bluevalue;
		fwrite(c, 1, 3, outfile);
	}  

	return GZ_SUCCESS;
}

int GzRender::GzFlushDisplay2FrameBuffer()
{
/* HW1.7 write pixels to framebuffer: 
	- put the pixels into the frame buffer
	- CAUTION: when storing the pixels into the frame buffer, the order is blue, green, and red 
	- NOT red, green, and blue !!!
*/
	for (int i = 0; i < xres*yres; i++)
	{	
		if (pixelbuffer[i].blue < 0)
			pixelbuffer[i].blue = 0;
		if (pixelbuffer[i].blue > 4095)
			pixelbuffer[i].blue = 4095;

		if (pixelbuffer[i].green < 0)
			pixelbuffer[i].green = 0;
		if (pixelbuffer[i].green > 4095)
			pixelbuffer[i].green = 4095;

		if (pixelbuffer[i].red < 0)
			pixelbuffer[i].red = 0;
		if (pixelbuffer[i].red > 4095)
			pixelbuffer[i].red = 4095;

		uint8_t bluevalue = pixelbuffer[i].blue >> 4;
		framebuffer[3 * i] = (char) bluevalue;

		uint8_t greenvalue = pixelbuffer[i].green >> 4;
		framebuffer[3 * i + 1] = (char) greenvalue;

		uint8_t redvalue = pixelbuffer[i].red >> 4;
		framebuffer[3 * i + 2] = (char) redvalue;
	}

	return GZ_SUCCESS;
}