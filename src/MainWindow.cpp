/***************************************************************************
 *   Copyright (C) 2008-2022 by Andreas Winter                             *
 *   andreas.f.winter@web.de                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "MainWindow.h"
#include "bitmaps.h"
#include "jbitmaps.h"
#include "mbitmaps.h"
#include "patterns.h"
#include "events.h"
#include "graphutils.h"
#include "allWidgets.h"
#include "editWidgets.h"
#include "appearanceWidgets.h"
#include "plotone.h"
#include "files.h"
#include "undo_module.h"
#include "device.h"

#ifdef WINDOWS_SYSTEM
#define WIN_SIZE_CORR 1
#endif

using namespace std;

int staticcounter=0;

QDateTime pipe_change_time;
//QTime GeneralPurposeTimer1;
extern graph *g;
extern bool useQPrinter;
extern bool use_print_command;//to tell the programm whether to use the native dialog (false) or a print command (like lpr -> true)
extern bool use_new_icons;
extern bool Pick_and_Move_active;
extern double GeneralPageZoomFactor;//only used for the screen-display
extern int current_axis_restriction;
//extern long orig_page_w,orig_page_h;
extern QPrinter * stdPrinter;
extern int stdOutputFormat;
extern int border_percent;
int lastPrintDevice=0;//stores the number of the printing device the user selected (used when switching between print-setup and page-setup)
extern bool printing_in_file;//used to tell the driver(s) how to handle focus-flags and transperancy
extern int print_target;//where to print to (how to initialize the x11-driver)
extern int default_Print_Device;
extern Input_buffer *ib_tbl;
extern int ib_tblsize;
extern frmDrawObjects * FormDrawObjects;
extern frmConsole * FormConsole;
extern frmEvalExpr * FormEvalExpr;
extern frmSetAppearance * FormSetAppearance;
extern frmGraphApp * FormGraphAppearance;
extern frmPlotAppearance * FormPlotAppearance;
extern frmLocatorProps * FormLocatorProps;
extern frmAxisProp * FormAxisProperties;
extern frmPointExplorer * FormPointExplorer;
extern frmNonlinCurveFit * FormNonlinCurveFit;
extern frm2DFit * Form2DFit;
extern frmInterpolation * FormInterpolation;
extern frmSetOp * FormSetOperations;
extern frmCommands * FormCommands;
extern frmDeviceSetup * FormDeviceSetup;
extern frmArrangeGraphs * FormArrangeGraphs;
extern frmOverlayGraphs * FormOverlayGraphs;
extern frmAutoscaleGraphs * FormAutoScaleGraphs;
extern frmDataSetProperties * FormDataSetProperties;
extern frmSetOperations * FormSetOPS;
extern frmFeatureExtract * FormFeatureExtraction;
extern frmInterpolation * FormHistograms;
extern frmFourier * FormFourier;
extern frmFourier2 * FormFourier2;
extern frmSmallCalc * FormIntegration;
extern frmSmallCalc * FormSeasonDiff;
extern frmSmallCalc * FormDifferences;
extern frmSmallCalc * FormAverages;
extern frmSmallCalc * FormRegression;
extern frmCorrelation * FormCorrelation;
extern frmCorrelation * FormConvolution;
extern frmCorrelation * FormDigitalFilter;
extern frmTransform * FormPruneData;
extern frmTransform * FormSamplePoints;
extern frmTransform * FormGeometricTransform;
extern frmHotLinks * FormHotLinks;
extern frmMasterRegionOperator * FormRegionMaster;
extern frmRegionStatus * FormRegionStatus;
extern frmRegions * FormReportRegion;
extern frmRegions * FormClearRegion;
extern frmRegions * FormDefineRegion;
extern frmExplorer * FormExplorer;
extern frmRealTimeInputManager * FormRTIManage;
extern frmReportOnFitParameters * FormReportFitParameters;
extern frmGeometricEvaluation * FormGeometricEvaluation;
extern frmAgrInfos * Form_AgrInfo;

extern frmNetCDF * FormNetCDF;
extern frmIOForm * FormReadSets;
extern frmIOForm * FormReadNetCDF;
extern frmIOForm * FormWriteSets;
extern frmIOForm * FormOpenProject;
extern frmIOForm * FormSaveProject;
extern frmIOForm * FormReadParameters;
extern frmIOForm * FormWriteParameters;
extern frmIOForm * FormReadBinary;
extern frmIOForm * FormWriteBinary;
extern frmCSVImporter * FormCSVImport;
extern frmCSVImporterV2 * FormCSVImportV2;
extern frmProgressWin * FormProgress;
extern frmQuestionDialog * FormQuestion;

extern frmBinaryFormatInput * FormBinaryImportFilter;

extern frmFontTool * FormFontTool;
extern frmAbout * FormAbout;
extern frmUndoList * FormUndoList;

extern frm_Preferences * Form_Preferences;
//extern frmColorManagement * FormColManage;
//extern frmPreferences * FormPreferences;
//extern frmExtraPreferences * ExtraPreferences;
extern SetupWizard * Form_SetupWizard;
extern InitQtGraceWizard * Form_InitWizard;
extern bailoutQuestion * FormBailoutQuestion;

extern bool showhideworkaround;
extern bool immediateUpdate;
extern bool updateRunning;
extern int rtiCheckTime;
extern int current_object_id,current_object_type;

extern int anchor_x;
extern int anchor_y;
extern QRect ShiftRect;
extern QPoint ShiftPoint;
extern MainWindow * mainWin;
extern QImage * MainPixmap;
extern QPainter * GeneralPainter;
extern QBitmap * patterns[MAXPATTERNS];
extern QPixmap * PatternPixmaps[MAXPATTERNS];
extern QBitmap * autoBitMap;
extern QBitmap * downBitMap;
extern QBitmap * expandBitMap;
extern QBitmap * leftBitMap;
extern QBitmap * rightBitMap;
extern QBitmap * shrinkBitMap;
extern QBitmap * upBitMap;
extern QBitmap * zoomBitMap;

extern QBitmap * redrawMap;
extern QBitmap * autoMap;
extern QBitmap * zoomXMap;
extern QBitmap * zoomYMap;
extern QBitmap * autoXMap;
extern QBitmap * autoYMap;
extern QBitmap * autoTickMap;
extern QBitmap * exitMap;

extern QPixmap * GraceIcon;
extern QPixmap * ActiveIcon;
extern QPixmap * HiddenIcon;
extern QPixmap * Qt_j_lm_o_bits;
extern QPixmap * Qt_j_cm_o_bits;
extern QPixmap * Qt_j_rm_o_bits;
extern QPixmap * Qt_j_lb_b_bits;
extern QPixmap * Qt_j_lm_b_bits;
extern QPixmap * Qt_j_lt_b_bits;
extern QPixmap * Qt_j_ct_b_bits;
extern QPixmap * Qt_j_rt_b_bits;
extern QPixmap * Qt_j_rm_b_bits;
extern QPixmap * Qt_j_rb_b_bits;
extern QPixmap * Qt_j_cb_b_bits;
extern QPixmap * Qt_j_cm_b_bits;
extern QPixmap * Qt_j_left_bits;
extern QPixmap * Qt_j_right_bits;
extern QPixmap * Qt_j_center_bits;

extern QPixmap * Qt_m_hv_lr_bt_bits;
extern QPixmap * Qt_m_hv_lr_tb_bits;
extern QPixmap * Qt_m_hv_rl_bt_bits;
extern QPixmap * Qt_m_hv_rl_tb_bits;
extern QPixmap * Qt_m_vh_lr_bt_bits;
extern QPixmap * Qt_m_vh_lr_tb_bits;
extern QPixmap * Qt_m_vh_rl_bt_bits;
extern QPixmap * Qt_m_vh_rl_tb_bits;

extern QPixmap * Qt_justifications[12];
extern QPixmap * Qt_sub_justifications[3];
extern QPixmap * Qt_matrixOrder[8];

extern int allocated_colors;
extern QIcon ** ColorIcons;
extern QPixmap ** ColorPixmaps;
extern QString ** ColorNames;
/*extern QIcon * LineIcons[MAXLINESTYLES];
extern QPixmap * LinePixmaps[MAXLINESTYLES];
extern QVector<qreal> * PenDashPattern[MAXLINESTYLES];*/
extern int nr_of_current_linestyles;
extern int * lenghts_of_linestyle_patterns;
extern char ** current_linestyle_patterns;
extern QIcon ** LineIcons;
extern QPixmap ** LinePixmaps;
extern QVector<qreal> ** PenDashPattern;

extern QCursor * wait_cursor;
extern QCursor * line_cursor;
extern QCursor * find_cursor;
extern QCursor * move_cursor;
extern QCursor * text_cursor;
extern QCursor * kill_cursor;
extern int cur_cursor;

extern CanvasAction action_flag;
extern char print_file[];

extern int max_history;
extern int current_history;
extern QString history[MAX_HISTORY];
extern char dummy[];
extern QStringList ListOfChanges;
extern QStringList ListOfOldStates;

extern QColor * stdColors[16];//standard Colors for Drawing
extern QBrush * stdBrush;//standard Brush for Drawing
extern QPen * stdPen;//standard Pen for Drawing

extern QBitmap generate_Bitmap_from_Bits(unsigned char * bits,int length,int rows,int cols);
extern void generate_Pixmap_from_Bits(unsigned char * bits,int length,int rows,int cols,double factor,QPixmap * target);
extern void convertBitmapToPixmap(QBitmap * source,QPixmap * dest);
extern void convertBitmapToIcon(QBitmap * source,QIcon * dest);
extern void convertBitmapToScaledIcon(QBitmap * source,QIcon * dest,QSize s);
extern int find_dev_nr(const char * dev_name);
extern void HelpCB(char *data);
extern void get_tracking_props(int *setno, int *move_dir, int *add_at);
extern int graph_zoom(int type,int nr_of_graphs,int * graph_nrs);
extern int graph_scroll(int type,int nr_of_graphs,int * graph_nrs);
extern QPoint VPoint2XPoint(VPoint vp);
extern VPoint xlibdev2VPoint2(qreal x, qreal y);
//extern bool openNativePrinter(int dev);
extern void drawSimpleSymbol(QPainter & paint1,int x,int y,int w,int sym,char sym_c);
extern void RecreatePatternIcons(int w,int h);
extern void round_axis_limits(double *amin, double *amax, int scale);

int nr_of_highlight_segments=0;
int nr_of_alloc_highlight_segm=0;
QVector<QPointF> * highlight_segments=NULL;
int highlight_width=5;
QList<int> highlight_clipping;//clip the segment to a graph-id or not (-1)
QList<int> highlight_type;//what to highlight (-1=line, >=0:symbol-type)
QList<int> highlight_set;

int nr_of_collect_set_lines=0;
int * collect_gnos=NULL;
int * collect_snos=NULL;
QList< QVector<QPointF> > collect_segments;

extern QString Last_Dialog_Path[NR_OF_FILE_DIALOGS];

#ifdef DEBUG_OUT_LOG
extern ofstream debug_out;
#endif

int donut_type;//0=donut, 1=pie-segment
double zoom_donut1,zoom_donut2;//for zooming in polar graph v2

#ifdef __cplusplus
extern "C" {
#endif
extern void prepare_strings_for_saving(void);
extern void resume_strings_after_load_or_save(void);
#ifdef __cplusplus
}
#endif

extern int should_open_grace_file_dialog(void);

extern void GeneralPaste(const QMimeData * mimeData);
extern void update_default_props(void);
extern void write_settings(void);
extern void removeAllWindows(void);

extern Device_entry *device_table;

extern QString get_filename_with_extension(int device);

#if QT_VERSION >= 0x050000
ulong os_dbl_click_dt;
ulong last_click_timestamp;
#endif

void init_Patterns(void)
{
    /*QPixmap tempIcon(38,16);//38,24
    QPixmap tempIcon2(38,16);//38,24
    QPainter paint;
    qDebug() << "InitPatterns";*/
    for (int i=0;i<MAXPATTERNS;i++)
    {
        PatternPixmaps[i]=new QPixmap(38,16);
        if (patterns[i]!=NULL) delete patterns[i];
        patterns[i]=new QBitmap();
        *(patterns[i])=generate_Bitmap_from_Bits(pat_bits[i],32,16,16);
        /*tempIcon.fill(QApplication::palette().window().color());
        paint.begin(&tempIcon);
        paint.setPen(Qt::black);
        paint.setBrush(Qt::black);
        if (i==0)
        {
            paint.drawText(4,12,QObject::tr("None"));//16
            qDebug() << "Text Pixel Size=" << paint.font().pixelSize();
        }
        else
        {
            convertBitmapToPixmap(patterns[i],&tempIcon2);
            paint.drawPixmap(11,0,tempIcon2);//4
        }
        paint.end();
        PatternPixmaps[i]=new QPixmap(tempIcon);
        qDebug() << "PatternPixmaps[i].size=" << PatternPixmaps[i]->size();*/
    }
RecreatePatternIcons(38*toolBarSizeFactor,16*toolBarSizeFactor);
}

void prepare_highlight_segments_with_graphs(int n,int * gnos)
{
view bb1;
VPoint vp;
QPointF tmp_point;
double sd_width=0.01;
highlight_clipping.clear();
highlight_type.clear();
if (nr_of_alloc_highlight_segm<n)
{
    if (highlight_segments!=NULL)
    {
    delete[] highlight_segments;
    highlight_segments=NULL;
    }
nr_of_alloc_highlight_segm=n;
    if (n>0)
    {
    highlight_segments=new QVector<QPointF>[n];
    }
}
nr_of_highlight_segments=0;
for (int i=0;i<n;i++)
{
    if (!is_valid_gno(gnos[i]) || is_graph_hidden(gnos[i])) continue;
nr_of_highlight_segments++;
highlight_segments[i].clear();
highlight_clipping << -1;//no clipping for the graph-highlights (otherwise the highlight might not be shown completely)
highlight_type << -1;//just lines
get_graph_viewport(gnos[i],&bb1);
    vp.x=bb1.xv1;
    vp.y=bb1.yv1;
tmp_point=VPoint2XPointF(vp);
highlight_segments[i] << tmp_point;
    vp.x=bb1.xv2;
    vp.y=bb1.yv1;
tmp_point=VPoint2XPointF(vp);
highlight_segments[i] << tmp_point;
    vp.x=bb1.xv2;
    vp.y=bb1.yv2;
tmp_point=VPoint2XPointF(vp);
highlight_segments[i] << tmp_point;
    vp.x=bb1.xv1;
    vp.y=bb1.yv2;
tmp_point=VPoint2XPointF(vp);
highlight_segments[i] << tmp_point;
    vp.x=bb1.xv1;
    vp.y=bb1.yv1;
tmp_point=VPoint2XPointF(vp);
highlight_segments[i] << tmp_point;
    if (i==0)
    {
    sd_width=g[gnos[i]].t[0]->t_drawbarlinew;
        for (int j=1;j<MAXAXES;j++)
        {
        if (g[gnos[i]].t[j]->t_drawbarlinew>sd_width) sd_width=g[gnos[i]].t[j]->t_drawbarlinew;
        }
    }
    else
    {
        for (int j=0;j<MAXAXES;j++)
        {
        if (g[gnos[i]].t[j]->t_drawbarlinew>sd_width) sd_width=g[gnos[i]].t[j]->t_drawbarlinew;
        }
    }
}
if (sd_width<1.0) sd_width=1.0;

sd_width*=2.5*GeneralPageZoomFactor;
highlight_width=(int)rint(sd_width)+1;
//cout << "highlight_width=" << highlight_width << endl;
}

void prepare_highlight_segments_with_sets(int n,int * gnos,int * snos,int do_clipping)
{
view bb1;
VPoint vp,vp2;
WPoint wp,wp2;
QPointF tmp_point;
QVector<QPointF> tmp_vector;
QList< QVector<QPointF> > tmp_List;
//Datapoint dpoint;
double sd_width=0.01;
int len;//,ncols;
tmp_List.clear();
highlight_type.clear();
highlight_clipping.clear();
highlight_set.clear();

collect_active=0;//to use the plot-routines to get the plot-lines
nr_of_collect_set_lines=n;//this activates the collecting
if (collect_gnos) delete[] collect_gnos;
if (collect_snos) delete[] collect_snos;
collect_gnos=new int[2+n];
collect_snos=new int[2+n];
memcpy(collect_gnos,gnos,sizeof(int)*n);
memcpy(collect_snos,snos,sizeof(int)*n);

/*qDebug() << "nr_of_collect_set_lines=" << nr_of_collect_set_lines;
for (int i=0;i<nr_of_collect_set_lines;i++)
{
qDebug() << "G" << gnos[i] << ".S" << snos[i];
}*/

collect_segments.clear();
force_redraw_wo_dialog_updates();
nr_of_collect_set_lines=0;//stop collecting

for (int i=0;i<collect_segments.length();i++)
{
    tmp_vector=collect_segments.at(i);
    if (tmp_vector.size()>0)
    {
    tmp_List << tmp_vector;
    //highlight_clipping << gnos[i];//-1
    highlight_type << -1;
    tmp_vector.clear();
    }
}

//qDebug() << "Collected Secments:" << collect_segments.length();

if (highlight_errorbars)
for (int i=0;i<n;i++)
{
    if (!is_valid_setno(gnos[i],snos[i])) continue;
tmp_vector.clear();
get_graph_viewport(gnos[i],&bb1);//get the bounding box of the first graph
len=getsetlength(gnos[i],snos[i]);
if (g[gnos[i]].p[snos[i]].linew>sd_width) sd_width=g[gnos[i]].p[snos[i]].linew;
/*    for (int j=0;j<len;j++)
    {//go through all points in a set to draw lines
    wp.x=g[gnos[i]].p[snos[i]].data.ex[0][j];
    wp.y=g[gnos[i]].p[snos[i]].data.ex[1][j];
    vp=Wpoint2Vpoint(wp);
        if (is_vpoint_inside(bb1,vp,0.0))//visible point
        {
            if (tmp_vector.size()<=0 && j>0)//there is an invisible point before this one
            {
                wp2.x=g[gnos[i]].p[snos[i]].data.ex[0][j-1];
                wp2.y=g[gnos[i]].p[snos[i]].data.ex[1][j-1];
                vp2=Wpoint2Vpoint(wp2);
                tmp_point=VPoint2XPointF(vp2);
                tmp_vector << tmp_point;//add last invisible point before visible points to segment
            }
        tmp_point=VPoint2XPointF(vp);
        tmp_vector << tmp_point;
        }
        else//invisible point
        {
            if (tmp_vector.size()>0)
            {
                tmp_point=VPoint2XPointF(vp);
                tmp_vector << tmp_point;//add first invisible point after visible points to segment
            tmp_List << tmp_vector;
            highlight_clipping << gnos[i];
            tmp_vector.clear();
            }
        }
    }//end go through all points in a set to draw lines
    if (tmp_vector.size()>0)
    {
    tmp_List << tmp_vector;
    highlight_clipping << gnos[i];
    highlight_type << -1;
    tmp_vector.clear();
    }*/
    if (g[gnos[i]].p[snos[i]].sym)
    {
    //qDebug() << "Set Has Symbols";
    for (int j=0;j<len;j+=g[gnos[i]].p[snos[i]].symskip+1)
    {//go through all points in a set to draw points
    wp.x=g[gnos[i]].p[snos[i]].data.ex[0][j];
    wp.y=g[gnos[i]].p[snos[i]].data.ex[1][j];
    vp=Wpoint2Vpoint(wp);
        if (is_vpoint_inside(bb1,vp,0.0) || do_clipping==FALSE)//visible point (without clipping, every point is visible)
        {
            if (tmp_vector.size()<=0 && j>0)//there is an invisible point before this one
            {
                wp2.x=g[gnos[i]].p[snos[i]].data.ex[0][j-1];
                wp2.y=g[gnos[i]].p[snos[i]].data.ex[1][j-1];
                vp2=Wpoint2Vpoint(wp2);
                tmp_point=VPoint2XPointF(vp2);
                tmp_vector << tmp_point;//add last invisible point before visible points to segment
            }
        tmp_point=VPoint2XPointF(vp);
        tmp_vector << tmp_point;
        }
    }//end go through all points in a set to draw points
    if (tmp_vector.size()>0)
    {
    tmp_List << tmp_vector;
    highlight_clipping << gnos[i];
    highlight_type << g[gnos[i]].p[snos[i]].sym;
    highlight_set << snos[i];
    tmp_vector.clear();
    }
    }
}

if (nr_of_alloc_highlight_segm<tmp_List.length())
{
    if (highlight_segments!=NULL)
    {
    delete[] highlight_segments;
    highlight_segments=NULL;
    }
nr_of_alloc_highlight_segm=tmp_List.length();
    if (nr_of_alloc_highlight_segm>0)
    {
    highlight_segments=new QVector<QPointF>[nr_of_alloc_highlight_segm];
    }
}
nr_of_highlight_segments=tmp_List.length();

//VPoint xlibdev2VPoint2(qreal x, qreal y)
/*double t_x,t_y;
VPoint t_vp;*/
//qDebug() << "nr_of_segments=" << nr_of_highlight_segments << "(" << highlight_type.length() << "," << highlight_clipping.length() << "," << highlight_set.length() << ")";
for (int i=0;i<nr_of_highlight_segments;i++)
{
highlight_segments[i]=tmp_List.at(i);
//qDebug() << "segment[" << i << "].length=" << highlight_segments[i].length() << "type=" << highlight_type.at(i) << "clipping=" << highlight_clipping.at(i) << "set=" << highlight_set.at(i);
    /*for (int j=0;j<highlight_segments[i].length();j++)
    {
    t_vp=xlibdev2VPoint2(highlight_segments[i].at(j).x(),highlight_segments[i].at(j).y());
    qDebug() << highlight_segments[i].at(j).x() << " " << highlight_segments[i].at(j).y();
    view2_graph_world(0,(double)t_vp.x,(double)t_vp.y,&t_x,&t_y);
    qDebug() << t_x << " " << t_y;
    }*/
}

if (sd_width<1.0) sd_width=1.0;

sd_width*=2.5*GeneralPageZoomFactor;
highlight_width=(int)rint(sd_width)+1;
//cout << "highlight_width=" << highlight_width << endl;
}

MainWindow::MainWindow( QWidget *parent ):QWidget( parent )
{
    setVisible(false);
    QPixmap HelpPixmap;

    future_load_status=0;
    future_load_file=QString("");
    default_IconW=default_IconH=16;
    defaultSmallButtonW=23;
    defaultSmallButtonH=23;
    defaultLargeButtonW=65;
    defaultLargeButtonH=25;
    defaultListW=65;//60//I changed this to fit the button-width
    defaultListH=68;
    defaultSliderW=65;
#ifdef WINDOWS_SYSTEM
    defaultSliderH=34;
#else
    defaultSliderH=48;
#endif
    defaultBorderGap=2;
    defaultVGap=2;
    defaultToolBarWidth=2*2+2*defaultBorderGap+defaultLargeButtonW;

    /// SocketConnection=NULL;

    setWindowTitle(tr("QtGrace: untitled"));

    windowWidth=872;
    windowHeight=670;
    main_win_gr_no=-1;
    main_win_set_no=-1;
#if QT_VERSION >= 0x050000

#else
    helpMapper=new QSignalMapper();
#endif
    rtiTimer=new QTimer(this);
    connect(rtiTimer,SIGNAL(timeout()),this,SLOT(doRealTimeMonitoring()));
    highlightTimer=new QTimer(this);
    connect(highlightTimer,SIGNAL(timeout()),this,SLOT(highlight_ping()));
    stopHighlightTimer=new QTimer(this);//for maximum time to stop highlighting
    connect(stopHighlightTimer,SIGNAL(timeout()),this,SLOT(highlight_stop_ping()));

    CreateActions();
    CreateIcons();

    setWindowIcon(QIcon(*GraceIcon));

    //setMinimumSize(windowWidth,windowHeight);
    //setMaximumSize(windowWidth,windowHeight);

    //Main menu bar
    menuBar=new QMenuBar(this);
    /// menuBar->setGeometry(0,0,windowWidth,menuBar->height());

    //The File-Menu and its entries
    mnuFile	=new QMenu(tr("&File"), this );
    mnuFile->setTearOffEnabled(TRUE);
    mnuFile->addAction(actNew);
    mnuFile->addAction(actOpen);
    mnuFile->addAction(actSave);
    mnuFile->addAction(actSaveAs);
    mnuFile->addAction(actRevert);
    mnuHistory=new QMenu(tr("Recently opened files"),this);
    mnuHistory->setTearOffEnabled(TRUE);
        for (int i=0;i<MAX_HISTORY;i++)
        mnuHistory->addAction(actHistory[i]);
    mnuHistory->addSeparator();
    mnuHistory->addAction(actClearHistory);
    mnuFile->addMenu(mnuHistory);
    mnuFile->addSeparator();
    mnuFile->addAction(actPrintSetup);
    mnuFile->addAction(actPrintToFile);
    mnuFile->addAction(actPrint);
    //mnuFile->addAction(actPrintSpecial);
#ifndef MAC_SYSTEM
    mnuFile->addSeparator();
#endif
    mnuFile->addAction(actExit);

    //The Edit-Menu and its entries
    mnuEdit=new QMenu(tr("&Edit"),  this );
    mnuEdit->setTearOffEnabled(TRUE);
    mnuEdit->addAction(actUndo);
    mnuEdit->addAction(actRedo);
    mnuEdit->addAction(actUndoList);
    mnuEdit->addSeparator();
    mnuEdit->addAction(actDataSets);
    mnuEdit->addAction(actSetOperations);
    mnuEdit->addSeparator();
    mnuEdit->addAction(actExplorer);
    mnuEdit->addSeparator();
    mnuEdit->addAction(actArrangeGraphs);
    mnuEdit->addAction(actOverlayGraphs);
    mnuEdit->addAction(actAutoscaleGraphs);
    mnuEdit->addSeparator();
    mnuRegions=new QMenu(tr("Reg&ions"),this);
    mnuRegions->setTearOffEnabled(TRUE);
    mnuRegions->addAction(actRegionsStatus);
    mnuRegions->addAction(actRegionsDefine);
    mnuRegions->addAction(actRegionsClear);
    mnuRegions->addSeparator();
    mnuRegions->addAction(actRegionMaster);
    mnuRegions->addSeparator();
    mnuRegions->addAction(actRegionsReportOn);
    mnuEdit->addMenu(mnuRegions);
    mnuEdit->addAction(actHotLinks);
    mnuEdit->addSeparator();
    mnuEdit->addAction(actSetLocFixPoint);
    mnuEdit->addAction(actClearLocFixPoint);
    mnuEdit->addAction(actLocProp);
    mnuEdit->addSeparator();
    mnuEdit->addAction(actPreferences);

    //The Data-Menu and its entries
    mnuData	=new QMenu(tr("&Data"), this );
    mnuData->setTearOffEnabled(TRUE);

    mnuTransform=new QMenu(tr("&Transformations"), this );
    mnuTransform->setTearOffEnabled(TRUE);
    mnuTransform->addAction(actEvaluateExpr);
    mnuTransform->addAction(actHistograms);
    mnuTransform->addAction(actFourier);
    mnuTransform->addAction(actFourier2);
    mnuTransform->addSeparator();
    mnuTransform->addAction(actRunningAverages);
    mnuTransform->addAction(actDifferences);
    mnuTransform->addAction(actSeasonalDiff);
    mnuTransform->addAction(actIntegration);
    mnuTransform->addSeparator();
    mnuTransform->addAction(actInterpolation);
    mnuTransform->addAction(actRegression);
    mnuTransform->addAction(actNonLinCurveFitting);
    mnuTransform->addSeparator();
    mnuTransform->addAction(actCorrelation);
    mnuTransform->addAction(actDigitalFilter);
    mnuTransform->addAction(actLinConvolution);
    mnuTransform->addSeparator();
    mnuTransform->addAction(actGeomTransform);
    mnuTransform->addAction(actGeomEval);
    mnuTransform->addAction(act2DFitting);
    mnuTransform->addSeparator();
    mnuTransform->addAction(actSamplePoints);
    mnuTransform->addAction(actPruneData);
    mnuTransform->addSeparator();
    mnuTransform->addAction(actReportFitParameters);

    mnuImport=new QMenu(tr("&Import"), this );
    mnuImport->setTearOffEnabled(TRUE);
    mnuImport->addAction(actImportAscii);
    mnuImport->addAction(actImportCSV);
    mnuImport->addAction(actImportBinary);
    mnuImport->addAction(actImportNetCDF);
    mnuImport->addAction(actImportAgr);
    mnuExport=new QMenu(tr("&Export"), this );
    mnuExport->setTearOffEnabled(TRUE);
    mnuExport->addAction(actExportAscii);
    mnuExport->addAction(actExportBinary);

    mnuData->addAction(actDataSetOperations);
    mnuData->addMenu(mnuTransform);
    mnuData->addAction(actFeatureExtraction);
    mnuData->addSeparator();
    mnuData->addMenu(mnuImport);
    mnuData->addMenu(mnuExport);

    //The Plo-Menu and its entries
    mnuPlot	=new QMenu(tr("&Plot"), this );
    mnuPlot->setTearOffEnabled(TRUE);
    mnuPlot->addAction(actPlotAppearance);
    mnuPlot->addAction(actGraphAppearance);
    mnuPlot->addAction(actSetAppearance);
    mnuPlot->addAction(actAxisProperties);
    mnuPlot->addSeparator();
    mnuPlot->addAction(actLoadParameters);
    mnuPlot->addAction(actSaveParameters);

    //The View-Menu and its entries
    mnuView	=new QMenu(tr("&View"), this );
    mnuView->setTearOffEnabled(TRUE);
    mnuView->addAction(actShowLocBar);
    mnuView->addAction(actShowStatusBar);
    mnuView->addAction(actShowToolBar);
    mnuView->addSeparator();
    mnuView->addAction(actPageSetup);
    mnuView->addSeparator();
    mnuView->addAction(actRedraw);
    mnuView->addSeparator();
    mnuView->addAction(actUpdateAll);

    //The Window-Menu and its entries
    mnuWindow=new QMenu(tr("&Window"),  this );
    mnuWindow->setTearOffEnabled(TRUE);
    mnuWindow->addAction(actCommands);
    mnuWindow->addAction(actPointExplorer);
    mnuWindow->addAction(actDrawingObjects);
    mnuWindow->addAction(actFontTool);
    mnuWindow->addAction(actConsole);
    //mnuWindow->addAction(actColManager);
    mnuWindow->addAction(actRealTimeInput);

    //The Help-Menu and its entries
    mnuHelp	=new QMenu(tr("&Help"),  this );
    mnuHelp->setTearOffEnabled(TRUE);
    mnuExample=new QMenu(tr("&Examples"), this );
    mnuExample->setTearOffEnabled(TRUE);
        for (int i=0;i<nr_of_Example_Menues;i++)
        mnuExample->addMenu(example_menues[i]);
#if QT_VERSION >= 0x050000

#else
    connect(helpMapper, SIGNAL(mapped(int)),this, SLOT(HelpOpenExample(int)));
#endif
    mnuHelp->addAction(actHelpOnContext);
    mnuHelp->addAction(actHelpUsersGuide);
    mnuHelp->addAction(actHelpTutorial);
    mnuHelp->addAction(actHelpFAQ);
    mnuHelp->addAction(actHelpChanges);
    mnuHelp->addAction(actHelpQtGrace);
    mnuHelp->addAction(actHelpReadme);
    mnuHelp->addSeparator();
    mnuHelp->addMenu(mnuExample);
    mnuHelp->addSeparator();
    mnuHelp->addAction(actHelpComments);
    mnuHelp->addSeparator();
    mnuHelp->addAction(actHelpLicense);
    mnuHelp->addAction(actHelpAbout);

    menuBar->addMenu( mnuFile );
    menuBar->addMenu( mnuEdit );
    menuBar->addMenu( mnuData );
    menuBar->addMenu( mnuPlot );
    menuBar->addMenu( mnuView );
    menuBar->addMenu( mnuWindow );
    menuBar->addSeparator();
    menuBar->addMenu( mnuHelp );

    stdBarHeight=menuBar->height()-7;
    stdRowHeight=218;
    stdColWidth=68;
    stdDistance1=5;
    stdDistance2=6;
    stdHeight1=25;
    stdHeight2=23;
    stdWidth2=23;

    //The status locator bar
    statLocBar=new QLabel(" ",this);
    //statLocBar->setGeometry(0,stdBarHeight,windowWidth,stdBarHeight-8);
    statLocBar->setText(QString("G0: X, Y = [-, -]"));
    statLocBar->setFrameStyle(QFrame::Box | QFrame::Raised);

    //The status bar
    statusBar=new QStatusBar(this);
    //statusBar->setGeometry(0,windowHeight-stdBarHeight,windowWidth,stdBarHeight);
    statusBar->showMessage( "-,-,Untitled" );
    statusBarLabel=new QLabel(" ",this);

    //The tool bar(s)
    toolBar1=new QFrame(this);
    //toolBar1->setGeometry(0,stdBarHeight+statLocBar->height(),70,130);
    toolBar1->setFrameStyle(QFrame::Box | QFrame::Raised);
    //toolBar1->setMinimumHeight(137);

    tool_empty=new QWidget();
    tool_layout=new QVBoxLayout();
    //tool_layout->setMargin(0);
    tool_layout->setContentsMargins(0,0,0,0);
    tool_layout->setSpacing(0);
    scroll=new QScrollArea(this);
    scroll->setWidget(tool_empty);
    tool_empty->setLayout(tool_layout);

#ifdef AUTOLAYOUT_FOR_TOOLBAR
    toolLayout1=new QGridLayout();
    //toolLayout1->setMargin(STD_MARGIN);
    toolLayout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    toolLayout1->setSpacing(STD_SPACING);
    toolLayout2=new QGridLayout();
    //toolLayout2->setMargin(STD_MARGIN);
    toolLayout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    toolLayout2->setSpacing(STD_SPACING);
    toolBar1->setLayout(toolLayout1);
    int toolIndex=0;
#endif

    cmdDraw=new QPushButton(tr("Draw"),toolBar1);
    cmdDraw->setToolTip(tr("Redraw project"));
    connect(cmdDraw, SIGNAL(clicked()), this, SLOT(doDraw()));
    convertBitmapToPixmap(zoomBitMap,&HelpPixmap);
    cmdZoom=new QPushButton(HelpPixmap,"",toolBar1);
    cmdZoom->setToolTip(tr("Zoom graph(s) in rectangle (Alt+Z)"));
    connect(cmdZoom, SIGNAL(clicked()), this, SLOT(doZoom()));
    convertBitmapToPixmap(autoBitMap,&HelpPixmap);
    cmdAutoScale=new QPushButton(HelpPixmap,"",toolBar1);
    cmdAutoScale->setToolTip(tr("Autoscale graph(s) on X and Y axis (Crtl+A)"));
    connect(cmdAutoScale, SIGNAL(clicked()), this, SLOT(doAutoScale()));
    convertBitmapToPixmap(expandBitMap,&HelpPixmap);
    cmdZz=new QPushButton(HelpPixmap,"",toolBar1);
    cmdZz->setToolTip(tr("Zoom out (Crtl -)"));
    connect(cmdZz, SIGNAL(clicked()), this, SLOT(doZz()));
    convertBitmapToPixmap(shrinkBitMap,&HelpPixmap);
    cmdzz=new QPushButton(HelpPixmap,"",toolBar1);
    cmdzz->setToolTip(tr("Zoom in (Crtl +)"));
    connect(cmdzz, SIGNAL(clicked()), this, SLOT(dozz()));
    convertBitmapToPixmap(leftBitMap,&HelpPixmap);
    cmdLeft=new QPushButton(HelpPixmap,"",toolBar1);
    cmdLeft->setToolTip(tr("Scroll graph(s) left (Crtl+LeftArrow)"));
    connect(cmdLeft, SIGNAL(clicked()), this, SLOT(doLeft()));
    convertBitmapToPixmap(rightBitMap,&HelpPixmap);
    cmdRight=new QPushButton(HelpPixmap,"",toolBar1);
    cmdRight->setToolTip(tr("Scroll graph(s) right (Crtl+RightArrow)"));
    connect(cmdRight, SIGNAL(clicked()), this, SLOT(doRight()));
    convertBitmapToPixmap(upBitMap,&HelpPixmap);
    cmdUp=new QPushButton(HelpPixmap,"",toolBar1);
    cmdUp->setToolTip(tr("Scroll graph(s) up (Crtl+UpArrow)"));
    connect(cmdUp, SIGNAL(clicked()), this, SLOT(doUp()));
    convertBitmapToPixmap(downBitMap,&HelpPixmap);
    cmdDown=new QPushButton(HelpPixmap,"",toolBar1);
    cmdDown->setToolTip(tr("Scroll graph(s) down (Crtl+DownArrow)"));
    connect(cmdDown, SIGNAL(clicked()), this, SLOT(doDown()));
    /*QtGrace-addition*/
    QCursor helpCursor(Qt::OpenHandCursor);
    HelpPixmap=helpCursor.pixmap();
    cmdPan=new QPushButton(HelpPixmap,"Pan",toolBar1);
    cmdPan->setToolTip(tr("Pan graph(s) (Alt+P)"));
    connect(cmdPan,SIGNAL(clicked(bool)),SLOT(doPan()));
    cmdPick=new QPushButton(HelpPixmap,"Move",toolBar1);
    cmdPick->setToolTip(tr("Relocate any object, legend, title, label (Alt+M)"));
    connect(cmdPick,SIGNAL(clicked(bool)),SLOT(doPick()));

    //chkSyncZoom=new QCheckBox(tr("Sync."),toolBar1);
    //chkSyncZoom->setGeometry(cmdDraw->x()+stdDistance2,cmdUp->y()+cmdUp->height()+stdDistance1,64,stdHeight1);

    ///QFont helpFont1(qApp->font());
    ///helpFont1.setPixelSize(12);
    lstGraphs=new uniList(GRAPHLIST,toolBar1);
    lstGraphs->setToolTip(tr("Select graph(s) for zoom operations"));
    ///lstGraphs->setFont(helpFont1);
    lstGraphs->prevent_from_autoupdate=true;
    lstGraphs->minimum_display=true;
    lstGraphs->setBehavior(true,true,true);
    lstGraphs->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    lstGraphs->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(lstGraphs,SIGNAL(new_selection(int)),this,SLOT(newgraphselection(int)));
    /*End: QtGrace-addition*/

#ifdef AUTOLAYOUT_FOR_TOOLBAR
    toolLayout1->addWidget(cmdDraw,toolIndex++,0,1,2);
    toolLayout1->addWidget(cmdZoom,toolIndex,0,1,1);
    toolLayout1->addWidget(cmdAutoScale,toolIndex++,1,1,1);
    toolLayout1->addWidget(cmdZz,toolIndex,0,1,1);
    toolLayout1->addWidget(cmdzz,toolIndex++,1,1,1);
    toolLayout1->addWidget(cmdLeft,toolIndex,0,1,1);
    toolLayout1->addWidget(cmdRight,toolIndex++,1,1,1);
    toolLayout1->addWidget(cmdUp,toolIndex,0,1,1);
    toolLayout1->addWidget(cmdDown,toolIndex++,1,1,1);
    toolLayout1->addWidget(cmdPan,toolIndex++,0,1,2);
    toolLayout1->addWidget(cmdPick,toolIndex++,0,1,2);
    toolLayout1->addWidget(lstGraphs,toolIndex++,0,1,2);
#else
    #ifdef WINDOWS_SYSTEM
        cmdDraw->setGeometry(2+WIN_SIZE_CORR*toolBarSizeFactor,4,64+WIN_SIZE_CORR*toolBarSizeFactor,stdHeight1);
    #else
        cmdDraw->setGeometry(2,4,64,stdHeight1);
    #endif
    cmdZoom->setGeometry(cmdDraw->x()+stdDistance2,cmdDraw->y()+cmdDraw->height()+6,stdWidth2,stdHeight2);
    cmdAutoScale->setGeometry(cmdDraw->x()+cmdZoom->width()+2*stdDistance2,cmdZoom->y(),stdWidth2,stdHeight2);
    cmdZz->setGeometry(cmdDraw->x()+stdDistance2,cmdZoom->y()+cmdZoom->height()+stdDistance1,stdWidth2,stdHeight2);
    cmdzz->setGeometry(cmdDraw->x()+cmdZoom->width()+2*stdDistance2,cmdZz->y(),stdWidth2,stdHeight2);
    cmdLeft->setGeometry(cmdDraw->x()+stdDistance2,cmdZz->y()+cmdZz->height()+stdDistance1,stdWidth2,stdHeight2);
    cmdRight->setGeometry(cmdDraw->x()+cmdZoom->width()+2*stdDistance2,cmdLeft->y(),stdWidth2,stdHeight2);
    cmdUp->setGeometry(cmdDraw->x()+stdDistance2,cmdLeft->y()+cmdLeft->height()+stdDistance1,stdWidth2,stdHeight2);
    cmdDown->setGeometry(cmdDraw->x()+cmdZoom->width()+2*stdDistance2,cmdUp->y(),stdWidth2,stdHeight2);
    cmdPan->setGeometry(cmdDraw->x(),cmdUp->y()+cmdUp->height()+stdDistance1,cmdDraw->width(),cmdDraw->height());
    cmdPick->setGeometry(cmdPan->x(),cmdPan->y()+cmdPan->height()+stdDistance1,cmdPan->width(),cmdPan->height());
    lstGraphs->setGeometry(cmdDraw->x()+stdDistance2-4,cmdUp->y()+cmdUp->height()+stdDistance1,60,68);//60x68 or 62x68
#endif


    /*tool1Grid=new QGridLayout();
    //tool1Grid->setMargin(2);
    tool1Grid->setContentsMargins(2,2,2,2);
    tool1Grid->setSpacing(2);

    toolBar1->setLayout(tool1Grid);*/

    toolBar2=new QFrame(this);
    //toolBar2->setGeometry(0,toolBar1->y()+toolBar1->height(),toolBar1->width(),windowHeight-stdBarHeight*2-statLocBar->height()-toolBar1->height());
    toolBar2->setFrameStyle(QFrame::Box | QFrame::Raised);

    cmdAutoT=new QPushButton("AutoT",toolBar2);
    cmdAutoT->setToolTip(tr("Automatically set the tick spacing"));
    connect(cmdAutoT, SIGNAL(clicked()), this, SLOT(doAutoT()));
    cmdAutoO=new QPushButton("AutoO",toolBar2);
    cmdAutoO->setToolTip(tr("Autoscale on nearest set"));
    connect(cmdAutoO, SIGNAL(clicked()), this, SLOT(doAutoO()));
    cmdZX=new QPushButton("ZX",toolBar2);
    cmdZX->setToolTip(tr("Zoom X axis only"));
    connect(cmdZX, SIGNAL(clicked()), this, SLOT(doZX()));
    cmdZY=new QPushButton("ZY",toolBar2);
    cmdZY->setToolTip(tr("Zoom Y axis only"));
    connect(cmdZY, SIGNAL(clicked()), this, SLOT(doZY()));
    cmdAX=new QPushButton("AX",toolBar2);
    cmdAX->setToolTip(tr("Autoscale X axis only"));
    connect(cmdAX, SIGNAL(clicked()), this, SLOT(doAX()));
    cmdAY=new QPushButton("AY",toolBar2);
    cmdAY->setToolTip(tr("Autoscale Y axis only"));
    connect(cmdAY, SIGNAL(clicked()), this, SLOT(doAY()));
    cmdPZ=new QPushButton("PZ",toolBar2);
    cmdPZ->setToolTip(tr("Push viewport on stack and zoom"));
    connect(cmdPZ, SIGNAL(clicked()), this, SLOT(doPZ()));
    cmdPu=new QPushButton("Pu",toolBar2);
    cmdPu->setToolTip(tr("Push viewort on stack"));
    connect(cmdPu, SIGNAL(clicked()), this, SLOT(doPu()));
    cmdPo=new QPushButton("Po",toolBar2);
    cmdPo->setToolTip(tr("Pop viewport stack"));
    connect(cmdPo, SIGNAL(clicked()), this, SLOT(doPo()));
    cmdCy=new QPushButton("Cy",toolBar2);
    cmdCy->setToolTip(tr("Cycle viewport stack"));
    connect(cmdCy, SIGNAL(clicked()), this, SLOT(doCy()));
    lblSD=new QLabel(" SD:1",toolBar2);
    lblSD->setToolTip(tr("Viewport stack depth"));
    lblCW=new QLabel(" CW:0",toolBar2);
    lblCW->setToolTip(tr("Current stack position"));

    /*alternative Viewport stack*/
    lblNewViewStack=new QLabel(tr("Viewport\nstack:"),toolBar2);
    lblNewViewStack->setAlignment(Qt::AlignCenter);
    QString entr[2];
    entr[0]=entr[1]=tr("Default");
    cmbViewStack=new QComboBox(toolBar2);
    cmbViewStack->setToolTip(tr("Select viewport from stack"));
    cmbViewStack->addItem(entr[0]);
    convertBitmapToPixmap(upBitMap,&HelpPixmap);
    cmdViewUp=new QPushButton(HelpPixmap,"",toolBar2);
    convertBitmapToPixmap(downBitMap,&HelpPixmap);
    cmdViewDown=new QPushButton(HelpPixmap,"",toolBar2);
    cmdViewUp->setToolTip(tr("Use next viewport lower in stack"));
    cmdViewDown->setToolTip(tr("Use next viewport higher in stack"));
    cmdViewRename=new QPushButton(tr("Name"),toolBar2);
    cmdViewAdd=new QPushButton(tr("+"),toolBar2);
    cmdViewRemove=new QPushButton(tr("-"),toolBar2);
    cmdViewRename->setToolTip(tr("Rename currently selected viewport in stack"));
    cmdViewAdd->setToolTip(tr("Add current world to viewport stack"));
    cmdViewRemove->setToolTip(tr("Remove current viewport from stack"));
    cmdViewReplace=new QPushButton(tr("Repl."),toolBar2);
    cmdViewRearrange=new QPushButton(tr("Pos."),toolBar2);
    cmdViewReplace->setToolTip(tr("Replace current stack-viewport with graph-viewport"));
    cmdViewRearrange->setToolTip(tr("Reposition selected viewport in stack"));

    connect(cmdViewUp,SIGNAL(clicked()),SLOT(newViewportUp()));
    connect(cmdViewDown,SIGNAL(clicked()),SLOT(newViewportDown()));
    connect(cmdViewRename,SIGNAL(clicked()),SLOT(newViewportRename()));
    connect(cmdViewAdd,SIGNAL(clicked()),SLOT(newViewportAdd()));
    connect(cmdViewRemove,SIGNAL(clicked()),SLOT(newViewportRemove()));
    connect(cmdViewReplace,SIGNAL(clicked()),SLOT(newViewportReplace()));
    connect(cmdViewRearrange,SIGNAL(clicked()),SLOT(newViewportReposition()));
    connect(cmbViewStack,SIGNAL(currentIndexChanged(int)),SLOT(newViewportSelected(int)));

    /*QtGrace-addition*/
    cmdUndo=new QPushButton(tr("Undo"),toolBar2);
    cmdUndo->setToolTip(tr("Undo last action"));
    connect(cmdUndo,SIGNAL(clicked(bool)),SLOT(doUndo()));
    cmdRedo=new QPushButton(tr("Redo"),toolBar2);
    cmdRedo->setToolTip(tr("Redo next action (undo the last undo)"));
    connect(cmdRedo,SIGNAL(clicked(bool)),SLOT(doRedo()));
    sldPageZoom=new stdSlider(toolBar2,QString(""),-100,100,0.01,SLIDE_LOGARITHMIC);
    sldPageZoom->setToolTip(tr("Page zoom"));
    connect(sldPageZoom,SIGNAL(valueChanged(int)),this,SLOT(doPageZoom(int)));
    cmdFitPage=new QPushButton(tr("Fit"),toolBar2);
    cmdFitPage->setToolTip(tr("Fit page size to window size"));
    connect(cmdFitPage, SIGNAL(clicked()), this, SLOT(doFitPage()));
    cmdExport=new QPushButton(tr("Export"),toolBar2);
    cmdExport->setToolTip(tr("Export to file"));
    connect(cmdExport, SIGNAL(clicked()), this, SLOT(PrintToFile()));
    cmdPrint=new QPushButton(tr("Print"),toolBar2);
    cmdPrint->setToolTip(tr("Print on physical printer"));
    connect(cmdPrint, SIGNAL(clicked()), this, SLOT(Print()));
    cmdExport->hide();
    cmdPrint->hide();
    /*End: QtGrace-addition*/
    cmdExit=new QPushButton(tr("Exit"),toolBar2);
    cmdExit->setToolTip(tr("Close QtGrace"));
    //cmdExit->setGeometry(cmdAutoO->x(),lblCW->y()+lblCW->height()+2,cmdAutoO->width(),cmdAutoO->height());
    //cmdExit->setGeometry(cmdAutoO->x(),sldPageZoom->y()+cmdAutoO->height()+5,cmdAutoO->width(),cmdAutoO->height());
    connect(cmdExit, SIGNAL(clicked()), this, SLOT(doExit()));

#ifdef AUTOLAYOUT_FOR_TOOLBAR
    toolBar2->setLayout(toolLayout2);
    toolIndex=0;
    toolLayout2->addWidget(cmdAutoT,toolIndex++,0,1,2);
    toolLayout2->addWidget(cmdAutoO,toolIndex++,0,1,2);
    toolLayout2->addWidget(cmdZX,toolIndex,0,1,1);
    toolLayout2->addWidget(cmdZY,toolIndex++,1,1,1);
    toolLayout2->addWidget(cmdAX,toolIndex,0,1,1);
    toolLayout2->addWidget(cmdAY,toolIndex++,1,1,1);
    //classic viewport-stack
    toolLayout2->addWidget(cmdPZ,toolIndex,0,1,1);
    toolLayout2->addWidget(cmdPu,toolIndex++,1,1,1);
    toolLayout2->addWidget(cmdPo,toolIndex,0,1,1);
    toolLayout2->addWidget(cmdCy,toolIndex++,1,1,1);
    toolLayout2->addWidget(lblSD,toolIndex++,0,1,2);
    toolLayout2->addWidget(lblCW ,toolIndex++,0,1,2);
    //new viewport-stack
    toolLayout2->addWidget(lblNewViewStack,toolIndex++,0,1,2);
    toolLayout2->addWidget(cmbViewStack,toolIndex++,0,1,2);
    toolLayout2->addWidget(cmdViewUp,toolIndex,0,1,1);
    toolLayout2->addWidget(cmdViewDown,toolIndex++,1,1,1);
    toolLayout2->addWidget(cmdViewAdd,toolIndex,0,1,1);
    toolLayout2->addWidget(cmdViewRemove,toolIndex++,1,1,1);
    toolLayout2->addWidget(cmdViewReplace,toolIndex++,0,1,2);
    toolLayout2->addWidget(cmdViewRearrange,toolIndex++,0,1,2);
    toolLayout2->addWidget(cmdViewRename,toolIndex++,0,1,2);
    //the rest
    toolLayout2->addWidget(cmdUndo,toolIndex,0,1,1);
    toolLayout2->addWidget(cmdRedo,toolIndex++,1,1,1);
    toolLayout2->addWidget(sldPageZoom ,toolIndex++,0,1,2);
    toolLayout2->addWidget(cmdFitPage,toolIndex++,0,1,2);
    toolLayout2->addWidget(cmdExport,toolIndex++,0,1,2);
    toolLayout2->addWidget(cmdPrint,toolIndex++,0,1,2);
    tool_empty2=new QWidget(toolBar2);
    tool_empty2->setMinimumHeight(8);
    toolLayout2->addWidget(tool_empty2,toolIndex++,0,1,2);
    toolLayout2->addWidget(cmdExit,toolIndex++,0,1,2);
#else
    #ifdef WINDOWS_SYSTEM
        cmdAutoT->setGeometry(2+WIN_SIZE_CORR*toolBarSizeFactor,2+2*WIN_SIZE_CORR*toolBarSizeFactor,64+WIN_SIZE_CORR*toolBarSizeFactor,stdHeight1);
    #else
        cmdAutoT->setGeometry(2,2,64,stdHeight1);
    #endif
    cmdAutoO->setGeometry(cmdAutoT->x(),cmdAutoT->y()+cmdAutoT->height()+2,cmdAutoT->width(),cmdAutoT->height());
    cmdZX->setGeometry(cmdAutoO->x()+stdDistance2,cmdAutoO->y()+cmdAutoO->height()+4,stdWidth2,stdHeight2);
    cmdZY->setGeometry(cmdZX->x()+cmdZX->width()+stdDistance2,cmdZX->y(),stdWidth2,stdHeight2);
    cmdAX->setGeometry(cmdZX->x(),cmdZX->y()+cmdZX->height()+stdDistance1,cmdZX->width(),cmdZX->height());
    cmdAY->setGeometry(cmdZY->x(),cmdAX->y(),cmdZX->width(),cmdZX->height());
    cmdPZ->setGeometry(cmdZX->x(),cmdAX->y()+cmdAX->height()+stdDistance1,cmdZX->width(),cmdZX->height());
    cmdPu->setGeometry(cmdZY->x(),cmdPZ->y(),cmdZX->width(),cmdZX->height());
    cmdPo->setGeometry(cmdZX->x(),cmdPZ->y()+cmdPZ->height()+stdDistance1,cmdZX->width(),cmdZX->height());
    cmdCy->setGeometry(cmdZY->x(),cmdPo->y(),cmdZX->width(),cmdZX->height());
    lblSD->setGeometry(cmdDraw->x(),cmdCy->y()+cmdCy->height()+stdDistance1,cmdDraw->width(),cmdDraw->height()-6);
    lblCW->setGeometry(lblSD->x(),lblSD->y()+lblSD->height(),lblSD->width(),lblSD->height());
    //sldPageZoom->setGeometry(cmdAutoO->x(),lblCW->y()+lblCW->height()+12,cmdAutoO->width(),cmdAutoO->height()+5);
    sldPageZoom->setGeometry(cmdAutoO->x(),lblCW->y()+lblCW->height()+12,defaultSliderW,defaultSliderH);
    cmdFitPage->setGeometry(cmdAutoO->x(),sldPageZoom->y()+sldPageZoom->height()+5,cmdAutoO->width(),cmdAutoO->height());
    cmdExport->setGeometry(0,0,cmdAutoT->width(),cmdAutoT->height());
    cmdPrint->setGeometry(0,0,cmdAutoT->width(),cmdAutoT->height());
    cmdExit->setGeometry(cmdAutoO->x(),cmdFitPage->y()+cmdAutoO->height()+14,cmdAutoO->width(),cmdAutoO->height()+5);
#endif

    //The main drawing area
    mainArea=new MainArea(this);
    mainArea->setGeometry(toolBar1->width(),stdBarHeight+statLocBar->height(),windowWidth-toolBar1->width(),windowHeight-statusBar->height()-stdBarHeight-statLocBar->height());
    mainArea->show();

    mainGrid=new QGridLayout();
    //mainGrid->setMargin(2);
    mainGrid->setContentsMargins(2,2,2,2);
    mainGrid->setSpacing(0);
#ifndef MAC_SYSTEM
    mainGrid->addWidget(menuBar,0,0,1,2);
#endif
    /// mainGrid->setRowMinimumHeight(0,stdBarHeight);
    mainGrid->setRowStretch(0,0);
    mainGrid->addWidget(statLocBar,1,0,1,2);
    mainGrid->setRowMinimumHeight(1,stdBarHeight);
    mainGrid->setRowStretch(1,0);

    /*mainGrid->addWidget(toolBar1,2,0,1,1);
    mainGrid->addWidget(toolBar2,3,0,1,1);
    scroll->setVisible(false);*/

    tool_layout->addWidget(toolBar1);
    tool_layout->addWidget(toolBar2);
    tool_layout->setStretch(0,0);
    tool_layout->setStretch(1,1);

    toolBar1->setMinimumWidth(stdColWidth);
//qDebug() << "stdColWidth=" << stdColWidth;
    ///toolBar1->setMaximumWidth(stdColWidth);//deactivated for v0.2.6
    toolBar2->setMinimumWidth(stdColWidth);
    /*toolBar1->setMinimumHeight(stdRowHeight);
    toolBar1->setMaximumHeight(stdRowHeight);*/

    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //scroll->setGeometry(0,0,stdColWidth,600);
    scroll->setMaximumWidth(stdColWidth+2);
    tool_empty->setGeometry(0,0,stdColWidth,600);
    mainGrid->addWidget(scroll,2,0,2,1);

    mainGrid->addWidget(mainArea,2,1,2,1);
    mainGrid->setRowStretch(2,0);
    mainGrid->setRowStretch(3,1);
    mainGrid->addWidget(statusBar,4,0,1,2);
    statusBar->insertPermanentWidget(0, statusBarLabel);
    mainGrid->setRowMinimumHeight(4,stdBarHeight);
    mainGrid->setRowStretch(4,0);
    mainGrid->setColumnMinimumWidth(0,stdColWidth);
    mainGrid->setColumnStretch(0,0);
    mainGrid->setColumnStretch(1,1);
    mainGrid->setRowMinimumHeight(2,stdRowHeight);

    mainGrid->setRowStretch(0,0);
    mainGrid->setRowStretch(1,0);
    mainGrid->setRowStretch(2,0);
    mainGrid->setRowStretch(3,1);
    mainGrid->setRowStretch(4,0);
#ifdef MAC_SYSTEM
    mainGrid->removeWidget(menuBar);
    mainGrid->setRowMinimumHeight(0,0);
#endif
    setLayout(mainGrid);
    //if (use_new_icons==true) redisplayIcons();
    redisplayIcons();
    hide();
#if QT_VERSION >= 0x050000
    last_click_timestamp=0;
#endif
}

MainWindow::~MainWindow()
{
    qDebug() << "deleteMainWin A";
    for (int i=0;i<MAXPATTERNS;i++)
        delete patterns[i];

    delete autoBitMap;
    delete downBitMap;
    delete expandBitMap;
    delete leftBitMap;
    delete rightBitMap;
    delete shrinkBitMap;
    delete upBitMap;
    delete zoomBitMap;
    delete redrawMap;
    delete autoMap;
    delete zoomXMap;
    delete zoomYMap;
    delete autoXMap;
    delete autoYMap;
    delete autoTickMap;
    delete exitMap;
    delete GraceIcon;

    deleteIcons();

    for (int i=0;i<nr_of_current_linestyles;i++)
    {
        delete PenDashPattern[i];
        delete LineIcons[i];
        delete LinePixmaps[i];
    }
    for (int i=0;i<allocated_colors;i++)
    {
        delete ColorIcons[i];
        delete ColorPixmaps[i];
        delete ColorNames[i];
    }

    delete wait_cursor;
    delete line_cursor;
    delete find_cursor;
    delete move_cursor;
    delete text_cursor;
    delete kill_cursor;
    /// if (SocketConnection) delete SocketConnection;

    if (enableServerMode)
    {
    /*fprintf(stderr, "qtgrace error");
    fflush(stderr);*/
    cerr << QObject::tr("qtgrace error").toLocal8Bit().constData() << endl;
    }
    qDebug() << "deleteMainWin B";
}

void MainWindow::deleteIcons(void)
{
N_DELETE(Qt_j_lm_o_bits);
N_DELETE(Qt_j_cm_o_bits);
N_DELETE(Qt_j_rm_o_bits);
N_DELETE(Qt_j_lb_b_bits);
N_DELETE(Qt_j_lm_b_bits);
N_DELETE(Qt_j_lt_b_bits);
N_DELETE(Qt_j_ct_b_bits);
N_DELETE(Qt_j_rt_b_bits);
N_DELETE(Qt_j_rm_b_bits);
N_DELETE(Qt_j_rb_b_bits);
N_DELETE(Qt_j_cb_b_bits);
N_DELETE(Qt_j_cm_b_bits);
N_DELETE(Qt_j_left_bits);
N_DELETE(Qt_j_right_bits);
N_DELETE(Qt_j_center_bits);
N_DELETE(Qt_m_hv_lr_bt_bits);
N_DELETE(Qt_m_hv_lr_tb_bits);
N_DELETE(Qt_m_hv_rl_bt_bits);
N_DELETE(Qt_m_hv_rl_tb_bits);
N_DELETE(Qt_m_vh_lr_bt_bits);
N_DELETE(Qt_m_vh_lr_tb_bits);
N_DELETE(Qt_m_vh_rl_bt_bits);
N_DELETE(Qt_m_vh_rl_tb_bits);
}

void MainWindow::redisplayIcons(void)
{
    QSize newIconSize(default_IconW*toolBarSizeFactor,default_IconH*toolBarSizeFactor);
    QString icondir=QString(qt_grace_icons_dir);//grace_path("fonts/icons");
    icondir+=QDir::separator();
    QIcon HelpIcon;
    QIcon pan_icon=CreateIconFromPNG(icondir+"Pan_Icon.png",newIconSize);
//cout << "redisplay: new=" << use_new_icons << endl;
if (use_new_icons==false)
{
convertBitmapToScaledIcon(zoomBitMap,&HelpIcon,newIconSize);
cmdZoom->setIcon(HelpIcon);
convertBitmapToScaledIcon(autoBitMap,&HelpIcon,newIconSize);
cmdAutoScale->setIcon(HelpIcon);
convertBitmapToScaledIcon(expandBitMap,&HelpIcon,newIconSize);
cmdZz->setIcon(HelpIcon);
convertBitmapToScaledIcon(shrinkBitMap,&HelpIcon,newIconSize);
cmdzz->setIcon(HelpIcon);
convertBitmapToScaledIcon(leftBitMap,&HelpIcon,newIconSize);
cmdLeft->setIcon(HelpIcon);
convertBitmapToScaledIcon(rightBitMap,&HelpIcon,newIconSize);
cmdRight->setIcon(HelpIcon);
convertBitmapToScaledIcon(upBitMap,&HelpIcon,newIconSize);
cmdUp->setIcon(HelpIcon);
cmdViewUp->setIcon(HelpIcon);
convertBitmapToScaledIcon(downBitMap,&HelpIcon,newIconSize);
cmdDown->setIcon(HelpIcon);
cmdViewDown->setIcon(HelpIcon);
}
else
{
cmdZoom->setIcon(CreateScaledIconFromPNG(icondir+"zoom.png",newIconSize));
cmdAutoScale->setIcon(CreateScaledIconFromPNG(icondir+"autoscale.png",newIconSize));
cmdZz->setIcon(CreateScaledIconFromPNG(icondir+"zoomOut.png",newIconSize));
cmdzz->setIcon(CreateScaledIconFromPNG(icondir+"zoomIn.png",newIconSize));
cmdLeft->setIcon(CreateScaledIconFromPNG(icondir+"goLeft.png",newIconSize));
cmdRight->setIcon(CreateScaledIconFromPNG(icondir+"goRight.png",newIconSize));
cmdUp->setIcon(CreateScaledIconFromPNG(icondir+"goUp.png",newIconSize));
cmdViewUp->setIcon(CreateScaledIconFromPNG(icondir+"goUp.png",newIconSize));
cmdDown->setIcon(CreateScaledIconFromPNG(icondir+"goDown.png",newIconSize));
cmdViewDown->setIcon(CreateScaledIconFromPNG(icondir+"goDown.png",newIconSize));
}

if (icon_preferences==0 || icon_preferences==1)//text only or text+icons
{
cmdDraw->setText(tr("Draw"));
cmdZX->setText(tr("ZX"));
cmdZY->setText(tr("ZY"));
cmdAX->setText(tr("AX"));
cmdAY->setText(tr("AY"));
cmdAutoT->setText(tr("AutoT"));
cmdExit->setText(tr("Exit"));
cmdPan->setText(tr("Pan"));
cmdPick->setText(tr("Move"));
/*cmdUndo->setText(tr("Undo"));
cmdRedo->setText(tr("Redo"));*/
cmdZX->setIcon(QIcon());
cmdZY->setIcon(QIcon());
cmdAX->setIcon(QIcon());
cmdAY->setIcon(QIcon());
cmdUndo->setIcon(QIcon());
cmdRedo->setIcon(QIcon());
    if(icon_preferences==0)//text only
    {
    cmdAutoT->setIcon(QIcon());
    cmdDraw->setIcon(QIcon());
    cmdExit->setIcon(QIcon());
    cmdPan->setIcon(QIcon());
    cmdPick->setIcon(QIcon());
    }
    else//text+icons
    {
    cmdPan->setIcon(pan_icon);
    pan_icon=CreateIconFromPNG(icondir+"Pick_Move_Icon.png",newIconSize);
    cmdPick->setIcon(pan_icon);
    convertBitmapToScaledIcon(redrawMap,&HelpIcon,newIconSize);
    cmdDraw->setIcon(HelpIcon);
    convertBitmapToScaledIcon(autoTickMap,&HelpIcon,newIconSize);
    cmdAutoT->setIcon(HelpIcon);
    convertBitmapToScaledIcon(exitMap,&HelpIcon,newIconSize);
    cmdExit->setIcon(HelpIcon);
    }
}
else if (icon_preferences==2)//icons only
{
cmdDraw->setText(tr(""));
cmdPan->setText(tr(""));
cmdPick->setText(tr(""));
cmdZX->setText(tr(""));
cmdZY->setText(tr(""));
cmdAX->setText(tr(""));
cmdAY->setText(tr(""));
cmdExit->setText(tr(""));
cmdAutoT->setText(tr(""));
cmdPan->setIcon(pan_icon);
pan_icon=CreateIconFromPNG(icondir+"Pick_Move_Icon.png",newIconSize);
cmdPick->setIcon(pan_icon);
convertBitmapToScaledIcon(redrawMap,&HelpIcon,newIconSize);
cmdDraw->setIcon(HelpIcon);
convertBitmapToScaledIcon(zoomXMap,&HelpIcon,newIconSize);
cmdZX->setIcon(HelpIcon);
convertBitmapToScaledIcon(zoomYMap,&HelpIcon,newIconSize);
cmdZY->setIcon(HelpIcon);
convertBitmapToScaledIcon(autoXMap,&HelpIcon,newIconSize);
cmdAX->setIcon(HelpIcon);
convertBitmapToScaledIcon(autoYMap,&HelpIcon,newIconSize);
cmdAY->setIcon(HelpIcon);
convertBitmapToScaledIcon(autoTickMap,&HelpIcon,newIconSize);
cmdAutoT->setIcon(HelpIcon);
convertBitmapToScaledIcon(exitMap,&HelpIcon,newIconSize);
cmdExit->setIcon(HelpIcon);
}
cmdUndo->setText(tr(""));
cmdRedo->setText(tr(""));
pan_icon=CreateIconFromPNG(icondir+"Undo_Icon.png",newIconSize);
cmdUndo->setIcon(pan_icon);
pan_icon=CreateIconFromPNG(icondir+"Redo_Icon.png",newIconSize);
cmdRedo->setIcon(pan_icon);
}

void MainWindow::CreateIcons(void)
{
    //generate Patterns for filling styles
    /*for (int i=0;i<MAXPATTERNS;i++)
    {
        if (patterns[i]!=NULL) delete patterns[i];
            patterns[i]=new QBitmap(16,16);
    }
    init_Patterns();*/

    autoBitMap=new QBitmap();
    *autoBitMap=generate_Bitmap_from_Bits(auto_bits,auto_width*auto_height,auto_width,auto_height);
    downBitMap=new QBitmap();
    *downBitMap=generate_Bitmap_from_Bits(down_bits,down_width*down_height,down_width,down_height);
    expandBitMap=new QBitmap();
    *expandBitMap=generate_Bitmap_from_Bits(expand_bits,expand_width*expand_height,expand_width,expand_height);
    leftBitMap=new QBitmap();
    *leftBitMap=generate_Bitmap_from_Bits(left_bits,left_width*left_height,left_width,left_height);
    rightBitMap=new QBitmap();
    *rightBitMap=generate_Bitmap_from_Bits(right_bits,right_width*right_height,right_width,right_height);
    shrinkBitMap=new QBitmap();
    *shrinkBitMap=generate_Bitmap_from_Bits(shrink_bits,shrink_width*shrink_height,shrink_width,shrink_height);
    upBitMap=new QBitmap();
    *upBitMap=generate_Bitmap_from_Bits(up_bits,up_width*up_height,up_width,up_height);
    zoomBitMap=new QBitmap();
    *zoomBitMap=generate_Bitmap_from_Bits(zoom_bits,zoom_width*zoom_height,zoom_width,zoom_height);

    redrawMap=new QBitmap();
    *redrawMap=generate_Bitmap_from_Bits(redraw_bits,redraw_width*redraw_height,redraw_width,redraw_height);
    zoomXMap=new QBitmap();
    *zoomXMap=generate_Bitmap_from_Bits(zoom_x_bits,zoom_x_width*zoom_x_height,zoom_x_width,zoom_x_height);
    zoomYMap=new QBitmap();
    *zoomYMap=generate_Bitmap_from_Bits(zoom_y_bits,zoom_y_width*zoom_y_height,zoom_y_width,zoom_y_height);
    autoMap=new QBitmap();
    *autoMap=generate_Bitmap_from_Bits(auto_bits2,auto2_width*auto2_height,auto2_width,auto2_height);
    autoXMap=new QBitmap();
    *autoXMap=generate_Bitmap_from_Bits(auto_x_bits,auto_x_width*auto_x_height,auto_x_width,auto_x_height);
    autoYMap=new QBitmap();
    *autoYMap=generate_Bitmap_from_Bits(auto_y_bits,auto_y_width*auto_y_height,auto_y_width,auto_y_height);
    autoTickMap=new QBitmap();
    *autoTickMap=generate_Bitmap_from_Bits(auto_tick_bits,auto_tick_width*auto_tick_height,auto_tick_width,auto_tick_height);
    exitMap=new QBitmap();
    *exitMap=generate_Bitmap_from_Bits(exit_bits,exit_width*exit_height,exit_width,exit_height);

    //Read Grace-icon-image
    /*GraceIcon=new QPixmap(16,16);
    ActiveIcon=new QPixmap(16,16);
    HiddenIcon=new QPixmap(16,16);
    char dummy1[80],dummy2[80],dummy3[80];
    unsigned int rr,gg,bb;
    char dummy[800];
    int c_width,c_height,nr_cols,bpc,c_read;

    c_read=sscanf(grace_icon_xpm[0],"%d %d %d %d",&c_width,&c_height,&nr_cols,&bpc);
    QImage img1(c_width,c_height,QImage::Format_ARGB32_Premultiplied);
    QColor * cols=new QColor[nr_cols];
    char * ctrl_char=new char[nr_cols];
    for (int i=0;i<nr_cols;i++)
    {
        c_read=sscanf(grace_icon_xpm[1+i],"%c c #%s",ctrl_char+i,dummy);
        dummy1[0]=dummy2[0]=dummy3[0]='0';
        dummy1[1]=dummy2[1]=dummy3[1]='x';
        for (int j=0;j<4;j++)
        {
            dummy1[2+j]=dummy[j];
            dummy2[2+j]=dummy[j+4];
            dummy3[2+j]=dummy[j+8];
        }
        dummy1[2+4]='\0';
        dummy2[2+4]='\0';
        dummy3[2+4]='\0';
        sscanf(dummy1,"%x",&rr);
        sscanf(dummy2,"%x",&gg);
        sscanf(dummy3,"%x",&bb);
        cols[i]=QColor(rr/256 ,gg/256 ,bb/256 );
    }
    for (int i=0;i<c_width;i++)
    {
        for (int j=0;j<c_height;j++)
        {
            for (int k=0;k<nr_cols;k++)
            {
                if (ctrl_char[k]==grace_icon_xpm[1+nr_cols+j][i])
                {
                    img1.setPixel(i,j,cols[k].rgb());
                    break;
                }
            }
        }
    }
    *GraceIcon=QPixmap::fromImage(img1,Qt::AutoColor);
    delete[] cols;
    delete[] ctrl_char;
    c_read=sscanf(active_xpm[0],"%d %d %d %d",&c_width,&c_height,&nr_cols,&bpc);
    img1=QImage(c_width,c_height,QImage::Format_ARGB32_Premultiplied);
    cols=new QColor[nr_cols];
    ctrl_char=new char[nr_cols];
    for (int i=0;i<nr_cols;i++)
    {
        c_read=sscanf(active_xpm[1+i],"%c c #%s",ctrl_char+i,dummy);
        if (c_read<2)
            c_read=0;
        else
            c_read=255;
        dummy1[0]=dummy2[0]=dummy3[0]='0';
        dummy1[1]=dummy2[1]=dummy3[1]='x';
        for (int j=0;j<2;j++)
        {
            dummy1[2+j]=dummy[j];
            dummy2[2+j]=dummy[j+2];
            dummy3[2+j]=dummy[j+4];
        }
        dummy1[2+2]='\0';
        dummy2[2+2]='\0';
        dummy3[2+2]='\0';
        sscanf(dummy1,"%x",&rr);
        sscanf(dummy2,"%x",&gg);
        sscanf(dummy3,"%x",&bb);
        if (c_read==0)
            cols[i]=Qt::white;
        else
            cols[i]=QColor(rr ,gg ,bb,c_read);
    }
    for (int i=0;i<c_width;i++)
    {
        for (int j=0;j<c_height;j++)
        {
            for (int k=0;k<nr_cols;k++)
            {
                if (ctrl_char[k]==active_xpm[1+nr_cols+j][i])
                {
                    img1.setPixel(i,j,cols[k].rgb());
                    break;
                }
            }
        }
    }
    *ActiveIcon=QPixmap::fromImage(img1,Qt::AutoColor);
    delete[] cols;
    delete[] ctrl_char;
    c_read=sscanf(hidden_xpm[0],"%d %d %d %d",&c_width,&c_height,&nr_cols,&bpc);
    img1=QImage(c_width,c_height,QImage::Format_ARGB32_Premultiplied);
    cols=new QColor[nr_cols];
    ctrl_char=new char[nr_cols];
    for (int i=0;i<nr_cols;i++)
    {
        c_read=sscanf(hidden_xpm[1+i],"%c c #%s",ctrl_char+i,dummy);
        if (c_read<2)
            c_read=0;
        else
            c_read=255;
        dummy1[0]=dummy2[0]=dummy3[0]='0';
        dummy1[1]=dummy2[1]=dummy3[1]='x';
        for (int j=0;j<2;j++)
        {
            dummy1[2+j]=dummy[j];
            dummy2[2+j]=dummy[j+2];
            dummy3[2+j]=dummy[j+4];
        }
        dummy1[2+2]='\0';
        dummy2[2+2]='\0';
        dummy3[2+2]='\0';
        sscanf(dummy1,"%x",&rr);
        sscanf(dummy2,"%x",&gg);
        sscanf(dummy3,"%x",&bb);
        if (c_read==0)
            cols[i]=Qt::white;
        else
            cols[i]=QColor(rr ,gg ,bb,c_read);
    }
    for (int i=0;i<c_width;i++)
    {
        for (int j=0;j<c_height;j++)
        {
            for (int k=0;k<nr_cols;k++)
            {
                if (ctrl_char[k]==hidden_xpm[1+nr_cols+j][i])
                {
                    img1.setPixel(i,j,cols[k].rgb());
                    break;
                }
            }
        }
    }
    *HiddenIcon=QPixmap::fromImage(img1,Qt::AutoColor);
    delete[] cols;
    delete[] ctrl_char;

    //GraceIcon->save(QString("GraceIcon.png"),"PNG",100);

    //Read justification bits
    Qt_j_lm_o_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
    // *Qt_j_lm_o_bits=generate_Bitmap_from_Bits(j_lm_o_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
    generate_Pixmap_from_Bits(j_lm_o_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,Qt_j_lm_o_bits);
    Qt_j_cm_o_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
    // *Qt_j_cm_o_bits=generate_Bitmap_from_Bits(j_cm_o_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
    generate_Pixmap_from_Bits(j_cm_o_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,Qt_j_cm_o_bits);
    Qt_j_rm_o_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
    // *Qt_j_rm_o_bits=generate_Bitmap_from_Bits(j_rm_o_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
    generate_Pixmap_from_Bits(j_rm_o_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,Qt_j_rm_o_bits);
    Qt_j_lb_b_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
    // *Qt_j_lb_b_bits=generate_Bitmap_from_Bits(j_lb_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
    generate_Pixmap_from_Bits(j_lb_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,Qt_j_lb_b_bits);
    Qt_j_lm_b_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
    // *Qt_j_lm_b_bits=generate_Bitmap_from_Bits(j_lm_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
    generate_Pixmap_from_Bits(j_lm_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,Qt_j_lm_b_bits);
    Qt_j_lt_b_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
    // *Qt_j_lt_b_bits=generate_Bitmap_from_Bits(j_lt_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
    generate_Pixmap_from_Bits(j_lt_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,Qt_j_lt_b_bits);
    Qt_j_ct_b_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
    // *Qt_j_ct_b_bits=generate_Bitmap_from_Bits(j_ct_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
    generate_Pixmap_from_Bits(j_ct_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,Qt_j_ct_b_bits);
    Qt_j_rt_b_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
    // *Qt_j_rt_b_bits=generate_Bitmap_from_Bits(j_rt_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
    generate_Pixmap_from_Bits(j_rt_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,Qt_j_rt_b_bits);
    Qt_j_rm_b_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
    // *Qt_j_rm_b_bits=generate_Bitmap_from_Bits(j_rm_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
    generate_Pixmap_from_Bits(j_rm_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,Qt_j_rm_b_bits);
    Qt_j_rb_b_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
    // *Qt_j_rb_b_bits=generate_Bitmap_from_Bits(j_rb_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
    generate_Pixmap_from_Bits(j_rb_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,Qt_j_rb_b_bits);
    Qt_j_cb_b_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
    // *Qt_j_cb_b_bits=generate_Bitmap_from_Bits(j_cb_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
    generate_Pixmap_from_Bits(j_cb_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,Qt_j_cb_b_bits);
    Qt_j_cm_b_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
    // *Qt_j_cm_b_bits=generate_Bitmap_from_Bits(j_cm_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
    generate_Pixmap_from_Bits(j_cm_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,Qt_j_cm_b_bits);

    //Orientation
    QPixmap * tmpOrientation;
    QPainter tmpPainter;
    int border=4;
    tmpOrientation=new QPixmap(MBITMAP_WIDTH,MBITMAP_HEIGHT);

    generate_Pixmap_from_Bits(m_hv_lr_bt_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,tmpOrientation);
    Qt_m_hv_lr_bt_bits=new QPixmap(MBITMAP_WIDTH+2*border,MBITMAP_HEIGHT+2*border);
    Qt_m_hv_lr_bt_bits->fill();
    tmpPainter.begin(Qt_m_hv_lr_bt_bits);
    tmpPainter.drawPixmap(border,border,MBITMAP_WIDTH,MBITMAP_HEIGHT,*tmpOrientation);
    tmpPainter.end();
    generate_Pixmap_from_Bits(m_hv_lr_tb_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,tmpOrientation);
    Qt_m_hv_lr_tb_bits=new QPixmap(MBITMAP_WIDTH+2*border,MBITMAP_HEIGHT+2*border);
    Qt_m_hv_lr_tb_bits->fill();
    tmpPainter.begin(Qt_m_hv_lr_tb_bits);
    tmpPainter.drawPixmap(border,border,MBITMAP_WIDTH,MBITMAP_HEIGHT,*tmpOrientation);
    tmpPainter.end();
    generate_Pixmap_from_Bits(m_hv_rl_bt_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,tmpOrientation);
    Qt_m_hv_rl_bt_bits=new QPixmap(MBITMAP_WIDTH+2*border,MBITMAP_HEIGHT+2*border);
    Qt_m_hv_rl_bt_bits->fill();
    tmpPainter.begin(Qt_m_hv_rl_bt_bits);
    tmpPainter.drawPixmap(border,border,MBITMAP_WIDTH,MBITMAP_HEIGHT,*tmpOrientation);
    tmpPainter.end();
    generate_Pixmap_from_Bits(m_hv_rl_tb_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,tmpOrientation);
    Qt_m_hv_rl_tb_bits=new QPixmap(MBITMAP_WIDTH+2*border,MBITMAP_HEIGHT+2*border);
    Qt_m_hv_rl_tb_bits->fill();
    tmpPainter.begin(Qt_m_hv_rl_tb_bits);
    tmpPainter.drawPixmap(border,border,MBITMAP_WIDTH,MBITMAP_HEIGHT,*tmpOrientation);
    tmpPainter.end();
    generate_Pixmap_from_Bits(m_vh_lr_bt_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,tmpOrientation);
    Qt_m_vh_lr_bt_bits=new QPixmap(MBITMAP_WIDTH+2*border,MBITMAP_HEIGHT+2*border);
    Qt_m_vh_lr_bt_bits->fill();
    tmpPainter.begin(Qt_m_vh_lr_bt_bits);
    tmpPainter.drawPixmap(border,border,MBITMAP_WIDTH,MBITMAP_HEIGHT,*tmpOrientation);
    tmpPainter.end();
    generate_Pixmap_from_Bits(m_vh_lr_tb_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,tmpOrientation);
    Qt_m_vh_lr_tb_bits=new QPixmap(MBITMAP_WIDTH+2*border,MBITMAP_HEIGHT+2*border);
    Qt_m_vh_lr_tb_bits->fill();
    tmpPainter.begin(Qt_m_vh_lr_tb_bits);
    tmpPainter.drawPixmap(border,border,MBITMAP_WIDTH,MBITMAP_HEIGHT,*tmpOrientation);
    tmpPainter.end();
    generate_Pixmap_from_Bits(m_vh_rl_bt_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,tmpOrientation);
    Qt_m_vh_rl_bt_bits=new QPixmap(MBITMAP_WIDTH+2*border,MBITMAP_HEIGHT+2*border);
    Qt_m_vh_rl_bt_bits->fill();
    tmpPainter.begin(Qt_m_vh_rl_bt_bits);
    tmpPainter.drawPixmap(border,border,MBITMAP_WIDTH,MBITMAP_HEIGHT,*tmpOrientation);
    tmpPainter.end();
    generate_Pixmap_from_Bits(m_vh_rl_tb_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,tmpOrientation);
    Qt_m_vh_rl_tb_bits=new QPixmap(MBITMAP_WIDTH+2*border,MBITMAP_HEIGHT+2*border);
    Qt_m_vh_rl_tb_bits->fill();
    tmpPainter.begin(Qt_m_vh_rl_tb_bits);
    tmpPainter.drawPixmap(border,border,MBITMAP_WIDTH,MBITMAP_HEIGHT,*tmpOrientation);
    tmpPainter.end();
    delete tmpOrientation;*/

    /*
        Qt_m_hv_lr_bt_bits=new QPixmap(MBITMAP_WIDTH,MBITMAP_HEIGHT);
        generate_Pixmap_from_Bits(m_hv_lr_bt_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,Qt_m_hv_lr_bt_bits);
        Qt_m_hv_lr_tb_bits=new QPixmap(MBITMAP_WIDTH,MBITMAP_HEIGHT);
        generate_Pixmap_from_Bits(m_hv_lr_tb_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,Qt_m_hv_lr_tb_bits);
        Qt_m_hv_rl_bt_bits=new QPixmap(MBITMAP_WIDTH,MBITMAP_HEIGHT);
        generate_Pixmap_from_Bits(m_hv_rl_bt_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,Qt_m_hv_rl_bt_bits);
        Qt_m_hv_rl_tb_bits=new QPixmap(MBITMAP_WIDTH,MBITMAP_HEIGHT);
        generate_Pixmap_from_Bits(m_hv_rl_tb_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,Qt_m_hv_rl_tb_bits);
        Qt_m_vh_lr_bt_bits=new QPixmap(MBITMAP_WIDTH,MBITMAP_HEIGHT);
        generate_Pixmap_from_Bits(m_vh_lr_bt_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,Qt_m_vh_lr_bt_bits);
        Qt_m_vh_lr_tb_bits=new QPixmap(MBITMAP_WIDTH,MBITMAP_HEIGHT);
        generate_Pixmap_from_Bits(m_vh_lr_tb_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,Qt_m_vh_lr_tb_bits);
    Qt_m_vh_rl_bt_bits=new QPixmap(MBITMAP_WIDTH,MBITMAP_HEIGHT);
    generate_Pixmap_from_Bits(m_vh_rl_bt_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,Qt_m_vh_rl_bt_bits);
    Qt_m_vh_rl_tb_bits=new QPixmap(MBITMAP_WIDTH,MBITMAP_HEIGHT);
    generate_Pixmap_from_Bits(m_vh_rl_tb_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,Qt_m_vh_rl_tb_bits);*/

    /*QPixmap templIcon(82,16);//22
    QPainter templPainter;
    QPen pen1(Qt::black);
    for (int i=0;i<MAXLINESTYLES;i++)
    {
        //templIcon.fill(Qt::white);
        templIcon.fill(QApplication::palette().window().color());
        templPainter.begin(&templIcon);

        PenDashPattern[i]=new QVector<qreal>();
        for (int j=0;j<dash_array_length[i];j++)
            *(PenDashPattern[i]) << dash_array[i][j];
        if (i==0)
        {
            templPainter.setPen(pen1);
            templPainter.drawText(10,13,"None");//15
        }
        else
        {
            pen1.setColor(Qt::black);
            pen1.setDashPattern(*PenDashPattern[i]);
            pen1.setWidth(2);
            templPainter.setPen(pen1);
            templPainter.drawLine(0,8,82,8);//11
        }
        templPainter.end();
        LineIcons[i]=new QIcon(templIcon);
        LinePixmaps[i]=new QPixmap(templIcon);
    }*/
}

void MainWindow::newFile(void)
{
    if (new_project(NULL)==RETURN_FAILURE) return;
    initNodes();//Undo-Stuff
    lstGraphs->update_number_of_entries_preserve_selection();
    mainArea->completeRedraw();
}

void MainWindow::Open(void)
{
QString openfilename;
    if (auto_set_cwd==TRUE)
    {
    openfilename=QString::fromLocal8Bit(get_workingdir());
    }
    else
    {
    openfilename=Last_Dialog_Path[READ_PROJECT_FORM];
    }
        if (openfilename.right(1)==QDir::separator())
        {}
        else
        {
        openfilename += QDir::separator();
        }
//qDebug() << "Open: openfilename=" << openfilename;
if (should_open_grace_file_dialog())
{
    if (FormOpenProject==NULL)
    {
        FormOpenProject=new frmIOForm(READ_PROJECT_FORM,this);
        connect(FormOpenProject,SIGNAL(newFileSelectedForIO(int,QString,bool,bool,bool)),SLOT(IOrequested(int,QString,bool,bool,bool)));
    }
    FormOpenProject->init(openfilename.toLocal8Bit().data());
    FormOpenProject->show();
    FormOpenProject->raise();
    FormOpenProject->activateWindow();
}
else
{
UseOperatingSystemFileDialog(READ_PROJECT_FORM,tr("QtGrace: Open project"),openfilename,tr("Grace-project-files (*.agr *.xmg);;All files (*)"));
}
}

void MainWindow::UseOperatingSystemFileDialog(int type, QString title, QString file, QString extension)
{
QString new_filename;
QFileInfo fi;
int save_noask=noask;
switch (type)
{
default:
case READ_PARAMETERS:
case READ_PROJECT_FORM:
case READ_COMMANDS_FILE:
case READ_FIT_PARAM:
case READ_NETCDF_FORM:
    new_filename=QFileDialog::getOpenFileName(this,title,file,extension);
break;
case WRITE_PARAMETERS:
case WRITE_PROJECT_FORM:
case WRITE_COMMANDS_FILE:
case WRITE_FIT_PARAM:
    noask=TRUE;
case SELECT_PRINT_FILE:
    new_filename=QFileDialog::getSaveFileName(this,title,file,extension);
break;
}
if (!new_filename.isEmpty())
{
fi.setFile(new_filename);
//qDebug() << "1 fipath=" << Last_Dialog_Path[type];
Last_Dialog_Path[type]=QDir::toNativeSeparators(fi.absolutePath());
//qDebug() << "2 fipath=" << Last_Dialog_Path[type];
    switch (type)
    {
    case READ_PARAMETERS:
    case WRITE_PARAMETERS:
    case READ_PROJECT_FORM:
    case WRITE_PROJECT_FORM:
        mainWin->IOrequested(type,new_filename,fi.exists(),fi.isWritable(),fi.isReadable());
    break;
    case READ_COMMANDS_FILE:
    case WRITE_COMMANDS_FILE:
        FormCommands->IOrequested(type,new_filename,fi.exists(),fi.isWritable(),fi.isReadable());
    break;
    case READ_FIT_PARAM:
    case WRITE_FIT_PARAM:
        FormNonlinCurveFit->IOrequested(type,new_filename,fi.exists(),fi.isWritable(),fi.isReadable());
    break;
    case READ_NETCDF_FORM:
        FormNetCDF->IOrequested(type,new_filename,fi.exists(),fi.isWritable(),fi.isReadable());
    break;
    case SELECT_PRINT_FILE:
        FormDeviceSetup->IOrequested(type,new_filename,fi.exists(),fi.isWritable(),fi.isReadable());
    break;
    }
}
else//cancelled
{
    if (future_load_status==1 || future_load_status==2)
    {
    errmsg(tr("Saving cancelled, Load-operation cancelled.").toLocal8Bit().constData());
    future_load_status=0;
    }
    else if (future_load_status==3)
    {
    errmsg(tr("Saving cancelled, Close-operation cancelled.").toLocal8Bit().constData());
    future_load_status=0;
    }
}
noask=save_noask;
}

void MainWindow::IOrequested(int type,QString file,bool exists,bool writeable,bool readable)
{
    int finish_with_load_or_close=0;
    char *dummy=new char[file.length()*4+8];
    strcpy(dummy,file.toLocal8Bit());
    char *dummy2=new char[strlen(dummy)+35];
    sprintf(dummy2,"%s%s",tr("Can't stat file ").toLocal8Bit().constData(),dummy);

/*qDebug() << "IOrequested: file=" << file << "#dummy=" << dummy;
qDebug() << "Origin(type)=" << type;
qDebug() << "file to load=" << dummy;
qDebug() << "exists? = " << exists;
qDebug() << "writable? = " << writeable;
qDebug() << "readable? = " << readable;*/

    set_wait_cursor();
    if (
        (readable==false && (type==READ_SET_FORM || type==READ_NETCDF_FORM || type==READ_PROJECT_FORM || type==READ_PARAMETERS) )
        ||
        (writeable==false && exists==true)
       )
    {
        errwin(dummy2);
        unset_wait_cursor();
        delete[] dummy;
        delete[] dummy2;
        return;
    }
    switch (type)
    {
    case READ_SET_FORM:
        ;//done separately
        break;
    case READ_NETCDF_FORM:
        ;//done separately
        break;
    case READ_PROJECT_FORM:
        if (readable==false)
        {
            errwin(dummy2);
            unset_wait_cursor();
            delete[] dummy;
            delete[] dummy2;
            return;
        }
        else
        {
            LoadProject(dummy);
                if (FormOpenProject)
                FormOpenProject->doCancel();
            if (FormDeviceSetup!=NULL) FormDeviceSetup->printfile_item->lenText->setText(get_filename_with_extension(FormDeviceSetup->cur_dev));
        }
        break;
    case WRITE_SET_FORM:
        ;//done separately
        break;
    case WRITE_PROJECT_FORM:
        overwriteAnswerNo=FALSE;
        if (exists==false || (exists==true && writeable==true))
        {
            save_project(dummy);//the user might be asked to overwrite a file --> we can check the answer afterwards
            if (FormSaveProject)
            {
            FormSaveProject->doFilter();
            strcpy(sformat,sav_sformat);
                if (overwriteAnswerNo==FALSE)
                FormSaveProject->hide();
            }
            strcpy(sformat,sav_sformat);//reset data-format to value before writing
            /// CHANGED!
            /*Device_entry dev = get_device_props(hdevice);
if (FormDeviceSetup!=NULL) dev = get_device_props(FormDeviceSetup->cur_dev);
sprintf(print_file,"%s.%s",get_docbname(),dev.fext);
if (FormDeviceSetup!=NULL) FormDeviceSetup->printfile_item->setText(QString(print_file));*/
            if (FormDeviceSetup!=NULL) FormDeviceSetup->printfile_item->lenText->setText(get_filename_with_extension(FormDeviceSetup->cur_dev));
        }
        if (future_load_status==1 || future_load_status==2)
        {
        finish_with_load_or_close=1;
        future_load_status=0;
        }
        else if (future_load_status==3)//close after save
        {
        finish_with_load_or_close=2;
        future_load_status=0;
        }
        break;
    case READ_PARAMETERS:
        if (readable!=false)
        {
            getparms(dummy);
            update_all();
            mainArea->completeRedraw();
            if (FormReadParameters)
            FormReadParameters->hide();
        }
        break;
    case WRITE_PARAMETERS:
        int gno;
        FILE *pp;
        if (!FormWriteParameters)
        {
        gno = ALL_GRAPHS;
        }
        else
        {
            if (GetChoice(FormWriteParameters->selParamGraph) == 0)
            {
            gno = get_cg();
            }
            else
            {
            gno = ALL_GRAPHS;
            }
        }
        pp = grace_openw(dummy);
        if (pp != NULL)
        {
            putparms(gno, pp, 0);
            grace_close(pp);
        }
            if (FormWriteParameters)
            FormWriteParameters->hide();
        break;
    }
    unset_wait_cursor();
    set_action(DO_NOTHING);
    delete[] dummy;
    delete[] dummy2;
//qDebug() << "finish_with_load_or_close=" << finish_with_load_or_close;
    if (overwriteAnswerNo==TRUE)
    {
        if (finish_with_load_or_close==1)
        {
        errmsg(tr("Saving cancelled, Load-operation cancelled.").toLocal8Bit().constData());
        }
        else if (finish_with_load_or_close==2)
        {
        errmsg(tr("Saving cancelled, Close-operation cancelled.").toLocal8Bit().constData());
        }
        else
        {
        qDebug() << "NO OVERWRITE";
        }
        finish_with_load_or_close=0;
    }
    else
    {
        if (finish_with_load_or_close==1)//load
        {
        clear_dirtystate();
        LoadProject(future_load_file.toLocal8Bit().data());
        }
        else if (finish_with_load_or_close==2)//close
        {
        finalBailout();
        }
    }
}

void MainWindow::Save(void)
{
    if (strcmp(get_docname(), NONAME) != 0)
        save_project(get_docname());
    else
        SaveAs();
}

void MainWindow::SaveAs(void)
{
QString openfilename;
openfilename=QString::fromLocal8Bit(get_docname());
//qDebug() << "SaveAs: openfilename=" << openfilename;
if (should_open_grace_file_dialog())
{
    if (FormSaveProject==NULL)
    {
        FormSaveProject=new frmIOForm(WRITE_PROJECT_FORM,this);
        connect(FormSaveProject,SIGNAL(newFileSelectedForIO(int,QString,bool,bool,bool)),SLOT(IOrequested(int,QString,bool,bool,bool)));
    }
    FormSaveProject->init(openfilename.toLocal8Bit().data());
    FormSaveProject->show();
    FormSaveProject->raise();
    FormSaveProject->activateWindow();
}
else
{
UseOperatingSystemFileDialog(WRITE_PROJECT_FORM,tr("QtGrace: Save project"),openfilename,tr("Grace-project-files (*.agr *.xmg);;All files (*)"));
}
}

void MainWindow::RevertToSaved(void)
{
    QString selectedFile(get_docname());
//qDebug() << "A selectedFile=" << selectedFile;
    selectedFile=selectedFile.fromLocal8Bit(get_docname());
//qDebug() << "B selectedFile=" << selectedFile;
    QFile fileTest(selectedFile);
    QFileInfo fileInfo(fileTest);
    bool FileExists,isWriteable,isReadable;
    if (fileInfo.isFile()==false)
    {
        FileExists=false;
        isWriteable=false;
        isReadable=false;
    }
    else
    {
        FileExists=fileTest.exists();
        isWriteable=fileInfo.isWritable();
        isReadable=fileInfo.isReadable();
    }
    IOrequested(READ_PROJECT_FORM,selectedFile,FileExists,isWriteable,isReadable);
}

void MainWindow::Exit(void)
{
#ifdef DEBUG_OUT_LOG
debug_out << "Exit-routine in MainWindow." << endl;
#endif
    int ret=bailout();
    if (ret!=0)
    {
    finalBailout();
    }
}

void MainWindow::PrintSetup(void)
{
//bool initiate=false;
    if (FormDeviceSetup==NULL)
    {
        FormDeviceSetup=new frmDeviceSetup(this);
        //initialize this only on startup
        /*cout << "hdevice=" << hdevice << endl;
        cout << "default_Print_Device=" << default_Print_Device << endl;
        cout << "stdOutput=" << stdOutputFormat << endl;*/

        if (default_Print_Device==-1)//last one
            FormDeviceSetup->devices_item->setCurrentValue(stdOutputFormat);
        else
            FormDeviceSetup->devices_item->setCurrentValue(default_Print_Device);

        if (lastPrintDevice<=0) lastPrintDevice=FormDeviceSetup->devices_item->currentValue();

        FormDeviceSetup->printfile_item->lenText->setText(get_filename_with_extension(FormDeviceSetup->cur_dev));
    //initiate=true;
    }
    FormDeviceSetup->changeDeviceList(2);

    /*Device_entry dev = get_device_props(FormDeviceSetup->cur_dev);
    sprintf(print_file,"%s.%s",get_docbname(),dev.fext);
    QDir tmpFile(get_docname());
    cout << "print_file=" << print_file << " docname=" << get_docname() << endl;
    tmpFile.cdUp();
    if (strcmp(get_docname(),"Untitled"))
    {
    FormDeviceSetup->printfile_item->setText(QDir::cleanPath(tmpFile.absolutePath())+QDir::separator()+QString(print_file));
    }
    else
    {
    FormDeviceSetup->printfile_item->setText(QString(print_file));
    }*/

///FormDeviceSetup->printfile_item->setText(get_filename_with_extension(FormDeviceSetup->cur_dev));

//cout << "LastPrintDevice=" << lastPrintDevice << " current=" << FormDeviceSetup->devices_item->currentValue() << endl;

    /// I changed this to std-Output-Format (not last format): this means, that if Device setup is closed without apply, the settings will be lost!
    /*if (FormDeviceSetup->devices_item->currentValue()!=lastPrintDevice)//dialog is set to display the page-setup --> switch back to print-setup with a 'real' print device
    {
        FormDeviceSetup->devices_item->setCurrentValue(lastPrintDevice);
    }*/

    FormDeviceSetup->devices_item->setCurrentValue(stdOutputFormat);

    //FormDeviceSetup->devices_item->setCurrentIndex(find_dev_nr("PS")-1);//'-1' because dummy-device not shown

    if (win_w>win_h)//landscape
    {
        if (FormDeviceSetup->page_orient_item->currentIndex()!=0)//not set to landscape
        {
            FormDeviceSetup->page_orient_item->setCurrentIndex(0);
        }
    }
    else//portrait
    {
        if (FormDeviceSetup->page_orient_item->currentIndex()==0)//not set to portrait
        {
            FormDeviceSetup->page_orient_item->setCurrentIndex(1);
        }
    }
//cout << "new device=" << FormDeviceSetup->devices_item->currentValue() << endl;
    /// FormDeviceSetup->devices_item->setEnabled(true);
    FormDeviceSetup->init(FormDeviceSetup->devices_item->currentValue());
    FormDeviceSetup->show();
    FormDeviceSetup->raise();
    FormDeviceSetup->activateWindow();
    //if (initiate==true)
    //FormDeviceSetup->resize(400,400);
}

void MainWindow::Print(void)
{
set_ptofile(false);//we are going to print on a printer (not realy in a file)
bool ret=true;
if (use_print_command==true)
{
FormProgress->init(tr("Printing..."),3);
FormProgress->show();
FormProgress->raise();
FormProgress->activateWindow();
qApp->processEvents();

do_hardcopy();

FormProgress->hide();
}
else
{
//if (FormDeviceSetup==NULL)
ret=FormDeviceSetup->openNativePrinter(DEVICE_PDF);
    if (ret==false)
    {
    ;//qDebug() << "native printing aborted";
    }
}
qApp->processEvents();
}

void MainWindow::PrintSpecial(void)
{
//qDebug() << "Someone pressend Print Special, dpi=" << actPrintSpecial->spnValue->value();
}

void MainWindow::PrintToFile(void)
{
static int save_dirty_state=dirtystate;
/// print_target=PRINT_TARGET_FILE;
    set_ptofile(true);
//cout << "Print To File: #" << print_file << "#" << endl;
    printing_in_file=true;
    //device_table[DEVICE_SCREEN].pg.width=orig_page_w;//use original page size
    //device_table[DEVICE_SCREEN].pg.height=orig_page_h;
FormProgress->init(tr("File output..."),3);
FormProgress->show();
FormProgress->raise();
FormProgress->activateWindow();
qApp->processEvents();
    do_hardcopy();
FormProgress->hide();
    //device_table[DEVICE_SCREEN].pg.width=orig_page_w*GeneralPageZoomFactor;//use Page Zoom
    //device_table[DEVICE_SCREEN].pg.height=orig_page_h*GeneralPageZoomFactor;
    printing_in_file=false;
    print_target=PRINT_TARGET_SCREEN;
    mainArea->completeRedraw();
dirtystate=save_dirty_state;
    update_app_title();
}

void MainWindow::DataSets(void)
{
    if (FormDataSetProperties==NULL)
    {
        FormDataSetProperties=new frmDataSetProperties(this);
    }
    FormDataSetProperties->init();
    FormDataSetProperties->show();
    FormDataSetProperties->raise();
    FormDataSetProperties->activateWindow();
}

void MainWindow::SetOperations(void)
{
    if (FormSetOPS==NULL)
    {
        FormSetOPS=new frmSetOperations(this);
    }
    FormSetOPS->init();
    FormSetOPS->show();
    FormSetOPS->raise();
    FormSetOPS->activateWindow();
}

void MainWindow::Explorer(void)
{
    if (FormExplorer==NULL)
    {
        FormExplorer=new frmExplorer(this);
    }
    FormExplorer->init();
    FormExplorer->show();
    FormExplorer->raise();
    FormExplorer->activateWindow();
}

void MainWindow::ColorManager(void)
{
    /*if (FormColManage==NULL)//Color-Management is now part of the Preferences
    {
        FormColManage=new frmColorManagement(0);
    }
    FormColManage->init();
    FormColManage->show();
    FormColManage->raise();
    FormColManage->activateWindow();*/
}

void MainWindow::RealTimeInputDisplay(void)
{
    if (FormRTIManage==NULL)
    {
        FormRTIManage=new frmRealTimeInputManager(this);
    }
    FormRTIManage->init();
    FormRTIManage->show();
    FormRTIManage->raise();
    FormRTIManage->activateWindow();
}

void MainWindow::ArrangeGraphs(void)
{
    if (FormArrangeGraphs==NULL)
    {
        FormArrangeGraphs=new frmArrangeGraphs(this);
    }
    FormArrangeGraphs->init();
    FormArrangeGraphs->show();
    FormArrangeGraphs->raise();
    FormArrangeGraphs->activateWindow();
}

void MainWindow::OverlayGraphs(void)
{
    if (FormOverlayGraphs==NULL)
    {
        FormOverlayGraphs=new frmOverlayGraphs(this);
    }
    FormOverlayGraphs->init();
    FormOverlayGraphs->show();
    FormOverlayGraphs->raise();
    FormOverlayGraphs->activateWindow();
}

void MainWindow::AutoscaleGraphs(void)
{
    if (FormAutoScaleGraphs==NULL)
    {
        FormAutoScaleGraphs=new frmAutoscaleGraphs(this);
    }
    FormAutoScaleGraphs->init();
    FormAutoScaleGraphs->show();
    FormAutoScaleGraphs->raise();
    FormAutoScaleGraphs->activateWindow();
}

void MainWindow::HotLinks(void)
{
    if (FormHotLinks==NULL)
    {
        FormHotLinks=new frmHotLinks(this);
    }
    FormHotLinks->init();
    FormHotLinks->show();
    FormHotLinks->raise();
    FormHotLinks->activateWindow();
}

void MainWindow::SetLocFixPoint(void)
{
    set_action(SEL_POINT);
}

void MainWindow::ClearLocFixPoint(void)
{
    GLocator locator;
    get_graph_locator(get_cg(), &locator);
    locator.pointset = FALSE;
    SaveLocatorFixPoint(get_cg());
    set_graph_locator(get_cg(), &locator);
    LocatorFixPointModified(get_cg());
    mainArea->completeRedraw();
}

void MainWindow::LocProp(void)
{
    if (FormLocatorProps==NULL)
    {
        FormLocatorProps=new frmLocatorProps(this);
    }
    FormLocatorProps->init();
    FormLocatorProps->show();
    FormLocatorProps->raise();
    FormLocatorProps->activateWindow();
}

void MainWindow::Preferences(void)
{
    /*if (FormPreferences==NULL)
    {
        FormPreferences=new frmPreferences(this);
    }
    FormPreferences->init();
    FormPreferences->show();
    FormPreferences->raise();
    FormPreferences->activateWindow();*/
    if (Form_Preferences==NULL)
    {
        Form_Preferences=new frm_Preferences(this);
    }
    Form_Preferences->init();
    Form_Preferences->show();
    Form_Preferences->raise();
    Form_Preferences->activateWindow();
}

void MainWindow::RegionsMaster(void)
{
    if (FormRegionMaster==NULL)
    {
        FormRegionMaster=new frmMasterRegionOperator(this);
    }
    FormRegionMaster->init();
    FormRegionMaster->show();
    FormRegionMaster->raise();
    FormRegionMaster->activateWindow();
}

void MainWindow::RegionsStatus(void)
{
    if (FormRegionStatus==NULL)
    {
        FormRegionStatus=new frmRegionStatus(this);
    }
    FormRegionStatus->init();
    FormRegionStatus->show();
    FormRegionStatus->raise();
    FormRegionStatus->activateWindow();
}

void MainWindow::RegionsDefine(void)
{
    if (FormDefineRegion==NULL)
    {
        FormDefineRegion=new frmRegions(DEFINEREGIONWINDOW,this);
    }
    FormDefineRegion->init();
    FormDefineRegion->show();
    FormDefineRegion->raise();
    FormDefineRegion->activateWindow();
}

void MainWindow::RegionsClear(void)
{
    if (FormClearRegion==NULL)
    {
        FormClearRegion=new frmRegions(CLEARREGIONWINDOW,this);
    }
    FormClearRegion->init();
    FormClearRegion->show();
    FormClearRegion->raise();
    FormClearRegion->activateWindow();
}

void MainWindow::RegionsReportOn(void)
{
    if (FormReportRegion==NULL)
    {
        FormReportRegion=new frmRegions(REPORTREGIONWINDOW,this);
    }
    FormReportRegion->init();
    FormReportRegion->show();
    FormReportRegion->raise();
    FormReportRegion->activateWindow();
}

void MainWindow::DataSetOperations(void)
{
    if (FormSetOperations==NULL)
    {
        FormSetOperations=new frmSetOp(this);
    }
    FormSetOperations->init();
    FormSetOperations->show();
    FormSetOperations->raise();
    FormSetOperations->activateWindow();
}

void MainWindow::FeatureExtraction(void)
{
    if (FormFeatureExtraction==NULL)
    {
        FormFeatureExtraction=new frmFeatureExtract(this);
        //FormFeatureExtraction->resize(250,600);//to make it small enough
    }
    FormFeatureExtraction->init();
    FormFeatureExtraction->show();
    FormFeatureExtraction->raise();
    FormFeatureExtraction->activateWindow();
}

void MainWindow::ImportAscii(void)
{
    if (FormReadSets==NULL)
    {
        FormReadSets=new frmIOForm(READ_SET_FORM,this);
    }
    FormReadSets->init();
    FormReadSets->show();
    FormReadSets->raise();
    FormReadSets->activateWindow();
}

void MainWindow::ImportCSV(void)
{
    /*if (FormCSVImport==NULL)
    {
        FormCSVImport=new frmCSVImporter(this);
    }
    FormCSVImport->init();
    FormCSVImport->show();
    FormCSVImport->raise();
    FormCSVImport->activateWindow();*/
    if (FormCSVImportV2==NULL)
        {
            FormCSVImportV2=new frmCSVImporterV2(this);
        }
        FormCSVImportV2->init();
        FormCSVImportV2->show();
        FormCSVImportV2->raise();
        FormCSVImportV2->activateWindow();
}

void MainWindow::ImportBinary(void)
{
    if (FormBinaryImportFilter==NULL)
    {
        FormBinaryImportFilter=new frmBinaryFormatInput(this);
    }
    FormBinaryImportFilter->init();
    FormBinaryImportFilter->show();
    FormBinaryImportFilter->raise();
    FormBinaryImportFilter->activateWindow();
/*if (FormReadBinary==NULL)
{
FormReadBinary=new frmIOForm(READ_BINARY_FILE,this);
}
FormReadBinary->init();
FormReadBinary->show();
FormReadBinary->raise();
FormReadBinary->activateWindow();*/
}

void MainWindow::ImportNetCDF(void)
{
    ///anything missing!!!
    if (FormNetCDF==NULL)
    {
        FormNetCDF=new frmNetCDF(this);
    }
    FormNetCDF->init();
    FormNetCDF->show();
    FormNetCDF->raise();
    FormNetCDF->activateWindow();
}

void MainWindow::ImportAgr(void)
{
QString openfilename;
    if (auto_set_cwd==TRUE)
    {
    openfilename=QString::fromLocal8Bit(get_workingdir());
    }
    else
    {
    openfilename=Last_Dialog_Path[READ_PROJECT_FORM];
    }
//qDebug() << "openfilename=" << openfilename;
QString new_filename=QFileDialog::getOpenFileName(this,tr("QtGrace: Import sets from project"),openfilename,tr("Grace-project-files (*.agr *.xmg);;All files (*)"));
if (!new_filename.isEmpty())
{
    if (Form_AgrInfo==NULL)
    {
        Form_AgrInfo=new frmAgrInfos(0);
    }
    Form_AgrInfo->init(new_filename.toLocal8Bit().data());
    Form_AgrInfo->buttons->cmdAccept->setDefault(true);
    Form_AgrInfo->show();
    Form_AgrInfo->raise();
    Form_AgrInfo->activateWindow();
}
}

void MainWindow::ExportAscii(void)
{
    if (FormWriteSets==NULL)
    {
        FormWriteSets=new frmIOForm(WRITE_SET_FORM,this);
    }
    FormWriteSets->init();
    FormWriteSets->show();
    FormWriteSets->raise();
    FormWriteSets->activateWindow();
}

void MainWindow::ExportBinary(void)
{
    if (FormWriteBinary==NULL)
    {
        FormWriteBinary=new frmIOForm(WRITE_BINARY_FILE,this);
    }
    FormWriteBinary->init();
    FormWriteBinary->show();
    FormWriteBinary->raise();
    FormWriteBinary->activateWindow();
}

void MainWindow::EvaluateExpr(void)
{
    if (FormEvalExpr==NULL)
    {
        FormEvalExpr=new frmEvalExpr(this,this);
    }
    FormEvalExpr->init();
    FormEvalExpr->show();
    FormEvalExpr->raise();
    FormEvalExpr->activateWindow();
}

void MainWindow::Histograms(void)
{
    if (FormHistograms==NULL)
    {
        FormHistograms=new frmInterpolation(HISTOGRAMSWINDOW,this);
    }
    FormHistograms->init();
    FormHistograms->show();
    FormHistograms->raise();
    FormHistograms->activateWindow();
}

void MainWindow::Fourier(void)
{
    if (FormFourier==NULL)
    {
        FormFourier=new frmFourier(this);
    }
    FormFourier->init();
    FormFourier->show();
    FormFourier->raise();
    FormFourier->activateWindow();
}

void MainWindow::Fourier2(void)
{
    if (FormFourier2==NULL)
    {
        FormFourier2=new frmFourier2(this);
        FormFourier2->init();
    }
    FormFourier2->show();
    FormFourier2->raise();
    FormFourier2->activateWindow();
}

void MainWindow::RunningAverages(void)
{
    if (FormAverages==NULL)
    {
        FormAverages=new frmSmallCalc(AVERAGESWINDOW,this);
    }
    FormAverages->init();
    FormAverages->show();
    FormAverages->raise();
    FormAverages->activateWindow();
}

void MainWindow::Differences(void)
{
    if (FormDifferences==NULL)
    {
        FormDifferences=new frmSmallCalc(DIFFERENCESWINDOW,this);
    }
    FormDifferences->init();
    FormDifferences->show();
    FormDifferences->raise();
    FormDifferences->activateWindow();
}

void MainWindow::SeasonalDiff(void)
{
    if (FormSeasonDiff==NULL)
    {
        FormSeasonDiff=new frmSmallCalc(SEASONALWINDOW,this);
    }
    FormSeasonDiff->init();
    FormSeasonDiff->show();
    FormSeasonDiff->raise();
    FormSeasonDiff->activateWindow();
}

void MainWindow::Integration(void)
{
    if (FormIntegration==NULL)
    {
        FormIntegration=new frmSmallCalc(INTEGRATIONWINDOW,this);
    }
    FormIntegration->init();
    FormIntegration->show();
    FormIntegration->raise();
    FormIntegration->activateWindow();
}

void MainWindow::Interpolation(void)
{
    if (FormInterpolation==NULL)
    {
        FormInterpolation=new frmInterpolation(INTERPOLATIONWINDOW,this);
    }
    FormInterpolation->init();
    FormInterpolation->show();
    FormInterpolation->raise();
    FormInterpolation->activateWindow();
}

void MainWindow::Regression(void)
{
    if (FormRegression==NULL)
    {
        FormRegression=new frmSmallCalc(REGRESSIONWINDOW,this);
    }
    FormRegression->init();
    FormRegression->show();
    FormRegression->raise();
    FormRegression->activateWindow();
}

void MainWindow::NonLinCurveFitting(void)
{
    if (FormNonlinCurveFit==NULL)
    {
        FormNonlinCurveFit=new frmNonlinCurveFit(this);
    }
    FormNonlinCurveFit->init();
    FormNonlinCurveFit->show();
    FormNonlinCurveFit->raise();
    FormNonlinCurveFit->activateWindow();
}

void MainWindow::do2DFitting(void)
{
    if (Form2DFit==NULL)
    {
        Form2DFit=new frm2DFit(this);
    }
    Form2DFit->init();
    Form2DFit->show();
    Form2DFit->raise();
    Form2DFit->activateWindow();
}

void MainWindow::ReportOnFitParameters(void)
{
    if (FormReportFitParameters==NULL)
    {
        FormReportFitParameters=new frmReportOnFitParameters(this);
        FormReportFitParameters->resize(420,350);
    }
    FormReportFitParameters->init();
    FormReportFitParameters->show();
    FormReportFitParameters->raise();
    FormReportFitParameters->activateWindow();
}

void MainWindow::Correlation(void)
{
    if(FormCorrelation==NULL)
    {
        FormCorrelation=new frmCorrelation(CORRELATIONWINDOW,this);
    }
    FormCorrelation->init();
    FormCorrelation->show();
    FormCorrelation->raise();
    FormCorrelation->activateWindow();
}

void MainWindow::DigitalFilter(void)
{
    if (FormDigitalFilter==NULL)
    {
        FormDigitalFilter=new frmCorrelation(FILTERWINDOW,this);
    }
    FormDigitalFilter->init();
    FormDigitalFilter->show();
    FormDigitalFilter->raise();
    FormDigitalFilter->activateWindow();
}

void MainWindow::LinConvolution(void)
{
    if (FormConvolution==NULL)
    {
        FormConvolution=new frmCorrelation(CONVOLUTIONWINDOW,this);
    }
    FormConvolution->init();
    FormConvolution->show();
    FormConvolution->raise();
    FormConvolution->activateWindow();
}

void MainWindow::GeomTransform(void)
{
    if (FormGeometricTransform==NULL)
    {
        FormGeometricTransform=new frmTransform(GEOMETRICWINDOW,this);
    }
    FormGeometricTransform->init();
    FormGeometricTransform->show();
    FormGeometricTransform->raise();
    FormGeometricTransform->activateWindow();
}

void MainWindow::GeomEval(void)
{
    if (FormGeometricEvaluation==NULL)
    {
        FormGeometricEvaluation=new frmGeometricEvaluation(this);
        //FormGeometricEvaluation->resize(FormGeometricEvaluation->width(),350);
        //FormGeometricEvaluation->setGeometry(FormGeometricEvaluation->x(),FormGeometricEvaluation->y(),FormGeometricEvaluation->width(),350);
    }
    FormGeometricEvaluation->init();
    FormGeometricEvaluation->show();
    FormGeometricEvaluation->raise();
    FormGeometricEvaluation->activateWindow();
}

void MainWindow::SamplePoints(void)
{
    if (FormSamplePoints==NULL)
    {
        FormSamplePoints=new frmTransform(SAMPLEPOINTSWINDOW,this);
    }
    FormSamplePoints->init();
    FormSamplePoints->show();
    FormSamplePoints->raise();
    FormSamplePoints->activateWindow();
}

void MainWindow::PruneData(void)
{
    if (FormPruneData==NULL)
    {
        FormPruneData=new frmTransform(PRUNEWINDOW,this);
    }
    FormPruneData->init();
    FormPruneData->show();
    FormPruneData->raise();
    FormPruneData->activateWindow();
}

void MainWindow::PlotAppearance(void)
{
    bool old_upd;
    old_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    if (FormPlotAppearance==NULL)
    {
        FormPlotAppearance=new frmPlotAppearance(this);
    }
    FormPlotAppearance->init();
    FormPlotAppearance->show();
    FormPlotAppearance->raise();
    FormPlotAppearance->activateWindow();
    immediateUpdate=old_upd;
    updateRunning=false;
}

void MainWindow::GraphAppearance(void)
{
    bool old_upd;
    old_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    if (FormGraphAppearance==NULL)
    {
        FormGraphAppearance=new frmGraphApp(this);
    }
    FormGraphAppearance->init();
    FormGraphAppearance->show();
    FormGraphAppearance->raise();
    FormGraphAppearance->activateWindow();
    immediateUpdate=old_upd;
    updateRunning=false;
}

void MainWindow::SetAppearance(void)
{
    bool old_upd;
    old_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    if (FormSetAppearance==NULL)
    {
        FormSetAppearance=new frmSetAppearance(this);
        FormSetAppearance->listSet->set_graph_number(get_cg(),false);
    }
    FormSetAppearance->init();
    FormSetAppearance->show();
    FormSetAppearance->raise();
    FormSetAppearance->activateWindow();
    immediateUpdate=old_upd;
    updateRunning=false;
}

void MainWindow::AxisProperties(void)
{
    bool old_upd;
    old_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    if (FormAxisProperties==NULL)
    {
        FormAxisProperties=new frmAxisProp(this);
    }
    FormAxisProperties->create_axes_dialog(0);
    FormAxisProperties->flp->tabs->setCurrentIndex(0);
    FormAxisProperties->show();
    FormAxisProperties->raise();
    FormAxisProperties->activateWindow();
    immediateUpdate=old_upd;
    updateRunning=false;
}

void MainWindow::LoadParameters(void)
{
QString openfilename;
    if (auto_set_cwd==TRUE)
    {
    openfilename=QString::fromLocal8Bit(get_workingdir());
    }
    else
    {
    openfilename=Last_Dialog_Path[READ_PARAMETERS];
    }
//qDebug() << "openfilename=" << openfilename;
if (should_open_grace_file_dialog())
{
    if (FormReadParameters==NULL)
    {
        FormReadParameters=new frmIOForm(READ_PARAMETERS,this);
        connect(FormReadParameters,SIGNAL(newFileSelectedForIO(int,QString,bool,bool,bool)),SLOT(IOrequested(int,QString,bool,bool,bool)));
    }
    FormReadParameters->init(openfilename.toLocal8Bit().data());
    FormReadParameters->show();
    FormReadParameters->raise();
    FormReadParameters->activateWindow();
}
else
{
mainWin->UseOperatingSystemFileDialog(READ_PARAMETERS,tr("QtGrace: Read parameters"),openfilename,tr("Parameter files (*.par);;All files (*)"));
}
}

void MainWindow::SaveParameters(void)
{
QString openfilename;
    if (auto_set_cwd==TRUE)
    {
    openfilename=QString::fromLocal8Bit(get_workingdir());
    }
    else
    {
    openfilename=Last_Dialog_Path[WRITE_PARAMETERS];
    }
//qDebug() << "openfilename=" << openfilename;
if (should_open_grace_file_dialog())
{
    if (FormWriteParameters==NULL)
    {
        FormWriteParameters=new frmIOForm(WRITE_PARAMETERS,this);
        connect(FormWriteParameters,SIGNAL(newFileSelectedForIO(int,QString,bool,bool,bool)),SLOT(IOrequested(int,QString,bool,bool,bool)));
    }
    FormWriteParameters->init(openfilename.toLocal8Bit().data());
    FormWriteParameters->show();
    FormWriteParameters->raise();
    FormWriteParameters->activateWindow();
}
else
{
mainWin->UseOperatingSystemFileDialog(WRITE_PARAMETERS,tr("QtGrace: Write parameters"),openfilename,tr("Parameter files (*.par);;All files (*)"));
}
}

void MainWindow::ShowLocBar(void)
{
show_LocatorBar=(actShowLocBar->isChecked()==true?TRUE:FALSE);
ManageBars();
}

void MainWindow::ShowStatusBar(void)
{
show_StatusBar=(actShowStatusBar->isChecked()==true?TRUE:FALSE);
ManageBars();
}

void MainWindow::ShowToolBar(void)
{
show_ToolBar=(actShowToolBar->isChecked()==true?TRUE:FALSE);
ManageBars();
}

void MainWindow::resizeAllIcons(void)
{
QSize newIconSize(default_IconW*toolBarSizeFactor,default_IconH*toolBarSizeFactor);

if (icon_preferences==0 || icon_preferences==1)//we actually have icons
{
cmdDraw->setIconSize(newIconSize);
cmdExit->setIconSize(newIconSize);
cmdAutoT->setIconSize(newIconSize);
cmdPan->setIconSize(newIconSize);
cmdPick->setIconSize(newIconSize);
cmdZz->setIconSize(newIconSize);
cmdzz->setIconSize(newIconSize);
cmdZoom->setIconSize(newIconSize);
cmdAutoScale->setIconSize(newIconSize);
cmdLeft->setIconSize(newIconSize);
cmdUp->setIconSize(newIconSize);
cmdViewUp->setIconSize(newIconSize);
cmdDown->setIconSize(newIconSize);
cmdViewDown->setIconSize(newIconSize);
cmdRight->setIconSize(newIconSize);
}

}

void MainWindow::ManageBars(void)
{
//show or hide the bars
    if (!show_LocatorBar)//Locator bar
    {
        statLocBar->hide();
        mainGrid->setRowMinimumHeight(1,0);
    }
    else
    {
        statLocBar->show();
        mainGrid->setRowMinimumHeight(1,stdBarHeight);
    }
    if (!show_StatusBar)//Status bar
    {
        statusBar->hide();
        mainGrid->setRowMinimumHeight(4,0);
    }
    else
    {
        statusBar->show();
        mainGrid->setRowMinimumHeight(4,stdBarHeight);
    }
    if (!show_ToolBar)//No tool bar
    {
        toolBar1->hide();
        toolBar2->hide();
        mainGrid->setColumnMinimumWidth(0,0);
    }
    else//Tool bar active
    {
        int SmallButtonW=defaultSmallButtonW*toolBarSizeFactor;
        int SmallButtonH=defaultSmallButtonH*toolBarSizeFactor;
        int LargeButtonW=defaultLargeButtonW*toolBarSizeFactor;
        int LargeButtonH=defaultLargeButtonH*toolBarSizeFactor;
        int ListW=defaultListW*toolBarSizeFactor;
        int ListH=defaultListH*toolBarSizeFactor;
        int SliderW=defaultSliderW*toolBarSizeFactor;
        int SliderH=defaultSliderH*toolBarSizeFactor;
        int BorderGap=defaultBorderGap*toolBarSizeFactor;
        //int VGap=defaultVGap*toolBarSizeFactor;
        int LeftGap;
        int ToolBarWidth=2*toolBar1->frameWidth()+2*BorderGap+LargeButtonW;
        //QIcon tmpIcon,newIcon;
        /*QFont cur_std_font=cmdAutoT->font();
        cur_std_font.setPixelSize(0.5*cmdAutoT->font().pixelSize());
        cmbViewStack->setFont(cur_std_font);
        cmdViewUp->setFont(cur_std_font);
        cmdViewDown->setFont(cur_std_font);*/

        LeftGap=(LargeButtonW-2*SmallButtonW)/3;
        //QSize newIconSize(default_IconW*toolBarSizeFactor,default_IconH*toolBarSizeFactor);
        ///Resizing buttons
        ///Large Buttons
#ifndef AUTOLAYOUT_FOR_TOOLBAR
    #ifdef WINDOWS_SYSTEM
        LargeButtonW+=WIN_SIZE_CORR*toolBarSizeFactor*2;
        cmdDraw->setGeometry(2+WIN_SIZE_CORR*toolBarSizeFactor,4,LargeButtonW,LargeButtonH);
    #else
        cmdDraw->setGeometry(2,4,LargeButtonW,LargeButtonH);
    #endif

        //cmdDraw->resize(LargeButtonW,LargeButtonH);
        /*qDebug() << "Draw: Iconsize=" << cmdDraw->iconSize();
        QIcon tmpIcon=cmdDraw->icon();
        QIcon newIcon(tmpIcon.pixmap(newIconSize));
        cmdDraw->setIconSize(newIconSize);
        cmdDraw->setIcon(newIcon);*/
        cmdAutoT->resize(LargeButtonW,LargeButtonH);
        cmdAutoO->resize(LargeButtonW,LargeButtonH);
        cmdFitPage->resize(LargeButtonW,LargeButtonH);
        cmdExit->resize(LargeButtonW,LargeButtonH);
        cmdExport->resize(LargeButtonW,LargeButtonH);
        cmdPrint->resize(LargeButtonW,LargeButtonH);
        cmdPan->resize(LargeButtonW,LargeButtonH);
        cmdPick->resize(LargeButtonW,LargeButtonH);

        ///Small Buttons
        cmdZoom->resize(SmallButtonW,SmallButtonH);
/*
        qDebug() << "Zoom: Iconsize=" << cmdZoom->iconSize() << "NewIconSize=" << newIconSize;
        tmpIcon=cmdZoom->icon();
        QPixmap pm1=tmpIcon.pixmap(newIconSize);
        QPixmap pm2=pm1.scaled(newIconSize);
        //newIcon=QIcon(tmpIcon.pixmap(newIconSize));
        newIcon=QIcon(pm2);
        qDebug() << "IconSize=" << newIcon.actualSize(newIconSize);
        cmdZoom->setIconSize(newIconSize);
        cmdZoom->setIcon(newIcon);
*/

        cmdAutoScale->resize(SmallButtonW,SmallButtonH);
        cmdZz->resize(SmallButtonW,SmallButtonH);
        cmdzz->resize(SmallButtonW,SmallButtonH);
        cmdLeft->resize(SmallButtonW,SmallButtonH);
        cmdRight->resize(SmallButtonW,SmallButtonH);
        cmdUp->resize(SmallButtonW,SmallButtonH);
        cmdDown->resize(SmallButtonW,SmallButtonH);
        cmdZX->resize(SmallButtonW,SmallButtonH);
        cmdZY->resize(SmallButtonW,SmallButtonH);
        cmdAX->resize(SmallButtonW,SmallButtonH);
        cmdAY->resize(SmallButtonW,SmallButtonH);
        cmdPZ->resize(SmallButtonW,SmallButtonH);
        cmdPu->resize(SmallButtonW,SmallButtonH);
        cmdPo->resize(SmallButtonW,SmallButtonH);
        cmdCy->resize(SmallButtonW,SmallButtonH);
        lblSD->resize(LargeButtonW,LargeButtonH);
        lblCW->resize(LargeButtonW,LargeButtonH);
        cmdUndo->resize(SmallButtonW,SmallButtonH);
        cmdRedo->resize(SmallButtonW,SmallButtonH);

        //cmdZoom->setIconSize(QSize(SmallButtonW,SmallButtonH));
        //cmdAutoScale->setIconSize(QSize(SmallButtonW,SmallButtonH));

        lblNewViewStack->resize(LargeButtonW,LargeButtonH*1.3);
        cmbViewStack->resize(LargeButtonW,LargeButtonH);
        cmdViewUp->resize(SmallButtonW,SmallButtonH);
        cmdViewDown->resize(SmallButtonW,SmallButtonH);
        cmdViewRename->resize(LargeButtonW,LargeButtonH);
        cmdViewAdd->resize(SmallButtonW,SmallButtonH);
        cmdViewRemove->resize(SmallButtonW,SmallButtonH);
        cmdViewReplace->resize(LargeButtonW,LargeButtonH);
        cmdViewRearrange->resize(LargeButtonW,LargeButtonH);

        lstGraphs->resize(ListW,ListH);
#endif
        //qDebug() << "Vor Slider Resize: w=" << SliderW << " h=" << SliderH;
        sldPageZoom->resize(SliderW,SliderH);
        sldPageZoom->resizeEvent(new QResizeEvent(QSize(SliderW,SliderH),sldPageZoom->size()));
        //qDebug() << "Nach Slider Resize: w=" << SliderW << " h=" << SliderH;
        /*toolBar1->setMinimumWidth(ToolBarWidth);
        toolBar2->setMinimumWidth(ToolBarWidth);*/

        mainGrid->setColumnMinimumWidth(0,ToolBarWidth*0.7);
        ///mainGrid->setColumnMinimumWidth(0,toolBarWidth*0.7*toolBarSizeFactor);
        ///toolBar1->resize(toolBarWidth*toolBarSizeFactor,278*toolBarSizeFactor);

        //rearrange/reposition the buttons
    int xpos,ypos,sav_stddist2;
    sav_stddist2=stdDistance2;
    stdDistance2=LeftGap;
        xpos=2+stdDistance2;
#ifdef WINDOWS_SYSTEM
xpos+=WIN_SIZE_CORR*toolBarSizeFactor;
#endif
        ypos=4+6+cmdDraw->height();
        //toolbar1
        if (show_Navi_B)
        {
#ifndef AUTOLAYOUT_FOR_TOOLBAR
        cmdZoom->move(xpos,ypos);
        cmdAutoScale->move(xpos+stdDistance2+cmdZoom->width(),ypos);
        ypos+=cmdZoom->height()+stdDistance1;
        cmdZz->move(xpos,ypos);
        cmdzz->move(xpos+stdDistance2+cmdZz->width(),ypos);
        ypos+=cmdZz->height()+stdDistance1;
        cmdLeft->move(xpos,ypos);
        cmdRight->move(xpos+cmdLeft->width()+stdDistance2,ypos);
        ypos+=cmdLeft->height()+stdDistance1;
        cmdUp->move(xpos,ypos);
        cmdDown->move(xpos+cmdUp->width()+stdDistance2,ypos);
        ypos+=cmdUp->height()+stdDistance1;
#endif
        cmdZoom->show();
        cmdAutoScale->show();
        cmdZz->show();
        cmdzz->show();
        cmdLeft->show();
        cmdRight->show();
        cmdUp->show();
        cmdDown->show();
        }
        else
        {
        cmdZoom->hide();
        cmdAutoScale->hide();
        cmdZz->hide();
        cmdzz->hide();
        cmdLeft->hide();
        cmdRight->hide();
        cmdUp->hide();
        cmdDown->hide();
        }
        if (show_PanButton==1)
        {
#ifndef AUTOLAYOUT_FOR_TOOLBAR
        cmdPan->move(cmdDraw->x(),ypos);
        ypos+=cmdPan->height()+stdDistance1;
#endif
        cmdPan->show();
        }
        else
        {
        cmdPan->hide();
        }
        if (show_PickButton==1)
        {
#ifndef AUTOLAYOUT_FOR_TOOLBAR
        cmdPick->move(cmdDraw->x(),ypos);
        ypos+=cmdPick->height()+stdDistance1;
#endif
        cmdPick->show();
        }
        else
        {
        cmdPick->hide();
        }
        if (show_Graph_List)
        {
/*
xpos=stdDistance2-2;
#ifdef WINDOWS_SYSTEM
xpos+=WIN_SIZE_CORR*toolBarSizeFactor;
#endif*/
xpos=cmdDraw->x();///corrected position
#ifdef WINDOWS_SYSTEM
xpos+=WIN_SIZE_CORR*toolBarSizeFactor;
#endif

#ifndef AUTOLAYOUT_FOR_TOOLBAR
        lstGraphs->move(xpos,ypos);
        ypos+=lstGraphs->height()+3;//2?
#endif
        lstGraphs->show();
        }
        else
        {
        lstGraphs->hide();
        }
        //cout << "stdRowHeight=" << stdRowHeight << endl;
        //cout << "berechnet=" << ypos << endl;
#ifndef AUTOLAYOUT_FOR_TOOLBAR
        mainGrid->setRowMinimumHeight(2,ypos);
        toolBar1->setMinimumHeight(ypos);
    #ifdef WINDOWS_SYSTEM
    toolBar1->resize(ToolBarWidth+WIN_SIZE_CORR*toolBarSizeFactor,ypos);
    #else
    toolBar1->resize(ToolBarWidth,ypos);
    #endif
        //toolbar2
        xpos=2;
    #ifdef WINDOWS_SYSTEM
    xpos+=WIN_SIZE_CORR*toolBarSizeFactor;
    #endif
        cmdAutoT->move(xpos,cmdAutoT->y());
        ypos=cmdAutoT->y()+cmdAutoT->height()+2;//position of AutoO
#endif
        if (show_special_Zoom)
        {
#ifndef AUTOLAYOUT_FOR_TOOLBAR
        cmdAutoO->move(xpos,ypos);
        xpos+=stdDistance2;
        ypos+=cmdAutoO->height()+stdDistance1-1;
        cmdZX->move(xpos,ypos);
        cmdZY->move(xpos+cmdZX->width()+stdDistance2,ypos);
        ypos+=cmdZX->height()+stdDistance1;
        cmdAX->move(xpos,ypos);
        cmdAY->move(xpos+cmdAX->width()+stdDistance2,ypos);
        ypos+=cmdAX->height()+stdDistance1;
#endif
        cmdAutoO->show();
        cmdZX->show();
        cmdZY->show();
        cmdAX->show();
        cmdAY->show();
        }
        else
        {
        cmdAutoO->hide();
        cmdZX->hide();
        cmdZY->hide();
        cmdAX->hide();
        cmdAY->hide();
        }

        if (show_Viewport_Stack==1)//Grace-Viewport-Stack
        {
#ifndef AUTOLAYOUT_FOR_TOOLBAR
        xpos=cmdAutoT->x()+stdDistance2;
        cmdPZ->move(xpos,ypos);
        cmdPu->move(xpos+cmdPZ->width()+stdDistance2,ypos);
        ypos+=cmdPZ->height()+stdDistance1;
        cmdPo->move(xpos,ypos);
        cmdCy->move(xpos+cmdPo->width()+stdDistance2,ypos);
        ypos+=cmdPo->height()+stdDistance1;
        lblSD->move(xpos,ypos);
        ypos+=lblSD->height();
        lblCW->move(xpos,ypos);
        ypos+=lblCW->height()+12;
#endif
        cmdPZ->show();
        cmdPu->show();
        cmdPo->show();
        cmdCy->show();
        lblSD->show();
        lblCW->show();

        lblNewViewStack->hide();
        cmbViewStack->hide();
        cmdViewUp->hide();
        cmdViewDown->hide();
        cmdViewRename->hide();
        cmdViewAdd->hide();
        cmdViewRemove->hide();
        cmdViewReplace->hide();
        cmdViewRearrange->hide();
        cmdViewRemove->hide();
        }
        else if (show_Viewport_Stack==2)//QtGrace-Viewport-Stack
        {
#ifndef AUTOLAYOUT_FOR_TOOLBAR
        xpos=cmdAutoT->x();
        lblNewViewStack->move(xpos,ypos);
        ypos+=lblNewViewStack->height()+stdDistance1;
        cmbViewStack->move(xpos,ypos);
        ypos+=cmbViewStack->height()+stdDistance1;
        xpos+=stdDistance2;
        cmdViewUp->move(xpos,ypos);
        xpos+=stdDistance2+cmdViewUp->width();
        cmdViewDown->move(xpos,ypos);
        ypos+=cmdViewUp->height()+stdDistance1;
        xpos=cmdViewUp->x();
        cmdViewAdd->move(xpos,ypos);
        //ypos+=cmdViewAdd->height()+stdDistance1;
        xpos+=stdDistance2+cmdViewUp->width();
        cmdViewRemove->move(xpos,ypos);
        xpos=cmdAutoT->x();
        ypos+=cmdViewAdd->height()+stdDistance1/2;
        cmdViewReplace->move(xpos,ypos);
        //xpos+=stdDistance2+cmdViewReplace->width();
        ypos+=cmdViewReplace->height()+stdDistance1/2;
        cmdViewRearrange->move(xpos,ypos);
        xpos=cmdAutoT->x();
        ypos+=cmdViewRearrange->height()+stdDistance1/2;
        cmdViewRename->move(xpos,ypos);
        ypos+=cmdViewRename->height()+stdDistance1;
#endif

        lblNewViewStack->show();
        cmbViewStack->show();
        cmdViewUp->show();
        cmdViewDown->show();
        cmdViewAdd->show();
        cmdViewRemove->show();
        cmdViewReplace->show();
        cmdViewRearrange->show();
        cmdViewRename->show();

        cmdPZ->hide();
        cmdPu->hide();
        cmdPo->hide();
        cmdCy->hide();
        lblSD->hide();
        lblCW->hide();
        }
        else
        {
        lblNewViewStack->hide();
        cmbViewStack->hide();
        cmdViewUp->hide();
        cmdViewDown->hide();
        cmdViewRename->hide();
        cmdViewAdd->hide();
        cmdViewRemove->hide();
        cmdViewReplace->hide();
        cmdViewRearrange->hide();
        cmdViewRemove->hide();

        cmdPZ->hide();
        cmdPu->hide();
        cmdPo->hide();
        cmdCy->hide();
        lblSD->hide();
        lblCW->hide();
        }
        xpos=cmdAutoT->x();
        if (show_UndoRedo_B)
        {
        ypos-=5;
        xpos=cmdZX->x();
#ifndef AUTOLAYOUT_FOR_TOOLBAR
        cmdUndo->move(xpos,ypos);
        cmdRedo->move(xpos+cmdPZ->width()+stdDistance2,ypos);
#endif
        cmdUndo->show();
        cmdRedo->show();
        xpos=cmdAutoT->x();
        ypos+=cmdUndo->height()+12;
        }
        else
        {
        cmdUndo->hide();
        cmdRedo->hide();
        }
        if (show_Page_Zoom)
        {
#ifndef AUTOLAYOUT_FOR_TOOLBAR
        sldPageZoom->move(xpos,ypos);
//qDebug() << "height=" << sldPageZoom->height() << " SLIDER_HEIGHT=" << SliderH;
        ypos+=sldPageZoom->height()+stdDistance1;
        cmdFitPage->move(xpos,ypos);
        ypos+=cmdFitPage->height()+stdDistance1;
#endif
        sldPageZoom->show();
        cmdFitPage->show();
        }
        else
        {
        sldPageZoom->hide();
        cmdFitPage->hide();
        }

        if (show_Export_B)
        {
#ifndef AUTOLAYOUT_FOR_TOOLBAR
        cmdExport->move(xpos,ypos);
        ypos+=cmdExport->height();
#endif
        cmdExport->show();
        }
        else
        {
        cmdExport->hide();
        }
        if (show_Print_B)
        {
#ifndef AUTOLAYOUT_FOR_TOOLBAR
        cmdPrint->move(xpos,ypos);
        ypos+=cmdPrint->height();
#endif
        cmdPrint->show();
        }
        else
        {
        cmdPrint->hide();
        }
#ifndef AUTOLAYOUT_FOR_TOOLBAR
        ypos+=int(8.0/5.0*stdDistance1);
        cmdExit->move(xpos,ypos);
        stdDistance2=sav_stddist2;
#endif
        cmdExit->show();
        //show the tool bar
        toolBar1->show();
        toolBar2->show();
    }

    /*qDebug() << "stdDistance1=" << stdDistance1 << " stdHeight=" << stdHeight1 << " stdHeight2=" << stdHeight2;
    qDebug() << "stdDistance2=" << stdDistance2 << " stdWidth2=" << stdWidth2;
    qDebug() << "Draw: " << cmdDraw->width() << " x " << cmdDraw->height();
    qDebug() << "cmdUp: " << cmdUp->width() << " x " << cmdUp->height();
    qDebug() << "List: " << lstGraphs->width() << " x " << lstGraphs->height();
    qDebug() << "Slider: " << sldPageZoom->width() << " x " << sldPageZoom->height();
    qDebug() << "toolBar1: " << toolBar1->width() << " x " << toolBar1->height();
    qDebug() << "toolBar1.frame.width: " << toolBar1->frameWidth();
    qDebug() << "Draw.pos: " << cmdDraw->x() << " | " << cmdDraw->y();
    qDebug() << "Zoom.pos: " << cmdZoom->x() << " | " << cmdZoom->y();
    qDebug() << "BEFORE RESIZE";*/
resizeAllIcons();
resizeEvent(new QResizeEvent(this->size(),this->size()));
    //qDebug() << "AFTER RESIZE";
}

void MainWindow::PageSetup(void)
{
//bool initiate=false;
    if (FormDeviceSetup==NULL)
    {
        FormDeviceSetup=new frmDeviceSetup(this);
        //initiate=true;
    }
    FormDeviceSetup->show();
        if (FormDeviceSetup->devices_item->currentValue()!=DEVICE_SCREEN)//'0'=Screen
        lastPrintDevice=FormDeviceSetup->devices_item->currentValue();
    FormDeviceSetup->changeDeviceList(1);
    FormDeviceSetup->devices_item->setCurrentValue(find_dev_nr("X11"));
    FormDeviceSetup->devices_item->setEnabled(false);
    FormDeviceSetup->DeviceChanged(DEVICE_SCREEN);
    FormDeviceSetup->raise();
    FormDeviceSetup->activateWindow();
    FormDeviceSetup->grpPage->adjustSize();
    FormDeviceSetup->grpFonts->adjustSize();
    FormDeviceSetup->adjustSize();
    //if (initiate==true)
    //FormDeviceSetup->resize(400,400);
}

void MainWindow::Redraw(void)
{
    mainArea->completeRedraw();
}

void MainWindow::UpdateAll(void)
{
    update_all();
}

void MainWindow::Commands(void)
{
    if (FormCommands==NULL)
    {
        FormCommands=new frmCommands(this);
        update_all();
    }
    FormCommands->show();
    FormCommands->raise();
    FormCommands->activateWindow();
}

void MainWindow::PointExplorer(void)
{
    if (FormPointExplorer==NULL)
    {
        FormPointExplorer=new frmPointExplorer(this);
    }
    FormPointExplorer->init();
    FormPointExplorer->show();
    FormPointExplorer->resize(FormPointExplorer->width(),FormPointExplorer->width()/3);
    FormPointExplorer->raise();
    FormPointExplorer->activateWindow();
}

void MainWindow::DrawingObjects(void)
{
    if (FormDrawObjects==NULL)
    {
        FormDrawObjects=new frmDrawObjects(this);
    }
    FormDrawObjects->show();
    FormDrawObjects->raise();
    FormDrawObjects->activateWindow();
}

void MainWindow::FontTool(void)
{
    if (FormFontTool==NULL)
    {
        FormFontTool=new frmFontTool(this);
        //FormFontTool->resize(460,570);
    }
    FormFontTool->show();
    FormFontTool->raise();
    FormFontTool->activateWindow();
}

void MainWindow::Console(void)
{
    if (FormConsole==NULL)
    {
        FormConsole=new frmConsole(this);
    //qDebug() << "A Console=" << FormConsole->size();
    }
    FormConsole->show();
    FormConsole->raise();
    FormConsole->activateWindow();
    //qDebug() << "B Console=" << FormConsole->size();
}

void MainWindow::HelpOnContext(void)
{
    //setCursor(QCursor(Qt::WhatsThisCursor));
    set_cursor(CURSOR_WHAT);
}

void MainWindow::HelpUsersGuide(void)
{
    QString helpfile=QString(qt_grace_doc_dir)+QDir::separator()+QString("UsersGuide.html");
    HelpCB(helpfile.toLocal8Bit().constData());
}

void MainWindow::HelpTutorial(void)
{
    QString helpfile=QString(qt_grace_doc_dir)+QDir::separator()+QString("Tutorial.html");
    HelpCB(helpfile.toLocal8Bit().constData());
}

void MainWindow::HelpFAQ(void)
{
    QString helpfile=QString(qt_grace_doc_dir)+QDir::separator()+QString("FAQ.html");
    HelpCB(helpfile.toLocal8Bit().constData());
}

void MainWindow::HelpChanges(void)
{
    QString helpfile=QString(qt_grace_doc_dir)+QDir::separator()+QString("CHANGES.html");
    HelpCB(helpfile.toLocal8Bit().constData());
}

void MainWindow::HelpQtGrace(void)
{
    /*
    //The following command was meant for testing
    for (int i=0;i<nr_of_Examples;i++)
    HelpOpenExample(i);
    */
    QString helpfile=QString(qt_grace_doc_dir)+QDir::separator()+QString("QTGRACE_EXTENSIONS.html");
    HelpCB(helpfile.toLocal8Bit().constData());
}

void MainWindow::HelpComments(void)
{
    char buf[256];
    sprintf(buf,"http://plasma-gate.weizmann.ac.il/Grace/comments.phtml?version_id=%ld",bi_version_id());
    HelpCB((const char*)buf);
}

void MainWindow::HelpLicense(void)
{
    QString helpfile=QString(qt_grace_doc_dir)+QDir::separator()+QString("GPL.html");
    HelpCB(helpfile.toLocal8Bit().constData());
}

void MainWindow::HelpReadme(void)
{
    QString helpfile=QString(qt_grace_doc_dir)+QDir::separator()+QString("README_QTGRACE.txt");
    HelpCB(helpfile.toLocal8Bit().constData());
}

void MainWindow::HelpAbout(void)
{
    if (FormAbout==NULL)
    {
        FormAbout=new frmAbout(this);
    }
    FormAbout->show();
    FormAbout->raise();
    FormAbout->activateWindow();
}

void MainWindow::HelpOpenExample(int i)
{
    if (i<0 || i>nr_of_Examples) return;
    char buf[2056];
    //char buf2[2056];
    char ex_name[2056];
    stop_repaint=TRUE;
    strcpy(ex_name,examplesFiles[i].toLocal8Bit());
    sprintf(buf, "%s/%s",qt_grace_example_dir, ex_name);
    //sprintf(buf2, "%s/examples",qt_grace_share_dir);
    //set_workingdir(buf2);
    QFile file(buf);
    if (!file.exists())
    {
        QString errorText=tr("Example file \"")+QString(buf)+tr("\" does not exist!");
        //cout << "Example file " << buf <<  " does not exist!" << endl;
        errmsg(errorText.toLocal8Bit().constData());
    }
    else
    {
        //load_project_file(buf, FALSE);
        //update_default_props();
        load_project(buf);
            int use_dev=hdevice;
            if (FormDeviceSetup!=NULL) use_dev=FormDeviceSetup->cur_dev;
            QString fwe=get_filename_with_extension(use_dev);
            strcpy(print_file,fwe.toLocal8Bit().constData());
            if (FormDeviceSetup!=NULL)
            {
            FormDeviceSetup->printfile_item->lenText->setText(fwe);
            FormDeviceSetup->init(FormDeviceSetup->devices_item->currentValue());
            }
    }
    initNodes();
    stop_repaint=FALSE;
        if (autofit_on_load==TRUE)
        doFitPage();
        else
        mainArea->completeRedraw();
/*mainArea->setGeometry(mainArea->x(),mainArea->y(),mainArea->width(),mainArea->height());*/
    //update_all();
}

void MainWindow::doDraw(void)
{
    simple_draw_setting=SIMPLE_DRAW_NONE;
//mainArea->completeRedraw();
    force_redraw();
    mainArea->scroll->setFocus();
//qDebug() << "Action_flag=" << (int)action_flag;
}

void MainWindow::doZoom(void)
{
    Pick_and_Move_active=false;
    if (action_flag==PICK_MOVE || action_flag==PAN_1ST || action_flag==PAN_2ND)
    set_action(DO_NOTHING);
    if (action_flag==ZOOM_1ST || action_flag==ZOOM_2ND)
    set_action(DO_NOTHING);
    else
    set_action(ZOOM_1ST);
    mainArea->scroll->setFocus();
}

void MainWindow::doAutoScale(void)
{
    Pick_and_Move_active=false;
    if (action_flag==PICK_MOVE || action_flag==PAN_1ST || action_flag==PAN_2ND)
    set_action(DO_NOTHING);
    autoscale_proc(AUTOSCALE_XY);
    mainArea->scroll->setFocus();
}

void MainWindow::doZz(void)
{
    Pick_and_Move_active=false;
    if (action_flag==PICK_MOVE || action_flag==PAN_1ST || action_flag==PAN_2ND)
    set_action(DO_NOTHING);
    graph_zoom_proc(GZOOM_EXPAND);
    mainArea->scroll->setFocus();
}

void MainWindow::dozz(void)
{
    Pick_and_Move_active=false;
    if (action_flag==PICK_MOVE || action_flag==PAN_1ST || action_flag==PAN_2ND)
    set_action(DO_NOTHING);
    graph_zoom_proc(GZOOM_SHRINK);
    mainArea->scroll->setFocus();
}

void MainWindow::doLeft(void)
{
    Pick_and_Move_active=false;
    if (action_flag==PICK_MOVE || action_flag==PAN_1ST || action_flag==PAN_2ND)
    set_action(DO_NOTHING);
    graph_scroll_proc(GSCROLL_LEFT);
    mainArea->scroll->setFocus();
}

void MainWindow::doRight(void)
{
    Pick_and_Move_active=false;
    if (action_flag==PICK_MOVE || action_flag==PAN_1ST || action_flag==PAN_2ND)
    set_action(DO_NOTHING);
    graph_scroll_proc(GSCROLL_RIGHT);
    mainArea->scroll->setFocus();
}

void MainWindow::doUp(void)
{
    Pick_and_Move_active=false;
    if (action_flag==PICK_MOVE || action_flag==PAN_1ST || action_flag==PAN_2ND)
    set_action(DO_NOTHING);
    graph_scroll_proc(GSCROLL_UP);
    mainArea->scroll->setFocus();
}

void MainWindow::doDown(void)
{
    Pick_and_Move_active=false;
    if (action_flag==PICK_MOVE || action_flag==PAN_1ST || action_flag==PAN_2ND)
    set_action(DO_NOTHING);
    graph_scroll_proc(GSCROLL_DOWN);
    mainArea->scroll->setFocus();
}

void MainWindow::doPan(void)
{
    Pick_and_Move_active=false;
    if (action_flag==PAN_1ST || action_flag==PAN_2ND)
    set_action(DO_NOTHING);
    else
    set_action(PAN_1ST);
    mainArea->scroll->setFocus();
}

void MainWindow::doPick(void)
{
    if (action_flag==PICK_MOVE || Pick_and_Move_active==true)
    {
    Pick_and_Move_active=false;
    set_action(DO_NOTHING);
    }
    else
    {
    set_action(PICK_MOVE);
    }
    //this->cmdPan->setDown(false);
    mainArea->scroll->setFocus();
}

void MainWindow::doAutoT(void)
{
    Pick_and_Move_active=false;
    if (action_flag==PICK_MOVE || action_flag==PAN_1ST || action_flag==PAN_2ND)
    set_action(DO_NOTHING);
    autoticks_proc();
    /*char dummy[256];
    SaveTickmarksStatesPrevious(X_AXIS,Y_AXIS,get_cg(),get_cg());
    autotick_axis(get_cg(), ALL_AXES);
        if (FormAxisProperties!=NULL)
        FormAxisProperties->update_ticks(get_cg());
    TickmarksModified(X_AXIS,Y_AXIS,get_cg(),get_cg());
    sprintf(dummy,"Autotick axes [G%d]",get_cg());
    addAditionalDescriptionToLastNode(-1,QString(dummy),QString(),0);
    mainArea->completeRedraw();
    mainArea->scroll->setFocus();*/
}

void MainWindow::doAutoO(void)
{
    Pick_and_Move_active=false;
    if (action_flag==AUTO_NEAREST)
    set_action(DO_NOTHING);
    else
    {
    set_action(DO_NOTHING);
    set_action(AUTO_NEAREST);
    }
    mainArea->scroll->setFocus();
}

void MainWindow::doZX(void)
{
    Pick_and_Move_active=false;
    if (action_flag==PICK_MOVE || action_flag==PAN_1ST || action_flag==PAN_2ND)
    set_action(DO_NOTHING);
    if (action_flag==ZOOMX_1ST || action_flag==ZOOMX_2ND)
    set_action(DO_NOTHING);
    else
    set_action(ZOOMX_1ST);
    mainArea->scroll->setFocus();
}

void MainWindow::doZY(void)
{
    Pick_and_Move_active=false;
    if (action_flag==PICK_MOVE || action_flag==PAN_1ST || action_flag==PAN_2ND)
    set_action(DO_NOTHING);
    if (action_flag==ZOOMY_1ST || action_flag==ZOOMY_2ND)
    set_action(DO_NOTHING);
    else
    set_action(ZOOMY_1ST);
    mainArea->scroll->setFocus();
}

void MainWindow::doAX(void)
{
    Pick_and_Move_active=false;
    if (action_flag==PICK_MOVE || action_flag==PAN_1ST || action_flag==PAN_2ND)
    set_action(DO_NOTHING);
    autoscale_proc(AUTOSCALE_X);
    mainArea->scroll->setFocus();
}

void MainWindow::doAY(void)
{
    Pick_and_Move_active=false;
    if (action_flag==PICK_MOVE || action_flag==PAN_1ST || action_flag==PAN_2ND)
    set_action(DO_NOTHING);
    autoscale_proc(AUTOSCALE_Y);
    mainArea->scroll->setFocus();
}

void MainWindow::doPZ(void)
{
    Pick_and_Move_active=false;
    world_stack_proc(WSTACK_PUSH_ZOOM);
}

void MainWindow::doPu(void)
{
    Pick_and_Move_active=false;
    world_stack_proc(WSTACK_PUSH);
}

void MainWindow::doPo(void)
{
    Pick_and_Move_active=false;
    world_stack_proc(WSTACK_POP);
}

void MainWindow::doCy(void)
{
    Pick_and_Move_active=false;
    world_stack_proc(WSTACK_CYCLE);
}

void MainWindow::doPageZoom(int i)
{
    GeneralPageZoomFactor=pow(10.0,i*sldPageZoom->ScalingFactor);
    //device_table[DEVICE_SCREEN].pg.width=orig_page_w*GeneralPageZoomFactor;
    //device_table[DEVICE_SCREEN].pg.height=orig_page_h*GeneralPageZoomFactor;
    mainArea->completeRedraw();
    mainArea->scroll->setFocus();
}

void MainWindow::doFitPage(void)
{
    // a) calculate proper zoom level
    double factorX =  double(mainArea->scroll->width()) / double(device_table[DEVICE_SCREEN].pg.width);
    double factorY =  double(mainArea->scroll->height()) / double(device_table[DEVICE_SCREEN].pg.height);
    /*double factorX =  double(mainArea->scroll->width()) / double(orig_page_w);
    double factorY =  double(mainArea->scroll->height()) / double(orig_page_h);*/
    double fact = factorX > factorY ? factorY : factorX;
    //fact = the smaller factor
    int zoomLevel = ceil(log(fact) / (sldPageZoom->ScalingFactor * log(10.)) -0.5)-1;/// round or int?
    if (zoomLevel<0) zoomLevel--;
    zoomLevel = zoomLevel > 100 ? 100 : (zoomLevel < -100 ? -100 : zoomLevel);

//cout << "In doFitPage: w=" << mainArea->scroll->width() << " h=" << mainArea->scroll->height() << endl;

    //cout << "x-factor=" << factorX << " y_factor=" << factorY << " zoomLevel=" << zoomLevel << " Umrechnung=" << (log(fact) / (sldPageZoom->ScalingFactor * log(10.))) << endl;

    // b) reset position sliders to 0
    mainArea->scroll->horizontalScrollBar()->setValue(0);
    mainArea->scroll->verticalScrollBar()->setValue(0);

    // c) change zoom-level
    if (enableServerMode)
    sldPageZoom->setValue(zoomLevel-2);//-1, -2 or -5 ?
    else
    sldPageZoom->setValue(zoomLevel);//as calculated
    mainArea->scroll->setFocus();
}

void MainWindow::doExit(void)
{
#ifdef DEBUG_OUT_LOG
debug_out << "Exit clicked in MainWindow." << endl;
#endif
    Exit();
}

void setExportTypeDescription(const char * ext)
{
mainWin->actPrintToFile->setText(QObject::tr("Export to &File")+QString(" (")+QString(ext)+QString(")"));
//mainWin->mnuFile->update();
}

void MainWindow::CreateActions(void)
{
    //Actions for File menu
    actNew = new QAction(tr("&New"), this);
    actNew->setShortcut(tr("Ctrl+N"));
    //actNew->setToolTip(tr("Create a new file"));
    actNew->setStatusTip(tr("Create a new file"));
    connect(actNew, SIGNAL(triggered()), this, SLOT(newFile()));
    actOpen= new QAction(tr("&Open..."), this);
    actOpen->setShortcut(tr("Ctrl+O"));
    actOpen->setStatusTip(tr("Open an existing file"));
    connect(actOpen, SIGNAL(triggered()), this, SLOT(Open()));
    actSave= new QAction(tr("&Save"), this);
    actSave->setShortcut(tr("Ctrl+S"));
    actSave->setStatusTip(tr("Save current data"));
    connect(actSave, SIGNAL(triggered()), this, SLOT(Save()));
    actSaveAs= new QAction(tr("S&ave As..."), this);
    actSaveAs->setStatusTip(tr("Save to new file"));
    connect(actSaveAs, SIGNAL(triggered()), this, SLOT(SaveAs()));
    actRevert= new QAction(tr("Re&vert to Saved"), this);
    actRevert->setStatusTip(tr("Revert to version last saved"));
    connect(actRevert, SIGNAL(triggered()), this, SLOT(RevertToSaved()));
    actPrint= new QAction(tr("&Print"), this);
    actPrint->setShortcut(tr("Ctrl+P"));
    actPrint->setStatusTip(tr("Print on physical printer using the default resolution (allways full page)"));
    connect(actPrint, SIGNAL(triggered()), this, SLOT(Print()));
    /*actPrintSpecial=new SpinBoxAction(tr("Print"),tr("dpi"),this);
    actPrintSpecial->setStatusTip(tr("Print on physical printer with given resolution."));
    connect(actPrintSpecial, SIGNAL(triggered()), this, SLOT(PrintSpecial()));*/
    actPrintToFile=new QAction(tr("Export to &File"),this);
    actPrintToFile->setShortcut(tr("Ctrl+F"));
    actPrintToFile->setStatusTip(tr("Export image to file"));
    connect(actPrintToFile, SIGNAL(triggered()), this, SLOT(PrintToFile()));
    actPrintSetup= new QAction(tr("File Expor&t Setup..."), this);
    actPrintSetup->setStatusTip(tr("Adjust export properties"));
    connect(actPrintSetup, SIGNAL(triggered()), this, SLOT(PrintSetup()));
    actExit= new QAction(tr("E&xit"), this);
    actExit->setShortcut(tr("Ctrl+Q"));
    actExit->setStatusTip(tr("Exit program"));
    connect(actExit, SIGNAL(triggered()), this, SLOT(Exit()));

    //Actions for Edit menu
    actDataSets= new QAction(tr( "&Data sets..."), this);
    //->setStatusTip(tr());
    connect(actDataSets, SIGNAL(triggered()), this, SLOT(DataSets()));
    actSetOperations= new QAction(tr("Set &operations..." ), this);
    connect(actSetOperations, SIGNAL(triggered()), this, SLOT(SetOperations()));
    actArrangeGraphs= new QAction(tr( "A&rrange graphs..."), this);
    connect(actArrangeGraphs, SIGNAL(triggered()), this, SLOT(ArrangeGraphs()));
    actOverlayGraphs= new QAction(tr("&Overlay graphs..." ), this);
    connect(actOverlayGraphs, SIGNAL(triggered()), this, SLOT(OverlayGraphs()));
    actAutoscaleGraphs= new QAction(tr("&Autoscale graphs..." ), this);
    connect(actAutoscaleGraphs, SIGNAL(triggered()), this, SLOT(AutoscaleGraphs()));
    actRegionMaster= new QAction(tr("Region &Master..." ), this);
    connect(actRegionMaster, SIGNAL(triggered()), this, SLOT(RegionsMaster()));
    actRegionsStatus= new QAction(tr("&Status..." ), this);
    connect(actRegionsStatus, SIGNAL(triggered()), this, SLOT(RegionsStatus()));
    actRegionsDefine= new QAction(tr("&Define..." ), this);
    connect(actRegionsDefine, SIGNAL(triggered()), this, SLOT(RegionsDefine()));
    actRegionsClear= new QAction(tr("&Clear..." ), this);
    connect(actRegionsClear, SIGNAL(triggered()), this, SLOT(RegionsClear()));
    actRegionsReportOn= new QAction(tr("&Report on..." ), this);
    connect(actRegionsReportOn, SIGNAL(triggered()), this, SLOT(RegionsReportOn()));
    actHotLinks= new QAction(tr( "Hot &links..."), this);
    connect(actHotLinks, SIGNAL(triggered()), this, SLOT(HotLinks()));
    actSetLocFixPoint= new QAction(tr("Set locator &fixed point" ), this);
    connect(actSetLocFixPoint, SIGNAL(triggered()), this, SLOT(SetLocFixPoint()));
    actClearLocFixPoint= new QAction(tr( "&Clear locator fixed point"), this);
    connect(actClearLocFixPoint, SIGNAL(triggered()), this, SLOT(ClearLocFixPoint()));
    actLocProp= new QAction(tr("Locator &props..." ), this);
    connect(actLocProp, SIGNAL(triggered()), this, SLOT(LocProp()));
    actPreferences= new QAction(tr("P&references..." ), this);
    connect(actPreferences, SIGNAL(triggered()), this, SLOT(Preferences()));
    actExplorer=new QAction(tr("Explorer"),this);
    connect(actExplorer,SIGNAL(triggered()),this,SLOT(Explorer()));

    //Actions for the Data menu
    actDataSetOperations= new QAction(tr("Data set &operations..." ), this);
    connect(actDataSetOperations, SIGNAL(triggered()), this, SLOT(DataSetOperations()));
    actFeatureExtraction= new QAction(tr("Feature e&xtraction..." ), this);
    connect(actFeatureExtraction, SIGNAL(triggered()), this, SLOT(FeatureExtraction()));
    actExportAscii= new QAction(tr("&ASCII..." ), this);
    connect(actExportAscii, SIGNAL(triggered()), this, SLOT(ExportAscii()));
    actExportBinary= new QAction(tr("&Binary..." ), this);
    connect(actExportBinary, SIGNAL(triggered()), this, SLOT(ExportBinary()));
    actImportNetCDF= new QAction(tr("&NetCDF..." ), this);
    connect(actImportNetCDF, SIGNAL(triggered()), this, SLOT(ImportNetCDF()));
    actImportAscii= new QAction(tr("&ASCII..." ), this);
    connect(actImportAscii, SIGNAL(triggered()), this, SLOT(ImportAscii()));
    actImportCSV= new QAction(tr("&CSV / ASCII..." ), this);
    connect(actImportCSV, SIGNAL(triggered()), this, SLOT(ImportCSV()));
    actImportBinary= new QAction(tr("&Binary..." ), this);
    connect(actImportBinary, SIGNAL(triggered()), this, SLOT(ImportBinary()));
    actImportAgr= new QAction(tr("&Set from agr..." ), this);
    connect(actImportAgr, SIGNAL(triggered()), this, SLOT(ImportAgr()));
    actEvaluateExpr= new QAction(tr("&Evaluate expression..." ), this);
    connect(actEvaluateExpr, SIGNAL(triggered()), this, SLOT(EvaluateExpr()));
    actHistograms= new QAction(tr("&Histograms..." ), this);
    connect(actHistograms, SIGNAL(triggered()), this, SLOT(Histograms()));
    actFourier= new QAction(tr("Fo&urier transforms..." ), this);
    connect(actFourier, SIGNAL(triggered()), this, SLOT(Fourier()));
    actFourier2= new QAction(tr("Filters..." ), this);
    connect(actFourier2, SIGNAL(triggered()), this, SLOT(Fourier2()));

    actRunningAverages= new QAction(tr("&Running &averages..." ), this);
    connect(actRunningAverages, SIGNAL(triggered()), this, SLOT(RunningAverages()));
    actDifferences= new QAction(tr("&Differences..." ), this);
    connect(actDifferences, SIGNAL(triggered()), this, SLOT(Differences()));
    actSeasonalDiff= new QAction(tr("Seas&onal differences..." ), this);
    connect(actSeasonalDiff, SIGNAL(triggered()), this, SLOT(SeasonalDiff()));
    actIntegration= new QAction(tr("&Integration..." ), this);
    connect(actIntegration, SIGNAL(triggered()), this, SLOT(Integration()));
    actInterpolation= new QAction(tr("In&terpolation/splines..." ), this);
    connect(actInterpolation, SIGNAL(triggered()), this, SLOT(Interpolation()));
    actRegression= new QAction(tr( "&Regression..."), this);
    connect(actRegression, SIGNAL(triggered()), this, SLOT(Regression()));
    actNonLinCurveFitting= new QAction(tr("&Non-linear curve fitting..." ), this);
    connect(actNonLinCurveFitting, SIGNAL(triggered()), this, SLOT(NonLinCurveFitting()));
    act2DFitting= new QAction(tr("Fit 2D-objects..." ), this);
    connect(act2DFitting, SIGNAL(triggered()), this, SLOT(do2DFitting()));
    actReportFitParameters= new QAction(tr("Report on fit parameters..." ), this);
    connect(actReportFitParameters, SIGNAL(triggered()), this, SLOT(ReportOnFitParameters()));
    actCorrelation= new QAction(tr("&Correlation/covariance..." ), this);
    connect(actCorrelation, SIGNAL(triggered()), this, SLOT(Correlation()));
    actDigitalFilter= new QAction(tr("Digital &filter..." ), this);
    connect(actDigitalFilter, SIGNAL(triggered()), this, SLOT(DigitalFilter()));
    actLinConvolution= new QAction(tr( "Linear con&volution..."), this);
    connect(actLinConvolution, SIGNAL(triggered()), this, SLOT(LinConvolution()));
    actGeomTransform= new QAction(tr("&Geometric transforms..." ), this);
    connect(actGeomTransform, SIGNAL(triggered()), this, SLOT(GeomTransform()));
    actGeomEval= new QAction(tr("Geometric &evaluations..." ), this);
    connect(actGeomEval, SIGNAL(triggered()), this, SLOT(GeomEval()));
    actSamplePoints= new QAction(tr("Sa&mple points..." ), this);
    connect(actSamplePoints, SIGNAL(triggered()), this, SLOT(SamplePoints()));
    actPruneData= new QAction(tr("&Prune data..." ), this);
    connect(actPruneData, SIGNAL(triggered()), this, SLOT(PruneData()));

    //Actions for the Plot menu
    actPlotAppearance= new QAction(tr("Plot a&ppearance..." ), this);
    connect(actPlotAppearance, SIGNAL(triggered()), this, SLOT(PlotAppearance()));
    actGraphAppearance= new QAction(tr("&Graph appearance..." ), this);
    connect(actGraphAppearance, SIGNAL(triggered()), this, SLOT(GraphAppearance()));
    actSetAppearance= new QAction(tr("&Set appearance..." ), this);
    connect(actSetAppearance, SIGNAL(triggered()), this, SLOT(SetAppearance()));
    actAxisProperties= new QAction(tr("A&xis properties..." ), this);
    connect(actAxisProperties, SIGNAL(triggered()), this, SLOT(AxisProperties()));
    actLoadParameters= new QAction(tr("&Load parameters..." ), this);
    connect(actLoadParameters, SIGNAL(triggered()), this, SLOT(LoadParameters()));
    actSaveParameters= new QAction(tr( "&Save parameters..."), this);
    connect(actSaveParameters, SIGNAL(triggered()), this, SLOT(SaveParameters()));
    actColManager= new QAction(tr( "Color management..."), this);
    connect(actColManager, SIGNAL(triggered()), this, SLOT(ColorManager()));
    actRealTimeInput= new QAction(tr( "Monitor real time input..."), this);
    connect(actRealTimeInput, SIGNAL(triggered()), this, SLOT(RealTimeInputDisplay()));

    //Actions for the View menu
    actShowLocBar= new QAction(tr("Show locator bar" ), this);
    actShowLocBar->setCheckable(TRUE);
    actShowLocBar->setChecked(TRUE);
    connect(actShowLocBar, SIGNAL(triggered()), this, SLOT(ShowLocBar()));
    actShowStatusBar= new QAction(tr("Show status bar" ), this);
    actShowStatusBar->setCheckable(true);
    actShowStatusBar->setChecked(TRUE);
    connect(actShowStatusBar, SIGNAL(triggered()), this, SLOT(ShowStatusBar()));
    actShowToolBar= new QAction(tr("Show tool bar" ), this);
    actShowToolBar->setCheckable(TRUE);
    actShowToolBar->setChecked(TRUE);
    connect(actShowToolBar, SIGNAL(triggered()), this, SLOT(ShowToolBar()));
    actPageSetup= new QAction(tr("&Page setup" ), this);
    connect(actPageSetup, SIGNAL(triggered()), this, SLOT(PageSetup()));
    actRedraw= new QAction(tr("&Redraw" ), this);
    connect(actRedraw, SIGNAL(triggered()), this, SLOT(Redraw()));
    actUpdateAll= new QAction(tr("&Update all" ), this);
    connect(actUpdateAll, SIGNAL(triggered()), this, SLOT(UpdateAll()));

    //Actions for the Window menu
    actCommands= new QAction(tr("&Commands" ), this);
    connect(actCommands, SIGNAL(triggered()), this, SLOT(Commands()));
    actPointExplorer= new QAction(tr("&Point explorer" ), this);
    connect(actPointExplorer, SIGNAL(triggered()), this, SLOT(PointExplorer()));
    actDrawingObjects= new QAction(tr("&Drawing objects" ), this);
    connect(actDrawingObjects, SIGNAL(triggered()), this, SLOT(DrawingObjects()));
    actFontTool= new QAction(tr("&Font tool" ), this);
    connect(actFontTool, SIGNAL(triggered()), this, SLOT(FontTool()));
    actConsole= new QAction(tr("&Console" ), this);
    connect(actConsole, SIGNAL(triggered()), this, SLOT(Console()));

    //Undo-Actions
    actUndo= new QAction(tr("&UnDo" ), this);
    actUndo->setShortcut(tr("Ctrl+Z"));
    connect(actUndo, SIGNAL(triggered()), this, SLOT(doUndo()));
    actRedo= new QAction(tr("&ReDo" ), this);
    actRedo->setShortcut(tr("Shift+Ctrl+Z"));
    connect(actRedo, SIGNAL(triggered()), this, SLOT(doRedo()));
    actUndoList= new QAction(tr("UndoList" ), this);
    connect(actUndoList, SIGNAL(triggered()), this, SLOT(doUndoList()));

    //Actions for the Help menu
    actHelpOnContext= new QAction(tr("On conte&xt" ), this);
    actHelpOnContext->setShortcut(tr("Shift+F1"));
    connect(actHelpOnContext, SIGNAL(triggered()), this, SLOT(HelpOnContext()));
    actHelpUsersGuide= new QAction(tr("User's &Guide" ), this);
    connect(actHelpUsersGuide, SIGNAL(triggered()), this, SLOT(HelpUsersGuide()));
    actHelpTutorial= new QAction(tr("&Tutorial"), this);
    connect(actHelpTutorial, SIGNAL(triggered()), this, SLOT(HelpTutorial()));
    actHelpFAQ= new QAction(tr("FA&Q"), this);
    connect(actHelpFAQ, SIGNAL(triggered()), this, SLOT(HelpFAQ()));
    actHelpChanges= new QAction(tr("&Changes" ), this);
    connect(actHelpChanges, SIGNAL(triggered()), this, SLOT(HelpChanges()));
    actHelpQtGrace= new QAction(tr("QtGrace-&Extensions" ), this);
    connect(actHelpQtGrace, SIGNAL(triggered()), this, SLOT(HelpQtGrace()));
    actHelpComments= new QAction(tr("Co&mments" ), this);
    connect(actHelpComments, SIGNAL(triggered()), this, SLOT(HelpComments()));
    actHelpLicense= new QAction(tr("License terms" ), this);
    connect(actHelpLicense, SIGNAL(triggered()), this, SLOT(HelpLicense()));
    actHelpReadme= new QAction(tr("&Readme" ), this);
    connect(actHelpReadme, SIGNAL(triggered()), this, SLOT(HelpReadme()));
    actHelpAbout= new QAction(tr("&About..." ), this);
    connect(actHelpAbout, SIGNAL(triggered()), this, SLOT(HelpAbout()));

    //the Examples
    nr_of_Example_Menues=9;
    ExampleMenuNames[0]=tr("General &intro");
    ExampleMenuNames[1]=tr("XY &graphs");
    ExampleMenuNames[2]=tr("XY &charts");
    ExampleMenuNames[3]=tr("&Polar graphs");
    ExampleMenuNames[4]=tr("P&ie charts");
    ExampleMenuNames[5]=tr("&Special set presentations");
    ExampleMenuNames[6]=tr("&Type setting");
    ExampleMenuNames[7]=tr("Calc&ulus");
    ExampleMenuNames[8]=tr("&QtGrace");
    nr_of_Example_Menu_Entries[0]=10;
    ExampleMenuEntries[0][0]=tr("Explain");
    ExampleMenuEntries[0][1]=tr("Properties");
    ExampleMenuEntries[0][2]=tr("Axes");
    ExampleMenuEntries[0][3]=tr("Fonts");
    ExampleMenuEntries[0][4]=tr("Arrows");
    ExampleMenuEntries[0][5]=tr("Symbols and lines");
    ExampleMenuEntries[0][6]=tr("Fills");
    ExampleMenuEntries[0][7]=tr("World stack");
    ExampleMenuEntries[0][8]=tr("Insert graphs");
    ExampleMenuEntries[0][9]=tr("Many graphs");
    nr_of_Example_Menu_Entries[1]=11;
    ExampleMenuEntries[1][0]=tr("Log scale");
    ExampleMenuEntries[1][1]=tr("Log2 scale");
    ExampleMenuEntries[1][2]=tr("Log2/Log scale");
    ExampleMenuEntries[1][3]=tr("Logit scale");
    ExampleMenuEntries[1][4]=tr("Reciprocal scale");
    ExampleMenuEntries[1][5]=tr("Error bars");
    ExampleMenuEntries[1][6]=tr("Data/time axis formats");
    ExampleMenuEntries[1][7]=tr("Australia map");
    ExampleMenuEntries[1][8]=tr("A CO2 analysis");
    ExampleMenuEntries[1][9]=tr("Motif statistics");
    ExampleMenuEntries[1][10]=tr("Spectrum");
    nr_of_Example_Menu_Entries[2]=4;
    ExampleMenuEntries[2][0]=tr("Bar chart");
    ExampleMenuEntries[2][1]=tr("Stacked bar");
    ExampleMenuEntries[2][2]=tr("Bar chart with error");
    ExampleMenuEntries[2][3]=tr("Different charts");
    nr_of_Example_Menu_Entries[3]=2;
    ExampleMenuEntries[3][0]=tr("Polar graph");
    ExampleMenuEntries[3][1]=tr("Polar graph v2");
    nr_of_Example_Menu_Entries[4]=1;
    ExampleMenuEntries[4][0]=tr("Pie chart");
    nr_of_Example_Menu_Entries[5]=7;
    ExampleMenuEntries[5][0]=tr("HILO");
    ExampleMenuEntries[5][1]=tr("XY Radius");
    ExampleMenuEntries[5][2]=tr("XYZ");
    ExampleMenuEntries[5][3]=tr("Box plot");
    ExampleMenuEntries[5][4]=tr("Vector map");
    ExampleMenuEntries[5][5]=tr("XY size");
    ExampleMenuEntries[5][6]=tr("XY color");
    nr_of_Example_Menu_Entries[6]=3;
    ExampleMenuEntries[6][0]=tr("Simple");
    ExampleMenuEntries[6][1]=tr("Tex transforms");
    ExampleMenuEntries[6][2]=tr("Advanced");
    nr_of_Example_Menu_Entries[7]=1;
    ExampleMenuEntries[7][0]=tr("Non-linear fit");
    nr_of_Example_Menu_Entries[8]=3;
    ExampleMenuEntries[8][0]=tr("Filter");
    ExampleMenuEntries[8][1]=tr("Advanced Scripting");
    ExampleMenuEntries[8][2]=tr("Circle- / Ellipse-fit");

    int numbers=0;//counts the different Examples

    for (int i=0;i<nr_of_Example_Menues;i++)
    {
        example_menues[i]=new QMenu(ExampleMenuNames[i],this);
        example_menues[i]->setTearOffEnabled(TRUE);
        for (int j=0;j<nr_of_Example_Menu_Entries[i];j++)
        {
            act_examples[i][j]=new QAction(ExampleMenuEntries[i][j], this);
#if QT_VERSION >= 0x050000
            connect(act_examples[i][j], &QAction::triggered,this,[this,numbers](){this->HelpOpenExample(numbers);});
#else
            connect(act_examples[i][j], SIGNAL(triggered()), helpMapper, SLOT(map()));
            helpMapper->setMapping(act_examples[i][j],numbers);
#endif
            numbers++;
            example_menues[i]->addAction(act_examples[i][j]);
        }
    }
    nr_of_Examples=numbers;

    examplesFiles[0]=QString("explain.agr");
    examplesFiles[1]=QString("props.agr");
    examplesFiles[2]=QString("axes.agr");
    examplesFiles[3]=QString("tfonts.agr");
    examplesFiles[4]=QString("arrows.agr");
    examplesFiles[5]=QString("symslines.agr");
    examplesFiles[6]=QString("fills.agr");
    examplesFiles[7]=QString("tstack.agr");
    examplesFiles[8]=QString("tinset.agr");
    examplesFiles[9]=QString("manygraphs.agr");
    examplesFiles[10]=QString("tlog.agr");
    examplesFiles[11]=QString("log2.agr");
    examplesFiles[12]=QString("log2log.agr");
    examplesFiles[13]=QString("logit.agr");
    examplesFiles[14]=QString("reciprocal.agr");
    examplesFiles[15]=QString("terr.agr");
    examplesFiles[16]=QString("times.agr");
    examplesFiles[17]=QString("au.agr");
    examplesFiles[18]=QString("co2.agr");
    examplesFiles[19]=QString("motif.agr");
    examplesFiles[20]=QString("spectrum.agr");
    examplesFiles[21]=QString("bar.agr");
    examplesFiles[22]=QString("stackedb.agr");
    examplesFiles[23]=QString("chartebar.agr");
    examplesFiles[24]=QString("charts.agr");
    examplesFiles[25]=QString("polar.agr");
    examplesFiles[26]=QString("PolarPlotV2_Example.agr");
    examplesFiles[27]=QString("pie.agr");
    examplesFiles[28]=QString("hilo.agr");
    examplesFiles[29]=QString("txyr.agr");
    examplesFiles[30]=QString("xyz.agr");
    examplesFiles[31]=QString("boxplot.agr");
    examplesFiles[32]=QString("vmap.agr");
    examplesFiles[33]=QString("xysize.agr");
    examplesFiles[34]=QString("xycolor.agr");
    examplesFiles[35]=QString("test2.agr");
    examplesFiles[36]=QString("txttrans.agr");
    examplesFiles[37]=QString("typeset.agr");
    examplesFiles[38]=QString("logistic.agr");
    examplesFiles[39]=QString("filterexample.agr");
    examplesFiles[40]=QString("script_example.agr");
    examplesFiles[41]=QString("Circle_Fit.agr");
    //History-actions
    char dummy[64];
#if QT_VERSION >= 0x050000

#else
    historyMapper=new QSignalMapper();
#endif
    actClearHistory= new QAction(tr("&Clear History" ), this);
    connect(actClearHistory, SIGNAL(triggered()), this, SLOT(clearHistory()));
    for (int i=0;i<MAX_HISTORY;i++)
    {
        sprintf(dummy,"item %d",i);
        actHistory[i]=new QAction(QString(dummy), this);
#if QT_VERSION >= 0x050000
        connect(actHistory[i], &QAction::triggered,this,[this,i](){this->HistoryClicked(i);});
#else
        connect(actHistory[i], SIGNAL(triggered()), historyMapper, SLOT(map()));
        historyMapper->setMapping(actHistory[i],i);
#endif
    }
#if QT_VERSION >= 0x050000

#else
    connect(historyMapper, SIGNAL(mapped(int)),this, SLOT(HistoryClicked(int)));
#endif
}

void MainWindow::showEvent( QShowEvent * e )
{
e->accept();
/*
cout << "ShowEvent: w=" << this->width() << " h=" << this->height() << endl;
cout << "mainArea:  w=" << mainArea->scroll->width() << " h=" << mainArea->scroll->height() << endl;
*/
if (autofit_pending==1 && startupphase==0)
{
autofit_pending=0;
doFitPage();
//cout << "autofit_pending in showEvent" << endl;
}
//else
//cout << "No autofit_pending in showEvent" << endl;
//qDebug() << "hdevice=" << hdevice << " fext=" << device_table[hdevice].fext;
setExportTypeDescription(device_table[hdevice].fext);

if (show_setup_wizard==TRUE)
{
QTimer::singleShot(200,this,SLOT(showWizard()));
show_setup_wizard=FALSE;
}

scroll->repaint();
//resizeEvent(new QResizeEvent(this->size(),this->size()));
}

void MainWindow::showWizard(void)
{
    /*
    if (Form_SetupWizard==NULL)
    {
    Form_SetupWizard=new SetupWizard(this);
    }
    Form_SetupWizard->init();
    Form_SetupWizard->show();
    Form_SetupWizard->raise();
    Form_SetupWizard->activateWindow();
    */
    if (Form_InitWizard==NULL)
    {
    Form_InitWizard=new InitQtGraceWizard(this);
    }
    Form_InitWizard->initAllPages();
    Form_InitWizard->exec();
//qDebug() << retval;
    delete Form_InitWizard;
    Form_InitWizard=NULL;
    show_setup_wizard=FALSE;
    setup_wizard_manual=FALSE;
}

void MainWindow::resizeEvent( QResizeEvent * e)
{
    windowWidth=e->size().width();
    windowHeight=e->size().height();

int min_height=cmdExit->y()+cmdExit->height()+toolBar1->height();
int c_height=mainArea->height()-2;
int d_width=scroll->verticalScrollBar()->width()+1;

int LargeButtonW=defaultLargeButtonW*toolBarSizeFactor;
int BorderGap=defaultBorderGap*toolBarSizeFactor;
int ToolBarWidth=2*BorderGap+LargeButtonW;
#ifdef WINDOWS_SYSTEM
ToolBarWidth+=2*WIN_SIZE_CORR*toolBarSizeFactor+1;//+1 added
#endif
if (small_screen_adjustments & 1)
{
    if (c_height<min_height)
    {
    c_height=min_height;
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }
    else
    {
    d_width=0;
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
}
else
{
d_width=0;
scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

#ifdef WINDOWS_SYSTEM
    tool_empty->setGeometry(0,0,ToolBarWidth,c_height);
    scroll->setMinimumWidth(ToolBarWidth+2+d_width);
    scroll->setMaximumWidth(ToolBarWidth+2+d_width);
    scroll->setMinimumWidth(ToolBarWidth+2+d_width);

    toolBar1->setMinimumWidth(ToolBarWidth);
    toolBar1->setMaximumWidth(ToolBarWidth);
    toolBar1->setMinimumWidth(ToolBarWidth);

    toolBar2->setMinimumWidth(ToolBarWidth);
    toolBar2->setMaximumWidth(ToolBarWidth);
    toolBar2->setMinimumWidth(ToolBarWidth);

#else
    tool_empty->setGeometry(0,0,ToolBarWidth,c_height);

    scroll->setMinimumWidth(ToolBarWidth+d_width);
    scroll->setMaximumWidth(ToolBarWidth+d_width);
    scroll->setMinimumWidth(ToolBarWidth+d_width);

    toolBar1->setMinimumWidth(ToolBarWidth);
    toolBar1->setMaximumWidth(ToolBarWidth);
    toolBar1->setMinimumWidth(ToolBarWidth);

    toolBar2->setMinimumWidth(ToolBarWidth);
    toolBar2->setMaximumWidth(ToolBarWidth);
    toolBar2->setMinimumWidth(ToolBarWidth);
#endif

    /*qDebug() << "RESIZE-EVENT: " << e->size().width() << " x " << e->size().height();
    qDebug() << "ToolBarWidth=" << ToolBarWidth;
    qDebug() << "ToolBar1.width=" << toolBar1->width();
    qDebug() << "ToolBar2.width=" << toolBar2->width();
    qDebug() << "scroll.width=" << scroll->width();
    qDebug() << "tool_empty=" << tool_empty->width();*/

/*
#ifdef WINDOWS_SYSTEM
    tool_empty->setGeometry(0,0,stdColWidth+2*WIN_SIZE_CORR,c_height);
    scroll->setMinimumWidth(stdColWidth+2+d_width+2*WIN_SIZE_CORR);
    scroll->setMaximumWidth(stdColWidth+2+d_width+2*WIN_SIZE_CORR);
#else
    tool_empty->setGeometry(0,0,stdColWidth,c_height);
    scroll->setMinimumWidth(stdColWidth+2+d_width);
    scroll->setMaximumWidth(stdColWidth+2+d_width);
#endif
*/

    if (get_pagelayout() != PAGE_FIXED)
    mainArea->completeRedraw();

scroll->repaint();

/*
cout << "ResizeEvent: w=" << windowWidth << " h=" << windowHeight << endl;
cout << "mainArea:    w=" << mainArea->scroll->width() << " h=" << mainArea->scroll->height() << endl;
cout << "Draw_width=" << cmdDraw->width() << " auto_t_width=" << cmdAutoT->width() << " fit_width=" << cmdFitPage->width() << endl;
cout << "draw_x=" << cmdDraw->x() << " auto_t.x=" << cmdAutoT->x() << endl;
*/
}

void MainWindow::LoadProject(char * filename)
{
stop_repaint=TRUE;
    inwin=1;
int ret=load_project(filename);
    if (ret==RETURN_FAILURE) return;
    ///Are the following two statements realy necessary?
    /*inwin=1;
    monomode=0;*/
    //addToHistory(filename);
    initNodes();//Undo-Stuff

int use_dev=hdevice;
    if (FormDeviceSetup!=NULL) use_dev=FormDeviceSetup->cur_dev;
    QString fwe=get_filename_with_extension(use_dev);
    strcpy(print_file,fwe.toLocal8Bit().constData());

/*qDebug() << "Filename with extension=" << fwe;
qDebug() << "print_file=" << print_file;*/

    if (FormDeviceSetup!=NULL)
    {
    FormDeviceSetup->printfile_item->lenText->setText(fwe);
    FormDeviceSetup->init(FormDeviceSetup->devices_item->currentValue());
    }

    /*Device_entry dev = get_device_props(hdevice);
if (FormDeviceSetup!=NULL) dev = get_device_props(FormDeviceSetup->cur_dev);
sprintf(print_file,"%s.%s",get_docbname(),dev.fext);
if (FormDeviceSetup!=NULL)
FormDeviceSetup->printfile_item->setText(QString(print_file));*/

//qDebug() << "PrintFile=" << print_file;
/*if (FormDeviceSetup!=NULL)
{
strcpy(print_file, get_docname());
char * bufptr = strrchr(print_file, '.');
    if (bufptr)
    {
    *(bufptr + 1) = '\0';
    }
    else
    {
    strcat(print_file, ".");
    }
Device_entry dev = get_device_props(FormDeviceSetup->cur_dev);
strcat(print_file, dev.fext);
FormDeviceSetup->printfile_item->setText(QString(print_file));
}
FormTestSpreadSheet->init(0,0);

mainArea->completeRedraw();
mainArea->setGeometry(mainArea->x(),mainArea->y(),mainArea->width(),mainArea->height());
*/
stop_repaint=FALSE;
set_wait_cursor();
        if (autofit_on_load==TRUE && ret==0)
        doFitPage();
        else
        force_redraw();/// mainArea->completeRedraw();
    if (ret==0)
    clear_dirtystate();
unset_wait_cursor();
    //qDebug() << "cwd=" << QString::fromLocal8Bit(get_workingdir());
}

void MainWindow::closeEvent( QCloseEvent * event )
{
#ifdef DEBUG_OUT_LOG
debug_out << "closeEvent in MainWindow." << endl;
#endif

//qDebug() << "TEST: closeEvent";

    int ret=bailout();
    if (ret==0)//do not close!
    {
        event->ignore();
    }
    else//close QtGrace for real
    {
        event->accept();
        finalBailout();
    }
}

void MainWindow::finalBailout(void)
{
//qDebug() << "finalBailout A";
    removeAllWindows();
//qDebug() << "finalBailout B";
    write_settings();
//qDebug() << "finalBailout C";
    initNodes();//clear undo-stuff
//qDebug() << "finalBailout D";
    #ifdef DEBUG_OUT_LOG
    debug_out.close();
    #endif
    qApp->exit(0);
}

void MainWindow::getselectedgraphs(int * nr_of_graphs, int ** graph_nrs)
{
    lstGraphs->get_selection(nr_of_graphs,graph_nrs);
    if (*nr_of_graphs==0 && is_valid_gno(get_cg())==TRUE)
    {
    int n_sel[2]={get_cg(),get_cg()};
    lstGraphs->set_new_selection(1,n_sel);
    lstGraphs->get_selection(nr_of_graphs,graph_nrs);
    }
    if (*nr_of_graphs==0) return;
    bool all_graphs=false;
    for (int i=0;i<(*nr_of_graphs);i++)
    {
        if ((*graph_nrs)[i]<0)
        {
            all_graphs=true;
            break;
        }
    }
    if (all_graphs==true)
    {
        (*nr_of_graphs)=number_of_graphs();
        delete[] (*graph_nrs);
        (*graph_nrs)=new int[2+(*nr_of_graphs)];
        for (int i=0;i<(*nr_of_graphs);i++)
        {
            (*graph_nrs)[i]=i;
        }
    }
    //else: the graph-id's are already complete
}

/*
 * service the autoscale buttons on the main panel
 */
void MainWindow::autoscale_proc(int data)
{
    QString errormsg=tr("Can't autoscale (no active sets?)");
    int nr_of_graphs,*graph_nrs=new int[2];
    world wtmp;
    int cg,ret=RETURN_SUCCESS;// = get_cg();
    getselectedgraphs(&nr_of_graphs,&graph_nrs);

    double min_x[2+nr_of_graphs],min_y[2+nr_of_graphs],max_x[2+nr_of_graphs],max_y[2+nr_of_graphs];
    bool graph_data_valid[2+nr_of_graphs];
    double t_x,t_y;
    VPoint t_vp;
    world w;

    if (nr_of_graphs<=0)
    {
        errormsg=tr("No Graph selected!");
        errmsg(errormsg.toLocal8Bit().constData());
        delete[] graph_nrs;
        return;
    }
    ListOfChanges.clear();
    ListOfOldStates.clear();
    //Undo-Stuff
    SaveTickmarksStatesPrevious(X_AXIS,Y_AXIS,nr_of_graphs,graph_nrs);

    QList<int> active_graphs,active_sets;
    active_graphs.clear();
    active_sets.clear();
    if (useErrorbarsInAutoscale)
    {
    autoscale_active=1;
        for (int l=0;l<nr_of_graphs;l++)
        {
            cg=graph_nrs[l];
            for (int k=0;k<number_of_sets(graph_nrs[l]);k++)
            {
                if (is_set_active(graph_nrs[l],k)==TRUE && g[graph_nrs[l]].p[k].ignore_in_autoscale==FALSE)
                {
                active_graphs << graph_nrs[l];
                active_sets << k;
                }
            }
        }
    int nr_of_sets1=active_sets.length();
    int * sno1=new int[2+nr_of_sets1],*gno1=new int[2+nr_of_sets1];
    for (int i=0;i<nr_of_sets1;i++)
    {
        gno1[i]=active_graphs.at(i);
        sno1[i]=active_sets.at(i);
    }

    int sav_highl_err_bars=highlight_errorbars;
    highlight_errorbars=TRUE;
    prepare_highlight_segments_with_sets(nr_of_sets1,gno1,sno1,FALSE);
    highlight_errorbars=sav_highl_err_bars;

    if (nr_of_highlight_segments<=0)
    {
    //qDebug() << "ERROR! NOTHING TO AUTOSCALE TO!?";
    errmsg(errormsg.toLocal8Bit().constData());
    QString set_status;
        for (int k=0;k<nr_of_graphs;k++)
        {
        set_status=tr("G")+QString::number(graph_nrs[k])+tr(" number of sets=")+QString::number(number_of_active_sets(graph_nrs[k]))+tr(" number of sets for autoscale=")+QString::number(number_of_active_autoscale_sets(graph_nrs[k]));
        errmsg(set_status.toLocal8Bit().constData());
        }
    }
    int nr_of_segments_found=0;
    for (int k=0;k<nr_of_graphs;k++)
    {
    min_x[k]=min_y[k]=max_x[k]=max_y[k]=0.0;
    graph_data_valid[k]=false;
    for (int i=0;i<nr_of_highlight_segments;i++)
    {
        if (highlight_clipping.at(i)!=k || is_set_hidden(graph_nrs[k],highlight_set.at(i)) || g[graph_nrs[k]].p[highlight_set.at(i)].ignore_in_autoscale==TRUE) continue;
        nr_of_segments_found++;
    //qDebug() << "segment[" << i << "].length=" << highlight_segments[i].length() << "type=" << highlight_type.at(i) << "clipping=" << highlight_clipping.at(i) << "set=" << highlight_set.at(i);
        for (int j=0;j<highlight_segments[i].length();j++)
        {
        t_vp=xlibdev2VPoint2(highlight_segments[i].at(j).x(),highlight_segments[i].at(j).y());
        //qDebug() << highlight_segments[i].at(j).x() << " " << highlight_segments[i].at(j).y();
        view2_graph_world(graph_nrs[k],(double)t_vp.x,(double)t_vp.y,&t_x,&t_y);
        //qDebug() << t_x << "|" << t_y;
        /*if (i==nr_of_highlight_segments-1)
        qDebug() << highlight_segments[i].at(j).x() << " " << highlight_segments[i].at(j).y() << "-->" << t_x << " " << t_y;*/
            if (graph_data_valid[k]==false)
            {
            min_x[k]=max_x[k]=t_x;
            min_y[k]=max_y[k]=t_y;
            graph_data_valid[k]=true;
            }
            else
            {
            if (t_x<min_x[k]) min_x[k]=t_x;
            if (t_y<min_y[k]) min_y[k]=t_y;
            if (t_x>max_x[k]) max_x[k]=t_x;
            if (t_y>max_y[k]) max_y[k]=t_y;
            }
        }
    }//end segments
    /*qDebug() << "Segments_found=" << nr_of_segments_found;
    qDebug() << "graph" << k << "x=[" << min_x[k] << "," << max_x[k] << "] y=[" << min_y[k] << "," << max_y[k] << "]";*/
    round_axis_limits(min_x+k, max_x+k, get_graph_xscale(k));
    round_axis_limits(min_y+k, max_y+k, get_graph_yscale(k));
    //qDebug() << "rounded: x=[" << min_x[k] << "," << max_x[k] << "] y=[" << min_y[k] << "," << max_y[k] << "]";
    }//end graphs
    autoscale_active=0;
    }

    //SaveTickmarksStatesPrevious(X_AXIS,Y_AXIS,cg,cg);
    for (int l=0;l<nr_of_graphs;l++)
    {
        cg=graph_nrs[l];
        get_graph_world(cg,&wtmp);
            sprintf(dummy,"with G%d",cg);
            ListOfChanges << QString(dummy);
            ListOfOldStates << QString(dummy);
            sprintf(dummy,"    world %g, %g, %g, %g",wtmp.xg1,wtmp.yg1,wtmp.xg2,wtmp.yg2);
            ListOfOldStates << QString(dummy);
        if (useErrorbarsInAutoscale)
        {
            if (graph_data_valid[l]==true && is_valid_gno(l))
            {
            w.xg1=min_x[l];
            w.xg2=max_x[l];
            w.yg1=min_y[l];
            w.yg2=max_y[l];
            set_graph_world(l, w);
                switch ((int) data)
                {
                case AUTOSCALE_X:
                    autotick_axis(l, ALL_X_AXES);
                    break;
                case AUTOSCALE_Y:
                    autotick_axis(l, ALL_Y_AXES);
                    break;
                case AUTOSCALE_XY:
                    autotick_axis(l, ALL_AXES);
                    break;
                default:
                    ;/*do nothing*/
                    break;
                }
            }
        }
        else
        {
        ret=autoscale_graph(cg, (int) data);
        }

        if (ret == RETURN_SUCCESS)
        {

            if (FormAxisProperties!=NULL)
                FormAxisProperties->update_ticks(cg);

            if (data==AUTOSCALE_XY)
            {
                ListOfChanges << QString("autoscale");
                sprintf(dummy,"Autoscale graph [G%d]",cg);
            }
            else if (data==AUTOSCALE_X)
            {
                ListOfChanges << QString("autoscale xaxes");
                sprintf(dummy,"Autoscale X-axis [G%d]",cg);
            }
            else
            {
                ListOfChanges << QString("autoscale yaxes");
                sprintf(dummy,"Autoscale Y-axis [G%d]",cg);
            }
        }
        else
        {
            sprintf(dummy," G%d",cg);
            errmsg((errormsg+QString(dummy)).toLocal8Bit().constData());
        }
    }
    if (ret==RETURN_SUCCESS)
    {
    //Undo-Stuff
    TickmarksModified(X_AXIS,Y_AXIS,nr_of_graphs,graph_nrs);
    //TickmarksModified(X_AXIS,Y_AXIS,cg,cg);
    addAditionalDescriptionToLastNode(-1,QString(dummy),QString(),0);
    }
    mainArea->completeRedraw();
    ListOfChanges.clear();
    delete[] graph_nrs;
}

/*
 * service the autoticks button on the main panel
 */
void MainWindow::autoticks_proc(void)
{
    int nr_of_graphs,*graph_nrs=new int[2],cg;
    getselectedgraphs(&nr_of_graphs,&graph_nrs);
    ListOfChanges.clear();
    SaveTickmarksStatesPrevious(X_AXIS,Y_AXIS,nr_of_graphs,graph_nrs);
    for (int ll=0;ll<nr_of_graphs;ll++)
    {
        cg=graph_nrs[ll];
        autotick_axis(cg, ALL_AXES);
        sprintf(dummy,"with G%d",cg);
        ListOfChanges << QString(dummy);
        ListOfChanges << QString("autoticks");
        sprintf(dummy,"Autotick graph [G%d]",cg);
        addAditionalDescriptionToLastNode(-1,QString(dummy),QString(),0);
    }
    TickmarksModified(X_AXIS,Y_AXIS,nr_of_graphs,graph_nrs);
    if (FormAxisProperties!=NULL)
        FormAxisProperties->update_ticks(get_cg());
    mainArea->completeRedraw();
    ListOfChanges.clear();
    if (graph_nrs) delete[] graph_nrs;
}

void MainWindow::graph_scroll_proc(int data)
{
    char dummy[256];
    int nr_of_graphs,*graph_nrs=new int[2];
    getselectedgraphs(&nr_of_graphs,&graph_nrs);
    if (nr_of_graphs<=0)
    {
        errmsg(tr("No graph(s) selected for scrolling!").toLocal8Bit().constData());
        return;
    }
    //SaveTickmarksStatesPrevious(X_AXIS,Y_AXIS,get_cg(),get_cg());
    SaveTickmarksStatesPrevious(X_AXIS,Y_AXIS,nr_of_graphs,graph_nrs);
    graph_scroll(data,nr_of_graphs,graph_nrs);
    TickmarksModified(X_AXIS,Y_AXIS,nr_of_graphs,graph_nrs);
    if (nr_of_graphs>1)
    {
        if (data==GSCROLL_LEFT)
            sprintf(dummy,"Scroll left graphs");
        else if (data==GSCROLL_RIGHT)
            sprintf(dummy,"Scroll right graphs");
        else if (data==GSCROLL_UP)
            sprintf(dummy,"Scroll up graphs");
        else
            sprintf(dummy,"Scroll down graphs");
        addAditionalDescriptionToLastNode(-1,QString(dummy),QString(),1);
    }
    else
    {
        if (data==GSCROLL_LEFT)
            sprintf(dummy,"Scroll left graph [G%d]",graph_nrs[0]);
        else if (data==GSCROLL_RIGHT)
            sprintf(dummy,"Scroll right graph [G%d]",graph_nrs[0]);
        else if (data==GSCROLL_UP)
            sprintf(dummy,"Scroll up graph [G%d]",graph_nrs[0]);
        else
            sprintf(dummy,"Scroll down graph [G%d]",graph_nrs[0]);
        addAditionalDescriptionToLastNode(-1,QString(dummy),QString(),0);
    }
    mainArea->completeRedraw();
    delete[] graph_nrs;
}

void MainWindow::graph_zoom_proc(int data)
{
    char dummy[256];
    int nr_of_graphs,*graph_nrs=new int[2];
    int sav_undo_act;
    getselectedgraphs(&nr_of_graphs,&graph_nrs);
    if (nr_of_graphs<=0)
    {
        errmsg(tr("No graph(s) selected for zoom!").toLocal8Bit().constData());
        return;
    }
    /*qDebug() << "nr_of_graphs=" << nr_of_graphs;
    for (int i=0;i<nr_of_graphs;i++)
    {
    qDebug() << "graph=" << graph_nrs[i];
    }*/
    //SaveTickmarksStatesPrevious(X_AXIS,Y_AXIS,get_cg(),get_cg());
    SaveTickmarksStatesPrevious(X_AXIS,Y_AXIS,nr_of_graphs,graph_nrs);
    //graph_zoom(data);
    sav_undo_act=undo_active;
    undo_active=FALSE;
    graph_zoom(data,nr_of_graphs,graph_nrs);
    undo_active=sav_undo_act;
    TickmarksModified(X_AXIS,Y_AXIS,nr_of_graphs,graph_nrs);
    if (nr_of_graphs>1)
    {
        if (data==GZOOM_EXPAND)
            sprintf(dummy,"Zoom out graphs");
        else
            sprintf(dummy,"Zoom in graphs");
        addAditionalDescriptionToLastNode(-1,QString(dummy),QString(),1);
    }
    else
    {
        if (data==GZOOM_EXPAND)
            sprintf(dummy,"Zoom out graph [G%d]",graph_nrs[0]);
        else
            sprintf(dummy,"Zoom in graph [G%d]",graph_nrs[0]);
        addAditionalDescriptionToLastNode(-1,QString(dummy),QString(),0);
    }
    mainArea->completeRedraw();
    delete[] graph_nrs;
}

void MainWindow::world_stack_proc(int data)
{
    switch (data) {
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
    mainArea->completeRedraw();//xdrawgraph();
}

void MainWindow::load_example(char *data)
{
    char buf[128];
    set_wait_cursor();
    sprintf(buf, "examples/%s", data);
    //load_project_file(buf, FALSE);
    //update_default_props();
    load_project(buf);
        if (autofit_on_load==TRUE)
        doFitPage();
        else
        mainArea->completeRedraw();//xdrawgraph();
    unset_wait_cursor();
}

void MainWindow::set_stack_message(void)
{
    char buf[16];
    //cout << "cg=" << get_cg() << " wss=" << graph_world_stack_size(get_cg()) << " wsc=" << get_world_stack_current(get_cg()) << endl;
    sprintf(buf, " SD:%1d ", graph_world_stack_size(get_cg()));
    lblSD->setText(QString(buf));
    sprintf(buf, " CW:%1d ", get_world_stack_current(get_cg()));
    lblCW->setText(QString(buf));
    UpdateViewportList();
}

void MainWindow::newgraphselection(int gr_nr)
{
    if (gr_nr<0 || gr_nr>=lstGraphs->number_of_entries) return;//invalid number
    disconnect(lstGraphs,SIGNAL(new_selection(int)),this,SLOT(newgraphselection(int)));
    int graph_number=lstGraphs->entries[gr_nr];
    int * selection=new int[2];
    int nr_selection=0;
    lstGraphs->get_selection(&nr_selection,&selection);
    //cout << "newgraphselection=" << gr_nr << endl;
    //simple_draw_setting|=SIMPLE_DRAW_GRAPH_FOCUS;//if you activate this you will never get a real redraw!
        if (graph_number<=0)
        switch_current_graph(0);
        else
        switch_current_graph(graph_number);
    //cout << "gr_nr=" << gr_nr << " graph_number=" << graph_number << " NR_OF_SEL_ITEMS=" << nr_selection << endl;
    //qApp->processEvents();
    if (graph_number==-1)
    {
        int sel[2]={graph_number,1};
        lstGraphs->set_new_selection(1,sel);
        //cout << "set -1" << endl;
    }
    else if (nr_selection>1)
    {
        int sel[2]={-1,1};
        if (nr_selection==number_of_graphs())
        lstGraphs->set_new_selection(1,sel);
        else
        lstGraphs->set_new_selection(nr_selection,selection);
    }
    connect(lstGraphs,SIGNAL(new_selection(int)),this,SLOT(newgraphselection(int)));
        if (selection!=NULL)
        delete[] selection;
}

void MainWindow::mouseReleaseEvent ( QMouseEvent * event )
{
    event->accept();
    /*cout << "mouse Release 0" << endl;
    mainArea->completeRedraw();//redraw on MouseRelease removed --> should not be neccessary
    cout << "mouse Release 1" << endl;*/
}

void MainWindow::keyPressEvent( QKeyEvent * e )
{
    QWidget::keyPressEvent(e);
}

void MainWindow::SpreadSheetClosed(int gno,int setno)
{
    (void)gno;
    (void)setno;
    //qDebug() << "Spreadsheet closed: G" << gno << ".S" << setno;
}

void MainWindow::checkForRealTimeIO(void)
{
    if (FormRTIManage!=NULL)
    {
        rtiCheckTime=FormRTIManage->selTimeout->value();
    }
    if (real_time_under_monitoring())
    {
        //cout << "Real time input monitored! Starting timer!" << endl;
        if (rtiTimer->isActive()==false)
        {
            rtiTimer->start(rtiCheckTime);
        }
    }
    else
    {
        //cout << "NO real time input!" << endl;
        if (rtiTimer->isActive()==true)
        {
            rtiTimer->stop();
        }
    }
    if (FormRTIManage!=NULL)
    {
        FormRTIManage->chkRTIactive->setChecked(rtiTimer->isActive());
        FormRTIManage->updateRTIs();
    }
}

void MainWindow::doRealTimeMonitoring(void)
{
    monitor_input(ib_tbl, ib_tblsize, 0);
    if (real_time_under_monitoring()<=0 && rtiTimer->isActive()==true)
    {
        rtiTimer->stop();
        if (FormRTIManage!=NULL)
        {
            FormRTIManage->chkRTIactive->setChecked(rtiTimer->isActive());
        }
    }
}

void MainWindow::set_barebones(int onoff)
{
    if (onoff){
        actShowStatusBar->setChecked(false);
        actShowToolBar->setChecked(false);
        actShowLocBar->setChecked(false);
        //locbar_visible = 0;
        //toolbar_visible = 0;
        //statusbar_visible = 0;
    }
}

void MainWindow::addToHistory(char * entry)
{
//char sep=QDir::separator().toLatin1();
    int found=-1;//,len;
    QString to_find=QDir::toNativeSeparators(QDir::cleanPath(QString::fromLocal8Bit(entry)));
    //char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    /*strcpy(buf,entry);
    len=strlen(buf);
    for (int i=0;i<len;i++)
    {
        if (buf[i]=='\\' || buf[i]=='/')
            found=i;
    }
    if (found>=0) buf[found]='\0';*/
    sprintf(buf2, "%s/examples",qt_grace_share_dir);
    QString to_find2(buf2);
    /*QString to_find3;
    QFileInfo d_tmp(to_find);
    to_find3=d_tmp.absolutePath();
    to_find3=QDir::cleanPath(to_find3);
    to_find3=QDir::toNativeSeparators(to_find3);*/
    to_find2=QDir::toNativeSeparators(QDir::cleanPath(to_find2));
    /*qDebug() << "Add to history:" << to_find;
    qDebug() << "Example_start: " << to_find2;
    qDebug() << "Entry start:   " << to_find3;*/
    //if (strcmp(buf,buf2)==0)
    if (to_find.indexOf(to_find2)==0)
    {
        //qDebug() << "example-File ignored!";
        return;//File is an example-file --> do not store this in the history
    }
    found=-1;
    for (int i=0;i<current_history;i++)
    {
        if (history[i]==to_find)
        {
            found=i;
            break;
        }
    }
    if (found>=0)
    {
        for (int i=found;i>0;i--)
            history[i]=history[i-1];
        history[0]=to_find;//QString::fromLocal8Bit(entry);
    }
    else
    {
        for (int i=MAX_HISTORY-1;i>0;i--)
            history[i]=history[i-1];

        history[0]=to_find;//QString::fromLocal8Bit(entry);
        current_history++;
        if (current_history>max_history) current_history=max_history;
    }
    recreateHistory();
}

void MainWindow::HistoryClicked(int i)
{
    if (current_history==0)
        return;//clicks are pointless
    int FileExists,isWriteable,isReadable;
    QFile fileTest(history[i]);//selectedFile);
    QFileInfo fileInfo(fileTest);
    if (fileInfo.isFile()==false)
    {
        FileExists=false;
        isWriteable=false;
        isReadable=false;
    }
    else
    {
        FileExists=fileTest.exists();
        isWriteable=fileInfo.isWritable();
        isReadable=fileInfo.isReadable();
    }
    IOrequested(READ_PROJECT_FORM,QString(history[i]),FileExists,isWriteable,isReadable);
}

void MainWindow::clearHistory(void)
{
    if (current_history==0)
        return;//clicks are pointless
    current_history=0;
    recreateHistory();
}

void MainWindow::recreateHistory(void)
{
    for (int i=current_history;i<MAX_HISTORY;i++)
        actHistory[i]->setVisible(false);
    for (int i=0;i<current_history;i++)
    {
        actHistory[i]->setText(history[i]);
        actHistory[i]->setVisible(true);
    }
    if (current_history==0)
        actClearHistory->setText(tr("No entries"));
    else
        actClearHistory->setText(tr("&Clear History"));
}

void MainWindow::doUndo(void)
{
    bool old_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    Undo();
    immediateUpdate=old_upd;
    updateRunning=false;
}

void MainWindow::doRedo(void)
{
    bool old_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    Redo();
    immediateUpdate=old_upd;
    updateRunning=false;
}

void MainWindow::doUndoList(void)
{
    if (FormUndoList==NULL)
    {
        FormUndoList=new frmUndoList(this);
    }
    FormUndoList->init();
    FormUndoList->show();
    FormUndoList->raise();
}

void MainWindow::start_highlighting(void)
{
simple_draw_setting|=SIMPLE_DRAW_HIGHLIGHT;
highlightTimer->start(300);
if (highlight_time>0)
stopHighlightTimer->start(1000*highlight_time);
}

void MainWindow::stop_highlighting(void)
{
bool highlight_timer_was_running=highlightTimer->isActive();
/// qDebug() << "stop Highlight, timer running=" << highlight_timer_was_running;
simple_draw_setting&=(~SIMPLE_DRAW_HIGHLIGHT);
highlightTimer->stop();
stopHighlightTimer->stop();
int sav_simple=simple_draw_setting;
//mainArea->completeRedraw();
print_target=PRINT_TARGET_SCREEN;//this is the standard (is only changed in do_hardcopy())
if (highlight_timer_was_running)
force_redraw_wo_dialog_updates();
/*mainWin->mainArea->compl_redraw_running=true;
drawgraph();
mainWin->mainArea->compl_redraw_running=false;*/
simple_draw_setting=sav_simple;
    if (highlight_timer_was_running)//we stopped something that was really running-->clear state and repaint (otherwise just reset everything, but no repaint needed)
    {
    mainWin->mainArea->repaint();
    qApp->processEvents();
    }
}

void MainWindow::highlight_ping(void)
{
    if (simple_draw_setting & SIMPLE_DRAW_HIGHLIGHT)
    {
    simple_draw_setting&=(~SIMPLE_DRAW_HIGHLIGHT);//remove highlighting
    }
    else
    {
    simple_draw_setting|=SIMPLE_DRAW_HIGHLIGHT;//add highlighting
    }
    if (simple_draw_setting==SIMPLE_DRAW_NONE) simple_draw_setting|=SIMPLE_DRAW_GRAPH_FOCUS;
int sav_simple=simple_draw_setting;
//mainArea->completeRedraw();
print_target=PRINT_TARGET_SCREEN;//this is the standard (is only changed in do_hardcopy())
mainWin->mainArea->compl_redraw_running=true;
//mainWin->mainArea->contentChanged=true;
//qDebug() << "drawGraph simple_draw=" << simple_draw_setting;
drawgraph();
mainWin->mainArea->compl_redraw_running=false;
simple_draw_setting=sav_simple;
}

void MainWindow::highlight_stop_ping(void)
{
stop_highlighting();
}

void MainWindow::UpdateViewportList(void)
{
QString text;
    if (!is_valid_gno(get_cg())) return;
//int sel_view=cmbViewStack->currentIndex();
    disconnect(cmbViewStack,0,0,0);
cmbViewStack->clear();
if (g[get_cg()].ws_top==1 && g[get_cg()].ws[0].w_name==NULL)
{
    cmbViewStack->addItem(QString("Default"));
}
else
{
    for (int i=g[get_cg()].ws_top-1;i>=0;i--)
    {
        if (g[get_cg()].ws[i].w_name==NULL)
            text=QString("NULL-")+QString::number(i);
        else if(strlen(g[get_cg()].ws[i].w_name)<1)
            text=QString("SD-")+QString::number(i);
        else
            text=QString(g[get_cg()].ws[i].w_name);
        cmbViewStack->addItem(text);
        //qDebug() << "item=" << text << " null=" << (g[get_cg()].ws[i].w_name==NULL?"+":"-");
    }
}
/*if (sel_view>=g[get_cg()].ws_top)
cmbViewStack->setCurrentIndex(g[get_cg()].ws_top);
else
cmbViewStack->setCurrentIndex(sel_view);*/
cmbViewStack->setCurrentIndex(g[get_cg()].ws_top-1-g[get_cg()].curw);
cmdViewReplace->setEnabled(true);
if (cmbViewStack->count()<=1)
{
cmdViewUp->setEnabled(false);
cmdViewDown->setEnabled(false);
cmdViewRemove->setEnabled(false);
cmdViewRearrange->setEnabled(false);
}
else
{
    if (cmbViewStack->count()>0)
    {
    cmdViewRemove->setEnabled(true);
    cmdViewDown->setEnabled(true);
    cmdViewUp->setEnabled(true);
    cmdViewRearrange->setEnabled(true);
    }
}
    connect(cmbViewStack,SIGNAL(currentIndexChanged(int)),this,SLOT(newViewportSelected(int)));
}

void MainWindow::newViewportUp(void)
{
//qDebug() << "Go one viewport UP";
int sel_view=cmbViewStack->currentIndex();
if (sel_view-1>=0) cmbViewStack->setCurrentIndex(sel_view-1);
else cmbViewStack->setCurrentIndex(cmbViewStack->count()-1);
set_dirtystate();
}

void MainWindow::newViewportDown(void)
{
//qDebug() << "Go one viewport DOWN";
int sel_view=cmbViewStack->currentIndex();
if (sel_view+1<cmbViewStack->count()) cmbViewStack->setCurrentIndex(sel_view+1);
else cmbViewStack->setCurrentIndex(0);
set_dirtystate();
}

void MainWindow::newViewportRename(void)
{
    if (!is_valid_gno(get_cg())) return;
//qDebug() << "Rename current Viewport";
bool ok;
QString old_title(g[get_cg()].ws[g[get_cg()].curw].w_name);
if (strlen(g[get_cg()].ws[g[get_cg()].curw].w_name)<=0) old_title=QString("SD ")+QString::number(g[get_cg()].curw);
QString new_name=QInputDialog::getText(this,tr("Viewport name"),tr("Enter new name for this viewport:"),QLineEdit::Normal,old_title,&ok);
    if (ok==true && !new_name.isEmpty())
    {
    //qDebug() << "New Name: " << new_name;
    g[get_cg()].ws[g[get_cg()].curw].w_name=copy_string(g[get_cg()].ws[g[get_cg()].curw].w_name,new_name.toLocal8Bit().constData());
    UpdateViewportList();
    set_dirtystate();
    }
}

void MainWindow::newViewportSelected(int c)
{
//qDebug() << "Set new viewport: " << c;
(void)c;
    if (!is_valid_gno(get_cg())) return;
int entr=g[get_cg()].ws_top-1-cmbViewStack->currentIndex();
show_world_stack(entr);
mainArea->completeRedraw();
}

void MainWindow::newViewportAdd(void)
{
//qDebug() << "Insert current as new";
doPu();
//qDebug() << "Adding:" << g[get_cg()].ws[g[get_cg()].curw].w_name;
UpdateViewportList();
set_dirtystate();
}

void MainWindow::newViewportRemove(void)
{
//qDebug() << "Remove current";
doPo();
UpdateViewportList();
set_dirtystate();
}

void MainWindow::newViewportReposition(void)
{
int gno=get_cg();
if (!is_valid_gno(gno)) return;
int top=g[gno].ws_top;
int pos=g[gno].curw;
QString text=tr("Please set new position for current viewport in the stack (currently ") + QString::number(pos) + QString("/") + QString::number(top) + QString(").");
bool ok;
int new_pos=QInputDialog::getInt(this,tr("Rearrange stack"),text,pos,0,top-1,1,&ok);
if (ok && new_pos!=pos)
{
//qDebug() << "old=" << pos << "/" << top << "--> new=" << new_pos;
    if (new_pos<pos)
    {
        for (int i=pos;i>new_pos;i--)
        swap_viewport_stack(gno,i,i-1);
    }
    else//new_pos>pos
    {
        for (int i=pos;i>new_pos;i++)
        swap_viewport_stack(gno,i,i+1);
    }
g[gno].curw=new_pos;
UpdateViewportList();
set_dirtystate();
}
}

void MainWindow::newViewportReplace(void)
{
int gno=get_cg();
if (!is_valid_gno(gno)) return;
int pos=g[gno].curw;
get_graph_world(gno,&g[gno].ws[pos].w);
UpdateViewportList();
set_dirtystate();
}

myScrollArea::myScrollArea(QWidget * parent):QScrollArea(parent)
{
scaleFactor=1.0;
tim1.start();
//tim2.start();
connect(&ping_timer,SIGNAL(timeout()),this,SLOT(timerStopEvent()));
connect(&event_timer,SIGNAL(timeout()),this,SLOT(doEvent()));
}

bool myScrollArea::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture && allow_gestures)
    {
        event->accept();
        return gestureEvent(static_cast<QGestureEvent*>(event));
    }
return QScrollArea::event(event);
}

bool myScrollArea::gestureEvent(QGestureEvent *event)
{
//qDebug() << "gestureEvent():" << event;
//qDebug() << "type=" << event->gestures();
    if (QGesture *swipe = event->gesture(Qt::SwipeGesture))
        swipeTriggered(static_cast<QSwipeGesture *>(swipe));
    else if (QGesture *pan = event->gesture(Qt::PanGesture))
        panTriggered(static_cast<QPanGesture *>(pan));
    if (QGesture *pinch = event->gesture(Qt::PinchGesture))
        pinchTriggered(static_cast<QPinchGesture *>(pinch));
    return true;
}

void myScrollArea::grabGestures(const QList<Qt::GestureType> &gestures)
{
    foreach (Qt::GestureType gesture, gestures)
        grabGesture(gesture);
}

void myScrollArea::panTriggered(QPanGesture *gesture)
{
    (void)gesture;
//qDebug() << "Pan";
}

void myScrollArea::timerStopEvent(void)
{
/*qDebug() << "Timer stop: Scale=" << scaleFactor;
qDebug() << "HotSpot=(" << hotSpot.x() << "|" << hotSpot.y() << ")";
qDebug() << "RealVPos=(" << realVPos.x << "|" << realVPos.y << ")";
qDebug() << "RealWPos=(" << realWPos.x << "|" << realWPos.y << ")";*/
ping_timer.stop();
int cur_gr=get_cg();
zoomGraphWorld(cur_gr,orig_world,realWPos,scaleFactor);
//Undo-Stuff
TickmarksModified(X_AXIS,Y_AXIS,cur_gr,cur_gr);
char dummy[128];
sprintf(dummy,"Zoom by pinch-gesture [G%d]",cur_gr);
addAditionalDescriptionToLastNode(-1,QString(dummy),QString(),0);
mainWin->mainArea->completeRedraw();
}

void myScrollArea::eventTimerEnvent(void)
{
    /*if (ignore_event_timer)
    {
    //qDebug() << "Ignoring Event Timer";
    return;
    }*/
//Event
//qDebug() << "Event-Timer--> do the event";
set_action((CanvasAction)eventType);
    /*if (eventType==PICK_MOVE)
    {
    qDebug() << "Starting PICK";
    }
    else if (eventType==PAN_1ST)
    {
    qDebug() << "Starting PAN";
    }*/
my_proc(&completeEvent);
//qDebug() << "completed delayed event-start";
}

void myScrollArea::startEvent(int type, special_XEvent event, VPoint vp)
{
    haltEvent();
    /// ignore_event_timer=false;
eventType=type;
completeEvent=event;
EventLastPoint=vp;
//qDebug() << "Starting Timer";
//event_timer.singleShot(CLICKINT+100,this,SLOT(eventTimerEnvent()));//wait a bit longer than double-click-interval
event_timer.start(CLICKINT+100);
}

void myScrollArea::haltEvent(void)//halt without doing the stuff -> i.e. there has been no double-click
{
//qDebug() << "Stopping Timer";
    //if (event_timer.isActive())
    event_timer.stop();
/// ignore_event_timer=true;
}

void myScrollArea::doEvent(void)//execute the saved event command immediately
{
    haltEvent();
//qDebug() << "Explicit DO-Timer";
/// ignore_event_timer=false;
    eventTimerEnvent();
}

void myScrollArea::pinchTriggered(QPinchGesture *gesture)
{
    if (gesture->changeFlags()!=QPinchGesture::ScaleFactorChanged) return;

if (tim1.elapsed()>100)//first time --> save scale factor
{
startScaleFactor=gesture->scaleFactor();
hotSpot=this->mapFromGlobal(gesture->hotSpot().toPoint());
int tmp_x,tmp_y,cur_gr=get_cg();
mainWin->mainArea->transf_window_coords(hotSpot.x(),hotSpot.y(),tmp_x,tmp_y);
realVPos = xlibdev2VPoint(tmp_x,tmp_y);
view2world(realVPos.x, realVPos.y, &realWPos.x, &realWPos.y);
get_graph_world(cur_gr,&orig_world);
//qDebug() << "start-Factor=" << startScaleFactor;

ListOfChanges.clear();
//Undo-Stuff
SaveTickmarksStatesPrevious(X_AXIS,Y_AXIS,cur_gr,cur_gr);

return;
}
else//one in a series of events
{
ping_timer.stop();
ping_timer.start(200);
//qDebug() << "current-Factor=" << gesture->scaleFactor();
scaleFactor=startScaleFactor/gesture->scaleFactor();
zoomGraphWorld(get_cg(),orig_world,realWPos,scaleFactor);
mainWin->mainArea->completeRedraw();
return;
}

}

void myScrollArea::swipeTriggered(QSwipeGesture *gesture)
{
    (void)gesture;
//qDebug() << "Swipe";
}

void myScrollArea::keyPressEvent( QKeyEvent * e )
{
int key=e->key();
    if ((e->modifiers()&Qt::CTRL))
    {
        if (key==Qt::Key_Up)
        {
        mainWin->doUp();
        //qDebug() << "Crtl+KeyUp";
        }
        else if (key==Qt::Key_Down)
        {
        mainWin->doDown();
        //qDebug() << "Crtl+KeyDown";
        }
        else if (key==Qt::Key_Left)
        {
        mainWin->doLeft();
        //qDebug() << "Crtl+KeyLeft";
        }
        else if (key==Qt::Key_Right)
        {
        mainWin->doRight();
        //qDebug() << "Crtl+KeyRight";
        }
        else
        e->ignore();
    }
    else
    {
        if (key==Qt::Key_Up)
        {
        verticalScrollBar()->setValue(verticalScrollBar()->value()-verticalScrollBar()->singleStep());
        }
        else if (key==Qt::Key_Down)
        {
        verticalScrollBar()->setValue(verticalScrollBar()->value()+verticalScrollBar()->singleStep());
        }
        else if (key==Qt::Key_Left)
        {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value()-horizontalScrollBar()->singleStep());
        }
        else if (key==Qt::Key_Right)
        {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value()+horizontalScrollBar()->singleStep());
        }
        else if (key==Qt::Key_PageUp)
        {
        verticalScrollBar()->setValue(verticalScrollBar()->value()-verticalScrollBar()->pageStep());
        }
        else if (key==Qt::Key_PageDown)
        {
        verticalScrollBar()->setValue(verticalScrollBar()->value()+verticalScrollBar()->pageStep());
        }
        else
        e->ignore();
    }
}

void myScrollArea::wheelEvent(QWheelEvent * e)
{
static bool wheel_active=false;
if (wheel_active==true) return;
int target_axis=ALL_AXES,true_graph;
wheel_active=true;
#if QT_VERSION >= 0x050000
double degrees=e->angleDelta().y()/8.0/15.0;
if (e->angleDelta().y()==0) degrees=e->angleDelta().x()/8.0/15.0;
#else
double degrees=double(e->delta())/8.0/15.0;//no difference in direction here
#endif
if (degrees==0.0)
{
    qDebug() << "WheelAngle=0 !?";
    wheel_active=false;
    return;
}
int tmp_x,tmp_y;//,cur_gr=get_cg();
view bb1;
//VPoint gr_cp;
ClickTarget ct;
static int parameters[4];

if ((e->modifiers() & Qt::AltModifier) || (get_action()==ZOOMX_1ST || get_action()==ZOOMX_2ND))
{
target_axis=X_AXIS;
}
else if ((e->modifiers() & Qt::ControlModifier) || (get_action()==ZOOMY_1ST || get_action()==ZOOMY_2ND))
{
target_axis=Y_AXIS;
}

#if QT_VERSION >= 0x060000
hotSpot=e->position().toPoint();
#else
hotSpot=e->pos();
#endif
//mainWin->mainArea->transf_window_coords(hotSpot.x(),hotSpot.y(),tmp_x,tmp_y);

QPoint p=mainWin->mainArea->lblBackGr->mapFrom(this,hotSpot);
QPoint p1=this->mapToGlobal(hotSpot);
QPoint p2=mainWin->mainArea->lblBackGr->mapToGlobal(QPoint(0,0));

p.setX(p1.x()-p2.x());
p.setY(p1.y()-p2.y());
tmp_x=p.x();
tmp_y=p.y();

realVPos = xlibdev2VPoint(tmp_x,tmp_y);
parameters[0]=next_graph_containing(get_cg(),realVPos);
true_graph=get_graph_truly_containing_vp(realVPos);
view2_graph_world(parameters[0],realVPos.x, realVPos.y, &realWPos.x, &realWPos.y);
get_graph_world(parameters[0],&orig_world);

//qDebug() << "True-Graph=" << true_graph;
//qDebug() << "WheelEvent: Graph=" << parameters[0] << " Viewport-Position=" << realVPos.x << "|" << realVPos.y << " WorldPos=" << realWPos.x << "|" << realWPos.y;

//view2world(realVPos.x, realVPos.y, &realWPos.x, &realWPos.y);
/// int newg = next_graph_containing(-1, realVPos);
//hotSpot=this->mapFromGlobal(e->pos());
//qDebug() << "Degrees=" << degrees << " Gno=" << newg << " Faktor=" << 1.0-shexper*degrees << " Pos=" << realWPos.x << "|" << realWPos.y;
    if (allow_wheel_zoom || allow_wheel_changes)
    {
        if (allow_wheel_changes==FALSE)//only zooming
        {
        //parameters[0]=next_graph_containing(get_cg(),realVPos);
            if (parameters[0]>=0)
            {
            get_graph_world(parameters[0],&orig_world);
            zoomGraphWorldKeepingFixedPoint(parameters[0],orig_world,realWPos,1.0-shexper*degrees*3.0,target_axis);
            drawgraph();
            set_dirtystate();
            }
        }
        else//changes are allowed
        {
        ct=getClickTarget(realVPos,&bb1,parameters);
            if (allow_wheel_zoom && true_graph>=0 && ct!=TARGET_LEGEND && ct!=TARGET_LEGEND_BOX && ct!=TARGET_OBJECT)
            {
            get_graph_world(true_graph,&orig_world);
            zoomGraphWorldKeepingFixedPoint(true_graph,orig_world,realWPos,1.0-shexper*degrees*3.0,target_axis);
            drawgraph();
            set_dirtystate();
            wheel_active=false;
            e->accept();
            return;
            }
        //debugClickTarget(ct,bb1,parameters);
        switch (ct)
        {
        case TARGET_GRAPH:
        case TARGET_SET:
            if (allow_wheel_zoom)
            {
            get_graph_world(parameters[0],&orig_world);
            zoomGraphWorldKeepingFixedPoint(parameters[0],orig_world,realWPos,1.0-shexper*degrees*3.0,target_axis);
            drawgraph();
            set_dirtystate();
            }
            break;
        case TARGET_TITLE:
            change_title_size(parameters[0],parameters[1],1.0+0.1*degrees/fabs(degrees));
            break;
        case TARGET_AXIS_LABEL:
            change_axis_label_size(parameters[0],parameters[1],1.0+0.1*degrees/fabs(degrees));
            break;
        case TARGET_AXIS:
            //we have to test further if it is the Axis or the Alt-Axis
            if (parameters[1]==X_AXIS)///check for tick-label-placement: normal/oposite/both
            {
                if (fabs(realVPos.y - bb1.yv1) < MAXPICKDIST)
                {
                parameters[1]=X_AXIS;
                }
                else
                {
                parameters[1]=ZX_AXIS;
                }
            }
            else
            {
                if (fabs(realVPos.x - bb1.xv1) < MAXPICKDIST)
                {
                parameters[1]=Y_AXIS;
                }
                else
                {
                parameters[1]=ZY_AXIS;
                }
            }
            change_axis_tick_label_size(parameters[0],parameters[1],1.0+0.1*degrees/fabs(degrees));
            break;
        case TARGET_TIMESTAMP:
            change_timestamp_size(1.0+0.1*degrees/fabs(degrees));
            break;
        case TARGET_LEGEND:
        case TARGET_LEGEND_BOX:
            change_legend_font_size(parameters[0],1.0+0.1*degrees/fabs(degrees));
            break;
        case TARGET_OBJECT:
            change_object_size(parameters[0],parameters[1],1.0+0.1*degrees/fabs(degrees));
            break;
        default:
            break;
        }
        }
    }
    else
    {
        verticalScrollBar()->setValue(verticalScrollBar()->value()-degrees*verticalScrollBar()->singleStep());
    }
wheel_active=false;
e->accept();
}

MainArea::MainArea(QWidget *parent):QWidget( parent )
{
    //drawArea=new QFrame(this);
    //drawArea->setGeometry(0,0,1,1);
    //drawArea->setFrameStyle(QFrame::Box | QFrame::Raised);
    //scrollH=new QScrollBar(Qt::Horizontal,this);

    //connect(scrollH,SIGNAL(sliderMoved(int)),SLOT(scroll_h(int)));
    //connect(scrollH,SIGNAL(valueChanged(int)),SLOT(change_h(int)));
    //scrollV=new QScrollBar(Qt::Vertical,this);

    //connect(scrollV,SIGNAL(sliderMoved(int)),SLOT(scroll_v(int)));
    //connect(scrollV,SIGNAL(valueChanged(int)),SLOT(change_v(int)));
    //useable_w=drawArea->width();
    //useable_h=drawArea->height();
    setMouseTracking(TRUE);
    setAcceptDrops(true);
    //drawArea->setMouseTracking(TRUE);

    scroll=new myScrollArea(this);
    scroll->setBackgroundRole(QPalette::Window);

    QList<Qt::GestureType> gestures;
    gestures << Qt::PanGesture;
    gestures << Qt::SwipeGesture;
    gestures << Qt::PinchGesture;
    scroll->grabGestures(gestures);

    //connect(scroll->verticalScrollBar(),SIGNAL(sliderMoved(int)),SLOT(scroll_v(int)));
    //connect(scroll->horizontalScrollBar(),SIGNAL(sliderMoved(int)),SLOT(scroll_v(int)));
    lblBackGr=new QLabel(QString(""),this);
    lblBackGr->setGeometry(0,0,20,20);
    lblBackGr->setMouseTracking(TRUE);

    scroll->setMouseTracking(TRUE);
    scroll->setWidget(lblBackGr);

    /*hier*/
    //scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    /*ende*/

    //rubber=new QRubberBand(QRubberBand::Rectangle, lblBackGr);
    rubber=new QRubberBand(QRubberBand::Rectangle, this);
    rubber->hide();
    rubberLine=new QRubberBand(QRubberBand::Line, lblBackGr);
    rubberLine->hide();
    inlineedit=new InLineEditor(lblBackGr);
    inlineedit2=new InLineEditor2(lblBackGr);

    compl_redraw_running=false;
    draw_box=false;
    contentChanged=true;

    layout=new QHBoxLayout;
    //layout->setMargin(2);
    layout->setContentsMargins(2,2,2,2);
    layout->addWidget(scroll);
    setLayout(layout);

    inlineSetPopup=new SetPopup(NULL);
    //inlineSetPopup->setParent(this);
    inlineSetPopup->hide();

    lblBackGr->setAutoFillBackground(false);
    scroll->setAutoFillBackground(true);
}

special_XEvent QMouseToXEvent( QMouseEvent * e )
{
    int x=e->x(),real_x;
    int y=e->y(),real_y;
    mainWin->mainArea->transf_window_coords(x,y,real_x,real_y);
    special_XEvent event;
    event.delta=0;
    event.xbutton.button=e->button();
    event.xbutton.x=real_x;
    event.xbutton.y=real_y;
    event.xmotion.x=event.xbutton.x;
    event.xmotion.y=event.xbutton.y;
#if QT_VERSION >= 0x050000
    if (event.xbutton.button==Qt::LeftButton && e->type()==QEvent::MouseButtonPress)
    {
    ulong delta_click_timestamp=e->timestamp()-last_click_timestamp;
    //qDebug() << "QMouseToXEvent: delta-timestamp=" << delta_click_timestamp << "ms os_dt=" << os_dbl_click_dt << "ms";
    last_click_timestamp=e->timestamp();
        if (delta_click_timestamp<=os_dbl_click_dt)
        {
        event.doubleClick=true;
        }
        else
        {
        event.doubleClick=false;
        }
    }
#endif
    /*if (bool(e->flags() & Qt::MouseEventCreatedDoubleClick))
    qDebug() << "QMouseToXEvent: MouseEventCreatedDoubleClick=" << bool(e->flags() & Qt::MouseEventCreatedDoubleClick);
#if QT_VERSION >= 0x050000
    event.doubleClick=bool(e->flags() & Qt::MouseEventCreatedDoubleClick);
#endif*/
    Qt::KeyboardModifiers mod=e->modifiers();
    if ((mod & Qt::AltModifier)==Qt::AltModifier)
        event.alt=true;
    else
        event.alt=false;
    if ((mod & Qt::ControlModifier)==Qt::ControlModifier)
        event.ctrl=true;
    else
        event.ctrl=false;
    if ((mod & Qt::ShiftModifier)==Qt::ShiftModifier)
        event.shift=true;
    else
        event.shift=false;

    if (event.alt==true && event.ctrl==false) event.axis_restrict=X_AXIS;
    else if (event.alt==false && event.ctrl==true) event.axis_restrict=Y_AXIS;
    else event.axis_restrict=NO_AXIS;

    return event;
}

special_XEvent QKeyToXEvent( QKeyEvent * e )
{
    special_XEvent event;
    event.doubleClick=false;
    event.type=QEvent::KeyPress;
    event.delta=0;
    event.key=e->key();
    event.xbutton.x=mainWin->mainArea->cursor().pos().x();
    event.xbutton.y=mainWin->mainArea->cursor().pos().y();
    event.xmotion.x=event.xbutton.x;
    event.xmotion.y=event.xbutton.y;
    event.xbutton.button=Qt::NoButton;
    Qt::KeyboardModifiers mod=e->modifiers();
    if ((mod & Qt::AltModifier)==Qt::AltModifier)
        event.alt=true;
    else
        event.alt=false;
    if ((mod & Qt::ControlModifier)==Qt::ControlModifier)
        event.ctrl=true;
    else
        event.ctrl=false;
    if ((mod & Qt::ShiftModifier)==Qt::ShiftModifier)
        event.shift=true;
    else
        event.shift=false;

    if (event.alt==true && event.ctrl==false) event.axis_restrict=X_AXIS;
    else if (event.alt==false && event.ctrl==true) event.axis_restrict=Y_AXIS;
    else event.axis_restrict=NO_AXIS;

    return event;
}

special_XEvent QWheelToXEvent( QWheelEvent * e )
{
#if QT_VERSION >= 0x051500
    int x=e->position().x(),real_x;
    int y=e->position().y(),real_y;
#else
    int x=e->x(),real_x;
    int y=e->y(),real_y;
#endif
    mainWin->mainArea->transf_window_coords(x,y,real_x,real_y);
    special_XEvent event;
    event.doubleClick=false;
    event.type=QEvent::Wheel;
    event.xbutton.button=Qt::NoButton;
    event.xbutton.x=real_x;
    event.xbutton.y=real_y;
    event.xmotion.x=event.xbutton.x;
    event.xmotion.y=event.xbutton.y;
    event.delta=e->angleDelta().manhattanLength();
    Qt::KeyboardModifiers mod=e->modifiers();
    if ((mod & Qt::AltModifier)==Qt::AltModifier)
        event.alt=true;
    else
        event.alt=false;
    if ((mod & Qt::ControlModifier)==Qt::ControlModifier)
        event.ctrl=true;
    else
        event.ctrl=false;
    if ((mod & Qt::ShiftModifier)==Qt::ShiftModifier)
        event.shift=true;
    else
        event.shift=false;

    if (event.alt==true && event.ctrl==false) event.axis_restrict=X_AXIS;
    else if (event.alt==false && event.ctrl==true) event.axis_restrict=Y_AXIS;
    else event.axis_restrict=NO_AXIS;

    return event;
}

void limit_viewport(int & posx,int & posy,int & dx,int & dy)
{
static Page_geometry pg;
static Device_entry dev;
static int delta1;
    dev = get_device_props(DEVICE_SCREEN);
    pg = dev.pg;
    pg.width*=GeneralPageZoomFactor;
    pg.height*=GeneralPageZoomFactor;

    if (posx<pg.width*0.01*border_percent)
    {
    delta1=pg.width*0.01*border_percent-posx;
    posx+=delta1;
    dx-=delta1;
    }
    else if (posx+dx>pg.width*0.01*(100-border_percent))
    {
    delta1=posx+dx-pg.width*0.01*(100-border_percent);
    dx-=delta1;
    }

    if (posy<pg.height*0.01*border_percent)
    {
    delta1=pg.height*0.01*border_percent-posy;
    posy+=delta1;
    dy-=delta1;
    }
    else if (posy+dy>pg.height*0.01*(100-border_percent))
    {
    delta1=posy+dy-pg.height*0.01*(100-border_percent);
    dy-=delta1;
    }
}

void limit_viewport(double & x1,double & y1,double & x2,double & y2)
{
static Page_geometry pg;
static Device_entry dev;
static double x_factor,y_factor;
    dev = get_device_props(DEVICE_SCREEN);
    pg = dev.pg;
x_factor=1.0;
y_factor=1.0;
if (pg.height<pg.width)//the height is [0-1]
{
x_factor=pg.width/(1.0*pg.height);
}
else//width is [0-1]
{
y_factor=pg.height/(1.0*pg.width);
}
    if (x1<0.01*border_percent*x_factor) x1=0.01*border_percent*x_factor;
    if (x2>0.01*(100-border_percent)*x_factor) x2=0.01*(100-border_percent)*x_factor;
    if (y1<0.01*border_percent*y_factor) y1=0.01*border_percent*y_factor;
    if (y2>0.01*(100-border_percent)*y_factor) y2=0.01*(100-border_percent)*y_factor;
}

void QPoint_to_RealPolarCoords(QPoint p,double & r,double & phi)
{
static VPoint vp;
vp=xlibdev2VPoint(p.x(), p.y());
view2world(vp.x, vp.y, &phi, &r);
}

void MainArea::mouseMoveEvent(QMouseEvent * e)
{
special_XEvent event=QMouseToXEvent(e);
legend tmp_l;
double tmp_phi;
world wtemp;
    e->accept();
//cout << "(" << e->x() << "|" << e->y() << ")" << endl;
    if (action_flag!=CORNER_2ND && (event.xbutton.x<0 || event.xbutton.x>MainPixmap->width() || event.xbutton.y<0 || event.xbutton.y>MainPixmap->height())) return;
    event.type=QEvent::MouseMove;
int setno,move_dir,add_at;
int dx,dy,posx,posy;
int parameters[3];
    switch (action_flag)
    {
    case ZOOM_2ND:
    case ZOOMX_2ND:
    case ZOOMY_2ND:
        if (get_graph_type(get_cg())==GRAPH_POLAR2)
        {
        simple_draw_setting |= SIMPLE_DRAW_RUBBER_DONUT;
                if (action_flag==ZOOMX_2ND)
                donut_type=1;//Phi-zooming
                else
                donut_type=0;//R-zooming
            get_graph_world(get_cg(),&wtemp);
                if (donut_type==0)//R-zooming
                {
                QPoint_to_RealPolarCoords(origin,zoom_donut1,tmp_phi);
                QPoint_to_RealPolarCoords(lblBackGr->mapFrom(this,e->pos()),zoom_donut2,tmp_phi);
                }
                else//Phi-zooming
                {
                QPoint_to_RealPolarCoords(origin,tmp_phi,zoom_donut1);
                QPoint_to_RealPolarCoords(lblBackGr->mapFrom(this,e->pos()),tmp_phi,zoom_donut2);
                while (zoom_donut1<0) zoom_donut1+=2.0*M_PI;
                while (zoom_donut2<0) zoom_donut2+=2.0*M_PI;
                while (zoom_donut1>2*M_PI) zoom_donut1-=2.0*M_PI;
                while (zoom_donut2>2*M_PI) zoom_donut2-=2.0*M_PI;
//qDebug() << "Phi1=" << zoom_donut1/M_PI*180.0 << " Phi2=" << zoom_donut2/M_PI*180.0;
                }
            if (zoom_donut1>zoom_donut2)
            {
            tmp_phi=zoom_donut1;
            zoom_donut1=zoom_donut2;
            zoom_donut2=tmp_phi;
            }
                if (donut_type==0)//R-zooming
                {
                    if (zoom_donut1<wtemp.yg1) zoom_donut1=wtemp.yg1;
                    if (zoom_donut2<wtemp.yg1) zoom_donut2=wtemp.yg1;
                    if (zoom_donut1>wtemp.yg2) zoom_donut1=wtemp.yg2;
                    if (zoom_donut2>wtemp.yg2) zoom_donut2=wtemp.yg2;
                }
                else//Phi-zooming
                {
                    if (zoom_donut1<wtemp.xg1) zoom_donut1=wtemp.xg1;
                    if (zoom_donut2<wtemp.xg1) zoom_donut2=wtemp.xg1;
                    if (zoom_donut1>wtemp.xg2) zoom_donut1=wtemp.xg2;
                    if (zoom_donut2>wtemp.xg2) zoom_donut2=wtemp.xg2;
//qDebug() << "korrigiert: Phi1=" << zoom_donut1/M_PI*180.0 << " Phi2=" << zoom_donut2/M_PI*180.0;
                }
        drawgraph();
        }
        else//the normal (rectangular) zooming
        {
        box_end_x=event.xbutton.x;
        box_end_y=event.xbutton.y;
        //rubber->setGeometry(QRect(origin, lblBackGr->mapFrom(this,e->pos())).normalized());
        repositionRubber(QRect(origin, lblBackGr->mapFrom(this,e->pos())).normalized());
        }
        break;
    case MAKE_ELLIP_2ND:
    case MAKE_BOX_2ND:
        box_end_x=event.xbutton.x;
        box_end_y=event.xbutton.y;
        //rubber->setGeometry(QRect(origin, lblBackGr->mapFrom(this,e->pos())).normalized());
        repositionRubber(QRect(origin, lblBackGr->mapFrom(this,e->pos())).normalized());
        break;
    case PLACE_LEGEND_2ND:
        get_graph_legend(get_cg(), &tmp_l);

        if (current_axis_restriction==X_AXIS)
        {
        event.xbutton.y=ShiftPoint.y();
        }
        else if (current_axis_restriction==Y_AXIS)
        {
        event.xbutton.x=ShiftPoint.x();
        }

        box_end_x=event.xbutton.x;
        box_end_y=event.xbutton.y;
        if (tmp_l.autoattach!=G_LB_ATTACH_NONE)
        {
            if ( (tmp_l.autoattach & G_LB_ATTACH_LEFT)!=0 || (tmp_l.autoattach & G_LB_ATTACH_RIGHT)!=0 )
            {
            //mainWin->mainArea->rubber->setGeometry(ShiftRect.translated(0,event.xbutton.y-ShiftPoint.y()));
            repositionRubber(ShiftRect.translated(0,event.xbutton.y-ShiftPoint.y()));
            }
            if ( (tmp_l.autoattach & G_LB_ATTACH_TOP)!=0 || (tmp_l.autoattach & G_LB_ATTACH_BOTTOM)!=0 )
            {
            //mainWin->mainArea->rubber->setGeometry(ShiftRect.translated(event.xbutton.x-ShiftPoint.x(),0));
            repositionRubber(ShiftRect.translated(event.xbutton.x-ShiftPoint.x(),0));
            }
        }
        else
        {
        //mainWin->mainArea->rubber->setGeometry(ShiftRect.translated(event.xbutton.x-ShiftPoint.x(),event.xbutton.y-ShiftPoint.y()));
        repositionRubber(ShiftRect.translated(event.xbutton.x-ShiftPoint.x(),event.xbutton.y-ShiftPoint.y()));
        }
        break;
    case MOVE_OBJECT_2ND:
    case COPY_OBJECT2ND:
    case PLACE_TIMESTAMP_2ND:
    case MOVE_AXIS_LABEL_2ND:
    case MOVE_GRAPH_2ND:
    case MOVE_TITLE_2ND:

        if (current_axis_restriction==X_AXIS)
        {
        event.xbutton.y=ShiftPoint.y();
        }
        else if (current_axis_restriction==Y_AXIS)
        {
        event.xbutton.x=ShiftPoint.x();
        }

        box_end_x=event.xbutton.x;
        box_end_y=event.xbutton.y;
        dx=event.xbutton.x-ShiftPoint.x();
        dy=event.xbutton.y-ShiftPoint.y();
        if (current_object_type==OBJECT_STRING)
        {
            if (is_valid_string(current_object_id)==TRUE)
            {
            parameters[0]=(pstr[current_object_id].master_align/100)%100;//none/graph/plot
            parameters[1]=(pstr[current_object_id].master_align/10)%10;//x
            parameters[2]=pstr[current_object_id].master_align%10;//y
                if (parameters[0]!=0)
                {
                    if (parameters[1]!=0)//fixed in x
                    {
                    dx=0;
                    }
                    if (parameters[2]!=0)//fixed in y
                    {
                    dy=0;
                    }
                }
            }
        }
        //mainWin->mainArea->rubber->setGeometry(ShiftRect.translated(event.xbutton.x-ShiftPoint.x(),event.xbutton.y-ShiftPoint.y()));
        repositionRubber(ShiftRect.translated(dx,dy));
        //rubber->setGeometry(QRect(origin, lblBackGr->mapFrom(this,e->pos())).normalized());
        break;
    case VIEW_2ND:
        box_end_x=event.xbutton.x;
        box_end_y=event.xbutton.y;
        posx=anchor_x<box_end_x?anchor_x:box_end_x;
        posy=anchor_y<box_end_y?anchor_y:box_end_y;
        dx=abs(anchor_x-box_end_x);
        dy=abs(anchor_y-box_end_y);
            limit_viewport(posx,posy,dx,dy);
        repositionRubber(QRect(posx,posy,dx,dy));
        break;
    case CORNER_2ND:
        box_end_x=event.xbutton.x;
        box_end_y=event.xbutton.y;
        /*posx=anchor_x<box_end_x?anchor_x:box_end_x;
        posy=anchor_y<box_end_y?anchor_y:box_end_y;
        dx=abs(anchor_x-box_end_x);
        dy=abs(anchor_y-box_end_y);
        repositionRubber(QRect(posx,posy,dx,dy));*/
        repositionRubber(QRect(0,0,box_end_x,box_end_y));
        break;
    case MAKE_LINE_2ND:

        break;
    case MOVE_POINT2ND:
        get_tracking_props(&setno,&move_dir,&add_at);
        /*switch (move_dir) {
        case MOVE_POINT_XY:
            rubberLine->setGeometry(QRect(origin, lblBackGr->mapFrom(this,e->pos())).normalized());
            break;
        case MOVE_POINT_Y:
            rubberLine->setGeometry(QRect(origin, lblBackGr->mapFrom(this,QPoint(origin.x(),e->y()))).normalized());
            break;
        case MOVE_POINT_X:
            rubberLine->setGeometry(QRect(origin, lblBackGr->mapFrom(this,QPoint(e->x(),origin.y()))).normalized());
            break;
        }*/
        break;
    default:
        break;
    }
    //GeneralPurposeTimer1.restart();
    my_proc(&event);
    //qDebug() << "moveEvent: myproc_timer=" << GeneralPurposeTimer1.restart() << "ms";
}

void MainArea::mousePressEvent(QMouseEvent * e)
{
//cout << "Mouse Press 0: " << e->x() << "|" << e->y() << endl;
    e->accept();
    QCursor curs=cursor();
    QCursor curs2(Qt::WhatsThisCursor);
    if (curs.shape()==curs2.shape())
    {
        QString helpfile=QString(qt_grace_doc_dir)+QDir::separator()+QString("UsersGuide.html#canvas");
        HelpCB(helpfile.toLocal8Bit().constData());
        setCursor(QCursor(Qt::ArrowCursor));
        //unsetCursor();
        return;
    }
    special_XEvent event=QMouseToXEvent(e);
    event.doubleClick=false;
    event.type=QEvent::MouseButtonPress;
    origin = lblBackGr->mapFrom(this,e->pos());
    //GeneralPurposeTimer1.restart();
    processClickCommand(event);
//qDebug() << "pressEvent: myproc_timer=" << GeneralPurposeTimer1.restart() << "ms";
//cout << "Mouse Press 1: " << e->x() << "|" << e->y() << endl;
}

void MainArea::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void MainArea::dropEvent(QDropEvent *event)
{
    GeneralPaste(event->mimeData());
}

void MainArea::processClickCommand(special_XEvent & event)
{
    switch (action_flag)
    {
    case ZOOM_1ST:
    case ZOOMX_1ST:
    case ZOOMY_1ST:
    case MAKE_ELLIP_1ST:
    case MAKE_BOX_1ST:
        box_end_x=box_start_x=event.xbutton.x;
        box_end_y=box_start_y=event.xbutton.y;
        rubber->setGeometry(QRect(origin, QSize()));
        rubber->show();
        draw_box=true;
        break;
    case COPY_OBJECT1ST:
    case MOVE_OBJECT_1ST:
    case PLACE_LEGEND_1ST:
        box_end_x=box_start_x=event.xbutton.x;
        box_end_y=box_start_y=event.xbutton.y;
        draw_box=true;
        break;
    case ZOOM_2ND:
    case ZOOMX_2ND:
    case ZOOMY_2ND:
    case MAKE_ELLIP_2ND:
    case MAKE_BOX_2ND:
        draw_box=false;
        rubber->hide();
        parentWidget()->setCursor(Qt::ArrowCursor);
        break;
    case MOVE_POINT1ST:
        box_end_x=box_start_x=event.xbutton.x;
        box_end_y=box_start_y=event.xbutton.y;
        /// rubberLine->setGeometry(QRect(origin, QSize()));
        /// rubberLine->show();
        rubberLine->hide();
        break;
    case MOVE_POINT2ND:
        rubberLine->hide();
        break;
    default:
        break;
    }
    my_proc(&event);
}

void MainArea::mouseDoubleClickEvent(QMouseEvent * e )
{
    special_XEvent event=QMouseToXEvent(e);
//qDebug() << "Double Click Recognized!";
    event.doubleClick=true;
    event.type=QEvent::MouseButtonPress;
    processClickCommand(event);
}

void MainArea::mouseReleaseEvent(QMouseEvent * e )
{
    special_XEvent event=QMouseToXEvent(e);
    event.type=QEvent::MouseButtonRelease;
    //GeneralPurposeTimer1.restart();
    my_proc(&event);
//qDebug() << "releaseEvent: myproc_timer=" << GeneralPurposeTimer1.restart() << "ms";
    mainWin->mouseReleaseEvent(e);
}

void MainArea::keyPressEvent( QKeyEvent * e )
{
    special_XEvent event=QKeyToXEvent(e);
    my_proc(&event);
/*qDebug() << "Key=" << event.key << " Crtl=" << event.ctrl << " Alt=" << event.alt;
        if (event.ctrl==true && (event.key==Qt::Key_Up || event.key==Qt::Key_Down || event.key==Qt::Key_Left || event.key==Qt::Key_Right))
        {
        e->accept();
        return;
        }*/
    mainWin->keyPressEvent( e );
}

void MainArea::keyReleaseEvent( QKeyEvent * e )
{
    special_XEvent event=QKeyToXEvent(e);
    event.type=QEvent::KeyRelease;
    my_proc(&event);
    mainWin->keyPressEvent( e );
}

void MainArea::wheelEvent( QWheelEvent * e )
{
    special_XEvent event=QWheelToXEvent(e);
    my_proc(&event);
}

QImage paintXOR(QImage * canvas,QImage * toDraw)//draw "toDraw" at position in XOR on "canvas" (no check for sizes, to Draw has to be at least as big as canvas; the result will have the same size as canvas)
{
    int c_width=canvas->width(),c_height=canvas->height();
    //Invert image to be painted --> white areas in toDraw will leave canvas untouched
    toDraw->invertPixels();
    //generate an empty image for the result
    QImage resultImage(c_width,c_height,QImage::Format_ARGB32_Premultiplied);
    //buffers for every pixel
    QRgb col;
    QColor col2;
    int cr[3],cg[3],cb[3],ca[3];
    //go through every line
    for (int i=0;i<c_width;i++)
    {
        //go through every row
        for (int j=0;j<c_height;j++)
        {
            //get color of canvas
            col=canvas->pixel(i,j);
            col2.setRgb(col);
            col2.getRgb(cr,cg,cb,ca);
            //get color of toDraw
            col=toDraw->pixel(i,j);
            col2.setRgb(col);
            col2.getRgb(cr+1,cg+1,cb+1,ca+1);
            //bitwise XOR-operation
            cr[2]=cr[0]^cr[1];
            cg[2]=cg[0]^cg[1];
            cb[2]=cb[0]^cb[1];
            //generate an RGB-value
            col2.setRgb(cr[2],cg[2],cb[2]);
            //set the pixel in the resulting image
            resultImage.setPixel(i,j,col2.rgb());
        }
    }
    return resultImage;
}

void force_redraw(void)
{
int sav_simple=simple_draw_setting;
int sav_stop_repaiont=stop_repaint;
int sav_print_target=print_target;
int sav_print_in_file=printing_in_file;
int sav_startupphase=startupphase;
bool sav_update_running=updateRunning;

stop_repaint=FALSE;
updateRunning=false;
simple_draw_setting=SIMPLE_DRAW_NONE;
print_target=PRINT_TARGET_SCREEN;
startupphase=0;
printing_in_file=false;

mainWin->mainArea->completeRedraw();

startupphase=sav_startupphase;
printing_in_file=sav_print_in_file;
print_target=sav_print_target;
stop_repaint=sav_stop_repaiont;
simple_draw_setting=sav_simple;
updateRunning=sav_update_running;
}

void force_redraw_wo_dialog_updates(void)
{
int sav_simple=simple_draw_setting;
int sav_stop_repaiont=stop_repaint;
int sav_print_target=print_target;
int sav_print_in_file=printing_in_file;
int sav_startupphase=startupphase;
stop_repaint=FALSE;
simple_draw_setting=SIMPLE_DRAW_NONE;
print_target=PRINT_TARGET_SCREEN;
startupphase=0;
printing_in_file=false;
drawgraph();
startupphase=sav_startupphase;
printing_in_file=sav_print_in_file;
print_target=sav_print_target;
stop_repaint=sav_stop_repaiont;
simple_draw_setting=sav_simple;
}

void MainArea::completeRedraw(void)
{
//qDebug() << "Start Complete redraw(), running=" << compl_redraw_running;
    if (compl_redraw_running==true)
    {
    return;
    }
    else
    {
    print_target=PRINT_TARGET_SCREEN;//this is the standard (is only changed in do_hardcopy())
    compl_redraw_running=true;
        update_all();
        xdrawgraph();
    compl_redraw_running=false;
    }
//qDebug() << "Ende Complete redraw()";

/*Incorporated into drawgraph as part of xdrawgraph
contentChanged=true;
repaint();*/
}

void drawRegionOnPainter(QPainter * pa,int region)
{
if (region>=0 && region<=MAXREGION)
{
QPoint * points=new QPoint[rg[region].n+2];
WPoint wps;
VPoint vps;
//qDebug() << "region " << region << " points=" << rg[region].n;
    for (int i = 0; i < rg[region].n; i++)
    {
        wps.x=rg[region].x[i];
        wps.y=rg[region].y[i];
    vps=Wpoint2Vpoint(wps);
        points[i] = VPoint2XPoint(vps);
    //cout << "rg.x=" << rg[region].x[i] << " v.x=" << vps.x << " QPoint.x=" << points[i].x() << endl;
    //cout << "rg.y=" << rg[region].y[i] << " v.y=" << vps.y << " QPoint.y=" << points[i].y() << endl;
    }
    pa->drawPolygon(points,rg[region].n);
delete[] points;
}
}

void drawFocusOnPainter(QPainter * pa)//show which graph is the current one
{
static VPoint vp;
static view v;
//static QPointF tmp_point;
int ret=get_graph_viewport(get_cg(),&v);
if (ret==RETURN_FAILURE) return;
static Page_geometry pg;
static int FocusSize,x,y;
static QImage hd_copy,content,res;
static QPainter painter;
static int ix1, iy1, ix2, iy2;
pg=get_page_geometry();
FocusSize=(pg.width<pg.height?pg.width:pg.height)*(1.0+0.5*(GeneralPageZoomFactor-1.0));
FocusSize/=61;
if (FocusSize%2!=0) FocusSize++;//for symmetry reasons we want even numbers
if (FocusSize<10) FocusSize=10;
//qDebug() << "pg.width=" << pg.width << "pg.height=" << pg.height << "PageZoom=" << GeneralPageZoomFactor << "--> FocusSize=" << FocusSize;

vp.x=v.xv1;
vp.y=v.yv1;
xlibVPoint2dev(vp, &ix1, &iy1);
vp.x=v.xv2;
vp.y=v.yv2;
xlibVPoint2dev(vp, &ix2, &iy2);

/*tmp_point=VPoint2XPointF(vp);
x=(int)(tmp_point.x()-0.5*FocusSize);
y=(int)(tmp_point.y()-0.5*FocusSize);*/

x=(int)(1.0*ix1-0.5*FocusSize);
y=(int)(1.0*iy1-0.5*FocusSize);

hd_copy=MainPixmap->copy(x,y,FocusSize,FocusSize);
content=QImage(hd_copy.width(),hd_copy.height(),hd_copy.format());//create an image with the same size
painter.begin(&content);
painter.setBrush(Qt::black);
painter.drawRect(0,0,FocusSize,FocusSize);
painter.end();
res=paintXOR(&hd_copy,&content);
pa->drawImage(x,y,res);

/*vp.x=v.xv2;
vp.y=v.yv1;
tmp_point=VPoint2XPointF(vp);
x=(int)tmp_point.x()-FocusSize/2;
y=(int)tmp_point.y()-FocusSize/2;*/

x=(int)(1.0*ix1-0.5*FocusSize);
y=(int)(1.0*iy2-0.5*FocusSize);

hd_copy=MainPixmap->copy(x,y,FocusSize,FocusSize);
content=QImage(hd_copy.width(),hd_copy.height(),hd_copy.format());//create an image with the same size
painter.begin(&content);
painter.setBrush(Qt::black);
painter.drawRect(0,0,FocusSize,FocusSize);
painter.end();
res=paintXOR(&hd_copy,&content);
pa->drawImage(x,y,res);

/*vp.x=v.xv2;
vp.y=v.yv2;
tmp_point=VPoint2XPointF(vp);
x=(int)tmp_point.x()-FocusSize/2;
y=(int)tmp_point.y()-FocusSize/2;*/

x=(int)(1.0*ix2-0.5*FocusSize);
y=(int)(1.0*iy1-0.5*FocusSize);

hd_copy=MainPixmap->copy(x,y,FocusSize,FocusSize);
content=QImage(hd_copy.width(),hd_copy.height(),hd_copy.format());//create an image with the same size
painter.begin(&content);
painter.setBrush(Qt::black);
painter.drawRect(0,0,FocusSize,FocusSize);
painter.end();
res=paintXOR(&hd_copy,&content);
pa->drawImage(x,y,res);

/*vp.x=v.xv1;
vp.y=v.yv2;
tmp_point=VPoint2XPointF(vp);
x=(int)tmp_point.x()-FocusSize/2;
y=(int)tmp_point.y()-FocusSize/2;*/

x=(int)(1.0*ix2-0.5*FocusSize);
y=(int)(1.0*iy2-0.5*FocusSize);

hd_copy=MainPixmap->copy(x,y,FocusSize,FocusSize);
content=QImage(hd_copy.width(),hd_copy.height(),hd_copy.format());//create an image with the same size
painter.begin(&content);
painter.setBrush(Qt::black);
painter.drawRect(0,0,FocusSize,FocusSize);
painter.end();
res=paintXOR(&hd_copy,&content);
pa->drawImage(x,y,res);
}

void drawHighlightOnPainter(QPainter * pa)
{
static view bb1;
static VPoint vp;
static QPointF tmp_point,tmp_point2;
static qreal temp1;
static QColor col;
pa->setBrush(Qt::NoBrush);
QPen pen(Qt::DashLine);
col=get_Color(highlight_color);
col.setAlpha(highlight_alpha);
pen.setColor(col);
pen.setWidth(highlight_width);
pa->setPen(pen);
    for (int i=0;i<nr_of_highlight_segments;i++)
    {
        if (highlight_clipping.at(i)>=0)//clipping on a single graph
        {
        get_graph_viewport(highlight_clipping.at(i),&bb1);
            vp.x=bb1.xv1;
            vp.y=bb1.yv1;
        tmp_point=VPoint2XPointF(vp);
            vp.x=bb1.xv2;
            vp.y=bb1.yv2;
        tmp_point2=VPoint2XPointF(vp);
        if (tmp_point.x() > tmp_point2.x())
        {
        temp1=tmp_point.x();
        tmp_point.setX(tmp_point2.x());
        tmp_point2.setX(temp1);
        }
        if (tmp_point.y() > tmp_point2.y())
        {
        temp1=tmp_point.y();
        tmp_point.setY(tmp_point2.y());
        tmp_point2.setY(temp1);
        }
        pa->setClipRect(QRectF(tmp_point,tmp_point2));
        pa->setClipping(true);
        }
        else
        {
        pa->setClipping(false);
        }

        if (highlight_type.at(i)==-1)//a line
        pa->drawPolyline(highlight_segments[i]);
        else//a symbol
        {
            for (int j=0;j<highlight_segments[i].size();j++)
            {
            drawSimpleSymbol(*pa,highlight_segments[i].at(j).x()+1,highlight_segments[i].at(j).y()+1,15,(highlight_type.at(i)>SYM_CHAR?SYM_CHAR:highlight_type.at(i)),highlight_type.at(i));
            }
        }
    }
pa->setClipping(false);
}

void drawRubberDonutOnPainter(QPainter * pa)
{
static QColor col1(10,70,255,90);
VPoint vp,vpc;
world wtmp;
double d1,d2,dphi;
int x1,x2,y1,y2;
world wtmp2;
get_graph_world(get_cg(),&wtmp);

dphi=g[get_cg()].phi0;

//determine center of donut (reference for all calculations)
world2view(0.0,0.5*(wtmp.yg2+wtmp.yg1),&vpc.x,&vpc.y);
world2view(M_PI,0.5*(wtmp.yg2+wtmp.yg1),&vp.x,&vp.y);
vpc.x+=vp.x;
vpc.x*=0.5;
vpc.y+=vp.y;
vpc.y*=0.5;

wtmp2.xg1=wtmp2.xg2=-dphi;

if (donut_type==0)//donut --> r-scaling
{
wtmp2.yg1=zoom_donut1;//use only limited radial range here
wtmp2.yg2=zoom_donut2;
}
else//pie-slice --> phi-scaling
{
wtmp2.yg1=wtmp.yg1;//use full radial range here
wtmp2.yg2=wtmp.yg2;
}

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

QPainterPath pp;
pp.addRect(MainPixmap->rect());
pp.addEllipse(x1,y1,x2-x1+1,y2-y1+1);//central area is empty (clipping area)

vp.x=vpc.x-d2;
vp.y=vpc.y+d2;
xlibVPoint2dev(vp,&x1,&y1);
vp.x=vpc.x+d2;
vp.y=vpc.y-d2;
xlibVPoint2dev(vp,&x2,&y2);//outer circle

pa->setClipPath(pp);
pa->setBrush(col1);
    if (donut_type==0)//donut --> r-scaling
    {
    pa->drawEllipse(x1,y1,x2-x1+1,y2-y1+1);
    }
    else//pie-slice --> phi-scaling
    {
    d1=((zoom_donut1<zoom_donut2?zoom_donut1:zoom_donut2)+dphi)/M_PI*180.0*16;
    pa->drawPie(x1,y1,x2-x1+1,y2-y1+1,int(d1),int(fabs(zoom_donut2-zoom_donut1)/M_PI*180.0*16));
    //qDebug() << "a=" << int(d1)/16 << " alen=" << int(fabs(zoom_donut2-zoom_donut1)/M_PI*180.0);
    }
}

void drawLocatorFixPointOnPainter(QPainter * pa)
{
    (void)pa;//this is wrong --> remove this later
    GLocator locator;
    int gno=get_cg();
    if (get_graph_locator(gno, &locator) != RETURN_SUCCESS) {
        //actual drawing missing here
        return;
    }

}

void MainArea::paintEvent( QPaintEvent *e )
{
static bool paint_event_running=false;
if (paint_event_running==true) return;
paint_event_running=true;
    (void)e;
static QPixmap pm;
//qDebug() << "PaintEvent, simple_Draw_Setting=" << simple_draw_setting << "contentChanged=" << contentChanged;
    if (contentChanged==false && simple_draw_setting==SIMPLE_DRAW_NONE)
    {
    paint_event_running=false;
    return;
    }
    contentChanged=false;

    /*if (cursortype!=0)
    {
        QPoint cpoint=mapFromGlobal(QCursor::pos());
        GeneralPainter->setPen(Qt::black);
        GeneralPainter->drawLine(-4,cpoint.y()-4,width()+4,cpoint.y()-4);
        GeneralPainter->drawLine(cpoint.x()-4,0,cpoint.x()-4,height());
    }*/

    /// QTextCodec *codec = QTextCodec::codecForName("KOI8-R");
    /// GeneralPainter->drawText(50,50,codec->toUnicode(g[0].labs.stitle.s));

    if (monomode == FALSE)
        pm=QPixmap::fromImage(*MainPixmap,Qt::AutoColor);
    else
        pm=QPixmap::fromImage(*MainPixmap,Qt::MonoOnly);

//qDebug() << "actual Image recreation";
//simple-drawing (without refreshing the whole background-graph)
    QPainter tempPainter(&pm);
    /*QPainter tempPainter;
    if (simple_draw_setting==SIMPLE_DRAW_NONE)
    {tempPainter.begin(&pm);}
    else
    {tempPainter.begin(this);}*/
//qDebug() << "PaintEvent: simple_draw_setting=" << simple_draw_setting << " (simple_draw_setting&SIMPLE_DRAW_LINE)=" << (simple_draw_setting & SIMPLE_DRAW_LINE);
    if (cursortype!=0 && (simple_draw_setting & SIMPLE_DRAW_CROSSHAIR) )
    {
        QPoint cpoint=mapFromGlobal(QCursor::pos());
        tempPainter.setPen(Qt::black);
        tempPainter.drawLine(-4,cpoint.y()-3,width()+4,cpoint.y()-3);//-4
        tempPainter.drawLine(cpoint.x()-3,0,cpoint.x()-3,height());//-4
    }
    if (simple_draw_setting & SIMPLE_DRAW_LINE)
    {
    //qDebug() << staticcounter++ << "PaintEvent: simple drawing line";
        if (rg[MAXREGION].active==TRUE)
        drawRegionOnPainter(&tempPainter,MAXREGION);
    }
    if (simple_draw_setting & SIMPLE_DRAW_REGION)
    {
    //cout << "PaintEvent: simple drawing Region" << endl;
        drawRegionOnPainter(&tempPainter,nr);
    }
    if (simple_draw_setting & SIMPLE_DRAW_HIGHLIGHT)
    {
        drawHighlightOnPainter(&tempPainter);
    }
    if (simple_draw_setting & SIMPLE_DRAW_RUBBER_DONUT)
    {
        drawRubberDonutOnPainter(&tempPainter);
    }
    if (simple_draw_setting & SIMPLE_DRAW_LOCATORFIXPOINT)
    {
        drawLocatorFixPointOnPainter(&tempPainter);
    }
    if (draw_focus_flag==TRUE)
    {
        drawFocusOnPainter(&tempPainter);
    }
    tempPainter.end();
    //if (simple_draw_setting==SIMPLE_DRAW_NONE)
    //{
    lblBackGr->setPixmap(pm);
        if (pm.width()!=lblBackGr->width() || pm.height()!=lblBackGr->height())
        {
        lblBackGr->setGeometry(0,0,pm.width(),pm.height());
        }
    //}
    simple_draw_setting=SIMPLE_DRAW_NONE;
//qDebug() << "lblBackGr=" << pm.width() << "x" << pm.height();
    if (showhideworkaround)//a strange thing which is necessary sometimes
    {
        //lblBackGr->hide();
        lblBackGr->update();
        qApp->processEvents();
        //lblBackGr->show();
    }
paint_event_running=false;
}

void MainArea::transf_window_coords(int x,int y,int & real_x,int & real_y)
{
    QPoint p=lblBackGr->mapFrom(this,QPoint(x,y));
    real_x=p.x();
    real_y=p.y();
}

void MainArea::setBGtoColor(QColor col)
{
//cout << "change BG-Color: " << col.red() << " " << col.green() << " " << col.blue() << endl;
QPalette Pal(scroll->palette());
Pal.setColor(QPalette::Window, col);
scroll->setPalette(Pal);
}

void MainArea::repositionRubber(QRect rect)
{
static QRect ShiftRect;
static QPoint p1,p2,p3,p4;
p1.setX(rect.x());
p1.setY(rect.y());
p2.setX(rect.x()+rect.width());
p2.setY(rect.y()+rect.height());
//qDebug() << "p1=" << p1 << " p2=" << p2;
    p3=lblBackGr->mapTo(this,p1);
    p4=lblBackGr->mapTo(this,p2);
//qDebug() << "p3=" << p3 << " p4=" << p4;
    ShiftRect=QRect(p3,p4);
mainWin->mainArea->rubber->setGeometry(ShiftRect);
}

void doPlotFit(void)
{
    mainWin->doFitPage();
}

void read_basic_icons(void)
{
//qDebug() << "ToolBarSizeFactor=" << toolBarSizeFactor << "FontSize=" << qApp->font().pixelSize();
    //generate Patterns for filling styles
    for (int i=0;i<MAXPATTERNS;i++)
    {
        if (patterns[i]!=NULL) delete patterns[i];
            patterns[i]=new QBitmap(16,16);
    }
    init_Patterns();

    //generate main icons
    GraceIcon=new QPixmap(16,16);
    ActiveIcon=new QPixmap(16,16);
    HiddenIcon=new QPixmap(16,16);
char dummy1[80],dummy2[80],dummy3[80];
unsigned int rr,gg,bb;
char dummy[800];
int c_width,c_height,nr_cols,bpc,c_read;
        c_read=sscanf(grace_icon_xpm[0],"%d %d %d %d",&c_width,&c_height,&nr_cols,&bpc);
QImage img1(c_width,c_height,QImage::Format_ARGB32_Premultiplied);
QColor * cols=new QColor[nr_cols];
char * ctrl_char=new char[nr_cols];
        for (int i=0;i<nr_cols;i++)
        {
            c_read=sscanf(grace_icon_xpm[1+i],"%c c #%s",ctrl_char+i,dummy);
            dummy1[0]=dummy2[0]=dummy3[0]='0';
            dummy1[1]=dummy2[1]=dummy3[1]='x';
            for (int j=0;j<4;j++)
            {
                dummy1[2+j]=dummy[j];
                dummy2[2+j]=dummy[j+4];
                dummy3[2+j]=dummy[j+8];
            }
            dummy1[2+4]='\0';
            dummy2[2+4]='\0';
            dummy3[2+4]='\0';
            sscanf(dummy1,"%x",&rr);
            sscanf(dummy2,"%x",&gg);
            sscanf(dummy3,"%x",&bb);
            cols[i]=QColor(rr/256 ,gg/256 ,bb/256 );
        }
        for (int i=0;i<c_width;i++)
        {
            for (int j=0;j<c_height;j++)
            {
                for (int k=0;k<nr_cols;k++)
                {
                    if (ctrl_char[k]==grace_icon_xpm[1+nr_cols+j][i])
                    {
                        img1.setPixel(i,j,cols[k].rgb());
                        break;
                    }
                }
            }
        }
        *GraceIcon=QPixmap::fromImage(img1,Qt::AutoColor);
        delete[] cols;
        delete[] ctrl_char;
        c_read=sscanf(active_xpm[0],"%d %d %d %d",&c_width,&c_height,&nr_cols,&bpc);
        img1=QImage(c_width,c_height,QImage::Format_ARGB32_Premultiplied);
        cols=new QColor[nr_cols];
        ctrl_char=new char[nr_cols];
        for (int i=0;i<nr_cols;i++)
        {
            c_read=sscanf(active_xpm[1+i],"%c c #%s",ctrl_char+i,dummy);
            if (c_read<2)
                c_read=0;
            else
                c_read=255;
            dummy1[0]=dummy2[0]=dummy3[0]='0';
            dummy1[1]=dummy2[1]=dummy3[1]='x';
            for (int j=0;j<2;j++)
            {
                dummy1[2+j]=dummy[j];
                dummy2[2+j]=dummy[j+2];
                dummy3[2+j]=dummy[j+4];
            }
            dummy1[2+2]='\0';
            dummy2[2+2]='\0';
            dummy3[2+2]='\0';
            sscanf(dummy1,"%x",&rr);
            sscanf(dummy2,"%x",&gg);
            sscanf(dummy3,"%x",&bb);
            if (c_read==0)
                cols[i]=Qt::white;
            else
                cols[i]=QColor(rr ,gg ,bb,c_read);
        }
        for (int i=0;i<c_width;i++)
        {
            for (int j=0;j<c_height;j++)
            {
                for (int k=0;k<nr_cols;k++)
                {
                    if (ctrl_char[k]==active_xpm[1+nr_cols+j][i])
                    {
                        img1.setPixel(i,j,cols[k].rgb());
                        break;
                    }
                }
            }
        }
        *ActiveIcon=QPixmap::fromImage(img1,Qt::AutoColor);
        delete[] cols;
        delete[] ctrl_char;
        c_read=sscanf(hidden_xpm[0],"%d %d %d %d",&c_width,&c_height,&nr_cols,&bpc);
        img1=QImage(c_width,c_height,QImage::Format_ARGB32_Premultiplied);
        cols=new QColor[nr_cols];
        ctrl_char=new char[nr_cols];
        for (int i=0;i<nr_cols;i++)
        {
            c_read=sscanf(hidden_xpm[1+i],"%c c #%s",ctrl_char+i,dummy);
            if (c_read<2)
                c_read=0;
            else
                c_read=255;
            dummy1[0]=dummy2[0]=dummy3[0]='0';
            dummy1[1]=dummy2[1]=dummy3[1]='x';
            for (int j=0;j<2;j++)
            {
                dummy1[2+j]=dummy[j];
                dummy2[2+j]=dummy[j+2];
                dummy3[2+j]=dummy[j+4];
            }
            dummy1[2+2]='\0';
            dummy2[2+2]='\0';
            dummy3[2+2]='\0';
            sscanf(dummy1,"%x",&rr);
            sscanf(dummy2,"%x",&gg);
            sscanf(dummy3,"%x",&bb);
            if (c_read==0)
                cols[i]=Qt::white;
            else
                cols[i]=QColor(rr ,gg ,bb,c_read);
        }
        for (int i=0;i<c_width;i++)
        {
            for (int j=0;j<c_height;j++)
            {
                for (int k=0;k<nr_cols;k++)
                {
                    if (ctrl_char[k]==hidden_xpm[1+nr_cols+j][i])
                    {
                        img1.setPixel(i,j,cols[k].rgb());
                        break;
                    }
                }
            }
        }
*HiddenIcon=QPixmap::fromImage(img1,Qt::AutoColor);
delete[] cols;
delete[] ctrl_char;

        //GraceIcon->save(QString("GraceIcon.png"),"PNG",100);

        //Read justification bits
        Qt_j_lm_o_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
        //*Qt_j_lm_o_bits=generate_Bitmap_from_Bits(j_lm_o_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
        generate_Pixmap_from_Bits(j_lm_o_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,toolBarSizeFactor,Qt_j_lm_o_bits);
        Qt_j_cm_o_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
        //*Qt_j_cm_o_bits=generate_Bitmap_from_Bits(j_cm_o_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
        generate_Pixmap_from_Bits(j_cm_o_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,toolBarSizeFactor,Qt_j_cm_o_bits);
        Qt_j_rm_o_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
        //*Qt_j_rm_o_bits=generate_Bitmap_from_Bits(j_rm_o_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
        generate_Pixmap_from_Bits(j_rm_o_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,toolBarSizeFactor,Qt_j_rm_o_bits);
        Qt_j_lb_b_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
        //*Qt_j_lb_b_bits=generate_Bitmap_from_Bits(j_lb_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
        generate_Pixmap_from_Bits(j_lb_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,toolBarSizeFactor,Qt_j_lb_b_bits);
        Qt_j_lm_b_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
        //*Qt_j_lm_b_bits=generate_Bitmap_from_Bits(j_lm_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
        generate_Pixmap_from_Bits(j_lm_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,toolBarSizeFactor,Qt_j_lm_b_bits);
        Qt_j_lt_b_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
        //*Qt_j_lt_b_bits=generate_Bitmap_from_Bits(j_lt_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
        generate_Pixmap_from_Bits(j_lt_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,toolBarSizeFactor,Qt_j_lt_b_bits);
        Qt_j_ct_b_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
        //*Qt_j_ct_b_bits=generate_Bitmap_from_Bits(j_ct_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
        generate_Pixmap_from_Bits(j_ct_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,toolBarSizeFactor,Qt_j_ct_b_bits);
        Qt_j_rt_b_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
        //*Qt_j_rt_b_bits=generate_Bitmap_from_Bits(j_rt_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
        generate_Pixmap_from_Bits(j_rt_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,toolBarSizeFactor,Qt_j_rt_b_bits);
        Qt_j_rm_b_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
        //*Qt_j_rm_b_bits=generate_Bitmap_from_Bits(j_rm_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
        generate_Pixmap_from_Bits(j_rm_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,toolBarSizeFactor,Qt_j_rm_b_bits);
        Qt_j_rb_b_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
        //*Qt_j_rb_b_bits=generate_Bitmap_from_Bits(j_rb_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
        generate_Pixmap_from_Bits(j_rb_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,toolBarSizeFactor,Qt_j_rb_b_bits);
        Qt_j_cb_b_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
        //*Qt_j_cb_b_bits=generate_Bitmap_from_Bits(j_cb_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
        generate_Pixmap_from_Bits(j_cb_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,toolBarSizeFactor,Qt_j_cb_b_bits);
        Qt_j_cm_b_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
        //*Qt_j_cm_b_bits=generate_Bitmap_from_Bits(j_cm_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
        generate_Pixmap_from_Bits(j_cm_b_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,toolBarSizeFactor,Qt_j_cm_b_bits);

        //Read sub-justification bits (for multiline-string-justification)
        Qt_j_left_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
        //*Qt_j_left_bits=generate_Bitmap_from_Bits(j_left_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
        generate_Pixmap_from_Bits(j_left_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,toolBarSizeFactor,Qt_j_left_bits);
        Qt_j_right_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
        //*Qt_j_right_bits=generate_Bitmap_from_Bits(j_right_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
        generate_Pixmap_from_Bits(j_right_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,toolBarSizeFactor,Qt_j_right_bits);
        Qt_j_center_bits=new QPixmap(JBITMAP_WIDTH,JBITMAP_HEIGHT);
        //*Qt_j_center_bits=generate_Bitmap_from_Bits(j_center_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT);
        generate_Pixmap_from_Bits(j_center_bits,5*12+9,JBITMAP_WIDTH,JBITMAP_HEIGHT,toolBarSizeFactor,Qt_j_center_bits);


        //Orientation
        QPixmap * tmpOrientation;
        QPainter tmpPainter;
        int border=2;//4
        tmpOrientation=new QPixmap(MBITMAP_WIDTH*toolBarSizeFactor,MBITMAP_HEIGHT*toolBarSizeFactor);

        generate_Pixmap_from_Bits(m_hv_lr_bt_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,toolBarSizeFactor,tmpOrientation);
        Qt_m_hv_lr_bt_bits=new QPixmap(MBITMAP_WIDTH*toolBarSizeFactor+2*border,MBITMAP_HEIGHT*toolBarSizeFactor+2*border);
        Qt_m_hv_lr_bt_bits->fill();
        tmpPainter.begin(Qt_m_hv_lr_bt_bits);
        tmpPainter.drawPixmap(border,border,MBITMAP_WIDTH*toolBarSizeFactor,MBITMAP_HEIGHT*toolBarSizeFactor,*tmpOrientation);
        tmpPainter.end();
        generate_Pixmap_from_Bits(m_hv_lr_tb_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,toolBarSizeFactor,tmpOrientation);
        Qt_m_hv_lr_tb_bits=new QPixmap(MBITMAP_WIDTH*toolBarSizeFactor+2*border,MBITMAP_HEIGHT*toolBarSizeFactor+2*border);
        Qt_m_hv_lr_tb_bits->fill();
        tmpPainter.begin(Qt_m_hv_lr_tb_bits);
        tmpPainter.drawPixmap(border,border,MBITMAP_WIDTH*toolBarSizeFactor,MBITMAP_HEIGHT*toolBarSizeFactor,*tmpOrientation);
        tmpPainter.end();
        generate_Pixmap_from_Bits(m_hv_rl_bt_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,toolBarSizeFactor,tmpOrientation);
        Qt_m_hv_rl_bt_bits=new QPixmap(MBITMAP_WIDTH*toolBarSizeFactor+2*border,MBITMAP_HEIGHT*toolBarSizeFactor+2*border);
        Qt_m_hv_rl_bt_bits->fill();
        tmpPainter.begin(Qt_m_hv_rl_bt_bits);
        tmpPainter.drawPixmap(border,border,MBITMAP_WIDTH*toolBarSizeFactor,MBITMAP_HEIGHT*toolBarSizeFactor,*tmpOrientation);
        tmpPainter.end();
        generate_Pixmap_from_Bits(m_hv_rl_tb_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,toolBarSizeFactor,tmpOrientation);
        Qt_m_hv_rl_tb_bits=new QPixmap(MBITMAP_WIDTH*toolBarSizeFactor+2*border,MBITMAP_HEIGHT*toolBarSizeFactor+2*border);
        Qt_m_hv_rl_tb_bits->fill();
        tmpPainter.begin(Qt_m_hv_rl_tb_bits);
        tmpPainter.drawPixmap(border,border,MBITMAP_WIDTH*toolBarSizeFactor,MBITMAP_HEIGHT*toolBarSizeFactor,*tmpOrientation);
        tmpPainter.end();
        generate_Pixmap_from_Bits(m_vh_lr_bt_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,toolBarSizeFactor,tmpOrientation);
        Qt_m_vh_lr_bt_bits=new QPixmap(MBITMAP_WIDTH*toolBarSizeFactor+2*border,MBITMAP_HEIGHT*toolBarSizeFactor+2*border);
        Qt_m_vh_lr_bt_bits->fill();
        tmpPainter.begin(Qt_m_vh_lr_bt_bits);
        tmpPainter.drawPixmap(border,border,MBITMAP_WIDTH*toolBarSizeFactor,MBITMAP_HEIGHT*toolBarSizeFactor,*tmpOrientation);
        tmpPainter.end();
        generate_Pixmap_from_Bits(m_vh_lr_tb_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,toolBarSizeFactor,tmpOrientation);
        Qt_m_vh_lr_tb_bits=new QPixmap(MBITMAP_WIDTH*toolBarSizeFactor+2*border,MBITMAP_HEIGHT*toolBarSizeFactor+2*border);
        Qt_m_vh_lr_tb_bits->fill();
        tmpPainter.begin(Qt_m_vh_lr_tb_bits);
        tmpPainter.drawPixmap(border,border,MBITMAP_WIDTH*toolBarSizeFactor,MBITMAP_HEIGHT*toolBarSizeFactor,*tmpOrientation);
        tmpPainter.end();
        generate_Pixmap_from_Bits(m_vh_rl_bt_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,toolBarSizeFactor,tmpOrientation);
        Qt_m_vh_rl_bt_bits=new QPixmap(MBITMAP_WIDTH*toolBarSizeFactor+2*border,MBITMAP_HEIGHT*toolBarSizeFactor+2*border);
        Qt_m_vh_rl_bt_bits->fill();
        tmpPainter.begin(Qt_m_vh_rl_bt_bits);
        tmpPainter.drawPixmap(border,border,MBITMAP_WIDTH*toolBarSizeFactor,MBITMAP_HEIGHT*toolBarSizeFactor,*tmpOrientation);
        tmpPainter.end();
        generate_Pixmap_from_Bits(m_vh_rl_tb_bits,5*12,MBITMAP_WIDTH,MBITMAP_HEIGHT,toolBarSizeFactor,tmpOrientation);
        Qt_m_vh_rl_tb_bits=new QPixmap(MBITMAP_WIDTH*toolBarSizeFactor+2*border,MBITMAP_HEIGHT*toolBarSizeFactor+2*border);
        Qt_m_vh_rl_tb_bits->fill();
        tmpPainter.begin(Qt_m_vh_rl_tb_bits);
        tmpPainter.drawPixmap(border,border,MBITMAP_WIDTH*toolBarSizeFactor,MBITMAP_HEIGHT*toolBarSizeFactor,*tmpOrientation);
        tmpPainter.end();
        delete tmpOrientation;

        Qt_justifications[0]=Qt_j_lm_o_bits;
        Qt_justifications[1]=Qt_j_cm_o_bits;
        Qt_justifications[2]=Qt_j_rm_o_bits;
        Qt_justifications[3]=Qt_j_lb_b_bits;
        Qt_justifications[4]=Qt_j_cb_b_bits;
        Qt_justifications[5]=Qt_j_rb_b_bits;
        Qt_justifications[6]=Qt_j_lm_b_bits;
        Qt_justifications[7]=Qt_j_cm_b_bits;
        Qt_justifications[8]=Qt_j_rm_b_bits;
        Qt_justifications[9]=Qt_j_lt_b_bits;
        Qt_justifications[10]=Qt_j_ct_b_bits;
        Qt_justifications[11]=Qt_j_rt_b_bits;

        Qt_sub_justifications[0]=Qt_j_left_bits;
        Qt_sub_justifications[1]=Qt_j_center_bits;
        Qt_sub_justifications[2]=Qt_j_right_bits;

        Qt_matrixOrder[0]=Qt_m_hv_lr_tb_bits;
        Qt_matrixOrder[1]=Qt_m_hv_lr_bt_bits;
        Qt_matrixOrder[2]=Qt_m_hv_rl_tb_bits;
        Qt_matrixOrder[3]=Qt_m_hv_rl_bt_bits;
        Qt_matrixOrder[4]=Qt_m_vh_lr_tb_bits;
        Qt_matrixOrder[5]=Qt_m_vh_lr_bt_bits;
        Qt_matrixOrder[6]=Qt_m_vh_rl_tb_bits;
        Qt_matrixOrder[7]=Qt_m_vh_rl_bt_bits;

}
