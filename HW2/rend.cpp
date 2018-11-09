#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
#include    <string>
#include    "iostream"
#include    "stdint.h"
using namespace std;


/***********************************************/
/* HW1 methods: copy here the methods from HW1 */

GzRender::GzRender(int xRes, int yRes)
{
	/* HW1.1 create a framebuffer for MS Windows display:
	-- set display resolution
	-- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
	-- allocate memory for pixel buffer
	*/
	xres = xRes;
	yres = yRes;

	int resolution = 0;
	resolution = xres * yres;
	int framebuffersize = 3 * resolution;	// 3 -> RGB 
	framebuffer = new char[framebuffersize];
	pixelbuffer = new GzPixel[resolution];
}

GzRender::~GzRender()
{
	/* HW1.2 clean up, free buffer memory */
	int framelength = strlen(framebuffer);
	
	for (int i = 0; i < framelength; i++)
	{
		delete &framebuffer[i];
	}
	delete[] framebuffer;
	
	for (int i = 0; i < xres*yres; i++) 
	{
		delete &pixelbuffer[i].red;
		delete &pixelbuffer[i].green;
		delete &pixelbuffer[i].blue;
		delete &pixelbuffer[i].alpha;
		delete &pixelbuffer[i].z;
	}
	delete[] pixelbuffer;
}

int GzRender::GzDefault()
{
	/* HW1.3 set pixel buffer to some default values - start a new frame */
	GzPixel defaultvalue = { 0, 3000, 3000, 1, MAXINT };

	int resolution = xres * yres;
	for (int i = 0; i < resolution; i++) {
		pixelbuffer[i] = defaultvalue;
		/*
		framebuffer[3 * i] = (char)0;
		framebuffer[3 * i + 1] = (char)3000;
		framebuffer[3 * i + 2] = (char)3000;
		*/
		//framebuffer[3 * i + 3] = (char)MAXINT;	// initialize Z.
	}
	return GZ_SUCCESS;
}


int GzRender::GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
	/* HW1.4 write pixel values into the buffer */
	int in = ARRAY(i, j);

	if (i >= 0 && i < xres && j >= 0 && j < yres) {
	//	int in = ARRAY(i, j);
		if (z < pixelbuffer[in].z) {
			pixelbuffer[in].red = r;
			pixelbuffer[in].green = g;
			pixelbuffer[in].blue = b;
			pixelbuffer[in].alpha = a;
			pixelbuffer[in].z = z;
		}
	}
	return GZ_SUCCESS;
}


int GzRender::GzGet(int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z)
{
	/* HW1.5 retrieve a pixel information from the pixel buffer */
	int in = ARRAY(i, j);

	if (i >= 0 && i < xres && j >= 0 && j < yres) {
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
		c[0] = (char)redvalue;
		c[1] = (char)greenvalue;
		c[2] = (char)bluevalue;
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
		framebuffer[3 * i] = (char)bluevalue;

		uint8_t greenvalue = pixelbuffer[i].green >> 4;
		framebuffer[3 * i + 1] = (char)greenvalue;

		uint8_t redvalue = pixelbuffer[i].red >> 4;
		framebuffer[3 * i + 2] = (char)redvalue;
	}

	return GZ_SUCCESS;
}


/***********************************************/
/* HW2 methods: implement from here */

int GzRender::GzPutAttribute(int numAttributes, GzToken	*nameList, GzPointer *valueList)
{
	/* HW 2.1
	-- Set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
	-- In later homeworks set shaders, interpolaters, texture maps, and lights
	*/
	for (int i = 0; i < numAttributes; i++)
	{
		float* c = (float*)valueList[0];
	
		flatcolor[0] = c[0];
		flatcolor[1] = c[1];
		flatcolor[2] = c[2];
	}

	return GZ_SUCCESS;
}

int GzRender::GzPutTriangle(int	numParts, GzToken *nameList, GzPointer *valueList)
/* numParts - how many names and values */
{
	/* HW 2.2
	-- Pass in a triangle description with tokens and values corresponding to
	GZ_NULL_TOKEN:		do nothing - no values
	GZ_POSITION:		3 vert positions in model space
	-- Invoke the rastrizer/scanline framework
	-- Return error code
	*/
	for (int i = 0; i < numParts; i++)
	{
		GzCoord* vP = (GzCoord*)valueList[0];
		GzCoord vertices[3];

		// V1:
		vertices[0][0] = vP[0][0];
		vertices[0][1] = vP[0][1];
		vertices[0][2] = vP[0][2];
		// V2:
		vertices[1][0] = vP[1][0];
		vertices[1][1] = vP[1][1];
		vertices[1][2] = vP[1][2];
		// V3:
		vertices[2][0] = vP[2][0];
		vertices[2][1] = vP[2][1];
		vertices[2][2] = vP[2][2];
		
		if (vertices[0][1] > vertices[1][1])
		{
			swap(vertices[0][0], vertices[1][0]);
			swap(vertices[0][1], vertices[1][1]);
			swap(vertices[0][2], vertices[1][2]);
		}
		if (vertices[0][1] > vertices[2][1])
		{
			swap(vertices[0][0], vertices[2][0]);
			swap(vertices[0][1], vertices[2][1]);
			swap(vertices[0][2], vertices[2][2]);
		}
		if (vertices[1][1] > vertices[2][1])
		{
			swap(vertices[1][0], vertices[2][0]);
			swap(vertices[1][1], vertices[2][1]);
			swap(vertices[1][2], vertices[2][2]);
		}
		//sorted by Y

		float dx01 = vertices[1][0] - vertices[0][0];
		float dy01 = vertices[1][1] - vertices[0][1];
		float dx12 = vertices[2][0] - vertices[1][0];
		float dy12 = vertices[2][1] - vertices[1][1];
		float dx20 = vertices[0][0] - vertices[2][0];
		float dy20 = vertices[0][1] - vertices[2][1];

		// Get Ax + By + C = 0 for LEE
		float a01 = dy01;
		float b01 = -1.0f * dx01;
		float c01 = dx01 * vertices[0][1] - dy01 * vertices[0][0];
		float a12 = dy12;
		float b12 = -1.0f * dx12;
		float c12 = dx12 * vertices[1][1] - dy12 * vertices[1][0];
		float a20 = dy20;
		float b20 = -1.0f * dx20;
		float c20 = dx20 * vertices[2][1] - dy20 * vertices[2][0];

		// Get the plane to interpolate Z:
		float X1 = vertices[1][0] - vertices[0][0];
		float Y1 = vertices[1][1] - vertices[0][1];
		float Z1 = vertices[1][2] - vertices[0][2];
		float X2 = vertices[2][0] - vertices[0][0];
		float Y2 = vertices[2][1] - vertices[0][1];
		float Z2 = vertices[2][2] - vertices[0][2];

		// Get Ax + By + Cz + D = 0 to find z
		float pA = (Y1 * Z2) - (Z1 * Y2);
		float pB = (Z1 * X2) - (X1 * Z2);
		float pC = (X1 * Y2) - (Y1 * X2);
		float pD = -1.0f * (pA * vertices[0][0] + pB * vertices[0][1] + pC * vertices[0][2]);

		// Get Bounding Box
		float minX = min(min(vertices[0][0], vertices[1][0]), vertices[2][0]);
		float maxX = max(max(vertices[0][0], vertices[1][0]), vertices[2][0]);
		float minY = min(min(vertices[0][1], vertices[1][1]), vertices[2][1]);
		float maxY = max(max(vertices[0][1], vertices[1][1]), vertices[2][1]);
		int minXpix = (int)(minX + 0.5);
		int maxXpix = (int)(maxX + 0.5);
		int minYpix = (int)(minY + 0.5);
		int maxYpix = (int)(maxY + 0.5);

		// Rasterize
		for (int i = minXpix; i <= maxXpix; i++)
		{
			for (int j = minYpix; j <= maxYpix; j++)
			{
				float LEE01 = a01 * i + b01 * j + c01; // Ax + By + C = ?
				float LEE12 = a12 * i + b12 * j + c12;
				float LEE20 = a20 * i + b20 * j + c20;

				if ((LEE01 > 0 && LEE12 > 0 && LEE20 > 0 && pC != 0) || (LEE01 < 0 && LEE12 < 0 && LEE20 < 0 && pC != 0) || LEE01 == 0 || LEE12 == 0 || LEE20 == 0)
				{
					float depthZ = -1.0f * (pA * i + pB * j + pD) / pC;
					int cZ = (int)(depthZ + 0.5);
					short red = ctoi(flatcolor[0]);
					short green = ctoi(flatcolor[1]);
					short blue = ctoi(flatcolor[2]);
					GzPut(i, j, red, green, blue, 1, cZ);
				}
			}
		}
	}

	return GZ_SUCCESS;
}