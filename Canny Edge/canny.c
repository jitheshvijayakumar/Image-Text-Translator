#include "canny.h"

int main(int argc, char **argv)
{
	int ret = 0;
	ret = readBmp(argv[1]);
	if(ret) {
		printf("Error reading bitmap. Error Code %d\n", ret);
		return ret;
	}
	ret = initDisp(&argc, argv, &renderScene, 5 * bmp.header.width,
				   bmp.header.height);
	if(ret) {
		printf("Error Initializing the Display");
		return ret;
	}
	struct timespec ts[9];
	clock_gettime(CLOCK_MONOTONIC, &ts[0]);
	createGrayscale();
	clock_gettime(CLOCK_MONOTONIC, &ts[1]);
	createGaussian();
	clock_gettime(CLOCK_MONOTONIC, &ts[2]);
	calcSobel();
	clock_gettime(CLOCK_MONOTONIC, &ts[3]);
	calcPrewitt();
	clock_gettime(CLOCK_MONOTONIC, &ts[4]);
	int i;
	for(i = 0; i < 5; i++)
		printf("Nanoseconds: %ld\n", ts[i].tv_nsec);
	glutMainLoop();
	return 0;
}

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	printf("Width: %d, Height: %d, format: %x, type: %x\n",
		   bmp.header.width, bmp.header.height, GL_RGBA,
		   GL_UNSIGNED_INT_8_8_8_8);
	glWindowPos2s(0, 0);
	glDrawPixels(bmp.header.width, bmp.header.height,
		GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, bmp.image);
	glWindowPos2s(bmp.header.width, 0);
	glDrawPixels(bmp.header.width, bmp.header.height,
				 GL_LUMINANCE, GL_FLOAT, bmp.grayscale);
	glWindowPos2s(2 * bmp.header.width, 0);
	glDrawPixels(bmp.header.width, bmp.header.height,
				 GL_LUMINANCE, GL_FLOAT, bmp.blur);
	glWindowPos2s(3 * bmp.header.width, 0);
	glDrawPixels(bmp.header.width, bmp.header.height,
				 GL_LUMINANCE, GL_FLOAT, bmp.sobel);
	glWindowPos2s(4 * bmp.header.width, 0);
	glDrawPixels(bmp.header.width, bmp.header.height,
				 GL_LUMINANCE, GL_FLOAT, bmp.prewitt);
	glFlush();
	glutSwapBuffers();
}

void calcSobel()
{
	if(!bmp.blur)
		return;
	int i;
	bmp.sobel = malloc(bmp.header.size);
	memset(bmp.sobel, 0, bmp.header.size);
	for(i = 1; i < bmp.header.height - 1; i++) {
		int j;
		for(j = 1; j < bmp.header.width - 1; j++) {
			float buf[3][3];
			buf[0][0] = bmp.blur[(i - 1) * bmp.header.width + j - 1];
			buf[0][1] = bmp.blur[(i - 1) * bmp.header.width + j];
			buf[0][2] = bmp.blur[(i - 1) * bmp.header.width + j + 1];
			buf[1][0] = bmp.blur[i * bmp.header.width + j - 1];
			buf[1][1] = bmp.blur[i * bmp.header.width + j];
			buf[1][2] = bmp.blur[i * bmp.header.width + j + 1];
			buf[2][0] = bmp.blur[(i + 1) * bmp.header.width + j - 1];
			buf[2][1] = bmp.blur[(i + 1) * bmp.header.width + j];
			buf[2][2] = bmp.blur[(i + 1) * bmp.header.width + j + 1];
			float gx = -buf[0][0] - 2 * buf[1][0] - buf[2][0] + buf[0][2] + 2 * buf[1][2] + buf[2][2];
			float gy = buf[0][0] + 2 * buf[0][1] + buf[0][2] - buf[2][0] - 2 * buf[2][1] - buf[2][2];
			bmp.sobel[i * bmp.header.width + j] = sqrt(gx * gx + gy * gy);
		}
	}
}

void calcPrewitt()
{
	if(!bmp.blur)
		return;
	int i;
	bmp.prewitt = malloc(bmp.header.size);
	memset(bmp.prewitt, 0, bmp.header.size);
	for(i = 1; i < bmp.header.height - 1; i++) {
		int j;
		for(j = 1; j < bmp.header.width - 1; j++) {
			float buf[3][3];
			buf[0][0] = bmp.blur[(i - 1) * bmp.header.width + j - 1];
			buf[0][1] = bmp.blur[(i - 1) * bmp.header.width + j];
			buf[0][2] = bmp.blur[(i - 1) * bmp.header.width + j + 1];
			buf[1][0] = bmp.blur[i * bmp.header.width + j - 1];
			buf[1][1] = bmp.blur[i * bmp.header.width + j];
			buf[1][2] = bmp.blur[i * bmp.header.width + j + 1];
			buf[2][0] = bmp.blur[(i + 1) * bmp.header.width + j - 1];
			buf[2][1] = bmp.blur[(i + 1) * bmp.header.width + j];
			buf[2][2] = bmp.blur[(i + 1) * bmp.header.width + j + 1];
			float gx = -buf[0][0] - buf[1][0] - buf[2][0] + buf[0][2] + buf[1][2] + buf[2][2];
			float gy = buf[0][0] + buf[0][1] + buf[0][2] - buf[2][0] - buf[2][1] - buf[2][2];
			bmp.prewitt[i * bmp.header.width + j] = sqrt(gx * gx + gy * gy);
		}
	}
}

void createGaussian()
{
	if(!bmp.grayscale)
		return;
	bmp.blur = malloc(bmp.header.size);
	int i;
	for(i = 0; i < bmp.header.height; i++) {
		int j;
		for(j = 0; j < bmp.header.width; j++) {
			bmp.blur[i * bmp.header.width + j] = blur(j, i, &bmp);
		}
	}
}

float blur(int x, int y, struct bitmap *pic)
{
	int i;
	float blurval = 0;
	for(i = 0; i < BLURMTX_H; i++) {
		int j;
		for(j = 0; j < BLURMTX_W; j++) {
			int gx = x + j - BLURMTX_W / 2;
			int gy = y + i - BLURMTX_H / 2;
			if(gx < 0 || gx >= pic->header.width ||
			   gy < 0 || gy >= pic->header.height)
				blurval += pic->average_l * BLURMTX[i][j];
			else
				blurval += pic->grayscale[gy * pic->header.width + gx] *
					BLURMTX[i][j];
		}
	}
	return blurval / BLURMTX_DIV;
}

void createGrayscale()
{
	if(!bmp.image)
		return;
	bmp.grayscale = malloc(bmp.header.size);
	float buf[4];
	int i;
	bmp.average_l = 0;
	for(i = 0; i < bmp.header.size / sizeof(unsigned); i++) {
		/* red clr here */
		buf[0] = bmp.image[i * sizeof(unsigned)];
		/* Green  here*/
		buf[1] = bmp.image[i * sizeof(unsigned) + 1];
		/* Blue here */
		buf[2] = bmp.image[i * sizeof(unsigned) + 2];
		bmp.grayscale[i] = LUM_RED * buf[0] + LUM_GREEN * buf[1] + LUM_BLUE * buf[2];
		/* albi nee edit cheyyumbo luminosity marakkalle , make it default, njan 2^8 aanu edanne*/
		bmp.grayscale[i] /= 255.0f;
		bmp.average_l += bmp.grayscale[i];
	}
	bmp.average_l /= bmp.header.size / sizeof(unsigned);
}

int initDisp(int *argc, char **argv, void (*func)(void), unsigned width, unsigned height)
{
	glutInit(argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	if(!glutCreateWindow("Canny"))
		return 4;
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glutDisplayFunc(func);
	return 0;
}

int readBmp(char *file)
{
	FILE *fbmp;
	fbmp = fopen(file, "r");
	if(!fbmp)
		return 1;
	fread(&bmp.header, 1, sizeof(bmp.header), fbmp);
	if(bmp.header.signature != 0x4d42) {
		fclose(fbmp);
		return 2;
	}
	
	bmp.image = malloc(bmp.header.size);
	if(!bmp.image) {
		fclose(fbmp);
		return 3;
	}
	fseek(fbmp, bmp.header.offset - sizeof(bmp.header), SEEK_CUR);
	size_t bytes = fread(bmp.image, 1, bmp.header.size, fbmp);
	if(bytes != bmp.header.size) {
		printf("Number of bytes read not number needed!!!\n");
	}
	fclose(fbmp);
	return 0;
}
