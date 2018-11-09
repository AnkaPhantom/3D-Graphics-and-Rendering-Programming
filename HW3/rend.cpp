/* CS580 Homework 3 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
#include    <string>
#include    "iostream"
#include    "stdint.h"
using namespace std;

#define PI (float) 3.14159265358979323846

int GzRender::GzRotXMat(float degree, GzMatrix mat)
{
	/* HW 3.1
	// Create rotate matrix : rotate along x axis
	// Pass back the matrix using mat value
	*/
	memset(mat, 0, sizeof(float) * 16);
	/*
	for (int i=0; i<4; i++)
	{
		for (int j=0; j<4; j++)
			mat[i][j] = 0;
	}
	*/
	float rad = (degree*PI) / 180;

	mat[0][0] = 1.0;
	mat[1][1] = (float)cos(rad);
	mat[1][2] = -1.0f*(float)sin(rad);
	mat[2][1] = (float)sin(rad);
	mat[2][2] = (float)cos(rad);
	mat[3][3] = 1.0;

	return GZ_SUCCESS;
}

int GzRender::GzRotYMat(float degree, GzMatrix mat)
{
	/* HW 3.2
	// Create rotate matrix : rotate along y axis
	// Pass back the matrix using mat value
	*/
	memset(mat, 0, sizeof(float) * 16);
	/*
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
			mat[i][j] = 0;
	}
	*/
	float rad = (degree*PI) / 180.0f;

	mat[0][0] = (float)cos(rad);
	mat[0][2] = (float)sin(rad);
	mat[1][1] = 1.0f;
	mat[2][0] = -1.0f*(float)sin(rad);
	mat[2][2] = (float)cos(rad);
	mat[3][3] = 1.0f;

	return GZ_SUCCESS;
}

int GzRender::GzRotZMat(float degree, GzMatrix mat)
{
	/* HW 3.3
	// Create rotate matrix : rotate along z axis
	// Pass back the matrix using mat value
	*/
	memset(mat, 0, sizeof(float) * 16);
	/*
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
			mat[i][j] = 0;
	}
	*/
	float rad = (degree*PI) / 180;

	mat[0][0] = (float)cos(rad);
	mat[0][1] = -1.0f*(float)sin(rad);
	mat[1][0] = (float)sin(rad);
	mat[1][1] = (float)cos(rad);
	mat[2][2] = 1.0;
	mat[3][3] = 1.0;

	return GZ_SUCCESS;
}

int GzRender::GzTrxMat(GzCoord translate, GzMatrix mat)
{
	/* HW 3.4
	// Create translation matrix
	// Pass back the matrix using mat value
	*/
	memset(mat, 0, sizeof(float) * 16);
	/*
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
			mat[i][j] = 0;
	}
	*/
	mat[0][0] = 1.0;
	mat[1][1] = 1.0;
	mat[2][2] = 1.0;
	mat[3][3] = 1.0;
	mat[0][3] = translate[0];
	mat[1][3] = translate[1];
	mat[2][3] = translate[2];

	return GZ_SUCCESS;
}


int GzRender::GzScaleMat(GzCoord scale, GzMatrix mat)
{
	/* HW 3.5
	// Create scaling matrix
	// Pass back the matrix using mat value
	*/
	//memset(mat, 0, sizeof(float) * 16);
	
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
			mat[i][j] = 0;
	}
	
	mat[0][0] = scale[0];
	mat[1][1] = scale[1];
	mat[2][2] = scale[2];
	mat[3][3] = 1.0;

	return GZ_SUCCESS;
}


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

	framebuffer = (char*)malloc(3 * sizeof(char) * xRes * yRes);
	//framebuffer = new char[3 * resolution];
	pixelbuffer = new GzPixel[resolution];

	/* HW 3.6
	- setup Xsp and anything only done once
	- init default camera
	*/
	matlevel = -1;

	memset(Xsp, 0, sizeof(float) * 16);
	/*
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
			Xsp[i][j] = 0;
	}
	*/
	Xsp[0][0] = (float)(xres / 2.0);
	Xsp[0][3] = (float)(xres / 2.0);
	Xsp[1][1] = -1.0f*((float)yres / 2.0);
	Xsp[1][3] = (float)(yres / 2.0);
	Xsp[2][2] = (float)MAXINT;
	Xsp[3][3] = 1.0;

	m_camera.position[0] = DEFAULT_IM_X;
	m_camera.position[1] = DEFAULT_IM_Y;
	m_camera.position[2] = DEFAULT_IM_Z;
	m_camera.lookat[0] = 0;
	m_camera.lookat[1] = 0;
	m_camera.lookat[2] = 0;
	m_camera.worldup[0] = 0;
	m_camera.worldup[1] = 1.0;
	m_camera.worldup[2] = 0;
	m_camera.FOV = DEFAULT_FOV;

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

	for (int i = 0; i < xres * yres; i++) 
	{
		pixelbuffer[i] = defaultvalue;
		
		framebuffer[3 * i] = (char)0;
		framebuffer[3 * i + 1] = (char)3000;
		framebuffer[3 * i + 2] = (char)3000;
		
		//framebuffer[3 * i + 3] = (char)MAXINT;	// initialize Z.
	}

	return GZ_SUCCESS;
}

int GzRender::GzBeginRender()
{
	/* HW 3.7
	- setup for start of each frame - init frame buffer color,alpha,z
	- compute Xiw and projection xform Xpi from camera definition
	- init Ximage - put Xsp at base of stack, push on Xpi and Xiw
	- now stack contains Xsw and app can push model Xforms when needed
	*/
	memset(m_camera.Xpi, 0, sizeof(float) * 16);
	/*
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
			m_camera.Xpi[i][j] = 0;
	}
	*/
	m_camera.Xpi[0][0] = 1.0;
	m_camera.Xpi[1][1] = 1.0;
	m_camera.Xpi[2][2] = (float)tan((m_camera.FOV * PI / 180.0) / 2.0);
	m_camera.Xpi[3][2] = (float)tan((m_camera.FOV * PI / 180.0) / 2.0);
	m_camera.Xpi[3][3] = 1.0;

	GzCoord cl;
	cl[0] = m_camera.lookat[0] - m_camera.position[0];
	cl[1] = m_camera.lookat[1] - m_camera.position[1];
	cl[2] = m_camera.lookat[2] - m_camera.position[2];

	GzCoord nZ;
	nZ[0] = cl[0] / (float)(sqrt(cl[0] * cl[0] + cl[1] * cl[1] + cl[2] * cl[2]));
	nZ[1] = cl[1] / (float)(sqrt(cl[0] * cl[0] + cl[1] * cl[1] + cl[2] * cl[2]));
	nZ[2] = cl[2] / (float)(sqrt(cl[0] * cl[0] + cl[1] * cl[1] + cl[2] * cl[2]));

	float dZ = m_camera.worldup[0] * nZ[0] + m_camera.worldup[1] * nZ[1] + m_camera.worldup[2] * nZ[2];

	GzCoord newup;
	newup[0] = m_camera.worldup[0] - dZ*nZ[0];
	newup[1] = m_camera.worldup[1] - dZ*nZ[1];
	newup[2] = m_camera.worldup[2] - dZ*nZ[2];

	GzCoord nY;
	nY[0] = newup[0] / (float)(sqrt(newup[0] * newup[0] + newup[1] * newup[1] + newup[2] * newup[2]));
	nY[1] = newup[1] / (float)(sqrt(newup[0] * newup[0] + newup[1] * newup[1] + newup[2] * newup[2]));
	nY[2] = newup[2] / (float)(sqrt(newup[0] * newup[0] + newup[1] * newup[1] + newup[2] * newup[2]));

	GzCoord nX;
	nX[0] = nY[1] * nZ[2] - nY[2] * nZ[1];
	nX[1] = nY[2] * nZ[0] - nY[0] * nZ[2];
	nX[2] = nY[0] * nZ[1] - nY[1] * nZ[0];

	memset(m_camera.Xiw, 0, sizeof(float) * 16);
	/*
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
			m_camera.Xiw[i][j] = 0;
	}
	*/
	m_camera.Xiw[0][0] = nX[0];
	m_camera.Xiw[0][1] = nX[1];
	m_camera.Xiw[0][2] = nX[2];
	m_camera.Xiw[0][3] = -1.0f * (nX[0] * m_camera.position[0] + nX[1] * m_camera.position[1] + nX[2] * m_camera.position[2]);
	m_camera.Xiw[1][0] = nY[0];
	m_camera.Xiw[1][1] = nY[1];
	m_camera.Xiw[1][2] = nY[2];
	m_camera.Xiw[1][3] = -1.0f * (nY[0] * m_camera.position[0] + nY[1] * m_camera.position[1] + nY[2] * m_camera.position[2]);
	m_camera.Xiw[2][0] = nZ[0];
	m_camera.Xiw[2][1] = nZ[1];
	m_camera.Xiw[2][2] = nZ[2];
	m_camera.Xiw[2][3] = -1.0f * (nZ[0] * m_camera.position[0] + nZ[1] * m_camera.position[1] + nZ[2] * m_camera.position[2]);
	m_camera.Xiw[3][3] = 1.0;

	GzPushMatrix(Xsp);
	GzPushMatrix(m_camera.Xpi);
	GzPushMatrix(m_camera.Xiw);

	return GZ_SUCCESS;
}

int GzRender::GzPutCamera(GzCamera camera)
{
	/* HW 3.8
	/*- overwrite renderer camera structure with new camera definition
	*/
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m_camera.Xiw[i][j] = camera.Xiw[i][j];
			m_camera.Xpi[i][j] = camera.Xpi[i][j];
		}
	}

	m_camera.position[0] = camera.position[0];
	m_camera.position[1] = camera.position[1];
	m_camera.position[2] = camera.position[2];
	m_camera.lookat[0] = camera.lookat[0];
	m_camera.lookat[1] = camera.lookat[1];
	m_camera.lookat[2] = camera.lookat[2];
	m_camera.worldup[0] = camera.worldup[0];
	m_camera.worldup[1] = camera.worldup[1];
	m_camera.worldup[2] = camera.worldup[2];
	m_camera.FOV = camera.FOV;

	return GZ_SUCCESS;
}

int GzRender::GzPushMatrix(GzMatrix	matrix)
{
	/* HW 3.9
	- push a matrix onto the Ximage stack
	- check for stack overflow
	*/
	if (matlevel < MATLEVELS)
	{
		if (matlevel == -1)
		{
			Ximage[0][0][0] = matrix[0][0];
			Ximage[0][0][1] = matrix[0][1];
			Ximage[0][0][2] = matrix[0][2];
			Ximage[0][0][3] = matrix[0][3];
			Ximage[0][1][0] = matrix[1][0];
			Ximage[0][1][1] = matrix[1][1];
			Ximage[0][1][2] = matrix[1][2];
			Ximage[0][1][3] = matrix[1][3];
			Ximage[0][2][0] = matrix[2][0];
			Ximage[0][2][1] = matrix[2][1];
			Ximage[0][2][2] = matrix[2][2];
			Ximage[0][2][3] = matrix[2][3];
			Ximage[0][3][0] = matrix[3][0];
			Ximage[0][3][1] = matrix[3][1];
			Ximage[0][3][2] = matrix[3][2];
			Ximage[0][3][3] = matrix[3][3];
		}
		else
		{
			float s = 0;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					//float s = 0;
					for (int k = 0; k < 4; k++)
						s = s + Ximage[matlevel][i][k] * matrix[k][j];

					Ximage[matlevel + 1][i][j] = s;
					s = 0;
				}
			}
		}
		matlevel++;
	}
	else
		return GZ_FAILURE;

	return GZ_SUCCESS;
}

int GzRender::GzPopMatrix()
{
	/* HW 3.10
	- pop a matrix off the Ximage stack
	- check for stack underflow
	*/
	if (matlevel < 0)
		return GZ_FAILURE;

	matlevel--;

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

int GzRender::GzPutTriangle(int numParts, GzToken *nameList, GzPointer *valueList)
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
/*
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
*/
		float v4d[3][4];
		//GzCoord v4d[4];
		v4d[0][0] = vP[0][0];
		v4d[0][1] = vP[0][1];
		v4d[0][2] = vP[0][2];
		v4d[1][0] = vP[1][0];
		v4d[1][1] = vP[1][1];
		v4d[1][2] = vP[1][2];
		v4d[2][0] = vP[2][0];
		v4d[2][1] = vP[2][1];
		v4d[2][2] = vP[2][2];
		v4d[0][3] = 1.0;
		v4d[1][3] = 1.0;
		v4d[2][3] = 1.0;

		float t4d[3][4];
		float s = 0;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 4; k++)
					s = s + Ximage[matlevel][i][k] * v4d[j][k];

				t4d[j][i] = s;
				s = 0;
			}
		}

		if (t4d[0][2] < m_camera.position[2] || t4d[1][2] < m_camera.position[2] || t4d[2][2] < m_camera.position[2])
			break;

		vertices[0][0] = t4d[0][0] / t4d[0][3];
		vertices[0][1] = t4d[0][1] / t4d[0][3];
		vertices[0][2] = t4d[0][2] / t4d[0][3];
		vertices[1][0] = t4d[1][0] / t4d[1][3];
		vertices[1][1] = t4d[1][1] / t4d[1][3];
		vertices[1][2] = t4d[1][2] / t4d[1][3];
		vertices[2][0] = t4d[2][0] / t4d[2][3];
		vertices[2][1] = t4d[2][1] / t4d[2][3];
		vertices[2][2] = t4d[2][2] / t4d[2][3];

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
			if (i < 0 || i > xres)
				continue;

			for (int j = minYpix; j <= maxYpix; j++)
			{
				if (j < 0 || j > yres)
					continue;

				float LEE01 = a01 * i + b01 * j + c01; // Ax + By + C = ?
				float LEE12 = a12 * i + b12 * j + c12;
				float LEE20 = a20 * i + b20 * j + c20;

				if ((LEE01 > 0 && LEE12 > 0 && LEE20 > 0 && pC != 0) || (LEE01 < 0 && LEE12 < 0 && LEE20 < 0 && pC != 0) || LEE01 == 0 || LEE12 == 0 || LEE20 == 0)
				{
					float depthZ = -1.0f * (pA * i + pB * j + pD) / pC;
					int cZ = (int)(depthZ + 0.5);
					if (cZ >= 0)
					{
						short red = ctoi(flatcolor[0]);
						short green = ctoi(flatcolor[1]);
						short blue = ctoi(flatcolor[2]);
						GzPut(i, j, red, green, blue, 1, cZ);
					}
				}
			}
		}
	}
	return GZ_SUCCESS;
}

