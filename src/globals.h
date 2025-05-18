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

/* if the next one is active around every text the bounding box is plotted as well (to see the positioning) */
//#define DEBUG_SHOW_TEXT_BOUNDING_BOXES 1

/* real-time input delay (prevents getting stuck reading) */
GLOBAL(timer_delay, int, 200);

/* linked scroll */
GLOBAL(scrolling_islinked, int, FALSE);
/* scroll fraction */
GLOBAL(scrollper, double, 0.05);
/* expand/shrink fraction */
GLOBAL(shexper, double, 0.05);

/* crosshair-cursor */
GLOBAL(cursortype, int, 0);

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
GLOBAL(save_no_ask, int, FALSE);
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

/* used to detect whether during a "overwrite file?"-question the user said "No"*/
GLOBAL(overwriteAnswerNo,int,FALSE);

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
#define DEVICE_BMP 5
#define DEVICE_TIFF 6
#define DEVICE_PS 7
#define DEVICE_EPS 8
#define DEVICE_MIF 9
#define DEVICE_PNM 10
#define DEVICE_METAFILE 11
#define DEVICE_PDF_HARU 12
#define DEVICE_HD_PNG 13

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
GLOBAL(clip_borders_for_output, int, FALSE);

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
GLOBAL(box_alpha, int, 255);
GLOBAL(box_lines, int, 1);
GLOBAL(box_linew, double, 1.0);
GLOBAL(box_fillpat, int, 0);
GLOBAL(box_fillcolor, int, 1);
GLOBAL(box_fillalpha, int, 255);
GLOBAL(box_filltype, int, 0);
GLOBAL(box_loctype, int, COORD_VIEW);

GLOBAL(ellipse_color, int, 1);
GLOBAL(ellipse_alpha, int, 255);
GLOBAL(ellipse_lines, int, 1);
GLOBAL(ellipse_linew, double, 1.0);
GLOBAL(ellipse_fillpat, int, 0);
GLOBAL(ellipse_fillcolor, int, 1);
GLOBAL(ellipse_fillalpha, int, 255);
GLOBAL(ellipse_filltype, int, 0);
GLOBAL(ellipse_loctype, int, COORD_VIEW);

GLOBAL(line_color, int, 1);
GLOBAL(line_alpha, int, 255);
GLOBAL(line_lines, int, 1);
GLOBAL(line_linew, double, 1.0);
GLOBAL(line_arrow_end, int, 0);
GLOBAL(line_atype, int, 0);
GLOBAL(line_asize, double, 1.0);
GLOBAL(line_a_dL_ff, double, 1.0);
GLOBAL(line_a_lL_ff, double, 1.0);
GLOBAL(line_loctype, int, COORD_VIEW);

GLOBAL(string_color, int, 1);
GLOBAL(string_alpha, int, 255);
GLOBAL(string_font, int, 0);/*standard-font: 0=TimesRoman, 4=*/
GLOBAL(string_rot, int, 0);
GLOBAL(string_just, int, 0);
GLOBAL(string_align, int, 0);
GLOBAL(string_master_align, int, 0);
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
GLOBALARR(sav_sformat, char, 32, "%.8g");

#ifdef HAVE_NETCDF
GLOBALARR(netcdf_name, char, 512, "");
GLOBALARR(xvar_name, char, 128, "");
GLOBALARR(yvar_name, char, 128, "");
#endif

GLOBALARR(qtgrace_help_viewer, char, 1024, GRACE_HELPVIEWER);
GLOBALARR(background_color, char, 32, "180;180;180");
GLOBALARR(file_codec_name, char, 128, "UTF-8");
GLOBAL(page_view_zoom,int,0);

GLOBAL(sync_device_dims,int,TRUE);
GLOBAL(rescale_plot_on_size_change,int,FALSE);
GLOBAL(showPageSettings,int,TRUE);
GLOBAL(showFontSettings,int,TRUE);

GLOBAL(dollar_t,char *, NULL);

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
GLOBAL(typicalPrinterResolution,int,300);/*resolution used for printing on a physical printer*/
GLOBAL(whoCalledThePrinter,int,0);/*from where the print-command has been issued: 0=Main Menu-->use default print settings; 1=from File Export Setup-->use the settings in the dialog (for more control)*/
GLOBAL(ScaleLineWidthByResolution,int,FALSE);
GLOBAL(auto_set_agr_extension,int,TRUE);
GLOBAL(auto_set_export_extensions,int,TRUE);
GLOBAL(auto_set_cwd,int,TRUE);

GLOBAL(toolBarSizeFactor,double,1.0);
GLOBAL(universal_font_size_factor,double,1.0);
GLOBAL(warn_on_encoding_change,int,TRUE);
GLOBAL(small_screen_adjustments,int,3);

GLOBALARR(qt_grace_share_dir,char,GR_MAXPATHLEN,"");/*the main directotry of QtGrace, gracerc is here*/
GLOBALARR(bin_dir,char,8,"");/*Name of the bin-subfolder (Windows: "bin", Mac OS X: "MacOS", linux: "templates"); this folder contains Default.agr and the default-bin-format-files*/
GLOBALARR(qt_grace_doc_dir,char,GR_MAXPATHLEN,"");/*where the helpfiles are, including readme and license*/
GLOBALARR(qt_grace_example_dir,char,GR_MAXPATHLEN,"");/*where the examples are*/
GLOBALARR(qt_grace_script_dir,char,GR_MAXPATHLEN,"");/*where the scripts are*/
GLOBALARR(qt_grace_icons_dir,char,GR_MAXPATHLEN,"");/*where the new icons are*/
GLOBALARR(qt_grace_templates_dir,char,GR_MAXPATHLEN,"");/*where the templates are (Fomat-files and Default.agr)*/
GLOBALARR(user_home_dir,char,GR_MAXPATHLEN,"");
GLOBALARR(wdir1,char,GR_MAXPATHLEN,"");
GLOBALARR(default_grace_file,char,GR_MAXPATHLEN,"");
GLOBALARR(default_csv_format,char,GR_MAXPATHLEN,"");/*the default format used for drag-and-drop of csv-files*/

GLOBAL(startupphase,char,1);
GLOBAL(barebonemainwindow,int,0);

GLOBAL(use_fftw3,int,FALSE);
GLOBAL(have_fftw3,int,FALSE);
GLOBAL(use_libHaru,int,FALSE);
GLOBAL(have_libHaru,int,FALSE);

GLOBAL(show_ToolBar,int,TRUE);
GLOBAL(show_StatusBar,int,TRUE);
GLOBAL(show_LocatorBar,int,TRUE);

GLOBAL(show_Navi_B,int,TRUE);
GLOBAL(show_Graph_List,int,TRUE);
GLOBAL(show_PanButton,int,TRUE);
GLOBAL(show_PickButton,int,TRUE);
GLOBAL(show_special_Zoom,int,TRUE);
GLOBAL(show_Viewport_Stack,int,TRUE);
GLOBAL(show_Page_Zoom,int,TRUE);
GLOBAL(show_Print_B,int,FALSE);
GLOBAL(show_Export_B,int,FALSE);
GLOBAL(show_UndoRedo_B,int,FALSE);
GLOBAL(show_host_name,int,FALSE);
GLOBAL(show_display_name,int,FALSE);
GLOBAL(show_transparency_selector,int,1);
GLOBAL(displ_project_filename,int,1);
GLOBAL(displ_export_filename,int,0);
GLOBAL(general_behavior,int,0);
GLOBAL(autofit_on_load,int,0);
GLOBAL(autofit_pending,int,0);
GLOBAL(activateLaTeXsupport,int,0);
GLOBAL(autoPackSets,int,FALSE);//if activated, all sets are automatically packed when a set is removed
GLOBAL(useErrorbarsInAutoscale,int,TRUE);//if activated, errorbars are considered when autoscaling
GLOBAL(minAutoscaleBorderOffset,double,0.0);//minimum offset after autoscale (so no points or errorbars lie on an axis)

GLOBAL(DefaultSetListShowHidden,int,TRUE);
GLOBAL(DefaultSetListShowDataless,int,FALSE);
GLOBAL(DefaultSetListShowIcons,int,FALSE);
GLOBAL(DefaultSetListShowComments,int,FALSE);
GLOBAL(DefaultSetListShowLegends,int,FALSE);

GLOBAL(SpreadsheetColumnFormat,int,1);
GLOBAL(SpreadsheetColumnPrecision,int,5);

GLOBAL(start_with_undo_active,int,1);
GLOBAL(undo_active,int,0);

GLOBAL(win_h,unsigned int,0);
GLOBAL(win_w,unsigned int,0);
GLOBAL(stop_repaint,int,FALSE);//to prevent multiple repaints
GLOBAL(start_dpi,int,72);//to adjust for screen resolution
GLOBALARR(print_file,char,GR_MAXPATHLEN,"");
GLOBAL(g, graph *, NULL);
GLOBAL(general_antialiasing,int,FALSE);//antialiasing for everything except fonts
GLOBAL(inline_editing,int,FALSE);//to use a line-editor directely inside the main window
GLOBAL(context_menu_in_main,int,FALSE);//open context-menus inside the main window
GLOBAL(icon_preferences,int,0);//how icons are displayed on buttons (0=text only, 1=icons+text, 2=icons only)
GLOBAL(comment_server_client,int,FALSE);//show messages about server-client-communication on stderr

GLOBAL(highlight_color,int,2);//the color used to hightlight sets or graphs
GLOBAL(highlight_alpha,int,255);//the alpha-channel used to hightlight sets or graphs
GLOBAL(highlight_time,int,3);//maximum time to hightlight sets or graphs (0=unlimited)
GLOBAL(auto_highlight,int,FALSE);//should a new selection automatically be highlighted in the main window
GLOBAL(highlight_errorbars,int,FALSE);//highlight error-bars and symbols as well as the lines

GLOBAL(allow_gestures,int,FALSE);
GLOBAL(allow_wheel_zoom,int,FALSE);//use the mouse-wheel for zooming
GLOBAL(allow_wheel_changes,int,FALSE);//use the mouse-wheel for font size changes

GLOBAL(show_setup_wizard,int,FALSE);
GLOBAL(block_setup_wizard,int,FALSE);
GLOBAL(setup_wizard_manual,int,FALSE);

GLOBAL(main_win_gr_no,int,-1);
GLOBAL(main_win_set_no,int,-1);

GLOBAL(initial_x_pos,int,65);
GLOBAL(initial_y_pos,int,65);
GLOBAL(initial_width,int,872);
GLOBAL(initial_height,int,670);
GLOBAL(initial_size_behavior,int,0);

GLOBAL(drop_autoscale,int,1);
GLOBAL(drop_comment,int,1);
GLOBAL(drop_legend,int,1);
GLOBAL(drop_dec_sep,char,'.');

GLOBAL(autoscale_active,int,0);

//for next saving operation
GLOBAL(current_compression_method,int,0);
GLOBAL(current_compression_level,int,0);

//for the next loading operation (what method to use and at which position the compressed data starts)
GLOBAL(compression_method,int,0);
GLOBAL(compressed_data_start_position,int,0);

//default settings
GLOBAL(default_compress_data,int,0);
GLOBAL(default_compression_level,int,3);

//redirection to ByteArray
GLOBAL(redirect_output_ba,int,0);
GLOBAL(inhibit_set_legend,int,0);

GLOBAL(allow_drag_in_lists,int,0);

GLOBAL(current_date_separator,int,0);

GLOBAL(collect_active,int,0);//to use the plot-routines to get the plot-lines
GLOBAL(active_graph,int,-1);//used during plotting to identify the graph that is currently plotted
GLOBAL(active_set,int,-1);//used during plotting to identify the set that is currently plotted

GLOBAL(yestoall,int,0);//for yes to all when importing multiple files (0=normal, 1=yestoall, 2=notoall)
GLOBAL(projectfileloading,int,0);//to tell the import routines that the data is read from a project file (i.e. no including filenames in comments)

#ifdef __cplusplus
} // closing brace for extern "C"

#endif

#endif /* __GLOBALS_H_ */
