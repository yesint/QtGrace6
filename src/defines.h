/*
 * Grace - GRaphing, Advanced Computation and Exploration of data
 * 
 * Home page: http://plasma-gate.weizmann.ac.il/Grace/
 * 
 * Copyright (c) 1991-1995 Paul J Turner, Portland, OR
 * Copyright (c) 1996-2000 Grace Development Team
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
 * constants and typedefs
 *
 */
#ifndef __DEFINES_H_
#define __DEFINES_H_

#include <stdio.h>
#include <string.h>

/*
 * some constants
 *
 */

/// #define DEBUG_OUT_LOG 1

#define READ_UTF8_ONLY 0

#define MAX_HISTORY 20
#define MAX_BIN_IMPORT_CHANNELS 32

#ifdef WINDOWS_SYSTEM
#define GRACE_HOME "C:\\"
#endif
#ifdef LINUX_SYSTEM
#define GRACE_HOME "/"
#endif
#ifdef MAC_SYSTEM
#define GRACE_HOME "/"
#endif

#define GRACE_PRINT_CMD "lpr"
#define GRACE_EDITOR "vi"
#ifdef MAC_SYSTEM
#define GRACE_HELPVIEWER "open /Applications/Safari.app \"%s\""
#else
    #ifdef LINUX_SYSTEM
    #define GRACE_HELPVIEWER "mozilla -remote openURL\\\\(%s,new-window\\\\) >>/dev/null 2>&1 || mozilla %s"
    #else
    #define GRACE_HELPVIEWER "iexplore %s &"
    #endif
#endif
//#define BI_VERSION_ID 50121
//#define BI_VERSION "Grace-5.1.22"
#define BI_GUI "Qt"
//"May 15th 2012"
#define BI_DATE __DATE__
///#define BI_SYSTEM "LINUX"
#define BI_T1LIB "T1"
#define QTGRACE_VERSION_STRING "v0.2.7"

#ifdef _MSC_VER
#define BI_CCOMPILER "Visual C++"
#else
#define BI_CCOMPILER __VERSION__
#endif

/* max path length */
#define GR_MAXPATHLEN 4096

/* max length for strings */
#define MAX_STRING_LENGTH 2048

#define MAXAXES 4               /* max number of axes per graph */
#define MAX_TICKS 256           /* max number of ticks/labels per axis */
#define MAXREGION 5             /* max number of regions */

#define MAX_ZOOM_STACK 1000       /* max stack depth for world stack */

#define MAXPARM 20              /* max number of parameters for non-lin fit */

#define MAXFIT 12               /* max degree of polynomial+1 that can be
                                 * fitted */

/* number of extra objects of a given type to allocate if not enough */
#define OBJECT_BUFNUM 10

/* number of extra columns (A,B,C,...) for postprocessing of csv-files */
#define MAX_DUMMY_COLS 15

#define MAX_ARROW 3
#define MAX_PREC 10

/* symbol types */

#define SYM_NONE    0
#define SYM_CIRCLE  1
#define SYM_SQUARE  2
#define SYM_DIAMOND 3
#define SYM_TRIANG1 4
#define SYM_TRIANG2 5
#define SYM_TRIANG3 6
#define SYM_TRIANG4 7
#define SYM_PLUS    8
#define SYM_X       9
#define SYM_SPLAT  10
#define SYM_CHAR   11

#define CHAR_SHIFT_NONE 0
#define CHAR_SHIFT_TO_SYMBOL 1
#define CHAR_SHIFT_TO_UPPERSET 2

#define OPTYPE_COPY 0
#define OPTYPE_MOVE 1
#define OPTYPE_SWAP 2
#define OPTYPE_COPY_LEGEND 3

/* max number of symbols defined */
#define MAXSYM  12

/* dot (obsolete) */
#define SYM_DOT_OBS     1

/*
 * types of coordinate frames
 */
#define COORDINATES_XY      0       /* Cartesian coordinates */
#define COORDINATES_POLAR   1       /* Polar coordinates */
#define COORDINATES_POLAR2  2       /* special Polar coordinates */
                                
/*
 * types of axis scale mappings
 */
#define SCALE_NORMAL    0       /* normal linear scale */
#define SCALE_LOG       1       /* logarithmic  scale */
#define SCALE_REC       2       /* reciprocal, reserved */
#define SCALE_LOGIT	    3	    /* logit scale */

/*
 * coordinates
 */
#define AXIS_TYPE_ANY -1
#define AXIS_TYPE_X    0
#define AXIS_TYPE_Y    1
#define AXIS_TYPE_BAD  2

/*
 * types of axes
 */
#define ALL_AXES    -3
#define ALL_X_AXES  -2
#define ALL_Y_AXES  -1
#define NO_AXIS -4
#define X_AXIS  0
#define Y_AXIS  1
#define ZX_AXIS 2
#define ZY_AXIS 3

/* setno == all sets selected */
#define ALL_SETS    -1
/* setno == new set to be created */
#define NEW_SET     -2

/*
 * gno == all graphs selected
 */
#define ALL_GRAPHS    -1

/* type of splines */
#define INTERP_LINEAR   0
#define INTERP_SPLINE   1
#define INTERP_ASPLINE  2

/* Canvas types */
#define PAGE_FREE       0
#define PAGE_FIXED      1

/* Strings and things */
#define OBJECT_NONE    -1
#define OBJECT_LINE     0
#define OBJECT_BOX      1
#define OBJECT_ELLIPSE  2
#define OBJECT_STRING   3

/* Region definitions */
#define REGION_ABOVE    0
#define REGION_BELOW    1
#define REGION_TOLEFT   2
#define REGION_TORIGHT  3
#define REGION_POLYI    4
#define REGION_POLYO    5
#define REGION_HORIZI   6
#define REGION_VERTI    7
#define REGION_HORIZO   8
#define REGION_VERTO    9

/* Axis label layout */
#define LAYOUT_PARALLEL         0
#define LAYOUT_PERPENDICULAR    1

/* States for simple drawing on background */
#define SIMPLE_DRAW_NONE 0
#define SIMPLE_DRAW_CROSSHAIR 1
#define SIMPLE_DRAW_LINE 2
#define SIMPLE_DRAW_REGION 4
#define SIMPLE_DRAW_HIGHLIGHT 8
#define SIMPLE_DRAW_RUBBER_DONUT 16
#define SIMPLE_DRAW_GRAPH_FOCUS 32
#define SIMPLE_DRAW_LOCATORFIXPOINT 64

/* Placement (axis labels, ticks, error bars) */
typedef enum {
    PLACEMENT_NORMAL,
    PLACEMENT_OPPOSITE,
    PLACEMENT_BOTH
} PlacementType;

/* Tick label placement */
#define LABEL_ONTICK    0
#define LABEL_BETWEEN   1

/* Coordinates */
#define COORD_VIEW      0
#define COORD_WORLD     1

/* Tick sign type */
#define SIGN_NORMAL     0
#define SIGN_ABSOLUTE   1
#define SIGN_NEGATE     2

/* Tick label/display formats */
#define FORMAT_STRING          -2
#define FORMAT_INVALID         -1
#define FORMAT_DECIMAL          0
#define FORMAT_EXPONENTIAL      1
#define FORMAT_GENERAL          2
#define FORMAT_POWER            3
#define FORMAT_SCIENTIFIC       4
#define FORMAT_ENGINEERING      5
#define FORMAT_COMPUTING        6
#define FORMAT_DDMMYY           7
#define FORMAT_MMDDYY           8
#define FORMAT_YYMMDD           9
#define FORMAT_MMYY            10
#define FORMAT_MMDD            11
#define FORMAT_MONTHDAY        12
#define FORMAT_DAYMONTH        13
#define FORMAT_MONTHS          14
#define FORMAT_MONTHSY         15
#define FORMAT_MONTHL          16
#define FORMAT_DAYOFWEEKS      17
#define FORMAT_DAYOFWEEKL      18
#define FORMAT_DAYOFYEAR       19
#define FORMAT_HMS             20
#define FORMAT_MMDDHMS         21
#define FORMAT_MMDDYYHMS       22
#define FORMAT_YYMMDDHMS       23
#define FORMAT_DEGREESLON      24
#define FORMAT_DEGREESMMLON    25
#define FORMAT_DEGREESMMSSLON  26
#define FORMAT_MMSSLON         27
#define FORMAT_DEGREESLAT      28
#define FORMAT_DEGREESMMLAT    29
#define FORMAT_DEGREESMMSSLAT  30
#define FORMAT_MMSSLAT         31
#define FORMAT_YYYY            32
#define FORMAT_YYDYHMS         33

#define DATE_SEPARATOR_MINUS 0
#define DATE_SEPARATOR_DOT 1
#define DATE_SEPARATOR_DASH 2

/* Focus policy */
#define FOCUS_CLICK     0
#define FOCUS_SET       1
#define FOCUS_FOLLOWS   2

/* Placement of labels etc */
#define TYPE_AUTO       0
#define TYPE_SPEC       1

/* User-defined tickmarks/labels */
#define TICKS_SPEC_NONE     0
#define TICKS_SPEC_MARKS    1
#define TICKS_SPEC_BOTH     2

/* Tick direction */
#define TICKS_IN        0
#define TICKS_OUT       1
#define TICKS_BOTH      2

/* Data source type */
#define SOURCE_DISK     0
#define SOURCE_PIPE     1

/* Types of running command */
#define RUN_AVG         0
#define RUN_MED         1
#define RUN_MIN         2
#define RUN_MAX         3
#define RUN_STD         4

/* Types of Fourier transforms */
#define FFT_FFT         0
#define FFT_INVFFT      1
#define FFT_DFT         2
#define FFT_INVDFT      3

/* return codes */
#define RETURN_SUCCESS (0)
#define RETURN_FAILURE (1)

#ifndef TRUE
#  define TRUE 1
#endif
#ifndef FALSE
#  define FALSE 0
#endif

/* types of autscales */
#define AUTOSCALE_NONE    0
#define AUTOSCALE_X       1
#define AUTOSCALE_Y       2
#define AUTOSCALE_XY      3

/*
 * for set selector gadgets
 */
#define SET_SELECT_ERROR -99
#define SET_SELECT_ACTIVE 0
#define SET_SELECT_ALL -1
#define SET_SELECT_NEXT -2
#define SET_SELECT_NEAREST -3
#define GRAPH_SELECT_CURRENT -1
#define GRAPH_SELECT_ALL -2
#define FILTER_SELECT_NONE 0
#define FILTER_SELECT_ACTIVE 1
#define FILTER_SELECT_ALL 2
#define FILTER_SELECT_INACT 3
#define FILTER_SELECT_DEACT 4
#define FILTER_SELECT_SORT 5
#define SELECTION_TYPE_SINGLE 0
#define SELECTION_TYPE_MULTIPLE 1

/* Default document name */
#define NONAME "Untitled"

/* for data pruning */
#define PRUNE_INTERPOLATION     0
#define PRUNE_CIRCLE            1
#define PRUNE_ELLIPSE           2
#define PRUNE_RECTANGLE         3

#define PRUNE_LIN               0
#define PRUNE_LOG               1

#define PRUNE_VIEWPORT          0
#define PRUNE_WORLD             1

/* for io filters */
#define FILTER_INPUT    0
#define FILTER_OUTPUT   1

#define FILTER_MAGIC    0
#define FILTER_PATTERN  1

/* set line types */
#define LINE_TYPE_NONE          0
#define LINE_TYPE_STRAIGHT      1
#define LINE_TYPE_LEFTSTAIR     2
#define LINE_TYPE_RIGHTSTAIR    3
#define LINE_TYPE_SEGMENT2      4
#define LINE_TYPE_SEGMENT3      5
#define LINE_TYPE_INCR_X        6
#define LINE_TYPE_DECR_X        7

/* baseline types */
#define BASELINE_TYPE_0         0
#define BASELINE_TYPE_SMIN      1
#define BASELINE_TYPE_SMAX      2
#define BASELINE_TYPE_GMIN      3
#define BASELINE_TYPE_GMAX      4
#define BASELINE_TYPE_SAVG      5

/* set fill types */
#define SETFILL_NONE            0
#define SETFILL_POLYGON         1
#define SETFILL_BASELINE        2

/* types of ann. values */
#define AVALUE_TYPE_NONE        0
#define AVALUE_TYPE_X           1
#define AVALUE_TYPE_Y           2
#define AVALUE_TYPE_XY          3
#define AVALUE_TYPE_STRING      4
#define AVALUE_TYPE_Z           5

/* ticks */
#define TICK_TYPE_MAJOR     0
#define TICK_TYPE_MINOR     1

/* push set direction */
#define PUSH_SET_TOFRONT    0
#define PUSH_SET_TOBACK     1

/* restriction types */
#define RESTRICT_NONE  -1
#define RESTRICT_WORLD -2
#define RESTRICT_REG0   0
#define RESTRICT_REG1   1
#define RESTRICT_REG2   2
#define RESTRICT_REG3   3
#define RESTRICT_REG4   4

/* features */
#define FEATURE_Y_MIN 0
#define FEATURE_Y_MAX 1
#define FEATURE_Y_AVG 2
#define FEATURE_Y_STD_DEV 3
#define FEATURE_Y_MEDIAN 4
#define FEATURE_X_MIN 5
#define FEATURE_X_MAX 6
#define FEATURE_X_AVG 7
#define FEATURE_X_STD_DEV 8
#define FEATURE_X_MEDIAN 9
#define FEATURE_FREQUENCY 10
#define FEATURE_PERIOD 11
#define FEATURE_ZERO_CROSSING 12
#define FEATURE_RISE_TIME 13
#define FEATURE_FALL_TIME 14
#define FEATURE_SLOPE 15
#define FEATURE_Y_INTERCEPT 16
#define FEATURE_SET_LENGTH 17
#define FEATURE_HALF_MAX_WIDTH 18
#define FEATURE_BARYCENTER_X 19
#define FEATURE_BARYCENTER_Y 20
#define FEATURE_X_OF_YMAX 21
#define FEATURE_Y_OF_XMAX 22
#define FEATURE_INTEGRAL 23
#define FEATURE_VALUE_CROSSING 24
#define FEATURE_VALUE_CROSSING2 25
#define FEATURE_X_OF_YMIN 26
#define FEATURE_Y_OF_XMIN 27

/* dataset-operations */
#define DATASETOP_SORT 0
#define DATASETOP_REVERSE 1
#define DATASETOP_JOIN 2
#define DATASETOP_SPLIT 3
#define DATASETOP_DROP 4
#define DATASETOP_RESTRICT 5
#define DATASETOP_SWAP_COLS 6
#define DATASETOP_AVERAGE_SET 7

/*cursors*/
#define CURSOR_LINE 0
#define CURSOR_FIND 1
#define CURSOR_TEXT 2
#define CURSOR_KILL 3
#define CURSOR_MOVE 4
#define CURSOR_WHAT 5
#define CURSOR_OPENHAND 6
#define CURSOR_CLOSEDHAND 7
#define CURSOR_POINTINGHAND 8
#define CURSOR_WAIT 9

/*image transformations*/
#define TRANSFORMATION_NONE 0
#define TRANSFORMATION_STRETCH_W 1
#define TRANSFORMATION_STRETCH_H 2
#define TRANSFORMATION_STRETCH_FIT 3
#define TRANSFORMATION_PATTERN 4

/*What to put into comment/legend after dropping files*/
#define DROP_LEGEND_NONE 0
#define DROP_LEGEND_PATH_FILE_COLUMN 1
#define DROP_LEGEND_PATH_FILE 2
#define DROP_LEGEND_FILE_COLUMN 3
#define DROP_LEGEND_FILE 4
#define DROP_LEGEND_COLUMN 5

/*How to behave concerning the ticks on the axes when packing graphs*/
#define TICK_PACK_UNCHANGED 0
#define TICK_PACK_LEFT 1
#define TICK_PACK_RIGHT 2
#define TICK_PACK_UP 1
#define TICK_PACK_DOWN 2
#define TICK_PACK_IN 4
#define TICK_PACK_OUT 8
#define TICK_PACK_NONE -2

#define TICK_PACK_DIRECTION_UNCHANGED 0
#define TICK_PACK_DIRECTION_IN 1
#define TICK_PACK_DIRECTION_OUT 2

#define DEFAULT_TICK_PACK_X_INNER (TICK_PACK_OUT | TICK_PACK_IN)
#define DEFAULT_TICK_PACK_Y_INNER (TICK_PACK_OUT | TICK_PACK_IN)
#define DEFAULT_TICK_PACK_X_OUTER (TICK_PACK_OUT | TICK_PACK_IN)
#define DEFAULT_TICK_PACK_Y_OUTER (TICK_PACK_OUT | TICK_PACK_IN)

#define DEFAULT_TICK_PACK_X_INNER_DIR (TICK_PACK_DIRECTION_IN)
#define DEFAULT_TICK_PACK_Y_INNER_DIR (TICK_PACK_DIRECTION_IN)
#define DEFAULT_TICK_PACK_X_OUTER_DIR (TICK_PACK_DIRECTION_IN)
#define DEFAULT_TICK_PACK_Y_OUTER_DIR (TICK_PACK_DIRECTION_IN)

#define SAMPLING_MESH           0
#define SAMPLING_SET            1
#define SAMPLING_INSIDE_BOUNDS  2
#define SAMPLING_EXPAND_KEEP_STEPPING    3
#define SAMPLING_LOG_MESH       4
#define SAMPLING_PARAMETRIC     5

#define OPERATION_NONE 0
#define OPERATION_PLUS 1
#define OPERATION_MINUS 2
#define OPERATION_MULT 3
#define OPERATION_DIV 4
#define OPERATION_POWER 5
#define OPERATION_10_POWER 6
#define OPERATION_2_POWER 7

#define COMPARISON_TOLERANCE 2

//#define AUTOLAYOUT_FOR_TOOLBAR 1

/*
 * defaults
 */
typedef struct {
    int color;
    int alpha;
    int bgcolor;
    int bgalpha;
    int pattern;
    int lines;
    double linew;
    double charsize;
    int font;
    double symsize;
} defaults;

typedef struct {
    int color;
    int pattern;
    int alpha;
} Pen;

/* A point in world coordinates */
typedef struct {
    double x;
    double y;
} WPoint;


/* A point in viewport coordinates */
typedef struct {
    double x;
    double y;
} VPoint;

typedef struct {
    double x;
    double y;
} VVector;

typedef struct {
    double xg1, xg2, yg1, yg2;  /* window into world coords */
} world;

typedef struct {
    double xv1, xv2, yv1, yv2;  /* viewport */
} view;

/*
 * information for an image to show somewhere
 */
typedef struct {
    int relpath;/*0=absolute paht, 1=relative path*/
    char imgpath[GR_MAXPATHLEN];/*the actual path*/
    int imgtransform;/*how to adjust the image to the available space:*/
    /*0=no transformation, just insert as it is --> image size changes with resolution, clipping at the borders
      1=stretch (or shrink) to fit the available width, keep aspect ratio
      2=stretch (or shrink) to fit the available height, keep aspect ratio
      3=stretch (or shrink) to fit all available space, does change aspect ratio
      4=use the image multiple times to create a tiled space*/
} imageinfo;

/*
 * typedefs for objects
 */
typedef struct {
    int active;
    int loctype;
    int gno;
    double x1;
    double y1;
    double x2;
    double y2;
    int lines;
    double linew;
    int color;
    int alpha;
    int fillcolor;
    int fillalpha;
    int fillpattern;
    view bb;
    int rot;
    int filltype;/*color or image*/
    imageinfo fillimage;/*information on where to find and how to use an image*/
} boxtype;

typedef struct {
    int type;
    double length;  /* head length (L) */
    double dL_ff;   /* d/L form factor */
    double lL_ff;   /* l/L form factor */
} Arrow;

typedef struct {
    int active;
    int loctype;
    int gno;
    double x1;
    double y1;
    double x2;
    double y2;
    int lines;
    double linew;
    int color;
    int alpha;
    int arrow_end;
    Arrow arrow;
    view bb;
} linetype;

typedef struct {
    int active;
    int loctype;
    int gno;
    double x1;
    double y1;
    double x2;
    double y2;
    int lines;
    double linew;
    int color;
    int alpha;
    int fillcolor;
    int fillalpha;
    int fillpattern;
    view bb;
    int rot;
    int filltype;/*color or image*/
    imageinfo fillimage;/*information on where to find and how to use an image*/
} ellipsetype;

typedef struct {
    int active;
    int path;//only for timestamp
    int loctype;
    int gno;
    double x;
    double y;
    int color;
    int alpha;
    int rot;
    int font;
    int just;
    int align;//added by Andreas Winter, 2018 (for text orientation in multi-line-texts)
    int master_align;//added by Andreas Winter, 2018 (for text-box orientation in graph or page)
    double charsize;
    char * s_plotstring;
    char * alt_plotstring;//added by Andreas Winter, 2011 (to keep original text in LaTeX-format)
    view bb;
} plotstr;

/*
 * world stack
 */
typedef struct {
    world w;                    /* current world */
    char * w_name;
} world_stack;

typedef struct {
    plotstr title;              /* graph title */
    plotstr stitle;             /* graph subtitle */
    VVector shift_title;        /* title shift vector */
    VVector shift_subtitle;     /* subtitle shift vector */
} labels;

typedef struct {
    int active;                 /* active flag */
    int type;                   /* regression type */
    double xmin;
    double xmax;
    double coef[15];
} Regression;

typedef struct {
    int active;                 /* active flag */
    int type;                   /* regression type */
    int npts;                   /* number of points */
    double xmin;
    double xmax;
    double *a;
    double *b;
    double *c;
    double *d;
} Spline;

typedef struct {
    int active;          /* on/off */
    int connect_bars;    /* instead of a bar-lines connect the error-bar-maxima or -minima*/
    int show_in_legend;  /* show error bars in legend */
    PlacementType ptype; /* placement type */
    Pen pen;             /* pen */
    double linew;        /* error bar line width */
    int lines;           /* error bar line style */
    double riser_linew;  /* connecting line between error limits line width */
    int riser_lines;     /* connecting line between error limits line style */
    double barsize;      /* size of error bar */
    int arrow_clip;      /* draw arrows if clipped */
    double cliplen;      /* riser clipped length (v.p.) */
} Errbar;

/* Annotative strings for data values */
typedef struct {
    int active;                 /* active or not */
    int type;                   /* type */
    double size;                /* char size */
    int font;                   /* font */
    int align;                  /* alignment */
    int color;                  /* color */
    int alpha;                  /* alpha-channel*/
    int angle;                  /* angle */
    int format;                 /* format */
    int prec;                   /* precision */
    char prestr[64];            /* prepend string */
    char appstr[64];            /* append string */
    char orig_prestr[64];       /* original prepend string */
    char orig_appstr[64];       /* original append string */
    VPoint offset;              /* offset related to symbol position */
} AValue;

typedef struct {
    int type;
    double wtpos;
    char *label;
    char *orig_label;
    view bb;
} tickloc;

typedef struct {
    double size;              /* length of tickmarks */
    int color;                /* color of tickmarks */
    int alpha;                /* alpha-channel */
    double linew;             /* linewidth of tickmarks */
    int lines;                /* linestyle of tickmarks */
    int gridflag;             /* grid lines at tick marks */
} tickprops;

#define QUICK_AXIS_TRANSFORM_NORMAL 0
#define QUICK_AXIS_TRANSFORM_DEGREES 1
#define QUICK_AXIS_TRANSFORM_MULT_PI 2
#define QUICK_AXIS_TRANSFORM_ALT_AXIS 3

typedef struct {
    int active;                 /* active or not */

    int zero;                   /* "zero" axis or plain */

    plotstr label;              /* graph axis label */
    int label_layout;           /* axis label orientation (h or v) */
    int label_place;            /* axis label placement (specfied or auto) */
    PlacementType label_op;     /* tick labels on opposite side or both */

    int t_drawbar;              /* draw a bar connecting tick marks */
    int t_drawbarcolor;         /* color of bar */
    int t_drawbaralpha;         /* alpha-channel */
    int t_drawbarlines;         /* linestyle of bar */
    double t_drawbarlinew;      /* line width of bar */

    double offsx, offsy;        /* offset of axes in viewport coords
                                    (attention: these
                                    are not x and y coordinates but
                                    perpendicular and parallel offsets */

    int t_flag;                 /* toggle tickmark display */
    int t_autonum;              /* approximate default number of major ticks */

    int t_spec;                 /* special (user-defined) tickmarks/ticklabels, */
                                /* can be none/marks/both marks and labels */

    int t_round;                /* place major ticks at rounded positions */

    double tmajor;              /* major tick divisions */
    int nminor;                 /* number of minor ticks per one major division */

    int nticks;                 /* total number of ticks */
    tickloc tloc[MAX_TICKS];    /* locations of ticks */

    int t_inout;                /* ticks inward, outward or both */
    PlacementType t_op;         /* ticks on opposite side */
    
    tickprops props;
    tickprops mprops;

    int tl_flag;                /* toggle tickmark labels on or off */
    int tl_angle;               /* angle to draw labels */

    int tl_format;              /* tickmark label format */
    int tl_prec;                /* places to right of decimal point */

    char *tl_formula;           /* transformation formula */

    int tl_skip;                /* tick labels to skip */
    int tl_staggered;           /* tick labels staggered */
    int tl_starttype;           /* start at graphmin or use tl_start/stop */
    int tl_stoptype;            /* start at graphmax or use tl_start/stop */
    double tl_start;            /* value of x to begin tick labels and major ticks */
    double tl_stop;             /* value of x to end tick labels and major ticks */

    PlacementType tl_op;        /* tick labels on opposite side or both */

    int tl_gaptype;             /* tick label placement auto or specified */
    VVector tl_gap;             /* tick label to tickmark distance
                                    (parallel and perpendicular to axis) */

    int tl_font;                /* font to use for tick labels */
    int tl_align;               /* alignment for tick labels */
    double tl_charsize;         /* character size for tick labels */
    int tl_color;               /* color of tick labels */
    int tl_alpha;               /* alpha-channel of tick labels */

    char tl_appstr[64];         /* append string to tick label */
    char tl_prestr[64];         /* prepend string to tick label */
    char orig_tl_appstr[64];    /* original append string to tick label */
    char orig_tl_prestr[64];    /* original prepend string to tick label */

    VPoint al_vp;               /* the start-position of the axis-label */
    view al_bb;                 /* bounding box for axis-label */
    view tl_bb;                 /* bounding box for all tick-labels */

} tickmarks;

typedef struct {
    int active;                 /* legend on or off */
    //int nr_of_entries;
    int loctype;                /* locate in world or viewport coords */
    int vgap;                   /* verticle gap between entries */
    int hgap;                   /* horizontal gap(s) between legend string
                                                                  elements */
    int len;                    /* length of line to draw */
    int invert;                 /* switch between ascending and descending
                                   order of set legends */
    double legx;                /* location on graph */
    double legy;
    int font;
    int align;
    double charsize;
    int color;
    int alpha;
    Pen boxpen;
    Pen boxfillpen;
    double boxlinew;            /* legend frame line width */
    int boxlines;               /* legend frame line style */
    view bb;
    int autoattach,autoattachG; /*if this is not 0 then the position is set to the be attached to a graph border*/
    double xshift,yshift;       /* the shift of the legend box so that it meets the attachement */
} legend;

typedef struct {
    int active;                 /* region on or off */
    int type;                   /* region type */
    int color;                  /* region color */
    int alpha;                  /* alpha-channel */
    int lines;                  /* region linestyle */
    double linew;               /* region line width */
    int linkto;                 /* associated with graph linkto */
    int n;                      /* number of points if type is POLY */
    double *x, *y;              /* coordinates if type is POLY */
    double x1, y1, x2, y2;      /* starting and ending points if type is not POLY */
} region;

typedef struct {
    int type;                   /* frame type */
    Pen pen;                    /* frame pen */
    int lines;                  /* frame linestyle */
    double linew;                  /* frame line width */
    Pen fillpen;                /* fill pen */
} framep;

/* parameters for non-linear fit */
typedef struct {
    double value;       /* parameter itself */
    int constr;         /* whether or not to use constraints */
    double min;         /* low bound constraint */
    double max;         /* upper bound constraint */
} nonlparms;

/* options for non-linear fit */
typedef struct {
    char *title;        /* fit title */
    char *formula;      /* fit function */
    int parnum;         /* # of fit parameters */
    double tolerance;   /* tolerance */
    /*the following parameter is not saved in a file*/
    int nsteps;          /* nr of iterations */
} nonlopts;

/* real time inputs */
typedef struct _Input_buffer {
    int           fd;     /* file descriptor */
    int           errors; /* number of successive parse errors */
    int           lineno; /* line number */
    int           zeros;  /* number of successive reads of zero byte */
    int           reopen; /* non-zero if we should close and reopen */
                          /* when other side is closed (mainly for fifos) */
    char         *name;   /* name of the input (filename or symbolic name) */
    int           size;   /* size of the buffer for already read lines */
    int           used;   /* number of bytes used in the buffer */
    char         *buf;    /* buffer for already read lines */
    unsigned long id;     /* id for X library */
} Input_buffer;

/* dates formats */
typedef enum   { FMT_iso,
                 FMT_european,
                 FMT_us,
                 FMT_nohint
               } Dates_format;

/* rounding types for dates */
#define NO_ROUND     0
#define ROUND_SECOND 1
#define ROUND_MINUTE 2
#define ROUND_HOUR   3
#define ROUND_DAY    4
#define ROUND_MONTH  5

/* tokens for the calendar dates parser */
typedef struct { int value;
                 int digits;
               } Int_token;

#define WSTACK_PUSH         0
#define WSTACK_POP          1
#define WSTACK_CYCLE        2
#define WSTACK_PUSH_ZOOM    3

#define NUM_FMT_OPTION_ITEMS 34

typedef struct {
    int value;
    const char *label;
} OptionItem;

/*static OptionItem as_option_items[4] =
{
    {AUTOSCALE_NONE, "None"},
    {AUTOSCALE_X,    "X"},
    {AUTOSCALE_Y,    "Y"},
    {AUTOSCALE_XY,   "XY"}
};*/

struct DIAdem_Global_Header
{
char origin[128];
char revision[128];
char description[128];
char comments[128];
char person[128];
char date[32];
char time[32];
char desc_comments[128];
char time_format[128];
double val_o_noval;
char exch_high_low[32];
};

struct DIAdem_Channel_Header
{
char name[128];
char comment[128];
char unit[32];
char ch_type[16];
char file_name[512];
int method;//0=BLOCK
char da_type[16];
char masking[32];
int points;
int d_pointer,offset,a_pointer;
char a_separator[8];
char dec_char[8];
char exp_char[8];
double start,step,minimum,maximum;
char noval_key[32];
char mono_key[32];
double val_o_noval;
char dat_display_key[32];
char RV[6][64];
int RI[6];
};

struct DIAdem_Header
{
int nr_of_channels;
int nr_of_real_channels,implicit_channel;
struct DIAdem_Global_Header global;
struct DIAdem_Channel_Header * channels;
};

#ifdef __cplusplus
#include <QString>
struct LatexCommands
{
    const char * la_com;
    char font;
    const char * ch;
    QString utf8;
    unsigned short unicode;
};
#endif

#endif /* __DEFINES_H_ */
