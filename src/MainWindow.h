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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <cstring>
#include <fstream>

#include <QtGui>
//#include <QtNetwork>
#include <QGestureEvent>

#include "Server.h"
#include "graphs.h"
#include "draw.h"
#include "utils.h"
#include "files.h"
#include "xprotos.h"
#include "events.h"
#include "fundamentals.h"

using namespace std;

extern void (*IO_function)(int,QString,bool,bool,bool);

class myScrollArea:public QScrollArea
{
    Q_OBJECT
public:
    myScrollArea(QWidget * parent=0);
double scaleFactor,startScaleFactor;
QElapsedTimer tim1;//,tim2;
QTimer ping_timer;
QTimer event_timer;
QPoint hotSpot;
VPoint realVPos;
WPoint realWPos;
world orig_world;
int eventType,ignore_event_timer;
VPoint EventLastPoint;
special_XEvent completeEvent;
public slots:
virtual bool event(QEvent *event);
virtual bool gestureEvent(QGestureEvent *event);
virtual void keyPressEvent( QKeyEvent * e );
virtual void wheelEvent(QWheelEvent * e);
void panTriggered(QPanGesture *gesture);
void pinchTriggered(QPinchGesture *gesture);
void swipeTriggered(QSwipeGesture *gesture);
void grabGestures(const QList<Qt::GestureType> &gestures);
void timerStopEvent(void);
void eventTimerEnvent(void);
void startEvent(int type,special_XEvent event,VPoint vp);
void haltEvent(void);
void doEvent(void);
};

class MainArea : public QWidget
{
    Q_OBJECT
public:
//QFrame * drawArea;
    QLabel * lblBackGr;
    int useable_w,useable_h;
    int box_start_x,box_start_y;
    int box_end_x,box_end_y;
    bool draw_box;
    int contentChanged;
    bool compl_redraw_running;

    QPoint origin;
    QRubberBand * rubber;
    QRubberBand * rubberLine;
    myScrollArea * scroll;
    QHBoxLayout * layout;
    InLineEditor * inlineedit;
    InLineEditor2 * inlineedit2;
    SetPopup * inlineSetPopup;

    MainArea(QWidget *parent=0);
    virtual void mouseMoveEvent( QMouseEvent * e);
    virtual void mousePressEvent(QMouseEvent * e);
    virtual void mouseReleaseEvent(QMouseEvent * e );
    virtual void mouseDoubleClickEvent(QMouseEvent * e );
    virtual void keyPressEvent( QKeyEvent * e );
    virtual void keyReleaseEvent( QKeyEvent * e );
    virtual void wheelEvent( QWheelEvent * e );
    //virtual void QDragEnterEvent(const QPoint &pos, Qt::DropActions actions, const QMimeData *data, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
    //virtual void QDropEvent(const QPoint &pos, Qt::DropActions actions, const QMimeData *data, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, QEvent::Type type);
    void processClickCommand(special_XEvent & event);
    void completeRedraw(void);
    void paintEvent( QPaintEvent *e );
    void transf_window_coords(int x,int y,int & real_x,int & real_y);
    void setBGtoColor(QColor col);
    void repositionRubber(QRect rect);
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
};

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    QString future_load_file;
    int future_load_status;//this is needed to be able to break the bailout-procedure
    //0=nothing special
    //1=we want to load, but we have to Save before that
    //2=we want to load, but we have to SaveAs before that
    //3=save and close after that

    enum Qt::CursorShape current_cursor;
    int windowWidth,windowHeight;
    int stdBarHeight,stdRowHeight,stdColWidth;
    int stdDistance1,stdHeight1,stdHeight2;
    int stdDistance2,stdWidth2;

    int defaultSmallButtonW,defaultSmallButtonH;
    int defaultLargeButtonW,defaultLargeButtonH;
    int defaultListW,defaultListH;
    int defaultSliderW,defaultSliderH;
    int defaultToolBarWidth;
    int defaultBorderGap,defaultVGap;
    int default_IconW,default_IconH;

    int nr_of_Example_Menues;
    int nr_of_Examples;
    QString ExampleMenuNames[9];
    QString ExampleMenuEntries[9][11];
    int nr_of_Example_Menu_Entries[9];
    QMenu * example_menues[9];
    QAction * act_examples[9][11];
#if QT_VERSION >= 0x050000

#else
    QSignalMapper * helpMapper;
    QSignalMapper * historyMapper;
#endif
    QString examplesFiles[42];

    QMenuBar * menuBar;
    QStatusBar * statusBar;
    QLabel * statusBarLabel;
    QLabel * statLocBar;
    QWidget * tool_empty,*tool_empty2;
    QVBoxLayout * tool_layout;
    QScrollArea * scroll;
    QFrame * toolBar1,*toolBar2;

    QGridLayout * mainGrid;
    //QGridLayout * tool1Grid;
    //QGridLayout * tool2Grid;

    QMenu * mnuFile;
    QMenu * mnuEdit;
    QMenu * mnuRegions;
    QMenu * mnuData;
    QMenu * mnuTransform;
    QMenu * mnuImport;
    QMenu * mnuExport;
    QMenu * mnuPlot;
    QMenu * mnuView;
    QMenu * mnuWindow;
    QMenu * mnuHelp;
    QMenu * mnuExample;

    QPushButton * cmdDraw;
    QPushButton * cmdZoom,*cmdAutoScale;
    QPushButton * cmdZz,*cmdzz;
    QPushButton * cmdLeft,*cmdRight;
    QPushButton * cmdUp,*cmdDown;
    QPushButton * cmdAutoT,*cmdAutoO;
    QPushButton * cmdZX,*cmdZY;
    QPushButton * cmdAX,*cmdAY;
    QPushButton * cmdPZ,*cmdPu;
    QPushButton * cmdPo,*cmdCy;
    QPushButton * cmdUndo,*cmdRedo;
    QPushButton * cmdExport,*cmdPrint,*cmdPan,*cmdPick;

#ifdef AUTOLAYOUT_FOR_TOOLBAR
    QGridLayout * toolLayout1, * toolLayout2;
#endif

    QLabel * lblNewViewStack;
    QComboBox * cmbViewStack;
    QPushButton * cmdViewUp, * cmdViewDown, * cmdViewReplace, * cmdViewRearrange;
    QPushButton * cmdViewRename, * cmdViewAdd, * cmdViewRemove;

    uniList * lstGraphs;
    stdSlider * sldPageZoom;
    QPushButton * cmdFitPage;
    QLabel * lblSD;
    QLabel * lblCW;
    QPushButton * cmdExit;

    QAction * actNew,*actOpen,*actSave,*actSaveAs,*actRevert,*actPrint,*actPrintToFile,*actPrintSetup,*actExit;
    //SpinBoxAction * actPrintSpecial;
    QAction *actDataSets,*actSetOperations,*actArrangeGraphs,*actOverlayGraphs,*actAutoscaleGraphs,*actRegionMaster,*actRegionsStatus,*actRegionsDefine,*actRegionsClear,*actRegionsReportOn,*actHotLinks,*actSetLocFixPoint,*actClearLocFixPoint,*actLocProp,*actPreferences;
    QAction*actDataSetOperations,*actFeatureExtraction,*actExportAscii,*actImportNetCDF,*actImportAscii,*actEvaluateExpr,*actHistograms,*actFourier,*actFourier2,*actRunningAverages,*actDifferences,*actSeasonalDiff,*actIntegration,*actInterpolation,*actRegression,*actNonLinCurveFitting,*act2DFitting,*actReportFitParameters,*actCorrelation,*actDigitalFilter,*actLinConvolution,*actGeomTransform,*actGeomEval,*actSamplePoints,*actPruneData;
    QAction *actPlotAppearance,*actGraphAppearance,*actSetAppearance,*actAxisProperties,*actLoadParameters,*actSaveParameters;
    QAction *actCommands,*actPointExplorer,*actDrawingObjects,*actFontTool,*actConsole;
    QAction *actHelpOnContext,*actHelpUsersGuide,*actHelpTutorial,*actHelpFAQ,*actHelpChanges,*actHelpQtGrace,*actHelpComments,*actHelpLicense,*actHelpReadme,*actHelpAbout;
    QAction *actShowLocBar,*actShowStatusBar,*actShowToolBar,*actPageSetup,*actRedraw,*actUpdateAll;
    QAction *actImportBinary,*actExportBinary,*actImportCSV,*actImportAgr;
    QAction *actUndo,*actRedo,*actUndoList,*actExplorer,*actColManager,*actRealTimeInput;


    QMenu * mnuHistory;
    QAction *actHistory[MAX_HISTORY],*actClearHistory;

    QTimer * rtiTimer;//for monitoring real time input
    QTimer * highlightTimer;//for simple-drawing of highlighted lines
    QTimer * stopHighlightTimer;//for maximum time to stop highlighting
    QtGraceTcpServer *SocketConnection;

    MainArea * mainArea;

    MainWindow( QWidget *parent=0 );
    ~MainWindow();
    void deleteIcons(void);

    virtual void showEvent( QShowEvent * e );
    virtual void resizeEvent( QResizeEvent * e);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void keyPressEvent( QKeyEvent * e );
public slots:
    //Menu Actions
    void newFile(void);
    void Open(void);
    void Save(void);
    void SaveAs(void);
    void RevertToSaved(void);
    void Print(void);
    void PrintSpecial(void);
    void PrintToFile(void);
    void PrintSetup(void);
    void Exit(void);
    void DataSets(void);
    void Explorer(void);
    void ColorManager(void);
    void RealTimeInputDisplay(void);
    void SetOperations(void);
    void ArrangeGraphs(void);
    void OverlayGraphs(void);
    void AutoscaleGraphs(void);
    void HotLinks(void);
    void SetLocFixPoint(void);
    void ClearLocFixPoint(void);
    void LocProp(void);
    void Preferences(void);
    void RegionsMaster(void);
    void RegionsStatus(void);
    void RegionsDefine(void);
    void RegionsClear(void);
    void RegionsReportOn(void);
    void DataSetOperations(void);
    void FeatureExtraction(void);
    void ImportAscii(void);
    void ImportCSV(void);
    void ImportBinary(void);
    void ImportNetCDF(void);
    void ImportAgr(void);
    void ExportAscii(void);
    void ExportBinary(void);
    void EvaluateExpr(void);
    void Histograms(void);
    void Fourier(void);
    void Fourier2(void);
    void RunningAverages(void);
    void Differences(void);
    void SeasonalDiff(void);
    void Integration(void);
    void Interpolation(void);
    void Regression(void);
    void NonLinCurveFitting(void);
    void do2DFitting(void);
    void ReportOnFitParameters(void);
    void Correlation(void);
    void DigitalFilter(void);
    void LinConvolution(void);
    void GeomTransform(void);
    void GeomEval(void);
    void SamplePoints(void);
    void PruneData(void);
    void PlotAppearance(void);
    void GraphAppearance(void);
    void SetAppearance(void);
    void AxisProperties(void);
    void LoadParameters(void);
    void SaveParameters(void);
    void ShowLocBar(void);
    void ShowStatusBar(void);
    void ShowToolBar(void);
    void resizeAllIcons(void);
    void ManageBars(void);
    void PageSetup(void);
    void Redraw(void);
    void UpdateAll(void);
    void Commands(void);
    void PointExplorer(void);
    void DrawingObjects(void);
    void FontTool(void);
    void Console(void);
    void HelpOnContext(void);
    void HelpUsersGuide(void);
    void HelpTutorial(void);
    void HelpFAQ(void);
    void HelpChanges(void);
    void HelpQtGrace(void);
    void HelpComments(void);
    void HelpLicense(void);
    void HelpReadme(void);
    void HelpAbout(void);
    void HelpOpenExample(int i);

    //History-Actions
    void addToHistory(char * entry);
    void HistoryClicked(int i);
    void clearHistory(void);
    void recreateHistory(void);

    //Undo-Actions
    void doUndo(void);
    void doRedo(void);
    void doUndoList(void);

    //Button Actions
    void doDraw(void);
    void doZoom(void);
    void doAutoScale(void);
    void doZz(void);
    void dozz(void);
    void doLeft(void);
    void doRight(void);
    void doUp(void);
    void doDown(void);
    void doPan(void);
    void doPick(void);
    void doAutoT(void);
    void doAutoO(void);
    void doZX(void);
    void doZY(void);
    void doAX(void);
    void doAY(void);
    void doPZ(void);
    void doPu(void);
    void doPo(void);
    void doCy(void);
    void doPageZoom(int i);
    void doFitPage(void);
    void doExit(void);

    //Initializations
    void CreateActions(void);
    void CreateIcons(void);
    void redisplayIcons(void);

    //Functions
    void LoadProject(char * filename);
    virtual void closeEvent( QCloseEvent * event );
    void finalBailout(void);
    void set_barebones(int onoff);

    void getselectedgraphs(int * nr_of_graphs,int**graph_nrs);
    void autoscale_proc(int data);
    void autoticks_proc(void);
    void graph_scroll_proc(int data);
    void graph_zoom_proc(int data);
    void world_stack_proc(int data);
    void load_example(char *data);
    void set_stack_message(void);
    void newgraphselection(int gr_nr);

    void UseOperatingSystemFileDialog(int type,QString title,QString file,QString extension);
    void IOrequested(int type,QString file,bool exists,bool writeable,bool readable);
    void SpreadSheetClosed(int gno,int setno);

    void checkForRealTimeIO(void);//looks whether real-time-io (pipes) have to be used and activates or deactivates a timer accordingly
    void doRealTimeMonitoring(void);//looks for new data from real time input

    //Highlighting of Graphs and Sets (before doing this, the segments to be highlighted should be initialized)
    void start_highlighting(void);
    void stop_highlighting(void);
    void highlight_ping(void);
    void highlight_stop_ping(void);
    void showWizard(void);

    //new viewport stack
    void UpdateViewportList(void);
    void newViewportUp(void);
    void newViewportDown(void);
    void newViewportRename(void);
    void newViewportSelected(int c);
    void newViewportAdd(void);
    void newViewportRemove(void);
    void newViewportReposition(void);
    void newViewportReplace(void);
};

#endif

