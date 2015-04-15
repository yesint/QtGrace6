/***************************************************************************
 *   Copyright (C) 2008-2015 by Andreas Winter                             *
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
#include "globals.h"
#include "parser.h"
#include "ssdata.h"
#include "defines.h"
#include "t1fonts.h"

#define STD_MARGIN 2
#define STD_SPACING 2

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

#define SAMPLING_MESH   0
#define SAMPLING_SET    1

#define READ_SET_FORM 0
#define READ_NETCDF_FORM 1
#define READ_PROJECT_FORM 2
#define WRITE_SET_FORM 3
#define WRITE_PROJECT_FORM 4
#define SELECT_HOT_LINK_FILE 5
#define READ_PARAMETERS 6
#define WRITE_PARAMETERS 7
#define SELECT_PRINT_FILE 8
#define WRITE_FIT_PARAM 9
#define READ_FIT_PARAM 10
#define READ_BINARY_FILE 11
#define WRITE_BINARY_FILE 12

#define INTERPOLATIONWINDOW 0
#define HISTOGRAMSWINDOW 1

#define DEFINEREGIONWINDOW 0
#define REPORTREGIONWINDOW 1
#define CLEARREGIONWINDOW 2

#define INTERPOLATIONWINDOW 0
#define HISTOGRAMSWINDOW 1

#define INTEGRATIONWINDOW 0
#define SEASONALWINDOW 1
#define DIFFERENCESWINDOW 2
#define AVERAGESWINDOW 3
#define REGRESSIONWINDOW 4

#define CORRELATIONWINDOW 0
#define CONVOLUTIONWINDOW 1
#define FILTERWINDOW 2

#define PRUNEWINDOW 0
#define SAMPLEPOINTSWINDOW 1
#define GEOMETRICWINDOW 2

#define SPECIAL_NONE 0
#define SPECIAL_FILTER 1
#define SPECIAL_REGRESSION 2
#define SPECIAL_REMEMBER 3
#define SPECIAL_ADD 4
#define SPECIAL_MINUS 5
#define SPECIAL_DIVIDE 6
#define SPECIAL_MULTIPLY 7
#define SPECIAL_USE 8
#define SPECIAL_EXTRACT 9
#define SPECIAL_FORMULA 10

using namespace std;

class frmEditBlockData;
class MainWindow;
class frmIOForm;

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

    QGridLayout * layout;
public slots:
    void update_number_of_entries(void);
    void mark_single_set(int gno,int setno);
    void mark_multiple_sets(int gno,int nsets,int * setnos);
    void set_graph_nr(int gno);
};

class frmEditColumnProp:public QDialog
{
    Q_OBJECT
public:
    frmEditColumnProp(QWidget * parent=0);

    int col_format[6];
    int col_precision[6];
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
    frmEditColumnProp * EditColumn;
    int col_format[6];
    int col_precision[6];
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
    QVBoxLayout * layout0;
    stdButtonGroup * buttonGroup;
    QVBoxLayout * layout;
    //SetTableView * table;
    QTableView * table;
    SetTableModel * model;
public slots:
    void CreateActions(void);
    void HHeaderClicked(int i);
    void init(int g_no,int s_no);
    void setColumnFormat(int col,int format,int precision);
    void changeDataSet(int type);
    void itemChanged(int r,int c,bool realy_new_value);
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

class frmPointExplorer:public QDialog
{
    Q_OBJECT
public:
    frmPointExplorer(QWidget * parent=0);
    int gno,sno,loc_pos;
    QLabel * lblInstructions;
    QLabel * lblRestrToSet;
    uniList * list;
    stdLineEdit * ledPointLocation;
    stdLineEdit * ledPointData;

    QGroupBox * grpButtons;
    QHBoxLayout * layout1;
    QPushButton * cmdGoTo;
    QPushButton * cmdTrack;
    QPushButton * cmdMove;
    QPushButton * cmdMoveX;
    QPushButton * cmdMoveY;
    QPushButton * cmdPrepend;
    QPushButton * cmdAppend;
    QPushButton * cmdInsert;
    QPushButton * cmdDelete;
    QPushButton * cmdClose;

    QGridLayout * layout;
public slots:
    void selectionChanged(int a);
    void init(void);
    void doGoTo(void);
    void doTrack(void);
    void update_point_locator(int gno, int setno, int loc);
    void doMove(void);
    void doMoveX(void);
    void doMoveY(void);
    void doPrepend(void);
    void doAppend(void);
    void doInsert(void);
    void doDelete(void);
    void doClose(void);
};

class frmText_Props:public QWidget
{
    Q_OBJECT
public:
    bool editWindow;
    int obj_id;
    frmText_Props(QWidget * parent=0,bool edit=false);
    stdLineEdit * string_item;
    FontSelector * strings_font_item;
    ColorSelector * strings_color_item;
    JustificationSelector * strings_just_item;
    PositionSelector * strings_loc_item;
    stdLineEdit * ledCoords[2];
    stdSlider * strings_rot_item;
    stdSlider * strings_size_item;
    stdButtonGroup * buttonGroup;
    QVBoxLayout * layout;
public slots:
    void init(int id);
    void viewCoordsChanged(int i);
    void doAccept(void);
    void doClose(void);
    //immidiate updates
    void update0(void);
    void update1(int v);
    void update2(QString v);
    void update3(bool v);
    void update4(double v);
};

class frmTextProps:public QDialog
{
    Q_OBJECT
public:
    frmTextProps(QWidget * parent=0,bool edit=false);
    frmText_Props * flp;
public slots:
    void init(int id);
    void doAccept(void);
    void doClose(void);
};

class frmLine_Props:public QWidget
{
    Q_OBJECT
public:
    int obj_id;
    bool editWindow;
    frmLine_Props(QWidget * parent=0,bool edit=false);

    ColorSelector * lines_color_item;
    LineWidthSelector * lines_width_item;
    LineStyleSelector * lines_style_item;
    PositionSelector * lines_loc_item;
    stdLineEdit * ledCoords[4];
    stdButtonGroup * buttonGroup;

    QGroupBox * fraArrow;
    StdSelector * lines_arrow_item;
    StdSelector * lines_atype_item;
    LineWidthSelector * lines_asize_item;
    LineWidthSelector * lines_a_dL_ff_item;
    LineWidthSelector * lines_a_lL_ff_item;

    QVBoxLayout * layout;
    QVBoxLayout * layout2;
public slots:
    void init(int id);
    void viewCoordsChanged(int i);
    void doAccept(void);
    void doClose(void);
    //immidiate updates
    void update0(void);
    void update1(int v);
    void update2(QString v);
    void update3(bool v);
    void update4(double v);
};

class frmLineProps:public QDialog
{
    Q_OBJECT
public:
    frmLineProps(QWidget * parent=0,bool edit=false);
    frmLine_Props * flp;
public slots:
    void init(int id);
    void doAccept(void);
    void doClose(void);
};

class frmEllipse_Props:public QWidget
{
    Q_OBJECT
public:
    int obj_id;
    bool editWindow;
    bool ellipse;//true --> it is really an ellipse
    //false --> it is a box
    frmEllipse_Props(QWidget * parent=0,bool edit=false,bool ellip=false);

    ColorSelector * ellip_color_item;
    LineWidthSelector * ellip_linew_item;
    LineStyleSelector * ellip_lines_item;
    FillPatternSelector * ellip_fillpat_item;
    ColorSelector * ellip_fillcol_item;
    PositionSelector * ellip_loc_item;
    stdSlider * ellip_rot_item;
    stdLineEdit * ledCoords[4];
    stdButtonGroup * buttonGroup;

    QVBoxLayout * layout;
public slots:
    void init(int id);
    void viewCoordsChanged(int i);
    void doAccept(void);
    void doClose(void);
    //immidiate updates
    void update0(void);
    void update1(int v);
    void update2(QString v);
    void update3(bool v);
    void update4(double v);
};

class frmEllipseProps:public QDialog
{
    Q_OBJECT
public:
    frmEllipseProps(QWidget * parent=0,bool edit=false,bool ellip=false);
    frmEllipse_Props * flp;
public slots:
    void init(int id);
    void doAccept(void);
    void doClose(void);
};

class frmDrawObjects:public QDialog
{
    Q_OBJECT
public:
    frmDrawObjects(QWidget * parent=0);
    ~frmDrawObjects();

    QPushButton * cmdText;
    QPushButton * cmdTextProp;
    QPushButton * cmdLine;
    QPushButton * cmdLineProp;
    QPushButton * cmdBox;
    QPushButton * cmdBoxProp;
    QPushButton * cmdEllipse;
    QPushButton * cmdEllipseProp;
    QPushButton * cmdEditObj;
    QPushButton * cmdMoveObj;
    QPushButton * cmdCopyObj;
    QPushButton * cmdDelObj;
    QPushButton * cmdClearAllText;
    QPushButton * cmdClearAllLines;
    QPushButton * cmdClearAllBoxes;
    QPushButton * cmdClearAllEllipses;
    QPushButton * cmdClose;
    QGridLayout * layout;

public slots:
    void doText(void);
    void doTextProp(void);
    void doLine(void);
    void doLineProp(void);
    void doBox(void);
    void doBoxProp(void);
    void doEllipse(void);
    void doEllipseProp(void);
    void doEditObj(void);
    void doMoveObj(void);
    void doCopyObj(void);
    void doDelObj(void);
    void doClearAllText(void);
    void doClearAllLines(void);
    void doClearAllBoxes(void);
    void doClearAllEllipses(void);
    void doClose(void);
};

class frmPlot_Appearance:public QWidget
{
    Q_OBJECT
public:
    frmPlot_Appearance(QWidget * parent=0);
    ~frmPlot_Appearance();

    QGroupBox * fraPageBackgr;
    QGroupBox * fraTimeStamp;
    QGroupBox * fraDescription;
    QTextEdit * txtDescription;
    FontSelector * timestamp_font_item;
    ColorSelector * bg_color_item;
    ColorSelector * timestamp_color_item;
    QCheckBox * bg_fill_item;
    QCheckBox * timestamp_active_item;
    QCheckBox * chkPathStamp;
    stdLineEdit * ledStampCoords[2];
    stdSlider * timestamp_rotate_item;
    stdSlider * timestamp_size_item;
    stdButtonGroup * buttonGroup;

    QVBoxLayout * layout;
    QHBoxLayout * layout1;
    QVBoxLayout * layout2;
    QVBoxLayout * layout3;
public slots:
    void update0(void);
    void update1(int v);
    void update2(QString v);
    void update3(bool v);

    void doApply(void);
    void doAccept(void);
    void doClose(void);
    void init(void);
};

class frmPlotAppearance:public QDialog
{
    Q_OBJECT
public:
    frmPlotAppearance(QWidget * parent=0);
    frmPlot_Appearance * flp;
public slots:
    void init(void);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
};

class frmLocatorProps:public QDialog
{
    Q_OBJECT
public:
    frmLocatorProps(QWidget * parent=0);
    ~frmLocatorProps();
    QGroupBox * fraXProp;
    QGroupBox * fraYProp;
    QGroupBox * fraFixedPoint;
    StdSelector * selLocDisplType;
    StdSelector * selXFormat;
    StdSelector * selXPrecision;
    StdSelector * selYFormat;
    StdSelector * selYPrecision;
    QCheckBox * chkEnableFixed;
    stdLineEdit * ledFixedCoords[2];
    stdButtonGroup * buttonGroup;

    QVBoxLayout * layout;
    QVBoxLayout * layout1;
    QVBoxLayout * layout2;
    QGridLayout * layout3;
public slots:
    void doApply(void);
    void doAccept(void);
    void doClose(void);
    void init(void);
    int locator_define_notify_proc(void);
    void update_locator_items(int gno);
};

class frmConsole:public QDialog
{
    Q_OBJECT
public:
    frmConsole(QWidget * parent=0);
    ~frmConsole();

    QLabel * lblMessages;
    QTextEdit * txtMessages;
    QVBoxLayout *layout;
    QMenuBar * menuBar;
    QMenu * mnuFile;
    QMenu * mnuEdit;
    QMenu * mnuOptions;
    QMenu * mnuHelp;

    QAction * actSave,*actClose,*actCopy,*actHelpOnContext,*actHelpOnConsole,*actPopOnError,*actClear;

public slots:
    void CreateActions(void);
    void doSave(void);
    void doClose(void);
    void doCopy(void);
    void doClear(void);
    void doHelpOnContext(void);
    void doHelpOnConsole(void);
    void doPopOnError(void);
    void errwin(const char *msg);
    void msgwin(const char *msg);
};

class frmSetOp:public QDialog
{
    Q_OBJECT
public:
    frmSetOp(QWidget * parent=0);

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

    stdLineEdit * ledLength;
    stdLineEdit * ledStart;
    stdLineEdit * ledStop;

    QAction *actClose,*actHelpOnContext,*actHelpOnSetOp;

    stdButtonGroup * buttonGroup;
    QGridLayout * layout;
public slots:
    void init(void);
    void changeSelection(int i);
    void CreateActions(void);
    void doClose(void);
    void doAccept(void);
    void doApply(void);
    void doHelpOnContext(void);
    void doHelpOnSetOp(void);
};

class frmCommands:public QDialog
{
    Q_OBJECT
public:
    frmCommands(QWidget * parent=0);

    grpSelect * grpSource;
    grpSelect * grpDestination;
    QPushButton * cmdReplayWithReplace;
    QLabel * lblCommand;
    QLineEdit * lenCommand;
    QListWidget * list;
    QGroupBox * grpBox1, * grpBox2, * grpBox3;
    QHBoxLayout * layout1,*layout2;
    QVBoxLayout * layout;
    QGridLayout *layout3;
    QGroupBox * grpSpecial;
    QHBoxLayout * layout4;
    QLabel * lblSpecial;
    QComboBox * cmbSpecial;
    QComboBox * cmbSpecial2;
    QPushButton * cmdSpecial;

    QPushButton * cmdAdd;
    QPushButton * cmdDelete;
    QPushButton * cmdReplace;
    QPushButton * cmdUp;
    QPushButton * cmdDown;
    QPushButton * cmdRead;
    QPushButton * cmdSave;
    QPushButton * cmdClear;
    QPushButton * cmdReplay;
    QPushButton * cmdClose;
    QPushButton * cmdHelp;

    frmIOForm * FormReadHistory;
    frmIOForm * FormWriteHistory;

public slots:
    void doAdd(void);
    void doDelete(void);
    void doReplace(void);
    void doUp(void);
    void doDown(void);
    void doRead(void);
    void doSave(void);
    void doClear(void);
    void doReplay(void);
    int special_Scanner(char * command,bool replace);
    void doReplayWithReplace(void);
    void doInsertSpecial(void);
    void Special1Changed(int nr);
    void Special2Changed(int nr);
    void doClose(void);
    void doHelp(void);
    void doDoubleClick(QListWidgetItem* index);
    void getListSelection(int * number,int ** selection);
    void IOrequested(int type,QString file,bool exists,bool writeable,bool readable);
    int next_unused_new_set(void);
};

class frmDeviceOptions:public QDialog
{
    Q_OBJECT
public:
    frmDeviceOptions(int device,QWidget * parent=0);
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

class frmDeviceSetup:public QDialog
{
    Q_OBJECT
public:
    frmDeviceSetup(QWidget * parent=0);

    QPushButton * device_opts_item;
    QPushButton * wbut;
    QGroupBox * grpDevSetup;
    QGroupBox * grpOutput;
    QGroupBox * grpPage;
    QGroupBox * grpFonts;
    QMenu * mnuFile;
    QMenu * mnuOptions;
    QMenu * mnuHelp;
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
    QPushButton * cmdNativePrinterDialog;
    QPushButton * cmdDoPrint;

    QCheckBox * printto_item;
    QCheckBox * fontaa_item;
    QCheckBox * devfont_item;

    frmDeviceOptions * DevOptions[6];
    int cur_dev,cur_version;
    int parent_of_print_dialog;//0=this,1=MainWindow
    char * out_format;
    char out_format_int[5];
    char out_format_float[5];
    QAction * actPrintToFile,*actClose,*dsync_item,*psync_item,*actHelpOnDevSetup,*actHelpOnContext,*actNativePrinterDialog;
    QPrintDialog * printDialog;
    ofstream ofi;
    Device_entry curdev,tmp_dev;

    stdButtonGroup * buttonGroup;

    QHBoxLayout * layout0;
    QGridLayout * layout1;
    QGridLayout * layout2;
    QHBoxLayout * layout3;
    QVBoxLayout * layout;
public slots:
    void init(int dev);
    void CreateActions(void);
    void PrintToFileClicked(int i);
    void DeviceChanged(int i);
    void SizeChanged(int i);
    void OrientationChanged(int i);
    void DimChanged(int i);
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
    bool openNativePrinter(int dev);
    void printerAccepted(QPrinter * pri);
    void printerRejected(void);
    void dpiInputChanged(QString text);
    void tryPrintingOnPrinter(QPrinter * printer);
    void doNativePrinterDialog(void);
    void replaceFileNameOnly(QString nname);//this assumes that nname is only the name without a path and without a suffix
    void IOrequested(int type,QString file,bool exists,bool writeable,bool readable);
    virtual void closeEvent(QCloseEvent * e);
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
    QSignalMapper * mapper;

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

class frmPreferences:public QWidget
{
    Q_OBJECT
public:
    frmPreferences(QWidget * parent=0);

    QGroupBox * grpResponciveness;
    QCheckBox * noask_item;//chkDontAskQuestions;
    QCheckBox * dc_item;//chkAllowDoubleClick;
    StdSelector * graph_focus_choice_item;
    QCheckBox * graph_drawfocus_choice_item;//chkDisplayFocus;
    QCheckBox * autoredraw_type_item;
    QCheckBox * cursor_type_item;
    QGroupBox * grpRestrictions;
    stdIntSelector * max_path_item;
    QCheckBox * safe_mode_item;
    QGroupBox * grpScrollZoom;
    stdSlider * scrollper_item;
    stdSlider * shexper_item;
    QCheckBox * linkscroll_item;
    QGroupBox * grpDates;
    StdSelector * hint_item;
    stdLineEdit * date_item;
    stdLineEdit * wrap_year_item;
    QCheckBox * two_digits_years_item;

    //StdSelector * selLanguage;

    stdButtonGroup * buttonGroup;

    QVBoxLayout * layout0;
    QVBoxLayout * layout1;
    QVBoxLayout * layout2;
    QGridLayout * layout3;
    QVBoxLayout * layout;
public slots:
    void init(void);
    void toggleWrapYear(int i);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
    void doExtraPreferences(void);
    void props_define_notify_proc(void);
    void update_props_items(void);
signals:
    void close_wish(void);
};

class frmExtraPreferences:public QWidget
{
    Q_OBJECT
public:
    frmExtraPreferences(QWidget * parent=0);

    QMenu * mnuFile;
    QMenuBar * menuBar;
    QAction * actClose,*actLoad,*actSave;

    //General Behavior of QtGrace
    QGroupBox * grp_Behavior;
    QGridLayout * layout0;
    StdSelector * selCodec;
    QCheckBox * chkActivateLaTeXSupport;
    QCheckBox * chkQtFonts;
    QCheckBox * chkSymbolSpecial;
    StdSelector * selDecSep;

    QCheckBox * chkUsePrintCommand;
    stdLineEdit * lenPrintCommand;
    QCheckBox * chkAutoSetAgr;
    QCheckBox * chkAutoSetExport;
    QCheckBox * chkAutoSetCWD;
    QCheckBox * chkWarnOnEncodingChange;
    QCheckBox * chkHDPrinterOutput;
    //FontSelector * defaultFont;
    //stdLineEdit * lenHome;//-->deleted

    //Settings for the QtGrace-Gui
    QGroupBox * grp_Gui;
    QVBoxLayout * layout1;
    QCheckBox * chkExternalHelpViewer;
    stdLineEdit * lenHelpViewer;
    QCheckBox * chkShowHideException;
    QCheckBox * chkImmediateUpdate;
    QCheckBox * chkNewIcons;
    //QCheckBox * chkNewPrintDialog;
    stdIntSelector * histSize;
    LineWidthSelector * selFontSize;//actually just a double-value-selector to change the general font size
    QLabel * lblGuiFont;
    QPushButton * cmdSelGuiFont;
    QPushButton * cmdResetGuiFont;
    QLabel * lblBackground_Color_Text;
    QPushButton * cmdSelGUIBGColor;
    QPushButton * cmdSetGUIBGColor_to_PageBG;
    QPushButton * cmdSetGUIBGColor_to_Std;

    QVBoxLayout * layout;
    stdButtonGroup * buttonGroup;

public slots:
    void init(void);
    void toggleHTMLviewer(int entry);
    void toggleQtFonts(bool check);
    void doLoad(void);
    void doSave(void);
    void doApply(void);
    void doAccept(void);
    void doClose(void);

    void toggleNewIcons(bool val);
    void changeGUIFont(void);
    void resetGUIFont(void);
    void select_BG_Color(void);
    void set_BG_Color_to_Std(void);
    void set_BG_Color_to_Page_BG(void);
signals:
    void close_wish(void);
};

class frmQtGracePrefs2:public QWidget
{
Q_OBJECT
public:
frmQtGracePrefs2(QWidget * parent=0);

//QLabel * lblExtLibs;
QGroupBox * grp_libFFTW3;
QCheckBox * chkUseFFTW3;
QLabel * lblFFTW3_found;
QLabel * lblfftw3_static;
stdLineEdit * ledFFTW3_dll;
QPushButton * cmdBrowseFFTW3;
QGridLayout * layout0;

QGroupBox * grp_libHaru;
QCheckBox * chkUselibHaru;
QLabel * lblHaru_found;
QLabel * lblHaru_static;
stdLineEdit * ledHaru_dll;
QPushButton * cmdBrowseHaru;
QGridLayout * layout1;

QGroupBox * grp_tool_bar;
QLabel * lblToolBar;
QCheckBox * chkShowNavi;
QCheckBox * chkShowGraph;
QCheckBox * chkShowSpecZoom;
QCheckBox * chkShowViewp;
QCheckBox * chkShowPageZoom;
QCheckBox * chkShowPrintB;
QCheckBox * chkShowExportP;
QLabel * lblStatusBar;
QCheckBox * chkShowHostName;
QCheckBox * chkShowDisplay;
StdSelector * selFileDisplay1;
StdSelector * selFileDisplay2;
QPushButton * cmdGraceDefaults;
QPushButton * cmdQtGraceDefaults;
QPushButton * cmdActDevs;
frmDeviceActivator * diaDevAct;
QGridLayout * layout2;

//Startup-Settings
QGroupBox * grp_Startup;
QVBoxLayout * layout3;
QLabel * lblSelStartup;
QLabel * lblStartupWarning;
StdSelector * selLanguage;
stdIntSelector * selStdDpi;
stdIntSelector * selStartupX;
stdIntSelector * selStartupY;
stdIntSelector * selStartupWidth;
stdIntSelector * selStartupHeight;
stdLineEdit * lenDefaultFile;
StdSelector * selDefaultPrintDevice;
QPushButton * cmdBrowseForDefault;
QPushButton * cmdStartupCurrent;

QVBoxLayout * layout;
stdButtonGroup * buttonGroup;
public slots:
    void doBrowseFFTW3_dll(void);
    void doBrowseHaru_dll(void);
    void init(void);
    void read_settings(void);
    void doGraceDefaults(void);
    void doQtGraceDefaults(void);
    void doActDevs(void);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
    void doBrowse(void);
    void doCurrentAsStartup(void);
signals:
    void close_wish(void);
};

class frmDefaults:public QWidget
{
Q_OBJECT
public:
frmDefaults(QWidget * parent=0);

//Grace-Defaults
QGroupBox * grp_defaults;
QGridLayout * layout0;
ColorSelector * selStdColor;
ColorSelector * selBGColor;
FillPatternSelector * selStdPattern;
LineStyleSelector * selLineStyle;
LineWidthSelector * selLineWidth;
stdSlider * sldStdCharSize;
FontSelector * selStdFont;
stdSlider * sldStdSymSize;
stdLineEdit * lenDefaultFormat;
QPushButton * cmdUseThisForCurrent;
QPushButton * cmdUseThisForProjectFile;
QVBoxLayout * layout;
QLabel * lblviewp;
stdLineEdit * selviewp[4];

StdSelector * selShowDefaults;
defaults show_defaults;
view show_view;
char show_sformat[32];

stdButtonGroup * buttonGroup;

public slots:
    void init(void);
    void readDefaultSettings(void);
    void doSetCurrent(void);
    void doSetFile(void);
    void doApply(void);
    void doAccept(void);
    void doClose(void);

    void show_current_defaults(void);
    void currentShowDefaultsChanged(int index);
signals:
    void close_wish(void);
};

class frmColorManagement;

class frm_Preferences:public QDialog
{
    Q_OBJECT
public:
    frm_Preferences(QWidget * parent=0);

    QTabWidget * tabs;

    frmPreferences * tab_prefs;
    frmExtraPreferences * tab_extra;
    frmQtGracePrefs2 * tab_qtgrace_prefs2;
    tabLinestyles * tab_linestyles;
    frmColorManagement * tab_colors;
    frmDefaults * tab_defaults;
    QVBoxLayout * vbox;
public slots:
    void init(void);
    void tab_changed(int nr);
    void redisplayContents(void);
    void doClose(void);
};

void showSetInSpreadSheet(int gno,int setno);
void deleteSpreadSheet(int gno,int setno);

class frmArrangeGraphs:public QDialog
{
    Q_OBJECT
public:
    frmArrangeGraphs(QWidget * parent=0);

    QLabel * lblArrGraphs;
    uniList * graphlist;

    QGroupBox * grpArrGraphs;
    QGroupBox * grpMatrix;
    QGroupBox * grpPageOffset;
    QGroupBox * grpSpacing;

    stdIntSelector * selCols;
    stdIntSelector * selRows;
    QDoubleSpinBox * selUpperOffset;
    QDoubleSpinBox * selLowerOffset;
    QDoubleSpinBox * selLeftOffset;
    QDoubleSpinBox * selRightOffset;
    LineWidthSelector * selHGap;
    LineWidthSelector * selVGap;
    OrderSelector * selOrder;

    QCheckBox * chkAddGraphs;
    QCheckBox * chkKillGraphs;
    QCheckBox * chkSnakeFill;
    QCheckBox * chkHPack;
    QCheckBox * chkVPack;

    stdButtonGroup * buttonGroup;

    QVBoxLayout * layout0;
    QHBoxLayout * layout1;
    QGridLayout * layout2;
    QHBoxLayout * layout3;
    QVBoxLayout * layout;
public slots:
    void init(void);
    void doApply(void);
    void doClose(void);
    void doAccept(void);
};

class frmOverlayGraphs:public QDialog
{
    Q_OBJECT
public:
    frmOverlayGraphs(QWidget * parent=0);

    QLabel * lblOverlayGraph;
    uniList * listOverlayGraph;
    QLabel * lblOntoGraph;
    uniList * listOntoGraph;

    StdSelector * selSmartAxisHint;
    stdButtonGroup * buttonGroup;
    QVBoxLayout * layout;
public slots:
    void init(void);
    void doApply(void);
    void doClose(void);
    void doAccept(void);
};

class frmAutoscaleGraphs:public QDialog
{
    Q_OBJECT
public:
    frmAutoscaleGraphs(QWidget * parent=0);
    StdSelector * selAutoscale;
    StdSelector * selApplyToGraph;
    QLabel * lblUseSet;
    uniList * listSets;
    stdButtonGroup * buttonGroup;
    QVBoxLayout * layout;
public slots:
    void init(void);
    void doApply(void);
    void doClose(void);
    void doAccept(void);
    void define_autos(int aon, int au, int ap);
};

class frmDataSetProperties:public QDialog
{
    Q_OBJECT
public:
    frmDataSetProperties(QWidget * parent=0);
    int gno,sno;
    QMenuBar * menuBar;
    QMenu * mnuFile;
    QMenu * mnuEdit;
    QMenu * mnuHelp;
    QMenu * mnuEditData;
    QMenu * mnuCreateNew;

    QAction * actClose,* actKillData, * actDuplicate,* actSetAppearance,*actSetOperations,* actHelpOnContext,*actHelpOnSetProp;
    QAction * actEditInSpreadsheet,*actEditInTextEditor,*actCreateNewByFormula,*actCreateNewInSpreadsheet,*actCreateNewInTextEditor,*actCreateNewFromBlockData;

    StdSelector * selType;
    stdLineEdit * ledLength;
    stdLineEdit * ledComment;
    QGroupBox * grpStatistics;
    QScrollArea * scroll;
    QWidget * background;
    QLabel * HLabels[7];
    QLabel * VLabels[6];
    QLineEdit * lenStat[36];
    QGridLayout * layout0;

    QLabel * lblDataSet;
    uniList * listDataSets;
    stdButtonGroup * buttonGroup;
    QHBoxLayout * layout1;
    QGridLayout * layout;
public slots:
    void CreateActions(void);

    void doKillData(void);
    void doDuplicate(void);
    void doSetAppearance(void);
    void doSetOperations(void);
    void doHelpOnContext(void);
    void doHelpOnSetProp(void);
    void doEditInSpreadsheet(void);
    void doEditInTextEditor(void);
    void doCreateNewByFormula(void);
    void doCreateNewInSpreadsheet(void);
    void doCreateNewInTextEditor(void);
    void doCreateNewFromBlockData(void);

    void setChanged(int nr);
    void setChanged(QModelIndex index);
    void init(void);
    void doClear(void);
    void doApply(void);
    void doClose(void);
    void doAccept(void);
};

class frmSetOperations:public QDialog
{
    Q_OBJECT
public:
    frmSetOperations(QWidget * parent=0);

    grpSelect * grpSource;
    grpSelect * grpDestination;

    stdButtonGroup * buttonGroup;
    StdSelector * selType;

    QGridLayout * layout;
public slots:
    void init(void);
    void doApply(void);
    void doClose(void);
    void doAccept(void);
};

class frmAbout:public QDialog
{
    Q_OBJECT
public:
    frmAbout(QWidget * parent=0);
    QGroupBox * grpGrace;
    QGroupBox * grpLegal;
    QGroupBox * grpThirdParty;
    QGroupBox * grpBuildInfo;
    QGroupBox * grpHomePage;

    QLabel * lblInfo[32];
    QPushButton * cmdIAdr;
    QPushButton * cmdIAdr2;
    QPushButton * cmdIAdr3;
    QPushButton * cmdClose;

    QVBoxLayout * layout0;
    QVBoxLayout * layout1;
    QVBoxLayout * layout2;
    QVBoxLayout * layout3;
    QVBoxLayout * layout4;
    QVBoxLayout * layout;
public slots:
    void doShowHomepage(void);
    void doShowHomepage2(void);
    void doShowHomepage3(void);
    void doClose(void);
};

class frmFeatureExtract:public QDialog
{
    Q_OBJECT
public:
    frmFeatureExtract(QWidget * parent=0);

    QLabel * lblSourceGraph;
    uniList * listSourceGraph;
    QLabel * lblResultGraph;
    uniList * listResultGraph;
    QLabel * lblAbscissaGraph;
    uniList * listAbscissaGraph;
    QLabel * lblSet;
    uniList * listSet;

    stdButtonGroup * buttonGroup;
    StdSelector * selFeature;
    StdSelector * selXValue;
    stdLineEdit * ledValue;
    QVBoxLayout * layout;
public slots:
    void init(void);
    void FeatureChanged(int i);
    void XValueChanged(int i);
    void doClose(void);
    void doAccept(void);
    void fext_routine(int gfrom, int gto, int feature, int abs_src, int abs_set, int abs_graph );
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

    QLabel * lblFilename;
    QLabel * lblFilenText;
    QLabel * lblDescription;
    QLabel * lblDescrText;

    StdSelector * selSelection;
    StdSelector * selTargetGno;

    QWidget * empty;
    QScrollArea * scroll;

    int allocated_controls;
    QCheckBox ** chkImport;
    QLabel ** lblID;
    QLabel ** lblLegend;
    QLabel ** lblComment;
    QLabel ** lblType;
    QLabel ** lblTitle;

    QPushButton * cmdImport;
    QPushButton * cmdShowAgrInfo;

    struct agr_file_info info;
    bool shButtons,shFilename;

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
signals:

};

class frmIOForm:public QDialog
{
    Q_OBJECT
public:
    frmIOForm(int type,QWidget * parent=0);
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

    QLineEdit * ledSelection;

    StdSelector * selLoadAs;
    StdSelector * selType;
    StdSelector * selAutoscale;
    StdSelector * selParamGraph;
    QRadioButton * radPipe;
    QRadioButton * radDisk;
    QCheckBox * chkExchangeCommaPoint;

    uniList * graphList;
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
    bool FileExists;
    bool isWriteable;
    bool isReadable;

public slots:
    void gotNewSelection(QString selection);
    void gotNewSelectionDoubleClick(QString selection);
    void init(void);
    void doOK(void);
    void doFilter(void);
    void doCancel(void);
    void doHelp(void);
    void doOpenSetImport(void);
    void headerChecked(int c);
    void readUntilEOFChecked(int c);
    void setTypeChanged(int c);
    void columnCountChanged(int c);
    void columnSizeChanged(int c);
signals:
    void newFileSelectedForIO(int type,QString file,bool exists,bool writeable,bool readable);
};

class GlyphPanel:public QLabel
{
    Q_OBJECT
public:
    GlyphPanel(QWidget * parent=0);
    int number;
    int font;
    bool marked;
    bool valid_char;
    QPixmap * pix;
public slots:
    void setMarked(bool mark);
    void setCurrentPixmap(int font_nr,int char_nr);
    void mousePressEvent(QMouseEvent *event);
    static QPixmap DrawCB(unsigned char c,int FontID,bool & val_char);
signals:
    void panelClicked(int i);
};

class frmFontTool:public QDialog
{
    Q_OBJECT
public:
    frmFontTool(QWidget * parent=0);

    FontSelector * selFont;
    GlyphPanel * panel[16*338];
    stdLineEdit * ledString;
    stdLineEdit * ledAscii,*ledUnicode;
    QWidget * background;
    QScrollArea * scroll;
    QString textString;
    QGridLayout * layout0;
    QGridLayout * layout;
    stdButtonGroup * buttonGroup;
    int marked,tileCount;
    unsigned short last_character;
public slots:
    void characterInserted(QString text);
    void insertAtCursor(QString c);
    void FontChanged(int i);
    void newClick(int i);
    void doApply(void);
    void doClose(void);
    void doAccept(void);
};

class frmFourier:public QDialog
{
    Q_OBJECT
public:
    frmFourier(QWidget * parent=0);

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

class frmHotLinks:public QDialog
{
    Q_OBJECT
public:
    frmHotLinks(QWidget * parent=0);

    QLabel * lblLinkSet;
    uniList * hotlink_set_item;
    uniList * hotlink_list_item;///NOCH ZU AENDERN
    StdSelector * hotlink_source_item;
    stdLineEdit * hotlink_file_item;

    QPushButton * buttons[5];

    QGridLayout * layout;
public slots:
    void init(void);
    void doLink(void);
    void doFiles(void);
    void doUnlink(void);
    void update_hotlinks(void);
    void doClose(void);
    void do_hotupdate_proc(void);
    void newLinkFileSelected(int type,QString file,bool exists,bool writeable,bool readable);
};

class frmMasterRegionOperator:public QDialog
{
    Q_OBJECT
public:
    frmMasterRegionOperator(QWidget * parent=0);
    QLabel * lblTitles[5];
    QLabel * lblRegions[7];
    QComboBox * cmbType[7];
    QPushButton * cmdActive[7];
    QPushButton * cmdDefine[7];
    QPushButton * cmdReportSet[7];
    QPushButton * cmdReportPoints[7];

    QPushButton * cmdClearARegion;
    QPushButton * cmdClearAllRegions;
    QPushButton * cmdClose;
    QGridLayout * layout;

    QSignalMapper * mapActive,*mapType,*mapDefine,*mapReportSet,*mapReportPoints;

    QString regtypes[10];
    int reg_order[10];
public slots:
    void init(void);
    void doClearARegion(void);
    void doClearAllRegions(void);
    void doClose(void);
    void clickActive(int regno);
    void changeType(int regno);
    void clickDefine(int regno);
    void clickReportSets(int regno);
    void clickReportPoints(int regno);
};

class frmRegionStatus:public QDialog
{
    Q_OBJECT
public:
    frmRegionStatus(QWidget * parent=0);

    QString active,inactive;
    QString regtypes[10];

    QScrollArea * scroll;
    QWidget * background;
    QLabel * lblHeader;
    QLabel * lblStatus[MAXREGION];
    QPushButton * cmdClose;
    QPushButton * cmdUpdate;
    QVBoxLayout * layout0;
    QGridLayout * layout;
public slots:
    void init(void);
    void doUpdate(void);
    void doClose(void);
};

class frmRegions:public QDialog
{
    Q_OBJECT
public:
    frmRegions(int type,QWidget * parent=0);
    int windowtype;
    StdSelector * selector0;
    StdSelector * selector1;
    stdButtonGroup * buttonGroup;
    int reg_order[10];
    QVBoxLayout * layout;
public slots:
    void init(void);
    void doAccept(void);
    void doDefine(void);
    void doClose(void);
    //void define_region(int nr, int rtype);
};

class frmEditBlockData:public QDialog
{
    Q_OBJECT
public:
    frmEditBlockData(QWidget * parent=0);
    int block_curtype;
    QString filename;
    int source;
    grpSelect * grpLoadTo;
    QString begining,middle;
    QLabel * lblData;
    QGroupBox * grpColumns;
    StdSelector * columnSelector[6];
    StdSelector * selType;
    StdSelector * selStringColumn;
    StdSelector * selAutoscale;
    stdButtonGroup * buttonGroup;
    QVBoxLayout * layout0;
    QVBoxLayout * layout;
public slots:
    void setCompleteData(int columns,int length);
    void setTypeChanged(int i);
    void init(void);
    void update_eblock(int gno);
    void doAccept(void);
    void doApply(void);
    void doClose(void);
};

class frmLoadEval:public QDialog
{
    Q_OBJECT
public:
    frmLoadEval(QWidget * parent=0);
    QGroupBox * grpParameterMesh;
    stdStartStop * ststst;
    QHBoxLayout * layout0;
    QLabel * lblText[MAX_SET_COLS];
    QLineEdit * ledText[MAX_SET_COLS];
    stdSetTypeSelector * selType;
    stdButtonGroup * buttonGroup;
    QGridLayout * layout;
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
    QPushButton * cmdApply;
    QPushButton * cmdAccept;
    QPushButton * cmdClose;
    StdSelector * selRestriction;

    frmEvalExpr(MainWindow * parWind,QWidget *parent=0);
    ~frmEvalExpr();

    stdButtonGroup * buttonGroup;
    QHBoxLayout * layout0;
    QGridLayout * layout;

public slots:
    void doApply(void);
    void doAccept(void);
    void doClose(void);
    void init(void);
    void update(void);
    void compute_aac(void);
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

    QLabel * lblAFit;

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
};

class frmInterpolation:public QDialog
{
    Q_OBJECT
public:
    frmInterpolation(int type,QWidget * parent=0);
    int WindowType;
    grpSelect * grpSource;
    grpSelect * grpDestination;
    uniList * sampSet;
    QCheckBox * chkStrict;
    QCheckBox * chkNextPowerOfTwo;
    QCheckBox * chkCumulHist;
    QCheckBox * chkNormalize;
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

class tabMain:public QWidget
{
    Q_OBJECT
public:
    tabMain(QWidget * parent=0);
    ~tabMain();
    QGroupBox * fraSetPres;
    StdSelector * cmbType;
    int number_of_Type_entries;
    int * Type_entries;
    QGroupBox * fraSymbProp;
    ColorSelector * cmbSymbColor;
    StdSelector * cmbSymbType;
    stdSlider * sldSymbSize;
    stdLineEdit * ledSymbChar;
    QGroupBox * fraLineProp;
    StdSelector * cmbLineType;
    LineStyleSelector * cmbLineStyle;
    LineWidthSelector * spnLineWidth;
    ColorSelector * cmbLineColor;
    QGroupBox * fraLegend;
    stdLineEdit * ledString;
    QGroupBox * fraDispOpt;
    QCheckBox * chkAnnVal;
    QCheckBox * chkDispErrBars;
    QHBoxLayout * layout0;
    QVBoxLayout * layout1;
    QVBoxLayout * layout2;
    QHBoxLayout * layout3;
    QHBoxLayout * layout4;
    QGridLayout * layout;
public slots:
    void LineColorChanged(int val);
    void SymbColorChanged(int val);
signals:
    void colorChanged(int val);
    void colorChanged2(int val);
};

class tabSymbol:public QWidget
{
    Q_OBJECT
public:
    tabSymbol(QWidget * parent=0);
    QGroupBox * fraSymbOutl;
    LineStyleSelector * cmbSymbStyle;
    FillPatternSelector * cmbSymbPattern;
    LineWidthSelector * spnSymbWidth;
    QGroupBox * fraSymbFill;
    ColorSelector * cmbFillColor;
    FillPatternSelector * cmbFillPattern;
    QGroupBox * fraExtra;
    stdIntSelector * spnSymbSkip;
    FontSelector * cmbSymbFont;
    QWidget * empty;
    QVBoxLayout * layout;
    QGridLayout * layout0;
    QHBoxLayout * layout1;
    QVBoxLayout * layout2;
};

class tabLine:public QWidget
{
    Q_OBJECT
public:
    tabLine(QWidget * parent=0);
    QGroupBox * fraLineProp;
    FillPatternSelector * cmbPattern;
    QCheckBox * chkDrawDropLines;
    QGroupBox * fraFillProp;
    StdSelector * cmbType;
    StdSelector * cmbRule;
    FillPatternSelector * cmbFillPattern;
    ColorSelector * cmbFillColor;
    SetSelectorCombo * cmbSet;
    QGroupBox * fraBaseLine;
    StdSelector * cmbBaseType;
    QCheckBox * chkDrawLine;
    QWidget * empty;
    QVBoxLayout * layout;
    QHBoxLayout * layout0;
    QGridLayout * layout1;
    QHBoxLayout * layout2;
};

class tabAnnVal:public QWidget
{
    Q_OBJECT
public:
    tabAnnVal(QWidget * parent=0);
    QGroupBox * fraTextProp;
    FontSelector * cmbFont;
    stdSlider * sldFontSize;
    ColorSelector * cmbColor;
    stdSlider * sldFontAngle;
    stdLineEdit * ledPrepend;
    stdLineEdit * ledAppend;
    QGroupBox * fraFormatOpt;
    StdSelector * cmbType;
    StdSelector * cmbPrecision;
    StdSelector * cmbFormat;
    QGroupBox * fraPlacement;
    stdLineEdit * ledXOffs;
    stdLineEdit * ledYOffs;
    QVBoxLayout * layout;
    QGridLayout * layout0;
    QGridLayout * layout1;
    QHBoxLayout * layout2;
};

class tabErrorBars:public QWidget
{
    Q_OBJECT
public:
    tabErrorBars(QWidget * parent=0);
    QGroupBox * fraCommon;
    StdSelector * cmbPlacement;
    ColorSelector * cmbColor;
    FillPatternSelector * cmbPattern;
    QGroupBox * fraClipping;
    QCheckBox * chkArrowClip;
    LineWidthSelector * spnMaxLength;
    QGroupBox * fraBarLine;
    stdSlider * sldBarSize;
    LineWidthSelector * spnbarWidth;
    LineStyleSelector * cmbBarStyle;
    QGroupBox * fraRiserLine;
    LineWidthSelector * spnRiserWidth;
    LineStyleSelector * cmbRiserStyle;
    QWidget * empty;
    QGridLayout * layout;
    QVBoxLayout * layout0;
    QVBoxLayout * layout1;
    QVBoxLayout * layout2;
    QVBoxLayout * layout3;
};

class frmSet_Appearance:public QWidget
{
    Q_OBJECT
public:

    frmSet_Appearance(QWidget * parent=0);
    ~frmSet_Appearance();

    bool updating;
    int cset;

    tabMain * tabMa;
    tabSymbol * tabSy;
    tabLine * tabLi;
    tabAnnVal * tabAnVa;
    tabErrorBars * tabErBa;

    QMenuBar * menuBar;
    QMenu * mnuFile;
    QMenu * mnuEdit;
    QMenu * mnuOptions;
    QMenu * mnuHelp;
    QLabel * lblSelSet;
    uniList * listSet;
    QTabWidget * tabs;
    stdButtonGroup * buttonGroup;

    QAction * actclose,*acthelponcontext,*acthelponsetappearance;
    QAction * actdupllegends,*actcolorsync;
    QAction * actsetdiffcolors,*actsetdifflinestyles,*actsetdifflinewidths,*actsetdiffsymbols,*actsetbaw;
    QAction * actloadcoments,*actstriplegends;

    QVBoxLayout * layout;

public slots:
    //Initializations
    void CreateActions(void);
    void init(void);
    //Button-Actions
    void doApply(void);
    void doAccept(void);
    void doClose(void);
    //Menu-Actions
    void doHelpOnContext(void);
    void doHelpOnSetAppearance(void);
    void doDuplLegends(void);
    void doColorSync(void);
    void doSetDiffColors(void);
    void doSetDiffLineStyles(void);
    void doSetDiffLineWidths(void);
    void doSetDiffSymbols(void);
    void doSetBlackAndWhite(void);
    void doLoadComments(void);
    void doStripLegends(void);
    //immediateUpdates
    void update0(void);
    void update1(int v);
    void update2(QString v);
    void update3(bool v);
    void update4(double v);
    //Set-Actions
    void newListSelection(int a);
    void ShowSetData_external(int graph_number,int set_number);//the same as showSetData, but does also a new selection
    void showSetData(int graph_number,int set_number);
    void writeSetData(int graph_number,int set_number);
    void SyncColors(int val);
    void SyncColors2(int val);
    void setapp_data_proc(int dat);
    void redisplayContents(void);
};

class frmSetAppearance:public QDialog
{
    Q_OBJECT
public:
    frmSetAppearance(QWidget * parent=0);
    frmSet_Appearance * flp;
    uniList * listSet;
public slots:
    void init(void);
    void ShowSetData_external(int graph_number,int set_number);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
};

class GrTabMain:public QWidget
{
    Q_OBJECT
public:
    GrTabMain(QWidget * parent=0);

    QGroupBox * grpPres;
    QGroupBox * grpTitles;
    QGroupBox * grpViewport;
    QGroupBox * grpDispOpt;

    StdSelector * selType;
    QCheckBox * chkStackChart;
    stdLineEdit * ledTitle;
    stdLineEdit * ledSubtitle;
    stdLineEdit * ledCoords[4];
    QCheckBox * chkDisplLegend;
    QCheckBox * chkFlipXY;

    QVBoxLayout * layout;
    QHBoxLayout * layout1;
    QVBoxLayout * layout2;
    QGridLayout * layout3;
    QHBoxLayout * layout4;
public slots:

};

class GrTabTitles:public QWidget
{
    Q_OBJECT
public:
    GrTabTitles(QWidget * parent=0);

    QGroupBox * grpTitle;
    QGroupBox * grpSubTitle;

    FontSelector * selTitleFont;
    stdSlider * sldTitleCharSize;
    ColorSelector * selTitleColor;

    FontSelector * selSubFont;
    stdSlider * sldSubCharSize;
    ColorSelector * selSubColor;

    QVBoxLayout * layout;
    QVBoxLayout * layout1;
    QVBoxLayout * layout2;
public slots:
};

class GrTabFrame:public QWidget
{
    Q_OBJECT
public:
    GrTabFrame(QWidget * parent=0);

    QGroupBox * grpFrameBox;
    QGroupBox * grpFrameFill;

    StdSelector * selFrameType;
    ColorSelector * selBoxColor;
    FillPatternSelector * selFrameBoxPattern;
    LineWidthSelector * selFrameBoxWidth;
    LineStyleSelector * selFrameBoxStyle;

    ColorSelector * selFillColor;
    FillPatternSelector * selFrameFillPattern;

    QWidget * emptyArea;

    QVBoxLayout * layout;
    QGridLayout * layout1;
    QHBoxLayout * layout2;
public slots:
};

class GrTabLegBox:public QWidget
{
    Q_OBJECT
public:
    GrTabLegBox(QWidget * parent=0);

    QGroupBox * grpLocation;
    QGroupBox * grpFrameLine;
    QGroupBox * grpFrameFill;

    PositionSelector * selLoc;
    stdLineEdit * ledX;
    stdLineEdit * ledY;

    ColorSelector * selFrameLineColor;
    FillPatternSelector * selFrameLinePattern;
    LineWidthSelector * selFrameLineWidth;
    LineStyleSelector * selFrameLineStyle;

    ColorSelector * selFrameFillColor;
    FillPatternSelector * selFrameFillPattern;

    StdSelector * selLegBoxAttachement;
    QPushButton * cmdAttachLeft;
    QPushButton * cmdAttachTop;
    QPushButton * cmdAttachRight;
    QPushButton * cmdAttachBottom;
    QPushButton * cmdMoveLegend;

    QWidget * emptyArea;

    QVBoxLayout * layout;
    QGridLayout * layout1;
    QGridLayout * layout2;
    QHBoxLayout * layout3;
public slots:
    void viewCoordsChanged(int index);
    void autoAttachChanged(int index);
    void doAttachLeft(void);
    void doAttachTop(void);
    void doAttachBottom(void);
    void doAttachRight(void);
    void doMoveLegend(void);
signals:
    void doApply(void);
};

class GrTabLegends:public QWidget
{
    Q_OBJECT
public:
    GrTabLegends(QWidget * parent=0);

    QGroupBox * grpTextProp;
    QGroupBox * grpPlacement;

    FontSelector * selTextFont;
    stdSlider * sldTextSize;
    ColorSelector * selTextColor;

    StdSelector * selVGap;
    StdSelector * selHGap;
    StdSelector * selLineLength;
    QCheckBox * chkPutRevOrder;

    QWidget * emptyArea;

    QVBoxLayout * layout;
    QVBoxLayout * layout1;
    QGridLayout * layout2;
public slots:
};

class GrTabSpecial:public QWidget
{
    Q_OBJECT
public:
    GrTabSpecial(QWidget * parent=0);

    QGroupBox * grp2Dplusgraphs;
    QGroupBox * grpXYcharts;

    stdLineEdit * ledZnormal;
    LineWidthSelector * selBarGap;

    QWidget * emptyArea;

    QVBoxLayout * layout;
    QHBoxLayout * layout1;
    QHBoxLayout * layout2;
public slots:
};

class frmGraph_App:public QWidget
{
    Q_OBJECT
public:
    frmGraph_App(QWidget * parent=0);
    ~frmGraph_App();

    QMenuBar * menuBar;
    QMenu * mnuFile;
    QMenu * mnuEdit;
    QMenu * mnuHelp;
    QLabel * lblTitle;
    uniList * listGraph;
    QTabWidget * tabs;
    GrTabMain * tabMain;
    GrTabTitles * tabTitles;
    GrTabFrame * tabFrame;
    GrTabLegBox * tabLegBox;
    GrTabLegends * tabLegends;
    GrTabSpecial * tabSpec;
    stdButtonGroup * buttonGroup;
    QVBoxLayout * layout;

    frmIOForm * frmOpenPara;
    frmIOForm * frmSavePara;

    QAction * actOpen,* actSave,* actClose,* actHelpOnContext,* actHelpGraphApp,* actFocusTo,*actDuplicate,*actCreateNew,* actShow,* actHide,* actKill;

public slots:
    void init(void);
    void CreateActions(void);
    void doAccept(void);
    void doClose(void);
    void doApply(void);
    void doOpen(void);
    void doSave(void);
    void doHelpOnContext(void);
    void doHelpGraphApp(void);
    void doPrepare(void);
    void doHide(void);
    void doShow(void);
    void doKill(void);
    void doDuplicate(void);
    void doFocus(void);
    void doCreateNew(void);
    int graphapp_aac_cb(void);
    void show_graph_data_external(int n_gno);
    void newSelection(int i);
    void redisplayContents(void);//to switch between different decimal separators
    void update_graphapp_items(int n, int *values);
    void update_view(int gno);
    void updatelegends(int gno);
    void update_frame_items(int gno);
    void IOrequested(int type,QString file,bool exists,bool writeable,bool readable);
    //immidiate updates
    void update0(void);
    void update1(int v);
    void update2(QString v);
    void update3(bool v);
    void update4(double v);
};

class frmGraphApp:public QDialog
{
    Q_OBJECT
public:
    frmGraphApp(QWidget * parent=0);
    frmGraph_App * flp;
    uniList * listGraph;
public slots:
    void init(void);
    void show_graph_data_external(int n_gno);
    void update_view(int gno);
    void updatelegends(int gno);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
};

class AxisTabMain:public QWidget
{
    Q_OBJECT
public:
    AxisTabMain(QWidget * parent=0);

    QGroupBox * grpAxisLabel;
    stdLineEdit * ledAxisLabel;
    QHBoxLayout * layout0;

    QGroupBox * grpTickProp;
    stdLineEdit * ledMajorSpacing;
    stdIntSelector * selMinTicks;
    StdSelector * selFormat;
    StdSelector * selPrecision;
    QGridLayout * layout1;

    QGroupBox * grpDisplOpt;
    QCheckBox * chkDisplTickLabels;
    QCheckBox * chkDisplAxixBar;
    QCheckBox * chkDisplTickMarks;
    QGridLayout * layout2;

    QGroupBox * grpAxisPlace;
    QCheckBox * chkZeroAxis;
    stdLineEdit * ledOffsetNormal;
    stdLineEdit * ledOffsetOpposite;
    QHBoxLayout * layout3;

    QGroupBox * grpTickLabelProp;
    FontSelector * selTickLabelFont;
    ColorSelector * selTickLabelColor;
    QHBoxLayout * layout4;

    QVBoxLayout * layout;
public slots:
};

class AxisTabLabelBars:public QWidget
{
    Q_OBJECT
public:
    AxisTabLabelBars(QWidget * parent=0);

    QGroupBox * grpLabelProperties;
    FontSelector * selLabelFont;
    ColorSelector * selLabelColor;
    stdSlider * sldCharSize;
    StdSelector * selLayout;
    StdSelector * selSide;
    StdSelector * selLocation;
    stdLineEdit * ledParaOffs;
    stdLineEdit * ledPerpendOffs;
    QGridLayout * layout1;

    QGroupBox * grpBarProperties;
    ColorSelector * selBarColor;
    LineStyleSelector * selBarStyle;
    LineWidthSelector * selBarWidth;
    QGridLayout * layout2;

    QWidget * empty;
    QVBoxLayout * layout;
public slots:
    void locationChanged(int i);
};

class AxisTabTickLabels:public QWidget
{
    Q_OBJECT
public:
    AxisTabTickLabels(QWidget * parent=0);

    QGroupBox * grpLabels;
    stdSlider * sldCharSize;
    stdSlider * sldCharAngle;
    QHBoxLayout * layout0;

    QGroupBox * grpPlacement;
    StdSelector * selSide;
    StdSelector * selStartAt;
    StdSelector * selStopAt;
    StdSelector * selStagger;
    QLineEdit * ledStart;
    QLineEdit * ledStop;

    QGroupBox * grpExtra;
    QGridLayout * layout1;
    stdLineEdit * ledPrepend;
    stdLineEdit * ledAppend;
    stdLineEdit * ledAxisTransform;
    stdLineEdit * ledParaOffs;
    stdLineEdit * ledPerpendOffs;
    StdSelector * selSkipEvery;
    StdSelector * selLocation;
    QGridLayout * layout2;

    QWidget * empty;

    QVBoxLayout * layout;
public slots:
    void locationChanged(int i);
};

class AxisTabTickMarks:public QWidget
{
    Q_OBJECT
public:
    AxisTabTickMarks(QWidget * parent=0);

    QGroupBox * grpPlacement;
    StdSelector * selPointing;
    StdSelector * selDrawOn;
    StdSelector * setAutotickDiv;
    QCheckBox * chkPlaceRoundPos;
    QGridLayout * layout0;

    QGroupBox * grpMajorTicks;
    QCheckBox * chkDrawMajGrid;
    stdSlider * sldMajTickLength;
    ColorSelector * selMajTickColor;
    LineWidthSelector * selMajTickWidth;
    LineStyleSelector * selMajTickStyle;
    QVBoxLayout * layout1;

    QGroupBox * grpMinorTicks;
    QCheckBox * chkDrawMinGrid;
    stdSlider * sldMinTickLength;
    ColorSelector * selMinTickColor;
    LineWidthSelector * selMinTickWidth;
    LineStyleSelector * selMinTickStyle;
    QVBoxLayout * layout2;

    QWidget * empty;

    QGridLayout * layout;
public slots:
};

class AxisTabSpecial:public QWidget
{
    Q_OBJECT
public:
    AxisTabSpecial(QWidget * parent=0);

    StdSelector * selSpecTicks;
    stdIntSelector * selNumber;
    //QLabel * lblTickLocLabel;

    QScrollArea * scroll;
    //spreadSheet * spreadSpecLabels;

    QLabel * lblTitles[3];
    QLabel * lblNr[MAX_TICKS];
    QLineEdit * ledLocation[MAX_TICKS];
    QLineEdit * ledLabel[MAX_TICKS];

    bool original[MAX_TICKS];
    char * orig_text[MAX_TICKS],*text[MAX_TICKS];
    int stdHeight,headerHeight;

    QWidget * empty;
    QGridLayout * layout0;

    QVBoxLayout * layout;
public slots:
    void updateSpreadSheet(int i);
};

class frmAxis_Prop:public QWidget
{
    Q_OBJECT
public:
    frmAxis_Prop(QWidget * parent=0);
    ~frmAxis_Prop();

    int curaxis,apply_to_selection;

    StdSelector * selEdit;
    QCheckBox * chkActive;
    stdLineEdit * ledStart;
    stdLineEdit * ledStop;
    StdSelector * selScale;
    QCheckBox * chkInvAxis;
    QTabWidget * tabs;
    AxisTabMain * tabMain;
    AxisTabLabelBars * tabLabelsBars;
    AxisTabTickLabels * tabTickLabels;
    AxisTabTickMarks * tabTickMarks;
    AxisTabSpecial * tabSpecial;

    StdSelector * selApplyTo;
    QPushButton * cmdApplyTo;
    stdButtonGroup * buttonGroup;
    QGridLayout * layout;
public slots:
    void selEditChanged(int i);
    void doAccept(void);
    void doApply(void);
    void doApplyTo(void);
    void doClose(void);
    int axes_aac_cb(void);
    void axis_scale_cb(int value);
    void set_active_proc(int onoff);
    void set_axis_proc(int value);
    void update_ticks(int gno);
    void create_axes_dialog(int axisno);
    void redisplayContents(void);//to switch between different decimal separators
    //immediateUpdates
    void update0(void);
    void update1(int v);
    void update2(QString v);
    void update3(bool v);
    void update4(double v);
};

class frmAxisProp:public QDialog
{
    Q_OBJECT
public:
    frmAxisProp(QWidget * parent=0);
    frmAxis_Prop * flp;
public slots:
    void update_ticks(int gno);
    void create_axes_dialog(int axisno);
    void doAccept(void);
    void doClose(void);
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
    char * getcdf_type(nc_type datatype);
    void IOrequested(int type,QString file,bool exists,bool writeable,bool readable);
};

/*binary format input*/
class inputLine:public QWidget
{
    Q_OBJECT
public:
    inputLine(int t,QWidget * parent=0);
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
    bool header_present;
    int header_format;//0=manual,1=header in data file itself,2=header in separate binary-data-file,3=header in separate ini-file, 4=header in separate ascii-file
    char string_end_char;
    bool read_to_eof,keep_trigger,multiple_header_files,determine_string_size;
    int setorder,autoscale;
    int target_gno,set_type;//target set is always the next set-id available
    int headersize,first_suggestion;

    //the following values may or may not be present in the header; they can be set ab initio or can be read from the header
    double triggervalue;
    int trigger_type;//-1=no trigger, 0=rising edge, 1=falling edge, 2=either edge
    int channels,points;

    //the actual import format for all channels
    int * channel_format;
    int * channel_size;
    int * channel_target;

/// informations (usually) read from a header
    //necessary to read the data
    int bytesize,bitsize;
    int whole_size,single_size;
    double factors[7];//scaling factors for x,y,y1,y2,y3,y4,trigger
    double channel_factors[MAX_BIN_IMPORT_CHANNELS];//scaling factors for every channel; we use only 16 channels here -- should be enough
    double channel_offsets[MAX_BIN_IMPORT_CHANNELS];//offset value for every channel
    //ini-header-files
    int nr_of_import_tokens;//how many relevant tokens are present in an ini-file
    int * token_target;//where to import header-data into
    QStringList vals,keys;//header-informations form an ini-file
    QList<int> import_channel_dest;//what channel the data is to import to (-1 = general information, not specific to a channel)
    QList<int> import_dest;//where to import the data read from the header
    //binary-header-files
    int nr_of_header_values;
    int * header_value_format,* header_value_size,* header_value_import;

    //control values set after a header has been read
    bool x0set,deltaxset,fset,contains_trigger;

    //auxiliary data read from the header
    char * title;
    char * subtitle;
    char * x_title,*y_title,*set_title;

    double x0,deltax,f;//f=sampling rate-->deltax=1/f; x=x0+deltax*i

/// first data read after header (just to check the import settings)
    double ** first_data;
    int points_read,columns_read;
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
public slots:
    void clearData(void);
    void initData(struct importSettings imp_set);
    void setData(struct importSettings & imp_set);
};

class pageHeaderInfo:public QWidget
{
    Q_OBJECT
public:
    pageHeaderInfo(QWidget * parent=0);
    QVBoxLayout * grid;
    QVBoxLayout * layout;
    QWidget * empty;
    QWidget * empty0;
    QWidget * empty1;
    QWidget * empty2;
    QScrollArea * scroll;
    QScrollArea * scroll2;
    QVBoxLayout * layout2;
    QPushButton * cmdAdd;
    QLabel ** Headers;
    int number_of_lines;
    inputLine ** inFormats;
    QSignalMapper * map;
    QLabel * lblEndChar;
    QLineEdit * lenEndChar;
    QPushButton * cmdTestLoad;
    //int nr_of_sels;
    //StdSelector ** sels;
    inputIniData * iniDataGroup;
    //QString * readValues;
    QPushButton * cmdReadIni;
    int nr_of_entries;
    QString entries[NUMBER_OF_IMPORT_DESTINATIONS];
    int entry_values[NUMBER_OF_IMPORT_DESTINATIONS];
    frmBinaryFormatInput * par_wid;
    inputIniData * inpIniData;
    QStringList keys;//the ini-imput-keys
signals:
    void readHeader(void);
public slots:
    void changeRepresentation(int r);
    void doReadIni(void);
    void updateOffsets(void);
    void doNew(void);
    void doDelete(int i);//remove a line
    void offsetChanged(int i);
    void doTestLoad(void);
    void read_header_settings(struct importSettings & imp_set);//transfer import settings from gui to imp_set
    void write_header_settings(struct importSettings & imp_set);//tranfer import settings to gui
};

class pageDataInfo:public QWidget
{
    Q_OBJECT
public:
    pageDataInfo(QWidget * parent=0);
    QGridLayout * grid;
    QLabel * lblComment;
    QCheckBox * chkReadToEOF;
    QCheckBox * chkKeppTrigger;
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
    LineWidthSelector * selTriggerValue;
    StdSelector * selTriggerType;
signals:
    void newChannelCount(int i);
public slots:
    void channelCountChanged(int i);
    void eofToggled(bool i);
    void readDataSettings(importSettings & imp_set);
    void writeDataSettings(importSettings & imp_set);
};

class pageFileInfo:public QWidget
{
    Q_OBJECT
public:
    pageFileInfo(QWidget * parent=0);
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

void LoadFileFormat(char * fname, struct importSettings & imp_set);
void SaveFileFormat(char * fname, struct importSettings & imp_set);
void matchSchemeToHeader(char * fname,struct importSettings & imp_set,struct importSettings & imp_scheme);
void readBinFileHeader(char * fname,struct importSettings & imp_set);

class frmBinaryFormatInput:public QDialog
{
    Q_OBJECT
public:
    frmBinaryFormatInput(QWidget * parent=0);
    QGridLayout * grid;
    QPushButton * cmdSave;
    QPushButton * cmdLoad;
    QPushButton * cmdStdSave;
    QPushButton * cmdStdLoad;
    QCheckBox * chkHeader;
    QCheckBox * chkMultiHeaders;
    QLabel * lblFormatSource;
    QComboBox * cmbFormatSource;
    QLabel * lblDataFile;
    QLineEdit * lenDataFile;
    QLabel * lblHeaderFile;
    QLineEdit * lenHeaderFile;
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

    char datFileName[512];
    QStringList datFileNames;
    char headerFileName[512];
    QStringList headerFileNames;

    importSettings imp_set;//the current import-settings to be used
    importSettings imp_scheme;//the current sheme for the import-settings

    QString settingString;
    int first_suggestion;
    bool auto_transfer_from_header,determine_string_size;
    int headersize;
    QString LoadFormatPath,SaveFormatPath,LoadIniPath,LoadDataPath;
    QString HeaderSuffix,Data_Suffix;
    QString HeaderPath,Data_Path;
    /*QFileDialog * dlgLoadFormat,* dlgSaveFormat;
    QFileDialog * dlgLoadIniFile,* dlgLoadDataFile;*/
public slots:
    void init(void);
    void getDatFilesFromString(QString * origin,QStringList * lst);
    void readHeader(void);
    void headerToggled(bool t);
    void doSaveFileFormat(void);
    void doLoadFileFormat(void);
    void doSaveStdFormat(void);
    void doLoadStdFormat(void);
    void SelectDataFile(void);
    void SelectHeaderFile(void);
    int detectStdBinFormat(char * filen);
    void formatSourceChanged(int i);
    void displaySettings(struct importSettings & imp_set);
    void readSettings(struct importSettings & imp_set);
    void HeaderFormatChanged(int i);
    void doOK(void);
    void doClose(void);
    void doAccept(void);
    void transmitInfos(void);
    void convertSettingsToString(void);
    void updateSuffixes(void);
    void CheckHeadersAndDatFiles(void);//to complete filenames and check completeness on informations
};

void doReadDataFromHeader(ifstream & ifi,struct importSettings & imp_set);
void readBinaryFromFile(ifstream & ifi,struct importSettings & imp_set,double *** data);
void get_all_settings_from_ini_file(char * ini_file,QStringList & keys,QStringList & vals);

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

class frmUndoList:public QDialog
{
    Q_OBJECT
public:
    frmUndoList(QWidget * parent=0);
    QVBoxLayout * layout;
    QListWidget * list;
    QPushButton * cmdCommands;
    QCheckBox * chkActive;
    stdButtonGroup * aac;
public slots:
    void init(void);//rebuild entries
    void doUndo(void);
    void doRedo(void);
    void doCommands(void);
    void doClose(void);
    void doToggleActive(int state);
};

class frmExplorer:public QDialog
{
    Q_OBJECT
public:
    frmExplorer(QWidget * parent=0);
    char oldSetting;
    QGridLayout * layout;
    QLabel * lblTest;

    frmLine_Props * LineProperties;
    frmText_Props * TextProperties;
    frmEllipse_Props * EllipseProperties;
    frmEllipse_Props * BoxProperties;
    frmAxis_Prop * AxisProperties;
    frmSet_Appearance * SetProperties;
    frmGraph_App * GraphProperties;
    frmPlot_Appearance * PlotAppearance;
    QWidget * empty;

    treeView * tree;
    stdButtonGroup * aac;
public slots:
    void init(void);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
    void initItem(char type,int gno,int sno);
    void setItemVisible(char type, bool vis,int gno,int sno);
    void itemClickedInTree(char type,int gno,int sno);
};

#define STD_COL_SPECTRUM 0
#define BW_SPECTRUM 1
#define COLD_HOT_SPECTRUM 2
#define CUSTOM_SPECTRUM 3

class frmColorManagement:public QWidget
{
    Q_OBJECT
public:
    frmColorManagement(QWidget * parent=0);

    ColorSelector * colorsel;//shows current palette

    QGroupBox * grpCurCol;
    QLabel * lblInstructions;
    QLabel * lblIllustration;
    stdIntSelector * ledRed;
    stdIntSelector * ledGreen;
    stdIntSelector * ledBlue;
    stdLineEdit * ledColName;
    QPushButton * cmdSelNewColor;
    QPushButton * cmdDelColor;
    QPushButton * cmdAddColor;
    QPushButton * cmdEditColor;
    QColorDialog * colDial;
    int cur_col_num;
    int map_entries;
    CMap_entry * local_cmap_table;
    int allocated_loc_colors;
    QIcon ** locColorIcons;
    QPixmap ** locColorPixmaps;
    QString ** locColorNames;
    int temp_spec_lenght;
    CMap_entry * temp_spec;
    int spectrum_path_length;
    int * path_positions;
    int tmp_path_length;
    int tmp_path_point_shown;
    int * tmp_path_pos;

    QLabel * lblColNumber;
    QGridLayout * layout0;

    QGroupBox * grpColSpectra;
    QLabel * lblInstructions2;
    QLabel * lblIllustration2;
    QPushButton * cmdStdCols;
    QPushButton * cmdSetSpectrum;
    StdSelector * selStdSpectra;
    QCheckBox * chkInvert;
    stdIntSelector * selNumberOfColors;
    stdIntSelector * selNumberOfPathPoints;
    stdIntSelector * selNumberOfCurPathPoint;
    ColorSelector * selPathCol;
    QGridLayout * layout1;

    QVBoxLayout * layout;
    stdButtonGroup * aac;
public slots:
    void doAddColor(void);
    void doDelColor(void);
    void doEditColor(void);
    void doSelColor(void);
    void doSetStdColors(void);
    void doSetSpectrum(void);
    void generateSpectrum(int nr);
    void showTempSpectrum(void);
    void curColorChanged(int nr);
    void curSpectrumChanged(int nr);
    void curNumberOfColorsChanged(int nr);
    void changedInvert(bool nr);
    void colorCompositionChanged(int nr);
    void colorNameChanged(void);
    void init(void);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
    void init_with_color(int nr);
    void showSingleColor(void);
    void addColorToLocal(CMap_entry * entry);
    void editColorToLocal(CMap_entry * entry,int nr);
    void delColorToLocal(int nr);
    void NrOfPathPointsChanged(int nr);
    void CurrentPathPointChanged(int nr);
    void CurrentPathColorChanged(int nr);
    CMap_entry constructColor(void);
signals:
    void close_wish(void);
};

class frmRealTimeInputManager:public QDialog
{
    Q_OBJECT
public:
    frmRealTimeInputManager(QWidget * parent=0);
    QVBoxLayout * layout;
    uniList * lstInputs;
    QLabel * lblRTI;
    QFileDialog * frmOpen;
    //frmIOForm * frmOpen;
    stdIntSelector * selTimeout;
    QCheckBox * chkRTIactive;
    stdButtonGroup * aac;
public slots:
    void updateRTIs(void);
    void toggle_RTI(bool s);
    void init(void);
    void doOpen(void);
    void doDelete(void);
    void doClose(void);
};

struct cvs_import_infos
{
    char * filename;
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
};

class frmCSVImporter:public QDialog
{
    Q_OBJECT
public:
    frmCSVImporter(QWidget * parent=0);
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

    QScrollArea * scroll;
    QLabel * empty;
    QGridLayout * grid1;
    int allocated_selectors;
    QLabel ** lblDescription;
    StdSelector ** selFormat;
    StdSelector ** selTargetSet;
    StdSelector ** selTargetColumn;

    struct cvs_import_infos import_info;
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
    void doApply(void);
    void doAccept(void);
    void doClose(void);
};

class frmProgressWin:public QDialog
{
    Q_OBJECT
public:
    frmProgressWin(QWidget * parent=0);
    QLabel * lblText;
    QProgressBar * progress;
    QVBoxLayout * layout;
public slots:
    void init(QString text,int max);
    void increase(void);
    void setVal(int val);
};

class frmQuestionDialog:public QDialog
{
    Q_OBJECT
public:
    frmQuestionDialog(QWidget * parent=0);
    QGridLayout * layout;
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

int generate_x_mesh_from_formula(int gno,int sno,double start,double stop,int npts,char * formula,int type);
void ParseFilterCommand(char * com,int & o_n_sets,int ** o_gnos,int ** o_snos,int & n_sets,int ** gnos,int ** snos,int & type,int & realization,double * limits,int * orders,char * x_formula,double & ripple,int & absolute,int & debug,int & point_extension,int & oversampling,int & rno,int & invr);
void ParseRegression(char * com,int & n_sets,int ** gnos,int ** snos,int & n_n_sets,int ** n_gnos,int ** n_snos,int & ideg,int & iresid,int & rno,int & invr,double & start,double & stop,int & points,int & rx,char * formula);
int containsSpecialCommand(char * com,char ** parameters);
int ParseExtractCommand(char * com,char * arg);
int ParseSpecialFormula(char * com,char * arg);

#endif
