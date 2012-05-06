#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "smooth.h"

int main(int argc, char** argv)
{
	int		w, h, imgsize, color = 3;

	FILE	*pi;
	imgtype *imgdata;

	if (argc<4)
	{
		printf("[infile][width][height]\n");
		return 0;
	}

	w		= atoi(argv[2]);
	h		= atoi(argv[3]);
	imgsize	= w*h;

	if (!(pi = fopen(argv[1], "rb")))
	{
		printf("open input file failed.\n");
		exit(-1);
	}
	imgdata = (imgtype *)calloc(color, sizeof(imgtype)*imgsize);
	assert(imgdata);

	if (fread(imgdata, sizeof(imgtype)*imgsize, color, pi)!=color)
	{
		printf("read data error.\n");
		exit(-1);
	}
	_smoothmain(w, h, color, imgdata);

	fclose(pi);
	free(imgdata);

	return 0;
}