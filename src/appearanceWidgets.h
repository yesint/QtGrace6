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

#ifndef APPEARANCEWIDGETS_H
#define APPEARANCEWIDGETS_H

#include "fundamentals.h"
#include "undo_module.h"
#include "allWidgets.h"
#include "windowWidgets.h"

using namespace std;

class frmPlot_Appearance:public QWidget
{
    Q_OBJECT
public:
    frmPlot_Appearance(QWidget * parent=0);
    ~frmPlot_Appearance();

    QGroupBox * fraFont;
    QGroupBox * fraPageBackgr;
    QGroupBox * fraTimeStamp;
    QGroupBox * fraDescription;
    QTextEdit * txtDescription;
    FontSelector * timestamp_font_item;
    ColorSelector * bg_color_item;
    ColorSelector * timestamp_color_item;
    //QCheckBox * bg_fill_item;
    StdSelector * selFill;
    ImageSelector * imgSelect;
    QCheckBox * timestamp_active_item;
    QCheckBox * chkPathStamp;
    stdLineEdit * ledStampCoords[2];
    stdSlider * timestamp_rotate_item;
    stdSlider * timestamp_size_item;
    LineWidthSelector * selFontSize;//actually just a double-value-selector to change the general font size
    stdButtonGroup * buttonGroup;

    QVBoxLayout * layout;
    QGridLayout * layout1;
    QVBoxLayout * layout2;
    QVBoxLayout * layout3;
    QVBoxLayout * layout4;
public slots:
    void update0(void);
    void update1(int v);
    void update2(QString v);
    void update3(bool v);
    void update4(double v);

    void doApply(void);
    void doAccept(void);
    void doClose(void);
    void init(void);
    void redisplayContents(void);
    void bg_fill_changed(int);
signals:
    void closeWish(void);
};

class frmPlotAppearance:public QDialog
{
    Q_OBJECT
public:
    frmPlotAppearance(QWidget * parent=0);
    ~frmPlotAppearance();
    frmPlot_Appearance * flp;
    QScrollArea * scroll;
    int min_w,min_h,bar_w,bar_h;
public slots:
    void init(void);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
    virtual void resizeEvent(QResizeEvent * event);
};


namespace Ui { class frmSet_Appearance; }

class frmSet_Appearance:public QWidget
{
    Q_OBJECT
public:
    frmSet_Appearance(QWidget * parent=0);
    ~frmSet_Appearance();

    bool updating;
    int cset;

    // --- Main tab ---
    QGroupBox * fraSetPres;
    StdSelector * cmbSetType;
    int number_of_Type_entries;
    int * Type_entries;
    stdLineEditLaTeX * ledString;
    QGroupBox * fraSymbProp;
    PatternComboBox4 * cmbSymbType;
    QDoubleSpinBox * spnSymbSize;
    ColorSelector * cmbSymbColor;
    StdSelector * selSymbChar;
    QGroupBox * fraSetLineProp;
    StdSelector * cmbLineType;
    LineStyleSelector * cmbLineStyle;
    LineWidthSelector * spnLineWidth;
    ColorSelector * cmbLineColor;
    QGroupBox * fraDispOpt;
    QCheckBox * chkAnnVal;
    QDoubleSpinBox * spnAnnValSize;
    QCheckBox * chkDispErrBars;
    QCheckBox * chkIgnoreInAutoscale;
    // --- Symbols tab ---
    QGroupBox * fraSymbOutl;
    LineStyleSelector * cmbSymbStyle;
    FillPatternSelector * cmbSymbPattern;
    LineWidthSelector * spnSymbWidth;
    QGroupBox * fraSymbFill;
    ColorSelector * cmbSymbFillColor;
    FillPatternSelector * cmbSymbFillPattern;
    QGroupBox * fraExtra;
    stdIntSelector * spnSymbSkip;
    FontSelector * cmbSymbFont;
    // --- Line tab ---
    QGroupBox * fraDropLineProp;
    FillPatternSelector * cmbDropPattern;
    QCheckBox * chkDrawDropLines;
    QGroupBox * fraFillProp;
    StdSelector * cmbFillType;
    StdSelector * cmbRule;
    FillPatternSelector * cmbAreaFillPattern;
    ColorSelector * cmbAreaFillColor;
    SetSelectorCombo * cmbSet;
    QGroupBox * fraBaseLine;
    StdSelector * cmbBaseType;
    QCheckBox * chkDrawLine;
    // --- Ann. values tab ---
    QGroupBox * fraTextProp;
    FontSelector * cmbFont;
    AlignmentSelector * selAlign;
    ColorSelector * cmbAnnColor;
    QSlider * sldFontAngle;
    QSpinBox * spnFontAngle;
    stdLineEditLaTeX * ledPrepend;
    stdLineEditLaTeX * ledAppend;
    QGroupBox * fraFormatOpt;
    StdSelector * cmbAnnValType;
    StdSelector * cmbPrecision;
    StdSelector * cmbFormat;
    QGroupBox * fraPlacement;
    stdLineEdit * ledXOffs;
    stdLineEdit * ledYOffs;
    // --- Error bars tab ---
    QGroupBox * fraCommon;
    StdSelector * cmbPlacement;
    ColorSelector * cmbErrColor;
    FillPatternSelector * cmbErrPattern;
    StdSelector * chkConnectErrorBars;
    QCheckBox * chkShowInLegend;
    QGroupBox * fraClipping;
    QCheckBox * chkArrowClip;
    LineWidthSelector * spnMaxLength;
    QGroupBox * fraBarLine;
    QDoubleSpinBox * spnBarSize;
    LineWidthSelector * spnbarWidth;
    LineStyleSelector * cmbBarStyle;
    QGroupBox * fraRiserLine;
    LineWidthSelector * spnRiserWidth;
    LineStyleSelector * cmbRiserStyle;

    // --- Outer shell ---
    QMenuBar * menuBar;
    QMenu * mnuFile;
    QMenu * mnuEdit;
    QMenu * mnuOptions;
    QMenu * mnuHelp;
    QLabel * lblSelSet;
    uniList * listSet;
    QTabWidget * tabs;
    stdButtonGroup * buttonGroup;

    QAction * actclose,*actOpenFontTool,*acthelponcontext,*acthelponsetappearance;
    QAction * actdupllegends,*actcolorsync,*actapplyall;
    QAction * actsetdiffcolors,*actsetdifflinestyles,*actsetdifflinewidths,*actsetdiffsymbols,*actsetbaw;
    QAction * actloadcoments,*actstriplegends;

public slots:
    void CreateActions(void);
    void init(void);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
    void doOpenFontTool(void);
    void doHelpOnContext(void);
    void doHelpOnSetAppearance(void);
    void doDuplLegends(void);
    void doColorSync(void);
    void doApplyAll(void);
    void doSetDiffColors(void);
    void doSetDiffLineStyles(void);
    void doSetDiffLineWidths(void);
    void doSetDiffSymbols(void);
    void doSetBlackAndWhite(void);
    void doLoadComments(void);
    void doStripLegends(void);
    void ApplyListOfChanges(void);
    void update0(void);
    void update1(int v);
    void update2(QString v);
    void update3(bool v);
    void update4(double v);
    void newListSelection(int a);
    void ShowSetData_external(int graph_number,int set_number);
    void showSetData(int graph_number,int set_number);
    void writeSetData(int graph_number,int set_number);
    void SyncColors(int val);
    void SyncColors2(int val);
    void setapp_data_proc(int dat);
    void redisplayContents(void);
    void SymbTypeChanged(int val);
    void LineColorChanged(int val);
    void SymbColorChanged(int val);
    void updateSymbolTypeIcons(void);
signals:
    void closeWish(void);
    void colorChanged(int val);
    void colorChanged2(int val);
private:
    Ui::frmSet_Appearance *ui;
};

class dialogScrollArea:public QScrollArea
{
    Q_OBJECT
public:
    dialogScrollArea(QWidget * parent=0);
public slots:
    virtual void keyPressEvent(QKeyEvent * k);
};

class frmSetAppearance:public QDialog
{
    Q_OBJECT
public:
    frmSetAppearance(QWidget * parent=0);
    ~frmSetAppearance();
    frmSet_Appearance * flp;
    uniList * listSet;
public slots:
    void init(void);
    void ShowSetData_external(int graph_number,int set_number);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
    virtual void showEvent(QShowEvent * event);
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
    stdLineEdit * selphi0;
    LineWidthSelector * selroffset;
    stdLineEdit * ledTitle;
    stdLineEdit * ledSubtitle;
    stdLineEdit * ledCoords[4];
    QCheckBox * chkDisplLegend;
    QCheckBox * chkFlipXY;

    QVBoxLayout * layout;
    QGridLayout * layout1;
    QVBoxLayout * layout2;
    QGridLayout * layout3;
    QHBoxLayout * layout4;
public slots:
    void serviceGrTypeSel(int nr);
};

class GrTabTitles:public QWidget
{
    Q_OBJECT
public:
    GrTabTitles(QWidget * parent=0);

    QGroupBox * grpTitle;
    QGroupBox * grpSubTitle;

    FontSelector * selTitleFont;
    AlignmentSelector * selAlign;
    stdSlider * sldTitleCharSize;
    ColorSelector * selTitleColor;
    stdLineEdit * ledTitleShiftX;
    stdLineEdit * ledTitleShiftY;

    FontSelector * selSubFont;
    AlignmentSelector * selSubAlign;
    stdSlider * sldSubCharSize;
    ColorSelector * selSubColor;
    stdLineEdit * ledSubTitleShiftX;
    stdLineEdit * ledSubTitleShiftY;

    QVBoxLayout * layout;
    QGridLayout * layout1;
    QGridLayout * layout2;
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
    AlignmentSelector * selAlign;
    stdSlider * sldTextSize;
    ColorSelector * selTextColor;

    StdSelector * selVGap;
    StdSelector * selHGap;
    StdSelector * selLineLength;
    QCheckBox * chkPutRevOrder;

    QVBoxLayout * layout;
    QGridLayout * layout1;
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
signals:
    void closeWish(void);
};

class frmGraphApp:public QDialog
{
    Q_OBJECT
public:
    frmGraphApp(QWidget * parent=0);
    ~frmGraphApp();
    frmGraph_App * flp;
    dialogScrollArea * scroll;
    //QScrollArea * scroll;
    uniList * listGraph;
    int min_w,min_h,bar_w,bar_h;
public slots:
    void init(void);
    void show_graph_data_external(int n_gno);
    void update_view(int gno);
    void updatelegends(int gno);
    void doApply(void);
    void doAccept(void);
    void doClose(void);
    virtual void resizeEvent(QResizeEvent * event);
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
    AlignmentSelector * selAlign;
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
    AlignmentSelector * selAlign;
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

    QGroupBox * grpQuick;
    QHBoxLayout * layout3;
    QPushButton * cmdQuickNormal;
    QPushButton * cmdQuickDegrees;
    QPushButton * cmdQuickPis;
    QPushButton * cmdQuickAlt;

    QWidget * empty;

    QVBoxLayout * layout;
public slots:
    void doQuickNormal(void);
    void doQuickDegrees(void);
    void doQuickPis(void);
    void doQuickAlt(void);
    void locationChanged(int i);
signals:
    void quickSetDegrees(void);
    void quickSetPis(void);
    void quickSetNormal(void);
    void quickSetAlt(void);
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

    int curaxis,apply_to_selection,transfer_tick_labels,transfer_label;

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
    QCheckBox * chkTransferLabel;
    QCheckBox * chkTransferTickLabel;
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
    void doQuickNormalTicks(void);
    void doQuickDegreesTicks(void);
    void doQuickPisTricks(void);
    void doQuickAltTricks(void);
    //immediateUpdates
    void update0(void);
    void update1(int v);
    void update2(QString v);
    void update3(bool v);
    void update4(double v);
signals:
    void closeWish(void);
};

class frmAxisProp:public QDialog
{
    Q_OBJECT
public:
    frmAxisProp(QWidget * parent=0);
    ~frmAxisProp();
    frmAxis_Prop * flp;
    QScrollArea * scroll;
    int min_w,min_h,bar_w,bar_h;
public slots:
    void update_ticks(int gno);
    void create_axes_dialog(int axisno);
    void doAccept(void);
    void doClose(void);
    virtual void resizeEvent(QResizeEvent * event);
};

#endif // APPEARANCEWIDGETS_H
