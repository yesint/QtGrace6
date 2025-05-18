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
 * routines to allocate, manipulate, and return
 * information about sets.
 *
 */

///#include <config.h>
#include <cmath>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "globals.h"
#include "utils.h"
#include "files.h"
#include "graphs.h"
#include "noxprotos.h"
#include "undo_module.h"
#include "mathstuff.h"
#include <iostream>
#include <fstream>
#include <QDebug>
#include <QObject>
#include <cfloat>
#include <QString>

using namespace std;

extern graph *g;
extern int new_set_no;
extern int * new_set_nos;
extern int * new_graph_nos;
extern QString newLegendString,newCommentString;

/*
 * for sorting a set
 */
static double *vptr;
static int compare_points1(const void *p1, const void *p2);
static int compare_points2(const void *p1, const void *p2);

extern void append_to_storage(int * l,int ** storage,int n,int * new_entries);
extern void append_to_storage2(int * l,int ** storage_a,int ** storage_b,int n,int * new_entries_a,int * new_entries_b);

extern "C" {
extern double *allocate_mesh(double start, double stop, int len);
}
extern int interpolate(double *mesh, double *yint, int meshlen,double *x, double *y, int len, int method);
extern void errwin(const char *s);
extern void ReplaceDecimalSeparator(char * str,bool remove_space=true);
extern void splitStringIntoSubstrings(QString text,QChar separator,QStringList & results);

extern void complete_LaTeX_to_Grace_Translator(QString & text);
extern QString generateDisplayStringFromGraceString(char * gracestring);
extern void generate_string_Qt_aware(char * str,QString text);

/*
 * return the string version of the set type
 */
const char *set_types(int it)
{
const char *s = "xy";
    switch (it) {
    case SET_XY:
	s = "xy";
	break;
    case SET_BAR:
	s = "bar";
	break;
    case SET_BARDY:
	s = "bardy";
	break;
    case SET_BARDYDY:
	s = "bardydy";
	break;
    case SET_XYZ:
	s = "xyz";
	break;
    case SET_XYDX:
	s = "xydx";
	break;
    case SET_XYDY:
	s = "xydy";
	break;
    case SET_XYDXDX:
	s = "xydxdx";
	break;
    case SET_XYDYDY:
	s = "xydydy";
	break;
    case SET_XYDXDY:
	s = "xydxdy";
	break;
    case SET_XYDXDXDYDY:
	s = "xydxdxdydy";
	break;
    case SET_XYHILO:
	s = "xyhilo";
	break;
    case SET_XYR:
	s = "xyr";
	break;
    case SET_XYCOLOR:
	s = "xycolor";
	break;
    case SET_XYCOLPAT:
	s = "xycolpat";
	break;
    case SET_XYVMAP:
	s = "xyvmap";
	break;
    case SET_BOXPLOT:
	s = "xyboxplot";
	break;
    case SET_XYSIZE:
	s = "xysize";
	break;
    case SET_BAND:
    s = "xyband";
    }
return s;
}

int get_settype_by_name(char *s)
{
int i;
    for (i = 0; i < NUMBER_OF_SETTYPES; i++) {
        if (strcmp(set_types(i), s) == 0) {
            return i;
        }
    }
return SET_BAD;
}

int settype_cols(int type)
{
int ncols;
    switch (type) {
    case SET_XY:
    case SET_BAR:
    case SET_BAND:
	ncols = 2;
	break;
    case SET_XYDX:
    case SET_XYDY:
    case SET_XYZ:
    case SET_BARDY:
    case SET_XYR:
    case SET_XYCOLOR:
    case SET_XYSIZE:
	ncols = 3;
	break;
    case SET_XYDXDX:
    case SET_XYDYDY:
    case SET_XYDXDY:
    case SET_BARDYDY:
    case SET_XYCOLPAT:
    case SET_XYVMAP:
	ncols = 4;
	break;
    case SET_XYHILO:
	ncols = 5;
	break;
    case SET_XYDXDXDYDY:
    case SET_BOXPLOT:
	ncols = 6;
	break;
    default:
        ncols = 0;
        break;
    }
return ncols;
}

int get_number_of_columns(int gno,int sno)
{
if (is_valid_setno(gno,sno)==FALSE) return 0;
return settype_cols(g[gno].p[sno].type);
}

/*
 * return the string version of the dataset column
 */
const char * dataset_colname(int col)
{
const char *s;
    switch (col) {
    case 0:
	s = "X";
	break;
    case 1:
	s = "Y";
	break;
    case 2:
	s = "Y1";
	break;
    case 3:
	s = "Y2";
	break;
    case 4:
	s = "Y3";
	break;
    case 5:
	s = "Y4";
	break;
    default:
	s = "?";
	errmsg("Internal error in dataset_colname()");
        break;
    }
return s;
}

int zero_set_data(Dataset *dsp)
{
    int k;
    
    if (dsp) {
        dsp->len = 0;
        for (k = 0; k < MAX_SET_COLS; k++) {
	    dsp->ex[k] = NULL;
        }
        dsp->s = NULL;
        dsp->orig_s = NULL;
        return RETURN_SUCCESS;
    } else {
        return RETURN_FAILURE;
    }
}

/*
 * free set data
 */
int free_set_data(Dataset *dsp)
{
    int k;
    
    if (dsp) {
        if (dsp->len) {
            for (k = 0; k < MAX_SET_COLS; k++) {
	        XCFREE(dsp->ex[k]);
            }
            if (dsp->s) {
	        for (k = 0; k < dsp->len; k++) {
		    XCFREE(dsp->s[k]);
            XCFREE(dsp->orig_s[k]);
	        }
                XCFREE(dsp->s);
                XCFREE(dsp->orig_s);
            }
            dsp->len = 0;
	    set_dirtystate();
        }
        return RETURN_SUCCESS;
    } else {
        return RETURN_FAILURE;
    }
}
 
/*
 * free set data, but preserve the parameter settings
 */
void killsetdata(int gno, int setno)
{
    if (is_valid_setno(gno, setno)) {
        //qDebug() << g[gno].p[setno].orig_lstr << " " << g[gno].p[setno].lstr;
        free_set_data(&g[gno].p[setno].data);
        //qDebug() << g[gno].p[setno].orig_lstr << " " << g[gno].p[setno].lstr;
        ///close_ss_editor(gno,setno);
        /// strcpy(g[gno].p[setno].orig_lstr,g[gno].p[setno].lstr);
        strcpy(g[gno].p[setno].comments,g[gno].p[setno].orig_comments);
        strcpy(g[gno].p[setno].lstr,g[gno].p[setno].orig_lstr);
    }
}

/*
 * (re)allocate data arrays for a set of length len.
 */
int setlength(int gno, int setno, int len)
{
    plotarr *p;
    int i, j, ncols, oldlen;

    if (is_valid_setno(gno, setno) != TRUE) {
        return RETURN_FAILURE;
    }
    
    p = &g[gno].p[setno];

    oldlen = p->data.len;
    if (len == oldlen) {
	return RETURN_SUCCESS;
    }
    if (len < 0) {
	return RETURN_FAILURE;
    }
    
    ncols = settype_cols(p->type);
    
    if (ncols == 0) {
	errmsg("Set type not found in setlength()!");
	return RETURN_FAILURE;
    }
    
    for (i = 0; i < ncols; i++) {
	if ((p->data.ex[i] = (double *)xrealloc(p->data.ex[i], len*sizeof(double))) == NULL
            && len != 0) {
	    return RETURN_FAILURE;
	}
        for (j = oldlen; j < len; j++) {
            p->data.ex[i][j] = 0.0;
        }
    }
    
    if (p->data.s != NULL) {
        for (i = len; i < oldlen; i++) {
            xfree(p->data.s[i]);
            xfree(p->data.orig_s[i]);
        }
        p->data.s = (char**)xrealloc(p->data.s, len*sizeof(char *));
        p->data.orig_s = (char**)xrealloc(p->data.orig_s, len*sizeof(char *));
        for (j = oldlen; j < len; j++) {
            p->data.s[j] = copy_string(NULL, "");
            p->data.orig_s[j] = copy_string(NULL, "");
        }
    }
    
    p->data.len = len;

    set_dirtystate();
    
    return RETURN_SUCCESS;
}

/*
 * moveset 
 */
int moveset(int gnofrom, int setfrom, int gnoto, int setto)
{
    if (gnoto == gnofrom && setfrom == setto) {
	return RETURN_FAILURE;
    }

    if (is_valid_setno(gnofrom, setfrom) != TRUE) {
    return RETURN_FAILURE;
    }

    if (is_set_active(gnoto, setto)) {
	killset(gnoto, setto);
    }
    activateset(gnoto, setto);

    memcpy(&g[gnoto].p[setto], &g[gnofrom].p[setfrom], sizeof(plotarr));

    zero_set_data(&g[gnofrom].p[setfrom].data);

    g[gnofrom].p[setfrom].lstr[0]='\0';
    g[gnofrom].p[setfrom].orig_lstr[0]='\0';
    g[gnofrom].p[setfrom].comments[0]='\0';
    g[gnofrom].p[setfrom].orig_comments[0]='\0';

    g[gnofrom].p[setfrom].hidden = TRUE;
    
    set_dirtystate();
    return RETURN_SUCCESS;
}

/*
 * copy a set to another set, if the 'to'-set doesn't exist allocate it
 */
int copyset(int gfrom, int setfrom, int gto, int setto)
{
    int i, k, len, ncols;
    double *savec[MAX_SET_COLS];
    char **saves;
    char **orig_saves;
    //char buf[256];
    QString tmp_str;

    if (!is_set_active(gfrom, setfrom)) {
	return RETURN_FAILURE;
    }
    if (!is_valid_gno(gto)) {
	return RETURN_FAILURE;
    }
    if (setfrom == setto && gfrom == gto) {
	return RETURN_FAILURE;
    }
    if (is_set_active(gto, setto)) {
	killset(gto, setto);
    }
    len = getsetlength(gfrom, setfrom);
    ncols = dataset_cols(gfrom, setfrom);
    activateset(gto, setto);
    set_dataset_type(gto, setto, dataset_type(gfrom, setfrom));
    if (setlength(gto, setto, len) != RETURN_SUCCESS) {
	return RETURN_FAILURE;
    }
    if (g[gfrom].p[setfrom].data.s != NULL) {
        if ((g[gto].p[setto].data.s = (char**)xmalloc(len*sizeof(char *))) == NULL) {
	    return RETURN_FAILURE;
        }
        if ((g[gto].p[setto].data.orig_s = (char**)xmalloc(len*sizeof(char *))) == NULL) {
        return RETURN_FAILURE;
        }
    }

    for (k = 0; k < MAX_SET_COLS; k++) {
	savec[k] = getcol(gto, setto, k);
    }
    saves = get_set_strings(gto, setto);
    orig_saves = get_set_original_strings(gto, setto);
    memcpy(&g[gto].p[setto], &g[gfrom].p[setfrom], sizeof(plotarr));
    for (k = 0; k < ncols; k++) {
	g[gto].p[setto].data.ex[k] = savec[k];
	memcpy(g[gto].p[setto].data.ex[k],
               g[gfrom].p[setfrom].data.ex[k],
               len*sizeof(double));///SIZEOF_DOUBLE);
    }
    g[gto].p[setto].data.s = saves;
    g[gto].p[setto].data.orig_s = orig_saves;
    if (g[gfrom].p[setfrom].data.s != NULL) {
        for (i = 0; i < len; i++) {
         g[gto].p[setto].data.s[i] = copy_string(NULL, g[gfrom].p[setfrom].data.s[i]);
         g[gto].p[setto].data.orig_s[i] = copy_string(NULL, g[gfrom].p[setfrom].data.orig_s[i]);
        }
    }

    tmp_str=QObject::tr("copy of set ")+QString("G")+QString::number(gfrom)+QString(".S")+QString::number(setfrom);
    //sprintf(buf, "copy of set G%d.S%d", gfrom, setfrom);
    //setcomment(gto, setto, buf);
    setcomment(gto, setto, tmp_str.toLocal8Bit().constData());
    set_dirtystate();
    return RETURN_SUCCESS;
}

/*
 * same as copyset(), but doesn't alter the to set appearance
 */
int copysetdata(int gfrom, int setfrom, int gto, int setto)
{
    int i, k, len, ncols;
    //char buf[256];
    QString tmp_str;

    if (!is_set_active(gfrom, setfrom)) {
	return RETURN_FAILURE;
    }
    if (!is_valid_gno(gto)) {
	return RETURN_FAILURE;
    }
    if (setfrom == setto && gfrom == gto) {
	return RETURN_FAILURE;
    }
    if (is_set_active(gto, setto)) {
	killsetdata(gto, setto);
    }
    len = getsetlength(gfrom, setfrom);
    ncols = dataset_cols(gfrom, setfrom);
    activateset(gto, setto);
    if (dataset_cols(gto, setto) != ncols) {
        set_dataset_type(gto, setto, dataset_type(gfrom, setfrom));
    }
    if (setlength(gto, setto, len) != RETURN_SUCCESS) {
        return RETURN_FAILURE;
    }
    if (g[gfrom].p[setfrom].data.s != NULL) {
        if ((g[gto].p[setto].data.s = (char**)xmalloc(len*sizeof(char *))) == NULL) {
	    return RETURN_FAILURE;
        }
        if ((g[gto].p[setto].data.orig_s = (char**)xmalloc(len*sizeof(char *))) == NULL) {
        return RETURN_FAILURE;
        }
    }
    for (k = 0; k < ncols; k++) {
	memcpy(g[gto].p[setto].data.ex[k],
               g[gfrom].p[setfrom].data.ex[k],
               len*sizeof(double));///SIZEOF_DOUBLE);
    }
    if (g[gfrom].p[setfrom].data.s != NULL) {
        for (i = 0; i < len; i++) {
         g[gto].p[setto].data.s[i] = copy_string(NULL, g[gfrom].p[setfrom].data.s[i]);
         g[gto].p[setto].data.orig_s[i] = copy_string(NULL, g[gfrom].p[setfrom].data.orig_s[i]);
        }
    }
    //sprintf(buf, "copy of setdata G%d.S%d", gfrom, setfrom);
    //setcomment(gto, setto, buf);
    tmp_str=QObject::tr("copy of setdata ")+QString("G")+QString::number(gfrom)+QString(".S")+QString::number(setfrom);
    setcomment(gto, setto, tmp_str.toLocal8Bit().constData());
    set_dirtystate();
    return RETURN_SUCCESS;
}

/*
 * swap a set with another set
 */
int swapset(int gno1, int setno1, int gno2, int setno2)
{
    plotarr p;

    if (is_valid_setno(gno1, setno1) == FALSE ||
        is_valid_setno(gno2, setno2) == FALSE) {
	return RETURN_FAILURE;
    }
    if (setno1 == setno2 && gno1 == gno2) {
	return RETURN_FAILURE;
    }

    memcpy(&p, &g[gno2].p[setno2], sizeof(plotarr));
    memcpy(&g[gno2].p[setno2], &g[gno1].p[setno1], sizeof(plotarr));
    memcpy(&g[gno1].p[setno1], &p, sizeof(plotarr));

    set_dirtystate();
    
    return RETURN_SUCCESS;
}

/*
 * kill a set
 */
void killset(int gno, int setno)
{
    if (is_valid_setno(gno, setno))
    {
    killsetdata(gno, setno);
	set_default_plotarr(&g[gno].p[setno]);
        ///close_ss_editor(gno,setno);
    }
}

double *getcol(int gno, int setno, int col)
{
    if (is_valid_setno(gno, setno)) {
        return g[gno].p[setno].data.ex[col];
    } else {
        return NULL;
    }
}

void setcol(int gno, int setno, int col, double *x, int len)
{
    if (is_valid_setno(gno, setno) != TRUE) {
        return;
    }
    g[gno].p[setno].data.ex[col] = x;
    g[gno].p[setno].data.len = len;
    set_dirtystate();
}

char **get_set_strings(int gno, int setno)
{
    if (is_valid_setno(gno, setno)) {
        return g[gno].p[setno].data.s;
    } else {
        return NULL;
    }
}

char **get_set_original_strings(int gno, int setno)
{
    if (is_valid_setno(gno, setno)) {
        return g[gno].p[setno].data.orig_s;
    } else {
        return NULL;
    }
}

int set_set_strings(int gno, int setno, int len, char **s)
{
QString tmp;
char c_tmp[GR_MAXPATHLEN];
    if (is_valid_setno(gno, setno) && len > 0 && s!= NULL) {
        g[gno].p[setno].data.s = s;
        g[gno].p[setno].data.len = len;
        g[gno].p[setno].data.orig_s = (char **)xmalloc(len*sizeof(char *));
        for (int i=0;i<len;i++)
        {
        tmp=QString::fromLocal8Bit(g[gno].p[setno].data.s[i]);
        g[gno].p[setno].data.orig_s[i]=copy_string(NULL,tmp.toUtf8().constData());
        complete_LaTeX_to_Grace_Translator(tmp);
        generate_string_Qt_aware(c_tmp,tmp);
        g[gno].p[setno].data.s[i]=copy_string(g[gno].p[setno].data.s[i],c_tmp);
        }
        set_dirtystate();
        return RETURN_SUCCESS;
    } else {
        return RETURN_FAILURE;
    }
}

int init_set_string(int gno, int setno)
{
    int len;
    if (is_valid_setno(gno, setno) && (len=getsetlength(gno,setno))) {
        if (g[gno].p[setno].data.s==NULL)
        {
        //qDebug() << "Create Memory len=" << len;
        g[gno].p[setno].data.s=(char**)xmalloc(sizeof(char*)*len);
        g[gno].p[setno].data.orig_s=(char**)xmalloc(sizeof(char*)*len);
            for (int i=0;i<len;i++)
            {
            g[gno].p[setno].data.s[i] = NULL;
            g[gno].p[setno].data.orig_s[i] = NULL;
            }
        }
            for (int i=0;i<len;i++)
            {
            g[gno].p[setno].data.s[i] = copy_string(g[gno].p[setno].data.s[i],"");
            g[gno].p[setno].data.orig_s[i] = copy_string(g[gno].p[setno].data.orig_s[i],"");
            }
        set_dirtystate();
        return RETURN_SUCCESS;
    } else {
        return RETURN_FAILURE;
    }
}

int getsetlength(int gno, int setno)
{
    if (is_valid_setno(gno, setno)) {
        return g[gno].p[setno].data.len;
    } else {
        return -1;
    }
}

int setcomment(int gno, int setno,const char *s)
{
QString n_str=QString::fromLocal8Bit(s);
    if (is_valid_setno(gno, setno) && s != NULL)
    {
        if (g[gno].p[setno].comments != s)
        {
        strncpy(g[gno].p[setno].comments, n_str.toLocal8Bit().constData(), MAX_STRING_LENGTH - 1);
        g[gno].p[setno].comments[MAX_STRING_LENGTH - 1]='\0';
        }
        if (g[gno].p[setno].orig_comments != s)
        {
        strncpy(g[gno].p[setno].orig_comments, n_str.toUtf8().constData(), MAX_STRING_LENGTH - 1);
        g[gno].p[setno].orig_comments[MAX_STRING_LENGTH - 1]='\0';
        }
        set_dirtystate();
        return RETURN_SUCCESS;
    }
    else
    {
        return RETURN_FAILURE;
    }
}

char *getcomment(int gno, int setno)
{ 
    if (is_valid_setno(gno, setno)) {
        return g[gno].p[setno].comments;
    } else {
        return NULL;
    }
}

int set_legend_string(int gno, int setno,const char *s)
{ 
QString n_str=QString::fromLocal8Bit(s);
    if (is_valid_setno(gno, setno) && s != NULL)
    {
        if (g[gno].p[setno].lstr!=s)
        {
        strncpy(g[gno].p[setno].lstr, n_str.toLocal8Bit(), MAX_STRING_LENGTH - 1);
        g[gno].p[setno].lstr[MAX_STRING_LENGTH - 1]='\0';
        }
        if (g[gno].p[setno].orig_lstr!=s)
        {
        strncpy(g[gno].p[setno].orig_lstr, n_str.toUtf8(), MAX_STRING_LENGTH - 1);
        g[gno].p[setno].orig_lstr[MAX_STRING_LENGTH - 1]='\0';
        }
    return RETURN_SUCCESS;
    }
    else
    {
    return RETURN_FAILURE;
    }
}

char *get_legend_string(int gno, int setno)
{ 
    if (is_valid_setno(gno, setno)) {
        return g[gno].p[setno].lstr;
    } else {
        return NULL;
    }
}

int set_dataset_type(int gno, int setno, int type)
{ 
    int old_type = dataset_type(gno, setno);
    if (old_type < 0) {
        /* wrong gno/setno */
        return RETURN_FAILURE;
    } else if (old_type == type) {
        /* nothing changed */
        return RETURN_SUCCESS;
    } else {
        int i, len, ncols_old, ncols_new;
        len = getsetlength(gno, setno);
        ncols_old = dataset_cols(gno, setno);
        ncols_new = settype_cols(type);
        for (i = ncols_old; i < ncols_new; i++) {
            g[gno].p[setno].data.ex[i] = (double *)xcalloc(len, sizeof(double));///SIZEOF_DOUBLE);
        }
        for (i = ncols_new; i < ncols_old; i++) {
            XCFREE(g[gno].p[setno].data.ex[i]);
        }
        g[gno].p[setno].type = type;
        set_dirtystate();
        return RETURN_SUCCESS;
    }
}

int dataset_type(int gno, int setno)
{ 
    if (is_valid_setno(gno, setno)) {
        return g[gno].p[setno].type;
    } else {
        return -1;
    }
}

void set_hotlink(int gno, int setno, int onoroff, char *fname, int src)
{
    if (is_valid_setno(gno, setno) != TRUE) {
        return;
    }
    
    g[gno].p[setno].hotlink = onoroff;
    if (onoroff && fname != NULL) {
	strcpy(g[gno].p[setno].hotfile, fname);
	g[gno].p[setno].hotsrc = src;
    }
    set_dirtystate();
}

int is_hotlinked(int gno, int setno)
{
    if (is_valid_setno(gno, setno) != TRUE) {
        return FALSE;
    }
    
    if (g[gno].p[setno].hotlink && strlen(g[gno].p[setno].hotfile)) {
        return g[gno].p[setno].hotlink;
    } else { 
        return FALSE;
    }
}

char *get_hotlink_file(int gno, int setno)
{
    if (is_valid_setno(gno, setno) != TRUE) {
        return NULL;
    } else {
        return g[gno].p[setno].hotfile;
    }
}

int get_hotlink_src(int gno, int setno)
{
    if (is_valid_setno(gno, setno) != TRUE) {
        return -1;
    } else {
        return g[gno].p[setno].hotsrc;
    }
}

void do_update_hotlink(int gno, int setno)
{
    if (is_hotlinked(gno, setno) != TRUE) {
        return;
    } else {
        plotarr *p;
        p = &g[gno].p[setno];
        update_set_from_file(gno, setno, p->hotfile, p->hotsrc);
    }
}

/*
 * get the min/max values including error-bars; xmin and xmax are assumed to already have min/max of x and dx1=downward error, dx2=upward error
 */
int getminmaxwitherrorbars(int len,double *x,double *dx1,double *dx2,double *xmin, double *xmax)
{
double tmp;
    /*for (int i=0;i<len;i++)
    {
    tmp=x[i]-dx1[i];
    if (tmp<*xmin) *xmin=tmp;
    tmp=x[i]+dx2[i];
    if (tmp>*xmax) *xmax=tmp;
    }*/
/*
    for (int i=0;i<len;i++)
    {
    tmp=x[i]+fabs(dx1[i]);
    //tmp=x[i]+dx1[i];
    if (tmp>*xmax) *xmax=tmp;
    tmp=x[i]-fabs(dx2[i]);
    //tmp=x[i]-dx2[i];
    if (tmp<*xmin) *xmin=tmp;
    }
*/
if (x!=NULL && dx1!=NULL)
    for (int i=0;i<len;i++)
    {
    tmp=x[i]+dx1[i];
    if (tmp>*xmax) *xmax=tmp;
    if (tmp<*xmin) *xmin=tmp;
    }
if (x!=NULL && dx2!=NULL)
    for (int i=0;i<len;i++)
    {
    tmp=x[i]-dx2[i];
    if (tmp>*xmax) *xmax=tmp;
    if (tmp<*xmin) *xmin=tmp;
    }
return RETURN_SUCCESS;
}

/*
 * get the min/max fields of a set
 */
int getsetminmax(int gno, int setno,double *xmin, double *xmax, double *ymin, double *ymax, int include_errorbares=0)
{
    double *x, *y, *dy2;
    int len;
    double x1, x2, y1, y2;
    int i, first = TRUE;
    int imin, imax; /* dummy */
    int nr_of_err,err_cols[2];

    if (setno == ALL_SETS) {
        for (i = 0; i < number_of_sets(gno); i++) {
            if (is_set_drawable(gno, i)) {
                if (g[gno].p[i].ignore_in_autoscale==TRUE && autoscale_active==1) continue;
                x = getcol(gno, i, 0);
                y = getcol(gno, i, 1);
                len = getsetlength(gno, i);
                minmax(x, len, &x1, &x2, &imin, &imax);
                minmax(y, len, &y1, &y2, &imin, &imax);
                if (first) {
                    *xmin = x1;
                    *xmax = x2;
                    *ymin = y1;
                    *ymax = y2;
                    first = FALSE;
                } else {
                    *xmin = (x1 < *xmin) ? x1 : *xmin;
                    *xmax = (x2 > *xmax) ? x2 : *xmax;
                    *ymin = (y1 < *ymin) ? y1 : *ymin;
                    *ymax = (y2 > *ymax) ? y2 : *ymax;
                }
                if (include_errorbares && (nr_of_err=errorbar_cols(gno,i,0,err_cols)))//x-error-bars
                {
                y = getcol(gno, i, err_cols[0]);
                    if (nr_of_err>1) dy2 = getcol(gno, i, err_cols[1]);
                    else dy2 = y;
                getminmaxwitherrorbars(len,x,y,dy2,xmin,xmax);
                }
                if (include_errorbares && (nr_of_err=errorbar_cols(gno,i,1,err_cols)))//y-error-bars
                {
                y = getcol(gno, i, 1);
                x = getcol(gno, i, err_cols[0]);
                    if (nr_of_err>1) dy2 = getcol(gno, i, err_cols[1]);
                    else dy2 = x;
                getminmaxwitherrorbars(len,y,x,dy2,ymin,ymax);
                }
            }
        }
    } else if (is_valid_setno(gno, setno)) {
        if (g[gno].p[setno].ignore_in_autoscale==FALSE || autoscale_active==0)
        {
        x = getcol(gno, setno, 0);
        y = getcol(gno, setno, 1);
        len = getsetlength(gno, setno);
        minmax(x, len, xmin, xmax, &imin, &imax);
        minmax(y, len, ymin, ymax, &imin, &imax);
        first = FALSE;
            if (include_errorbares && (nr_of_err=errorbar_cols(gno,setno,0,err_cols)))//x-error-bars
            {
            y = getcol(gno, setno, err_cols[0]);
                if (nr_of_err>1) dy2 = getcol(gno, setno, err_cols[1]);
                else dy2 = y;
            getminmaxwitherrorbars(len,x,y,dy2,xmin,xmax);
            }
            if (include_errorbares && (nr_of_err=errorbar_cols(gno,setno,1,err_cols)))//y-error-bars
            {
            y = getcol(gno, setno, 1);
            x = getcol(gno, setno, err_cols[0]);
                if (nr_of_err>1) dy2 = getcol(gno, setno, err_cols[1]);
                else dy2 = x;
            getminmaxwitherrorbars(len,y,x,dy2,ymin,ymax);
            }
        }
    }
    
    if (first == FALSE) {
        return RETURN_SUCCESS;
    } else {
        return RETURN_FAILURE;
    }
}

/*
 * get the min/max fields of a set with fixed x/y range
 */
int getsetminmax_c(int gno, int setno, double *xmin, double *xmax, double *ymin, double *ymax, int ivec)
{
    double vmin_t, vmax_t, *vmin, *vmax, bvmin, bvmax, *vec, *bvec;
    int i, start, stop, n;
    int first = TRUE, hits;
    vmin_t=vmax_t=0.0;

    if (ivec == 1) {    
        bvmin = *xmin;
        bvmax = *xmax;
        vmin  = ymin; 
        vmax  = ymax; 
    } else {
        bvmin = *ymin;
        bvmax = *ymax;
        vmin  = xmin;
        vmax  = xmax;
    }
    if (setno == ALL_SETS) {
        start = 0;
        stop  = number_of_sets(gno) - 1;
    } else if (is_valid_setno(gno, setno)) {
        start = setno;
        stop  = setno;
    } else {
        return RETURN_FAILURE;
    }
    
    for (i = start; i <= stop; i++) {
        if (is_set_drawable(gno, i)) {
            if (g[gno].p[i].ignore_in_autoscale==TRUE && autoscale_active==1) continue;
            if (ivec == 1) {
                bvec = getx(gno, i);
                vec  = gety(gno, i);
            } else {
                bvec = gety(gno, i);
                vec  = getx(gno, i);
            }
            
            n = getsetlength(gno, i);
            hits = minmaxrange(bvec, vec, n, bvmin, bvmax, &vmin_t, &vmax_t);
            if (hits == RETURN_SUCCESS) {
                if (first) {
                    *vmin = vmin_t;
                    *vmax = vmax_t;
                    first = FALSE;
                } else {
                    *vmin = MIN2(vmin_t, *vmin);
                    *vmax = MAX2(vmax_t, *vmax);
                }
            }
        }
    }
    
    if (first == FALSE) {
        return RETURN_SUCCESS;
    } else {
        return RETURN_FAILURE;
    }
}

/* same as above, but including error-bars*/
int getsetminmax_c2(int gno, int setno, double *xmin, double *xmax, double *ymin, double *ymax, int ivec)
{
    double vmin_t, vmax_t, *vmin, *vmax, bvmin, bvmax, *vec, *bvec;
    double *error_bar1,*error_bar2;
    int i, start, stop, n;
    int first = TRUE, hits;
    int nr_of_err,err_cols[2];
    err_cols[0]=err_cols[1]=0;
    vmin_t=vmax_t=0.0;

    if (ivec == 1) {
        bvmin = *xmin;
        bvmax = *xmax;
        vmin  = ymin;
        vmax  = ymax;
    } else {
        bvmin = *ymin;
        bvmax = *ymax;
        vmin  = xmin;
        vmax  = xmax;
    }
    if (setno == ALL_SETS) {
        start = 0;
        stop  = number_of_sets(gno) - 1;
    } else if (is_valid_setno(gno, setno)) {
        start = setno;
        stop  = setno;
    } else {
        return RETURN_FAILURE;
    }
    for (i = start; i <= stop; i++) {
        if (is_set_drawable(gno, i)) {
            if (g[gno].p[i].ignore_in_autoscale==TRUE && autoscale_active==1) continue;
            if (ivec == 1) {
                bvec = getx(gno, i);
                vec  = gety(gno, i);
                nr_of_err=errorbar_cols(gno,i,1,err_cols);
            } else {
                bvec = gety(gno, i);
                vec  = getx(gno, i);
                nr_of_err=errorbar_cols(gno,i,0,err_cols);
            }
//qDebug() << "G[" << gno << "].S[" << i << "] nr_of_err=" << nr_of_err << " ivec=" << (ivec==2?"X":"Y") << " cols[0]=" << err_cols[0] << " cols[1]=" << err_cols[1];
            n = getsetlength(gno, i);
            if (nr_of_err<=0)//no errorbars-->use the simpler function instead
            {
            hits = minmaxrange(bvec, vec, n, bvmin, bvmax, &vmin_t, &vmax_t);
            }
            else if (nr_of_err==1)
            {
            error_bar1=error_bar2=getcol(gno,i,err_cols[0]);
            hits = minmaxrange2(bvec, vec, error_bar1, error_bar2, n, bvmin, bvmax, &vmin_t, &vmax_t);
            }
            else
            {
            error_bar1=getcol(gno,i,err_cols[0]);
            error_bar2=getcol(gno,i,err_cols[1]);
            hits = minmaxrange2(bvec, vec, error_bar1, error_bar2, n, bvmin, bvmax, &vmin_t, &vmax_t);
            }
            if (hits == RETURN_SUCCESS) {
                if (first) {
                    *vmin = vmin_t;
                    *vmax = vmax_t;
                    first = FALSE;
                } else {
                    *vmin = MIN2(vmin_t, *vmin);
                    *vmax = MAX2(vmax_t, *vmax);
                }
            }
        }
    }
    if (first == FALSE) {
        return RETURN_SUCCESS;
    } else {
        return RETURN_FAILURE;
    }
}

/* linear regression without posting results to log */
int mute_linear_regression(int n, double *x, double *y, double *slope, double *intercept)
{
    double xbar, ybar;		/* sample means */
    double sdx, sdy;		/* sample standard deviations */
    double SXX, SYY, SXY;	/* sums of squares */
    int i;

    if (n <= 3) {
        return 1;
    }
    xbar = ybar = 0.0;
    SXX = SYY = SXY = 0.0;
    for (i = 0; i < n; i++) {
        xbar = xbar + x[i];
        ybar = ybar + y[i];
    }
    xbar = xbar / n;
    ybar = ybar / n;
    for (i = 0; i < n; i++) {
        SXX = SXX + (x[i] - xbar) * (x[i] - xbar);
        SYY = SYY + (y[i] - ybar) * (y[i] - ybar);
        SXY = SXY + (x[i] - xbar) * (y[i] - ybar);
    }
    sdx = sqrt(SXX / (n - 1));
    sdy = sqrt(SYY / (n - 1));
    if (sdx == 0.0) {
        return 2;
    }
    if (sdy == 0.0) {
        return 2;
    }
    *slope = SXY / SXX;
    *intercept = ybar - *slope * xbar;
    return 0;
}

/*
 * assume graph starts off at ymin and rises to ymax
 * Determine time to go from 10% to 90% of rise
 */
int get_rise_time( int setl, double *xv, double *yv, double min, double max,double *width )
{
    int x10=0, x90;
    double amp10, amp90;

    amp10 = min + (max-min)*0.1;
    amp90 = min + (max-min)*0.9;
    while( x10<setl && yv[x10]<amp10  )
        x10++;

    if( x10==setl || x10==0)
        return 1;

    x90 = x10+1;

    while( x90<setl && yv[x90]<amp90 )
        x90++;

    *width = linear_interp( yv[x90-1], xv[x90-1], yv[x90], xv[x90], amp90 ) -
            linear_interp( yv[x10-1], xv[x10-1], yv[x10], xv[x10], amp10 );
    return 0;
}

/* assume graph starts off at ymax and drops to ymin
   Determine time to go from 90% to 10% of drop			*/
int get_fall_time( int setl, double *xv, double *yv, double min, double max,double *width )
{
    int x10, x90=0;
    double amp10, amp90;

    amp10 = min + (max-min)*0.1;
    amp90 = min + (max-min)*0.9;
    while( x90<setl && yv[x90]>amp90 )
        x90++;

    if( x90==setl || x90==0)
        return 1;

    x10= x90+1;

    while( x10<setl && yv[x10]>amp10  )
        x10++;

    if( x10==setl )
        return 1;

    *width = linear_interp( yv[x10-1], xv[x10-1], yv[x10], xv[x10], amp10 )-
            linear_interp( yv[x90-1], xv[x90-1], yv[x90], xv[x90], amp90 );
    return 0;
}

int get_zero_crossing( int setl, double *xv, double *yv, double *crossing )
{
    int i=0;

    while( i<setl-1 && yv[i] != 0. && yv[i]*yv[i+1]>0. )
        i++;

    if( i==setl-1 )
        return 1;

    if( yv[i] == 0 )
        *crossing = xv[i];
    else
        *crossing = linear_interp( yv[i], xv[i], yv[i+1], xv[i+1], 0 );

    return 0;
}

/* Get FWHM of the highest peak */
int get_half_max_width(int len, double *x, double *y, double *width)
{
    int i, imin, imax;
    double ymin, ymax, yavg;
    double x_u, x_d;

    minmax(y, len, &ymin, &ymax, &imin, &imax);
    yavg = (ymin + ymax)/2.0;

    i = imax;
    while (i >= 0 && y[i] > yavg) {
        i--;
    }
    if (i < 0) {
        return RETURN_FAILURE;
    }
    x_d = linear_interp(y[i], x[i], y[i + 1], x[i + 1], yavg);

    i = imax;
    while (i < len && y[i] > yavg) {
        i++;
    }
    if (i == len) {
        return RETURN_FAILURE;
    }
    x_u = linear_interp(y[i - 1], x[i - 1], y[i], x[i], yavg);

    *width = fabs(x_u - x_d);

    return RETURN_SUCCESS;
}

int dbl_comp( const void *a, const void *b )
{
    if( *(double *)a < *(double *)b )
        return -1;
    else if( *(double *)a > *(double *)b )
        return 1;
    else
        return 0;
}

/* get the median of the X or Y portion of a set */
int getmedian( int grno, int setno, int sorton, double *median )
{
    int setlen;
    double *setdata;

    setlen = getsetlength( get_cg(), setno );
    setdata = (double *)xmalloc( setlen*sizeof(double) );
    /*if( sorton == DATA_X )
        memcpy( setdata, getx( grno, setno ), setlen*sizeof(double) );
    else
        memcpy( setdata, gety( grno, setno ), setlen*sizeof(double) );*/
    if (sorton<DATA_X || sorton>DATA_Y4)
        memcpy( setdata, getx( grno, setno ), setlen*sizeof(double) );
    else
        memcpy( setdata, getcol(grno, setno, sorton), setlen*sizeof(double) );

    qsort(
                setdata,
                setlen,
                sizeof(double),
                dbl_comp );

    if( setlen%2 )		/* odd set */
        *median = setdata[(setlen+1)/2-1];
    else
        *median = ( setdata[setlen/2-1] + setdata[setlen/2] )/2.;

    xfree( setdata );
    return 0;
}

int get_barycenter( int n, double *x, double *y, double *barycenter )
{
    double wsum=0;

    *barycenter = 0;
    for( n--; n>=0; n-- ) {
        wsum += x[n]*y[n];
        *barycenter += x[n];
    }
    *barycenter = wsum/(*barycenter);
    return 0;
}

/* given maximum of set a, find the corresponding entry in set b */
void get_max_pos( double *a, double *b, int n, double max, double *d )
{
    int i=-1;

    while( ++i<n && a[i] != max  ){;}

    if( i==n )
        return;
    else
        *d = b[i];
}

/* linear interpolate between two points, return a y value given a x */
double linear_interp( double x1, double y1, double x2, double y2, double x )
{
    if (fabs(x2-x1)<10.0*DBL_EPSILON)
    return y1;
    else
    return y1 + ( x-x1 )*(y2-y1)/(x2-x1);
}

/* linear interpolation: returns a and b in y=a*x+b for the line that goes through x1 and x2
 * if x1=x2 then infi=1 (slope is infinite) otherwise infi=0
 */
void linear_interp2(double x1,double y1,double x2,double y2,double * a,double * b,int * infi)
{
    if (fabs(x1-x2)<10.0*DBL_EPSILON) {*b=x1;*a=0.0;*infi=1;return;}
    *infi=0;
    *a=(y2-y1)/(x2-x1);
    *b=y1-(*a)*x1;
}

/*
 * checks if x/y is inside the area made of x1/y1 and x2/y2 (being on the border counts as inside)
 * 1=inside, 0=outside
 */
inline int simple_check_inside(double x, double y,double x1,double y1,double x2,double y2)
{
bool x1eqx2=tolerant_eq(x1,x2);
bool y1eqy2=tolerant_eq(y1,y2);
bool in_x_range=true;
bool in_y_range=true;
if (x1eqx2)//same x1 and x2 --> x has to be roughly the same as x1 OR x2 to be inside
{
if (tolerant_eq(x1,x)==0 && tolerant_eq(x2,x)==0) in_x_range=false;
//qDebug() << "eqx=true";
}
else//different x1 and x2
{
if ((tolerant_lt(x,x1) && tolerant_lt(x,x2)) || (tolerant_gt(x,x1) && tolerant_gt(x,x2))) in_x_range=false;
}
if (y1eqy2)
{
if (tolerant_eq(y1,y)==0 && tolerant_eq(y2,y)==0) in_y_range=false;
//qDebug() << "eqy=true, eq1=" << tolerant_eq(y1,y) << ", eq2=" << tolerant_eq(y2,y);
}
else
{
if ((tolerant_lt(y,y1) && tolerant_lt(y,y2)) || (tolerant_gt(y,y1) && tolerant_gt(y,y2))) in_y_range=false;
}
//const double d_eps=10.0*DBL_EPSILON;
//if ((x<x1 && x<x2) || (x>x1 && x>x2) || (y>y1 && y>y2) || (y<y1 && y<y2)) return 0;
//if ((x1-x>d_eps && x2-x>d_eps) || (x-x1>d_eps && x-x2>d_eps) || (y-y1>d_eps && y-y2>d_eps) || (y1-y>d_eps && y2-y>d_eps)) return 0;
//if ((tolerant_lt(x,x1) && tolerant_lt(x,x2)) || (tolerant_gt(x,x1) && tolerant_gt(x,x2)) || (tolerant_gt(y,y1) && tolerant_gt(y,y2)) || (tolerant_lt(y,y1) && tolerant_lt(y,y2))) return 0;
//qDebug() << "in_x_range=" << in_x_range << "in_y_range=" << in_y_range;
if (in_x_range==false || in_y_range==false) return 0;
else return 1;
}

int get_value_crossing( int gno, int setno, double value, double *crossing )//returns in crossing the x-value for which the set first crosses the y-value
{
    int i=0,setl=getsetlength(gno,setno);
    double * yv=gety(gno,setno),* xv=getx(gno,setno);
    if (yv==NULL || xv==NULL || setl<0) return RETURN_FAILURE;

    while( i<setl-1 && (yv[i]-value) != 0. && (yv[i]-value)*(yv[i+1]-value)>0. )
        i++;

    if( i==setl-1 )
        return RETURN_FAILURE;

    if( yv[i]-value == 0. )
        *crossing = xv[i];
    else
        *crossing = linear_interp( yv[i], xv[i], yv[i+1], xv[i+1], value );

    return RETURN_SUCCESS;
}

int get_value_crossing2( int gno, int setno, double value, double *crossing )//returns in crossing the y-value for which the set first crosses the x-value
{
    int i=0,setl=getsetlength(gno,setno);
    double * yv=getx(gno,setno),* xv=gety(gno,setno);
    if (yv==NULL || xv==NULL || setl<0) return RETURN_FAILURE;

    while( i<setl-1 && (yv[i]-value) != 0. && (yv[i]-value)*(yv[i+1]-value)>0. )
        i++;

    if( i==setl-1 )
        return RETURN_FAILURE;

    if( yv[i]-value == 0. )
        *crossing = xv[i];
    else
        *crossing = linear_interp( yv[i], xv[i], yv[i+1], xv[i+1], value );

    return RETURN_SUCCESS;
}

//we do not extrapolate! only visible intersections inside set-restrictions (real region-restrictions have to be applied later)
int get_all_intersection_points_between_two_sets(int gno1,int setno1,int gno2,int setno2,double ** x,double ** y,int * n)
{
if (!is_valid_setno(gno1,setno1) || !is_valid_setno(gno2,setno2)) return RETURN_FAILURE;
double x1min,x2min;
double x1max,x2max;
double y1min,y2min;
double y1max,y2max;
double gxmin,gxmax;//,gymin,gymax;
int setlen_max=g[gno1].p[setno1].data.len>g[gno2].p[setno2].data.len?g[gno1].p[setno1].data.len:g[gno2].p[setno2].data.len;
x1min=x1max=g[gno1].p[setno1].data.ex[0][0];
y1min=y1max=g[gno1].p[setno1].data.ex[1][0];
x2min=x2max=g[gno2].p[setno2].data.ex[0][0];
y2min=y2max=g[gno2].p[setno2].data.ex[1][0];
for (int i=1;i<g[gno1].p[setno1].data.len;i++)
{
    if (g[gno1].p[setno1].data.ex[0][i]<x1min) x1min=g[gno1].p[setno1].data.ex[0][i];
    else if (g[gno1].p[setno1].data.ex[0][i]>x1max) x1max=g[gno1].p[setno1].data.ex[0][i];
    if (g[gno1].p[setno1].data.ex[1][i]<y1min) y1min=g[gno1].p[setno1].data.ex[1][i];
    else if (g[gno1].p[setno1].data.ex[1][i]>y1max) y1max=g[gno1].p[setno1].data.ex[1][i];
}
for (int i=1;i<g[gno2].p[setno2].data.len;i++)
{
    if (g[gno2].p[setno2].data.ex[0][i]<x2min) x2min=g[gno2].p[setno2].data.ex[0][i];
    else if (g[gno2].p[setno2].data.ex[0][i]>x2max) x2max=g[gno2].p[setno2].data.ex[0][i];
    if (g[gno2].p[setno2].data.ex[1][i]<y2min) y2min=g[gno2].p[setno2].data.ex[1][i];
    else if (g[gno2].p[setno2].data.ex[1][i]>y2max) y2max=g[gno2].p[setno2].data.ex[1][i];
}
gxmin=x1min>x2min?x1min:x2min;
gxmax=x1max<x2max?x1max:x2max;
/*gymin=y1min>y2min?y1min:y2min;
gymax=y1max<y2max?y1max:y2max;*/
double * xnew=new double[setlen_max];
double * xnew2=new double[setlen_max];
double dx=(gxmax-gxmin)/(setlen_max-1);
for (int i=0;i<setlen_max;i++)
{
xnew[i]=dx*i+gxmin;
}
double * ynew1=new double[setlen_max];
double * ynew2=new double[setlen_max];
double * ynew3=new double[setlen_max];
interpolate(xnew,ynew1,setlen_max,g[gno1].p[setno1].data.ex[0],g[gno1].p[setno1].data.ex[1],g[gno1].p[setno1].data.len,0);
interpolate(xnew,ynew2,setlen_max,g[gno2].p[setno2].data.ex[0],g[gno2].p[setno2].data.ex[1],g[gno2].p[setno2].data.len,0);
for (int i=0;i<setlen_max;i++) ynew3[i]=ynew1[i]-ynew2[i];
int intersection_counter=0;
for (int i=1;i<setlen_max;i++)
{
    if (!(ynew3[i-1] != 0.0 && ynew3[i-1]*ynew3[i]>0.0))
    {
        if (ynew3[i-1] == 0.0)
        {
        xnew2[intersection_counter]=xnew[i-1];
        ynew2[intersection_counter++]=ynew1[i-1];
        }
        else
        {
        xnew2[intersection_counter]=linear_interp( ynew3[i-1], xnew[i-1], ynew3[i], xnew[i], 0.0 );
        ynew2[intersection_counter]=linear_interp( xnew[i-1], ynew1[i-1], xnew[i], ynew1[i], xnew2[intersection_counter]);
        intersection_counter++;
        }
    }
}
*n=intersection_counter;
if (*x!=NULL) delete[] *x;
if (*y!=NULL) delete[] *y;
*x=new double[2+intersection_counter];
*y=new double[2+intersection_counter];
if (intersection_counter>0)
{
//cout << "intersections_found=" << intersection_counter << endl;
//cout << "gxmin=" << gxmin << " gxmax=" << gxmax << endl;
for (int i=0;i<intersection_counter;i++)
{
//cout << i << ": (" << xnew2[i] << "|" << ynew2[i] << ")" << endl;
(*x)[i]=xnew2[i];
(*y)[i]=ynew2[i];
}
delete[] xnew;
delete[] xnew2;
delete[] ynew1;
delete[] ynew2;
delete[] ynew3;
return RETURN_SUCCESS;
}
else
{
delete[] xnew;
delete[] xnew2;
delete[] ynew1;
delete[] ynew2;
delete[] ynew3;
return RETURN_FAILURE;
}
}

//find the intersection angles between gno1.setno1 and gno2.setno2 at the positions x (it is not checked whether there is an actual intersection at the specified points)
int get_intersection_angles(int gno1,int setno1,int gno2,int setno2,double * x,double * angles,int n)
{
if (n<1) return RETURN_FAILURE;
double * m1=new double[n+2];
double * m2=new double[n+2];
double * mx=new double[n+2];
double * mt=new double[n+2];
if (getsetlength(gno1,setno1)==2)//a simple line
{
    if ((g[gno1].p[setno1].data.ex[0][1]-g[gno1].p[setno1].data.ex[0][0])==0.0)
    {
    m1[0]=1e15;//not correct, but I can not put infinity here
    if ((g[gno1].p[setno1].data.ex[1][1]-g[gno1].p[setno1].data.ex[1][0])==0.0) m1[0]=0.0;
    }
    else
    {
    m1[0]=(g[gno1].p[setno1].data.ex[1][1]-g[gno1].p[setno1].data.ex[1][0])/(g[gno1].p[setno1].data.ex[0][1]-g[gno1].p[setno1].data.ex[0][0]);
    }
    for (int i=1;i<n;i++) m1[i]=m1[0];
}
else
{
    delete[] mx;
    delete[] mt;
    mx=new double[2+getsetlength(gno1,setno1)];
    mt=new double[2+getsetlength(gno1,setno1)];
forwarddiff(getcol(gno1,setno1,0),getcol(gno1,setno1,1),mx,mt,getsetlength(gno1,setno1));
interpolate(x,m1,n,mx,mt,getsetlength(gno1,setno1)-1,0);
}
if (getsetlength(gno2,setno2)==2)//a simple line
{
    if ((g[gno2].p[setno2].data.ex[0][1]-g[gno2].p[setno2].data.ex[0][0])==0.0)
    {
    m2[0]=1e15;//not correct, but I can not put infinity here
    if ((g[gno2].p[setno2].data.ex[1][1]-g[gno2].p[setno2].data.ex[1][0])==0.0) m2[0]=0.0;
    }
    else
    {
    m2[0]=(g[gno2].p[setno2].data.ex[1][1]-g[gno2].p[setno2].data.ex[1][0])/(g[gno2].p[setno2].data.ex[0][1]-g[gno2].p[setno2].data.ex[0][0]);
    }
    for (int i=1;i<n;i++) m2[i]=m2[0];
}
else
{
    delete[] mx;
    delete[] mt;
    mx=new double[2+getsetlength(gno2,setno2)];
    mt=new double[2+getsetlength(gno2,setno2)];
forwarddiff(getcol(gno2,setno2,0),getcol(gno2,setno2,1),mx,mt,getsetlength(gno2,setno2));
interpolate(x,m2,n,mx,mt,getsetlength(gno2,setno2)-1,0);
}
    for (int i=0;i<n;i++)
    {
    angles[i]=acos((1.0+m1[i]*m2[i])/sqrt(1.0+m1[i]*m1[i])/sqrt(1.0+m2[i]*m2[i]));
    }
delete[] m1;
delete[] m2;
delete[] mx;
delete[] mt;
return RETURN_SUCCESS;
}

/* get intersection points and angles of two sets, no monotonocity needed
 * if there is an overlapping area between the two sets, only the first point in this area will be returned as intersection point (because there would be an infinite number of them)
 * x/y=insersection points, a=intersection angles, n=number of intersection points
 * return SUCCESS if at least one intersection point is found
 */
int get_all_intersection_points_and_all_angles_between_two_sets(int gno1, int setno1, int gno2, int setno2, double **x, double **y,double ** a, int *n)
{
int len1=getsetlength(gno1,setno1);
int len2=getsetlength(gno2,setno2);
int nr_of_intersections=0;
if (len1<1 || len2<1) {*n=0;return RETURN_FAILURE;}//at least one set has no points in it or does not exist
double * lv1[2];
double * lv2[2];
int * infi[2];
double * inters_x,*inters_y,*inters_a;
lv1[0]=new double[len1>1?len1:2];
lv1[1]=new double[len1>1?len1:2];
infi[0]=new int[len1>1?len1:2];
lv2[0]=new double[len2>1?len2:2];
lv2[1]=new double[len2>1?len2:2];
infi[1]=new int[len2>1?len2:2];
inters_x=new double[len2>len1?len2:len1];
inters_y=new double[len2>len1?len2:len1];
inters_a=new double[len2>len1?len2:len1];
//first: we calculate the linear equations for every two consecutive points in both sets
for (int i=1;i<len1;i++)
{
linear_interp2(g[gno1].p[setno1].data.ex[0][i-1],g[gno1].p[setno1].data.ex[1][i-1],g[gno1].p[setno1].data.ex[0][i],g[gno1].p[setno1].data.ex[1][i],lv1[0]+i-1,lv1[1]+i-1,infi[0]+i-1);
}
for (int i=1;i<len2;i++)
{
linear_interp2(g[gno2].p[setno2].data.ex[0][i-1],g[gno2].p[setno2].data.ex[1][i-1],g[gno2].p[setno2].data.ex[0][i],g[gno2].p[setno2].data.ex[1][i],lv2[0]+i-1,lv2[1]+i-1,infi[1]+i-1);
}
double pos_x,pos_y;
double hyp1,hyp2;
double a1,a2,b1,b2;
int pos_res,ret;

//double t_y,t_y1,t_y2,d_eps=10.0*DBL_EPSILON;

for (int i=0;i<len1-1;i++)
{
    for (int j=0;j<len2-1;j++)
    {
    pos_res=get_simple_line_crossing(lv1[0][i],lv1[1][i],infi[0][i],lv2[0][j],lv2[1][j],infi[1][j],&pos_x,&pos_y);
//qDebug() << "i=" << i << "j=" << j << "pos_res=" << pos_res << "pos_x/posy=" << pos_x << "/" << pos_y;
        switch (pos_res)
        {
        case 0://just a single intersection point --> look if it is actually between the two points of line1 AND line2
        ret=simple_check_inside(pos_x,pos_y,g[gno1].p[setno1].data.ex[0][i],g[gno1].p[setno1].data.ex[1][i],g[gno1].p[setno1].data.ex[0][i+1],g[gno1].p[setno1].data.ex[1][i+1]);
//qDebug() << "ret=" << ret;
            if (ret==1)//inside one of the two limiting points
            {
            ret=simple_check_inside(pos_x,pos_y,g[gno2].p[setno2].data.ex[0][j],g[gno2].p[setno2].data.ex[1][j],g[gno2].p[setno2].data.ex[0][j+1],g[gno2].p[setno2].data.ex[1][j+1]);
/*qDebug() << "ret=" << ret;
    t_y=pos_y;
    t_y1=g[gno2].p[setno2].data.ex[1][j];
    t_y2=g[gno2].p[setno2].data.ex[1][j+1];*/
    //(y-y1>d_eps && y-y2>d_eps) || (y1-y>d_eps && y2-y>d_eps)
  /*qDebug() << "y-y1=" << t_y-t_y1 << "y-y2=" << t_y-t_y2 << "y1-y=" << t_y1-t_y << "y2-y=" << t_y2-t_y;
    qDebug() << "default:   y-y1=" << int(t_y-t_y1>d_eps) << "y-y2=" << int(t_y-t_y2>d_eps) << "y1-y=" << int(t_y1-t_y>d_eps) << "y2-y=" << int(t_y2-t_y>d_eps);
    qDebug() << "incl.tol.: y-y1=" << tolerant_gt(t_y,t_y1) << "y-y2=" << tolerant_gt(t_y,t_y2) << "y1-y=" << tolerant_gt(t_y1,t_y) << "y2-y=" << tolerant_gt(t_y2,t_y);*/
                if (ret==1)//really inside both --> we found an intersection
                {
                inters_x[nr_of_intersections]=pos_x;
                inters_y[nr_of_intersections]=pos_y;
                a1=g[gno1].p[setno1].data.ex[0][i+1]-g[gno1].p[setno1].data.ex[0][i];
                b1=g[gno1].p[setno1].data.ex[1][i+1]-g[gno1].p[setno1].data.ex[1][i];
                a2=g[gno2].p[setno2].data.ex[0][j+1]-g[gno2].p[setno2].data.ex[0][j];
                b2=g[gno2].p[setno2].data.ex[1][j+1]-g[gno2].p[setno2].data.ex[1][j];
                hyp1=hypot(a1,b1);
                hyp2=hypot(a2,b2);
                    if (hyp1<10.0*DBL_EPSILON || hyp2<10.0*DBL_EPSILON) inters_a[nr_of_intersections++]=0.0;
                    else
                    {
                    hyp1=(a1*a2+b1*b2)/(hyp1*hyp2);
                    inters_a[nr_of_intersections++]=acos(hyp1);
                    //qDebug() << "hyp1=" << hyp1 << "acos(hyp1)=" << acos(hyp1);
                    }
                }
            }
        break;
        case -1://infinitely many intersection points
            //we have to find the first intersection points here! We take only the limiting points of th two lines.
            ret=simple_check_inside(g[gno1].p[setno1].data.ex[0][i],g[gno1].p[setno1].data.ex[1][i],g[gno2].p[setno2].data.ex[0][j],g[gno2].p[setno2].data.ex[1][j],g[gno2].p[setno2].data.ex[0][j+1],g[gno2].p[setno2].data.ex[1][j+1]);
            if (ret==1)
            {
            inters_x[nr_of_intersections]=g[gno1].p[setno1].data.ex[0][i];
            inters_y[nr_of_intersections]=g[gno1].p[setno1].data.ex[1][i];
            inters_a[nr_of_intersections++]=0.0;//parallel lines
            }
            ret=simple_check_inside(g[gno1].p[setno1].data.ex[0][i+1],g[gno1].p[setno1].data.ex[1][i+1],g[gno2].p[setno2].data.ex[0][j],g[gno2].p[setno2].data.ex[1][j],g[gno2].p[setno2].data.ex[0][j+1],g[gno2].p[setno2].data.ex[1][j+1]);
            if (ret==1)
            {
            inters_x[nr_of_intersections]=g[gno1].p[setno1].data.ex[0][i+1];
            inters_y[nr_of_intersections]=g[gno1].p[setno1].data.ex[1][i+1];
            inters_a[nr_of_intersections++]=0.0;//parallel lines
            }
            ret=simple_check_inside(g[gno2].p[setno2].data.ex[0][j],g[gno2].p[setno2].data.ex[1][j],g[gno1].p[setno1].data.ex[0][i],g[gno1].p[setno1].data.ex[1][i],g[gno1].p[setno1].data.ex[0][i+1],g[gno1].p[setno1].data.ex[1][i+1]);
            if (ret==1)
            {
            inters_x[nr_of_intersections]=g[gno2].p[setno2].data.ex[0][j];
            inters_y[nr_of_intersections]=g[gno2].p[setno2].data.ex[1][j];
            inters_a[nr_of_intersections++]=0.0;//parallel lines
            }
            ret=simple_check_inside(g[gno2].p[setno2].data.ex[0][j+1],g[gno2].p[setno2].data.ex[1][j+1],g[gno1].p[setno1].data.ex[0][i],g[gno1].p[setno1].data.ex[1][i],g[gno1].p[setno1].data.ex[0][i+1],g[gno1].p[setno1].data.ex[1][i+1]);
            if (ret==1)
            {
            inters_x[nr_of_intersections]=g[gno2].p[setno2].data.ex[0][j+1];
            inters_y[nr_of_intersections]=g[gno2].p[setno2].data.ex[1][j+1];
            inters_a[nr_of_intersections++]=0.0;//parallel lines
            }
        break;
        case 1://no intersection points at all
        default:
        break;
        }//switch-type
    }//j
}//i
*n=nr_of_intersections;
if (nr_of_intersections>0)
{
    if ((*x)!=NULL) delete[] (*x);
    if ((*y)!=NULL) delete[] (*y);
    if ((*a)!=NULL) delete[] (*a);
    (*x)=new double[1+nr_of_intersections];
    (*y)=new double[1+nr_of_intersections];
    (*a)=new double[1+nr_of_intersections];
    memcpy(*x,inters_x,sizeof(double)*nr_of_intersections);
    memcpy(*y,inters_y,sizeof(double)*nr_of_intersections);
    memcpy(*a,inters_a,sizeof(double)*nr_of_intersections);
/*qDebug() << "Intersections:";
    for (int i=0;i<nr_of_intersections;i++)
    {
        qDebug() << inters_x[i] << " " << inters_y[i] << "a=" << inters_a[i];
    }*/
}
delete[] lv1[0];
delete[] lv1[1];
delete[] infi[0];
delete[] lv2[0];
delete[] lv2[1];
delete[] infi[1];
delete[] inters_x;
delete[] inters_y;
delete[] inters_a;
if (*n>0) return RETURN_SUCCESS;
else return RETURN_FAILURE;
}

/*
 * this function takes two lines y=a*x+b and calculates the crossing point (if it exists)
 * returns 0 if the point exists
 * returns 1 if no such point exists (i.e. both lines are parallel, but not the same line)
 * returns -1 if infinitely many points exist (i.e. both lines are the same)
 * inif1 and inif2 have to be =0 if the slope is a regular value and have to be =1 if the slope is infinite (in this case b is not only the x-value for y=0, but the only x-value for all points on the line)
 */
int get_simple_line_crossing(double a1,double b1,int infi1,double a2,double b2,int infi2,double * x,double * y)
{
const double d_eps=10.0*DBL_EPSILON;
    if (infi1==1)//first line vertical
    {
        if (infi2==1)//second line vertical
        {
            if (fabs(b1-b2)<d_eps)//same vertical line --> infinitely many intersection points
            {
            *x=b1;
            *y=0.0;
            return -1;
            }
            else//not the same line --> parallel --> no intersection
            {
            *x=*y=0.0;
            return 1;
            }
        }
        else//second line NOT vertical --> there is an intersection
        {
            *x=b1;//because there are no other x-values on the first line
            *y=a2*b1+b2;//the intersection point is on the second line at the x-value of the first (vertical) line
            return 0;
        }
    }
    else//first line NOT vertical
    {
        if (infi2==1)//second line vertical
        {
            *x=b2;
            *y=a1*b2+b1;//the intersection point is on the first line at the x-value of the second (vertical) line
            return 0;
        }
        else if (fabs(a2-a1)<d_eps)//both lines have the same slope
        {
            if (fabs(b2-b1)<d_eps)//both lines are the same --> infinitely many intersection points
            {
            *x=0.0;
            *y=b2;
            return -1;
            }
            else//different parallel line --> no intersection
            {
            *x=*y=0.0;
            return 1;
            }
        }
        else//both lines have a different slope --> there has to be an intersection
        {
            if (fabs(a1)<d_eps)//line1 is a horizontal line
            {
            *y=b1;
            *x=(*y-b2)/a2;
            }
            else if (fabs(a2)<d_eps)//line2 is a horizontal line
            {
            *y=b2;
            *x=(*y-b1)/a1;
            }
            else
            {
            *x=(b2-b1)/(a1-a2);
            *y=a1*(*x)+b1;
            }
            return 0;
        }
    }
return 1;
}

/*
 * get the (first) intersection point between two sets
 */
int get_set_crossing(int gno1,int setno1,int gno2,int setno2,int restriction,int negate,double * cx,double * cy)
{
    /*ofstream ofi;
    ofi.open("debug_info.txt");*/
/// test get intesection line
cout << "test set intersection G" << gno2 << ".S" << setno2 << endl;
world w;
get_graph_world(gno1,&w);
double * xw=new double[4];
double * yw=new double[4];
xw[0]=w.xg1;
yw[0]=w.yg1;
xw[1]=w.xg1;
yw[1]=w.yg2;
xw[2]=w.xg2;
yw[2]=w.yg2;
xw[3]=w.xg2;
yw[3]=w.yg1;
int mn1,mn2;

int ret0=find_section_that_crosses_rectangle(xw,yw,g[gno2].p[setno2].data.ex[0],g[gno2].p[setno2].data.ex[1],g[gno2].p[setno2].data.len,&mn1,&mn2);

    if (ret0==RETURN_SUCCESS)
    cout << "n1=" << mn1 << " n2=" << mn2 << endl;
    else
    cout << "no intersection" << endl;
delete[] xw;
delete[] yw;
/// test get intersectionline end

char * rarray1,*rarray2;
char errormessage[512],setname[32];
double * d_tmp;
//get restrictions
int ret=get_restriction_array(gno1,setno1,restriction,negate,&rarray1);
if (ret==RETURN_FAILURE) return RETURN_FAILURE;
ret=get_restriction_array(gno2,setno2,restriction,negate,&rarray2);
if (ret==RETURN_FAILURE) return RETURN_FAILURE;
int n1,n2;
n1=getsetlength(gno1,setno1);
n2=getsetlength(gno2,setno2);
double *x1,*y1,*x2,*y2;
int * indices;
double *mesh=NULL;
x1=new double[n1+2];
y1=new double[n1+2];
x2=new double[n2+2];
y2=new double[n2+2];
double * yint1,*yint2;
int meshlen=(n1>n2?n1:n2);
indices=new int[meshlen+2];
int nn1,nn2;
double xmin,xmax;
nn1=nn2=0;
//apply restriction
    for (int i=0;i<n1;i++)
    {
        if (rarray1[i]==1)
        {
        indices[nn1]=nn1;
        x1[nn1]=g[gno1].p[setno1].data.ex[0][i];
        y1[nn1++]=g[gno1].p[setno1].data.ex[1][i];
        }
    }
if (nn1==0)
{
ret=RETURN_FAILURE;
strcpy(errormessage,QObject::tr("No data points in set ").toLocal8Bit().constData());
sprintf(setname,"G%d.S%d",gno1,setno1);
strcat(errormessage,setname);
strcat(errormessage,QObject::tr(" inside restriction area").toLocal8Bit().constData());
errmsg(errormessage);
goto end_of_get_set_crossing;
}
    for (int i=0;i<n2;i++)
    {
        if (rarray2[i]==1)
        {
        x2[nn2]=g[gno2].p[setno2].data.ex[0][i];
        y2[nn2++]=g[gno2].p[setno2].data.ex[1][i];
        }
    }
if (nn2==0)
{
ret=RETURN_FAILURE;
strcpy(errormessage,QObject::tr("No data points in set ").toLocal8Bit().constData());
sprintf(setname,"G%d.S%d",gno2,setno2);
strcat(errormessage,setname);
strcat(errormessage,QObject::tr(" inside restriction area").toLocal8Bit().constData());
errmsg(errormessage);
goto end_of_get_set_crossing;
}

/*for (int i=0;i<nn1;i++)
{
    ofi << x1[i] << " " << y1[i] << endl;
}
ofi << endl;
for (int i=0;i<nn2;i++)
{
    ofi << x2[i] << " " << y2[i] << endl;
}
ofi << endl;*/

//sort the two sets
vptr=x1;
qsort(indices, nn1, sizeof(int), compare_points1);
d_tmp=new double[nn1];
for (int i=0;i<nn1;i++)
d_tmp[i]=x1[indices[i]];
for (int i=0;i<nn1;i++)
x1[i]=d_tmp[i];
for (int i=0;i<nn1;i++)
d_tmp[i]=y1[indices[i]];
for (int i=0;i<nn1;i++)
y1[i]=d_tmp[i];
delete[] d_tmp;
for (int i=0;i<nn2;i++) indices[i]=i;
vptr=x2;
qsort(indices, nn2, sizeof(int), compare_points1);
d_tmp=new double[nn2];
for (int i=0;i<nn2;i++)
d_tmp[i]=x2[indices[i]];
for (int i=0;i<nn2;i++)
x2[i]=d_tmp[i];
for (int i=0;i<nn2;i++)
d_tmp[i]=y2[indices[i]];
for (int i=0;i<nn2;i++)
y2[i]=d_tmp[i];
delete[] d_tmp;

/*for (int i=0;i<nn1;i++)
{
    ofi << x1[i] << " " << y1[i] << endl;
}
ofi << endl;
for (int i=0;i<nn2;i++)
{
    ofi << x2[i] << " " << y2[i] << endl;
}
ofi << endl;*/

//determine minimal intersection range (in x)
xmin=x1[0];
xmax=x1[nn1-1];
if (x2[0]<xmin) xmin=x2[0];
if (x2[nn2-1]>xmax) xmax=x2[nn2-1];
    //cout << "x in [" << xmin << "|" << xmax << "]" << endl;
/*if (xmax<xmin)
{
ret=RETURN_FAILURE;
strcpy(errormessage,QObject::tr("No intersection within restriction area").toLocal8Bit().constData());
errmsg(errormessage);
goto end_of_get_set_crossing;
}*/
meshlen=(nn1>nn2?nn1:nn2)*2;
mesh = allocate_mesh(xmin, xmax, meshlen);
yint1=new double[meshlen];
yint2=new double[meshlen];
ret = interpolate(mesh, yint1, meshlen, x1, y1, nn1, INTERP_LINEAR);
if (ret==RETURN_FAILURE)
{
ret=RETURN_FAILURE;
strcpy(errormessage,QObject::tr("Unable to interpolate set ").toLocal8Bit().constData());
sprintf(setname,"G%d.S%d",gno1,setno1);
strcat(errormessage,setname);
errmsg(errormessage);
goto end_of_get_set_crossing;
}
ret = interpolate(mesh, yint2, meshlen, x2, y2, nn2, INTERP_LINEAR);

/*for (int i=0;i<meshlen;i++)
{
    ofi << mesh[i] << " " << yint1[i] << endl;
}
ofi << endl;
for (int i=0;i<meshlen;i++)
{
    ofi << mesh[i] << " " << yint2[i] << endl;
}
ofi << endl;*/

if (ret==RETURN_FAILURE)
{
ret=RETURN_FAILURE;
strcpy(errormessage,QObject::tr("Unable to interpolate set ").toLocal8Bit().constData());
sprintf(setname,"G%d.S%d",gno2,setno2);
strcat(errormessage,setname);
errmsg(errormessage);
goto end_of_get_set_crossing;
}
delete[] y1;
y1=new double[meshlen];
for (int i=0;i<meshlen;i++) y1[i]=yint2[i]-yint1[i];

/*for (int i=0;i<meshlen;i++)
{
    ofi << mesh[i] << " " << y1[i] << endl;
}
ofi.close();*/
//ofi << endl;

ret=get_zero_crossing(meshlen,mesh,y1,&xmin);
delete[] y2;
y2=new double[4];
for (int i=0;i<meshlen-1;i++)
{
    if (y1[i]==0.0 || y1[i]*y1[i+1]<0.0)//we found the intersection point
    {
    y2[0]=mesh[i];
    y2[1]=yint1[i];
    y2[2]=mesh[i+1];
    y2[3]=yint1[i+1];
    break;
    }
}
/*cout << "p1=(" << y2[0] << "|" << y2[1] << ")" << endl;
cout << "p1=(" << y2[2] << "|" << y2[3] << ")" << endl;*/
xmax=linear_interp(y2[0],y2[1],y2[2],y2[3],xmin);
*cx=xmin;
*cy=xmax;
ret=RETURN_SUCCESS;
end_of_get_set_crossing:
xfree(mesh);
delete[] x1;
delete[] x2;
delete[] y1;
delete[] y2;
delete[] indices;
return ret;
}

/*
 * compute the mins and maxes of a vector x
 */
void minmax(double *x, int n, double *xmin, double *xmax, int *imin, int *imax)
{
    int i;
    
    *imin = 0;
    *imax = 0;

    if (x == NULL) {
        *xmin = 0.0;
        *xmax = 0.0;
        return;
    }
    
    *xmin = x[0];
    *xmax = x[0];
    
    for (i = 1; i < n; i++) {
	if (x[i] < *xmin) {
	    *xmin = x[i];
	    *imin = i;
	}
	if (x[i] > *xmax) {
	    *xmax = x[i];
	    *imax = i;
	}
    }
}


/*
 * compute the min and max of vector vec calculated for indices such that
 * bvec values lie within [bmin, bmax] range
 * returns RETURN_FAILURE if none found
 */
int minmaxrange(double *bvec, double *vec, int n, double bvmin, double bvmax,
              	   double *vmin, double *vmax)
{
    int i, first = TRUE;
    
    if ((vec == NULL) || (bvec == NULL)) {
        return RETURN_FAILURE;
    }
    
    for (i = 0; i < n; i++) {
        if ((bvec[i] >= bvmin) && (bvec[i] <= bvmax)) {
	    if (first == TRUE) {
                *vmin = vec[i];
                *vmax = vec[i];
                first = FALSE;
            } else {
                if (vec[i] < *vmin) {
                    *vmin = vec[i];
  	        } else if (vec[i] > *vmax) {
                    *vmax = vec[i];
       	        }
            }
        }
    }
    
    if (first == FALSE) {
        return RETURN_SUCCESS;
    } else {
        return RETURN_FAILURE;
    }
}

/* same as above, but including the error-bars*/
int minmaxrange2(double *bvec, double *vec, double *error_vec1, double *error_vec2, int n, double bvmin, double bvmax,
                   double *vmin, double *vmax)
{
    int i, first = TRUE;

    if ((vec == NULL) || (bvec == NULL) || (error_vec1 == NULL) || (error_vec2 == NULL) ) {
        return RETURN_FAILURE;
    }

    for (i = 0; i < n; i++) {
        if ((bvec[i] >= bvmin) && (bvec[i] <= bvmax)) {
        if (first == TRUE) {
                *vmin = vec[i]-error_vec1[i];
                *vmax = vec[i]+error_vec2[i];
                first = FALSE;
            } else {
                if (vec[i]-error_vec1[i] < *vmin) {
                    *vmin = vec[i]-error_vec1[i];
            } else if (vec[i]+error_vec2[i] > *vmax) {
                    *vmax = vec[i]+error_vec2[i];
                }
            }
        }
    }

    if (first == FALSE) {
        return RETURN_SUCCESS;
    } else {
        return RETURN_FAILURE;
    }
}

/*
 * compute the mins and maxes of a vector x
 */
double vmin(double *x, int n)
{
    int i;
    double xmin;
    if (n <= 0) {
	return 0.0;
    }
    xmin = x[0];
    for (i = 1; i < n; i++) {
	if (x[i] < xmin) {
	    xmin = x[i];
	}
    }
    return xmin;
}

double vmax(double *x, int n)
{
    int i;
    double xmax;
    if (n <= 0) {
	return 0.0;
    }
    xmax = x[0];
    for (i = 1; i < n; i++) {
	if (x[i] > xmax) {
	    xmax = x[i];
	}
    }
    return xmax;
}

int set_point(int gno, int setno, int seti, WPoint wp)
{
    if (is_valid_setno(gno, setno) != TRUE) {
        return RETURN_FAILURE;
    }
    if (seti >= getsetlength(gno, setno) || seti < 0) {
        return RETURN_FAILURE;
    }
    (getcol(gno, setno, DATA_X))[seti] = wp.x;
    (getcol(gno, setno, DATA_Y))[seti] = wp.y;
    set_dirtystate();
    return RETURN_SUCCESS;
}

int get_point(int gno, int setno, int seti, WPoint *wp)
{
    if (is_valid_setno(gno, setno) != TRUE) {
        return RETURN_FAILURE;
    }
    if (seti >= getsetlength(gno, setno) || seti < 0) {
        return RETURN_FAILURE;
    }
    wp->x = (getcol(gno, setno, DATA_X))[seti];
    wp->y = (getcol(gno, setno, DATA_Y))[seti];
    return RETURN_SUCCESS;
}

void copycol2(int gfrom, int setfrom, int gto, int setto, int col)
{
    int i, n1, n2;
    double *x1, *x2;

    if (is_valid_setno(gfrom, setfrom) != TRUE ||
        is_valid_setno(gto, setto) != TRUE) {
        return;
    }
    n1 = getsetlength(gfrom, setfrom);
    n2 = getsetlength(gto, setto);
    if (n1 != n2) {
        return;
    }
    x1 = getcol(gfrom, setfrom, col);
    x2 = getcol(gto, setto, col);
    for (i = 0; i < n1; i++) {
	x2[i] = x1[i];
    }
    set_dirtystate();
}


int pushset(int gno, int setno, int push_type)
{
    int i;
    
    if (is_valid_setno(gno, setno) != TRUE) {
        return RETURN_FAILURE;
    } else {
        switch (push_type) {
        case PUSH_SET_TOFRONT:
            new_set_no = nextSetAfterHighestID(gno)-1;
            //new_set_no = number_of_sets(gno) - 1;
            for (i = setno; i < new_set_no; i++) {
                if (swapset(gno, i, gno, i + 1) != RETURN_SUCCESS) {
                    return RETURN_FAILURE;
                }
            }
            break;
        case PUSH_SET_TOBACK:
            new_set_no = 0;
            for (i = setno; i > new_set_no; i--) {
                if (swapset(gno, i, gno, i - 1) != RETURN_SUCCESS) {
                    return RETURN_FAILURE;
                }
            }
            break;
        default:
            return RETURN_FAILURE;
            break;
        }
        return RETURN_SUCCESS;
    }
}

/*
 * pack all sets leaving no gaps in the set structure
 */
void packsets(int gno)
{
    int i, j;

    for (i = 0; i < number_of_sets(gno); i++) {
	if (is_set_active(gno, i)) {
	    for (j = 0; j < i; j++) {
		if (is_set_active(gno, j) != TRUE) {
		    moveset(gno, i, gno, j);
		}
	    }
	}
    }
}

int allocate_set(int gno, int setno)
{
    if (is_valid_setno(gno, setno)) {
        return RETURN_SUCCESS;
    } else
    if (setno >= 0) {
        return realloc_graph_plots(gno, setno + 1);
    } else {
        return RETURN_FAILURE;
    }
}    

int activateset(int gno, int setno)
{
    int retval;
    
    if (is_valid_gno(gno) != TRUE) {
        return RETURN_FAILURE;
    } else {
        retval = allocate_set(gno, setno);
        if (retval == RETURN_SUCCESS) {
            set_set_hidden(gno, setno, FALSE);
        }
        return retval;
    }
}

static target recent_target = {-1, -1};

int get_recent_setno(void)
{
    return recent_target.setno;
}

int get_recent_gno(void)
{
    return recent_target.gno;
}

int nextSetAfterHighestID(int gno)
{
int ret=0;
if (is_valid_gno(gno)==FALSE) return -1;
for (int i=0;i<number_of_sets(gno);i++)
{
if (is_set_active(gno,i)==TRUE) ret=i+1;
}
return ret;
}

/*
 * return the next available set in graph gno
 * If target is allocated but with no data, choose it (used for loading sets
 * from project files when sets aren't packed)
 */
int nextset(int gno)
{
    int setno;
    int maxplot;

    if ( is_valid_gno(gno) != TRUE ) {
        return (-1);
    }
    
    if ( (target_set.gno == gno) &&
         is_valid_setno(target_set.gno, target_set.setno) &&
         !is_set_active(gno, target_set.setno) ) {
	setno = target_set.setno;
	target_set.gno = -1;
	target_set.setno = -1;
    } else {
        maxplot = number_of_sets(gno);
        for (setno = 0; setno < maxplot; setno++) {
            if (!is_set_active(gno, setno)) {
                break;
            }
        }
        /* if no sets found, try allocating new one */
        if (setno == maxplot && allocate_set(gno, setno) != RETURN_SUCCESS) {
            return (-1);
        }
    }
    recent_target.gno = gno;
    recent_target.setno = setno;
    return (setno);
}

int nextset_after(int gno,int after)//like next_set,but only set-ids after one id is used
{
    int setno;
    int maxplot;

    if (is_valid_gno(gno) != TRUE) {
        return (-1);
    }

    if ( (target_set.gno == gno) &&
         is_valid_setno(target_set.gno, target_set.setno) &&
         !is_set_active(gno, target_set.setno)) {
    setno = target_set.setno;
    target_set.gno = -1;
    target_set.setno = -1;
    //qDebug() << "nextset_after1";
    } else {
        maxplot = number_of_sets(gno);
    //qDebug() << "nextset_after2, maxplot=" << maxplot << "after=" << after;
        for (setno = after+1; setno < maxplot; setno++) {
            if (!is_set_active(gno, setno)) {
                break;
            }
        }
        if (after>=maxplot) setno=after+1;
        /* if no sets found, try allocating new one */
        if (setno >= maxplot && allocate_set(gno, setno) != RETURN_SUCCESS) {
            return (-1);
        }
    }
    recent_target.gno = gno;
    recent_target.setno = setno;
    return (setno);
}

int get_next_set_ids(int gno,int n,int * ids)//the next n ids new sets would get
{
    if (is_valid_gno(gno) != TRUE)
    {
    return RETURN_FAILURE;
    }
int counter=0,setno;
int maxplot = number_of_sets(gno);
    for (setno = 0; setno < maxplot; setno++)
    {
        if (!is_set_active(gno, setno) && counter<n)
        {
            ids[counter++]=setno;
        }

    }
if (counter<n)
{
    for (setno=maxplot;setno<maxplot+n;setno++)
    {
    if (counter>=n) break;
    ids[counter++]=setno;
    }
}
return RETURN_SUCCESS;
}

int is_set_active(int gno, int setno)
{
    if (is_valid_setno(gno, setno) && getsetlength(gno, setno) > 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/*
 * return number of active set(s) in gno
 */
int number_of_active_sets(int gno)
{
    int setno, na;

    if (is_valid_gno(gno) != TRUE) {
        return -1;
    }
    
    na = 0;
    for (setno = 0; setno < number_of_sets(gno); setno++) {
	if (is_set_active(gno, setno) == TRUE) {
	    na++;
	}
    }
    return na;
}

/*
 * return number of active set(s) not to ignore for autoscale in gno
 */
int number_of_active_autoscale_sets(int gno)
{
    int setno, na;

    if (is_valid_gno(gno) != TRUE) {
        return -1;
    }

    na = 0;
    for (setno = 0; setno < number_of_sets(gno); setno++) {
    if (is_set_active(gno, setno) == TRUE && !is_set_hidden(gno, setno)) {
        if (g[gno].p[setno].ignore_in_autoscale == FALSE || autoscale_active==0)
        na++;
    }
    }
    return na;
}

/*
 * drop points from a set
 */
void droppoints(int gno, int setno, int startno, int endno)
{
    double *x;
    char **s;
    int i, j, len, ncols, dist;

    if (is_valid_setno(gno, setno) != TRUE) {
        return;
    }

    dist = endno - startno + 1;
    if (dist <= 0) {
        return;
    }
    
    len = getsetlength(gno, setno);
    
    if (dist == len) {
        killsetdata(gno, setno);
        return;
    }
    
    ncols = dataset_cols(gno, setno);
    for (j = 0; j < ncols; j++) {
	x = getcol(gno, setno, j);
	for (i = endno + 1; i < len; i++) {
	    x[i - dist] = x[i];
	}
    }
    if ((s = get_set_strings(gno, setno)) != NULL) {
	for (i = endno + 1; i < len; i++) {
	    s[i - dist] = copy_string(s[i - dist], s[i]);
	}
    }
    setlength(gno, setno, len - dist);
}

int join_multiple_sets(int nr,int * gnos,int * snos)//joins sets from multiple graphs into one set
{
int tmp_set,tmp_counter;
int start_gno,start_sno;
int ret=RETURN_SUCCESS;
int * tmp_sets=NULL;
if (nr<2)
{
errmsg(QObject::tr("Unable to join sets; number of sets < 2").toLocal8Bit().constData());
return RETURN_FAILURE;
}
tmp_sets=new int[nr+1];
start_gno=gnos[0];
start_sno=snos[0];
//join
tmp_counter=1;//we count the sets from the current graph
for (int i=1;i<nr;i++)
{
    if (gnos[i]!=start_gno) break;
    else tmp_counter++;
}
for (int i=0;i<tmp_counter;i++)
{
tmp_sets[i]=snos[i];
}
if (tmp_counter>1)
{
ret+=join_sets(start_gno,tmp_sets,tmp_counter);
}
for (int i=tmp_counter;i<nr;i++)
{
tmp_sets[0]=start_sno;
    if (gnos[i]!=start_gno)
    {
    tmp_set=nextset(start_gno);
    ret+=moveset(gnos[i],snos[i],start_gno,tmp_set);//move the next set into the first graph
    tmp_sets[1]=tmp_set;
    }
    else
    {
    tmp_sets[1]=snos[i];
    }
ret+=join_sets(start_gno,tmp_sets,2);
}
delete[] tmp_sets;
return ret;
}

/*
 * join several sets together; all but the first set in the list will be killed 
 */
int join_sets(int gno, int *sets, int nsets)
{
    int i, j, n, setno, setno_final, ncols, old_length, new_length;
    double *x1, *x2;
    char **s1, **s2;

    if (nsets < 2) {
        errmsg(QObject::tr("Unable to join sets; number of sets < 2").toLocal8Bit().constData());
        return RETURN_FAILURE;
    }
    
    setno_final = sets[0];
    ncols = dataset_cols(gno, setno_final);
    for (i = 0; i < nsets; i++) {
        setno = sets[i];
        if (is_valid_setno(gno, setno) != TRUE) {
            errmsg(QObject::tr("Invalid setno in the list").toLocal8Bit().constData());
            return RETURN_FAILURE;
        }
        if (dataset_cols(gno, setno) != ncols) {
            errmsg(QObject::tr("Can't join datasets with different number of cols").toLocal8Bit().constData());
            return RETURN_FAILURE;
        }
    }
    
    new_length = getsetlength(gno, setno_final);
    for (i = 1; i < nsets; i++) {
        setno = sets[i];
        old_length = new_length;
        new_length += getsetlength(gno, setno);
        if (setlength(gno, setno_final, new_length) != RETURN_SUCCESS) {
            return RETURN_FAILURE;
        }
        for (j = 0; j < ncols; j++) {
            x1 = getcol(gno, setno_final, j);
            x2 = getcol(gno, setno, j);
            for (n = old_length; n < new_length; n++) {
                x1[n] = x2[n - old_length];
            }
        }
        s1 = get_set_strings(gno, setno_final);
        s2 = get_set_strings(gno, setno);
        if (s1 != NULL && s2 != NULL) {
            for (n = old_length; n < new_length; n++) {
                s1[n] = copy_string(s1[n], s2[n - old_length]);
            }
        }
        killset(gno, setno);
    }
    
    return RETURN_SUCCESS;
}

void reverse_set(int gno, int setno)
{
    int n, i, j, k, ncols;
    double *x;
    char **s;

    if (!is_valid_setno(gno, setno)) {
	return;
    }
    n = getsetlength(gno, setno);
    ncols = dataset_cols(gno, setno);
    for (k = 0; k < ncols; k++) {
	x = getcol(gno, setno, k);
	for (i = 0; i < n / 2; i++) {
	    j = (n - 1) - i;
	    fswap(&x[i], &x[j]);
	}
    }
    if ((s = get_set_strings(gno, setno)) != NULL) {
	char *stmp;
        for (i = 0; i < n / 2; i++) {
	    j = (n - 1) - i;
	    stmp = s[i];
            s[i] = s[j];
            s[j] = stmp;
	}
    }
    set_dirtystate();
}

void restrict_set_to_region(int gno, int setno, int region, int invert)
{
if (region==RESTRICT_NONE) return;
char *rarray;
    if (!is_valid_setno(gno, setno))
    {
    return;
    }
int resno = get_restriction_array(gno,setno,region,invert,&rarray);
if (resno != RETURN_SUCCESS)
{
    errmsg(QObject::tr("Error in restricting set").toLocal8Bit().constData());
    return;
}
filter_set(gno,setno,rarray);
XCFREE(rarray);
}

/*
 * for ascending and descending sorts
 */
static int compare_points1(const void *p1, const void *p2)
{
    const int *i1, *i2;
    double a, b;
    i1 = (const int *)p1;
    i2 = (const int *)p2;
    a = vptr[*i1];
    b = vptr[*i2];
    if (a < b) {
	return -1;
    }
    if (a > b) {
	return 1;
    }
    return 0;
}

static int compare_points2(const void *p1, const void *p2)
{
    const int *i1, *i2;
    double a, b;
    i1 = (const int *)p1;
    i2 = (const int *)p2;
    a = vptr[*i1];
    b = vptr[*i2];
    if (a > b) {
	return -1;
    }
    if (a < b) {
	return 1;
    }
    return 0;
}

void sortset(int gno, int setno, int sorton, int stype)
{
    int i, j, nc, len, *ind;
    double *x, *xtmp;
    char **s, **stmp;

    /* get the vector to sort on */
    vptr = getcol(gno, setno, sorton);
    if (vptr == NULL) {
	errmsg("NULL vector in sort, operation cancelled, check set type");
	return;
    }

    len = getsetlength(gno, setno);
    if (len <= 1) {
	return;
    }
    
    /* allocate memory for permuted indices */
    ind = (int*)xmalloc(len*sizeof(int));///SIZEOF_INT);
    if (ind == NULL) {
	return;
    }
    /* allocate memory for temporary array */
    xtmp = (double *)xmalloc(len*sizeof(double));///SIZEOF_DOUBLE);
    if (xtmp == NULL) {
	xfree(ind);
	return;
    }
    
    s = get_set_strings(gno, setno);
    if (s != NULL) {
        stmp = (char**)xmalloc(len*sizeof(char *));
        if (stmp == NULL) {
	    xfree(xtmp);
	    xfree(ind);
        }
    } else {
        stmp = NULL;
    }
    
    /* initialize indices */
    for (i = 0; i < len; i++) {
	ind[i] = i;
    }

    /* sort */
    qsort(ind, len, sizeof(int), stype ? compare_points2 : compare_points1);

    /* straighten things out - done one vector at a time for storage */
    
    nc = dataset_cols(gno, setno);
    /* loop over the number of columns */
    for (j = 0; j < nc; j++) {
        /* get this vector and put into the temporary vector in the right order */
	x = getcol(gno, setno, j);
	for (i = 0; i < len; i++) {
	    xtmp[i] = x[ind[i]];
	}
        
        /* load it back to the set */
	for (i = 0; i < len; i++) {
	    x[i] = xtmp[i];
	}
    }
    
    /* same with strings, if any */
    if (s != NULL) {
	for (i = 0; i < len; i++) {
	    stmp[i] = s[ind[i]];
	}

	for (i = 0; i < len; i++) {
	    s[i] = stmp[i];
	}
    }
    
    /* free allocated temporary arrays */
    xfree(stmp);
    xfree(xtmp);
    xfree(ind);

    set_dirtystate();
}

/*
 * sort two arrays
 */
void sort_xy(double *tmp1, double *tmp2, int up, int sorton, int stype)
{

    int d, i, j;
    int lo = 0;
    double t1, t2;

    if (sorton == 1) {
	double *ttmp;

	ttmp = tmp1;
	tmp1 = tmp2;
	tmp2 = ttmp;
    }
    up--;

    for (d = up - lo + 1; d > 1;) {
	if (d < 5)
	    d = 1;
	else
	    d = (5 * d - 1) / 11;
	for (i = up - d; i >= lo; i--) {
	    t1 = tmp1[i];
	    t2 = tmp2[i];
	    if (!stype) {
		for (j = i + d; j <= up && (t1 > tmp1[j]); j += d) {
		    tmp1[j - d] = tmp1[j];
		    tmp2[j - d] = tmp2[j];
		}
		tmp1[j - d] = t1;
		tmp2[j - d] = t2;
	    } else {
		for (j = i + d; j <= up && (t1 < tmp1[j]); j += d) {
		    tmp1[j - d] = tmp1[j];
		    tmp2[j - d] = tmp2[j];
		}
		tmp1[j - d] = t1;
		tmp2[j - d] = t2;
	    }
	}
    }
    set_dirtystate();
}

/*
 * delete the point pt in setno
 */
void del_point(int gno, int setno, int pt)
{
    droppoints(gno, setno, pt, pt);
}

/*
 * add a point to setno
 */
void add_point(int gno, int setno, double px, double py)
{
    int len;
    double *x, *y;

    if (is_valid_setno(gno, setno)) {
	 len = getsetlength(gno, setno);
	 setlength(gno, setno, len + 1);
	 x = getx(gno, setno);
	 y = gety(gno, setno);
	 x[len] = px;
	 y[len] = py;
    }
}

void zero_datapoint(Datapoint *dpoint)
{
    int k;
    
    for (k = 0; k < MAX_SET_COLS; k++) {
        dpoint->ex[k] = 0.0;
    }
    dpoint->s = NULL;
    dpoint->orig_s = NULL;
}

/*
 * add a point to setno at ind
 */
int add_point_at(int gno, int setno, int ind, const Datapoint *dpoint)
{
    int len, col, ncols;
    double *ex;
    char **s;
    char **orig_s;

    if (is_valid_setno(gno, setno)) {
        len = getsetlength(gno, setno);
        if (ind < 0 || ind > len) {
            return RETURN_FAILURE;
        }
        len++;
        setlength(gno, setno, len);
        ncols = dataset_cols(gno, setno);
        for (col = 0; col < ncols; col++) {
            ex = getcol(gno, setno, col);
            if (ind < len - 1) {
                memmove(ex + ind + 1, ex + ind, (len - ind - 1)*sizeof(double));///SIZEOF_DOUBLE);
            }
            ex[ind] = dpoint->ex[col];
        }
        s = get_set_strings(gno, setno);
        orig_s = get_set_original_strings(gno, setno);
        if (s != NULL) {
            if (ind < len - 1) {
                memmove(s + ind + 1, s + ind, (len - ind - 1)*sizeof(char *));
                memmove(orig_s + ind + 1, orig_s + ind, (len - ind - 1)*sizeof(char *));
            }
            s[ind] = copy_string(NULL, dpoint->s);
            orig_s[ind] = copy_string(NULL, dpoint->orig_s);
        }
        set_dirtystate();
        return RETURN_SUCCESS;
    } else {
        return RETURN_FAILURE;
    }
}

int get_datapoint(int gno, int setno, int ind, int *ncols, Datapoint *dpoint)
{
    int n, col;
    double *ex;
    char **s;
    char **orig_s;
    n = getsetlength(gno, setno);
    if (ind < 0 || ind >= n) {
        return RETURN_FAILURE;
    } else {
        *ncols = dataset_cols(gno, setno);
        for (col = 0; col < *ncols; col++) {
            ex = getcol(gno, setno, col);
            dpoint->ex[col] = ex[ind];
        }
        s = get_set_strings(gno, setno);
        orig_s = get_set_original_strings(gno, setno);
        if (s != NULL) {
            dpoint->s = s[ind];
            dpoint->orig_s = orig_s[ind];
        } else {
            dpoint->s = NULL;
            dpoint->orig_s = NULL;
        }
        return RETURN_SUCCESS;
    }
}

void delete_byindex(int gno, int setno, int *ind)
{
    int i, j, cnt = 0;
    int ncols = dataset_cols(gno, setno);

    if (is_valid_setno(gno, setno) != TRUE) {
        return;
    }
    
    for (i = 0; i < getsetlength(gno, setno); i++) {
	if (ind[i]) {
	    cnt++;
	}
    }
    if (cnt == getsetlength(gno, setno)) {
	killset(gno, setno);
	return;
    }
    cnt = 0;
    for (i = 0; i < getsetlength(gno, setno); i++) {
	if (ind[i] == 0) {
	    for (j = 0; j < ncols; j++) {
                (getcol(gno, setno, j))[cnt] = (getcol(gno, setno, j))[i];
	    }
	    cnt++;
	}
    }
    setlength(gno, setno, cnt);
}

/*
 * move a set to another set, in possibly another graph
 */
int do_moveset(int gfrom, int setfrom, int gto, int setto)
{
    int retval;
    char buf[64];
    
    retval = moveset(gfrom, setfrom, gto, setto);
    if (retval != RETURN_SUCCESS) {
        sprintf(buf,
            "Error moving G%d.S%d to G%d.S%d",
            gfrom, setfrom, gto, setto);
        errmsg(buf);
    }
    return retval;
}

/*
 * do_copyset
 */
int do_copyset(int gfrom, int setfrom, int gto, int setto)
{
    int retval;
    char buf[64];
    
    retval = copyset(gfrom, setfrom, gto, setto);
    if (retval != RETURN_SUCCESS) {
        sprintf(buf,
            "Error copying G%d.S%d to G%d.S%d",
            gfrom, setfrom, gto, setto);
        errmsg(buf);
    }
    return retval;
}

/*
 * do_swapset
 */
int do_swapset(int gfrom, int setfrom, int gto, int setto)
{
    int retval;
    char buf[64];
    
    retval = swapset(gfrom, setfrom, gto, setto);
    if (retval != RETURN_SUCCESS) {
        sprintf(buf,
            "Error swapping G%d.S%d with G%d.S%d",
            gfrom, setfrom, gto, setto);
        errmsg(buf);
    }
    return retval;
}

/*
 * split a set into lpart length sets
 */
void do_splitsets(int gno, int setno, int lpart)
{
    int i, j, k, ncols, len, plen, tmpset, npsets;
    double *x;
    //char s[256];
    QString tmp_str;
    plotarr p;
    Dataset ds, dstmp;

    if ((len = getsetlength(gno, setno)) < 2) {
    errmsg(QObject::tr("Set length < 2").toLocal8Bit().constData());
	return;
    }
    if (lpart >= len) {
    errmsg(QObject::tr("Split length >= set length").toLocal8Bit().constData());
	return;
    }
    if (lpart <= 0) {
    errmsg(QObject::tr("Split length <= 0").toLocal8Bit().constData());
	return;
    }

    new_set_no=0;
    if (new_set_nos!=NULL)
    {
        delete[] new_set_nos;
        new_set_nos=NULL;
    }

    npsets = (len - 1)/lpart + 1;

    /* get number of columns in this set */
    ncols = dataset_cols(gno, setno);

    p = g[gno].p[setno];

    /* save the contents to a temporary buffer */
    memcpy(&ds, &p.data, sizeof(Dataset));

    /* zero data contents of the original set */
    zero_set_data(&g[gno].p[setno].data);

    /* now load each set */
    for (i = 0; i < npsets; i++) {
	plen = MIN2(lpart, len - i*lpart); 
    tmpset = nextset(gno);
        if (!is_valid_setno(gno, tmpset)) {
            errmsg(QObject::tr("Can't create new set").toLocal8Bit().constData());
            return;
        }
    append_to_storage(&new_set_no,&new_set_nos,1,&tmpset);
        /* set the plot parameters */
	dstmp = g[gno].p[tmpset].data;
        g[gno].p[tmpset] = p;
	g[gno].p[tmpset].data = dstmp;

	set_set_hidden(gno, tmpset, FALSE);
	if (setlength(gno, tmpset, plen) != RETURN_SUCCESS) {
            /* should not happen */
            return;
        }
        if (ds.s) {
            g[gno].p[tmpset].data.s = (char**)xmalloc(plen*sizeof(char *));
            g[gno].p[tmpset].data.orig_s = (char**)xmalloc(plen*sizeof(char *));
        }
        
        /* load the data into each column */
	for (k = 0; k < ncols; k++) {
	    x = getcol(gno, tmpset, k);
	    for (j = 0; j < plen; j++) {
		x[j] = ds.ex[k][i*lpart + j];
	    }
	}
        if (ds.s) {
	    for (j = 0; j < plen; j++) {
        g[gno].p[tmpset].data.s[j] = copy_string(NULL, ds.s[i*lpart + j]);
        g[gno].p[tmpset].data.orig_s[j] = copy_string(NULL, ds.orig_s[i*lpart + j]);
	    }
        }
        //sprintf(s, "partition %d of set G%d.S%d", i + 1, gno, setno);
        //setcomment(gno, tmpset, s);
    tmp_str=QObject::tr("partition ")+QString::number(i+1)+QObject::tr(" of set G")+QString::number(gno)+QString(".S")+QString::number(setno);
    setcomment(gno, tmpset, tmp_str.toLocal8Bit().constData());
    }
    
    free_set_data(&ds);
}

/*
 * drop points from an active set
 */
void do_drop_points(int gno, int setno, int startno, int endno)
{
    int setlength;
    char buf[256];

    if (!is_set_active(gno, setno)) {
	sprintf(buf, "Set %d not active", setno);
	errmsg(buf);
	return;
    }

    setlength = getsetlength(gno, setno);
    if (startno < 0) {
        startno = setlength + 1 + startno;
    }
    if (endno < 0) {
        endno = setlength + 1 + endno;
    }

    if (startno > endno) {
        iswap(&startno, &endno);
    }

    if (startno < 0) {
	errmsg("Start # < 0");
	return;
    }
    if (endno >= setlength) {
	errmsg("Ending # >= set length");
	return;
    }

    droppoints(gno, setno, startno, endno);
}

/*
 * sort sets, only works on sets of type XY
 */
void do_sort(int setno, int sorton, int stype)
{
    int i, gno = get_cg();
    char buf[256];

    if (setno == -1) {
	for (i = 0; i < number_of_sets(gno); i++) {
	    if (is_set_active(gno, i)) {
		sortset(gno, i, sorton, stype);
	    }
	}
    } else {
	if (!is_set_active(gno, setno)) {
	    sprintf(buf, "Set %d not active", setno);
	    errmsg(buf);
	    return;
	} else {
	    sortset(gno, setno, sorton, stype);
	}
    }
}

double setybase(int gno, int setno)
{
    double dummy, *y, ybase = 0.0;
    int len;

    if (is_valid_setno(gno, setno) != TRUE) {
        return 0.0;
    }
    
    y = getcol(gno, setno, DATA_Y);
    len = getsetlength(gno, setno);
    
    switch (g[gno].p[setno].baseline_type) {
    case BASELINE_TYPE_0:
        ybase = 0.0;
        break;
    case BASELINE_TYPE_SMIN:
        ybase = vmin(y, len);
        break;
    case BASELINE_TYPE_SAVG:
        stasum(y, len, &ybase, &dummy);
        break;
    case BASELINE_TYPE_SMAX:
        ybase = vmax(y, len);
        break;
    case BASELINE_TYPE_GMIN:
        ybase = g[gno].w.yg1;
        break;
    case BASELINE_TYPE_GMAX:
        ybase = g[gno].w.yg2;
        break;
    default:
        errmsg("Wrong type of baseline");
    }
    
    return(ybase);
}

int dataset_cols(int gno, int setno)
{
    return settype_cols(dataset_type(gno, setno));
}

int load_comments_to_legend(int gno, int setno)
{
//newLegendString=QString::fromLocal8Bit(getcomment(gno, setno));
    return set_legend_string(gno, setno, getcomment(gno, setno));
}

int filter_set(int gno, int setno, char *rarray)
{
    int i, ip, j, ncols;
    Dataset *dsp;
    
    if (is_valid_setno(gno, setno) != TRUE) {
        return RETURN_FAILURE;
    }
    if (rarray == NULL) {
        return RETURN_SUCCESS;
    }
    ncols = dataset_cols(gno, setno);
    dsp = &(g[gno].p[setno].data);
    ip = 0;
    for (i = 0; i < dsp->len; i++) {
        if (rarray[i]) {
            for (j = 0; j < ncols; j++) {
                dsp->ex[j][ip] = dsp->ex[j][i];
            }
            if (dsp->s != NULL) {
                dsp->s[ip] = copy_string(dsp->s[ip], dsp->s[i]);
                dsp->orig_s[ip] = copy_string(dsp->orig_s[ip], dsp->orig_s[i]);
            }
            ip++;
        }
    }
    setlength(gno, setno, ip);
    return RETURN_SUCCESS;
}

int is_set_in_graph(int gno,int setno)//tells you if at least one data point is inside the visible area of a graph
{
int ret=0;
int len=getsetlength(gno,setno);
    for (int i=0;i<len;i++)
    {
        if (inregion(MAXREGION,g[gno].p[setno].data.ex[0][i],g[gno].p[setno].data.ex[1][i]))
        {
        ret=1;
        break;
        }
    }
return ret;
}

int current_visible_sets(int gno,int ** list)//returns a list of all sets, that are currently visible
{
int ret=0;
if (*list!=NULL) delete[] *list;
*list=new int[2+g[gno].maxplot];
for (int i=0;i<g[gno].maxplot;i++)
{
    if (is_set_active(gno,i) && is_set_in_graph(gno,i))
    {
    (*list)[ret++]=i;
    }
}
return ret;
}

int extract_single_feature(int feature, int cgr,int cs,int restr,int neg,double * datum)
{
int extract_err=0;
double xmin,xmax,ymin,ymax,dummy=*datum;
double y1, y2;
int iy1, iy2, fts;
double *x;
char tbuf[1024];
int tmp_set=nextset(cgr);
tbuf[0]=tbuf[1]=tbuf[2]='\0';
copyset(cgr,cs,cgr,tmp_set);
restrict_set_to_region(cgr,tmp_set,restr,neg);
    getsetminmax(cgr, tmp_set, &xmin, &xmax, &ymin, &ymax);
    switch( feature ) {
    case FEATURE_Y_MIN:			/* Y minimum */
        *datum = ymin;
        break;
    case FEATURE_Y_MAX: 		/* Y maximum */
        *datum = ymax;
        break;
    case FEATURE_Y_AVG: 		/* Y mean    */
        stasum(gety(cgr, tmp_set), getsetlength(cgr, tmp_set), datum, &dummy);
        break;
    case FEATURE_Y_STD_DEV:			/* Y std dev */
        stasum(gety(cgr, tmp_set), getsetlength(cgr, tmp_set), &dummy, datum);
        break;
    case FEATURE_Y_MEDIAN: 		/* Y median  */
        getmedian( cgr, tmp_set, DATA_Y, datum );
        break;
    case FEATURE_X_MIN:			/* X minimum */
        *datum = xmin;
        break;
    case FEATURE_X_MAX: 		/* X maximum */
        *datum = xmax;
        break;
    case FEATURE_X_AVG: 		/* X mean    */
        stasum(getx(cgr, tmp_set), getsetlength(cgr, tmp_set), datum, &dummy);
        break;
    case FEATURE_X_STD_DEV:			/* X std dev */
        stasum(getx(cgr, tmp_set), getsetlength(cgr, tmp_set), &dummy, datum);
        break;
    case FEATURE_X_MEDIAN:			/* X median  */
        getmedian( cgr, tmp_set, DATA_X, datum );
        break;
    case FEATURE_FREQUENCY: 		/* frequency and period */
    case FEATURE_PERIOD:
        //cout << cg << " " << cs << " " << getsetlength(cg, cs) << " " << ilog2(getsetlength(cg, cs)) << endl;
        if ( ilog2(getsetlength(cgr, tmp_set)) <= 0)   	 /* only DFT */
            do_fourier(cgr, tmp_set, 0, 0, 1, 0, 0, 0);
        else		                                 /* FFT      */
            do_fourier(cgr, tmp_set, 1, 0, 1, 0, 0, 0);
        if (ilog2(getsetlength(cgr, tmp_set)) != 0)
        {
            sprintf( tbuf, "FT of set %d", cs );
            fts = 0;
            while( strcmp( tbuf, getcomment(cgr, fts)+1 ) ){
                fts++;
            }
            minmax(gety(cgr, fts), getsetlength(cgr, fts),&y1,&y2,&iy1,&iy2);
            x = getx(cgr, fts);
            if( feature == FEATURE_FREQUENCY )
                *datum = x[iy2];
            else
                *datum = 1./x[iy2];
            killset( cgr, fts );				/* get rid of Fourier set */
        }
        break;
    case FEATURE_ZERO_CROSSING:		/* first zero crossing */
        if( get_zero_crossing( getsetlength( cgr, tmp_set ), getx( cgr, tmp_set ),gety( cgr, tmp_set ), datum ) ){
            sprintf( tbuf+strlen(tbuf),"%s%d\n",QObject::tr("Unable to find zero crossing of set ").toLocal8Bit().constData(), cs );
            errwin( tbuf );
            extract_err = 1;
        }
        break;
    case FEATURE_RISE_TIME:		/* rise time   */
        getsetminmax(cgr, tmp_set, &xmin, &xmax, &ymin, &ymax);
        if( get_rise_time( getsetlength(cgr,tmp_set), getx(cgr,tmp_set), gety(cgr,tmp_set), ymin, ymax, datum ) ){
            sprintf( tbuf+strlen(tbuf),"%s%d\n",QObject::tr("Unable to find rise time of set ").toLocal8Bit().constData(), cs );
            errwin( tbuf );
            extract_err = 1;
        }
        break;
    case FEATURE_FALL_TIME: 		/* fall time   */
        getsetminmax(cgr, cs, &xmin, &xmax, &ymin, &ymax);
        if( get_fall_time( getsetlength(cgr,tmp_set), getx(cgr,tmp_set), gety(cgr,tmp_set), ymin, ymax, datum ) ){
            sprintf( tbuf+strlen(tbuf),"%s%d\n",QObject::tr("Unable to find fall time of set ").toLocal8Bit().constData(), cs );
            extract_err = 1;
            errwin( tbuf );
        }
        break;
    case FEATURE_SLOPE:		/* slope       */
        if( mute_linear_regression( getsetlength( cgr, tmp_set ), getx( cgr, tmp_set ),gety( cgr, tmp_set ), datum, &dummy ) ) {
            sprintf( tbuf+strlen(tbuf),"%s%d\n",QObject::tr("Unable to find slope of set ").toLocal8Bit().constData(), cs );
            errwin( tbuf );
            extract_err = 1;
        }
        break;
    case FEATURE_Y_INTERCEPT:		/* Y intercept */
        if( mute_linear_regression( getsetlength( cgr, tmp_set ),getx( cgr, tmp_set ), gety( cgr, tmp_set ), &dummy, datum ) ) {
            sprintf( tbuf+strlen(tbuf),"%s%d\n",QObject::tr("Unable to find y-intercept of set ").toLocal8Bit().constData(), cs );
            errwin( tbuf );
            extract_err = 1;
        }
        break;
    case FEATURE_SET_LENGTH:		/* set length  */
        *datum = (double)getsetlength( cgr, tmp_set );
        break;
    case FEATURE_HALF_MAX_WIDTH:		/* half maximal widths */
        if (get_half_max_width(getsetlength(cgr, tmp_set), getx(cgr,tmp_set), gety(cgr,tmp_set), datum) != RETURN_SUCCESS) {
            sprintf( tbuf+strlen(tbuf),"%s%d\n",QObject::tr("Unable to find half maximal width of set ").toLocal8Bit().constData(), cs );
            extract_err = 1;
            errwin( tbuf );
        }
        break;
    case FEATURE_BARYCENTER_X:		/* Barycenter X */
        get_barycenter( getsetlength( cgr, tmp_set ), gety(cgr,tmp_set), getx(cgr,tmp_set), datum );
        break;
    case FEATURE_BARYCENTER_Y:		/* Barycenter Y */
        get_barycenter( getsetlength( cgr, tmp_set ), getx(cgr,tmp_set), gety(cgr,tmp_set), datum );
        break;
    case FEATURE_X_OF_YMAX:		/* X of Maximum Y */
        getsetminmax(cgr, tmp_set, &xmin, &xmax, &ymin, &ymax);
        get_max_pos( gety(cgr, tmp_set), getx( cgr, tmp_set ), getsetlength( cgr, tmp_set ), ymax, datum );
        break;
    case FEATURE_Y_OF_XMAX:		/* Y of Maximum X */
        getsetminmax(cgr, tmp_set, &xmin, &xmax, &ymin, &ymax);
        get_max_pos( getx(cgr, tmp_set), gety( cgr, tmp_set ), getsetlength( cgr, tmp_set ), xmax, datum );
        break;
    case FEATURE_INTEGRAL:		/* cumulative sum */
        *datum = do_int(cgr, tmp_set, 1);
        break;
    case FEATURE_VALUE_CROSSING:        /* y-value crossing */
        //cout << "returning Value-Crossing: G" << cgr << ".S" << cs << " value=" << dummy << endl;
        if( get_value_crossing(cgr, tmp_set, dummy, datum) == RETURN_FAILURE){
            sprintf( tbuf+strlen(tbuf),"%s%g%s%d\n",QObject::tr("Unable to find crossing of y-value ").toLocal8Bit().constData(),dummy,QObject::tr(" of set ").toLocal8Bit().constData(), cs );
            errwin( tbuf );
            extract_err = 1;
            *datum=0.0;
        }
        break;
    case FEATURE_VALUE_CROSSING2:        /* x-value crossing */
        if( get_value_crossing2(cgr, tmp_set, dummy, datum) ){
            sprintf( tbuf+strlen(tbuf),"%s%g%s%d\n",QObject::tr("Unable to find crossing of x-value ").toLocal8Bit().constData(),dummy,QObject::tr(" of set ").toLocal8Bit().constData(), cs );
            errwin( tbuf );
            extract_err = 1;
            *datum=0.0;
        }
        break;
    case FEATURE_X_OF_YMIN:		/* X of Minimum Y */
        getsetminmax(cgr, tmp_set, &xmin, &xmax, &ymin, &ymax);
        get_max_pos( gety(cgr, tmp_set), getx( cgr, tmp_set ), getsetlength( cgr, tmp_set ), ymin, datum );
        break;
    case FEATURE_Y_OF_XMIN:		/* Y of Minimum X */
        getsetminmax(cgr, tmp_set, &xmin, &xmax, &ymin, &ymax);
        get_max_pos( getx(cgr, tmp_set), gety( cgr, tmp_set ), getsetlength( cgr, tmp_set ), xmin, datum );
        break;
    }
killset(cgr,tmp_set);
return extract_err;
}

void ShiftSetAxis(int gno,int sno,double value,int axis)
{
    if (is_set_active(gno,sno)==FALSE || axis<0 || axis>=dataset_cols(gno,sno)) return;
    for (int i=0;i<g[gno].p[sno].data.len;i++)
    {
    g[gno].p[sno].data.ex[axis][i]+=value;
    }
}

void MultiplySetAxis(int gno,int sno,double value,int axis)
{
    if (is_set_active(gno,sno)==FALSE || axis<0 || axis>=dataset_cols(gno,sno)) return;
    for (int i=0;i<g[gno].p[sno].data.len;i++)
    {
    g[gno].p[sno].data.ex[axis][i]*=value;
    }
}

void CopySetAxis(int f_gno,int f_sno,int f_axis,int t_gno,int t_sno,int t_axis)
{
    if (is_set_active(f_gno,f_sno)==FALSE || is_set_active(t_gno,t_sno)==FALSE) return;//at least one set does not exist
    if (f_axis<0 || t_axis<0 || f_axis>=dataset_cols(f_gno,f_sno) || t_axis>=dataset_cols(t_gno,t_sno)) return;//at least one axis does not exist
    if (f_gno==t_gno && f_sno==t_sno && f_axis==t_axis) return;//everything is the same --> nothing to copy here
int len=g[f_gno].p[f_sno].data.len<g[t_gno].p[t_sno].data.len?g[f_gno].p[f_sno].data.len:g[t_gno].p[t_sno].data.len;
    for (int i=0;i<len;i++)
    {
    g[t_gno].p[t_sno].data.ex[t_axis][i]=g[f_gno].p[f_sno].data.ex[f_axis][i];
    }
}

void SwapSetColumns(int gno,int sno,int column1,int column2)
{
if (column1==column2 || column1<0 || column2<0 || column1>=dataset_cols(gno,sno) || column2>=dataset_cols(gno,sno)) return;//nothing to do here
int len=getsetlength(gno,sno);
double * tmp_col=new double[len];
memcpy(tmp_col,getcol(gno,sno,column1),sizeof(double)*len);
memcpy(getcol(gno,sno,column1),getcol(gno,sno,column2),sizeof(double)*len);
memcpy(getcol(gno,sno,column2),tmp_col,sizeof(double)*len);
delete[] tmp_col;
}

//we only find the first section!
int find_section_that_crosses_rectangle(double * xr,double * yr,double * x,double * y,int n,int * n1,int * n2)//n1 and n2 will be the index-numbers of the points in x,y that define a line that crosses the rectangle xr,yr (4 points)
{
int ret=RETURN_FAILURE;
int left_pos[4],above_pos[4];
int sum1,sum2;
if (n<2) return RETURN_FAILURE;
*n1=*n2=0;
for (int i=0;i<n-1;i++)//go through all points in the list
{
    sum1=sum2=0;
    for (int j=0;j<4;j++)
    {
    left_pos[j]=isleft(xr[j],yr[j],x[i],y[i],x[i+1],y[i+1]);
    sum1+=left_pos[j];
    above_pos[j]=isabove(xr[j],yr[j],x[i],y[i],x[i+1],y[i+1]);
    sum2+=above_pos[j];
    }
    if ((sum1!=0 && sum1!=4) || (sum2!=0 && sum2!=4))
    {
    *n1=i;
    *n2=i+1;
    ret=RETURN_SUCCESS;
    break;
    }
}
return ret;
}

int SmallSetOperations(int nsets,int * gnos,int * selset,int optype,int sorton,int stype,int lpart,double * param)
{
int i, j, setno, tmp_set, tmp_dat, cur_gr;
int counter;
int * n_gnos=NULL;
int * t_gnos=NULL;
int * n_snos=NULL;
QString descr_text;
char dummy[64];
double * mesh=NULL;
double * y_values=NULL;
    ///Undo-Stuff
    if (optype!=DATASETOP_RESTRICT && optype!=DATASETOP_AVERAGE_SET)//"restrict" and "average" have their own SaveSetStates-functions
    SaveSetStatesPrevious(nsets,gnos,selset,UNDO_COMPLETE);
switch (optype) {
case DATASETOP_SORT:
    for (i = 0; i < nsets; i++)
    {
        //setno = selset[i];
        //do_sort(setno, sorton, stype);
        sortset(gnos[i],selset[i],sorton,stype);
    }
    SetsModified(nsets,gnos,selset,UNDO_DATA);
    if (nsets>1)
    {
        addAditionalDescriptionToLastNode(-1,QObject::tr("Sort data sets"),QString(),-1);
    }
    else
    {
        sprintf(dummy,"[G%d.S%d]",gnos[0],selset[0]);
        addAditionalDescriptionToLastNode(-1,QObject::tr("Sort data set")+QString(dummy),QString(),-1);
    }
    break;
case DATASETOP_REVERSE:
    for (i = 0; i < nsets; i++)
    {
        reverse_set(gnos[i], selset[i]);
    }
    SetsModified(nsets,gnos,selset,UNDO_DATA);
    if (nsets>1)
    {
        addAditionalDescriptionToLastNode(-1,QObject::tr("Data sets reversed"),QString(),-1);
    }
    else
    {
        sprintf(dummy,"[G%d.S%d]",gnos[0],selset[0]);
        addAditionalDescriptionToLastNode(-1,QObject::tr("Data set reversed")+QString(dummy),QString(),-1);
    }
    break;
case DATASETOP_JOIN:
    join_multiple_sets(nsets,gnos,selset);
    SetsModified(1,gnos,selset,UNDO_COMPLETE);
    if (nsets>1)
    {
        addAditionalDescriptionToLastNode(-1,QObject::tr("Sets joined"),QString(),-1);
    }
    else
    {
        sprintf(dummy,"[G%d.S%d]",gnos[0],selset[0]);
        addAditionalDescriptionToLastNode(-1,QObject::tr("Set joined")+QString(dummy),QString(),-1);
    }
    break;
case DATASETOP_SPLIT:
    counter=0;
    n_gnos=NULL;//storage for the new set ids for undo
    n_snos=NULL;
    for (i = 0; i < nsets; i++)
    {
        do_splitsets(gnos[i], selset[i], sorton);
        if (new_set_no>0)
        {
        if (t_gnos!=NULL) delete[] t_gnos;
        t_gnos=new int[new_set_no+1];
        for (int j=0;j<new_set_no;j++) t_gnos[j]=gnos[i];
        //new_set_no=number of new sets (after split of one set)
        //new_set_nos=the ids of the new sets
        append_to_storage2(&counter,&n_gnos,&n_snos,new_set_no,t_gnos,new_set_nos);
        }
    }
    SetsModified(counter,n_gnos,n_snos,UNDO_COMPLETE);
    if (new_set_nos!=NULL) delete[] new_set_nos;
    if (new_graph_nos!=NULL) delete[] new_graph_nos;
    new_set_nos=new int[counter+1];
    new_graph_nos=new int[counter+1];
    new_set_no=counter;
    memcpy(new_graph_nos,n_gnos,counter*sizeof(int));
    memcpy(new_set_nos,n_snos,counter*sizeof(int));
        if (t_gnos!=NULL) delete[] t_gnos;
        if (n_gnos!=NULL) delete[] n_gnos;
        if (n_snos!=NULL) delete[] n_snos;
            t_gnos=NULL;
            n_gnos=NULL;
            n_snos=NULL;
    if (nsets>1)
    {
        addAditionalDescriptionToLastNode(-1,QObject::tr("Sets split"),QString(),-1);
    }
    else
    {
        sprintf(dummy,"[G%d.S%d]",gnos[0],selset[0]);
        addAditionalDescriptionToLastNode(-1,QObject::tr("Set split ")+QString(dummy),QString(),0);
    }
    break;
case DATASETOP_DROP:
    for (i = 0; i < nsets; i++)
    {
        do_drop_points(gnos[i], selset[i], sorton, stype);
    }
    SetsModified(nsets,gnos,selset,UNDO_DATA);
    if (nsets>1)
    {
        addAditionalDescriptionToLastNode(-1,QObject::tr("Points dropped"),QString(),-1);
    }
    else
    {
        sprintf(dummy,"[G%d.S%d]",gnos[0],selset[0]);
        addAditionalDescriptionToLastNode(-1,QObject::tr("Points dropped from ")+QString(dummy),QString(),-1);
    }
    break;
case DATASETOP_RESTRICT:
        if (lpart==0)//modify existing sets
        {
        SaveSetStatesPrevious(nsets,gnos,selset,UNDO_DATA);
        }
    for (i = 0; i < nsets; i++)
    {
    setno = selset[i];
        if (lpart==1)//create new sets
        {
        tmp_set=nextset(gnos[i]);
        do_copyset(gnos[i], setno, gnos[i], tmp_set);
        setno = selset[i] = tmp_set;
        }
        restrict_set_to_region(gnos[i],setno,sorton,stype);
    }
        if (lpart==0)//modify existing sets
        {
        SetsModified(nsets,gnos,selset,UNDO_DATA);
        }
        else//created new sets
        {
        SetsCreated(nsets,gnos,selset,UNDO_COMPLETE);
        }
    break;
case DATASETOP_SWAP_COLS:
    if (stype==sorton)
    {
    errwin(QObject::tr("Please select different columns to swap!").toLocal8Bit().constData());
    return RETURN_FAILURE;
    }
    else
    {
        SaveSetStatesPrevious(nsets,gnos,selset,UNDO_DATA);
        for (i = 0; i < nsets; i++)
        {
        SwapSetColumns(gnos[i],selset[i],stype,sorton);
        }
        SetsModified(nsets,gnos,selset,UNDO_DATA);
    }
    break;
case DATASETOP_AVERAGE_SET:
        if (nsets<2)
        {
        errwin(QObject::tr("Please select at least two sets!").toLocal8Bit().constData());
        return RETURN_FAILURE;
        }
    if (lpart<0)//no interpolation
    {
        cur_gr=get_cg();
        tmp_dat=getsetlength(gnos[0],selset[0]);
        descr_text=QObject::tr("Average of sets G") + QString::number(gnos[0]) +QString(".S")+ QString::number(selset[0]);
        for (i=1;i<nsets;i++)//check for same set lengths
        {
        descr_text+=QString(" + G") + QString::number(gnos[i]) +QString(".S")+ QString::number(selset[i]);
            if (tmp_dat!=getsetlength(gnos[i],selset[i]))
            {
            errwin(QObject::tr("The lengths of all selected sets have to be the same!").toLocal8Bit().constData());
            return RETURN_FAILURE;
            }
        }
        tmp_set=nextset(cur_gr);//create new set
        do_copyset(gnos[0],selset[0], cur_gr, tmp_set);//copy the first set (to have the first x- and y-values)
        set_dataset_type(cur_gr, tmp_set,SET_XY);
        for (i=1;i<nsets;i++)//go through all sets to add up the y-values
        {
            for (j=0;j<tmp_dat;j++)//go throught all points in a set
            {
            g[cur_gr].p[tmp_set].data.ex[DATA_Y][j]+=g[gnos[i]].p[selset[i]].data.ex[DATA_Y][j];
            }
        }
        for (j=0;j<tmp_dat;j++)//go throught all points in a set
        {
        g[cur_gr].p[tmp_set].data.ex[DATA_Y][j]/=nsets;
        }
        if (inhibit_set_legend==0)
        set_legend_string(cur_gr, tmp_set,descr_text.toLocal8Bit().constData());
        setcomment(cur_gr, tmp_set,descr_text.toLocal8Bit().constData());
        SetsCreated(1,&cur_gr,&tmp_set,UNDO_DATA);
        descr_text=QObject::tr("Created: ")+descr_text;
        addAditionalDescriptionToLastNode(-1,descr_text,QString(),-1);
    }
    else//interpolation
    {
        if (param[0]>=param[1] || lpart<1)
        {
        errwin(QObject::tr("Please set a suitable range and set length for interpolation!").toLocal8Bit().constData());
        return RETURN_FAILURE;
        }
//param[0]-->left
//param[1]-->right
//lpart-->length
        cur_gr=get_cg();
        tmp_set=nextset(cur_gr);//create new set
        activateset(cur_gr,tmp_set);
        setlength(cur_gr,tmp_set,lpart);
        mesh = allocate_mesh(param[0],param[1],lpart);
        y_values = (double*)xmalloc(lpart*sizeof(double));
        descr_text=QObject::tr("Average of sets G") + QString::number(gnos[0]) +QString(".S")+ QString::number(selset[0]);
        for (i=0;i<nsets;i++)//go through all sets to add up the y-values
        {
        tmp_dat=getsetlength(gnos[i],selset[i]);
        interpolate(mesh,y_values,lpart,g[gnos[i]].p[selset[i]].data.ex[DATA_X], g[gnos[i]].p[selset[i]].data.ex[DATA_Y],tmp_dat,INTERP_LINEAR);
            if (i==0)
            {
                for (j=0;j<lpart;j++)//go throught all points in a set
                {
                g[cur_gr].p[tmp_set].data.ex[DATA_X][j]=mesh[j];
                g[cur_gr].p[tmp_set].data.ex[DATA_Y][j]=y_values[j];
                }
            }
            else
            {
            descr_text+=QString(" + G") + QString::number(gnos[i]) +QString(".S")+ QString::number(selset[i]);
                for (j=0;j<lpart;j++)//go throught all points in a set
                {
                g[cur_gr].p[tmp_set].data.ex[DATA_Y][j]+=y_values[j];
                }
            }
        }
            for (j=0;j<lpart;j++)//go throught all points in a set
            {
            g[cur_gr].p[tmp_set].data.ex[DATA_Y][j]/=nsets;
            }
        xfree(mesh);
        xfree(y_values);
        descr_text+=QObject::tr(" (interpolated)");
        if (inhibit_set_legend==0)
        set_legend_string(cur_gr, tmp_set,descr_text.toLocal8Bit().constData());
        setcomment(cur_gr, tmp_set,descr_text.toLocal8Bit().constData());
        SetsCreated(1,&cur_gr,&tmp_set,UNDO_DATA);
        descr_text=QObject::tr("Created: ")+descr_text;
        addAditionalDescriptionToLastNode(-1,descr_text,QString(),-1);
    }
    break;
default:
    break;
}
set_dirtystate();
    if (t_gnos!=NULL) delete[] t_gnos;
    if (n_gnos!=NULL) delete[] n_gnos;
    if (n_snos!=NULL) delete[] n_snos;
return RETURN_SUCCESS;
}

int has_errorbar(int gno,int sno,int col)/*col=0: dx-error-bars, col=1: dy-error-bars*/
{
int ret=FALSE;
int settype = dataset_type(gno,sno);
if (settype==-1) return FALSE;
int has_x,has_y;
has_x=has_y=FALSE;
switch (settype)
{
case SET_XYDX:
case SET_XYDXDX:
has_x=TRUE;
break;
case SET_BARDY:
case SET_XYDYDY:
case SET_XYDY:
case SET_BARDYDY:
case SET_BOXPLOT:
has_y=TRUE;
break;
case SET_XYDXDXDYDY:
case SET_XYDXDY:
has_x=has_y=TRUE;
break;
case SET_XY:
case SET_BAR:
case SET_XYCOLPAT:
case SET_XYVMAP:
case SET_XYR:
case SET_XYCOLOR:
case SET_XYSIZE:
case SET_XYZ:
case SET_XYHILO:
default:
break;
}
    if (col==0) ret=has_x;
    else ret=has_y;
return ret;
}

int errorbar_cols(int gno,int sno,int col,int * cols)/*returns in cols the numbers of the columns that contain error-bar-values for col=0=x or col=1=y*/
{
int ret=0;
int settype = dataset_type(gno,sno);
if (settype==-1) return 0;
int dx1,dy1,dx2,dy2;
dx1=dy1=dx2=dy2=-1;//none
    switch (settype)
    {
    case SET_XYDX:
    dx1=2;
    break;
    case SET_XYDXDX:
    dx1=2;
    dx2=3;
    break;
    case SET_BARDY:
    dy1=2;
    break;
    case SET_XYDYDY:
    dy1=2;
    dy2=3;
    break;
    case SET_XYDY:
    dy1=2;
    break;
    case SET_BARDYDY:
    dy1=2;
    dy2=3;
    break;
    case SET_BOXPLOT:
    dy1=4;
    dy2=5;
    break;
    case SET_XYDXDXDYDY:
    dx1=2;
    dx2=3;
    dy1=4;
    dy2=5;
    break;
    case SET_XYDXDY:
    dx1=2;
    dy1=3;
    break;
    case SET_XY:
    case SET_BAR:
    case SET_XYCOLPAT:
    case SET_XYVMAP:
    case SET_XYR:
    case SET_XYCOLOR:
    case SET_XYSIZE:
    case SET_XYZ:
    case SET_XYHILO:
    default:
    break;
    }
        if (col==0)
        {
        cols[0]=dx1;
        cols[1]=dx2;
        ret=0;
        if (dx1>=0) ret++;
        if (dx2>=0) ret++;
        }
        else
        {
        cols[0]=dy1;
        cols[1]=dy2;
        ret=0;
        if (dy1>=0) ret++;
        if (dy2>=0) ret++;
        }
return ret;
}

char * get_alpha_channels_string(int gno,int sno)
{
static char alpha_string[128];
alpha_string[0]='\0';
if (is_valid_setno(gno,sno))
{
strcpy(alpha_string,create_list_of_arguments(4, g[gno].p[sno].linepen.alpha,g[gno].p[sno].setfillpen.alpha,g[gno].p[sno].sympen.alpha,g[gno].p[sno].symfillpen.alpha));
}
return alpha_string;
}

int getXYPointsInRestriction(int gno,int sno,int restriction,int negate,double ** target_x,double ** target_y,int * points_in_restriction)
{
if (is_valid_setno(gno,sno)==FALSE) return RETURN_FAILURE;
int nr_of_points=getsetlength(gno,sno);
if (nr_of_points<1) return RETURN_FAILURE;
char * rarray=NULL;
int rno=get_restriction_array(gno,sno,restriction,negate,&rarray);
if (rno==RETURN_FAILURE)
{
if (rarray!=NULL) xfree(rarray);
return RETURN_FAILURE;
}
if (*target_x) delete[] *target_x;
if (*target_y) delete[] *target_y;
*target_x=new double[2+nr_of_points];
*target_y=new double[2+nr_of_points];
    if (rarray==NULL)//everything
    {
    memcpy(*target_x,g[gno].p[sno].data.ex[DATA_X],sizeof(double)*nr_of_points);
    memcpy(*target_y,g[gno].p[sno].data.ex[DATA_Y],sizeof(double)*nr_of_points);
    (*points_in_restriction)=nr_of_points;
    }
    else
    {
    (*points_in_restriction)=0;
        for (int i=0;i<nr_of_points;i++)
        {
            if (rarray[i])
            {
            (*target_x)[*points_in_restriction]=g[gno].p[sno].data.ex[DATA_X][i];
            (*target_y)[*points_in_restriction]=g[gno].p[sno].data.ex[DATA_Y][i];
            (*points_in_restriction)++;
            }
        }
    }
xfree(rarray);
return RETURN_SUCCESS;
}

void makeSetACircle(int gno,int sno,double x0,double y0,double r)
{
    if (is_valid_setno(gno,sno)==FALSE) return;
int points=getsetlength(gno,sno);
    if (points<=0)
    {
    points=720;
    setlength(gno,sno,points);
    }
double factor=atan(1.0)*8.0/points;
    for (int i=0;i<points;i++)
    {
    g[gno].p[sno].data.ex[DATA_X][i]=x0+r*cos(i*factor);
    g[gno].p[sno].data.ex[DATA_Y][i]=y0+r*sin(i*factor);
    }
}

void makeSetAnEllipse(int gno,int sno,double x0,double y0,double a,double b,double alpha)
{
    if (is_valid_setno(gno,sno)==FALSE) return;
int points=getsetlength(gno,sno);
    if (points<=0)
    {
    points=720;
    setlength(gno,sno,points);
    }
double factor=atan(1.0)*8.0/points;
    for (int i=0;i<points;i++)
    {
    g[gno].p[sno].data.ex[DATA_X][i]=x0+a*cos(i*factor)*cos(alpha)-b*sin(i*factor)*sin(alpha);
    g[gno].p[sno].data.ex[DATA_Y][i]=y0+a*cos(i*factor)*sin(alpha)+b*sin(i*factor)*cos(alpha);
    }
}

void guess_circle(int gno,int sno,double * x0,double * y0,double * r,int restriction,int negate)
{
    if (is_valid_setno(gno,sno)==FALSE) return;
double * x=NULL,*y=NULL;
int points_in_restriction=0;
int retno=getXYPointsInRestriction(gno,sno,restriction,negate,&x,&y,&points_in_restriction);
if (retno==RETURN_FAILURE || points_in_restriction<=0)
{
if (x!=NULL) delete[] x;
if (y!=NULL) delete[] y;
return;
}
    (*x0)=x[0];
    (*y0)=y[0];
    for (int i=1;i<points_in_restriction;i++)
    {
    (*x0)+=x[i];
    (*y0)+=y[i];
    }
(*x0)*=1.0/points_in_restriction;
(*y0)*=1.0/points_in_restriction;
    (*r)=sqrt(((*x0)-x[0])*((*x0)-x[0])+((*y0)-y[0])*((*y0)-y[0]));
    for (int i=1;i<points_in_restriction;i++)
    (*r)+=sqrt(((*x0)-x[i])*((*x0)-x[i])+((*y0)-y[i])*((*y0)-y[i]));
(*r)*=1.0/points_in_restriction;
}

void guess_ellipse(int gno,int sno,double * x0,double * y0,double * a,double * b, double * alpha,int restriction,int negate)
{
    if (is_valid_setno(gno,sno)==FALSE) return;
double * x=NULL,*y=NULL;
int points_in_restriction=0;
int retno=getXYPointsInRestriction(gno,sno,restriction,negate,&x,&y,&points_in_restriction);
if (retno==RETURN_FAILURE || points_in_restriction<=0)
{
if (x!=NULL) delete[] x;
if (y!=NULL) delete[] y;
return;
}
    (*x0)=x[0];
    (*y0)=y[0];
    for (int i=1;i<points_in_restriction;i++)
    {
    (*x0)+=x[i];
    (*y0)+=y[i];
    }
(*x0)*=1.0/points_in_restriction;
(*y0)*=1.0/points_in_restriction;
double tmp;
int max_at=0;//,min_at=0;
(*b)=(*a)=sqrt(((*x0)-x[0])*((*x0)-x[0])+((*y0)-y[0])*((*y0)-y[0]));
    for (int i=1;i<points_in_restriction;i++)
    {
    tmp=sqrt(((*x0)-x[i])*((*x0)-x[i])+((*y0)-y[i])*((*y0)-y[i]));
        if ( (*a)<tmp )
        {
        max_at=i;
        (*a)=tmp;
        }
        if ( (*b)>tmp )
        {
        //min_at=i;
        (*b)=tmp;
        }
    }
(*alpha)=atan(((*y0)-y[max_at])/((*x0)-x[max_at]));
if (x!=NULL) delete[] x;
if (y!=NULL) delete[] y;
}

void fit_circle(int gno,int sno,int * n_sno,double * x0,double * y0,double * r,int restriction,int negate)
{
QString text;
QString setid=QString("G")+QString::number(gno)+QString(".S")+QString::number(sno);
if (is_valid_setno(gno,sno)==FALSE)
{
text=QObject::tr("Invalid set for fit of circle: G")+QString::number(gno)+QString(".S")+QString::number(sno);
errmsg(text.toLocal8Bit().constData());
*n_sno=-1;
return;
}
int nr_of_points=getsetlength(gno,sno);
if (nr_of_points<3)
{
text=setid+QObject::tr(" contains too few points to fit a cirle. At least three points needed.");
errmsg(text.toLocal8Bit().constData());
*n_sno=-1;
return;
}
*n_sno=nextset(gno);
activateset(gno,*n_sno);
setlength(gno,*n_sno,nr_of_points);
set_dataset_type(gno,*n_sno,SET_XY);
for (int i=0;i<nr_of_points;i++)
{
g[gno].p[*n_sno].data.ex[DATA_X][i]=0.0;
g[gno].p[*n_sno].data.ex[DATA_Y][i]=0.0;//only y=0 is important here
}
QString fit_formula;
fit_formula=QString("y=(")+setid+QString(".x-a0)*(")+setid+QString(".x-a0)+(")+setid+QString(".y-a1)*(")+setid+QString(".y-a1)-a2*a2");
fit_formula=fit_formula.toUpper();
//qDebug() << "Circle Fit Formula=" << fit_formula;
char *rarray=NULL;
int resno = get_restriction_array(gno, sno, restriction, negate, &rarray);
if (resno==RETURN_FAILURE)
{
errmsg(QObject::tr("Unable to get restriction-array during fit_circle-function!?").toLocal8Bit().constData());
return;
}
reset_nonl();
nonl_opts.formula = copy_string(nonl_opts.formula,fit_formula.toLatin1());
ReplaceDecimalSeparator(nonl_opts.formula);
nonl_opts.nsteps = 10;
nonl_opts.parnum = 3;
nonl_parms[0].value = *x0;
nonl_parms[1].value = *y0;
nonl_parms[2].value = *r;
/* The fit itself! */
resno = do_nonlfit(gno, *n_sno, NULL, rarray, nonl_opts.nsteps);
*x0 = nonl_parms[0].value;
*y0 = nonl_parms[1].value;
*r = nonl_parms[2].value;
makeSetACircle(gno, *n_sno,*x0,*y0,*r);
xfree(rarray);
}

void fit_ellipse(int gno,int sno,int * n_sno,double * x0,double * y0,double * a,double * b, double * alpha,int restriction,int negate)
{
QString text;
QString setid=QString("G")+QString::number(gno)+QString(".S")+QString::number(sno);
if (is_valid_setno(gno,sno)==FALSE)
{
text=QObject::tr("Invalid set for fit of ellipse: G")+QString::number(gno)+QString(".S")+QString::number(sno);
errmsg(text.toLocal8Bit().constData());
*n_sno=-1;
return;
}
int nr_of_points=getsetlength(gno,sno);
if (nr_of_points<5)
{
text=setid+QObject::tr(" contains too few points to fit an ellipse. At least five points needed.");
errmsg(text.toLocal8Bit().constData());
*n_sno=-1;
return;
}
*n_sno=nextset(gno);
activateset(gno,*n_sno);
setlength(gno,*n_sno,nr_of_points);
set_dataset_type(gno,*n_sno,SET_XY);
for (int i=0;i<nr_of_points;i++)
{
g[gno].p[*n_sno].data.ex[DATA_X][i]=0.0;
g[gno].p[*n_sno].data.ex[DATA_Y][i]=0.0;//only y=0 is important here
}
QString fit_formula;
fit_formula=QString("y=sqrt((")+setid+QString(".x-a0+sqrt(a2*a2-a3*a3)*cos(a4))^2+(")+setid+QString(".y-a1+sqrt(a2*a2-a3*a3)*sin(a4))^2)+sqrt((")+setid+QString(".x-a0-sqrt(a2*a2-a3*a3)*cos(a4))^2+(")+setid+QString(".y-a1-sqrt(a2*a2-a3*a3)*sin(a4))^2)-2*a2");
fit_formula=fit_formula.toUpper();
//qDebug() << "Ellipse Fit Formula=" << fit_formula;
char *rarray=NULL;
int resno = get_restriction_array(gno, sno, restriction, negate, &rarray);
if (resno==RETURN_FAILURE)
{
errmsg(QObject::tr("Unable to get restriction-array during fit_ellipse-function!?").toLocal8Bit().constData());
return;
}
reset_nonl();
nonl_opts.formula = copy_string(nonl_opts.formula,fit_formula.toLatin1());
ReplaceDecimalSeparator(nonl_opts.formula);
nonl_opts.nsteps = 15;
nonl_opts.parnum = 5;
nonl_parms[0].value = *x0;
nonl_parms[1].value = *y0;
nonl_parms[2].value = *a;
nonl_parms[3].value = *b;
nonl_parms[4].value = *alpha;
/* The fit itself! */
resno = do_nonlfit(gno, *n_sno, NULL, rarray, nonl_opts.nsteps);
*x0 = nonl_parms[0].value;
*y0 = nonl_parms[1].value;
*a = nonl_parms[2].value;
*b = nonl_parms[3].value;
*alpha = nonl_parms[4].value;
makeSetAnEllipse(gno, *n_sno,*x0,*y0,*a,*b,*alpha);
xfree(rarray);
}

//this function generates a
void createSetList(int gno,QList<int> & new_index,int nr_of_sets,int * shift_sets,int after_set)
{
new_index.clear();
if (is_valid_gno(gno)==FALSE) return;
QList<int> all_ids;
QList<int> shift_ids;
all_ids.clear();
shift_ids.clear();
    for (int i=0;i<nr_of_sets;i++)
    {
    shift_ids << shift_sets[i];
    }
    for (int i=0;i<number_of_sets(gno);i++)
    {
        if (is_set_active(gno,i)==TRUE)
        {
        all_ids << i;
        }
    }
int pos;
if (after_set<0)
{
new_index.append(shift_ids);
    for (int i=0;i<all_ids.length();i++)
    {
    if (shift_ids.indexOf(all_ids.at(i))<0) new_index << all_ids.at(i);
    }
}
else
{
pos=all_ids.indexOf(after_set);
    for (int i=0;i<=pos;i++)
    {
    if (shift_ids.indexOf(all_ids.at(i))<0) new_index << all_ids.at(i);
    }
new_index.append(shift_ids);
    for (int i=pos+1;i<all_ids.length();i++)
    {
    if (shift_ids.indexOf(all_ids.at(i))<0) new_index << all_ids.at(i);
    }
}
}

void reSortSets(int gno,QList<int> new_index)
{
if (is_valid_gno(gno)==FALSE) return;
int newsize=new_index.length();
if (newsize<g[gno].maxplot) newsize=g[gno].maxplot;
plotarr * parr = (plotarr *)xrealloc(NULL, newsize * sizeof(plotarr));
for (int i=0;i<new_index.length();i++)
{
memcpy(parr+i,g[gno].p+new_index.at(i),sizeof(plotarr));
}
for (int i=new_index.length();i<newsize;i++)
{
memcpy(parr+i,g[gno].p+i,sizeof(plotarr));
}
xfree(g[gno].p);//free the original
g[gno].p=parr;//keep the new one (do not delete parr here!)
}

void invertedList(QList<int> list, QList<int> & inv_list)
{
inv_list.clear();
for (int i=0;i<list.length();i++)
{
inv_list << list.indexOf(i);
//qDebug() << "Search" << i << "FoundAt" << inv_list.last();
}
}

void create_format_preferences_string(int gno,int sno,char * text)
{
    if (!is_valid_setno(gno,sno))
    {
    strcpy(text,"");
    }
    else
    {
    char tmp_str1[256];
    char tmp_str2[256];
    tmp_str1[0]='\0';
        for (int i=0;i<MAX_SET_COLS;i++)
        {
        sprintf(tmp_str2,"%d,",g[gno].p[sno].pref_col_format[i]);
        strcat(tmp_str1,tmp_str2);
        }
        tmp_str1[strlen(tmp_str1)-1]=';';
        for (int i=0;i<MAX_SET_COLS;i++)
        {
        sprintf(tmp_str2,"%d,",g[gno].p[sno].pref_col_prec[i]);
        strcat(tmp_str1,tmp_str2);
        }
        tmp_str1[strlen(tmp_str1)-1]='\0';
    strcpy(text,tmp_str1);
    }
}

void parse_format_preferences_string(char * text,int * pref_format,int * pref_prec)
//there are always MAX_SET_COLS entries (if entries are missing, the default[SpreadsheetColumnFormat,SpreadsheetColumnPrecision] will be used )
{
    for (int i=0;i<0;i++)//init settings with default
    {
    pref_format[i]=SpreadsheetColumnFormat;
    pref_prec[i]=SpreadsheetColumnPrecision;
    }
if (text[0]=='\0') return;
//qDebug() << "pars_text:" << text;
QString str_text(text);
QChar sep(';');
QStringList results1,results2;
splitStringIntoSubstrings(str_text,sep,results1);//separate into format and precision
if (results1.length()!=2) return;
char tmp_string[256];
strcpy(tmp_string,results1.at(0).toLocal8Bit().constData());
sep=QChar(',');
splitStringIntoSubstrings(tmp_string,sep,results2);
//qDebug() << "first half=" << results2;
for (int i=0;i<results2.length();i++)
{
if (i>=MAX_SET_COLS) break;
pref_format[i]=results2.at(i).toInt();
}
strcpy(tmp_string,results1.at(1).toLocal8Bit().constData());
splitStringIntoSubstrings(tmp_string,sep,results2);
//qDebug() << "second half=" << results2;
for (int i=0;i<results2.length();i++)
{
if (i>=MAX_SET_COLS) break;
pref_prec[i]=results2.at(i).toInt();
}



}

