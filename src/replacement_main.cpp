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

///#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#ifdef _MSC_VER
#define STDIN_FILENO 0
#include <io.h>
#if QT_VERSION >= 0x050000
#define open _open
#endif
#endif

#include <string.h>
//#ifdef HAVE_FCNTL_H
//#ifndef WINDOWS_SYSTEM
#include <fcntl.h>
//#endif

/* for globals.h */
#define MAIN

//#include "globals.h"
#ifdef _MSC_VER
#else
#include <unistd.h>
#endif

#include "utils.h"
#include "files.h"
#include "ssdata.h"

#include "graphs.h"
#include "graphutils.h"
#include "plotone.h"

#include "device.h"
#include "devlist.h"
/*#ifndef NONE_GUI
#  include "x11drv.h"
#endif*/
#include "parser.h"
#include "noxprotos.h"
#include "xprotos.h"
#include "patterns.h"
#include "draw.h"
#include "MainWindow.h"
#include "allWidgets.h"
#include "QtGui/QPainter"
#include "QtNetwork/QHostInfo"

//#include <QtWebKit>

/*#if QT_VERSION >= 0x050000
#include <QtWebKitWidgets/QWebView>
#endif*/

#ifdef WINDOWS_SYSTEM
#define O_NONBLOCK 0x0004
#endif

extern CMap_entry *cmap_table;
extern int allocated_colors;
extern QIcon ** ColorIcons;
extern QPixmap ** ColorPixmaps;
extern QString ** ColorNames;
extern MainWindow * mainWin;
extern "C" char batchfile[];
//extern char print_file[];
extern int install_cmap;
extern QBitmap * patterns[MAXPATTERNS];
extern Input_buffer *ib_tbl;
extern int ib_tblsize;
extern void register_qt_devices(void);
extern int register_haru_pdf_drv(void);
extern frmAxisProp * FormAxisProperties;
extern frmLocatorProps * FormLocatorProps;
extern frmTransform * FormPruneData;
extern frm_Preferences * Form_Preferences;
extern frmHotLinks * FormHotLinks;
extern frmExplorer * FormExplorer;
extern frmSetAppearance * FormSetAppearance;
extern frmPlotAppearance * FormPlotAppearance;
extern frmMasterRegionOperator * FormRegionMaster;
extern QList<QFont> stdFontList;

extern uniList ** plist;
extern int nplist;
extern uniList ** graph_selectors;
extern int ngraph_selectors;
extern uniList ** set_selectors;
extern int nset_selectors;
extern ColorSelector ** color_selectors;
extern int ncolor_selectors;
extern FontSelector ** font_selectors;
extern int nfont_selectors;
extern int nfonts;

extern int curdevice;
extern int number_of_opened_spreadsheets;
extern frmSpreadSheet ** FormSpreadSheets;
extern bool qtfonts_have_been_reset;
extern bool display_help_externally;
extern bool immediateUpdate;
extern bool updateRunning;
extern bool useQtFonts;
//extern QList<QFont> stdFontList;

extern int nr_of_current_linestyles;
extern int * lenghts_of_linestyle_patterns;
extern char ** current_linestyle_patterns;
extern QIcon ** LineIcons;
extern QPixmap ** LinePixmaps;
extern QVector<qreal> ** PenDashPattern;
extern LineStyleSelector ** line_style_selectors;
extern int nr_of_line_style_selectors;

extern bool hdeviceFlag;
extern int hardCopyDeviceNr;

extern void update_default_props(void);
#ifdef __cplusplus
extern "C" {
#endif
extern void prepare_strings_for_saving(void);
extern void resume_strings_after_load_or_save(void);
#ifdef __cplusplus
}
#endif
static void usage(FILE *stream, char *progname);
static void VersionInfo(void);
extern int get_Std_GraceFontID_From_QFont(QFont & font,bool current,bool & ok);
extern QFont get_Std_QFont_From_GraceFontID(int fontID,bool & ok);

#ifdef _MSC_VER
// Ugly woraround. This variable has no connection to lgamma function
// in c++ library. On the other hand, lgamma exists there and cannot be duplicated.
extern "C" int signgam;
int signgam=0;
#endif

int openPipe(char * pname,int * fd);

#if defined(DEBUG)    
extern int yydebug;
#endif

int force_external_viewer =
        #if defined WITH_XMHTML || defined WITH_LIBHELP
        FALSE;
#else
        TRUE;
#endif

/*void xdrawgraph(void)
{
mainWin->mainArea->completeRedraw();
}*/

void update_ss_editors(int gno)
{
    plotarr p;
    for (int i=0;i<number_of_opened_spreadsheets;i++)
    {
        if (FormSpreadSheets[i]!=NULL)
            if (FormSpreadSheets[i]->gno==gno || gno == ALL_GRAPHS )
            {
                get_graph_plotarr(FormSpreadSheets[i]->gno,FormSpreadSheets[i]->sno, &p);
                if (is_valid_setno(FormSpreadSheets[i]->gno,FormSpreadSheets[i]->sno) == FALSE)// || p.data.len<=0)//no valid set
                {
                    delete FormSpreadSheets[i];
                    FormSpreadSheets[i]=NULL;
                }
                else
                {
                    FormSpreadSheets[i]->init(FormSpreadSheets[i]->gno,FormSpreadSheets[i]->sno);
                }
            }
    }
}

void close_ss_editor(int gno,int setno)
{
    for (int i=0;i<number_of_opened_spreadsheets;i++)
    {
        if (FormSpreadSheets[i]!=NULL)
            if ((FormSpreadSheets[i]->gno==gno || gno == ALL_GRAPHS) && (FormSpreadSheets[i]->sno==setno || setno==ALL_SETS) )
            {
                delete FormSpreadSheets[i];
                FormSpreadSheets[i]=NULL;
            }
    }
}

void update_set_list(int gno, uniList * l)
{
    /*int i, cnt, scnt=0;
    char buf[1024];
    XmString *xms;

    XmListDeleteAllItems(l.list);
    for (i = 0; i < number_of_sets(gno); i++) {
        if (is_set_active(gno, i)) {
            scnt++;
        }
    }

    switch (l.type) {		// TODO
    case SET_SELECT_ACTIVE:
        xms = xmalloc(sizeof(XmString) * scnt);
        cnt = 0;
        break;
    case SET_SELECT_ALL:
        xms = xmalloc(sizeof(XmString) * (scnt + 1));
        xms[0] = XmStringCreateLocalized("All sets");
        cnt = 1;
        break;
    case SET_SELECT_NEXT:
        xms = xmalloc(sizeof(XmString) * (scnt + 1));
        xms[0] = XmStringCreateLocalized("New set");
        cnt = 1;
        break;
    case SET_SELECT_NEAREST:
        xms = xmalloc(sizeof(XmString) * (scnt + 1));
        xms[0] = XmStringCreateLocalized("Nearest set");
        cnt = 1;
        break;
    default:
        xms = xmalloc(sizeof(XmString) * scnt);
        cnt = 0;
        break;
    }

    for (i = 0; i < number_of_sets(gno); i++) {
        if (is_set_active(gno, i)) {
            sprintf(buf, "S%d (N=%d, %s)", i, getsetlength(gno, i), getcomment(gno, i));
            xms[cnt] = XmStringCreateLocalized(buf);
            cnt++;
        }
    }
    XmListAddItemsUnselected(l.list, xms, cnt, 0);

    // automatically highlight if only 1 selection
    if (scnt == 1) {
        XmListSelectItem(l.list, xms[cnt-1], True);
    }

    for (i = 0; i < cnt; i++) {
        XmStringFree(xms[i]);
    }
    xfree(xms);*/
    l->update_number_of_entries_preserve_selection();
}

void update_graph_selectors(void)
{
    /*int i, new_n = number_of_graphs();
    char buf[64];
    OptionItem *p;

    for (i = 0; i < ngraph_select_items; i++) {
        xfree(graph_select_items[i].label);
    }
    p = xrealloc(graph_select_items, new_n*sizeof(OptionItem));
    if (p == NULL && new_n != 0) {
        ngraph_select_items = 0;
        return;
    } else {
        graph_select_items = p;
    }

    for (i = 0; i < new_n; i++) {
        graph_select_items[i].value = i;
        sprintf(buf, "(%c) G%d (%d sets)",
            is_graph_hidden(i) ? '-':'+', i, number_of_sets(i));
        graph_select_items[i].label = copy_string(NULL, buf);
    }
    ngraph_select_items = new_n;
    */
    //cout << "updateing " << ngraph_selectors << " graph selectors" << endl;
    for (int i = 0; i < ngraph_selectors; i++)
    {
        if (graph_selectors[i]->prevent_from_autoupdate==false)
            graph_selectors[i]->update_number_of_entries_preserve_selection();
    }
}

void update_set_selectors(int gno)
{
    int i, cg;
    //SetChoiceData *sdata;

    cg = get_cg();
    update_graph_selectors();
    for (i = 0; i < nset_selectors; i++) {
        if (set_selectors[i]->partner==NULL && set_selectors[i]->prevent_from_autoupdate==false)
        {
            set_selectors[i]->set_graph_number(cg,true);
            //set_selectors[i]->update_number_of_entries_preserve_selection();
        }
        /*sdata = (SetChoiceData *) set_selectors[i]->anydata;
        if (sdata->standalone == TRUE && (gno == cg || gno == ALL_GRAPHS)) {
            UpdateSetChoice(set_selectors[i], cg);
        } else if (sdata->standalone == FALSE && sdata->gno == gno) {
            UpdateSetChoice(set_selectors[i], gno);
        }*/
    }
    for (i = 0; i < nplist; i++)
    {
        if (plist[i]->prevent_from_autoupdate==false)
            plist[i]->set_graph_number(cg,true);
    }
}

void init_color_icons(int nr_of_cols,CMap_entry * entries,int & allocated_colors,QIcon *** ColorIcons,QPixmap *** ColorPixmaps,QString *** ColorNames)
{
    QPixmap templIcon(62,22);
    QPainter templPainter;
    QPen pen1(Qt::black);
    QColor col1;
    QFont standardfont=qApp->font();
    double intensity;//=get_colorintensity(i);

    if (*ColorIcons!=NULL)
    {
        for (int i=0;i<allocated_colors;i++)
            delete (*ColorIcons)[i];
        delete[] (*ColorIcons);
    }
    if (*ColorPixmaps!=NULL)
    {
        for (int i=0;i<allocated_colors;i++)
            delete (*ColorPixmaps)[i];
        delete[] (*ColorPixmaps);
    }
    if (*ColorNames!=NULL)
    {
        for (int i=0;i<allocated_colors;i++)
            delete (*ColorNames)[i];
        delete[] (*ColorNames);
    }
    allocated_colors=nr_of_cols;
    *ColorIcons=new QIcon*[allocated_colors];
    *ColorPixmaps=new QPixmap*[allocated_colors];
    *ColorNames=new QString*[allocated_colors];

    for (int i=0;i<nr_of_cols;i++)
    {
        col1.setRgb(entries[i].rgb.red,entries[i].rgb.green,entries[i].rgb.blue);
        //intensity=get_colorintensity(i);
        intensity=RGB2YIQ(entries[i].rgb).y;
        if (intensity>0.5)
            pen1.setColor(Qt::black);
        else
            pen1.setColor(Qt::white);
        templIcon.fill(col1);
        templPainter.begin(&templIcon);
        templPainter.setPen(pen1);
        templPainter.setBrush(pen1.color());
        /*strcpy(dummy,cmap_table[i].cname);
QFontInfo fi(standardfont);
cout << "Text-Groesse zum Zeichnen(pixel)=" << standardfont.pixelSize() << endl;
cout << "Text-Groesse zum Zeichnen(points)=" << standardfont.pointSize() << " | ";
cout << "STANDARD IST 9" << endl;
cout << "FontInfo(pixel, sollte 9 sein)=" << fi.pixelSize() << endl;*/
        standardfont.setPixelSize(9);
        templPainter.setFont(standardfont);
        templPainter.drawText(4,15,QString(entries[i].cname));//4,13
        templPainter.end();
        (*ColorIcons)[i]=new QIcon(templIcon);
        (*ColorPixmaps)[i]=new QPixmap(templIcon);
        (*ColorNames)[i]=new QString(entries[i].cname);
    }
}

void update_color_selectors(void)
{
    int * real_colors=new int[4];
    int nr_of_aux_cols;
    int map_entries=get_main_color_indices(&real_colors,&nr_of_aux_cols);
    CMap_entry * local_cmap_table=new CMap_entry[map_entries];
    for (int i=0;i<map_entries;i++)
    {
        memcpy(local_cmap_table+i,cmap_table+real_colors[i],sizeof(CMap_entry));
        /*local_cmap_table[i].cname=new char[strlen(cmap_table[real_colors[i]].cname)+2];
        strcpy(local_cmap_table[i].cname,cmap_table[real_colors[i]].cname);*/
        local_cmap_table[i].cname=NULL;
        copy_string(local_cmap_table[i].cname,cmap_table[real_colors[i]].cname);
    }
    init_color_icons(map_entries,local_cmap_table,allocated_colors,&ColorIcons,&ColorPixmaps,&ColorNames);
    //Local colors are only main colors
    for (int i=0;i<ncolor_selectors;i++)
    {
        if (color_selectors[i]->prevent_from_update==false)
        {
            color_selectors[i]->updateColorIcons(map_entries,ColorPixmaps,ColorNames);
        }
    }
    for (int i=0;i<map_entries;i++)
    {
        //delete[] local_cmap_table[i].cname;
        copy_string(local_cmap_table[i].cname,NULL);
    }
    delete[] local_cmap_table;
    delete[] real_colors;
}

void update_one_line_style_selector(LineStyleSelector * selStyles,int len,QPixmap ** pix)
{
    for (int i=0;i<selStyles->cmbStyleSelect->panels->number_of_elements;i++)
    delete selStyles->LineNames[i];
delete[] selStyles->LineNames;
    selStyles->LineNames=new QString*[len];
char dummy[48];
        for (int i=0;i<len;i++)
        {
        sprintf(dummy,"%d",i);
        selStyles->LineNames[i]=new QString(dummy);
        }
selStyles->cmbStyleSelect->reinitializePanels(len,1,len,pix,selStyles->LineNames,false);
}

void update_line_style_selectors(void)
{
    for (int i=0;i<nr_of_line_style_selectors;i++)
    {
    update_one_line_style_selector(line_style_selectors[i],nr_of_current_linestyles,LinePixmaps);
    }
}

void update_font_selectors(bool appearance=false)
{
    int font_id;
    QFont refFont;
    bool ok;
    bool saved_useQtFonts=useQtFonts;
    /*
//first: remap QtFonts to match ordering of standard-fonts
    if (qtfonts_have_been_reset==true)//a file with a new qt-version has been loaded
    {//map standard-grace-fonts to match qt-fonts
        //cout << "new qt-fonts in File" << endl;
        for (int i=0;i<stdFontList.length();i++)
        {
        //look for the font (i) in the default Font list to match one of the included Grace fonts
        refFont=stdFontList.at(i);
        font_id=get_Std_GraceFontID_From_QFont(refFont,false,ok);
        map_font(font_id,i);
        }
    }
    else//old file version --> no QtFonts saved
    {//map qt-fonts to match standard-grace-fonts
        stdFontList.clear();
        useQtFonts=false;
        //cout << "only old fonts in file" << endl;
        for (int i=0;i<nfonts;i++)
        {
        font_id=get_mapped_font(i);//get real id of mapped font (ordering could have been changed)
        //cout << "id = " << i << " found_font_id=" << font_id;
        if (font_id!=BAD_FONT_ID || ok==false)
        {
        refFont=get_Std_QFont_From_GraceFontID(font_id,ok);
        stdFontList << refFont;
        //cout << " Name=" << refFont.toString().toLatin1().constData();
        }
        else
        break;//stop adding fonts to database

        //cout << endl;
        }
    }
useQtFonts=saved_useQtFonts;
*/
    //second: update font selectors
    for (int i=0;i<nfont_selectors;i++)
    {
        font_selectors[i]->updateFonts(true);
        if (appearance==true) font_selectors[i]->updateAppearance(useQtFonts);
    }
}

int replacement_main(int argc, char **argv)
{
    char *s;
    int i, j;
    int gno;
    int fd;
    world w;
    view v;
    int cur_graph;	        /* default (current) graph */
    int loadlegend = FALSE;	/* legend on and load file names */
    int cli = FALSE;            /* command line interface only */
    int remove_flag = FALSE;	/* remove file after read */
    int noprint = FALSE;	/* if gracebat, then don't print if true */
    int sigcatch = TRUE;	/* we handle signals ourselves */
    hdeviceFlag = false;
    char fd_name[GR_MAXPATHLEN];

    int wpp, hpp;
    
    /*
     * set version
     */
    reset_project_version();
    /*
     * grace home directory
     */
    if ((s = getenv("GRACE_HOME")) != NULL) {
        set_grace_home(s);
    }
    
    /* define the user's name */
    init_username();

    /* define the user's home dir */
    init_userhome();

    /* set the starting directory */
    set_workingdir(NULL);

    /*
     * print command
     */
    if ((s = getenv("GRACE_PRINT_CMD")) != NULL) {
        set_print_cmd(s);
    }

    /* if no print command defined, print to file by default */
    s = get_print_cmd();
    if (s == NULL || s[0] == '\0') {
        set_ptofile(TRUE);
    } else {
        set_ptofile(FALSE);
    }
    
    /*
     * editor
     */
    if ((s = getenv("GRACE_EDITOR")) != NULL) {
        set_editor(s);
    }
    
    /*
     * check for changed help file viewer command
     */
    if ((s = getenv("GRACE_HELPVIEWER")) != NULL) {
        set_help_viewer(s);
    }

    /* initialize plots, strings, graphs */
    set_program_defaults();

    /* initialize the nonl-fit parameters */
    initialize_nonl();

    /* initialize the parser symbol table */
    init_symtab();
    
    /* initialize the rng */
#ifndef WINDOWS_SYSTEM
    srand48(100L);
#else
    srand(100L);
#endif

    /* initialize T1lib */
    if (init_t1() != RETURN_SUCCESS) {
        errmsg(QObject::tr("--> Broken or incomplete installation - read the FAQ!").toLocal8Bit().constData());
        exit(1);
    }

    /* initialize colormap data */
    initialize_cmap();

    /*
     * if program name is gracebat* then don't initialize the X toolkit
     */
    s = mybasename(argv[0]);

    if (strstr(s, "gracebat") == s) {
        gracebat = TRUE;
    } else {
        gracebat = FALSE;
        if (strstr(s, "grace") == s) {
            cli = TRUE;
        } else {;
            /*#ifndef NONE_GUI
            cli = FALSE;
            if (initialize_gui(&argc, argv) != RETURN_SUCCESS) {
            errmsg("Failed initializing GUI, exiting");
                exit(1);
            }
#endif*/
        }
    }

    /* initialize devices */
    /*#ifndef NONE_GUI
    if (cli == TRUE || gracebat == TRUE) {
        tdevice = register_dummy_drv();
    } else {*/
    tdevice = register_x11_drv();
    /*    }
#else*/
    //tdevice = register_dummy_drv();
    //#endif
    select_device(tdevice);

    hdevice = register_pdf_drv();//the picture-pdf-driver (just a picture as a pdf)
    register_svg_drv();
    /*a QT special*/
    register_qt_devices();
    register_ps_drv();
    register_eps_drv();
    register_mif_drv();
    register_pnm_drv();
    register_mf_drv();

    /*
#ifdef HAVE_LIBPDF
    register_pdf_drv();
#endif
#ifdef HAVE_LIBJPEG
    register_jpg_drv();
#endif
#ifdef HAVE_LIBPNG
    register_png_drv();
#endif
*/

    register_haru_pdf_drv();

    /* check whether locale is correctly set */
    if (init_locale() != RETURN_SUCCESS) {
        errmsg(QObject::tr("Invalid or unsupported locale").toLocal8Bit().constData());
    }
    /* default is POSIX */
    set_locale_num(FALSE);

    /* load startup file */
    char dummy[1024];
    sprintf(dummy,"%s/gracerc",qt_grace_exe_dir);
#ifndef WINDOWS_SYSTEM
    getparms(dummy);//this should only be useful on unix-systems
#endif

    /* load default template */
    new_project(NULL);

    cur_graph = get_cg();
    
    if (argc >= 2) {
        for (i = 1; i < argc; i++) {
            if (argv[i][0] == '-' && argv[i][1] != '\0') {
                if (argmatch(argv[i], "-version", 2)) {
                    VersionInfo();
                    exit(0);
                }
#if defined(DEBUG)
                if (argmatch(argv[i], "-debug", 6)) {
                    i++;
                    if (i == argc) {
                        fprintf(stderr, "Missing argument for debug flag\n");
                        usage(stderr, argv[0]);
                    } else {
                        set_debuglevel(atoi(argv[i]));
                        if (get_debuglevel() == 4) {
                            /* turn on debugging in pars.y */
                            yydebug = TRUE;
                        }
                    }
                } else
#endif
                    if (argmatch(argv[i], "-nosigcatch", 6)) {
                        sigcatch = FALSE;
                    } else if (argmatch(argv[i], "-autoscale", 2)) {
                        i++;
                        if (i == argc) {
                            errmsg(QObject::tr("Missing argument for autoscale flag").toLocal8Bit().constData());
                            usage(stderr, argv[0]);
                        } else {
                            if (!strcmp("x", argv[i])) {
                                autoscale_onread = AUTOSCALE_X;
                            } else if (!strcmp("y", argv[i])) {
                                autoscale_onread = AUTOSCALE_Y;
                            } else if (!strcmp("xy", argv[i])) {
                                autoscale_onread = AUTOSCALE_XY;
                            } else if (!strcmp("none", argv[i])) {
                                autoscale_onread = AUTOSCALE_NONE;
                            } else {
                                errmsg(QObject::tr("Improper argument for autoscale flag").toLocal8Bit().constData());
                                usage(stderr, argv[0]);
                            }
                        }
                    } else if (argmatch(argv[i], "-batch", 2)) {
                        i++;
                        if (i == argc) {
                            fprintf(stderr, "Missing argument for batch file\n");
                            usage(stderr, argv[0]);
                        } else {
                            strcpy(batchfile, argv[i]);
                        }
                    } else if (argmatch(argv[i], "-datehint", 5)) {
                        i++;
                        if (i == argc) {
                            errmsg(QObject::tr("Missing argument for datehint flag").toLocal8Bit().constData());
                            usage(stderr, argv[0]);
                        } else {
                            if (!strcmp("iso", argv[i])) {
                                set_date_hint(FMT_iso);
                            } else if (!strcmp("european", argv[i])) {
                                set_date_hint(FMT_european);
                            } else if (!strcmp("us", argv[i])) {
                                set_date_hint(FMT_us);
                            } else if (!strcmp("nohint", argv[i])) {
                                set_date_hint(FMT_nohint);
                            } else {
                                errmsg(QObject::tr("Improper argument for datehint flag").toLocal8Bit().constData());
                                usage(stderr, argv[0]);
                            }
                        }
                    } else if (argmatch(argv[i], "-pipe", 5)) {
                        inpipe = TRUE;
                    /*} else if (argmatch(argv[i], "-ServerMode", 4)) {
                        enableServerMode = TRUE;*/
                    } else if (argmatch(argv[i], "-noprint", 8)) {
                        noprint = TRUE;
                    } else if (argmatch(argv[i], "-dpipe", 6)) {
                        i++;
                        if (i == argc) {
                            fprintf(stderr, "Missing argument for descriptor pipe\n");
                            usage(stderr, argv[0]);
                        } else {
                            fd = atoi(argv[i]);
                            sprintf(fd_name, "pipe<%d>", fd);
                            if (register_real_time_input(fd, fd_name, FALSE) != RETURN_SUCCESS)
                            {
                                exit(1);
                            }
                        }
                    } else if (argmatch(argv[i], "-npipe", 6)) {
                        i++;
                        if (i == argc)
                        {
                            fprintf(stderr, "Missing argument for named pipe\n");
                            usage(stderr, argv[0]);
                        }
                        else
                        {
                            if (openPipe(argv[i],&fd)!=RETURN_SUCCESS)
                            {
                                exit(1);
                            }
                            /*fd = open(argv[i], O_RDONLY | O_NONBLOCK); //open a pipe!!
                        if (fd < 0)
                        {
                        fprintf(stderr, "Can't open fifo\n");
                        }
                        else
                        {
                        //cout << "opened pipe #" << argv[i] << "#" << endl;
                            if (register_real_time_input(fd, argv[i], TRUE) != RETURN_SUCCESS)
                            {
                            exit(1);
                            }
                        }*/
                        }
#ifdef HAVE_NETCDF
                    } else if (argmatch(argv[i], "-netcdf", 7) || argmatch(argv[i], "-hdf", 4)) {
                        i++;
                        if (i == argc) {
                            fprintf(stderr, "Missing argument for netcdf file\n");
                            usage(stderr, argv[0]);
                        } else {
                            strcpy(netcdf_name, argv[i]);
                        }
                    } else if (argmatch(argv[i], "-netcdfxy", 9) || argmatch(argv[i], "-hdfxy", 6)) {
                        i++;
                        if (i == argc) {
                            fprintf(stderr, "Missing argument for netcdf X variable name\n");
                            usage(stderr, argv[0]);
                        } else {
                            strcpy(xvar_name, argv[i]);
                        }
                        i++;
                        if (i == argc) {
                            fprintf(stderr, "Missing argument for netcdf Y variable name\n");
                            usage(stderr, argv[0]);
                        } else {
                            strcpy(yvar_name, argv[i]);
                        }
                        if (strcmp(xvar_name, "null")) {
                            readnetcdf(cur_graph, -1, netcdf_name, xvar_name, yvar_name, -1, -1, 1);
                        } else {
                            readnetcdf(cur_graph, -1, netcdf_name, NULL, yvar_name, -1, -1, 1);
                        }
#endif				/* HAVE_NETCDF */
                    } else if (argmatch(argv[i], "-timer", 6)) {
                        i++;
                        if (i == argc) {
                            fprintf(stderr, "Missing argument for time delay\n");
                            usage(stderr, argv[0]);
                        } else {
                            timer_delay = atoi(argv[i]);
                        }
                        /*#ifndef NONE_GUI*/
                    } else if (argmatch(argv[i], "-install", 7)) {
                        install_cmap = CMAP_INSTALL_ALWAYS;
                    } else if (argmatch(argv[i], "-noinstall", 9)) {
                        install_cmap = CMAP_INSTALL_NEVER;
                    } else if (argmatch(argv[i], "-barebones", 9)) {
                        mainWin->set_barebones( TRUE );
                        /*#endif*/
                    } else if (argmatch(argv[i], "-timestamp", 10)) {
                        timestamp.active = TRUE;
                    } else if (argmatch(argv[i], "-QtFonts", 8)) {
                        useQtFonts=true;
                    } else if (argmatch(argv[i], "-remove", 7)) {
                        remove_flag = TRUE;
                    } else if (argmatch(argv[i], "-fixed", 5)) {
                        i++;
                        if (i == argc) {
                            fprintf(stderr, "Missing argument for fixed canvas width\n");
                            usage(stderr, argv[0]);
                        } else {
                            if (i == argc - 1) {
                                fprintf(stderr, "Missing argument for fixed canvas height\n");
                                usage(stderr, argv[0]);
                            } else {
                                wpp = atoi(argv[i]);
                                i++;
                                hpp = atoi(argv[i]);
                                set_page_dimensions(wpp, hpp, FALSE);
#ifndef NONE_GUI
                                set_pagelayout(PAGE_FIXED);
#endif
                            }
                        }
#ifndef NONE_GUI
                    } else if (argmatch(argv[i], "-free", 5)) {
                        set_pagelayout(PAGE_FREE);
#endif
                    } else if (argmatch(argv[i], "-noask", 5)) {
                        noask = TRUE;
#ifndef NONE_GUI
                    } else if (argmatch(argv[i], "-mono", 5)) {
                        monomode = TRUE;
#endif
                    } else if (argmatch(argv[i], "-hdevice", 5)) {
                        i++;
                        if (i == argc) {
                            hdeviceFlag = true;
                            hardCopyDeviceNr = DEVICE_PDF;// --> no further Argument --> create pdf
                            //fprintf(stderr, "Missing argument for hardcopy device select flag\n");
                            //usage(stderr, argv[0]);
                        } else {
                            if (set_printer_by_name(argv[i]) != RETURN_SUCCESS) {//uable to set printing device (or print to file)
                                hdeviceFlag = false;
                                errmsg(QObject::tr("Unknown or unsupported device").toLocal8Bit().constData());
                                exit(1);
                            } else {//successfull device selection --> maybe we need a special treatment
                                hdeviceFlag = true;

                                hardCopyDeviceNr = get_device_by_name(argv[i]);

                                /*if(!strcmp("PDF",argv[i])){
                                    hardCopyDeviceNr = DEVICE_PDF;
                                }else if(!strcmp("PNG",argv[i])){
                                    hardCopyDeviceNr = DEVICE_PNG;
                                }else if(!strcmp("HD_PNG",argv[i])){
                                    hardCopyDeviceNr = DEVICE_HD_PNG;
                                }else if(!strcmp("PostScript",argv[i])){
                                    hardCopyDeviceNr = DEVICE_PS;
                                }else if(!strcmp("EPS",argv[i])){
                                    hardCopyDeviceNr = DEVICE_EPS;
                                }else if(!strcmp("SVG",argv[i])){
                                    hardCopyDeviceNr = DEVICE_SVG;
                                }else if(!strcmp("PDF(Haru)",argv[i])){
                                    hardCopyDeviceNr = DEVICE_PDF_HARU;
                                }*/
                            }
                        }
                    } else if (argmatch(argv[i], "-log", 2)) {
                        i++;
                        if (i == argc) {
                            fprintf(stderr, "Missing argument for log plots flag\n");
                            usage(stderr, argv[0]);
                        }
                        if (!strcmp("x", argv[i])) {
                            set_graph_xscale(cur_graph, SCALE_LOG);
                        } else if (!strcmp("y", argv[i])) {
                            set_graph_yscale(cur_graph, SCALE_LOG);
                        } else if (!strcmp("xy", argv[i])) {
                            set_graph_xscale(cur_graph, SCALE_LOG);
                            set_graph_yscale(cur_graph, SCALE_LOG);
                        } else {
                            fprintf(stderr, "%s: Improper argument for -l flag; should be one of 'x', 'y', 'xy'\n", argv[0]);
                        }
                    } else if (argmatch(argv[i], "-printfile", 6)) {
                        i++;
                        if (i == argc) {
                            fprintf(stderr, "Missing file name for printing\n");
                            usage(stderr, argv[0]);
                        } else {
                            set_ptofile(TRUE);
                            strcpy(print_file, argv[i]);
                        }
                    } else if (argmatch(argv[i], "-hardcopy", 6)) {
                        gracebat = TRUE;
                    } else if (argmatch(argv[i], "-pexec", 6)) {
                        i++;
                        if (i == argc) {
                            fprintf(stderr, "Missing argument for exec\n");
                            usage(stderr, argv[0]);
                        } else {
                            scanner(argv[i]);
                        }
                    } else if (argmatch(argv[i], "-graph", 6)) {
                        i++;
                        if (i == argc) {
                            fprintf(stderr, "Missing parameter for graph select\n");
                            usage(stderr, argv[0]);
                        } else {
                            sscanf(argv[i], "%d", &gno);
                            if (set_graph_active(gno) == RETURN_SUCCESS) {
                                cur_graph = gno;
                                select_graph(gno);
                            } else {
                                fprintf(stderr, "Error activating graph %d\n", gno);
                            }
                        }
                    } else if (argmatch(argv[i], "-block", 6)) {
                        i++;
                        if (i == argc)
                        {
                            fprintf(stderr, "Missing filename for block data\n");
                            usage(stderr, argv[0]);
                        }
                        else
                        {
                            prepare_strings_for_saving();
                            getdata(cur_graph, argv[i], cursource, LOAD_BLOCK);
                            resume_strings_after_load_or_save();
                        }
                    } else if (argmatch(argv[i], "-bxy", 4)) {
                        i++;
                        if (i == argc) {
                            fprintf(stderr, "Missing parameter for block data set creation\n");
                            usage(stderr, argv[0]);
                        } else {
                            int nc, *cols, scol;
                            if (field_string_to_cols(argv[i], &nc, &cols, &scol) !=
                                    RETURN_SUCCESS) {
                                errmsg(QObject::tr("Erroneous field specifications").toLocal8Bit().constData());
                                return 1;
                            }
                            create_set_fromblock(cur_graph, NEW_SET,
                                                 curtype, nc, cols, scol, autoscale_onread);
                            xfree(cols);
                        }
                    } else if (argmatch(argv[i], "-nxy", 4)) {
                        i++;
                        if (i == argc)
                        {
                            fprintf(stderr, "Missing filename for nxy data\n");
                            usage(stderr, argv[0]);
                        }
                        else
                        {
                            prepare_strings_for_saving();
                            getdata(cur_graph, argv[i], cursource, LOAD_NXY);
                            resume_strings_after_load_or_save();
                        }
                    } else if (argmatch(argv[i], "-type", 2) ||
                               argmatch(argv[i], "-settype", 8)) {
                        /* set types */
                        i++;
                        curtype = get_settype_by_name(argv[i]);
                        if (curtype == -1) {
                            fprintf(stderr, "%s: Unknown set type '%s'\n", argv[0], argv[i]);
                            usage(stderr, argv[0]);
                        }
                    } else if (argmatch(argv[i], "-graphtype", 7)) {
                        i++;
                        if (i == argc) {
                            fprintf(stderr, "Missing argument for graph type\n");
                        } else {
                            if (!strcmp("xy", argv[i])) {
                                set_graph_type(cur_graph, GRAPH_XY);
                            } else if (!strcmp("polar", argv[i])) {
                                set_graph_type(cur_graph, GRAPH_POLAR);
                            } else if (!strcmp("bar", argv[i])) {
                                set_graph_type(cur_graph, GRAPH_CHART);
                            } else if (!strcmp("smith", argv[i])) {
                                set_graph_type(cur_graph, GRAPH_SMITH);
                            } else if (!strcmp("fixed", argv[i])) {
                                set_graph_type(cur_graph, GRAPH_FIXED);
                            } else if (!strcmp("pie", argv[i])) {
                                set_graph_type(cur_graph, GRAPH_PIE);
                            } else {
                                fprintf(stderr, "%s: Improper argument for -graphtype\n", argv[0]);
                                usage(stderr, argv[0]);
                            }
                        }
                    } else if (argmatch(argv[i], "-legend", 4)) {
                        i++;
                        if (i == argc) {
                            fprintf(stderr, "Missing argument for -legend\n");
                            usage(stderr, argv[0]);
                        } else {
                            if (!strcmp("load", argv[i])) {
                                loadlegend = TRUE;
                                set_graph_legend_active(cur_graph, TRUE);
                            } else {
                                fprintf(stderr, "Improper argument for -legend\n");
                                usage(stderr, argv[0]);
                            }
                        }
                    } else if (argmatch(argv[i], "-rvideo", 7)) {
                        reverse_video();
                    } else if (argmatch(argv[i], "-param", 2)) {
                        i++;
                        if (i == argc) {
                            fprintf(stderr, "Missing parameter file name\n");
                            usage(stderr, argv[0]);
                        } else {
                            if (!getparms(argv[i])) {
                                fprintf(stderr, "Unable to read parameter file %s\n", argv[i]);
                            }
                        }
                    } else if (argmatch(argv[i], "-results", 2)) {
                        i++;
                        if (i == argc) {
                            fprintf(stderr, "Missing results file name\n");
                            usage(stderr, argv[0]);
                        } else {
                            /*  open resfile if -results option given */
                            if ((resfp = grace_openw(argv[i])) == NULL) {
                                exit(1);
                            }
                            setvbuf(resfp, NULL, _IOLBF, 0);
                        }
                    } else if (argmatch(argv[i], "-saveall", 8)) {
                        i++;
                        if (i == argc) {
                            fprintf(stderr, "Missing save file name\n");
                            usage(stderr, argv[0]);
                        } else {
                            save_project(argv[i]);
                        }
                    } else if (argmatch(argv[i], "-wd", 3)) {
                        i++;
                        if (i == argc) {
                            fprintf(stderr, "Missing parameters for working directory\n");
                            usage(stderr, argv[0]);
                        } else {
                            if (set_workingdir(argv[i]) != RETURN_SUCCESS) {
                                fprintf(stderr, "Can't change to directory %s, fatal error", argv[i]);
                                exit(1);
                            }
                        }
                    } else if (argmatch(argv[i], "-source", 2)) {
                        i++;
                        if (i == argc) {
                            fprintf(stderr, "Missing argument for data source parameter\n");
                            usage(stderr, argv[0]);
                        }
                        if (argmatch(argv[i], "pipe", 4)) {
                            cursource = SOURCE_PIPE;
                        } else if (argmatch(argv[i], "disk", 4)) {
                            cursource = SOURCE_DISK;
                        }
                    } else if (argmatch(argv[i], "-viewport", 2)) {
                        i++;
                        if (i > argc - 4) {
                            fprintf(stderr, "Missing parameter(s) for viewport setting\n");
                            usage(stderr, argv[0]);
                        } else {
                            v.xv1 = atof(argv[i++]);
                            v.yv1 = atof(argv[i++]);
                            v.xv2 = atof(argv[i++]);
                            v.yv2 = atof(argv[i]);
                            set_graph_viewport(cur_graph, v);
                        }
                    }
                //2013-13-09 Nimalendiran Kailasanathan added command to hide QtGrace main application window and communication
                //with ViewBeast
                    else if (argmatch(argv[i], "-ServerMode", 4)) {
                        i++;
                        if (i > argc - 2) {
                            fprintf(stderr, "Missing parameter(s) to connect to client\n");
                            usage(stderr, argv[0]);
                        } else {
                            //connectToViewBeast = TRUE;
                            enableServerMode = TRUE;
                            strcpy(sendToBeast, argv[i++]);
                            strcpy(readFromBeast, argv[i]);
                        }
                    }
                    else if (argmatch(argv[i], "-hideMainWindow", 15)) {
                        hideMainWindow = TRUE;
                        disableConsole = FALSE;
                    }
                    else if (argmatch(argv[i], "-world", 2)) {
                        i++;
                        if (i > argc - 4) {
                            fprintf(stderr, "Missing parameter(s) for world setting\n");
                            usage(stderr, argv[0]);
                        } else {
                            w.xg1 = atof(argv[i++]);
                            w.yg1 = atof(argv[i++]);
                            w.xg2 = atof(argv[i++]);
                            w.yg2 = atof(argv[i]);
                            set_graph_world(cur_graph, w);
                        }
                    } else if (argmatch(argv[i], "-seed", 5)) {
                        i++;
                        if (i == argc) {
                            fprintf(stderr, "Missing seed for srand48()\n");
                            usage(stderr, argv[0]);
                        } else {
#ifndef WINDOWS_SYSTEM
                            srand48(atol(argv[i]));	/* note atol() */
#else
                            srand(atol(argv[i]));
#endif
                        }
                    } else if (argmatch(argv[i], "-maxpath", 8)) {
                        i++;
                        if (i == argc) {
                            fprintf(stderr, "Missing argument for max drawing path\n");
                            usage(stderr, argv[0]);
                        } else {
                            set_max_path_limit(atoi(argv[i]));
                        }
                    } else if (argmatch(argv[i], "-safe", 5)) {
                        safe_mode = TRUE;
                    } else if (argmatch(argv[i], "-nosafe", 7)) {
                        safe_mode = FALSE;
                    } else if (argmatch(argv[i], "-help", 2)) {
                        usage(stdout, argv[0]);
                    } else if (argmatch(argv[i], "-usage", 5)) {
                        usage(stdout, argv[0]);
                    } else {
                        fprintf(stderr, "No option %s\n", argv[i]);
                        usage(stderr, argv[0]);
                    }
            } else {
                if (i != argc)
                {
                    prepare_strings_for_saving();
                    if (getdata(cur_graph, argv[i], cursource, LOAD_SINGLE) == RETURN_SUCCESS)
                    {
                        set_docname(argv[i]);
                        if (remove_flag)
                        {
                            unlink(argv[i]);
                        }
                        update_default_props();
                        clear_dirtystate();
                    }
                    resume_strings_after_load_or_save();
                }		/* end if */
            }			/* end else */
        }			/* end for */
    }				/* end if */

    
    /*
     * Process events.
     */
    if (sigcatch == TRUE) {
        installSignal();
    }

    /*
 * load legend
 */
    if (loadlegend) {
        for (i = 0; i < number_of_graphs(); i++) {
            if (is_graph_active(i)) {
                for (j = 0; j < number_of_sets(i); j++) {
                    load_comments_to_legend(i, j);
                }
            }
        }
    }

    /*
 * if -hardcopy on command line or executed as gracebat,
 * just plot the graph and quit
 */
    if (gracebat == TRUE) {
        if (hdevice == DEVICE_SCREEN)
        {
            errmsg(QObject::tr("Terminal device can't be used for batch plotting").toLocal8Bit().constData());
            exit(1);
        }
        if (inpipe == TRUE)
        {
            prepare_strings_for_saving();
            getdata(cur_graph, "stdin", SOURCE_DISK, LOAD_SINGLE);
            resume_strings_after_load_or_save();
            inpipe = FALSE;
        }
        if (batchfile[0])
        {
            getparms(batchfile);
        }
        while (real_time_under_monitoring())
        {
            monitor_input(ib_tbl, ib_tblsize, 0);
        }
        if (!noprint)
        {
        cout << "ptofile=" << get_ptofile() << endl;
        cout << "outfile=" << print_file << endl;
            do_hardcopy();
        }
        bailout();
    } else {
        /*
 * go main loop
 */
        /*
#ifndef NONE_GUI
        if (cli == TRUE) {
            cli_loop();
        } else {
            startup_gui();
        }
#else
        cli_loop();
#endif        
*/

        if (inpipe == TRUE)
        {
            prepare_strings_for_saving();
            getdata(get_cg(), "stdin", SOURCE_DISK, LOAD_SINGLE);
            resume_strings_after_load_or_save();
            inpipe = FALSE;
        }
        if (batchfile[0])
        {
            getparms(batchfile);
        }
    }

    init_color_icons(number_of_colors(),cmap_table,allocated_colors,&ColorIcons,&ColorPixmaps,&ColorNames);
    //init_color_icons();

    /* never reaches */
    ///exit(0);
    return 0;
}

/*
 * command interface loop
 */
void cli_loop(void)//never used--replaced by the Qt-event-loop of QApplication
{
    Input_buffer *ib_stdin;
    int previous = -1;

    if (inpipe == TRUE)
    {
        prepare_strings_for_saving();
        getdata(get_cg(), "stdin", SOURCE_DISK, LOAD_SINGLE);
        resume_strings_after_load_or_save();
        inpipe = FALSE;
    }
    if (batchfile[0])
    {
        getparms(batchfile);
    }
    if (register_real_time_input(STDIN_FILENO, "stdin", 0) != RETURN_SUCCESS)
    {
        exit(1);
    }
    for (ib_stdin = ib_tbl; ib_stdin->fd != STDIN_FILENO; ib_stdin++)
    {
        ;
    }
    while (ib_stdin->fd == STDIN_FILENO)
    {
        /* the standard input is still under monitoring */
        if (ib_stdin->lineno != previous)
        {
            printf("grace:%d> ", ib_stdin->lineno + 1);
            fflush(stdout);
            previous = ib_stdin->lineno;
        }
        monitor_input(ib_tbl, ib_tblsize, 0);
    }
}

static void usage(FILE *stream, char *progname)
{
    /* We use alphabetial order */

    fprintf(stream, "Usage of %s command line arguments: \n", progname);

    fprintf(stream, "-autoscale [x|y|xy|none]              Set autoscale type\n");
#ifndef NONE_GUI
    fprintf(stream, "-barebones                            Turn off all toolbars\n");
#endif
    fprintf(stream, "-batch     [batch_file]               Execute batch_file on start up\n");
    fprintf(stream, "-block     [block_data]               Assume data file is block data\n");
    fprintf(stream, "-bxy       [x:y:etc.]                 Form a set from the current block data set\n");
    fprintf(stream, "                                        using the current set type from columns\n");
    fprintf(stream, "                                        given in the argument\n");
    fprintf(stream, "-ServerMode [Server1 Server2]         start in Server-Mode\n");
    fprintf(stream, "-datehint  [iso|european|us\n");
    fprintf(stream, "            |days|seconds|nohint]     Set the hint for dates analysis\n");
    fprintf(stream, "                                        (it is only a hint for the parser)\n");
#if defined(DEBUG)
    fprintf(stream, "-debug     [debug_level]              Set debugging options\n");
#endif
    fprintf(stream, "-dpipe     [descriptor]               Read data from descriptor on startup\n");
    fprintf(stream, "-fixed     [width] [height]           Set canvas size fixed to width*height\n");
#ifndef NONE_GUI
    fprintf(stream, "-free                                 Use free page layout\n");
#endif
    fprintf(stream, "-graph     [graph_number]             Set the current graph number\n");
    fprintf(stream, "-graphtype [graph_type]               Set the type of the current graph\n");
    fprintf(stream, "-hardcopy                             No interactive session, just print and\n");
    fprintf(stream, "                                        quit\n");
    fprintf(stream, "-hdevice   [hardcopy_device_name]     Set default hardcopy device\n");
    fprintf(stream, "-hideMainWindow                       hide the QtGrace application window\n");
#ifndef NONE_GUI
    fprintf(stream, "-install                              Install private colormap\n");
#endif
    fprintf(stream, "-legend    [load]                     Turn the graph legend on\n");
    fprintf(stream, "-log       [x|y|xy]                   Set the axis scaling of the current graph\n");
    fprintf(stream, "                                        to logarithmic\n");
    fprintf(stream, "-maxpath   [length]                   Set the maximal drawing path length\n");
#ifndef NONE_GUI
    fprintf(stream, "-mono                                 Run Grace in monochrome mode (affects\n");
    fprintf(stream, "                                        the display only)\n");
#endif
#ifdef HAVE_NETCDF
    fprintf(stream, "-netcdf    [netcdf file]              Assume data file is in netCDF format\n");
    fprintf(stream, "-netcdfxy  [X var name] [Y var name]  If -netcdf was used previously, read from\n");
    fprintf(stream, "                                        the netCDF file 'X var name' and 'Y\n");
    fprintf(stream, "                                        var name' and create a set. If 'X var\n");
    fprintf(stream, "                                        name' is \"null\" then load the\n");
    fprintf(stream, "                                        index of Y to X\n");
#endif
    fprintf(stream, "-noask                                Assume the answer is yes to all requests -\n");
    fprintf(stream, "                                        if the operation would overwrite a file,\n");
    fprintf(stream, "                                        grace will do so without prompting\n");
#ifndef NONE_GUI
    fprintf(stream, "-noinstall                            Don't use private colormap\n");
#endif
    fprintf(stream, "-noprint                              In batch mode, do not print\n");
    fprintf(stream, "-nosafe                               Disable safe mode\n");
    fprintf(stream, "-nosigcatch                           Don't catch signals\n");
    fprintf(stream, "-npipe     [file]                     Read data from named pipe on startup\n");
    fprintf(stream, "-nxy       [nxy_file]                 Assume data file is in X Y1 Y2 Y3 ...\n");
    fprintf(stream, "                                        format\n");
    fprintf(stream, "-param     [parameter_file]           Load parameters from parameter_file to the\n");
    fprintf(stream, "                                        current graph\n");
    fprintf(stream, "-pexec     [parameter_string]         Interpret string as a parameter setting\n");
    fprintf(stream, "-pipe                                 Read data from stdin on startup\n");
    fprintf(stream, "-printfile [file for hardcopy output] Save print output to file \n");
    fprintf(stream, "-remove                               Remove data file after read\n");
    fprintf(stream, "-results   [results_file]             Write results of some data manipulations\n");
    fprintf(stream, "                                        to results_file\n");
    fprintf(stream, "-rvideo                               Exchange the color indices for black and\n");
    fprintf(stream, "                                        white\n");
    fprintf(stream, "-safe                                 Safe mode (default)\n");
    fprintf(stream, "-saveall   [save_file]                Save all to save_file\n");
    fprintf(stream, "-seed      [seed_value]               Integer seed for random number generator\n");
    fprintf(stream, "-source    [disk|pipe]                Source type of next data file\n");
    fprintf(stream, "-timer     [delay]                    Set allowed time slice for real time\n");
    fprintf(stream, "                                        inputs to delay ms\n");
    fprintf(stream, "-timestamp                            Add timestamp to plot\n");
    fprintf(stream, "-settype   [xy|xydx|...]              Set the type of the next data file\n");
    fprintf(stream, "-version                              Show the program version\n");
    fprintf(stream, "-viewport  [xmin ymin xmax ymax]      Set the viewport for the current graph\n");
    fprintf(stream, "-wd        [directory]                Set the working directory\n");
    fprintf(stream, "-world     [xmin ymin xmax ymax]      Set the world coordinates for the\n");
    fprintf(stream, "                                        current graph\n");

    fprintf(stream, "-usage|-help                          This message\n");
    fprintf(stream, "\n");
    fprintf(stream, " ** If it scrolls too fast, run `%s -help | more\' **\n", progname);
    exit(0);
}

static void VersionInfo(void)
{
    int i;
    
    fprintf(stdout, "\n%s\n\n", bi_version_string());

    /* We don't want to reproduce the complete config.h,
   but those settings which may be related to problems on runtime */

    fprintf(stdout, "GUI toolkit: %s %s\n", bi_gui(),QT_VERSION_STR);
/*#ifdef MOTIF_GUI
    fprintf(stdout, "Xbae version: %s\n", bi_gui_xbae());
#endif*/
    fprintf(stdout, "T1lib: %s\n", bi_t1lib());
#ifdef HAVE_FFTW
    fprintf(stdout, "FFT: FFTW\n");
#else
    fprintf(stdout, "FFT: built-in\n");
#endif
#ifdef HAVE_NETCDF
    fprintf(stdout, "NetCDF support: on\n");
#else
    fprintf(stdout, "NetCDF support: off\n");
#endif
#ifdef HAVE_LIBPNG
    fprintf(stdout, "libpng: %s\n", bi_pnglib());
#endif
#ifdef HAVE_LIBJPEG
    fprintf(stdout, "libjpeg: %s\n", bi_libjpeg());
#endif
#ifdef HAVE_LIBPDF
    fprintf(stdout, "PDFlib: %s\n", bi_libpdf());
#endif


#ifdef DEBUG
    fprintf(stdout, "Debugging: enabled\n");
#endif
    fprintf(stdout, "Built: %s on %s\n", bi_date(), bi_system());
#ifdef _MSC_VER
    fprintf(stdout, "Compiler flags: %s version %d\n", bi_ccompiler(),_MSC_VER);
#else
    fprintf(stdout, "Compiler flags: %s\n", bi_ccompiler());
#endif

    fprintf(stdout, "\n");
    
    fprintf(stdout, "Registered devices:\n");
    for (i = 0; i < number_of_devices(); i++) {
        fprintf(stdout, "%s ", get_device_name(i));
    }
    fprintf(stdout, "\n\n");
    
    fprintf(stdout, "(C) Copyright 1991-1995 Paul J Turner\n");
    fprintf(stdout, "(C) Copyright 1996-2008 Grace Development Team\n");
    fprintf(stdout, "QtGrace: (C) Copyright 2008-2015 Andreas Winter\n");
    fprintf(stdout, "All Rights Reserved\n");

    return;
}

void sync_canvas_size(unsigned int *w, unsigned int *h, int inv)
{
    Page_geometry pg = get_page_geometry();
    if (inv) {
        *w=mainWin->mainArea->width()-8;
        *h=mainWin->mainArea->height()-8;
        ///        GetDimensions(canvas, w, h);
        set_page_dimensions(*w*72.0/pg.dpi, *h*72.0/pg.dpi, TRUE);//hier fehlen die Canvas-Dimensionen!!!!!!!
    } else {
        *w = pg.width;
        *h = pg.height;
        ///        SetDimensions(canvas, *w, *h);
    }
//Page_geometry pg1=get_page_geometry();
//cout << inv << "(inv) sync_canvas_size=" << pg1.width << " x " << pg1.height << " dpi=" << pg1.dpi << endl;
}

static int page_layout = PAGE_FIXED;

int get_pagelayout(void)
{
    return page_layout;
}

#ifdef __cplusplus
extern "C" {
#endif
void set_pagelayout(int layout)
{
    if (page_layout == layout) {
        return;
    }
    
    if (inwin) {
        errmsg(QObject::tr("Can not change layout after initialization of GUI").toLocal8Bit().constData());
        return;
    } else {
        page_layout = layout;
    }
}
#ifdef __cplusplus
}
#endif

/*static void graph_scroll_proc(void *data)
{
    graph_scroll(*(int*) data);
    xdrawgraph();
}

static void graph_zoom_proc(void *data)
{
    graph_zoom(*(int*) data);
    xdrawgraph();
}

static void world_stack_proc(void *data)
{
    switch (*(int*) data) {
    case WSTACK_PUSH_ZOOM:
        push_and_zoom();
        break;
    case WSTACK_PUSH:
        push_world();
        break;
    case WSTACK_POP:
        pop_world();
        break;
    case WSTACK_CYCLE:
        cycle_world_stack();
        break;
    default:
        return;
    }
    update_all();
    xdrawgraph();
}

static void load_example(void *data)
{
    char *s, buf[128];
    
    set_wait_cursor();
    
    s = (char *) data;
    sprintf(buf, "examples/%s", s);
    load_project_file(buf, FALSE);

    xdrawgraph();

    unset_wait_cursor();
}*/

extern int ReqUpdateColorSel;

#ifdef __cplusplus
extern "C" {
#endif
void update_all(void)
{
    static int gno;
    bool sav_imm_upd;
    if (updateRunning==true) return;
    sav_imm_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    gno = get_cg();
    //cout << "update all: upd. graph lists" << endl;
    update_graph_selectors();
    //cout << "finished updating" << endl;
    //cout << "update all: upd. set lists" << endl;
    update_set_lists(gno);
    //cout << "update all: upd. set sel" << endl;
    update_set_selectors(ALL_GRAPHS);
    //cout << "update all: upd. set edit" << endl;
    update_ss_editors(ALL_GRAPHS);
    if (ReqUpdateColorSel == TRUE)
    {
        update_color_selectors();
        ReqUpdateColorSel = FALSE;
    }
    if (FormRegionMaster!=NULL) FormRegionMaster->number_of_graphs_changed();
    if (FormAxisProperties!=NULL) FormAxisProperties->update_ticks(gno);
    /*if (Form_Preferences!=NULL)
    {
        Form_Preferences->tab_prefs->update_props_items();
    }*/
    //cout << "update all: upd. Hotlinks" << endl;
    if (FormHotLinks!=NULL) FormHotLinks->update_hotlinks();
    if (FormPruneData!= NULL)
    {//update_prune_frame();
        SetChoice(FormPruneData->selector[2],(get_graph_xscale(get_cg()) == SCALE_LOG) ? 1 : 0);
        SetChoice(FormPruneData->selector[3],(get_graph_yscale(get_cg()) == SCALE_LOG) ? 1 : 0);
    }
    if (FormLocatorProps!=NULL) FormLocatorProps->update_locator_items(gno);
    if (FormSetAppearance!=NULL) FormSetAppearance->flp->tabLi->cmbSet->update_entries(gno,true);
    if (FormExplorer!=NULL)
    {
        QList<QTreeWidgetItem *> items=FormExplorer->tree->selectedItems();
        if (items.length()>0)
        {
            char selected_type;
            int gno,sno;
            FormExplorer->tree->getItemData(items.at(0),selected_type,gno,sno);
            FormExplorer->tree->RecreateCompleteTree();
            FormExplorer->tree->selectItem(selected_type,gno,sno);
        }
    }
    if (FormRegionMaster!=NULL) FormRegionMaster->init();
    if (FormPlotAppearance!=NULL) FormPlotAppearance->init();
    mainWin->set_stack_message();
    mainWin->lstGraphs->update_number_of_entries_preserve_selection();
    set_left_footer(NULL);
    immediateUpdate=sav_imm_upd;
    updateRunning=false;
}
#ifdef __cplusplus
}
#endif

void update_all_cb(void *data)
{
    update_all();
}

void update_set_lists(int gno)
{
    int i;
    if (gno == GRAPH_SELECT_CURRENT) {
        update_set_selectors(get_cg());
        update_ss_editors(get_cg());
    } else {
        update_set_selectors(gno);
        update_ss_editors(gno);
    }
    //if (1) { //inwin) {
    for (i = 0; i < nplist; i++)
    {
        if (plist[i]->gr_no == gno || (gno == get_cg() && plist[i]->gr_no == GRAPH_SELECT_CURRENT) && plist[i]->prevent_from_autoupdate==false)
        {
            update_set_list(gno, plist[i]);
        }
    }
    //}
}

/*
 * set the message in the left footer
 */
void set_left_footer(char *s)
{
    if (s == NULL)
    {
    QString dispText("");
    QString komma(", ");
    if (show_host_name)
    {
    QString hst=QHostInfo::localHostName();
    dispText+=hst;
        if (show_display_name || displ_project_filename>0 || displ_export_filename>0)
        {
        dispText+=komma;
        }
    }
    if (show_display_name)
    {
    char * dbuf=display_name();
    dispText+=QString(dbuf);
        if (displ_project_filename>0 || displ_export_filename>0)
        {
        dispText+=komma;
        }
    }
    if (displ_project_filename>0)
    {
        if (displ_project_filename==1)
        {
        dispText+=QString(get_docname());
        }
        else
        {
        dispText+=QString(get_docfilename());
        }
        if (displ_export_filename>0)
        {
        dispText+=komma;
        }
    }
    if (displ_export_filename)
    {
        if (displ_export_filename==1)
        {
        dispText+=QString(get_exportname());
        }
        else
        {
        dispText+=QString(get_exportfilename());
        }
    }
        /*
        char * hbuf;
        char * buf=new char[3*GR_MAXPATHLEN+256];
        //gethostname(hbuf, 63);
        hbuf=new char[2+hst.length()];
        buf=new char[1024+hst.length()];
        strcpy(hbuf,hst.toLocal8Bit());
        sprintf(buf, "%s, %s, %s", hbuf, 0, get_docname());
        mainWin->statusBar->showMessage(QString(buf));
        delete[] hbuf;
        delete[] buf;
        */
    mainWin->statusBar->showMessage(dispText);
    }
    else
    {
    mainWin->statusBar->showMessage(QString(s));
    }
}

QBitmap generate_Bitmap_from_Bits(unsigned char * bits,int length,int rows,int cols)//generates a rows x cols-QBitmap(black and white) form the bits[] with length entries (number of bytes)
{
    QBitmap result(cols,rows);
    //result.fill(Qt::white);
    //result.fill(Qt::color0);
    QPainter painter;//(&result);
    painter.begin(&result);
    painter.setPen(Qt::color0);
    painter.setBrush(Qt::color0);
    painter.drawRect(0,0,cols,rows);
    painter.setPen(Qt::color1);
    painter.setBrush(Qt::color1);
    unsigned char probe,ref;
    int row=0,col=0;//current number of row and colum
    for (int i=0;i<length;i++)//go through bits-bytes
    {
        probe=bits[i];//read one byte
        for (int j=0;j<8;j++)//go through the bits
        {
            ref=1<<j;
            if (ref&probe)
                painter.drawPoint(col,row);
            col++;
            if (col>=cols)
            {
                row++;
                col=0;
                break;
            }
        }
    }
    painter.end();
    return result;
}

void generate_Pixmap_from_Bits(unsigned char * bits,int length,int rows,int cols,QPixmap * target)
{
    /*QBitmap tempBitMap(rows,cols);
QPainter paint;
tempBitMap=generate_Bitmap_from_Bits(bits,length,rows,cols);
target->fill(QApplication::palette().window().color());
paint.begin(target);
paint.drawPixmap(0,0,tempBitMap.copy(tempBitMap.rect()));
paint.end();*/
    QPixmap result(rows,cols);
    //result.fill(Qt::color0);
    result.fill(Qt::white);
    QPainter painter;//(&result);
    painter.begin(&result);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    /*
painter.setPen(Qt::color1);
painter.setBrush(Qt::color1);
*/
    unsigned char probe,ref;
    int row=0,col=0;//current number of row and colum
    for (int i=0;i<length;i++)//go through bits-bytes
    {
        probe=bits[i];//read one byte
        for (int j=0;j<8;j++)//go through the bits
        {
            ref=1<<j;
            if (ref&probe)
                painter.drawPoint(col,row);
            col++;
            if (col>=cols)
            {
                row++;
                col=0;
                break;
            }
        }
    }
    painter.end();
    *target=result.copy();
}

void generate_Pixmap_from_Bits(unsigned char * bits,int length,int rows,int cols,QPixmap * target,int color)
{
    QPixmap result(rows,cols);
    QColor colo=get_Color(color);
    //result.fill(Qt::white);
    result.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&result);
    painter.setPen(colo);
    painter.setBrush(colo);
    unsigned char probe,ref;
    int row=0,col=0;//current number of row and colum
    for (int i=0;i<length;i++)//go through bits-bytes
    {
        probe=bits[i];//read one byte
        for (int j=0;j<8;j++)//go through the bits
        {
            ref=1<<j;
            if (ref&probe)
                painter.drawPoint(col,row);
            col++;
            if (col>=cols)
            {
                row++;
                col=0;
                break;
            }
        }
    }
    painter.end();
    *target=result.copy();
}

void convertBitmapToPixmap(QBitmap * source,QPixmap * dest)
{
#if QT_VERSION >= 0x050000
    *dest=source->copy();
#else
    QPixmap m2(source->size());
    m2.fill(Qt::white);
    m2.setAlphaChannel(*source);
    *dest=m2;
    QBitmap bm1;
    bm1.fill(Qt::color1);
    dest->fill(Qt::black);
    dest->setAlphaChannel(bm1);
    //dest->setMask(source->createMaskFromColor(Qt::color1));
    QPainter pai1(dest);
    //pai1.setCompositionMode(QPainter::CompositionMode_Xor);
    pai1.setCompositionMode(QPainter::CompositionMode_SourceOver);
    pai1.drawPixmap(0,0,m2);
    pai1.end();
#endif
}

int openPipe(char * pname,int * fd)
{
    int ret=RETURN_SUCCESS;
    *fd = open(pname, O_RDONLY | O_NONBLOCK); //open a pipe!!
    if (fd < 0)
    {
        fprintf(stderr, "Can't open fifo\n");
    }
    else
    {
        if (register_real_time_input(*fd, pname, TRUE) != RETURN_SUCCESS)
        {
            ret=RETURN_FAILURE;
        }
    }
    return ret;
}

#define NO_HELP "doc/nohelp.html"

#ifdef __cplusplus
extern "C" {
#endif
void HelpCB(char *data)
{
    ///this opens the default web-browser to open a help-file
    ///simple and portable, but the fragment is lost :-(
    ///this means, that "help on context" always opens the users guide at the top of the page instead of the apropriate position
    QString strbuf;
    if (data==strstr(data, "http:") || data==strstr(data, "file:"))
    {
        strbuf=QString(data);
    }
    else
    {
#ifdef WINDOWS_SYSTEM
        strbuf=QString("file:///")+QString(qt_grace_exe_dir)+QString("/../")+QString(data);
#else
        strbuf=QString("file://")+QString(qt_grace_exe_dir)+QString("/../")+QString(data);
#endif
    }
    //qDebug(strbuf.toLocal8Bit());
    ///QUrl helpUrl=QUrl::fromUserInput(strbuf);//not necessary
    QUrl helpUrl(strbuf);
    helpUrl=helpUrl.resolved(QUrl(""));
    ///cout << "#" << data << "#" << strbuf.toLatin1().constData() << "#" << endl;
/*#ifndef WINDOWS_SYSTEM
    if (display_help_externally)
    {
#endif*/
        QDesktopServices::openUrl(helpUrl);//open in std-html-viewer
/*#ifndef WINDOWS_SYSTEM
    }
    else
    {
        QWebView *view = new QWebView(0);
        //cout << "Loading=" << helpUrl.path().toAscii().constData() << endl;
        view->load(helpUrl);
        view->show();
    }
#endif*/
    return;
}
#ifdef __cplusplus
}
#endif
