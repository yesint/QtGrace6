/*
 * Grace - GRaphing, Advanced Computation and Exploration of data
 *
 * Home page: http://plasma-gate.weizmann.ac.il/Grace/
 *
 * Copyright (c) 1996-2004 Grace Development Team
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
 * Grace PDF driver
 */
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include "defines.h"
#include "device.h"
#include "devlist.h"
#include "globals.h"
#include "utils.h"
#include "cmath.h"
#include "draw.h"
#include "graphs.h"
//#include "patterns.h"
#include "svgdrv.h"
#include "external_libs.h"
#include "pdfdrv.h"
#include "QObject"
#include <QDebug>
#include <QFile>

using namespace std;

static Device_entry dev_pdf = {
    DEVICE_FILE,
    "PDF",
    svginitgraphics,
    NULL,
    NULL,
    "pdf",
    TRUE,
    TRUE,
    {DEFAULT_PAGE_WIDTH, DEFAULT_PAGE_HEIGHT, 72},
    NULL,
    1,
    ""
};

int register_pdf_drv(void)
{
    return register_device(dev_pdf);//this is the picture-in-pdf-generator
}

static unsigned long page_scale;
static float pixel_size;
static float page_scalef;

/*static int *pdf_font_ids;
static int *pdf_pattern_ids;*/

static int pdf_color;
static int pdf_pattern;
static double pdf_linew;
static int pdf_lines;
static int pdf_linecap;
static int pdf_linejoin;
static int pdf_setup_pdfpattern = FALSE;

#ifdef HAVE_LIBPDF

#include <cmath.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

#include <pdflib.h>

#include "utils.h"
#include "draw.h"


#include "patterns.h"


#include "protos.h"

#ifndef NONE_GUI
#  include "motifinc.h"
#endif

static void pdf_error_handler(PDF *p, int type, const char* msg);


static int pdf_setup_pdf1_3 = TRUE;
static int pdf_setup_compression = 4;

extern FILE *prstream;

static PDF *phandle;

static Device_entry dev_pdf = {DEVICE_FILE,
                               "PDF",
                               pdfinitgraphics,
                               pdf_op_parser,
                               pdf_gui_setup,
                               "pdf",
                               TRUE,
                               FALSE,
                               {3300, 2550, 300.0},
                               NULL
                              };

int register_pdf_drv(void)
{
    PDF_boot();
    return register_device(dev_pdf);
}

/* Define Patterns for subsequent calls to PDF_setcolor(). */
void pdfinitpatterns(void)
{
    int i;
    
    if (pdf_setup_pdfpattern) {
        pdf_pattern_ids = xmalloc(number_of_patterns()*SIZEOF_INT);
        for (i = 1; i < number_of_patterns(); i++) {
            int j, k, l;
            pdf_pattern_ids[i] = PDF_begin_pattern(phandle, 16, 16, 16, 16, 2);
            for (j = 0; j < 256; j++) {
                k = j%16;
                l = 15 - j/16;
                if ((pat_bits[i][j/8] >> (j%8)) & 0x01) {
                    /* the bit is set */
                    PDF_rect(phandle, (float) k, (float) l, 1.0, 1.0);
                    PDF_fill(phandle);
                }
            }
            PDF_end_pattern(phandle);
        }
    }
}

static size_t pdf_writeproc(PDF *p, void *data, size_t size)
{
    FILE *fp = PDF_get_opaque(p);
    return fwrite(data, 1, size, fp);
}

int pdfinitgraphics(void)
{
    int i;
    Page_geometry pg;
    char *s;

    /* device-dependent routines */
    devupdatecmap   = NULL;
    
    devdrawpixel    = pdf_drawpixel;
    devdrawpolyline = pdf_drawpolyline;
    devfillpolygon  = pdf_fillpolygon;
    devdrawarc      = pdf_drawarc;
    devfillarc      = pdf_fillarc;
    devputpixmap    = pdf_putpixmap;
    devputtext      = pdf_puttext;
    
    devleavegraphics = pdf_leavegraphics;
    
    pg = get_page_geometry();
    
    page_scale = MIN2(pg.height, pg.width);
    pixel_size = 1.0/page_scale;
    page_scalef = (float) page_scale*72.0/pg.dpi;

    /* undefine all graphics state parameters */
    pdf_color = -1;
    pdf_pattern = -1;
    pdf_linew = -1.0;
    pdf_lines = -1;
    pdf_linecap = -1;
    pdf_linejoin = -1;

    phandle = PDF_new2(pdf_error_handler, NULL, NULL, NULL, (void *) prstream);
    if (phandle == NULL) {
        return RETURN_FAILURE;
    }

    if (pdf_setup_pdf1_3 == TRUE) {
        s = "1.3";
    } else {
        s = "1.4";
    }
    PDF_set_parameter(phandle, "compatibility", s);

    PDF_open_mem(phandle, pdf_writeproc);
    
    PDF_set_value(phandle, "compress", (float) pdf_setup_compression);

    PDF_set_info(phandle, "Creator", bi_version_string());
    PDF_set_info(phandle, "Author", get_username());
    PDF_set_info(phandle, "Title", get_docname());

    pdf_font_ids = xmalloc(number_of_fonts()*SIZEOF_INT);
    for (i = 0; i < number_of_fonts(); i++) {
        pdf_font_ids[i] = -1;
    }
    
    pdfinitpatterns();

    PDF_begin_page(phandle, pg.width*72.0/pg.dpi, pg.height*72.0/pg.dpi);

    if ((s = get_project_description())) {
        PDF_set_border_style(phandle, "dashed", 3.0);
        PDF_set_border_dash(phandle, 5.0, 1.0);
        PDF_set_border_color(phandle, 1.0, 0.0, 0.0);

        PDF_add_note(phandle,
                     20.0, 50.0, 320.0, 100.0, s, "Project description", "note", 0);
    }
    
    PDF_scale(phandle, page_scalef, page_scalef);

    return RETURN_SUCCESS;
}

void pdf_setpen(const Pen *pen)
{
    fRGB *frgb;
    
    if (pen->color != pdf_color || pen->pattern != pdf_pattern) {
        frgb = get_frgb(pen->color);

        PDF_setcolor(phandle, "both", "rgb",
                     (float) frgb->red, (float) frgb->green,(float) frgb->blue, 0.0);

        if (pdf_setup_pdfpattern &&
                pen->pattern > 1 && pen->pattern < number_of_patterns()) {
            PDF_setcolor(phandle, "both", "pattern",
                         (float) pdf_pattern_ids[pen->pattern], 0.0, 0.0, 0.0);
        }
        
        pdf_color = pen->color;
        pdf_pattern = pen->pattern;
    }
}

void pdf_setdrawbrush(void)
{
    int i;
    float lw;
    int ls;
    float *darray;
    Pen pen;

    pen=getpen();

    pdf_setpen(&pen);
    
    ls = getlinestyle();
    lw = MAX2(getlinewidth(), pixel_size);

    if (ls != pdf_lines || lw != pdf_linew) {
        PDF_setlinewidth(phandle, lw);

        if (ls == 0 || ls == 1) {
            PDF_setdash(phandle, 0, 0);
        } else {
            darray = xmalloc(dash_array_length[ls]*SIZEOF_FLOAT);
            for (i = 0; i < dash_array_length[ls]; i++) {
                darray[i] = lw*dash_array[ls][i];
            }
            PDF_setpolydash(phandle, darray, dash_array_length[ls]);
            xfree(darray);
        }
        pdf_linew = lw;
        pdf_lines = ls;
    }
}

void pdf_setlineprops(void)
{
    int lc, lj;
    
    lc = getlinecap();
    lj = getlinejoin();
    
    if (lc != pdf_linecap) {
        switch (lc) {
        case LINECAP_BUTT:
            PDF_setlinecap(phandle, 0);
            break;
        case LINECAP_ROUND:
            PDF_setlinecap(phandle, 1);
            break;
        case LINECAP_PROJ:
            PDF_setlinecap(phandle, 2);
            break;
        }
        pdf_linecap = lc;
    }

    if (lj != pdf_linejoin) {
        switch (lj) {
        case LINEJOIN_MITER:
            PDF_setlinejoin(phandle, 0);
            break;
        case LINEJOIN_ROUND:
            PDF_setlinejoin(phandle, 1);
            break;
        case LINEJOIN_BEVEL:
            PDF_setlinejoin(phandle, 2);
            break;
        }
        pdf_linejoin = lj;
    }
}

void pdf_drawpixel(VPoint vp)
{
    Pen pen;

    pen=getpen();
    pdf_setpen(&pen);
    
    if (pdf_linew != pixel_size) {
        PDF_setlinewidth(phandle, pixel_size);
        pdf_linew = pixel_size;
    }
    if (pdf_linecap != LINECAP_ROUND) {
        PDF_setlinecap(phandle, 1);
        pdf_linecap = LINECAP_ROUND;
    }
    if (pdf_lines != 1) {
        PDF_setpolydash(phandle, NULL, 0);
        pdf_lines = 1;
    }

    PDF_moveto(phandle, (float) vp.x, (float) vp.y);
    PDF_lineto(phandle, (float) vp.x, (float) vp.y);
    PDF_stroke(phandle);
}

void pdf_drawpolyline(VPoint *vps, int n, int mode)
{
    int i;
    
    if (getlinestyle() == 0) {
        return;
    }
    
    pdf_setdrawbrush();
    pdf_setlineprops();
    
    PDF_moveto(phandle, (float) vps[0].x, (float) vps[0].y);
    for (i = 1; i < n; i++) {
        PDF_lineto(phandle, (float) vps[i].x, (float) vps[i].y);
    }
    if (mode == POLYLINE_CLOSED) {
        PDF_closepath_stroke(phandle);
    } else {
        PDF_stroke(phandle);
    }
}

void pdf_fillpolygon(VPoint *vps, int nc)
{
    int i;
    Pen pen;
    
    pen=getpen();
    pdf_setpen(&pen);
    
    if (pen.pattern == 0) {
        return;
    }

    if (getfillrule() == FILLRULE_WINDING) {
        PDF_set_parameter(phandle, "fillrule", "winding");
    } else {
        PDF_set_parameter(phandle, "fillrule", "evenodd");
    }
    
    /* fill bg first if the pattern != solid */
    if (pdf_setup_pdfpattern && pen.pattern != 1) {
        Pen solid_pen;
        solid_pen.color = getbgcolor();
        solid_pen.pattern = 1;
        
        pdf_setpen(&solid_pen);
        PDF_moveto(phandle, (float) vps[0].x, (float) vps[0].y);
        for (i = 1; i < nc; i++) {
            PDF_lineto(phandle, (float) vps[i].x, (float) vps[i].y);
        }
        PDF_fill(phandle);
    }

    pen=getpen();
    pdf_setpen(&pen);
    PDF_moveto(phandle, (float) vps[0].x, (float) vps[0].y);
    for (i = 1; i < nc; i++) {
        PDF_lineto(phandle, (float) vps[i].x, (float) vps[i].y);
    }
    PDF_fill(phandle);
}

void pdf_drawarc(VPoint vp1, VPoint vp2, int a1, int a2)
{
    VPoint vpc;
    double rx, ry;
    
    if (getlinestyle() == 0) {
        return;
    }
    
    pdf_setdrawbrush();
    
    vpc.x = (vp1.x + vp2.x)/2;
    vpc.y = (vp1.y + vp2.y)/2;
    rx = fabs(vp2.x - vp1.x)/2;
    ry = fabs(vp2.y - vp1.y)/2;
    
    if (rx == 0.0 || ry == 0.0) {
        return;
    }
    
    PDF_save(phandle);
    PDF_scale(phandle, 1.0, ry/rx);
    PDF_moveto(phandle, (float) vpc.x + rx*cos(a1*M_PI/180.0),
               (float) rx/ry*vpc.y + rx*sin(a1*M_PI/180.0));
    PDF_arc(phandle, (float) vpc.x, (float) rx/ry*vpc.y, rx,
            (float) a1, (float) a2);
    PDF_stroke(phandle);
    PDF_restore(phandle);
}

void pdf_fillarc(VPoint vp1, VPoint vp2, int a1, int a2, int mode)
{
    VPoint vpc;
    double rx, ry;
    Pen pen;
    
    if (getpattern() == 0) {
        return;
    }

    pen=getpen();
    pdf_setpen(&pen);
    
    vpc.x = (vp1.x + vp2.x)/2;
    vpc.y = (vp1.y + vp2.y)/2;
    rx = fabs(vp2.x - vp1.x)/2;
    ry = fabs(vp2.y - vp1.y)/2;
    
    if (rx == 0.0 || ry == 0.0) {
        return;
    }

    /* fill bg first if the pattern != solid */
    if (pdf_setup_pdfpattern && pen.pattern != 1) {
        Pen solid_pen;
        solid_pen.color = getbgcolor();
        solid_pen.pattern = 1;
        
        PDF_save(phandle);
        pdf_setpen(&solid_pen);
        PDF_scale(phandle, 1.0, ry/rx);

        PDF_moveto(phandle, (float) vpc.x + rx*cos(a1*M_PI/180.0),
                   (float) rx/ry*vpc.y + rx*sin(a1*M_PI/180.0));
        PDF_arc(phandle, (float) vpc.x, (float) rx/ry*vpc.y, rx,
                (float) a1, (float) a2);
        if (mode == ARCFILL_PIESLICE) {
            PDF_lineto(phandle, (float) vpc.x, (float) rx/ry*vpc.y);
        }
        PDF_fill(phandle);
        PDF_restore(phandle);
    }

    PDF_save(phandle);
    PDF_scale(phandle, 1.0, ry/rx);
    PDF_moveto(phandle, (float) vpc.x + rx*cos(a1*M_PI/180.0),
               (float) rx/ry*vpc.y + rx*sin(a1*M_PI/180.0));
    PDF_arc(phandle, (float) vpc.x, (float) rx/ry*vpc.y, rx,
            (float) a1, (float) a2);
    if (mode == ARCFILL_PIESLICE) {
        PDF_lineto(phandle, (float) vpc.x, (float) rx/ry*vpc.y);
    }
    PDF_fill(phandle);
    PDF_restore(phandle);
}

/* TODO: transparent pixmaps */
void pdf_putpixmap(VPoint vp, int width, int height, char *databits, 
                   int pixmap_bpp, int bitmap_pad, int pixmap_type)
{
    char *buf, *bp;
    int image;
    int cindex;
    RGB *fg, *bg;
    int	i, k, j;
    long paddedW;

    int components    = 3;

    buf = xmalloc(width*height*components);
    if (buf == NULL) {
        errmsg("xmalloc failed in pdf_putpixmap()");
        return;
    }
    
    bp = buf;
    if (pixmap_bpp == 1) {
        paddedW = PAD(width, bitmap_pad);
        fg = get_rgb(getcolor());
        bg = get_rgb(getbgcolor());
        for (k = 0; k < height; k++) {
            for (j = 0; j < paddedW/bitmap_pad; j++) {
                for (i = 0; i < bitmap_pad && j*bitmap_pad + i < width; i++) {
                    if (bin_dump(&(databits)[k*paddedW/bitmap_pad + j], i, bitmap_pad)) {
                        *bp++ = (char) fg->red;
                        *bp++ = (char) fg->green;
                        *bp++ = (char) fg->blue;
                    } else {
                        *bp++ = (char) bg->red;
                        *bp++ = (char) bg->green;
                        *bp++ = (char) bg->blue;
                    }
                }
            }
        }
    } else {
        for (k = 0; k < height; k++) {
            for (j = 0; j < width; j++) {
                cindex = (databits)[k*width + j];
                fg = get_rgb(cindex);
                *bp++ = (char) fg->red;
                *bp++ = (char) fg->green;
                *bp++ = (char) fg->blue;
            }
        }
    }
    
    image = PDF_open_image(phandle, "raw", "memory",
                           buf, width*height*components,
                           width, height, components, GRACE_BPP, "");
    if (image == -1) {
        errmsg("Not enough memory for image!");
        xfree(buf);
        return;
    }

    PDF_place_image(phandle, image, vp.x, vp.y - height*pixel_size, pixel_size);
    PDF_close_image(phandle, image);
    
    xfree(buf);
}

static const char *pdf_builtin_fonts[] =
{
    "Times-Roman",              /// 0
    "Times-Italic",             /// 1
    "Times-Bold",               /// 2
    "Times-BoldItalic",         /// 3
    "Helvetica",                /// 4
    "Helvetica-Oblique",        /// 5
    "Helvetica-Bold",           /// 6
    "Helvetica-BoldOblique",    /// 7
    "Courier",                  /// 8
    "Courier-Oblique",          /// 9
    "Courier-Bold",             /// 10
    "Courier-BoldOblique",      /// 11
    "Symbol",                   /// 12
    "ZapfDingbats"              /// 13
};

static int number_of_pdf_builtin_fonts = sizeof(pdf_builtin_fonts)/sizeof(char *);

static int pdf_builtin_font(const char *fname)
{
    int i;
    for (i = 0; i < number_of_pdf_builtin_fonts; i++) {
        if (strcmp(pdf_builtin_fonts[i], fname) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

void pdf_puttext(VPoint vp, char *s, int len, int font,
                 TextMatrix *tm, int underline, int overline, int kerning)
{
    Pen pen;

    pen=getpen();
    pdf_setpen(&pen);
    
    if (pdf_font_ids[font] < 0) {
        char buf[GR_MAXPATHLEN];
        char *fontname, *encscheme;
        char *pdflibenc;
        int embed;
        
        fontname = get_fontalias(font);
        
        if (pdf_builtin_font(fontname)) {
            embed = 0;
        } else {
            sprintf(buf, "%s==%s",
                    fontname, get_afmfilename(font, TRUE));
            PDF_set_parameter(phandle, "FontAFM", buf);
            sprintf(buf, "%s==%s",
                    fontname, get_fontfilename(font, TRUE));
            PDF_set_parameter(phandle, "FontOutline", buf);

            embed = 1;
        }

        encscheme = get_encodingscheme(font);
        if (strcmp(encscheme, "FontSpecific") == 0) {
            pdflibenc = "builtin";
        } else {
            pdflibenc = "winansi";
        }
        
        pdf_font_ids[font] = PDF_findfont(phandle, fontname, pdflibenc, embed);
    }
    
    PDF_save(phandle);
    
    PDF_setfont(phandle, pdf_font_ids[font], 1.0);

    PDF_set_parameter(phandle, "underline", true_or_false(underline));
    PDF_set_parameter(phandle, "overline",  true_or_false(overline));
    PDF_concat(phandle, (float) tm->cxx, (float) tm->cyx,
               (float) tm->cxy, (float) tm->cyy,
               vp.x, vp.y);

    PDF_show2(phandle, s, len);

    PDF_restore(phandle);
}

void pdf_leavegraphics(void)
{
    PDF_end_page(phandle);
    PDF_close(phandle);
    PDF_delete(phandle);
    xfree(pdf_font_ids);
    XCFREE(pdf_pattern_ids);
}

static void pdf_error_handler(PDF *p, int type, const char *msg)
{
    char buf[MAX_STRING_LENGTH];

    switch (type) {
    case PDF_NonfatalError:
        /* continue on a non-fatal error */
        sprintf(buf, "PDFlib: %s", msg);
        errmsg(buf);
        break;
    default:
        /* give up in all other cases */
        sprintf(buf, "PDFlib: %s", msg);
        errmsg(buf);
        return;
    }
}

int pdf_op_parser(char *opstring)
{
    if (!strcmp(opstring, "PDF1.3")) {
        pdf_setup_pdf1_3 = TRUE;
        return RETURN_SUCCESS;
    } else if (!strcmp(opstring, "PDF1.4")) {
        pdf_setup_pdf1_3 = FALSE;
        return RETURN_SUCCESS;
    } else if (!strcmp(opstring, "patterns:on")) {
        pdf_setup_pdfpattern = TRUE;
        return RETURN_SUCCESS;
    } else if (!strcmp(opstring, "patterns:off")) {
        pdf_setup_pdfpattern = FALSE;
        return RETURN_SUCCESS;
    } else if (!strncmp(opstring, "compression:", 12)) {
        char *bufp;
        bufp = strchr(opstring, ':');
        bufp++;
        if (bufp != NULL && *bufp != '\0') {
            pdf_setup_compression = atoi(bufp);
            return RETURN_SUCCESS;
        } else {
            return RETURN_FAILURE;
        }
    } else {
        return RETURN_FAILURE;
    }
}

#ifndef NONE_GUI

static void update_pdf_setup_frame(void);
static int set_pdf_setup_proc(void *data);

static Widget pdf_setup_frame;
static Widget pdf_setup_pdf1_3_item;
static Widget pdf_setup_pdfpattern_item;
static SpinStructure *pdf_setup_compression_item;

void pdf_gui_setup(void)
{
    set_wait_cursor();
    
    if (pdf_setup_frame == NULL) {
        Widget fr, rc;

        pdf_setup_frame = CreateDialogForm(app_shell, "PDF options");

        fr = CreateFrame(pdf_setup_frame, "PDF options");
        rc = CreateVContainer(fr);
        pdf_setup_pdf1_3_item = CreateToggleButton(rc, "PDF-1.3");
        pdf_setup_pdfpattern_item = CreateToggleButton(rc, "Use patterns");
        pdf_setup_compression_item = CreateSpinChoice(rc,
                                                      "Compression:", 1, SPIN_TYPE_INT, 0.0, 9.0, 1.0);

        CreateAACDialog(pdf_setup_frame, fr, set_pdf_setup_proc, NULL);
    }
    update_pdf_setup_frame();
    RaiseWindow(GetParent(pdf_setup_frame));
    unset_wait_cursor();
}

static void update_pdf_setup_frame(void)
{
    if (pdf_setup_frame) {
        SetToggleButtonState(pdf_setup_pdf1_3_item, pdf_setup_pdf1_3);
        SetToggleButtonState(pdf_setup_pdfpattern_item, pdf_setup_pdfpattern);
        SetSpinChoice(pdf_setup_compression_item, (double) pdf_setup_compression);
    }
}

static int set_pdf_setup_proc(void *data)
{
    pdf_setup_pdf1_3 = GetToggleButtonState(pdf_setup_pdf1_3_item);
    pdf_setup_pdfpattern = GetToggleButtonState(pdf_setup_pdfpattern_item);
    pdf_setup_compression = (int) GetSpinChoice(pdf_setup_compression_item);
    
    return RETURN_SUCCESS;
}

#endif

#else /* No PDFlib */
void _pdfdrv_c_dummy_func(void) {}

extern Prototype_HPDF_Page_SetHeight libHaru_HPDF_Page_SetHeight;
extern Prototype_HPDF_Page_SetWidth libHaru_HPDF_Page_SetWidth;
extern Prototype_HPDF_AddPage libHaru_HPDF_AddPage;
extern Prototype_HPDF_Free libHaru_HPDF_Free;
extern Prototype_HPDF_SaveToFile libHaru_HPDF_SaveToFile;
extern Prototype_HPDF_New libHaru_HPDF_New;
extern Prototype_HPDF_ResetError libHaru_HPDF_ResetError;
extern Prototype_HPDF_Page_GetWidth libHaru_HPDF_Page_GetWidth;
extern Prototype_HPDF_Page_GetHeight libHaru_HPDF_Page_GetHeight;
extern Prototype_HPDF_GetFont libHaru_HPDF_GetFont;
extern Prototype_HPDF_Page_SetFontAndSize libHaru_HPDF_Page_SetFontAndSize;
extern Prototype_HPDF_Page_SetLineWidth libHaru_HPDF_Page_SetLineWidth;
extern Prototype_HPDF_Page_SetLineCap libHaru_HPDF_Page_SetLineCap;
extern Prototype_HPDF_Page_SetLineJoin libHaru_HPDF_Page_SetLineJoin;
extern Prototype_HPDF_Page_SetMiterLimit libHaru_HPDF_Page_SetMiterLimit;
extern Prototype_HPDF_Page_SetDash libHaru_HPDF_Page_SetDash;
extern Prototype_HPDF_Page_GetLineWidth libHaru_HPDF_Page_GetLineWidth;
extern Prototype_HPDF_Page_MoveTo libHaru_HPDF_Page_MoveTo;
extern Prototype_HPDF_Page_LineTo libHaru_HPDF_Page_LineTo;
extern Prototype_HPDF_Page_Stroke libHaru_HPDF_Page_Stroke;
extern Prototype_HPDF_Page_Fill libHaru_HPDF_Page_Fill;
extern Prototype_HPDF_Page_BeginText libHaru_HPDF_Page_BeginText;
extern Prototype_HPDF_Page_EndText libHaru_HPDF_Page_EndText;
extern Prototype_HPDF_Page_MoveTextPos libHaru_HPDF_Page_MoveTextPos;
extern Prototype_HPDF_Page_ShowText libHaru_HPDF_Page_ShowText;
extern Prototype_HPDF_Page_ClosePathStroke libHaru_HPDF_Page_ClosePathStroke;
extern Prototype_HPDF_Page_SetRGBStroke libHaru_HPDF_Page_SetRGBStroke;
extern Prototype_HPDF_Page_Ellipse libHaru_HPDF_Page_Ellipse;
extern Prototype_HPDF_Page_Arc libHaru_HPDF_Page_Arc;
extern Prototype_HPDF_Page_Rectangle libHaru_HPDF_Page_Rectangle;
extern Prototype_HPDF_Page_GSave libHaru_HPDF_Page_GSave;
extern Prototype_HPDF_Page_GRestore libHaru_HPDF_Page_GRestore;
extern Prototype_HPDF_Page_SetRGBFill libHaru_HPDF_Page_SetRGBFill;
extern Prototype_HPDF_SetCompressionMode libHaru_HPDF_SetCompressionMode;
extern Prototype_HPDF_Page_SetTextMatrix libHaru_HPDF_Page_SetTextMatrix;
extern Prototype_HPDF_Page_Concat libHaru_HPDF_Page_Concat;
extern Prototype_HPDF_Page_Eofill libHaru_HPDF_Page_Eofill;
extern Prototype_HPDF_Page_TextWidth libHaru_HPDF_Page_TextWidth;
extern Prototype_HPDF_SetInfoAttr libHaru_HPDF_SetInfoAttr;
extern Prototype_HPDF_LoadType1FontFromFile libHaru_HPDF_LoadType1FontFromFile;
extern Prototype_HPDF_GetTTFontDefFromFile libHaru_HPDF_GetTTFontDefFromFile;
extern Prototype_HPDF_LoadTTFontFromFile libHaru_HPDF_LoadTTFontFromFile;
extern Prototype_HPDF_LoadTTFontFromFile2 libHaru_HPDF_LoadTTFontFromFile2;
extern Prototype_HPDF_SetCurrentEncoder libHaru_HPDF_SetCurrentEncoder;

static Device_entry dev_haru_pdf = {DEVICE_FILE,
                               "PDF(Haru)",
                               haru_pdf_initgraphics,
                               NULL,
                               NULL,
                               "pdf",
                               TRUE,
                               FALSE,
                               {792, 612, 72.0},
                               NULL,
                               1,
                               "HPDF"
                              };
//std-size changed from{3300, 2550, 300.0},

static HPDF_Doc  pdf;
static HPDF_Page page;
//static jmp_buf env;

int nr_of_dash_patterns=0;
int nr_of_alloc_dash_patterns=0;
HPDF_UINT16 ** pdf_dash_pattern=NULL;
HPDF_UINT16 * cur_pdf_dash_pattern=NULL;

extern int nr_of_current_linestyles;
extern int * lenghts_of_linestyle_patterns;
extern char ** current_linestyle_patterns;
extern bool useQtFonts;

void init_pdf_dash_patterns(void)
{
if (nr_of_alloc_dash_patterns>0)
{
for (int i=0;i<nr_of_alloc_dash_patterns;i++) delete[] pdf_dash_pattern[i];
delete[] pdf_dash_pattern;
pdf_dash_pattern=NULL;
}
pdf_dash_pattern=new HPDF_UINT16*[nr_of_current_linestyles];
for (int i=0;i<nr_of_current_linestyles;i++)
{
pdf_dash_pattern[i]=new HPDF_UINT16[lenghts_of_linestyle_patterns[i]];
    for (int j=0;j<lenghts_of_linestyle_patterns[i];j++)
    pdf_dash_pattern[i][j]=(HPDF_UINT16)current_linestyle_patterns[i][j];
}
}

void center_of_mass(float * xvals,float *yvals,int n,float & x0,float & y0)
{
x0=y0=0.0;
for (int i=0;i<n;i++)
{
x0+=xvals[i];
y0+=yvals[i];
}
x0/=1.0*n;
y0/=1.0*n;
}

void error_handler(HPDF_STATUS   error_no,HPDF_STATUS   detail_no,void *user_data)
{
    (void) user_data;
    char err_buf[128];
    sprintf(err_buf,"Problem while saving pdf: error_no=%04X, detail_no=%u", (HPDF_UINT)error_no, (HPDF_UINT)detail_no);
    errmsg(err_buf);
    //longjmp(env, 1);
    throw std::exception();
    libHaru_HPDF_ResetError(pdf);
}

int register_haru_pdf_drv(void)
{
    if (have_libHaru==false || use_libHaru==false) return -1;
    return register_device(dev_haru_pdf);
}

void haru_pdf_setpen(const Pen *pen)
{
    fRGB *frgb;
static double lw;//=getlinewidth()/MAGIC_LINEW_SCALE;

lw = MAX2(getlinewidth(), pixel_size)*page_scale;

    if (pdf_linew != lw)
    {
    libHaru_HPDF_Page_SetLineWidth(page,lw);
//cout << "pdf: setting linewidth=" << lw << endl;
    pdf_linew = lw;
    }

    if (pen->color != pdf_color || pen->pattern != pdf_pattern)
    {
    frgb = get_frgb(pen->color);
        libHaru_HPDF_Page_SetRGBStroke(page,(float) frgb->red, (float) frgb->green,(float) frgb->blue);
        /// PDF_setcolor(phandle, "both", "rgb",(float) frgb->red, (float) frgb->green,(float) frgb->blue, 0.0);
//cout << "pdf: setting pen color= " << frgb->red << "|" << frgb->green << "|" << frgb->blue << " LineWidth=" << lw << endl;
        if (pdf_setup_pdfpattern && pen->pattern > 1 && pen->pattern < number_of_patterns())//line patterns not supported in libHaru
        {
        /// PDF_setcolor(phandle, "both", "pattern",(float) pdf_pattern_ids[pen->pattern], 0.0, 0.0, 0.0);
        }
        pdf_color = pen->color;
        pdf_pattern = pen->pattern;
    }
}

void haru_pdf_setdrawbrush(void)
{
static float lw;
static int ls;
static Pen pen;

    pen=getpen();

    haru_pdf_setpen(&pen);

    ls = getlinestyle();
    lw = MAX2(getlinewidth(), pixel_size)*page_scale;

    if (ls != pdf_lines){// || lw != pdf_linew) {
        //libHaru_HPDF_Page_SetLineWidth(page, lw);

        if (ls==0 || ls==1)
        libHaru_HPDF_Page_SetDash(page, NULL, 0, 0);
        else
        {
            if (cur_pdf_dash_pattern!=NULL) delete[] cur_pdf_dash_pattern;
        cur_pdf_dash_pattern=new HPDF_UINT16[lenghts_of_linestyle_patterns[ls]];
        for (int j=0;j<lenghts_of_linestyle_patterns[ls];j++)
        {
        cur_pdf_dash_pattern[j]=(HPDF_UINT16)(pdf_dash_pattern[ls][j]*round(lw));
        }

        libHaru_HPDF_Page_SetDash(page, cur_pdf_dash_pattern, lenghts_of_linestyle_patterns[ls], 0);
        }

        //pdf_linew = lw;
        pdf_lines = ls;
    }
}

void haru_pdf_setlineprops(void)
{
    int lc, lj;

    lc = getlinecap();
    lj = getlinejoin();

    if (lc != pdf_linecap) {
        switch (lc) {
        case LINECAP_BUTT:
            libHaru_HPDF_Page_SetLineCap(page,HPDF_BUTT_END);
            /// PDF_setlinecap(phandle, 0);
            break;
        case LINECAP_ROUND:
            libHaru_HPDF_Page_SetLineCap(page,HPDF_ROUND_END);
            /// PDF_setlinecap(phandle, 1);
            break;
        case LINECAP_PROJ:
            libHaru_HPDF_Page_SetLineCap(page,HPDF_PROJECTING_SCUARE_END);
            /// PDF_setlinecap(phandle, 2);
            break;
        }
        pdf_linecap = lc;
    }

    if (lj != pdf_linejoin) {
        switch (lj) {
        case LINEJOIN_MITER:
            libHaru_HPDF_Page_SetLineJoin(page,HPDF_MITER_JOIN);
            /// PDF_setlinejoin(phandle, 0);
            break;
        case LINEJOIN_ROUND:
            libHaru_HPDF_Page_SetLineJoin(page,HPDF_ROUND_JOIN);
            /// PDF_setlinejoin(phandle, 1);
            break;
        case LINEJOIN_BEVEL:
            libHaru_HPDF_Page_SetLineJoin(page,HPDF_BEVEL_JOIN);
            /// PDF_setlinejoin(phandle, 2);
            break;
        }
        pdf_linejoin = lj;
    }
}

int haru_pdf_initgraphics(void)
{
    /*int i;
    char *s;*/
    Page_geometry pg;

    if (libHaru_HPDF_New==NULL)
    {
    errmsg(QObject::tr("PDF-library libHaru not usable!").toLocal8Bit().data());
    return RETURN_FAILURE;
    }

    /* device-dependent routines */
    devupdatecmap   = NULL;

    devdrawpixel    = haru_pdf_drawpixel;
    devdrawpolyline = haru_pdf_drawpolyline;
    devfillpolygon  = haru_pdf_fillpolygon;
    devdrawarc      = haru_pdf_drawarc;
    devfillarc      = haru_pdf_fillarc;
    devputpixmap    = haru_pdf_putpixmap;
    devputtext      = haru_pdf_puttext;

    devleavegraphics = haru_pdf_leavegraphics;

    pg = get_page_geometry();
//qDebug() << "target pg:" << pg.width << "x" << pg.height;

    page_scale = MIN2(pg.height, pg.width);
    pixel_size = 1.0/page_scale;
    page_scalef = (float) page_scale*72.0/pg.dpi;

    /* undefine all graphics state parameters */
    pdf_color = -1;
    pdf_pattern = -1;
    pdf_linew = -1.0;
    pdf_lines = -1;
    pdf_linecap = -1;
    pdf_linejoin = -1;

    init_pdf_dash_patterns();

    /* from grid_sheet.c-example from libHaru */

    pdf = libHaru_HPDF_New (error_handler, NULL);
    if (!pdf) {
        printf ("error: cannot create PdfDoc object\n");
//cout << "FAILURE CREATE NEW PDF" << endl;
        return RETURN_FAILURE;
    }

    /*if (setjmp(env)) {
        libHaru_HPDF_Free (pdf);
//cout << "FAILURE setjmp: FREE PDF" << endl;
        return RETURN_FAILURE;
    }*/

    libHaru_HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);
    //libHaru_HPDF_SetCurrentEncoder(pdf, "WinAnsiEncoding");

    libHaru_HPDF_SetInfoAttr(pdf,HPDF_INFO_SUBJECT,get_project_description());
    char dummy_n[GR_MAXPATHLEN+2];
    sprintf(dummy_n,"QtGrace %s",QTGRACE_VERSION_STRING);
    libHaru_HPDF_SetInfoAttr(pdf,HPDF_INFO_CREATOR,dummy_n);

    /* add a new page object. */
    page = libHaru_HPDF_AddPage (pdf);

//qDebug() << "pdf: size=" << libHaru_HPDF_Page_GetWidth(page) << "x" << libHaru_HPDF_Page_GetHeight(page) << endl;
//qDebug() << "target pg:" << pg.width << "x" << pg.height;

    libHaru_HPDF_Page_SetHeight (page, pg.height);
    libHaru_HPDF_Page_SetWidth (page, pg.width);

//qDebug() << "pdf: size=" << libHaru_HPDF_Page_GetWidth(page) << "x" << libHaru_HPDF_Page_GetHeight(page) << endl;

    /// phandle = PDF_new2(pdf_error_handler, NULL, NULL, NULL, (void *) prstream);
/*    if (phandle == NULL) {
        return RETURN_FAILURE;
    }

    if (pdf_setup_pdf1_3 == TRUE) {
        s = "1.3";
    } else {
        s = "1.4";
    }
    PDF_set_parameter(phandle, "compatibility", s);

    PDF_open_mem(phandle, pdf_writeproc);

    PDF_set_value(phandle, "compress", (float) pdf_setup_compression);

    PDF_set_info(phandle, "Creator", bi_version_string());
    PDF_set_info(phandle, "Author", get_username());
    PDF_set_info(phandle, "Title", get_docname());

    pdf_font_ids = xmalloc(number_of_fonts()*SIZEOF_INT);
    for (i = 0; i < number_of_fonts(); i++) {
        pdf_font_ids[i] = -1;
    }

    pdfinitpatterns();

    PDF_begin_page(phandle, pg.width*72.0/pg.dpi, pg.height*72.0/pg.dpi);

    if ((s = get_project_description())) {
        PDF_set_border_style(phandle, "dashed", 3.0);
        PDF_set_border_dash(phandle, 5.0, 1.0);
        PDF_set_border_color(phandle, 1.0, 0.0, 0.0);

        PDF_add_note(phandle,
                     20.0, 50.0, 320.0, 100.0, s, "Project description", "note", 0);
    }

    PDF_scale(phandle, page_scalef, page_scalef);
*/
    return RETURN_SUCCESS;
}

void haru_pdf_leavegraphics(void)
{
/*QFile fi(print_file);
if (fi.exists()==true)
{
qDebug() << "File exists --> delete it!";
qDebug() << "Success?" << fi.remove();
}*/
try
{
    /* save the document to a file */
    libHaru_HPDF_SaveToFile(pdf, print_file);
}
catch (...)
{
qDebug() << "Error during pdf-save!";
}
    /* clean up */
    libHaru_HPDF_Free(pdf);

//cout << "PrintFile: " << print_file << endl;
}

void haru_pdf_puttext(VPoint vp, char *s, int len, int font,
                 TextMatrix *tm, int underline, int overline, int kerning)
{
static float tw;
static double w, pos;
(void) kerning;

if (useQtFonts==true)
{
return;//do not plot text if QtFonts are to be used
}

    fRGB * frgb = get_frgb(getcolor());

    libHaru_HPDF_Page_SetRGBFill(page, (float)frgb->red, (float)frgb->green, (float)frgb->blue);

//cout << "pdf: FontID=" << font << " GetFontName=" << get_fontname(font) << " GetFontAlias=" << get_fontalias(font) << endl;
    HPDF_Font pdf_font;

    if (font==12 || font==13)//symbol || zapf dingbats
    pdf_font = libHaru_HPDF_GetFont(pdf, get_fontalias(font), "FontSpecific");
    else
    pdf_font = libHaru_HPDF_GetFont(pdf, get_fontalias(font), "WinAnsiEncoding");

    char * tmp_txt=new char[len+1];
    strncpy(tmp_txt,s,len);
    tmp_txt[len]='\0';
    double fsize=sqrt(tm->cxx*tm->cxx+tm->cyx*tm->cyx)/MAGIC_FONT_SCALE;
    double fsize2=fsize*MAGIC_FONT_SCALE;
    fsize*=page_scale*9.0/445.0*1.38;//why *1.38 ?

//cout << "PutText: " << tmp_txt << " font_size=" << fsize << endl;
//cout << "Text=" << tmp_txt << " Pos=" << (float)vp.x*page_scale << "|" << (float)vp.y*page_scale << endl;

    libHaru_HPDF_Page_SetFontAndSize(page, pdf_font, (float)fsize);
    libHaru_HPDF_Page_BeginText(page);
    libHaru_HPDF_Page_SetTextMatrix(page,(float)tm->cxx/fsize2,(float)tm->cyx/fsize2,(float)tm->cxy/fsize2,(float)tm->cyy/fsize2,(float)vp.x*page_scale,(float)vp.y*page_scale);
    tw = libHaru_HPDF_Page_TextWidth(page, s);
    libHaru_HPDF_Page_ShowText(page, s);
    libHaru_HPDF_Page_EndText(page);

    if (underline | overline)
    {
        libHaru_HPDF_Page_GSave(page);
        libHaru_HPDF_Page_Concat(page, (float)tm->cxx/fsize2,(float)tm->cyx/fsize2,(float)tm->cxy/fsize2,(float)tm->cyy/fsize2, (float)vp.x*page_scale, (float)vp.y*page_scale);
        w = get_textline_width(font)*fsize;
        libHaru_HPDF_Page_SetLineWidth(page,w);
        libHaru_HPDF_Page_SetRGBStroke(page,(float) frgb->red, (float) frgb->green,(float) frgb->blue);
        if (underline) {
            pos = get_underline_pos(font);//*page_scale;
            pos*=fsize;
            libHaru_HPDF_Page_MoveTo(page,0,0+pos);
            libHaru_HPDF_Page_LineTo(page,tw,0+pos);
            libHaru_HPDF_Page_Stroke(page);
        }
        if (overline) {
            pos = get_overline_pos(font);//*page_scale;
            pos*=fsize;
            libHaru_HPDF_Page_MoveTo(page,0,0+pos);
            libHaru_HPDF_Page_LineTo(page,tw,0+pos);
            libHaru_HPDF_Page_Stroke(page);
        }
//cout << (underline?"underline":"overline") << ": w=" << w << " pos=" << pos << " vp=(" << vp.x*page_scale << " x " << vp.y*page_scale << ") tw=" << tw << endl;
        libHaru_HPDF_Page_GRestore(page);
    }

/*
    if (pdf_font_ids[font] < 0) {
        char buf[GR_MAXPATHLEN];
        char *fontname, *encscheme;
        char *pdflibenc;
        int embed;

        fontname = get_fontalias(font);

        if (pdf_builtin_font(fontname)) {
            embed = 0;
        } else {
            sprintf(buf, "%s==%s",
                    fontname, get_afmfilename(font, TRUE));
            PDF_set_parameter(phandle, "FontAFM", buf);
            sprintf(buf, "%s==%s",
                    fontname, get_fontfilename(font, TRUE));
            PDF_set_parameter(phandle, "FontOutline", buf);

            embed = 1;
        }

        encscheme = get_encodingscheme(font);
        if (strcmp(encscheme, "FontSpecific") == 0) {
            pdflibenc = "builtin";
        } else {
            pdflibenc = "winansi";
        }

        pdf_font_ids[font] = PDF_findfont(phandle, fontname, pdflibenc, embed);
    }

    PDF_save(phandle);

    PDF_setfont(phandle, pdf_font_ids[font], 1.0);

    PDF_set_parameter(phandle, "underline", true_or_false(underline));
    PDF_set_parameter(phandle, "overline",  true_or_false(overline));
    PDF_concat(phandle, (float) tm->cxx, (float) tm->cyx,
               (float) tm->cxy, (float) tm->cyy,
               vp.x, vp.y);

    PDF_show2(phandle, s, len);

    PDF_restore(phandle);*/
}

void haru_pdf_drawpixel(VPoint vp)
{
    Pen pen;

    pen=getpen();
    haru_pdf_setpen(&pen);

    if (pdf_linew != pixel_size) {
        libHaru_HPDF_Page_SetLineWidth(page,pixel_size);
        /// PDF_setlinewidth(phandle, pixel_size);
        pdf_linew = pixel_size;
    }
    if (pdf_linecap != LINECAP_ROUND) {
        libHaru_HPDF_Page_SetLineCap(page,HPDF_ROUND_END);
        /// PDF_setlinecap(phandle, 1);
        pdf_linecap = LINECAP_ROUND;
    }
    if (pdf_lines != 1) {
        /// ???
        /// PDF_setpolydash(phandle, NULL, 0);
        pdf_lines = 1;
    }
//cout << "Pixel: pos=" << (float) vp.x*page_scale << "|" << (float) vp.y*page_scale << endl;
    libHaru_HPDF_Page_MoveTo(page, (float) vp.x*page_scale, (float) vp.y*page_scale);
    libHaru_HPDF_Page_LineTo(page, (float) vp.x*page_scale, (float) vp.y*page_scale);
    libHaru_HPDF_Page_Stroke(page);
}

void haru_pdf_drawpolyline(VPoint *vps, int n, int mode)
{
static int i;
static double rad;
static float x_0,y_0;

    if (getlinestyle() == 0) {
        return;
    }

int linepattern=getpattern();
if (linepattern==0 || linepattern==8) return;//invisible patterns

    haru_pdf_setdrawbrush();
    haru_pdf_setlineprops();

    float * nvps[2];
    nvps[0]=new float[n+1];
    nvps[1]=new float[n+1];
    for (int i=0;i<n;i++)
    {
    nvps[0][i]=(float) (vps[i].x*page_scale);
    nvps[1][i]=(float) (vps[i].y*page_scale);
    //cout << "polyline: pos=" << nvps[0][i] << "|" << nvps[1][i] << endl;
    }

center_of_mass(nvps[0],nvps[1],n,x_0,y_0);
rad=-RotationAngle*M_PI/180.0;
libHaru_HPDF_Page_GSave(page);
libHaru_HPDF_Page_Concat(page, cos(rad), sin(rad), -sin(rad), cos(rad), x_0, y_0);
    for (int i=0;i<n;i++)
    {
    nvps[0][i] -= x_0;
    nvps[1][i] -= y_0;
    }

    libHaru_HPDF_Page_MoveTo(page, nvps[0][0], nvps[1][0]);
    for (i = 1; i < n; i++) {
        //cout << "x|y=" << (float) vps[i].x*page_scalef << "|" << (float) vps[i].y*page_scalef << endl;
        libHaru_HPDF_Page_LineTo(page, nvps[0][i], nvps[1][i]);
    }
    ///do not use Fill here! only Stroke!
    if (mode == POLYLINE_CLOSED) {
        libHaru_HPDF_Page_ClosePathStroke(page);
        //cout << "pdf: closePath n=" << n << endl;
    } else {
        libHaru_HPDF_Page_Stroke(page);
        //cout << "pdf: Stroke n=" << n << endl;
    }

libHaru_HPDF_Page_GRestore(page);
    delete[] nvps[0];
    delete[] nvps[1];
}

void haru_pdf_fillpolygon(VPoint *vps, int nc)
{
static int i;
static int pattern;
static double rad;
static float x_0,y_0;
static int fg_color;

pattern=getpattern();
    if (pattern == 0 || pattern == 8 || nc<3) {
        return;//No fill pattern (or less than 3 points) --> do not fill
    }

    float * nvps[2];
    nvps[0]=new float[nc+1];
    nvps[1]=new float[nc+1];
    for (int i=0;i<nc;i++)
    {
    nvps[0][i]=(float) (vps[i].x*page_scale);
    nvps[1][i]=(float) (vps[i].y*page_scale);
    //cout << "fill polyline: pos=" << nvps[0][i] << "|" << nvps[1][i] << endl;
    }
center_of_mass(nvps[0],nvps[1],nc,x_0,y_0);
rad=-RotationAngle*M_PI/180.0;
libHaru_HPDF_Page_GSave(page);
libHaru_HPDF_Page_Concat(page, cos(rad), sin(rad), -sin(rad), cos(rad), x_0, y_0);
    for (int i=0;i<nc;i++)
    {
    nvps[0][i] -= x_0;
    nvps[1][i] -= y_0;
    }

    fg_color=getcolor();//for filling something
    fRGB * frgb = get_frgb(fg_color);

    libHaru_HPDF_Page_SetRGBFill(page, (float) frgb->red, (float) frgb->green, (float) frgb->blue);
    ///do not use Stroke here! only Fill!
    libHaru_HPDF_Page_MoveTo(page, nvps[0][0], nvps[1][0]);
    for (i = 1; i < nc; i++)
    {
    libHaru_HPDF_Page_LineTo(page, nvps[0][i], nvps[1][i]);
    }
    if (getfillrule()==FILLRULE_EVENODD)
    libHaru_HPDF_Page_Eofill(page);
    else
    libHaru_HPDF_Page_Fill(page);

libHaru_HPDF_Page_GRestore(page);
    delete[] nvps[0];
    delete[] nvps[1];
}

void haru_pdf_drawarc(VPoint vp1, VPoint vp2, int a1, int a2)
{
static VPoint vpc;
static float rx, ry;//, rfactor;
static double rad;

    if (getlinestyle() == 0)
    {
        return;
    }

int linepattern=getpattern();
if (linepattern==0 || linepattern==8) return;//invisible patterns

    haru_pdf_setdrawbrush();
    haru_pdf_setlineprops();

    vpc.x = (vp1.x + vp2.x)*0.5*page_scale;
    vpc.y = (vp1.y + vp2.y)*0.5*page_scale;
    rx = (float)(fabs(vp2.x - vp1.x)*0.5*page_scale);
    ry = (float)(fabs(vp2.y - vp1.y)*0.5*page_scale);
    //cout << "drawarc rx=" << rx << " ry=" << ry << " vpc.x=" << vpc.x << " vpc.y=" << vpc.y << " a1=" << a1 << "a2=" << a2 << endl;
    if (rx == 0.0 || ry == 0.0)
    {
        return;
    }
    //rfactor = ry/rx;
rad=-RotationAngle*M_PI/180.0;
libHaru_HPDF_Page_GSave(page);
libHaru_HPDF_Page_Concat(page, cos(rad), sin(rad), -sin(rad), cos(rad), (float) vpc.x,(float) vpc.y);
//cout << "center= " << vpc.x*page_scale << " | "<< vpc.y*page_scale << " Radius=" << rx*page_scale << " a1=" << a1 << " a2=" << a2 << endl;
    //if(a1==0 && a2==360)
    if (a2-a1>=360)//a full circle
    {
    //libHaru_HPDF_Page_Ellipse(page,(float) vpc.x*page_scale,(float) vpc.y*page_scale,(float) rx*page_scale,(float) ry*page_scale);
    libHaru_HPDF_Page_Ellipse(page,(float) 0.0,(float) 0.0, rx, ry);
    }
    else
    {
    libHaru_HPDF_Page_Arc(page,(float) 0.0,(float) 0.0,(float) rx, 90-a2, 90-a1);
    }
    libHaru_HPDF_Page_Stroke(page);
libHaru_HPDF_Page_GRestore(page);
}

void haru_pdf_fillarc(VPoint vp1, VPoint vp2, int a1, int a2, int mode)
{
static VPoint vpc;
static float rx, ry;//rfactor;
static int pattern;
static double rad;
static int fg_color;

pattern=getpattern();
    if (pattern == 0 || pattern==8)//invisible patterns
    {
        return;
    }

    vpc.x = (vp1.x + vp2.x)*0.5*page_scale;
    vpc.y = (vp1.y + vp2.y)*0.5*page_scale;
    rx = (float)(fabs(vp2.x - vp1.x)*0.5*page_scale);
    ry = (float)(fabs(vp2.y - vp1.y)*0.5*page_scale);
    //cout << "fillarc rx=" << rx << " ry=" << ry << " vpc.x=" << vpc.x << " vpc.y=" << vpc.y << endl;
    if (rx == 0.0 || ry == 0.0)
    {
        return;
    }
    //rfactor = ry/rx;
rad=-RotationAngle*M_PI/180.0;
libHaru_HPDF_Page_GSave(page);
libHaru_HPDF_Page_Concat(page, cos(rad), sin(rad), -sin(rad), cos(rad), (float) vpc.x,(float) vpc.y);
    fg_color=getcolor();//for filling something
    fRGB * frgb = get_frgb(fg_color);
    libHaru_HPDF_Page_SetRGBFill(page, (float) frgb->red, (float) frgb->green, (float) frgb->blue);
    ///do not use Stroke here! only fill!
    if (mode==ARCFILL_PIESLICE)//only segment not pie
    {
    libHaru_HPDF_Page_MoveTo(page,0,0);
    libHaru_HPDF_Page_LineTo(page,cos(M_PI/180.0*(90.0-a2)),sin(M_PI/180.0*(90.0-a2)));
    }
    if(a1==0 && a2==360)//a full circle/ellipse
    {
    libHaru_HPDF_Page_Ellipse(page,(float) 0.0,(float) 0.0,(float) rx,(float) ry);
    }
    else
    {
    libHaru_HPDF_Page_Arc(page,(float) 0.0,(float) 0.0,(float) rx, 90-a2, 90-a1);
    }
    if (mode==ARCFILL_PIESLICE)//only segment not pie
    {
    libHaru_HPDF_Page_LineTo(page,0,0);
    }
    /*if (mode==ARCFILL_CHORD)//only segment not pie
    libHaru_HPDF_Page_Eofill(page);
    else*/
    libHaru_HPDF_Page_Fill(page);
libHaru_HPDF_Page_GRestore(page);
}

/* TODO: transparent pixmaps */
void haru_pdf_putpixmap(VPoint vp, int width, int height, char *databits,
                   int pixmap_bpp, int bitmap_pad, int pixmap_type)
{
    char *buf, *bp;
    /*int image;*/
    int cindex;
    RGB *fg, *bg;
    int	i, k, j;
    long paddedW;
    (void) pixmap_type;
    int components    = 3;

    buf = (char*) xmalloc(width*height*components);
    if (buf == NULL) {
        errmsg("xmalloc failed in haru pdf_putpixmap()");
        return;
    }

    bp = buf;
    if (pixmap_bpp == 1) {
        paddedW = PAD(width, bitmap_pad);
        fg = get_rgb(getcolor());
        bg = get_rgb(getbgcolor());
        for (k = 0; k < height; k++) {
            for (j = 0; j < paddedW/bitmap_pad; j++) {
                for (i = 0; i < bitmap_pad && j*bitmap_pad + i < width; i++) {
                    if (bin_dump(&(databits)[k*paddedW/bitmap_pad + j], i, bitmap_pad)) {
                        *bp++ = (char) fg->red;
                        *bp++ = (char) fg->green;
                        *bp++ = (char) fg->blue;
                    } else {
                        *bp++ = (char) bg->red;
                        *bp++ = (char) bg->green;
                        *bp++ = (char) bg->blue;
                    }
                }
            }
        }
    } else {
        for (k = 0; k < height; k++) {
            for (j = 0; j < width; j++) {
                cindex = (databits)[k*width + j];
                fg = get_rgb(cindex);
                *bp++ = (char) fg->red;
                *bp++ = (char) fg->green;
                *bp++ = (char) fg->blue;
            }
        }
    }

    /*image = PDF_open_image(phandle, "raw", "memory",
                           buf, width*height*components,
                           width, height, components, GRACE_BPP, "");
    if (image == -1) {
        errmsg("Not enough memory for image!");
        xfree(buf);
        return;
    }

    PDF_place_image(phandle, image, vp.x, vp.y - height*pixel_size, pixel_size);
    PDF_close_image(phandle, image);

    xfree(buf);*/

    /* Putting pixmap is not implemented yet */
    (void)vp;
}


#endif
