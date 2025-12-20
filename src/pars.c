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
     POLARV2 = 486,
     POLYI = 487,
     POLYO = 488,
     POP = 489,
     PORTRAIT = 490,
     POWER = 491,
     PREC = 492,
     PREPEND = 493,
     PRINT = 494,
     PS = 495,
     PUSH = 496,
     PUTP = 497,
     RAND = 498,
     READ = 499,
     REAL = 500,
     RECIPROCAL = 501,
     REDRAW = 502,
     REFERENCE = 503,
     REGNUM = 504,
     REGRESS = 505,
     RESIZE = 506,
     RESTRICT = 507,
     REVERSE = 508,
     RIGHT = 509,
     RISER = 510,
     ROT = 511,
     ROUNDED = 512,
     RSUM = 513,
     RULE = 514,
     RUNAVG = 515,
     RUNMAX = 516,
     RUNMED = 517,
     RUNMIN = 518,
     RUNSTD = 519,
     SAVEALL = 520,
     SCALE = 521,
     SCIENTIFIC = 522,
     SCROLL = 523,
     SD = 524,
     SET = 525,
     SETNUM = 526,
     SFORMAT = 527,
     SIGN = 528,
     SIZE = 529,
     SKIP = 530,
     SLEEP = 531,
     SMITH = 532,
     SORT = 533,
     SOURCE = 534,
     SPEC = 535,
     SPLINE = 536,
     SPLIT = 537,
     STACK = 538,
     STACKED = 539,
     STACKEDBAR = 540,
     STACKEDHBAR = 541,
     STAGGER = 542,
     START = 543,
     STOP = 544,
     STRING = 545,
     SUM = 546,
     SUBTITLE = 547,
     SWAP = 548,
     SYMBOL = 549,
     TARGET = 550,
     TICKLABEL = 551,
     TICKP = 552,
     TICKSP = 553,
     TIMER = 554,
     TIMESTAMP = 555,
     TITLE = 556,
     TO = 557,
     TOP = 558,
     TRIANGULAR = 559,
     TYPE = 560,
     UP = 561,
     UPDATEALL = 562,
     USE = 563,
     VERSION = 564,
     VERTI = 565,
     VERTICAL = 566,
     VERTO = 567,
     VGAP = 568,
     VIEW = 569,
     VX1 = 570,
     VX2 = 571,
     VXMAX = 572,
     VY1 = 573,
     VY2 = 574,
     VYMAX = 575,
     WELCH = 576,
     WITH = 577,
     WORLD = 578,
     WRAP = 579,
     WRITE = 580,
     WX1 = 581,
     WX2 = 582,
     WY1 = 583,
     WY2 = 584,
     X_TOK = 585,
     X0 = 586,
     X1 = 587,
     XAXES = 588,
     XAXIS = 589,
     XCOR = 590,
     XMAX = 591,
     XMIN = 592,
     XY = 593,
     XYAXES = 594,
     XYBOXPLOT = 595,
     XYCOLOR = 596,
     XYCOLPAT = 597,
     XYDX = 598,
     XYDXDX = 599,
     XYDXDXDYDY = 600,
     XYDXDY = 601,
     XYDY = 602,
     XYDYDY = 603,
     XYHILO = 604,
     XYR = 605,
     XYSIZE = 606,
     XYSTRING = 607,
     XYVMAP = 608,
     XYZ = 609,
     Y_TOK = 610,
     Y0 = 611,
     Y1 = 612,
     Y2 = 613,
     Y3 = 614,
     Y4 = 615,
     YAXES = 616,
     YAXIS = 617,
     YEAR = 618,
     YMAX = 619,
     YMIN = 620,
     YYMMDD = 621,
     YYMMDDHMS = 622,
     YYDYHMS = 623,
     YYYY = 624,
     ZERO = 625,
     ZNORM = 626,
     FITPARM = 627,
     FITPMAX = 628,
     FITPMIN = 629,
     NUMBER = 630,
     NEW_TOKEN = 631,
     OR = 632,
     AND = 633,
     NE = 634,
     EQ = 635,
     GE = 636,
     LE = 637,
     LT = 638,
     GT = 639,
     NOT = 640,
     UMINUS = 641
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
#define POLARV2 486
#define POLYI 487
#define POLYO 488
#define POP 489
#define PORTRAIT 490
#define POWER 491
#define PREC 492
#define PREPEND 493
#define PRINT 494
#define PS 495
#define PUSH 496
#define PUTP 497
#define RAND 498
#define READ 499
#define REAL 500
#define RECIPROCAL 501
#define REDRAW 502
#define REFERENCE 503
#define REGNUM 504
#define REGRESS 505
#define RESIZE 506
#define RESTRICT 507
#define REVERSE 508
#define RIGHT 509
#define RISER 510
#define ROT 511
#define ROUNDED 512
#define RSUM 513
#define RULE 514
#define RUNAVG 515
#define RUNMAX 516
#define RUNMED 517
#define RUNMIN 518
#define RUNSTD 519
#define SAVEALL 520
#define SCALE 521
#define SCIENTIFIC 522
#define SCROLL 523
#define SD 524
#define SET 525
#define SETNUM 526
#define SFORMAT 527
#define SIGN 528
#define SIZE 529
#define SKIP 530
#define SLEEP 531
#define SMITH 532
#define SORT 533
#define SOURCE 534
#define SPEC 535
#define SPLINE 536
#define SPLIT 537
#define STACK 538
#define STACKED 539
#define STACKEDBAR 540
#define STACKEDHBAR 541
#define STAGGER 542
#define START 543
#define STOP 544
#define STRING 545
#define SUM 546
#define SUBTITLE 547
#define SWAP 548
#define SYMBOL 549
#define TARGET 550
#define TICKLABEL 551
#define TICKP 552
#define TICKSP 553
#define TIMER 554
#define TIMESTAMP 555
#define TITLE 556
#define TO 557
#define TOP 558
#define TRIANGULAR 559
#define TYPE 560
#define UP 561
#define UPDATEALL 562
#define USE 563
#define VERSION 564
#define VERTI 565
#define VERTICAL 566
#define VERTO 567
#define VGAP 568
#define VIEW 569
#define VX1 570
#define VX2 571
#define VXMAX 572
#define VY1 573
#define VY2 574
#define VYMAX 575
#define WELCH 576
#define WITH 577
#define WORLD 578
#define WRAP 579
#define WRITE 580
#define WX1 581
#define WX2 582
#define WY1 583
#define WY2 584
#define X_TOK 585
#define X0 586
#define X1 587
#define XAXES 588
#define XAXIS 589
#define XCOR 590
#define XMAX 591
#define XMIN 592
#define XY 593
#define XYAXES 594
#define XYBOXPLOT 595
#define XYCOLOR 596
#define XYCOLPAT 597
#define XYDX 598
#define XYDXDX 599
#define XYDXDXDYDY 600
#define XYDXDY 601
#define XYDY 602
#define XYDYDY 603
#define XYHILO 604
#define XYR 605
#define XYSIZE 606
#define XYSTRING 607
#define XYVMAP 608
#define XYZ 609
#define Y_TOK 610
#define Y0 611
#define Y1 612
#define Y2 613
#define Y3 614
#define Y4 615
#define YAXES 616
#define YAXIS 617
#define YEAR 618
#define YMAX 619
#define YMIN 620
#define YYMMDD 621
#define YYMMDDHMS 622
#define YYDYHMS 623
#define YYYY 624
#define ZERO 625
#define ZNORM 626
#define FITPARM 627
#define FITPMAX 628
#define FITPMIN 629
#define NUMBER 630
#define NEW_TOKEN 631
#define OR 632
#define AND 633
#define NE 634
#define EQ 635
#define GE 636
#define LE 637
#define LT 638
#define GT 639
#define NOT 640
#define UMINUS 641




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

#include <stdio.h>
#include <stdlib.h>
/*#include <unistd.h>*/
#include <string.h>
#include <ctype.h>
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
#include "noxprotos.h"
#define PURE_C_PARSER 1
#include "parser.h"
#undef PURE_C_PARSER
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

static int alias_force = FALSE; /* controls whether aliases can override existing keywords */
extern int error_count;
extern int qtspecial_scanner(char * command,int * errors);
extern int current_origin_graph,current_origin_set;
extern int current_target_graph,current_target_set;
extern void prepare_strings_for_saving(void);
extern void resume_strings_after_load_or_save(void);
extern double log2_wrap(double x);

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
double rint_2(double v);

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
#line 151 "pars.yacc"
{
    int     ival;
    double  dval;
    char   *sval;
    double *dptr;
    target *trgt;
    grarr  *vrbl;
}
/* Line 193 of yacc.c.  */
#line 1027 "pars.tab.cacc"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 1040 "pars.tab.cacc"

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
# if defined YYENABLE_NLS && YYENABLE_NLS
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
#define YYFINAL  525
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   11566

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  403
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  77
/* YYNRULES -- Number of rules.  */
#define YYNRULES  842
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1676

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   641

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   391,     2,     2,
     397,   398,   389,   387,   399,   388,   395,   390,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   378,   396,
       2,   402,     2,   377,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   400,     2,   401,   394,     2,     2,     2,     2,     2,
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
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   379,   380,   381,   382,   383,   384,   385,   386,
     392,   393
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
    2696,  2698,  2700,  2702,  2704,  2706,  2708,  2710,  2712,  2714,
    2716,  2718,  2721,  2724,  2727,  2730,  2733,  2736,  2745,  2748,
    2751,  2753,  2755,  2757,  2761,  2766,  2769,  2773,  2778,  2796,
    2800,  2804,  2807,  2810,  2813,  2816,  2820,  2824,  2828,  2833,
    2838,  2844,  2847,  2850,  2854,  2858,  2862,  2866,  2870,  2874,
    2878,  2882,  2886,  2890,  2894,  2898,  2902,  2906,  2910,  2914,
    2918,  2922,  2927,  2932,  2937,  2941,  2946,  2951,  2954,  2956,
    2958,  2963,  2967,  2971,  2976,  2981,  2984,  2987,  2990,  2995,
    3001,  3006,  3011,  3014,  3017,  3020,  3023,  3026,  3029,  3032,
    3035,  3038,  3041,  3045,  3047,  3049,  3052,  3055,  3058,  3061,
    3064,  3068,  3072,  3074,  3077,  3079,  3081,  3083,  3086,  3088,
    3090,  3092,  3094
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     404,     0,    -1,   405,    -1,   407,    -1,   415,    -1,   406,
      -1,   405,   396,   406,    -1,    -1,   421,    -1,   472,    -1,
     420,    -1,   434,    -1,   424,    -1,   422,    -1,   423,    -1,
     416,    -1,   418,    -1,   419,    -1,     1,    -1,   375,    -1,
      19,    -1,   372,    -1,   373,    -1,   374,    -1,   414,   411,
      -1,   464,   397,   415,   398,    -1,   154,   397,   415,   399,
     415,   398,    -1,   414,   395,   167,    -1,   433,   395,   167,
      -1,   433,   395,   147,    -1,   432,   395,   147,    -1,    21,
      -1,   407,    22,    -1,   243,    -1,    23,   397,   409,   398,
      -1,    24,   397,   407,   398,    -1,    26,   397,   409,   399,
     407,   398,    -1,    25,   397,   409,   399,   409,   398,    -1,
      27,   397,   407,   399,   407,   398,    -1,    28,   397,   409,
     399,   409,   399,   407,   398,    -1,    29,   397,   407,   399,
     407,   399,   407,   398,    -1,    30,   397,   407,   399,   407,
     399,   407,   399,   407,   398,    -1,    31,   397,   407,   399,
     407,   399,   407,   399,   407,   399,   407,   398,    -1,    32,
     397,   407,   399,   407,   399,   407,   399,   407,   399,   407,
     399,   407,   398,    -1,   432,   395,   315,    -1,   432,   395,
     316,    -1,   432,   395,   318,    -1,   432,   395,   319,    -1,
     432,   395,   326,    -1,   432,   395,   327,    -1,   432,   395,
     328,    -1,   432,   395,   329,    -1,    18,   397,   412,   398,
      -1,    18,   397,   409,   399,   410,   399,   410,   398,    -1,
      18,   397,   409,   399,   410,   399,   410,   399,   410,   399,
     410,   399,   407,   398,    -1,   315,    -1,   316,    -1,   318,
      -1,   319,    -1,   326,    -1,   327,    -1,   328,    -1,   329,
      -1,   317,    -1,   320,    -1,   397,   407,   398,    -1,   407,
     387,   407,    -1,   407,   388,   407,    -1,   388,   407,    -1,
     387,   407,    -1,   407,   389,   407,    -1,   407,   390,   407,
      -1,   407,   391,   407,    -1,   407,   394,   407,    -1,   407,
     377,   407,   378,   407,    -1,   407,   386,   407,    -1,   407,
     385,   407,    -1,   407,   384,   407,    -1,   407,   383,   407,
      -1,   407,   382,   407,    -1,   407,   381,   407,    -1,   407,
     380,   407,    -1,   407,   379,   407,    -1,   392,   407,    -1,
      69,    -1,   408,   395,   408,    -1,   408,   395,   407,    -1,
     407,    -1,   409,    -1,   400,   409,   401,    -1,   407,    -1,
     408,    -1,   407,    -1,   408,    -1,    20,    -1,   455,    -1,
     433,   395,   455,    -1,   414,    -1,   414,   400,   409,   378,
     409,   401,    -1,   187,   397,   410,   398,    -1,   187,   397,
     407,   399,   407,   399,   410,   398,    -1,   243,   397,   410,
     398,    -1,   249,   397,   433,   398,    -1,   258,   397,   415,
     398,    -1,    23,   397,   415,   398,    -1,    24,   397,   415,
     398,    -1,    27,   397,   415,   399,   415,   398,    -1,    27,
     397,   407,   399,   415,   398,    -1,    27,   397,   415,   399,
     407,   398,    -1,    26,   397,   409,   399,   415,   398,    -1,
      28,   397,   409,   399,   409,   399,   415,   398,    -1,    29,
     397,   407,   399,   407,   399,   415,   398,    -1,    30,   397,
     407,   399,   407,   399,   407,   399,   415,   398,    -1,    31,
     397,   407,   399,   407,   399,   407,   399,   407,   399,   415,
     398,    -1,    32,   397,   407,   399,   407,   399,   407,   399,
     407,   399,   407,   399,   415,   398,    -1,   415,   387,   415,
      -1,   415,   387,   407,    -1,   407,   387,   415,    -1,   415,
     388,   415,    -1,   415,   388,   407,    -1,   407,   388,   415,
      -1,   415,   389,   415,    -1,   415,   389,   407,    -1,   407,
     389,   415,    -1,   415,   390,   415,    -1,   415,   390,   407,
      -1,   407,   390,   415,    -1,   415,   391,   415,    -1,   415,
     391,   407,    -1,   407,   391,   415,    -1,   415,   394,   415,
      -1,   415,   394,   407,    -1,   407,   394,   415,    -1,   415,
      22,    -1,   415,   377,   407,   378,   407,    -1,   415,   377,
     407,   378,   415,    -1,   415,   377,   415,   378,   407,    -1,
     415,   377,   415,   378,   415,    -1,   415,   379,   415,    -1,
     415,   379,   407,    -1,   407,   379,   415,    -1,   415,   380,
     415,    -1,   415,   380,   407,    -1,   407,   380,   415,    -1,
     415,   386,   415,    -1,   415,   386,   407,    -1,   407,   386,
     415,    -1,   415,   385,   415,    -1,   415,   385,   407,    -1,
     407,   385,   415,    -1,   415,   383,   415,    -1,   415,   383,
     407,    -1,   407,   383,   415,    -1,   415,   384,   415,    -1,
     415,   384,   407,    -1,   407,   384,   415,    -1,   415,   382,
     415,    -1,   415,   382,   407,    -1,   407,   382,   415,    -1,
     415,   381,   415,    -1,   415,   381,   407,    -1,   407,   381,
     415,    -1,   392,   415,    -1,   397,   415,   398,    -1,   388,
     415,    -1,    19,   402,   407,    -1,   372,   402,   407,    -1,
     373,   402,   407,    -1,   374,   402,   407,    -1,   414,   411,
     402,   407,    -1,   414,    -1,   417,   402,   415,    -1,   417,
     402,   407,    -1,    90,   376,    -1,    90,   376,   400,   401,
      -1,    90,   376,   400,   410,   401,    -1,    90,    19,    -1,
      90,    20,    -1,    70,    19,    -1,    70,    20,    -1,    35,
     408,   408,    -1,    35,   123,   446,    -1,   308,   408,   305,
     436,   129,   408,    -1,   308,   408,   305,   436,   129,   408,
      35,   408,    -1,   249,   446,    -1,   249,   305,   444,    -1,
     249,   468,    -1,   249,   466,    -1,   249,   469,    -1,   249,
     169,   407,   399,   407,   399,   407,   399,   407,    -1,   249,
     338,   407,   399,   407,    -1,   173,   249,   302,   432,    -1,
     309,   410,    -1,   218,   251,   410,   399,   410,    -1,   218,
     274,   410,   399,   410,    -1,    99,   408,   218,   274,   410,
     399,   410,    -1,    99,   408,   104,   407,    -1,    99,   408,
     122,    40,   446,    -1,    99,   408,   122,   446,    -1,    99,
     408,   215,   408,    -1,   137,    99,   408,    -1,   248,    18,
     413,    -1,    18,   324,   446,    -1,    18,   324,   363,   409,
      -1,    51,   468,    -1,   218,    51,   115,   446,    -1,   218,
     268,   407,   391,    -1,   218,   153,   407,   391,    -1,   173,
     218,   446,    -1,   283,   323,   407,   399,   407,   399,   407,
     399,   407,    -1,   299,   410,    -1,   295,   433,    -1,   322,
     432,    -1,   322,   433,    -1,   433,   173,   448,   408,    -1,
     433,   173,   446,    -1,   322,    64,    -1,   322,    64,   410,
      -1,    64,   446,    -1,    64,   432,    -1,    64,   407,   399,
     407,   399,   407,   399,   407,    -1,    64,   175,   454,    -1,
      64,   466,    -1,    64,   469,    -1,    64,   468,    -1,    64,
     115,   468,    -1,    64,   115,   467,    -1,    64,    88,    -1,
     322,   108,    -1,   322,   108,   410,    -1,   108,   446,    -1,
     108,   432,    -1,   108,   407,   399,   407,   399,   407,   399,
     407,    -1,   108,   175,   454,    -1,   108,   466,    -1,   108,
     469,    -1,   108,   468,    -1,   108,   115,   468,    -1,   108,
     115,   467,    -1,   108,    88,    -1,   322,   169,    -1,   322,
     169,   410,    -1,   169,   446,    -1,   169,   432,    -1,   169,
     407,   399,   407,   399,   407,   399,   407,    -1,   169,   175,
     454,    -1,   169,   469,    -1,   169,   466,    -1,   169,   468,
      -1,   169,    43,   410,    -1,   169,    43,   167,   407,    -1,
     169,    43,   305,   410,    -1,   169,    43,   164,   407,   399,
     407,    -1,   169,    88,    -1,   322,   290,    -1,   322,   290,
     410,    -1,   290,   446,    -1,   290,   432,    -1,   290,   407,
     399,   407,    -1,   290,   175,   454,    -1,   290,   468,    -1,
     290,   256,   410,    -1,   290,   465,    -1,   290,   160,   410,
      -1,   290,    67,   274,   407,    -1,   290,    88,   408,    -1,
     300,   446,    -1,   300,   465,    -1,   300,    67,   274,   407,
      -1,   300,   256,   410,    -1,   300,   468,    -1,   300,   407,
     399,   407,    -1,   300,    88,   408,    -1,    89,   466,    -1,
      89,   469,    -1,    89,   468,    -1,    89,   467,    -1,    89,
      67,   274,   407,    -1,    89,   465,    -1,    89,   294,   274,
     407,    -1,    89,   272,   408,    -1,   185,   122,   410,   302,
     408,   399,   408,    -1,   185,    75,   410,   302,   397,   410,
     399,   410,   399,   410,   398,   399,   408,    -1,   323,   407,
     399,   407,   399,   407,   399,   407,    -1,   371,   407,    -1,
     314,   407,   399,   407,   399,   407,   399,   407,    -1,   301,
     408,    -1,   301,   465,    -1,   301,   274,   407,    -1,   301,
     468,    -1,   292,   408,    -1,   292,   465,    -1,   292,   274,
     407,    -1,   292,   468,    -1,   333,   266,   445,    -1,   361,
     266,   445,    -1,   333,   158,   446,    -1,   361,   158,   446,
      -1,    47,   214,   204,    -1,    47,   214,   333,    -1,    47,
     214,   361,    -1,    47,   214,   339,    -1,    98,   408,    -1,
      70,    98,    -1,   166,   446,    -1,   166,   175,   454,    -1,
     166,   313,   410,    -1,   166,   140,   410,    -1,   166,   167,
     410,    -1,   166,   158,   446,    -1,   166,    64,   115,   468,
      -1,   166,    64,   115,   467,    -1,   166,    64,   468,    -1,
     166,    64,   467,    -1,   166,    64,   466,    -1,   166,    64,
     469,    -1,   166,   407,   399,   407,    -1,   166,    67,   274,
     407,    -1,   166,   465,    -1,   166,   468,    -1,   126,   446,
      -1,   126,   305,   410,    -1,   126,   466,    -1,   126,   469,
      -1,   126,   468,    -1,   126,   467,    -1,   126,    51,   468,
      -1,   126,    51,   467,    -1,   432,   446,    -1,   432,   141,
     446,    -1,   432,   305,   441,    -1,   432,   284,   446,    -1,
     432,    52,   140,   407,    -1,   432,   118,   446,    -1,   432,
     118,   124,   451,   451,    -1,   432,   118,   237,   407,   399,
     407,    -1,   432,   118,   338,   407,   399,   407,    -1,   432,
     118,   305,   410,    -1,   305,   440,    -1,    90,   438,   408,
     439,   408,    -1,    70,   438,    -1,   279,   448,    -1,   124,
     451,    -1,   116,   431,    -1,   372,    79,   446,    -1,   247,
      -1,   307,    -1,    65,   408,    -1,   107,   408,    -1,   107,
     407,    -1,    73,    -1,    73,   408,    -1,   111,    -1,   111,
     397,   409,   398,    -1,   239,    -1,   239,   302,    99,    -1,
     239,   302,   408,    -1,   218,   453,    -1,   276,   407,    -1,
     139,   408,    -1,   139,    -1,   131,   408,    -1,   242,   408,
      -1,   433,   141,   446,    -1,   433,   167,   410,    -1,    20,
     167,   410,    -1,   433,   229,   407,   399,   407,    -1,   433,
     105,   410,   399,   410,    -1,   278,   433,   457,   456,    -1,
      80,   433,   302,   433,    -1,    41,   433,   302,   433,    -1,
     253,   433,    -1,   282,   433,   410,    -1,   201,   433,   302,
     433,    -1,   293,   433,   380,   433,    -1,   161,   433,    -1,
     161,   433,   265,    -1,   161,   432,    -1,   161,   249,    -1,
     119,    -1,    42,   397,   410,   399,   410,   399,   407,   399,
     407,   399,   407,   398,    -1,    42,   397,   410,   399,   410,
     399,   407,   399,   407,   399,   407,   399,   446,   399,   446,
     399,   446,   398,    -1,    42,   397,   410,   399,   410,   399,
     407,   399,   407,   399,   407,   399,   446,   399,   446,   399,
     446,   399,   446,   398,    -1,   205,   397,   433,   399,   410,
     398,    -1,   205,   397,   433,   399,   415,   399,   410,   398,
      -1,   250,   397,   433,   399,   410,   398,    -1,   447,   397,
     433,   399,   410,   398,    -1,   458,   397,   433,   399,   410,
     398,    -1,   458,   397,   433,   399,   459,   399,   462,   399,
     460,   399,   461,   398,    -1,   156,   397,   433,   399,   415,
     399,   463,   399,   446,   398,    -1,   142,   397,   433,   399,
     415,   399,   446,   399,   446,   398,    -1,   101,   397,   433,
     399,   410,   398,    -1,   155,   397,   433,   398,    -1,   335,
     397,   433,   399,   433,   399,   410,   399,   446,   398,    -1,
     168,   397,   433,   399,   433,   398,    -1,   252,   397,   433,
     399,   415,   398,    -1,   252,   397,   433,   399,   249,   399,
     446,   398,    -1,    47,    -1,    47,   333,    -1,    47,   361,
      -1,    47,   433,    -1,    48,    -1,   120,   432,    -1,   244,
     408,    -1,   244,    56,   408,    -1,   244,    61,   408,    -1,
     244,    61,   448,   408,    -1,    61,   440,   408,    -1,   161,
      61,    -1,   244,   440,   408,    -1,   244,   440,   448,   408,
      -1,   244,   207,   408,    -1,   244,   207,   448,   408,    -1,
     325,   433,    -1,   325,   433,   124,   408,    -1,   325,   433,
     114,   408,    -1,   325,   433,   114,   408,   124,   408,    -1,
     265,   408,    -1,   174,   408,    -1,   203,    -1,   203,   129,
     408,    -1,   241,    -1,   234,    -1,    81,    -1,   283,   410,
      -1,    70,   283,    -1,    70,    64,    -1,    70,   108,    -1,
      70,   169,    -1,    70,   290,    -1,   218,   164,   442,    -1,
      46,   247,   446,    -1,   120,   446,    -1,   120,   270,    -1,
     120,   121,    -1,   120,    71,    -1,   425,    -1,   474,    -1,
     433,   446,    -1,   433,   305,   440,    -1,   433,   294,   410,
      -1,   433,   294,   468,    -1,   433,   294,   467,    -1,   433,
     294,   469,    -1,   433,   294,   466,    -1,   433,   294,   115,
     468,    -1,   433,   294,   115,   467,    -1,   433,   294,   274,
     407,    -1,   433,   294,    67,   410,    -1,   433,   294,    67,
     465,    -1,   433,   294,   275,   410,    -1,   433,   169,   305,
     410,    -1,   433,   169,   466,    -1,   433,   169,   469,    -1,
     433,   169,   468,    -1,   433,   169,   467,    -1,   433,   115,
     305,   410,    -1,   433,   115,   259,   410,    -1,   433,   115,
     468,    -1,   433,   115,   467,    -1,   433,    55,   446,    -1,
     433,    55,   305,   410,    -1,   433,   106,   446,    -1,   433,
      49,   446,    -1,   433,    49,   305,   410,    -1,   433,    49,
      67,   274,   407,    -1,   433,    49,   465,    -1,   433,    49,
     468,    -1,   433,    49,   256,   410,    -1,   433,    49,   124,
     451,    -1,   433,    49,   237,   410,    -1,   433,    49,   209,
     407,   399,   407,    -1,   433,    49,   238,   408,    -1,   433,
      49,    41,   408,    -1,   433,   110,   446,    -1,   433,   110,
     470,    -1,   433,   110,   468,    -1,   433,   110,   467,    -1,
     433,   110,   274,   407,    -1,   433,   110,   469,    -1,   433,
     110,   466,    -1,   433,   110,   255,   469,    -1,   433,   110,
     255,   466,    -1,   433,   110,   255,    72,   446,    -1,   433,
     110,   255,    72,   167,   407,    -1,   433,    76,   408,    -1,
     433,   166,   408,    -1,   446,    -1,   305,   370,   446,    -1,
     297,   427,    -1,   297,   475,    -1,   296,   428,    -1,   296,
     476,    -1,   162,   429,    -1,   162,   473,    -1,    52,   430,
      -1,   209,   407,   399,   407,    -1,   446,    -1,   184,   407,
      -1,   189,   298,   410,    -1,   228,   257,   446,    -1,   210,
     407,    -1,   211,   407,    -1,    89,   410,    -1,   450,    -1,
     184,   274,   407,    -1,   189,   274,   407,    -1,   468,    -1,
     184,   468,    -1,   189,   468,    -1,   469,    -1,   184,   469,
      -1,   189,   469,    -1,   184,   466,    -1,   189,   466,    -1,
     184,   134,   446,    -1,   189,   134,   446,    -1,   470,    -1,
     280,   305,   437,    -1,   280,   410,    -1,   184,   410,   399,
     407,    -1,   189,   410,   399,   407,    -1,   446,    -1,   237,
     410,    -1,   124,   451,    -1,   124,   407,    -1,    41,   408,
      -1,   238,   408,    -1,    39,   410,    -1,   275,   410,    -1,
     287,   410,    -1,   470,    -1,   125,   408,    -1,   288,   407,
      -1,   289,   407,    -1,   288,   305,   280,    -1,   288,   305,
      46,    -1,   289,   305,   280,    -1,   289,   305,    46,    -1,
      67,   274,   407,    -1,   465,    -1,   468,    -1,   410,   399,
     408,    -1,   209,    46,    -1,   209,   280,    -1,   209,   407,
     399,   407,    -1,   408,    -1,   164,   224,    -1,   164,   219,
      -1,   228,    46,    -1,   228,   280,    -1,   228,   407,   399,
     407,    -1,   160,   449,    -1,    67,   274,   407,    -1,   465,
      -1,   468,    -1,   470,    -1,   446,    -1,   468,    -1,   466,
      -1,   469,    -1,   301,   408,    -1,   125,   408,    -1,   322,
     410,   220,    -1,   237,   407,    -1,   133,    -1,   132,   411,
      -1,   432,   395,   271,    -1,   432,   395,   270,   411,    -1,
     271,    -1,   270,   411,    -1,   435,   426,    -1,   432,   435,
     426,    -1,   334,    -1,   362,    -1,    37,    -1,    38,    -1,
       5,    -1,     6,    -1,     7,    -1,     8,    -1,    10,    -1,
       9,    -1,    11,    -1,    12,    -1,    13,    -1,    14,    -1,
      15,    -1,    16,    -1,   204,    -1,   298,    -1,    62,    -1,
     148,    -1,   212,    -1,   182,    -1,   222,    -1,   338,    -1,
      52,    -1,    53,    -1,    54,    -1,   354,    -1,   343,    -1,
     347,    -1,   344,    -1,   348,    -1,   346,    -1,   345,    -1,
     349,    -1,   350,    -1,   351,    -1,   341,    -1,   342,    -1,
     353,    -1,   340,    -1,   352,    -1,   338,    -1,    68,    -1,
     230,    -1,   231,    -1,   277,    -1,   117,    -1,   226,    -1,
     127,    -1,   117,    -1,   163,    -1,   235,    -1,    33,    -1,
      58,    -1,   165,    -1,   254,    -1,   232,    -1,   233,    -1,
     144,    -1,   310,    -1,   146,    -1,   312,    -1,   206,    -1,
     177,    -1,   246,    -1,   178,    -1,   213,    -1,   208,    -1,
     260,    -1,   264,    -1,   262,    -1,   261,    -1,   263,    -1,
     102,    -1,   227,    -1,   254,    -1,   165,    -1,    66,    -1,
     151,    -1,   217,    -1,    62,    -1,    87,    -1,   112,    -1,
     130,    -1,   267,    -1,   109,    -1,    78,    -1,   236,    -1,
      86,    -1,   192,    -1,   366,    -1,   196,    -1,   190,    -1,
     197,    -1,    82,    -1,   199,    -1,   200,    -1,   198,    -1,
      84,    -1,    83,    -1,    85,    -1,   143,    -1,   191,    -1,
     193,    -1,   367,    -1,    92,    -1,    94,    -1,    96,    -1,
     195,    -1,    91,    -1,    93,    -1,    95,    -1,   194,    -1,
     369,    -1,   368,    -1,   206,    -1,    34,    -1,   202,    -1,
     306,    -1,   103,    -1,   254,    -1,   165,    -1,   151,    -1,
     217,    -1,   323,    -1,   314,    -1,   330,    -1,   355,    -1,
     331,    -1,   356,    -1,   357,    -1,   358,    -1,   359,    -1,
     360,    -1,    44,    -1,    97,    -1,   330,    -1,   355,    -1,
     100,    -1,   113,    -1,   157,    -1,   159,    -1,   245,    -1,
      77,    -1,    17,    -1,   128,    -1,   223,    -1,   183,    -1,
     225,    -1,    74,    -1,   204,    -1,   304,    -1,   136,    -1,
     321,    -1,   135,    -1,    60,    -1,   221,    -1,   170,    -1,
     281,    -1,    45,    -1,   188,    -1,   186,    -1,    50,    -1,
     269,    -1,   291,    -1,   150,    -1,   149,    -1,   122,   410,
      -1,   122,   408,    -1,   171,   410,    -1,   222,   410,    -1,
      75,   410,    -1,    75,   408,    -1,    75,   397,   410,   399,
     410,   399,   410,   398,    -1,   172,   407,    -1,   228,   471,
      -1,   206,    -1,   216,    -1,    62,    -1,   218,   164,   443,
      -1,   218,   274,   375,   375,    -1,   218,   410,    -1,   218,
     153,   410,    -1,    89,   122,   279,   407,    -1,   283,   323,
     407,   399,   407,   399,   407,   399,   407,   297,   407,   399,
     407,   399,   407,   399,   407,    -1,    64,   115,   477,    -1,
     108,   115,   477,    -1,   290,   469,    -1,   300,   469,    -1,
     301,   469,    -1,   292,   469,    -1,   166,    64,   446,    -1,
     166,   332,   407,    -1,   166,   357,   407,    -1,   166,   290,
     410,   408,    -1,   166,    64,   115,   446,    -1,   166,    64,
     115,   322,   477,    -1,   166,   466,    -1,   166,   469,    -1,
     432,   162,   446,    -1,   432,   305,   179,    -1,   432,   305,
     181,    -1,   432,   305,   180,    -1,   432,   305,    52,    -1,
     432,   305,   138,    -1,   432,   305,   285,    -1,   432,   305,
     286,    -1,   323,   337,   407,    -1,   323,   336,   407,    -1,
     323,   365,   407,    -1,   323,   364,   407,    -1,   314,   337,
     407,    -1,   314,   336,   407,    -1,   314,   365,   407,    -1,
     314,   364,   407,    -1,   166,   164,   407,    -1,   126,   115,
     446,    -1,   432,    47,   305,    46,    -1,   432,    47,   305,
     280,    -1,   169,    43,   274,   407,    -1,   137,    99,   407,
      -1,   240,   172,    57,   407,    -1,   240,   172,   152,   407,
      -1,   240,   469,    -1,   469,    -1,   478,    -1,   433,   294,
     115,   410,    -1,   433,   275,   410,    -1,   433,   115,   410,
      -1,   433,   110,   305,   479,    -1,   433,   294,    66,   446,
      -1,   433,   466,    -1,   433,   469,    -1,   433,   468,    -1,
     433,   115,   322,   477,    -1,   433,   354,   407,   399,   407,
      -1,   433,   110,   167,   407,    -1,   433,   110,   255,   446,
      -1,   184,   446,    -1,   189,   446,    -1,    36,   446,    -1,
     188,   375,    -1,   186,   375,    -1,   176,   446,    -1,   305,
      46,    -1,   305,   280,    -1,   189,   407,    -1,   274,   407,
      -1,   410,   399,   407,    -1,   478,    -1,   469,    -1,   305,
      46,    -1,   305,   280,    -1,   164,   280,    -1,   164,   145,
      -1,   164,   311,    -1,   228,   213,   298,    -1,   228,    59,
     298,    -1,   478,    -1,   273,   452,    -1,   204,    -1,    75,
      -1,   222,    -1,   215,   479,    -1,   303,    -1,    63,    -1,
     165,    -1,   254,    -1,    62,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   635,   635,   636,   640,   647,   648,   651,   653,   654,
     655,   656,   657,   658,   659,   660,   661,   662,   663,   670,
     673,   676,   679,   682,   685,   692,   726,   734,   737,   740,
     743,   746,   750,   754,   758,   762,   766,   770,   774,   778,
     782,   786,   790,   794,   798,   801,   804,   807,   810,   813,
     816,   819,   822,   825,   828,   832,   839,   846,   853,   860,
     867,   874,   881,   888,   893,   898,   901,   904,   907,   910,
     913,   916,   925,   933,   944,   947,   950,   953,   956,   959,
     962,   965,   968,   971,   976,   979,   983,   992,  1002,  1011,
    1021,  1024,  1039,  1042,  1058,  1062,  1075,  1091,  1095,  1117,
    1135,  1153,  1169,  1198,  1208,  1218,  1228,  1243,  1254,  1265,
    1276,  1287,  1298,  1309,  1320,  1331,  1346,  1357,  1368,  1383,
    1394,  1405,  1420,  1431,  1442,  1461,  1476,  1491,  1511,  1526,
    1542,  1565,  1584,  1603,  1613,  1622,  1635,  1648,  1661,  1676,
    1687,  1698,  1713,  1724,  1735,  1750,  1761,  1772,  1787,  1798,
    1809,  1824,  1835,  1846,  1861,  1872,  1883,  1898,  1909,  1920,
    1935,  1946,  1957,  1967,  1977,  1990,  1994,  1998,  2002,  2006,
    2017,  2044,  2055,  2065,  2082,  2090,  2101,  2105,  2109,  2114,
    2120,  2138,  2141,  2148,  2159,  2162,  2165,  2168,  2171,  2174,
    2181,  2189,  2196,  2206,  2209,  2212,  2227,  2241,  2255,  2269,
    2284,  2288,  2291,  2294,  2297,  2300,  2303,  2306,  2309,  2313,
    2318,  2322,  2326,  2329,  2334,  2338,  2343,  2346,  2353,  2360,
    2367,  2377,  2380,  2383,  2386,  2389,  2392,  2395,  2426,  2429,
    2436,  2443,  2450,  2460,  2463,  2466,  2469,  2472,  2475,  2478,
    2509,  2512,  2519,  2526,  2533,  2543,  2546,  2549,  2552,  2555,
    2558,  2561,  2564,  2568,  2585,  2588,  2595,  2602,  2609,  2617,
    2620,  2623,  2626,  2629,  2632,  2635,  2651,  2654,  2657,  2660,
    2663,  2666,  2670,  2676,  2680,  2684,  2688,  2691,  2695,  2699,
    2702,  2706,  2714,  2727,  2737,  2740,  2750,  2758,  2765,  2772,
    2779,  2787,  2794,  2801,  2809,  2816,  2823,  2830,  2837,  2840,
    2843,  2846,  2850,  2859,  2863,  2870,  2877,  2884,  2891,  2898,
    2905,  2912,  2919,  2926,  2933,  2940,  2947,  2955,  2962,  2969,
    2977,  2984,  2991,  2998,  3005,  3012,  3019,  3027,  3036,  3039,
    3042,  3045,  3049,  3053,  3056,  3060,  3064,  3068,  3072,  3077,
    3084,  3088,  3091,  3094,  3095,  3101,  3104,  3111,  3115,  3119,
    3126,  3129,  3132,  3135,  3138,  3145,  3148,  3153,  3175,  3180,
    3188,  3195,  3200,  3212,  3215,  3218,  3221,  3225,  3230,  3235,
    3238,  3248,  3251,  3254,  3257,  3260,  3263,  3266,  3269,  3272,
    3275,  3278,  3284,  3290,  3297,  3309,  3312,  3315,  3318,  3338,
    3342,  3346,  3349,  3352,  3355,  3358,  3375,  3390,  3395,  3400,
    3405,  3408,  3411,  3419,  3424,  3428,  3432,  3436,  3449,  3452,
    3458,  3465,  3469,  3473,  3480,  3488,  3496,  3505,  3513,  3517,
    3520,  3524,  3527,  3530,  3533,  3537,  3540,  3543,  3546,  3549,
    3556,  3561,  3564,  3567,  3570,  3573,  3580,  3581,  3585,  3588,
    3592,  3595,  3598,  3601,  3604,  3607,  3610,  3613,  3616,  3619,
    3622,  3626,  3630,  3634,  3638,  3642,  3647,  3651,  3655,  3673,
    3693,  3697,  3702,  3707,  3711,  3715,  3719,  3723,  3727,  3731,
    3735,  3739,  3743,  3748,  3754,  3757,  3760,  3763,  3766,  3769,
    3772,  3775,  3778,  3781,  3784,  3788,  3793,  3801,  3808,  3815,
    3816,  3817,  3818,  3819,  3820,  3821,  3822,  3833,  3840,  3847,
    3854,  3862,  3869,  3876,  3883,  3890,  3897,  3904,  3911,  3918,
    3925,  3932,  3939,  3946,  3953,  3960,  3967,  3974,  3981,  3988,
    3995,  4003,  4014,  4021,  4028,  4035,  4042,  4051,  4060,  4067,
    4074,  4081,  4088,  4097,  4104,  4111,  4118,  4125,  4132,  4139,
    4146,  4153,  4160,  4177,  4184,  4191,  4202,  4210,  4217,  4224,
    4231,  4238,  4246,  4253,  4260,  4267,  4274,  4284,  4291,  4298,
    4305,  4315,  4319,  4323,  4326,  4332,  4336,  4343,  4356,  4369,
    4382,  4398,  4399,  4403,  4404,  4405,  4406,  4410,  4411,  4412,
    4413,  4414,  4415,  4416,  4417,  4418,  4419,  4420,  4421,  4425,
    4426,  4427,  4431,  4432,  4436,  4437,  4441,  4442,  4443,  4444,
    4445,  4446,  4447,  4448,  4449,  4450,  4451,  4452,  4453,  4454,
    4455,  4456,  4457,  4458,  4459,  4463,  4464,  4465,  4466,  4467,
    4468,  4469,  4473,  4474,  4478,  4479,  4483,  4484,  4485,  4486,
    4487,  4488,  4489,  4490,  4491,  4492,  4495,  4496,  4497,  4498,
    4501,  4502,  4505,  4506,  4507,  4508,  4509,  4513,  4514,  4524,
    4525,  4526,  4529,  4530,  4531,  4534,  4535,  4536,  4537,  4538,
    4539,  4540,  4541,  4542,  4543,  4544,  4545,  4546,  4547,  4548,
    4549,  4550,  4551,  4552,  4553,  4554,  4555,  4556,  4557,  4558,
    4559,  4560,  4561,  4562,  4563,  4564,  4565,  4566,  4567,  4570,
    4571,  4572,  4575,  4576,  4577,  4578,  4579,  4580,  4583,  4584,
    4587,  4588,  4589,  4590,  4591,  4592,  4593,  4594,  4597,  4598,
    4601,  4602,  4605,  4606,  4607,  4608,  4612,  4613,  4617,  4618,
    4619,  4623,  4624,  4625,  4629,  4630,  4631,  4632,  4633,  4634,
    4635,  4639,  4640,  4641,  4644,  4645,  4646,  4647,  4648,  4649,
    4650,  4654,  4658,  4666,  4679,  4692,  4702,  4712,  4731,  4746,
    4752,  4753,  4754,  4759,  4771,  4774,  4777,  4781,  4784,  4789,
    4791,  4793,  4795,  4797,  4798,  4800,  4809,  4816,  4823,  4831,
    4832,  4833,  4834,  4836,  4838,  4842,  4846,  4852,  4858,  4863,
    4868,  4875,  4882,  4889,  4896,  4904,  4911,  4918,  4925,  4933,
    4936,  4944,  4946,  4949,  4953,  4954,  4955,  4956,  4961,  4962,
    4972,  4986,  4990,  5013,  5021,  5022,  5025,  5028,  5031,  5032,
    5033,  5036,  5041,  5049,  5050,  5051,  5052,  5053,  5054,  5061,
    5070,  5082,  5089,  5097,  5107,  5108,  5117,  5124,  5126,  5133,
    5140,  5141,  5142,  5149,  5171,  5172,  5173,  5176,  5182,  5183,
    5184,  5185,  5186
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
  "PIPE", "PLACE", "POINT", "POLAR", "POLARV2", "POLYI", "POLYO", "POP",
  "PORTRAIT", "POWER", "PREC", "PREPEND", "PRINT", "PS", "PUSH", "PUTP",
  "RAND", "READ", "REAL", "RECIPROCAL", "REDRAW", "REFERENCE", "REGNUM",
  "REGRESS", "RESIZE", "RESTRICT", "REVERSE", "RIGHT", "RISER", "ROT",
  "ROUNDED", "RSUM", "RULE", "RUNAVG", "RUNMAX", "RUNMED", "RUNMIN",
  "RUNSTD", "SAVEALL", "SCALE", "SCIENTIFIC", "SCROLL", "SD", "SET",
  "SETNUM", "SFORMAT", "SIGN", "SIZE", "SKIP", "SLEEP", "SMITH", "SORT",
  "SOURCE", "SPEC", "SPLINE", "SPLIT", "STACK", "STACKED", "STACKEDBAR",
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
  "YEAR", "YMAX", "YMIN", "YYMMDD", "YYMMDDHMS", "YYDYHMS", "YYYY", "ZERO",
  "ZNORM", "FITPARM", "FITPMAX", "FITPMIN", "NUMBER", "NEW_TOKEN", "'?'",
  "':'", "OR", "AND", "NE", "EQ", "GE", "LE", "LT", "GT", "'+'", "'-'",
  "'*'", "'/'", "'%'", "NOT", "UMINUS", "'^'", "'.'", "';'", "'('", "')'",
  "','", "'['", "']'", "'='", "$accept", "full_list", "multi_list", "list",
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
     625,   626,   627,   628,   629,   630,   631,    63,    58,   632,
     633,   634,   635,   636,   637,   638,   639,    43,    45,    42,
      47,    37,   640,   641,    94,    46,    59,    40,    41,    44,
      91,    93,    61
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   403,   404,   404,   404,   405,   405,   406,   406,   406,
     406,   406,   406,   406,   406,   406,   406,   406,   406,   407,
     407,   407,   407,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   407,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   407,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   407,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   407,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   407,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   407,   407,   408,   408,   408,   409,   410,   411,
     412,   412,   413,   413,   414,   414,   414,   415,   415,   415,
     415,   415,   415,   415,   415,   415,   415,   415,   415,   415,
     415,   415,   415,   415,   415,   415,   415,   415,   415,   415,
     415,   415,   415,   415,   415,   415,   415,   415,   415,   415,
     415,   415,   415,   415,   415,   415,   415,   415,   415,   415,
     415,   415,   415,   415,   415,   415,   415,   415,   415,   415,
     415,   415,   415,   415,   415,   415,   415,   415,   415,   415,
     415,   415,   415,   415,   415,   416,   416,   416,   416,   416,
     417,   418,   418,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   420,   420,   420,   420,   420,   420,
     420,   420,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     423,   423,   423,   423,   423,   423,   424,   424,   425,   425,
     425,   425,   425,   425,   425,   425,   425,   425,   425,   425,
     425,   425,   425,   425,   425,   425,   425,   425,   425,   425,
     425,   425,   425,   425,   425,   425,   425,   425,   425,   425,
     425,   425,   425,   425,   425,   425,   425,   425,   425,   425,
     425,   425,   425,   425,   425,   425,   425,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   427,   427,   427,
     427,   427,   427,   427,   427,   427,   427,   427,   427,   427,
     427,   427,   427,   427,   427,   427,   427,   427,   427,   427,
     427,   427,   428,   428,   428,   428,   428,   428,   428,   428,
     428,   428,   428,   428,   428,   428,   428,   428,   428,   428,
     428,   428,   428,   428,   428,   428,   429,   429,   429,   429,
     429,   429,   429,   429,   429,   429,   429,   430,   430,   430,
     430,   431,   431,   431,   431,   432,   432,   433,   433,   433,
     433,   434,   434,   435,   435,   435,   435,   436,   436,   436,
     436,   436,   436,   436,   436,   436,   436,   436,   436,   437,
     437,   437,   438,   438,   439,   439,   440,   440,   440,   440,
     440,   440,   440,   440,   440,   440,   440,   440,   440,   440,
     440,   440,   440,   440,   440,   441,   441,   441,   441,   441,
     441,   441,   442,   442,   443,   443,   444,   444,   444,   444,
     444,   444,   444,   444,   444,   444,   445,   445,   445,   445,
     446,   446,   447,   447,   447,   447,   447,   448,   448,   449,
     449,   449,   450,   450,   450,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   451,
     451,   451,   451,   451,   451,   451,   451,   451,   451,   452,
     452,   452,   453,   453,   453,   453,   453,   453,   454,   454,
     455,   455,   455,   455,   455,   455,   455,   455,   456,   456,
     457,   457,   458,   458,   458,   458,   459,   459,   460,   460,
     460,   461,   461,   461,   462,   462,   462,   462,   462,   462,
     462,   463,   463,   463,   464,   464,   464,   464,   464,   464,
     464,   465,   465,   466,   467,   468,   468,   468,   469,   470,
     471,   471,   471,   472,   472,   472,   472,   472,   472,   472,
     472,   472,   472,   472,   472,   472,   472,   472,   472,   472,
     472,   472,   472,   472,   472,   472,   472,   472,   472,   472,
     472,   472,   472,   472,   472,   472,   472,   472,   472,   472,
     472,   472,   472,   472,   472,   472,   472,   472,   473,   473,
     474,   474,   474,   474,   474,   474,   474,   474,   474,   474,
     474,   474,   475,   475,   475,   475,   475,   475,   475,   475,
     475,   475,   475,   475,   476,   476,   476,   476,   476,   476,
     476,   476,   476,   476,   477,   477,   477,   478,   479,   479,
     479,   479,   479
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     8,     2,     2,
       1,     1,     1,     3,     4,     2,     3,     4,    17,     3,
       3,     2,     2,     2,     2,     3,     3,     3,     4,     4,
       5,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     4,     4,     4,     3,     4,     4,     2,     1,     1,
       4,     3,     3,     4,     4,     2,     2,     2,     4,     5,
       4,     4,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     3,     1,     1,     2,     2,     2,     2,     2,
       3,     3,     1,     2,     1,     1,     1,     2,     1,     1,
       1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,    18,     0,    20,    94,    31,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   575,   576,     0,
       0,     0,   397,   401,   736,     0,     0,     0,     0,     0,
     350,     0,   423,     0,     0,     0,     0,   712,     0,     0,
       0,   352,   713,     0,   379,     0,     0,     0,     0,     0,
     565,     0,   360,     0,   740,   739,     0,     0,     0,   714,
     715,     0,     0,     0,     0,     0,     0,     0,   735,     0,
     734,     0,   419,     0,     0,   422,   354,     0,   421,     0,
      33,     0,   345,     0,     0,     0,     0,     0,     0,   642,
     645,   644,   646,   643,     0,   737,     0,   569,     0,     0,
       0,     0,     0,     0,   738,     0,     0,     0,     0,     0,
       0,     0,   346,     0,     0,     0,    55,    56,    63,    57,
      58,    64,     0,     0,     0,    59,    60,    61,    62,   700,
     702,     0,   573,     0,   701,   703,   704,   705,   706,   707,
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
       0,   641,   640,    33,    21,    22,    23,     0,     0,     0,
       0,     0,   219,     0,   218,   222,   224,   223,   347,   178,
     179,   426,   303,   427,   592,   428,   593,   425,   429,   340,
     351,     0,     0,     0,     0,     0,     0,   278,   273,   276,
     275,   274,   176,   177,   173,     0,   302,     0,     0,   349,
     348,     0,   239,     0,     0,     0,   231,   230,   234,   236,
     235,     0,     0,     0,     0,     0,   343,   435,   434,   433,
     402,   432,   660,   668,   673,   672,   674,   662,   655,   683,
     679,   684,   680,   685,   681,   659,   656,   657,   675,   666,
     676,   663,   677,   686,   682,   665,   667,   671,   669,   670,
     661,   658,   664,   678,   688,   687,   342,     0,     0,     0,
     320,   322,   325,   324,   323,   361,     0,   566,     0,   359,
       0,     0,     0,     0,   408,   378,   377,   375,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   704,
       0,   304,   318,   771,   319,   772,     0,     0,   253,     0,
       0,   243,   242,   247,   248,   246,     0,     0,   418,     0,
       0,     0,     0,     0,     0,     0,   693,   696,     0,     0,
     695,   697,     0,   694,     0,     0,   692,    87,    88,   755,
     357,     0,     0,   797,   362,     0,     0,     0,     0,   403,
       0,     0,     0,     0,     0,     0,   184,   187,   186,   188,
       0,     0,   371,     0,   417,   570,   358,     0,   647,   648,
     341,     0,     0,   424,     0,     0,     0,     0,     0,     0,
     257,   256,   262,   260,   761,     0,   290,   291,   293,   764,
       0,   211,   210,     0,     0,     0,     0,   266,   267,   270,
     762,     0,   286,   287,   289,   763,   338,     0,   192,     0,
       0,     0,     0,     0,   216,   228,   240,   254,   212,   213,
       0,     0,     0,     0,     0,   413,     0,     0,     0,     0,
       0,   284,     0,     0,     0,     0,    69,     0,    68,    97,
     164,    83,   162,     0,     0,     1,     0,    32,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    24,   133,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   328,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   438,   805,   807,   806,     0,     0,     0,     0,     0,
       0,   571,   487,     0,     0,     0,     0,   202,    87,    91,
       0,     0,   165,   365,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   181,     0,
     180,     0,     0,     0,   431,   298,   299,   301,   300,     0,
     746,   745,   407,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   835,   834,   836,   226,   225,   759,   743,   748,
     699,   698,   221,    68,    83,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    24,     0,     0,     0,   742,   741,   744,   280,     0,
       0,     0,     0,     0,     0,     0,     0,   238,   237,   760,
     233,     0,     0,   562,   564,   561,     0,   327,   326,   790,
     321,     0,   794,   200,     0,     0,     0,     0,     0,   376,
       0,   765,   314,   313,   312,   315,     0,   307,   309,   789,
     308,   305,     0,   306,   766,   767,     0,     0,     0,     0,
       0,     0,   249,   245,     0,   208,     0,     0,     0,    87,
       0,     0,   420,     0,     0,    87,   756,   623,   622,   624,
     625,   430,   753,     0,     0,    19,     0,   355,   356,     0,
       0,     0,   404,   405,     0,   411,     0,   409,     0,    92,
      93,   201,     0,   626,   627,   632,   634,   628,   630,   631,
     629,   633,   635,   185,     0,     0,     0,     0,     0,   710,
     711,     0,   372,     0,     0,   265,   263,   259,   261,     0,
     292,     0,     0,   272,   269,     0,   288,     0,   786,   785,
     788,   787,     0,   217,   229,   241,   255,   782,   781,   784,
     783,     0,     0,     0,   296,   637,   639,   636,   638,   294,
       0,   297,   295,   344,   166,   167,   168,    65,   163,     0,
       0,     0,     0,     0,     0,     6,   170,     0,     0,     0,
      82,   140,    81,   143,    80,   161,    79,   158,    78,   152,
      77,   155,    76,   149,    75,   146,    66,   117,    67,   120,
      70,   123,    71,   126,    72,   129,    73,   132,    27,     0,
       0,     0,     0,   139,   138,   142,   141,   160,   159,   157,
     156,   151,   150,   154,   153,   148,   147,   145,   144,   116,
     115,   119,   118,   122,   121,   125,   124,   128,   127,   131,
     130,   172,   171,     0,     0,     0,     0,     0,     0,   333,
     329,   773,   331,   777,   616,   620,   778,   774,   776,   775,
     621,   617,   618,   619,   779,   780,   615,   330,    30,     0,
     567,    44,    45,    46,    47,    48,    49,    50,    51,   572,
       0,     0,     0,     0,     0,     0,     0,     0,   463,   466,
     467,     0,   460,   485,     0,   462,     0,     0,     0,     0,
       0,   474,   480,   477,   476,   479,   475,     0,     0,     0,
     802,   459,   458,   363,   486,   364,     0,   452,   455,   454,
     453,   215,     0,     0,   801,     0,     0,     0,     0,     0,
     440,   444,   442,   441,   443,   439,     0,    29,    28,    96,
     495,   557,   559,   558,   560,     0,     0,     0,     0,     0,
     546,   493,   554,   555,   798,   556,   494,   799,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   491,   522,   540,   541,
     824,   531,   492,   832,     0,   654,     0,   652,     0,     0,
     735,   734,     0,     0,     0,   653,     0,     0,     0,     0,
       0,   489,   497,   504,   507,   510,   517,   490,   823,     0,
       0,     0,     0,   203,     0,    52,    34,   104,    35,   105,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
      85,   370,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    82,    81,    80,    79,    78,    77,    76,
      75,    66,    67,    70,    71,    72,    73,     0,   369,   277,
     757,   279,   174,     0,   594,   595,     0,   196,     0,   198,
     199,     0,     0,     0,   353,   563,    89,     0,     0,   392,
       0,     0,   769,   311,   310,   317,   768,   316,     0,     0,
     250,   793,   251,     0,   191,     0,     0,     0,    99,   373,
       0,   205,   207,     0,   206,   754,     0,   795,   796,   101,
     406,   412,   410,     0,     0,   102,     0,     0,   103,   708,
     709,   368,     0,   264,   258,   374,   268,   271,   577,   578,
     579,   580,   582,   581,   583,   584,   585,   586,   587,   588,
       0,     0,     0,   415,   414,     0,     0,     0,     0,     0,
     169,     0,     0,   791,   792,   332,     0,     0,   337,     0,
     568,   473,     0,   469,     0,   470,   472,   468,   464,   461,
       0,   810,   752,   750,   751,   749,     0,   811,   482,   481,
     478,   842,   839,   840,   841,   838,   803,   457,   456,   835,
     836,   808,   451,   214,     0,   804,   448,   449,   800,   446,
     445,   447,   450,     0,     0,   651,   650,   649,   552,   548,
     547,   837,   549,   550,     0,     0,   528,   526,     0,   525,
     524,   532,   828,   827,   829,   543,   544,     0,     0,     0,
     523,   527,   690,   691,   689,   833,   529,   530,     0,   533,
       0,   534,   825,   826,     0,   814,   503,   817,     0,     0,
     498,     0,   812,   513,   508,   511,   816,   815,     0,     0,
       0,   820,     0,   813,   514,   509,   512,   501,   502,     0,
     821,     0,   519,   818,   819,     0,   488,     0,     0,    25,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   175,   339,   197,     0,     0,     0,     0,
       0,     0,   770,     0,     0,     0,     0,     0,     0,     0,
       0,   193,   194,     0,   190,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,   134,   135,   136,   137,
     334,     0,     0,   465,     0,   367,     0,   483,   366,   809,
     553,     0,   496,   539,     0,   831,   830,   536,   535,   538,
     537,   542,   515,   505,     0,   516,   506,   499,     0,   500,
     591,   589,   590,   518,   822,     0,   717,   716,     0,     0,
       0,    37,    36,   109,    38,   107,   108,   106,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   391,     0,     0,    26,     0,   394,
     252,     0,     0,     0,     0,   383,     0,     0,   385,     0,
     395,     0,   182,     0,     0,   416,     0,    98,   335,   336,
     471,   484,   551,   545,   520,   521,   386,   387,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   195,     0,     0,   733,   731,
     732,     0,     0,     0,   281,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   729,   728,   726,   724,   730,   725,
     727,     0,    53,     0,    39,   110,    40,   111,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   220,   232,
       0,     0,   244,     0,   100,   384,     0,   396,     0,   183,
     285,   283,     0,     0,     0,     0,     0,     0,     0,     0,
     747,     0,     0,     0,     0,     0,     0,   189,   209,     0,
     718,   719,   720,     0,     0,    41,   112,     0,     0,     0,
       0,     0,     0,   390,   389,     0,     0,   393,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   723,   721,
     722,     0,     0,    42,   113,     0,   380,     0,     0,     0,
       0,     0,   388,     0,     0,     0,     0,     0,   282,     0,
      54,    43,   114,     0,     0,     0,     0,     0,     0,     0,
       0,   758,   381,     0,     0,   382
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,   151,   152,   153,   417,   640,   418,   641,   681,   611,
     781,   251,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   601,  1081,  1056,  1031,  1020,   306,   291,
     253,   169,   170,  1220,  1453,   269,  1146,   223,   947,   761,
     762,   793,   839,   436,   171,   450,  1288,  1083,   346,  1315,
     420,   662,   172,  1201,   801,   173,  1459,  1613,  1641,  1561,
    1541,   174,   277,   437,   279,   438,   439,   986,  1255,   175,
    1036,   176,  1087,  1062,   657,  1037,  1266
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -903
static const yytype_int16 yypact[] =
{
    3037,  -903,  -289,  -357,  -102,  -903,  -325,  -318,  -306,  -287,
    -275,  -246,  -217,  -211,  -186,  -183,    18,  -903,  -903,    65,
    -170,  -141,    30,  -903,  -903,    59,  4257,  5549,   166,    57,
     166,    65,  -903,   353,    -2,   166,   166,  -903,  -145,  9022,
    5740,  -142,  -903,    68,  -903,   686, 11197,    49,   166,  -128,
    -903,   162,   166,  -110,  -903,  -903,  -106,   -85,   -81,  -903,
    -903,   235,  4636,   -74,  5931,  -129,   166,    77,  -903,   -56,
    -903,    65,   179,   -52,  4866,  -903,    -7,   193,  -903,   166,
     -17,  5058,  -903,   358,   -55,     7,     9,    65,    39,  -903,
    -903,  -903,  -903,  -903,   166,  -903,  -128,  -903, 10244,    65,
     -35,    65,  9114,  5123,  -903,    50,    65,    65, 10244,  6122,
     167,  4257,  -903,   166, 10244,  7105,  -903,  -903,  -903,  -903,
    -903,  -903,   317,  7425,    65,  -903,  -903,  -903,  -903,  -903,
    -903,  -121,  -903,    43,  -903,  -903,  -903,  -903,  -903,  -903,
     -99,  -903, 10244,   -65,   -10,    16,  -903, 10244,  7800,  7800,
    7800,   448,    72,  -903,  3983,    42,  5361,  -903,    71,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,  -903,    -6, 11011,  -903,
     142,    79,  -903,    81,   107,  -903,  -903,  -139,  9022, 10244,
   10244,  7800,  7800, 10244, 10244,  7800, 10244, 10244, 10244, 10244,
   10244,  -903,   118,   -62,   115,   209, 10244,   118,   160,  -903,
    -903,  -903,  9212,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,   166,   119,  -903,  -903,   121,   125,   129,
     149,   168,   180,   182,   183,   184,  -903,    29, 10244, 10244,
      -5,  -903,  -903,  -903,  -903,  -903,  -903, 10244, 10244, 10244,
     633,    24,   169,   173,  -903,  -903,  -903,  -903,   178,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
     178,   269,   308,  7990, 10244,   166,   322,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,   191,   166,   178,   -89,    65,  6970,
     178,   169,  -903,    29,    -5,   826,   169,  -903,  -903,  -903,
    -903, 10244,   166, 10244,   166, 10244,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,  -903,   -49,   118, 10244,
    -903,  -903,  -903,  -903,  -903,   178, 10244,  -903,  9022,   178,
      65,  7800,    65,    65,  -903,  -903,   115,   333,   538,   326,
    9022, 10244,   118, 10244, 10244,    -5, 10244, 10244, 10244, 10244,
     870,  -903,  -903,  -903,  -903,  -903,    65,  7571,  -903,    -5,
     963,   169,  -903,  -903,  -903,  -903,   118,   299,   178, 10244,
   10244, 10244,   300,   166,    65,   493,  -903,  -903, 10244,   400,
    -903,  -903, 10244,  -903, 10244, 10336,  -903,  6970,  -903,  -903,
    -903,     2,  8272,  -903,   178, 10244,   166,   172,   172,   178,
     172,  9022, 10244,    60, 10244,    65,  -903,  -903,  -903,  -903,
      65,    65,  -903,  7800,   178,  -903,  6970,  -242,  -903,  -903,
    -903, 10244, 10244,  -903,   335,   166, 10244,    -5, 10244,  1455,
     169,  -903,  -903,  -903,  -903, 10244,   178,  -903,  -903,  -903,
     230,  -903,  -903,   338,   166, 10244,  1521,  -903,  -903,  -903,
    -903, 10244,   178,  -903,  -903,  -903,  -903,  -178,  -903, 10244,
   10244, 10244, 10244,  1630, 10244, 10244, 10244, 10244,   115,  -903,
   10244, 10244, 10244, 10244,  1860,   -30,   118,   171,    65,   118,
     171,  6970,   118, 10244, 10244, 10244,   234,   219,   236,    51,
     244,   236,   244,   701,  3872,  -903,  3400,  -903, 10244,  7800,
    7800,  7800,  7800,  7800,  7800,  7800,  7800,  7800,  7800,  7800,
    7800,  7800,  7800,   476, 10244,   240,  -903,  7800,  7800,  7800,
    7800,  7800,  7800,  7800,  7800,  7800,  7800,  7800,  7800,  7800,
    7800,  7800,  7800,   347,   519,   174,   118,   118,   118,  1378,
     556,   142,  -903,   189,  -130,   166, 10244,   118,   432,  6695,
     118,   166, 10244,    95,   258, 10244, 10244,  5314,  4257, 10244,
     262,  -903,  -903,  -903,  -903,   423,   835, 10244,  4173,  4405,
     292,  -903,  -903,    65,    65,  7800, 10244,  -903,  4255,   178,
     264,   266,  6970,  -903,  3983,   283,  4361,  8603,  8815,   286,
     287,  1891,  1914,   289,  1935,  1959,  2006,  2031,  -903,  9022,
     178,   113,    65,   291,  -903,  -903,  -903,  -903,  -903, 10244,
     178,  -903,   178, 10244, 10244, 10244, 10244, 10244, 10244, 10244,
   10244, 10244,  9212,  -903, 10244,  -903,  -903,  -903,  -903,  6970,
    -903,  -903,  -903,   234,   234,  8935, 10244, 10244, 10244, 10244,
   10244, 10244, 10244, 10244, 10244, 10244, 10244, 10244, 10244, 10244,
   10244,  -903,    65, 10244, 10244,   178,  -903,  -903,   178, 10244,
    3596,  -152, 10244,    62,   166,   391,   298,  -903,  -903,  -903,
    -903, 10244,   284,   178,  6970,   178,   471,  -903,  -903,  -903,
    -903,   301,  6970,   178,   306,  3983,  2080,   302,   309,  -903,
     -54,  -903,  -903,  -903,  -903,  -903, 10244,  -903,  -903,  6970,
    -903,  -903,   166,  -903,  6970,  6970, 10244,   329, 10244, 10244,
   10244, 10244,  -903,  -903, 10244,  -903,   187,   402,   405,  2103,
     336,    65,   178,   330,   118,  8232,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,   342,  8835,   360,   343,  -903,   178, 10244,
   10244,   345,   178,   178,   166,   178,   166,   178,   166,  6970,
     178,  -903,  2181,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,  2264,   349,   346,   351,  9251,  -903,
    -903,    14,  -903,  2288, 10244,   178,  -903,  -903,  -903, 10244,
    6970,    65, 10244,   178,  -903, 10244,  6970,  1512,  6970,  6970,
    6970,  6970, 10244,  -903,  -903,  -903,  -903,  6970,  6970,  6970,
    6970, 10244,   166,   166,  -903,  -903,  -903,  -903,  -903,  -903,
     354,  -903,  -903,  -903,  6970,  6970,  6970,  -903,  -903,   424,
    -357,     5,   -65,   -10,    16,  -903,  -128, 11045, 11044,  3788,
      73,   475,   331,   765,  1015,  1348,  1015,  1348,  1015,  1348,
    1015,  1348,  1015,  1348,  1015,  1348,   243,   267,   243,   267,
     236,   244,   236,   244,   236,   244,   236,   244,  -903,  -147,
   10244,  4595,  4935,    73,   475,   331,   765,  1015,  1348,  1015,
    1348,  1015,  1348,  1015,  1348,  1015,  1348,  1015,  1348,   243,
     267,   243,   267,   236,   244,   236,   244,   236,   244,   236,
     244,  3983,  5361,   -21, 10244, 11197, 10244, 10244, 10244,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -128,
    -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
     166,   478, 11197, 10244, 10244,   166, 10244, 10244,  -903,  -903,
    -903, 10244,  -903,   178,   357,  -903, 10244,   109,   222, 10244,
      19,  -903,  -903,  -903,  -903,  -903,  -903, 10244, 10244,   201,
    -903,  -903,  -903,  -903,   178,  -903, 10244,  -903,  -903,  -903,
    -903,  -903,   166,  2382,  -903,   118,  9494,  8418, 10244, 10244,
    -903,  -903,  -903,  -903,  -903,  -903,  2405,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,  -903,   484,   -22,   188,    19,  7010,
     178,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  2431, 10244,
     166,   487,  3916,   166,  -111,  8647,   717, 10244,   166,    13,
   10244, 10244,  9640,  9869,    -3,   363,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,   118,  -903, 10244,  -903,   118,  6504,
     392,   398,  6313, 10244, 10244,  -903,   146, 10244, 10015,     8,
     367,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,   118,
     378,   379,  9312,  -903, 10244,  -903,  -903,  -903,  -903,  -903,
   10244,  7800,  7800,  7800, 10244, 10244, 10244, 10244, 10244,  6970,
    -903,  -903, 10244,  8935,   381,  9332,   382,  2473,   383,  2530,
    2569,  2590,  2634,  1071,  1601,  1983,  1983,  1983,  1983,  1983,
    1983,   305,   305,   234,   234,   234,   234,  3418,  -903,  6970,
    6970,  6970,  -903,   387,  -903,  -903,   166,  6970,   118,  -903,
     178, 10244, 10244,  3441,  -903,  -903,  -903,  7800,  7800,  -903,
    7800,   201,  -903,  -903,  -903,  6970,   178,  6970,    65,  3464,
    6970,  6970,  -903,  3492,  -903,   393,   166, 10244,  -903,  -903,
    7800,  -903, 10244, 10244, 10244,  -903, 10244,  6970,  6970,  -903,
     178,   178,   178, 10244, 10244,  -903, 10244,  7892,  -903,  -903,
    -903,  -903, 10244,  6970,  6970,  -903,  6970,  6970,  -903,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
     664,  3653,  3694,  -105,   178,    65,   240,   558, 10244, 10244,
    6970,  7800,  7800,  -903,  -903,  6970, 11197,  3741,  -903,  4128,
    -903,   178, 10244,  -903,  4446,  -903,   178,  -903,  -903,  -903,
   10244,  6970,  -903,  -903,  -903,  -903,   208,  -903,  -903,  -903,
    6970,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,   178, 10244,  -903,  -903,  -903,  -903,  -903,
    -903,  6970,  -903, 10244, 10244,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,  4473, 10244,  -903,   178, 10244,  6970,
    -903,   178,  -903,  -903,  -903,  -903,  -903,  4551,   496,   499,
    -903,   178,  -903,  -903,  -903,  -903,  -903,  -903,    17,  6970,
      27,  6970,  -903,  -903,   166,  -903,  -903,  -903,   118, 10244,
    4659,   401,  -903,  -903,  -903,  -903,  -903,  -903,   118, 10244,
   10244,  4659,   404,  -903,  -903,  -903,  -903,  6970,  6970,   118,
    6970,   -38,  -903,  -903,  -903, 10244,  -903, 10244,  8793,  -903,
     406,   403,  9364,  9553,  9595,  9702,  9734,  9825,   407,  4680,
    4705,  5144,  5165,   411, 10244, 10244, 10244, 10244, 10244, 10244,
   10244, 10244, 10244,  -903,   178,  -903,   412,   416, 10244,  5227,
    9928,  5269,  -903,   422, 10244, 10244, 10244,  -263,  5397,   425,
    5422,  -903,  -903,  5462,  6970,   426,   -67, 10037,  5605,   166,
   10244, 10244,   166,   429,  9169,   420,  9454, 10690,  9454, 10690,
    -903, 10244, 10244,  6970, 10244,  -903, 10244,  -903,  6970,  6970,
    6970, 10244,  6970,  6970, 10244,  -903,  -903,  -903,  -903,  -903,
    -903,   178,  -903,  6970, 10244,  -903,  6970,  -903, 10244,  -903,
    -903,  -903,  -903,  -903,  6970,   427,  -903,  -903,   431,   434,
   10244,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  7800,  7800,
   10244, 10244, 10244, 10244,   451, 10072, 10112,   452,  5653,  6161,
    6182,  6209,  6352, 10244,  -903,  6373,   118,  -903,    21,  -903,
    6970,  6400,   453,   166, 10244,  -903, 10244, 10244,  -903,   118,
    -903, 10244,   -29,  6590,  6722,   178, 10244,  -903,  6970,  6970,
    6970,  6970,  6970,  6970,  6970,  6970,  -903,  -903,    25,    89,
   10157, 10357, 10377, 10407,  6786,  6820,  6883,  6923, 10244, 10244,
   10244, 10244, 10244, 10244, 10244,  -903, 10244,   468,  -903,  -903,
    -903,   469, 10244, 10244,   178,   479,   480,  7124,   481,  7156,
     166, 10244, 10244,   474,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,   477,  -903, 10244,  -903,  -903,  -903,  -903,  7800, 10244,
   10244, 10244,   482, 10442, 10462,  7253,  7276,  7299,  6970,  6970,
     118,   118,  6970,   488,  -903,  -903, 10244,  -903, 10244,   178,
    6970,  6970,   118,    11,   492, 10482, 10504,  7590,  7715,  8009,
    -903, 10244, 10244, 10244,   495,   497, 10244,  6970,  3516,   500,
    -903,  -903,  -903,   498, 10244,  -903,  -903,  7800, 10244, 10244,
   10527,  8090,  8115,  -903,  -903,   507, 10244,  -903,    12,   508,
   10547, 10567,  8145,   455, 10244, 10244,   509,  8331,  -903,  -903,
    -903,   511, 10244,  -903,  -903,  7800,  -903,   118, 10587,  8465,
     166, 10244,  -903, 10607, 10627, 10647,   520, 10244,   178,  8494,
    -903,  -903,  -903,   118, 10667, 10244,   521,  8515,   118, 10244,
      97,  6970,  -903,   118,   523,  -903
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -903,  -903,  -903,   273,     0,  1397,  -173,  1548,   -48,  -903,
    -903,   583,  1526,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,   259,  -903,  -903,  -903,  -903,  -903,  2092,
     881,  -903,  -164,  -903,  -903,   872,  -903,   -69,  -903,  -903,
    -903,  -903,   415,   218,  -903,  -331,  -903,  -903,  -902,  -903,
    -903,  -245,  -586,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,   -53,   -11,    -9,   176,    28,  -116,  -903,  -903,
    -903,  -903,  -903,  -903,  -291,   -90,   -96
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -171
static const yytype_int16 yytable[] =
{
     154,   357,   699,   571,  1019,   610,  1550,   191,   615,   382,
     619,   620,   430,   623,   512,   692,   255,   282,   283,  1412,
     202,   202,   278,  1236,  1450,  1233,   202,   250,  1610,   298,
    1144,    17,    18,   693,  1302,   177,   351,   506,   352,   289,
     295,   563,   486,  1322,  1285,   179,   564,  1312,   445,   700,
     462,   383,   467,   393,  1353,   257,   478,   483,  1199,   509,
    1243,   281,   380,  1437,   390,   180,  1538,   448,   300,   241,
    1145,   191,   181,  1439,   242,   354,   259,   260,   241,   182,
     202,  1261,  1262,   242,   832,  1554,  1638,   191,   799,   396,
     385,   183,   395,   783,   833,   527,   774,   776,   446,   778,
     347,   767,  1148,   459,   652,   423,   197,   545,   178,   476,
     184,  1200,   565,   800,   432,   493,   238,   239,   784,   191,
     397,   261,   185,   504,   202,   202,   694,   817,   702,   695,
     731,   464,   629,   469,   202,   566,  1493,   480,   485,  1611,
    1300,   192,   511,  1286,   743,   507,   264,   516,   518,   521,
     523,   186,   399,   241,   241,   262,   567,   592,   242,   242,
    1555,  1556,    49,    50,   348,   263,  1451,   510,   274,  1303,
     202,  1252,   370,   274,   432,   971,   238,   239,   608,   612,
     187,   614,   617,   711,  1263,   621,   188,   624,   625,   626,
     627,  1539,   449,   302,   595,  1639,   594,    49,    50,   400,
    1304,   203,   241,   256,   785,   264,   786,   242,  1252,   280,
     266,   189,   807,   241,   190,  1313,   299,   629,   242,  1314,
     238,   239,   239,   353,   606,   787,   265,   196,   655,  1557,
     960,  1229,  1287,   653,  1612,   191,   191,  1640,   384,   659,
     394,   191,   202,   629,   198,   254,  1558,   663,   664,   665,
     433,   654,   288,  1002,  1156,   301,   961,   241,   297,  1234,
    1452,   358,   242,   311,   202,   350,   238,   239,  1161,   266,
     241,   274,   356,  1264,   448,   242,  1269,  1323,   568,   463,
     381,   468,   392,   434,   697,   479,   484,   360,  1354,   370,
     629,   361,   788,   789,  1256,   421,   364,  1438,   925,   569,
      96,    97,  1540,   704,   596,   303,   629,  1440,   403,   660,
     433,   370,   362,   962,   790,  1253,   363,   274,   661,    49,
      50,   461,  1265,   386,   465,  1254,   241,   477,   132,  1559,
     435,   242,  1499,   629,  1420,    96,    97,   513,   707,   239,
     267,   401,   435,   434,   593,   404,  1560,   268,   835,   836,
     241,   597,  1253,   527,   349,   242,   141,   722,   712,   723,
     448,   715,  1254,   199,   635,   422,   629,    49,    50,   304,
     791,   889,   792,   729,   284,  1426,   431,   837,   734,   735,
     425,   494,   241,   949,   950,   572,   591,   242,   602,   570,
     305,   200,   514,   238,   239,   607,   725,   241,   963,   449,
     996,   749,   242,  1349,   440,   653,   441,  1289,   755,  1017,
     628,   926,  1290,   656,   764,   634,   241,   838,   515,   543,
     272,   242,   659,  1270,   356,   495,   964,   965,   202,  1018,
     241,   779,   782,  1093,   794,   242,   443,   543,   598,   599,
     508,   481,   544,   715,  -170,   966,   543,   600,   525,    49,
      50,   544,   803,   530,   531,   532,   533,   534,   535,   536,
     537,   538,   539,   540,   541,   810,   241,   542,   526,   698,
     615,   242,  1116,   562,  1118,   273,   603,   527,   604,   927,
    1035,   816,  1061,  1086,   365,   449,   496,  1562,  1563,   818,
     819,   820,   821,   636,   967,  1672,  1673,   546,   202,   637,
     827,   828,   829,   830,   605,    96,    97,   202,  1063,  1088,
     631,   632,   928,   844,   845,   846,   178,   757,   643,  1015,
     969,   638,   644,   708,   238,   239,   645,   758,   859,   860,
     862,   864,   866,   868,   870,   872,   874,   876,   878,   880,
     882,   884,   886,  1032,   724,  1058,   646,   891,   893,   895,
     897,   899,   901,   903,   905,   907,   909,   911,   913,   915,
     917,   919,   921,   759,   570,   647,   709,   982,   590,   983,
     991,   682,   997,   629,   998,   274,  1011,   648,  1012,   649,
     650,   651,   683,   155,  1022,  1003,   721,    96,    97,  1016,
     728,   690,   129,   130,   238,   239,   689,  1038,   719,   976,
     726,   746,   751,   238,   239,   715,   985,   497,   754,   804,
     811,  1000,   812,   202,   745,  1014,   435,   134,   135,   136,
     137,   138,   139,  1024,  1034,   275,  1060,  1085,   679,  1109,
     542,   241,   539,   540,   541,   760,   242,   542,   561,  1113,
     241,  1019,   890,   888,  1115,   242,  1117,   276,  1119,  1120,
    1121,  1122,   923,   720,   274,   527,   558,   559,   560,   924,
     977,   561,  1089,  1094,  1095,  1151,  1123,  1124,  1125,  1126,
    1127,  1128,  1129,  1130,  1131,  1132,  1133,  1134,  1135,  1136,
    1137,  1096,  1154,  1139,  1140,  1100,  1101,   978,  1104,  1141,
    1112,  1155,  1147,   571,   676,   677,   678,  1152,  1271,   679,
    1159,  1153,  1156,   948,  1175,  1157,   979,  1176,  1160,   238,
     239,  1163,   531,   532,   533,   534,   535,   536,   537,   538,
     539,   540,   541,   527,   834,   542,  1165,   841,  1168,  1180,
     843,   519,   519,   519,  1178,  1185,  1167,   980,  1169,  1170,
    1171,  1183,  1186,  1189,  1173,  1196,   241,  1195,   177,   970,
    1197,   242,  1242,  1225,   984,   992,  1250,   307,  1284,   999,
     274,  1298,  1324,  1013,   519,   519,  1355,  1336,   519,  1187,
    1188,  1023,  1033,  1337,  1059,  1084,  1308,  1357,  1358,  1252,
    1374,  1375,  1377,   929,   930,   931,   932,   546,  1383,   602,
    1396,   968,   972,  1409,  1435,   975,   981,  1436,   993,   855,
    1444,  1461,  1001,  1448,  1203,  1460,  1468,   308,  1226,  1204,
    1473,  1483,  1206,  1021,  1484,  1207,  1057,  1082,    49,    50,
    1489,  1507,  1221,  1495,  1498,  1516,   949,   950,  1506,  1517,
     959,  1222,   528,  1518,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,   677,   678,   592,   527,   679,
    1528,  1529,  1543,  1646,  1647,   549,   550,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,  1580,  1581,   561,
    1392,   951,   952,  1592,   953,   954,  1593,  1584,  1585,  1587,
    1600,   168,   955,   956,   957,   958,   594,  1606,   129,   130,
    1230,  1614,   527,  1623,   241,  1624,  1164,  1628,  1627,   242,
     195,  1240,  1025,   201,   191,  1636,   285,  1642,  1650,  1652,
     202,  1149,   271,   134,   135,   136,   137,   138,   139,  1663,
    1668,  1675,     0,  1253,  1235,   842,  1237,  1361,  1239,     0,
    1309,  1368,  1291,  1254,     0,     0,     0,     0,  1162,     0,
       0,     0,   367,     0,   519,     0,     0,     0,     0,     0,
       0,     0,   402,  1277,     0,     0,   309,   370,     0,     0,
       0,     0,     0,  1244,     0,     0,     0,  1258,   442,     0,
       0,     0,  1181,     0,     0,     0,  1251,     0,     0,  1260,
     447,     0,   451,     0,     0,   527,     0,   470,   471,     0,
       0,     0,     0,     0,     0,  1026,     0,     0,  1279,  1027,
       0,     0,     0,   499,     0,   505,  1259,   239,  1281,     0,
     528,     0,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,   677,   678,     0,   519,   679,     0,  1294,
       0,     0,   680,     0,   593,     0,     0,   527,     0,     0,
       0,     0,  1299,     0,     0,  1307,     0,     0,     0,     0,
    1028,     0,  1319,  1321,     0,     0,  1415,     0,  1333,     0,
       0,  1344,     0,  1029,     0,     0,     0,     0,     0,  1330,
       0,     0,  1341,  1347,  1348,   572,   591,  1350,   528,     0,
     529,   530,   531,   532,   533,   534,   535,   536,   537,   538,
     539,   540,   541,   527,     0,   542,     0,  1335,     0,   847,
    1346,  1362,  1364,  1366,     0,  1369,  1370,  1371,  1372,   856,
       0,     0,   519,   519,   519,   519,   519,   519,   519,   519,
     519,   519,   519,   519,   519,   519,     0,     0,     0,     0,
     519,   519,   519,   519,   519,   519,   519,   519,   519,   519,
     519,   519,   519,   519,   519,   519,   550,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,   715,   715,   561,
     715,     0,     0,     0,     0,     0,     0,     0,     0,   696,
       0,     0,     0,     0,     0,     0,     0,  1398,     0,     0,
     614,     0,  1135,  1280,  1135,     0,     0,     0,   519,     0,
       0,     0,     0,  1403,  1404,     0,  1257,   715,     0,     0,
       0,     0,  1408,   528,  1477,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,   677,   678,     0,     0,
     679,     0,     0,  1275,     0,   701,     0,     0,  1414,     0,
       0,  1416,  1418,     0,     0,     0,     0,     0,     0,     0,
       0,   714,  1423,   717,   718,  1334,     0,   528,  1345,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
     677,   678,     0,     0,   679,     0,     0,   737,     0,   736,
       0,     0,     0,     0,  1428,     0,     0,     0,     0,     0,
       0,     0,  1325,  1429,  1430,   753,  1327,  1332,     0,     0,
    1343,     0,     0,     0,     0,  1432,     0,     0,  1433,     0,
       0,     0,     0,     0,     0,     0,     0,  1356,     0,     0,
       0,     0,     0,     0,     0,     0,   795,     0,     0,     0,
       0,   796,   797,     0,     0,     0,     0,     0,     0,  1443,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1446,
     528,     0,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,   677,   678,  1454,     0,   679,     0,     0,
       0,     0,   744,     0,     0,     0,  1385,     0,     0,     0,
     546,     0,     0,     0,     0,  1475,  1476,     0,  1478,  1479,
    1480,  1481,  1482,     0,     0,     0,     0,     0,  1485,   840,
       0,     0,     0,     0,  1490,  1491,  -171,  -171,  -171,  -171,
    -171,  -171,   537,   538,   539,   540,   541,   858,     0,   542,
    1503,  1504,     0,   193,     0,     0,     0,     0,     0,     0,
       0,  1508,  1509,     0,  1510,   258,  1511,   270,     0,     0,
     933,  1512,   286,   287,  1513,     0,   290,     0,     0,     0,
       0,     0,     0,     0,  1514,   355,   934,     0,  1515,   359,
       0,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,   677,   678,   398,     0,   679,     0,     0,  1520,  1522,
    1524,  1525,  1526,  1527,  1427,     0,   424,   527,   429,     0,
       0,     0,     0,     0,  1090,  1091,     0,     0,     0,     0,
       0,   444,     0,     0,     0,   935,     0,  1547,     0,     0,
       0,  1549,   466,     0,     0,     0,     0,   482,     0,     0,
     487,     0,     0,  1111,     0,     0,   936,  1208,  1209,  1210,
    1211,  1212,  1213,  1214,  1215,  1216,  1217,  1218,  1219,  1573,
    1574,  1575,  1576,  1577,  1578,     0,  1579,     0,     0,     0,
       0,     0,  1582,   527,     0,     0,  1442,     0,     0,     0,
       0,  1590,  1591,     0,     0,     0,  1445,   937,   938,   939,
       0,     0,     0,  1138,     0,     0,     0,  1449,  1595,  1597,
    1598,  1599,     0,     0,     0,   609,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1607,     0,  1608,     0,
     630,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1620,  1621,  1622,   940,     0,     0,     0,   941,   942,
       0,     0,     0,     0,     0,     0,     0,  1630,  1632,  1633,
     642,     0,   419,   527,     0,     0,  1637,     0,     0,     0,
       0,     0,  1179,     0,  1648,  1649,     0,     0,     0,     0,
       0,     0,  1653,     0,     0,  1654,     0,     0,     0,     0,
     453,  1659,   527,     0,     0,   943,   472,  1664,     0,     0,
       0,     0,   488,   944,   945,  1667,     0,     0,     0,  1671,
     685,     0,   688,     0,   520,   522,   524,     0,     0,     0,
       0,     0,   691,     0,   519,   519,   519,     0,     0,     0,
       0,     0,  1205,     0,     0,     0,     0,     0,     0,   703,
       0,   705,     0,     0,  1537,     0,     0,   616,   618,     0,
       0,   622,     0,     0,     0,     0,   946,  1548,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   613,  -171,
    -171,  -171,  -171,  -171,  -171,   556,   557,   558,   559,   560,
     519,   519,   561,   519,   633,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   713,     0,     0,     0,     0,
       0,     0,     0,   519,     0,     0,     0,   685,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     519,     0,     0,     0,     0,     0,   658,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1604,  1605,
     752,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1609,     0,     0,     0,   519,   519,     0,     0,   768,     0,
       0,   686,   687,   772,   773,   775,     0,   777,   780,     0,
       0,     0,   528,     0,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,   677,   678,     0,     0,   679,
       0,     0,   805,   706,   809,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1656,     0,     0,     0,     0,
       0,   813,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1666,   527,     0,     0,     0,  1670,   716,     0,     0,
       0,  1674,     0,     0,     0,     0,     0,   710,   528,     0,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,   677,   678,   527,     0,   679,     0,     0,   686,   727,
     815,     0,   730,     0,   732,   733,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   742,   546,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   747,   748,   750,
       0,     0,     0,     0,     0,     0,   756,   527,     0,     0,
     763,     0,     0,   766,     0,     0,     0,     0,     0,   798,
       0,     0,   973,   771,     0,     0,     0,     0,   994,     0,
       0,   527,   668,   669,   670,   671,   672,   673,   674,   675,
     676,   677,   678,  1030,     0,   679,     0,     0,     0,   802,
       0,     0,     0,     0,   806,   527,   808,   528,     0,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
     677,   678,     0,   814,   679,     0,  1110,     0,   527,   822,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   823,   824,   825,   826,     0,     0,     0,  1393,
       0,   519,   519,   527,     0,   861,   863,   865,   867,   869,
     871,   873,   875,   877,   879,   881,   883,   885,   887,     0,
       0,     0,     0,   892,   894,   896,   898,   900,   902,   904,
     906,   908,   910,   912,   914,   916,   918,   920,   922,     0,
       0,  1150,   167,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   546,     0,     0,     0,  1413,     0,     0,     0,
       0,   194,     0,     0,   194,     0,     0,     0,     0,   252,
       0,     0,     0,   194,   974,   527,     0,   990,     0,  1166,
     995,  1092,   296,     0,  1004,  1010,     0,   310,     0,     0,
       0,     0,     0,     0,     0,     0,  1055,  1080,     0,     0,
       0,   519,     0,   366,     0,     0,   391,     0,     0,     0,
       0,     0,     0,   194,     0,     0,     0,     0,     0,     0,
       0,  1190,     0,  1191,     0,  1192,     0,     0,     0,   194,
       0,     0,     0,     0,     0,     0,     0,  1114,     0,     0,
       0,   194,     0,   194,     0,   460,     0,     0,   194,   194,
     519,     0,   687,   527,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   498,     0,   194,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   519,  1223,
    1224,     0,     0,     0,     0,     0,     0,   528,  1143,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
     677,   678,     0,     0,   679,     0,     0,     0,     0,   831,
       0,     0,     0,     0,     0,     0,     0,     0,   528,     0,
     529,   530,   531,   532,   533,   534,   535,   536,   537,   538,
     539,   540,   541,     0,     0,   542,   527,     0,     0,  1172,
    1102,   547,     0,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,   559,   560,     0,     0,   561,     0,
     527,     0,   528,  1103,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,   677,   678,     0,     0,   679,
       0,     0,     0,     0,  1105,     0,   528,     0,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,   677,
     678,     0,     0,   679,     0,     0,     0,  1241,  1106,     0,
       0,     0,  1246,     0,  -171,  -171,  -171,  -171,  -171,  -171,
     674,   675,   676,   677,   678,     0,     0,   679,     0,     0,
     194,     0,     0,   528,     0,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,   677,   678,     0,  1273,
     679,     0,     0,     0,   527,  1107,     0,     0,   528,     0,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,   677,   678,     0,     0,   679,     0,   527,     0,     0,
    1108,     0,     0,     0,     0,     0,     0,  1297,     0,     0,
    1301,     0,     0,     0,     0,  1311,     0,     0,     0,     0,
       0,     0,   194,   527,   194,   194,     0,   547,     0,   548,
     549,   550,   551,   552,   553,   554,   555,   556,   557,   558,
     559,   560,     0,     0,   561,  1238,     0,     0,   194,  1158,
     528,     0,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,   677,   678,   527,   194,   679,     0,     0,
       0,     0,  1177,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1245,     0,  1247,  1248,     0,     0,     0,  1249,
       0,     0,     0,     0,     0,     0,     0,   194,     0,     0,
       0,     0,   194,   194,     0,  1267,  1268,     0,     0,     0,
       0,     0,     0,  1384,  1272,     0,     0,     0,     0,     0,
       0,     0,   527,     0,  1276,  1278,     0,  1282,   528,     0,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,   677,   678,  1397,     0,   679,     0,     0,     0,     0,
    1193,     0,     0,     0,     0,     0,     0,  1296,     0,     0,
       0,   527,     0,     0,     0,  1310,     0,     0,  1316,  1317,
     194,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   527,     0,  1326,     0,     0,  1331,   857,     0,
    1342,     0,     0,     0,     0,     0,  1352,  1363,  1365,  1367,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   528,  1360,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,   677,   678,   527,     0,   679,     0,
    1373,     0,     0,  1194,     0,   528,     0,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,   677,   678,
       0,     0,   679,  1389,  1390,     0,  1391,  1202,     0,     0,
       0,     0,     0,     0,     0,   194,   194,     0,     0,  1386,
    1387,     0,     0,     0,     0,     0,  1400,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1441,     0,  1407,   194,     0,     0,     0,  1399,     0,
       0,  1401,     0,     0,  1402,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1405,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1417,  1419,   528,
       0,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,   677,   678,   194,     0,   679,     0,     0,     0,
       0,  1274,   528,     0,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,   677,   678,     0,  1425,   679,
       0,     0,     0,     0,  1283,     0,  1502,     0,   528,  1505,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,   677,   678,     0,     0,   679,     0,     0,     0,     0,
    1295,     0,     0,     0,     0,     0,     0,     0,  1174,     0,
       0,     0,     0,   194,     0,     0,     0,     0,     0,     0,
     528,     0,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,   677,   678,     0,     0,   679,     0,     0,
       0,     0,  1376,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1447,     0,
    1544,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   194,     0,  1455,  1458,   528,     0,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
     677,   678,  1474,     0,   679,     0,     0,     0,     0,  1378,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1492,     0,   528,  1589,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,   677,
     678,     0,     0,   679,     0,     0,     0,   528,  1379,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
     677,   678,     0,     0,   679,     0,     0,     0,     0,  1380,
       0,     0,     0,     0,  1521,  1523,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1519,     0,
       0,   528,     0,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,   677,   678,     0,     0,   679,     0,
       0,  1535,     0,  1381,     0,     0,     0,    -7,     1,     0,
       0,     0,  1545,     0,  1546,     0,     0,  1658,     0,     0,
       0,     0,     0,     0,  1553,     2,     3,     4,     5,     0,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
       0,     0,    16,     0,    17,    18,  1572,     0,    19,    20,
       0,     0,     0,    21,    22,    23,     0,    24,    25,     0,
       0,  1583,     0,     0,  1596,     0,     0,     0,    26,     0,
       0,    27,    28,     0,     0,     0,     0,    29,     0,     0,
      30,  1594,     0,     0,     0,     0,     0,    31,    32,     0,
       0,     0,     0,     0,     0,     0,    33,    34,     0,     0,
       0,     0,     0,     0,     0,    35,    36,    37,    38,     0,
       0,     0,     0,  1631,    39,    40,     0,     0,    41,     0,
      42,     0,     0,    43,  1625,     0,    44,    45,     0,     0,
       0,    46,  1629,    47,     0,     0,     0,     0,    48,    49,
      50,  1655,     0,     0,    51,     0,    52,     0,     0,    53,
       0,     0,     0,     0,     0,     0,    54,    55,     0,     0,
       0,    56,    57,    58,    59,     0,    60,     0,    61,     0,
       0,     0,     0,    62,     0,    63,    64,     0,     0,     0,
      65,    66,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    71,     0,
      72,     0,    73,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    74,     0,     0,     0,     0,
     194,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,     0,     0,     0,     0,    76,    77,    78,    79,
      80,    81,     0,     0,    82,    83,    84,    85,     0,    86,
      87,     0,     0,     0,     0,    88,     0,    89,    90,    91,
      92,    93,    94,     0,     0,     0,    95,    96,    97,     0,
       0,     0,     0,    98,     0,    99,   100,   194,     0,   101,
     102,     0,     0,     0,     0,     0,     0,   103,   104,   105,
     106,     0,   107,     0,     0,     0,   108,   109,   110,     0,
       0,     0,   111,     0,   112,   113,   114,     0,     0,     0,
       0,   115,   116,   117,   118,   119,   120,   121,     0,   122,
     123,     0,   124,   125,   126,   127,   128,   129,   130,     0,
     131,   132,   133,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   134,   135,   136,   137,   138,   139,   140,   141,
      -7,     1,     0,     0,     0,     0,     0,     0,   142,   143,
     144,   145,   146,     0,     0,     0,     0,     0,   849,   850,
       4,     0,     0,     0,   147,   148,     0,     0,     0,   149,
       0,     0,     0,    -7,   150,    16,     0,    17,    18,     0,
     527,    19,    20,     0,     0,     0,    21,    22,    23,     0,
       0,    25,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    26,     0,   527,    27,    28,     0,     0,     0,     0,
      29,     0,     0,    30,     0,     0,     0,     0,     0,     0,
      31,    32,     0,     0,     0,     0,   527,     0,     0,    33,
      34,     0,     0,     0,     0,     0,     0,     0,    35,    36,
      37,    38,     0,     0,     0,     0,     0,    39,    40,     0,
       0,    41,     0,    42,   527,     0,    43,     0,     0,    44,
      45,     0,     0,     0,    46,     0,    47,     0,     0,     0,
       0,    48,    49,    50,     0,     0,     0,    51,   527,    52,
       0,     0,    53,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,    59,     0,    60,
       0,    61,     0,     0,     0,     0,    62,     0,    63,    64,
       0,     0,     0,    65,    66,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    67,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    71,     0,    72,     0,    73,     0,     0,     0,     0,
       0,     0,     0,     0,   224,   225,   226,     5,    74,   227,
     228,     8,   229,   230,   231,   232,   233,   234,   235,     0,
       0,     0,     0,     0,    75,     0,     0,     0,     0,    76,
      77,    78,    79,     0,    81,     0,    24,    82,    83,   851,
      85,     0,    86,    87,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,    94,     0,     0,     0,     0,
      96,    97,     0,     0,     0,   527,    98,     0,    99,   100,
       0,     0,   101,   102,     0,     0,     0,     0,     0,     0,
     103,     0,   105,   106,     0,   107,     0,     0,     0,   108,
     109,   110,     0,     0,     0,   111,     0,   112,   113,   114,
       0,     0,     0,     0,   115,     0,   527,     0,     0,     0,
       0,     0,   122,   123,     0,   124,     0,     0,    49,    50,
     129,   130,     0,   131,   132,   133,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,    55,     0,     0,     0,
      56,     0,     0,     0,     0,   134,   135,   136,   137,   138,
     139,   140,   141,   527,     0,     0,     0,     0,     0,     0,
       0,   142,   852,   853,   854,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   528,    -7,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,   677,   678,
     527,     0,   679,  1626,     0,     0,     0,  1382,   528,     0,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,   677,   678,     0,     0,   679,     0,     0,     0,   243,
    1388,   528,     0,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,   677,   678,     0,     0,   679,     0,
       0,     0,     0,  1394,     0,    95,    96,    97,     0,   528,
       0,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,   677,   678,     0,     0,   679,   104,     0,     0,
       0,  1395,     0,   528,   546,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,   677,   678,     0,     0,
     679,   116,   117,   118,   119,   120,   121,     0,     0,     0,
       0,     0,   125,   126,   127,   128,   129,   130,     0,     0,
       0,     0,     0,     0,   224,   225,   226,     5,     0,   227,
     228,     8,   229,   230,   231,   232,   233,   234,   235,     0,
       0,   134,   135,   136,   137,   138,   139,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    24,     0,   244,   245,
     246,   146,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   147,   247,     0,     0,     0,   248,     0,
       0,     0,     0,   249,   312,     0,     0,  1142,   313,   314,
     315,   316,   317,   318,     0,   527,     0,   319,   320,   321,
     322,   323,   324,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   325,     0,     0,   326,     0,
     528,     0,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,   677,   678,     0,   327,   679,    49,    50,
       0,     0,  1410,     0,     0,     0,     0,     0,     0,   328,
       0,     0,     0,     0,     0,    54,    55,     0,     0,     0,
      56,   528,     0,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,   677,   678,     0,     0,   679,     0,
       0,     0,     0,  1411,     0,     0,     0,     0,     0,     0,
       0,     0,    68,     0,    70,     0,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,     0,   528,     0,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,   677,   678,     0,     0,   679,     0,     0,     0,     0,
    1421,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     527,     0,   340,     0,     0,     0,     0,     0,     0,   243,
       0,     0,     0,     0,     0,   528,  1228,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,   677,   678,
       0,     0,   679,   341,     0,    95,    96,    97,     0,     0,
       0,   224,   225,   226,     5,     0,   227,   228,     8,   229,
     230,   231,   232,   233,   234,   235,     0,   104,     0,     0,
       0,     0,  1039,     0,  1040,     0,     0,     0,     0,     0,
       0,     0,     0,    24,     0,     0,     0,     0,     0,     0,
       0,   116,   117,   118,   119,   120,   121,     0,     0,     0,
    1041,     0,   125,   126,   127,   128,   129,   130,   202,   547,
       0,   548,   549,   550,   551,   552,   553,   554,   555,   556,
     557,   558,   559,   560,     0,     0,   561,     0,     0,     0,
     848,   134,   135,   136,   137,   138,   139,   527,     0,     0,
       0,     0,   342,   343,   344,   345,     0,     0,   244,   245,
     246,   146,     0,     0,     0,   370,     0,  1042,  1043,     0,
       0,     0,     0,   147,   247,    49,    50,     0,   248,   204,
     205,   206,     0,   249,     0,     0,     0,     0,     0,     0,
       0,     0,    54,    55,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1044,     0,     0,
       0,     0,     0,     0,     0,   239,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
     528,    70,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   538,   539,   540,   541,     0,     0,   542,     0,     0,
       0,   241,  1045,   546,     0,     0,   242,     0,  1028,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1046,     0,     0,     0,     0,     0,     0,     0,     0,
    1047,  1048,     0,     0,     0,     0,   243,     0,     0,     0,
       0,     0,     0,   224,   225,   226,     5,     0,   227,   228,
       8,   229,   230,   231,   232,   233,   234,   235,     0,     0,
       0,  1064,    95,    96,    97,     0,  1049,     0,  1050,     0,
       0,     0,     0,     0,     0,    24,     0,     0,     0,     0,
    1051,  1052,  1053,     0,   104,     0,     0,  1065,   527,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1054,     0,
     202,     0,     0,     0,     0,     0,     0,     0,   116,   117,
     118,   119,   120,   121,  1066,   527,     0,     0,     0,   125,
     126,   127,   128,   129,   130,   528,     0,   666,   667,   668,
     669,   670,   671,   672,   673,   674,   675,   676,   677,   678,
       0,     0,   679,     0,     0,     0,     0,  1422,   134,   135,
     136,   137,   138,   139,     0,     0,     0,    49,    50,     0,
       0,     0,     0,     0,     0,   244,   245,   246,   146,     0,
       0,     0,     0,     0,    54,    55,  1067,     0,     0,    56,
     147,   247,     0,     0,     0,   248,     0,     0,     0,     0,
     249,     0,     0,   527,     0,     0,     0,   239,     0,     0,
       0,  1068,     0,     0,     0,     0,     0,     0,     0,  1069,
       0,  1070,     0,  1071,  1072,   207,     0,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,     0,   241,     0,  1073,  1074,   527,   242,     0,
    1028,     0,  1075,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   528,  1076,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,   677,   678,     0,   243,   679,
       0,     0,     0,   -90,   224,   225,   226,     5,     0,   227,
     228,     8,   229,   230,   231,   232,   233,   234,   235,     0,
       0,     0,     0,     0,    95,    96,    97,     0,     0,  1077,
       0,   527,     0,     0,     0,  1078,    24,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   104,     0,     0,     0,
     368,     0,   527,   369,     0,     0,     0,     0,     0,     0,
    1079,   202,     0,     0,     0,     0,     0,     0,     0,     0,
     116,   117,   118,   119,   120,   121,     0,   527,     0,     0,
       0,   125,   126,   127,   128,   129,   130,     0,   547,     0,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,     0,     0,   561,     0,     0,   370,  1097,
     134,   135,   136,   137,   138,   139,     0,     0,    49,    50,
       0,     0,     0,     0,     0,     0,   371,   244,   245,   246,
     146,     0,     0,     0,     0,    54,    55,     0,     0,     0,
      56,     0,   147,   247,   372,     0,     0,   248,     0,     0,
     373,     0,   249,   374,     0,     0,     0,   238,   239,     0,
       0,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    68,   528,    70,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,   677,   678,     0,     0,
     679,     0,     0,     0,   241,  1424,     0,     0,     0,   242,
     528,     0,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,   677,   678,     0,     0,   679,     0,     0,
       0,     0,  1431,     0,     0,     0,     0,     0,     0,   243,
       0,     0,     0,     0,   224,   225,   226,     5,     0,   227,
     228,     8,   229,   230,   231,   232,   233,   234,   235,     0,
       0,     0,     0,     0,     0,    95,    96,    97,     0,     0,
       0,     0,     0,     0,     0,     0,    24,   405,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   104,   528,     0,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,   677,   678,     0,     0,   679,     0,     0,     0,   377,
    1434,   116,   117,   118,   119,   120,   121,   546,     0,     0,
       0,     0,   125,   126,   127,   128,   129,   130,   378,   406,
       0,     0,   528,  1231,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,   539,   540,   541,     0,     0,   542,
       0,   134,   135,   379,   137,   138,   139,     0,    49,    50,
       0,     0,     0,     0,     0,     0,     0,     0,   244,   245,
     246,   146,     0,     0,     0,    54,    55,   407,     0,   408,
      56,     0,     0,   147,   247,     0,     0,     0,   248,     0,
     409,   410,     0,   249,     0,     0,   528,     0,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,   677,
     678,     0,    68,   679,    70,     0,     0,   528,   -87,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
     677,   678,     0,     0,   679,     0,     0,     0,     0,  1469,
       0,     0,   528,   411,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,   677,   678,     0,     0,   679,
       0,     0,     0,     0,  1470,     0,     0,     0,     0,   243,
     204,   205,   206,     0,   426,     0,     0,   412,     0,   427,
     413,     0,     0,     0,     0,     0,     0,   191,     0,     0,
       0,     0,     0,     0,   414,    95,    96,    97,     0,     0,
     415,   224,   225,   226,     5,     0,   227,   228,     8,   229,
     230,   231,   232,   233,   234,   235,     0,   104,     0,     0,
       0,     0,     0,     0,     0,     0,   527,     0,     0,     0,
       0,     0,   416,    24,     0,     0,     0,     0,     0,     0,
       0,   116,   117,   118,   119,   120,   121,   527,     0,     0,
     454,     0,   125,   126,   127,   128,   129,   130,   202,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   455,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   134,   135,   136,   137,   138,   139,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   244,   245,
     246,   146,     0,     0,     0,   370,     0,     0,     0,   546,
       0,     0,     0,   147,   247,    49,    50,     0,   248,     0,
       0,     0,     0,   249,     0,   428,     0,     0,     0,     0,
       0,     0,    54,    55,     0,     0,     0,    56,     0,     0,
       0,     0,     0,   456,     0,     0,     0,     0,     0,     0,
       0,   546,     0,     0,     0,   239,     0,     0,   457,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    68,
       0,    70,   547,  1232,   548,   549,   550,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,     0,     0,   561,
       0,   241,   224,   225,   226,     5,   242,   227,   228,     8,
     229,   230,   231,   232,   233,   234,   235,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,     0,   243,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   458,
    1005,  1006,     0,   546,     0,     0,     0,     0,     0,   202,
       0,     0,    95,    96,    97,     0,   207,     0,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,     0,   104,     0,     0,     0,     0,   527,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1007,
       0,     0,     0,     0,     0,     0,     0,     0,   116,   117,
     118,   119,   120,   121,   546,     0,    49,    50,     0,   125,
     126,   127,   128,   129,   130,     0,     0,     0,     0,     0,
       0,     0,     0,    54,    55,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   134,   135,
     136,   137,   138,   139,   527,   238,   239,     0,     0,     0,
       0,     0,     0,     0,     0,   244,   245,   246,   146,     0,
      68,     0,    70,     0,     0,     0,     0,     0,     0,     0,
     147,   247,     0,     0,     0,   248,     0,     0,     0,     0,
     249,   528,     0,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,   677,   678,   274,     0,   679,     0,
       0,     0,   528,  1471,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,   677,   678,   243,     0,   679,
       0,     0,     0,     0,  1472,     0,     0,   224,   225,   226,
       5,     0,   227,   228,     8,   229,   230,   231,   232,   233,
     234,   235,     0,    95,    96,    97,     0,     0,  1008,  1009,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    24,
       0,     0,     0,     0,   547,   104,   548,   549,   550,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,     0,
       0,   561,     0,     0,   202,     0,  1486,   527,     0,   116,
     117,   118,   119,   120,   121,     0,     0,   236,     0,     0,
     125,   126,   127,   128,   129,   130,   547,     0,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,     0,     0,   561,   237,     0,     0,     0,  1488,   134,
     135,   136,   137,   138,   139,   527,     0,     0,     0,     0,
       0,    49,    50,     0,     0,     0,   244,   245,   246,   146,
       0,     0,     0,     0,     0,     0,     0,     0,    54,    55,
       0,   147,   247,    56,     0,     0,   248,     0,     0,     0,
       0,   249,     0,     0,     0,     0,     0,     0,     0,     0,
     238,   239,     0,     0,   240,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    70,   547,     0,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,     0,     0,   561,     0,   241,   224,   225,
     226,     5,   242,   227,   228,     8,   229,   230,   231,   232,
     233,   234,   235,     0,   528,     0,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,   677,   678,     0,
      24,   679,   243,     0,     0,     0,  1494,     0,     0,   547,
       0,   548,   549,   550,   551,   552,   553,   554,   555,   556,
     557,   558,   559,   560,     0,   202,   561,     0,    95,    96,
      97,  1496,     0,     0,     0,     0,     0,     0,   292,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   528,
     104,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,   677,   678,     0,   293,   679,     0,     0,     0,
       0,  1497,     0,     0,   116,   117,   118,   119,   120,   121,
       0,     0,    49,    50,     0,   125,   126,   127,   128,   129,
     130,     0,     0,     0,     0,     0,     0,     0,     0,    54,
      55,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   134,   135,   136,   137,   138,   139,
       0,   238,   239,     0,     0,   294,     0,     0,     0,     0,
       0,   244,   245,   246,   146,     0,    68,     0,    70,     0,
       0,     0,     0,     0,     0,     0,   147,   247,     0,     0,
       0,   248,     0,     0,     0,     0,   249,     0,   241,   224,
     225,   226,     5,   242,   227,   228,     8,   229,   230,   231,
     232,   233,   234,   235,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   387,     0,     0,     0,     0,     0,
       0,    24,   528,   243,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,   677,   678,     0,     0,   679,
       0,     0,     0,     0,  1501,     0,   202,     0,     0,    95,
      96,    97,     0,     0,     0,     0,     0,     0,     0,   388,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     528,   104,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,   677,   678,     0,     0,   679,     0,     0,
       0,     0,  1530,     0,     0,   116,   117,   118,   119,   120,
     121,     0,     0,    49,    50,     0,   125,   126,   127,   128,
     129,   130,     0,     0,     0,     0,     0,     0,     0,     0,
      54,    55,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   134,   135,   136,   137,   138,
     139,     0,   238,   239,     0,     0,   389,     0,     0,     0,
       0,     0,   244,   245,   246,   146,     0,    68,     0,    70,
       0,     0,     0,     0,     0,     0,     0,   147,   247,     0,
       0,     0,   248,     0,     0,     0,     0,   249,     0,   241,
     224,   225,   226,     5,   242,   227,   228,     8,   229,   230,
     231,   232,   233,   234,   235,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    24,     0,   243,     0,     0,     0,     0,     0,
       0,     0,     0,   527,     0,     0,     0,     0,     0,   473,
       0,     0,     0,     0,     0,     0,     0,   202,     0,     0,
      95,    96,    97,     0,   527,     0,     0,     0,     0,     0,
     474,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   104,     0,     0,     0,     0,     0,     0,     0,
       0,   527,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   370,     0,   116,   117,   118,   119,
     120,   121,     0,     0,    49,    50,     0,   125,   126,   127,
     128,   129,   130,     0,     0,     0,     0,     0,     0,     0,
       0,    54,    55,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   134,   135,   136,   137,
     138,   139,     0,     0,   239,     0,     0,     0,     0,     0,
       0,     0,     0,   244,   245,   246,   146,     0,    68,     0,
      70,     0,     0,     0,     0,     0,     0,     0,   147,   247,
       0,     0,     0,   248,     0,     0,     0,     0,   249,     0,
     241,   224,   225,   226,     5,   242,   227,   228,     8,   229,
     230,   231,   232,   233,   234,   235,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    24,     0,   243,     0,     0,     0,     0,
       0,     0,     0,     0,   527,     0,     0,     0,   475,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   202,     0,
       0,    95,    96,    97,     0,   527,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   104,     0,     0,     0,     0,     0,     0,
       0,     0,   527,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   116,   117,   118,
     119,   120,   121,     0,     0,    49,    50,  1338,   125,   126,
     127,   128,   129,   130,     0,     0,     0,     0,     0,     0,
       0,     0,    54,    55,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   134,   135,   136,
     137,   138,   139,     0,   238,   239,     0,     0,     0,     0,
       0,     0,     0,     0,   244,   245,   246,   146,     0,    68,
       0,    70,     0,     0,     0,     0,     0,     0,     0,   147,
     247,     0,     0,     0,   248,     0,     0,     0,     0,   249,
       0,   241,   224,   225,   226,     5,   242,   227,   228,     8,
     229,   230,   231,   232,   233,   234,   235,     0,   528,     0,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,   677,   678,     0,    24,   679,   243,     0,     0,   528,
    1531,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,   677,   678,     0,     0,   679,     0,     0,   202,
       0,  1532,    95,    96,    97,     0,   528,  1339,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,   677,
     678,     0,     0,   679,   104,     0,     0,     0,  1533,     0,
       0,  1340,   527,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   116,   117,
     118,   119,   120,   121,     0,     0,    49,    50,  1328,   125,
     126,   127,   128,   129,   130,     0,     0,     0,     0,     0,
       0,     0,     0,    54,    55,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   134,   135,
     136,   137,   138,   139,     0,   238,   239,     0,     0,     0,
       0,     0,     0,     0,     0,   244,   245,   246,   146,     0,
      68,     0,    70,     0,     0,     0,     0,     0,     0,     0,
     147,   247,     0,     0,     0,   248,     0,     0,     0,     0,
     249,     0,   241,   224,   225,   226,     5,   242,   227,   228,
       8,   229,   230,   231,   232,   233,   234,   235,     0,   528,
       0,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,   677,   678,   527,    24,   679,   243,     0,     0,
     528,  1534,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,   677,   678,     0,     0,   679,     0,     0,
     202,     0,  1536,    95,    96,    97,     0,   528,  1329,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
     677,   678,     0,     0,   679,   104,     0,     0,     0,  1542,
       0,     0,     0,     0,     0,     0,     0,     0,   527,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   116,
     117,   118,   119,   120,   121,     0,     0,    49,    50,     0,
     125,   126,   127,   128,   129,   130,     0,     0,     0,     0,
       0,     0,   527,     0,    54,    55,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   134,
     135,   136,   137,   138,   139,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   244,   245,   246,   146,
       0,    68,     0,    70,     0,     0,     0,     0,     0,     0,
       0,   147,   247,     0,     0,     0,   248,     0,     0,     0,
       0,   249,     0,     0,     0,   527,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   274,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   243,     0,
       0,     0,     0,     0,     0,   527,     0,     0,     0,     0,
       0,     0,     0,     0,   987,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,   528,     0,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
     677,   678,     0,     0,   679,     0,   104,     0,     0,  1551,
       0,     0,   527,     0,     0,     0,     0,     0,     0,     0,
     988,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     116,   117,   118,   119,   120,   121,     0,   989,     0,     0,
       0,   125,   126,   127,   128,   129,   130,     0,   224,   225,
     226,     5,     0,   227,   228,     8,   229,   230,   231,   232,
     233,   234,   235,     0,     0,     0,     0,     0,     0,     0,
     134,   135,   136,   137,   138,   139,  1292,     0,     0,     0,
      24,     0,     0,     0,     0,     0,     0,   244,   245,   246,
     146,     0,  1252,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   147,   247,     0,     0,     0,   248,     0,     0,
       0,     0,   249,     0,     0,     0,     0,     0,     0,   528,
       0,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,   677,   678,     0,     0,   679,     0,     0,     0,
       0,  1552,     0,   224,   225,   226,     5,     0,   227,   228,
       8,   229,   230,   231,   232,   233,   234,   235,     0,     0,
       0,     0,    49,    50,     0,     0,   527,     0,     0,     0,
       0,     0,     0,     0,     0,    24,     0,     0,     0,    54,
      55,     0,     0,   528,    56,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,   677,   678,   527,     0,
     679,     0,     0,     0,     0,  1568,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,   528,    70,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
     677,   678,     0,     0,   679,     0,  1253,     0,     0,  1569,
       0,     0,     0,     0,     0,     0,  1254,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    49,    50,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   243,    54,    55,     0,     0,     0,    56,
     528,     0,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,   677,   678,   527,     0,   679,     0,    95,
      96,    97,  1570,     0,     0,     0,     0,     0,     0,     0,
    1293,    68,     0,    70,     0,     0,     0,     0,   527,     0,
     528,   104,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,   677,   678,     0,     0,   679,     0,     0,
       0,   527,  1571,     0,     0,   116,   117,   118,   119,   120,
     121,     0,     0,     0,     0,     0,   125,   126,   127,   128,
     129,   130,     0,     0,     0,     0,     0,   528,   243,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
     677,   678,     0,     0,   679,   134,   135,   136,   137,   138,
     139,     0,     0,     0,    95,    96,    97,     0,     0,     0,
       0,     0,   244,   245,   246,   146,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   104,   147,   247,     0,
       0,     0,   248,     0,     0,     0,     0,   249,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     116,   117,   118,   119,   120,   121,     0,     0,     0,     0,
       0,   125,   126,   127,   128,   129,   130,     0,     0,     0,
       0,   489,   490,   224,   225,   226,     5,     0,   227,   228,
       8,   229,   230,   231,   232,   233,   234,   235,     0,     0,
     134,   135,   136,   137,   138,   139,     0,     0,     0,   491,
     492,     0,     0,     0,     0,    24,     0,   244,   245,   246,
     146,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   147,   247,     0,     0,     0,   248,     0,     0,
       0,   528,   249,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,   677,   678,     0,     0,   679,     0,
       0,     0,     0,  1586,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   528,     0,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,   677,   678,     0,     0,
     679,     0,     0,     0,     0,  1588,     0,    49,    50,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,    55,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   224,
     225,   226,     5,     0,   227,   228,     8,   229,   230,   231,
     232,   233,   234,   235,     0,     0,     0,     0,     0,     0,
       0,    68,   527,    70,     0,     0,     0,     0,     0,     0,
       0,    24,     0,     0,     0,     0,     0,     0,     0,     0,
     528,     0,   666,   667,   668,   669,   670,   671,   672,   673,
     674,   675,   676,   677,   678,     0,     0,   679,     0,     0,
       0,     0,  1601,   528,     0,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,   677,   678,   243,     0,
     679,     0,     0,     0,     0,  1602,   528,     0,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,   677,
     678,     0,     0,   679,    95,    96,    97,     0,  1603,     0,
       0,     0,     0,    49,    50,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   104,     0,     0,     0,
      54,    55,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   738,     0,   527,   739,     0,
     116,   117,   118,   119,   120,   121,     0,     0,     0,     0,
       0,   125,   126,   127,   128,   129,   130,    68,     0,    70,
       0,   500,   501,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     134,   135,   136,   137,   138,   139,     0,     0,     0,   502,
     503,     0,     0,     0,     0,     0,     0,   244,   245,   246,
     146,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   147,   247,   243,     0,     0,   248,   224,   225,
     226,     5,   249,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,     0,     0,     0,     0,     0,     0,     0,
      95,    96,    97,     0,     0,   740,     0,     0,     0,     0,
      24,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   104,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   741,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   116,   117,   118,   119,
     120,   121,     0,     0,     0,     0,     0,   125,   126,   127,
     128,   129,   130,     0,     0,     0,     0,     0,     0,     0,
     224,   225,   226,     5,     0,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,     0,   134,   135,   136,   137,
     138,   139,    49,    50,     0,     0,     0,     0,     0,     0,
       0,     0,    24,   244,   245,   246,   146,     0,     0,    54,
      55,     0,     0,     0,    56,     0,     0,     0,   147,   247,
       0,     0,     0,   248,     0,     0,     0,   528,   249,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
     677,   678,     0,     0,   679,     0,    68,    69,    70,  1617,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   224,   225,
     226,     5,     0,   227,   228,     8,   229,   230,   231,   232,
     233,   234,   235,     0,    49,    50,     0,     0,     0,     0,
       0,   527,     0,     0,     0,     0,     0,     0,     0,     0,
      24,    54,    55,    80,     0,     0,    56,     0,     0,   517,
       0,     0,     0,     0,     0,     0,     0,     0,    88,   191,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,     0,     0,     0,     0,     0,     0,    68,    69,
      70,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   104,   528,     0,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,   677,   678,     0,     0,   679,
       0,     0,   527,     0,  1618,   116,   117,   118,   119,   120,
     121,     0,    49,    50,     0,     0,   125,   126,   127,   128,
     129,   130,     0,     0,     0,    80,     0,   527,     0,    54,
      55,  1406,     0,     0,    56,     0,     0,     0,     0,     0,
      88,     0,     0,     0,     0,   134,   135,   136,   137,   138,
     139,    95,    96,    97,     0,     0,     0,   527,     0,     0,
       0,     0,   244,   245,   246,   146,    68,     0,    70,     0,
       0,     0,     0,   104,     0,     0,     0,   147,   148,     0,
       0,     0,   149,     0,     0,     0,     0,   150,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   116,   117,   118,
     119,   120,   121,     0,     0,     0,     0,     0,   125,   126,
     127,   128,   129,   130,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   243,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   134,   135,   136,
     137,   138,   139,     0,   527,     0,     0,     0,     0,    95,
      96,    97,     0,     0,   244,   245,   246,   146,     0,   684,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   147,
     148,   104,     0,     0,   149,     0,     0,     0,     0,   150,
     224,   225,   226,     5,     0,   227,   228,     8,   229,   230,
     231,   232,   233,   234,   235,   116,   117,   118,   119,   120,
     121,     0,     0,     0,     0,     0,   125,   126,   127,   128,
     129,   130,    24,     0,     0,     0,     0,     0,     0,   769,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   134,   135,   136,   137,   138,
     139,     0,     0,   527,     0,     0,     0,     0,     0,     0,
       0,     0,   244,   245,   246,   146,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   147,   247,     0,
       0,     0,   248,     0,     0,     0,   528,   249,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,   677,
     678,     0,     0,   679,    49,    50,     0,     0,  1619,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,    55,     0,   770,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   224,   225,   226,     5,
       0,   227,   228,     8,   229,   230,   231,   232,   233,   234,
     235,     0,     0,     0,     0,     0,     0,     0,    68,     0,
      70,     0,     0,     0,     0,     0,     0,   528,    24,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
     677,   678,     0,     0,   679,     0,     0,   527,     0,  1634,
       0,     0,   528,   202,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,   677,   678,     0,     0,   679,
       0,     0,     0,     0,  1635,   243,   527,     0,     0,     0,
       0,     0,   528,     0,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,   677,   678,   527,     0,   679,
       0,    95,    96,    97,  1645,     0,     0,     0,     0,     0,
      49,    50,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   104,     0,     0,     0,    54,    55,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   116,   117,   118,
     119,   120,   121,     0,     0,     0,     0,     0,   125,   126,
     127,   128,   129,   130,    68,     0,    70,     0,     0,   528,
       0,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,   677,  1182,     0,   527,   679,   134,   135,   136,
     137,   138,   139,     0,     0,     0,     0,     0,     0,     0,
     274,     0,     0,     0,   244,   245,   246,   146,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   147,
     247,   243,     0,     0,   248,   224,   225,   226,     5,   249,
     227,   228,     8,   229,   230,   231,   232,   233,   234,   235,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
       0,     0,     0,  1305,     0,     0,     0,    24,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   528,   104,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,   677,   678,     0,     0,   679,     0,     0,     0,     0,
    1651,     0,     0,   116,   117,   118,   119,   120,   121,     0,
       0,     0,     0,     0,   125,   126,   127,   128,   129,   130,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   134,   135,   136,   137,   138,   139,    49,
      50,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     244,   245,   246,   146,     0,     0,    54,    55,     0,     0,
       0,    56,     0,     0,     0,   147,   247,     0,     0,     0,
     248,   224,   225,   226,     5,   249,   227,   228,     8,   229,
     230,   231,   232,   233,   234,   235,     0,     0,     0,     0,
       0,     0,     0,    68,     0,    70,     0,   546,     0,     0,
       0,     0,   528,    24,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,   677,   678,   527,     0,   679,
       0,     0,     0,     0,  1657,     0,     0,     0,     0,     0,
    1456,   528,     0,   666,   667,   668,   669,   670,   671,   672,
     673,   674,   675,   676,   677,   678,     0,     0,   679,     0,
     243,     0,   528,  1665,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,   677,   678,     0,     0,   679,
       0,     0,     0,     0,  1669,     0,    95,    96,    97,     0,
       0,     0,     0,     0,     0,    49,    50,  1306,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,     0,
       0,     0,    54,    55,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   527,     0,     0,
       0,     0,   116,   117,   118,   119,   120,   121,     0,     0,
       0,     0,     0,   125,   126,   127,   128,   129,   130,    68,
     528,    70,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   538,   539,   540,   541,     0,     0,   542,     0,     0,
       0,  1098,   134,   135,   136,   137,   138,   139,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   244,
     245,   246,   146,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   147,   247,   243,     0,  1457,   248,
     224,   225,   226,     5,   249,   227,   228,     8,   229,   230,
     231,   232,   233,   234,   235,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,     0,     0,     0,     0,     0,
       0,     0,    24,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   104,     0,     0,     0,     0,     0,
       0,   191,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   116,   117,
     118,   119,   120,   121,     0,     0,     0,     0,     0,   125,
     126,   127,   128,   129,   130,     0,     0,     0,     0,     0,
       0,     0,   224,   225,   226,     5,     0,   227,   228,     8,
     229,   230,   231,   232,   233,   234,   235,     0,   134,   135,
     136,   137,   138,   139,    49,    50,     0,     0,     0,     0,
       0,     0,     0,     0,    24,   244,   245,   246,   146,     0,
       0,    54,    55,     0,     0,     0,    56,     0,     0,     0,
     147,   247,     0,     0,     0,   248,     0,     0,     0,     0,
     249,   527,   547,     0,   548,   549,   550,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,     0,    68,   561,
      70,     0,   528,  1099,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,   677,  1184,     0,     0,   679,
     224,   225,   226,     5,     0,   227,   228,     8,   229,   230,
     231,   232,   233,   234,   235,     0,    49,    50,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    24,    54,    55,   243,     0,     0,    56,     0,
       0,     0,     0,   546,     0,     0,     0,     0,     0,     0,
       0,   191,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,    96,    97,     0,     0,     0,     0,     0,     0,
      68,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   528,   104,   666,   667,   668,   669,   670,   671,
     672,   673,   674,   675,   676,   677,   678,     0,     0,   679,
       0,     0,     0,   847,   546,     0,     0,   116,   117,   118,
     119,   120,   121,     0,    49,    50,     0,     0,   125,   126,
     127,   128,   129,   130,   527,     0,     0,   243,     0,     0,
       0,    54,    55,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   134,   135,   136,
     137,   138,   139,    95,    96,    97,   527,     0,     0,     0,
       0,     0,     0,     0,   244,   245,   246,   146,    68,     0,
      70,     0,     0,     0,     0,   104,     0,     0,     0,   147,
     247,     0,     0,     0,   248,     0,     0,     0,     0,   249,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   116,
     117,   118,   119,   120,   121,     0,     0,   452,     0,     0,
     125,   126,   127,   128,   129,   130,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   243,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   134,
     135,   136,   137,   138,   139,     0,   527,     0,     0,     0,
       0,    95,    96,    97,     0,     0,   244,   245,   246,   146,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   147,   247,   104,     0,     0,   248,     0,     0,     0,
       0,   249,   224,   225,   226,     5,     0,   227,   228,     8,
     229,   230,   231,   232,   233,   234,   235,   116,   117,   118,
     119,   120,   121,     0,     0,     0,     0,     0,   125,   126,
     127,   128,   129,   130,    24,     0,  -171,     0,   666,   667,
     668,   669,   670,   671,   672,   673,   674,   675,   676,   677,
     678,     0,     0,   679,     0,     0,     0,   134,   135,   136,
     137,   138,   139,     0,     0,   546,     0,     0,     0,     0,
       0,     0,     0,     0,   244,   245,   246,   146,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   147,
     247,     0,     0,     0,   248,     0,     0,     0,     0,   639,
       0,     0,     0,     0,     0,     0,   370,   527,     0,     0,
       0,     0,     0,     0,     0,     0,    49,    50,   547,     0,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,    54,    55,   561,     0,     0,    56,  1198,
       0,     0,     0,     0,     0,     0,     0,     0,   224,   225,
     226,     5,     0,   227,   228,     8,   229,   230,   231,   232,
     233,   234,   235,     0,     0,     0,     0,     0,     0,     0,
      68,     0,    70,     0,     0,     0,     0,     0,     0,   547,
      24,   548,   549,   550,   551,   552,   553,   554,   555,   556,
     557,   558,   559,   560,     0,     0,   561,     0,     0,   528,
    1359,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,   677,   678,   546,     0,   679,     0,     0,     0,
    1098,     0,     0,     0,     0,     0,     0,   243,     0,     0,
       0,   528,     0,   529,   530,   531,   532,   533,   534,   535,
     536,   537,   538,   539,   540,   541,   527,     0,   542,     0,
       0,     0,  1462,    95,    96,    97,     0,     0,     0,     0,
       0,     0,    49,    50,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   104,     0,     0,     0,    54,
      55,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   116,
     117,   118,   119,   120,   121,     0,     0,     0,     0,     0,
     125,   126,   127,   128,   129,   130,    68,     0,    70,     0,
       0,  -171,     0,   529,   530,   531,   532,   533,   534,   535,
     536,   537,   538,   539,   540,   541,     0,   546,   542,   134,
     135,   136,   137,   138,   139,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   244,   245,   246,   146,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   147,   247,   243,     0,     0,   248,   224,   225,   226,
       5,   249,   227,   228,     8,   229,   230,   231,   232,   233,
     234,   235,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,     0,     0,     0,     0,     0,     0,     0,    24,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     547,   104,   548,   549,   550,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,  1318,     0,   561,     0,     0,
     546,  1463,     0,     0,     0,   116,   117,   118,   119,   120,
     121,     0,     0,     0,     0,     0,   125,   126,   127,   128,
     129,   130,   528,     0,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,   539,   540,   541,     0,     0,   542,
       0,     0,     0,  1464,     0,   134,   135,   136,   137,   138,
     139,    49,    50,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   244,   245,   246,   146,     0,     0,    54,    55,
       0,     0,     0,    56,     0,     0,     0,   147,   247,     0,
       0,     0,   248,   224,   225,   226,     5,   249,   227,   228,
       8,   229,   230,   231,   232,   233,   234,   235,     0,     0,
       0,     0,     0,     0,     0,    68,     0,    70,     0,   546,
       0,     0,     0,     0,     0,    24,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   547,
       0,   548,   549,   550,   551,   552,   553,   554,   555,   556,
     557,   558,   559,   560,   527,     0,   561,     0,     0,     0,
    1465,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   528,   243,   529,   530,   531,   532,   533,   534,   535,
     536,   537,   538,   539,   540,   541,     0,     0,   542,     0,
       0,     0,  1466,     0,   527,     0,     0,     0,    95,    96,
      97,     0,     0,     0,     0,     0,     0,    49,    50,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     104,     0,     0,     0,    54,    55,     0,     0,     0,    56,
       0,     0,     0,     0,  1320,     0,     0,     0,     0,   527,
       0,     0,     0,     0,   116,   117,   118,   119,   120,   121,
       0,     0,     0,     0,     0,   125,   126,   127,   128,   129,
     130,    68,   547,    70,   548,   549,   550,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,     0,     0,   561,
       0,     0,     0,  1467,   134,   135,   136,   137,   138,   139,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   244,   245,   246,   146,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   147,   247,   243,     0,
       0,   248,   224,   225,   226,     5,   249,   227,   228,     8,
     229,   230,   231,   232,   233,   234,   235,     0,     0,     0,
       0,     0,     0,     0,    95,    96,    97,     0,     0,     0,
       0,     0,     0,     0,    24,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   547,   104,   548,   549,   550,
     551,   552,   553,   554,   555,   556,   557,   558,   559,   560,
    1351,     0,   561,     0,     0,     0,  1487,     0,     0,     0,
     116,   117,   118,   119,   120,   121,     0,     0,     0,     0,
       0,   125,   126,   127,   128,   129,   130,     0,     0,     0,
       0,     0,     0,     0,   224,   225,   226,     5,     0,   227,
     228,     8,   229,   230,   231,   232,   233,   234,   235,     0,
     134,   135,   136,   137,   138,   139,    49,    50,     0,   546,
       0,     0,     0,     0,     0,     0,    24,   244,   245,   246,
     146,     0,     0,    54,    55,     0,     0,     0,    56,   527,
       0,     0,   147,   247,     0,     0,     0,   248,     0,     0,
       0,     0,   249,     0,   547,     0,   548,   549,   550,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,   546,
      68,   561,    70,     0,     0,  1500,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   528,
       0,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,   677,   678,   527,     0,   679,     0,    49,    50,
    1462,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   527,    54,    55,   243,     0,   528,
      56,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,   677,   678,   527,     0,   679,     0,     0,     0,
    1464,     0,     0,    95,    96,    97,     0,     0,     0,     0,
       0,     0,    68,     0,    70,     0,   546,     0,     0,     0,
       0,     0,     0,     0,   528,   104,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,   539,   540,   541,   527,
       0,   542,     0,     0,     0,  1564,     0,     0,     0,   116,
     117,   118,   119,   120,   121,     0,     0,     0,     0,   527,
     125,   126,   127,   128,   129,   130,     0,     0,     0,   243,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   546,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   134,
     135,   136,   137,   138,   139,    95,    96,    97,     0,   527,
       0,     0,     0,     0,     0,     0,   244,   245,   246,   146,
       0,     0,     0,     0,     0,     0,     0,   104,     0,   527,
       0,   147,   247,     0,     0,     0,   248,     0,     0,     0,
       0,   249,     0,     0,     0,     0,     0,     0,     0,   527,
       0,   116,   117,   118,   119,   120,   121,     0,     0,     0,
       0,     0,   125,   126,   127,   128,   129,   130,     0,   546,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   527,
       0,   134,   135,   136,   137,   138,   139,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   244,   245,
     246,   765,   546,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   147,   247,     0,     0,     0,   248,     0,
       0,     0,     0,   249,   547,     0,   548,   549,   550,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,     0,
       0,   561,     0,     0,   528,  1565,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,   539,   540,   541,     0,
       0,   542,     0,     0,     0,  1566,     0,     0,     0,     0,
       0,     0,     0,     0,   547,     0,   548,   549,   550,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,     0,
       0,   561,     0,     0,     0,  1567,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   528,
       0,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,   677,   678,     0,     0,   679,     0,     0,   528,
    1564,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   676,   677,   678,     0,     0,   679,     0,     0,   528,
    1566,   529,   530,   531,   532,   533,   534,   535,   536,   537,
     538,   539,   540,   541,     0,     0,   542,     0,     0,     0,
    1615,   547,     0,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,   559,   560,     0,     0,   561,     0,
       0,     0,  1616,     0,   528,     0,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,   677,   678,     0,
       0,   679,     0,     0,   528,  1615,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,   539,   540,   541,     0,
       0,   542,     0,     0,   547,  1643,   548,   549,   550,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,     0,
       0,   561,     0,     0,   528,  1644,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,   677,   678,     0,
       0,   679,     0,     0,   528,  1643,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,   677,   678,     0,
       0,   679,     0,     0,   528,  1660,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,   539,   540,   541,     0,
       0,   542,     0,     0,   547,  1661,   548,   549,   550,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,     0,
       0,   561,     0,     0,   528,  1662,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,   677,   678,     0,
     573,   679,     0,     0,     0,  1661,   574,  -171,     0,   548,
     549,   550,   551,   552,   553,   554,   555,   556,   557,   558,
     559,   560,    17,    18,   561,     0,   202,   575,     0,     0,
       0,     0,   563,   573,     0,     0,     0,   564,     0,   574,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   576,   577,     0,   202,
     575,   578,     0,     0,     0,     0,   579,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   576,
     577,     0,   580,     0,   578,     0,     0,     0,     0,   579,
       0,     0,     0,   565,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   581,   582,     0,
     583,     0,   238,   239,   584,   580,   566,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   567,     0,     0,
     581,   582,     0,   583,     0,   238,   239,   584,     0,   241,
       0,     0,     0,     0,   242,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     585,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   241,   241,     0,     0,     0,   242,   242,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   585,     0,   312,     0,     0,     0,   313,
     314,   315,   316,   317,   318,     0,   586,     0,   319,   320,
     321,   322,   323,   324,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   587,   325,     0,     0,   326,
       0,     0,     0,     0,     0,     0,   588,     0,     0,   586,
       0,     0,     0,     0,     0,     0,     0,   327,     0,   568,
       0,     0,     0,     0,     0,     0,     0,     0,   587,     0,
     328,     0,     0,     0,     0,     0,     0,     0,     0,   588,
     569,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   589,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   132,
       0,     0,     0,     0,     0,     0,     0,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   589,     0,
       0,     0,     0,     0,     0,     0,   590,   141,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   340,     0,     0,     0,     0,     0,  1227,
     631,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   341,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   342,   343,   344,   345
};

static const yytype_int16 yycheck[] =
{
       0,    49,   293,   167,   590,   178,    35,    69,   181,    62,
     183,   184,    81,   186,    79,   104,    27,    19,    20,   124,
      75,    75,    33,   925,    62,    46,    75,    27,    17,    40,
     182,    37,    38,   122,   145,   324,    47,   158,    47,    39,
      40,    47,   111,    46,    66,   402,    52,    34,    96,   294,
     103,    62,   105,    64,    46,    27,   109,   110,    44,   158,
     962,    33,    62,    46,    64,   167,    45,   102,    40,   208,
     222,    69,   397,    46,   213,    47,    19,    20,   208,   397,
      75,    62,    63,   213,   114,    60,    74,    69,   330,   218,
      62,   397,    64,    33,   124,    22,   427,   428,    98,   430,
      51,    99,    40,   103,    75,    77,   247,   155,   397,   109,
     397,    97,   118,   355,   169,   115,   171,   172,    58,    69,
     249,    64,   397,   123,    75,    75,   215,   305,   301,   218,
     375,   103,   395,   105,    75,   141,   399,   109,   110,   128,
    1042,   123,   142,   165,   389,   266,   148,   147,   148,   149,
     150,   397,    75,   208,   208,    98,   162,   168,   213,   213,
     135,   136,   132,   133,   115,   108,   204,   266,   222,   280,
      75,    62,   122,   222,   169,   305,   171,   172,   178,   179,
     397,   181,   182,   356,   165,   185,   397,   187,   188,   189,
     190,   170,   227,   125,    52,   183,   168,   132,   133,   122,
     311,    25,   208,    27,   144,   148,   146,   213,    62,    33,
     212,   397,   457,   208,   397,   202,    40,   395,   213,   206,
     171,   172,   172,    47,   363,   165,   169,   397,   237,   204,
      41,   378,   254,   204,   223,    69,    69,   225,    62,   239,
      64,    69,    75,   395,   214,    27,   221,   247,   248,   249,
     305,   222,   397,   584,   401,   397,    67,   208,    40,   280,
     298,    99,   213,    45,    75,    47,   171,   172,   322,   212,
     208,   222,   400,   254,   102,   213,    75,   280,   284,   103,
      62,   105,    64,   338,   293,   109,   110,   397,   280,   122,
     395,   397,   232,   233,    72,   302,    61,   280,   124,   305,
     270,   271,   281,   303,   162,   237,   395,   280,   129,   314,
     305,   122,   397,   124,   254,   206,   397,   222,   323,   132,
     133,   103,   303,   397,   274,   216,   208,   109,   334,   304,
     397,   213,   399,   395,  1236,   270,   271,   402,   347,   172,
     283,   397,   397,   338,   168,   397,   321,   290,   177,   178,
     208,   209,   206,    22,   305,   213,   362,   368,   358,   368,
     102,   361,   216,   333,   204,   172,   395,   132,   133,   301,
     310,   544,   312,   373,   376,   167,    18,   206,   378,   379,
     397,    64,   208,   270,   271,   167,   168,   213,   170,   395,
     322,   361,   402,   171,   172,   177,   368,   208,   209,   227,
     305,   401,   213,   257,   397,   204,   397,   219,   408,   147,
     192,   237,   224,   237,   414,   197,   208,   246,   402,   395,
      67,   213,   422,   222,   400,   108,   237,   238,    75,   167,
     208,   431,   432,   606,   434,   213,   397,   395,   296,   297,
     397,   274,   400,   443,   402,   256,   395,   305,     0,   132,
     133,   400,   452,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,   465,   208,   394,   396,   293,
     643,   213,   645,   402,   647,   122,   397,    22,   397,   305,
     596,   481,   598,   599,   249,   227,   169,   398,   399,   489,
     490,   491,   492,   333,   305,   398,   399,    22,    75,   339,
     500,   501,   502,   503,   397,   270,   271,    75,   598,   599,
     395,   302,   338,   513,   514,   515,   397,   117,   397,   588,
     573,   361,   397,   347,   171,   172,   397,   127,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   538,   539,
     540,   541,   542,   596,   368,   598,   397,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,   561,   562,   163,   395,   397,   348,   578,   395,   578,
     579,   302,   583,   395,   583,   222,   587,   397,   587,   397,
     397,   397,   274,     0,   595,   585,   368,   270,   271,   589,
     372,   400,   330,   331,   171,   172,   274,   597,   265,   167,
     274,   302,   302,   171,   172,   605,   578,   290,   115,   274,
     380,   583,   274,    75,   396,   587,   397,   355,   356,   357,
     358,   359,   360,   595,   596,   272,   598,   599,   394,   629,
     394,   208,   389,   390,   391,   235,   213,   394,   394,   639,
     208,  1227,   402,   167,   644,   213,   646,   294,   648,   649,
     650,   651,   305,   115,   222,    22,   389,   390,   391,   140,
     228,   394,   370,   399,   398,   274,   666,   667,   668,   669,
     670,   671,   672,   673,   674,   675,   676,   677,   678,   679,
     680,   398,   398,   683,   684,   399,   399,   255,   399,   689,
     399,   220,   692,   857,   389,   390,   391,   399,   989,   394,
     398,   701,   401,   147,   302,   399,   274,   302,   399,   171,
     172,   720,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,    22,   506,   394,   726,   509,   399,   399,
     512,   148,   149,   150,   398,   375,   736,   305,   738,   739,
     740,   399,   399,   398,   744,   399,   208,   398,   324,   573,
     399,   213,   274,   399,   578,   579,   399,    71,   274,   583,
     222,   274,   399,   587,   181,   182,   399,   375,   185,   769,
     770,   595,   596,   375,   598,   599,    59,   399,   399,    62,
     399,   399,   399,   565,   566,   567,   568,    22,   401,   571,
     397,   573,   574,   129,   298,   577,   578,   298,   580,   526,
     399,   398,   584,   399,   804,   399,   399,   121,   856,   809,
     399,   399,   812,   595,   398,   815,   598,   599,   132,   133,
     398,   401,   822,   398,   398,   398,   270,   271,   399,   398,
     571,   831,   377,   399,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   858,    22,   394,
     399,   399,   399,   398,   399,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   399,   399,   394,
    1161,   315,   316,   399,   318,   319,   399,   398,   398,   398,
     398,     0,   326,   327,   328,   329,   858,   399,   330,   331,
     890,   399,    22,   398,   208,   398,   720,   399,   398,   213,
      19,   949,    67,    22,    69,   398,    34,   399,   399,   398,
      75,   693,    31,   355,   356,   357,   358,   359,   360,   399,
     399,   398,    -1,   206,   924,   510,   926,  1100,   928,    -1,
     213,  1104,  1028,   216,    -1,    -1,    -1,    -1,   720,    -1,
      -1,    -1,    61,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    71,  1006,    -1,    -1,   270,   122,    -1,    -1,
      -1,    -1,    -1,   963,    -1,    -1,    -1,   978,    87,    -1,
      -1,    -1,   754,    -1,    -1,    -1,   976,    -1,    -1,   979,
      99,    -1,   101,    -1,    -1,    22,    -1,   106,   107,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,  1007,   164,
      -1,    -1,    -1,   122,    -1,   124,   978,   172,  1008,    -1,
     377,    -1,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,    -1,   443,   394,    -1,  1029,
      -1,    -1,   399,    -1,   858,    -1,    -1,    22,    -1,    -1,
      -1,    -1,  1042,    -1,    -1,  1045,    -1,    -1,    -1,    -1,
     215,    -1,  1052,  1053,    -1,    -1,  1229,    -1,  1069,    -1,
      -1,  1072,    -1,   228,    -1,    -1,    -1,    -1,    -1,  1069,
      -1,    -1,  1072,  1073,  1074,   857,   858,  1077,   377,    -1,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,    22,    -1,   394,    -1,  1069,    -1,   398,
    1072,  1101,  1102,  1103,    -1,  1105,  1106,  1107,  1108,   526,
      -1,    -1,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   538,   539,   540,   541,   542,    -1,    -1,    -1,    -1,
     547,   548,   549,   550,   551,   552,   553,   554,   555,   556,
     557,   558,   559,   560,   561,   562,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,  1157,  1158,   394,
    1160,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1177,    -1,    -1,
    1180,    -1,  1182,  1007,  1184,    -1,    -1,    -1,   605,    -1,
      -1,    -1,    -1,  1193,  1194,    -1,   978,  1197,    -1,    -1,
      -1,    -1,  1202,   377,  1377,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,   391,    -1,    -1,
     394,    -1,    -1,  1005,    -1,   399,    -1,    -1,  1228,    -1,
      -1,  1231,  1232,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   360,  1242,   362,   363,  1069,    -1,   377,  1072,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,    -1,    -1,   394,    -1,    -1,   386,    -1,   399,
      -1,    -1,    -1,    -1,  1274,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1064,  1283,  1284,   404,  1068,  1069,    -1,    -1,
    1072,    -1,    -1,    -1,    -1,  1295,    -1,    -1,  1298,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1089,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   435,    -1,    -1,    -1,
      -1,   440,   441,    -1,    -1,    -1,    -1,    -1,    -1,  1329,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1339,
     377,    -1,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,  1355,    -1,   394,    -1,    -1,
      -1,    -1,   399,    -1,    -1,    -1,  1148,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    -1,  1375,  1376,    -1,  1378,  1379,
    1380,  1381,  1382,    -1,    -1,    -1,    -1,    -1,  1388,   508,
      -1,    -1,    -1,    -1,  1394,  1395,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   526,    -1,   394,
    1410,  1411,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1421,  1422,    -1,  1424,    28,  1426,    30,    -1,    -1,
      52,  1431,    35,    36,  1434,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1444,    48,    68,    -1,  1448,    52,
      -1,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,    66,    -1,   394,    -1,    -1,  1468,  1469,
    1470,  1471,  1472,  1473,  1256,    -1,    79,    22,    81,    -1,
      -1,    -1,    -1,    -1,   603,   604,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    -1,    -1,   117,    -1,  1497,    -1,    -1,
      -1,  1501,   105,    -1,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   632,    -1,    -1,   138,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,  1529,
    1530,  1531,  1532,  1533,  1534,    -1,  1536,    -1,    -1,    -1,
      -1,    -1,  1542,    22,    -1,    -1,  1328,    -1,    -1,    -1,
      -1,  1551,  1552,    -1,    -1,    -1,  1338,   179,   180,   181,
      -1,    -1,    -1,   682,    -1,    -1,    -1,  1349,  1568,  1569,
    1570,  1571,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1586,    -1,  1588,    -1,
     193,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1601,  1602,  1603,   226,    -1,    -1,    -1,   230,   231,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1617,  1618,  1619,
     223,    -1,    74,    22,    -1,    -1,  1626,    -1,    -1,    -1,
      -1,    -1,   751,    -1,  1634,  1635,    -1,    -1,    -1,    -1,
      -1,    -1,  1642,    -1,    -1,  1645,    -1,    -1,    -1,    -1,
     102,  1651,    22,    -1,    -1,   277,   108,  1657,    -1,    -1,
      -1,    -1,   114,   285,   286,  1665,    -1,    -1,    -1,  1669,
     273,    -1,   275,    -1,   148,   149,   150,    -1,    -1,    -1,
      -1,    -1,   285,    -1,  1101,  1102,  1103,    -1,    -1,    -1,
      -1,    -1,   811,    -1,    -1,    -1,    -1,    -1,    -1,   302,
      -1,   304,    -1,    -1,  1486,    -1,    -1,   181,   182,    -1,
      -1,   185,    -1,    -1,    -1,    -1,   338,  1499,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   180,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
    1157,  1158,   394,  1160,   196,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1180,    -1,    -1,    -1,   370,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1197,    -1,    -1,    -1,    -1,    -1,   238,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1580,  1581,
     403,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1592,    -1,    -1,    -1,  1231,  1232,    -1,    -1,   421,    -1,
      -1,   273,   274,   426,   427,   428,    -1,   430,   431,    -1,
      -1,    -1,   377,    -1,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,    -1,    -1,   394,
      -1,    -1,   455,   305,   399,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1647,    -1,    -1,    -1,    -1,
      -1,   474,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1663,    22,    -1,    -1,    -1,  1668,   361,    -1,    -1,
      -1,  1673,    -1,    -1,    -1,    -1,    -1,   349,   377,    -1,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,    22,    -1,   394,    -1,    -1,   370,   371,
     399,    -1,   374,    -1,   376,   377,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   387,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   399,   400,   401,
      -1,    -1,    -1,    -1,    -1,    -1,   408,    22,    -1,    -1,
     412,    -1,    -1,   415,    -1,    -1,    -1,    -1,    -1,   443,
      -1,    -1,   575,   425,    -1,    -1,    -1,    -1,   581,    -1,
      -1,    22,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,   596,    -1,   394,    -1,    -1,    -1,   451,
      -1,    -1,    -1,    -1,   456,    22,   458,   377,    -1,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,    -1,   475,   394,    -1,   629,    -1,    22,   399,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   494,   495,   496,   497,    -1,    -1,    -1,  1168,
      -1,  1468,  1469,    22,    -1,   529,   530,   531,   532,   533,
     534,   535,   536,   537,   538,   539,   540,   541,   542,    -1,
      -1,    -1,    -1,   547,   548,   549,   550,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,   561,   562,    -1,
      -1,   694,     0,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,  1225,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    22,    -1,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    31,   576,    22,    -1,   579,    -1,   732,
     582,   605,    40,    -1,   586,   587,    -1,    45,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   598,   599,    -1,    -1,
      -1,  1568,    -1,    61,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   774,    -1,   776,    -1,   778,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   639,    -1,    -1,
      -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,   107,
    1617,    -1,   654,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1645,   832,
     833,    -1,    -1,    -1,    -1,    -1,    -1,   377,   690,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,    -1,    -1,   394,    -1,    -1,    -1,    -1,   399,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   377,    -1,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,    -1,    -1,   394,    22,    -1,    -1,   741,
     399,   377,    -1,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   391,    -1,    -1,   394,    -1,
      22,    -1,   377,   399,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,    -1,    -1,   394,
      -1,    -1,    -1,    -1,   399,    -1,   377,    -1,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,    -1,    -1,   394,    -1,    -1,    -1,   960,   399,    -1,
      -1,    -1,   965,    -1,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,    -1,    -1,   394,    -1,    -1,
     288,    -1,    -1,   377,    -1,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,   391,    -1,  1002,
     394,    -1,    -1,    -1,    22,   399,    -1,    -1,   377,    -1,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,    -1,    -1,   394,    -1,    22,    -1,    -1,
     399,    -1,    -1,    -1,    -1,    -1,    -1,  1040,    -1,    -1,
    1043,    -1,    -1,    -1,    -1,  1048,    -1,    -1,    -1,    -1,
      -1,    -1,   360,    22,   362,   363,    -1,   377,    -1,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,    -1,    -1,   394,   927,    -1,    -1,   386,   399,
     377,    -1,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,    22,   404,   394,    -1,    -1,
      -1,    -1,   399,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   964,    -1,   966,   967,    -1,    -1,    -1,   971,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   435,    -1,    -1,
      -1,    -1,   440,   441,    -1,   987,   988,    -1,    -1,    -1,
      -1,    -1,    -1,  1146,   996,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,  1006,  1007,    -1,  1009,   377,    -1,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,  1176,    -1,   394,    -1,    -1,    -1,    -1,
     399,    -1,    -1,    -1,    -1,    -1,    -1,  1039,    -1,    -1,
      -1,    22,    -1,    -1,    -1,  1047,    -1,    -1,  1050,  1051,
     508,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,  1066,    -1,    -1,  1069,   526,    -1,
    1072,    -1,    -1,    -1,    -1,    -1,  1078,  1101,  1102,  1103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   377,  1094,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   391,    22,    -1,   394,    -1,
    1112,    -1,    -1,   399,    -1,   377,    -1,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
      -1,    -1,   394,  1157,  1158,    -1,  1160,   399,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   603,   604,    -1,    -1,  1151,
    1152,    -1,    -1,    -1,    -1,    -1,  1180,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1324,    -1,  1197,   632,    -1,    -1,    -1,  1180,    -1,
      -1,  1183,    -1,    -1,  1186,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1196,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1231,  1232,   377,
      -1,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,   682,    -1,   394,    -1,    -1,    -1,
      -1,   399,   377,    -1,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,    -1,  1250,   394,
      -1,    -1,    -1,    -1,   399,    -1,  1409,    -1,   377,  1412,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,    -1,    -1,   394,    -1,    -1,    -1,    -1,
     399,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   746,    -1,
      -1,    -1,    -1,   751,    -1,    -1,    -1,    -1,    -1,    -1,
     377,    -1,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,    -1,    -1,   394,    -1,    -1,
      -1,    -1,   399,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1340,    -1,
    1493,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   811,    -1,  1357,  1358,   377,    -1,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,  1374,    -1,   394,    -1,    -1,    -1,    -1,   399,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1396,    -1,   377,  1550,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,    -1,    -1,   394,    -1,    -1,    -1,   377,   399,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,    -1,    -1,   394,    -1,    -1,    -1,    -1,   399,
      -1,    -1,    -1,    -1,  1468,  1469,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1460,    -1,
      -1,   377,    -1,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   391,    -1,    -1,   394,    -1,
      -1,  1483,    -1,   399,    -1,    -1,    -1,     0,     1,    -1,
      -1,    -1,  1494,    -1,  1496,    -1,    -1,  1650,    -1,    -1,
      -1,    -1,    -1,    -1,  1506,    18,    19,    20,    21,    -1,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,    35,    -1,    37,    38,  1528,    -1,    41,    42,
      -1,    -1,    -1,    46,    47,    48,    -1,    50,    51,    -1,
      -1,  1543,    -1,    -1,  1568,    -1,    -1,    -1,    61,    -1,
      -1,    64,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      73,  1563,    -1,    -1,    -1,    -1,    -1,    80,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,   100,   101,    -1,
      -1,    -1,    -1,  1617,   107,   108,    -1,    -1,   111,    -1,
     113,    -1,    -1,   116,  1606,    -1,   119,   120,    -1,    -1,
      -1,   124,  1614,   126,    -1,    -1,    -1,    -1,   131,   132,
     133,  1645,    -1,    -1,   137,    -1,   139,    -1,    -1,   142,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,    -1,    -1,
      -1,   154,   155,   156,   157,    -1,   159,    -1,   161,    -1,
      -1,    -1,    -1,   166,    -1,   168,   169,    -1,    -1,    -1,
     173,   174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,   187,   188,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,    -1,
     203,    -1,   205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,    -1,    -1,
    1168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   234,    -1,    -1,    -1,    -1,   239,   240,   241,   242,
     243,   244,    -1,    -1,   247,   248,   249,   250,    -1,   252,
     253,    -1,    -1,    -1,    -1,   258,    -1,   260,   261,   262,
     263,   264,   265,    -1,    -1,    -1,   269,   270,   271,    -1,
      -1,    -1,    -1,   276,    -1,   278,   279,  1225,    -1,   282,
     283,    -1,    -1,    -1,    -1,    -1,    -1,   290,   291,   292,
     293,    -1,   295,    -1,    -1,    -1,   299,   300,   301,    -1,
      -1,    -1,   305,    -1,   307,   308,   309,    -1,    -1,    -1,
      -1,   314,   315,   316,   317,   318,   319,   320,    -1,   322,
     323,    -1,   325,   326,   327,   328,   329,   330,   331,    -1,
     333,   334,   335,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   355,   356,   357,   358,   359,   360,   361,   362,
       0,     1,    -1,    -1,    -1,    -1,    -1,    -1,   371,   372,
     373,   374,   375,    -1,    -1,    -1,    -1,    -1,    18,    19,
      20,    -1,    -1,    -1,   387,   388,    -1,    -1,    -1,   392,
      -1,    -1,    -1,   396,   397,    35,    -1,    37,    38,    -1,
      22,    41,    42,    -1,    -1,    -1,    46,    47,    48,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    22,    64,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    -1,
      80,    81,    -1,    -1,    -1,    -1,    22,    -1,    -1,    89,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    99,
     100,   101,    -1,    -1,    -1,    -1,    -1,   107,   108,    -1,
      -1,   111,    -1,   113,    22,    -1,   116,    -1,    -1,   119,
     120,    -1,    -1,    -1,   124,    -1,   126,    -1,    -1,    -1,
      -1,   131,   132,   133,    -1,    -1,    -1,   137,    22,   139,
      -1,    -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   155,   156,   157,    -1,   159,
      -1,   161,    -1,    -1,    -1,    -1,   166,    -1,   168,   169,
      -1,    -1,    -1,   173,   174,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   185,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   201,    -1,   203,    -1,   205,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    19,    20,    21,   218,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    -1,    -1,    -1,   234,    -1,    -1,    -1,    -1,   239,
     240,   241,   242,    -1,   244,    -1,    50,   247,   248,   249,
     250,    -1,   252,   253,    -1,    -1,    -1,    -1,    -1,    -1,
     260,   261,   262,   263,   264,   265,    -1,    -1,    -1,    -1,
     270,   271,    -1,    -1,    -1,    22,   276,    -1,   278,   279,
      -1,    -1,   282,   283,    -1,    -1,    -1,    -1,    -1,    -1,
     290,    -1,   292,   293,    -1,   295,    -1,    -1,    -1,   299,
     300,   301,    -1,    -1,    -1,   305,    -1,   307,   308,   309,
      -1,    -1,    -1,    -1,   314,    -1,    22,    -1,    -1,    -1,
      -1,    -1,   322,   323,    -1,   325,    -1,    -1,   132,   133,
     330,   331,    -1,   333,   334,   335,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   149,   150,    -1,    -1,    -1,
     154,    -1,    -1,    -1,    -1,   355,   356,   357,   358,   359,
     360,   361,   362,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   371,   372,   373,   374,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   186,    -1,   188,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   377,   396,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
      22,    -1,   394,   297,    -1,    -1,    -1,   399,   377,    -1,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,    -1,    -1,   394,    -1,    -1,    -1,   243,
     399,   377,    -1,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   391,    -1,    -1,   394,    -1,
      -1,    -1,    -1,   399,    -1,   269,   270,   271,    -1,   377,
      -1,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,    -1,    -1,   394,   291,    -1,    -1,
      -1,   399,    -1,   377,    22,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,   391,    -1,    -1,
     394,   315,   316,   317,   318,   319,   320,    -1,    -1,    -1,
      -1,    -1,   326,   327,   328,   329,   330,   331,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,   355,   356,   357,   358,   359,   360,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,   372,   373,
     374,   375,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   387,   388,    -1,    -1,    -1,   392,    -1,
      -1,    -1,    -1,   397,    78,    -1,    -1,   401,    82,    83,
      84,    85,    86,    87,    -1,    22,    -1,    91,    92,    93,
      94,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
     377,    -1,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,    -1,   130,   394,   132,   133,
      -1,    -1,   399,    -1,    -1,    -1,    -1,    -1,    -1,   143,
      -1,    -1,    -1,    -1,    -1,   149,   150,    -1,    -1,    -1,
     154,   377,    -1,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   391,    -1,    -1,   394,    -1,
      -1,    -1,    -1,   399,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   186,    -1,   188,    -1,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,    -1,   377,    -1,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,    -1,    -1,   394,    -1,    -1,    -1,    -1,
     399,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    -1,   236,    -1,    -1,    -1,    -1,    -1,    -1,   243,
      -1,    -1,    -1,    -1,    -1,   377,   378,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
      -1,    -1,   394,   267,    -1,   269,   270,   271,    -1,    -1,
      -1,    18,    19,    20,    21,    -1,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,   291,    -1,    -1,
      -1,    -1,    39,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   315,   316,   317,   318,   319,   320,    -1,    -1,    -1,
      67,    -1,   326,   327,   328,   329,   330,   331,    75,   377,
      -1,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,    -1,    -1,   394,    -1,    -1,    -1,
     398,   355,   356,   357,   358,   359,   360,    22,    -1,    -1,
      -1,    -1,   366,   367,   368,   369,    -1,    -1,   372,   373,
     374,   375,    -1,    -1,    -1,   122,    -1,   124,   125,    -1,
      -1,    -1,    -1,   387,   388,   132,   133,    -1,   392,    52,
      53,    54,    -1,   397,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,   150,    -1,    -1,    -1,   154,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,
     377,   188,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,    -1,    -1,   394,    -1,    -1,
      -1,   208,   209,    22,    -1,    -1,   213,    -1,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   228,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     237,   238,    -1,    -1,    -1,    -1,   243,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    19,    20,    21,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
      -1,    36,   269,   270,   271,    -1,   273,    -1,   275,    -1,
      -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
     287,   288,   289,    -1,   291,    -1,    -1,    62,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   305,    -1,
      75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,   316,
     317,   318,   319,   320,    89,    22,    -1,    -1,    -1,   326,
     327,   328,   329,   330,   331,   377,    -1,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
      -1,    -1,   394,    -1,    -1,    -1,    -1,   399,   355,   356,
     357,   358,   359,   360,    -1,    -1,    -1,   132,   133,    -1,
      -1,    -1,    -1,    -1,    -1,   372,   373,   374,   375,    -1,
      -1,    -1,    -1,    -1,   149,   150,   151,    -1,    -1,   154,
     387,   388,    -1,    -1,    -1,   392,    -1,    -1,    -1,    -1,
     397,    -1,    -1,    22,    -1,    -1,    -1,   172,    -1,    -1,
      -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,
      -1,   186,    -1,   188,   189,   338,    -1,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,    -1,   208,    -1,   210,   211,    22,   213,    -1,
     215,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   377,   228,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,    -1,   243,   394,
      -1,    -1,    -1,   398,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    -1,    -1,    -1,   269,   270,   271,    -1,    -1,   274,
      -1,    22,    -1,    -1,    -1,   280,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,    -1,    -1,
      64,    -1,    22,    67,    -1,    -1,    -1,    -1,    -1,    -1,
     305,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     315,   316,   317,   318,   319,   320,    -1,    22,    -1,    -1,
      -1,   326,   327,   328,   329,   330,   331,    -1,   377,    -1,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,    -1,    -1,   394,    -1,    -1,   122,   398,
     355,   356,   357,   358,   359,   360,    -1,    -1,   132,   133,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   372,   373,   374,
     375,    -1,    -1,    -1,    -1,   149,   150,    -1,    -1,    -1,
     154,    -1,   387,   388,   158,    -1,    -1,   392,    -1,    -1,
     164,    -1,   397,   167,    -1,    -1,    -1,   171,   172,    -1,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   186,   377,   188,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,   391,    -1,    -1,
     394,    -1,    -1,    -1,   208,   399,    -1,    -1,    -1,   213,
     377,    -1,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,    -1,    -1,   394,    -1,    -1,
      -1,    -1,   399,    -1,    -1,    -1,    -1,    -1,    -1,   243,
      -1,    -1,    -1,    -1,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   269,   270,   271,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    50,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   290,   291,   377,    -1,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,    -1,    -1,   394,    -1,    -1,    -1,   313,
     399,   315,   316,   317,   318,   319,   320,    22,    -1,    -1,
      -1,    -1,   326,   327,   328,   329,   330,   331,   332,   103,
      -1,    -1,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,    -1,    -1,   394,
      -1,   355,   356,   357,   358,   359,   360,    -1,   132,   133,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   372,   373,
     374,   375,    -1,    -1,    -1,   149,   150,   151,    -1,   153,
     154,    -1,    -1,   387,   388,    -1,    -1,    -1,   392,    -1,
     164,   165,    -1,   397,    -1,    -1,   377,    -1,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,    -1,   186,   394,   188,    -1,    -1,   377,   399,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,    -1,    -1,   394,    -1,    -1,    -1,    -1,   399,
      -1,    -1,   377,   217,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,    -1,    -1,   394,
      -1,    -1,    -1,    -1,   399,    -1,    -1,    -1,    -1,   243,
      52,    53,    54,    -1,    56,    -1,    -1,   251,    -1,    61,
     254,    -1,    -1,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,   268,   269,   270,   271,    -1,    -1,
     274,    18,    19,    20,    21,    -1,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,   291,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,   306,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   315,   316,   317,   318,   319,   320,    22,    -1,    -1,
      67,    -1,   326,   327,   328,   329,   330,   331,    75,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   355,   356,   357,   358,   359,   360,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   372,   373,
     374,   375,    -1,    -1,    -1,   122,    -1,    -1,    -1,    22,
      -1,    -1,    -1,   387,   388,   132,   133,    -1,   392,    -1,
      -1,    -1,    -1,   397,    -1,   207,    -1,    -1,    -1,    -1,
      -1,    -1,   149,   150,    -1,    -1,    -1,   154,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,   172,    -1,    -1,   175,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,
      -1,   188,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,    -1,    -1,   394,
      -1,   208,    18,    19,    20,    21,   213,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    -1,   243,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   256,
      66,    67,    -1,    22,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    -1,   269,   270,   271,    -1,   338,    -1,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,    -1,   291,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,   316,
     317,   318,   319,   320,    22,    -1,   132,   133,    -1,   326,
     327,   328,   329,   330,   331,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,   150,    -1,    -1,    -1,   154,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   355,   356,
     357,   358,   359,   360,    22,   171,   172,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   372,   373,   374,   375,    -1,
     186,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     387,   388,    -1,    -1,    -1,   392,    -1,    -1,    -1,    -1,
     397,   377,    -1,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   391,   222,    -1,   394,    -1,
      -1,    -1,   377,   399,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   243,    -1,   394,
      -1,    -1,    -1,    -1,   399,    -1,    -1,    18,    19,    20,
      21,    -1,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,   269,   270,   271,    -1,    -1,   274,   275,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,   377,   291,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,    -1,
      -1,   394,    -1,    -1,    75,    -1,   399,    22,    -1,   315,
     316,   317,   318,   319,   320,    -1,    -1,    88,    -1,    -1,
     326,   327,   328,   329,   330,   331,   377,    -1,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,    -1,    -1,   394,   115,    -1,    -1,    -1,   399,   355,
     356,   357,   358,   359,   360,    22,    -1,    -1,    -1,    -1,
      -1,   132,   133,    -1,    -1,    -1,   372,   373,   374,   375,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
      -1,   387,   388,   154,    -1,    -1,   392,    -1,    -1,    -1,
      -1,   397,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     171,   172,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   186,    -1,   188,   377,    -1,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,    -1,    -1,   394,    -1,   208,    18,    19,
      20,    21,   213,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,   377,    -1,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,    -1,
      50,   394,   243,    -1,    -1,    -1,   399,    -1,    -1,   377,
      -1,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,    -1,    75,   394,    -1,   269,   270,
     271,   399,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   377,
     291,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,    -1,   115,   394,    -1,    -1,    -1,
      -1,   399,    -1,    -1,   315,   316,   317,   318,   319,   320,
      -1,    -1,   132,   133,    -1,   326,   327,   328,   329,   330,
     331,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,
     150,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   355,   356,   357,   358,   359,   360,
      -1,   171,   172,    -1,    -1,   175,    -1,    -1,    -1,    -1,
      -1,   372,   373,   374,   375,    -1,   186,    -1,   188,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   387,   388,    -1,    -1,
      -1,   392,    -1,    -1,    -1,    -1,   397,    -1,   208,    18,
      19,    20,    21,   213,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,
      -1,    50,   377,   243,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,    -1,    -1,   394,
      -1,    -1,    -1,    -1,   399,    -1,    75,    -1,    -1,   269,
     270,   271,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     377,   291,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,    -1,    -1,   394,    -1,    -1,
      -1,    -1,   399,    -1,    -1,   315,   316,   317,   318,   319,
     320,    -1,    -1,   132,   133,    -1,   326,   327,   328,   329,
     330,   331,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     149,   150,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   355,   356,   357,   358,   359,
     360,    -1,   171,   172,    -1,    -1,   175,    -1,    -1,    -1,
      -1,    -1,   372,   373,   374,   375,    -1,   186,    -1,   188,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   387,   388,    -1,
      -1,    -1,   392,    -1,    -1,    -1,    -1,   397,    -1,   208,
      18,    19,    20,    21,   213,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    -1,   243,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,
     269,   270,   271,    -1,    22,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   291,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,   315,   316,   317,   318,
     319,   320,    -1,    -1,   132,   133,    -1,   326,   327,   328,
     329,   330,   331,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,    -1,    -1,    -1,   154,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   355,   356,   357,   358,
     359,   360,    -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   372,   373,   374,   375,    -1,   186,    -1,
     188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   387,   388,
      -1,    -1,    -1,   392,    -1,    -1,    -1,    -1,   397,    -1,
     208,    18,    19,    20,    21,   213,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    50,    -1,   243,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,   256,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      -1,   269,   270,   271,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   291,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,   316,   317,
     318,   319,   320,    -1,    -1,   132,   133,   134,   326,   327,
     328,   329,   330,   331,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,   150,    -1,    -1,    -1,   154,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   355,   356,   357,
     358,   359,   360,    -1,   171,   172,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   372,   373,   374,   375,    -1,   186,
      -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   387,
     388,    -1,    -1,    -1,   392,    -1,    -1,    -1,    -1,   397,
      -1,   208,    18,    19,    20,    21,   213,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,   377,    -1,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,    -1,    50,   394,   243,    -1,    -1,   377,
     399,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,    -1,    -1,   394,    -1,    -1,    75,
      -1,   399,   269,   270,   271,    -1,   377,   274,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,    -1,    -1,   394,   291,    -1,    -1,    -1,   399,    -1,
      -1,   298,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,   316,
     317,   318,   319,   320,    -1,    -1,   132,   133,   134,   326,
     327,   328,   329,   330,   331,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   149,   150,    -1,    -1,    -1,   154,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   355,   356,
     357,   358,   359,   360,    -1,   171,   172,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   372,   373,   374,   375,    -1,
     186,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     387,   388,    -1,    -1,    -1,   392,    -1,    -1,    -1,    -1,
     397,    -1,   208,    18,    19,    20,    21,   213,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,   377,
      -1,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,    22,    50,   394,   243,    -1,    -1,
     377,   399,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,    -1,    -1,   394,    -1,    -1,
      75,    -1,   399,   269,   270,   271,    -1,   377,   274,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,    -1,    -1,   394,   291,    -1,    -1,    -1,   399,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,
     316,   317,   318,   319,   320,    -1,    -1,   132,   133,    -1,
     326,   327,   328,   329,   330,   331,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,   149,   150,    -1,    -1,    -1,   154,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   355,
     356,   357,   358,   359,   360,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   372,   373,   374,   375,
      -1,   186,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   387,   388,    -1,    -1,    -1,   392,    -1,    -1,    -1,
      -1,   397,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   222,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   243,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   259,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   269,   270,   271,   377,    -1,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,    -1,    -1,   394,    -1,   291,    -1,    -1,   399,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     305,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     315,   316,   317,   318,   319,   320,    -1,   322,    -1,    -1,
      -1,   326,   327,   328,   329,   330,   331,    -1,    18,    19,
      20,    21,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     355,   356,   357,   358,   359,   360,    46,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,    -1,   372,   373,   374,
     375,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   387,   388,    -1,    -1,    -1,   392,    -1,    -1,
      -1,    -1,   397,    -1,    -1,    -1,    -1,    -1,    -1,   377,
      -1,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,    -1,    -1,   394,    -1,    -1,    -1,
      -1,   399,    -1,    18,    19,    20,    21,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
      -1,    -1,   132,   133,    -1,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,   149,
     150,    -1,    -1,   377,   154,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,   391,    22,    -1,
     394,    -1,    -1,    -1,    -1,   399,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   186,   377,   188,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,    -1,    -1,   394,    -1,   206,    -1,    -1,   399,
      -1,    -1,    -1,    -1,    -1,    -1,   216,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   243,   149,   150,    -1,    -1,    -1,   154,
     377,    -1,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,    22,    -1,   394,    -1,   269,
     270,   271,   399,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     280,   186,    -1,   188,    -1,    -1,    -1,    -1,    22,    -1,
     377,   291,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,    -1,    -1,   394,    -1,    -1,
      -1,    22,   399,    -1,    -1,   315,   316,   317,   318,   319,
     320,    -1,    -1,    -1,    -1,    -1,   326,   327,   328,   329,
     330,   331,    -1,    -1,    -1,    -1,    -1,   377,   243,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,    -1,    -1,   394,   355,   356,   357,   358,   359,
     360,    -1,    -1,    -1,   269,   270,   271,    -1,    -1,    -1,
      -1,    -1,   372,   373,   374,   375,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   291,   387,   388,    -1,
      -1,    -1,   392,    -1,    -1,    -1,    -1,   397,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     315,   316,   317,   318,   319,   320,    -1,    -1,    -1,    -1,
      -1,   326,   327,   328,   329,   330,   331,    -1,    -1,    -1,
      -1,   336,   337,    18,    19,    20,    21,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
     355,   356,   357,   358,   359,   360,    -1,    -1,    -1,   364,
     365,    -1,    -1,    -1,    -1,    50,    -1,   372,   373,   374,
     375,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   387,   388,    -1,    -1,    -1,   392,    -1,    -1,
      -1,   377,   397,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   391,    -1,    -1,   394,    -1,
      -1,    -1,    -1,   399,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   377,    -1,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,   391,    -1,    -1,
     394,    -1,    -1,    -1,    -1,   399,    -1,   132,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,   150,    -1,    -1,    -1,   154,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,
      19,    20,    21,    -1,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   186,    22,   188,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     377,    -1,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,    -1,    -1,   394,    -1,    -1,
      -1,    -1,   399,   377,    -1,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,   391,   243,    -1,
     394,    -1,    -1,    -1,    -1,   399,   377,    -1,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,    -1,    -1,   394,   269,   270,   271,    -1,   399,    -1,
      -1,    -1,    -1,   132,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,    -1,    -1,
     149,   150,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   164,    -1,    22,   167,    -1,
     315,   316,   317,   318,   319,   320,    -1,    -1,    -1,    -1,
      -1,   326,   327,   328,   329,   330,   331,   186,    -1,   188,
      -1,   336,   337,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     355,   356,   357,   358,   359,   360,    -1,    -1,    -1,   364,
     365,    -1,    -1,    -1,    -1,    -1,    -1,   372,   373,   374,
     375,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   387,   388,   243,    -1,    -1,   392,    18,    19,
      20,    21,   397,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     269,   270,   271,    -1,    -1,   274,    -1,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   291,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   305,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   315,   316,   317,   318,
     319,   320,    -1,    -1,    -1,    -1,    -1,   326,   327,   328,
     329,   330,   331,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      18,    19,    20,    21,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,   355,   356,   357,   358,
     359,   360,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,   372,   373,   374,   375,    -1,    -1,   149,
     150,    -1,    -1,    -1,   154,    -1,    -1,    -1,   387,   388,
      -1,    -1,    -1,   392,    -1,    -1,    -1,   377,   397,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,    -1,    -1,   394,    -1,   186,   187,   188,   399,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    19,
      20,    21,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,   132,   133,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,   149,   150,   243,    -1,    -1,   154,    -1,    -1,   249,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   258,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   269,
     270,   271,    -1,    -1,    -1,    -1,    -1,    -1,   186,   187,
     188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   291,   377,    -1,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,    -1,    -1,   394,
      -1,    -1,    22,    -1,   399,   315,   316,   317,   318,   319,
     320,    -1,   132,   133,    -1,    -1,   326,   327,   328,   329,
     330,   331,    -1,    -1,    -1,   243,    -1,    22,    -1,   149,
     150,   249,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,
     258,    -1,    -1,    -1,    -1,   355,   356,   357,   358,   359,
     360,   269,   270,   271,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,   372,   373,   374,   375,   186,    -1,   188,    -1,
      -1,    -1,    -1,   291,    -1,    -1,    -1,   387,   388,    -1,
      -1,    -1,   392,    -1,    -1,    -1,    -1,   397,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,   316,   317,
     318,   319,   320,    -1,    -1,    -1,    -1,    -1,   326,   327,
     328,   329,   330,   331,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   243,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   355,   356,   357,
     358,   359,   360,    -1,    22,    -1,    -1,    -1,    -1,   269,
     270,   271,    -1,    -1,   372,   373,   374,   375,    -1,   279,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   387,
     388,   291,    -1,    -1,   392,    -1,    -1,    -1,    -1,   397,
      18,    19,    20,    21,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,   315,   316,   317,   318,   319,
     320,    -1,    -1,    -1,    -1,    -1,   326,   327,   328,   329,
     330,   331,    50,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   355,   356,   357,   358,   359,
     360,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   372,   373,   374,   375,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   387,   388,    -1,
      -1,    -1,   392,    -1,    -1,    -1,   377,   397,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,    -1,    -1,   394,   132,   133,    -1,    -1,   399,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   149,   150,    -1,   152,    -1,   154,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,    21,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,
     188,    -1,    -1,    -1,    -1,    -1,    -1,   377,    50,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,    -1,    -1,   394,    -1,    -1,    22,    -1,   399,
      -1,    -1,   377,    75,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,    -1,    -1,   394,
      -1,    -1,    -1,    -1,   399,   243,    22,    -1,    -1,    -1,
      -1,    -1,   377,    -1,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,    22,    -1,   394,
      -1,   269,   270,   271,   399,    -1,    -1,    -1,    -1,    -1,
     132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   291,    -1,    -1,    -1,   149,   150,    -1,
      -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,   316,   317,
     318,   319,   320,    -1,    -1,    -1,    -1,    -1,   326,   327,
     328,   329,   330,   331,   186,    -1,   188,    -1,    -1,   377,
      -1,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,    -1,    22,   394,   355,   356,   357,
     358,   359,   360,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     222,    -1,    -1,    -1,   372,   373,   374,   375,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   387,
     388,   243,    -1,    -1,   392,    18,    19,    20,    21,   397,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   269,   270,   271,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   377,   291,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,    -1,    -1,   394,    -1,    -1,    -1,    -1,
     399,    -1,    -1,   315,   316,   317,   318,   319,   320,    -1,
      -1,    -1,    -1,    -1,   326,   327,   328,   329,   330,   331,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   355,   356,   357,   358,   359,   360,   132,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     372,   373,   374,   375,    -1,    -1,   149,   150,    -1,    -1,
      -1,   154,    -1,    -1,    -1,   387,   388,    -1,    -1,    -1,
     392,    18,    19,    20,    21,   397,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   186,    -1,   188,    -1,    22,    -1,    -1,
      -1,    -1,   377,    50,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,    22,    -1,   394,
      -1,    -1,    -1,    -1,   399,    -1,    -1,    -1,    -1,    -1,
      77,   377,    -1,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   391,    -1,    -1,   394,    -1,
     243,    -1,   377,   399,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,    -1,    -1,   394,
      -1,    -1,    -1,    -1,   399,    -1,   269,   270,   271,    -1,
      -1,    -1,    -1,    -1,    -1,   132,   133,   280,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,
      -1,    -1,   149,   150,    -1,    -1,    -1,   154,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,   315,   316,   317,   318,   319,   320,    -1,    -1,
      -1,    -1,    -1,   326,   327,   328,   329,   330,   331,   186,
     377,   188,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,    -1,    -1,   394,    -1,    -1,
      -1,   398,   355,   356,   357,   358,   359,   360,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   372,
     373,   374,   375,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   387,   388,   243,    -1,   245,   392,
      18,    19,    20,    21,   397,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   269,   270,   271,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   291,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,   316,
     317,   318,   319,   320,    -1,    -1,    -1,    -1,    -1,   326,
     327,   328,   329,   330,   331,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    18,    19,    20,    21,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,   355,   356,
     357,   358,   359,   360,   132,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    50,   372,   373,   374,   375,    -1,
      -1,   149,   150,    -1,    -1,    -1,   154,    -1,    -1,    -1,
     387,   388,    -1,    -1,    -1,   392,    -1,    -1,    -1,    -1,
     397,    22,   377,    -1,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,    -1,   186,   394,
     188,    -1,   377,   398,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,    -1,    -1,   394,
      18,    19,    20,    21,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,   132,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,   149,   150,   243,    -1,    -1,   154,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   269,   270,   271,    -1,    -1,    -1,    -1,    -1,    -1,
     186,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   377,   291,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,    -1,    -1,   394,
      -1,    -1,    -1,   398,    22,    -1,    -1,   315,   316,   317,
     318,   319,   320,    -1,   132,   133,    -1,    -1,   326,   327,
     328,   329,   330,   331,    22,    -1,    -1,   243,    -1,    -1,
      -1,   149,   150,    -1,    -1,    -1,   154,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   355,   356,   357,
     358,   359,   360,   269,   270,   271,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   372,   373,   374,   375,   186,    -1,
     188,    -1,    -1,    -1,    -1,   291,    -1,    -1,    -1,   387,
     388,    -1,    -1,    -1,   392,    -1,    -1,    -1,    -1,   397,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,
     316,   317,   318,   319,   320,    -1,    -1,   323,    -1,    -1,
     326,   327,   328,   329,   330,   331,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   243,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   355,
     356,   357,   358,   359,   360,    -1,    22,    -1,    -1,    -1,
      -1,   269,   270,   271,    -1,    -1,   372,   373,   374,   375,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   387,   388,   291,    -1,    -1,   392,    -1,    -1,    -1,
      -1,   397,    18,    19,    20,    21,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,   315,   316,   317,
     318,   319,   320,    -1,    -1,    -1,    -1,    -1,   326,   327,
     328,   329,   330,   331,    50,    -1,   377,    -1,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,    -1,    -1,   394,    -1,    -1,    -1,   355,   356,   357,
     358,   359,   360,    -1,    -1,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   372,   373,   374,   375,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   387,
     388,    -1,    -1,    -1,   392,    -1,    -1,    -1,    -1,   397,
      -1,    -1,    -1,    -1,    -1,    -1,   122,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   132,   133,   377,    -1,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,   149,   150,   394,    -1,    -1,   154,   398,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    18,    19,
      20,    21,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     186,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,   377,
      50,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,    -1,    -1,   394,    -1,    -1,   377,
     398,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,    22,    -1,   394,    -1,    -1,    -1,
     398,    -1,    -1,    -1,    -1,    -1,    -1,   243,    -1,    -1,
      -1,   377,    -1,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   391,    22,    -1,   394,    -1,
      -1,    -1,   398,   269,   270,   271,    -1,    -1,    -1,    -1,
      -1,    -1,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   291,    -1,    -1,    -1,   149,
     150,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,
     316,   317,   318,   319,   320,    -1,    -1,    -1,    -1,    -1,
     326,   327,   328,   329,   330,   331,   186,    -1,   188,    -1,
      -1,   377,    -1,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   391,    -1,    22,   394,   355,
     356,   357,   358,   359,   360,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   372,   373,   374,   375,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   387,   388,   243,    -1,    -1,   392,    18,    19,    20,
      21,   397,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   269,
     270,   271,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     377,   291,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   389,   390,   391,   305,    -1,   394,    -1,    -1,
      22,   398,    -1,    -1,    -1,   315,   316,   317,   318,   319,
     320,    -1,    -1,    -1,    -1,    -1,   326,   327,   328,   329,
     330,   331,   377,    -1,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,    -1,    -1,   394,
      -1,    -1,    -1,   398,    -1,   355,   356,   357,   358,   359,
     360,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   372,   373,   374,   375,    -1,    -1,   149,   150,
      -1,    -1,    -1,   154,    -1,    -1,    -1,   387,   388,    -1,
      -1,    -1,   392,    18,    19,    20,    21,   397,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   186,    -1,   188,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   377,
      -1,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,    22,    -1,   394,    -1,    -1,    -1,
     398,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   377,   243,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   391,    -1,    -1,   394,    -1,
      -1,    -1,   398,    -1,    22,    -1,    -1,    -1,   269,   270,
     271,    -1,    -1,    -1,    -1,    -1,    -1,   132,   133,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     291,    -1,    -1,    -1,   149,   150,    -1,    -1,    -1,   154,
      -1,    -1,    -1,    -1,   305,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,   315,   316,   317,   318,   319,   320,
      -1,    -1,    -1,    -1,    -1,   326,   327,   328,   329,   330,
     331,   186,   377,   188,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,    -1,    -1,   394,
      -1,    -1,    -1,   398,   355,   356,   357,   358,   359,   360,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   372,   373,   374,   375,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   387,   388,   243,    -1,
      -1,   392,    18,    19,    20,    21,   397,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   269,   270,   271,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   377,   291,   379,   380,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     305,    -1,   394,    -1,    -1,    -1,   398,    -1,    -1,    -1,
     315,   316,   317,   318,   319,   320,    -1,    -1,    -1,    -1,
      -1,   326,   327,   328,   329,   330,   331,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
     355,   356,   357,   358,   359,   360,   132,   133,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    50,   372,   373,   374,
     375,    -1,    -1,   149,   150,    -1,    -1,    -1,   154,    22,
      -1,    -1,   387,   388,    -1,    -1,    -1,   392,    -1,    -1,
      -1,    -1,   397,    -1,   377,    -1,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,    22,
     186,   394,   188,    -1,    -1,   398,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   377,
      -1,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,    22,    -1,   394,    -1,   132,   133,
     398,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,   149,   150,   243,    -1,   377,
     154,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,    22,    -1,   394,    -1,    -1,    -1,
     398,    -1,    -1,   269,   270,   271,    -1,    -1,    -1,    -1,
      -1,    -1,   186,    -1,   188,    -1,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   377,   291,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,    22,
      -1,   394,    -1,    -1,    -1,   398,    -1,    -1,    -1,   315,
     316,   317,   318,   319,   320,    -1,    -1,    -1,    -1,    22,
     326,   327,   328,   329,   330,   331,    -1,    -1,    -1,   243,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   355,
     356,   357,   358,   359,   360,   269,   270,   271,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,   372,   373,   374,   375,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,    22,
      -1,   387,   388,    -1,    -1,    -1,   392,    -1,    -1,    -1,
      -1,   397,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,   315,   316,   317,   318,   319,   320,    -1,    -1,    -1,
      -1,    -1,   326,   327,   328,   329,   330,   331,    -1,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,   355,   356,   357,   358,   359,   360,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   372,   373,
     374,   375,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   387,   388,    -1,    -1,    -1,   392,    -1,
      -1,    -1,    -1,   397,   377,    -1,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,    -1,
      -1,   394,    -1,    -1,   377,   398,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,    -1,
      -1,   394,    -1,    -1,    -1,   398,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   377,    -1,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,    -1,
      -1,   394,    -1,    -1,    -1,   398,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   377,
      -1,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,    -1,    -1,   394,    -1,    -1,   377,
     398,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,    -1,    -1,   394,    -1,    -1,   377,
     398,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,    -1,    -1,   394,    -1,    -1,    -1,
     398,   377,    -1,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   391,    -1,    -1,   394,    -1,
      -1,    -1,   398,    -1,   377,    -1,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,    -1,
      -1,   394,    -1,    -1,   377,   398,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,    -1,
      -1,   394,    -1,    -1,   377,   398,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,    -1,
      -1,   394,    -1,    -1,   377,   398,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,    -1,
      -1,   394,    -1,    -1,   377,   398,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,    -1,
      -1,   394,    -1,    -1,   377,   398,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,    -1,
      -1,   394,    -1,    -1,   377,   398,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,    -1,
      -1,   394,    -1,    -1,   377,   398,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,    -1,
      49,   394,    -1,    -1,    -1,   398,    55,   377,    -1,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,    37,    38,   394,    -1,    75,    76,    -1,    -1,
      -1,    -1,    47,    49,    -1,    -1,    -1,    52,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   105,   106,    -1,    75,
      76,   110,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
     106,    -1,   141,    -1,   110,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,   167,    -1,
     169,    -1,   171,   172,   173,   141,   141,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,    -1,    -1,
     166,   167,    -1,   169,    -1,   171,   172,   173,    -1,   208,
      -1,    -1,    -1,    -1,   213,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     229,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   208,   208,    -1,    -1,    -1,   213,   213,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   229,    -1,    78,    -1,    -1,    -1,    82,
      83,    84,    85,    86,    87,    -1,   275,    -1,    91,    92,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   294,   109,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,   305,    -1,    -1,   275,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,   284,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   294,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   305,
     305,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   354,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   334,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   354,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   395,   362,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   236,    -1,    -1,    -1,    -1,    -1,   395,
     395,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   267,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   366,   367,   368,   369
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
     188,   201,   203,   205,   218,   234,   239,   240,   241,   242,
     243,   244,   247,   248,   249,   250,   252,   253,   258,   260,
     261,   262,   263,   264,   265,   269,   270,   271,   276,   278,
     279,   282,   283,   290,   291,   292,   293,   295,   299,   300,
     301,   305,   307,   308,   309,   314,   315,   316,   317,   318,
     319,   320,   322,   323,   325,   326,   327,   328,   329,   330,
     331,   333,   334,   335,   355,   356,   357,   358,   359,   360,
     361,   362,   371,   372,   373,   374,   375,   387,   388,   392,
     397,   404,   405,   406,   407,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   423,   424,   425,   432,   433,   434,
     435,   447,   455,   458,   464,   472,   474,   324,   397,   402,
     167,   397,   397,   397,   397,   397,   397,   397,   397,   397,
     397,    69,   123,   408,   432,   433,   397,   247,   214,   333,
     361,   433,    75,   468,    52,    53,    54,   338,   340,   341,
     342,   343,   344,   345,   346,   347,   348,   349,   350,   351,
     352,   353,   354,   440,    18,    19,    20,    23,    24,    26,
      27,    28,    29,    30,    31,    32,    88,   115,   171,   172,
     175,   208,   213,   243,   372,   373,   374,   388,   392,   397,
     407,   414,   432,   433,   446,   466,   468,   469,   408,    19,
      20,    64,    98,   108,   148,   169,   212,   283,   290,   438,
     408,   433,    67,   122,   222,   272,   294,   465,   466,   467,
     468,   469,    19,    20,   376,   438,   408,   408,   397,   407,
     408,   432,    88,   115,   175,   407,   432,   446,   466,   468,
     469,   397,   125,   237,   301,   322,   431,    71,   121,   270,
     432,   446,    78,    82,    83,    84,    85,    86,    87,    91,
      92,    93,    94,    95,    96,   109,   112,   130,   143,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     236,   267,   366,   367,   368,   369,   451,    51,   115,   305,
     446,   466,   467,   468,   469,   408,   400,   411,    99,   408,
     397,   397,   397,   397,    61,   249,   432,   433,    64,    67,
     122,   140,   158,   164,   167,   175,   290,   313,   332,   357,
     407,   446,   465,   466,   468,   469,   397,    43,    88,   175,
     407,   432,   446,   466,   468,   469,   218,   249,   408,    75,
     122,   397,   433,   129,   397,    51,   103,   151,   153,   164,
     165,   217,   251,   254,   268,   274,   306,   407,   409,   410,
     453,   302,   172,   469,   408,   397,    56,    61,   207,   408,
     440,    18,   169,   305,   338,   397,   446,   466,   468,   469,
     397,   397,   433,   397,   408,   411,   407,   433,   102,   227,
     448,   433,   323,   410,    67,    88,   160,   175,   256,   407,
     432,   446,   465,   468,   469,   274,   408,   465,   468,   469,
     433,   433,   410,    67,    88,   256,   407,   446,   465,   468,
     469,   274,   408,   465,   468,   469,   440,   408,   410,   336,
     337,   364,   365,   407,    64,   108,   169,   290,   432,   433,
     336,   337,   364,   365,   407,   433,   158,   266,   397,   158,
     266,   407,    79,   402,   402,   402,   407,   249,   407,   414,
     415,   407,   415,   407,   415,     0,   396,    22,   377,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   394,   395,   400,   411,    22,   377,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   394,   402,    47,    52,   118,   141,   162,   284,   305,
     395,   435,   446,    49,    55,    76,   105,   106,   110,   115,
     141,   166,   167,   169,   173,   229,   275,   294,   305,   354,
     395,   446,   466,   468,   469,    52,   162,   209,   296,   297,
     305,   426,   446,   397,   397,   397,   363,   446,   407,   408,
     409,   412,   407,   410,   407,   409,   415,   407,   415,   409,
     409,   407,   415,   409,   407,   407,   407,   407,   446,   395,
     408,   395,   302,   410,   446,   204,   333,   339,   361,   397,
     408,   410,   408,   397,   397,   397,   397,   397,   397,   397,
     397,   397,    75,   204,   222,   467,   468,   477,   410,   407,
     314,   323,   454,   407,   407,   407,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,   394,
     399,   411,   302,   274,   279,   408,   410,   410,   408,   274,
     400,   408,   104,   122,   215,   218,   433,   467,   468,   477,
     454,   399,   409,   408,   407,   408,   410,   467,   468,   446,
     410,   409,   407,   408,   433,   407,   415,   433,   433,   265,
     115,   446,   466,   467,   468,   469,   274,   410,   446,   407,
     410,   454,   410,   410,   407,   407,   399,   433,   164,   167,
     274,   305,   410,   454,   399,   446,   302,   410,   410,   407,
     410,   302,   408,   433,   115,   407,   410,   117,   127,   163,
     235,   442,   443,   410,   407,   375,   410,    99,   408,    57,
     152,   410,   408,   408,   448,   408,   448,   408,   448,   407,
     408,   413,   407,    33,    58,   144,   146,   165,   232,   233,
     254,   310,   312,   444,   407,   433,   433,   433,   415,   330,
     355,   457,   410,   407,   274,   408,   410,   454,   410,   399,
     407,   380,   274,   408,   410,   399,   407,   305,   407,   407,
     407,   407,   399,   410,   410,   410,   410,   407,   407,   407,
     407,   399,   114,   124,   446,   177,   178,   206,   246,   445,
     433,   446,   445,   446,   407,   407,   407,   398,   398,    18,
      19,   249,   372,   373,   374,   406,   414,   432,   433,   407,
     407,   415,   407,   415,   407,   415,   407,   415,   407,   415,
     407,   415,   407,   415,   407,   415,   407,   415,   407,   415,
     407,   415,   407,   415,   407,   415,   407,   415,   167,   409,
     402,   407,   415,   407,   415,   407,   415,   407,   415,   407,
     415,   407,   415,   407,   415,   407,   415,   407,   415,   407,
     415,   407,   415,   407,   415,   407,   415,   407,   415,   407,
     415,   407,   415,   305,   140,   124,   237,   305,   338,   446,
     446,   446,   446,    52,    68,   117,   138,   179,   180,   181,
     226,   230,   231,   277,   285,   286,   338,   441,   147,   270,
     271,   315,   316,   318,   319,   326,   327,   328,   329,   426,
      41,    67,   124,   209,   237,   238,   256,   305,   446,   465,
     468,   305,   446,   408,   410,   446,   167,   228,   255,   274,
     305,   446,   466,   467,   468,   469,   470,   259,   305,   322,
     410,   467,   468,   446,   408,   410,   305,   466,   467,   468,
     469,   446,   448,   407,   410,    66,    67,   115,   274,   275,
     410,   466,   467,   468,   469,   440,   407,   147,   167,   455,
     430,   446,   466,   468,   469,    67,   160,   164,   215,   228,
     408,   429,   465,   468,   469,   470,   473,   478,   407,    39,
      41,    67,   124,   125,   164,   209,   228,   237,   238,   273,
     275,   287,   288,   289,   305,   410,   428,   446,   465,   468,
     469,   470,   476,   478,    36,    62,    89,   151,   176,   184,
     186,   188,   189,   210,   211,   217,   228,   274,   280,   305,
     410,   427,   446,   450,   468,   469,   470,   475,   478,   370,
     433,   433,   415,   409,   399,   398,   398,   398,   398,   398,
     399,   399,   399,   399,   399,   399,   399,   399,   399,   407,
     408,   433,   399,   407,   410,   407,   409,   407,   409,   407,
     407,   407,   407,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   407,   407,   407,   407,   407,   407,   433,   407,
     407,   407,   401,   410,   182,   222,   439,   407,    40,   446,
     408,   274,   399,   407,   398,   220,   401,   399,   399,   398,
     399,   322,   446,   467,   468,   407,   408,   407,   399,   407,
     407,   407,   410,   407,   432,   302,   302,   399,   398,   433,
     399,   446,   391,   399,   391,   375,   399,   407,   407,   398,
     408,   408,   408,   399,   399,   398,   399,   399,   398,    44,
      97,   456,   399,   407,   407,   433,   407,   407,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
     436,   407,   407,   408,   408,   399,   411,   395,   378,   378,
     407,   378,   378,    46,   280,   407,   451,   407,   410,   407,
     411,   408,   274,   451,   407,   410,   408,   410,   410,   410,
     399,   407,    62,   206,   216,   471,    72,   446,   466,   469,
     407,    62,    63,   165,   254,   303,   479,   410,   410,    75,
     222,   477,   410,   408,   399,   446,   410,   465,   410,   467,
     468,   407,   410,   399,   274,    66,   165,   254,   449,   219,
     224,   479,    46,   280,   407,   399,   410,   408,   274,   407,
     451,   408,   145,   280,   311,    46,   280,   407,    59,   213,
     410,   408,    34,   202,   206,   452,   410,   410,   305,   407,
     305,   407,    46,   280,   399,   446,   410,   446,   134,   274,
     407,   410,   446,   466,   468,   469,   375,   375,   134,   274,
     298,   407,   410,   446,   466,   468,   469,   407,   407,   257,
     407,   305,   410,    46,   280,   399,   446,   399,   399,   398,
     410,   409,   407,   415,   407,   415,   407,   415,   409,   407,
     407,   407,   407,   410,   399,   399,   399,   399,   399,   399,
     399,   399,   399,   401,   408,   446,   410,   410,   399,   415,
     415,   415,   477,   433,   399,   399,   397,   408,   407,   410,
     415,   410,   410,   407,   407,   410,   249,   415,   407,   129,
     399,   399,   124,   433,   407,   409,   407,   415,   407,   415,
     451,   399,   399,   407,   399,   410,   167,   446,   407,   407,
     407,   399,   407,   407,   399,   298,   298,    46,   280,    46,
     280,   408,   446,   407,   399,   446,   407,   410,   399,   446,
      62,   204,   298,   437,   407,   410,    77,   245,   410,   459,
     399,   398,   398,   398,   398,   398,   398,   398,   399,   399,
     399,   399,   399,   399,   410,   407,   407,   409,   407,   407,
     407,   407,   407,   399,   398,   407,   399,   398,   399,   398,
     407,   407,   410,   399,   399,   398,   399,   399,   398,   399,
     398,   399,   408,   407,   407,   408,   399,   401,   407,   407,
     407,   407,   407,   407,   407,   407,   398,   398,   399,   410,
     407,   415,   407,   415,   407,   407,   407,   407,   399,   399,
     399,   399,   399,   399,   399,   410,   399,   446,    45,   170,
     281,   463,   399,   399,   408,   410,   410,   407,   446,   407,
      35,   399,   399,   410,    60,   135,   136,   204,   221,   304,
     321,   462,   398,   399,   398,   398,   398,   398,   399,   399,
     399,   399,   410,   407,   407,   407,   407,   407,   407,   407,
     399,   399,   407,   410,   398,   398,   399,   398,   399,   408,
     407,   407,   399,   399,   410,   407,   415,   407,   407,   407,
     398,   399,   399,   399,   446,   446,   399,   407,   407,   446,
      17,   128,   223,   460,   399,   398,   398,   399,   399,   399,
     407,   407,   407,   398,   398,   410,   297,   398,   399,   410,
     407,   415,   407,   407,   399,   399,   398,   407,    74,   183,
     225,   461,   399,   398,   398,   399,   398,   399,   407,   407,
     399,   399,   398,   407,   407,   415,   446,   399,   408,   407,
     398,   398,   398,   399,   407,   399,   446,   407,   399,   399,
     446,   407,   398,   399,   446,   398
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
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
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
#line 636 "pars.yacc"
    {
            expr_parsed = TRUE;
            s_result = (yyvsp[(1) - (1)].dval);
        ;}
    break;

  case 4:
#line 640 "pars.yacc"
    {
            vexpr_parsed = TRUE;
            v_result = (yyvsp[(1) - (1)].vrbl);
        ;}
    break;

  case 8:
#line 653 "pars.yacc"
    {;}
    break;

  case 9:
#line 654 "pars.yacc"
    {;}
    break;

  case 10:
#line 655 "pars.yacc"
    {;}
    break;

  case 11:
#line 656 "pars.yacc"
    {;}
    break;

  case 12:
#line 657 "pars.yacc"
    {;}
    break;

  case 13:
#line 658 "pars.yacc"
    {;}
    break;

  case 14:
#line 659 "pars.yacc"
    {;}
    break;

  case 15:
#line 660 "pars.yacc"
    {;}
    break;

  case 16:
#line 661 "pars.yacc"
    {;}
    break;

  case 17:
#line 662 "pars.yacc"
    {;}
    break;

  case 18:
#line 663 "pars.yacc"
    {
	    return 1;
	;}
    break;

  case 19:
#line 670 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(1) - (1)].dval);
	;}
    break;

  case 20:
#line 673 "pars.yacc"
    {
	    (yyval.dval) = *((yyvsp[(1) - (1)].dptr));
	;}
    break;

  case 21:
#line 676 "pars.yacc"
    {
	    (yyval.dval) = nonl_parms[(yyvsp[(1) - (1)].ival)].value;
	;}
    break;

  case 22:
#line 679 "pars.yacc"
    {
	    (yyval.dval) = nonl_parms[(yyvsp[(1) - (1)].ival)].max;
	;}
    break;

  case 23:
#line 682 "pars.yacc"
    {
	    (yyval.dval) = nonl_parms[(yyvsp[(1) - (1)].ival)].min;
	;}
    break;

  case 24:
#line 685 "pars.yacc"
    {
            if ((yyvsp[(2) - (2)].ival) >= (yyvsp[(1) - (2)].vrbl)->length) {
                errmsg("Access beyond array bounds");
                return 1;
            }
            (yyval.dval) = (yyvsp[(1) - (2)].vrbl)->data[(yyvsp[(2) - (2)].ival)];
	;}
    break;

  case 25:
#line 692 "pars.yacc"
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
	;}
    break;

  case 26:
#line 726 "pars.yacc"
    {
	    if ((yyvsp[(3) - (6)].vrbl)->length != (yyvsp[(5) - (6)].vrbl)->length) {
		yyerror("X and Y are of different length");
		return 1;
            } else {
                (yyval.dval) = trapint((yyvsp[(3) - (6)].vrbl)->data, (yyvsp[(5) - (6)].vrbl)->data, NULL, NULL, (yyvsp[(3) - (6)].vrbl)->length);
            }
	;}
    break;

  case 27:
#line 734 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(1) - (3)].vrbl)->length;
	;}
    break;

  case 28:
#line 737 "pars.yacc"
    {
	    (yyval.dval) = getsetlength((yyvsp[(1) - (3)].trgt)->gno, (yyvsp[(1) - (3)].trgt)->setno);
	;}
    break;

  case 29:
#line 740 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(1) - (3)].trgt)->setno;
	;}
    break;

  case 30:
#line 743 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(1) - (3)].ival);
	;}
    break;

  case 31:
#line 747 "pars.yacc"
    {
            (yyval.dval) = ((ParserFnc) (key[(yyvsp[(1) - (1)].ival)].data)) ();
	;}
    break;

  case 32:
#line 751 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(1) - (2)].dval) * ((ParserFnc) (key[(yyvsp[(2) - (2)].ival)].data)) ();
	;}
    break;

  case 33:
#line 755 "pars.yacc"
    {
        (yyval.dval) = (((double)rand())/((double)RAND_MAX));//drand48();
	;}
    break;

  case 34:
#line 759 "pars.yacc"
    {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[(1) - (4)].ival)].data)) ((yyvsp[(3) - (4)].ival));
	;}
    break;

  case 35:
#line 763 "pars.yacc"
    {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[(1) - (4)].ival)].data)) ((yyvsp[(3) - (4)].dval));
	;}
    break;

  case 36:
#line 767 "pars.yacc"
    {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[(1) - (6)].ival)].data)) ((yyvsp[(3) - (6)].ival), (yyvsp[(5) - (6)].dval));
	;}
    break;

  case 37:
#line 771 "pars.yacc"
    {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[(1) - (6)].ival)].data)) ((yyvsp[(3) - (6)].ival), (yyvsp[(5) - (6)].ival));
	;}
    break;

  case 38:
#line 775 "pars.yacc"
    {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[(1) - (6)].ival)].data)) ((yyvsp[(3) - (6)].dval), (yyvsp[(5) - (6)].dval));
	;}
    break;

  case 39:
#line 779 "pars.yacc"
    {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[(1) - (8)].ival)].data)) ((yyvsp[(3) - (8)].ival), (yyvsp[(5) - (8)].ival), (yyvsp[(7) - (8)].dval));
	;}
    break;

  case 40:
#line 783 "pars.yacc"
    {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[(1) - (8)].ival)].data)) ((yyvsp[(3) - (8)].dval), (yyvsp[(5) - (8)].dval), (yyvsp[(7) - (8)].dval));
	;}
    break;

  case 41:
#line 787 "pars.yacc"
    {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[(1) - (10)].ival)].data)) ((yyvsp[(3) - (10)].dval), (yyvsp[(5) - (10)].dval), (yyvsp[(7) - (10)].dval), (yyvsp[(9) - (10)].dval));
	;}
    break;

  case 42:
#line 791 "pars.yacc"
    {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[(1) - (12)].ival)].data)) ((yyvsp[(3) - (12)].dval), (yyvsp[(5) - (12)].dval), (yyvsp[(7) - (12)].dval), (yyvsp[(9) - (12)].dval), (yyvsp[(11) - (12)].dval));
	;}
    break;

  case 43:
#line 795 "pars.yacc"
    {
	    (yyval.dval) = ((ParserFnc) (key[(yyvsp[(1) - (14)].ival)].data)) ((yyvsp[(3) - (14)].dval), (yyvsp[(5) - (14)].dval), (yyvsp[(7) - (14)].dval), (yyvsp[(9) - (14)].dval), (yyvsp[(11) - (14)].dval), (yyvsp[(13) - (14)].dval));
	;}
    break;

  case 44:
#line 798 "pars.yacc"
    {
	    (yyval.dval) = g[(yyvsp[(1) - (3)].ival)].v.xv1;
	;}
    break;

  case 45:
#line 801 "pars.yacc"
    {
	    (yyval.dval) = g[(yyvsp[(1) - (3)].ival)].v.xv2;
	;}
    break;

  case 46:
#line 804 "pars.yacc"
    {
	    (yyval.dval) = g[(yyvsp[(1) - (3)].ival)].v.yv1;
	;}
    break;

  case 47:
#line 807 "pars.yacc"
    {
	    (yyval.dval) = g[(yyvsp[(1) - (3)].ival)].v.yv2;
	;}
    break;

  case 48:
#line 810 "pars.yacc"
    {
	    (yyval.dval) = g[(yyvsp[(1) - (3)].ival)].w.xg1;
	;}
    break;

  case 49:
#line 813 "pars.yacc"
    {
	    (yyval.dval) = g[(yyvsp[(1) - (3)].ival)].w.xg2;
	;}
    break;

  case 50:
#line 816 "pars.yacc"
    {
	    (yyval.dval) = g[(yyvsp[(1) - (3)].ival)].w.yg1;
	;}
    break;

  case 51:
#line 819 "pars.yacc"
    {
	    (yyval.dval) = g[(yyvsp[(1) - (3)].ival)].w.yg2;
	;}
    break;

  case 52:
#line 822 "pars.yacc"
    {
            (yyval.dval) = (yyvsp[(3) - (4)].dval);
	;}
    break;

  case 53:
#line 825 "pars.yacc"
    { /* yr, mo, day */
	    (yyval.dval) = cal_and_time_to_jul((yyvsp[(3) - (8)].ival), (yyvsp[(5) - (8)].ival), (yyvsp[(7) - (8)].ival), 12, 0, 0.0);
	;}
    break;

  case 54:
#line 829 "pars.yacc"
    { /* yr, mo, day, hr, min, sec */
	    (yyval.dval) = cal_and_time_to_jul((yyvsp[(3) - (14)].ival), (yyvsp[(5) - (14)].ival), (yyvsp[(7) - (14)].ival), (yyvsp[(9) - (14)].ival), (yyvsp[(11) - (14)].ival), (yyvsp[(13) - (14)].dval));
	;}
    break;

  case 55:
#line 832 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
            (yyval.dval) = g[whichgraph].v.xv1;
	;}
    break;

  case 56:
#line 839 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    (yyval.dval) = g[whichgraph].v.xv2;
	;}
    break;

  case 57:
#line 846 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    (yyval.dval) = g[whichgraph].v.yv1;
	;}
    break;

  case 58:
#line 853 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    (yyval.dval) = g[whichgraph].v.yv2;
	;}
    break;

  case 59:
#line 860 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    (yyval.dval) = g[whichgraph].w.xg1;
	;}
    break;

  case 60:
#line 867 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    (yyval.dval) = g[whichgraph].w.xg2;
	;}
    break;

  case 61:
#line 874 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    (yyval.dval) = g[whichgraph].w.yg1;
	;}
    break;

  case 62:
#line 881 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    (yyval.dval) = g[whichgraph].w.yg2;
	;}
    break;

  case 63:
#line 888 "pars.yacc"
    {
	    double vx, vy;
            get_page_viewport(&vx, &vy);
            (yyval.dval) = vx;
	;}
    break;

  case 64:
#line 893 "pars.yacc"
    {
	    double vx, vy;
            get_page_viewport(&vx, &vy);
            (yyval.dval) = vy;
	;}
    break;

  case 65:
#line 898 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(2) - (3)].dval);
	;}
    break;

  case 66:
#line 901 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(1) - (3)].dval) + (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 67:
#line 904 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(1) - (3)].dval) - (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 68:
#line 907 "pars.yacc"
    {
	    (yyval.dval) = -(yyvsp[(2) - (2)].dval);
	;}
    break;

  case 69:
#line 910 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(2) - (2)].dval);
	;}
    break;

  case 70:
#line 913 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(1) - (3)].dval) * (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 71:
#line 917 "pars.yacc"
    {
	    if ((yyvsp[(3) - (3)].dval) != 0.0) {
		(yyval.dval) = (yyvsp[(1) - (3)].dval) / (yyvsp[(3) - (3)].dval);
	    } else {
		yyerror("Divide by zero");
		return 1;
	    }
	;}
    break;

  case 72:
#line 925 "pars.yacc"
    {
	    if ((yyvsp[(3) - (3)].dval) != 0.0) {
		(yyval.dval) = fmod((yyvsp[(1) - (3)].dval), (yyvsp[(3) - (3)].dval));
	    } else {
		yyerror("Divide by zero");
		return 1;
	    }
	;}
    break;

  case 73:
#line 933 "pars.yacc"
    {
        if ((yyvsp[(1) - (3)].dval) < 0 && rint_2((yyvsp[(3) - (3)].dval)) != (yyvsp[(3) - (3)].dval)) {
		yyerror("Negative value raised to non-integer power");
		return 1;
            } else if ((yyvsp[(1) - (3)].dval) == 0.0 && (yyvsp[(3) - (3)].dval) <= 0.0) {
		yyerror("Zero raised to non-positive power");
		return 1;
            } else {
                (yyval.dval) = pow((yyvsp[(1) - (3)].dval), (yyvsp[(3) - (3)].dval));
            }
	;}
    break;

  case 74:
#line 944 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(1) - (5)].dval) ? (yyvsp[(3) - (5)].dval) : (yyvsp[(5) - (5)].dval);
	;}
    break;

  case 75:
#line 947 "pars.yacc"
    {
	   (yyval.dval) = ((yyvsp[(1) - (3)].dval) > (yyvsp[(3) - (3)].dval));
	;}
    break;

  case 76:
#line 950 "pars.yacc"
    {
	   (yyval.dval) = ((yyvsp[(1) - (3)].dval) < (yyvsp[(3) - (3)].dval));
	;}
    break;

  case 77:
#line 953 "pars.yacc"
    {
	   (yyval.dval) = ((yyvsp[(1) - (3)].dval) <= (yyvsp[(3) - (3)].dval));
	;}
    break;

  case 78:
#line 956 "pars.yacc"
    {
	   (yyval.dval) = ((yyvsp[(1) - (3)].dval) >= (yyvsp[(3) - (3)].dval));
	;}
    break;

  case 79:
#line 959 "pars.yacc"
    {
	   (yyval.dval) = ((yyvsp[(1) - (3)].dval) == (yyvsp[(3) - (3)].dval));
	;}
    break;

  case 80:
#line 962 "pars.yacc"
    {
	    (yyval.dval) = ((yyvsp[(1) - (3)].dval) != (yyvsp[(3) - (3)].dval));
	;}
    break;

  case 81:
#line 965 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(1) - (3)].dval) && (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 82:
#line 968 "pars.yacc"
    {
	    (yyval.dval) = (yyvsp[(1) - (3)].dval) || (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 83:
#line 971 "pars.yacc"
    {
	    (yyval.dval) = !((yyvsp[(2) - (2)].dval));
	;}
    break;

  case 84:
#line 976 "pars.yacc"
    {
            (yyval.sval) = (yyvsp[(1) - (1)].sval);
	;}
    break;

  case 85:
#line 979 "pars.yacc"
    {
            (yyval.sval) = concat_strings((yyvsp[(1) - (3)].sval), (yyvsp[(3) - (3)].sval));
            xfree((yyvsp[(3) - (3)].sval));
        ;}
    break;

  case 86:
#line 983 "pars.yacc"
    {
            char buf[32];
            set_locale_num(TRUE);
            sprintf(buf, "%g", (yyvsp[(3) - (3)].dval));
            set_locale_num(FALSE);
            (yyval.sval) = concat_strings((yyvsp[(1) - (3)].sval), buf);
        ;}
    break;

  case 87:
#line 992 "pars.yacc"
    {
        int itmp = rint_2((yyvsp[(1) - (1)].dval));
            if (fabs(itmp - (yyvsp[(1) - (1)].dval)) > 1.e-6) {
		yyerror("Non-integer value supplied for integer");
		return 1;
            }
            (yyval.ival) = itmp;
        ;}
    break;

  case 88:
#line 1002 "pars.yacc"
    {
            if ((yyvsp[(1) - (1)].ival) < 0) {
		yyerror("Negative value supplied for non-negative");
		return 1;
            }
            (yyval.ival) = (yyvsp[(1) - (1)].ival);
	;}
    break;

  case 89:
#line 1011 "pars.yacc"
    {
	    int itmp = (yyvsp[(2) - (3)].ival) - index_shift;
            if (itmp < 0) {
		yyerror("Negative index");
		return 1;
            }
            (yyval.ival) = itmp;
	;}
    break;

  case 90:
#line 1021 "pars.yacc"
    {
            (yyval.dval) = (yyvsp[(1) - (1)].dval);
        ;}
    break;

  case 91:
#line 1024 "pars.yacc"
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
        ;}
    break;

  case 92:
#line 1039 "pars.yacc"
    {
            (yyval.dval) = (yyvsp[(1) - (1)].dval);
        ;}
    break;

  case 93:
#line 1042 "pars.yacc"
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
        ;}
    break;

  case 94:
#line 1059 "pars.yacc"
    {
            (yyval.vrbl) = (yyvsp[(1) - (1)].vrbl);
	;}
    break;

  case 95:
#line 1063 "pars.yacc"
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
	;}
    break;

  case 96:
#line 1076 "pars.yacc"
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
	;}
    break;

  case 97:
#line 1092 "pars.yacc"
    {
            (yyval.vrbl) = (yyvsp[(1) - (1)].vrbl);
	;}
    break;

  case 98:
#line 1096 "pars.yacc"
    {
            int start = (yyvsp[(3) - (6)].ival) - index_shift, stop = (yyvsp[(5) - (6)].ival) - index_shift;
            if (start < 0 || stop < start || stop >= (yyvsp[(1) - (6)].vrbl)->length) {
		yyerror("Invalid index range");
            } else {
                int len = stop - start + 1;
            double *ptr = (double*)xmalloc(len*sizeof(double));
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
	;}
    break;

  case 99:
#line 1118 "pars.yacc"
    {
            int len = (yyvsp[(3) - (4)].ival);
            if (len < 1) {
                yyerror("npoints must be > 0");
            } else {
                double *ptr = (double*)allocate_index_data(len);
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
	;}
    break;

  case 100:
#line 1136 "pars.yacc"
    {
            int len = (yyvsp[(7) - (8)].ival);
            if (len < 2) {
                yyerror("npoints must be > 1");
            } else {
                double *ptr = (double*)allocate_mesh((yyvsp[(3) - (8)].dval), (yyvsp[(5) - (8)].dval), len);
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
	;}
    break;

  case 101:
#line 1154 "pars.yacc"
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
        (yyval.vrbl)->data[i] = (((double)rand())/((double)RAND_MAX));//drand48();
	    }
	;}
    break;

  case 102:
#line 1170 "pars.yacc"
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
	;}
    break;

  case 103:
#line 1199 "pars.yacc"
    {
            int i;
            (yyval.vrbl) = &freelist[fcnt++];
            copy_vrbl((yyval.vrbl), (yyvsp[(3) - (4)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            for (i = 1; i < (yyval.vrbl)->length; i++) {
                (yyval.vrbl)->data[i] += (yyval.vrbl)->data[i - 1];
            }
	;}
    break;

  case 104:
#line 1209 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (4)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[(1) - (4)].ival)].data)) ((int) ((yyvsp[(3) - (4)].vrbl)->data[i]));
	    }
	;}
    break;

  case 105:
#line 1219 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (4)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[(1) - (4)].ival)].data)) (((yyvsp[(3) - (4)].vrbl)->data[i]));
	    }
	;}
    break;

  case 106:
#line 1229 "pars.yacc"
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
	;}
    break;

  case 107:
#line 1244 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(5) - (6)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            
	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[(1) - (6)].ival)].data)) ((yyvsp[(3) - (6)].dval), (yyvsp[(5) - (6)].vrbl)->data[i]);
	    }
	;}
    break;

  case 108:
#line 1255 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (6)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            
	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[(1) - (6)].ival)].data)) ((yyvsp[(3) - (6)].vrbl)->data[i], (yyvsp[(5) - (6)].dval));
	    }
	;}
    break;

  case 109:
#line 1266 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(5) - (6)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[(1) - (6)].ival)].data)) ((yyvsp[(3) - (6)].ival), (yyvsp[(5) - (6)].vrbl)->data[i]);
	    }
	;}
    break;

  case 110:
#line 1277 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(7) - (8)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[(1) - (8)].ival)].data)) ((yyvsp[(3) - (8)].ival), (yyvsp[(5) - (8)].ival), (yyvsp[(7) - (8)].vrbl)->data[i]);
	    }
	;}
    break;

  case 111:
#line 1288 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(7) - (8)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[(1) - (8)].ival)].data)) ((yyvsp[(3) - (8)].dval), (yyvsp[(5) - (8)].dval), (yyvsp[(7) - (8)].vrbl)->data[i]);
	    }
	;}
    break;

  case 112:
#line 1299 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(9) - (10)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[(1) - (10)].ival)].data)) ((yyvsp[(3) - (10)].dval), (yyvsp[(5) - (10)].dval), (yyvsp[(7) - (10)].dval), (yyvsp[(9) - (10)].vrbl)->data[i]);
	    }
	;}
    break;

  case 113:
#line 1310 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(11) - (12)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[(1) - (12)].ival)].data)) ((yyvsp[(3) - (12)].dval), (yyvsp[(5) - (12)].dval), (yyvsp[(7) - (12)].dval), (yyvsp[(9) - (12)].dval), (yyvsp[(11) - (12)].vrbl)->data[i]);
	    }
	;}
    break;

  case 114:
#line 1321 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(13) - (14)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((ParserFnc) (key[(yyvsp[(1) - (14)].ival)].data)) ((yyvsp[(3) - (14)].dval), (yyvsp[(5) - (14)].dval), (yyvsp[(7) - (14)].dval), (yyvsp[(9) - (14)].dval), (yyvsp[(11) - (14)].dval), (yyvsp[(13) - (14)].vrbl)->data[i]);
	    }
	;}
    break;

  case 115:
#line 1332 "pars.yacc"
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
	;}
    break;

  case 116:
#line 1347 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].vrbl)->data[i] + (yyvsp[(3) - (3)].dval);
	    }
	;}
    break;

  case 117:
#line 1358 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].dval) + (yyvsp[(3) - (3)].vrbl)->data[i];
	    }
	;}
    break;

  case 118:
#line 1369 "pars.yacc"
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
	;}
    break;

  case 119:
#line 1384 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].vrbl)->data[i] - (yyvsp[(3) - (3)].dval);
	    }
	;}
    break;

  case 120:
#line 1395 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].dval) - (yyvsp[(3) - (3)].vrbl)->data[i];
	    }
	;}
    break;

  case 121:
#line 1406 "pars.yacc"
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
	;}
    break;

  case 122:
#line 1421 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].vrbl)->data[i] * (yyvsp[(3) - (3)].dval);
	    }
	;}
    break;

  case 123:
#line 1432 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].dval) * (yyvsp[(3) - (3)].vrbl)->data[i];
	    }
	;}
    break;

  case 124:
#line 1443 "pars.yacc"
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
	;}
    break;

  case 125:
#line 1462 "pars.yacc"
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
	;}
    break;

  case 126:
#line 1477 "pars.yacc"
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
	;}
    break;

  case 127:
#line 1492 "pars.yacc"
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
	;}
    break;

  case 128:
#line 1512 "pars.yacc"
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
	;}
    break;

  case 129:
#line 1527 "pars.yacc"
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
	;}
    break;

  case 130:
#line 1543 "pars.yacc"
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
            if ((yyvsp[(1) - (3)].vrbl)->data[i] < 0 && rint_2((yyvsp[(3) - (3)].vrbl)->data[i]) != (yyvsp[(3) - (3)].vrbl)->data[i]) {
	            yyerror("Negative value raised to non-integer power");
	            return 1;
                } else if ((yyvsp[(1) - (3)].vrbl)->data[i] == 0.0 && (yyvsp[(3) - (3)].vrbl)->data[i] <= 0.0) {
	            yyerror("Zero raised to non-positive power");
	            return 1;
                } else {
                    (yyval.vrbl)->data[i] = pow((yyvsp[(1) - (3)].vrbl)->data[i], (yyvsp[(3) - (3)].vrbl)->data[i]);
                }
	    }
	;}
    break;

  case 131:
#line 1566 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
            if ((yyvsp[(1) - (3)].vrbl)->data[i] < 0 && rint_2((yyvsp[(3) - (3)].dval)) != (yyvsp[(3) - (3)].dval)) {
	            yyerror("Negative value raised to non-integer power");
	            return 1;
                } else if ((yyvsp[(1) - (3)].vrbl)->data[i] == 0.0 && (yyvsp[(3) - (3)].dval) <= 0.0) {
	            yyerror("Zero raised to non-positive power");
	            return 1;
                } else {
                    (yyval.vrbl)->data[i] = pow((yyvsp[(1) - (3)].vrbl)->data[i], (yyvsp[(3) - (3)].dval));
                }
	    }
	;}
    break;

  case 132:
#line 1585 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
            if ((yyvsp[(1) - (3)].dval) < 0 && rint_2((yyvsp[(3) - (3)].vrbl)->data[i]) != (yyvsp[(3) - (3)].vrbl)->data[i]) {
	            yyerror("Negative value raised to non-integer power");
	            return 1;
                } else if ((yyvsp[(1) - (3)].dval) == 0.0 && (yyvsp[(3) - (3)].vrbl)->data[i] <= 0.0) {
	            yyerror("Zero raised to non-positive power");
	            return 1;
                } else {
                    (yyval.vrbl)->data[i] = pow((yyvsp[(1) - (3)].dval), (yyvsp[(3) - (3)].vrbl)->data[i]);
                }
	    }
	;}
    break;

  case 133:
#line 1604 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (2)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (2)].vrbl)->data[i] * ((ParserFnc) (key[(yyvsp[(2) - (2)].ival)].data)) ();
	    }
	;}
    break;

  case 134:
#line 1613 "pars.yacc"
    {
            int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (5)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            for (i = 0; i < (yyval.vrbl)->length; i++) { 
                (yyval.vrbl)->data[i] = CAST_DBL_TO_BOOL((yyvsp[(1) - (5)].vrbl)->data[i]) ? (yyvsp[(3) - (5)].dval) : (yyvsp[(5) - (5)].dval);
            }
	;}
    break;

  case 135:
#line 1622 "pars.yacc"
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
	;}
    break;

  case 136:
#line 1635 "pars.yacc"
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
	;}
    break;

  case 137:
#line 1648 "pars.yacc"
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
	;}
    break;

  case 138:
#line 1662 "pars.yacc"
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
	;}
    break;

  case 139:
#line 1677 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].vrbl)->data[i] || (yyvsp[(3) - (3)].dval);
	    }
	;}
    break;

  case 140:
#line 1688 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].dval) || (yyvsp[(3) - (3)].vrbl)->data[i];
	    }
	;}
    break;

  case 141:
#line 1699 "pars.yacc"
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
	;}
    break;

  case 142:
#line 1714 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].vrbl)->data[i] && (yyvsp[(3) - (3)].dval);
	    }
	;}
    break;

  case 143:
#line 1725 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = (yyvsp[(1) - (3)].dval) && (yyvsp[(3) - (3)].vrbl)->data[i];
	    }
	;}
    break;

  case 144:
#line 1736 "pars.yacc"
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
	;}
    break;

  case 145:
#line 1751 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].vrbl)->data[i] > (yyvsp[(3) - (3)].dval));
	    }
	;}
    break;

  case 146:
#line 1762 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].dval) > (yyvsp[(3) - (3)].vrbl)->data[i]);
	    }
	;}
    break;

  case 147:
#line 1773 "pars.yacc"
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
	;}
    break;

  case 148:
#line 1788 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].vrbl)->data[i] < (yyvsp[(3) - (3)].dval));
	    }
	;}
    break;

  case 149:
#line 1799 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].dval) < (yyvsp[(3) - (3)].vrbl)->data[i]);
	    }
	;}
    break;

  case 150:
#line 1810 "pars.yacc"
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
	;}
    break;

  case 151:
#line 1825 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].vrbl)->data[i] >= (yyvsp[(3) - (3)].dval));
	    }
	;}
    break;

  case 152:
#line 1836 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].dval) >= (yyvsp[(3) - (3)].vrbl)->data[i]);
	    }
	;}
    break;

  case 153:
#line 1847 "pars.yacc"
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
	;}
    break;

  case 154:
#line 1862 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].vrbl)->data[i] <= (yyvsp[(3) - (3)].dval));
	    }
	;}
    break;

  case 155:
#line 1873 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].dval) <= (yyvsp[(3) - (3)].vrbl)->data[i]);
	    }
	;}
    break;

  case 156:
#line 1884 "pars.yacc"
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
	;}
    break;

  case 157:
#line 1899 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].vrbl)->data[i] == (yyvsp[(3) - (3)].dval));
	    }
	;}
    break;

  case 158:
#line 1910 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].dval) == (yyvsp[(3) - (3)].vrbl)->data[i]);
	    }
	;}
    break;

  case 159:
#line 1921 "pars.yacc"
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
	;}
    break;

  case 160:
#line 1936 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(1) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].vrbl)->data[i] != (yyvsp[(3) - (3)].dval));
	    }
	;}
    break;

  case 161:
#line 1947 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(3) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;

	    for (i = 0; i < (yyval.vrbl)->length; i++) {
		(yyval.vrbl)->data[i] = ((yyvsp[(1) - (3)].dval) != (yyvsp[(3) - (3)].vrbl)->data[i]);
	    }
	;}
    break;

  case 162:
#line 1958 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(2) - (2)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            for (i = 0; i < (yyval.vrbl)->length; i++) { 
                (yyval.vrbl)->data[i] = !(yyvsp[(2) - (2)].vrbl)->data[i];
            }
	;}
    break;

  case 163:
#line 1968 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(2) - (3)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            for (i = 0; i < (yyval.vrbl)->length; i++) { 
                (yyval.vrbl)->data[i] = (yyvsp[(2) - (3)].vrbl)->data[i];
            }
	;}
    break;

  case 164:
#line 1977 "pars.yacc"
    {
	    int i;
            (yyval.vrbl) = &freelist[fcnt++];
	    copy_vrbl((yyval.vrbl), (yyvsp[(2) - (2)].vrbl));
            (yyval.vrbl)->type = GRARR_TMP;
            for (i = 0; i < (yyval.vrbl)->length; i++) { 
                (yyval.vrbl)->data[i] = - (yyvsp[(2) - (2)].vrbl)->data[i];
            }
	;}
    break;

  case 165:
#line 1991 "pars.yacc"
    {
	    *((yyvsp[(1) - (3)].dptr)) = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 166:
#line 1995 "pars.yacc"
    {
	    nonl_parms[(yyvsp[(1) - (3)].ival)].value = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 167:
#line 1999 "pars.yacc"
    {
	    nonl_parms[(yyvsp[(1) - (3)].ival)].max = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 168:
#line 2003 "pars.yacc"
    {
	    nonl_parms[(yyvsp[(1) - (3)].ival)].min = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 169:
#line 2007 "pars.yacc"
    {
	    if ((yyvsp[(2) - (4)].ival) >= (yyvsp[(1) - (4)].vrbl)->length) {
		yyerror("Access beyond array bounds");
		return 1;
            }
            (yyvsp[(1) - (4)].vrbl)->data[(yyvsp[(2) - (4)].ival)] = (yyvsp[(4) - (4)].dval);
	;}
    break;

  case 170:
#line 2018 "pars.yacc"
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
        ;}
    break;

  case 171:
#line 2045 "pars.yacc"
    {
	    int i;
	    if ((yyvsp[(1) - (3)].vrbl)->length != (yyvsp[(3) - (3)].vrbl)->length) {
                errmsg("Left and right vectors are of different lengths");
                return 1;
            }
	    for (i = 0; i < (yyvsp[(1) - (3)].vrbl)->length; i++) {
	        (yyvsp[(1) - (3)].vrbl)->data[i] = (yyvsp[(3) - (3)].vrbl)->data[i];
	    }
	;}
    break;

  case 172:
#line 2056 "pars.yacc"
    {
	    int i;
	    for (i = 0; i < (yyvsp[(1) - (3)].vrbl)->length; i++) {
	        (yyvsp[(1) - (3)].vrbl)->data[i] = (yyvsp[(3) - (3)].dval);
	    }
	;}
    break;

  case 173:
#line 2066 "pars.yacc"
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
        ;}
    break;

  case 174:
#line 2083 "pars.yacc"
    {
	    if (define_parser_arr((yyvsp[(2) - (4)].sval)) == NULL) {
	        yyerror("Adding new symbol failed");
	    }

            xfree((yyvsp[(2) - (4)].sval));
        ;}
    break;

  case 175:
#line 2091 "pars.yacc"
    {
	    grarr *var;
            if ((var = define_parser_arr((yyvsp[(2) - (5)].sval))) == NULL) {
	        yyerror("Adding new symbol failed");
	    } else {
                realloc_vrbl(var, (yyvsp[(4) - (5)].ival));
            }

            xfree((yyvsp[(2) - (5)].sval));
        ;}
    break;

  case 176:
#line 2102 "pars.yacc"
    {
            yyerror("Keyword already exists");
        ;}
    break;

  case 177:
#line 2106 "pars.yacc"
    {
            yyerror("Keyword already exists");
        ;}
    break;

  case 178:
#line 2110 "pars.yacc"
    {
            undefine_parser_var((void *) (yyvsp[(2) - (2)].dptr));
            xfree((yyvsp[(2) - (2)].dptr));
        ;}
    break;

  case 179:
#line 2115 "pars.yacc"
    {
            realloc_vrbl((yyvsp[(2) - (2)].vrbl), 0);
            undefine_parser_var((void *) (yyvsp[(2) - (2)].vrbl));
            xfree((yyvsp[(2) - (2)].vrbl));
        ;}
    break;

  case 180:
#line 2120 "pars.yacc"
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
	;}
    break;

  case 181:
#line 2138 "pars.yacc"
    {
	    alias_force = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 182:
#line 2141 "pars.yacc"
    {
	    if (load_module((yyvsp[(6) - (6)].sval), (yyvsp[(2) - (6)].sval), (yyvsp[(2) - (6)].sval), (yyvsp[(4) - (6)].ival)) != 0) {
	        yyerror("DL module load failed");
	    }
	    xfree((yyvsp[(2) - (6)].sval));
	    xfree((yyvsp[(6) - (6)].sval));
	;}
    break;

  case 183:
#line 2148 "pars.yacc"
    {
	    if (load_module((yyvsp[(6) - (8)].sval), (yyvsp[(2) - (8)].sval), (yyvsp[(8) - (8)].sval), (yyvsp[(4) - (8)].ival)) != 0) {
	        yyerror("DL module load failed");
	    }
	    xfree((yyvsp[(2) - (8)].sval));
	    xfree((yyvsp[(6) - (8)].sval));
	    xfree((yyvsp[(8) - (8)].sval));
	;}
    break;

  case 184:
#line 2159 "pars.yacc"
    {
	    rg[(yyvsp[(1) - (2)].ival)].active = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 185:
#line 2162 "pars.yacc"
    {
	    rg[(yyvsp[(1) - (3)].ival)].type = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 186:
#line 2165 "pars.yacc"
    {
	    rg[(yyvsp[(1) - (2)].ival)].color = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 187:
#line 2168 "pars.yacc"
    {
	    rg[(yyvsp[(1) - (2)].ival)].lines = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 188:
#line 2171 "pars.yacc"
    {
	    rg[(yyvsp[(1) - (2)].ival)].linew = (yyvsp[(2) - (2)].dval);
	;}
    break;

  case 189:
#line 2175 "pars.yacc"
    {
	    rg[(yyvsp[(1) - (9)].ival)].x1 = (yyvsp[(3) - (9)].dval);
	    rg[(yyvsp[(1) - (9)].ival)].y1 = (yyvsp[(5) - (9)].dval);
	    rg[(yyvsp[(1) - (9)].ival)].x2 = (yyvsp[(7) - (9)].dval);
	    rg[(yyvsp[(1) - (9)].ival)].y2 = (yyvsp[(9) - (9)].dval);
	;}
    break;

  case 190:
#line 2182 "pars.yacc"
    {
	    rg[(yyvsp[(1) - (5)].ival)].x = (double*)xrealloc(rg[(yyvsp[(1) - (5)].ival)].x, (rg[(yyvsp[(1) - (5)].ival)].n + 1) * sizeof(double));
	    rg[(yyvsp[(1) - (5)].ival)].y = (double*)xrealloc(rg[(yyvsp[(1) - (5)].ival)].y, (rg[(yyvsp[(1) - (5)].ival)].n + 1) * sizeof(double));
	    rg[(yyvsp[(1) - (5)].ival)].x[rg[(yyvsp[(1) - (5)].ival)].n] = (yyvsp[(3) - (5)].dval);
	    rg[(yyvsp[(1) - (5)].ival)].y[rg[(yyvsp[(1) - (5)].ival)].n] = (yyvsp[(5) - (5)].dval);
	    rg[(yyvsp[(1) - (5)].ival)].n++;
	;}
    break;

  case 191:
#line 2189 "pars.yacc"
    {
	    rg[(yyvsp[(2) - (4)].ival)].linkto = (yyvsp[(4) - (4)].ival);
	;}
    break;

  case 192:
#line 2196 "pars.yacc"
    {
            if (set_project_version((yyvsp[(2) - (2)].ival)) != RETURN_SUCCESS) {
                errmsg("Project version is newer than software!");
            }
            if (get_project_version() < 50001) {
                map_fonts(FONT_MAP_ACEGR);
            } else {
                map_fonts(FONT_MAP_DEFAULT);
            }
        ;}
    break;

  case 193:
#line 2206 "pars.yacc"
    {
            set_page_dimensions((yyvsp[(3) - (5)].ival), (yyvsp[(5) - (5)].ival), TRUE);
        ;}
    break;

  case 194:
#line 2209 "pars.yacc"
    {
            set_page_dimensions((yyvsp[(3) - (5)].ival), (yyvsp[(5) - (5)].ival), FALSE);
        ;}
    break;

  case 195:
#line 2212 "pars.yacc"
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
        ;}
    break;

  case 196:
#line 2227 "pars.yacc"
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
        ;}
    break;

  case 197:
#line 2241 "pars.yacc"
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
        ;}
    break;

  case 198:
#line 2255 "pars.yacc"
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
        ;}
    break;

  case 199:
#line 2269 "pars.yacc"
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
        ;}
    break;

  case 200:
#line 2284 "pars.yacc"
    {
            set_printer_by_name((yyvsp[(3) - (3)].sval));
            xfree((yyvsp[(3) - (3)].sval));
        ;}
    break;

  case 201:
#line 2288 "pars.yacc"
    {
            set_ref_date((yyvsp[(3) - (3)].dval));
	;}
    break;

  case 202:
#line 2291 "pars.yacc"
    {
            allow_two_digits_years((yyvsp[(3) - (3)].ival));
	;}
    break;

  case 203:
#line 2294 "pars.yacc"
    {
            set_wrap_year((yyvsp[(4) - (4)].ival));
	;}
    break;

  case 204:
#line 2297 "pars.yacc"
    {
	    setbgcolor((yyvsp[(2) - (2)].ival));
	;}
    break;

  case 205:
#line 2300 "pars.yacc"
    {
	    setbgfill((yyvsp[(4) - (4)].ival));
	;}
    break;

  case 206:
#line 2303 "pars.yacc"
    {
	    scroll_proc((int) (yyvsp[(3) - (4)].dval));
	;}
    break;

  case 207:
#line 2306 "pars.yacc"
    {
	    scrollinout_proc((int) (yyvsp[(3) - (4)].dval));
	;}
    break;

  case 208:
#line 2309 "pars.yacc"
    {
	    scrolling_islinked = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 209:
#line 2314 "pars.yacc"
    {
	    add_world(whichgraph, (yyvsp[(3) - (9)].dval), (yyvsp[(5) - (9)].dval), (yyvsp[(7) - (9)].dval), (yyvsp[(9) - (9)].dval));
	;}
    break;

  case 210:
#line 2318 "pars.yacc"
    {
            timer_delay = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 211:
#line 2322 "pars.yacc"
    {
	    target_set = *((yyvsp[(2) - (2)].trgt));
	    set_parser_setno(target_set.gno, target_set.setno);
	;}
    break;

  case 212:
#line 2326 "pars.yacc"
    {
	    set_parser_gno((yyvsp[(2) - (2)].ival));
	;}
    break;

  case 213:
#line 2329 "pars.yacc"
    {
	    set_parser_setno((yyvsp[(2) - (2)].trgt)->gno, (yyvsp[(2) - (2)].trgt)->setno);
	;}
    break;

  case 214:
#line 2334 "pars.yacc"
    {
	    set_hotlink((yyvsp[(1) - (4)].trgt)->gno, (yyvsp[(1) - (4)].trgt)->setno, 1, (yyvsp[(4) - (4)].sval), (yyvsp[(3) - (4)].ival));
	    xfree((yyvsp[(4) - (4)].sval));
	;}
    break;

  case 215:
#line 2338 "pars.yacc"
    {
	    set_hotlink((yyvsp[(1) - (3)].trgt)->gno, (yyvsp[(1) - (3)].trgt)->setno, (yyvsp[(3) - (3)].ival), NULL, 0);
	;}
    break;

  case 216:
#line 2343 "pars.yacc"
    {
	    curbox = next_box();
	;}
    break;

  case 217:
#line 2346 "pars.yacc"
    {
            int no = (yyvsp[(3) - (3)].ival);
            if (is_valid_box(no) ||
                realloc_boxes(no + 1) == RETURN_SUCCESS) {
                curbox = no;
            }
	;}
    break;

  case 218:
#line 2353 "pars.yacc"
    {
	    if (!is_valid_box(curbox)) {
                yyerror("Box not active");
	    } else {
	        boxes[curbox].active = (yyvsp[(2) - (2)].ival);
            }
	;}
    break;

  case 219:
#line 2360 "pars.yacc"
    {
	    if (!is_valid_box(curbox)) {
                yyerror("Box not active");
	    } else {
	        boxes[curbox].gno = (yyvsp[(2) - (2)].ival);
            }
	;}
    break;

  case 220:
#line 2367 "pars.yacc"
    {
	    if (!is_valid_box(curbox)) {
                yyerror("Box not active");
	    } else {
		boxes[curbox].x1 = (yyvsp[(2) - (8)].dval);
		boxes[curbox].y1 = (yyvsp[(4) - (8)].dval);
		boxes[curbox].x2 = (yyvsp[(6) - (8)].dval);
		boxes[curbox].y2 = (yyvsp[(8) - (8)].dval);
	    }
	;}
    break;

  case 221:
#line 2377 "pars.yacc"
    {
	    box_loctype = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 222:
#line 2380 "pars.yacc"
    {
	    box_lines = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 223:
#line 2383 "pars.yacc"
    {
	    box_linew = (yyvsp[(2) - (2)].dval);
	;}
    break;

  case 224:
#line 2386 "pars.yacc"
    {
	    box_color = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 225:
#line 2389 "pars.yacc"
    {
	    box_fillcolor = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 226:
#line 2392 "pars.yacc"
    {
	    box_fillpat = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 227:
#line 2395 "pars.yacc"
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
	;}
    break;

  case 228:
#line 2426 "pars.yacc"
    {
		curellipse = next_ellipse();
	;}
    break;

  case 229:
#line 2429 "pars.yacc"
    {
            int no = (yyvsp[(3) - (3)].ival);
            if (is_valid_ellipse(no) ||
                realloc_ellipses(no + 1) == RETURN_SUCCESS) {
                curellipse = no;
            }
	;}
    break;

  case 230:
#line 2436 "pars.yacc"
    {
	    if (!is_valid_ellipse(curellipse)) {
                yyerror("Ellipse not active");
	    } else {
	        ellip[curellipse].active = (yyvsp[(2) - (2)].ival);
            }
	;}
    break;

  case 231:
#line 2443 "pars.yacc"
    {
	    if (!is_valid_ellipse(curellipse)) {
                yyerror("Ellipse not active");
	    } else {
	        ellip[curellipse].gno = (yyvsp[(2) - (2)].ival);
            }
	;}
    break;

  case 232:
#line 2450 "pars.yacc"
    {
	    if (!is_valid_ellipse(curellipse)) {
                yyerror("Ellipse not active");
	    } else {
		ellip[curellipse].x1 = (yyvsp[(2) - (8)].dval);
		ellip[curellipse].y1 = (yyvsp[(4) - (8)].dval);
		ellip[curellipse].x2 = (yyvsp[(6) - (8)].dval);
		ellip[curellipse].y2 = (yyvsp[(8) - (8)].dval);
	    }
	;}
    break;

  case 233:
#line 2460 "pars.yacc"
    {
	    ellipse_loctype = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 234:
#line 2463 "pars.yacc"
    {
	    ellipse_lines = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 235:
#line 2466 "pars.yacc"
    {
	    ellipse_linew = (yyvsp[(2) - (2)].dval);
	;}
    break;

  case 236:
#line 2469 "pars.yacc"
    {
	    ellipse_color = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 237:
#line 2472 "pars.yacc"
    {
	    ellipse_fillcolor = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 238:
#line 2475 "pars.yacc"
    {
	    ellipse_fillpat = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 239:
#line 2478 "pars.yacc"
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
	;}
    break;

  case 240:
#line 2509 "pars.yacc"
    {
	    curline = next_line();
	;}
    break;

  case 241:
#line 2512 "pars.yacc"
    {
            int no = (yyvsp[(3) - (3)].ival);
            if (is_valid_line(no) ||
                realloc_lines(no + 1) == RETURN_SUCCESS) {
                curline = no;
            }
	;}
    break;

  case 242:
#line 2519 "pars.yacc"
    {
	    if (!is_valid_line(curline)) {
                yyerror("Line not active");
	    } else {
	        lines[curline].active = (yyvsp[(2) - (2)].ival);
            }
	;}
    break;

  case 243:
#line 2526 "pars.yacc"
    {
	    if (!is_valid_line(curline)) {
                yyerror("Line not active");
	    } else {
	        lines[curline].gno = (yyvsp[(2) - (2)].ival);
            }
	;}
    break;

  case 244:
#line 2533 "pars.yacc"
    {
	    if (!is_valid_line(curline)) {
                yyerror("Line not active");
	    } else {
	        lines[curline].x1 = (yyvsp[(2) - (8)].dval);
	        lines[curline].y1 = (yyvsp[(4) - (8)].dval);
	        lines[curline].x2 = (yyvsp[(6) - (8)].dval);
	        lines[curline].y2 = (yyvsp[(8) - (8)].dval);
            }
	;}
    break;

  case 245:
#line 2543 "pars.yacc"
    {
	    line_loctype = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 246:
#line 2546 "pars.yacc"
    {
	    line_linew = (yyvsp[(2) - (2)].dval);
	;}
    break;

  case 247:
#line 2549 "pars.yacc"
    {
	    line_lines = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 248:
#line 2552 "pars.yacc"
    {
	    line_color = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 249:
#line 2555 "pars.yacc"
    {
	    line_arrow_end = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 250:
#line 2558 "pars.yacc"
    {
	    line_asize = (yyvsp[(4) - (4)].dval);
	;}
    break;

  case 251:
#line 2561 "pars.yacc"
    {
	    line_atype = (yyvsp[(4) - (4)].ival);
	;}
    break;

  case 252:
#line 2564 "pars.yacc"
    {
	    line_a_dL_ff = (yyvsp[(4) - (6)].dval);
	    line_a_lL_ff = (yyvsp[(6) - (6)].dval);
	;}
    break;

  case 253:
#line 2568 "pars.yacc"
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
	;}
    break;

  case 254:
#line 2585 "pars.yacc"
    {
            curstring = next_string();
        ;}
    break;

  case 255:
#line 2588 "pars.yacc"
    {
            int no = (yyvsp[(3) - (3)].ival);
            if (is_valid_string(no) ||
                realloc_strings(no + 1) == RETURN_SUCCESS) {
                curstring = no;
            }
        ;}
    break;

  case 256:
#line 2595 "pars.yacc"
    {
	    if (!is_valid_string(curstring)) {
                yyerror("String not active");
	    } else {
                pstr[curstring].active = (yyvsp[(2) - (2)].ival);
            }
        ;}
    break;

  case 257:
#line 2602 "pars.yacc"
    {
	    if (!is_valid_string(curstring)) {
                yyerror("String not active");
	    } else {
                pstr[curstring].gno = (yyvsp[(2) - (2)].ival);
            }
        ;}
    break;

  case 258:
#line 2609 "pars.yacc"
    {
	    if (!is_valid_string(curstring)) {
                yyerror("String not active");
	    } else {
	        pstr[curstring].x = (yyvsp[(2) - (4)].dval);
	        pstr[curstring].y = (yyvsp[(4) - (4)].dval);
            }
	;}
    break;

  case 259:
#line 2617 "pars.yacc"
    {
            string_loctype = (yyvsp[(3) - (3)].ival);
        ;}
    break;

  case 260:
#line 2620 "pars.yacc"
    {
            string_color = (yyvsp[(2) - (2)].ival);
        ;}
    break;

  case 261:
#line 2623 "pars.yacc"
    {
            string_rot = (yyvsp[(3) - (3)].ival);
        ;}
    break;

  case 262:
#line 2626 "pars.yacc"
    {
            string_font = (yyvsp[(2) - (2)].ival);
        ;}
    break;

  case 263:
#line 2629 "pars.yacc"
    {
            string_just = (yyvsp[(3) - (3)].ival);
        ;}
    break;

  case 264:
#line 2632 "pars.yacc"
    {
            string_size = (yyvsp[(4) - (4)].dval);
        ;}
    break;

  case 265:
#line 2635 "pars.yacc"
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
	;}
    break;

  case 266:
#line 2651 "pars.yacc"
    {
            timestamp.active = (yyvsp[(2) - (2)].ival);
        ;}
    break;

  case 267:
#line 2654 "pars.yacc"
    {
            timestamp.font = (yyvsp[(2) - (2)].ival);
        ;}
    break;

  case 268:
#line 2657 "pars.yacc"
    {
            timestamp.charsize = (yyvsp[(4) - (4)].dval);
        ;}
    break;

  case 269:
#line 2660 "pars.yacc"
    {
            timestamp.rot = (yyvsp[(3) - (3)].ival);
        ;}
    break;

  case 270:
#line 2663 "pars.yacc"
    {
            timestamp.color = (yyvsp[(2) - (2)].ival);
        ;}
    break;

  case 271:
#line 2666 "pars.yacc"
    {
	    timestamp.x = (yyvsp[(2) - (4)].dval);
	    timestamp.y = (yyvsp[(4) - (4)].dval);
	;}
    break;

  case 272:
#line 2670 "pars.yacc"
    {
	  set_plotstr_string(&timestamp, (yyvsp[(3) - (3)].sval));
	  xfree((yyvsp[(3) - (3)].sval));
	;}
    break;

  case 273:
#line 2676 "pars.yacc"
    {
	    grdefaults.lines = (yyvsp[(2) - (2)].ival);
	    box_lines = ellipse_lines = line_lines = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 274:
#line 2680 "pars.yacc"
    {
	    grdefaults.linew = (yyvsp[(2) - (2)].dval);
	    box_linew = ellipse_linew = line_linew = (yyvsp[(2) - (2)].dval);
	;}
    break;

  case 275:
#line 2684 "pars.yacc"
    {
	    grdefaults.color = (yyvsp[(2) - (2)].ival);
	    box_color = ellipse_color = line_color = string_color = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 276:
#line 2688 "pars.yacc"
    {
	    grdefaults.pattern = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 277:
#line 2691 "pars.yacc"
    {
	    grdefaults.charsize = (yyvsp[(4) - (4)].dval);
	    string_size = (yyvsp[(4) - (4)].dval);
	;}
    break;

  case 278:
#line 2695 "pars.yacc"
    {
	    grdefaults.font = (yyvsp[(2) - (2)].ival);
	    string_font = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 279:
#line 2699 "pars.yacc"
    {
	    grdefaults.symsize = (yyvsp[(4) - (4)].dval);
	;}
    break;

  case 280:
#line 2702 "pars.yacc"
    {
	    strcpy(sformat, (yyvsp[(3) - (3)].sval));
	    xfree((yyvsp[(3) - (3)].sval));
	;}
    break;

  case 281:
#line 2706 "pars.yacc"
    {
	    if ((map_font_by_name((yyvsp[(5) - (7)].sval), (yyvsp[(3) - (7)].ival)) != RETURN_SUCCESS) && 
                (map_font_by_name((yyvsp[(7) - (7)].sval), (yyvsp[(3) - (7)].ival)) != RETURN_SUCCESS)) {
                errmsg("Failed mapping a font");
            }
            xfree((yyvsp[(5) - (7)].sval));
	    xfree((yyvsp[(7) - (7)].sval));
	;}
    break;

  case 282:
#line 2714 "pars.yacc"
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
        ;}
    break;

  case 283:
#line 2727 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].w.xg1 = (yyvsp[(2) - (8)].dval);
	    g[whichgraph].w.yg1 = (yyvsp[(4) - (8)].dval);
	    g[whichgraph].w.xg2 = (yyvsp[(6) - (8)].dval);
	    g[whichgraph].w.yg2 = (yyvsp[(8) - (8)].dval);
	;}
    break;

  case 284:
#line 2737 "pars.yacc"
    {
	    set_graph_znorm(whichgraph, (yyvsp[(2) - (2)].dval));
	;}
    break;

  case 285:
#line 2740 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].v.xv1 = (yyvsp[(2) - (8)].dval);
	    g[whichgraph].v.yv1 = (yyvsp[(4) - (8)].dval);
	    g[whichgraph].v.xv2 = (yyvsp[(6) - (8)].dval);
	    g[whichgraph].v.yv2 = (yyvsp[(8) - (8)].dval);
	;}
    break;

  case 286:
#line 2750 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    set_plotstr_string(&g[whichgraph].labs.title, (yyvsp[(2) - (2)].sval));
	    xfree((yyvsp[(2) - (2)].sval));
	;}
    break;

  case 287:
#line 2758 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].labs.title.font = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 288:
#line 2765 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].labs.title.charsize = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 289:
#line 2772 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].labs.title.color = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 290:
#line 2779 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    set_plotstr_string(&g[whichgraph].labs.stitle, (yyvsp[(2) - (2)].sval));
	    xfree((yyvsp[(2) - (2)].sval));
	;}
    break;

  case 291:
#line 2787 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].labs.stitle.font = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 292:
#line 2794 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].labs.stitle.charsize = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 293:
#line 2801 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].labs.stitle.color = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 294:
#line 2809 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].xscale = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 295:
#line 2816 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].yscale = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 296:
#line 2823 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].xinvert = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 297:
#line 2830 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].yinvert = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 298:
#line 2837 "pars.yacc"
    {
            autoscale_onread = AUTOSCALE_NONE;
        ;}
    break;

  case 299:
#line 2840 "pars.yacc"
    {
            autoscale_onread = AUTOSCALE_X;
        ;}
    break;

  case 300:
#line 2843 "pars.yacc"
    {
            autoscale_onread = AUTOSCALE_Y;
        ;}
    break;

  case 301:
#line 2846 "pars.yacc"
    {
            autoscale_onread = AUTOSCALE_XY;
        ;}
    break;

  case 302:
#line 2850 "pars.yacc"
    {
            char *s;
            s = copy_string(NULL, get_project_description());
            s = concat_strings(s, (yyvsp[(2) - (2)].sval));
	    xfree((yyvsp[(2) - (2)].sval));
            s = concat_strings(s, "\n");
            set_project_description(s);
            xfree(s);
	;}
    break;

  case 303:
#line 2859 "pars.yacc"
    {
            set_project_description(NULL);
        ;}
    break;

  case 304:
#line 2863 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.active = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 305:
#line 2870 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.loctype = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 306:
#line 2877 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
            g[whichgraph].l.vgap = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 307:
#line 2884 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.hgap = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 308:
#line 2891 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.len = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 309:
#line 2898 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.invert = (yyvsp[(3) - (3)].ival);
        ;}
    break;

  case 310:
#line 2905 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.boxfillpen.color = (yyvsp[(4) - (4)].ival);
        ;}
    break;

  case 311:
#line 2912 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.boxfillpen.pattern = (yyvsp[(4) - (4)].ival);
        ;}
    break;

  case 312:
#line 2919 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.boxpen.color = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 313:
#line 2926 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.boxpen.pattern = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 314:
#line 2933 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.boxlines = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 315:
#line 2940 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.boxlinew = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 316:
#line 2947 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.legx = (yyvsp[(2) - (4)].dval);
	    g[whichgraph].l.legy = (yyvsp[(4) - (4)].dval);
	;}
    break;

  case 317:
#line 2955 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.charsize = (yyvsp[(4) - (4)].dval);
	;}
    break;

  case 318:
#line 2962 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.font = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 319:
#line 2969 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.color = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 320:
#line 2977 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
            g[whichgraph].f.pen.pattern = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 321:
#line 2984 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].f.type = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 322:
#line 2991 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].f.lines = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 323:
#line 2998 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].f.linew = (yyvsp[(2) - (2)].dval);
	;}
    break;

  case 324:
#line 3005 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].f.pen.color = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 325:
#line 3012 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].f.pen.pattern = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 326:
#line 3020 "pars.yacc"
    { 
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
            g[whichgraph].f.fillpen.color = (yyvsp[(3) - (3)].ival);
        ;}
    break;

  case 327:
#line 3028 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
            g[whichgraph].f.fillpen.pattern = (yyvsp[(3) - (3)].ival);
        ;}
    break;

  case 328:
#line 3036 "pars.yacc"
    {
            set_graph_hidden((yyvsp[(1) - (2)].ival), !(yyvsp[(2) - (2)].ival));
        ;}
    break;

  case 329:
#line 3039 "pars.yacc"
    {
            set_graph_hidden((yyvsp[(1) - (3)].ival), (yyvsp[(3) - (3)].ival));
        ;}
    break;

  case 330:
#line 3042 "pars.yacc"
    {
            set_graph_type((yyvsp[(1) - (3)].ival), (yyvsp[(3) - (3)].ival));
        ;}
    break;

  case 331:
#line 3045 "pars.yacc"
    {
            set_graph_stacked((yyvsp[(1) - (3)].ival), (yyvsp[(3) - (3)].ival));
        ;}
    break;

  case 332:
#line 3049 "pars.yacc"
    {
	    set_graph_bargap((yyvsp[(1) - (4)].ival), (yyvsp[(4) - (4)].dval));
	;}
    break;

  case 333:
#line 3053 "pars.yacc"
    {
            g[(yyvsp[(1) - (3)].ival)].locator.pointset = (yyvsp[(3) - (3)].ival);
        ;}
    break;

  case 334:
#line 3056 "pars.yacc"
    {
	    g[(yyvsp[(1) - (5)].ival)].locator.fx = (yyvsp[(4) - (5)].ival);
	    g[(yyvsp[(1) - (5)].ival)].locator.fy = (yyvsp[(5) - (5)].ival);
	;}
    break;

  case 335:
#line 3060 "pars.yacc"
    {
	    g[(yyvsp[(1) - (6)].ival)].locator.px = (yyvsp[(4) - (6)].dval);
	    g[(yyvsp[(1) - (6)].ival)].locator.py = (yyvsp[(6) - (6)].dval);
	;}
    break;

  case 336:
#line 3064 "pars.yacc"
    {
	    g[(yyvsp[(1) - (6)].ival)].locator.dsx = (yyvsp[(4) - (6)].dval);
	    g[(yyvsp[(1) - (6)].ival)].locator.dsy = (yyvsp[(6) - (6)].dval);
	;}
    break;

  case 337:
#line 3068 "pars.yacc"
    {
            g[(yyvsp[(1) - (4)].ival)].locator.pt_type = (yyvsp[(4) - (4)].ival);
        ;}
    break;

  case 338:
#line 3072 "pars.yacc"
    {
	    curtype = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 339:
#line 3077 "pars.yacc"
    {
	    if (add_io_filter((yyvsp[(2) - (5)].ival), (yyvsp[(4) - (5)].ival), (yyvsp[(5) - (5)].sval), (yyvsp[(3) - (5)].sval)) != 0) {
	        yyerror("Failed adding i/o filter");
	    }
	    xfree((yyvsp[(3) - (5)].sval));
	    xfree((yyvsp[(5) - (5)].sval));
	;}
    break;

  case 340:
#line 3084 "pars.yacc"
    {
	    clear_io_filters((yyvsp[(2) - (2)].ival));
	;}
    break;

  case 341:
#line 3088 "pars.yacc"
    {
	    cursource = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 342:
#line 3091 "pars.yacc"
    {
	    readxformat = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 343:
#line 3094 "pars.yacc"
    { ;}
    break;

  case 344:
#line 3095 "pars.yacc"
    {
	    nonl_parms[(yyvsp[(1) - (3)].ival)].constr = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 345:
#line 3101 "pars.yacc"
    {
        force_redraw();
	;}
    break;

  case 346:
#line 3104 "pars.yacc"
    {
#ifndef NONE_GUI
            if (inwin) {
                update_all();
            }
#endif
        ;}
    break;

  case 347:
#line 3111 "pars.yacc"
    {
	    set_workingdir((yyvsp[(2) - (2)].sval));
	    xfree((yyvsp[(2) - (2)].sval));
	;}
    break;

  case 348:
#line 3115 "pars.yacc"
    {
	    echomsg((yyvsp[(2) - (2)].sval));
	    xfree((yyvsp[(2) - (2)].sval));
	;}
    break;

  case 349:
#line 3119 "pars.yacc"
    {
	    char buf[32];
            set_locale_num(TRUE);
            sprintf(buf, "%g", (yyvsp[(2) - (2)].dval));
            set_locale_num(FALSE);
            echomsg(buf);
	;}
    break;

  case 350:
#line 3126 "pars.yacc"
    {
	    close_input = copy_string(close_input, "");
	;}
    break;

  case 351:
#line 3129 "pars.yacc"
    {
	    close_input = copy_string(close_input, (yyvsp[(2) - (2)].sval));
	;}
    break;

  case 352:
#line 3132 "pars.yacc"
    {
	    exit(0);
	;}
    break;

  case 353:
#line 3135 "pars.yacc"
    {
	    exit((yyvsp[(3) - (4)].ival));
	;}
    break;

  case 354:
#line 3138 "pars.yacc"
    {
	    if (!safe_mode) {
                do_hardcopy();
            } else {
                yyerror("File modifications are disabled in safe mode");
            }
	;}
    break;

  case 355:
#line 3145 "pars.yacc"
    {
            set_ptofile(FALSE);
	;}
    break;

  case 356:
#line 3148 "pars.yacc"
    {
            set_ptofile(TRUE);
	    strcpy(print_file, (yyvsp[(3) - (3)].sval));
            xfree((yyvsp[(3) - (3)].sval));
	;}
    break;

  case 357:
#line 3153 "pars.yacc"
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
	;}
    break;

  case 358:
#line 3175 "pars.yacc"
    {
	    if ((yyvsp[(2) - (2)].dval) > 0) {
	        msleep_wrap((unsigned int) (1000 * (yyvsp[(2) - (2)].dval)));
	    }
	;}
    break;

  case 359:
#line 3180 "pars.yacc"
    {
#ifndef NONE_GUI
            if (inwin) {
                HelpCB((yyvsp[(2) - (2)].sval));
            }
            xfree((yyvsp[(2) - (2)].sval));
#endif
	;}
    break;

  case 360:
#line 3188 "pars.yacc"
    {
#ifndef NONE_GUI
            if (inwin) {
                HelpCB("doc/UsersGuide.html");
            }
#endif
	;}
    break;

  case 361:
#line 3195 "pars.yacc"
    {
	    gotparams = TRUE;
	    strcpy(paramfile, (yyvsp[(2) - (2)].sval));
	    xfree((yyvsp[(2) - (2)].sval));
	;}
    break;

  case 362:
#line 3200 "pars.yacc"
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
	;}
    break;

  case 363:
#line 3212 "pars.yacc"
    {
	    set_set_hidden((yyvsp[(1) - (3)].trgt)->gno, (yyvsp[(1) - (3)].trgt)->setno, (yyvsp[(3) - (3)].ival));
	;}
    break;

  case 364:
#line 3215 "pars.yacc"
    {
	    setlength((yyvsp[(1) - (3)].trgt)->gno, (yyvsp[(1) - (3)].trgt)->setno, (yyvsp[(3) - (3)].ival));
	;}
    break;

  case 365:
#line 3218 "pars.yacc"
    {
	    realloc_vrbl((yyvsp[(1) - (3)].vrbl), (yyvsp[(3) - (3)].ival));
	;}
    break;

  case 366:
#line 3221 "pars.yacc"
    {
	    add_point((yyvsp[(1) - (5)].trgt)->gno, (yyvsp[(1) - (5)].trgt)->setno, (yyvsp[(3) - (5)].dval), (yyvsp[(5) - (5)].dval));
	;}
    break;

  case 367:
#line 3225 "pars.yacc"
    {
	    int start = (yyvsp[(3) - (5)].ival) - index_shift;
	    int stop = (yyvsp[(5) - (5)].ival) - index_shift;
	    droppoints((yyvsp[(1) - (5)].trgt)->gno, (yyvsp[(1) - (5)].trgt)->setno, start, stop);
	;}
    break;

  case 368:
#line 3230 "pars.yacc"
    {
	    if (is_set_active((yyvsp[(2) - (4)].trgt)->gno, (yyvsp[(2) - (4)].trgt)->setno)) {
	        sortset((yyvsp[(2) - (4)].trgt)->gno, (yyvsp[(2) - (4)].trgt)->setno, (yyvsp[(3) - (4)].ival), (yyvsp[(4) - (4)].ival) == ASCENDING ? 0 : 1);
	    }
	;}
    break;

  case 369:
#line 3235 "pars.yacc"
    {
	    do_copyset((yyvsp[(2) - (4)].trgt)->gno, (yyvsp[(2) - (4)].trgt)->setno, (yyvsp[(4) - (4)].trgt)->gno, (yyvsp[(4) - (4)].trgt)->setno);
	;}
    break;

  case 370:
#line 3238 "pars.yacc"
    {
	    if ((yyvsp[(2) - (4)].trgt)->gno != (yyvsp[(4) - (4)].trgt)->gno) {
                errmsg("Can't append sets from different graphs");
            } else {
                int sets[2];
	        sets[0] = (yyvsp[(4) - (4)].trgt)->setno;
	        sets[1] = (yyvsp[(2) - (4)].trgt)->setno;
	        join_sets((yyvsp[(2) - (4)].trgt)->gno, sets, 2);
            }
	;}
    break;

  case 371:
#line 3248 "pars.yacc"
    {
            reverse_set((yyvsp[(2) - (2)].trgt)->gno, (yyvsp[(2) - (2)].trgt)->setno);
	;}
    break;

  case 372:
#line 3251 "pars.yacc"
    {
            do_splitsets((yyvsp[(2) - (3)].trgt)->gno, (yyvsp[(2) - (3)].trgt)->setno, (yyvsp[(3) - (3)].ival));
	;}
    break;

  case 373:
#line 3254 "pars.yacc"
    {
	    do_moveset((yyvsp[(2) - (4)].trgt)->gno, (yyvsp[(2) - (4)].trgt)->setno, (yyvsp[(4) - (4)].trgt)->gno, (yyvsp[(4) - (4)].trgt)->setno);
	;}
    break;

  case 374:
#line 3257 "pars.yacc"
    {
	    do_swapset((yyvsp[(2) - (4)].trgt)->gno, (yyvsp[(2) - (4)].trgt)->setno, (yyvsp[(4) - (4)].trgt)->gno, (yyvsp[(4) - (4)].trgt)->setno);
	;}
    break;

  case 375:
#line 3260 "pars.yacc"
    {
	    killset((yyvsp[(2) - (2)].trgt)->gno, (yyvsp[(2) - (2)].trgt)->setno);
	;}
    break;

  case 376:
#line 3263 "pars.yacc"
    {
            killsetdata((yyvsp[(2) - (3)].trgt)->gno, (yyvsp[(2) - (3)].trgt)->setno);
        ;}
    break;

  case 377:
#line 3266 "pars.yacc"
    {
            kill_graph((yyvsp[(2) - (2)].ival));
        ;}
    break;

  case 378:
#line 3269 "pars.yacc"
    {
            kill_region((yyvsp[(2) - (2)].ival));
        ;}
    break;

  case 379:
#line 3272 "pars.yacc"
    {
            wipeout();
        ;}
    break;

  case 380:
#line 3275 "pars.yacc"
    {
            arrange_graphs_simple((yyvsp[(3) - (12)].ival), (yyvsp[(5) - (12)].ival), 0, FALSE, (yyvsp[(7) - (12)].dval), (yyvsp[(9) - (12)].dval), (yyvsp[(11) - (12)].dval));
        ;}
    break;

  case 381:
#line 3278 "pars.yacc"
    {
            int order = ((yyvsp[(13) - (18)].ival) * GA_ORDER_HV_INV) |
                        ((yyvsp[(15) - (18)].ival) * GA_ORDER_H_INV ) |
                        ((yyvsp[(17) - (18)].ival) * GA_ORDER_V_INV );
            arrange_graphs_simple((yyvsp[(3) - (18)].ival), (yyvsp[(5) - (18)].ival), order, FALSE, (yyvsp[(7) - (18)].dval), (yyvsp[(9) - (18)].dval), (yyvsp[(11) - (18)].dval));
        ;}
    break;

  case 382:
#line 3284 "pars.yacc"
    {
            int order = ((yyvsp[(13) - (20)].ival) * GA_ORDER_HV_INV) |
                        ((yyvsp[(15) - (20)].ival) * GA_ORDER_H_INV ) |
                        ((yyvsp[(17) - (20)].ival) * GA_ORDER_V_INV );
            arrange_graphs_simple((yyvsp[(3) - (20)].ival), (yyvsp[(5) - (20)].ival), order, (yyvsp[(19) - (20)].ival), (yyvsp[(7) - (20)].dval), (yyvsp[(9) - (20)].dval), (yyvsp[(11) - (20)].dval));
        ;}
    break;

  case 383:
#line 3290 "pars.yacc"
    {
	    gotnlfit = TRUE;
	    nlfit_gno = (yyvsp[(3) - (6)].trgt)->gno;
	    nlfit_setno = (yyvsp[(3) - (6)].trgt)->setno;
	    nlfit_nsteps = (yyvsp[(5) - (6)].ival);
	    nlfit_warray = NULL;
	;}
    break;

  case 384:
#line 3297 "pars.yacc"
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
	;}
    break;

  case 385:
#line 3309 "pars.yacc"
    {
	    do_regress((yyvsp[(3) - (6)].trgt)->gno, (yyvsp[(3) - (6)].trgt)->setno, (yyvsp[(5) - (6)].ival), 0, -1, 0, -1);
	;}
    break;

  case 386:
#line 3312 "pars.yacc"
    {
	    do_runavg((yyvsp[(3) - (6)].trgt)->gno, (yyvsp[(3) - (6)].trgt)->setno, (yyvsp[(5) - (6)].ival), (yyvsp[(1) - (6)].ival), -1, 0);
	;}
    break;

  case 387:
#line 3315 "pars.yacc"
    {
	    do_fourier_command((yyvsp[(3) - (6)].trgt)->gno, (yyvsp[(3) - (6)].trgt)->setno, (yyvsp[(1) - (6)].ival), (yyvsp[(5) - (6)].ival));
	;}
    break;

  case 388:
#line 3319 "pars.yacc"
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
        ;}
    break;

  case 389:
#line 3338 "pars.yacc"
    {
            do_interp((yyvsp[(3) - (10)].trgt)->gno, (yyvsp[(3) - (10)].trgt)->setno, get_cg(), SET_SELECT_NEXT,
                (yyvsp[(5) - (10)].vrbl)->data, (yyvsp[(5) - (10)].vrbl)->length, (yyvsp[(7) - (10)].ival), (yyvsp[(9) - (10)].ival));
	;}
    break;

  case 390:
#line 3342 "pars.yacc"
    {
            do_histo((yyvsp[(3) - (10)].trgt)->gno, (yyvsp[(3) - (10)].trgt)->setno, get_cg(), SET_SELECT_NEXT,
                (yyvsp[(5) - (10)].vrbl)->data, (yyvsp[(5) - (10)].vrbl)->length - 1, (yyvsp[(7) - (10)].ival), (yyvsp[(9) - (10)].ival));
	;}
    break;

  case 391:
#line 3346 "pars.yacc"
    {
	    do_differ((yyvsp[(3) - (6)].trgt)->gno, (yyvsp[(3) - (6)].trgt)->setno, (yyvsp[(5) - (6)].ival));
	;}
    break;

  case 392:
#line 3349 "pars.yacc"
    {
	    do_int((yyvsp[(3) - (4)].trgt)->gno, (yyvsp[(3) - (4)].trgt)->setno, 0);
	;}
    break;

  case 393:
#line 3352 "pars.yacc"
    {
	    do_xcor((yyvsp[(3) - (10)].trgt)->gno, (yyvsp[(3) - (10)].trgt)->setno, (yyvsp[(5) - (10)].trgt)->gno, (yyvsp[(5) - (10)].trgt)->setno, (yyvsp[(7) - (10)].ival), (yyvsp[(9) - (10)].ival));
	;}
    break;

  case 394:
#line 3355 "pars.yacc"
    {
	    do_linearc((yyvsp[(3) - (6)].trgt)->gno, (yyvsp[(3) - (6)].trgt)->setno, (yyvsp[(5) - (6)].trgt)->gno, (yyvsp[(5) - (6)].trgt)->setno);
	;}
    break;

  case 395:
#line 3358 "pars.yacc"
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
	;}
    break;

  case 396:
#line 3375 "pars.yacc"
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
	;}
    break;

  case 397:
#line 3390 "pars.yacc"
    {
	    if (autoscale_graph(whichgraph, AUTOSCALE_XY) != RETURN_SUCCESS) {
		errmsg("Can't autoscale (no active sets?)");
	    }
	;}
    break;

  case 398:
#line 3395 "pars.yacc"
    {
	    if (autoscale_graph(whichgraph, AUTOSCALE_X) != RETURN_SUCCESS) {
		errmsg("Can't autoscale (no active sets?)");
	    }
	;}
    break;

  case 399:
#line 3400 "pars.yacc"
    {
	    if (autoscale_graph(whichgraph, AUTOSCALE_Y) != RETURN_SUCCESS) {
		errmsg("Can't autoscale (no active sets?)");
	    }
	;}
    break;

  case 400:
#line 3405 "pars.yacc"
    {
	    autoscale_byset((yyvsp[(2) - (2)].trgt)->gno, (yyvsp[(2) - (2)].trgt)->setno, AUTOSCALE_XY);
	;}
    break;

  case 401:
#line 3408 "pars.yacc"
    {
            autotick_axis(whichgraph, ALL_AXES);
        ;}
    break;

  case 402:
#line 3411 "pars.yacc"
    {
	    int gno = (yyvsp[(2) - (2)].ival);
            if (is_graph_hidden(gno) == FALSE) {
                select_graph(gno);
            } else {
		errmsg("Graph is not active");
            }
	;}
    break;

  case 403:
#line 3419 "pars.yacc"
    {
	    gotread = TRUE;
	    strcpy(readfile, (yyvsp[(2) - (2)].sval));
	    xfree((yyvsp[(2) - (2)].sval));
	;}
    break;

  case 404:
#line 3424 "pars.yacc"
    {
	    strcpy(batchfile, (yyvsp[(3) - (3)].sval));
	    xfree((yyvsp[(3) - (3)].sval));
	;}
    break;

  case 405:
#line 3428 "pars.yacc"
    {
	    getdata(whichgraph, (yyvsp[(3) - (3)].sval), SOURCE_DISK, LOAD_BLOCK);
	    xfree((yyvsp[(3) - (3)].sval));
	;}
    break;

  case 406:
#line 3432 "pars.yacc"
    {
	    getdata(whichgraph, (yyvsp[(4) - (4)].sval), (yyvsp[(3) - (4)].ival), LOAD_BLOCK);
	    xfree((yyvsp[(4) - (4)].sval));
	;}
    break;

  case 407:
#line 3436 "pars.yacc"
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
	;}
    break;

  case 408:
#line 3449 "pars.yacc"
    {
	    set_blockdata(NULL);
	;}
    break;

  case 409:
#line 3452 "pars.yacc"
    {
	    gotread = TRUE;
	    curtype = (yyvsp[(2) - (3)].ival);
	    strcpy(readfile, (yyvsp[(3) - (3)].sval));
	    xfree((yyvsp[(3) - (3)].sval));
	;}
    break;

  case 410:
#line 3458 "pars.yacc"
    {
	    gotread = TRUE;
	    strcpy(readfile, (yyvsp[(4) - (4)].sval));
	    curtype = (yyvsp[(2) - (4)].ival);
	    cursource = (yyvsp[(3) - (4)].ival);
	    xfree((yyvsp[(4) - (4)].sval));
	;}
    break;

  case 411:
#line 3465 "pars.yacc"
    {
	    getdata(whichgraph, (yyvsp[(3) - (3)].sval), SOURCE_DISK, LOAD_NXY);
	    xfree((yyvsp[(3) - (3)].sval));
	;}
    break;

  case 412:
#line 3469 "pars.yacc"
    {
	    getdata(whichgraph, (yyvsp[(4) - (4)].sval), (yyvsp[(3) - (4)].ival), LOAD_NXY);
	    xfree((yyvsp[(4) - (4)].sval));
	;}
    break;

  case 413:
#line 3473 "pars.yacc"
    {
	    if (!safe_mode) {
                outputset((yyvsp[(2) - (2)].trgt)->gno, (yyvsp[(2) - (2)].trgt)->setno, "stdout", NULL);
            } else {
                yyerror("File modifications are disabled in safe mode");
            }
	;}
    break;

  case 414:
#line 3480 "pars.yacc"
    {
	    if (!safe_mode) {
	        outputset((yyvsp[(2) - (4)].trgt)->gno, (yyvsp[(2) - (4)].trgt)->setno, "stdout", (yyvsp[(4) - (4)].sval));
            } else {
                yyerror("File modifications are disabled in safe mode");
            }
	    xfree((yyvsp[(4) - (4)].sval));
	;}
    break;

  case 415:
#line 3488 "pars.yacc"
    {
	    if (!safe_mode) {
	        outputset((yyvsp[(2) - (4)].trgt)->gno, (yyvsp[(2) - (4)].trgt)->setno, (yyvsp[(4) - (4)].sval), NULL);
            } else {
                yyerror("File modifications are disabled in safe mode");
            }
	    xfree((yyvsp[(4) - (4)].sval));
	;}
    break;

  case 416:
#line 3496 "pars.yacc"
    {
	    if (!safe_mode) {
	        outputset((yyvsp[(2) - (6)].trgt)->gno, (yyvsp[(2) - (6)].trgt)->setno, (yyvsp[(4) - (6)].sval), (yyvsp[(6) - (6)].sval));
            } else {
                yyerror("File modifications are disabled in safe mode");
            }
	    xfree((yyvsp[(4) - (6)].sval));
	    xfree((yyvsp[(6) - (6)].sval));
	;}
    break;

  case 417:
#line 3505 "pars.yacc"
    {
            if (!safe_mode) {
                save_project((yyvsp[(2) - (2)].sval));
            } else {
                yyerror("File modifications are disabled in safe mode");
            }
            xfree((yyvsp[(2) - (2)].sval));
        ;}
    break;

  case 418:
#line 3513 "pars.yacc"
    {
            load_project((yyvsp[(2) - (2)].sval));
            xfree((yyvsp[(2) - (2)].sval));
        ;}
    break;

  case 419:
#line 3517 "pars.yacc"
    {
            new_project(NULL);
        ;}
    break;

  case 420:
#line 3520 "pars.yacc"
    {
            new_project((yyvsp[(3) - (3)].sval));
            xfree((yyvsp[(3) - (3)].sval));
        ;}
    break;

  case 421:
#line 3524 "pars.yacc"
    {
	    push_world();
	;}
    break;

  case 422:
#line 3527 "pars.yacc"
    {
	    pop_world();
	;}
    break;

  case 423:
#line 3530 "pars.yacc"
    {
	    cycle_world_stack();
	;}
    break;

  case 424:
#line 3533 "pars.yacc"
    {
	    if ((yyvsp[(2) - (2)].ival) > 0)
		show_world_stack((yyvsp[(2) - (2)].ival) - 1);
	;}
    break;

  case 425:
#line 3537 "pars.yacc"
    {
	    clear_world_stack();
	;}
    break;

  case 426:
#line 3540 "pars.yacc"
    {
	    do_clear_boxes();
	;}
    break;

  case 427:
#line 3543 "pars.yacc"
    {
	    do_clear_ellipses();
	;}
    break;

  case 428:
#line 3546 "pars.yacc"
    {
	    do_clear_lines();
	;}
    break;

  case 429:
#line 3549 "pars.yacc"
    {
	    do_clear_text();
	;}
    break;

  case 430:
#line 3556 "pars.yacc"
    {
#ifndef NONE_GUI
            set_pagelayout((yyvsp[(3) - (3)].ival));
#endif
        ;}
    break;

  case 431:
#line 3561 "pars.yacc"
    {
	    auto_redraw = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 432:
#line 3564 "pars.yacc"
    {
	    draw_focus_flag = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 433:
#line 3567 "pars.yacc"
    {
	    focus_policy = FOCUS_SET;
	;}
    break;

  case 434:
#line 3570 "pars.yacc"
    {
	    focus_policy = FOCUS_FOLLOWS;
	;}
    break;

  case 435:
#line 3573 "pars.yacc"
    {
	    focus_policy = FOCUS_CLICK;
	;}
    break;

  case 436:
#line 3580 "pars.yacc"
    {;}
    break;

  case 437:
#line 3581 "pars.yacc"
    {;}
    break;

  case 438:
#line 3585 "pars.yacc"
    {
	    set_set_hidden((yyvsp[(1) - (2)].trgt)->gno, (yyvsp[(1) - (2)].trgt)->setno, !(yyvsp[(2) - (2)].ival));
	;}
    break;

  case 439:
#line 3588 "pars.yacc"
    {
	    set_dataset_type((yyvsp[(1) - (3)].trgt)->gno, (yyvsp[(1) - (3)].trgt)->setno, (yyvsp[(3) - (3)].ival));
	;}
    break;

  case 440:
#line 3592 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].sym = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 441:
#line 3595 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].sympen.color = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 442:
#line 3598 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].sympen.pattern = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 443:
#line 3601 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].symlinew = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 444:
#line 3604 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].symlines = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 445:
#line 3607 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].symfillpen.color = (yyvsp[(4) - (4)].ival);
	;}
    break;

  case 446:
#line 3610 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].symfillpen.pattern = (yyvsp[(4) - (4)].ival);
	;}
    break;

  case 447:
#line 3613 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].symsize = (yyvsp[(4) - (4)].dval);
	;}
    break;

  case 448:
#line 3616 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].symchar = (yyvsp[(4) - (4)].ival);
	;}
    break;

  case 449:
#line 3619 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].charfont = (yyvsp[(4) - (4)].ival);
	;}
    break;

  case 450:
#line 3622 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].symskip = (yyvsp[(4) - (4)].ival);
	;}
    break;

  case 451:
#line 3627 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].linet = (yyvsp[(4) - (4)].ival);
	;}
    break;

  case 452:
#line 3631 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].lines = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 453:
#line 3635 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].linew = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 454:
#line 3639 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].linepen.color = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 455:
#line 3643 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].linepen.pattern = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 456:
#line 3648 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].filltype = (yyvsp[(4) - (4)].ival);
	;}
    break;

  case 457:
#line 3652 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].fillrule = (yyvsp[(4) - (4)].ival);
	;}
    break;

  case 458:
#line 3656 "pars.yacc"
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
	;}
    break;

  case 459:
#line 3674 "pars.yacc"
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
	;}
    break;

  case 460:
#line 3694 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].baseline = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 461:
#line 3698 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].baseline_type = (yyvsp[(4) - (4)].ival);
	;}
    break;

  case 462:
#line 3703 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].dropline = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 463:
#line 3708 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].avalue.active = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 464:
#line 3712 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].avalue.type = (yyvsp[(4) - (4)].ival);
	;}
    break;

  case 465:
#line 3716 "pars.yacc"
    {
	    g[(yyvsp[(1) - (5)].trgt)->gno].p[(yyvsp[(1) - (5)].trgt)->setno].avalue.size = (yyvsp[(5) - (5)].dval);
	;}
    break;

  case 466:
#line 3720 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].avalue.font = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 467:
#line 3724 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].avalue.color = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 468:
#line 3728 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].avalue.angle = (yyvsp[(4) - (4)].ival);
	;}
    break;

  case 469:
#line 3732 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].avalue.format = (yyvsp[(4) - (4)].ival);
	;}
    break;

  case 470:
#line 3736 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].avalue.prec = (yyvsp[(4) - (4)].ival);
	;}
    break;

  case 471:
#line 3739 "pars.yacc"
    {
	    g[(yyvsp[(1) - (6)].trgt)->gno].p[(yyvsp[(1) - (6)].trgt)->setno].avalue.offset.x = (yyvsp[(4) - (6)].dval);
	    g[(yyvsp[(1) - (6)].trgt)->gno].p[(yyvsp[(1) - (6)].trgt)->setno].avalue.offset.y = (yyvsp[(6) - (6)].dval);
	;}
    break;

  case 472:
#line 3744 "pars.yacc"
    {
	    strcpy(g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].avalue.prestr, (yyvsp[(4) - (4)].sval));
	    xfree((yyvsp[(4) - (4)].sval));
	;}
    break;

  case 473:
#line 3749 "pars.yacc"
    {
	    strcpy(g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].avalue.appstr, (yyvsp[(4) - (4)].sval));
	    xfree((yyvsp[(4) - (4)].sval));
	;}
    break;

  case 474:
#line 3754 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].errbar.active = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 475:
#line 3757 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].errbar.ptype = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 476:
#line 3760 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].errbar.pen.color = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 477:
#line 3763 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].errbar.pen.pattern = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 478:
#line 3766 "pars.yacc"
    {
            g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].errbar.barsize = (yyvsp[(4) - (4)].dval);
	;}
    break;

  case 479:
#line 3769 "pars.yacc"
    {
            g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].errbar.linew = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 480:
#line 3772 "pars.yacc"
    {
            g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].errbar.lines = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 481:
#line 3775 "pars.yacc"
    {
            g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].errbar.riser_linew = (yyvsp[(4) - (4)].dval);
	;}
    break;

  case 482:
#line 3778 "pars.yacc"
    {
            g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].errbar.riser_lines = (yyvsp[(4) - (4)].ival);
	;}
    break;

  case 483:
#line 3781 "pars.yacc"
    {
            g[(yyvsp[(1) - (5)].trgt)->gno].p[(yyvsp[(1) - (5)].trgt)->setno].errbar.arrow_clip = (yyvsp[(5) - (5)].ival);
	;}
    break;

  case 484:
#line 3784 "pars.yacc"
    {
            g[(yyvsp[(1) - (6)].trgt)->gno].p[(yyvsp[(1) - (6)].trgt)->setno].errbar.cliplen = (yyvsp[(6) - (6)].dval);
	;}
    break;

  case 485:
#line 3788 "pars.yacc"
    {
	    strncpy(g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].comments, (yyvsp[(3) - (3)].sval), MAX_STRING_LENGTH - 1);
	    xfree((yyvsp[(3) - (3)].sval));
	;}
    break;

  case 486:
#line 3793 "pars.yacc"
    {
        set_legend_string((yyvsp[(1) - (3)].trgt)->gno,(yyvsp[(1) - (3)].trgt)->setno,(yyvsp[(3) - (3)].sval));
	    xfree((yyvsp[(3) - (3)].sval));
	;}
    break;

  case 487:
#line 3801 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->active = (yyvsp[(1) - (1)].ival);
	;}
    break;

  case 488:
#line 3808 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->zero = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 489:
#line 3815 "pars.yacc"
    {;}
    break;

  case 490:
#line 3816 "pars.yacc"
    {;}
    break;

  case 491:
#line 3817 "pars.yacc"
    {;}
    break;

  case 492:
#line 3818 "pars.yacc"
    {;}
    break;

  case 493:
#line 3819 "pars.yacc"
    {;}
    break;

  case 494:
#line 3820 "pars.yacc"
    {;}
    break;

  case 495:
#line 3821 "pars.yacc"
    {;}
    break;

  case 496:
#line 3822 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
            g[whichgraph].t[naxis]->offsx = (yyvsp[(2) - (4)].dval);
	    g[whichgraph].t[naxis]->offsy = (yyvsp[(4) - (4)].dval);
	;}
    break;

  case 497:
#line 3833 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_flag = (yyvsp[(1) - (1)].ival);
	;}
    break;

  case 498:
#line 3840 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
            g[whichgraph].t[naxis]->tmajor = (yyvsp[(2) - (2)].dval);
	;}
    break;

  case 499:
#line 3847 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->nminor = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 500:
#line 3854 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_round = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 501:
#line 3862 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
            g[whichgraph].t[naxis]->offsx = (yyvsp[(2) - (2)].dval);
	;}
    break;

  case 502:
#line 3869 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
            g[whichgraph].t[naxis]->offsy = (yyvsp[(2) - (2)].dval);
	;}
    break;

  case 503:
#line 3876 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_autonum = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 504:
#line 3883 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_inout = (yyvsp[(1) - (1)].ival);
	;}
    break;

  case 505:
#line 3890 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.size = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 506:
#line 3897 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->mprops.size = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 507:
#line 3904 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.color = g[whichgraph].t[naxis]->mprops.color = (yyvsp[(1) - (1)].ival);
	;}
    break;

  case 508:
#line 3911 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.color = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 509:
#line 3918 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->mprops.color = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 510:
#line 3925 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.linew = g[whichgraph].t[naxis]->mprops.linew = (yyvsp[(1) - (1)].dval);
	;}
    break;

  case 511:
#line 3932 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.linew = (yyvsp[(2) - (2)].dval);
	;}
    break;

  case 512:
#line 3939 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->mprops.linew = (yyvsp[(2) - (2)].dval);
	;}
    break;

  case 513:
#line 3946 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.lines = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 514:
#line 3953 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->mprops.lines = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 515:
#line 3960 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.gridflag = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 516:
#line 3967 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->mprops.gridflag = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 517:
#line 3974 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_op = (yyvsp[(1) - (1)].ival);
	;}
    break;

  case 518:
#line 3981 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_spec = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 519:
#line 3988 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->nticks = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 520:
#line 3995 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tloc[(yyvsp[(2) - (4)].ival)].wtpos = (yyvsp[(4) - (4)].dval);
	    g[whichgraph].t[naxis]->tloc[(yyvsp[(2) - (4)].ival)].type = TICK_TYPE_MAJOR;
	;}
    break;

  case 521:
#line 4003 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tloc[(yyvsp[(2) - (4)].ival)].wtpos = (yyvsp[(4) - (4)].dval);
	    g[whichgraph].t[naxis]->tloc[(yyvsp[(2) - (4)].ival)].type = TICK_TYPE_MINOR;
	;}
    break;

  case 522:
#line 4014 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_flag = (yyvsp[(1) - (1)].ival);
	;}
    break;

  case 523:
#line 4021 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_prec = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 524:
#line 4028 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_format = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 525:
#line 4035 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_format = (yyvsp[(2) - (2)].dval);
	;}
    break;

  case 526:
#line 4042 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    strcpy(g[whichgraph].t[naxis]->tl_appstr, (yyvsp[(2) - (2)].sval));
        strcpy(g[whichgraph].t[naxis]->orig_tl_appstr, (yyvsp[(2) - (2)].sval));
	    xfree((yyvsp[(2) - (2)].sval));
	;}
    break;

  case 527:
#line 4051 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    strcpy(g[whichgraph].t[naxis]->tl_prestr, (yyvsp[(2) - (2)].sval));
        strcpy(g[whichgraph].t[naxis]->orig_tl_prestr, (yyvsp[(2) - (2)].sval));
	    xfree((yyvsp[(2) - (2)].sval));
	;}
    break;

  case 528:
#line 4060 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_angle = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 529:
#line 4067 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_skip = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 530:
#line 4074 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_staggered = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 531:
#line 4081 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_op = (yyvsp[(1) - (1)].ival);
	;}
    break;

  case 532:
#line 4088 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
            g[whichgraph].t[naxis]->tl_formula =
                copy_string(g[whichgraph].t[naxis]->tl_formula, (yyvsp[(2) - (2)].sval));
            xfree((yyvsp[(2) - (2)].sval));
	;}
    break;

  case 533:
#line 4097 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_start = (yyvsp[(2) - (2)].dval);
	;}
    break;

  case 534:
#line 4104 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_stop = (yyvsp[(2) - (2)].dval);
	;}
    break;

  case 535:
#line 4111 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_starttype = TYPE_SPEC;
	;}
    break;

  case 536:
#line 4118 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_starttype = TYPE_AUTO;
	;}
    break;

  case 537:
#line 4125 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_stoptype = TYPE_SPEC;
	;}
    break;

  case 538:
#line 4132 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_stoptype = TYPE_AUTO;
	;}
    break;

  case 539:
#line 4139 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_charsize = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 540:
#line 4146 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_font = (yyvsp[(1) - (1)].ival);
	;}
    break;

  case 541:
#line 4153 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_color = (yyvsp[(1) - (1)].ival);
	;}
    break;

  case 542:
#line 4160 "pars.yacc"
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
        g[whichgraph].t[naxis]->tloc[(yyvsp[(1) - (3)].ival)].orig_label =
                copy_string(g[whichgraph].t[naxis]->tloc[(yyvsp[(1) - (3)].ival)].orig_label, (yyvsp[(3) - (3)].sval));
	    xfree((yyvsp[(3) - (3)].sval));
	;}
    break;

  case 543:
#line 4177 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_gaptype = TYPE_AUTO;
	;}
    break;

  case 544:
#line 4184 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_gaptype = TYPE_SPEC;
	;}
    break;

  case 545:
#line 4191 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_gap.x = (yyvsp[(2) - (4)].dval);
	    g[whichgraph].t[naxis]->tl_gap.y = (yyvsp[(4) - (4)].dval);
	;}
    break;

  case 546:
#line 4202 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    set_plotstr_string(&g[whichgraph].t[naxis]->label, (yyvsp[(1) - (1)].sval));
	    xfree((yyvsp[(1) - (1)].sval));
	;}
    break;

  case 547:
#line 4210 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label_layout = LAYOUT_PERPENDICULAR;
	;}
    break;

  case 548:
#line 4217 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label_layout = LAYOUT_PARALLEL;
	;}
    break;

  case 549:
#line 4224 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label_place = TYPE_AUTO;
	;}
    break;

  case 550:
#line 4231 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label_place = TYPE_SPEC;
	;}
    break;

  case 551:
#line 4238 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label.x = (yyvsp[(2) - (4)].dval);
	    g[whichgraph].t[naxis]->label.y = (yyvsp[(4) - (4)].dval);
	;}
    break;

  case 552:
#line 4246 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label.just = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 553:
#line 4253 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label.charsize = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 554:
#line 4260 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label.font = (yyvsp[(1) - (1)].ival);
	;}
    break;

  case 555:
#line 4267 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label.color = (yyvsp[(1) - (1)].ival);
	;}
    break;

  case 556:
#line 4274 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label_op = (yyvsp[(1) - (1)].ival);
	;}
    break;

  case 557:
#line 4284 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_drawbar = (yyvsp[(1) - (1)].ival);
	;}
    break;

  case 558:
#line 4291 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_drawbarcolor = (yyvsp[(1) - (1)].ival);
	;}
    break;

  case 559:
#line 4298 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_drawbarlines = (yyvsp[(1) - (1)].ival);
	;}
    break;

  case 560:
#line 4305 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_drawbarlinew = (yyvsp[(1) - (1)].dval);
	;}
    break;

  case 561:
#line 4315 "pars.yacc"
    { 
          nonl_opts.title = copy_string(nonl_opts.title, (yyvsp[(2) - (2)].sval));
	  xfree((yyvsp[(2) - (2)].sval));
        ;}
    break;

  case 562:
#line 4319 "pars.yacc"
    { 
          nonl_opts.formula = copy_string(nonl_opts.formula, (yyvsp[(2) - (2)].sval));
	  xfree((yyvsp[(2) - (2)].sval));
        ;}
    break;

  case 563:
#line 4323 "pars.yacc"
    { 
            nonl_opts.parnum = (yyvsp[(2) - (3)].ival); 
        ;}
    break;

  case 564:
#line 4326 "pars.yacc"
    { 
            nonl_opts.tolerance = (yyvsp[(2) - (2)].dval); 
        ;}
    break;

  case 565:
#line 4333 "pars.yacc"
    {
            (yyval.ival) = (yyvsp[(1) - (1)].ival);
        ;}
    break;

  case 566:
#line 4337 "pars.yacc"
    {
            (yyval.ival) = (yyvsp[(2) - (2)].ival);
        ;}
    break;

  case 567:
#line 4344 "pars.yacc"
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
	;}
    break;

  case 568:
#line 4357 "pars.yacc"
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
	;}
    break;

  case 569:
#line 4370 "pars.yacc"
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
	;}
    break;

  case 570:
#line 4383 "pars.yacc"
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
	;}
    break;

  case 571:
#line 4398 "pars.yacc"
    {;}
    break;

  case 572:
#line 4399 "pars.yacc"
    {;}
    break;

  case 573:
#line 4403 "pars.yacc"
    { naxis =  X_AXIS; ;}
    break;

  case 574:
#line 4404 "pars.yacc"
    { naxis = Y_AXIS; ;}
    break;

  case 575:
#line 4405 "pars.yacc"
    { naxis = ZX_AXIS; ;}
    break;

  case 576:
#line 4406 "pars.yacc"
    { naxis = ZY_AXIS; ;}
    break;

  case 577:
#line 4410 "pars.yacc"
    { (yyval.ival) = CONSTANT;  ;}
    break;

  case 578:
#line 4411 "pars.yacc"
    { (yyval.ival) = UCONSTANT; ;}
    break;

  case 579:
#line 4412 "pars.yacc"
    { (yyval.ival) = FUNC_I;    ;}
    break;

  case 580:
#line 4413 "pars.yacc"
    { (yyval.ival) = FUNC_D;    ;}
    break;

  case 581:
#line 4414 "pars.yacc"
    { (yyval.ival) = FUNC_ND;   ;}
    break;

  case 582:
#line 4415 "pars.yacc"
    { (yyval.ival) = FUNC_NN;   ;}
    break;

  case 583:
#line 4416 "pars.yacc"
    { (yyval.ival) = FUNC_DD;   ;}
    break;

  case 584:
#line 4417 "pars.yacc"
    { (yyval.ival) = FUNC_NND;  ;}
    break;

  case 585:
#line 4418 "pars.yacc"
    { (yyval.ival) = FUNC_PPD;  ;}
    break;

  case 586:
#line 4419 "pars.yacc"
    { (yyval.ival) = FUNC_PPPD; ;}
    break;

  case 587:
#line 4420 "pars.yacc"
    { (yyval.ival) = FUNC_PPPPD; ;}
    break;

  case 588:
#line 4421 "pars.yacc"
    { (yyval.ival) = FUNC_PPPPPD; ;}
    break;

  case 589:
#line 4425 "pars.yacc"
    { (yyval.ival) =  TICKS_SPEC_NONE; ;}
    break;

  case 590:
#line 4426 "pars.yacc"
    { (yyval.ival) = TICKS_SPEC_MARKS; ;}
    break;

  case 591:
#line 4427 "pars.yacc"
    { (yyval.ival) = TICKS_SPEC_BOTH; ;}
    break;

  case 592:
#line 4431 "pars.yacc"
    { (yyval.ival) = FILTER_INPUT; ;}
    break;

  case 593:
#line 4432 "pars.yacc"
    { (yyval.ival) = FILTER_OUTPUT; ;}
    break;

  case 594:
#line 4436 "pars.yacc"
    { (yyval.ival) = FILTER_MAGIC; ;}
    break;

  case 595:
#line 4437 "pars.yacc"
    { (yyval.ival) = FILTER_PATTERN; ;}
    break;

  case 596:
#line 4441 "pars.yacc"
    { (yyval.ival) = SET_XY; ;}
    break;

  case 597:
#line 4442 "pars.yacc"
    { (yyval.ival) = SET_BAR; ;}
    break;

  case 598:
#line 4443 "pars.yacc"
    { (yyval.ival) = SET_BARDY; ;}
    break;

  case 599:
#line 4444 "pars.yacc"
    { (yyval.ival) = SET_BARDYDY; ;}
    break;

  case 600:
#line 4445 "pars.yacc"
    { (yyval.ival) = SET_XYZ; ;}
    break;

  case 601:
#line 4446 "pars.yacc"
    { (yyval.ival) = SET_XYDX; ;}
    break;

  case 602:
#line 4447 "pars.yacc"
    { (yyval.ival) = SET_XYDY; ;}
    break;

  case 603:
#line 4448 "pars.yacc"
    { (yyval.ival) = SET_XYDXDX; ;}
    break;

  case 604:
#line 4449 "pars.yacc"
    { (yyval.ival) = SET_XYDYDY; ;}
    break;

  case 605:
#line 4450 "pars.yacc"
    { (yyval.ival) = SET_XYDXDY; ;}
    break;

  case 606:
#line 4451 "pars.yacc"
    { (yyval.ival) = SET_XYDXDXDYDY; ;}
    break;

  case 607:
#line 4452 "pars.yacc"
    { (yyval.ival) = SET_XYHILO; ;}
    break;

  case 608:
#line 4453 "pars.yacc"
    { (yyval.ival) = SET_XYR; ;}
    break;

  case 609:
#line 4454 "pars.yacc"
    { (yyval.ival) = SET_XYSIZE; ;}
    break;

  case 610:
#line 4455 "pars.yacc"
    { (yyval.ival) = SET_XYCOLOR; ;}
    break;

  case 611:
#line 4456 "pars.yacc"
    { (yyval.ival) = SET_XYCOLPAT; ;}
    break;

  case 612:
#line 4457 "pars.yacc"
    { (yyval.ival) = SET_XYVMAP; ;}
    break;

  case 613:
#line 4458 "pars.yacc"
    { (yyval.ival) = SET_BOXPLOT; ;}
    break;

  case 614:
#line 4459 "pars.yacc"
    { (yyval.ival) = SET_XY; ;}
    break;

  case 615:
#line 4463 "pars.yacc"
    { (yyval.ival) = GRAPH_XY; ;}
    break;

  case 616:
#line 4464 "pars.yacc"
    { (yyval.ival) = GRAPH_CHART; ;}
    break;

  case 617:
#line 4465 "pars.yacc"
    { (yyval.ival) = GRAPH_POLAR; ;}
    break;

  case 618:
#line 4466 "pars.yacc"
    { (yyval.ival) = GRAPH_POLAR2; ;}
    break;

  case 619:
#line 4467 "pars.yacc"
    { (yyval.ival) = GRAPH_SMITH; ;}
    break;

  case 620:
#line 4468 "pars.yacc"
    { (yyval.ival) = GRAPH_FIXED; ;}
    break;

  case 621:
#line 4469 "pars.yacc"
    { (yyval.ival) = GRAPH_PIE;   ;}
    break;

  case 622:
#line 4473 "pars.yacc"
    { (yyval.ival) = PAGE_FREE; ;}
    break;

  case 623:
#line 4474 "pars.yacc"
    { (yyval.ival) = PAGE_FIXED; ;}
    break;

  case 624:
#line 4478 "pars.yacc"
    { (yyval.ival) = PAGE_ORIENT_LANDSCAPE; ;}
    break;

  case 625:
#line 4479 "pars.yacc"
    { (yyval.ival) = PAGE_ORIENT_PORTRAIT;  ;}
    break;

  case 626:
#line 4483 "pars.yacc"
    { (yyval.ival) = REGION_ABOVE; ;}
    break;

  case 627:
#line 4484 "pars.yacc"
    { (yyval.ival) = REGION_BELOW; ;}
    break;

  case 628:
#line 4485 "pars.yacc"
    { (yyval.ival) = REGION_TOLEFT; ;}
    break;

  case 629:
#line 4486 "pars.yacc"
    { (yyval.ival) = REGION_TORIGHT; ;}
    break;

  case 630:
#line 4487 "pars.yacc"
    { (yyval.ival) = REGION_POLYI; ;}
    break;

  case 631:
#line 4488 "pars.yacc"
    { (yyval.ival) = REGION_POLYO; ;}
    break;

  case 632:
#line 4489 "pars.yacc"
    { (yyval.ival) = REGION_HORIZI; ;}
    break;

  case 633:
#line 4490 "pars.yacc"
    { (yyval.ival) = REGION_VERTI; ;}
    break;

  case 634:
#line 4491 "pars.yacc"
    { (yyval.ival) = REGION_HORIZO; ;}
    break;

  case 635:
#line 4492 "pars.yacc"
    { (yyval.ival) = REGION_VERTO; ;}
    break;

  case 636:
#line 4495 "pars.yacc"
    { (yyval.ival) = SCALE_NORMAL; ;}
    break;

  case 637:
#line 4496 "pars.yacc"
    { (yyval.ival) = SCALE_LOG; ;}
    break;

  case 638:
#line 4497 "pars.yacc"
    { (yyval.ival) = SCALE_REC; ;}
    break;

  case 639:
#line 4498 "pars.yacc"
    { (yyval.ival) = SCALE_LOGIT; ;}
    break;

  case 640:
#line 4501 "pars.yacc"
    { (yyval.ival) = TRUE; ;}
    break;

  case 641:
#line 4502 "pars.yacc"
    { (yyval.ival) = FALSE; ;}
    break;

  case 642:
#line 4505 "pars.yacc"
    { (yyval.ival) = RUN_AVG; ;}
    break;

  case 643:
#line 4506 "pars.yacc"
    { (yyval.ival) = RUN_STD; ;}
    break;

  case 644:
#line 4507 "pars.yacc"
    { (yyval.ival) = RUN_MED; ;}
    break;

  case 645:
#line 4508 "pars.yacc"
    { (yyval.ival) = RUN_MAX; ;}
    break;

  case 646:
#line 4509 "pars.yacc"
    { (yyval.ival) = RUN_MIN; ;}
    break;

  case 647:
#line 4513 "pars.yacc"
    { (yyval.ival) = SOURCE_DISK; ;}
    break;

  case 648:
#line 4514 "pars.yacc"
    {
            if (!safe_mode) {
                (yyval.ival) = SOURCE_PIPE;
            } else {
                yyerror("Pipe inputs are disabled in safe mode");
                (yyval.ival) = SOURCE_DISK;
            }
        ;}
    break;

  case 649:
#line 4524 "pars.yacc"
    { (yyval.ival) = JUST_RIGHT; ;}
    break;

  case 650:
#line 4525 "pars.yacc"
    { (yyval.ival) = JUST_LEFT; ;}
    break;

  case 651:
#line 4526 "pars.yacc"
    { (yyval.ival) = JUST_CENTER; ;}
    break;

  case 652:
#line 4529 "pars.yacc"
    { (yyval.ival) = TICKS_IN; ;}
    break;

  case 653:
#line 4530 "pars.yacc"
    { (yyval.ival) = TICKS_OUT; ;}
    break;

  case 654:
#line 4531 "pars.yacc"
    { (yyval.ival) = TICKS_BOTH; ;}
    break;

  case 655:
#line 4534 "pars.yacc"
    { (yyval.ival) = FORMAT_DECIMAL; ;}
    break;

  case 656:
#line 4535 "pars.yacc"
    { (yyval.ival) = FORMAT_EXPONENTIAL; ;}
    break;

  case 657:
#line 4536 "pars.yacc"
    { (yyval.ival) = FORMAT_GENERAL; ;}
    break;

  case 658:
#line 4537 "pars.yacc"
    { (yyval.ival) = FORMAT_SCIENTIFIC; ;}
    break;

  case 659:
#line 4538 "pars.yacc"
    { (yyval.ival) = FORMAT_ENGINEERING; ;}
    break;

  case 660:
#line 4539 "pars.yacc"
    { (yyval.ival) = FORMAT_COMPUTING; ;}
    break;

  case 661:
#line 4540 "pars.yacc"
    { (yyval.ival) = FORMAT_POWER; ;}
    break;

  case 662:
#line 4541 "pars.yacc"
    { (yyval.ival) = FORMAT_DDMMYY; ;}
    break;

  case 663:
#line 4542 "pars.yacc"
    { (yyval.ival) = FORMAT_MMDDYY; ;}
    break;

  case 664:
#line 4543 "pars.yacc"
    { (yyval.ival) = FORMAT_YYMMDD; ;}
    break;

  case 665:
#line 4544 "pars.yacc"
    { (yyval.ival) = FORMAT_MMYY; ;}
    break;

  case 666:
#line 4545 "pars.yacc"
    { (yyval.ival) = FORMAT_MMDD; ;}
    break;

  case 667:
#line 4546 "pars.yacc"
    { (yyval.ival) = FORMAT_MONTHDAY; ;}
    break;

  case 668:
#line 4547 "pars.yacc"
    { (yyval.ival) = FORMAT_DAYMONTH; ;}
    break;

  case 669:
#line 4548 "pars.yacc"
    { (yyval.ival) = FORMAT_MONTHS; ;}
    break;

  case 670:
#line 4549 "pars.yacc"
    { (yyval.ival) = FORMAT_MONTHSY; ;}
    break;

  case 671:
#line 4550 "pars.yacc"
    { (yyval.ival) = FORMAT_MONTHL; ;}
    break;

  case 672:
#line 4551 "pars.yacc"
    { (yyval.ival) = FORMAT_DAYOFWEEKS; ;}
    break;

  case 673:
#line 4552 "pars.yacc"
    { (yyval.ival) = FORMAT_DAYOFWEEKL; ;}
    break;

  case 674:
#line 4553 "pars.yacc"
    { (yyval.ival) = FORMAT_DAYOFYEAR; ;}
    break;

  case 675:
#line 4554 "pars.yacc"
    { (yyval.ival) = FORMAT_HMS; ;}
    break;

  case 676:
#line 4555 "pars.yacc"
    { (yyval.ival) = FORMAT_MMDDHMS; ;}
    break;

  case 677:
#line 4556 "pars.yacc"
    { (yyval.ival) = FORMAT_MMDDYYHMS; ;}
    break;

  case 678:
#line 4557 "pars.yacc"
    { (yyval.ival) = FORMAT_YYMMDDHMS; ;}
    break;

  case 679:
#line 4558 "pars.yacc"
    { (yyval.ival) = FORMAT_DEGREESLON; ;}
    break;

  case 680:
#line 4559 "pars.yacc"
    { (yyval.ival) = FORMAT_DEGREESMMLON; ;}
    break;

  case 681:
#line 4560 "pars.yacc"
    { (yyval.ival) = FORMAT_DEGREESMMSSLON; ;}
    break;

  case 682:
#line 4561 "pars.yacc"
    { (yyval.ival) = FORMAT_MMSSLON; ;}
    break;

  case 683:
#line 4562 "pars.yacc"
    { (yyval.ival) = FORMAT_DEGREESLAT; ;}
    break;

  case 684:
#line 4563 "pars.yacc"
    { (yyval.ival) = FORMAT_DEGREESMMLAT; ;}
    break;

  case 685:
#line 4564 "pars.yacc"
    { (yyval.ival) = FORMAT_DEGREESMMSSLAT; ;}
    break;

  case 686:
#line 4565 "pars.yacc"
    { (yyval.ival) = FORMAT_MMSSLAT; ;}
    break;

  case 687:
#line 4566 "pars.yacc"
    { (yyval.ival) = FORMAT_YYYY; ;}
    break;

  case 688:
#line 4567 "pars.yacc"
    { (yyval.ival) = FORMAT_YYDYHMS; ;}
    break;

  case 689:
#line 4570 "pars.yacc"
    { (yyval.ival) = SIGN_NORMAL; ;}
    break;

  case 690:
#line 4571 "pars.yacc"
    { (yyval.ival) = SIGN_ABSOLUTE; ;}
    break;

  case 691:
#line 4572 "pars.yacc"
    { (yyval.ival) = SIGN_NEGATE; ;}
    break;

  case 692:
#line 4575 "pars.yacc"
    { (yyval.ival) = UP; ;}
    break;

  case 693:
#line 4576 "pars.yacc"
    { (yyval.ival) = DOWN; ;}
    break;

  case 694:
#line 4577 "pars.yacc"
    { (yyval.ival) = RIGHT; ;}
    break;

  case 695:
#line 4578 "pars.yacc"
    { (yyval.ival) = LEFT; ;}
    break;

  case 696:
#line 4579 "pars.yacc"
    { (yyval.ival) = IN; ;}
    break;

  case 697:
#line 4580 "pars.yacc"
    { (yyval.ival) = OUT; ;}
    break;

  case 698:
#line 4583 "pars.yacc"
    { (yyval.ival) = COORD_WORLD; ;}
    break;

  case 699:
#line 4584 "pars.yacc"
    { (yyval.ival) = COORD_VIEW; ;}
    break;

  case 700:
#line 4587 "pars.yacc"
    { (yyval.ival) = DATA_X; ;}
    break;

  case 701:
#line 4588 "pars.yacc"
    { (yyval.ival) = DATA_Y; ;}
    break;

  case 702:
#line 4589 "pars.yacc"
    { (yyval.ival) = DATA_X; ;}
    break;

  case 703:
#line 4590 "pars.yacc"
    { (yyval.ival) = DATA_Y; ;}
    break;

  case 704:
#line 4591 "pars.yacc"
    { (yyval.ival) = DATA_Y1; ;}
    break;

  case 705:
#line 4592 "pars.yacc"
    { (yyval.ival) = DATA_Y2; ;}
    break;

  case 706:
#line 4593 "pars.yacc"
    { (yyval.ival) = DATA_Y3; ;}
    break;

  case 707:
#line 4594 "pars.yacc"
    { (yyval.ival) = DATA_Y4; ;}
    break;

  case 708:
#line 4597 "pars.yacc"
    { (yyval.ival) = ASCENDING; ;}
    break;

  case 709:
#line 4598 "pars.yacc"
    { (yyval.ival) = DESCENDING; ;}
    break;

  case 710:
#line 4601 "pars.yacc"
    { (yyval.ival) = DATA_X; ;}
    break;

  case 711:
#line 4602 "pars.yacc"
    { (yyval.ival) = DATA_Y; ;}
    break;

  case 712:
#line 4605 "pars.yacc"
    { (yyval.ival) = FFT_DFT; ;}
    break;

  case 713:
#line 4606 "pars.yacc"
    { (yyval.ival) = FFT_FFT; ;}
    break;

  case 714:
#line 4607 "pars.yacc"
    { (yyval.ival) = FFT_INVDFT; ;}
    break;

  case 715:
#line 4608 "pars.yacc"
    { (yyval.ival) = FFT_INVFFT; ;}
    break;

  case 716:
#line 4612 "pars.yacc"
    {(yyval.ival)=0;;}
    break;

  case 717:
#line 4613 "pars.yacc"
    {(yyval.ival)=1;;}
    break;

  case 718:
#line 4617 "pars.yacc"
    {(yyval.ival)=0;;}
    break;

  case 719:
#line 4618 "pars.yacc"
    {(yyval.ival)=1;;}
    break;

  case 720:
#line 4619 "pars.yacc"
    {(yyval.ival)=2;;}
    break;

  case 721:
#line 4623 "pars.yacc"
    {(yyval.ival)=0;;}
    break;

  case 722:
#line 4624 "pars.yacc"
    {(yyval.ival)=1;;}
    break;

  case 723:
#line 4625 "pars.yacc"
    {(yyval.ival)=2;;}
    break;

  case 724:
#line 4629 "pars.yacc"
    {(yyval.ival)=0;;}
    break;

  case 725:
#line 4630 "pars.yacc"
    {(yyval.ival)=1;;}
    break;

  case 726:
#line 4631 "pars.yacc"
    {(yyval.ival)=2;;}
    break;

  case 727:
#line 4632 "pars.yacc"
    {(yyval.ival)=3;;}
    break;

  case 728:
#line 4633 "pars.yacc"
    {(yyval.ival)=4;;}
    break;

  case 729:
#line 4634 "pars.yacc"
    {(yyval.ival)=5;;}
    break;

  case 730:
#line 4635 "pars.yacc"
    {(yyval.ival)=6;;}
    break;

  case 731:
#line 4639 "pars.yacc"
    { (yyval.ival) = INTERP_LINEAR; ;}
    break;

  case 732:
#line 4640 "pars.yacc"
    { (yyval.ival) = INTERP_SPLINE; ;}
    break;

  case 733:
#line 4641 "pars.yacc"
    { (yyval.ival) = INTERP_ASPLINE; ;}
    break;

  case 734:
#line 4644 "pars.yacc"
    { (yyval.ival) = MINP; ;}
    break;

  case 735:
#line 4645 "pars.yacc"
    { (yyval.ival) = MAXP; ;}
    break;

  case 736:
#line 4646 "pars.yacc"
    { (yyval.ival) = AVG; ;}
    break;

  case 737:
#line 4647 "pars.yacc"
    { (yyval.ival) = SD; ;}
    break;

  case 738:
#line 4648 "pars.yacc"
    { (yyval.ival) = SUM; ;}
    break;

  case 739:
#line 4649 "pars.yacc"
    { (yyval.ival) = IMIN; ;}
    break;

  case 740:
#line 4650 "pars.yacc"
    { (yyval.ival) = IMAX; ;}
    break;

  case 741:
#line 4655 "pars.yacc"
    {
            (yyval.ival) = get_mapped_font((yyvsp[(2) - (2)].ival));
        ;}
    break;

  case 742:
#line 4659 "pars.yacc"
    {
            (yyval.ival) = get_font_by_name((yyvsp[(2) - (2)].sval));
            xfree((yyvsp[(2) - (2)].sval));
        ;}
    break;

  case 743:
#line 4667 "pars.yacc"
    {
	    int lines = (yyvsp[(2) - (2)].ival);
            if (lines >= 0 && lines < number_of_linestyles()) {
	        (yyval.ival) = lines;
	    } else {
	        errmsg("invalid linestyle");
	        (yyval.ival) = 1;
	    }
        ;}
    break;

  case 744:
#line 4680 "pars.yacc"
    {
	    int patno = (yyvsp[(2) - (2)].ival);
            if (patno >= 0 && patno < number_of_patterns()) {
	        (yyval.ival) = patno;
	    } else {
	        errmsg("invalid pattern number");
	        (yyval.ival) = 1;
	    }
        ;}
    break;

  case 745:
#line 4693 "pars.yacc"
    {
            int c = (yyvsp[(2) - (2)].ival);
            if (c >= 0 && c < (int)number_of_colors()) {
                (yyval.ival) = c;
            } else {
                errmsg("Invalid color ID");
                (yyval.ival) = 1;
            }
        ;}
    break;

  case 746:
#line 4703 "pars.yacc"
    {
            int c = get_color_by_name((yyvsp[(2) - (2)].sval));
            if (c == BAD_COLOR) {
                errmsg("Invalid color name");
                c = 1;
            }
            xfree((yyvsp[(2) - (2)].sval));
            (yyval.ival) = c;
        ;}
    break;

  case 747:
#line 4713 "pars.yacc"
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
        ;}
    break;

  case 748:
#line 4732 "pars.yacc"
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
        ;}
    break;

  case 749:
#line 4747 "pars.yacc"
    {
            (yyval.ival) = (yyvsp[(2) - (2)].ival);
        ;}
    break;

  case 750:
#line 4752 "pars.yacc"
    { (yyval.ival) = PLACEMENT_NORMAL; ;}
    break;

  case 751:
#line 4753 "pars.yacc"
    { (yyval.ival) = PLACEMENT_OPPOSITE; ;}
    break;

  case 752:
#line 4754 "pars.yacc"
    { (yyval.ival) = PLACEMENT_BOTH; ;}
    break;

  case 753:
#line 4760 "pars.yacc"
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
        ;}
    break;

  case 754:
#line 4771 "pars.yacc"
    {
            set_page_dimensions((int) (yyvsp[(3) - (4)].dval), (int) (yyvsp[(4) - (4)].dval), FALSE);
        ;}
    break;

  case 755:
#line 4774 "pars.yacc"
    {
	    scroll_proc((yyvsp[(2) - (2)].ival));
	;}
    break;

  case 756:
#line 4777 "pars.yacc"
    {
	    scrollinout_proc((yyvsp[(3) - (3)].ival));
	;}
    break;

  case 757:
#line 4781 "pars.yacc"
    {
	;}
    break;

  case 758:
#line 4785 "pars.yacc"
    {
	    add_world(whichgraph, (yyvsp[(3) - (17)].dval), (yyvsp[(5) - (17)].dval), (yyvsp[(7) - (17)].dval), (yyvsp[(9) - (17)].dval));
	;}
    break;

  case 759:
#line 4789 "pars.yacc"
    {filltype_obs = (yyvsp[(3) - (3)].ival);;}
    break;

  case 760:
#line 4791 "pars.yacc"
    {filltype_obs = (yyvsp[(3) - (3)].ival);;}
    break;

  case 761:
#line 4793 "pars.yacc"
    { ;}
    break;

  case 762:
#line 4795 "pars.yacc"
    { ;}
    break;

  case 763:
#line 4797 "pars.yacc"
    { ;}
    break;

  case 764:
#line 4798 "pars.yacc"
    { ;}
    break;

  case 765:
#line 4800 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    if ((yyvsp[(3) - (3)].ival) == FALSE && get_project_version() <= 40102) {
                g[whichgraph].l.boxpen.pattern = 0;
            }
	;}
    break;

  case 766:
#line 4809 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.legx = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 767:
#line 4816 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].l.legy = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 768:
#line 4823 "pars.yacc"
    {
	    if (is_valid_setno(whichgraph, (yyvsp[(3) - (4)].ival))) {
                set_legend_string(whichgraph, (yyvsp[(3) - (4)].ival), (yyvsp[(4) - (4)].sval));
	    } else {
                yyerror("Unallocated set");
            }
            xfree((yyvsp[(4) - (4)].sval));
	;}
    break;

  case 769:
#line 4831 "pars.yacc"
    { ;}
    break;

  case 770:
#line 4832 "pars.yacc"
    {filltype_obs = (yyvsp[(5) - (5)].ival);;}
    break;

  case 771:
#line 4833 "pars.yacc"
    { ;}
    break;

  case 772:
#line 4834 "pars.yacc"
    { ;}
    break;

  case 773:
#line 4836 "pars.yacc"
    { ;}
    break;

  case 774:
#line 4838 "pars.yacc"
    { 
	    g[(yyvsp[(1) - (3)].ival)].type = GRAPH_XY;
	    g[(yyvsp[(1) - (3)].ival)].xscale = SCALE_LOG;
	;}
    break;

  case 775:
#line 4842 "pars.yacc"
    { 
	    g[(yyvsp[(1) - (3)].ival)].type = GRAPH_XY;
	    g[(yyvsp[(1) - (3)].ival)].yscale = SCALE_LOG;
	;}
    break;

  case 776:
#line 4847 "pars.yacc"
    { 
	    g[(yyvsp[(1) - (3)].ival)].type = GRAPH_XY;
	    g[(yyvsp[(1) - (3)].ival)].xscale = SCALE_LOG;
	    g[(yyvsp[(1) - (3)].ival)].yscale = SCALE_LOG;
	;}
    break;

  case 777:
#line 4853 "pars.yacc"
    { 
	    g[(yyvsp[(1) - (3)].ival)].type = GRAPH_CHART;
	    g[(yyvsp[(1) - (3)].ival)].xyflip = FALSE;
	    g[(yyvsp[(1) - (3)].ival)].stacked = FALSE;
	;}
    break;

  case 778:
#line 4859 "pars.yacc"
    { 
	    g[(yyvsp[(1) - (3)].ival)].type = GRAPH_CHART;
	    g[(yyvsp[(1) - (3)].ival)].xyflip = TRUE;
	;}
    break;

  case 779:
#line 4864 "pars.yacc"
    { 
	    g[(yyvsp[(1) - (3)].ival)].type = GRAPH_CHART;
	    g[(yyvsp[(1) - (3)].ival)].stacked = TRUE;
	;}
    break;

  case 780:
#line 4869 "pars.yacc"
    { 
	    g[(yyvsp[(1) - (3)].ival)].type = GRAPH_CHART;
	    g[(yyvsp[(1) - (3)].ival)].stacked = TRUE;
	    g[(yyvsp[(1) - (3)].ival)].xyflip = TRUE;
	;}
    break;

  case 781:
#line 4875 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].w.xg1 = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 782:
#line 4882 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].w.xg2 = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 783:
#line 4889 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].w.yg1 = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 784:
#line 4896 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].w.yg2 = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 785:
#line 4904 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].v.xv1 = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 786:
#line 4911 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].v.xv2 = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 787:
#line 4918 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].v.yv1 = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 788:
#line 4925 "pars.yacc"
    {
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
	    g[whichgraph].v.yv2 = (yyvsp[(3) - (3)].dval);
	;}
    break;

  case 789:
#line 4933 "pars.yacc"
    {
	;}
    break;

  case 790:
#line 4936 "pars.yacc"
    { 
	    if (!is_valid_gno(whichgraph)) {
                yyerror("No valid graph selected");
                return 1;
            }
            g[whichgraph].f.fillpen.pattern = (yyvsp[(3) - (3)].ival);
        ;}
    break;

  case 791:
#line 4944 "pars.yacc"
    {
        ;}
    break;

  case 792:
#line 4946 "pars.yacc"
    {
        ;}
    break;

  case 793:
#line 4949 "pars.yacc"
    {
	    line_asize = 2.0*(yyvsp[(4) - (4)].dval);
	;}
    break;

  case 794:
#line 4953 "pars.yacc"
    { ;}
    break;

  case 795:
#line 4954 "pars.yacc"
    { ;}
    break;

  case 796:
#line 4955 "pars.yacc"
    { ;}
    break;

  case 797:
#line 4956 "pars.yacc"
    { ;}
    break;

  case 798:
#line 4961 "pars.yacc"
    { ;}
    break;

  case 799:
#line 4962 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->label_op = (yyvsp[(1) - (1)].ival);
	;}
    break;

  case 800:
#line 4972 "pars.yacc"
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
	;}
    break;

  case 801:
#line 4987 "pars.yacc"
    {
	    g[(yyvsp[(1) - (3)].trgt)->gno].p[(yyvsp[(1) - (3)].trgt)->setno].symskip = (yyvsp[(3) - (3)].ival);
	;}
    break;

  case 802:
#line 4991 "pars.yacc"
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
	;}
    break;

  case 803:
#line 5013 "pars.yacc"
    {
	    g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].errbar.ptype = (yyvsp[(4) - (4)].ival);
	;}
    break;

  case 804:
#line 5021 "pars.yacc"
    { ;}
    break;

  case 805:
#line 5022 "pars.yacc"
    {
	    g[(yyvsp[(1) - (2)].trgt)->gno].p[(yyvsp[(1) - (2)].trgt)->setno].lines = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 806:
#line 5025 "pars.yacc"
    {
	    g[(yyvsp[(1) - (2)].trgt)->gno].p[(yyvsp[(1) - (2)].trgt)->setno].linew = (yyvsp[(2) - (2)].dval);
	;}
    break;

  case 807:
#line 5028 "pars.yacc"
    {
	    g[(yyvsp[(1) - (2)].trgt)->gno].p[(yyvsp[(1) - (2)].trgt)->setno].linepen.color = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 808:
#line 5031 "pars.yacc"
    {filltype_obs = (yyvsp[(4) - (4)].ival);;}
    break;

  case 809:
#line 5032 "pars.yacc"
    { ;}
    break;

  case 810:
#line 5033 "pars.yacc"
    {
            g[(yyvsp[(1) - (4)].trgt)->gno].p[(yyvsp[(1) - (4)].trgt)->setno].errbar.barsize = (yyvsp[(4) - (4)].dval);
	;}
    break;

  case 811:
#line 5036 "pars.yacc"
    { ;}
    break;

  case 812:
#line 5041 "pars.yacc"
    {
	    /* <= xmgr-4.1 */
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->active = (yyvsp[(2) - (2)].ival);
	;}
    break;

  case 813:
#line 5049 "pars.yacc"
    { ;}
    break;

  case 814:
#line 5050 "pars.yacc"
    { ;}
    break;

  case 815:
#line 5051 "pars.yacc"
    { ;}
    break;

  case 816:
#line 5052 "pars.yacc"
    { ;}
    break;

  case 817:
#line 5053 "pars.yacc"
    { ;}
    break;

  case 818:
#line 5054 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_spec = TICKS_SPEC_NONE;
	;}
    break;

  case 819:
#line 5061 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    if (g[whichgraph].t[naxis]->t_spec != TICKS_SPEC_BOTH) {
                g[whichgraph].t[naxis]->t_spec = TICKS_SPEC_MARKS;
            }
	;}
    break;

  case 820:
#line 5070 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    if ((yyvsp[(2) - (2)].dval) != 0.0) {
                g[whichgraph].t[naxis]->nminor = 
                            (int) rint_2(g[whichgraph].t[naxis]->tmajor / (yyvsp[(2) - (2)].dval) - 1);
            } else {
                g[whichgraph].t[naxis]->nminor = 0;
            }
	;}
    break;

  case 821:
#line 5082 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->props.size = (yyvsp[(2) - (2)].dval);
	;}
    break;

  case 822:
#line 5089 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tloc[(yyvsp[(1) - (3)].ival)].wtpos = (yyvsp[(3) - (3)].dval);
	    g[whichgraph].t[naxis]->tloc[(yyvsp[(1) - (3)].ival)].type = TICK_TYPE_MAJOR;
	;}
    break;

  case 823:
#line 5097 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_op = (yyvsp[(1) - (1)].ival);
	;}
    break;

  case 824:
#line 5107 "pars.yacc"
    { ;}
    break;

  case 825:
#line 5108 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    if (g[whichgraph].t[naxis]->t_spec == TICKS_SPEC_BOTH) {
                g[whichgraph].t[naxis]->t_spec = TICKS_SPEC_MARKS;
            }
	;}
    break;

  case 826:
#line 5117 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->t_spec = TICKS_SPEC_BOTH;
	;}
    break;

  case 827:
#line 5124 "pars.yacc"
    { ;}
    break;

  case 828:
#line 5126 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_angle = 0;
	;}
    break;

  case 829:
#line 5133 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_angle = 90;
	;}
    break;

  case 830:
#line 5140 "pars.yacc"
    { ;}
    break;

  case 831:
#line 5141 "pars.yacc"
    { ;}
    break;

  case 832:
#line 5142 "pars.yacc"
    {
	    if (!is_valid_axis(whichgraph, naxis)) {
                yyerror("No valid axis selected");
                return 1;
            }
	    g[whichgraph].t[naxis]->tl_op = (yyvsp[(1) - (1)].ival);
	;}
    break;

  case 833:
#line 5149 "pars.yacc"
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
	;}
    break;

  case 837:
#line 5177 "pars.yacc"
    {
            (yyval.ival) = (yyvsp[(2) - (2)].ival);
        ;}
    break;

  case 838:
#line 5182 "pars.yacc"
    { (yyval.ival) = PLACEMENT_OPPOSITE; ;}
    break;

  case 839:
#line 5183 "pars.yacc"
    { (yyval.ival) = PLACEMENT_NORMAL; ;}
    break;

  case 840:
#line 5184 "pars.yacc"
    { (yyval.ival) = PLACEMENT_NORMAL; ;}
    break;

  case 841:
#line 5185 "pars.yacc"
    { (yyval.ival) = PLACEMENT_OPPOSITE; ;}
    break;

  case 842:
#line 5186 "pars.yacc"
    { (yyval.ival) = PLACEMENT_BOTH; ;}
    break;


/* Line 1267 of yacc.c.  */
#line 13300 "pars.tab.cacc"
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


#line 5189 "pars.yacc"


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
    {"A10", FITPARM, NULL},
    {"A10MAX", FITPMAX, NULL},
    {"A10MIN", FITPMIN, NULL},
    {"A11", FITPARM, NULL},
    {"A11MAX", FITPMAX, NULL},
    {"A11MIN", FITPMIN, NULL},
    {"A12", FITPARM, NULL},
    {"A12MAX", FITPMAX, NULL},
    {"A12MIN", FITPMIN, NULL},
    {"A13", FITPARM, NULL},
    {"A13MAX", FITPMAX, NULL},
    {"A13MIN", FITPMIN, NULL},
    {"A14", FITPARM, NULL},
    {"A14MAX", FITPMAX, NULL},
    {"A14MIN", FITPMIN, NULL},
    {"A15", FITPARM, NULL},
    {"A15MAX", FITPMAX, NULL},
    {"A15MIN", FITPMIN, NULL},
    {"A16", FITPARM, NULL},
    {"A16MAX", FITPMAX, NULL},
    {"A16MIN", FITPMIN, NULL},
    {"A17", FITPARM, NULL},
    {"A17MAX", FITPMAX, NULL},
    {"A17MIN", FITPMIN, NULL},
    {"A18", FITPARM, NULL},
    {"A18MAX", FITPMAX, NULL},
    {"A18MIN", FITPMIN, NULL},
    {"A19", FITPARM, NULL},
    {"A19MAX", FITPMAX, NULL},
    {"A19MIN", FITPMIN, NULL},
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
    {"CEIL", FUNC_D, (void *) ceil_wrap},
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
    {"FLOOR", FUNC_D, (void *) floor_wrap},
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
    {"LOG2", FUNC_D, (void *) log2_wrap},
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
        {"POLARV2", POLARV2, NULL},
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
    {"YYDYHMS", YYDYHMS, NULL},
    {"YYYY", YYYY, NULL},
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
    if ( qtspecial_scanner(s,&error_count) == RETURN_SUCCESS ) return RETURN_SUCCESS;
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

void replace_graph_set_index(char * buffer)/*this just inserts graph and set id*/
{
int i,j;
char set_id[2][16],gr_id[2][16];
int sl[2],gl[2],l;
int state=0;/* 0=left of '=', 1=right of '=' */
sprintf(set_id[1],"%d",current_origin_set);
sprintf(gr_id[1],"%d",current_origin_graph);
sprintf(set_id[0],"%d",current_target_set);
sprintf(gr_id[0],"%d",current_target_graph);
for (i=0;i<2;i++)
{
sl[i]=strlen(set_id[i]);
gl[i]=strlen(gr_id[i]);
}
l=strlen(buffer);
for (i=0;i<l-1;i++)
{
if (buffer[i+1]=='#')
{
    if (buffer[i]=='S' || buffer[i]=='s')/* we have to insert a set-id */
    {
        if (sl[state]<=2)/* we can insert the numbers and shift the rest to the left */
        {
            for (j=0;j<sl[state];j++) buffer[i+j]=set_id[state][j];
            if (sl[state]<2)/* shift to the left */
            for (j=i+2;j<=l;j++) buffer[j-(2-sl[state])]=buffer[j];
        }
        else/* we have to shift everything to the right to make enough space */
        {
            for (j=l;j>=i+2;j--) buffer[j+(sl[state]-2)]=buffer[j];
            for (j=0;j<sl[state];j++) buffer[i+j]=set_id[state][j];
        }
    }
    else if (buffer[i]=='G' || buffer[i]=='g')/* we have to insert a graph-id */
    {
        if (gl[state]<=2)/* we can insert the numbers and shift the rest to the left */
        {
            for (j=0;j<gl[state];j++) buffer[i+j]=gr_id[state][j];
            if (gl[state]<2)/* shift to the left */
            for (j=i+2;j<=l;j++) buffer[j-(2-gl[state])]=buffer[j];
        }
        else/* we have to shift everything to the right to make enough space */
        {
            for (j=l;j>=i+2;j--) buffer[j+(gl[state]-2)]=buffer[j];
            for (j=0;j<gl[state];j++) buffer[i+j]=gr_id[state][j];
        }
    }
}
if (buffer[i]=='=') state=1;
}

}

int scanner(char *s)
{
char * buffer=(char*)malloc(sizeof(char)*(64+2*strlen(s)));
strcpy(buffer,s);
replace_graph_set_index(buffer);
    int retval = parser(buffer, PARSER_TYPE_VOID);
    if (retval != RETURN_SUCCESS) {
        free(buffer);
        return RETURN_FAILURE;
    }
    
    if (gotparams) {
	gotparams = FALSE;
        prepare_strings_for_saving();getparms(paramfile);resume_strings_after_load_or_save();
    }
    
    if (gotread) {
	gotread = FALSE;
        prepare_strings_for_saving();getdata(whichgraph, readfile, cursource, LOAD_SINGLE);resume_strings_after_load_or_save();
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

grarr *get_parser_arr_by_name(const char * const name)
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

    int low, high, mid, c;

    low = 0;
    high = maxfunc - 1;
    while (low <= high) {
	mid = (low + high) / 2;
    c = strcmp(s, keytable[mid].s);
    if (c < 0) {
	    high = mid - 1;
	} else {
        if (c > 0) {
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
    if (pos >= (int)(strlen(f_string)))
	 return EOF;
    return ((int)f_string[pos++]);
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
        if (isdigit(sbuf[2]))
        {
        index=index*10+(sbuf[2] - '0');
        }
		yylval.ival = index;
		return FITPARM;
	    }
	    else if (key[found].type == FITPMAX) {
		int index = sbuf[1] - '0';
        if (isdigit(sbuf[2]))
        {
        index=index*10+(sbuf[2] - '0');
        }
		yylval.ival = index;
		return FITPMAX;
	    }
	    else if (key[found].type == FITPMIN) {
		int index = sbuf[1] - '0';
        if (isdigit(sbuf[2]))
        {
        index=index*10+(sbuf[2] - '0');
        }
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

double rint_2(double v)
{
return (double)( (int)( (v>=0.0) ? (v+0.5) : (v-0.5) ) );
}


