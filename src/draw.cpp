/*
 * Grace - GRaphing, Advanced Computation and Exploration of data
 * 
 * Home page: http://plasma-gate.weizmann.ac.il/Grace/
 * 
 * Copyright (c) 1991-1995 Paul J Turner, Portland, OR
 * Copyright (c) 1996-2005 Grace Development Team
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
 * Interface to device drivers
 *
 */

#include <iostream>
 
///#include <config.h>
#include <cmath>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "defines.h"
#include "globals.h"
#include "device.h"
#include "graphs.h"
#include "utils.h"
#include "draw.h"
#include <QtGui>
#include "cmath.h"
#include "parser.h"
#include "noxprotos.h"

using namespace std;

extern QPainter * GeneralPainter;

extern QList<int> highlight_clipping;
extern QList<int> highlight_set;
extern QList< QVector<QPointF> > collect_segments;
extern QPointF VPoint2XPointF(VPoint vp);

int ReqUpdateColorSel = FALSE;  /* a part of pre-GUI layer; should be in
                                   a separate module */

int AAGrayLevelsOK = FALSE;     /* used to invalidate AA gray levels' cache */

/* device exit */
void (*devleavegraphics) (void);
/* device pixel routine */
void (*devdrawpixel) (VPoint vp);  
/* device polyline routine */
void (*devdrawpolyline) (VPoint *vps, int n, int mode);  
/* device polygon routine */
void (*devfillpolygon) (VPoint *vps, int nc);   
/* device arc routine */
void (*devdrawarc) (VPoint vp1, VPoint vp2, int a1, int a2);	
/* device fill arc routine */
void (*devfillarc) (VPoint vp1, VPoint vp2, int a1, int a2, int mode);	
/* device pixmap drawing */
void (*devputpixmap) (VPoint vp, int width, int height, char *databits,int pixmap_bpp, int bitmap_pad, int pixmap_type);
/* device text typesetting */
void (*devputtext) (VPoint vp, char *s, int len, int font,TextMatrix *tm, int underline, int overline, int kerning);
/* update color map */
void (*devupdatecmap)(void);
/* x- and y-axis-conversation*/
double (*xy_xconv) (double wx);
double (*xy_yconv) (double wy);

static int all_points_inside(VPoint *vps, int n);
static void purge_dense_points(const VPoint *vps, int n, VPoint *pvps, int *np);

/* Current drawing properties */
extern DrawProps draw_props;
//={{1, 1}, 0, 255, TRUE, 1, 0.0, LINECAP_BUTT, LINEJOIN_MITER, 1.0, 0, FILLRULE_WINDING};

static world worldwin;
static view viewport;
static int coordinates;
int scaletypex;
int scaletypey;
static long double xv_med;
static long double yv_med;
static long double xv_rc;
static long double yv_rc;
long double polar2_f,phi0,rhomin,roffset,rmax;//scale-factor(world to vp),phi0,roffset(minimal rho in vp),rmax(maximal rho in vp)
static long double fxg_med;
static long double fyg_med;

static int clipflag = TRUE;        /* whether clipping must be in force */
static int revflag = FALSE;

/*
 * set pen properties
 */
void setpen(Pen pen)
{
    draw_props.pen = pen;
    return;
}

Pen getpen(void)
{
    return (draw_props.pen);
}

/*
 * make the current color color
 */
void setcolor(int color)
{
    draw_props.pen.color = color;
}

int getcolor(void)
{
    return (draw_props.pen.color);
}

/*
 * alpha-channel
 */
void setalpha(int alpha)
{
    if (alpha>255 || alpha<0)
    draw_props.pen.alpha = 255;
    else
    draw_props.pen.alpha = alpha;
}

int getalpha(void)
{
    if (draw_props.pen.alpha>255 || draw_props.pen.alpha<0 || show_transparency_selector==0)
    return 255;
    else
    return (draw_props.pen.alpha);
}

/*
 * background alpha-channel
 */
void setbgalpha(int alpha)
{
    if (alpha>255 || alpha<0)
    draw_props.bgalpha = 255;
    else
    draw_props.bgalpha = alpha;
}

int getbgalpha(void)
{
    if (draw_props.bgalpha>255 || draw_props.bgalpha<0)
    return 255;
    else
    return (draw_props.bgalpha);
}

/*
 * set the background color of the canvas
 */
void setbgcolor(int bgcolor)
{
    draw_props.bgcolor = bgcolor;
}

int getbgcolor(void)
{
    return (draw_props.bgcolor);
}

/*
 * determines whether page background is filled
 */
void setbgfill(int flag)
{
    draw_props.bgfilled = flag;
}

int getbgfill(void)
{
    return (draw_props.bgfilled);
}

/*
 * make the current linestyle lines
 */
void setlinestyle(int lines)
{
    draw_props.lines = lines;
    return;
}

int getlinestyle(void)
{
    return (draw_props.lines);
}

/*
 * make the current line width linew
 */
void setlinewidth(double linew)
{
    draw_props.linew = linew;
    return;
}

double getlinewidth(void)
{
    return ((double) MAGIC_LINEW_SCALE*draw_props.linew);
}

void setpattern(int pattern)
{
    draw_props.pen.pattern = pattern;
    return;
}

int getpattern(void)
{
    return (draw_props.pen.pattern);
}

/*
 * set the current character size to size
 */
void setcharsize(double charsize)
{
    draw_props.charsize = charsize*universal_font_size_factor;
    return;
}

double getcharsize(void)
{
    return (draw_props.charsize);
}

/*
 * setfont - make font the current font to use for writing strings
 */
void setfont(int font)
{
    draw_props.font = font;
    return;
}

int getfont(void)
{
    return (draw_props.font);
}

/*
 * set the current fillrule
 */
void setfillrule(int rule)
{
    draw_props.fillrule = rule;
    return;
}

int getfillrule(void)
{
    return (draw_props.fillrule);
}

/*
 * set/get the current linecap parameter
 */
void setlinecap(int type)
{
    draw_props.linecap = type;
    return;
}

int getlinecap(void)
{
    return (draw_props.linecap);
}

/*
 * set/get the current linejoin type
 */
void setlinejoin(int type)
{
    draw_props.linejoin = type;
    return;
}

int getlinejoin(void)
{
    return (draw_props.linejoin);
}

/*
 * Convert point's world coordinates to viewport
 */
VPoint Wpoint2Vpoint(WPoint wp)
{
static VPoint vp;
    world2view(wp.x, wp.y, &vp.x, &vp.y);
return (vp);
}

/*
 * Convert point's viewport to world coordinates
 */
WPoint Vpoint2Wpoint(VPoint wp)
{
static WPoint vp;
    view2world(wp.x, wp.y, &vp.x, &vp.y);
return (vp);
}

void symplus(VPoint vp, double s)
{
    VPoint vp1, vp2;
    vp1.x = vp.x - s;
    vp1.y = vp.y;
    vp2.x = vp.x + s;
    vp2.y = vp.y;
    DrawLine(vp1, vp2);

    vp1.x = vp.x;
    vp1.y = vp.y - s;
    vp2.x = vp.x;
    vp2.y = vp.y + s;
    DrawLine(vp1, vp2);
}

void symx(VPoint vp, double s)
{
    VPoint vp1, vp2;
    double side = M_SQRT1_2*s;
    
    vp1.x = vp.x - side;
    vp1.y = vp.y - side;
    vp2.x = vp.x + side;
    vp2.y = vp.y + side;
    DrawLine(vp1, vp2);
    
    vp1.x = vp.x - side;
    vp1.y = vp.y + side;
    vp2.x = vp.x + side;
    vp2.y = vp.y - side;
    DrawLine(vp1, vp2);
}

void symsplat(VPoint vp, double s)
{
    symplus(vp, s);
    symx(vp, s);
}

void leavegraphics(void)
{
/*    devsymreset(); */
    (*devleavegraphics) ();
}

/*
 * DrawPixel - put a pixel in the current color at position vp
 */
void DrawPixel(VPoint vp)
{
     if (is_validVPoint(vp)) {
         if (get_draw_mode() == TRUE) {
            (*devdrawpixel)(vp);
         }
         update_bboxes(vp);
     }
}

/*
 * DrawRect - draw a rectangle using the current color and linestyle
 */
void DrawRect(VPoint vp1, VPoint vp2)
{
static VPoint vps[4];
/*static double x_centre,y_centre;
static double sa,ca;
if (RotationAngle>0)
{
    sa=sin(RotationAngle*1.74532925199e-2);
    ca=cos(RotationAngle*1.74532925199e-2);
    x_centre=(vp1.x+vp2.x)*0.5;
    y_centre=(vp1.y+vp2.y)*0.5;

    vps[0].x = x_centre+((vp1.x-x_centre)*ca-(vp1.y-y_centre)*sa);
    vps[1].x = x_centre+((vp2.x-x_centre)*ca-(vp1.y-y_centre)*sa);
    vps[2].x = x_centre+((vp2.x-x_centre)*ca-(vp2.y-y_centre)*sa);
    vps[3].x = x_centre+((vp1.x-x_centre)*ca-(vp2.y-y_centre)*sa);

    vps[0].y = y_centre+((vp1.x-x_centre)*sa+(vp1.y-y_centre)*ca);
    vps[1].y = y_centre+((vp2.x-x_centre)*sa+(vp1.y-y_centre)*ca);
    vps[2].y = y_centre+((vp2.x-x_centre)*sa+(vp2.y-y_centre)*ca);
    vps[3].y = y_centre+((vp1.x-x_centre)*sa+(vp2.y-y_centre)*ca);
}
else
{*/
    vps[0].x = vp1.x;
    vps[0].y = vp1.y;
    vps[1].x = vp1.x;
    vps[1].y = vp2.y;
    vps[2].x = vp2.x;
    vps[2].y = vp2.y;
    vps[3].x = vp2.x;
    vps[3].y = vp1.y;
//}
    DrawPolyline(vps, 4, POLYLINE_CLOSED);
}

/*
 * DrawRect - draw a rectangle using the current color and linestyle
 */
void FillRect_(VPoint vp1, VPoint vp2)
{
static VPoint vps[4];
/*static double x_centre,y_centre;
static double sa,ca;
if (RotationAngle>0)
{
    sa=sin(RotationAngle*1.74532925199e-2);
    ca=cos(RotationAngle*1.74532925199e-2);
    x_centre=(vp1.x+vp2.x)*0.5;
    y_centre=(vp1.y+vp2.y)*0.5;

    vps[0].x = x_centre+((vp1.x-x_centre)*ca-(vp1.y-y_centre)*sa);
    vps[1].x = x_centre+((vp2.x-x_centre)*ca-(vp1.y-y_centre)*sa);
    vps[2].x = x_centre+((vp2.x-x_centre)*ca-(vp2.y-y_centre)*sa);
    vps[3].x = x_centre+((vp1.x-x_centre)*ca-(vp2.y-y_centre)*sa);

    vps[0].y = y_centre+((vp1.x-x_centre)*sa+(vp1.y-y_centre)*ca);
    vps[1].y = y_centre+((vp2.x-x_centre)*sa+(vp1.y-y_centre)*ca);
    vps[2].y = y_centre+((vp2.x-x_centre)*sa+(vp2.y-y_centre)*ca);
    vps[3].y = y_centre+((vp1.x-x_centre)*sa+(vp2.y-y_centre)*ca);
}
else
{*/
    vps[0].x = vp1.x;
    vps[0].y = vp1.y;
    vps[1].x = vp1.x;
    vps[1].y = vp2.y;
    vps[2].x = vp2.x;
    vps[2].y = vp2.y;
    vps[3].x = vp2.x;
    vps[3].y = vp1.y;
//}
    DrawPolygon(vps, 4);
}


/*
 * DrawPolyline - draw a connected line in the current color and linestyle
 *            with nodes given by vps[]
 */
void DrawPolyline(VPoint *vps, int n, int mode)
{
static int i, nmax, nc, max_purge, npurged;
static VPoint vp1, vp2;
static VPoint vp1c, vp2c;
static VPoint *vpsc;
    
    if (collect_active)
    {
    QVector<QPointF> tmp_vec;
    tmp_vec.clear();
    for (i=0;i<n;i++)
    {
    tmp_vec << VPoint2XPointF(vps[i]);
    }
    if (mode == POLYLINE_CLOSED) tmp_vec << tmp_vec.first();
    collect_segments << tmp_vec;
    highlight_clipping << active_graph;
    highlight_set << active_set;
    }

    if (getlinestyle() == 0 || (getpen()).pattern == 0) {
        return;
    }
    
    if (n <= 1) {
        return;
    }
    
    if (mode == POLYLINE_CLOSED) {
        nmax = n + 1;
    } else {
        nmax = n;
    }
    
    max_purge = get_max_path_limit();
    
/*
 *  in most real cases, all points of a set are inside the viewport;
 *  so we check it prior to going into complicated clipping mode
 */
    if (doclipping() && !all_points_inside(vps, n)) {
        
        vpsc = (VPoint *)xmalloc((nmax)*sizeof(VPoint));
        if (vpsc == NULL) {
            errmsg ("xmalloc() failed in DrawPolyline()");
            return;
        }
        
        nc = 0;
        for (i = 0; i < nmax - 1; i++) {
            vp1 = vps[i];
            if (i < n - 1) {
                vp2 = vps[i + 1];
            } else {
                vp2 = vps[0];
            }
            if (clip_line(vp1, vp2, &vp1c, &vp2c)) {
                if (nc == 0) {
                    vpsc[nc] = vp1c;
                    nc++;
                }
                vpsc[nc] = vp2c;
                nc++;
                
                if (vp2.x != vp2c.x || vp2.y != vp2c.y || i == nmax - 2) {
                    update_bboxes_with_vpoints(vpsc, nc, getlinewidth());
                    
                    if (get_draw_mode() == TRUE) {
                        if (nc != nmax) {
                            mode = POLYLINE_OPEN;
                        }
                        if (max_purge && nc > max_purge) {
                            npurged = max_purge;
                            purge_dense_points(vpsc, nc, vpsc, &npurged);
                        } else {
                            npurged = nc;
                        }
                       (*devdrawpolyline)(vpsc, npurged, mode);
                    }
                    
                    nc = 0;
                }
            }
        }
        xfree(vpsc);
    } else {
        update_bboxes_with_vpoints(vps, n, getlinewidth());

        if (get_draw_mode() == TRUE) {
            if (max_purge && n > max_purge) {
                npurged = max_purge;
                vpsc = (VPoint *)xmalloc(max_purge*sizeof(VPoint));
                if (vpsc == NULL) {
                    errmsg ("xmalloc() failed in DrawPolyline()");
                    return;
                }
                purge_dense_points(vps, n, vpsc, &npurged);
                (*devdrawpolyline)(vpsc, npurged, mode);
                xfree(vpsc);
            } else {
                (*devdrawpolyline)(vps, n, mode);
            }
        }
    }
}

/*
 * DrawLine - draw a straight line in the current color and linestyle
 *            with nodes given by vp1 and vp2
 */
void DrawLine(VPoint vp1, VPoint vp2)
{
    VPoint vps[2];
    
    vps[0] = vp1;
    vps[1] = vp2;
    
    DrawPolyline(vps, 2, POLYLINE_OPEN);
}

/*
 * DrawPolygon - draw a filled polygon in the current color and pattern
 *      with nodes given by vps[]
 */
void DrawPolygon(VPoint *vps, int n)
{
    int nc, max_purge, npurged;
    VPoint *vptmp;

    if ((getpen()).pattern == 0) {
        return;
    }
    if (n < 3) {
        return;
    }

    max_purge = get_max_path_limit();
    
    if (doclipping() && !all_points_inside(vps, n)) {
        /* In the worst case, the clipped polygon may have twice more vertices */
        vptmp = (VPoint *)xmalloc((2*n) * sizeof(VPoint));
        if (vptmp == NULL) {
            errmsg("xmalloc() failed in DrawPolygon");
            return;
        } else {
            memcpy(vptmp, vps, n * sizeof(VPoint));
            nc = clip_polygon(vptmp, n);
            if (nc > 2) {
                update_bboxes_with_vpoints(vptmp, nc, 0.0);
                
                if (get_draw_mode() == TRUE) {
                    if (max_purge && nc > max_purge) {
                        npurged = max_purge;
                        purge_dense_points(vptmp, nc, vptmp, &npurged);
                    } else {
                        npurged = nc;
                    }
                    (*devfillpolygon) (vptmp, npurged);
                }
            }
            xfree(vptmp);
        }
    } else {
        update_bboxes_with_vpoints(vps, n, 0.0);

        if (get_draw_mode() == TRUE) {
            if (max_purge && n > max_purge) {
                npurged = max_purge;
                vptmp = (VPoint *)xmalloc(max_purge*sizeof(VPoint));
                if (vptmp == NULL) {
                    errmsg ("xmalloc() failed in DrawPolygon()");
                    return;
                }
                purge_dense_points(vps, n, vptmp, &npurged);
                (*devfillpolygon) (vptmp, npurged);
                xfree(vptmp);
            } else {
                (*devfillpolygon) (vps, n);
            }
        }
    }
}

/*
 * DrawArc - draw an arc line 
 */
void DrawArc(VPoint vp1, VPoint vp2, int angle1, int angle2)
{
    view v;
    v.xv1=v.xv2=v.yv1=v.yv2=0.0;

    if (getlinestyle() == 0 || (getpen()).pattern == 0)
    {
        return;
    }
    
    if (angle1 > angle2)
    {
        iswap(&angle1, &angle2);
    }
    
    /* TODO: clipping!!!*/
    if (get_draw_mode() == TRUE)
    {
        (*devdrawarc)(vp1, vp2, angle1, angle2);
    }
    
    /* TODO: consider open arcs! */
    VPoints2bbox(&vp1, &vp2, &v);
    view_extend(&v, getlinewidth()/2);
    update_bboxes_with_view(&v);
}

/*
 * DrawFilledArc - draw a filled arc 
 */
void DrawFilledArc(VPoint vp1, VPoint vp2, int angle1, int angle2, int mode)
{
    if ((getpen()).pattern == 0)
    {
        return;
    }

    if (points_overlap(vp1, vp2))
    {
        DrawPixel(vp1);
        return;
    }

    if (angle1 > angle2)
    {
        iswap(&angle1, &angle2);
    }
            
    /* TODO: clipping!!!*/
    if (get_draw_mode() == TRUE)
    {
        (*devfillarc)(vp1, vp2, angle1, angle2, mode);
    }
    /* TODO: consider open arcs! */
    update_bboxes(vp1);
    update_bboxes(vp2);
}

/*
 * DrawEllipse - draw an ellipse
 */
void DrawEllipse(VPoint vp1, VPoint vp2)
{
    DrawArc(vp1, vp2, 0, 360);
}

/*
 * DrawFilledEllipse - draw a filled ellipse
 */
void DrawFilledEllipse(VPoint vp1, VPoint vp2)
{
    DrawFilledArc(vp1, vp2, 0, 360, ARCFILL_CHORD);
}

/*
 * DrawCircle - draw a circle
 */
void DrawCircle(VPoint vp, double radius)
{
    VPoint vp1, vp2;
    
    vp1.x = vp.x - radius;
    vp1.y = vp.y - radius;
    vp2.x = vp.x + radius;
    vp2.y = vp.y + radius;
    
    DrawArc(vp1, vp2, 0, 360);
}

/*
 * DrawFilledCircle - draw a filled circle
 */
void DrawFilledCircle(VPoint vp, double radius)
{
    VPoint vp1, vp2;
    
    vp1.x = vp.x - radius;
    vp1.y = vp.y - radius;
    vp2.x = vp.x + radius;
    vp2.y = vp.y + radius;
    
    DrawFilledArc(vp1, vp2, 0, 360, ARCFILL_CHORD);
}


/* 
 * ------------------ Clipping routines ---------------
 */

/*
 * clip if clipflag = TRUE
 */
void setclipping(int flag)
{
    clipflag = flag ? TRUE:FALSE;
}

/*
 * 
 */
int doclipping(void)
{
    return(clipflag ? TRUE:FALSE);
}

/*
 * is_wpoint_inside() checks if point qp is inside of world rectangle w
 */
int is_wpoint_inside(WPoint *wp, world *w)
{
    return ((wp->x >= w->xg1) && (wp->x <= w->xg2) &&
            (wp->y >= w->yg1) && (wp->y <= w->yg2));
}

/* some to avoid round errors due to the finite FP precision */
#define VP_EPSILON  0.0001

/*
 * is_vpoint_inside() checks if point vp is inside of viewport rectangle v
 */
int is_vpoint_inside(view v, VPoint vp, double epsilon)
{
    return ((vp.x >= v.xv1 - epsilon) && (vp.x <= v.xv2 + epsilon) &&
            (vp.y >= v.yv1 - epsilon) && (vp.y <= v.yv2 + epsilon));
}

int is_vpoint_truly_inside(view v, VPoint vp)
{
double x_epsilon=fabs(v.xv2-v.xv1)*0.05;
double y_epsilon=fabs(v.yv2-v.yv1)*0.05;
    return ((vp.x >= v.xv1 + x_epsilon) && (vp.x <= v.xv2 - x_epsilon) &&
            (vp.y >= v.yv1 + y_epsilon) && (vp.y <= v.yv2 - y_epsilon));
}

static int all_points_inside(VPoint *vps, int n)
{
    int i;
    
    for (i = 0; i < n; i++) {
        if (is_vpoint_inside(viewport, vps[i], VP_EPSILON) != TRUE) {
            return FALSE;
        }
    }
    return TRUE;
}

/*
 * is_validVPoint() checks if a point is inside of (current) graph viewport
 */
int is_validVPoint(VPoint vp)
{
    if (doclipping()) {
        return (is_vpoint_inside(viewport, vp, VP_EPSILON));
    } else {
        return TRUE;
    }
}

/*
 * is_inside_angle_range() checks if an angle a is inside two other angles (start-stop); please note, that multiples of 2Pi is allowed (i.e. the angle 0 is considered to be inside [6-7])
 */
int is_inside_angle_range(double a,double start,double stop)
{
static double twopi=2.0*M_PI;
double nnn;
    if (a>=start && a<=stop)
    {
    return TRUE;//the most simple case!
    }
    else if (a<start)
    {
    nnn=twopi*ceil((start-a)/twopi)+a;
    if (nnn>=start && nnn<=stop) return TRUE;
    }
    else//a>stop
    {
    nnn=a-twopi*ceil((a-stop)/twopi);
    if (nnn>=start && nnn<=stop) return TRUE;
    }
return FALSE;
}

/*
 * is_validWPoint() checks if a point is inside of (current) world rectangle
 */
int is_validWPoint(WPoint wp)
{
    if (coordinates == COORDINATES_POLAR) {
        if (wp.y >= 0.0 && wp.y <= worldwin.yg2) {
            return TRUE;
        } else {
            return FALSE;
        }
    } else if (coordinates == COORDINATES_POLAR2) {
        if (wp.y >= worldwin.yg1 && wp.y <= worldwin.yg2 && is_inside_angle_range(wp.x,worldwin.xg1,worldwin.xg2)) {
            return TRUE;
        } else {
            return FALSE;
        }
    } else {
        if (((wp.x >= worldwin.xg1 && wp.x <= worldwin.xg2) ||
             (wp.x >= worldwin.xg2 && wp.x <= worldwin.xg1)) &&
            ((wp.y >= worldwin.yg1 && wp.y <= worldwin.yg2) ||
             (wp.y >= worldwin.yg2 && wp.y <= worldwin.yg1))) {
            return TRUE;
        } else {
            return FALSE;
        }
    }
}

#define LINE_FINITE     0
#define LINE_INFINITE   1

/* TODO: implement fpcomp() */
#define FPCMP_EPS      1.0e-6
/*
 * line_intersect() returns pointer to the intersection point of two
 * lines defined by points vp1, vp2 and vp1p, vp2p respectively. 
 * If the lines don't intersect, return NULL.
 * If mode == LINE_INFINTE, the second line is assumed to be infinite.
 * Note!! If the lines have more than single intersection point (parallel
 * partially coinsiding lines), the function returns NULL, too.
 * The routine uses the Liang-Barsky algorithm, slightly modified for the
 * sake of generality (but for the price of performance) 
 */
VPoint *line_intersect(VPoint vp1, VPoint vp2, VPoint vp1p, VPoint vp2p, int mode) {
    static VPoint vpbuf;
    long double vprod, t, tp;
    
    vprod = ((long double) vp2p.x - (long double) vp1p.x)*((long double) vp2.y - (long double) vp1.y) -
            ((long double) vp2.x - (long double) vp1.x)*((long double) vp2p.y - (long double) vp1p.y);
    if (vprod == 0) {
        return NULL;
    } else {
        t = (((long double) vp1.x - (long double) vp1p.x)*((long double) vp2p.y) +
             ((long double) vp2p.x - (long double) vp1.x)*((long double) vp1p.y) -
             ((long double) vp2p.x - (long double) vp1p.x)*((long double) vp1.y))/vprod;
        if ((t >= 0.0 - FPCMP_EPS) && (t <= 1.0 + FPCMP_EPS)) {
            vpbuf.x = (double)((long double) vp1.x + t*((long double) vp2.x - (long double) vp1.x));
            vpbuf.y = (double)((long double) vp1.y + t*((long double) vp2.y - (long double) vp1.y));
            
            if (mode == LINE_INFINITE) {
                return &vpbuf;
            } else {
                if (vp1p.x != vp2p.x) {
                    tp = ((long double) vpbuf.x - (long double) vp1p.x)/((long double) vp2p.x - (long double) vp1p.x);
                } else {
                    tp = ((long double) vpbuf.y - (long double) vp1p.y)/((long double) vp2p.y - (long double) vp1p.y);
                }
                
                if ((tp >= 0.0 - FPCMP_EPS) && (tp <= 1.0 + FPCMP_EPS)) {
                    return &vpbuf;
                } else {
                    return NULL;
                }
            }
        } else {
            return NULL;
        }
    }
}

/*
 * clip_line() clips a straight line defined by points vp1 and vp2
 * onto viewport rectangle; endpoints of the clipped line are returned by
 * vp1c and vp2c, and the function itself returns TRUE if (a part of) the line
 * should be drawn and FALSE otherwise
 */
int clip_line(VPoint vp1, VPoint vp2, VPoint *vp1c, VPoint *vp2c)
{
    int ends_found = 0;
    int na;
    int vp1_ok = FALSE, vp2_ok = FALSE;
    VPoint *vpp, vptmp[2], vpsa[5];
    
    if (is_validVPoint(vp1)) {
        vp1_ok = TRUE;
        ends_found++;
    }
    
    if (is_validVPoint(vp2)) {
        vp2_ok = TRUE;
        ends_found++;
    }
    
    if (vp1_ok && vp2_ok) {
        *vp1c = vp1;
        *vp2c = vp2;
        return (TRUE);
    } else {
        vpsa[0].x = viewport.xv1 - VP_EPSILON;
        vpsa[0].y = viewport.yv1 - VP_EPSILON;
        vpsa[1].x = viewport.xv2 + VP_EPSILON;
        vpsa[1].y = viewport.yv1 - VP_EPSILON;
        vpsa[2].x = viewport.xv2 + VP_EPSILON;
        vpsa[2].y = viewport.yv2 + VP_EPSILON;
        vpsa[3].x = viewport.xv1 - VP_EPSILON;
        vpsa[3].y = viewport.yv2 + VP_EPSILON;
        vpsa[4] = vpsa[0];
        
        na = 0;
        while ((ends_found < 2) && na < 4) {
            if ((vpp = line_intersect(vp1, vp2, vpsa[na], vpsa[na + 1], LINE_FINITE)) != NULL) {
                vptmp[ends_found] = *vpp;
                ends_found++;
            }
            na++;
        }
        if (ends_found == 0) {
            return (FALSE);
        } else if (ends_found == 2) {
            if (vp1_ok) {
                *vp1c = vp1;
                *vp2c = vptmp[1];
            } else if (vp2_ok) {
                *vp1c = vptmp[1];
                *vp2c = vp2;
            } else {
                *vp1c = vptmp[0];
                *vp2c = vptmp[1];
            }
            return (TRUE);
        } else if (ends_found == 1) {
            /* one of the points was on a frame edge exactly, but 
             * line_intersect(), due to a final FP precision, didn't 
             * find it
             */
            return (FALSE);
        } else {
            /* this would be really strange! */
            errmsg("Internal error in clip_line()");
            return (FALSE);
        }
    }
}

static int is_inside_boundary(VPoint vp, VPoint vp1c, VPoint vp2c)
{
    /* vector product should be positive if vp1c, vp2c and vp lie 
     * counter-clockwise
     */
    if (((long double) vp2c.x - (long double) vp1c.x)*((long double) vp.y - (long double) vp2c.y) - ((long double) vp.x - (long double) vp2c.x)*((long double) vp2c.y - (long double) vp1c.y) >= 0.0){
        return TRUE;
    } else {
        return FALSE;
    }
}

/* size of buffer array used in polygon clipping */
static int polybuf_length;

int intersect_polygon(VPoint *vps, int n, VPoint vp1p, VPoint vp2p)
{
    int i, nc, ishift;
    VPoint vp1, vp2, *vpp;
    
    nc = 0;
    ishift = polybuf_length - n;
    
    memmove(vps + ishift, vps, n * sizeof(VPoint));
    
    vp1 = vps[polybuf_length - 1];
    for (i = ishift; i < polybuf_length; i++) {
        vp2 = vps[i];
        if (is_inside_boundary(vp2, vp1p, vp2p)) {
            if (is_inside_boundary(vp1, vp1p, vp2p)) {
                vps[nc] = vp2;
                nc++;
            } else {
                vpp = line_intersect(vp1, vp2, vp1p, vp2p, LINE_INFINITE);
                if (vpp != NULL) {
                    vps[nc] = *vpp;
                    nc++;
                }
                vps[nc] = vp2;
                nc++;
            }
        } else if (is_inside_boundary(vp1, vp1p, vp2p)) {
            vpp = line_intersect(vp1, vp2, vp1p, vp2p, LINE_INFINITE);
            if (vpp != NULL) {
                vps[nc] = *vpp;
                nc++;
            }
        }
        vp1 = vp2;
    }
    
    return nc;
}

int clip_polygon(VPoint *vps, int n)
{
    int nc, na;
    VPoint vpsa[5];
    
    polybuf_length = 2*n;
    
    vpsa[0].x = viewport.xv1;
    vpsa[0].y = viewport.yv1;
    vpsa[1].x = viewport.xv2;
    vpsa[1].y = viewport.yv1;
    vpsa[2].x = viewport.xv2;
    vpsa[2].y = viewport.yv2;
    vpsa[3].x = viewport.xv1;
    vpsa[3].y = viewport.yv2;
    vpsa[4] = vpsa[0];
    
    nc = n;
    for (na = 0; na < 4; na++) {
        nc = intersect_polygon(vps, nc, vpsa[na], vpsa[na + 1]);
        if (nc < 2) {
            break;
        }
    }
    
    return nc;
}

/* 
 * ------------------ Colormap routines ---------------
 */
extern CMap_entry *cmap_table;
static unsigned int maxcolors = 0;

unsigned int number_of_colors(void)
{
    return maxcolors;
}

int get_main_color_indices(int ** maincolors,int * nr_of_aux_cols)
{
int counter=0;
if (*maincolors)
delete[] *maincolors;
*maincolors=new int[maxcolors];
for (unsigned int i=0;i<maxcolors;i++)
{
    if (cmap_table[i].ctype==COLOR_MAIN)
    {
    (*maincolors)[counter++]=i;
    }
}
*nr_of_aux_cols=maxcolors-counter;
return counter;
}

int is_valid_color(RGB rgb)
{
    if (((rgb.red   <= 0xff) && (rgb.red   >= 0x00)) &&
        ((rgb.green <= 0xff) && (rgb.green >= 0x00)) &&
        ((rgb.blue  <= 0xff) && (rgb.blue  >= 0x00))) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int compare_rgb(RGB *rgb1, RGB *rgb2)
{
    if ((rgb1->red   == rgb2->red)   &&
        (rgb1->green == rgb2->green) &&
        (rgb1->blue  == rgb2->blue)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int find_color(RGB rgb)
{
    unsigned int i;
    int cindex = BAD_COLOR;
    
    for (i = 0; i < maxcolors; i++) {
        if (compare_rgb(&cmap_table[i].rgb, &rgb) == TRUE) {
            cindex = i;
            break;
        }
    }
    
    return (cindex);
}

int get_color_by_name(char *cname)
{
    unsigned int i;
    int cindex = BAD_COLOR;
    
    for (i = 0; i < maxcolors; i++) {
        if (cmap_table[i].ctype == COLOR_MAIN &&
            compare_strings(cmap_table[i].cname, cname) == TRUE) {
            cindex = (int)i;
            break;
        }
    }
    
    return (cindex);
}

int realloc_colors(int n)
{
    unsigned int i;
    CMap_entry *cmap_tmp;
    //cout << "reallocate=" << n << endl;
    if (n > MAXCOLORS) {
        return RETURN_FAILURE;
    } else {
        for (i = n; i < maxcolors; i++) {
            XCFREE(cmap_table[i].cname);
        }
        cmap_tmp = (CMap_entry *)xrealloc(cmap_table, n*sizeof(CMap_entry));
        if (cmap_tmp == NULL) {
            return RETURN_FAILURE;
        } else {
            cmap_table = cmap_tmp;
            for (i = maxcolors; i < (unsigned int)n; i++) {
                cmap_table[i].rgb.red = 0;
                cmap_table[i].rgb.green = 0;
                cmap_table[i].rgb.blue = 0;
                cmap_table[i].cname = NULL;
                cmap_table[i].ctype = COLOR_NONE;
                cmap_table[i].tstamp = 0;
            }
        }
        maxcolors = n;
    }
    
    return RETURN_SUCCESS;
}

int store_color(int n, CMap_entry cmap)
{
    if (is_valid_color(cmap.rgb) != TRUE) {
        return RETURN_FAILURE;
    } else if ((unsigned int)n >= maxcolors && realloc_colors(n + 1) == RETURN_FAILURE) {
        return RETURN_FAILURE;
    } else {
        if (cmap.cname == NULL || strlen(cmap.cname) == 0) {
            cmap_table[n].cname =
                copy_string(cmap_table[n].cname, "unnamed");
        } else {
            cmap_table[n].cname =
                copy_string(cmap_table[n].cname, cmap.cname);
        }
        cmap_table[n].rgb = cmap.rgb;
        cmap_table[n].ctype = cmap.ctype;
        cmap_table[n].tstamp = 1;
                
        /* invalidate AA gray levels' cache */
        AAGrayLevelsOK = FALSE;
        
        /* inform current device of changes in the cmap database */
        if (devupdatecmap != NULL) {
///            (*devupdatecmap)();
        }
        if (cmap.ctype == COLOR_MAIN) {
            ReqUpdateColorSel = TRUE;
        }
        return RETURN_SUCCESS;
    }
}

/*
 * add_color() adds a new entry to the colormap table
 */
int add_color(CMap_entry cmap)
{
    int cindex;
    
    if (is_valid_color(cmap.rgb) != TRUE) {
        cindex = BAD_COLOR;
    } else if ((cindex = find_color(cmap.rgb)) != BAD_COLOR) {
        if (cmap.ctype == COLOR_MAIN && 
            cmap_table[cindex].ctype != COLOR_MAIN) {
            cmap_table[cindex].ctype = COLOR_MAIN;
            ReqUpdateColorSel = TRUE;
        }
    } else if (store_color(maxcolors, cmap) == RETURN_FAILURE) {
        cindex = BAD_COLOR;
    } else {
        cindex = maxcolors - 1;
    }
    
    return (cindex);
}

int delete_color(int cindex)
{
    if (cindex<0 || (unsigned int)cindex>=maxcolors) return RETURN_FAILURE;
    CMap_entry * saved_map=new CMap_entry[maxcolors+2];
    memcpy(saved_map,cmap_table,sizeof(CMap_entry)*maxcolors);
        for (unsigned int i=0;i<maxcolors;i++)
        {
            /*saved_map[i].cname=new char[2+strlen(cmap_table[i].cname)];
            strcpy(saved_map[i].cname,cmap_table[i].cname);*/
            saved_map[i].cname = NULL;
            saved_map[i].cname = copy_string(saved_map[i].cname,cmap_table[i].cname);
        }
    realloc_colors(maxcolors-1);
//int counter=0;
    for (unsigned int i=0;i<=maxcolors;i++)
    {
        if (i==(unsigned int)cindex) continue;
        memcpy(cmap_table+i,saved_map+i,sizeof(CMap_entry));
        cmap_table[i].cname = copy_string(cmap_table[i].cname,saved_map[i].cname);
        //delete[] saved_map[i].cname;
        saved_map[i].cname = copy_string(saved_map[i].cname,NULL);
    }
    //delete[] saved_map[cindex].cname;
    saved_map[cindex].cname = copy_string(saved_map[cindex].cname,NULL);
    delete[] saved_map;
    return RETURN_SUCCESS;
}

RGB *get_rgb(unsigned int cindex)
{
    if (cindex < maxcolors) {
        return &(cmap_table[cindex].rgb);
    } else {
        return NULL;
    }
}

fRGB *get_frgb(unsigned int cindex)
{
    static fRGB frgb;
    
    if (cindex < maxcolors) {
        frgb.red   = (double) cmap_table[cindex].rgb.red   / (MAXCOLORS - 1);
        frgb.green = (double) cmap_table[cindex].rgb.green / (MAXCOLORS - 1);
        frgb.blue  = (double) cmap_table[cindex].rgb.blue  / (MAXCOLORS - 1);
        return &frgb;
    } else {
        return NULL;
    }
}

fRGB *get_fsrgb(unsigned int cindex)
{
    static fRGB fsrgb;
    
    if (cindex < maxcolors) {
        fsrgb = *get_frgb(cindex);
        if (fsrgb.red <= 0.0031308) {
            fsrgb.red *= 12.92;
        } else {
            fsrgb.red = 1.055*pow(fsrgb.red,1.0/2.4)-0.055;
        }
        if (fsrgb.green <= 0.0031308) {
            fsrgb.green *= 12.92;
        } else {
            fsrgb.green = 1.055*pow(fsrgb.green,1.0/2.4)-0.055;
        }
        if (fsrgb.blue <= 0.0031308) {
            fsrgb.blue *= 12.92;
        } else {
            fsrgb.blue = 1.055*pow(fsrgb.blue,1.0/2.4)-0.055;
        }
        return &fsrgb;
    } else {
        return NULL;
    }
}

RGB *get_srgb(unsigned int cindex)
{
    static fRGB fsrgb;
    static RGB srgb;
    
    if (cindex < maxcolors) {
        fsrgb = *get_fsrgb(cindex);
        srgb.red   = round(fsrgb.red   * (MAXCOLORS - 1));
        srgb.green = round(fsrgb.green * (MAXCOLORS - 1));
        srgb.blue  = round(fsrgb.blue  * (MAXCOLORS - 1));
        return &srgb;
    } else {
        return NULL;
    }
}

CMap_entry *get_cmap_entry(unsigned int cindex)
{
    if (cindex < maxcolors) {
        return &(cmap_table[cindex]);
    } else {
        return NULL;
    }
}

char *get_colorname(unsigned int cindex)
{
    if (cindex < maxcolors) {
        return (cmap_table[cindex].cname);
    } else {
        return NULL;
    }
}

int get_colortype(unsigned int cindex)
{
    if (cindex < maxcolors) {
        return (cmap_table[cindex].ctype);
    } else {
        return (BAD_COLOR);
    }
}

YIQ RGB2YIQ(RGB rgb)
{
    YIQ yiq;
    
    if (is_valid_color(rgb)) {
        yiq.y = (0.299*rgb.red + 0.587*rgb.green + 0.114*rgb.blue)
                                                            /(MAXCOLORS - 1);
        yiq.i = (0.596*rgb.red - 0.275*rgb.green - 0.321*rgb.blue)
                                                            /(MAXCOLORS - 1);
        yiq.q = (0.212*rgb.red - 0.528*rgb.green + 0.311*rgb.blue)
                                                             /(MAXCOLORS - 1);
    } else {
        yiq.y = 0.0;
        yiq.i = 0.0;
        yiq.q = 0.0;
    }
    
    return (yiq);
}

double get_colorintensity(int cindex)
{
    double retval;
    
    if ((unsigned int)cindex < maxcolors) {
        retval = RGB2YIQ(cmap_table[cindex].rgb).y;
    } else {
        retval = 0.0;
    }
    
    return (retval);
}

CMap_entry cmap_init[16] = {
    /* white  */
    {{255, 255, 255}, NULL, COLOR_MAIN, 0},
    /* black  */
    {{0, 0, 0}, NULL, COLOR_MAIN, 0},
    /* red    */
    {{255, 0, 0}, NULL, COLOR_MAIN, 0},
    /* green  */
    {{0, 255, 0}, NULL, COLOR_MAIN, 0},
    /* blue   */
    {{0, 0, 255}, NULL, COLOR_MAIN, 0},
    /* yellow */
    {{255, 255, 0}, NULL, COLOR_MAIN, 0},
    /* brown  */
    {{188, 143, 143}, NULL, COLOR_MAIN, 0},
    /* grey   */
    {{220, 220, 220}, NULL, COLOR_MAIN, 0},
    /* violet */
    {{148, 0, 211}, NULL, COLOR_MAIN, 0},
    /* cyan   */
    {{0, 255, 255}, NULL, COLOR_MAIN, 0},
    /* magenta*/
    {{255, 0, 255}, NULL, COLOR_MAIN, 0},
    /* orange */
    {{255, 165, 0}, NULL, COLOR_MAIN, 0},
    /* indigo */
    {{114, 33, 188}, NULL, COLOR_MAIN, 0},
    /* maroon */
    {{103, 7, 72}, NULL, COLOR_MAIN, 0},
    /* turquoise */
    {{64, 224, 208}, NULL, COLOR_MAIN, 0},
    /* forest green */
    {{0, 139, 0}, NULL, COLOR_MAIN, 0}
};

char str_cmap_col_names[16][16];

/*
 * initialize_cmap()
 *    Initialize the colormap segment data and setup the RGB values.
 */
void initialize_cmap(void)
{
int i, n;
strcpy(str_cmap_col_names[0],QString("white").toLatin1().constData());
strcpy(str_cmap_col_names[1],QString("black").toLatin1().constData());
strcpy(str_cmap_col_names[2],QString("red").toLatin1().constData());
strcpy(str_cmap_col_names[3],QString("green").toLatin1().constData());
strcpy(str_cmap_col_names[4],QString("blue").toLatin1().constData());
strcpy(str_cmap_col_names[5],QString("yellow").toLatin1().constData());
strcpy(str_cmap_col_names[6],QString("brown").toLatin1().constData());
strcpy(str_cmap_col_names[7],QString("grey").toLatin1().constData());
strcpy(str_cmap_col_names[8],QString("violet").toLatin1().constData());
strcpy(str_cmap_col_names[9],QString("cyan").toLatin1().constData());
strcpy(str_cmap_col_names[10],QString("magenta").toLatin1().constData());
strcpy(str_cmap_col_names[11],QString("orange").toLatin1().constData());
strcpy(str_cmap_col_names[12],QString("indigo").toLatin1().constData());
strcpy(str_cmap_col_names[13],QString("maroon").toLatin1().constData());
strcpy(str_cmap_col_names[14],QString("turquoise").toLatin1().constData());
strcpy(str_cmap_col_names[15],QString("green4").toLatin1().constData());
for (int i=0;i<16;i++) cmap_init[i].cname=copy_string(cmap_init[i].cname,str_cmap_col_names[i]);
    n = sizeof(cmap_init)/sizeof(CMap_entry);
    realloc_colors(n);
    for (i = 0; i < n; i++) {
        store_color(i, cmap_init[i]);
    }
}

void reverse_video(void)
{
    /*
    CMap_entry ctmp;
    memcpy(&ctmp, &cmap_table[0], sizeof(CMap_entry));
    memcpy(&cmap_table[0], &cmap_table[1], sizeof(CMap_entry));
    memcpy(&cmap_table[1], &ctmp, sizeof(CMap_entry));
    revflag = !revflag;
    */
    for (unsigned int i=0;i<number_of_colors();i++)
    {
    cmap_table[i].rgb.blue=255-cmap_table[i].rgb.blue;
    cmap_table[i].rgb.red=255-cmap_table[i].rgb.red;
    cmap_table[i].rgb.green=255-cmap_table[i].rgb.green;
    }
}

int is_video_reversed(void)
{
    return revflag;
}

/* 
 * ------------------ Pattern routines ---------------
 */

int number_of_patterns(void)
{
    return MAXPATTERNS;
}

/* 
 * ------------------ Line style routines ---------------
 */

int number_of_linestyles(void)
{
    return MAXLINESTYLES;
}

/*
 * ------------- coordinate conversion routines ------------
 */

char *scale_types(int it)
{
    static char s[16];

    switch (it) {
    case SCALE_NORMAL:
	strcpy(s, "Normal");
	break;
    case SCALE_LOG:
	strcpy(s, "Logarithmic");
	break;
    case SCALE_REC:
	strcpy(s, "Reciprocal");
	break;
    case SCALE_LOGIT:
	strcpy(s, "Logit");
	break; 	   
    default:
        strcpy(s, "Unknown");
	break;
    }
    
    return s;
}

/*
 * axis scaling
 */
double fscale(double wc, int scale)
{
    switch (scale) {
    case SCALE_NORMAL:
        return (wc);
    case SCALE_LOG:
        return (log10(wc));
    case SCALE_REC:
        return (1.0/wc);
    case SCALE_LOGIT:
        return (log(wc/(1.0-wc)));
    default:
        errmsg("internal error in fscale()");
        return (wc);
    }
}

/*
 * inverse of the above
 */
double ifscale(double vc, int scale)
{
    switch (scale) {
    case SCALE_NORMAL:
        return (vc);
    case SCALE_LOG:
        return (pow(10.0, vc));
    case SCALE_REC:
        return (1.0/vc);
    case SCALE_LOGIT:
        return (exp(vc)/(1+exp(vc)));
    default:
        errmsg("internal error in ifscale()");
        return (vc);
    }
}

double fscale2(double vc,int gno,int axis)
{
    if (is_xaxis(axis))
    {
        if (get_graph_xscale(gno)==SCALE_REC)
        return fscale(vc,get_graph_xscale(gno))*(-1.0+2.0*is_graph_xinvert(gno));
        else
        return fscale(vc,get_graph_xscale(gno))*(1.0-2.0*is_graph_xinvert(gno));
    }
    else
    {
        if (get_graph_yscale(gno)==SCALE_REC)
        return fscale(vc,get_graph_yscale(gno))*(-1.0+2.0*is_graph_yinvert(gno));
        else
        return fscale(vc,get_graph_yscale(gno))*(1.0-2.0*is_graph_yinvert(gno));
    }
}

double ifscale2(double vc,int gno,int axis)
{
    if (is_xaxis(axis))
    {
        if (get_graph_xscale(gno)==SCALE_REC)
        return ifscale(vc*(-1.0+2.0*is_graph_xinvert(gno)),get_graph_xscale(gno));
        else
        return ifscale(vc*(1.0-2.0*is_graph_xinvert(gno)),get_graph_xscale(gno));
    }
    else
    {
        if (get_graph_yscale(gno)==SCALE_REC)
        return ifscale(vc*(-1.0+2.0*is_graph_yinvert(gno)),get_graph_yscale(gno));
        else
        return ifscale(vc*(1.0-2.0*is_graph_yinvert(gno)),get_graph_yscale(gno));
    }
}

/*
 * map world co-ordinates to viewport
 */

/* x-axis */
double xy_xconv_general(double wx)
{
    if ((scaletypex == SCALE_LOG && wx <= 0.0) ||
        (scaletypex == SCALE_REC && wx == 0.0) ||
        (scaletypex == SCALE_LOGIT && wx <= 0.0) ||
        (scaletypex == SCALE_LOGIT && wx >= 1.0)){
        return 0;
    } else {
        return (double)(xv_med + xv_rc*((long double) fscale(wx, scaletypex) - fxg_med));
    }
}

double xy_xconv_rec(double wx)
{
    if (wx == 0.0)
    {
        return 0;
    } else {
        return (double)(xv_med + xv_rc*((long double) (1.0/wx) - fxg_med));
    }
}

double xy_xconv_logit(double wx)
{
    if (wx <= 0.0 || wx >= 1.0)
    {
        return 0;
    } else {
        return (double)(xv_med + xv_rc*((long double) (log(wx/(1.0-wx))) - fxg_med));
    }
}

double xy_xconv_log(double wx)
{
    if (wx <= 0.0)
    {
        return 0;
    } else {
        return (double)(xv_med + xv_rc*((long double) log10(wx) - fxg_med));
    }
}

double xy_xconv_simple(double wx)
{
return (double)(xv_med + xv_rc*((long double) wx - fxg_med));
}
/* y-axis */
double xy_yconv_general(double wy)
{
    if ((scaletypey == SCALE_LOG && wy <= 0.0) ||
        (scaletypey == SCALE_REC && wy == 0.0) ||
        (scaletypey == SCALE_LOGIT && wy <= 0.0) ||
        (scaletypey == SCALE_LOGIT && wy >= 1.0)) {
        return 0;
    } else {
        return (double)(yv_med + yv_rc*((long double) fscale(wy, scaletypey) - fyg_med));
    }
}

double xy_yconv_rec(double wy)
{
    if (wy == 0.0)
    {
        return 0;
    } else {
        return (double)(yv_med + yv_rc*((long double) (1.0/wy) - fyg_med));
    }
}

double xy_yconv_logit(double wy)
{
    if (wy <= 0.0 || wy >= 1.0)
    {
        return 0;
    } else {
        return (double)(yv_med + yv_rc*((long double) (log(wy/(1.0-wy))) - fyg_med));
    }
}

double xy_yconv_log(double wy)
{
    if (wy <= 0.0)
    {
        return 0;
    } else {
        return (double)(yv_med + yv_rc*((long double) log10(wy) - fyg_med));
    }
}

double xy_yconv_simple(double wy)
{
    return (double)(yv_med + yv_rc*((long double) wy - fyg_med));
}

int polar2xy(double phi, double rho, double *x, double *y)
{
    if (rho < 0.0) {
        return (RETURN_FAILURE);
    } else {
        *x = rho*cos(phi);
        *y = rho*sin(phi);
        return (RETURN_SUCCESS);
    }
}

void xy2polar(double x, double y, double *phi, double *rho)
{
    *phi = atan2(y, x);
    *rho = hypot(x, y);
}

int polar2xy2(double phi, double rho, double *x, double *y)
{
    *x = ((rho-rhomin)*polar2_f+roffset)*cos(phi+phi0);
    *y = ((rho-rhomin)*polar2_f+roffset)*sin(phi+phi0);
return (RETURN_SUCCESS);
}

void xy2polar2(double x, double y, double *phi, double *rho)
{
    *phi = atan2(y, x)-phi0;
    *rho = (hypot(x, y)-roffset)/polar2_f+rhomin;
}

int world2view(double x, double y, double *xv, double *yv)
{
   /*if (coordinates == COORDINATES_XY)
   {
        *xv = xy_xconv(x);
        *yv = xy_yconv(y);
   }
   else */if (coordinates == COORDINATES_POLAR) {
        if (polar2xy(xv_rc*x, yv_rc*y, xv, yv) != RETURN_SUCCESS) {
            return (RETURN_FAILURE);
        }
        *xv += xv_med;
        *yv += yv_med;
    }
    else if (coordinates == COORDINATES_POLAR2) {
            if (polar2xy2(xv_rc*x, yv_rc*y, xv, yv) != RETURN_SUCCESS) {
                return (RETURN_FAILURE);
            }
            *xv += xv_med;
            *yv += yv_med;
    } else {
        *xv = xy_xconv(x);
        *yv = xy_yconv(y);
    }
    return (RETURN_SUCCESS);
}

/*
 * view2world - given (xv,yv) in viewport coordinates, return world coordinates
 *            in (xw,yw)
 */
void view2world(double xv, double yv, double *xw, double *yw)
{
    /*if (coordinates == COORDINATES_XY)
    {
        *xw = ifscale(fxg_med + (1.0/xv_rc)*((long double) xv - xv_med), scaletypex);
        *yw = ifscale(fyg_med + (1.0/yv_rc)*((long double) yv - yv_med), scaletypey);
    }
    else */if (coordinates == COORDINATES_POLAR) {
        xy2polar(xv - xv_med, yv - yv_med, xw, yw);
        *xw /= xv_rc;
        *yw /= yv_rc;
    }
    else if (coordinates == COORDINATES_POLAR2) {
            xy2polar2(xv - xv_med, yv - yv_med, xw, yw);
            *xw /= xv_rc;
            *yw /= yv_rc;
    } else {
        *xw = ifscale(fxg_med + (1.0/xv_rc)*((long double) xv - xv_med), scaletypex);
        *yw = ifscale(fyg_med + (1.0/yv_rc)*((long double) yv - yv_med), scaletypey);
    }
}

void view2_graph_world(int gno, double xv, double yv, double *xw, double *yw)
{
*xw=*yw=0.0;
    if (is_valid_gno(gno)==FALSE) return;
    world w;
    view v;
    world l_worldwin=worldwin;
    view l_viewport=viewport;
    int l_coordinates=coordinates;
    int l_scaletypex=scaletypex;
    int l_scaletypey=scaletypey;
    long double l_xv_med=xv_med;
    long double l_yv_med=yv_med;
    long double l_xv_rc=xv_rc;
    long double l_yv_rc=yv_rc;
    long double l_polar2_f=polar2_f;
    long double l_phi0=phi0;
    long double l_rhomin=rhomin;
    long double l_roffset=roffset;
    long double l_rmax=rmax;
    long double l_fxg_med=fxg_med;
    long double l_fyg_med=fyg_med;
get_graph_world(gno,&w);
get_graph_viewport(gno,&v);
int ret=definewindow_local(w,v,g[gno].type,get_graph_xscale(gno),get_graph_yscale(gno),g[gno].xinvert,g[gno].yinvert,
                           &l_worldwin,&l_viewport,&l_coordinates,
                           &l_scaletypex,&l_scaletypey,
                           &l_xv_med,&l_yv_med,
                           &l_xv_rc,&l_yv_rc,
                           &l_polar2_f,&l_phi0,
                           &l_rhomin,&l_roffset,&l_rmax,
                           &l_fxg_med,&l_fyg_med,gno);
if (ret==RETURN_FAILURE) return;
    if (l_coordinates == COORDINATES_POLAR) {
        *xw = atan2(yv - l_yv_med, xv - l_xv_med) / l_xv_rc;
        *yw = hypot(xv - l_xv_med, yv - l_yv_med) / l_yv_rc;
    }
    else if (l_coordinates == COORDINATES_POLAR2) {
        *xw = (atan2(yv - l_yv_med, xv - l_xv_med)-l_phi0) / l_xv_rc;
        *yw = ((hypot(xv - l_xv_med, yv - l_yv_med)-l_roffset)/l_polar2_f+l_rhomin) / l_yv_rc;
    } else {
        *xw = ifscale(l_fxg_med + (1.0/l_xv_rc)*((long double) xv - l_xv_med), l_scaletypex);
        *yw = ifscale(l_fyg_med + (1.0/l_yv_rc)*((long double) yv - l_yv_med), l_scaletypey);
    }
}

/*
 * definewindow - defines the scaling
 *               of the plotting rectangle to be used for clipping
 * local-version returns the values,
 * the other version sets the values in the global variables
 */
int definewindow_local(world w, view v, int gtype,
                       int xscale, int yscale,
                       int invx, int invy,
                       world * l_worldwin,
                       view * l_viewport,
                       int * l_coordinates,
                       int * l_scaletypex,
                       int * l_scaletypey,
                       long double * l_xv_med,
                       long double * l_yv_med,
                       long double * l_xv_rc,
                       long double * l_yv_rc,
                       long double * l_polar2_f,
                       long double * l_phi0,
                       long double * l_rhomin,
                       long double * l_roffset,
                       long double * l_rmax,
                       long double * l_fxg_med,
                       long double * l_fyg_med,
                       int gno)
{
    long double dx, dy;
    
    /* Safety checks */
    if (isvalid_viewport(v) == FALSE) {
        errmsg("Invalid viewport coordinates");
        return RETURN_FAILURE;
    }
    
    dx = (long double) w.xg2 - (long double) w.xg1;
    if (dx <= 0.0) {
        errmsg("World DX <= 0.0");
        return RETURN_FAILURE;
    }
    dy = (long double) w.yg2 - (long double) w.yg1;
    if (dy <= 0.0) {
        errmsg("World DY <= 0.0");
        return RETURN_FAILURE;
    }

    switch (gtype) {
    case GRAPH_POLAR:
        if (w.yg2 <= 0.0) {
            errmsg("World Rho-max <= 0.0");
            return RETURN_FAILURE;
        } else
        if ((xscale != SCALE_NORMAL) ||
            (yscale != SCALE_NORMAL)) {
            errmsg("Only linear scales are supported in Polar plots");
            return RETURN_FAILURE;
        } else
        if (invy == TRUE) {
            errmsg("Can't set Y scale inverted in Polar plot");
            return RETURN_FAILURE;
        } else {
            *l_coordinates = COORDINATES_POLAR;
            *l_worldwin = w;
            *l_viewport = v;
            *l_scaletypex = xscale;
            *l_xv_med = ((long double) v.xv1 + (long double) v.xv2)*0.5;
            if (invx == FALSE) {
                *l_xv_rc = +1.0;
            } else {
                *l_xv_rc = -1.0;
            }
            *l_scaletypey = yscale;
            *l_yv_med = ((long double) v.yv1 + (long double) v.yv2)*0.5;
            *l_yv_rc = (MIN2((long double) v.xv2 - (long double) v.xv1, (long double) v.yv2 - (long double) v.yv1)*0.5)/((long double) w.yg2);
            return RETURN_SUCCESS;
        }
        break;
    case GRAPH_POLAR2:
        /*if (w.yg2 <= 0.0) {
            errmsg("World Rho-max <= 0.0");
            return RETURN_FAILURE;
        } else
        if ((xscale != SCALE_NORMAL) ||
            (yscale != SCALE_NORMAL)) {
            errmsg("Only linear scales are supported in Polar plots");
            return RETURN_FAILURE;
        } else
        if (invy == TRUE) {
            errmsg("Can't set Y scale inverted in Polar plot");
            return RETURN_FAILURE;
        } else {*/
            *l_coordinates = COORDINATES_POLAR2;
            *l_worldwin = w;
            *l_viewport = v;
            *l_scaletypex = xscale;
            *l_xv_med = ((long double) v.xv1 + (long double) v.xv2)*0.5;
            if (invx == FALSE) {
                *l_xv_rc = +1.0;
            } else {
                *l_xv_rc = -1.0;
            }
            *l_scaletypey = yscale;
            *l_yv_med = ((long double) v.yv1 + (long double) v.yv2)*0.5;
            if (invy == FALSE)
            {
                *l_yv_rc = +1.0;
            } else {
                *l_yv_rc = -1.0;
            }
            //yv_rc = (MIN2((long double) v.xv2 - (long double) v.xv1, (long double) v.yv2 - (long double) v.yv1)*0.5)/((long double) w.yg2);
            *l_rmax=(MIN2((long double) v.xv2 - (long double) v.xv1, (long double) v.yv2 - (long double) v.yv1)*0.5);
            *l_roffset=*l_rmax*((long double)g[gno].roffset);
            *l_phi0=(long double)g[gno].phi0;
            *l_rhomin=(long double)w.yg1;
            *l_polar2_f=(*l_rmax-*l_roffset)/((long double)w.yg2 - (long double)w.yg1);
            return RETURN_SUCCESS;
        //}
        break;
    case GRAPH_FIXED:
        if ((xscale != SCALE_NORMAL) ||
            (yscale != SCALE_NORMAL)) {
            errmsg("Only linear axis scale is allowed in Fixed graphs");
            return RETURN_FAILURE;
        } else {
            *l_coordinates = COORDINATES_XY;
            *l_worldwin = w;
            *l_viewport = v;

            *l_scaletypex = xscale;
            *l_xv_med = ((long double) v.xv1 + (long double) v.xv2)*0.5;
            *l_fxg_med = ((long double) w.xg1 + (long double) w.xg2)*0.5;
            *l_scaletypey = yscale;
            *l_yv_med = ((long double) v.yv1 + (long double) v.yv2)*0.5;
            *l_fyg_med = ((long double) w.yg1 + (long double) w.yg2)*0.5;

            *l_xv_rc = MIN2(((long double) v.xv2 - (long double) v.xv1)/((long double) w.xg2 - (long double) w.xg1),
                         ((long double) v.yv2 - (long double) v.yv1)/((long double) w.yg2 - (long double) w.yg1));
            *l_yv_rc = *l_xv_rc;
            if (invx == TRUE) {
                *l_xv_rc = -*l_xv_rc;
            }
            if (invy == TRUE) {
                *l_yv_rc = -*l_yv_rc;
            }

            return RETURN_SUCCESS;
        }
        break;
    default:
        if (xscale == SCALE_LOG) {
            if (w.xg1 <= 0) {
                errmsg("World X-min <= 0.0");
                return RETURN_FAILURE;
            }
            if (w.xg2 <= 0) {
                errmsg("World X-max <= 0.0");
                return RETURN_FAILURE;
            }
        } else if (xscale == SCALE_REC) {
            if (sign(w.xg1) != sign(w.xg2)) {
                errmsg("X-axis contains 0");
                return RETURN_FAILURE;
            }
	    
        }
        if (xscale == SCALE_LOGIT) {
            if (w.xg1 <= 0) {
                errmsg("World X-min <= 0.0");
                return RETURN_FAILURE;
            }
            if (w.xg2 >= 1) {
                errmsg("World X-max >= 1.0");
                return RETURN_FAILURE;
            }
	}    
        
        if (yscale == SCALE_LOG) {
            if (w.yg1 <= 0.0) {
                errmsg("World Y-min <= 0.0");
                return RETURN_FAILURE;
            }
            if (w.yg2 <= 0.0) {
                errmsg("World Y-max <= 0.0");
                return RETURN_FAILURE;
            }
        } else if (yscale == SCALE_REC) {
            if (sign(w.yg1) != sign(w.yg2)) {
                errmsg("Y-axis contains 0");
                return RETURN_FAILURE;
            }
        }
	if (yscale == SCALE_LOGIT) {
            if (w.yg1 <= 0) {
                errmsg("World Y-min <= 0.0");
                return RETURN_FAILURE;
            }
            if (w.yg2 >= 1) {
                errmsg("World Y-max >= 1.0");
                return RETURN_FAILURE;
            }
	}    

        *l_coordinates = COORDINATES_XY;
        *l_worldwin = w;
        *l_viewport = v;

        *l_scaletypex = xscale;
        *l_xv_med = ((long double) v.xv1 + (long double) v.xv2)*0.5;
        *l_fxg_med = ((long double) fscale(w.xg1, xscale) + (long double) fscale(w.xg2, xscale))*0.5;
        if (invx == FALSE) {
            *l_xv_rc = ((long double) v.xv2 - (long double) v.xv1)/((long double) fscale(w.xg2, xscale) - (long double) fscale(w.xg1, xscale));
        } else {
            *l_xv_rc = - ((long double) v.xv2 - (long double) v.xv1)/((long double) fscale(w.xg2, xscale) - (long double) fscale(w.xg1, xscale));
        }

        *l_scaletypey = yscale;
        *l_yv_med = ((long double) v.yv1 + (long double) v.yv2)*0.5;
        *l_fyg_med = ((long double) fscale(w.yg1, yscale) + (long double) fscale(w.yg2, yscale))*0.5;
        if (invy == FALSE) {
            *l_yv_rc = ((long double) v.yv2 - (long double) v.yv1)/((long double) fscale(w.yg2, yscale) - (long double) fscale(w.yg1, yscale));
        } else {
            *l_yv_rc = - ((long double) v.yv2 - (long double) v.yv1)/((long double) fscale(w.yg2, yscale) - (long double) fscale(w.yg1, yscale));
        }
 
        return RETURN_SUCCESS;
        break;
    }
}

int definewindow(world w, view v, int gtype,
                        int xscale, int yscale,
                        int invx, int invy)
{
world l_worldwin=worldwin;
view l_viewport=viewport;
int l_coordinates=coordinates;
int l_scaletypex=scaletypex;
int l_scaletypey=scaletypey;
long double l_xv_med=xv_med;
long double l_yv_med=yv_med;
long double l_xv_rc=xv_rc;
long double l_yv_rc=yv_rc;
long double l_polar2_f=polar2_f;
long double l_phi0=phi0;
long double l_rhomin=rhomin;
long double l_roffset=roffset;
long double l_rmax=rmax;
long double l_fxg_med=fxg_med;
long double l_fyg_med=fyg_med;

int ret=definewindow_local(w,v,gtype,xscale,yscale,invx,invy,
                           &l_worldwin,&l_viewport,&l_coordinates,
                           &l_scaletypex,&l_scaletypey,
                           &l_xv_med,&l_yv_med,
                           &l_xv_rc,&l_yv_rc,
                           &l_polar2_f,&l_phi0,
                           &l_rhomin,&l_roffset,&l_rmax,
                           &l_fxg_med,&l_fyg_med,get_parser_gno());
    if (ret==RETURN_SUCCESS)
    {
    worldwin=l_worldwin;
    viewport=l_viewport;
    coordinates=l_coordinates;
    scaletypex=l_scaletypex;
    scaletypey=l_scaletypey;
    xv_med=l_xv_med;
    yv_med=l_yv_med;
    xv_rc=l_xv_rc;
    yv_rc=l_yv_rc;
    polar2_f=l_polar2_f;
    phi0=l_phi0;
    rhomin=l_rhomin;
    roffset=l_roffset;
    rmax=l_rmax;
    fxg_med=l_fxg_med;
    fyg_med=l_fyg_med;
    }
return ret;
}

int isvalid_viewport(view v)
{
    if ((v.xv2 <= v.xv1) || (v.yv2 <= v.yv1)) {
	return FALSE;
    } else {
        return TRUE;
    }
}

/*
 * ---------------- bbox utilities --------------------
 */

static BBox_type bboxes[3];
static const view invalid_view = {0.0, 0.0, 0.0, 0.0};

void reset_bbox(int type)
{
    view *vp;
    
    switch(type) {
    case BBOX_TYPE_GLOB:
        vp = &(bboxes[0].v);
        break;
    case BBOX_TYPE_TEMP:
        vp = &(bboxes[1].v);
        break;
    case BBOX_TYPE_LOCAL:
        vp = &(bboxes[2].v);
        break;
    default:
        errmsg ("Incorrect call of reset_bbox()");
        return;
    }
    *vp = invalid_view;
}

void reset_bboxes(void)
{
    reset_bbox(BBOX_TYPE_GLOB);
    reset_bbox(BBOX_TYPE_TEMP);
    reset_bbox(BBOX_TYPE_LOCAL);
}

void freeze_bbox(int type)
{
    BBox_type *bbp;
    
    switch(type) {
    case BBOX_TYPE_GLOB:
        bbp = &bboxes[0];
        break;
    case BBOX_TYPE_TEMP:
        bbp = &bboxes[1];
        break;
    case BBOX_TYPE_LOCAL:
        bbp = &bboxes[2];
        break;
    default:
        errmsg ("Incorrect call of freeze_bbox()");
        return;
    }
    bbp->fv = bbp->v;
}

view get_bbox(int type)
{
    view v;
    
    switch(type) {
    case BBOX_TYPE_GLOB:
        v = bboxes[0].v;
        break;
    case BBOX_TYPE_TEMP:
        v = bboxes[1].v;
        break;
    case BBOX_TYPE_LOCAL:
        v = bboxes[2].v;
        break;
    default:
        v = invalid_view;
        errmsg ("Incorrect call of get_bbox()");
        break;
    }
    return (v);
}

int is_valid_bbox(view v)
{
    if ((v.xv1 == invalid_view.xv1) &&
        (v.xv2 == invalid_view.xv2) &&
        (v.yv1 == invalid_view.yv1) &&
        (v.yv2 == invalid_view.yv2)) {
        return (FALSE);
    } else {
        return (TRUE);
    }
}

view merge_bboxes(view v1, view v2)
{
    view vtmp;
    
    if (!is_valid_bbox(v1)) {
        if (is_valid_bbox(v2)) {
            return (v2);
        } else {
            return (invalid_view);
        }
    } else if (!is_valid_bbox(v2)) {
        return (v1);
    } else {
        vtmp.xv1 = MIN2(v1.xv1, v2.xv1);
        vtmp.xv2 = MAX2(v1.xv2, v2.xv2);
        vtmp.yv1 = MIN2(v1.yv1, v2.yv1);
        vtmp.yv2 = MAX2(v1.yv2, v2.yv2);
        return (vtmp);
    }
}

void update_bbox(int type, VPoint vp)
{
    BBox_type *bbp;
    
    switch(type) {
    case BBOX_TYPE_GLOB:
        /* Global bbox is updated only with real drawings */
        if (get_draw_mode() == FALSE) {
            return;
        }
        bbp = &bboxes[0];
        break;
    case BBOX_TYPE_TEMP:
        bbp = &bboxes[1];
        break;
    case BBOX_TYPE_LOCAL:
        bbp = &bboxes[2];
        break;
    default:
        errmsg ("Incorrect call of update_bbox()");
        return;
    }
    if (bbp->active == TRUE) {
        if (is_vpoint_inside(bbp->v, vp, 0.0) == FALSE) {
            if (is_valid_bbox(bbp->v)) {
                bbp->v.xv1 = MIN2(bbp->v.xv1, vp.x);
                bbp->v.xv2 = MAX2(bbp->v.xv2, vp.x);
                bbp->v.yv1 = MIN2(bbp->v.yv1, vp.y);
                bbp->v.yv2 = MAX2(bbp->v.yv2, vp.y);
            } else {
                bbp->v.xv1 = vp.x;
                bbp->v.xv2 = vp.x;
                bbp->v.yv1 = vp.y;
                bbp->v.yv2 = vp.y;
            }
        }
    }
}

void update_bboxes(VPoint vp)
{
    update_bbox(BBOX_TYPE_GLOB, vp);
    update_bbox(BBOX_TYPE_TEMP, vp);
    update_bbox(BBOX_TYPE_LOCAL, vp);
}

void melt_bbox(int type)
{
    BBox_type *bbp;
    
    switch(type) {
    case BBOX_TYPE_GLOB:
        bbp = &bboxes[0];
        break;
    case BBOX_TYPE_TEMP:
        bbp = &bboxes[1];
        break;
    case BBOX_TYPE_LOCAL:
        bbp = &bboxes[2];
        break;
    default:
        errmsg ("Incorrect call of melt_bbox()");
        return;
    }
    bbp->v = merge_bboxes(bbp->v, bbp->fv);
}

void activate_bbox(int type, int status)
{
    BBox_type *bbp;
    
    switch(type) {
    case BBOX_TYPE_GLOB:
        bbp = &bboxes[0];
        break;
    case BBOX_TYPE_TEMP:
        bbp = &bboxes[1];
        break;
    case BBOX_TYPE_LOCAL:
        bbp = &bboxes[2];
        break;
    default:
        errmsg ("Incorrect call of activate_bbox()");
        return;
    }
    bbp->active = status;
}

/* Extend all view boundaries with w */
int view_extend(view *v, double w)
{
    if (!v) {
        return RETURN_FAILURE;
    } else {
        v->xv1 -= w;
        v->xv2 += w;
        v->yv1 -= w;
        v->yv2 += w;
        return RETURN_SUCCESS;
    }
}

int update_bboxes_with_view(view *v)
{
    if (!v) {
        return RETURN_FAILURE;
    } else {
        VPoint vp;
        
        vp.x = v->xv1;
        vp.y = v->yv1;
        update_bboxes(vp);
        vp.x = v->xv2;
        vp.y = v->yv2;
        update_bboxes(vp);
        
        return RETURN_SUCCESS;
    }
}

int update_bboxes_with_vpoints(VPoint *vps, int n, double lw)
{
    if (!vps || n < 1) {
        return RETURN_FAILURE;
    } else {
        int i;
        double xmin, xmax, ymin, ymax;
        view v;
        
        xmin = xmax = vps[0].x;
        ymin = ymax = vps[0].y;
        
        for (i = 1; i < n; i++) {
            if (vps[i].x < xmin) {
                xmin = vps[i].x;
            } else
            if  (vps[i].x > xmax) {
                xmax = vps[i].x;
            }
            
            if (vps[i].y < ymin) {
                ymin = vps[i].y;
            } else
            if  (vps[i].y > ymax) {
                ymax = vps[i].y;
            }
        }
        
        v.xv1 = xmin;
        v.xv2 = xmax;
        v.yv1 = ymin;
        v.yv2 = ymax;
        
        view_extend(&v, lw*0.5);
        
        update_bboxes_with_view(&v);
        
        return RETURN_SUCCESS;
    }
}

int VPoints2bbox(VPoint *vp1, VPoint *vp2, view *bb)
{
    if (!bb || !vp1 || !vp2) {
        return RETURN_FAILURE;
    } else {
        if (vp1->x <= vp2->x) {
            bb->xv1 = vp1->x;
            bb->xv2 = vp2->x;
        } else {
            bb->xv1 = vp2->x;
            bb->xv2 = vp1->x;
        }
        if (vp1->y <= vp2->y) {
            bb->yv1 = vp1->y;
            bb->yv2 = vp2->y;
        } else {
            bb->yv1 = vp2->y;
            bb->yv2 = vp1->y;
        }
        
        return RETURN_SUCCESS;
    }
}

static int draw_mode = TRUE;

void set_draw_mode(int mode)
{
    draw_mode = mode ? TRUE:FALSE;
}

int get_draw_mode(void)
{
    return (draw_mode);
}

void vpswap(VPoint *vp1, VPoint *vp2)
{
    VPoint vptmp;

    vptmp = *vp1;
    *vp1 = *vp2;
    *vp2 = vptmp;
}

int points_overlap(VPoint vp1, VPoint vp2)
{
    double delta;
    
delta = 1.0/MIN2(page_width, page_height);
///A4-Format temorarely
///delta = 1.0/MIN2(842, 595);
    if (fabs(vp2.x - vp1.x) < delta || fabs(vp2.y - vp1.y) < delta) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static int max_path_length = MAX_DRAWING_PATH;

void set_max_path_limit(int limit)
{
    max_path_length = limit;
}

int get_max_path_limit(void)
{
    return max_path_length;
}

#define PURGE_INIT_FACTOR   1.0
#define PURGE_ITER_FACTOR   M_SQRT2
#define PURGE_EPS_WORST     0.01

/* Note: vps and pvps may be the same array! */
static void purge_dense_points(const VPoint *vps, int n, VPoint *pvps, int *np)
{
    int i, j, iter;
    int ok;
    double eps;
    VPoint vptmp;
    
    if (*np <= 0) {
        *np = 0;
        return;
    }
    
    if (n <= *np) {
        memmove(pvps, vps, n*sizeof(VPoint));
        *np = n;
        return;
    }
    
    /* Start with 1/np epsilon */
    eps = PURGE_INIT_FACTOR/(*np);
    iter = 0;
    ok = FALSE;
    while (ok == FALSE) {
        j = 0;
        vptmp = vps[0];
        for (i = 1; i < n; i++) {
            if (fabs(vps[i].x - vptmp.x) > eps ||
                fabs(vps[i].y - vptmp.y) > eps) {
                vptmp = vps[i];
                j++;
                if (j >= *np) {
                    break;
                }
            }
        }
        if (j < *np - 1) {
            ok = TRUE;
        } else {
            eps *= PURGE_ITER_FACTOR;
        }
        iter++;
    }

    if (eps > PURGE_EPS_WORST) {
        errmsg("Purging failed. Increase 'Max drawing path length' in prefs.");
    }

    /* actually fill in the purged array */
    pvps[0] = vps[0];
    j = 0;
    for (i = 1; i < n; i++) {
        if (fabs(vps[i].x - pvps[j].x) > eps ||
            fabs(vps[i].y - pvps[j].y) > eps) {
            pvps[++j] = vps[i];
        }
    }
    
    *np = j + 1;
#ifdef DEBUG    
    if (get_debuglevel() == 6) {
        printf("Purging %d points to %d in %d iteration(s)\n", n, *np, iter);
    }
#endif
}

void rotateVPoint(VPoint * vp,VPoint center,double angle)
{
double c=cos(angle);
double s=sin(angle);
static double tmp1,tmp2;
vp->x-=center.x;
vp->y-=center.y;
    tmp1=c*vp->x+s*vp->y;
    tmp2=-s*vp->x+c*vp->y;
vp->x=tmp1+center.x;
vp->y=tmp2+center.y;
}
