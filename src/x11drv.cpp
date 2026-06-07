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
 * driver for X11 for Grace
 * modified for Qt-Library by Andreas Winter
 *
 */

///#include <config.h>
#include <cmath>
#include "defines.h"

#include <stdlib.h>

///#include <X11/Xlib.h>
#include <QtGui>
#include <QSvgGenerator>

#include "globals.h"
#include "utils.h"
#include "device.h"
#include "devlist.h"
#include "draw.h"
#include "graphs.h"
#include "patterns.h"

#include "x11drv.h"
#include "cmath.h"

#include "noxprotos.h"
#include "MainWindow.h"

//#define USE_FLOATING_POINT_DATA 1

extern bool useQPrinter;
extern bool symbol_font_is_special;
extern double GeneralPageZoomFactor;
extern unsigned int unicode_greek_shift;
extern bool printing_in_file;
extern QSvgGenerator * stdGenerator;
#if QT_VERSION >= 0x050300
extern QPdfWriter * stdPDFWriter;
#endif
extern QPrinter * stdPrinter;
extern QImage * MainPixmap;
extern QPainter * GeneralPainter;
extern MainWindow * mainWin;
extern QVector<qreal> ** PenDashPattern;
extern QBitmap * patterns[MAXPATTERNS];
//extern QTextCodec * FileCodec;
extern int print_target;
extern DrawProps draw_props;
extern unsigned int qtCharShift;
extern QMap<unsigned char,ushort> key_for_greek;
//extern int currentStringAlignment;

///Window root;
int screennumber;
///GC gc, gcxor;
int depth;

///static Visual *visual;
static int pixel_size;

int install_cmap = CMAP_INSTALL_AUTO;

//static int private_cmap = FALSE;

unsigned long xvlibcolors[MAXCOLORS];
///Colormap cmap;

/*
static QPixmap * curtile;
static QPixmap * displaybuff;// = (Pixmap) NULL;
*/

static int xlibcolor;
static int xlibbgcolor;
static int xlibpatno;
static int xliblinewidth;
static int xliblinestyle;
static int xlibfillrule;
static int xlibarcfillmode;
static int xliblinecap;
static int xliblinejoin;

void qtPutText(VPoint vp, char *s, int len, int font,TextMatrix *tm, int underline, int overline, int kerning);
extern void generate_Pixmap_from_Bits(unsigned char * bits,int length,int rows,int cols,double factor,QPixmap * target);
extern void generate_Pixmap_from_Bits(unsigned char * bits,int length,int rows,int cols,QPixmap * target,int color);
extern int get_QtFontID_from_Grace_Name(const char * name,int whatlist);

int current_dpi=72;
bool tmp_underline,tmp_overline,tmp_kerning;
TextMatrix *tmp_tm;
#define win_scale ((win_h < win_w) ? win_h:win_w)

//QPixmap resize_bufpixmap(unsigned int w, unsigned int h);

///original Grace: 'X11' instead of 'Screen'

static Device_entry dev_x11 = {DEVICE_TERM,
          "Screen",
          xlibinitgraphics,
          NULL,
          NULL,
          "",
          FALSE,
          TRUE,
          {DEFAULT_PAGE_WIDTH, DEFAULT_PAGE_HEIGHT, 72.0},
          NULL,
          1,
          "TERMINAL"
         };

QPoint CenterOfMass(int n,QPoint *p)
{
double x=0.0,y=0.0;
for (int i=0;i<n;i++)
{
x+=p[i].x();
y+=p[i].y();
}
return QPoint(x/n,y/n);
}

QPointF CenterOfMass(int n,QPointF *p)
{
double x=0.0,y=0.0;
for (int i=0;i<n;i++)
{
x+=p[i].x();
y+=p[i].y();
}
return QPointF(x/n,y/n);
}

int register_x11_drv(void)
{
    /*long mrsize;
    int max_path_limit;
    
     XExtendedMaxRequestSize() appeared in X11R6 */
/*#if XlibSpecificationRelease > 5
    mrsize = XExtendedMaxRequestSize(disp);
#else
    mrsize = 0;
#endif
    if (mrsize <= 0) {
        mrsize = XMaxRequestSize(disp);
    }
    max_path_limit = (mrsize - 3)/2;
    if (max_path_limit < get_max_path_limit()) {
        char buf[128];
        sprintf(buf,
            "Setting max drawing path length to %d (limited by the X server)",
            max_path_limit);
        errmsg(buf);
        set_max_path_limit(max_path_limit);
    }
    
    dev_x11.pg.dpi = rint(MM_PER_INCH*DisplayWidth(disp, screennumber)/
        DisplayWidthMM(disp, screennumber));
    */
//qDebug() << "LogicalDpiX=" << QApplication::desktop()->logicalDpiX() << " PhysicalDpiX=" << QApplication::desktop()->physicalDpiX();
//qDebug() << "LogicalDpiY=" << QApplication::desktop()->logicalDpiY() << " PhysicalDpiY=" << QApplication::desktop()->physicalDpiY();

/*#ifdef WINDOWS_SYSTEM
dev_x11.pg.dpi=QApplication::desktop()->logicalDpiX();
#else*/
#if QT_VERSION >= 0x060000
dev_x11.pg.dpi=qApp->primaryScreen()->physicalDotsPerInchX();
#else
dev_x11.pg.dpi=QApplication::desktop()->physicalDpiX();
#endif
/*#endif*/

/*
cout << "INIT X11: Logical DpiX=" << QApplication::desktop()->logicalDpiX() << " DpiY=" << QApplication::desktop()->logicalDpiY() << endl;
cout << "INIT X11: Physical DpiX=" << QApplication::desktop()->physicalDpiX() << " DpiY=" << QApplication::desktop()->physicalDpiY() << endl;
cout << "ScreenGeometry Width=" << QApplication::desktop()->screenGeometry().width() << " Height=" << QApplication::desktop()->screenGeometry().height() << endl;
cout << "MM Width=" << QApplication::desktop()->widthMM() << " Height=" << QApplication::desktop()->heightMM() << endl;
cout << "berechnete dpi=" << QApplication::desktop()->screenGeometry().width()/(QApplication::desktop()->widthMM()/25.4) << endl;
cout << "qApp:" << endl;
cout << "INIT X11: Logical DpiX=" << qApp->desktop()->logicalDpiX() << " DpiY=" << qApp->desktop()->logicalDpiY() << endl;
cout << "INIT X11: Physical DpiX=" << qApp->desktop()->physicalDpiX() << " DpiY=" << qApp->desktop()->physicalDpiY() << endl;
cout << "ScreenGeometry Width=" << qApp->desktop()->screenGeometry().width() << " Height=" << qApp->desktop()->screenGeometry().height() << endl;
cout << "MM Width=" << qApp->desktop()->widthMM() << " Height=" << qApp->desktop()->heightMM() << endl;
cout << "berechnete dpi=" << qApp->desktop()->screenGeometry().width()/(qApp->desktop()->widthMM()/25.4) << endl;
*/

pixel_size=1;
/*curtile=new QPixmap(10,10);
displaybuff=new QPixmap(10,10);*/
    return register_device(dev_x11);
}

int xlibinit(void)
{
pixel_size=1;
    return RETURN_SUCCESS;
}

int xconvxlib(double x)
{
    return ((int) rint(win_scale * x));
}

int yconvxlib(double y)
{
    return ((int) rint(win_h - win_scale * y));
}

void xlibVPoint2dev(VPoint vp, int *x, int *y)
{
    *x = xconvxlib(vp.x);
    *y = yconvxlib(vp.y);
}

QPoint VPoint2XPoint(VPoint vp)
{
    return QPoint(xconvxlib(vp.x),yconvxlib(vp.y));
}

QPointF VPoint2XPointF(VPoint vp)
{
    return QPointF(win_scale *vp.x , win_h *1.0- win_scale * vp.y);
}

/*
 * xlibdev2VPoint - given (x,y) in screen coordinates, return the 
 * viewport coordinates
 */
VPoint xlibdev2VPoint(int x, int y)
{
VPoint vp;
    if (win_scale == 0) {
        vp.x = (double) 0.0;
        vp.y = (double) 0.0;
    } else {
        vp.x = ((double) x) / win_scale;
        vp.y = (double) ((double)win_h - (double)y) / win_scale;
        if (vp.x<0.0) vp.x = 0.0;
        else if (vp.x>win_w) vp.x = ((double)win_w-1) / win_scale;
        if (vp.y<0.0) vp.y = 0.0;
        else if (vp.y>win_h) vp.y = ((double)win_h-1) / win_scale;
    }
return (vp);
}

VPoint xlibdev2VPoint2(qreal x, qreal y)
{
VPoint vp;
    if (win_scale == 0) {
        vp.x = (double) 0.0;
        vp.y = (double) 0.0;
    } else {
        vp.x = ((double) x) / win_scale;
        vp.y = (double) ((double)win_h - (double)y) / win_scale;
        /*if (vp.x<0.0) vp.x = 0.0;
        else if (vp.x>win_w) vp.x = ((double)win_w-1) / win_scale;
        if (vp.y<0.0) vp.y = 0.0;
        else if (vp.y>win_h) vp.y = ((double)win_h-1) / win_scale;*/
    }
return (vp);
}

void xlibupdatecmap(void)
{
    /* TODO: replace!!! */
    //if (inwin) 
    //{
        xlibinitcmap();
    //}
}

void xlibinitcmap(void)
{
    /*int i;
    RGB *prgb;
    QColor xc[MAXCOLORS];*/
    
    /*for (i = 0; i < MAXCOLORS; i++) {
        xc[i].pixel = 0;
        xc[i].flags = DoRed | DoGreen | DoBlue;
    }*/
    
    /*
	for (i = 0; i < number_of_colors(); i++) {
        // even in mono, b&w must be allocated 
        if (monomode == FALSE || i < 2) { 
            prgb = get_rgb(i);
            if (prgb != NULL) {
                xc[i].red = prgb->red << (16 - GRACE_BPP);
                xc[i].green = prgb->green << (16 - GRACE_BPP);
                xc[i].blue = prgb->blue << (16 - GRACE_BPP);
                if (XAllocColor(disp, cmap, &xc[i])) {
                    xvlibcolors[i] = xc[i].pixel;
                } else {
                    if (install_cmap != CMAP_INSTALL_NEVER && 
                                        private_cmap == FALSE) {
                        cmap = XCopyColormapAndFree(disp, cmap);
                        private_cmap = TRUE;
                        // will try to allocate the same color 
                        // in the private colormap
                        i--; 
                    } else {
                        // really bad
                        xvlibcolors[i] = xvlibcolors[1];
 //
 //                         errmsg("Can't allocate color");
 //
                    }
                }
            }
        } else {
            xvlibcolors[i] = xvlibcolors[1];
        }
    }
    */
}

int xlibinitgraphics(void)
{
    int i, j;
    double step;
    QPoint xp;

    /*if (inwin == FALSE) {
        return RETURN_FAILURE;
    }*/

//qDebug() << "xlib_init_graphics";

    xlibcolor = BAD_COLOR;
    xlibbgcolor = BAD_COLOR;
    xlibpatno = -1;
    xliblinewidth = -1;
    xliblinestyle = -1;
    xlibfillrule = -1;
    xlibarcfillmode = -1;
    xliblinecap   = -1;
    xliblinejoin  = -1;
    
    /* device-dependent routines */    
    devupdatecmap = xlibupdatecmap;
    devdrawpixel = xlibdrawpixel;
    devdrawpolyline = xlibdrawpolyline;
    devfillpolygon = xlibfillpolygon;
    devdrawarc = xlibdrawarc;
    devfillarc = xlibfillarc;
    devputpixmap = xlibputpixmap;
    devleavegraphics = xlibleavegraphics;
    devputtext = qtPutText;

    /* init settings specific to X11 driver */    
    if (get_pagelayout() == PAGE_FIXED)
    {
    sync_canvas_size(&win_w, &win_h, FALSE);
    }
    else
    {
    sync_canvas_size(&win_w, &win_h, TRUE);
    }
    ///*displaybuff = resize_bufpixmap(win_w, win_h);
    
    xlibupdatecmap();
    
    /*XSetForeground(disp, gc, xvlibcolors[0]);
    XSetFillStyle(disp, gc, FillSolid);
    XFillRectangle(disp, displaybuff, gc, 0, 0, win_w, win_h);
    XSetForeground(disp, gc, xvlibcolors[1]);*/

Page_geometry pg = get_page_geometry();//this depends on the current device
current_dpi=pg.dpi;//needed for the line-width-setting

    /*Qt-plotting stuff*/
    if (GeneralPainter->isActive()==true)
    GeneralPainter->end();
//Setting new target for GeneralPainter
if (print_target==PRINT_TARGET_SVG_FILE)//use the svg-Generator
{
GeneralPainter->begin(stdGenerator);
}
else if (print_target==PRINT_TARGET_PRINTER)//use a real printer
{
GeneralPainter->begin(stdPrinter);
//QPageLayout pl=stdPrinter->pageLayout();
//QRect rec=pl.fullRectPixels(stdPrinter->resolution());
qDebug() << "Printer-Size=" << stdPrinter->width() << "x" << stdPrinter->height();
qDebug() << "Resolution=" << stdPrinter->resolution();
qDebug() << "PageRct(Point)=" << stdPrinter->pageRect(QPrinter::Point).width() << "x" << stdPrinter->pageRect(QPrinter::Point).height();
qDebug() << "PageRct(DevPixel)=" << stdPrinter->pageRect(QPrinter::DevicePixel).width() << "x" << stdPrinter->pageRect(QPrinter::DevicePixel).height();
qDebug() << "PageRct(Inch)=" << stdPrinter->pageRect(QPrinter::Inch).width() << "x" << stdPrinter->pageRect(QPrinter::Inch).height();
qDebug() << "Physical Resolution=" << stdPrinter->physicalDpiX() << "x" << stdPrinter->physicalDpiY();
//qDebug() << "RectPixels=" << rec.width() << "x" << rec.height();
}
#if QT_VERSION >= 0x050300
else if (print_target==PRINT_TARGET_PDF_FILE)
{
stdPDFWriter=new QPdfWriter(print_file);
double xsize=pg.width/pg.dpi*MM_PER_INCH;
double ysize=pg.height/pg.dpi*MM_PER_INCH;
QSizeF page_size(xsize,ysize);
stdPDFWriter->setResolution(pg.dpi);
#if QT_VERSION >= 0x050A00
stdPDFWriter->setPageSize(QPageSize(page_size,QPageSize::Millimeter));
#else
stdPDFWriter->setPageSizeMM(QSizeF(xsize,ysize));
#endif
stdPDFWriter->newPage();

//QMessageBox::information(0,QString("filename="),QString(print_file));

GeneralPainter->begin(stdPDFWriter);
}
#endif
else//if (print_target== useQPrinter==false || stdPrinter==NULL)
{//export using the MainPixmap
    if (MainPixmap!=NULL) delete MainPixmap;
    /*if (print_target==PRINT_TARGET_SCREEN)
    *MainPixmap=QImage(win_w*GeneralPageZoomFactor,win_h*GeneralPageZoomFactor,QImage::Format_ARGB32_Premultiplied);
    else*/
    MainPixmap=new QImage(win_w,win_h,QImage::Format_ARGB32_Premultiplied);
/*qDebug() << "setting picture Size to " << win_w << "x" << win_h;
int a,b;
return_Page_Dimensions_pix(PAGE_FORMAT_A4,PAGE_ORIENT_LANDSCAPE,72,&a,&b);
qDebug() << "Definitionsgroesse DIN A4 (Landscape): " << a << "x" << b;*/
    if(MainPixmap->isNull())
    {
    QString p=QObject::tr("Invalid QImage size w=");
        p+=QString::number(win_w);
        p+=QObject::tr(" h=");
        p+=QString::number(win_h);
        p+=QObject::tr(", cannot paint there.");
    qDebug() << p.toLatin1();// << endl;
    return RETURN_FAILURE;
    }
GeneralPainter->begin(MainPixmap);
GeneralPainter->setRenderHint(QPainter::Antialiasing,general_antialiasing);
}

if ((printing_in_file==true || getbgfill()==FALSE) && !(printing_in_file==true && getbgfill()==TRUE))//I changed the first to || instead of && and added !(...)
{
MainPixmap->fill(Qt::transparent);
GeneralPainter->setPen(Qt::transparent);
GeneralPainter->setBrush(Qt::transparent);
GeneralPainter->setBackgroundMode(Qt::TransparentMode);
}
else
{
GeneralPainter->setPen(Qt::white);
GeneralPainter->setBrush(Qt::white);
GeneralPainter->setBackgroundMode(Qt::OpaqueMode);
GeneralPainter->drawRect(0,0,win_w - 1,win_h - 1);
}
//GeneralPainter->setPen(Qt::black);
GeneralPainter->setPen(Qt::NoPen);
GeneralPainter->setBrush(Qt::black);

if (printing_in_file==false && getbgfill() == FALSE && print_target!=PRINT_TARGET_PRINTER)
{
    step = ((double) win_scale) / 10.0;
    for (i = 0; i < win_w/step; i++)
    {
        for (j = 0; j < win_h/step; j++)
        {
            xp.setX(rint(i*step));
            xp.setY(win_h - rint(j*step));
            //GeneralPainter->drawPoint(xp.x(),xp.y());//drawing a point-grid to indicate transparency
            GeneralPainter->drawRect(xp.x(),xp.y(),1,1);
        }
    }
   ///XSetLineAttributes(disp, gc, 1, LineSolid, CapButt, JoinMiter);
   ///XDrawRectangle(disp, displaybuff, gc, 0, 0, win_w - 1, win_h - 1);
GeneralPainter->setBrush(Qt::NoBrush);
GeneralPainter->setPen(Qt::black);
GeneralPainter->drawRect(0,0,win_w - 1,win_h - 1);//draw border
    //cout << "drawing: " << win_w << " x " << win_h << endl;
}
return RETURN_SUCCESS;
}

void xlib_setpen(void)
{
QBrush brush(GeneralPainter->brush());
QPen pen(GeneralPainter->pen());
    int fg, bg, p;
    QPixmap ptmp;
    //RGB * rgb_color;

    fg = getcolor();
    bg = getbgcolor();
    p = getpattern();

xlibcolor=fg;
xlibbgcolor=bg;
xlibpatno=p;
set_Pen_only();
set_Brush_only(fg,bg,p);
return;
/// FUNCTION ENDS HERE!!!
    if ((fg == xlibcolor) && (bg == xlibbgcolor) && (p == xlibpatno)) {
        return;
    }
        
    if (fg != xlibcolor) {
    //rgb_color=get_rgb(fg);
        ///XSetForeground(disp, gc, xvlibcolors[fg]);
	pen.setColor(get_Color(fg));
	///GeneralPainter->setPen(QColor(rgb_color->red,rgb_color->green,rgb_color->blue));
	//GeneralPainter->setBrush(QColor(rgb_color->red,rgb_color->green,rgb_color->blue));
        xlibcolor = fg;
    }
    
    if (bg != xlibbgcolor) {
    //rgb_color=get_rgb(bg);
        ///XSetBackground(disp, gc, xvlibcolors[bg]);
	//GeneralPainter->setPen(QColor(rgb_color->red,rgb_color->green,rgb_color->blue));
	///GeneralPainter->setBrush(QColor(rgb_color->red,rgb_color->green,rgb_color->blue));
        xlibbgcolor = bg;
    }

    if (p >= number_of_patterns() || p < 0) {
        p = 0;
    }
    xlibpatno = p;
    
    if (p == 0) { /* TODO: transparency !!!*/
	brush.setStyle(Qt::NoBrush);
	GeneralPainter->setBrush(brush);
	///GeneralPainter->setBrush(brush);
        return;
    } else if (p == 1) {
        /* To make X faster */
	brush.setStyle(Qt::SolidPattern);
	///GeneralPainter->setBrush(brush);
        ///XSetFillStyle(disp, gc, FillSolid);
    } else {
        /* TODO: implement cache ? */
        ///ptmp = XCreateBitmapFromData(disp, root, (char *) pat_bits[p], 16, 16);
        ///XCopyPlane(disp, ptmp, curtile, gc, 0, 0, 16, 16, 0, 0, 1);
        ///XFreePixmap(disp, ptmp);

brush.setColor(get_Color(fg));

QPixmap pic(16,16);
generate_Pixmap_from_Bits(pat_bits[p],32,16,16,&pic,fg);
//brush.setTexture(patterns[p]->copy(0,0,16,16));
brush.setTexture(pic);
pic.fill(get_Color(bg));
QPainter paintr(&pic);
paintr.setBrush(brush);
paintr.setPen(Qt::NoPen);
paintr.drawRect(0,0,16,16);
paintr.end();
brush.setTexture(pic);
/*
 *      XSetFillStyle(disp, gc, FillStippled);
 *      XSetStipple(disp, gc, curstipple);
 */
        ///XSetFillStyle(disp, gc, FillTiled);
        ///XSetTile(disp, gc, curtile);

	///setDrawPropsOfPainter();
GeneralPainter->setBrush(brush);
        return;
    }
}

void xlib_setdrawbrush(void)
{
QPen pen(GeneralPainter->pen());
    int iw;
    int style;
    int lc, lj;
    int i, scale, darr_len;
    char *xdarr;

    xlib_setpen();
    
    iw = (int) rint(getlinewidth()*win_scale);
    if (iw == 1) {
        iw = 0;
    }
    style = getlinestyle();
    lc = getlinecap();
    lj = getlinejoin();
    

    /*switch (lc) {
    case LINECAP_BUTT:
	pen.setCapStyle(Qt::SquareCap);
        ///lc = CapButt;
        break;
    case LINECAP_ROUND:
	pen.setCapStyle(Qt::RoundCap);
        ///lc = CapRound;
        break;
    case LINECAP_PROJ:
	pen.setCapStyle(Qt::FlatCap);
        ///lc = CapProjecting;
        break;
    }

    switch (lj) {
    case LINEJOIN_MITER:
	pen.setJoinStyle(Qt::MiterJoin);
        ///lj = JoinMiter;
        break;
    case LINEJOIN_ROUND:
	pen.setJoinStyle(Qt::RoundJoin);
        ///lj = JoinRound;
        break;
    case LINEJOIN_BEVEL:
	pen.setJoinStyle(Qt::BevelJoin);
        ///lj = JoinBevel;
        break;
    }*/
    
    if (iw != xliblinewidth || style != xliblinestyle ||
        lc != xliblinecap   || lj    != xliblinejoin) {
        if (style > 1) {
            darr_len = dash_array_length[style];
            xdarr = (char*)xmalloc(darr_len*sizeof(char));
            if (xdarr == NULL) {
                return;
            }
            scale = MAX2(1, iw);
            for (i = 0; i < darr_len; i++) {
                xdarr[i] = scale*dash_array[style][i];
            }
	pen.setStyle(Qt::CustomDashLine);
	pen.setDashPattern(*PenDashPattern[style]);
            ///XSetLineAttributes(disp, gc, iw, LineOnOffDash, lc, lj);
            ///XSetDashes(disp, gc, 0, xdarr, darr_len);
            xfree(xdarr);
        } else if (style == 1) {
	pen.setStyle(Qt::SolidLine);
            ///XSetLineAttributes(disp, gc, iw, LineSolid, lc, lj);
        }
	else if (style == 0)
	{
	pen.setStyle(Qt::NoPen);
	}
	pen.setWidth(iw);

        xliblinestyle = style;
        xliblinewidth = iw;
        xliblinecap   = lc;
        xliblinejoin  = lj;
	GeneralPainter->setPen(pen);
    }
set_Pen_only();
    return;
}

void xlibdrawpixel(VPoint vp)
{
#ifndef USE_FLOATING_POINT_DATA
    QPoint xp;
    xp = VPoint2XPoint(vp);
#else
    QPointF xp;
    xp = VPoint2XPointF(vp);
#endif
xlib_setpen();
set_Pen_only();

GeneralPainter->drawPoint(xp);
    ///XDrawPoint(disp, displaybuff, gc, xp.x, xp.y);
}

void xlibdrawpolyline(VPoint *vps, int n, int mode)
{
    int i, xn = n;
#ifndef USE_FLOATING_POINT_DATA
    QPoint *p;
#else
    QPointF *p;
#endif
    if (n <= 1 || getlinestyle() == 0 || getpattern() == 0) {
        return;
    }
    
    if (mode == POLYLINE_CLOSED) {
        xn++;
    }
#ifndef USE_FLOATING_POINT_DATA
    p = new QPoint[xn];
#else
    p = new QPointF[xn];//xmalloc(xn*sizeof(XPoint));
#endif
    if (p == NULL) {
        return;
    }
    
    for (i = 0; i < n; i++) {
#ifndef USE_FLOATING_POINT_DATA
        p[i] = VPoint2XPoint(vps[i]);
#else
        p[i] = VPoint2XPointF(vps[i]);
#endif
    }
    if (mode == POLYLINE_CLOSED) {
        p[n] = p[0];
    }
    
    ///xlib_setdrawbrush();
    set_Pen_only();
if (RotationAngle!=0)
{
#ifndef USE_FLOATING_POINT_DATA
    QPoint po=CenterOfMass(n,p);
#else
    QPointF po=CenterOfMass(n,p);
#endif
set_Rotation_Matrix(po.x(),po.y(),RotationAngle);
    for (i=0;i<xn;i++)
    p[i]-=po;
}
//cout << "Draw Polyline xn=" << xn << endl;
/// GeneralPainter->setRenderHint(QPainter::Antialiasing,true);
    if (xn>10000)
    {
    int todraw=xn,pos=0;
        while (todraw>0)
        {
            if (todraw>10000)
            GeneralPainter->drawPolyline(p+pos,10000);
            else
            GeneralPainter->drawPolyline(p+pos,todraw);
        pos+=9999;
        todraw-=9999;
        }
    }
    else
    GeneralPainter->drawPolyline(p,xn);
/// GeneralPainter->setRenderHint(QPainter::Antialiasing,false);
    if (RotationAngle!=0)
    {
    reset_Transformation_Matrix();
    }
delete[] p;
}

void xlibfillpolygon(VPoint *vps, int npoints)
{
int i;
    if (npoints < 3 || getpattern() == 0) {
        return;
    }
#ifndef USE_FLOATING_POINT_DATA
    QPoint *p = new QPoint[npoints];
#else
    QPointF *p = new QPointF[npoints];
#endif
    if (p == NULL) {
        return;
    }
    
    for (i = 0; i < npoints; i++) {
#ifndef USE_FLOATING_POINT_DATA
        p[i] = VPoint2XPoint(vps[i]);
#else
        p[i] = VPoint2XPointF(vps[i]);
#endif
    }

    xlib_setpen();

    if (getfillrule() != xlibfillrule) {
        xlibfillrule = getfillrule();
        /*if (getfillrule() == FILLRULE_WINDING) {
            ///XSetFillRule(disp, gc, WindingRule);
        } else {
            ///XSetFillRule(disp, gc, EvenOddRule);
        }*/
    }
set_Pen_only();
GeneralPainter->setPen(Qt::NoPen);
set_Brush_only(getcolor(),getbgcolor(),getpattern());
if (RotationAngle!=0)
{
#ifndef USE_FLOATING_POINT_DATA
QPoint po=CenterOfMass(npoints,p);
#else
QPointF po=CenterOfMass(npoints,p);
#endif
set_Rotation_Matrix(po.x(),po.y(),RotationAngle);
//qDebug() << "RotCenter=" << po.x() << "|" << po.y();
    for (i=0;i<npoints;i++)
    p[i]-=po;
}

if (getfillrule() == FILLRULE_WINDING)
GeneralPainter->drawPolygon(p,npoints,Qt::WindingFill);
else
GeneralPainter->drawPolygon(p,npoints,Qt::OddEvenFill);

if (RotationAngle!=0)
{
reset_Transformation_Matrix();
}
delete[] p;
}

/*
 *  xlibdrawarc
 */
void xlibdrawarc(VPoint vp1, VPoint vp2, int angle1, int angle2)
{
    int x1, y1, x2, y2;
    
    xlibVPoint2dev(vp1, &x1, &y2);
    xlibVPoint2dev(vp2, &x2, &y1);

    if (getlinestyle() == 0 || getpattern() == 0)
    {
    return;
    }
	
    xlib_setdrawbrush();///Brush sets only the outline
        set_Pen_only();
    GeneralPainter->setBrush(Qt::NoBrush);

    if (x1 != x2 || y1 != y2)
    {
    set_Rotation_Matrix((x1+x2)*0.5,(y1+y2)*0.5,RotationAngle);
        /// GeneralPainter->drawPie(MIN2(x1, x2), MIN2(y1, y2),abs(x2 - x1), abs(y2 - y1),16*angle1,16*(angle2-angle1));
    //GeneralPainter->drawArc(MIN2(x1, x2), MIN2(y1, y2),abs(x2 - x1), abs(y2 - y1),16*angle1,16*(angle2-angle1));
    GeneralPainter->drawArc(-abs(x2 - x1)*0.5,-abs(y2 - y1)*0.5,abs(x2 - x1), abs(y2 - y1),16*angle1,16*(angle2-angle1));
        /// GeneralPainter->drawEllipse(MIN2(x1, x2), MIN2(y1, y2),abs(x2 - x1), abs(y2 - y1));
        /// XDrawArc(disp, displaybuff, gc, MIN2(x1, x2), MIN2(y1, y2),
        /// abs(x2 - x1), abs(y2 - y1), 64 * angle1, 64 * (angle2 - angle1));
    reset_Transformation_Matrix();
    }
    else
    { /* zero radius */
    QBrush br=GeneralPainter->brush();
    QPen pe=GeneralPainter->pen();
        GeneralPainter->setPen(Qt::NoPen);
        GeneralPainter->setBrush(pe.color());
    GeneralPainter->drawRect(x1,y1,1,1);
        GeneralPainter->setPen(pe);
        GeneralPainter->setBrush(br);
    //GeneralPainter->drawPoint(x1,y1);
    }
}

/*
 *  xlibfillarc
 */
void xlibfillarc(VPoint vp1, VPoint vp2, int angle1, int angle2, int mode)
{
    int x1, y1, x2, y2;
    bool pie=true;
    xlibVPoint2dev(vp1, &x1, &y2);
    xlibVPoint2dev(vp2, &x2, &y1);
    
    if (getpattern() != 0) {
        xlib_setpen();///Pen sets outline and filling
        if (x1 != x2 || y1 != y2) {
            //if (xlibarcfillmode != mode) {
                xlibarcfillmode = mode;
                if (mode == ARCFILL_CHORD) {
			pie=false;
                    ///XSetArcMode(disp, gc, ArcChord);
                } else {
			pie=true;
                    ///XSetArcMode(disp, gc, ArcPieSlice);
                }
            //}
set_Pen_only();
GeneralPainter->setPen(Qt::NoPen);
set_Brush_only(getcolor(),getbgcolor(),getpattern());
set_Rotation_Matrix((x1+x2)*0.5,(y1+y2)*0.5,RotationAngle);
if (pie)
{
//GeneralPainter->drawPie(MIN2(x1, x2), MIN2(y1, y2),abs(x2 - x1), abs(y2 - y1),16*angle1,16*(angle2-angle1));
GeneralPainter->drawPie(-abs(x2 - x1)*0.5,-abs(y2 - y1)*0.5,abs(x2 - x1), abs(y2 - y1),16*angle1,16*(angle2-angle1));
}
else
{
//GeneralPainter->drawChord(MIN2(x1, x2), MIN2(y1, y2),abs(x2 - x1), abs(y2 - y1),16*angle1,16*(angle2-angle1));
GeneralPainter->drawChord(-abs(x2 - x1)*0.5,-abs(y2 - y1)*0.5,abs(x2 - x1), abs(y2 - y1),16*angle1,16*(angle2-angle1));
}
//GeneralPainter->drawEllipse(MIN2(x1, x2), MIN2(y1, y2),abs(x2 - x1), abs(y2 - y1));
            ///XFillArc(disp, displaybuff, gc, MIN2(x1, x2), MIN2(y1, y2),
            ///   abs(x2 - x1), abs(y2 - y1), 64 * angle1, 64 * (angle2 - angle1));
        reset_Transformation_Matrix();
        } else { /* zero radius */
        GeneralPainter->drawRect(x1,y1,1,1);
        //GeneralPainter->drawPoint(x1,y1);
        }
    }
}

/*void xlibputpixmap(VPoint vp, int width, int height,
     char *databits, int pixmap_bpp, int bitmap_pad, int pixmap_type)
{
    int j, k, l;
    QPoint xp;
QPen linePen;
RGB * rgbColor;
QPixmap clipmask(width,height);
clipmask.fill();
QPainter clp_painter(&clipmask);
int x1,y1;//real_world_coordinates
xlibVPoint2dev(vp, &x1, &y1);

//TEST BEGIN
//cout << "X11-PutPixmap:" << endl;
//for (int i=0;i<height;i++)
//{
//    for (j=0;j<width;j++)
//    {
//    cout << (unsigned int)((unsigned char)(databits[i*width+j])) << "\t";
//    }
//    cout << endl;
//}
//TEST END

    char *pixmap_ptr;
    char *clipmask_ptr = NULL;
    
    int line_off;

    int cindex, fg, bg;
    
    xp = VPoint2XPoint(vp);
//cout << "pixmap_bpp=" << pixmap_bpp << endl;
    if (pixmap_bpp != 1)
    {
        if (monomode == TRUE)
        {
            // TODO: dither pixmaps on mono displays
            return;
        }
        pixmap_ptr = (char *)xcalloc(PAD(width, 8) * height, pixel_size);
        if (pixmap_ptr == NULL)
        {
            errmsg("xmalloc failed in xlibputpixmap()");
            return;
        }
        // re-index pixmap
        for (k = 0; k < height; k++)
        {
            for (j = 0; j < width; j++)
            {
                cindex = (unsigned char) (databits)[k*width+j];
                for (l = 0; l < pixel_size; l++)
                {
                    pixmap_ptr[pixel_size*(k*width+j) + l] =
                                        (char) (xvlibcolors[cindex] >> (8*l));
                }
            }
        }
//cout << "pixel_size=" << pixel_size << endl;
        //for (int i=0;i<height;i++)
        //{
        //	for (int j=0;j<width;j++)
        //	{
        //		if (pixmap_ptr[i*width+j])
        //		clp_painter.drawPoint(j,i);
        //	}
        //}
//cout << "pixmap_bpp="<< pixmap_bpp << endl;
        ///ximage=XCreateImage(disp, visual,
        ///                   depth, ZPixmap, 0, pixmap_ptr, width, height,
        ///                   bitmap_pad,  // lines padded to bytes
        ///                   0 // number of bytes per line
        ///                   );

        if (pixmap_type == PIXMAP_TRANSPARENT) {
            clipmask_ptr =(char*) xcalloc((PAD(width, 8)>>3)* height, sizeof(char));
            if (clipmask_ptr == NULL) {
                errmsg("xmalloc failed in xlibputpixmap()");
                return;
            } else {
                // Note: We pad the clipmask always to byte boundary
                bg = getbgcolor();
                for (k = 0; k < height; k++) {
                    line_off = k*(PAD(width, 8) >> 3);
                    for (j = 0; j < width; j++) {
                        cindex = (unsigned char) (databits)[k*width+j];
                        if (cindex != bg) {
				//cout << "cindex=" << (int)cindex << endl;
	rgbColor=get_rgb((int)cindex);
	linePen=QPen(QColor(rgbColor->red,rgbColor->green,rgbColor->blue));
	GeneralPainter->setPen(linePen);
				clp_painter.drawPoint(j,k);
	GeneralPainter->drawPoint(j+x1,k+y1);
                            clipmask_ptr[line_off+(j>>3)] |= (0x01 << (j%8));
                        }
                    }
                }
                ///clipmask=XCreateBitmapFromData(disp, root, clipmask_ptr, width, height);
                xfree(clipmask_ptr);
            }
        }
    } else {///pixmap_bpp == 1
        pixmap_ptr = (char*)xcalloc((PAD(width, bitmap_pad)>>3) * height,sizeof(char));
        if (pixmap_ptr == NULL) {
            errmsg("xmalloc failed in xlibputpixmap()");
            return;
        }
        memcpy(pixmap_ptr, databits, ((PAD(width, bitmap_pad)>>3) * height));

        fg = getcolor();
        if (fg != xlibcolor) {
            ///XSetForeground(disp, gc, xvlibcolors[fg]);
            xlibcolor = fg;
        }

        bg = getbgcolor();
        rgbColor=get_rgb((int)fg);
        linePen=QPen(QColor(rgbColor->red,rgbColor->green,rgbColor->blue));
        GeneralPainter->setPen(linePen);

        char one_byte,probe_byte;
        int b_index,n_index=ceil(width/8.0)*8;

//cout << "width=" << width << " n_index=" << n_index << " bitmap_pad=" << bitmap_pad << endl;

        for (k = 0; k < height; k++)
        {
        //line_off = k*(PAD(width, 8) >> 3);
            for (j = 0; j < width; j++)
            {
                b_index=(n_index*k+j)/bitmap_pad;
                probe_byte=-b_index*bitmap_pad+(n_index*k+j);
//cout << "k=" << k << " j=" << j << " b_index=" << b_index << " probe=" << (int)probe_byte << endl;
                one_byte=1<<probe_byte;
                probe_byte=((unsigned char) (databits)[b_index])&one_byte;
                //one_byte=probe_byte?1:0;
                //cindex = (unsigned char) (databits)[k*width+j];
                //if ((unsigned char) (databits)[k*width+j]) cindex=fg;
                //else cindex=bg;
                //if (cindex != bg) {
                if (probe_byte)
                {
        //cout << "cindex=" << (int)cindex << endl;

        clp_painter.drawPoint(j,k);
GeneralPainter->drawPoint(j+x1,k+y1);
                    //clipmask_ptr[line_off+(j>>3)] |= (0x01 << (j%8));
                }
            }
        }

        ///ximage=XCreateImage(disp, visual,
        ///                    1, XYBitmap, 0, pixmap_ptr, width, height,
        ///                    bitmap_pad, // lines padded to bytes
        ///                    0 // number of bytes per line
        ///                    );
        if (pixmap_type == PIXMAP_TRANSPARENT) {
            ///clipmask=XCreateBitmapFromData(disp, root, pixmap_ptr, PAD(width, bitmap_pad), height);
        }
    }

    if (pixmap_type == PIXMAP_TRANSPARENT) {
        ///XSetClipMask(disp, gc, clipmask);
       /// XSetClipOrigin(disp, gc, xp.x, xp.y);
    }
        
        
    // Force bit and byte order
    ///ximage->bitmap_bit_order=LSBFirst;
    ///ximage->byte_order=LSBFirst;
    
    ///XPutImage(disp, displaybuff, gc, ximage, 0, 0, xp.x, xp.y, width, height);
    ///XDestroyImage(ximage);

clp_painter.end();
//GeneralPainter->drawPixmap(x1,y1,clipmask);

    if (pixmap_type == PIXMAP_TRANSPARENT) {
        ///XFreePixmap(disp, clipmask);
        ///clipmask = 0;
        ///XSetClipMask(disp, gc, None);
        ///XSetClipOrigin(disp, gc, 0, 0);
    }    
}*/

//simplified version of xlibputpixmap
void xlibputpixmap(VPoint vp, int width, int height,
     char *databits, int pixmap_bpp, int bitmap_pad, int pixmap_type)
{
int j, k, alpha=getalpha();
QPoint xp;
QPen linePen;
RGB * rgbColor;
QColor col;
QBrush br=GeneralPainter->brush();
int x1,y1;//real_world_coordinates
xlibVPoint2dev(vp, &x1, &y1);

//TEST BEGIN
//cout << "X11-PutPixmap:" << endl;
//for (int i=0;i<height;i++)
//{
//    for (j=0;j<width;j++)
//    {
//    cout << (unsigned int)((unsigned char)(databits[i*width+j])) << "\t";
//    }
//    cout << endl;
//}
//TEST END

//int line_off;
int cindex, fg, bg;

    xp = VPoint2XPoint(vp);
//cout << "pixmap_bpp=" << pixmap_bpp << endl;
    if (pixmap_bpp != 1)
    {
        if (monomode == TRUE)//monomode=black/white - no gray and no colors
        {
            // TODO: dither pixmaps on mono displays
            return;
        }

        if (pixmap_type == PIXMAP_TRANSPARENT)
        {
        bg = getbgcolor();
                for (k = 0; k < height; k++)
                {
                //line_off = k*(PAD(width, 8) >> 3);
                    for (j = 0; j < width; j++)
                    {
                        cindex = (unsigned char) (databits)[k*width+j];
                        if (cindex != bg)
                        {
                //cout << "cindex=" << (int)cindex << endl;
    rgbColor=get_rgb((int)cindex);
    col=QColor(rgbColor->red,rgbColor->green,rgbColor->blue,alpha);
    linePen=QPen(col);
    ///GeneralPainter->setPen(linePen);
    GeneralPainter->setPen(Qt::NoPen);
    GeneralPainter->setBrush(col);
                        //GeneralPainter->drawPoint(j+x1,k+y1);
                        GeneralPainter->drawRect(j+x1,k+y1,1,1);
                        }
                    }
                }

        }
    }
    else
    {///pixmap_bpp == 1
        fg = getcolor();
        if (fg != xlibcolor)
        {
            xlibcolor = fg;
        }

        bg = getbgcolor();
        rgbColor=get_rgb((int)fg);
        col=QColor(rgbColor->red,rgbColor->green,rgbColor->blue,alpha);
        linePen=QPen(col);
        ///GeneralPainter->setPen(linePen);
        GeneralPainter->setPen(Qt::NoPen);
        GeneralPainter->setBrush(col);

        char one_byte,probe_byte;
        int b_index,n_index=ceil(width/8.0)*8;

//cout << "width=" << width << " n_index=" << n_index << " bitmap_pad=" << bitmap_pad << endl;

        for (k = 0; k < height; k++)
        {
            for (j = 0; j < width; j++)
            {
                b_index=(n_index*k+j)/bitmap_pad;
                probe_byte=-b_index*bitmap_pad+(n_index*k+j);
//cout << "k=" << k << " j=" << j << " b_index=" << b_index << " probe=" << (int)probe_byte << endl;
                one_byte=1<<probe_byte;
                probe_byte=((unsigned char) (databits)[b_index])&one_byte;
                if (probe_byte)
                {
                //GeneralPainter->drawPoint(j+x1,k+y1);
                GeneralPainter->drawRect(j+x1,k+y1,1,1);
                }
            }
        }
    }
GeneralPainter->setBrush(br);
}

void xlibleavegraphics(void)
{
    /// focus indicators are drawn in the mainArea-paint-event
    /*int cg = get_cg();
    if (is_graph_hidden(cg) == FALSE) {
        draw_focus(cg);
    }*/
    reset_crosshair();
}

QColor get_Color(int i)
{
RGB * rgb_color=get_rgb(i);
return QColor(rgb_color->red,rgb_color->green,rgb_color->blue);
}

void set_Brush_only(int fg,int bg,int p)
{
    (void)bg;
static QBrush brush;
static QColor col;
brush=GeneralPainter->brush();
col=get_Color(fg);
col.setAlpha(getalpha());
brush.setColor(col);

if (p==0)
{
GeneralPainter->setBrush(Qt::NoBrush);
return;
}
else if (p==1)
{
brush.setStyle(Qt::SolidPattern);
GeneralPainter->setBrush(brush);
return;
}
else
{
//QPixmap pic(16,16);
QBitmap bitm(*(patterns[p]));
/*brush.setTexture(*(patterns[p]));
pic.fill(get_Color(bg));
QPainter paintr(&pic);
paintr.setBrush(brush);
paintr.setPen(Qt::NoPen);
paintr.drawRect(0,0,16,16);
paintr.end();
brush.setTexture(pic);*/
brush.setTexture(bitm);
GeneralPainter->setBrush(brush);
return;
}
}

void set_Pen_only(void)
{
static QPen tmpPen;
static QColor col;
tmpPen=GeneralPainter->pen();
col=get_Color(draw_props.pen.color);
if (show_transparency_selector>0)
col.setAlpha(draw_props.pen.alpha);
//tmpPen.setColor(get_Color(draw_props.pen.color));
tmpPen.setColor(col);

if (current_dpi!=72 && ScaleLineWidthByResolution)// && printing_in_file==true)/// removed the second condition
tmpPen.setWidthF(qreal(draw_props.linew*current_dpi/72.0));
else
tmpPen.setWidthF(qreal(draw_props.linew));

switch (xliblinecap)
{
case LINECAP_BUTT:
tmpPen.setCapStyle(Qt::SquareCap);
break;
case LINECAP_ROUND:
tmpPen.setCapStyle(Qt::RoundCap);
break;
case LINECAP_PROJ:
tmpPen.setCapStyle(Qt::FlatCap);
break;
}
switch (xliblinejoin)
{
case LINEJOIN_MITER:
tmpPen.setJoinStyle(Qt::MiterJoin);
break;
case LINEJOIN_ROUND:
tmpPen.setJoinStyle(Qt::RoundJoin);
break;
case LINEJOIN_BEVEL:
tmpPen.setJoinStyle(Qt::BevelJoin);
break;
}
if (xliblinestyle==0)
tmpPen.setStyle(Qt::NoPen);
else if (draw_props.lines==1)
tmpPen.setStyle(Qt::SolidLine);
else
{
tmpPen.setStyle(Qt::CustomDashLine);
tmpPen.setDashPattern(*PenDashPattern[draw_props.lines]);
GeneralPainter->setBackground(Qt::transparent);
}

if (draw_props.pen.pattern>1)//0=none, 1=solid, >1 special filling pattern
{
/*
QBrush tmpBrush;
tmpBrush.setTexture(patterns[draw_props.pen.pattern]->copy(0,0,16,16));
tmpBrush.setColor(tmpPen.color());
QPixmap pic(16,16);
pic.fill(get_Color(getbgcolor()));
QPainter paintr(&pic);
paintr.setBrush(tmpBrush);
paintr.setPen(Qt::NoPen);
paintr.drawRect(0,0,16,16);
paintr.end();
tmpBrush.setTexture(pic);
tmpPen.setBrush(tmpBrush);
*/
QPixmap pixm;
generate_Pixmap_from_Bits(pat_bits[draw_props.pen.pattern],32,16,16,&pixm,draw_props.pen.color);
tmpPen.setBrush(pixm);
//tmpPen.setBrush(QPixmap(QBitmap(*(patterns[draw_props.pen.pattern]))));
}
//Make graphs visible, when they are over each other)
//GeneralPainter->setBackgroundMode(Qt::TransparentMode);
GeneralPainter->setPen(tmpPen);
}

void set_Transformation_Matrix(TextMatrix *tm,double shift_x,double shift_y)
{
double size=sqrt(tm->cxx*tm->cxx+tm->cyx*tm->cyx);
//QTransform trans(tm->cxx/MAGIC_FONT_SCALE,-tm->cyx/MAGIC_FONT_SCALE,-tm->cxy/MAGIC_FONT_SCALE,tm->cyy/MAGIC_FONT_SCALE,shift_x,shift_y);
QTransform trans(tm->cxx/size,-tm->cyx/size,-tm->cxy/size,tm->cyy/size,shift_x,shift_y);
GeneralPainter->setTransform(trans);
}

void reset_Transformation_Matrix(void)
{
QTransform trans(1.0,0.0,0.0,1.0,0.0,0.0);//identity-matrix
GeneralPainter->setTransform(trans);
}

void set_Rotation_Matrix(double x_centre,double y_centre,int rot)
{
QTransform trans(1.0,0.0,0.0,1.0,0.0,0.0);
trans.translate(x_centre,y_centre);
trans.rotate(rot*1.0);
GeneralPainter->setTransform(trans);
}

void ShiftQtChar(QChar & c)//shifts according to the qtCharShift
{
if (qtCharShift==CHAR_SHIFT_NONE) return;//nothing to do
ushort ascii=c.unicode();
//cout << "ascii=" << ascii << endl;
ushort new_ascii;
if (!((ascii>=('!') && ascii<=('~')) || (ascii-128>=('!'-1) && ascii-128<=('~')))) return;//we do not shift in this range
//cout << "ShiftQtChar=" << c.toLatin1() << " Shift=" << qtCharShift << endl;
if (qtCharShift==CHAR_SHIFT_TO_SYMBOL)//only shift to symbol
{
    new_ascii=key_for_greek.value(ascii,ascii);
    //if (ascii==1) new_ascii=ascii;
}
else if (qtCharShift==CHAR_SHIFT_TO_UPPERSET)//only shift to upper char-set
{
    new_ascii=key_for_greek.value(ascii+128,ascii+128);
    //if (ascii==1) new_ascii=ascii+128;
}
else if (qtCharShift==(CHAR_SHIFT_TO_SYMBOL|CHAR_SHIFT_TO_UPPERSET))//shift to symbol and to the upper set
{
    new_ascii=key_for_greek.value(ascii+128,ascii+128);
    //if (ascii==1) new_ascii=ascii+128;
}
else
{
new_ascii=ascii;
}
c=QChar(new_ascii);
}

void ShiftQtString(QString & string,unsigned int shift)
{
    (void)shift;
QString empty;
QChar tmp;
//ushort unicode;
empty.clear();
    for (int i=0;i<string.length();i++)
    {
    tmp=string.at(i);
    /*unicode=tmp.unicode()+shift;
    empty.append(QChar(unicode));*/
    ShiftQtChar(tmp);
    empty.append(tmp);
    }
string=empty;
}

void WriteQtString(VPoint vp,int rot,int just,char * s,double charSize,int font,int color,int alpha=255)
{
static QFont dFont;
static unsigned long smaller;
static Page_geometry cur_pg;
static int cx,cy;
static int fm_w,fm_asc,fm_dsc;//fm_h
static double shift_x,shift_y;
static QString TextToPrint;
static VPoint vp1,vp2,vp3,vp4,bbox_ll,bbox_ur;
static double ca,sa;
static QColor col;
ca=cos(rot/180.0*3.1415927);
sa=sin(rot/180.0*3.1415927);

TextToPrint=QString::fromUtf8(s);
///starting with v0.2.7 only UTF-8 is used
//TextToPrint=FileCodec->toUnicode(s);

/*if (strlen(s)!=TextToPrint.length())
{
cout << "alte und neue Laengen stimmen nicht: " << strlen(s) << "<-->" << TextToPrint.length() << endl;
cout << s << endl;
qDebug(TextToPrint.toLocal8Bit().constData());
}*/

//qDebug() << "Original=#" << s << "# printing=#" << TextToPrint.toLocal8Bit().constData() << "#";

if (font==get_QtFontID_from_Grace_Name("Symbol",0) && symbol_font_is_special==true)
{
//cout << "Font ist Symbol --> shifting" << endl;
if (qtCharShift==CHAR_SHIFT_NONE) qtCharShift|=CHAR_SHIFT_TO_SYMBOL;
if (qtCharShift==128)
ShiftQtString(TextToPrint,unicode_greek_shift+128);
else
ShiftQtString(TextToPrint,unicode_greek_shift);
//cout << "shifted=#" << TextToPrint.toLocal8Bit().constData() << "#" << endl;
}

/*
if (qtCharShift!=0)
{
ShiftQtString(TextToPrint,qtCharShift);
}
*/

shift_x=shift_y=0.0;
xlibVPoint2dev(vp,&cx,&cy);

cur_pg=get_page_geometry();
smaller=cur_pg.height<cur_pg.width?cur_pg.height:cur_pg.width;
dFont=getFontFromDatabase(font);
//cout << "gefundener Font=" << dFont.toString().toLatin1().constData() << endl;

dFont.setPixelSize(charSize*smaller*MAGIC_FONT_SCALE);
dFont.setOverline(tmp_overline);
dFont.setUnderline(tmp_underline);
dFont.setKerning(tmp_kerning);

    GeneralPainter->setBrush(Qt::transparent);/// NoBrush
    GeneralPainter->setBackground(Qt::transparent);/// NoBrush
    col=get_Color(color);
    col.setAlpha(alpha);
    GeneralPainter->setPen(col);
    GeneralPainter->setFont(dFont);

QFontMetrics fm=GeneralPainter->fontMetrics();
//fm_w=fm.width(TextToPrint);//deprecated
fm_w=fm.horizontalAdvance(TextToPrint);
//fm_h=fm.height();
fm_asc=fm.ascent();
fm_dsc=fm.descent();

//cout << "Height=" << fm_h << " Width=" << fm_w << " asc=" << fm_asc << " dsc=" << fm_dsc << endl;
//QRect rec=fm.boundingRect(TextToPrint);
//cout << "rec.height=" << rec.height() << " rec.width=" << rec.width() << endl;

    if ((just & 03) == JUST_CENTER)
    {
    shift_x=-fm_w*0.5;
    }
    else if ((just & 03) == JUST_RIGHT)
    {
    shift_x=-fm_w;
    }
    //JUST_LEFT == 0 == default

    if ((just & 014) == JUST_MIDDLE)
    {
    shift_y=fm_asc*0.5;//fm_h/2-fm_dsc;
    }
    else if ((just & 014) == JUST_BOTTOM)
    {
    shift_y=-fm_dsc;
    }
    else if ((just & 014) == JUST_TOP)
    {
    shift_y=fm_asc;//fm_h-fm_dsc;
    }
    //JUST_BLINE == 0 == default

/*cx+=shift_x;
cy+=shift_y;*/

//set_Rotation_Matrix(cx,cy,-rot);
//GeneralPainter->drawText(shift_x,shift_y,TextToPrint);

    QRect fm_rect=fm.boundingRect(TextToPrint);
    vp1=xlibdev2VPoint(cx,cy);

/// Perform rotation --> Mind Baseline-shift!!! Mind shift_x and shift_y

    bbox_ll.x=fm_rect.width()*ca-fm_rect.height()*sa;
    bbox_ll.y=fm_rect.width()*sa+fm_rect.height()*ca;
    vp2=xlibdev2VPoint(cx+bbox_ll.x,cy-bbox_ll.y);
    bbox_ll.x=0.0*ca-fm_rect.height()*sa;
    bbox_ll.y=0.0*sa+fm_rect.height()*ca;
    vp3=xlibdev2VPoint(cx+bbox_ll.x,cy-bbox_ll.y);
    bbox_ll.x=fm_rect.width()*ca-0.0*sa;
    bbox_ll.y=fm_rect.width()*sa+0.0*ca;
    vp4=xlibdev2VPoint(cx+bbox_ll.x,cy-bbox_ll.y);

    /*vp1.x=bbox_ll.x;
    vp1.y=bbox_ll.y;
    bbox_ll.x=vp2.x*ca-vp2.y*sa;
    bbox_ll.y=vp2.x*sa+vp2.y*ca;*/

    /*vp2.x+=vp1.x;
    vp2.y+=vp1.y;
        vp3.x+=vp1.x;
        vp3.y+=vp1.y;
    vp4.x+=vp1.x;
    vp4.y+=vp1.y;*/

    //test
    bbox_ll.x=vp1.x<vp2.x?vp1.x:vp2.x;
    bbox_ll.x=bbox_ll.x<vp3.x?bbox_ll.x:vp3.x;
    bbox_ll.x=bbox_ll.x<vp4.x?bbox_ll.x:vp4.x;
    bbox_ll.y=vp1.y<vp2.y?vp1.y:vp2.y;
    bbox_ll.y=bbox_ll.y<vp3.y?bbox_ll.y:vp3.y;
    bbox_ll.y=bbox_ll.y<vp4.y?bbox_ll.y:vp4.y;

    bbox_ur.x=vp1.x>vp2.x?vp1.x:vp2.x;
    bbox_ur.x=bbox_ur.x>vp3.x?bbox_ur.x:vp3.x;
    bbox_ur.x=bbox_ur.x>vp4.x?bbox_ur.x:vp4.x;
    bbox_ur.y=vp1.y>vp2.y?vp1.y:vp2.y;
    bbox_ur.y=bbox_ur.y>vp3.y?bbox_ur.y:vp3.y;
    bbox_ur.y=bbox_ur.y>vp4.y?bbox_ur.y:vp4.y;

update_bboxes(bbox_ll);
update_bboxes(bbox_ur);

    set_Rotation_Matrix(cx+shift_x,cy+shift_y,-rot);

    set_Transformation_Matrix(tmp_tm,cx+shift_x,cy+shift_y);
//cout << "t=" << tmp_tm->cxx << "|" << tmp_tm->cxy << "|" << tmp_tm->cyx << "|" << tmp_tm->cyy << endl;
    //set_Rotation_Matrix(cx,cy,-rot);
//qDebug() << "Writing Text=" << TextToPrint;
    GeneralPainter->drawText(0,0,TextToPrint);
    //GeneralPainter->drawText(fm_rect,currentStringAlignment,TextToPrint);
    /*
#ifdef DEBUG_SHOW_TEXT_BOUNDING_BOXES
    GeneralPainter->drawRect(fm_rect);
#endif
    */
dFont.setOverline(false);
dFont.setUnderline(false);
dFont.setKerning(true);
    GeneralPainter->setFont(dFont);
reset_Transformation_Matrix();
}

void qtPutText(VPoint vp, char *s, int len, int font,TextMatrix *tm, int underline, int overline, int kerning)
{
    (void)len;
    if (s==NULL)
    {
    //cout << get_docname() << " Warning: s=NULL" << endl;
    return;
    }
int rot=180.0/3.1415927*atan(tm->cyx/tm->cyy);
if (tm->cxx<0.0) rot+=180;
double size=sqrt(tm->cxx*tm->cxx+tm->cyx*tm->cyx)/MAGIC_FONT_SCALE;
/// s[len]='\0'; /// HIER IST DAS PROBLEM!!
/// cout << "s=#" << s << "# len=" << len << endl;
tmp_underline=bool(underline);
tmp_overline=bool(overline);
tmp_kerning=bool(kerning);
tmp_tm=tm;
//qDebug() << "about to draw #" << s << "# alpha=" << draw_props.pen.alpha << endl;
    if (show_transparency_selector>0)
    WriteQtString(vp,rot,0,s,size,font,draw_props.pen.color,draw_props.pen.alpha);
    else
    WriteQtString(vp,rot,0,s,size,font,draw_props.pen.color,255);
//cout << "Text to Print=#" << s << "# size=" << size << " draw_props.charsize=" << draw_props.charsize << endl;
tmp_underline=false;
tmp_overline=false;
tmp_kerning=true;
}

void WriteQtString(VPoint vp,int rot,int just,char * s)
{
    if (show_transparency_selector>0)
    WriteQtString(vp,rot,just,s,draw_props.charsize,draw_props.font,draw_props.pen.color,draw_props.pen.alpha);
    else
    WriteQtString(vp,rot,just,s,draw_props.charsize,draw_props.font,draw_props.pen.color,255);
}

inline void matrix_mult(double ca,double sa,double x,double y,double & xs,double & ys)
{
xs=ca*x-sa*y;
ys=sa*x+ca*y;
}

GLYPH *GetQtGlyph(CompositeString *cs, double dpv, int fontaa)
{
static GLYPH string_glyph={NULL,{0,0,0,0,0,0},NULL,1};
double rot=atan(cs->tm.cyx/cs->tm.cyy);// *180.0/3.1415927
if (cs->tm.cxx<0.0) rot+=M_PI;
double ca=cos(rot),sa=sin(rot);
double size=sqrt(cs->tm.cxx*cs->tm.cxx+cs->tm.cyx*cs->tm.cyx);///MAGIC_FONT_SCALE;
char * dummy=new char[cs->len+8];
    (void)dpv;
/*cout << "first values=";
    for (int i=0;i<8;i++)
    cout << (unsigned int)(cs->s[i]) << " ";
cout << endl;*/
//cs->s[cs->len]='\0';
strncpy(dummy,cs->s,cs->len);
dummy[cs->len]='\0';
    /*for (int i=0;i<cs->len;i++)
    {
        //if (cs->s[i]>255) cout << "Fehler!" << endl;
        cout << (int)((unsigned char)(cs->s[i])) << " ";
    }
    cout << endl;*/
//cout << "dummy, len=" << cs->len << endl << " cs.s=#" << cs->s << "# dummy=#" << dummy << "#" << endl;
Page_geometry cur_pg=get_page_geometry();
//QString TextToPrint=FileCodec->toUnicode(dummy);
QString TextToPrint=QString::fromUtf8(dummy);
    if (TextToPrint.isEmpty() || TextToPrint.length()<1)// cout << "Fehler kommt sicher!" << endl;
    return (&string_glyph);
//cout << "GetQtGlyph of =" << TextToPrint.toAscii().constData() << endl;
int smaller=cur_pg.height<cur_pg.width?cur_pg.height:cur_pg.width;
QFont dFont=getFontFromDatabase(cs->font);
dFont.setKerning(bool(cs->kerning));
dFont.setUnderline(bool(cs->underline));
dFont.setOverline(bool(cs->overline));
//cout << "QtGlyph: fontaa=" << fontaa << endl;
    if (fontaa==false)
    dFont.setStyleStrategy(QFont::NoAntialias);
    else
    dFont.setStyleStrategy(QFont::PreferAntialias);
dFont.setPixelSize(size*smaller);//*MAGIC_FONT_SCALE);
dFont.setOverline(cs->overline);
dFont.setUnderline(cs->underline);
dFont.setKerning(cs->kerning);
GeneralPainter->setFont(dFont);
    QFontMetrics fm=GeneralPainter->fontMetrics();
//int fm_w=fm.width(TextToPrint);//deprecated
int fm_w=fm.horizontalAdvance(TextToPrint);
//int fm_h=fm.height();
double as,ds,lb,rb;
double tmp_x,tmp_y;

as=fm.ascent();
ds=fm.descent();
lb=fm.leftBearing(TextToPrint.at(0));
rb=fm_w-fm.rightBearing(TextToPrint.at(TextToPrint.length()-1));
//rb=fm_w-lb;

string_glyph.metrics.advanceX=fm_w*ca;
string_glyph.metrics.advanceY=fm_w*sa;

matrix_mult(ca,sa,-lb,-ds,tmp_x,tmp_y);
    string_glyph.metrics.leftSideBearing=string_glyph.metrics.rightSideBearing=tmp_x;
    string_glyph.metrics.ascent=string_glyph.metrics.descent=tmp_y;
matrix_mult(ca,sa,rb,-ds,tmp_x,tmp_y);
    if (tmp_x<string_glyph.metrics.leftSideBearing) string_glyph.metrics.leftSideBearing=tmp_x;
    if (tmp_x>string_glyph.metrics.rightSideBearing) string_glyph.metrics.rightSideBearing=tmp_x;
    if (tmp_y<string_glyph.metrics.descent) string_glyph.metrics.descent=tmp_y;
    if (tmp_y>string_glyph.metrics.ascent) string_glyph.metrics.ascent=tmp_y;
matrix_mult(ca,sa,rb,as,tmp_x,tmp_y);
    if (tmp_x<string_glyph.metrics.leftSideBearing) string_glyph.metrics.leftSideBearing=tmp_x;
    if (tmp_x>string_glyph.metrics.rightSideBearing) string_glyph.metrics.rightSideBearing=tmp_x;
    if (tmp_y<string_glyph.metrics.descent) string_glyph.metrics.descent=tmp_y;
    if (tmp_y>string_glyph.metrics.ascent) string_glyph.metrics.ascent=tmp_y;
matrix_mult(ca,sa,-lb,as,tmp_x,tmp_y);
    if (tmp_x<string_glyph.metrics.leftSideBearing) string_glyph.metrics.leftSideBearing=tmp_x;
    if (tmp_x>string_glyph.metrics.rightSideBearing) string_glyph.metrics.rightSideBearing=tmp_x;
    if (tmp_y<string_glyph.metrics.descent) string_glyph.metrics.descent=tmp_y;
    if (tmp_y>string_glyph.metrics.ascent) string_glyph.metrics.ascent=tmp_y;

return (&string_glyph);
}
