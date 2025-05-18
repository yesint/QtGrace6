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

#ifndef EDITWIDGETS_H
#define EDITWIDGETS_H

#include "fundamentals.h"
#include "undo_module.h"
#include "allWidgets.h"
#include "appearanceWidgets.h"
#include "windowWidgets.h"

using namespace std;

class frmDataSetProperties:public QDialog
{
    Q_OBJECT
public:
    frmDataSetProperties(QWidget * parent=0);
    ~frmDataSetProperties();
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
    void adjustSizeToFont(void);
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
    ~frmSetOperations();
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


class frmArrangeGraphs:public QDialog
{
    Q_OBJECT
public:
    frmArrangeGraphs(QWidget * parent=0);
    ~frmArrangeGraphs();
    QLabel * lblArrGraphs;
    uniList * graphlist;

    QGroupBox * grpArrGraphs;
    QGroupBox * grpMatrix;
    QGroupBox * grpPageOffset;
    QGroupBox * grpSpacing;

    stdIntSelector * selCols;
    stdIntSelector * selRows;

    /*QDoubleSpinBox * selUpperOffset;
    QDoubleSpinBox * selLowerOffset;
    QDoubleSpinBox * selLeftOffset;
    QDoubleSpinBox * selRightOffset;*/

    LineWidthSelector * selUpperOffset;
    LineWidthSelector * selLowerOffset;
    LineWidthSelector * selLeftOffset;
    LineWidthSelector * selRightOffset;

    LineWidthSelector * selLegendX, * selLegendY;

    LineWidthSelector * selHGap;
    LineWidthSelector * selVGap;
    OrderSelector * selOrder;

    QCheckBox * chkAddGraphs;
    QCheckBox * chkKillGraphs;
    StdSelector * selMoveLegends;
    QCheckBox * chkSnakeFill;
    QCheckBox * chkHPack;
    QCheckBox * chkVPack;

    stdButtonGroup * buttonGroup;

    QAction * actPackXInside[2],* actPackXOutside[2],* actPackXBoth[2],* actPackXUnchanged[2],* actPackXNone[2],* actPackXBottom[2],* actPackXTop[2];
    QAction * actPackYInside[2],* actPackYOutside[2],* actPackYBoth[2],* actPackYUnchanged[2],* actPackYNone[2],* actPackYLeft[2],* actPackYRight[2];

    QAction * actPackXDirectionIn[2],* actPackXDirectionOut[2],* actPackXDirectionBoth[2],* actPackXDirectionUnchanged[2];
    QAction * actPackYDirectionIn[2],* actPackYDirectionOut[2],* actPackYDirectionBoth[2],* actPackYDirectionUnchanged[2];

    QActionGroup * grpPackXInner,* grpPackXOuter,* grpPackYInner,* grpPackYOuter;
    QActionGroup * grpPackXInnerDirection,* grpPackXOuterDirection,* grpPackYInnerDirection,* grpPackYOuterDirection;
    QMenuBar * menuBar;
    QMenu * mnuPackX,* mnuPackY,* mnuGuess;
    QAction * actGuessAll,*actGuessPart;

    QGridLayout * layout0;
    QHBoxLayout * layout1;
    QGridLayout * layout2;
    QHBoxLayout * layout3;
    QVBoxLayout * layout;

    int guess_r,guess_c,n_graphs,guess_snake,guess_order,guess_success,guess_hpack,guess_vpack;
    double guess_hgap,guess_vgap;
    double left_offset,right_offset,bottom_offset,top_offset;

    int packTicksXInner,packTicksYInner;
    int packTicksXOuter,packTicksYOuter;
    int packTicksXInnerDir,packTicksYInnerDir;
    int packTicksXOuterDir,packTicksYOuterDir;
public slots:
    void createActions(void);
    void readTickSettings(void);
    void PackToggled(bool t);
    void init(void);
    void doApply(void);
    void doClose(void);
    void doAccept(void);
    void guessGraphOrdering(int nr,int * graphs);
    void guessSelectedGraphOrdering(void);
    void guessAllGraphOrdering(void);
    void MoveLegendsChanged(int l);
    void updateAfterGuessing(void);
};

class frmOverlayGraphs:public QDialog
{
    Q_OBJECT
public:
    frmOverlayGraphs(QWidget * parent=0);
    ~frmOverlayGraphs();
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
    ~frmAutoscaleGraphs();
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

class frmHotLinks:public QDialog
{
    Q_OBJECT
public:
    frmHotLinks(QWidget * parent=0);
    ~frmHotLinks();
    QLabel * lblLinkSet;
    uniList * hotlink_set_item;
    uniList * hotlink_list_item;///NOCH ZU AENDERN
    StdSelector * hotlink_source_item;
    StdSelector * auto_hotlink_update;
    stdLineEdit * hotlink_file_item;

    QPushButton * buttons[5];

    QGridLayout * layout;
    QTimer * autoupdatetimer;
public slots:
    void init(void);
    void doLink(void);
    void doFiles(void);
    void doUnlink(void);
    void update_hotlinks(void);
    void doClose(void);
    void do_hotupdate_proc(void);
    void autoupdatechanged(int a);
    void newLinkFileSelected(int type,QString file,bool exists,bool writeable,bool readable);
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
    QCheckBox * chkShowFixedInFile;
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
    void enableToggled(bool ch);
};

class frmUndoList:public QDialog
{
    Q_OBJECT
public:
    frmUndoList(QWidget * parent=0);
    ~frmUndoList();
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
    ~frmExplorer();
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

class frmMasterRegionOperator_Main:public QWidget
{
    Q_OBJECT
public:
    frmMasterRegionOperator_Main(QWidget * parent=0);
    QLabel * lblTitles[6];
    QLabel * lblRegions[7];
    QComboBox * cmbType[7];
    QPushButton * cmdActive[7];
    QPushButton * cmdDefine[7];
    QPushButton * cmdReportSet[7];
    QPushButton * cmdReportPoints[7];
    QSpinBox * spnLink[7];

    QPushButton * cmdClearARegion;
    QPushButton * cmdClearAllRegions;
    QPushButton * cmdClose;
    QGridLayout * layout;
#if QT_VERSION >= 0x050000

#else
    QSignalMapper * mapActive,*mapDefine,*mapReportSet,*mapReportPoints,*mapLink;
    //QSignalMapper *mapType;
#endif
signals:
void closeWish(void);
public slots:
    void init(void);
    void doClearARegion(void);
    void doClearAllRegions(void);
    void doClose(void);
    void clickActive(int regno);
    void changeType(int reg_no);
    void changeLink(int reg_no);
    void clickDefine(int regno);
    void clickReportSets(int regno);
    void clickReportPoints(int regno);
};

class frmMasterRegionOperator_Style:public QWidget
{
    Q_OBJECT
public:
    frmMasterRegionOperator_Style(QWidget * parent=0);

    QLabel * lblRegion[7];
    ColorSelector * selCol[7];
    LineWidthSelector * selWidth[7];
    LineStyleSelector * selStyle[7];
    stdButtonGroup * cmdButtons;
#if QT_VERSION >= 0x050000

#else
    QSignalMapper * mapColor,*mapLineW,*mapLineS;
#endif
    QGridLayout * layout;
signals:
void closeWish(void);
public slots:
    void init(void);
    void setNewColor(int c);
    void setNewLineW(int w);
    void setNewLineS(int s);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
};

class frmMasterRegionOperator_Edit:public QWidget
{
    Q_OBJECT
public:
    frmMasterRegionOperator_Edit(QWidget * parent=0);

    QLabel * lblTitle[5];
    int nr_of_lines;//what has been allocated
    int nr_of_vis_lines;//the visible lines -- the amount of data
    int start_line,max_lines;//start_line=first data to be displayed, max_lines=how many lines can be displayed on screen
    QScrollBar * scrollBar;
    QGridLayout * layout0;
    QWidget * Empty0;
    QHBoxLayout * layout1;
    QWidget * Empty;
    //QFrame * Empty;
    QWidget * Empty2,* Empty3;
    QLabel ** lblCoords;
    QLineEdit ** ledCoords[2];
    QPushButton ** cmdEdit[2];
    QScrollArea * scroll;
    StdSelector * regType;
    StdSelector * selRegion;
    QLabel * lblPoints;
#if QT_VERSION >= 0x050000

#else
    QSignalMapper * plusMapper,* minusMapper;
#endif
    QPushButton * cmdClearRegion;
    stdButtonGroup * cmdButtons;
    QGridLayout * layout;
    int in_definition,cur_region;
signals:
void initWish(void);
void closeWish(void);
public slots:
virtual void showEvent(QShowEvent * event);
virtual void resizeEvent(QResizeEvent *event);
    void init(void);
    void regionChanged(int re);
    void regionTypeChanged(int re);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
    void doAdd(int l);
    void doRemove(int l);
    void doClearRegion(void);
    void linesScolled(int pos);
    void reallocateLines(void);
    void displayRegionPoints(void);
};

class frmMasterRegionOperator_Operations:public QWidget
{
    Q_OBJECT
public:
    frmMasterRegionOperator_Operations(QWidget * parent=0);

    QLabel * lblGraph,*lblSet;
    uniList * graphList;
    uniList * setList;

    StdSelector * selRestriction;
    QCheckBox * chkNegRes;
    StdSelector * selNewSets;
    StdSelector * selOperation;
    StdSelector * selTargetGraph;

    stdButtonGroup * cmdButtons;
    QGridLayout * layout;
signals:
void closeWish(void);
public slots:
    void init(void);
    void newGraphSelection(int r);
    void OperationChanged(int op);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
};

class frmMasterRegionOperator_Create:public QWidget
{
    Q_OBJECT
public:
    frmMasterRegionOperator_Create(QWidget * parent=0);
    QLabel * lblGraph,*lblSet;
    uniList * graphList;
    uniList * setList;

    StdSelector * selRestriction;
    StdSelector * selOperation;
    QLabel * lblInfo;

    stdButtonGroup * cmdButtons;
    QGridLayout * layout;
    int n_sel_gno,*sel_gno;
    int n_sel_sno,*sel_sno;
    int target_operation,target_region;
signals:
void closeWish(void);
public slots:
virtual void showEvent(QShowEvent * event);
    void init(void);
    void read_inputs(void);
    void newGraphSelection(int r);
    void newSetSelection(int r);
    void operation_changed(int r);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
};

class frmMasterRegionOperator:public QDialog
{
    Q_OBJECT
public:
    frmMasterRegionOperator(QWidget * parent=0);
    ~frmMasterRegionOperator();

    frmMasterRegionOperator_Main * tab_Main;
    frmMasterRegionOperator_Style * tab_Style;
    frmMasterRegionOperator_Edit * tab_Edit;
    frmMasterRegionOperator_Operations * tab_Operations;
    frmMasterRegionOperator_Create * tab_Create;

    QTabWidget * tabs;
    QVBoxLayout * layout;
public slots:
    void init(void);
    void updateDecimalSeparator(void);
    void doClose(void);
    void number_of_graphs_changed(void);
};

class frmRegionStatus:public QDialog
{
    Q_OBJECT
public:
    frmRegionStatus(QWidget * parent=0);
    ~frmRegionStatus();
    QString active,inactive;

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
    ~frmRegions();
    int windowtype;
    StdSelector * selector0;
    StdSelector * selector1;
    stdButtonGroup * buttonGroup;
    QVBoxLayout * layout;
public slots:
    void init(void);
    void doAccept(void);
    void doDefine(void);
    void doClose(void);
    //void define_region(int nr, int rtype);
};


class frmUserDefinedFormats:public QWidget
{
    Q_OBJECT
public:
    frmUserDefinedFormats(QWidget * parent=0);
    QGroupBox * grpFormats;
    stdIntSelector * selNrOfFormats;
    StdSelector * selFormat;
    QPushButton * cmdRemove;
    QPushButton * cmdUp;
    QPushButton * cmdDown;
    QGridLayout * layout0;

    QGroupBox * grpEdit;
    StdSelector * selGeneralType;
    stdLineEdit * ledName;
    QLabel * lblFields;
    QPushButton * cmdEdit;
    QPushButton * cmdPlus;
    QPushButton * cmdMinus;
    QComboBox ** fieldType;
    QPushButton ** EditField;
#if QT_VERSION >= 0x050000

#else
    QSignalMapper * mapper1;
    QSignalMapper * mapper2;
#endif
    QScrollArea * scroll;
    QWidget * empty;
    QGridLayout * subLayout;
    stdLineEdit * lenInputTest;
    LineWidthSelector * selOutputTest;
    QLabel * lblInputTest;
    QLabel * lblOutputTest;
    QGridLayout * layout1;

    QGroupBox * grpDetails;
    int cur_Format,cur_field;

    QGridLayout * layout2;

    stdButtonGroup * buttons;
    QGridLayout * layout;
public slots:
    void doEdit(void);
    void doRemove(void);
    void doUp(void);
    void doDown(void);
    void doPlus(void);
    void doMinus(void);
    void doMapper1(int nr);
    void doMapper2(int nr);
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
    StdSelector * date_sep_item;
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

QGroupBox * grp_SetLists;
QGridLayout * layoutSetList;
QCheckBox * chkShowDataLess;
QCheckBox * chkShowHidden;
QCheckBox * chkShowComment;
QCheckBox * chkShowLegend;
QCheckBox * chkShowIcons;
QCheckBox * chkActivateDrag;

QGroupBox * grpColumnFormats;
QGridLayout * layoutColumnFormat;
StdSelector * selFormat;
stdIntSelector * selPrecision;

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

#define STD_COL_SPECTRUM 0
#define BW_SPECTRUM 1
#define COLD_HOT_SPECTRUM 2
#define GOOD_BAD_SPECTRUM 3
#define CUSTOM_SPECTRUM 4

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
    int temp_spec_length;
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

    QPushButton * cmdInverseColor;

    QVBoxLayout * layout;
    stdButtonGroup * aac;
public slots:
    void doAddColor(void);
    void doDelColor(void);
    void doEditColor(void);
    void doSelColor(void);
    void doSetStdColors(void);
    void doSetSpectrum(void);
    void doInvertColors(void);
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

class frm_Preferences:public QDialog
{
    Q_OBJECT
public:
    frm_Preferences(QWidget * parent=0);
    ~frm_Preferences();

    QWidget * flp;
    QTabWidget * tabs;
    QScrollArea * scroll;
    int min_w,min_h,bar_w,bar_h;
    frmPreferences * tab_prefs;

    frmDefaults * tab_defaults;
    tabLinestyles * tab_linestyles;
    frmColorManagement * tab_colors;
    frmUserDefinedFormats * tab_DefFormats;

    QWidget * tab_GUI;
    QVBoxLayout * guiLayout;

    ///GUI
    //customize Interface
    QGroupBox * grp_tool_bar;

    QGroupBox * grpToolBar;
    QGridLayout * layoutToolBar;
    QGroupBox * grpStatusBar;
    QGridLayout * layoutStatusBar;
    QGroupBox * grpAppFont;
    QGridLayout * layoutAppFont;
    QGroupBox * grpBackgroundColor;
    QGridLayout * layoutBackgroundColor;

    //QLabel * lblToolBar;
    QCheckBox * chkShowNavi;
    QCheckBox * chkShowGraph;
    QCheckBox * chkShowSpecZoom;
    QCheckBox * chkShowPageZoom;
    QCheckBox * chkShowPrintB;
    QCheckBox * chkShowExportP;
    QCheckBox * chkShowUndoRedo;
    /*QCheckBox * chkShowPanB;
    QCheckBox * chkShowPickB;
    QCheckBox * chkShowViewp;*/
    StdSelector * selShowPan;
    StdSelector * selShowPick;
    StdSelector * selShowViewp;
    LineWidthSelector * selToolbarSize;
    //QLabel * lblStatusBar;
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
    //QLabel * lblSelStartup;
    QLabel * lblStartupWarning;
    StdSelector * selLanguage;
    QGroupBox * grpIniPos;
    QGridLayout * layoutIniPos;
    stdIntSelector * selStdDpi;
    stdIntSelector * selStartupX;
    stdIntSelector * selStartupY;
    stdIntSelector * selStartupWidth;
    stdIntSelector * selStartupHeight;
    QLabel * lblStartupMain;
    StdSelector * selStartupBehavior;
    stdLineEdit * lenDefaultFile;
    StdSelector * selDefaultPrintDevice;
    QPushButton * cmdBrowseForDefault;
    QPushButton * cmdStartupCurrent;

    QPushButton * cmdTest;
    TestDialog * frmTest;

    //QLabel * lblAppearance;
    QCheckBox * chkNewIcons;
    //gui-font and background
    QLabel * lblGuiFont;
    QPushButton * cmdSelGuiFont;
    QPushButton * cmdResetGuiFont;
    //QLabel * lblBackground_Color_Text;
    QPushButton * cmdSelGUIBGColor;
    QPushButton * cmdSetGUIBGColor_to_PageBG;
    QPushButton * cmdSetGUIBGColor_to_Std;

    /// Behavior
    QWidget * tab_Behaviour;
    QGridLayout * behavLayout;
    StdSelector * selGeneral;
    QGroupBox * grpLoadSave;
    QGridLayout * layoutLoadSave;
    QGroupBox * grpInput;
    QGridLayout * layoutInput;
    QGroupBox * grpResponse;
    QGridLayout * layoutResponse;
    //General Behavior of QtGrace
    QGroupBox * grp_Behavior;
    StdSelector * selCodec;
    StdSelector * selTranspSelection;
    QCheckBox * chkActivateLaTeXSupport;
    QCheckBox * chkQtFonts;
    QCheckBox * chkSymbolSpecial;
    StdSelector * selDecSep;

    QCheckBox * chkAutoFitLoad;
    QCheckBox * chkAutoSetAgr;
    QCheckBox * chkAutoSetExport;
    QCheckBox * chkAutoSetCWD;
    QCheckBox * chkWarnOnEncodingChange;

    CompressionSelector * selCompression;

    QGroupBox * grpHighlight;
    ColorSelector * selHighlightColor;
    StdSelector * selHighlightTime;
    QGridLayout * layoutHighlight;
    QCheckBox * chkAutoHighlight;
    QCheckBox * chkHighlightErrorbars;

    QGroupBox * grpDrop;
    StdSelector * selDropComment;
    StdSelector * selDropLegend;
    QCheckBox * chkDropAutoscale;
    StdSelector * selDropDecSep;
    multiFileSelector * selCSVDropFormat;
    QGridLayout * layoutDrop;

    QCheckBox * chkImmediateUpdate;
    QCheckBox * chkEnableInlineEditing;
    QCheckBox * chkEnableContextMenuInMain;
    QCheckBox * chkAutoPackSets;
    StdSelector * selIconBehavior;
    QCheckBox * chkErrorbarsAutoscale;
    LineWidthSelector * selAutoscaleOffset;
    QCheckBox * chkAllowGestures;
    QCheckBox * chkAllowWheel;
    QCheckBox * chkAllowWheelChanges;
    QCheckBox * chkUndoActive;
    QCheckBox * chkUndoActiveStart;

    stdIntSelector * histSize;

    /// Misc
    QWidget * tab_Misc;
    QVBoxLayout * miscLayout;

    //QLabel * lblExtLibs;
    QGroupBox * grp_libFFTW3;
    QCheckBox * chkUseFFTW3;
    QLabel * lblFFTW3_found;
    QLabel * lblfftw3_static;
    stdLineEdit * ledFFTW3_dll;
    QPushButton * cmdBrowseFFTW3;
    QGridLayout * layout_misc0;

    QGroupBox * grp_libHaru;
    QCheckBox * chkUselibHaru;
    QLabel * lblHaru_found;
    QLabel * lblHaru_static;
    stdLineEdit * ledHaru_dll;
    QPushButton * cmdBrowseHaru;
    QGridLayout * layout_misc1;

    QPushButton * cmdSummonWizard;

    QGroupBox * grpMiscMisc;
    QGroupBox * grpPrinting;
    QVBoxLayout * misc2Layout;
    QVBoxLayout * misc3Layout;
    QCheckBox * chkUsePrintCommand;
    stdLineEdit * lenPrintCommand;
    QCheckBox * chkHDPrinterOutput;
    stdIntSelector * selPrintDpi;
    QCheckBox * chkExternalHelpViewer;
    stdLineEdit * lenHelpViewer;
    QCheckBox * chkShowHideException;
    QLabel * lblSmallScreen;
    QCheckBox * chkSmallScreenMain;
    QCheckBox * chkSmallScreenDialogs;
    QGroupBox * grpRemote;
    QGridLayout * layoutRemote;
    QCheckBox * chkAnnounceRemote;

    stdButtonGroup * buttons;

    QVBoxLayout * vbox;
public slots:
    void init(void);//init sets the dialog to represent the variables
    void init_GUI(void);
    void init_Behavior(void);
    void init_Misc(void);
    void read(void);//read sets the variables to represent the dialog
    void read_GUI(void);
    void read_Behavior(void);
    void read_Misc(void);
    void tab_changed(int nr);
    void redisplayContents(void);
    void doClose(void);
    void doApply(void);
    void doAccept(void);
    void doGraceDefaults(void);
    void doQtGraceDefaults(void);
    void doActDevs(void);
    void changeGUIFont(void);
    void resetGUIFont(void);
    void select_BG_Color(void);
    void set_BG_Color_to_Std(void);
    void set_BG_Color_to_Page_BG(void);
    void doBrowseStartup(void);
    void doCurrentAsStartup(void);
    void toggleHTMLviewer(int entry);
    void togglePrintCommand(int entry);
    void toggleQtFonts(bool check);
    void doBrowseFFTW3_dll(void);
    void doBrowseHaru_dll(void);
    void doSummonWizard(void);
    void doTest(void);
    virtual void resizeEvent(QResizeEvent * event);
};

class frmAbout:public QDialog
{
    Q_OBJECT
public:
    frmAbout(QWidget * parent=0);
    ~frmAbout();
    QGroupBox * grpGrace;
    QGroupBox * grpLegal;
    QGroupBox * grpThirdParty;
    QGroupBox * grpBuildInfo;
    QGroupBox * grpHomePage;

    QLabel * lblInfo[40];
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

#endif // EDITWIDGETS_H
