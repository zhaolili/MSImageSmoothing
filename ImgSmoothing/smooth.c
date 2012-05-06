#define  _CRT_SECURE_NO_DEPRECATE

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "static_func.h"
#include "smooth.h"

extern int get_mem2D_imgtype(imgtype ***array2D, int h, int w);
extern int get_mem3D_imgtype(imgtype ****array3D, int blk, int h, int w);
extern void free_mem2D_imgtype(imgtype **array2D);
extern void free_mem3D_imgtype(imgtype ***array3D, int blk);

ImgInfo *pImg;
imgtype ***msImg;

const int Ws = 16;
const int Hs = 32;
const int Hr = 16;
const double epsilon = 0.000001;

int _smoothmain(int w, int h, int color, imgtype *imgdata)
{
	fill_img_info(w, h, color, imgdata);
	meanshift();
	writeoutimg();
	free_mem();

	printf("done.\n");
	return 0;
}

//r...g...b...
int fill_img_info(int w, int h, int color, imgtype *imgdata)
{
	int			i, j, k, imgsize;
	int			tmp;
	imgtype		***pimgdata;
	PixelInfo	*pfeaturevect;

	pImg				= (ImgInfo *)calloc(1, sizeof(ImgInfo));
	pImg->width			= w;
	pImg->height		= h;
	pImg->imgsize		= imgsize = w*h;
	pImg->colorinfo		= color;
	pImg->featureVect	= (PixelInfo *)calloc(pImg->imgsize, sizeof(PixelInfo));
	pfeaturevect		= pImg->featureVect;
	get_mem3D_imgtype(&pimgdata, color, h, w);
	

	for (k=0; k<color; k++)
	{
		for (j=0; j<h; j++)
		{
			for (i=0; i<w; i++)
			{
				tmp					= k*imgsize + j*w + i;
				pimgdata[k][j][i]	= imgdata[tmp]; 
			}
		}
	}

	for (j=0; j<h; j++)
	{
		for (i=0; i<w; i++)
		{
			tmp					= j*w + i;
			pfeaturevect[tmp].x = i;
			pfeaturevect[tmp].y = j;
			pfeaturevect[tmp].r = pimgdata[R][j][i];
			pfeaturevect[tmp].g = pimgdata[G][j][i];
			pfeaturevect[tmp].b = pimgdata[B][j][i];
		}
	}

	free_mem3D_imgtype(pimgdata, color);
	return 0;
}

int meanshift()
{
	int			i, j, k, tmp, flag;
	int			x, y, r, g, b;
	int			lb, rb, tb, bb;
	int			color, h, w, imgsize;
	double		toR, toG, toB, cnt, ken;
	double		pr, pg, pb, cr, cg, cb, diff;
	PixelInfo	*pfeaturevect;

	color			= pImg->colorinfo;
	w				= pImg->width;
	h				= pImg->height;
	imgsize			= pImg->imgsize;
	pfeaturevect	= pImg->featureVect;

	get_mem3D_imgtype(&msImg, color, h, w);

	printf("smoothing.");
	for (k=0; k<imgsize; k++)
	{
		x = pfeaturevect[k].x;
		y = pfeaturevect[k].y;
		r = pfeaturevect[k].r;
		g = pfeaturevect[k].g;
		b = pfeaturevect[k].b;

		lb = ((x-Ws)<0)?0:(x-Ws);
		rb = ((x+Ws)>w)?w:(x+Ws);
		tb = ((y-Ws)<0)?0:(y-Ws);
		bb = ((y+Ws)>h)?h:(y+Ws);
		//lb = 0;
		//rb = w;
		//tb = 0;
		//bb = h;

		cr = cg = cb = -100.0;   //initialization
		pr = pg = pb = -200.0;
		diff = (cr-pr)*(cr-pr) + (cg-pg)*(cg-pg) + (cb-pb)*(cb-pb);
		while(diff>epsilon)
		{
			printf(".");
			toR = toG = toB = 0;
			cnt = 0;
			pr	= cr;
			pg	= cg;
			pb	= cb;

			for (j=tb; j<bb; j++)
			{
				for (i=lb; i<rb; i++)
				{
					tmp = j*w + i;
					ken = norm_kernel(&(pfeaturevect[k]), &(pfeaturevect[tmp]), Hs, Hr);
					toR += pfeaturevect[tmp].r*ken;
					toG += pfeaturevect[tmp].g*ken;
					toB += pfeaturevect[tmp].b*ken;
					cnt += ken;

					//flag = flat_kernel(&(pfeaturevect[k]), &(pfeaturevect[tmp]), Hs, Hr);
					//if (flag!=0)
					//{
					//	cnt++;
					//	toR += pfeaturevect[tmp].r;
					//	toG += pfeaturevect[tmp].g;
					//	toB += pfeaturevect[tmp].b;
					//}
				}
			}
			cr = toR/cnt+0.5;
			cg = toG/cnt+0.5;
			cb = toB/cnt+0.5;
			pfeaturevect[k].r = (cr>255) ? 255 : (imgtype)cr;
			pfeaturevect[k].g = (cg>255) ? 255 : (imgtype)cg;
			pfeaturevect[k].b = (cb>255) ? 255 : (imgtype)cb;
			diff = (cr-pr)*(cr-pr) + (cg-pg)*(cg-pg) + (cb-pb)*(cb-pb);
		}
		msImg[R][y][x] = pfeaturevect[k].r;
		msImg[G][y][x] = pfeaturevect[k].g;
		msImg[B][y][x] = pfeaturevect[k].b;
		printf("\n");
	}
	printf("\n");
	return 0;
}

void free_mem(void)
{
	int blk, h, w;
	blk = pImg->colorinfo;
	h	= pImg->height;
	w	= pImg->width;

	free(pImg->featureVect);
	free(pImg);
	free_mem3D_imgtype(msImg, blk);
}
void writeoutimg()
{
	int j,k;
	int imgsize = pImg->imgsize;
	int blk		= pImg->colorinfo;
	int h		= pImg->height;
	int w		= pImg->width;
	char *ofile = "out.rgb";
	FILE *po;

	if (!(po = fopen(ofile, "wb")))
	{
		printf("open output image file failed.\n");
		exit(-1);
	}

	for (k=0; k<blk; k++)
	{
		for (j=0; j<h; j++)
		{
			if (fwrite(msImg[k][j], sizeof(imgtype), w, po)!=w)
			{
				printf("write failed\n");
				exit(-1);
			}
		}
	}

	fclose(po);
}

#ifdef _CRT_SECURE_NO_DEPRECATE
#undef _CRT_SECURE_NO_DEPRECATE
#endif