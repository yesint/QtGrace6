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

#ifndef ALLWIDGETS_H
#define ALLWIDGETS_H

#include <QtGui>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cfloat>
#include "draw.h"
#include "graphs.h"
#include "x11drv.h"
#include "t1fonts.h"
#include "fundamentals.h"
#include "compressdecompress.h"
#include "globals.h"
#include "parser.h"
#include "ssdata.h"
#include "defines.h"
#include "t1fonts.h"

#define nr_of_translations 2

#define SETAPP_STRIP_LEGENDS    0
#define SETAPP_LOAD_COMMENTS    1
#define SETAPP_ALL_COLORS       2
#define SETAPP_ALL_SYMBOLS      3
#define SETAPP_ALL_LINEW        4
#define SETAPP_ALL_LINES        5
#define SETAPP_ALL_BW           6

#define CSYNC_LINE      0
#define CSYNC_SYM       1

#define READ_SET_FORM           0
#define READ_NETCDF_FORM        1
#define READ_PROJECT_FORM       2
#define WRITE_SET_FORM          3
#define WRITE_PROJECT_FORM      4
#define SELECT_HOT_LINK_FILE    5
#define READ_PARAMETERS         6
#define WRITE_PARAMETERS        7
#define SELECT_PRINT_FILE       8
#define WRITE_FIT_PARAM         9
#define READ_FIT_PARAM          10
#define READ_BINARY_FILE        11
#define WRITE_BINARY_FILE       12
#define READ_COMMANDS_FILE      13
#define WRITE_COMMANDS_FILE     14
#define NR_OF_FILE_DIALOGS      15

#define INTERPOLATIONWINDOW     0
#define HISTOGRAMSWINDOW        1

#define DEFINEREGIONWINDOW      0
#define REPORTREGIONWINDOW      1
#define CLEARREGIONWINDOW       2

#define INTERPOLATIONWINDOW     0
#define HISTOGRAMSWINDOW        1

#define INTEGRATIONWINDOW       0
#define SEASONALWINDOW          1
#define DIFFERENCESWINDOW       2
#define AVERAGESWINDOW          3
#define REGRESSIONWINDOW        4

#define CORRELATIONWINDOW       0
#define CONVOLUTIONWINDOW       1
#define FILTERWINDOW            2

#define PRUNEWINDOW             0
#define SAMPLEPOINTSWINDOW      1
#define GEOMETRICWINDOW         2

#define SPECIAL_NONE            0
#define SPECIAL_FILTER          1
#define SPECIAL_REGRESSION      2
#define SPECIAL_REMEMBER        3
#define SPECIAL_ADD             4
#define SPECIAL_MINUS           5
#define SPECIAL_DIVIDE          6
#define SPECIAL_MULTIPLY        7
#define SPECIAL_USE             8
#define SPECIAL_EXTRACT         9
#define SPECIAL_FORMULA         10
#define SPECIAL_APPEND          11
#define SPECIAL_FIT2D           12
#define SPECIAL_IF              13

#define NR_OF_QUICKSELECTOPTIONS 26
#define NR_OF_USER_DEFAULT_DEVICE_GEOMETRIES 3

#define CONNECT_ERRBARS_NONE 0
#define CONNECT_ERRBARS_XY 1
#define CONNECT_ERRBARS_Y_ONLY 2
#define CONNECT_ERRBARS_X_ONLY 3
#define CONNECT_ERRBARS_FILL_Y 4
#define CONNECT_ERRBARS_FILL_X 5
#define CONNECT_ERRBARS_FILL_XY 6

#define INI_PREPROCESSING_AFTER_FIRST 0
#define INI_PREPROCESSING_AFTER_LAST 1
#define INI_PREPROCESSING_BEFORE_FIRST 2
#define INI_PREPROCESSING_BEFORE_LAST 3
#define INI_PREPROCESSING_BETWEEN_FIRST 4
#define INI_PREPROCESSING_BETWEEN_LAST 5

using namespace std;

class frmEditBlockData;
class MainWindow;
class frmIOForm;
class TestDialog;

class grpSelect:public QGroupBox
{
    Q_OBJECT
public:
    grpSelect(QString title,QWidget *parent=0);

    QString titleString;

    ///QGroupBox * fraTitle;
    QLabel * lblGraph;
    uniList * listGraph;
    QLabel * lblSet;
    uniList * listSet;
    QCheckBox * chkSyncSelection;
    uniList * syncSource;

    QGridLayout * layout;
public slots:
    void enable_sync(uniList * sync_partner);
    void syncToggled(bool s);
    void update_number_of_entries(void);
    void mark_single_set(int gno,int setno);
    void mark_multiple_sets(int gno,int nsets,int * setnos);
    void set_graph_nr(int gno);
    void source_got_new_selection(int a);
};

class frmEditColumnProp:public QDialog
{
    Q_OBJECT
public:
    frmEditColumnProp(QWidget * parent=0);
    ~frmEditColumnProp();
    int col_format[MAX_SET_COLS];
    int col_precision[MAX_SET_COLS];
    StdSelector * selColumn;
    StdSelector * selFormat;
    stdIntSelector * selPrecision;

    stdButtonGroup * buttonGroup;
    QVBoxLayout * layout;
public slots:
    void doApply(void);
    void doAccept(void);
    void doClose(void);
    void initToColumn(int i);
signals:
    void newColumnFormat(int col,int format,int precision);
};

class frmSpreadSheet:public QDialog
{
    Q_OBJECT
public:
    frmSpreadSheet(QWidget * parent=0,int gno=-1,int sno=-1);
    ~frmSpreadSheet();
    frmEditColumnProp * EditColumn;
    int col_format[MAX_SET_COLS];
    int col_precision[MAX_SET_COLS];
    int gno,sno;
    QMenuBar * menuBar;
    QMenu * mnuFile;
    QMenu * mnuEdit;
    QMenu * mnuHelp;
    QAction * actClose,*actAddRow,*actDelSelRow,*actColumnFormat,*actHelpOnContext,*actHelpOnDataSetEditor;
    QAction * actSelectEven,*actSelectOdd,*actCopySelected;
    QGroupBox * fraDataset;
    StdSelector * selType;
    stdLineEdit * ledComment;
    QCheckBox * chkShowStrings;
    QCheckBox * chkShowStringOperations;
    StdSelector * selConvertTo;
    StdSelector * selFormat;
    StdSelector * selDecSep;
    LineWidthSelector * selInvalidVal;
    QPushButton * cmdConvertStrings;
    QPushButton * cmdRemoveStrings;
    QGridLayout * layout0;
    stdButtonGroup * buttonGroup;
    QVBoxLayout * layout;
    //SetTableView * table;
    QTableView * table;
    SetTableModel * model;
    uniList * parentList;
public slots:
    void CreateActions(void);
    void HHeaderClicked(int i);
    void init(int g_no,int s_no);
    void setColumnFormat(int col,int format,int precision);
    void changeDataSet(int type);
    void itemChanged(int r,int c,bool realy_new_value);
    void minimalItemChanged(void);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
    void doAddRow(void);
    void doDelSelRow(void);
    void doColumnFormat(void);
    void doHelpOnContext(void);
    void doHelpOnDataSetEditor(void);
    void getSelection(int * n_of_sel,int ** sel);
    void setSelection(int n_of_sel,int * sel);
    void update_entries(void);
    void doSelectEven(void);
    void doSelectOdd(void);
    void doCopySelected(void);
    void showStringsToggled(bool a);
    void showStringsOperationsToggled(bool a);
    void doConvertStrings(void);
    void doRemoveStrings(void);
    void setPossibleColumns(void);
    void stringDataChanged(void);
virtual void keyPressEvent(QKeyEvent * e);
};

class frmSpreadSheet2:public QDialog//NOT USED --> frmSpreadSheet has been altered to do this
{
    Q_OBJECT
public:
    frmSpreadSheet2(QWidget * parent=0);

    int allocated_rows,start_row;
    int visible_row_count;
    int number_of_rows,number_of_columns;
    bool initiated;
    int g_no,s_no,length;

    stdSetTypeSelector *selType;
    QGroupBox * fraDataset;
    stdLineEdit * ledComment;
    QVBoxLayout * layout0;

    QTableWidget * table;
    QTableWidgetItem *** Items;//the Entries!
    QScrollBar * scroll;
    stdButtonGroup * buttonGroup;
    QGridLayout * layout;
public slots:
    void resizeEvent(QResizeEvent * event);
    void init(int gno,int sno);
    void position_changed(int pos);
    void update_entries(void);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
};

//the physically allocated lines in the spreadsheet
#define MAX_NUMBER_OF_LINES_IN_SPREADSHEET 100
//the number of empty lines that are displayed below the set entries (for the user to fill additional data in)
#define EXCESS_LINES_IN_SPREADSHEET 5

class myTableWidget:public QTableWidget
{
    Q_OBJECT
public:
    myTableWidget(int r, int c,QWidget * parent=0);
public slots:
    void wheelEvent(QWheelEvent * e);
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);
signals:
    void wheeled(int d);
    void newDirection(int keynr);
};

class frmSpreadSheet3:public QDialog
{
    Q_OBJECT
public:
    frmSpreadSheet3(QWidget * parent=0);

    int gno,setno,inited;//information about the current set, that is displayed and whether it has been initeed or not
    bool working;//currently processing data (or not)

    frmEditColumnProp * EditColumn;

    int col_format[6];
    int col_precision[6];

    int start_row;//the number of the first row to be displayed
    int setLength;//the number of entries in the current set
    int number_of_visible_entries;//the guessed number of rows that are visible at the same time in the current SpreadSheetSize
    //int number_of_rows,filled_rows;//filled rows are the rows that have actual numbers in it --> we do not want to use this any longer
    int number_of_columns;//number of columns used in the current set
    int nextRow,nextCol,nextScroll;//for navigation

    //QTableWidget * table;
    myTableWidget * table;//special TableWidget with WheelActions enabled
    QTableWidgetItem *** Items;//the Entries!
    QScrollBar * scroll;//Vertical Scroll Bar

    QMenuBar * menuBar;
    QMenu * mnuFile;
    QMenu * mnuEdit;
    QMenu * mnuHelp;

    QAction * actClose,*actAddRow,*actDelSelRow,*actColumnFormat,*actHelpOnContext,*actHelpOnDataSetEditor;

    QGroupBox * fraDataset;
    StdSelector * selType;//select current set-Type
    stdLineEdit * ledComment;
    QVBoxLayout * layout0;

    stdButtonGroup * buttonGroup;

    QGridLayout * layout;
public slots:
    void resizeEvent(QResizeEvent * event);
    void wheel_used(int d);
    void position_changed(int pos);
    void setColumnFormat(int col,int format,int precision);
    void changeDataSet(int type);
    void update_entries(void);
    void HHeaderClicked(int i);
    void init(int g_no,int set_no);
    void CreateActions(void);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
    void doAddRow(void);
    void doDelSelRow(void);
    void doColumnFormat(void);
    void doHelpOnContext(void);
    void doHelpOnDataSetEditor(void);
    //void read_set(int g_no,int s_no);//Display set points --> function replaced by init()
    void write_set(void);//store displayed set points in current graph/set
    void spreadSheetEntryChanged(void);
    void itemChanged(int r,int c);
    void getSelection(int * n_of_sel,int ** sel);
    void updateScrollBar(void);
    void changeDirection(int keynr);
    void currentCellChangedWrapper( int currentRow, int currentColumn, int previousRow, int previousColumn );
    void StepLeft(int curRow,int curCol);
    void StepRight(int curRow,int curCol);
signals:
    void spreadSheetClosed(int gno,int setno);
};


class frmSetOp:public QDialog
{
    Q_OBJECT
public:
    frmSetOp(QWidget * parent=0);
    ~frmSetOp();

    int prev_operation;

    QLabel * lblDataSet;
    uniList * listSets;

    QMenuBar * menuBar;
    QMenu * mnuFile;
    QMenu * mnuHelp;

    StdSelector * selOperation;
    StdSelector * selSortOn;
    StdSelector * selSortOrder;
    StdSelector * selRegion;
    QCheckBox * chkInvert;
    QCheckBox * chkCreateNew;
    QCheckBox * chkAllowInterpolation;

    StdSelector * selSwap1;
    StdSelector * selSwap2;

    stdLineEdit * ledLength;
    stdLineEdit * ledStart;
    stdLineEdit * ledStop;

    QAction *actClose,*actHelpOnContext,*actHelpOnSetOp;

    stdButtonGroup * buttonGroup;
    QGridLayout * layout;
public slots:
    void init(void);
    void changeSelection(int i);
    void allow_interpol_toggled(bool c);
    void CreateActions(void);
    void doClose(void);
    void doAccept(void);
    void doApply(void);
    void doHelpOnContext(void);
    void doHelpOnSetOp(void);
};

class frmDeviceOptions:public QDialog
{
    Q_OBJECT
public:
    frmDeviceOptions(int device,QWidget * parent=0);
    ~frmDeviceOptions();
    int Device;

    QGroupBox * grpPSoptions;
    QCheckBox * ps_setup_grayscale_item;
    QCheckBox * ps_setup_level2_item;
    StdSelector * ps_setup_docdata_item;
    QVBoxLayout * layout0;
    QGroupBox * grpPageOffset;
    stdIntSelector * ps_setup_offset_x_item;
    stdIntSelector * ps_setup_offset_y_item;
    QHBoxLayout * layout1;
    QGroupBox * grpHardware;
    StdSelector * ps_setup_feed_item;
    QCheckBox * ps_setup_hwres_item;
    QVBoxLayout * layout2;

    QGroupBox * grpPNMoptions;
    StdSelector * pnm_setup_format_item;
    QCheckBox * pnm_setup_rawbits_item;
    QVBoxLayout * layout3;

    QGroupBox * grpEPSoptions;
    QCheckBox * eps_setup_grayscale_item;
    QCheckBox * eps_setup_level2_item;
    QCheckBox * eps_setup_tight_bb_item;
    StdSelector * eps_setup_docdata_item;
    QVBoxLayout * layout4;

    QGroupBox * grpJPEGoptions;
    stdIntSelector * selQuality;
    QCheckBox * chkOptimize;
    QCheckBox * chkProgressive;
    QCheckBox * chkJPG_Grayscale;
    QVBoxLayout * layout5;
    QGroupBox * grpJPEGadvoptions;
    stdIntSelector * selSmoothing;
    QCheckBox * chkForceBaseline;
    StdSelector * selDCT;
    QVBoxLayout * layout6;

    QGroupBox * grpPNGoptions;
    QCheckBox * chkInterlaced;
    QCheckBox * chkTransparent;
    stdIntSelector * selCompression;
    QVBoxLayout * layout7;

    QGroupBox * grpBMPoptions;
    stdSlider * sldQuality;
    QCheckBox * chkGrayscale;
    QVBoxLayout * layout8;

    stdButtonGroup * buttonGroup;

    QVBoxLayout * layout;
public slots:
    void init(void);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
};

class frmDeviceActivator:public QDialog
{
    Q_OBJECT
public:
    frmDeviceActivator(QWidget * parent=0);
    ~frmDeviceActivator();
    QGridLayout * layout;
    QPushButton * cmdAll;
    QPushButton * cmdApply;
    QPushButton * cmdClose;
    int alloc_checks;
    QCheckBox ** chkDeviceActive;
public slots:
    void doAll(void);
    void init(void);
    void doApply(void);
    void doClose(void);
};

class frmUserDefaultGeometries:public QDialog
{
    Q_OBJECT
public:
    frmUserDefaultGeometries(QWidget * parent=0);
    ~frmUserDefaultGeometries();
    QGridLayout * layout;
    QLabel * lblDescription;
    QLabel * lblTitles[12];
    QLabel * lblNr[NR_OF_USER_DEFAULT_DEVICE_GEOMETRIES];
    QCheckBox * chkActive1[NR_OF_USER_DEFAULT_DEVICE_GEOMETRIES];
    QLineEdit * lenName[NR_OF_USER_DEFAULT_DEVICE_GEOMETRIES];
    QComboBox * cmbDevice[NR_OF_USER_DEFAULT_DEVICE_GEOMETRIES];
    QComboBox * cmbOrientation[NR_OF_USER_DEFAULT_DEVICE_GEOMETRIES];
    QDoubleSpinBox * spnWidth[NR_OF_USER_DEFAULT_DEVICE_GEOMETRIES];
    QDoubleSpinBox * spnHeight[NR_OF_USER_DEFAULT_DEVICE_GEOMETRIES];
    QComboBox * cmbUnit[NR_OF_USER_DEFAULT_DEVICE_GEOMETRIES];
    QDoubleSpinBox * spnResolution[NR_OF_USER_DEFAULT_DEVICE_GEOMETRIES];
    QComboBox * cmbAntialiasing[NR_OF_USER_DEFAULT_DEVICE_GEOMETRIES];
    QComboBox * cmbLineScaling[NR_OF_USER_DEFAULT_DEVICE_GEOMETRIES];
    QWidget * empty;
    QGridLayout * layout0;
    QLabel * lblActive2;
    QCheckBox * chkActive2[NR_OF_QUICKSELECTOPTIONS];
    stdButtonGroup * buttons;
public slots:
    void init(void);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
    void redisplaySeparators(void);
signals:
    void newUserDefaults(void);
};

class frmDeviceSetup:public QDialog
{
    Q_OBJECT
public:
    frmDeviceSetup(QWidget * parent=0);
    ~frmDeviceSetup();
    QPushButton * device_opts_item;
    QPushButton * wbut;
    QPushButton * cmdUseScreenResolution;
    QGroupBox * grpDevSetup;
    QGroupBox * grpOutput;
    QGroupBox * grpPage;
    QGroupBox * grpFonts;
    QMenu * mnuFile;
    QMenu * mnuOptions;
    QMenu * mnuHelp;
    QMenu * mnuQuickSelect;
    QMenuBar * menuBar;

    StdSelector * devices_item;
    StdSelector * page_orient_item;
    StdSelector * page_format_item;
    stdLineEdit * print_string_item;
    stdLineEdit * printfile_item;
    stdLineEdit * page_x_item;
    stdLineEdit * page_y_item;
    stdLineEdit * dev_res_item;
    QComboBox * page_size_unit_item;
    QCheckBox * chkDontChangeSize;
    QCheckBox * chkScaleLineWidthByResolution;
    QCheckBox * chkUseAntialiasing;
    QCheckBox * chkClipOutput;
    QPushButton * cmdNativePrinterDialog;
    QPushButton * cmdDoPrint;

    //Page_geometry quick_pg[NR_OF_QUICKSELECTOPTIONS];
    StdSelector * quick_resolution_selector;
    QAction * act_quickSel[NR_OF_QUICKSELECTOPTIONS+NR_OF_USER_DEFAULT_DEVICE_GEOMETRIES+2];
#if QT_VERSION >= 0x050000

#else
    QSignalMapper * quickMapper;
#endif
    QCheckBox * printto_item;
    QCheckBox * fontaa_item;
    QCheckBox * devfont_item;

    frmDeviceOptions * DevOptions[7];
    QString cur_FileName;
    int cur_dev,cur_version;
    int parent_of_print_dialog;//0=this,1=MainWindow
    char * out_format;
    char out_format_int[5];
    char out_format_float[5];
    QAction * actPrintToFile,*actClose,*dsync_item,*psync_item,*actHelpOnDevSetup,*actHelpOnContext,*actNativePrinterDialog;
    QPrintDialog * printDialog;
    //ofstream ofi;
    Device_entry curdev,tmp_dev;

    stdButtonGroup * buttonGroup;

    QHBoxLayout * layout0;
    QGridLayout * layout1;
    QGridLayout * layout2;
    QGridLayout * layout3;
    QVBoxLayout * layout;
    int deltaFonts,deltaGeometry;
public slots:
    void init(int dev);
    void CreateActions(void);
    void PrintToFileClicked(int i);
    void DeviceChanged(int i);
    void SizeChanged(int i);
    void OrientationChanged(int i);
    void DimChanged(int i);
    void setLikeDefaultGeometry(UserDeviceGeometry * dg);
    void QuickResolutionChange(int val);
    void changeDeviceList(int version);
    void DpisChanged(void);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
    void doPrintToPrinter(void);
    void doPrintToFile(void);
    void doSyncPage(void);
    void doRescalePlot(void);
    void doHelpOnContext(void);
    void doHelpOnDevSetup(void);
    void doBrowse(void);
    void doDevOpt(void);
    void doUseScreenResolution(void);
    bool openNativePrinter(int dev);
    void printerAccepted(QPrinter * pri);
    void printerRejected(void);
    void dpiInputChanged(QString text);
    void tryPrintingOnPrinter(QPrinter * printer);
    void doNativePrinterDialog(void);
    void replaceFileNameOnly(QString nname);//this assumes that nname is only the name without a path and without a suffix
    void IOrequested(int type,QString file,bool exists,bool writeable,bool readable);
    void fontGroupToggled(bool t);
    void geometryGroupToggled(bool t);
    void doDefaultPageGeometrySetup(void);
    void doReset(void);
    virtual void closeEvent(QCloseEvent * e);
    void recreateQuickMenu(void);
};

class tabLinestyles:public QWidget
{
    Q_OBJECT
public:
    tabLinestyles(QWidget * parent=0);

    StdSelector * selShowLinestyles;
    LineStyleSelector * selStyles;
    QWidget * empty;
    QHBoxLayout * hbox;
    int nr_of_panels;
    Panel ** panels;
    QPixmap *whitepanel,*blackpanel,*pmExample;
    QLabel * lblExample;
    stdIntSelector * selLen;
    stdIntSelector * selPos;
    QGridLayout * layout;

    QPushButton * cmdEdit;
    QPushButton * cmdAppend;
    QPushButton * cmdInsert;
    QPushButton * cmdDelete;
    QPushButton * cmdUp;
    QPushButton * cmdDown;

    QPushButton * cmdUseAsCurrent;
    QPushButton * cmdUseForIni;

    stdButtonGroup * buttons;
#if QT_VERSION >= 0x050000

#else
    QSignalMapper * mapper;
#endif
int cur_list_len;
int * cur_list_entries_length;
char ** cur_list_patterns;
QIcon ** cur_list_LineIcons;
QPixmap ** cur_list_LinePixmaps;
QVector<qreal> ** cur_list_PenDashPattern;

    int cur_line_len;
    char * cur_line_pattern;
    QPixmap * cur_line_pm;
    QIcon * cur_line_ic;
    QVector<qreal> * cur_line_pendash;

public slots:
    void init(void);
    void doApply(void);
    void doAccept(void);
    void doClose(void);

    void doAppend(void);
    void doSingularAppend(void);
    void doInsert(void);
    void doEdit(void);
    void doDelete(void);
    void doUseAsCurrent(void);//Use the settings currently shown as the Currrent LineStyles
    void doUseForIni(void);//Use the settings currently shown for the Ini-File
    void doSwap(int a,int b);
    void doUp(void);
    void doDown(void);
    void setCurrentListForAll(void);

    void show_style_changed(int nr);
    void currentStyleChanged(int nr);
    void setPanelsToOnePattern(int len,char * pat,int pan_count);
    void currentLengthChanged(int nr);
    void panel_clicked(int nr);
    void refillLineStyleSelector(void);//resets the LinesStyleSelectors Panels to the current line-style-settings in this dialog

    void generate_Pattern(void);
    void plotExamples(void);
virtual void resizeEvent ( QResizeEvent * event );
signals:
    void close_wish(void);
};


frmSpreadSheet * findOpenSpreadSheet(int gno,int setno);
void showSetInSpreadSheet(int gno,int setno);
//void deleteSpreadSheet(int gno,int setno);


class frmFeatureExtract:public QDialog
{
    Q_OBJECT
public:
    frmFeatureExtract(QWidget * parent=0);
    ~frmFeatureExtract();
    QLabel * lblSourceGraph;
    uniList * listSourceGraph;
    QLabel * lblResultGraph;
    uniList * listResultGraph;
    QLabel * lblAbscissaGraph;
    uniList * listAbscissaGraph;
    QLabel * lblSet;
    uniList * listSet;

    StdSelector * selRestriction;
    QCheckBox * chkNeg;

    stdButtonGroup * buttonGroup;
    StdSelector * selFeature;
    StdSelector * selXValue;
    stdLineEdit * ledValue;
    QVBoxLayout * layout;
    char tbuf[1024];
public slots:
    void init(void);
    void FeatureChanged(int i);
    void XValueChanged(int i);
    void doClose(void);
    void doAccept(void);
    void fext_routine(int gfrom, int gto, int feature, int abs_src, int abs_set, int abs_graph , int restr, int neg);
    /*int mute_linear_regression(int n, double *x, double *y, double *slope, double *intercept);
    int get_rise_time( int setl, double *xv, double *yv, double min, double max,double *width );
    int get_fall_time( int setl, double *xv, double *yv, double min, double max,double *width );
    int get_zero_crossing( int setl, double *xv, double *yv, double *crossing );
    int get_half_max_width(int len, double *x, double *y, double *width);
    int getmedian( int grno, int setno, int sorton, double *median );
    int get_barycenter( int n, double *x, double *y, double *barycenter );
    void get_max_pos( double *a, double *b, int n, double max, double *d );*/
};

class frmAgrInfos:public QDialog
{
    Q_OBJECT
public:
    frmAgrInfos(QWidget * parent=0);
    ~frmAgrInfos();
    QLabel * lblFilename;
    QLabel * lblFilenText;
    QLabel * lblDescription;
    QLabel * lblDescrText;

    StdSelector * selSelection;
    StdSelector * selTargetGno;
    QCheckBox * chkAutoscale;

    QWidget * empty;
    QScrollArea * scroll;

    int allocated_controls;
    QCheckBox ** chkImport;
    QLabel ** lblID;
    QLabel ** lblLegend;
    QLabel ** lblComment;
    QLabel ** lblType;
    QLabel ** lblTitle;

    //QPushButton * cmdImport;
    QPushButton * cmdShowAgrInfo;

    struct agr_file_info info;
    bool shButtons,shFilename;
    int sets_imported,agrs_opened;

    QGridLayout * layout0;
    QGridLayout * layout1;

    stdButtonGroup * buttons;

public slots:
    void setVisibleItems(bool showButtons,bool showFilename);
    void init(char * filen);//initialize the display (not the target graph)
    void readSettings(void);
    void changeSelection(int index);
    void doImport(void);
    void setInfoVisible(bool v);
    void toggleShowAgrInfo(void);
    void doCancel(void);
    void doOpenAgr(void);
    void doImportAgr(void);
    void reset_import_counters(void);
    void get_import_counts(int * n_o_sets,int * n_o_agrs);
signals:

};

class frmIOForm:public QDialog
{
    Q_OBJECT
public:
    frmIOForm(int type,QWidget * parent=0);
    ~frmIOForm();
    int formType;
    QString stdExtension;

    FileSelector * selector;
    frmEditBlockData * frmEdBlock;

    QLabel * lblReadGraph;
    QLabel * lblWriteSet;
    QLabel * lblDescription;
    QLabel * lblSelection;
    QLabel * lblDataSource;

    QTextEdit * txtDescription;
    QLabel * lblProjectContent;

    stdLineEdit * ledFormat;
    stdLineEdit * ledFormat2;
    stdLineEdit * ledTitle;

    stdLineEdit * ledSelection;

    StdSelector * selLoadAs;
    StdSelector * selType;
    StdSelector * selAutoscale;
    StdSelector * selParamGraph;
    QRadioButton * radPipe;
    QRadioButton * radDisk;
    QCheckBox * chkExchangeCommaPoint;
    QCheckBox * chkWriteWav;

    CompressionSelector * selCompress;

    uniList * graphList;
    StdSelector * selExportGraph;
    uniList * setList;

    QPushButton * cmdOK;
    QPushButton * cmdFilter;
    QPushButton * cmdCancel;
    QPushButton * cmdHelp;

    QGroupBox * grpRead;
    QGridLayout * layout1;
    QGroupBox * grpWrite;
    QVBoxLayout * layout2;
    QGroupBox * grpDescription;
    QVBoxLayout * layout3;
    QGroupBox * grpTitle;
    QVBoxLayout * layout4;
    QGroupBox * grpParamGraph;
    QVBoxLayout * layout5;

    QGroupBox * grpBinary;
    QGridLayout * layout6;
    QGridLayout * layout7;
    QWidget * empty;
    QScrollArea * area;
    QCheckBox * chkHeader;
    stdIntSelector * selHeaderLength;
    stdIntSelector * selTotalColumnNumber;
    StdSelector ** selColFormats;
    StdSelector ** selImportTo;
    QSpinBox ** selColSize;
    int selector_count;
    QLabel * lblFormat;
    QLabel * lblBytes;
    QLabel * lblImport;
    QCheckBox * chkImportToEOF;
    stdIntSelector * selMaxImportData;

    QGridLayout * layout;
//frmAgrInfos * agrInfo;
    QPushButton * cmdOpenSetImport;

    QString selectedFile;
    QString savedExtension;
    QFileInfo selFileInfo;
    bool FileExists;
    bool isWriteable;
    bool isReadable;

public slots:
    void gotNewSelection(QString selection);
    void gotNewSelectionDoubleClick(QString selection);
    void init(char * f_name=NULL);
    void exportGraphChanged(int g);
    void doOK(void);
    void doFilter(void);
    void doCancel(void);
    void doHelp(void);
    void doOpenSetImport(void);
    void newFileEnteredManually(void);
    void headerChecked(int c);
    void readUntilEOFChecked(int c);
    void setTypeChanged(int c);
    void columnCountChanged(int c);
    void columnSizeChanged(int c);
    void writeWavToggled(int c);
signals:
    void newFileSelectedForIO(int type,QString file,bool exists,bool writeable,bool readable);
};

class frmFourier:public QDialog
{
    Q_OBJECT
public:
    frmFourier(QWidget * parent=0);
    ~frmFourier();
    QLabel * lblApplyTo;
    uniList * sel;

    stdButtonGroup * buttonGroup;
    StdSelector * selectors[5];
    QCheckBox * chk_scale;
    QVBoxLayout * layout;
public slots:
    void init(void);
    void doDFT(void);
    void doFFT(void);
    void doWindowOnly(void);
    void doClose(void);
};

#define FILTER_LOW_PASS 0
#define FILTER_HIGH_PASS 1
#define FILTER_BAND_PASS 2
#define FILTER_BAND_STOP 3

#define FILTER_BRICKWALL 0
#define FILTER_BUTTERWORTH 1
#define FILTER_BESSEL 2
#define FILTER_CHEBYCHEV 3
#define FILTER_GAUSSIAN 4
#define FILTER_BUTTERWORTH_SIMPLE 5

#define PROCESSING_INTERPOLATION 0
#define PROCESSING_ZERO_PADDING 1
#define PROCESSING_FIRST_LAST_PADDING 2

int create_padded_set(int n_gno,int & n_sno,int o_gno,int o_sno,int new_length,int padding_type);
int do_filter_on_one_set(int n_gno,int n_sno,int o_gno,int o_sno,int type,int realization,int resid,int res_negate,int abs,int debug,char * formula,int point_extension,int oversampling,double ripple,int order1,int order2,double f1,double f2);

class frmFourier2:public QDialog
{
    Q_OBJECT
public:
    frmFourier2(QWidget * parent=0);
    ~frmFourier2();
    QGroupBox * gpbLow;
    QGroupBox * gpbHigh;
    QGroupBox * gpbProcessing;

    grpSelect * grpSource;
    grpSelect * grpDestination;

    stdButtonGroup * buttonGroup;
    StdSelector * selType;
    StdSelector * selPoints;
    StdSelector * selKind;
    StdSelector * selOversampling;
    stdLineEdit * lenTimeFormula;
    stdLineEdit * lenLowFrequency;
    stdLineEdit * lenHighFrequency;
    stdIntSelector * selLowOrder;
    stdIntSelector * selHighOrder;
    stdLineEdit * lenRipple;
    QCheckBox * chkAbsValue;
    QCheckBox * chkDebug;

    QGroupBox * fraSourceDatFilt;
    StdSelector * selRestriction;
    QCheckBox * chkNeg;
    QHBoxLayout * layout0;

    QGridLayout * layout;
    QHBoxLayout * layout1;
    QHBoxLayout * layout2;
    QGridLayout * layout3;

public slots:
    void init(void);
    void filterTypeChanged(int i);
    void filterKindChanged(int i);
    void selPointsChanged(int i);
    void doFilter(void);
    void doAccept(void);
    void doClose(void);
};

class frmExtendedEditBlockData;

class frmEditBlockData:public QDialog
{
    Q_OBJECT
public:
    frmEditBlockData(QWidget * parent=0);
    ~frmEditBlockData();
    int block_curtype;
    QString filename;
    int source;
    grpSelect * grpLoadTo;
    QString begining,middle;
    QLabel * lblData;
    QGroupBox * grpColumns;
    QGroupBox * grpBlockContents;
    QLabel * lblBlockContents;
    QVBoxLayout * layout1;
    show_text_file_widget * showBlockData;
    QCheckBox * chkExtendedInput;
    StdSelector * columnSelector[6];
    StdSelector * selType;
    StdSelector * selStringColumn;
    StdSelector * selAutoscale;
    stdButtonGroup * buttonGroup;
    QGridLayout * layout0;
    QGridLayout * layout;
    frmExtendedEditBlockData * extendedBlockInput;
    int width_without,initialized;
public slots:
    void setCompleteData(int columns,int length);
    void setTypeChanged(int i);
    void init(void);
    void update_eblock(int gno);
    void extInputToggled(bool ch);
    void gotNewBlockData(void);
    void doAccept(void);
    void doApply(void);
    void doClose(void);
};

class frmExtendedEditBlockData:public QWidget
{
    Q_OBJECT
public:
    frmExtendedEditBlockData(QWidget * parent=0);
    char * filename;
    QLabel * lblFilename;
    QLabel * lblContents;
    stdIntSelector * selMaxLines;
    StdSelector * selLoadSet;
    QLabel * lblContentsTitle;
    QLabel * lblValuesTitle;
    QScrollArea * scroll;
    QWidget * empty;
    int cols_alloc,rows_alloc,selectors_alloc;//rows are the different sets, selectors_alloc is needed for memory management
    QLabel ** lblSetInfos;//the set-infos: Set:Rows/Cols
    QLabel ** lblColumns;
    QComboBox ** cmbColumns;
    QGridLayout * grid1;
    QGridLayout * grid2;
    QPushButton * cmdReRead;
    QPushButton * cmdStore;
    show_text_file_widget * showFile;
    show_text_file_widget * showResult;
    struct block_input_format block;
public slots:
    void init(const char *file_name);
    void show_results(void);
    void recreateInputColumns(void);
    void readInputColumns(void);
    void doTestRead(void);
    void doStore(void);
    void doClose(void);
signals:
    void newBlockData(void);
};

class frmLoadEval:public QDialog
{
    Q_OBJECT
public:
    frmLoadEval(QWidget * parent=0);
    ~frmLoadEval();
    QGroupBox * grpParameterMesh;
    stdStartStop * ststst;
    QHBoxLayout * layout0;
    QLabel * lblText[MAX_SET_COLS];
    QLineEdit * ledText[MAX_SET_COLS];
    stdSetTypeSelector * selType;
    stdButtonGroup * buttonGroup;
    QGridLayout * layout;
    uniList * parentList;
public slots:
    void Redisplay(void);
    void typeChanged(int i);
    void doAccept(void);
    void doApply(void);
    void doClose(void);
};

class frmEvalExpr : public QDialog
{
    Q_OBJECT
public:
    MainWindow * parentWindow;

    grpSelect * grpSource;
    grpSelect * grpDestination;

    QLabel * lblFormula;
    QTextEdit * ledFormula;
    QGroupBox * fraSourceDatFilt;
    QLabel * lblRestr;
    QCheckBox * chkNeg;
    QCheckBox * chkApplyAndKeep;
    QPushButton * cmdApply;
    QPushButton * cmdAccept;
    QPushButton * cmdClose;
    StdSelector * selRestriction;

    frmEvalExpr(MainWindow * parWind,QWidget *parent=0);
    ~frmEvalExpr();

    stdButtonGroup * buttonGroup;
    QGridLayout * layout0;
    QGridLayout * layout;

public slots:
    void doApply(void);
    void doAccept(void);
    void doClose(void);
    void init(void);
    void update(void);
    void compute_aac(void);
    void takeSourceToDest(void);
    void takeDestToSource(void);
    void restrictionChanged(int i);
};

class nlcfTabMain:public QWidget
{
    Q_OBJECT
public:
    nlcfTabMain(QWidget * parent);
    stdLineEdit * ledFormula;
    stdLineEdit * ledTolerance;
    StdSelector * selParameters;
    stdIntSelector * selIterations;

    QScrollArea * scroll;
    spreadSheet * spreadParameters;

    QGridLayout * layout;
public slots:
    void paraNumberChanged(int i);
};

class nlcfTabAdvanced:public QWidget
{
    Q_OBJECT
public:
    nlcfTabAdvanced(QWidget * parent);

    QGroupBox * grpSourceDataFilter;
    StdSelector * selRestrictions;
    QCheckBox * chkNegated;
    QHBoxLayout * layout0;

    QGroupBox * grpWeighting;
    StdSelector * selWeights;
    stdLineEdit * ledFunction;
    QHBoxLayout * layout1;
    QGroupBox * grpLoadOptions;
    StdSelector * selLoad;
    stdLineEdit * ledStartLoadAt;
    stdLineEdit * ledStopLoadAt;
    stdLineEdit * ledNumberOfPoints;
    stdLineEdit * ledXFunction;
    QGridLayout * layout2;

    QVBoxLayout * layout;
public slots:
    void weights_changed(int i);
    void load_changed(int i);
};

class frmNonlinCurveFit:public QDialog
{
    Q_OBJECT
public:
    frmNonlinCurveFit(QWidget * parent=0);
    ~frmNonlinCurveFit();
    frmIOForm * frmOpenFitPara;
    frmIOForm * frmSaveFitPara;

    QMenuBar * menuBar;
    QMenu * mnuFile;
    QMenu * mnuEdit;
    QMenu * mnuView;
    QMenu * mnuHelp;

    grpSelect * grpSource;
    grpSelect * grpDestination;
    QWidget * empty;

    QLineEdit * lblAFit;

    QTabWidget * tabs;
    nlcfTabMain * tabMain;
    nlcfTabAdvanced * tabAdvanced;

    stdButtonGroup * buttonGroup;
    QHBoxLayout * layout0;
    QVBoxLayout * layout;

    QAction * actOpen,*actSave,*actClose,*actResetFit,*actLoadFit,*actAutoLoad,*actUpdate,*actHelpOnContext,*actHelpOnFit;

public slots:
    void init(void);
    bool read_fit_options(void);
    void update_nonl_frame(void);
    void CreateActions(void);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
    void doOpen(void);
    void doSave(void);
    void doResetFit(void);
    void doLoadFit(void);
    void doAutoLoad(void);
    void doUpdate(void);
    void doHelpContext(void);
    void doHelpFit(void);
    void load_nonl_fit(int src_gno, int src_setno, int force);
    void IOrequested(int type,QString file,bool exists,bool writeable,bool readable);
    void adjustToFontSize(void);
virtual void showEvent(QShowEvent * event);
};

class frm2DFit:public QDialog
{
    Q_OBJECT
public:
    frm2DFit(QWidget * parent=0);
    ~frm2DFit();

    grpSelect * grpSource;
    QCheckBox * chkGuessStartingValues;
    QCheckBox * chkShowResult;
    StdSelector * selObject;
    stdIntSelector * selNrOfPoints;
    stdLineEdit * selParameter[8];
    StdSelector * selRestrictions;
    QCheckBox * chkNegated;
    QLabel * lblResults;

    QPushButton * cmdFit;
    QPushButton * cmdExit;

    QGridLayout * layout;

    int sel_gno,sel_sno;
    double x0,y0,r,a,b,alpha;
    double phi0,phi1;
    int restr_type,restr_negate;
    int nr_of_points;
    bool block_guessing;
public slots:
    void init(void);
    void readInputs(void);
    void reDisplayContents(void);
    void objectSelectionChanged(int i);
    void showToggled(bool c);
    void guessToggled(bool c);
    void newSetSelected(int s);
    void doFit(void);
    void doClose(void);
};

class frmInterpolation:public QDialog
{
    Q_OBJECT
public:
    frmInterpolation(int type,QWidget * parent=0);
    ~frmInterpolation();
    int WindowType;
    grpSelect * grpSource;
    grpSelect * grpDestination;
    uniList * sampSet;
    QCheckBox * chkStrict;
    QCheckBox * chkNextPowerOfTwo;
    QCheckBox * chkCumulHist;
    QCheckBox * chkNormalize;
    QCheckBox * chkDensity;
    StdSelector * selMethod;
    StdSelector * selSampling;
    StdSelector * selSamplingGraph;
    QGroupBox * grpSampling;
    stdLineEdit * lenStart;
    stdLineEdit * lenStop;
    stdLineEdit * lenLength;
    QLabel * lblSamplSet;
    stdButtonGroup * buttonGroup;
    QGridLayout * layout;
    QGridLayout * layout1;

public slots:
    void init(void);
    void selSamplingChanged(int i);
    void selSamplGraphChanged(int i);
    void toggleNextPowerOfTwo(bool i);
    void doAccept(void);
    void doApply(void);
    void doClose(void);
};

class frmSmallCalc:public QDialog
{
    Q_OBJECT
public:
    frmSmallCalc(int type,QWidget * parent=0);
    ~frmSmallCalc();
    int WindowType;
    QLabel * lblApplyTo;
    uniList * listSets;
    StdSelector * selector1;
    StdSelector * selector2;
    StdSelector * selector3;
    stdLineEdit * lenText;
    QCheckBox * chkInvert;
    stdStartStop * StartStop;
    stdButtonGroup * buttonGroup;
    QGridLayout * layout;
public slots:
    void init(void);
    void loadChanged(int i);
    void doAccept(void);
    void doClose(void);
};

class frmCorrelation:public QDialog
{
    Q_OBJECT
public:
    frmCorrelation(int type,QWidget * parent=0);
    ~frmCorrelation();
    int WindowType;
    QLabel * lblSelect1;
    uniList * listSet1;
    QLabel * lblSelect2;
    uniList * listSet2;
    stdLineEdit * lenLag;
    QCheckBox * chkCovariance;
    stdButtonGroup * buttonGroup;
    QVBoxLayout * layout;
public slots:
    void init(void);
    void doAccept(void);
    void doClose(void);
};

class frmTransform:public QDialog
{
    Q_OBJECT
public:
    frmTransform(int type,QWidget * parent=0);
    ~frmTransform();
    int WindowType;
    QLabel * lblApplyTo;
    uniList * listSets;
    stdLineEdit * lenText[7];
    StdSelector * selector[4];
    stdButtonGroup * buttonGroup;
    QVBoxLayout * layout;
public slots:
    void init(void);
    void PruneTypeChanged(int i);
    void CoordsChanged(int i);
    void doAccept(void);
    void doReset(void);
    void doClose(void);
    void selectorChanged(int i);
};

#ifndef HAVE_NETCDF
#define nc_type int
#define NC_SHORT 0
#define NC_LONG 1
#define NC_FLOAT 2
#define NC_DOUBLE 3
#define NC_CHAR 4
#endif

class frmNetCDF:public QDialog
{
    Q_OBJECT
public:
    frmNetCDF(QWidget * parent=0);
    ~frmNetCDF();
    QPushButton * cmdQuery;
    QLabel * lblSelX;
    QLabel * lblSelY;
    uniList * netcdf_listx_item;
    uniList * netcdf_listy_item;
    stdLineEdit * netcdf_file_item;
    stdButtonGroup * buttonGroup;
    QVBoxLayout * layout;
    int ncopts;

public slots:
    void init(void);
    void update_netcdfs(void);
    void do_netcdfquery_proc(void);
    void do_netcdf_proc(void);
    void doFiles(void);
    void doClose(void);
    const char *getcdf_type(nc_type datatype);
    void IOrequested(int type,QString file,bool exists,bool writeable,bool readable);
};

/*binary format input*/
class inputLine:public QWidget
{
    Q_OBJECT
public:
    inputLine(int t,QWidget * parent=0);
    ~inputLine();
    int type;//type of input line: input from header(0) or input from data
    QHBoxLayout * layout;
    QLabel * lblOffset;
    QPushButton * cmdNew;
    QSpinBox * spnSize;
    QComboBox * cmbFormat;
    QComboBox * cmbImportAs;
    int datas[NUMBER_OF_IMPORT_DESTINATIONS];
public slots:
    int getSize(void);
    int getType(void);
    int getTarget(void);
    void setSize(int s);
    void setType(int t);
    void setTarget(int t);
    void formatChanged(int i);
};

#define HEADER_FORMAT_MANUAL 0
#define HEADER_FORMAT_DATA_FILE 1
#define HEADER_FORMAT_BIN_FILE 2
#define HEADER_FORMAT_INI_FILE 3
#define HEADER_FORMAT_ASCII_FILE 4

class frmBinaryFormatInput;

struct importSettings
{
    QString filename,name;//used to associate this with a file name
//all settings needed to import from a binary file - some are set directely, some are read from a header, not all are used every time (depends on the settings)
    int valid_status;//-1=invalid (uninitialized), 0=initialized as a scheme, 1=header read, 2=ready for loading bin-data (everything set)
/// general file-informations (set without having to read a header)
    QString HeaderFile,DataFile;
    QString DataSuffix,HeaderSuffix;//if no header is present or the header is part of the datafile itself HeaderSuffix==""
    QString ReadFromHeader,ReadFromData;//Files header-information and data has been read from (needed to tell whether informations have been read from the right files already)
    bool header_present;
    int header_format;//0=manual,1=header in data file itself,2=header in separate binary-data-file,3=header in separate ini-file, 4=header in separate ascii-file
    char string_end_char;
    //bool keep_trigger;
    bool read_to_eof,multiple_header_files,determine_string_size;
    int setorder,autoscale,trigger_channel;
    int target_gno,set_type;//target set is always the next set-id available
    int headersize;//the number of bytes before the actual data starts

    int data_is_compressed;//compression method; 0=none, 1=Qt-internal (zlib-based), 2=external (zlib), 3=... not implemented (yet)

    //the following values may or may not be present in the header; they can be set ab initio or can be read from the header
    double triggervalue;
    bool trigger_is_percent;
    int trigger_type;//-1=no trigger, 0=rising edge, 1=falling edge, 2=either edge
    int channels,points;

    //the actual import format for all channels
    int * format_suggestion;//read from header and scheme to find the most suitable format
    int * channel_format;
    int * channel_size;
    int * channel_target;

/// informations (usually) read from a header
    //necessary to read the data
    int bytesize,bitsize;
    int whole_size,single_size;
    double factors[7];//scaling factors for x,y,y1,y2,y3,y4,trigger
    double offsets[7];//linear offsets for x,y,y1,y2,y3,y4,trigger
    double channel_factors[MAX_BIN_IMPORT_CHANNELS];//scaling factors for every channel; we use only 32 channels here -- should be enough
    double channel_offsets[MAX_BIN_IMPORT_CHANNELS];//offset value for every channel
    //bool channel_format_known;//tells the gui whether the channel format has been determined (e.g. from the header)
    //ini-header-files
    int nr_of_import_tokens;//how many relevant tokens are present in an ini-file
    int * token_target;//where to import header-data into
    QStringList vals,keys;//header-informations form an ini-file
    QList<int> import_channel_dest;//what channel the data is to import to (-1 = general information, not specific to a channel)
    QList<int> import_dest;//where to import the data read from the header
    //entry-counters
    int nr_of_counters;
    QList<int> counterTypes,counterTargets,counterSets;
    QStringList counterSections,counterText;
    QList<int> counterValues;
    //key preprocessing
    int nr_of_preprocessors;
    QList<int> preprocessingType,preprocessingSet,preprocessingTarget,preprocessingMultipleA,preprocessingMultipleB;
    QStringList preprocessingKey,preprocessingCharA,preprocessingCharB;
    QList<double> preprocessingValue;
    QStringList preprocessingString;

    //binary-header-files
    int nr_of_header_values;
    int * header_value_format,* header_value_size,* header_value_import;

    //control values set after a header has been read
    bool x0set,deltaxset,fset,contains_trigger;

    //auxiliary data read from the header
    char * title;
    char * subtitle;
    char * x_title,*y_title,*set_title[MAX_BIN_IMPORT_CHANNELS];

    double x0,deltax,f;//f=sampling rate-->deltax=1/f; x=x0+deltax*i

/// first data read after header (just to check the import settings) --> later used for all data read (all channels)
    double ** first_data;
    int points_read,columns_read;
};

/*manual data input (simulationg a header)*/
class manualHeaderData:public QWidget
{
    Q_OBJECT
public:
    manualHeaderData(QWidget * parent=0);
    QGridLayout * layout;
    stdIntSelector * selNrOfEntries;
    StdSelector ** selTokenType;
    stdLineEdit ** ledTokenValue;
    StdSelector ** selTargetChannel;
    int tokens;
public slots:
    void changeNumberOfEntries(int nr);
    void readDataToScheme(struct importSettings & imp_schema);
    void displayDataFromScheme(struct importSettings imp_schema);
};

class inputIniPreprocessing:public QWidget
{
    Q_OBJECT
public:
    inputIniPreprocessing(QWidget * parent=0);
    QHBoxLayout * layout;
    stdLineEdit * lenCharA;
    stdLineEdit * lenCharB;
    QSpinBox * spnMultA,* spnMultB;
    StdSelector * selTarget;
    StdSelector * selKey;
    StdSelector * selSet;
    StdSelector * selType;
    importSettings * imp_set;//needed for preview
public slots:
    void init(QStringList & keys);
    void typeChanged(int t);
    void getPostprocessingSettings(int & Type,QString & key,QString & A,QString & B,int & multA,int & multB,int & set,int & target);
    void textChanged(QString s);
    void valueCanged(int t);
    void generatePreview(void);
};

class inputIniCounter:public QWidget
{
    Q_OBJECT
public:
    inputIniCounter(QWidget * parent=0);
    QHBoxLayout * layout;
    stdLineEdit * lenText;
    StdSelector * selTarget;
    StdSelector * selSection;
    StdSelector * selSet;
    StdSelector * selType;
    importSettings * imp_set;//needed for preview
public slots:
    void init(QStringList & sections);
    void typeChanged(int t);
    void getSettings(int & type,QString & section,QString & text,int & target,int & set);
    void sourceChanged(int t);
    void sourceTextChanged(QString & s);
    void generatePreview(void);
};

class multiIniCountPreproc:public QWidget
{
    Q_OBJECT
public:
    multiIniCountPreproc(QWidget * parent=0,int type=0);
    int type;//0=counter, 1=preprocessor
    QGridLayout * layout;
    inputIniCounter ** counter;
    inputIniPreprocessing ** preproc;
    QPushButton ** minus;
    QPushButton * plus;
    QLabel * lblTitle;
    int allocated,nr_of_counters;//there are only counters OR preprocessors
    QStringList sections;
    importSettings * imp_set;//needed for preview
#if QT_VERSION >= 0x050000

#else
    QSignalMapper* map;
#endif
public slots:
    void init(struct importSettings & input);
    void clearCounters(void);
    void setAllCounters(struct importSettings & input);
    void setAllPreprocessors(struct importSettings & input);
    void minusClicked(int c);
    void plusClicked(void);
    void redisplayCounters(void);
    void get_all_counter_settings(struct importSettings & input);
    void get_all_preprocessor_settings(struct importSettings & input);
    int nr_of_lines(void);
    void setImportSetting(importSettings * i_s);
signals:
    void nrOfCountersChanged(void);
};

/*ini-file format input*/
class inputIniData:public QWidget
{
    Q_OBJECT
public:
    inputIniData(QWidget * parent=0);
    QGridLayout * layout;
    QLabel ** titles;
    QLabel ** lbl_imp_key;
    QLabel ** lbl_imp_value;
    QComboBox ** cmbChannel;
    QComboBox ** cmbImportTo;
    int lines;
    QList<int> datas;
    multiIniCountPreproc * multCounters;
    multiIniCountPreproc * multPreprocessors;
    importSettings * imp_set;//needed for preview
public slots:
    void clearData(void);
    void setCounterData(struct importSettings & imp_set);
    void initData(struct importSettings & imp_set);
    void setData(struct importSettings & imp_set);
    void redisplay(void);
};

class pageHeaderInfo:public QWidget
{
    Q_OBJECT
public:
    pageHeaderInfo(QWidget * parent=0);
    QVBoxLayout * grid;
    QGridLayout * gridLayout;
    QVBoxLayout * layout;
    QWidget * empty;
    //QWidget * empty0;
    QWidget * empty1;
    //QWidget * empty2;
    QScrollArea * scroll;
    QScrollArea * scroll2;
    QScrollArea * scroll3;
    //QVBoxLayout * layout2;
    QPushButton * cmdAdd;
    QLabel ** Headers;
    int number_of_lines;
    inputLine ** inFormats;
#if QT_VERSION >= 0x050000

#else
    QSignalMapper * map;
#endif
    QLabel * lblEndChar;
    QLineEdit * lenEndChar;
    QPushButton * cmdTestLoad;
    //int nr_of_sels;
    //StdSelector ** sels;
    manualHeaderData * manHeaderGroup;
    inputIniData * iniDataGroup;
    //QString * readValues;
    QPushButton * cmdReadIni;
    int nr_of_entries;
    QString entries[NUMBER_OF_IMPORT_DESTINATIONS];
    int entry_values[NUMBER_OF_IMPORT_DESTINATIONS];
    frmBinaryFormatInput * par_wid;
    inputIniData * inpIniData;
    QStringList keys;//the ini-input-keys
signals:
    void readHeader(void);
public slots:
    void changeRepresentation(int r);
    void doReadIni(void);//read Data from a ini-header-file
    void resizeIniDisplay(void);
    void updateOffsets(void);
    void doNew(void);
    void doDelete(int i);//remove a line
    void offsetChanged(int i);
    void doTestLoad(void);
    void read_header_settings(struct importSettings & imp_set);//transfer import settings from gui to imp_set
    void write_header_settings(struct importSettings & imp_set);//transfer import settings to gui
};

class pageDataInfo:public QWidget
{
    Q_OBJECT
public:
    pageDataInfo(QWidget * parent=0);
    QGridLayout * grid;
    QLabel * lblComment;
    QCheckBox * chkReadToEOF;
    //QCheckBox * chkKeppTrigger;
    StdSelector * selTriggerSet;
    CompressionSelector * selDataCompressed;
    QLabel * lblChannelCount;
    QSpinBox * spnChannelCount;
    QLabel * lblDataSetCount;
    QSpinBox * spnDataSetCount;
    QScrollArea * scroll;
    QVBoxLayout * layout;
    QWidget * empty;
    int number_of_lines;
    inputLine ** inFormats;
    StdSelector * selOrder;
    stdLineEdit * selTriggerValue;
    StdSelector * selTriggerType;
#if QT_VERSION >= 0x050000

#else
    QSignalMapper * map;
#endif
signals:
    void newChannelCount(int i);
public slots:
    void channelCountChanged(int i);
    void eofToggled(bool i);
    void input_target_changed(int t);
    void readDataSettings(importSettings & imp_set);
    void writeDataSettings(importSettings & imp_set);
};

class pageFileInfo:public QWidget
{
    Q_OBJECT
public:
    pageFileInfo(QWidget * parent=0);
    ~pageFileInfo();
    QVBoxLayout * layout;
    QLabel * lblText;
    QTextEdit * lenText;
    QString headerContents;
    char * DatFile;
    char * HeaderFile;
signals:
    void requestInfos(void);
public slots:
    virtual void showEvent(QShowEvent * event);
    void ShowInfos(void);
};

class pageImportInfo:public QWidget
{
    Q_OBJECT
public:
    pageImportInfo(QWidget * parent=0);
    stdSetTypeSelector * selType;
    StdSelector * selAutoscale;
    QLabel * lblImportTo;
    uniList * importGraph;
    QVBoxLayout * layout;
    //graphList=new uniList(GRAPHLIST,grpRead);

public slots:
    void newChannelCount(int i);
    void read_settings(struct importSettings & imp_set);//transfers settings from gui to imp_set
    void write_settings(struct importSettings & imp_set);//transfers settings from imp_set to gui
};

void LoadFileFormat(const char * fname, struct importSettings & imp_set);
void SaveFileFormat(const char *fname, struct importSettings & imp_set);
/*void matchSchemeToHeader(char * fname,struct importSettings & imp_set,struct importSettings & imp_scheme);
void readBinFileHeader(const char * fname,struct importSettings & imp_set);*/

class frmBinaryFormatInput:public QDialog
{
    Q_OBJECT
public:
    frmBinaryFormatInput(QWidget * parent=0);
    ~frmBinaryFormatInput();
    QGridLayout * grid;
/*
    QPushButton * cmdSave;
    QPushButton * cmdLoad;
    QPushButton * cmdStdSave;
    QPushButton * cmdStdLoad;
    QCheckBox * chkAutoGuessFormat;
*/
    QLabel * lblCurScheme;
    QCheckBox * chkHeader;
    QCheckBox * chkMultiHeaders;
    QLabel * lblFormatSource;
    QComboBox * cmbFormatSource;

    stdLineEdit * lenDataFile;
    stdLineEdit * lenHeaderFile;
/*
    QLabel * lblDataFile;
    QLineEdit * lenDataFile;
    QLabel * lblHeaderFile;
    QLineEdit * lenHeaderFile;
*/
    QLabel * lblHeaderFileFormat;
    QComboBox * cmbHeaderFileFormat;
    QTabWidget * tabs;
    stdButtonGroup * aac;
    QPushButton * cmdSelectDataFile;
    QPushButton * cmdSelectHeaderFile;

    pageHeaderInfo * tabHeader;
    pageDataInfo * tabDataInfo;
    pageFileInfo * tabFileInfo;
    pageImportInfo * tabImportInfo;

    int bin_file_nr_to_import;
    //char datFileName[512];
    QStringList datFileNames;
    //char headerFileName[512];
    QStringList headerFileNames;

    importSettings imp_set;//the current import-settings to be used
    importSettings imp_scheme;//the current sheme for the import-settings

    QString settingString;
    int first_suggestion;
    bool auto_transfer_from_header,determine_string_size;
    int headersize;
    int cur_import_scheme;//-1=None, -2=from a format-file, 0....n=std_format_sheme --> to tell the contents of the scheme
    QString FormatFileLastUsed;//used if cur_import_scheme==-2 to update settings
    QString LoadFormatPath,SaveFormatPath,LoadIniPath,LoadDataPath;
    QString HeaderSuffix,Data_Suffix;
    QString HeaderPath,Data_Path;
    /*QFileDialog * dlgLoadFormat,* dlgSaveFormat;
    QFileDialog * dlgLoadIniFile,* dlgLoadDataFile;*/

    QMenuBar * mnuBar;
    QMenu * mnuData,*mnuSettings;
    QAction * actLoadSettings,*actSaveSettings,*actLoadStdSettings,*actSaveStdSettings,*actClose,*actDeleteSettings,*actautoguess;
    QAction * actLoadDataFile,*actLoadHeaderFile;

public slots:
    void CreateActions(void);
    void init(void);
    void getDatFilesFromString(QString * origin,QStringList * lst);//separates a text with files separated by ';' into a list with filenames
    void readHeader(void);//try to read data from a header
    void headerToggled(bool t);
    void doSaveFileFormat(void);
    void doLoadFileFormat(void);
    void doSaveStdFormat(void);
    void doLoadStdFormat(void);
    void doClearCurrentScheme(void);
    void SelectDataFile(void);//open a dialog to select a file
    void SelectHeaderFile(void);//open a dialog to select a file
    int detectStdBinFormat(QString filen);
    void formatSourceChanged(int i);
    void displaySettings(struct importSettings & imp_s);//transfer the settings from the scheme to the gui
    void readSettings(struct importSettings & imp_s,int type);//transfer the settings in the gui in the scheme (it is meant to be the scheme - but any import_settings are possible), type=0: just header, type=1: just data, type=2: just auxilliary data, type=3: everything
    void HeaderFormatChanged(int i);
    void transmitInfos(void);//this function is used after data has been read from a header to update the settings in the gui
    void convertSettingsToString(void);
    void updateSuffixes(void);
    void newFileEntry(void);//a new file (header or data) has been selected --> complete settings and try to load header (if suitable)
    void readAndCompleteFileNames(int dat_header,int & std_schema_nr,bool & is_header_file);//read the filenames from the LineEdits and complete the information (like: search for suitable headerfiles matching the datafiles)
    void CheckHeadersAndDatFiles(void);//to complete filenames and check completeness on informations

    void doOK(void);//do actual loading
    void doClose(void);
    void doAccept(void);//load and close

    //void binary_load_Phase0(int stage,struct importSettings & imp_scheme);//stage=0: read from header input, stage=2: read from data import settings, stage=3: read import settings (autoscale, target, ...)
};

void doReadDataFromHeader(ifstream & ifi,struct importSettings & imp_set);
void readBinaryFromFile(QBuffer & ifi,struct importSettings & imp_set,double *** data);
void get_all_settings_from_ini_file(QString ini_file,QStringList & keys,QStringList & vals);

void copy_basic_scheme_data(struct importSettings & imp_set,struct importSettings & imp_schema);
void copy_scheme_counters_preprocessors(struct importSettings & imp_set,struct importSettings & imp_schema);
void copy_manual_header_data(struct importSettings & imp_set,struct importSettings & imp_schema);
void read_INI_header(struct importSettings & imp_set,struct importSettings & imp_schema);
void read_BINARY_header(struct importSettings & imp_set,struct importSettings & imp_schema);
void read_ASCII_header(struct importSettings & imp_set,struct importSettings & imp_schema);
int postprocess_bin_import_data(struct importSettings & imp_set,int & nr_of_new_sets,int ** n_gnos,int ** n_snos);
void insert_filenames_in_settings(struct importSettings & imp_set,struct importSettings & imp_schema,QString headerfilename,QString datafilename);
void readHeaderData(struct importSettings & imp_set,struct importSettings & imp_scheme);
void prepare_imp_settings_for_header_import(struct importSettings & imp_set);

//this function takes a filename from the import-settings and tries to find a matching file in the same directory.
//This is meant to find header-files that match binary-data-files and vice versa.
//The function takes a file-extension and looks for all files in the same directory that have this extension.
//If one of the files has the same name as the file we are matching to this function returns just this file in the list (MATCH_TYPE_EXACT).
//If no file matches the name, all files with the same desired extension are returned (MATCH_TYPE_RESIDUAL).
//If no such files exist then RETURN_FAILURE is returned (MATCH_TYPE_NONE), otherwise RETURN_SUCCESS.
#define MATCH_TYPE_NONE 0
#define MATCH_TYPE_EXACT 1
#define MATCH_TYPE_RESIDUAL 2
int find_matching_files(struct importSettings & imp_set, bool find_header, QStringList & list, int & match_type);
int find_std_format_and_matching_files(QString filename, int & std_format_nr, QStringList & list, bool &is_header);

void binary_load_Phase1(QString Header_Filename,QString Data_Filename,struct importSettings & imp_set,struct importSettings & imp_scheme);//load header and initialize imp_set
//void binary_load_Phase2(struct importSettings & imp_set,struct importSettings & imp_scheme);//compare imp_set with imp_schema to prepare for binary import
void binary_load_Phase3(struct importSettings & imp_set);//the actual import
void binary_load_Phase4(struct importSettings & imp_set,int & nr_of_new_sets,int ** n_gnos,int ** n_snos);//the postprocessing of the data

class frmSetEditor:public QDialog
{
    Q_OBJECT
public:
    frmSetEditor(QWidget * parent=0);
    ~frmSetEditor();
    QVBoxLayout * layout;
    QHBoxLayout * layout1;
    QWidget * empty;
    QLineEdit * lenFromGraph,*lenFromSet;
    QLabel * labDataFrom1,*labDataFrom2;
    QTextEdit * text;
    stdButtonGroup * aac;
    int gno,setno;
    char * fname;
public slots:
    void init(int g_no,int set_no);
    void doUpdate(void);
    void doClose(void);
    void convertText(char oldDecSep,char newDecSep);//changes the Decimal separator in the text (except in lines containing commands)
};

struct csv_import_infos
{
    char * filename;
    bool valid_settings;
    int gno;//target-graph-number
    int headerlines;//number of lines to be ignored during reed
    int columns_to_read;//number of columns that are present in the file
    int * target_cols;//the column-numbers the read columns are to go to (X=0, Y=1, Y1=2 ...)
    int * col_format;//the format to be used for import of a column
    int * import_to_set;//set numbers to be actually used (one set number (0,1,2,...) for every column - could be -1=none or -2=all sets)
    int nr_of_target_sets;//number of sets to be created for the data
    int * target_sets;//next available set numbers (all available, before load --> this will be set to the sets actually to be used, because these sets have to be created)
    int * max_col_per_set;//the number of columns the new sets have to have
    char colsep,datsep,textsep;//the separator-characters
    bool dec_sep_komma;//is the decimal separator a komma (this means: exchange '.' and ',')
    double invalid_value;//this value is used if invalid data is present

    int set_generation;//0=single set, 1=NXY
    int set_type_gerenation;//set-type that is wanted (determines the number of columns in the new set(s))

    QByteArray ba;//The data read from a file (raw)
    QStringList csv_headerlines;
    QStringList csv_comments;
    QList<int> csv_comment_line_nr;
    QList<bool> csv_comment_at_end;//=true--> data before comment, =false--> the line is just a comment
    QList< QStringList > csv_tokens;//the QList is a list of lines, the QStringList is a list of the columns in a line
    QList< QList<int> > csv_token_format;
    QList< QList<bool> > csv_token_ok;
    QList< QList<double> > csv_token_value;
    QList<int> csv_column_formats;
    QList<int> csv_column_formats_suggestion;
    int number_of_errors,columns,rows;
    QList<int> error_pos1,error_pos2;//row and column of error
    QList<int> overlong_rows,underlong_rows;//error: a line has more/less columns than usual
    int csv_read_started,csv_read_stopped;//where in the file the read data comes from

    bool htmlShowComments,htmlShowValues,htmlShowOriginal,htmlShowErrorLines,htmlShowHeaderLines;
    //for html- and text-output: startValues are the number of lines to show at the start, endValues are the number of lines shown at the end (showing beginning and end of file)
    //if startValues is -1 all lines will be shown (or if startValues+endValues>=rows)
    //if endValues is -1 only the starting lines will be shown
    int startValues,endValues;
    QString text_as_html,errors_as_html,errors_as_text;

    QList<int> allColumnIndices,dataOnlyIndices,stringOnlyIndices,similarIndices,usedIndices;
    int lastColIndex,firstColIndex;
};

struct csv_multi_import_infos
{
    char * filename;
    bool valid_settings;
    int csv_file_length;//the whole number of lines in the file (needed for testing whether there is some data left)
    int nr_of_sets,nr_of_lines;//nr_of_lines=overall number of lines in the file
    char line_separator;//the line-separator used in the file
    char default_col_separator;
    QList<char> default_col_separators;
    bool komma_is_decimal_sep;
    double invalid_value;
    int error_count;
    QList<int> set_positions1,set_positions2;//beginning and end of set (number of line in file)
    QList<char> col_separator;//column-separator of every set (it is allowed that different sets in the same file have different column-separators)
    struct csv_import_infos * set;

    QStringList ignoredHeaderLines;
    bool htmlShowComments,htmlShowValues,htmlShowOriginal,htmlShowErrorLines,htmlShowHeaderLines;
    int startValues,endValues;
    int headerlines;//number of lines to be ignored during read
    QString text_as_html,errors_as_html,errors_as_text;

    QList< QList<int> > csv_column_formats_suggestion;

    //postprocessing options
    bool readAll,hasStringCol;
    int targetSetType,targetSetCols;
    QList<int> postprocess_target_col;//column of the target set(s) an operation is to be performed on
    QList<int> postprocess_operation_type;//+,-,*,:,*10^x --> value before=Z --> value after=Z+C or Z-C or Z*C or Z/C or Z*10^C
    QList<int> postprocess_operation_source1,postprocess_operation_source2;//>=0 --> a column in a set, <0 --> a temporary column, -100 --> value, -200 --> index; the column the second operand is comming from (C=0...n are the target columns, C=-1...-m are the original columns in the file (C'=-C-1, C' is the real column-index)
    QList<int> postprocess_operation_source1_sav,postprocess_operation_source2_sav;
    QList<int> postprocess_operation_source_set1,postprocess_operation_source_set2;//a set of -1 means that the source is one of the temporary columns (A,B,...)
    QList<double> postprocess_val1,postprocess_val2;
    //advancement: 0=no advancement, 1=just add 1, 2=keep original spacing but use next set of columns, 3=use next column with same data-format
    QList<int> postprocess_advancement1,postprocess_advancement2;
    int string_source_col,string_source_col_sav,string_source_set,string_advancement,sourceOfX;
    int number_of_possible_advancements;

    QList<int> postprocessing_order1,postprocessing_order2,postprocessing_order3,postprocessing_order4;//in what order the different columns in the different sets are used
    //order1=nr of target-line(string=-1), order2=source1(1)/source2(2)/string(0), order3=set (always set1,set2,set3 like {1,1,2,3,3,3,4,4} meaning first 2 columns are in set1, then one column from set2, then 3 columns from set3 and two columns from set4), warning: numbering starts with 0; order4=what column
    QList<int> postprocessing_first_column_order[4];//the positions of the first used column of every set in the postprocessing_order-lists; if no column in the set is used a -1 is added to the list
    QList<int> postprocessing_last_column_order[4];//the positions of the last used column of every set in the postprocessing_order-lists; if no column in the set is used a -1 is added to the list
    QList<int> advancement_order,advancement_delta,advancement_ref;
    int * advancement_plus_n;//special (for every set), because different cases exist: -3: no '+n'; -1: just '+n' --> advance by block; -2: '+0' and '+n' --> advance by block, but last used column is kind of reference; >=0: this value is a column that is a reference and has to advance first (at least virtually because it is a reference for the '+n' )
    int advancement_counter;
    int autoscale;
};

void prepareColumnsForProgression(struct csv_multi_import_infos * info);
void gatherDefaultColumnIndices(struct csv_import_infos * info);
void gatherSimilarColumnIndices(struct csv_import_infos * info,int type,int start);
void gatherColumnOrder(struct csv_multi_import_infos * info);//fill the postprocessing-order-entries
void gatherUsedColumnIndices(struct csv_multi_import_infos * info);
void addColumnToUsedIndices(struct csv_multi_import_infos * info, int set, int col);

void init_csv_import_infos(struct csv_import_infos * info);
void clear_csv_import_infos(struct csv_import_infos * info);
void init_mcsv_import_infos(struct csv_multi_import_infos * info);
void clear_mcsv_import_infos(struct csv_multi_import_infos * info);
void copy_csv_import_infos(struct csv_import_infos * target,struct csv_import_infos * source);
void copy_mcsv_import_infos(struct csv_multi_import_infos * target, struct csv_multi_import_infos * source, bool include_data);//if include_data=true --> copy data as well as settings, include_data=false --> copy only the settings
void calc_mcsv_advancement(struct csv_multi_import_infos * mcsv_target, struct csv_multi_import_infos * mcsv_template, int nr);//nr is the number of advancements done (1...n)
void saveCSV_singleSetFormat_ToFile(ofstream & ofi,struct csv_import_infos * cvs);
void saveCSV_multiSetFormat_ToFile(ofstream & ofi,struct csv_multi_import_infos * cvs);
void loadCSV_singleSetFormat_FromFile(ifstream & ifi,struct csv_import_infos * cvs);
void loadCSV_multiSetFormat_FromFile(ifstream & ifi,struct csv_multi_import_infos * cvs);

void saveCSVformatToFile(const char * file,struct csv_import_infos * cvs);
void loadCSVformatFromFile(const char * file,struct csv_import_infos * cvs);
QString return_format_name(int nr);

class frmCSVImporter:public QDialog
{
    Q_OBJECT
public:
    frmCSVImporter(QWidget * parent=0);
    ~frmCSVImporter();
    QGridLayout * layout;
    QPushButton * cmdOpenFile;
    QPushButton * cmdReguess;
    stdLineEdit * ledFileName;
    QTextEdit * lblFileContents;
    stdLineEdit * ledColSep;
    stdLineEdit * ledSetSep;
    stdLineEdit * ledTextSep;
    stdIntSelector * selHeaderLines;
    stdIntSelector * selSetsToRead;
    QCheckBox * chkKommaDecimalSep;
    QCheckBox * chkAutoscaleOnRead;
    QFileDialog * fileDialog;
    QLabel * lblFileCont;
    QLabel * lblGraphSelector;
    uniList * graphselector;
    stdButtonGroup * aac;

    QMenuBar * menuBar;
    QMenu * DataMenu;
    QAction * actOpenFormat,*actSaveFormat,*actClose;

    QScrollArea * scroll;
    QLabel * empty;
    QGridLayout * grid1;
    int allocated_selectors;
    QLabel ** lblDescription;
    StdSelector ** selFormat;
    StdSelector ** selTargetSet;
    StdSelector ** selTargetColumn;

    QStringList FileNames;
    struct csv_import_infos import_info;//the read import settings
//the temporary import settings used for every file (different from the import_info because the number of columns may differ or the target-set-ids may be different)
    struct csv_import_infos import_info_tmp;

    bool exists,readable,writable;
    long size_kB;
public slots:
    void read_inputs(void);
    void read_import_settings(void);
    void recreate_input_selectors(void);
    void doReguess(void);
    void doOpen(void);
    void readFirstLines(void);
    void init(void);
    void headerLinesChanged(int c);
    void setCountChanged(int c);
    void doLoadFormat(void);
    void doSaveFormat(void);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
};

class frmCSVImporterV2:public QDialog
{
    Q_OBJECT
public:
    frmCSVImporterV2(QWidget * parent=0);
    ~frmCSVImporterV2();
    struct csv_import_infos * Template;
    struct csv_multi_import_infos * input;

    multiFileSelector * mfsel;
    QCheckBox * chkGuess;
    StdSelector * selGraph;
    uniList * lstSet;

    QGroupBox * grpContents;
    QTextEdit * txtContent;

    QGridLayout * layout0;
    QGridLayout * layout1;
    QWidget * empty0;
    stdIntSelector * selHeaderLines;
    StdSelector * selColSep,*selLineSep;
    LineWidthSelector * selInvalidVal;
    StdSelector * selDecSep;
    QLabel* lblFormat;
    MultiDataFormatSelector * multDataFormSel;
    //StdSelector * selSourceSet;
    //ColumnTargetSelector * colTargetSel;
    postProcessingSettings * postproset;

    QPushButton *cmdReRead,*cmdGuess,*cmdRead,*cmdReadAll,*cmdReadBlockData;
    stdButtonGroup * aac;

    QMenuBar * menuBar;
    QMenu * DataMenu;
    QMenu * FileViewMenu;
    QAction * actOpenFormat,*actSaveFormat,*actOpenFile,*actClose,*actOpenBlockData;
    QAction * actShowComments,*actShowOriginal,*actShowValues,*actShowErrors,*actShowHeader;
    menuIntSelector * selStartLines,*selStopLines;

    QGridLayout * layout;
    bool ApplyError;
    int guess_type;
public slots:
    virtual void dropEvent(QDropEvent *);
    virtual void dragEnterEvent(QDragEnterEvent *event);
    void init(void);
    void setGUItoInput(bool incl_postprocessing);
    void updateViewSettingsA(bool a);
    void updateViewSettingsB(int a);
    void updateViewSettings(void);
    void doReRead(void);
    void doGuess(void);
    void doRead(void);
    void doReadAll(void);
    void doReadBlockData(void);
    void doOpenEditBlockData(void);
    void newFilesSelection(QStringList l);
    void doOpenFormat(void);
    void doSaveFormat(void);
    void readAllInputs(void);
    void readGeneralSettingsIntoInput(void);
    void readDisplaySetting(void);
    void contentsToggled(bool t);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
signals:

};

class frmProgressWin:public QDialog
{
    Q_OBJECT
public:
    frmProgressWin(QWidget * parent=0);
    ~frmProgressWin();
    QLabel * lblText;
    QProgressBar * progress;
    QVBoxLayout * layout;
public slots:
    void init(QString text,int max);
    void increase(void);
    void setVal(int val);
};

typedef enum {
    QUESTION_RETURN_YES,
    QUESTION_RETURN_NO,
    QUESTION_RETURN_DISCARD,
    QUESTION_RETURN_CANCEL,
    QUESTION_RETURN_SAVE,
    QUESTION_RETURN_SAVEAS,
    QUESTION_RETURN_BAD
} QuestionReturnValues;

class frmQuestionDialog:public QDialog
{
    Q_OBJECT
public:
    frmQuestionDialog(QWidget * parent=0);
    ~frmQuestionDialog();
    QGridLayout * layout;
    QLabel * lblIcon;
    QPushButton * cmdYes;
    QPushButton * cmdNo;
    QPushButton * cmdSave;
    QPushButton * cmdSaveAs;
    QLabel * lblText;
    //QLabel * lblIcon;
public slots:
    void init(QString text,QString title_text,bool show_Save=true,bool show_SaveAs=true,bool show_Yes=true, bool show_No=true);
    void doYes(void);
    void doNo(void);
    void doSave(void);
    void doSaveAs(void);
};

class frmSimpleListSelectionDialog:public QDialog
{
    Q_OBJECT
public:
    frmSimpleListSelectionDialog(QWidget * parent=0);
    ~frmSimpleListSelectionDialog();
    QGridLayout * layout;
    StdSelector * selNames;
    stdLineEdit * ledNewName;
    QPushButton * cmdSelect;
    QPushButton * cmdNew;
    QPushButton * cmdCancel;
    QString return_name;
    int return_nr;
public slots:
    void init(QString * name_list,int nr, bool read);
    void doCancel(void);
    void doSelect(void);
    void doNew(void);
};

class frmReportOnFitParameters:public QDialog
{
    Q_OBJECT
public:
    frmReportOnFitParameters(QWidget * parent=0);
    ~frmReportOnFitParameters();
    QGridLayout * layout;
    QLabel * lblName[2],*lblValue[2];
    LineWidthSelector * spnPara[MAXPARM];
    /*QLabel * lblPara[MAXPARM];
    QDoubleSpinBox * spnPara[MAXPARM];*/
    QPushButton * cmdSet;
    QPushButton * cmdUpdate;
    QPushButton * cmdReport;
    QPushButton * cmdClose;
public slots:
    void init(void);
    void doSet(void);
    void doReport(void);
    void doClose(void);
};

class frmGeometricEvaluation: public QDialog
{
    Q_OBJECT
public:
    frmGeometricEvaluation(QWidget * parent=0);
    ~frmGeometricEvaluation();
    grpSelect * sourceSelect;
    QGridLayout * layout;
    StdSelector * selOption;
    StdSelector * selLoadX;
    StdSelector * selRegion;
    QCheckBox * chkInvert;
    stdButtonGroup * buttons;
public slots:
    void optionChanged(int v);
    void init(void);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
};

int generate_x_mesh_from_formula(int gno,int sno,double start,double stop,int npts,char * formula,int type);
void ParseFilterCommand(char * com,int & o_n_sets,int ** o_gnos,int ** o_snos,int & n_sets,int ** gnos,int ** snos,int & type,int & realization,double * limits,int * orders,char * x_formula,double & ripple,int & absolute,int & debug,int & point_extension,int & oversampling,int & rno,int & invr);
void ParseRegression(char * com,int & n_sets,int ** gnos,int ** snos,int & n_n_sets,int ** n_gnos,int ** n_snos,int & ideg,int & iresid,int & rno,int & invr,double & start,double & stop,int & points,int & rx,char * formula);
int startIfStatement(char * compl_com,int & result,char * real_com);
int split_if_statement(char * compl_com, QList<int> & comp_pos, QStringList & comp_text, int & comparator);
int containsSpecialCommand(char * com,char ** parameters);
int ParseExtractCommand(char * com,char * arg);
int ParseSpecialFormula(char * com,char * arg);
QIcon CreateIconFromPNG(QString png_file, QSize s=QSize(0,0));
QIcon CreateScaledIconFromPNG(QString png_file, QSize s=QSize(0,0));
const char * get_format_option_item_label(int i);

class TestDialog:public QWidget
{
Q_OBJECT
public:
TestDialog(QWidget * parent=0);
QGridLayout * layout;
QPushButton * cmdSetExportName;
QPushButton * cmdSetDocName;
QPushButton * cmdhardcopy;
QPushButton * cmdLoad;
QPushButton * cmdImportSin;
QPushButton * cmdArrangeGr;
QPushButton * cmdCreatePolar2;
QPushButton * cmdTestExamples;
stdLineEdit * lenFile;
stdLineEdit * lenExport;
stdLineEdit * lenDoc;

StdSelector * devices_item;

stdLineEdit * lenDPI;
stdLineEdit * lenSizeX;
stdLineEdit * lenSizeY;

stdLineEdit * lenFormulaTest;
QLabel * lblReplacements;
QLabel * lblReplacedFormula;
QPushButton * cmdReplacements;
QPushButton * cmdExecute;
int g_lhs,g_rhs,s_lhs,s_rhs;
class formula_to_process formula1;

stdLineEdit * lenTestString;
QLabel * lblReReadTest;
QPushButton * cmdTestString;

stdLineEdit * lenTextCodecTest;
QLabel * lblTextCodecTest;
QPushButton * cmdTextCodecTest;

public slots:
    void doLoad(void);
    void doImportSin(void);
    void doExport(void);
    void doDocname(void);
    void doHardcopy(void);
    void doArrange(void);
    void doCreatePolar2(void);
    void doSetReplacements(void);
    void doExecuteFormula(void);
    void doTestExamples(void);
    void doStringSaveLoadTest(void);
    void doTextCodecTest(void);
};

#define MAX_SETUP_OPTIONS 7

class SetupWizard:public QDialog
{
Q_OBJECT
public:
    SetupWizard(QWidget * parent=0);
    ~SetupWizard();
    int page_number,max_pages;
    QLabel * lblPicture;
    QLabel * title1;
    QLabel * title2;
    QLabel * title3;
    QRadioButton * optOptions[MAX_SETUP_OPTIONS];
    QCheckBox * chkOptions[MAX_SETUP_OPTIONS];
    stdLineEdit * ledOptions[MAX_SETUP_OPTIONS];
    QGridLayout * grid;
    QPushButton * cmdPrev;
    QPushButton * cmdNext;
    QPushButton * cmdCancel;
public slots:
    virtual void closeEvent( QCloseEvent * event );
    void init(void);
    void doCancel(void);
    void doPrev(void);
    void doNext(void);
    void reinitPage(void);//show contents suitable to page number
    void doApplySettings(void);
    void setNumberOfOptions(int nr, QString * optentries,int nr2, QString * chkentries,int nr3, QString * ledentries);
};

class WizardIntroPage:public QWizardPage
{
Q_OBJECT
public:
    WizardIntroPage(QWidget * parent=0);
};

class WizardUndoPage:public QWizardPage
{
Q_OBJECT
public:
    WizardUndoPage(QWidget * parent=0);
};

class WizardImmediateUpdatePage:public QWizardPage
{
Q_OBJECT
public:
    WizardImmediateUpdatePage(QWidget * parent=0);
};

class WizardDecSepPage:public QWizardPage
{
Q_OBJECT
public:
    WizardDecSepPage(QWidget * parent=0);
};

class WizardLatexPage:public QWizardPage
{
Q_OBJECT
public:
    WizardLatexPage(QWidget * parent=0);
};

class WizardQtFontsPage:public QWizardPage
{
Q_OBJECT
public:
    WizardQtFontsPage(QWidget * parent=0);
};

class WizardMouseWheelPage:public QWizardPage
{
Q_OBJECT
public:
    WizardMouseWheelPage(QWidget * parent=0);
};

class WizardTextEditPage:public QWizardPage
{
Q_OBJECT
public:
    WizardTextEditPage(QWidget * parent=0);
};

class WizardContextMenuPage:public QWizardPage
{
Q_OBJECT
public:
    WizardContextMenuPage(QWidget * parent=0);
};

class WizardHighlightPage:public QWizardPage
{
Q_OBJECT
public:
    WizardHighlightPage(QWidget * parent=0);
};

class WizardAutoscalePage:public QWizardPage
{
Q_OBJECT
public:
    WizardAutoscalePage(QWidget * parent=0);
    QDoubleSpinBox * spnAutoScaleRange;
};

class WizardListIconsPage:public QWizardPage
{
Q_OBJECT
public:
    WizardListIconsPage(QWidget * parent=0);
};

class WizardUniListSettingsPage:public QWizardPage
{
Q_OBJECT
public:
    WizardUniListSettingsPage(QWidget * parent=0);
};

class WizardLibFFTW3Page:public QWizardPage
{
Q_OBJECT
public:
    WizardLibFFTW3Page(QWidget * parent=0);
    stdLineEdit * ledLibraryPath;
    QLabel * lblUsable;
public slots:
    void doBrowse(void);
};

class WizardOutputPage;

class WizardLibHaruPage:public QWizardPage
{
Q_OBJECT
public:
    WizardLibHaruPage(QWidget * parent=0);
    stdLineEdit * ledLibraryPath;
    QLabel * lblUsable;
class WizardOutputPage * outPage;
public slots:
    virtual bool validatePage();
    void doBrowse(void);
};

class WizardToolBarPage:public QWizardPage
{
Q_OBJECT
public:
    WizardToolBarPage(QWidget * parent=0);
};

class WizardToolBarPage2:public QWizardPage
{
Q_OBJECT
public:
    WizardToolBarPage2(QWidget * parent=0);
};

class WizardStartUpPage:public QWizardPage
{
Q_OBJECT
public:
    WizardStartUpPage(QWidget * parent=0);
    QPushButton * cmdBrowse;
    stdLineEdit * lenAbsoluteFile;
    QLabel * lblResult;
    QRadioButton * radAbsolute;
    QRadioButton * radRelative;
public slots:
    void doBrowse(void);
    void changeSaving(bool t);
};

class WizardOutputPage:public QWizardPage
{
Q_OBJECT
public:
    WizardOutputPage(QWidget * parent=0);
    StdSelector * devices_item;
public slots:
    virtual void initializePage();
    virtual void cleanupPage();
};

class WizardBehaviorPage:public QWizardPage
{
Q_OBJECT
public:
    WizardBehaviorPage(QWidget * parent=0);
};

class WizardCompressionPage:public QWizardPage
{
Q_OBJECT
public:
    WizardCompressionPage(QWidget * parent=0);
};

class WizardFinishPage:public QWizardPage
{
Q_OBJECT
public:
    WizardFinishPage(QWidget * parent=0);
};

class InitQtGraceWizard:public QWizard
{
Q_OBJECT
public:
    InitQtGraceWizard(QWidget * parent=0);
    ~InitQtGraceWizard();
    WizardStartUpPage * startuppage;
    WizardAutoscalePage * autoscale;
public slots:
    void initAllPages(void);
    void setGlobalsToWizardValues(void);
    void PageChanged(int p);
    void done(int result);
    virtual void closeEvent(QCloseEvent * e);
};

class graphicTestWidget:public QWidget
{
Q_OBJECT
public:
    graphicTestWidget(QWidget * parent=0);
    QGraphicsView * g_view;
    QGraphicsScene * g_scene;
    QGraphicsProxyWidget * g_proxy;
    QLineEdit * lenEdit;
    QPalette transpPalette;
    QLabel * lbltest;
    QLabel * lblTitles[5];
    QSpinBox * spnKoords[4];
    QComboBox * cmbAlign;
    QPushButton * push1;
    QPushButton * push2;
    int ppoint;
    QPoint points[3];
public slots:
    void doPush1(void);
    void doPush2(void);
    void value_changed(int i);
    void position_text(int xp,int yp);
};

class frmGroupTestWidget:public QWidget
{
Q_OBJECT
public:
    frmGroupTestWidget(QWidget * parent);
    QGroupBox * grp_1;
    QLabel * lbltest1[4];
    QLineEdit * lentest1[4];
    QGridLayout * layout1;
    QGroupBox * grp_2;
    QGridLayout * layout2;
    QGroupBox * grp_3;
    QLabel * lbltest3[4];
    QLineEdit * lentest3[4];
    QGridLayout * layout3;
    QGroupBox * grp_4;
    QLabel * lbltest4[4];
    QLineEdit * lentest4[4];
    QGridLayout * layout4;
    QPushButton * push;
QVBoxLayout * layout;
int delta;
public slots:
    void doClick(void);
};

//guess=0: no guessing; guess=1: full guessing (guess everything), guess=2: guess column-formats, general settings stay untouched (like decimal-separator or line-endings)
int read_and_guess_csv_file(char * filename,int nr_of_header_lines,QStringList & ignoredHeaderLines, char &guessed_line_sep, char &guessed_dec_sep, QList<int> &separate_positions1, QList<int> &separate_positions2, QList<char> &guessed_separator, int & lines, int guess);
int read_csv_file_into_tokens(struct csv_import_infos & info,int guess);//data from file will be stored in info as tokens
int read_csv_file_into_sets(struct csv_multi_import_infos & minfo,int guess);
int autoload_csv_file(struct csv_multi_import_infos & minfo, int guess);//just read everything in a file (filename is specified in minfo) into XY-Sets
int get_csv_column(csv_import_infos & info, int col, double * v);
int get_csv_column(csv_multi_import_infos & minfo, int setid, int col, double * v);
int get_csv_string_column(csv_import_infos & info, int col, QStringList & slist);
int get_csv_string_column(csv_multi_import_infos & minfo, int setid, int col, QStringList & slist);
int constructSetFromCSVWithPostprocessing(csv_multi_import_infos & minfo, QList<int> & target_graph, QList<int> & target_set, QList<int> &usedTargetSets);
int constructSetFromCSV(csv_multi_import_infos & minfo, int source_setid, int target_graph, int & target_set,int target_set_type,QList<int> source_columns);
QString convert_csv_contents_to_html(csv_import_infos & info);//, int startline, int maxlines,int endlines, bool include_comments);
QString convert_csv_contents_to_html(csv_multi_import_infos & info);//, int startline, int maxlines,int endlines, bool include_comments);
QString convert_csv_errors_to_html(csv_import_infos & info);
QString convert_csv_errors_to_html(csv_multi_import_infos & info);
QString convert_csv_errors_to_text(csv_import_infos & info);
QString convert_csv_errors_to_text(csv_multi_import_infos & info);
/// target_type=
/// 0: new set(s) (target-graph is stored in info, target-sets we be available in info afterwards)
/// 1: block-data
/// 2: html-type-string (in info)
int write_csv_contents_to_target(csv_import_infos & info,int target_type);
int simple_read_data_from_ascii_file(csv_import_infos & info);

#endif

