/* Texture functions for cs580 GzLib	*/
#include    "stdafx.h" 
#include	"stdio.h"
#include	"Gz.h"
#include	"math.h"
using namespace std;

GzColor	*image=NULL;
int xs, ys;
int reset = 1;

/* Image texture function */
int tex_fun(float u, float v, GzColor color) // accept uv and write color
{
  unsigned char		pixel[3];
  unsigned char     dummy;
  char  		foo[8];
  int   		i, j;
  FILE			*fd;

  if (reset) {          /* open and load texture file */
    fd = fopen ("texture", "rb");
    if (fd == NULL) {
      fprintf (stderr, "texture file not found\n");
      exit(-1);
    }
    fscanf (fd, "%s %d %d %c", foo, &xs, &ys, &dummy);
    image = (GzColor*)malloc(sizeof(GzColor)*(xs+1)*(ys+1));
    if (image == NULL) {
      fprintf (stderr, "malloc for texture image failed\n");
      exit(-1);
    }

    for (i = 0; i < xs*ys; i++) {	/* create array of GzColor values */
      fread(pixel, sizeof(pixel), 1, fd);
      image[i][RED] = (float)((int)pixel[RED]) * (1.0 / 255.0);
      image[i][GREEN] = (float)((int)pixel[GREEN]) * (1.0 / 255.0);
      image[i][BLUE] = (float)((int)pixel[BLUE]) * (1.0 / 255.0);
      }

    reset = 0;          /* init is done */
	fclose(fd);
  }

/* bounds-test u,v to make sure nothing will overflow image array bounds */
/* determine texture cell corner values and perform bilinear interpolation */
/* set color to interpolated GzColor value and return */
  if (u > 1.0)
	  u = 1.0;
  if (u < 0)
	  u = 0;
  if (v > 1.0)
	  v = 1.0;
  if (v < 0)
	  v = 0;

  float resx = u*(xs - 1);
  float resy = v*(ys - 1);

  int lowerx, upperx, lowery, uppery;

  if ( (resx - (int)resx) != 0 ) 
  {
	  lowerx = (int)resx;
	  upperx = (int)(resx + 1);
	  lowery = (int)resy;
	  uppery = (int)(resy + 1);
  }
  else
  {
	  lowerx = (int)resx;
	  upperx = (int)resx;
	  lowery = (int)resy;
	  uppery = (int)resy;
  }

  float s = resx - lowerx;
  float t = resy - lowery;

  GzColor cA, cB, cC, cD;
  cA[0] = image[xs*lowery + lowerx][0];
  cA[1] = image[xs*lowery + lowerx][1];
  cA[2] = image[xs*lowery + lowerx][2];
  cB[0] = image[xs*lowery + upperx][0];
  cB[1] = image[xs*lowery + upperx][1];
  cB[2] = image[xs*lowery + upperx][2];
  cC[0] = image[xs*uppery + upperx][0];
  cC[1] = image[xs*uppery + upperx][1];
  cC[2] = image[xs*uppery + upperx][2];
  cD[0] = image[xs*uppery + lowerx][0];
  cD[1] = image[xs*uppery + lowerx][1];
  cD[2] = image[xs*uppery + lowerx][2];

  for (int i = 0; i < 3; i++)
	  color[i] = s*t*cC[i] + (1 - s)*t*cD[i] + s*(1 - t)*cB[i] + (1 - s)*(1 - t)*cA[i];
  
	return GZ_SUCCESS;
}

/* Procedural texture function - add any procedure texture you can think of  */
int ptex_fun(float u, float v, GzColor color)
{
	if (u > 1.0)
		u = 1.0;
	if (u < 0)
		u = 0;
	if (v > 1.0)
		v = 1.0;
	if (v < 0)
		v = 0;
	
	color[0] = 1 * sin(cos(500 * u * v));
	color[1] = 1 * sin(cos(500 * u * v));
	color[2] = 1 * sin(cos(500 * v * u));
	/*
	color[0] = u*u + v;
	color[1] = u*u + v;
	color[2] = u*u + v;*/
	/*
	float r_min = -2.0, r_max = 2.0, i_min = -2.0, i_max = 2.0;
	float x_real = r_min + u*(r_max - r_min);
	float x_imag = i_max + v*(i_max - i_min);
	int i;
	for (i = 0; i < 100; i++)
	{
		float aa = x_real*x_real - x_imag*x_imag;
		float bb = 2*x_real*x_imag;
		x_real = aa - 0.70176;
		x_imag = bb - 0.3842;
		if ((x_real*x_real + x_imag*x_imag) > 16.0)
			break;
	}
	float s = sqrt();*/

	return GZ_SUCCESS;
}

/* Free texture memory */
int GzFreeTexture()
{
	if(image!=NULL)
		free(image);
	return GZ_SUCCESS;
}

