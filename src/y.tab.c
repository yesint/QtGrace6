/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     KEY_VAR = 258,
     KEY_VEC = 259,
     KEY_CONST = 260,
     KEY_UNIT = 261,
     KEY_FUNC_I = 262,
     KEY_FUNC_D = 263,
     KEY_FUNC_NN = 264,
     KEY_FUNC_ND = 265,
     KEY_FUNC_DD = 266,
     KEY_FUNC_NND = 267,
     KEY_FUNC_PPD = 268,
     KEY_FUNC_PPPD = 269,
     KEY_FUNC_PPPPD = 270,
     KEY_FUNC_PPPPPD = 271,
     INDEX = 272,
     DATE = 273,
     VAR_D = 274,
     VEC_D = 275,
     CONSTANT = 276,
     UCONSTANT = 277,
     FUNC_I = 278,
     FUNC_D = 279,
     FUNC_NN = 280,
     FUNC_ND = 281,
     FUNC_DD = 282,
     FUNC_NND = 283,
     FUNC_PPD = 284,
     FUNC_PPPD = 285,
     FUNC_PPPPD = 286,
     FUNC_PPPPPD = 287,
     ABOVE = 288,
     ABSOLUTE = 289,
     ALIAS = 290,
     ALT = 291,
     ALTXAXIS = 292,
     ALTYAXIS = 293,
     ANGLE = 294,
     ANTIALIASING = 295,
     APPEND = 296,
     ARRANGE = 297,
     ARROW = 298,
     ASCENDING = 299,
     ASPLINE = 300,
     AUTO = 301,
     AUTOSCALE = 302,
     AUTOTICKS = 303,
     AVALUE = 304,
     AVG = 305,
     BACKGROUND = 306,
     BAR = 307,
     BARDY = 308,
     BARDYDY = 309,
     BASELINE = 310,
     BATCH = 311,
     BEGIN = 312,
     BELOW = 313,
     BETWEEN = 314,
     BLACKMAN = 315,
     BLOCK = 316,
     BOTH = 317,
     BOTTOM = 318,
     BOX = 319,
     CD = 320,
     CENTER = 321,
     CHAR = 322,
     CHART = 323,
     CHRSTR = 324,
     CLEAR = 325,
     CLICK = 326,
     CLIP = 327,
     CLOSE = 328,
     COEFFICIENTS = 329,
     COLOR = 330,
     COMMENT = 331,
     COMPLEX = 332,
     COMPUTING = 333,
     CONSTRAINTS = 334,
     COPY = 335,
     CYCLE = 336,
     DAYMONTH = 337,
     DAYOFWEEKL = 338,
     DAYOFWEEKS = 339,
     DAYOFYEAR = 340,
     DDMMYY = 341,
     DECIMAL = 342,
     DEF = 343,
     DEFAULT = 344,
     DEFINE = 345,
     DEGREESLAT = 346,
     DEGREESLON = 347,
     DEGREESMMLAT = 348,
     DEGREESMMLON = 349,
     DEGREESMMSSLAT = 350,
     DEGREESMMSSLON = 351,
     DESCENDING = 352,
     DESCRIPTION = 353,
     DEVICE = 354,
     DFT = 355,
     DIFFERENCE = 356,
     DISK = 357,
     DOWN = 358,
     DPI = 359,
     DROP = 360,
     DROPLINE = 361,
     ECHO = 362,
     ELLIPSE = 363,
     ENGINEERING = 364,
     ERRORBAR = 365,
     EXIT = 366,
     EXPONENTIAL = 367,
     FFT = 368,
     FILEP = 369,
     FILL = 370,
     FIT = 371,
     FIXED = 372,
     FIXEDPOINT = 373,
     FLUSH = 374,
     FOCUS = 375,
     FOLLOWS = 376,
     FONTP = 377,
     FORCE = 378,
     FORMAT = 379,
     FORMULA = 380,
     FRAMEP = 381,
     FREE = 382,
     FREQUENCY = 383,
     FROM = 384,
     GENERAL = 385,
     GETP = 386,
     GRAPH = 387,
     GRAPHNO = 388,
     GRID = 389,
     HAMMING = 390,
     HANNING = 391,
     HARDCOPY = 392,
     HBAR = 393,
     HELP = 394,
     HGAP = 395,
     HIDDEN = 396,
     HISTOGRAM = 397,
     HMS = 398,
     HORIZI = 399,
     HORIZONTAL = 400,
     HORIZO = 401,
     ID = 402,
     IFILTER = 403,
     IMAX = 404,
     IMIN = 405,
     IN = 406,
     INCREMENT = 407,
     INOUT = 408,
     INT = 409,
     INTEGRATE = 410,
     INTERPOLATE = 411,
     INVDFT = 412,
     INVERT = 413,
     INVFFT = 414,
     JUST = 415,
     KILL = 416,
     LABEL = 417,
     LANDSCAPE = 418,
     LAYOUT = 419,
     LEFT = 420,
     LEGEND = 421,
     LENGTH = 422,
     LINCONV = 423,
     LINE = 424,
     LINEAR = 425,
     LINESTYLE = 426,
     LINEWIDTH = 427,
     LINK = 428,
     LOAD = 429,
     LOCTYPE = 430,
     LOG = 431,
     LOGARITHMIC = 432,
     LOGIT = 433,
     LOGX = 434,
     LOGXY = 435,
     LOGY = 436,
     MAGIC = 437,
     MAGNITUDE = 438,
     MAJOR = 439,
     MAP = 440,
     MAXP = 441,
     MESH = 442,
     MINP = 443,
     MINOR = 444,
     MMDD = 445,
     MMDDHMS = 446,
     MMDDYY = 447,
     MMDDYYHMS = 448,
     MMSSLAT = 449,
     MMSSLON = 450,
     MMYY = 451,
     MONTHDAY = 452,
     MONTHL = 453,
     MONTHS = 454,
     MONTHSY = 455,
     MOVE = 456,
     NEGATE = 457,
     NEW = 458,
     NONE = 459,
     NONLFIT = 460,
     NORMAL = 461,
     NXY = 462,
     OFF = 463,
     OFFSET = 464,
     OFFSETX = 465,
     OFFSETY = 466,
     OFILTER = 467,
     ON = 468,
     ONREAD = 469,
     OP = 470,
     OPPOSITE = 471,
     OUT = 472,
     PAGE = 473,
     PARA = 474,
     PARAMETERS = 475,
     PARZEN = 476,
     PATTERN = 477,
     PERIOD = 478,
     PERP = 479,
     PHASE = 480,
     PIE = 481,
     PIPE = 482,
     PLACE = 483,
     POINT = 484,
     POLAR = 485,
     POLYI = 486,
     POLYO = 487,
     POP = 488,
     PORTRAIT = 489,
     POWER = 490,
     PREC = 491,
     PREPEND = 492,
     PRINT = 493,
     PS = 494,
     PUSH = 495,
     PUTP = 496,
     RAND = 497,
     READ = 498,
     REAL = 499,
     RECIPROCAL = 500,
     REDRAW = 501,
     REFERENCE = 502,
     REGNUM = 503,
     REGRESS = 504,
     RESIZE = 505,
     RESTRICT = 506,
     REVERSE = 507,
     RIGHT = 508,
     RISER = 509,
     ROT = 510,
     ROUNDED = 511,
     RSUM = 512,
     RULE = 513,
     RUNAVG = 514,
     RUNMAX = 515,
     RUNMED = 516,
     RUNMIN = 517,
     RUNSTD = 518,
     SAVEALL = 519,
     SCALE = 520,
     SCIENTIFIC = 521,
     SCROLL = 522,
     SD = 523,
     SET = 524,
     SETNUM = 525,
     SFORMAT = 526,
     SIGN = 527,
     SIZE = 528,
     SKIP = 529,
     SLEEP = 530,
     SMITH = 531,
     SORT = 532,
     SOURCE = 533,
     SPEC = 534,
     SPLINE = 535,
     SPLIT = 536,
     STACK = 537,
     STACKED = 538,
     STACKEDBAR = 539,
     STACKEDHBAR = 540,
     STAGGER = 541,
     START = 542,
     STOP = 543,
     STRING = 544,
     SUM = 545,
     SUBTITLE = 546,
     SWAP = 547,
     SYMBOL = 548,
     TARGET = 549,
     TICKLABEL = 550,
     TICKP = 551,
     TICKSP = 552,
     TIMER = 553,
     TIMESTAMP = 554,
     TITLE = 555,
     TO = 556,
     TOP = 557,
     TRIANGULAR = 558,
     TYPE = 559,
     UP = 560,
     UPDATEALL = 561,
     USE = 562,
     VERSION = 563,
     VERTI = 564,
     VERTICAL = 565,
     VERTO = 566,
     VGAP = 567,
     VIEW = 568,
     VX1 = 569,
     VX2 = 570,
     VXMAX = 571,
     VY1 = 572,
     VY2 = 573,
     VYMAX = 574,
     WELCH = 575,
     WITH = 576,
     WORLD = 577,
     WRAP = 578,
     WRITE = 579,
     WX1 = 580,
     WX2 = 581,
     WY1 = 582,
     WY2 = 583,
     X_TOK = 584,
     X0 = 585,
     X1 = 586,
     XAXES = 587,
     XAXIS = 588,
     XCOR = 589,
     XMAX = 590,
     XMIN = 591,
     XY = 592,
     XYAXES = 593,
     XYBOXPLOT = 594,
     XYCOLOR = 595,
     XYCOLPAT = 596,
     XYDX = 597,
     XYDXDX = 598,
     XYDXDXDYDY = 599,
     XYDXDY = 600,
     XYDY = 601,
     XYDYDY = 602,
     XYHILO = 603,
     XYR = 604,
     XYSIZE = 605,
     XYSTRING = 606,
     XYVMAP = 607,
     XYZ = 608,
     Y_TOK = 609,
     Y0 = 610,
     Y1 = 611,
     Y2 = 612,
     Y3 = 613,
     Y4 = 614,
     YAXES = 615,
     YAXIS = 616,
     YEAR = 617,
     YMAX = 618,
     YMIN = 619,
     YYMMDD = 620,
     YYMMDDHMS = 621,
     ZERO = 622,
     ZNORM = 623,
     FITPARM = 624,
     FITPMAX = 625,
     FITPMIN = 626,
     NUMBER = 627,
     NEW_TOKEN = 628,
     OR = 629,
     AND = 630,
     NE = 631,
     EQ = 632,
     GE = 633,
     LE = 634,
     LT = 635,
     GT = 636,
     NOT = 637,
     UMINUS = 638
   };
#endif
/* Tokens.  */
#define KEY_VAR 258
#define KEY_VEC 259
#define KEY_CONST 260
#define KEY_UNIT 261
#define KEY_FUNC_I 262
#define KEY_FUNC_D 263
#define KEY_FUNC_NN 264
#define KEY_FUNC_ND 265
#define KEY_FUNC_DD 266
#define KEY_FUNC_NND 267
#define KEY_FUNC_PPD 268
#define KEY_FUNC_PPPD 269
#define KEY_FUNC_PPPPD 270
#define KEY_FUNC_PPPPPD 271
#define INDEX 272
#define DATE 273
#define VAR_D 274
#define VEC_D 275
#define CONSTANT 276
#define UCONSTANT 277
#define FUNC_I 278
#define FUNC_D 279
#define FUNC_NN 280
#define FUNC_ND 281
#define FUNC_DD 282
#define FUNC_NND 283
#define FUNC_PPD 284
#define FUNC_PPPD 285
#define FUNC_PPPPD 286
#define FUNC_PPPPPD 287
#define ABOVE 288
#define ABSOLUTE 289
#define ALIAS 290
#define ALT 291
#define ALTXAXIS 292
#define ALTYAXIS 293
#define ANGLE 294
#define ANTIALIASING 295
#define APPEND 296
#define ARRANGE 297
#define ARROW 298
#define ASCENDING 299
#define ASPLINE 300
#define AUTO 301
#define AUTOSCALE 302
#define AUTOTICKS 303
#define AVALUE 304
#define AVG 305
#define BACKGROUND 306
#define BAR 307
#define BARDY 308
#define BARDYDY 309
#define BASELINE 310
#define BATCH 311
#define BEGIN 312
#define BELOW 313
#define BETWEEN 314
#define BLACKMAN 315
#define BLOCK 316
#define BOTH 317
#define BOTTOM 318
#define BOX 319
#define CD 320
#define CENTER 321
#define CHAR 322
#define CHART 323
#define CHRSTR 324
#define CLEAR 325
#define CLICK 326
#define CLIP 327
#define CLOSE 328
#define COEFFICIENTS 329
#define COLOR 330
#define COMMENT 331
#define COMPLEX 332
#define COMPUTING 333
#define CONSTRAINTS 334
#define COPY 335
#define CYCLE 336
#define DAYMONTH 337
#define DAYOFWEEKL 338
#define DAYOFWEEKS 339
#define DAYOFYEAR 340
#define DDMMYY 341
#define DECIMAL 342
#define DEF 343
#define DEFAULT 344
#define DEFINE 345
#define DEGREESLAT 346
#define DEGREESLON 347
#define DEGREESMMLAT 348
#define DEGREESMMLON 349
#define DEGREESMMSSLAT 350
#define DEGREESMMSSLON 351
#define DESCENDING 352
#define DESCRIPTION 353
#define DEVICE 354
#define DFT 355
#define DIFFERENCE 356
#define DISK 357
#define DOWN 358
#define DPI 359
#define DROP 360
#define DROPLINE 361
#define ECHO 362
#define ELLIPSE 363
#define ENGINEERING 364
#define ERRORBAR 365
#define EXIT 366
#define EXPONENTIAL 367
#define FFT 368
#define FILEP 369
#define FILL 370
#define FIT 371
#define FIXED 372
#define FIXEDPOINT 373
#define FLUSH 374
#define FOCUS 375
#define FOLLOWS 376
#define FONTP 377
#define FORCE 378
#define FORMAT 379
#define FORMULA 380
#define FRAMEP 381
#define FREE 382
#define FREQUENCY 383
#define FROM 384
#define GENERAL 385
#define GETP 386
#define GRAPH 387
#define GRAPHNO 388
#define GRID 389
#define HAMMING 390
#define HANNING 391
#define HARDCOPY 392
#define HBAR 393
#define HELP 394
#define HGAP 395
#define HIDDEN 396
#define HISTOGRAM 397
#define HMS 398
#define HORIZI 399
#define HORIZONTAL 400
#define HORIZO 401
#define ID 402
#define IFILTER 403
#define IMAX 404
#define IMIN 405
#define IN 406
#define INCREMENT 407
#define INOUT 408
#define INT 409
#define INTEGRATE 410
#define INTERPOLATE 411
#define INVDFT 412
#define INVERT 413
#define INVFFT 414
#define JUST 415
#define KILL 416
#define LABEL 417
#define LANDSCAPE 418
#define LAYOUT 419
#define LEFT 420
#define LEGEND 421
#define LENGTH 422
#define LINCONV 423
#define LINE 424
#define LINEAR 425
#define LINESTYLE 426
#define LINEWIDTH 427
#define LINK 428
#define LOAD 429
#define LOCTYPE 430
#define LOG 431
#define LOGARITHMIC 432
#define LOGIT 433
#define LOGX 434
#define LOGXY 435
#define LOGY 436
#define MAGIC 437
#define MAGNITUDE 438
#define MAJOR 439
#define MAP 440
#define MAXP 441
#define MESH 442
#define MINP 443
#define MINOR 444
#define MMDD 445
#define MMDDHMS 446
#define MMDDYY 447
#define MMDDYYHMS 448
#define MMSSLAT 449
#define MMSSLON 450
#define MMYY 451
#define MONTHDAY 452
#define MONTHL 453
#define MONTHS 454
#define MONTHSY 455
#define MOVE 456
#define NEGATE 457
#define NEW 458
#define NONE 459
#define NONLFIT 460
#define NORMAL 461
#define NXY 462
#define OFF 463
#define OFFSET 464
#define OFFSETX 465
#define OFFSETY 466
#define OFILTER 467
#define ON 468
#define ONREAD 469
#define OP 470
#define OPPOSITE 471
#define OUT 472
#define PAGE 473
#define PARA 474
#define PARAMETERS 475
#define PARZEN 476
#define PATTERN 477
#define PERIOD 478
#define PERP 479
#define PHASE 480
#define PIE 481
#define PIPE 482
#define PLACE 483
#define POINT 484
#define POLAR 485
#define POLYI 486
#define POLYO 487
#define POP 488
#define PORTRAIT 489
#define POWER 490
#define PREC 491
#define PREPEND 492
#define PRINT 493
#define PS 494
#define PUSH 495
#define PUTP 496
#define RAND 497
#define READ 498
#define REAL 499
#define RECIPROCAL 500
#define REDRAW 501
#define REFERENCE 502
#define REGNUM 503
#define REGRESS 504
#define RESIZE 505
#define RESTRICT 506
#define REVERSE 507
#define RIGHT 508
#define RISER 509
#define ROT 510
#define ROUNDED 511
#define RSUM 512
#define RULE 513
#define RUNAVG 514
#define RUNMAX 515
#define RUNMED 516
#define RUNMIN 517
#define RUNSTD 518
#define SAVEALL 519
#define SCALE 520
#define SCIENTIFIC 521
#define SCROLL 522
#define SD 523
#define SET 524
#define SETNUM 525
#define SFORMAT 526
#define SIGN 527
#define SIZE 528
#define SKIP 529
#define SLEEP 530
#define SMITH 531
#define SORT 532
#define SOURCE 533
#define SPEC 534
#define SPLINE 535
#define SPLIT 536
#define STACK 537
#define STACKED 538
#define STACKEDBAR 539
#define STACKEDHBAR 540
#define STAGGER 541
#define START 542
#define STOP 543
#define STRING 544
#define SUM 545
#define SUBTITLE 546
#define SWAP 547
#define SYMBOL 548
#define TARGET 549
#define TICKLABEL 550
#define TICKP 551
#define TICKSP 552
#define TIMER 553
#define TIMESTAMP 554
#define TITLE 555
#define TO 556
#define TOP 557
#define TRIANGULAR 558
#define TYPE 559
#define UP 560
#define UPDATEALL 561
#define USE 562
#define VERSION 563
#define VERTI 564
#define VERTICAL 565
#define VERTO 566
#define VGAP 567
#define VIEW 568
#define VX1 569
#define VX2 570
#define VXMAX 571
#define VY1 572
#define VY2 573
#define VYMAX 574
#define WELCH 575
#define WITH 576
#define WORLD 577
#define WRAP 578
#define WRITE 579
#define WX1 580
#define WX2 581
#define WY1 582
#define WY2 583
#define X_TOK 584
#define X0 585
#define X1 586
#define XAXES 587
#define XAXIS 588
#define XCOR 589
#define XMAX 590
#define XMIN 591
#define XY 592
#define XYAXES 593
#define XYBOXPLOT 594
#define XYCOLOR 595
#define XYCOLPAT 596
#define XYDX 597
#define XYDXDX 598
#define XYDXDXDYDY 599
#define XYDXDY 600
#define XYDY 601
#define XYDYDY 602
#define XYHILO 603
#define XYR 604
#define XYSIZE 605
#define XYSTRING 606
#define XYVMAP 607
#define XYZ 608
#define Y_TOK 609
#define Y0 610
#define Y1 611
#define Y2 612
#define Y3 613
#define Y4 614
#define YAXES 615
#define YAXIS 616
#define YEAR 617
#define YMAX 618
#define YMIN 619
#define YYMMDD 620
#define YYMMDDHMS 621
#define ZERO 622
#define ZNORM 623
#define FITPARM 624
#define FITPMAX 625
#define FITPMIN 626
#define NUMBER 627
#define NEW_TOKEN 628
#define OR 629
#define AND 630
#define NE 631
#define EQ 632
#define GE 633
#define LE 634
#define LT 635
#define GT 636
#define NOT 637
#define UMINUS 638




/* Copy the first part of user declarations.  */
#line 1 "pars.yacc"

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
 * evaluate expressions, commands, parameter files
 * 
 */


#include "cmath.h"

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <cctype>
#if defined(HAVE_SYS_PARAM_H)
#  include <sys/param.h>
#endif

/* bison not always handles it well itself */
#if defined(HAVE_ALLOCA_H)
#  include <alloca.h>
#endif

#include "defines.h"
#include "globals.h"
#include "cephes.h"
#include "device.h"
#include "utils.h"
#include "files.h"
#include "graphs.h"
#include "graphutils.h"
#include "plotone.h"
#include "dlmodule.h"
#include "t1fonts.h"
#include "ssdata.h"
#include "protos.h"
#include "noxparser.h"
#include "mathstuff.h"

#define MAX_PARS_STRING_LENGTH  4096

#define CAST_DBL_TO_BOOL(x) (fabs(x) < 0.5 ? 0:1)

typedef double (*ParserFnc)();

extern graph *g;

static double  s_result;    /* return value if a scalar expression is scanned*/
static grarr *v_result;    /* return value if a vector expression is scanned*/

static int expr_parsed, vexpr_parsed;

static int interr;

static grarr freelist[100]; 	/* temporary vectors */
static int fcnt = 0;		/* number of the temporary vectors allocated */

static target trgt_pool[100]; 	/* pool of temporary targets */
static int tgtn = 0;		/* number of the temporary targets used */

int naxis = 0;	/* current axis */
static int curline, curbox, curellipse, curstring;
/* these guys attempt to avoid reentrancy problems */
static int gotparams = FALSE, gotread = FALSE, gotnlfit = FALSE; 
int readxformat;
static int nlfit_gno, nlfit_setno, nlfit_nsteps;
static double *nlfit_warray = NULL;

char batchfile[GR_MAXPATHLEN] = "",
     paramfile[GR_MAXPATHLEN] = "",
     readfile[GR_MAXPATHLEN] = "";

static char f_string[MAX_PARS_STRING_LENGTH]; /* buffer for string to parse */
static int pos;

/* the graph, set, and its length of the parser's current state */
static int whichgraph;
static int whichset;

/* the graph and set of the left part of a vector assignment */
static int vasgn_gno;
static int vasgn_setno;

static int alias_force = FALSE; /* controls whether aliases can override
                                                       existing keywords */

extern char print_file[];
extern char *close_input;

static int filltype_obs;

static int index_shift = 0;     /* 0 for C, 1 for F77 index notation */

static void free_tmpvrbl(grarr *vrbl);
static void copy_vrbl(grarr *dest, grarr *src);
static int find_set_bydata(double *data, target *tgt);

static int getcharstr(void);
static void ungetchstr(void);
static int follow(int expect, int ifyes, int ifno);

static int yylex(void);
static int yyparse(void);
static void yyerror(char *s);

static int findf(symtab_entry *keytable, char *s);

/* Total (intrinsic + user-defined) list of functions and keywords */
symtab_entry *key;



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 142 "pars.yacc"
{
    int     ival;
    double  dval;
    char   *sval;
    double *dptr;
    target *trgt;
    grarr  *vrbl;
}
/* Line 193 of yacc.c.  */
#line 1012 "y.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 1025 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  523
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   11867

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  400
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  77
/* YYNRULES -- Number of rules.  */
#define YYNRULES  839
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1673

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   638

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   388,     2,     2,
     394,   395,   386,   384,   396,   385,   392,   387,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   375,   393,
       2,   399,     2,   374,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   397,     2,   398,   391,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   376,
     377,   378,   379,   380,   381,   382,   383,   389,   390
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    15,    16,    18,
      20,    22,    24,    26,    28,    30,    32,    34,    36,    38,
      40,    42,    44,    46,    48,    51,    56,    63,    67,    71,
      75,    79,    81,    84,    86,    91,    96,   103,   110,   117,
     126,   135,   146,   159,   174,   178,   182,   186,   190,   194,
     198,   202,   206,   211,   220,   235,   237,   239,   241,   243,
     245,   247,   249,   251,   253,   255,   259,   263,   267,   270,
     273,   277,   281,   285,   289,   295,   299,   303,   307,   311,
     315,   319,   323,   327,   330,   332,   336,   340,   342,   344,
     348,   350,   352,   354,   356,   358,   360,   364,   366,   373,
     378,   387,   392,   397,   402,   407,   412,   419,   426,   433,
     440,   449,   458,   469,   482,   497,   501,   505,   509,   513,
     517,   521,   525,   529,   533,   537,   541,   545,   549,   553,
     557,   561,   565,   569,   572,   578,   584,   590,   596,   600,
     604,   608,   612,   616,   620,   624,   628,   632,   636,   640,
     644,   648,   652,   656,   660,   664,   668,   672,   676,   680,
     684,   688,   692,   695,   699,   702,   706,   710,   714,   718,
     723,   725,   729,   733,   736,   741,   747,   750,   753,   756,
     759,   763,   767,   774,   783,   786,   790,   793,   796,   799,
     809,   815,   820,   823,   829,   835,   843,   848,   854,   859,
     864,   868,   872,   876,   881,   884,   889,   894,   899,   903,
     913,   916,   919,   922,   925,   930,   934,   937,   941,   944,
     947,   956,   960,   963,   966,   969,   973,   977,   980,   983,
     987,   990,   993,  1002,  1006,  1009,  1012,  1015,  1019,  1023,
    1026,  1029,  1033,  1036,  1039,  1048,  1052,  1055,  1058,  1061,
    1065,  1070,  1075,  1082,  1085,  1088,  1092,  1095,  1098,  1103,
    1107,  1110,  1114,  1117,  1121,  1126,  1130,  1133,  1136,  1141,
    1145,  1148,  1153,  1157,  1160,  1163,  1166,  1169,  1174,  1177,
    1182,  1186,  1194,  1208,  1217,  1220,  1229,  1232,  1235,  1239,
    1242,  1245,  1248,  1252,  1255,  1259,  1263,  1267,  1271,  1275,
    1279,  1283,  1287,  1290,  1293,  1296,  1300,  1304,  1308,  1312,
    1316,  1321,  1326,  1330,  1334,  1338,  1342,  1347,  1352,  1355,
    1358,  1361,  1365,  1368,  1371,  1374,  1377,  1381,  1385,  1388,
    1392,  1396,  1400,  1405,  1409,  1415,  1422,  1429,  1434,  1437,
    1443,  1446,  1449,  1452,  1455,  1459,  1461,  1463,  1466,  1469,
    1472,  1474,  1477,  1479,  1484,  1486,  1490,  1494,  1497,  1500,
    1503,  1505,  1508,  1511,  1515,  1519,  1523,  1529,  1535,  1540,
    1545,  1550,  1553,  1557,  1562,  1567,  1570,  1574,  1577,  1580,
    1582,  1595,  1614,  1635,  1642,  1651,  1658,  1665,  1672,  1685,
    1696,  1707,  1714,  1719,  1730,  1737,  1744,  1753,  1755,  1758,
    1761,  1764,  1766,  1769,  1772,  1776,  1780,  1785,  1789,  1792,
    1796,  1801,  1805,  1810,  1813,  1818,  1823,  1830,  1833,  1836,
    1838,  1842,  1844,  1846,  1848,  1851,  1854,  1857,  1860,  1863,
    1866,  1870,  1874,  1877,  1880,  1883,  1886,  1888,  1890,  1893,
    1897,  1901,  1905,  1909,  1913,  1917,  1922,  1927,  1932,  1937,
    1942,  1947,  1952,  1956,  1960,  1964,  1968,  1973,  1978,  1982,
    1986,  1990,  1995,  1999,  2003,  2008,  2014,  2018,  2022,  2027,
    2032,  2037,  2044,  2049,  2054,  2058,  2062,  2066,  2070,  2075,
    2079,  2083,  2088,  2093,  2099,  2106,  2110,  2114,  2116,  2120,
    2123,  2126,  2129,  2132,  2135,  2138,  2141,  2146,  2148,  2151,
    2155,  2159,  2162,  2165,  2168,  2170,  2174,  2178,  2180,  2183,
    2186,  2188,  2191,  2194,  2197,  2200,  2204,  2208,  2210,  2214,
    2217,  2222,  2227,  2229,  2232,  2235,  2238,  2241,  2244,  2247,
    2250,  2253,  2255,  2258,  2261,  2264,  2268,  2272,  2276,  2280,
    2284,  2286,  2288,  2292,  2295,  2298,  2303,  2305,  2308,  2311,
    2314,  2317,  2322,  2325,  2329,  2331,  2333,  2335,  2337,  2339,
    2341,  2343,  2346,  2349,  2353,  2356,  2358,  2361,  2365,  2370,
    2372,  2375,  2378,  2382,  2384,  2386,  2388,  2390,  2392,  2394,
    2396,  2398,  2400,  2402,  2404,  2406,  2408,  2410,  2412,  2414,
    2416,  2418,  2420,  2422,  2424,  2426,  2428,  2430,  2432,  2434,
    2436,  2438,  2440,  2442,  2444,  2446,  2448,  2450,  2452,  2454,
    2456,  2458,  2460,  2462,  2464,  2466,  2468,  2470,  2472,  2474,
    2476,  2478,  2480,  2482,  2484,  2486,  2488,  2490,  2492,  2494,
    2496,  2498,  2500,  2502,  2504,  2506,  2508,  2510,  2512,  2514,
    2516,  2518,  2520,  2522,  2524,  2526,  2528,  2530,  2532,  2534,
    2536,  2538,  2540,  2542,  2544,  2546,  2548,  2550,  2552,  2554,
    2556,  2558,  2560,  2562,  2564,  2566,  2568,  2570,  2572,  2574,
    2576,  2578,  2580,  2582,  2584,  2586,  2588,  2590,  2592,  2594,
    2596,  2598,  2600,  2602,  2604,  2606,  2608,  2610,  2612,  2614,
    2616,  2618,  2620,  2622,  2624,  2626,  2628,  2630,  2632,  2634,
    2636,  2638,  2640,  2642,  2644,  2646,  2648,  2650,  2652,  2654,
    2656,  2658,  2660,  2662,  2664,  2666,  2668,  2670,  2672,  2674,
    2676,  2678,  2680,  2682,  2684,  2686,  2688,  2690,  2692,  2694,
    2696,  2698,  2700,  2702,  2704,  2706,  2708,  2710,  2712,  2715,
    2718,  2721,  2724,  2727,  2730,  2739,  2742,  2745,  2747,  2749,
    2751,  2755,  2760,  2763,  2767,  2772,  2790,  2794,  2798,  2801,
    2804,  2807,  2810,  2814,  2818,  2822,  2827,  2832,  2838,  2841,
    2844,  2848,  2852,  2856,  2860,  2864,  2868,  2872,  2876,  2880,
    2884,  2888,  2892,  2896,  2900,  2904,  2908,  2912,  2916,  2921,
    2926,  2931,  2935,  2940,  2945,  2948,  2950,  2952,  2957,  2961,
    2965,  2970,  2975,  2978,  2981,  2984,  2989,  2995,  3000,  3005,
    3008,  3011,  3014,  3017,  3020,  3023,  3026,  3029,  3032,  3035,
    3039,  3041,  3043,  3046,  3049,  3052,  3055,  3058,  3062,  3066,
    3068,  3071,  3073,  3075,  3077,  3080,  3082,  3084,  3086,  3088
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     401,     0,    -1,   402,    -1,   404,    -1,   412,    -1,   403,
      -1,   402,   393,   403,    -1,    -1,   418,    -1,   469,    -1,
     417,    -1,   431,    -1,   421,    -1,   419,    -1,   420,    -1,
     413,    -1,   415,    -1,   416,    -1,     1,    -1,   372,    -1,
      19,    -1,   369,    -1,   370,    -1,   371,    -1,   411,   408,
      -1,   461,   394,   412,   395,    -1,   154,   394,   412,   396,
     412,   395,    -1,   411,   392,   167,    -1,   430,   392,   167,
      -1,   430,   392,   147,    -1,   429,   392,   147,    -1,    21,
      -1,   404,    22,    -1,   242,    -1,    23,   394,   406,   395,
      -1,    24,   394,   404,   395,    -1,    26,   394,   406,   396,
     404,   395,    -1,    25,   394,   406,   396,   406,   395,    -1,
      27,   394,   404,   396,   404,   395,    -1,    28,   394,   406,
     396,   406,   396,   404,   395,    -1,    29,   394,   404,   396,
     404,   396,   404,   395,    -1,    30,   394,   404,   396,   404,
     396,   404,   396,   404,   395,    -1,    31,   394,   404,   396,
     404,   396,   404,   396,   404,   396,   404,   395,    -1,    32,
     394,   404,   396,   404,   396,   404,   396,   404,   396,   404,
     396,   404,   395,    -1,   429,   392,   314,    -1,   429,   392,
     315,    -1,   429,   392,   317,    -1,   429,   392,   318,    -1,
     429,   392,   325,    -1,   429,   392,   326,    -1,   429,   392,
     327,    -1,   429,   392,   328,    -1,    18,   394,   409,   395,
      -1,    18,   394,   406,   396,   407,   396,   407,   395,    -1,
      18,   394,   406,   396,   407,   396,   407,   396,   407,   396,
     407,   396,   404,   395,    -1,   314,    -1,   315,    -1,   317,
      -1,   318,    -1,   325,    -1,   326,    -1,   327,    -1,   328,
      -1,   316,    -1,   319,    -1,   394,   404,   395,    -1,   404,
     384,   404,    -1,   404,   385,   404,    -1,   385,   404,    -1,
     384,   404,    -1,   404,   386,   404,    -1,   404,   387,   404,
      -1,   404,   388,   404,    -1,   404,   391,   404,    -1,   404,
     374,   404,   375,   404,    -1,   404,   383,   404,    -1,   404,
     382,   404,    -1,   404,   381,   404,    -1,   404,   380,   404,
      -1,   404,   379,   404,    -1,   404,   378,   404,    -1,   404,
     377,   404,    -1,   404,   376,   404,    -1,   389,   404,    -1,
      69,    -1,   405,   392,   405,    -1,   405,   392,   404,    -1,
     404,    -1,   406,    -1,   397,   406,   398,    -1,   404,    -1,
     405,    -1,   404,    -1,   405,    -1,    20,    -1,   452,    -1,
     430,   392,   452,    -1,   411,    -1,   411,   397,   406,   375,
     406,   398,    -1,   187,   394,   407,   395,    -1,   187,   394,
     404,   396,   404,   396,   407,   395,    -1,   242,   394,   407,
     395,    -1,   248,   394,   430,   395,    -1,   257,   394,   412,
     395,    -1,    23,   394,   412,   395,    -1,    24,   394,   412,
     395,    -1,    27,   394,   412,   396,   412,   395,    -1,    27,
     394,   404,   396,   412,   395,    -1,    27,   394,   412,   396,
     404,   395,    -1,    26,   394,   406,   396,   412,   395,    -1,
      28,   394,   406,   396,   406,   396,   412,   395,    -1,    29,
     394,   404,   396,   404,   396,   412,   395,    -1,    30,   394,
     404,   396,   404,   396,   404,   396,   412,   395,    -1,    31,
     394,   404,   396,   404,   396,   404,   396,   404,   396,   412,
     395,    -1,    32,   394,   404,   396,   404,   396,   404,   396,
     404,   396,   404,   396,   412,   395,    -1,   412,   384,   412,
      -1,   412,   384,   404,    -1,   404,   384,   412,    -1,   412,
     385,   412,    -1,   412,   385,   404,    -1,   404,   385,   412,
      -1,   412,   386,   412,    -1,   412,   386,   404,    -1,   404,
     386,   412,    -1,   412,   387,   412,    -1,   412,   387,   404,
      -1,   404,   387,   412,    -1,   412,   388,   412,    -1,   412,
     388,   404,    -1,   404,   388,   412,    -1,   412,   391,   412,
      -1,   412,   391,   404,    -1,   404,   391,   412,    -1,   412,
      22,    -1,   412,   374,   404,   375,   404,    -1,   412,   374,
     404,   375,   412,    -1,   412,   374,   412,   375,   404,    -1,
     412,   374,   412,   375,   412,    -1,   412,   376,   412,    -1,
     412,   376,   404,    -1,   404,   376,   412,    -1,   412,   377,
     412,    -1,   412,   377,   404,    -1,   404,   377,   412,    -1,
     412,   383,   412,    -1,   412,   383,   404,    -1,   404,   383,
     412,    -1,   412,   382,   412,    -1,   412,   382,   404,    -1,
     404,   382,   412,    -1,   412,   380,   412,    -1,   412,   380,
     404,    -1,   404,   380,   412,    -1,   412,   381,   412,    -1,
     412,   381,   404,    -1,   404,   381,   412,    -1,   412,   379,
     412,    -1,   412,   379,   404,    -1,   404,   379,   412,    -1,
     412,   378,   412,    -1,   412,   378,   404,    -1,   404,   378,
     412,    -1,   389,   412,    -1,   394,   412,   395,    -1,   385,
     412,    -1,    19,   399,   404,    -1,   369,   399,   404,    -1,
     370,   399,   404,    -1,   371,   399,   404,    -1,   411,   408,
     399,   404,    -1,   411,    -1,   414,   399,   412,    -1,   414,
     399,   404,    -1,    90,   373,    -1,    90,   373,   397,   398,
      -1,    90,   373,   397,   407,   398,    -1,    90,    19,    -1,
      90,    20,    -1,    70,    19,    -1,    70,    20,    -1,    35,
     405,   405,    -1,    35,   123,   443,    -1,   307,   405,   304,
     433,   129,   405,    -1,   307,   405,   304,   433,   129,   405,
      35,   405,    -1,   248,   443,    -1,   248,   304,   441,    -1,
     248,   465,    -1,   248,   463,    -1,   248,   466,    -1,   248,
     169,   404,   396,   404,   396,   404,   396,   404,    -1,   248,
     337,   404,   396,   404,    -1,   173,   248,   301,   429,    -1,
     308,   407,    -1,   218,   250,   407,   396,   407,    -1,   218,
     273,   407,   396,   407,    -1,    99,   405,   218,   273,   407,
     396,   407,    -1,    99,   405,   104,   404,    -1,    99,   405,
     122,    40,   443,    -1,    99,   405,   122,   443,    -1,    99,
     405,   215,   405,    -1,   137,    99,   405,    -1,   247,    18,
     410,    -1,    18,   323,   443,    -1,    18,   323,   362,   406,
      -1,    51,   465,    -1,   218,    51,   115,   443,    -1,   218,
     267,   404,   388,    -1,   218,   153,   404,   388,    -1,   173,
     218,   443,    -1,   282,   322,   404,   396,   404,   396,   404,
     396,   404,    -1,   298,   407,    -1,   294,   430,    -1,   321,
     429,    -1,   321,   430,    -1,   430,   173,   445,   405,    -1,
     430,   173,   443,    -1,   321,    64,    -1,   321,    64,   407,
      -1,    64,   443,    -1,    64,   429,    -1,    64,   404,   396,
     404,   396,   404,   396,   404,    -1,    64,   175,   451,    -1,
      64,   463,    -1,    64,   466,    -1,    64,   465,    -1,    64,
     115,   465,    -1,    64,   115,   464,    -1,    64,    88,    -1,
     321,   108,    -1,   321,   108,   407,    -1,   108,   443,    -1,
     108,   429,    -1,   108,   404,   396,   404,   396,   404,   396,
     404,    -1,   108,   175,   451,    -1,   108,   463,    -1,   108,
     466,    -1,   108,   465,    -1,   108,   115,   465,    -1,   108,
     115,   464,    -1,   108,    88,    -1,   321,   169,    -1,   321,
     169,   407,    -1,   169,   443,    -1,   169,   429,    -1,   169,
     404,   396,   404,   396,   404,   396,   404,    -1,   169,   175,
     451,    -1,   169,   466,    -1,   169,   463,    -1,   169,   465,
      -1,   169,    43,   407,    -1,   169,    43,   167,   404,    -1,
     169,    43,   304,   407,    -1,   169,    43,   164,   404,   396,
     404,    -1,   169,    88,    -1,   321,   289,    -1,   321,   289,
     407,    -1,   289,   443,    -1,   289,   429,    -1,   289,   404,
     396,   404,    -1,   289,   175,   451,    -1,   289,   465,    -1,
     289,   255,   407,    -1,   289,   462,    -1,   289,   160,   407,
      -1,   289,    67,   273,   404,    -1,   289,    88,   405,    -1,
     299,   443,    -1,   299,   462,    -1,   299,    67,   273,   404,
      -1,   299,   255,   407,    -1,   299,   465,    -1,   299,   404,
     396,   404,    -1,   299,    88,   405,    -1,    89,   463,    -1,
      89,   466,    -1,    89,   465,    -1,    89,   464,    -1,    89,
      67,   273,   404,    -1,    89,   462,    -1,    89,   293,   273,
     404,    -1,    89,   271,   405,    -1,   185,   122,   407,   301,
     405,   396,   405,    -1,   185,    75,   407,   301,   394,   407,
     396,   407,   396,   407,   395,   396,   405,    -1,   322,   404,
     396,   404,   396,   404,   396,   404,    -1,   368,   404,    -1,
     313,   404,   396,   404,   396,   404,   396,   404,    -1,   300,
     405,    -1,   300,   462,    -1,   300,   273,   404,    -1,   300,
     465,    -1,   291,   405,    -1,   291,   462,    -1,   291,   273,
     404,    -1,   291,   465,    -1,   332,   265,   442,    -1,   360,
     265,   442,    -1,   332,   158,   443,    -1,   360,   158,   443,
      -1,    47,   214,   204,    -1,    47,   214,   332,    -1,    47,
     214,   360,    -1,    47,   214,   338,    -1,    98,   405,    -1,
      70,    98,    -1,   166,   443,    -1,   166,   175,   451,    -1,
     166,   312,   407,    -1,   166,   140,   407,    -1,   166,   167,
     407,    -1,   166,   158,   443,    -1,   166,    64,   115,   465,
      -1,   166,    64,   115,   464,    -1,   166,    64,   465,    -1,
     166,    64,   464,    -1,   166,    64,   463,    -1,   166,    64,
     466,    -1,   166,   404,   396,   404,    -1,   166,    67,   273,
     404,    -1,   166,   462,    -1,   166,   465,    -1,   126,   443,
      -1,   126,   304,   407,    -1,   126,   463,    -1,   126,   466,
      -1,   126,   465,    -1,   126,   464,    -1,   126,    51,   465,
      -1,   126,    51,   464,    -1,   429,   443,    -1,   429,   141,
     443,    -1,   429,   304,   438,    -1,   429,   283,   443,    -1,
     429,    52,   140,   404,    -1,   429,   118,   443,    -1,   429,
     118,   124,   448,   448,    -1,   429,   118,   236,   404,   396,
     404,    -1,   429,   118,   337,   404,   396,   404,    -1,   429,
     118,   304,   407,    -1,   304,   437,    -1,    90,   435,   405,
     436,   405,    -1,    70,   435,    -1,   278,   445,    -1,   124,
     448,    -1,   116,   428,    -1,   369,    79,   443,    -1,   246,
      -1,   306,    -1,    65,   405,    -1,   107,   405,    -1,   107,
     404,    -1,    73,    -1,    73,   405,    -1,   111,    -1,   111,
     394,   406,   395,    -1,   238,    -1,   238,   301,    99,    -1,
     238,   301,   405,    -1,   218,   450,    -1,   275,   404,    -1,
     139,   405,    -1,   139,    -1,   131,   405,    -1,   241,   405,
      -1,   430,   141,   443,    -1,   430,   167,   407,    -1,    20,
     167,   407,    -1,   430,   229,   404,   396,   404,    -1,   430,
     105,   407,   396,   407,    -1,   277,   430,   454,   453,    -1,
      80,   430,   301,   430,    -1,    41,   430,   301,   430,    -1,
     252,   430,    -1,   281,   430,   407,    -1,   201,   430,   301,
     430,    -1,   292,   430,   377,   430,    -1,   161,   430,    -1,
     161,   430,   264,    -1,   161,   429,    -1,   161,   248,    -1,
     119,    -1,    42,   394,   407,   396,   407,   396,   404,   396,
     404,   396,   404,   395,    -1,    42,   394,   407,   396,   407,
     396,   404,   396,   404,   396,   404,   396,   443,   396,   443,
     396,   443,   395,    -1,    42,   394,   407,   396,   407,   396,
     404,   396,   404,   396,   404,   396,   443,   396,   443,   396,
     443,   396,   443,   395,    -1,   205,   394,   430,   396,   407,
     395,    -1,   205,   394,   430,   396,   412,   396,   407,   395,
      -1,   249,   394,   430,   396,   407,   395,    -1,   444,   394,
     430,   396,   407,   395,    -1,   455,   394,   430,   396,   407,
     395,    -1,   455,   394,   430,   396,   456,   396,   459,   396,
     457,   396,   458,   395,    -1,   156,   394,   430,   396,   412,
     396,   460,   396,   443,   395,    -1,   142,   394,   430,   396,
     412,   396,   443,   396,   443,   395,    -1,   101,   394,   430,
     396,   407,   395,    -1,   155,   394,   430,   395,    -1,   334,
     394,   430,   396,   430,   396,   407,   396,   443,   395,    -1,
     168,   394,   430,   396,   430,   395,    -1,   251,   394,   430,
     396,   412,   395,    -1,   251,   394,   430,   396,   248,   396,
     443,   395,    -1,    47,    -1,    47,   332,    -1,    47,   360,
      -1,    47,   430,    -1,    48,    -1,   120,   429,    -1,   243,
     405,    -1,   243,    56,   405,    -1,   243,    61,   405,    -1,
     243,    61,   445,   405,    -1,    61,   437,   405,    -1,   161,
      61,    -1,   243,   437,   405,    -1,   243,   437,   445,   405,
      -1,   243,   207,   405,    -1,   243,   207,   445,   405,    -1,
     324,   430,    -1,   324,   430,   124,   405,    -1,   324,   430,
     114,   405,    -1,   324,   430,   114,   405,   124,   405,    -1,
     264,   405,    -1,   174,   405,    -1,   203,    -1,   203,   129,
     405,    -1,   240,    -1,   233,    -1,    81,    -1,   282,   407,
      -1,    70,   282,    -1,    70,    64,    -1,    70,   108,    -1,
      70,   169,    -1,    70,   289,    -1,   218,   164,   439,    -1,
      46,   246,   443,    -1,   120,   443,    -1,   120,   269,    -1,
     120,   121,    -1,   120,    71,    -1,   422,    -1,   471,    -1,
     430,   443,    -1,   430,   304,   437,    -1,   430,   293,   407,
      -1,   430,   293,   465,    -1,   430,   293,   464,    -1,   430,
     293,   466,    -1,   430,   293,   463,    -1,   430,   293,   115,
     465,    -1,   430,   293,   115,   464,    -1,   430,   293,   273,
     404,    -1,   430,   293,    67,   407,    -1,   430,   293,    67,
     462,    -1,   430,   293,   274,   407,    -1,   430,   169,   304,
     407,    -1,   430,   169,   463,    -1,   430,   169,   466,    -1,
     430,   169,   465,    -1,   430,   169,   464,    -1,   430,   115,
     304,   407,    -1,   430,   115,   258,   407,    -1,   430,   115,
     465,    -1,   430,   115,   464,    -1,   430,    55,   443,    -1,
     430,    55,   304,   407,    -1,   430,   106,   443,    -1,   430,
      49,   443,    -1,   430,    49,   304,   407,    -1,   430,    49,
      67,   273,   404,    -1,   430,    49,   462,    -1,   430,    49,
     465,    -1,   430,    49,   255,   407,    -1,   430,    49,   124,
     448,    -1,   430,    49,   236,   407,    -1,   430,    49,   209,
     404,   396,   404,    -1,   430,    49,   237,   405,    -1,   430,
      49,    41,   405,    -1,   430,   110,   443,    -1,   430,   110,
     467,    -1,   430,   110,   465,    -1,   430,   110,   464,    -1,
     430,   110,   273,   404,    -1,   430,   110,   466,    -1,   430,
     110,   463,    -1,   430,   110,   254,   466,    -1,   430,   110,
     254,   463,    -1,   430,   110,   254,    72,   443,    -1,   430,
     110,   254,    72,   167,   404,    -1,   430,    76,   405,    -1,
     430,   166,   405,    -1,   443,    -1,   304,   367,   443,    -1,
     296,   424,    -1,   296,   472,    -1,   295,   425,    -1,   295,
     473,    -1,   162,   426,    -1,   162,   470,    -1,    52,   427,
      -1,   209,   404,   396,   404,    -1,   443,    -1,   184,   404,
      -1,   189,   297,   407,    -1,   228,   256,   443,    -1,   210,
     404,    -1,   211,   404,    -1,    89,   407,    -1,   447,    -1,
     184,   273,   404,    -1,   189,   273,   404,    -1,   465,    -1,
     184,   465,    -1,   189,   465,    -1,   466,    -1,   184,   466,
      -1,   189,   466,    -1,   184,   463,    -1,   189,   463,    -1,
     184,   134,   443,    -1,   189,   134,   443,    -1,   467,    -1,
     279,   304,   434,    -1,   279,   407,    -1,   184,   407,   396,
     404,    -1,   189,   407,   396,   404,    -1,   443,    -1,   236,
     407,    -1,   124,   448,    -1,   124,   404,    -1,    41,   405,
      -1,   237,   405,    -1,    39,   407,    -1,   274,   407,    -1,
     286,   407,    -1,   467,    -1,   125,   405,    -1,   287,   404,
      -1,   288,   404,    -1,   287,   304,   279,    -1,   287,   304,
      46,    -1,   288,   304,   279,    -1,   288,   304,    46,    -1,
      67,   273,   404,    -1,   462,    -1,   465,    -1,   407,   396,
     405,    -1,   209,    46,    -1,   209,   279,    -1,   209,   404,
     396,   404,    -1,   405,    -1,   164,   224,    -1,   164,   219,
      -1,   228,    46,    -1,   228,   279,    -1,   228,   404,   396,
     404,    -1,   160,   446,    -1,    67,   273,   404,    -1,   462,
      -1,   465,    -1,   467,    -1,   443,    -1,   465,    -1,   463,
      -1,   466,    -1,   300,   405,    -1,   125,   405,    -1,   321,
     407,   220,    -1,   236,   404,    -1,   133,    -1,   132,   408,
      -1,   429,   392,   270,    -1,   429,   392,   269,   408,    -1,
     270,    -1,   269,   408,    -1,   432,   423,    -1,   429,   432,
     423,    -1,   333,    -1,   361,    -1,    37,    -1,    38,    -1,
       5,    -1,     6,    -1,     7,    -1,     8,    -1,    10,    -1,
       9,    -1,    11,    -1,    12,    -1,    13,    -1,    14,    -1,
      15,    -1,    16,    -1,   204,    -1,   297,    -1,    62,    -1,
     148,    -1,   212,    -1,   182,    -1,   222,    -1,   337,    -1,
      52,    -1,    53,    -1,    54,    -1,   353,    -1,   342,    -1,
     346,    -1,   343,    -1,   347,    -1,   345,    -1,   344,    -1,
     348,    -1,   349,    -1,   350,    -1,   340,    -1,   341,    -1,
     352,    -1,   339,    -1,   351,    -1,   337,    -1,    68,    -1,
     230,    -1,   276,    -1,   117,    -1,   226,    -1,   127,    -1,
     117,    -1,   163,    -1,   234,    -1,    33,    -1,    58,    -1,
     165,    -1,   253,    -1,   231,    -1,   232,    -1,   144,    -1,
     309,    -1,   146,    -1,   311,    -1,   206,    -1,   177,    -1,
     245,    -1,   178,    -1,   213,    -1,   208,    -1,   259,    -1,
     263,    -1,   261,    -1,   260,    -1,   262,    -1,   102,    -1,
     227,    -1,   253,    -1,   165,    -1,    66,    -1,   151,    -1,
     217,    -1,    62,    -1,    87,    -1,   112,    -1,   130,    -1,
     266,    -1,   109,    -1,    78,    -1,   235,    -1,    86,    -1,
     192,    -1,   365,    -1,   196,    -1,   190,    -1,   197,    -1,
      82,    -1,   199,    -1,   200,    -1,   198,    -1,    84,    -1,
      83,    -1,    85,    -1,   143,    -1,   191,    -1,   193,    -1,
     366,    -1,    92,    -1,    94,    -1,    96,    -1,   195,    -1,
      91,    -1,    93,    -1,    95,    -1,   194,    -1,   206,    -1,
      34,    -1,   202,    -1,   305,    -1,   103,    -1,   253,    -1,
     165,    -1,   151,    -1,   217,    -1,   322,    -1,   313,    -1,
     329,    -1,   354,    -1,   330,    -1,   355,    -1,   356,    -1,
     357,    -1,   358,    -1,   359,    -1,    44,    -1,    97,    -1,
     329,    -1,   354,    -1,   100,    -1,   113,    -1,   157,    -1,
     159,    -1,   244,    -1,    77,    -1,    17,    -1,   128,    -1,
     223,    -1,   183,    -1,   225,    -1,    74,    -1,   204,    -1,
     303,    -1,   136,    -1,   320,    -1,   135,    -1,    60,    -1,
     221,    -1,   170,    -1,   280,    -1,    45,    -1,   188,    -1,
     186,    -1,    50,    -1,   268,    -1,   290,    -1,   150,    -1,
     149,    -1,   122,   407,    -1,   122,   405,    -1,   171,   407,
      -1,   222,   407,    -1,    75,   407,    -1,    75,   405,    -1,
      75,   394,   407,   396,   407,   396,   407,   395,    -1,   172,
     404,    -1,   228,   468,    -1,   206,    -1,   216,    -1,    62,
      -1,   218,   164,   440,    -1,   218,   273,   372,   372,    -1,
     218,   407,    -1,   218,   153,   407,    -1,    89,   122,   278,
     404,    -1,   282,   322,   404,   396,   404,   396,   404,   396,
     404,   296,   404,   396,   404,   396,   404,   396,   404,    -1,
      64,   115,   474,    -1,   108,   115,   474,    -1,   289,   466,
      -1,   299,   466,    -1,   300,   466,    -1,   291,   466,    -1,
     166,    64,   443,    -1,   166,   331,   404,    -1,   166,   356,
     404,    -1,   166,   289,   407,   405,    -1,   166,    64,   115,
     443,    -1,   166,    64,   115,   321,   474,    -1,   166,   463,
      -1,   166,   466,    -1,   429,   162,   443,    -1,   429,   304,
     179,    -1,   429,   304,   181,    -1,   429,   304,   180,    -1,
     429,   304,    52,    -1,   429,   304,   138,    -1,   429,   304,
     284,    -1,   429,   304,   285,    -1,   322,   336,   404,    -1,
     322,   335,   404,    -1,   322,   364,   404,    -1,   322,   363,
     404,    -1,   313,   336,   404,    -1,   313,   335,   404,    -1,
     313,   364,   404,    -1,   313,   363,   404,    -1,   166,   164,
     404,    -1,   126,   115,   443,    -1,   429,    47,   304,    46,
      -1,   429,    47,   304,   279,    -1,   169,    43,   273,   404,
      -1,   137,    99,   404,    -1,   239,   172,    57,   404,    -1,
     239,   172,   152,   404,    -1,   239,   466,    -1,   466,    -1,
     475,    -1,   430,   293,   115,   407,    -1,   430,   274,   407,
      -1,   430,   115,   407,    -1,   430,   110,   304,   476,    -1,
     430,   293,    66,   443,    -1,   430,   463,    -1,   430,   466,
      -1,   430,   465,    -1,   430,   115,   321,   474,    -1,   430,
     353,   404,   396,   404,    -1,   430,   110,   167,   404,    -1,
     430,   110,   254,   443,    -1,   184,   443,    -1,   189,   443,
      -1,    36,   443,    -1,   188,   372,    -1,   186,   372,    -1,
     176,   443,    -1,   304,    46,    -1,   304,   279,    -1,   189,
     404,    -1,   273,   404,    -1,   407,   396,   404,    -1,   475,
      -1,   466,    -1,   304,    46,    -1,   304,   279,    -1,   164,
     279,    -1,   164,   145,    -1,   164,   310,    -1,   228,   213,
     297,    -1,   228,    59,   297,    -1,   475,    -1,   272,   449,
      -1,   204,    -1,    75,    -1,   222,    -1,   215,   476,    -1,
     302,    -1,    63,    -1,   165,    -1,   253,    -1,    62,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   623,   623,   624,   628,   635,   636,   639,   641,   642,
     643,   644,   645,   646,   647,   648,   649,   650,   651,   658,
     661,   664,   667,   670,   673,   680,   714,   722,   725,   728,
     731,   734,   738,   742,   746,   750,   754,   758,   762,   766,
     770,   774,   778,   782,   786,   789,   792,   795,   798,   801,
     804,   807,   810,   813,   816,   820,   827,   834,   841,   848,
     855,   862,   869,   876,   881,   886,   889,   892,   895,   898,
     901,   904,   913,   921,   932,   935,   938,   941,   944,   947,
     950,   953,   956,   959,   964,   967,   971,   980,   990,   999,
    1009,  1012,  1027,  1030,  1046,  1050,  1063,  1079,  1083,  1105,
    1123,  1141,  1157,  1186,  1196,  1206,  1216,  1231,  1242,  1253,
    1264,  1275,  1286,  1297,  1308,  1319,  1334,  1345,  1356,  1371,
    1382,  1393,  1408,  1419,  1430,  1449,  1464,  1479,  1499,  1514,
    1530,  1553,  1572,  1591,  1601,  1610,  1623,  1636,  1649,  1664,
    1675,  1686,  1701,  1712,  1723,  1738,  1749,  1760,  1775,  1786,
    1797,  1812,  1823,  1834,  1849,  1860,  1871,  1886,  1897,  1908,
    1923,  1934,  1945,  1955,  1965,  1978,  1982,  1986,  1990,  1994,
    2005,  2032,  2043,  2053,  2070,  2078,  2089,  2093,  2097,  2102,
    2108,  2126,  2129,  2136,  2147,  2150,  2153,  2156,  2159,  2162,
    2169,  2177,  2184,  2194,  2197,  2200,  2215,  2229,  2243,  2257,
    2272,  2276,  2279,  2282,  2285,  2288,  2291,  2294,  2297,  2301,
    2306,  2310,  2314,  2317,  2322,  2326,  2331,  2334,  2341,  2348,
    2355,  2365,  2368,  2371,  2374,  2377,  2380,  2383,  2414,  2417,
    2424,  2431,  2438,  2448,  2451,  2454,  2457,  2460,  2463,  2466,
    2497,  2500,  2507,  2514,  2521,  2531,  2534,  2537,  2540,  2543,
    2546,  2549,  2552,  2556,  2573,  2576,  2583,  2590,  2597,  2605,
    2608,  2611,  2614,  2617,  2620,  2623,  2639,  2642,  2645,  2648,
    2651,  2654,  2658,  2664,  2668,  2672,  2676,  2679,  2683,  2687,
    2690,  2694,  2702,  2715,  2725,  2728,  2738,  2746,  2753,  2760,
    2767,  2775,  2782,  2789,  2797,  2804,  2811,  2818,  2825,  2828,
    2831,  2834,  2838,  2847,  2851,  2858,  2865,  2872,  2879,  2886,
    2893,  2900,  2907,  2914,  2921,  2928,  2935,  2943,  2950,  2957,
    2965,  2972,  2979,  2986,  2993,  3000,  3007,  3015,  3024,  3027,
    3030,  3033,  3037,  3041,  3044,  3048,  3052,  3056,  3060,  3065,
    3072,  3076,  3079,  3082,  3083,  3089,  3092,  3099,  3103,  3107,
    3114,  3117,  3120,  3123,  3126,  3133,  3136,  3141,  3163,  3168,
    3176,  3183,  3188,  3200,  3203,  3206,  3209,  3213,  3218,  3223,
    3226,  3236,  3239,  3242,  3245,  3248,  3251,  3254,  3257,  3260,
    3263,  3266,  3272,  3278,  3285,  3297,  3300,  3303,  3306,  3326,
    3330,  3334,  3337,  3340,  3343,  3346,  3363,  3378,  3383,  3388,
    3393,  3396,  3399,  3407,  3412,  3416,  3420,  3424,  3437,  3440,
    3446,  3453,  3457,  3461,  3468,  3476,  3484,  3493,  3501,  3505,
    3508,  3512,  3515,  3518,  3521,  3525,  3528,  3531,  3534,  3537,
    3544,  3549,  3552,  3555,  3558,  3561,  3568,  3569,  3573,  3576,
    3580,  3583,  3586,  3589,  3592,  3595,  3598,  3601,  3604,  3607,
    3610,  3614,  3618,  3622,  3626,  3630,  3635,  3639,  3643,  3661,
    3681,  3685,  3690,  3695,  3699,  3703,  3707,  3711,  3715,  3719,
    3723,  3727,  3731,  3736,  3742,  3745,  3748,  3751,  3754,  3757,
    3760,  3763,  3766,  3769,  3772,  3776,  3781,  3789,  3796,  3803,
    3804,  3805,  3806,  3807,  3808,  3809,  3810,  3821,  3828,  3835,
    3842,  3850,  3857,  3864,  3871,  3878,  3885,  3892,  3899,  3906,
    3913,  3920,  3927,  3934,  3941,  3948,  3955,  3962,  3969,  3976,
    3983,  3991,  4002,  4009,  4016,  4023,  4030,  4038,  4046,  4053,
    4060,  4067,  4074,  4083,  4090,  4097,  4104,  4111,  4118,  4125,
    4132,  4139,  4146,  4161,  4168,  4175,  4186,  4194,  4201,  4208,
    4215,  4222,  4230,  4237,  4244,  4251,  4258,  4268,  4275,  4282,
    4289,  4299,  4303,  4307,  4310,  4316,  4320,  4327,  4340,  4353,
    4366,  4382,  4383,  4387,  4388,  4389,  4390,  4394,  4395,  4396,
    4397,  4398,  4399,  4400,  4401,  4402,  4403,  4404,  4405,  4409,
    4410,  4411,  4415,  4416,  4420,  4421,  4425,  4426,  4427,  4428,
    4429,  4430,  4431,  4432,  4433,  4434,  4435,  4436,  4437,  4438,
    4439,  4440,  4441,  4442,  4443,  4447,  4448,  4449,  4450,  4451,
    4452,  4456,  4457,  4461,  4462,  4466,  4467,  4468,  4469,  4470,
    4471,  4472,  4473,  4474,  4475,  4478,  4479,  4480,  4481,  4484,
    4485,  4488,  4489,  4490,  4491,  4492,  4496,  4497,  4507,  4508,
    4509,  4512,  4513,  4514,  4517,  4518,  4519,  4520,  4521,  4522,
    4523,  4524,  4525,  4526,  4527,  4528,  4529,  4530,  4531,  4532,
    4533,  4534,  4535,  4536,  4537,  4538,  4539,  4540,  4541,  4542,
    4543,  4544,  4545,  4546,  4547,  4548,  4551,  4552,  4553,  4556,
    4557,  4558,  4559,  4560,  4561,  4564,  4565,  4568,  4569,  4570,
    4571,  4572,  4573,  4574,  4575,  4578,  4579,  4582,  4583,  4586,
    4587,  4588,  4589,  4593,  4594,  4598,  4599,  4600,  4604,  4605,
    4606,  4610,  4611,  4612,  4613,  4614,  4615,  4616,  4620,  4621,
    4622,  4625,  4626,  4627,  4628,  4629,  4630,  4631,  4635,  4639,
    4647,  4660,  4673,  4683,  4693,  4712,  4727,  4733,  4734,  4735,
    4740,  4752,  4755,  4758,  4762,  4765,  4770,  4772,  4774,  4776,
    4778,  4779,  4781,  4790,  4797,  4804,  4812,  4813,  4814,  4815,
    4817,  4819,  4823,  4827,  4833,  4839,  4844,  4849,  4856,  4863,
    4870,  4877,  4885,  4892,  4899,  4906,  4914,  4917,  4925,  4927,
    4930,  4934,  4935,  4936,  4937,  4942,  4943,  4953,  4967,  4971,
    4994,  5002,  5003,  5006,  5009,  5012,  5013,  5014,  5017,  5022,
    5030,  5031,  5032,  5033,  5034,  5035,  5042,  5051,  5063,  5070,
    5078,  5088,  5089,  5098,  5105,  5107,  5114,  5121,  5122,  5123,
    5130,  5152,  5153,  5154,  5157,  5163,  5164,  5165,  5166,  5167
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "KEY_VAR", "KEY_VEC", "KEY_CONST",
  "KEY_UNIT", "KEY_FUNC_I", "KEY_FUNC_D", "KEY_FUNC_NN", "KEY_FUNC_ND",
  "KEY_FUNC_DD", "KEY_FUNC_NND", "KEY_FUNC_PPD", "KEY_FUNC_PPPD",
  "KEY_FUNC_PPPPD", "KEY_FUNC_PPPPPD", "INDEX", "DATE", "VAR_D", "VEC_D",
  "CONSTANT", "UCONSTANT", "FUNC_I", "FUNC_D", "FUNC_NN", "FUNC_ND",
  "FUNC_DD", "FUNC_NND", "FUNC_PPD", "FUNC_PPPD", "FUNC_PPPPD",
  "FUNC_PPPPPD", "ABOVE", "ABSOLUTE", "ALIAS", "ALT", "ALTXAXIS",
  "ALTYAXIS", "ANGLE", "ANTIALIASING", "APPEND", "ARRANGE", "ARROW",
  "ASCENDING", "ASPLINE", "AUTO", "AUTOSCALE", "AUTOTICKS", "AVALUE",
  "AVG", "BACKGROUND", "BAR", "BARDY", "BARDYDY", "BASELINE", "BATCH",
  "BEGIN", "BELOW", "BETWEEN", "BLACKMAN", "BLOCK", "BOTH", "BOTTOM",
  "BOX", "CD", "CENTER", "CHAR", "CHART", "CHRSTR", "CLEAR", "CLICK",
  "CLIP", "CLOSE", "COEFFICIENTS", "COLOR", "COMMENT", "COMPLEX",
  "COMPUTING", "CONSTRAINTS", "COPY", "CYCLE", "DAYMONTH", "DAYOFWEEKL",
  "DAYOFWEEKS", "DAYOFYEAR", "DDMMYY", "DECIMAL", "DEF", "DEFAULT",
  "DEFINE", "DEGREESLAT", "DEGREESLON", "DEGREESMMLAT", "DEGREESMMLON",
  "DEGREESMMSSLAT", "DEGREESMMSSLON", "DESCENDING", "DESCRIPTION",
  "DEVICE", "DFT", "DIFFERENCE", "DISK", "DOWN", "DPI", "DROP", "DROPLINE",
  "ECHO", "ELLIPSE", "ENGINEERING", "ERRORBAR", "EXIT", "EXPONENTIAL",
  "FFT", "FILEP", "FILL", "FIT", "FIXED", "FIXEDPOINT", "FLUSH", "FOCUS",
  "FOLLOWS", "FONTP", "FORCE", "FORMAT", "FORMULA", "FRAMEP", "FREE",
  "FREQUENCY", "FROM", "GENERAL", "GETP", "GRAPH", "GRAPHNO", "GRID",
  "HAMMING", "HANNING", "HARDCOPY", "HBAR", "HELP", "HGAP", "HIDDEN",
  "HISTOGRAM", "HMS", "HORIZI", "HORIZONTAL", "HORIZO", "ID", "IFILTER",
  "IMAX", "IMIN", "IN", "INCREMENT", "INOUT", "INT", "INTEGRATE",
  "INTERPOLATE", "INVDFT", "INVERT", "INVFFT", "JUST", "KILL", "LABEL",
  "LANDSCAPE", "LAYOUT", "LEFT", "LEGEND", "LENGTH", "LINCONV", "LINE",
  "LINEAR", "LINESTYLE", "LINEWIDTH", "LINK", "LOAD", "LOCTYPE", "LOG",
  "LOGARITHMIC", "LOGIT", "LOGX", "LOGXY", "LOGY", "MAGIC", "MAGNITUDE",
  "MAJOR", "MAP", "MAXP", "MESH", "MINP", "MINOR", "MMDD", "MMDDHMS",
  "MMDDYY", "MMDDYYHMS", "MMSSLAT", "MMSSLON", "MMYY", "MONTHDAY",
  "MONTHL", "MONTHS", "MONTHSY", "MOVE", "NEGATE", "NEW", "NONE",
  "NONLFIT", "NORMAL", "NXY", "OFF", "OFFSET", "OFFSETX", "OFFSETY",
  "OFILTER", "ON", "ONREAD", "OP", "OPPOSITE", "OUT", "PAGE", "PARA",
  "PARAMETERS", "PARZEN", "PATTERN", "PERIOD", "PERP", "PHASE", "PIE",
  "PIPE", "PLACE", "POINT", "POLAR", "POLYI", "POLYO", "POP", "PORTRAIT",
  "POWER", "PREC", "PREPEND", "PRINT", "PS", "PUSH", "PUTP", "RAND",
  "READ", "REAL", "RECIPROCAL", "REDRAW", "REFERENCE", "REGNUM", "REGRESS",
  "RESIZE", "RESTRICT", "REVERSE", "RIGHT", "RISER", "ROT", "ROUNDED",
  "RSUM", "RULE", "RUNAVG", "RUNMAX", "RUNMED", "RUNMIN", "RUNSTD",
  "SAVEALL", "SCALE", "SCIENTIFIC", "SCROLL", "SD", "SET", "SETNUM",
  "SFORMAT", "SIGN", "SIZE", "SKIP", "SLEEP", "SMITH", "SORT", "SOURCE",
  "SPEC", "SPLINE", "SPLIT", "STACK", "STACKED", "STACKEDBAR",
  "STACKEDHBAR", "STAGGER", "START", "STOP", "STRING", "SUM", "SUBTITLE",
  "SWAP", "SYMBOL", "TARGET", "TICKLABEL", "TICKP", "TICKSP", "TIMER",
  "TIMESTAMP", "TITLE", "TO", "TOP", "TRIANGULAR", "TYPE", "UP",
  "UPDATEALL", "USE", "VERSION", "VERTI", "VERTICAL", "VERTO", "VGAP",
  "VIEW", "VX1", "VX2", "VXMAX", "VY1", "VY2", "VYMAX", "WELCH", "WITH",
  "WORLD", "WRAP", "WRITE", "WX1", "WX2", "WY1", "WY2", "X_TOK", "X0",
  "X1", "XAXES", "XAXIS", "XCOR", "XMAX", "XMIN", "XY", "XYAXES",
  "XYBOXPLOT", "XYCOLOR", "XYCOLPAT", "XYDX", "XYDXDX", "XYDXDXDYDY",
  "XYDXDY", "XYDY", "XYDYDY", "XYHILO", "XYR", "XYSIZE", "XYSTRING",
  "XYVMAP", "XYZ", "Y_TOK", "Y0", "Y1", "Y2", "Y3", "Y4", "YAXES", "YAXIS",
  "YEAR", "YMAX", "YMIN", "YYMMDD", "YYMMDDHMS", "ZERO", "ZNORM",
  "FITPARM", "FITPMAX", "FITPMIN", "NUMBER", "NEW_TOKEN", "'?'", "':'",
  "OR", "AND", "NE", "EQ", "GE", "LE", "LT", "GT", "'+'", "'-'", "'*'",
  "'/'", "'%'", "NOT", "UMINUS", "'^'", "'.'", "';'", "'('", "')'", "','",
  "'['", "']'", "'='", "$accept", "full_list", "multi_list", "list",
  "expr", "sexpr", "iexpr", "nexpr", "indx", "jdate", "jrawdate", "array",
  "vexpr", "asgn", "lside_array", "vasgn", "defines", "regionset",
  "parmset", "actions", "options", "set_setprop", "setprop", "axisfeature",
  "tickattr", "ticklabelattr", "axislabeldesc", "axisbardesc",
  "nonlfitopts", "selectgraph", "selectset", "setaxis", "axis", "proctype",
  "tickspectype", "filtertype", "filtermethod", "xytype", "graphtype",
  "pagelayout", "pageorient", "regiontype", "scaletype", "onoff",
  "runtype", "sourcetype", "justchoice", "inoutchoice", "formatchoice",
  "signchoice", "direction", "worldview", "datacolumn", "sortdir",
  "sorton", "ffttype", "fourierdata", "fourierloadx", "fourierloady",
  "windowtype", "interpmethod", "stattype", "font_select", "lines_select",
  "pattern_select", "color_select", "linew_select", "opchoice_sel",
  "opchoice", "parmset_obs", "axislabeldesc_obs", "setprop_obs",
  "tickattr_obs", "ticklabelattr_obs", "colpat_obs", "opchoice_sel_obs",
  "opchoice_obs", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,   539,   540,   541,   542,   543,   544,
     545,   546,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   581,   582,   583,   584,
     585,   586,   587,   588,   589,   590,   591,   592,   593,   594,
     595,   596,   597,   598,   599,   600,   601,   602,   603,   604,
     605,   606,   607,   608,   609,   610,   611,   612,   613,   614,
     615,   616,   617,   618,   619,   620,   621,   622,   623,   624,
     625,   626,   627,   628,    63,    58,   629,   630,   631,   632,
     633,   634,   635,   636,    43,    45,    42,    47,    37,   637,
     638,    94,    46,    59,    40,    41,    44,    91,    93,    61
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   400,   401,   401,   401,   402,   402,   403,   403,   403,
     403,   403,   403,   403,   403,   403,   403,   403,   403,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   404,   405,   405,   405,   406,   407,   408,
     409,   409,   410,   410,   411,   411,   411,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   412,   412,   412,   412,   412,
     412,   412,   412,   412,   412,   413,   413,   413,   413,   413,
     414,   415,   415,   416,   416,   416,   416,   416,   416,   416,
     416,   416,   416,   416,   417,   417,   417,   417,   417,   417,
     417,   417,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     420,   420,   420,   420,   420,   420,   421,   421,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   423,   423,   423,
     423,   423,   423,   423,   423,   423,   423,   424,   424,   424,
     424,   424,   424,   424,   424,   424,   424,   424,   424,   424,
     424,   424,   424,   424,   424,   424,   424,   424,   424,   424,
     424,   424,   425,   425,   425,   425,   425,   425,   425,   425,
     425,   425,   425,   425,   425,   425,   425,   425,   425,   425,
     425,   425,   425,   425,   425,   425,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   427,   427,   427,
     427,   428,   428,   428,   428,   429,   429,   430,   430,   430,
     430,   431,   431,   432,   432,   432,   432,   433,   433,   433,
     433,   433,   433,   433,   433,   433,   433,   433,   433,   434,
     434,   434,   435,   435,   436,   436,   437,   437,   437,   437,
     437,   437,   437,   437,   437,   437,   437,   437,   437,   437,
     437,   437,   437,   437,   437,   438,   438,   438,   438,   438,
     438,   439,   439,   440,   440,   441,   441,   441,   441,   441,
     441,   441,   441,   441,   441,   442,   442,   442,   442,   443,
     443,   444,   444,   444,   444,   444,   445,   445,   446,   446,
     446,   447,   447,   447,   448,   448,   448,   448,   448,   448,
     448,   448,   448,   448,   448,   448,   448,   448,   448,   448,
     448,   448,   448,   448,   448,   448,   448,   448,   448,   448,
     448,   448,   448,   448,   448,   448,   449,   449,   449,   450,
     450,   450,   450,   450,   450,   451,   451,   452,   452,   452,
     452,   452,   452,   452,   452,   453,   453,   454,   454,   455,
     455,   455,   455,   456,   456,   457,   457,   457,   458,   458,
     458,   459,   459,   459,   459,   459,   459,   459,   460,   460,
     460,   461,   461,   461,   461,   461,   461,   461,   462,   462,
     463,   464,   465,   465,   465,   466,   467,   468,   468,   468,
     469,   469,   469,   469,   469,   469,   469,   469,   469,   469,
     469,   469,   469,   469,   469,   469,   469,   469,   469,   469,
     469,   469,   469,   469,   469,   469,   469,   469,   469,   469,
     469,   469,   469,   469,   469,   469,   469,   469,   469,   469,
     469,   469,   469,   469,   469,   470,   470,   471,   471,   471,
     471,   471,   471,   471,   471,   471,   471,   471,   471,   472,
     472,   472,   472,   472,   472,   472,   472,   472,   472,   472,
     472,   473,   473,   473,   473,   473,   473,   473,   473,   473,
     473,   474,   474,   474,   475,   476,   476,   476,   476,   476
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     3,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     4,     6,     3,     3,     3,
       3,     1,     2,     1,     4,     4,     6,     6,     6,     8,
       8,    10,    12,    14,     3,     3,     3,     3,     3,     3,
       3,     3,     4,     8,    14,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     2,     2,
       3,     3,     3,     3,     5,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     1,     3,     3,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     3,     1,     6,     4,
       8,     4,     4,     4,     4,     4,     6,     6,     6,     6,
       8,     8,    10,    12,    14,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     5,     5,     5,     5,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     3,     2,     3,     3,     3,     3,     4,
       1,     3,     3,     2,     4,     5,     2,     2,     2,     2,
       3,     3,     6,     8,     2,     3,     2,     2,     2,     9,
       5,     4,     2,     5,     5,     7,     4,     5,     4,     4,
       3,     3,     3,     4,     2,     4,     4,     4,     3,     9,
       2,     2,     2,     2,     4,     3,     2,     3,     2,     2,
       8,     3,     2,     2,     2,     3,     3,     2,     2,     3,
       2,     2,     8,     3,     2,     2,     2,     3,     3,     2,
       2,     3,     2,     2,     8,     3,     2,     2,     2,     3,
       4,     4,     6,     2,     2,     3,     2,     2,     4,     3,
       2,     3,     2,     3,     4,     3,     2,     2,     4,     3,
       2,     4,     3,     2,     2,     2,     2,     4,     2,     4,
       3,     7,    13,     8,     2,     8,     2,     2,     3,     2,
       2,     2,     3,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     3,     3,     3,     3,     3,
       4,     4,     3,     3,     3,     3,     4,     4,     2,     2,
       2,     3,     2,     2,     2,     2,     3,     3,     2,     3,
       3,     3,     4,     3,     5,     6,     6,     4,     2,     5,
       2,     2,     2,     2,     3,     1,     1,     2,     2,     2,
       1,     2,     1,     4,     1,     3,     3,     2,     2,     2,
       1,     2,     2,     3,     3,     3,     5,     5,     4,     4,
       4,     2,     3,     4,     4,     2,     3,     2,     2,     1,
      12,    18,    20,     6,     8,     6,     6,     6,    12,    10,
      10,     6,     4,    10,     6,     6,     8,     1,     2,     2,
       2,     1,     2,     2,     3,     3,     4,     3,     2,     3,
       4,     3,     4,     2,     4,     4,     6,     2,     2,     1,
       3,     1,     1,     1,     2,     2,     2,     2,     2,     2,
       3,     3,     2,     2,     2,     2,     1,     1,     2,     3,
       3,     3,     3,     3,     3,     4,     4,     4,     4,     4,
       4,     4,     3,     3,     3,     3,     4,     4,     3,     3,
       3,     4,     3,     3,     4,     5,     3,     3,     4,     4,
       4,     6,     4,     4,     3,     3,     3,     3,     4,     3,
       3,     4,     4,     5,     6,     3,     3,     1,     3,     2,
       2,     2,     2,     2,     2,     2,     4,     1,     2,     3,
       3,     2,     2,     2,     1,     3,     3,     1,     2,     2,
       1,     2,     2,     2,     2,     3,     3,     1,     3,     2,
       4,     4,     1,     2,     2,     2,     2,     2,     2,     2,
       2,     1,     2,     2,     2,     3,     3,     3,     3,     3,
       1,     1,     3,     2,     2,     4,     1,     2,     2,     2,
       2,     4,     2,     3,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     3,     2,     1,     2,     3,     4,     1,
       2,     2,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     2,     2,     8,     2,     2,     1,     1,     1,
       3,     4,     2,     3,     4,    17,     3,     3,     2,     2,
       2,     2,     3,     3,     3,     4,     4,     5,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     4,     4,
       4,     3,     4,     4,     2,     1,     1,     4,     3,     3,
       4,     4,     2,     2,     2,     4,     5,     4,     4,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     3,
       1,     1,     2,     2,     2,     2,     2,     3,     3,     1,
       2,     1,     1,     1,     2,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,    18,     0,    20,    94,    31,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   575,   576,     0,
       0,     0,   397,   401,   733,     0,     0,     0,     0,     0,
     350,     0,   423,     0,     0,     0,     0,   709,     0,     0,
       0,   352,   710,     0,   379,     0,     0,     0,     0,     0,
     565,     0,   360,     0,   737,   736,     0,     0,     0,   711,
     712,     0,     0,     0,     0,     0,     0,     0,   732,     0,
     731,     0,   419,     0,     0,   422,   354,     0,   421,     0,
      33,     0,   345,     0,     0,     0,     0,     0,     0,   641,
     644,   643,   645,   642,     0,   734,     0,   569,     0,     0,
       0,     0,     0,     0,   735,     0,     0,     0,     0,     0,
       0,     0,   346,     0,     0,     0,    55,    56,    63,    57,
      58,    64,     0,     0,     0,    59,    60,    61,    62,   697,
     699,     0,   573,     0,   698,   700,   701,   702,   703,   704,
       0,   574,     0,    21,    22,    23,    19,     0,     0,     0,
       0,     0,     2,     5,     3,    97,     4,    15,     0,    16,
      17,    10,     8,    13,    14,    12,   436,     0,     0,    11,
       0,     0,    95,     0,     0,     9,   437,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    84,     0,     0,     0,     0,     0,     0,     0,   398,
     399,   400,     0,   204,   597,   598,   599,   596,   613,   610,
     611,   601,   603,   606,   605,   602,   604,   607,   608,   609,
     614,   612,   600,     0,     0,    20,    94,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   227,     0,     0,     0,
       0,   640,   639,    33,    21,    22,    23,     0,     0,     0,
       0,     0,   219,     0,   218,   222,   224,   223,   347,   178,
     179,   426,   303,   427,   592,   428,   593,   425,   429,   340,
     351,     0,     0,     0,     0,     0,     0,   278,   273,   276,
     275,   274,   176,   177,   173,     0,   302,     0,     0,   349,
     348,     0,   239,     0,     0,     0,   231,   230,   234,   236,
     235,     0,     0,     0,     0,     0,   343,   435,   434,   433,
     402,   432,   659,   667,   672,   671,   673,   661,   654,   682,
     678,   683,   679,   684,   680,   658,   655,   656,   674,   665,
     675,   662,   676,   685,   681,   664,   666,   670,   668,   669,
     660,   657,   663,   677,   342,     0,     0,     0,   320,   322,
     325,   324,   323,   361,     0,   566,     0,   359,     0,     0,
       0,     0,   408,   378,   377,   375,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   701,     0,   304,
     318,   768,   319,   769,     0,     0,   253,     0,     0,   243,
     242,   247,   248,   246,     0,     0,   418,     0,     0,     0,
       0,     0,     0,     0,   690,   693,     0,     0,   692,   694,
       0,   691,     0,     0,   689,    87,    88,   752,   357,     0,
       0,   794,   362,     0,     0,     0,     0,   403,     0,     0,
       0,     0,     0,     0,   184,   187,   186,   188,     0,     0,
     371,     0,   417,   570,   358,     0,   646,   647,   341,     0,
       0,   424,     0,     0,     0,     0,     0,     0,   257,   256,
     262,   260,   758,     0,   290,   291,   293,   761,     0,   211,
     210,     0,     0,     0,     0,   266,   267,   270,   759,     0,
     286,   287,   289,   760,   338,     0,   192,     0,     0,     0,
       0,     0,   216,   228,   240,   254,   212,   213,     0,     0,
       0,     0,     0,   413,     0,     0,     0,     0,     0,   284,
       0,     0,     0,     0,    69,     0,    68,    97,   164,    83,
     162,     0,     0,     1,     0,    32,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    24,   133,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     328,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   438,
     802,   804,   803,     0,     0,     0,     0,     0,     0,   571,
     487,     0,     0,     0,     0,   202,    87,    91,     0,     0,
     165,   365,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   181,     0,   180,     0,
       0,     0,   431,   298,   299,   301,   300,     0,   743,   742,
     407,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     832,   831,   833,   226,   225,   756,   740,   745,   696,   695,
     221,    68,    83,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    24,
       0,     0,     0,   739,   738,   741,   280,     0,     0,     0,
       0,     0,     0,     0,     0,   238,   237,   757,   233,     0,
       0,   562,   564,   561,     0,   327,   326,   787,   321,     0,
     791,   200,     0,     0,     0,     0,     0,   376,     0,   762,
     314,   313,   312,   315,     0,   307,   309,   786,   308,   305,
       0,   306,   763,   764,     0,     0,     0,     0,     0,     0,
     249,   245,     0,   208,     0,     0,     0,    87,     0,     0,
     420,     0,     0,    87,   753,   622,   621,   623,   624,   430,
     750,     0,     0,    19,     0,   355,   356,     0,     0,     0,
     404,   405,     0,   411,     0,   409,     0,    92,    93,   201,
       0,   625,   626,   631,   633,   627,   629,   630,   628,   632,
     634,   185,     0,     0,     0,     0,     0,   707,   708,     0,
     372,     0,     0,   265,   263,   259,   261,     0,   292,     0,
       0,   272,   269,     0,   288,     0,   783,   782,   785,   784,
       0,   217,   229,   241,   255,   779,   778,   781,   780,     0,
       0,     0,   296,   636,   638,   635,   637,   294,     0,   297,
     295,   344,   166,   167,   168,    65,   163,     0,     0,     0,
       0,     0,     0,     6,   170,     0,     0,     0,    82,   140,
      81,   143,    80,   161,    79,   158,    78,   152,    77,   155,
      76,   149,    75,   146,    66,   117,    67,   120,    70,   123,
      71,   126,    72,   129,    73,   132,    27,     0,     0,     0,
       0,   139,   138,   142,   141,   160,   159,   157,   156,   151,
     150,   154,   153,   148,   147,   145,   144,   116,   115,   119,
     118,   122,   121,   125,   124,   128,   127,   131,   130,   172,
     171,     0,     0,     0,     0,     0,     0,   333,   329,   770,
     331,   774,   616,   619,   775,   771,   773,   772,   620,   617,
     618,   776,   777,   615,   330,    30,     0,   567,    44,    45,
      46,    47,    48,    49,    50,    51,   572,     0,     0,     0,
       0,     0,     0,     0,     0,   463,   466,   467,     0,   460,
     485,     0,   462,     0,     0,     0,     0,     0,   474,   480,
     477,   476,   479,   475,     0,     0,     0,   799,   459,   458,
     363,   486,   364,     0,   452,   455,   454,   453,   215,     0,
       0,   798,     0,     0,     0,     0,     0,   440,   444,   442,
     441,   443,   439,     0,    29,    28,    96,   495,   557,   559,
     558,   560,     0,     0,     0,     0,     0,   546,   493,   554,
     555,   795,   556,   494,   796,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   491,   522,   540,   541,   821,   531,   492,
     829,     0,   653,     0,   651,     0,     0,   732,   731,     0,
       0,     0,   652,     0,     0,     0,     0,     0,   489,   497,
     504,   507,   510,   517,   490,   820,     0,     0,     0,     0,
     203,     0,    52,    34,   104,    35,   105,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    85,   370,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      82,    81,    80,    79,    78,    77,    76,    75,    66,    67,
      70,    71,    72,    73,     0,   369,   277,   754,   279,   174,
       0,   594,   595,     0,   196,     0,   198,   199,     0,     0,
       0,   353,   563,    89,     0,     0,   392,     0,     0,   766,
     311,   310,   317,   765,   316,     0,     0,   250,   790,   251,
       0,   191,     0,     0,     0,    99,   373,     0,   205,   207,
       0,   206,   751,     0,   792,   793,   101,   406,   412,   410,
       0,     0,   102,     0,     0,   103,   705,   706,   368,     0,
     264,   258,   374,   268,   271,   577,   578,   579,   580,   582,
     581,   583,   584,   585,   586,   587,   588,     0,     0,     0,
     415,   414,     0,     0,     0,     0,     0,   169,     0,     0,
     788,   789,   332,     0,     0,   337,     0,   568,   473,     0,
     469,     0,   470,   472,   468,   464,   461,     0,   807,   749,
     747,   748,   746,     0,   808,   482,   481,   478,   839,   836,
     837,   838,   835,   800,   457,   456,   832,   833,   805,   451,
     214,     0,   801,   448,   449,   797,   446,   445,   447,   450,
       0,     0,   650,   649,   648,   552,   548,   547,   834,   549,
     550,     0,     0,   528,   526,     0,   525,   524,   532,   825,
     824,   826,   543,   544,     0,     0,     0,   523,   527,   687,
     688,   686,   830,   529,   530,     0,   533,     0,   534,   822,
     823,     0,   811,   503,   814,     0,     0,   498,     0,   809,
     513,   508,   511,   813,   812,     0,     0,     0,   817,     0,
     810,   514,   509,   512,   501,   502,     0,   818,     0,   519,
     815,   816,     0,   488,     0,     0,    25,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     175,   339,   197,     0,     0,     0,     0,     0,     0,   767,
       0,     0,     0,     0,     0,     0,     0,     0,   193,   194,
       0,   190,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    74,     0,   134,   135,   136,   137,   334,     0,     0,
     465,     0,   367,     0,   483,   366,   806,   553,     0,   496,
     539,     0,   828,   827,   536,   535,   538,   537,   542,   515,
     505,     0,   516,   506,   499,     0,   500,   591,   589,   590,
     518,   819,     0,   714,   713,     0,     0,     0,    37,    36,
     109,    38,   107,   108,   106,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   391,     0,     0,    26,     0,   394,   252,     0,     0,
       0,     0,   383,     0,     0,   385,     0,   395,     0,   182,
       0,     0,   416,     0,    98,   335,   336,   471,   484,   551,
     545,   520,   521,   386,   387,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   195,     0,     0,   730,   728,   729,     0,     0,
       0,   281,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   726,   725,   723,   721,   727,   722,   724,     0,    53,
       0,    39,   110,    40,   111,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   220,   232,     0,     0,   244,
       0,   100,   384,     0,   396,     0,   183,   285,   283,     0,
       0,     0,     0,     0,     0,     0,     0,   744,     0,     0,
       0,     0,     0,     0,   189,   209,     0,   715,   716,   717,
       0,     0,    41,   112,     0,     0,     0,     0,     0,     0,
     390,   389,     0,     0,   393,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   720,   718,   719,     0,     0,
      42,   113,     0,   380,     0,     0,     0,     0,     0,   388,
       0,     0,     0,     0,     0,   282,     0,    54,    43,   114,
       0,     0,     0,     0,     0,     0,     0,     0,   755,   381,
       0,     0,   382
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,   151,   152,   153,   415,   638,   416,   639,   679,   609,
     779,   251,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   599,  1078,  1053,  1028,  1017,   306,   291,
     253,   169,   170,  1217,  1450,   269,  1143,   223,   944,   759,
     760,   791,   837,   434,   171,   448,  1285,  1080,   344,  1312,
     418,   660,   172,  1198,   799,   173,  1456,  1610,  1638,  1558,
    1538,   174,   277,   435,   279,   436,   437,   983,  1252,   175,
    1033,   176,  1084,  1059,   655,  1034,  1263
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -900
static const yytype_int16 yypact[] =
{
    3356,  -900,  -289,  -319,   -53,  -900,  -274,  -264,  -256,  -248,
    -241,  -234,  -197,  -160,  -124,   -87,    20,  -900,  -900,   315,
     -80,    92,    79,  -900,  -900,   271,  4134,  6059,   289,    97,
     289,   315,  -900,   295,   -13,   289,   289,  -900,   -30,  9653,
    6250,   -19,  -900,   -71,  -900,   493, 11501,   202,   289,   -16,
    -900,   270,   289,   -11,  -900,  -900,    -3,     3,    14,  -900,
    -900,   -39,  5093,    19,  6441,  -144,   289,   -32,  -900,    25,
    -900,   315,   259,    31,  5366,  -900,    93,   254,  -900,   289,
      34,  7219,  -900,   413,   -50,    39,    43,   315,    44,  -900,
    -900,  -900,  -900,  -900,   289,  -900,   -16,  -900, 11038,   315,
     -58,   315,  9885,  5679,  -900,   111,   315,   315, 11038,  6632,
     169,  4134,  -900,   289, 11038,  7694,  -900,  -900,  -900,  -900,
    -900,  -900,   679,  7975,   315,  -900,  -900,  -900,  -900,  -900,
    -900,   -93,  -900,    46,  -900,  -900,  -900,  -900,  -900,  -900,
     -89,  -900, 11038,   -64,    47,    50,  -900, 11038,  8352,  8352,
    8352,   444,    71,  -900,  3199,     4,  4365,  -900,    69,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  7120, 11172,  -900,
      -7,    78,  -900,    80,    82,  -900,  -900,  -166,  9653, 11038,
   11038,  8352,  8352, 11038, 11038,  8352, 11038, 11038, 11038, 11038,
   11038,  -900,    60,   -60,    86,   180, 11038,    60,   -48,  -900,
    -900,  -900, 10030,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,   289,    88,  -900,  -900,    89,    91,   109,
     110,   114,   120,   121,   122,   124,  -900,   194, 11038, 11038,
     -27,  -900,  -900,  -900,  -900,  -900,  -900, 11038, 11038, 11038,
     370,  -291,   149,   151,  -900,  -900,  -900,  -900,   176,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
     176,   275,   304,  8751, 11038,   289,   305,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,   183,   289,   176,   -81,   315,  4967,
     176,   149,  -900,   194,   -27,   458,   149,  -900,  -900,  -900,
    -900, 11038,   289, 11038,   289, 11038,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,    -4,    60, 11038,  -900,  -900,
    -900,  -900,  -900,   176, 11038,  -900,  9653,   176,   315,  8352,
     315,   315,  -900,  -900,    86,   322,   685,   316,  9653, 11038,
      60, 11038, 11038,   -27, 11038, 11038, 11038, 11038,   633,  -900,
    -900,  -900,  -900,  -900,   315,  8120,  -900,   -27,   742,   149,
    -900,  -900,  -900,  -900,    60,   296,   176, 11038, 11038, 11038,
     297,   289,   315,   475,  -900,  -900, 11038,   273,  -900,  -900,
   11038,  -900, 11038, 11183,  -900,  4967,  -900,  -900,  -900,     9,
    8899,  -900,   176, 11038,   289,    75,    75,   176,    75,  9653,
   11038,   134, 11038,   315,  -900,  -900,  -900,  -900,   315,   315,
    -900,  8352,   176,  -900,  4967,  -262,  -900,  -900,  -900, 11038,
   11038,  -900,   328,   289, 11038,   -27, 11038,   831,   149,  -900,
    -900,  -900,  -900, 11038,   176,  -900,  -900,  -900,   225,  -900,
    -900,   334,   289, 11038,   864,  -900,  -900,  -900,  -900, 11038,
     176,  -900,  -900,  -900,  -900,  -285,  -900, 11038, 11038, 11038,
   11038,   920, 11038, 11038, 11038, 11038,    86,  -900, 11038, 11038,
   11038, 11038,  1126,   -41,    60,   148,   315,    60,   148,  4967,
      60, 11038, 11038, 11038,   218,   219,   227,   -82,   230,   227,
     230,  4138,  6168,  -900,  3718,  -900, 11038,  8352,  8352,  8352,
    8352,  8352,  8352,  8352,  8352,  8352,  8352,  8352,  8352,  8352,
    8352,   445, 11038,   229,  -900,  8352,  8352,  8352,  8352,  8352,
    8352,  8352,  8352,  8352,  8352,  8352,  8352,  8352,  8352,  8352,
    8352,   326,   491,   -72,    60,    60,    60,    45,   556,    -7,
    -900,   828,  -129,   289, 11038,    60,  1949,  7284,    60,   289,
   11038,    36,   278, 11038, 11038,  5870,  4134, 11038,   166,  -900,
    -900,  -900,  -900,   402,   730, 11038,  4552,  4865,   265,  -900,
    -900,   315,   315,  8352, 11038,  -900,  6358,   176,   247,   245,
    4967,  -900,  3199,   250,  6550,  7303,  7377,   255,   256,  1327,
    1372,   257,  1398,  1436,  1463,  1519,  -900,  9653,   176,    54,
     315,   260,  -900,  -900,  -900,  -900,  -900, 11038,   176,  -900,
     176, 11038, 11038, 11038, 11038, 11038, 11038, 11038, 11038, 11038,
   10030,  -900, 11038,  -900,  -900,  -900,  -900,  4967,  -900,  -900,
    -900,   218,   218,  7414, 11038, 11038, 11038, 11038, 11038, 11038,
   11038, 11038, 11038, 11038, 11038, 11038, 11038, 11038, 11038,  -900,
     315, 11038, 11038,   176,  -900,  -900,   176, 11038,  4035,  -156,
   11038,     6,   289,   377,   261,  -900,  -900,  -900,  -900, 11038,
     263,   176,  4967,   176,   441,  -900,  -900,  -900,  -900,   264,
    4967,   176,   267,  3199,  1580,   284,   287,  -900,   -54,  -900,
    -900,  -900,  -900,  -900, 11038,  -900,  -900,  4967,  -900,  -900,
     289,  -900,  4967,  4967, 11038,   288, 11038, 11038, 11038, 11038,
    -900,  -900, 11038,  -900,   -33,   379,   384,  1604,   291,   315,
     176,   292,    60,  5492,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,   293,  8082,   320,   299,  -900,   176, 11038, 11038,   298,
     176,   176,   289,   176,   289,   176,   289,  4967,   176,  -900,
    1625,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  1659,   301,   302,   306,  7513,  -900,  -900,    -6,
    -900,  1694, 11038,   176,  -900,  -900,  -900, 11038,  4967,   315,
   11038,   176,  -900, 11038,  4967,   987,  4967,  4967,  4967,  4967,
   11038,  -900,  -900,  -900,  -900,  4967,  4967,  4967,  4967, 11038,
     289,   289,  -900,  -900,  -900,  -900,  -900,  -900,   309,  -900,
    -900,  -900,  4967,  4967,  4967,  -900,  -900,   374,  -319,     2,
     -64,    47,    50,  -900,   -16,  8370, 11362,  3318,    74,   600,
    1068,  1503,  2039,  2066,  2039,  2066,  2039,  2066,  2039,  2066,
    2039,  2066,  2039,  2066,   106,   205,   106,   205,   227,   230,
     227,   230,   227,   230,   227,   230,  -900,  -326, 11038,  4990,
    5052,    74,   600,  1068,  1503,  2039,  2066,  2039,  2066,  2039,
    2066,  2039,  2066,  2039,  2066,  2039,  2066,   106,   205,   106,
     205,   227,   230,   227,   230,   227,   230,   227,   230,  3199,
    4365,   -28, 11038, 11501, 11038, 11038, 11038,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,   -16,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,   289,   427, 11501,
   11038, 11038,   289, 11038, 11038,  -900,  -900,  -900, 11038,  -900,
     176,   311,  -900, 11038,   179,   398, 11038,   -26,  -900,  -900,
    -900,  -900,  -900,  -900, 11038, 11038,   280,  -900,  -900,  -900,
    -900,   176,  -900, 11038,  -900,  -900,  -900,  -900,  -900,   289,
    1814,  -900,    60, 10284,  9128, 11038, 11038,  -900,  -900,  -900,
    -900,  -900,  -900,  1890,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,   435,   -36,   118,   -26,  7598,   176,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  1925, 11038,   289,   437,  4180,
     289,    68,  9276,   359, 11038,   289,    -2, 11038, 11038, 10429,
   10661,   -17,   329,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,    60,  -900, 11038,  -900,    60,  7093,   356,   357,  6865,
   11038, 11038,  -900,    40, 11038, 10806,   -15,   336,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,    60,   337,   339,  7715,
    -900, 11038,  -900,  -900,  -900,  -900,  -900, 11038,  8352,  8352,
    8352, 11038, 11038, 11038, 11038, 11038,  4967,  -900,  -900, 11038,
    7414,   343,  7795,   344,  1998,   363,  2122,  2196,  2233,  2257,
    1018,  2453,  3526,  3526,  3526,  3526,  3526,  3526,   248,   248,
     218,   218,   218,   218,  2305,  -900,  4967,  4967,  4967,  -900,
     365,  -900,  -900,   289,  4967,    60,  -900,   176, 11038, 11038,
    2475,  -900,  -900,  -900,  8352,  8352,  -900,  8352,   280,  -900,
    -900,  -900,  4967,   176,  4967,   315,  2502,  4967,  4967,  -900,
    2540,  -900,   371,   289, 11038,  -900,  -900,  8352,  -900, 11038,
   11038, 11038,  -900, 11038,  4967,  4967,  -900,   176,   176,   176,
   11038, 11038,  -900, 11038,  8497,  -900,  -900,  -900,  -900, 11038,
    4967,  4967,  -900,  4967,  4967,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,   640,  2573,  2600,
    -107,   176,   315,   229,   562, 11038, 11038,  4967,  8352,  8352,
    -900,  -900,  4967, 11501,  2639,  -900,  2676,  -900,   176, 11038,
    -900,  2699,  -900,   176,  -900,  -900,  -900, 11038,  4967,  -900,
    -900,  -900,  -900,   -10,  -900,  -900,  -900,  4967,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
     176, 11038,  -900,  -900,  -900,  -900,  -900,  -900,  4967,  -900,
   11038, 11038,  -900,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  2753, 11038,  -900,   176, 11038,  4967,  -900,   176,  -900,
    -900,  -900,  -900,  -900,  2805,   473,   476,  -900,   176,  -900,
    -900,  -900,  -900,  -900,  -900,    15,  4967,    22,  4967,  -900,
    -900,   289,  -900,  -900,  -900,    60, 11038,  2883,   376,  -900,
    -900,  -900,  -900,  -900,  -900,    60, 11038, 11038,  2883,   378,
    -900,  -900,  -900,  -900,  4967,  4967,    60,  4967,   -34,  -900,
    -900,  -900, 11038,  -900, 11038,  9508,  -900,   381,   380,  7815,
    7842,  8182,  8581,  8601,  8621,   382,  2920,  2943,  3717,  3740,
     383, 11038, 11038, 11038, 11038, 11038, 11038, 11038, 11038, 11038,
    -900,   176,  -900,   385,   387, 11038,  3763,  8814,  3858,  -900,
     388, 11038, 11038, 11038,  -310,  4229,   389,  4256,  -900,  -900,
    4277,  4967,   390,    13,  8856,  4419,   289, 11038, 11038,   289,
     394,  8313,   395,  8712,  9089,  8712,  9089,  -900, 11038, 11038,
    4967, 11038,  -900, 11038,  -900,  4967,  4967,  4967, 11038,  4967,
    4967, 11038,  -900,  -900,  -900,  -900,  -900,  -900,   176,  -900,
    4967, 11038,  -900,  4967,  -900, 11038,  -900,  -900,  -900,  -900,
    -900,  4967,   391,  -900,  -900,   399,   405, 11038,  -900,  -900,
    -900,  -900,  -900,  -900,  -900,  8352,  8352, 11038, 11038, 11038,
   11038,   407,  8951,  8995,   408,  4467,  4579,  4600,  4644,  4681,
   11038,  -900,  4906,    60,  -900,   -25,  -900,  4967,  4946,   410,
     289, 11038,  -900, 11038, 11038,  -900,    60,  -900, 11038,   -31,
    5179,  5208,   176, 11038,  -900,  4967,  4967,  4967,  4967,  4967,
    4967,  4967,  4967,  -900,  -900,    16,  -202,  9330,  9377,  9466,
    9705,  5281,  5392,  5439,  5576, 11038, 11038, 11038, 11038, 11038,
   11038, 11038,  -900, 11038,   412,  -900,  -900,  -900,   419, 11038,
   11038,   176,   400,   403,  5719,   423,  5740,   289, 11038, 11038,
     425,  -900,  -900,  -900,  -900,  -900,  -900,  -900,   428,  -900,
   11038,  -900,  -900,  -900,  -900,  8352, 11038, 11038, 11038,   424,
    9727,  9752,  5785,  5911,  6119,  4967,  4967,    60,    60,  4967,
     431,  -900,  -900, 11038,  -900, 11038,   176,  4967,  4967,    60,
      38,   432,  9906, 10119,  6293,  6472,  6589,  -900, 11038, 11038,
   11038,   452,   455, 11038,  4967,  1348,   460,  -900,  -900,  -900,
     434, 11038,  -900,  -900,  8352, 11038, 11038, 10151,  6678,  6703,
    -900,  -900,   463, 11038,  -900,   244,   464, 10242, 10344,  6822,
     335, 11038, 11038,   465,  6962,  -900,  -900,  -900,   467, 11038,
    -900,  -900,  8352,  -900,    60, 10386,  6991,   289, 11038,  -900,
   10486, 10528, 10858,   468, 11038,   176,  7114,  -900,  -900,  -900,
      60, 10878, 11038,   469,  7137,    60, 11038,   -45,  4967,  -900,
      60,   472,  -900
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -900,  -900,  -900,   351,     0,  1524,  -170,  1755,   -37,  -900,
    -900,  1140,  2000,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,  -900,   290,  -900,  -900,  -900,  -900,  -900,  2371,
     221,  -900,  -164,  -900,  -900,   829,  -900,   -76,  -900,  -900,
    -900,  -900,   360,   769,  -900,  -301,  -900,  -900,  -899,  -900,
    -900,   -20,  -586,  -900,  -900,  -900,  -900,  -900,  -900,  -900,
    -900,  -900,   -52,    48,   -14,    23,   195,  -192,  -900,  -900,
    -900,  -900,  -900,  -900,  -292,  -174,  -149
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -171
static const yytype_int16 yytable[] =
{
     154,   697,  1016,   569,  1547,   428,   282,   283,   608,   191,
     380,   613,   355,   617,   618,   510,   621,  1409,  1230,   815,
    1535,   202,   362,   690,  1233,   202,  1141,   250,  1447,  1319,
    1282,  1350,  1309,   350,   177,   484,  1258,  1259,  1196,   289,
     295,   691,   241,   397,   446,   593,  1145,   242,   203,  1226,
     256,   460,   923,   465,   302,  1607,   280,   476,   481,   443,
    1240,  1434,   378,   299,   388,   504,  1142,   797,  1436,   507,
     351,   202,  1153,   830,   394,   255,  1551,   202,   191,   241,
     179,   278,   627,   831,   242,   382,  1490,   392,   298,   191,
     398,  1197,   798,    49,    50,   349,   525,   931,   444,    49,
      50,   541,  1249,   457,   395,   178,   354,   627,   765,   474,
     381,   202,   391,   932,   180,   491,   259,   260,   543,   430,
     181,   238,   239,   502,   772,   774,   461,   776,   466,  1283,
     182,   700,   477,   482,   692,   264,   241,   693,   183,  1260,
    1297,   242,   509,   192,   191,  1536,   184,   514,   516,   519,
     521,  1552,  1553,   185,   241,   594,   633,  1423,   241,   242,
     186,   261,   933,   242,   924,   303,  1608,   781,   274,   447,
    1448,   430,   505,   238,   239,   968,   508,   446,   606,   610,
     191,   612,   615,   934,   709,   619,   202,   622,   623,   624,
     625,   591,   782,  1559,  1560,   262,   604,   187,   241,   266,
    1310,   241,   595,   242,  1311,   263,   242,   238,   239,   363,
     241,    49,    50,  1299,   241,   242,   590,  1284,   274,   242,
    1554,   168,   257,   653,   935,   936,   937,  1261,   281,   304,
      96,    97,   925,   368,   188,   300,   627,  1555,   191,   657,
     195,  1249,   352,   201,   202,   264,  1250,   661,   662,   663,
     305,  1231,   271,   345,   431,  1537,  1251,   383,   274,   393,
     654,  1609,  1320,  1449,  1351,   926,   265,  1158,   241,   650,
     189,   938,   421,   242,   698,   939,  1262,   202,   783,   695,
     784,   999,   365,   239,   634,   627,   658,   432,   596,   597,
     635,   368,   400,   198,  1435,   659,  1346,   598,   462,   785,
     467,  1437,   447,   702,   478,   483,   431,   190,   440,   266,
     541,   627,   636,  1014,   196,   542,   696,   346,  1635,  1556,
     445,   940,   449,   946,   947,   833,   834,   468,   469,   941,
     942,   705,   627,  1015,  1417,   511,  1557,  1286,   197,   432,
     993,   239,  1287,   497,   433,   503,   202,  1300,    96,    97,
    1669,  1670,   721,   729,   835,  1266,   710,   525,   191,   713,
     284,   627,   272,   592,   288,   786,   787,   741,   706,   356,
     202,   727,   887,   238,   239,   301,   732,   733,  1301,   267,
     446,   354,   943,   358,   463,  1250,   268,   788,   401,   722,
     755,   359,   525,   836,   419,  1251,   541,   360,   651,   747,
     756,   542,  1032,  -170,  1058,  1083,   753,   433,   361,  1496,
     241,   199,   762,   384,   720,   242,   652,   273,  1305,   399,
     657,  1249,  1060,  1085,   274,   402,   420,  1636,   423,   777,
     780,   429,   792,   438,  1090,   805,   757,   439,   441,   200,
     506,   713,   479,   789,   523,   790,   512,    49,    50,   513,
     801,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   538,   539,   808,   524,   540,   238,   239,   560,  1637,
    1253,   613,   601,  1113,   602,  1115,   603,   202,   629,   814,
     525,   630,   178,   641,   651,   642,   241,   816,   817,   818,
     819,   242,   537,   538,   539,   129,   130,   540,   825,   826,
     827,   828,  1267,   643,   644,   447,   347,   758,   645,   694,
    1012,   842,   843,   844,   646,   647,   648,   274,   649,   966,
     134,   135,   136,   137,   138,   139,   857,   858,   860,   862,
     864,   866,   868,   870,   872,   874,   876,   878,   880,   882,
     884,   568,  1029,   588,  1055,   889,   891,   893,   895,   897,
     899,   901,   903,   905,   907,   909,   911,   913,   915,   917,
     919,   723,   980,   988,   307,  1250,   275,   995,   627,   238,
     239,  1009,  1306,   238,   239,  1251,   680,   681,   687,   712,
     688,   715,   716,  1000,    96,    97,   717,  1013,   276,   724,
     752,   556,   557,   558,   967,  1035,   559,   744,   749,   981,
     989,   802,   809,   713,   996,   735,   241,   810,  1010,   677,
     241,   242,   886,   433,   308,   242,  1020,  1030,   540,  1056,
    1081,   559,   544,   751,   979,    49,    50,  1106,   888,   994,
     921,   922,  1086,  1008,   674,   675,   676,  1110,  1016,   677,
    1092,  1019,  1112,  1091,  1114,  1093,  1116,  1117,  1118,  1119,
    1148,  1097,  1098,  1101,   793,   525,  1109,  1149,  1151,   794,
     795,  1152,  1153,  1154,  1120,  1121,  1122,  1123,  1124,  1125,
    1126,  1127,  1128,  1129,  1130,  1131,  1132,  1133,  1134,  1156,
    1172,  1136,  1137,  1157,  1165,  1173,  1175,  1138,  1177,  1180,
    1144,   569,  1182,  1186,  1268,  1183,  1192,   177,  1193,  1150,
    1239,   241,  1194,   945,  1160,  1222,   242,  1247,  1281,   526,
    1295,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,  1162,  1321,   677,   838,  1333,  1334,
    1643,  1644,  1352,  1354,  1164,  1355,  1166,  1167,  1168,  1371,
    1372,  1161,  1170,   492,   526,   856,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,  1374,
     202,   677,   309,  1380,   525,  1393,   678,  1184,  1185,  1406,
    1432,   982,  1441,  1433,  1445,  1458,   997,  1457,  1465,  1470,
    1011,  1480,  1481,  1486,  1492,  1495,  1513,   493,  1021,  1031,
    1503,  1057,  1082,  1504,  1514,  1581,   254,  1022,  1582,   191,
     718,  1515,  1200,  1525,  1526,   202,  1540,  1201,  1577,   297,
    1203,    49,    50,  1204,   311,  1578,   348,  1223,  1584,  1597,
    1218,  1589,  1087,  1088,  1590,   946,   947,  1603,  1611,  1219,
    1625,   379,   526,   390,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,  1620,   494,   677,
    1621,  1108,   368,   525,   699,  1624,   238,   239,  1633,   956,
    1639,  1647,  1649,   285,  1660,  1665,  1389,  1672,   840,   957,
     948,   949,   459,   950,   951,   853,  1288,     0,   475,   591,
       0,   952,   953,   954,   955,     0,   525,     0,  1227,     0,
    1023,   129,   130,   241,  1024,   958,     0,     0,   242,     0,
       0,  1135,   239,   202,   590,     0,     0,   274,     0,  1237,
       0,     0,     0,     0,     0,     0,   134,   135,   136,   137,
     138,   139,  1232,     0,  1234,     0,  1236,  1358,     0,     0,
       0,  1365,     0,     0,     0,     0,   570,   589,     0,   600,
       0,     0,   525,     0,     0,  1025,   605,     0,    96,    97,
     368,  1274,   959,     0,     0,     0,     0,     0,  1026,     0,
    1241,   626,     0,     0,     0,     0,   632,     0,   495,     0,
    1176,     0,     0,  1248,     0,     0,  1257,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,     0,
    1276,   559,  1205,  1206,  1207,  1208,  1209,  1210,  1211,  1212,
    1213,  1214,  1215,  1216,     0,  1278,     0,   526,     0,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,     0,  1255,   677,     0,  1291,  1277,     0,   734,
    1202,     0,     0,     0,     0,     0,   241,   960,     0,  1296,
     525,   242,  1304,     0,     0,     0,     0,     0,     0,  1316,
    1318,   592,     0,     0,     0,     0,  1412,     0,     0,     0,
       0,     0,     0,     0,   961,   962,  1327,     0,     0,  1338,
    1344,  1345,     0,     0,  1347,     0,     0,     0,     0,     0,
       0,     0,     0,   963,     0,     0,     0,     0,     0,  1331,
     525,     0,  1342,     0,     0,     0,     0,     0,  1359,  1361,
    1363,     0,  1366,  1367,  1368,  1369,     0,     0,     0,     0,
       0,     0,     0,     0,  1330,   707,   526,  1341,   664,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,     0,   964,   677,     0,   719,     0,     0,   742,   726,
     155,     0,     0,     0,     0,     0,     0,     0,   525,     0,
       0,     0,     0,     0,   713,   713,     0,   713,     0,     0,
       0,     0,     0,   743,     0,     0,     0,     0,     0,     0,
    1256,     0,     0,     0,  1395,     0,     0,   612,     0,  1132,
       0,  1132,     0,     0,     0,     0,     0,     0,     0,     0,
    1400,  1401,     0,     0,   713,     0,     0,     0,     0,  1405,
       0,     0,     0,     0,  1474,   526,     0,   664,   665,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
       0,     0,   677,     0,     0,  1411,     0,   807,  1413,  1415,
       0,     0,     0,     0,     0,     0,     0,     0,   526,  1420,
     664,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,     0,     0,   677,     0,     0,     0,     0,
     813,  1332,     0,     0,  1343,     0,     0,     0,     0,     0,
       0,  1425,     0,   832,     0,     0,   839,     0,     0,   841,
    1426,  1427,     0,     0,     0,     0,     0,     0,   517,   517,
     517,     0,  1429,     0,   526,  1430,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,     0,
       0,   677,     0,     0,     0,     0,   820,     0,     0,     0,
       0,   517,   517,     0,     0,   517,  1440,     0,     0,     0,
       0,     0,   927,   928,   929,   930,  1443,     0,   600,     0,
     965,   969,     0,     0,   972,   978,     0,   990,     0,   525,
       0,   998,  1451,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1018,     0,     0,  1054,  1079,     0,     0,     0,
     525,     0,  1472,  1473,     0,  1475,  1476,  1477,  1478,  1479,
       0,     0,     0,     0,     0,  1482,  1390,     0,     0,     0,
       0,  1487,  1488,     0,   544,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,  1500,  1501,   677,
       0,     0,     0,     0,     0,     0,     0,     0,  1505,  1506,
     525,  1507,     0,  1508,     0,     0,     0,     0,  1509,     0,
       0,  1510,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1511,     0,  1410,     0,  1512,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,   539,     0,   525,   540,
    1146,     0,     0,     0,     0,  1517,  1519,  1521,  1522,  1523,
    1524,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   525,     0,  1159,     0,     0,
       0,     0,     0,     0,  1544,     0,     0,     0,  1546,   517,
     526,     0,   664,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,     0,     0,   677,     0,     0,
       0,  1178,   829,     0,     0,   544,  1570,  1571,  1572,  1573,
    1574,  1575,     0,  1576,     0,     0,     0,     0,     0,  1579,
     193,   525,     0,     0,     0,     0,     0,     0,  1587,  1588,
       0,     0,   258,     0,   270,     0,     0,     0,     0,   286,
     287,     0,     0,   290,     0,  1592,  1594,  1595,  1596,     0,
       0,     0,   353,     0,     0,     0,   357,     0,     0,     0,
       0,   517,     0,  1604,     0,  1605,     0,     0,     0,     0,
     396,     0,     0,     0,     0,     0,     0,     0,  1617,  1618,
    1619,     0,   544,   422,     0,   427,     0,     0,     0,     0,
       0,     0,     0,     0,  1627,  1629,  1630,     0,   442,     0,
       0,     0,     0,  1634,   570,   589,   525,     0,     0,   464,
       0,  1645,  1646,     0,   480,     0,     0,   485,     0,  1650,
       0,     0,  1651,     0,  1623,     0,     0,   525,  1656,     0,
       0,     0,     0,     0,  1661,     0,     0,     0,     0,     0,
       0,     0,  1664,     0,   854,     0,  1668,   517,   517,   517,
     517,   517,   517,   517,   517,   517,   517,   517,   517,   517,
     517,   525,     0,     0,     0,   517,   517,   517,   517,   517,
     517,   517,   517,   517,   517,   517,   517,   517,   517,   517,
     517,   526,   607,   527,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,   538,   539,   525,   628,   540,     0,
       0,     0,   526,  1099,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,     0,     0,   677,
       0,     0,     0,   517,  1254,     0,   545,   640,   546,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,     0,     0,   559,     0,     0,     0,     0,  1100,     0,
       0,  1272,   526,     0,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,     0,     0,   677,
       0,     0,     0,     0,  1102,     0,     0,   683,     0,   686,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   689,
     526,     0,   664,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,     0,   701,   677,   703,   417,
    1322,     0,  1103,     0,  1324,  1329,   525,   526,  1340,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,     0,     0,   677,  1353,     0,   451,     0,  1104,
       0,     0,     0,   470,     0,     0,     0,     0,     0,   486,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     711,   548,   549,   550,   551,   552,   553,   554,   555,   556,
     557,   558,   683,   526,   559,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,     0,     0,
     677,     0,   525,     0,  1382,  1105,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   750,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   611,     0,     0,     0,     0,
       0,     0,     0,   766,     0,     0,     0,   525,   770,   771,
     773,   631,   775,   778,   545,     0,   546,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,     0,
       0,   559,     0,     0,     0,     0,  1155,   803,   526,     0,
     664,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,   656,     0,   677,   811,     0,     0,   526,
    1174,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,     0,     0,   677,     0,     0,     0,
     525,  1190,  1424,     0,   202,     0,     0,     0,   684,   685,
       0,     0,     0,   526,     0,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,     0,     0,
     677,     0,     0,     0,     0,  1191,     0,     0,     0,     0,
     704,   525,     0,     0,     0,     0,     0,     0,   526,     0,
     664,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,     0,     0,   677,     0,     0,   544,     0,
    1199,     0,     0,     0,  1439,     0,     0,   970,     0,     0,
       0,     0,   708,   991,  1442,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1446,   973,     0,  1027,     0,
     238,   239,     0,   684,   725,     0,     0,   728,     0,   730,
     731,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     740,     0,     0,     0,   525,     0,     0,     0,   518,   520,
     522,  1107,   745,   746,   748,     0,     0,   241,     0,     0,
       0,   754,   242,     0,     0,   761,     0,     0,   764,     0,
       0,   274,     0,     0,     0,     0,     0,   974,   769,     0,
       0,   614,   616,     0,     0,   620,     0,     0,   526,     0,
     664,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,   975,   800,   677,     0,     0,     0,   804,
    1271,   806,     0,     0,     0,     0,  1147,     0,   525,     0,
       0,     0,   976,     0,     0,     0,     0,     0,   812,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   517,   517,
     517,     0,     0,     0,     0,     0,     0,   821,   822,   823,
     824,     0,  1534,   977,  1163,   525,     0,     0,     0,     0,
       0,     0,     0,     0,   526,  1545,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,   525,
       0,   677,     0,     0,     0,     0,  1280,     0,     0,     0,
       0,     0,     0,     0,   517,   517,  1187,   517,  1188,   526,
    1189,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,     0,     0,   677,   517,     0,     0,
       0,  1292,     0,     0,     0,     0,     0,   525,     0,   971,
       0,     0,   987,     0,   517,   992,     0,     0,     0,  1001,
    1007,     0,     0,     0,     0,     0,  1601,  1602,     0,     0,
       0,  1052,  1077,     0,  1220,  1221,     0,     0,  1606,   714,
       0,     0,     0,     0,     0,     0,     0,     0,   517,   517,
       0,   167,   526,     0,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,     0,     0,   677,
     194,     0,  1111,   194,  1373,     0,     0,     0,   252,     0,
       0,     0,   194,     0,     0,     0,     0,   685,     0,     0,
       0,   296,     0,  1653,     0,     0,   310,  -171,  -171,  -171,
    -171,  -171,  -171,   535,   536,   537,   538,   539,     0,  1663,
     540,     0,   364,     0,  1667,   389,     0,     0,     0,  1671,
       0,   796,   194,  1140,  -171,  -171,  -171,  -171,  -171,  -171,
     554,   555,   556,   557,   558,     0,     0,   559,   194,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     194,     0,   194,     0,   458,   525,     0,   194,   194,     0,
       0,  1238,     0,     0,     0,     0,  1243,     0,     0,     0,
       0,     0,     0,   496,  1169,   194,   526,   525,   664,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,     0,     0,   677,     0,     0,     0,     0,  1375,     0,
       0,     0,     0,  1270,   525,     0,     0,   859,   861,   863,
     865,   867,   869,   871,   873,   875,   877,   879,   881,   883,
     885,     0,     0,     0,     0,   890,   892,   894,   896,   898,
     900,   902,   904,   906,   908,   910,   912,   914,   916,   918,
     920,  1294,   525,     0,  1298,     0,     0,     0,     0,  1308,
     526,     0,   664,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,     0,     0,   677,     0,     0,
       0,     0,  1376,     0,     0,   525,     0,     0,     0,     0,
       0,     0,     0,  1089,     0,   517,   517,   526,     0,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,   525,     0,   677,     0,     0,     0,     0,  1377,
       0,   526,     0,   664,   665,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,     0,     0,   677,     0,
       0,     0,     0,  1378,     0,     0,     0,     0,     0,   194,
       0,   525,     0,     0,     0,     0,     0,  1381,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   526,
    1235,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,     0,     0,   677,  1394,   525,     0,
       0,  1379,     0,     0,     0,   517,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1242,     0,  1244,  1245,
       0,   525,     0,  1246,     0,     0,     0,     0,     0,   194,
       0,   194,   194,     0,     0,     0,     0,     0,     0,  1264,
    1265,     0,     0,     0,     0,     0,     0,     0,  1269,     0,
       0,     0,     0,     0,   517,   194,     0,     0,  1273,  1275,
       0,  1279,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   194,     0,   525,     0,     0,     0,     0,
       0,     0,   517,     0,     0,     0,     0,     0,     0,     0,
       0,  1293,     0,     0,     0,     0,     0,     0,     0,  1307,
       0,     0,  1313,  1314,   194,     0,     0,     0,     0,   194,
     194,     0,     0,     0,     0,     0,     0,     0,  1323,     0,
       0,  1328,     0,     0,  1339,     0,     0,   525,     0,     0,
    1349,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,     0,     0,   677,  1438,  1357,     0,     0,   526,
       0,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,  1370,     0,   677,     0,     0,     0,
       0,  1385,     0,     0,     0,     0,   526,   194,   664,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,     0,     0,   677,     0,   855,     0,     0,  1391,     0,
       0,     0,     0,  1383,  1384,   525,     0,     0,     0,     0,
       0,     0,     0,     0,   526,     0,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,     0,
    1499,   677,  1396,  1502,     0,  1398,  1392,     0,  1399,     0,
       0,     0,   525,     0,     0,     0,     0,   526,  1402,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,     0,     0,   677,   525,     0,     0,     0,  1407,
       0,     0,   194,   194,   526,     0,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,     0,
       0,   677,     0,     0,     0,     0,  1408,     0,     0,     0,
       0,   194,  1422,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   526,  1541,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,     0,     0,
     677,     0,     0,     0,     0,  1418,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     526,   194,   664,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,     0,     0,   677,     0,     0,
       0,  1586,  1419,   526,     0,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,     0,     0,
     677,     0,  1444,     0,     0,  1421,     0,     0,  1360,  1362,
    1364,     0,     0,     0,     0,     0,     0,     0,     0,  1452,
    1455,     0,     0,     0,     0,  1171,     0,     0,     0,     0,
     194,     0,     0,     0,     0,     0,  1471,   526,     0,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,     0,     0,   677,     0,     0,     0,  1489,  1428,
       0,     0,     0,     0,  1386,  1387,     0,  1388,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1655,     0,     0,     0,     0,     0,  1397,     0,   526,
     194,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,  1404,     0,   677,     0,     0,     0,
       0,  1431,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1516,     0,     0,     0,     0,     0,     0,     0,
       0,   525,     0,     0,     0,     0,     0,     0,  1414,  1416,
       0,     0,     0,     0,     0,  1532,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1542,     0,  1543,     0,
       0,     0,     0,     0,     0,     0,     0,   526,  1550,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,     0,     0,   677,     0,     0,     0,     0,   -87,
    1569,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   526,  1580,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,     0,
       0,   677,     0,     0,     0,  1591,  1466,   526,     0,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,     0,     0,   677,     0,     0,     0,     0,  1467,
     525,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    -7,     1,  1622,     0,
       0,     0,     0,     0,     0,     0,  1626,     0,     0,     0,
       0,     0,     0,     0,     2,     3,     4,     5,     0,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,     0,
       0,    16,     0,    17,    18,     0,     0,    19,    20,     0,
       0,     0,    21,    22,    23,     0,    24,    25,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    26,     0,     0,
      27,    28,     0,     0,     0,     0,    29,     0,     0,    30,
       0,     0,     0,     0,     0,     0,    31,    32,     0,     0,
       0,     0,     0,     0,     0,    33,    34,     0,     0,     0,
       0,     0,     0,     0,    35,    36,    37,    38,     0,     0,
       0,     0,     0,    39,    40,  1518,  1520,    41,     0,    42,
       0,     0,    43,     0,     0,    44,    45,     0,     0,     0,
      46,     0,    47,     0,     0,     0,     0,    48,    49,    50,
       0,     0,     0,    51,     0,    52,     0,     0,    53,     0,
       0,     0,     0,     0,     0,    54,    55,     0,     0,     0,
      56,    57,    58,    59,     0,    60,     0,    61,     0,     0,
       0,     0,    62,     0,    63,    64,     0,     0,     0,    65,
      66,     0,     0,     0,     0,     0,   194,     0,     0,     0,
       0,    67,    68,    69,    70,     0,     0,     0,   525,     0,
       0,     0,     0,     0,     0,     0,     0,    71,     0,    72,
       0,    73,     0,     0,     0,  1593,     0,     0,     0,     0,
       0,     0,     0,   526,    74,   527,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,   538,   539,     0,    75,
     540,     0,     0,   194,    76,    77,    78,    79,    80,    81,
       0,     0,    82,    83,    84,    85,     0,    86,    87,     0,
       0,     0,     0,    88,  1628,    89,    90,    91,    92,    93,
      94,     0,     0,     0,    95,    96,    97,     0,     0,     0,
       0,    98,     0,    99,   100,     0,     0,   101,   102,     0,
       0,     0,  1652,     0,     0,   103,   104,   105,   106,     0,
     107,     0,     0,     0,   108,   109,   110,     0,     0,     0,
     111,     0,   112,   113,   114,     0,     0,     0,     0,   115,
     116,   117,   118,   119,   120,   121,     0,   122,   123,     0,
     124,   125,   126,   127,   128,   129,   130,     0,   131,   132,
     133,     0,   526,  1225,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,     0,     0,   677,
     134,   135,   136,   137,   138,   139,   140,   141,    -7,     1,
       0,     0,     0,     0,   142,   143,   144,   145,   146,     0,
       0,     0,     0,     0,     0,     0,   847,   848,     4,   525,
     147,   148,     0,     0,     0,   149,     0,     0,     0,    -7,
     150,     0,     0,    16,     0,    17,    18,     0,     0,    19,
      20,     0,   525,     0,    21,    22,    23,     0,     0,    25,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    26,
       0,     0,    27,    28,     0,   544,     0,     0,    29,     0,
       0,    30,     0,     0,     0,     0,     0,     0,    31,    32,
       0,     0,     0,     0,     0,     0,     0,    33,    34,     0,
       0,     0,     0,     0,     0,     0,    35,    36,    37,    38,
       0,     0,     0,     0,     0,    39,    40,     0,     0,    41,
       0,    42,     0,     0,    43,     0,     0,    44,    45,     0,
       0,     0,    46,     0,    47,     0,     0,     0,     0,    48,
      49,    50,     0,     0,     0,    51,     0,    52,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,    59,     0,    60,     0,    61,
     544,     0,     0,     0,    62,     0,    63,    64,     0,     0,
       0,    65,    66,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    67,  -171,  -171,  -171,  -171,  -171,  -171,
     672,   673,   674,   675,   676,     0,     0,   677,     0,    71,
       0,    72,     0,    73,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,     0,     0,     0,     0,    76,    77,    78,    79,
       0,    81,     0,     0,    82,    83,   849,    85,     0,    86,
      87,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,    94,     0,     0,     0,     0,    96,    97,     0,
       0,     0,     0,    98,     0,    99,   100,     0,     0,   101,
     102,     0,     0,     0,     0,     0,     0,   103,     0,   105,
     106,     0,   107,     0,     0,     0,   108,   109,   110,     0,
       0,     0,   111,     0,   112,   113,   114,     0,     0,     0,
       0,   115,     0,     0,     0,     0,     0,     0,     0,   122,
     123,     0,   124,     0,     0,     0,     0,   129,   130,     0,
     131,   132,   133,   224,   225,   226,     5,     0,   227,   228,
       8,   229,   230,   231,   232,   233,   234,   235,     0,     0,
       0,     0,   134,   135,   136,   137,   138,   139,   140,   141,
       0,     0,     0,     0,     0,    24,   142,   850,   851,   852,
       0,   526,     0,   664,   665,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,     0,     0,   677,     0,
       0,    -7,     0,  1468,   526,     0,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,     0,
       0,   677,     0,     0,     0,     0,  1469,   545,     0,   546,
     547,   548,   549,   550,   551,   552,   553,   554,   555,   556,
     557,   558,     0,     0,   559,     0,     0,     0,     0,  1483,
     525,     0,     0,     0,     0,     0,     0,    49,    50,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,    55,   204,   205,   206,    56,
       0,     0,     0,     0,     0,     0,     0,     0,   224,   225,
     226,     5,     0,   227,   228,     8,   229,   230,   231,   232,
     233,   234,   235,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,    70,     0,     0,     0,     0,     0,     0,
      24,     0,   545,     0,   546,   547,   548,   549,   550,   551,
     552,   553,   554,   555,   556,   557,   558,     0,     0,   559,
       0,   525,     0,     0,  1485,     0,     0,     0,   312,     0,
       0,     0,   313,   314,   315,   316,   317,   318,     0,     0,
       0,   319,   320,   321,   322,   323,   324,   243,   544,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   325,
       0,     0,   326,     0,     0,     0,     0,     0,     0,   525,
       0,     0,     0,    95,    96,    97,     0,     0,     0,     0,
     327,     0,    49,    50,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   328,     0,   104,     0,     0,     0,    54,
      55,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   116,
     117,   118,   119,   120,   121,     0,     0,     0,     0,     0,
     125,   126,   127,   128,   129,   130,    68,     0,    70,     0,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,     0,     0,     0,     0,     0,     0,   544,     0,   134,
     135,   136,   137,   138,   139,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   244,   245,   246,   146,     0,     0,
       0,     0,     0,     0,     0,   340,     0,     0,     0,   147,
     247,     0,   243,     0,   248,     0,     0,     0,     0,   249,
       0,     0,     0,  1139,     0,     0,     0,     0,     0,     0,
       0,   525,     0,     0,     0,     0,   341,     0,    95,    96,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     104,   207,     0,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,     0,   525,
       0,     0,     0,     0,   116,   117,   118,   119,   120,   121,
       0,     0,     0,     0,     0,   125,   126,   127,   128,   129,
     130,     0,   526,     0,   527,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,   539,     0,     0,   540,
       0,     0,     0,   845,   134,   135,   136,   137,   138,   139,
       0,     0,     0,     0,     0,   342,   343,     0,     0,   244,
     245,   246,   146,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   147,   247,     0,     0,     0,   248,
     224,   225,   226,     5,   249,   227,   228,     8,   229,   230,
     231,   232,   233,   234,   235,     0,     0,     0,     0,     0,
       0,  1036,     0,  1037,     0,     0,     0,     0,     0,     0,
       0,   525,    24,   526,     0,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,     0,  1038,
     677,     0,   525,     0,     0,  1491,     0,   202,     0,     0,
     545,     0,   546,   547,   548,   549,   550,   551,   552,   553,
     554,   555,   556,   557,   558,     0,     0,   559,     0,     0,
       0,   526,  1493,   664,   665,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,   525,     0,   677,     0,
       0,     0,     0,  1494,   368,     0,  1039,  1040,     0,     0,
       0,     0,     0,     0,    49,    50,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,    55,   525,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1041,     0,     0,     0,
       0,     0,     0,     0,   239,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    68,   545,
      70,   546,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,     0,     0,   559,     0,     0,     0,
     241,  1042,     0,     0,     0,   242,     0,  1025,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1043,     0,     0,     0,     0,     0,     0,     0,  1044,  1045,
       0,     0,     0,   526,   243,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,     0,     0,
     677,     0,     0,     0,     0,  1498,     0,     0,     0,     0,
      95,    96,    97,     0,  1046,     0,  1047,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1048,  1049,
    1050,   526,   104,   664,   665,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,  1051,     0,   677,     0,
       0,     0,     0,  1527,     0,     0,   116,   117,   118,   119,
     120,   121,     0,     0,     0,     0,     0,   125,   126,   127,
     128,   129,   130,   224,   225,   226,     5,     0,   227,   228,
       8,   229,   230,   231,   232,   233,   234,   235,     0,     0,
       0,  1061,     0,     0,     0,     0,   134,   135,   136,   137,
     138,   139,     0,     0,     0,    24,     0,     0,     0,     0,
       0,   244,   245,   246,   146,     0,     0,  1062,   525,     0,
       0,     0,     0,     0,     0,     0,   147,   247,     0,     0,
     202,   248,     0,     0,     0,     0,   249,     0,     0,     0,
       0,     0,     0,   526,  1063,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,   525,     0,
     677,     0,     0,     0,   526,  1528,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,   525,
       0,   677,     0,     0,     0,     0,  1529,    49,    50,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   525,     0,    54,    55,  1064,     0,   526,    56,
     664,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,     0,     0,   677,     0,   239,     0,     0,
    1530,  1065,     0,     0,     0,     0,     0,     0,     0,  1066,
       0,  1067,     0,  1068,  1069,   526,     0,   664,   665,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
       0,     0,   677,   241,   544,  1070,  1071,  1531,   242,     0,
    1025,     0,  1072,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1073,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   243,     0,     0,
       0,   224,   225,   226,     5,     0,   227,   228,     8,   229,
     230,   231,   232,   233,   234,   235,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,     0,     0,  1074,     0,
       0,     0,     0,    24,  1075,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   104,     0,   366,     0,     0,
     367,     0,     0,     0,     0,     0,     0,     0,   202,  1076,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   116,
     117,   118,   119,   120,   121,     0,     0,     0,     0,     0,
     125,   126,   127,   128,   129,   130,     0,     0,     0,     0,
       0,   525,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   368,     0,     0,     0,   134,
     135,   136,   137,   138,   139,    49,    50,     0,     0,     0,
     525,     0,     0,   369,   244,   245,   246,   146,     0,     0,
       0,     0,    54,    55,     0,     0,     0,    56,     0,   147,
     247,   370,     0,     0,   248,     0,     0,   371,     0,   249,
     372,     0,     0,     0,   238,   239,     0,     0,   373,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
     526,    70,   664,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,     0,     0,   677,     0,     0,
       0,   241,  1533,   525,     0,     0,   242,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     526,     0,   664,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,   243,     0,   677,     0,     0,
       0,   526,  1539,   664,   665,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,     0,     0,   677,     0,
       0,    95,    96,    97,   526,  1228,   527,   528,   529,   530,
     531,   532,   533,   534,   535,   536,   537,   538,   539,     0,
       0,   540,   374,   104,   224,   225,   226,     5,     0,   227,
     228,     8,   229,   230,   231,   232,   233,   234,   235,     0,
       0,     0,     0,     0,     0,   375,     0,   116,   117,   118,
     119,   120,   121,     0,   525,     0,    24,   403,   125,   126,
     127,   128,   129,   130,   376,     0,   545,  1229,   546,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,     0,     0,   559,     0,     0,     0,   134,   135,   377,
     137,   138,   139,     0,     0,     0,     0,     0,     0,     0,
       0,   525,   244,   245,   246,   146,     0,     0,     0,   404,
       0,     0,     0,     0,     0,     0,     0,   147,   247,     0,
       0,     0,   248,     0,     0,     0,     0,   249,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    49,    50,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   525,    54,    55,   405,     0,   406,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     407,   408,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    68,   526,    70,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,     0,     0,
     677,     0,     0,     0,     0,  1548,     0,     0,     0,     0,
       0,     0,   526,   409,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,     0,   525,   677,
       0,     0,     0,     0,  1549,     0,     0,     0,   243,     0,
       0,     0,     0,     0,     0,     0,   410,     0,     0,   411,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   412,    95,    96,    97,     0,     0,   413,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   526,   104,   664,   665,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
       0,   414,   677,     0,     0,     0,     0,  1565,     0,     0,
     116,   117,   118,   119,   120,   121,     0,     0,     0,     0,
       0,   125,   126,   127,   128,   129,   130,   224,   225,   226,
       5,     0,   227,   228,     8,   229,   230,   231,   232,   233,
     234,   235,     0,     0,     0,     0,     0,     0,     0,     0,
     134,   135,   136,   137,   138,   139,     0,     0,     0,    24,
       0,     0,     0,     0,     0,   244,   245,   246,   146,     0,
       0,   525,     0,     0,     0,     0,   452,     0,     0,     0,
     147,   247,     0,     0,   202,   248,     0,     0,     0,     0,
     249,     0,   525,     0,     0,     0,   526,   453,   664,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,     0,     0,   677,     0,     0,     0,     0,  1566,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   368,     0,     0,     0,     0,     0,   525,     0,     0,
       0,    49,    50,   526,     0,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,    54,    55,
     677,     0,     0,    56,     0,  1567,     0,     0,     0,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   239,     0,     0,   455,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,   526,    70,   664,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
    1179,     0,     0,   677,     0,     0,     0,   241,   224,   225,
     226,     5,   242,   227,   228,     8,   229,   230,   231,   232,
     233,   234,   235,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,   243,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   525,   456,     0,  1002,  1003,     0,     0,
       0,     0,     0,     0,     0,   202,     0,    95,    96,    97,
     526,     0,   664,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,     0,     0,   677,     0,   104,
       0,     0,  1568,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1004,     0,     0,     0,     0,
       0,     0,     0,   116,   117,   118,   119,   120,   121,     0,
       0,     0,    49,    50,   125,   126,   127,   128,   129,   130,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
      55,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,   134,   135,   136,   137,   138,   139,     0,
       0,   238,   239,     0,     0,     0,     0,     0,   244,   245,
     246,   146,     0,     0,     0,     0,    68,     0,    70,     0,
       0,     0,     0,   147,   247,     0,     0,     0,   248,     0,
       0,     0,     0,   249,     0,     0,     0,   224,   225,   226,
       5,     0,   227,   228,     8,   229,   230,   231,   232,   233,
     234,   235,   274,   526,     0,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,     0,    24,
     677,     0,   243,     0,   526,  1583,   664,   665,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,     0,
       0,   677,     0,     0,   202,     0,  1585,     0,    95,    96,
      97,   525,     0,  1005,  1006,     0,     0,   236,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   526,
     104,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,   237,     0,   677,     0,     0,     0,
       0,  1598,     0,     0,   116,   117,   118,   119,   120,   121,
     544,    49,    50,     0,     0,   125,   126,   127,   128,   129,
     130,     0,     0,     0,     0,     0,     0,     0,    54,    55,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   134,   135,   136,   137,   138,   139,
     238,   239,     0,     0,   240,     0,     0,     0,     0,   244,
     245,   246,   146,     0,     0,    68,     0,    70,     0,     0,
       0,     0,     0,     0,   147,   247,     0,     0,     0,   248,
       0,     0,     0,     0,   249,     0,     0,   241,   224,   225,
     226,     5,   242,   227,   228,     8,   229,   230,   231,   232,
     233,   234,   235,     0,     0,   526,     0,   664,   665,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
      24,   243,   677,     0,     0,     0,     0,  1599,     0,     0,
       0,     0,     0,     0,     0,   525,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   202,     0,    95,    96,    97,
       0,     0,     0,     0,     0,     0,     0,     0,   292,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   104,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   293,     0,     0,     0,     0,
       0,     0,     0,   116,   117,   118,   119,   120,   121,     0,
     525,     0,    49,    50,   125,   126,   127,   128,   129,   130,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
      55,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,   134,   135,   136,   137,   138,   139,     0,
       0,   238,   239,     0,     0,   294,     0,     0,   244,   245,
     246,   146,     0,     0,     0,     0,    68,     0,    70,     0,
       0,     0,     0,   147,   247,     0,     0,     0,   248,     0,
       0,     0,     0,   249,     0,     0,     0,     0,   241,   224,
     225,   226,     5,   242,   227,   228,     8,   229,   230,   231,
     232,   233,   234,   235,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   385,     0,     0,     0,     0,     0,
       0,    24,   243,   526,   525,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,     0,     0,
     677,     0,     0,     0,     0,  1600,   202,     0,    95,    96,
      97,     0,     0,     0,     0,     0,     0,     0,     0,   386,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     104,     0,   545,     0,   546,   547,   548,   549,   550,   551,
     552,   553,   554,   555,   556,   557,   558,     0,     0,   559,
       0,     0,     0,   846,   116,   117,   118,   119,   120,   121,
       0,     0,   544,    49,    50,   125,   126,   127,   128,   129,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      54,    55,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,   134,   135,   136,   137,   138,   139,
       0,   525,   238,   239,     0,     0,   387,     0,     0,   244,
     245,   246,   146,     0,     0,     0,     0,    68,     0,    70,
       0,     0,     0,     0,   147,   247,     0,     0,     0,   248,
       0,     0,     0,     0,   249,     0,     0,     0,     0,   241,
     224,   225,   226,     5,   242,   227,   228,     8,   229,   230,
     231,   232,   233,   234,   235,     0,     0,   526,     0,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,    24,   243,   677,     0,     0,     0,     0,  1614,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   471,
     525,     0,     0,     0,     0,     0,     0,   202,     0,    95,
      96,    97,     0,     0,     0,     0,     0,     0,     0,     0,
     472,     0,     0,     0,     0,   525,     0,     0,     0,     0,
       0,   104,   526,     0,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,     0,     0,   677,
       0,     0,     0,   -90,   368,   116,   117,   118,   119,   120,
     121,     0,     0,     0,    49,    50,   125,   126,   127,   128,
     129,   130,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,    55,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,   134,   135,   136,   137,   138,
     139,     0,     0,     0,   239,     0,     0,     0,     0,     0,
     244,   245,   246,   146,     0,     0,     0,     0,    68,     0,
      70,     0,     0,     0,     0,   147,   247,     0,     0,     0,
     248,     0,     0,     0,     0,   249,     0,     0,     0,     0,
     241,     0,     0,     0,   525,   242,   526,     0,   664,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,     0,     0,   677,     0,     0,     0,     0,  1615,     0,
       0,     0,     0,     0,   243,     0,     0,     0,     0,     0,
       0,     0,     0,   224,   225,   226,     5,   473,   227,   228,
       8,   229,   230,   231,   232,   233,   234,   235,     0,     0,
      95,    96,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    24,     0,     0,     0,     0,
       0,     0,   104,     0,   545,     0,   546,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,     0,
     202,   559,     0,     0,     0,  1094,   116,   117,   118,   119,
     120,   121,     0,     0,     0,     0,     0,   125,   126,   127,
     128,   129,   130,   526,     0,   664,   665,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,     0,     0,
     677,     0,     0,     0,   525,  1616,   134,   135,   136,   137,
     138,   139,     0,     0,     0,     0,     0,    49,    50,  1335,
       0,   244,   245,   246,   146,     0,     0,     0,     0,     0,
       0,     0,     0,   525,    54,    55,   147,   247,     0,    56,
       0,   248,     0,     0,     0,     0,   249,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238,   239,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,   526,    70,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,     0,     0,   677,
       0,     0,     0,   241,  1631,     0,     0,   526,   242,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,     0,     0,   677,     0,     0,     0,     0,  1632,
       0,     0,     0,     0,     0,     0,     0,   243,     0,     0,
       0,   224,   225,   226,     5,     0,   227,   228,     8,   229,
     230,   231,   232,   233,   234,   235,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,   525,     0,  1336,     0,
       0,     0,     0,    24,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   104,     0,    17,    18,   525,
       0,     0,  1337,     0,     0,     0,     0,   561,   202,     0,
       0,     0,   562,     0,     0,     0,     0,     0,     0,   116,
     117,   118,   119,   120,   121,     0,     0,     0,     0,     0,
     125,   126,   127,   128,   129,   130,   526,     0,   664,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,     0,     0,   677,     0,     0,     0,     0,  1642,   134,
     135,   136,   137,   138,   139,    49,    50,  1325,     0,     0,
       0,     0,     0,     0,   244,   245,   246,   146,   563,     0,
       0,     0,    54,    55,     0,     0,     0,    56,     0,   147,
     247,     0,     0,     0,   248,     0,     0,     0,     0,   249,
       0,   564,     0,     0,   238,   239,     0,     0,     0,     0,
       0,   204,   205,   206,     0,   424,     0,     0,     0,    68,
     425,    70,   565,     0,     0,     0,     0,     0,   191,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   241,   224,   225,   226,     5,   242,   227,   228,     8,
     229,   230,   231,   232,   233,   234,   235,     0,     0,     0,
       0,     0,     0,     0,     0,   525,     0,     0,   241,     0,
       0,     0,     0,   242,    24,   243,   526,     0,   664,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,     0,     0,   677,     0,     0,     0,     0,  1648,   202,
       0,    95,    96,    97,     0,   526,  1326,   664,   665,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
       0,     0,   677,   104,     0,     0,     0,  1654,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   544,
       0,     0,     0,   566,     0,     0,     0,   116,   117,   118,
     119,   120,   121,     0,     0,     0,    49,    50,   125,   126,
     127,   128,   129,   130,   567,     0,   426,     0,     0,     0,
       0,     0,     0,    54,    55,     0,   525,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,   134,   135,   136,
     137,   138,   139,   132,     0,     0,     0,     0,     0,     0,
       0,     0,   244,   245,   246,   146,     0,     0,     0,     0,
      68,     0,    70,     0,     0,     0,     0,   147,   247,     0,
       0,   141,   248,     0,     0,     0,     0,   249,   526,     0,
     664,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,     0,     0,   677,   274,     0,     0,     0,
    1662,   526,   568,   664,   665,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,   243,     0,   677,     0,
       0,     0,     0,  1666,     0,   544,     0,     0,     0,     0,
       0,     0,   984,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,     0,   207,     0,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,     0,   104,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   985,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   116,   117,
     118,   119,   120,   121,     0,   986,     0,     0,     0,   125,
     126,   127,   128,   129,   130,     0,   224,   225,   226,     5,
       0,   227,   228,     8,   229,   230,   231,   232,   233,   234,
     235,     0,     0,     0,     0,     0,     0,     0,   134,   135,
     136,   137,   138,   139,  1289,     0,     0,     0,    24,     0,
       0,     0,     0,   244,   245,   246,   146,     0,     0,     0,
    1249,     0,     0,     0,     0,     0,     0,     0,   147,   247,
       0,     0,     0,   248,     0,     0,     0,   526,   249,   527,
     528,   529,   530,   531,   532,   533,   534,   535,   536,   537,
     538,   539,     0,     0,   540,     0,     0,     0,  1095,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   224,   225,   226,     5,     0,   227,   228,     8,
     229,   230,   231,   232,   233,   234,   235,     0,     0,     0,
      49,    50,     0,     0,     0,     0,     0,   544,     0,     0,
       0,     0,     0,     0,    24,     0,     0,    54,    55,     0,
       0,   545,    56,   546,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,   557,   558,     0,     0,   559,     0,
       0,     0,  1096,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    70,     0,   526,     0,
     664,   665,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,     0,  1250,   677,     0,     0,     0,   845,
       0,     0,     0,     0,  1251,     0,     0,   525,     0,     0,
       0,     0,     0,     0,     0,     0,    49,    50,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,     0,     0,
     243,     0,     0,    54,    55,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   544,     0,    95,    96,    97,     0,
       0,     0,     0,     0,     0,     0,     0,  1290,     0,     0,
      68,     0,    70,     0,     0,     0,     0,   545,   104,   546,
     547,   548,   549,   550,   551,   552,   553,   554,   555,   556,
     557,   558,     0,     0,   559,     0,     0,     0,  1195,     0,
       0,     0,   116,   117,   118,   119,   120,   121,     0,     0,
       0,     0,     0,   125,   126,   127,   128,   129,   130,     0,
       0,     0,     0,     0,     0,     0,   243,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   134,   135,   136,   137,   138,   139,     0,     0,
       0,     0,    95,    96,    97,     0,     0,   244,   245,   246,
     146,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   147,   247,   104,     0,     0,   248,     0,     0,
       0,     0,   249,   224,   225,   226,     5,     0,   227,   228,
       8,   229,   230,   231,   232,   233,   234,   235,   116,   117,
     118,   119,   120,   121,     0,     0,     0,     0,     0,   125,
     126,   127,   128,   129,   130,    24,     0,     0,     0,   487,
     488,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   134,   135,
     136,   137,   138,   139,     0,     0,     0,   489,   490,     0,
       0,     0,     0,   244,   245,   246,   146,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   147,   247,
       0,     0,     0,   248,     0,     0,     0,     0,   249,   545,
       0,   546,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,   525,     0,   559,    49,    50,     0,
    1356,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,    55,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,   224,   225,
     226,     5,     0,   227,   228,     8,   229,   230,   231,   232,
     233,   234,   235,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,    70,     0,     0,     0,     0,     0,   526,
      24,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,     0,     0,   677,     0,     0,   526,
    1095,   527,   528,   529,   530,   531,   532,   533,   534,   535,
     536,   537,   538,   539,   525,     0,   540,     0,     0,     0,
    1459,     0,     0,     0,     0,     0,   545,   243,   546,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,     0,     0,   559,     0,     0,     0,  1460,     0,     0,
       0,     0,     0,    95,    96,    97,     0,     0,     0,     0,
       0,     0,    49,    50,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   104,     0,     0,     0,    54,
      55,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   736,     0,     0,   737,     0,   116,
     117,   118,   119,   120,   121,     0,     0,     0,     0,     0,
     125,   126,   127,   128,   129,   130,    68,     0,    70,     0,
     498,   499,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   134,
     135,   136,   137,   138,   139,   525,     0,     0,   500,   501,
       0,     0,     0,     0,   244,   245,   246,   146,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   147,
     247,     0,   243,     0,   248,     0,     0,     0,     0,   249,
     224,   225,   226,     5,     0,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,     0,     0,     0,    95,    96,
      97,     0,     0,   738,     0,     0,     0,     0,     0,     0,
       0,     0,    24,     0,     0,     0,     0,    17,    18,     0,
     104,     0,     0,     0,     0,     0,     0,   561,     0,     0,
       0,     0,   562,     0,   739,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   116,   117,   118,   119,   120,   121,
       0,     0,     0,     0,     0,   125,   126,   127,   128,   129,
     130,     0,     0,     0,     0,     0,   526,     0,   664,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
    1181,     0,     0,   677,   134,   135,   136,   137,   138,   139,
       0,     0,     0,     0,    49,    50,     0,     0,   563,   244,
     245,   246,   146,     0,     0,     0,     0,     0,     0,     0,
       0,    54,    55,     0,   147,   247,    56,     0,     0,   248,
       0,   564,     0,     0,   249,   224,   225,   226,     5,     0,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
       0,     0,   565,     0,     0,     0,     0,     0,    68,    69,
      70,     0,     0,     0,     0,     0,     0,    24,     0,     0,
       0,     0,     0,     0,     0,     0,   526,     0,   527,   528,
     529,   530,   531,   532,   533,   534,   535,   536,   537,   538,
     539,     0,     0,   540,     0,     0,     0,  1461,   241,     0,
       0,     0,     0,   242,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    80,     0,     0,     0,     0,     0,
     515,     0,     0,   544,     0,     0,     0,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,    96,    97,   525,     0,     0,     0,     0,     0,    49,
      50,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   104,   544,     0,     0,    54,    55,     0,     0,
       0,    56,     0,   566,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   116,   117,   118,   119,
     120,   121,     0,     0,   567,     0,     0,   125,   126,   127,
     128,   129,   130,    68,    69,    70,     0,  -171,     0,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,     0,   132,   677,     0,   134,   135,   136,   137,
     138,   139,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   244,   245,   246,   146,     0,     0,     0,     0,     0,
       0,   141,     0,     0,   525,     0,   147,   148,     0,    80,
       0,   149,     0,     0,     0,  1403,   150,     0,     0,     0,
       0,     0,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,   629,     0,     0,    95,    96,    97,     0,   224,
     225,   226,     5,     0,   227,   228,     8,   229,   230,   231,
     232,   233,   234,   235,     0,     0,     0,   104,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    24,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   116,   117,   118,   119,   120,   121,     0,     0,     0,
     191,     0,   125,   126,   127,   128,   129,   130,     0,     0,
       0,     0,     0,     0,     0,     0,   544,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   134,   135,   136,   137,   138,   139,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   244,   245,   246,   146,
       0,     0,     0,     0,     0,     0,     0,     0,   544,     0,
       0,   147,   148,    49,    50,     0,   149,     0,     0,     0,
       0,   150,     0,     0,     0,     0,     0,     0,     0,     0,
      54,    55,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   224,   225,   226,
       5,     0,   227,   228,     8,   229,   230,   231,   232,   233,
     234,   235,     0,     0,     0,     0,     0,    68,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    24,
       0,     0,     0,     0,     0,   545,   767,   546,   547,   548,
     549,   550,   551,   552,   553,   554,   555,   556,   557,   558,
       0,     0,   559,   525,     0,   526,  1462,   527,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   538,   539,
       0,     0,   540,   243,     0,   545,  1463,   546,   547,   548,
     549,   550,   551,   552,   553,   554,   555,   556,   557,   558,
       0,     0,   559,     0,     0,     0,  1464,   525,     0,    95,
      96,    97,     0,     0,     0,     0,     0,     0,     0,   682,
       0,    49,    50,     0,     0,     0,     0,     0,     0,     0,
       0,   104,     0,     0,     0,     0,     0,     0,    54,    55,
       0,   768,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   116,   117,   118,   119,   120,
     121,     0,     0,     0,     0,     0,   125,   126,   127,   128,
     129,   130,     0,     0,     0,    68,  -171,    70,   527,   528,
     529,   530,   531,   532,   533,   534,   535,   536,   537,   538,
     539,     0,     0,   540,     0,   134,   135,   136,   137,   138,
     139,   544,     0,     0,     0,     0,     0,     0,     0,     0,
     244,   245,   246,   146,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   147,   247,     0,     0,     0,
     248,   243,     0,     0,     0,   249,   224,   225,   226,     5,
       0,   227,   228,     8,   229,   230,   231,   232,   233,   234,
     235,     0,     0,     0,     0,     0,     0,    95,    96,    97,
       0,     0,     0,     0,     0,     0,     0,     0,    24,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   545,   104,
     546,   547,   548,   549,   550,   551,   552,   553,   554,   555,
     556,   557,   558,   202,     0,   559,     0,     0,     0,  1484,
       0,     0,     0,   116,   117,   118,   119,   120,   121,     0,
       0,     0,     0,     0,   125,   126,   127,   128,   129,   130,
     545,     0,   546,   547,   548,   549,   550,   551,   552,   553,
     554,   555,   556,   557,   558,     0,     0,   559,     0,     0,
       0,  1497,     0,   134,   135,   136,   137,   138,   139,     0,
      49,    50,     0,     0,     0,     0,     0,     0,   244,   245,
     246,   146,     0,     0,     0,     0,     0,    54,    55,     0,
       0,     0,    56,   147,   247,     0,     0,     0,   248,     0,
       0,     0,     0,   249,   224,   225,   226,     5,     0,   227,
     228,     8,   229,   230,   231,   232,   233,   234,   235,     0,
       0,     0,     0,     0,    68,     0,    70,     0,     0,     0,
       0,     0,  1302,     0,     0,   526,    24,   664,   665,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
       0,     0,   677,     0,     0,     0,  1459,     0,     0,     0,
     274,     0,   525,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   526,
     243,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,     0,     0,   677,     0,     0,     0,
    1461,     0,     0,     0,     0,     0,    95,    96,    97,   544,
       0,     0,     0,     0,     0,     0,     0,     0,    49,    50,
       0,     0,     0,     0,     0,     0,     0,     0,   104,     0,
       0,     0,     0,     0,     0,    54,    55,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   116,   117,   118,   119,   120,   121,     0,     0,
       0,     0,     0,   125,   126,   127,   128,   129,   130,     0,
       0,     0,    68,  -171,    70,   546,   547,   548,   549,   550,
     551,   552,   553,   554,   555,   556,   557,   558,     0,     0,
     559,     0,   134,   135,   136,   137,   138,   139,   525,     0,
       0,     0,     0,     0,     0,     0,     0,   244,   245,   246,
     146,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   147,   247,     0,     0,     0,   248,   243,     0,
       0,     0,   249,     0,     0,     0,   224,   225,   226,     5,
       0,   227,   228,     8,   229,   230,   231,   232,   233,   234,
     235,     0,     0,     0,    95,    96,    97,     0,     0,     0,
       0,     0,     0,     0,     0,  1303,     0,     0,    24,     0,
       0,     0,     0,     0,     0,     0,   104,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1453,     0,     0,     0,     0,
     116,   117,   118,   119,   120,   121,     0,     0,     0,     0,
       0,   125,   126,   127,   128,   129,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     134,   135,   136,   137,   138,   139,     0,     0,     0,     0,
      49,    50,     0,     0,     0,   244,   245,   246,   146,     0,
       0,     0,     0,     0,     0,     0,     0,    54,    55,     0,
     147,   247,    56,     0,     0,   248,     0,     0,     0,     0,
     249,   224,   225,   226,     5,     0,   227,   228,     8,   229,
     230,   231,   232,   233,   234,   235,     0,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    70,     0,     0,     0,
       0,     0,     0,    24,   526,     0,   527,   528,   529,   530,
     531,   532,   533,   534,   535,   536,   537,   538,   539,     0,
       0,   540,   191,     0,     0,  1561,     0,   544,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   525,
     243,   545,  1454,   546,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,   557,   558,     0,     0,   559,     0,
       0,     0,  1562,     0,   525,     0,    95,    96,    97,     0,
       0,     0,     0,     0,     0,    49,    50,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,     0,
       0,     0,    54,    55,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   116,   117,   118,   119,   120,   121,     0,     0,
       0,     0,     0,   125,   126,   127,   128,   129,   130,    68,
     526,    70,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,   539,     0,     0,   540,     0,     0,
       0,  1563,   134,   135,   136,   137,   138,   139,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   244,   245,   246,
     146,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   147,   247,     0,   243,     0,   248,     0,     0,
       0,     0,   249,   224,   225,   226,     5,     0,   227,   228,
       8,   229,   230,   231,   232,   233,   234,   235,     0,     0,
       0,    95,    96,    97,     0,     0,     0,     0,   525,     0,
       0,     0,     0,     0,     0,    24,     0,     0,     0,     0,
       0,     0,     0,   104,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   116,   117,   118,
     119,   120,   121,     0,     0,     0,     0,     0,   125,   126,
     127,   128,   129,   130,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   134,   135,   136,
     137,   138,   139,     0,     0,     0,     0,    49,    50,     0,
       0,     0,   244,   245,   246,   146,     0,     0,     0,     0,
       0,     0,     0,     0,    54,    55,     0,   147,   247,    56,
       0,     0,   248,     0,     0,     0,     0,   249,   224,   225,
     226,     5,     0,   227,   228,     8,   229,   230,   231,   232,
     233,   234,   235,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,    70,     0,     0,     0,     0,     0,   545,
      24,   546,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,     0,     0,   559,     0,     0,   191,
    1564,   526,     0,   664,   665,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,     0,     0,   677,     0,
       0,     0,  1561,     0,     0,     0,   526,   243,   664,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,   544,     0,   677,     0,     0,     0,  1563,     0,     0,
       0,     0,     0,    95,    96,    97,     0,     0,     0,     0,
       0,     0,    49,    50,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   525,     0,   104,     0,     0,     0,    54,
      55,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   116,
     117,   118,   119,   120,   121,     0,     0,   450,     0,     0,
     125,   126,   127,   128,   129,   130,    68,     0,    70,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   134,
     135,   136,   137,   138,   139,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   244,   245,   246,   146,     0,     0,
       0,     0,     0,     0,   525,     0,     0,     0,     0,   147,
     247,     0,   243,     0,   248,     0,     0,     0,     0,   249,
     526,     0,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,   539,     0,     0,   540,    95,    96,
      97,  1612,   224,   225,   226,     5,     0,   227,   228,     8,
     229,   230,   231,   232,   233,   234,   235,     0,     0,     0,
     104,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   116,   117,   118,   119,   120,   121,
       0,     0,     0,     0,     0,   125,   126,   127,   128,   129,
     130,     0,     0,     0,     0,     0,   544,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   134,   135,   136,   137,   138,   139,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   244,
     245,   246,   146,     0,     0,     0,   368,     0,   525,     0,
       0,     0,     0,     0,   147,   247,    49,    50,     0,   248,
       0,     0,     0,     0,   637,     0,     0,     0,     0,     0,
       0,     0,     0,    54,    55,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,   224,   225,   226,
       5,     0,   227,   228,     8,   229,   230,   231,   232,   233,
     234,   235,     0,     0,     0,     0,     0,     0,     0,     0,
      68,     0,    70,     0,     0,     0,     0,     0,     0,    24,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   545,     0,   546,   547,   548,   549,   550,
     551,   552,   553,   554,   555,   556,   557,   558,   525,     0,
     559,     0,     0,     0,  1613,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   526,   243,   664,   665,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
       0,     0,   677,     0,     0,     0,  1612,     0,     0,     0,
     525,     0,    95,    96,    97,     0,     0,     0,     0,     0,
       0,    49,    50,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   104,     0,     0,     0,    54,    55,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   116,   117,
     118,   119,   120,   121,     0,     0,     0,     0,     0,   125,
     126,   127,   128,   129,   130,    68,   526,    70,   527,   528,
     529,   530,   531,   532,   533,   534,   535,   536,   537,   538,
     539,     0,     0,   540,     0,     0,     0,  1640,   134,   135,
     136,   137,   138,   139,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   244,   245,   246,   146,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   147,   247,
       0,   243,     0,   248,     0,     0,     0,     0,   249,   224,
     225,   226,     5,     0,   227,   228,     8,   229,   230,   231,
     232,   233,   234,   235,     0,     0,     0,    95,    96,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    24,     0,     0,     0,     0,     0,     0,   545,   104,
     546,   547,   548,   549,   550,   551,   552,   553,   554,   555,
     556,   557,   558,  1315,     0,   559,     0,     0,     0,  1641,
       0,     0,     0,   116,   117,   118,   119,   120,   121,     0,
       0,     0,     0,     0,   125,   126,   127,   128,   129,   130,
     526,     0,   664,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,     0,     0,   677,     0,     0,
       0,  1640,     0,   134,   135,   136,   137,   138,   139,     0,
       0,     0,     0,    49,    50,     0,     0,     0,   244,   245,
     246,   146,     0,     0,     0,     0,     0,     0,     0,     0,
      54,    55,     0,   147,   247,    56,     0,     0,   248,     0,
       0,     0,     0,   249,   224,   225,   226,     5,     0,   227,
     228,     8,   229,   230,   231,   232,   233,   234,   235,     0,
       0,     0,     0,     0,     0,     0,     0,    68,     0,    70,
       0,     0,     0,     0,     0,     0,    24,     0,     0,     0,
     526,     0,   664,   665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,     0,     0,   677,     0,     0,
     544,  1657,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     525,     0,   526,   243,   527,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,   539,     0,     0,   540,
       0,     0,     0,  1658,     0,     0,     0,     0,     0,    95,
      96,    97,     0,     0,     0,     0,     0,     0,    49,    50,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   104,     0,     0,     0,    54,    55,     0,     0,     0,
      56,     0,     0,     0,     0,  1317,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   116,   117,   118,   119,   120,
     121,     0,     0,     0,     0,     0,   125,   126,   127,   128,
     129,   130,    68,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   134,   135,   136,   137,   138,
     139,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     244,   245,   246,   146,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   147,   247,     0,   243,     0,
     248,     0,     0,     0,     0,   249,   224,   225,   226,     5,
       0,   227,   228,     8,   229,   230,   231,   232,   233,   234,
     235,     0,     0,     0,    95,    96,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    24,     0,
       0,     0,     0,     0,     0,     0,   104,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1348,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     116,   117,   118,   119,   120,   121,     0,     0,     0,     0,
       0,   125,   126,   127,   128,   129,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     134,   135,   136,   137,   138,   139,     0,     0,     0,     0,
      49,    50,     0,     0,     0,   244,   245,   246,   146,     0,
       0,     0,     0,     0,     0,     0,     0,    54,    55,     0,
     147,   247,    56,     0,     0,   248,     0,     0,     0,     0,
     249,   224,   225,   226,     5,     0,   227,   228,     8,   229,
     230,   231,   232,   233,   234,   235,     0,     0,     0,     0,
       0,   571,     0,     0,    68,     0,    70,   572,     0,     0,
       0,     0,   545,    24,   546,   547,   548,   549,   550,   551,
     552,   553,   554,   555,   556,   557,   558,   202,   573,   559,
       0,     0,   526,  1659,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,     0,     0,   677,
       0,     0,     0,  1658,     0,     0,     0,   574,   575,     0,
     243,     0,   576,     0,     0,     0,     0,   577,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    95,    96,    97,     0,
       0,     0,     0,   578,     0,    49,    50,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,     0,
       0,     0,    54,    55,     0,     0,     0,    56,   579,   580,
       0,   581,     0,   238,   239,   582,     0,     0,     0,     0,
       0,     0,   116,   117,   118,   119,   120,   121,     0,     0,
       0,     0,     0,   125,   126,   127,   128,   129,   130,    68,
       0,    70,     0,     0,     0,     0,     0,     0,     0,     0,
     241,     0,     0,     0,     0,   242,     0,     0,     0,     0,
       0,     0,   134,   135,   136,   137,   138,   139,     0,     0,
       0,   583,     0,     0,     0,     0,     0,   244,   245,   246,
     146,   571,     0,     0,     0,     0,     0,   572,     0,     0,
       0,     0,   147,   247,     0,   243,     0,   248,     0,     0,
       0,     0,   249,     0,     0,     0,     0,   202,   573,     0,
       0,     0,     0,     0,     0,     0,   584,     0,     0,     0,
       0,    95,    96,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   585,     0,   574,   575,     0,
       0,     0,   576,   104,     0,     0,   586,   577,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   116,   117,   118,
     119,   120,   121,   578,     0,     0,     0,     0,   125,   126,
     127,   128,   129,   130,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   587,     0,     0,   579,   580,
       0,   581,     0,   238,   239,   582,     0,   134,   135,   136,
     137,   138,   139,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   244,   245,   246,   763,     0,     0,     0,     0,
       0,     0,     0,     0,   588,     0,     0,   147,   247,     0,
     241,     0,   248,     0,     0,   242,     0,   249,     0,   312,
       0,     0,     0,   313,   314,   315,   316,   317,   318,     0,
       0,   583,   319,   320,   321,   322,   323,   324,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     325,     0,     0,   326,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   327,     0,     0,     0,     0,   584,     0,     0,     0,
       0,     0,     0,     0,   328,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   585,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   586,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   587,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   340,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1224,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   341,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   342,   343
};

static const yytype_int16 yycheck[] =
{
       0,   293,   588,   167,    35,    81,    19,    20,   178,    69,
      62,   181,    49,   183,   184,    79,   186,   124,    46,   304,
      45,    75,    61,   104,   923,    75,   182,    27,    62,    46,
      66,    46,    34,    47,   323,   111,    62,    63,    44,    39,
      40,   122,   208,    75,   102,    52,    40,   213,    25,   375,
      27,   103,   124,   105,   125,    17,    33,   109,   110,    96,
     959,    46,    62,    40,    64,   158,   222,   329,    46,   158,
      47,    75,   398,   114,   218,    27,    60,    75,    69,   208,
     399,    33,   392,   124,   213,    62,   396,    64,    40,    69,
     122,    97,   354,   132,   133,    47,    22,    52,    98,   132,
     133,   392,    62,   103,   248,   394,   397,   392,    99,   109,
      62,    75,    64,    68,   167,   115,    19,    20,   155,   169,
     394,   171,   172,   123,   425,   426,   103,   428,   105,   165,
     394,   301,   109,   110,   215,   148,   208,   218,   394,   165,
    1039,   213,   142,   123,    69,   170,   394,   147,   148,   149,
     150,   135,   136,   394,   208,   162,   204,   167,   208,   213,
     394,    64,   117,   213,   236,   236,   128,    33,   222,   227,
     204,   169,   265,   171,   172,   304,   265,   102,   178,   179,
      69,   181,   182,   138,   354,   185,    75,   187,   188,   189,
     190,   168,    58,   395,   396,    98,   362,   394,   208,   212,
     202,   208,   209,   213,   206,   108,   213,   171,   172,   248,
     208,   132,   133,   145,   208,   213,   168,   253,   222,   213,
     204,     0,    27,   237,   179,   180,   181,   253,    33,   300,
     269,   270,   304,   122,   394,    40,   392,   221,    69,   239,
      19,    62,    47,    22,    75,   148,   206,   247,   248,   249,
     321,   279,    31,    51,   304,   280,   216,    62,   222,    64,
     237,   223,   279,   297,   279,   337,   169,   321,   208,    75,
     394,   226,    77,   213,   294,   230,   302,    75,   144,   293,
     146,   582,    61,   172,   332,   392,   313,   337,   295,   296,
     338,   122,    71,   214,   279,   322,   256,   304,   103,   165,
     105,   279,   227,   303,   109,   110,   304,   394,    87,   212,
     392,   392,   360,   147,   394,   397,   293,   115,    74,   303,
      99,   276,   101,   269,   270,   177,   178,   106,   107,   284,
     285,   345,   392,   167,  1233,   399,   320,   219,   246,   337,
     304,   172,   224,   122,   394,   124,    75,   279,   269,   270,
     395,   396,   366,   373,   206,    75,   356,    22,    69,   359,
     373,   392,    67,   168,   394,   231,   232,   387,   345,    99,
      75,   371,   542,   171,   172,   394,   376,   377,   310,   282,
     102,   397,   337,   394,   273,   206,   289,   253,   129,   366,
     117,   394,    22,   245,   301,   216,   392,   394,   204,   399,
     127,   397,   594,   399,   596,   597,   406,   394,   394,   396,
     208,   332,   412,   394,   366,   213,   222,   122,    59,   394,
     420,    62,   596,   597,   222,   394,   172,   183,   394,   429,
     430,    18,   432,   394,   604,   455,   163,   394,   394,   360,
     394,   441,   273,   309,     0,   311,   399,   132,   133,   399,
     450,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   463,   393,   391,   171,   172,   399,   225,
      72,   641,   394,   643,   394,   645,   394,    75,   392,   479,
      22,   301,   394,   394,   204,   394,   208,   487,   488,   489,
     490,   213,   386,   387,   388,   329,   330,   391,   498,   499,
     500,   501,   222,   394,   394,   227,   304,   234,   394,   288,
     586,   511,   512,   513,   394,   394,   394,   222,   394,   571,
     354,   355,   356,   357,   358,   359,   526,   527,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   538,   539,
     540,   392,   594,   392,   596,   545,   546,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,   366,   576,   577,    71,   206,   271,   581,   392,   171,
     172,   585,   213,   171,   172,   216,   301,   273,   273,   358,
     397,   360,   361,   583,   269,   270,   264,   587,   293,   273,
     115,   386,   387,   388,   571,   595,   391,   301,   301,   576,
     577,   273,   377,   603,   581,   384,   208,   273,   585,   391,
     208,   213,   167,   394,   121,   213,   593,   594,   391,   596,
     597,   391,    22,   402,   576,   132,   133,   627,   399,   581,
     304,   140,   367,   585,   386,   387,   388,   637,  1224,   391,
     395,   593,   642,   396,   644,   395,   646,   647,   648,   649,
     273,   396,   396,   396,   433,    22,   396,   396,   395,   438,
     439,   220,   398,   396,   664,   665,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,   677,   678,   395,
     301,   681,   682,   396,   396,   301,   395,   687,   396,   396,
     690,   855,   372,   395,   986,   396,   395,   323,   396,   699,
     273,   208,   396,   147,   718,   396,   213,   396,   273,   374,
     273,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   724,   396,   391,   506,   372,   372,
     395,   396,   396,   396,   734,   396,   736,   737,   738,   396,
     396,   718,   742,    64,   374,   524,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   396,
      75,   391,   269,   398,    22,   394,   396,   767,   768,   129,
     297,   576,   396,   297,   396,   395,   581,   396,   396,   396,
     585,   396,   395,   395,   395,   395,   395,   108,   593,   594,
     396,   596,   597,   398,   395,   395,    27,    67,   395,    69,
     115,   396,   802,   396,   396,    75,   396,   807,   396,    40,
     810,   132,   133,   813,    45,   396,    47,   854,   395,   395,
     820,   396,   601,   602,   396,   269,   270,   396,   396,   829,
     396,    62,   374,    64,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   395,   169,   391,
     395,   630,   122,    22,   396,   395,   171,   172,   395,   569,
     396,   396,   395,    34,   396,   396,  1158,   395,   508,    41,
     314,   315,   103,   317,   318,   524,  1025,    -1,   109,   856,
      -1,   325,   326,   327,   328,    -1,    22,    -1,   888,    -1,
     160,   329,   330,   208,   164,    67,    -1,    -1,   213,    -1,
      -1,   680,   172,    75,   856,    -1,    -1,   222,    -1,   946,
      -1,    -1,    -1,    -1,    -1,    -1,   354,   355,   356,   357,
     358,   359,   922,    -1,   924,    -1,   926,  1097,    -1,    -1,
      -1,  1101,    -1,    -1,    -1,    -1,   167,   168,    -1,   170,
      -1,    -1,    22,    -1,    -1,   215,   177,    -1,   269,   270,
     122,  1003,   124,    -1,    -1,    -1,    -1,    -1,   228,    -1,
     960,   192,    -1,    -1,    -1,    -1,   197,    -1,   289,    -1,
     749,    -1,    -1,   973,    -1,    -1,   976,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
    1004,   391,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,  1005,    -1,   374,    -1,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,   975,   391,    -1,  1026,  1004,    -1,   396,
     809,    -1,    -1,    -1,    -1,    -1,   208,   209,    -1,  1039,
      22,   213,  1042,    -1,    -1,    -1,    -1,    -1,    -1,  1049,
    1050,   856,    -1,    -1,    -1,    -1,  1226,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   236,   237,  1066,    -1,    -1,  1069,
    1070,  1071,    -1,    -1,  1074,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   255,    -1,    -1,    -1,    -1,    -1,  1066,
      22,    -1,  1069,    -1,    -1,    -1,    -1,    -1,  1098,  1099,
    1100,    -1,  1102,  1103,  1104,  1105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1066,   346,   374,  1069,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,   304,   391,    -1,   366,    -1,    -1,   396,   370,
       0,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,  1154,  1155,    -1,  1157,    -1,    -1,
      -1,    -1,    -1,   394,    -1,    -1,    -1,    -1,    -1,    -1,
     975,    -1,    -1,    -1,  1174,    -1,    -1,  1177,    -1,  1179,
      -1,  1181,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1190,  1191,    -1,    -1,  1194,    -1,    -1,    -1,    -1,  1199,
      -1,    -1,    -1,    -1,  1374,   374,    -1,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
      -1,    -1,   391,    -1,    -1,  1225,    -1,   396,  1228,  1229,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   374,  1239,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,    -1,    -1,   391,    -1,    -1,    -1,    -1,
     396,  1066,    -1,    -1,  1069,    -1,    -1,    -1,    -1,    -1,
      -1,  1271,    -1,   504,    -1,    -1,   507,    -1,    -1,   510,
    1280,  1281,    -1,    -1,    -1,    -1,    -1,    -1,   148,   149,
     150,    -1,  1292,    -1,   374,  1295,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      -1,   391,    -1,    -1,    -1,    -1,   396,    -1,    -1,    -1,
      -1,   181,   182,    -1,    -1,   185,  1326,    -1,    -1,    -1,
      -1,    -1,   563,   564,   565,   566,  1336,    -1,   569,    -1,
     571,   572,    -1,    -1,   575,   576,    -1,   578,    -1,    22,
      -1,   582,  1352,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   593,    -1,    -1,   596,   597,    -1,    -1,    -1,
      22,    -1,  1372,  1373,    -1,  1375,  1376,  1377,  1378,  1379,
      -1,    -1,    -1,    -1,    -1,  1385,  1165,    -1,    -1,    -1,
      -1,  1391,  1392,    -1,    22,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,  1407,  1408,   391,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1418,  1419,
      22,  1421,    -1,  1423,    -1,    -1,    -1,    -1,  1428,    -1,
      -1,  1431,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1441,    -1,  1222,    -1,  1445,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    22,   391,
     691,    -1,    -1,    -1,    -1,  1465,  1466,  1467,  1468,  1469,
    1470,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,   718,    -1,    -1,
      -1,    -1,    -1,    -1,  1494,    -1,    -1,    -1,  1498,   359,
     374,    -1,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,
      -1,   752,   396,    -1,    -1,    22,  1526,  1527,  1528,  1529,
    1530,  1531,    -1,  1533,    -1,    -1,    -1,    -1,    -1,  1539,
      16,    22,    -1,    -1,    -1,    -1,    -1,    -1,  1548,  1549,
      -1,    -1,    28,    -1,    30,    -1,    -1,    -1,    -1,    35,
      36,    -1,    -1,    39,    -1,  1565,  1566,  1567,  1568,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,
      -1,   441,    -1,  1583,    -1,  1585,    -1,    -1,    -1,    -1,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1598,  1599,
    1600,    -1,    22,    79,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1614,  1615,  1616,    -1,    94,    -1,
      -1,    -1,    -1,  1623,   855,   856,    22,    -1,    -1,   105,
      -1,  1631,  1632,    -1,   110,    -1,    -1,   113,    -1,  1639,
      -1,    -1,  1642,    -1,   296,    -1,    -1,    22,  1648,    -1,
      -1,    -1,    -1,    -1,  1654,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1662,    -1,   524,    -1,  1666,   527,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   538,   539,
     540,    22,    -1,    -1,    -1,   545,   546,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,   374,   178,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,    22,   193,   391,    -1,
      -1,    -1,   374,   396,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    -1,   391,
      -1,    -1,    -1,   603,   975,    -1,   374,   223,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,    -1,   391,    -1,    -1,    -1,    -1,   396,    -1,
      -1,  1002,   374,    -1,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    -1,   391,
      -1,    -1,    -1,    -1,   396,    -1,    -1,   273,    -1,   275,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   285,
     374,    -1,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,   302,   391,   304,    74,
    1061,    -1,   396,    -1,  1065,  1066,    22,   374,  1069,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,    -1,   391,  1086,    -1,   102,    -1,   396,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     356,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   368,   374,   391,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,    -1,
     391,    -1,    22,    -1,  1145,   396,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   401,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   419,    -1,    -1,    -1,    22,   424,   425,
     426,   196,   428,   429,   374,    -1,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      -1,   391,    -1,    -1,    -1,    -1,   396,   453,   374,    -1,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   238,    -1,   391,   472,    -1,    -1,   374,
     396,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,    -1,
      22,   396,  1253,    -1,    75,    -1,    -1,    -1,   273,   274,
      -1,    -1,    -1,   374,    -1,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,    -1,
     391,    -1,    -1,    -1,    -1,   396,    -1,    -1,    -1,    -1,
     305,    22,    -1,    -1,    -1,    -1,    -1,    -1,   374,    -1,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,    -1,    -1,   391,    -1,    -1,    22,    -1,
     396,    -1,    -1,    -1,  1325,    -1,    -1,   573,    -1,    -1,
      -1,    -1,   347,   579,  1335,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1346,   167,    -1,   594,    -1,
     171,   172,    -1,   368,   369,    -1,    -1,   372,    -1,   374,
     375,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     385,    -1,    -1,    -1,    22,    -1,    -1,    -1,   148,   149,
     150,   627,   397,   398,   399,    -1,    -1,   208,    -1,    -1,
      -1,   406,   213,    -1,    -1,   410,    -1,    -1,   413,    -1,
      -1,   222,    -1,    -1,    -1,    -1,    -1,   228,   423,    -1,
      -1,   181,   182,    -1,    -1,   185,    -1,    -1,   374,    -1,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   254,   449,   391,    -1,    -1,    -1,   454,
     396,   456,    -1,    -1,    -1,    -1,   692,    -1,    22,    -1,
      -1,    -1,   273,    -1,    -1,    -1,    -1,    -1,   473,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1098,  1099,
    1100,    -1,    -1,    -1,    -1,    -1,    -1,   492,   493,   494,
     495,    -1,  1483,   304,   730,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   374,  1496,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    22,
      -1,   391,    -1,    -1,    -1,    -1,   396,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1154,  1155,   772,  1157,   774,   374,
     776,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,    -1,    -1,   391,  1177,    -1,    -1,
      -1,   396,    -1,    -1,    -1,    -1,    -1,    22,    -1,   574,
      -1,    -1,   577,    -1,  1194,   580,    -1,    -1,    -1,   584,
     585,    -1,    -1,    -1,    -1,    -1,  1577,  1578,    -1,    -1,
      -1,   596,   597,    -1,   830,   831,    -1,    -1,  1589,   359,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1228,  1229,
      -1,     0,   374,    -1,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    -1,   391,
      19,    -1,   637,    22,   396,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,   652,    -1,    -1,
      -1,    40,    -1,  1644,    -1,    -1,    45,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,  1660,
     391,    -1,    61,    -1,  1665,    64,    -1,    -1,    -1,  1670,
      -1,   441,    71,   688,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,    -1,   391,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,    -1,   101,    -1,   103,    22,    -1,   106,   107,    -1,
      -1,   957,    -1,    -1,    -1,    -1,   962,    -1,    -1,    -1,
      -1,    -1,    -1,   122,   739,   124,   374,    22,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,    -1,   391,    -1,    -1,    -1,    -1,   396,    -1,
      -1,    -1,    -1,   999,    22,    -1,    -1,   527,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   538,   539,
     540,    -1,    -1,    -1,    -1,   545,   546,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,  1037,    22,    -1,  1040,    -1,    -1,    -1,    -1,  1045,
     374,    -1,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,
      -1,    -1,   396,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   603,    -1,  1465,  1466,   374,    -1,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    22,    -1,   391,    -1,    -1,    -1,    -1,   396,
      -1,   374,    -1,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,    -1,    -1,   391,    -1,
      -1,    -1,    -1,   396,    -1,    -1,    -1,    -1,    -1,   288,
      -1,    22,    -1,    -1,    -1,    -1,    -1,  1143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   374,
     925,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,    -1,    -1,   391,  1173,    22,    -1,
      -1,   396,    -1,    -1,    -1,  1565,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   961,    -1,   963,   964,
      -1,    22,    -1,   968,    -1,    -1,    -1,    -1,    -1,   358,
      -1,   360,   361,    -1,    -1,    -1,    -1,    -1,    -1,   984,
     985,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   993,    -1,
      -1,    -1,    -1,    -1,  1614,   384,    -1,    -1,  1003,  1004,
      -1,  1006,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   402,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,  1642,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1036,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1044,
      -1,    -1,  1047,  1048,   433,    -1,    -1,    -1,    -1,   438,
     439,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1063,    -1,
      -1,  1066,    -1,    -1,  1069,    -1,    -1,    22,    -1,    -1,
    1075,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,    -1,   391,  1321,  1091,    -1,    -1,   374,
      -1,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,  1109,    -1,   391,    -1,    -1,    -1,
      -1,   396,    -1,    -1,    -1,    -1,   374,   506,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,    -1,   391,    -1,   524,    -1,    -1,   396,    -1,
      -1,    -1,    -1,  1148,  1149,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   374,    -1,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
    1406,   391,  1177,  1409,    -1,  1180,   396,    -1,  1183,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,   374,  1193,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,    -1,   391,    22,    -1,    -1,    -1,   396,
      -1,    -1,   601,   602,   374,    -1,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      -1,   391,    -1,    -1,    -1,    -1,   396,    -1,    -1,    -1,
      -1,   630,  1247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   374,  1490,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,    -1,
     391,    -1,    -1,    -1,    -1,   396,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     374,   680,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,
      -1,  1547,   396,   374,    -1,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,    -1,
     391,    -1,  1337,    -1,    -1,   396,    -1,    -1,  1098,  1099,
    1100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1354,
    1355,    -1,    -1,    -1,    -1,   744,    -1,    -1,    -1,    -1,
     749,    -1,    -1,    -1,    -1,    -1,  1371,   374,    -1,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,    -1,   391,    -1,    -1,    -1,  1393,   396,
      -1,    -1,    -1,    -1,  1154,  1155,    -1,  1157,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1647,    -1,    -1,    -1,    -1,    -1,  1177,    -1,   374,
     809,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,  1194,    -1,   391,    -1,    -1,    -1,
      -1,   396,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1457,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,  1228,  1229,
      -1,    -1,    -1,    -1,    -1,  1480,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1491,    -1,  1493,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   374,  1503,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,    -1,   391,    -1,    -1,    -1,    -1,   396,
    1525,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   374,  1540,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      -1,   391,    -1,    -1,    -1,  1560,   396,   374,    -1,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,    -1,   391,    -1,    -1,    -1,    -1,   396,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     0,     1,  1603,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1611,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    35,    -1,    37,    38,    -1,    -1,    41,    42,    -1,
      -1,    -1,    46,    47,    48,    -1,    50,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,
      64,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    80,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,   100,   101,    -1,    -1,
      -1,    -1,    -1,   107,   108,  1465,  1466,   111,    -1,   113,
      -1,    -1,   116,    -1,    -1,   119,   120,    -1,    -1,    -1,
     124,    -1,   126,    -1,    -1,    -1,    -1,   131,   132,   133,
      -1,    -1,    -1,   137,    -1,   139,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   150,    -1,    -1,    -1,
     154,   155,   156,   157,    -1,   159,    -1,   161,    -1,    -1,
      -1,    -1,   166,    -1,   168,   169,    -1,    -1,    -1,   173,
     174,    -1,    -1,    -1,    -1,    -1,  1165,    -1,    -1,    -1,
      -1,   185,   186,   187,   188,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,    -1,   203,
      -1,   205,    -1,    -1,    -1,  1565,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   374,   218,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,   233,
     391,    -1,    -1,  1222,   238,   239,   240,   241,   242,   243,
      -1,    -1,   246,   247,   248,   249,    -1,   251,   252,    -1,
      -1,    -1,    -1,   257,  1614,   259,   260,   261,   262,   263,
     264,    -1,    -1,    -1,   268,   269,   270,    -1,    -1,    -1,
      -1,   275,    -1,   277,   278,    -1,    -1,   281,   282,    -1,
      -1,    -1,  1642,    -1,    -1,   289,   290,   291,   292,    -1,
     294,    -1,    -1,    -1,   298,   299,   300,    -1,    -1,    -1,
     304,    -1,   306,   307,   308,    -1,    -1,    -1,    -1,   313,
     314,   315,   316,   317,   318,   319,    -1,   321,   322,    -1,
     324,   325,   326,   327,   328,   329,   330,    -1,   332,   333,
     334,    -1,   374,   375,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    -1,   391,
     354,   355,   356,   357,   358,   359,   360,   361,     0,     1,
      -1,    -1,    -1,    -1,   368,   369,   370,   371,   372,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,    22,
     384,   385,    -1,    -1,    -1,   389,    -1,    -1,    -1,   393,
     394,    -1,    -1,    35,    -1,    37,    38,    -1,    -1,    41,
      42,    -1,    22,    -1,    46,    47,    48,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,
      -1,    -1,    64,    65,    -1,    22,    -1,    -1,    70,    -1,
      -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,    80,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,
      -1,    -1,    -1,    -1,    -1,   107,   108,    -1,    -1,   111,
      -1,   113,    -1,    -1,   116,    -1,    -1,   119,   120,    -1,
      -1,    -1,   124,    -1,   126,    -1,    -1,    -1,    -1,   131,
     132,   133,    -1,    -1,    -1,   137,    -1,   139,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   155,   156,   157,    -1,   159,    -1,   161,
      22,    -1,    -1,    -1,   166,    -1,   168,   169,    -1,    -1,
      -1,   173,   174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   185,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,    -1,   391,    -1,   201,
      -1,   203,    -1,   205,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   233,    -1,    -1,    -1,    -1,   238,   239,   240,   241,
      -1,   243,    -1,    -1,   246,   247,   248,   249,    -1,   251,
     252,    -1,    -1,    -1,    -1,    -1,    -1,   259,   260,   261,
     262,   263,   264,    -1,    -1,    -1,    -1,   269,   270,    -1,
      -1,    -1,    -1,   275,    -1,   277,   278,    -1,    -1,   281,
     282,    -1,    -1,    -1,    -1,    -1,    -1,   289,    -1,   291,
     292,    -1,   294,    -1,    -1,    -1,   298,   299,   300,    -1,
      -1,    -1,   304,    -1,   306,   307,   308,    -1,    -1,    -1,
      -1,   313,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   321,
     322,    -1,   324,    -1,    -1,    -1,    -1,   329,   330,    -1,
     332,   333,   334,    18,    19,    20,    21,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
      -1,    -1,   354,   355,   356,   357,   358,   359,   360,   361,
      -1,    -1,    -1,    -1,    -1,    50,   368,   369,   370,   371,
      -1,   374,    -1,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,    -1,    -1,   391,    -1,
      -1,   393,    -1,   396,   374,    -1,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      -1,   391,    -1,    -1,    -1,    -1,   396,   374,    -1,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,    -1,   391,    -1,    -1,    -1,    -1,   396,
      22,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,    52,    53,    54,   154,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    19,
      20,    21,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   186,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    -1,   374,    -1,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    -1,   391,
      -1,    22,    -1,    -1,   396,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    82,    83,    84,    85,    86,    87,    -1,    -1,
      -1,    91,    92,    93,    94,    95,    96,   242,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,   268,   269,   270,    -1,    -1,    -1,    -1,
     130,    -1,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,   290,    -1,    -1,    -1,   149,
     150,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   314,
     315,   316,   317,   318,   319,    -1,    -1,    -1,    -1,    -1,
     325,   326,   327,   328,   329,   330,   186,    -1,   188,    -1,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,   354,
     355,   356,   357,   358,   359,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   369,   370,   371,   372,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   235,    -1,    -1,    -1,   384,
     385,    -1,   242,    -1,   389,    -1,    -1,    -1,    -1,   394,
      -1,    -1,    -1,   398,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,   266,    -1,   268,   269,
     270,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     290,   337,    -1,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,    -1,    22,
      -1,    -1,    -1,    -1,   314,   315,   316,   317,   318,   319,
      -1,    -1,    -1,    -1,    -1,   325,   326,   327,   328,   329,
     330,    -1,   374,    -1,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    -1,   391,
      -1,    -1,    -1,   395,   354,   355,   356,   357,   358,   359,
      -1,    -1,    -1,    -1,    -1,   365,   366,    -1,    -1,   369,
     370,   371,   372,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   384,   385,    -1,    -1,    -1,   389,
      18,    19,    20,    21,   394,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    39,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    50,   374,    -1,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,    67,
     391,    -1,    22,    -1,    -1,   396,    -1,    75,    -1,    -1,
     374,    -1,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,
      -1,   374,   396,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,    22,    -1,   391,    -1,
      -1,    -1,    -1,   396,   122,    -1,   124,   125,    -1,    -1,
      -1,    -1,    -1,    -1,   132,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,    22,    -1,    -1,   154,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,   374,
     188,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,    -1,
     208,   209,    -1,    -1,    -1,   213,    -1,   215,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     228,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   236,   237,
      -1,    -1,    -1,   374,   242,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,    -1,
     391,    -1,    -1,    -1,    -1,   396,    -1,    -1,    -1,    -1,
     268,   269,   270,    -1,   272,    -1,   274,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   286,   287,
     288,   374,   290,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   304,    -1,   391,    -1,
      -1,    -1,    -1,   396,    -1,    -1,   314,   315,   316,   317,
     318,   319,    -1,    -1,    -1,    -1,    -1,   325,   326,   327,
     328,   329,   330,    18,    19,    20,    21,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
      -1,    36,    -1,    -1,    -1,    -1,   354,   355,   356,   357,
     358,   359,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,   369,   370,   371,   372,    -1,    -1,    62,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   384,   385,    -1,    -1,
      75,   389,    -1,    -1,    -1,    -1,   394,    -1,    -1,    -1,
      -1,    -1,    -1,   374,    89,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    22,    -1,
     391,    -1,    -1,    -1,   374,   396,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    22,
      -1,   391,    -1,    -1,    -1,    -1,   396,   132,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,   149,   150,   151,    -1,   374,   154,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,    -1,    -1,   391,    -1,   172,    -1,    -1,
     396,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,
      -1,   186,    -1,   188,   189,   374,    -1,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
      -1,    -1,   391,   208,    22,   210,   211,   396,   213,    -1,
     215,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   228,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   242,    -1,    -1,
      -1,    18,    19,    20,    21,    -1,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   268,   269,   270,    -1,    -1,   273,    -1,
      -1,    -1,    -1,    50,   279,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   290,    -1,    64,    -1,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,   304,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   314,
     315,   316,   317,   318,   319,    -1,    -1,    -1,    -1,    -1,
     325,   326,   327,   328,   329,   330,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,   354,
     355,   356,   357,   358,   359,   132,   133,    -1,    -1,    -1,
      22,    -1,    -1,   140,   369,   370,   371,   372,    -1,    -1,
      -1,    -1,   149,   150,    -1,    -1,    -1,   154,    -1,   384,
     385,   158,    -1,    -1,   389,    -1,    -1,   164,    -1,   394,
     167,    -1,    -1,    -1,   171,   172,    -1,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,
     374,   188,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,
      -1,   208,   396,    22,    -1,    -1,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     374,    -1,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   242,    -1,   391,    -1,    -1,
      -1,   374,   396,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,    -1,    -1,   391,    -1,
      -1,   268,   269,   270,   374,   375,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      -1,   391,   289,   290,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   312,    -1,   314,   315,   316,
     317,   318,   319,    -1,    22,    -1,    50,    51,   325,   326,
     327,   328,   329,   330,   331,    -1,   374,   375,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,    -1,   391,    -1,    -1,    -1,   354,   355,   356,
     357,   358,   359,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,   369,   370,   371,   372,    -1,    -1,    -1,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   384,   385,    -1,
      -1,    -1,   389,    -1,    -1,    -1,    -1,   394,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,   149,   150,   151,    -1,   153,
     154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   186,   374,   188,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,    -1,
     391,    -1,    -1,    -1,    -1,   396,    -1,    -1,    -1,    -1,
      -1,    -1,   374,   217,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    22,   391,
      -1,    -1,    -1,    -1,   396,    -1,    -1,    -1,   242,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   250,    -1,    -1,   253,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   267,   268,   269,   270,    -1,    -1,   273,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   374,   290,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
      -1,   305,   391,    -1,    -1,    -1,    -1,   396,    -1,    -1,
     314,   315,   316,   317,   318,   319,    -1,    -1,    -1,    -1,
      -1,   325,   326,   327,   328,   329,   330,    18,    19,    20,
      21,    -1,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     354,   355,   356,   357,   358,   359,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,   369,   370,   371,   372,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,
     384,   385,    -1,    -1,    75,   389,    -1,    -1,    -1,    -1,
     394,    -1,    22,    -1,    -1,    -1,   374,    88,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,    -1,   391,    -1,    -1,    -1,    -1,   396,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,   132,   133,   374,    -1,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   149,   150,
     391,    -1,    -1,   154,    -1,   396,    -1,    -1,    -1,   160,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   172,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   186,   374,   188,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,    -1,   391,    -1,    -1,    -1,   208,    18,    19,
      20,    21,   213,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,   242,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,   255,    -1,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    -1,   268,   269,   270,
     374,    -1,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,    -1,   391,    -1,   290,
      -1,    -1,   396,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   314,   315,   316,   317,   318,   319,    -1,
      -1,    -1,   132,   133,   325,   326,   327,   328,   329,   330,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   354,   355,   356,   357,   358,   359,    -1,
      -1,   171,   172,    -1,    -1,    -1,    -1,    -1,   369,   370,
     371,   372,    -1,    -1,    -1,    -1,   186,    -1,   188,    -1,
      -1,    -1,    -1,   384,   385,    -1,    -1,    -1,   389,    -1,
      -1,    -1,    -1,   394,    -1,    -1,    -1,    18,    19,    20,
      21,    -1,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,   222,   374,    -1,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,    50,
     391,    -1,   242,    -1,   374,   396,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      -1,   391,    -1,    -1,    75,    -1,   396,    -1,   268,   269,
     270,    22,    -1,   273,   274,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   374,
     290,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   115,    -1,   391,    -1,    -1,    -1,
      -1,   396,    -1,    -1,   314,   315,   316,   317,   318,   319,
      22,   132,   133,    -1,    -1,   325,   326,   327,   328,   329,
     330,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
      -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   354,   355,   356,   357,   358,   359,
     171,   172,    -1,    -1,   175,    -1,    -1,    -1,    -1,   369,
     370,   371,   372,    -1,    -1,   186,    -1,   188,    -1,    -1,
      -1,    -1,    -1,    -1,   384,   385,    -1,    -1,    -1,   389,
      -1,    -1,    -1,    -1,   394,    -1,    -1,   208,    18,    19,
      20,    21,   213,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,   374,    -1,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
      50,   242,   391,    -1,    -1,    -1,    -1,   396,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    -1,   268,   269,   270,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   314,   315,   316,   317,   318,   319,    -1,
      22,    -1,   132,   133,   325,   326,   327,   328,   329,   330,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   354,   355,   356,   357,   358,   359,    -1,
      -1,   171,   172,    -1,    -1,   175,    -1,    -1,   369,   370,
     371,   372,    -1,    -1,    -1,    -1,   186,    -1,   188,    -1,
      -1,    -1,    -1,   384,   385,    -1,    -1,    -1,   389,    -1,
      -1,    -1,    -1,   394,    -1,    -1,    -1,    -1,   208,    18,
      19,    20,    21,   213,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,
      -1,    50,   242,   374,    22,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,    -1,
     391,    -1,    -1,    -1,    -1,   396,    75,    -1,   268,   269,
     270,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     290,    -1,   374,    -1,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    -1,   391,
      -1,    -1,    -1,   395,   314,   315,   316,   317,   318,   319,
      -1,    -1,    22,   132,   133,   325,   326,   327,   328,   329,
     330,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   354,   355,   356,   357,   358,   359,
      -1,    22,   171,   172,    -1,    -1,   175,    -1,    -1,   369,
     370,   371,   372,    -1,    -1,    -1,    -1,   186,    -1,   188,
      -1,    -1,    -1,    -1,   384,   385,    -1,    -1,    -1,   389,
      -1,    -1,    -1,    -1,   394,    -1,    -1,    -1,    -1,   208,
      18,    19,    20,    21,   213,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    -1,   374,    -1,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    50,   242,   391,    -1,    -1,    -1,    -1,   396,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,   268,
     269,   270,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,   290,   374,    -1,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    -1,   391,
      -1,    -1,    -1,   395,   122,   314,   315,   316,   317,   318,
     319,    -1,    -1,    -1,   132,   133,   325,   326,   327,   328,
     329,   330,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,    -1,    -1,    -1,   154,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   354,   355,   356,   357,   358,
     359,    -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,
     369,   370,   371,   372,    -1,    -1,    -1,    -1,   186,    -1,
     188,    -1,    -1,    -1,    -1,   384,   385,    -1,    -1,    -1,
     389,    -1,    -1,    -1,    -1,   394,    -1,    -1,    -1,    -1,
     208,    -1,    -1,    -1,    22,   213,   374,    -1,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,    -1,   391,    -1,    -1,    -1,    -1,   396,    -1,
      -1,    -1,    -1,    -1,   242,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    19,    20,    21,   255,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
     268,   269,   270,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,    -1,   290,    -1,   374,    -1,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      75,   391,    -1,    -1,    -1,   395,   314,   315,   316,   317,
     318,   319,    -1,    -1,    -1,    -1,    -1,   325,   326,   327,
     328,   329,   330,   374,    -1,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,    -1,
     391,    -1,    -1,    -1,    22,   396,   354,   355,   356,   357,
     358,   359,    -1,    -1,    -1,    -1,    -1,   132,   133,   134,
      -1,   369,   370,   371,   372,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,   149,   150,   384,   385,    -1,   154,
      -1,   389,    -1,    -1,    -1,    -1,   394,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   171,   172,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   186,   374,   188,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    -1,   391,
      -1,    -1,    -1,   208,   396,    -1,    -1,   374,   213,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,    -1,   391,    -1,    -1,    -1,    -1,   396,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   242,    -1,    -1,
      -1,    18,    19,    20,    21,    -1,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   268,   269,   270,    22,    -1,   273,    -1,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   290,    -1,    37,    38,    22,
      -1,    -1,   297,    -1,    -1,    -1,    -1,    47,    75,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,   314,
     315,   316,   317,   318,   319,    -1,    -1,    -1,    -1,    -1,
     325,   326,   327,   328,   329,   330,   374,    -1,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,    -1,   391,    -1,    -1,    -1,    -1,   396,   354,
     355,   356,   357,   358,   359,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,   369,   370,   371,   372,   118,    -1,
      -1,    -1,   149,   150,    -1,    -1,    -1,   154,    -1,   384,
     385,    -1,    -1,    -1,   389,    -1,    -1,    -1,    -1,   394,
      -1,   141,    -1,    -1,   171,   172,    -1,    -1,    -1,    -1,
      -1,    52,    53,    54,    -1,    56,    -1,    -1,    -1,   186,
      61,   188,   162,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   208,    18,    19,    20,    21,   213,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,   208,    -1,
      -1,    -1,    -1,   213,    50,   242,   374,    -1,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,    -1,   391,    -1,    -1,    -1,    -1,   396,    75,
      -1,   268,   269,   270,    -1,   374,   273,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
      -1,    -1,   391,   290,    -1,    -1,    -1,   396,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,   283,    -1,    -1,    -1,   314,   315,   316,
     317,   318,   319,    -1,    -1,    -1,   132,   133,   325,   326,
     327,   328,   329,   330,   304,    -1,   207,    -1,    -1,    -1,
      -1,    -1,    -1,   149,   150,    -1,    22,    -1,   154,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   354,   355,   356,
     357,   358,   359,   333,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   369,   370,   371,   372,    -1,    -1,    -1,    -1,
     186,    -1,   188,    -1,    -1,    -1,    -1,   384,   385,    -1,
      -1,   361,   389,    -1,    -1,    -1,    -1,   394,   374,    -1,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,    -1,    -1,   391,   222,    -1,    -1,    -1,
     396,   374,   392,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   242,    -1,   391,    -1,
      -1,    -1,    -1,   396,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   268,   269,   270,    -1,   337,    -1,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,    -1,   290,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   304,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   314,   315,
     316,   317,   318,   319,    -1,   321,    -1,    -1,    -1,   325,
     326,   327,   328,   329,   330,    -1,    18,    19,    20,    21,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   354,   355,
     356,   357,   358,   359,    46,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,   369,   370,   371,   372,    -1,    -1,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   384,   385,
      -1,    -1,    -1,   389,    -1,    -1,    -1,   374,   394,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,    -1,   391,    -1,    -1,    -1,   395,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    19,    20,    21,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,    -1,    -1,
     132,   133,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    -1,    -1,   149,   150,    -1,
      -1,   374,   154,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,    -1,    -1,   391,    -1,
      -1,    -1,   395,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   186,    -1,   188,    -1,   374,    -1,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,    -1,   206,   391,    -1,    -1,    -1,   395,
      -1,    -1,    -1,    -1,   216,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
     242,    -1,    -1,   149,   150,    -1,    -1,    -1,   154,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,   268,   269,   270,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   279,    -1,    -1,
     186,    -1,   188,    -1,    -1,    -1,    -1,   374,   290,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,    -1,   391,    -1,    -1,    -1,   395,    -1,
      -1,    -1,   314,   315,   316,   317,   318,   319,    -1,    -1,
      -1,    -1,    -1,   325,   326,   327,   328,   329,   330,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   242,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   354,   355,   356,   357,   358,   359,    -1,    -1,
      -1,    -1,   268,   269,   270,    -1,    -1,   369,   370,   371,
     372,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   384,   385,   290,    -1,    -1,   389,    -1,    -1,
      -1,    -1,   394,    18,    19,    20,    21,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,   314,   315,
     316,   317,   318,   319,    -1,    -1,    -1,    -1,    -1,   325,
     326,   327,   328,   329,   330,    50,    -1,    -1,    -1,   335,
     336,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   354,   355,
     356,   357,   358,   359,    -1,    -1,    -1,   363,   364,    -1,
      -1,    -1,    -1,   369,   370,   371,   372,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   384,   385,
      -1,    -1,    -1,   389,    -1,    -1,    -1,    -1,   394,   374,
      -1,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,    22,    -1,   391,   132,   133,    -1,
     395,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,    -1,    -1,    -1,   154,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    19,
      20,    21,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   186,    -1,   188,    -1,    -1,    -1,    -1,    -1,   374,
      50,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,   374,
     395,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,    22,    -1,   391,    -1,    -1,    -1,
     395,    -1,    -1,    -1,    -1,    -1,   374,   242,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,    -1,   391,    -1,    -1,    -1,   395,    -1,    -1,
      -1,    -1,    -1,   268,   269,   270,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   290,    -1,    -1,    -1,   149,
     150,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   164,    -1,    -1,   167,    -1,   314,
     315,   316,   317,   318,   319,    -1,    -1,    -1,    -1,    -1,
     325,   326,   327,   328,   329,   330,   186,    -1,   188,    -1,
     335,   336,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   354,
     355,   356,   357,   358,   359,    22,    -1,    -1,   363,   364,
      -1,    -1,    -1,    -1,   369,   370,   371,   372,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   384,
     385,    -1,   242,    -1,   389,    -1,    -1,    -1,    -1,   394,
      18,    19,    20,    21,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    -1,    -1,   268,   269,
     270,    -1,    -1,   273,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    37,    38,    -1,
     290,    -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,
      -1,    -1,    52,    -1,   304,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   314,   315,   316,   317,   318,   319,
      -1,    -1,    -1,    -1,    -1,   325,   326,   327,   328,   329,
     330,    -1,    -1,    -1,    -1,    -1,   374,    -1,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,    -1,   391,   354,   355,   356,   357,   358,   359,
      -1,    -1,    -1,    -1,   132,   133,    -1,    -1,   118,   369,
     370,   371,   372,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,    -1,   384,   385,   154,    -1,    -1,   389,
      -1,   141,    -1,    -1,   394,    18,    19,    20,    21,    -1,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,   162,    -1,    -1,    -1,    -1,    -1,   186,   187,
     188,    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   374,    -1,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,    -1,   391,    -1,    -1,    -1,   395,   208,    -1,
      -1,    -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   242,    -1,    -1,    -1,    -1,    -1,
     248,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,   257,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     268,   269,   270,    22,    -1,    -1,    -1,    -1,    -1,   132,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   290,    22,    -1,    -1,   149,   150,    -1,    -1,
      -1,   154,    -1,   283,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   314,   315,   316,   317,
     318,   319,    -1,    -1,   304,    -1,    -1,   325,   326,   327,
     328,   329,   330,   186,   187,   188,    -1,   374,    -1,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,    -1,   333,   391,    -1,   354,   355,   356,   357,
     358,   359,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   369,   370,   371,   372,    -1,    -1,    -1,    -1,    -1,
      -1,   361,    -1,    -1,    22,    -1,   384,   385,    -1,   242,
      -1,   389,    -1,    -1,    -1,   248,   394,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   257,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   392,    -1,    -1,   268,   269,   270,    -1,    18,
      19,    20,    21,    -1,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    -1,    -1,   290,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   314,   315,   316,   317,   318,   319,    -1,    -1,    -1,
      69,    -1,   325,   326,   327,   328,   329,   330,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   354,   355,   356,   357,   358,   359,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   369,   370,   371,   372,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,   384,   385,   132,   133,    -1,   389,    -1,    -1,    -1,
      -1,   394,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,
      21,    -1,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    -1,    -1,    -1,    -1,   186,    -1,   188,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,   374,    57,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
      -1,    -1,   391,    22,    -1,   374,   395,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
      -1,    -1,   391,   242,    -1,   374,   395,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
      -1,    -1,   391,    -1,    -1,    -1,   395,    22,    -1,   268,
     269,   270,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,
      -1,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   290,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
      -1,   152,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   314,   315,   316,   317,   318,
     319,    -1,    -1,    -1,    -1,    -1,   325,   326,   327,   328,
     329,   330,    -1,    -1,    -1,   186,   374,   188,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,    -1,   391,    -1,   354,   355,   356,   357,   358,
     359,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     369,   370,   371,   372,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   384,   385,    -1,    -1,    -1,
     389,   242,    -1,    -1,    -1,   394,    18,    19,    20,    21,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   268,   269,   270,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   374,   290,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,    75,    -1,   391,    -1,    -1,    -1,   395,
      -1,    -1,    -1,   314,   315,   316,   317,   318,   319,    -1,
      -1,    -1,    -1,    -1,   325,   326,   327,   328,   329,   330,
     374,    -1,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,
      -1,   395,    -1,   354,   355,   356,   357,   358,   359,    -1,
     132,   133,    -1,    -1,    -1,    -1,    -1,    -1,   369,   370,
     371,   372,    -1,    -1,    -1,    -1,    -1,   149,   150,    -1,
      -1,    -1,   154,   384,   385,    -1,    -1,    -1,   389,    -1,
      -1,    -1,    -1,   394,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    -1,    -1,    -1,   186,    -1,   188,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,   374,    50,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
      -1,    -1,   391,    -1,    -1,    -1,   395,    -1,    -1,    -1,
     222,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   374,
     242,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,    -1,
     395,    -1,    -1,    -1,    -1,    -1,   268,   269,   270,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   150,    -1,    -1,    -1,
     154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   314,   315,   316,   317,   318,   319,    -1,    -1,
      -1,    -1,    -1,   325,   326,   327,   328,   329,   330,    -1,
      -1,    -1,   186,   374,   188,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    -1,    -1,
     391,    -1,   354,   355,   356,   357,   358,   359,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   369,   370,   371,
     372,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   384,   385,    -1,    -1,    -1,   389,   242,    -1,
      -1,    -1,   394,    -1,    -1,    -1,    18,    19,    20,    21,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    -1,   268,   269,   270,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   279,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
     314,   315,   316,   317,   318,   319,    -1,    -1,    -1,    -1,
      -1,   325,   326,   327,   328,   329,   330,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     354,   355,   356,   357,   358,   359,    -1,    -1,    -1,    -1,
     132,   133,    -1,    -1,    -1,   369,   370,   371,   372,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,    -1,
     384,   385,   154,    -1,    -1,   389,    -1,    -1,    -1,    -1,
     394,    18,    19,    20,    21,    -1,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   186,    -1,   188,    -1,    -1,    -1,
      -1,    -1,    -1,    50,   374,    -1,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,    -1,
      -1,   391,    69,    -1,    -1,   395,    -1,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
     242,   374,   244,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,    -1,    -1,   391,    -1,
      -1,    -1,   395,    -1,    22,    -1,   268,   269,   270,    -1,
      -1,    -1,    -1,    -1,    -1,   132,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,
      -1,    -1,   149,   150,    -1,    -1,    -1,   154,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   314,   315,   316,   317,   318,   319,    -1,    -1,
      -1,    -1,    -1,   325,   326,   327,   328,   329,   330,   186,
     374,   188,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,
      -1,   395,   354,   355,   356,   357,   358,   359,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   369,   370,   371,
     372,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   384,   385,    -1,   242,    -1,   389,    -1,    -1,
      -1,    -1,   394,    18,    19,    20,    21,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
      -1,   268,   269,   270,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   290,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   314,   315,   316,
     317,   318,   319,    -1,    -1,    -1,    -1,    -1,   325,   326,
     327,   328,   329,   330,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   354,   355,   356,
     357,   358,   359,    -1,    -1,    -1,    -1,   132,   133,    -1,
      -1,    -1,   369,   370,   371,   372,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,    -1,   384,   385,   154,
      -1,    -1,   389,    -1,    -1,    -1,    -1,   394,    18,    19,
      20,    21,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   186,    -1,   188,    -1,    -1,    -1,    -1,    -1,   374,
      50,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,    69,
     395,   374,    -1,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,    -1,    -1,   391,    -1,
      -1,    -1,   395,    -1,    -1,    -1,   374,   242,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    22,    -1,   391,    -1,    -1,    -1,   395,    -1,    -1,
      -1,    -1,    -1,   268,   269,   270,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,   290,    -1,    -1,    -1,   149,
     150,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   314,
     315,   316,   317,   318,   319,    -1,    -1,   322,    -1,    -1,
     325,   326,   327,   328,   329,   330,   186,    -1,   188,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   354,
     355,   356,   357,   358,   359,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   369,   370,   371,   372,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,   384,
     385,    -1,   242,    -1,   389,    -1,    -1,    -1,    -1,   394,
     374,    -1,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,    -1,   391,   268,   269,
     270,   395,    18,    19,    20,    21,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,    -1,    -1,
     290,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   314,   315,   316,   317,   318,   319,
      -1,    -1,    -1,    -1,    -1,   325,   326,   327,   328,   329,
     330,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   354,   355,   356,   357,   358,   359,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   369,
     370,   371,   372,    -1,    -1,    -1,   122,    -1,    22,    -1,
      -1,    -1,    -1,    -1,   384,   385,   132,   133,    -1,   389,
      -1,    -1,    -1,    -1,   394,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,   150,    -1,    -1,    -1,   154,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,
      21,    -1,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     186,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   374,    -1,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,    22,    -1,
     391,    -1,    -1,    -1,   395,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   374,   242,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
      -1,    -1,   391,    -1,    -1,    -1,   395,    -1,    -1,    -1,
      22,    -1,   268,   269,   270,    -1,    -1,    -1,    -1,    -1,
      -1,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   290,    -1,    -1,    -1,   149,   150,
      -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   314,   315,
     316,   317,   318,   319,    -1,    -1,    -1,    -1,    -1,   325,
     326,   327,   328,   329,   330,   186,   374,   188,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,    -1,    -1,   391,    -1,    -1,    -1,   395,   354,   355,
     356,   357,   358,   359,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   369,   370,   371,   372,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   384,   385,
      -1,   242,    -1,   389,    -1,    -1,    -1,    -1,   394,    18,
      19,    20,    21,    -1,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    -1,    -1,   268,   269,   270,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,   374,   290,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   304,    -1,   391,    -1,    -1,    -1,   395,
      -1,    -1,    -1,   314,   315,   316,   317,   318,   319,    -1,
      -1,    -1,    -1,    -1,   325,   326,   327,   328,   329,   330,
     374,    -1,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,
      -1,   395,    -1,   354,   355,   356,   357,   358,   359,    -1,
      -1,    -1,    -1,   132,   133,    -1,    -1,    -1,   369,   370,
     371,   372,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,    -1,   384,   385,   154,    -1,    -1,   389,    -1,
      -1,    -1,    -1,   394,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,   188,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,
     374,    -1,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,    -1,    -1,   391,    -1,    -1,
      22,   395,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,   374,   242,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    -1,   391,
      -1,    -1,    -1,   395,    -1,    -1,    -1,    -1,    -1,   268,
     269,   270,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   290,    -1,    -1,    -1,   149,   150,    -1,    -1,    -1,
     154,    -1,    -1,    -1,    -1,   304,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   314,   315,   316,   317,   318,
     319,    -1,    -1,    -1,    -1,    -1,   325,   326,   327,   328,
     329,   330,   186,    -1,   188,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   354,   355,   356,   357,   358,
     359,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     369,   370,   371,   372,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   384,   385,    -1,   242,    -1,
     389,    -1,    -1,    -1,    -1,   394,    18,    19,    20,    21,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    -1,   268,   269,   270,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     304,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     314,   315,   316,   317,   318,   319,    -1,    -1,    -1,    -1,
      -1,   325,   326,   327,   328,   329,   330,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     354,   355,   356,   357,   358,   359,    -1,    -1,    -1,    -1,
     132,   133,    -1,    -1,    -1,   369,   370,   371,   372,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,    -1,
     384,   385,   154,    -1,    -1,   389,    -1,    -1,    -1,    -1,
     394,    18,    19,    20,    21,    -1,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    -1,   186,    -1,   188,    55,    -1,    -1,
      -1,    -1,   374,    50,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    75,    76,   391,
      -1,    -1,   374,   395,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,    -1,    -1,   391,
      -1,    -1,    -1,   395,    -1,    -1,    -1,   105,   106,    -1,
     242,    -1,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   268,   269,   270,    -1,
      -1,    -1,    -1,   141,    -1,   132,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,    -1,
      -1,    -1,   149,   150,    -1,    -1,    -1,   154,   166,   167,
      -1,   169,    -1,   171,   172,   173,    -1,    -1,    -1,    -1,
      -1,    -1,   314,   315,   316,   317,   318,   319,    -1,    -1,
      -1,    -1,    -1,   325,   326,   327,   328,   329,   330,   186,
      -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     208,    -1,    -1,    -1,    -1,   213,    -1,    -1,    -1,    -1,
      -1,    -1,   354,   355,   356,   357,   358,   359,    -1,    -1,
      -1,   229,    -1,    -1,    -1,    -1,    -1,   369,   370,   371,
     372,    49,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,   384,   385,    -1,   242,    -1,   389,    -1,    -1,
      -1,    -1,   394,    -1,    -1,    -1,    -1,    75,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   274,    -1,    -1,    -1,
      -1,   268,   269,   270,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   293,    -1,   105,   106,    -1,
      -1,    -1,   110,   290,    -1,    -1,   304,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   314,   315,   316,
     317,   318,   319,   141,    -1,    -1,    -1,    -1,   325,   326,
     327,   328,   329,   330,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   353,    -1,    -1,   166,   167,
      -1,   169,    -1,   171,   172,   173,    -1,   354,   355,   356,
     357,   358,   359,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   369,   370,   371,   372,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   392,    -1,    -1,   384,   385,    -1,
     208,    -1,   389,    -1,    -1,   213,    -1,   394,    -1,    78,
      -1,    -1,    -1,    82,    83,    84,    85,    86,    87,    -1,
      -1,   229,    91,    92,    93,    94,    95,    96,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,    -1,    -1,    -1,    -1,   274,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   293,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   304,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   353,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   235,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   392,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   266,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   365,   366
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,    18,    19,    20,    21,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    35,    37,    38,    41,
      42,    46,    47,    48,    50,    51,    61,    64,    65,    70,
      73,    80,    81,    89,    90,    98,    99,   100,   101,   107,
     108,   111,   113,   116,   119,   120,   124,   126,   131,   132,
     133,   137,   139,   142,   149,   150,   154,   155,   156,   157,
     159,   161,   166,   168,   169,   173,   174,   185,   186,   187,
     188,   201,   203,   205,   218,   233,   238,   239,   240,   241,
     242,   243,   246,   247,   248,   249,   251,   252,   257,   259,
     260,   261,   262,   263,   264,   268,   269,   270,   275,   277,
     278,   281,   282,   289,   290,   291,   292,   294,   298,   299,
     300,   304,   306,   307,   308,   313,   314,   315,   316,   317,
     318,   319,   321,   322,   324,   325,   326,   327,   328,   329,
     330,   332,   333,   334,   354,   355,   356,   357,   358,   359,
     360,   361,   368,   369,   370,   371,   372,   384,   385,   389,
     394,   401,   402,   403,   404,   411,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   422,   429,   430,   431,
     432,   444,   452,   455,   461,   469,   471,   323,   394,   399,
     167,   394,   394,   394,   394,   394,   394,   394,   394,   394,
     394,    69,   123,   405,   429,   430,   394,   246,   214,   332,
     360,   430,    75,   465,    52,    53,    54,   337,   339,   340,
     341,   342,   343,   344,   345,   346,   347,   348,   349,   350,
     351,   352,   353,   437,    18,    19,    20,    23,    24,    26,
      27,    28,    29,    30,    31,    32,    88,   115,   171,   172,
     175,   208,   213,   242,   369,   370,   371,   385,   389,   394,
     404,   411,   429,   430,   443,   463,   465,   466,   405,    19,
      20,    64,    98,   108,   148,   169,   212,   282,   289,   435,
     405,   430,    67,   122,   222,   271,   293,   462,   463,   464,
     465,   466,    19,    20,   373,   435,   405,   405,   394,   404,
     405,   429,    88,   115,   175,   404,   429,   443,   463,   465,
     466,   394,   125,   236,   300,   321,   428,    71,   121,   269,
     429,   443,    78,    82,    83,    84,    85,    86,    87,    91,
      92,    93,    94,    95,    96,   109,   112,   130,   143,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     235,   266,   365,   366,   448,    51,   115,   304,   443,   463,
     464,   465,   466,   405,   397,   408,    99,   405,   394,   394,
     394,   394,    61,   248,   429,   430,    64,    67,   122,   140,
     158,   164,   167,   175,   289,   312,   331,   356,   404,   443,
     462,   463,   465,   466,   394,    43,    88,   175,   404,   429,
     443,   463,   465,   466,   218,   248,   405,    75,   122,   394,
     430,   129,   394,    51,   103,   151,   153,   164,   165,   217,
     250,   253,   267,   273,   305,   404,   406,   407,   450,   301,
     172,   466,   405,   394,    56,    61,   207,   405,   437,    18,
     169,   304,   337,   394,   443,   463,   465,   466,   394,   394,
     430,   394,   405,   408,   404,   430,   102,   227,   445,   430,
     322,   407,    67,    88,   160,   175,   255,   404,   429,   443,
     462,   465,   466,   273,   405,   462,   465,   466,   430,   430,
     407,    67,    88,   255,   404,   443,   462,   465,   466,   273,
     405,   462,   465,   466,   437,   405,   407,   335,   336,   363,
     364,   404,    64,   108,   169,   289,   429,   430,   335,   336,
     363,   364,   404,   430,   158,   265,   394,   158,   265,   404,
      79,   399,   399,   399,   404,   248,   404,   411,   412,   404,
     412,   404,   412,     0,   393,    22,   374,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     391,   392,   397,   408,    22,   374,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   391,
     399,    47,    52,   118,   141,   162,   283,   304,   392,   432,
     443,    49,    55,    76,   105,   106,   110,   115,   141,   166,
     167,   169,   173,   229,   274,   293,   304,   353,   392,   443,
     463,   465,   466,    52,   162,   209,   295,   296,   304,   423,
     443,   394,   394,   394,   362,   443,   404,   405,   406,   409,
     404,   407,   404,   406,   412,   404,   412,   406,   406,   404,
     412,   406,   404,   404,   404,   404,   443,   392,   405,   392,
     301,   407,   443,   204,   332,   338,   360,   394,   405,   407,
     405,   394,   394,   394,   394,   394,   394,   394,   394,   394,
      75,   204,   222,   464,   465,   474,   407,   404,   313,   322,
     451,   404,   404,   404,   376,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   391,   396,   408,
     301,   273,   278,   405,   407,   407,   405,   273,   397,   405,
     104,   122,   215,   218,   430,   464,   465,   474,   451,   396,
     406,   405,   404,   405,   407,   464,   465,   443,   407,   406,
     404,   405,   430,   404,   412,   430,   430,   264,   115,   443,
     463,   464,   465,   466,   273,   407,   443,   404,   407,   451,
     407,   407,   404,   404,   396,   430,   164,   167,   273,   304,
     407,   451,   396,   443,   301,   407,   407,   404,   407,   301,
     405,   430,   115,   404,   407,   117,   127,   163,   234,   439,
     440,   407,   404,   372,   407,    99,   405,    57,   152,   407,
     405,   405,   445,   405,   445,   405,   445,   404,   405,   410,
     404,    33,    58,   144,   146,   165,   231,   232,   253,   309,
     311,   441,   404,   430,   430,   430,   412,   329,   354,   454,
     407,   404,   273,   405,   407,   451,   407,   396,   404,   377,
     273,   405,   407,   396,   404,   304,   404,   404,   404,   404,
     396,   407,   407,   407,   407,   404,   404,   404,   404,   396,
     114,   124,   443,   177,   178,   206,   245,   442,   430,   443,
     442,   443,   404,   404,   404,   395,   395,    18,    19,   248,
     369,   370,   371,   403,   411,   429,   430,   404,   404,   412,
     404,   412,   404,   412,   404,   412,   404,   412,   404,   412,
     404,   412,   404,   412,   404,   412,   404,   412,   404,   412,
     404,   412,   404,   412,   404,   412,   167,   406,   399,   404,
     412,   404,   412,   404,   412,   404,   412,   404,   412,   404,
     412,   404,   412,   404,   412,   404,   412,   404,   412,   404,
     412,   404,   412,   404,   412,   404,   412,   404,   412,   404,
     412,   304,   140,   124,   236,   304,   337,   443,   443,   443,
     443,    52,    68,   117,   138,   179,   180,   181,   226,   230,
     276,   284,   285,   337,   438,   147,   269,   270,   314,   315,
     317,   318,   325,   326,   327,   328,   423,    41,    67,   124,
     209,   236,   237,   255,   304,   443,   462,   465,   304,   443,
     405,   407,   443,   167,   228,   254,   273,   304,   443,   463,
     464,   465,   466,   467,   258,   304,   321,   407,   464,   465,
     443,   405,   407,   304,   463,   464,   465,   466,   443,   445,
     404,   407,    66,    67,   115,   273,   274,   407,   463,   464,
     465,   466,   437,   404,   147,   167,   452,   427,   443,   463,
     465,   466,    67,   160,   164,   215,   228,   405,   426,   462,
     465,   466,   467,   470,   475,   404,    39,    41,    67,   124,
     125,   164,   209,   228,   236,   237,   272,   274,   286,   287,
     288,   304,   407,   425,   443,   462,   465,   466,   467,   473,
     475,    36,    62,    89,   151,   176,   184,   186,   188,   189,
     210,   211,   217,   228,   273,   279,   304,   407,   424,   443,
     447,   465,   466,   467,   472,   475,   367,   430,   430,   412,
     406,   396,   395,   395,   395,   395,   395,   396,   396,   396,
     396,   396,   396,   396,   396,   396,   404,   405,   430,   396,
     404,   407,   404,   406,   404,   406,   404,   404,   404,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   404,   404,   430,   404,   404,   404,   398,
     407,   182,   222,   436,   404,    40,   443,   405,   273,   396,
     404,   395,   220,   398,   396,   396,   395,   396,   321,   443,
     464,   465,   404,   405,   404,   396,   404,   404,   404,   407,
     404,   429,   301,   301,   396,   395,   430,   396,   443,   388,
     396,   388,   372,   396,   404,   404,   395,   405,   405,   405,
     396,   396,   395,   396,   396,   395,    44,    97,   453,   396,
     404,   404,   430,   404,   404,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,   433,   404,   404,
     405,   405,   396,   408,   392,   375,   375,   404,   375,   375,
      46,   279,   404,   448,   404,   407,   404,   408,   405,   273,
     448,   404,   407,   405,   407,   407,   407,   396,   404,    62,
     206,   216,   468,    72,   443,   463,   466,   404,    62,    63,
     165,   253,   302,   476,   407,   407,    75,   222,   474,   407,
     405,   396,   443,   407,   462,   407,   464,   465,   404,   407,
     396,   273,    66,   165,   253,   446,   219,   224,   476,    46,
     279,   404,   396,   407,   405,   273,   404,   448,   405,   145,
     279,   310,    46,   279,   404,    59,   213,   407,   405,    34,
     202,   206,   449,   407,   407,   304,   404,   304,   404,    46,
     279,   396,   443,   407,   443,   134,   273,   404,   407,   443,
     463,   465,   466,   372,   372,   134,   273,   297,   404,   407,
     443,   463,   465,   466,   404,   404,   256,   404,   304,   407,
      46,   279,   396,   443,   396,   396,   395,   407,   406,   404,
     412,   404,   412,   404,   412,   406,   404,   404,   404,   404,
     407,   396,   396,   396,   396,   396,   396,   396,   396,   396,
     398,   405,   443,   407,   407,   396,   412,   412,   412,   474,
     430,   396,   396,   394,   405,   404,   407,   412,   407,   407,
     404,   404,   407,   248,   412,   404,   129,   396,   396,   124,
     430,   404,   406,   404,   412,   404,   412,   448,   396,   396,
     404,   396,   407,   167,   443,   404,   404,   404,   396,   404,
     404,   396,   297,   297,    46,   279,    46,   279,   405,   443,
     404,   396,   443,   404,   407,   396,   443,    62,   204,   297,
     434,   404,   407,    77,   244,   407,   456,   396,   395,   395,
     395,   395,   395,   395,   395,   396,   396,   396,   396,   396,
     396,   407,   404,   404,   406,   404,   404,   404,   404,   404,
     396,   395,   404,   396,   395,   396,   395,   404,   404,   407,
     396,   396,   395,   396,   396,   395,   396,   395,   396,   405,
     404,   404,   405,   396,   398,   404,   404,   404,   404,   404,
     404,   404,   404,   395,   395,   396,   407,   404,   412,   404,
     412,   404,   404,   404,   404,   396,   396,   396,   396,   396,
     396,   396,   407,   396,   443,    45,   170,   280,   460,   396,
     396,   405,   407,   407,   404,   443,   404,    35,   396,   396,
     407,    60,   135,   136,   204,   221,   303,   320,   459,   395,
     396,   395,   395,   395,   395,   396,   396,   396,   396,   407,
     404,   404,   404,   404,   404,   404,   404,   396,   396,   404,
     407,   395,   395,   396,   395,   396,   405,   404,   404,   396,
     396,   407,   404,   412,   404,   404,   404,   395,   396,   396,
     396,   443,   443,   396,   404,   404,   443,    17,   128,   223,
     457,   396,   395,   395,   396,   396,   396,   404,   404,   404,
     395,   395,   407,   296,   395,   396,   407,   404,   412,   404,
     404,   396,   396,   395,   404,    74,   183,   225,   458,   396,
     395,   395,   396,   395,   396,   404,   404,   396,   396,   395,
     404,   404,   412,   443,   396,   405,   404,   395,   395,   395,
     396,   404,   396,   443,   404,   396,   396,   443,   404,   395,
     396,   443,   395
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 624 "pars.yacc"
    {
            expr_parsed = TRUE;
            s_result = (yyvsp[(1) - (1)].dval);
        }
    break;

  case 4:
#line 628 "pars.yacc"
    {
            vexpr_parsed = TRUE;
            v_result = (yyvsp[(1) - (1)].vrbl);
        }
    break;

  case 8:
#line 641 "pars.yacc"
    {}
    break;

  case 9:
#line 642 "pars.yacc"
    {}
    break;

  case 10:
#line 643 "pars.yacc"
    {}
    break;

  case 11:
#line 644 "pars.yacc"
    {}
    break;

  case 12:
#line 645 "pars.yacc"
    {}
    break;

  case 13:
#line 646 "pars.yacc"
    {}
    break;

  case 14:
#line 647 "pars.yacc"
    {}
    break;

  case 15:
#line 648 "pars.yacc"
    {}
    break;

  case 16:
#line 649 "pars.yacc"
    {}
    break;

  case 17:
#line 650 "pars.yacc"
    {}
    break;

  case 18:
#line 651 "pars.yacc"
    {
	    return 1;
	}
    break;

  case 19:
#line 658 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(1) - (1)].dval);
	}
    break;

  case 20:
#line 661 "pars.yacc"
    {
	    (yyval.dval) = *((yyvsp[(1) - (1)].dptr));
	}
    break;

  case 21:
#line 664 "pars.yacc"
    {
	    (yyval.dval) = nonl_parms[(yyvsp[(1) - (1)].ival)].value;
	}
    break;

  case 22:
#line 667 "pars.yacc"
    {
	    (yyval.dval) = nonl_parms[(yyvsp[(1) - (1)].ival)].max;
	}
    break;

  case 23:
#line 670 "pars.yacc"
    {
	    (yyval.dval) = nonl_parms[(yyvsp[(1) - (1)].ival)].min;
	}
    break;

  case 24:
#line 673 "pars.yacc"
    {
            if ((yyvsp[(2) - (2)].ival) >= (yyvsp[(1) - (2)].vrbl)->length) {
                errmsg("Access beyond array bounds");
                return 1;
            }
            (yyval.dval) = (yyvsp[(1) - (2)].vrbl)->data[(yyvsp[(2) - (2)].ival)];
	}
    break;

  case 25:
#line 680 "pars.yacc"
    {
	    double dummy, dummy2;
            int idummy, ind, length = (yyvsp[(3) - (4)].vrbl)->length;
	    if ((yyvsp[(3) - (4)].vrbl)->data == NULL) {
		yyerror("NULL variable, check set type");
		return 1;
	    }
	    switch ((yyvsp[(1) - (4)].ival)) {
	    case MINP:
		(yyval.dval) = vmin((yyvsp[(3) - (4)].vrbl)->data, length);
		break;
	    case MAXP:
		(yyval.dval) = vmax((yyvsp[(3) - (4)].vrbl)->data, length);
		break;
            case AVG:
		stasum((yyvsp[(3) - (4)].vrbl)->data, length, &(yyval.dval), &dummy);
                break;
            case SD:
		stasum((yyvsp[(3) - (4)].vrbl)->data, length, &dummy, &(yyval.dval));
                break;
            case SUM:
		stasum((yyvsp[(3) - (4)].vrbl)->data, length, &(yyval.dval), &dummy);
                (yyval.dval) *= length;
                break;
            case IMIN:
		minmax((yyvsp[(3) - (4)].vrbl)->data, length, &dummy, &dummy2, &ind, &idummy);
                (yyval.dval) = (double) ind;
                break;
            case IMAX:
		minmax((yyvsp[(3) - (4)].vrbl)->data, length, &dummy, &dummy2, &idummy, &ind);
                (yyval.dval) = (double) ind;
                break;
	    }
	}
    break;

  case 26:
#line 714 "pars.yacc"
    {
	    if ((yyvsp[(3) - (6)].vrbl)->length != (yyvsp[(5) - (6)].vrbl)->length) {
		yyerror("X and Y are of different length");
		return 1;
            } else {
                (yyval.dval) = trapint((yyvsp[(3) - (6)].vrbl)->data, (yyvsp[(5) - (6)].vrbl)->data, NULL, NULL, (yyvsp[(3) - (6)].vrbl)->length);
            }
	}
    break;

  case 27:
#line 722 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(1) - (3)].vrbl)->length;
	}
    break;

  case 28:
#line 725 "pars.yacc"
    {
	    (yyval.dval) = getsetlength((yyvsp[(1) - (3)].trgt)->gno, (yyvsp[(1) - (3)].trgt)->setno);
	}
    break;

  case 29:
#line 728 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(1) - (3)].trgt)->setno;
	}
    break;

  case 30:
#line 731 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(1) - (3)].ival);
	}
    break;

  case 31:
#line 735 "pars.yacc"
    {
            (yyval.dval) = ((ParserFnc) (key[(yyvsp[(1) - (1)].ival)].data)) ();
	}
    break;

  case 32:
#line 739 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(1) - (2)].dval) * ((ParserFnc) (key[(yyvsp[(2) - (2)].ival)].data)) ();
	}
    break;

  case 33:
#line 743 "pars.yacc"
    {
	    (yyval.dval) = drand48();
	}
    break;

  case 34:
#line 747 "pars.yacc"
    {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[(1) - (4)].ival)].data)) ((yyvsp[(3) - (4)].ival));
	}
    break;

  case 35:
#line 751 "pars.yacc"
    {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[(1) - (4)].ival)].data)) ((yyvsp[(3) - (4)].dval));
	}
    break;

  case 36:
#line 755 "pars.yacc"
    {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[(1) - (6)].ival)].data)) ((yyvsp[(3) - (6)].ival), (yyvsp[(5) - (6)].dval));
	}
    break;

  case 37:
#line 759 "pars.yacc"
    {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[(1) - (6)].ival)].data)) ((yyvsp[(3) - (6)].ival), (yyvsp[(5) - (6)].ival));
	}
    break;

  case 38:
#line 763 "pars.yacc"
    {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[(1) - (6)].ival)].data)) ((yyvsp[(3) - (6)].dval), (yyvsp[(5) - (6)].dval));
	}
    break;

  case 39:
#line 767 "pars.yacc"
    {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[(1) - (8)].ival)].data)) ((yyvsp[(3) - (8)].ival), (yyvsp[(5) - (8)].ival), (yyvsp[(7) - (8)].dval));
	}
    break;

  case 40:
#line 771 "pars.yacc"
    {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[(1) - (8)].ival)].data)) ((yyvsp[(3) - (8)].dval), (yyvsp[(5) - (8)].dval), (yyvsp[(7) - (8)].dval));
	}
    break;

  case 41:
#line 775 "pars.yacc"
    {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[(1) - (10)].ival)].data)) ((yyvsp[(3) - (10)].dval), (yyvsp[(5) - (10)].dval), (yyvsp[(7) - (10)].dval), (yyvsp[(9) - (10)].dval));
	}
    break;

  case 42:
#line 779 "pars.yacc"
    {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[(1) - (12)].ival)].data)) ((yyvsp[(3) - (12)].dval), (yyvsp[(5) - (12)].dval), (yyvsp[(7) - (12)].dval), (yyvsp[(9) - (12)].dval), (yyvsp[(11) - (12)].dval));
	}
    break;

  case 43:
#line 783 "pars.yacc"
    {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[(1) - (14)].ival)].data)) ((yyvsp[(3) - (14)].dval), (yyvsp[(5) - (14)].dval), (yyvsp[(7) - (14)].dval), (yyvsp[(9) - (14)].dval), (yyvsp[(11) - (14)].dval), (yyvsp[(13) - (14)].dval));
	}
    break;

  case 44:
#line 786 "pars.yacc"
    {
	    (yyval.dval) = g[(yyvsp[(1) - (3)].ival)].v.xv1;
	}
    break;

  case 45:
#line 789 "pars.yacc"
    {
	    (yyval.dval) = g[(yyvsp[(1) - (3)].ival)].v.xv2;
	}
    break;

  case 46:
#line 792 "pars.yacc"
    {
	    (yyval.dval) = g[(yyvsp[(1) - (3)].ival)].v.yv1;
	}
    break;

  case 47:
#line 795 "pars.yacc"
    {
	    (yyval.dval) = g[(yyvsp[(1) - (3)].ival)].v.yv2;
	}
    break;

  case 48:
#line 798 "pars.yacc"
    {
	    (yyval.dval) = g[(yyvsp[(1) - (3)].ival)].w.xg1;
	}
    break;

  case 49:
#line 801 "pars.yacc"
    {
	    (yyval.dval) = g[(yyvsp[(1) - (3)].ival)].w.xg2;
	}
    break;

  case 50:
#line 804 "pars.yacc"
    {
	    (yyval.dval) = g[(yyvsp[(1) - (3)].ival)].w.yg1;
	}
    break;

  case 51:
#line 807 "pars.yacc"
    {
	    (yyval.dval) = g[(yyvsp[(1) - (3)].ival)].w.yg2;
	}
    break;

  case 52:
#line 810 "pars.yacc"
    {
            (yyval.dval) = (yyvsp[(3) - (4)].dval);
	}
    break;

  case 53:
#line 813 "pars.yacc"
    { /* yr, mo, day */
	    (yyval.dval) = cal_and_time_to_jul((yyvsp[(3) - (8)].ival), (yyvsp[(5) - (8)].ival), (yyvsp[(7) - (8)].ival), 12, 0, 0.0);
	}
    break;

  case 54:
#line 817 "pars.yacc"
    { /* yr, mo, day, hr, min, sec */
	    (yyval.dval) = cal_and_time_to_jul((yyvsp[(3) - (14)].ival), (yyvsp[(5) - (14)].ival), (yyvsp[(7) - (14)].ival), (yyvsp[(9) - (14)].ival), (yyvsp[(11) - (14)].ival), (yyvsp[(13) - (14)].dval));
	}
    break;

  case 55:
#line 820 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
            (yyval.dval) = g[whichgraph].v.xv1;
	}
    break;

  case 56:
#line 827 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    (yyval.dval) = g[whichgraph].v.xv2;
	}
    break;

  case 57:
#line 834 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    (yyval.dval) = g[whichgraph].v.yv1;
	}
    break;

  case 58:
#line 841 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    (yyval.dval) = g[whichgraph].v.yv2;
	}
    break;

  case 59:
#line 848 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    (yyval.dval) = g[whichgraph].w.xg1;
	}
    break;

  case 60:
#line 855 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    (yyval.dval) = g[whichgraph].w.xg2;
	}
    break;

  case 61:
#line 862 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    (yyval.dval) = g[whichgraph].w.yg1;
	}
    break;

  case 62:
#line 869 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    (yyval.dval) = g[whichgraph].w.yg2;
	}
    break;

  case 63:
#line 876 "pars.yacc"
    {
	    double vx, vy;
            get_page_viewport(&vx, &vy);
            (yyval.dval) = vx;
	}
    break;

  case 64:
#line 881 "pars.yacc"
    {
	    double vx, vy;
            get_page_viewport(&vx, &vy);
            (yyval.dval) = vy;
	}
    break;

  case 65:
#line 886 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(2) - (3)].dval);
	}
    break;

  case 66:
#line 889 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(1) - (3)].dval) + (yyvsp[(3) - (3)].dval);
	}
    break;

  case 67:
#line 892 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(1) - (3)].dval) - (yyvsp[(3) - (3)].dval);
	}
    break;

  case 68:
#line 895 "pars.yacc"
    {
	    (yyval.dval) = -(yyvsp[(2) - (2)].dval);
	}
    break;

  case 69:
#line 898 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(2) - (2)].dval);
	}
    break;

  case 70:
#line 901 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(1) - (3)].dval) * (yyvsp[(3) - (3)].dval);
	}
    break;

  case 71:
#line 905 "pars.yacc"
    {
	    if ((yyvsp[(3) - (3)].dval) != 0.0) {
		(yyval.dval) = (yyvsp[(1) - (3)].dval) / (yyvsp[(3) - (3)].dval);
	    } else {
		yyerror("Divide by zero");
		return 1;
	    }
	}
    break;

  case 72:
#line 913 "pars.yacc"
    {
	    if ((yyvsp[(3) - (3)].dval) != 0.0) {
		(yyval.dval) = fmod((yyvsp[(1) - (3)].dval), (yyvsp[(3) - (3)].dval));
	    } else {
		yyerror("Divide by zero");
		return 1;
	    }
	}
    break;

  case 73:
#line 921 "pars.yacc"
    {
	    if ((yyvsp[(1) - (3)].dval) < 0 && rint((yyvsp[(3) - (3)].dval)) != (yyvsp[(3) - (3)].dval)) {
		yyerror("Negative value raised to non-integer power");
		return 1;
            } else if ((yyvsp[(1) - (3)].dval) == 0.0 && (yyvsp[(3) - (3)].dval) <= 0.0) {
		yyerror("Zero raised to non-positive power");
		return 1;
            } else {
                (yyval.dval) = pow((yyvsp[(1) - (3)].dval), (yyvsp[(3) - (3)].dval));
            }
	}
    break;

  case 74:
#line 932 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(1) - (5)].dval) ? (yyvsp[(3) - (5)].dval) : (yyvsp[(5) - (5)].dval);
	}
    break;

  case 75:
#line 935 "pars.yacc"
    {
	   (yyval.dval) = ((yyvsp[(1) - (3)].dval) > (yyvsp[(3) - (3)].dval));
	}
    break;

  case 76:
#line 938 "pars.yacc"
    {
	   (yyval.dval) = ((yyvsp[(1) - (3)].dval) < (yyvsp[(3) - (3)].dval));
	}
    break;

  case 77:
#line 941 "pars.yacc"
    {
	   (yyval.dval) = ((yyvsp[(1) - (3)].dval) <= (yyvsp[(3) - (3)].dval));
	}
    break;

  case 78:
#line 944 "pars.yacc"
    {
	   (yyval.dval) = ((yyvsp[(1) - (3)].dval) >= (yyvsp[(3) - (3)].dval));
	}
    break;

  case 79:
#line 947 "pars.yacc"
    {
	   (yyval.dval) = ((yyvsp[(1) - (3)].dval) == (yyvsp[(3) - (3)].dval));
	}
    break;

  case 80:
#line 950 "pars.yacc"
    {
	    (yyval.dval) = ((yyvsp[(1) - (3)].dval) != (yyvsp[(3) - (3)].dval));
	}
    break;

  case 81:
#line 953 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(1) - (3)].dval) && (yyvsp[(3) - (3)].dval);
	}
    break;

  case 82:
#line 956 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(1) - (3)].dval) || (yyvsp[(3) - (3)].dval);
	}
    break;

  case 83:
#line 959 "pars.yacc"
    {
	    (yyval.dval) = !((yyvsp[(2) - (2)].dval));
	}
    break;

  case 84:
#line 964 "pars.yacc"
    {
            (yyval.sval) = (yyvsp[(1) - (1)].sval);
	}
    break;

  case 85:
#line 967 "pars.yacc"
    {
            (yyval.sval) = concat_strings((yyvsp[(1) - (3)].sval), (yyvsp[(3) - (3)].sval));
            xfree((yyvsp[(3) - (3)].sval));
        }
    break;

  case 86:
#line 971 "pars.yacc"
    {
            char buf[32];
            set_locale_num(TRUE);
            sprintf(buf, "%g", (yyvsp[(3) - (3)].dval));
            set_locale_num(FALSE);
            (yyval.sval) = concat_strings((yyvsp[(1) - (3)].sval), buf);
        }
    break;

  case 87:
#line 980 "pars.yacc"
    {
	    int itmp = rint((yyvsp[(1) - (1)].dval));
            if (fabs(itmp - (yyvsp[(1) - (1)].dval)) > 1.e-6) {
		yyerror("Non-integer value supplied for integer");
		return 1;
            }
            (yyval.ival) = itmp;
        }
    break;

  case 88:
#line 990 "pars.yacc"
    {
            if ((yyvsp[(1) - (1)].ival) < 0) {
		yyerror("Negative value supplied for non-negative");
		return 1;
            }
            (yyval.ival) = (yyvsp[(1) - (1)].ival);
	}
    break;

  case 89:
#line 999 "pars.yacc"
    {
	    int itmp = (yyvsp[(2) - (3)].ival) - index_shift;
            if (itmp < 0) {
		yyerror("Negative index");
		return 1;
            }
            (yyval.ival) = itmp;
	}
    break;

  case 90:
#line 1009 "pars.yacc"
    {
            (yyval.dval) = (yyvsp[(1) - (1)].dval);
        }
    break;

  case 91:
#line 1012 "pars.yacc"
    {
            double jul;
            Dates_format dummy;
            if (parse_date((yyvsp[(1) - (1)].sval), get_date_hint(), FALSE, &jul, &dummy)
                == RETURN_SUCCESS) {
                xfree((yyvsp[(1) - (1)].sval));
                (yyval.dval) = jul;
            } else {
                xfree((yyvsp[(1) - (1)].sval));
		yyerror("Invalid date");
		return 1;
            }
        }
    break;

  case 92:
#line 1027 "pars.yacc"
    {
            (yyval.dval) = (yyvsp[(1) - (1)].dval);
        }
    break;

  case 93:
#line 1030 "pars.yacc"
    {
            double jul;
            Dates_format dummy;
            if (parse_date((yyvsp[(1) - (1)].sval), get_date_hint(), TRUE, &jul, &dummy)
                == RETURN_SUCCESS) {
                xfree((yyvsp[(1) - (1)].sval));
                (yyval.dval) = jul;
            } else {
                xfree((yyvsp[(1) - (1)].sval));
		yyerror("Invalid date");
		return 1;
            }
        }
    break;

  case 94:
#line 1047 "pars.yacc"
    {
            (yyval.vrbl) = (yyvsp[(1) - (1)].vrbl);
	}
    break;

  case 95:
#line 1051 "pars.yacc"
    {
	    double *ptr = getcol(vasgn_gno, vasgn_setno, (yyvsp[(1) - (1)].ival));
            (yyval.vrbl) = &freelist[fcnt++];
            (yyval.vrbl)->type = GRARR_SET;
            (yyval.vrbl)->data = ptr;
            if (ptr == NULL) {
                errmsg("NULL variable - check set type");
                return 1;
            } else {
                (yyval.vrbl)->length = getsetlength(vasgn_gno, vasgn_setno);
            }
	}
    break;

  case 96:
#line 1064 "pars.yacc"
    {
	    double *ptr = getcol((yyvsp[(1) - (3)].trgt)->gno, (yyvsp[(1) - (3)].trgt)->setno, (yyvsp[(3) - (3)].ival));
            (yyval.vrbl) = &freelist[fcnt++];
            (yyval.vrbl)->type = GRARR_SET;
            (yyval.vrbl)->data = ptr;
            if (ptr == NULL) {
                errmsg("NULL variable - check set type");
                return 1;
            } else {
                (yyval.vrbl)->length = getsetlength((yyvsp[(1) - (3)].trgt)->gno, (yyvsp[(1) - (3)].trgt)->setno);
            }
	}
    break;

  case 97:
#line 1080 "pars.yacc"
    {
            (yyval.vrbl) = (yyvsp[(1) - (1)].vrbl);
	}
    break;

  case 98:
#line 1084 "pars.yacc"
    {
            int start = (yyvsp[(3) - (6)].ival) - index_shift, stop = (yyvsp[(5) - (6)].ival) - index_shift;
            if (start < 0 || stop < start || stop >= (yyvsp[(1) - (6)].vrbl)->length) {
		yyerror("Invalid index range");
            } else {
                int len = stop - start + 1;
	        double *ptr = (double*)xmalloc(len*sizeof(double)));
                if ((yyval.vrbl)->data == NULL) {
                    yyerror("Not enough memory");
                } else {
                    int i;
                    (yyval.vrbl) = &freelist[fcnt++];
	            (yyval.vrbl)->data = ptr;
                    (yyval.vrbl)->length = len;
                    (yyval.vrbl)->type = GRARR_TMP;
                    for (i = 0; i < len; i++) {
                        (yyval.vrbl)->data[i] = (yyvsp[(1) - (6)].vrbl)->data[i + (yyvsp[(3) - (6)].ival)];
                    }
                }
            }
	}
    break;

  case 99:
#line 1106 "pars.yacc"
    {
            int len = (yyvsp[(3) - (4)].ival);
            if (len < 1) {
                yyerror("npoints must be > 0");
            } else {
                double *ptr = allocate_index_data(len);
                if (ptr == NULL) {
                    errmsg("Malloc failed");
                    return 1;
                } else {
                    (yyval.vrbl) = &freelist[fcnt++];
                    (yyval.vrbl)->type = GRARR_TMP;
                    (yyval.vrbl)->data = ptr;
                    (yyval.vrbl)->length = len;
                }
            }
	}
    break;

  case 100:
#line 1124 "pars.yacc"
    {
            int len = (yyvsp[(7) - (8)].ival);
            if (len < 2) {
                yyerror("npoints must be > 1");
            } else {
                double *ptr = allocate_mesh((yyvsp[(3) - (8)].dval), (yyvsp[(5) - (8)].dval), len);
                if (ptr == NULL) {
                    errmsg("Malloc failed");
                    return 1;
                } else {
                    (yyval.vrbl) = &freelist[fcnt++];
                    (yyval.vrbl)->type = GRARR_TMP;
                    (yyval.vrbl)->data = ptr;
                    (yyval.vrbl)->length = len;
                }
            }
	}
    break;

  case 101:
#line 1142 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    (yyval.vrbl)->data = (double*)xmalloc((yyvsp[(3) - (4)].ival)*sizeof(double));
            if ((yyval.vrbl)->data == NULL) {
                errmsg("Not enough memory");
                return 1;
            } else {
                (yyval.vrbl)->length = (yyvsp[(3) - (4)].ival);
                (yyval.vrbl)->type = GRARR_TMP;
            }
            for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = drand48();
	    }
	}
    break;

  case 102:
#line 1158 "pars.yacc"
    {
	    int rtype, i, len;
            char *rarray;
            
            rtype = RESTRICT_REG0 + (yyvsp[(1) - (4)].ival);
            
	    if (get_restriction_array((yyvsp[(3) - (4)].trgt)->gno, (yyvsp[(3) - (4)].trgt)->setno,
                rtype, FALSE, &rarray) != RETURN_SUCCESS) {
                errmsg("Error in region evaluation");
                return 1;
	    }

            len = getsetlength((yyvsp[(3) - (4)].trgt)->gno, (yyvsp[(3) - (4)].trgt)->setno);
            (yyval.vrbl) = &freelist[fcnt++];
	    (yyval.vrbl)->data = (double*)xmalloc(len*sizeof(double));
            if ((yyval.vrbl)->data == NULL) {
                errmsg("Not enough memory");
                return 1;
            } else {
                (yyval.vrbl)->length = len;
                (yyval.vrbl)->type = GRARR_TMP;
            }
            for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = rarray[i];
	    }
            
            xfree(rarray);
	}
    break;

  case 103:
#line 1187 "pars.yacc"
    {
            int i;
            (yyval.vrbl) = &freelist[fcnt++];
            copy_vrbl((yyval.vrbl), (yyvsp[(3) - (4)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            for (i = 1; i < (yyval.vrbl)->length; i++) {
                (yyval.vrbl)->data[i] += (yyval.vrbl)->data[i - 1];
            }
	}
    break;

  case 104:
#line 1197 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (4)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[(1) - (4)].ival)].data)) ((int) ((yyvsp[(3) - (4)].vrbl)->data[i]));
	    }
	}
    break;

  case 105:
#line 1207 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (4)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[(1) - (4)].ival)].data)) (((yyvsp[(3) - (4)].vrbl)->data[i]));
	    }
	}
    break;

  case 106:
#line 1217 "pars.yacc"
    {
	    int i;
	    if ((yyvsp[(3) - (6)].vrbl)->length != (yyvsp[(5) - (6)].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (6)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            
	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[(1) - (6)].ival)].data)) ((yyvsp[(3) - (6)].vrbl)->data[i], (yyvsp[(5) - (6)].vrbl)->data[i]);
	    }
	}
    break;

  case 107:
#line 1232 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(5) - (6)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            
	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[(1) - (6)].ival)].data)) ((yyvsp[(3) - (6)].dval), (yyvsp[(5) - (6)].vrbl)->data[i]);
	    }
	}
    break;

  case 108:
#line 1243 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (6)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            
	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[(1) - (6)].ival)].data)) ((yyvsp[(3) - (6)].vrbl)->data[i], (yyvsp[(5) - (6)].dval));
	    }
	}
    break;

  case 109:
#line 1254 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(5) - (6)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[(1) - (6)].ival)].data)) ((yyvsp[(3) - (6)].ival), (yyvsp[(5) - (6)].vrbl)->data[i]);
	    }
	}
    break;

  case 110:
#line 1265 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(7) - (8)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[(1) - (8)].ival)].data)) ((yyvsp[(3) - (8)].ival), (yyvsp[(5) - (8)].ival), (yyvsp[(7) - (8)].vrbl)->data[i]);
	    }
	}
    break;

  case 111:
#line 1276 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(7) - (8)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[(1) - (8)].ival)].data)) ((yyvsp[(3) - (8)].dval), (yyvsp[(5) - (8)].dval), (yyvsp[(7) - (8)].vrbl)->data[i]);
	    }
	}
    break;

  case 112:
#line 1287 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(9) - (10)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[(1) - (10)].ival)].data)) ((yyvsp[(3) - (10)].dval), (yyvsp[(5) - (10)].dval), (yyvsp[(7) - (10)].dval), (yyvsp[(9) - (10)].vrbl)->data[i]);
	    }
	}
    break;

  case 113:
#line 1298 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(11) - (12)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[(1) - (12)].ival)].data)) ((yyvsp[(3) - (12)].dval), (yyvsp[(5) - (12)].dval), (yyvsp[(7) - (12)].dval), (yyvsp[(9) - (12)].dval), (yyvsp[(11) - (12)].vrbl)->data[i]);
	    }
	}
    break;

  case 114:
#line 1309 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(13) - (14)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[(1) - (14)].ival)].data)) ((yyvsp[(3) - (14)].dval), (yyvsp[(5) - (14)].dval), (yyvsp[(7) - (14)].dval), (yyvsp[(9) - (14)].dval), (yyvsp[(11) - (14)].dval), (yyvsp[(13) - (14)].vrbl)->data[i]);
	    }
	}
    break;

  case 115:
#line 1320 "pars.yacc"
    {
	    int i;
	    if ((yyvsp[(1) - (3)].vrbl)->length != (yyvsp[(3) - (3)].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].vrbl)->data[i] + (yyvsp[(3) - (3)].vrbl)->data[i];
	    }
	}
    break;

  case 116:
#line 1335 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].vrbl)->data[i] + (yyvsp[(3) - (3)].dval);
	    }
	}
    break;

  case 117:
#line 1346 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].dval) + (yyvsp[(3) - (3)].vrbl)->data[i];
	    }
	}
    break;

  case 118:
#line 1357 "pars.yacc"
    {
	    int i;
	    if ((yyvsp[(1) - (3)].vrbl)->length != (yyvsp[(3) - (3)].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].vrbl)->data[i] - (yyvsp[(3) - (3)].vrbl)->data[i];
	    }
	}
    break;

  case 119:
#line 1372 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].vrbl)->data[i] - (yyvsp[(3) - (3)].dval);
	    }
	}
    break;

  case 120:
#line 1383 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].dval) - (yyvsp[(3) - (3)].vrbl)->data[i];
	    }
	}
    break;

  case 121:
#line 1394 "pars.yacc"
    {
	    int i;
	    if ((yyvsp[(1) - (3)].vrbl)->length != (yyvsp[(3) - (3)].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].vrbl)->data[i] * (yyvsp[(3) - (3)].vrbl)->data[i];
	    }
	}
    break;

  case 122:
#line 1409 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].vrbl)->data[i] * (yyvsp[(3) - (3)].dval);
	    }
	}
    break;

  case 123:
#line 1420 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].dval) * (yyvsp[(3) - (3)].vrbl)->data[i];
	    }
	}
    break;

  case 124:
#line 1431 "pars.yacc"
    {
	    int i;
	    if ((yyvsp[(1) - (3)].vrbl)->length != (yyvsp[(3) - (3)].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		if ((yyvsp[(3) - (3)].vrbl)->data[i] == 0.0) {
                    errmsg("Divide by zero");
                    return 1;
                }
                (yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].vrbl)->data[i] / (yyvsp[(3) - (3)].vrbl)->data[i];
	    }
	}
    break;

  case 125:
#line 1450 "pars.yacc"
    {
	    int i;
	    if ((yyvsp[(3) - (3)].dval) == 0.0) {
                errmsg("Divide by zero");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].vrbl)->data[i] / (yyvsp[(3) - (3)].dval);
	    }
	}
    break;

  case 126:
#line 1465 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		if ((yyvsp[(3) - (3)].vrbl)->data[i] == 0.0) {
                    errmsg("Divide by zero");
                    return 1;
                }
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].dval) / (yyvsp[(3) - (3)].vrbl)->data[i];
	    }
	}
    break;

  case 127:
#line 1480 "pars.yacc"
    {
	    int i;
	    if ((yyvsp[(1) - (3)].vrbl)->length != (yyvsp[(3) - (3)].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		if ((yyvsp[(3) - (3)].vrbl)->data[i] == 0.0) {
                    errmsg("Divide by zero");
                    return 1;
                } else {
                    (yyval.vrbl)->data[i] = fmod((yyvsp[(1) - (3)].vrbl)->data[i], (yyvsp[(3) - (3)].vrbl)->data[i]);
                }
	    }
	}
    break;

  case 128:
#line 1500 "pars.yacc"
    {
	    int i;
	    if ((yyvsp[(3) - (3)].dval) == 0.0) {
                errmsg("Divide by zero");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = fmod((yyvsp[(1) - (3)].vrbl)->data[i], (yyvsp[(3) - (3)].dval));
	    }
	}
    break;

  case 129:
#line 1515 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		if ((yyvsp[(3) - (3)].vrbl)->data[i] == 0.0) {
                    errmsg("Divide by zero");
                    return 1;
                } else {
		    (yyval.vrbl)->data[i] = fmod((yyvsp[(1) - (3)].dval), (yyvsp[(3) - (3)].vrbl)->data[i]);
                }
	    }
	}
    break;

  case 130:
#line 1531 "pars.yacc"
    {
	    int i;
	    if ((yyvsp[(1) - (3)].vrbl)->length != (yyvsp[(3) - (3)].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
	        if ((yyvsp[(1) - (3)].vrbl)->data[i] < 0 && rint((yyvsp[(3) - (3)].vrbl)->data[i]) != (yyvsp[(3) - (3)].vrbl)->data[i]) {
	            yyerror("Negative value raised to non-integer power");
	            return 1;
                } else if ((yyvsp[(1) - (3)].vrbl)->data[i] == 0.0 && (yyvsp[(3) - (3)].vrbl)->data[i] <= 0.0) {
	            yyerror("Zero raised to non-positive power");
	            return 1;
                } else {
                    (yyval.vrbl)->data[i] = pow((yyvsp[(1) - (3)].vrbl)->data[i], (yyvsp[(3) - (3)].vrbl)->data[i]);
                }
	    }
	}
    break;

  case 131:
#line 1554 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
	        if ((yyvsp[(1) - (3)].vrbl)->data[i] < 0 && rint((yyvsp[(3) - (3)].dval)) != (yyvsp[(3) - (3)].dval)) {
	            yyerror("Negative value raised to non-integer power");
	            return 1;
                } else if ((yyvsp[(1) - (3)].vrbl)->data[i] == 0.0 && (yyvsp[(3) - (3)].dval) <= 0.0) {
	            yyerror("Zero raised to non-positive power");
	            return 1;
                } else {
                    (yyval.vrbl)->data[i] = pow((yyvsp[(1) - (3)].vrbl)->data[i], (yyvsp[(3) - (3)].dval));
                }
	    }
	}
    break;

  case 132:
#line 1573 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
	        if ((yyvsp[(1) - (3)].dval) < 0 && rint((yyvsp[(3) - (3)].vrbl)->data[i]) != (yyvsp[(3) - (3)].vrbl)->data[i]) {
	            yyerror("Negative value raised to non-integer power");
	            return 1;
                } else if ((yyvsp[(1) - (3)].dval) == 0.0 && (yyvsp[(3) - (3)].vrbl)->data[i] <= 0.0) {
	            yyerror("Zero raised to non-positive power");
	            return 1;
                } else {
                    (yyval.vrbl)->data[i] = pow((yyvsp[(1) - (3)].dval), (yyvsp[(3) - (3)].vrbl)->data[i]);
                }
	    }
	}
    break;

  case 133:
#line 1592 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (2)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (2)].vrbl)->data[i] * ((ParserFnc) (key[(yyvsp[(2) - (2)].ival)].data)) ();
	    }
	}
    break;

  case 134:
#line 1601 "pars.yacc"
    {
            int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (5)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            for (i = 0; i < (yyval.vrbl)->length; i++) { 
                (yyval.vrbl)->data[i] = CAST_DBL_TO_BOOL((yyvsp[(1) - (5)].vrbl)->data[i]) ? (yyvsp[(3) - (5)].dval) : (yyvsp[(5) - (5)].dval);
            }
	}
    break;

  case 135:
#line 1610 "pars.yacc"
    {
            int i;
	    if ((yyvsp[(1) - (5)].vrbl)->length != (yyvsp[(5) - (5)].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (5)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            for (i = 0; i < (yyval.vrbl)->length; i++) { 
                (yyval.vrbl)->data[i] = CAST_DBL_TO_BOOL((yyvsp[(1) - (5)].vrbl)->data[i]) ? (yyvsp[(3) - (5)].dval) : (yyvsp[(5) - (5)].vrbl)->data[i];
            }
	}
    break;

  case 136:
#line 1623 "pars.yacc"
    {
            int i;
	    if ((yyvsp[(1) - (5)].vrbl)->length != (yyvsp[(3) - (5)].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (5)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            for (i = 0; i < (yyval.vrbl)->length; i++) { 
                (yyval.vrbl)->data[i] = CAST_DBL_TO_BOOL((yyvsp[(1) - (5)].vrbl)->data[i]) ? (yyvsp[(3) - (5)].vrbl)->data[i] : (yyvsp[(5) - (5)].dval);
            }
	}
    break;

  case 137:
#line 1636 "pars.yacc"
    {
            int i;
	    if ((yyvsp[(1) - (5)].vrbl)->length != (yyvsp[(5) - (5)].vrbl)->length || (yyvsp[(1) - (5)].vrbl)->length != (yyvsp[(3) - (5)].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (5)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            for (i = 0; i < (yyval.vrbl)->length; i++) { 
                (yyval.vrbl)->data[i] = CAST_DBL_TO_BOOL((yyvsp[(1) - (5)].vrbl)->data[i]) ? (yyvsp[(3) - (5)].vrbl)->data[i] : (yyvsp[(5) - (5)].vrbl)->data[i];
            }
	}
    break;

  case 138:
#line 1650 "pars.yacc"
    {
	    int i;
	    if ((yyvsp[(1) - (3)].vrbl)->length != (yyvsp[(3) - (3)].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].vrbl)->data[i] || (yyvsp[(3) - (3)].vrbl)->data[i];
	    }
	}
    break;

  case 139:
#line 1665 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].vrbl)->data[i] || (yyvsp[(3) - (3)].dval);
	    }
	}
    break;

  case 140:
#line 1676 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].dval) || (yyvsp[(3) - (3)].vrbl)->data[i];
	    }
	}
    break;

  case 141:
#line 1687 "pars.yacc"
    {
	    int i;
	    if ((yyvsp[(1) - (3)].vrbl)->length != (yyvsp[(3) - (3)].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].vrbl)->data[i] && (yyvsp[(3) - (3)].vrbl)->data[i];
	    }
	}
    break;

  case 142:
#line 1702 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].vrbl)->data[i] && (yyvsp[(3) - (3)].dval);
	    }
	}
    break;

  case 143:
#line 1713 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].dval) && (yyvsp[(3) - (3)].vrbl)->data[i];
	    }
	}
    break;

  case 144:
#line 1724 "pars.yacc"
    {
	    int i;
	    if ((yyvsp[(1) - (3)].vrbl)->length != (yyvsp[(3) - (3)].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].vrbl)->data[i] > (yyvsp[(3) - (3)].vrbl)->data[i]);
	    }
	}
    break;

  case 145:
#line 1739 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].vrbl)->data[i] > (yyvsp[(3) - (3)].dval));
	    }
	}
    break;

  case 146:
#line 1750 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].dval) > (yyvsp[(3) - (3)].vrbl)->data[i]);
	    }
	}
    break;

  case 147:
#line 1761 "pars.yacc"
    {
	    int i;
	    if ((yyvsp[(1) - (3)].vrbl)->length != (yyvsp[(3) - (3)].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].vrbl)->data[i] < (yyvsp[(3) - (3)].vrbl)->data[i]);
	    }
	}
    break;

  case 148:
#line 1776 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].vrbl)->data[i] < (yyvsp[(3) - (3)].dval));
	    }
	}
    break;

  case 149:
#line 1787 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].dval) < (yyvsp[(3) - (3)].vrbl)->data[i]);
	    }
	}
    break;

  case 150:
#line 1798 "pars.yacc"
    {
	    int i;
	    if ((yyvsp[(1) - (3)].vrbl)->length != (yyvsp[(3) - (3)].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].vrbl)->data[i] >= (yyvsp[(3) - (3)].vrbl)->data[i]);
	    }
	}
    break;

  case 151:
#line 1813 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].vrbl)->data[i] >= (yyvsp[(3) - (3)].dval));
	    }
	}
    break;

  case 152:
#line 1824 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].dval) >= (yyvsp[(3) - (3)].vrbl)->data[i]);
	    }
	}
    break;

  case 153:
#line 1835 "pars.yacc"
    {
	    int i;
	    if ((yyvsp[(1) - (3)].vrbl)->length != (yyvsp[(3) - (3)].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].vrbl)->data[i] <= (yyvsp[(3) - (3)].vrbl)->data[i]);
	    }
	}
    break;

  case 154:
#line 1850 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].vrbl)->data[i] <= (yyvsp[(3) - (3)].dval));
	    }
	}
    break;

  case 155:
#line 1861 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].dval) <= (yyvsp[(3) - (3)].vrbl)->data[i]);
	    }
	}
    break;

  case 156:
#line 1872 "pars.yacc"
    {
	    int i;
	    if ((yyvsp[(1) - (3)].vrbl)->length != (yyvsp[(3) - (3)].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].vrbl)->data[i] == (yyvsp[(3) - (3)].vrbl)->data[i]);
	    }
	}
    break;

  case 157:
#line 1887 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].vrbl)->data[i] == (yyvsp[(3) - (3)].dval));
	    }
	}
    break;

  case 158:
#line 1898 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].dval) == (yyvsp[(3) - (3)].vrbl)->data[i]);
	    }
	}
    break;

  case 159:
#line 1909 "pars.yacc"
    {
	    int i;
	    if ((yyvsp[(1) - (3)].vrbl)->length != (yyvsp[(3) - (3)].vrbl)->length) {
                errmsg("Can't operate on vectors of different lengths");
                return 1;
            }
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].vrbl)->data[i] != (yyvsp[(3) - (3)].vrbl)->data[i]);
	    }
	}
    break;

  case 160:
#line 1924 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].vrbl)->data[i] != (yyvsp[(3) - (3)].dval));
	    }
	}
    break;

  case 161:
#line 1935 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].dval) != (yyvsp[(3) - (3)].vrbl)->data[i]);
	    }
	}
    break;

  case 162:
#line 1946 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(2) - (2)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            for (i = 0; i < (yyval.vrbl)->length; i++) { 
                (yyval.vrbl)->data[i] = !(yyvsp[(2) - (2)].vrbl)->data[i];
            }
	}
    break;

  case 163:
#line 1956 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(2) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            for (i = 0; i < (yyval.vrbl)->length; i++) { 
                (yyval.vrbl)->data[i] = (yyvsp[(2) - (3)].vrbl)->data[i];
            }
	}
    break;

  case 164:
#line 1965 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(2) - (2)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            for (i = 0; i < (yyval.vrbl)->length; i++) { 
                (yyval.vrbl)->data[i] = - (yyvsp[(2) - (2)].vrbl)->data[i];
            }
	}
    break;

  case 165:
#line 1979 "pars.yacc"
    {
	    *((yyvsp[(1) - (3)].dptr)) = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 166:
#line 1983 "pars.yacc"
    {
	    nonl_parms[(yyvsp[(1) - (3)].ival)].value = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 167:
#line 1987 "pars.yacc"
    {
	    nonl_parms[(yyvsp[(1) - (3)].ival)].max = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 168:
#line 1991 "pars.yacc"
    {
	    nonl_parms[(yyvsp[(1) - (3)].ival)].min = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 169:
#line 1995 "pars.yacc"
    {
	    if ((yyvsp[(2) - (4)].ival) >= (yyvsp[(1) - (4)].vrbl)->length) {
		yyerror("Access beyond array bounds");
		return 1;
            }
            (yyvsp[(1) - (4)].vrbl)->data[(yyvsp[(2) - (4)].ival)] = (yyvsp[(4) - (4)].dval);
	}
    break;

  case 170:
#line 2006 "pars.yacc"
    {
            target tgt;
            switch ((yyvsp[(1) - (1)].vrbl)->type) {
            case GRARR_SET:
                if (find_set_bydata((yyvsp[(1) - (1)].vrbl)->data, &tgt) == RETURN_SUCCESS) {
                    vasgn_gno   = tgt.gno;
                    vasgn_setno = tgt.setno;
                } else {
                    errmsg("Internal error");
		    return 1;
                }
                break;
            case GRARR_VEC:
                vasgn_gno   = -1;
                vasgn_setno = -1;
                break;
            default:
                /* It can NOT be a tmp array on the left side! */
                errmsg("Internal error");
	        return 1;
            }
            (yyval.vrbl) = (yyvsp[(1) - (1)].vrbl);
        }
    break;

  case 171:
#line 2033 "pars.yacc"
    {
	    int i;
	    if ((yyvsp[(1) - (3)].vrbl)->length != (yyvsp[(3) - (3)].vrbl)->length) {
                errmsg("Left and right vectors are of different lengths");
                return 1;
            }
	    for (i = 0; i < (yyvsp[(1) - (3)].vrbl)->length; i++) {
	        (yyvsp[(1) - (3)].vrbl)->data[i] = (yyvsp[(3) - (3)].vrbl)->data[i];
	    }
	}
    break;

  case 172:
#line 2044 "pars.yacc"
    {
	    int i;
	    for (i = 0; i < (yyvsp[(1) - (3)].vrbl)->length; i++) {
	        (yyvsp[(1) - (3)].vrbl)->data[i] = (yyvsp[(3) - (3)].dval);
	    }
	}
    break;

  case 173:
#line 2054 "pars.yacc"
    {
	    symtab_entry tmpkey;
            double *var;
            
            var = (double*)xmalloc(sizeof(double));
            *var = 0.0;
            
	    tmpkey.s = (yyvsp[(2) - (2)].sval);
	    tmpkey.type = KEY_VAR;
	    tmpkey.data = (void *) var;
	    if (addto_symtab(tmpkey) != RETURN_SUCCESS) {
	        yyerror("Adding new symbol failed");
	    }

            xfree((yyvsp[(2) - (2)].sval));
        }
    break;

  case 174:
#line 2071 "pars.yacc"
    {
	    if (define_parser_arr((yyvsp[(2) - (4)].sval)) == NULL) {
	        yyerror("Adding new symbol failed");
	    }

            xfree((yyvsp[(2) - (4)].sval));
        }
    break;

  case 175:
#line 2079 "pars.yacc"
    {
	    grarr *var;
            if ((var = define_parser_arr((yyvsp[(2) - (5)].sval))) == NULL) {
	        yyerror("Adding new symbol failed");
	    } else {
                realloc_vrbl(var, (yyvsp[(4) - (5)].ival));
            }

            xfree((yyvsp[(2) - (5)].sval));
        }
    break;

  case 176:
#line 2090 "pars.yacc"
    {
            yyerror("Keyword already exists");
        }
    break;

  case 177:
#line 2094 "pars.yacc"
    {
            yyerror("Keyword already exists");
        }
    break;

  case 178:
#line 2098 "pars.yacc"
    {
            undefine_parser_var((void *) (yyvsp[(2) - (2)].dptr));
            xfree((yyvsp[(2) - (2)].dptr));
        }
    break;

  case 179:
#line 2103 "pars.yacc"
    {
            realloc_vrbl((yyvsp[(2) - (2)].vrbl), 0);
            undefine_parser_var((void *) (yyvsp[(2) - (2)].vrbl));
            xfree((yyvsp[(2) - (2)].vrbl));
        }
    break;

  case 180:
#line 2108 "pars.yacc"
    {
	    int position;

	    lowtoupper((yyvsp[(3) - (3)].sval));
	    if ((position = findf(key, (yyvsp[(3) - (3)].sval))) >= 0) {
	        symtab_entry tmpkey;
		tmpkey.s = (yyvsp[(2) - (3)].sval);
		tmpkey.type = key[position].type;
		tmpkey.data = key[position].data;
		if (addto_symtab(tmpkey) != RETURN_SUCCESS) {
		    yyerror("Keyword already exists");
		}
	    } else {
	        yyerror("Aliased keyword not found");
	    }
	    xfree((yyvsp[(2) - (3)].sval));
	    xfree((yyvsp[(3) - (3)].sval));
	}
    break;

  case 181:
#line 2126 "pars.yacc"
    {
	    alias_force = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 182:
#line 2129 "pars.yacc"
    {
	    if (load_module((yyvsp[(6) - (6)].sval), (yyvsp[(2) - (6)].sval), (yyvsp[(2) - (6)].sval), (yyvsp[(4) - (6)].ival)) != 0) {
	        yyerror("DL module load failed");
	    }
	    xfree((yyvsp[(2) - (6)].sval));
	    xfree((yyvsp[(6) - (6)].sval));
	}
    break;

  case 183:
#line 2136 "pars.yacc"
    {
	    if (load_module((yyvsp[(6) - (8)].sval), (yyvsp[(2) - (8)].sval), (yyvsp[(8) - (8)].sval), (yyvsp[(4) - (8)].ival)) != 0) {
	        yyerror("DL module load failed");
	    }
	    xfree((yyvsp[(2) - (8)].sval));
	    xfree((yyvsp[(6) - (8)].sval));
	    xfree((yyvsp[(8) - (8)].sval));
	}
    break;

  case 184:
#line 2147 "pars.yacc"
    {
	    rg[(yyvsp[(1) - (2)].ival)].active = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 185:
#line 2150 "pars.yacc"
    {
	    rg[(yyvsp[(1) - (3)].ival)].type = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 186:
#line 2153 "pars.yacc"
    {
	    rg[(yyvsp[(1) - (2)].ival)].color = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 187:
#line 2156 "pars.yacc"
    {
	    rg[(yyvsp[(1) - (2)].ival)].lines = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 188:
#line 2159 "pars.yacc"
    {
	    rg[(yyvsp[(1) - (2)].ival)].linew = (yyvsp[(2) - (2)].dval);
	}
    break;

  case 189:
#line 2163 "pars.yacc"
    {
	    rg[(yyvsp[(1) - (9)].ival)].x1 = (yyvsp[(3) - (9)].dval);
	    rg[(yyvsp[(1) - (9)].ival)].y1 = (yyvsp[(5) - (9)].dval);
	    rg[(yyvsp[(1) - (9)].ival)].x2 = (yyvsp[(7) - (9)].dval);
	    rg[(yyvsp[(1) - (9)].ival)].y2 = (yyvsp[(9) - (9)].dval);
	}
    break;

  case 190:
#line 2170 "pars.yacc"
    {
	    rg[(yyvsp[(1) - (5)].ival)].x = (double*)xrealloc(rg[(yyvsp[(1) - (5)].ival)].x, (rg[(yyvsp[(1) - (5)].ival)].n + 1) * sizeof(double));
	    rg[(yyvsp[(1) - (5)].ival)].y = (double*)xrealloc(rg[(yyvsp[(1) - (5)].ival)].y, (rg[(yyvsp[(1) - (5)].ival)].n + 1) * sizeof(double));
	    rg[(yyvsp[(1) - (5)].ival)].x[rg[(yyvsp[(1) - (5)].ival)].n] = (yyvsp[(3) - (5)].dval);
	    rg[(yyvsp[(1) - (5)].ival)].y[rg[(yyvsp[(1) - (5)].ival)].n] = (yyvsp[(5) - (5)].dval);
	    rg[(yyvsp[(1) - (5)].ival)].n++;
	}
    break;

  case 191:
#line 2177 "pars.yacc"
    {
	    rg[(yyvsp[(2) - (4)].ival)].linkto = (yyvsp[(4) - (4)].ival);
	}
    break;

  case 192:
#line 2184 "pars.yacc"
    {
            if (set_project_version((yyvsp[(2) - (2)].ival)) != RETURN_SUCCESS) {
                errmsg("Project version is newer than software!");
            }
            if (get_project_version() < 50001) {
                map_fonts(FONT_MAP_ACEGR);
            } else {
                map_fonts(FONT_MAP_DEFAULT);
            }
        }
    break;

  case 193:
#line 2194 "pars.yacc"
    {
            set_page_dimensions((yyvsp[(3) - (5)].ival), (yyvsp[(5) - (5)].ival), TRUE);
        }
    break;

  case 194:
#line 2197 "pars.yacc"
    {
            set_page_dimensions((yyvsp[(3) - (5)].ival), (yyvsp[(5) - (5)].ival), FALSE);
        }
    break;

  case 195:
#line 2200 "pars.yacc"
    {
            int device_id;
            Device_entry dev;
            
            device_id = get_device_by_name((yyvsp[(2) - (7)].sval));
            xfree((yyvsp[(2) - (7)].sval));
            if (device_id < 0) {
                yyerror("Unknown device");
            } else {
                dev = get_device_props(device_id);
                dev.pg.width =  (long) ((yyvsp[(5) - (7)].ival)*dev.pg.dpi/72);
                dev.pg.height = (long) ((yyvsp[(7) - (7)].ival)*dev.pg.dpi/72);
                set_device_props(device_id, dev);
            }
        }
    break;

  case 196:
#line 2215 "pars.yacc"
    {
            int device_id;
            Device_entry dev;
            
            device_id = get_device_by_name((yyvsp[(2) - (4)].sval));
            if (device_id < 0) {
                yyerror("Unknown device");
            } else {
                dev = get_device_props(device_id);
                dev.pg.dpi = (yyvsp[(4) - (4)].dval);
                set_device_props(device_id, dev);
            }
            xfree((yyvsp[(2) - (4)].sval));
        }
    break;

  case 197:
#line 2229 "pars.yacc"
    {
            int device_id;
            Device_entry dev;
            
            device_id = get_device_by_name((yyvsp[(2) - (5)].sval));
            if (device_id < 0) {
                yyerror("Unknown device");
            } else {
                dev = get_device_props(device_id);
                dev.fontaa = (yyvsp[(5) - (5)].ival);
                set_device_props(device_id, dev);
            }
            xfree((yyvsp[(2) - (5)].sval));
        }
    break;

  case 198:
#line 2243 "pars.yacc"
    {
            int device_id;
            Device_entry dev;
            
            device_id = get_device_by_name((yyvsp[(2) - (4)].sval));
            if (device_id < 0) {
                yyerror("Unknown device");
            } else {
                dev = get_device_props(device_id);
                dev.devfonts = (yyvsp[(4) - (4)].ival);
                set_device_props(device_id, dev);
            }
            xfree((yyvsp[(2) - (4)].sval));
        }
    break;

  case 199:
#line 2257 "pars.yacc"
    {
            int device_id;
            
            device_id = get_device_by_name((yyvsp[(2) - (4)].sval));
            if (device_id < 0) {
                yyerror("Unknown device");
            } else {
                if (parse_device_options(device_id, (yyvsp[(4) - (4)].sval)) != 
                                                        RETURN_SUCCESS) {
                    yyerror("Incorrect device option string");
                }
            }
            xfree((yyvsp[(2) - (4)].sval));
            xfree((yyvsp[(4) - (4)].sval));
        }
    break;

  case 200:
#line 2272 "pars.yacc"
    {
            set_printer_by_name((yyvsp[(3) - (3)].sval));
            xfree((yyvsp[(3) - (3)].sval));
        }
    break;

  case 201:
#line 2276 "pars.yacc"
    {
            set_ref_date((yyvsp[(3) - (3)].dval));
	}
    break;

  case 202:
#line 2279 "pars.yacc"
    {
            allow_two_digits_years((yyvsp[(3) - (3)].ival));
	}
    break;

  case 203:
#line 2282 "pars.yacc"
    {
            set_wrap_year((yyvsp[(4) - (4)].ival));
	}
    break;

  case 204:
#line 2285 "pars.yacc"
    {
	    setbgcolor((yyvsp[(2) - (2)].ival));
	}
    break;

  case 205:
#line 2288 "pars.yacc"
    {
	    setbgfill((yyvsp[(4) - (4)].ival));
	}
    break;

  case 206:
#line 2291 "pars.yacc"
    {
	    scroll_proc((int) (yyvsp[(3) - (4)].dval));
	}
    break;

  case 207:
#line 2294 "pars.yacc"
    {
	    scrollinout_proc((int) (yyvsp[(3) - (4)].dval));
	}
    break;

  case 208:
#line 2297 "pars.yacc"
    {
	    scrolling_islinked = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 209:
#line 2302 "pars.yacc"
    {
	    add_world(whichgraph, (yyvsp[(3) - (9)].dval), (yyvsp[(5) - (9)].dval), (yyvsp[(7) - (9)].dval), (yyvsp[(9) - (9)].dval));
	}
    break;

  case 210:
#line 2306 "pars.yacc"
    {
            timer_delay = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 211:
#line 2310 "pars.yacc"
    {
	    target_set = *((yyvsp[(2) - (2)].trgt));
	    set_parser_setno(target_set.gno, target_set.setno);
	}
    break;

  case 212:
#line 2314 "pars.yacc"
    {
	    set_parser_gno((yyvsp[(2) - (2)].ival));
	}
    break;

  case 213:
#line 2317 "pars.yacc"
    {
	    set_parser_setno((yyvsp[(2) - (2)].trgt)->gno, (yyvsp[(2) - (2)].trgt)->setno);
	}
    break;

  case 214:
#line 2322 "pars.yacc"
    {
	    set_hotlink((yyvsp[(1) - (4)].trgt)->gno, (yyvsp[(1) - (4)].trgt)->setno, 1, (yyvsp[(4) - (4)].sval), (yyvsp[(3) - (4)].ival));
	    xfree((yyvsp[(4) - (4)].sval));
	}
    break;

  case 215:
#line 2326 "pars.yacc"
    {
	    set_hotlink((yyvsp[(1) - (3)].trgt)->gno, (yyvsp[(1) - (3)].trgt)->setno, (yyvsp[(3) - (3)].ival), NULL, 0);
	}
    break;

  case 216:
#line 2331 "pars.yacc"
    {
	    curbox = next_box();
	}
    break;

  case 217:
#line 2334 "pars.yacc"
    {
            int no = (yyvsp[(3) - (3)].ival);
            if (is_valid_box(no) ||
                realloc_boxes(no + 1) == RETURN_SUCCESS) {
                curbox = no;
            }
	}
    break;

  case 218:
#line 2341 "pars.yacc"
    {
	    if (!is_valid_box(curbox)) {
                yyerror("Box not active");
	    } else {
	        boxes[curbox].active = (yyvsp[(2) - (2)].ival);
            }
	}
    break;

  case 219:
#line 2348 "pars.yacc"
    {
	    if (!is_valid_box(curbox)) {
                yyerror("Box not active");
	    } else {
	        boxes[curbox].gno = (yyvsp[(2) - (2)].ival);
            }
	}
    break;

  case 220:
#line 2355 "pars.yacc"
    {
	    if (!is_valid_box(curbox)) {
                yyerror("Box not active");
	    } else {
		boxes[curbox].x1 = (yyvsp[(2) - (8)].dval);
		boxes[curbox].y1 = (yyvsp[(4) - (8)].dval);
		boxes[curbox].x2 = (yyvsp[(6) - (8)].dval);
		boxes[curbox].y2 = (yyvsp[(8) - (8)].dval);
	    }
	}
    break;

  case 221:
#line 2365 "pars.yacc"
    {
	    box_loctype = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 222:
#line 2368 "pars.yacc"
    {
	    box_lines = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 223:
#line 2371 "pars.yacc"
    {
	    box_linew = (yyvsp[(2) - (2)].dval);
	}
    break;

  case 224:
#line 2374 "pars.yacc"
    {
	    box_color = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 225:
#line 2377 "pars.yacc"
    {
	    box_fillcolor = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 226:
#line 2380 "pars.yacc"
    {
	    box_fillpat = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 227:
#line 2383 "pars.yacc"
    {
	    if (!is_valid_box(curbox)) {
                yyerror("Box not active");
	    } else {
		boxes[curbox].lines = box_lines;
		boxes[curbox].linew = box_linew;
		boxes[curbox].color = box_color;
		if (get_project_version() <= 40102) {
                    switch (filltype_obs) {
                    case COLOR:
                        boxes[curbox].fillcolor = box_fillcolor;
		        boxes[curbox].fillpattern = 1;
                        break;
                    case PATTERN:
                        boxes[curbox].fillcolor = 1;
		        boxes[curbox].fillpattern = box_fillpat;
                        break;
                    default: /* NONE */
                        boxes[curbox].fillcolor = box_fillcolor;
		        boxes[curbox].fillpattern = 0;
                        break;
                    }
		} else {
                    boxes[curbox].fillcolor = box_fillcolor;
		    boxes[curbox].fillpattern = box_fillpat;
                }
                boxes[curbox].loctype = box_loctype;
	    }
	}
    break;

  case 228:
#line 2414 "pars.yacc"
    {
		curellipse = next_ellipse();
	}
    break;

  case 229:
#line 2417 "pars.yacc"
    {
            int no = (yyvsp[(3) - (3)].ival);
            if (is_valid_ellipse(no) ||
                realloc_ellipses(no + 1) == RETURN_SUCCESS) {
                curellipse = no;
            }
	}
    break;

  case 230:
#line 2424 "pars.yacc"
    {
	    if (!is_valid_ellipse(curellipse)) {
                yyerror("Ellipse not active");
	    } else {
	        ellip[curellipse].active = (yyvsp[(2) - (2)].ival);
            }
	}
    break;

  case 231:
#line 2431 "pars.yacc"
    {
	    if (!is_valid_ellipse(curellipse)) {
                yyerror("Ellipse not active");
	    } else {
	        ellip[curellipse].gno = (yyvsp[(2) - (2)].ival);
            }
	}
    break;

  case 232:
#line 2438 "pars.yacc"
    {
	    if (!is_valid_ellipse(curellipse)) {
                yyerror("Ellipse not active");
	    } else {
		ellip[curellipse].x1 = (yyvsp[(2) - (8)].dval);
		ellip[curellipse].y1 = (yyvsp[(4) - (8)].dval);
		ellip[curellipse].x2 = (yyvsp[(6) - (8)].dval);
		ellip[curellipse].y2 = (yyvsp[(8) - (8)].dval);
	    }
	}
    break;

  case 233:
#line 2448 "pars.yacc"
    {
	    ellipse_loctype = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 234:
#line 2451 "pars.yacc"
    {
	    ellipse_lines = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 235:
#line 2454 "pars.yacc"
    {
	    ellipse_linew = (yyvsp[(2) - (2)].dval);
	}
    break;

  case 236:
#line 2457 "pars.yacc"
    {
	    ellipse_color = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 237:
#line 2460 "pars.yacc"
    {
	    ellipse_fillcolor = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 238:
#line 2463 "pars.yacc"
    {
	    ellipse_fillpat = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 239:
#line 2466 "pars.yacc"
    {
	    if (!is_valid_ellipse(curellipse)) {
                yyerror("Ellipse not active");
	    } else {
		ellip[curellipse].lines = ellipse_lines;
		ellip[curellipse].linew = ellipse_linew;
		ellip[curellipse].color = ellipse_color;
		if (get_project_version() <= 40102) {
                    switch (filltype_obs) {
                    case COLOR:
                        ellip[curellipse].fillcolor = ellipse_fillcolor;
		        ellip[curellipse].fillpattern = 1;
                        break;
                    case PATTERN:
                        ellip[curellipse].fillcolor = 1;
		        ellip[curellipse].fillpattern = ellipse_fillpat;
                        break;
                    default: /* NONE */
                        ellip[curellipse].fillcolor = ellipse_fillcolor;
		        ellip[curellipse].fillpattern = 0;
                        break;
                    }
		} else {
                    ellip[curellipse].fillcolor = ellipse_fillcolor;
		    ellip[curellipse].fillpattern = ellipse_fillpat;
                }
		ellip[curellipse].loctype = ellipse_loctype;
	    }
	}
    break;

  case 240:
#line 2497 "pars.yacc"
    {
	    curline = next_line();
	}
    break;

  case 241:
#line 2500 "pars.yacc"
    {
            int no = (yyvsp[(3) - (3)].ival);
            if (is_valid_line(no) ||
                realloc_lines(no + 1) == RETURN_SUCCESS) {
                curline = no;
            }
	}
    break;

  case 242:
#line 2507 "pars.yacc"
    {
	    if (!is_valid_line(curline)) {
                yyerror("Line not active");
	    } else {
	        lines[curline].active = (yyvsp[(2) - (2)].ival);
            }
	}
    break;

  case 243:
#line 2514 "pars.yacc"
    {
	    if (!is_valid_line(curline)) {
                yyerror("Line not active");
	    } else {
	        lines[curline].gno = (yyvsp[(2) - (2)].ival);
            }
	}
    break;

  case 244:
#line 2521 "pars.yacc"
    {
	    if (!is_valid_line(curline)) {
                yyerror("Line not active");
	    } else {
	        lines[curline].x1 = (yyvsp[(2) - (8)].dval);
	        lines[curline].y1 = (yyvsp[(4) - (8)].dval);
	        lines[curline].x2 = (yyvsp[(6) - (8)].dval);
	        lines[curline].y2 = (yyvsp[(8) - (8)].dval);
            }
	}
    break;

  case 245:
#line 2531 "pars.yacc"
    {
	    line_loctype = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 246:
#line 2534 "pars.yacc"
    {
	    line_linew = (yyvsp[(2) - (2)].dval);
	}
    break;

  case 247:
#line 2537 "pars.yacc"
    {
	    line_lines = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 248:
#line 2540 "pars.yacc"
    {
	    line_color = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 249:
#line 2543 "pars.yacc"
    {
	    line_arrow_end = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 250:
#line 2546 "pars.yacc"
    {
	    line_asize = (yyvsp[(4) - (4)].dval);
	}
    break;

  case 251:
#line 2549 "pars.yacc"
    {
	    line_atype = (yyvsp[(4) - (4)].ival);
	}
    break;

  case 252:
#line 2552 "pars.yacc"
    {
	    line_a_dL_ff = (yyvsp[(4) - (6)].dval);
	    line_a_lL_ff = (yyvsp[(6) - (6)].dval);
	}
    break;

  case 253:
#line 2556 "pars.yacc"
    {
	    if (!is_valid_line(curline)) {
                yyerror("Line not active");
	    } else {
	        lines[curline].lines = line_lines;
	        lines[curline].linew = line_linew;
	        lines[curline].color = line_color;
	        lines[curline].arrow_end = line_arrow_end;
	        lines[curline].arrow.length = line_asize;
	        lines[curline].arrow.type = line_atype;
	        lines[curline].arrow.dL_ff = line_a_dL_ff;
	        lines[curline].arrow.lL_ff = line_a_lL_ff;
	        lines[curline].loctype = line_loctype;
            }
	}
    break;

  case 254:
#line 2573 "pars.yacc"
    {
            curstring = next_string();
        }
    break;

  case 255:
#line 2576 "pars.yacc"
    {
            int no = (yyvsp[(3) - (3)].ival);
            if (is_valid_string(no) ||
                realloc_strings(no + 1) == RETURN_SUCCESS) {
                curstring = no;
            }
        }
    break;

  case 256:
#line 2583 "pars.yacc"
    {
	    if (!is_valid_string(curstring)) {
                yyerror("String not active");
	    } else {
                pstr[curstring].active = (yyvsp[(2) - (2)].ival);
            }
        }
    break;

  case 257:
#line 2590 "pars.yacc"
    {
	    if (!is_valid_string(curstring)) {
                yyerror("String not active");
	    } else {
                pstr[curstring].gno = (yyvsp[(2) - (2)].ival);
            }
        }
    break;

  case 258:
#line 2597 "pars.yacc"
    {
	    if (!is_valid_string(curstring)) {
                yyerror("String not active");
	    } else {
	        pstr[curstring].x = (yyvsp[(2) - (4)].dval);
	        pstr[curstring].y = (yyvsp[(4) - (4)].dval);
            }
	}
    break;

  case 259:
#line 2605 "pars.yacc"
    {
            string_loctype = (yyvsp[(3) - (3)].ival);
        }
    break;

  case 260:
#line 2608 "pars.yacc"
    {
            string_color = (yyvsp[(2) - (2)].ival);
        }
    break;

  case 261:
#line 2611 "pars.yacc"
    {
            string_rot = (yyvsp[(3) - (3)].ival);
        }
    break;

  case 262:
#line 2614 "pars.yacc"
    {
            string_font = (yyvsp[(2) - (2)].ival);
        }
    break;

  case 263:
#line 2617 "pars.yacc"
    {
            string_just = (yyvsp[(3) - (3)].ival);
        }
    break;

  case 264:
#line 2620 "pars.yacc"
    {
            string_size = (yyvsp[(4) - (4)].dval);
        }
    break;

  case 265:
#line 2623 "pars.yacc"
    {
	    if (!is_valid_string(curstring)) {
                yyerror("String not active");
	    } else {
	        set_plotstr_string(&pstr[curstring], (yyvsp[(3) - (3)].sval));
	        pstr[curstring].color = string_color;
	        pstr[curstring].font = string_font;
	        pstr[curstring].just = string_just;
	        pstr[curstring].loctype = string_loctype;
	        pstr[curstring].rot = string_rot;
	        pstr[curstring].charsize = string_size;
            }
	    xfree((yyvsp[(3) - (3)].sval));
	}
    break;

  case 266:
#line 2639 "pars.yacc"
    {
            timestamp.active = (yyvsp[(2) - (2)].ival);
        }
    break;

  case 267:
#line 2642 "pars.yacc"
    {
            timestamp.font = (yyvsp[(2) - (2)].ival);
        }
    break;

  case 268:
#line 2645 "pars.yacc"
    {
            timestamp.charsize = (yyvsp[(4) - (4)].dval);
        }
    break;

  case 269:
#line 2648 "pars.yacc"
    {
            timestamp.rot = (yyvsp[(3) - (3)].ival);
        }
    break;

  case 270:
#line 2651 "pars.yacc"
    {
            timestamp.color = (yyvsp[(2) - (2)].ival);
        }
    break;

  case 271:
#line 2654 "pars.yacc"
    {
	    timestamp.x = (yyvsp[(2) - (4)].dval);
	    timestamp.y = (yyvsp[(4) - (4)].dval);
	}
    break;

  case 272:
#line 2658 "pars.yacc"
    {
	  set_plotstr_string(&timestamp, (yyvsp[(3) - (3)].sval));
	  xfree((yyvsp[(3) - (3)].sval));
	}
    break;

  case 273:
#line 2664 "pars.yacc"
    {
	    grdefaults.lines = (yyvsp[(2) - (2)].ival);
	    box_lines = ellipse_lines = line_lines = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 274:
#line 2668 "pars.yacc"
    {
	    grdefaults.linew = (yyvsp[(2) - (2)].dval);
	    box_linew = ellipse_linew = line_linew = (yyvsp[(2) - (2)].dval);
	}
    break;

  case 275:
#line 2672 "pars.yacc"
    {
	    grdefaults.color = (yyvsp[(2) - (2)].ival);
	    box_color = ellipse_color = line_color = string_color = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 276:
#line 2676 "pars.yacc"
    {
	    grdefaults.pattern = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 277:
#line 2679 "pars.yacc"
    {
	    grdefaults.charsize = (yyvsp[(4) - (4)].dval);
	    string_size = (yyvsp[(4) - (4)].dval);
	}
    break;

  case 278:
#line 2683 "pars.yacc"
    {
	    grdefaults.font = (yyvsp[(2) - (2)].ival);
	    string_font = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 279:
#line 2687 "pars.yacc"
    {
	    grdefaults.symsize = (yyvsp[(4) - (4)].dval);
	}
    break;

  case 280:
#line 2690 "pars.yacc"
    {
	    strcpy(sformat, (yyvsp[(3) - (3)].sval));
	    xfree((yyvsp[(3) - (3)].sval));
	}
    break;

  case 281:
#line 2694 "pars.yacc"
    {
	    if ((map_font_by_name((yyvsp[(5) - (7)].sval), (yyvsp[(3) - (7)].ival)) != RETURN_SUCCESS) && 
                (map_font_by_name((yyvsp[(7) - (7)].sval), (yyvsp[(3) - (7)].ival)) != RETURN_SUCCESS)) {
                errmsg("Failed mapping a font");
            }
            xfree((yyvsp[(5) - (7)].sval));
	    xfree((yyvsp[(7) - (7)].sval));
	}
    break;

  case 282:
#line 2702 "pars.yacc"
    {
	    CMap_entry cmap;
            cmap.rgb.red   = (yyvsp[(6) - (13)].ival);
            cmap.rgb.green = (yyvsp[(8) - (13)].ival);
            cmap.rgb.blue  = (yyvsp[(10) - (13)].ival);
            cmap.ctype = COLOR_MAIN;
            cmap.cname = (yyvsp[(13) - (13)].sval);
            if (store_color((yyvsp[(3) - (13)].ival), cmap) == RETURN_FAILURE) {
                errmsg("Failed mapping a color");
            }
	    xfree((yyvsp[(13) - (13)].sval));
        }
    break;

  case 283:
#line 2715 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].w.xg1 = (yyvsp[(2) - (8)].dval);
	    g[whichgraph].w.yg1 = (yyvsp[(4) - (8)].dval);
	    g[whichgraph].w.xg2 = (yyvsp[(6) - (8)].dval);
	    g[whichgraph].w.yg2 = (yyvsp[(8) - (8)].dval);
	}
    break;

  case 284:
#line 2725 "pars.yacc"
    {
	    set_graph_znorm(whichgraph, (yyvsp[(2) - (2)].dval));
	}
    break;

  case 285:
#line 2728 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].v.xv1 = (yyvsp[(2) - (8)].dval);
	    g[whichgraph].v.yv1 = (yyvsp[(4) - (8)].dval);
	    g[whichgraph].v.xv2 = (yyvsp[(6) - (8)].dval);
	    g[whichgraph].v.yv2 = (yyvsp[(8) - (8)].dval);
	}
    break;

  case 286:
#line 2738 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    set_plotstr_string(&g[whichgraph].labs.title, (yyvsp[(2) - (2)].sval));
	    xfree((yyvsp[(2) - (2)].sval));
	}
    break;

  case 287:
#line 2746 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].labs.title.font = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 288:
#line 2753 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].labs.title.charsize = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 289:
#line 2760 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].labs.title.color = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 290:
#line 2767 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    set_plotstr_string(&g[whichgraph].labs.stitle, (yyvsp[(2) - (2)].sval));
	    xfree((yyvsp[(2) - (2)].sval));
	}
    break;

  case 291:
#line 2775 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].labs.stitle.font = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 292:
#line 2782 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].labs.stitle.charsize = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 293:
#line 2789 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].labs.stitle.color = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 294:
#line 2797 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].xscale = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 295:
#line 2804 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].yscale = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 296:
#line 2811 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].xinvert = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 297:
#line 2818 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].yinvert = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 298:
#line 2825 "pars.yacc"
    {
            autoscale_onread = AUTOSCALE_NONE;
        }
    break;

  case 299:
#line 2828 "pars.yacc"
    {
            autoscale_onread = AUTOSCALE_X;
        }
    break;

  case 300:
#line 2831 "pars.yacc"
    {
            autoscale_onread = AUTOSCALE_Y;
        }
    break;

  case 301:
#line 2834 "pars.yacc"
    {
            autoscale_onread = AUTOSCALE_XY;
        }
    break;

  case 302:
#line 2838 "pars.yacc"
    {
            char *s;
            s = copy_string(NULL, get_project_description());
            s = concat_strings(s, (yyvsp[(2) - (2)].sval));
	    xfree((yyvsp[(2) - (2)].sval));
            s = concat_strings(s, "\n");
            set_project_description(s);
            xfree(s);
	}
    break;

  case 303:
#line 2847 "pars.yacc"
    {
            set_project_description(NULL);
        }
    break;

  case 304:
#line 2851 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.active = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 305:
#line 2858 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.loctype = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 306:
#line 2865 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
            g[whichgraph].l.vgap = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 307:
#line 2872 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.hgap = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 308:
#line 2879 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.len = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 309:
#line 2886 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.invert = (yyvsp[(3) - (3)].ival);
        }
    break;

  case 310:
#line 2893 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.boxfillpen.color = (yyvsp[(4) - (4)].ival);
        }
    break;

  case 311:
#line 2900 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.boxfillpen.pattern = (yyvsp[(4) - (4)].ival);
        }
    break;

  case 312:
#line 2907 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.boxpen.color = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 313:
#line 2914 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.boxpen.pattern = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 314:
#line 2921 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.boxlines = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 315:
#line 2928 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.boxlinew = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 316:
#line 2935 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.legx = (yyvsp[(2) - (4)].dval);
	    g[whichgraph].l.legy = (yyvsp[(4) - (4)].dval);
	}
    break;

  case 317:
#line 2943 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.charsize = (yyvsp[(4) - (4)].dval);
	}
    break;

  case 318:
#line 2950 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.font = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 319:
#line 2957 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.color = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 320:
#line 2965 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
            g[whichgraph].f.pen.pattern = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 321:
#line 2972 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].f.type = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 322:
#line 2979 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].f.lines = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 323:
#line 2986 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].f.linew = (yyvsp[(2) - (2)].dval);
	}
    break;

  case 324:
#line 2993 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].f.pen.color = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 325:
#line 3000 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].f.pen.pattern = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 326:
#line 3008 "pars.yacc"
    { 
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
            g[whichgraph].f.fillpen.color = (yyvsp[(3) - (3)].ival);
        }
    break;

  case 327:
#line 3016 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
            g[whichgraph].f.fillpen.pattern = (yyvsp[(3) - (3)].ival);
        }
    break;

  case 328:
#line 3024 "pars.yacc"
    {
            set_graph_hidden((yyvsp[(1) - (2)].ival), !(yyvsp[(2) - (2)].ival));
        }
    break;

  case 329:
#line 3027 "pars.yacc"
    {
            set_graph_hidden((yyvsp[(1) - (3)].ival), (yyvsp[(3) - (3)].ival));
        }
    break;

  case 330:
#line 3030 "pars.yacc"
    {
            set_graph_type((yyvsp[(1) - (3)].ival), (yyvsp[(3) - (3)].ival));
        }
    break;

  case 331:
#line 3033 "pars.yacc"
    {
            set_graph_stacked((yyvsp[(1) - (3)].ival), (yyvsp[(3) - (3)].ival));
        }
    break;

  case 332:
#line 3037 "pars.yacc"
    {
	    set_graph_bargap((yyvsp[(1) - (4)].ival), (yyvsp[(4) - (4)].dval));
	}
    break;

  case 333:
#line 3041 "pars.yacc"
    {
            g[(yyvsp[(1) - (3)].ival)].locator.pointset = (yyvsp[(3) - (3)].ival);
        }
    break;

  case 334:
#line 3044 "pars.yacc"
    {
	    g[(yyvsp[(1) - (5)].ival)].locator.fx = (yyvsp[(4) - (5)].ival);
	    g[(yyvsp[(1) - (5)].ival)].locator.fy = (yyvsp[(5) - (5)].ival);
	}
    break;

  case 335:
#line 3048 "pars.yacc"
    {
	    g[(yyvsp[(1) - (6)].ival)].locator.px = (yyvsp[(4) - (6)].dval);
	    g[(yyvsp[(1) - (6)].ival)].locator.py = (yyvsp[(6) - (6)].dval);
	}
    break;

  case 336:
#line 3052 "pars.yacc"
    {
	    g[(yyvsp[(1) - (6)].ival)].locator.dsx = (yyvsp[(4) - (6)].dval);
	    g[(yyvsp[(1) - (6)].ival)].locator.dsy = (yyvsp[(6) - (6)].dval);
	}
    break;

  case 337:
#line 3056 "pars.yacc"
    {
            g[(yyvsp[(1) - (4)].ival)].locator.pt_type = (yyvsp[(4) - (4)].ival);
        }
    break;

  case 338:
#line 3060 "pars.yacc"
    {
	    curtype = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 339:
#line 3065 "pars.yacc"
    {
	    if (add_io_filter((yyvsp[(2) - (5)].ival), (yyvsp[(4) - (5)].ival), (yyvsp[(5) - (5)].sval), (yyvsp[(3) - (5)].sval)) != 0) {
	        yyerror("Failed adding i/o filter");
	    }
	    xfree((yyvsp[(3) - (5)].sval));
	    xfree((yyvsp[(5) - (5)].sval));
	}
    break;

  case 340:
#line 3072 "pars.yacc"
    {
	    clear_io_filters((yyvsp[(2) - (2)].ival));
	}
    break;

  case 341:
#line 3076 "pars.yacc"
    {
	    cursource = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 342:
#line 3079 "pars.yacc"
    {
	    readxformat = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 343:
#line 3082 "pars.yacc"
    { }
    break;

  case 344:
#line 3083 "pars.yacc"
    {
	    nonl_parms[(yyvsp[(1) - (3)].ival)].constr = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 345:
#line 3089 "pars.yacc"
    {
	    drawgraph();
	}
    break;

  case 346:
#line 3092 "pars.yacc"
    {
#ifndef NONE_GUI
            if (inwin) {
///                update_all();
            }
#endif
        }
    break;

  case 347:
#line 3099 "pars.yacc"
    {
	    set_workingdir((yyvsp[(2) - (2)].sval));
	    xfree((yyvsp[(2) - (2)].sval));
	}
    break;

  case 348:
#line 3103 "pars.yacc"
    {
	    echomsg((yyvsp[(2) - (2)].sval));
	    xfree((yyvsp[(2) - (2)].sval));
	}
    break;

  case 349:
#line 3107 "pars.yacc"
    {
	    char buf[32];
            set_locale_num(TRUE);
            sprintf(buf, "%g", (yyvsp[(2) - (2)].dval));
            set_locale_num(FALSE);
            echomsg(buf);
	}
    break;

  case 350:
#line 3114 "pars.yacc"
    {
	    close_input = copy_string(close_input, "");
	}
    break;

  case 351:
#line 3117 "pars.yacc"
    {
	    close_input = copy_string(close_input, (yyvsp[(2) - (2)].sval));
	}
    break;

  case 352:
#line 3120 "pars.yacc"
    {
	    exit(0);
	}
    break;

  case 353:
#line 3123 "pars.yacc"
    {
	    exit((yyvsp[(3) - (4)].ival));
	}
    break;

  case 354:
#line 3126 "pars.yacc"
    {
	    if (!safe_mode) {
                do_hardcopy();
            } else {
                yyerror("File modifications are disabled in safe mode");
            }
	}
    break;

  case 355:
#line 3133 "pars.yacc"
    {
            set_ptofile(FALSE);
	}
    break;

  case 356:
#line 3136 "pars.yacc"
    {
            set_ptofile(TRUE);
	    strcpy(print_file, (yyvsp[(3) - (3)].sval));
            xfree((yyvsp[(3) - (3)].sval));
	}
    break;

  case 357:
#line 3141 "pars.yacc"
    {
	    switch ((yyvsp[(2) - (2)].ival)) {
	    case UP:
		graph_scroll(GSCROLL_UP);
		break;
	    case DOWN:
		graph_scroll(GSCROLL_DOWN);
		break;
	    case RIGHT:
		graph_scroll(GSCROLL_RIGHT);
		break;
	    case LEFT:
		graph_scroll(GSCROLL_LEFT);
		break;
	    case IN:
		graph_zoom(GZOOM_SHRINK);
		break;
	    case OUT:
		graph_zoom(GZOOM_EXPAND);
		break;
	    }
	}
    break;

  case 358:
#line 3163 "pars.yacc"
    {
	    if ((yyvsp[(2) - (2)].dval) > 0) {
	        msleep_wrap((unsigned int) (1000 * (yyvsp[(2) - (2)].dval)));
	    }
	}
    break;

  case 359:
#line 3168 "pars.yacc"
    {
#ifndef NONE_GUI
            if (inwin) {
///                HelpCB($2);
            }
            xfree((yyvsp[(2) - (2)].sval));
#endif
	}
    break;

  case 360:
#line 3176 "pars.yacc"
    {
#ifndef NONE_GUI
            if (inwin) {
///                HelpCB("doc/UsersGuide.html");
            }
#endif
	}
    break;

  case 361:
#line 3183 "pars.yacc"
    {
	    gotparams = TRUE;
	    strcpy(paramfile, (yyvsp[(2) - (2)].sval));
	    xfree((yyvsp[(2) - (2)].sval));
	}
    break;

  case 362:
#line 3188 "pars.yacc"
    {
	    if (!safe_mode) {
                FILE *pp = grace_openw((yyvsp[(2) - (2)].sval));
	        if (pp != NULL) {
	            putparms(whichgraph, pp, 0);
	            grace_close(pp);
	        }
            } else {
                yyerror("File modifications are disabled in safe mode");
            }
	    xfree((yyvsp[(2) - (2)].sval));
	}
    break;

  case 363:
#line 3200 "pars.yacc"
    {
	    set_set_hidden((yyvsp[(1) - (3)].trgt)->gno, (yyvsp[(1) - (3)].trgt)->setno, (yyvsp[(3) - (3)].ival));
	}
    break;

  case 364:
#line 3203 "pars.yacc"
    {
	    setlength((yyvsp[(1) - (3)].trgt)->gno, (yyvsp[(1) - (3)].trgt)->setno, (yyvsp[(3) - (3)].ival));
	}
    break;

  case 365:
#line 3206 "pars.yacc"
    {
	    realloc_vrbl((yyvsp[(1) - (3)].vrbl), (yyvsp[(3) - (3)].ival));
	}
    break;

  case 366:
#line 3209 "pars.yacc"
    {
	    add_point((yyvsp[(1) - (5)].trgt)->gno, (yyvsp[(1) - (5)].trgt)->setno, (yyvsp[(3) - (5)].dval), (yyvsp[(5) - (5)].dval));
	}
    break;

  case 367:
#line 3213 "pars.yacc"
    {
	    int start = (yyvsp[(3) - (5)].ival) - index_shift;
	    int stop = (yyvsp[(5) - (5)].ival) - index_shift;
	    droppoints((yyvsp[(1) - (5)].trgt)->gno, (yyvsp[(1) - (5)].trgt)->setno, start, stop);
	}
    break;

  case 368:
#line 3218 "pars.yacc"
    {
	    if (is_set_active((yyvsp[(2) - (4)].trgt)->gno, (yyvsp[(2) - (4)].trgt)->setno)) {
	        sortset((yyvsp[(2) - (4)].trgt)->gno, (yyvsp[(2) - (4)].trgt)->setno, (yyvsp[(3) - (4)].ival), (yyvsp[(4) - (4)].ival) == ASCENDING ? 0 : 1);
	    }
	}
    break;

  case 369:
#line 3223 "pars.yacc"
    {
	    do_copyset((yyvsp[(2) - (4)].trgt)->gno, (yyvsp[(2) - (4)].trgt)->setno, (yyvsp[(4) - (4)].trgt)->gno, (yyvsp[(4) - (4)].trgt)->setno);
	}
    break;

  case 370:
#line 3226 "pars.yacc"
    {
	    if ((yyvsp[(2) - (4)].trgt)->gno != (yyvsp[(4) - (4)].trgt)->gno) {
                errmsg("Can't append sets from different graphs");
            } else {
                int sets[2];
	        sets[0] = (yyvsp[(4) - (4)].trgt)->setno;
	        sets[1] = (yyvsp[(2) - (4)].trgt)->setno;
	        join_sets((yyvsp[(2) - (4)].trgt)->gno, sets, 2);
            }
	}
    break;

  case 371:
#line 3236 "pars.yacc"
    {
            reverse_set((yyvsp[(2) - (2)].trgt)->gno, (yyvsp[(2) - (2)].trgt)->setno);
	}
    break;

  case 372:
#line 3239 "pars.yacc"
    {
            do_splitsets((yyvsp[(2) - (3)].trgt)->gno, (yyvsp[(2) - (3)].trgt)->setno, (yyvsp[(3) - (3)].ival));
	}
    break;

  case 373:
#line 3242 "pars.yacc"
    {
	    do_moveset((yyvsp[(2) - (4)].trgt)->gno, (yyvsp[(2) - (4)].trgt)->setno, (yyvsp[(4) - (4)].trgt)->gno, (yyvsp[(4) - (4)].trgt)->setno);
	}
    break;

  case 374:
#line 3245 "pars.yacc"
    {
	    do_swapset((yyvsp[(2) - (4)].trgt)->gno, (yyvsp[(2) - (4)].trgt)->setno, (yyvsp[(4) - (4)].trgt)->gno, (yyvsp[(4) - (4)].trgt)->setno);
	}
    break;

  case 375:
#line 3248 "pars.yacc"
    {
	    killset((yyvsp[(2) - (2)].trgt)->gno, (yyvsp[(2) - (2)].trgt)->setno);
	}
    break;

  case 376:
#line 3251 "pars.yacc"
    {
            killsetdata((yyvsp[(2) - (3)].trgt)->gno, (yyvsp[(2) - (3)].trgt)->setno);
        }
    break;

  case 377:
#line 3254 "pars.yacc"
    {
            kill_graph((yyvsp[(2) - (2)].ival));
        }
    break;

  case 378:
#line 3257 "pars.yacc"
    {
            kill_region((yyvsp[(2) - (2)].ival));
        }
    break;

  case 379:
#line 3260 "pars.yacc"
    {
            wipeout();
        }
    break;

  case 380:
#line 3263 "pars.yacc"
    {
            arrange_graphs_simple((yyvsp[(3) - (12)].ival), (yyvsp[(5) - (12)].ival), 0, FALSE, (yyvsp[(7) - (12)].dval), (yyvsp[(9) - (12)].dval), (yyvsp[(11) - (12)].dval));
        }
    break;

  case 381:
#line 3266 "pars.yacc"
    {
            int order = ((yyvsp[(13) - (18)].ival) * GA_ORDER_HV_INV) |
                        ((yyvsp[(15) - (18)].ival) * GA_ORDER_H_INV ) |
                        ((yyvsp[(17) - (18)].ival) * GA_ORDER_V_INV );
            arrange_graphs_simple((yyvsp[(3) - (18)].ival), (yyvsp[(5) - (18)].ival), order, FALSE, (yyvsp[(7) - (18)].dval), (yyvsp[(9) - (18)].dval), (yyvsp[(11) - (18)].dval));
        }
    break;

  case 382:
#line 3272 "pars.yacc"
    {
            int order = ((yyvsp[(13) - (20)].ival) * GA_ORDER_HV_INV) |
                        ((yyvsp[(15) - (20)].ival) * GA_ORDER_H_INV ) |
                        ((yyvsp[(17) - (20)].ival) * GA_ORDER_V_INV );
            arrange_graphs_simple((yyvsp[(3) - (20)].ival), (yyvsp[(5) - (20)].ival), order, (yyvsp[(19) - (20)].ival), (yyvsp[(7) - (20)].dval), (yyvsp[(9) - (20)].dval), (yyvsp[(11) - (20)].dval));
        }
    break;

  case 383:
#line 3278 "pars.yacc"
    {
	    gotnlfit = TRUE;
	    nlfit_gno = (yyvsp[(3) - (6)].trgt)->gno;
	    nlfit_setno = (yyvsp[(3) - (6)].trgt)->setno;
	    nlfit_nsteps = (yyvsp[(5) - (6)].ival);
	    nlfit_warray = NULL;
	}
    break;

  case 384:
#line 3285 "pars.yacc"
    {
	    if (getsetlength((yyvsp[(3) - (8)].trgt)->gno, (yyvsp[(3) - (8)].trgt)->setno) != (yyvsp[(5) - (8)].vrbl)->length) {
                errmsg("Data and weight arrays are of different lengths");
                return 1;
            } else {
	        gotnlfit = TRUE;
	        nlfit_gno = (yyvsp[(3) - (8)].trgt)->gno;
	        nlfit_setno = (yyvsp[(3) - (8)].trgt)->setno;
	        nlfit_nsteps = (yyvsp[(7) - (8)].ival);
	        nlfit_warray = copy_data_column((yyvsp[(5) - (8)].vrbl)->data, (yyvsp[(5) - (8)].vrbl)->length);
            }
	}
    break;

  case 385:
#line 3297 "pars.yacc"
    {
	    do_regress((yyvsp[(3) - (6)].trgt)->gno, (yyvsp[(3) - (6)].trgt)->setno, (yyvsp[(5) - (6)].ival), 0, -1, 0, -1);
	}
    break;

  case 386:
#line 3300 "pars.yacc"
    {
	    do_runavg((yyvsp[(3) - (6)].trgt)->gno, (yyvsp[(3) - (6)].trgt)->setno, (yyvsp[(5) - (6)].ival), (yyvsp[(1) - (6)].ival), -1, 0);
	}
    break;

  case 387:
#line 3303 "pars.yacc"
    {
	    do_fourier_command((yyvsp[(3) - (6)].trgt)->gno, (yyvsp[(3) - (6)].trgt)->setno, (yyvsp[(1) - (6)].ival), (yyvsp[(5) - (6)].ival));
	}
    break;

  case 388:
#line 3307 "pars.yacc"
    {
	    switch ((yyvsp[(1) - (12)].ival)) {
	    case FFT_DFT:
                do_fourier((yyvsp[(3) - (12)].trgt)->gno, (yyvsp[(3) - (12)].trgt)->setno, 0, (yyvsp[(11) - (12)].ival), (yyvsp[(9) - (12)].ival), 0, (yyvsp[(5) - (12)].ival), (yyvsp[(7) - (12)].ival));
	        break;
	    case FFT_INVDFT    :
                do_fourier((yyvsp[(3) - (12)].trgt)->gno, (yyvsp[(3) - (12)].trgt)->setno, 0, (yyvsp[(11) - (12)].ival), (yyvsp[(9) - (12)].ival), 1, (yyvsp[(5) - (12)].ival), (yyvsp[(7) - (12)].ival));
	        break;
	    case FFT_FFT:
                do_fourier((yyvsp[(3) - (12)].trgt)->gno, (yyvsp[(3) - (12)].trgt)->setno, 1, (yyvsp[(11) - (12)].ival), (yyvsp[(9) - (12)].ival), 0, (yyvsp[(5) - (12)].ival), (yyvsp[(7) - (12)].ival));
	        break;
	    case FFT_INVFFT    :
                do_fourier((yyvsp[(3) - (12)].trgt)->gno, (yyvsp[(3) - (12)].trgt)->setno, 1, (yyvsp[(11) - (12)].ival), (yyvsp[(9) - (12)].ival), 1, (yyvsp[(5) - (12)].ival), (yyvsp[(7) - (12)].ival));
	        break;
	    default:
                errmsg("Internal error");
	        break;
	    }
        }
    break;

  case 389:
#line 3326 "pars.yacc"
    {
            do_interp((yyvsp[(3) - (10)].trgt)->gno, (yyvsp[(3) - (10)].trgt)->setno, get_cg(), SET_SELECT_NEXT,
                (yyvsp[(5) - (10)].vrbl)->data, (yyvsp[(5) - (10)].vrbl)->length, (yyvsp[(7) - (10)].ival), (yyvsp[(9) - (10)].ival));
	}
    break;

  case 390:
#line 3330 "pars.yacc"
    {
            do_histo((yyvsp[(3) - (10)].trgt)->gno, (yyvsp[(3) - (10)].trgt)->setno, get_cg(), SET_SELECT_NEXT,
                (yyvsp[(5) - (10)].vrbl)->data, (yyvsp[(5) - (10)].vrbl)->length - 1, (yyvsp[(7) - (10)].ival), (yyvsp[(9) - (10)].ival));
	}
    break;

  case 391:
#line 3334 "pars.yacc"
    {
	    do_differ((yyvsp[(3) - (6)].trgt)->gno, (yyvsp[(3) - (6)].trgt)->setno, (yyvsp[(5) - (6)].ival));
	}
    break;

  case 392:
#line 3337 "pars.yacc"
    {
	    do_int((yyvsp[(3) - (4)].trgt)->gno, (yyvsp[(3) - (4)].trgt)->setno, 0);
	}
    break;

  case 393:
#line 3340 "pars.yacc"
    {
	    do_xcor((yyvsp[(3) - (10)].trgt)->gno, (yyvsp[(3) - (10)].trgt)->setno, (yyvsp[(5) - (10)].trgt)->gno, (yyvsp[(5) - (10)].trgt)->setno, (yyvsp[(7) - (10)].ival), (yyvsp[(9) - (10)].ival));
	}
    break;

  case 394:
#line 3343 "pars.yacc"
    {
	    do_linearc((yyvsp[(3) - (6)].trgt)->gno, (yyvsp[(3) - (6)].trgt)->setno, (yyvsp[(5) - (6)].trgt)->gno, (yyvsp[(5) - (6)].trgt)->setno);
	}
    break;

  case 395:
#line 3346 "pars.yacc"
    {
            int len = getsetlength((yyvsp[(3) - (6)].trgt)->gno, (yyvsp[(3) - (6)].trgt)->setno);
            if (len != (yyvsp[(5) - (6)].vrbl)->length) {
		errmsg("Filter expression is of a wrong length");
            } else {
                char *rarray;
                rarray = (char*)xmalloc(len*sizeof(char));
                if (rarray) {
                    int i;
                    for (i = 0; i < len; i++) {
                        rarray[i] = CAST_DBL_TO_BOOL((yyvsp[(5) - (6)].vrbl)->data[i]);
                    }
                    filter_set((yyvsp[(3) - (6)].trgt)->gno, (yyvsp[(3) - (6)].trgt)->setno, rarray);
                    xfree(rarray);
                }
            }
	}
    break;

  case 396:
#line 3363 "pars.yacc"
    {
            int rtype;
            char *rarray;
            
            rtype = RESTRICT_REG0 + (yyvsp[(5) - (8)].ival);

	    if (get_restriction_array((yyvsp[(3) - (8)].trgt)->gno, (yyvsp[(3) - (8)].trgt)->setno,
                rtype, (yyvsp[(7) - (8)].ival), &rarray) != RETURN_SUCCESS) {
                errmsg("Error in region evaluation");
                return 1;
	    } else {
                filter_set((yyvsp[(3) - (8)].trgt)->gno, (yyvsp[(3) - (8)].trgt)->setno, rarray);
                xfree(rarray);
            }
	}
    break;

  case 397:
#line 3378 "pars.yacc"
    {
	    if (autoscale_graph(whichgraph, AUTOSCALE_XY) != RETURN_SUCCESS) {
		errmsg("Can't autoscale (no active sets?)");
	    }
	}
    break;

  case 398:
#line 3383 "pars.yacc"
    {
	    if (autoscale_graph(whichgraph, AUTOSCALE_X) != RETURN_SUCCESS) {
		errmsg("Can't autoscale (no active sets?)");
	    }
	}
    break;

  case 399:
#line 3388 "pars.yacc"
    {
	    if (autoscale_graph(whichgraph, AUTOSCALE_Y) != RETURN_SUCCESS) {
		errmsg("Can't autoscale (no active sets?)");
	    }
	}
    break;

  case 400:
#line 3393 "pars.yacc"
    {
	    autoscale_byset((yyvsp[(2) - (2)].trgt)->gno, (yyvsp[(2) - (2)].trgt)->setno, AUTOSCALE_XY);
	}
    break;

  case 401:
#line 3396 "pars.yacc"
    {
            autotick_axis(whichgraph, ALL_AXES);
        }
    break;

  case 402:
#line 3399 "pars.yacc"
    {
	    int gno = (yyvsp[(2) - (2)].ival);
            if (is_graph_hidden(gno) == FALSE) {
                select_graph(gno);
            } else {
		errmsg("Graph is not active");
            }
	}
    break;

  case 403:
#line 3407 "pars.yacc"
    {
	    gotread = TRUE;
	    strcpy(readfile, (yyvsp[(2) - (2)].sval));
	    xfree((yyvsp[(2) - (2)].sval));
	}
    break;

  case 404:
#line 3412 "pars.yacc"
    {
	    strcpy(batchfile, (yyvsp[(3) - (3)].sval));
	    xfree((yyvsp[(3) - (3)].sval));
	}
    break;

  case 405:
#line 3416 "pars.yacc"
    {
	    getdata(whichgraph, (yyvsp[(3) - (3)].sval), SOURCE_DISK, LOAD_BLOCK);
	    xfree((yyvsp[(3) - (3)].sval));
	}
    break;

  case 406:
#line 3420 "pars.yacc"
    {
	    getdata(whichgraph, (yyvsp[(4) - (4)].sval), (yyvsp[(3) - (4)].ival), LOAD_BLOCK);
	    xfree((yyvsp[(4) - (4)].sval));
	}
    break;

  case 407:
#line 3424 "pars.yacc"
    {
            int nc, *cols, scol;
            if (field_string_to_cols((yyvsp[(3) - (3)].sval), &nc, &cols, &scol) != RETURN_SUCCESS) {
                errmsg("Erroneous field specifications");
	        xfree((yyvsp[(3) - (3)].sval));
                return 1;
            } else {
	        xfree((yyvsp[(3) - (3)].sval));
	        create_set_fromblock(whichgraph, NEW_SET,
                    (yyvsp[(2) - (3)].ival), nc, cols, scol, autoscale_onread);
                xfree(cols);
            }
	}
    break;

  case 408:
#line 3437 "pars.yacc"
    {
	    set_blockdata(NULL);
	}
    break;

  case 409:
#line 3440 "pars.yacc"
    {
	    gotread = TRUE;
	    curtype = (yyvsp[(2) - (3)].ival);
	    strcpy(readfile, (yyvsp[(3) - (3)].sval));
	    xfree((yyvsp[(3) - (3)].sval));
	}
    break;

  case 410:
#line 3446 "pars.yacc"
    {
	    gotread = TRUE;
	    strcpy(readfile, (yyvsp[(4) - (4)].sval));
	    curtype = (yyvsp[(2) - (4)].ival);
	    cursource = (yyvsp[(3) - (4)].ival);
	    xfree((yyvsp[(4) - (4)].sval));
	}
    break;

  case 411:
#line 3453 "pars.yacc"
    {
	    getdata(whichgraph, (yyvsp[(3) - (3)].sval), SOURCE_DISK, LOAD_NXY);
	    xfree((yyvsp[(3) - (3)].sval));
	}
    break;

  case 412:
#line 3457 "pars.yacc"
    {
	    getdata(whichgraph, (yyvsp[(4) - (4)].sval), (yyvsp[(3) - (4)].ival), LOAD_NXY);
	    xfree((yyvsp[(4) - (4)].sval));
	}
    break;

  case 413:
#line 3461 "pars.yacc"
    {
	    if (!safe_mode) {
                outputset((yyvsp[(2) - (2)].trgt)->gno, (yyvsp[(2) - (2)].trgt)->setno, "stdout", NULL);
            } else {
                yyerror("File modifications are disabled in safe mode");
            }
	}
    break;

  case 414:
#line 3468 "pars.yacc"
    {
	    if (!safe_mode) {
	        outputset((yyvsp[(2) - (4)].trgt)->gno, (yyvsp[(2) - (4)].trgt)->setno, "stdout", (yyvsp[(4) - (4)].sval));
            } else {
                yyerror("File modifications are disabled in safe mode");
            }
	    xfree((yyvsp[(4) - (4)].sval));
	}
    break;

  case 415:
#line 3476 "pars.yacc"
    {
	    if (!safe_mode) {
	        outputset((yyvsp[(2) - (4)].trgt)->gno, (yyvsp[(2) - (4)].trgt)->setno, (yyvsp[(4) - (4)].sval), NULL);
            } else {
                yyerror("File modifications are disabled in safe mode");
            }
	    xfree((yyvsp[(4) - (4)].sval));
	}
    break;

  case 416:
#line 3484 "pars.yacc"
    {
	    if (!safe_mode) {
	        outputset((yyvsp[(2) - (6)].trgt)->gno, (yyvsp[(2) - (6)].trgt)->setno, (yyvsp[(4) - (6)].sval), (yyvsp[(6) - (6)].sval));
            } else {
                yyerror("File modifications are disabled in safe mode");
            }
	    xfree((yyvsp[(4) - (6)].sval));
	    xfree((yyvsp[(6) - (6)].sval));
	}
    break;

  case 417:
#line 3493 "pars.yacc"
    {
            if (!safe_mode) {
                save_project((yyvsp[(2) - (2)].sval));
            } else {
                yyerror("File modifications are disabled in safe mode");
            }
            xfree((yyvsp[(2) - (2)].sval));
        }
    break;

  case 418:
#line 3501 "pars.yacc"
    {
            load_project((yyvsp[(2) - (2)].sval));
            xfree((yyvsp[(2) - (2)].sval));
        }
    break;

  case 419:
#line 3505 "pars.yacc"
    {
            new_project(NULL);
        }
    break;

  case 420:
#line 3508 "pars.yacc"
    {
            new_project((yyvsp[(3) - (3)].sval));
            xfree((yyvsp[(3) - (3)].sval));
        }
    break;

  case 421:
#line 3512 "pars.yacc"
    {
	    push_world();
	}
    break;

  case 422:
#line 3515 "pars.yacc"
    {
	    pop_world();
	}
    break;

  case 423:
#line 3518 "pars.yacc"
    {
	    cycle_world_stack();
	}
    break;

  case 424:
#line 3521 "pars.yacc"
    {
	    if ((yyvsp[(2) - (2)].ival) > 0)
		show_world_stack((yyvsp[(2) - (2)].ival) - 1);
	}
    break;

  case 425:
#line 3525 "pars.yacc"
    {
	    clear_world_stack();
	}
    break;

  case 426:
#line 3528 "pars.yacc"
    {
	    do_clear_boxes();
	}
    break;

  case 427:
#line 3531 "pars.yacc"
    {
	    do_clear_ellipses();
	}
    break;

  case 428:
#line 3534 "pars.yacc"
    {
	    do_clear_lines();
	}
    break;

  case 429:
#line 3537 "pars.yacc"
    {
	    do_clear_text();
	}
    break;

  case 430:
#line 3544 "pars.yacc"
    {
#ifndef NONE_GUI
///            set_pagelayout($3);
#endif
        }
    break;

  case 431:
#line 3549 "pars.yacc"
    {
	    auto_redraw = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 432:
#line 3552 "pars.yacc"
    {
	    draw_focus_flag = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 433:
#line 3555 "pars.yacc"
    {
	    focus_policy = FOCUS_SET;
	}
    break;

  case 434:
#line 3558 "pars.yacc"
    {
	    focus_policy = FOCUS_FOLLOWS;
	}
    break;

  case 435:
#line 3561 "pars.yacc"
    {
	    focus_policy = FOCUS_CLICK;
	}
    break;

  case 436:
#line 3568 "pars.yacc"
    {}
    break;

  case 437:
#line 3569 "pars.yacc"
    {}
    break;

  case 438:
#line 3573 "pars.yacc"
    {
	    set_set_hidden((yyvsp[(1) - (2)].trgt)->gno, (yyvsp[(1) - (2)].trgt)->setno, !(yyvsp[(2) - (2)].ival));
	}
    break;

  case 439:
#line 3576 "pars.yacc"
    {
	    set_dataset_type((yyvsp[(1) - (3)].trgt)->gno, (yyvsp[(1) - (3)].trgt)->setno, (yyvsp[(3) - (3)].ival));
	}
    break;

  case 440:
#line 3580 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].sym = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 441:
#line 3583 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].sympen.color = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 442:
#line 3586 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].sympen.pattern = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 443:
#line 3589 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].symlinew = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 444:
#line 3592 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].symlines = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 445:
#line 3595 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].symfillpen.color = (yyvsp[(4) - (4)].ival);
	}
    break;

  case 446:
#line 3598 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].symfillpen.pattern = (yyvsp[(4) - (4)].ival);
	}
    break;

  case 447:
#line 3601 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].symsize = (yyvsp[(4) - (4)].dval);
	}
    break;

  case 448:
#line 3604 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].symchar = (yyvsp[(4) - (4)].ival);
	}
    break;

  case 449:
#line 3607 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].charfont = (yyvsp[(4) - (4)].ival);
	}
    break;

  case 450:
#line 3610 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].symskip = (yyvsp[(4) - (4)].ival);
	}
    break;

  case 451:
#line 3615 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].linet = (yyvsp[(4) - (4)].ival);
	}
    break;

  case 452:
#line 3619 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].lines = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 453:
#line 3623 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].linew = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 454:
#line 3627 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].linepen.color = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 455:
#line 3631 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].linepen.pattern = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 456:
#line 3636 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].filltype = (yyvsp[(4) - (4)].ival);
	}
    break;

  case 457:
#line 3640 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].fillrule = (yyvsp[(4) - (4)].ival);
	}
    break;

  case 458:
#line 3644 "pars.yacc"
    {
	    int prop = (yyvsp[(3) - (3)].ival);

	    if (get_project_version() <= 40102 && get_project_version() >= 30000) {
                switch (filltype_obs) {
                case COLOR:
                    break;
                case PATTERN:
                    prop = 1;
                    break;
                default: /* NONE */
	            prop = 0;
                    break;
                }
	    }
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].setfillpen.color = prop;
	}
    break;

  case 459:
#line 3662 "pars.yacc"
    {
	    int prop = (yyvsp[(3) - (3)].ival);

	    if (get_project_version() <= 40102) {
                switch (filltype_obs) {
                case COLOR:
                    prop = 1;
                    break;
                case PATTERN:
                    break;
                default: /* NONE */
	            prop = 0;
                    break;
                }
	    }
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].setfillpen.pattern = prop;
	}
    break;

  case 460:
#line 3682 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].baseline = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 461:
#line 3686 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].baseline_type = (yyvsp[(4) - (4)].ival);
	}
    break;

  case 462:
#line 3691 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].dropline = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 463:
#line 3696 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].avalue.active = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 464:
#line 3700 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].avalue.type = (yyvsp[(4) - (4)].ival);
	}
    break;

  case 465:
#line 3704 "pars.yacc"
    {
	    g[(yyvsp[(1) - (5)].trgt)->gno].p[(yyvsp[(1) - (5)].trgt)->setno].avalue.size = (yyvsp[(5) - (5)].dval);
	}
    break;

  case 466:
#line 3708 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].avalue.font = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 467:
#line 3712 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].avalue.color = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 468:
#line 3716 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].avalue.angle = (yyvsp[(4) - (4)].ival);
	}
    break;

  case 469:
#line 3720 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].avalue.format = (yyvsp[(4) - (4)].ival);
	}
    break;

  case 470:
#line 3724 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].avalue.prec = (yyvsp[(4) - (4)].ival);
	}
    break;

  case 471:
#line 3727 "pars.yacc"
    {
	    g[(yyvsp[(1) - (6)].trgt)->gno].p[(yyvsp[(1) - (6)].trgt)->setno].avalue.offset.x = (yyvsp[(4) - (6)].dval);
	    g[(yyvsp[(1) - (6)].trgt)->gno].p[(yyvsp[(1) - (6)].trgt)->setno].avalue.offset.y = (yyvsp[(6) - (6)].dval);
	}
    break;

  case 472:
#line 3732 "pars.yacc"
    {
	    strcpy(g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].avalue.prestr, (yyvsp[(4) - (4)].sval));
	    xfree((yyvsp[(4) - (4)].sval));
	}
    break;

  case 473:
#line 3737 "pars.yacc"
    {
	    strcpy(g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].avalue.appstr, (yyvsp[(4) - (4)].sval));
	    xfree((yyvsp[(4) - (4)].sval));
	}
    break;

  case 474:
#line 3742 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].errbar.active = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 475:
#line 3745 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].errbar.ptype = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 476:
#line 3748 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].errbar.pen.color = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 477:
#line 3751 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].errbar.pen.pattern = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 478:
#line 3754 "pars.yacc"
    {
            g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].errbar.barsize = (yyvsp[(4) - (4)].dval);
	}
    break;

  case 479:
#line 3757 "pars.yacc"
    {
            g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].errbar.linew = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 480:
#line 3760 "pars.yacc"
    {
            g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].errbar.lines = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 481:
#line 3763 "pars.yacc"
    {
            g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].errbar.riser_linew = (yyvsp[(4) - (4)].dval);
	}
    break;

  case 482:
#line 3766 "pars.yacc"
    {
            g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].errbar.riser_lines = (yyvsp[(4) - (4)].ival);
	}
    break;

  case 483:
#line 3769 "pars.yacc"
    {
            g[(yyvsp[(1) - (5)].trgt)->gno].p[(yyvsp[(1) - (5)].trgt)->setno].errbar.arrow_clip = (yyvsp[(5) - (5)].ival);
	}
    break;

  case 484:
#line 3772 "pars.yacc"
    {
            g[(yyvsp[(1) - (6)].trgt)->gno].p[(yyvsp[(1) - (6)].trgt)->setno].errbar.cliplen = (yyvsp[(6) - (6)].dval);
	}
    break;

  case 485:
#line 3776 "pars.yacc"
    {
	    strncpy(g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].comments, (yyvsp[(3) - (3)].sval), MAX_STRING_LENGTH - 1);
	    xfree((yyvsp[(3) - (3)].sval));
	}
    break;

  case 486:
#line 3781 "pars.yacc"
    {
	    strncpy(g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].lstr, (yyvsp[(3) - (3)].sval), MAX_STRING_LENGTH - 1);
	    xfree((yyvsp[(3) - (3)].sval));
	}
    break;

  case 487:
#line 3789 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->active = (yyvsp[(1) - (1)].ival);
	}
    break;

  case 488:
#line 3796 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->zero = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 489:
#line 3803 "pars.yacc"
    {}
    break;

  case 490:
#line 3804 "pars.yacc"
    {}
    break;

  case 491:
#line 3805 "pars.yacc"
    {}
    break;

  case 492:
#line 3806 "pars.yacc"
    {}
    break;

  case 493:
#line 3807 "pars.yacc"
    {}
    break;

  case 494:
#line 3808 "pars.yacc"
    {}
    break;

  case 495:
#line 3809 "pars.yacc"
    {}
    break;

  case 496:
#line 3810 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
            g[whichgraph].t[naxis]->offsx = (yyvsp[(2) - (4)].dval);
	    g[whichgraph].t[naxis]->offsy = (yyvsp[(4) - (4)].dval);
	}
    break;

  case 497:
#line 3821 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_flag = (yyvsp[(1) - (1)].ival);
	}
    break;

  case 498:
#line 3828 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
            g[whichgraph].t[naxis]->tmajor = (yyvsp[(2) - (2)].dval);
	}
    break;

  case 499:
#line 3835 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->nminor = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 500:
#line 3842 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_round = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 501:
#line 3850 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
            g[whichgraph].t[naxis]->offsx = (yyvsp[(2) - (2)].dval);
	}
    break;

  case 502:
#line 3857 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
            g[whichgraph].t[naxis]->offsy = (yyvsp[(2) - (2)].dval);
	}
    break;

  case 503:
#line 3864 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_autonum = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 504:
#line 3871 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_inout = (yyvsp[(1) - (1)].ival);
	}
    break;

  case 505:
#line 3878 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.size = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 506:
#line 3885 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->mprops.size = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 507:
#line 3892 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.color = g[whichgraph].t[naxis]->mprops.color = (yyvsp[(1) - (1)].ival);
	}
    break;

  case 508:
#line 3899 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.color = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 509:
#line 3906 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->mprops.color = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 510:
#line 3913 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.linew = g[whichgraph].t[naxis]->mprops.linew = (yyvsp[(1) - (1)].dval);
	}
    break;

  case 511:
#line 3920 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.linew = (yyvsp[(2) - (2)].dval);
	}
    break;

  case 512:
#line 3927 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->mprops.linew = (yyvsp[(2) - (2)].dval);
	}
    break;

  case 513:
#line 3934 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.lines = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 514:
#line 3941 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->mprops.lines = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 515:
#line 3948 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.gridflag = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 516:
#line 3955 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->mprops.gridflag = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 517:
#line 3962 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_op = (yyvsp[(1) - (1)].ival);
	}
    break;

  case 518:
#line 3969 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_spec = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 519:
#line 3976 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->nticks = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 520:
#line 3983 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tloc[(yyvsp[(2) - (4)].ival)].wtpos = (yyvsp[(4) - (4)].dval);
	    g[whichgraph].t[naxis]->tloc[(yyvsp[(2) - (4)].ival)].type = TICK_TYPE_MAJOR;
	}
    break;

  case 521:
#line 3991 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tloc[(yyvsp[(2) - (4)].ival)].wtpos = (yyvsp[(4) - (4)].dval);
	    g[whichgraph].t[naxis]->tloc[(yyvsp[(2) - (4)].ival)].type = TICK_TYPE_MINOR;
	}
    break;

  case 522:
#line 4002 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_flag = (yyvsp[(1) - (1)].ival);
	}
    break;

  case 523:
#line 4009 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_prec = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 524:
#line 4016 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_format = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 525:
#line 4023 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_format = (yyvsp[(2) - (2)].dval);
	}
    break;

  case 526:
#line 4030 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    strcpy(g[whichgraph].t[naxis]->tl_appstr, (yyvsp[(2) - (2)].sval));
	    xfree((yyvsp[(2) - (2)].sval));
	}
    break;

  case 527:
#line 4038 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    strcpy(g[whichgraph].t[naxis]->tl_prestr, (yyvsp[(2) - (2)].sval));
	    xfree((yyvsp[(2) - (2)].sval));
	}
    break;

  case 528:
#line 4046 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_angle = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 529:
#line 4053 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_skip = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 530:
#line 4060 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_staggered = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 531:
#line 4067 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_op = (yyvsp[(1) - (1)].ival);
	}
    break;

  case 532:
#line 4074 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
            g[whichgraph].t[naxis]->tl_formula =
                copy_string(g[whichgraph].t[naxis]->tl_formula, (yyvsp[(2) - (2)].sval));
            xfree((yyvsp[(2) - (2)].sval));
	}
    break;

  case 533:
#line 4083 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_start = (yyvsp[(2) - (2)].dval);
	}
    break;

  case 534:
#line 4090 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_stop = (yyvsp[(2) - (2)].dval);
	}
    break;

  case 535:
#line 4097 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_starttype = TYPE_SPEC;
	}
    break;

  case 536:
#line 4104 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_starttype = TYPE_AUTO;
	}
    break;

  case 537:
#line 4111 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_stoptype = TYPE_SPEC;
	}
    break;

  case 538:
#line 4118 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_stoptype = TYPE_AUTO;
	}
    break;

  case 539:
#line 4125 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_charsize = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 540:
#line 4132 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_font = (yyvsp[(1) - (1)].ival);
	}
    break;

  case 541:
#line 4139 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_color = (yyvsp[(1) - (1)].ival);
	}
    break;

  case 542:
#line 4146 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                xfree((yyvsp[(3) - (3)].sval));
                return 1;
            }
	    if ((yyvsp[(1) - (3)].ival) >= MAX_TICKS) {
	         yyerror("Number of ticks exceeds maximum");
	         xfree((yyvsp[(3) - (3)].sval));
	         return 1;
	    }
	    g[whichgraph].t[naxis]->tloc[(yyvsp[(1) - (3)].ival)].label = 
                copy_string(g[whichgraph].t[naxis]->tloc[(yyvsp[(1) - (3)].ival)].label, (yyvsp[(3) - (3)].sval));
	    xfree((yyvsp[(3) - (3)].sval));
	}
    break;

  case 543:
#line 4161 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_gaptype = TYPE_AUTO;
	}
    break;

  case 544:
#line 4168 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_gaptype = TYPE_SPEC;
	}
    break;

  case 545:
#line 4175 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_gap.x = (yyvsp[(2) - (4)].dval);
	    g[whichgraph].t[naxis]->tl_gap.y = (yyvsp[(4) - (4)].dval);
	}
    break;

  case 546:
#line 4186 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    set_plotstr_string(&g[whichgraph].t[naxis]->label, (yyvsp[(1) - (1)].sval));
	    xfree((yyvsp[(1) - (1)].sval));
	}
    break;

  case 547:
#line 4194 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label_layout = LAYOUT_PERPENDICULAR;
	}
    break;

  case 548:
#line 4201 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label_layout = LAYOUT_PARALLEL;
	}
    break;

  case 549:
#line 4208 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label_place = TYPE_AUTO;
	}
    break;

  case 550:
#line 4215 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label_place = TYPE_SPEC;
	}
    break;

  case 551:
#line 4222 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label.x = (yyvsp[(2) - (4)].dval);
	    g[whichgraph].t[naxis]->label.y = (yyvsp[(4) - (4)].dval);
	}
    break;

  case 552:
#line 4230 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label.just = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 553:
#line 4237 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label.charsize = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 554:
#line 4244 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label.font = (yyvsp[(1) - (1)].ival);
	}
    break;

  case 555:
#line 4251 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label.color = (yyvsp[(1) - (1)].ival);
	}
    break;

  case 556:
#line 4258 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label_op = (yyvsp[(1) - (1)].ival);
	}
    break;

  case 557:
#line 4268 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_drawbar = (yyvsp[(1) - (1)].ival);
	}
    break;

  case 558:
#line 4275 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_drawbarcolor = (yyvsp[(1) - (1)].ival);
	}
    break;

  case 559:
#line 4282 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_drawbarlines = (yyvsp[(1) - (1)].ival);
	}
    break;

  case 560:
#line 4289 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_drawbarlinew = (yyvsp[(1) - (1)].dval);
	}
    break;

  case 561:
#line 4299 "pars.yacc"
    { 
          nonl_opts.title = copy_string(nonl_opts.title, (yyvsp[(2) - (2)].sval));
	  xfree((yyvsp[(2) - (2)].sval));
        }
    break;

  case 562:
#line 4303 "pars.yacc"
    { 
          nonl_opts.formula = copy_string(nonl_opts.formula, (yyvsp[(2) - (2)].sval));
	  xfree((yyvsp[(2) - (2)].sval));
        }
    break;

  case 563:
#line 4307 "pars.yacc"
    { 
            nonl_opts.parnum = (yyvsp[(2) - (3)].ival); 
        }
    break;

  case 564:
#line 4310 "pars.yacc"
    { 
            nonl_opts.tolerance = (yyvsp[(2) - (2)].dval); 
        }
    break;

  case 565:
#line 4317 "pars.yacc"
    {
            (yyval.ival) = (yyvsp[(1) - (1)].ival);
        }
    break;

  case 566:
#line 4321 "pars.yacc"
    {
            (yyval.ival) = (yyvsp[(2) - (2)].ival);
        }
    break;

  case 567:
#line 4328 "pars.yacc"
    {
	    int gno = (yyvsp[(1) - (3)].ival), setno = (yyvsp[(3) - (3)].ival);
            if (allocate_set(gno, setno) == RETURN_SUCCESS) {
                (yyval.trgt) = &trgt_pool[tgtn];
                (yyval.trgt)->gno   = gno;
                (yyval.trgt)->setno = setno;
                tgtn++;
            } else {
                errmsg("Can't allocate referred set");
                return 1;
            }
	}
    break;

  case 568:
#line 4341 "pars.yacc"
    {
	    int gno = (yyvsp[(1) - (4)].ival), setno = (yyvsp[(4) - (4)].ival);
            if (allocate_set(gno, setno) == RETURN_SUCCESS) {
                (yyval.trgt) = &trgt_pool[tgtn];
                (yyval.trgt)->gno   = gno;
                (yyval.trgt)->setno = setno;
                tgtn++;
            } else {
                errmsg("Can't allocate referred set");
                return 1;
            }
	}
    break;

  case 569:
#line 4354 "pars.yacc"
    {
	    int gno = whichgraph, setno = (yyvsp[(1) - (1)].ival);
            if (allocate_set(gno, setno) == RETURN_SUCCESS) {
                (yyval.trgt) = &trgt_pool[tgtn];
                (yyval.trgt)->gno   = gno;
                (yyval.trgt)->setno = setno;
                tgtn++;
            } else {
                errmsg("Can't allocate referred set");
                return 1;
            }
	}
    break;

  case 570:
#line 4367 "pars.yacc"
    {
	    int gno = whichgraph, setno = (yyvsp[(2) - (2)].ival);
            if (allocate_set(gno, setno) == RETURN_SUCCESS) {
                (yyval.trgt) = &trgt_pool[tgtn];
                (yyval.trgt)->gno   = gno;
                (yyval.trgt)->setno = setno;
                tgtn++;
            } else {
                errmsg("Can't allocate referred set");
                return 1;
            }
	}
    break;

  case 571:
#line 4382 "pars.yacc"
    {}
    break;

  case 572:
#line 4383 "pars.yacc"
    {}
    break;

  case 573:
#line 4387 "pars.yacc"
    { naxis =  X_AXIS; }
    break;

  case 574:
#line 4388 "pars.yacc"
    { naxis = Y_AXIS; }
    break;

  case 575:
#line 4389 "pars.yacc"
    { naxis = ZX_AXIS; }
    break;

  case 576:
#line 4390 "pars.yacc"
    { naxis = ZY_AXIS; }
    break;

  case 577:
#line 4394 "pars.yacc"
    { (yyval.ival) = CONSTANT;  }
    break;

  case 578:
#line 4395 "pars.yacc"
    { (yyval.ival) = UCONSTANT; }
    break;

  case 579:
#line 4396 "pars.yacc"
    { (yyval.ival) = FUNC_I;    }
    break;

  case 580:
#line 4397 "pars.yacc"
    { (yyval.ival) = FUNC_D;    }
    break;

  case 581:
#line 4398 "pars.yacc"
    { (yyval.ival) = FUNC_ND;   }
    break;

  case 582:
#line 4399 "pars.yacc"
    { (yyval.ival) = FUNC_NN;   }
    break;

  case 583:
#line 4400 "pars.yacc"
    { (yyval.ival) = FUNC_DD;   }
    break;

  case 584:
#line 4401 "pars.yacc"
    { (yyval.ival) = FUNC_NND;  }
    break;

  case 585:
#line 4402 "pars.yacc"
    { (yyval.ival) = FUNC_PPD;  }
    break;

  case 586:
#line 4403 "pars.yacc"
    { (yyval.ival) = FUNC_PPPD; }
    break;

  case 587:
#line 4404 "pars.yacc"
    { (yyval.ival) = FUNC_PPPPD; }
    break;

  case 588:
#line 4405 "pars.yacc"
    { (yyval.ival) = FUNC_PPPPPD; }
    break;

  case 589:
#line 4409 "pars.yacc"
    { (yyval.ival) =  TICKS_SPEC_NONE; }
    break;

  case 590:
#line 4410 "pars.yacc"
    { (yyval.ival) = TICKS_SPEC_MARKS; }
    break;

  case 591:
#line 4411 "pars.yacc"
    { (yyval.ival) = TICKS_SPEC_BOTH; }
    break;

  case 592:
#line 4415 "pars.yacc"
    { (yyval.ival) = FILTER_INPUT; }
    break;

  case 593:
#line 4416 "pars.yacc"
    { (yyval.ival) = FILTER_OUTPUT; }
    break;

  case 594:
#line 4420 "pars.yacc"
    { (yyval.ival) = FILTER_MAGIC; }
    break;

  case 595:
#line 4421 "pars.yacc"
    { (yyval.ival) = FILTER_PATTERN; }
    break;

  case 596:
#line 4425 "pars.yacc"
    { (yyval.ival) = SET_XY; }
    break;

  case 597:
#line 4426 "pars.yacc"
    { (yyval.ival) = SET_BAR; }
    break;

  case 598:
#line 4427 "pars.yacc"
    { (yyval.ival) = SET_BARDY; }
    break;

  case 599:
#line 4428 "pars.yacc"
    { (yyval.ival) = SET_BARDYDY; }
    break;

  case 600:
#line 4429 "pars.yacc"
    { (yyval.ival) = SET_XYZ; }
    break;

  case 601:
#line 4430 "pars.yacc"
    { (yyval.ival) = SET_XYDX; }
    break;

  case 602:
#line 4431 "pars.yacc"
    { (yyval.ival) = SET_XYDY; }
    break;

  case 603:
#line 4432 "pars.yacc"
    { (yyval.ival) = SET_XYDXDX; }
    break;

  case 604:
#line 4433 "pars.yacc"
    { (yyval.ival) = SET_XYDYDY; }
    break;

  case 605:
#line 4434 "pars.yacc"
    { (yyval.ival) = SET_XYDXDY; }
    break;

  case 606:
#line 4435 "pars.yacc"
    { (yyval.ival) = SET_XYDXDXDYDY; }
    break;

  case 607:
#line 4436 "pars.yacc"
    { (yyval.ival) = SET_XYHILO; }
    break;

  case 608:
#line 4437 "pars.yacc"
    { (yyval.ival) = SET_XYR; }
    break;

  case 609:
#line 4438 "pars.yacc"
    { (yyval.ival) = SET_XYSIZE; }
    break;

  case 610:
#line 4439 "pars.yacc"
    { (yyval.ival) = SET_XYCOLOR; }
    break;

  case 611:
#line 4440 "pars.yacc"
    { (yyval.ival) = SET_XYCOLPAT; }
    break;

  case 612:
#line 4441 "pars.yacc"
    { (yyval.ival) = SET_XYVMAP; }
    break;

  case 613:
#line 4442 "pars.yacc"
    { (yyval.ival) = SET_BOXPLOT; }
    break;

  case 614:
#line 4443 "pars.yacc"
    { (yyval.ival) = SET_XY; }
    break;

  case 615:
#line 4447 "pars.yacc"
    { (yyval.ival) = GRAPH_XY; }
    break;

  case 616:
#line 4448 "pars.yacc"
    { (yyval.ival) = GRAPH_CHART; }
    break;

  case 617:
#line 4449 "pars.yacc"
    { (yyval.ival) = GRAPH_POLAR; }
    break;

  case 618:
#line 4450 "pars.yacc"
    { (yyval.ival) = GRAPH_SMITH; }
    break;

  case 619:
#line 4451 "pars.yacc"
    { (yyval.ival) = GRAPH_FIXED; }
    break;

  case 620:
#line 4452 "pars.yacc"
    { (yyval.ival) = GRAPH_PIE;   }
    break;

  case 621:
#line 4456 "pars.yacc"
    { (yyval.ival) = PAGE_FREE; }
    break;

  case 622:
#line 4457 "pars.yacc"
    { (yyval.ival) = PAGE_FIXED; }
    break;

  case 623:
#line 4461 "pars.yacc"
    { (yyval.ival) = PAGE_ORIENT_LANDSCAPE; }
    break;

  case 624:
#line 4462 "pars.yacc"
    { (yyval.ival) = PAGE_ORIENT_PORTRAIT;  }
    break;

  case 625:
#line 4466 "pars.yacc"
    { (yyval.ival) = REGION_ABOVE; }
    break;

  case 626:
#line 4467 "pars.yacc"
    { (yyval.ival) = REGION_BELOW; }
    break;

  case 627:
#line 4468 "pars.yacc"
    { (yyval.ival) = REGION_TOLEFT; }
    break;

  case 628:
#line 4469 "pars.yacc"
    { (yyval.ival) = REGION_TORIGHT; }
    break;

  case 629:
#line 4470 "pars.yacc"
    { (yyval.ival) = REGION_POLYI; }
    break;

  case 630:
#line 4471 "pars.yacc"
    { (yyval.ival) = REGION_POLYO; }
    break;

  case 631:
#line 4472 "pars.yacc"
    { (yyval.ival) = REGION_HORIZI; }
    break;

  case 632:
#line 4473 "pars.yacc"
    { (yyval.ival) = REGION_VERTI; }
    break;

  case 633:
#line 4474 "pars.yacc"
    { (yyval.ival) = REGION_HORIZO; }
    break;

  case 634:
#line 4475 "pars.yacc"
    { (yyval.ival) = REGION_VERTO; }
    break;

  case 635:
#line 4478 "pars.yacc"
    { (yyval.ival) = SCALE_NORMAL; }
    break;

  case 636:
#line 4479 "pars.yacc"
    { (yyval.ival) = SCALE_LOG; }
    break;

  case 637:
#line 4480 "pars.yacc"
    { (yyval.ival) = SCALE_REC; }
    break;

  case 638:
#line 4481 "pars.yacc"
    { (yyval.ival) = SCALE_LOGIT; }
    break;

  case 639:
#line 4484 "pars.yacc"
    { (yyval.ival) = TRUE; }
    break;

  case 640:
#line 4485 "pars.yacc"
    { (yyval.ival) = FALSE; }
    break;

  case 641:
#line 4488 "pars.yacc"
    { (yyval.ival) = RUN_AVG; }
    break;

  case 642:
#line 4489 "pars.yacc"
    { (yyval.ival) = RUN_STD; }
    break;

  case 643:
#line 4490 "pars.yacc"
    { (yyval.ival) = RUN_MED; }
    break;

  case 644:
#line 4491 "pars.yacc"
    { (yyval.ival) = RUN_MAX; }
    break;

  case 645:
#line 4492 "pars.yacc"
    { (yyval.ival) = RUN_MIN; }
    break;

  case 646:
#line 4496 "pars.yacc"
    { (yyval.ival) = SOURCE_DISK; }
    break;

  case 647:
#line 4497 "pars.yacc"
    {
            if (!safe_mode) {
                (yyval.ival) = SOURCE_PIPE;
            } else {
                yyerror("Pipe inputs are disabled in safe mode");
                (yyval.ival) = SOURCE_DISK;
            }
        }
    break;

  case 648:
#line 4507 "pars.yacc"
    { (yyval.ival) = JUST_RIGHT; }
    break;

  case 649:
#line 4508 "pars.yacc"
    { (yyval.ival) = JUST_LEFT; }
    break;

  case 650:
#line 4509 "pars.yacc"
    { (yyval.ival) = JUST_CENTER; }
    break;

  case 651:
#line 4512 "pars.yacc"
    { (yyval.ival) = TICKS_IN; }
    break;

  case 652:
#line 4513 "pars.yacc"
    { (yyval.ival) = TICKS_OUT; }
    break;

  case 653:
#line 4514 "pars.yacc"
    { (yyval.ival) = TICKS_BOTH; }
    break;

  case 654:
#line 4517 "pars.yacc"
    { (yyval.ival) = FORMAT_DECIMAL; }
    break;

  case 655:
#line 4518 "pars.yacc"
    { (yyval.ival) = FORMAT_EXPONENTIAL; }
    break;

  case 656:
#line 4519 "pars.yacc"
    { (yyval.ival) = FORMAT_GENERAL; }
    break;

  case 657:
#line 4520 "pars.yacc"
    { (yyval.ival) = FORMAT_SCIENTIFIC; }
    break;

  case 658:
#line 4521 "pars.yacc"
    { (yyval.ival) = FORMAT_ENGINEERING; }
    break;

  case 659:
#line 4522 "pars.yacc"
    { (yyval.ival) = FORMAT_COMPUTING; }
    break;

  case 660:
#line 4523 "pars.yacc"
    { (yyval.ival) = FORMAT_POWER; }
    break;

  case 661:
#line 4524 "pars.yacc"
    { (yyval.ival) = FORMAT_DDMMYY; }
    break;

  case 662:
#line 4525 "pars.yacc"
    { (yyval.ival) = FORMAT_MMDDYY; }
    break;

  case 663:
#line 4526 "pars.yacc"
    { (yyval.ival) = FORMAT_YYMMDD; }
    break;

  case 664:
#line 4527 "pars.yacc"
    { (yyval.ival) = FORMAT_MMYY; }
    break;

  case 665:
#line 4528 "pars.yacc"
    { (yyval.ival) = FORMAT_MMDD; }
    break;

  case 666:
#line 4529 "pars.yacc"
    { (yyval.ival) = FORMAT_MONTHDAY; }
    break;

  case 667:
#line 4530 "pars.yacc"
    { (yyval.ival) = FORMAT_DAYMONTH; }
    break;

  case 668:
#line 4531 "pars.yacc"
    { (yyval.ival) = FORMAT_MONTHS; }
    break;

  case 669:
#line 4532 "pars.yacc"
    { (yyval.ival) = FORMAT_MONTHSY; }
    break;

  case 670:
#line 4533 "pars.yacc"
    { (yyval.ival) = FORMAT_MONTHL; }
    break;

  case 671:
#line 4534 "pars.yacc"
    { (yyval.ival) = FORMAT_DAYOFWEEKS; }
    break;

  case 672:
#line 4535 "pars.yacc"
    { (yyval.ival) = FORMAT_DAYOFWEEKL; }
    break;

  case 673:
#line 4536 "pars.yacc"
    { (yyval.ival) = FORMAT_DAYOFYEAR; }
    break;

  case 674:
#line 4537 "pars.yacc"
    { (yyval.ival) = FORMAT_HMS; }
    break;

  case 675:
#line 4538 "pars.yacc"
    { (yyval.ival) = FORMAT_MMDDHMS; }
    break;

  case 676:
#line 4539 "pars.yacc"
    { (yyval.ival) = FORMAT_MMDDYYHMS; }
    break;

  case 677:
#line 4540 "pars.yacc"
    { (yyval.ival) = FORMAT_YYMMDDHMS; }
    break;

  case 678:
#line 4541 "pars.yacc"
    { (yyval.ival) = FORMAT_DEGREESLON; }
    break;

  case 679:
#line 4542 "pars.yacc"
    { (yyval.ival) = FORMAT_DEGREESMMLON; }
    break;

  case 680:
#line 4543 "pars.yacc"
    { (yyval.ival) = FORMAT_DEGREESMMSSLON; }
    break;

  case 681:
#line 4544 "pars.yacc"
    { (yyval.ival) = FORMAT_MMSSLON; }
    break;

  case 682:
#line 4545 "pars.yacc"
    { (yyval.ival) = FORMAT_DEGREESLAT; }
    break;

  case 683:
#line 4546 "pars.yacc"
    { (yyval.ival) = FORMAT_DEGREESMMLAT; }
    break;

  case 684:
#line 4547 "pars.yacc"
    { (yyval.ival) = FORMAT_DEGREESMMSSLAT; }
    break;

  case 685:
#line 4548 "pars.yacc"
    { (yyval.ival) = FORMAT_MMSSLAT; }
    break;

  case 686:
#line 4551 "pars.yacc"
    { (yyval.ival) = SIGN_NORMAL; }
    break;

  case 687:
#line 4552 "pars.yacc"
    { (yyval.ival) = SIGN_ABSOLUTE; }
    break;

  case 688:
#line 4553 "pars.yacc"
    { (yyval.ival) = SIGN_NEGATE; }
    break;

  case 689:
#line 4556 "pars.yacc"
    { (yyval.ival) = UP; }
    break;

  case 690:
#line 4557 "pars.yacc"
    { (yyval.ival) = DOWN; }
    break;

  case 691:
#line 4558 "pars.yacc"
    { (yyval.ival) = RIGHT; }
    break;

  case 692:
#line 4559 "pars.yacc"
    { (yyval.ival) = LEFT; }
    break;

  case 693:
#line 4560 "pars.yacc"
    { (yyval.ival) = IN; }
    break;

  case 694:
#line 4561 "pars.yacc"
    { (yyval.ival) = OUT; }
    break;

  case 695:
#line 4564 "pars.yacc"
    { (yyval.ival) = COORD_WORLD; }
    break;

  case 696:
#line 4565 "pars.yacc"
    { (yyval.ival) = COORD_VIEW; }
    break;

  case 697:
#line 4568 "pars.yacc"
    { (yyval.ival) = DATA_X; }
    break;

  case 698:
#line 4569 "pars.yacc"
    { (yyval.ival) = DATA_Y; }
    break;

  case 699:
#line 4570 "pars.yacc"
    { (yyval.ival) = DATA_X; }
    break;

  case 700:
#line 4571 "pars.yacc"
    { (yyval.ival) = DATA_Y; }
    break;

  case 701:
#line 4572 "pars.yacc"
    { (yyval.ival) = DATA_Y1; }
    break;

  case 702:
#line 4573 "pars.yacc"
    { (yyval.ival) = DATA_Y2; }
    break;

  case 703:
#line 4574 "pars.yacc"
    { (yyval.ival) = DATA_Y3; }
    break;

  case 704:
#line 4575 "pars.yacc"
    { (yyval.ival) = DATA_Y4; }
    break;

  case 705:
#line 4578 "pars.yacc"
    { (yyval.ival) = ASCENDING; }
    break;

  case 706:
#line 4579 "pars.yacc"
    { (yyval.ival) = DESCENDING; }
    break;

  case 707:
#line 4582 "pars.yacc"
    { (yyval.ival) = DATA_X; }
    break;

  case 708:
#line 4583 "pars.yacc"
    { (yyval.ival) = DATA_Y; }
    break;

  case 709:
#line 4586 "pars.yacc"
    { (yyval.ival) = FFT_DFT; }
    break;

  case 710:
#line 4587 "pars.yacc"
    { (yyval.ival) = FFT_FFT; }
    break;

  case 711:
#line 4588 "pars.yacc"
    { (yyval.ival) = FFT_INVDFT; }
    break;

  case 712:
#line 4589 "pars.yacc"
    { (yyval.ival) = FFT_INVFFT; }
    break;

  case 713:
#line 4593 "pars.yacc"
    {(yyval.ival)=0;}
    break;

  case 714:
#line 4594 "pars.yacc"
    {(yyval.ival)=1;}
    break;

  case 715:
#line 4598 "pars.yacc"
    {(yyval.ival)=0;}
    break;

  case 716:
#line 4599 "pars.yacc"
    {(yyval.ival)=1;}
    break;

  case 717:
#line 4600 "pars.yacc"
    {(yyval.ival)=2;}
    break;

  case 718:
#line 4604 "pars.yacc"
    {(yyval.ival)=0;}
    break;

  case 719:
#line 4605 "pars.yacc"
    {(yyval.ival)=1;}
    break;

  case 720:
#line 4606 "pars.yacc"
    {(yyval.ival)=2;}
    break;

  case 721:
#line 4610 "pars.yacc"
    {(yyval.ival)=0;}
    break;

  case 722:
#line 4611 "pars.yacc"
    {(yyval.ival)=1;}
    break;

  case 723:
#line 4612 "pars.yacc"
    {(yyval.ival)=2;}
    break;

  case 724:
#line 4613 "pars.yacc"
    {(yyval.ival)=3;}
    break;

  case 725:
#line 4614 "pars.yacc"
    {(yyval.ival)=4;}
    break;

  case 726:
#line 4615 "pars.yacc"
    {(yyval.ival)=5;}
    break;

  case 727:
#line 4616 "pars.yacc"
    {(yyval.ival)=6;}
    break;

  case 728:
#line 4620 "pars.yacc"
    { (yyval.ival) = INTERP_LINEAR; }
    break;

  case 729:
#line 4621 "pars.yacc"
    { (yyval.ival) = INTERP_SPLINE; }
    break;

  case 730:
#line 4622 "pars.yacc"
    { (yyval.ival) = INTERP_ASPLINE; }
    break;

  case 731:
#line 4625 "pars.yacc"
    { (yyval.ival) = MINP; }
    break;

  case 732:
#line 4626 "pars.yacc"
    { (yyval.ival) = MAXP; }
    break;

  case 733:
#line 4627 "pars.yacc"
    { (yyval.ival) = AVG; }
    break;

  case 734:
#line 4628 "pars.yacc"
    { (yyval.ival) = SD; }
    break;

  case 735:
#line 4629 "pars.yacc"
    { (yyval.ival) = SUM; }
    break;

  case 736:
#line 4630 "pars.yacc"
    { (yyval.ival) = IMIN; }
    break;

  case 737:
#line 4631 "pars.yacc"
    { (yyval.ival) = IMAX; }
    break;

  case 738:
#line 4636 "pars.yacc"
    {
            (yyval.ival) = get_mapped_font((yyvsp[(2) - (2)].ival));
        }
    break;

  case 739:
#line 4640 "pars.yacc"
    {
            (yyval.ival) = get_font_by_name((yyvsp[(2) - (2)].sval));
            xfree((yyvsp[(2) - (2)].sval));
        }
    break;

  case 740:
#line 4648 "pars.yacc"
    {
	    int lines = (yyvsp[(2) - (2)].ival);
            if (lines >= 0 && lines < number_of_linestyles()) {
	        (yyval.ival) = lines;
	    } else {
	        errmsg("invalid linestyle");
	        (yyval.ival) = 1;
	    }
        }
    break;

  case 741:
#line 4661 "pars.yacc"
    {
	    int patno = (yyvsp[(2) - (2)].ival);
            if (patno >= 0 && patno < number_of_patterns()) {
	        (yyval.ival) = patno;
	    } else {
	        errmsg("invalid pattern number");
	        (yyval.ival) = 1;
	    }
        }
    break;

  case 742:
#line 4674 "pars.yacc"
    {
            int c = (yyvsp[(2) - (2)].ival);
            if (c >= 0 && c < number_of_colors()) {
                (yyval.ival) = c;
            } else {
                errmsg("Invalid color ID");
                (yyval.ival) = 1;
            }
        }
    break;

  case 743:
#line 4684 "pars.yacc"
    {
            int c = get_color_by_name((yyvsp[(2) - (2)].sval));
            if (c == BAD_COLOR) {
                errmsg("Invalid color name");
                c = 1;
            }
            xfree((yyvsp[(2) - (2)].sval));
            (yyval.ival) = c;
        }
    break;

  case 744:
#line 4694 "pars.yacc"
    {
            int c;
            CMap_entry cmap;
            cmap.rgb.red = (yyvsp[(3) - (8)].ival);
            cmap.rgb.green = (yyvsp[(5) - (8)].ival);
            cmap.rgb.blue = (yyvsp[(7) - (8)].ival);
            cmap.ctype = COLOR_MAIN;
            cmap.cname = NULL;
            c = add_color(cmap);
            if (c == BAD_COLOR) {
                errmsg("Can't allocate requested color");
                c = 1;
            }
            (yyval.ival) = c;
        }
    break;

  case 745:
#line 4713 "pars.yacc"
    {
            double linew;
            linew = (yyvsp[(2) - (2)].dval);
            if (linew < 0.0) {
                yyerror("Negative linewidth");
                linew = 0.0;
            } else if (linew > MAX_LINEWIDTH) {
                yyerror("Linewidth too large");
                linew = MAX_LINEWIDTH;
            }
            (yyval.dval) = linew;
        }
    break;

  case 746:
#line 4728 "pars.yacc"
    {
            (yyval.ival) = (yyvsp[(2) - (2)].ival);
        }
    break;

  case 747:
#line 4733 "pars.yacc"
    { (yyval.ival) = PLACEMENT_NORMAL; }
    break;

  case 748:
#line 4734 "pars.yacc"
    { (yyval.ival) = PLACEMENT_OPPOSITE; }
    break;

  case 749:
#line 4735 "pars.yacc"
    { (yyval.ival) = PLACEMENT_BOTH; }
    break;

  case 750:
#line 4741 "pars.yacc"
    {
            int wpp, hpp;
            if ((yyvsp[(3) - (3)].ival) == PAGE_ORIENT_LANDSCAPE) {
                wpp = 792;
                hpp = 612;
            } else {
                wpp = 612;
                hpp = 792;
            }
            set_page_dimensions(wpp, hpp, FALSE);
        }
    break;

  case 751:
#line 4752 "pars.yacc"
    {
            set_page_dimensions((int) (yyvsp[(3) - (4)].dval), (int) (yyvsp[(4) - (4)].dval), FALSE);
        }
    break;

  case 752:
#line 4755 "pars.yacc"
    {
	    scroll_proc((yyvsp[(2) - (2)].ival));
	}
    break;

  case 753:
#line 4758 "pars.yacc"
    {
	    scrollinout_proc((yyvsp[(3) - (3)].ival));
	}
    break;

  case 754:
#line 4762 "pars.yacc"
    {
	}
    break;

  case 755:
#line 4766 "pars.yacc"
    {
	    add_world(whichgraph, (yyvsp[(3) - (17)].dval), (yyvsp[(5) - (17)].dval), (yyvsp[(7) - (17)].dval), (yyvsp[(9) - (17)].dval));
	}
    break;

  case 756:
#line 4770 "pars.yacc"
    {filltype_obs = (yyvsp[(3) - (3)].ival);}
    break;

  case 757:
#line 4772 "pars.yacc"
    {filltype_obs = (yyvsp[(3) - (3)].ival);}
    break;

  case 758:
#line 4774 "pars.yacc"
    { }
    break;

  case 759:
#line 4776 "pars.yacc"
    { }
    break;

  case 760:
#line 4778 "pars.yacc"
    { }
    break;

  case 761:
#line 4779 "pars.yacc"
    { }
    break;

  case 762:
#line 4781 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    if ((yyvsp[(3) - (3)].ival) == FALSE && get_project_version() <= 40102) {
                g[whichgraph].l.boxpen.pattern = 0;
            }
	}
    break;

  case 763:
#line 4790 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.legx = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 764:
#line 4797 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.legy = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 765:
#line 4804 "pars.yacc"
    {
	    if (is_valid_setno(whichgraph, (yyvsp[(3) - (4)].ival))) {
                strncpy(g[whichgraph].p[(yyvsp[(3) - (4)].ival)].lstr, (yyvsp[(4) - (4)].sval), MAX_STRING_LENGTH - 1);
	    } else {
                yyerror("Unallocated set");
            }
            xfree((yyvsp[(4) - (4)].sval));
	}
    break;

  case 766:
#line 4812 "pars.yacc"
    { }
    break;

  case 767:
#line 4813 "pars.yacc"
    {filltype_obs = (yyvsp[(5) - (5)].ival);}
    break;

  case 768:
#line 4814 "pars.yacc"
    { }
    break;

  case 769:
#line 4815 "pars.yacc"
    { }
    break;

  case 770:
#line 4817 "pars.yacc"
    { }
    break;

  case 771:
#line 4819 "pars.yacc"
    { 
	    g[(yyvsp[(1) - (3)].ival)].type = GRAPH_XY;
	    g[(yyvsp[(1) - (3)].ival)].xscale = SCALE_LOG;
	}
    break;

  case 772:
#line 4823 "pars.yacc"
    { 
	    g[(yyvsp[(1) - (3)].ival)].type = GRAPH_XY;
	    g[(yyvsp[(1) - (3)].ival)].yscale = SCALE_LOG;
	}
    break;

  case 773:
#line 4828 "pars.yacc"
    { 
	    g[(yyvsp[(1) - (3)].ival)].type = GRAPH_XY;
	    g[(yyvsp[(1) - (3)].ival)].xscale = SCALE_LOG;
	    g[(yyvsp[(1) - (3)].ival)].yscale = SCALE_LOG;
	}
    break;

  case 774:
#line 4834 "pars.yacc"
    { 
	    g[(yyvsp[(1) - (3)].ival)].type = GRAPH_CHART;
	    g[(yyvsp[(1) - (3)].ival)].xyflip = FALSE;
	    g[(yyvsp[(1) - (3)].ival)].stacked = FALSE;
	}
    break;

  case 775:
#line 4840 "pars.yacc"
    { 
	    g[(yyvsp[(1) - (3)].ival)].type = GRAPH_CHART;
	    g[(yyvsp[(1) - (3)].ival)].xyflip = TRUE;
	}
    break;

  case 776:
#line 4845 "pars.yacc"
    { 
	    g[(yyvsp[(1) - (3)].ival)].type = GRAPH_CHART;
	    g[(yyvsp[(1) - (3)].ival)].stacked = TRUE;
	}
    break;

  case 777:
#line 4850 "pars.yacc"
    { 
	    g[(yyvsp[(1) - (3)].ival)].type = GRAPH_CHART;
	    g[(yyvsp[(1) - (3)].ival)].stacked = TRUE;
	    g[(yyvsp[(1) - (3)].ival)].xyflip = TRUE;
	}
    break;

  case 778:
#line 4856 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].w.xg1 = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 779:
#line 4863 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].w.xg2 = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 780:
#line 4870 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].w.yg1 = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 781:
#line 4877 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].w.yg2 = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 782:
#line 4885 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].v.xv1 = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 783:
#line 4892 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].v.xv2 = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 784:
#line 4899 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].v.yv1 = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 785:
#line 4906 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].v.yv2 = (yyvsp[(3) - (3)].dval);
	}
    break;

  case 786:
#line 4914 "pars.yacc"
    {
	}
    break;

  case 787:
#line 4917 "pars.yacc"
    { 
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
            g[whichgraph].f.fillpen.pattern = (yyvsp[(3) - (3)].ival);
        }
    break;

  case 788:
#line 4925 "pars.yacc"
    {
        }
    break;

  case 789:
#line 4927 "pars.yacc"
    {
        }
    break;

  case 790:
#line 4930 "pars.yacc"
    {
	    line_asize = 2.0*(yyvsp[(4) - (4)].dval);
	}
    break;

  case 791:
#line 4934 "pars.yacc"
    { }
    break;

  case 792:
#line 4935 "pars.yacc"
    { }
    break;

  case 793:
#line 4936 "pars.yacc"
    { }
    break;

  case 794:
#line 4937 "pars.yacc"
    { }
    break;

  case 795:
#line 4942 "pars.yacc"
    { }
    break;

  case 796:
#line 4943 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label_op = (yyvsp[(1) - (1)].ival);
	}
    break;

  case 797:
#line 4953 "pars.yacc"
    {
	    switch ((yyvsp[(4) - (4)].ival)){
	    case 0:
	        g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].symfillpen.pattern = 0;
	        break;
	    case 1:
	        g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].symfillpen.pattern = 1;
	        break;
	    case 2:
	        g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].symfillpen.pattern = 1;
	        g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].symfillpen.color = getbgcolor();
	        break;
	    }
	}
    break;

  case 798:
#line 4968 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].symskip = (yyvsp[(3) - (3)].ival);
	}
    break;

  case 799:
#line 4972 "pars.yacc"
    {
	    switch ((yyvsp[(3) - (3)].ival)) {
            case 0:
                g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].filltype = SETFILL_NONE;
                break;
            case 1:
                g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].filltype = SETFILL_POLYGON;
                break;
            case 2:
                g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].filltype = SETFILL_BASELINE;
                g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].baseline_type = BASELINE_TYPE_0;
                break;
            case 6:
                g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].filltype = SETFILL_BASELINE;
                g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].baseline_type = BASELINE_TYPE_GMIN;
                break;
            case 7:
                g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].filltype = SETFILL_BASELINE;
                g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].baseline_type = BASELINE_TYPE_GMAX;
                break;
            }
	}
    break;

  case 800:
#line 4994 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].errbar.ptype = (yyvsp[(4) - (4)].ival);
	}
    break;

  case 801:
#line 5002 "pars.yacc"
    { }
    break;

  case 802:
#line 5003 "pars.yacc"
    {
	    g[(yyvsp[(1) - (2)].trgt)->gno].p[(yyvsp[(1) - (2)].trgt)->setno].lines = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 803:
#line 5006 "pars.yacc"
    {
	    g[(yyvsp[(1) - (2)].trgt)->gno].p[(yyvsp[(1) - (2)].trgt)->setno].linew = (yyvsp[(2) - (2)].dval);
	}
    break;

  case 804:
#line 5009 "pars.yacc"
    {
	    g[(yyvsp[(1) - (2)].trgt)->gno].p[(yyvsp[(1) - (2)].trgt)->setno].linepen.color = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 805:
#line 5012 "pars.yacc"
    {filltype_obs = (yyvsp[(4) - (4)].ival);}
    break;

  case 806:
#line 5013 "pars.yacc"
    { }
    break;

  case 807:
#line 5014 "pars.yacc"
    {
            g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].errbar.barsize = (yyvsp[(4) - (4)].dval);
	}
    break;

  case 808:
#line 5017 "pars.yacc"
    { }
    break;

  case 809:
#line 5022 "pars.yacc"
    {
	    /* <= xmgr-4.1 */
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->active = (yyvsp[(2) - (2)].ival);
	}
    break;

  case 810:
#line 5030 "pars.yacc"
    { }
    break;

  case 811:
#line 5031 "pars.yacc"
    { }
    break;

  case 812:
#line 5032 "pars.yacc"
    { }
    break;

  case 813:
#line 5033 "pars.yacc"
    { }
    break;

  case 814:
#line 5034 "pars.yacc"
    { }
    break;

  case 815:
#line 5035 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_spec = TICKS_SPEC_NONE;
	}
    break;

  case 816:
#line 5042 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    if (g[whichgraph].t[naxis]->t_spec != TICKS_SPEC_BOTH) {
                g[whichgraph].t[naxis]->t_spec = TICKS_SPEC_MARKS;
            }
	}
    break;

  case 817:
#line 5051 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    if ((yyvsp[(2) - (2)].dval) != 0.0) {
                g[whichgraph].t[naxis]->nminor = 
                            (int) rint(g[whichgraph].t[naxis]->tmajor / (yyvsp[(2) - (2)].dval) - 1);
            } else {
                g[whichgraph].t[naxis]->nminor = 0;
            }
	}
    break;

  case 818:
#line 5063 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.size = (yyvsp[(2) - (2)].dval);
	}
    break;

  case 819:
#line 5070 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tloc[(yyvsp[(1) - (3)].ival)].wtpos = (yyvsp[(3) - (3)].dval);
	    g[whichgraph].t[naxis]->tloc[(yyvsp[(1) - (3)].ival)].type = TICK_TYPE_MAJOR;
	}
    break;

  case 820:
#line 5078 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_op = (yyvsp[(1) - (1)].ival);
	}
    break;

  case 821:
#line 5088 "pars.yacc"
    { }
    break;

  case 822:
#line 5089 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    if (g[whichgraph].t[naxis]->t_spec == TICKS_SPEC_BOTH) {
                g[whichgraph].t[naxis]->t_spec = TICKS_SPEC_MARKS;
            }
	}
    break;

  case 823:
#line 5098 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_spec = TICKS_SPEC_BOTH;
	}
    break;

  case 824:
#line 5105 "pars.yacc"
    { }
    break;

  case 825:
#line 5107 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_angle = 0;
	}
    break;

  case 826:
#line 5114 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_angle = 90;
	}
    break;

  case 827:
#line 5121 "pars.yacc"
    { }
    break;

  case 828:
#line 5122 "pars.yacc"
    { }
    break;

  case 829:
#line 5123 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_op = (yyvsp[(1) - (1)].ival);
	}
    break;

  case 830:
#line 5130 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    switch((yyvsp[(2) - (2)].ival)) {
            case SIGN_NEGATE:
                g[whichgraph].t[naxis]->tl_formula =
                    copy_string(g[whichgraph].t[naxis]->tl_formula, "-$t");
                break;
            case SIGN_ABSOLUTE:
                g[whichgraph].t[naxis]->tl_formula =
                    copy_string(g[whichgraph].t[naxis]->tl_formula, "abs($t)");
                break;
            default:
                g[whichgraph].t[naxis]->tl_formula =
                    copy_string(g[whichgraph].t[naxis]->tl_formula, NULL);
                break;
            }
	}
    break;

  case 834:
#line 5158 "pars.yacc"
    {
            (yyval.ival) = (yyvsp[(2) - (2)].ival);
        }
    break;

  case 835:
#line 5163 "pars.yacc"
    { (yyval.ival) = PLACEMENT_OPPOSITE; }
    break;

  case 836:
#line 5164 "pars.yacc"
    { (yyval.ival) = PLACEMENT_NORMAL; }
    break;

  case 837:
#line 5165 "pars.yacc"
    { (yyval.ival) = PLACEMENT_NORMAL; }
    break;

  case 838:
#line 5166 "pars.yacc"
    { (yyval.ival) = PLACEMENT_OPPOSITE; }
    break;

  case 839:
#line 5167 "pars.yacc"
    { (yyval.ival) = PLACEMENT_BOTH; }
    break;


/* Line 1267 of yacc.c.  */
#line 13319 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 5170 "pars.yacc"


/* list of intrinsic functions and keywords */
symtab_entry ikey[] = {
	{"A0", FITPARM, NULL},
	{"A0MAX", FITPMAX, NULL},
	{"A0MIN", FITPMIN, NULL},
	{"A1", FITPARM, NULL},
	{"A1MAX", FITPMAX, NULL},
	{"A1MIN", FITPMIN, NULL},
	{"A2", FITPARM, NULL},
	{"A2MAX", FITPMAX, NULL},
	{"A2MIN", FITPMIN, NULL},
	{"A3", FITPARM, NULL},
	{"A3MAX", FITPMAX, NULL},
	{"A3MIN", FITPMIN, NULL},
	{"A4", FITPARM, NULL},
	{"A4MAX", FITPMAX, NULL},
	{"A4MIN", FITPMIN, NULL},
	{"A5", FITPARM, NULL},
	{"A5MAX", FITPMAX, NULL},
	{"A5MIN", FITPMIN, NULL},
	{"A6", FITPARM, NULL},
	{"A6MAX", FITPMAX, NULL},
	{"A6MIN", FITPMIN, NULL},
	{"A7", FITPARM, NULL},
	{"A7MAX", FITPMAX, NULL},
	{"A7MIN", FITPMIN, NULL},
	{"A8", FITPARM, NULL},
	{"A8MAX", FITPMAX, NULL},
	{"A8MIN", FITPMIN, NULL},
	{"A9", FITPARM, NULL},
	{"A9MAX", FITPMAX, NULL},
	{"A9MIN", FITPMIN, NULL},
	{"ABOVE", ABOVE, NULL},
	{"ABS", FUNC_D, (void *) fabs},
	{"ABSOLUTE", ABSOLUTE, NULL},
	{"ACOS", FUNC_D, (void *) acos},
	{"ACOSH", FUNC_D, (void *) acosh},
	{"AI", FUNC_D, (void *) ai_wrap},
	{"ALIAS", ALIAS, NULL},
	{"ALT", ALT, NULL},
	{"ALTXAXIS", ALTXAXIS, NULL},
	{"ALTYAXIS", ALTYAXIS, NULL},
	{"AND", AND, NULL},
	{"ANGLE", ANGLE, NULL},
	{"ANTIALIASING", ANTIALIASING, NULL},
	{"APPEND", APPEND, NULL},
	{"ARRANGE", ARRANGE, NULL},
	{"ARROW", ARROW, NULL},
	{"ASCENDING", ASCENDING, NULL},
	{"ASIN", FUNC_D, (void *) asin},
	{"ASINH", FUNC_D, (void *) asinh},
	{"ASPLINE", ASPLINE, NULL},
	{"ATAN", FUNC_D, (void *) atan},
	{"ATAN2", FUNC_DD, (void *) atan2},
	{"ATANH", FUNC_D, (void *) atanh},
	{"AUTO", AUTO, NULL},
	{"AUTOSCALE", AUTOSCALE, NULL},
	{"AUTOTICKS", AUTOTICKS, NULL},
	{"AVALUE", AVALUE, NULL},
	{"AVG", AVG, NULL},
	{"BACKGROUND", BACKGROUND, NULL},
	{"BAR", BAR, NULL},
	{"BARDY", BARDY, NULL},
	{"BARDYDY", BARDYDY, NULL},
	{"BASELINE", BASELINE, NULL},
	{"BATCH", BATCH, NULL},
        {"BEGIN", BEGIN, NULL},
	{"BELOW", BELOW, NULL},
	{"BETA", FUNC_DD, (void *) beta},
	{"BETWEEN", BETWEEN, NULL},
	{"BI", FUNC_D, (void *) bi_wrap},
	{"BLACKMAN", BLACKMAN, NULL},
	{"BLOCK", BLOCK, NULL},
	{"BOTH", BOTH, NULL},
	{"BOTTOM", BOTTOM, NULL},
	{"BOX", BOX, NULL},
	{"CD", CD, NULL},
	{"CEIL", FUNC_D, (void *) ceil},
	{"CENTER", CENTER, NULL},
	{"CHAR", CHAR, NULL},
	{"CHART", CHART, NULL},
	{"CHDTR", FUNC_DD, (void *) chdtr},
	{"CHDTRC", FUNC_DD, (void *) chdtrc},
	{"CHDTRI", FUNC_DD, (void *) chdtri},
	{"CHI", FUNC_D, (void *) chi_wrap},
	{"CI", FUNC_D, (void *) ci_wrap},
	{"CLEAR", CLEAR, NULL},
	{"CLICK", CLICK, NULL},
	{"CLIP", CLIP, NULL},
	{"CLOSE", CLOSE, NULL},
	{"COEFFICIENTS", COEFFICIENTS, NULL},
	{"COLOR", COLOR, NULL},
	{"COMMENT", COMMENT, NULL},
	{"COMPLEX", COMPLEX, NULL},
	{"COMPUTING", COMPUTING, NULL},
	{"CONST", KEY_CONST, NULL},
	{"CONSTRAINTS", CONSTRAINTS, NULL},
	{"COPY", COPY, NULL},
	{"COS", FUNC_D, (void *) cos},
	{"COSH", FUNC_D, (void *) cosh},
	{"CYCLE", CYCLE, NULL},
	{"DATE", DATE, NULL},
	{"DAWSN", FUNC_D, (void *) dawsn},
	{"DAYMONTH", DAYMONTH, NULL},
	{"DAYOFWEEKL", DAYOFWEEKL, NULL},
	{"DAYOFWEEKS", DAYOFWEEKS, NULL},
	{"DAYOFYEAR", DAYOFYEAR, NULL},
	{"DDMMYY", DDMMYY, NULL},
	{"DECIMAL", DECIMAL, NULL},
	{"DEF", DEF, NULL},
	{"DEFAULT", DEFAULT, NULL},
	{"DEFINE", DEFINE, NULL},
	{"DEG", UCONSTANT, (void *) deg_uconst},
	{"DEGREESLAT", DEGREESLAT, NULL},
	{"DEGREESLON", DEGREESLON, NULL},
	{"DEGREESMMLAT", DEGREESMMLAT, NULL},
	{"DEGREESMMLON", DEGREESMMLON, NULL},
	{"DEGREESMMSSLAT", DEGREESMMSSLAT, NULL},
	{"DEGREESMMSSLON", DEGREESMMSSLON, NULL},
	{"DESCENDING", DESCENDING, NULL},
	{"DESCRIPTION", DESCRIPTION, NULL},
	{"DEVICE", DEVICE, NULL},
	{"DFT", DFT, NULL},
	{"DIFF", DIFFERENCE, NULL},
	{"DIFFERENCE", DIFFERENCE, NULL},
	{"DISK", DISK, NULL},
	{"DOWN", DOWN, NULL},
	{"DPI", DPI, NULL},
	{"DROP", DROP, NULL},
	{"DROPLINE", DROPLINE, NULL},
	{"ECHO", ECHO, NULL},
	{"ELLIE", FUNC_DD, (void *) ellie},
	{"ELLIK", FUNC_DD, (void *) ellik},
	{"ELLIPSE", ELLIPSE, NULL},
	{"ELLPE", FUNC_D, (void *) ellpe_wrap},
	{"ELLPK", FUNC_D, (void *) ellpk_wrap},
	{"ENGINEERING", ENGINEERING, NULL},
	{"EQ", EQ, NULL},
	{"ER", ERRORBAR, NULL},
	{"ERF", FUNC_D, (void *) erf},
	{"ERFC", FUNC_D, (void *) erfc},
	{"ERRORBAR", ERRORBAR, NULL},
	{"EXIT", EXIT, NULL},
	{"EXP", FUNC_D, (void *) exp},
	{"EXPN", FUNC_ND, (void *) expn},
	{"EXPONENTIAL", EXPONENTIAL, NULL},
	{"FAC", FUNC_I, (void *) fac},
	{"FALSE", OFF, NULL},
	{"FDTR", FUNC_NND, (void *) fdtr},
	{"FDTRC", FUNC_NND, (void *) fdtrc},
	{"FDTRI", FUNC_NND, (void *) fdtri},
	{"FFT", FFT, NULL},
	{"FILE", FILEP, NULL},
	{"FILL", FILL, NULL},
	{"FIT", FIT, NULL},
	{"FIXED", FIXED, NULL},
	{"FIXEDPOINT", FIXEDPOINT, NULL},
	{"FLOOR", FUNC_D, (void *) floor},
	{"FLUSH", FLUSH, NULL},
	{"FOCUS", FOCUS, NULL},
	{"FOLLOWS", FOLLOWS, NULL},
	{"FONT", FONTP, NULL},
	{"FORCE", FORCE, NULL},
	{"FORMAT", FORMAT, NULL},
	{"FORMULA", FORMULA, NULL},
	{"FRAME", FRAMEP, NULL},
	{"FREE", FREE, NULL},
	{"FREQUENCY", FREQUENCY, NULL},
	{"FRESNLC", FUNC_D, (void *) fresnlc_wrap},
	{"FRESNLS", FUNC_D, (void *) fresnls_wrap},
	{"FROM", FROM, NULL},
	{"F_OF_D", KEY_FUNC_D, NULL},
	{"F_OF_DD", KEY_FUNC_DD, NULL},
        {"F_OF_I", KEY_FUNC_I, NULL},
	{"F_OF_ND", KEY_FUNC_ND, NULL},
	{"F_OF_NN", KEY_FUNC_NN, NULL},
	{"F_OF_NND", KEY_FUNC_NND, NULL},
	{"F_OF_PPD", KEY_FUNC_PPD, NULL},
	{"F_OF_PPPD", KEY_FUNC_PPPD, NULL},
	{"F_OF_PPPPD", KEY_FUNC_PPPPD, NULL},
	{"F_OF_PPPPPD", KEY_FUNC_PPPPPD, NULL},
	{"GAMMA", FUNC_D, (void *) true_gamma},
	{"GDTR", FUNC_PPD, (void *) gdtr},
	{"GDTRC", FUNC_PPD, (void *) gdtrc},
	{"GE", GE, NULL},
	{"GENERAL", GENERAL, NULL},
	{"GETP", GETP, NULL},
	{"GRAPH", GRAPH, NULL},
	{"GRID", GRID, NULL},
	{"GT", GT, NULL},
	{"HAMMING", HAMMING, NULL},
	{"HANNING", HANNING, NULL},
	{"HARDCOPY", HARDCOPY, NULL},
	{"HBAR", HBAR, NULL},
	{"HELP", HELP, NULL},
	{"HGAP", HGAP, NULL},
	{"HIDDEN", HIDDEN, NULL},
	{"HISTOGRAM", HISTOGRAM, NULL},
	{"HMS", HMS, NULL},
	{"HORIZI", HORIZI, NULL},
	{"HORIZO", HORIZO, NULL},
	{"HORIZONTAL", HORIZONTAL, NULL},
	{"HYP2F1", FUNC_PPPD, (void *) hyp2f1},
	{"HYPERG", FUNC_PPD, (void *) hyperg},
	{"HYPOT", FUNC_DD, (void *) hypot},
	{"I0E", FUNC_D, (void *) i0e},
	{"I1E", FUNC_D, (void *) i1e},
	{"ID", ID, NULL},
	{"IFILTER", IFILTER, NULL},
	{"IGAM", FUNC_DD, (void *) igam},
	{"IGAMC", FUNC_DD, (void *) igamc},
	{"IGAMI", FUNC_DD, (void *) igami},
	{"IMAX", IMAX, NULL},
	{"IMIN", IMIN, NULL},
	{"IN", IN, NULL},
	{"INCBET", FUNC_PPD, (void *) incbet},
	{"INCBI", FUNC_PPD, (void *) incbi},
	{"INCREMENT", INCREMENT, NULL},
	{"INDEX", INDEX, NULL},
	{"INOUT", INOUT, NULL},
	{"INT", INT, NULL},
	{"INTEGRATE", INTEGRATE, NULL},
	{"INTERPOLATE", INTERPOLATE, NULL},
	{"INVDFT", INVDFT, NULL},
	{"INVERT", INVERT, NULL},
	{"INVFFT", INVFFT, NULL},
	{"IRAND", FUNC_I, (void *) irand_wrap},
	{"IV", FUNC_DD, (void *) iv_wrap},
	{"JUST", JUST, NULL},
	{"JV", FUNC_DD, (void *) jv_wrap},
	{"K0E", FUNC_D, (void *) k0e},
	{"K1E", FUNC_D, (void *) k1e},
	{"KILL", KILL, NULL},
	{"KN", FUNC_ND, (void *) kn_wrap},
	{"LABEL", LABEL, NULL},
	{"LANDSCAPE", LANDSCAPE, NULL},
	{"LAYOUT", LAYOUT, NULL},
	{"LBETA", FUNC_DD, (void *) lbeta},
	{"LE", LE, NULL},
	{"LEFT", LEFT, NULL},
	{"LEGEND", LEGEND, NULL},
	{"LENGTH", LENGTH, NULL},
	{"LGAMMA", FUNC_D, (void *) lgamma},
	{"LINCONV", LINCONV, NULL},
	{"LINE", LINE, NULL},
	{"LINEAR", LINEAR, NULL},
	{"LINESTYLE", LINESTYLE, NULL},
	{"LINEWIDTH", LINEWIDTH, NULL},
	{"LINK", LINK, NULL},
	{"LN", FUNC_D, (void *) log},
	{"LOAD", LOAD, NULL},
	{"LOCTYPE", LOCTYPE, NULL},
	{"LOG", LOG, NULL},
	{"LOG10", FUNC_D, (void *) log10},
	{"LOG2", FUNC_D, (void *) log2},
	{"LOGARITHMIC", LOGARITHMIC, NULL},
	{"LOGX", LOGX, NULL},
	{"LOGXY", LOGXY, NULL},
	{"LOGY", LOGY, NULL},
	{"LOGIT", LOGIT, NULL},
	{"LT", LT, NULL},
	{"MAGIC", MAGIC, NULL},
	{"MAGNITUDE", MAGNITUDE, NULL},
	{"MAJOR", MAJOR, NULL},
	{"MAP", MAP, NULL},
	{"MAX", MAXP, NULL},
	{"MAXOF", FUNC_DD, (void *) max_wrap},
	{"MESH", MESH, NULL},
	{"MIN", MINP, NULL},
	{"MINOF", FUNC_DD, (void *) min_wrap},
	{"MINOR", MINOR, NULL},
	{"MMDD", MMDD, NULL},
	{"MMDDHMS", MMDDHMS, NULL},
	{"MMDDYY", MMDDYY, NULL},
	{"MMDDYYHMS", MMDDYYHMS, NULL},
	{"MMSSLAT", MMSSLAT, NULL},
	{"MMSSLON", MMSSLON, NULL},
	{"MMYY", MMYY, NULL},
	{"MOD", FUNC_DD, (void *) fmod},
	{"MONTHDAY", MONTHDAY, NULL},
	{"MONTHL", MONTHL, NULL},
	{"MONTHS", MONTHS, NULL},
	{"MONTHSY", MONTHSY, NULL},
	{"MOVE", MOVE, NULL},
	{"NDTR", FUNC_D, (void *) ndtr},
	{"NDTRI", FUNC_D, (void *) ndtri},
	{"NE", NE, NULL},
	{"NEGATE", NEGATE, NULL},
	{"NEW", NEW, NULL},
	{"NONE", NONE, NULL},
	{"NONLFIT", NONLFIT, NULL},
	{"NORM", FUNC_D, (void *) fx},
	{"NORMAL", NORMAL, NULL},
	{"NOT", NOT, NULL},
	{"NXY", NXY, NULL},
	{"OFF", OFF, NULL},
	{"OFFSET", OFFSET, NULL},
	{"OFFSETX", OFFSETX, NULL},
	{"OFFSETY", OFFSETY, NULL},
	{"OFILTER", OFILTER, NULL},
	{"ON", ON, NULL},
	{"ONREAD", ONREAD, NULL},
	{"OP", OP, NULL},
	{"OPPOSITE", OPPOSITE, NULL},
	{"OR", OR, NULL},
	{"OUT", OUT, NULL},
	{"PAGE", PAGE, NULL},
	{"PARA", PARA, NULL},
	{"PARAMETERS", PARAMETERS, NULL},
	{"PARZEN", PARZEN, NULL},
	{"PATTERN", PATTERN, NULL},
	{"PDTR", FUNC_ND, (void *) pdtr},
	{"PDTRC", FUNC_ND, (void *) pdtrc},
	{"PDTRI", FUNC_ND, (void *) pdtri},
	{"PERIOD", PERIOD, NULL},
	{"PERP", PERP, NULL},
	{"PHASE", PHASE, NULL},
	{"PI", CONSTANT, (void *) pi_const},
	{"PIE", PIE, NULL},
	{"PIPE", PIPE, NULL},
	{"PLACE", PLACE, NULL},
	{"POINT", POINT, NULL},
	{"POLAR", POLAR, NULL},
	{"POLYI", POLYI, NULL},
	{"POLYO", POLYO, NULL},
	{"POP", POP, NULL},
	{"PORTRAIT", PORTRAIT, NULL},
	{"POWER", POWER, NULL},
	{"PREC", PREC, NULL},
	{"PREPEND", PREPEND, NULL},
	{"PRINT", PRINT, NULL},
	{"PS", PS, NULL},
	{"PSI", FUNC_D, (void *) psi},
	{"PUSH", PUSH, NULL},
	{"PUTP", PUTP, NULL},
	{"RAD", UCONSTANT, (void *) rad_uconst},
	{"RAND", RAND, NULL},
	{"READ", READ, NULL},
	{"REAL", REAL, NULL},
	{"RECIPROCAL", RECIPROCAL, NULL},
	{"REDRAW", REDRAW, NULL},
	{"REFERENCE", REFERENCE, NULL},
	{"REGRESS", REGRESS, NULL},
	{"RESIZE", RESIZE, NULL},
	{"RESTRICT", RESTRICT, NULL},
	{"REVERSE", REVERSE, NULL},
	{"RGAMMA", FUNC_D, (void *) rgamma},
	{"RIGHT", RIGHT, NULL},
	{"RINT", FUNC_D, (void *) rint},
	{"RISER", RISER, NULL},
	{"RNORM", FUNC_DD, (void *) rnorm},
	{"ROT", ROT, NULL},
	{"ROUNDED", ROUNDED, NULL},
	{"RSUM", RSUM, NULL},
	{"RULE", RULE, NULL},
	{"RUNAVG", RUNAVG, NULL},
	{"RUNMAX", RUNMAX, NULL},
	{"RUNMED", RUNMED, NULL},
	{"RUNMIN", RUNMIN, NULL},
	{"RUNSTD", RUNSTD, NULL},
	{"SAVEALL", SAVEALL, NULL},
	{"SCALE", SCALE, NULL},
	{"SCIENTIFIC", SCIENTIFIC, NULL},
	{"SCROLL", SCROLL, NULL},
	{"SD", SD, NULL},
	{"SET", SET, NULL},
	{"SFORMAT", SFORMAT, NULL},
	{"SGN", FUNC_D, (void *) sign_wrap},
	{"SHI", FUNC_D, (void *) shi_wrap},
	{"SI", FUNC_D, (void *) si_wrap},
	{"SIGN", SIGN, NULL},
	{"SIN", FUNC_D, (void *) sin},
	{"SINH", FUNC_D, (void *) sinh},
	{"SIZE", SIZE, NULL},
	{"SKIP", SKIP, NULL},
	{"SLEEP", SLEEP, NULL},
	{"SMITH", SMITH, NULL},
	{"SORT", SORT, NULL},
	{"SOURCE", SOURCE, NULL},
	{"SPEC", SPEC, NULL},
	{"SPENCE", FUNC_D, (void *) spence},
	{"SPLINE", SPLINE, NULL},
	{"SPLIT", SPLIT, NULL},
	{"SQR", FUNC_D, (void *) sqr_wrap},
	{"SQRT", FUNC_D, (void *) sqrt},
	{"STACK", STACK, NULL},
	{"STACKED", STACKED, NULL},
	{"STACKEDBAR", STACKEDBAR, NULL},
	{"STACKEDHBAR", STACKEDHBAR, NULL},
	{"STAGGER", STAGGER, NULL},
	{"START", START, NULL},
	{"STDTR", FUNC_ND, (void *) stdtr},
	{"STDTRI", FUNC_ND, (void *) stdtri},
	{"STOP", STOP, NULL},
	{"STRING", STRING, NULL},
	{"STRUVE", FUNC_DD, (void *) struve},
	{"SUBTITLE", SUBTITLE, NULL},
	{"SUM", SUM, NULL},
	{"SWAP", SWAP, NULL},
	{"SYMBOL", SYMBOL, NULL},
	{"TAN", FUNC_D, (void *) tan},
	{"TANH", FUNC_D, (void *) tanh},
	{"TARGET", TARGET, NULL},
	{"TICK", TICKP, NULL},
	{"TICKLABEL", TICKLABEL, NULL},
	{"TICKS", TICKSP, NULL},
	{"TIMER", TIMER, NULL},
	{"TIMESTAMP", TIMESTAMP, NULL},
	{"TITLE", TITLE, NULL},
	{"TO", TO, NULL},
	{"TOP", TOP, NULL},
	{"TRIANGULAR", TRIANGULAR, NULL},
	{"TRUE", ON, NULL},
	{"TYPE", TYPE, NULL},
	{"UNIT", KEY_UNIT, NULL},
	{"UP", UP, NULL},
	{"UPDATEALL", UPDATEALL, NULL},
	{"USE", USE, NULL},
	{"VERSION", VERSION, NULL},
	{"VERTI", VERTI, NULL},
	{"VERTICAL", VERTICAL, NULL},
	{"VERTO", VERTO, NULL},
	{"VGAP", VGAP, NULL},
	{"VIEW", VIEW, NULL},
	{"VOIGT", FUNC_PPD, (void *) voigt},
	{"VX1", VX1, NULL},
	{"VX2", VX2, NULL},
	{"VXMAX", VXMAX, NULL},
	{"VY1", VY1, NULL},
	{"VY2", VY2, NULL},
	{"VYMAX", VYMAX, NULL},
	{"WELCH", WELCH, NULL},
	{"WITH", WITH, NULL},
	{"WORLD", WORLD, NULL},
	{"WRAP", WRAP, NULL},
	{"WRITE", WRITE, NULL},
	{"WX1", WX1, NULL},
	{"WX2", WX2, NULL},
	{"WY1", WY1, NULL},
	{"WY2", WY2, NULL},
	{"X", X_TOK, NULL},
	{"X0", X0, NULL},
	{"X1", X1, NULL},
	{"XAXES", XAXES, NULL},
	{"XAXIS", XAXIS, NULL},
	{"XCOR", XCOR, NULL},
	{"XMAX", XMAX, NULL},
	{"XMIN", XMIN, NULL},
	{"XY", XY, NULL},
	{"XYAXES", XYAXES, NULL},
	{"XYBOXPLOT", XYBOXPLOT, NULL},
	{"XYCOLOR", XYCOLOR, NULL},
	{"XYCOLPAT", XYCOLPAT, NULL},
	{"XYDX", XYDX, NULL},
	{"XYDXDX", XYDXDX, NULL},
	{"XYDXDXDYDY", XYDXDXDYDY, NULL},
	{"XYDXDY", XYDXDY, NULL},
	{"XYDY", XYDY, NULL},
	{"XYDYDY", XYDYDY, NULL},
	{"XYHILO", XYHILO, NULL},
	{"XYR", XYR, NULL},
	{"XYSIZE", XYSIZE, NULL},
	{"XYSTRING", XYSTRING, NULL},
	{"XYVMAP", XYVMAP, NULL},
	{"XYZ", XYZ, NULL},
	{"Y", Y_TOK, NULL},
	{"Y0", Y0, NULL},
	{"Y1", Y1, NULL},
	{"Y2", Y2, NULL},
	{"Y3", Y3, NULL},
	{"Y4", Y4, NULL},
	{"YAXES", YAXES, NULL},
	{"YAXIS", YAXIS, NULL},
	{"YEAR", YEAR, NULL},
	{"YMAX", YMAX, NULL},
	{"YMIN", YMIN, NULL},
	{"YV", FUNC_DD, (void *) yv_wrap},
	{"YYMMDD", YYMMDD, NULL},
	{"YYMMDDHMS", YYMMDDHMS, NULL},
	{"ZERO", ZERO, NULL},
	{"ZEROXAXIS", ALTXAXIS, NULL},
	{"ZEROYAXIS", ALTYAXIS, NULL},
	{"ZETA", FUNC_DD, (void *) zeta},
	{"ZETAC", FUNC_D, (void *) zetac},
	{"ZNORM", ZNORM, NULL}
};

static int maxfunc = sizeof(ikey) / sizeof(symtab_entry);

int get_parser_gno(void)
{
    return whichgraph;
}

int set_parser_gno(int gno)
{
    if (is_valid_gno(gno) == TRUE) {
        whichgraph = gno;
        return RETURN_SUCCESS;
    } else {
        return RETURN_FAILURE;
    }
}

int get_parser_setno(void)
{
    return whichset;
}

int set_parser_setno(int gno, int setno)
{
    if (is_valid_setno(gno, setno) == TRUE) {
        whichgraph = gno;
        whichset = setno;
        /* those will usually be overridden except when evaluating
           a _standalone_ vexpr */
        vasgn_gno = gno;
        vasgn_setno = setno;
        return RETURN_SUCCESS;
    } else {
        return RETURN_FAILURE;
    }
}

void realloc_vrbl(grarr *vrbl, int len)
{
    double *a;
    int i, oldlen;
    
    if (vrbl->type != GRARR_VEC) {
        errmsg("Internal error");
        return;
    }
    oldlen = vrbl->length;
    if (oldlen == len) {
        return;
    } else {
        a = (double*)xrealloc(vrbl->data, len*sizeof(double));
        if (a != NULL || len == 0) {
            vrbl->data = a;
            vrbl->length = len;
            for (i = oldlen; i < len; i++) {
                vrbl->data[i] = 0.0;
            }
        } else {
            errmsg("Malloc failed in realloc_vrbl()");
        }
    }
}


#define PARSER_TYPE_VOID    0
#define PARSER_TYPE_EXPR    1
#define PARSER_TYPE_VEXPR   2

static int parser(char *s, int type)
{
    char *seekpos;
    int i;
    
    if (s == NULL || s[0] == '\0') {
        if (type == PARSER_TYPE_VOID) {
            /* don't consider an empty string as error for generic parser */
            return RETURN_SUCCESS;
        } else {
            return RETURN_FAILURE;
        }
    }
    
    strncpy(f_string, s, MAX_PARS_STRING_LENGTH - 2);
    f_string[MAX_PARS_STRING_LENGTH - 2] = '\0';
    strcat(f_string, " ");
    
    seekpos = f_string;

    while ((seekpos - f_string < MAX_PARS_STRING_LENGTH - 1) && (*seekpos == ' ' || *seekpos == '\t')) {
        seekpos++;
    }
    if (*seekpos == '\n' || *seekpos == '#') {
        if (type == PARSER_TYPE_VOID) {
            /* don't consider an empty string as error for generic parser */
            return RETURN_SUCCESS;
        } else {
            return RETURN_FAILURE;
        }
    }
    
    lowtoupper(f_string);
        
    pos = 0;
    interr = 0;
    expr_parsed  = FALSE;
    vexpr_parsed = FALSE;
    
    yyparse();

    /* free temp. arrays; for a vector expression keep the last one
     * (which is none but v_result), given there have been no errors
     * and it's what we've been asked for
     */
    if (vexpr_parsed && !interr && type == PARSER_TYPE_VEXPR) {
        for (i = 0; i < fcnt - 1; i++) {
            free_tmpvrbl(&(freelist[i]));
        }
    } else {
        for (i = 0; i < fcnt; i++) {
            free_tmpvrbl(&(freelist[i]));
        }
    }
    fcnt = 0;
    
    tgtn = 0;
    
    if ((type == PARSER_TYPE_VEXPR && !vexpr_parsed) ||
        (type == PARSER_TYPE_EXPR  && !expr_parsed)) {
        return RETURN_FAILURE;
    } else {
        return (interr ? RETURN_FAILURE:RETURN_SUCCESS);
    }
}

int s_scanner(char *s, double *res)
{
    int retval = parser(s, PARSER_TYPE_EXPR);
    *res = s_result;
    return retval;
}

int v_scanner(char *s, int *reslen, double **vres)
{
    int retval = parser(s, PARSER_TYPE_VEXPR);
    if (retval != RETURN_SUCCESS) {
        return RETURN_FAILURE;
    } else {
        *reslen = v_result->length;
        if (v_result->type == GRARR_TMP) {
            *vres = v_result->data;
            v_result->length = 0;
            v_result->data = NULL;
        } else {
            *vres = copy_data_column(v_result->data, v_result->length);
        }
        return RETURN_SUCCESS;
    }
}

int scanner(char *s)
{
    int retval = parser(s, PARSER_TYPE_VOID);
    if (retval != RETURN_SUCCESS) {
        return RETURN_FAILURE;
    }
    
    if (gotparams) {
	gotparams = FALSE;
        getparms(paramfile);
    }
    
    if (gotread) {
	gotread = FALSE;
        getdata(whichgraph, readfile, cursource, LOAD_SINGLE);
    }
    
    if (gotnlfit) {
	gotnlfit = FALSE;
        do_nonlfit(nlfit_gno, nlfit_setno, nlfit_warray, NULL, nlfit_nsteps);
        XCFREE(nlfit_warray);
    }
    return retval;
}

static void free_tmpvrbl(grarr *vrbl)
{
    if (vrbl->type == GRARR_TMP) {
        vrbl->length = 0;
        XCFREE(vrbl->data);
    }
}

static void copy_vrbl(grarr *dest, grarr *src)
{
    dest->type = src->type;
    dest->data = (double*)xmalloc(src->length*sizeof(double));
    if (dest->data == NULL) {
        errmsg("Malloc failed in copy_vrbl()");
    } else {
        memcpy(dest->data, src->data, src->length*sizeof(double));
        dest->length = src->length;
    }
}

grarr *get_parser_arr_by_name(char * const name)
{
     int position;
     char *s;
     
     s = copy_string(NULL, name);
     lowtoupper(s);
     
     position = findf(key, s);
     xfree(s);
     
     if (position >= 0) {
         if (key[position].type == KEY_VEC) {
            return (grarr *) key[position].data;
         }
     }
     
     return NULL;
}

grarr *define_parser_arr(char * const name)
{
     if (get_parser_arr_by_name(name) == NULL) {
	symtab_entry tmpkey;
        grarr *var;
        
        var = (grarr*)xmalloc(sizeof(grarr));
        var->type = GRARR_VEC;
        var->length = 0;
        var->data = NULL;
        
	tmpkey.s = name;
	tmpkey.type = KEY_VEC;
	tmpkey.data = (void *) var;
	if (addto_symtab(tmpkey) == RETURN_SUCCESS) {
	    return var;
	} else {
            return NULL;
        }
     } else {
        return NULL;
     }
}

int undefine_parser_var(void *ptr)
{
    int i;
    
    for (i = 0; i < maxfunc; i++) {
	if (key[i].data == ptr) {
            xfree(key[i].s);
            maxfunc--;
            if (i != maxfunc) {
                memmove(&(key[i]), &(key[i + 1]), (maxfunc - i)*sizeof(symtab_entry));
            }
            key = (symtab_entry*)xrealloc(key, maxfunc*sizeof(symtab_entry));
            return RETURN_SUCCESS;
        }
    }
    return RETURN_FAILURE;
}

static int find_set_bydata(double *data, target *tgt)
{
    int gno, setno, ncol;
    
    if (data == NULL) {
        return RETURN_FAILURE;
    } else {
        for (gno = 0; gno < number_of_graphs(); gno++) {
            for (setno = 0; setno < number_of_sets(gno); setno++) {
                for (ncol = 0; ncol < MAX_SET_COLS; ncol++) {
                    if (getcol(gno, setno, ncol) == data) {
                        tgt->gno   = gno;
                        tgt->setno = setno;
                        return RETURN_SUCCESS;
                    }
                }
            }
        }
    }
    return RETURN_FAILURE;
}

static int findf(symtab_entry *keytable, char *s)
{

    int low, high, mid;

    low = 0;
    high = maxfunc - 1;
    while (low <= high) {
	mid = (low + high) / 2;
	if (strcmp(s, keytable[mid].s) < 0) {
	    high = mid - 1;
	} else {
	    if (strcmp(s, keytable[mid].s) > 0) {
		low = mid + 1;
	    } else {
		return (mid);
	    }
	}
    }
    return (-1);
}

static int compare_keys (const void *a, const void *b)
{
    return (int) strcmp (((const symtab_entry*)a)->s,
                         ((const symtab_entry*)b)->s);
}

/* add new entry to the symbol table */
int addto_symtab(symtab_entry newkey)
{
    int position;
    char *s;
    
    s = copy_string(NULL, newkey.s);
    lowtoupper(s);
    if ((position = findf(key, s)) < 0) {
        if ((key = (symtab_entry *) xrealloc(key, (maxfunc + 1)*sizeof(symtab_entry))) != NULL) {
	    key[maxfunc].type = newkey.type;
	    key[maxfunc].data = newkey.data;
	    key[maxfunc].s = s;
	    maxfunc++;
	    qsort(key, maxfunc, sizeof(symtab_entry), compare_keys);
	    return RETURN_SUCCESS;
	} else {
	    xfree(s);
	    return RETURN_FAILURE;
	}
    } else if (alias_force == TRUE) { /* already exists but alias_force enabled */
        key[position].type = newkey.type;
	key[position].data = newkey.data;
	return RETURN_SUCCESS;
    } else {
	xfree(s);
        return RETURN_FAILURE;
    }
}

/* initialize symbol table */
void init_symtab(void)
{
    int i;
    
    if ((key = (symtab_entry *) xmalloc(maxfunc*sizeof(symtab_entry))) != NULL) {
    	memcpy (key, ikey, maxfunc*sizeof(symtab_entry));
	for (i = 0; i < maxfunc; i++) {
	    key[i].s = (char*)xmalloc(strlen((char*)ikey[i].s) + 1);
	    strcpy(key[i].s, (char*)ikey[i].s);
	}
	qsort(key, maxfunc, sizeof(symtab_entry), compare_keys);
	return;
    } else {
	key = ikey;
	return;
    }
}

static int getcharstr(void)
{
    if (pos >= strlen(f_string))
	 return EOF;
    return (f_string[pos++]);
}

static void ungetchstr(void)
{
    if (pos > 0)
	pos--;
}

static int yylex(void)
{
    int c, i;
    int found;
    char sbuf[MAX_PARS_STRING_LENGTH + 40];

    while ((c = getcharstr()) == ' ' || c == '\t');
    if (c == EOF) {
	return (0);
    }
    if (c == '"') {
	i = 0;
	while ((c = getcharstr()) != '"' && c != EOF) {
	    if (c == '\\') {
		int ctmp;
		ctmp = getcharstr();
		if (ctmp != '"') {
		    ungetchstr();
		}
		else {
		    c = ctmp;
		}
	    }
	    sbuf[i] = c;
	    i++;
	}
	if (c == EOF) {
	    yyerror("Nonterminating string");
	    return 0;
	}
	sbuf[i] = '\0';
	yylval.sval = copy_string(NULL, sbuf);
	return CHRSTR;
    }
    if (c == '.' || isdigit(c)) {
	double d;
	int i, gotdot = 0;

	i = 0;
	while (c == '.' || isdigit(c)) {
	    if (c == '.') {
		if (gotdot) {
		    yyerror("Reading number, too many dots");
	    	    return 0;
		} else {
		    gotdot = 1;
		}
	    }
	    sbuf[i++] = c;
	    c = getcharstr();
	}
	if (c == 'E' || c == 'e') {
	    sbuf[i++] = c;
	    c = getcharstr();
	    if (c == '+' || c == '-') {
		sbuf[i++] = c;
		c = getcharstr();
	    }
	    while (isdigit(c)) {
		sbuf[i++] = c;
		c = getcharstr();
	    }
	}
	if (gotdot && i == 1) {
	    ungetchstr();
	    return '.';
	}
	sbuf[i] = '\0';
	ungetchstr();
	sscanf(sbuf, "%lf", &d);
	yylval.dval = d;
	return NUMBER;
    }
/* graphs, sets, regions resp. */
    if (c == 'G' || c == 'S' || c == 'R') {
	int i = 0, ctmp = c, gn, sn, rn;
	c = getcharstr();
	while (isdigit(c) || c == '$' || c == '_') {
	    sbuf[i++] = c;
	    c = getcharstr();
	}
	if (i == 0) {
	    c = ctmp;
	    ungetchstr();
	} else {
	    ungetchstr();
	    if (ctmp == 'G') {
	        sbuf[i] = '\0';
		if (i == 1 && sbuf[0] == '_') {
                    gn = get_recent_gno();
                } else if (i == 1 && sbuf[0] == '$') {
                    gn = whichgraph;
                } else {
                    gn = atoi(sbuf);
                }
		if (is_valid_gno(gn) || graph_allocate(gn) == RETURN_SUCCESS) {
		    yylval.ival = gn;
		    return GRAPHNO;
		}
	    } else if (ctmp == 'S') {
	        sbuf[i] = '\0';
		if (i == 1 && sbuf[0] == '_') {
                    sn = get_recent_setno();
                } else if (i == 1 && sbuf[0] == '$') {
                    sn = whichset;
                } else {
		    sn = atoi(sbuf);
                }
		yylval.ival = sn;
		return SETNUM;
	    } else if (ctmp == 'R') {
	        sbuf[i] = '\0';
		rn = atoi(sbuf);
		if (rn >= 0 && rn < MAXREGION) {
		    yylval.ival = rn;
		    return REGNUM;
		} else {
                    errmsg("Invalid region number");
                }
	    }
	}
    }
    if (isalpha(c) || c == '$') {
	char *p = sbuf;

	do {
	    *p++ = c;
	} while ((c = getcharstr()) != EOF && (isalpha(c) || isdigit(c) ||
                  c == '_' || c == '$'));
	ungetchstr();
	*p = '\0';
#ifdef DEBUG
        if (get_debuglevel() == 2) {
	    printf("->%s<-\n", sbuf);
	}
#endif
	found = -1;
	if ((found = findf(key, sbuf)) >= 0) {
	    if (key[found].type == FITPARM) {
		int index = sbuf[1] - '0';
		yylval.ival = index;
		return FITPARM;
	    }
	    else if (key[found].type == FITPMAX) {
		int index = sbuf[1] - '0';
		yylval.ival = index;
		return FITPMAX;
	    }
	    else if (key[found].type == FITPMIN) {
		int index = sbuf[1] - '0';
		yylval.ival = index;
		return FITPMIN;
	    }

	    else if (key[found].type == KEY_VAR) {
		yylval.dptr = (double *) key[found].data;
		return VAR_D;
	    }
	    else if (key[found].type == KEY_VEC) {
		yylval.vrbl = (grarr *) key[found].data;
		return VEC_D;
	    }

	    else if (key[found].type == FUNC_I) {
		yylval.ival = found;
		return FUNC_I;
	    }
	    else if (key[found].type == CONSTANT) {
		yylval.ival = found;
		return CONSTANT;
	    }
	    else if (key[found].type == UCONSTANT) {
		yylval.ival = found;
		return UCONSTANT;
	    }
	    else if (key[found].type == FUNC_D) {
		yylval.ival = found;
		return FUNC_D;
	    }
	    else if (key[found].type == FUNC_ND) {
		yylval.ival = found;
		return FUNC_ND;
	    }
	    else if (key[found].type == FUNC_DD) {
		yylval.ival = found;
		return FUNC_DD;
	    }
	    else if (key[found].type == FUNC_NND) {
		yylval.ival = found;
		return FUNC_NND;
	    }
	    else if (key[found].type == FUNC_PPD) {
		yylval.ival = found;
		return FUNC_PPD;
	    }
	    else if (key[found].type == FUNC_PPPD) {
		yylval.ival = found;
		return FUNC_PPPD;
	    }
	    else if (key[found].type == FUNC_PPPPD) {
		yylval.ival = found;
		return FUNC_PPPPD;
	    }
	    else if (key[found].type == FUNC_PPPPPD) {
		yylval.ival = found;
		return FUNC_PPPPPD;
	    }
	    else {
	        yylval.ival = key[found].type;
	        return key[found].type;
	    }
	} else {
	    yylval.sval = copy_string(NULL, sbuf);
	    return NEW_TOKEN;
	}
    }
    switch (c) {
    case '>':
	return follow('=', GE, GT);
    case '<':
	return follow('=', LE, LT);
    case '=':
	return follow('=', EQ, '=');
    case '!':
	return follow('=', NE, NOT);
    case '|':
	return follow('|', OR, '|');
    case '&':
	return follow('&', AND, '&');
    case '\n':
	return '\n';
    default:
	return c;
    }
}

static int follow(int expect, int ifyes, int ifno)
{
    int c = getcharstr();

    if (c == expect) {
	return ifyes;
    }
    ungetchstr();
    return ifno;
}

static void yyerror(char *s)
{
    char *buf;
    
    buf = copy_string(NULL, s);
    buf = concat_strings(buf, ": ");
    buf = concat_strings(buf, f_string);
    errmsg(buf);
    xfree(buf);
    interr = 1;
}

