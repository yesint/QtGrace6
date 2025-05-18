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
 * procedures for performing transformations from the command
 * line interpreter and the GUI.
 *
 */

///#include <config.h>
#include <cmath>
#ifndef MAXNUM
extern "C" double MAXNUM;
#endif
#include <cstdio>
#include <cstdlib>
#include <cstring>
//#include <cmath.h>
#include "cmath.h"

#include "globals.h"
#include "utils.h"
#include "draw.h"
#include "ssdata.h"
#include "graphs.h"
#include "parser.h"
#include "noxprotos.h"
#include <iostream>
#include <QObject>
#include <QString>
#include "fundamentals.h"

using namespace std;

//static void forwarddiff(double *x, double *y, double *resx, double *resy, int n);
//static void backwarddiff(double *x, double *y, double *resx, double *resy, int n);
//static void centereddiff(double *x, double *y, double *resx, double *resy, int n);
int get_points_inregion(int rno, int invr, int len, double *x, double *y, int *cnt, double **xt, double **yt);

//static char buf[256];
static QString tmp_str;
extern int new_set_no;
extern void SetDecimalSeparatorToUserValue(char * str,bool remove_space=true);
extern void remove_whitespaces(char * tar,char * ch);
extern void remove_a_character(char * tar,char * ch,char a);
extern char last_formula[];

extern void errwin(const char *s);

extern void ParseRegression(char * com,int & n_sets,int ** gnos,int ** snos,int & n_n_sets,int ** n_gnos,int ** n_snos,int & ideg,int & iresid,int & rno,int & invr,double & start,double & stop,int & points,int & rx,char * formula);
extern void ParseFilterCommand(char * com,int & o_n_sets,int ** o_gnos,int ** o_snos,int & n_sets,int ** gnos,int ** snos,int & type,int & realization,double * limits,int * orders,char * x_formula,double & ripple,int & absolute,int & debug,int & point_extension,int & oversampling,int & rno,int & invr);
extern int ParseExtractCommand(char * com,char * arg);
extern int generate_x_mesh_from_formula(int gno,int sno,double start,double stop,int npts,char * formula,int type);
extern int do_filter_on_one_set(int n_gno,int n_sno,int o_gno,int o_sno,int type,int realization,int restr_type,int restr_negate,int abs,int debug,char * formula,int point_extension,int oversampling,double ripple,int order1,int order2,double f1,double f2);


void do_fourier_command(int gno, int setno, int ftype, int ltype)
{
    switch (ftype) {
    case FFT_DFT:
	do_fourier(gno, setno, 0, 0, ltype, 0, 0, 0);
	break;
    case FFT_INVDFT       :
	do_fourier(gno, setno, 0, 0, ltype, 1, 0, 0);
	break;
    case FFT_FFT:
	do_fourier(gno, setno, 1, 0, ltype, 0, 0, 0);
	break;
    case FFT_INVFFT       :
	do_fourier(gno, setno, 1, 0, ltype, 1, 0, 0);
	break;
    }
}

/*
 * evaluate a formula
 */
int do_compute(int gno, int setno, int graphto, int loadto, char *rarray, char *fstr)
{
int return_value;
QString formula_c;
class formula_to_process formula1;
    if (is_set_active(gno, setno)) {
	if (gno != graphto || setno != loadto) {
        if (copysetdata(gno, setno, graphto, loadto) != RETURN_SUCCESS) {
	        return RETURN_FAILURE;
            }
        }
//qDebug() << "do_compute= gno=" << gno << "setno=" << setno << "graphto=" << graphto << "loadto=" << loadto << "fstr=" << fstr;
    set_parser_setno(graphto, loadto);
//qDebug() << "before init";
    formula1.init_formula(QString(fstr));
//qDebug() << "after init";
    /// the first version has target-set and -graph for left AND right hand side
    /// the second version has the correct set- and graph-ids for left and right hand side
    //formula_c=formula1.execute_formula(graphto, loadto, graphto, loadto, 0,return_value);//complete ids and execute formula
    formula_c=formula1.execute_formula(graphto, loadto, gno, setno, 0,return_value);//complete ids and execute formula
//qDebug() << "after execute formula: length=" << getsetlength(graphto, loadto);
    filter_set(graphto, loadto, rarray);
//qDebug() << "after filter set: length=" << getsetlength(graphto, loadto);
    strcpy(fstr,formula_c.toLatin1().constData());
    /*if (copy_string(fstr,formula_c.toLatin1().constData()) != fstr)
    {
        qDebug("ERROR with fstr; copy_string - changed!!");
    }*/
//qDebug() << "fstr=" << fstr;

        //if (scanner(fstr) != RETURN_SUCCESS) {
        if (return_value != RETURN_SUCCESS) {
            if (graphto != gno || loadto != setno) {
            killset(graphto, loadto);
            }
	    return RETURN_FAILURE;
	} else {
	    set_dirtystate();
            return RETURN_SUCCESS;
        }
    } else {
        return RETURN_FAILURE;
    }
}

/*
 * forward, backward and centered differences
 */
void forwarddiff(double *x, double *y, double *resx, double *resy, int n)
{
    int i, eflag = 0;
    double h;

    for (i = 1; i < n; i++) {
	resx[i - 1] = x[i - 1];
	h = x[i - 1] - x[i];
	if (h == 0.0) {
	    resy[i - 1] = - MAXNUM;
	    eflag = 1;
	} else {
	    resy[i - 1] = (y[i - 1] - y[i]) / h;
	}
    }
    if (eflag) {
    errmsg(QObject::tr("Warning: infinite slope, check set status before proceeding").toLocal8Bit().constData());
    }
}

void backwarddiff(double *x, double *y, double *resx, double *resy, int n)
{
    int i, eflag = 0;
    double h;

    for (i = 0; i < n - 1; i++) {
	resx[i] = x[i];
	h = x[i + 1] - x[i];
	if (h == 0.0) {
	    resy[i] = - MAXNUM;
	    eflag = 1;
	} else {
	    resy[i] = (y[i + 1] - y[i]) / h;
	}
    }
    if (eflag) {
    errmsg(QObject::tr("Warning: infinite slope, check set status before proceeding").toLocal8Bit().constData());
    }
}

void centereddiff(double *x, double *y, double *resx, double *resy, int n)
{
    int i, eflag = 0;
    double h1, h2;

    for (i = 1; i < n - 1; i++) {
	resx[i - 1] = x[i];
	h1 = x[i] - x[i - 1];
	h2 = x[i + 1] - x[i];
	if (h1 + h2 == 0.0) {
	    resy[i - 1] = - MAXNUM;
	    eflag = 1;
	} else {
	    resy[i - 1] = (y[i + 1] - y[i - 1]) / (h1 + h2);
	}
    }
    if (eflag) {
    errmsg(QObject::tr("Warning: infinite slope, check set status before proceeding").toLocal8Bit().constData());
    }
}

static void seasonaldiff(double *x, double *y,
			 double *resx, double *resy, int n, int period)
{
    int i;

    for (i = 0; i < n - period; i++) {
	resx[i] = x[i];
	resy[i] = y[i] - y[i + period];
    }
}

/*
 * trapezoidal rule
 */
double trapint(double *x, double *y, double *resx, double *resy, int n)
{
    int i;
    double sum = 0.0;
    double h;

    if (n < 2) {
        return 0.0;
    }
    
    if (resx != NULL) {
        resx[0] = x[0];
    }
    if (resy != NULL) {
        resy[0] = 0.0;
    }
    for (i = 1; i < n; i++) {
	h = (x[i] - x[i - 1]);
	if (resx != NULL) {
	    resx[i] = x[i];
	}
	sum = sum + h * (y[i - 1] + y[i]) * 0.5;
	if (resy != NULL) {
	    resy[i] = sum;
	}
    }
    return sum;
}

/*
 * apply a digital filter
 */
void do_digfilter(int set1, int set2)
{
    int digfiltset;
new_set_no=-1;
    if (!(is_set_active(get_cg(), set1) && is_set_active(get_cg(), set2))) {
    errmsg(QObject::tr("Set not active").toLocal8Bit().constData());
	return;
    }
    if ((getsetlength(get_cg(), set1) < 3) || (getsetlength(get_cg(), set2) < 3)) {
    errmsg(QObject::tr("Set length < 3").toLocal8Bit().constData());
	return;
    }
    new_set_no = digfiltset = nextset(get_cg());
    if (digfiltset != (-1)) {
	activateset(get_cg(), digfiltset);
	setlength(get_cg(), digfiltset, getsetlength(get_cg(), set1) - getsetlength(get_cg(), set2) + 1);
    //sprintf(buf, "Digital filter from set %d applied to set %d", set2, set1);
    tmp_str=QObject::tr("Digital filter from set ")+QString::number(set2)+QObject::tr(" applied to set ")+QString::number(set1);
	filterser(getsetlength(get_cg(), set1),
		  getx(get_cg(), set1),
		  gety(get_cg(), set1),
		  getx(get_cg(), digfiltset),
		  gety(get_cg(), digfiltset),
		  gety(get_cg(), set2),
		  getsetlength(get_cg(), set2));
    //setcomment(get_cg(), digfiltset, buf);
    setcomment(get_cg(), digfiltset, tmp_str.toLocal8Bit().constData());
    }
}

/*
 * linear convolution
 */
void do_linearc(int gno1, int set1, int gno2, int set2)
{
    int linearcset, i, itmp, cg = get_cg();
    double *xtmp;
new_set_no=-1;
    if (!(is_set_active(gno1, set1) && is_set_active(gno2, set2)))
    {
        tmp_str=QObject::tr("Set not active");
    errmsg(tmp_str.toLocal8Bit().constData());
	return;
    }
    if ((getsetlength(gno1, set1) < 3) || (getsetlength(gno2, set2) < 3))
    {
        tmp_str=QObject::tr("Set length < 3");
    errmsg(tmp_str.toLocal8Bit().constData());
	return;
    }
    new_set_no = linearcset = nextset(cg);
    if (linearcset != (-1)) {
	activateset(cg, linearcset);
	setlength(cg, linearcset, (itmp = getsetlength(gno1, set1) + getsetlength(gno2, set2) - 1));
	linearconv(gety(gno2, set2), gety(gno1, set1), gety(cg, linearcset), getsetlength(gno2, set2), getsetlength(gno1, set1));
	xtmp = getx(cg, linearcset);
	for (i = 0; i < itmp; i++) {
	    xtmp[i] = i;
	}
    //sprintf(buf, "Linear convolution of set %d with set %d", set1, set2);
    //setcomment(cg, linearcset, buf);
    tmp_str=QObject::tr("Linear convolution of set ")+QString::number(set2)+QObject::tr(" with set ")+QString::number(set1);
    setcomment(cg, linearcset, tmp_str.toLocal8Bit().constData());
    }
}

/*
 * cross correlation/covariance
 */
void do_xcor(int gno1, int set1, int gno2, int set2, int maxlag, int covar)
{
    int xcorset, i, len, cg = get_cg();//ierr
    double *xtmp;
    QString tmp_str2;
new_set_no=-1;
    if (!(is_set_active(gno1, set1) && is_set_active(gno2, set2)))
    {
        tmp_str=QObject::tr("Set not active");
    errmsg(tmp_str.toLocal8Bit().constData());
	return;
    }
    
    len = getsetlength(gno1, set1);
    if (getsetlength(gno2, set2) != len)
    {
        tmp_str=QObject::tr("Sets must be of the same length");
    errmsg(tmp_str.toLocal8Bit().constData());
    }
    if (len < 2)
    {
        tmp_str=QObject::tr("Set length < 2");
    errmsg(tmp_str.toLocal8Bit().constData());
	return;
    }
    
    if (maxlag < 1 || maxlag > len)
    {
        tmp_str=QObject::tr("Lag incorrectly specified");
    errmsg(tmp_str.toLocal8Bit().constData());
	return;
    }
    new_set_no=xcorset = nextset(cg);
    if (xcorset != (-1)) {
    //const char *fname;
        activateset(cg, xcorset);
	setlength(cg, xcorset, maxlag);
	if (covar) {
            //fname = "covariance";
            tmp_str2=QObject::tr("covariance");
        } else {
            //fname = "correlation";
            tmp_str2=QObject::tr("correlation");
        }
    if (set1 != set2)
    {
    //sprintf(buf, "X-%s of G%d.S%d and G%d.S%d at maximum lag %d",fname, gno1, set1, gno2, set2, maxlag);
    tmp_str=QObject::tr("X-")+tmp_str2+QObject::tr(" of G")+QString::number(gno1)+QObject::tr(".S")+QString::number(set1)+QObject::tr(" and G")+QString::number(gno2)+QObject::tr(".S")+QString::number(set2)+QObject::tr(" at maximum lag ")+QString::number(maxlag);
    }
    else
    {
    //sprintf(buf, "Auto-%s of G%d.S%d at maximum lag %d",fname, gno1, set1, maxlag);
    tmp_str=QObject::tr("Auto-")+tmp_str2+QObject::tr(" of G")+QString::number(gno1)+QObject::tr(".S")+QString::number(set1)+QObject::tr(" at maximum lag ")+QString::number(maxlag);
	}
    //ierr = crosscorr(gety(gno1, set1), gety(gno2, set2), len, maxlag, covar, gety(cg, xcorset));
    (void)crosscorr(gety(gno1, set1), gety(gno2, set2), len, maxlag, covar, gety(cg, xcorset));
	xtmp = getx(cg, xcorset);
	for (i = 0; i < maxlag; i++) {
	    xtmp[i] = i;
	}
    //setcomment(cg, xcorset, buf);
    setcomment(cg, xcorset, tmp_str.toLocal8Bit().constData());
    }
}


/*
 * numerical integration
 */
double do_int(int gno, int setno, int itype)
{
    int intset;
    double sum = 0;
new_set_no=-1;
    if (!is_set_active(gno, setno))
    {
    tmp_str=QObject::tr("Set not active");
    errmsg(tmp_str.toLocal8Bit().constData());
	return 0.0;
    }
    if (getsetlength(gno, setno) < 3)
    {
    tmp_str=QObject::tr("Set length < 3");
    errmsg(tmp_str.toLocal8Bit().constData());
	return 0.0;
    }
    if (itype == 0) {
        new_set_no = intset = nextset(gno);
	if (intset != (-1)) {
	    activateset(gno, intset);
	    setlength(gno, intset, getsetlength(gno, setno));
        //sprintf(buf, "Cumulative sum of set %d", setno);
        tmp_str=QObject::tr("Cumulative sum of set ")+QString::number(setno);
	    sum = trapint(getx(gno, setno), gety(gno, setno), getx(gno, intset), gety(gno, intset), getsetlength(gno, setno));
        //setcomment(gno, intset, buf);
        setcomment(gno, intset, tmp_str.toLocal8Bit().constData());
	}
    } else {
	sum = trapint(getx(gno, setno), gety(gno, setno), NULL, NULL, getsetlength(gno, setno));
    }
    return sum;
}

/*
 * difference a set
 * itype means
 *  0 - forward
 *  1 - backward
 *  2 - centered difference
 */
void do_differ(int gno, int setno, int itype)
{
    int diffset;
new_set_no=-1;
    if (!is_set_active(gno, setno)) {
    errmsg(QObject::tr("Set not active").toLocal8Bit().constData());
	return;
    }
    if (getsetlength(gno, setno) < 3) {
    errmsg(QObject::tr("Set length < 3").toLocal8Bit().constData());
	return;
    }
    new_set_no = diffset = nextset(gno);
    if (diffset != (-1)) {
	activateset(gno, diffset);
	switch (itype) {
	case 0:
        //sprintf(buf, "Forward difference of set %d", setno);
        tmp_str=QObject::tr("Forward difference of set ")+QString::number(setno);
	    setlength(gno, diffset, getsetlength(gno, setno) - 1);
	    forwarddiff(getx(gno, setno), gety(gno, setno), getx(gno, diffset), gety(gno, diffset), getsetlength(gno, setno));
	    break;
	case 1:
        //sprintf(buf, "Backward difference of set %d", setno);
        tmp_str=QObject::tr("Backward difference of set ")+QString::number(setno);
	    setlength(gno, diffset, getsetlength(gno, setno) - 1);
	    backwarddiff(getx(gno, setno), gety(gno, setno), getx(gno, diffset), gety(gno, diffset), getsetlength(gno, setno));
	    break;
	case 2:
    default:
        //sprintf(buf, "Centered difference of set %d", setno);
        tmp_str=QObject::tr("Centered difference of set ")+QString::number(setno);
	    setlength(gno, diffset, getsetlength(gno, setno) - 2);
	    centereddiff(getx(gno, setno), gety(gno, setno), getx(gno, diffset), gety(gno, diffset), getsetlength(gno, setno));
	    break;
	}
    //setcomment(gno, diffset, buf);
    setcomment(gno, diffset, tmp_str.toLocal8Bit().constData());
    }
}

/*
 * seasonally difference a set
 */
void do_seasonal_diff(int setno, int period)
{
    int diffset;
new_set_no=-1;
    if (!is_set_active(get_cg(), setno))
    {
        tmp_str=QObject::tr("Set not active");
    errmsg(tmp_str.toLocal8Bit().constData());
	return;
    }
    if (getsetlength(get_cg(), setno) < 2)
    {
        tmp_str=QObject::tr("Set length < 2");
    errmsg(tmp_str.toLocal8Bit().constData());
	return;
    }
    new_set_no = diffset = nextset(get_cg());
    if (diffset != (-1)) {
	activateset(get_cg(), diffset);
	setlength(get_cg(), diffset, getsetlength(get_cg(), setno) - period);
	seasonaldiff(getx(get_cg(), setno), gety(get_cg(), setno),
		     getx(get_cg(), diffset), gety(get_cg(), diffset),
		     getsetlength(get_cg(), setno), period);
    //sprintf(buf, "Seasonal difference of set %d, period %d", setno, period);
    //setcomment(get_cg(), diffset, buf);
    tmp_str=QObject::tr("Seasonal difference of set ")+QString::number(setno)+QObject::tr(", period ")+QString::number(period);
    setcomment(get_cg(), diffset, tmp_str.toLocal8Bit().constData());
    }
}

/*
 * regression with restrictions to region rno if rno >= 0
 */
void do_regress(int gno, int setno, int ideg, int iresid, int rno, int invr, int fitset)
	/* 
	 * gno, setno  - set to perform fit on
	 * ideg   - degree of fit
     * iresid - 0 -> whole set, 1 -> subset of setno, 2 -> none at all
	 * rno    - region number of subset
     * invr   - 1 -> invert region, 0 -> do nothing
	 * fitset - set to which fitted function will be loaded
	 * 			Y values are computed at the x values in the set
	 *          if -1 is specified, a set with the same x values as the
	 *          one being fitted will be created and used
	 */
{
    int len, i, sdeg = ideg;
    int cnt = 0, fitlen = 0;
    double *x, *y, *xt = NULL, *yt = NULL, *xr = NULL, *yr = NULL;
    char buf[256],buf2[256];
    double c[20];   /* coefficients of fitted polynomial */
new_set_no=-1;
    if (!is_set_active(gno, setno))
    {
        tmp_str=QObject::tr("Set not active");
        errmsg(tmp_str.toLocal8Bit().constData());
        return;
    }
    len = getsetlength(gno, setno);
    x = getx(gno, setno);
    y = gety(gno, setno);
    if (rno == -1) {
		xt = x;
		yt = y;
    } else if (isactive_region(rno)) {
		if (!get_points_inregion(rno, invr, len, x, y, &cnt, &xt, &yt)) {
            if (cnt == 0)
            {
            tmp_str=QObject::tr("No points found in region, operation cancelled");
            errmsg(tmp_str.toLocal8Bit().constData());
			}
			return;
		}
		len = cnt;
    } else {
        tmp_str=QObject::tr("Selected region is not active");
        errmsg(tmp_str.toLocal8Bit().constData());
		return;
    }
    /*
     * first part for polynomials, second part for linear fits to transformed
     * data
     */
    if ((len < ideg && ideg <= 10) || (len < 2 && ideg > 10))
    {
        tmp_str=QObject::tr("Too few points in set, operation cancelled");
        errmsg(tmp_str.toLocal8Bit().constData());
		return;
    }
	/* determine is set provided or use abscissa from fitted set */
    if( fitset == -1 && iresid != 2) {		        /* create set */
    	if( (fitset = nextset(gno)) != -1 ) {
			activateset(gno, fitset);
			setlength(gno, fitset, len);
			fitlen = len;
			xr = getx(gno, fitset);
			for (i = 0; i < len; i++) {
	    		xr[i] = xt[i];
			}	
			yr = gety(gno, fitset);
		}
    } else if (iresid == 2) {//do not create a set
        fitlen = len;
        xr=new double[len+1];
        yr=new double[len+1];
            for (int i=0;i<len;i++)
            {
            xr[i]=x[i];
            yr[i]=y[i];
            }
    } else {									/* set has been provided */
		fitlen = getsetlength( gno, fitset );
		xr = getx(gno, fitset);
		yr = gety(gno, fitset);
    }
new_set_no=fitset;
	/* transform data so that system has the form y' = A + B * x' */
    if (fitset != -1 || iresid == 2) {
	if (ideg == 12) {	/* y=A*x^B -> ln(y) = ln(A) + B * ln(x) */
	    ideg = 1;
	    for (i = 0; i < len; i++) {
            if (xt[i] <= 0.0)
            {
                tmp_str=QObject::tr("One of X[i] <= 0.0");
                errmsg(tmp_str.toLocal8Bit().constData());
				return;
			}
            if (yt[i] <= 0.0)
            {
                tmp_str=QObject::tr("One of Y[i] <= 0.0");
                errmsg(tmp_str.toLocal8Bit().constData());
				return;
			}
	    }
	    for (i = 0; i < len; i++) {
			xt[i] = log(xt[i]);
			yt[i] = log(yt[i]);
	    }
		for( i=0; i<fitlen; i++ )
            if( xr[i] <= 0.0 )
            {
                tmp_str=QObject::tr("One of X[i] <= 0.0");
                errmsg(tmp_str.toLocal8Bit().constData());
				return;
			} 
		for( i=0; i<fitlen; i++ )
			xr[i] = log( xr[i] );
    } else if (ideg == 13) {   /*y=A*exp(B*x) -> ln(y) = ln(A) + B * x */
	    ideg = 1;
	    for (i = 0; i < len; i++) {
            if (yt[i] <= 0.0)
            {
                tmp_str=QObject::tr("One of Y[i] <= 0.0");
                errmsg(tmp_str.toLocal8Bit().constData());
				return;
		 	}
	    }
	    for (i = 0; i < len; i++) {
			yt[i] = log(yt[i]);
	    }
	} else if (ideg == 14) {	/* y = A + B * ln(x) */
	    ideg = 1;
	    for (i = 0; i < len; i++) {
            if (xt[i] <= 0.0)
            {
                tmp_str=QObject::tr("One of X[i] <= 0.0");
                errmsg(tmp_str.toLocal8Bit().constData());
				return;
			}
	    }
		for (i = 0; i < len; i++) {
			xt[i] = log(xt[i]);	
		}
		for( i=0; i<fitlen; i++ )
            if( xr[i] <= 0.0 )
            {
                tmp_str=QObject::tr("One of X[i] <= 0.0");
                errmsg(tmp_str.toLocal8Bit().constData());
				return;
			} 
		for( i=0; i<fitlen; i++ ){
			xr[i] = log( xr[i] );
		}	  
	} else if (ideg == 15) {	/* y = 1/( A + B*x ) -> 1/y = a + B*x */
	    ideg = 1;
	    for (i = 0; i < len; i++) {
            if (yt[i] == 0.0)
            {
                tmp_str=QObject::tr("One of Y[i] <= 0.0");
                errmsg(tmp_str.toLocal8Bit().constData());
			    return;
			}
	    }
	    for (i = 0; i < len; i++) {
			yt[i] = 1.0 / yt[i];
	    }
	}

    if (fitcurve(xt, yt, len, ideg, c))
    {
	    killset(gno, fitset);
	    goto bustout;
    }

    /* compute function at requested x coordinates */
        if (iresid != 2)
        {
        for( i=0; i<fitlen; i++ )
		yr[i] = leasev( c, ideg, xr[i] );
        }

    strcpy(buf, QObject::tr("\nN.B. Statistics refer to the transformed model\n").toLocal8Bit().constData());
	/* apply inverse transform, output fitted function in human readable form */
    if( sdeg<11 )
    {
		sprintf(buf, "\ny = %.5g %c %.5g * x", c[0], c[1]>=0?'+':'-', fabs(c[1]));
            for( i=2; i<=ideg; i++ )
            sprintf( buf+strlen(buf), " %c %.5g * x^%d", c[i]>=0?'+':'-',fabs(c[i]), i );
		strcat( buf, "\n" );
	} else if (sdeg == 12) {	/* ln(y) = ln(A) + b * ln(x) */
		sprintf( buf, "\ny = %.5g * x^%.5g\n", exp(c[0]), c[1] );
	    for (i = 0; i < len; i++) {
			xt[i] = exp(xt[i]);
            yt[i] = exp(yt[i]);
	    }
	    for (i = 0; i < fitlen; i++){
			yr[i] = exp(yr[i]);
			xr[i] = exp(xr[i]);
		}
	} else if (sdeg == 13) { 	/* ln(y) = ln(A) + B * x */
		sprintf( buf, "\ny = %.5g * exp( %.5g * x )\n", exp(c[0]), c[1] );
	    for (i = 0; i < len; i++) {
			yt[i] = exp(yt[i]);
	    }
	    for (i = 0; i < fitlen; i++)
			yr[i] = exp(yr[i]);
	} else if (sdeg == 14) {	/* y = A + B * ln(x) */
        sprintf(buf, "\ny = %.5g %c %.5g * ln(x)\n", c[0], c[1]>=0?'+':'-',fabs(c[1]) );
	    for (i = 0; i < len; i++) {
			xt[i] = exp(xt[i]);
	    }
	    for (i = 0; i < fitlen; i++)
			xr[i] = exp(xr[i]);
	} else if (sdeg == 15) {	/* y = 1/( A + B*x ) */
        sprintf( buf, "\ny = 1/(%.5g %c %.5g * x)\n", c[0], c[1]>=0?'+':'-',fabs(c[1]) );
	    for (i = 0; i < len; i++) {
			yt[i] = 1.0 / yt[i];
	    }
	    for (i = 0; i < fitlen; i++)
			yr[i] = 1.0 / yr[i];
	}

    remove_whitespaces(last_formula,buf);//save the formula used for the regression

//cout << "Regression: last Formula=#" << last_formula << "#" << endl;

    SetDecimalSeparatorToUserValue(buf,false);

    remove_a_character(buf2,buf,'\n');
    stufftext("");
    stufftext(buf2);//buf

    if (fitset!=-1)
    {
    //sprintf(buf, "\nRegression of set %d results to set %d", setno, fitset);//\n
    tmp_str=QString("\n")+QObject::tr("Regression of set ")+QString::number(setno)+QObject::tr(" results to set ")+QString::number(fitset);
    }
    else//no new set
    {
    sprintf(buf, "\nRegression of set %d only, no new set.", setno);//\n
    tmp_str=QString("\n")+QObject::tr("Regression of set ")+QString::number(setno)+QObject::tr(" only, no new set.");
    }
    //stufftext(buf);
    stufftext(tmp_str.toLocal8Bit().constData());
	
	switch (iresid) {
	case 1:
		/* determine residual */
	    for (i = 0; i < len; i++) {
			yr[i] = yt[i] - yr[i];
	    }
	    break;
	case 2:
	    break;
	}
    //sprintf(buf, "%d deg fit of set %d; fit-formula: %s", ideg, setno, buf2);
    //setcomment(gno, fitset, buf);
    tmp_str=QString::number(ideg)+QObject::tr(" deg fit of set ")+QString::number(setno)+QObject::tr("; fit-formula: ")+QString(buf2);
    setcomment(gno, fitset, tmp_str.toLocal8Bit().constData());
    }
    bustout:;
    if (rno >= 0 && cnt != 0) {	/* had a region and allocated memory there */
	xfree(xt);
	xfree(yt);
    }
    if (iresid == 2)
    {
    delete[] xr;
    delete[] yr;
    }
}

/*
 * running averages, medians, min, max, std. deviation
 */
void do_runavg(int gno, int setno, int runlen, int runtype, int rno, int invr)
{
    int runset;
    int len, cnt = 0;
    double *x, *y, *xt = NULL, *yt = NULL, *xr, *yr;
new_set_no=-1;
    if (!is_set_active(gno, setno))
    {
        tmp_str=QObject::tr("Set not active");
        errmsg(tmp_str.toLocal8Bit().constData());
	return;
    }
    if (runlen < 2)
    {
        tmp_str=QObject::tr("Length of running average < 2");
        errmsg(tmp_str.toLocal8Bit().constData());
	return;
    }
    len = getsetlength(gno, setno);
    x = getx(gno, setno);
    y = gety(gno, setno);
    if (rno == -1) {
	xt = x;
	yt = y;
    } else if (isactive_region(rno)) {
	if (!get_points_inregion(rno, invr, len, x, y, &cnt, &xt, &yt)) {
        if (cnt == 0)
        {
        tmp_str=QObject::tr("No points found in region, operation cancelled");
        errmsg(tmp_str.toLocal8Bit().constData());
	    }
	    return;
	}
	len = cnt;
    }
    else
    {
        tmp_str=QObject::tr("Selected region is not active");
        errmsg(tmp_str.toLocal8Bit().constData());
	return;
    }
    if (runlen >= len)
    {
        tmp_str=QObject::tr("Length of running average > set length");
        errmsg(tmp_str.toLocal8Bit().constData());
	goto bustout;
    }
    runset = nextset(gno);
    new_set_no=runset;
    if (runset != (-1)) {
	activateset(gno, runset);
	setlength(gno, runset, len - runlen + 1);
	xr = getx(gno, runset);
	yr = gety(gno, runset);
	switch (runtype) {
	case 0:
	    runavg(xt, yt, xr, yr, len, runlen);
        //sprintf(buf, "%d-pt. avg. on set %d ", runlen, setno);
        tmp_str=QString::number(runlen)+QObject::tr("-pt. avg. on set ")+QString::number(setno);
	    break;
	case 1:
	    runmedian(xt, yt, xr, yr, len, runlen);
        //sprintf(buf, "%d-pt. median on set %d ", runlen, setno);
        tmp_str=QString::number(runlen)+QObject::tr("-pt. median on set ")+QString::number(setno);
	    break;
	case 2:
	    runminmax(xt, yt, xr, yr, len, runlen, 0);
        //sprintf(buf, "%d-pt. min on set %d ", runlen, setno);
        tmp_str=QString::number(runlen)+QObject::tr("-pt. min on set ")+QString::number(setno);
	    break;
	case 3:
	    runminmax(xt, yt, xr, yr, len, runlen, 1);
        //sprintf(buf, "%d-pt. max on set %d ", runlen, setno);
        tmp_str=QString::number(runlen)+QObject::tr("-pt. max on set ")+QString::number(setno);
	    break;
	case 4:
	    runstddev(xt, yt, xr, yr, len, runlen);
        //sprintf(buf, "%d-pt. std dev., set %d ", runlen, setno);
        tmp_str=QString::number(runlen)+QObject::tr("-pt. std dev., set ")+QString::number(setno);
	    break;
	}
    //setcomment(gno, runset, buf);
    setcomment(gno, runset, tmp_str.toLocal8Bit().constData());
    }
  bustout:;
    if (rno >= 0 && cnt != 0) {	/* had a region and allocated memory there */
	xfree(xt);
	xfree(yt);
    }
}

/*
 * DFT by FFT or definition
 */
void do_fourier(int gno, int setno, int fftflag, int load, int loadx, int invflag, int type, int wind)
{
    int i, ilen;
    double *x, *xx, *yy, delt, T;//*y
    int i2 = 0, specset;
    QString e_txt;
    e_txt=QString("G")+QString::number(gno)+QString(".S")+QString::number(setno)+QString(": ");

    if (!is_set_active(gno, setno))
    {
    e_txt=e_txt+QObject::tr("Set not active");
    errmsg(e_txt.toLocal8Bit().constData());
	return;
    }
    ilen = getsetlength(gno, setno);
    if (ilen < 2)
    {
    e_txt=e_txt+QObject::tr("Set length < 2");
    errmsg(e_txt.toLocal8Bit().constData());
	return;
    }
    if (fftflag && !(use_fftw3==TRUE && have_fftw3==TRUE))
    {
        if ((i2 = ilog2(ilen)) <= 0)
        {
        e_txt=e_txt+QObject::tr("Set length not a power of 2");
        errmsg(e_txt.toLocal8Bit().constData());
	    return;
        }
    }
    new_set_no = specset = nextset(gno);
    if (specset != -1) {
    activateset(gno, specset);
    setlength(gno, specset, ilen);
    xx = getx(gno, specset);
    yy = gety(gno, specset);
    x = getx(gno, setno);
    //y = gety(gno, setno);
    copyx(gno, setno, specset);
    copyy(gno, setno, specset);
	if (wind != 0) {	/* apply data window if needed */
	    apply_window(xx, yy, ilen, type, wind);
	}
	if (type == 0) {	/* real data */
	    for (i = 0; i < ilen; i++) {
		xx[i] = yy[i];
		yy[i] = 0.0;
	    }
	}
	if (fftflag) {
	    fft(xx, yy, ilen, i2, invflag);
	} else {
	    dft(xx, yy, ilen, invflag);
	}
	switch (load) {
	case 0:
	    delt = (x[ilen-1] - x[0])/(ilen -1.0);
	    T = (x[ilen - 1] - x[0]);
        xx = getx(gno, specset);
        yy = gety(gno, specset);
	    for (i = 0; i < ilen / 2; i++) {
	      /* carefully get amplitude of complex xform: 
		 use abs(a[i]) + abs(a[-i]) except for zero term */
	      if(i) yy[i] = hypot(xx[i], yy[i])+hypot(xx[ilen-i], yy[ilen-i]);
	      else yy[i]=fabs(xx[i]);
		switch (loadx) {
		case 0:
		    xx[i] = i;
		    break;
		case 1:
		    /* xx[i] = 2.0 * M_PI * i / ilen; */
		    xx[i] = i / T;
		    break;
		case 2:
		    if (i == 0) {
			xx[i] = T + delt;	/* the mean */
		    } else {
			/* xx[i] = (double) ilen / (double) i; */
			xx[i] = T / i;
		    }
		    break;
		}
	    }
        setlength(gno, specset, ilen / 2);
	    break;
	case 1:
	    delt = (x[ilen-1] - x[0])/(ilen -1.0);
	    T = (x[ilen - 1] - x[0]);
        setlength(gno, specset, ilen / 2);
        xx = getx(gno, specset);
        yy = gety(gno, specset);
	    for (i = 0; i < ilen / 2; i++) {
		yy[i] = -atan2(yy[i], xx[i]);
		switch (loadx) {
		case 0:
		    xx[i] = i;
		    break;
		case 1:
		    /* xx[i] = 2.0 * M_PI * i / ilen; */
		    xx[i] = i / T;
		    break;
		case 2:
		    if (i == 0) {
			xx[i] = T + delt;
		    } else {
			/* xx[i] = (double) ilen / (double) i; */
			xx[i] = T / i;
		    }
		    break;
		}
	    }
	    break;
	}
	if (fftflag) {
        //sprintf(buf, "FFT of set %d", setno);
        tmp_str=QObject::tr("FFT of set ")+QString::number(setno);
	} else {
        //sprintf(buf, "DFT of set %d", setno);
        tmp_str=QObject::tr("DFT of set ")+QString::number(setno);
	}
    //setcomment(gno, specset, buf);
    setcomment(gno, specset, tmp_str.toLocal8Bit().constData());
    }
}

/*
 * Apply a window to a set, result goes to a new set.
 */
void do_window(int setno, int type, int wind)
{
    int ilen;
    double *xx, *yy;
    int specset;

    if (!is_set_active(get_cg(), setno))
    {
        tmp_str=QObject::tr("Set not active");
        errmsg(tmp_str.toLocal8Bit().constData());
	return;
    }
    ilen = getsetlength(get_cg(), setno);
    if (ilen < 2)
    {
        tmp_str=QObject::tr("Set length < 2");
        errmsg(tmp_str.toLocal8Bit().constData());
	return;
    }
    specset = nextset(get_cg());
    if (specset != -1) {
    const char * wtype[6];
	wtype[0] = "Triangular";
	wtype[1] = "Hanning";
	wtype[2] = "Welch";
	wtype[3] = "Hamming";
	wtype[4] = "Blackman";
	wtype[5] = "Parzen";

	activateset(get_cg(), specset);
	setlength(get_cg(), specset, ilen);
	xx = getx(get_cg(), specset);
	yy = gety(get_cg(), specset);
	copyx(get_cg(), setno, specset);
	copyy(get_cg(), setno, specset);
	if (wind != 0) {
	    apply_window(xx, yy, ilen, type, wind);
        //sprintf(buf, "%s windowed set %d", wtype[wind - 1], setno);
        tmp_str=QString(wtype[wind - 1])+QObject::tr(" windowed set ")+QString::number(setno);
	} else {		/* shouldn't happen */
	}
    //setcomment(get_cg(), specset, buf);
    setcomment(get_cg(), specset, tmp_str.toLocal8Bit().constData());
    }
}

void apply_window(double *xx, double *yy, int ilen, int type, int wind)
{
    int i;

    for (i = 0; i < ilen; i++) {
	switch (wind) {
	case 1:		/* triangular */
	    if (type != 0) {
		xx[i] *= 1.0 - fabs((i - 0.5 * (ilen - 1.0)) / (0.5 * (ilen - 1.0)));
	    }
	    yy[i] *= 1.0 - fabs((i - 0.5 * (ilen - 1.0)) / (0.5 * (ilen - 1.0)));

	    break;
	case 2:		/* Hanning */
	    if (type != 0) {
		xx[i] = xx[i] * (0.5 - 0.5 * cos(2.0 * M_PI * i / (ilen - 1.0)));
	    }
	    yy[i] = yy[i] * (0.5 - 0.5 * cos(2.0 * M_PI * i / (ilen - 1.0)));
	    break;
	case 3:		/* Welch (from Numerical Recipes) */
	    if (type != 0) {
		xx[i] *= 1.0 - pow((i - 0.5 * (ilen - 1.0)) / (0.5 * (ilen + 1.0)), 2.0);
	    }
	    yy[i] *= 1.0 - pow((i - 0.5 * (ilen - 1.0)) / (0.5 * (ilen + 1.0)), 2.0);
	    break;
	case 4:		/* Hamming */
	    if (type != 0) {
		xx[i] = xx[i] * (0.54 - 0.46 * cos(2.0 * M_PI * i / (ilen - 1.0)));
	    }
	    yy[i] = yy[i] * (0.54 - 0.46 * cos(2.0 * M_PI * i / (ilen - 1.0)));
	    break;
	case 5:		/* Blackman */
	    if (type != 0) {
		xx[i] = xx[i] * (0.42 - 0.5 * cos(2.0 * M_PI * i / (ilen - 1.0)) + 0.08 * cos(4.0 * M_PI * i / (ilen - 1.0)));
	    }
	    yy[i] = yy[i] * (0.42 - 0.5 * cos(2.0 * M_PI * i / (ilen - 1.0)) + 0.08 * cos(4.0 * M_PI * i / (ilen - 1.0)));
	    break;
	case 6:		/* Parzen (from Numerical Recipes) */
	    if (type != 0) {
		xx[i] *= 1.0 - fabs((i - 0.5 * (ilen - 1)) / (0.5 * (ilen + 1)));
	    }
	    yy[i] *= 1.0 - fabs((i - 0.5 * (ilen - 1)) / (0.5 * (ilen + 1)));
	    break;
	}
    }
}

/*
 * histograms
 */
int do_histo(int fromgraph, int fromset, int tograph, int toset,
	      double *bins, int nbins, int cumulative, int normalize)
{
    int i, ndata,density;
    int *hist;
    double *x, *y, *data;
    plotarr p;
new_set_no=-1;
    if (!is_set_active(fromgraph, fromset)) {
    errmsg(QObject::tr("Set not active").toLocal8Bit().constData());
	return RETURN_FAILURE;
    }
    if (nbins <= 0) {
    errmsg(QObject::tr("Number of bins <= 0").toLocal8Bit().constData());
	return RETURN_FAILURE;
    }
    if (toset == SET_SELECT_NEXT) {
	toset = nextset(tograph);
    }
    if (!is_valid_setno(tograph, toset)) {
    errmsg(QObject::tr("Can't activate destination set").toLocal8Bit().constData());
        return RETURN_FAILURE;
    }

    if (normalize>=2)
    {
    density=1;
    normalize-=2;
    }
    else
    {
    density=0;
    }
    ndata = getsetlength(fromgraph, fromset);
    data = gety(fromgraph, fromset);
    
    hist = (int *)xmalloc(nbins*sizeof(int));
    if (hist == NULL) {
        errmsg(QObject::tr("xmalloc failed in do_histo()").toLocal8Bit().constData());
        return RETURN_FAILURE;
    }

    if (histogram(ndata, data, nbins, bins, hist) == RETURN_FAILURE){
        xfree(hist);
        return RETURN_FAILURE;
    }

    activateset(tograph, toset);
    setlength(tograph, toset, nbins + 1);
    x = getx(tograph, toset);
    y = gety(tograph, toset);
    
    x[0] = bins[0];
    y[0] = 0.0;
    for (i = 1; i < nbins + 1; i++) {
        x[i] = bins[i];
            if (density==0)
            y[i] = hist[i - 1];
            else
            y[i] = hist[i - 1]/fabs(x[i]-x[i-1]);
        if (cumulative) {
            y[i] += y[i - 1];
        }
    }
    
    if (normalize) {
        for (i = 1; i < nbins + 1; i++) {
            double factor;
            if (cumulative) {
                factor = 1.0/ndata;
            } else {
                factor = 1.0/((bins[i] - bins[i - 1])*ndata);
            }
            y[i] *= factor;
        }
    }
    
    xfree(hist);

    get_graph_plotarr(tograph, toset, &p);
    p.sym = SYM_NONE;
    p.linet = LINE_TYPE_LEFTSTAIR;
    p.dropline = TRUE;
    p.baseline = FALSE;
    p.baseline_type = BASELINE_TYPE_0;
    p.lines = 1;
    p.symlines = 1;
    tmp_str=QObject::tr("Histogram from G")+QString::number(fromgraph)+QObject::tr(".S")+QString::number(fromset);
    //sprintf(p.comments, "Histogram from G%d.S%d", fromgraph, fromset);
    strcpy(p.comments,tmp_str.toLocal8Bit().constData());
    set_graph_plotarr(tograph, toset, &p);
new_set_no=toset;
    return RETURN_SUCCESS;
}

int histogram(int ndata, double *data, int nbins, double *bins, int *hist)
{
    int i, j, bsign;
    double minval, maxval;
    
    if (nbins < 1) {
        errmsg(QObject::tr("Number of bins < 1").toLocal8Bit().constData());
        return RETURN_FAILURE;
    }

    bsign = monotonicity(bins, nbins + 1, TRUE);
    if (bsign == 0) {
        errmsg(QObject::tr("Non-monotonic bins").toLocal8Bit().constData());
        return RETURN_FAILURE;
    }
    
    for (i = 0; i < nbins; i++) {
        hist[i] = 0;
    }
    
    /* TODO: binary search */
    for (i = 0; i < ndata; i++) {
        for (j = 0; j < nbins; j++) {
            if (bsign > 0) {
                minval = bins[j];
                maxval = bins[j + 1];
            } else {
                minval = bins[j + 1];
                maxval = bins[j];
            }
            if (data[i] >= minval && data[i] <= maxval) {
                hist[j] += 1;
                break;
            }
        }
    }
    return RETURN_SUCCESS;
}


/*
 * sample a set, by start/step or logical expression
 */
void do_sample(int setno, int typeno,char *exprstr, int startno, int stepno)
{
    int len, npts = 0, i, resset;
    double *x, *y;
    int reslen;
    double *result;
    int gno = get_cg();
new_set_no=-1;
    if (!is_set_active(gno, setno)) {
    errmsg(QObject::tr("Set not active").toLocal8Bit().constData());
	return;
    }

    len = getsetlength(gno, setno);

    new_set_no = resset = nextset(gno);
    if (resset < 0) {
	return;
    }

    x = getx(gno, setno);
    y = gety(gno, setno);

    if (typeno == 0) {
	if (len <= 2) {
        errmsg(QObject::tr("Set has <= 2 points").toLocal8Bit().constData());
	    return;
	}
	if (startno < 1) {
        errmsg(QObject::tr("Start point < 1 (locations in sets are numbered starting from 1)").toLocal8Bit().constData());
	    return;
	}
	if (stepno < 1) {
        errmsg(QObject::tr("Step < 1").toLocal8Bit().constData());
	    return;
	}
	for (i = startno - 1; i < len; i += stepno) {
	    add_point(gno, resset, x[i], y[i]);
	    npts++;
	}
    //sprintf(buf, "Sample, %d, %d set #%d", startno, stepno, setno);
    tmp_str=QObject::tr("Sample, ")+QString::number(startno)+QObject::tr(", ")+QString::number(stepno)+QObject::tr(" set #")+QString::number(setno);
    } else {
        if (set_parser_setno(gno, setno) != RETURN_SUCCESS)
        {
            tmp_str=QObject::tr("Bad set");
            errmsg(tmp_str.toLocal8Bit().constData());
            killset(gno, resset);
	    return;
        }
        if (v_scanner(exprstr, &reslen, &result) != RETURN_SUCCESS) {
            killset(gno, resset);
	    return;
        }
        if (reslen != len)
        {
            tmp_str=QObject::tr("Internal error");
            errmsg(tmp_str.toLocal8Bit().constData());
            killset(gno, resset);
	    return;
        }

        npts = 0;
    //sprintf(buf, "Sample from %d, using '%s'", setno, exprstr);
    tmp_str=QObject::tr("Sample from ")+QString::number(setno)+QObject::tr(", using '")+QString(exprstr)+QObject::tr("'");
	for (i = 0; i < len; i++) {
        if ( int( rint(result[i]) ) ) {
		add_point(gno, resset, x[i], y[i]);
		npts++;
	    }
	}
        xfree(result);
    }
    if (npts > 0)
    {
    //setcomment(gno, resset, buf);
    setcomment(gno, resset, tmp_str.toLocal8Bit().constData());
    }
}

#define prune_xconv(res,x,xtype)	\
    switch (deltatypeno) {		\
    case PRUNE_VIEWPORT:		\
	res = xy_xconv(x);			\
	break;				\
    case PRUNE_WORLD:			\
	switch (xtype) {		\
	case PRUNE_LIN:			\
	    res = x;			\
	    break;			\
	case PRUNE_LOG:			\
	    res = log(x);		\
	    break;			\
	}				\
    }

#define prune_yconv(res,y,ytype)	\
    switch (deltatypeno) {		\
    case PRUNE_VIEWPORT:		\
	res = xy_yconv(y);			\
	break;				\
    case PRUNE_WORLD:			\
	switch (ytype) {		\
	case PRUNE_LIN:			\
	    res = y;			\
	    break;			\
	case PRUNE_LOG:			\
	    res = log(y);		\
	    break;			\
	}				\
    }

/*
 * Prune data
 */
void do_prune(int setno, int typeno, int deltatypeno, double deltax, double deltay, int dxtype, int dytype)
{
    int len, npts = 0, d, i, j, k, drop, resset;
    double *x, *y, *resx, *resy, xtmp, ytmp, ddx = 0.0, ddy = 0.0;
    double xj = 0.0, xjm = 0.0, xjp = 0.0, yj = 0.0, yjm = 0.0, yjp = 0.0;
new_set_no=-1;
    if (!is_set_active(get_cg(), setno))
    {
        tmp_str=QObject::tr("Set not active");
        errmsg(tmp_str.toLocal8Bit().constData());
        return;
    }
    len = getsetlength(get_cg(), setno);
    if (len <= 2)
    {
        tmp_str=QObject::tr("Set has <= 2 points");
        errmsg(tmp_str.toLocal8Bit().constData());
	return;
    }
    x = getx(get_cg(), setno);
    y = gety(get_cg(), setno);
    switch (typeno) {
    case PRUNE_CIRCLE:
    case PRUNE_ELLIPSE:
    case PRUNE_RECTANGLE:
	deltax = fabs(deltax);
	if (deltax == 0)
	    return;
	break;
    }
    switch (typeno) {
    case PRUNE_CIRCLE:
	deltay = deltax;
	break;
    case PRUNE_ELLIPSE:
    case PRUNE_RECTANGLE:
    case PRUNE_INTERPOLATION:
	deltay = fabs(deltay);
	if (deltay == 0)
	    return;
	break;
    }
    if (deltatypeno == PRUNE_WORLD) {
	if (dxtype == PRUNE_LOG && deltax < 1.0) {
	    deltax = 1.0 / deltax;
	}
	if (dytype == PRUNE_LOG && deltay < 1.0) {
	    deltay = 1.0 / deltay;
	}
    }
    new_set_no = resset = nextset(get_cg());
    if (resset < 0) {
        return;
    }
    add_point(get_cg(), resset, x[0], y[0]);
    npts++;
    resx = getx(get_cg(), resset);
    resy = gety(get_cg(), resset);
    switch (typeno) {
    case PRUNE_CIRCLE:
    case PRUNE_ELLIPSE:
	for (i = 1; i < len; i++) {
	    xtmp = x[i];
	    ytmp = y[i];
	    drop = FALSE;
	    for (j = npts - 1; j >= 0 && drop == FALSE; j--) {
		switch (deltatypeno) {
		case PRUNE_VIEWPORT:
		    ddx = (xy_xconv(xtmp) - xy_xconv(resx[j])) / deltax;
		    if (fabs(ddx) < 1.0) {
			ddy = (xy_yconv(ytmp) - xy_yconv(resy[j])) / deltay;
			if (ddx * ddx + ddy * ddy < 1.0) {
			    drop = TRUE;
			}
		    }
		    break;
		case PRUNE_WORLD:
		    switch (dxtype) {
		    case PRUNE_LIN:
			ddx = (xtmp - resx[j]) / deltax;
			break;
		    case PRUNE_LOG:
			ddx = (xtmp / resx[j]);
			if (ddx < 1.0) {
			    ddx = 1.0 / ddx;
			}
			ddx /= deltax;
			break;
		    }
		    if (fabs(ddx) < 1.0) {
			switch (dytype) {
			case PRUNE_LIN:
			    ddy = (ytmp - resy[j]) / deltay;
			    break;
			case PRUNE_LOG:
			    ddy = (ytmp / resy[j]);
			    if (ddy < 1.0) {
				ddy = 1.0 / ddy;
			    }
			    ddy /= deltay;
			    break;
			}
			if (ddx * ddx + ddy * ddy < 1.0) {
			    drop = TRUE;
			}
		    }
		    break;
		}
	    }
	    if (drop == FALSE) {
		add_point(get_cg(), resset, xtmp, ytmp);
		npts++;
		resx = getx(get_cg(), resset);
		resy = gety(get_cg(), resset);
	    }
	}
    //sprintf(buf, "Prune from %d, %s dx = %g dy = %g", setno, (typeno == 0) ? "Circle" : "Ellipse", deltax, deltay);
    tmp_str=QObject::tr("Prune from ")+QString::number(setno)+QObject::tr(", ")+((typeno == 0) ? QObject::tr("Circle") : QObject::tr("Ellipse"))+QObject::tr(" dx = ")+QString::number(deltax)+QObject::tr(" dy = ")+QString::number(deltay);
	break;
    case PRUNE_RECTANGLE:
	for (i = 1; i < len; i++) {
	    xtmp = x[i];
	    ytmp = y[i];
	    drop = FALSE;
	    for (j = npts - 1; j >= 0 && drop == FALSE; j--) {
		switch (deltatypeno) {
		case PRUNE_VIEWPORT:
		    ddx = fabs(xy_xconv(xtmp) - xy_xconv(resx[j]));
		    if (ddx < deltax) {
			ddy = fabs(xy_yconv(ytmp) - xy_yconv(resy[j]));
			if (ddy < deltay) {
			    drop = TRUE;
			}
		    }
		    break;
		case PRUNE_WORLD:
		    switch (dxtype) {
		    case PRUNE_LIN:
			ddx = fabs(xtmp - resx[j]);
			break;
		    case PRUNE_LOG:
			ddx = (xtmp / resx[j]);
			if (ddx < 1.0) {
			    ddx = 1.0 / ddx;
			}
			break;
		    }
		    if (ddx < deltax) {
			switch (dytype) {
			case PRUNE_LIN:
			    ddy = fabs(ytmp - resy[j]);
			    break;
			case PRUNE_LOG:
			    ddy = (ytmp / resy[j]);
			    if (ddy < 1.0) {
				ddy = 1.0 / ddy;
			    }
			    break;
			}
			if (ddy < deltay) {
			    drop = TRUE;
			}
		    }
		    break;
		}
	    }
	    if (drop == FALSE) {
		add_point(get_cg(), resset, xtmp, ytmp);
		npts++;
		resx = getx(get_cg(), resset);
		resy = gety(get_cg(), resset);
	    }
	}
    //sprintf(buf, "Prune from %d, %s dx = %g dy = %g", setno,"Rectangle", deltax, deltay);
    tmp_str=QObject::tr("Prune from ")+QString::number(setno)+QObject::tr(", Rectangle dx = ")+QString::number(deltax)+QObject::tr(" dy = ")+QString::number(deltay);
	break;
    case PRUNE_INTERPOLATION:
	k = 0;
	prune_xconv(xjm, x[0], dxtype);
	prune_yconv(yjm, y[0], dytype);
	while (k < len - 2) {
	    d = 1;
	    i = k + d + 1;
	    drop = TRUE;
	    while (TRUE) {
		prune_xconv(xjp, x[i], dxtype);
		prune_yconv(yjp, y[i], dytype);
		for (j = k + 1; j < i && drop == TRUE; j++) {
		    prune_xconv(xj, x[j], dxtype);
		    prune_yconv(yj, y[j], dytype);
		    if (xjp == xjm) {
			ytmp = 0.5 * (yjp + yjm);
		    } else {
			ytmp = (yjp*(xj-xjm)+yjm*(xjp-xj))/(xjp-xjm);
		    }
		    switch (deltatypeno) {
		    case PRUNE_VIEWPORT:
			ddy = fabs(ytmp - yj);
			break;
		    case PRUNE_WORLD:
			switch (dytype) {
			case PRUNE_LIN:
			    ddy = fabs(ytmp - yj);
			    break;
			case PRUNE_LOG:
			    ddy = exp(fabs(ytmp - yj));
			    break;
			}
		    }
		    if (ddy > deltay) {
			drop = FALSE;
		    }
		}
		if (drop == FALSE || i == len - 1) {
		    break;
		}
		d *= 2;
		i = k + d + 1;
		if (i >= len) {
		    i = len - 1;
		}
	    }
	    if (drop == FALSE) {
		i = k + 1;
		drop = TRUE;
		while (d > 1) {
		    d /= 2;
		    i += d;
		    prune_xconv(xjp, x[i], dxtype);
		    prune_yconv(yjp, y[i], dytype);
		    drop = TRUE;
		    for (j = k + 1; j < i && drop == TRUE; j++) {
			prune_xconv(xj, x[j], dxtype);
			prune_yconv(yj, y[j], dytype);
			ytmp = (yjp*(xj-xjm)+yjm*(xjp-xj))/(xjp-xjm);
			switch (deltatypeno) {
			case PRUNE_VIEWPORT:
			    ddy = fabs(ytmp - yj);
			    break;
			case PRUNE_WORLD:
			    switch (dytype) {
			    case PRUNE_LIN:
				ddy = fabs(ytmp - yj);
				break;
			    case PRUNE_LOG:
				ddy = exp(fabs(ytmp - yj));
				break;
			    }
			}
			if (ddy > deltay) {
			    drop = FALSE;
			}
		    }
		    if (drop == FALSE) {
			i -= d;
		    }
		}
	    }
	    k = i;
	    prune_xconv(xjm, x[k], dxtype);
	    prune_yconv(yjm, y[k], dytype);
	    add_point(get_cg(), resset, x[k], y[k]);
	    npts++;
	    resx = getx(get_cg(), resset);
	    resy = gety(get_cg(), resset);
	}
	if (k == len - 2) {
	    add_point(get_cg(), resset, x[len-1], y[len-1]);
	    npts++;
	}
    //sprintf(buf, "Prune from %d, %s dy = %g", setno,"Interpolation", deltay);
    tmp_str=QObject::tr("Prune from ")+QString::number(setno)+QObject::tr(", Interpolation dy = ")+QString::number(deltay);
	break;
    }
    //setcomment(get_cg(), resset, buf);
    setcomment(get_cg(), resset, tmp_str.toLocal8Bit().constData());
}

int get_points_inregion(int rno, int invr, int len, double *x, double *y, int *cnt, double **xt, double **yt)
{
    int i, clen = 0;
    double *xtmp, *ytmp;
    *cnt = 0;
    if (isactive_region(rno)) {
	for (i = 0; i < len; i++) {
	    if (invr) {
		if (!inregion(rno, x[i], y[i])) {
		    clen++;
		}
	    } else {
		if (inregion(rno, x[i], y[i])) {
		    clen++;
		}
	    }
	}
	if (clen == 0) {
	    return 0;
	}
	xtmp = (double *) xcalloc(clen, sizeof(double));
	if (xtmp == NULL) {
	    return 0;
	}
	ytmp = (double *) xcalloc(clen, sizeof(double));
	if (ytmp == NULL) {
	    xfree(xtmp);
	    return 0;
	}
	clen = 0;
	for (i = 0; i < len; i++) {
	    if (invr) {
		if (!inregion(rno, x[i], y[i])) {
		    xtmp[clen] = x[i];
		    ytmp[clen] = y[i];
		    clen++;
		}
	    } else {
		if (inregion(rno, x[i], y[i])) {
		    xtmp[clen] = x[i];
		    ytmp[clen] = y[i];
		    clen++;
		}
	    }
	}
    } else {
	return 0;
    }
    *cnt = clen;
    *xt = xtmp;
    *yt = ytmp;
    return 1;
}

int interpolate(double *mesh, double *yint, int meshlen,
    double *x, double *y, int len, int method)
{
    double *b, *c, *d;
    double dx;
    int i, ifound;
    int m;

    /* For linear interpolation, non-strict monotonicity is fine */
    m = monotonicity(x, len, method == INTERP_LINEAR ? FALSE:TRUE);
    if (m == 0) {
        errmsg(QObject::tr("Can't interpolate a set with non-monotonic abscissas").toLocal8Bit().constData());
        return RETURN_FAILURE;
    }

    switch (method) {
    case INTERP_SPLINE:
    case INTERP_ASPLINE:
        b = (double *)xcalloc(len, sizeof(double));
        c = (double *)xcalloc(len, sizeof(double));
        d = (double *)xcalloc(len, sizeof(double));
        if (b == NULL || c == NULL || d == NULL) {
            xfree(b);
            xfree(c);
            xfree(d);
            return RETURN_FAILURE;
        }
        if (method == INTERP_ASPLINE){
            /* Akima spline */
            aspline(len, x, y, b, c, d);
        } else {
            /* Plain cubic spline */
            spline(len, x, y, b, c, d);
        }

	seval(mesh, yint, meshlen, x, y, b, c, d, len);

        xfree(b);
        xfree(c);
        xfree(d);
        break;
    default:
        /* linear interpolation */

        for (i = 0; i < meshlen; i++) {
            ifound = find_span_index(x, len, m, mesh[i]);
            if (ifound < 0) {
                ifound = 0;
            } else if (ifound > len - 2) {
                ifound = len - 2;
            }
            dx = x[ifound + 1] - x[ifound];
            if (dx != 0.0) {
                yint[i] = y[ifound] + (mesh[i] - x[ifound])*
                    ((y[ifound + 1] - y[ifound])/dx);
            } else {
                yint[i] = (y[ifound] + y[ifound + 1])/2;
            }
        }
        break;
    }
    
    return RETURN_SUCCESS;
}

/* interpolate a set at abscissas from mesh
 * method - type of spline (or linear interpolation)
 * if strict is set, perform interpolation only within source set bounds
 * (i.e., no extrapolation)
 */
int do_interp(int gno_src, int setno_src, int gno_dest, int setno_dest,
    double * mesh, int meshlen, int method, int strict)
{
    int len, n, ncols;
    double *x, *xint;
    double * mesh1=mesh;
const char * s;
new_set_no=-1;
    if (!is_valid_setno(gno_src, setno_src))
    {
        tmp_str=QObject::tr("Interpolated set not active");
        errmsg(tmp_str.toLocal8Bit().constData());
	return RETURN_FAILURE;
    }

    if (strict!=SAMPLING_PARAMETRIC)
    if (mesh == NULL || meshlen < 1)
    {
        tmp_str=QObject::tr("NULL sampling mesh");
        errmsg(tmp_str.toLocal8Bit().constData());
        return RETURN_FAILURE;
    }
    
    len = getsetlength(gno_src, setno_src);
    ncols = dataset_cols(gno_src, setno_src);

    if (setno_dest == SET_SELECT_NEXT)
    {
	setno_dest = nextset(gno_dest);
    }

    if (!is_valid_setno(gno_dest, setno_dest))
    {
        tmp_str=QObject::tr("Can't activate destination set");
        errmsg(tmp_str.toLocal8Bit().constData());
	return RETURN_FAILURE;
    }

    double *y;
    double * yint[MAX_SET_COLS];
    if (gno_src==gno_dest && setno_src==setno_dest)
    {//set already exists, we do the length-modification later
        for (n = 1; n < ncols; n++) yint[n]=new double[meshlen];
    }
    else
    {//new set
    //qDebug() << "NEW SET: COPY OLD SET";
        if (dataset_cols(gno_dest, setno_dest) != ncols)
        {
        copyset(gno_src, setno_src, gno_dest, setno_dest);
        }
        setlength(gno_dest, setno_dest, meshlen);
        activateset(gno_dest, setno_dest);
    }
//qDebug() << "Meshlen=" << meshlen;
//qDebug() << "Meshlen=" << meshlen << "st=" << mesh[0] << "en=" << mesh[meshlen-1];
    int n0;
    if (strict==SAMPLING_PARAMETRIC)
    {
    x = new double[1+len];
    for (int i=0;i<len;i++) x[i]=i;
    n0=0;
    mesh1 = allocate_mesh(0,len-1, meshlen);
    }
    else
    {
    x = getcol(gno_src, setno_src, DATA_X);
    n0=1;
    }
    for (n = n0; n < ncols; n++)
    {
        int res;
        y = getcol(gno_src, setno_src, n);
            if (!(gno_src==gno_dest && setno_src==setno_dest))
            {
            yint[n] = getcol(gno_dest, setno_dest, n);
            }
        res = interpolate(mesh1, yint[n], meshlen, x, y, len, method);
            if (res != RETURN_SUCCESS)
            {
                if (gno_src==gno_dest && setno_src==setno_dest)
                {
                for (n = 1; n < ncols; n++) delete[] yint[n];
                }
                else
                {
                killset(gno_dest, setno_dest);
                }
            return RETURN_FAILURE;
            }
    }
    if (gno_src==gno_dest && setno_src==setno_dest)
    {
        setlength(gno_dest, setno_dest, meshlen);
        activateset(gno_dest, setno_dest);
        for (n = n0; n < ncols; n++)
        {
            y = getcol(gno_dest, setno_dest, n);
            memcpy(y, yint[n], meshlen*sizeof(double));
            delete[] yint[n];
        }
    }

new_set_no=setno_dest;
    if (strict!=SAMPLING_PARAMETRIC)
    {
    xint = getcol(gno_dest, setno_dest, DATA_X);
    memcpy(xint, mesh1, meshlen*sizeof(double));
    }

//qDebug() << "We do the strict-thing now";
    if (strict>0 && strict!=SAMPLING_PARAMETRIC) {
        double xmin, xmax;
        int i, imin, imax;
        minmax(x, len, &xmin, &xmax, &imin, &imax);
        //qDebug() << "in strict: xmin=" << xmin << "xmax=" << xmax;
        for (i = meshlen - 1; i >= 0; i--) {
            if (xint[i] < xmin || xint[i] > xmax) {
                del_point(gno_dest, setno_dest, i);
            }
        }
    }
    
    switch (method)
    {
    case INTERP_SPLINE:
        s = QObject::tr("cubic spline").toLocal8Bit().constData();
        break;
    case INTERP_ASPLINE:
        s = QObject::tr("Akima spline").toLocal8Bit().constData();
        break;
    default:
        s = QObject::tr("linear interpolation").toLocal8Bit().constData();
        break;
    }
    //sprintf(buf, "Interpolated from G%d.S%d using %s", gno_src, setno_src, s);
    //setcomment(gno_dest, setno_dest, buf);
    tmp_str=QObject::tr("Interpolated from G")+QString::number(gno_src)+QObject::tr(".S")+QString::number(setno_src)+QObject::tr(" using ")+QString(s);
    setcomment(gno_dest, setno_dest, tmp_str.toLocal8Bit().constData());
    if (strict==SAMPLING_PARAMETRIC)
    {
    delete[] x;
    XCFREE(mesh1);
    }
    return RETURN_SUCCESS;
}

int get_restriction_array(int gno, int setno, int rtype, int negate, char **rarray)
{
    int i, n, regno;
    double *x, *y;
    world w;
    WPoint wp;
    
    if (rtype == RESTRICT_NONE) {
        *rarray = NULL;
        return RETURN_SUCCESS;
    }
    
    n = getsetlength(gno, setno);
    if (n <= 0) {
        *rarray = NULL;
        return RETURN_FAILURE;
    }
    
    *rarray = (char*)xmalloc(n*sizeof(char));
    if (*rarray == NULL) {
        return RETURN_FAILURE;
    }
    
    x = getcol(gno, setno, DATA_X);
    y = getcol(gno, setno, DATA_Y);
    
    switch (rtype) {
    case RESTRICT_REG0:
    case RESTRICT_REG1:
    case RESTRICT_REG2:
    case RESTRICT_REG3:
    case RESTRICT_REG4:
        regno = rtype - RESTRICT_REG0;
        for (i = 0; i < n; i++) {
            (*rarray)[i] = inregion(regno, x[i], y[i]) ? !negate : negate;
        }
        break;
    case RESTRICT_WORLD:
        get_graph_world(gno, &w);
        for (i = 0; i < n; i++) {
            wp.x = x[i];
            wp.y = y[i];
            (*rarray)[i] = is_wpoint_inside(&wp, &w) ? !negate : negate;
        }
        break;
    default:
        tmp_str=QObject::tr("Internal error in get_restriction_array()");
        errmsg(tmp_str.toLocal8Bit().constData());
        XCFREE(*rarray);
        return RETURN_FAILURE;
        break;
    }
    return RETURN_SUCCESS;
}

int monotonicity(double *array, int len, int strict)
{
    int i;
    int s0, s1;
    
    if (len < 2)
    {
        tmp_str=QObject::tr("Monotonicity of an array of length < 2 is meaningless");
        errmsg(tmp_str.toLocal8Bit().constData());
        return 0;
    }
    
    s0 = sign(array[1] - array[0]);
    for (i = 2; i < len; i++) {
        s1 = sign(array[i] - array[i - 1]);
        if (s1 != s0) {
            if (strict) {
                return 0;
            } else if (s0 == 0) {
                s0 = s1;
            } else if (s1 != 0) {
                return 0;
            }
        }
    }
    
    return s0;
}

int find_span_index(double *array, int len, int m, double x)
{
    int ind, low = 0, high = len - 1;
    
    if (len < 2 || m == 0)
    {
        tmp_str=QObject::tr("find_span_index() called with a non-monotonic array");
        errmsg(tmp_str.toLocal8Bit().constData());
        return -2;
    } else if (m > 0) {
        /* ascending order */
        if (x < array[0]) {
            return -1;
        } else if (x > array[len - 1]) {
            return len - 1;
        } else {
            while (low <= high) {
	        ind = (low + high) / 2;
	        if (x < array[ind]) {
	            high = ind - 1;
	        } else if (x > array[ind + 1]) {
	            low = ind + 1;
	        } else {
	            return ind;
	        }
            }
        }
    } else {
        /* descending order */
        if (x > array[0]) {
            return -1;
        } else if (x < array[len - 1]) {
            return len - 1;
        } else {
            while (low <= high) {
	        ind = (low + high) / 2;
	        if (x > array[ind]) {
	            high = ind - 1;
	        } else if (x < array[ind + 1]) {
	            low = ind + 1;
	        } else {
	            return ind;
	        }
            }
        }
    }

    /* should never happen */
    tmp_str=QObject::tr("internal error in find_span_index()");
    errmsg(tmp_str.toLocal8Bit().constData());
    return -2;
}

int execute_special_filter_command(QString command,QList<int> orig_gno,QList<int> orig_sno,QList<int> target_gno,QList<int> target_sno)
{
int *o_gnos=NULL,*o_snos=NULL;
int *gnos=NULL,*snos=NULL;
double limits[2],ripple;
int orders[2];
char x_formula[512];
char * com=new char[command.length()*2+8];
strcpy(com,command.toLatin1().constData());
int o_n_sets, n_sets, type, realization, absolute, debug, point_extension, oversampling, rno, invr;
int errors,retval2;
ParseFilterCommand(com+28,o_n_sets,&o_gnos,&o_snos,n_sets,&gnos,&snos,type,realization,limits,orders,x_formula,ripple,absolute,debug,point_extension,oversampling,rno,invr);
if (orig_gno.length()>0)//we are to execute the filter command with different set-IDs
{
delete[] o_gnos;
delete[] o_snos;
delete[] gnos;
delete[] snos;
o_n_sets=n_sets=orig_gno.length();
o_gnos=new int[n_sets];
o_snos=new int[n_sets];
gnos=new int[n_sets];
snos=new int[n_sets];
    for (int i=0;i<n_sets;i++)
    {
    o_gnos[i]=orig_gno.at(i);
    o_snos[i]=orig_sno.at(i);
    gnos[i]=target_gno.at(i);
    snos[i]=target_sno.at(i);
    }
}
errors=0;
for (int ii=0;ii<n_sets;ii++)
{
retval2=do_filter_on_one_set(gnos[ii],snos[ii],o_gnos[ii],o_snos[ii],type,realization,rno,invr,absolute,debug,x_formula,point_extension,oversampling,ripple,orders[0],orders[1],limits[0],limits[1]);
if (retval2!=RETURN_SUCCESS) (errors)++;
}
delete[] com;
delete[] o_gnos;
delete[] o_snos;
delete[] gnos;
delete[] snos;
return errors;
}

void WriteFilterString(QString & filterCommand,int o_n_sets,int * o_gnos,int * o_snos,int n_sets,int * gnos,int * snos,int type,int realization,double * limits,int * orders,char * x_formula,double ripple,int absolute,int debug,int point_extension,int oversampling,int rno,int invr)
{
int id[10];
char dummy2[512],tmp_str[512];
id[0]=type;
id[1]=realization;
id[2]=orders[0];
id[3]=orders[1];
id[4]=absolute;
id[5]=debug;
id[6]=point_extension;
id[7]=oversampling;
id[8]=rno;
id[9]=invr;

filterCommand=QString("#QTGRACE_SPECIAL FILTER_SET ");

sprintf(dummy2,"%d,%d",o_n_sets,n_sets);
filterCommand+=QString(dummy2);
strcpy(dummy2,"");
for (int i=0;i<o_n_sets;i++)
{
sprintf(tmp_str,"%d,%d",o_gnos[i],o_snos[i]);
strcat(dummy2,tmp_str);
if (i<o_n_sets-1) strcat(dummy2,";");
}//original sets
filterCommand+=QString("{") + QString(dummy2) + QString("}");

strcpy(dummy2,"");
for (int i=0;i<n_sets;i++)
{
sprintf(tmp_str,"%d,%d",gnos[i],snos[i]);
strcat(dummy2,tmp_str);
if (i<n_sets-1) strcat(dummy2,";");
}//target sets
filterCommand+=QString("{") + QString(dummy2) + QString("}");

strcpy(dummy2,"");
for (int i=0;i<10;i++)
{
sprintf(tmp_str,"%d",id[i]);
strcat(dummy2,tmp_str);
if (i<9) strcat(dummy2,";");
}//integer parameters
sprintf(tmp_str,";%f;%f;%f;%s",limits[0],limits[1],ripple,x_formula);
strcat(dummy2,tmp_str);
filterCommand+=QString("{") + QString(dummy2) + QString("}");
//cout << "Filter Command: " << filterCommand.toLatin1().constData() << endl;
}

int execute_regression_command(QString & command,QList<int> orig_gno,QList<int> orig_sno,QList<int> target_gno,QList<int> target_sno)
{
int *o_gnos=NULL,*o_snos=NULL;
int *gnos=NULL,*snos=NULL;
int o_n_sets, n_sets;
char * com=new char[command.length()*2+8];
strcpy(com,command.toLatin1().constData());
int ideg,iresid,rno,invr,points,rx;
double start,stop;
char formula[512];
char x_formula[512];
ParseRegression(com+28,o_n_sets,&o_gnos,&o_snos,n_sets,&gnos,&snos,ideg,iresid,rno,invr,start,stop,points,rx,formula);
if (orig_gno.length()>0)//we are to execute the filter command with different set-IDs
{
delete[] o_gnos;
delete[] o_snos;
delete[] gnos;
delete[] snos;
o_n_sets=n_sets=orig_gno.length();
o_gnos=new int[n_sets];
o_snos=new int[n_sets];
gnos=new int[n_sets];
snos=new int[n_sets];
    for (int i=0;i<n_sets;i++)
    {
    o_gnos[i]=orig_gno.at(i);
    o_snos[i]=orig_sno.at(i);
    gnos[i]=target_gno.at(i);
    snos[i]=target_sno.at(i);
    }
}
for (int ii=0;ii<o_n_sets;ii++)
{
    if (is_set_active(o_gnos[ii],o_snos[ii])==TRUE)
    {//set exists --> do regression
        if( rx == 2 )//rx
        {
            if (generate_x_mesh_from_formula(gnos[ii],snos[ii],start,stop,points,x_formula,SET_XY)==RETURN_FAILURE)
            {
                errwin(QObject::tr("Not enough sets").toLocal8Bit().constData());
                return RETURN_FAILURE;
            }
        }
        do_regress(o_gnos[ii],o_snos[ii],ideg,iresid,rno,invr,snos[ii]);
    }
}
delete[] o_gnos;
delete[] o_snos;
delete[] gnos;
delete[] snos;
delete[] com;
return RETURN_SUCCESS;
}

void WriteRegressionString(QString & regressionCommand,int n_sets,int * gnos,int * snos,int n_n_sets,int * n_gnos,int * n_snos,int ideg,int rno,int invr,double start,double stop,int points,int rx,char * formula)
{
char dummy2[512];
    regressionCommand=QString("#QTGRACE_SPECIAL REGRESSION ");
    sprintf(dummy2,"%d,%d{",n_sets,n_n_sets);
    regressionCommand+=QString(dummy2);
for (int i=0;i<n_sets;i++)
{
sprintf(dummy2,"%d,%d",gnos[i],snos[i]);
    regressionCommand+=QString(dummy2);
    if (i<n_sets-1) regressionCommand+=QString(";");
}
regressionCommand+=QString("}{");
for (int i=0;i<n_n_sets;i++)
{
    sprintf(dummy2,"%d,%d",n_gnos[i],n_snos[i]);
    regressionCommand+=QString(dummy2);
    if (i<n_n_sets-1) regressionCommand+=QString(";");
}
regressionCommand+=QString("}{");
sprintf(dummy2,"%d;%d;%d;%d;%d;%f;%f;%s",ideg,rno,invr,points,rx,start,stop,formula);
regressionCommand+=QString(dummy2)+QString("}");
}

//y=f(x), we know f and y and want to find x, we need an initial guess for x
//in formula the 'x' should be '$t'
int find_x_for_y(QString formula,double * x,double guess,double y)
{
double lim1,lim2,y1,y2,deltax,dy1,dy2;
double lim3,y3,dy3;
int ret,counter=0;
lim1=guess*0.95;
lim2=guess*1.05;
deltax=lim2-lim1;
QString t_formula_o(formula);
QString t_formula;
char * form=new char[t_formula_o.length()+64];
int ok=RETURN_FAILURE;
//qDebug() << "y=" << y << "guess=" << guess;
while (ok==RETURN_FAILURE && counter<1000)
{
t_formula=t_formula_o;
t_formula=t_formula.replace("$t",QString::number(lim1,'g',10));
strcpy(form,t_formula.toLatin1().constData());
ret=std_evalexpr(form,&y1);
    if (ret==RETURN_FAILURE) goto end_func_find_x;
t_formula=t_formula_o;
t_formula=t_formula.replace("$t",QString::number(lim2,'g',10));
strcpy(form,t_formula.toLatin1().constData());
ret=std_evalexpr(form,&y2);
    if (ret==RETURN_FAILURE) goto end_func_find_x;
//qDebug() << "counter=" << counter << "lim1=" << lim1 << "y1=" << y1 << "lim2=" << lim2 << "y2=" << y2;
dy1=fabs(y1-y);
dy2=fabs(y2-y);
if ((y1>=y && y2<y) || (y1<=y && y2>y))//the ideal starting position
{
//qDebug() << "ok";
ok=RETURN_SUCCESS;
}
else
{
//qDebug() << "NOT ok";
ok=RETURN_FAILURE;
    if (dy1<dy2)//lim1 is closer
    {
    lim1-=deltax;
    lim2-=deltax;
    }
    else
    {
    lim1+=deltax;
    lim2+=deltax;
    }
}
counter++;
}

if (ok==RETURN_FAILURE)
{
goto end_func_find_x;
}
//now we start the real iteration
counter=0;
while (counter<32)
{
lim3=lim1+dy1*(lim2-lim1)/(y2-y1);
t_formula=t_formula_o;
t_formula=t_formula.replace("$t",QString::number(lim3,'g',10));
strcpy(form,t_formula.toLatin1().constData());
ret=std_evalexpr(form,&y3);
    if (ret==RETURN_FAILURE) goto end_func_find_x;
dy3=fabs(y3-y);
    if (y1<=y && y2>y)
    {
        if (y3>y)//replace y2
        {
        y2=y3;
        dy2=dy3;
        lim2=lim3;
        }
        else//replace y1
        {
        y1=y3;
        dy1=dy3;
        lim1=lim3;
        }
    }
    else//(y1>y && y2<=y)
    {
        if (y3>y)//replace y1
        {
        y1=y3;
        dy1=dy3;
        lim1=lim3;
        }
        else//replace y2
        {
        y2=y3;
        dy2=dy3;
        lim2=lim3;
        }
    }
counter++;
}
*x=lim3;
//qDebug() << "Result: y=" << y << "x=" << *x;
end_func_find_x:
if (ret==RETURN_FAILURE) ok=ret;
delete[] form;
return ok;
}
