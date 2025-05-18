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
#include <winsock2.h>
#include <winsock.h>
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
#include <QFileInfo>
#include "MainWindow.h"
#include "allWidgets.h"
#include "windowWidgets.h"
#include "compressdecompress.h"
#include "undo_module.h"

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

extern MainWindow * mainWin;
extern frmDeviceSetup * FormDeviceSetup;
extern int new_set_no;
extern int * new_set_nos;
extern int * new_graph_nos;
extern int new_set_blocks;
extern int * new_set_block_size;
extern int ReqUpdateColorSel;
extern graph * g;
extern QList<QFont> stdFontList;
extern Device_entry *device_table;

extern OptionItem fmt_option_items[NUM_FMT_OPTION_ITEMS];

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

extern void readBinaryFromFile(QBuffer & ifi,importSettings & imp_set,double *** data);
extern int LoadFileIntoByteArray(QString FileName,QByteArray & ba);
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
extern void get_text_from_to(char * original,int f,int t,char * text);
extern void generateCommentAndLegendAfterImport(const char * filename,int set,int col1,int col2,char * new_comment,char * new_legend);
extern void generateCommentAndLegendAfterImportV2(const char * filename,QStringList target_cols,QStringList target_col_origins,char * new_comment,char * new_legend);

extern void write_qtGrace_Additions_to_Bytearray(QByteArray & ba);
extern void copy_LaTeX_to_Grace(void);

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

static int uniread(FILE *fp, int load_type, const char *label);

extern void append_to_storage_single(int * l,int ** storage,int new_entry);

void postprocess_diadem_header(struct DIAdem_Header & header,struct importSettings & bin_import,int & nr_of_new_sets,int ** n_gnos,int ** n_snos);
int postprocess_bin_import_data(struct importSettings & imp_set,int & nr_of_new_sets,int ** n_gnos,int ** n_snos);

QString path_we_currently_save_to;
QByteArray output_parameter_array;
QString result_params;
QBuffer result_buffer;
extern imageinfo bg_fill_image;//for the plot
extern imageinfo bg_box_fill_image;//default for boxes
extern imageinfo bg_ellipse_fill_image;//default for ellipses

#ifdef DEBUG_OUT_LOG
extern ofstream debug_out;
#endif

#if READ_UTF8_ONLY == 0
extern QTextCodec * FileCodec;
extern QTextCodec * FileCodecSave;
#endif
extern void update_timestamp(void);
#ifdef __cplusplus
extern "C" {
#endif
extern void prepare_strings_for_saving(void);
extern void resume_strings_after_load_or_save(void);
#ifdef __cplusplus
}
#endif

//warning: string=-2 --> use this with format+2
//formats that have ordering-numbers that differ by more than 20 are not compatible at all
static int format_ordering[FORMAT_YYDYHMS+3]
={-80,-40,
  0,1,2,30,60,4,3,90,120,150,
  180,210,240,270,300,330,301,360,361,-2,
  390,420,450,480,510,540,570,600,630,660,
  690,720,-1,750};

QString correct_file_name(char * fn)
{
QFileInfo f_inf(fn);
    if (f_inf.exists()==false)//oh-oh the file does not exist!? Maybe there are unusual characters in the name-string...
    {
    //qDebug() << "fn=" << fn << "does NOT EXIST!";
    QString test_string1=QString::fromLocal8Bit(fn);
    f_inf.setFile(test_string1);
    //qDebug() << "string1=" << test_string1 << "exists?" << f_inf.exists();
    if (f_inf.exists()) return f_inf.absoluteFilePath();
    QString test_string2=QString::fromUtf8(fn);
    f_inf.setFile(test_string2);
    //qDebug() << "string2=" << test_string2 << "exists?" << f_inf.exists();
    if (f_inf.exists()) return f_inf.absoluteFilePath();
    f_inf.setFile(QString(fn));
    }
return f_inf.absoluteFilePath();
}

bool is_windows_path(const char * path)//returns true if the path seems to be an absolute windows path like c:\abc
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

bool is_path_absolute(const char * path)//returns true if path is an absolute path
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
return fi.completeBaseName();
}

inline QString getPathOnly(QString filepath)
{
QFileInfo fi(filepath);
return fi.absolutePath();
}

inline QString getSuffixOnly(QString filepath)
{
QFileInfo fi(filepath);
return fi.suffix();
}

void save_qtGrace_Additions(FILE * pp)
{
char * sp=new char[2];
(void)qtGrace_Additions_String(&sp);
fprintf(pp,"%s",sp);
delete[] sp;
}

void write_qtGrace_Additions_to_Bytearray(QByteArray & ba)
{
char * sp=new char[2];
(void)qtGrace_Additions_String(&sp);
ba=QByteArray(sp);
delete[] sp;
}

int qtGrace_Additions_String(char ** sp)
{
    QString pstring;
    int i,j;
    boxtype b;
    ellipsetype e;
    linetype l;
    plotstr st;
    QString tmp_info_string;
    char text[512];
    char * pp=new char[1024];//enough space for one line
    pstring.clear();//the complete string with all parameters
    for (i = 0; i < number_of_boxes(); i++)
    {
        get_graph_box(i, &b);
        if (b.active == TRUE)
        {
            sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: BOX %d ROT %d\n",i,b.rot);
            pstring+=QString(pp);
            sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: BOX %d ALPHA %d %d\n",i,b.alpha,b.fillalpha);
            pstring+=QString(pp);
            createImageInfoText(b.fillimage,tmp_info_string,path_we_currently_save_to);
            sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: BOX %d IMAGE %d ",i,b.filltype);
            pstring+=QString(pp)+tmp_info_string+QString("\n");
        }
    }
    for (i = 0; i < number_of_ellipses(); i++)
    {
        get_graph_ellipse(i, &e);
        if (e.active == TRUE)
        {
            sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: ELLIPSE %d ROT %d\n",i,e.rot);
            pstring+=QString(pp);
            sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: ELLIPSE %d ALPHA %d %d\n",i,e.alpha,e.fillalpha);
            pstring+=QString(pp);
            createImageInfoText(e.fillimage,tmp_info_string,path_we_currently_save_to);
            sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: ELLIPSE %d IMAGE %d ",i,e.filltype);
            pstring+=QString(pp)+tmp_info_string+QString("\n");
        }
    }
    for (i=0; i < number_of_lines(); i++)
    {
        get_graph_line(i,&l);
        if (l.active == TRUE)
        {
            sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: LINE %d ALPHA %d\n",i,l.alpha);
            pstring+=QString(pp);
        }
    }
    for (i=0; i < number_of_strings(); i++)
    {
        get_graph_string(i,&st);
        if (st.active == TRUE)
        {
            sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: STRING %d ALPHA %d\n",i,st.alpha);
            pstring+=QString(pp);
            sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: STRING %d ALIGN %d %d\n",i,st.align,st.master_align);
            pstring+=QString(pp);
        }
    }
    for (i=0; i < MAXREGION; i++)
    {
        sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: REGION %d ALPHA %d\n",i,rg[i].alpha);
        pstring+=QString(pp);
    }
    sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: PLOT_ALPHA %d %d\n",getbgalpha(),timestamp.alpha);
    pstring+=QString(pp);
    createImageInfoText(bg_fill_image,tmp_info_string,path_we_currently_save_to);
    sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: PLOT_IMAGE %d ",getbgfill());
    pstring+=QString(pp)+tmp_info_string+QString("\n");
    for (i=0; i < number_of_graphs(); i++)
    {
    sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: GRAPH_ALPHA G %d %s\n",i,create_list_of_arguments(7, g[i].labs.title.alpha,g[i].labs.stitle.alpha,g[i].f.pen.alpha,g[i].f.fillpen.alpha,g[i].l.alpha,g[i].l.boxpen.alpha,g[i].l.boxfillpen.alpha));
    pstring+=QString(pp);
    sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: ALIGN_TITLE G %d %d %d\n",i,g[i].labs.title.align,g[i].labs.stitle.align);
    pstring+=QString(pp);
    sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: ALIGN_LEGEND G %d %d\n",i,g[i].l.align);
    pstring+=QString(pp);
        for (j=0; j < MAXAXES; j++)
        {
            sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: AXIS_ALPHA G %d A %d %s\n",i,j,create_list_of_arguments(5, g[i].t[j]->tl_alpha,g[i].t[j]->label.alpha,g[i].t[j]->t_drawbaralpha,g[i].t[j]->props.alpha,g[i].t[j]->mprops.alpha));
            pstring+=QString(pp);
            sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: AXIS_TL_ALIGN G %d A %d %d\n",i,j,g[i].t[j]->tl_align);
            pstring+=QString(pp);
            sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: AXIS_LABEL_ALIGN G %d A %d %d\n",i,j,g[i].t[j]->label.align);
            pstring+=QString(pp);
        }
        for (j=0; j < number_of_sets(i); j++)
        {
            sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d POLYGONEBASESET %d\n",i,j,g[i].p[j].polygone_base_set);
            pstring+=QString(pp);
            sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d SHOWERRORBARINLEGEND %d\n",i,j,g[i].p[j].errbar.show_in_legend);
            pstring+=QString(pp);
            sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d CONNECTERRORBARS %d\n",i,j,g[i].p[j].errbar.connect_bars);
            pstring+=QString(pp);
            sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d IGNOREINAUTOSCALE %d\n",i,j,g[i].p[j].ignore_in_autoscale);
            pstring+=QString(pp);
            sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d ALPHA_CHANNELS %s\n",i,j,create_list_of_arguments(6, g[i].p[j].linepen.alpha,g[i].p[j].setfillpen.alpha,g[i].p[j].sympen.alpha,g[i].p[j].symfillpen.alpha,g[i].p[j].avalue.alpha,g[i].p[j].errbar.pen.alpha));
            pstring+=QString(pp);
            sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d ALIGN_ANNOTATE_VALUES %d\n",i,j,g[i].p[j].avalue.align);
            pstring+=QString(pp);
            text[0]='\0';
            create_format_preferences_string(i,j,text);
            if (text[0]!='\0')
            {
            sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d FORMAT_PREFERENCES %s\n",i,j,text);
            pstring+=QString(pp);
            }
        }
    sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: AUTOATTACH G %d %d %d\n",i,g[i].l.autoattach,g[i].l.autoattachG);
    pstring+=QString(pp);
        if (g[i].type==GRAPH_POLAR2)
        {
        sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: POLAR_V2 G %d %g %g\n",i,g[i].phi0,g[i].roffset);
        pstring+=QString(pp);
        }
    sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: TITLE_SHIFT G %d %g %g\n",i,g[i].labs.shift_title.x,g[i].labs.shift_title.y);
    pstring+=QString(pp);
    sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: SUBTITLE_SHIFT G %d %g %g\n",i,g[i].labs.shift_subtitle.x,g[i].labs.shift_subtitle.y);
    pstring+=QString(pp);
        for (int j=0;j<g[i].ws_top;j++)
        {
            if (g[i].ws[j].w_name!=NULL && strlen(g[i].ws[j].w_name)>0)
            {
            sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: VIEWPORT_NAME %d %d \"%s\"\n",i,j,g[i].ws[j].w_name);
            pstring+=QString(pp);
            }
        }
    sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: FIXPOINT G %d SHOW_IN_FILE %d\n",i,g[i].locator.plot_in_file);
    pstring+=QString(pp);
    }
        if (stdFontList.length()<1)
        {
        sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: QTFONT_RESET 0\n");//we want the standard-fonts for the Qt-Fonts
        pstring+=QString(pp);
        }
        else
        {
        sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: QTFONT_RESET 1\n");//we ask for a cleared font-list to insert new fonts
        pstring+=QString(pp);
        }
    for (i=0;i<stdFontList.length();i++)
    {
        sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: QTFONT \"%s\"\n",stdFontList.at(i).toString().toLocal8Bit().constData());
        pstring+=QString(pp);
    }
    //sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: ENCODING \"%s\"\n",FileCodec->name().constData());
    ///starting with v0.2.7 the output will always be UTF-8
    sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: ENCODING \"UTF-8\"\n");
    pstring+=QString(pp);
    sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: UNIVERSAL_FONT_SIZE_FACTOR %.4f\n",universal_font_size_factor);
    pstring+=QString(pp);
    sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: TIMESTAMP_PATH %d\n",timestamp.path);
    pstring+=QString(pp);
    QString pat_lens,pat_styles;
    generate_strings_for_linestyles(pat_lens,pat_styles);
    sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: LINESTYLES %d %s %s\n",nr_of_current_linestyles,pat_lens.toLatin1().constData(),pat_styles.toLatin1().constData());
    pstring+=QString(pp);
    if (current_compression_method>0)
    {
    sprintf(pp,"#QTGRACE_ADDITIONAL_PARAMETER: DATA_COMPRESSED %d\n",current_compression_method);
    pstring+=QString(pp);
    }
    delete[] pp;
int len=pstring.length();
delete[] *sp;
(*sp)=new char[len+1];
//strcpy(*sp,pstring.toLatin1().constData());
strcpy(*sp,pstring.toLocal8Bit().constData());
return len;
}

int help_read_in_quotation(char * text,char * in_quotation)
{
int len=strlen(text);
int first_pos,last_pos;
first_pos=last_pos=-1;
for (int i=0;i<len;i++)
{
    if (text[i]=='"')
    {
    first_pos=i;
    break;
    }
}
if (first_pos>=0)
{
    for (int i=first_pos+1;i<=len;i++)
    {
        if (text[i]=='"' || text[i]=='\0')
        {
        last_pos=i;
        break;
        }
    }
}
else
return RETURN_FAILURE;
    if (last_pos>=0)
    get_text_from_to(text,first_pos,last_pos,in_quotation);
    else
    return RETURN_FAILURE;
return RETURN_SUCCESS;
}

void parse_qtGrace_Additions(char * s)
{
    int * lens_of_styles;
    char ** line_styles;
    QString style_lens,style_pats,tmp_string;
    char c='\0';
    char c2='\0';
    int read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: %c%c",&c,&c2);
    int data[8];
    int d_col_f[MAX_SET_COLS],d_col_p[MAX_SET_COLS];
    char sdata[256];
    char sdata2[256];
    double ddata;
    double ddata2[2];
    int * args=NULL;
    QString errortext;
//qDebug() << "ParseQtGraceAdditions=" << s << " READ=" << read_data;
    if (read_data>0)
    {
        //cout << "s=#" << s << "# --> read_data=" << read_data << " c=" << c << endl;
        switch (c)
        {
        case 'A':
            if (c2=='X')
            {
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: AXIS_ALPHA G %d A %d %s",data,data+1,sdata);
            //qDebug() << "Read(AXIS_ALPHA) s=" << s << "read_data=" << read_data;
                if (read_data>0 && is_valid_axis(data[0],data[1]))
                {
                    args=new int[2];
                data[2]=process_list_of_arguments(sdata,&args);
                if (data[2]==5)
                {
                g[data[0]].t[data[1]]->tl_alpha=args[0];
                g[data[0]].t[data[1]]->label.alpha=args[1];
                g[data[0]].t[data[1]]->t_drawbaralpha=args[2];
                g[data[0]].t[data[1]]->props.alpha=args[3];
                g[data[0]].t[data[1]]->mprops.alpha=args[4];
                }
                else
                {
                qDebug() << "Invalid Number of Arguments for axis_alpha: " << sdata;
                }
                    delete[] args;
                    args=NULL;
                }
                else
                {
                read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: AXIS_TL_ALIGN G %d A %d %d",data,data+1,data+2);
                //qDebug() << "Read(AXIS_TL_ALIGN) s=" << s << "read_data=" << read_data;
                    if (read_data>0 && is_valid_axis(data[0],data[1]))
                    {
                    g[data[0]].t[data[1]]->tl_align=data[2];
                    //qDebug() << "setting tl_align=" << data[2];
                    }
                    else
                    {
                    read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: AXIS_LABEL_ALIGN G %d A %d %d",data,data+1,data+2);
                    //qDebug() << "Read(AXIS_LABEL_ALIGN) s=" << s << "read_data=" << read_data;
                        if (read_data>0 && is_valid_axis(data[0],data[1]))
                        {
                        g[data[0]].t[data[1]]->label.align=data[2];
                        //qDebug() << "setting label align=" << data[2];
                        }
                        else
                        {
                        qDebug() << "Invalid Additional Parameter:" << s;
                        }
                    }
                }
            }
            else if (c2=='U')
            {
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: AUTOATTACH G %d %d %d",data,data+1,data+2);
                if (read_data>0 && is_valid_gno(data[0]))
                {
                    g[data[0]].l.autoattach=data[1];
                    g[data[0]].l.autoattachG=data[2];
                }
            }
            else
            {
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: ALIGN_TITLE G %d %d %d",data,data+1,data+2);
                if (read_data>2)
                {
                    if (is_valid_gno(data[0]))
                    {
                    g[data[0]].labs.title.align=data[1];
                    g[data[0]].labs.stitle.align=data[2];
                    }
                }
                else
                {
                read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: ALIGN_LEGEND G %d %d",data,data+1);
                    if (read_data>1)
                    {
                        if (is_valid_gno(data[0]))
                        {
                        g[data[0]].l.align=data[1];
                        }
                    }
                }
            }
            break;
        case 'P':
            if (c2=='O')
            {
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: POLAR_V2 G %d %lf %lf",data,ddata2+0,ddata2+1);
                if (read_data>0 && is_valid_gno(data[0]))
                {
                    g[data[0]].phi0=ddata2[0];
                    g[data[0]].roffset=ddata2[1];
                }
            }
            else if (c2=='L')
            {
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: PLOT_ALPHA %d %d",data,data+1);
                if (read_data>0)
                {
                setbgalpha(data[0]);
                timestamp.alpha=data[1];
                }
                else//could be PLOT_IMAGE
                {
                read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: PLOT_IMAGE %d %d ",data,data+2);
                    if (read_data>0)
                    {
                    tmp_string=QString(s);
                    data[1]=tmp_string.indexOf(QString("IMAGE"))+6;
                    data[1]=tmp_string.indexOf(QString(" "),data[1])+1;
                    tmp_string=tmp_string.mid(data[1]);
                    parseImageInfoText(bg_fill_image,tmp_string,path_we_currently_save_to);
                    setbgfill(data[0]);
                    //qDebug() << "s=" << s << " --> bgfill=" << data[0];
                    }
                }
            }
            break;
        case 'B':
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: BOX %d ROT %d",data,data+1);
            if (read_data==2)
            {
                if (is_valid_box(data[0]))
                {
                boxes[data[0]].rot=data[1];
                }
            }
            else
            {
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: BOX %d ALPHA %d %d",data,data+1,data+2);
                if (read_data==3)
                {
                    if (is_valid_box(data[0]))
                    {
                    boxes[data[0]].alpha=data[1];
                    boxes[data[0]].fillalpha=data[2];
                    }
                //qDebug() << "box-alpha: " << data[0] << " - " << data[1] << " - " << data[2];
                }
                else
                {
                read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: BOX %d IMAGE %d ",data,data+2);
                    if (read_data>0)
                    {
                        if (is_valid_box(data[0]))
                        {
                        tmp_string=QString(s);
                        data[1]=tmp_string.indexOf(QString("IMAGE"))+6;
                        data[1]=tmp_string.indexOf(QString(" "),data[1])+1;
                        tmp_string=tmp_string.mid(data[1]);
                        parseImageInfoText(boxes[data[0]].fillimage,tmp_string,path_we_currently_save_to);
                        boxes[data[0]].filltype=data[2];
                        }
                    }
                    /*else
                    {
                    read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: BOX %d GNO %d",data,data+1);
                        if (read_data==2)
                        {
                            if (is_valid_box(data[0]))
                            {
                            boxes[data[0]].gno=data[1];
                            }
                        }
                    }*/
                }
            }
            break;
        case 'E':
            if (c2=='L')
            {
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: ELLIPSE %d ROT %d",data,data+1);
            //qDebug() << "Ellipse READ=" << read_data;
                if (read_data==2)
                {
                    if (is_valid_ellipse(data[0]))
                    {
                    ellip[data[0]].rot=data[1];
                    }
                }
                else
                {
                read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: ELLIPSE %d ALPHA %d %d",data,data+1,data+2);
                    //qDebug() << "Ellipse READ=" << read_data;
                    if (read_data==3)
                    {
                        if (is_valid_ellipse(data[0]))
                        {
                        ellip[data[0]].alpha=data[1];
                        ellip[data[0]].fillalpha=data[2];
                        }
                        //qDebug() << "ellipse-alpha: " << data[0] << " - " << data[1] << " - " << data[2];
                    }
                    else
                    {
                    read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: ELLIPSE %d IMAGE %d ",data,data+2);
                        if (read_data>0)
                        {
                            if (is_valid_ellipse(data[0]))
                            {
                            tmp_string=QString(s);
                            data[1]=tmp_string.indexOf(QString("IMAGE"))+6;
                            data[1]=tmp_string.indexOf(QString(" "),data[1])+1;
                            tmp_string=tmp_string.mid(data[1]);
                            parseImageInfoText(ellip[data[0]].fillimage,tmp_string,path_we_currently_save_to);
                            ellip[data[0]].filltype=data[2];
                            }
                        }
                        /*else
                        {
                        read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: ELLIPSE %d GNO %d",data,data+1);
                            if (read_data>0)
                            {
                                if (is_valid_ellipse(data[0]))
                                {
                                ellip[data[0]].gno=data[1];
                                }
                            }
                        }*/
                    }
                }
            }
            else//'N' --> Encoding
            {
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: ENCODING \"%s\"",sdata);
                data[0]=help_read_in_quotation(s,sdata2);
#if READ_UTF8_ONLY == 1
                if (data[0]!=RETURN_SUCCESS)
                {
                    if (sdata[strlen(sdata)-1]=='"') sdata[strlen(sdata)-1]='\0';
                    if (strcmp(sdata,"UTF-8")!=0)
                    {
                    errortext=QObject::tr("Encoding ") + QString(sdata) + QObject::tr(" not supported! Only UTF-8 allowed.");
                    errmsg(errortext.toLocal8Bit().constData());
                    }
                }
                else
                {
                    if (strcmp(sdata2,"UTF-8")!=0)
                    {
                    errortext=QObject::tr("Encoding ") + QString(sdata2) + QObject::tr(" not supported! Only UTF-8 allowed.");
                    errmsg(errortext.toLocal8Bit().constData());
                    }
                }
                strcpy(new_encoding_name,"UTF-8");

#else
                if (data[0]!=RETURN_SUCCESS)
                {
                    if (sdata[strlen(sdata)-1]=='"') sdata[strlen(sdata)-1]='\0';
                    strcpy(new_encoding_name,sdata);
                    /// Todo: sdata verarbeiten ...
                    FileCodec=QTextCodec::codecForName(sdata);
                }
                else
                {
                    strcpy(new_encoding_name,sdata2);
                    /// Todo: sdata verarbeiten ...
                    FileCodec=QTextCodec::codecForName(sdata2);
                }
#endif
            }
            break;
        case 'G':
            if (c2=='R')
            {
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: GRAPH_ALPHA G %d %s",data,sdata);
                if (read_data>0 && is_valid_gno(data[0]))
                {
                    args=new int[2];
                data[1]=process_list_of_arguments(sdata,&args);
                if (data[1]==7)
                {
                g[data[0]].labs.title.alpha=args[0];
                g[data[0]].labs.stitle.alpha=args[1];
                g[data[0]].f.pen.alpha=args[2];
                g[data[0]].f.fillpen.alpha=args[3];
                g[data[0]].l.alpha=args[4];
                g[data[0]].l.boxpen.alpha=args[5];
                g[data[0]].l.boxfillpen.alpha=args[6];
                }
                else
                {
                qDebug() << "Invalid Number of Arguments for graph_alpha: " << sdata;
                }
                    delete[] args;
                    args=NULL;
                }
            }
            else
            {
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d POLYGONEBASESET %d",data,data+1,data+2);
            if (read_data>2)
            {
                if (is_valid_setno(data[0],data[1]))
                g[data[0]].p[data[1]].polygone_base_set=data[2];
            }
            else
            {
                read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d SHOWERRORBARINLEGEND %d",data,data+1,data+2);
                if (read_data>2)
                {
                    if (is_valid_setno(data[0],data[1]))
                    g[data[0]].p[data[1]].errbar.show_in_legend=data[2];
                }
                else
                {
                    read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d CONNECTERRORBARS %d",data,data+1,data+2);
                    if (read_data>2)
                    {
                        if (is_valid_setno(data[0],data[1]))
                        g[data[0]].p[data[1]].errbar.connect_bars=data[2];
                    }
                    else
                    {
                        read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d IGNOREINAUTOSCALE %d",data,data+1,data+2);
                        if (read_data>2)
                        {
                            if (is_valid_setno(data[0],data[1]))
                            g[data[0]].p[data[1]].ignore_in_autoscale=data[2];
                        }
                        else
                        {
                            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d ALPHA_CHANNELS %s",data,data+1,sdata);
                            if (read_data>2)
                            {
                                if (is_valid_setno(data[0],data[1]))
                                {
                                args=new int[2];
                                data[2]=process_list_of_arguments(sdata,&args);
                                if (data[2]==6)
                                {
                                g[data[0]].p[data[1]].linepen.alpha=args[0];
                                g[data[0]].p[data[1]].setfillpen.alpha=args[1];
                                g[data[0]].p[data[1]].sympen.alpha=args[2];
                                g[data[0]].p[data[1]].symfillpen.alpha=args[3];
                                g[data[0]].p[data[1]].avalue.alpha=args[4];
                                g[data[0]].p[data[1]].errbar.pen.alpha=args[5];
                                }
                                else
                                {
                                qDebug() << "Invalid Number of Arguments for set-alpha-channels: " << sdata;
                                }
                                delete[] args;
                                args=NULL;
                                }
                            }
                            else
                            {
                                read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d ALIGN_ANNOTATE_VALUES %d",data,data+1,data+2);
                                if (read_data>2)
                                {
                                    if (is_valid_setno(data[0],data[1]))
                                    {
                                        g[data[0]].p[data[1]].avalue.align=data[2];
                                    }
                                }
                                else
                                {
                                    read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d FORMAT_PREFERENCES %s",data,data+1,sdata);
                                    if (read_data>2)
                                    {
                                    parse_format_preferences_string(sdata,d_col_f,d_col_p);
                                        if (sdata[0]!='\0' && is_valid_setno(data[0],data[1]))
                                        {
                                            for (int kkk=0;kkk<MAX_SET_COLS;kkk++)
                                            {
                                            g[data[0]].p[data[1]].pref_col_format[kkk]=d_col_f[kkk];
                                            g[data[0]].p[data[1]].pref_col_prec[kkk]=d_col_p[kkk];
                                            }
                                        }
                                    }
                                    else
                                    {

                                    }
                                }
                            }
                        }
                    }
                }
            }
            }
            break;
        case 'Q':
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: QTFONT_RESET %d",data);
            if (read_data>0)
            {
                if (data[0]==0)//QTFONT_RESET 0 --> Reset fonts and insert the current Grace-fonts
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
                data[0]=help_read_in_quotation(s,sdata2);
                    if (qtfonts_have_been_reset==false)
                    {
                        reset_stdFontList(false);
                        qtfonts_have_been_reset=true;
                    }
                    QFont stfont;
                    if (data[0]!=RETURN_SUCCESS)
                    stfont.fromString(QString(sdata));
                    else
                    stfont.fromString(QString(sdata2));
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
            else
            {
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: TITLE_SHIFT G %d %lf %lf",data,ddata2,ddata2+1);
                if (read_data>2)
                {
                g[data[0]].labs.shift_title.x=ddata2[0];
                g[data[0]].labs.shift_title.y=ddata2[1];
                }
            }
            break;
        case 'S':
            if (c2=='U')
            {
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: SUBTITLE_SHIFT G %d %lf %lf",data,ddata2,ddata2+1);
                if (read_data>2)
                {
                g[data[0]].labs.shift_subtitle.x=ddata2[0];
                g[data[0]].labs.shift_subtitle.y=ddata2[1];
                }
            }
            else if (c2=='T')
            {
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: STRING %d ALPHA %d",data,data+1);
            //qDebug() << "s=" << s << "Alpha; read_data=" << read_data;
                if (read_data>1)
                {
                    if (is_valid_string(data[0]))
                    {
                    pstr[data[0]].alpha=data[1];
                    }
                }
                else
                {
                read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: STRING %d ALIGN %d %d",data,data+1,data+2);
                    if (read_data>2)
                    {
                        if (is_valid_string(data[0]))
                        {
                        pstr[data[0]].align=data[1];
                        pstr[data[0]].master_align=data[2];
                        }
                    }
                    else
                    {
                    read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: STRING %d ALIGN %d",data,data+1);
                    //qDebug() << "Alignment read String " << data[0] << " Align=" << data[1] << " Master-Align=" << data[2];
                        if (read_data>1)
                        {
                            if (is_valid_string(data[0]))
                            {
                            pstr[data[0]].align=data[1];
                            pstr[data[0]].master_align=0;
                            }
                        }
                    }
                }
            }
            break;
        case 'L':
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: LINESTYLES %d %s %s",data,sdata,sdata2);
            if (read_data==3)
            {
            //cout << "linestyles=" << data[0] << " slen=" << sdata << " patterns=" << sdata2 << endl;
            style_lens=QString(sdata);
            style_pats=QString(sdata2);
            parse_strings_for_linestyles(data[0],style_lens,style_pats,&lens_of_styles,&line_styles);
            copy_line_style_patterns_to_target(data[0],lens_of_styles,line_styles,2);//copy linestyle to file-settings
            //use the styles from the file as current styles and generate icons and dash-patterns
            copy_line_style_patterns_to_current(data[0],lens_of_styles,line_styles);
            }
            else
            {
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: LINE %d ALPHA %d",data,data+1);
                if (read_data==2)
                {
                    if (is_valid_line(data[0]))
                    {
                    lines[data[0]].alpha=data[1];
                    }
                }
            }
            break;
        case 'V':
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: VIEWPORT_NAME %d %d \"%s\"",data,data+1,sdata);
            if (read_data>0)
            {
            data[2]=help_read_in_quotation(s,sdata2);
                if (data[2]!=RETURN_SUCCESS)
                g[data[0]].ws[data[1]].w_name=copy_string(g[data[0]].ws[data[1]].w_name,sdata);
                else
                g[data[0]].ws[data[1]].w_name=copy_string(g[data[0]].ws[data[1]].w_name,sdata2);
            }
            break;
        case 'D':
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: DATA_COMPRESSED %d",data);
            if (read_data>0)
            {
            current_compression_method=data[0];
            }
            break;
        case 'F':
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: FIXPOINT G %d SHOW_IN_FILE %d",data,data+1);
                if (read_data>1 && is_valid_gno(data[0]))
                {
                g[data[0]].locator.plot_in_file=data[1];
                }
            break;
        case 'R':
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: REGION %d ALPHA %d",data,data+1);
                if (read_data>1 && data[0]>=0 && data[0]<MAXREGION)
                {
                rg[data[0]].alpha=data[1];
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
#ifndef WINDOWS_SYSTEM
    struct timeval timeout;
#endif
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
#ifndef WINDOWS_SYSTEM
        timeout.tv_sec = remaining / 1000;
        timeout.tv_usec = 1000l * (remaining % 1000);
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
if (redirect_output_ba==1)
{
    slen = result_buffer.readLine(s,size);
    if (slen <= 0) s=NULL;
}
else
{
    s = fgets(s, size, stream);
}
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
                    overwriteAnswerNo=TRUE;
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

const char *grace_path(const char *fn)
{
static char buf[GR_MAXPATHLEN];
struct stat statb;
QFileInfo fi1(fn);
bool is_absolute_path=fi1.isAbsolute();

    if (fn == NULL) {
        return NULL;
    } else {
        strcpy(buf, fn);

        if(strlen(fn)>2)
        {
            //if (is_windows_path(fn)==true)
            if (is_absolute_path==true)
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
        ///sprintf(buf,"%s/../%s",qt_grace_share_dir,fn);

        // The original trick was sprintf(buf,"%s/../%s",qt_grace_share_dir,fn);
        // The trick does not work because "stat" does not accept "/../" on Windows
        QDir dir_above_bin(qt_grace_share_dir);  // XXXqtgrace/bin
        //dir_above_bin.cdUp();  // becomes XXXqtgrace/
        /// share-dir is already the main-directory (not the bin-directory)
/*#ifdef MAC_SYSTEM
        dir_above_bin.cdUp();
        dir_above_bin.cdUp();
        dir_above_bin.cdUp();
#endif*/
        QString qs_dir_above_bin=dir_above_bin.path();
        qs_dir_above_bin+=QDir::separator();
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

const char *grace_exe_path(const char *fn)
{
static char buf[GR_MAXPATHLEN];
const char *cp;
    
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

int look_for_file(char * fn)//tries to find the file "fn"; returns TRUE if file exists and FALSE otherwise; fn is modified to account for a relative path
{
QFileInfo * file1=new QFileInfo(QString::fromLocal8Bit(fn));
bool exists=file1->exists();
char buf[GR_MAXPATHLEN + 64];
if (strcmp(fn,"stdin")==0) return TRUE;
/*errmsg("Looking for:");
errmsg(fn);
errmsg("Absolute?!");
errmsg(file1->absoluteFilePath().toLocal8Bit().constData());*/
//qDebug() << "look for file: working_dir=" << get_workingdir() << "file=" << fn << "Exists=" << exists;
if (exists==false)
{
    //strcpy(buf,file1->fileName().toLocal8Bit());
    strcpy(buf,fn);
//qDebug() << "does not exist - look for file: buf=" << buf;
    QFileInfo * file2=NULL;
    if (strcmp(buf,"gracerc.user")==0 || strcmp(buf,"gracerc")==0)//a special case: look in home-dir, then in share-dir
    {
        file2=new QFileInfo(QString(user_home_dir)+QDir::separator()+QString::fromLocal8Bit(buf));
        /*errmsg("In Home Dir:");
        errmsg(file2->absoluteFilePath().toLocal8Bit().constData());*/
        exists=file2->exists();
        if (exists==false)//not in home-dir --> look in share-dir
        {
            if (file2)
            delete file2;
        file2=new QFileInfo(QString(qt_grace_share_dir)+QDir::separator()+QString::fromLocal8Bit(buf));
        /*errmsg("In Share Dir:");
        errmsg(file2->absoluteFilePath().toLocal8Bit().constData());*/
        exists=file2->exists();
            if (exists==true)//we found it in share-dir --> replace original file name with new absolute file
            {
                sprintf(fn,"%s%c%s",qt_grace_share_dir,QDir::separator().toLatin1(),buf);
            }
        }
        else//exists==true
        {//found in home-dir --> replace original file name with new absolute file
            sprintf(fn,"%s%c%s",user_home_dir,QDir::separator().toLatin1(),buf);
        }
    }
    else//not gracerc/gracerc.user
    {//first: look in share-dir
        file2=new QFileInfo(QString(qt_grace_share_dir)+QDir::separator()+QString::fromLocal8Bit(buf));
        /*errmsg("In Share Dir:");
        errmsg(file2->absoluteFilePath().toLocal8Bit().constData());*/
        exists=file2->exists();
        if (exists==true)
        {
            sprintf(fn,"%s%c%s",qt_grace_share_dir,QDir::separator().toLatin1(),buf);
        }
        else//does not exist in share-dir --> look in working-directory
        {
            if (file2)
            delete file2;
        file2=new QFileInfo(QString(get_workingdir())+QString::fromLocal8Bit(buf));
        /*errmsg("In Working Dir:");
        errmsg(file2->absoluteFilePath().toLocal8Bit().constData());*/
        exists=file2->exists();
            if (exists==true)//found in working-dir --> replace original file name with new absolute file
            {
                sprintf(fn,"%s%s",get_workingdir(),buf);
            }
        }
    }
    if (file2)
    delete file2;
}
    if (file1)
    delete file1;
if (exists==true)
{
QFileInfo fi1(QString::fromLocal8Bit(fn));//originally only fn
//errmsg("Exists, but not absolute.");
//QFileInfo fi1(fn);
//qDebug() << "file exists: fn=" << fn << "canoncialPath=" << fi1.canonicalFilePath();
    if (fi1.isAbsolute()==false)
    {
    strcpy(fn,fi1.canonicalFilePath().toLocal8Bit().constData());
    }
}
/*errmsg("Finally:");
errmsg(fn);*/
return exists==true?TRUE:FALSE;
}

/* open a file for read */
FILE *grace_openr(const char *fn2, int src)
{
    struct stat statb;
    const char *tfn;
static char buf[GR_MAXPATHLEN + 50];
static char fn[GR_MAXPATHLEN + 50];
    //Qt-stuff for checking file existence
    //QString realFileName=FileCodec->toUnicode(fn2);
    //QFile * file1=new QFile(realFileName);
    bool exists;
    /// QFile * file1=new QFile(QString(fn2));
    /// bool exists=file1->exists();

    /*QString realFileName=QString(fn2);
    QFileInfo fi1(realFileName);
    bool exists2=fi1.exists();
    bool absolute=fi1.isAbsolute();
qDebug() << QString(fn2) << ": exists2=" << exists2 << " absolute=" << absolute;*/

//cout << "start: open{fn2}=#" << fn2 << "# exists=" << exists << endl;
/*cout << "working_dir=" << get_workingdir() << endl;
cout << "wdir1=" << wdir1 << endl;*/


/*
    if (exists==true)
    {
        strcpy(fn,fn2);
    }
    else
    {
        strcpy(buf,file1->fileName().toLocal8Bit());
        QFile * file2;
        if (strcmp(buf,"gracerc.user")==0)//a special case: look in home-dir, then in share-dir
        {
            file2=new QFile(QString(user_home_dir)+QDir::separator()+QString(buf));//realFileName);//QString(buf));
            exists=file2->exists();
            if (exists==false)
            {
                delete file2;
                file2=new QFile(QString(qt_grace_share_dir)+QDir::separator()+QString(buf));//realFileName);//QString(buf));
                exists=file2->exists();
                if (exists==true)
                {
                    sprintf(fn,"%s/%s",qt_grace_share_dir,buf);
                }
            }
            else//exists==true
            {//found in dir of executable
                sprintf(fn,"%s/%s",user_home_dir,buf);
            }
        }
        else
        {
            file2=new QFile(QString(qt_grace_share_dir)+QDir::separator()+QString(fn2));
            exists=file2->exists();
//cout << "file2=" << file2->fileName().toLocal8Bit().constData() << " exists=" << exists << endl;
            if (exists==true)
            {
                sprintf(fn,"%s/%s",qt_grace_share_dir,fn2);
            }
            else
            {
            delete file2;
            file2=new QFile(QString(wdir1)+QDir::separator()+QString(fn2));
            exists=file2->exists();
//cout << "file2=" << file2->fileName().toLocal8Bit().constData() << " exists=" << exists << endl;
                if (exists==true)
                {
                    sprintf(fn,"%s/%s",wdir1,fn2);
                }
            }
        }
        if (exists==false)//nothing found, use original file name and tell users that this did not work
        {
            strcpy(fn,fn2);
        }
        delete file2;
    }
    delete file1;
*/

    strcpy(fn,fn2);
    exists=(bool)look_for_file(fn);

    //qDebug() << "open{fn2}=" << fn2;
    //qDebug() << "open{fn}=" << fn;

    if (!fn2 || !fn[0])
    {
        errmsg("No file name given");
        return NULL;
    }
    else if (exists==false)
    {
        errmsg("File does not exist");
        errmsg(fn);
        return NULL;
    }
    switch (src) {
    case SOURCE_DISK:
        tfn = grace_path(fn);
//cout << "open{tfn}=" << tfn << endl;
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
    return NULL;
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
        sprintf(dummy,"%s/%s",qt_grace_share_dir,plfile);
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
        //qDebug() << "getparms=" << linebuf;
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

static int uniread(FILE *fp, int load_type, const char *label)
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
    int * maj_new_set_blocks=NULL;
    int set_block_count=0;

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
                append_to_storage(&set_block_count,&maj_new_set_blocks,1,&new_set_no);//always add just one number (the number of new sets per block)

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
        append_to_storage(&set_block_count,&maj_new_set_blocks,1,&new_set_no);//always add just one number (the number of new sets per block)
    }

    xfree(linebuf);
    xfree(formats);

    new_set_no=nr_count;
    if (new_set_nos!=NULL) delete[] new_set_nos;
    new_set_nos=maj_new_nrs;
    maj_new_nrs=NULL;
    if (new_set_block_size!=NULL)delete[] new_set_block_size;
    new_set_block_size=maj_new_set_blocks;
    maj_new_set_blocks=NULL;
    new_set_blocks=set_block_count;

    return RETURN_SUCCESS;
}

int getdata(int gno, char *fn, int src, int load_type)
{
    FILE *fp=NULL;
    int retval=0;
    int save_version, cur_version;
    char fn2[1024];

#ifdef WINDOWS_SYSTEM
    unsigned int p,p2=0;
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

    if (src!=SOURCE_DISK)
    {
    fp = grace_openr(fn2, src);
        if (fp == NULL)
        {
        return RETURN_FAILURE;
        }
    }
    
    save_version = get_project_version();
    set_project_version(0);

    device_table[DEVICE_SCREEN].pg.dpi=start_dpi;

    set_parser_gno(gno);

    if (src!=SOURCE_DISK)
    {
    retval = uniread(fp, load_type, fn2);
    }
    else
    {
    redirect_output_ba=1;
    retval = LoadFileIntoByteArray(correct_file_name(fn2),output_parameter_array);
    //qDebug() << "Load ByteArray : retval=" << retval;
    retval = DeCompressByteArray_simple(output_parameter_array);
    //qDebug() << "Load decompress: retval=" << retval;
    result_buffer.setBuffer(&output_parameter_array);
    result_buffer.open(QIODevice::ReadOnly);
    retval = uniread(fp, load_type, fn2);
    result_buffer.close();
    //qDebug() << "Load uniread   : retval=" << retval;
    redirect_output_ba=0;
    }
//if (number_of_sets(0)>0)
//qDebug() << "getdata: Comment=" << g[0].p[0].comments;
    if (src!=SOURCE_DISK)
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
        write_set(gno, setno, cp, dformat, TRUE, TRUE);
        grace_close(cp);
    }
}

int load_project_file(char *fn, int as_n_template)
{    
    int gno;
    int retval;
    bool old_dec_sep=exchange_point_comma;
    bool old_useQtFonts=useQtFonts;
projectfileloading=1;
#ifdef DEBUG_OUT_LOG
debug_out << "Trying to load file: " << fn << endl;
#endif

if (mainWin!=NULL)
mainWin->future_load_file=QString::fromLocal8Bit(fn);

//qDebug() << "load project file=" << fn << endl;
    if (wipeout())
    {
        /*exchange_point_comma=old_dec_sep;
        reset_default_states();
        useQtFonts=old_useQtFonts;*/
//qDebug() << "wipeout==False" << endl;
#ifdef DEBUG_OUT_LOG
debug_out << "Loading aborted." << endl;
#endif
        path_we_currently_save_to.clear();
        projectfileloading=0;
        return RETURN_FAILURE;//do not load!
    }
    else//really load a new file
    {
        universal_font_size_factor=1.0;//reset Font-Size to normal
        useQtFonts=false;
        qtfonts_have_been_reset=false;
        exchange_point_comma=false;
        timestamp.path=FALSE;
    set_wait_cursor();
QFileInfo fi(fn);
path_we_currently_save_to=fi.absolutePath()+QDir::separator();
//qDebug() << "fi.exist()=" << fi.exists();
//qDebug() << "BEFORE LOAD: FONT=" << grdefaults.font << endl;
    tmp_defaults=grdefaults;//save the current defaults
    tmp_view=grview;
    strcpy(tmp_sformat,sformat);

        setDefaultImage(bg_fill_image);
        for (int i = 0; i < number_of_boxes(); i++)
        {
        boxes[i].rot = 0.0;
        setDefaultImage(boxes[i].fillimage);
        }
        for (int i = 0; i < number_of_ellipses(); i++)
        {
        ellip[i].rot = 0.0;
        setDefaultImage(ellip[i].fillimage);
        }
        for (int i = 0; i < number_of_graphs(); i++)
        {
        g[i].phi0 = 0.0;
        g[i].roffset = 0.0;
        }
        for (int i = 0; i < number_of_strings(); i++)
        {
        pstr[i].master_align=MASTER_ALIGN_TARGET_NONE;
        }
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
                if (mainWin!=NULL)
                mainWin->addToHistory(fn);
            }
            clear_dirtystate();
            retval = RETURN_SUCCESS;
        }
        else
        {
            retval = RETURN_FAILURE;
        }
//if (number_of_sets(0)>0)
//qDebug() << "load_project_file: Comment=" << g[0].p[0].comments;
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
        FormDeviceSetup->printfile_item->lenText->setText(get_filename_with_extension(FormDeviceSetup->cur_dev));

        if (auto_set_cwd)
        {
            /*if ()
            {*/
            QFileInfo finfo(QString::fromLocal8Bit(fn));
            QString pa=QDir::toNativeSeparators(finfo.absolutePath());
            //qDebug() << "setWorkingDir=" << pa;
            set_workingdir(pa.toLocal8Bit().constData());
            //qDebug() << "getWorkingDir=" << get_workingdir() << "fromLocal8Bit()=" << QString::fromLocal8Bit(get_workingdir());
            /*}
            else
            {
            char * wdir1=new char[GR_MAXPATHLEN+1];
            getcwd(wdir1, GR_MAXPATHLEN - 1);
            set_workingdir(wdir1);
            delete[] wdir1;
            }
            qDebug() << "load project file: fn=" << fn;
            qDebug() << "cwd=" << get_workingdir();*/
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

        if (is_valid_gno(get_cg()))
        {
            if (g[get_cg()].ws_top==1 && g[get_cg()].ws[0].w.xg1==g[get_cg()].ws[0].w.xg2)
            {//new viewport stack set
            //qDebug() << "no viewport stack set -- resetting Default";
            g[get_cg()].ws[0].w.xg1=g[get_cg()].w.xg1;
            g[get_cg()].ws[0].w.xg2=g[get_cg()].w.xg2;
            g[get_cg()].ws[0].w.yg1=g[get_cg()].w.yg1;
            g[get_cg()].ws[0].w.yg2=g[get_cg()].w.yg2;
            g[get_cg()].ws[0].w_name=copy_string(g[get_cg()].ws[0].w_name,"Default");
            }
        }

        if (mainWin)
        {
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
            {
            mainWin->doPageZoom(mainWin->sldPageZoom->value());
            }
        }
        else//no MainWindow yet
        {
            if (autofit_on_load==TRUE)
            {
            autofit_pending=1;
            }
        }

        #ifdef DEBUG_OUT_LOG
        if (retval==RETURN_SUCCESS)
        debug_out << "Loading finished successfully." << endl;
        else
        debug_out << "Loading NOT successfull." << endl;
        #endif
    unset_wait_cursor();
        path_we_currently_save_to.clear();
        projectfileloading=0;
        return retval;
    }
path_we_currently_save_to.clear();
projectfileloading=0;
return retval;
}

int load_project(char *fn)
{
    initialize_cmap();//set color map back to normal
    int ret=load_project_file(fn, FALSE);
    if (ret!=RETURN_FAILURE)
    {
    update_default_props();
    update_font_selectors(false);
    }
    return ret;
}

int new_project(char *n_template)
{
    int retval;
    char *s;
    char dummy[1024];
    QString d_text;
    if (n_template == NULL || n_template[0] == '\0')
    {
    QFileInfo fi1(QString::fromLocal8Bit(default_grace_file));
    //qDebug() << "cwd=" << get_workingdir();
    //qDebug() << "default_grace_file=" << default_grace_file;
    //qDebug() << "exists=" << fi1.exists();
        if (fi1.isAbsolute()==true)
        {
        //qDebug() << "is absolute file";
        strcpy(dummy,default_grace_file);
        d_text=QString::fromLocal8Bit(default_grace_file);
        }
        else
        {
        //qDebug() << "is NOT an absolute file";
        sprintf(dummy,"%s/%s",qt_grace_templates_dir,default_grace_file);
        d_text=QString::fromLocal8Bit(qt_grace_templates_dir) + QDir::separator() + QString::fromLocal8Bit(default_grace_file);
        }
    //fi1=QFileInfo(dummy);
    fi1=QFileInfo(d_text);
    //qDebug() << "dummy=" << dummy << "d_text=" << d_text << "Exists=" << fi1.exists();
            if (fi1.exists()==false)
            {
            sprintf(dummy,"%s/%s",qt_grace_templates_dir,"Default.agr");
            d_text=QString::fromLocal8Bit(qt_grace_templates_dir) + QDir::separator() + QString("Default.agr");
            strcpy(default_grace_file,"Default.agr");
            errmsg(QObject::tr("Default startup file not found, reset startup file to Default.agr.").toLocal8Bit().constData());
            }
        strcpy(dummy,d_text.toLocal8Bit().constData());
        retval = load_project_file(dummy, TRUE);
        /// retval = load_project_file("n_templates/Default.agr", TRUE);
    }//end template==NULL meaning: load default file
    else if (is_path_absolute(n_template))//(n_template[0] == '/')//an absolute path
    {
        retval = load_project_file(n_template, TRUE);
    }
    else
    {
        s = (char*)xmalloc(strlen("templates/") + strlen(n_template) + 1);
        if (s == NULL)
        {
            retval = RETURN_FAILURE;
        }
        else
        {
            sprintf(s, "templates/%s", n_template);
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
    FILE *cp=NULL;
    int gno, setno;
    char * old_fn, * n_fn=new char[strlen(fn)+16];
    int noask_save = noask,compr_succ;
    QByteArray ba1,ba2;

QFileInfo finfo(QString::fromLocal8Bit(fn));
//qDebug() << "save_project:" << finfo.absoluteFilePath();
QString suf=finfo.suffix();
path_we_currently_save_to=finfo.absolutePath()+QDir::separator();
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

//qDebug() << "Vor Prepare=" << g[0].labs.title.s_plotstring;
#if READ_UTF8_ONLY == 1
    strcpy(new_encoding_name,"UTF-8");
#else
    strcpy(new_encoding_name,FileCodec->name().constData());
#endif
    copy_LaTeX_to_Grace();
#if READ_UTF8_ONLY == 0
    FileCodecSave=FileCodec;
#endif
    /// prepare_strings_for_saving();
//qDebug() << "Vor Prepare=" << g[0].labs.title.s_plotstring;

    old_fn = get_docname();
    if (compare_strings(old_fn, n_fn))
    {
        /* If saving under the same name, don't warn about overwriting */
        noask = TRUE;
    }
//qDebug() << "n_fn=" << n_fn;
    if (current_compression_method==COMPRESSION_NONE)
    {
        if ((cp = grace_openw(n_fn)) == NULL)
        {
        noask = noask_save;
        delete[] n_fn;
        path_we_currently_save_to.clear();
        return RETURN_FAILURE;
        }
    }
    redirect_output_ba=1;//warning: if redirect_output_ba==0 an open FILE is needed!
    putparms(-1, cp, TRUE);

    QString tt11;//(output_parameter_array);
    //tt11=tt11.toUtf8();
    //tt11=QString::fromUtf8(output_parameter_array.constData());
    //tt11=output_parameter_array;
    /// output_parameter_array=FileCodec->fromUnicode(output_parameter_array.constData());
    ///tt11=FileCodec->fromUnicode(result_params);

//test-output of the new file
//qDebug() << output_parameter_array;

    ba1.clear();
    //ba1.append(FileCodec->fromUnicode(result_params));
    ba1.append(result_params.toUtf8());///starting with v0.2.7 only UTF-8 is used
    /// ba1=output_parameter_array;
    //START OF QTGRACE_SPECIALS_HERE!
    //save_qtGrace_Additions(cp);
    write_qtGrace_Additions_to_Bytearray(output_parameter_array);
    ba1+=output_parameter_array;
    //END OF QTGRACE_SPECIALS_HERE!
    result_params.clear();
    for (gno = 0; gno < number_of_graphs(); gno++)
    {
        for (setno = 0; setno < number_of_sets(gno); setno++)
        {
            write_set(gno, setno, cp, tmp_sformat, FALSE, FALSE);//we have to use the current sformat here
        }
    }
    ba2.clear();
///ba2.append(result_params);
    //ba2.append(FileCodec->fromUnicode(result_params.toUtf8()));
    ba2.append(result_params.toUtf8());///starting with v0.2.7 only UTF-8 is used
    //ba2=QByteArray(result_params.toUtf8().constData());
///qDebug() << "Current-Compression: Method=" << current_compression_method << "Level=" << current_compression_level << "ba2.length=" << ba2.length();
    compr_succ=CompressByteArray(ba2,current_compression_method,current_compression_level);
///qDebug() << "Compression Success(0=ok,1=failure)=" << compr_succ << "ba2.length=" << ba2.length();
///qDebug() << "header:   ba1.length=" << ba1.length();
    ba1+=ba2;
///qDebug() << "complete: ba1.length=" << ba1.length();
    redirect_output_ba=0;
    if (current_compression_method==COMPRESSION_NONE)
    {
    fprintf(cp,"%s",ba1.data());
    grace_close(cp);
    }
    else
    {
    QFileInfo fifo(QString::fromLocal8Bit(n_fn));
    overwriteAnswerNo=FALSE;
    QString text;
    bool write_ok=true;
        if (fifo.exists()==true)
        {
        text=QObject::tr("File ")+QString::fromLocal8Bit(n_fn)+QObject::tr(" exists!\nOverwrite?");
        QMessageBox::StandardButton resBut=QMessageBox::question(0,QObject::tr("Overwrite file?"),text);
            if (resBut==QMessageBox::Yes)
            {
            overwriteAnswerNo=false;
            write_ok=true;
            }
            else
            {
            overwriteAnswerNo=true;
            write_ok=false;
            }
        }

        if (write_ok==true)
        {
        compr_succ=SaveByteArrayToFile(QString::fromLocal8Bit(n_fn),ba1);
        }
///qDebug() << "Saving Success:" << compr_succ;
    }

    resume_strings_after_load_or_save();

    set_docname(n_fn);
    clear_dirtystate();
    if (mainWin!=NULL)
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
    path_we_currently_save_to.clear();
    return RETURN_SUCCESS;
}

/*
 * write out a set
 */
int write_set(int gno, int setno, FILE *cp, char *format, int rawdata, int write_to_cp)
{
    int i, n, col, ncols;
    double *x[MAX_SET_COLS];
    char **s;

    char * result_params_line=new char[1024];

    if (write_to_cp==TRUE) result_params.clear();

    if (cp == NULL && redirect_output_ba==0) {
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
            sprintf(result_params_line, "@target G%d.S%d\n", gno, setno);
            result_params+=QString(result_params_line);
            sprintf(result_params_line, "@type %s\n", set_types(dataset_type(gno, setno)));
            result_params+=QString(result_params_line);
        }
        
        for (i = 0; i < n; i++) {
            for (col = 0; col < ncols; col++) {
                if (col != 0) {
                    //fputs(" ", cp);
                    sprintf(result_params_line, " ");
                    result_params+=QString(result_params_line);
                }
                sprintf(result_params_line, format, x[col][i]);
                result_params+=QString(result_params_line);
            }
            if (s != NULL) {
                sprintf(result_params_line, " \"%s\"", PSTRING(s[i]));
                result_params+=QString(result_params_line);
            }
            //fputs("\n", cp);
            sprintf(result_params_line, "\n");
            result_params+=QString(result_params_line);
        }
        if (rawdata) {
            sprintf(result_params_line, "\n");
            result_params+=QString(result_params_line);
        } else {
            sprintf(result_params_line, "&\n");
            result_params+=QString(result_params_line);
        }
    }
    if (write_to_cp==TRUE)
    {
    fputs(result_params.toLocal8Bit().constData(),cp);
    }
    delete[] result_params_line;
    return RETURN_SUCCESS;
}

int is_Gould_file(char * file)
{
QFileInfo fi(file);
//cout << "GOULD suffix#" << fi.suffix().toLatin1().constData() << "#" << endl;
if (fi.suffix().compare("DAT",Qt::CaseInsensitive)!=0) return FALSE;
FILE * ifi;
char dummy[8];
dummy[4]='\0';
ifi=fopen(file,"r");
        for (int i=0;i<3;i++) dummy[i]=(char)fgetc(ifi);
fclose(ifi);
//cout << "GOULD suffix#" << dummy << "#" << endl;
if (dummy[0]=='|' && dummy[1]=='C' && dummy[2]=='F') return TRUE;
else return FALSE;
}

int is_agr_file(char * file)
{
ifstream ifi;
char d_test[32],d_test2[32];//,c;
ifi.open(file);
ifi.read(d_test,32);
ifi.close();
for (int i=0;i<23;i++)
{
d_test2[i]=d_test[i];
}
d_test[20]='\0';
d_test2[23]='\0';
//qDebug() << "d_test=" << d_test << "d_test2=" << d_test2;
if (strcmp(d_test,"# Grace project file")==0 || strcmp(d_test2,"# ACE/gr parameter file")==0)
return TRUE;
else
return FALSE;
}

int is_text_file(char * file)
{
int maxread=30000;//maximum number of bytes to test-read
int to_read;
QFile fil(QString::fromLocal8Bit(file));
QFileInfo fi(fil);
to_read=fi.size();
if (to_read>maxread) to_read=maxread;
int is_printable,not_printable;
char probe;
is_printable=not_printable=0;
ifstream ifi;
ifi.open(file,ios::binary);
    for (int i=0;i<to_read;i++)
    {
    //ifi.get(probe);
    ifi.read(&probe,1);
        if (isprint(probe) || probe=='\t' || probe=='\n' || probe==0x0D || probe==0x0A) is_printable++;//all printable characters, tab, newline, CR and LF
        else not_printable++;
    }
ifi.close();
double ratio=0.0;
if (not_printable<=0) ratio=100.0;
else ratio=(1.0*is_printable)/(1.0*not_printable);
//qDebug() << "File=" << file << endl << "Ratio=" << ratio << "is_printable=" << is_printable << "not_printable=" << not_printable;
if (ratio>2.0)
return TRUE;
else
return FALSE;
}

bool is_diadem(char * file,char ** header_name)
{
if (file==NULL) return false;
    if (*header_name!=NULL) delete[] *header_name;
    *header_name=new char[strlen(file)+8];
    strcpy(*header_name,file);
    char dummy[128],dummy2[128],dummy3[1024];
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

int is_csv_file(char * file)
{
QFileInfo fi(file);
if (fi.suffix().compare("CSV",Qt::CaseInsensitive)!=0) return FALSE;
else return TRUE;
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
return 0;
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
//bin_import.channel_format_known=true;

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

//ifstream ifi;
//ifi.open(bin_import.DataFile.toLocal8Bit().constData(),ios::binary);
QByteArray ba;
QBuffer buffer;
int ret=LoadFileIntoByteArray(bin_import.DataFile,ba);
if (ret==RETURN_FAILURE) qDebug() << "Unable to load file into ByteArray";

buffer.setBuffer(&ba);
buffer.open(QIODevice::ReadOnly);
//cout << "reading from Diadem-File=#" << bin_import.DataFile.toLocal8Bit().constData() << "# opened?" << ifi.is_open() << endl;
//cout << "std bin import: channels=" << bin_import.channels << " points=" << bin_import.points << endl;
readBinaryFromFile(buffer,bin_import,&bin_import.first_data);
//cout << "gelesen: std bin import: columns=" << bin_import.columns_read << " points=" << bin_import.points_read << endl;
/// ifi.close();
buffer.close();
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
(void)target_gno;
(void)snos;
(void)nr_new_sets;
//nr_new_sets=0;
/// ifstream ifi;
/// ifi.open(header.channels[1].file_name, ios::binary );/// warning: just channel 1 for testing
double ** data;
QByteArray ba;
QBuffer buffer;
int ret=LoadFileIntoByteArray(header.channels[1].file_name,ba);
if (ret==RETURN_FAILURE) qDebug() << "Unable to load file into ByteArray";

buffer.setBuffer(&ba);
buffer.open(QIODevice::ReadOnly);
readBinaryFromFile(buffer,bin_import,&data);
buffer.close();
/// ifi.close();
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
    afi.encoding.clear();
}

//to determine the number of graphs and datasets in an agr file
void read_header_from_agr(char * filename, struct agr_file_info & afi)
{
char dummy_line[1024];
char new_linetype[128];
double new_linetype_val;
int pos,tmp,len,index;//status
int cur_graph,cur_set;

    init_agr_file_info(afi);
QFileInfo fi(QString::fromLocal8Bit(filename));

    if (fi.exists()==false) return;
    strcpy(afi.filename,filename);

//cout << "reading infos from #" << filename << "#" << endl;
QRegularExpressionMatch rm;
QRegularExpression regex1("@\\s+s\\d+\\s+comment");
QRegularExpression regex1a("@\\s+S\\d+\\s+comment");
QRegularExpression regex2("@\\s+s\\d+\\s+legend");
QRegularExpression regex2a("@\\s+S\\d+\\s+legend");
QRegularExpression regex3("@\\s+s\\d+\\s+type");
QRegularExpression regex3a("@\\s+S\\d+\\s+type");
QRegularExpression regex4("@\\s+s\\d+\\s+line");
QString tmp_str,tmp_str2;
QString new_comment,new_type,new_legend;

int n_linet,n_lines,n_linepattern,n_linecolor;
double n_linew;

ifstream ifi;
ifi.open(filename);//we need to know the encoding first
while (!ifi.eof())
{
ifi.getline(dummy_line,1023);
index=sscanf(dummy_line,"#QTGRACE_ADDITIONAL_PARAMETER: ENCODING \"%s",new_linetype);
    if (index>0 && strlen(new_linetype)>1)
    {
    afi.encoding=QString(new_linetype);
    afi.encoding.chop(1);
    break;
    }
}
//qDebug() << "Read Header Encoding: #" << afi.encoding << "#";
if (afi.encoding.isEmpty()) afi.encoding=QString("UTF-8");//set a default

QTextCodec * TempFileCodec=QTextCodec::codecForName(afi.encoding.toLatin1().constData());

if (TempFileCodec==NULL)
{
afi.encoding=QString("UTF-8");
TempFileCodec=QTextCodec::codecForName(afi.encoding.toLatin1().constData());
qDebug() << "Encoding not available; Trying UTF-8";
    if (TempFileCodec==NULL)
    {
    qDebug() << "UTF-8 unavailable!?";
    }
}

ifi.clear();
ifi.seekg(0, ios::beg);

afi.headerlines=0;
//status=0;//nothing
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
//cout << "End found:#" << dummy_line << "#" << endl;
cur_graph=-1;
afi.headerlines--;
break;
}
else if (strncmp(dummy_line,"#QTGRACE_ADDITIONAL_PARAMETER: DATA_COMPRESSED",46)==0)
{
cur_graph=-1;
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
/*
tmp_str=QString::fromLocal8Bit(dummy_line);//::fromLocal8bit
//tmp_str=tmp_str.toUtf8();
tmp_str2=get_text_in_quotations(tmp_str);
qDebug() << "A tmp_str=" << tmp_str << "tmp_str2=" << tmp_str2;
*/
tmp_str=TempFileCodec->toUnicode(dummy_line);
tmp_str2=get_text_in_quotations(tmp_str);
/*qDebug() << "B tmp_str=" << tmp_str << "tmp_str2=" << tmp_str2;
if (tmp_str2.length()>0)
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
    rm=regex1.match(tmp_str);
    //if (regex1.indexIn(tmp_str)>=0)//comment
    if (rm.hasMatch()==true)//comment
    {
    new_comment=tmp_str2;
    goto end_regex_found;
    }
    rm=regex1a.match(tmp_str);
    //else if (regex1a.indexIn(tmp_str)>=0)//comment
    if (rm.hasMatch()==true)//comment
    {
    new_comment=tmp_str2;
    goto end_regex_found;
    }

    rm=regex2.match(tmp_str);
    //else if (regex2.indexIn(tmp_str)>=0)//legend
    if (rm.hasMatch()==true)//legend
    {
    new_legend=tmp_str2;
    goto end_regex_found;
    }
    rm=regex2a.match(tmp_str);
    //else if (regex2a.indexIn(tmp_str)>=0)//legend
    if (rm.hasMatch()==true)//legend
    {
    new_legend=tmp_str2;
    goto end_regex_found;
    }

    rm=regex3.match(tmp_str);
    //else if (regex3.indexIn(tmp_str)>=0)//type
    if (rm.hasMatch()==true)//type
    {
    pos=tmp_str.lastIndexOf(" ");
    new_type=tmp_str.mid(pos+1);
    }
    rm=regex3a.match(tmp_str);
    //else if (regex3a.indexIn(tmp_str)>=0)//type
    if (rm.hasMatch()==true)//type
    {
    pos=tmp_str.lastIndexOf(" ");
    new_type=tmp_str.mid(pos+1);
    }

    rm=regex4.match(tmp_str);
    //else if (regex4.indexIn(tmp_str)>=0)//line-setting of a set
    if (rm.hasMatch()==true)//line-setting of a set
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
end_regex_found:
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
int c_gno,c_sno,c_settype=0,c_cols_to_read=0;
int n_set;//new setid
int index_in_afi;//import-informations in afi for the current data set
int expected_type=0;
int gno=afi.target_gno,slen,size;
bool import_this;
char * dummy_string=NULL;
//ifstream ifi;
QString s_dummy_line,s_dummy_single;

QRegularExpressionMatch rm;
QRegularExpression regex("@\\s+S\\d+\\s+type\\s+xy");
QRegularExpression regex2("@type\\s+");
QRegularExpression regex3("@target\\s+G\\d+.S\\d+");
QRegularExpression regex4("\\S");
QStringList i_list;
QList<double> vals[6];
    for (int i=0;i<6;i++)
    vals[i].clear();
//cout << "read dataset from agr " << regex.pattern().toLocal8Bit().constData() << endl;

    /*for (int i=0;i<afi.set_legends.length();i++)
    {
    qDebug() << i << "-->" << afi.set_legends.at(i);
    }*/

redirect_output_ba=1;//reading data from agr-files now works only with byte-array
int retval = LoadFileIntoByteArray(correct_file_name(afi.filename),output_parameter_array);
//qDebug() << "Load ByteArray : retval=" << retval;
retval = DeCompressByteArray_simple(output_parameter_array);
//qDebug() << "Load decompress: retval=" << retval;
result_buffer.setBuffer(&output_parameter_array);
result_buffer.open(QIODevice::ReadOnly);
//retval = uniread(fp, load_type, fn2);
size=1023;//length of dummy_line without '\0'

//qDebug() << "buffer_open=" << result_buffer.isOpen();

prepare_strings_for_saving();

/// nachher kommt:
/// resume_strings_after_load_or_save();
/// ==>
//convert_all_strings_from_encoding_to_UTF8();
/// --> convert_single_string_from_encoding_to_UTF8_static(g[i].p[j].comments);
/// --> convert_single_string_from_encoding_to_UTF8_static(g[i].p[j].lstr);
//FileCodec=FileCodecSave;//Restore the codec originally set
//copy_Grace_to_LaTeX();
/// --> strcpy(g[i].p[j].orig_lstr,g[i].p[j].lstr);//copy UTF8-String to orig-label
/// --> update_grace_string_from_UTF8_static(g[i].p[j].lstr);//use UTF8-string in label to create a QString, then do LaTeX-to-Grace, then create a C-string or UTF8-string for use without or with QtFonts
/// d.h. lstr und comments müssen im encoding sein
/// d.h. oben in read_header_from_agr: lese String von File im File-Codec, convertiere in UTF8
/// und dann hier: speichere label und comments in lstr und comments im aktuell eingestellten FileCodec, damit resume... am Ende das machen kann, was es immer machen soll

if (new_set_nos!=NULL) delete[] new_set_nos;
new_set_no=0;
new_set_nos=new int[afi.nr_of_sets+2];

//ifi.open(afi.filename);
status=0;//nothing
//1=set found-->read data
//2=set complete-->0
index=0;
while (index<afi.headerlines && !result_buffer.atEnd())//!ifi.eof())
{
//ifi.getline(dummy_line,1023);
slen = result_buffer.readLine(dummy_line,size);
//qDebug() << "[" << index << "]" << dummy_line;
index++;
}

//qDebug() << "nr_of_headerlines=" << afi.headerlines << "last_line_read=" << dummy_line;

c_gno=c_sno=-1;
index_in_afi=-1;
while (!result_buffer.atEnd())//!ifi.eof())
{
//ifi.getline(dummy_line,1023);
slen = result_buffer.readLine(dummy_line,size);
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

        /*if (dummy_string!=NULL) delete[] dummy_string;
        s_dummy_single=afi.set_legends.at(index_in_afi);//.toUtf8();
qDebug() << "Legend import1" << afi.set_legends.at(index_in_afi);
qDebug() << "Legend import2" << afi.set_legends.at(index_in_afi).toUtf8();
qDebug() << "Legend import3" << afi.set_legends.at(index_in_afi).toLocal8Bit();
        dummy_string=new char[s_dummy_single.length()*4+8];*/
        //strcpy(g[gno].p[n_set].orig_lstr,afi.set_legends.at(index_in_afi).toLocal8Bit().constData());
        //strcpy(g[gno].p[n_set].orig_lstr,afi.set_legends.at(index_in_afi).toUtf8().constData());
            /*if (activateLaTeXsupport==TRUE)
            complete_LaTeX_to_Grace_Translator(s_dummy_single);
        generate_string_Qt_aware(dummy_string,s_dummy_single);*/
        //strcpy(g[gno].p[n_set].lstr,dummy_string);
        //strcpy(g[gno].p[n_set].lstr,afi.set_legends.at(index_in_afi).toLocal8Bit().constData());
        if (inhibit_set_legend==0)
        {
        ///set_legend_string(gno,n_set,afi.set_legends.at(index_in_afi).toLocal8Bit().constData());
        //strcpy(g[gno].p[n_set].lstr,FileCodec->fromUnicode(afi.set_legends.at(index_in_afi).toUtf8()).constData());
        strcpy(g[gno].p[n_set].lstr,afi.set_legends.at(index_in_afi).toUtf8().constData());///starting with v0.2.7 only UTF-8 is used
        }

        //set-comment
        /*if (dummy_string!=NULL) delete[] dummy_string;
        s_dummy_single=afi.set_comments.at(index_in_afi);//.toUtf8();
        dummy_string=new char[s_dummy_single.length()*4+8];
        //strcpy(g[gno].p[n_set].orig_comments,afi.set_comments.at(index_in_afi).toLocal8Bit().constData());
            if (activateLaTeXsupport==TRUE)
            complete_LaTeX_to_Grace_Translator(s_dummy_single);
        generate_string_Qt_aware(dummy_string,s_dummy_single);
        //strcpy(g[gno].p[n_set].comments,dummy_string);
        setcomment(gno,n_set,afi.set_comments.at(index_in_afi).toLocal8Bit().constData());*/
        //strcpy(g[gno].p[n_set].comments,FileCodec->fromUnicode(afi.set_comments.at(index_in_afi).toUtf8()).constData());
        strcpy(g[gno].p[n_set].comments,afi.set_comments.at(index_in_afi).toUtf8().constData());///starting with v0.2.7 only UTF-8 is used

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

//qDebug() << "end_of_file: last_line_read=" << dummy_line;

//ifi.close();
result_buffer.close();
//qDebug() << "Load uniread   : retval=" << retval;
redirect_output_ba=0;

resume_strings_after_load_or_save();
}

int readDataFromClient(char* dataIn, int load_type,const char *label)
{
    int save_version, cur_version;
    prepare_strings_for_saving();
    save_version = get_project_version();
    set_project_version(0);

    set_parser_gno(0);

   int nrows, ncols, nncols, nscols, nncols_req;
   int *formats = NULL;
   int breakon, readerror;
   ss_data ssd;
   char *s, tbuf[128];
   char *linebuf=NULL;
   //int linelen=0;   /* a misleading name ... */
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
                   resume_strings_after_load_or_save();
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
                   resume_strings_after_load_or_save();
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
                       resume_strings_after_load_or_save();
                       return RETURN_FAILURE;
                   }
               }

               ncols = nncols + nscols;

               /* init the data storage */
               if (init_ss_data(&ssd, ncols, formats) != RETURN_SUCCESS)
               {
                   errmsg("Malloc failed in uniread()");
                   xfree(linebuf);
                   resume_strings_after_load_or_save();
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
                   resume_strings_after_load_or_save();
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
                       resume_strings_after_load_or_save();
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
           resume_strings_after_load_or_save();
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
   resume_strings_after_load_or_save();
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

void init_csv_import_infos(struct csv_import_infos * info)
{
info->filename=NULL;
info->valid_settings=false;
info->gno=0;
info->headerlines=0;
info->columns_to_read=0;
info->target_cols=NULL;
info->col_format=NULL;
info->import_to_set=NULL;
info->nr_of_target_sets=0;
info->target_sets=NULL;
info->max_col_per_set=NULL;
info->colsep=0;
info->datsep=0;
info->textsep=0;
info->dec_sep_komma=false;
info->text_as_html.clear();
info->invalid_value=0.0;
info->set_generation=0;
info->set_type_gerenation=SET_XY;
info->ba.clear();
info->csv_headerlines.clear();
info->csv_comments.clear();
info->csv_comment_line_nr.clear();
info->csv_comment_at_end.clear();
info->csv_tokens.clear();
info->csv_token_format.clear();
info->csv_token_ok.clear();
info->csv_token_value.clear();
info->csv_column_formats.clear();
info->csv_column_formats_suggestion.clear();
info->number_of_errors=info->columns=info->rows=info->csv_read_started=info->csv_read_stopped=0;
info->error_pos1.clear();
info->error_pos2.clear();
info->overlong_rows.clear();
info->underlong_rows.clear();
info->htmlShowComments=false;
info->htmlShowValues=true;
info->htmlShowOriginal=true;
info->htmlShowErrorLines=true;
info->htmlShowHeaderLines=false;
info->startValues=6;
info->endValues=3;
}

void clear_csv_import_infos(struct csv_import_infos * info)
{
N_DELETE2(info->filename);
N_DELETE2(info->target_cols);
N_DELETE2(info->col_format);
N_DELETE2(info->import_to_set);
N_DELETE2(info->target_sets);
N_DELETE2(info->max_col_per_set);
init_csv_import_infos(info);
}

void init_mcsv_import_infos(struct csv_multi_import_infos * info)
{
info->filename=NULL;
info->valid_settings=false;
info->csv_file_length=0;
info->nr_of_sets=info->nr_of_lines=0;
info->line_separator='\r';
info->default_col_separator='\t';
info->invalid_value=-100.0;
info->set_positions1.clear();
info->set_positions2.clear();
info->col_separator.clear();
info->default_col_separators.clear();
info->set=NULL;
info->text_as_html.clear();
info->errors_as_html.clear();
info->htmlShowComments=false;
info->htmlShowValues=true;
info->htmlShowOriginal=true;
info->startValues=6;
info->endValues=3;
info->postprocess_target_col.clear();
info->postprocess_operation_type.clear();
info->postprocess_operation_source1.clear();
info->postprocess_operation_source2.clear();
info->postprocess_operation_source1_sav.clear();
info->postprocess_operation_source2_sav.clear();
info->postprocess_operation_source_set1.clear();
info->postprocess_operation_source_set2.clear();
info->postprocess_val1.clear();
info->postprocess_val2.clear();
info->postprocess_advancement1.clear();
info->postprocess_advancement2.clear();
info->string_source_col=-1;
info->string_source_set=-1;
info->string_advancement=-1;
info->komma_is_decimal_sep=false;
info->advancement_plus_n=NULL;
}

void clear_mcsv_import_infos(struct csv_multi_import_infos * info)
{
    for (int i=0;i<info->set_positions1.length();i++)
    {
    clear_csv_import_infos(info->set+i);
    }
N_DELETE2(info->set);
init_mcsv_import_infos(info);
}

void copy_csv_import_infos(struct csv_import_infos * target,struct csv_import_infos * source)
{
clear_csv_import_infos(target);
memcpy(target,source,sizeof(struct csv_import_infos));
target->filename=new char[1+strlen(source->filename)];
strcpy(target->filename,source->filename);
target->gno=source->gno;
target->headerlines=source->headerlines;
target->columns_to_read=source->columns_to_read;
target->target_cols=new int[target->columns_to_read];
memcpy(target->target_cols,source->target_cols,sizeof(int)*target->columns_to_read);
target->col_format=new int[target->columns_to_read];
memcpy(target->col_format,source->col_format,sizeof(int)*target->columns_to_read);
target->import_to_set=new int[target->columns_to_read];
memcpy(target->import_to_set,source->import_to_set,sizeof(int)*target->columns_to_read);
target->nr_of_target_sets=source->nr_of_target_sets;
target->target_sets=new int[target->nr_of_target_sets];
memcpy(target->target_sets,source->target_sets,sizeof(int)*target->nr_of_target_sets);
target->max_col_per_set=new int[target->nr_of_target_sets];
memcpy(target->max_col_per_set,source->max_col_per_set,sizeof(int)*target->nr_of_target_sets);
target->colsep=source->colsep;
target->datsep=source->datsep;
target->textsep=source->textsep;
target->dec_sep_komma=source->dec_sep_komma;
target->text_as_html=source->text_as_html;
target->ba=source->ba;
target->csv_headerlines=source->csv_headerlines;
target->csv_comments=source->csv_comments;
target->csv_comment_line_nr=source->csv_comment_line_nr;
target->csv_comment_at_end=source->csv_comment_at_end;
target->csv_tokens=source->csv_tokens;
target->csv_token_format=source->csv_token_format;
target->csv_token_ok=source->csv_token_ok;
target->csv_token_value=source->csv_token_value;
target->csv_column_formats=source->csv_column_formats;
target->error_pos1=source->error_pos1;
target->error_pos2=source->error_pos2;
target->overlong_rows=source->overlong_rows;
target->underlong_rows=source->underlong_rows;
}

void copy_mcsv_import_infos(struct csv_multi_import_infos * target,struct csv_multi_import_infos * source,bool include_data)
{
/// HIER FEHLT NOCH WAS
if (target->filename!=NULL) delete[] target->filename;
if (source->filename==NULL)
{
target->filename=NULL;
}
else
{
target->filename=new char[8+strlen(source->filename)];
strcpy(target->filename,source->filename);
}

target->postprocess_target_col=source->postprocess_target_col;
target->postprocess_operation_type=source->postprocess_operation_type;
target->postprocess_operation_source1=source->postprocess_operation_source1;
target->postprocess_operation_source2=source->postprocess_operation_source2;
target->postprocess_operation_source_set1=source->postprocess_operation_source_set1;
target->postprocess_operation_source_set2=source->postprocess_operation_source_set2;
target->postprocess_val1=source->postprocess_val1;
target->postprocess_val2=source->postprocess_val2;
target->postprocess_advancement1=source->postprocess_advancement1;
target->postprocess_advancement2=source->postprocess_advancement2;
target->string_source_col=source->string_source_col;
target->string_source_set=source->string_source_set;
target->string_advancement=source->string_advancement;

target->readAll=source->readAll;
target->hasStringCol=source->hasStringCol;
target->targetSetType=source->targetSetType;
target->targetSetCols=source->targetSetCols;
target->autoscale=source->autoscale;

target->line_separator=source->line_separator;
target->default_col_separator=source->default_col_separator;
target->default_col_separators=source->default_col_separators;
target->komma_is_decimal_sep=source->komma_is_decimal_sep;
target->invalid_value=source->invalid_value;
target->col_separator=source->col_separator;

target->htmlShowComments=source->htmlShowComments;
target->htmlShowValues=source->htmlShowValues;
target->htmlShowOriginal=source->htmlShowOriginal;
target->htmlShowErrorLines=source->htmlShowErrorLines;
target->htmlShowHeaderLines=source->htmlShowHeaderLines;

if (include_data)
{
    if (target->nr_of_sets>0 && target->set!=NULL)
    {
        for (int i=0;i<target->nr_of_sets;i++)
        clear_csv_import_infos(target->set+i);
        target->set=NULL;
    }

target->valid_settings=source->valid_settings;
target->csv_file_length=source->csv_file_length;
target->nr_of_sets=source->nr_of_sets;
target->nr_of_lines=source->nr_of_lines;
target->error_count=source->error_count;
target->set_positions1=source->set_positions1;
target->set_positions2=source->set_positions2;
target->set=new struct csv_import_infos[target->nr_of_sets];
    for (int i=0;i<target->nr_of_sets;i++)
    {
    copy_csv_import_infos(target->set+i,source->set+i);
    }

target->text_as_html=source->text_as_html;
target->errors_as_html=source->errors_as_html;
target->errors_as_text=source->errors_as_text;
target->csv_column_formats_suggestion=source->csv_column_formats_suggestion;
target->error_count=source->error_count;
target->valid_settings=source->valid_settings;
target->csv_file_length=source->csv_file_length;
target->set_positions1=source->set_positions1;
target->set_positions2=source->set_positions2;

target->ignoredHeaderLines=source->ignoredHeaderLines;
target->startValues=source->startValues;
target->endValues=source->endValues;
target->headerlines=source->headerlines;
target->csv_column_formats_suggestion=source->csv_column_formats_suggestion;

target->postprocess_operation_source1_sav=source->postprocess_operation_source1_sav;
target->postprocess_operation_source2_sav=source->postprocess_operation_source2_sav;
target->string_source_col_sav=source->string_source_col_sav;
target->sourceOfX=source->sourceOfX;
target->number_of_possible_advancements=source->number_of_possible_advancements;

target->postprocessing_order1=source->postprocessing_order1;
target->postprocessing_order2=source->postprocessing_order2;
target->postprocessing_order3=source->postprocessing_order3;
target->postprocessing_order4=source->postprocessing_order4;
for (int i=0;i<4;i++)
{
target->postprocessing_first_column_order[i]=source->postprocessing_first_column_order[i];
target->postprocessing_last_column_order[i]=source->postprocessing_first_column_order[i];
}
target->advancement_order=source->advancement_order;
target->advancement_delta=source->advancement_delta;
target->advancement_ref=source->advancement_ref;
target->advancement_counter=source->advancement_counter;
N_DELETE2(target->advancement_plus_n);
if (source->advancement_plus_n!=NULL)
{
target->advancement_plus_n=new int[target->advancement_counter+2];
memcpy(target->advancement_plus_n,source->advancement_plus_n,sizeof(int)*target->advancement_counter);
}

}

}

void calc_mcsv_advancement(struct csv_multi_import_infos * mcsv_target,struct csv_multi_import_infos * mcsv_template,int nr)
{
//we try to calculate source1/source2 for the target
//warning: target should already contain all settings from the template
QList<int> delta1,delta2;
int pos_x_col=-1,d;
delta1.clear();
delta2.clear();
mcsv_target->postprocess_operation_source1.clear();
mcsv_target->postprocess_operation_source2.clear();
for (int i=0;i<mcsv_target->postprocess_target_col.length();i++)//first: we try to find the X-column
{
    if (mcsv_target->postprocess_target_col.at(i)==0)//X-column
    {
    pos_x_col=i;
    }
}

    /*mcsv_target->postprocess_operation_source_set1
            mcsv_template->postprocess_operation_source_set1;
    mcsv_target->postprocess_operation_source_set2
            mcsv_template->postprocess_operation_source_set2;*/

    mcsv_target->postprocess_operation_source1.clear();
    mcsv_target->postprocess_operation_source2.clear();
for (int i=0;i<mcsv_template->postprocess_operation_source1.length();i++)
{
    if (mcsv_template->postprocess_operation_source1.at(i)<0)//not a real column
    {
    mcsv_target->postprocess_operation_source1 << mcsv_template->postprocess_operation_source1.at(i);
    }
    else
    {
    //a real column-id
    }
    if (mcsv_template->postprocess_operation_source2.at(i)<0)//not a real column
    {
    mcsv_target->postprocess_operation_source2 << mcsv_template->postprocess_operation_source2.at(i);
    }
    else
    {
    //a real column-id
    }
}

    /*mcsv_target->postprocess_operation_source1
            mcsv_template->postprocess_operation_source1;
    mcsv_target->postprocess_operation_source2
            mcsv_template->postprocess_operation_source2;

    mcsv_target->postprocess_advancement1
            mcsv_template->postprocess_advancement1;
    mcsv_target->postprocess_advancement2
            mcsv_template->postprocess_advancement2;*/
}

void saveCSV_singleSetFormat_ToFile(ofstream & ofi,struct csv_import_infos * csv)
{
ofi << "NUMBER_OF_COLUMN_FORMATS= " << csv->csv_column_formats.length() << endl;
for (int i=0;i<csv->csv_column_formats.length();i++)
ofi << "COLUMN_FORMAT= " << csv->csv_column_formats.at(i) << endl;
}

void saveCSV_multiSetFormat_ToFile(ofstream & ofi,struct csv_multi_import_infos * csv)
{
char dummy[128];
ofi << "CSV_FORMAT_SETTINGS V1" << endl;
ofi << "#GENERAL_SETTINGS" << endl;
ofi << "NR_OF_HEADER_LINES= " << csv->headerlines << endl;
ofi << "LINE_SEPARATOR= " << ((int)csv->line_separator) << endl;
ofi << "DEFAULT_COLUMN_SEPARATOR= " << ((int)csv->default_col_separator) << endl;
ofi << "KOMMA_IS_DECIMAL_SEPARATOR= " << (csv->komma_is_decimal_sep==true?1:0) << endl;
sprintf(dummy,sformat,csv->invalid_value);
ofi << "INVALID_VALUE= " << dummy << endl;
ofi << "#SET_FORMAT" << endl;
ofi << "NR_OF_SETS= " << csv->nr_of_sets << endl;
    for (int i=0;i<csv->nr_of_sets;i++)
    {
    ofi << "SET " << i << " COLUMN_SEPARATOR= " << ((int)csv->col_separator.at(i)) << endl;
    saveCSV_singleSetFormat_ToFile(ofi,csv->set+i);
    }
ofi << "#POSTPROCESSING_SETTINGS" << endl;
ofi << "READ_ALL= " << (csv->readAll==true?1:0) << endl;
ofi << "AUTOSCALE= " << csv->autoscale << endl;
ofi << "TARGET_SET_TYPE= " << csv->targetSetType << endl;
int nr_of_target_cols=csv->postprocess_target_col.length();
ofi << "NR_OF_TARGET_LINES= " << nr_of_target_cols << endl;
for (int i=0;i<nr_of_target_cols;i++)
{
ofi << "TARGET_LINE " << i << endl;
ofi << "TARGET_COLUMN= " << csv->postprocess_target_col.at(i) << " OPERATION= " << csv->postprocess_operation_type.at(i) << endl;
ofi << "SOURCE_SET1= " << csv->postprocess_operation_source_set1.at(i) << " SOURCE_SET2= " << csv->postprocess_operation_source_set2.at(i) << endl;
ofi << "SOURCE1= " << csv->postprocess_operation_source1.at(i) << " SOURCE2= " << csv->postprocess_operation_source2.at(i) << endl;
sprintf(dummy,sformat,csv->postprocess_val1.at(i));
ofi << "VALUE1= " << dummy;
sprintf(dummy,sformat,csv->postprocess_val2.at(i));
ofi << " VALUE2= " << dummy << endl;
ofi << "ADVANCEMENT1= " << csv->postprocess_advancement1.at(i) << " ADVANCEMENT2= " << csv->postprocess_advancement2.at(i) << endl;
}
ofi << "STRING_COLUMN: PRESENT= " << (csv->hasStringCol==true?1:0) << " SOURCE_SET= " << csv->string_source_set << " SOURCE_COLUMN= " << csv->string_source_col << " ADVANCEMENT= " << csv->string_advancement << endl;
}

void loadCSV_singleSetFormat_FromFile(ifstream & ifi,struct csv_import_infos * csv)
{
int probe,probe2;
char dummy[1024];
csv->csv_column_formats.clear();
ifi >> dummy >> probe;
    for (int i=0;i<probe;i++)
    {
    ifi >> dummy >> probe2;
    csv->csv_column_formats << probe2;
    }
}

void loadCSV_multiSetFormat_FromFile(ifstream & ifi,struct csv_multi_import_infos * csv)
{
char dummy[1024],dummy2[1024],dummy3[1024];
int version,probe,probe2;
double d_probe;
ifi >> dummy >> dummy2;//"CSV_FORMAT_SETTINGS V1"
if (strcmp(dummy,"CSV_FORMAT_SETTINGS")!=0)
{
errmsg("Error! File is NOT a CSV-Format-Settings-File!");
return;
}
sscanf(dummy2,"V%d",&version);
//qDebug() << "CSV-File-Format Version " << version;
clear_mcsv_import_infos(csv);
ifi >> dummy;//"#GENERAL_SETTINGS"
if (strcmp(dummy,"#GENERAL_SETTINGS")!=0)
{
errmsg("Error reading CSV-Format-Settings (#GENERAL_SETTINGS missing)");
return;
}
ifi >> dummy >> csv->headerlines;
ifi >> dummy >> probe;
csv->line_separator=(char)probe;
ifi >> dummy >> probe;
csv->default_col_separator=(char)probe;
ifi >> dummy >> probe;
csv->komma_is_decimal_sep=(probe==1?true:false);
ifi >> dummy >> csv->invalid_value;
ifi >> dummy;//"#SET_FORMAT"
if (strcmp(dummy,"#SET_FORMAT")!=0)
{
errmsg("Error reading CSV-Format-Settings (#SET_FORMAT missing)");
return;
}
ifi >> dummy >> csv->nr_of_sets;
csv->set=new csv_import_infos[csv->nr_of_sets>1?csv->nr_of_sets:2];
csv->col_separator.clear();
    for (int i=0;i<csv->nr_of_sets;i++)
    {
    ifi >> dummy >> probe >> dummy2 >> probe2;
    csv->col_separator << (char)probe2;
    loadCSV_singleSetFormat_FromFile(ifi,csv->set+probe);
    }
ifi >> dummy;//"#POSTPROCESSING_SETTINGS"
if (strcmp(dummy,"#POSTPROCESSING_SETTINGS")!=0)
{
errmsg("Error reading CSV-Format-Settings (#POSTPROCESSING_SETTINGS missing)");
return;
}
    ifi >> dummy >> probe;
    csv->readAll=(probe==1?true:false);
    ifi >> dummy >> csv->autoscale;
    ifi >> dummy >> csv->targetSetType;
int nr_of_target_cols;
    ifi >> dummy >> nr_of_target_cols;
    for (int i=0;i<nr_of_target_cols;i++)
    {
    ifi >> dummy >> probe;
    ifi >> dummy >> probe >> dummy2 >> probe2;
    csv->postprocess_target_col << probe;
    csv->postprocess_operation_type << probe2;
    ifi >> dummy >> probe >> dummy2 >> probe2;
    csv->postprocess_operation_source_set1 << probe;
    csv->postprocess_operation_source_set2 << probe2;
    ifi >> dummy >> probe >> dummy2 >> probe2;
    csv->postprocess_operation_source1 << probe;
    csv->postprocess_operation_source2 << probe2;
    ifi >> dummy >> d_probe;
    csv->postprocess_val1 << d_probe;
    ifi >> dummy2 >> d_probe;
    csv->postprocess_val2 << d_probe;
    ifi >> dummy >> probe >> dummy2 >> probe2;
    csv->postprocess_advancement1 << probe;
    csv->postprocess_advancement2 << probe2;
    }
    ifi >> dummy >> dummy2 >> probe;
    csv->hasStringCol=(probe==1?true:false);
    ifi >> dummy >> csv->string_source_set >> dummy2 >> csv->string_source_col >> dummy3 >> csv->string_advancement;
//qDebug() << dummy << csv->string_source_set << dummy2 << csv->string_source_col << dummy3 << csv->string_advancement;
csv->csv_column_formats_suggestion.clear();
for (int i=0;i<csv->nr_of_sets;i++)
{
csv->csv_column_formats_suggestion << csv->set[i].csv_column_formats;
}
csv->valid_settings=true;
}

void saveCSVformatToFile(const char *file,struct csv_import_infos * cvs)
{
ofstream ofi;
//preprocess-set-ids (should begin with 0 and then increase by one; could be {5,6,9} --> should be {0,1,2}
/// missing preprocessing
//int * tcols=new int[cvs->columns_to_read];
//int * fcols=new int[cvs->columns_to_read];
int * icols=new int[cvs->columns_to_read];
int min_set=-1,max_set=-1,min_set2;
bool found;
for (int i=0;i<cvs->columns_to_read;i++)
{
    if ((min_set<0 && cvs->import_to_set[i]>=0) || (min_set>=0 && cvs->import_to_set[i]>=0 && cvs->import_to_set[i]<min_set)) min_set=cvs->import_to_set[i];
    if ((max_set<0 && cvs->import_to_set[i]>=0) || (max_set>=0 && cvs->import_to_set[i]>max_set)) max_set=cvs->import_to_set[i];
}
/*qDebug() << "min=" << min_set << "max=" << max_set;
qDebug() << "set-ids before resort:";*/
for (int i=0;i<cvs->columns_to_read;i++)
{
icols[i]=cvs->import_to_set[i];//first: just copy everything
//qDebug() << icols[i];
}
if (!(min_set==-1 && max_set==-1))//at least something is not -1 or -2
{
for (int i=0;i<cvs->columns_to_read;i++) if (icols[i]>=0) icols[i]-=min_set;//reduce the set-ids in order to begin with 0
max_set-=min_set;
min_set=0;
//now min-set-id=0, but there may be set-ids missing between min and max
for (int j=min_set+1;j<=max_set;j++)//search for every id greater than min_set
{
found=false;
min_set2=-1;
    for (int i=0;i<cvs->columns_to_read;i++)
    {
    if (icols[i]==j) {found=true;}
    if (icols[i]>j && (icols[i]<min_set2 || min_set2<0)) {min_set2=icols[i];}
    }
//qDebug() << "searching id" << j << "found=" << found << "min_set2=" << min_set2;
    if (found==false && min_set2>=0)//set-id does not exist - yet
    {
        for (int i=0;i<cvs->columns_to_read;i++) if (icols[i]>j) icols[i]-=min_set2-j;//reduce all set-ids greater than j by the difference to the next bigger set-id
        max_set-=min_set2;
    }
}
}
/*qDebug() << "set-ids after resort:";
for (int i=0;i<cvs->columns_to_read;i++)
{
qDebug() << icols[i];
}*/

ofi.open(file);
ofi << "CSV-File-Format_V " << 1 << endl;//current format version=1
//cvs->filename;//not needed for format-info
//cvs->gno;//target-graph-number-->will change to current-graph anyway-->not saved
ofi << "Number_of_header_line= " << cvs->headerlines << endl;
ofi << "Columns_in_file= " << cvs->columns_to_read << endl;//here it is the number of columns we have import settings in this file for
for (int i=0;i<cvs->columns_to_read;i++)
{
ofi << "Target_column " << i << " = " << cvs->target_cols[i] << endl;//the column-numbers the read columns are to go to (X=0, Y=1, Y1=2 ...)
ofi << "Column_format " << i << " = " << cvs->col_format[i] << endl;//the format to be used for import of a column
ofi << "Target_set " << i << " = " << icols[i] << endl;//set numbers to be actually used (one set number (0,1,2,...) for every column - could be -1=none or -2=all sets)
}
//the next three settings will be generated when needed --> not saved
//cvs->nr_of_target_sets;//number of sets to be created for the data
//cvs->target_sets;//next available set numbers (all available, before load --> this will be set to the sets actually to be used, because these sets have to be created)
//cvs->max_col_per_set;//the number of columns the new sets have to have
QChar uchar;
uchar=QChar(cvs->colsep);
ofi << "Column_separator= " << uchar.unicode() << endl;
uchar=QChar(cvs->datsep);
ofi << "Data_point_separator= " << uchar.unicode() << endl;
uchar=QChar(cvs->textsep);
ofi << "Text_separators= " << uchar.unicode() << endl;//the separator-characters
ofi << "Decimal_separator_is_komma= " << (cvs->dec_sep_komma==true?1:0) << endl;//is the decimal separator a komma (this means: exchange '.' and ',')
ofi.close();
//delete[] tcols;
//delete[] fcols;
delete[] icols;
}

void loadCSVformatFromFile(const char * file,struct csv_import_infos * cvs)
{
ifstream ifi;
char dummy1[512],dummy2[512];
int current_format_version=1,tmp_int1,tmp_int2;
ifi.open(file);
ifi >> dummy1 >> dummy2;
//cout << "dummy1=" << dummy1 << " dummy2=" << dummy2;
if (strcmp(dummy1,"CSV-File-Format_V")!=0)
{
    errmsg(QObject::tr("Not a csv-format-file!").toLocal8Bit().constData());
    return;
}
if (atoi(dummy2)!=current_format_version)
{
    errmsg(QObject::tr("Format-Version unknown (does not match)!").toLocal8Bit().constData());
    return;
}
N_DELETE2(cvs->filename);
    cvs->gno=get_cg();
ifi >> dummy1 >> dummy2;
    cvs->headerlines=atoi(dummy2);
ifi >> dummy1 >> dummy2;
    cvs->columns_to_read=atoi(dummy2);
N_DELETE2(cvs->target_cols);
N_DELETE2(cvs->col_format);
N_DELETE2(cvs->import_to_set);
    if (cvs->columns_to_read>0)
    {
    cvs->target_cols=new int[cvs->columns_to_read];
    cvs->col_format=new int[cvs->columns_to_read];
    cvs->import_to_set=new int[cvs->columns_to_read];
    }
for (int i=0;i<cvs->columns_to_read;i++)
{
ifi >> dummy1 >> tmp_int1 >> dummy2 >> tmp_int2;
    if (tmp_int1<cvs->columns_to_read)
    cvs->target_cols[tmp_int1]=tmp_int2;
ifi >> dummy1 >> tmp_int1 >> dummy2 >> tmp_int2;
    if (tmp_int1<cvs->columns_to_read)
    cvs->col_format[tmp_int1]=tmp_int2;
ifi >> dummy1 >> tmp_int1 >> dummy2 >> tmp_int2;
    if (tmp_int1<cvs->columns_to_read)
    cvs->import_to_set[tmp_int1]=tmp_int2;
}
//the next three settings will be generated when needed --> not saved
    cvs->nr_of_target_sets=0;
N_DELETE2(cvs->target_sets);
N_DELETE2(cvs->max_col_per_set);
/// missing: postproces target-set-numbers and their column-lengths
QChar uchar;
ifi >> dummy1 >> dummy2;
uchar=QChar((unsigned short)(atoi(dummy2)));
    cvs->colsep=uchar.toLatin1();
ifi >> dummy1 >> dummy2;
uchar=QChar((unsigned short)(atoi(dummy2)));
    cvs->datsep=uchar.toLatin1();
ifi >> dummy1 >> dummy2;
uchar=QChar((unsigned short)(atoi(dummy2)));
    cvs->textsep=uchar.toLatin1();
ifi >> dummy1 >> dummy2;
    if (atoi(dummy2)==1)
    cvs->dec_sep_komma=true;
    else
    cvs->dec_sep_komma=false;
ifi.close();
}

void prepareColumnsForProgression(struct csv_multi_import_infos * info)
{
info->postprocess_operation_source1_sav=info->postprocess_operation_source1;
info->postprocess_operation_source2_sav=info->postprocess_operation_source2;
info->string_source_col_sav=info->string_source_col;
info->number_of_possible_advancements=0;
if (info->readAll==true)//advancement possible/needed
{
    for (int i=0;i<info->postprocess_advancement1.length();i++)
    {
        if (info->postprocess_advancement1.at(i)>0)//0 means: no change
        {
        info->number_of_possible_advancements++;
        }
    }
    for (int i=0;i<info->postprocess_advancement2.length();i++)
    {
        if (info->postprocess_advancement2.at(i)>0)//0 means: no change
        {
        info->number_of_possible_advancements++;
        }
    }
}
for (int i=0;i<info->nr_of_sets;i++) gatherDefaultColumnIndices(info->set+i);
int set,column;
for (int i=0;i<info->postprocess_operation_type.length();i++)
{
set=info->postprocess_operation_source_set1.at(i);
    if (set>=0 && set<info->nr_of_sets)
    {
    column=info->postprocess_operation_source1.at(i);
    if (column>=0 && column<info->set[set].columns)
        if (info->set[set].usedIndices.indexOf(column)<0)
            info->set[set].usedIndices << column;
    }
set=info->postprocess_operation_source_set2.at(i);
    if (set>=0 && set<info->nr_of_sets)
    {
    column=info->postprocess_operation_source2.at(i);
    if (column>=0 && column<info->set[set].columns)
        if (info->set[set].usedIndices.indexOf(column)<0)
            info->set[set].usedIndices << column;
    }
}
info->sourceOfX=-1;
for (int i=0;i<info->nr_of_sets;i++)
{
info->set[i].lastColIndex=-1;
info->set[i].firstColIndex=-1;
   for (int j=0;j<info->set[i].usedIndices.length();j++)
   {
   if (info->set[i].usedIndices.at(j)>info->set[i].lastColIndex) info->set[i].lastColIndex=info->set[i].usedIndices.at(j);
   if (info->set[i].firstColIndex==-1 || info->set[i].firstColIndex>info->set[i].usedIndices.at(j)) info->set[i].firstColIndex=info->set[i].usedIndices.at(j);
   }
}
}

void gatherDefaultColumnIndices(struct csv_import_infos * info)
{
info->allColumnIndices.clear();
info->dataOnlyIndices.clear();
info->stringOnlyIndices.clear();
info->similarIndices.clear();
info->usedIndices.clear();
    for (int i=0;i<info->csv_column_formats.length();i++)
    {
    info->allColumnIndices << i;
        if (info->csv_column_formats.at(i)==FORMAT_STRING || info->csv_column_formats.at(i)==FORMAT_INVALID)
        {
        info->stringOnlyIndices << i;
        }
        else
        {
        info->dataOnlyIndices << i;
        }
    }
}

void gatherSimilarColumnIndices(struct csv_import_infos * info,int type,int start)
{
info->similarIndices.clear();
    for (int i=start;i<info->csv_column_formats.length();i++)
    {
    if (info->csv_column_formats.at(i)==type) info->similarIndices << i;
    }
}

void addColumnToUsedIndices(struct csv_multi_import_infos * info, int set, int col)
{
if (set<0 || col<0 || set>=info->nr_of_sets) return;
if (col>=info->set[set].columns || info->set[set].usedIndices.contains(col)) return;
info->set[set].usedIndices << col;
}

void gatherUsedColumnIndices(struct csv_multi_import_infos * info)
{
int s,c;
bool cont;
    for (int l=0;l<info->postprocess_target_col.length();l++)//search in all input-lines
    {
    s=info->postprocess_operation_source_set1.at(l);
    c=info->postprocess_operation_source1.at(l);
        if (c>=0 && s>=0 && s<info->nr_of_sets)//a real column
        {
            cont=info->set[s].usedIndices.contains(c);
            if (cont==false)
            {
            info->set[s].usedIndices << c;
            //qDebug() << "Set" << s << "adding Column" << c;
            }
        }
        if (info->postprocess_operation_type.at(l)>0)
        {
        s=info->postprocess_operation_source_set2.at(l);
        c=info->postprocess_operation_source2.at(l);
            if (c>=0 && s>=0 && s<info->nr_of_sets)//a real column
            {
                cont=info->set[s].usedIndices.contains(c);
                if (cont==false)
                {
                info->set[s].usedIndices << c;
                //qDebug() << "Set" << s << "adding Column" << c;
                }
            }
        }
    }
    if (info->hasStringCol==true && info->string_source_col>=0 && info->string_source_set>=0 && info->string_source_set<info->nr_of_sets)//a real column
    {
        cont=info->set[info->string_source_set].usedIndices.contains(info->string_source_col);
        if (cont==false)
        {
        info->set[info->string_source_set].usedIndices << info->string_source_col;
        //qDebug() << "Set" << info->string_source_set << "adding Column" << info->string_source_col;
        }
    }
}

int find_nearest_index(QList<int> & list,int value,bool before)//go through the list and look for the entry that is nearest value, but lower (before=true) or higher (before=false) - list is assumed to be sorted ascending
{
int index=-1;
if (list.length()<1) return -1;
int found_val=list.at(0);
int tmp_index=0;
int ref_delta,tmp_delta;
ref_delta=tmp_delta=-1;
if ((found_val<value && before) || (found_val>value && !before))
{
index=tmp_index;
ref_delta=abs(value-found_val);
}
else
{
return -1;//if list is sorted this means that there will be no suitable entry
}

for (int i=1;i<list.length();i++)
{
    if (list.at(i)>=value && before) break;//no entries >value allowed
    if (list.at(i)<=value && !before) continue;//we do not look at lower entries
tmp_delta=abs(value-list.at(i));
    if (ref_delta>tmp_delta || index<0)
    {
    ref_delta=tmp_delta;
    index=i;
    }
}

return index;
}

const char * advancement_names[6]={"+0","+1","+n","same","next set/same column",""};

void gatherColumnOrder(struct csv_multi_import_infos * info)
{
int advance_type_counter[info->nr_of_sets+2][6];
for (int s=0;s<info->nr_of_sets;s++)
for (int i=0;i<6;i++) advance_type_counter[s][i]=0;
N_DELETE2(info->advancement_plus_n);
info->advancement_plus_n=new int[info->nr_of_sets+2];
for (int s=0;s<info->nr_of_sets;s++) info->advancement_plus_n[s]=-3;
info->postprocessing_order1.clear();
info->postprocessing_order2.clear();
info->postprocessing_order3.clear();
info->postprocessing_order4.clear();
info->advancement_order.clear();
info->advancement_delta.clear();
info->advancement_ref.clear();
for (int s=0;s<4;s++)
{
info->postprocessing_first_column_order[s].clear();
info->postprocessing_last_column_order[s].clear();
}
int * f_index=new int[2+info->nr_of_sets];
int * l_index=new int[2+info->nr_of_sets];
for (int i=0;i<info->nr_of_sets;i++)
{
f_index[i]=l_index[i]=-1;
}
if (info->readAll==true)
{
    for (int l=0;l<info->postprocess_target_col.length();l++)//search in all input-lines
    {
        if (info->postprocess_operation_source1.at(l)>0)
    advance_type_counter[info->postprocess_operation_source_set1.at(l)][info->postprocess_advancement1.at(l)]++;
        if (info->postprocess_operation_type.at(l)>0 && info->postprocess_operation_source2.at(l)>0)
        advance_type_counter[info->postprocess_operation_source_set2.at(l)][info->postprocess_advancement2.at(l)]++;
    }
    if (info->hasStringCol==true && info->string_source_col>=0)
    {
    advance_type_counter[info->string_source_set][info->string_advancement]++;
    }
}
QList<int> start_i,stop_i;
start_i.clear();
stop_i.clear();
start_i << 0;
for (int s=0;s<info->nr_of_sets;s++)//go through all sets
{
    if (s>0)
    {
    stop_i << info->postprocessing_order3.length()-1;
    start_i <<  info->postprocessing_order3.length();
    }
    for (int c=0;c<info->set[s].columns;c++)//go through all columns in the set
    {
        for (int l=0;l<info->postprocess_target_col.length();l++)//search in all input-lines
        {
            if (info->postprocess_operation_source_set1.at(l)==s && info->postprocess_operation_source1.at(l)==c)
            {
            info->postprocessing_order1 << l;
            info->postprocessing_order2 << 1;
            info->postprocessing_order3 << s;
            info->postprocessing_order4 << c;
            if (f_index[s]==-1) f_index[s]=info->postprocessing_order1.length()-1;
            l_index[s]=info->postprocessing_order1.length()-1;
            }
            if (info->postprocess_operation_type.at(l)>0)
            if (info->postprocess_operation_source_set2.at(l)==s && info->postprocess_operation_source2.at(l)==c)
            {
            info->postprocessing_order1 << l;
            info->postprocessing_order2 << 2;
            info->postprocessing_order3 << s;
            info->postprocessing_order4 << c;
            if (f_index[s]==-1) f_index[s]=info->postprocessing_order1.length()-1;
            l_index[s]=info->postprocessing_order1.length()-1;
            }
        }
        if (info->string_source_col==c && info->string_source_set==s && info->hasStringCol==true)
        {
        info->postprocessing_order1 << -1;
        info->postprocessing_order2 << 0;
        info->postprocessing_order3 << s;
        info->postprocessing_order4 << c;
        if (f_index[s]==-1) f_index[s]=info->postprocessing_order1.length()-1;
        l_index[s]=info->postprocessing_order1.length()-1;
        }
    }
}
stop_i << info->postprocessing_order3.length()-1;
for (int i=0;i<info->nr_of_sets;i++)
{
info->postprocessing_first_column_order[i] << f_index[i];
info->postprocessing_last_column_order[i] << l_index[i];
}
if (info->readAll==true)
{
    for (int s=0;s<info->nr_of_sets;s++)
    {
    if (advance_type_counter[s][2]>0)
    {
    if (advance_type_counter[s][0]==0 && advance_type_counter[s][1]==0 && advance_type_counter[s][3]==0)
    {//just '+n' --> block-advancement
    info->advancement_plus_n[s]=-1;
    }
    else if (advance_type_counter[s][0]>0 && advance_type_counter[s][1]==0 && advance_type_counter[s][3]==0)
    {
    info->advancement_plus_n[s]=-2;
    }
    else if (advance_type_counter[s][3]>0)//'same'
    {

    }
    else if (advance_type_counter[s][1]>0)//'+1'
    {

    }
    }
    /*qDebug() << "Set" << s << "advancement-types:";
    for (int i=0;i<6;i++)
    {
    qDebug() << advancement_names[i] << "=" << advance_type_counter[s][i];
    }*/
    }
/*qDebug() << "Order1:" << info->postprocessing_order1;
qDebug() << "Order2:" << info->postprocessing_order2;
qDebug() << "Order3:" << info->postprocessing_order3;
qDebug() << "Order4:" << info->postprocessing_order4;
qDebug() << "start_i:" << start_i;
qDebug() << "stop_i:" << stop_i;*/
int cur_adv_type=-1,cur_col=-1;
QList<int> p0,p1,pn,ps,pnsnc;//list of advancement-indices in order3
QList<int> c0,c1,cn,cs,cnsnc;//list of column-indices in order4
int li=-1;
int temp_ref=-1;
int temp_index,temp_delta,best_index,best_delta;
if (info->postprocessing_order3.length()>0)
{
    for (int i=0;i<start_i.length();i++)//this is equal to 'go through all sets'
    {
        p0.clear();
        p1.clear();
        pn.clear();
        ps.clear();
        pnsnc.clear();
        c0.clear();
        c1.clear();
        cn.clear();
        cs.clear();
        cnsnc.clear();
        for (int j=start_i.at(i);j<=stop_i.at(i);j++)
        {
            li=info->postprocessing_order1.at(j);
            cur_adv_type=-1;
            cur_col=-1;
            if (li==-1)//string
            {
            cur_adv_type=info->string_advancement;
            cur_col=info->string_source_col;
            }
            else
            {
                if (info->postprocessing_order2.at(j)==1)
                {
                cur_adv_type=info->postprocess_advancement1.at(li);
                }
                else
                {
                cur_adv_type=info->postprocess_advancement2.at(li);
                }
            cur_col=info->postprocessing_order4.at(j);
            }
            switch (cur_adv_type)
            {
            default:
            break;
            case 0://+0
            p0 << j;
            c0 << cur_col;
            break;
            case 1://+1
            p1 << j;
            c1 << cur_col;
            break;
            case 2://+n
            pn << j;
            cn << cur_col;
            break;
            case 3://+same
            ps << j;
            cs << cur_col;
            break;
            case 4://+next set/same column
            pnsnc << j;
            cnsnc << cur_col;
            break;
            }
        }

        if (p0.length()>0)
        {
        info->advancement_order << p0;
        info->advancement_delta << p0;//ref and delta do not need this, but we do this, so that the length matches
        info->advancement_ref << p0;
        }
        if (pnsnc.length()>0)
        {
        info->advancement_order << pnsnc;
        info->advancement_delta << pnsnc;
        info->advancement_ref << pnsnc;
        }
        if (ps.length()>0)
        {
        info->advancement_order << ps;
        info->advancement_delta << ps;
        info->advancement_ref << ps;
        }
        if (p1.length()>0)
        {
        info->advancement_order << p1;
        info->advancement_delta << p1;
        info->advancement_ref << p1;
        }
        if (pn.length()>0)
        {
        info->advancement_order << pn;
            for (int n=0;n<cn.length();n++)
            {
            temp_ref=-1;//we need to find a temporary reference and a temporary delta
            //ref is every column that is not '+n' before this one
            temp_index=temp_delta=best_index=best_delta=-1;
                if (c0.length()>0)
                {
                temp_index=find_nearest_index(c0,cn.at(n),true);
                    if (temp_index>=0)//found something
                    {
                    best_index=p0.at(temp_index);
                    best_delta=abs(c0.at(temp_index)-cn.at(n));
                    }
                    else//maybe something after this
                    {
                    temp_index=find_nearest_index(c0,cn.at(n),false);
                        if (temp_index>=0)//found something
                        {
                        best_index=p0.at(temp_index);
                        best_delta=-abs(c0.at(temp_index)-cn.at(n));//a negative delta here to indicate that the reference is after the '+n'
                        }
                    }
                }
                if (cnsnc.length()>0)
                {
                temp_index=find_nearest_index(cnsnc,cn.at(n),true);
                    if (temp_index>=0)
                    {
                    temp_delta=abs(cnsnc.at(temp_index)-cn.at(n));
                        if (best_delta<0 || best_index<0 || (temp_delta<best_delta && best_delta>0))
                        {//we found a better one
                        best_index=pnsnc.at(temp_index);
                        best_delta=temp_delta;
                        }
                    }
                    else if (best_delta<0)//maybe something after this (only if we have not found something before yet)
                    {
                    temp_index=find_nearest_index(cnsnc,cn.at(n),false);
                        if (temp_index>=0)//found something
                        {
                        temp_delta=abs(cnsnc.at(temp_index)-cn.at(n));//a negative delta here to indicate that the reference is after the '+n'
                            if (best_index<0 || temp_delta<abs(best_delta))//we are only here if we haven't found anything yet
                            {
                            best_index=pnsnc.at(temp_index);
                            best_delta=-temp_delta;
                            }
                        }
                    }
                }
                if (cs.length()>0)
                {
                temp_index=find_nearest_index(cs,cn.at(n),true);
                    if (temp_index>=0)
                    {
                    temp_delta=abs(cs.at(temp_index)-cn.at(n));
                        if (best_delta<0 || best_index<0 || (temp_delta<best_delta && best_delta>0))
                        {//we found a better one
                        best_index=ps.at(temp_index);
                        best_delta=temp_delta;
                        }
                    }
                    else if (best_delta<0)//maybe something after this (only if we have not found something before yet)
                    {
                    temp_index=find_nearest_index(cs,cn.at(n),false);
                        if (temp_index>=0)//found something
                        {
                        temp_delta=abs(cs.at(temp_index)-cn.at(n));//a negative delta here to indicate that the reference is after the '+n'
                            if (best_index<0 || temp_delta<abs(best_delta))//we are only here if we haven't found anything yet
                            {
                            best_index=ps.at(temp_index);
                            best_delta=-temp_delta;
                            }
                        }
                    }
                }
                if (c1.length()>0)
                {
                temp_index=find_nearest_index(c1,cn.at(n),true);
                    if (temp_index>=0)
                    {
                    temp_delta=abs(c1.at(temp_index)-cn.at(n));
                        if (best_delta<0 || best_index<0 || (temp_delta<best_delta && best_delta>0))
                        {//we found a better one
                        best_index=p1.at(temp_index);
                        best_delta=temp_delta;
                        }
                    }
                    else if (best_delta<0)//maybe something after this (only if we have not found something before yet)
                    {
                    temp_index=find_nearest_index(c1,cn.at(n),false);
                        if (temp_index>=0)//found something
                        {
                        temp_delta=abs(c1.at(temp_index)-cn.at(n));//a negative delta here to indicate that the reference is after the '+n'
                            if (best_index<0 || temp_delta<abs(best_delta))//we are only here if we haven't found anything yet
                            {
                            best_index=p1.at(temp_index);
                            best_delta=-temp_delta;
                            }
                        }
                    }
                }
                if (cn.length()>0 && best_index<0)//nothing found yet --> only '+n' in this set
                {
                    best_index=cn.at(n);
                    best_delta=cn.at(n)-cn.at(0)+1;
                }
            info->advancement_ref << best_index;
            info->advancement_delta << best_delta;
            }
        }
    }
}
/*qDebug() << "advancement_order=" << info->advancement_order;
qDebug() << "advancement_ref  =" << info->advancement_ref;
qDebug() << "advancement_delta=" << info->advancement_delta;*/
}
delete[] f_index;
delete[] l_index;
}

int removeQuotation(QString orig,QString & without)
{
int quote1=orig.indexOf('\"',0);
    if (quote1<0 || quote1>=orig.length()-1)
    {
    without=orig;
    return 0;
    }
int quote2=orig.indexOf('\"',quote1+1);
    if (quote2<0 || quote2>orig.length()-1)
    {
    without=orig;
    return 0;
    }
//[ "test"  A] --> quote1=1, quote2=6 --> start at quote1+1=2, length=quote2-quote1-1 --> [test] (part after second '"' is lost)
without=orig.mid(quote1+1,quote2-quote1-1);
return 1;
}

int superior_format(int format0,int format1)//compares two column-formats and tries to guess to more special one (i.e. General-numbers can be read by most formats, Engineering-numbers can only be read as engineering --> engineering is superior to General)
{//return-value: 0-->format0, 1=format1, -1=formats not compatible (like longitudinal and dates)
int order0=format_ordering[format0+2];
int order1=format_ordering[format1+2];
if (abs(order1-order0)>19) return -1;//not compatible
    if (order0>order1)
    return 0;
    else
    return 1;
}

//this function reads the tokens in a column again according to the "format"
void reread_csv_token_column(struct csv_import_infos & info,int col,int format)
{
int ret,endpos,emptyresidual;
double val;
//qDebug() << "ReReadColumn:" << col << "Format:" << format;
    for (int i=0;i<info.rows;i++)
    {
    if (info.csv_token_format.at(i).length()<=col) continue;
    if (info.csv_token_format.at(i).at(col)==format) continue;//token has already been read in the suitable format
    ret=read_date_or_number_in_format(info.csv_tokens.at(i).at(col).toLocal8Bit().constData(),format,TRUE,&val,&endpos,&emptyresidual);
    info.csv_token_format[i].replace(col,format);
        if ((ret==RETURN_SUCCESS && emptyresidual==1) || (format==-2))//string (format==-2) is always ok...
        {
        info.csv_token_ok[i].replace(col,true);
        }
        else
        {
        //qDebug() << "ReReadColumn: RETURN_FAILURE, (" << i << "|" << col << "):ret=" << ret << "empty=" << emptyresidual;
        errmsg((QObject::tr("ReReadColumn: RETURN_FAILURE, (")+QString::number(i)+QString("|")+QString::number(col)+QString(")")).toLocal8Bit().constData());
        info.csv_token_ok[i].replace(col,false);
        val=info.invalid_value;
        }
    info.csv_token_value[i].replace(col,val);
    }
    info.csv_column_formats.replace(col,format);
}

int error_counter_in_csv_import(struct csv_import_infos & info)
{
info.number_of_errors=0;
info.error_pos1.clear();
info.error_pos2.clear();
    for (int i=0;i<info.rows;i++)
    {
        for (int j=0;j<info.columns;j++)
        {
        if (info.csv_token_ok.at(i).length()<=j) continue;
//qDebug() << "Format=" << info.csv_column_formats.at(j);
            if (info.csv_token_ok.at(i).at(j)==false && info.csv_column_formats.at(j)!=FORMAT_STRING)
            {
            info.number_of_errors++;
            info.error_pos1 << i;
            info.error_pos2 << j;
            }
        }
    }
info.number_of_errors+=info.underlong_rows.length();
/*for (int i=0;i<info.underlong_rows.length();i++)
{
info.error_pos1 << info.underlong_rows.at(i);
info.error_pos2 << info.csv_tokens.at(info.underlong_rows.at(i)).length();
}*/
info.number_of_errors+=info.overlong_rows.length();
/*for (int i=0;i<info.overlong_rows.length();i++)
{
info.error_pos1 << info.overlong_rows.at(i);
info.error_pos2 << info.csv_tokens.at(info.overlong_rows.at(i)).length();
}*/
return info.number_of_errors;
}

QByteArray removeNonPrint(QByteArray ba)
{
int len=ba.length();
QByteArray nba;
nba.clear();
    for (int i=0;i<len;i++)
    {
    if (isprint(ba.at(i))) nba += ba.at(i);
    }
return nba;
}

QByteArray removeComment(QByteArray ba)//removes everthing right of '#' (if it is not inside '"')
{
QByteArray nba,nba2;
QList<QByteArray> splitted;
int nr_of_quotes,quotes_so_far;
nba.clear();
if (ba.length()<1) return nba;
splitted=ba.split('#');
//qDebug() << "splitted=" << splitted;
if (splitted.length()<2)
{
nba=ba;
}
else if (splitted.at(0).length()<1)
{
return nba;
}
else
{
nba2 = removeNonPrint(splitted.at(0));
//qDebug() << "splitted=" << splitted;
if (nba2.at(0)=='#') return nba;//everything is a comment

quotes_so_far=splitted.at(0).count('\"');
nba = splitted.at(0);
    for (int i=1;i<splitted.length();i++)
    {
        //qDebug() << i << "quotes_so_far=" << quotes_so_far;
        if (quotes_so_far%2!=0)//# is inside a string \"Text\"
        {
        nba += QByteArray("#");
        nba += splitted.at(i);
        }
        else//# is NOT inside a string \"Text\" -->stop here
        {
        break;
        }
        nr_of_quotes=splitted.at(i).count('\"');
        quotes_so_far+=nr_of_quotes;
    }
}
return nba;
}

int read_and_guess_csv_file(char * filename, int nr_of_header_lines, QStringList &ignoredHeaderLines, char & guessed_line_sep, char & guessed_dec_sep, QList<int> & separate_positions1, QList<int> &separate_positions2, QList<char> & guessed_separator, int &lines, int guess)
{
QByteArray ba,nba;
int ret=LoadFileIntoByteArray(QString(filename),ba);
int counter[8];
char default_col_sep;
if (ret!=0) return RETURN_FAILURE;
double ratio;
QList<QByteArray> lines_list;
//qDebug() << "read_and_guess_csv_file=" << guessed_line_sep;
if (guess==1) guessed_line_sep='.';
else default_col_sep=guessed_separator.at(0);
/*
qDebug() << "remove Comment Test";
QByteArray tba,tba2;
tba=QByteArray("0.14 3.2");
tba2=removeComment(tba);
qDebug() << "tba=" << tba << "--> tba2=" << tba2;
tba=QByteArray("0.14 3.2 #test1");
tba2=removeComment(tba);
qDebug() << "tba=" << tba << "--> tba2=" << tba2;
tba=QByteArray("0.14 #test1 3.2");
tba2=removeComment(tba);
qDebug() << "tba=" << tba << "--> tba2=" << tba2;
tba=QByteArray("0.14 \"#test1\" 3.2 #test2");
tba2=removeComment(tba);
qDebug() << "tba=" << tba << "--> tba2=" << tba2;
tba=QByteArray("    #test1");
tba2=removeComment(tba);
qDebug() << "tba=" << tba << "--> tba2=" << tba2;
tba=QByteArray("#test1");
tba2=removeComment(tba);
qDebug() << "tba=" << tba << "--> tba2=" << tba2;
*/
if (guess==1)
{
counter[0]=ba.count('\t');//<tab>
counter[1]=ba.count('\r');//CR
counter[2]=ba.count('\n');//LF
counter[3]=ba.count(' ');//<space>
counter[4]=ba.count(';');//;
counter[5]=ba.count(',');//,
counter[6]=ba.count('.');//.
/*qDebug() << "filename:" << filename;
qDebug() << "Tabs=  " << counter[0];
qDebug() << "CR=    " << counter[1];
qDebug() << "LF=    " << counter[2];
qDebug() << "Space= " << counter[3];
qDebug() << "';'=   " << counter[4];
qDebug() << "','=   " << counter[5];
qDebug() << "'.'=   " << counter[6];*/
ratio=((double)counter[1])/((double)counter[2]);
if (counter[2]==0 || ratio>10.0)//(nearly)no LF
{
//qDebug() << "Line-Ending is probably CR";
guessed_line_sep='\r';
}
else if (ratio>0.9 && ratio<1.1 && counter[1]>2)//roughly the same amount of CR and LF (and there are more than 2 of them)
{
//qDebug() << "Line-Ending is probably CR+LF";
guessed_line_sep='\n';//we just need the end! '\r' will be ignored if found alone
}
else if (ratio<0.1)//(nearly) no CR
{
//qDebug() << "Line-Ending is probably LF";
guessed_line_sep='\n';
}
else if (counter[1]<3 && counter[2]<3)//(nearly) no CR AND LF
{
//qDebug() << "No CR or LF found";
    if (counter[4]>0)
    {
    guessed_line_sep=';';
    //qDebug() << "Line-Ending is probably semicolon ';'";
    }
    else if (counter[5]>0 && counter[6]>0)
    {
    guessed_line_sep=',';
    //qDebug() << "We found ',' AND '.' --> we guess:" << endl << "Line-Ending is probably comma ','";
    }
    else
    {
    guessed_line_sep='\n';
    //qDebug() << "Unable to guess --> set '\n' as default line-separator";
    }
}
else
{
guessed_line_sep='\n';
//qDebug() << "Unable to guess --> set '\n' as default line-separator";
}
}//end: guess line-separator
///now we think we know the line-separators and need to guess the column-separator(s)
lines_list=ba.split(guessed_line_sep);//this generates all the lines as separate entries
int status=0;//0=we are in empty space(last entry was empty);1=we found a non-empty-line(last entry was non-empty)
separate_positions1.clear();
separate_positions2.clear();
guessed_separator.clear();
double means[9];
int deviations[9];
int best_index;
bool possible[9];
//QString control_char[8]={"\\t"," ",",",";",".","\\r","\\n","NONE"};
char control_char_c[9]={'\t',' ',';','|',',','.','\r','\n','n'};
int index_of_constr_char;
QList<int> tab_counter;
QList<int> space_counter;
QList<int> semicolon_counter;
QList<int> vline_counter;
QList<int> comma_counter;
QList<int> dot_counter;
QList<int> cr_counter;
QList<int> lf_counter;
QList< QList<int> > Counters;
lines=lines_list.length();
    //for (int i=0;i<lines_list.length();i++)
ignoredHeaderLines.clear();
    for (int i=0;i<nr_of_header_lines;i++)
    {
    ignoredHeaderLines << QString(lines_list.at(i));
    }
    for (int i=nr_of_header_lines;i<lines_list.length();i++)
    {
        nba=removeNonPrint(lines_list.at(i));
        if (nba.length()>0)
        {
            if (nba.at(0)=='#')//comment
            {
            continue;
            }
        }
    lines_list.replace(i,removeComment(lines_list.at(i)));
    nba=removeNonPrint(lines_list.at(i));
    //qDebug() << "ba=" << lines_list.at(i) << "nba=" << nba;
        if (nba.length()>0 && status==0)//non-empty line after empty-line --> new set
        {
        separate_positions1 << i;
        status=1;
        tab_counter.clear();
        space_counter.clear();
        comma_counter.clear();
        semicolon_counter.clear();
        dot_counter.clear();
        cr_counter.clear();
        lf_counter.clear();
        vline_counter.clear();

        tab_counter << lines_list.at(i).count('\t');
        space_counter << lines_list.at(i).count(' ');
        semicolon_counter << lines_list.at(i).count(';');
        vline_counter << lines_list.at(i).count('|');
        comma_counter << lines_list.at(i).count(',');
        dot_counter << lines_list.at(i).count('.');
        cr_counter << lines_list.at(i).count('\r');
        lf_counter << lines_list.at(i).count('\n');
        }
        else if (nba.length()<=0 && status>0)//empty-line after non-empty-line --> set complete --> look for the possible separators
        {
        separate_positions2 << i-1;//i-1 because stop-position is last non-empty-line and we are in the first line after an empty line
        status=0;
        Counters.clear();
        Counters << tab_counter;//0
        Counters << space_counter;//1
        Counters << semicolon_counter;//2
        Counters << vline_counter;//3
        Counters << comma_counter;//4
        Counters << dot_counter;//5
        Counters << cr_counter;//6
        Counters << lf_counter;//7
        Counters << lf_counter;//8: just for savety, 8=none=impossible
        //separators
        for (int j=0;j<9;j++)
        {
            means[j]=0;possible[j]=true;
        //if (j<8)
        //qDebug() << "Counter.length=" << Counters.at(j).length();
        }
        //qDebug() << "tab_counter.length()=" << tab_counter.length() << "vline_counter.length()=" << vline_counter.length();
            for (int j=0;j<tab_counter.length();j++)
            {
            means[0]+=tab_counter.at(j);
            means[1]+=space_counter.at(j);
            means[2]+=semicolon_counter.at(j);
            means[3]+=vline_counter.at(j);
            means[4]+=comma_counter.at(j);
            means[5]+=dot_counter.at(j);
            means[6]+=cr_counter.at(j);
            means[7]+=lf_counter.at(j);
            //8=none --> impossible
            }
        for (int j=0;j<9;j++)
        {
        deviations[j]=0;
        means[j]/=tab_counter.length();//the mean amount of the different possible separator-characters in every line
        //qDebug() << "Mean[" << j << "]=" << means[j];
        }
            for (int j=0;j<tab_counter.length();j++)
            {
                for (int k=0;k<9;k++)
                {
                //qDebug() << "Counter.length()=" << Counters.length() << "Counters[k].length()=" << Counters.at(k).length();
                if (Counters.at(k).at(j)==0 || fabs(1.0-means[k]/Counters.at(k).at(j))>0.2) possible[k]=false;
                if (Counters.at(k).at(j)==0) deviations[k]+=5;//if the potential separator is missing in a line, this counts as a very bad thing --> +5 deviations
                    else if (fabs(1.0-means[k]/Counters.at(k).at(j))>0.2) deviations[k]++;//if the potential separator is underrepresented in a line --> +1 deviation
                }
            //qDebug() << "Space count[" << j << "]=" << space_counter.at(j) << "limit=" << fabs(1.0-means[1]/space_counter.at(j));
            /*if (tab_counter.at(j)==0 || fabs(1.0-means[0]/tab_counter.at(j))>0.2) possible[0]=false;
            if (space_counter.at(j)==0 || fabs(1.0-means[1]/space_counter.at(j))>0.2) possible[1]=false;
            if (comma_counter.at(j)==0 || fabs(1.0-means[2]/comma_counter.at(j))>0.2) possible[2]=false;
            if (semicolon_counter.at(j)==0 || fabs(1.0-means[3]/semicolon_counter.at(j))>0.2) possible[3]=false;
            if (dot_counter.at(j)==0 || fabs(1.0-means[4]/dot_counter.at(j))>0.2) possible[4]=false;
            if (cr_counter.at(j)==0 || fabs(1.0-means[5]/cr_counter.at(j))>0.2) possible[5]=false;
            if (lf_counter.at(j)==0 || fabs(1.0-means[6]/lf_counter.at(j))>0.2) possible[6]=false;

            if (tab_counter.at(j)==0) deviations[0]+=5;//if the potential separator is missing in a line, this counts as a very bad thing --> +5 deviations
                else if (fabs(1.0-means[0]/tab_counter.at(j))>0.2) deviations[0]++;//if the potential separator is underrepresented in a line --> +1 deviation
            if (space_counter.at(j)==0) deviations[1]+=5;
                else if (fabs(1.0-means[1]/space_counter.at(j))>0.2) deviations[1]++;
            if (comma_counter.at(j)==0) deviations[2]+=5;
                else if (fabs(1.0-means[2]/comma_counter.at(j))>0.2) deviations[2]++;
            if (semicolon_counter.at(j)==0) deviations[3]+=5;
                else if (fabs(1.0-means[3]/semicolon_counter.at(j))>0.2) deviations[3]++;
            if (dot_counter.at(j)==0) deviations[4]+=5;
                else if (fabs(1.0-means[4]/dot_counter.at(j))>0.2) deviations[4]++;
            if (cr_counter.at(j)==0) deviations[5]+=5;
                else if (fabs(1.0-means[5]/cr_counter.at(j))>0.2) deviations[5]++;
            if (lf_counter.at(j)==0) deviations[6]+=5;
                else if (fabs(1.0-means[6]/lf_counter.at(j))>0.2) deviations[6]++;*/
            }
        best_index=0;
        index_of_constr_char=-1;
        possible[8]=false;//8=none --> not possible
        deviations[8]=tab_counter.length()*20;//8=none --> very bad!
        for (int j=8;j>=0;j--)
        {
        //qDebug() << control_char[j] << "possible=" << possible[j] << "deviations=" << deviations[j];
        if (deviations[j]<deviations[best_index]) best_index=j;
        if (possible[j]==true) index_of_constr_char=j;
        }
        //qDebug() << "bestIndex=" << best_index;
        //{'\t',' ',',',';','.','\r','\n','n'};//possible separator
        //{0,1,2,3,4,5,6,7};//index
        //{6,5,3,4,2,1,0,'n'};//priority

        /*if (possible[6]==true) index_of_constr_char=6;/// first we try a priority based guess (and just exclude unlikely options)
        if (possible[5]==true) index_of_constr_char=5;
        if (possible[4]==true) index_of_constr_char=4;
        if (possible[2]==true) index_of_constr_char=2;
        if (possible[3]==true) index_of_constr_char=3;
        if (possible[1]==true) index_of_constr_char=1;
        if (possible[0]==true) index_of_constr_char=0;`*/

            if (guess!=1)
            {
            guessed_separator << default_col_sep;
            }
            else
            {
                if (index_of_constr_char==-1)/// if the first guess does not work, we try a guess based on the amount of deviations, i.e. how many line would not match the scheme
                {
                //qDebug() << "Unable to guess column-separator --> take best guess";
                guessed_separator << control_char_c[best_index];//'\t';
                }
                else
                {
                guessed_separator << control_char_c[index_of_constr_char];
                //qDebug() << "Guessed column-separator=" << guessed_separator.last();
                }
            }
        }
        else if (nba.length()>0)//just a non-empty-line somewhere
        {
            tab_counter << lines_list.at(i).count('\t');
            space_counter << lines_list.at(i).count(' ');
            comma_counter << lines_list.at(i).count(',');
            semicolon_counter << lines_list.at(i).count(';');
            dot_counter << lines_list.at(i).count('.');
            cr_counter << lines_list.at(i).count('\r');
            lf_counter << lines_list.at(i).count('\n');
            vline_counter << lines_list.at(i).count('|');
        }
    }//end of search through all lines
if (separate_positions1.length()>separate_positions2.length())//we found start, but no stop
{
separate_positions2 << lines_list.length()-1;//we add the last line to the storage (we have to because we can not read afterwards, but we need the info for the end-position)
Counters.clear();
Counters << tab_counter;//0
Counters << space_counter;//1
Counters << semicolon_counter;//2
Counters << vline_counter;//3
Counters << comma_counter;//4
Counters << dot_counter;//5
Counters << cr_counter;//6
Counters << lf_counter;//7
Counters << lf_counter;//8: just for savety, 8=none=impossible
//separators
for (int j=0;j<8;j++) {means[j]=0;possible[j]=true;}
    for (int j=0;j<tab_counter.length();j++)
    {
        means[0]+=tab_counter.at(j);
        means[1]+=space_counter.at(j);
        means[2]+=semicolon_counter.at(j);
        means[3]+=vline_counter.at(j);
        means[4]+=comma_counter.at(j);
        means[5]+=dot_counter.at(j);
        means[6]+=cr_counter.at(j);
        means[7]+=lf_counter.at(j);
        //8=none --> impossible
    }
for (int j=0;j<8;j++)
{
means[j]/=tab_counter.length();//the mean amount of the different possible separator-characters in every line
deviations[j]=0;
//qDebug() << "Mean[" << j << "]=" << means[j];
}
    for (int j=0;j<tab_counter.length();j++)
    {
        for (int k=0;k<9;k++)
        {
        //qDebug() << "Counter.length()=" << Counters.length() << "Counters[k].length()=" << Counters.at(k).length();
        if (Counters.at(k).at(j)==0 || fabs(1.0-means[k]/Counters.at(k).at(j))>0.2) possible[k]=false;
        if (Counters.at(k).at(j)==0) deviations[k]+=5;//if the potential separator is missing in a line, this counts as a very bad thing --> +5 deviations
            else if (fabs(1.0-means[k]/Counters.at(k).at(j))>0.2) deviations[k]++;//if the potential separator is underrepresented in a line --> +1 deviation
        }
    /*
    if (tab_counter.at(j)==0 || fabs(1.0-means[0]/tab_counter.at(j))>0.2) possible[0]=false;
    if (space_counter.at(j)==0 || fabs(1.0-means[1]/space_counter.at(j))>0.2) possible[1]=false;
    if (comma_counter.at(j)==0 || fabs(1.0-means[2]/comma_counter.at(j))>0.2) possible[2]=false;
    if (semicolon_counter.at(j)==0 || fabs(1.0-means[3]/semicolon_counter.at(j))>0.2) possible[3]=false;
    if (dot_counter.at(j)==0 || fabs(1.0-means[4]/dot_counter.at(j))>0.2) possible[4]=false;
    if (cr_counter.at(j)==0 || fabs(1.0-means[5]/cr_counter.at(j))>0.2) possible[5]=false;
    if (lf_counter.at(j)==0 || fabs(1.0-means[6]/lf_counter.at(j))>0.2) possible[6]=false;

    if (tab_counter.at(j)==0) deviations[0]+=5;
        else if (fabs(1.0-means[0]/tab_counter.at(j))>0.2) deviations[0]++;
    if (space_counter.at(j)==0) deviations[1]+=5;
        else if (fabs(1.0-means[1]/space_counter.at(j))>0.2) deviations[1]++;
    if (comma_counter.at(j)==0) deviations[2]+=5;
        else if (fabs(1.0-means[2]/comma_counter.at(j))>0.2) deviations[2]++;
    if (semicolon_counter.at(j)==0) deviations[3]+=5;
        else if (fabs(1.0-means[3]/semicolon_counter.at(j))>0.2) deviations[3]++;
    if (dot_counter.at(j)==0) deviations[4]+=5;
        else if (fabs(1.0-means[4]/dot_counter.at(j))>0.2) deviations[4]++;
    if (cr_counter.at(j)==0) deviations[5]+=5;
        else if (fabs(1.0-means[5]/cr_counter.at(j))>0.2) deviations[5]++;
    if (lf_counter.at(j)==0) deviations[6]+=5;
        else if (fabs(1.0-means[6]/lf_counter.at(j))>0.2) deviations[6]++;*/
    }
best_index=0;
index_of_constr_char=-1;
possible[8]=false;//8=none --> not possible
deviations[8]=tab_counter.length()*20;//8=none --> very bad!
for (int j=8;j>=0;j--)
{
//qDebug() << control_char[j] << "possible=" << possible[j] << "deviations=" << deviations[j];
    if (deviations[j]<deviations[best_index]) best_index=j;
    if (possible[j]==true) index_of_constr_char=j;
}
//{'\t',' ',',',';','.','\r','\n','n'};//possible separator
//{0,1,2,3,4,5,6,7};//index
//{6,5,3,4,2,1,0,'n'};//priority
/*index_of_constr_char=-1;
if (possible[6]==true) index_of_constr_char=6;
if (possible[5]==true) index_of_constr_char=5;
if (possible[4]==true) index_of_constr_char=4;
if (possible[2]==true) index_of_constr_char=2;
if (possible[3]==true) index_of_constr_char=3;
if (possible[1]==true) index_of_constr_char=1;
if (possible[0]==true) index_of_constr_char=0;*/
    if (guess!=1)
    {
    guessed_separator << default_col_sep;
    }
    else
    {
        if (index_of_constr_char==-1)
        {
        //qDebug() << "Unable to guess column-separator --> using best guess";
        errmsg((QObject::tr("Unable to guess column-separator --> using best guess: '")+control_char_c[best_index]+QString("'")).toLocal8Bit().constData());
        guessed_separator << control_char_c[best_index];//'\t';
        }
        else
        {
        guessed_separator << control_char_c[index_of_constr_char];
        //qDebug() << "Guessed column-separator=" << guessed_separator.last();
        }
    }
}
else if (separate_positions1.length()==0 && separate_positions2.length()==0)
{
//qDebug() << "We did not find any printable characters in the ascii-file";
errmsg((QObject::tr("Unable to find any printable characters in the ascii-file.")).toLocal8Bit().constData());
}
/*
    qDebug() << "Set-separation-positions (line-sep:" << ((int)(guessed_line_sep)) << "):";
    for (int i=0;i<separate_positions1.length();i++)
    {
    qDebug() << "Set" << i << ": von" << separate_positions1.at(i) << "bis" << separate_positions2.at(i) << "separator=" << ((int)guessed_separator.at(i));
    }
*/
if (guess==1)
{
bool komma_as_separator_used=false;
if (guessed_line_sep==',') komma_as_separator_used=true;
    for (int i=0;i<guessed_separator.length();i++)
    {
    if (guessed_separator.at(i)==',') komma_as_separator_used=true;
    }
if (counter[5]>3*counter[6] && komma_as_separator_used==false) guessed_dec_sep=',';
//qDebug() << "guess dec sep:" << guessed_dec_sep << "| counter[5]=" << counter[5] << "| counter[6]=" << counter[6] << "| komma_as_separator_used=" << komma_as_separator_used;
}
return RETURN_SUCCESS;
}

int read_csv_file_into_sets(struct csv_multi_import_infos & minfo,int guess)
{
char guessed_dec_sep='.';
if (guess!=1)
{
guessed_dec_sep=(minfo.komma_is_decimal_sep==true?',':'.');
minfo.col_separator.clear();
for (int i=0;i<minfo.nr_of_sets;i++)
minfo.col_separator << minfo.default_col_separator;
}
int ret=read_and_guess_csv_file(minfo.filename,minfo.headerlines,minfo.ignoredHeaderLines,minfo.line_separator,guessed_dec_sep,minfo.set_positions1,minfo.set_positions2,minfo.col_separator,minfo.nr_of_lines,guess);
minfo.komma_is_decimal_sep=(guessed_dec_sep=='.'?false:true);
int sum=0;
    if (ret!=RETURN_SUCCESS)
    {
    //qDebug() << "Error during 'read_and_guess_csv_file'.";
    errmsg(QObject::tr("Error while reading csv-file.").toLocal8Bit().constData());
    }
minfo.nr_of_sets=minfo.set_positions1.length();
minfo.set=new struct csv_import_infos[minfo.nr_of_sets>0?minfo.nr_of_sets:1];
for (int i=0;i<minfo.nr_of_sets;i++)
{
init_csv_import_infos(minfo.set+i);
if (minfo.csv_column_formats_suggestion.length()>i)//at first we copy any existing format-suggestions
{
minfo.set[i].csv_column_formats_suggestion=minfo.csv_column_formats_suggestion.at(i);
}
else
{
minfo.set[i].csv_column_formats_suggestion.clear();
}
minfo.set[i].filename=new char[1+strlen(minfo.filename)];
strcpy(minfo.set[i].filename,minfo.filename);
//copy the general settings
minfo.set[i].datsep=minfo.line_separator;
minfo.set[i].colsep=minfo.col_separator.at(i);
minfo.set[i].headerlines=minfo.set_positions1.at(i);
minfo.set[i].invalid_value=minfo.invalid_value;
minfo.set[i].dec_sep_komma=minfo.komma_is_decimal_sep;

minfo.set[i].htmlShowValues=minfo.htmlShowValues;
minfo.set[i].htmlShowComments=minfo.htmlShowComments;
minfo.set[i].htmlShowOriginal=minfo.htmlShowOriginal;
minfo.set[i].htmlShowErrorLines=minfo.htmlShowErrorLines;
minfo.set[i].htmlShowHeaderLines=minfo.htmlShowHeaderLines;
minfo.set[i].startValues=minfo.startValues;
minfo.set[i].endValues=minfo.endValues;

ret=read_csv_file_into_tokens(minfo.set[i],guess);//remember: error-count is done at the end of this!
sum+=minfo.set[i].number_of_errors;
    if (ret!=RETURN_SUCCESS)
    {
    //qDebug() << "Error during 'read_csv_file_into_tokens', Set" << i << ".";
    errmsg((QObject::tr("Error reading data into tokens (Set ")+QString::number(i)+QObject::tr(").")).toLocal8Bit().constData());
    }
minfo.set[i].text_as_html=convert_csv_contents_to_html(minfo.set[i]);
minfo.set[i].errors_as_html=convert_csv_errors_to_html(minfo.set[i]);
minfo.set[i].errors_as_text=convert_csv_errors_to_text(minfo.set[i]);

//qDebug() << "Set"<< i << ": columns=" << minfo.set[i].columns << "rows=" << minfo.set[i].rows;

/*qDebug() << "Set"<< i << ": start=" << minfo.set_positions1.at(i) << "stop=" << minfo.set_positions2.at(i);
qDebug() << "resulting read start=" << minfo.set[i].csv_read_started << "stop=" << minfo.set[i].csv_read_stopped;*/
}//end of loop over all sets
minfo.text_as_html=convert_csv_contents_to_html(minfo);//,0,-1,-1,true);//0,5,2,false
minfo.errors_as_html=convert_csv_errors_to_html(minfo);
minfo.errors_as_text=convert_csv_errors_to_text(minfo);
minfo.error_count=sum;
//qDebug() << "overall-csv-error-count=" << sum;
    if (sum==0)
    return RETURN_SUCCESS;
    else
    return RETURN_FAILURE;
}

int autoload_csv_file(struct csv_multi_import_infos & minfo,int guess)
{
int ret=0;
char * newlegend=new char[GR_MAXPATHLEN+128];
/// first: read data from file (note: the error-counting is also done here)
(void)read_csv_file_into_sets(minfo,guess);
//errors?
if (minfo.error_count>0)
{
if (minfo.errors_as_text.length()*2+16>GR_MAXPATHLEN+128)
{
delete[] newlegend;
newlegend=new char[minfo.errors_as_text.length()*2+16];
}
strcpy(newlegend,minfo.errors_as_text.toLocal8Bit().constData());
errmsg(newlegend);
}
/*qDebug() << "Autoloader: errors=" << minfo.error_count;
qDebug() << "Autoloader: nr_of_sets=" << minfo.nr_of_sets;*/
/// second: create new sets
//rules: first column in a set is X-column
//every other column is Y
//except string-columns; string-columns are attached to the last set that has been created
//if two string columns follow each other, the first is attached to the last set created,
//the second one is put into a new set with Y=0
//legend and comment are generated according to the drop-file-rules (see also GeneralPaste)
QList<int> new_graph_ids,new_set_ids;//for undo later
QList<int> source_columns;
int cur_g_id,cur_s_id,cur_x_col;
int cur_y_col,cur_s_col;
int setl;

char * newcomment=new char[GR_MAXPATHLEN];
new_graph_ids.clear();
new_set_ids.clear();
for (int set=0;set<minfo.nr_of_sets;set++)
{
cur_g_id=get_cg();
    cur_x_col=-1;
//qDebug() << "Autoloader: set" << set << "columns=" << minfo.set[set].csv_column_formats.length();
    //find the first column that is not a string-column
    for (int i=0;i<minfo.set[set].csv_column_formats.length();i++)
    {
        //FORMAT_STRING==-2
        //FORMAT_INVALID==-1
        if (minfo.set[set].csv_column_formats.at(i)>=0)//any useful data-format
        {
        cur_x_col=i;
        break;
        }
    }
if (cur_x_col==-1)
{
//sprintf(newlegend,"Unable to find a valid data column in set %d in file %s. No data imported.",set,minfo.filename);
//errmsg(newlegend);
errmsg((QObject::tr("Unable to find a valid data column in set ")+QString::number(set)+QObject::tr(" in file ")+QString(minfo.filename)+QObject::tr(". No data imported.")).toLocal8Bit().constData());
continue;//--> next set
}
//we have one X-column in the set --> find the other sets
cur_y_col=cur_s_col=-1;
    for (int i=0;i<minfo.set[set].csv_column_formats.length();i++)
    {
    if (i==cur_x_col) continue;//we skip the X-column
        if (minfo.set[set].csv_column_formats.at(i)>=0)//we found a column with valid data format --> use this as Y
        {
        cur_y_col=i;
        //test for following string-column:
            if (i<minfo.set[set].csv_column_formats.length()-1)
            {
                if (minfo.set[set].csv_column_formats.at(i+1)<0)//there is a String-column (or INVALID-column) after the X-column
                {
                cur_s_col=i+1;
                i++;//we already looked at the next column --> ignore it next time
                }
            }
        }
        else if (cur_y_col==-1)//we found a string column, but there is no Y-column before
        {
        cur_s_col=i;
        }

//now we actually create a new set
source_columns.clear();
source_columns << cur_x_col;
if (cur_y_col>=0) source_columns << cur_y_col;
else source_columns << -2;//just a column of zeros
if (cur_s_col>=0) source_columns << cur_s_col;//if this is added we have more than 2 source-columns, that tells the create-function, that this is a string-column
//Warning: it is possible to have just X and STRING here! --> Y=0
cur_s_id=-1;//no set id so far --> a new one will be created
setl=constructSetFromCSV(minfo,set,cur_g_id,cur_s_id,SET_XY,source_columns);
generateCommentAndLegendAfterImport(minfo.filename,set,cur_x_col,cur_y_col,newcomment,newlegend);
if (inhibit_set_legend==0)
set_legend_string(cur_g_id,cur_s_id,newlegend);
setcomment(cur_g_id,cur_s_id,newcomment);
        //save set-id(s) for later undo
        new_graph_ids << cur_g_id;
        new_set_ids << cur_s_id;
        //reset columns
        cur_y_col=cur_s_col=-1;
    }//end of loop over all columns
}//end of loop over all sets

/// third: undo-stuff
int len=new_graph_ids.length();
int * gnos=new int[len];
int * snos=new int[len];
    for (int i=0;i<len;i++)
    {
    gnos[i]=new_graph_ids.at(i);
    snos[i]=new_set_ids.at(i);
    //qDebug() << "created G" << gnos[i] << ".S" << snos[i];
    }
SetsCreated(len,gnos,snos,UNDO_COMPLETE);
delete[] gnos;
delete[] snos;
delete[] newlegend;
delete[] newcomment;
return ret;
}

QList<QByteArray> cleanup_line_tokens(QList<QByteArray> line,char col_sep,QString & comment,bool & comment_at_end)
{
QList<QByteArray> n_line;
n_line.clear();
comment.clear();
comment_at_end=false;
QByteArray tmp_token;
int quote_start,quote_stop;
int quote_count,com_count,com_pos;
//qDebug() << "cleanup=" << line << "length=" << line.length();
for (int i=0;i<line.length();i++)
{
tmp_token.clear();
quote_count=line.at(i).count('\"');
com_count=line.at(i).count('#');
//qDebug() << "token=" << line.at(i) << "quotes=" << quote_count << "comments=" << com_count;
if (quote_count%2==0 && com_count<=0)//regular token (no comments inside, no unclosed quotations)
{
    if (line.at(i).length()>0)
    n_line << line.at(i);
continue;
}
//firstly: take care of the quotations
    if (quote_count%2>0)//odd number of quotation-marks
    {
    quote_start=i;
    quote_stop=-1;
        for (int j=quote_start+1;j<line.length();j++)
        {
        quote_count=line.at(j).count('\"');
            if (quote_count%2>0)
            {
            quote_stop=j;
            break;
            }
        }
        if (quote_start>=0 && quote_stop>=0)
        {
            //qDebug() << "start=" << quote_start << "stop=" << quote_stop;
        tmp_token.clear();
        tmp_token=line.at(quote_start);
            for (int j=quote_start+1;j<quote_stop;j++)
            {
            tmp_token+=col_sep+line.at(j);
            }
            quote_start=line.at(quote_stop).indexOf('\"');
            com_pos=line.at(quote_stop).indexOf('#',quote_start+1);
                if (com_pos>=0)
                {
                //qDebug() << "We are here (1):" << line.at(quote_stop);
                comment=line.at(quote_stop).mid(com_pos);
                    for (int j=i+1;j<line.length();j++)
                    {
                    comment+=col_sep+line.at(j);
                    }
                tmp_token+=col_sep+line.at(quote_stop).left(com_pos);
                i=line.length();
                }
                else
                {
                tmp_token+=col_sep+line.at(quote_stop);
                i=quote_stop+1;
                }
        quote_start=quote_stop=-1;
            if (!tmp_token.isEmpty())
            n_line << tmp_token;
        }
    }
//secondly: take care of comments (outside quotations)
if (i>line.length()-1) continue;//last one (unable to mend this) or '#' inside quotations
if (quote_count%2==0 && !(quote_count==0 && com_count>0))
{
    if (!line.at(i).isEmpty())
    n_line << line.at(i);
continue;
}
com_pos=line.at(i).indexOf('#');
//qDebug() << "com_pos=" << com_pos;
    if (com_pos>=0)
    {
    //qDebug() << "We are here (2):" << line.at(i);
    tmp_token=line.at(i).left(com_pos);
        if (!tmp_token.isEmpty())
        n_line << tmp_token;
    tmp_token=line.at(i).mid(com_pos);
        for (int j=i+1;j<line.length();j++)
        {
        tmp_token+=col_sep+line.at(j);
        }
    comment=tmp_token;
    comment_at_end=(com_pos>0?true:false);
    i=line.length();
    }
}
if (n_line.length()>0 && !comment.isEmpty()) comment_at_end=true;
//qDebug() << "after cleanup=" << n_line << "length=" << n_line.length();
//qDebug() << "comment=" << comment << "atEnd=" << comment_at_end;
return n_line;
}

int read_csv_file_into_tokens(struct csv_import_infos & info,int guess)
{
int ret=LoadFileIntoByteArray(QString::fromLocal8Bit(info.filename),info.ba);
if (ret!=0) return RETURN_FAILURE;
QList<int> sav_csv_column_formats;
info.csv_tokens.clear();
//qDebug() << "Original_format:" << info.csv_column_formats;
sav_csv_column_formats=info.csv_column_formats;
//if (guess>0)//we clear it every time! we set the format the first time according to the format_suggestions
info.csv_column_formats.clear();
info.csv_token_format.clear();//csv_token_format.at(i).at(j) is the format of the token number j in line number i
info.csv_token_ok.clear();
info.csv_token_value.clear();
info.csv_headerlines.clear();
info.csv_comments.clear();
info.csv_comment_line_nr.clear();
info.csv_comment_at_end.clear();
info.csv_read_started=info.csv_read_stopped=-1;
QStringList tmp_strings;
QString tmp_string,tmp_string2,tmp_comment;
char * tmp_str_array=NULL;
double single_val;
int single_format,comment_token,quote_count,endpos,emptyresidual;
bool single_ok,tmp_comment_at_end;
QList<int> format_list;
QList<bool> csv_token_line_ok;
QList<double> csv_token_line_value;
QList<QByteArray> linetokens,temp_tokens;
QByteArray single_temp_token;
info.ba.replace("\r","");
QList<QByteArray> lines=info.ba.split(info.datsep);//a list of lines (split at new-data-separator - usually newline)
//just save the headerlines
for (int i=0;i<info.headerlines;i++)
{
info.csv_headerlines << QString(lines.at(i));
}
//split the lines into tokens and read the values
for (int i=info.headerlines;i<lines.length();i++)
{
comment_token=formula_to_process::is_comment(lines.at(i));
//qDebug() << "i:" << lines.at(i) << comment_token;
if (comment_token==0 && info.csv_read_started<0)//not a comment and we have not started reading yet
{
info.csv_read_started=i;//the first line
}
else if (comment_token>0 && comment_token!=4)//actually a non-empty comment
{
info.csv_comments << lines.at(i);
info.csv_comment_line_nr << info.csv_tokens.length();//i;
info.csv_comment_at_end << false;
continue;
}
else if (comment_token==4)//only empty space --> end of set! stop reading!
{
break;
}
csv_token_line_value.clear();
csv_token_line_ok.clear();
tmp_strings.clear();
format_list.clear();
linetokens=lines.at(i).split(info.colsep);//now we have all the tokens in a line
linetokens=cleanup_line_tokens(linetokens,info.colsep,tmp_comment,tmp_comment_at_end);//to clean the tokens (the split-operation just splits, but we want no split inside quotation marks and we stop to split after '#' (comment))
if (!tmp_comment.isEmpty())
{
info.csv_comments << tmp_comment;
info.csv_comment_line_nr << info.csv_tokens.length();//i;
info.csv_comment_at_end << tmp_comment_at_end;
}
if (linetokens.length()<=0 || linetokens.at(0).isEmpty())//empty line --> stop reading (needed to know where we stopped because we need to know whether there may be more data to read)
{
info.csv_read_stopped=i-1;
break;
}
temp_tokens.clear();
//we have to clear the tokens (sometimes " are not taken into account correctly)
    for (int j=0;j<linetokens.length();j++)
    {
    quote_count=linetokens.at(j).count('\"');
//qDebug() << "linetoken:" << linetokens.at(j) << "count=" << linetokens.at(j).count('\"');
        if (quote_count%2==0)//even number of quotations (usually 0 or 2)
        {
        temp_tokens << linetokens.at(j);
        }
        else//odd number of quotations (usually just 1) --> search for the missing one
        {
        single_temp_token=linetokens.at(j);
        j++;
        if (j>=linetokens.length()) goto finish_token_corret;
        quote_count=linetokens.at(j).count('\"');
            while (quote_count%2==0)
            {
                single_temp_token.append(info.colsep);
                single_temp_token+=linetokens.at(j);
                j++;
                if (j>=linetokens.length()) goto finish_token_corret;
                quote_count=linetokens.at(j).count('\"');
            }
        single_temp_token.append(info.colsep);
        single_temp_token+=linetokens.at(j);
        finish_token_corret: temp_tokens << single_temp_token;
        }
    }
linetokens=temp_tokens;
    for (int j=0;j<linetokens.length();j++)
    {
    single_val=info.invalid_value;
    tmp_string=QString::fromLocal8Bit(linetokens.at(j));

    ret=removeQuotation(tmp_string,tmp_string2);
    tmp_string=tmp_string2;
    if (info.dec_sep_komma==true)
    {
    ///tmp_string.remove(QString("."));
    tmp_string.replace(QString(","),QString("."));
    }

//qDebug() << "rem quote: tmp_string=" << tmp_string << "tmp_string2=" << tmp_string2;
    tmp_str_array=new char[tmp_string.length()+8];
    strcpy(tmp_str_array,tmp_string.toLocal8Bit().constData());
        if (guess>0)
        {
            if (ret==1)//token has quotations --> may be a string
            {
            read_one_number(tmp_str_array,&single_format,&single_val);
                if (single_format==FORMAT_INVALID)
                format_list << FORMAT_STRING;
                else
                format_list << single_format;
            single_ok=true;
            }
            else
            {
            //try to guess the format
            read_one_number(tmp_str_array,&single_format,&single_val);
            //qDebug() << "tmp_str_array=" << tmp_str_array << "single_format=" << single_format << "single_val=" << single_val;
            format_list << single_format;
                if (single_format==FORMAT_INVALID)
                {
                single_ok=false;
                single_val=info.invalid_value;
                }
                else
                {
                single_ok=true;
                }
            }
        }
        else//format is known
        {
//qDebug() << "Format is known --> Original_format:" << info.csv_column_formats;
            if (info.csv_column_formats.length()<=j)
            {
                if (info.csv_column_formats_suggestion.length()>j)//we have enough suggestions
                {
                info.csv_column_formats << info.csv_column_formats_suggestion.at(j);
                }
                else if (info.csv_column_formats_suggestion.isEmpty()==false)
                {
                info.csv_column_formats << info.csv_column_formats_suggestion.last();
                }
                else
                {
                read_one_number(tmp_str_array,&single_format,&single_val);
                    if (single_format==FORMAT_INVALID)
                    {
                    single_ok=false;
                    single_val=info.invalid_value;
                    }
                    else
                    {
                    single_ok=true;
                    }
                info.csv_column_formats << single_format;
                }
            }
            format_list << info.csv_column_formats.at(j);
            if (info.csv_column_formats.at(j)==FORMAT_STRING)//string means: just read text, do not convert (or unable to convert)
            {
            single_ok=(bool(ret));
            }
            else
            {
            ret=read_date_or_number_in_format(tmp_str_array,info.csv_column_formats.at(j),true,&single_val,&endpos,&emptyresidual);
            single_ok=!(bool(ret));
            }
            if (single_ok==false) single_val=info.invalid_value;
        }
    csv_token_line_value << single_val;
    tmp_strings << tmp_string2;
    csv_token_line_ok << single_ok;
    delete[] tmp_str_array;
    }
info.csv_tokens << tmp_strings;
info.csv_token_format << format_list;
info.csv_token_ok << csv_token_line_ok;
info.csv_token_value << csv_token_line_value;
}//END OF READING
if (info.csv_read_stopped<0)
{
info.csv_read_stopped=lines.length()-1;
}
//guess suitable column-format (that idealy matches all entries in a column)
info.number_of_errors=0;
info.error_pos1.clear();
info.error_pos2.clear();
info.rows=info.csv_tokens.length();
QList<int> column_numbers;
QList< QList<int> > different_formats,nr_or_occurences;
QList<int> col_diff_formats,col_nr_of_occ;
int tmp_index;
different_formats.clear();
nr_or_occurences.clear();
double mean_col_count=0.0;
//qDebug() << "ERROR-Count=" << info.number_of_errors;
for (int i=0;i<info.csv_tokens.length();i++)
{
mean_col_count+=info.csv_tokens.at(i).length();
ret=column_numbers.indexOf(info.csv_tokens.at(i).length());
if (ret<0) column_numbers << info.csv_tokens.at(i).length();
    for (int j=0;j<info.csv_tokens.at(i).length();j++)
    {
            /*if (j<info.csv_column_formats.length())
            qDebug() << "Column=" << j << "Format=" << info.csv_column_formats.at(j);
            else
            qDebug() << "info.csv_column_formats.length()=" << info.csv_column_formats.length();*/
        if (info.csv_token_ok.at(i).at(j)==false && (guess==0 && info.csv_column_formats.at(j)!=FORMAT_INVALID))
        {
        info.number_of_errors++;
        info.error_pos1 << i;
        info.error_pos2 << j;
        }
    }
}
//qDebug() << "ERROR-Count=" << info.number_of_errors;
mean_col_count/=info.csv_tokens.length();
//qDebug() << "Mean_col_count=" << mean_col_count << "rounded=" << round(mean_col_count);
info.columns=round(mean_col_count);//column_numbers.at(0);
//qDebug() << "Columns=" << info.columns;
info.overlong_rows.clear();
info.underlong_rows.clear();
for (int j=0;j<info.rows;j++)
{
    if (info.csv_token_ok.at(j).length()<info.columns)
    {
    info.underlong_rows << j;
    }
    else if (info.csv_token_ok.at(j).length()>info.columns)
    {
    info.overlong_rows << j;
    }
}
/*qDebug() << "underlong=" << info.underlong_rows;
qDebug() << "overlong =" << info.overlong_rows;
qDebug() << "token_format.length()=" << info.csv_token_format.length();*/
int st_index=0;
for (int i=0;i<info.columns;i++)
{
col_diff_formats.clear();
col_nr_of_occ.clear();
st_index=0;
while (info.csv_token_format.length()>st_index && info.csv_token_format.at(st_index).length()<=i) st_index++;
//qDebug() << "st_index=" << st_index;
col_diff_formats << info.csv_token_format.at(st_index).at(i);
col_nr_of_occ << 1;
    for (int j=st_index+1;j<info.rows;j++)
    {
        if (info.csv_token_format.at(j).length()<=i) continue;
        /// todo: check for missing entries for the last column(s) --> a line may have less entries than the others
        tmp_index=col_diff_formats.indexOf(info.csv_token_format.at(j).at(i));
        if (tmp_index<0)
        {
        col_diff_formats << info.csv_token_format.at(j).at(i);
        col_nr_of_occ << 1;
        }
        else
        {
        col_nr_of_occ.replace(tmp_index,1+col_nr_of_occ.at(tmp_index));
        }
    }
different_formats << col_diff_formats;
nr_or_occurences << col_nr_of_occ;
}
int max,maxindex;
QList<int> most_formats;
for (int i=0;i<different_formats.length();i++)//go through all columns and find the format that occures the most
{
maxindex=0;
max=nr_or_occurences[i].at(0);
    for (int j=1;j<nr_or_occurences[i].length();j++)
    {
        if (nr_or_occurences[i].at(j)>max)
        {
        max=nr_or_occurences[i].at(j);
        maxindex=j;
        }
    }
most_formats << different_formats[i].at(maxindex);
}
//qDebug() << "column_formats:" << different_formats << "Occurences:" << nr_or_occurences << "MaxFormats:" << most_formats;
bool all_equal=true,overlong_comment_only=true;
for (int i=1;i<column_numbers.length();i++)
{
    if (info.columns>column_numbers.at(i))
    {
    //info.columns=column_numbers.at(i);
    all_equal=false;
    }
}
    //qDebug() << "ERROR-Count=" << info.number_of_errors;
//now we look for the rows with too many/few columns
//info.overlong_rows.clear();
//info.underlong_rows.clear();
if (all_equal==false)
{
    for (int i=0;i<info.csv_tokens.length();i++)
    {
        if (info.columns!=info.csv_tokens.at(i).length())//current row is longer than usual
        {
        overlong_comment_only=true;//we assume we only have comments after the usual column-count
            for (int j=info.columns;j<info.csv_tokens.at(i).length();j++)
            {
            comment_token=formula_to_process::is_comment(info.csv_tokens.at(i).at(j));
                if (comment_token>0 && comment_token!=4)//it is a comment (and not empty) --> we ignore the rest
                {
                break;
                }
                else if (comment_token==0)//not a comment --> this is a problem
                {
                overlong_comment_only=false;
                }
            }
        //if (overlong_comment_only==false) info.overlong_rows << i;
        }
    }
}
//qDebug() << "before guess>0";
//now we guess the column-formats if not already done
if (guess>0)
{
QList< QList<int> > sum_column_formats;
QList< QList<int> > sum_column_formats_occurence;
QList<int> column_formats_1;
QList<int> column_formats_2;
    for (int i=0;i<info.columns;i++)
    {
    column_formats_1.clear();
    column_formats_2.clear();
    column_formats_1 << info.csv_token_format.at(0).at(i);
    column_formats_2 << 1;
    sum_column_formats << column_formats_1;
    sum_column_formats_occurence << column_formats_2;
    }
    for (int i=1;i<info.csv_token_format.length();i++)//go through the rows and try to find different formats
    {
        for (int j=0;j<info.columns;j++)
        {
            if (info.csv_token_format.at(i).length()<=j) continue;
            ret=sum_column_formats.at(j).indexOf(info.csv_token_format.at(i).at(j));
            if (ret<0)//new format
            {
            sum_column_formats[j] << info.csv_token_format.at(i).at(j);
            sum_column_formats_occurence[j] << 1;
            }
            else//existing format --> just increase the counter
            {
            sum_column_formats_occurence[j].replace(ret,1+sum_column_formats_occurence[j].at(ret));
            }
        }
    }
info.csv_column_formats.clear();
int std_format;
    for (int i=0;i<info.columns;i++)
    {
    std_format=sum_column_formats.at(i).at(0);
        for (int j=1;j<sum_column_formats.at(i).length();j++)
        {
        ret=superior_format(std_format,sum_column_formats.at(i).at(j));
            if (ret==1) std_format=sum_column_formats.at(i).at(j);
            else if (ret==-1)
            {
            std_format=FORMAT_INVALID;
            break;
            }
        }
    info.csv_column_formats << std_format;
    }

    for (int i=0;i<info.columns;i++)
    {
        if (info.csv_column_formats.at(i)==FORMAT_INVALID)
        {
            /*for (int j=0;j<info.rows;j++)
            {
            qDebug() << j << "Format=" << info.csv_token_format.at(j).at(i) << "Value=" << info.csv_token_value.at(j).at(i);
            }*/
        reread_csv_token_column(info,i,most_formats.at(i));
        }
        else
        {
            /*for (int j=0;j<info.rows;j++)
            {
                if (info.csv_token_format.at(j).at(i)!=info.csv_column_formats.at(i))
                {
                qDebug() << "ReReading Token " << j << "/" << i;
                }
            }*/
        reread_csv_token_column(info,i,info.csv_column_formats.at(i));
        }
    }

}//end guess==true
    //qDebug() << "ERROR-Count=" << info.number_of_errors;
//qDebug() << "end guess==true";
//now we eliminate the empty columns
//qDebug() << "looking for empty columns";
bool is_empty;
QList<int> to_remove;
to_remove.clear();
for (int i=0;i<info.columns;i++)
{
is_empty=true;
    for (int j=0;j<info.rows;j++)
    {
    if (info.csv_tokens.at(j).at(i).isEmpty()==false) {is_empty=false; break;}
    }
    if (is_empty==true)
    {
    //qDebug() << "Column" << i << "is EMPTY!";
    to_remove << i;
    }
}
QStringList tmpL1;
QList<int> tmpL2;
QList<bool> tmpL3;
QList<double> tmpL4;
//qDebug() << "toRemove=" << to_remove;
if (to_remove.length()>0)
{
//qDebug() << "csv_column_formats";
//qDebug() << "Vorher=" << info.csv_column_formats;
    for (int i=to_remove.length()-1;i>=0;i--)
    info.csv_column_formats.removeAt(to_remove.at(i));
//qDebug() << "Nachher=" << info.csv_column_formats;
//qDebug() << "all_rows";
for (int j=0;j<info.rows;j++)
{
tmpL1=info.csv_tokens.at(j);
tmpL2=info.csv_token_format.at(j);
tmpL3=info.csv_token_ok.at(j);
tmpL4=info.csv_token_value.at(j);
    for (int i=to_remove.length()-1;i>=0;i--)
    {
    tmpL1.removeAt(to_remove.at(i));
    tmpL2.removeAt(to_remove.at(i));
    tmpL3.removeAt(to_remove.at(i));
    tmpL4.removeAt(to_remove.at(i));
    }
info.csv_tokens.replace(j,tmpL1);
info.csv_token_format.replace(j,tmpL2);
info.csv_token_ok.replace(j,tmpL3);
info.csv_token_value.replace(j,tmpL4);
}
info.columns-=to_remove.length();
}
//qDebug() << "removing done";
    //qDebug() << "ERROR-Count=" << info.number_of_errors;
(void)error_counter_in_csv_import(info);
    //qDebug() << "ERROR-Count=" << info.number_of_errors;
    if (info.number_of_errors==0)
    return RETURN_SUCCESS;
    else
    return RETURN_FAILURE;
}

void writeOneLine(csv_import_infos & info,QString & text,int i)//write row i of info into text
{
static char buffer[4096];
static double displ_val;
int add_comment,real_comment_pos,comment_offset;

add_comment=-1;
if (info.csv_comments.length()>0 && info.htmlShowComments)
{
    real_comment_pos=comment_offset=0;
    for (int j=0;j<info.csv_comments.length();j++)
    {
    real_comment_pos=info.csv_comment_line_nr.at(j);
        /*if (info.csv_comment_at_end.at(j)==false)
        {
        real_comment_pos-=comment_offset;
        comment_offset++;
        }*/
        if (real_comment_pos==i)
        {
        if (info.csv_comment_at_end.at(j)==true) add_comment=j;
        else
        {
            text+=QString("<tr>");
            text+=QString("<td colspan=\"")+QString::number(info.columns+1)+QString("\">");
            text+=ColorToHtml(4);
            text+=info.csv_comments.at(j);
            text+=ColorHtmlEnd();
            text+=QString("</td>");
            text+=QString("</tr>");
        }
        break;
        }
    }
}
    text+=QString("<tr>");
    text+=QString("<td>");
    text+=QString::number(i);
    text+=QString("</td>");
        for (int j=0;j<info.csv_token_value.at(i).length();j++)//columns
        {
        text+=QString("<td> &nbsp; &emsp; ");
        if (info.htmlShowOriginal==true || (info.htmlShowValues==false && info.htmlShowOriginal==false))//htmlShowValues,htmlShowOriginal
        {
            if (info.csv_tokens.at(i).at(j).isEmpty())
            {
            text+=ColorToHtml(2);
            text+=QString("---");
            }
            else
            {
                if (info.htmlShowValues==false && info.csv_token_ok.at(i).at(j)==false)// || (info.csv_column_formats.at(j)<0 || info.csv_column_formats.at(j)>=NUM_FMT_OPTION_ITEMS)))
                text+=ColorToHtml(2);
                else
                text+=ColorToHtml(1);
            text+=info.csv_tokens.at(i).at(j);
            }
        text+=ColorHtmlEnd();
        }
        if (info.htmlShowValues==true)
        {
            if (info.csv_token_ok.at(i).at(j)==true && !(info.csv_column_formats.at(j)<0 || info.csv_column_formats.at(j)>=NUM_FMT_OPTION_ITEMS))
            {
            text+=ColorToHtml(3);
            }
            else
            {
            text+=ColorToHtml(2);
            }
                if (info.csv_column_formats.at(j)<0 || info.csv_column_formats.at(j)>=NUM_FMT_OPTION_ITEMS)
                displ_val=info.invalid_value;//Strings always have an invalid data value
                else
                displ_val=info.csv_token_value.at(i).at(j);
            if (info.htmlShowOriginal==true)
            sprintf(buffer," (%g)",displ_val);
            else
            sprintf(buffer,"%g",displ_val);
        text+=QString(buffer);
        text+=ColorHtmlEnd();
        }
        text+=QString("</td>");
        }
        for (int j=info.csv_token_value.at(i).length();j<info.columns;j++)
        {
        text+=QString("<td> &nbsp; &emsp; ");
            if (info.htmlShowOriginal==true || (info.htmlShowValues==false && info.htmlShowOriginal==false))//htmlShowValues,htmlShowOriginal
            {
                text+=ColorToHtml(2);
                text+=QString("---");
                text+=ColorHtmlEnd();
            }
            if (info.htmlShowValues==true)
            {
                text+=ColorToHtml(2);
                    if (info.htmlShowOriginal==true)
                    sprintf(buffer," (%g)",info.invalid_value);
                    else
                    sprintf(buffer,"%g",info.invalid_value);
            text+=QString(buffer);
            text+=ColorHtmlEnd();
            }
        text+=QString("</td>");
        }
        if (add_comment>=0 && info.htmlShowComments==true)
        {
        text+=QString("<td>");
        text+=ColorToHtml(4);
        text+=info.csv_comments.at(add_comment);
        text+=ColorHtmlEnd();
        text+=QString("</td>");
        }
    text+=QString("</tr>\n");
}

QString convert_csv_contents_to_html(csv_import_infos & info)
{
//char buffer[4096];
QString text;
int prim_start,prim_stop;//the first part of lines (may be everything)
int sec_start,sec_stop;//the second part of lines (if it exists)
bool show_dots=false;
int prev_row,n_error_row,error_row[3],index1,index2,index3;
prim_start=-1;
prim_stop=-2;
sec_start=-1;
sec_stop=-2;
//qDebug() << "CONVERT_CSV_CONTENTS_TO_HTML";
//qDebug() << "start=" << info.startValues << "end=" << info.endValues;
//qDebug() << "comments_on=" << info.htmlShowComments;
if (info.startValues==-1)//show everything
{
prim_start=0;
prim_stop=info.rows;
//no second part
}
else//show only a part
{
prim_start=0;
prim_stop=info.startValues;
    if (prim_stop>=info.rows || info.startValues+info.endValues>=info.rows)//we have already to show more lines than there is data present
    {
    prim_stop=info.rows;
    }
    else
    {
        if (info.endValues<0)
        {
        show_dots=true;
        }
        else
        {
        show_dots=true;
        sec_stop=info.rows;
        sec_start=sec_stop-info.endValues;
        }
    }
}
text+=QString("<table>");
///header=the column-formats
text+=QString("<tr>");
for (int i=0;i<=info.columns;i++)
{
    text+=QString("<th align=\"left\">");
    if (i==0)
    {
    text+=QString("Row");
    }
    else
    {
    text+=QString(" &nbsp; &emsp; [C")+QString::number(i-1)+QString("]:");
    /*if (info.csv_column_formats.length()<=i-1)
    {
        qDebug() << "WARNING: FORMAT COLUMN NOT LONG ENOUGH:" << info.csv_column_formats.length();
    }*/
        if (info.csv_column_formats.at(i-1)<0 || info.csv_column_formats.at(i-1)>=NUM_FMT_OPTION_ITEMS)
        {
        text+=QString("INVALID(String?)");
        }
        else
        {
        text+=QString(fmt_option_items[info.csv_column_formats.at(i-1)].label);
        }
    }
    text+=QString("</th>");
}
text+=QString("</tr>\n");
//qDebug() << "startline=" << prim_start << "end_first_part=" << prim_stop;
///data (first part)
for (int i=prim_start;i<prim_stop;i++)
{
if (i<0||i>=info.csv_token_value.length()) continue;//no data in this row-range
writeOneLine(info,text,i);
prev_row=i;
}
    if (info.htmlShowErrorLines==true && info.startValues!=-1)//if startValues==-1 everything will be shown anyway
    {
        index1=index2=index3=0;
        n_error_row=error_row[0]=error_row[1]=error_row[2]=-1;
        while (index1>=0 || index2>=0 || index3>=0)
        {
        if (index1>=info.error_pos1.length()) index1=-1;
        if (index2>=info.underlong_rows.length()) index2=-1;
        if (index3>=info.overlong_rows.length()) index3=-1;
            if (index1>=0)
            error_row[0]=info.error_pos1.at(index1);
            else
            error_row[0]=-1;
            if (index2>=0)
            error_row[1]=info.underlong_rows.at(index2);
            else
            error_row[1]=-1;
            if (index3>=0)
            error_row[2]=info.overlong_rows.at(index3);
            else
            error_row[2]=-1;

            if (error_row[0]>=0) n_error_row=error_row[0];
            if (error_row[1]>=0 && (error_row[1]<n_error_row || n_error_row==-1)) n_error_row=error_row[1];
            if (error_row[2]>=0 && (error_row[2]<n_error_row || n_error_row==-1)) n_error_row=error_row[2];

            if (error_row[0]>=0 && error_row[0]==n_error_row) index1++;
            if (error_row[1]>=0 && error_row[1]==n_error_row) index2++;
            if (error_row[2]>=0 && error_row[2]==n_error_row) index3++;

            if (n_error_row>=0)
            {
            if (n_error_row<prim_stop || (n_error_row>=sec_start && sec_start>=0) || prev_row==n_error_row) continue;//row will be shown anyway
            if (n_error_row>prev_row+1) text+=QString("<tr><td>...</td></tr>\n");
                writeOneLine(info,text,n_error_row);
                prev_row=n_error_row;
            }

        }

    }
    //else qDebug() << "ShowErrorLines==FALSE";

    /// the '...' to indicate missing lines
    if (show_dots==true && (prev_row<sec_start-1))
    text+=QString("<tr><td>...</td></tr>\n");

if (sec_start>=0)
{
//qDebug() << "endlines=" << info.endValues << "sec_start=" << sec_start << "sec_stop=" << sec_stop;
/// the final lines
    for (int i=sec_start;i<sec_stop;i++)
    {
    if (i<0||i>=info.csv_token_value.length()) continue;//no data in this range
    writeOneLine(info,text,i);
    }
}
text+=QString("</table>");
return text;
}

QString convert_csv_contents_to_html(csv_multi_import_infos & info)//, int startline, int maxlines,int endlines, bool include_comments)
{
QString text;
text.clear();
text+=QString("<p>File: ")+QString(info.filename)+QString("</p>");
if (info.htmlShowHeaderLines==true && info.headerlines>0)
{
text+=ColorToHtml(4);
text+=QString("<p>--- Start: Ignored header lines ---<br>");
for (int j=0;j<info.ignoredHeaderLines.length();j++)
text+=info.ignoredHeaderLines.at(j)+QString("<br>");
text+=QString("--- End: Ignored header lines ---</p>");
text+=ColorHtmlEnd();
}
text+=QString("<p>")+QString::number(info.nr_of_sets)+QString(" Set");
if (info.nr_of_sets>1) text+=QString("s");
text+=QString("</p>");
for (int i=0;i<info.nr_of_sets;i++)
{
    info.set[i].htmlShowValues=info.htmlShowValues;
    info.set[i].htmlShowComments=info.htmlShowComments;
    info.set[i].htmlShowOriginal=info.htmlShowOriginal;
    info.set[i].htmlShowErrorLines=info.htmlShowErrorLines;
    info.set[i].htmlShowHeaderLines=info.htmlShowHeaderLines;
    info.set[i].startValues=info.startValues;
    info.set[i].endValues=info.endValues;
info.set[i].text_as_html=convert_csv_contents_to_html(info.set[i]);//,startline,maxlines,endlines,include_comments);
text+=QString("<p>Set ")+QString::number(i)+QString(" (")+QString::number(info.set[i].rows)+QString(" Row");
if (info.set[i].rows>1) text+=QString("s");
text+=QString(", ")+QString::number(info.set[i].columns)+QString(" Column");
if (info.set[i].columns>1) text+=QString("s");
text+=QString(")</p>");
text+=info.set[i].text_as_html+QString("\n");
if (i<info.nr_of_sets-1) text+=QString("<p> </p>\n");
}
return text;
}

QString convert_csv_errors_to_html(csv_import_infos & info)
{
QString text;
int tmp_type;
double tmp_val;
text.clear();
if (info.number_of_errors<=0)
{
text+=ColorToHtml(3);
text+=QString("<table><tr><td>No errors.</td></tr></table>\n");
text+=ColorHtmlEnd();
}
else
{
text+=QString("<table>");
text+=QString("<tr><td>");
text+=ColorToHtml(2);
text+=QString::number(info.number_of_errors);
text+=QString(" error");
    if (info.number_of_errors>1)
    text+=QString("s");
text+=QString(" during data input.");
text+=ColorHtmlEnd();
text+=QString("</td></tr>\n");
int errors_in_col,error_pos;
    for (int i=0;i<info.columns;i++)
    {
    errors_in_col = info.error_pos2.count(i);
        if (errors_in_col>0)
        {
            text += QString("<tr><td>Column ")+QString::number(i)+QString(": ")+QString::number(errors_in_col)+QString(" error");
            if (errors_in_col>1) text += QString("s");
            text += QString(" in column (")+QString::number(errors_in_col*100.0/info.rows,'g',3)+QString("% of all entries).</td></tr><tr><td>Column-format=");
            if (info.csv_column_formats.at(i)<0 || info.csv_column_formats.at(i)>=NUM_FMT_OPTION_ITEMS)
            {
            text+=QString("INVALID(String?)");
            }
            else
            {
            text+=QString(fmt_option_items[info.csv_column_formats.at(i)].label);
            }
        text+=QString("</td></tr>");
        error_pos=-1;
        error_pos=info.error_pos2.indexOf(i,error_pos+1);
            while (error_pos>=0)
            {
            text+=QString("<tr><td>Error in row ")+QString::number(info.error_pos1.at(error_pos))+QString(": data read from file=[")+info.csv_tokens.at(info.error_pos1.at(error_pos)).at(info.error_pos2.at(error_pos))+QString("]")+QString("</td></tr>");
                if (info.csv_tokens.at(info.error_pos1.at(error_pos)).at(info.error_pos2.at(error_pos)).isEmpty())
                {
                    text+=QString("<tr><td>Empty entry - unable to read/guess value.</td></tr>");
                }
                else
                {
                    read_one_number(info.csv_tokens.at(info.error_pos1.at(error_pos)).at(info.error_pos2.at(error_pos)).toLatin1().data(),&tmp_type,&tmp_val);
                    text+=QString("<tr><td>Guessed data format: ");
                    if (tmp_type<0 || tmp_type>=NUM_FMT_OPTION_ITEMS)
                    {
                    text+=QString("INVALID(String?)");
                    }
                    else
                    {
                    text+=QString(fmt_option_items[tmp_type].label);
                    }
                    text+=QString(" / read data value=")+QString::number(tmp_val)+QString("</td></tr>");
                    if (tmp_type!=info.csv_column_formats.at(i))
                    text+=QString("<tr><td>Data format does NOT match column-format.</td></tr>");
                }
            error_pos=info.error_pos2.indexOf(i,error_pos+1);
            }
        }
    }
if (info.underlong_rows.length()>0)
{
text+=QString("<tr><td>");
    if (info.underlong_rows.length()>1)
    text+=QString::number(info.underlong_rows.length())+QString(" rows have");
    else
    text+=QString::number(info.underlong_rows.length())+QString(" row has");
text+=QString(" too few columns. Missing data is replaced by invalid value (")+QString::number(info.invalid_value)+QString(").");
text+=QString("</tr></td>");
    for (int i=0;i<info.underlong_rows.length();i++)
    {
    text+=QString("<tr><td>");
    text+=QString("Row ")+QString::number(info.underlong_rows.at(i))+QString(" just has ")+QString::number(info.csv_tokens.at(info.underlong_rows.at(i)).length())+QString(" columns (")+QString::number(info.columns-info.csv_tokens.at(info.underlong_rows.at(i)).length())+QString(" missing)");
    text+=QString("</tr></td>");
    }
}
if (info.overlong_rows.length()>0)
{
text+=QString("<tr><td>");
    if (info.overlong_rows.length()>1)
    text+=QString::number(info.overlong_rows.length())+QString(" rows have");
    else
    text+=QString::number(info.overlong_rows.length())+QString(" row has");
text+=QString(" too many columns. Data in excess-columns will be ignored.");
text+=QString("</tr></td>");
    for (int i=0;i<info.overlong_rows.length();i++)
    {
    text+=QString("<tr><td>");
    text+=QString("Row ")+QString::number(info.overlong_rows.at(i))+QString(" has ")+QString::number(info.csv_tokens.at(info.overlong_rows.at(i)).length())+QString(" columns (")+QString::number(-info.columns+info.csv_tokens.at(info.overlong_rows.at(i)).length())+QString(" too many)");
    text+=QString("</tr></td>");
    }
}
text+=QString("</table>");
}
return text;
}

QString convert_csv_errors_to_html(csv_multi_import_infos & info)
{
QString text;
text.clear();
/*text+=QString("<p>File: ")+QString(info.filename)+QString("</p>");
text+=QString("<p>")+QString::number(info.nr_of_sets)+QString(" Set");
if (info.nr_of_sets>1) text+=QString("s");
text+=QString("</p>");*/
for (int i=0;i<info.nr_of_sets;i++)
{
info.set[i].errors_as_html=convert_csv_errors_to_html(info.set[i]);
text+=QString("<p>Set ")+QString::number(i)+QString(" (")+QString::number(info.set[i].rows)+QString(" Row");
if (info.set[i].rows>1) text+=QString("s");
text+=QString(", ")+QString::number(info.set[i].columns)+QString(" Column");
if (info.set[i].columns>1) text+=QString("s");
text+=QString("):</p>");
text+=info.set[i].errors_as_html+QString("\n");
if (i<info.nr_of_sets-1) text+=QString("<p> </p>\n");
}
return text;
}

QString convert_csv_errors_to_text(csv_import_infos & info)
{
QString text;
int tmp_type;
double tmp_val;
text.clear();
if (info.number_of_errors<=0)
{
text+=QString("No errors.\n");
}
else
{
text+=QString::number(info.number_of_errors);
text+=QString(" error");
    if (info.number_of_errors>1)
    text+=QString("s");
text+=QString(" during data input.\n");
int errors_in_col,error_pos;
    for (int i=0;i<info.columns;i++)
    {
    errors_in_col = info.error_pos2.count(i);
        if (errors_in_col>0)
        {
            text += QString("Column ")+QString::number(i)+QString(": ")+QString::number(errors_in_col)+QString(" error");
            if (errors_in_col>1) text += QString("s");
            text += QString(" in column (")+QString::number(errors_in_col*100.0/info.rows,'g',3)+QString("% of all entries). Column-format=");
            if (info.csv_column_formats.at(i)<0 || info.csv_column_formats.at(i)>=NUM_FMT_OPTION_ITEMS)
            {
            text+=QString("INVALID(String?)");
            }
            else
            {
            text+=QString(fmt_option_items[info.csv_column_formats.at(i)].label);
            }
            text+=QString("\n");
        error_pos=-1;
        error_pos=info.error_pos2.indexOf(i,error_pos+1);
            while (error_pos>=0)
            {
            text+=QString("Error in row ")+QString::number(info.error_pos1.at(error_pos))+QString(": data read from file=[")+info.csv_tokens.at(info.error_pos1.at(error_pos)).at(info.error_pos2.at(error_pos))+QString("]\n");
                if (info.csv_tokens.at(info.error_pos1.at(error_pos)).at(info.error_pos2.at(error_pos)).isEmpty())
                {
                    text+=QString("Empty entry - unable to read/guess value.");
                }
                else
                {
                    read_one_number(info.csv_tokens.at(info.error_pos1.at(error_pos)).at(info.error_pos2.at(error_pos)).toLatin1().data(),&tmp_type,&tmp_val);
                    text+=QString("Guessed data format: ");
                    if (tmp_type<0 || tmp_type>=NUM_FMT_OPTION_ITEMS)
                    {
                    text+=QString("INVALID(String?)");
                    }
                    else
                    {
                    text+=QString(fmt_option_items[tmp_type].label);
                    }
                    text+=QString(" / read data value=")+QString::number(tmp_val)+QString("\n");
                    if (tmp_type!=info.csv_column_formats.at(i))
                    text+=QString("Data format does NOT match column-format.\n");
                }
            error_pos=info.error_pos2.indexOf(i,error_pos+1);
            }
        }
    }
if (info.underlong_rows.length()>0)
{
    if (info.underlong_rows.length()>1)
    text+=QString::number(info.underlong_rows.length())+QString(" rows have");
    else
    text+=QString::number(info.underlong_rows.length())+QString(" row has");
text+=QString(" too few columns. Missing data is replaced by invalid value (")+QString::number(info.invalid_value)+QString(").\n");
    for (int i=0;i<info.underlong_rows.length();i++)
    {
    text+=QString("Row ")+QString::number(info.underlong_rows.at(i))+QString(" just has ")+QString::number(info.csv_tokens.at(info.underlong_rows.at(i)).length())+QString(" columns (")+QString::number(info.columns-info.csv_tokens.at(info.underlong_rows.at(i)).length())+QString(" missing)");
    }
}
if (info.overlong_rows.length()>0)
{
    if (info.overlong_rows.length()>1)
    text+=QString::number(info.overlong_rows.length())+QString(" rows have");
    else
    text+=QString::number(info.overlong_rows.length())+QString(" row has");
text+=QString(" too many columns. Data in excess-columns will be ignored.\n");
    for (int i=0;i<info.overlong_rows.length();i++)
    {
    text+=QString("Row ")+QString::number(info.overlong_rows.at(i))+QString(" has ")+QString::number(info.csv_tokens.at(info.overlong_rows.at(i)).length())+QString(" columns (")+QString::number(-info.columns+info.csv_tokens.at(info.overlong_rows.at(i)).length())+QString(" too many)");
    }
}
//text+=QString("</table>");
}
return text;
}

QString convert_csv_errors_to_text(csv_multi_import_infos & info)
{
QString text;
text.clear();
for (int i=0;i<info.nr_of_sets;i++)
{
info.set[i].errors_as_html=convert_csv_errors_to_text(info.set[i]);
text+=QString("Set ")+QString::number(i)+QString(" (")+QString::number(info.set[i].rows)+QString(" Row");
if (info.set[i].rows>1) text+=QString("s");
text+=QString(", ")+QString::number(info.set[i].columns)+QString(" Column");
if (info.set[i].columns>1) text+=QString("s");
text+=QString("):\n");
text+=info.set[i].errors_as_html+QString("\n");
if (i<info.nr_of_sets-1) text+=QString("\n");
}
return text;
}

int get_csv_column(csv_import_infos & info, int col, double * v)
{
int count=0;
if (col<0 || col>=info.columns) return -1;
for (count=0;count<info.rows;count++)
{
    if (info.csv_token_value.at(count).length()<=col)
    {
    v[count]=info.invalid_value;
    continue;
    }
    /*if (info.csv_token_ok.at(count).at(col)==false)
    v[count]=info.invalid_value;
    else*/
    v[count]=info.csv_token_value.at(count).at(col);
}
return count;
}

int get_csv_column(csv_multi_import_infos & minfo, int setid, int col, double * v)
{
if (minfo.nr_of_sets<=setid) return -1;
return get_csv_column(minfo.set[setid],col,v);
}

int get_csv_string_column(csv_import_infos & info, int col, QStringList & slist)
{
slist.clear();
if(info.columns>col && col>=0)
{
    for (int i=0;i<info.rows;i++)
    {
        if (info.csv_tokens.at(i).length()<=col)
        {
        slist << QObject::tr("");//QObject::tr("VOID");
        continue;
        }
    slist << info.csv_tokens.at(i).at(col);
    }
}
return slist.length();
}

int get_csv_string_column(csv_multi_import_infos & minfo, int setid, int col, QStringList & slist)
{
if (minfo.nr_of_sets<=setid || setid<0) return -1;
return get_csv_string_column(minfo.set[setid],col,slist);
}

inline void setDataToZero(double * data,int len)//{0,0,0,0,....,0}
{
for (int i=0;i<len;i++) data[i]=0.0;
}

inline void setDataToValue(double * data,int len,double x)//{x,x,x,x,....,x}
{
for (int i=0;i<len;i++) data[i]=x;
}

inline void setDataToData(double * data,int len,double * data2,int len2)//set data to data2, if data2 is shorter, then the rest will be set to 0
{
int len0=len;
if (len2<len) len0=len2;
    memcpy(data,data2,len0*sizeof(double));
    if (len2<len) setDataToZero(data+len0,len-len2);
}

inline void setDataToIndex(double * data,int len)//{0,1,2,3,...,len-1}
{
for (int i=0;i<len;i++) data[i]=i;
}

void computeData(double * data1,int len1,double * data2,int len2,int type)//data1=data1+data2 if type=1; if the length of data1 and data2 does not match len1 will be used (missing data will be set to 0)
{//type=operation, type=2=-, type=3=*, ...
int len0=len1;
if (len2<len1) len0=len2;
switch (type)
{
default://do nothing
break;
case 1://"+"
    for (int i=0;i<len0;i++)
    {
    data1[i]+=data2[i];
    }
    //for the rest data2=0 --> nothing more to do since a+0=a
break;
case 2://"-"
    for (int i=0;i<len0;i++)
    {
    data1[i]-=data2[i];
    }
    //for the rest data2=0 --> nothing more to do since a-0=a
break;
case 3://"*"
    for (int i=0;i<len0;i++)
    {
    data1[i]*=data2[i];
    }
    for (int i=len0;i<len1;i++)//data2 is too short --> data2=0 --> a*0=0
    {
    data1[i]=0.0;
    }
break;
case 4://"/"
    for (int i=0;i<len0;i++)
    {
    data1[i]/=data2[i];
    }
    //since a/0=undefined, we assume for the rest data2=1 --> nothing more to do since a/1=a
break;
case 5://"^"
    for (int i=0;i<len0;i++)
    {
    data1[i]=pow(data1[i],data2[i]);
    }
    for (int i=len0;i<len1;i++)//data2 is too short --> data2=0 --> a^0=1
    {
    data1[i]=1.0;
    }
break;
case 6://"*10^"
    for (int i=0;i<len0;i++)
    {
    data1[i]=data1[i]*pow(10.0,data2[i]);
    }
    //data2 is too short --> data2=0 --> a*10^0=a*1=a
break;
case 7://"*2^"
    for (int i=0;i<len0;i++)
    {
    data1[i]=data1[i]*pow(2.0,data2[i]);
    }
    //data2 is too short --> data2=0 --> a*2^0=a*1=a
break;
}
}

int checkCSVusedColumns(csv_multi_import_infos & minfo,int set,int column)//looks in set for column; if it is unused yet --> returns columns, otherwise returns next unused column, -1 if nothing left
{
if (set<0 || column<0 || minfo.nr_of_sets<=set) return -1;
if (minfo.set[set].columns<=column) return -1;
bool used=minfo.set[set].usedIndices.contains(column);
int n_col=column+1;
if (used==false)
{
return column;
}
else
{
while (minfo.set[set].usedIndices.contains(n_col)==true)
{
n_col++;
}
    if (minfo.set[set].columns>n_col)//enough columns
    return n_col;
    else
    return -1;
}
}

int doSingleCSVAdvancement(csv_multi_import_infos & minfo,int nr,int count)
{
int initial_set,initial_col,adv_type;
int ref_set,ref_col,cur_ref_col,cur_ref_set,ref_adv;
int new_set,new_col,col_format;
new_set=new_col=ref_set=ref_col=cur_ref_col=cur_ref_set=-1;
///initial settings
initial_set=minfo.postprocessing_order3.at(minfo.advancement_order.at(nr));
initial_col=minfo.postprocessing_order4.at(minfo.advancement_order.at(nr));
    //qDebug() << "initial_set=" << initial_set << "initial_col=" << initial_col;
col_format=minfo.set[initial_set].csv_column_formats.at(initial_col);
if (minfo.postprocessing_order2.at(minfo.advancement_order.at(nr))==0)
adv_type=minfo.string_advancement;
else if (minfo.postprocessing_order2.at(minfo.advancement_order.at(nr))==1)
adv_type=minfo.postprocess_advancement1.at(minfo.postprocessing_order1.at(minfo.advancement_order.at(nr)));
else//==2
adv_type=minfo.postprocess_advancement2.at(minfo.postprocessing_order1.at(minfo.advancement_order.at(nr)));
///reference for '+n'
if (adv_type==2)
{
ref_set=minfo.postprocessing_order3.at(minfo.advancement_ref.at(nr));
ref_col=minfo.postprocessing_order4.at(minfo.advancement_ref.at(nr));
        if (minfo.postprocessing_order2.at(minfo.advancement_ref.at(nr))==0)
        {
        ref_adv=minfo.string_advancement;
        cur_ref_col=minfo.string_source_col;
        cur_ref_set=minfo.string_source_set;
        }
        else if (minfo.postprocessing_order2.at(minfo.advancement_ref.at(nr))==1)
        {
        ref_adv=minfo.postprocess_advancement1.at(minfo.postprocessing_order1.at(minfo.advancement_ref.at(nr)));
        cur_ref_col=minfo.postprocess_operation_source1.at(minfo.postprocessing_order1.at(minfo.advancement_ref.at(nr)));
        cur_ref_set=minfo.postprocess_operation_source_set1.at(minfo.postprocessing_order1.at(minfo.advancement_ref.at(nr)));
        }
        else//==2
        {
        ref_adv=minfo.postprocess_advancement2.at(minfo.postprocessing_order1.at(minfo.advancement_ref.at(nr)));
        cur_ref_col=minfo.postprocess_operation_source2.at(minfo.postprocessing_order1.at(minfo.advancement_ref.at(nr)));
        cur_ref_set=minfo.postprocess_operation_source_set2.at(minfo.postprocessing_order1.at(minfo.advancement_ref.at(nr)));
        }
}
switch (adv_type)
{
default:
case 0://'+0'
new_set=initial_set;
new_col=initial_col;
break;
case 1://'+1'
new_set=initial_set;
new_col=checkCSVusedColumns(minfo,initial_set,initial_col+count);
break;
case 2://'+n'
new_set=initial_set;
    if (ref_adv==0)//relative to '+0'
    {
    new_col=initial_col;
    }
    else if (ref_adv==1 || ref_adv==3)//relative to '+1' or 'same'
    {
    new_col=cur_ref_col+minfo.advancement_delta.at(nr);
    }
    else if (ref_adv==4)//relative to 'next set/same col'
    {
    new_set=cur_ref_set;
    new_col=cur_ref_col+minfo.advancement_delta.at(nr);
    }
    else//relative to '+n' --> i.e. only '+n'
    {
    cur_ref_col=minfo.set[cur_ref_set].lastColIndex;
    new_col=cur_ref_col+minfo.advancement_delta.at(nr);
    }
new_col=checkCSVusedColumns(minfo,initial_set,new_col);
break;
case 3://'same'
new_set=initial_set;
gatherSimilarColumnIndices(minfo.set+initial_set,col_format,initial_col);
    //qDebug() << "SimilarIndices=" << minfo.set[initial_set].similarIndices << "count=" << count;
if (minfo.set[initial_set].similarIndices.length()<=count)//not enough similar columns
{
new_col=-1;
}
else
{
new_col=count;
    while (minfo.set[initial_set].usedIndices.contains(minfo.set[initial_set].similarIndices.at(new_col))==true)
    {
    new_col++;
        if (minfo.set[initial_set].similarIndices.length()<=new_col)
        {
        new_col=-1;
        break;
        }
    }
if (new_col>=0 && minfo.set[initial_set].similarIndices.length()>new_col)
new_col=minfo.set[initial_set].similarIndices.at(new_col);
}
break;
case 4://'next set/same column'
new_set=initial_set+count;
new_col=checkCSVusedColumns(minfo,new_set,initial_col);
break;
}
if (new_set<0 || new_col<0)
{
return RETURN_FAILURE;
}
else
{
    addColumnToUsedIndices(&minfo,new_set,new_col);
    if (minfo.postprocessing_order2.at(minfo.advancement_order.at(nr))==0)
    {
    minfo.string_source_set=new_set;
    minfo.string_source_col=new_col;
    }
    else if (minfo.postprocessing_order2.at(minfo.advancement_order.at(nr))==1)
    {
    minfo.postprocess_operation_source_set1.replace(minfo.postprocessing_order1.at(minfo.advancement_order.at(nr)),new_set);
    minfo.postprocess_operation_source1.replace(minfo.postprocessing_order1.at(minfo.advancement_order.at(nr)),new_col);
    }
    else//==2
    {
    minfo.postprocess_operation_source_set2.replace(minfo.postprocessing_order1.at(minfo.advancement_order.at(nr)),new_set);
    minfo.postprocess_operation_source2.replace(minfo.postprocessing_order1.at(minfo.advancement_order.at(nr)),new_col);
    }
}
return RETURN_SUCCESS;
}

void find_max_in_list(QList<int> & list,int & max_val,int & max_index)
{
    if (list.length()<1)
    {
    max_val=max_index=-1;
    }
    else
    {
    max_val=list.at(0);
    max_index=0;
        for (int i=1;i<list.length();i++)
        {
            if (list.at(i)>max_val)
            {
            max_val=list.at(i);
            max_index=i;
            }
        }
    }
}

int doCSVAdvancement2(csv_multi_import_infos & minfo)
{
int success=RETURN_FAILURE;
int max_val,max_index;

for (int i=0;i<minfo.nr_of_sets;i++)//we need to know the last column index for the advancement later
{
max_val=max_index=-1;
find_max_in_list(minfo.set[i].usedIndices,max_val,max_index);
minfo.set[i].lastColIndex=max_val;
//qDebug() << "Set" << i << "max=" << minfo.set[i].lastColIndex << "used Indices=" << minfo.set[i].usedIndices;
}

if (minfo.number_of_possible_advancements<=0 || minfo.readAll==false) return RETURN_FAILURE;//a static set, no advancement possible
minfo.advancement_counter++;
for (int i=0;i<minfo.advancement_order.length();i++)
{
success=doSingleCSVAdvancement(minfo,i,minfo.advancement_counter);
if (success==RETURN_FAILURE) break;
}

for (int i=0;i<minfo.nr_of_sets;i++)//we need to know the last column index for the advancement later
{
max_val=max_index=-1;
find_max_in_list(minfo.set[i].usedIndices,max_val,max_index);
minfo.set[i].lastColIndex=max_val;
}
return success;
}

int generateTargetColTitle(int target_col,int s_set1,int s_source1,int type,int s_set2,int s_source2,QList<double> vals,QString & Title,QString & Source)
{
Title=QString(dataset_colname(target_col));
Source.clear();
int probe,index=0;
int one_operand=TRUE;
    if (s_source1==-200)//index
    {
    Source=QString("<Index>");
    }
    else if(s_source1==-100)//value
    {
    Source=QString::number(vals.at(index++));
    }
    else if (s_source1<0 && s_source1>=-MAX_DUMMY_COLS)//temp-col
    {
    probe=-1-s_source1;
    Source=QChar(65+probe);
    }
    else
    {
    Source=QString("Set")+QString::number(s_set1)+QString(".Col")+QString::number(s_source1);
    }

    if (type>0)
    {
        one_operand=FALSE;
        switch (type)
        {
        default://do nothing
        break;
        case 1://"+"
        Source+=QString("+");
        break;
        case 2://"-"
        Source+=QString("-");
        break;
        case 3://"*"
        Source+=QString("*");
        break;
        case 4://"/"
        Source+=QString("/");
        break;
        case 5://"^"
        Source+=QString("^");
        break;
        case 6://"*10^"
        Source+=QString("*10^");
        break;
        case 7://"*2^"
        Source+=QString("*2^");
        break;
        }
        if (s_source2==-200)//index
        {
        Source+=QString("<Index>");
        }
        else if(s_source2==-100)//value
        {
        Source+=QString::number(vals.at(index++));
        }
        else if (s_source2<0 && s_source2>=-MAX_DUMMY_COLS)//temp-col
        {
        probe=-1-s_source2;
        Source+=QChar(65+probe);
        }
        else
        {
        Source+=QString("Set")+QString::number(s_set2)+QString(".Col")+QString::number(s_source2);
        }
    }
return one_operand;
}

int generateTempColTitle(int tmp_col,QList<int> origins,QList<double> vals,QString & Title,QString & Source)
{
Title=QChar(65+tmp_col);
Source.clear();
int probe,index=0;
int one_operand=TRUE;
    if (origins.at(1)==-200)//index
    {
    Source=QString("<Index>");
    }
    else if(origins.at(1)==-100)//value
    {
    Source=QString::number(vals.at(index++));
    }
    else if (origins.at(1)<0 && origins.at(1)>=-MAX_DUMMY_COLS)//temp-col
    {
    probe=-1-origins.at(1);
    Source=QChar(65+probe);
    }
    else
    {
    Source=QString("Set")+QString::number(origins.at(0))+QString(".Col")+QString::number(origins.at(1));
    }

    if (origins.at(2)>0)
    {
        one_operand=FALSE;
        switch (origins.at(2))
        {
        default://do nothing
        break;
        case 1://"+"
        Source+=QString("+");
        break;
        case 2://"-"
        Source+=QString("-");
        break;
        case 3://"*"
        Source+=QString("*");
        break;
        case 4://"/"
        Source+=QString("/");
        break;
        case 5://"^"
        Source+=QString("^");
        break;
        case 6://"*10^"
        Source+=QString("*10^");
        break;
        case 7://"*2^"
        Source+=QString("*2^");
        break;
        }
        if (origins.at(4)==-200)//index
        {
        Source+=QString("<Index>");
        }
        else if(origins.at(4)==-100)//value
        {
        Source+=QString::number(vals.at(index++));
        }
        else if (origins.at(4)<0 && origins.at(4)>=-MAX_DUMMY_COLS)//temp-col
        {
        probe=-1-origins.at(4);
        Source+=QChar(65+probe);
        }
        else
        {
        Source+=QString("Set")+QString::number(origins.at(3))+QString(".Col")+QString::number(origins.at(4));
        }
    }
return one_operand;
}

void combineTargetColTitle(QStringList & all_temp_cols,QStringList & all_temp_col_origs,QList<int> only_one_operand_temp,QStringList & all_target_cols,QStringList & all_target_col_origs,QList<int> only_one_operand_target)
{
QString temp,temp2;
for (int i=0;i<all_temp_col_origs.length();i++)
{
    if (only_one_operand_temp.at(i)==0)
    {
    temp=QString("(")+all_temp_col_origs.at(i)+QString(")");
    all_temp_col_origs.replace(i,temp);
    }
}
for (int k=0;k<3;k++)
{
    for (int i=0;i<all_temp_col_origs.length();i++)
    {
        temp2=all_temp_cols.at(i);
        for (int j=0;j<all_temp_col_origs.length();j++)
        {
        if (i==j) continue;
        temp=all_temp_col_origs.at(j);
        if (temp.contains(temp2)==false) continue;
        temp=temp.replace(temp2,all_temp_col_origs.at(i));
        all_temp_col_origs.replace(j,temp);
        }
    }
}
for (int i=0;i<all_target_col_origs.length();i++)
{
    temp2=all_target_col_origs.at(i);
    for (int j=0;j<all_temp_col_origs.length();j++)
    {
    temp=all_temp_cols.at(j);
    if (temp2.contains(temp)==false) continue;//temp-col does not appeare here
    temp2=temp2.replace(temp,all_temp_col_origs.at(j));
    all_target_col_origs.replace(i,temp2);
    }
}

}

int constructSetFromCSVWithPostprocessing(csv_multi_import_infos & minfo,QList<int> & target_graph,QList<int> & target_set,QList<int> & usedTargetSets)
{
int ret=0,probe,tmp_len1,tmp_len2,t_col_0,t_col_1,t_col_2,unfinished_tmp_cols,ret_val=0,len0;
//postprocessing options
/*bool readAll*/
//qDebug() << "minfo format_suggestion=" << minfo.csv_column_formats_suggestion;
QString text;
char str_txt[GR_MAXPATHLEN];
double * temp_cols[MAX_DUMMY_COLS];//data for temporary columns A,B,C,...
QList<int> temp_col_origins[MAX_DUMMY_COLS];
QList<double> temp_col_vals[MAX_DUMMY_COLS];
double * temp_data_col=NULL;
int col_origins[4][minfo.targetSetCols+2],col_orig_type[minfo.targetSetCols+2];
QList<double> col_orig_vals[minfo.targetSetCols+2];
QList<int> col_occurance[DATA_BAD];
for (int i=0;i<DATA_BAD;i++) col_occurance[i].clear();
QList<bool> temp_col_present;
QList<int> temp_col_len,temp_col_target,temp_col_def_in;
QList<int> temp_col_ok,target_cols_ok;//-1=nothing done, 0=initialized with length, 1=first operand calculated, 2=second operand calculated=finished
int target_set_length,progression_good=0;
int new_id_index=0;
char n_legend[2*GR_MAXPATHLEN],n_comment[2*GR_MAXPATHLEN];
QString error_text;
if (target_graph.length()<1) return RETURN_FAILURE;//we need at least a single target graph-id
//init everything
minfo.targetSetCols=settype_cols(minfo.targetSetType);
prepareColumnsForProgression(&minfo);//This saves the initial settings for the columns --> needed to reset at the end
for (int i=0;i<minfo.nr_of_sets;i++)
{
gatherDefaultColumnIndices(minfo.set+i);
}
gatherColumnOrder(&minfo);
minfo.advancement_counter=0;
//now postprocessing_order1,postprocessing_order2,postprocessing_order3,postprocessing_order4 contain the first column-indices and where they are used

///We start the advancement here
while (progression_good<2)
{
    for (int i=0;i<minfo.targetSetCols;i++)
    {
        for (int j=0;j<4;j++)
        col_origins[j][i]=-1;
        col_orig_vals[i].clear();
        col_orig_type[i]=-1;
    }
    for (int i=0;i<MAX_DUMMY_COLS;i++)
    {
    temp_col_origins[i].clear();
    temp_col_vals[i].clear();
    }
    gatherUsedColumnIndices(&minfo);//record the columns that have been used already (needed for the advancement)
    /*qDebug() << "used Indices";
    for (int i=0;i<minfo.nr_of_sets;i++)
    {
    qDebug() << "Set" << i << "used:"<< minfo.set[i].usedIndices;
    }*/
target_set_length=-1;
target_cols_ok.clear();
//qDebug() << "target_set_length=" << target_set_length;
for (int i=0;i<minfo.targetSetCols;i++)
{
target_cols_ok << -1;
}
temp_col_ok.clear();
temp_col_len.clear();
temp_col_present.clear();
temp_col_target.clear();
temp_col_def_in.clear();
for (int i=0;i<MAX_DUMMY_COLS;i++)
{
temp_cols[i]=NULL;
temp_col_ok << -1;
temp_col_len << -1;
temp_col_present << false;
temp_col_target << -1;
temp_col_def_in << -1;
}
//look for the temp_cols in the postprocessing-settings (and try to guess the length)
//qDebug() << "TargetColumns:" << minfo.targetSetCols << "minfo.postprocess_target_col.length()=" << minfo.postprocess_target_col.length();
for (int i=0;i<minfo.postprocess_target_col.length();i++)
{
    t_col_0=t_col_1=t_col_2=-1;
    probe=minfo.postprocess_target_col.at(i);//0...n=real target columns, probe<0 --> -1-probe=temp_col
//qDebug() << "Probe=" << probe << "1:S" << minfo.postprocess_operation_source_set1.at(i) << ".C" << minfo.postprocess_operation_source1.at(i) << "2:S" <<  minfo.postprocess_operation_source_set2.at(i) << ".C" << minfo.postprocess_operation_source2.at(i);
    if (probe<0)//a temporary column (A,B,C,...)
    {
    probe=-1-probe;//the real temp-col-id
    t_col_0=probe;
    temp_col_def_in.replace(probe,i);
    temp_col_present.replace(probe,true);//we know it is present and we know that a definition has been set, but we do not know the length or anything, yet
    temp_col_origins[probe] << minfo.postprocess_operation_source_set1.at(i) << minfo.postprocess_operation_source1.at(i) << minfo.postprocess_operation_type.at(i) << minfo.postprocess_operation_source_set2.at(i) << minfo.postprocess_operation_source2.at(i);
    }
    else//a regular column
    {
        if (probe<DATA_BAD)
        col_occurance[probe] << i;
        /*else
        qDebug() << "Column-Number too big:" << probe << "Max=" << DATA_BAD;*/
    }
    //now look through the operands
    tmp_len1=tmp_len2=-1;
    //there is always a source1
/*qDebug() << "Source=" << minfo.postprocess_operation_source1.at(i)
         << "Set=" << minfo.postprocess_operation_source_set1.at(i)
         << "minfo.nr_of_sets=" << minfo.nr_of_sets;*/
    if (minfo.postprocess_operation_source1.at(i)>=0 && minfo.postprocess_operation_source_set1.at(i)>=0 && minfo.postprocess_operation_source_set1.at(i)<minfo.nr_of_sets)//info.postprocess_operation_source_set1.length())
    {//the source1 is from a real set-column in the file
    tmp_len1=minfo.set[minfo.postprocess_operation_source_set1.at(i)].rows;
    }
    else if (minfo.postprocess_operation_source1.at(i)<0 && (minfo.postprocess_operation_source1.at(i)!=-100 && minfo.postprocess_operation_source1.at(i)!=-200))
    {//'index' or 'value' do not tell us the set-length --> this is a temp-set-id
    probe=-1-minfo.postprocess_operation_source1.at(i);//the real temp-col-id
    t_col_1=probe;
    temp_col_present.replace(probe,true);
        if (temp_col_len.at(probe)>=0) tmp_len1=temp_col_len.at(probe);
    }
    if (minfo.postprocess_operation_type.at(i)!=0)//there is not always a source2
    {
        if (minfo.postprocess_operation_source2.at(i)>=0 && minfo.postprocess_operation_source_set2.at(i)>=0 && minfo.postprocess_operation_source_set2.at(i)<minfo.nr_of_sets)
        {
        tmp_len2=minfo.set[minfo.postprocess_operation_source_set2.at(i)].rows;
        }
        else if (minfo.postprocess_operation_source2.at(i)<0 && (minfo.postprocess_operation_source2.at(i)!=-100 && minfo.postprocess_operation_source2.at(i)!=-200))
        {//'index' or 'value' do not tell us the set-length --> this is a temp-set-id
        probe=-1-minfo.postprocess_operation_source2.at(i);//the real temp-col-id
        t_col_2=probe;
        temp_col_present.replace(probe,true);
            if (temp_col_len.at(probe)>=0) tmp_len2=temp_col_len.at(probe);
        }
    }
//qDebug() << "operand" << i << "tmp_len1=" << tmp_len1 << "tmp_len2=" << tmp_len2;
    if (tmp_len1>=0)
    {
        if (target_set_length<0 || tmp_len1>target_set_length) target_set_length=tmp_len1;
        if (t_col_0>=0)
            if(temp_col_len.at(t_col_0)<0 || tmp_len1>temp_col_len.at(t_col_0)) temp_col_len.replace(t_col_0,tmp_len1);
        if (t_col_1>=0)
            if(temp_col_len.at(t_col_1)<0 || tmp_len1>temp_col_len.at(t_col_1)) temp_col_len.replace(t_col_1,tmp_len1);
        if (t_col_2>=0)
            if(temp_col_len.at(t_col_2)<0 || tmp_len1>temp_col_len.at(t_col_2)) temp_col_len.replace(t_col_2,tmp_len1);
    }
    if (tmp_len2>=0)
    {
        if (target_set_length<0 || tmp_len2>target_set_length) target_set_length=tmp_len2;
        if (t_col_0>=0)
            if(temp_col_len.at(t_col_0)<0 || tmp_len2>temp_col_len.at(t_col_0)) temp_col_len.replace(t_col_0,tmp_len2);
        if (t_col_1>=0)
            if(temp_col_len.at(t_col_1)<0 || tmp_len2>temp_col_len.at(t_col_1)) temp_col_len.replace(t_col_1,tmp_len2);
        if (t_col_2>=0)
            if(temp_col_len.at(t_col_2)<0 || tmp_len2>temp_col_len.at(t_col_2)) temp_col_len.replace(t_col_2,tmp_len2);
    }
}
//qDebug() << "First postprocessing analysis complete. Result:";
//qDebug() << "SetLength=" << target_set_length << "tmp_len1=" << tmp_len1 << "tmp_len2=" << tmp_len2;
unfinished_tmp_cols=0;
for (int i=0;i<MAX_DUMMY_COLS;i++)
{
    if (temp_col_present.at(i)==true)
    {
    unfinished_tmp_cols++;
    //qDebug() << QChar(65+i) << "PRESENT, Defined in row" << temp_col_def_in.at(i) << "Length=" << temp_col_len.at(i);
    }
}
//qDebug() << "unfinished_tmp_cols=" << unfinished_tmp_cols;
for (int k=0;k<MAX_DUMMY_COLS;k++)//we do this loop several times in order to set every temporary column even if they are defined in a very unfavorable order
{
if (unfinished_tmp_cols<=0) break;
for (int i=0;i<MAX_DUMMY_COLS;i++)
{
    if (temp_col_ok.at(i)==2) continue;//everything already done here
    if (temp_col_present.at(i)==true)
    {
        if (temp_col_len.at(i)<0) temp_col_len.replace(i,target_set_length);
        if (temp_col_len.at(i)<0) continue;
            temp_cols[i]=new double[2+temp_col_len.at(i)];
        if (temp_col_def_in.at(i)>=minfo.postprocess_operation_source1.length()) continue;//invalid source id
        //init: a column of zeros
        if (temp_col_ok.at(i)<0)
        {
        setDataToZero(temp_cols[i],temp_col_len.at(i)<0);
        temp_col_ok.replace(i,0);
        }
        if (temp_col_def_in.at(i)>=0)//there is a definition
        {
            //init with first operand
            if (minfo.postprocess_operation_source1.at(temp_col_def_in.at(i))==-200)//index
            {
            setDataToIndex(temp_cols[i],temp_col_len.at(i));
            if (minfo.postprocess_operation_type.at(temp_col_def_in.at(i))==0) temp_col_ok.replace(i,2);
            else temp_col_ok.replace(i,1);
            }
            else if(minfo.postprocess_operation_source1.at(temp_col_def_in.at(i))==-100)//value
            {
            setDataToValue(temp_cols[i],temp_col_len.at(i),minfo.postprocess_val1.at(temp_col_def_in.at(i)));
            if (minfo.postprocess_operation_type.at(temp_col_def_in.at(i))==0) temp_col_ok.replace(i,2);
            else temp_col_ok.replace(i,1);
            temp_col_vals[i] << minfo.postprocess_val1.at(temp_col_def_in.at(i));
            }
            else if (minfo.postprocess_operation_source1.at(temp_col_def_in.at(i))<0 && minfo.postprocess_operation_source1.at(temp_col_def_in.at(i))>=-MAX_DUMMY_COLS)//temp-col
            {
            probe=-1-minfo.postprocess_operation_source1.at(temp_col_def_in.at(i));
                if (temp_col_ok.at(probe)==2)//this temp column has already been calculated
                {
                setDataToData(temp_cols[i],temp_col_len.at(i),temp_cols[probe],temp_col_len.at(probe));
                if (minfo.postprocess_operation_type.at(temp_col_def_in.at(i))==0) temp_col_ok.replace(i,2);
                else temp_col_ok.replace(i,1);
                }
            }
            else if (minfo.postprocess_operation_source1.at(temp_col_def_in.at(i))>=0 && minfo.postprocess_operation_source_set1.at(temp_col_def_in.at(i))<minfo.nr_of_sets && minfo.postprocess_operation_source_set1.at(temp_col_def_in.at(i))>=0)//real set and real column
            {
            N_DELETE2(temp_data_col);
            temp_data_col=new double[2+minfo.set[minfo.postprocess_operation_source_set1.at(temp_col_def_in.at(i))].rows];
            //qDebug() << "NOW WE TAKE COLUMN" << minfo.postprocess_operation_source1.at(temp_col_def_in.at(i));
            get_csv_column(minfo,minfo.postprocess_operation_source_set1.at(temp_col_def_in.at(i)),minfo.postprocess_operation_source1.at(temp_col_def_in.at(i)),temp_data_col);
            setDataToData(temp_cols[i],temp_col_len.at(i),temp_data_col,minfo.set[minfo.postprocess_operation_source_set1.at(temp_col_def_in.at(i))].rows);
                if (minfo.postprocess_operation_type.at(temp_col_def_in.at(i))==0) temp_col_ok.replace(i,2);
                else temp_col_ok.replace(i,1);
            }
            else
            {
            //qDebug() << "UNDEFINED SOURCE FOR COL" << QChar(65+i) << "ID=" << minfo.postprocess_operation_source1.at(temp_col_def_in.at(i));
            errmsg((QObject::tr("Undefined source for column ")+QChar(65+i)+QObject::tr(", ID=")+QString::number(minfo.postprocess_operation_source1.at(temp_col_def_in.at(i)))).toLocal8Bit().constData());
            }
            //Work on second operand
            if (minfo.postprocess_operation_type.at(temp_col_def_in.at(i))>0 && temp_col_ok.at(i)==1)//there is a second operand and the first is already complete
            {
            //qDebug() << QChar(65+i) << ": len=" << temp_col_len.at(i) << ", Working on second part, operation=" << minfo.postprocess_operation_type.at(temp_col_def_in.at(i)) << "Source1=" << minfo.postprocess_operation_source1.at(temp_col_def_in.at(i)) << "Source2=" << minfo.postprocess_operation_source2.at(temp_col_def_in.at(i));
                N_DELETE2(temp_data_col);
                temp_data_col=new double[2+temp_col_len.at(i)];
                if (minfo.postprocess_operation_source2.at(temp_col_def_in.at(i))==-200)//index
                {
                setDataToIndex(temp_data_col,temp_col_len.at(i));
                computeData(temp_cols[i],temp_col_len.at(i),temp_data_col,temp_col_len.at(i),minfo.postprocess_operation_type.at(temp_col_def_in.at(i)));
                temp_col_ok.replace(i,2);
                }
                else if(minfo.postprocess_operation_source2.at(temp_col_def_in.at(i))==-100)//value
                {
                setDataToValue(temp_data_col,temp_col_len.at(i),minfo.postprocess_val2.at(temp_col_def_in.at(i)));
                computeData(temp_cols[i],temp_col_len.at(i),temp_data_col,temp_col_len.at(i),minfo.postprocess_operation_type.at(temp_col_def_in.at(i)));
                temp_col_ok.replace(i,2);
                temp_col_vals[i] << minfo.postprocess_val2.at(temp_col_def_in.at(i));
                }
                else if (minfo.postprocess_operation_source2.at(temp_col_def_in.at(i))<0 && minfo.postprocess_operation_source2.at(temp_col_def_in.at(i))>=-MAX_DUMMY_COLS)//temp-col
                {
                probe=-1-minfo.postprocess_operation_source2.at(temp_col_def_in.at(i));
                    if (temp_col_ok.at(probe)==2)//this temp column has already been calculated
                    {
                    setDataToData(temp_data_col,temp_col_len.at(i),temp_cols[probe],temp_col_len.at(probe));
                    computeData(temp_cols[i],temp_col_len.at(i),temp_data_col,temp_col_len.at(i),minfo.postprocess_operation_type.at(temp_col_def_in.at(i)));
                    temp_col_ok.replace(i,2);
                    }
                }
                else if (minfo.postprocess_operation_source2.at(temp_col_def_in.at(i))>=0 && minfo.postprocess_operation_source_set2.at(temp_col_def_in.at(i))<minfo.nr_of_sets && minfo.postprocess_operation_source_set2.at(temp_col_def_in.at(i))>=0)//real set and real column
                {
                N_DELETE2(temp_data_col);
                temp_data_col=new double[2+minfo.set[minfo.postprocess_operation_source_set2.at(temp_col_def_in.at(i))].rows];
                //qDebug() << "NOW WE TAKE COLUMN" << minfo.postprocess_operation_source2.at(temp_col_def_in.at(i));
                get_csv_column(minfo,minfo.postprocess_operation_source_set2.at(temp_col_def_in.at(i)),minfo.postprocess_operation_source2.at(temp_col_def_in.at(i)),temp_data_col);
                computeData(temp_cols[i],temp_col_len.at(i),temp_data_col,minfo.set[minfo.postprocess_operation_source_set2.at(temp_col_def_in.at(i))].rows,minfo.postprocess_operation_type.at(temp_col_def_in.at(i)));
                temp_col_ok.replace(i,2);
                }
                else
                {
                //qDebug() << "UNDEFINED SOURCE FOR COL" << QChar(65+i) << "ID=" << minfo.postprocess_operation_source2.at(temp_col_def_in.at(i));
                errmsg((QObject::tr("Undefined source for column ")+QChar(65+i)+QObject::tr(", ID=")+QString::number(minfo.postprocess_operation_source2.at(temp_col_def_in.at(i)))).toLocal8Bit().constData());
                }
            }
        }
    }
}
unfinished_tmp_cols=0;
for (int i=0;i<MAX_DUMMY_COLS;i++)
{
    if (temp_col_present.at(i)==true && temp_col_ok.at(i)<2)
    {
    unfinished_tmp_cols++;
    }
}
}

//qDebug() << "TargetColumns:" << minfo.targetSetCols;

//After Initialization of temp_cols
/*for (int i=0;i<MAX_DUMMY_COLS;i++)
{
    if (temp_col_present.at(i)==true)
    {
    qDebug() << QChar(65+i) << "PRESENT, Defined in row" << temp_col_def_in.at(i) << "Length=" << temp_col_len.at(i) << "OK=" << temp_col_ok.at(i);
        if (temp_col_ok.at(i)==2)//finished
        {
            for (int j=0;j<temp_col_len.at(i);j++)
            {
            qDebug() << temp_cols[i][j];
            }
        }
    }
}*/

/*
QList<int> perm1,perm3;
perm1.clear();
qDebug() << "definition count of different columns:";
    for (int i=0;i<DATA_BAD;i++)
    {
        if (i>=minfo.targetSetCols) break;
    qDebug() << "Column" << i << "Occurances=" << col_occurance[i].length() << "Lines:" << col_occurance[i];
    perm1 << col_occurance[i].length();
    }
qDebug() << "Occurances for Permutations=" << perm1;
QList< QList<int> > perm2;
generatePermutations(perm1,perm2);
//perm2 ist now a list of all column-combinations to be used
qDebug() << "perm2=" << perm2;
*/
int new_id,target_graph_id;
double * col_target;
//now we have to actually generate a set (maybe more than one)
/*for (int k=0;k<perm2.length();k++)
{
    qDebug() << "Target Graphs=" << target_graph;
    qDebug() << "Target Sets=" << target_set;
perm3.clear();//perm3 will be the list of lines that have the definitions for the columns that are to be used for generating the current new set
    for (int i=0;i<perm2.at(k).length();i++)
    {
        if (perm2.at(k).at(i)>=0 && perm2.at(k).at(i)<col_occurance[i].length())
        perm3 << col_occurance[i].at(perm2.at(k).at(i));
        else
        perm3 << -1;
    }
qDebug() << "k=" << k << "perm3=" << perm3 << "target_set_length=" << target_set.length();
if (target_set.length()<=k || target_graph.length()<=k)
{
target_graph_id=target_graph.at(0);
if (is_valid_gno(target_graph_id)==FALSE) target_graph_id=get_cg();
new_id=nextset(target_graph_id);
qDebug() << "Stage 1: new_id=" << new_id << "target_graph_id=" << target_graph_id;
}
else if (is_valid_setno(target_graph.at(k),target_set.at(k))==FALSE)
{
target_graph_id=target_graph.at(k);
if (is_valid_gno(target_graph_id)==FALSE) target_graph_id=get_cg();
new_id=nextset(target_graph_id);
qDebug() << "Stage 2: new_id=" << new_id << "target_graph_id=" << target_graph_id;
}
else
{
new_id=target_set.at(k);
target_graph_id=target_graph.at(k);
qDebug() << "Stage 3: new_id=" << new_id << "target_graph_id=" << target_graph_id;
}*/

/*target_graph_id=target_graph.at(0);
if (is_valid_gno(target_graph_id)==FALSE) target_graph_id=get_cg();
new_id=nextset(target_graph_id);*/
///look for new set-ids
if (target_graph.length()>new_id_index)//enough set-ids
{
target_graph_id=target_graph.at(new_id_index);
    if (is_valid_gno(target_graph_id)==FALSE)
    {
    target_graph_id=get_cg();
    new_id=nextset(target_graph_id);
    }
    else
    {
    new_id=target_set.at(new_id_index);
        if (is_valid_setno(target_graph_id,new_id)==FALSE)
        {
        new_id=nextset(target_graph_id);
        }
    }
}
else//not enough set-ids
{
    target_graph_id=get_cg();
    new_id=nextset(target_graph_id);
}
new_id_index++;
set_default_plotarr(g[target_graph_id].p+new_id);
set_dataset_type(target_graph_id,new_id,minfo.targetSetType);
setlength(target_graph_id,new_id,target_set_length);
//qDebug() << "Setting up new set: G" << target_graph_id << ".S" << new_id << "length=" << target_set_length;
for (int s=0;s<minfo.targetSetCols;s++)
{
//qDebug() << "s=" << s;
setDataToZero(getcol(target_graph_id,new_id,s),target_set_length);
}
//qDebug() << "Generated empty set: G" << target_graph_id << "S" << new_id << "with length=" << getsetlength(target_graph_id,new_id);
for (int i=0;i<minfo.postprocess_target_col.length();i++)
{
    //qDebug() << "Look At Line " << i << "perm3.contains=" << perm3.contains(i);
//if (minfo.postprocess_target_col.at(i)<0 || perm3.contains(i)==false) continue;//not a target-set-line (probably a temp-column)
if (minfo.postprocess_target_col.at(i)<0) continue;
col_target=getcol(target_graph_id,new_id,minfo.postprocess_target_col.at(i));
if (minfo.postprocess_target_col.at(i)>=0 && minfo.postprocess_target_col.at(i)<minfo.targetSetCols)
{
col_origins[0][minfo.postprocess_target_col.at(i)]=minfo.postprocess_operation_source_set1.at(i);
col_origins[1][minfo.postprocess_target_col.at(i)]=minfo.postprocess_operation_source1.at(i);
col_origins[2][minfo.postprocess_target_col.at(i)]=minfo.postprocess_operation_source_set2.at(i);
col_origins[3][minfo.postprocess_target_col.at(i)]=minfo.postprocess_operation_source2.at(i);
}
if (col_target==NULL) continue;
    //init with first operand
    if (minfo.postprocess_operation_source1.at(i)==-200)//index
    {
    setDataToIndex(col_target,target_set_length);
    }
    else if(minfo.postprocess_operation_source1.at(i)==-100)//value
    {
    setDataToValue(col_target,target_set_length,minfo.postprocess_val1.at(i));
    col_orig_vals[minfo.postprocess_target_col.at(i)] << minfo.postprocess_val1.at(i);
    }
    else if (minfo.postprocess_operation_source1.at(i)<0 && minfo.postprocess_operation_source1.at(i)>=-MAX_DUMMY_COLS)//temp-col
    {
    probe=-1-minfo.postprocess_operation_source1.at(i);
        if (temp_col_ok.at(probe)==2)//this temp column has already been calculated
        {
        setDataToData(col_target,target_set_length,temp_cols[probe],temp_col_len.at(probe));
        }
        else
        {
        //qDebug() << "ERROR: TempCol" << QChar(65+probe) << "NOT calculated, yet!";
        error_text=QObject::tr("Unable to calculate temporary column ")+QChar(65+probe)+QObject::tr(". Column not defined?");
        errmsg(error_text.toLocal8Bit().constData());
        }
    }
    else if (minfo.postprocess_operation_source1.at(i)>=0 && minfo.postprocess_operation_source_set1.at(i)<minfo.nr_of_sets && minfo.postprocess_operation_source_set1.at(i)>=0)//real set and real column
    {
    N_DELETE2(temp_data_col);
    temp_data_col=new double[2+minfo.set[minfo.postprocess_operation_source_set1.at(i)].rows];
    get_csv_column(minfo,minfo.postprocess_operation_source_set1.at(i),minfo.postprocess_operation_source1.at(i),temp_data_col);
    setDataToData(col_target,target_set_length,temp_data_col,minfo.set[minfo.postprocess_operation_source_set1.at(i)].rows);
    //qDebug() << "SOURCE 1: Set=" << minfo.postprocess_operation_source_set1.at(i) << "Col=" << minfo.postprocess_operation_source1.at(i) << "--> G" << target_graph_id << ".S" << new_id << ".C" << minfo.postprocess_target_col.at(i);
    }
    else
    {
    //qDebug() << "UNDEFINED SOURCE FOR TARGET-SET-COL" << i << "ID=" << minfo.postprocess_operation_source1.at(i);
    errmsg((QObject::tr("Undefined source for target-set-column ")+QString::number(i)+QObject::tr(", ID=")+QString::number(minfo.postprocess_operation_source1.at(i))).toLocal8Bit().constData());
    }
    //compute second operand
    if (minfo.postprocess_operation_type.at(i)>0)
    {
        N_DELETE2(temp_data_col);
        col_orig_type[minfo.postprocess_target_col.at(i)]=minfo.postprocess_operation_type.at(i);
        temp_data_col=new double[2+target_set_length];
        if (minfo.postprocess_operation_source2.at(i)==-200)//index
        {
        setDataToIndex(temp_data_col,target_set_length);
        computeData(col_target,target_set_length,temp_data_col,target_set_length,minfo.postprocess_operation_type.at(i));
        }
        else if(minfo.postprocess_operation_source2.at(i)==-100)//value
        {
        setDataToValue(temp_data_col,target_set_length,minfo.postprocess_val2.at(i));
        computeData(col_target,target_set_length,temp_data_col,target_set_length,minfo.postprocess_operation_type.at(i));
        col_orig_vals[minfo.postprocess_target_col.at(i)] << minfo.postprocess_val2.at(i);
        }
        else if (minfo.postprocess_operation_source2.at(i)<0 && minfo.postprocess_operation_source2.at(i)>=-MAX_DUMMY_COLS)//temp-col
        {
        probe=-1-minfo.postprocess_operation_source2.at(i);
            if (temp_col_ok.at(probe)==2)//this temp column has already been calculated
            {
            setDataToData(temp_data_col,target_set_length,temp_cols[probe],temp_col_len.at(probe));
            computeData(col_target,target_set_length,temp_data_col,target_set_length,minfo.postprocess_operation_type.at(i));
            }
            else
            {
            //qDebug() << "ERROR: TempCol" << QChar(65+probe) << "NOT calculated, yet!";
            error_text=QObject::tr("Unable to calculate temporary column ")+QChar(65+probe)+QObject::tr(". Column not defined?");
            errmsg(error_text.toLocal8Bit().constData());
            }
        }
        else if (minfo.postprocess_operation_source2.at(i)>=0 && minfo.postprocess_operation_source_set2.at(i)<minfo.nr_of_sets && minfo.postprocess_operation_source_set2.at(i)>=0)//real set and real column
        {
        N_DELETE2(temp_data_col);
        temp_data_col=new double[2+minfo.set[minfo.postprocess_operation_source_set2.at(i)].rows];
        get_csv_column(minfo,minfo.postprocess_operation_source_set2.at(i),minfo.postprocess_operation_source2.at(i),temp_data_col);
        computeData(col_target,target_set_length,temp_data_col,minfo.set[minfo.postprocess_operation_source_set2.at(i)].rows,minfo.postprocess_operation_type.at(i));
        }
        else
        {
        //qDebug() << "UNDEFINED SOURCE FOR TARGET-SET-COL" << i << "ID=" << minfo.postprocess_operation_source2.at(i);
        errmsg((QObject::tr("Undefined source for target-set-column ")+QString::number(i)+QObject::tr(", ID=")+QString::number(minfo.postprocess_operation_source2.at(i))).toLocal8Bit().constData());
        }
    }
}//end of "go through all lines of definitions"
/*if (target_set.length()>k)
{
target_set.replace(k,new_id);
}
else
{
target_graph << target_graph_id;*/
//qDebug() << "AddNewID to usedTargetSets=" << new_id;
usedTargetSets << new_id;
while (target_graph.length()<usedTargetSets.length())
{
target_graph << target_graph_id;
}
//}
//do string-column
if (minfo.hasStringCol==true)
{
//qDebug() << "minfo.hasStringCol=" << minfo.hasStringCol;
QStringList list1;
get_csv_string_column(minfo,minfo.string_source_set,minfo.string_source_col,list1);
ret_val=init_set_string(target_graph_id,new_id);
len0=(target_set_length<minfo.set[minfo.string_source_set].rows?target_set_length:minfo.set[minfo.string_source_set].rows);
    for (int i=0;i<len0;i++)
    {
    //g[target_graph_id].p[new_id].data.s[i] = copy_string(g[target_graph_id].p[new_id].data.s[i],list1.at(i).toLocal8Bit().data());
    //g[target_graph_id].p[new_id].data.orig_s[i] = copy_string(g[target_graph_id].p[new_id].data.orig_s[i],list1.at(i).toLocal8Bit().data());
    text=list1.at(i);
    g[target_graph_id].p[new_id].data.orig_s[i] = copy_string(g[target_graph_id].p[new_id].data.orig_s[i],text.toUtf8().constData());
        if (activateLaTeXsupport==TRUE)
        complete_LaTeX_to_Grace_Translator(text);
    generate_string_Qt_aware(str_txt,text);
    g[target_graph_id].p[new_id].data.s[i] = copy_string(g[target_graph_id].p[new_id].data.s[i],str_txt);
    }
error_text=QString(", String = Set")+QString::number(minfo.string_source_set)+QString(".Col")+QString::number(minfo.string_source_col)+QString(" ]");
}
else
{
error_text=QString(" ]");
}
QString tmp_col_title,tmp_col_source;
QStringList all_temp_cols,all_temp_col_origs;
QStringList all_target_cols,all_target_col_origs;
QList<int> only_one_operand_temp,only_one_operand_target;
all_temp_cols.clear();
all_temp_col_origs.clear();
all_target_cols.clear();
all_target_col_origs.clear();
only_one_operand_temp.clear();
only_one_operand_target.clear();
//qDebug() << "TempColumns:";
    for (int i=0;i<MAX_DUMMY_COLS;i++)
    {
        if (temp_col_origins[i].length()>0)
        {
        only_one_operand_temp << generateTempColTitle(i,temp_col_origins[i],temp_col_vals[i],tmp_col_title,tmp_col_source);
        all_temp_cols << tmp_col_title;
        all_temp_col_origs << tmp_col_source;
        //qDebug() << tmp_col_title << "=" << tmp_col_source << "(" << only_one_operand_temp.last() << ")";
        }
    }
//qDebug() << "TargetColumns:" << minfo.targetSetCols;
    for (int i=0;i<minfo.targetSetCols;i++)
    {
    only_one_operand_target << generateTargetColTitle(i,col_origins[0][i],col_origins[1][i],col_orig_type[i],col_origins[2][i],col_origins[3][i],col_orig_vals[i],tmp_col_title,tmp_col_source);
    all_target_cols << tmp_col_title;
    all_target_col_origs << tmp_col_source;
    //qDebug() << tmp_col_title << "=" << tmp_col_source << "(" << only_one_operand_target.last() << ")";
    }
//qDebug() << "all_temp_cols=" << all_temp_cols;
//qDebug() << "all_temp_col_origs=" << all_temp_col_origs;

combineTargetColTitle(all_temp_cols,all_temp_col_origs,only_one_operand_temp,all_target_cols,all_target_col_origs,only_one_operand_target);
/*qDebug() << "Combined TargetColumns:";
    for (int i=0;i<all_target_cols.length();i++)
    qDebug() << all_target_cols.at(i) << "=" << all_target_col_origs.at(i);*/
//generateCommentAndLegendAfterImport(minfo.filename,minfo.postprocess_operation_source_set1.at(0),minfo.postprocess_operation_source1.at(0),minfo.postprocess_operation_source1.at(1),n_legend,n_comment);

//qDebug() << "all_target_cols=" << all_target_cols;
//qDebug() << "all_target_col_origs=" << all_target_col_origs;

generateCommentAndLegendAfterImportV2(minfo.filename,all_target_cols,all_target_col_origs,n_legend,n_comment);

/*qDebug() << "n_legend=" << n_legend;
qDebug() << "n_comment=" << n_comment;*/

if (error_text.length()>2)
{
strcpy(n_legend+strlen(n_legend)-2,error_text.toLocal8Bit().constData());
strcpy(n_comment+strlen(n_comment)-2,error_text.toLocal8Bit().constData());
}
set_legend_string(target_graph_id,new_id,n_legend);
setcomment(target_graph_id,new_id,n_comment);
//append_to_storage2(&new_set_no,&new_set_nos,&new_graph_nos,1,&target_graph_id,&new_id);
//}//end loop over all possible new sets
///progression_good=doCSVAdvancement(minfo);
    progression_good=doCSVAdvancement2(minfo);
    if (progression_good==RETURN_FAILURE) progression_good=2;
    else progression_good=0;
}//end progression-loop

//reset source columns
minfo.postprocess_operation_source1=minfo.postprocess_operation_source1_sav;
minfo.postprocess_operation_source2=minfo.postprocess_operation_source2_sav;
minfo.string_source_col=minfo.string_source_col_sav;
//clean up
N_DELETE2(temp_data_col);
for (int i=0;i<MAX_DUMMY_COLS;i++)
{
N_DELETE2(temp_cols[i]);
}
return ret;
}

int constructSetFromCSV(csv_multi_import_infos & minfo, int source_setid, int target_graph, int & target_set,int target_set_type,QList<int> source_columns)
{
int ret=0;
QString text;
char str_txt[GR_MAXPATHLEN];
if (minfo.nr_of_sets<=source_setid || is_valid_gno(target_graph)==FALSE) return 1;
if (target_set==-1)//new set needed
{
target_set=nextset(target_graph);
ret=activateset(target_graph,target_set);
if (ret!=0) return ret;
ret=set_dataset_type(target_graph,target_set,target_set_type);
if (ret!=0) return ret;
}
ret=setlength(target_graph,target_set,minfo.set[source_setid].rows);
if (ret!=0) return ret;
//double * tmp_col=new double[2+minfo.set[source_setid].rows];
int nrofcols=get_number_of_columns(target_graph,target_set);
int read_rows=0,sourcecol;
for (int i=0;i<nrofcols;i++)
{
sourcecol=source_columns.at(i);
//qDebug() << "Creating Column[" << i << "], source=" << sourcecol;
if (sourcecol==-1)//index
{
    for (int j=0;j<minfo.set[source_setid].rows;j++)
    {
    g[target_graph].p[target_set].data.ex[i][j]=(double)j;
    }
}
else if (sourcecol==-2)//just zeros
{
    for (int j=0;j<minfo.set[source_setid].rows;j++)
    {
    g[target_graph].p[target_set].data.ex[i][j]=(double)0.0;
    }
}
else
{
    read_rows=get_csv_column(minfo,source_setid,sourcecol,g[target_graph].p[target_set].data.ex[i]);
    if (read_rows!=minfo.set[source_setid].rows) qDebug() << "ERROR IN READ CSV TO SET: ROWS=" << minfo.set[source_setid].rows << "READ=" << read_rows;
}
//memcpy(g[target_graph].p[target_set].data.ex[i],tmp_col,sizeof(double)*minfo.set[source_setid].rows);
//setcol(target_graph,target_set,i,tmp_col,minfo.set[source_setid].rows);
}
if (source_columns.length()==nrofcols+1)//we want a string-column
{
    //qDebug() << "creating string-column";
ret=init_set_string(target_graph,target_set);
    QStringList slist;
    read_rows=get_csv_string_column(minfo,source_setid,source_columns.at(nrofcols),slist);
    for (int i=0;i<minfo.set[source_setid].rows;i++)
    {
    //g[target_graph].p[target_set].data.s[i] = copy_string(g[target_graph].p[target_set].data.s[i],slist.at(i).toLocal8Bit().constData());
    //g[target_graph].p[target_set].data.orig_s[i] = copy_string(g[target_graph].p[target_set].data.orig_s[i],slist.at(i).toLocal8Bit().constData());
    text=slist.at(i);
    g[target_graph].p[target_set].data.orig_s[i] = copy_string(g[target_graph].p[target_set].data.orig_s[i],text.toUtf8().constData());
        if (activateLaTeXsupport==TRUE)
        complete_LaTeX_to_Grace_Translator(text);
    generate_string_Qt_aware(str_txt,text);
    g[target_graph].p[target_set].data.s[i] = copy_string(g[target_graph].p[target_set].data.s[i],str_txt);
    }
}
//delete[] tmp_col;
return ret;
}

int write_csv_contents_to_target(csv_import_infos & info,int target_type)
{
int ret=RETURN_SUCCESS;
int target_set;
    if (target_type==0)//new set(s)
    {
        //qDebug() << "Read File:" << info.filename;
        //qDebug() << "Zeilen=" << info.rows << "Spalten=" << info.columns;
    target_set=nextset(info.gno);
    activateset(info.gno, target_set);
    set_dataset_type(info.gno, target_set, info.set_type_gerenation);
    setlength(info.gno,target_set,info.rows);
    for (int i=0;i<info.rows;i++)
        for (int j=0;j<info.columns;j++)
            g[info.gno].p[target_set].data.ex[j][i]=info.csv_token_value.at(i).at(j);
    }
    else if (target_type==1)//block_data (block data can only take one set with several columns)
    {

    }
    else//html-text
    {
    info.text_as_html=convert_csv_contents_to_html(info);//,0,-1,-1,true);
    }
return ret;
}

int simple_read_data_from_ascii_file(csv_import_infos & info)
{
int ret=RETURN_SUCCESS;

return ret;
}

void write_data_as_wav(char * filename,int nr_of_sets,int * gnos,int * snos)
{
char wav_header[44];
unsigned int headersize=44;
unsigned int datasize,size[2];
unsigned int filesize;
unsigned short channels=1;
signed short probe;
unsigned int tmp_int;
unsigned short tmp_short;
unsigned int sample_rate;
unsigned int datablocksize;
double tmp_double;
if (is_set_active(gnos[0],snos[0])==FALSE) return;

size[0]=size[1]=datasize=getsetlength(gnos[0],snos[0]);
tmp_double=(datasize-1.0)/(g[gnos[0]].p[snos[0]].data.ex[0][datasize-1]-g[gnos[0]].p[snos[0]].data.ex[0][0]);
sample_rate=(unsigned int)tmp_double;
/*
qDebug() << "sizeof(int)=" << sizeof(int);
qDebug() << "sizeof(signed short)=" << sizeof(signed short);
qDebug() << "sample_rate=" << tmp_double << "#" << sample_rate;
*/
if (nr_of_sets>1)
{
    if (is_set_active(gnos[1],snos[1])==TRUE)
    {
    channels=2;
    size[1]=getsetlength(gnos[1],snos[1]);
        if (datasize<getsetlength(gnos[1],snos[1]))
        {
        datasize=size[1];
        }
    }
}
datablocksize=datasize*channels*sizeof(signed short);

/*
qDebug() << "datapoints=" << datasize;
qDebug() << "datablocksize=" << datablocksize;
*/

filesize=datablocksize+headersize;
strcpy(wav_header,"RIFF    WAVEfmt ");
filesize-=8;
memcpy((void*)(wav_header+4),(void*)&filesize,sizeof(unsigned int));
tmp_int=16;
memcpy((void*)(wav_header+16),(void*)&tmp_int,sizeof(unsigned int));
tmp_short=1;//PCM-format-tag
memcpy((void*)(wav_header+20),(void*)&tmp_short,sizeof(unsigned short));
memcpy((void*)(wav_header+22),(void*)&channels,sizeof(unsigned short));
memcpy((void*)(wav_header+24),(void*)&sample_rate,sizeof(unsigned int));
tmp_short=channels*((7+16)/8);//frame-size
tmp_int=tmp_short*sample_rate;
memcpy((void*)(wav_header+28),(void*)&tmp_int,sizeof(unsigned int));
memcpy((void*)(wav_header+32),(void*)&tmp_short,sizeof(unsigned short));//block-align=frame-size
tmp_short=16;//bits/sample
memcpy((void*)(wav_header+34),(void*)&tmp_short,sizeof(unsigned short));
strcpy(wav_header+36,"data");
memcpy((void*)(wav_header+40),(void*)&datablocksize,sizeof(unsigned int));

ofstream ofi;
ofi.open(filename,ios::binary);
ofi.write(wav_header,sizeof(char)*headersize);
for (unsigned int j=0;j<datasize;j++)
{
    for (unsigned int i=0;i<channels;i++)
    {
        if (j<size[i]) probe=(signed short)g[gnos[i]].p[snos[i]].data.ex[1][j];
        else probe=0;

        ofi.write((char*)((void*)&probe),sizeof(signed short));
    }
}
ofi.close();
}

void split_ini_keys(QStringList & keys,QStringList & sections,QList<QStringList> & sec_vals)
{
sections.clear();
sec_vals.clear();
QStringList temp_strings;
QString probe;
QString l_probe,r_probe;
QStringList all_vals;
QList<int> val_in_section;
int pos,sec_pos;
all_vals.clear();
val_in_section.clear();
    for (int i=0;i<keys.length();i++)
    {
    probe=keys.at(i);
    pos=probe.indexOf("/");
        if (pos>=0)
        {
        l_probe=probe.left(pos);
        r_probe=probe.mid(pos+1);
        sec_pos=sections.indexOf(l_probe);
            if (sec_pos<0)
            {
            sections << l_probe;
            sec_pos=sections.length()-1;
            }
        all_vals << r_probe;
        val_in_section << sec_pos;
        }
    }
    for (int i=0;i<sections.length();i++)
    {
    temp_strings.clear();
        for (int j=0;j<all_vals.length();j++)
        {
            if (val_in_section.at(j)==i)
            {
            temp_strings << all_vals.at(j);
            }
        }
    sec_vals << temp_strings;
    }
}

//we have the sections and the entries (sec_vals) in the sections
//type=0 means: count all entries, type=1 means: count only entries that include a specific text (if text contains '*' then all text-fragments separated by '*' have to be present in an entry to be counted)
//if section.isEmpty() then all entries in all sections are considered
//if there is a section mentioned then only entries in this section are considered
int evaluateSingleCounter(QStringList sections,QList<QStringList> sec_vals,int type, QString section, QString text)
{
int c=0;
QStringList search_entries;
QString error_text;
int section_index;
bool ok;
search_entries=text.split(QString("*"));
if (search_entries.length()<=0) search_entries << text;
if (type==0)//all entries
{
    if (section.isEmpty())//All sections
    {
        for (int j=0;j<sec_vals.length();j++)
        {
        c+=sec_vals.at(j).length();
        }
    }
    else//just one section
    {
    section_index=sections.indexOf(section);
        if (section_index>=0)
        {
        c+=sec_vals.at(section_index).length();
        }
        else
        {
        //qDebug() << "Section" << section << "not found in sections=" << sections;
        error_text=QObject::tr("Section ")+section+QObject::tr(" not found in sections= {");
            for (int k=0;k<sections.length();k++)
            {
            error_text+=sections.at(k);
            if (k<sections.length()-1) error_text+=QString(", ");
            }
        error_text+=QString("}");
        errmsg(error_text.toLocal8Bit().constData());
        }
    }
}
else//specific entries
{
    if (section.isEmpty())//All sections
    {
        for (int j=0;j<sec_vals.length();j++)//go through all entries sections
        {
            for (int k=0;k<sec_vals.at(j).length();k++)//go through all entries in the section
            {
            ok=true;
                for (int l=0;l<search_entries.length();l++)//look for every search criterium
                {
                if (sec_vals.at(j).at(k).contains(search_entries.at(l))==false) ok=false;//if one search-value is not present, it is not ok
                }
            if (ok==true) c++;//only count if everything is ok
            }
        }
    }
    else//just one section
    {
        section_index=sections.indexOf(section);
            if (section_index>=0)
            {
                for (int k=0;k<sec_vals.at(section_index).length();k++)//go through all entries in the section
                {
                ok=true;
                    for (int l=0;l<search_entries.length();l++)//look for every search criterium
                    {
                    if (sec_vals.at(section_index).at(k).contains(search_entries.at(l))==false) ok=false;//if one search-value is not present, it is not ok
                    }
                if (ok==true) c++;//only count if everything is ok
                }
            }
            else
            {
            //qDebug() << "Section" << section << "not found in sections=" << sections;
            error_text=QObject::tr("Section ")+section+QObject::tr(" not found in sections= {");
                for (int k=0;k<sections.length();k++)
                {
                error_text+=sections.at(k);
                if (k<sections.length()-1) error_text+=QString(", ");
                }
            error_text+=QString("}");
            errmsg(error_text.toLocal8Bit().constData());
            }
    }
}
return c;
}

void evaluateCounters(struct importSettings & imp_set)
{
QStringList sections;
QList<QStringList> sec_vals;
QStringList search_entries;
int counter;
/*int section_index;
bool ok;*/
split_ini_keys(imp_set.keys,sections,sec_vals);
imp_set.counterValues.clear();
for (int i=0;i<imp_set.nr_of_counters;i++)
{
counter=evaluateSingleCounter(sections,sec_vals,imp_set.counterTypes.at(i),imp_set.counterSections.at(i),imp_set.counterText.at(i));
imp_set.counterValues << counter;
}
/*qDebug() << "Counters=" << imp_set.nr_of_counters;
qDebug() << "Result of Evaluation=" << imp_set.counterValues;*/
}

double processString(QString text,int type,QString cA,QString cB,int multA,int multB,QString & result)
{
double val=0.0;
bool ok;
int posA,posB,m;
QString tmp_str;
switch (type)
{
case INI_PREPROCESSING_AFTER_FIRST:
case INI_PREPROCESSING_BEFORE_FIRST:
posA=text.indexOf(cA);
m=1;
while (posA>=0 && m<multA)
{
posA=text.indexOf(cA,posA+1);
m++;
}
    if (posA>=0)
    {
        if (type==INI_PREPROCESSING_AFTER_FIRST)
        tmp_str=text.mid(posA+1);
        else if (type==INI_PREPROCESSING_BEFORE_FIRST)
        tmp_str=text.left(posA);
    }
    else
    {
    tmp_str=QString("");
    }
break;
case INI_PREPROCESSING_AFTER_LAST:
case INI_PREPROCESSING_BEFORE_LAST:
posA=text.lastIndexOf(cA);
m=1;
while (posA>=0 && m<multA)
{
posA=text.lastIndexOf(cA,posA-1-text.length());
m++;
}
    if (posA>=0)
    {
        if (type==INI_PREPROCESSING_AFTER_LAST)
        tmp_str=text.mid(posA+1);
        else if (type==INI_PREPROCESSING_BEFORE_LAST)
        tmp_str=text.left(posA);
    }
    else
    {
    tmp_str=QString("");
    }
break;
case INI_PREPROCESSING_BETWEEN_FIRST:
posA=text.indexOf(cA);
m=1;
while (posA>=0 && m<multA)
{
posA=text.indexOf(cA,posA+1);
m++;
}
posB=text.indexOf(cB,posA+1);
m=1;
while (posB>=0 && m<multB)
{
posB=text.indexOf(cB,posB+1);
m++;
}
tmp_str=text.mid(posA+1,posB-posA-1);
break;
case INI_PREPROCESSING_BETWEEN_LAST:
posA=text.lastIndexOf(cA);
m=1;
while (posA>=0 && m<multA)
{
posA=text.lastIndexOf(cA,posA-1-text.length());
m++;
}
posB=text.indexOf(cB,posA+1);
m=1;
while (posB>=0 && m<multB)
{
posB=text.indexOf(cB,posB+1);
m++;
}
tmp_str=text.mid(posA+1,posB-posA-1);
break;
}
result=tmp_str;
val=tmp_str.toDouble(&ok);
return val;
}

void evaluatePreprocessors(struct importSettings & imp_set)
{
int index;
//int posA,posB;
QString probe,tmp_str;
//qDebug() << "evaluatePreprocessors" << imp_set.nr_of_preprocessors;
//qDebug() << "keys=" << imp_set.keys;
imp_set.preprocessingValue.clear();
imp_set.preprocessingString.clear();
    for (int i=0;i<imp_set.nr_of_preprocessors;i++)
    {
    index=imp_set.keys.indexOf(imp_set.preprocessingKey.at(i));
    //qDebug() << "searchKey=" << imp_set.preprocessingKey.at(i) << "index=" << index;
        if (index<0 || index>=imp_set.vals.length())
        {
        imp_set.preprocessingValue << 0;
        imp_set.preprocessingString << QString("0");
        continue;
        }
    probe=imp_set.vals.at(index);
    imp_set.preprocessingValue << processString(probe,imp_set.preprocessingType.at(i),imp_set.preprocessingCharA.at(i),imp_set.preprocessingCharB.at(i),imp_set.preprocessingMultipleA.at(i),imp_set.preprocessingMultipleB.at(i),tmp_str);
    imp_set.preprocessingString << tmp_str;
    //qDebug() << "Preprocessing" << i << "Probe=" << probe << "Result=" << imp_set.preprocessingString.last() << "converted=" << imp_set.preprocessingValue.last();
    }
}

void simpleReadCSVFileLikeFormat(QString filename,struct csv_multi_import_infos & input,QList<int> & target_graphs,QList<int> & target_sets)
{
///readAllInputs();//--> copy data into csv-read-template
QString error_text;
int target_gno=target_graphs.at(0);
//int target_set=-1;
int tmp1,tmp2,c;
//int number,*sel=new int[2];
//qDebug() << "format_suggestion=" << input.csv_column_formats_suggestion;

///lstSet->get_selection(&number,&sel);
int ret,source_setid;
QList<int> sourcecols;
QList<int> used_target_sets;
//delete the memory for new set-ids for undo
N_DELETE2(new_set_nos);
N_DELETE2(new_graph_nos);
new_set_no=0;
//append_to_storage2(&new_set_no,&new_set_nos,&new_graph_nos,1,&new_graph,&new_set);
set_wait_cursor();
//target-columns from postprocessing
///postproset->readSettings(*input);
source_setid=0;

QList<char> lstColumnSep,lstDataSep,lstTextSep;
QList<bool> lstDecSep;
QList< QList<int> > lstSavedCSVColumnFormats;
lstColumnSep.clear();
lstDataSep.clear();
lstTextSep.clear();
lstDecSep.clear();
//lstSavedCSVColumnFormats.clear();
lstSavedCSVColumnFormats=input.csv_column_formats_suggestion;

//first we have to save the settings
///multDataFormSel->getAllFormatSettings(lstSavedCSVColumnFormats);
/*for (int i=0;i<input->nr_of_sets;i++)
{
lstColumnSep << input->set[i].colsep;
lstDataSep << input->set[i].datsep;
lstTextSep << input->set[i].textsep;
lstDecSep << input->set[i].dec_sep_komma;
}*/

int max_cols=settype_cols(input.targetSetType);
for (int i=0;i<input.postprocess_target_col.length();i++)
{
        /*if (input->postprocess_target_col.at(i)<0 || input->postprocess_target_col.at(i)>5)
        qDebug() << i << ": Target=" << input->postprocess_target_col.at(i) << " --> "<< input->postprocess_target_col.at(i) << "/" << max_cols;
        else
        qDebug() << i << ": Target=" << dataset_colname(input->postprocess_target_col.at(i)) << " --> "<< input->postprocess_target_col.at(i) << "/" << max_cols;*/
    if (input.postprocess_target_col.at(i)>=max_cols)
    {
    error_text=QObject::tr("Target set type does not have column ")+QString(dataset_colname(input.postprocess_target_col.at(i)))+QObject::tr(". This target column will be ignored.");
    errmsg(error_text.toLocal8Bit().constData());
    input.postprocess_target_col.removeAt(i);
    input.postprocess_operation_type.removeAt(i);
    input.postprocess_operation_source1.removeAt(i);
    input.postprocess_operation_source2.removeAt(i);
    input.postprocess_operation_source1_sav.removeAt(i);
    input.postprocess_operation_source2_sav.removeAt(i);
    input.postprocess_operation_source_set1.removeAt(i);
    input.postprocess_operation_source_set2.removeAt(i);
    input.postprocess_val1.removeAt(i);
    input.postprocess_val2.removeAt(i);
    input.postprocess_advancement1.removeAt(i);
    input.postprocess_advancement2.removeAt(i);
    i--;
    }
}

QString description;
description.clear();
//we have to fill target_graphs, target_sets with suggestions for new set-ids
///target_graphs.clear();
///target_sets.clear();

/*for (int i=0;i<number;i++)
{
target_graphs << target_gno;
target_sets << sel[i];
}*/

c=-2;//we start with a color and give different colors to all new sets

//go throught the list of files and load data from every file
/// only one file
    for (int j=0;j<used_target_sets.length();j++)//we have to remove the already used indices from the beginning of the list
    {
    if (target_graphs.length()>0) target_graphs.removeFirst();
    if (target_sets.length()>0) target_sets.removeFirst();
    }
    if (target_graphs.length()<=0)//if there are no suggested graphs and sets left --> fill with a dummy
    {
    target_graphs.clear();
    target_sets.clear();
    target_graphs << target_gno;
    target_sets << -1;
    }
used_target_sets.clear();//we have to clear this for every file

N_DELETE2(input.filename);
input.filename=new char[filename.length()*4+8];
strcpy(input.filename,filename.toLocal8Bit().constData());
///warning: lstSavedCSVColumnFormats is empty! as well as input.csv_column_formats_suggestion
//qDebug() << "A format_suggestion=" << input.csv_column_formats_suggestion;
input.csv_column_formats_suggestion=lstSavedCSVColumnFormats;//for every new file we make the same suggestions
//qDebug() << "B format_suggestion=" << input.csv_column_formats_suggestion;
description+=filename;
ret=read_csv_file_into_sets(input,0);//do the actual reading (this reads all data in memory, the new sets will be constructed later)

//qDebug() << "Read from file:" << filelist.at(i);
//we construct more than one set from each file
//IMPORTANT: All files have to have the same file-format
//we have to reset the filename in "input" for every file and order a re-reading of the file with same format-settings as before
//if more than one set is to be read from a file we have to increase the column-indices
    //qDebug() << "target_graphs=" << target_graphs;
    //qDebug() << "target_sets=" << target_sets;
    //qDebug() << "used_target_sets=" << used_target_sets;
//qDebug() << "postprocessing START, used target sets=" << used_target_sets << "new_set_no=" << new_set_no;
//this just reads and generates a single(!) set (the reading from the file has to be completed beforehand); strictly speaking more than a single set is created if a target column is set more than once
//if multiple sets are wanted the different settings have to be made elsewhere
ret=constructSetFromCSVWithPostprocessing(input,target_graphs,target_sets,used_target_sets);
//qDebug() << "postprocessing STOP, used target sets=" << used_target_sets << "new_set_no=" << new_set_no;
//add new set-ids to list of new set-ids for undo-function later
//qDebug() << "usedTargetSets.length=" << used_target_sets.length() << "target_graphs.length=" << target_graphs.length();
    for (int j=0;j<used_target_sets.length();j++)
    {
    tmp1=target_graphs.at(j);
    tmp2=used_target_sets.at(j);
    append_to_storage2(&new_set_no,&new_graph_nos,&new_set_nos,1,&tmp1,&tmp2);
    }

//delete[] sel;

if (new_set_no>0)
{
    if (c==-2)
    {
    c=new_set_nos[0];
    }
    if (c>=0)
    {
        for (int i=0;i<new_set_no;i++)
        {
            if (new_set_nos[i]>0) c=g[new_graph_nos[i]].p[new_set_nos[i]-1].linepen.color+1;
            while (c == getbgcolor() || get_colortype(c) != COLOR_MAIN)
            {
            c++;
            c %= number_of_colors();
            }
        (void)set_set_colors(new_graph_nos[i],new_set_nos[i],c);
            c++;
            c %= number_of_colors();
        }
    }
    /*qDebug() << "undo: new_set_no=" << new_set_no;
    for (int k=0;k<new_set_no;k++)
    {
    qDebug() << "G" << new_graph_nos[k] << ".S" << new_set_nos[k];
    }*/
//undo-stuff
SetsCreated(new_set_no,new_graph_nos,new_set_nos,UNDO_COMPLETE);
description=QObject::tr("Set")+(new_set_no>1?QObject::tr("s"):QString(""))+QObject::tr(" created from CSV-file: ")+description;
addAditionalDescriptionToLastNode(-1,description,QString(),-1);//usefull for adding a formula or changing the description
}
    if (input.autoscale>0)
    {
        if (input.autoscale==1)
        mainWin->doAX();
        else if (input.autoscale==2)
        mainWin->doAY();
        else
        mainWin->doAutoScale();
    }
    else
    {
    mainWin->mainArea->completeRedraw();
    }
unset_wait_cursor();
}

