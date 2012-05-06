#include <stdlib.h>
#include <stdio.h>

#include "smooth.h"

int get_mem2D_imgtype(imgtype ***array2D, int h, int w)
{
	int i;
	if ((*array2D = (imgtype**)calloc(h, sizeof(imgtype *)))==NULL)
	{
		printf("2d mem allocation failed.\n");
		exit(-1);
	}
	if (((*array2D)[0] = (imgtype *)calloc(h*w, sizeof(imgtype)))==NULL)
	{
		printf("2d mem allocation failed.\n");
		exit(-1);
	}

	for (i=1; i<h; i++)
		(*array2D)[i] = (*array2D)[i-1] + w;

	return h*w*sizeof(imgtype);
}

int get_mem3D_imgtype(imgtype ****array3D, int blk, int h, int w)
{
	int k;
	if ((*array3D = (imgtype ***)calloc(blk, sizeof(imgtype **)))==NULL)
	{
		printf("3d mem allocation failed.\n");
		exit(-1);
	}
	for (k=0; k<blk; k++)
		get_mem2D_imgtype((*array3D)+k, h, w);

	return blk*h*w*sizeof(imgtype);
}

void free_mem2D_imgtype(imgtype **array2D)
{
	if (array2D)
	{
		if (array2D[0])
			free(array2D[0]);
		else
		{
			printf("2d mem free failed.\n");
			exit(-1);
		}
		free(array2D);
	}
	else
	{
		printf("no 2d mem needs to be free.\n");
	}
}

void free_mem3D_imgtype(imgtype ***array3D, int blk)
{
	int k;
	if (array3D)
	{
		for (k=0;k<blk;k++)
		{ 
			free_mem2D_imgtype(array3D[k]);
		}
		free (array3D);
	} else
	{
		printf("free_mem3D: trying to free unused memory");
		exit(-1);
	}
}

