/*
 * Grace - GRaphing, Advanced Computation and Exploration of data
 * 
 * Home page: http://plasma-gate.weizmann.ac.il/Grace/
 * 
 * Copyright (c) 1996-99 Grace Development Team
 * Copyright (c) 1991-95 Paul J Turner, Portland, OR
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

#include "defines.h"
#include "MainWindow.h"

//#include <X11/Xlib.h>
#include <QtGui>

#include "xprotos.h"

#define CMAP_INSTALL_NEVER      0
#define CMAP_INSTALL_ALWAYS     1
#define CMAP_INSTALL_AUTO       2

int xlibinit(void);
int xlibinitgraphics(void);
void drawxlib(int x, int y, int mode);
void xlibupdatecmap(void);
void xlibinitcmap(void);
void xlibdrawpixel(VPoint vp);
void xlibdrawpolyline(VPoint *vps, int n, int mode);
void xlibfillpolygon(VPoint *vps, int npoints);
void xlibdrawarc(VPoint vp1, VPoint vp2, int angle1, int angle2);
void xlibfillarc(VPoint vp1, VPoint vp2, int angle1, int angle2, int mode);
//void xlibredraw(MainArea * ma, int x, int y, int widht, int height);
void xlibputpixmap(VPoint vp, int width, int height,char *databits, int pixmap_bpp, int bitmap_pad, int pixmap_type);

void xlibleavegraphics(void);

int xconvxlib(double x);
int yconvxlib(double y);
void xlibVPoint2dev(VPoint vp, int *x, int *y);
VPoint xlibdev2VPoint(int x, int y);
QPoint VPoint2XPoint(VPoint vp);
QPointF VPoint2XPointF(VPoint vp);

QColor get_Color(int i);
void set_Pen_only(void);
void set_Brush_only(int fg,int bg,int p);

void reset_Transformation_Matrix(void);
void set_Rotation_Matrix(double x_centre,double y_centre,int rot);
