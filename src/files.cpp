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
 * read data files
 *
 */

///#include <config.h>

#include <cstdio>
#include <cstdlib>

#ifdef _MSC_VER
#include <WinSock2.h>
// fd_set
#include <time.h>
// _tzset
#include <io.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif

#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>

//#include <sys/select.h>
#include <cerrno>

#ifdef WINDOWS_SYSTEM
#include <winsock.h>
#include <winsock2.h>
#define O_NONBLOCK 0x0004
#endif

#ifdef HAVE_SYS_SELECT_H
#  include <sys/select.h>
#endif
///#ifdef HAVE_FCNTL_H
//#ifndef WINDOWS_SYSTEM
#include <fcntl.h>
//#endif

#ifdef HAVE_NETCDF
#  include <netcdf.h>
#endif

#include "globals.h"
#include "utils.h"
#include "files.h"
#include "ssdata.h"
#include "graphs.h"
#include "graphutils.h"
#include "parser.h"

#include "noxprotos.h"
#include "xprotos.h"
#include <iostream>
#include <QtCore>
#include "MainWindow.h"
#include "allWidgets.h"

using namespace std;

#ifdef __EMX__
char *exe_path_translate(char *path);
#else
#  define exe_path_translate(p) (p)
#endif

#define MAXERR 5

/*
 * number of rows to allocate for each call to realloc
 */
#define BUFSIZE  512

/*
 * number of bytes in each line chunk
 * (should be related to system pipe size, typically 4K)
 */
#ifndef PIPE_BUF
#  define PIPE_BUF 4096
#endif
#define CHUNKSIZE 2*PIPE_BUF

extern char user_home_dir[1024];
extern char qt_grace_exe_dir[1024];
extern char default_grace_file[];
extern MainWindow * mainWin;
extern frmDeviceSetup * FormDeviceSetup;
extern int new_set_no;
extern int * new_set_nos;
extern int ReqUpdateColorSel;
extern graph * g;
extern QList<QFont> stdFontList;
extern Device_entry *device_table;

extern int n_linestyles_ini,n_linestyles_tmp,n_linestyles_file;
extern int *l_linestyle_pat_ini,*l_linestyle_pat_tmp,*l_linestyle_pat_file;
extern char **linestyle_pat_ini,**linestyle_pat_tmp,**linestyle_pat_file;

extern void reset_stdFontList(bool clear_only);
extern char new_encoding_name[128];
extern bool qtfonts_have_been_reset;
extern bool useQtFonts;
extern void post_load_font_status(void);
extern void update_fonts_to_default_ordering(void);
extern int nr_of_current_linestyles;
extern void generate_strings_for_linestyles(QString & pat_len,QString & patterns);

extern void readBinaryFromFile(ifstream & ifi,importSettings & imp_set,double *** data);
bool exchange_point_comma=false;//exchange ',' for '.' as decimal-seperator

extern void save_default_states(void);
extern void reset_default_states(void);
extern void update_default_props(void);
extern void append_to_storage(int * len,int ** storage,int n,int * new_entries);
extern void update_font_selectors(bool appearance);
extern void prepare_imp_settings_for_header_import(struct importSettings & imp_set);
extern void complete_channel_settings(struct importSettings & imp_set);
extern int bin_format_size(int nr);
extern QString get_text_in_quotations(QString text);
extern QString get_filename_with_extension(int device);
extern void generate_string_Qt_aware(char * string,QString text);
extern void complete_LaTeX_to_Grace_Translator(QString & text);
extern void copy_line_style_patterns_to_target(int n_length,int * n_style_lengths,char ** n_patterns,int target);
extern void copy_line_style_patterns_to_current(int n_length,int * n_style_lengths,char ** n_patterns);
extern void parse_strings_for_linestyles(int len,QString pat_len,QString patterns, int ** lens_of_styles,char *** line_styles);
extern void update_line_style_selectors(void);

extern "C" char *close_input;
char *close_input;		/* name of real-time input to close */

struct timeval read_begin = {0l, 0l};	/* used to check too long inputs */

static Input_buffer dummy_ib = {-1, 0, 0, 0, 0, NULL, 0, 0, NULL, 0l};

int nb_rt = 0;		        /* number of real time file descriptors */
Input_buffer *ib_tbl = 0;	/* table for each open input */
int ib_tblsize = 0;		/* number of elements in ib_tbl */

extern defaults d_d;
extern defaults file_defaults;
extern defaults ini_defaults;
extern defaults tmp_defaults;

extern view d_v;
extern view file_view;
extern view ini_view;
extern view tmp_view;

static int time_spent(void);
static int expand_ib_tbl(void);
static int expand_line_buffer(char **adrBuf, int *ptrSize, char **adrPtr);
static int reopen_real_time_input(Input_buffer *ib);
static int read_real_time_lines(Input_buffer *ib);
static int process_complete_lines(Input_buffer *ib);

static int read_long_line(FILE *fp, char **linebuf, int *buflen);

static int uniread(FILE *fp, int load_type, char *label);

void postprocess_diadem_header(struct DIAdem_Header & header,struct importSettings & bin_import,int & nr_of_new_sets,int ** n_gnos,int ** n_snos);
int postprocess_bin_import_data(struct importSettings & imp_set,int & nr_of_new_sets,int ** n_gnos,int ** n_snos);

extern QTextCodec * FileCodec;
extern void update_timestamp(void);
#ifdef __cplusplus
extern "C" {
#endif
extern void prepare_strings_for_saving(void);
extern void resume_strings_after_load_or_save(void);
#ifdef __cplusplus
}
#endif

bool is_windows_path(char * path)//returns true if the path seems to be an absolute windows path like c:\abc
{
bool ret=false;
int len=strlen(path);
int count_separators=0;
    for (int i=0;i<len;i++)
    {
        if (path[i]=='\\' || path[i]=='/')
        {
        count_separators++;
        }
        else
        {
            if (path[i]==':' && count_separators==0)
            {
            ret=true;
            break;
            }
        }
    }
return ret;
}

bool is_path_absolute(char * path)//returns true if path is an absolute path
{
return QDir::isAbsolutePath(QString(path));
}

inline QString getFileNameOnly(QString filepath)//filename including suffix
{
QFileInfo fi(filepath);
return fi.fileName();
}

inline QString getFileBaseNameOnly(QString filepath)//filename without suffix
{
QFileInfo fi(filepath);
return fi.baseName();
}

inline QString getPathOnly(QString filepath)
{
QFileInfo fi(filepath);
return fi.absolutePath();
}

inline QString getSuffixOnly(QString filepath)
{
QFileInfo fi(filepath);
return fi.completeSuffix();
}

void save_qtGrace_Additions(FILE * pp)
{
    int i,j;
    boxtype b;
    ellipsetype e;
    for (i = 0; i < number_of_boxes(); i++)
    {
        get_graph_box(i, &b);
        if (b.active == TRUE)
        {
            fprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: BOX %d ROT %d\n",i,b.rot);
        }
    }
    for (i = 0; i < number_of_ellipses(); i++)
    {
        get_graph_ellipse(i, &e);
        if (e.active == TRUE)
        {
            fprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: ELLIPSE %d ROT %d\n",i,e.rot);
        }
    }
    for (i=0;i<number_of_graphs();i++)
    {
        for (j=0;j<number_of_sets(i);j++)
        {
            fprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d POLYGONEBASESET %d\n",i,j,g[i].p[j].polygone_base_set);
        }
    fprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: AUTOATTACH G %d %d %d\n",i,g[i].l.autoattach,g[i].l.autoattachG);
    }
    if (stdFontList.length()<1)
        fprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: QTFONT_RESET 0\n");//we want the standard-fonts for the Qt-Fonts
    else
        fprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: QTFONT_RESET 1\n");//we ask for a cleared font-list to insert new fonts
    for (i=0;i<stdFontList.length();i++)
    {
        fprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: QTFONT \"%s\"\n",stdFontList.at(i).toString().toLocal8Bit().constData());
    }
    fprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: ENCODING \"%s\"\n",FileCodec->name().constData());
    fprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: UNIVERSAL_FONT_SIZE_FACTOR %.4f\n",universal_font_size_factor);
    fprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: TIMESTAMP_PATH %d\n",timestamp.path);
    QString pat_lens,pat_styles;
    generate_strings_for_linestyles(pat_lens,pat_styles);
    fprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: LINESTYLES %d %s %s\n",nr_of_current_linestyles,pat_lens.toLatin1().constData(),pat_styles.toLatin1().constData());
}

void parse_qtGrace_Additions(char * s)
{
    int * lens_of_styles;
    char ** line_styles;
    QString style_lens,style_pats;
    char c='\0';
    char c2='\0';
    int read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: %c%c",&c,&c2);
    int data[3];
    char sdata[128];
    char sdata2[128];
    double ddata;
    if (read_data>0)
    {
        //cout << "s=#" << s << "# --> read_data=" << read_data << " c=" << c << endl;
        switch (c)
        {
        case 'A':
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: AUTOATTACH G %d %d %d\n",data,data+1,data+2);
            if (read_data>0 && is_valid_gno(data[0]))
            {
                g[data[0]].l.autoattach=data[1];
                g[data[0]].l.autoattachG=data[2];
            }
            break;
        case 'B':
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: BOX %d ROT %d",data,data+1);
            if (read_data>0)
            {
                if (is_valid_box(data[0]))
                    boxes[data[0]].rot=data[1];
            }
            break;
        case 'E':
            if (c2=='L')
            {
                read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: ELLIPSE %d ROT %d",data,data+1);
                if (read_data>0)
                {
                    if (is_valid_ellipse(data[0]))
                        ellip[data[0]].rot=data[1];
                }
            }
            else//'N' --> Encoding
            {
                read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: ENCODING \"%s\"",sdata);
                if (sdata[strlen(sdata)-1]=='"') sdata[strlen(sdata)-1]='\0';
                //cout << "Encoding=#" << sdata << "#" << endl;
                strcpy(new_encoding_name,sdata);
                /// Todo: sdata verarbeiten ...
                FileCodec=QTextCodec::codecForName(sdata);
            }
            break;
        case 'G':
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d POLYGONEBASESET %d",data,data+1,data+2);
            if (read_data>0)
            {
                if (is_valid_setno(data[0],data[1]))
                    g[data[0]].p[data[1]].polygone_base_set=data[2];
            }
            break;
        case 'Q':
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: QTFONT_RESET %d",data);
            if (read_data>0)
            {
                if (data==0)//QTFONT_RESET 0 --> Reset fonts and insert the current Grace-fonts
                {
                    reset_stdFontList(false);
                }
                else//QTFONT_RESET 1 --> Clear the Qt-Fonts and insert all standard-fonts in standard order
                {
                    reset_stdFontList(true);
                }
                qtfonts_have_been_reset=true;
            }
            else
            {
                read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: QTFONT \"%s\"",sdata);
                if (read_data>0)//we want to insert a new Qt-Font
                {
                    if (qtfonts_have_been_reset==false)
                    {
                        reset_stdFontList(false);
                        qtfonts_have_been_reset=true;
                    }
                    QFont stfont;
                    stfont.fromString(QString(sdata));
                    (void)addFontToDatabase(stfont);
                }
            }
            break;
        case 'U':
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: UNIVERSAL_FONT_SIZE_FACTOR %lf",&ddata);
            if (read_data>0)
            {
            universal_font_size_factor=ddata;
            }
            break;
        case 'T':
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: TIMESTAMP_PATH %d",data);
            if (read_data>0)
            {
            timestamp.path=data[0];
            }
            break;
        case 'L':
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: LINESTYLES %d %s %s",data,sdata,sdata2);
            if (read_data>0)
            {
            //cout << "linestyles=" << data[0] << " slen=" << sdata << " patterns=" << sdata2 << endl;
            style_lens=QString(sdata);
            style_pats=QString(sdata2);
            parse_strings_for_linestyles(data[0],style_lens,style_pats,&lens_of_styles,&line_styles);
            copy_line_style_patterns_to_target(data[0],lens_of_styles,line_styles,2);//copy linestyle to file-settings
            //use the styles from the file as current styles and generate icons and dash-patterns
            copy_line_style_patterns_to_current(data[0],lens_of_styles,line_styles);
            }
            break;
        }
    }//end read_data>0
}

#ifdef _MSC_VER

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

struct timezone
{
    int  tz_minuteswest; /* minutes W of Greenwich */
    int  tz_dsttime;     /* type of dst correction */
};

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    FILETIME ft;
    unsigned __int64 tmpres = 0;
    static int tzflag;

    if (NULL != tv)
    {
        GetSystemTimeAsFileTime(&ft);

        tmpres |= ft.dwHighDateTime;
        tmpres <<= 32;
        tmpres |= ft.dwLowDateTime;

        /*converting file time to unix epoch*/
        tmpres -= DELTA_EPOCH_IN_MICROSECS;
        tmpres /= 10;  /*convert into microseconds*/
        tv->tv_sec = (long)(tmpres / 1000000UL);
        tv->tv_usec = (long)(tmpres % 1000000UL);
    }

    if (NULL != tz)
    {
        if (!tzflag)
        {
            _tzset();
            tzflag++;
        }
        tz->tz_minuteswest = _timezone / 60;
        tz->tz_dsttime = _daylight;
    }

    return 0;
}
#endif

/*
 * part of the time sliced already spent in milliseconds
 */
static int time_spent(void)
{
    struct timeval now;

    gettimeofday(&now, NULL);

    return 1000 * (now.tv_sec - read_begin.tv_sec)
            + (now.tv_usec - read_begin.tv_usec) / 1000;
}

/*
 * expand the table of monitored real time inputs
 */
static int expand_ib_tbl(void)
{
    int i, new_size;
    Input_buffer *new_tbl;

    new_size = (ib_tblsize > 0) ? 2*ib_tblsize : 5;
    new_tbl  = (Input_buffer*)xcalloc(new_size, sizeof(Input_buffer));
    if (new_tbl == NULL) {
        return RETURN_FAILURE;
    }

    for (i = 0; i < new_size; i++) {
        new_tbl[i] = (i < ib_tblsize) ? ib_tbl[i] : dummy_ib;
    }

    if (ib_tblsize > 0) {
        xfree((void *) ib_tbl);
    }
    ib_tbl  = new_tbl;
    ib_tblsize = new_size;

    return RETURN_SUCCESS;

}

/*
 * expand a line buffer
 */
static int expand_line_buffer(char **adrBuf, int *ptrSize, char **adrPtr)
{
    char *newbuf;
    int   newsize;

    newsize = *ptrSize + CHUNKSIZE;
    newbuf = (char*)xmalloc(newsize);
    if (newbuf == 0) {
        return RETURN_FAILURE;
    }

    if (*ptrSize == 0) {
        /* this is the first time through */
        if (adrPtr) {
            *adrPtr = newbuf;
        }
    } else {
        /* we are expanding an existing line */
        strncpy(newbuf, *adrBuf, *ptrSize);
        if (adrPtr) {
            *adrPtr += newbuf - *adrBuf;
        }
        xfree(*adrBuf);
    }

    *adrBuf  = newbuf;
    *ptrSize = newsize;

    return RETURN_SUCCESS;
}

/*
 * reopen an Input_buffer (surely a fifo)
 */
static int reopen_real_time_input(Input_buffer *ib)
{
    int fd;
    char buf[256];

    /* in order to avoid race conditions (broken pipe on the write
       side), we open a new file descriptor before closing the
       existing one */
    fd = open(ib->name, O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        sprintf(buf, "Can't reopen real time input %s", ib->name);
        errmsg(buf);
        unregister_real_time_input(ib->name);
        return RETURN_FAILURE;
    }

#ifndef NONE_GUI
    ///    xunregister_rti((XtInputId) ib->id);
#endif

    /* swapping the file descriptors */
    close(ib->fd);
    ib->fd = fd;

#ifndef NONE_GUI
    ///    xregister_rti(ib);
#endif

    return RETURN_SUCCESS;

}

/*
 * unregister a file descriptor no longer monitored
 * (since Input_buffer structures dedicated to static inputs
 *  are not kept in the table, it is not an error to unregister
 *  an input not already registered)
 */
void unregister_real_time_input(const char *name)
{
    Input_buffer *ib;
    signed int           l1, l2;

    l1 = strlen(name);

    nb_rt = 0;
    for (ib = ib_tbl; ib < ib_tbl + ib_tblsize; ib++) {
        l2 = (ib->name == NULL) ? -1 : strlen(ib->name);
        if (l1 == l2 && strcmp (name, ib->name) == 0) {
            /* name is usually the same pointer as ib->name so we cannot */
            /* free the string and output the message using name afterwards */
#ifndef NONE_GUI
            ///           xunregister_rti((XtInputId) ib->id);
#endif
            close(ib->fd);
            ib->fd = -1;
            xfree(ib->name);
            ib->name = NULL;
        } else
            if (l2 > 0) {
                /* this descriptor (if not dummy!) is still in use */
                nb_rt++;
            }
    }
}

/*
 * register a file descriptor for monitoring
 */
int register_real_time_input(int fd, const char *name, int reopen)
{
    Input_buffer *ib;
    char buf[256];

    /* some safety checks */
    if (fd < 0)
    {
        sprintf(buf, "%s : internal error, wrong file descriptor", name);
        errmsg(buf);
        return RETURN_FAILURE;
    }

    ///#ifdef HAVE_FCNTL
#ifndef WINDOWS_SYSTEM
    if (fcntl(fd, F_GETFL) & O_WRONLY)
    {
        fprintf(stderr,"Descriptor %d not open for reading\n",fd);
        return RETURN_FAILURE;
    }
#endif

    /* remove previous entry for the same set if any */
    unregister_real_time_input(name);

    /* find an empty slot in the table */
    for (ib = ib_tbl; ib < ib_tbl + ib_tblsize; ib++)
    {
        if (ib->fd == fd)
        {
            sprintf(buf, "%s : internal error, file descriptor already in use",name);
            errmsg(buf);
            return RETURN_FAILURE;
        }
        else if (ib->fd < 0)
        {
            break;
        }
    }

    if (ib == ib_tbl + ib_tblsize)
    {
        /* the table was full, we expand it */
        int old_size = ib_tblsize;
        if (expand_ib_tbl() != RETURN_SUCCESS)
        {
            return RETURN_FAILURE;
        }
        ib = ib_tbl + old_size;
    }

    /* we keep the current buffer (even if 0),
       and only say everything is available */
    ib->fd     = fd;
    ib->errors = 0;
    ib->lineno = 0;
    ib->zeros  = 0;
    ib->reopen = reopen;
    ib->name   = copy_string(ib->name, name);
    ib->used   = 0;
#ifndef NONE_GUI
    ///    xregister_rti (ib);
#endif

    nb_rt++;

    return RETURN_SUCCESS;
}

/*
 * read a real-time line (but do not process it)
 */
static int read_real_time_lines(Input_buffer *ib)
{
    char *cursor;
    int   available, nbread;
    char buf[256];

    cursor     = ib->buf  + ib->used;
    available  = ib->size - ib->used;

    /* have we enough space to store the characters ? */
    if (available < 2) {
        if (expand_line_buffer(&(ib->buf), &(ib->size), &cursor)
                != RETURN_SUCCESS) {
            return RETURN_FAILURE;
        }
        available = ib->buf + ib->size - cursor;
    }

    /* read as much as possible */
    nbread = read(ib->fd, (void *) cursor, available - 1);

    if (nbread < 0) {
        sprintf(buf, "%s : read error on real time input", ib->name);
        errmsg(buf);
        return RETURN_FAILURE;
    } else {
        if (nbread == 0) {
            ib->zeros++;
        } else {
            ib->zeros = 0;
            ib->used += nbread;
            ib->buf[ib->used] = '\0';
        }
    }

    return RETURN_SUCCESS;
}

/*
 * process complete lines that have already been read
 */
static int process_complete_lines(Input_buffer *ib)
{
    int line_corrupted;
    char *begin_of_line, *end_of_line;
    char buf[256];

    if (ib->used <= 0) {
        return RETURN_SUCCESS;
    }

    end_of_line = NULL;
    do {
        /* loop over the embedded lines */
        begin_of_line  = (end_of_line == NULL) ? ib->buf : (end_of_line + 1);
        end_of_line    = begin_of_line;
        line_corrupted = 0;
        while (end_of_line != NULL && *end_of_line != '\n') {
            /* trying to find a complete line */
            if (end_of_line == ib->buf + ib->used) {
                end_of_line = NULL;
            } else {
                if (*end_of_line == '\0') {
                    line_corrupted = 1;
                }
                ++end_of_line;
            }
        }

        if (end_of_line != NULL) {
            /* we have a whole line */

            ++(ib->lineno);
            *end_of_line = '\0';
            close_input = NULL;

            if (line_corrupted || scanner(begin_of_line)) {
                sprintf(buf, "Error at line %d", ib->lineno);
                errmsg(buf);
                ++(ib->errors);
                if (ib->errors > MAXERR) {

#ifndef NONE_GUI
                    /* this prevents from being called recursively by
                       the inner X loop of yesno */
                    ///                    xunregister_rti((XtInputId) ib->id);
#endif
                    if (yesno("Lots of errors, abort?", NULL, NULL, NULL)) {
                        close_input = copy_string(close_input, "");
                    }
#ifndef NONE_GUI
                    ///                    xregister_rti(ib);
#endif
                    ib->errors = 0;

                }
            }

            if (close_input != NULL) {
                /* something should be closed */
                if (close_input[0] == '\0') {
                    unregister_real_time_input(ib->name);
                } else {
                    unregister_real_time_input(close_input);
                }

                xfree(close_input);
                close_input = NULL;

                if (ib->fd < 0) {
                    /* we have closed ourselves */
                    return RETURN_SUCCESS;
                }

            }

        }

    } while (end_of_line != NULL);

    if (end_of_line != NULL) {
        /* the line has just been processed */
        begin_of_line = end_of_line + 1;
    }

    if (begin_of_line > ib->buf) {
        /* move the remaining data to the beginning */
        ib->used -= begin_of_line - ib->buf;
        memmove(ib->buf, begin_of_line, ib->used);
        ib->buf[ib->used] = '\0';

    }

    return RETURN_SUCCESS;
}

int real_time_under_monitoring(void)
{
    return nb_rt > 0;
}

/*
 * monitor the set of registered file descriptors for pending input
 */
int monitor_input(Input_buffer *tbl, int tblsize, int no_wait)
{

    Input_buffer *ib;
    fd_set rfds;
    int remaining;
    struct timeval timeout;
    int highest, first_time, retsel;

    /* we don't want to get stuck here, we memorize the start date
       and will check we do not exceed our allowed time slice */
    gettimeofday(&read_begin, NULL);
    first_time    = 1;
    retsel        = 1;
    while (((time_spent() < timer_delay) || first_time) && retsel > 0) {

        /* register all the monitored descriptors */
        highest = -1;
        FD_ZERO(&rfds);
        for (ib = tbl; ib < tbl + tblsize; ib++) {
            if (ib->fd >= 0) {
                FD_SET(ib->fd, &rfds);
                if (ib->fd > highest) {
                    highest = ib->fd;
                }
            }
        }

        if (highest < 0) {
            /* there's nothing to do */
            return RETURN_SUCCESS;
        }

        if (no_wait) {
            /* just check for available data without waiting */
            remaining = 0;
        } else {
            /* wait until data or end of time slice arrive */
            remaining = timer_delay - time_spent();
            if (remaining < 0) {
                remaining = 0;
            }
        }
        timeout.tv_sec = remaining / 1000;
        timeout.tv_usec = 1000l * (remaining % 1000);
#ifndef WINDOWS_SYSTEM
        retsel = select(highest + 1, &rfds, NULL, NULL, &timeout);
#else
        cout << "select() not supported in Windows" << endl;
        retsel=1;
#endif
        for (ib = tbl;
             ((time_spent() < timer_delay) || first_time) && ib < tbl + tblsize;
             ib++) {
#ifndef WINDOWS_SYSTEM
            if (ib->fd >= 0 && FD_ISSET(ib->fd, &rfds))
#else
            if (ib->fd >= 0)
#endif
            {
                /* there is pending input */
                if (read_real_time_lines(ib) != RETURN_SUCCESS
                        || process_complete_lines(ib) != RETURN_SUCCESS) {
                    return RETURN_FAILURE;
                }

                if (ib->zeros >= 5) {
                    /* we were told five times something happened, but
                       never got any byte : we assume the pipe (or
                       whatever) has been closed by the peer */
                    if (ib->reopen) {
                        /* we should reset the input buffer, in case
                           the peer also reopens it */
                        if (reopen_real_time_input(ib) != RETURN_SUCCESS) {
                            return RETURN_FAILURE;
                        }
                    } else {
                        unregister_real_time_input(ib->name);
                    }

                    /* we have changed the table, we should end the loop */
                    break;
                }
            }
        }

        /* after one pass, we obey timeout */
        first_time = 0;
    }

    return RETURN_SUCCESS;
}

/* replacement for fgets() to fix up reading DOS text files */
char *grace_fgets(char *s, int size, FILE *stream) {
    int  slen;
    char *endptr;

    s = fgets(s, size, stream);
    if (!s) {
        return NULL;
    }

    slen = strlen(s);
    if (slen >= 2) {
        endptr = s + slen - 2;
        /* check for DOS ending "\r\n" */
        if (*endptr == '\r') {
            /* 'move' un*x string tail "\n\0" one char forward */
            *endptr     = '\n';
            *(endptr+1) = '\0';
        }
    }
    return s;
}

/*
 * read a line increasing buffer as necessary
 */
static int read_long_line(FILE * fp, char **linebuf, int *buflen)
{
    char *cursor;
    int  available;
    int  nbread, retval;

    cursor    = *linebuf;
    available = *buflen;
    retval    = RETURN_FAILURE;
    do {
        /* do we have enough space to store the characters ? */
        if (available < 2) {
            if (expand_line_buffer(linebuf, buflen, &cursor)
                    != RETURN_SUCCESS) {
                return RETURN_FAILURE;
            }
        }
        available = (int)(*linebuf-cursor) + *buflen;

        /* read as much as possible */
        if (grace_fgets(cursor, available, fp) == NULL) {
            return retval;
        }
        nbread = strlen(cursor);
        if (nbread < 1) {
            return retval;
        } else {
            retval = RETURN_SUCCESS;
        }

        /* prepare next read */
        cursor    += nbread;
        available -= nbread;

    } while (*(cursor - 1) != '\n');

    return retval;
}

#ifndef S_ISREG
#define S_ISREG(m)     (((m) & S_IFMT) == S_IFREG)
#endif

/* open a file for write */
FILE *grace_openw(char *fn)
{
    struct stat statb;
    char buf[GR_MAXPATHLEN + 50];
    FILE *retval;

    if (!fn || !fn[0]) {
        errmsg("No file name given");
        return NULL;
    } else if (strcmp(fn, "-") == 0 || strcmp(fn, "stdout") == 0) {
        return stdout;
    } else {
        if (stat(fn, &statb) == 0) {
            /* check to make sure this is a file and not a dir */
            if (S_ISREG(statb.st_mode)) {
                sprintf(buf, "%s %s?",QObject::tr("Overwrite").toLocal8Bit().constData(), fn);
                if (!yesno(buf, NULL, NULL, NULL)) {
                    return NULL;
                }
            } else {
                sprintf(buf, "%s is not a regular file!", fn);
                errmsg(buf);
                return NULL;
            }
        }
        retval = filter_write(fn);
        if (!retval) {
            sprintf(buf, "Can't write to file %s, check permissions!", fn);
            errmsg(buf);
        }
        return retval;
    }
}

char *grace_path(char *fn)
{
    static char buf[GR_MAXPATHLEN];
    struct stat statb;

    if (fn == NULL) {
        return NULL;
    } else {
        strcpy(buf, fn);

        if(strlen(fn)>2)
        {
            if (is_windows_path(fn)==true)
            return fn;
            /*if(fn[1]==':')
            {
            return fn; //  This is a Windows full path like "c:\abc"
            }*/
        }

        switch (fn[0]) {
        case '/':
        case '\0':
            return buf;
            break;
        case '~':
            expand_tilde(buf);
            return buf;
            break;
        case '.':
            switch (fn[1]) {
            case '/':
                return buf;
                break;
            case '.':
                if (fn[2] == '/') {
                    return buf;
                }
                break;
            }
        }
        /* if we arrived here, the path is relative */

        ///dirty work
        ///sprintf(buf,"%s/../%s",qt_grace_exe_dir,fn);

        // The original trick was sprintf(buf,"%s/../%s",qt_grace_exe_dir,fn);
        // The trick does not work because "stat" does not accept "/../" on Windows
        QDir dir_above_bin(qt_grace_exe_dir);  // XXXqtgrace/bin
        dir_above_bin.cdUp();  // becomes XXXqtgrace/
/*#ifdef MAC_SYSTEM
        dir_above_bin.cdUp();
        dir_above_bin.cdUp();
        dir_above_bin.cdUp();
#endif*/
        QString qs_dir_above_bin=dir_above_bin.path();
        qs_dir_above_bin+="/";
        qs_dir_above_bin+=fn; // Note that "fn" can be  a directory or file or a/b
        // becomes  XXXqtgrace/a/b
        strcpy(buf, qs_dir_above_bin.toLocal8Bit());

        if (stat(buf, &statb) == 0) {
            /* ok, we found it */
            return buf;
        }
        
        /* second try: in .grace/ in the current dir */
        strcpy(buf, ".grace/");
        strcat(buf, fn);
        if (stat(buf, &statb) == 0) {
            return buf;
        }
        
        /* third try: in .grace/ in the $HOME dir */
        strcpy(buf, get_userhome());
        strcat(buf, ".grace/");
        strcat(buf, fn);
        if (stat(buf, &statb) == 0) {
            return buf;
        }

        /* the last attempt: in $GRACE_HOME */
        strcpy(buf, get_grace_home());
        strcat(buf, "/");
        strcat(buf, fn);
        if (stat(buf, &statb) == 0) {
            return buf;
        }
        
        /* giving up... */
        strcpy(buf, fn);
        return buf;
    }
}

char *grace_exe_path(char *fn)
{
    static char buf[GR_MAXPATHLEN];
    char *cp;
    
    if (fn == NULL) {
        return NULL;
    } else {
        cp = strchr(fn, ' ');
        if (cp == NULL) {
            ///return grace_path(fn);
            return exe_path_translate(grace_path(fn));
        } else {
            strcpy(buf, fn);
            buf[cp - fn] = '\0';
            strcpy(buf, grace_path(buf));
            strcat(buf, " ");
            strcat(buf, cp);
            ///return buf;
            return exe_path_translate(buf);
        }
    }
}

#ifdef _MSC_VER
#define popen _popen
#define pclose _pclose
#endif

/* open a file for read */
FILE *grace_openr(char *fn2, int src)
{
    struct stat statb;
    char *tfn;
    static char buf[GR_MAXPATHLEN + 50];
    static char fn[GR_MAXPATHLEN + 50];
    //Qt-stuff for checking file existence
    //QString realFileName=FileCodec->toUnicode(fn2);
    //QFile * file1=new QFile(realFileName);
    QFile * file1=new QFile(QString(fn2));
    bool exists=file1->exists();
    if (exists==true)
    {
        strcpy(fn,fn2);
    }
    else
    {
        strcpy(buf,file1->fileName().toLocal8Bit());
        QFile * file2;
        if (strcmp(buf,"gracerc.user")==0)
        {//first: search in dir of executable
            file2=new QFile(QString(qt_grace_exe_dir)+QDir::separator()+QString(buf));//realFileName);//QString(buf));
            exists=file2->exists();
            if (exists==false)
            {//if not found: search in users home dir
                delete file2;
                file2=new QFile(QString(user_home_dir)+QDir::separator()+QString(buf));//realFileName);//QString(buf));
                exists=file2->exists();
                if (exists==true)
                {
                    sprintf(fn,"%s/%s",user_home_dir,buf);
                }
            }
            else//exists==true
            {//found in dir of executable
                sprintf(fn,"%s/%s",qt_grace_exe_dir,buf);
            }
        }
        else
        {
            file2=new QFile(QString(qt_grace_exe_dir)+QDir::separator()+QString(fn2));
            exists=file2->exists();
            if (exists==true)
            {
                sprintf(fn,"%s/%s",qt_grace_exe_dir,fn2);
            }
        }
        if (exists==false)//nothing found, use original file name and tell users that this did not work
        {
            strcpy(fn,fn2);
        }
        delete file2;
    }
    delete file1;

    if (!fn || !fn[0]) {
        errmsg("No file name given");
        return NULL;
    }
    switch (src) {
    case SOURCE_DISK:
        tfn = grace_path(fn);
        if (strcmp(tfn, "-") == 0 || strcmp(tfn, "stdin") == 0) {
            return stdin;
        } else if (stat(tfn, &statb)) {
            sprintf(buf, "Can't stat file %s", tfn);
            errmsg(buf);
            return NULL;
            /* check to make sure this is a file and not a dir */
        } else if (!S_ISREG(statb.st_mode)) {
            sprintf(buf, "%s is not a regular file", tfn);
            errmsg(buf);
            return NULL;
        } else {
            return filter_read(tfn);
            ///return 0;
        }
        break;
    case SOURCE_PIPE:
        tfn = grace_exe_path(fn);
        return popen(tfn, "r");
        break;
    default:
        errmsg("Wrong call to grace_openr()");
        return NULL;
    }
}

/*
 * close either a pipe or a file pointer
 *
 */
void grace_close(FILE *fp)
{
    if (fp == stdin || fp == stderr || fp == stdout) {
        return;
    }
    if (pclose(fp) == -1) {
        fclose(fp);
    }
}

int getparms(char *plfile)
{
    int linecount = 0, errcnt = 0;
    char *linebuf=NULL;
    int linelen=0;
    FILE *pp=grace_openr(plfile, SOURCE_DISK);

    if (pp==NULL)
    {
        char dummy[1024];
        sprintf(dummy,"%s/%s",qt_grace_exe_dir,plfile);
        pp=grace_openr(dummy, SOURCE_DISK);
    }

    if (pp== NULL)
    {
        return 0;
    }
    else
    {
        errcnt = 0;
        while (read_long_line(pp, &linebuf, &linelen) == RETURN_SUCCESS)
        {
            linecount++;
            if (scanner(linebuf)) {
                sprintf(linebuf, "Error at line %d", linecount);
                errmsg(linebuf);
                errcnt++;
                if (errcnt > MAXERR) {
                    if (yesno("Lots of errors, abort?", NULL, NULL, NULL))
                    {
                        grace_close(pp);
                        xfree(linebuf);
                        return 0;
                    }
                    else
                    {
                        errcnt = 0;
                    }
                }
            }
        }
        if (pp != stdin)
        {
            grace_close(pp);
        }
    }
    xfree(linebuf);
    return 1;
}

static int uniread(FILE *fp, int load_type, char *label)
{
    int nrows, ncols, nncols, nscols, nncols_req;
    int *formats = NULL;
    int breakon, readerror;
    ss_data ssd;
    char *s, tbuf[128];
    char *linebuf=NULL;
    int linelen=0;   /* a misleading name ... */
    int linecount;

    //we have to reserve some memory here for input-data
    int * maj_new_nrs=NULL;
    int nr_count=0;

    linecount = 0;
    readerror = 0;
    nrows = 0;
    
    breakon = TRUE;
    
    memset(&ssd, 0, sizeof(ssd));

    while (read_long_line(fp, &linebuf, &linelen) == RETURN_SUCCESS)
    {
        linecount++;
        s = linebuf;
        while (*s == ' ' || *s == '\t' || *s == '\n')
        {
            s++;
        }
        /* skip comments */
        if (*s == '#') {
            parse_qtGrace_Additions(s);
            continue;
        }
        if (exchange_point_comma && (*s==',' || *s=='.'))
        {
            if (*s=='.') *s=',';
            else *s='.';
        }
        /*   command     end-of-set      EOL   */
        if (*s == '@' || *s == '&' || *s == '\0')
        {
            /* a data break line */
            if (breakon != TRUE)
            {
                /* free excessive storage */
                realloc_ss_data(&ssd, nrows);

                new_set_no=0;
                if (new_set_nos!=NULL)
                {
                    delete[] new_set_nos;
                    new_set_nos=NULL;
                }

                /* store accumulated data in set(s) */
                if (store_data(&ssd, load_type, label) != RETURN_SUCCESS)
                {
                    xfree(linebuf);
                    return RETURN_FAILURE;
                }
                
                append_to_storage(&nr_count,&maj_new_nrs,new_set_no,new_set_nos);

                /* reset state registers */
                nrows = 0;
                readerror = 0;
                breakon = TRUE;
            }
            if (*s == '@')
            {
                /*int retval = */scanner(s + 1);
                /*cout << "s=" << s << " | " << retval << endl;*/
                continue;
            }
        }
        else
        {
            if (breakon)
            {
                /* parse the data line */
                XCFREE(formats);
                if (parse_ss_row(s, &nncols, &nscols, &formats) != RETURN_SUCCESS)
                {
                    errmsg("Can't parse data");
                    xfree(linebuf);
                    return RETURN_FAILURE;
                }
                
                if (load_type == LOAD_SINGLE)
                {
                    nncols_req = settype_cols(curtype);
                    if (nncols_req <= nncols)
                    {
                        nncols = nncols_req;
                    }
                    else if (nncols_req == nncols + 1)
                    {
                        /* X from index, OK */
                        ;
                    }
                    else
                    {
                        errmsg("Column count incorrect");
                        xfree(linebuf);
                        return RETURN_FAILURE;
                    }
                }

                ncols = nncols + nscols;

                /* init the data storage */
                if (init_ss_data(&ssd, ncols, formats) != RETURN_SUCCESS)
                {
                    errmsg("Malloc failed in uniread()");
                    xfree(linebuf);
                    return RETURN_FAILURE;
                }
                
                breakon = FALSE;
            }
            if (nrows % BUFSIZE == 0)
            {
                if (realloc_ss_data(&ssd, nrows + BUFSIZE) != RETURN_SUCCESS)
                {
                    errmsg("Malloc failed in uniread()");
                    free_ss_data(&ssd);
                    xfree(linebuf);
                    return RETURN_FAILURE;
                }
            }

            if (insert_data_row(&ssd, nrows, s) != RETURN_SUCCESS)
            {
                sprintf(tbuf, "Error parsing line %d, skipped", linecount);
                errmsg(tbuf);
                readerror++;
                if (readerror > MAXERR)
                {
                    if (yesno("Lots of errors, abort?", NULL, NULL, NULL))
                    {
                        free_ss_data(&ssd);
                        xfree(linebuf);
                        return RETURN_FAILURE;
                    }
                    else
                    {
                        readerror = 0;
                    }
                }
            }
            else
            {
                nrows++;
            }
        }
    }

    if (nrows > 0)
    {
        /* free excessive storage */
        realloc_ss_data(&ssd, nrows);

        new_set_no=0;
        if (new_set_nos!=NULL)
        {
            delete[] new_set_nos;
            new_set_nos=NULL;
        }

        /* store accumulated data in set(s) */
        if (store_data(&ssd, load_type, label) != RETURN_SUCCESS)
        {
            xfree(linebuf);
            return RETURN_FAILURE;
        }

        append_to_storage(&nr_count,&maj_new_nrs,new_set_no,new_set_nos);
    }

    xfree(linebuf);
    xfree(formats);

    new_set_no=nr_count;
    if (new_set_nos!=NULL) delete[] new_set_nos;
    new_set_nos=maj_new_nrs;
    maj_new_nrs=NULL;

    return RETURN_SUCCESS;
}

int getdata(int gno, char *fn, int src, int load_type)
{
    FILE *fp;
    int retval;
    int save_version, cur_version;
    char fn2[1024];
#ifdef WINDOWS_SYSTEM
    int p,p2=0;
    for (p=0;p<=strlen(fn);p++)
    {
        if (fn[p]=='\\')
        {
            fn2[p2]=fn[p];
            p2++;
            fn2[p2]=fn[p];
            p2++;
        }
        else
        {
            fn2[p2]=fn[p];
            p2++;
        }
    }
#else
    strcpy(fn2,fn);
#endif
    fp = grace_openr(fn2, src);
    if (fp == NULL) {
        return RETURN_FAILURE;
    }
    
    save_version = get_project_version();
    set_project_version(0);

    device_table[DEVICE_SCREEN].pg.dpi=start_dpi;

    set_parser_gno(gno);
    
    retval = uniread(fp, load_type, fn2);

    grace_close(fp);
    
    cur_version = get_project_version();
    if (cur_version != 0) {
        /* a complete project */
        postprocess_project(cur_version);
    } else if (load_type != LOAD_BLOCK) {
        /* just a few sets */
        autoscale_graph(gno, autoscale_onread);
    }
    set_project_version(save_version);

    //device_table[DEVICE_SCREEN].pg.width*=device_table[DEVICE_SCREEN].pg.dpi/72.0;
    //device_table[DEVICE_SCREEN].pg.height*=device_table[DEVICE_SCREEN].pg.dpi/72.0;

    return retval;
}

/*
 * read data to the set from a file overriding the current contents
 */
int update_set_from_file(int gno, int setno, char *fn, int src)
{
    int retval;
    if (set_parser_setno(gno, setno) != RETURN_SUCCESS)
    {
        retval = RETURN_FAILURE;
    }
    else
    {
        FILE *fp;
        prepare_strings_for_saving();

        fp = grace_openr(fn, src);
        if (fp==NULL) return RETURN_FAILURE;

        killsetdata(gno, setno);
        curtype = dataset_type(gno, setno);
        retval = uniread(fp, LOAD_SINGLE, fn);

        grace_close(fp);

        resume_strings_after_load_or_save();
    }
    return retval;
}

void outputset(int gno, int setno, char *fname, char *dformat)
{
    FILE *cp;
    if ((cp = grace_openw(fname)) == NULL)
    {
        return;
    }
    else
    {
        write_set(gno, setno, cp, dformat, TRUE);
        grace_close(cp);
    }
}

int load_project_file(char *fn, int as_n_template)
{    
    int gno;
    int retval;
    bool old_dec_sep=exchange_point_comma;
    bool old_useQtFonts=useQtFonts;

    if (wipeout())
    {
        /*exchange_point_comma=old_dec_sep;
        reset_default_states();
        useQtFonts=old_useQtFonts;*/
        return RETURN_FAILURE;//do not load!
    }
    else//really load a new file
    {
        universal_font_size_factor=1.0;//reset Font-Size to normal
        useQtFonts=false;
        qtfonts_have_been_reset=false;
        exchange_point_comma=false;
        timestamp.path=FALSE;

    //cout << "BEFORE LOAD: FONT=" << grdefaults.font << endl;
    tmp_defaults=grdefaults;//save the current defaults
    tmp_view=grview;
    strcpy(tmp_sformat,sformat);

        for (int i = 0; i < number_of_boxes(); i++) boxes[i].rot = 0.0;
        for (int i = 0; i < number_of_ellipses(); i++) ellip[i].rot = 0.0;

    //cout << "A load project file: default font=" << grdefaults.font << endl;

        save_default_states();

    //cout << "B load project file: default font=" << grdefaults.font << endl;

        //reset linestyles back to ini-settings
        copy_line_style_patterns_to_target(n_linestyles_ini,l_linestyle_pat_ini,linestyle_pat_ini,0);

        prepare_strings_for_saving();

        if (getdata(0, fn, SOURCE_DISK, LOAD_SINGLE) == RETURN_SUCCESS)
        {
            if (as_n_template == FALSE)
            {
                set_docname(fn);
                mainWin->addToHistory(fn);
            }
            clear_dirtystate();
            retval = RETURN_SUCCESS;
        }
        else
        {
            retval = RETURN_FAILURE;
        }

        /* try to switch to the first active graph */
        for (gno = 0; gno < number_of_graphs(); gno++)
        {
            if (is_graph_hidden(gno) == FALSE)
            {
                select_graph(gno);
                break;
            }
        }

        resume_strings_after_load_or_save();

        ///#ifndef NONE_GUI
        ReqUpdateColorSel = TRUE;
        //update_all();
        ///#endif
        exchange_point_comma=old_dec_sep;

//cout << "C load project file: default font=" << grdefaults.font << endl;

        reset_default_states();

//cout << "D load project file: default font=" << grdefaults.font << endl;

        update_fonts_to_default_ordering();

        /// JUST TO TEST SOMETHING!
        /// post_load_font_status();
        useQtFonts=old_useQtFonts;

        /*if (number_of_graphs()<1 || number_of_visible_graphs()<1)
    {*/
        update_timestamp();
        update_all();
        /// mainWin->mainArea->completeRedraw();

        if (FormDeviceSetup!=NULL)
        FormDeviceSetup->printfile_item->setText(get_filename_with_extension(FormDeviceSetup->cur_dev));

        if (auto_set_cwd)
        {
            /*if ()
            {*/
            QFileInfo finfo(fn);
            QString pa=finfo.absolutePath();
            set_workingdir(pa.toLocal8Bit().constData());
            /*}
            else
            {
            char * wdir1=new char[GR_MAXPATHLEN+1];
            getcwd(wdir1, GR_MAXPATHLEN - 1);
            set_workingdir(wdir1);
            delete[] wdir1;
            }*/
        }

        //}
//cout << "E load project file: default font=" << grdefaults.font << endl;

        file_defaults=grdefaults;
        file_view=grview;
        strcpy(file_sformat,sformat);

        grdefaults=tmp_defaults;//restore the defaults
        grview=tmp_view;
        strcpy(sformat,tmp_sformat);
//cout << "AFTER LOAD: FONT=" << grdefaults.font << endl;
        //set_exportname(NULL);
        update_line_style_selectors();
            if (autofit_on_load==TRUE)
            {
                if (mainWin->isVisible()==false)
                {
                autofit_pending=1;
                }
                else
                {
                mainWin->doFitPage();
                }
            }
            else
            mainWin->doPageZoom(mainWin->sldPageZoom->value());
        return retval;
    }
}

int load_project(char *fn)
{
    initialize_cmap();//set color map back to normal
    int ret=load_project_file(fn, FALSE);
    update_default_props();
    update_font_selectors(false);
    return ret;
}

int new_project(char *n_template)
{
    int retval;
    char *s;
    char dummy[1024];
    if (n_template == NULL || n_template[0] == '\0')
    {
        sprintf(dummy,"%s/%s",qt_grace_exe_dir,default_grace_file);
        retval = load_project_file(dummy, TRUE);
        /// retval = load_project_file("n_templates/Default.agr", TRUE);
    }
    else if (is_path_absolute(n_template))//(n_template[0] == '/')//an absolute path
    {
        retval = load_project_file(n_template, TRUE);
    }
    else
    {
        s = (char*)xmalloc(strlen("n_templates/") + strlen(n_template) + 1);
        if (s == NULL)
        {
            retval = RETURN_FAILURE;
        }
        else
        {
            sprintf(s, "n_templates/%s", n_template);
            retval = load_project_file(s, TRUE);
            xfree(s);
        }
    }
    update_default_props();
    update_font_selectors(false);
    return retval;
}

int save_project(char *fn)
{
    FILE *cp;
    int gno, setno;
    char * old_fn, * n_fn=new char[strlen(fn)+16];
    int noask_save = noask;
QFileInfo finfo(fn);
QString suf=finfo.suffix();

tmp_defaults=grdefaults;//save current defaults
tmp_view=grview;
strcpy(tmp_sformat,sformat);
grdefaults=file_defaults;
grview=file_view;
strcpy(sformat,file_sformat);

    if (suf.isEmpty() && auto_set_agr_extension)
    {
    suf=finfo.path()+QDir::separator()+finfo.completeBaseName()+QString(".agr");
    strcpy(n_fn,suf.toLocal8Bit().constData());
    }
    else
    {
    strcpy(n_fn,fn);
    }

    prepare_strings_for_saving();

    old_fn = get_docname();
    if (compare_strings(old_fn, n_fn))
    {
        /* If saving under the same name, don't warn about overwriting */
        noask = TRUE;
    }
    if ((cp = grace_openw(n_fn)) == NULL)
    {
        noask = noask_save;
        delete[] n_fn;
        return RETURN_FAILURE;
    }
    putparms(-1, cp, TRUE);
    //START OF QTGRACE_SPECIALS_HERE!
    save_qtGrace_Additions(cp);
    //END OF QTGRACE_SPECIALS_HERE!
    for (gno = 0; gno < number_of_graphs(); gno++)
    {
        for (setno = 0; setno < number_of_sets(gno); setno++)
        {
            write_set(gno, setno, cp, tmp_sformat, FALSE);//we have to use the current sformat here
        }
    }

    grace_close(cp);

    resume_strings_after_load_or_save();

    set_docname(n_fn);
    clear_dirtystate();
    mainWin->addToHistory(n_fn);

    noask = noask_save;

    if (auto_set_cwd)
    {
    QFileInfo finfo(n_fn);
    QString pa=finfo.absolutePath();
    set_workingdir(pa.toLocal8Bit().constData());
    }
    delete[] n_fn;

    grdefaults=tmp_defaults;//restore defaults
    grview=tmp_view;
    strcpy(sformat,tmp_sformat);

    if (FormDeviceSetup!=NULL)
    FormDeviceSetup->replaceFileNameOnly(getFileBaseNameOnly(QString(get_docname())));

    set_left_footer(NULL);

    return RETURN_SUCCESS;
}

/*
 * write out a set
 */
int write_set(int gno, int setno, FILE *cp, char *format, int rawdata)
{
    int i, n, col, ncols;
    double *x[MAX_SET_COLS];
    char **s;

    if (cp == NULL) {
        return RETURN_FAILURE;
    }
    
    if (is_set_active(gno, setno) == TRUE) {
        n = getsetlength(gno, setno);
        ncols = dataset_cols(gno, setno);
        for (col = 0; col < ncols; col++) {
            x[col] = getcol(gno, setno, col);
        }
        s = get_set_strings(gno, setno);

        if (format == NULL) {
            format = sformat;
        }

        if (!rawdata) {
            fprintf(cp, "@target G%d.S%d\n", gno, setno);
            fprintf(cp, "@type %s\n", set_types(dataset_type(gno, setno)));
        }
        
        for (i = 0; i < n; i++) {
            for (col = 0; col < ncols; col++) {
                if (col != 0) {
                    fputs(" ", cp);
                }
                fprintf(cp, format, x[col][i]);
            }
            if (s != NULL) {
                fprintf(cp, " \"%s\"", PSTRING(s[i]));
            }
            fputs("\n", cp);
        }
        if (rawdata) {
            fprintf(cp, "\n");
        } else {
            fprintf(cp, "&\n");
        }
    }
    
    return RETURN_SUCCESS;
}

int is_agr_file(char * file)
{
ifstream ifi;
char d_test[32],c;
ifi.open(file);
ifi.read(d_test,20);
/*for (int i=0;i<20;i++)
{
ifi >> c;
d_test[i]=c;
}*/
d_test[20]='\0';
ifi.close();
//cout << "d_test=" << d_test << endl;
if (strcmp(d_test,"# Grace project file")==0)
return TRUE;
else
return FALSE;
}

bool is_diadem(char * file,char ** header_name)
{
if (file==NULL) return false;
    if (*header_name!=NULL) delete[] header_name;
    *header_name=new char[strlen(file)+8];
    strcpy(*header_name,file);
    char dummy[128],dummy2[128],dummy3[256];
QFileInfo finfo(*header_name);
QString dat_suffix("dat");
ifstream ifi;
//cout << "Suffix=" << dat_suffix.toLocal8Bit().constData() << endl;
if (QString::compare(finfo.suffix(),dat_suffix,Qt::CaseInsensitive)==0)
{//file has dat-suffix
    ifi.open(*header_name);
    ifi.getline(dummy3,1023);
    ifi.close();
    sscanf(dummy3,"%s %s",dummy,dummy2);
    if (strcmp(dummy,"DIAEXTENDED")==0 && strcmp(dummy2,"{@:ENGLISH")==0)
    {
    return true;
    }
    //else: dat-suffix, but no diadem-header
}
else
{//file does not have dat-suffix-->look for similar file with dat-suffix that is diadem-header
QString new_header_file;
new_header_file=finfo.absolutePath()+QDir::separator()+finfo.completeBaseName()+QString(".dat");
delete[] *header_name;
*header_name=new char[new_header_file.length()+8];
strcpy(*header_name,new_header_file.toLocal8Bit().constData());
    ifi.open(*header_name);
    ifi.getline(dummy3,1023);
    ifi.close();
    sscanf(dummy3,"%s %s",dummy,dummy2);
    if (strcmp(dummy,"DIAEXTENDED")==0 && strcmp(dummy2,"{@:ENGLISH")==0)
    {
    return true;
    }
    //else: dat-suffix, but no diadem-header
}
return false;
}

int read_one_line_from_diadem_header(ifstream & ifi,int & number,char * text)
{
static char dummy[1024];
if (ifi.eof()==true) return -3;//file-end reached
ifi.getline(dummy,1023);
//cout << "dummy=#" << dummy << "# first=" << (int)(dummy[0]) << endl;
if (strlen(dummy)<2) return -1;//empty line
if (dummy[0]=='#')//control_line
{
    if (strncmp("#BEGINGLOBALHEADER",dummy,strlen("#BEGINGLOBALHEADER"))==0) return 1;//begin global header
    else if (strncmp("#ENDGLOBALHEADER",dummy,strlen("#ENDGLOBALHEADER"))==0) return 2;//end global header
    else if (strncmp("#BEGINCHANNELHEADER",dummy,strlen("#BEGINCHANNELHEADER"))==0) return 3;//begin channel header
    else if (strncmp("#ENDCHANNELHEADER",dummy,strlen("#ENDCHANNELHEADER"))==0) return 4;//end channel header
}
else if (dummy[0]=='D')//first_line
{
return -2;//just the first line
}
else//a standard-line with number and ',' and value
{
int pos=0;
while (dummy[pos]!='\0' && dummy[pos]!=',') pos++;
    if (dummy[pos]!=',')
    {
        strcpy(text,dummy);
        return -4;//invalid entry
    }
    else
    {
        strncpy(text,dummy,pos);
        text[pos]='\0';
        //cout << "1 text=#" << text << "#" << endl;
        number=atoi(text);
        strcpy(text,dummy+pos+1);
        //cout << "2 text=#" << text << "#" << endl;
        return 0;//normal line with a normal entry
    }
}
}

void write_to_global_header(int number,char * text,struct DIAdem_Global_Header & gh)
{
char * target=NULL;
    switch (number)
    {
    case 1:
        target=gh.origin;
        break;
    case 2:
        target=gh.revision;
        break;
    case 101:
        target=gh.description;
        break;
    case 102:
        target=gh.comments;
        break;
    case 103:
        target=gh.person;
        break;
    case 104:
        target=gh.date;
        break;
    case 105:
        target=gh.time;
        break;
    case 106:
        target=gh.desc_comments;
        break;
    case 110:
        target=gh.time_format;
        break;
    case 111:
        gh.val_o_noval=atof(text);
        break;
    case 112:
        target=gh.exch_high_low;
        break;
    }
if (target!=NULL)
{
int offset;
#ifdef MAC_SYSTEM
offset=1;
#else
offset=0;
#endif
strcpy(target,text);
if (strlen(target)>1)
target[strlen(target)-offset]='\0';
}
}

void write_to_channel_header(int number,char * text,struct DIAdem_Channel_Header & ch)
{
char * target=NULL;
    switch (number)
    {
    case 200:
        target=ch.name;
        break;
    case 201:
        target=ch.comment;
        break;
    case 202:
        target=ch.unit;
        break;
    case 210:
        target=ch.ch_type;
        break;
    case 211:
        target=ch.file_name;
        break;
    case 213:
        ch.method=atoi(text);
        break;
    case 214:
        target=ch.da_type;
        break;
    case 215:
        target=ch.masking;
        break;
    case 220:
        ch.points=atoi(text);
        break;
    case 221:
        ch.d_pointer=atoi(text);
        break;
    case 222:
        ch.offset=atoi(text);
        break;
    case 223:
        ch.a_pointer=atoi(text);
        break;
    case 230:
        target=ch.a_separator;
        break;
    case 231:
        target=ch.dec_char;
        break;
    case 232:
        target=ch.exp_char;
        break;
    case 240:
        ch.start=atof(text);
        break;
    case 241:
        ch.step=atof(text);
        break;
    case 250:
        ch.minimum=atof(text);
        break;
    case 251:
        ch.maximum=atof(text);
        break;
    case 252:
        target=ch.noval_key;
        break;
    case 253:
        target=ch.mono_key;
        break;
    case 254:
        ch.val_o_noval=atof(text);
        break;
    case 260:
        target=ch.dat_display_key;
        break;
    case 270:
        target=ch.RV[0];
        break;
    case 271:
        target=ch.RV[1];
        break;
    case 272:
        target=ch.RV[2];
        break;
    case 273:
        target=ch.RV[3];
        break;
    case 274:
        target=ch.RV[4];
        break;
    case 275:
        target=ch.RV[5];
        break;
    case 280:
        ch.RI[0]=atoi(text);
        break;
    case 281:
        ch.RI[1]=atoi(text);
        break;
    case 282:
        ch.RI[2]=atoi(text);
        break;
    case 283:
        ch.RI[3]=atoi(text);
        break;
    case 284:
        ch.RI[4]=atoi(text);
        break;
    }
if (target!=NULL)
{
int offset;
#ifdef MAC_SYSTEM
offset=1;
#else
offset=0;
#endif
strcpy(target,text);
if (strlen(target)>1)
target[strlen(target)-offset]='\0';
}
}

void read_DIAdem_header(char * header_file,struct DIAdem_Header & header)
{
    int status,number;
    int position;
    char text[512];
    struct DIAdem_Channel_Header temp_channel;
    struct DIAdem_Channel_Header *n_channels;
    memset((void*)(&header),0,sizeof(header));
    memset((void*)(&temp_channel),0,sizeof(temp_channel));
ifstream ifi;
ifi.open(header_file);
position=0;//nothing
while (!ifi.eof())
{
status=read_one_line_from_diadem_header(ifi,number,text);
//cout << "status=" << status << " nr=" << number << " text=" << text << endl;
    switch (status)
    {
    case 1://begin global header
        position=1;//write to global header
    break;
    case 2://end global header
        position=0;//nothing
    break;
    case 3://begin channel header
        position=2;//write to channel header
        memset((void*)(&temp_channel),0,sizeof(temp_channel));
    break;
    case 4://end channel header
        position=0;//nothing
        //write temp_channel into header
        n_channels=new struct DIAdem_Channel_Header[header.nr_of_channels+1];
        for (int i=0;i<header.nr_of_channels;i++)
        {
        memcpy(n_channels+i,header.channels+i,sizeof(struct DIAdem_Channel_Header));
        }
        memcpy(n_channels+header.nr_of_channels,&temp_channel,sizeof(struct DIAdem_Channel_Header));
        delete[] header.channels;
        header.channels=n_channels;
        header.nr_of_channels++;
    break;
    case 0:
        if (position==1)//write to global header
        {
        write_to_global_header(number,text,header.global);
        }
        else if (position==2)//write to channel header
        {
        write_to_channel_header(number,text,temp_channel);
        }
    break;
    default:
    //do nothing
    break;
    }
}
ifi.close();
}

void postprocess_diadem_header(struct DIAdem_Header & header,struct importSettings & bin_import,int & nr_of_new_sets,int ** n_gnos,int ** n_snos)
{
header.nr_of_real_channels=0;//real means not implicit
header.implicit_channel=-1;//no implicit channel
if (*n_snos!=NULL) delete[] n_snos;
if (*n_gnos!=NULL) delete[] n_gnos;
bin_import.nr_of_header_values=0;
bin_import.nr_of_import_tokens=0;
QString a1,imp_ref("IMPLICIT"),ex_ref("EXPLICIT");
QStringList setLegends;
setLegends.clear();
QStringList setComment;
setComment.clear();
QStringList setFiles;
setFiles.clear();

/*char x_axis_formula[128];
char y_axis_formula[128];*/

for (int i=0;i<header.nr_of_channels;i++)
{
//cout << i << " header.channel=#" << header.channels[i].ch_type << "#" << endl;
    a1=QString(header.channels[i].ch_type);
    if (QString::compare(a1.left(ex_ref.length()),ex_ref,Qt::CaseInsensitive)==0)//strcmp(header.channels[i].ch_type,"EXPLICIT")==0)
    {
    header.nr_of_real_channels++;
    setLegends << QString(header.channels[i].name);
    setComment << QString(header.channels[i].comment);
    setFiles << QString(header.channels[i].file_name);
    }
    else if (QString::compare(a1.left(imp_ref.length()),imp_ref,Qt::CaseInsensitive)==0 && header.implicit_channel==-1)//strcmp(header.channels[i].ch_type,"IMPLICIT")==0
    {
    header.implicit_channel=i;
    }
}
cout << "channels=" << header.nr_of_channels << " real_channels=" << header.nr_of_real_channels << " first_implicit_channel=" << header.implicit_channel << endl;

nr_of_new_sets=header.nr_of_real_channels;
if (header.nr_of_real_channels<=0) return;//nothing to do here

*n_snos=new int[header.nr_of_real_channels];
*n_gnos=new int[header.nr_of_real_channels];

prepare_imp_settings_for_header_import(bin_import);

bin_import.autoscale=3;//autoscale all axes

bin_import.channels=header.nr_of_real_channels;//only import one channel at a time (even if there are more channels)
bin_import.contains_trigger=false;
bin_import.channel_target=new int[header.nr_of_real_channels+2];
bin_import.channel_size=new int[header.nr_of_real_channels+2];
bin_import.channel_format=new int[header.nr_of_real_channels+2];

int * tngnos=new int[header.nr_of_real_channels+2];
int * tnsnos=new int[header.nr_of_real_channels+2];
int tnr_of_sets;
int counter=0;
for (int i=0;i<header.nr_of_channels;i++)
{
cout << "ch_type=#" << header.channels[i].ch_type << "# file=#" << header.channels[i].file_name << "#" << endl;
    if (strncmp(header.channels[i].ch_type,"IMPLICIT",8)==0)
    {
    cout << "Channel " << i << " = Implicit" << endl;
    continue;
    }
    cout << "Channel " << i << " = Explicit" << endl;
bin_import.whole_size=bin_import.single_size=bin_import.bytesize=bin_import.bitsize=-1;
QFileInfo fi(bin_import.HeaderFile);

bin_import.DataFile=fi.path()+QDir::separator()+QString(header.channels[i].file_name);

bin_import.points=header.channels[i].points;
bin_import.channel_factors[counter]=header.channels[i].step;
bin_import.channel_offsets[counter]=header.channels[i].start;

if (strcmp(header.channels[i].da_type,"INT16")==0)
{
bin_import.bytesize=2;
bin_import.channel_format[counter]=COLUMN_SIG_SHORT;

}
else if (strcmp(header.channels[i].da_type,"INT32")==0)
{
bin_import.bytesize=4;
bin_import.channel_format[counter]=COLUMN_SIG_INT;

}
else if (strcmp(header.channels[i].da_type,"REAL32")==0)
{
bin_import.bytesize=4;
bin_import.channel_format[counter]=COLUMN_FLOAT;

}
else if (strcmp(header.channels[i].da_type,"REAL64")==0)
{
bin_import.bytesize=8;
bin_import.channel_format[counter]=COLUMN_DOUBLE;

}

bin_import.channel_size[counter]=bin_format_size(bin_import.channel_format[counter]);
bin_import.channel_target[counter]=IMPORT_TO_Y;

counter++;
}

bin_import.headersize=0;//header.channels[0].d_pointer-1;
bin_import.setorder=0;
bin_import.x0=header.channels[header.implicit_channel].start;
bin_import.x0set=true;
bin_import.deltax=header.channels[header.implicit_channel].step;
bin_import.deltaxset=true;

complete_channel_settings(bin_import);

cout << "sizeof filename=" << strlen(header.channels[0].file_name) << endl;
//bin_import.DataFile=QString("/Users/andreaswinter/Desktop/qtgrace_w_undo/Diadem/test_0001_S000.I32");
//cout << "ok: sizeof=" << strlen("/Users/andreaswinter/Desktop/qtgrace_w_undo/Diadem/test_0001_S000.I32") << endl;
QFileInfo fi2(bin_import.DataFile.toLocal8Bit().constData());
/*cout << "notok sizeof=" << strlen(bin_import.DataFile.toLocal8Bit().constData()) << endl;
cout << fi2.fileName().toLocal8Bit().constData() << endl;
cout << fi2.filePath().toLocal8Bit().constData() << endl;
cout << "exists=" << fi2.exists() << endl;
cout << "is_readable=" << fi2.isReadable() << endl;*/

ifstream ifi;
ifi.open(bin_import.DataFile.toLocal8Bit().constData(),ios::binary);
cout << "reading from Diadem-File=#" << bin_import.DataFile.toLocal8Bit().constData() << "# opened?" << ifi.is_open() << endl;
cout << "std bin import: channels=" << bin_import.channels << " points=" << bin_import.points << endl;
readBinaryFromFile(ifi,bin_import,&bin_import.first_data);
cout << "gelesen: std bin import: columns=" << bin_import.columns_read << " points=" << bin_import.points_read << endl;
ifi.close();

postprocess_bin_import_data(bin_import,tnr_of_sets,&tngnos,&tnsnos);

for (int j=0;j<tnr_of_sets;j++)
{
setcomment(tngnos[j],tnsnos[j],setComment.at(j).toLocal8Bit().data());
set_legend_string(tngnos[j],tnsnos[j],setLegends.at(j).toLocal8Bit().data());
}

/*counter=0;
    for (int j=0;j<tnr_of_sets;j++)
    {
    (*n_gnos)[counter]=tngnos[j];
    (*n_snos)[counter]=tnsnos[j];
    //set_legend_string(tngnos[j],tnsnos[j],);
        if (header.implicit_channel>=0)//postprocess x-axis
        for (int k=0;k<getsetlength(tngnos[j],tnsnos[j]);k++)
        {
        g[tngnos[j]].p[tnsnos[j]].data.ex[0][k]=header.channels[header.implicit_channel].start+k*header.channels[header.implicit_channel].step;
        //g[tngnos[j]].p[tnsnos[j]].data.ex[1][k]*=header.channels[i].step;
        //g[tngnos[j]].p[tnsnos[j]].data.ex[1][k]+=header.channels[i].start;
        }
    counter++;
    }*/

//}
nr_of_new_sets=counter;
/*
if (header.implicit_channel!=-1)
{
sprintf(y_axis_formula,"y=%f+x*%f",header.channels[header.implicit_channel].start,header.channels[header.implicit_channel].step);
}
*/

delete[] tngnos;
delete[] tnsnos;
}

void read_diadem_data_into_sets(struct DIAdem_Header & header,struct importSettings & bin_import,int target_gno,int & nr_new_sets,int * & snos)
{
nr_new_sets=0;
ifstream ifi;

ifi.open(header.channels[1].file_name, ios::binary );/// warning: just channel 1 for testing
double ** data;
readBinaryFromFile(ifi,bin_import,&data);

ifi.close();
}

void init_agr_file_info(struct agr_file_info & afi)
{
    strcpy(afi.filename,"");
    afi.nr_of_graphs=afi.nr_of_sets=0;
    afi.target_gno=afi.headerlines=-1;
    afi.g_ids.clear();
    afi.s_ids.clear();
    afi.set_comments.clear();
    afi.set_legends.clear();
    afi.set_types.clear();
    afi.project_description.clear();
    afi.import.clear();
    afi.color.clear();
    afi.linet.clear();
    afi.lines.clear();
    afi.linew.clear();
    afi.pattern.clear();
}

//to determine the number of graphs and datasets in an agr file
void read_header_from_agr(char * filename, struct agr_file_info & afi)
{
char dummy_line[1024];
char new_linetype[32];
double new_linetype_val;
int pos,status,tmp,len,index;
int cur_graph,cur_set;

    init_agr_file_info(afi);
    QFileInfo fi(filename);
    if (fi.exists()==false) return;
    strcpy(afi.filename,filename);

//cout << "reading infos from #" << filename << "#" << endl;
QRegExp regex1("@\\s+s\\d+\\s+comment");
QRegExp regex1a("@\\s+S\\d+\\s+comment");
QRegExp regex2("@\\s+s\\d+\\s+legend");
QRegExp regex2a("@\\s+S\\d+\\s+legend");
QRegExp regex3("@\\s+s\\d+\\s+type");
QRegExp regex3a("@\\s+S\\d+\\s+type");
QRegExp regex4("@\\s+s\\d+\\s+line");
QString tmp_str,tmp_str2;
QString new_comment,new_type,new_legend;

int n_linet,n_lines,n_linepattern,n_linecolor;
double n_linew;

ifstream ifi;
ifi.open(filename);
afi.headerlines=0;
status=0;//nothing
cur_graph=cur_set=-1;//none
while (!ifi.eof())
{
ifi.getline(dummy_line,1023);
afi.headerlines++;
dummy_line[1023]='\0';//for safety
//cout << "dummy_line=" << dummy_line << endl;
if (dummy_line[0]!='@' && dummy_line[0]!='#') continue;//no settings here - go to next line
if (strncmp(dummy_line,"@target",7)==0)
{
//cout << "Ende found:#" << dummy_line << "#" << endl;
cur_graph=-1;
afi.headerlines--;
break;
}
if (strncmp(dummy_line,"@description",12)==0)
{
tmp_str=QString(dummy_line);
afi.project_description=get_text_in_quotations(tmp_str);
continue;
}
/*if (sscanf(dummy_line,"@with G%d",&tmp))
{
cur_graph=tmp;
}
else*/if (sscanf(dummy_line,"@with g%d",&tmp))
{
cur_graph=tmp;
}
if (cur_graph>=0)
{
len=strlen(dummy_line);
tmp_str=QString::fromLocal8Bit(dummy_line);//::fromLocal8bit
//tmp_str=tmp_str.toUtf8();
tmp_str2=get_text_in_quotations(tmp_str);

/*if (tmp_str2.length()>0)
{
cout << "in quotations=#" << tmp_str2.toLocal8Bit().constData() << "#" << endl;
//tmp_str2=tmp_str2.fromLocal8Bit(tmp_str2);
//cout << "in quotations=#" << tmp_str2.toLocal8Bit().constData() << "#" << endl;
}*/

//find the set-id
pos=-1;
for (int j=0;j<len;j++)
{
    if (dummy_line[j]=='s' || dummy_line[j]=='S')
    {
    pos=j+1;
    break;
    }
}
cur_set=-1;
if (pos>=0)
{
sscanf(dummy_line+pos,"%d",&cur_set);
}
index=-1;
for (int j=0;j<afi.g_ids.length();j++)
{
    if (afi.g_ids.at(j)==cur_graph && afi.s_ids.at(j)==cur_set)
    {
    index=j;
    break;
    }
}
new_comment=QString("");
new_type=QString("");
new_legend=QString("");

n_linet=n_lines=n_linepattern=n_linecolor=-1;
n_linew=-1.0;
    if (regex1.indexIn(tmp_str)>=0)//comment
    {
    new_comment=tmp_str2;
    }
    else if (regex1a.indexIn(tmp_str)>=0)//comment
    {
    new_comment=tmp_str2;
    }
    else if (regex2.indexIn(tmp_str)>=0)//legend
    {
    new_legend=tmp_str2;
    }
    else if (regex2a.indexIn(tmp_str)>=0)//legend
    {
    new_legend=tmp_str2;
    }
    else if (regex3.indexIn(tmp_str)>=0)//type
    {
    pos=tmp_str.lastIndexOf(" ");
    new_type=tmp_str.mid(pos+1);
    }
    else if (regex3a.indexIn(tmp_str)>=0)//type
    {
    pos=tmp_str.lastIndexOf(" ");
    new_type=tmp_str.mid(pos+1);
    }
    else if (regex4.indexIn(tmp_str)>=0)//line-setting of a set
    {
    pos=tmp_str.lastIndexOf(" ");
    pos=tmp_str.lastIndexOf(" ",pos-1);
    sscanf(dummy_line+pos,"%s %lf",new_linetype,&new_linetype_val);
        if (strcmp(new_linetype,"type")==0)
        {
        n_linet=(int)new_linetype_val;
        }
        else if (strcmp(new_linetype,"linestyle")==0)
        {
        n_lines=(int)new_linetype_val;
        }
        else if (strcmp(new_linetype,"linewidth")==0)
        {
        n_linew=new_linetype_val;
        }
        else if (strcmp(new_linetype,"color")==0)
        {
        n_linecolor=(int)new_linetype_val;
        }
        else if (strcmp(new_linetype,"pattern")==0)
        {
        n_linepattern=(int)new_linetype_val;
        }
    //cout << "Line Setting #" << dummy_line << "# type=" << new_linetype << " val=" << new_linetype_val << endl;
    }
if ( !( new_comment.isEmpty() && new_type.isEmpty() && new_legend.isEmpty() &&
        n_linecolor==-1 && n_linet==-1 && n_lines==-1 && n_linepattern==-1 && n_linew==-1.0
        ) )
{//new data found
    if (index==-1)//no suitable set yet
    {
    afi.g_ids << cur_graph;
    afi.s_ids << cur_set;
    afi.set_comments << new_comment;
    afi.set_legends << new_legend;
    afi.set_types << new_type;
    afi.import << TRUE;

    afi.color << n_linecolor;
    afi.lines << n_lines;
    afi.pattern << n_linepattern;
    afi.linet << n_linet;
    afi.linew << n_linew;
    }
    else//a set with suitable index exists
    {
    if (!new_comment.isEmpty()) afi.set_comments.replace(index,new_comment);
    if (!new_type.isEmpty()) afi.set_types.replace(index,new_type);
    if (!new_legend.isEmpty()) afi.set_legends.replace(index,new_legend);

    if (n_linecolor>=0) afi.color.replace(index,n_linecolor);
    if (n_linet>=0) afi.linet.replace(index,n_linet);
    if (n_lines>=0) afi.lines.replace(index,n_lines);
    if (n_linepattern>=0) afi.pattern.replace(index,n_linepattern);
    if (n_linew>=0.0 )afi.linew.replace(index,n_linew);

    }
}

}//end if cur_graph>=0

}//end while
ifi.close();
//cout << "stopped reading" << endl;
/*for (int i=0;i<afi.g_ids.length();i++)
{
cout << "G" << afi.g_ids.at(i) << ".S" << afi.s_ids.at(i) << " type=" << afi.set_types.at(i).toLocal8Bit().constData() << " legend=\"" <<  afi.set_legends.at(i).toLocal8Bit().constData() << "\" comment=\"" << afi.set_comments.at(i).toLocal8Bit().constData() << "\"" << endl;
}*/
QList<int> tmp_list=afi.g_ids;
afi.nr_of_graphs=0;
for (int i=0;i<tmp_list.length();tmp_list.removeAll(tmp_list.at(0)))
{
afi.nr_of_graphs++;
}
afi.nr_of_sets=afi.g_ids.length();
//cout << "Graphs=" << afi.nr_of_graphs << endl;
//cout << "Sets  =" << afi.nr_of_sets << endl;
//cout << "Header Lines=" << afi.headerlines << endl;
}

//reads only the datasets found in an agr-file into the graph gno (no format-settings or texts are read)
void read_datasets_from_agr(struct agr_file_info afi)
{
char dummy_line[1024];
char type_ident[32];
int pos,status,index;
int c_gno,c_sno,c_settype,c_cols_to_read;
int n_set;//new setid
int index_in_afi;//import-informations in afi for the current data set
int expected_type;
int gno=afi.target_gno;
bool import_this;
char * dummy_string=NULL;
QString s_dummy_line,s_dummy_single;
QRegExp regex("@\\s+S\\d+\\s+type\\s+xy");
QRegExp regex2("@type\\s+");
QRegExp regex3("@target\\s+G\\d+.S\\d+");
QRegExp regex4("\\S");
QStringList i_list;
QList<double> vals[6];
    for (int i=0;i<6;i++)
    vals[i].clear();
//cout << "read dataset from agr " << regex.pattern().toLocal8Bit().constData() << endl;
ifstream ifi;

prepare_strings_for_saving();

if (new_set_nos!=NULL) delete[] new_set_nos;
new_set_no=0;
new_set_nos=new int[afi.nr_of_sets+2];

ifi.open(afi.filename);
status=0;//nothing
//1=set found-->read data
//2=set complete-->0
index=0;
while (index<afi.headerlines && !ifi.eof())
{
ifi.getline(dummy_line,1023);
index++;
}
c_gno=c_sno=-1;
index_in_afi=-1;
while (!ifi.eof())
{
ifi.getline(dummy_line,1023);
    switch (status)
    {
    case 0:
    s_dummy_line=QString(dummy_line);
    pos=s_dummy_line.indexOf(regex2);//@type ...
        if (pos>=0)
        {
        status=1;//we expect new data in the next lines
        sscanf(dummy_line,"@type %s",type_ident);
        c_settype=get_settype_by_name(type_ident);
        c_cols_to_read=settype_cols(c_settype);
        //cout << "2:" << dummy_line << " TYPE=" << type_ident << " ID=" << c_settype << " COLS=" << c_cols_to_read << endl;
            for (int i=0;i<6;i++)//clear previous data
            vals[i].clear();
        }
        else
        {
        pos=s_dummy_line.indexOf(regex3);//@target ...
            if (pos>=0)
            {
            sscanf(dummy_line,"@target G%d.S%d",&c_gno,&c_sno);
            index_in_afi=-1;
                for (int k=0;k<afi.g_ids.length();k++)
                {
                    if (afi.g_ids.at(k)==c_gno && afi.s_ids.at(k)==c_sno)
                    {
                    index_in_afi=k;
                    break;
                    }
                }
                if (index_in_afi!=-1)
                {
                expected_type=get_settype_by_name(afi.set_types.at(index_in_afi).toLatin1().data());
                }
            }
        }
    break;
    case 1:
        if (dummy_line[0]=='&')//finish reading a set
        {
        status=0;
        //cout << vals[0].length() << " Datenpunkte gelesen" << endl;
            if (index_in_afi>=0 && expected_type==c_settype)
            {
            //cout << "Original: G" << c_gno << ".S" << c_sno << " type=" << afi.set_types.at(index_in_afi).toLatin1().data() << endl;
            //cout << "Import? " << afi.import.at(index_in_afi) << endl;
            import_this=afi.import.at(index_in_afi);
            }
            else
            {
            //cout << "FEHLER BEI IMPORT" << endl;
            errmsg(QObject::tr("Mismatch in set-import-types.").toLocal8Bit().data());
            import_this=false;
            }
        if (import_this==true)
        {
        //create a new set
        n_set=nextset(gno);
        setlength(gno,n_set,vals[0].length());
        set_dataset_type(gno,n_set,c_settype);
        //store the data into the new set
            for (int i=0;i<c_cols_to_read;i++)
            {
                for (int j=0;j<vals[i].length();j++)
                g[gno].p[n_set].data.ex[i][j]=vals[i].at(j);
            }
        //add label and set-decorations
        //set-legend
        if (dummy_string!=NULL) delete[] dummy_string;
        s_dummy_single=afi.set_legends.at(index_in_afi).toUtf8();
        dummy_string=new char[s_dummy_single.length()*4+8];
        strcpy(g[gno].p[n_set].orig_lstr,afi.set_legends.at(index_in_afi).toLocal8Bit().constData());
            if (activateLaTeXsupport==TRUE)
            complete_LaTeX_to_Grace_Translator(s_dummy_single);
        generate_string_Qt_aware(dummy_string,s_dummy_single);
        strcpy(g[gno].p[n_set].lstr,dummy_string);
        //set-comment
        delete[] dummy_string;
        s_dummy_single=afi.set_comments.at(index_in_afi).toUtf8();
        dummy_string=new char[s_dummy_single.length()*4+8];
        strcpy(g[gno].p[n_set].orig_comments,afi.set_comments.at(index_in_afi).toLocal8Bit().constData());
            if (activateLaTeXsupport==TRUE)
            complete_LaTeX_to_Grace_Translator(s_dummy_single);
        generate_string_Qt_aware(dummy_string,s_dummy_single);
        strcpy(g[gno].p[n_set].comments,dummy_string);
        delete[] dummy_string;
        dummy_string=NULL;
        //linestyles
        g[gno].p[n_set].linet=afi.linet.at(index_in_afi);
        g[gno].p[n_set].lines=afi.lines.at(index_in_afi);
        g[gno].p[n_set].linew=afi.linew.at(index_in_afi);
        g[gno].p[n_set].linepen.color=afi.color.at(index_in_afi);
        g[gno].p[n_set].linepen.pattern=afi.pattern.at(index_in_afi);

        new_set_nos[new_set_no++]=n_set;
        }
        c_gno=c_sno=-1;
        expected_type=index_in_afi=-1;
        import_this=false;
        }
        else//read set data
        {
        s_dummy_line=QString(dummy_line);
        pos=0;
        i_list=s_dummy_line.split(" ");
        /*cout << "Before cleanup:" << endl;
        for (int i=0;i<i_list.length();i++)
        {
        cout << i_list.at(i).toLocal8Bit().constData() << " # ";
        }
        cout << endl;*/
        for (int i=0;i<i_list.length();i++)//remove all entries that are just whitespaces
        {
            if (!i_list.at(i).contains(regex4)) i_list.removeAt(i--);
        }
        /*cout << "After cleanup:" << endl;
        for (int i=0;i<i_list.length();i++)
        {
        cout << i_list.at(i).toLocal8Bit().constData() << " # ";
        }
        cout << endl;*/
            for (int i=0;i<i_list.length();i++)
            {
            vals[i] << i_list.at(i).toDouble();
            }
        }
    break;
    }
//cout << "dummy_line=" << dummy_line << endl;
/*
pos=s_dummy_line.indexOf(regex);
    if (pos>=0)
    {
    cout << "1:" << dummy_line << endl;
    }
*/
}
ifi.close();
resume_strings_after_load_or_save();
}

int readDataFromClient(char* dataIn, int load_type,char *label)
{
    int save_version, cur_version;

    save_version = get_project_version();
    set_project_version(0);

    set_parser_gno(0);

   int nrows, ncols, nncols, nscols, nncols_req;
   int *formats = NULL;
   int breakon, readerror;
   ss_data ssd;
   char *s, tbuf[128];
   char *linebuf=NULL;
   int linelen=0;   /* a misleading name ... */
   int linecount;


   //we have to reserve some memory here for input-data
   int * maj_new_nrs=NULL;
   int nr_count=0;

   linecount = 0;
   readerror = 0;
   nrows = 0;

   breakon = TRUE;

   memset(&ssd, 0, sizeof(ssd));

   linebuf = strtok(dataIn,"\n");

   while (linebuf != NULL)
   {
       linecount++;
       s = (char*) malloc(strlen(linebuf)+2);

       strcpy(s,linebuf);
       strcat(s,"\n");

       linebuf = strtok(NULL, "\n");

       while (*s == ' ' || *s == '\t' || *s == '\n')
       {
           s++;
       }
       /* skip comments */
       if (*s == '#') {
           parse_qtGrace_Additions(s);
           continue;
       }
       if (exchange_point_comma && (*s==',' || *s=='.'))
       {
           if (*s=='.') *s=',';
           else *s='.';
       }
       /*   command     end-of-set      EOL   */
       if (*s == '@' || *s == '&' || *s == '\0')
       {
           /* a data break line */
           if (breakon != TRUE)
           {
               /* free excessive storage */
               realloc_ss_data(&ssd, nrows);

               new_set_no=0;
               if (new_set_nos!=NULL)
               {
                   delete[] new_set_nos;
                   new_set_nos=NULL;
               }

               /* store accumulated data in set(s) */
               if (store_data(&ssd, load_type, label) != RETURN_SUCCESS)
               {
                   xfree(linebuf);
                   return RETURN_FAILURE;
               }

               append_to_storage(&nr_count,&maj_new_nrs,new_set_no,new_set_nos);

               /* reset state registers */
               nrows = 0;
               readerror = 0;
               breakon = TRUE;
           }
           if (*s == '@')
           {
               /*int retval = */scanner(s + 1);
               /*cout << "s=" << s << " | " << retval << endl;*/
               continue;
           }
       }
       else
       {
           if (breakon)
           {
               /* parse the data line */
               XCFREE(formats);
               if (parse_ss_row(s, &nncols, &nscols, &formats) != RETURN_SUCCESS)
               {
                   errmsg("Can't parse data");
                   xfree(linebuf);
                   return RETURN_FAILURE;
               }

               if (load_type == LOAD_SINGLE)
               {
                   nncols_req = settype_cols(curtype);
                   if (nncols_req <= nncols)
                   {
                       nncols = nncols_req;
                   }
                   else if (nncols_req == nncols + 1)
                   {
                       /* X from index, OK */
                       ;
                   }
                   else
                   {
                       errmsg("Column count incorrect");
                       xfree(linebuf);
                       return RETURN_FAILURE;
                   }
               }

               ncols = nncols + nscols;

               /* init the data storage */
               if (init_ss_data(&ssd, ncols, formats) != RETURN_SUCCESS)
               {
                   errmsg("Malloc failed in uniread()");
                   xfree(linebuf);
                   return RETURN_FAILURE;
               }

               breakon = FALSE;
           }
           if (nrows % BUFSIZE == 0)
           {
               if (realloc_ss_data(&ssd, nrows + BUFSIZE) != RETURN_SUCCESS)
               {
                   errmsg("Malloc failed in uniread()");
                   free_ss_data(&ssd);
                   xfree(linebuf);
                   return RETURN_FAILURE;
               }
           }

           if (insert_data_row(&ssd, nrows, s) != RETURN_SUCCESS)
           {
               sprintf(tbuf, "Error parsing line %d, skipped", linecount);
               errmsg(tbuf);
               readerror++;
               if (readerror > MAXERR)
               {
                   if (yesno("Lots of errors, abort?", NULL, NULL, NULL))
                   {
                       free_ss_data(&ssd);
                       xfree(linebuf);
                       return RETURN_FAILURE;
                   }
                   else
                   {
                       readerror = 0;
                   }
               }
           }
           else
           {
               nrows++;
           }
       }
   }

   if (nrows > 0)
   {
       /* free excessive storage */
       realloc_ss_data(&ssd, nrows);

       new_set_no=0;
       if (new_set_nos!=NULL)
       {
           delete[] new_set_nos;
           new_set_nos=NULL;
       }

       /* store accumulated data in set(s) */
       if (store_data(&ssd, load_type, label) != RETURN_SUCCESS)
       {
           xfree(linebuf);
           return RETURN_FAILURE;
       }

       append_to_storage(&nr_count,&maj_new_nrs,new_set_no,new_set_nos);
   }

   xfree(linebuf);
   xfree(formats);

   new_set_no=nr_count;
   if (new_set_nos!=NULL) delete[] new_set_nos;
   new_set_nos=maj_new_nrs;
   maj_new_nrs=NULL;

   cur_version = get_project_version();
   if (cur_version != 0) {
       /* a complete project */
       postprocess_project(cur_version);
   } else if (load_type != LOAD_BLOCK) {
       /* just a few sets */
       if(1){
           autoscale_graph(0, autoscale_onread);
       }
   }
   set_project_version(save_version);

   return RETURN_SUCCESS;
}

#ifdef HAVE_NETCDF

/*
 * read a variable from netcdf file into a set in graph gno
 * xvar and yvar are the names for x, y in the netcdf file resp.
 * return 0 on fail, return 1 if success.
 *
 * if xvar == NULL, then load the index of the point to x
 *
 */
int readnetcdf(int gno,
               int setno,
               char *netcdfname,
               char *xvar,
               char *yvar,
               int nstart,
               int nstop,
               int nstride)
{
    int cdfid;			/* netCDF id */
    int i, n;
    double *x, *y;
    float *xf, *yf;
    short *xs, *ys;
    long *xl, *yl;
    char buf[256];

    /* variable ids */
    int x_id = -1, y_id;

    /* variable shapes */
    long start[2];
    long count[2];

    nc_type xdatatype = 0;
    nc_type ydatatype = 0;
    int xndims, xdim[10], xnatts;
    int yndims, ydim[10], ynatts;
    long nx, ny;

    ncopts = 0;			/* no crash on error */

    /*
 * get a set if on entry setno == -1, if setno=-1, then fail
 */
    if (setno == -1) {
        if ((setno = nextset(gno)) == -1) {
            return 0;
        }
    } else {
        if (is_set_active(gno, setno)) {
            killset(gno, setno);
        }
    }
    /*
 * open the netcdf file and locate the variable to read
 */
    if ((cdfid = ncopen(netcdfname, NC_NOWRITE)) == -1) {
        errmsg("Can't open file.");
        return 0;
    }
    if (xvar != NULL) {
        if ((x_id = ncvarid(cdfid, xvar)) == -1) {
            char ebuf[256];
            sprintf(ebuf, "readnetcdf(): No such variable %s for X", xvar);
            errmsg(ebuf);
            return 0;
        }
        ncvarinq(cdfid, x_id, NULL, &xdatatype, &xndims, xdim, &xnatts);
        ncdiminq(cdfid, xdim[0], NULL, &nx);
        if (xndims != 1) {
            errmsg("Number of dimensions for X must be 1.");
            return 0;
        }
    }
    if ((y_id = ncvarid(cdfid, yvar)) == -1) {
        char ebuf[256];
        sprintf(ebuf, "readnetcdf(): No such variable %s for Y", yvar);
        errmsg(ebuf);
        return 0;
    }
    ncvarinq(cdfid, y_id, NULL, &ydatatype, &yndims, ydim, &ynatts);
    ncdiminq(cdfid, ydim[0], NULL, &ny);
    if (yndims != 1) {
        errmsg("Number of dimensions for Y must be 1.");
        return 0;
    }
    if (xvar != NULL) {
        n = nx < ny ? nx : ny;
    } else {
        n = ny;
    }
    if (n <= 0) {
        errmsg("Length of dimension == 0.");
        return 0;
    }
    /*
 * allocate for this set
 */
    x = xcalloc(n, SIZEOF_DOUBLE);
    y = xcalloc(n, SIZEOF_DOUBLE);
    if (x == NULL || y == NULL) {
        XCFREE(x);
        XCFREE(y);
        ncclose(cdfid);
        return 0;
    }
    start[0] = 0;
    count[0] = n;		/* This will retrieve whole file, modify
                 * these values to get subset. This will only
                 * work for single-dimension vars.  You need
                 * to add dims to start & count for
                 * multi-dimensional. */

    /*
 * read the variables from the netcdf file
 */
    if (xvar != NULL) {
        /* TODO should check for other data types here */
        /* TODO should check for NULL on the xcallocs() */
        /* TODO making assumptions about the sizes of shorts and longs */
        switch (xdatatype) {
        case NC_SHORT:
            xs = xcalloc(n, SIZEOF_SHORT);
            ncvarget(cdfid, x_id, start, count, (void *) xs);
            for (i = 0; i < n; i++) {
                x[i] = xs[i];
            }
            xfree(xs);
            break;
        case NC_LONG:
            xl = xcalloc(n, SIZEOF_LONG);
            ncvarget(cdfid, x_id, start, count, (void *) xl);
            for (i = 0; i < n; i++) {
                x[i] = xl[i];
            }
            xfree(xl);
            break;
        case NC_FLOAT:
            xf = xcalloc(n, SIZEOF_FLOAT);
            ncvarget(cdfid, x_id, start, count, (void *) xf);
            for (i = 0; i < n; i++) {
                x[i] = xf[i];
            }
            xfree(xf);
            break;
        case NC_DOUBLE:
            ncvarget(cdfid, x_id, start, count, (void *) x);
            break;
        default:
            errmsg("Data type not supported");
            XCFREE(x);
            XCFREE(y);
            ncclose(cdfid);
            return 0;
            break;
        }
    } else {			/* just load index */
        for (i = 0; i < n; i++) {
            x[i] = i + 1;
        }
    }
    switch (ydatatype) {
    case NC_SHORT:
        ys = xcalloc(n, SIZEOF_SHORT);
        ncvarget(cdfid, y_id, start, count, (void *) ys);
        for (i = 0; i < n; i++) {
            y[i] = ys[i];
        }
        break;
    case NC_LONG:
        yl = xcalloc(n, SIZEOF_LONG);
        ncvarget(cdfid, y_id, start, count, (void *) yl);
        for (i = 0; i < n; i++) {
            y[i] = yl[i];
        }
        break;
    case NC_FLOAT:
        /* TODO should check for NULL here */
        yf = xcalloc(n, SIZEOF_FLOAT);
        ncvarget(cdfid, y_id, start, count, (void *) yf);
        for (i = 0; i < n; i++) {
            y[i] = yf[i];
        }
        xfree(yf);
        break;
    case NC_DOUBLE:
        ncvarget(cdfid, y_id, start, count, (void *) y);
        break;
    default:
        errmsg("Data type not supported");
        XCFREE(x);
        XCFREE(y);
        ncclose(cdfid);
        return 0;
        break;
    }
    ncclose(cdfid);

    /*
 * initialize stuff for the newly created set
 */
    activateset(gno, setno);
    set_dataset_type(gno, setno, SET_XY);
    setcol(gno, setno, 0, x, n);
    setcol(gno, setno, 1, y, n);

    sprintf(buf, "File %s x = %s y = %s", netcdfname, xvar == NULL ? "Index" : xvar, yvar);
    setcomment(gno, setno, buf);
    
    autoscale_graph(gno, autoscale_onread);
    
    return 1;
}

int write_netcdf(char *fname)
{
    char buf[512];
    int ncid;			/* netCDF id */
    int i, j;
    /* dimension ids */
    int n_dim;
    /* variable ids */
    int x_id, y_id;
    int dims[1];
    long len[1];
    long it = 0;
    double *x, *y, x1, x2, y1, y2;
    ncid = nccreate(fname, NC_CLOBBER);
    ncattput(ncid, NC_GLOBAL, "Contents", NC_CHAR, 11, (void *) "grace sets");
    for (i = 0; i < number_of_graphs(); i++) {
        if (is_graph_active(i)) {
            for (j = 0; j < number_of_sets(i); j++) {
                if (is_set_active(i, j)) {
                    char s[64];

                    sprintf(buf, "g%d_s%d_comment", i, j);
                    ncattput(ncid, NC_GLOBAL, buf, NC_CHAR,
                             strlen(getcomment(i, j)), (void *) getcomment(i, j));

                    sprintf(buf, "g%d_s%d_type", i, j);
                    strcpy(s, set_types(dataset_type(i, j)));
                    ncattput(ncid, NC_GLOBAL, buf, NC_CHAR, strlen(s), (void *) s);

                    sprintf(buf, "g%d_s%d_n", i, j);
                    n_dim = ncdimdef(ncid, buf, getsetlength(i, j));
                    dims[0] = n_dim;
                    getsetminmax(i, j, &x1, &x2, &y1, &y2);
                    sprintf(buf, "g%d_s%d_x", i, j);
                    x_id = ncvardef(ncid, buf, NC_DOUBLE, 1, dims);
                    ncattput(ncid, x_id, "min", NC_DOUBLE, 1, (void *) &x1);
                    ncattput(ncid, x_id, "max", NC_DOUBLE, 1, (void *) &x2);
                    dims[0] = n_dim;
                    sprintf(buf, "g%d_s%d_y", i, j);
                    y_id = ncvardef(ncid, buf, NC_DOUBLE, 1, dims);
                    ncattput(ncid, y_id, "min", NC_DOUBLE, 1, (void *) &y1);
                    ncattput(ncid, y_id, "max", NC_DOUBLE, 1, (void *) &y2);
                }
            }
        }
    }
    ncendef(ncid);
    ncclose(ncid);
    if ((ncid = ncopen(fname, NC_WRITE)) == -1) {
        errmsg("Can't open file.");
        return 1;
    }
    for (i = 0; i < number_of_graphs(); i++) {
        if (is_graph_active(i)) {
            for (j = 0; j < number_of_sets(i); j++) {
                if (is_set_active(i, j)) {
                    len[0] = getsetlength(i, j);
                    x = getx(i, j);
                    y = gety(i, j);
                    sprintf(buf, "g%d_s%d_x", i, j);
                    x_id = ncvarid(ncid, buf);
                    sprintf(buf, "g%d_s%d_y", i, j);
                    y_id = ncvarid(ncid, buf);
                    ncvarput(ncid, x_id, &it, len, (void *) x);
                    ncvarput(ncid, y_id, &it, len, (void *) y);
                }
            }
        }
    }

    ncclose(ncid);
    return 0;
}

#endif				/* HAVE_NETCDF */
