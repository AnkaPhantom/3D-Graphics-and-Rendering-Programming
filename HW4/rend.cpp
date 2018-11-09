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

short matlevelN;

int push_into_S(short &target, GzMatrix *Sta, GzMatrix mat);

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
	float rad = (degree*PI) / 180;

	mat[0][0] = (float)cos(rad);
	mat[0][2] = (float)sin(rad);
	mat[1][1] = 1.0;
	mat[2][0] = -1.0f*(float)sin(rad);
	mat[2][2] = (float)cos(rad);
	mat[3][3] = 1.0;

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
	memset(mat, 0, sizeof(float) * 16);
	/*
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
			mat[i][j] = 0;
	}
	*/
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
	matlevelN = -1;
	matlevel = -1;
	numlights = 0;

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

	numlights = 0;
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
	GzMatrix Imat;
	memset(Imat, 0, sizeof(float) * 16);

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
	push_into_S(matlevel, Ximage, matrix);

	GzMatrix Imat;
	memset(Imat, 0, sizeof(float) * 16);

	Imat[0][0] = 1.0;
	Imat[1][1] = 1.0;
	Imat[2][2] = 1.0;
	Imat[3][3] = 1.0;
	
	if (matlevelN < 2)
	{
		push_into_S(matlevelN, Xnorm, Imat);
	}
	else if (matlevelN == 2)
	{
		GzMatrix Xnorm2;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				Xnorm2[i][j] = m_camera.Xiw[i][j];
		}
		Xnorm2[0][3] = 0;
		Xnorm2[1][3] = 0;
		Xnorm2[2][3] = 0;

		push_into_S(matlevelN, Xnorm, Xnorm2);
	}
	else
	{	
		if (matrix[0][1] == 0 && matrix[0][2] == 0 && matrix[1][0] == 0 && matrix[1][2] == 0 && matrix[2][0] == 0 && matrix[2][1] == 0)
			push_into_S(matlevelN, Xnorm, Imat);
		else
			push_into_S(matlevelN, Xnorm, matrix);
		/*
		matrix[0][3] = 0;
		matrix[0][3] = 0;
		matrix[0][3] = 0;

		matrix[0][0] = matrix[0][0] / (float)(sqrt(matrix[0][0] * matrix[0][0] + matrix[0][1] * matrix[0][1] + matrix[0][2] * matrix[0][2]));
		matrix[0][1] = matrix[0][1] / (float)(sqrt(matrix[0][0] * matrix[0][0] + matrix[0][1] * matrix[0][1] + matrix[0][2] * matrix[0][2]));
		matrix[0][2] = matrix[0][2] / (float)(sqrt(matrix[0][0] * matrix[0][0] + matrix[0][1] * matrix[0][1] + matrix[0][2] * matrix[0][2]));

		matrix[1][0] = matrix[1][0] / (float)(sqrt(matrix[1][0] * matrix[1][0] + matrix[1][1] * matrix[1][1] + matrix[1][2] * matrix[1][2]));
		matrix[1][1] = matrix[1][1] / (float)(sqrt(matrix[1][0] * matrix[1][0] + matrix[1][1] * matrix[1][1] + matrix[1][2] * matrix[1][2]));
		matrix[1][2] = matrix[1][2] / (float)(sqrt(matrix[1][0] * matrix[1][0] + matrix[1][1] * matrix[1][1] + matrix[1][2] * matrix[1][2]));

		matrix[2][0] = matrix[2][0] / (float)(sqrt(matrix[2][0] * matrix[2][0] + matrix[2][1] * matrix[2][1] + matrix[2][2] * matrix[2][2]));
		matrix[2][1] = matrix[2][1] / (float)(sqrt(matrix[2][0] * matrix[2][0] + matrix[2][1] * matrix[2][1] + matrix[2][2] * matrix[2][2]));
		matrix[2][2] = matrix[2][2] / (float)(sqrt(matrix[2][0] * matrix[2][0] + matrix[2][1] * matrix[2][1] + matrix[2][2] * matrix[2][2]));

		push_into_S(matlevelN, Xnorm, matrix);*/
	}

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

	if (i >= 0 && i < xres && j >= 0 && j < yres) 
	{
		//	int in = ARRAY(i, j);
		if (z < pixelbuffer[in].z) 
		{
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

	if (i >= 0 && i < xres && j >= 0 && j < yres) 
	{
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
		GzToken tok = nameList[i];

		if (tok == GZ_RGB_COLOR)
		{
			float *c = (float*)valueList[i];
			//float *c = (float*)*(valueList+i);
			flatcolor[0] = c[0];
			flatcolor[1] = c[1];
			flatcolor[2] = c[2];
		}
		else if (tok == GZ_INTERPOLATE)
		{
			int *m = (int*)valueList[i];
			//int *m = (int*)*(valueList+i);
			interp_mode = *m;

			//interp_mode = (int)valueList[i];
		}
		else if (tok == GZ_DIRECTIONAL_LIGHT)
		{
			GzLight *dir = (GzLight*)valueList[i];
			lights[numlights].direction[0] = dir->direction[0];
			lights[numlights].direction[1] = dir->direction[1];
			lights[numlights].direction[2] = dir->direction[2];
			lights[numlights].color[0] = dir->color[0]; //le
			lights[numlights].color[1] = dir->color[1];
			lights[numlights].color[2] = dir->color[2];
			numlights++;
		}
		else if (tok == GZ_AMBIENT_LIGHT)
		{
			GzLight *amb = (GzLight*)valueList[i];
			ambientlight.direction[0] = amb->direction[0];
			ambientlight.direction[1] = amb->direction[1];
			ambientlight.direction[2] = amb->direction[2];
			ambientlight.color[0] = amb->color[0]; //la
			ambientlight.color[1] = amb->color[1];
			ambientlight.color[2] = amb->color[2];
		}
		else if (tok == GZ_AMBIENT_COEFFICIENT)
		{
			float *a = (float*)valueList[i];
			Ka[0] = a[0];
			Ka[1] = a[1];
			Ka[2] = a[2];
		}
		else if (tok == GZ_DIFFUSE_COEFFICIENT)
		{
			float *d = (float*)valueList[i];
			Kd[0] = d[0];
			Kd[1] = d[1];
			Kd[2] = d[2];
		}
		else if (tok == GZ_SPECULAR_COEFFICIENT)
		{
			float *s = (float*)valueList[i];
			Ks[0] = s[0];
			Ks[1] = s[1];
			Ks[2] = s[2];
		}
		else if (tok == GZ_DISTRIBUTION_COEFFICIENT)
		{
			float *s = (float*)valueList[i];
			spec = *s;
		}
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
		GzCoord* nP = (GzCoord*)valueList[1];
		GzCoord vertices[3];
		GzCoord normals[3];
		GzCoord fnormals[3];
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
		float n4d[3][4];
		//GzCoord v4d[4];
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				v4d[i][j] = vP[i][j];
				n4d[i][j] = nP[i][j];
			}
		}
		v4d[0][3] = 1.0f;
		v4d[1][3] = 1.0f;
		v4d[2][3] = 1.0f;
		n4d[0][3] = 1.0f;
		n4d[1][3] = 1.0f;
		n4d[2][3] = 1.0f;

		float t4d[3][4];
		float nt4d[3][4];
		float s = 0;
		float a = 0;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 4; k++)
				{
					s = s + Ximage[matlevel][i][k] * v4d[j][k];
					a = a + Xnorm[matlevelN][i][k] * n4d[j][k];
				}
				t4d[j][i] = s;
				nt4d[j][i] = a;
				s = 0;
				a = 0;
			}
		}
		/*
		if (t4d[0][2] < m_camera.position[2] || t4d[1][2] < m_camera.position[2] || t4d[2][2] < m_camera.position[2])
			break;*/

		//4D>3D
		vertices[0][0] = t4d[0][0] / t4d[0][3];
		vertices[0][1] = t4d[0][1] / t4d[0][3];
		vertices[0][2] = t4d[0][2] / t4d[0][3];
		vertices[1][0] = t4d[1][0] / t4d[1][3];
		vertices[1][1] = t4d[1][1] / t4d[1][3];
		vertices[1][2] = t4d[1][2] / t4d[1][3];
		vertices[2][0] = t4d[2][0] / t4d[2][3];
		vertices[2][1] = t4d[2][1] / t4d[2][3];
		vertices[2][2] = t4d[2][2] / t4d[2][3];

		normals[0][0] = nt4d[0][0] / nt4d[0][3];
		normals[0][1] = nt4d[0][1] / nt4d[0][3];
		normals[0][2] = nt4d[0][2] / nt4d[0][3];
		normals[1][0] = nt4d[1][0] / nt4d[1][3];
		normals[1][1] = nt4d[1][1] / nt4d[1][3];
		normals[1][2] = nt4d[1][2] / nt4d[1][3];
		normals[2][0] = nt4d[2][0] / nt4d[2][3];
		normals[2][1] = nt4d[2][1] / nt4d[2][3];
		normals[2][2] = nt4d[2][2] / nt4d[2][3];

		fnormals[0][0] = nt4d[0][0] / nt4d[0][3];
		fnormals[0][1] = nt4d[0][1] / nt4d[0][3];
		fnormals[0][2] = nt4d[0][2] / nt4d[0][3];
		fnormals[1][0] = nt4d[1][0] / nt4d[1][3];
		fnormals[1][1] = nt4d[1][1] / nt4d[1][3];
		fnormals[1][2] = nt4d[1][2] / nt4d[1][3];
		fnormals[2][0] = nt4d[2][0] / nt4d[2][3];
		fnormals[2][1] = nt4d[2][1] / nt4d[2][3];
		fnormals[2][2] = nt4d[2][2] / nt4d[2][3];

		//sorted by Y
		if (vertices[0][1] > vertices[1][1])
		{
			swap(vertices[0][0], vertices[1][0]);
			swap(vertices[0][1], vertices[1][1]);
			swap(vertices[0][2], vertices[1][2]);
			swap(normals[0][0], normals[1][0]);
			swap(normals[0][1], normals[1][1]);
			swap(normals[0][2], normals[1][2]);
		}
		if (vertices[0][1] > vertices[2][1])
		{
			swap(vertices[0][0], vertices[2][0]);
			swap(vertices[0][1], vertices[2][1]);
			swap(vertices[0][2], vertices[2][2]);
			swap(normals[0][0], normals[2][0]);
			swap(normals[0][1], normals[2][1]);
			swap(normals[0][2], normals[2][2]);
		}
		if (vertices[1][1] > vertices[2][1])
		{
			swap(vertices[1][0], vertices[2][0]);
			swap(vertices[1][1], vertices[2][1]);
			swap(vertices[1][2], vertices[2][2]);
			swap(normals[1][0], normals[2][0]);
			swap(normals[1][1], normals[2][1]);
			swap(normals[1][2], normals[2][2]);
		}

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

		//lighting
		GzColor specmotherfk[3], diffmotherfk[3], ambmotherfk[3], Cmotherfk[3];
		GzColor Fspecmotherfk[3], Fdiffmotherfk[3], Fambmotherfk[3], FCmotherfk[3];
		memset(specmotherfk, 0, sizeof(float) * 9);
		memset(diffmotherfk, 0, sizeof(float) * 9);
		memset(ambmotherfk, 0, sizeof(float) * 9);
		memset(Cmotherfk, 0, sizeof(float) * 9);
		memset(Fspecmotherfk, 0, sizeof(float) * 9);
		memset(Fdiffmotherfk, 0, sizeof(float) * 9);
		memset(Fambmotherfk, 0, sizeof(float) * 9);
		memset(FCmotherfk, 0, sizeof(float) * 9);

		for (int i = 0; i < 3; i++) 
		{
			for (int j = 0; j < numlights; j++) 
			{
				GzCoord E;
				E[0] = 0, E[1] = 0, E[2] = -1.0f;

				float NL = normals[i][0] * (lights[j]).direction[0] + normals[i][1] * (lights[j]).direction[1] + normals[i][2] * (lights[j]).direction[2];
				float NE = normals[i][0] * E[0] + normals[i][1] * E[1] + normals[i][2] * E[2];

				float fNL = fnormals[i][0] * (lights[j]).direction[0] + fnormals[i][1] * (lights[j]).direction[1] + fnormals[i][2] * (lights[j]).direction[2];
				float fNE = fnormals[i][0] * E[0] + fnormals[i][1] * E[1] + fnormals[i][2] * E[2];

				if ((NL > 0 && NE > 0) || (NL < 0 && NE < 0))
				{
					//R = 2(N.L)N-L
					GzCoord R, fR;
					R[0] = 2.0 * NL * normals[i][0] - (lights[j]).direction[0];
					R[1] = 2.0 * NL * normals[i][1] - (lights[j]).direction[1];
					R[2] = 2.0 * NL * normals[i][2] - (lights[j]).direction[2];
					fR[0] = 2.0 * fNL * fnormals[i][0] - (lights[j]).direction[0];
					fR[1] = 2.0 * fNL * fnormals[i][1] - (lights[j]).direction[1];
					fR[2] = 2.0 * fNL * fnormals[i][2] - (lights[j]).direction[2];

					R[0] = R[0] / (float)(sqrt(R[0] * R[0] + R[1] * R[1] + R[2] * R[2]));
					R[1] = R[1] / (float)(sqrt(R[0] * R[0] + R[1] * R[1] + R[2] * R[2]));
					R[2] = R[2] / (float)(sqrt(R[0] * R[0] + R[1] * R[1] + R[2] * R[2]));
					fR[0] = fR[0] / (float)(sqrt(fR[0] * fR[0] + fR[1] * fR[1] + fR[2] * fR[2]));
					fR[1] = fR[1] / (float)(sqrt(fR[0] * fR[0] + fR[1] * fR[1] + fR[2] * fR[2]));
					fR[2] = fR[2] / (float)(sqrt(fR[0] * fR[0] + fR[1] * fR[1] + fR[2] * fR[2]));

					for (int k = 0; k < 3; k++)
					{
						float RE = R[0] * E[0] + R[1] * E[1] + R[2] * E[2];
						if (RE < 0)
							RE = 0;
						if (RE > 1.0)
							RE = 1.0;

						float fRE = fR[0] * E[0] + fR[1] * E[1] + fR[2] * E[2];
						if (fRE < 0)
							fRE = 0;
						if (fRE > 1.0)
							fRE = 1.0;

						specmotherfk[i][k] = specmotherfk[i][k] + Ks[k] * (lights[j]).color[k] * (float)pow((double)(RE), (double)spec);
						Fspecmotherfk[i][k] = Fspecmotherfk[i][k] + Ks[k] * (lights[j]).color[k] * (float)pow((double)(fRE), (double)spec);
						if (NL > 0 && NE > 0)
						{
							//diffmotherfk[i][k] = diffmotherfk[i][k] + Kd[k] * (normals[i][0] * (lights[j]).direction[0] + normals[i][1] * (lights[j]).direction[1] + normals[i][2] * (lights[j]).direction[2]) * (lights[j]).color[k];
							diffmotherfk[i][k] = diffmotherfk[i][k] + (lights[j]).color[k] * Kd[k] * NL;
							Fdiffmotherfk[i][k] = Fdiffmotherfk[i][k] + (lights[j]).color[k] * Kd[k] * fNL;
						}
						else
						{
							//diffmotherfk[i][k] = diffmotherfk[i][k] + Kd[k] * (-1.0f * normals[i][0] * (lights[j]).direction[0] - 1.0f * normals[i][1] * (lights[j]).direction[1] - 1.0f * normals[i][2] * (lights[j]).direction[2]) * (lights[j]).color[k];
							diffmotherfk[i][k] = diffmotherfk[i][k] + (lights[j]).color[k] * Kd[k] * (-1.0f) * NL;
							Fdiffmotherfk[i][k] = Fdiffmotherfk[i][k] + (lights[j]).color[k] * Kd[k] * (-1.0f) * fNL;
						}
					}
				}
			}
			for (int k = 0; k < 3; k++)
			{
				ambmotherfk[i][k] = ambmotherfk[i][k] + ambientlight.color[k] * Ka[k];
				Fambmotherfk[i][k] = Fambmotherfk[i][k] + ambientlight.color[k] * Ka[k];
			}
		}
		for (int j = 0; j < 3; j++) 
		{
			for (int i = 0; i < 3; i++) 
			{
				Cmotherfk[j][i] = specmotherfk[j][i] + diffmotherfk[j][i] + ambmotherfk[j][i];
				FCmotherfk[j][i] = Fspecmotherfk[0][i] + Fdiffmotherfk[0][i] + Fambmotherfk[0][i];

				if (Cmotherfk[j][i] > 1.0)
					Cmotherfk[j][i] = 1.0;
				if (Cmotherfk[j][i] < 0)
					Cmotherfk[j][i] = 0;

				if (FCmotherfk[j][i] > 1.0)
					FCmotherfk[j][i] = 1.0;
				if (FCmotherfk[j][i] < 0)
					FCmotherfk[j][i] = 0;
			}
		}

		// Get the plane to interpolate red
		float rX1 = vertices[1][0] - vertices[0][0];
		float rY1 = vertices[1][1] - vertices[0][1];
		float rZ1 = Cmotherfk[1][0] - Cmotherfk[0][0];
		float frZ1 = FCmotherfk[1][0] - FCmotherfk[0][0];
		float rX2 = vertices[2][0] - vertices[0][0];
		float rY2 = vertices[2][1] - vertices[0][1];
		float rZ2 = Cmotherfk[2][0] - Cmotherfk[0][0];
		float frZ2 = FCmotherfk[2][0] - FCmotherfk[0][0];
		float rpA = (rY1 * rZ2) - (rZ1 * rY2);
		float rpB = (rZ1 * rX2) - (rX1 * rZ2);
		float rpC = (rX1 * rY2) - (rY1 * rX2);
		float rpD = -1.0f * (rpA * vertices[0][0] + rpB * vertices[0][1] + rpC * Cmotherfk[0][0]);

		float frpA = (rY1 * frZ2) - (frZ1 * rY2);
		float frpB = (frZ1 * rX2) - (rX1 * frZ2);
		float frpC = (rX1 * rY2) - (rY1 * rX2);
		float frpD = -1.0f * (frpA * vertices[0][0] + frpB * vertices[0][1] + frpC * FCmotherfk[0][0]);
		// Get the plane to interpolate green
		float gX1 = vertices[1][0] - vertices[0][0];
		float gY1 = vertices[1][1] - vertices[0][1];
		float gZ1 = Cmotherfk[1][1] - Cmotherfk[0][1];
		float fgZ1 = FCmotherfk[1][1] - FCmotherfk[0][1];
		float gX2 = vertices[2][0] - vertices[0][0];
		float gY2 = vertices[2][1] - vertices[0][1];
		float gZ2 = Cmotherfk[2][1] - Cmotherfk[0][1];
		float fgZ2 = FCmotherfk[2][1] - FCmotherfk[0][1];
		float gpA = (gY1 * gZ2) - (gZ1 * gY2);
		float gpB = (gZ1 * gX2) - (gX1 * gZ2);
		float gpC = (gX1 * gY2) - (gY1 * gX2);
		float gpD = -1.0f * (gpA * vertices[0][0] + gpB * vertices[0][1] + gpC * Cmotherfk[0][1]);

		float fgpA = (gY1 * fgZ2) - (fgZ1 * gY2);
		float fgpB = (fgZ1 * gX2) - (gX1 * fgZ2);
		float fgpC = (gX1 * gY2) - (gY1 * gX2);
		float fgpD = -1.0f * (fgpA * vertices[0][0] + fgpB * vertices[0][1] + fgpC * FCmotherfk[0][1]);
		// Get the plane to interpolate blue
		float bX1 = vertices[1][0] - vertices[0][0];
		float bY1 = vertices[1][1] - vertices[0][1];
		float bZ1 = Cmotherfk[1][2] - Cmotherfk[0][2];
		float fbZ1 = FCmotherfk[1][2] - FCmotherfk[0][2];
		float bX2 = vertices[2][0] - vertices[0][0];
		float bY2 = vertices[2][1] - vertices[0][1];
		float bZ2 = Cmotherfk[2][2] - Cmotherfk[0][2];
		float fbZ2 = FCmotherfk[2][2] - FCmotherfk[0][2];
		float bpA = (bY1 * bZ2) - (bZ1 * bY2);
		float bpB = (bZ1 * bX2) - (bX1 * bZ2);
		float bpC = (bX1 * bY2) - (bY1 * bX2);
		float bpD = -1.0f * (bpA * vertices[0][0] + bpB * vertices[0][1] + bpC * Cmotherfk[0][2]);

		float fbpA = (bY1 * fbZ2) - (fbZ1 * bY2);
		float fbpB = (fbZ1 * bX2) - (bX1 * fbZ2);
		float fbpC = (bX1 * bY2) - (bY1 * bX2);
		float fbpD = -1.0f * (fbpA * vertices[0][0] + fbpB * vertices[0][1] + fbpC * FCmotherfk[0][2]);

		// Get the plane to interpolate normalX
		float nxX1 = vertices[1][0] - vertices[0][0];
		float nxY1 = vertices[1][1] - vertices[0][1];
		float nxZ1 = normals[1][0] - normals[0][0];
		float nxX2 = vertices[2][0] - vertices[0][0];
		float nxY2 = vertices[2][1] - vertices[0][1];
		float nxZ2 = normals[2][0] - normals[0][0];
		float nxpA = (nxY1 * nxZ2) - (nxZ1 * nxY2);
		float nxpB = (nxZ1 * nxX2) - (nxX1 * nxZ2);
		float nxpC = (nxX1 * nxY2) - (nxY1 * nxX2);
		float nxpD = -1.0f * (nxpA * vertices[0][0] + nxpB * vertices[0][1] + nxpC * normals[0][0]);
		// Get the plane to interpolate normalY
		float nyX1 = vertices[1][0] - vertices[0][0];
		float nyY1 = vertices[1][1] - vertices[0][1];
		float nyZ1 = normals[1][1] - normals[0][1];
		float nyX2 = vertices[2][0] - vertices[0][0];
		float nyY2 = vertices[2][1] - vertices[0][1];
		float nyZ2 = normals[2][1] - normals[0][1];
		float nypA = (nyY1 * nyZ2) - (nyZ1 * nyY2);
		float nypB = (nyZ1 * nyX2) - (nyX1 * nyZ2);
		float nypC = (nyX1 * nyY2) - (nyY1 * nyX2);
		float nypD = -1.0f * (nypA * vertices[0][0] + nypB * vertices[0][1] + nypC * normals[0][1]);
		// Get the plane to interpolate normalZ
		float nzX1 = vertices[1][0] - vertices[0][0];
		float nzY1 = vertices[1][1] - vertices[0][1];
		float nzZ1 = normals[1][2] - normals[0][2];
		float nzX2 = vertices[2][0] - vertices[0][0];
		float nzY2 = vertices[2][1] - vertices[0][1];
		float nzZ2 = normals[2][2] - normals[0][2];
		float nzpA = (nzY1 * nzZ2) - (nzZ1 * nzY2);
		float nzpB = (nzZ1 * nzX2) - (nzX1 * nzZ2);
		float nzpC = (nzX1 * nzY2) - (nzY1 * nzX2);
		float nzpD = -1.0f * (nzpA * vertices[0][0] + nzpB * vertices[0][1] + nzpC * normals[0][2]);

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

				if ( ((LEE01 > 0 && LEE12 > 0 && LEE20 > 0 && pC != 0) || (LEE01 < 0 && LEE12 < 0 && LEE20 < 0 && pC != 0) || LEE01 == 0 || LEE12 == 0 || LEE20 == 0) && (rpC*gpC*bpC != 0) && (nxpC*nypC*nzpC !=0) )
				{
					float depthZ = -1.0f * (pA * i + pB * j + pD) / pC;
					int cZ = (int)(depthZ + 0.5);
					if (cZ >= 0)
					{
						short red=0, green=0, blue=0;
						if (interp_mode == GZ_FLAT)//Flat
						{
							GzColor colorf;
							colorf[0] = -1.0f * (frpA * i + frpB * j + frpD) / frpC;
							colorf[1] = -1.0f * (fgpA * i + fgpB * j + fgpD) / fgpC;
							colorf[2] = -1.0f * (fbpA * i + fbpB * j + fbpD) / fbpC;

							red = ctoi(colorf[0]);
							green = ctoi(colorf[1]);
							blue = ctoi(colorf[2]);
						}
						else if (interp_mode == GZ_COLOR)//Gauroud
						{
							GzColor color1;
							color1[0] = -1.0f * (rpA * i + rpB * j + rpD) / rpC;
							color1[1] = -1.0f * (gpA * i + gpB * j + gpD) / gpC;
							color1[2] = -1.0f * (bpA * i + bpB * j + bpD) / bpC;

							red = ctoi(color1[0]);
							green = ctoi(color1[1]);
							blue = ctoi(color1[2]);
						}
						else if (interp_mode == GZ_NORMALS)//Phong
						{
							GzColor color2;
							GzCoord interp_normal;
							interp_normal[0] = -1.0f*(nxpA*i + nxpB*j + nxpD) / nxpC;
							interp_normal[1] = -1.0f*(nypA*i + nypB*j + nypD) / nypC;
							interp_normal[2] = -1.0f*(nzpA*i + nzpB*j + nzpD) / nzpC;

							float specI[3], diffI[3], ambI[3];
							memset(specI, 0, sizeof(float) * 3);
							memset(diffI, 0, sizeof(float) * 3);
							memset(ambI, 0, sizeof(float) * 3);

							for (int l = 0; l < numlights; l++)
							{
								GzCoord E;
								E[0] = 0, E[1] = 0, E[2] = -1.0f;

								float nL = interp_normal[0] * lights[l].direction[0] + interp_normal[1] * lights[l].direction[1] + interp_normal[2] * lights[l].direction[2];
								float nE = interp_normal[0] * E[0] + interp_normal[1] * E[1] + interp_normal[2] * E[2];

								if ( (nL > 0 && nE > 0) || (nL < 0 && nE < 0) )
								{
									//R = 2(N.L)N-L
									GzCoord R;
									R[0] = 2.0*nL*interp_normal[0] - lights[l].direction[0];
									R[1] = 2.0*nL*interp_normal[1] - lights[l].direction[1];
									R[2] = 2.0*nL*interp_normal[2] - lights[l].direction[2];

									R[0] = R[0] / (float)sqrt(R[0] * R[0] + R[1] * R[1] + R[2] * R[2]);
									R[1] = R[1] / (float)sqrt(R[0] * R[0] + R[1] * R[1] + R[2] * R[2]);
									R[2] = R[2] / (float)sqrt(R[0] * R[0] + R[1] * R[1] + R[2] * R[2]);

									for (int m = 0; m < 3; m++)
									{
										float rE = R[0] * E[0] + R[1] * E[1] + R[2] * E[2];
										if (rE < 0)
											rE = 0;
										if (rE > 1.0)
											rE = 1.0;

										specI[m] = specI[m] + Ks[m] * (lights[l]).color[m] * (float)pow((double)rE, (double)spec);
										if (nL > 0 && nE > 0)
											diffI[m] = diffI[m] + Kd[m] * (lights[l]).color[m] * nL;
										else
											diffI[m] = diffI[m] + Kd[m] * (lights[l]).color[m] * -1.0f * nL;
									}
								}
							}
							for (int m = 0; m < 3; m++)
								ambI[m] = ambI[m] + Ka[m] * ambientlight.color[m];

							for (int a = 0; a < 3; a++)
							{
								color2[a] = specI[a] + diffI[a] + ambI[a];
								if (color2[a] < 0)
									color2[a] = 0;
								if (color2[a] > 1.0f)
									color2[a] = 1.0f;
							}
							red = ctoi(color2[0]);
							green = ctoi(color2[1]);
							blue = ctoi(color2[2]);
						}
						GzPut(i, j, red, green, blue, 1, cZ);
					}
				}
			}
		}
	}
	return GZ_SUCCESS;
}

int push_into_S(short &target, GzMatrix *Sta, GzMatrix mat)
{
	if (target < MATLEVELS)
	{
		if (target == -1)
		{
			Sta[0][0][0] = mat[0][0];
			Sta[0][0][1] = mat[0][1];
			Sta[0][0][2] = mat[0][2];
			Sta[0][0][3] = mat[0][3];
			Sta[0][1][0] = mat[1][0];
			Sta[0][1][1] = mat[1][1];
			Sta[0][1][2] = mat[1][2];
			Sta[0][1][3] = mat[1][3];
			Sta[0][2][0] = mat[2][0];
			Sta[0][2][1] = mat[2][1];
			Sta[0][2][2] = mat[2][2];
			Sta[0][2][3] = mat[2][3];
			Sta[0][3][0] = mat[3][0];
			Sta[0][3][1] = mat[3][1];
			Sta[0][3][2] = mat[3][2];
			Sta[0][3][3] = mat[3][3];
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
						s = s + Sta[target][i][k] * mat[k][j];

					Sta[target + 1][i][j] = s;
					s = 0;
				}
			}
		}
		target++;
	}
	else
		return GZ_FAILURE;

	return GZ_SUCCESS;
}
