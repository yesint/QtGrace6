/*
 * Grace - GRaphing, Advanced Computation and Exploration of data
 * 
 * Home page: http://plasma-gate.weizmann.ac.il/Grace/
 * 
 * Copyright (c) 1991-1995 Paul J Turner, Portland, OR
 * Copyright (c) 1996-2002 Grace Development Team
 * 
 * Maintained by Evgeny Stambulchik
 * 
 * Modified by Andreas Winter 2008-2015
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
 * Global variables of Grace - should be empty :-(
 *
 */

#ifndef __GLOBALS_H_
#define __GLOBALS_H_

#include <stdlib.h>
#include <stdio.h>

#include "defines.h"
#include "graphs.h"
///#include "draw.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MAIN
#  define GLOBAL(var, type, val) type var = val
#  define GLOBALARR(arr, type, dim, val) type arr[dim] = val

/* target set */
target target_set;
/* timestamp */
plotstr timestamp;
/* default properties */
defaults grdefaults;
view grview;

/* parameters for non-linear fit */
nonlparms nonl_parms[MAXPARM];
nonlopts nonl_opts;

/* region definition */
region rg[MAXREGION+1];

#else
#  define GLOBAL(var, type, val) extern type var
#  define GLOBALARR(arr, type, dim, val) extern type arr[]

extern target target_set;
extern defaults grdefaults;
extern view grview;
extern plotstr timestamp;

extern nonlparms nonl_parms[];
extern nonlopts nonl_opts;

extern region rg[];

#endif

/* real-time input delay (prevents getting stuck reading) */
GLOBAL(timer_delay, int, 200);

/* linked scroll */
GLOBAL(scrolling_islinked, int, FALSE);
/* scroll fraction */
GLOBAL(scrollper, double, 0.05);
/* expand/shrink fraction */
GLOBAL(shexper, double, 0.05);

/* terminal device */
GLOBAL(tdevice, int, 0);
/* hardcopy device */
GLOBAL(hdevice, int, 0);

/* set mono mode */
GLOBAL(monomode, int, FALSE);
/* use GXxor or GXinvert for xor'ing */
GLOBAL(invert, int, TRUE);
/* if true, redraw graph each time action is performed */
GLOBAL(auto_redraw, int, TRUE);
/* allow double click ops */
GLOBAL(allow_dc, int, TRUE);
/* if TRUE, assume yes for everything */
GLOBAL(noask, int, FALSE);
/* if TRUE, then there is unsaved data*/
GLOBAL(dirtystate, int, FALSE);
GLOBAL(dirtysave, int, FALSE);
/* to tell what objects are to be drawn on an image - e.g. lines - making simple drawings much faster*/
/* simple draw means: we draw only a few lines on the background of the whole picture (graphs and everything) - this is useful if you want to draw a crosshair-cursor or a region or just a line in a project that has a lot of data points - much faster this way */
GLOBAL(simple_draw_setting, int, 0);
/* angle of rotation of various graphical objects */
GLOBAL(RotationAngle,signed int, 0);
/* a zoom factor to be able to plot very large or very small pages on a screen with fixed size */
GLOBAL(GeneralPageZoomFactor, double, 1.0);

/* if False, assume no for everything */
GLOBAL(cancelExport, int, FALSE);

#define MAGIC_LINEW_SCALE 0.0015

/* Export format devices */
/*#define DEVICE_PDF 0
#define DEVICE_PNG 1
#define DEVICE_PS 2
#define DEVICE_EPS 3
#define DEVICE_SVG 4
#define DEVICE_SCREEN 5*/
#define DEVICE_NULL -1
#define DEVICE_X11 0
#define DEVICE_SCREEN 0
//#define DEVICE_DUMMY 1
#define DEVICE_PDF 1
#define DEVICE_SVG 2
#define DEVICE_JPEG 3
#define DEVICE_PNG 4
#define DEVICE_HD_PNG 5
#define DEVICE_BMP 6
#define DEVICE_PS 7
#define DEVICE_EPS 8
#define DEVICE_MIF 9
#define DEVICE_PNM 10
#define DEVICE_METAFILE 11
#define DEVICE_PDF_HARU 12

//to tell the x11-Qt-driver what to paint on
#define PRINT_TARGET_SCREEN 0
#define PRINT_TARGET_PRINTER 1
#define PRINT_TARGET_SVG_FILE 2
#define PRINT_TARGET_PDF_FILE 3

/* true if running X */
GLOBAL(inwin, int, TRUE);

/*if FALSE disable console window*/
GLOBAL(disableConsole, int, TRUE);

/* autoscale after reading in data sets */
GLOBAL(autoscale_onread, int, AUTOSCALE_XY);

GLOBAL(focus_policy, int, FOCUS_CLICK);
GLOBAL(draw_focus_flag, int, TRUE);

/* lines */
GLOBAL(lines, linetype *, NULL);
/* boxes */
GLOBAL(boxes, boxtype *, NULL);
/* ellipses */
GLOBAL(ellip, ellipsetype *, NULL);
/* strings */
GLOBAL(pstr, plotstr *, NULL);

/* lines, boxes, ellipses and strings flags */
GLOBAL(box_color, int, 1);
GLOBAL(box_lines, int, 1);
GLOBAL(box_linew, double, 1.0);
GLOBAL(box_fillpat, int, 0);
GLOBAL(box_fillcolor, int, 1);
GLOBAL(box_loctype, int, COORD_VIEW);

GLOBAL(ellipse_color, int, 1);
GLOBAL(ellipse_lines, int, 1);
GLOBAL(ellipse_linew, double, 1.0);
GLOBAL(ellipse_fillpat, int, 0);
GLOBAL(ellipse_fillcolor, int, 1);
GLOBAL(ellipse_loctype, int, COORD_VIEW);

GLOBAL(line_color, int, 1);
GLOBAL(line_lines, int, 1);
GLOBAL(line_linew, double, 1.0);
GLOBAL(line_arrow_end, int, 0);
GLOBAL(line_atype, int, 0);
GLOBAL(line_asize, double, 1.0);
GLOBAL(line_a_dL_ff, double, 1.0);
GLOBAL(line_a_lL_ff, double, 1.0);
GLOBAL(line_loctype, int, COORD_VIEW);

GLOBAL(string_color, int, 1);
GLOBAL(string_font, int, 0);/*standard-font: 0=TimesRoman, 4=*/
GLOBAL(string_rot, int, 0);
GLOBAL(string_just, int, 0);
GLOBAL(string_size, double, 1.0);
GLOBAL(string_loctype, int, COORD_VIEW);

/* used in the parser */
GLOBAL(curtype, int, SET_XY);
GLOBAL(cursource, int, SOURCE_DISK);

/* the current region */
GLOBAL(nr, int, 0);

/* file for results */
GLOBAL(resfp, FILE *, NULL);

/* format for saving data sets */
GLOBALARR(sformat, char, 32, "%.8g");
GLOBALARR(grace_sformat, char, 32, "%.8g");
GLOBALARR(ini_sformat, char, 32, "%.8g");
GLOBALARR(file_sformat, char, 32, "%.8g");
GLOBALARR(tmp_sformat, char, 32, "%.8g");

#ifdef HAVE_NETCDF
GLOBALARR(netcdf_name, char, 512, "");
GLOBALARR(xvar_name, char, 128, "");
GLOBALARR(yvar_name, char, 128, "");
#endif

/* the safe mode flag */
GLOBAL(safe_mode, int, TRUE);

/* if xmgrace is to participate in a pipe */
GLOBAL(inpipe, int, FALSE);
/* if executed as 'gracebat' then TRUE */
GLOBAL(gracebat, int, FALSE);

GLOBALARR(sendToBeast,char,512,"");
GLOBALARR(readFromBeast,char,512,"");
//GLOBAL(connectToViewBeast,int,FALSE);
GLOBAL(enableServerMode,int,FALSE);
GLOBAL(hideMainWindow, int, FALSE);

GLOBAL(useHDPrinterOutput,int,FALSE);
GLOBAL(ScaleLineWidthByResolution,int,FALSE);
GLOBAL(auto_set_agr_extension,int,TRUE);
GLOBAL(auto_set_export_extensions,int,TRUE);
GLOBAL(auto_set_cwd,int,TRUE);

GLOBAL(universal_font_size_factor,double,1.0);
GLOBAL(warn_on_encoding_change,int,TRUE);

GLOBALARR(qt_grace_exe_dir,char,1024,"");
GLOBALARR(user_home_dir,char,1024,"");

GLOBAL(startupphase,char,1);

GLOBAL(use_fftw3,int,FALSE);
GLOBAL(have_fftw3,int,FALSE);
GLOBAL(use_libHaru,int,FALSE);
GLOBAL(have_libHaru,int,FALSE);

GLOBAL(show_ToolBar,int,TRUE);
GLOBAL(show_StatusBar,int,TRUE);
GLOBAL(show_LocatorBar,int,TRUE);

GLOBAL(show_Navi_B,int,TRUE);
GLOBAL(show_Graph_List,int,TRUE);
GLOBAL(show_special_Zoom,int,TRUE);
GLOBAL(show_Viewport_Stack,int,TRUE);
GLOBAL(show_Page_Zoom,int,TRUE);
GLOBAL(show_Print_B,int,FALSE);
GLOBAL(show_Export_B,int,FALSE);
GLOBAL(show_host_name,int,FALSE);
GLOBAL(show_display_name,int,FALSE);
GLOBAL(displ_project_filename,int,1);
GLOBAL(displ_export_filename,int,0);
GLOBAL(general_behavior,int,0);
GLOBAL(autofit_on_load,int,0);
GLOBAL(autofit_pending,int,0);
GLOBAL(activateLaTeXsupport,int,0);

GLOBAL(win_h,unsigned int,0);
GLOBAL(win_w,unsigned int,0);
GLOBAL(stop_repaint,int,FALSE);//to prevent multiple repaints
GLOBAL(start_dpi,int,72);//to adjust for screen resolution
GLOBALARR(print_file,char,GR_MAXPATHLEN,"");
GLOBAL(g, graph *, NULL);
#ifdef __cplusplus
} // closing brace for extern "C"

#endif

#endif /* __GLOBALS_H_ */
