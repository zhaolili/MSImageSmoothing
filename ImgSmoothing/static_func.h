#ifndef _STATIC_FUNC_H_
#define _STATIC_FUNC_H_


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "smooth.h"

static _inline int flat_kernel(PixelInfo *M1, PixelInfo *M, int hs, int hr)
{
	int dx, dy;
	int dr, dg, db;

	dx = M1->x-M->x;
	dy = M1->y-M->y;
	dx = dx*dx;
	dy = dy*dy;

	dr = M1->r-M->r;
	dg = M1->g-M->g;
	db = M1->b-M->b;
	dr = dr*dr;
	dg = dg*dg;
	db = db*db;

	if ((dx+dy)>(hs*hs))
		return 0;
	if ((dr+dg+db)>(hr*hr))
		return 0;
	
	return 1;
}

static _inline double norm_kernel(PixelInfo *M1, PixelInfo *M, double hs, double hr)
{
	int		dx, dy;
	int		dr, dg, db;
	double	ks, kr;

	dx = M1->x-M->x;
	dy = M1->y-M->y;
	dx = dx*dx;
	dy = dy*dy;

	dr = M1->r-M->r;
	dg = M1->g-M->g;
	db = M1->b-M->b;
	dr = dr*dr;
	dg = dg*dg;
	db = db*db;

	ks = exp(-(dx+dy)/(hs*hs*2));
	kr = exp(-(dr+dg+db)/(hr*hr*2));

	return ks*kr;
}

#endif