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
 * spreadsheet data stuff
 *
 */

///#include <config.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "defines.h"
#include "globals.h"
#include "utils.h"
#include "graphs.h"
#include "graphutils.h"
#include "files.h"
#include "ssdata.h"
#include "parser.h"

#include "noxprotos.h"

extern bool exchange_point_comma;
extern int new_set_no;
extern int * new_set_nos;
extern char DecimalPointToUse;

double *copy_data_column(double *src, int nrows)
{
    double *dest;
    
    dest = (double*)xmalloc(nrows*sizeof(double));
    if (dest != NULL) {
        memcpy(dest, src, nrows*sizeof(double));
    }
    return dest;
}

char **copy_string_column(char **src, int nrows)
{
    char **dest;
    int i;

    dest = (char **)xmalloc(nrows*sizeof(char *));
    if (dest != NULL) {
        for (i = 0; i < nrows; i++)
            dest[i] = copy_string(NULL, src[i]);
    }
    return dest;
}

/* TODO: index_shift */
double *allocate_index_data(int nrows)
{
    int i;
    double *retval;
    
    retval = (double*)xmalloc(nrows*sizeof(double));
    if (retval != NULL) {
        for (i = 0; i < nrows; i++) {
            retval[i] = i;
        }
    }
    return retval;
}

double *allocate_mesh(double start, double stop, int len)
{
    int i;
    double *retval;
//qDebug() << "alloc_LIN_mesh: n=" << len << "start=" << start << "stop=" << stop;
    retval = (double*)xmalloc(len*sizeof(double));
    if (retval != NULL) {
        double s = (start + stop)/2, d = (stop - start)/2;
        if (len==1)
        {
        retval[0] = start;
        }
        else
        {
            for (i = 0; i < len; i++) {
                retval[i] = s + d*((double) (2*i + 1 - len)/(len - 1));
            }
        }
    }
    return retval;
}

double *allocate_logarithmic_mesh(double start, double stop, int len)//warning: if start or stop are <=0 NULL is returned!
{
    if ((start==0 || stop==0) || (start<=0 && stop>=0)) return NULL;//in this case calculating a logarithm does not work
    int i;
    int neg_sgn=(start<0 || stop<0)?1:0;
    double *retval;
//qDebug() << "alloc_LOG_mesh: n=" << len << "start=" << start << "stop=" << stop;
    retval = (double*)xmalloc(len*sizeof(double));
    if (retval != NULL)
    {
        double l_start=log(fabs(start));//log is in fact the natural logarithm
        double l_stop=log(fabs(stop));
        double l_s,l_d;
        if (l_stop<l_start)
        {
        l_s=l_start;
        l_start=l_stop;
        l_stop=l_s;
        }
        l_s = (l_start + l_stop)*0.5;
        l_d = (l_stop - l_start)*0.5;
        if (len==1)
        {
        retval[0] = start;
        }
        else
        {
            if (neg_sgn==1)
            {
                for (i = 0; i < len; i++)
                {
                    retval[len-1-i] = -exp(l_s + l_d*((double) (2*i + 1 - len)/(len - 1)));
                }
            }
            else
            {
                for (i = 0; i < len; i++)
                {
                    retval[i] = exp(l_s + l_d*((double) (2*i + 1 - len)/(len - 1)));
                }
            }

        }
    }
    return retval;
}

static char block_data_origin_file[GR_MAXPATHLEN];
static ss_data blockdata = {0, 0, NULL, NULL};

void set_block_origin(char * s)
{
strcpy(block_data_origin_file,s);
}

char * get_block_origin(void)
{
return block_data_origin_file;
}

void set_blockdata(ss_data *ssd)
{
    free_ss_data(&blockdata);
    if (ssd) {
        memcpy(&blockdata, ssd, sizeof(ss_data));
    }
}

int get_blockncols(void)
{
    return blockdata.ncols;
}

int get_blocknrows(void)
{
    return blockdata.nrows;
}

int *get_blockformats(void)
{
    return blockdata.formats;
}

double * get_blockdata_column_d(int col)
{
    if (col<0 || col>blockdata.ncols) return NULL;
    else
    return ((double *) blockdata.data[col]);
}

char ** get_block_data_column_s(int col)
{
    if (col<0 || col>blockdata.ncols) return NULL;
    else
    return ((char **) blockdata.data[col]);
}

int realloc_ss_data(ss_data *ssd, int nrows)
{
    int i, j;
    char  **sp;
    
    for (i = 0; i < ssd->ncols; i++) {
        if (ssd->formats[i] == FFORMAT_STRING) {
            sp = (char **) ssd->data[i];
            for (j = nrows; j < ssd->nrows; j++) {
                XCFREE(sp[j]);
            }
            ssd->data[i] = (char **)xrealloc(ssd->data[i], nrows*sizeof(char *));
            sp = (char **) ssd->data[i];
            for (j = ssd->nrows; j < nrows; j++) {
                sp[j] = NULL;
            }
        } else {
            ssd->data[i] = (double*)xrealloc(ssd->data[i], nrows*sizeof(double));
        }
    }
    ssd->nrows = nrows;
    
    return RETURN_SUCCESS;
}

void free_ss_data(ss_data *ssd)
{
    if (ssd) {
        int i, j;
        char  **sp;

        for (i = 0; i < ssd->ncols; i++) {
            if (ssd->formats && ssd->formats[i] == FFORMAT_STRING) {
                sp = (char **) ssd->data[i];
                for (j = 0; j < ssd->nrows; j++) {
                    XCFREE(sp[j]);
                }
            }
            XCFREE(ssd->data[i]);
        }
        XCFREE(ssd->data);
        XCFREE(ssd->formats);
        ssd->nrows = 0;
        ssd->ncols = 0;
    }
}

int init_ss_data(ss_data *ssd, int ncols, int *formats)
{
    int i;
    
    ssd->data = (void**)xmalloc(ncols*sizeof(void*));
    for (i = 0; i < ncols; i++) {
        ssd->data[i] = NULL;
    }
    ssd->formats = (int*)xmalloc(ncols*sizeof(int));
    memcpy(ssd->formats, formats, ncols*sizeof(int));
    ssd->ncols = ncols;
    ssd->nrows = 0;

    return RETURN_SUCCESS;
}

char *next_token(char *s, char **token, int *quoted)
{
    *quoted = FALSE;
    *token = NULL;
    
    if (s == NULL) {
        return NULL;
    }
    
    while (*s == ' ' || *s == '\t') {
        s++;
    }
    if (*s == '"') {
        s++;
        *token = s;
        while (*s != '\0' && (*s != '"' || (*s == '"' && *(s - 1) == '\\'))) {
            s++;
        }
        if (*s == '"') {
            /* successfully identified a quoted string */
            *quoted = TRUE;
        }
    } else {
        *token = s;
        if (**token == '\n') {
            /* EOL reached */
            return NULL;
        }
        while (*s != '\n' && *s != '\0' && *s != ' ' && *s != '\t') {
            s++;
        }
    }
    
    if (*s != '\0') {
        *s = '\0';
        s++;
        return s;
    } else {
        return NULL;
    }
}

int parse_ss_row(const char *s, int *nncols, int *nscols, int **formats)
{
    int ncols;
    int quoted;
    char *buf, *s1, *token;
    double value;
    Dates_format df_pref, ddummy;
    const char *sdummy;

    *nscols = 0;
    *nncols = 0;
    *formats = NULL;
    df_pref = get_date_hint();
    buf = copy_string(NULL, s);
    s1 = buf;
    while ((s1 = next_token(s1, &token, &quoted)) != NULL) {
        if (token == NULL) {
            *nscols = 0;
            *nncols = 0;
            XCFREE(*formats);
            xfree(buf);
            return RETURN_FAILURE;
        }
        
        ncols = *nncols + *nscols;
        /* reallocate the formats array */
        if (ncols % 10 == 0) {
            *formats = (int*)xrealloc(*formats, (ncols + 10)*sizeof(int));
        }

        if (quoted) {
            (*formats)[ncols] = FFORMAT_STRING;
            (*nscols)++;
        } else if (parse_date(token, df_pref, FALSE, &value, &ddummy) ==
            RETURN_SUCCESS) {
            (*formats)[ncols] = FFORMAT_DATE;
            (*nncols)++;
        } else if (parse_float(token, &value, &sdummy) == RETURN_SUCCESS) {
            (*formats)[ncols] = FFORMAT_NUMBER;
            (*nncols)++;
        } else {
            /* last resort - treat the field as string, even if not quoted */
            (*formats)[ncols] = FFORMAT_STRING;
            (*nscols)++;
        }
    }
    xfree(buf);
    
    return RETURN_SUCCESS;
}

void exchangePointComma(char * token)
{
static int i;
i=0;
while (token[i]!='\0')
{
if (token[i]=='.') token[i]=',';
else if (token[i]==',') token[i]='.';
i++;
}
}

void removePoint(char * token)
{
static int i,j;
i=0;
while (token[i]!='\0')
{
if (token[i]=='.')
{
    j=i+1;
    while (token[j]!='\0')
    {
    token[j-1]=token[j];
    j++;
    }
    token[j-1]='\0';
i--;
}
i++;
}
}

void exchangePointCommaInFormat(char * token,int type)//only exchanges '.' and ',' in certain formats
{
switch (type)
{
default:
case FORMAT_STRING:
case FORMAT_INVALID:
case FORMAT_DDMMYY:
case FORMAT_MMDDYY:
case FORMAT_YYMMDD:
case FORMAT_MMYY:
case FORMAT_MMDD:
case FORMAT_MONTHDAY:
case FORMAT_DAYMONTH:
case FORMAT_MONTHS:
case FORMAT_MONTHSY:
case FORMAT_MONTHL:
case FORMAT_DAYOFWEEKS:
case FORMAT_DAYOFWEEKL:
case FORMAT_DAYOFYEAR:
case FORMAT_HMS:
case FORMAT_MMDDHMS:
case FORMAT_MMDDYYHMS:
case FORMAT_YYMMDDHMS:
case FORMAT_YYYY:
case FORMAT_YYDYHMS:
break;
case FORMAT_DECIMAL:
case FORMAT_EXPONENTIAL:
case FORMAT_GENERAL:
case FORMAT_POWER:
case FORMAT_SCIENTIFIC:
case FORMAT_ENGINEERING:
case FORMAT_COMPUTING:
case FORMAT_DEGREESLON:
case FORMAT_DEGREESMMLON:
case FORMAT_DEGREESMMSSLON:
case FORMAT_MMSSLON:
case FORMAT_DEGREESLAT:
case FORMAT_DEGREESMMLAT:
case FORMAT_DEGREESMMSSLAT:
case FORMAT_MMSSLAT:
exchangePointComma(token);
break;
}
}

void removePointInFormat(char * token,int type)//remove '.' if needed (if ',' is decimal separator, '.' is iritating)
{
if (DecimalPointToUse=='.') return;
switch (type)
{
default:
case FORMAT_STRING:
case FORMAT_INVALID:
case FORMAT_DDMMYY:
case FORMAT_MMDDYY:
case FORMAT_YYMMDD:
case FORMAT_MMYY:
case FORMAT_MMDD:
case FORMAT_MONTHDAY:
case FORMAT_DAYMONTH:
case FORMAT_MONTHS:
case FORMAT_MONTHSY:
case FORMAT_MONTHL:
case FORMAT_DAYOFWEEKS:
case FORMAT_DAYOFWEEKL:
case FORMAT_DAYOFYEAR:
case FORMAT_HMS:
case FORMAT_MMDDHMS:
case FORMAT_MMDDYYHMS:
case FORMAT_YYMMDDHMS:
case FORMAT_YYYY:
case FORMAT_YYDYHMS:
break;
case FORMAT_DECIMAL:
case FORMAT_EXPONENTIAL:
case FORMAT_GENERAL:
case FORMAT_POWER:
case FORMAT_SCIENTIFIC:
case FORMAT_ENGINEERING:
case FORMAT_COMPUTING:
case FORMAT_DEGREESLON:
case FORMAT_DEGREESMMLON:
case FORMAT_DEGREESMMSSLON:
case FORMAT_MMSSLON:
case FORMAT_DEGREESLAT:
case FORMAT_DEGREESMMLAT:
case FORMAT_DEGREESMMSSLAT:
case FORMAT_MMSSLAT:
removePoint(token);
break;
}
}

/* NOTE: the input string will be corrupted! */
int insert_data_row(ss_data *ssd, int row, char *s)
{
    int i, j;
    int ncols = ssd->ncols;
    char *token;
    int quoted;
    char  **sp;
    double *np;
    Dates_format df_pref, ddummy;
    const char *sdummy;
    int res;
    if (exchange_point_comma)
    {
    exchangePointComma(s);
    /*j=strlen(s);
    for (i=0;i<j;i++)
    {
    if (s[i]=='.') s[i]=',';
    else if (s[i]==',') s[i]='.';
    }*/
    }
    df_pref = get_date_hint();
    for (i = 0; i < ncols; i++) {
        s = next_token(s, &token, &quoted);
        if (s == NULL || token == NULL) {
            /* invalid line: free the already allocated string fields */
            for (j = 0; j < i; j++) {
                if (ssd->formats[j] == FFORMAT_STRING) {
                    sp = (char **) ssd->data[j];
                    XCFREE(sp[row]);
                }
            }
            return RETURN_FAILURE;
        } else {
            if (ssd->formats[i] == FFORMAT_STRING) {
                sp = (char **) ssd->data[i];
                sp[row] = copy_string(NULL, token);
                if (sp[row] != NULL) {
                    res = RETURN_SUCCESS;
                } else {
                    res = RETURN_FAILURE;
                }
            } else if (ssd->formats[i] == FFORMAT_DATE) {
                np = (double *) ssd->data[i];
                res = parse_date(token, df_pref, FALSE, &np[row], &ddummy);
            } else {
                np = (double *) ssd->data[i];
                res = parse_float(token, &np[row], &sdummy);
            }
            if (res != RETURN_SUCCESS) {
                for (j = 0; j < i; j++) {
                    if (ssd->formats[j] == FFORMAT_STRING) {
                        sp = (char **) ssd->data[j];
                        XCFREE(sp[row]);
                    }
                }
                return RETURN_FAILURE;
            }
        }
    }
    
    return RETURN_SUCCESS;
}


int store_data(ss_data *ssd, int load_type, const char *label)
{
    int ncols, nncols, nncols_req, nscols, nrows;
    int i, j;
    double *xdata;
    int gno, setno;
    int x_from_index;
    
    if (ssd == NULL) {
        return RETURN_FAILURE;
    }
    ncols = ssd->ncols;
    nrows = ssd->nrows;
    if (ncols <= 0 || nrows <= 0) {
        return RETURN_FAILURE;
    }

    nncols = 0;
    for (j = 0; j < ncols; j++) {
        if (ssd->formats[j] != FFORMAT_STRING) {
            nncols++;
        }
    }
    nscols = ncols - nncols;
    
    gno = get_parser_gno();
    if (is_valid_gno(gno) != TRUE) {
        return RETURN_FAILURE;
    }
    
    switch (load_type) {
    case LOAD_SINGLE:
        if (nscols > 1) {
            errmsg("Can not use more than one column of strings per set");
            free_ss_data(ssd);
            return RETURN_FAILURE;
        }

        nncols_req = settype_cols(curtype);
        x_from_index = FALSE;
        if (nncols_req == nncols + 1) {
            x_from_index = TRUE;
        } else if (nncols_req != nncols) {
	    errmsg("Column count incorrect");
	    return RETURN_FAILURE;
        }

        new_set_no=setno = nextset(gno);
        set_dataset_type(gno, setno, curtype);

/// set_set_hidden(gno,setno,false);/// I added this

        new_set_no=1;
        if (new_set_nos!=NULL) delete[] new_set_nos;
        new_set_nos=new int[2];
        new_set_nos[0]=setno;

        nncols = 0;
        if (x_from_index) {
            xdata = allocate_index_data(nrows);
            if (xdata == NULL) {
                free_ss_data(ssd);
            }
            setcol(gno, setno, nncols, xdata, nrows);
            nncols++;
        }
        for (j = 0; j < ncols; j++) {
            if (ssd->formats[j] == FFORMAT_STRING) {
                set_set_strings(gno, setno, nrows, (char **) ssd->data[j]);
            } else {
                setcol(gno, setno, nncols, (double *) ssd->data[j], nrows);
                nncols++;
            }
        }
        if (!strlen(getcomment(gno, setno)) && projectfileloading==0) {
            setcomment(gno, setno, label);
        }
        
        XCFREE(ssd->data);
        XCFREE(ssd->formats);
        break;
    case LOAD_NXY:
        if (nscols != 0) {
            errmsg("Can not yet use strings when reading in data as NXY");
            free_ss_data(ssd);
            return RETURN_FAILURE;
        }
        
        new_set_no=ncols - 1;
        if (new_set_nos!=NULL) delete[] new_set_nos;
        new_set_nos=new int[ncols];

        for (i = 0; i < ncols - 1; i++) {
            setno = nextset(gno);
            new_set_nos[i]=setno;
            if (setno == -1) {
                free_ss_data(ssd);
                return RETURN_FAILURE;
            }
            if (i > 0) {
                xdata = copy_data_column((double *) ssd->data[0], nrows);
                if (xdata == NULL) {
                    free_ss_data(ssd);
                }
            } else {
                xdata = (double *) ssd->data[0];
            }
            set_dataset_type(gno, setno, SET_XY);
            setcol(gno, setno, DATA_X, xdata, nrows);
            setcol(gno, setno, DATA_Y, (double *) ssd->data[i + 1], nrows);
            setcomment(gno, setno, label);
            set_set_hidden(gno,setno,false);/// I added this
        }
    
        XCFREE(ssd->data);
        XCFREE(ssd->formats);
        break;
    case LOAD_BLOCK:
        set_blockdata(ssd);
        break;
    default:
        errmsg("Internal error");
        free_ss_data(ssd);
        return RETURN_FAILURE;
    }
    
    return RETURN_SUCCESS;
}

int field_string_to_cols(const char *fs, int *nc, int **cols, int *scol)
{
    int col;
    char *s, *buf;

    buf = copy_string(NULL, fs);
    if (buf == NULL) {
        return RETURN_FAILURE;
    }

    s = buf;
    *nc = 0;
    while ((s = strtok(s, ":")) != NULL) {
	(*nc)++;
	s = NULL;
    }
    *cols = (int*)xmalloc((*nc)*sizeof(int));
    if (*cols == NULL) {
        xfree(buf);
        return RETURN_FAILURE;
    }

    strcpy(buf, fs);
    s = buf;
    *nc = 0;
    *scol = -1;
    while ((s = strtok(s, ":")) != NULL) {
        int strcol;
        if (*s == '{') {
            char *s1;
            strcol = TRUE;
            s++;
            if ((s1 = strchr(s, '}')) != NULL) {
                *s1 = '\0';
            }
        } else {
            strcol = FALSE;
        }
        col = atoi(s);
        col--;
        if (strcol) {
            *scol = col;
        } else {
            (*cols)[*nc] = col;
	    (*nc)++;
        }
	s = NULL;
    }
    
    xfree(buf);
    
    return RETURN_SUCCESS;
}

char *cols_to_field_string(int nc, int *cols, int scol)
{
    int i;
    char *s, buf[32];
    
    s = NULL;
    for (i = 0; i < nc; i++) {
        sprintf(buf, "%d", cols[i] + 1);
        if (i != 0) {
            s = concat_strings(s, ":");
        }
        s = concat_strings(s, buf);
    }
    if (scol >= 0) {
        sprintf(buf, ":{%d}", scol + 1);
        s = concat_strings(s, buf);
    }
    
    return s;
}

int create_set_fromblock(int gno, int setno, int type, int nc, int *coli, int scol, int autoscale)
{
    int i, ncols, blockncols, blocklen, column;
    double *cdata;
    char *s;//buf[256],
    QString tmp_str;

    blockncols = get_blockncols();
    if (blockncols <= 0) {
        errmsg("No block data read");
        return RETURN_FAILURE;
    }

    blocklen = get_blocknrows();
    
    ncols = settype_cols(type);
    if (nc > ncols) {
        errmsg("Too many columns scanned in column string");
        return RETURN_FAILURE;
    }
    if (nc < ncols) {
	errmsg("Too few columns scanned in column string");
	return RETURN_FAILURE;
    }
    
    for (i = 0; i < nc; i++) {
	if (coli[i] < -1 || coli[i] >= blockncols) {
	    errmsg("Column index out of range");
	    return RETURN_FAILURE;
	}
    }
    
    if (scol >= blockncols) {
	errmsg("String column index out of range");
	return RETURN_FAILURE;
    }

    new_set_no = setno;

    if (setno == NEW_SET) {
        new_set_no = setno = nextset(gno);
        if (setno == -1) {
            return RETURN_FAILURE;
        }
    }
    
    /* clear data stored in the set, if any */
    killsetdata(gno, setno);
    
    if (activateset(gno, setno) != RETURN_SUCCESS) {
        return RETURN_FAILURE;
    }
    
    set_dataset_type(gno, setno, type);

    for (i = 0; i < nc; i++) {
        column = coli[i];
        if (column == -1) {
            cdata = allocate_index_data(blocklen);
        } else {
            if (blockdata.formats[column] != FFORMAT_STRING) {
                cdata = copy_data_column((double *) blockdata.data[column], blocklen);
            } else {
                errmsg("Tried to read doubles from strings!");
                killsetdata(gno, setno);
                return RETURN_FAILURE;
            }
        }
        if (cdata == NULL) {
            killsetdata(gno, setno);
            return RETURN_FAILURE;
        }
        setcol(gno, setno, i, cdata, blocklen);
    }

    /* strings, if any */
    if (scol >= 0) {
        if (blockdata.formats[scol] != FFORMAT_STRING) {
            errmsg("Tried to read strings from doubles!");
            killsetdata(gno, setno);
            return RETURN_FAILURE;
        } else {
            set_set_strings(gno, setno, blocklen,
                copy_string_column((char **) blockdata.data[scol], blocklen));
        }
    }

    s = cols_to_field_string(nc, coli, scol);
    //sprintf(buf, "Cols %s", s);
    tmp_str=QObject::tr("Cols ")+QString(s);
    xfree(s);
    //setcomment(gno, setno, buf);
    setcomment(gno, setno, tmp_str.toLocal8Bit().constData());
    autoscale_graph(gno, autoscale);
    return RETURN_SUCCESS;
}
