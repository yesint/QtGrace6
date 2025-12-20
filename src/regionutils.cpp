/*
 * Grace - Graphics for Exploratory Data Analysis
 * 
 * Home page: http://plasma-gate.weizmann.ac.il/Grace/
 * 
 * Copyright (c) 1991-1995 Paul J Turner, Portland, OR
 * Copyright (c) 1996-2001 Grace Development Team
 * 
 * Maintained by Evgeny Stambulchik
 * 
 * Modified by Andreas Winter 2008-2022
 * 
 *                           All Rights Reserved
 * 
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 * 
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 * 
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 *
 * routines to allocate, manipulate, and return
 * information about regions.
 *
 */

///#include <config.h>

#include <cstdio>
#include <cstdlib>

#include "globals.h"

#include "draw.h"
#include "graphs.h"
#include "utils.h"
#include "noxprotos.h"
#include <iostream>
#include <QDebug>

using namespace std;

int regiontype = 0;

/*
 * see if (x,y) lies inside the plot
 */
int inbounds(int gno, double x, double y)
{
WPoint wp;
(void)gno;
    wp.x = x;
    wp.y = y;
return is_validWPoint(wp);
}

int isInWorld(int gno,double x,double y)
{
    if (is_valid_gno(gno)==TRUE)
    {
        if (g[gno].w.xg1<=x && g[gno].w.xg2>=x && g[gno].w.yg1<=y && g[gno].w.yg2>=y)
        return TRUE;
        else
        return FALSE;
    }
    else
    {
    return FALSE;
    }
}

int isactive_region(int regno)
{
return (regno == MAXREGION || regno == MAXREGION + 1 || rg[regno].active == TRUE);
}

int is_region_defined(region * reg)
{
if (reg==NULL) return FALSE;
if (reg->active) return TRUE;
if (reg->type==0 && reg->x1==0.0 && reg->y1==0.0 && reg->x2==0.0 && reg->y2==0.0) return FALSE;
else return TRUE;
}

const char *region_types(int it, int which)
{
const char *s;
    s = "UNDEFINED";
    switch (it) {
    case REGION_TOLEFT:
	s = "REGION_TOLEFT";
	break;
    case REGION_TORIGHT:
	s = "REGION_TORIGHT";
	break;
    case REGION_ABOVE:
	s = "REGION_ABOVE";
	break;
    case REGION_BELOW:
	s = "REGION_BELOW";
	break;
    case REGION_POLYI:
	if (which) {
	    s = "REGION_POLYI";
	} else {
	    s = "INSIDE POLY";
	}
	break;
    case REGION_POLYO:
	if (which) {
	    s = "REGION_POLYO";
	} else {
	    s = "OUTSIDE POLY";
	}
	break;
    case REGION_HORIZI:
      s ="REGION_HORIZI";
      break;
    case REGION_VERTI:
      s ="REGION_VERTI";
      break;
    case REGION_HORIZO:
       s ="REGION_HORIZO";
      break;
    case REGION_VERTO:
      s ="REGION_VERTO";
      break;
    }
return s;
}

int remove_point_from_region(int r,int p)
{
if (!(rg[r].type==REGION_POLYI || rg[r].type==REGION_POLYO)) return RETURN_FAILURE;
if (p>=rg[r].n || p<0 || rg[r].n<1) return RETURN_FAILURE;
for (int i=p;i<rg[r].n-1;i++)
{
    rg[r].x[i]=rg[r].x[i+1];
    rg[r].y[i]=rg[r].y[i+1];
}
rg[r].n--;
return RETURN_SUCCESS;
}

int add_point_to_region(int r,int after, double x,double y)
{
if (!(rg[r].type==REGION_POLYI || rg[r].type==REGION_POLYO)) return RETURN_FAILURE;
if (after>=rg[r].n || after<0) return RETURN_FAILURE;
double * nx = (double*)xcalloc(rg[r].n+1, sizeof(double));
double * ny = (double*)xcalloc(rg[r].n+1, sizeof(double));
memcpy(nx,rg[r].x,after*sizeof(double));
memcpy(ny,rg[r].y,after*sizeof(double));
    nx[after]=x;
    ny[after]=y;
memcpy(nx+after+1,rg[r].x+after,(rg[r].n-after)*sizeof(double));
memcpy(ny+after+1,rg[r].y+after,(rg[r].n-after)*sizeof(double));
XCFREE(rg[r].x);
XCFREE(rg[r].y);
rg[r].x=nx;
rg[r].y=ny;
rg[r].n++;
return RETURN_SUCCESS;
}

void kill_region(int r)
{
    //if (rg[r].active)
    //{
	XCFREE(rg[r].x);
	XCFREE(rg[r].y);
        rg[r].active = FALSE;
        rg[r].n = 0;
        rg[r].type=0;
        rg[r].x1=rg[r].x2=rg[r].y1=rg[r].y2=0.0;
        set_dirtystate();
    //}
}

void kill_all_regions(void)
{
    int r;
    for (r = 0; r < MAXREGION; r++)
    {
        kill_region(r);
    }
}

void change_region_type(int regno,int n_type)
{
if (regno<0 || regno>=MAXREGION || n_type<0 || n_type>=10) return;
if (n_type==rg[regno].type) return;
    if ((n_type==REGION_POLYI || n_type==REGION_POLYO) && rg[regno].n<=0)
    {//we want to have a polygon, but only have x1,y1 and x2,y2
        rg[regno].n = 4;
        rg[regno].x = (double*)xcalloc(4, sizeof(double));
        rg[regno].y = (double*)xcalloc(4, sizeof(double));
            rg[regno].x[0] = rg[regno].x1;
            rg[regno].y[0] = rg[regno].y1;
            rg[regno].x[1] = rg[regno].x2;
            rg[regno].y[1] = rg[regno].y1;
            rg[regno].x[2] = rg[regno].x2;
            rg[regno].y[2] = rg[regno].y2;
            rg[regno].x[3] = rg[regno].x1;
            rg[regno].y[3] = rg[regno].y2;
    }
    else if ((rg[regno].x1==0.0 && rg[regno].x2==0.0 && rg[regno].y1==0.0 && rg[regno].y2==0.0) && (rg[regno].type==REGION_POLYI || rg[regno].type==REGION_POLYO) && !(n_type==REGION_POLYI || n_type==REGION_POLYO))
    {//we had a polygon so far, but now want something else
    rg[regno].x1=rg[regno].x[0];
    rg[regno].x2=rg[regno].x[0];
    rg[regno].y1=rg[regno].y[0];
    rg[regno].y2=rg[regno].y[0];
        for (int i=1;i<rg[regno].n;i++)
        {
        if (rg[regno].x[i]<rg[regno].x1) rg[regno].x1=rg[regno].x[i];
        if (rg[regno].x[i]>rg[regno].x2) rg[regno].x2=rg[regno].x[i];
        if (rg[regno].y[i]<rg[regno].y1) rg[regno].y1=rg[regno].y[i];
        if (rg[regno].y[i]>rg[regno].y2) rg[regno].y2=rg[regno].y[i];
        }
    }
rg[regno].type=n_type;
}

void activate_region(int r, int type, int linkto)
{
    kill_region(r);
    rg[r].active = TRUE;
    rg[r].type = type;
    rg[r].linkto = linkto;
    set_dirtystate();
}

int is_set_in_region(int gno,int sno,int rno,int inv)
{
if (is_set_active(gno, sno)==FALSE) return FALSE;
double * x = getx(gno, sno);
double * y = gety(gno, sno);
int contained=0;
//cout << "Looking for G" << gno << ".S" << sno << " if in region=" << rno << " inv=" << inv << endl;
if (rno==MAXREGION)//inside world
{
    for (int i = 0; i < getsetlength(gno, sno); i++)
    {
        if (isInWorld(gno, x[i], y[i]))
        {
        contained=1;
        break;
        }
    }
}
else//in a real region
{
    for (int i = 0; i < getsetlength(gno, sno); i++)
    {
        if (inregion(rno, x[i], y[i]))
        {
        contained=1;
        break;
        }
    }
}
if ((contained==0 && inv==1) || (contained==1 && inv==0)) return TRUE;
else return FALSE;
}

/*
 * report on sets in a region
 */
void reporton_region(int gno, int rno, int type)
{
    char buf[256];
    int i, j, first;//, contained;
    double *x, *y;
    QString report_text;
    report_text.clear();
        if (rno>=0 && rno<MAXREGION)
        sprintf(buf, "Region R%1d contains:\n", rno);
        else if (rno==MAXREGION)
        sprintf(buf, "Inside World:\n");
        else
        sprintf(buf, "Outside World:\n");
    //stufftext(buf);
    report_text+=QString(buf);
    for (j = 0; j < number_of_sets(gno); j++) {
	if (is_set_active(gno, j)) {
	    x = getx(gno, j);
	    y = gety(gno, j);
	    first = 1;
        //contained = 0;
	    for (i = 0; i < getsetlength(gno, j); i++) {
		if (inregion(rno, x[i], y[i])) {
            //contained = 1;
		    switch (type) {
		    case 0:	/* report on sets */
			if (first) {
			    first = 0;
			    sprintf(buf, "  Set S%1d\n", j);
                //stufftext(buf);
                report_text+=QString(buf);
			}
			break;
		    case 1:	/* points */
			if (first) {
			    first = 0;
			    sprintf(buf, "  Set S%1d\n", j);
                //stufftext(buf);
                report_text+=QString(buf);
			}
			sprintf(buf, "    %d %f %f\n", i + 1, x[i], y[i]);
            //stufftext(buf);
            report_text+=QString(buf);
			break;
		    }
		} else {
            ;//contained = 0;
		}
	    }
	}
    }
    //stufftext("\n");
    stufftext(report_text.toLocal8Bit().constData());
}

void load_poly_region(int r, int gno, int n, WPoint *wps)
{
    int i;
    if (n > 2)
    {
	activate_region(r, regiontype, gno);
	rg[r].n = n;
	rg[r].x = (double*)xcalloc(n, sizeof(double));///SIZEOF_DOUBLE);
	rg[r].y = (double*)xcalloc(n, sizeof(double));///SIZEOF_DOUBLE);
        for (i = 0; i < n; i++)
        {
	    rg[r].x[i] = wps[i].x;
	    rg[r].y[i] = wps[i].y;
        }
    }
}

/*
 * routines to determine if a point lies in a polygon
 */
int intersect_to_left(double x, double y, double x1, double y1, double x2, double y2)
{
    double xtmp, m, b;

    /* ignore horizontal lines */
    if (y1 == y2) {
	return 0;
    }
    /* not contained vertically */
    if (((y < y1) && (y < y2)) || ((y > y1) && (y > y2))) {
	return 0;
    }
    /* none of the above, compute the intersection */
    if ((xtmp = x2 - x1) != 0.0) {
	m = (y2 - y1) / xtmp;
	b = y1 - m * x1;
	xtmp = (y - b) / m;
    } else {
	xtmp = x1;
    }
    if (xtmp <= x) {
	/* check for intersections at a vertex */
	/* if this is the max ordinate then accept */
	if (y == y1) {
	    if (y1 > y2) {
		return 1;
	    } else {
		return 0;
	    }
	}
	/* check for intersections at a vertex */
	if (y == y2) {
	    if (y2 > y1) {
		return 1;
	    } else {
		return 0;
	    }
	}
	/* no vertices intersected */
	return 1;
    }
    return 0;
}

/*
 * determine if (x,y) is in the polygon xlist[], ylist[]
 */
int inbound(double x, double y, double *xlist, double *ylist, int n)
{
    int i, l = 0;

    for (i = 0; i < n; i++) {
	l += intersect_to_left(x, y, xlist[i], ylist[i], xlist[(i + 1) % n], ylist[(i + 1) % n]);
    }
    return l % 2;
}

/*
 * routines to determine if a point lies to the left of an infinite line
 */
int isleft(double x, double y, double x1, double y1, double x2, double y2)
{
    double xtmp, m, b;

    /* horizontal lines */
    if (y1 == y2) {
	return 0;
    }
    /* none of the above, compute the intersection */
    if ((xtmp = x2 - x1) != 0.0) {
	m = (y2 - y1) / xtmp;
	b = y1 - m * x1;
	xtmp = (y - b) / m;
    } else {
	xtmp = x1;
    }
    if (xtmp >= x) {
	return 1;
    }
    return 0;
}

/*
 * routines to determine if a point lies to the left of an infinite line
 */
int isright(double x, double y, double x1, double y1, double x2, double y2)
{
    double xtmp, m, b;

    /* horizontal lines */
    if (y1 == y2) {
	return 0;
    }
    if ((xtmp = x2 - x1) != 0.0) {
	m = (y2 - y1) / xtmp;
	b = y1 - m * x1;
	xtmp = (y - b) / m;
    } else {
	xtmp = x1;
    }
    if (xtmp <= x) {
	return 1;
    }
    return 0;
}

/*
 * routines to determine if a point lies above an infinite line
 */
int isabove(double x, double y, double x1, double y1, double x2, double y2)
{
    double ytmp, m, b;

    /* vertical lines */
    if (x1 == x2) {
	return 0;
    }
    if ((ytmp = y2 - y1) != 0.0) {
	m = ytmp / (x2 - x1);
	b = y1 - m * x1;
	ytmp = m * x + b;
    } else {
	ytmp = y1;
    }
    if (ytmp <= y) {
	return 1;
    }
    return 0;
}

/*
 * routines to determine if a point lies below an infinite line
 */
int isbelow(double x, double y, double x1, double y1, double x2, double y2)
{
    double ytmp, m, b;

    /* vertical lines */
    if (x1 == x2) {
	return 0;
    }
    if ((ytmp = y2 - y1) != 0.0) {
	m = ytmp / (x2 - x1);
	b = y1 - m * x1;
	ytmp = m * x + b;
    } else {
	ytmp = y1;
    }
    if (ytmp >= y) {
	return 1;
    }
    return 0;
}

int inregion(int regno, double x, double y)
{
    if (regno == MAXREGION) {
	return (inbounds(get_cg() , x, y));
    }
    if (regno == MAXREGION + 1) {
	return (!inbounds(get_cg() , x, y));
    }
    if (rg[regno].active == TRUE) {
	switch (rg[regno].type) {
	case REGION_POLYI:
	    if (inbound(x, y, rg[regno].x, rg[regno].y, rg[regno].n)) {
		return 1;
	    }
	    break;
	case REGION_POLYO:
	    if (!inbound(x, y, rg[regno].x, rg[regno].y, rg[regno].n)) {
		return 1;
	    }
	    break;
	case REGION_TORIGHT:
	    if (isright(x, y, rg[regno].x1, rg[regno].y1, rg[regno].x2, rg[regno].y2)) {
		return 1;
	    }
	    break;
	case REGION_TOLEFT:
	    if (isleft(x, y, rg[regno].x1, rg[regno].y1, rg[regno].x2, rg[regno].y2)) {
		return 1;
	    }
	    break;
	case REGION_ABOVE:
	    if (isabove(x, y, rg[regno].x1, rg[regno].y1, rg[regno].x2, rg[regno].y2)) {
		return 1;
	    }
	    break;
	case REGION_BELOW:
	    if (isbelow(x, y, rg[regno].x1, rg[regno].y1, rg[regno].x2, rg[regno].y2)) {
		return 1;
	    }
	    break;
	case REGION_HORIZI:
	  return (x >= rg[regno].x1) && ( x <= rg[regno].x2);
	  break;
	case REGION_VERTI:
	  return (y >= rg[regno].y1) && ( y <= rg[regno].y2);
	  break;
	case REGION_HORIZO:
	  return !( (x >= rg[regno].x1) && ( x <= rg[regno].x2) );
	  break;
	case REGION_VERTO:
	  return !( (y >= rg[regno].y1) && ( y <= rg[regno].y2) );
	  break;

	}
    }
    return 0;
}
