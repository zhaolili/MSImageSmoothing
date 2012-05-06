#ifndef _SMOOTH_H_
#define _SMOOTH_H_

typedef unsigned char imgtype;

typedef struct 
{
	int			x;
	int			y;

	imgtype		r;
	imgtype		g;
	imgtype		b;
}PixelInfo;


typedef struct
{
	int			width;
	int			height;
	int			imgsize;				//w*h
	int			colorinfo;			//color: r,g,b

	//imgtype		***imgData;				//[r,g,b][h][w]
	PixelInfo	*featureVect;			//[w*h]
}ImgInfo;

#define R 0
#define G 1
#define B 2

int _smoothmain(int w, int h, int color, imgtype *imgdata);
int fill_img_info(int w, int h, int color, imgtype *imgdata);
int meanshift();
void free_mem(void);
void writeoutimg();

#endif