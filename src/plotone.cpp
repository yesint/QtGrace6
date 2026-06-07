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
 * plotone.c - entry for graphics
 *
 */

///#include <config.h>
#include <cmath>

#include <cstdio>
#include <cstdlib>
///#include <cunistd>
#include <cstring>

#include "globals.h"
#include "utils.h"
#include "files.h"
#include "graphs.h"
#include "draw.h"
#include "device.h"
#include "plotone.h"
#include "noxprotos.h"
#include <QPixmap>
#include <QColor>
#include <QtGui>
#include <QPainter>
#include <QSvgRenderer>
#include <QSvgGenerator>
#include <iostream>
#include "MainWindow.h"
#include "allWidgets.h"
#include "windowWidgets.h"
#include "cmath.h"
#include "parser.h"

using namespace std;

FILE *prstream=NULL;
QTemporaryFile * temp_file=NULL;
Device_entry x11_settings_saved,target_dev_settings_saved;
char cur_print_fname[GR_MAXPATHLEN];
int cur_errorbar=-1;//none; 1=yplus,2=yminus,3=xplus,4=xminus
//extern char desired_hardcopy_filename[GR_MAXPATHLEN];
bool print_error=false;
bool use_temp_file=false,export_from_mainpixmap=false,x11ok=true;
int target_device=DEVICE_X11;

extern MainWindow * mainWin;
extern frmProgressWin * FormProgress;
extern frmQuestionDialog * FormQuestion;
extern QPainter * GeneralPainter;
#if QT_VERSION >= 0x050300
extern QPdfWriter * stdPDFWriter;
#endif
extern QSvgGenerator * stdGenerator;
extern int print_target;
extern bool printing_in_file;
extern int region_def_under_way;
extern int curdevice;
extern bool use_print_command;
extern QImage * MainPixmap;
extern int outputQuality;
extern bool outputGrayscale;
extern bool useQtFonts;
extern QList<QFont> stdFontList;
extern char DecimalPointToUse;
extern char dummy[];
extern unsigned int win_h, win_w;
extern Device_entry dev_null;
extern Device_entry *device_table;
extern int currentStringAlignment;
extern VPoint global_bb_ll;
extern VPoint global_bb_ur;

extern void WriteQtString(VPoint vp,int rot,int just,char * s);
extern void WriteQtString(VPoint vp,int rot,int just,char * s,double charSize,int font,int color,int alpha);
extern QString get_filename_with_extension(int device);
extern void SetDecimalSeparatorToUserValue(char * str,bool remove_space=true);
extern void return_last_string_bounding_box(view * bb);
extern void replaceSuffix(QString & fpath,QString n_suffix);
extern void draw_simple_rectangle(VPoint a, VPoint b,int col);
extern void init_local_bounding_box(VPoint * string_edges,VPoint * ll,VPoint * ur);
extern void expand_local_bounding_box(VPoint * string_edges,VPoint * ll,VPoint * ur);
extern void expand_local_bounding_box(VPoint string_edge,VPoint * ll,VPoint * ur);
extern void expand_local_bounding_box(view string_edge,VPoint * ll,VPoint * ur);

extern int nr_of_collect_set_lines;
extern int * collect_gnos;
extern int * collect_snos;
extern QList< QVector<QPointF> > collect_segments;
extern imageinfo bg_fill_image;

int nr_of_entry_graphs=0;
int * nr_of_legend_entries=NULL;//entries for every graph [gno]
int ** legend_entry_setno=NULL;//the set-numbers of every entry [gno][entry]
double * legend_entry_x_start=NULL;//first x-value of legend [gno]
double * legend_entry_x_stop=NULL;//last x-value of legend [gno]
double ** legend_entry_border=NULL;//the actual y-positions of the entries [gno][entry (one more than number of sets)]

extern int scaletypex;
extern int scaletypey;

bool lines_are_to_be_collected(int gno,int sno)
{
static bool ret;
ret=false;
if (nr_of_collect_set_lines<=0 || !is_valid_setno(gno,sno)) return false;
    for (int i=0;i<nr_of_collect_set_lines;i++)
    {
        //qDebug() << i << "collect_gnos[i]=" << collect_gnos[i] << "collect_snos[i]=" << collect_snos[i];
        if (collect_gnos[i]==gno && collect_snos[i]==sno)
        {
        ret=true;
        break;
        }
    }
return ret;
}

void activate_Qt_Clipping(void)
{
    if (GeneralPainter!=NULL && GeneralPainter->isActive())
    {
    GeneralPainter->setClipping(true);
    }
}

void deactivate_Qt_Clipping(void)
{
    if (GeneralPainter!=NULL && GeneralPainter->isActive())
    {
    GeneralPainter->setClipping(false);
    }
}

void delete_legend_entry_positions(void)
{
    if (nr_of_entry_graphs>0)//we have already set something
    {
        for (int i=0;i<nr_of_entry_graphs;i++)
        {
            delete[] legend_entry_border[i];
            delete[] legend_entry_setno[i];
        }
        delete[] legend_entry_border;
        delete[] legend_entry_setno;
        delete[] legend_entry_x_start;
        delete[] legend_entry_x_stop;
        delete[] nr_of_legend_entries;
        legend_entry_border=NULL;
        legend_entry_x_start=legend_entry_x_stop=NULL;
        nr_of_legend_entries=NULL;
        legend_entry_setno=NULL;
    }
}

void reinit_legend_entry_positions(void)
{
delete_legend_entry_positions();
nr_of_entry_graphs=number_of_graphs();
if (nr_of_entry_graphs<=0) return;
nr_of_legend_entries=new int[nr_of_entry_graphs];
legend_entry_x_start=new double[nr_of_entry_graphs];
legend_entry_x_stop=new double[nr_of_entry_graphs];
legend_entry_border=new double*[nr_of_entry_graphs];
legend_entry_setno=new int*[nr_of_entry_graphs];
    for (int i=0;i<nr_of_entry_graphs;i++)
    {
    legend_entry_border[i]=new double[2+number_of_sets(i)];
    legend_entry_setno[i]=new int[2+number_of_sets(i)];
    }
}

void clear_graph_legend_entry_positions(int gno)
{
    for (int i=0;i<number_of_sets(gno)+1;i++)
    {
    legend_entry_border[gno][i]=-10.0;
    legend_entry_setno[gno][i]=-1;
    }
    legend_entry_x_start[gno]=-10.0;
    legend_entry_x_stop[gno]=-10.0;
    nr_of_legend_entries[gno]=0;
}

void debug_show_entry_pos(void)
{
    VPoint vp1,vp2;
/// qDebug() << "Entry graphs=" << nr_of_entry_graphs;
    for (int i=0;i<nr_of_entry_graphs;i++)
    {
    vp1.x=legend_entry_x_start[i];
    vp2.x=legend_entry_x_stop[i];
    /// qDebug() << "Graph " << i << " entries=" << nr_of_legend_entries[i];
        for (int j=0;j<nr_of_legend_entries[i];j++)
        {
        vp1.y=legend_entry_border[i][j];
        vp2.y=legend_entry_border[i][j+1];
        DrawRect(vp1, vp2);
        }
    }
}

int get_number_of_legend_entries(int gno)
{
    if (gno<0 || gno>=nr_of_entry_graphs)
    return 0;
    else
    return nr_of_legend_entries[gno];
}

int get_setno_of_legend_entry(int gno,int nr)
{
    if (gno<0 || gno>=nr_of_entry_graphs) return -5;
    if (nr<0 || nr>=nr_of_legend_entries[gno]) return -5;
    return legend_entry_setno[gno][nr];
}

int get_legend_entry_bb(int gno, int nr, view * bb)
{
    if (gno<0 || gno>=nr_of_entry_graphs) return RETURN_FAILURE;
    if (nr<0 || nr>=nr_of_legend_entries[gno]) return RETURN_FAILURE;
    bb->xv1=legend_entry_x_start[gno];
    bb->xv2=legend_entry_x_stop[gno];
    bb->yv2=legend_entry_border[gno][nr];
    bb->yv1=legend_entry_border[gno][nr+1];
    return RETURN_SUCCESS;
}

int get_legend_entry_bb_for_set(int gno, int setno, view * bb)
{
    if (gno<0 || gno>=nr_of_entry_graphs) return RETURN_FAILURE;
int nr=-1;
    for (int i=0;i<nr_of_legend_entries[gno];i++)
    {
        if (legend_entry_setno[gno][i]==setno)
        {
        nr=i;
        break;
        }
    }
    if (nr<0 || nr>=nr_of_legend_entries[gno]) return RETURN_FAILURE;
    bb->xv1=legend_entry_x_start[gno];
    bb->xv2=legend_entry_x_stop[gno];
    bb->yv2=legend_entry_border[gno][nr];
    bb->yv1=legend_entry_border[gno][nr+1];
    return RETURN_SUCCESS;
}

int clicked_in_legend(VPoint vp,int * gno,int * setno, view * bb)
{
legend l;
view tmp_bb;
int ret=RETURN_FAILURE;
*gno=-5;
*setno=-5;
for (int i=0;i<number_of_graphs();i++)
{
if (ret==RETURN_SUCCESS) break;
    if (is_graph_hidden(i) == FALSE) {
        get_graph_legend(i, &l);
        if (l.autoattach!=G_LB_ATTACH_NONE)
        {
        l.bb.xv1+=l.xshift;
        l.bb.xv2+=l.xshift;
        l.bb.yv1+=l.yshift;
        l.bb.yv2+=l.yshift;
        }
    if (l.active && is_vpoint_inside(l.bb, vp, MAXPICKDIST)) {
    //now we know, that the user clicked in the current graphs (i) legend --> did the user click on a legend?
        for (int j=0;j<get_number_of_legend_entries(i);j++)//look through all legend entries
        {
        if (ret==RETURN_SUCCESS) break;
            if (get_legend_entry_bb(i,j,&tmp_bb)==RETURN_SUCCESS)
            {
                if (is_vpoint_inside(tmp_bb, vp, MAXPICKDIST))
                {
                *bb=tmp_bb;
                *gno=i;
                *setno=get_setno_of_legend_entry(i,j);
                ret=RETURN_SUCCESS;
                }
            }
        }

    } else {
            continue;
        }
    } else {
        continue;
    }
}//end of graph-for-loop
return ret;
}

//char print_file[GR_MAXPATHLEN] = "";

QImage convertImageToGrayscale(QImage * img)
{
QImage g_img(*img);
QColor col;
uint gr;
QPainter pa(&g_img);
pa.setPen(Qt::NoPen);
for (int i=0;i<g_img.width();i++)
{
    for (int j=0;j<g_img.height();j++)
    {
    col.setRgb(img->pixel(i,j));
    gr=(col.red() * 11 + col.green() * 16 + col.blue() * 5)/32;
    col.setRgb(gr,gr,gr);
    //pa.setPen(col);
    //pa.drawPoint(i,j);
    pa.setBrush(col);
    pa.drawRect(i,j,1,1);
    }
}
pa.end();
return g_img;
}

QString InternalTargetName(int targ)
{
QString ret;
switch (targ)
{
case PRINT_TARGET_SCREEN:
ret=QString("SCREEN");
break;
case PRINT_TARGET_SVG_FILE:
ret=QString("SVG_FILE");
break;
case PRINT_TARGET_PRINTER:
ret=QString("PRINTER");
break;
}
return ret;
}

QString InternalDeviceName(int devnr)
{
QString ret;
switch (devnr)
{
default:
case DEVICE_NULL:
ret=QString("DEVICE_NULL");
break;
case DEVICE_SCREEN:
ret=QString("DEVICE_SCREEN");
break;
case DEVICE_PDF:
ret=QString("DEVICE_PDF");
break;
case DEVICE_SVG:
ret=QString("DEVICE_SVG");
break;
case DEVICE_JPEG:
ret=QString("DEVICE_JPEG");
break;
case DEVICE_PNG:
ret=QString("DEVICE_PNG");
break;
case DEVICE_HD_PNG:
ret=QString("DEVICE_HD_PNG");
break;
case DEVICE_BMP:
ret=QString("DEVICE_BMP");
break;
case DEVICE_TIFF:
ret=QString("DEVICE_TIFF");
break;
case DEVICE_PS:
ret=QString("DEVICE_PS");
break;
case DEVICE_EPS:
ret=QString("DEVICE_EPS");
break;
case DEVICE_MIF:
ret=QString("DEVICE_MIF");
break;
case DEVICE_PNM:
ret=QString("DEVICE_PNM");
break;
case DEVICE_METAFILE:
ret=QString("DEVICE_METAFILE");
break;
case DEVICE_PDF_HARU:
ret=QString("DEVICE_PDF_HARU");
break;
}
return ret;
}

//this function is central to the plotting- / printing- process (here the decision is made what settings are to be used for the x11-driver)
//if 'false' is returned, the x11-drv is not to be used --> no special settings needed
//if 'true' is returned, the x11drv is to be used
bool determine_what_to_paint_on(int p_device,bool p_to_file)
{
char * s=get_print_cmd();
target_device=p_device;//what driver actually to use
print_target=PRINT_TARGET_SCREEN;//how to initialize the x11drv
use_temp_file=export_from_mainpixmap=false;
if (p_to_file==false && use_print_command==false)//use a real physical printer
{
print_target=PRINT_TARGET_PRINTER;
return true;
}

if (!get_ptofile() && use_print_command==true && (s == NULL || s[0] == '\0'))
{//direct printing to printer selected (no file output) and print-command is to be used, but no usable print-command specified
    errmsg(QObject::tr("No print command defined, output aborted").toLocal8Bit().constData());
    print_error=true;
    return false;
}
//do not print to a file, but use the printer-command (like 'lpr') to send a file directely to the printer --> we use a temporary file here
if (!get_ptofile() && use_print_command==true)
{
use_temp_file=true;
printing_in_file=true;
}
//cout << "p_device=" << p_device << endl;
    switch (p_device)
    {
    case DEVICE_PS:
    case DEVICE_EPS:
    case DEVICE_MIF:
    case DEVICE_PNM:
    case DEVICE_METAFILE:
    case DEVICE_PDF_HARU:
        return false;//nothing to set here-just use the std-driver
    break;
    case DEVICE_SCREEN:
        return true;//just use the x11-drv normally
    break;
    //case DEVICE_HD_PNG:
    case DEVICE_PDF:
#if QT_VERSION >= 0x050300
        print_target=PRINT_TARGET_PDF_FILE;
        target_device=DEVICE_X11;
        return true;
        /// Stop here - just to test it
#else
        //we generate a temporary svg-file here and convert it with the svg-renderer afterwards
        print_target=PRINT_TARGET_SVG_FILE;
        use_temp_file=true;//the temporary file is a svg-file
            /*if (useQtFonts==true)
            {*/
            target_device=DEVICE_X11;
            return true;
            /*}// we can not use the default Grace-SVG-driver because it does not generate nodern svg-files!
            else//no Qt-fonts-->use the standard-svg-driver
            {
            target_device=DEVICE_SVG;//remember: here we have to do something afterwards!
            return false;
            }*/
#endif
    break;
    case DEVICE_SVG:
        if (useQtFonts==true)
        {
        print_target=PRINT_TARGET_SVG_FILE;//generate an svg-file with the Qt-x11drv
        target_device=DEVICE_X11;
        return true;
        }
        else//no QtFonts-->use the std-SVG-driver
        {
        return false;
        }
    break;
    case DEVICE_HD_PNG:/// changed to here -> use std-output-driver
    case DEVICE_JPEG://for these file-types we use Qt's export functions
    case DEVICE_TIFF:
    case DEVICE_PNG:
    case DEVICE_BMP:
        export_from_mainpixmap=true;
        target_device=DEVICE_X11;
        return true;
    break;
    }
return false;
}

//the following two functions are especially for hardcopy export or printing
bool prepare_x11drv(int c_dev,bool p_to_file)//p_to_file should be the result of get_ptofile(); --> i.e. the ultimate target
{
/*
cout << "BEFORE PREPARE:" << endl;
cout << "device width= " << device_table[DEVICE_X11].pg.width << " height=" << device_table[DEVICE_X11].pg.height << endl;
*/
prstream = NULL;
print_error=false;
printing_in_file=p_to_file;//printing in file is useful for intermediate works (e.g. printing using a print-command means no output-file, but intermediate printing in a file and sending this file to the printer via the command)

if (use_print_command==true) printing_in_file=true;//we send a temporary file directely to the printer via a terminal-command (if we actualy want a file export this is ok either)

x11ok=determine_what_to_paint_on(c_dev,p_to_file);

//save old settings (warning, the functions are not changed, only the dimensions and so on)
x11_settings_saved=dev_null;//just to initialize every with NULL
target_dev_settings_saved=dev_null;
copy_device_props(&target_dev_settings_saved,device_table+target_device,1);//save target-device_settings
copy_device_props(&x11_settings_saved,device_table+DEVICE_X11,1);//save screen settings - just for savety

if (print_error==true) return false;//some error occured

Device_entry dev = get_device_props(c_dev);//we have to use the currently set hard-copy-device because we want to make sure to have the right output settings
QString temptemplate;

if (target_device!=c_dev)
copy_device_props(device_table+target_device,device_table+c_dev,1);//copy the settings from the hard-copy-device to the x11-driver
if (target_device==DEVICE_X11 && useQtFonts==false && get_device_props(target_device).devfonts==true)
{//devfonts in x11/screen only possible if QtFonts are to be used
device_table[DEVICE_X11].devfonts=false;
}
if (c_dev==DEVICE_HD_PNG)
{
device_table[DEVICE_X11].pg.width*=300/device_table[DEVICE_X11].pg.dpi;
device_table[DEVICE_X11].pg.height*=300/device_table[DEVICE_X11].pg.dpi;
}
if (use_temp_file==true)//generate a temporary file
{
    if (temp_file!=NULL) delete temp_file;
    temp_file=new QTemporaryFile();

    temptemplate=QDir::tempPath()+QDir::separator()+QString("XXXXXX.");
    if (print_target==PRINT_TARGET_SVG_FILE)
    temptemplate += QString("svg");
    else//otherwise we use the generic file type (like .ps)
    temptemplate += QString(dev.fext);

    temp_file->setFileTemplate(temptemplate);
    temp_file->open();
    strcpy(cur_print_fname, temp_file->fileName().toUtf8().data());
    temp_file->close();
//cout << "Template=#" << temptemplate.toLocal8Bit().constData() << "#" << endl;
//cout << "Temporary file=#" << cur_print_fname << "#" << endl;
    temp_file->remove();//temporary file is now gone, but we can now use the filename
    if (print_target!=PRINT_TARGET_SVG_FILE)//we do not need the prstream for the Qt-svg-export
    {
    prstream = grace_openw(cur_print_fname);
//cout << "File Opened" << endl;
        if (prstream == NULL)
        {
            /// errwin(QObject::tr("Unable to open print-device!").toLocal8Bit().constData());
            print_error=true;
            return false;
        }
    }
}//ende use_temp_file
else if (print_target==PRINT_TARGET_SVG_FILE && use_temp_file==false)
{
    QFileInfo filinfo=QFileInfo(print_file);
    bool overwrite=true;
    char buf[2048];
        if (filinfo.exists()==true)//because we did not use prstream here we have to ask ourselves
        {
        sprintf(buf, "%s %s?",QObject::tr("Overwrite").toLocal8Bit().constData(), print_file);
        overwrite=(bool)yesno(buf, NULL, NULL, NULL);
        }
    if (overwrite==false) return false;//some error occured (overwrite not possible)
strcpy(cur_print_fname,print_file);
}
else
{
strcpy(cur_print_fname,print_file);
}

if (print_target==PRINT_TARGET_SVG_FILE)
{
    if (prstream != NULL)
    {
    grace_close(prstream);//we do not use the Grace-File-Output-Routines here
    prstream=NULL;
    }
    if (stdGenerator==NULL)
    {
    stdGenerator=new QSvgGenerator();
    }
    stdGenerator->setFileName(cur_print_fname);
    stdGenerator->setSize(QSize(dev.pg.width,dev.pg.height));
    stdGenerator->setViewBox(QRect(0, 0, dev.pg.width, dev.pg.height));
    std::string Str=std::string(get_docname());
    stdGenerator->setTitle(QString("QtGrace: ")+QString::fromStdString(Str));
    //stdGenerator->setTitle(QString("QtGrace: ")+QString(get_docname()));
    stdGenerator->setDescription(QString(get_project_description()));
}
else if (target_device==DEVICE_PDF_HARU)
{
//cout << "USING HARU" << endl;
    if (prstream != NULL)
    {
    //cout << "prstream not NULL in USING HARU" << endl;
    grace_close(prstream);//we do not use the Grace-File-Output-Routines here
    prstream=NULL;
    }
    if (print_file[0] == '\0')
    {
        QString fwe=get_filename_with_extension(hdevice);
        strcpy(print_file,fwe.toLocal8Bit().constData());
    }
    strcpy(cur_print_fname, print_file);
}
else if (x11ok==false && print_target!=PRINT_TARGET_PRINTER && use_temp_file==false)//if we use a printer, we print directly to it; if we do not use a printer and no temporary file, we use the output-file directely
{
    if (print_file[0] == '\0')
    {
        QString fwe=get_filename_with_extension(hdevice);
        strcpy(print_file,fwe.toLocal8Bit().constData());
    }
    strcpy(cur_print_fname, print_file);
    //cout << "opening file for writing=#" << cur_print_fname << "#" << endl;
    prstream = grace_openw(cur_print_fname);
    //cout << "File Opened" << endl;
    if (prstream == NULL)
    {
        /// errwin(QObject::tr("Unable to open print-device!").toLocal8Bit().constData());
        print_error=true;
        return false;
    }
}
select_device(target_device);

/*
QString prepare_result("AfterPrepare\n");
prepare_result+=QString("cur_print_fname= ")+QString(cur_print_fname)+QString("\n");
prepare_result+=QString("Print_File= ")+QString(print_file)+QString("\n");
prepare_result+=QString("Print_Error= ")+QString((print_error==true?"FEHLER":"Alles_OK"))+QString("\n");
prepare_result+=QString("Use_Temp-File= ")+QString((use_temp_file==true?"TempFile!":"Kein_Temp_File"))+QString("\n");
prepare_result+=QString("Export_from_MainPixmap= ")+QString((export_from_mainpixmap==true?"Export":"Anders"))+QString("\n");
prepare_result+=QString("x11_ok= ")+QString((x11ok==true?"X11":"kein_X11"))+QString("\n");
prepare_result+=QString("Target_Device= ")+InternalDeviceName(target_device)+QString("\n");
prepare_result+=QString("Print_Target= ")+InternalTargetName(print_target)+QString("\n");
prepare_result+=QString("hdevice= ")+InternalDeviceName(hdevice);
QMessageBox::information(0,QString("After Prepare x11drv"),prepare_result);
*/

/*cout << "After Prepare:" << endl;
cout << "File=" << cur_print_fname << endl;
cout << "Print File=" << print_file << endl;
cout << "Print Error=" << print_error << endl;
cout << "Use Temp-File=" << use_temp_file << endl;
cout << "Export from MainPixmap=" << export_from_mainpixmap << endl;
cout << "x11_ok=" << x11ok << endl;
cout << "Target Device=" << target_device << endl;
cout << "Print Target=" << print_target << endl;*/

return true;
}

void postprocess_x11drv(int c_dev,bool p_to_file)//only needed if we abused the x11-screen-driver for file-output-printing
{
Device_entry dev = get_device_props(c_dev);
QPixmap pm;
QSvgRenderer renderer;
QImage image;
QPainter painter;
QPrinter printer(QPrinter::HighResolution);
QFileInfo filinfo;
QString export_file;
view v;
double vx, vy;
int truncated_out;
double oputputdpi=300.0;
QSize paperSi;
QSizeF paperSi2;
bool ret,overwrite;
print_error=false;
char buf[GR_MAXPATHLEN + 50];
char * printcommand=get_print_cmd();

//QMessageBox::information(0,QString("After_draw"),QString("postprocess_x11_dpi=")+QString::number(dev.pg.dpi));

if (dev.pg.dpi>0.0)
{
paperSi.setWidth(dev.pg.width*oputputdpi/dev.pg.dpi);
paperSi.setHeight(dev.pg.height*oputputdpi/dev.pg.dpi);

}
else
{
    dev.pg.dpi=72.0;
    set_device_props(c_dev,dev);
paperSi.setWidth(dev.pg.width*oputputdpi/72.0);
paperSi.setHeight(dev.pg.height*oputputdpi/72.0);
}

paperSi2.setWidth(paperSi.width()*oputputdpi);
paperSi2.setHeight(paperSi.height()*oputputdpi);

//postprocessing
if (prstream!=NULL)//close output stream
{
grace_close(prstream);
prstream=NULL;
}

v = get_bbox(BBOX_TYPE_GLOB);
get_page_viewport(&vx, &vy);
if (v.xv1 < 0.0 || v.xv2 > vx || v.yv1 < 0.0 || v.yv2 > vy)
{
truncated_out = TRUE;
    if (print_target!=PRINT_TARGET_PRINTER && !(use_print_command==true && !p_to_file))
    errmsg(QObject::tr("Output is truncated - tune device dimensions").toLocal8Bit().constData());
}
else
{
truncated_out = FALSE;
}
//QMessageBox::information(0,QString("before ending"),QString("before_real_export"));
    if (print_target==PRINT_TARGET_PRINTER)//use a real printer
    {
        if (GeneralPainter->isActive()==true)
        GeneralPainter->end();
    }
#if QT_VERSION >= 0x050300
    else if (print_target==PRINT_TARGET_PDF_FILE)
    {
        if (GeneralPainter->isActive()==true)
        GeneralPainter->end();
        delete stdPDFWriter;
        stdPDFWriter=NULL;
    }
#endif
    else if (print_target==PRINT_TARGET_SVG_FILE && use_temp_file==false)
    {
        GeneralPainter->end();
        if (stdGenerator!=NULL)
        {
        delete stdGenerator;
        stdGenerator=NULL;
        }
    }
    else if (export_from_mainpixmap==true)
    {
       export_file=QString::fromLocal8Bit(print_file);
       filinfo=QFileInfo(export_file);
//qDebug() << "export_file=" << export_file;
       overwrite=true;
       if (filinfo.exists()==true)//because we did not use prstream here we have to ask ourselves
       {
       sprintf(buf, "%s %s?",QObject::tr("Overwrite").toLocal8Bit().constData(), print_file);
       overwrite=(bool)yesno(buf, NULL, NULL, NULL);
       }

       if (filinfo.exists()==false || (filinfo.exists()==true && overwrite==true))
       {
       if (GeneralPainter->isActive()==true)
       GeneralPainter->end();
       //cout << "monoMode=" << monomode << " OutputGrayScale=" << outputGrayscale << " Quality=" << outputQuality << endl;
            if (monomode == FALSE && outputGrayscale==false)
            {
            pm=QPixmap::fromImage(*MainPixmap,Qt::AutoColor | Qt::DiffuseAlphaDither);
            //cout << "Color" << endl;
            }
            else
            {
            //QImage con_img=MainPixmap->convertToFormat(QImage::Format_Mono,Qt::MonoOnly);
            QImage con_img=convertImageToGrayscale(MainPixmap);
            //pm=QPixmap::fromImage(*MainPixmap,Qt::MonoOnly);
            //pm=QPixmap::fromImage(con_img,Qt::MonoOnly);
            pm=QPixmap::fromImage(con_img);
            //cout << "MonoOnly" << endl;
            }
            ret=false;

            if (clip_borders_for_output==TRUE)
            {
            QPoint a,b;
            a=VPoint2XPoint(global_bb_ll);
            b=VPoint2XPoint(global_bb_ur);
            pm=pm.copy(a.x(),b.y(),abs(b.x()-a.x())+1,abs(b.y()-a.y()));
            }

                switch (c_dev)
                {
                case DEVICE_PNG:
                case DEVICE_HD_PNG:
                    ret=pm.save(export_file,"PNG",outputQuality);
                break;
                case DEVICE_BMP:
                    ret=pm.save(export_file,"BMP",outputQuality);
                break;
                case DEVICE_JPEG:
                    ret=pm.save(export_file,"JPG",outputQuality);
                break;
                case DEVICE_TIFF:
                    ret=pm.save(export_file,"TIFF",outputQuality);
                break;
                default:
                    stufftext(QObject::tr("Error: export from main pixmap, unsuitable export device").toLocal8Bit().constData());
                    //cout << "unsuitable export device=" << c_dev << endl;
                break;
                }
       }
       else
       {
       ret=true;
       }
            if (ret==false)
            {
            errwin((QObject::tr("Unable to export to image file. (")+QString(print_file)+QString(")")).toLocal8Bit().constData());
            print_error=true;
            }
    }
    else if (use_temp_file==true)
    {
        if (use_print_command==true && !p_to_file)//we want to print on a printer using a unix-terminal-command
        {
            sprintf(dummy,"%s %s",printcommand,cur_print_fname);
            if (truncated_out == FALSE || yesno(QObject::tr("Printout is truncated. Continue?").toLocal8Bit().data(), NULL, NULL, NULL))
            {
            system_wrap(dummy);
            }
        }
        else//we convert the file format now
        {
            switch (c_dev)
            {
            case DEVICE_HD_PNG://currently not in use, because HD-PNG is done via std-png-output
                    GeneralPainter->end();//needed to end/complete the svg-export!
                    delete stdGenerator;
                    stdGenerator=NULL;
                //Read svg file and convert to png
                renderer.load(QString::fromLocal8Bit(cur_print_fname));
                // Prepare a QImage with desired characteritisc
                image=QImage(renderer.defaultSize(), QImage::Format_RGB32);
                // Get QPainter that paints to the image
                painter.begin(&image);
                renderer.render(&painter);
                painter.end();
                // Save, image format based on file extension
                image.save(print_file);/// ,"PNG",outputQuality);
            break;
            case DEVICE_PDF:
                /*cout << "device.dpi=" << device_table[c_dev].pg.dpi << " x11.dpi=" << device_table[DEVICE_X11].pg.dpi << endl;
                cout << "device width= " << device_table[c_dev].pg.width << " height=" << device_table[c_dev].pg.height << endl;
                cout << "device width= " << device_table[DEVICE_X11].pg.width << " height=" << device_table[DEVICE_X11].pg.height << endl;
            cout << "Reading from #" << cur_print_fname << "#" << endl;
            cout << "Printing to #" << print_file << "#" << endl;*/
                GeneralPainter->end();//needed to end/complete the svg-export!
                delete stdGenerator;
                stdGenerator=NULL;
            filinfo=QFileInfo(QString::fromLocal8Bit(cur_print_fname));
            //befehl=QString("cp ")+QString(cur_print_fname)+QString(" /Users/andreaswinter/test_svg1.svg");
            //cout << "copy-befehl=#" << befehl.toLocal8Bit().constData() << "#" << endl;
            //system(befehl.toLocal8Bit().constData());
                //Read svg file and convert to pdf
                ret=renderer.load(QString::fromLocal8Bit(cur_print_fname));
                printer.setOutputFormat(QPrinter::PdfFormat);
                printer.setOutputFileName(print_file);
            /*cout << "ret=" << ret << " exists=" << filinfo.exists() << endl;
            cout << "defaultSize=" << renderer.defaultSize().width() << " x " << renderer.defaultSize().height() << endl;
            cout << "paperSi=" << paperSi.width() << " x " << paperSi.height() << endl;*/
                printer.setResolution(int(oputputdpi));
#if QT_VERSION >= 0x060000
                printer.setPageMargins(QMarginsF(0.0,0.0,0.0,0.0),QPageLayout::Point);
                printer.setPageSize(QPageSize(paperSi2,QPageSize::Point));
#else
                printer.setPageMargins(0.0,0.0,0.0,0.0,QPrinter::DevicePixel);
                printer.setPaperSize(paperSi,QPrinter::DevicePixel);
#endif
                painter.begin(&printer);
                renderer.render(&painter);
                painter.end();
            //cout << "finished exporting from svg to pdf" << endl;
            break;
            default:
                stufftext(QObject::tr("Error: conversion of temporary file not successful.").toLocal8Bit().constData());
                //cout << "unsuitable export device=" << c_dev << endl;
            break;
            }
        }
        temp_file->remove();
        delete temp_file;
        temp_file=NULL;
    }
//QMessageBox::information(0,QString("Restore old settings"),QString("Restore old settings\ntarget_device=")+InternalDeviceName(target_device));
set_exportname(print_file);

//restore old settings
copy_device_props(device_table+target_device,&target_dev_settings_saved,1);
copy_device_props(device_table+DEVICE_X11,&x11_settings_saved,1);

//QMessageBox::information(0,QString("Begin Screen Repaint"),QString("Begin Screen Repaint\ndrawgraph()"));
//repaint for the screen
set_ptofile(false);
printing_in_file=false;
print_target=PRINT_TARGET_SCREEN;
select_device(DEVICE_X11);//this sets the output to the screen again (as usual)
/// drawgraph();
/*cout << "AFTER POSTPROCESS:" << endl;
cout << "device width= " << device_table[DEVICE_X11].pg.width << " height=" << device_table[DEVICE_X11].pg.height << endl;
cout << "x11_saved: width=" << x11_settings_saved.pg.width << " height=" << x11_settings_saved.pg.height << endl;*/
}

/*
 * draw all active graphs
 */
void drawgraph(void)
{
static int save_w,save_h;
if (stop_repaint==TRUE) return;//no repaint
//Page_geometry pg1=get_page_geometry();
//qDebug() << "redraw: w=" << pg1.width << " h=" << pg1.height << endl;
//qDebug() << "drawgraph!";
if (simple_draw_setting==SIMPLE_DRAW_NONE)
{
//qDebug() << "We really redraw everything!";//to test occurences of multiple-redraws
        if (print_target==PRINT_TARGET_SCREEN)//if we are about to draw on the screen we have to change the device-table-entry
        {
        save_w=device_table[DEVICE_SCREEN].pg.width;
        save_h=device_table[DEVICE_SCREEN].pg.height;
        device_table[DEVICE_SCREEN].pg.width*=GeneralPageZoomFactor;
        device_table[DEVICE_SCREEN].pg.height*=GeneralPageZoomFactor;
        }
//qDebug() << "DecimalPointToUse=" << DecimalPointToUse;
    int i;
    VPoint vp1, vp2;
    view g_v;
    Pen pen;
    int saveg;
    RotationAngle=0;
    saveg = get_cg();

    currentStringAlignment=Qt::AlignLeft;//the default text alignment

    if (initgraphics() == RETURN_FAILURE)
    {
        errmsg(QObject::tr("Device wasn't properly initialized (probably too many or too few pixels)").toLocal8Bit().constData());
        if (print_target==PRINT_TARGET_SCREEN)
        {
        device_table[DEVICE_SCREEN].pg.width=save_w;
        device_table[DEVICE_SCREEN].pg.height=save_h;
        }
        return;
    }
    setclipping(FALSE);
    if (getbgfill() != FALSE)//do the background-filling
    {
        pen.color = getbgcolor();
        pen.alpha = getbgalpha();
        pen.pattern = 1;
        setpen(pen);
        vp1.x = 0.0;
        vp1.y = 0.0;
        get_page_viewport(&vp2.x, &vp2.y);
        FillRect_(vp1, vp2);//always solid fill first, in case the image fills not the whole space
        if (print_target==PRINT_TARGET_SCREEN && getbgfill()==2)//fill with image
        {
        QColor def_col;
        RGB * rgbColor=get_rgb(getbgcolor());
        def_col.setRgb(rgbColor->red,rgbColor->green,rgbColor->blue,getbgalpha());
        drawImageOnPainter(GeneralPainter,bg_fill_image,MainPixmap->rect(),def_col);
        }
    }
    reset_bboxes();
    global_bb_ll.x=global_bb_ll.y=-1.0;
    global_bb_ur.x=global_bb_ur.y=-1.0;
    activate_bbox(BBOX_TYPE_GLOB, TRUE);
    activate_bbox(BBOX_TYPE_TEMP, FALSE);
    reinit_legend_entry_positions();//for saving the positions of the text-entries in the legends
    //qDebug() << "start_bounding_box=" << global_bb_ll.x << "|" << global_bb_ll.y << "-" << global_bb_ur.x << "|" << global_bb_ur.y;
        for (i = 0; i < number_of_graphs(); i++)
        {
        active_graph=i;
            if (is_graph_hidden(i)==TRUE) continue;
        clear_graph_legend_entry_positions(i);
        plotone(i);
            if (get_graph_viewport(i,&g_v)==RETURN_SUCCESS)
            {
            //qDebug() << "expand by graph-viewport:" << g_v.xv1 << "|"<< g_v.yv1 << "-" << g_v.xv2 << "|" << g_v.yv2;
            expand_local_bounding_box(g_v,&global_bb_ll,&global_bb_ur);
            }
        //qDebug() << "bounding_box before legend=" << global_bb_ll.x << "|" << global_bb_ll.y << "-" << global_bb_ur.x << "|" << global_bb_ur.y;
            if (nr_of_legend_entries!=NULL && nr_of_entry_graphs>=i)
            {
            if (g[i].l.active && nr_of_legend_entries[i]>0)
            expand_local_bounding_box(g[i].l.bb,&global_bb_ll,&global_bb_ur);
            }
        }
    //qDebug() << "bounding_box before objects=" << global_bb_ll.x << "|" << global_bb_ll.y << "-" << global_bb_ur.x << "|" << global_bb_ur.y;
    /* draw objects NOT clipped to a particular graph */
    draw_objects(-1);
    //qDebug() << "bounding_box before timestamp=" << global_bb_ll.x << "|" << global_bb_ll.y << "-" << global_bb_ur.x << "|" << global_bb_ur.y;
    draw_timestamp();
    //qDebug() << "bounding_box after timestamp=" << global_bb_ll.x << "|" << global_bb_ll.y << "-" << global_bb_ur.x << "|" << global_bb_ur.y;
#ifdef DEBUG_SHOW_TEXT_BOUNDING_BOXES
    draw_simple_rectangle(global_bb_ll,global_bb_ur,7);
#endif
        if (get_cg() != saveg)
        {
        select_graph(saveg);
        }
/// debug the legend entry positions
/// debug_show_entry_pos();
    leavegraphics();
        if (print_target==PRINT_TARGET_SCREEN)//if we printed on the screen, we may have altered the dimensions - we have to set them back
        {
        device_table[DEVICE_SCREEN].pg.width=save_w;
        device_table[DEVICE_SCREEN].pg.height=save_h;
        }
    if (mainWin)
    mainWin->mainArea->contentChanged=true;
}//End simple-draw
    if (startupphase==0 && printing_in_file==false)
    {
        if (mainWin)
        mainWin->mainArea->repaint();//do the actual repainting on the screen
    qApp->processEvents();
    }
    //if (enableServerMode)
    //{
    /// mainWin->SocketConnection->sendParam();/// is private within this context
    //}
}

void debug_device_settings(Device_entry dev)
{
cout << "---- DEVICE FOR PRINTING ----" << endl;
cout << "Device Name=#" << dev.name << "#" << endl;
cout << "Dev-Fonts=" << dev.devfonts << endl;
cout << "Font-AntiAliasing=" << dev.fontaa << endl;
cout << "DPI=" << dev.pg.dpi << endl;
cout << "Size= " << dev.pg.width << " x " << dev.pg.height << endl;
cout << "---- END DEVICE SETTINGS ----" << endl;
}

int what_is_the_optimal_pdf_driver(void)//returns 0 if the Qt-pdf-driver should be used and return 1 if libharu should be better
{
int ret=0;
    if (useQtFonts==false)
    {
    ret=1;
    }
    else//Use QtFonts-->this excludes the haru-pdf-library
    {
    ret=0;
    }
return ret;
}

/*
 * If writing to a file, check to see if it exists
 */
void do_hardcopy(void)
{
char *s=get_print_cmd();
double sav_zoom_factor=GeneralPageZoomFactor;
GeneralPageZoomFactor=1.0;
    //char tbuf[256];
    //char fname[GR_MAXPATHLEN];
    //view v;
    //double vx, vy;
    //int truncated_out;
int dirty=is_dirtystate();
    /*QTemporaryFile file;
    QString fileN;
    QByteArray byteArray;
    char *cTempFileName;*/
    int sav_simpl_draw=simple_draw_setting;
    Device_entry dev,displ_dev,orig_displ_settings;
    dev = get_device_props(hdevice);//hdevice is the output-device for the hardcopy - usually a file

if (auto_set_export_extensions==TRUE)
{
/*QFileInfo fi3(print_file);
QString suf=fi3.suffix();
suf=suf.toLower();
QString suf2=QString(dev.fext).toLower();
    if (suf!=suf2)
    {
        if (print_file[strlen(print_file)-1]=='.')
        {
        print_file[strlen(print_file)-1]='\0';
        }
        strcat(print_file,".");
        strcat(print_file,dev.fext);
    }*/
QString suf=QString::fromLocal8Bit(print_file);
replaceSuffix(suf,QString(dev.fext));
strcpy(print_file,suf.toLocal8Bit().constData());
}
        if (dev.pg.dpi<=0.0)
        {
        dev.pg.dpi=72.0;
        set_device_props(hdevice,dev);
        }
    displ_dev=get_device_props(DEVICE_SCREEN);//get screen settings -- this is for modifying
        if (displ_dev.pg.dpi<=0.0)
        {
        displ_dev.pg.dpi=72.0;
        set_device_props(DEVICE_SCREEN,displ_dev);
        }
    orig_displ_settings=get_device_props(DEVICE_SCREEN);//get screen settings again to have them for backup -- this is for setting everything back at the end
    prstream=NULL;
    simple_draw_setting=SIMPLE_DRAW_NONE;
/// copy_Grace_to_LaTeX();//for safety -- should not be needed
    if (!get_ptofile() && use_print_command==true && (s == NULL || s[0] == '\0'))
    {//direct printing to printer selected and print-command is to be used, but no usable print-command specified
        errmsg(QObject::tr("No print command defined, output aborted").toLocal8Bit().constData());
        simple_draw_setting=sav_simpl_draw;
        GeneralPageZoomFactor=sav_zoom_factor;
        return;
    }
//bool use_x11drv=prepare_x11drv(hdevice,get_ptofile());
    (void)prepare_x11drv(hdevice,get_ptofile());
    if (FormProgress!=NULL)
    FormProgress->increase();
if (print_error==true)
{
    //restore old settings
    copy_device_props(device_table+target_device,&target_dev_settings_saved,1);
    copy_device_props(device_table+DEVICE_X11,&x11_settings_saved,1);
    //repaint for the screen
    set_ptofile(false);
    printing_in_file=false;
    print_target=PRINT_TARGET_SCREEN;
    GeneralPageZoomFactor=sav_zoom_factor;
    select_device(DEVICE_X11);//this sets the output to the screen again (as usual)
    drawgraph();
    simple_draw_setting=sav_simpl_draw;
return;
}
    if ((target_device==DEVICE_PDF_HARU || hdevice==DEVICE_PDF) && gracebat==FALSE)//we do not use grace_openw --> look for existing file with same name (|| hdevice==DEVICE_HD_PNG --> changed)
    {
        char buf[GR_MAXPATHLEN + 50];
        QFileInfo fi(print_file);
        if (fi.exists()==true)
        {
            sprintf(buf, "%s %s?",QObject::tr("Overwrite").toLocal8Bit().constData(), print_file);
            if (!yesno(buf, NULL, NULL, NULL))//do not overwrite
            {
                set_ptofile(false);
                printing_in_file=false;
                print_target=PRINT_TARGET_SCREEN;
                select_device(DEVICE_X11);//this sets the output to the screen again (as usual)
                GeneralPageZoomFactor=sav_zoom_factor;
                drawgraph();
                simple_draw_setting=sav_simpl_draw;
            return;
            }
        }
    }

/*QString st1=(printing_in_file==true?QString("Printing_in_File=TRUE"):QString("Printing_in_File=FALSE"));
QString st2=(get_ptofile()==TRUE?QString("Print_to_File=TRUE"):QString("Print_to_File=FALSE"));
QMessageBox::information(0,QString("vor drawgraph"),st1+QString("\n")+st2);*/

/// do the actual drawing
drawgraph();
    if (FormProgress!=NULL)
    FormProgress->increase();
postprocess_x11drv(hdevice,get_ptofile());
        if (FormProgress!=NULL)
        FormProgress->increase();
    if (dirty==FALSE)
    clear_dirtystate();
    else
    set_dirtystate();
simple_draw_setting=sav_simpl_draw;
GeneralPageZoomFactor=sav_zoom_factor;
drawgraph();
return;
}

void plotone(int gno)
{
//QTime tim1;
//tim1.start();
//qDebug() << "plotone" << flush;
    xy_xconv = xy_xconv_general;
    xy_yconv = xy_yconv_general;

    GraphType gtype;
    if (is_graph_active(gno) != TRUE || is_graph_hidden(gno) == TRUE)
    {
        return;
    }
    setclipping(TRUE);
    set_draw_mode(TRUE);
    if (select_graph(gno) != RETURN_SUCCESS)
    {
        return;
    }

    switch (scaletypex)
    {
    case SCALE_NORMAL:
        xy_xconv = xy_xconv_simple;
    break;
    case SCALE_LOG:
        xy_xconv = xy_xconv_log;
    break;
    case SCALE_REC:
        xy_xconv = xy_xconv_rec;
    break;
    case SCALE_LOGIT:
        xy_xconv = xy_xconv_logit;
    break;
    default:
        xy_xconv = xy_xconv_general;
    break;
    }

    switch (scaletypey)
    {
    case SCALE_NORMAL:
        xy_yconv = xy_yconv_simple;
    break;
    case SCALE_LOG:
        xy_yconv = xy_yconv_log;
    break;
    case SCALE_REC:
        xy_yconv = xy_yconv_rec;
    break;
    case SCALE_LOGIT:
        xy_yconv = xy_yconv_logit;
    break;
    default:
        xy_yconv = xy_yconv_general;
    break;
    }

    /* fill frame */
    fillframe(gno);
    gtype = (GraphType)get_graph_type(gno);
    if (gtype != GRAPH_PIE)
    {
        /* calculate tick mark positions for all axes */
        calculate_tickgrid(gno);
        /* draw grid lines */
        drawgrid(gno);
    }
    /* plot type specific routines */
    switch(gtype)
    {
    case GRAPH_POLAR2:
        draw_polar_graph(gno);
        break;
    case GRAPH_POLAR:
        draw_polar_graph(gno);
        break;
    case GRAPH_SMITH:
        draw_smith_chart(gno);
        break;
    case GRAPH_PIE:
        draw_pie_chart(gno);
        break;
    default:
        xyplot(gno);
        break;
    }
    if (gtype != GRAPH_PIE)
    {
        /* plot axes and tickmarks */
        drawaxes(gno);
    }
    /* plot frame */
    drawframe(gno);
    /* plot objects */
    draw_objects(gno);
    if (gtype != GRAPH_PIE)
    {
        /* plot legends */
        dolegend(gno);
    }
    /* draw title and subtitle */
    draw_titles(gno);
    /* draw regions and mark the reference points only if in interactive mode */
    if (terminal_device() == TRUE)
    {
        draw_regions(gno);
        draw_ref_point(gno);
    }

    xy_xconv = xy_xconv_general;
    xy_yconv = xy_yconv_general;

//qDebug() << "plot-time=" << tim1.restart() << "ms";
}

void draw_smith_chart(int gno)
{
    (void)gno;
    /* This is empty! This does not work - yet! */
}

void draw_pie_chart(int gno)
{
    int i, setno, nsets = 0;
    plotarr p;
    view v;
    world w;
    int sgn;
    VPoint vpc, vp1, vp2, vps[3], vpa;
    VVector offset;
    double r, start_angle, stop_angle;
    double e_max, norm;
    double *x, *c, *e, *pt;
    AValue avalue;
    char str[MAX_STRING_LENGTH];

    get_graph_viewport(gno, &v);
    vpc.x = (v.xv1 + v.xv2)/2;
    vpc.y = (v.yv1 + v.yv2)/2;

    get_graph_world(gno, &w);
    sgn = is_graph_xinvert(gno) ? -1:1;
    
    for (setno = 0; setno < number_of_sets(gno); setno++)
    {
        active_set=setno;
        if (is_set_drawable(gno, setno))
        {
            nsets++;
            if (nsets > 1)
            {
                errmsg(QObject::tr("Only one set per pie chart can be drawn").toLocal8Bit().constData());
                return;
            }
            
            switch (dataset_type(gno, setno))
            {
            case SET_XY:
            case SET_XYCOLOR:
            case SET_XYCOLPAT:
                get_graph_plotarr(gno, setno, &p);
                /* data */
                x = getcol(gno, setno, DATA_X);
                /* explode factor */
                e = getcol(gno, setno, DATA_Y);
                /* colors */
                c = getcol(gno, setno, DATA_Y1);
                /* patterns */
                pt = getcol(gno, setno, DATA_Y2);
                
                /* get max explode factor */
                e_max = 0.0;
                for (i = 0; i < p.data.len; i++)
                {
                    e_max = MAX2(e_max, e[i]);
                }
                
                r = 0.8/(1.0 + e_max)*MIN2(v.xv2 - v.xv1, v.yv2 - v.yv1)/2;

                norm = 0.0;
                for (i = 0; i < p.data.len; i++)
                {
                    if (x[i] < 0.0)
                    {
                        errmsg(QObject::tr("No negative values in pie charts allowed").toLocal8Bit().constData());
                        return;
                    }
                    if (e[i] < 0.0)
                    {
                        errmsg(QObject::tr("No negative offsets in pie charts allowed").toLocal8Bit().constData());
                        return;
                    }
                    norm += x[i];
                }
                
                stop_angle = w.xg1;
                for (i = 0; i < p.data.len; i++)
                {
                    Pen pen;
                    
                    start_angle = stop_angle;
                    stop_angle = start_angle + sgn*2*M_PI*x[i]/norm;
                    offset.x = e[i]*r*cos((start_angle + stop_angle)/2.0);
                    offset.y = e[i]*r*sin((start_angle + stop_angle)/2.0);
                    vps[0].x = vpc.x + r*cos(start_angle) + offset.x;
                    vps[0].y = vpc.y + r*sin(start_angle) + offset.y;
                    vps[1].x = vpc.x + offset.x;
                    vps[1].y = vpc.y + offset.y;
                    vps[2].x = vpc.x + r*cos(stop_angle) + offset.x;
                    vps[2].y = vpc.y + r*sin(stop_angle) + offset.y;
                    vp1.x = vpc.x - r + offset.x;
                    vp1.y = vpc.y - r + offset.y;
                    vp2.x = vpc.x + r + offset.x;
                    vp2.y = vpc.y + r + offset.y;
                    
                    if (c != NULL)
                    {
                        pen.color   = (int) rint(c[i]);
                    }
                    else
                    {
                        pen.color = p.symfillpen.color;
                    }
                        pen.alpha = p.symfillpen.alpha;
                    if (pt != NULL)
                    {
                        pen.pattern   = (int) rint(pt[i]);
                    }
                    else
                    {
                        pen.pattern = p.symfillpen.pattern;
                    }
                    setpen(pen);
                    DrawFilledArc(vp1, vp2,
                                  (int) rint(180.0/M_PI*start_angle),
                                  (int) rint(180.0/M_PI*stop_angle),
                                  ARCFILL_PIESLICE);
                    
                    setpen(p.sympen);
                    setlinewidth(p.symlinew);
                    setlinestyle(p.symlines);
                    DrawPolyline(vps, 3, POLYLINE_OPEN);
                    DrawArc(vp1, vp2,
                            (int) rint(180.0/M_PI*start_angle),
                            (int) rint(180.0/M_PI*stop_angle));

                    avalue = p.avalue;

                    if (avalue.active == TRUE)
                    {

                        vpa.x = vpc.x + ((1 + e[i])*r + avalue.offset.y)*
                                cos((start_angle + stop_angle)/2.0);
                        vpa.y = vpc.y + ((1 + e[i])*r + avalue.offset.y)*
                                sin((start_angle + stop_angle)/2.0);

                        strcpy(str, avalue.prestr);
                        dummy[0]='\0';
                        switch(avalue.type)
                        {
                        case AVALUE_TYPE_X:
                            strcpy(dummy, create_fstring(avalue.format, avalue.prec, x[i], LFORMAT_TYPE_EXTENDED));
                            break;
                        case AVALUE_TYPE_STRING:
                            if (p.data.s != NULL && p.data.s[i] != NULL)
                            {
                                strcpy(dummy, p.data.s[i]);
                            }
                            break;
                        default:
                            continue;
                        }
                        SetDecimalSeparatorToUserValue(dummy,false);
                        strcat(str, dummy);
                        strcat(str, avalue.appstr);
                        setcharsize(avalue.size);
                        setfont(avalue.font);
                        setcolor(avalue.color);
                        setalpha(avalue.alpha);
                        WriteString(vpa, avalue.angle, JUST_CENTER|JUST_MIDDLE, str);
                    }
                }
                break;
            default:
                errmsg(QObject::tr("Unsupported in pie chart set type").toLocal8Bit().constData());
                break;
            }
        }
    }
}

void draw_polar_graph(int gno)
{
int i;
plotarr p;
QPainterPath pp;
VPoint vp,vpc;
world wtmp;
double d1,d2,dphi;
int inner_x1,inner_y1,inner_w,inner_h,center_x,center_y;
int x1,x2,y1,y2;
world wtmp2;
int save_clipping=doclipping();

if (is_valid_gno(gno)==TRUE && get_graph_type(gno)==GRAPH_POLAR2)
{//generate clipping area for polar plots
get_graph_world(get_cg(),&wtmp);
dphi=g[get_cg()].phi0;

//determine center of donut (reference for all calculations)
world2view(0.0,0.5*(wtmp.yg2+wtmp.yg1),&vpc.x,&vpc.y);
world2view(M_PI,0.5*(wtmp.yg2+wtmp.yg1),&vp.x,&vp.y);
vpc.x+=vp.x;
vpc.x*=0.5;
vpc.y+=vp.y;
vpc.y*=0.5;

xlibVPoint2dev(vpc,&center_x,&center_y);

wtmp2.xg1=wtmp2.xg2=-dphi;

wtmp2.yg1=wtmp.yg1;//use full radial range here
wtmp2.yg2=wtmp.yg2;

    world2view(wtmp2.xg1,wtmp2.yg1,&vp.x,&vp.y);
    d1=fabs(vp.x-vpc.x);
    world2view(wtmp2.xg2,wtmp2.yg2,&vp.x,&vp.y);
    d2=fabs(vp.x-vpc.x);
    vp.x=vpc.x-d1;
    vp.y=vpc.y+d1;
    xlibVPoint2dev(vp,&x1,&y1);
    vp.x=vpc.x+d1;
    vp.y=vpc.y-d1;
    xlibVPoint2dev(vp,&x2,&y2);//inner circle

    inner_x1=x1;
    inner_y1=y1;
    inner_w=x2-x1+1;
    inner_h=y2-y1+1;

    vp.x=vpc.x-d2;
    vp.y=vpc.y+d2;
    xlibVPoint2dev(vp,&x1,&y1);
    vp.x=vpc.x+d2;
    vp.y=vpc.y-d2;
    xlibVPoint2dev(vp,&x2,&y2);//outer circle

/*QColor col1(10,70,255,90);
if (GeneralPainter!=NULL && GeneralPainter->isActive())
GeneralPainter->setBrush(col1);*/

        if (fabs(wtmp.xg2-wtmp.xg1)>=2.0*M_PI)//full circle
        {
            /*if (GeneralPainter!=NULL && GeneralPainter->isActive())
            {
            GeneralPainter->drawEllipse(x1,y1,x2-x1+1,y2-y1+1);
            }*/
        pp.addEllipse(x1,y1,x2-x1+1,y2-y1+1);
        }
        else//part circle (pie-slice)
        {
        d1=((wtmp.xg1<wtmp.xg2?wtmp.xg1:wtmp.xg2)+dphi)/M_PI*180.0;
            /*if (GeneralPainter!=NULL && GeneralPainter->isActive())
            {
            GeneralPainter->drawPie(x1,y1,x2-x1+1,y2-y1+1,int(d1*16),int(fabs(wtmp.xg2-wtmp.xg1)/M_PI*180.0*16));
            }*/
        pp.moveTo(center_x,center_y);
        pp.arcTo(x1,y1,x2-x1+1,y2-y1+1,d1,fabs(wtmp.xg2-wtmp.xg1)/M_PI*180.0);
        pp.closeSubpath();
        }
pp.addEllipse(inner_x1,inner_y1,inner_w,inner_h);//central area is empty (clipping area)
    if (GeneralPainter!=NULL && GeneralPainter->isActive())
    {
        /*col1=QColor(70,10,155,90);
        GeneralPainter->setBrush(col1);
        GeneralPainter->drawEllipse(inner_x1,inner_y1,inner_w,inner_h);*/
    GeneralPainter->setClipPath(pp);
    }
}
    for (i = 0; i < number_of_sets(gno); i++)
    {
        active_set=i;
        if (is_set_drawable(gno, i)) {
            get_graph_plotarr(gno, i, &p);
            switch (dataset_type(gno, i)) {
            case SET_XY:
            case SET_XYSIZE:
            case SET_XYCOLOR:
            case SET_XYZ:
                    if (get_graph_type(gno)==GRAPH_POLAR2)
                    {
                    activate_Qt_Clipping();
                    setclipping(FALSE);
                    }
                drawsetline(gno, i, &p, 0, NULL, NULL, 0.0);
                    if (get_graph_type(gno)==GRAPH_POLAR2)
                    {
                    deactivate_Qt_Clipping();
                    setclipping(save_clipping);
                    }
                drawsetsyms(gno, i, &p, 0, NULL, NULL, 0.0);
                drawsetavalues(gno, i, &p, 0, NULL, NULL, 0.0);
                break;
            default:
                errmsg(QObject::tr("Unsupported in polar graph set type").toLocal8Bit().constData());
                break;
            }
        }
    }
if (get_graph_type(gno)==GRAPH_POLAR2)
deactivate_Qt_Clipping();

}

void xyplot(int gno)
{
    int i, j;
    plotarr p;
    int refn;
    double *refx, *refy;
    double offset, epsilon;
    QPen linePen;
    QColor tmpColor;
    int colorNumber;
    RGB * rgbColor;
    bool collect_cur_set_lines=false;

    refn = 0;
    offset = 0.0;
    refx = NULL;
    refy = NULL;

    /* draw sets */
    switch (get_graph_type(gno))
    {
    case GRAPH_XY:
        for (i = 0; i < number_of_sets(gno); i++)
        {
            active_set=i;
            colorNumber=getpen().color;
            rgbColor=get_rgb(colorNumber);
            tmpColor=QColor(rgbColor->red,rgbColor->blue,rgbColor->green,getpen().alpha);
            linePen=QPen(tmpColor);
            if (is_set_drawable(gno, i))
            {
                collect_cur_set_lines=lines_are_to_be_collected(gno,i);
                //qDebug() << "collect G" << gno << ".S" << i << "-->" << collect_cur_set_lines;
                collect_active=(int)collect_cur_set_lines;
                get_graph_plotarr(gno, i, &p);
                switch (dataset_type(gno, i))
                {
                case SET_XY:
                case SET_XYSIZE:
                case SET_XYCOLOR:
                case SET_XYZ:
                    drawsetline(gno, i, &p, 0, NULL, NULL, 0.0);
                    collect_active=0;
                    drawsetsyms(gno, i, &p, 0, NULL, NULL, 0.0);
                    drawsetavalues(gno, i, &p, 0, NULL, NULL, 0.0);
                    break;
                case SET_BAR:
                    drawsetline(gno, i, &p, 0, NULL, NULL, 0.0);
                    drawsetbars(gno, i, &p, 0, NULL, NULL, 0.0);
                    collect_active=0;
                    drawsetavalues(gno, i, &p, 0, NULL, NULL, 0.0);
                    break;
                case SET_BARDY:
                case SET_BARDYDY:
                    //drawsetline(gno, i, &p, refn, refx, refy, offset);
                    drawsetbars(gno, i, &p, refn, refx, refy, offset);
                    if (!highlight_errorbars) collect_active=0;
                    drawseterrbars(gno, i, &p, refn, refx, refy, offset);
                    collect_active=0;
                    drawsetavalues(gno, i, &p, refn, refx, refy, offset);
                    collect_active=(int)collect_cur_set_lines;
                    drawsetline(gno, i, &p, refn, refx, refy, offset);
                    collect_active=0;
                    break;
                case SET_XYDX:
                case SET_XYDY:
                case SET_XYDXDX:
                case SET_XYDYDY:
                case SET_XYDXDY:
                case SET_XYDXDXDYDY:
                    //drawsetline(gno, i, &p, 0, NULL, NULL, 0.0);
                    if (!highlight_errorbars) collect_active=0;
                    drawseterrbars(gno, i, &p, 0, NULL, NULL, 0.0);
                    collect_active=0;
                    drawsetavalues(gno, i, &p, 0, NULL, NULL, 0.0);
                    collect_active=(int)collect_cur_set_lines;
                    drawsetline(gno, i, &p, 0, NULL, NULL, 0.0);
                    collect_active=0;
                    drawsetsyms(gno, i, &p, 0, NULL, NULL, 0.0);
                    break;
                case SET_XYHILO:
                    drawsethilo(&p);
                    collect_active=0;
                    drawsetavalues(gno, i, &p, 0, NULL, NULL, 0.0);
                    break;
                case SET_XYVMAP:
                    drawsetline(gno, i, &p, 0, NULL, NULL, 0.0);
                    collect_active=0;
                    drawsetvmap(gno, &p);
                    drawsetsyms(gno, i, &p, 0, NULL, NULL, 0.0);
                    drawsetavalues(gno, i, &p, 0, NULL, NULL, 0.0);
                    break;
                case SET_BOXPLOT:
                    drawsetline(gno, i, &p, 0, NULL, NULL, 0.0);
                    if (!highlight_errorbars) collect_active=0;
                    drawsetboxplot(&p);
                    collect_active=0;
                    drawsetavalues(gno, i, &p, 0, NULL, NULL, 0.0);
                    break;
                default:
                    errmsg(QObject::tr("Unsupported in XY graph set type").toLocal8Bit().constData());
                    break;
                }
                collect_active=0;
            }
        }
        break;
    case GRAPH_CHART:
        for (i = 0; i < number_of_sets(gno); i++)
        {
            active_set=i;
            get_graph_plotarr(gno, i, &p);
            if (is_set_drawable(gno, i))
            {
                if (p.data.len > refn)
                {
                    refn = p.data.len;
                    refx = p.data.ex[0];
                }
                if (is_graph_stacked(gno) != TRUE)
                {
                    offset -= 0.5*0.02*p.symsize;
                }
            }
        }
        offset -= 0.5*(nactive(gno) - 1)*get_graph_bargap(gno);
        
        if (is_graph_stacked(gno) == TRUE)
        {
            refy = (double*)xcalloc(refn, sizeof(double));///SIZEOF_DOUBLE);
            if (refy == NULL)
            {
                return;
            }
        }
        
        if (refx)
        {
            double xmin, xmax;
            int imin, imax;
            minmax(refx, refn, &xmin, &xmax, &imin, &imax);
            epsilon = 1.0e-3*(xmax - xmin)/refn;
        }
        else
        {
            epsilon = 0.0;
        }

        for (i = 0; i < number_of_sets(gno); i++)
        {
            int x_ok;
            double *x;
            
            get_graph_plotarr(gno, i, &p);
            if (is_set_drawable(gno, i))
            {
                collect_cur_set_lines=lines_are_to_be_collected(gno,i);
                collect_active=(int)collect_cur_set_lines;
                /* check that abscissas are identical with refx */
                x = getcol(gno, i, DATA_X);
                x_ok = TRUE;
                for (j = 0; j < getsetlength(gno, i); j++)
                {
                    if (fabs(x[j] - refx[j]) > epsilon)
                    {
                        x_ok = FALSE;
                        break;
                    }
                }
                if (x_ok != TRUE)
                {
                    char buf[128];
                    QString buf_str;
                    sprintf(buf, "G%d.S%d", gno, i);
                    buf_str=QObject::tr("Set ")+QString(buf)+QObject::tr(" has different abscissas, skipped from the chart.");
                    errmsg(buf_str.toLocal8Bit().constData());
                    continue;
                }
                
                if (is_graph_stacked(gno) != TRUE)
                {
                    offset += 0.5*0.02*p.symsize;
                }
                switch (dataset_type(gno, i))
                {
                case SET_XY:
                case SET_XYSIZE:
                case SET_XYCOLOR:
                    drawsetline(gno, i, &p, refn, refx, refy, offset);
                    collect_active=0;
                    if (is_graph_stacked(gno) != TRUE)
                    {
                        drawsetsyms(gno, i, &p, refn, refx, refy, offset);
                        drawsetavalues(gno, i, &p, refn, refx, refy, offset);
                    }
                    break;
                case SET_BAR:
                    drawsetline(gno, i, &p, refn, refx, refy, offset);
                    drawsetbars(gno, i, &p, refn, refx, refy, offset);
                    collect_active=0;
                    if (is_graph_stacked(gno) != TRUE)
                    {
                        drawsetavalues(gno, i, &p, refn, refx, refy, offset);
                    }
                    break;
                case SET_BARDY:
                case SET_BARDYDY:
                    //drawsetline(gno, i, &p, refn, refx, refy, offset);
                    drawsetbars(gno, i, &p, refn, refx, refy, offset);
                    if (is_graph_stacked(gno) != TRUE)
                    {
                        if (!highlight_errorbars) collect_active=0;
                        drawseterrbars(gno, i, &p, refn, refx, refy, offset);
                        collect_active=0;
                        drawsetavalues(gno, i, &p, refn, refx, refy, offset);
                    }
                    collect_active=(int)collect_cur_set_lines;
                    drawsetline(gno, i, &p, refn, refx, refy, offset);
                    collect_active=0;
                    break;
                case SET_XYDY:
                case SET_XYDYDY:
                    //drawsetline(gno, i, &p, refn, refx, refy, offset);
                    if (is_graph_stacked(gno) != TRUE)
                    {
                        if (!highlight_errorbars) collect_active=0;
                        drawseterrbars(gno, i, &p, refn, refx, refy, offset);
                        collect_active=0;
                        //drawsetsyms(gno, i, &p, refn, refx, refy, offset);
                        drawsetavalues(gno, i, &p, refn, refx, refy, offset);
                    }
                    collect_active=(int)collect_cur_set_lines;
                    drawsetline(gno, i, &p, refn, refx, refy, offset);
                    collect_active=0;
                    if (is_graph_stacked(gno) != TRUE)
                    drawsetsyms(gno, i, &p, refn, refx, refy, offset);
                    break;
                default:
                    errmsg(QObject::tr("Unsupported in XY chart set type").toLocal8Bit().constData());
                    break;
                }
                collect_active=0;
                if (is_graph_stacked(gno) != TRUE)
                {
                    offset += 0.5*0.02*p.symsize + get_graph_bargap(gno);
                }
                else
                {
                    for (j = 0; j < p.data.len; j++)
                    {
                        refy[j] += p.data.ex[1][j];
                    }
                }
            }
        }
        
        if (is_graph_stacked(gno) == TRUE)
        {
            /* Second pass for stacked charts: symbols and avalues */
            offset = 0.0;
            for (j = 0; j < refn; j++)
            {
                refy[j] = 0.0;
            }
            
            for (i = 0; i < number_of_sets(gno); i++)
            {
                get_graph_plotarr(gno, i, &p);
                if (is_set_drawable(gno, i))
                {
                    collect_cur_set_lines=lines_are_to_be_collected(gno,i);
                    collect_active=(int)collect_cur_set_lines;
                    switch (dataset_type(gno, i))
                    {
                    case SET_XY:
                    case SET_XYSIZE:
                    case SET_XYCOLOR:
                        collect_active=0;
                        drawsetsyms(gno, i, &p, refn, refx, refy, offset);
                        drawsetavalues(gno, i, &p, refn, refx, refy, offset);
                        break;
                    case SET_BAR:
                        collect_active=0;
                        drawsetavalues(gno, i, &p, refn, refx, refy, offset);
                        break;
                    case SET_BARDY:
                    case SET_BARDYDY:
                        if (!highlight_errorbars) collect_active=0;
                        drawseterrbars(gno, i, &p, refn, refx, refy, offset);
                        collect_active=0;
                        drawsetavalues(gno, i, &p, refn, refx, refy, offset);
                        break;
                    case SET_XYDY:
                    case SET_XYDYDY:
                        if (!highlight_errorbars) collect_active=0;
                        drawseterrbars(gno, i, &p, refn, refx, refy, offset);
                        collect_active=0;
                        drawsetsyms(gno, i, &p, refn, refx, refy, offset);
                        drawsetavalues(gno, i, &p, refn, refx, refy, offset);
                        break;
                    }
                    collect_active=0;
                    for (j = 0; j < p.data.len; j++)
                    {
                        refy[j] += p.data.ex[1][j];
                    }
                }
            }
        }

        if (refy != NULL)
        {
            xfree(refy);
        }
        break;
    case GRAPH_FIXED:
        for (i = 0; i < number_of_sets(gno); i++)
        {
            active_set=i;
            if (is_set_drawable(gno, i))
            {
                collect_cur_set_lines=lines_are_to_be_collected(gno,i);
                collect_active=(int)collect_cur_set_lines;
                get_graph_plotarr(gno, i, &p);
                switch (dataset_type(gno, i))
                {
                case SET_XY:
                case SET_XYSIZE:
                case SET_XYCOLOR:
                case SET_XYZ:
                    drawsetline(gno, i, &p, 0, NULL, NULL, 0.0);
                    collect_active=0;
                    drawsetsyms(gno, i, &p, 0, NULL, NULL, 0.0);
                    drawsetavalues(gno, i, &p, 0, NULL, NULL, 0.0);
                    break;
                case SET_XYDX:
                case SET_XYDY:
                case SET_XYDXDX:
                case SET_XYDYDY:
                case SET_XYDXDY:
                case SET_XYDXDXDYDY:
                    //drawsetline(gno, i, &p, 0, NULL, NULL, 0.0);
                    if (!highlight_errorbars) collect_active=0;
                    drawseterrbars(gno, i, &p, 0, NULL, NULL, 0.0);
                    collect_active=0;
                    drawsetavalues(gno, i, &p, 0, NULL, NULL, 0.0);
                    collect_active=(int)collect_cur_set_lines;
                    drawsetline(gno, i, &p, 0, NULL, NULL, 0.0);
                    collect_active=0;
                    drawsetsyms(gno, i, &p, 0, NULL, NULL, 0.0);
                    break;
                case SET_XYR:
                    collect_active=0;
                    drawcirclexy(&p);
                    drawsetsyms(gno, i, &p, 0, NULL, NULL, 0.0);
                    drawsetavalues(gno, i, &p, 0, NULL, NULL, 0.0);
                    break;
                case SET_XYVMAP:
                    //drawsetline(gno, i, &p, 0, NULL, NULL, 0.0);
                    collect_active=0;
                    drawsetvmap(gno, &p);
                    drawsetavalues(gno, i, &p, 0, NULL, NULL, 0.0);
                    collect_active=(int)collect_cur_set_lines;
                    drawsetline(gno, i, &p, 0, NULL, NULL, 0.0);
                    collect_active=0;
                    drawsetsyms(gno, i, &p, 0, NULL, NULL, 0.0);
                    break;
                default:
                    errmsg(QObject::tr("Unsupported in XY graph set type").toLocal8Bit().constData());
                    break;
                }
                collect_active=0;
            }
        }
        break;
    } /* end g.type */

}

void draw_regions(int gno)
{
    int i;
    setclipping(TRUE);
    /* draw any defined regions for this graph */
    for (i = 0; i <= MAXREGION; i++)
    {
        //cout << "region " << i << " : active=" << rg[i].active << " region_def_under_way=" << region_def_under_way << endl;
        if ((rg[i].active || region_def_under_way==i) && (rg[i].linkto == gno || rg[i].linkto == ALL_GRAPHS))
        {
            if (i==MAXREGION && region_def_under_way==-1) continue;
            setcolor(rg[i].color);
            setalpha(rg[i].alpha);
            setpattern(1);
            setlinewidth(rg[i].linew);
            setlinestyle(rg[i].lines);
            draw_region(i);
        }
    }
}

void draw_ref_point(int gno)
{
    GLocator locator;
    WPoint wp;
    VPoint vp;
    
    if (is_refpoint_active(gno))
    {
        get_graph_locator(gno, &locator);
        //qDebug() << "print_target=" << print_target << "printing_in_file=" << printing_in_file << "SCREEN=" << PRINT_TARGET_SCREEN << "Graph=" << gno << "locator_in_file=" << locator.plot_in_file;
        if (printing_in_file==true && locator.plot_in_file==FALSE) return;
        wp.x = locator.dsx;
        wp.y = locator.dsy;
        vp = Wpoint2Vpoint(wp);
        setcolor(1);
        setalpha(255);
        setpattern(1);
        setlinewidth(1.0);
        setlinestyle(1);
        symplus(vp, 0.01);
        DrawCircle (vp, 0.01);
    }
}


/* draw title and subtitle */
void draw_titles(int gno)
{
    view v;
    labels lab;
    VPoint vp1, vp2;
    
    get_graph_viewport(gno, &v);
    get_graph_labels(gno, &lab);

    vp1.x = (v.xv2 + v.xv1) / 2;
    vp1.y = (v.yv2 < v.yv1)? v.yv1 : v.yv2;
    vp2 = vp1;
    if (lab.title.s_plotstring && lab.title.s_plotstring[0])
    {
        setcolor(lab.title.color);
        setalpha(lab.title.alpha);
        setcharsize(lab.title.charsize);
        setfont(lab.title.font);
        vp1.y += 0.06;

        vp1.x+=lab.shift_title.x;
        vp1.y+=lab.shift_title.y;

        switch (lab.title.align)
        {
        default:
        case JUST_LEFT:
        currentStringAlignment=Qt::AlignLeft;
        break;
        case JUST_RIGHT:
        currentStringAlignment=Qt::AlignRight;
        break;
        case JUST_CENTER:
        currentStringAlignment=Qt::AlignCenter;
        break;
        }

        WriteString(vp1, 0, JUST_CENTER|JUST_BOTTOM, lab.title.s_plotstring);
        currentStringAlignment=Qt::AlignLeft;
        return_last_string_bounding_box(&(g[gno].labs.title.bb));
    }
    if (lab.stitle.s_plotstring && lab.stitle.s_plotstring[0])
    {
        setcolor(lab.stitle.color);
        setalpha(lab.stitle.alpha);
        setcharsize(lab.stitle.charsize);
        setfont(lab.stitle.font);
        vp2.y += 0.02;

        vp2.x+=lab.shift_subtitle.x;
        vp2.y+=lab.shift_subtitle.y;

        switch (lab.stitle.align)
        {
        default:
        case JUST_LEFT:
        currentStringAlignment=Qt::AlignLeft;
        break;
        case JUST_RIGHT:
        currentStringAlignment=Qt::AlignRight;
        break;
        case JUST_CENTER:
        currentStringAlignment=Qt::AlignCenter;
        break;
        }

        WriteString(vp2, 0, JUST_CENTER|JUST_BOTTOM, lab.stitle.s_plotstring);
        currentStringAlignment=Qt::AlignLeft;
        return_last_string_bounding_box(&(g[gno].labs.stitle.bb));
    }
}

/*
 * draw the graph frame
 */
void drawframe(int gno)
{
    view v;
    framep f;
    VPoint vps[4];

    get_graph_viewport(gno, &v);
    get_graph_framep(gno, &f);

    setpen(f.pen);
    setlinewidth(f.linew);
    setlinestyle(f.lines);

    switch (f.type) {
    case 0:
        vps[0].x = v.xv1;
        vps[0].y = v.yv1;
        vps[1].x = v.xv2;
        vps[1].y = v.yv2;
        DrawRect(vps[0], vps[1]);
        break;
    case 1:                     /* half open */
        vps[0].x = v.xv1;
        vps[0].y = v.yv2;
        vps[1].x = v.xv1;
        vps[1].y = v.yv1;
        vps[2].x = v.xv2;
        vps[2].y = v.yv1;
        DrawPolyline(vps, 3, POLYLINE_OPEN);
        break;
    case 2:                     /* break top */
        vps[0].x = v.xv1;
        vps[0].y = v.yv2;
        vps[1].x = v.xv1;
        vps[1].y = v.yv1;
        vps[2].x = v.xv2;
        vps[2].y = v.yv1;
        vps[3].x = v.xv2;
        vps[3].y = v.yv2;
        DrawPolyline(vps, 4, POLYLINE_OPEN);
        break;
    case 3:                     /* break bottom */
        vps[0].x = v.xv1;
        vps[0].y = v.yv1;
        vps[1].x = v.xv1;
        vps[1].y = v.yv2;
        vps[2].x = v.xv2;
        vps[2].y = v.yv2;
        vps[3].x = v.xv2;
        vps[3].y = v.yv1;
        DrawPolyline(vps, 4, POLYLINE_OPEN);
        break;
    case 4:                     /* break left */
        vps[0].x = v.xv1;
        vps[0].y = v.yv1;
        vps[1].x = v.xv2;
        vps[1].y = v.yv1;
        vps[2].x = v.xv2;
        vps[2].y = v.yv2;
        vps[3].x = v.xv1;
        vps[3].y = v.yv2;
        DrawPolyline(vps, 4, POLYLINE_OPEN);
        break;
    case 5:                     /* break right */
        vps[0].x = v.xv2;
        vps[0].y = v.yv1;
        vps[1].x = v.xv1;
        vps[1].y = v.yv1;
        vps[2].x = v.xv1;
        vps[2].y = v.yv2;
        vps[3].x = v.xv2;
        vps[3].y = v.yv2;
        DrawPolyline(vps, 4, POLYLINE_OPEN);
        break;
    }
}

void fillframe(int gno)
{
    view v;
    framep f;
    VPoint vp1, vp2;

    get_graph_viewport(gno, &v);
    get_graph_framep(gno, &f);
    
    /* fill coordinate frame with background color */
    if (f.fillpen.pattern != 0)
    {
        setpen(f.fillpen);
        vp1.x = v.xv1;
        vp1.y = v.yv1;
        vp2.x = v.xv2;
        vp2.y = v.yv2;
        FillRect_(vp1, vp2);
    }
}    

/*
 * draw a set filling polygon
 */
void drawsetfill(int gno, int setno, plotarr *p,
                 int refn, double *refx, double *refy, double offset)
{
    int i, len, setlen, polylen,len_poly_base;
    int line_type = p->linet;
    double *x, *y;
    double ybase;
    world w;
    WPoint wptmp;
    VPoint *vps;
    double xmin, xmax, ymin, ymax;
    int stacked_chart;
    
    if (p->filltype == SETFILL_NONE)
    {
        return;
    }
    
    if (get_graph_type(gno) == GRAPH_CHART)
    {
        x = refx;
        setlen = MIN2(p->data.len, refn);
    }
    else
    {
        x = p->data.ex[0];
        setlen = p->data.len;
    }
    y = p->data.ex[1];
    
    if (get_graph_type(gno) == GRAPH_CHART && is_graph_stacked(gno) == TRUE)
    {
        stacked_chart = TRUE;
    }
    else
    {
        stacked_chart = FALSE;
    }
    
    setclipping(TRUE);
    
    get_graph_world(gno, &w);

    switch (line_type)
    {
    case LINE_TYPE_STRAIGHT:
    case LINE_TYPE_SEGMENT2:
    case LINE_TYPE_SEGMENT3:
    case LINE_TYPE_INCR_X:
    case LINE_TYPE_DECR_X:
        if (stacked_chart == TRUE && p->filltype == SETFILL_BASELINE)
        {
            len = 2*setlen;
        }
        else
        {
            len = setlen;
        }
        vps = (VPoint *) xmalloc((len + 2) * sizeof(VPoint));
        if (vps == NULL)
        {
            errmsg(QObject::tr("Can't xmalloc in drawsetfill").toLocal8Bit().constData());
            return;
        }

        for (i = 0; i < setlen; i++)
        {
            wptmp.x = x[i];
            wptmp.y = y[i];
            if (stacked_chart == TRUE)
            {
                wptmp.y += refy[i];
            }
            vps[i] = Wpoint2Vpoint(wptmp);
            vps[i].x += offset;
        }
        if (stacked_chart == TRUE && p->filltype == SETFILL_BASELINE)
        {
            for (i = 0; i < setlen; i++)
            {
                wptmp.x = x[setlen - i - 1];
                wptmp.y = refy[setlen - i - 1];
                vps[setlen + i] = Wpoint2Vpoint(wptmp);
                vps[setlen + i].x += offset;
            }
        }
        break;
    case LINE_TYPE_LEFTSTAIR:
    case LINE_TYPE_RIGHTSTAIR:
        len = 2*setlen - 1;
        vps = (VPoint *) xmalloc((len + 2) * sizeof(VPoint));
        if (vps == NULL)
        {
            errmsg(QObject::tr("Can't xmalloc in drawsetfill").toLocal8Bit().constData());
            return;
        }

        for (i = 0; i < setlen; i++)
        {
            wptmp.x = x[i];
            wptmp.y = y[i];
            if (stacked_chart == TRUE)
            {
                wptmp.y += refy[i];
            }
            vps[2*i] = Wpoint2Vpoint(wptmp);
            vps[2*i].x += offset;
        }
        for (i = 1; i < len; i += 2)
        {
            if (line_type == LINE_TYPE_LEFTSTAIR)
            {
                vps[i].x = vps[i - 1].x;
                vps[i].y = vps[i + 1].y;
            }
            else
            {
                vps[i].x = vps[i + 1].x;
                vps[i].y = vps[i - 1].y;
            }
        }
        break;
    default:
        return;
    }
    ///I added something here to be able to fill the area between two sets!
    switch (p->filltype)
    {
    case SETFILL_POLYGON:
        polylen = len;
        break;
    case SETFILL_BASELINE:
        if (stacked_chart == TRUE)
        {
            polylen = len;
        }
        else
        {
            getsetminmax(gno, setno, &xmin, &xmax, &ymin, &ymax, FALSE);
            ybase = setybase(gno, setno);
            polylen = len + 2;
            wptmp.x = MIN2(xmax, w.xg2);
            wptmp.y = ybase;
            vps[len] = Wpoint2Vpoint(wptmp);
            vps[len].x += offset;
            wptmp.x = MAX2(xmin, w.xg1);
            wptmp.y = ybase;
            vps[len + 1] = Wpoint2Vpoint(wptmp);
            vps[len + 1].x += offset;
        }
        break;
    default:
        xfree(vps);
        return;
    }
    
    setpen(p->setfillpen);
    setfillrule(p->fillrule);
    if (p->filltype==SETFILL_POLYGON && is_set_active(gno,p->polygone_base_set))
    {
        len_poly_base=getsetlength(gno,p->polygone_base_set);
        vps = (VPoint *) xrealloc(vps,(polylen+len_poly_base) * sizeof(VPoint));
        x = getcol(gno,p->polygone_base_set,0);
        y = getcol(gno,p->polygone_base_set,1);
        for (i=0;i<len_poly_base;i++)
        {
            wptmp.x = x[len_poly_base-1-i];
            wptmp.y = y[len_poly_base-1-i];
            if (stacked_chart == TRUE)
            {
                wptmp.y += refy[i];
            }
            vps[polylen+i] = Wpoint2Vpoint(wptmp);//-2
            vps[polylen+i].x += offset;//-2
        }
        polylen+=len_poly_base;//-2
    }
    DrawPolygon(vps, polylen);
    xfree(vps);
}

/*
 * draw set's connecting line
 */
void drawsetline(int gno, int setno, plotarr *p,
                 int refn, double *refx, double *refy, double offset)
{
    int setlen, len;
    int i, ly = p->lines;
    int line_type = p->linet;
    VPoint vps[4], *vpstmp;
    WPoint wp;
    double *x, *y;
    double lw;
    double ybase;
    double xmin, xmax, ymin, ymax;
    int stacked_chart;
    
    if (get_graph_type(gno) == GRAPH_CHART) {
        x = refx;
        setlen = MIN2(p->data.len, refn);
    } else {
        x = p->data.ex[0];
        setlen = p->data.len;
    }
    y = p->data.ex[1];
    
    if (get_graph_type(gno) == GRAPH_CHART && is_graph_stacked(gno) == TRUE) {
        stacked_chart = TRUE;
    } else {
        stacked_chart = FALSE;
    }
    
    if (stacked_chart == TRUE) {
        ybase = 0.0;
    } else {
        ybase = setybase(gno, setno);
    }
    
    setclipping(TRUE);

    drawsetfill(gno, setno, p, refn, refx, refy, offset);

    setpen(p->linepen);
    setlinewidth(p->linew);
    setlinestyle(ly);

    if (stacked_chart == TRUE) {
        lw = getlinewidth();
    } else {
        lw = 0.0;
    }
    
    /* draw the line */
    if (ly != 0 && p->linepen.pattern != 0) {
        
        switch (line_type) {
        case LINE_TYPE_NONE:
            break;
        case LINE_TYPE_STRAIGHT:
            vpstmp = (VPoint *) xmalloc(setlen*sizeof(VPoint));
            if (vpstmp == NULL) {
                errmsg(QObject::tr("xmalloc failed in drawsetline()").toLocal8Bit().constData());
                break;
            }
            for (i = 0; i < setlen; i++) {
                wp.x = x[i];
                wp.y = y[i];
                if (stacked_chart == TRUE) {
                    wp.y += refy[i];
                }
                vpstmp[i] = Wpoint2Vpoint(wp);
                vpstmp[i].x += offset;
                
                vpstmp[i].y -= lw/2.0;
            }
            DrawPolyline(vpstmp, setlen, POLYLINE_OPEN);
            xfree(vpstmp);
            break;
        case LINE_TYPE_SEGMENT2:
            for (i = 0; i < setlen - 1; i += 2) {
                wp.x = x[i];
                wp.y = y[i];
                if (stacked_chart == TRUE) {
                    wp.y += refy[i];
                }
                vps[0] = Wpoint2Vpoint(wp);
                vps[0].x += offset;
                wp.x = x[i + 1];
                wp.y = y[i + 1];
                if (stacked_chart == TRUE) {
                    wp.y += refy[i + 1];
                }
                vps[1] = Wpoint2Vpoint(wp);
                vps[1].x += offset;
                
                vps[0].y -= lw/2.0;
                vps[1].y -= lw/2.0;
                
                DrawLine(vps[0], vps[1]);
            }
            break;
        case LINE_TYPE_SEGMENT3:
            for (i = 0; i < setlen - 2; i += 3) {
                wp.x = x[i];
                wp.y = y[i];
                if (stacked_chart == TRUE) {
                    wp.y += refy[i];
                }
                vps[0] = Wpoint2Vpoint(wp);
                vps[0].x += offset;
                wp.x = x[i + 1];
                wp.y = y[i + 1];
                if (stacked_chart == TRUE) {
                    wp.y += refy[i + 1];
                }
                vps[1] = Wpoint2Vpoint(wp);
                vps[1].x += offset;
                wp.x = x[i + 2];
                wp.y = y[i + 2];
                if (stacked_chart == TRUE) {
                    wp.y += refy[i + 2];
                }
                vps[2] = Wpoint2Vpoint(wp);
                vps[2].x += offset;
                DrawPolyline(vps, 3, POLYLINE_OPEN);
                
                vps[0].y -= lw/2.0;
                vps[1].y -= lw/2.0;
                vps[2].y -= lw/2.0;
            }
            if (i == setlen - 2) {
                wp.x = x[i];
                wp.y = y[i];
                if (stacked_chart == TRUE) {
                    wp.y += refy[i];
                }
                vps[0] = Wpoint2Vpoint(wp);
                vps[0].x += offset;
                wp.x = x[i + 1];
                wp.y = y[i + 1];
                if (stacked_chart == TRUE) {
                    wp.y += refy[i + 1];
                }
                vps[1] = Wpoint2Vpoint(wp);
                vps[1].x += offset;
                
                vps[0].y -= lw/2.0;
                vps[1].y -= lw/2.0;
                
                DrawLine(vps[0], vps[1]);
            }
            break;
        case LINE_TYPE_LEFTSTAIR:
        case LINE_TYPE_RIGHTSTAIR:
            len = 2*setlen - 1;
            vpstmp = (VPoint *) xmalloc(len*sizeof(VPoint));
            if (vpstmp == NULL) {
                errmsg(QObject::tr("xmalloc failed in drawsetline()").toLocal8Bit().constData());
                break;
            }
            for (i = 0; i < setlen; i++) {
                wp.x = x[i];
                wp.y = y[i];
                if (stacked_chart == TRUE) {
                    wp.y += refy[i];
                }
                vpstmp[2*i] = Wpoint2Vpoint(wp);
                vpstmp[2*i].x += offset;
            }
            for (i = 1; i < len; i += 2) {
                if (line_type == LINE_TYPE_LEFTSTAIR) {
                    vpstmp[i].x = vpstmp[i - 1].x;
                    vpstmp[i].y = vpstmp[i + 1].y;
                } else {
                    vpstmp[i].x = vpstmp[i + 1].x;
                    vpstmp[i].y = vpstmp[i - 1].y;
                }
            }
            DrawPolyline(vpstmp, len, POLYLINE_OPEN);
            xfree(vpstmp);
            break;
        case LINE_TYPE_INCR_X:
        case LINE_TYPE_DECR_X:
            for (i = 0; i < setlen - 1; i++)
            {
                if((x[i+1]>x[i] && line_type==LINE_TYPE_INCR_X) || (x[i+1]<x[i] && line_type==LINE_TYPE_DECR_X))
                {
                       wp.x = x[i];
                       wp.y = y[i];
                       if (stacked_chart == TRUE)
                       {
                           wp.y += refy[i];
                       }
                       vps[0] = Wpoint2Vpoint(wp);
                       vps[0].x += offset;
                       wp.x = x[i + 1];
                       wp.y = y[i + 1];
                       if (stacked_chart == TRUE)
                       {
                           wp.y += refy[i + 1];
                       }
                       vps[1] = Wpoint2Vpoint(wp);
                       vps[1].x += offset;
                       vps[0].y -= lw/2.0;
                       vps[1].y -= lw/2.0;
                 DrawLine(vps[0], vps[1]);
                }
            }
            break;
        default:
            errmsg(QObject::tr("Invalid line type").toLocal8Bit().constData());
            break;
        }
    }

    if (p->dropline == TRUE) {
        for (i = 0; i < setlen; i ++) {
            wp.x = x[i];
            if (stacked_chart == TRUE) {
                wp.y = refy[i];
            } else {
                wp.y = ybase;
            }
            vps[0] = Wpoint2Vpoint(wp);
            vps[0].x += offset;
            wp.x = x[i];
            wp.y = y[i];
            if (stacked_chart == TRUE) {
                wp.y += refy[i];
            }
            vps[1] = Wpoint2Vpoint(wp);
            vps[1].x += offset;
            
            vps[1].y -= lw/2.0;

            DrawLine(vps[0], vps[1]);
        }
    }
    
    getsetminmax(gno, setno, &xmin, &xmax, &ymin, &ymax, FALSE);

    if (p->baseline == TRUE && stacked_chart != TRUE) {
        wp.x = xmin;
        wp.y = ybase;
        vps[0] = Wpoint2Vpoint(wp);
        vps[0].x += offset;
        wp.x = xmax;
        vps[1] = Wpoint2Vpoint(wp);
        vps[1].x += offset;

        DrawLine(vps[0], vps[1]);
    }
}    

/* draw the symbols */
void drawsetsyms(int gno, int setno, plotarr *p,
                 int refn, double *refx, double *refy, double offset)
{
    int setlen;
    int i, sy = p->sym;
    if (sy==0) return;
    double symsize;
    VPoint vp;
    WPoint wp;
    double *x, *y, *z, *c;
    int skip = p->symskip + 1;
    int stacked_chart;
    double znorm = get_graph_znorm(gno);
    (void)setno;//useless, because all information is in plotarr * p

    if (get_graph_type(gno) == GRAPH_CHART) {
        x = refx;
        setlen = MIN2(p->data.len, refn);
    } else {
        x = p->data.ex[0];
        setlen = p->data.len;
    }
    y = p->data.ex[1];
    
    if (get_graph_type(gno) == GRAPH_CHART && is_graph_stacked(gno) == TRUE) {
        stacked_chart = TRUE;
    } else {
        stacked_chart = FALSE;
    }
    
    if (p->type == SET_XYSIZE) {
        if (znorm == 0.0) {
            return;
        }
        z = p->data.ex[2];
    } else {
        z = NULL;
    }
    
    if (p->type == SET_XYCOLOR) {
        c = p->data.ex[2];
    } else {
        c = NULL;
    }
    
    setclipping(FALSE);
    
    if ((p->sympen.pattern != 0 && p->symlines != 0) ||
            (p->symfillpen.pattern != 0)) {

        Pen fillpen;
        
        setlinewidth(p->symlinew);
        setlinestyle(p->symlines);
        setfont(p->charfont);
        for (i = 0; i < setlen; i += skip) {
            wp.x = x[i];
            wp.y = y[i];
            if (stacked_chart == TRUE) {
                wp.y += refy[i];
            }
            
            if (!is_validWPoint(wp)){
            //qDebug() << "Point " << i << " not in graph!?";
                continue;
            }

            vp = Wpoint2Vpoint(wp);
            vp.x += offset;
            //qDebug() << "Point " << i << "=(" << vp.x << "|" << vp.y << ")";
            if (z) {
                symsize = z[i]/znorm;
            } else {
                symsize = p->symsize;
            }
            if (c) {
                fillpen.color = (int) rint(c[i]);
                if (get_colortype(fillpen.color) != COLOR_MAIN) {
                    fillpen.color = 1;
                }
            } else {
                fillpen.color = p->symfillpen.color;
            }
            fillpen.alpha = p->symfillpen.alpha;
            fillpen.pattern = p->symfillpen.pattern;
            if (drawxysym(vp, symsize, sy, p->sympen, fillpen, p->symchar)
                    != RETURN_SUCCESS) {
                return;
            }
        }
    }
}

/* draw the annotative values */
void drawsetavalues(int gno, int setno, plotarr *p, int refn, double *refx, double *refy, double offset)
{
    int i;
    int setlen;
    double *x, *y, *z;
    WPoint wp;
    VPoint vp;
    int skip = p->symskip + 1;
    AValue avalue;
    char str[MAX_STRING_LENGTH];
    int stacked_chart;

    avalue = p->avalue;
    if (avalue.active != TRUE) {
        return;
    }

    if (get_graph_type(gno) == GRAPH_CHART) {
        x = refx;
        setlen = MIN2(p->data.len, refn);
    } else {
        x = p->data.ex[0];
        setlen = p->data.len;
    }
    y = p->data.ex[1];
    
    if (dataset_cols(gno, setno) > 2) {
        z = p->data.ex[2];
    } else {
        z = NULL;
    }
    
    if (get_graph_type(gno) == GRAPH_CHART && is_graph_stacked(gno) == TRUE)
    {
        stacked_chart = TRUE;
    }
    else
    {
        stacked_chart = FALSE;
    }
    
    setcharsize(avalue.size);
    setfont(avalue.font);

    for (i = 0; i < setlen; i += skip) {
        wp.x = x[i];
        wp.y = y[i];
        if (stacked_chart == TRUE) {
            wp.y += refy[i];
        }
        
        if (!is_validWPoint(wp)){
            continue;
        }
        
        vp = Wpoint2Vpoint(wp);
        
        vp.x += avalue.offset.x;
        vp.y += avalue.offset.y;
        vp.x += offset;
        
        strcpy(str, avalue.prestr);//use prepend-string
        dummy[0]='\0';//clear value-string
        switch(avalue.type)
        {
        case AVALUE_TYPE_NONE:
            break;
        case AVALUE_TYPE_X:
            strcpy(dummy, create_fstring(avalue.format, avalue.prec, wp.x,LFORMAT_TYPE_EXTENDED));
            break;
        case AVALUE_TYPE_Y:
            strcpy(dummy, create_fstring(avalue.format, avalue.prec, wp.y,LFORMAT_TYPE_EXTENDED));
            break;
        case AVALUE_TYPE_XY:
            strcpy(dummy, create_fstring(avalue.format, avalue.prec, wp.x,LFORMAT_TYPE_EXTENDED));
            if (DecimalPointToUse=='.')
                strcat(dummy, ", ");
            else
                strcat(dummy, " | ");
            strcat(dummy, create_fstring(avalue.format, avalue.prec, wp.y,LFORMAT_TYPE_EXTENDED));
            break;
        case AVALUE_TYPE_STRING:
            if (p->data.s != NULL && p->data.s[i] != NULL)
            {
                strcpy(dummy, p->data.s[i]);
            }
            break;
        case AVALUE_TYPE_Z:
            if (z != NULL)
            {
                strcpy(dummy, create_fstring(avalue.format, avalue.prec, z[i],LFORMAT_TYPE_EXTENDED));
            }
            break;
        default:
            errmsg(QObject::tr("Invalid type of ann. value").toLocal8Bit().constData());
            return;
        }//value-string generated
//cout << "Annotate Values: DecimalPointToUse=" << DecimalPointToUse << endl;
    SetDecimalSeparatorToUserValue(dummy,false);//replace Decimal separator if necessary
        strcat(str, dummy);//add value-string to annotation
        strcat(str, avalue.appstr);//add append-string
        setcolor(avalue.color);
        setalpha(avalue.alpha);
        switch (avalue.align)
        {
        case JUST_LEFT:
        currentStringAlignment=Qt::AlignLeft;
        break;
        case JUST_RIGHT:
        currentStringAlignment=Qt::AlignRight;
        break;
        case JUST_CENTER:
        currentStringAlignment=Qt::AlignCenter;
        break;
        }
        WriteString(vp, avalue.angle, JUST_CENTER|JUST_BOTTOM, str);
        currentStringAlignment=Qt::AlignLeft;
    }
}

void drawseterrbars(int gno, int setno, plotarr *p,
                    int refn, double *refx, double *refy, double offset)
{
    int i, n, no_errorbars;
    double *x, *y;
    double *dx_plus, *dx_minus, *dy_plus, *dy_minus, *dtmp;
    PlacementType ptype = p->errbar.ptype;
    WPoint wp1, wp2;
    VPoint vp1, vp2;
    int stacked_chart;
    int skip = p->symskip + 1;
    (void)setno;//useless, because all information is in plotarr * p

    if (p->errbar.active != TRUE) {
        return;
    }
    
    if (get_graph_type(gno) == GRAPH_CHART) {
        x = refx;
        n = MIN2(p->data.len, refn);
    } else {
        x = p->data.ex[0];
        n = p->data.len;
    }
    y = p->data.ex[1];
    
    if (get_graph_type(gno) == GRAPH_CHART && is_graph_stacked(gno) == TRUE) {
        stacked_chart = TRUE;
    } else {
        stacked_chart = FALSE;
    }
    
    dx_plus  = NULL;
    dx_minus = NULL;
    dy_plus  = NULL;
    dy_minus = NULL;
    no_errorbars=TRUE;
    switch (p->type) {
    case SET_XYDX:
        dx_plus = p->data.ex[2];
        break;
    case SET_XYDY:
    case SET_BARDY:
        dy_plus = p->data.ex[2];
        break;
    case SET_XYDXDX:
        dx_plus  = p->data.ex[2];
        dx_minus = p->data.ex[3];
        break;
    case SET_XYDYDY:
    case SET_BARDYDY:
        dy_plus  = p->data.ex[2];
        dy_minus = p->data.ex[3];
        break;
    case SET_XYDXDY:
        dx_plus = p->data.ex[2];
        dy_plus = p->data.ex[3];
        break;
    case SET_XYDXDXDYDY:
        dx_plus  = p->data.ex[2];
        dx_minus = p->data.ex[3];
        dy_plus  = p->data.ex[4];
        dy_minus = p->data.ex[5];
        break;
    default:
        return;
    }
    
    switch (ptype) {
    case PLACEMENT_OPPOSITE:
        dtmp     = dx_minus;
        dx_minus = dx_plus;
        dx_plus  = dtmp;
        dtmp     = dy_minus;
        dy_minus = dy_plus;
        dy_plus  = dtmp;
        break;
    case PLACEMENT_BOTH:
        if (dx_minus == NULL && dy_minus == NULL) {
            dx_minus = dx_plus;
            dy_minus = dy_plus;
        }
        break;
    default:
        break;
    }
    
    if (dx_plus || dy_plus || dx_minus || dy_minus)
    {
    no_errorbars=FALSE;
    }

    setclipping(TRUE);
    
int boundlen=0;
VPoint * x_error_line[2];
VPoint * y_error_line[2];
if (p->errbar.connect_bars>CONNECT_ERRBARS_NONE)
{
    for (i=0;i<2;i++)
    {
    x_error_line[i]=new VPoint[(n/skip+2)*2];
    y_error_line[i]=new VPoint[(n/skip+2)*2];
    }
setpen(p->errbar.pen);
setlinewidth(p->errbar.linew);
setlinestyle(p->errbar.lines);
    if (p->errbar.connect_bars>CONNECT_ERRBARS_X_ONLY)
    {
    setpattern(p->errbar.pen.pattern);
    }
}

    for (i = 0; i < n; i += skip) {
        wp1.x = x[i];
        wp1.y = y[i];
        if (stacked_chart == TRUE) {
            wp1.y += refy[i];
        }
        if (is_validWPoint(wp1) == FALSE && no_errorbars == TRUE ) {
            continue;
        }/// we have to check here for the errorbars to be inside the graph --> check for side of graph of point and errorbars

        vp1 = Wpoint2Vpoint(wp1);
        vp1.x += offset;

        if (dx_plus != NULL) {
            wp2 = wp1;
            wp2.x += dx_plus[i];//fabs(dx_plus[i]);
            vp2 = Wpoint2Vpoint(wp2);
            vp2.x += offset;
            //if (p->errbar.connect_bars==CONNECT_ERRBARS_XY || p->errbar.connect_bars==CONNECT_ERRBARS_X_ONLY)
            if (p->errbar.connect_bars>CONNECT_ERRBARS_NONE)
            x_error_line[0][boundlen]=vp2;
            cur_errorbar=3;//none; 1=yplus,2=yminus,3=xplus,4=xminus
            drawerrorbar(vp1, vp2, &p->errbar);
        }
        if (dx_minus != NULL) {
            wp2 = wp1;
            wp2.x -= dx_minus[i];//fabs(dx_minus[i]);
            vp2 = Wpoint2Vpoint(wp2);
            vp2.x += offset;
            //if (p->errbar.connect_bars==CONNECT_ERRBARS_XY || p->errbar.connect_bars==CONNECT_ERRBARS_X_ONLY)
            if (p->errbar.connect_bars>CONNECT_ERRBARS_NONE)
            x_error_line[1][boundlen]=vp2;
            cur_errorbar=4;//none; 1=yplus,2=yminus,3=xplus,4=xminus
            drawerrorbar(vp1, vp2, &p->errbar);
        }
        if (dy_plus != NULL) {
            wp2 = wp1;
            wp2.y += dy_plus[i];//fabs(dy_plus[i]);
            vp2 = Wpoint2Vpoint(wp2);
            vp2.x += offset;
            //if (p->errbar.connect_bars==CONNECT_ERRBARS_XY || p->errbar.connect_bars==CONNECT_ERRBARS_Y_ONLY)
            if (p->errbar.connect_bars>CONNECT_ERRBARS_NONE)
            y_error_line[0][boundlen]=vp2;
            cur_errorbar=1;//none; 1=yplus,2=yminus,3=xplus,4=xminus
            drawerrorbar(vp1, vp2, &p->errbar);
        }
        if (dy_minus != NULL) {
            wp2 = wp1;
            wp2.y -= dy_minus[i];//fabs(dy_minus[i]);
            vp2 = Wpoint2Vpoint(wp2);
            vp2.x += offset;
            //if (p->errbar.connect_bars==CONNECT_ERRBARS_XY || p->errbar.connect_bars==CONNECT_ERRBARS_Y_ONLY)
            if (p->errbar.connect_bars>CONNECT_ERRBARS_NONE)
            y_error_line[1][boundlen]=vp2;
            cur_errorbar=2;//none; 1=yplus,2=yminus,3=xplus,4=xminus
            drawerrorbar(vp1, vp2, &p->errbar);
        }
        boundlen++;
    }
    if (p->errbar.connect_bars>CONNECT_ERRBARS_NONE)
    {
        int boundlen2=0;
        if (ptype==PLACEMENT_OPPOSITE || ptype==PLACEMENT_NORMAL)
        {
            for (i = 0; i < n; i += skip)
            {
                wp1.x = x[i];
                wp1.y = y[i];
            if (dx_plus == NULL) {
                vp2 = Wpoint2Vpoint(wp1);
                vp2.x += offset;
                x_error_line[0][boundlen2]=vp2;
                cur_errorbar=3;//none; 1=yplus,2=yminus,3=xplus,4=xminus
            }
            if (dx_minus == NULL) {
                vp2 = Wpoint2Vpoint(wp1);
                vp2.x += offset;
                x_error_line[1][boundlen2]=vp2;
                cur_errorbar=4;//none; 1=yplus,2=yminus,3=xplus,4=xminus
            }
            if (dy_plus == NULL) {
                vp2 = Wpoint2Vpoint(wp1);
                vp2.x += offset;
                y_error_line[0][boundlen2]=vp2;
                cur_errorbar=1;//none; 1=yplus,2=yminus,3=xplus,4=xminus
            }
            if (dy_minus == NULL) {
                vp2 = Wpoint2Vpoint(wp1);
                vp2.x += offset;
                y_error_line[1][boundlen2]=vp2;
                cur_errorbar=2;//none; 1=yplus,2=yminus,3=xplus,4=xminus
            }
            boundlen2++;
            }
        }

    //actually draw the lines
        if ((dx_plus != NULL || dx_minus != NULL) && (p->errbar.connect_bars==CONNECT_ERRBARS_XY || p->errbar.connect_bars==CONNECT_ERRBARS_X_ONLY))
        {
            if (dx_plus != NULL)
            {
            DrawPolyline(x_error_line[0], boundlen, POLYLINE_OPEN);
            }
            if (dx_minus != NULL)
            {
            DrawPolyline(x_error_line[1], boundlen, POLYLINE_OPEN);
            }
        }
        if ((dy_plus != NULL || dy_minus != NULL) && (p->errbar.connect_bars==CONNECT_ERRBARS_XY || p->errbar.connect_bars==CONNECT_ERRBARS_Y_ONLY))
        {
            if (dy_plus != NULL)
            {
            DrawPolyline(y_error_line[0], boundlen, POLYLINE_OPEN);
            }
            if (dy_minus != NULL)
            {
            DrawPolyline(y_error_line[1], boundlen, POLYLINE_OPEN);
            }
        }
        if ((dx_plus != NULL || dx_minus != NULL) && (p->errbar.connect_bars==CONNECT_ERRBARS_FILL_X || p->errbar.connect_bars==CONNECT_ERRBARS_FILL_XY))
        {
            int n_polygon=boundlen*2;
            VPoint *vps_polygon=new VPoint[n_polygon];

            for (int k=0;k<boundlen;k++)
            vps_polygon[k]=x_error_line[0][k];

            for (int k=0;k<boundlen;k++)
            vps_polygon[boundlen+k]=x_error_line[1][boundlen-1-k];

            setalpha(p->errbar.pen.alpha);
            DrawPolygon(vps_polygon,n_polygon);
            setalpha(255);
            DrawPolyline(vps_polygon,n_polygon,POLYLINE_CLOSED);
            setalpha(p->errbar.pen.alpha);
            delete[] vps_polygon;
        }
        if ((dy_plus != NULL || dy_minus != NULL) && (p->errbar.connect_bars==CONNECT_ERRBARS_FILL_Y || p->errbar.connect_bars==CONNECT_ERRBARS_FILL_XY))
        {
            int n_polygon=boundlen*2;
            VPoint *vps_polygon=new VPoint[n_polygon];

            for (int k=0;k<boundlen;k++)
            vps_polygon[k]=y_error_line[0][k];

            for (int k=0;k<boundlen;k++)
            vps_polygon[boundlen+k]=y_error_line[1][boundlen-1-k];

            setalpha(p->errbar.pen.alpha);
            DrawPolygon(vps_polygon,n_polygon);
            setalpha(255);
            DrawPolyline(vps_polygon,n_polygon,POLYLINE_CLOSED);
            setalpha(p->errbar.pen.alpha);
            delete[] vps_polygon;
        }
    //clear memory
        for (i=0;i<2;i++)
        {
        delete[] x_error_line[i];
        delete[] y_error_line[i];
        }
    }
}

/*
 * draw hi/lo-open/close
 */
void drawsethilo(plotarr *p)
{
    int i;
    double *x = p->data.ex[0], *y1 = p->data.ex[1];
    double *y2 = p->data.ex[2], *y3 = p->data.ex[3], *y4 = p->data.ex[4];
    double ilen = 0.02*p->symsize;
    int skip = p->symskip + 1;
    WPoint wp;
    VPoint vp1, vp2;

    if (p->symlines != 0) {
        setpen(p->sympen);
        setlinewidth(p->symlinew);
        setlinestyle(p->symlines);
        for (i = 0; i < p->data.len; i += skip) {
            wp.x = x[i];
            wp.y = y1[i];
            vp1 = Wpoint2Vpoint(wp);
            wp.y = y2[i];
            vp2 = Wpoint2Vpoint(wp);
            DrawLine(vp1, vp2);
            wp.y = y3[i];
            vp1 = Wpoint2Vpoint(wp);
            vp2 = vp1;
            vp2.x -= ilen;
            DrawLine(vp1, vp2);
            wp.y = y4[i];
            vp1 = Wpoint2Vpoint(wp);
            vp2 = vp1;
            vp2.x += ilen;
            DrawLine(vp1, vp2);
        }
    }
}

/*
 * draw 2D bars
 */
void drawsetbars(int gno, int setno, plotarr *p,
                 int refn, double *refx, double *refy, double offset)
{
    int i, n;
    double *x, *y;
    double lw, bw = 0.01*p->symsize;
    int skip = p->symskip + 1;
    double ybase;
    WPoint wp;
    VPoint vp1, vp2;
    int stacked_chart;
    
    if (get_graph_type(gno) == GRAPH_CHART) {
        x = refx;
        n = MIN2(p->data.len, refn);
    } else {
        x = p->data.ex[0];
        n = p->data.len;
    }
    y = p->data.ex[1];
    
    if (get_graph_type(gno) == GRAPH_CHART && is_graph_stacked(gno) == TRUE) {
        stacked_chart = TRUE;
    } else {
        stacked_chart = FALSE;
    }


    
    if (stacked_chart == TRUE) {
        ybase = 0.0;
    } else {
        ybase = setybase(gno, setno);
    }

    setlinewidth(p->symlinew);
    setlinestyle(p->symlines);
    if (get_graph_type(gno) == GRAPH_CHART &&
            p->symlines != 0 && p->sympen.pattern != 0) {
        lw = getlinewidth();
    } else {
        lw = 0.0;
    }

    if (p->symfillpen.pattern != 0) {
        setpen(p->symfillpen);
        for (i = 0; i < n; i += skip) {
            wp.x = x[i];
            if (stacked_chart == TRUE) {
                wp.y = refy[i];
            } else {
                wp.y = ybase;
            }
            vp1 = Wpoint2Vpoint(wp);
            vp1.x -= bw;
            vp1.x += offset;
            wp.x = x[i];
            if (stacked_chart == TRUE) {
                wp.y += y[i];
            } else {
                wp.y = y[i];
            }
            vp2 = Wpoint2Vpoint(wp);
            vp2.x += bw;
            vp2.x += offset;
            
            vp1.x += lw/2.0;
            vp2.x -= lw/2.0;
            vp1.y += lw/2.0;
            
            FillRect_(vp1, vp2);
        }
    }
    if (p->symlines != 0 && p->sympen.pattern != 0) {
        setpen(p->sympen);
        for (i = 0; i < n; i += skip) {
            wp.x = x[i];
            if (stacked_chart == TRUE) {
                wp.y = refy[i];
            } else {
                wp.y = ybase;
            }
            vp1 = Wpoint2Vpoint(wp);
            vp1.x -= bw;
            vp1.x += offset;
            wp.x = x[i];
            if (stacked_chart == TRUE) {
                wp.y += y[i];
            } else {
                wp.y = y[i];
            }
            vp2 = Wpoint2Vpoint(wp);
            vp2.x += bw;
            vp2.x += offset;

            vp1.x += lw/2.0;
            vp2.x -= lw/2.0;
            vp1.y += lw/2.0;

            DrawRect(vp1, vp2);
        }
    }
}

void drawcirclexy(plotarr *p)
{
    int i, setlen;
    double *x, *y, *r;
    int skip = p->symskip + 1;
    WPoint wp;
    VPoint vp1, vp2;

    setclipping(TRUE);
    
    setlen = p->data.len;
    x = p->data.ex[0];
    y = p->data.ex[1];
    r = p->data.ex[2];

    setfillrule(p->fillrule);
    setlinewidth(p->linew);
    setlinestyle(p->lines);

    for (i = 0; i < setlen; i += skip) {
        wp.x = x[i];
        wp.y = y[i];
        /* TODO: remove once ellipse clipping works */
        if (!is_validWPoint(wp)){
            continue;
        }
        wp.x = x[i] - r[i];
        wp.y = y[i] - r[i];
        vp1 = Wpoint2Vpoint(wp);
        wp.x = x[i] + r[i];
        wp.y = y[i] + r[i];
        vp2 = Wpoint2Vpoint(wp);
        if (p->filltype != SETFILL_NONE) {
            setpen(p->setfillpen);
            DrawFilledEllipse(vp1, vp2);
        }
        setpen(p->linepen);
        DrawEllipse(vp1, vp2);
    }
}

/* Arrows for vector map plots */
void drawsetvmap(int gno, plotarr *p)
{
    int i, setlen;
    double znorm = get_graph_znorm(gno);
    int skip = p->symskip + 1;
    double *x, *y, *vx, *vy;
    WPoint wp;
    VPoint vp1, vp2;
    Arrow arrow = {0, 1.0, 1.0, 0.0};
    
    Errbar eb = p->errbar;
    
    setclipping(TRUE);
    
    if (znorm == 0.0) {
        return;
    }
    
    setlen = p->data.len;
    x = p->data.ex[DATA_X];
    y = p->data.ex[DATA_Y];
    vx = p->data.ex[DATA_Y1];
    vy = p->data.ex[DATA_Y2];

    arrow.length = 2*eb.barsize;

    setpen(p->errbar.pen);

    for (i = 0; i < setlen; i += skip) {
        wp.x = x[i];
        wp.y = y[i];
        if (!is_validWPoint(wp)){
            continue;
        }
        vp1 = Wpoint2Vpoint(wp);
        vp2.x = vp1.x + vx[i]/znorm;
        vp2.y = vp1.y + vy[i]/znorm;

        setlinewidth(eb.riser_linew);
        setlinestyle(eb.riser_lines);
        DrawLine(vp1, vp2);

        setlinewidth(eb.linew);
        setlinestyle(eb.lines);
        draw_arrowhead(vp1, vp2, &arrow);
    }
}

void drawsetboxplot(plotarr *p)
{
    int i;
    double *x, *md, *lb, *ub, *lw, *uw;
    double size = 0.01*p->symsize;
    int skip = p->symskip + 1;
    WPoint wp;
    VPoint vp1, vp2;

    x  = p->data.ex[0];
    md = p->data.ex[1];
    lb = p->data.ex[2];
    ub = p->data.ex[3];
    lw = p->data.ex[4];
    uw = p->data.ex[5];

    setclipping(TRUE);

    for (i = 0; i < p->data.len; i += skip) {
        wp.x =  x[i];

        wp.y = lb[i];
        vp1 = Wpoint2Vpoint(wp);
        wp.y = ub[i];
        vp2 = Wpoint2Vpoint(wp);
        
        /* whiskers */
        if (p->errbar.active == TRUE) {
            VPoint vp3;
            wp.y = lw[i];
            vp3 = Wpoint2Vpoint(wp);
            drawerrorbar(vp1, vp3, &p->errbar);
            wp.y = uw[i];
            vp3 = Wpoint2Vpoint(wp);
            drawerrorbar(vp2, vp3, &p->errbar);
        }

        /* box */
        vp1.x -= size;
        vp2.x += size;
        setpen(p->symfillpen);
        FillRect_(vp1, vp2);

        setpen(p->sympen);
        setlinewidth(p->symlinew);
        setlinestyle(p->symlines);
        DrawRect(vp1, vp2);

        /* median line */
        wp.y = md[i];
        vp2 = vp1 = Wpoint2Vpoint(wp);
        vp1.x -= size;
        vp2.x += size;
        DrawLine(vp1, vp2);
    }
}

int drawxysym(VPoint vp, double size, int symtype,
              Pen sympen, Pen symfillpen, char s)
{
    double symsize;
    VPoint vps[4];
    char buf[2];
    
    symsize = size*0.01;
    
    switch (symtype) {
    case SYM_NONE:
        break;
    case SYM_CIRCLE:
        setpen(symfillpen);
        DrawFilledCircle (vp, symsize);
        setpen(sympen);
        DrawCircle (vp, symsize);
        break;
    case SYM_SQUARE:
        symsize *= 0.85;
        vps[0].x = vp.x - symsize;
        vps[0].y = vp.y - symsize;
        vps[1].x = vps[0].x;
        vps[1].y = vp.y + symsize;
        vps[2].x = vp.x + symsize;
        vps[2].y = vps[1].y;
        vps[3].x = vps[2].x;
        vps[3].y = vps[0].y;
        
        setpen(symfillpen);
        DrawPolygon (vps, 4);
        setpen(sympen);
        DrawPolyline (vps, 4, POLYLINE_CLOSED);
        break;
    case SYM_DIAMOND:
        vps[0].x = vp.x;
        vps[0].y = vp.y + symsize;
        vps[1].x = vp.x - symsize;
        vps[1].y = vp.y;
        vps[2].x = vps[0].x;
        vps[2].y = vp.y - symsize;
        vps[3].x = vp.x + symsize;
        vps[3].y = vps[1].y;
        
        setpen(symfillpen);
        DrawPolygon (vps, 4);
        setpen(sympen);
        DrawPolyline (vps, 4, POLYLINE_CLOSED);
        break;
    case SYM_TRIANG1:
        vps[0].x = vp.x;
        vps[0].y = vp.y + 2*M_SQRT1_3*symsize;
        vps[1].x = vp.x - symsize;
        vps[1].y = vp.y - M_SQRT1_3*symsize;
        vps[2].x = vp.x + symsize;
        vps[2].y = vps[1].y;
        
        setpen(symfillpen);
        DrawPolygon (vps, 3);
        setpen(sympen);
        DrawPolyline (vps, 3, POLYLINE_CLOSED);
        break;
    case SYM_TRIANG2:
        vps[0].x = vp.x - 2*M_SQRT1_3*symsize;
        vps[0].y = vp.y;
        vps[1].x = vp.x + M_SQRT1_3*symsize;
        vps[1].y = vp.y - symsize;
        vps[2].x = vps[1].x;
        vps[2].y = vp.y + symsize;
        
        setpen(symfillpen);
        DrawPolygon (vps, 3);
        setpen(sympen);
        DrawPolyline (vps, 3, POLYLINE_CLOSED);
        break;
    case SYM_TRIANG3:
        vps[0].x = vp.x - symsize;
        vps[0].y = vp.y + M_SQRT1_3*symsize;
        vps[1].x = vp.x;
        vps[1].y = vp.y - 2*M_SQRT1_3*symsize;
        vps[2].x = vp.x + symsize;
        vps[2].y = vps[0].y;
        
        setpen(symfillpen);
        DrawPolygon (vps, 3);
        setpen(sympen);
        DrawPolyline (vps, 3, POLYLINE_CLOSED);
        break;
    case SYM_TRIANG4:
        vps[0].x = vp.x - M_SQRT1_3*symsize;
        vps[0].y = vp.y + symsize;
        vps[1].x = vps[0].x;
        vps[1].y = vp.y - symsize;
        vps[2].x = vp.x + 2*M_SQRT1_3*symsize;
        vps[2].y = vp.y;
        
        setpen(symfillpen);
        DrawPolygon (vps, 3);
        setpen(sympen);
        DrawPolyline (vps, 3, POLYLINE_CLOSED);
        break;
    case SYM_PLUS:
        setpen(sympen);
        symplus(vp, symsize);
        break;
    case SYM_X:
        setpen(sympen);
        symx(vp, symsize);
        break;
    case SYM_SPLAT:
        setpen(sympen);
        symsplat(vp, symsize);
        break;
    case SYM_CHAR:
        setcolor(sympen.color);
        setalpha(sympen.alpha);
        buf[0] = s;
        buf[1] = '\0';
        setcharsize(size);
        WriteString(vp, 0, JUST_CENTER|JUST_MIDDLE, buf);
        break;
    default:
        errmsg(QObject::tr("Invalid symbol type").toLocal8Bit().constData());
        return RETURN_FAILURE;
    }
    return RETURN_SUCCESS;
}

static void drawlegbarsym(VPoint vp, double size, Pen sympen, Pen symfillpen)
{
    double width, height;
    VPoint vps[4];

    width  = 0.02*size;
    height = 0.02*getcharsize();
    
    vps[0].x = vps[1].x = vp.x - width/2;
    vps[2].x = vps[3].x = vp.x + width/2;
    vps[0].y = vps[3].y = vp.y - height/2;
    vps[1].y = vps[2].y = vp.y + height/2;
    
    setpen(symfillpen);
    DrawPolygon (vps, 4);
    setpen(sympen);
    DrawPolyline (vps, 4, POLYLINE_CLOSED);
}

void drawerrorbar(VPoint vp1, VPoint vp2, Errbar *eb)
{
    double ilen;
    VPoint vp_plus, vp_minus;
    VVector lvv;
    double vlength;
    static Arrow arrow = {0, 1.0, 1.0, 0.0};

    lvv.x = vp2.x - vp1.x;
    lvv.y = vp2.y - vp1.y;

    vlength = hypot(lvv.x, lvv.y);
    if (vlength == 0.0) {
        return;
    }
    
    lvv.x /= vlength;
    lvv.y /= vlength;
    
    setpen(eb->pen);
    
    if (eb->arrow_clip && is_validVPoint(vp2) == FALSE) {
        vp2.x = vp1.x + eb->cliplen*lvv.x;
        vp2.y = vp1.y + eb->cliplen*lvv.y;
        setlinewidth(eb->riser_linew);
        setlinestyle(eb->riser_lines);
        DrawLine(vp1, vp2);
        arrow.length = 2*eb->barsize;
        setlinewidth(eb->linew);
        setlinestyle(eb->lines);
        if (eb->connect_bars==CONNECT_ERRBARS_NONE || ((eb->connect_bars==CONNECT_ERRBARS_X_ONLY || eb->connect_bars==CONNECT_ERRBARS_FILL_X) && (cur_errorbar==1 || cur_errorbar==2)) || ((eb->connect_bars==CONNECT_ERRBARS_Y_ONLY || eb->connect_bars==CONNECT_ERRBARS_FILL_Y) && (cur_errorbar==3 || cur_errorbar==4)))
        draw_arrowhead(vp1, vp2, &arrow);
    } else {
        setlinewidth(eb->riser_linew);
        setlinestyle(eb->riser_lines);
        DrawLine(vp1, vp2);
        setlinewidth(eb->linew);
        setlinestyle(eb->lines);
        ilen = 0.01*eb->barsize;
        vp_minus.x = vp2.x - ilen*lvv.y;
        vp_minus.y = vp2.y + ilen*lvv.x;
        vp_plus.x  = vp2.x + ilen*lvv.y;
        vp_plus.y  = vp2.y - ilen*lvv.x;
        if (eb->connect_bars==CONNECT_ERRBARS_NONE || ((eb->connect_bars==CONNECT_ERRBARS_X_ONLY || eb->connect_bars==CONNECT_ERRBARS_FILL_X) && (cur_errorbar==1 || cur_errorbar==2)) || ((eb->connect_bars==CONNECT_ERRBARS_Y_ONLY || eb->connect_bars==CONNECT_ERRBARS_FILL_Y) && (cur_errorbar==3 || cur_errorbar==4)))
        DrawLine(vp_minus, vp_plus);
    }
}

/* --------------------------------------------------------------- */
/* Objects ... TODO: move to draw.c or separate file */

void draw_objects(int gno)
{
    int i;

    setclipping(FALSE);         /* shut down clipping for strings, boxes,
                                 * lines, and legends */
    
    /* Temporarily; pattern property should be part of object props */
    setpattern(1);
    
    for (i = 0; i < number_of_boxes(); i++)
    {
        if (isactive_box(i))
        {
            draw_box(gno, i);
        }
    }
    for (i = 0; i < number_of_ellipses(); i++) {
        if (isactive_ellipse(i)) {
            draw_ellipse(gno, i);
        }
    }
    for (i = 0; i < number_of_lines(); i++) {
        if (isactive_line(i)) {
            draw_line(gno, i);
        }
    }
    for (i = 0; i < number_of_strings(); i++) {
        if (isactive_string(i)) {
            draw_string(gno, i);
        }
    }
    setclipping(TRUE);
}


/*
 * draw annotative text
 */
void draw_string(int gno, int i)
{
static plotstr pstr;
static VPoint vp;
static WPoint wptmp;
static Page_geometry pg;
static view gv,st_v;
static int align_t,align_x,align_y;
static double dx,dy;

    get_graph_string(i, &pstr);

    if (gno != -2) {
        if (pstr.loctype == COORD_WORLD && pstr.gno != gno) {
            return;
        }
        if (pstr.loctype == COORD_VIEW && gno != -1) {
            return;
        }
    }

    if (strlen(pstr.s_plotstring) && (pstr.charsize > 0.0) && pstr.active) {
        if (pstr.loctype == COORD_WORLD) {
            wptmp.x = pstr.x;
            wptmp.y = pstr.y;
            vp = Wpoint2Vpoint(wptmp);
        } else {
            vp.x = pstr.x;
            vp.y = pstr.y;
        }
        align_y=pstr.master_align%10;
        align_x=(pstr.master_align/10)%10;
        align_t=(pstr.master_align/100)%10;

        setcolor(pstr.color);
        setalpha(pstr.alpha);
        setpattern(1);
        setcharsize(pstr.charsize);
        setfont(pstr.font);

        activate_bbox(BBOX_TYPE_TEMP, TRUE);
        reset_bbox(BBOX_TYPE_TEMP);

/*if (curdevice==0 && useQtFonts==true)
{
vp.x+=0.2;
WriteQtString(vp, pstr.rot, pstr.just, pstr.s,pstr.charsize,pstr.font,pstr.color);
vp.x-=0.2;
}
else
{
WriteString(vp, pstr.rot, pstr.just, pstr.s);
}*/
        switch (pstr.align)
        {
        case JUST_LEFT:
        currentStringAlignment=Qt::AlignLeft;
        break;
        case JUST_RIGHT:
        currentStringAlignment=Qt::AlignRight;
        break;
        case JUST_CENTER:
        currentStringAlignment=Qt::AlignCenter;
        break;
        }
        //qDebug() << pstr.s_plotstring << " Alignment:" << pstr.align;
        dx=dy=0.0;
        if (align_t!=MASTER_ALIGN_TARGET_NONE)
        {
        set_draw_mode(FALSE);
        WriteString(vp, pstr.rot, pstr.just, pstr.s_plotstring);
        set_draw_mode(TRUE);
        reset_bbox(BBOX_TYPE_TEMP);
        return_last_string_bounding_box(&st_v);
            if (align_t==MASTER_ALIGN_TARGET_PAGE)
            {
            pg=get_page_geometry();
            //qDebug() << "Page Geometry=" << pg.width << "x" << pg.height;
            gv.xv1=gv.xv2=gv.yv1=gv.yv2=0.0;
                if (pg.width > pg.height)//landscape
                {
                gv.yv2=1.0;
                gv.xv2=pg.width*1.0/pg.height;
                }
                else//portrait
                {
                gv.yv2=pg.height*1.0/pg.width;
                gv.xv2=1.0;
                }
            }
            else if (get_graph_viewport(pstr.gno,&gv)==RETURN_SUCCESS)//Graph
            {
            //qDebug() << gv.xv1 << "|" << gv.yv1 << "x" << gv.xv2 << "|" << gv.yv2;
            ;//qDebug() << "Graph viewport found!";
            }
            //qDebug() << "GV(" << pstr.gno << ")=" << gv.xv1 << "|" << gv.yv1 << "x" << gv.xv2 << "|" << gv.yv2;
            //qDebug() << "ST_V =" << st_v.xv1 << "|" << st_v.yv1 << "x" << st_v.xv2 << "|" << st_v.yv2;
            switch (align_x)
            {
            case MASTER_ALIGN_X_NONE:
            dx=0.0;
            break;
            case MASTER_ALIGN_X_LEFT_IN:
            dx=st_v.xv1-gv.xv1;
            break;
            case MASTER_ALIGN_X_LEFT_OUT:
            dx=st_v.xv1-gv.xv1+(st_v.xv2-st_v.xv1);
            break;
            case MASTER_ALIGN_X_CENTER:
            dx=st_v.xv1-gv.xv1-(gv.xv2-gv.xv1)*0.5+(st_v.xv2-st_v.xv1)*0.5;
            break;
            case MASTER_ALIGN_X_RIGHT_IN:
            dx=st_v.xv1-gv.xv2+(st_v.xv2-st_v.xv1);
            break;
            case MASTER_ALIGN_X_RIGHT_OUT:
            dx=st_v.xv1-gv.xv2;
            break;
            }

            switch (align_y)
            {
            case MASTER_ALIGN_Y_NONE:
            dy=0.0;
            break;
            case MASTER_ALIGN_Y_DOWN_IN:
            dy=st_v.yv1-gv.yv1;
            break;
            case MASTER_ALIGN_Y_DOWN_OUT:
            dy=st_v.yv1-gv.yv1+(st_v.yv2-st_v.yv1);
            break;
            case MASTER_ALIGN_Y_CENTER:
            dy=st_v.yv1-gv.yv1-(gv.yv2-gv.yv1)*0.5+(st_v.yv2-st_v.yv1)*0.5;
            break;
            case MASTER_ALIGN_Y_UP_IN:
            dy=st_v.yv1-gv.yv2+(st_v.yv2-st_v.yv1);
            break;
            case MASTER_ALIGN_Y_UP_OUT:
            dy=st_v.yv1-gv.yv2;
            break;
            }
        }
        //qDebug() << "master_align=" << pstr.master_align << "dx=" << dx << "dy=" << dy;
        vp.x-=dx;
        vp.y-=dy;

        WriteString(vp, pstr.rot, pstr.just, pstr.s_plotstring);

        currentStringAlignment=Qt::AlignLeft;

        pstr.bb = get_bbox(BBOX_TYPE_TEMP);
        set_graph_string(i, &pstr);
#ifdef DEBUG_SHOW_TEXT_BOUNDING_BOXES
    VPoint vp1,vp2;
    vp1.x=pstr.bb.xv1;
    vp1.y=pstr.bb.yv1;
    vp2.x=pstr.bb.xv2;
    vp2.y=pstr.bb.yv2;
    draw_simple_rectangle(vp1,vp2,3);/// draw whole bounding box in green
#endif
    }
}

/*
 * draw annotative boxes
 */
void draw_box(int gno, int i)
{
    boxtype b;
    WPoint wptmp;
    VPoint vp1, vp2;

    get_graph_box(i, &b);
    if (gno != -2) {
        if (b.loctype == COORD_WORLD && b.gno != gno) {
            return;
        }
        if (b.loctype == COORD_VIEW && gno != -1) {
            return;
        }
    }
    if (b.active) {
        setclipping(FALSE);

        if (b.loctype == COORD_WORLD) {
            wptmp.x = b.x1;
            wptmp.y = b.y1;
            vp1 = Wpoint2Vpoint(wptmp);
            wptmp.x = b.x2;
            wptmp.y = b.y2;
            vp2 = Wpoint2Vpoint(wptmp);
        } else {
            vp1.x = b.x1;
            vp1.y = b.y1;
            vp2.x = b.x2;
            vp2.y = b.y2;
        }

        activate_bbox(BBOX_TYPE_TEMP, TRUE);
        reset_bbox(BBOX_TYPE_TEMP);
        
        RotationAngle=-b.rot;//changed to '-' starting with 0.2.4

        setcolor(b.fillcolor);
        setalpha(b.fillalpha);
        setpattern(b.fillpattern);
        FillRect_(vp1, vp2);
        
        setcolor(b.color);
        setalpha(b.alpha);
        setlinewidth(b.linew);
        setlinestyle(b.lines);
        setpattern(1);
        DrawRect(vp1, vp2);
        
        b.bb = get_bbox(BBOX_TYPE_TEMP);
        set_graph_box(i, &b);

        expand_local_bounding_box(b.bb,&global_bb_ll,&global_bb_ur);

        if (print_target==PRINT_TARGET_SCREEN && b.filltype==1)//image
        {
        #ifndef USE_FLOATING_POINT_DATA
        QPoint *p = new QPoint[4];
        #else
        QPointF *p = new QPointF[4];
        #endif
        for (i = 0; i < 4; i++)
        {
        #ifndef USE_FLOATING_POINT_DATA
        vp1.x=b.x1;
        vp1.y=b.y1;
        p[0] = VPoint2XPoint(vp1);
        vp1.x=b.x1;
        vp1.y=b.y2;
        p[1] = VPoint2XPoint(vp1);
        vp1.x=b.x2;
        vp1.y=b.y2;
        p[2] = VPoint2XPoint(vp1);
        vp1.x=b.x2;
        vp1.y=b.y1;
        p[3] = VPoint2XPoint(vp1);
        #else
        vp1.x=b.x1;
        vp1.y=b.y1;
        p[0] = VPoint2XPointF(vp1);
        vp1.x=b.x1;
        vp1.y=b.y2;
        p[1] = VPoint2XPointF(vp1);
        vp1.x=b.x2;
        vp1.y=b.y2;
        p[2] = VPoint2XPointF(vp1);
        vp1.x=b.x2;
        vp1.y=b.y1;
        p[3] = VPoint2XPointF(vp1);
        #endif
        }
        if (RotationAngle!=0)
        {
        int cx=abs(p[2].x()+p[0].x())/2,cy=abs(p[2].y()+p[0].y())/2;
        QPoint cpoint(cx,cy);
        set_Rotation_Matrix(cx,cy,RotationAngle);
            for (int i=0;i<4;i++)
            {
            p[i]-=cpoint;
            }
        }
        //qDebug() << "Center=" << abs(p[2].x()+p[0].x())/2 << "|" << abs(p[2].y()+p[0].y())/2;
        QColor def_col;
        RGB * rgbColor=get_rgb(b.color);
        def_col.setRgb(rgbColor->red,rgbColor->green,rgbColor->blue,b.alpha);
        //QRect box_rect(MIN2(b.x1,b.x2),MIN2(b.y1,b.y2),abs(b.x2-b.x1),abs(b.y2-b.y1));
        QRect box_rect(MIN2(p[2].x(),p[0].x()),MIN2(p[2].y(),p[0].y()),abs(p[2].x()-p[0].x()),abs(p[2].y()-p[0].y()));
        GeneralPainter->setClipRect(box_rect);
        GeneralPainter->setClipping(true);
        //qDebug() << "Box: drawRect=" << box_rect;
        drawImageOnPainter(GeneralPainter,b.fillimage,box_rect,def_col);
        if (RotationAngle!=0)
        reset_Transformation_Matrix();
        delete[] p;
        GeneralPainter->setClipping(false);
        }

        setclipping(TRUE);
        RotationAngle=0;
    }
}

/* draw annotative ellipses */
void draw_ellipse(int gno, int i)
{
    WPoint wptmp;
    VPoint vp1, vp2;
    ellipsetype b;

    b = ellip[i];

    if (gno != -2) {
        if (b.loctype == COORD_WORLD && b.gno != gno) {
            return;
        }
        if (b.loctype == COORD_VIEW && gno != -1) {
            return;
        }
    }
    if (b.active) {
        setclipping(FALSE);

        if (b.loctype == COORD_WORLD) {
            wptmp.x = b.x1;
            wptmp.y = b.y1;
            vp1 = Wpoint2Vpoint(wptmp);
            wptmp.x = b.x2;
            wptmp.y = b.y2;
            vp2 = Wpoint2Vpoint(wptmp);
        } else {
            vp1.x = b.x1;
            vp1.y = b.y1;
            vp2.x = b.x2;
            vp2.y = b.y2;
        }

        activate_bbox(BBOX_TYPE_TEMP, TRUE);
        reset_bbox(BBOX_TYPE_TEMP);
        
        RotationAngle=-b.rot;//changed to '-' starting with 0.2.4

        setcolor(b.fillcolor);
        setalpha(b.fillalpha);
        setpattern(b.fillpattern);
        DrawFilledEllipse(vp1, vp2);
        
        setcolor(b.color);
        setalpha(b.alpha);
        setlinewidth(b.linew);
        setlinestyle(b.lines);
        setpattern(1);
        DrawEllipse(vp1, vp2);
        
        b.bb = get_bbox(BBOX_TYPE_TEMP);
        set_graph_ellipse(i, &b);

        expand_local_bounding_box(b.bb,&global_bb_ll,&global_bb_ur);

        if (print_target==PRINT_TARGET_SCREEN && b.filltype==1)//image
        {
        #ifndef USE_FLOATING_POINT_DATA
        QPoint *p = new QPoint[4];
        #else
        QPointF *p = new QPointF[4];
        #endif
        for (i = 0; i < 4; i++)
        {
        #ifndef USE_FLOATING_POINT_DATA
        vp1.x=b.x1;
        vp1.y=b.y1;
        p[0] = VPoint2XPoint(vp1);
        vp1.x=b.x1;
        vp1.y=b.y2;
        p[1] = VPoint2XPoint(vp1);
        vp1.x=b.x2;
        vp1.y=b.y2;
        p[2] = VPoint2XPoint(vp1);
        vp1.x=b.x2;
        vp1.y=b.y1;
        p[3] = VPoint2XPoint(vp1);
        #else
        vp1.x=b.x1;
        vp1.y=b.y1;
        p[0] = VPoint2XPointF(vp1);
        vp1.x=b.x1;
        vp1.y=b.y2;
        p[1] = VPoint2XPointF(vp1);
        vp1.x=b.x2;
        vp1.y=b.y2;
        p[2] = VPoint2XPointF(vp1);
        vp1.x=b.x2;
        vp1.y=b.y1;
        p[3] = VPoint2XPointF(vp1);
        #endif
        }
        if (RotationAngle!=0)
        {
        int cx=abs(p[2].x()+p[0].x())/2,cy=abs(p[2].y()+p[0].y())/2;
        QPoint cpoint(cx,cy);
        set_Rotation_Matrix(cx,cy,RotationAngle);
            for (int i=0;i<4;i++)
            {
            p[i]-=cpoint;
            }
        }
        QColor def_col;
        RGB * rgbColor=get_rgb(b.color);
        def_col.setRgb(rgbColor->red,rgbColor->green,rgbColor->blue,b.alpha);
        //QRect box_rect(MIN2(b.x1,b.x2),MIN2(b.y1,b.y2),abs(b.x2-b.x1),abs(b.y2-b.y1));
        QRect box_rect(MIN2(p[2].x(),p[0].x()),MIN2(p[2].y(),p[0].y()),abs(p[2].x()-p[0].x()),abs(p[2].y()-p[0].y()));
        QPainterPath pat;
        pat.addEllipse(box_rect);
        GeneralPainter->setClipPath(pat);
        GeneralPainter->setClipping(true);
        //qDebug() << "Ellipse: drawRect=" << box_rect;
        drawImageOnPainter(GeneralPainter,b.fillimage,box_rect,def_col);
        if (RotationAngle!=0)
        reset_Transformation_Matrix();
        delete[] p;
        GeneralPainter->setClipping(false);
        }

        setclipping(TRUE);
        RotationAngle=0;
    }
}

/*
 * draw annotative lines
 */
void draw_line(int gno, int i)
{
    linetype l;
    WPoint wptmp;
    VPoint vp1, vp2;

    get_graph_line(i, &l);
    if (gno != -2) {
        if (l.loctype == COORD_WORLD && l.gno != gno) {
            return;
        }
        if (l.loctype == COORD_VIEW && gno != -1) {
            return;
        }
    }
    if (l.active) {
        setclipping(FALSE);
        
        if (l.loctype == COORD_WORLD) {
            wptmp.x = l.x1;
            wptmp.y = l.y1;
            vp1 = Wpoint2Vpoint(wptmp);
            wptmp.x = l.x2;
            wptmp.y = l.y2;
            vp2 = Wpoint2Vpoint(wptmp);
        } else {
            vp1.x = l.x1;
            vp1.y = l.y1;
            vp2.x = l.x2;
            vp2.y = l.y2;
        }

        activate_bbox(BBOX_TYPE_TEMP, TRUE);
        reset_bbox(BBOX_TYPE_TEMP);
        
        setcolor(l.color);
        setalpha(l.alpha);
        setlinewidth(l.linew);
        setlinestyle(l.lines);
        DrawLine(vp1, vp2);

        switch (l.arrow_end) {
        case 0:
            break;
        case 1:
            draw_arrowhead(vp2, vp1, &l.arrow);
            break;
        case 2:
            draw_arrowhead(vp1, vp2, &l.arrow);
            break;
        case 3:
            draw_arrowhead(vp2, vp1, &l.arrow);
            draw_arrowhead(vp1, vp2, &l.arrow);
            break;
        }

        l.bb = get_bbox(BBOX_TYPE_TEMP);
        set_graph_line(i, &l);

        expand_local_bounding_box(l.bb,&global_bb_ll,&global_bb_ur);

        setclipping(TRUE);
    }
}

/*
 * draw arrow head
 */
void draw_arrowhead(VPoint vp1, VPoint vp2, const Arrow *arrowp)
{
    double L, l, d, vlength;
    VVector vnorm;
    VPoint vpc, vpl, vpr, vps[4];
    int lines;
    int fg;
    
    vlength = hypot((vp2.x - vp1.x), (vp2.y - vp1.y));
    if (vlength == 0.0) {
        return;
    }

    vnorm.x = (vp2.x - vp1.x)/vlength;
    vnorm.y = (vp2.y - vp1.y)/vlength;
    
    L = 0.01*arrowp->length;
    d = L*arrowp->dL_ff;
    l = L*arrowp->lL_ff;

    vpc.x = vp2.x - L*vnorm.x;
    vpc.y = vp2.y - L*vnorm.y;
    vpl.x = vpc.x + 0.5*d*vnorm.y;
    vpl.y = vpc.y - 0.5*d*vnorm.x;
    vpr.x = vpc.x - 0.5*d*vnorm.y;
    vpr.y = vpc.y + 0.5*d*vnorm.x;
    vpc.x += l*vnorm.x;
    vpc.y += l*vnorm.y;
    
    vps[0] = vpl;
    vps[1] = vp2;
    vps[2] = vpr;
    vps[3] = vpc;
    
    lines = getlinestyle();
    setlinestyle(1);
    
    switch (arrowp->type) {
    case 0:
        DrawPolyline(vps, 3, POLYLINE_OPEN);
        break;
    case 1:
        setpattern(1);
        DrawPolygon(vps, 4);
        DrawPolyline(vps, 4, POLYLINE_CLOSED);
        break;
    case 2:
        fg = getcolor();
        setcolor(getbgcolor());
        setalpha(getbgalpha());
        setpattern(1);
        DrawPolygon(vps, 4);
        setcolor(fg);
        setalpha(getalpha());
        DrawPolyline(vps, 4, POLYLINE_CLOSED);
        break;
    default:
        errmsg(QObject::tr("Internal error in draw_arrowhead()").toLocal8Bit().constData());
        break;
    }

    setlinestyle(lines);
    
    return;
}

void draw_region(int r)
{
    int i;
    double vshift = 0.05;
    double xshift = 0.0, yshift = 0.0;
    
    region *this_one;

    int rgndouble=0;
    Arrow arrow;
    
    WPoint wptmp, wp1, wp2, wp3, wp4;
    VPoint vps[4], *vpstmp;

    set_default_arrow(&arrow);
    
    this_one=&rg[r];

    //cout << "Region " << r << " Punkte=" << this_one->n << endl;
    
    switch (this_one->type) {
    case REGION_POLYI:
    case REGION_POLYO:

        if (this_one->x != NULL && this_one->y != NULL && this_one->n >= 2) {
            vpstmp = (VPoint*)xmalloc (this_one->n*sizeof(VPoint));
            if (vpstmp == NULL) {
                errmsg(QObject::tr("xmalloc error in draw_region()").toLocal8Bit().constData());
                return;
            } else {
                for (i = 0; i < this_one->n; i++) {
                    wptmp.x = this_one->x[i];
                    wptmp.y = this_one->y[i];
                    vpstmp[i] = Wpoint2Vpoint(wptmp);
                }
                DrawPolyline(vpstmp, this_one->n, POLYLINE_CLOSED);
                xfree(vpstmp);
            }
        }
        return;
    case REGION_ABOVE:
        xshift = 0.0;
        yshift = vshift;
        break;
    case REGION_BELOW:
        xshift = 0.0;
        yshift = -vshift;
        break;
    case REGION_TOLEFT:
        xshift = -vshift;
        yshift = 0.0;
        break;
    case REGION_TORIGHT:
        xshift = vshift;
        yshift = 0.0;
        break;
    case REGION_HORIZI:
    case REGION_HORIZO:
        wp1.x=this_one->x1;
        wp1.y=this_one->y1;
        wp2.x=this_one->x1;
        wp2.y=this_one->y2;
        wp3.x=this_one->x2;
        wp3.y=this_one->y1;
        wp4.x=this_one->x2;
        wp4.y=this_one->y2;
        rgndouble=1;
        break;
    case REGION_VERTI:
    case REGION_VERTO:
        wp1.x=this_one->x1;
        wp1.y=this_one->y1;
        wp2.x=this_one->x2;
        wp2.y=this_one->y1;
        wp3.x=this_one->x1;
        wp3.y=this_one->y2;
        wp4.x=this_one->x2;
        wp4.y=this_one->y2;
        rgndouble=1;
        break;
    default:
        errmsg(QObject::tr("Internal error in draw_region").toLocal8Bit().constData());
        return;
    }
    
    if(!rgndouble) {
        wptmp.x = this_one->x1;
        wptmp.y = this_one->y1;
        vps[1] = Wpoint2Vpoint(wptmp);
        wptmp.x = this_one->x2;
        wptmp.y = this_one->y2;
        vps[2] = Wpoint2Vpoint(wptmp);
        vps[0].x = vps[1].x + xshift;
        vps[0].y = vps[1].y + yshift;
        vps[3].x = vps[2].x + xshift;
        vps[3].y = vps[2].y + yshift;
        DrawPolyline(vps, 4, POLYLINE_OPEN);
        draw_arrowhead(vps[1], vps[0], &arrow);
        draw_arrowhead(vps[2], vps[3], &arrow);
    } else {
        vps[0] = Wpoint2Vpoint(wp1);
        vps[1] = Wpoint2Vpoint(wp2);
        DrawLine(vps[0], vps[1]);
        vps[0] = Wpoint2Vpoint(wp3);
        vps[1] = Wpoint2Vpoint(wp4);
        DrawLine(vps[0], vps[1]);
        wp1.x=(wp1.x+wp2.x)/2;
        wp1.y=(wp1.y+wp2.y)/2;
        wp3.x=(wp3.x+wp4.x)/2;
        wp3.y=(wp3.y+wp4.y)/2;
        vps[0] = Wpoint2Vpoint(wp1);
        vps[1] = Wpoint2Vpoint(wp3);
        DrawLine(vps[0], vps[1]);
        draw_arrowhead(vps[0], vps[1], &arrow);
    }
}

/* ---------------------- legends ---------------------- */


/*
 * draw the legend
 */
void dolegend(int gno)
{
    int i;
    int draw_flag;
    double maxsymsize;
    double ldist, sdist, yskip;
    double old_vp_x,old_vp_y;
    
    WPoint wptmp;
    VPoint vp, vp2;
    
    view v;
    legend l;
    plotarr p;

    get_graph_legend(gno, &l);
    if (l.active == FALSE) {
        return;
    }
    
    maxsymsize = 0.0;
    draw_flag = FALSE;
    for (i = 0; i < number_of_sets(gno); i++) {
        if (is_set_drawable(gno, i)) {
            get_graph_plotarr(gno, i, &p);
            if (p.lstr[0] != '\0') {
                draw_flag = TRUE;
            }
            if (p.symsize > maxsymsize) {
                maxsymsize = p.symsize;
            }
        }
    }
    
    if (draw_flag == FALSE) {
        l.bb.xv1 = l.bb.xv2 = l.bb.yv1 = l.bb.yv2 = 0.0;
        /* The bb update shouldn't change the dirtystate flag */
        lock_dirtystate(TRUE);
        set_graph_legend(gno, &l);
        lock_dirtystate(FALSE);
        return;
    }

    setclipping(FALSE);
    
    if (l.loctype == COORD_WORLD) {
        wptmp.x = l.legx;
        wptmp.y = l.legy;
        old_vp_x=wptmp.x;
        old_vp_y=wptmp.y;
        vp = Wpoint2Vpoint(wptmp);
    } else {
        vp.x = l.legx;
        vp.y = l.legy;
        old_vp_x=vp.x;
        old_vp_y=vp.y;
    }
    
    ldist = 0.01*l.len;
    sdist = 0.01*(l.hgap + maxsymsize);
    
    yskip = 0.01*l.vgap;
    
    activate_bbox(BBOX_TYPE_TEMP, TRUE);
    reset_bbox(BBOX_TYPE_TEMP);
    update_bbox(BBOX_TYPE_TEMP, vp);

//qDebug() << "Draw mode = FALSE" << endl;
    set_draw_mode(FALSE);
    putlegends(gno, vp, ldist, sdist, yskip);
    v = get_bbox(BBOX_TYPE_TEMP);
    
    vp2.x = vp.x + (v.xv2 - v.xv1) + 2*0.01*l.hgap;
    vp2.y = vp.y - (v.yv2 - v.yv1) - 2*0.01*l.vgap;

    l.bb.xv1 = vp.x;
    l.bb.yv1 = vp2.y;
    l.bb.xv2 = vp2.x;
    l.bb.yv2 = vp.y;

    if (l.autoattach!=G_LB_ATTACH_NONE)
    {
    double dx,dy;
    view tov;
    dx=0;
    dy=0;
    get_graph_viewport(l.autoattachG,&tov);
        //horizontal
        if (l.autoattach & G_LB_ATTACH_LEFT)
        {
        dx=tov.xv1-l.bb.xv1;
        }
        else if (l.autoattach & G_LB_ATTACH_RIGHT)
        {
        dx=tov.xv2-fabs(l.bb.xv2-l.bb.xv1)-l.bb.xv1;
        }
        //vertical
        if (l.autoattach & G_LB_ATTACH_TOP)
        {
        dy=tov.yv2-l.bb.yv2;
        }
        else if (l.autoattach & G_LB_ATTACH_BOTTOM)
        {
        dy=tov.yv1+fabs(l.bb.yv2-l.bb.yv1)-l.bb.yv2;
        }
    vp.x+=dx;
    vp.y+=dy;
    vp2.x+=dx;
    vp2.y+=dy;
    v.xv1+=dx;
    v.yv2+=dy;
    l.xshift=dx;
    l.yshift=dy;
    }

    /* The bb update shouldn't change the dirtystate flag */
    lock_dirtystate(TRUE);
    set_graph_legend(gno, &l);
    lock_dirtystate(FALSE);
    
    set_draw_mode(TRUE);
    
    setpen(l.boxfillpen);
    FillRect_(vp, vp2);

    if (l.boxlines != 0 && l.boxpen.pattern != 0) {
        setpen(l.boxpen);
        setlinewidth(l.boxlinew);
        setlinestyle(l.boxlines);
        DrawRect(vp, vp2);
    }
    
    /* correction */
    vp.x += (vp.x - v.xv1) + 0.01*l.hgap;
    vp.y += (vp.y - v.yv2) - 0.01*l.vgap;

    reset_bbox(BBOX_TYPE_TEMP);
    update_bbox(BBOX_TYPE_TEMP, vp);
//qDebug() << "After correction" << endl;
    putlegends(gno, vp, ldist, sdist, yskip);
    l.legx=old_vp_x;
    l.legy=old_vp_y;
    lock_dirtystate(TRUE);
    set_graph_legend(gno, &l);
    lock_dirtystate(FALSE);
}

void putlegends(int gno, VPoint vp, double ldist, double sdist, double yskip)
{
    int i, setno;
    VPoint vp2, vpstr;
    VPoint tvp1,tvp2;
    VPoint tvpa,tvpb;
    VPoint vptmp;
    VPoint * fill_box=new VPoint[4];
    double tmp_w;
const double errorbarlength=1.35;
    plotarr p;
    legend l;
    int x_box,y_box;
    
    vp2.y = vp.y;
    vp2.x = vp.x + ldist;
    vpstr.y = vp.y;
    vpstr.x = vp2.x + sdist;
    
    get_graph_legend(gno, &l);

//qDebug() << "Start Legend bb_temp: x=" << get_bbox(BBOX_TYPE_TEMP).xv1 << "-" << get_bbox(BBOX_TYPE_TEMP).xv2 << " y=" << get_bbox(BBOX_TYPE_TEMP).yv1 << "-" << get_bbox(BBOX_TYPE_TEMP).yv2;

    nr_of_legend_entries[gno]=0;
    legend_entry_border[gno][nr_of_legend_entries[gno]]=get_bbox(BBOX_TYPE_TEMP).yv1;
    legend_entry_x_stop[gno]=legend_entry_x_start[gno]=get_bbox(BBOX_TYPE_TEMP).xv1;

    for (i = 0; i < number_of_sets(gno); i++) {
        if (l.invert == FALSE) {
            setno = i;
        } else {
            setno = number_of_sets(gno) - i - 1;
        }
        if (is_set_drawable(gno, setno)) {
            get_graph_plotarr(gno, setno, &p);
            //if (p.lstr == NULL || p.lstr[0] == '\0') {
            if (p.lstr[0] == '\0') {
                continue;
            }

            legend_entry_setno[gno][nr_of_legend_entries[gno]]=i;

            setcharsize(l.charsize);
            setfont(l.font);
            setcolor(l.color);
            setalpha(l.alpha);
            switch (l.align)
            {
            case JUST_LEFT:
            currentStringAlignment=Qt::AlignLeft;
            break;
            case JUST_RIGHT:
            currentStringAlignment=Qt::AlignRight;
            break;
            case JUST_CENTER:
            currentStringAlignment=Qt::AlignCenter;
            break;
            }
            WriteString(vpstr, 0, JUST_LEFT|JUST_TOP, p.lstr);
            currentStringAlignment=Qt::AlignLeft;
            legend_entry_x_start[gno]=vpstr.x;
//qDebug() << "Entry=" << nr_of_legend_entries[gno] << " x=" << vpstr.x;
            vp.y = (vpstr.y + get_bbox(BBOX_TYPE_TEMP).yv1)/2;
            vp2.y = vp.y;
            vpstr.y = get_bbox(BBOX_TYPE_TEMP).yv1 - yskip;

            legend_entry_x_stop[gno]=get_bbox(BBOX_TYPE_TEMP).xv2;
            legend_entry_border[gno][nr_of_legend_entries[gno]+1]=get_bbox(BBOX_TYPE_TEMP).yv1;
            nr_of_legend_entries[gno]++;
//qDebug() << i << " In Legend bb_temp: x=" << get_bbox(BBOX_TYPE_TEMP).xv1 << "-" << get_bbox(BBOX_TYPE_TEMP).xv2 << " y=" << get_bbox(BBOX_TYPE_TEMP).yv1 << "-" << get_bbox(BBOX_TYPE_TEMP).yv2 << " Label=" << p.lstr;
            setfont(p.charfont);
            
            vptmp.x = (vp.x + vp2.x)/2;
            vptmp.y = vp.y;
            if (p.errbar.connect_bars==CONNECT_ERRBARS_FILL_Y || p.errbar.connect_bars==CONNECT_ERRBARS_FILL_XY)
            y_box=1;
            else
            y_box=0;
            if (p.errbar.connect_bars==CONNECT_ERRBARS_FILL_X || p.errbar.connect_bars==CONNECT_ERRBARS_FILL_XY)
            x_box=1;
            else
            x_box=0;

            /*Start drawing errorbars in legend*/
            if (p.errbar.show_in_legend && has_errorbar(gno, setno, 0))/*we have to show x-error-bars in the legend*/
            {
                setpen(p.errbar.pen);
                tmp_w=p.symsize*0.02*0.5;
                if (l.len != 0 && p.lines != 0 && p.linet != 0)/*a line exists --> draw everything two times*/
                {
                /*left*/
                setlinewidth(p.errbar.linew);
                setlinestyle(p.errbar.lines);
                tvp1.x=vp.x-tmp_w*errorbarlength;
                tvp2.x=vp.x-tmp_w*errorbarlength;
                tvp1.y=vp.y+tmp_w;
                tvp2.y=vp.y-tmp_w;
                    if (x_box==0)
                    if ((p.errbar.ptype==PLACEMENT_OPPOSITE || p.errbar.ptype==PLACEMENT_BOTH) && (p.errbar.connect_bars==CONNECT_ERRBARS_NONE || p.errbar.connect_bars==CONNECT_ERRBARS_Y_ONLY || p.errbar.connect_bars==CONNECT_ERRBARS_FILL_Y))
                    DrawLine(tvp1, tvp2);
                tvp1.x=vp.x+tmp_w*errorbarlength;
                tvp2.x=vp.x+tmp_w*errorbarlength;
                    if (x_box==0)
                    if ((p.errbar.ptype==PLACEMENT_NORMAL || p.errbar.ptype==PLACEMENT_BOTH) && (p.errbar.connect_bars==CONNECT_ERRBARS_NONE || p.errbar.connect_bars==CONNECT_ERRBARS_Y_ONLY || p.errbar.connect_bars==CONNECT_ERRBARS_FILL_Y))
                    DrawLine(tvp1, tvp2);
                tvp2.y=tvp1.y=vp.y;
                tvp1.x=vp.x-tmp_w*errorbarlength;
                tvp2.x=vp.x+tmp_w*errorbarlength;

                fill_box[0].x=vp.x-tmp_w*errorbarlength;
                fill_box[0].y=vp.y+0.5*tmp_w*errorbarlength;
                fill_box[1].x=vp2.x+tmp_w*errorbarlength;
                fill_box[1].y=vp2.y+0.5*tmp_w*errorbarlength;
                fill_box[2].x=vp2.x+tmp_w*errorbarlength;
                fill_box[2].y=vp2.y-0.5*tmp_w*errorbarlength;
                fill_box[3].x=vp.x-tmp_w*errorbarlength;
                fill_box[3].y=vp.y-0.5*tmp_w*errorbarlength;

                setlinewidth(p.errbar.riser_linew);
                setlinestyle(p.errbar.riser_lines);
                    if (p.errbar.ptype==PLACEMENT_BOTH)
                    DrawLine(tvp1, tvp2);
                    else if (p.errbar.ptype==PLACEMENT_NORMAL)
                    DrawLine(vp, tvp2);
                    else
                    DrawLine(tvp1, vp);
                /*right*/
                setlinewidth(p.errbar.linew);
                setlinestyle(p.errbar.lines);
                tvp1.x=vp2.x-tmp_w*errorbarlength;
                tvp2.x=vp2.x-tmp_w*errorbarlength;
                tvp1.y=vp2.y+tmp_w;
                tvp2.y=vp2.y-tmp_w;
                    if (x_box==0)
                    if ((p.errbar.ptype==PLACEMENT_OPPOSITE || p.errbar.ptype==PLACEMENT_BOTH) && (p.errbar.connect_bars==CONNECT_ERRBARS_NONE || p.errbar.connect_bars==CONNECT_ERRBARS_Y_ONLY || p.errbar.connect_bars==CONNECT_ERRBARS_FILL_Y))
                    DrawLine(tvp1, tvp2);
                tvp1.x=vp2.x+tmp_w*errorbarlength;
                tvp2.x=vp2.x+tmp_w*errorbarlength;
                    if (x_box==0)
                    if ((p.errbar.ptype==PLACEMENT_NORMAL || p.errbar.ptype==PLACEMENT_BOTH) && (p.errbar.connect_bars==CONNECT_ERRBARS_NONE || p.errbar.connect_bars==CONNECT_ERRBARS_Y_ONLY || p.errbar.connect_bars==CONNECT_ERRBARS_FILL_Y))
                    DrawLine(tvp1, tvp2);
                tvp2.y=tvp1.y=vp2.y;
                tvp1.x=vp2.x-tmp_w*errorbarlength;
                tvp2.x=vp2.x+tmp_w*errorbarlength;
                setlinewidth(p.errbar.riser_linew);
                setlinestyle(p.errbar.riser_lines);
                    if (p.errbar.ptype==PLACEMENT_BOTH)
                    DrawLine(tvp1, tvp2);
                    else if (p.errbar.ptype==PLACEMENT_NORMAL)
                    DrawLine(vp2, tvp2);
                    else
                    DrawLine(tvp1, vp2);
                        /*if (p.errbar.connect_bars==CONNECT_ERRBARS_X_ONLY || p.errbar.connect_bars==CONNECT_ERRBARS_XY)
                        {
                        setlinewidth(p.errbar.linew);
                        setlinestyle(p.errbar.lines);
                        if (p.errbar.ptype==PLACEMENT_NORMAL || p.errbar.ptype==PLACEMENT_BOTH)
                        DrawLine(tvpa, tvpb);
                        tvpa.y-=2*tmp_w*errorbarlength;
                        tvpb.y-=2*tmp_w*errorbarlength;
                        if (p.errbar.ptype==PLACEMENT_OPPOSITE || p.errbar.ptype==PLACEMENT_BOTH)
                        DrawLine(tvpa, tvpb);
                        }*/
                }
                else/*no line --> draw everything just once*/
                {
                setlinewidth(p.errbar.linew);
                setlinestyle(p.errbar.lines);
                tvp1.x=vptmp.x-tmp_w*errorbarlength;
                tvp2.x=vptmp.x-tmp_w*errorbarlength;
                tvp1.y=vptmp.y+tmp_w;
                tvp2.y=vptmp.y-tmp_w;
                    if (x_box==0)
                    if (p.errbar.ptype==PLACEMENT_OPPOSITE || p.errbar.ptype==PLACEMENT_BOTH)
                    DrawLine(tvp1, tvp2);
                tvp1.x=vptmp.x+tmp_w*errorbarlength;
                tvp2.x=vptmp.x+tmp_w*errorbarlength;
                    if (x_box==0)
                    if (p.errbar.ptype==PLACEMENT_NORMAL || p.errbar.ptype==PLACEMENT_BOTH)
                    DrawLine(tvp1, tvp2);
                tvp2.y=tvp1.y=vptmp.y;
                tvp1.x=vptmp.x-tmp_w*errorbarlength;
                tvp2.x=vptmp.x+tmp_w*errorbarlength;
                setlinewidth(p.errbar.riser_linew);
                setlinestyle(p.errbar.riser_lines);
                    if (p.errbar.ptype==PLACEMENT_BOTH)
                    DrawLine(tvp1, tvp2);
                    else if (p.errbar.ptype==PLACEMENT_NORMAL)
                    DrawLine(vptmp, tvp2);
                    else
                    DrawLine(tvp1, vptmp);

                fill_box[0].x=vptmp.x-tmp_w*errorbarlength;
                fill_box[0].y=vptmp.y+0.5*tmp_w*errorbarlength;
                fill_box[1].x=vptmp.x+tmp_w*errorbarlength;
                fill_box[1].y=vptmp.y+0.5*tmp_w*errorbarlength;
                fill_box[2].x=vptmp.x+tmp_w*errorbarlength;
                fill_box[2].y=vptmp.y-0.5*tmp_w*errorbarlength;
                fill_box[3].x=vptmp.x-tmp_w*errorbarlength;
                fill_box[3].y=vptmp.y-0.5*tmp_w*errorbarlength;
                }
            }
            if (x_box==1)
            {
            setpattern(p.errbar.pen.pattern);
            setalpha(p.errbar.pen.alpha);
            DrawPolygon(fill_box,4);
            setalpha(255);
            DrawPolyline(fill_box,4,POLYLINE_CLOSED);
            setalpha(p.errbar.pen.alpha);
            }
            if (p.errbar.show_in_legend && has_errorbar(gno, setno, 1))/*we have to show y-error-bars in the legend*/
            {
                setpen(p.errbar.pen);
                tmp_w=p.symsize*0.02*0.5;
                if (l.len != 0 && p.lines != 0 && p.linet != 0)/*a line exists --> draw everything two times*/
                {
                /*left*/
                setlinewidth(p.errbar.linew);
                setlinestyle(p.errbar.lines);
                tvp1.x=vp.x-tmp_w;
                tvp2.x=vp.x+tmp_w;
                tvp1.y=vp.y+tmp_w*errorbarlength;
                tvp2.y=vp.y+tmp_w*errorbarlength;
                tvpa.x=vp.x;
                tvpa.y=tvp1.y;
                    if (y_box==0)
                    if ((p.errbar.ptype==PLACEMENT_NORMAL || p.errbar.ptype==PLACEMENT_BOTH) && (p.errbar.connect_bars==CONNECT_ERRBARS_NONE || p.errbar.connect_bars==CONNECT_ERRBARS_X_ONLY || p.errbar.connect_bars==CONNECT_ERRBARS_FILL_X))
                    DrawLine(tvp1, tvp2);
                tvp1.y=vp.y-tmp_w*errorbarlength;
                tvp2.y=vp.y-tmp_w*errorbarlength;
                    if (y_box==0)
                    if ((p.errbar.ptype==PLACEMENT_OPPOSITE || p.errbar.ptype==PLACEMENT_BOTH) && (p.errbar.connect_bars==CONNECT_ERRBARS_NONE || p.errbar.connect_bars==CONNECT_ERRBARS_X_ONLY || p.errbar.connect_bars==CONNECT_ERRBARS_FILL_X))
                    DrawLine(tvp1, tvp2);
                tvp2.x=tvp1.x=vp.x;
                tvp1.y=vp.y-tmp_w*errorbarlength;
                tvp2.y=vp.y+tmp_w*errorbarlength;
                setlinewidth(p.errbar.riser_linew);
                setlinestyle(p.errbar.riser_lines);
                    if (p.errbar.ptype==PLACEMENT_BOTH)
                    DrawLine(tvp1, tvp2);
                    else if (p.errbar.ptype==PLACEMENT_NORMAL)
                    DrawLine(vp, tvp2);
                    else//PLACEMENT_OPPOSITE
                    DrawLine(tvp1, vp);
                fill_box[0].x=vp.x-0.5*tmp_w*errorbarlength;
                fill_box[0].y=vp.y+tmp_w*errorbarlength;
                fill_box[1].x=vp2.x+0.5*tmp_w*errorbarlength;
                fill_box[1].y=vp2.y+tmp_w*errorbarlength;
                fill_box[2].x=vp2.x+0.5*tmp_w*errorbarlength;
                fill_box[2].y=vp2.y-tmp_w*errorbarlength;
                fill_box[3].x=vp.x-0.5*tmp_w*errorbarlength;
                fill_box[3].y=vp.y-tmp_w*errorbarlength;

                /*right*/
                setlinewidth(p.errbar.linew);
                setlinestyle(p.errbar.lines);
                tvp1.x=vp2.x-tmp_w;
                tvp2.x=vp2.x+tmp_w;
                tvp1.y=vp2.y+tmp_w*errorbarlength;
                tvp2.y=vp2.y+tmp_w*errorbarlength;
                tvpb.x=vp2.x;
                tvpb.y=tvp1.y;
                    if (y_box==0)
                    if ((p.errbar.ptype==PLACEMENT_NORMAL || p.errbar.ptype==PLACEMENT_BOTH) && (p.errbar.connect_bars==CONNECT_ERRBARS_NONE || p.errbar.connect_bars==CONNECT_ERRBARS_X_ONLY || p.errbar.connect_bars==CONNECT_ERRBARS_FILL_X))
                    DrawLine(tvp1, tvp2);
                tvp1.y=vp2.y-tmp_w*errorbarlength;
                tvp2.y=vp2.y-tmp_w*errorbarlength;
                    if (y_box==0)
                    if ((p.errbar.ptype==PLACEMENT_OPPOSITE || p.errbar.ptype==PLACEMENT_BOTH) && (p.errbar.connect_bars==CONNECT_ERRBARS_NONE || p.errbar.connect_bars==CONNECT_ERRBARS_X_ONLY || p.errbar.connect_bars==CONNECT_ERRBARS_FILL_X))
                    DrawLine(tvp1, tvp2);
                tvp2.x=tvp1.x=vp2.x;
                tvp1.y=vp2.y-tmp_w*errorbarlength;
                tvp2.y=vp2.y+tmp_w*errorbarlength;
                setlinewidth(p.errbar.riser_linew);
                setlinestyle(p.errbar.riser_lines);
                    if (p.errbar.ptype==PLACEMENT_BOTH)
                    DrawLine(tvp1, tvp2);
                    else if (p.errbar.ptype==PLACEMENT_NORMAL)
                    DrawLine(vp2, tvp2);
                    else//PLACEMENT_OPPOSITE
                    DrawLine(tvp1, vp2);
                        if (p.errbar.connect_bars==CONNECT_ERRBARS_Y_ONLY || p.errbar.connect_bars==CONNECT_ERRBARS_XY)
                        {
                        setlinewidth(p.errbar.linew);
                        setlinestyle(p.errbar.lines);
                        if (p.errbar.ptype==PLACEMENT_NORMAL || p.errbar.ptype==PLACEMENT_BOTH)
                        DrawLine(tvpa, tvpb);
                        tvpa.y-=2*tmp_w*errorbarlength;
                        tvpb.y-=2*tmp_w*errorbarlength;
                        if (p.errbar.ptype==PLACEMENT_OPPOSITE || p.errbar.ptype==PLACEMENT_BOTH)
                        DrawLine(tvpa, tvpb);
                        }
                }
                else/*no line --> draw everything just once*/
                {
                setlinewidth(p.errbar.linew);
                setlinestyle(p.errbar.lines);
                tvp1.x=vptmp.x-tmp_w;
                tvp2.x=vptmp.x+tmp_w;
                tvp1.y=vptmp.y+tmp_w*errorbarlength;
                tvp2.y=vptmp.y+tmp_w*errorbarlength;
                    if (y_box==0)
                    if (p.errbar.ptype==PLACEMENT_NORMAL || p.errbar.ptype==PLACEMENT_BOTH)
                    DrawLine(tvp1, tvp2);
                tvp1.y=vptmp.y-tmp_w*errorbarlength;
                tvp2.y=vptmp.y-tmp_w*errorbarlength;
                    if (y_box==0)
                    if (p.errbar.ptype==PLACEMENT_OPPOSITE || p.errbar.ptype==PLACEMENT_BOTH)
                    DrawLine(tvp1, tvp2);
                tvp2.x=tvp1.x=vptmp.x;
                tvp1.y=vptmp.y-tmp_w*errorbarlength;
                tvp2.y=vptmp.y+tmp_w*errorbarlength;
                setlinewidth(p.errbar.riser_linew);
                setlinestyle(p.errbar.riser_lines);
                    if (p.errbar.ptype==PLACEMENT_BOTH)
                    DrawLine(tvp1, tvp2);
                    else if (p.errbar.ptype==PLACEMENT_NORMAL)
                    DrawLine(vptmp, tvp2);
                    else
                    DrawLine(tvp1, vptmp);
                fill_box[0].x=vptmp.x-0.5*tmp_w*errorbarlength;
                fill_box[0].y=vptmp.y+tmp_w*errorbarlength;
                fill_box[1].x=vptmp.x+0.5*tmp_w*errorbarlength;
                fill_box[1].y=vptmp.y+tmp_w*errorbarlength;
                fill_box[2].x=vptmp.x+0.5*tmp_w*errorbarlength;
                fill_box[2].y=vptmp.y-tmp_w*errorbarlength;
                fill_box[3].x=vptmp.x-0.5*tmp_w*errorbarlength;
                fill_box[3].y=vptmp.y-tmp_w*errorbarlength;
                }
            }/*End drawing errorbars in legend*/
            if (y_box==1)
            {
            setpattern(p.errbar.pen.pattern);
            setalpha(p.errbar.pen.alpha);
            DrawPolygon(fill_box,4);
            setalpha(255);
            DrawPolyline(fill_box,4,POLYLINE_CLOSED);
            setalpha(p.errbar.pen.alpha);
            }

            if (l.len != 0 && p.lines != 0 && p.linet != 0) {
                setpen(p.linepen);
                setlinewidth(p.linew);
                setlinestyle(p.lines);
                DrawLine(vp, vp2);

                setlinewidth(p.symlinew);
                setlinestyle(p.symlines);
                if (p.type == SET_BAR   || p.type == SET_BOXPLOT ||
                        p.type == SET_BARDY || p.type == SET_BARDYDY) {
                    drawlegbarsym(vp, p.symsize, p.sympen, p.symfillpen);
                    drawlegbarsym(vp2, p.symsize, p.sympen, p.symfillpen);
                } else {
                    drawxysym(vp, p.symsize, p.sym, p.sympen, p.symfillpen, p.symchar);
                    drawxysym(vp2, p.symsize, p.sym, p.sympen, p.symfillpen, p.symchar);
                }
            } else {
                setlinewidth(p.symlinew);
                setlinestyle(p.symlines);
                if (p.type == SET_BAR   || p.type == SET_BOXPLOT ||
                        p.type == SET_BARDY || p.type == SET_BARDYDY) {
                    drawlegbarsym(vptmp, p.symsize, p.sympen, p.symfillpen);
                } else {
                    drawxysym(vptmp, p.symsize, p.sym, p.sympen, p.symfillpen, p.symchar);
                }
            }
        }
    }
    delete[] fill_box;
}

/* plot time stamp */
void draw_timestamp(void)
{
    if (timestamp.active || timestamp.path) {
        VPoint vp;
        setfont(timestamp.font);
        setcharsize(timestamp.charsize);
        setcolor(timestamp.color);
        setalpha(timestamp.alpha);
        vp.x = timestamp.x;
        vp.y = timestamp.y;

        activate_bbox(BBOX_TYPE_TEMP, TRUE);
        reset_bbox(BBOX_TYPE_TEMP);

        WriteString(vp, timestamp.rot, timestamp.just, QString::fromLocal8Bit(timestamp.s_plotstring).toLatin1().data());

        timestamp.bb = get_bbox(BBOX_TYPE_TEMP);
    }
}

void do_hardcopy_external(char * target_file,const char * target_device,float dpi,unsigned long w,unsigned long h)
{
int dev_nr=get_device_by_name(target_device);
do_hardcopy_external(target_file,dev_nr,dpi,w,h);
}

void do_hardcopy_external(char * target_file,int dev_nr,float dpi,unsigned long w,unsigned long h)
{
int sav_cur_file=curdevice;
int sav_hdevice=hdevice;
int sav_ptofile=get_ptofile();
char exname_sav[GR_MAXPATHLEN];

    strcpy(exname_sav,get_exportname());
    set_exportname(target_file);
    set_ptofile(TRUE);

Page_geometry pg,sav_pg;
sav_pg=get_page_geometry();
    pg.dpi=dpi;
    pg.height=h;
    pg.width=w;

    hdevice=dev_nr;
    select_device(hdevice);
    set_page_geometry(pg);

    do_hardcopy();

    hdevice=sav_hdevice;
    select_device(sav_cur_file);
    set_page_geometry(sav_pg);
    set_ptofile(sav_ptofile);
    set_exportname(exname_sav);
}
