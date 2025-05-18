/*
 * Grace - GRaphing, Advanced Computation and Exploration of data
 *
 * Home page: http://plasma-gate.weizmann.ac.il/Grace/
 *
 * Copyright (c) 1991-1995 Paul J Turner, Portland, OR
 * Copyright (c) 1996-2007 Grace Development Team
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
 * misc utilities
 *
 */

#ifdef _MSC_VER
#include <iostream>
#endif

#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>

#ifdef _MSC_VER
#include <direct.h>
#else
#include <unistd.h>
#endif
#include <string.h>

#ifndef WINDOWS_SYSTEM
#include <pwd.h>
#endif

#ifdef TIME_WITH_SYS_TIME
#  include <sys/time.h>
#  include <time.h>
#else
#  ifdef HAVE_SYS_TIME_H
#    include <sys/time.h>
#  else
#    include <time.h>
#  endif
#endif
#include <signal.h>
#include <sys/types.h>

#ifndef WINDOWS_SYSTEM
#include <sys/resource.h>
#endif

#ifdef HAVE_SYS_SELECT_H
#  include <sys/select.h>
#endif
#include <limits.h>

#ifdef HAVE_SETLOCALE
#  include <locale.h>
#endif

/*#include "buildinfo.h"*/
#define MAJOR_REV 5
#define MINOR_REV 1
#define PATCHLEVEL 22
#define RETSIGTYPE void

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#ifndef WINDOWS_SYSTEM
#include <sys/utsname.h>
#endif

#include <time.h>

#include "globals.h"
#include "utils.h"
#include "files.h"
#include "noxprotos.h"
#include "xprotos.h"
#include "missing.h"
#include "noxprotos.h"
#include "MainWindow.h"
#include "allWidgets.h"
#include "windowWidgets.h"

extern bool useQtFonts;
extern MainWindow * mainWin;
extern frmProgressWin * FormProgress;
extern frmQuestionDialog * FormQuestion;
extern bailoutQuestion * FormBailoutQuestion;
extern frmConsole * FormConsole;
extern int yesnowin(const char * msg,const char * s1,const char * s2,const char * help_anchor);
extern void CheckLaTeXLinesForAddress(char * o_adr,char * n_adr);
extern void initNodes(void);
extern void removeAllWindows(void);
extern void write_settings(void);

//static void rereadConfig(void);
/*static RETSIGTYPE actOnSignal(int signo);*/
static void bugwarn(char *signame);
extern void replaceSuffix(QString & fpath,QString n_suffix);
extern QString getFileNameOnly(QString filepath);

#ifdef DEBUG_OUT_LOG
extern ofstream debug_out;
#endif

#ifndef WINDOWS_SYSTEM
struct utsname u_info;
time_t time_info;
#endif
extern char BI_VERSION[128];
extern char BI_SYSTEM[256];

/*
 * free and check for NULL pointer
 */
void xfree(void *ptr)
{
    if (ptr != NULL) {
        free(ptr);
    }
}

void *xmalloc(size_t size)
{
    void *retval;

    if (size == 0) {
        retval = NULL;
    } else {
        retval = malloc(size);
    }

    if (retval == NULL && size != 0) {
        errmsg("Memory storage exceeded!");
    }
    return retval;
}

void *xcalloc(size_t nmemb, size_t size)
{
    void *retval;

    if (nmemb == 0) {
        retval = NULL;
    } else {
        retval = calloc(nmemb, size);
    }
    
    if (retval == NULL && nmemb != 0) {
        errmsg("Memory storage exceeded!");
    }
    return retval;
}

void *xrealloc(void *ptr, size_t size)
{
    void *retval;

#if defined(REALLOC_IS_BUGGY)
    if (ptr == NULL) {
        retval = malloc(size);
    } else if (size == 0) {
        xfree(ptr);
        retval = NULL;
    } else {
        retval = realloc(ptr, size);
    }
#else
    retval = realloc(ptr, size);
    if (size == 0) {
        retval = NULL;
    }
#endif
    
    if (retval == NULL && size != 0) {
        errmsg("Memory storage exceeded!");
    }
    return retval;
}

/*
 * swap doubles and ints
 */
void fswap(double *x, double *y)
{
    double tmp;

    tmp = *x;
    *x = *y;
    *y = tmp;
}

void iswap(int *x, int *y)
{
    int tmp;

    tmp = *x;
    *x = *y;
    *y = tmp;
}

int isoneof(int c,const char *s)
{
    while (*s) {
        if (c == *s) {
            return 1;
        } else {
            s++;
        }
    }
    return 0;
}

int argmatch(char *s1,const char *s2, int atleast)
{
    int l1 = strlen(s1);
    int l2 = strlen(s2);

    if (l1 < atleast) {
        return 0;
    }
    if (l1 > l2) {
        return 0;
    }
    return (strncmp(s1, s2, l1) == 0);
}

/*
 * convert a string from lower to upper case
 * leaving quoted strings alone
 */
void lowtoupper(char *s)
{
    unsigned int i, quoteon = FALSE;

    for (i = 0; i < strlen(s); i++) {
        if (s[i] == '"') {
            if (!quoteon) {
                quoteon = TRUE;
            } else if ((i > 0) && (s[i-1] != '\\')) {
                quoteon = FALSE;
            }
        }
        if (quoteon == FALSE) {
            if (!isprint(s[i])) {
                s[i] = ' ';
            } else if (s[i] >= 'a' && s[i] <= 'z') {
                s[i] -= ' ';
            }
        }
    }
}

/*
 * remove all that fortran nastiness
 */
void convertchar(char *s)
{
    while (*s++) {
        if (*s == ',')
            *s = ' ';
        if (*s == 'D' || *s == 'd')
            *s = 'e';
    }
}

/*
 * log base 2
 */
int ilog2(int n)
{
    int i = 0;
    int n1 = n;

    while (n1 >>= 1)
        i++;
    if (1 << i != n)
        return -1;
    else
        return i;
}

/*
 * compute the area bounded by the polygon (xi,yi)
 */
double comp_area(int n, double *x, double *y)
{
    int i;
    double sum = 0.0;
if (x==NULL || y==NULL) return 0.0;//for safety!
    for (i = 0; i < n; i++) {
        sum = sum + x[i] * y[(i + 1) % n] - y[i] * x[(i + 1) % n];
    }
    return sum * 0.5;
}

/*
 * compute the perimeter bounded by the polygon (xi,yi)
 */
double comp_perimeter(int n, double *x, double *y)
{
    int i;
    double sum = 0.0;
if (x==NULL || y==NULL) return 0.0;//for safety!
    for (i = 0; i < n - 1; i++) {
        sum = sum + hypot(x[i] - x[(i + 1) % n], y[i] - y[(i + 1) % n]);
    }
    return sum;
}

const char *dayofweekstrs[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char *dayofweekstrl[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const char *monthl[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

int which_day_of_week(const char * day)
{
int ret=-1,i;
for (i=0;i<7;i++)
{
    if (strcmp(dayofweekstrs[i],day)==0 || strcmp(dayofweekstrl[i],day)==0)
    {
    ret=i-1;
    if (ret<0) ret+=7;//because Monday is 0
    break;
    }
}
return ret;
}

int which_month_of_year(const char * month)
{
int ret=-1,i;
for (i=0;i<12;i++)
{
    if (strcmp(months[i],month)==0 || strcmp(monthl[i],month)==0)
    {
    ret=i+1;
    break;
    }
}
return ret;
}

int dayofweek(double j)
{
    int i = (int) floor(j + 1.5);
    return (i <= 0) ? 6 - (6 - i)%7 : i%7;
}

/*
 * escape quotes
 */
char *escapequotes (char *s)
{
    static char *es = NULL;
    int i, k, n, len, elen;
    
    if (s == NULL)
        return NULL;
    
    len = strlen(s);
    es = (char*)xrealloc(es, (len + 1)*sizeof(char));
    strcpy(es, s);
    n = 0;
    while ((es = strchr(es, '\"'))) {
        es++;
        n++;
    }
    
    elen = len + n + 1;
    es = (char*)xrealloc(es, elen*sizeof(char));
    
    i = k = 0;
    while (i < len) {
        if (s[i] == '\"') {
            es[k] = '\\';
            k++;
        }
        es[k] = s[i];
        i++; k++;
    }
    es[elen-1] = '\0';
    return es;
}

int sign(double a)
{
    if (a > 0.0) {
        return +1;
    } else if (a < 0.0) {
        return -1;
    } else {
        return 0;
    }
}

double mytrunc(double a)
{
    if (a > 0.0) {
        return floor(a);
    } else {
        return ceil(a);
    }
}

/*
 * exit grace
 */
int bailout(void)
{
bool ret=false;
int ret2;
#ifdef DEBUG_OUT_LOG
debug_out << "Bailout." << endl;
#endif
if (enableServerMode == TRUE || gracebat == TRUE || !is_dirtystate() || noask)//do not ask
{
ret=true;
}
else
{
///we have to ask here
ret2=yesnosave(0);
#ifdef DEBUG_OUT_LOG
debug_out << "Closing question: return-value=" << ret2 << endl;
#endif
return ret2;
}
    if (!is_dirtystate() || ret)
    {
        if (resfp)
        {
        grace_close(resfp);
        }
        //qApp->quit();
#ifdef DEBUG_OUT_LOG
debug_out << "Just closing QtGrace." << endl;
#endif
        return 1;
    }
#ifdef DEBUG_OUT_LOG
debug_out << "Bailout halted." << endl;
#endif
return 0;
}

/*
 * Reread config (TODO)
 */
#ifdef SIGHUP
static void rereadConfig(void)
{
//getparms("gracerc");
}
#endif

static void please_report_the_bug(void)
{
    fprintf(stderr, "\nPlease use \"Help/Comments\" to report the bug.\n");
    fprintf(stderr, "Please report Bugs in QtGrace to andreas.f.winter@web.de\n");
#ifdef HAVE_LESSTIF
    fprintf(stderr, "NB. This version of Grace was compiled with LessTif.\n");
    fprintf(stderr, "    Make sure to read the FAQ carefully prior to\n");
    fprintf(stderr, "    reporting the bug, ESPECIALLY is the problem might\n");
    fprintf(stderr, "    be related to the graphical interface.\n");
#endif
}

/*
 * Warn about bug (TODO X message)
 */
static void bugwarn(char *signame)
{
    static int emergency_save = FALSE;
    /*
 *  Since we got so far, memory is probably corrupted so it's better to use
 *  a static storage
 */
    static char buf[GR_MAXPATHLEN];
    /* number of interrupts received during the emergency save */
    static int interrupts;
    
    if (emergency_save != FALSE) {
        /* don't mind signals anymore: we're in emergency save mode already */
        interrupts++;
        if (interrupts > 10) {
            fprintf(stderr, "oh, no luck :-(\n");
            please_report_the_bug();
            abort();
        }
        return;
    } else {
        emergency_save = TRUE;
        interrupts = 0;
        fprintf(stderr, "\a\nOops! Got %s\n", signame);
        if (is_dirtystate()) {
            strcpy(buf, get_docname());
            strcat(buf, "$");
            fprintf(stderr, "Trying to save your work into file \"%s\"... ", buf);
            fflush(stderr);
            noask = TRUE;
            if (save_project(buf) == RETURN_SUCCESS) {
                fprintf(stderr, "ok!\n");
            } else {
                fprintf(stderr, "oh, no luck :-(\n");
            }
        }
        please_report_the_bug();
        abort();
    }
}

/*
 * Signal-handling routines
 */
/*Signal-handling*/

RETSIGTYPE actOnSignal(int signo)
{
    char signame[16];

    installSignal();
    
    switch (signo) {
#ifdef SIGHUP
    case SIGHUP:
        rereadConfig();
        break;
#endif
#ifdef SIGINT
    case SIGINT:
#endif
#ifdef SIGQUIT
    case SIGQUIT:
#endif
#ifdef SIGTERM
    case SIGTERM:
#endif
        if (bailout()!=0)
        {
            removeAllWindows();
            write_settings();
            initNodes();//to clear all Contents of the undo-list
            #ifdef DEBUG_OUT_LOG
            debug_out.close();
            #endif
            qApp->exit(0);
        }
        break;
#ifdef SIGILL
    case SIGILL:
        strcpy(signame, "SIGILL");
#endif
#ifdef SIGFPE
    case SIGFPE:
        strcpy(signame, "SIGFPE");
#endif
#ifdef SIGBUS
    case SIGBUS:
        strcpy(signame, "SIGBUS");
#endif
#ifdef SIGSEGV
    case SIGSEGV:
        strcpy(signame, "SIGSEGV");
#endif
#ifdef SIGSYS
    case SIGSYS:
        strcpy(signame, "SIGSYS");
#endif
        bugwarn(signame);
        break;
    default:
        break;
    }
}

void installSignal(void){
#ifdef SIGHUP
    signal(SIGHUP,  actOnSignal);   /// hangup
#endif
#ifdef SIGINT
    signal(SIGINT,  actOnSignal);   /// interrupt
#endif
#ifdef SIGQUIT
    signal(SIGQUIT, actOnSignal);   /// quit
#endif
#ifdef SIGILL
    signal(SIGILL,  actOnSignal);   /// illegal instruction
#endif
#ifdef SIGFPE
    signal(SIGFPE,  actOnSignal);   /// floating point exception
#endif
#ifdef SIGBUS
    signal(SIGBUS,  actOnSignal);   /// bus error
#endif
#ifdef SIGSEGV
    signal(SIGSEGV, actOnSignal);   /// segmentation violation
#endif
#ifdef SIGSYS
    signal(SIGSYS,  actOnSignal);   /// bad argument to system call
#endif
#ifdef SIGTERM
    signal(SIGTERM, actOnSignal);   /// software termination signal
#endif
#ifdef SIGALRM
    signal(SIGALRM, actOnSignal);   /// timer
#endif
#ifdef SIGIO
    signal(SIGIO, actOnSignal);     /// input/output ready
#endif
}


/* create format string */
char *create_fstring(int form, int prec, double loc, int type)
{
static char format[64],c_frac_sec[32];
    //char yformat[16];
    const char * comp_prefix, *eng_prefix;
static char s[MAX_STRING_LENGTH];
    double tmp;
    int m, d, y, h, mm, sec, day_of_year,i_frac_sec;
    double arcmin, arcsec, fracsec;
    int exponent;
    double mantissa;
    int yprec;
    
    if (two_digits_years_allowed()) {
        yprec = 2;
    } else {
        yprec = 4;
    }
    //sprintf(yformat,"%%0%dd",yprec);
    //qDebug() << "YFORMAT=" << yformat;

    /* for locale decimal points */
    set_locale_num(TRUE);

    strcpy(format, "%.*lf");
    switch (form) {
    case FORMAT_DECIMAL:
        sprintf(s, format, prec, loc);
        tmp = atof(s);		/* fix reverse axes problem when loc == -0.0 */
        if (tmp == 0.0) {
            strcpy(format, "%.*lf");
            loc = 0.0;
            sprintf(s, format, prec, loc);
        }
        break;
    case FORMAT_EXPONENTIAL:
        strcpy(format, "%.*le");
        sprintf(s, format, prec, loc);
        tmp = atof(s);		/* fix reverse axes problem when loc == -0.0 */
        if (tmp == 0.0) {
            strcpy(format, "%.*le");
            loc = 0.0;
            sprintf(s, format, prec, loc);
        }
        break;
    case FORMAT_SCIENTIFIC:
        if (loc != 0.0) {
            exponent = (int) floor(log10(fabs(loc)));
            mantissa = loc/pow(10.0, (double) exponent);
            if (type == LFORMAT_TYPE_EXTENDED) {
                strcpy(format, "%.*f\\x\\c4\\C\\f{}10\\S%d\\N");
            } else {
                strcpy(format, "%.*fx10(%d)");
            }
            sprintf(s, format, prec, mantissa, exponent);
        } else {
            strcpy(format, "%.*f");
            sprintf(s, format, prec, 0.0);
        }
        break;
    case FORMAT_COMPUTING:
        /* As per FORMAT_GENERAL but uses computer notation (K,M,G,...)
         * to give the value in multiples of the powers of 1024
         */
        if (loc != 0.0) {
            exponent = (int) floor(log(fabs(loc))/log(2.0));
            if (exponent < 10) {
                exponent = 0;
            } else if (exponent > 80) {
                exponent = 80;
            } else {
                exponent = (int) floor((double) exponent/10)*10;
            }
        } else {
            exponent = 0;
        }

        /* use next prefix if we would get 1024 because
        ** of the print precision requested.  This happens
        ** for values slightly less than 1024.
        */
        sprintf(s, "%.*g", prec, loc/(pow(2.0, exponent)));
        if ((exponent < 80) && (strcmp(s, "1024") == 0)){
            exponent += 10;
        }

        switch (exponent) {
        case 10: /* kilo */
            comp_prefix = "K";
            break;
        case 20: /* Mega */
            comp_prefix = "M";
            break;
        case 30: /* Giga */
            comp_prefix = "G";
            break;
        case 40: /* Tera */
            comp_prefix = "T";
            break;
        case 50: /* Peta */
            comp_prefix = "P";
            break;
        case 60: /* Exa */
            comp_prefix = "E";
            break;
        case 70: /* Zetta */
            comp_prefix = "Z";
            break;
        case 80: /* Yotta */
            comp_prefix = "Y";
            break;
        default:
            comp_prefix = "";
            break;
        }
        sprintf(s,"%.*g%s", prec, loc/(pow(2.0, exponent)), comp_prefix);
        tmp = atof(s);          /* fix reverse axes problem when loc == -0.0 */
        if (tmp == 0.0) {
            strcpy(format, "%lg");
            loc = 0.0;
            sprintf(s, format, loc);
        }
        break;
    case FORMAT_ENGINEERING:
        if (loc != 0.0) {
            exponent = (int) floor(log10(fabs(loc)));
            if (exponent < -24) {
                exponent = -24;
            } else if (exponent > 24) {
                exponent = 24;
            } else {
                exponent = (int) floor((double) exponent/3)*3;
            }
        } else {
            exponent = 0;
        }
        switch (exponent) {
        case -24: /* yocto */
            eng_prefix = "y";
            break;
        case -21: /* zepto */
            eng_prefix = "z";
            break;
        case -18: /* atto */
            eng_prefix = "a";
            break;
        case -15: /* fempto */
            eng_prefix = "f";
            break;
        case -12: /* pico */
            eng_prefix = "p";
            break;
        case -9: /* nano */
            eng_prefix = "n";
            break;
        case -6: /* micro */
            if (type == LFORMAT_TYPE_EXTENDED) {
                if (useQtFonts==true)
                    eng_prefix = "\\xl\\f{} ";
                else
                    eng_prefix = "\\xm\\f{}";
            } else {
                eng_prefix = "mk";
            }
            break;
        case -3: /* milli */
            eng_prefix = "m";
            break;
        case 3: /* kilo */
            eng_prefix = "k";
            break;
        case 6: /* Mega */
            eng_prefix = "M";
            break;
        case 9: /* Giga */
            eng_prefix = "G";
            break;
        case 12: /* Tera */
            eng_prefix = "T";
            break;
        case 15: /* Peta */
            eng_prefix = "P";
            break;
        case 18: /* Exa */
            eng_prefix = "E";
            break;
        case 21: /* Zetta */
            eng_prefix = "Z";
            break;
        case 24: /* Yotta */
            eng_prefix = "Y";
            break;
        default:
            eng_prefix = "";
            break;
        }
        strcpy(format, "%.*f %s");
        sprintf(s, format, prec, loc/(pow(10.0, exponent)), eng_prefix);
        break;
    case FORMAT_POWER:
        if (loc < 0.0) {
            loc = log10(-loc);
            if (type == LFORMAT_TYPE_EXTENDED) {
                strcpy(format, "-10\\S%.*lf\\N");
            } else {
                strcpy(format, "-10(%.*lf)");
            }
        } else if (loc == 0.0) {
            sprintf(format, "%.*f", prec, 0.0);
        } else {
            loc = log10(loc);
            if (type == LFORMAT_TYPE_EXTENDED) {
                strcpy(format, "10\\S%.*lf\\N");
            } else {
                strcpy(format, "10(%.*lf)");
            }
        }
        sprintf(s, format, prec, loc);
        break;
    case FORMAT_GENERAL:
        strcpy(format, "%.*lg");
        sprintf(s, format, prec, loc);
        tmp = atof(s);
        if (tmp == 0.0) {
            strcpy(format, "%lg");
            loc = 0.0;
            sprintf(s, format, loc);
        }
        break;
    case FORMAT_DDMMYY:
            if (current_date_separator==DATE_SEPARATOR_MINUS)
            strcpy(format, "%02d-%02d-%0*d");
            else if (current_date_separator==DATE_SEPARATOR_DOT)
            strcpy(format, "%02d.%02d.%0*d");
            else
            strcpy(format, "%02d/%02d/%0*d");
        jul_to_cal_and_time(loc, ROUND_DAY, &y, &m, &d, &h, &mm, &sec);
        sprintf(s, format, d, m, yprec, y);
        break;
    case FORMAT_MMDDYY:
            if (current_date_separator==DATE_SEPARATOR_MINUS)
            strcpy(format, "%02d-%02d-%0*d");
            else if (current_date_separator==DATE_SEPARATOR_DOT)
            strcpy(format, "%02d.%02d.%0*d");
            else
            strcpy(format, "%02d/%02d/%0*d");
        jul_to_cal_and_time(loc, ROUND_DAY, &y, &m, &d, &h, &mm, &sec);
        sprintf(s, format, m, d, yprec, y);
        break;
    case FORMAT_YYMMDD:
            if (current_date_separator==DATE_SEPARATOR_MINUS)
            strcpy(format, "%0*d-%02d-%02d");
            else if (current_date_separator==DATE_SEPARATOR_DOT)
            strcpy(format, "%0*d.%02d.%02d");
            else
            strcpy(format, "%0*d/%02d/%02d");
        jul_to_cal_and_time(loc, ROUND_DAY, &y, &m, &d, &h, &mm, &sec);
        sprintf(s, format, yprec, y, m, d);
        break;
    case FORMAT_MMYY:
            if (current_date_separator==DATE_SEPARATOR_MINUS)
            strcpy(format, "%02d-%0*d");
            else if (current_date_separator==DATE_SEPARATOR_DOT)
            strcpy(format, "%02d.%0*d");
            else
            strcpy(format, "%02d/%0*d");
        jul_to_cal_and_time(loc, ROUND_MONTH, &y, &m, &d, &h, &mm, &sec);
        sprintf(s, format, m, yprec, y);
        break;
    case FORMAT_MMDD:
            if (current_date_separator==DATE_SEPARATOR_MINUS)
            strcpy(format, "%02d-%02d");
            else if (current_date_separator==DATE_SEPARATOR_DOT)
            strcpy(format, "%02d.%02d");
            else
            strcpy(format, "%02d/%02d");
        jul_to_cal_and_time(loc, ROUND_DAY, &y, &m, &d, &h, &mm, &sec);
        sprintf(s, format, m, d);
        break;
    case FORMAT_MONTHDAY:
            if (current_date_separator==DATE_SEPARATOR_MINUS)
            strcpy(format, "%s-%02d");
            else if (current_date_separator==DATE_SEPARATOR_DOT)
            strcpy(format, "%s.%02d");
            else
            strcpy(format, "%s/%02d");
        jul_to_cal_and_time(loc, ROUND_DAY, &y, &m, &d, &h, &mm, &sec);
        if (m - 1 < 0 || m - 1 > 11) {
            sprintf(s, format, "???");
        } else {
            sprintf(s, format, months[m - 1], d);
        }
        break;
    case FORMAT_DAYMONTH:
            if (current_date_separator==DATE_SEPARATOR_MINUS)
            strcpy(format, "%02d-%s");
            else if (current_date_separator==DATE_SEPARATOR_DOT)
            strcpy(format, "%02d.%s");
            else
            strcpy(format, "%02d/%s");
        jul_to_cal_and_time(loc, ROUND_DAY, &y, &m, &d, &h, &mm, &sec);
        if (m - 1 < 0 || m - 1 > 11) {
            sprintf(s, format, "???");
        } else {
            sprintf(s, format, d, months[m - 1]);
        }
        break;
    case FORMAT_MONTHS:
        strcpy(format, "%s");
        jul_to_cal_and_time(loc, ROUND_MONTH, &y, &m, &d, &h, &mm, &sec);
        if (m - 1 < 0 || m - 1 > 11) {
            sprintf(s, format, "???");
        } else {
            sprintf(s, format, months[m - 1]);
        }
        break;
    case FORMAT_MONTHSY:
            if (current_date_separator==DATE_SEPARATOR_MINUS)
            strcpy(format, "%s-%0*d");
            else if (current_date_separator==DATE_SEPARATOR_DOT)
            strcpy(format, "%s.%0*d");
            else
            strcpy(format, "%s/%0*d");
        jul_to_cal_and_time(loc, ROUND_MONTH, &y, &m, &d, &h, &mm, &sec);
        if (m - 1 < 0 || m - 1 > 11) {
            sprintf(s, format, "???");
        } else {
            sprintf(s, format, months[m - 1], yprec, y);
        }
        break;
    case FORMAT_MONTHL:
        strcpy(format, "%s");
        jul_to_cal_and_time(loc, ROUND_MONTH, &y, &m, &d, &h, &mm, &sec);
        if (m - 1 < 0 || m - 1 > 11) {
            sprintf(s, format, "???");
        } else {
            sprintf(s, format, monthl[m - 1]);
        }
        break;
    case FORMAT_YYYY:
        jul_to_cal_and_time(loc, ROUND_MONTH, &y, &m, &d, &h, &mm, &sec);
        sprintf(s, "%4d", y);
        break;
    case FORMAT_DAYOFWEEKS:
        strcpy(format, "%s");
        sprintf(s, format, dayofweekstrs[dayofweek(loc + get_ref_date())]);
        break;
    case FORMAT_DAYOFWEEKL:
        strcpy(format, "%s");
        sprintf(s, format, dayofweekstrl[dayofweek(loc + get_ref_date())]);
        break;
    case FORMAT_DAYOFYEAR:
        strcpy(format, "%d");
        jul_to_cal_and_time(loc, ROUND_DAY, &y, &m, &d, &h, &mm, &sec);
        sprintf(s, format,
                1 + (int) (cal_to_jul(y, m, d) - cal_to_jul(y, 1, 1)));
        break;
#define MSECS(x) (((int) floor(x * 1000.0)) % 1000) //msec
#define SECS_FMT "%02d.%03d"
#define HMS_FMT "%02d:%02d:" SECS_FMT
#define HMS_FMT_S "%02d:%02d:%02d"
    case FORMAT_HMS:
        //strcpy(format, HMS_FMT);
        jul_to_cal_and_time_with_frac(loc, -prec, &y, &m, &d, &h, &mm, &sec, &fracsec);
        if (prec==0)//no fractions of seconds
        {
        strcpy(format, "%02d:%02d:%02d");
        sprintf(s, format, h, mm, sec);
        }
        else//format includes fractions of seconds
        {
        i_frac_sec=(((int) floor(fracsec * pow(10.0,1.0*prec))) % ((int)pow(10.0,1.0*prec)));
        sprintf(c_frac_sec,"%s.%%0%dd","%02d:%02d:%02d",prec);
        strcpy(format, c_frac_sec);
        sprintf(s, format, h, mm, sec, i_frac_sec);
        }
        break;
    case FORMAT_MMDDHMS:
            if (current_date_separator==DATE_SEPARATOR_MINUS)
            strcpy(format, "%02d-%02d " HMS_FMT_S);
            else if (current_date_separator==DATE_SEPARATOR_DOT)
            strcpy(format, "%02d.%02d " HMS_FMT_S);
            else
            strcpy(format, "%02d/%02d " HMS_FMT_S);
        jul_to_cal_and_time_with_frac(loc, NO_ROUND, &y, &m, &d, &h, &mm, &sec, &fracsec);
        sprintf(s, format, m, d, h, mm, sec, MSECS(fracsec));
        break;
    case FORMAT_MMDDYYHMS:
            if (current_date_separator==DATE_SEPARATOR_MINUS)
            strcpy(format, "%02d-%02d-%0*d " HMS_FMT_S);
            else if (current_date_separator==DATE_SEPARATOR_DOT)
            strcpy(format, "%02d.%02d.%0*d " HMS_FMT_S);
            else
            strcpy(format, "%02d/%02d/%0*d " HMS_FMT_S);
        jul_to_cal_and_time_with_frac(loc, NO_ROUND, &y, &m, &d, &h, &mm, &sec, &fracsec);
        sprintf(s, format, m, d, yprec, y, h, mm, sec, MSECS(fracsec));
        break;
    case FORMAT_YYMMDDHMS:
            if (current_date_separator==DATE_SEPARATOR_MINUS)
            strcpy(format, "%0*d-%02d-%02d " HMS_FMT_S);
            else if (current_date_separator==DATE_SEPARATOR_DOT)
            strcpy(format, "%0*d.%02d.%02d " HMS_FMT_S);
            else
            strcpy(format, "%0*d/%02d/%02d " HMS_FMT_S);
        jul_to_cal_and_time_with_frac(loc, NO_ROUND, &y, &m, &d, &h, &mm, &sec, &fracsec);
        /*	secloc = loc / (24.0*3600.0);
            msec = ((int)((secloc - floor(secloc))* 1000.0)) % 1000;*/
        sprintf(s, format, yprec, y, m, d, h, mm, sec, MSECS(fracsec));
        break;
    case FORMAT_YYDYHMS:
            if (current_date_separator==DATE_SEPARATOR_MINUS)
            strcpy(format, "%0*d-%03d-" HMS_FMT);/*Warning: this will show seconds and milliseconds*/
            else if (current_date_separator==DATE_SEPARATOR_DOT)
            strcpy(format, "%0*d.%03d." HMS_FMT);/*Warning: this will show seconds and milliseconds*/
            else
            strcpy(format, "%0*d/%03d/" HMS_FMT);/*Warning: this will show seconds and milliseconds*/
        jul_to_cal_and_time_with_yday(loc, NO_ROUND, &y, &m, &d, &h, &mm, &sec, &fracsec, &day_of_year);
        sprintf(s, format, yprec, y, day_of_year, h, mm, sec, MSECS(fracsec));
        break;
    case FORMAT_DEGREESLON:
        if (loc < 0.0) {
            loc *= -1.0;
            strcpy(format, "%.*lfW");
        } else if (loc > 0.0) {
            strcpy(format, "%.*lfE");
        } else {
            strcpy(format, "0");
        }
        sprintf(s, format, prec, loc);
        break;
    case FORMAT_DEGREESMMLON:
        if (loc < 0.0) {
            loc *= -1.0;
            strcpy(format, "%d %.*lf' W");
        } else if (loc > 0.0) {
            strcpy(format, "%d %.*lf' E");
        } else {
            strcpy(format, "0 0'");
        }
        y = loc;
        arcmin = (loc - y) * 60.0;
        sprintf(s, format, y, prec, arcmin);
        break;
    case FORMAT_DEGREESMMSSLON:
        if (loc < 0.0) {
            loc *= -1.0;
            strcpy(format, "%d %d' %.*lf\" W");
        } else if (loc > 0.0) {
            strcpy(format, "%d %d' %.*lf\" E");
        } else {
            strcpy(format, "0 0' 0\"");
        }
        y = loc;
        arcsec = (loc - y) * 3600.0;
        m = arcsec / 60.0;
        arcsec = (arcsec - m * 60);
        sprintf(s, format, y, m, prec, arcsec);
        break;
    case FORMAT_MMSSLON:
        if (loc < 0.0) {
            loc *= -1.0;
            strcpy(format, "%d' %.*lf\" W");
        } else if (loc > 0.0) {
            strcpy(format, "%d' %.*lf\" E");
        } else {
            strcpy(format, "0 0' 0\"");
        }
        y = loc;
        arcsec = (loc - y) * 3600.0;
        m = arcsec / 60.0;
        arcsec = (arcsec - m * 60);
        sprintf(s, format, m, prec, arcsec);
        break;
    case FORMAT_DEGREESLAT:
        if (loc < 0.0) {
            loc *= -1.0;
            strcpy(format, "%.*lfS");
        } else if (loc > 0.0) {
            strcpy(format, "%.*lfN");
        } else {
            strcpy(format, "0");
        }
        sprintf(s, format, prec, loc);
        break;
    case FORMAT_DEGREESMMLAT:
        if (loc < 0.0) {
            loc *= -1.0;
            strcpy(format, "%d %.*lf' S");
        } else if (loc > 0.0) {
            strcpy(format, "%d %.*lf' N");
        } else {
            strcpy(format, "0 0'");
        }
        y = loc;
        arcsec = (loc - y) * 60.0;
        sprintf(s, format, y, prec, arcsec);
        break;
    case FORMAT_DEGREESMMSSLAT:
        if (loc < 0.0) {
            loc *= -1.0;
            strcpy(format, "%d %d' %.*lf\" S");
        } else if (loc > 0.0) {
            strcpy(format, "%d %d' %.*lf\" N");
        } else {
            strcpy(format, "0 0' 0\"");
        }
        y = loc;
        arcsec = (loc - y) * 3600.0;
        m = arcsec / 60.0;
        arcsec = (arcsec - m * 60);
        sprintf(s, format, y, m, prec, arcsec);
        break;
    case FORMAT_MMSSLAT:
        if (loc < 0.0) {
            loc *= -1.0;
            strcpy(format, "%d' %.*lf\" S");
        } else if (loc > 0.0) {
            strcpy(format, "%d' %.*lf\" N");
        } else {
            strcpy(format, "0 0' 0\"");
        }
        y = loc;
        arcsec = (loc - y) * 3600.0;
        m = arcsec / 60.0;
        arcsec = (arcsec - m * 60);
        sprintf(s, format, m, prec, arcsec);
        break;
    default:
        sprintf(s, format, prec, loc);
        break;
    }

    /*if (current_date_separator!=DATE_SEPARATOR_MINUS)
    {
    QString tmp_s(s);
        switch(form)
        {
        default:
        ;
        break;
        case FORMAT_DDMMYY:
        case FORMAT_MMDDYY:
        case FORMAT_YYMMDD:
        case FORMAT_MMYY:
        case FORMAT_MMDD:
        case FORMAT_MONTHDAY:
        case FORMAT_DAYMONTH:
        case FORMAT_MONTHSY:
        case FORMAT_MMDDHMS:
        case FORMAT_MMDDYYHMS:
        case FORMAT_YYMMDDHMS:
            if (current_date_separator==DATE_SEPARATOR_DASH)
            {
            tmp_s=tmp_s.replace("-","/");
            }
            else//.
            {
            tmp_s=tmp_s.replace("-",".");
            }
        strcpy(s,tmp_s.toLocal8Bit().constData());
        break;
        }
    }*/

    /* revert to POSIX */
    set_locale_num(FALSE);
    
    return(s);
}

int bin_dump(char *value, int i, int pad)
{
    char *word;
    
    if (i > pad - 1) {
        return 0;
    }
    
    word = value;
    
#ifdef WORDS_BIGENDIAN
    return (((*word)>>i)&0x01);
#else
    switch (pad) {
    case 8:
        return (((*word)>>i)&0x01);
        break;
    case 16:
        if (i < 8) {
            word++;
            return (((*word)>>i)&0x01);
        } else {
            return (((*word)>>(8 - i))&0x01);
        }
        break;
    case 32:
        if (i < 8) {
            word += 2;
            return (((*word)>>i)&0x01);
        } else if (i < 16) {
            word++;
            return (((*word)>>(8 - i))&0x01);
        } else {
            return (((*word)>>(16 - i))&0x01);
        }
        break;
    default:
        return 0;
    }
#endif
}

unsigned char reversebits(unsigned char inword)
{
    int i;
    unsigned char result = 0;
    
    for (i = 0; i <= 7; i++) {
        result |= (((inword)>>i)&0x01)<<(7 - i);
    }
    
    return (result);
}

char *copy_string(char *dest, const char *src)
{
static char * old_dest;
    old_dest=dest;
    if (src == dest)
    {
        ;
    }
    else if (src == NULL)
    {
        xfree(dest);
        dest = NULL;
    }
    else
    {
        dest = (char*)xrealloc(dest, (strlen(src) + 1)*sizeof(char));
        strcpy(dest, src);
    }
    CheckLaTeXLinesForAddress(old_dest,dest);
    return(dest);
}

char *concat_strings(char *dest, const char *src)
{
    static char * old_dest;
    if (src != NULL)
    {
        old_dest=dest;
        if (dest == NULL)
        {
            dest = copy_string(NULL, src);
        }
        else
        {
            dest = (char*)xrealloc(dest, (strlen(dest) + strlen(src) + 1)*sizeof(char));
            if (dest != NULL)
            {
                strcat(dest, src);
            }
        }
        CheckLaTeXLinesForAddress(old_dest,dest);
    }
    return(dest);
}

int compare_strings(const char *s1, const char *s2)
{
    if (s1 == NULL && s2 == NULL) {
        return TRUE;
    } else if (s1 == NULL || s2 == NULL) {
        return FALSE;
    } else {
        return (strcmp(s1, s2) == 0);
    }
}

/* location of Grace home directory */
static char grace_home[GR_MAXPATHLEN] = GRACE_HOME;	

char *get_grace_home(void)
{
    return grace_home;
}

void set_grace_home(const char *dir)
{
    strncpy(grace_home, dir, GR_MAXPATHLEN - 1);
    grace_home[GR_MAXPATHLEN - 1]='\0';
}

/* print command */
char print_cmd[GR_MAXPATHLEN] = GRACE_PRINT_CMD;

char *get_print_cmd(void)
{
    return print_cmd;
}

void set_print_cmd(const char *cmd)
{
    strncpy(print_cmd, cmd, GR_MAXPATHLEN - 1);
    print_cmd[GR_MAXPATHLEN - 1]='\0';
}

/* editor */
static char grace_editor[GR_MAXPATHLEN] = GRACE_EDITOR;	

char *get_editor(void)
{
    return grace_editor;
}

void set_editor(const char *cmd)
{
    strncpy(grace_editor, cmd, GR_MAXPATHLEN - 1);
    grace_editor[GR_MAXPATHLEN - 1]='\0';
}

static char help_viewer[GR_MAXPATHLEN] = GRACE_HELPVIEWER;	

char *get_help_viewer(void)
{
    return help_viewer;
}

void set_help_viewer(const char *dir)
{
    strncpy(help_viewer, dir, GR_MAXPATHLEN - 1);
    help_viewer[GR_MAXPATHLEN - 1]='\0';
}

/* project file name */
static char docname[GR_MAXPATHLEN] = NONAME;	
static char exportname[GR_MAXPATHLEN] = NONAME;

char *get_exportname(void)
{
    return exportname;
}

char *get_exportbname(void)
{
static char buf[GR_MAXPATHLEN];
char *bufp;
    strcpy(buf, mybasename(exportname));
    bufp = strrchr(buf, '.');
    if (bufp)
    {
    *(bufp) = '\0';
    }
return buf;
}

char *get_exportfilename(void)
{
static char buf[GR_MAXPATHLEN];
strcpy(buf, mybasename(exportname));
return buf;
}

void set_exportname(const char *s)
{
    if (s != NULL) {
        strncpy(exportname, s, GR_MAXPATHLEN - 1);
        exportname[GR_MAXPATHLEN - 1]='\0';
    } else {
        strcpy(exportname, NONAME);
    }
    strcpy(print_file,exportname);
}

char *get_docname(void)
{
    return docname;
}

char *get_docfilename(void)
{
static char buf[GR_MAXPATHLEN];
strcpy(buf, mybasename(docname));
return buf;
}

char *get_docbname(void)
{
static char buf[GR_MAXPATHLEN];
char *bufp;
    strcpy(buf, mybasename(docname));
    bufp = strrchr(buf, '.');
    if (bufp)
    {
    *(bufp) = '\0';
    }
return buf;
}

void set_docname(const char *s)
{
    if (s != NULL) {
        strncpy(docname, s, GR_MAXPATHLEN - 1);
        docname[GR_MAXPATHLEN - 1]='\0';
    QString pf1(QString::fromLocal8Bit(s));
    Device_entry dev = get_device_props(hdevice);
    replaceSuffix(pf1,QString(dev.fext));

    set_exportname(pf1.toLocal8Bit().constData());

    } else {
        strcpy(docname, NONAME);
        set_exportname(NONAME);
    }
}

void errmsg(const char *buf)
{
//qDebug() << "errmsg:" << buf;
    /*if (!inwin)
    {*/
    fprintf(stderr, "%s\n", buf);
#ifdef DEBUG_OUT_LOG
debug_out << "ERROR-MESSAGE: " << buf << endl;
#endif
    //}
if (gracebat==TRUE || startupphase) return;//no graphical messages for gracebat or during startup
#ifndef NONE_GUI
    if (disableConsole)
    {
        if (FormConsole==NULL)
        {
            FormConsole=new frmConsole(mainWin);
        }
//qDebug() << "A Console-Size" << FormConsole->size();
        FormConsole->show();
        FormConsole->raise();
        FormConsole->errwin(buf);
        /// qApp->processEvents();
//qDebug() << "B Console-Size" << FormConsole->size();
    }
    /*else {
        fprintf(stderr, "%s\n", buf);
    }*/
#endif
}

int yesnoterm(const char *msg)
{
    (void)msg;
    return 1;
}

int yesnosave(int version)//version=0 --> Exit / version=1 --> just load something else
{
bool save_on=false,saveas_on=false;
save_no_ask=noask;
if (is_dirtystate())//unsaved data
{
saveas_on=true;
if (strcmp(get_docname(),NONAME)!=0) save_on=true;//the project already has a name, offer to save it under that name
}
int ret0=FormBailoutQuestion->askBailout((version==0?true:false),save_on,saveas_on);
switch (ret0)
{
case 0://OK - Yes
    noask=save_no_ask;
    return ANSWER_YES;
break;
default:
case 1://Cancel - No
    noask=save_no_ask;
    return ANSWER_NO;
break;
case 2://Save
        if (version==0)//exit
        mainWin->future_load_status=3;
        else//load
        mainWin->future_load_status=1;
    mainWin->Save();
    noask=save_no_ask;
    return ANSWER_YES;
break;
case 3://SaveAs
        if (version==0)//exit
        mainWin->future_load_status=3;
        else//load
        mainWin->future_load_status=2;
    mainWin->SaveAs();
    noask=save_no_ask;
    return ANSWER_NO;
break;
}
///function ends here!
/*
//int ret=QMessageBox::question(0,QString("Error"),QString(msg),QMessageBox::Yes | QMessageBox::No | QMessageBox::Save,QMessageBox::No);
int ret2;
if (general_behavior==0)//like Grace
{
    if (version==0)
    ret2=QMessageBox::question(mainWin,QObject::tr("Exit QtGrace"),QObject::tr("Exit losing unsaved changes?"),QMessageBox::Yes|QMessageBox::No|QMessageBox::Save,QMessageBox::No);
    else
    ret2=QMessageBox::question(mainWin,QObject::tr("Close project"),QObject::tr("Abandon unsaved changes?"),QMessageBox::Yes|QMessageBox::No|QMessageBox::Save,QMessageBox::No);

    if (ret2==QMessageBox::Save)
    {
    ret2=ANSWER_SAVE;
    }
    else if (ret2==QMessageBox::Yes)
    {
    ret2=ANSWER_YES;
    }
    else
    {
    ret2=ANSWER_NO;
    }
}
else//like QtGrace
{
    if (version==0)
    ret2=QMessageBox::question(mainWin,QObject::tr("Exit QtGrace"),QObject::tr("Content of current project changed!\nSave project?"),QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel,QMessageBox::Cancel);
    else
    ret2=QMessageBox::question(mainWin,QObject::tr("Close project"),QObject::tr("Content of current project changed!\nSave project?"),QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel,QMessageBox::Cancel);

    if (ret2==QMessageBox::Save)
    {
    ret2=ANSWER_SAVE;
    }
    else if (ret2==QMessageBox::Discard)//"discard" means "Yes, Exit without saving!"
    {
    ret2=ANSWER_YES;
    }
    else
    {
    ret2=ANSWER_NO;
    }
}

if (ret2==ANSWER_SAVE)
{
//cout << "getDocName=" << get_docname() << endl;
QString sav_file_name=QFileDialog::getSaveFileName(mainWin,QObject::tr("Save project file"),get_docfilename(),QObject::tr("Project files (*.agr)"));
    if (!sav_file_name.isEmpty())
    {
    int noask_save = noask;
    noask=1;
    //cout << "Yes to save: " << sav_file_name.toLocal8Bit().constData() << endl;
    save_project(sav_file_name.toLocal8Bit().data());
    noask=noask_save;
    return ANSWER_YES;
    }
    else
    {
    //cout << "Not To Save:" << sav_file_name.toLocal8Bit().constData() << endl;
    return ANSWER_NO;
    }
}
else
{
return ret2;
}*/

/*if (ret==QMessageBox::Yes)
    return 1;
else if (ret==QMessageBox::No)
    return 0;
else
{
QString question;
question=QObject::tr("Overwrite file ");
question+=QString(get_docname());
question+=QString("?");
    if (yesno(question.toLocal8Bit().data(),NULL,NULL,NULL))
    {
    mainWin->Save();
    return -1;
    }
    else
    {
    return 0;
    }
}*/

}

int yesno(const char *msg,const char *s1,const char *s2,const char *help_anchor)
{
    if (noask)
    {
        //cout << "oops... noask=true" << endl;
        return 1;
    }
    return yesnowin(msg, s1, s2, help_anchor);
    /*#ifdef NONE_GUI
    return (yesnoterm(msg));
#else
    if (inwin) {
        return (yesnowin(msg, s1, s2, help_anchor));
    } else {
        return (yesnoterm(msg));
    }
#endif*/
}

void stufftext(const char *s)
{
    /*#ifdef NONE_GUI
    printf(s);
#else*/
    //if (inwin) {
if (gracebat==TRUE) return;
    if (FormConsole==NULL)
    {
        FormConsole=new frmConsole(mainWin);
    }
    FormConsole->show();
    FormConsole->raise();
    FormConsole->msgwin(s);
    qApp->processEvents();
    /*        stufftextwin(s);*/
    /*} else {
        printf(s);
    }*/
    //#endif
    /* log results to file */
    if (resfp != NULL) {
        fprintf(resfp,"%s", s);
    }
}


char *mybasename(const char *s)
{
static char basename[GR_MAXPATHLEN];
QFileInfo fi(QString::fromLocal8Bit(s));
strcpy(basename,fi.fileName().toLocal8Bit().data());
return basename;
//FUNCTION ENDS HERE
/*
    int start, end;
    char seperator=QDir::separator().toLatin1();
    //s = path_translate(s);
    if (s == NULL) {
        errmsg("Could not translate basename:");
        return "???";
    }
    
    end = strlen(s) - 1;
    
    // root is a special case
    if (end == 0 && *s == seperator){
        basename[0] = seperator;
        return basename;
    }

    // strip trailing white space and slashes
    while (s[end] == seperator || s[end] == ' ' || s[end] == '\t') {
        end--;
    }
    // find start of basename
    start = end;
    do {
        start--;
    } while (start >= 0 && s[start] != seperator);

    strncpy(basename, s + (start + 1), end - start);
    basename[end - start] = '\0';
    return basename;*/
}

static char workingdir[GR_MAXPATHLEN];

#ifdef _MSC_VER
#define chdir _chdir
#define getcwd _getcwd
#endif

int set_workingdir(const char *wd)
{
    char buf[GR_MAXPATHLEN];
    
    if (wd == NULL) {
        (void)getcwd(workingdir, GR_MAXPATHLEN - 1);
        if (workingdir[strlen(workingdir)-1] != QDir::separator().toLatin1()) {
            //strcat(workingdir, QDir::separator().toLatin1());
            workingdir[strlen(workingdir)+1]='\0';
            workingdir[strlen(workingdir)]=QDir::separator().toLatin1();
        }
        return RETURN_SUCCESS;
    }
    
        strncpy(buf, wd, GR_MAXPATHLEN - 1);
        buf[GR_MAXPATHLEN - 1]='\0';
    if (buf[0] == '~') {
        expand_tilde(buf);
    }
    if (chdir(buf) >= 0) {
        strncpy(workingdir, buf, GR_MAXPATHLEN - 1);
        workingdir[GR_MAXPATHLEN - 1]='\0';
        if (workingdir[strlen(workingdir)-1] != QDir::separator().toLatin1()) {
            //strcat(workingdir, QDir::separator().toLatin1());
            workingdir[strlen(workingdir)+1]='\0';
            workingdir[strlen(workingdir)]=QDir::separator().toLatin1();
        }
        return RETURN_SUCCESS;
    } else {
        return RETURN_FAILURE;
    }
}

char *get_workingdir(void)
{
    return workingdir;
}

static char *username = NULL;

void init_username(void)
{
const char *s;
/*
 *     We don't use it for any kind of authentication, so why not let
 *     user to customize her name? :)
 */
    s = getenv("LOGNAME");
    if (s == NULL || s[0] == '\0') {
#ifndef WINDOWS_SYSTEM
        s = getlogin();
#else
        s="user";
#endif
        if (s == NULL || s[0] == '\0') {
            s = "a user";
        }
    }
    username = copy_string(username, s);
//qDebug() << "UserName=" << username;
}

char *get_username(void)
{
    return username;
}

static char *userhome = NULL;

void init_userhome(void)
{
#if QT_VERSION >= 0x050000
    QStringList list=QStandardPaths::standardLocations(QStandardPaths::HomeLocation);//,QString("*"),QStandardPaths::LocateDirectory);
#else
    QStringList list;
    QString l_home=QDesktopServices::storageLocation(QDesktopServices::HomeLocation);
    list << l_home;
#endif
    QString f1;
    if (list.length()>0)
    {
    //qDebug() << "A Init Home Dir=" << list.at(0);
    f1=list.at(0)+QDir::separator();
    f1=QDir::toNativeSeparators(f1);
    userhome = copy_string(NULL,f1.toLocal8Bit());
    //qDebug() << "B Init Home Dir=" << userhome;
    }
    else
    {
    userhome = copy_string(NULL, getenv("HOME"));
    }
    f1=QString(userhome);
    QChar last=f1.at(f1.length()-1);
    //if (userhome == NULL || userhome[strlen(userhome) - 1] != '/')
    if (userhome == NULL || last!=QDir::separator())
    {
        f1=QDir::separator();
        userhome = concat_strings(userhome, f1.toLocal8Bit());//"/"
    }
    //qDebug() << "C Init Home Dir=" << userhome;
}

char *get_userhome(void)
{
    if (userhome==NULL) init_userhome();
    // was missing. And if missing, it causes segm fault on win64
    return userhome;
}

/* TODO this needs some work */
void expand_tilde(char *buf)
{
    char buf2[GR_MAXPATHLEN];

    if (buf[0] == '~') {
        if (strlen(buf) == 1) {
            strcpy(buf, get_userhome());
        } else if (buf[1] == '/') {
            if (strlen(buf) > 2) {
                strcpy(buf2, get_userhome());
                strcat(buf2, buf + 1);
                strcpy(buf, buf2);
            } else {
                strcpy(buf, get_userhome());
            }
        } else {
            char tmp[128], *pp = tmp, *q = buf + 1;

            while (*q && (*q != '/')) {
                *pp++ = *q++;
            }
            *pp = 0;
#ifndef WINDOWS_SYSTEM
            struct passwd *pent;
            if ((pent = getpwnam(tmp)) != NULL)
            {
                strcpy(buf2, pent->pw_dir);
                strcat(buf2, "/");
                strcat(buf2, q);
                strcpy(buf, buf2);
            } else {
                errmsg("No user by that name");
            }
#endif

        }
    }
}

void echomsg(char *msg)
{
    if (inwin) {
        //#ifndef NONE_GUI
        set_left_footer(msg);
        //#endif
    } else {
        printf("%s\n", msg);
    }
}

void update_timestamp(void)
{
    struct tm tm;
    time_t time_value;
    char *str;
    char dummy[GR_MAXPATHLEN+32];
    QDir doc_path;
    QString doc_str=doc_path.absoluteFilePath(QString::fromLocal8Bit(get_docname()));
    doc_str=QDir::toNativeSeparators(doc_str);
    doc_str.replace("\\","\\\\");
    (void) time(&time_value);
    tm = *localtime(&time_value);
    str = asctime(&tm);
    if (str[strlen(str) - 1] == '\n')
    {
        str[strlen(str) - 1]= '\0';
    }
    if (timestamp.path==TRUE)
    {
        if (timestamp.active==FALSE)
        {
        sprintf(dummy,"%s",doc_str.toLocal8Bit().constData());
        }
        else if (timestamp.active==TRUE)
        {
        sprintf(dummy,"%s, %s",str,doc_str.toLocal8Bit().constData());
        }
    set_plotstr_string(&timestamp, dummy);
    }
    else
    set_plotstr_string(&timestamp, str);
}

void update_app_title(void)
{
    //#ifndef NONE_GUI
///QString getFileNameOnly(QString filepath)
    set_title(mybasename(get_docname()));
    //#endif
}

/*
 * dirtystate routines
 */

/*int dirtystate = 0;*/
static int dirtystate_lock = FALSE;

void set_dirtystate(void)
{
    if (dirtystate_lock == FALSE)
    {
        dirtystate++;
        update_timestamp();
        update_app_title();

        /*
 * TODO:
 * 	if ( (dirtystate > SOME_LIMIT) ||
 *           (current_time - autosave_time > ANOTHER_LIMIT) ) {
 * 	    autosave();
 * 	}
 */
    }
}

void clear_dirtystate(void)
{
    dirtystate = 0;
    dirtystate_lock = FALSE;
    update_app_title();
}

void lock_dirtystate(int flag)
{
    dirtystate_lock = flag;
}

int is_dirtystate(void)
{
    return (dirtystate ? TRUE:FALSE);
}

int system_wrap(const char *string)
{
    return system(string);
}

void msleep_wrap(unsigned int msec)
{
#ifndef WINDOWS_SYSTEM
    struct timeval timeout;
    timeout.tv_sec = msec / 1000;
    timeout.tv_usec = 1000 * (msec % 1000);
    select(0, NULL, NULL, NULL, &timeout);
#else
    (void) msec;
    cout << "NOT SUPPORTED ON WINDOWS" << endl;
#endif
}

#ifdef HAVE_SETLOCALE
static int need_locale = FALSE;
static char *system_locale_string, *posix_locale_string;

int init_locale(void)
{
    char *s;
    //s = setlocale(LC_NUMERIC, "");
    s = setlocale(LC_NUMERIC, "C");//we only use LC_NUMERIC=C (always) and replace the decimal separator according to the users wishes!
    if (s == NULL) {
        /* invalid/unsupported locale */
        return RETURN_FAILURE;
    } else if (!strcmp(s, "C")) {
        /* don't enable need_locale, since the system locale is C */
        return RETURN_SUCCESS;
    } else {
        system_locale_string = copy_string(NULL, s);
        s = setlocale(LC_NUMERIC, "C");
        posix_locale_string = copy_string(NULL, s);
        need_locale = TRUE;
        return RETURN_SUCCESS;
    }
}

void set_locale_num(int flag)
{
    if (need_locale) {
        if (flag == TRUE) {
            setlocale(LC_NUMERIC, system_locale_string);
        } else {
            setlocale(LC_NUMERIC, posix_locale_string);
        }
    }
}
#else
int init_locale(void)
{
    return RETURN_SUCCESS;
}

void set_locale_num(int flag)
{
}
#endif

/*
 * Build info stuff
 */
long bi_version_id(void)
{
    ///return BI_VERSION_ID;
    return MAJOR_REV*10000 + MINOR_REV*100 + PATCHLEVEL;
}

char *bi_version_string(void)
{
    sprintf(BI_VERSION, "Grace-%d.%d.%d",MAJOR_REV, MINOR_REV, PATCHLEVEL);
    return BI_VERSION;
}

char *bi_system(void)
{
#ifndef WINDOWS_SYSTEM
    uname(&u_info);
    sprintf(BI_SYSTEM, "%s\n%s\n%s\n%s",u_info.sysname, u_info.version, u_info.release, u_info.machine);
#else
    sprintf(BI_SYSTEM, "Windows");
#endif
    return BI_SYSTEM;
}

const char *bi_date(void)
{
    /*#ifndef WINDOWS_SYSTEM
time_info = time(NULL);
strcpy(ctime_string,ctime(&time_info));
#else
strcpy(ctime_string,"NOT SUPPORTED");
#endif
return ctime_string;*/
    return BI_DATE;
}

const char *bi_gui(void)
{
    return BI_GUI;
}

/*#ifdef MOTIF_GUI
char *bi_gui_xbae(void)
{
    return BI_GUI_XBAE;
}
#endif*/

const char *bi_t1lib(void)
{
    return BI_T1LIB;
}

#ifdef HAVE_LIBPNG
char *bi_pnglib(void)
{
    return BI_PNGLIB;
}
#endif

#ifdef HAVE_LIBJPEG
char *bi_libjpeg(void)
{
    return BI_LIBJPEG;
}
#endif

#ifdef HAVE_LIBPDF
char *bi_libpdf(void)
{
    return BI_LIBPDF;
}
#endif

const char *bi_ccompiler(void)
{
    return BI_CCOMPILER;
}

#ifdef DEBUG
static int debuglevel = 0;

void set_debuglevel(int level)
{
    debuglevel = level;
}

int get_debuglevel(void)
{
    return debuglevel;
}
#endif


char * create_list_of_arguments(int n,...)
{
static char list1[1024];
static char list2[32];
    va_list valist;
    int i;
    va_start(valist, n);
    list1[0]='{';
    list1[1]='\0';
    for (i = 0; i < n; i++)
    {
    sprintf(list2,"%d",va_arg(valist, int));
    strcat(list1,list2);
    if (i<n-1) strcat(list1,";");
    }
    strcat(list1,"}");
    va_end(valist);
return list1;
}

int process_list_of_arguments(char * list,int ** args)
{
int count=0;
int len=strlen(list);
if (len<=2) return 0;
char * n_list=new char[len];
strcpy(n_list,list+1);
n_list[strlen(n_list)-1]='\0';
QString n_list2(n_list);
QStringList n_list3=n_list2.split(";");
count=n_list3.length();
if (*args!=NULL) delete[] *args;
*args=new int[2+count];
    for (int i=0;i<count;i++)
    {
    (*args)[i]=n_list3.at(i).toInt();
    }
delete[] n_list;
return count;
}
