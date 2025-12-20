/*
 * Grace - GRaphing, Advanced Computation and Exploration of data
 * 
 * Home page: http://plasma-gate.weizmann.ac.il/Grace/
 * 
 * Copyright (c) 1991-1995 Paul J Turner, Portland, OR
 * Copyright (c) 1996-2003 Grace Development Team
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
 * utilities for graphs
 *
 */

///#include <config.h>
#include <cmath>

#include <cstdio>

#include "globals.h"
#include "utils.h"
#include "draw.h"
#include "device.h"
#include "graphs.h"
#include "graphutils.h"
#include "noxprotos.h"
#include <iostream>
#include "allWidgets.h"
#include "appearanceWidgets.h"
#include "cmath.h"
#include "undo_module.h"
#include <QtCore>

using namespace std;

extern char print_file[];
extern char dummy[];
extern frmQuestionDialog * FormQuestion;
extern QStringList ListOfChanges;
extern QStringList ListOfOldStates;
extern MainWindow * mainWin;
extern frmAxisProp * FormAxisProperties;
extern frmCreateAltAxis * FormCreateAltAxis;

static void auto_ticks(int gno, int axis);

const char *get_format_types(int f)
{
const char *s;
    s = "decimal";
    switch (f) {
    case FORMAT_DECIMAL:
	s = "decimal";
	break;
    case FORMAT_EXPONENTIAL:
	s = "exponential";
	break;
    case FORMAT_GENERAL:
	s = "general";
	break;
    case FORMAT_POWER:
	s = "power";
	break;
    case FORMAT_SCIENTIFIC:
	s = "scientific";
	break;
    case FORMAT_ENGINEERING:
	s = "engineering";
	break;
    case FORMAT_COMPUTING:
	s = "computing";
	break;
    case FORMAT_DDMMYY:
	s = "ddmmyy";
	break;
    case FORMAT_MMDDYY:
	s = "mmddyy";
	break;
    case FORMAT_YYMMDD:
	s = "yymmdd";
	break;
    case FORMAT_MMYY:
	s = "mmyy";
	break;
    case FORMAT_MMDD:
	s = "mmdd";
	break;
    case FORMAT_MONTHDAY:
	s = "monthday";
	break;
    case FORMAT_DAYMONTH:
	s = "daymonth";
	break;
    case FORMAT_MONTHS:
	s = "months";
	break;
    case FORMAT_MONTHSY:
	s = "monthsy";
	break;
    case FORMAT_MONTHL:
	s = "monthl";
	break;
    case FORMAT_DAYOFWEEKS:
	s = "dayofweeks";
	break;
    case FORMAT_DAYOFWEEKL:
	s = "dayofweekl";
	break;
    case FORMAT_DAYOFYEAR:
	s = "dayofyear";
	break;
    case FORMAT_HMS:
	s = "hms";
	break;
    case FORMAT_MMDDHMS:
	s = "mmddhms";
	break;
    case FORMAT_MMDDYYHMS:
	s = "mmddyyhms";
	break;
    case FORMAT_YYMMDDHMS:
	s = "yymmddhms";
	break;
    case FORMAT_DEGREESLON:
	s = "degreeslon";
	break;
    case FORMAT_DEGREESMMLON:
	s = "degreesmmlon";
	break;
    case FORMAT_DEGREESMMSSLON:
	s = "degreesmmsslon";
	break;
    case FORMAT_MMSSLON:
	s = "mmsslon";
	break;
    case FORMAT_DEGREESLAT:
	s = "degreeslat";
	break;
    case FORMAT_DEGREESMMLAT:
	s = "degreesmmlat";
	break;
    case FORMAT_DEGREESMMSSLAT:
	s = "degreesmmsslat";
	break;
    case FORMAT_MMSSLAT:
	s = "mmsslat";
	break;
    case FORMAT_YYYY:
    s = "yyyy";
    break;
    case FORMAT_YYDYHMS:
    s = "yydyhms";
    break;
    default:
	s = "unknown";
        errmsg("Internal error in get_format_types()");
	break;
    }
    return s;
}

int wipeout(void)
{
//qDebug() << "wipeout: noask=" << noask << "dirty=" << is_dirtystate();
    if (!noask && is_dirtystate())
    {
    int ret;
        /*
        FormQuestion->init(QObject::tr("Abandon unsaved changes?"),QObject::tr("Close Project"));
        ret=FormQuestion->exec();
        if (ret==0)//No
            return 1;
        */
        /*if (!yesno("Abandon unsaved changes?", NULL, NULL, NULL)) {
            return 1;
        }*/
    ret=yesnosave(1);
    //cout << "in Wipeout(): Answer=" << ret << endl;
        if (ret==0)//Cancel
        return 1;
    }
    //if we get here: everything will be deleted (wiped out)
    kill_all_graphs();
    do_clear_lines();
    do_clear_boxes();
    do_clear_ellipses();
    do_clear_text();
    kill_all_regions();
    reset_project_version();
    map_fonts(FONT_MAP_DEFAULT);
    set_docname(NULL);
    set_project_description(NULL);
    print_file[0] = '\0';
    /* a hack! the global "curtype" (as well as all others) should be removed */
    curtype = SET_XY;
    clear_dirtystate();
    return 0;
}


/* The following routines determine default axis range and tickmarks */

static void autorange_byset(int gno, int setno, int autos_type);
static double nicenum(double x, int nrange, int round);

#define NICE_FLOOR   0
#define NICE_CEIL    1
#define NICE_ROUND   2

void autotick_axis(int gno, int axis)
{
    switch (axis)
    {
    case ALL_AXES:
        auto_ticks(gno, X_AXIS);
        auto_ticks(gno, ZX_AXIS);
        auto_ticks(gno, Y_AXIS);
        auto_ticks(gno, ZY_AXIS);
        break;
    case ALL_X_AXES:
        auto_ticks(gno, X_AXIS);
        auto_ticks(gno, ZX_AXIS);
        break;
    case ALL_Y_AXES:
        auto_ticks(gno, Y_AXIS);
        auto_ticks(gno, ZY_AXIS);
        break;
    default:
        auto_ticks(gno, axis);
        break;
    }
}

void autoscale_byset(int gno, int setno, int autos_type)
{
    if ((setno == ALL_SETS && is_valid_gno(gno)) || is_set_active(gno, setno))
    {
	autorange_byset(gno, setno, autos_type);
        switch (autos_type)
        {
        case AUTOSCALE_X:
            autotick_axis(gno, ALL_X_AXES);
            break;
        case AUTOSCALE_Y:
            autotick_axis(gno, ALL_Y_AXES);
            break;
        case AUTOSCALE_XY:
            autotick_axis(gno, ALL_AXES);
            break;
        default:
            ;/*do nothing*/
            break;
        }
    }
}

int autoscale_graph(int gno, int autos_type)
{
//qDebug() << "autoscale_graph: gno=" << gno << " autos_type=" << autos_type;
    //if (number_of_active_sets(gno) > 0) {
        autoscale_active=1;
    if (number_of_active_autoscale_sets(gno) > 0) {
        autoscale_byset(gno, ALL_SETS, autos_type);
        autoscale_active=0;
        return RETURN_SUCCESS;
    } else {
        autoscale_active=0;
        return RETURN_FAILURE;
    }
}

void round_axis_limits(double *amin, double *amax, int scale)
{
    double smin, smax;
    int nrange;
/*qDebug() << "1 Round Axis Limits:";
sprintf(dummy,"%.17f",*amin);
qDebug() << "amin=" << dummy;
sprintf(dummy,"%.17f",*amax);
qDebug() << "amax=" << dummy;*/
    if (*amin == *amax) {
        switch (sign(*amin)) {
        case 0:
            *amin = -1.0;
            *amax = +1.0;
            break;
        case 1:
            *amin /= 2.0;
            *amax *= 2.0;
            break;
        case -1:
            *amin *= 2.0;
            *amax /= 2.0;
            break;
        }
    } 
    
    if (scale == SCALE_LOG) {
        if (*amax <= 0.0) {
            errmsg("Can't autoscale a log axis by non-positive data");
            *amax = 10.0;
            *amin = 1.0;
            return;
        } else if (*amin <= 0.0) {
            errmsg("Data have non-positive values");
            *amin = *amax/1.0e3;
        }
        smin = log10(*amin);
        smax = log10(*amax);
    } else if (scale == SCALE_LOGIT) {
	if (*amax <= 0.0) {
            errmsg("Can't autoscale a logit axis by non-positive data");
            *amax = 0.9;
            *amin = 0.1;
            return;
        } else if (*amin <= 0.0) {
            errmsg("Data have non-positive values");
            *amin = 0.1;
        }
        smin = log(*amin/(1-*amin));
        smax = log(*amax/(1-*amax));	
    } else {
        smin = *amin;
        smax = *amax;
    }

    if (sign(smin) == sign(smax)) {
        nrange = -rint(log10(fabs(2*(smax - smin)/(smax + smin))));
        nrange = MAX2(0, nrange);
    } else {
        nrange = 0;
    }
    smin = nicenum(smin, nrange, NICE_FLOOR);
    smax = nicenum(smax, nrange, NICE_CEIL);
    if (sign(smin) == sign(smax)) {
        if (smax/smin > 5.0) {
            smin = 0.0;
        } else if (smin/smax > 5.0) {
            smax = 0.0;
        }
    }
    
    if (scale == SCALE_LOG) {
        *amin = pow(10.0, smin);
        *amax = pow(10.0, smax);
    } else if (scale == SCALE_LOGIT) {
	*amin = exp(smin)/(1.0 + exp(smin));
        *amax = exp(smax)/(1.0 + exp(smax));	
    } else {
        *amin = smin;
        *amax = smax;
    }
    /*qDebug() << "2 Round Axis Limits:";
    sprintf(dummy,"%.17f",*amin);
    qDebug() << "amin=" << dummy;
    sprintf(dummy,"%.17f",*amax);
    qDebug() << "amax=" << dummy;*/
}

static void autorange_byset(int gno, int setno, int autos_type)
{
    world w;
    view bb;
    double xmax, xmin, ymax, ymin, offset;
    double o_xmax, o_xmin, o_ymax, o_ymin;
    int scale;

    if (autos_type == AUTOSCALE_NONE || !is_valid_gno(gno)) {
        return;
    }

    if (setno!=ALL_SETS)
    {
    if (!is_valid_setno(gno,setno)) return;
    if (g[gno].p[setno].ignore_in_autoscale==TRUE && autoscale_active==1) return;//no autoscale with this set!
    }

    get_graph_world(gno, &w);
    get_graph_viewport(gno,&bb);

    if (get_graph_type(gno) == GRAPH_SMITH) {
        if (autos_type == AUTOSCALE_X || autos_type == AUTOSCALE_XY) {
            w.xg1 = -1.0;
            w.yg1 = -1.0;
        }
        if (autos_type == AUTOSCALE_Y || autos_type == AUTOSCALE_XY) {
            w.xg2 = 1.0;
            w.yg2 = 1.0;
	}
        set_graph_world(gno, w);
        return;
    }

    xmin=w.xg1;
    xmax=w.xg2;
    ymin=w.yg1;
    ymax=w.yg2;
    if (autos_type == AUTOSCALE_XY) {
        getsetminmax(gno, setno, &xmin, &xmax, &ymin, &ymax, useErrorbarsInAutoscale);
    } else if (autos_type == AUTOSCALE_X) {
        if (useErrorbarsInAutoscale)
        getsetminmax_c2(gno, setno, &xmin, &xmax, &ymin, &ymax, 2);
        else
        getsetminmax_c(gno, setno, &xmin, &xmax, &ymin, &ymax, 2);
    } else if (autos_type == AUTOSCALE_Y) {
        if (useErrorbarsInAutoscale)
        getsetminmax_c2(gno, setno, &xmin, &xmax, &ymin, &ymax, 1);
        else
        getsetminmax_c(gno, setno, &xmin, &xmax, &ymin, &ymax, 1);
    }
    o_xmax=xmax;/*values before rounding*/
    o_xmin=xmin;
    o_ymax=ymax;
    o_ymin=ymin;

/*char dummy_t2[512];
sprintf(dummy_t2,"xmin=%.16g, xmax=%.16g, ymin=%.16g, ymax=%.16g",xmin,xmax,ymin,ymax);
cout << "autoscale A: " << dummy_t2 << endl;*/
//cout << xmin << " " << xmax << " " << ymin << " " << ymax << endl;

    if (autos_type == AUTOSCALE_X || autos_type == AUTOSCALE_XY) {
        scale = get_graph_xscale(gno);
        round_axis_limits(&xmin, &xmax, scale);
        /// we have to check here for the minimum offset (if offset > 0)
        if (minAutoscaleBorderOffset>0.0)
        {
        //offset=minAutoscaleBorderOffset/fabs(bb.xv2-bb.xv1)*fabs(w.xg2-w.xg1);
        offset=fabs(o_xmax-o_xmin)*minAutoscaleBorderOffset;
        if (fabs(o_xmax-xmax)<offset) xmax+=offset;
        if (fabs(o_xmin-xmin)<offset) xmin-=offset;
        round_axis_limits(&xmin, &xmax, scale);
        }
        w.xg1 = xmin;
        w.xg2 = xmax;
    }

    if (autos_type == AUTOSCALE_Y || autos_type == AUTOSCALE_XY) {
        scale = get_graph_yscale(gno);
        round_axis_limits(&ymin, &ymax, scale);
        if (minAutoscaleBorderOffset>0.0)
        {
        //offset=minAutoscaleBorderOffset/fabs(bb.yv2-bb.yv1)*fabs(w.yg2-w.yg1);
        offset=fabs(o_ymax-o_ymin)*minAutoscaleBorderOffset;
        if (fabs(o_ymax-ymax)<offset) ymax+=offset;
        if (fabs(o_ymin-ymin)<offset) ymin-=offset;
        round_axis_limits(&ymin, &ymax, scale);
        }
        w.yg1 = ymin;
        w.yg2 = ymax;
    }

/*sprintf(dummy_t2,"xmin=%.16g, xmax=%.16g, ymin=%.16g, ymax=%.16g",xmin,xmax,ymin,ymax);
cout << "autoscale B: " << dummy_t2 << endl;*/
//cout << xmin << " " << xmax << " " << ymin << " " << ymax << endl;

    //in case of polar plot v2 --> overwrite x-axis-scaling to always be [0,2*PI]
    if (get_graph_type(gno) == GRAPH_POLAR2 && (autos_type == AUTOSCALE_X || autos_type == AUTOSCALE_XY))//the x-axis will be scaled to 0-2*PI
    {
    w.xg1 = 0.0;
    w.xg2 = 2.0*M_PI;
    g[gno].t[0]->tl_stoptype=TYPE_SPEC;
    g[gno].t[0]->tl_stop=2.0*M_PI-0.00001;
    }

    set_graph_world(gno, w);
}

static void auto_ticks(int gno, int axis)
{
    tickmarks *t;
    world w;
    double range, d, tmpmax, tmpmin;
    int axis_scale;

    t = get_graph_tickmarks(gno, axis);
    if (t == NULL) {
        return;
    }
    get_graph_world(gno, &w);

    if (get_graph_type(gno)==GRAPH_POLAR2 && is_xaxis(axis))
    {
    t->tmajor=M_PI/4;
    t->nminor=1;
    set_dirtystate();
    return;
    }

    if (is_xaxis(axis)) {
        tmpmin = w.xg1;
        tmpmax = w.xg2;
        axis_scale = get_graph_xscale(gno);
    } else {
        tmpmin = w.yg1;
        tmpmax = w.yg2;
        axis_scale = get_graph_yscale(gno);
    }

    if (axis_scale == SCALE_LOG) {
	if (t->tmajor <= 1.0) {
            t->tmajor = 10.0;
        }
        tmpmax = log10(tmpmax)/log10(t->tmajor);
	tmpmin = log10(tmpmin)/log10(t->tmajor);
    } else if (axis_scale == SCALE_LOGIT) {
    	if (t->tmajor >= 0.5) {
            t->tmajor = 0.4;
        }
        tmpmax = log(tmpmax/(1-tmpmax))/log(t->tmajor/(1-t->tmajor));
	tmpmin = log(tmpmin/(1-tmpmin))/log(t->tmajor/(1-t->tmajor)); 
    } else if (t->tmajor <= 0.0) {
        t->tmajor = 1.0;
    }
    
    range = tmpmax - tmpmin;
    if (axis_scale == SCALE_LOG) {
	d = ceil(range/(t->t_autonum - 1));
	t->tmajor = pow(t->tmajor, d);
    } 
    else if (axis_scale == SCALE_LOGIT ){
        d = ceil(range/(t->t_autonum - 1));
	t->tmajor = exp(d)/(1.0 + exp(d));
    } 
    else {
	d = nicenum(range/(t->t_autonum - 1), 0, NICE_ROUND);
    /*qDebug() << "Gno=" << gno << "Axis=" << axis << "d=" << d;*/
	t->tmajor = d;
    }

    /* alter # of minor ticks only if the current value is anomalous */
    if (t->nminor < 0 || t->nminor > 10) {
        if (axis_scale != SCALE_LOG) {
	    t->nminor = 1;
        } else {
            t->nminor = 8;
        }
    }
//cout << "d=" << d << " tmpmax=" << tmpmax << " tmpmin=" << tmpmin << " range=" << range << " autonum=" << t->t_autonum << endl;
    set_dirtystate();
}

/*
 * nicenum: find a "nice" number approximately equal to x
 */
static double nicenum(double x, int nrange, int round)
{
    int xsign;
    double f, y, fexp, rx, sx;
    
    if (x == 0.0) {
        return(0.0);
    }

    xsign = sign(x);
    x = fabs(x);

    fexp = floor(log10(x)) - nrange;
    sx = x/pow(10.0, fexp)/10.0;            /* scaled x */
    rx = floor(sx);                         /* rounded x */
    f = 10*(sx - rx);                       /* fraction between 0 and 10 */

    if ((round == NICE_FLOOR && xsign == +1) ||
        (round == NICE_CEIL  && xsign == -1)) {
        y = (int) floor(f);
    } else if ((round == NICE_FLOOR && xsign == -1) ||
               (round == NICE_CEIL  && xsign == +1)) {
	y = (int) ceil(f);
    } else {    /* round == NICE_ROUND */
	if (f < 1.5)
	    y = 1;
	else if (f < 3.)
	    y = 2;
	else if (f < 7.)
	    y = 5;
	else
	    y = 10;
    }
    
    sx = rx + (double) y/10.0;
    
    return (xsign*sx*10.0*pow(10.0, fexp));
}

/*
 * set scroll amount
 */
void scroll_proc(int value)
{
    scrollper = value / 100.0;
}

void scrollinout_proc(int value)
{
    shexper = value / 100.0;
}

/*
 * pan through world coordinates
 */
int graph_scroll(int type,int nr_of_graphs,int * graph_nrs)
{
    world w,orig_w;
    WPoint wp;
    double xmax, xmin, ymax, ymin;
    double vp_to_wx,vp_to_wy;
    double dx,factor,dwc;
    int i;
    //int gstart, gstop, i;

    /*if (scrolling_islinked) {
        gstart = 0;
        gstop = number_of_graphs() - 1;
    } else {
        gstart = get_cg();
        gstop = gstart;
    }*/

    //for (i = gstart; i <= gstop; i++)
    for (int ll=0;ll<nr_of_graphs;ll++)
    {
        i=graph_nrs[ll];
        if (get_graph_world(i, &w) == RETURN_SUCCESS)
        {
        orig_w=w;

        if (type==GSCROLL_UP || type==GSCROLL_RIGHT)
        factor=-1.0;
        else
        factor=1.0;

	    if (islogx(i) == TRUE) {
        // X-Axis logarithmical
		xmin = log10(w.xg1);
		xmax = log10(w.xg2);
        } else if (islogitx(i) == TRUE) {
        // X-Axis in logit-scale
        vp_to_wx=(fscale2(w.xg2,i,X_AXIS)-fscale2(w.xg1,i,X_AXIS));
        wp.x=(fscale2(w.xg1,i,X_AXIS)+fscale2(w.xg2,i,X_AXIS))*0.5-scrollper*vp_to_wx*factor*(1.0-2.0*is_graph_xinvert(i));
        dx=fabs(fscale2(w.xg1,i,X_AXIS)-fscale2(w.xg2,i,X_AXIS));
        w.xg1=ifscale2(wp.x-dx*0.5,i,X_AXIS);
        w.xg2=ifscale2(wp.x+dx*0.5,i,X_AXIS);
            if (w.xg2<w.xg1)
            {
            dx=w.xg1;
            w.xg1=w.xg2;
            w.xg2=dx;
            }
        xmin = w.xg1;
        xmax = w.xg2;
//qDebug() << "Logit-X-Scale: " << xmin << " : " << xmax;
        } else if (isrecx(i) == TRUE) {
        // X-Axis is reciprocal (1/x) (x=0 is not allowed)
            if (w.xg1>0.0 || w.xg2>0.0) dx=1.0;
            else dx=-1.0;
        xmin=log10(fabs(w.xg1));
        xmax=log10(fabs(w.xg2));
            factor*=(xmax-xmin)*scrollper*(1.0-2.0*is_graph_xinvert(i));
//qDebug() << "xmin=" << xmin << " xmax=" << xmax << " factor=" << factor;
            xmin-=factor;
            xmax-=factor;
//qDebug() << "neu: xmin=" << xmin << " xmax=" << xmax;
        w.xg1 = dx*pow(10.0,xmin);
        w.xg2 = dx*pow(10.0,xmax);
//qDebug() << "xg1=" << w.xg1 << " xg2=" << w.xg2;
            if (w.xg2<w.xg1)
            {
            dx=w.xg1;
            w.xg1=w.xg2;
            w.xg2=dx;
            }
        xmin = w.xg1;
        xmax = w.xg2;
        } else {
        // X-Axis linear
		xmin = w.xg1;
		xmax = w.xg2;
	    }
	    
        if (type==GSCROLL_UP || type==GSCROLL_RIGHT)
        factor=-1.0;
        else
        factor=1.0;

	    if (islogy(i) == TRUE) {
        // Y-Axis logarithmical
		ymin = log10(w.yg1);
		ymax = log10(w.yg2);
        } else if (islogity(i) == TRUE) {
        // Y-Axis in logit-scale
        vp_to_wy=(fscale2(w.yg2,i,Y_AXIS)-fscale2(w.yg1,i,Y_AXIS));
        wp.y=(fscale2(w.yg1,i,Y_AXIS)+fscale2(w.yg2,i,Y_AXIS))*0.5-scrollper*vp_to_wy*factor*(1.0-2.0*is_graph_yinvert(i));
        dx=fabs(fscale2(w.yg1,i,Y_AXIS)-fscale2(w.yg2,i,Y_AXIS));
        w.yg1=ifscale2(wp.y-dx*0.5,i,Y_AXIS);
        w.yg2=ifscale2(wp.y+dx*0.5,i,Y_AXIS);
            if (w.yg2<w.yg1)
            {
            dx=w.yg1;
            w.yg1=w.yg2;
            w.yg2=dx;
            }
        ymin = w.yg1;
        ymax = w.yg2;
        } else if (isrecy(i) == TRUE) {
        // Y-Axis is reciprocal (1/y)
            if (w.yg1>0.0 || w.yg2>0.0) dx=1.0;
            else dx=-1.0;
        ymin=log10(fabs(w.yg1));
        ymax=log10(fabs(w.yg2));
            factor*=(ymax-ymin)*scrollper*(1.0-2.0*is_graph_yinvert(i));
//qDebug() << "ymin=" << ymin << " ymax=" << ymax << " factor=" << factor;
            ymin-=factor;
            ymax-=factor;
//qDebug() << "neu: ymin=" << ymin << " ymax=" << ymax;
            w.yg1 = dx*pow(10.0,ymin);
            w.yg2 = dx*pow(10.0,ymax);
//qDebug() << "yg1=" << w.yg1 << " yg2=" << w.yg2;
            if (w.yg2<w.yg1)
            {
            dx=w.yg1;
            w.yg1=w.yg2;
            w.yg2=dx;
            }
        ymin = w.yg1;
        ymax = w.yg2;
	    } else {
        // Y-Axis linear
		ymin = w.yg1;
		ymax = w.yg2;
	    }

        if ( ((type==GSCROLL_LEFT || type==GSCROLL_RIGHT) && islogitx(i) == FALSE && isrecx(i) == FALSE) ||
             ((type==GSCROLL_UP || type==GSCROLL_DOWN) && islogity(i) == FALSE && isrecy(i) == FALSE) )
        {
        dwc = 1.0;
        switch (type) {
        case GSCROLL_LEFT:
        dwc = -1.0;
	    case GSCROLL_RIGHT:    
                dwc *= scrollper * (xmax - xmin);
                xmin += dwc;
                xmax += dwc;
            break;
        case GSCROLL_DOWN:
        dwc = -1.0;
	    case GSCROLL_UP:    
                dwc *= scrollper * (ymax - ymin);/// this was deactivated
                ymin += dwc;
                ymax += dwc;
            break;
        }

	    if (islogx(i) == TRUE) {
		w.xg1 = pow(10.0, xmin);
		w.xg2 = pow(10.0, xmax);
	    } else {
		w.xg1 = xmin;
		w.xg2 = xmax;
	    }
	    
	    if (islogy(i) == TRUE) {
		w.yg1 = pow(10.0, ymin);
		w.yg2 = pow(10.0, ymax);
	    } else {
		w.yg1 = ymin;
		w.yg2 = ymax;
	    }

        }//if (suitable axis scale...)
            if (type==GSCROLL_LEFT || type==GSCROLL_RIGHT)
            {//going left or right (y unchanged)
            w.yg1=orig_w.yg1;
            w.yg2=orig_w.yg2;
            }
            else//going up or down (x unchanged)
            {
            w.xg1=orig_w.xg1;
            w.xg2=orig_w.xg2;
            }
        set_graph_world(i, w);
        }//graph i exists
    }//go through all graphs selected
    
    return RETURN_SUCCESS;
}

int graph_scroll(int type)
{
    int gstart, gstop;

    if (scrolling_islinked) {
        gstart = 0;
        gstop = number_of_graphs() - 1;
    } else {
        gstart = get_cg();
        gstop = gstart;
    }

    int nr_of_graphs=gstop-gstart+1;
    int * graph_nrs=new int[nr_of_graphs+1];
        for (int i=gstart;i<=gstop;i++)
        graph_nrs[i-gstart]=i;
    graph_scroll(type,nr_of_graphs,graph_nrs);
    delete[] graph_nrs;
    return RETURN_SUCCESS;
}

int graph_zoom(int type,int nr_of_graphs,int * graph_nrs)
{
    world w;//,orig_w;
    WPoint wp;
    double xmax, xmin, ymax, ymin;
    double vp_to_wx,vp_to_wy;
    double dy,dx,factor;//dwc
    //int gstart, gstop, gno;
    int gno;

    /*if (scrolling_islinked) {
        gstart = 0;
        gstop = number_of_graphs() - 1;
    } else {
        gstart = get_cg();
        gstop = gstart;
    }*/
    
    //for (gno = gstart; gno <= gstop; gno++)
    for (int ll=0;ll<nr_of_graphs;ll++)
    {
        gno=graph_nrs[ll];
	if (get_graph_world(gno, &w) == RETURN_SUCCESS) {
        switch (g[gno].xscale)
        {
        default:
        case SCALE_NORMAL:
        wp.x=0.5*(w.xg1+w.xg2);
        break;
        case SCALE_LOG:
        wp.x=pow(10.0,0.5*(log10(w.xg1)+log10(w.xg2)));
        break;
        case SCALE_REC:
        wp.x=1.0/(0.5*(1.0/w.xg1+1.0/w.xg2));
        break;
        case SCALE_LOGIT:
        wp.x=0.5*((log(w.xg1/(1.0-w.xg1)))+(log(w.xg2/(1.0-w.xg2))));
        wp.x=(exp(wp.x)/(1+exp(wp.x)));
        break;
        }
        switch (g[gno].yscale)
        {
        default:
        case SCALE_NORMAL:
        wp.y=0.5*(w.yg1+w.yg2);
        break;
        case SCALE_LOG:
        wp.y=pow(10.0,0.5*(log10(w.yg1)+log10(w.yg2)));
        break;
        case SCALE_REC:
        wp.y=1.0/(0.5*(1.0/w.yg1+1.0/w.yg2));
        break;
        case SCALE_LOGIT:
        wp.y=0.5*((log(w.yg1/(1.0-w.yg1)))+(log(w.yg2/(1.0-w.yg2))));
        wp.y=(exp(wp.y)/(1+exp(wp.y)));
        break;
        }
//qDebug() << "Center=" << wp.x << "," << wp.y;
        factor=shexper*2;
        if (type == GZOOM_SHRINK)
        {
        factor*=-1;
        }
        factor+=1.0;
zoomGraphWorldKeepingFixedPoint(gno,w,wp,factor,ALL_AXES);
        continue;

	    if (islogx(gno) == TRUE) {
        // X-Axis logarithmical
		xmin = log10(w.xg1);
		xmax = log10(w.xg2);
        } else if (islogitx(gno) == TRUE) {
        // X-Axis is in logit-scale
        /*vp_to_wy=(fscale2(w.yg2,gno,Y_AXIS)-fscale2(w.yg1,gno,Y_AXIS));
        wp.y=(fscale2(w.yg1,gno,Y_AXIS)+fscale2(w.yg2,gno,Y_AXIS))*0.5-scrollper*vp_to_wy*factor*(1.0-2.0*is_graph_yinvert(gno));
        dy=fabs(fscale2(w.yg1,gno,Y_AXIS)-fscale2(w.yg2,gno,Y_AXIS));
        ymin = w.yg1;
        ymax = w.yg2;*/
        xmin=fscale2(w.xg1,gno,X_AXIS);
        xmax=fscale2(w.xg2,gno,X_AXIS);
        } else if (isrecx(gno) == TRUE) {
        // X-Axis is reciprocal (1/x) (x=0 not allowed)
            if (w.xg1>0 || w.xg2>0) vp_to_wx=1.0;
            else vp_to_wx=-1.0;
        xmin = log10(fabs(w.xg1));
        xmax = log10(fabs(w.xg2));
	    } else {
        // X-Axis linear
		xmin = w.xg1;
		xmax = w.xg2;
	    }
	    
	    if (islogy(gno) == TRUE) {
        // X-Axis logarithmical
		ymin = log10(w.yg1);
		ymax = log10(w.yg2);
        } else if (islogity(gno) == TRUE) {
        // X-Axis is in logit-scale
        ymin=fscale2(w.yg1,gno,Y_AXIS);
        ymax=fscale2(w.yg2,gno,Y_AXIS);
        } else if (isrecy(gno) == TRUE) {
        // X-Axis is reciprocal (1/x) (x=0 not allowed)
            if (w.yg1>0 || w.yg2>0) vp_to_wy=1.0;
            else vp_to_wy=-1.0;
        ymin = log10(fabs(w.yg1));
        ymax = log10(fabs(w.yg2));
	    } else {
        // X-Axis linear
		ymin = w.yg1;
		ymax = w.yg2;
	    }
	    
	    dx = shexper * (xmax - xmin);
	    dy = shexper * (ymax - ymin);
	    if (type == GZOOM_SHRINK) {
		dx *= -1;
		dy *= -1;
	    }

	    xmin -= dx;
	    xmax += dx;
	    ymin -= dy;
	    ymax += dy;
	
	    if (islogx(gno) == TRUE) {
		w.xg1 = pow(10.0, xmin);
		w.xg2 = pow(10.0, xmax);
        } else if (islogitx(gno) == TRUE) {
        w.xg1=ifscale2(xmin,gno,X_AXIS);
        w.xg2=ifscale2(xmax,gno,X_AXIS);
            if (w.xg2<w.xg1)
            {
            dx=w.xg1;
            w.xg1=w.xg2;
            w.xg2=dx;
            }
        } else if (isrecx(gno) == TRUE) {
        w.xg1 = vp_to_wx*pow(10.0, xmin);
        w.xg2 = vp_to_wx*pow(10.0, xmax);
            if (w.xg2<w.xg1)
            {
            vp_to_wx=w.xg1;
            w.xg1=w.xg2;
            w.xg2=vp_to_wx;
            }
        } else {
		w.xg1 = xmin;
		w.xg2 = xmax;
	    }
	    
	    if (islogy(gno) == TRUE) {
		w.yg1 = pow(10.0, ymin);
		w.yg2 = pow(10.0, ymax);
        } else if (islogity(gno) == TRUE) {
        w.yg1=ifscale2(ymin,gno,Y_AXIS);
        w.yg2=ifscale2(ymax,gno,Y_AXIS);
            if (w.yg2<w.yg1)
            {
            dy=w.yg1;
            w.yg1=w.yg2;
            w.yg2=dy;
            }
        } else if (isrecy(gno) == TRUE) {
        w.yg1 = vp_to_wy*pow(10.0, ymin);
        w.yg2 = vp_to_wy*pow(10.0, ymax);
            if (w.yg2<w.yg1)
            {
            vp_to_wy=w.yg1;
            w.yg1=w.yg2;
            w.yg2=vp_to_wy;
            }
	    } else {
		w.yg1 = ymin;
		w.yg2 = ymax;
	    }

        if (get_graph_type(gno)==GRAPH_POLAR2)
        {
        w.xg1=0.0;
        w.xg2=2.0*M_PI;
        }

        /// if zooming results in INF or NAN --> keep original values and report error (for safety)

        set_graph_world(gno, w);
    }//graph gno exists
    }//go through all graphs
    
    return RETURN_SUCCESS;
}

int graph_zoom(int type)
{
    int gstart, gstop;

    if (scrolling_islinked) {
        gstart = 0;
        gstop = number_of_graphs() - 1;
    } else {
        gstart = get_cg();
        gstop = gstart;
    }

    int nr_of_graphs=gstop-gstart+1;
    int * graph_nrs=new int[nr_of_graphs+1];
        for (int i=gstart;i<=gstop;i++)
        graph_nrs[i-gstart]=i;
    graph_zoom(type,nr_of_graphs,graph_nrs);
    delete[] graph_nrs;
    return RETURN_SUCCESS;
}

/*
 *  Arrange graphs
 */
int arrange_graphs(int *graphs, int ngraphs,
                   int nrows, int ncols, int order, int snake,
                   double loff, double roff, double toff, double boff,
                   double vgap, double hgap,
                   int hpack, int vpack, int move_legend,
                   double legendX, double legendY,
                   int packXInner,int packXOuter,int packYInner,int packYOuter,
                   int packXInnerDir,int packXOuterDir,int packYInnerDir,int packYOuterDir)
{
    int i, imax, j, jmax, iw, ih, ng, gno;
    double pw, ph, w, h;
    view v,v_outer;
    view g_prev,g_after;
    double l_xpos,l_ypos,portion;
    tickmarks * t;

    if (hpack) {
        hgap = 0.0;
    }
    if (vpack) {
        vgap = 0.0;
    }
    if (ncols < 1 || nrows < 1) {
	errmsg("# of rows and columns must be > 0");
        return RETURN_FAILURE;
    }
    if (hgap < 0.0 || vgap < 0.0) {
	errmsg("hgap and vgap must be >= 0");
        return RETURN_FAILURE;
    }
    
    get_page_viewport(&pw, &ph);
    w = (pw - loff - roff)/(ncols + (ncols - 1)*hgap);
    h = (ph - toff - boff)/(nrows + (nrows - 1)*vgap);
    if (h <= 0.0 || w <= 0.0) {
	errmsg("Page offsets are too large");
        return RETURN_FAILURE;
    }
    
    ng = 0;
    if (order & GA_ORDER_HV_INV) {
        imax = ncols;
        jmax = nrows;
    } else {
        imax = nrows;
        jmax = ncols;
    }
    for (i = 0; i < imax && ng < ngraphs; i++) {
        for (j = 0; j < jmax && ng < ngraphs; j++) {
            gno = graphs[ng];
            set_graph_active(gno);
            
            get_graph_viewport(gno,&g_prev);

            if (order & GA_ORDER_HV_INV) {
                iw = i;
                ih = j;
                if (snake && (iw % 2)) {
                    ih = nrows - ih - 1;
                }
            } else {
                iw = j;
                ih = i;
                if (snake && (ih % 2)) {
                    iw = ncols - iw - 1;
                }
            }
            if (order & GA_ORDER_H_INV) {
                iw = ncols - iw - 1;
            }
            /* viewport y coord goes bottom -> top ! */
            if (!(order & GA_ORDER_V_INV)) {
                ih = nrows - ih - 1;
            }
            
            v.xv1 = loff + iw*w*(1.0 + hgap);
            v.xv2 = v.xv1 + w;
            v.yv1 = boff + ih*h*(1.0 + vgap);
            v.yv2 = v.yv1 + h;
            set_graph_viewport(gno, v);
            
            if (hpack)
            {
                if (iw == 0)
                {
                t = get_graph_tickmarks(gno, Y_AXIS);
                    if (!t)
                    {
                    continue;
                    }
                t->active = TRUE;
	            t->label_op = PLACEMENT_NORMAL;
	            t->t_op = PLACEMENT_NORMAL;
	            t->tl_op = PLACEMENT_NORMAL;
                }
                else
                {
                    activate_tick_labels(gno, Y_AXIS, FALSE);
                }
            }
            if (vpack)
            {
                if (ih == 0)
                {
                t = get_graph_tickmarks(gno, X_AXIS);
                    if (!t)
                    {
                    continue;
                    }
	            t->active = TRUE;
	            t->label_op = PLACEMENT_NORMAL;
	            t->t_op = PLACEMENT_NORMAL;
	            t->tl_op = PLACEMENT_NORMAL;
                }
                else
                {
                    activate_tick_labels(gno, X_AXIS, FALSE);
                }
            }
            
            ng++;

            if (move_legend==1)//keep relative positions
            {
            get_graph_viewport(gno,&g_after);
            l_xpos=g[gno].l.legx;
            l_ypos=g[gno].l.legy;
            portion=(l_xpos-g_prev.xv1)/(g_prev.xv2-g_prev.xv1);
            g[gno].l.legx=g_after.xv1+portion*(g_after.xv2-g_after.xv1);
            portion=(l_ypos-g_prev.yv1)/(g_prev.yv2-g_prev.yv1);
            g[gno].l.legy=g_after.yv1+portion*(g_after.yv2-g_after.yv1);
            }
            else if (move_legend==2)//set new relative positions
            {
            get_graph_viewport(gno,&g_after);
            g[gno].l.legx=g_after.xv1+legendX*(g_after.xv2-g_after.xv1);
            g[gno].l.legy=g_after.yv1+legendY*(g_after.yv2-g_after.yv1);
            }
        }
    }

    if (hpack || vpack)//do this only if graphs are packed
    {
    //find the outer limits
    get_graph_viewport(graphs[0],&v_outer);
    for (int i=1;i<ngraphs;i++)
    {
    gno = graphs[i];
    get_graph_viewport(gno,&v);
    if (v.xv1<v_outer.xv1) v_outer.xv1=v.xv1;
    if (v.xv2>v_outer.xv2) v_outer.xv2=v.xv2;
    if (v.yv1<v_outer.yv1) v_outer.yv1=v.yv1;
    if (v.yv2>v_outer.yv2) v_outer.yv2=v.yv2;
    }
    //find the position of every graph in the whole grid
    int orientation=0;
    QList<int> graph_orientation;
    graph_orientation.clear();
    for (int i=0;i<ngraphs;i++)
    {
    orientation=0;//0 means: somewhere in the center (i.e. not on the border of the grid)
    gno = graphs[i];
    get_graph_viewport(gno,&v);
    if (v.xv1==v_outer.xv1) orientation|=1;//1=left
    if (v.xv2==v_outer.xv2) orientation|=2;//2=right
    if (v.yv1==v_outer.yv1) orientation|=4;//4=bottom
    if (v.yv2==v_outer.yv2) orientation|=8;//8=top
    graph_orientation << orientation;
    }

//qDebug() << "GraphOrientation:" << graph_orientation;

    //set the borders for packed sets
    for (int i=0;i<ngraphs;i++)
    {
        gno = graphs[i];
        orientation = graph_orientation.at(i);
        if (vpack)
        {
///First: X_AXIS
        t = get_graph_tickmarks(gno, X_AXIS);
//qDebug() << "X: Graph=" << gno << "Orientation=" << orientation;
//qDebug() << "packXInner=" << packXInner << "packXInnerDir=" << packXInnerDir;
//qDebug() << "packYInner=" << packYInner << "packYInnerDir=" << packYInnerDir;
        //qDebug() << "graph G" << i << "orientation=" << orientation << "packXOuter=" << packXOuter << "packYOuter=" << packYOuter;
///Inner Graph
        if ((orientation==0 || orientation==1 || orientation==2 || orientation==3) && packXInner!=TICK_PACK_UNCHANGED)
        {
            t->tl_flag=0;//tick labels off for all inner graphs
            if (packXInner==TICK_PACK_NONE)
            {
                t->t_flag=0;//ticks off
            }
            else
            {
                t->t_flag=1;//ticks on
                if ((packXInner&TICK_PACK_IN) && (packXInner&TICK_PACK_OUT))
                {
                    t->t_op = PLACEMENT_BOTH;
                }
                else if (packXInner!=TICK_PACK_UNCHANGED)
                {
                    if ((packXInner&TICK_PACK_DOWN) && !(packXInner&TICK_PACK_UP))
                    t->t_op = PLACEMENT_NORMAL;
                    else if (!(packXInner&TICK_PACK_DOWN) && (packXInner&TICK_PACK_UP))
                    t->t_op = PLACEMENT_OPPOSITE;
                    else
                    t->t_op = PLACEMENT_BOTH;
                }
                if (packXInnerDir!=TICK_PACK_DIRECTION_UNCHANGED)
                {
                    if ((packXInnerDir&TICK_PACK_DIRECTION_IN) && !(packXInnerDir&TICK_PACK_DIRECTION_OUT))
                    t->t_inout = TICKS_IN;
                    else if (!(packXInnerDir&TICK_PACK_DIRECTION_IN) && (packXInnerDir&TICK_PACK_DIRECTION_OUT))
                    t->t_inout = TICKS_OUT;
                    else
                    t->t_inout = TICKS_BOTH;
                }
            }
        }
///Outer Graph
        if ((orientation!=0 && orientation!=1 && orientation!=2 && orientation!=3) && packXOuter!=TICK_PACK_UNCHANGED)
        {
            //tick marks
            if (packXOuter==TICK_PACK_NONE)
            {
                t->t_flag=0;//ticks off
            }
            else
            {
                t->t_flag=1;//ticks on
                if ((packXOuter&TICK_PACK_IN) && (packXOuter&TICK_PACK_OUT))
                {
                    t->t_op = PLACEMENT_BOTH;
                }
                else if (!(packXOuter&TICK_PACK_IN) && !(packXOuter&TICK_PACK_OUT))
                {
                    if ((packXOuter&TICK_PACK_DOWN) && !(packXOuter&TICK_PACK_UP))
                    t->t_op = PLACEMENT_NORMAL;
                    else
                    t->t_op = PLACEMENT_OPPOSITE;
                }
                else
                {  //inwards or outwards --> special treatment because 'in' and 'out' mean differnt things depending on position
                    switch (orientation)
                    {//X-AXIS of OUTER GRAPH
                    default://'default' makes the compiler happy
                    case 1://just left (not on the bottom or the top) -- no in or out
                    case 2://just right (not on the bottom or the top) -- no in or out
                    case 3://left and right (not bottom or top) -- no in or out
                    case 12://bottom and top (not left or right) -- no in or out
                    case 13://bottom/top/left -- no in or out
                    case 14://bottom/top/right -- no in or out
                    case 15://only one graph -- no in or out
                        t->t_op = PLACEMENT_BOTH;
                    break;
                    case 4://just bottom (not on left or right border) -- opposite = Inside
                    case 5://left/bottom -- OuterGraph
                    case 6://right/bottom -- OuterGraph
                    case 7://left/right/bottom -- OuterGraph -- inside=opposite
                        if ((packXOuter&TICK_PACK_IN) && !(packXOuter&TICK_PACK_OUT))
                        t->t_op = PLACEMENT_OPPOSITE;
                        else if (!(packXOuter&TICK_PACK_IN) && (packXOuter&TICK_PACK_OUT))
                        t->t_op = PLACEMENT_NORMAL;
                    break;
                    case 8://just top (not on left or right border) -- normal = inside
                    case 9://left/top -- OuterGraph
                    case 10://right/top -- OuterGraph
                    case 11://left/right/top -- OuterGraph -- inside=normal
                        if ((packXOuter&TICK_PACK_IN) && !(packXOuter&TICK_PACK_OUT))
                        t->t_op = PLACEMENT_NORMAL;
                        else if (!(packXOuter&TICK_PACK_IN) && (packXOuter&TICK_PACK_OUT))
                        t->t_op = PLACEMENT_OPPOSITE;
                    break;
                    }
                }
                if (packXOuterDir!=TICK_PACK_DIRECTION_UNCHANGED)
                {
                    if ((packXOuterDir&TICK_PACK_DIRECTION_IN) && !(packXOuterDir&TICK_PACK_DIRECTION_OUT))
                    t->t_inout = TICKS_IN;
                    else if (!(packXOuterDir&TICK_PACK_DIRECTION_IN) && (packXOuterDir&TICK_PACK_DIRECTION_OUT))
                    t->t_inout = TICKS_OUT;
                    else
                    t->t_inout = TICKS_BOTH;
                }
            }
            //tick labels
            t->tl_op = PLACEMENT_NORMAL;
            switch (orientation)
            {//X-AXIS of OUTER GRAPH
            default://'default' makes the compiler happy
            case 12://bottom and top (not left or right) -- tick labels on
            case 13://bottom/top/left -- tick labels on
            case 14://bottom/top/right -- tick labels on
            case 15://only one graph -- tick labels on
                t->tl_flag = 1;
                //t->tl_op = PLACEMENT_NORMAL;
            break;
            case 4://just bottom (not on left or right border) -- tick labels only if tick marks on the bottom
            case 5://left/bottom -- tick labels only if tick marks on the bottom
            case 6://right/bottom -- tick labels only if tick marks on the bottom
            case 7://left/right/bottom -- tick labels only if tick marks on the bottom
                /*if (t->t_op == PLACEMENT_NORMAL || t->t_op == PLACEMENT_BOTH)
                {
                t->tl_flag=1;
                }
                else
                {
                t->tl_flag=0;
                }
                t->tl_op = PLACEMENT_NORMAL;*/
                t->tl_flag = 1;
            break;
            case 8://just top (not on left or right border) -- tick labels only if tick marks on the top
            case 9://left/top -- tick labels only if tick marks on the top
            case 10://right/top -- tick labels only if tick marks on the top
            case 11://left/right/top -- tick labels only if tick marks on the top
                /*if (t->t_op == PLACEMENT_OPPOSITE || t->t_op == PLACEMENT_BOTH)
                {
                t->tl_flag=1;
                }
                else
                {
                t->tl_flag=0;
                }
                t->tl_op = PLACEMENT_OPPOSITE;*/
                t->tl_flag = 0;
            break;
            }
        }//end outer

        }//end vpack
        if (hpack)
        {
///Second: Y_AXIS
        t = get_graph_tickmarks(gno, Y_AXIS);
//qDebug() << "Y: Graph=" << gno << "Orientation=" << orientation;
//qDebug() << "packYInner=" << packYInner << "packYInnerDir=" << packYInnerDir;
//qDebug() << "packYOuter=" << packYOuter << "packYOuterDir=" << packYOuterDir;
///Inner Graph
        if ((orientation==0 || orientation==8 || orientation==4 || orientation==12) && packYOuter!=TICK_PACK_UNCHANGED)
        {
            t->tl_flag=0;//tick-lables off for all inner graphs
            if (packYInner==TICK_PACK_NONE)
            {
                t->t_flag=0;//ticks off
            }
            else
            {
                t->t_flag=1;//ticks on
                if ((packYOuter&TICK_PACK_IN) && (packYOuter&TICK_PACK_OUT))
                {
                t->t_op = PLACEMENT_BOTH;
                }
                else if (packYInner!=TICK_PACK_UNCHANGED)
                {
                    if ((packYInner&TICK_PACK_LEFT) && !(packYInner&TICK_PACK_RIGHT))
                    t->t_op = PLACEMENT_NORMAL;
                    else if (!(packYInner&TICK_PACK_LEFT) && (packYInner&TICK_PACK_RIGHT))
                    t->t_op = PLACEMENT_OPPOSITE;
                    else
                    t->t_op = PLACEMENT_BOTH;
                }
                if (packYInnerDir!=TICK_PACK_DIRECTION_UNCHANGED)
                {
                    if ((packYInnerDir&TICK_PACK_DIRECTION_IN) && !(packYInnerDir&TICK_PACK_DIRECTION_OUT))
                    t->t_inout = TICKS_IN;
                    else if (!(packYInnerDir&TICK_PACK_DIRECTION_IN) && (packYInnerDir&TICK_PACK_DIRECTION_OUT))
                    t->t_inout = TICKS_OUT;
                    else
                    t->t_inout = TICKS_BOTH;
                }
            }
            //qDebug() << "Y-Inner" << t->t_flag;
        }
///Outer Graph
        if ((orientation!=0 && orientation!=4 && orientation!=8 && orientation!=12) && packYOuter!=TICK_PACK_UNCHANGED)
        {
            //tick-marks
            if (packYOuter==TICK_PACK_NONE)
            {
                t->t_flag=0;//ticks off
            }
            else
            {
                t->t_flag=1;//ticks on
                if ((packYOuter&TICK_PACK_IN) && (packYOuter&TICK_PACK_OUT))
                {
                    t->t_op = PLACEMENT_BOTH;
                }
                else if (!(packYOuter&TICK_PACK_IN) && !(packYOuter&TICK_PACK_OUT))
                {
                    if ((packYOuter&TICK_PACK_LEFT) && !(packYOuter&TICK_PACK_RIGHT))
                    t->t_op = PLACEMENT_NORMAL;
                    else
                    t->t_op = PLACEMENT_OPPOSITE;
                }
                else//IN or OUT
                {
                    switch (orientation)
                    {
                    case 1://just left (not on the bottom or the top) -- Inside=opposite
                    case 9://left/top
                    case 5://left/bottom
                    case 13://bottom/top/left
                        if ((packYOuter&TICK_PACK_IN) && !(packYOuter&TICK_PACK_OUT))
                        t->t_op = PLACEMENT_OPPOSITE;
                        else if (!(packYOuter&TICK_PACK_IN) && (packYOuter&TICK_PACK_OUT))
                        t->t_op = PLACEMENT_NORMAL;
                    break;
                    case 2://just right (not on the bottom or the top) -- Inside=normal
                    case 6://right/bottom
                    case 10://right/top
                    case 14://bottom/top/right
                        if ((packYOuter&TICK_PACK_IN) && !(packYOuter&TICK_PACK_OUT))
                        t->t_op = PLACEMENT_NORMAL;
                        else if (!(packYOuter&TICK_PACK_IN) && (packYOuter&TICK_PACK_OUT))
                        t->t_op = PLACEMENT_OPPOSITE;
                    break;
                    case 4://just bottom (not on left or right border)
                    case 8://just top (not on left or right border)
                    case 12://bottom and top (not border)
                    case 3://left and right (not bottom or top)
                    case 15://only one graph
                    default://'default' makes the compiler happy
                    case 11://left/right/top -- no in or out
                    case 7://left/right/bottom -- no in or out
                        t->t_op = PLACEMENT_BOTH;
                    break;
                    }
                }
                if (packYOuterDir!=TICK_PACK_DIRECTION_UNCHANGED)
                {
                    if ((packYOuterDir&TICK_PACK_DIRECTION_IN) && !(packYOuterDir&TICK_PACK_DIRECTION_OUT))
                    t->t_inout = TICKS_IN;
                    else if (!(packYOuterDir&TICK_PACK_DIRECTION_IN) && (packYOuterDir&TICK_PACK_DIRECTION_OUT))
                    t->t_inout = TICKS_OUT;
                    else
                    t->t_inout = TICKS_BOTH;
                }
            }
            //qDebug() << "Y-Outer" << t->t_flag;
            //tick-labels
            t->tl_op = PLACEMENT_NORMAL;
            switch (orientation)
            {//Y-AXIS of OUTER GRAPH
            case 1://just left (not on the bottom or the top) -- tick labels only if marks on left hand side
            case 9://left/top -- tick labels only if marks on left hand side
            case 5://left/bottom -- tick labels only if marks on left hand side
            case 13://bottom/top/left -- tick labels only if marks on left hand side
                /*if (t->t_op == PLACEMENT_NORMAL || t->t_op == PLACEMENT_BOTH)
                {
                t->tl_flag=1;
                }
                else
                {
                t->tl_flag=0;
                }
                t->tl_op = PLACEMENT_NORMAL;*/
                t->tl_flag = 1;
            break;
            case 2://just right (not on the bottom or the top) -- tick labels only if marks on right hand side
            case 6://right/bottom -- tick labels only if marks on right hand side
            case 10://right/top -- tick labels only if marks on right hand side
            case 14://bottom/top/right -- tick labels only if marks on right hand side
                /*if (t->t_op == PLACEMENT_OPPOSITE || t->t_op == PLACEMENT_BOTH)
                {
                t->tl_flag=1;
                }
                else
                {
                t->tl_flag=0;
                }
                t->tl_op = PLACEMENT_OPPOSITE;*/
                t->tl_flag = 0;
            break;
            case 3://left and right (not bottom or top)
            case 15://only one graph
            default://'default' makes the compiler happy
            case 11://left/right/top
            case 7://left/right/bottom
                t->tl_flag = 1;
                //t->tl_op = PLACEMENT_NORMAL;
            break;
            }

                }//end of outer part
            }//end hpack
        }//end all graphs-loop
    }//end if graphs packed
    else//graphs not packed
    {
    /// Change something? NO!
    }
    return RETURN_SUCCESS;
}

int arrange_graphs_simple(int nrows, int ncols,
    int order, int snake, double offset, double hgap, double vgap)
{
    int *graphs, i, ngraphs, retval;
    
    ngraphs = nrows*ncols;
    graphs = (int*)xmalloc(ngraphs*sizeof(int));///SIZEOF_INT);
    if (graphs == NULL) {
        return RETURN_FAILURE;
    }
    
    for (i = 0; i < ngraphs; i++) {
        graphs[i] = i;
    }
    
    for (i = number_of_graphs() - 1; i >= ngraphs; i--) {
        kill_graph(i);
    }
    
    retval = arrange_graphs(graphs, ngraphs, nrows, ncols, order, snake,
             offset, offset, offset, offset, vgap, hgap, FALSE, FALSE, FALSE, 0.0, 0.0, DEFAULT_TICK_PACK_X_INNER, DEFAULT_TICK_PACK_X_OUTER, DEFAULT_TICK_PACK_Y_INNER, DEFAULT_TICK_PACK_Y_OUTER,DEFAULT_TICK_PACK_X_INNER_DIR,DEFAULT_TICK_PACK_X_OUTER_DIR,DEFAULT_TICK_PACK_Y_INNER_DIR,DEFAULT_TICK_PACK_Y_OUTER_DIR);
    
    xfree(graphs);
    
    return retval;
}

void move_legend(int gno, VVector shift)
{
    double xtmp, ytmp;
    legend l;

    if (is_valid_gno(gno)) {
        get_graph_legend(gno, &l);
        if (l.loctype == COORD_VIEW) {
            l.legx += shift.x;
            l.legy += shift.y;
        } else {
            world2view(l.legx, l.legy, &xtmp, &ytmp);
            xtmp += shift.x;
            ytmp += shift.y;
            view2world(xtmp, ytmp, &l.legx, &l.legy);
        }
        set_graph_legend(gno, &l);
        set_dirtystate();
    }
}

void move_timestamp(VVector shift)
{
    timestamp.x += shift.x;
    timestamp.y += shift.y;
    set_dirtystate();
}

/*shifts an axis-label by a vector (and sets the axis-label-position to special)*/
void move_axis_label(int gno,int axis,VVector shift)
{
    if (is_valid_axis(gno, axis) != TRUE) return;
char dummy[MAX_STRING_LENGTH],descr_axis[32];
    //Undo-Stuff
    SaveTickmarksStatesPrevious(axis,axis,gno,gno);
    ListOfChanges.clear();
    ListOfOldStates.clear();
tickmarks * t = copy_graph_tickmarks(get_graph_tickmarks(gno,axis));
tickmarks * t2 = get_graph_tickmarks(gno,axis);//t2 is the original
view bb0;
get_graph_viewport(gno,&bb0);
/*
qDebug() << "Graph y=0: vp=" << bb0.yv1;
qDebug() << "View Label: " << t->label.bb.xv1 << "," << t->label.bb.yv1 << " | " << t->label.bb.xv2 << "," << t->label.bb.yv2;
*/
    sprintf(dummy,"with g%d",gno);
    ListOfChanges << QString(dummy);
    ListOfOldStates << QString(dummy);

    switch (axis)
    {
    case 0:
        strcpy(descr_axis,"xaxis");
        break;
    case 1:
        strcpy(descr_axis,"yaxis");
        break;
    case 2:
        strcpy(descr_axis,"altxaxis");
        break;
    case 3:
        strcpy(descr_axis,"altyaxis");
        break;
    }

    if (t->label_place==TYPE_AUTO)
    {
        if (is_xaxis(axis))
        {
            if (t->label_op==PLACEMENT_NORMAL || t->label_op==PLACEMENT_BOTH)
            {
            t->label.y=bb0.yv1-(t->label.bb.yv2+t->label.bb.yv1)*0.5;
            }
            else
            {
            t->label.y=(t->label.bb.yv2+t->label.bb.yv1)*0.5-bb0.yv2;
            }
        }
        else
        {
            if (t->label_op==PLACEMENT_NORMAL || t->label_op==PLACEMENT_BOTH)
            {
            t->label.y=bb0.xv1-(t->label.bb.xv2+t->label.bb.xv1)*0.5;
            }
            else
            {
            t->label.y=(t->label.bb.xv2+t->label.bb.xv1)*0.5-bb0.xv2;
            }
        }
    }

    if (is_xaxis(axis))
    {
        if (t->label_op==PLACEMENT_NORMAL || t->label_op==PLACEMENT_BOTH)
        {
        t->label.x+=shift.x;
        t->label.y-=shift.y;
        }
        else
        {
        t->label.x+=shift.x;
        t->label.y+=shift.y;
        }
    }
    else//y-axis
    {
        if (t->label_op==PLACEMENT_NORMAL || t->label_op==PLACEMENT_BOTH)
        {
        t->label.x+=shift.y;
        t->label.y-=shift.x;
        }
        else
        {
        t->label.x+=shift.y;
        t->label.y+=shift.x;
        }
    }

    /*qDebug() << descr_axis;
    qDebug() << "Original label: " << t2->label.x << " | " << t2->label.y;
    qDebug() << "Shifting axis label by " << shift.x << " | " << shift.y;*/

    t->label_place=TYPE_SPEC;

    sprintf(dummy,"    %s  label place %s",descr_axis,t->label_place==TYPE_SPEC?"spec":"auto");
    ListOfChanges << QString(dummy);
    sprintf(dummy,"    %s  label place %s",descr_axis,t2->label_place==TYPE_SPEC?"spec":"auto");
    ListOfOldStates << QString(dummy);
    if (t->label_place==TYPE_SPEC)
    {
        sprintf(dummy,"    %s  label place %f, %f",descr_axis,t->label.x,t->label.y);
        ListOfChanges << QString(dummy);
    }
    if (t2->label_place==TYPE_SPEC)
    {
        sprintf(dummy,"    %s  label place %f, %f",descr_axis,t2->label.x,t2->label.y);
        ListOfOldStates << QString(dummy);
    }

    set_graph_tickmarks(gno, axis, t);

    //Undo-Stuff
    TickmarksModified(axis,axis,gno,gno);

    QString description=QObject::tr("Axis label repositioned");

    sprintf(dummy,"[G%d.",gno);
    description+=QString(dummy);
        if (axis==0)
        description+=QObject::tr("X-axis");
        else if (axis==1)
        description+=QObject::tr("Y-axis");
        else if (axis==2)
        description+=QObject::tr("Alt-X-axis");
        else if (axis==3)
        description+=QObject::tr("Alt-Y-axis");
    description+=QString("]");

    addAditionalDescriptionToLastNode(-1,description,QString(),0);
    ListOfChanges.clear();
    ListOfOldStates.clear();
    set_dirtystate();
    free_graph_tickmarks(t);
force_redraw_wo_dialog_updates();
updateAxisProps(gno,axis);
}

/*moves a graph to a new viewport (the shift is defined by a vector) */
void move_graph_vp(int gno,VVector shift)
{
char dummy[MAX_STRING_LENGTH];
    if (!is_valid_gno(gno)) return;

    view v;
    get_graph_viewport(gno,&v);

    v.xv1+=shift.x;
    v.xv2+=shift.x;
    v.yv1+=shift.y;
    v.yv2+=shift.y;

    //Undo-Stuff
    ViewportChanged(gno,v);
    //the actual change
    set_graph_viewport(gno,v);

    QString description;
        description=QObject::tr("Graph moved");
        sprintf(dummy,"[G%d]",gno);
    description+=QString(dummy);
    addAditionalDescriptionToLastNode(-1,description,QString(),0);
set_dirtystate();
    ListOfChanges.clear();
    ListOfOldStates.clear();
force_redraw_wo_dialog_updates();
updateGraphAppearance(gno);
}

/*moves a title of a graph (title_subtitle=0=Title, title_subtitle=1=Subtitle)*/
void move_graph_title(int gno,int title_subtitle,VVector shift)
{
char dummy[MAX_STRING_LENGTH];
    if (!is_valid_gno(gno)) return;
//qDebug() << "Move Title gno=" << gno << " title_subtitle=" << title_subtitle;
    ///Undo-Stuff
    SaveSingleGraphState(gno,UNDO_APPEARANCE);
    ListOfChanges.clear();
    ListOfOldStates.clear();

    sprintf(dummy,"with g%d",gno);
    ListOfChanges << QString(dummy);
    ListOfOldStates << QString(dummy);

    //sprintf(dummy,"    view %f, %f, %f, %f",v.xv1,v.yv1,v.xv2,v.yv2);
    //ListOfOldStates << QString(dummy);

    if (title_subtitle==0)
    {
    g[gno].labs.shift_title.x+=shift.x;
    g[gno].labs.shift_title.y+=shift.y;
    }
    else
    {
    g[gno].labs.shift_subtitle.x+=shift.x;
    g[gno].labs.shift_subtitle.y+=shift.y;
    }

    //sprintf(dummy,"    view %f, %f, %f, %f",v.xv1,v.yv1,v.xv2,v.yv2);
    //ListOfChanges << QString(dummy);

    ///Undo-Stuff
    SingleGraphModified(gno,UNDO_APPEARANCE);
    QString description;
        if (title_subtitle==0)
        description=QObject::tr("Graph title moved");
        else
        description=QObject::tr("Graph subtitle moved");
        sprintf(dummy,"[G%d]",gno);
    description+=QString(dummy);
    addAditionalDescriptionToLastNode(-1,description,QString(),0);
    set_dirtystate();
    ListOfChanges.clear();
    ListOfOldStates.clear();
force_redraw_wo_dialog_updates();
updateGraphAppearance(gno);
}

/*increases or decreases font-size for axis-label by factor*/
void change_axis_label_size(int gno,int axis,double factor)
{
    if (is_valid_axis(gno, axis) != TRUE) return;

char dummy[MAX_STRING_LENGTH],descr_axis[32];

    //Undo-Stuff
    SaveTickmarksStatesPrevious(axis,axis,gno,gno);
    ListOfChanges.clear();
    ListOfOldStates.clear();

tickmarks * t = copy_graph_tickmarks(get_graph_tickmarks(gno,axis));
tickmarks * t2 = get_graph_tickmarks(gno,axis);//t2 is the original

    sprintf(dummy,"with g%d",gno);
    ListOfChanges << QString(dummy);
    ListOfOldStates << QString(dummy);

    switch (axis)
    {
    case 0:
        strcpy(descr_axis,"xaxis");
        break;
    case 1:
        strcpy(descr_axis,"yaxis");
        break;
    case 2:
        strcpy(descr_axis,"altxaxis");
        break;
    case 3:
        strcpy(descr_axis,"altyaxis");
        break;
    }

    t->label.charsize*=factor;

        sprintf(dummy,"    %s  label char size %f",descr_axis,t->label.charsize);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    %s  label char size %f",descr_axis,t2->label.charsize);
        ListOfOldStates << QString(dummy);

    set_graph_tickmarks(gno, axis, t);

    //Undo-Stuff
    TickmarksModified(axis,axis,gno,gno);

    QString description=QObject::tr("Size of axis label changed");

    sprintf(dummy,"[G%d.",gno);
    description+=QString(dummy);
        if (axis==0)
        description+=QObject::tr("X-axis");
        else if (axis==1)
        description+=QObject::tr("Y-axis");
        else if (axis==2)
        description+=QObject::tr("Alt-X-axis");
        else if (axis==3)
        description+=QObject::tr("Alt-Y-axis");
    description+=QString("]");

    addAditionalDescriptionToLastNode(-1,description,QString(),0);
    ListOfChanges.clear();
    ListOfOldStates.clear();
    set_dirtystate();
    free_graph_tickmarks(t);
force_redraw_wo_dialog_updates();
updateAxisProps(gno,axis);
}

/*increases or decreases font-size for axis-tick-labels by factor*/
void change_axis_tick_label_size(int gno,int axis,double factor)
{
    if (is_valid_axis(gno, axis) != TRUE) return;

char dummy[MAX_STRING_LENGTH],descr_axis[32];

    //Undo-Stuff
    SaveTickmarksStatesPrevious(axis,axis,gno,gno);
    ListOfChanges.clear();
    ListOfOldStates.clear();

tickmarks * t = copy_graph_tickmarks(get_graph_tickmarks(gno,axis));
tickmarks * t2 = get_graph_tickmarks(gno,axis);//t2 is the original

    sprintf(dummy,"with g%d",gno);
    ListOfChanges << QString(dummy);
    ListOfOldStates << QString(dummy);

    switch (axis)
    {
    case 0:
        strcpy(descr_axis,"xaxis");
        break;
    case 1:
        strcpy(descr_axis,"yaxis");
        break;
    case 2:
        strcpy(descr_axis,"altxaxis");
        break;
    case 3:
        strcpy(descr_axis,"altyaxis");
        break;
    }

    t->tl_charsize*=factor;

        sprintf(dummy,"    %s  ticklabel char size %f",descr_axis,t->tl_charsize);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    %s  ticklabel char size %f",descr_axis,t2->tl_charsize);
        ListOfOldStates << QString(dummy);

    set_graph_tickmarks(gno, axis, t);

    //Undo-Stuff
    TickmarksModified(axis,axis,gno,gno);

    QString description=QObject::tr("Size of axis tick labels changed");

    sprintf(dummy,"[G%d.",gno);
    description+=QString(dummy);
        if (axis==0)
        description+=QObject::tr("X-axis");
        else if (axis==1)
        description+=QObject::tr("Y-axis");
        else if (axis==2)
        description+=QObject::tr("Alt-X-axis");
        else if (axis==3)
        description+=QObject::tr("Alt-Y-axis");
    description+=QString("]");

    addAditionalDescriptionToLastNode(-1,description,QString(),0);
    ListOfChanges.clear();
    ListOfOldStates.clear();
    set_dirtystate();
    free_graph_tickmarks(t);
force_redraw_wo_dialog_updates();
updateAxisProps(gno,axis);
}

/*increases or decreases font-size for axis-label by factor*/
void change_title_size(int gno,int title_subtitle,double factor)
{
char dummy[MAX_STRING_LENGTH];
QString description;
    if (!is_valid_gno(gno)) return;

    ///Undo-Stuff
    SaveSingleGraphState(gno,UNDO_APPEARANCE);
    ListOfChanges.clear();
    ListOfOldStates.clear();

    sprintf(dummy,"with g%d",gno);
    ListOfChanges << QString(dummy);
    ListOfOldStates << QString(dummy);

    if (title_subtitle==0)
    {
        sprintf(dummy,"    title size %f",g[gno].labs.title.charsize);
        ListOfOldStates << QString(dummy);
    g[gno].labs.title.charsize*=factor;
        sprintf(dummy,"    title size %f",g[gno].labs.title.charsize);
        ListOfChanges << QString(dummy);
    }
    else
    {
        sprintf(dummy,"    subtitle size %f",g[gno].labs.stitle.charsize);
        ListOfChanges << QString(dummy);
    g[gno].labs.stitle.charsize*=factor;
        sprintf(dummy,"    subtitle size %f",g[gno].labs.stitle.charsize);
        ListOfOldStates << QString(dummy);
    }

    ///Undo-Stuff
    SingleGraphModified(gno,UNDO_APPEARANCE);
        if (title_subtitle==0)
        description=QObject::tr("Title size changed");
        else
        description=QObject::tr("Subtitle size changed");
        sprintf(dummy,"[G%d]",gno);
    description+=QString(dummy);
    addAditionalDescriptionToLastNode(-1,description,QString(),0);
    set_dirtystate();
    ListOfChanges.clear();
    ListOfOldStates.clear();
force_redraw_wo_dialog_updates();
updateGraphAppearance(gno);
}

/*increases or decreases font-size for the timestamp by factor*/
void change_timestamp_size(double factor)
{
char dummy[MAX_STRING_LENGTH];
QString description;
    ///Undo-Stuff
    SavePlotAppearance();
    ListOfChanges.clear();
    ListOfOldStates.clear();

    sprintf(dummy,"timestamp char size %f",timestamp.charsize);
    ListOfOldStates << QString(dummy);

timestamp.charsize*=factor;

    sprintf(dummy,"timestamp char size %f",timestamp.charsize);
    ListOfChanges << QString(dummy);

    ///Undo-Stuff
    PlotAppearanceModified();
        description=QObject::tr("Timestamp size changed");
    addAditionalDescriptionToLastNode(-1,description,QString(),0);
    set_dirtystate();
    ListOfChanges.clear();
    ListOfOldStates.clear();
force_redraw_wo_dialog_updates();
updatePlotAppearance();
}

/*increases or decreases font-size for the legend by factor*/
void change_legend_font_size(int gno,double factor)
{
char dummy[MAX_STRING_LENGTH];
QString description;
    if (!is_valid_gno(gno)) return;

    ///Undo-Stuff
    SaveSingleGraphState(gno,UNDO_APPEARANCE);
    ListOfChanges.clear();
    ListOfOldStates.clear();

    sprintf(dummy,"with g%d",gno);
    ListOfChanges << QString(dummy);
    ListOfOldStates << QString(dummy);

        sprintf(dummy,"    legend char size %f",g[gno].l.charsize);
        ListOfOldStates << QString(dummy);
    g[gno].l.charsize*=factor;
        sprintf(dummy,"    legend char size %f",g[gno].l.charsize);
        ListOfChanges << QString(dummy);

    ///Undo-Stuff
    SingleGraphModified(gno,UNDO_APPEARANCE);
        description=QObject::tr("Legend font size changed");
    sprintf(dummy,"[G%d]",gno);
    description+=QString(dummy);
    addAditionalDescriptionToLastNode(-1,description,QString(),0);
    set_dirtystate();
    ListOfChanges.clear();
    ListOfOldStates.clear();
//force_redraw();
force_redraw_wo_dialog_updates();
updateGraphAppearance(gno);
}

/*increases or decreases size of an object (font size for text)*/
void change_object_size(int type,int id,double factor)
{
char dummy[MAX_STRING_LENGTH];
double xmid,w;
QString description;

    ///Undo-Stuff
    SaveObjectData(id,type);
    ListOfChanges.clear();
    ListOfOldStates.clear();

    switch (type)
    {
    case OBJECT_BOX:
        description=QObject::tr("Box size changed");
        sprintf(dummy,"with box %d",id);
        ListOfChanges << QString(dummy);
        ListOfOldStates << QString(dummy);

        sprintf(dummy,"    box %f, %f, %f, %f",boxes[id].x1,boxes[id].y1,boxes[id].x2,boxes[id].y2);
        ListOfOldStates << QString(dummy);

        xmid=0.5*(boxes[id].x1+boxes[id].x2);
        w=(boxes[id].x2-boxes[id].x1)*factor*0.5;
        boxes[id].x1=xmid-w;
        boxes[id].x2=xmid+w;
        xmid=0.5*(boxes[id].y1+boxes[id].y2);
        w=(boxes[id].y2-boxes[id].y1)*factor*0.5;
        boxes[id].y1=xmid-w;
        boxes[id].y2=xmid+w;

        sprintf(dummy,"    box %f, %f, %f, %f",boxes[id].x1,boxes[id].y1,boxes[id].x2,boxes[id].y2);
        ListOfChanges << QString(dummy);
        ListOfChanges << QString("    box on");
        ListOfChanges << QString("box def");
        ListOfOldStates << QString("    box on");
        ListOfOldStates << QString("box def");
    break;
    case OBJECT_LINE:
        description=QObject::tr("Line size changed");
        sprintf(dummy,"with line %d",id);
        ListOfChanges << QString(dummy);
        ListOfOldStates << QString(dummy);

        sprintf(dummy,"    line %f, %f, %f, %f",lines[id].x1,lines[id].y1,lines[id].x2,lines[id].y2);
        ListOfOldStates << QString(dummy);

        xmid=0.5*(lines[id].x1+lines[id].x2);
        w=(lines[id].x2-lines[id].x1)*factor*0.5;
        lines[id].x1=xmid-w;
        lines[id].x2=xmid+w;
        xmid=0.5*(lines[id].y1+lines[id].y2);
        w=(lines[id].y2-lines[id].y1)*factor*0.5;
        lines[id].y1=xmid-w;
        lines[id].y2=xmid+w;

        sprintf(dummy,"    line %f, %f, %f, %f",lines[id].x1,lines[id].y1,lines[id].x2,lines[id].y2);
        ListOfChanges << QString(dummy);
        ListOfChanges << QString("    line on");
        ListOfChanges << QString("line def");
        ListOfOldStates << QString("    line on");
        ListOfOldStates << QString("line def");
    break;
    case OBJECT_ELLIPSE:
        description=QObject::tr("Ellipse size changed");
        sprintf(dummy,"with ellipse %d",id);
        ListOfChanges << QString(dummy);
        ListOfOldStates << QString(dummy);

        sprintf(dummy,"    ellipse %f, %f, %f, %f",ellip[id].x1,ellip[id].y1,ellip[id].x2,ellip[id].y2);
        ListOfOldStates << QString(dummy);

        xmid=0.5*(ellip[id].x1+ellip[id].x2);
        w=(ellip[id].x2-ellip[id].x1)*factor*0.5;
        ellip[id].x1=xmid-w;
        ellip[id].x2=xmid+w;
        xmid=0.5*(ellip[id].y1+ellip[id].y2);
        w=(ellip[id].y2-ellip[id].y1)*factor*0.5;
        ellip[id].y1=xmid-w;
        ellip[id].y2=xmid+w;

        sprintf(dummy,"    ellipse %f, %f, %f, %f",ellip[id].x1,ellip[id].y1,ellip[id].x2,ellip[id].y2);
        ListOfChanges << QString(dummy);
        ListOfChanges << QString("    ellipse on");
        ListOfChanges << QString("ellipse def");
        ListOfOldStates << QString("    ellipse on");
        ListOfOldStates << QString("ellipse def");
    break;
    case OBJECT_STRING:
        description=QObject::tr("Text-object size changed");
        sprintf(dummy,"with string %d",id);
        ListOfChanges << QString(dummy);
        ListOfOldStates << QString(dummy);

        sprintf(dummy,"    string char size %f",pstr[id].charsize);
        ListOfOldStates << QString(dummy);

        pstr[id].charsize*=factor;

        sprintf(dummy,"    string char size %f",pstr[id].charsize);
        ListOfChanges << QString(dummy);
        ListOfChanges << QString("    string on");
        ListOfOldStates << QString("    string on");
    break;
    }
    ///Undo-Stuff
    ObjectDataModified(id,type);
    addAditionalDescriptionToLastNode(-1,description,QString(),0);
    set_dirtystate();
    ListOfChanges.clear();
    ListOfOldStates.clear();
//force_redraw();
force_redraw_wo_dialog_updates();
updateObjectData(type,id);
}

void rescale_viewport(double ext_x, double ext_y)
{
    int i, gno;
    view v;
    legend leg;
    linetype l;
    boxtype b;
    ellipsetype e;
    plotstr s;
    
    for (gno = 0; gno < number_of_graphs(); gno++) {
        get_graph_viewport(gno, &v);
        v.xv1 *= ext_x;
        v.xv2 *= ext_x;
        v.yv1 *= ext_y;
        v.yv2 *= ext_y;
        set_graph_viewport(gno, v);
        
        get_graph_legend(gno, &leg);
        if (leg.loctype == COORD_VIEW) {
            leg.legx *= ext_x;
            leg.legy *= ext_y;
            set_graph_legend(gno, &leg);
        }
        
        /* TODO: tickmark offsets */
    }

    for (i = 0; i < number_of_lines(); i++) {
        get_graph_line(i, &l);
        if (l.loctype == COORD_VIEW) {
            l.x1 *= ext_x;
            l.x2 *= ext_x;
            l.y1 *= ext_y;
            l.y2 *= ext_y;
            set_graph_line(i, &l);
        }
    }
    for (i = 0; i < number_of_boxes(); i++) {
        get_graph_box(i, &b);
        if (b.loctype == COORD_VIEW) {
            b.x1 *= ext_x;
            b.x2 *= ext_x;
            b.y1 *= ext_y;
            b.y2 *= ext_y;
            set_graph_box(i, &b);
        }
    }
    for (i = 0; i < number_of_ellipses(); i++) {
        get_graph_ellipse(i, &e);
        if (e.loctype == COORD_VIEW) {
            e.x1 *= ext_x;
            e.x2 *= ext_x;
            e.y1 *= ext_y;
            e.y2 *= ext_y;
            set_graph_ellipse(i, &e);
        }
    }
    for (i = 0; i < number_of_strings(); i++) {
        get_graph_string(i, &s);
        if (s.loctype == COORD_VIEW) {
            s.x *= ext_x;
            s.y *= ext_y;
            set_graph_string(i, &s);
        }
    }
}

int overlay_graphs(int gsec, int gpri, int type)
{
    int i;
    tickmarks *tsec, *tpri;
    world wpri, wsec;
    view v;
    
    if (gsec == gpri) {
        return RETURN_FAILURE;
    }
    if (is_valid_gno(gpri) == FALSE || is_valid_gno(gsec) == FALSE) {
        return RETURN_FAILURE;
    }
    
    get_graph_viewport(gpri, &v);
    get_graph_world(gpri, &wpri);
    get_graph_world(gsec, &wsec);

    switch (type) {
    case GOVERLAY_SMART_AXES_XY:
        wsec = wpri;
	for (i = 0; i < MAXAXES; i++) {
	    tpri = get_graph_tickmarks(gpri, i);
	    tsec = get_graph_tickmarks(gsec, i);
            switch(i) {
            case X_AXIS:
            case Y_AXIS:
            tpri->active = TRUE;
	        tpri->label_op = PLACEMENT_NORMAL;
	        tpri->t_op = PLACEMENT_BOTH;
	        tpri->tl_op = PLACEMENT_NORMAL;

	        tsec->active = FALSE;
                break;
            default:
                /* don't touch alternative axes */
                break;
            }
	}
	break;
    case GOVERLAY_SMART_AXES_X:
        wsec.xg1 = wpri.xg1;
        wsec.xg2 = wpri.xg2;
	for (i = 0; i < MAXAXES; i++) {
	    tpri = get_graph_tickmarks(gpri, i);
	    tsec = get_graph_tickmarks(gsec, i);
	    switch(i) {
            case X_AXIS:
            tpri->active = TRUE;
	        tpri->label_op = PLACEMENT_NORMAL;
	        tpri->t_op = PLACEMENT_BOTH;
	        tpri->tl_op = PLACEMENT_NORMAL;

	        tsec->active = FALSE;
                break;
            case Y_AXIS:
	        tpri->active = TRUE;
	        tpri->label_op = PLACEMENT_NORMAL;
	        tpri->t_op = PLACEMENT_NORMAL;
	        tpri->tl_op = PLACEMENT_NORMAL;

            tsec->active = TRUE;
	        tsec->label_op = PLACEMENT_OPPOSITE;
	        tsec->t_op = PLACEMENT_OPPOSITE;
	        tsec->tl_op = PLACEMENT_OPPOSITE;
                break;
            default:
                /* don't touch alternative axes */
                break;
            }
	}
	break;
    case GOVERLAY_SMART_AXES_Y:
        wsec.yg1 = wpri.yg1;
        wsec.yg2 = wpri.yg2;
	for (i = 0; i < MAXAXES; i++) {
	    tpri = get_graph_tickmarks(gpri, i);
	    tsec = get_graph_tickmarks(gsec, i);
	    switch(i) {
            case X_AXIS:
	        tpri->active = TRUE;
	        tpri->label_op = PLACEMENT_NORMAL;
	        tpri->t_op = PLACEMENT_NORMAL;
	        tpri->tl_op = PLACEMENT_NORMAL;

            tsec->active = TRUE;
	        tsec->label_op = PLACEMENT_OPPOSITE;
	        tsec->t_op = PLACEMENT_OPPOSITE;
	        tsec->tl_op = PLACEMENT_OPPOSITE;
                break;
            case Y_AXIS:
            tpri->active = TRUE;
	        tpri->label_op = PLACEMENT_NORMAL;
	        tpri->t_op = PLACEMENT_BOTH;
	        tpri->tl_op = PLACEMENT_NORMAL;

	        tsec->active = FALSE;
                break;
            default:
                /* don't touch alternative axes */
                break;
            }
	}
	break;
    case GOVERLAY_SMART_AXES_NONE:
	for (i = 0; i < MAXAXES; i++) {
	    tpri = get_graph_tickmarks(gpri, i);
	    tsec = get_graph_tickmarks(gsec, i);
	    switch(i) {
            case X_AXIS:
            case Y_AXIS:
	        tpri->active = TRUE;
	        tpri->label_op = PLACEMENT_NORMAL;
	        tpri->t_op = PLACEMENT_NORMAL;
	        tpri->tl_op = PLACEMENT_NORMAL;

            tsec->active = TRUE;
	        tsec->label_op = PLACEMENT_OPPOSITE;
	        tsec->t_op = PLACEMENT_OPPOSITE;
	        tsec->tl_op = PLACEMENT_OPPOSITE;
                break;
            default:
                /* don't touch alternative axes */
                break;
            }
	}
	break;
    default:
        break;
    }
    
    /* set identical viewports */
    set_graph_viewport(gsec, v);
    
    /* update world coords */
    set_graph_world(gsec, wsec);

    return RETURN_SUCCESS;
}

//get the position (nvx,nvy) in viewport-coordinates needed to align the legendbox from the graph from_gno to the attachement-axis of graph to_gno
void position_leg_box(int from_gno,int to_gno,int attachement,double * nvx,double * nvy)
{
view tov;
view legbb;
legend gr_l;
get_graph_legend(from_gno, &gr_l);
legbb=gr_l.bb;
get_graph_viewport(to_gno,&tov);
*nvx=legbb.xv1;//initialize with current position
*nvy=legbb.yv1;
if (attachement == G_LB_ATTACH_NONE)//nothing to do here --> return the usual coordinates
{
return;
}
else
{
    //horizontal
    if (attachement & G_LB_ATTACH_LEFT)
    {
    *nvx=tov.xv1;
    }
    else if (attachement & G_LB_ATTACH_RIGHT)
    {
    *nvx=tov.xv2-fabs(legbb.xv2-legbb.xv1);
    }
    //vertical
    if (attachement & G_LB_ATTACH_TOP)
    {
    *nvy=tov.yv2;
    }
    else if (attachement & G_LB_ATTACH_BOTTOM)
    {
    *nvy=tov.yv1+fabs(legbb.yv2-legbb.yv1);
    }
}
}

int CheckWorldIntegrity(int gno)
{
double dx,dy;
int ret=RETURN_SUCCESS;
QString err_msg;
err_msg.clear();
    dx = g[gno].w.xg2 - g[gno].w.xg1;
    if (fabs(dx)<=DBL_EPSILON)
    {
    err_msg=QObject::tr("Scaling for X-axis too small, X-interval increased.");
    /*g[gno].w.xg2*=1+1e-15;
    g[gno].w.xg1*=1-1e-15;*/
    g[gno].w.xg2+=(DBL_EPSILON-fabs(dx))*0.5;
    g[gno].w.xg1-=(DBL_EPSILON-fabs(dx))*0.5;
    ret=RETURN_FAILURE;
    }
    else if (dx<0.0)
    {
    err_msg=QObject::tr("Scaling of X-axis inverted because Xmin>Xmax.");
    dx=g[gno].w.xg2;
    g[gno].w.xg2=g[gno].w.xg1;
    g[gno].w.xg1=dx;
    ret=RETURN_FAILURE;
    }
    dy = g[gno].w.yg2 - g[gno].w.yg1;
    if (fabs(dy)<=DBL_EPSILON)
    {
    //qDebug() << "fabs(dy)=" << fabs(dy) << " DBL_EPSILON=" << DBL_EPSILON;
        if (!err_msg.isEmpty()) err_msg+=QString("\n");
    err_msg+=QObject::tr("Scaling for Y-axis too small, Y-interval increased.");
    /*g[gno].w.yg2*=1+1e-15;
    g[gno].w.yg1*=1-1e-15;*/
    g[gno].w.yg2+=(DBL_EPSILON-fabs(dy))*0.5; ;
    g[gno].w.yg1-=(DBL_EPSILON-fabs(dy))*0.5; ;
    ret=RETURN_FAILURE;
    }
    else if (dy<0.0)
    {
        if (!err_msg.isEmpty()) err_msg+=QString("\n");
    err_msg+=QObject::tr("Scaling of Y-axis inverted because Ymin>Ymax.");
    dy=g[gno].w.yg2;
    g[gno].w.yg2=g[gno].w.yg1;
    g[gno].w.yg1=dy;
    ret=RETURN_FAILURE;
    }
/*char dummy_t[128];
sprintf(dummy_t,"wy= %.16g | %.16g eps=%.16g",g[gno].w.yg1,g[gno].w.yg2,DBL_EPSILON);
cout << dummy_t << endl;*/

if (ret==RETURN_FAILURE)
{
errmsg(err_msg.toLocal8Bit().constData());
}

return ret;
}

void zoomGraphWorld(int gno,world orig_world,WPoint wp,double factor)
{
world w;
w=orig_world;
double dx=fabs(fscale2(w.xg1,gno,X_AXIS)-fscale2(w.xg2,gno,X_AXIS));
double dy=fabs(fscale2(w.yg1,gno,Y_AXIS)-fscale2(w.yg2,gno,Y_AXIS));
/*qDebug() << "ZoomGraphWorld: orig_word=" << orig_world.xg1 << " | " << orig_world.yg1 << " - " << orig_world.xg2 << " | " << orig_world.yg2;
qDebug() << "wp=" << wp.x << " | " << wp.y;*/
w.xg1=ifscale2(wp.x-dx*0.5*factor,gno,X_AXIS);
w.xg2=ifscale2(wp.x+dx*0.5*factor,gno,X_AXIS);
w.yg1=ifscale2(wp.y-dy*0.5*factor,gno,Y_AXIS);
w.yg2=ifscale2(wp.y+dy*0.5*factor,gno,Y_AXIS);
if (w.xg2<w.xg1)
{
dx=w.xg1;
w.xg1=w.xg2;
w.xg2=dx;
}
if (w.yg2<w.yg1)
{
dy=w.yg1;
w.yg1=w.yg2;
w.yg2=dy;
}
set_graph_world(gno,w);
//qDebug() << "Scale=" << factor;
}

void add_failure(int & fail_value,int current_value)
{
//current_value=0: lower failed
//current_value=1: upper failed
    if (fail_value==-1)//no failure yet
    {
    fail_value=current_value;
    }
    else if (fail_value==2 || current_value==fail_value)//failure already registered
    {
    return;//everything ok
    }
    else//current_value!=fail_value --> the other one already failed--> everything failed
    {
    fail_value=2;
    }
}

void zoomGraphWorldKeepingFixedPoint(int gno, world orig_world, WPoint wp, double factor, int axis)
{
world w;
w=orig_world;
double x0=wp.x,x1=w.xg1,x2=w.xg2;
double y0=wp.y,y1=w.yg1,y2=w.yg2;
double t_x,t_y;
if (!is_valid_gno(gno)) return;
//transform coordinates according to scaling
for (int i=0;i<3;i++)
{
switch (g[gno].xscale)
{
default:
case SCALE_NORMAL:
    if (i==0)
    {
    t_x=x0;
    }
    else if (i==1)
    {
    t_x=x1;
    }
    else
    {
    t_x=x2;
    }
break;
case SCALE_LOG:
    if (i==0)
    {
    t_x=log10(x0);
    }
    else if (i==1)
    {
    t_x=log10(x1);
    }
    else
    {
    t_x=log10(x2);
    }
break;
case SCALE_REC:
    if (i==0)
    {
    t_x=1.0/x0;
    }
    else if (i==1)
    {
    t_x=1.0/x1;
    }
    else
    {
    t_x=1.0/x2;
    }
break;
case SCALE_LOGIT:
    if (i==0)
    {
    t_x=log(x0/(1.0-x0));
    }
    else if (i==1)
    {
    t_x=log(x1/(1.0-x1));
    }
    else
    {
    t_x=log(x2/(1.0-x2));
    }
break;
}
switch (g[gno].yscale)
{
default:
case SCALE_NORMAL:
    if (i==0)
    {
    t_y=y0;
    }
    else if (i==1)
    {
    t_y=y1;
    }
    else
    {
    t_y=y2;
    }
break;
case SCALE_LOG:
    if (i==0)
    {
    t_y=log10(y0);
    }
    else if (i==1)
    {
    t_y=log10(y1);
    }
    else
    {
    t_y=log10(y2);
    }
break;
case SCALE_REC:
    if (i==0)
    {
    t_y=1.0/y0;
    }
    else if (i==1)
    {
    t_y=1.0/y1;
    }
    else
    {
    t_y=1.0/y2;
    }
break;
case SCALE_LOGIT:
    if (i==0)
    {
    t_y=log(y0/(1.0-y0));
    }
    else if (i==1)
    {
    t_y=log(y1/(1.0-y1));
    }
    else
    {
    t_y=log(y2/(1.0-y2));
    }
break;
}
    if (i==0)
    {
    x0=t_x;
    y0=t_y;
    }
    else if (i==1)
    {
    x1=t_x;
    y1=t_y;
    }
    else
    {
    x2=t_x;
    y2=t_y;
    }
}
//now we do the zooming
if (get_graph_type(gno)!=GRAPH_POLAR2)
{
w.xg1=x0+(x1-x0)*factor;
w.xg2=x0+(x2-x0)*factor;
}
w.yg1=y0+(y1-y0)*factor;
w.yg2=y0+(y2-y0)*factor;
//transform back
switch (g[gno].xscale)
{
default:
case SCALE_NORMAL:
    ;//now back-transformation needed here
break;
case SCALE_LOG:
    w.xg1=pow(10.0,w.xg1);
    w.xg2=pow(10.0,w.xg2);
break;
case SCALE_REC:
    w.xg1=1.0/w.xg1;
    w.xg2=1.0/w.xg2;
break;
case SCALE_LOGIT:
    t_x=w.xg1;
    w.xg1=exp(t_x)/(1+exp(t_x));
    t_x=w.xg2;
    w.xg2=exp(t_x)/(1+exp(t_x));
break;
}
switch (g[gno].yscale)
{
default:
case SCALE_NORMAL:
    ;//no back-transformation needed here
break;
case SCALE_LOG:
    w.yg1=pow(10.0,w.yg1);
    w.yg2=pow(10.0,w.yg2);
break;
case SCALE_REC:
    w.yg1=1.0/w.yg1;
    w.yg2=1.0/w.yg2;
break;
case SCALE_LOGIT:
    t_y=w.yg1;
    w.yg1=exp(t_y)/(1+exp(t_y));
    t_y=w.yg2;
    w.yg2=exp(t_y)/(1+exp(t_y));
break;
}
//set new axis-limits
sprintf(dummy,"[G%d]",gno);
if (axis==X_AXIS)
{
w.yg1=orig_world.yg1;
w.yg2=orig_world.yg2;
strcat(dummy,QObject::tr(".X-Axis").toLocal8Bit().constData());
}
else if (axis==Y_AXIS)
{
w.xg1=orig_world.xg1;
w.xg2=orig_world.xg2;
strcat(dummy,QObject::tr(".Y-Axis").toLocal8Bit().constData());
}
bool ok=true;
int x_failed=-1,y_failed=-1;//-1: ok; 0: lower failed; 1: upper failed; 2: both failed
if (qIsNaN(w.xg1) || !qIsFinite(w.xg1))  {w.xg1=orig_world.xg1;ok=false;add_failure(x_failed,0);}
if (qIsNaN(w.yg1) || !qIsFinite(w.yg1))  {w.yg1=orig_world.yg1;ok=false;add_failure(y_failed,0);}
if (qIsNaN(w.xg2) || !qIsFinite(w.xg2))  {w.xg2=orig_world.xg2;ok=false;add_failure(x_failed,1);}
if (qIsNaN(w.yg2) || !qIsFinite(w.yg2))  {w.yg2=orig_world.yg2;ok=false;add_failure(y_failed,1);}
if (g[gno].xscale==SCALE_LOG && w.xg1<=0.0) {w.xg1=orig_world.xg1;ok=false;add_failure(x_failed,0);}
if (g[gno].xscale==SCALE_LOG && w.xg2<=0.0) {w.xg2=orig_world.xg2;ok=false;add_failure(x_failed,1);}
if (g[gno].yscale==SCALE_LOG && w.yg1<=0.0) {w.yg1=orig_world.yg1;ok=false;add_failure(y_failed,0);}
if (g[gno].yscale==SCALE_LOG && w.yg2<=0.0) {w.yg2=orig_world.yg2;ok=false;add_failure(y_failed,1);}
if (g[gno].xscale==SCALE_LOGIT && (w.xg1<=0.0 || w.xg1>=1.0)) {w.xg1=orig_world.xg1;ok=false;add_failure(x_failed,0);}
if (g[gno].xscale==SCALE_LOGIT && (w.xg2<=0.0 || w.xg2>=1.0)) {w.xg2=orig_world.xg2;ok=false;add_failure(x_failed,1);}
if (g[gno].yscale==SCALE_LOGIT && (w.yg1<=0.0 || w.yg1>=1.0)) {w.yg1=orig_world.yg1;ok=false;add_failure(y_failed,0);}
if (g[gno].yscale==SCALE_LOGIT && (w.yg2<=0.0 || w.yg2>=1.0)) {w.yg2=orig_world.yg2;ok=false;add_failure(y_failed,1);}
if (g[gno].xscale==SCALE_REC && w.xg1<=0.0 && w.xg2>=0.0) {w.xg1=orig_world.xg1;w.xg2=orig_world.xg2;ok=false;add_failure(x_failed,0);add_failure(x_failed,1);}
if (g[gno].xscale==SCALE_REC && w.xg2<=0.0 && w.xg1>=0.0) {w.xg1=orig_world.xg1;w.xg2=orig_world.xg2;ok=false;add_failure(x_failed,0);add_failure(x_failed,1);}
if (g[gno].yscale==SCALE_REC && w.yg1<=0.0 && w.yg2>=0.0) {w.yg1=orig_world.yg1;w.yg2=orig_world.yg2;ok=false;add_failure(y_failed,0);add_failure(y_failed,1);}
if (g[gno].yscale==SCALE_REC && w.yg2<=0.0 && w.yg1>=0.0) {w.yg1=orig_world.yg1;w.yg2=orig_world.yg2;ok=false;add_failure(y_failed,0);add_failure(y_failed,1);}
    if (ok==false)
    {
    if (x_failed==0)
    errmsg(QObject::tr("Zooming failed on X-Axis. Lower X-Axis-limit unchanged.").toLocal8Bit().constData());
    else if (x_failed==1)
    errmsg(QObject::tr("Zooming failed on X-Axis. Upper X-Axis-limit unchanged.").toLocal8Bit().constData());
    else if (x_failed==2)
    errmsg(QObject::tr("Zooming failed on X-Axis. Both X-Axis-limits unchanged.").toLocal8Bit().constData());
        if (y_failed==0)
        errmsg(QObject::tr("Zooming failed on Y-Axis. Lower Y-Axis-limit unchanged.").toLocal8Bit().constData());
        else if (y_failed==1)
        errmsg(QObject::tr("Zooming failed on Y-Axis. Upper Y-Axis-limit unchanged.").toLocal8Bit().constData());
        else if (y_failed==2)
        errmsg(QObject::tr("Zooming failed on Y-Axis. Both Y-Axis-limits unchanged.").toLocal8Bit().constData());
    }
SaveSingleGraphTickmarks(gno);
set_graph_world(gno,w);
SingleGraphTickmarksModified(gno);
addAditionalDescriptionToLastNode(-1,QObject::tr("Mouse Wheel Zooming")+QString(dummy),QString(),0);
}

int find_nearest_graph(VPoint vp)
{
int found=-1;
int nr_of_graphs=number_of_graphs();
    if (nr_of_graphs<1) return found;
    else if (nr_of_graphs==1) return 0;
double * x_dist=new double[nr_of_graphs+2];
double * y_dist=new double[nr_of_graphs+2];
view gr_view;
    for (int i=0;i<nr_of_graphs;i++)
    {
        if (get_graph_viewport(i,&gr_view)==RETURN_FAILURE)
        {
        x_dist[i]=y_dist[i]=99999.9;
        continue;
        }
        if (is_vpoint_inside(gr_view, vp, MAXPICKDIST) == TRUE)
        {
        found=i;
        break;
        }
    x_dist[i]=fabs(vp.x-gr_view.xv1)<fabs(vp.x-gr_view.xv2)?fabs(vp.x-gr_view.xv1):fabs(vp.x-gr_view.xv2);
    y_dist[i]=fabs(vp.y-gr_view.yv1)<fabs(vp.y-gr_view.yv2)?fabs(vp.y-gr_view.yv1):fabs(vp.y-gr_view.yv2);
    }
if (found<0)
{
double min=x_dist[0]*x_dist[0]+y_dist[0]*y_dist[0];
double abst;
int index=0;
    for (int i=1;i<nr_of_graphs;i++)
    {
    abst=x_dist[i]*x_dist[i]+y_dist[i]*y_dist[i];
        if (abst<min)
        {
        min=abst;
        index=i;
        }
    }
found=index;
}
delete[] x_dist;
delete[] y_dist;
return found;
}

void quickSetAxisFormat(int gno,int axis,int quickformat)
{
    if (!is_valid_axis(gno,axis)) return;
    switch (quickformat)
    {
    case QUICK_AXIS_TRANSFORM_NORMAL:
        g[gno].t[axis]->tl_formula=copy_string(g[gno].t[axis]->tl_formula,"");
        strcpy(g[gno].t[axis]->tl_appstr,"");
        strcpy(g[gno].t[axis]->orig_tl_appstr,"");
        autotick_axis(gno,axis);
        break;
    case QUICK_AXIS_TRANSFORM_DEGREES:
        g[gno].t[axis]->tl_formula=copy_string(g[gno].t[axis]->tl_formula,"$t*180.0/PI");
        strcpy(g[gno].t[axis]->tl_appstr,"\\c0");//"\\c:"('\\c:' looks strange in some fonts)
        strcpy(g[gno].t[axis]->orig_tl_appstr,"\\c0");//"\\c:"
        g[gno].t[axis]->tmajor=M_PI*0.5;
        break;
    case QUICK_AXIS_TRANSFORM_MULT_PI:
        g[gno].t[axis]->tl_formula=copy_string(g[gno].t[axis]->tl_formula,"$t/PI");
        strcpy(g[gno].t[axis]->tl_appstr,"\\xp");//"\\c7\\C\\xp"
        strcpy(g[gno].t[axis]->orig_tl_appstr,"\\xp");
        g[gno].t[axis]->tmajor=M_PI*0.5;
        break;
    case QUICK_AXIS_TRANSFORM_ALT_AXIS:
        if (FormCreateAltAxis==NULL)
        {
        FormCreateAltAxis=new frmCreateAltAxis(0);
        FormCreateAltAxis->setModal(true);
        }
        FormCreateAltAxis->init(gno,axis);
        FormCreateAltAxis->show();
        FormCreateAltAxis->raise();
        FormCreateAltAxis->activateWindow();
        break;
    default:
        break;
    }
    updateAxisProps(gno,axis);
}

void createAltAxis(int gno,int axis)///INCOMPLETE
{

}

