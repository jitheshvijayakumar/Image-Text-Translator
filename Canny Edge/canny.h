
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "gaussian.h"

#define PI 3.14159265358979323846264338327950288
#define SQRT2 1.4142135623730950488016887242096980785696718753769480731766797379907324784621
#define SQRT3 1.7320508075688772935274463415058723669428052538103806

#define LUM_RED 0.299
#define LUM_GREEN 0.587
#define LUM_BLUE 0.114

struct __attribute__((packed)) bmpHead
{
	unsigned short signature;
	unsigned size_ignore;
	unsigned res;
	unsigned offset;
	unsigned headsize;
	unsigned width;
	unsigned height;
	unsigned short planes;
	unsigned short bpp;
	unsigned compression;
	unsigned size;
	unsigned hres;
	unsigned vres;
	unsigned colorcount;
	unsigned impcolorcount;
};

struct bitmap
{
	struct bmpHead header;
	unsigned char *image;
	float average_l;
	float *grayscale;
	float *blur;
	float *sobel;
	float *prewitt;
} bmp;

void renderScene(void);
int readBmp(char *file);
void createGrayscale();
void calcSobel();
void calcPrewitt();
void createGaussian();
float blur(int x, int y, struct bitmap *pic);
int initDisp(int *argc, char **argv, void (*func)(void), unsigned width, unsigned height);
