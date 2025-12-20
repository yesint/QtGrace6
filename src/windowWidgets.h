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

#ifndef WINDOWWIDGETS_H
#define WINDOWWIDGETS_H

#include "allWidgets.h"

using namespace std;

QString specialExtractCommand(int type,int gno,int sno,double data);

class frmCommands:public QDialog
{
    Q_OBJECT
public:
    frmCommands(QWidget * parent=0);
    ~frmCommands();
    int min_w,min_h,bar_w,bar_h;
    QScrollArea * scroll;
    QWidget * flp;
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
    void updateRowColor(int r);
    virtual void resizeEvent(QResizeEvent * event);
};

class frmPointExplorer:public QDialog
{
    Q_OBJECT
public:
    frmPointExplorer(QWidget * parent=0);
    ~frmPointExplorer();
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

#define MASTER_ALIGN_TARGET_NONE 0
#define MASTER_ALIGN_TARGET_GRAPH 1
#define MASTER_ALIGN_TARGET_PAGE 2

#define MASTER_ALIGN_X_NONE 0
#define MASTER_ALIGN_X_LEFT_IN 1
#define MASTER_ALIGN_X_LEFT_OUT 2
#define MASTER_ALIGN_X_CENTER 3
#define MASTER_ALIGN_X_RIGHT_IN 4
#define MASTER_ALIGN_X_RIGHT_OUT 5

#define MASTER_ALIGN_Y_NONE 0
#define MASTER_ALIGN_Y_DOWN_IN 1
#define MASTER_ALIGN_Y_DOWN_OUT 2
#define MASTER_ALIGN_Y_CENTER 3
#define MASTER_ALIGN_Y_UP_IN 4
#define MASTER_ALIGN_Y_UP_OUT 5

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
    StdSelector * selGno;
    StdSelector * selMainAlignTarget;
    StdSelector * selMainAlignX;
    StdSelector * selMainAlignY;
    PositionSelector * strings_loc_item;
    stdLineEdit * ledCoords[2];
    stdSlider * strings_rot_item;
    stdSlider * strings_size_item;
    stdButtonGroup * buttonGroup;
    QVBoxLayout * layout;
public slots:
    void init(int id);
    void reDisplayContents(void);
    void viewCoordsChanged(int i);
    void updateMasterAlign(int i, int keep_xy_setting=TRUE);
    void updateXYafterAlign(int i);
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
    ~frmTextProps();
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
    StdSelector * selGno;
    PositionSelector * lines_loc_item;
    stdLineEdit * ledCoords[4];
    stdButtonGroup * buttonGroup;

    QGroupBox * fraArrow;
    StdSelector * lines_arrow_item;
    StdSelector * lines_atype_item;
    LineWidthSelector * lines_asize_item;
    LineWidthSelector * lines_a_dL_ff_item;
    LineWidthSelector * lines_a_lL_ff_item;

    QWidget * empty;
    QHBoxLayout * layout3;
    QComboBox * cmbStartEnd;
    QPushButton * cmdEdit;

    QVBoxLayout * layout;
    QVBoxLayout * layout2;
public slots:
    void init(int id);
    void reDisplayContents(void);
    void viewCoordsChanged(int i);
    void doAccept(void);
    void doClose(void);
    void doEdit(void);
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
    ~frmLineProps();
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
    bool ellipse;
    //true --> it is really an ellipse
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
    StdSelector * selFill;
    StdSelector * selGno;
    ImageSelector * imgSelect;
    stdButtonGroup * buttonGroup;

    QVBoxLayout * layout;
public slots:
    void init(int id);
    void reDisplayContents(void);
    void viewCoordsChanged(int i);
    void fillingChanged(int i);
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
    ~frmEllipseProps();
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

    QIcon buttonIcons[17];
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
    void CreateIcons(void);
    void redisplayButtons(void);
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
virtual void resizeEvent(QResizeEvent * event);
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
    ~frmFontTool();

    FontSelector * selFont;
    GlyphPanel * panel[16*338];
    stdLineEdit * ledString;
    stdLineEdit * ledAscii,*ledUnicode;
    QWidget * background;
    QScrollArea * scroll;
    QString textString;
    QGridLayout * layout0;
    QGridLayout * layout;
    QPushButton * cmdUseSymbolValue;
    stdButtonGroup * buttonGroup;
    int marked,tileCount;
    unsigned short last_character;
public slots:
    void characterInserted(QString text);
    void insertAtCursor(QString c);
    void FontChanged(int i);
    void newClick(int i);
    void doUseSymbolValue(void);
    void updateGlyphSize(void);
    void doApply(void);
    void doClose(void);
    void doAccept(void);
};

class frmRealTimeInputManager:public QDialog
{
    Q_OBJECT
public:
    frmRealTimeInputManager(QWidget * parent=0);
    ~frmRealTimeInputManager();
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

#endif // WINDOWWIDGETS_H
