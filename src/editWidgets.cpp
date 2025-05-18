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

#include "editWidgets.h"

#ifdef _MSC_VER
#include "windows.h"
#endif

#define cg get_cg()

//int default_Print_Device;//-1=last one
extern char dummy[];
extern int max_node_nr;
extern bool wait_till_update;
extern bool ApplyError;
extern bool GlobalInhibitor;
extern bool immediateUpdate;
extern bool updateRunning;
extern int regiontype;
extern int ReqUpdateColorSel;
extern region undo_region;
extern region undo_region2;
//bool silent_nonl_fit;
extern QStringList ListOfChanges;
extern QStringList ListOfOldStates;
extern OptionItem fmt_option_items[NUM_FMT_OPTION_ITEMS];
extern CMap_entry *cmap_table;
extern CMap_entry cmap_init[16];
extern QFile path_to_fftw3_lib,path_to_libharu;
extern int AAGrayLevelsOK;
extern QMap<unsigned char,ushort> key_for_greek;
extern bool showhideworkaround;
extern bool use_new_icons;
extern QFont * GuiFont,*stdGuiFont;
extern QList<QByteArray> avcod;
//extern QTextCodec * FileCodec;
extern int max_history;
extern int current_history;
extern QString history[MAX_HISTORY];
extern int current_language;
extern int default_Print_Device;
extern bool useQtFonts;
extern bool symbol_font_is_special;
extern bool use_print_command;
extern bool display_help_externally;
extern char SystemsDecimalPoint;
extern char OldDecimalPoint;
extern char DecimalPointToUse;
extern bool display_help_externally;

extern defaults d_d;
extern defaults file_defaults;
extern defaults ini_defaults;
extern defaults tmp_defaults;

extern view d_v;
extern view file_view;
extern view ini_view;
extern view tmp_view;

extern QSize LastSize_FormDataSetProperties;
extern QSize LastSize_FormSetOperations;
extern QSize LastSize_FormArrangeGraphs;
extern QSize LastSize_FormOverlayGraphs;
extern QSize LastSize_FormAutoScaleGraphs;
extern QSize LastSize_FormHotLinks;
extern QSize LastSize_FormSelectHotLink;
extern QSize LastSize_FormLocatorProps;
extern QSize LastSize_FormUndoList;
extern QSize LastSize_FormExplorer;
extern QSize LastSize_FormRegionMaster;
extern QSize LastSize_FormRegionStatus;
extern QSize LastSize_FormReportRegion;
extern QSize LastSize_FormClearRegion;
extern QSize LastSize_FormDefineRegion;
extern QSize LastSize_FormAbout;
extern QSize LastSize_Form_Preferences;

extern frmDeviceSetup * FormDeviceSetup;
extern frmFontTool * FormFontTool;
extern frmPlotAppearance * FormPlotAppearance;
extern frmGraphApp * FormGraphAppearance;
extern frm_Preferences * Form_Preferences;
extern frmCommands * FormCommands;
extern frmIOForm * FormSelectHotLink;
extern frmSetAppearance * FormSetAppearance;
extern frmAxisProp * FormAxisProperties;
extern frmSetOperations * FormSetOPS;
extern frmUndoList * FormUndoList;
extern QPixmap * GraceIcon;
extern MainWindow * mainWin;

extern char DecimalPointToUse;
extern QLocale * comma_locale;//a setting where ',' is the decimal separator (we use the setting for Germany here)
extern QLocale * dot_locale;//a setting where '.' is the decimal separator (we use the setting for the USA here)

#ifdef __cplusplus
extern "C" {
#endif
extern double rint_2(double v);
#ifdef __cplusplus
}
#endif

extern int nr_of_true_changes(QStringList & list);
extern void SetDecimalSeparatorToUserValue(char * str,bool remove_space=true);
extern void strToUpper(char * tar,const char * ch);
extern double rint_v2(double x);
extern void init_color_icons(int nr_of_cols,CMap_entry * entries,int & allocated_colors,QIcon *** ColorIcons,QPixmap *** ColorPixmaps,QString *** ColorNames);
extern void UpdateAllWindowContents(void);
extern void updateAllSelectors(void);
extern void update_font_selectors(bool appearance);
extern void change_language(int index);
extern void update_default_props(void);
extern char *next_token(char *s, char **token, int *quoted);
extern void exchangePointComma(char * token);
extern void exchangePointCommaInFormat(char * token,int type);
extern void convert_Grace_Strings(void);
extern void copy_LaTeX_to_Grace(void);
extern void convert_all_strings_from_encoding_to_UTF8(void);
extern void copy_Grace_to_LaTeX(void);
extern void check_external_lib_usability(void);
extern void create_line_Patterns(void);
extern void read_basic_icons(void);
extern void updateUniLists(void);
extern void changeDragInUniLists(void);

QString regtypes[10];
int reg_order[10];

frmDataSetProperties::frmDataSetProperties(QWidget * parent):QDialog(parent)
{
    int number;
    char dummy[128];
    QString entr[50];
//setFont(*stdFont);
    setWindowTitle(tr("QtGrace: Data set properties"));
    setWindowIcon(QIcon(*GraceIcon));
    gno=sno=-1;//invalid
    CreateActions();

    menuBar=new QMenuBar(this);
    mnuFile=new QMenu("&File",this);
    mnuFile->setTearOffEnabled(TRUE);
    mnuFile->addAction(actClose);
    mnuEditData=new QMenu("&Edit data",this);
    mnuEditData->setTearOffEnabled(TRUE);
    mnuEditData->addAction(actEditInSpreadsheet);
    mnuEditData->addAction(actEditInTextEditor);
    mnuCreateNew=new QMenu("Create &new",this);
    mnuCreateNew->setTearOffEnabled(TRUE);
    mnuCreateNew->addAction(actCreateNewByFormula);
    mnuCreateNew->addAction(actCreateNewInSpreadsheet);
    mnuCreateNew->addAction(actCreateNewInTextEditor);
    mnuCreateNew->addAction(actCreateNewFromBlockData);
    mnuEdit=new QMenu("&Edit",this);
    mnuEdit->setTearOffEnabled(TRUE);
    mnuEdit->addAction(actDuplicate);
    mnuEdit->addAction(actKillData);
    mnuEdit->addSeparator();
    mnuEdit->addMenu(mnuEditData);
    mnuEdit->addMenu(mnuCreateNew);
    mnuEdit->addSeparator();
    mnuEdit->addAction(actSetAppearance);
    mnuEdit->addAction(actSetOperations);
    mnuHelp=new QMenu("&Help",this);
    mnuHelp->setTearOffEnabled(TRUE);
    mnuHelp->addAction(actHelpOnContext);
    mnuHelp->addSeparator();
    mnuHelp->addAction(actHelpOnSetProp);

    menuBar->addMenu(mnuFile);
    menuBar->addMenu(mnuEdit);
    menuBar->addSeparator();
    menuBar->addMenu(mnuHelp);

    number=NUMBER_OF_SETTYPES;
    for (int i=0;i<number;i++)
    {
        strToUpper(dummy,set_types(i));
        entr[i]=QString(dummy);
    }
    selType=new StdSelector(this,tr("Type:"),number,entr);
    ledLength=new stdLineEdit(this,tr("Length:"));
    ledLength->lenText->setText(QString(""));
    ledComment=new stdLineEdit(this,tr("Comment:"),true);
    ledComment->lenText->setText(QString(""));
    grpStatistics=new QGroupBox(tr("Statistics"),this);

    background=new QWidget;
    layout0=new QGridLayout;
    //layout0->setMargin(STD_MARGIN);
    layout0->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    HLabels[0]=new QLabel(QString(""),background);
    HLabels[1]=new QLabel(tr("Min"),background);
    HLabels[2]=new QLabel(tr("at"),background);
    HLabels[3]=new QLabel(tr("Max"),background);
    HLabels[4]=new QLabel(tr("at"),background);
    HLabels[5]=new QLabel(tr("Mean"),background);
    HLabels[6]=new QLabel(tr("Stdev"),background);

    VLabels[0]=new QLabel(tr("X"),background);
    VLabels[1]=new QLabel(tr("Y"),background);
    VLabels[2]=new QLabel(tr("Y1"),background);
    VLabels[3]=new QLabel(tr("Y2"),background);
    VLabels[4]=new QLabel(tr("Y3"),background);
    VLabels[5]=new QLabel(tr("Y4"),background);
    layout0->addWidget(HLabels[0],0,0);
    for (int i=0;i<6;i++)//row
    {
        layout0->addWidget(VLabels[i],i+1,0);
        layout0->addWidget(HLabels[1+i],0,i+1);
        for (int j=0;j<6;j++)//Column
        {
            lenStat[i*6+j]=new QLineEdit(QString(""),background);
            layout0->addWidget(lenStat[i*6+j],1+i,1+j);
        }
    }
    background->setLayout(layout0);

    layout1=new QHBoxLayout;
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    scroll=new QScrollArea(grpStatistics);
    scroll->setWidget(background);
    layout1->addWidget(scroll);
    grpStatistics->setLayout(layout1);

    lblDataSet=new QLabel(tr("Data sets:"),this);
    listDataSets=new uniList(SETLIST,this);
    connect(listDataSets,SIGNAL(new_selection(int)),SLOT(setChanged(int)));
    connect(listDataSets,SIGNAL(clicked(QModelIndex)),SLOT(setChanged(QModelIndex)));

    buttonGroup=new stdButtonGroup(this);
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));

    layout=new QGridLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
#ifndef MAC_SYSTEM
    layout->addWidget(menuBar,0,0,1,2);
#endif
    layout->addWidget(lblDataSet,1,0,1,2);
    layout->addWidget(listDataSets,2,0,1,2);
    layout->addWidget(selType,3,0);
    layout->addWidget(ledLength,3,1);
    layout->addWidget(ledComment,4,0,1,2);
    layout->addWidget(grpStatistics,5,0,1,2);
    layout->addWidget(buttonGroup,6,0,1,2);

    layout->setRowStretch(0,0);
    layout->setRowStretch(1,0);
    layout->setRowStretch(2,1);
    layout->setRowStretch(3,0);
    layout->setRowStretch(4,0);
    layout->setRowStretch(5,1);
    layout->setRowStretch(6,0);
    setLayout(layout);
    resize(LastSize_FormDataSetProperties);
}

frmDataSetProperties::~frmDataSetProperties()
{
    LastSize_FormDataSetProperties=this->size();
}

void frmDataSetProperties::adjustSizeToFont(void)
{
    for (int i=0;i<6;i++)//row
    {
        for (int j=0;j<6;j++)//Column
        {
        lenStat[i*6+j]->setMinimumHeight(lenStat[i*6+j]->font().pixelSize()+9);
        }
    }
    background->adjustSize();
}

void frmDataSetProperties::CreateActions(void)
{
    actClose=new QAction(tr("&Close"),this);
    connect(actClose,SIGNAL(triggered()), this, SLOT(doClose()));
    actHelpOnContext=new QAction(tr("On conte&xt"),this);
    actHelpOnContext->setShortcut(tr("Shift+F1"));
    connect(actHelpOnContext,SIGNAL(triggered()), this, SLOT(doHelpOnContext()));
    actHelpOnSetProp=new QAction(tr("On data &sets"),this);
    connect(actHelpOnSetProp,SIGNAL(triggered()), this, SLOT(doHelpOnSetProp()));
    actKillData=new QAction(tr("Kill D&ata"),this);
    connect(actKillData,SIGNAL(triggered()), this, SLOT(doKillData()));
    actDuplicate=new QAction(tr("&Duplicate"),this);
    connect(actDuplicate,SIGNAL(triggered()), this, SLOT(doDuplicate()));
    actSetAppearance=new QAction(tr("&Set appearance..."),this);
    connect(actSetAppearance,SIGNAL(triggered()), this, SLOT(doSetAppearance()));
    actSetOperations=new QAction(tr("Set &operations..."),this);
    connect(actSetOperations,SIGNAL(triggered()), this, SLOT(doSetOperations()));
    actEditInSpreadsheet=new QAction(tr("In &spreadsheet"),this);
    connect(actEditInSpreadsheet,SIGNAL(triggered()), this, SLOT(doEditInSpreadsheet()));
    actEditInTextEditor=new QAction(tr("In t&ext editor"),this);
    connect(actEditInTextEditor,SIGNAL(triggered()), this, SLOT(doEditInTextEditor()));
    actCreateNewByFormula=new QAction(tr("By &formula"),this);
    connect(actCreateNewByFormula,SIGNAL(triggered()), this, SLOT(doCreateNewByFormula()));
    actCreateNewInSpreadsheet=new QAction(tr("In &spreadsheet"),this);
    connect(actCreateNewInSpreadsheet,SIGNAL(triggered()), this, SLOT(doCreateNewInSpreadsheet()));
    actCreateNewInTextEditor=new QAction(tr("In t&ext editor"),this);
    connect(actCreateNewInTextEditor,SIGNAL(triggered()), this, SLOT(doCreateNewInTextEditor()));
    actCreateNewFromBlockData=new QAction(tr("Form &block data"),this);
    connect(actCreateNewFromBlockData,SIGNAL(triggered()), this, SLOT(doCreateNewFromBlockData()));
}

void frmDataSetProperties::doClear(void)
{
    QString clString("");
    selType->setEnabled(false);
    ledLength->setText(clString);
    ledComment->setText(clString);
    for (int i=0;i<36;i++)
        lenStat[i]->setText(clString);
}

void frmDataSetProperties::setChanged(QModelIndex index)
{
    if (index.isValid())
    {
        setChanged(listDataSets->entries[index.row()]);
    }
}

void frmDataSetProperties::setChanged(int nr)
{
    double *datap;
    int i,j;
    double dmin, dmax;
    int imin, imax;
    double dmean, dsd;
    gno=listDataSets->gr_no;
    sno=nr;
    int ncols = dataset_cols(gno, sno);
    int number_of_sel_sets=-1;
    int * selection=new int[2];
    listDataSets->get_selection(&number_of_sel_sets,&selection);
    if (number_of_sel_sets!=1 || !is_valid_setno(gno,nr))
    {
        doClear();
        ncols=-1;
        return;
    }
    selType->setEnabled(true);
    char buf[1024];
    ledComment->setText(QString(getcomment(gno, sno)));
    //xv_setstr(tui.comment_item, getcomment(gno, setno));
    sprintf(buf, "%d", getsetlength(gno, sno));
    ledLength->setText(QString(buf));
    //xv_setstr(tui.length_item, buf);
    selType->setCurrentIndex(dataset_type(gno, sno));
    //SetOptionChoice(tui.datatype_item, dataset_type(gno, setno));
    for (i = 0; i < MAX_SET_COLS; i++)
    {
        datap = getcol(gno, sno, i);
        minmax(datap, getsetlength(gno, sno), &dmin, &dmax, &imin, &imax);
        stasum(datap, getsetlength(gno, sno), &dmean, &dsd);
        for (j = 0; j < 6; j++)
        {
            if (i < ncols)
            {
                switch (j)
                {
                case 0:
                    sprintf(buf, "%g", dmin);
                    break;
                case 1:
                    sprintf(buf, "%d", imin);
                    break;
                case 2:
                    sprintf(buf, "%g", dmax);
                    break;
                case 3:
                    sprintf(buf, "%d", imax);
                    break;
                case 4:
                    sprintf(buf, "%g", dmean);
                    break;
                case 5:
                    sprintf(buf, "%g", dsd);
                    break;
                default:
                    strcpy(buf, "");
                    break;
                }
                SetDecimalSeparatorToUserValue(buf);
                lenStat[i*6+j]->setText(QString(buf));
                //tui.rows[i][j] = copy_string(tui.rows[i][j], buf);
            }
            else
            {
                lenStat[i*6+j]->setText(QString(""));
                //tui.rows[i][j] = copy_string(tui.rows[i][j], "");
            }
        }
    }

}

void frmDataSetProperties::doKillData(void)
{
    listDataSets->popupMenu1->doKillData();
    int act_sav=undo_active;
    undo_active=FALSE;
    listDataSets->hide();
    listDataSets->show();
    undo_active=act_sav;
    repaint();
}

void frmDataSetProperties::doDuplicate(void)
{
    listDataSets->popupMenu1->doDuplicate();
    int act_sav=undo_active;
    undo_active=FALSE;
    listDataSets->hide();
    listDataSets->show();
    undo_active=act_sav;
    repaint();
}

void frmDataSetProperties::doSetAppearance(void)
{
    bool old_upd;
    old_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    if (FormSetAppearance==NULL)
    {
        FormSetAppearance=new frmSetAppearance(this);
    }
    FormSetAppearance->init();
    FormSetAppearance->show();
    FormSetAppearance->raise();
    immediateUpdate=old_upd;
    updateRunning=false;
}

void frmDataSetProperties::doSetOperations(void)
{
    if (FormSetOPS==NULL)
    {
        FormSetOPS=new frmSetOperations(this);
    }
    FormSetOPS->init();
    FormSetOPS->show();
    FormSetOPS->raise();
}

void frmDataSetProperties::doHelpOnContext(void)
{
    mainWin->setCursor(QCursor(Qt::WhatsThisCursor));
}

void frmDataSetProperties::doHelpOnSetProp(void)
{
    HelpCB("doc/UsersGuide.html#data-sets");
}

void frmDataSetProperties::doEditInSpreadsheet(void)
{
    listDataSets->popupMenu1->doEditInSpreadSheet();
}

void frmDataSetProperties::doEditInTextEditor(void)
{
    listDataSets->popupMenu1->doEditInTextEditor();
}

void frmDataSetProperties::doCreateNewByFormula(void)
{
    listDataSets->popupMenu1->doCreateByFormula();
}

void frmDataSetProperties::doCreateNewInSpreadsheet(void)
{
    listDataSets->popupMenu1->doCreateInSpreadsheet();
}

void frmDataSetProperties::doCreateNewInTextEditor(void)
{
    listDataSets->popupMenu1->doCreateInTextEditor();
}

void frmDataSetProperties::doCreateNewFromBlockData(void)
{
    listDataSets->popupMenu1->doCreateFromBlockData();
}

void frmDataSetProperties::init(void)
{
    listDataSets->set_graph_number(get_cg(),false);
    //listDataSets->update_number_of_entries();
}

void frmDataSetProperties::doApply(void)
{
    ApplyError=false;
    int error = false;
    int *selset=new int[2], nsets, i, len, setno, type;
    //char *s;
    int * gnos;
    int index;

    nsets = GetListChoices(listDataSets, &selset);

    if (nsets < 1) {
        errmsg(tr("No set selected").toLocal8Bit().constData());
        ApplyError=true;
    } else {
        type = GetOptionChoice(selType);
        xv_evalexpri(ledLength, &len);
        if (len < 0)
        {
            errmsg(tr("Negative set length!").toLocal8Bit().constData());
            ApplyError=true;
            error = TRUE;
        }
        //s = xv_getstr(ledComment);

        if (error == FALSE)
        {
            gnos=new int[nsets+2];
            for (i=0;i<nsets;i++)
                gnos[i]=get_cg();
            SaveSetStatesPrevious(nsets,gnos,selset,UNDO_COMPLETE);
            index=0;
            for (i = 0; i < nsets; i++)
            {
                setno = selset[i];
                //I do not check for the set comment intentionally!!
                if (type!=dataset_type(get_cg(), setno) || len!=getsetlength(get_cg(), setno)) index++;
                set_dataset_type(get_cg(), setno, type);
                setlength(get_cg(), setno, len);
                //setcomment(get_cg(), setno, s);
                setcomment(get_cg(), setno, ledComment->lenText->text().toLocal8Bit().constData());
            }

            if (index>0)
            {
                SetsModified(nsets,gnos,selset,UNDO_COMPLETE);
            }
            update_set_lists(cg);
            mainWin->mainArea->completeRedraw();
            //return RETURN_SUCCESS;
        }
    }
    delete[] selset;
}

void frmDataSetProperties::doClose(void)
{
    hide();
}

void frmDataSetProperties::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

frmSetOperations::frmSetOperations(QWidget * parent):QDialog(parent)
{
    int number;
    QString entr[5];
//setFont(*stdFont);
    setWindowTitle(tr("QtGrace: Set operations"));
    setWindowIcon(QIcon(*GraceIcon));

    grpSource=new grpSelect(tr("Source"),this);
    grpDestination=new grpSelect(tr("Destination"),this);
    grpDestination->enable_sync(grpSource->listSet);

    number=4;
    entr[0]=tr("Copy");
    entr[1]=tr("Move");
    entr[2]=tr("Swap");
    entr[3]=tr("Copy Legend");
    selType=new StdSelector(this,tr("Type of operation:"),number,entr);
    selType->entryValues[0]=OPTYPE_COPY;
    selType->entryValues[1]=OPTYPE_MOVE;
    selType->entryValues[2]=OPTYPE_SWAP;
    selType->entryValues[3]=OPTYPE_COPY_LEGEND;

    buttonGroup=new stdButtonGroup(this);
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));

    layout=new QGridLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(grpSource,0,0);
    layout->addWidget(grpDestination,0,1);
    layout->addWidget(selType,1,0,1,2);
    layout->addWidget(buttonGroup,2,0,1,2);
    setLayout(layout);
    resize(LastSize_FormSetOperations);
}

frmSetOperations::~frmSetOperations()
{
    LastSize_FormSetOperations=this->size();
}

void frmSetOperations::init(void)
{
    grpSource->update_number_of_entries();
    grpDestination->update_number_of_entries();
    int nr_sel=1;
    int sel=get_cg();
    grpSource->listGraph->set_new_selection(nr_sel,&sel);
    grpSource->listSet->set_graph_number(sel,false);
    grpSource->listSet->update_number_of_entries();
    grpDestination->listGraph->set_new_selection(nr_sel,&sel);
    grpDestination->listSet->set_graph_number(sel,false);
    grpDestination->listSet->update_number_of_entries();
}

void frmSetOperations::doApply(void)
{
    ApplyError=false;
    int optype, error;
    int i, g1_ok, g2_ok, ns1, ns2, gno1, gno2, setno2;
    int *svalues1=new int[2];
    int *svalues2=new int[2];
    char dummy[128];

    optype = selType->currentValue();

    grpSource->listGraph->get_selection(&ns1,&svalues1);
    grpDestination->listGraph->get_selection(&ns2,&svalues2);
    gno1=gno2=0;
    if (ns1!=1)
    {
        g1_ok=RETURN_FAILURE;
    }
    else
    {
        g1_ok=RETURN_SUCCESS;
        gno1=svalues1[0];
    }
    if (ns2!=1)
    {
        g2_ok=RETURN_FAILURE;
    }
    else
    {
        g2_ok=RETURN_SUCCESS;
        gno2=svalues2[0];
    }
    grpSource->listSet->get_selection(&ns1,&svalues1);
    grpDestination->listSet->get_selection(&ns2,&svalues2);

    error = FALSE;
    if (g1_ok == RETURN_FAILURE || g2_ok == RETURN_FAILURE) {
        error = TRUE;
        ApplyError=true;
        errmsg(tr("Please select single source and destination graphs").toLocal8Bit().constData());
    } else if (ns1 == 0) {
        error = TRUE;
        ApplyError=true;
        errmsg(tr("No source sets selected").toLocal8Bit().constData());
    } else if (ns2 == 0 && (optype == OPTYPE_SWAP || optype == OPTYPE_COPY_LEGEND)) {
        error = TRUE;
        ApplyError=true;
        errmsg(tr("No destination sets selected").toLocal8Bit().constData());
    } else if (ns1 != ns2 && (optype == OPTYPE_SWAP || optype == OPTYPE_COPY_LEGEND || ns2 != 0)) {
        error = TRUE;
        ApplyError=true;
        errmsg(tr("Different number of source and destination sets").toLocal8Bit().constData());
    } else if (gno1 == gno2 && ns2 == 0 && optype == OPTYPE_MOVE) {
        error = TRUE;
        ApplyError=true;
        errmsg(tr("Can't move a set to itself").toLocal8Bit().constData());
    } else {
        int * gnos=new int[ns1+ns2+2];
        int * snos=new int[ns1+ns2+2];
        int index=0;
        if (ns1==1)
        {
            sprintf(dummy," [G%d.S%d]",gno1,svalues1[0]);
        }
        if (optype!=OPTYPE_MOVE)
        {//save only the states in the target area
            for (i=0;i<ns2;i++)
                gnos[i]=gno2;
            if (optype==OPTYPE_COPY_LEGEND)
                SaveSetStatesPrevious(ns2,gnos,svalues2,UNDO_APPEARANCE);
            else
                SaveSetStatesPrevious(ns2,gnos,svalues2,UNDO_COMPLETE);
        }
        else
        {//save origin and target
            for (i=0;i<ns1;i++)
            {
                gnos[i]=gno1;
                snos[i]=svalues1[i];
            }
            for (i=0;i<ns2;i++)
            {
                gnos[i+ns1]=gno2;
                snos[i+ns1]=svalues2[i];
            }
            SaveSetStatesPrevious(ns2+ns1,gnos,snos,UNDO_COMPLETE);
        }
        for (i = 0; i < ns1; i++) {
            switch (optype) {
            case OPTYPE_SWAP:
                UndoSwapSets(gno1, svalues1[i], gno2, svalues2[i]);
                if (do_swapset(gno1, svalues1[i], gno2, svalues2[i])!= RETURN_SUCCESS) {
                    error = TRUE;
                }
                break;
            case OPTYPE_COPY:
                if (ns2 == 0) {
                    setno2 = nextset(gno2);
                } else {
                    setno2 = svalues2[i];
                }
                snos[index++]=setno2;
                if (do_copyset(gno1, svalues1[i], gno2, setno2)!= RETURN_SUCCESS) {
                    error = TRUE;
                }
                break;
            case OPTYPE_MOVE:
                if (ns2 == 0) {
                    setno2 = nextset(gno2);
                } else {
                    setno2 = svalues2[i];
                }
                snos[index++]=setno2;
                if (do_moveset(gno1, svalues1[i], gno2, setno2)!= RETURN_SUCCESS) {
                    error = TRUE;
                }
                break;
            case OPTYPE_COPY_LEGEND:
                strcpy(g[gno2].p[svalues2[i]].lstr,g[gno1].p[svalues1[i]].lstr);
                strcpy(g[gno2].p[svalues2[i]].orig_lstr,g[gno1].p[svalues1[i]].orig_lstr);
                strcpy(g[gno2].p[svalues2[i]].comments,g[gno1].p[svalues1[i]].comments);
                strcpy(g[gno2].p[svalues2[i]].orig_comments,g[gno1].p[svalues1[i]].orig_comments);
                break;
            }
        }
        if (optype==OPTYPE_COPY || optype==OPTYPE_MOVE)
        {
            for (i=0;i<index;i++)
                gnos[i]=gno2;
            SetsModified(index,gnos,snos,UNDO_COMPLETE);
            if (optype==OPTYPE_COPY)
            {
                if (ns1==1)
                {
                    addAditionalDescriptionToLastNode(-1,QObject::tr("Set coppied")+QString(dummy),QString(),-1);
                }
                else
                {
                    addAditionalDescriptionToLastNode(-1,QObject::tr("Sets coppied"),QString(),-1);
                }
            }
            else
            {
                if (ns1==1)
                {
                    addAditionalDescriptionToLastNode(-1,QObject::tr("Set moved")+QString(dummy),QString(),-1);
                }
                else
                {
                    addAditionalDescriptionToLastNode(-1,QObject::tr("Sets moved"),QString(),-1);
                }
            }
        }
        else if (optype==OPTYPE_COPY_LEGEND)
        {
            SetsModified(ns2,gnos,svalues2,UNDO_APPEARANCE);
            addAditionalDescriptionToLastNode(-1,QObject::tr("Legend coppied")+QString(dummy),QString(),-1);
        }
        delete[] gnos;
        delete[] snos;
    }
    delete[] svalues1;
    delete[] svalues2;
    if (error == FALSE)
    {
        grpSource->update_number_of_entries();
        grpDestination->update_number_of_entries();
        mainWin->mainArea->completeRedraw();
        //        return RETURN_SUCCESS;
    } /*else {
        return RETURN_FAILURE;
    }*/
}

void frmSetOperations::doClose(void)
{
    hide();
}

void frmSetOperations::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}


void get_distance_between_graphs(int g1,int g2,int & direction,double & d)
{//returns the smallest distance (d) between two graphs and the direction (1=g2 right of g1, 2=g2 left of g1, 4=g2 above g1, 8=g2 below g1, combinations allowed)
view p1,p2;
double left,right,top,bottom;
char direction_name[16][32];
for (int i=0;i<16;i++) strcpy(direction_name[i],"undefined");
strcpy(direction_name[1],"right");
strcpy(direction_name[2],"left");

strcpy(direction_name[4],"above");
strcpy(direction_name[5],"above+right");
strcpy(direction_name[6],"above+left");

strcpy(direction_name[8],"below");
strcpy(direction_name[9],"below+right");
strcpy(direction_name[10],"below+left");

get_graph_viewport(g1,&p1);
get_graph_viewport(g2,&p2);
    left=-(p2.xv2-p1.xv1);
    right=p2.xv1-p1.xv2;
    top=p2.yv1-p1.yv2;
    bottom=-(p2.yv2-p1.yv1);

/*cout << "left=  " << left << endl;
cout << "right= " << right << endl;
cout << "top=   " << top << endl;
cout << "bottom=" << bottom << endl;*/

direction=(right>=0?1:0)|(left>=0?2:0)|(top>=0?4:0)|(bottom>=0?8:0);
    d=-1;
if ((left>=0.0 && left<=d) || d<0)
{
d=left;
}
if ((right>=0.0 && right<=d) || d<0)
{
d=right;
}
if ((top>=0.0 && top<=d) || d<0)
{
d=top;
}
if ((bottom>=0.0 && bottom<=d) || d<0)
{
d=bottom;
}
//cout << "direction=" << direction << " --> " << direction_name[direction] << " d=" << d << endl;
}

frmArrangeGraphs::frmArrangeGraphs(QWidget * parent):QDialog(parent)
{
//setFont(*stdFont);
    setWindowTitle(tr("QtGrace: Arrange Graphs"));
    setWindowIcon(QIcon(*GraceIcon));

    menuBar=new QMenuBar(this);
    mnuPackX=new QMenu(tr("&X-Ticks(packed)"),menuBar);
    mnuPackY=new QMenu(tr("&Y-Ticks(packed)"),menuBar);
    mnuGuess=new QMenu(tr("&Guess ordering"),menuBar);
    createActions();
    menuBar->addMenu(mnuPackX);
    menuBar->addMenu(mnuPackY);
    menuBar->addMenu(mnuGuess);

    grpArrGraphs=new QGroupBox(tr("Arrange graphs:"),this);
    grpMatrix=new QGroupBox(tr("Matrix"),this);
    grpPageOffset=new QGroupBox(tr("Page offset"),this);
    grpSpacing=new QGroupBox(tr("Spacing"),this);

    lblArrGraphs=new QLabel(tr("Arrange graphs:"),grpArrGraphs);
    lblArrGraphs->setVisible(false);

    graphlist=new uniList(GRAPHLIST,grpArrGraphs);
    graphlist->setSelectionMode(QAbstractItemView::ExtendedSelection);

    selCols=new stdIntSelector(grpMatrix,tr("Cols:"),1,99);
    selRows=new stdIntSelector(grpMatrix,tr("Rows:"),1,99);

    /*selUpperOffset=new QDoubleSpinBox(grpPageOffset);
    selUpperOffset->setRange(0.0,1.0);
    selUpperOffset->setSingleStep(0.05);
    selUpperOffset->setValue(0.15);
    selLowerOffset=new QDoubleSpinBox(grpPageOffset);
    selLowerOffset->setRange(0.0,1.0);
    selLowerOffset->setSingleStep(0.05);
    selLowerOffset->setValue(0.15);
    selLeftOffset=new QDoubleSpinBox(grpPageOffset);
    selLeftOffset->setRange(0.0,1.0);
    selLeftOffset->setSingleStep(0.05);
    selLeftOffset->setValue(0.15);
    selRightOffset=new QDoubleSpinBox(grpPageOffset);
    selRightOffset->setRange(0.0,1.0);
    selRightOffset->setSingleStep(0.05);
    selRightOffset->setValue(0.15);*/

    selUpperOffset=new LineWidthSelector(grpPageOffset);
    selUpperOffset->setText(tr("Top:"));
    selUpperOffset->setRange(0.0,1.0);
    selUpperOffset->setDecimals(2);
    selUpperOffset->setSingleStep(0.05);
    selUpperOffset->setValue(0.15);
    selLowerOffset=new LineWidthSelector(grpPageOffset);
    selLowerOffset->setText(tr("Bottom:"));
    selLowerOffset->setRange(0.0,1.0);
    selLowerOffset->setDecimals(2);
    selLowerOffset->setSingleStep(0.05);
    selLowerOffset->setValue(0.15);
    selLeftOffset=new LineWidthSelector(grpPageOffset);
    selLeftOffset->setText(tr("Left:"));
    selLeftOffset->setRange(0.0,1.0);
    selLeftOffset->setDecimals(2);
    selLeftOffset->setSingleStep(0.05);
    selLeftOffset->setValue(0.15);
    selRightOffset=new LineWidthSelector(grpPageOffset);
    selRightOffset->setText(tr("Right:"));
    selRightOffset->setRange(0.0,1.0);
    selRightOffset->setDecimals(2);
    selRightOffset->setSingleStep(0.05);
    selRightOffset->setValue(0.15);

    selHGap=new LineWidthSelector(grpSpacing);
    selHGap->lblText->setText(tr("Hgap/width:"));
    selHGap->spnLineWidth->setRange(0.0,9.0);
    selHGap->spnLineWidth->setSingleStep(0.1);
    selHGap->spnLineWidth->setValue(0.2);
    selHGap->spnLineWidth->setDecimals(2);
    selVGap=new LineWidthSelector(grpSpacing);
    selVGap->lblText->setText(tr("Vgap/height:"));
    selVGap->spnLineWidth->setRange(0.0,9.0);
    selVGap->spnLineWidth->setSingleStep(0.1);
    selVGap->spnLineWidth->setValue(0.2);
    selVGap->spnLineWidth->setDecimals(2);

    chkAddGraphs=new QCheckBox(tr("Add graphs as needed to fill the matrix"),grpArrGraphs);
    chkAddGraphs->setChecked(TRUE);
    chkKillGraphs=new QCheckBox(tr("Kill extra graphs"),grpArrGraphs);
    int number=3;
    QString list[3];
    list[0]=tr("No");
    list[1]=tr("Keep relative position(s)");
    list[2]=tr("New relative position(s)");
    selMoveLegends=new StdSelector(grpArrGraphs,tr("Move legends:"),number,list);
    connect(selMoveLegends,SIGNAL(currentIndexChanged(int)),SLOT(MoveLegendsChanged(int)));
    selLegendX=new LineWidthSelector(grpArrGraphs);
    selLegendX->setText(tr("Relative X of legend:"));
    selLegendX->setRange(-100.0,200.0);
    selLegendX->setDecimals(2);
    selLegendX->setValue(5.0);
    selLegendX->setVisible(false);
    selLegendX->spnLineWidth->setSuffix(QString(" %"));
    selLegendY=new LineWidthSelector(grpArrGraphs);
    selLegendY->setText(tr("Relative Y of legend:"));
    selLegendY->setRange(-100.0,200.0);
    selLegendY->setDecimals(2);
    selLegendY->setValue(90.0);
    selLegendY->setVisible(false);
    selLegendY->spnLineWidth->setSuffix(QString(" %"));
    packTicksXInner=packTicksYInner=0;
    packTicksXOuter=packTicksYOuter=0;

    chkSnakeFill=new QCheckBox(tr("Snake fill"),grpMatrix);
    chkHPack=new QCheckBox(tr("Pack"),grpSpacing);
    chkVPack=new QCheckBox(tr("Pack"),grpSpacing);
    connect(chkHPack,SIGNAL(toggled(bool)),SLOT(PackToggled(bool)));
    connect(chkVPack,SIGNAL(toggled(bool)),SLOT(PackToggled(bool)));

    selOrder=new OrderSelector(grpMatrix);

    buttonGroup=new stdButtonGroup(this);
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));

    number=0;
    layout0=new QGridLayout();
    //layout0->setMargin(STD_MARGIN);
    layout0->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout0->setSpacing(STD_SPACING);
    layout0->addWidget(lblArrGraphs,number++,0,1,2);
    layout0->addWidget(graphlist,number++,0,1,2);
    layout0->addWidget(chkAddGraphs,number++,0,1,2);
    layout0->addWidget(chkKillGraphs,number++,0,1,2);
    layout0->addWidget(selMoveLegends,number++,0,1,2);
    layout0->addWidget(selLegendX,number,0,1,1);
    layout0->addWidget(selLegendY,number++,1,1,1);
    grpArrGraphs->setLayout(layout0);

    layout1=new QHBoxLayout();
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout1->setSpacing(STD_SPACING);
    layout1->addWidget(selCols);
    layout1->addWidget(selRows);
    layout1->addWidget(selOrder);
    layout1->addWidget(chkSnakeFill);
    grpMatrix->setLayout(layout1);

    layout2=new QGridLayout();
    //layout2->setMargin(STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout2->setSpacing(STD_SPACING);
    layout2->addWidget(selUpperOffset,0,2);
    layout2->addWidget(selLowerOffset,2,2);
    layout2->addWidget(selLeftOffset,1,0);
    layout2->addWidget(selRightOffset,1,4);
    grpPageOffset->setLayout(layout2);

    layout3=new QHBoxLayout();
    //layout3->setMargin(STD_MARGIN);
    layout3->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout3->setSpacing(STD_SPACING);
    layout3->addWidget(selHGap);
    layout3->addWidget(chkHPack);
    layout3->addWidget(selVGap);
    layout3->addWidget(chkVPack);
    grpSpacing->setLayout(layout3);

    layout=new QVBoxLayout();
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->setSpacing(STD_SPACING);
#ifndef MAC_SYSTEM
    layout->addWidget(menuBar);
#endif
    layout->addWidget(grpArrGraphs);
    layout->addWidget(grpMatrix);
    layout->addWidget(grpPageOffset);
    layout->addWidget(grpSpacing);
    layout->addWidget(buttonGroup);
    setLayout(layout);
    resize(LastSize_FormArrangeGraphs);
}

frmArrangeGraphs::~frmArrangeGraphs()
{
    LastSize_FormArrangeGraphs=this->size();
}

void frmArrangeGraphs::createActions(void)
{
    grpPackXInner=new QActionGroup(mnuPackX);
    grpPackXInner->setExclusive(true);
    grpPackXInnerDirection=new QActionGroup(mnuPackX);
    grpPackXInnerDirection->setExclusive(true);
    grpPackXOuter=new QActionGroup(mnuPackX);
    grpPackXOuter->setExclusive(true);
    grpPackXOuterDirection=new QActionGroup(mnuPackX);
    grpPackXOuterDirection->setExclusive(true);

    grpPackYInner=new QActionGroup(mnuPackY);
    grpPackYInner->setExclusive(true);
    grpPackYInnerDirection=new QActionGroup(mnuPackY);
    grpPackYInnerDirection->setExclusive(true);
    grpPackYOuter=new QActionGroup(mnuPackY);
    grpPackYOuter->setExclusive(true);
    grpPackYOuterDirection=new QActionGroup(mnuPackY);
    grpPackYOuterDirection->setExclusive(true);

    QActionGroup * actGr=NULL;
    QString text1;
    for (int i=0;i<2;i++)
    {
        if (i==0)
        {
            actGr=grpPackXInner;
            text1=tr("Inner graph(s): ");
        }
        else
        {
            actGr=grpPackXOuter;
            text1=tr("Outer graph(s): ");
        }
    actPackXInside[i]=new QAction(text1+tr("only inside"),actGr);
    actPackXOutside[i]=new QAction(text1+tr("only outside"),actGr);
    actPackXBoth[i]=new QAction(text1+tr("both sides"),actGr);
    actPackXBottom[i]=new QAction(text1+tr("bottom side"),actGr);
    actPackXTop[i]=new QAction(text1+tr("top side"),actGr);
    actPackXUnchanged[i]=new QAction(text1+tr("unchanged"),actGr);
    actPackXNone[i]=new QAction(text1+tr("none"),actGr);
    if (i==0) actGr=grpPackYInner;
    else actGr=grpPackYOuter;
    actPackYInside[i]=new QAction(text1+tr("only inside"),actGr);
    actPackYOutside[i]=new QAction(text1+tr("only outside"),actGr);
    actPackYBoth[i]=new QAction(text1+tr("both sides"),actGr);
    actPackYLeft[i]=new QAction(text1+tr("left side"),actGr);
    actPackYRight[i]=new QAction(text1+tr("right side"),actGr);
    actPackYUnchanged[i]=new QAction(text1+tr("unchanged"),actGr);
    actPackYNone[i]=new QAction(text1+tr("none"),actGr);
    if (i==0) actGr=grpPackXInnerDirection;
    else actGr=grpPackXOuterDirection;
    actPackXDirectionIn[i]=new QAction(text1+tr("ticks inward"),actGr);
    actPackXDirectionOut[i]=new QAction(text1+tr("ticks outward"),actGr);
    actPackXDirectionBoth[i]=new QAction(text1+tr("ticks in- and outward"),actGr);
    actPackXDirectionUnchanged[i]=new QAction(text1+tr("unchanged"),actGr);
    if (i==0) actGr=grpPackYInnerDirection;
    else actGr=grpPackYOuterDirection;
    actPackYDirectionIn[i]=new QAction(text1+tr("ticks inward"),actGr);
    actPackYDirectionOut[i]=new QAction(text1+tr("ticks outward"),actGr);
    actPackYDirectionBoth[i]=new QAction(text1+tr("ticks in- and outward"),actGr);
    actPackYDirectionUnchanged[i]=new QAction(text1+tr("unchanged"),actGr);

    if (i==0)
    {
    actPackXInside[i]->setVisible(false);
    actPackYInside[i]->setVisible(false);
    actPackXOutside[i]->setVisible(false);
    actPackYOutside[i]->setVisible(false);
    }

    actPackXInside[i]->setCheckable(true);
    actPackXOutside[i]->setCheckable(true);
    actPackXBottom[i]->setCheckable(true);
    actPackXTop[i]->setCheckable(true);
    actPackXBoth[i]->setCheckable(true);
    actPackXUnchanged[i]->setCheckable(true);
    actPackXNone[i]->setCheckable(true);
    actPackYInside[i]->setCheckable(true);
    actPackYOutside[i]->setCheckable(true);
    actPackYLeft[i]->setCheckable(true);
    actPackYRight[i]->setCheckable(true);
    actPackYBoth[i]->setCheckable(true);
    actPackYUnchanged[i]->setCheckable(true);
    actPackYNone[i]->setCheckable(true);

    actPackXDirectionIn[i]->setCheckable(true);
    actPackXDirectionOut[i]->setCheckable(true);
    actPackXDirectionBoth[i]->setCheckable(true);
    actPackXDirectionUnchanged[i]->setCheckable(true);
    actPackYDirectionIn[i]->setCheckable(true);
    actPackYDirectionOut[i]->setCheckable(true);
    actPackYDirectionBoth[i]->setCheckable(true);
    actPackYDirectionUnchanged[i]->setCheckable(true);

    mnuPackX->addAction(actPackXInside[i]);
    mnuPackX->addAction(actPackXOutside[i]);
    mnuPackX->addAction(actPackXBottom[i]);
    mnuPackX->addAction(actPackXTop[i]);
    mnuPackX->addAction(actPackXBoth[i]);
    actPackXBoth[i]->setChecked(true);
    mnuPackX->addAction(actPackXUnchanged[i]);
    mnuPackX->addAction(actPackXNone[i]);
    mnuPackX->addSeparator();
    mnuPackX->addAction(actPackXDirectionIn[i]);
    actPackXDirectionIn[i]->setChecked(true);
    mnuPackX->addAction(actPackXDirectionOut[i]);
    mnuPackX->addAction(actPackXDirectionBoth[i]);
    mnuPackX->addAction(actPackXDirectionUnchanged[i]);
    if (i==0)
    mnuPackX->addSeparator();

    mnuPackY->addAction(actPackYInside[i]);
    mnuPackY->addAction(actPackYOutside[i]);
    mnuPackY->addAction(actPackYLeft[i]);
    mnuPackY->addAction(actPackYRight[i]);
    mnuPackY->addAction(actPackYBoth[i]);
    actPackYBoth[i]->setChecked(true);
    mnuPackY->addAction(actPackYUnchanged[i]);
    mnuPackY->addAction(actPackYNone[i]);
    mnuPackY->addSeparator();
    mnuPackY->addAction(actPackYDirectionIn[i]);
    actPackYDirectionIn[i]->setChecked(true);
    mnuPackY->addAction(actPackYDirectionOut[i]);
    mnuPackY->addAction(actPackYDirectionBoth[i]);
    mnuPackY->addAction(actPackYDirectionUnchanged[i]);
    if (i==0)
    mnuPackY->addSeparator();

    }
    mnuPackY->setEnabled(false);
    mnuPackX->setEnabled(false);

    actGuessAll=new QAction(tr("Guess ordering of all graphs"),mnuGuess);
    actGuessPart=new QAction(tr("Guess ordering of selected graphs"),mnuGuess);
    mnuGuess->addAction(actGuessPart);
    mnuGuess->addAction(actGuessAll);
    connect(actGuessPart,SIGNAL(triggered()),this,SLOT(guessSelectedGraphOrdering()));
    connect(actGuessAll,SIGNAL(triggered()),this,SLOT(guessAllGraphOrdering()));
}

void frmArrangeGraphs::readTickSettings(void)
{
packTicksXInner=packTicksYInner=TICK_PACK_UNCHANGED;
packTicksXOuter=packTicksYOuter=TICK_PACK_UNCHANGED;
packTicksXInnerDir=packTicksYInnerDir=TICK_PACK_DIRECTION_UNCHANGED;
packTicksXOuterDir=packTicksYOuterDir=TICK_PACK_DIRECTION_UNCHANGED;
//TICK_PACK_UNCHANGED
//TICK_PACK_LEFT
//TICK_PACK_RIGHT
//TICK_PACK_UP
//TICK_PACK_DOWN
//TICK_PACK_IN
//TICK_PACK_OUT

//TICK_PACK_DIRECTION_UNCHANGED
//TICK_PACK_DIRECTION_IN
//TICK_PACK_DIRECTION_OUT

//i=0=inside
//i=1=outside
int target_X,target_Y,target_dir_X,target_dir_Y;
for (int i=0;i<2;i++)
{
    target_X=target_Y=0;
    target_dir_X=target_dir_Y=0;
if (actPackXInside[i]->isChecked()==true) target_X |= TICK_PACK_IN;
if (actPackXOutside[i]->isChecked()==true) target_X |= TICK_PACK_OUT;
if (actPackXBottom[i]->isChecked()==true) target_X |= TICK_PACK_DOWN;
if (actPackXTop[i]->isChecked()==true) target_X |= TICK_PACK_UP;
if (actPackXBoth[i]->isChecked()==true) target_X |= (TICK_PACK_IN | TICK_PACK_OUT);
if (actPackXUnchanged[i]->isChecked()==true) target_X = TICK_PACK_UNCHANGED;
if (actPackXNone[i]->isChecked()==true) target_X = TICK_PACK_NONE;

if (actPackYInside[i]->isChecked()==true) target_Y |= TICK_PACK_IN;
if (actPackYOutside[i]->isChecked()==true) target_Y |= TICK_PACK_OUT;
if (actPackYLeft[i]->isChecked()==true) target_Y |= TICK_PACK_LEFT;
if (actPackYRight[i]->isChecked()==true) target_Y |= TICK_PACK_RIGHT;
if (actPackYBoth[i]->isChecked()==true) target_Y |= (TICK_PACK_IN | TICK_PACK_OUT);
if (actPackYUnchanged[i]->isChecked()==true) target_Y = TICK_PACK_UNCHANGED;
if (actPackYNone[i]->isChecked()==true) target_Y = TICK_PACK_NONE;

if (actPackXDirectionIn[i]->isChecked()==true) target_dir_X |= TICK_PACK_DIRECTION_IN;
if (actPackXDirectionOut[i]->isChecked()==true) target_dir_X |= TICK_PACK_DIRECTION_OUT;
if (actPackXDirectionBoth[i]->isChecked()==true) target_dir_X |= (TICK_PACK_DIRECTION_IN | TICK_PACK_DIRECTION_OUT);
if (actPackXDirectionUnchanged[i]->isChecked()==true) target_dir_X = TICK_PACK_DIRECTION_UNCHANGED;

if (actPackYDirectionIn[i]->isChecked()==true) target_dir_Y |= TICK_PACK_DIRECTION_IN;
if (actPackYDirectionOut[i]->isChecked()==true) target_dir_Y |= TICK_PACK_DIRECTION_OUT;
if (actPackYDirectionBoth[i]->isChecked()==true) target_dir_Y |= (TICK_PACK_DIRECTION_IN | TICK_PACK_DIRECTION_OUT);
if (actPackYDirectionUnchanged[i]->isChecked()==true) target_dir_Y = TICK_PACK_DIRECTION_UNCHANGED;

    if (i==0)//inside
    {
    packTicksXInner=target_X;
    packTicksYInner=target_Y;
    packTicksXInnerDir=target_dir_X;
    packTicksYInnerDir=target_dir_Y;
    }
    else//outside
    {
    packTicksXOuter=target_X;
    packTicksYOuter=target_Y;
    packTicksXOuterDir=target_dir_X;
    packTicksYOuterDir=target_dir_Y;
    }
}

}

void frmArrangeGraphs::PackToggled(bool t)
{
    (void)t;
    selHGap->setEnabled(!(chkHPack->isChecked()));
    selVGap->setEnabled(!(chkVPack->isChecked()));
    mnuPackX->setEnabled((chkVPack->isChecked()));
    mnuPackY->setEnabled((chkHPack->isChecked()));
}

void frmArrangeGraphs::init(void)
{
QLocale newLocale=(DecimalPointToUse=='.')?(*dot_locale):(*comma_locale);
selUpperOffset->setLocale(newLocale);
selLowerOffset->setLocale(newLocale);
selLeftOffset->setLocale(newLocale);
selRightOffset->setLocale(newLocale);
selHGap->spnLineWidth->setLocale(newLocale);
selVGap->spnLineWidth->setLocale(newLocale);
int sel_number,*sel=new int[2];
    graphlist->update_number_of_entries();
    graphlist->get_all_entries(&sel_number,&sel);
        //sel[0]=get_cg();
        //graphlist->set_new_selection(1,sel);
    graphlist->set_new_selection(sel_number,sel);//select all
guessGraphOrdering(sel_number,sel);//guess ordering of all!
    delete[] sel;
updateAfterGuessing();
}

void frmArrangeGraphs::doApply(void)
{
    ApplyError=false;
    int ngraphs, * graphs=new int[2];
    int nrows, ncols, order, snake;
    int hpack, vpack, add, kill, move_legend;
    double toff, loff, roff, boff, vgap, hgap, legendXPos, legendYPos;

    readTickSettings();

    nrows = (int) GetSpinChoice(selRows);
    ncols = (int) GetSpinChoice(selCols);

    if (nrows < 1 || ncols < 1)
    {
        errmsg(tr("# of rows and columns must be > 0").toLocal8Bit().constData());
        delete[] graphs;
        ApplyError=true;
        return;// RETURN_FAILURE;
    }

    graphlist->get_selection(&ngraphs,&graphs);

    if (ngraphs == 0) {
        graphs = NULL;
    }

    order = selOrder->currentIndex();
    snake = GetToggleButtonState(chkSnakeFill);

    toff = GetSpinChoice(selUpperOffset);
    loff = GetSpinChoice(selLeftOffset);
    roff = GetSpinChoice(selRightOffset);
    boff = GetSpinChoice(selLowerOffset);

    hgap = GetSpinChoice(selHGap);
    vgap = GetSpinChoice(selVGap);

    add  = GetToggleButtonState(chkAddGraphs);
    kill = GetToggleButtonState(chkKillGraphs);
    move_legend = selMoveLegends->currentIndex();
    legendXPos = selLegendX->value()*0.01;
    legendYPos = selLegendY->value()*0.01;

    hpack = GetToggleButtonState(chkHPack);
    vpack = GetToggleButtonState(chkVPack);

    //Undo-Stuff
    SaveGraphStatesPrevious(ngraphs,graphs,UNDO_APPEARANCE);//save the sates of the graphs affected

    if (add && ngraphs < nrows*ncols)
    {
        int gno;
        int * gr_sav=new int[ngraphs+5];
        memcpy(gr_sav,graphs,sizeof(int)*ngraphs);
        delete[] graphs;
        graphs=new int[nrows*ncols];
        memcpy(graphs,gr_sav,sizeof(int)*ngraphs);
        delete[] gr_sav;
        //graphs = xrealloc(graphs, nrows*ncols*sizeof(int));
        for (gno = number_of_graphs(); ngraphs < nrows*ncols; ngraphs++, gno++)
        {
            graphs[ngraphs] = gno;
        }
    }

    if (kill && ngraphs > nrows*ncols) {
        for (; ngraphs > nrows*ncols; ngraphs--) {
            kill_graph(graphs[ngraphs - 1]);
        }
    }

    arrange_graphs(graphs, ngraphs,
                   nrows, ncols, order, snake,
                   loff, roff, toff, boff, vgap, hgap,
                   hpack, vpack, move_legend, legendXPos, legendYPos,
                   packTicksXInner,packTicksXOuter,packTicksYInner,packTicksYOuter,
                   packTicksXInnerDir,packTicksXOuterDir,packTicksYInnerDir,packTicksYOuterDir);

    //Undo-Staff
    GraphsModified(ngraphs,graphs,UNDO_APPEARANCE);
    addAditionalDescriptionToLastNode(UNDO_TYPE_GRAPH_APPEARANCE,QString(),tr("rearranged"));

    update_all();

    ///SelectListChoices(ui->graphs, ngraphs, graphs);
    //xfree(graphs);
    graphlist->update_number_of_entries();

    mainWin->mainArea->completeRedraw();
    graphlist->set_new_selection(ngraphs,graphs);
    delete[] graphs;
}

void frmArrangeGraphs::doClose(void)
{
    hide();
}

void frmArrangeGraphs::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmArrangeGraphs::guessSelectedGraphOrdering(void)
{
int nr,*graphs=new int[2];
graphlist->get_selection(&nr,&graphs);
    if (nr>0)
    {
    guessGraphOrdering(nr,graphs);
    updateAfterGuessing();
    }
    else
    {
    errmsg(tr("No graphs selected.").toLocal8Bit().constData());
    }
}

void frmArrangeGraphs::guessAllGraphOrdering(void)
{
int nr,*graphs=new int[2];
graphlist->get_all_entries(&nr,&graphs);
    if (nr>0)
    {
    guessGraphOrdering(nr,graphs);
    updateAfterGuessing();
    }
    else
    {
    errmsg(tr("No graphs.").toLocal8Bit().constData());
    }
}

void frmArrangeGraphs::guessGraphOrdering(int nr, int *graphs)
{
double xmin,xmax,ymin,ymax;
double width,height;
int first=-1,i;
tickmarks * t;

guess_r=guess_c=n_graphs=0;
guess_hgap=guess_vgap=0.0;
guess_success=0;
xmax=xmin=ymax=ymin=0.0;
//first part is getting the outer margins
//qDebug() << "guess Graph Ordering: nr of graphs=" << nr;
for (int k=0;k<nr;k++)
{
    i=graphs[k];
//qDebug() << i;
        if (g[i].hidden==true) continue;//only use visible graphs for checking the borders
    if (first==-1)
    {
    xmin=g[i].v.xv1;
    xmax=g[i].v.xv2;
    ymin=g[i].v.yv1;
    ymax=g[i].v.yv2;
    first=i;
    n_graphs=1;
    }
    else
    {
        if (xmin>g[i].v.xv1)
        xmin=g[i].v.xv1;
        if (xmax<g[i].v.xv2)
        xmax=g[i].v.xv2;
        if (ymin>g[i].v.yv1)
        ymin=g[i].v.yv1;
        if (ymax<g[i].v.yv2)
        ymax=g[i].v.yv2;
    n_graphs++;
    }
}
//n_graphs only inlcudes visible graphs
//qDebug() << "Guessed values: number_of_graphs=" << n_graphs;
//qDebug() << "xmin=" << xmin << " xmax=" << xmax;
//qDebug() << "ymin=" << ymin << " ymax=" << ymax;
get_page_viewport(&width,&height);
//qDebug() << "Page:" << width << " x " << height;
left_offset=xmin;
right_offset=width-xmax;
bottom_offset=ymin;
top_offset=height-ymax;
//round to 2 decimal places
left_offset=0.01*rint_v2(100.0*left_offset);
right_offset=0.01*rint_v2(100.0*right_offset);
bottom_offset=0.01*rint_v2(100.0*bottom_offset);
top_offset=0.01*rint_v2(100.0*top_offset);
//here we have the outer margins
//qDebug() << "Offsets:";
//qDebug() << "Left=" << left_offset << " Right=" << right_offset;
//qDebug() << "Bottom=" << bottom_offset << " Top=" << top_offset;
int direction,initial_direction,dir_first_after,dir_before_after;
double d,initial_d,tmp_d1,tmp_d2;
int index;
guess_vgap=0.2;
guess_hgap=0.2;
guess_hpack=0;
guess_vpack=0;
//now we guess the ordering
if (nr>1)
{
get_distance_between_graphs(graphs[0],graphs[1],initial_direction,initial_d);
index=1;
direction=initial_direction;
while (index<nr-1 && direction==initial_direction)
{
get_distance_between_graphs(graphs[index],graphs[index+1],direction,d);
index++;
}
//qDebug() << "index=" << index << "nr=" << nr;
//qDebug() << "initial_d=" << initial_d << "initial_dir=" << initial_direction;
//index is now the id of the first graph after switching directions
if (index>=nr-1)//just one row or column
{
    if (initial_direction==1 || initial_direction==2)//g1 right of g0 or left
    {
    guess_r=1;
    guess_c=nr;
    guess_snake=0;
    guess_order=0;
        if (initial_direction==2) guess_order=1;
    guess_success=1;
        if (g[graphs[0]].v.xv2-g[graphs[0]].v.xv1==0.0)
        guess_hgap=0.2;
        else
        guess_hgap=initial_d/(g[graphs[0]].v.xv2-g[graphs[0]].v.xv1);
    }
    else if (initial_direction==4 || initial_direction==8)//g1 is above g0 or below
    {
    guess_r=nr;
    guess_c=1;
    guess_snake=0;
    guess_order=4;
        if (initial_direction==4) guess_order=5;
    guess_success=1;
        if (g[graphs[0]].v.yv2-g[graphs[0]].v.yv1==0.0)
        guess_vgap=0.2;
        else
        guess_vgap=initial_d/(g[graphs[0]].v.yv2-g[graphs[0]].v.yv1);
    }
    else
    {
    guess_success=0;//no auto-ordering
    }
    if (fabs(guess_vgap)<0.01){guess_vgap=0.2;guess_vpack=1;}
    else guess_vpack=0;
    if (fabs(guess_hgap)<0.01){guess_hgap=0.2;guess_hpack=1;}
    else guess_hpack=0;
//cout << "Guessing: just one row/column: vgap=" << guess_vgap << " hgap=" << guess_hgap << endl;
}
else//more than one row and column
{
    if (initial_direction==1 || initial_direction==2)
    {
    guess_c=index;
    guess_r=nr/index;
    }
    else if (initial_direction==4 || initial_direction==8)
    {
    guess_r=index;
    guess_c=nr/index;
    }
get_distance_between_graphs(graphs[0],graphs[index],dir_first_after,tmp_d1);
get_distance_between_graphs(graphs[index-1],graphs[index],dir_before_after,tmp_d2);
    switch (initial_direction)
    {
    case 1:
        switch (dir_first_after)
        {
        case 4:
        guess_order=1;
        guess_snake=0;
        guess_success=1;
        break;
        case 5:
        guess_order=1;
        guess_snake=1;
        guess_success=1;
        break;
        case 8:
        guess_order=0;
        guess_snake=0;
        guess_success=1;
        break;
        case 9:
        guess_order=0;
        guess_snake=1;
        guess_success=1;
        break;
        default:
        guess_success=0;
        break;
        }
    break;
    case 2:
        switch (dir_first_after)
        {
        case 4:
        guess_order=3;
        guess_snake=0;
        guess_success=1;
        break;
        case 6:
        guess_order=3;
        guess_snake=1;
        guess_success=1;
        break;
        case 8:
        guess_order=2;
        guess_snake=0;
        guess_success=1;
        break;
        case 10:
        guess_order=2;
        guess_snake=1;
        guess_success=1;
        break;
        default:
        guess_success=0;
        break;
        }
    break;
    case 4:
        switch (dir_first_after)
        {
        case 1:
        guess_order=5;
        guess_snake=0;
        guess_success=1;
        break;
        case 5:
        guess_order=5;
        guess_snake=1;
        guess_success=1;
        break;
        case 2:
        guess_order=7;
        guess_snake=0;
        guess_success=1;
        break;
        case 6:
        guess_order=7;
        guess_snake=1;
        guess_success=1;
        break;
        default:
        guess_success=0;
        break;
        }
    break;
    case 8:
        switch (dir_first_after)
        {
        case 1:
        guess_order=4;
        guess_snake=0;
        guess_success=1;
        break;
        case 9:
        guess_order=4;
        guess_snake=1;
        guess_success=1;
        break;
        case 2:
        guess_order=6;
        guess_snake=0;
        guess_success=1;
        break;
        case 10:
        guess_order=6;
        guess_snake=1;
        guess_success=1;
        break;
        default:
        guess_success=0;
        break;
        }
    break;
    default:
    guess_success=0;
    break;
    }
//qDebug() << "guess_success=" << guess_success;
    ///guess the gaps now
    if (guess_success==1)
    {
        switch (guess_order)
        {
        case 0:
        case 1:
        case 2:
        case 3:
            if (g[graphs[0]].v.xv2-g[graphs[0]].v.xv1==0.0)
            guess_hgap=0.2;
            else
            guess_hgap=initial_d/(g[graphs[0]].v.xv2-g[graphs[0]].v.xv1);
                if (guess_snake==1)
                guess_vgap=tmp_d2;
                else
                guess_vgap=tmp_d1;
            if (g[graphs[0]].v.yv2-g[graphs[0]].v.yv1==0.0)
            guess_vgap=0.2;
            else
            guess_vgap/=(g[graphs[0]].v.yv2-g[graphs[0]].v.yv1);
        break;
        case 4:
        case 5:
        case 6:
        case 7:
            if (g[graphs[0]].v.yv2-g[graphs[0]].v.yv1==0.0)
            guess_vgap=0.2;
            else
            guess_vgap=initial_d/(g[graphs[0]].v.yv2-g[graphs[0]].v.yv1);
                if (guess_snake==1)
                guess_hgap=tmp_d2;
                else
                guess_hgap=tmp_d1;
            if (g[graphs[0]].v.xv2-g[graphs[0]].v.xv1==0.0)
            guess_hgap=0.2;
            else
            guess_hgap/=(g[graphs[0]].v.xv2-g[graphs[0]].v.xv1);
        break;
        }
        guess_hgap=0.01*rint_v2(100.0*guess_hgap);
        guess_vgap=0.01*rint_v2(100.0*guess_vgap);
//qDebug() << "guess_hgap=" << guess_hgap << "guess_vgap=" << guess_vgap;
        //looking for packed graphs
        if (guess_hgap==0.0)//may be h-packed
        {
            guess_hpack=0;
            switch (guess_order)
            {
            case 0:
            case 1:
            default:
                t = get_graph_tickmarks(graphs[1], 1);//graph 1, y-axis
            break;
            case 2:
            case 3:
            case 6:
            case 7:
                t = get_graph_tickmarks(graphs[0], 1);//graph 0, y-axis
            break;
            case 4:
            case 5:
                t = get_graph_tickmarks(graphs[index], 1);//graph 'index', y-axis
            break;
            }
            if (t!=NULL)
            guess_hpack=!t->tl_flag;
        }
        else guess_hpack=0;
        if (guess_vgap==0.0)//may be v-packed
        {
            guess_vpack=0;
            switch (guess_order)
            {
            case 0:
            case 2:
            case 4:
            case 6:
            default:
                t = get_graph_tickmarks(graphs[0], 0);//graph 0, x-axis
            break;
            case 5:
            case 7:
                t = get_graph_tickmarks(graphs[1], 0);//graph 1, x-axis
            break;
            case 1:
            case 3:
                t = get_graph_tickmarks(graphs[index], 0);//graph 'index', x-axis
            break;
            }
            if (t!=NULL)
            guess_vpack=!t->tl_flag;
        }
        else guess_vpack=0;
            //simpler case: (the more complicated guessing-method)
            if (fabs(guess_vgap)<0.001) guess_vpack=1;
            if (fabs(guess_hgap)<0.001) guess_hpack=1;
        if (guess_vpack==1) guess_vgap=0.2;
        if (guess_hpack==1) guess_hgap=0.2;
    }
}//end more than one row or column
}//end more than one graph
else//just one graph
{
    guess_r=1;
    guess_c=1;
    guess_order=0;
    guess_snake=0;
    guess_hpack=guess_vpack=0;
    guess_success=1;
}
guess_hgap=0.01*rint_v2(guess_hgap*100.0);
guess_vgap=0.01*rint_v2(guess_vgap*100.0);
//cout << "hgap=" << guess_hgap << " vgap=" << guess_vgap << endl;
}

void frmArrangeGraphs::MoveLegendsChanged(int l)
{
    if (l==2)
    {
    selLegendX->setVisible(true);
    selLegendY->setVisible(true);
    }
    else
    {
    selLegendX->setVisible(false);
    selLegendY->setVisible(false);
    }
}

void frmArrangeGraphs::updateAfterGuessing(void)
{
    if (guess_success==1)
    {
        selUpperOffset->setValue(top_offset);
        selLowerOffset->setValue(bottom_offset);
        selLeftOffset->setValue(left_offset);
        selRightOffset->setValue(right_offset);
        selHGap->setValue(guess_hgap);
        selVGap->setValue(guess_vgap);
        chkHPack->setChecked(guess_hpack==1?true:false);
        chkVPack->setChecked(guess_vpack==1?true:false);
        selCols->setValue(guess_c);
        selRows->setValue(guess_r);
        chkSnakeFill->setChecked(guess_snake==1?true:false);
        selOrder->setCurrentIndex(guess_order);
        /*int * sel=new int[number_of_graphs()+2];
        for (int i=0;i<number_of_graphs();i++) sel[i]=i;
        graphlist->set_new_selection(number_of_graphs(),sel);
        delete[] sel;*/
    }
}

frmOverlayGraphs::frmOverlayGraphs(QWidget * parent):QDialog(parent)
{
    int number=5;
    QString entr[5];
 //setFont(*stdFont);
    setWindowTitle(tr("QtGrace: Overlay Graphs"));
    setWindowIcon(QIcon(*GraceIcon));

    lblOverlayGraph=new QLabel(tr("Overlay graph:"),this);
    listOverlayGraph=new uniList(GRAPHLIST,this);
    listOverlayGraph->setBehavior(true,false,false);
    lblOntoGraph=new QLabel(tr("Onto graph:"),this);
    listOntoGraph=new uniList(GRAPHLIST,this);
    listOntoGraph->setBehavior(true,false,false);

    entr[0]=tr("Disabled");
    entr[1]=tr("X and Y axes different");
    entr[2]=tr("Same X axis scaling");
    entr[3]=tr("Same Y axis scaling");
    entr[4]=tr("Same X and Y axis scaling");
    selSmartAxisHint=new StdSelector(this,tr("Smart axis hint:"),number,entr);

    buttonGroup=new stdButtonGroup(this);
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));

    layout=new QVBoxLayout();
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(lblOverlayGraph);
    layout->addWidget(listOverlayGraph);
    layout->addWidget(lblOntoGraph);
    layout->addWidget(listOntoGraph);
    layout->addWidget(selSmartAxisHint);
    layout->addWidget(buttonGroup);
    setLayout(layout);
    resize(LastSize_FormOverlayGraphs);
}

frmOverlayGraphs::~frmOverlayGraphs()
{
    LastSize_FormOverlayGraphs=this->size();
}

void frmOverlayGraphs::init(void)
{
    listOverlayGraph->update_number_of_entries();
    listOntoGraph->update_number_of_entries();
    int nr=1,*sel=new int[2];
    sel[0]=get_cg();
    listOverlayGraph->set_new_selection(nr,sel);
    listOntoGraph->set_new_selection(nr,sel);
    delete[] sel;
}

void frmOverlayGraphs::doApply(void)
{
    ApplyError=false;
    int g1, g2;
    int *gr1=new int[2];
    int *gr2=new int[2];
    int type = selSmartAxisHint->currentIndex();//GetOptionChoice(graph_overlaytype_item);

    listOverlayGraph->get_selection(&g1,&gr1);
    listOntoGraph->get_selection(&g2,&gr2);

    if (g1!=1)//GetSingleListChoice(graph_overlay1_choice_item, &g1) != RETURN_SUCCESS)
    {
        errmsg(tr("Please select a single graph").toLocal8Bit().constData());
        delete[] gr1;
        delete[] gr2;
        ApplyError=true;
        return;// RETURN_FAILURE;
    }
    else
    {
        g1=gr1[0];
    }

    if (g2!=1)//GetSingleListChoice(graph_overlay2_choice_item, &g2) != RETURN_SUCCESS)
    {
        errmsg(tr("Please select a single graph").toLocal8Bit().constData());
        delete[] gr1;
        delete[] gr2;
        ApplyError=true;
        return;// RETURN_FAILURE;
    }
    else
    {
        g2=gr2[0];
    }

    if (g1 == g2)
    {
        errmsg(tr("Can't overlay a graph onto itself").toLocal8Bit().constData());
        delete[] gr1;
        delete[] gr2;
        ApplyError=true;
        return;// RETURN_FAILURE;
    }

    int * gnos=new int[2];
    gnos[0]=g1;
    gnos[1]=g2;

    SaveGraphStatesPrevious(2,gnos,UNDO_APPEARANCE|UNDO_AXIS);
    overlay_graphs(g1, g2, type);
    GraphsModified(2,gnos,UNDO_APPEARANCE|UNDO_AXIS);
    addAditionalDescriptionToLastNode(-1,QObject::tr("Overlay Graphs"),QString(),-1);

    delete[] gnos;
    mainWin->mainArea->completeRedraw();
    delete[] gr1;
    delete[] gr2;
}

void frmOverlayGraphs::doClose(void)
{
    hide();
}

void frmOverlayGraphs::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

frmAutoscaleGraphs::frmAutoscaleGraphs(QWidget * parent):QDialog(parent)
{
    int number=4;
    QString entr[4];
//setFont(*stdFont);
    setWindowTitle(tr("QtGrace: Autoscale Graphs"));
    setWindowIcon(QIcon(*GraceIcon));

    entr[0]=tr("None");
    entr[1]=tr("X");
    entr[2]=tr("Y");
    entr[3]=tr("XY");
    selAutoscale=new StdSelector(this,tr("Autoscale:"),number,entr);
    number=2;
    entr[0]=tr("Current");
    entr[1]=tr("All");
    selApplyToGraph=new StdSelector(this,tr("Apply to graph:"),number,entr);
    lblUseSet=new QLabel(tr("Use set:"),this);
    listSets=new uniList(SETCHOICE,this);
    listSets->setBehavior(true,false,true);

    buttonGroup=new stdButtonGroup(this);
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));

    layout=new QVBoxLayout();
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(selAutoscale);
    layout->addWidget(lblUseSet);
    layout->addWidget(listSets);
    layout->addWidget(selApplyToGraph);
    layout->addWidget(buttonGroup);
    setLayout(layout);
    resize(LastSize_FormAutoScaleGraphs);
}

frmAutoscaleGraphs::~frmAutoscaleGraphs()
{
    LastSize_FormAutoScaleGraphs=this->size();
}

void frmAutoscaleGraphs::init(void)
{
    selAutoscale->setCurrentIndex(3);
    listSets->gr_no=get_cg();
    listSets->update_number_of_entries();
}

void frmAutoscaleGraphs::doApply(void)
{
    ApplyError=false;
    int aon, au, ap;
    int * sel=new int[2];
    aon = selAutoscale->currentIndex();
    ap = GetChoice(selApplyToGraph);
    //au = GetSelectedSet(ui->sel);
    listSets->get_selection(&au,&sel);
    if (au<1)
    {
        errmsg(tr("No set selected").toLocal8Bit().constData());
        delete[] sel;
        ApplyError=true;
        return;//RETURN_FAILURE;
    }
    au=sel[0]-1;
    int * gnos=new int[number_of_graphs()+1];
    int index=0;
    if (ap==0)//current graph
    {
        gnos[0]=ap;
        index=1;
    }
    else//all graphs
    {
        index=number_of_graphs();
        for (int i=0;i<index;i++)
            gnos[i]=i;
    }
    SaveGraphStatesPrevious(index,gnos,UNDO_APPEARANCE);
    define_autos(aon, au, ap);
    GraphsModified(index,gnos,UNDO_APPEARANCE);
    delete[] gnos;
    delete[] sel;
    //return RETURN_SUCCESS;
}

void frmAutoscaleGraphs::doClose(void)
{
    hide();
}

void frmAutoscaleGraphs::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmAutoscaleGraphs::define_autos(int aon, int au, int ap)
{
    int i, ming, maxg;

    if (au >= 0 && !is_set_active(cg, au)) {
        errmsg(tr("Set not active").toLocal8Bit().constData());
        ApplyError=true;
        return;
    }
    if (ap) {
        ming = 0;
        maxg = number_of_graphs() - 1;
    } else {
        ming = cg;
        maxg = cg;
    }
    if (ming == cg && maxg == cg) {
        if (!is_graph_active(cg)) {
            errmsg(tr("Current graph is not active!").toLocal8Bit().constData());
            ApplyError=true;
            return;
        }
    }
    for (i = ming; i <= maxg; i++) {
        if (is_graph_active(i)) {
            autoscale_byset(i, au, aon);
        }
    }
    if (FormAxisProperties!=NULL)
        FormAxisProperties->update_ticks(cg);
    mainWin->mainArea->completeRedraw();
}

frmHotLinks::frmHotLinks(QWidget * parent):QDialog(parent)
{
    int number;
    QString entr[8];
//setFont(*stdFont);
    setWindowIcon(QIcon(*GraceIcon));
    setWindowTitle(tr("QtGrace: Hot links"));
    lblLinkSet=new QLabel(tr("Link set:"),this);
    hotlink_set_item=new uniList(SETCHOICE,this);
    hotlink_set_item->setBehavior(false,true,false);
    hotlink_list_item=new uniList(TEXTLIST,this);

    hotlink_file_item=new stdLineEdit(this,tr("To file or SOURCE_PIPE:"));
    hotlink_file_item->setText(QString(""));
    number=2;
    entr[0]=tr("Disk file");
    entr[1]=tr("Pipe");
    hotlink_source_item=new StdSelector(this,tr("Source:"),number,entr);
    number=6;
    entr[0]=tr("Never");
    entr[1]=tr("Every 1 sec");
    entr[2]=tr("Every 5 sec");
    entr[3]=tr("Every 10 sec");
    entr[4]=tr("Every 30 sec");
    entr[5]=tr("Every 60 sec");
    auto_hotlink_update=new StdSelector(this,tr("Autoupdate hotlinks:"),number,entr);
    connect(auto_hotlink_update->cmbSelect,SIGNAL(activated(int)),SLOT(autoupdatechanged(int)));
    buttons[0]=new QPushButton(tr("Link"),this);
    buttons[0]->setDefault(TRUE);
    connect(buttons[0],SIGNAL(clicked()),SLOT(doLink()));
    buttons[1]=new QPushButton(tr("Files..."),this);
    connect(buttons[1],SIGNAL(clicked()),SLOT(doFiles()));
    buttons[2]=new QPushButton(tr("Unlink"),this);
    connect(buttons[2],SIGNAL(clicked()),SLOT(doUnlink()));
    buttons[3]=new QPushButton(tr("Update"),this);
    connect(buttons[3],SIGNAL(clicked()),SLOT(do_hotupdate_proc()));
    buttons[4]=new QPushButton(tr("Close"),this);
    connect(buttons[4],SIGNAL(clicked()),SLOT(doClose()));

    autoupdatetimer=new QTimer(this);
    connect(autoupdatetimer,SIGNAL(timeout()),SLOT(do_hotupdate_proc()));

    layout=new QGridLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(hotlink_list_item,0,0,1,5);
    layout->addWidget(lblLinkSet,1,0,1,5);
    layout->addWidget(hotlink_set_item,2,0,1,5);
    layout->addWidget(hotlink_file_item,3,0,1,5);
    layout->addWidget(hotlink_source_item,4,0,1,5);
    layout->addWidget(auto_hotlink_update,5,0,1,5);
    layout->addWidget(buttons[0],6,0);
    layout->addWidget(buttons[1],6,1);
    layout->addWidget(buttons[2],6,2);
    layout->addWidget(buttons[3],6,3);
    layout->addWidget(buttons[4],6,4);
    setLayout(layout);
    resize(LastSize_FormHotLinks);
}

frmHotLinks::~frmHotLinks()
{
    LastSize_FormHotLinks=this->size();
}

void frmHotLinks::init(void)
{
    hotlink_set_item->update_number_of_entries();
    /*hotlink_list_item->update_number_of_entries();*/
    update_hotlinks();
}

void frmHotLinks::doLink(void)
/*
 * establish hotlinks by associating a set with a column in a file
 * or the stdout of a process
 */
{
    int i, numset, src, *sets=new int[5];
    int * gnos;
    char fname[GR_MAXPATHLEN];
    char buf[GR_MAXPATHLEN];
    QString xms;

    set_wait_cursor();

    numset = GetSelectedSets(hotlink_set_item, &sets);
    src = GetChoice(hotlink_source_item);
    //strcpy(fname, xv_getstr(hotlink_file_item));
    strcpy(fname,hotlink_file_item->text().toLocal8Bit());

    if (numset <= 0){//if (numset == SET_SELECT_ERROR) {
        errwin(tr("No set selected").toLocal8Bit().constData());
        goto end_doLink;
    }
    if (fname[0] == '\0') {
        errwin(tr("No source selected").toLocal8Bit().constData());
        goto end_doLink;
    }
    gnos=new int[numset+1];
    for (int i=0;i<numset;i++) gnos[i]=cg;
    SaveHotlinkData(numset,gnos,sets);
    for( i=0; i<numset; i++ ) {
        if( numset == 1 )
            sprintf(buf, "G%d.S%d -> %s -> %s", cg, sets[i],src==0 ? "DISK" : "PIPE", fname );
        else
            sprintf(buf, "G%d.S%d -> %s -> %s:%d", cg, sets[i],src == 0 ? "DISK" : "PIPE", fname, i+1);
        xms=QString(buf);
        //xms = XmStringCreateLocalized(buf);
        ///XmListAddItemUnselected(hotlink_list_item, xms, 0);
        hotlink_list_item->add_Item(xms);
        //XmStringFree(xms);
        set_hotlink(cg, sets[i], i+1, fname, src==0?SOURCE_DISK:SOURCE_PIPE);
        if( numset == 1 )
            setcomment( cg, sets[i], fname );
        else {
            sprintf( buf, "%s:%d", fname, i+1 );
            setcomment( cg, sets[i], buf );
        }
    }
    HotlinkModified(numset,gnos,sets);
    delete[] gnos;
end_doLink:
    delete[] sets;
    unset_wait_cursor();
}

void frmHotLinks::do_hotupdate_proc(void)
/*
 * update the sets in the current graph
 */
{
    int gno, setno;
    set_wait_cursor();
    /* do links */
    int nr;
    int *gnos=new int[2];
    int *snos=new int[2];
    getHotlinkedSets(&nr,&gnos,&snos);
    SaveSetStatesPrevious(nr,gnos,snos,UNDO_DATA);
    for (gno = 0; gno < number_of_graphs(); gno++)
    {
        for (setno = 0; setno < number_of_sets(gno); setno++)
        {
            do_update_hotlink(gno, setno);
        }
    }
    SetsModified(nr,gnos,snos,UNDO_DATA);
    addAditionalDescriptionToLastNode(-1,QObject::tr("Update sets by hotlink"),QString(),-1);
    delete[] gnos;
    delete[] snos;
    unset_wait_cursor();
    mainWin->mainArea->completeRedraw();
}

void frmHotLinks::autoupdatechanged(int a)
{
    if (autoupdatetimer->isActive()==true) autoupdatetimer->stop();
    switch (a)
    {
    default:
    case 0://never
    break;
    case 1://1sec
        autoupdatetimer->start(1000);
    break;
    case 2://5sec
        autoupdatetimer->start(5000);
    break;
    case 3://10sec
        autoupdatetimer->start(10000);
    break;
    case 4://30sec
        autoupdatetimer->start(30000);
    break;
    case 5://60sec
        autoupdatetimer->start(60000);
    break;
    }
}

void frmHotLinks::doFiles(void)
{
    if (FormSelectHotLink==NULL)
    {
        FormSelectHotLink=new frmIOForm(SELECT_HOT_LINK_FILE,this);
        connect(FormSelectHotLink,SIGNAL(newFileSelectedForIO(int,QString,bool,bool,bool)),SLOT(newLinkFileSelected(int,QString,bool,bool,bool)));
    }
    FormSelectHotLink->init();
    FormSelectHotLink->show();
    FormSelectHotLink->raise();
}

void frmHotLinks::doUnlink(void)
/*
 * unlink sets
 */
{
    ///XmString *s, cs;
    QString *s=new QString[3], cs;
    int  cnt, setno;
    char *cstr=new char[256];

    set_wait_cursor();

    ///XtVaGetValues(hotlink_list_item, XmNselectedItemCount, &cnt,XmNselectedItems, &s,NULL);
    hotlink_list_item->get_selection(&cnt,&s);
    ///hotlink_list_item
    if( cnt ) {
        for( ; cnt; cnt-- ) {
            ///cs = XmStringCopy(s[cnt-1]);
            cs=s[cnt-1];
            //if ((cstr = GetStringSimple(cs)))
            if ( strcpy(cstr,cs.toLocal8Bit()) )
            {
                sscanf(cstr, "G%*d.S%d", &setno);
                if (setno >= 0 && setno < number_of_sets(get_cg())) {
                    set_hotlink(get_cg(), setno, FALSE, NULL, 0);
                }
            }
            ///XmStringFree(cs);
            ///XtFree(cstr);
        }
        update_hotlinks();
    }
    delete[] s;
    delete[] cstr;
    unset_wait_cursor();
}

void frmHotLinks::update_hotlinks(void)
/*
 * update hot links displayed in scrolled list
 */
{
    int j;
    char buf[256];
    QString xms;

    //if (hotlink_frame != NULL) {
    set_wait_cursor();
    hotlink_list_item->clear();
    hotlink_list_item->number_of_entries=0;
    for (j = 0; j < number_of_sets(get_cg()); j++) {
//cout << "hotlinks: setnr=" << j << " ";
        if (is_hotlinked(get_cg(), j)) {
            sprintf(buf, "G%d.S%d -> %s -> %s:%d", get_cg(), j,
                    get_hotlink_src(get_cg(), j) == SOURCE_DISK ? "DISK" : "PIPE",
                    get_hotlink_file(get_cg(), j), is_hotlinked(get_cg(),j) );
//cout << "buf=" << buf << endl;
            xms=QString(buf);
            hotlink_list_item->add_Item(xms);
        }
    }
//cout << "number_f_entries=" << hotlink_list_item->number_of_entries << " count=" << hotlink_list_item->count() << endl;
    /// hotlink_list_item->update_number_of_entries_preserve_selection();
    unset_wait_cursor();
    //}
}

void frmHotLinks::doClose(void)
{
    hide();
}

void frmHotLinks::newLinkFileSelected(int type,QString file,bool exists,bool writeable,bool readable)
{
    (void)type;
    (void)exists;
    (void)writeable;
    (void)readable;
    char * dummy=new char[file.length()+1];
    strcpy(dummy,file.toLocal8Bit());
    /*//for testing -- maybe some tests needed (readable files...)
cout << "type=" << type << endl;
cout << "file=" << dummy << endl;
cout << "exists=" << exists << endl;
cout << "writeable=" << writeable << endl;
cout << "readable=" << readable << endl;*/
    FormSelectHotLink->hide();
    hotlink_file_item->setText(file);
    delete[] dummy;
}


frmLocatorProps::frmLocatorProps(QWidget * parent):QDialog(parent)
{
    int number;
    QString * entr=new QString[NUM_FMT_OPTION_ITEMS+2];
    char dummy[10];

//setFont(*stdFont);
    setWindowTitle(tr("QtGrace: Locator properties"));
    setWindowIcon(QIcon(*GraceIcon));

    fraXProp=new QGroupBox(tr("X properties"),this);
    fraYProp=new QGroupBox(tr("Y properties"),this);
    fraFixedPoint=new QGroupBox(tr("Fixed Point"),this);

    number=6;
    entr[0]=QString("[X, Y]");
    entr[1]=QString("[DX, DY]");
    entr[2]=QString("[DISTANCE]");
    entr[3]=QString("[Phi, Rho]");
    entr[4]=QString("[VX, VY]");
    entr[5]=QString("[SX, SY]");

    selLocDisplType=new StdSelector(this,tr("Locator display type:"),number,entr);
    number=NUM_FMT_OPTION_ITEMS;
    for (int i=0;i<number;i++)
    {
        entr[i]=QString(fmt_option_items[i].label);
    }
    selXFormat=new StdSelector(fraXProp,tr("Format:"),number,entr);
    selXFormat->setCurrentIndex(2);
    selYFormat=new StdSelector(fraYProp,tr("Format:"),number,entr);
    selYFormat->setCurrentIndex(2);
    number=17;
    for (int i=0;i<number;i++)
    {
        sprintf(dummy,"%d",i);
        entr[i]=QString(dummy);
    }
    selXPrecision=new StdSelector(fraXProp,tr("Precision:"),number,entr);
    selXPrecision->setCurrentIndex(6);
    selYPrecision=new StdSelector(fraYProp,tr("Precision:"),number,entr);
    selYPrecision->setCurrentIndex(6);

    chkEnableFixed=new QCheckBox(tr("Enable"),fraFixedPoint);
    chkEnableFixed->setToolTip(tr("Show a set fixpoint for a graph.\nA fixpoint is a point of reference for measurements."));
    chkShowFixedInFile=new QCheckBox(tr("Show fixpoint in file-export"),fraFixedPoint);
    chkShowFixedInFile->setToolTip(tr("If checked, the fixpoint will be exported to files and printed on printers.\nIf not checked the fixpoint will only be visible on screen."));
    ledFixedCoords[0]=new stdLineEdit(fraFixedPoint,tr("X:"));
    ledFixedCoords[1]=new stdLineEdit(fraFixedPoint,tr("Y:"));

    layout1=new QVBoxLayout;
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout1->addWidget(selXFormat);
    layout1->addWidget(selXPrecision);
    fraXProp->setLayout(layout1);
    layout2=new QVBoxLayout;
    //layout2->setMargin(STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout2->addWidget(selYFormat);
    layout2->addWidget(selYPrecision);
    fraYProp->setLayout(layout2);

    layout3=new QGridLayout;
    //layout3->setMargin(STD_MARGIN);
    layout3->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout3->addWidget(chkEnableFixed,0,0);
    layout3->addWidget(chkShowFixedInFile,0,1);
    layout3->addWidget(ledFixedCoords[0],1,0);
    layout3->addWidget(ledFixedCoords[1],1,1);
    fraFixedPoint->setLayout(layout3);

    buttonGroup=new stdButtonGroup(this);
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));

    connect(chkEnableFixed,SIGNAL(toggled(bool)),SLOT(enableToggled(bool)));

    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(selLocDisplType);
    layout->addWidget(fraXProp);
    layout->addWidget(fraYProp);
    layout->addWidget(fraFixedPoint);
    layout->addWidget(buttonGroup);
    layout->setStretch(0,0);
    layout->setStretch(1,1);
    layout->setStretch(2,1);
    layout->setStretch(3,1);
    layout->setStretch(4,0);
    setLayout(layout);
    resize(LastSize_FormLocatorProps);
    delete[] entr;
}

frmLocatorProps::~frmLocatorProps()
{
    LastSize_FormLocatorProps=this->size();
}

void frmLocatorProps::doApply(void)
{
    (void)locator_define_notify_proc();
}

void frmLocatorProps::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmLocatorProps::doClose(void)
{
    hide();
}

void frmLocatorProps::init(void)
{
    update_locator_items(get_cg());
}

/*
 * Notify and event procs
 */
int frmLocatorProps::locator_define_notify_proc(void)
{
    ApplyError=false;
    char dummy[MAX_STRING_LENGTH];
    GLocator locator;
    GLocator locator2;
    int gno;

    gno = get_cg();

    if (get_graph_locator(gno, &locator) != RETURN_SUCCESS) {
        return RETURN_FAILURE;
    }

    get_graph_locator(gno, &locator2);//to old state (for comparison)
    SaveLocatorFixPoint(gno);
    locator.pt_type = selLocDisplType->currentIndex();
    locator.fx = selXFormat->currentIndex();
    locator.fy = selYFormat->currentIndex();
    locator.px = selXPrecision->currentIndex();
    locator.py = selYPrecision->currentIndex();
    locator.pointset = chkEnableFixed->isChecked()==true?1:0;
    locator.plot_in_file = chkShowFixedInFile->isChecked()==true?TRUE:FALSE;
    xv_evalexpr(ledFixedCoords[0], &locator.dsx );
    xv_evalexpr(ledFixedCoords[1], &locator.dsy );

    ListOfChanges.clear();
    sprintf(dummy,"with g%d",gno);
    ListOfChanges << QString(dummy);

    if (locator.pt_type != locator2.pt_type)
    {
        sprintf(dummy,"g%d fixedpoint type %d",gno,locator.pt_type);
        ListOfChanges << QString(dummy);
    }
    if (locator.fx!=locator2.fx || locator.fy!=locator2.fy)
    {
        sprintf(dummy,"g%d fixedpoint format %s, %s",get_cg(),fmt_option_items[locator.fx].label,fmt_option_items[locator.fy].label);
        ListOfChanges << QString(dummy);
    }
    if (locator.px!=locator2.px || locator.py!=locator2.py)
    {
        sprintf(dummy,"g%d fixedpoint prec %d, %d",get_cg(),locator.px,locator.py);
        ListOfChanges << QString(dummy);
    }
    if (locator.pointset!=locator2.pointset)
    {
        sprintf(dummy,"g%d fixedpoint %s",gno,locator.pointset?"on":"off");
        ListOfChanges << QString(dummy);
    }
    if (locator.dsx!=locator2.dsx || locator.dsy!=locator2.dsy)
    {
        sprintf(dummy,"g%d fixedpoint xy %f, %f",gno,locator.dsx,locator.dsy);
        //cout << locator2.dsx << " " << locator2.dsy << endl;
        ListOfChanges << QString(dummy);
    }
    if (locator.plot_in_file!=locator2.plot_in_file)
    {
        sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: FIXPOINT G %d SHOW_IN_FILE %d\n",gno,locator.plot_in_file);
        ListOfChanges << QString(dummy);
    }
    /*
for (int i=0;i<ListOfChanges.size();i++)
cout << ListOfChanges.at(i).toLocal8Bit().constData() << endl;
*/
    set_graph_locator(gno, &locator);
    LocatorFixPointModified(gno);
    mainWin->mainArea->completeRedraw();
    return 0;
}

void frmLocatorProps::update_locator_items(int gno)
{
    GLocator locator;
    char buf[256];

    if (get_graph_locator(gno, &locator) != RETURN_SUCCESS)
    {
        return;
    }

    setWindowTitle(tr("QtGrace: Locator properties (G") + QString::number(gno) + QString(")"));

    int number=6;
    QString * entr=new QString[33];
    if (DecimalPointToUse=='.')
    {
        entr[0]=QString("[X, Y]");
        entr[1]=QString("[DX, DY]");
        entr[2]=QString("[DISTANCE]");
        entr[3]=QString("[Phi, Rho]");
        entr[4]=QString("[VX, VY]");
        entr[5]=QString("[SX, SY]");
    }
    else//',' is decimal separator --> use a '|' as separation between numbers
    {
        entr[0]=QString("[X | Y]");
        entr[1]=QString("[DX | DY]");
        entr[2]=QString("[DISTANCE]");
        entr[3]=QString("[Phi | Rho]");
        entr[4]=QString("[VX | VY]");
        entr[5]=QString("[SX | SY]");
    }
    selLocDisplType->setNewEntries(number,entr);
    delete[] entr;

    selLocDisplType->setCurrentIndex(locator.pt_type);

    if (locator.pointset==1)
        chkEnableFixed->setChecked(TRUE);
    else
        chkEnableFixed->setChecked(FALSE);
    if (locator.plot_in_file==TRUE)
        chkShowFixedInFile->setChecked(TRUE);
    else
        chkShowFixedInFile->setChecked(FALSE);

    selXFormat->setCurrentIndex(locator.fx);
    selYFormat->setCurrentIndex(locator.fy);
    /*SetOptionChoice(loc_formatx, locator.fx);
    SetOptionChoice(loc_formaty, locator.fy);*/
    selXPrecision->setCurrentIndex(locator.px);
    selYPrecision->setCurrentIndex(locator.py);
    /*SetChoice(loc_precx, locator.px);
    SetChoice(loc_precy, locator.py);*/
    sprintf(buf, "%g", locator.dsx);
    SetDecimalSeparatorToUserValue(buf);
    ledFixedCoords[0]->setText(QString(buf));
    //xv_setstr(locx_item, buf);
    sprintf(buf, "%g", locator.dsy);
    SetDecimalSeparatorToUserValue(buf);
    ledFixedCoords[1]->setText(QString(buf));
    //xv_setstr(locy_item, buf);
}

void frmLocatorProps::enableToggled(bool ch)
{
chkShowFixedInFile->setEnabled(ch);
}


frmUndoList::frmUndoList(QWidget * parent):QDialog(parent)
{
//setFont(*stdFont);
    setWindowTitle(tr("QtGrace: UndoList"));
    setWindowIcon(QIcon(*GraceIcon));
    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->setSpacing(STD_SPACING);
    list=new QListWidget(this);
    list->setSelectionMode(QAbstractItemView::ExtendedSelection);

    aac=new stdButtonGroup(this,true,true,false);
    aac->cmdApply->setText(tr("Undo"));
    aac->cmdAccept->setText(tr("Redo"));

    cmdCommands=new QPushButton(tr("Translate selection to commands"),this);
    connect(cmdCommands,SIGNAL(clicked()),SLOT(doCommands()));

    connect(aac->cmdApply,SIGNAL(clicked()),SLOT(doUndo()));
    connect(aac->cmdAccept,SIGNAL(clicked()),SLOT(doRedo()));
    connect(aac->cmdClose,SIGNAL(clicked()),SLOT(doClose()));
    chkActive=new QCheckBox(tr("Undo active"),this);
    connect(chkActive,SIGNAL(stateChanged(int)),SLOT(doToggleActive(int)));
    layout->addWidget(chkActive);
    layout->addWidget(list);
    layout->addWidget(cmdCommands);
    layout->addWidget(aac);
    setLayout(layout);
    resize(LastSize_FormUndoList);
}

frmUndoList::~frmUndoList()
{
    LastSize_FormUndoList=this->size();
}

void frmUndoList::init(void)
{
    int nr_of_Nodes=getNrOfNodes();
    list->clear();
//cout << "nr_of_Nodes=" << nr_of_Nodes << " Count=" << list->count() << endl;
    chkActive->blockSignals(true);
    chkActive->setChecked(undo_active);
    chkActive->blockSignals(false);
//cout << "nr_of_Nodes=" << nr_of_Nodes << " Count=" << list->count() << endl;
    QStringList StrList;
    char dummy[256];
    sprintf(dummy," (%d / %d)",nr_of_Nodes,max_node_nr);
    setWindowTitle(tr("QtGrace: UndoList")+QString(dummy));
    for (int i=0;i<nr_of_Nodes;i++)
    {
        StrList << getNodeDescription(i);
    }
    list->addItems(StrList);
    list->update(list->model()->index(0,0));
    list->repaint();
//cout << "count=" << list->count() << endl;
}

void frmUndoList::doUndo(void)
{
    if (undo_active==FALSE)
    {
    errmsg(tr("Undo not activated.").toLocal8Bit().constData());
    return;
    }
    int nr=list->count();
    int * selection=new int[nr>0?nr:2];
    int index=0;
    int sav_und_act=undo_active;
    bool old_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    undo_active=FALSE;
    for (int i=0;i<nr;i++)
    {
        if (list->item(i)->isSelected()==true)
        {
            selection[index++]=i;
        }
    }
    wait_till_update=true;
    for (int i=index-1;i>=0;i--)
    {
        RestoreNode(selection[i]);
    }
    init();
    for (int i=0;i<index;i++)
    {
        list->item(selection[i])->setSelected(true);
    }
    delete[] selection;
    undo_active=sav_und_act;
    wait_till_update=false;
    chkActive->setChecked(undo_active);
    immediateUpdate=old_upd;
    updateRunning=false;
    mainWin->mainArea->completeRedraw();
}

void frmUndoList::doRedo(void)
{
    if (undo_active==FALSE)
    {
    errmsg(tr("Undo not activated.").toLocal8Bit().constData());
    return;
    }
    int nr=list->count();
    int * selection=new int[nr>0?nr:2];
    int index=0;
    int sav_und_act=undo_active;
    bool old_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    undo_active=FALSE;
    for (int i=0;i<nr;i++)
    {
        if (list->item(i)->isSelected()==true)
        {
            selection[index++]=i;
        }
    }
    wait_till_update=true;
    for (int i=0;i<index;i++)
    {
        RestoreNode(selection[i]);
    }
    init();
    for (int i=0;i<index;i++)
    {
        list->item(selection[i])->setSelected(true);
    }
    delete[] selection;
    mainWin->mainArea->completeRedraw();
    undo_active=sav_und_act;
    wait_till_update=false;
    chkActive->setChecked(undo_active);
    immediateUpdate=old_upd;
    updateRunning=false;
}

void frmUndoList::doCommands(void)
{
    undo_node * Node;
    QString errortext;
    int nr=list->count(),nr2,read_count;
    int * selection=new int[nr>0?nr:2];
    int index=0,gr_n,new_n;
    for (int i=0;i<nr;i++)
    {
        if (list->item(i)->isSelected()==true)
        {
            selection[index++]=i;
        }
    }
    if (index==0)
    {
    errortext=tr("Please select at least one entry!");
    errmsg(errortext.toLocal8Bit().constData());
    return;
    }
    //index = number of selected entries, selection = indices of selection
    if (FormCommands==NULL)
    {
        FormCommands=new frmCommands(mainWin);
        update_all();
    }
    new_n=gr_n=-1;//current graph number is invalid; in this way the first 'with'-command is included!
    for (int i=0;i<index;i++)
    {
        Node=getNodeByNr(selection[i]);
        nr=nr_of_true_changes(Node->Changes);
        nr2=nr_of_true_changes(Node->OldStates);
        if ((Node->active==true || (Node->active==false && nr2<=0)) && nr>0)
        {
            if (Node->active==false)
            {
            errortext=tr("No translatable command in active node. Using inverse node.");
            errmsg(errortext.toLocal8Bit().constData());
            }
                for (int j=0;j<Node->Changes.length();j++)
                {
                    if (Node->Changes.at(j).indexOf("with")==0)//commands begins with 'with' --> new graph-nr?
                    {
                        strcpy(dummy,Node->Changes.at(j).toLocal8Bit());
                        read_count=sscanf(dummy,"with G%d",&new_n);
                        if (new_n!=gr_n || read_count<=0)//include 'with'-commands only if there is really a new graph-nr
                        {
                            FormCommands->list->addItem(Node->Changes.at(j));
                            FormCommands->updateRowColor(FormCommands->list->count()-1);
                            gr_n=new_n;
                        }
                    }
                    else//command does not begin with 'with'
                    {
                        FormCommands->list->addItem(Node->Changes.at(j));
                        FormCommands->updateRowColor(FormCommands->list->count()-1);
                    }
                }
        }
        else if ((Node->active==false || (Node->active==true && nr<=0)) && nr2>0)//already undone
        {
            if (Node->active==true)
            {
            errortext=tr("No translatable command in active node. Using inverse node.");
            errmsg(errortext.toLocal8Bit().constData());
            }
                for (int j=0;j<Node->OldStates.length();j++)
                {
                    if (Node->OldStates.at(j).indexOf("with")==0)//commands begins with 'with' --> new graph-nr?
                    {
                        strcpy(dummy,Node->OldStates.at(j).toLocal8Bit());
                        read_count=sscanf(dummy,"with G%d",&new_n);
                        if (new_n!=gr_n || read_count<=0)//include 'with'-commands only if there is really a new graph-nr
                        {
                            FormCommands->list->addItem(Node->OldStates.at(j));
                            FormCommands->updateRowColor(FormCommands->list->count()-1);
                            gr_n=new_n;
                        }
                    }
                    else//command does not begin with 'with'
                    {
                        FormCommands->list->addItem(Node->OldStates.at(j));
                        FormCommands->updateRowColor(FormCommands->list->count()-1);
                    }
                }
        }
        else
        {
        errortext=tr("No translatable command in node.");
        errmsg(errortext.toLocal8Bit().constData());
        }
    }
    FormCommands->list->scrollToItem(FormCommands->list->item(FormCommands->list->count()-1));
    FormCommands->show();
    FormCommands->raise();
    FormCommands->activateWindow();
}

void frmUndoList::doClose(void)
{
    hide();
}

void frmUndoList::doToggleActive(int state)
{
    (void)state;
//cout << "Start Toggled" << endl;
    undo_active=chkActive->isChecked();
    if (Form_Preferences!=NULL)
    {
    Form_Preferences->chkUndoActive->blockSignals(true);
    Form_Preferences->chkUndoActive->setChecked(undo_active);
    Form_Preferences->chkUndoActive->blockSignals(false);
    }
    if (undo_active==FALSE)
    {
    mainWin->cmdUndo->setEnabled(false);
    mainWin->cmdRedo->setEnabled(false);
    mainWin->actUndo->setEnabled(false);
    mainWin->actRedo->setEnabled(false);
    }
    else
    {
    CheckActive();
    }
//cout << "End Toggled" << endl;
}

frmExplorer::frmExplorer(QWidget * parent):QDialog(parent)
{
    GlobalInhibitor=true;
    oldSetting='0';
//setFont(*stdFont);
    setWindowTitle(tr("QtGrace: Explorer"));
    setWindowIcon(QIcon(*GraceIcon));
    layout=new QGridLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->setSpacing(STD_SPACING);
    tree=new treeView(this);
    tree->popup->par2=this;
    connect(tree,SIGNAL(newItemClicked(char,int,int)),this,SLOT(itemClickedInTree(char,int,int)));
    lblTest=new QLabel("Nothing selected!",this);

    LineProperties=new frmLine_Props(this,true);
    LineProperties->buttonGroup->hide();
    LineProperties->hide();
    TextProperties=new frmText_Props(this,true);
    TextProperties->buttonGroup->hide();
    TextProperties->hide();
    EllipseProperties=new frmEllipse_Props(this,true,true);
    EllipseProperties->buttonGroup->hide();
    EllipseProperties->hide();
    BoxProperties=new frmEllipse_Props(this,true,false);
    BoxProperties->buttonGroup->hide();
    BoxProperties->hide();

    AxisProperties=new frmAxis_Prop(this);
    AxisProperties->buttonGroup->hide();
    AxisProperties->hide();
    AxisProperties->chkActive->hide();
    AxisProperties->selEdit->hide();

    SetProperties=new frmSet_Appearance(this);
    SetProperties->menuBar->hide();
    SetProperties->buttonGroup->hide();
    SetProperties->hide();
    SetProperties->listSet->hide();
    SetProperties->lblSelSet->hide();
    SetProperties->listSet->prevent_from_autoupdate=true;

    GraphProperties=new frmGraph_App(this);
    GraphProperties->menuBar->hide();
    GraphProperties->buttonGroup->hide();
    GraphProperties->hide();
    GraphProperties->listGraph->hide();
    GraphProperties->lblTitle->hide();
    GraphProperties->listGraph->prevent_from_autoupdate=true;
    GraphProperties->tabMain->grpDispOpt->hide();
    //GraphProp TabMain Title und Subtitle
    GraphProperties->tabTitles->grpTitle->setTitle(tr("Settings"));
    GraphProperties->tabTitles->grpSubTitle->setTitle(tr("Settings"));
    GraphProperties->tabMain->grpTitles->setTitle(tr("String"));
    GraphProperties->tabMain->layout->addWidget(GraphProperties->tabTitles->grpTitle);
    GraphProperties->tabMain->layout->addWidget(GraphProperties->tabTitles->grpSubTitle);
    GraphProperties->tabMain->layout->addStretch(100);

    PlotAppearance=new frmPlot_Appearance(this);
    PlotAppearance->buttonGroup->hide();
    PlotAppearance->hide();

    aac=new stdButtonGroup(this);
    connect(aac->cmdAccept,SIGNAL(clicked()),SLOT(doAccept()));
    connect(aac->cmdApply,SIGNAL(clicked()),SLOT(doApply()));
    connect(aac->cmdClose,SIGNAL(clicked()),SLOT(doClose()));
    layout->addWidget(tree,0,0,1,1);

    connect(AxisProperties,SIGNAL(closeWish()),SLOT(doClose()));
    connect(SetProperties,SIGNAL(closeWish()),SLOT(doClose()));
    connect(GraphProperties,SIGNAL(closeWish()),SLOT(doClose()));

    layout->addWidget(lblTest,0,1,1,1);
    layout->addWidget(LineProperties,0,1,1,1);
    layout->addWidget(TextProperties,0,1,1,1);
    layout->addWidget(EllipseProperties,0,1,1,1);
    layout->addWidget(BoxProperties,0,1,1,1);
    layout->addWidget(AxisProperties,0,1,1,1);
    layout->addWidget(SetProperties,0,1,1,1);
    layout->addWidget(GraphProperties,0,1,1,1);
    layout->addWidget(PlotAppearance,0,1,1,1);

    layout->addWidget(aac,1,0,1,2);
    setLayout(layout);
    aac->cmdApply->setDefault(true);
    GlobalInhibitor=false;
    resize(LastSize_FormExplorer);
}

frmExplorer::~frmExplorer()
{
    LastSize_FormExplorer=this->size();
}

void frmExplorer::init(void)
{
    tree->RecreateCompleteTree();
    QTreeWidgetItem * item=tree->findItem('R',-1,-1);
    if (item!=NULL)
    {
        tree->clearSelection();
        tree->itemClickedAt(item,0);
        item->setSelected(true);
    }
}

void frmExplorer::doApply(void)
{
    ApplyError=false;

    switch (oldSetting)
    {
    default:
    case (int)'0'://nothing
        ;
        break;
    case (int)'N'://String-Object
        TextProperties->doAccept();
        break;
    case (int)'I'://Line-Object
        LineProperties->doAccept();
        break;
    case (int)'B'://Box-Object
        BoxProperties->doAccept();
        break;
    case (int)'P'://Ellipse-Object
        EllipseProperties->doAccept();
        break;
    case (int)'E'://Legend
        GraphProperties->doApply();
        break;
    case (int)'S'://Set
        SetProperties->doApply();
        break;
    case (int)'A'://Axis
        AxisProperties->doApply();
        break;
    case (int)'R'://General Settings
        PlotAppearance->doApply();
        break;
    case (int)'G'://Graph
        GraphProperties->doApply();
        break;
    case (int)'L'://Label/Title
        GraphProperties->doApply();
        break;
    }

}

void frmExplorer::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmExplorer::doClose(void)
{
    hide();
}

void frmExplorer::setItemVisible(char type, bool vis,int gno,int sno)
{
    int ty=(int)type;
    (void)gno;
    switch (ty)
    {
    default:
    case (int)'0'://nothing
        lblTest->setVisible(vis);
        break;
    case (int)'N'://String-Object
        TextProperties->setVisible(vis);
        break;
    case (int)'I'://Line-Object
        LineProperties->setVisible(vis);
        break;
    case (int)'B'://Box-Object
        BoxProperties->setVisible(vis);
        break;
    case (int)'P'://Ellipse-Object
        EllipseProperties->setVisible(vis);
        break;
    case (int)'E'://Legend
        GraphProperties->tabs->clear();
        if (vis==true)
        {
            //GraphProperties->tabFrame->setVisible(false);
            GraphProperties->tabs->addTab(GraphProperties->tabLegBox, tr("Leg. box"));
            GraphProperties->tabs->addTab(GraphProperties->tabLegends, tr("Legends"));
            //GraphProperties->tabMain->setVisible(false);
            //GraphProperties->tabSpec->setVisible(false);
            //GraphProperties->tabTitles->setVisible(false);
        }
        GraphProperties->tabMain->grpDispOpt->hide();
        GraphProperties->setVisible(vis);
        break;
    case (int)'S'://Set
        SetProperties->setVisible(vis);
        break;
    case (int)'A'://Axis
        AxisProperties->setVisible(vis);
        break;
    case (int)'R'://General Settings
        PlotAppearance->setVisible(vis);
        break;
    case (int)'G'://Graph
        GraphProperties->tabs->clear();
        if (vis==true)
        {
            GraphProperties->tabMain->grpViewport->show();
            GraphProperties->tabMain->grpDispOpt->show();
            GraphProperties->tabMain->grpPres->show();
            GraphProperties->tabMain->grpTitles->hide();
            GraphProperties->tabs->addTab(GraphProperties->tabMain, tr("Main"));
            GraphProperties->tabs->addTab(GraphProperties->tabFrame, tr("Frame"));
            //GraphProperties->tabLegBox->setVisible(false);
            //GraphProperties->tabLegends->setVisible(false);
            GraphProperties->tabs->addTab(GraphProperties->tabSpec, tr("Special"));
            //GraphProperties->tabTitles->setVisible(false);
        }
        GraphProperties->tabMain->grpDispOpt->hide();
        GraphProperties->setVisible(vis);
        break;
    case (int)'L'://Label/Title
        GraphProperties->tabs->clear();
        if (vis==true)
        {
            //GraphProperties->tabFrame->setVisible(false);
            //GraphProperties->tabLegBox->setVisible(false);
            //GraphProperties->tabLegends->setVisible(false);
            GraphProperties->tabMain->grpViewport->hide();
            GraphProperties->tabMain->grpDispOpt->hide();
            GraphProperties->tabMain->grpPres->hide();
            GraphProperties->tabMain->grpTitles->show();
            if (sno==0)
            {
                GraphProperties->tabMain->ledTitle->show();
                GraphProperties->tabMain->ledSubtitle->hide();
                GraphProperties->tabTitles->grpTitle->show();
                GraphProperties->tabTitles->grpSubTitle->hide();
                GraphProperties->tabs->addTab(GraphProperties->tabMain, tr("Title-Properties"));
            }
            else
            {
                GraphProperties->tabMain->ledTitle->hide();
                GraphProperties->tabMain->ledSubtitle->show();
                GraphProperties->tabTitles->grpTitle->hide();
                GraphProperties->tabTitles->grpSubTitle->show();
                GraphProperties->tabs->addTab(GraphProperties->tabMain, tr("SubTitle-Properties"));
            }

            //GraphProperties->tabSpec->setVisible(false);
            //GraphProperties->tabs->addTab(GraphProperties->tabTitles, tr("Titles"));
        }
        GraphProperties->tabMain->grpDispOpt->hide();
        GraphProperties->setVisible(vis);
        break;
    }
}

void frmExplorer::initItem(char type,int gno,int sno)
{
    bool imUpd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    int ty=(int)type;
    switch (ty)
    {
    default:
    case (int)'0'://nothing
        ;
        break;
    case (int)'N'://String-Object
        TextProperties->init(gno);
        TextProperties->reDisplayContents();
        break;
    case (int)'I'://Line-Object
        LineProperties->init(gno);
        LineProperties->reDisplayContents();
        break;
    case (int)'B'://Box-Object
        BoxProperties->init(gno);
        BoxProperties->reDisplayContents();
        break;
    case (int)'P'://Ellipse-Object
        EllipseProperties->init(gno);
        EllipseProperties->reDisplayContents();
        break;
    case (int)'E'://Legend
        GraphProperties->show_graph_data_external(gno);
        GraphProperties->redisplayContents();
        break;
    case (int)'S'://Set
        SetProperties->ShowSetData_external(gno,sno);
        SetProperties->redisplayContents();
        break;
    case (int)'A'://Axis
        AxisProperties->curaxis = sno;
        AxisProperties->update_ticks(gno);
        AxisProperties->redisplayContents();
        break;
    case (int)'R'://General Settings
        PlotAppearance->init();
        PlotAppearance->redisplayContents();
        break;
    case (int)'G'://Graph
        GraphProperties->show_graph_data_external(gno);
        GraphProperties->redisplayContents();
        break;
    case (int)'L'://Label/Title
        GraphProperties->show_graph_data_external(gno);
        GraphProperties->redisplayContents();
        break;
    }
    immediateUpdate=imUpd;
    updateRunning=false;
}

void frmExplorer::itemClickedInTree(char type,int gno,int sno)
{
    simple_draw_setting|=SIMPLE_DRAW_GRAPH_FOCUS;
    switch_current_graph(gno);

    setItemVisible(oldSetting,false,gno,sno);
    initItem(type,gno,sno);
    setItemVisible(type,true,gno,sno);

    oldSetting=type;
}



frmMasterRegionOperator_Style::frmMasterRegionOperator_Style(QWidget * parent):QWidget(parent)
{
    layout=new QGridLayout(this);
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->setSpacing(STD_SPACING*2);
int line;
#if QT_VERSION >= 0x050000

#else
    mapColor=new QSignalMapper();
    mapLineW=new QSignalMapper();
    mapLineS=new QSignalMapper();
#endif
line=0;
for (int i=0;i<MAXREGION;i++)
{
selCol[i]=new ColorSelector(this);
//selCol[i]->alphaSelector->setVisible(false);//no alpha for regions
selWidth[i]=new LineWidthSelector(this);
selStyle[i]=new LineStyleSelector(this);
lblRegion[i]=new QLabel(tr("Region ")+QString::number(i)+QString(": "),this);
#if QT_VERSION >= 0x050000
    connect(selCol[i],QOverload<int>::of(&ColorSelector::currentIndexChanged),this,[this,i](int index){(void)index;this->setNewColor(i);});
    connect(selCol[i],QOverload<int>::of(&ColorSelector::alphaChanged),this,[this,i](int index){(void)index;this->setNewColor(i);});
    connect(selWidth[i],QOverload<double>::of(&LineWidthSelector::currentValueChanged),this,[this,i](double index){(void)index;this->setNewLineW(i);});
    connect(selStyle[i],QOverload<int>::of(&LineStyleSelector::currentIndexChanged),this,[this,i](int index){(void)index;this->setNewLineS(i);});
#else
    connect(selCol[i],SIGNAL(currentIndexChanged(int)),mapColor,SLOT(map()));
    connect(selCol[i],SIGNAL(alphaChanged(int)),mapColor,SLOT(map()));
    connect(selWidth[i],SIGNAL(currentValueChanged(double)),mapLineW,SLOT(map()));
    connect(selStyle[i],SIGNAL(currentIndexChanged(int)),mapLineS,SLOT(map()));
    mapColor->setMapping(selCol[i],i);
    mapLineW->setMapping(selWidth[i],i);
    mapLineS->setMapping(selStyle[i],i);
#endif
layout->addWidget(lblRegion[i],line,0);
layout->addWidget(selCol[i],line,1);
layout->addWidget(selWidth[i],line,2);
layout->addWidget(selStyle[i],line,3);
line++;
}
#if QT_VERSION >= 0x050000

#else
connect(mapColor, SIGNAL(mapped(int)),this, SLOT(setNewColor(int)));
connect(mapLineW, SIGNAL(mapped(int)),this, SLOT(setNewLineW(int)));
connect(mapLineS, SIGNAL(mapped(int)),this, SLOT(setNewLineS(int)));
#endif
layout->setColumnStretch(0,1);
layout->setColumnStretch(1,3);
layout->setColumnStretch(2,3);
layout->setColumnStretch(3,3);

cmdButtons=new stdButtonGroup(this);
connect(cmdButtons->cmdApply,SIGNAL(clicked()),SLOT(doApply()));
connect(cmdButtons->cmdAccept,SIGNAL(clicked()),SLOT(doAccept()));
connect(cmdButtons->cmdClose,SIGNAL(clicked()),SLOT(doClose()));
layout->addWidget(cmdButtons,line,0,1,4);

setLayout(layout);
}

void frmMasterRegionOperator_Style::init(void)
{
int savImmUpdate=immediateUpdate;
    immediateUpdate=false;
    for (int i=0;i<MAXREGION;i++)
    {
    selCol[i]->setCurrentIndex(rg[i].color);
    selCol[i]->setAlpha(rg[i].alpha);
    selWidth[i]->setValue(rg[i].linew);
    selStyle[i]->setCurrentIndex(rg[i].lines);
    }
    immediateUpdate=savImmUpdate;
}

void frmMasterRegionOperator_Style::setNewColor(int c)
{
if (immediateUpdate==false || c<0 || c>=MAXREGION) return;
QString text;
ListOfOldStates.clear();
ListOfChanges.clear();
text=QString("r")+QString::number(c)+QString(" color ")+QString::number(rg[c].color);
ListOfOldStates << text;
text=QString("#QTGRACE_ADDITIONAL_PARAMETER: REGION ")+QString::number(c)+QString(" ALPHA ")+QString::number(rg[c].alpha);
ListOfOldStates << text;
CopyRegion(rg+c,&undo_region);
    rg[c].color=selCol[c]->currentIndex();
    rg[c].alpha=selCol[c]->alpha();
text=QString("r")+QString::number(c)+QString(" color ")+QString::number(rg[c].color);
ListOfChanges << text;
text=QString("#QTGRACE_ADDITIONAL_PARAMETER: REGION ")+QString::number(c)+QString(" ALPHA ")+QString::number(rg[c].alpha);
ListOfChanges << text;
RegionModified(c,rg+c,0);
    mainWin->mainArea->completeRedraw();
}

void frmMasterRegionOperator_Style::setNewLineW(int w)
{
if (immediateUpdate==false || w<0 || w>=MAXREGION) return;
QString text;
ListOfOldStates.clear();
ListOfChanges.clear();
text=QString("r")+QString::number(w)+QString(" linewidth ")+QString::number(rg[w].linew,'f',1);
ListOfOldStates << text;
CopyRegion(rg+w,&undo_region);
    rg[w].linew=selWidth[w]->value();
text=QString("r")+QString::number(w)+QString(" linewidth ")+QString::number(rg[w].linew,'f',1);
ListOfChanges << text;
RegionModified(w,rg+w,0);
    mainWin->mainArea->completeRedraw();
}

void frmMasterRegionOperator_Style::setNewLineS(int s)
{
if (immediateUpdate==false || s<0 || s>=MAXREGION) return;
QString text;
ListOfOldStates.clear();
ListOfChanges.clear();
text=QString("r")+QString::number(s)+QString(" linestyle ")+QString::number(rg[s].lines);
ListOfOldStates << text;
CopyRegion(rg+s,&undo_region);
    rg[s].lines=selStyle[s]->currentIndex();
text=QString("r")+QString::number(s)+QString(" linestyle ")+QString::number(rg[s].lines);
ListOfChanges << text;
RegionModified(s,rg+s,0);
    mainWin->mainArea->completeRedraw();
}

void frmMasterRegionOperator_Style::doApply(void)
{
QString text;
bool true_changes;
ListOfOldStates.clear();
ListOfChanges.clear();
    for (int i=0;i<MAXREGION;i++)
    {
    ListOfOldStates.clear();
    ListOfChanges.clear();
    true_changes=false;
    CopyRegion(rg+i,&undo_region);
        if (rg[i].color!=selCol[i]->currentIndex())
        {
            true_changes=true;
            text=QString("r")+QString::number(i)+QString(" color ")+QString::number(rg[i].color);
            ListOfOldStates << text;
            rg[i].color=selCol[i]->currentIndex();
            text=QString("r")+QString::number(i)+QString(" color ")+QString::number(rg[i].color);
            ListOfChanges << text;
        }
        if (rg[i].alpha!=selCol[i]->alpha())
        {
            true_changes=true;
            text=QString("#QTGRACE_ADDITIONAL_PARAMETER: REGION ")+QString::number(i)+QString(" ALPHA ")+QString::number(rg[i].alpha);
            ListOfOldStates << text;
            rg[i].alpha=selCol[i]->alpha();
            text=QString("#QTGRACE_ADDITIONAL_PARAMETER: REGION ")+QString::number(i)+QString(" ALPHA ")+QString::number(rg[i].alpha);
            ListOfChanges << text;
        }
        if (rg[i].linew!=selWidth[i]->value())
        {
            true_changes=true;
            text=QString("r")+QString::number(i)+QString(" linewidth ")+QString::number(rg[i].linew,'f',1);
            ListOfOldStates << text;
            rg[i].linew=selWidth[i]->value();
            text=QString("r")+QString::number(i)+QString(" linewidth ")+QString::number(rg[i].linew,'f',1);
            ListOfChanges << text;
        }
        if (rg[i].lines!=selStyle[i]->currentIndex())
        {
            true_changes=true;
            text=QString("r")+QString::number(i)+QString(" linestyle ")+QString::number(rg[i].lines);
            ListOfOldStates << text;
            rg[i].lines=selStyle[i]->currentIndex();
            text=QString("r")+QString::number(i)+QString(" linestyle ")+QString::number(rg[i].lines);
            ListOfChanges << text;
        }
        if (true_changes==true)
        {
        RegionModified(i,rg+i,0);
        }
    }
    mainWin->mainArea->completeRedraw();
}

void frmMasterRegionOperator_Style::doAccept(void)
{
ApplyError=false;
doApply();
    if (ApplyError==false)
    doClose();
}

void frmMasterRegionOperator_Style::doClose(void)
{
emit(closeWish());
}

frmMasterRegionOperator_Edit::frmMasterRegionOperator_Edit(QWidget * parent):QWidget(parent)
{
int line=0;
in_definition=-1;//no region selected at the moment

    layout=new QGridLayout();
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->setSpacing(STD_SPACING);

    layout0=new QGridLayout();
    //layout0->setMargin(STD_MARGIN);
    layout0->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout0->setSpacing(STD_SPACING);

int number=5;
QString entr[16];
entr[0]=tr("Region 0");
entr[1]=tr("Region 1");
entr[2]=tr("Region 2");
entr[3]=tr("Region 3");
entr[4]=tr("Region 4");
selRegion=new StdSelector(this,tr("Edit:"),number,entr);
connect(selRegion->cmbSelect,SIGNAL(activated(int)),SLOT(regionChanged(int)));
#if QT_VERSION >= 0x050000

#else
plusMapper=new QSignalMapper();
minusMapper=new QSignalMapper();
#endif
number=10;
for (int j=0;j<number;j++)
entr[j]=regtypes[reg_order[j]];
regType=new StdSelector(this,tr("Type:"),number,entr);
regType->setValues(reg_order);
connect(regType->cmbSelect,SIGNAL(activated(int)),SLOT(regionTypeChanged(int)));
lblPoints=new QLabel(tr("Points:"),this);

Empty0=new QWidget(this);
layout1=new QHBoxLayout();
layout1->setSpacing(STD_SPACING);
//layout1->setMargin(0);
layout1->setContentsMargins(0,0,0,0);
layout1->addWidget(selRegion);
layout1->addWidget(regType);
layout1->addWidget(lblPoints);
Empty0->setLayout(layout1);
scroll=new QScrollArea(this);
scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
scroll->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);

Empty=new QWidget(scroll);
//Empty=new QFrame(this);
//Empty->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
Empty2=new QWidget(this);
Empty3=new QWidget(this);
scrollBar=new QScrollBar(Qt::Vertical,this);

cmdClearRegion=new QPushButton(tr("Clear Region"),this);
cmdClearRegion->setToolTip(tr("Remove all settings from this region.\nDeletes all limiting points of the region."));
cmdClearRegion->setEnabled(false);
cmdButtons=new stdButtonGroup(this);
connect(cmdButtons->cmdApply,SIGNAL(clicked()),SLOT(doApply()));
connect(cmdButtons->cmdAccept,SIGNAL(clicked()),SLOT(doAccept()));
connect(cmdButtons->cmdClose,SIGNAL(clicked()),SLOT(doClose()));
connect(cmdClearRegion,SIGNAL(clicked()),SLOT(doClearRegion()));
connect(scrollBar,SIGNAL(valueChanged(int)),SLOT(linesScolled(int)));
#if QT_VERSION >= 0x050000

#else
connect(plusMapper, SIGNAL(mapped(int)),this, SLOT(doAdd(int)));
connect(minusMapper, SIGNAL(mapped(int)),this, SLOT(doRemove(int)));
#endif
lblTitle[0]=new QLabel(tr("#"),Empty);
lblTitle[1]=new QLabel(tr("X"),Empty);
lblTitle[2]=new QLabel(tr("Y"),Empty);
lblTitle[3]=new QLabel(tr("+"),Empty);
lblTitle[4]=new QLabel(tr("-"),Empty);

layout0->addWidget(lblTitle[0],0,0,1,1);
layout0->addWidget(lblTitle[1],0,1,1,1);
layout0->addWidget(lblTitle[2],0,2,1,1);
layout0->addWidget(lblTitle[3],0,3,1,1);
layout0->addWidget(lblTitle[4],0,4,1,1);
layout0->addWidget(cmdClearRegion,1,1,1,5);
layout0->addWidget(Empty2,1,1,1,5);
layout0->addWidget(Empty3,1,1,1,5);
Empty->setLayout(layout0);
Empty2->setVisible(false);
Empty3->setVisible(false);
start_line=0;
max_lines=4;//allocated lines
nr_of_lines=0;//no data
nr_of_vis_lines=0;//do not show anything
ledCoords[0]=new QLineEdit*[max_lines];
ledCoords[1]=new QLineEdit*[max_lines];
lblCoords=new QLabel*[max_lines];
cmdEdit[0]=new QPushButton*[max_lines];
cmdEdit[1]=new QPushButton*[max_lines];
for (int i=0;i<max_lines;i++)
{
ledCoords[0][i]=new QLineEdit(QString(""),Empty);
ledCoords[1][i]=new QLineEdit(QString(""),Empty);
ledCoords[0][i]->setVisible(false);
ledCoords[1][i]->setVisible(false);
cmdEdit[0][i]=new QPushButton(QString(""),Empty);
cmdEdit[1][i]=new QPushButton(QString(""),Empty);
cmdEdit[0][i]->setVisible(false);
cmdEdit[1][i]->setVisible(false);
lblCoords[i]=new QLabel(QString(""),Empty);
lblCoords[i]->setVisible(false);
cmdEdit[0][i]->disconnect();
cmdEdit[1][i]->disconnect();
#if QT_VERSION >= 0x050000
    connect(cmdEdit[0][i],&QPushButton::clicked,this,[this,i](){this->doAdd(i);});
    connect(cmdEdit[1][i],&QPushButton::clicked,this,[this,i](){this->doRemove(i);});
#else
    connect(cmdEdit[0][i], SIGNAL(clicked()), plusMapper, SLOT(map()));
    connect(cmdEdit[1][i], SIGNAL(clicked()), minusMapper, SLOT(map()));
    plusMapper->setMapping(cmdEdit[0][i],i);
    minusMapper->setMapping(cmdEdit[1][i],i);
#endif
}
int growline;
scroll->setWidget(Empty);
/*layout->addWidget(selRegion,line,0,1,1);
layout->addWidget(regType,line,1,1,1);
layout->addWidget(lblPoints,line++,2,1,1);*/
layout->addWidget(Empty0,line++,0,1,3);
/*layout->addWidget(lblTitle[0],line,0,1,1);
layout->addWidget(lblTitle[1],line++,1,1,1);*/
layout->addWidget(scroll,line,0,1,2);
//layout->addWidget(Empty,line,0,1,2);
growline=line;
layout->addWidget(scrollBar,line++,2,1,1);
layout->addWidget(cmdButtons,line++,0,1,3);
for (int i=0;i<line;i++)
{
if (i==growline) layout->setRowStretch(i,10);
else layout->setRowStretch(i,0);
}
setLayout(layout);
}

void frmMasterRegionOperator_Edit::init(void)
{
regionChanged(0);
}

void frmMasterRegionOperator_Edit::showEvent(QShowEvent * event)
{
    (void)event;
//qDebug() << "show";
int re=selRegion->currentIndex();
bool is_defined=(bool)is_region_defined(rg+re);
    if (in_definition>=0 && is_defined==true && in_definition==re)
    {
    selRegion->setCurrentIndex(in_definition);
    }
    else
    {
    in_definition=-1;
    selRegion->setCurrentIndex(re);
    }
}

void frmMasterRegionOperator_Edit::resizeEvent(QResizeEvent *event)
{
static bool running=false;
if (running==true)
{
event->ignore();
return;
}
event->accept();
running=true;
/*QWidget::resizeEvent(event);
qApp->processEvents();*/
int def_size=25;
if (def_size<ledCoords[0][0]->height()) def_size=ledCoords[0][0]->height();
max_lines=scroll->height()/def_size-2;
//max_lines=Empty->height()/def_size-2;
if (max_lines<4) max_lines=4;
//qDebug() << "Height=" << scroll->height() << "SingleHeight=" << ledCoords[0][0]->height()  << "max_lines=" << max_lines;
    if (nr_of_vis_lines<1)
    scrollBar->setRange(0,0);
    else
    scrollBar->setRange(0,nr_of_vis_lines-(max_lines));//scrollBar->setRange(0,nr_of_vis_lines-(max_lines-1));
Empty->setMaximumWidth(scroll->width()-4);

//qDebug() << "Before Region Changed: in_definition=" << in_definition;
if (in_definition!=1)
regionChanged(selRegion->currentIndex());
//qDebug() << "After Region Changed";
Empty->adjustSize();
running=false;
}

void frmMasterRegionOperator_Edit::regionChanged(int re)
{
static bool is_running=false;
if (is_running==true) return;
is_running=true;
if (cur_region>=0 && cur_region<MAXREGION && ((bool)is_region_defined(rg+cur_region))==false) rg[cur_region].active=FALSE;
cur_region=re;
char dummy23[GR_MAXPATHLEN];
QString dsp_text;
bool is_defined;
//int minWidth=80,minHeight=25;//,curWidth,curHeight;
//int maxHeight=qApp->font().pixelSize()*1.5;
is_defined=(bool)is_region_defined(rg+re);
this->blockSignals(true);

regType->setCurrentValue(rg[re].type);

dsp_text=tr("Points: ");
if (is_defined==false && in_definition!=re)
{
dsp_text+=QString::number(0);
}
else
{
    if (rg[re].type==REGION_POLYI || rg[re].type==REGION_POLYO)
    {
    dsp_text+=QString::number(rg[re].n);
    }
    else
    {
    dsp_text+=QString::number(2);
    }
}
lblPoints->setText(dsp_text);

if (is_defined==false && in_definition==re)//we currently want to generate a new region
{
    if (rg[re].type==REGION_POLYI || rg[re].type==REGION_POLYO)
    {
    nr_of_lines=nr_of_vis_lines=4;
    }
    else
    {
    nr_of_lines=nr_of_vis_lines=2;
    }
}

//qDebug() << "regionChanged: max_lines=" << max_lines;
reallocateLines();
//qDebug() << "afterReallocate";

/*if (nr_of_lines>0)
{
    for (int i=0;i<nr_of_lines;i++)
    {
    layout0->removeWidget(lblCoords[i]);
    layout0->removeWidget(ledCoords[0][i]);
    layout0->removeWidget(ledCoords[1][i]);
    layout0->removeWidget(cmdEdit[0][i]);
    layout0->removeWidget(cmdEdit[1][i]);
    delete lblCoords[i];
    delete ledCoords[0][i];
    delete ledCoords[1][i];
    delete cmdEdit[0][i];
    delete cmdEdit[1][i];
    }
delete[] lblCoords;
delete[] ledCoords[0];
delete[] ledCoords[1];
delete[] cmdEdit[0];
delete[] cmdEdit[1];
    ledCoords[0]=NULL;
    ledCoords[1]=NULL;
    cmdEdit[0]=NULL;
    cmdEdit[1]=NULL;
    lblCoords=NULL;
    nr_of_lines=0;
}*/
    //if (rg[re].active==false) return;
    //if (rg[re].x1==0.0 && rg[re].x2==0.0 && rg[re].y1==0.0 && rg[re].y2==0.0) is_defined=false;
    //else is_defined=true;
//qDebug() << "Region-Type=" << rg[re].type << "defined=" << is_defined << "sformat=" << sformat;

if (is_defined==true || in_definition==re)
{
in_definition=re;
nr_of_vis_lines=2;
    if (rg[re].type==REGION_POLYI || rg[re].type==REGION_POLYO)
    {
    nr_of_vis_lines=rg[re].n;
    }

    for (int i=0;i<nr_of_lines;i++)
    {
        if (lblCoords[i]==NULL) lblCoords[i]=new QLabel(QString::number(i)+QString(":"),Empty);
        if (rg[re].type==REGION_POLYI || rg[re].type==REGION_POLYO)
        {
        sprintf(dummy23,"%g",rg[re].x[i]);
        dsp_text=QString(dummy23);
        if (DecimalPointToUse!='.') dsp_text.replace(QString("."),QString(DecimalPointToUse));
        if (ledCoords[0][i]==NULL) ledCoords[0][i]=new QLineEdit(dsp_text,Empty);
        sprintf(dummy23,"%g",rg[re].y[i]);
        dsp_text=QString(dummy23);
        if (DecimalPointToUse!='.') dsp_text.replace(QString("."),QString(DecimalPointToUse));
        if (ledCoords[1][i]==NULL) ledCoords[1][i]=new QLineEdit(dsp_text,Empty);
        //ledCoords[0][i]=new QLineEdit(QString::number(rg[re].x[i]),Empty);
        //ledCoords[1][i]=new QLineEdit(QString::number(rg[re].y[i]),Empty);
        }
        else
        {
            if (i==0)
            {
            sprintf(dummy23,"%g",rg[re].x1);
            dsp_text=QString(dummy23);
            if (DecimalPointToUse!='.') dsp_text.replace(QString("."),QString(DecimalPointToUse));
            if (ledCoords[0][i]==NULL) ledCoords[0][i]=new QLineEdit(dsp_text,Empty);
            sprintf(dummy23,"%g",rg[re].y1);
            dsp_text=QString(dummy23);
            if (DecimalPointToUse!='.') dsp_text.replace(QString("."),QString(DecimalPointToUse));
            if (ledCoords[1][i]==NULL) ledCoords[1][i]=new QLineEdit(dsp_text,Empty);
            }
            else
            {
            sprintf(dummy23,"%g",rg[re].x2);
            dsp_text=QString(dummy23);
            if (DecimalPointToUse!='.') dsp_text.replace(QString("."),QString(DecimalPointToUse));
            if (ledCoords[0][i]==NULL) ledCoords[0][i]=new QLineEdit(dsp_text,Empty);
            sprintf(dummy23,"%g",rg[re].y2);
            dsp_text=QString(dummy23);
            if (DecimalPointToUse!='.') dsp_text.replace(QString("."),QString(DecimalPointToUse));
            if (ledCoords[1][i]==NULL) ledCoords[1][i]=new QLineEdit(dsp_text,Empty);
            }
        }
        if (i>=max_lines)
        {
        lblCoords[i]->hide();
        ledCoords[0][i]->hide();
        ledCoords[1][i]->hide();
        }
        else
        {
        lblCoords[i]->show();
        ledCoords[0][i]->show();
        ledCoords[1][i]->show();
        layout0->addWidget(lblCoords[i],1+i,0);
        layout0->addWidget(ledCoords[0][i],1+i,1);
        layout0->addWidget(ledCoords[1][i],1+i,2);
        }
//qDebug() << "recreating buttons " << i;
    if (cmdEdit[0][i]==NULL) cmdEdit[0][i]=new QPushButton(QString("+"),Empty);
    cmdEdit[0][i]->setToolTip(tr("Add a point after this one"));
    if (cmdEdit[1][i]==NULL) cmdEdit[1][i]=new QPushButton(QString("-"),Empty);
    cmdEdit[1][i]->setToolTip(tr("Remove this point"));
    cmdEdit[0][i]->disconnect();
    cmdEdit[1][i]->disconnect();
#if QT_VERSION >= 0x050000
    connect(cmdEdit[0][i],&QPushButton::clicked,this,[this,i](){this->doAdd(i);});
    connect(cmdEdit[1][i],&QPushButton::clicked,this,[this,i](){this->doRemove(i);});
#else
    connect(cmdEdit[0][i], SIGNAL(clicked()), plusMapper, SLOT(map()));
    connect(cmdEdit[1][i], SIGNAL(clicked()), minusMapper, SLOT(map()));
#endif
    if (!(rg[re].type==REGION_POLYI || rg[re].type==REGION_POLYO))
    {
    cmdEdit[0][i]->setEnabled(false);
    cmdEdit[1][i]->setEnabled(false);
    }
    else
    {
    cmdEdit[0][i]->setEnabled(true);
    cmdEdit[1][i]->setEnabled(true);
    }
        if (i>=max_lines)
        {
//qDebug() << "hiding additional buttons: " << i << "maxlines=" << max_lines << "nr_of_lines=" << nr_of_lines;
        cmdEdit[0][i]->hide();
        cmdEdit[1][i]->hide();
        }
        else
        {
//qDebug() << "NOT hiding additional buttons: " << i << "maxlines=" << max_lines << "nr_of_lines=" << nr_of_lines;
#if QT_VERSION >= 0x050000

#else
    plusMapper->setMapping(cmdEdit[0][i],i);
    minusMapper->setMapping(cmdEdit[1][i],i);
#endif
        cmdEdit[0][i]->show();
        cmdEdit[1][i]->show();
        layout0->addWidget(cmdEdit[0][i],1+i,3);
        layout0->addWidget(cmdEdit[1][i],1+i,4);
        }
    }
    layout0->addWidget(cmdClearRegion,nr_of_lines+1,0,1,5);

    /*if (lblTitle[0]->width()<minWidth)
    curWidth=5*minWidth*1.05;
    else
    curWidth=5*(lblTitle[0]->width()+layout0->spacing())*1.05;

    if (regType->height()<minHeight)
    curHeight=minHeight*(nr_of_lines+2)*1.3;
    else if (regType->height()>maxHeight)
    curHeight=maxHeight*(nr_of_lines+2)*1.3;
    else
    curHeight=(regType->height()+layout0->spacing())*(nr_of_lines+2)*1.3;
qDebug() << "defined/indefinition: curWi=" << curWidth << "curHei=" << curHeight;
    Empty->resize(curWidth,curHeight);
qDebug() << "After Resize";*/

    Empty->setMaximumWidth(scroll->width()-4);
    Empty->adjustSize();

    cmdClearRegion->setText(tr("Clear Region"));
    cmdClearRegion->setToolTip(tr("Remove all settings from this region.\nDeletes all limiting points of the region."));
    cmdClearRegion->setEnabled(true);
}
else
{
nr_of_lines=0;
in_definition=-1;
    layout0->addWidget(cmdClearRegion,0,0,1,5);

    /*curWidth=minWidth*1.55;
    if (regType->height()<minHeight)
    curHeight=minHeight*(nr_of_lines+1)*1.6;
    else if (regType->height()>maxHeight)
    curHeight=maxHeight*(nr_of_lines+1)*1.6;
    else
    curHeight=(regType->height())*(nr_of_lines+1)*1.6;
qDebug() << "else: curWi=" << curWidth << "curHei=" << curHeight;
    Empty->resize(curWidth,curHeight);*/

    Empty->setMaximumWidth(scroll->width()-4);
    Empty->adjustSize();

    cmdClearRegion->setText(tr("Create Region"));
    cmdClearRegion->setToolTip(tr("Create a new region."));
    cmdClearRegion->setEnabled(true);

    for (int i=0;i<max_lines;i++)
    {
    lblCoords[i]->hide();
    ledCoords[0][i]->hide();
    ledCoords[1][i]->hide();
    cmdEdit[0][i]->hide();
    cmdEdit[1][i]->hide();
    }
}
for (int i=nr_of_vis_lines;i<nr_of_lines;i++)
{
lblCoords[i]->hide();
ledCoords[0][i]->hide();
ledCoords[1][i]->hide();
cmdEdit[0][i]->hide();
cmdEdit[1][i]->hide();
}
    //cmdClearRegion->setEnabled(is_defined);
displayRegionPoints();

if (nr_of_vis_lines<1)
scrollBar->setRange(0,0);
else
scrollBar->setRange(0,nr_of_vis_lines-(max_lines));//scrollBar->setRange(0,nr_of_vis_lines-(max_lines-1));

Empty->adjustSize();
this->blockSignals(false);
//qDebug() << "Size= " << Empty->width() << " x " << Empty->height() << endl;
is_running=false;
}

void frmMasterRegionOperator_Edit::regionTypeChanged(int re)
{
static bool running=false;
if (running==true || in_definition==-1) return;
running=true;
char com_dummy[128];
    (void)re;
disconnect(regType->cmbSelect,0,0,0);
int reg=selRegion->currentValue();
int type=regType->currentValue();
ListOfOldStates.clear();
ListOfChanges.clear();
region_type_string(re,com_dummy);
ListOfOldStates << com_dummy;
CopyRegion(rg+re,&undo_region);
change_region_type(reg,type);
region_type_string(re,com_dummy);
ListOfChanges << com_dummy;
RegionModified(re,rg+re,0);
emit(initWish());
qApp->processEvents();
mainWin->mainArea->completeRedraw();
connect(regType->cmbSelect,SIGNAL(activated(int)),SLOT(regionTypeChanged(int)));
running=false;
}

void frmMasterRegionOperator_Edit::doApply(void)
{
double d_tmp;
int re=selRegion->currentIndex();
bool is_defined;
if (in_definition==-1) return;//nothing activated --> do nothing

//if (rg[re].active==false) return;
if (rg[re].type==0 && rg[re].x1==0.0 && rg[re].x2==0.0 && rg[re].y1==0.0 && rg[re].y2==0.0)
is_defined=false;
else
is_defined=true;
CopyRegion(rg+re,&undo_region);
    if (rg[re].type==REGION_POLYI || rg[re].type==REGION_POLYO)
    {
        for (int i=0;i<nr_of_lines;i++)
        {
        xv_evalexpr(ledCoords[0][i],&d_tmp);
        rg[re].x[i]=d_tmp;
        xv_evalexpr(ledCoords[1][i],&d_tmp);
        rg[re].y[i]=d_tmp;
        }
    }
    else
    {
    //strcpy(dummy,ledCoords[0][0]->text().toLocal8Bit().constData());
    xv_evalexpr(ledCoords[0][0],&d_tmp);
    rg[re].x1=d_tmp;
    xv_evalexpr(ledCoords[1][0],&d_tmp);
    rg[re].y1=d_tmp;
    xv_evalexpr(ledCoords[0][1],&d_tmp);
    rg[re].x2=d_tmp;
    xv_evalexpr(ledCoords[1][1],&d_tmp);
    rg[re].y2=d_tmp;
    }
RegionModified(re,rg+re,0);
mainWin->mainArea->completeRedraw();
}

void frmMasterRegionOperator_Edit::doAccept(void)
{
ApplyError=false;
doApply();
    if (ApplyError==false)
    doClose();
}

void frmMasterRegionOperator_Edit::doClose(void)
{
emit(closeWish());
}

void frmMasterRegionOperator_Edit::doAdd(int l)
{
char dummy[128];
QString error_m;
int re=selRegion->currentIndex();
//qDebug() << "Clicked line:" << l;
//qDebug() << "Add after line:" << l+start_line;
CopyRegion(rg+re,&undo_region);
int ret_val=add_point_to_region(re,l+start_line,rg[re].x[l+start_line],rg[re].y[l+start_line]);
    if (ret_val==RETURN_FAILURE)
    {
    error_m=tr("Unable to add a point to selected region.");
    strcpy(dummy,error_m.toLocal8Bit().constData());
    errmsg(dummy);
    }
    else
    {
    RegionModified(re,rg+re,0);
    regionChanged(selRegion->currentIndex());
    mainWin->mainArea->completeRedraw();
    }
}

void frmMasterRegionOperator_Edit::doRemove(int l)
{
char dummy[128];
QString error_m;
int re=selRegion->currentIndex();
//qDebug() << "Clicked line:" << l;
//qDebug() << "Remove line:" << l+start_line;
CopyRegion(rg+re,&undo_region);
int ret_val=remove_point_from_region(re,l+start_line);
    if (ret_val==RETURN_FAILURE)
    {
    error_m=tr("Unable to remove a point from selected region.");
    strcpy(dummy,error_m.toLocal8Bit().constData());
    errmsg(dummy);
    }
    else
    {
    RegionModified(re,rg+re,0);
    regionChanged(selRegion->currentIndex());
    mainWin->mainArea->completeRedraw();
    }
}

void frmMasterRegionOperator_Edit::doClearRegion(void)
{
int re=selRegion->currentIndex();
    if (in_definition==-1)//not active
    {
    //qDebug() << "Setting Region to be editable";
    in_definition=re;
    regiontype=rg[re].type=regType->currentValue();
        if (rg[re].x==NULL || rg[re].y==NULL)
        {
        WPoint *wps=new WPoint[4];
        wps[0].x=0.0;
        wps[0].y=0.0;
        wps[1]=wps[2]=wps[3]=wps[0];
        load_poly_region(re,get_cg(),4,wps);
        delete[] wps;
        }
        else
        {
        rg[re].active=TRUE;
        }
    }
    else
    {
    //qDebug() << "Delete complete region";
    int ret=QMessageBox::question(this,tr("Clear region R")+QString::number(re),tr("Do you really want to clear region R")+QString::number(re)+QString("?"),QMessageBox::Yes,QMessageBox::No);
        if (ret==QMessageBox::Yes)
        {
        RegionModified(re,NULL,1);
        kill_region(re);
        in_definition=-1;
        mainWin->mainArea->completeRedraw();
        }
    }
regionChanged(selRegion->currentIndex());
}

void frmMasterRegionOperator_Edit::linesScolled(int pos)
{
//qDebug() << "scrolling to line" << pos;
start_line=pos;
displayRegionPoints();
}

void frmMasterRegionOperator_Edit::reallocateLines(void)
{
    QLabel ** tmpLabels;
    QLineEdit ** tmpLineEdits;
    QPushButton ** tmpPushButtons;
//qDebug() << "reallocateLines: max_lines=" << max_lines << "nr_of_lines=" << nr_of_lines << "nr_of_vis_line=" << nr_of_vis_lines;
if (max_lines>nr_of_lines)//not enough data lines, generate memory for them
{
layout0->removeWidget(cmdClearRegion);
tmpLabels=new QLabel*[max_lines];
for (int i=0;i<nr_of_lines;i++) tmpLabels[i]=lblCoords[i];
for (int i=nr_of_lines;i<max_lines;i++) tmpLabels[i]=new QLabel(QString(""),Empty);
delete[] lblCoords;
lblCoords=tmpLabels;
for (int j=0;j<2;j++)
{
tmpLineEdits=new QLineEdit*[max_lines];
for (int i=0;i<nr_of_lines;i++) tmpLineEdits[i]=ledCoords[j][i];
for (int i=nr_of_lines;i<max_lines;i++) tmpLineEdits[i]=new QLineEdit(QString(""),Empty);
delete[] ledCoords[j];
ledCoords[j]=tmpLineEdits;
tmpPushButtons=new QPushButton*[max_lines];
for (int i=0;i<nr_of_lines;i++) tmpPushButtons[i]=cmdEdit[j][i];
    if (j==0)
    for (int i=nr_of_lines;i<max_lines;i++) tmpPushButtons[i]=new QPushButton(QString("+"),Empty);
    else
    for (int i=nr_of_lines;i<max_lines;i++) tmpPushButtons[i]=new QPushButton(QString("-"),Empty);
delete[] cmdEdit[j];
cmdEdit[j]=tmpPushButtons;
}
for (int i=nr_of_lines;i<max_lines;i++)
{
    cmdEdit[0][i]->disconnect();
    cmdEdit[1][i]->disconnect();
#if QT_VERSION >= 0x050000
    connect(cmdEdit[0][i],&QPushButton::clicked,this,[this,i](){this->doAdd(i);});
    connect(cmdEdit[1][i],&QPushButton::clicked,this,[this,i](){this->doRemove(i);});
#else
    connect(cmdEdit[0][i], SIGNAL(clicked()), plusMapper, SLOT(map()));
    connect(cmdEdit[1][i], SIGNAL(clicked()), minusMapper, SLOT(map()));
    plusMapper->setMapping(cmdEdit[0][i],i);
    minusMapper->setMapping(cmdEdit[1][i],i);
#endif
//now we have to arrange the lines in the grid
    layout0->addWidget(lblCoords[i],1+i,0);
    layout0->addWidget(ledCoords[0][i],1+i,1);
    layout0->addWidget(ledCoords[1][i],1+i,2);
    layout0->addWidget(cmdEdit[0][i],1+i,3);
    layout0->addWidget(cmdEdit[1][i],1+i,4);
}
nr_of_lines=max_lines;
layout0->addWidget(cmdClearRegion,nr_of_lines+1,0,1,5);
}
else
{
    for (int i=max_lines;i<nr_of_lines;i++)
    {
    layout0->removeWidget(lblCoords[i]);
    layout0->removeWidget(ledCoords[0][i]);
    layout0->removeWidget(ledCoords[1][i]);
    layout0->removeWidget(cmdEdit[0][i]);
    layout0->removeWidget(cmdEdit[1][i]);
    lblCoords[i]->hide();
    ledCoords[0][i]->hide();
    ledCoords[1][i]->hide();
//qDebug() << "hide lines: i=" << i;
    cmdEdit[0][i]->hide();
    cmdEdit[1][i]->hide();
    }
}
Empty->adjustSize();
//else: nothing to do here!
}

void frmMasterRegionOperator_Edit::displayRegionPoints(void)
{
//qDebug() << "nr_of_lines=" << nr_of_lines << "nr_of_vis_lines=" << nr_of_vis_lines << "start_line=" << start_line << "max_lines=" << max_lines;
QString dsp_text;
char dummy23[1024];
int re=cur_region;

for (int i=start_line;i<start_line+nr_of_lines;i++)
{
lblCoords[i-start_line]->setText(QString::number(i)+QString(":"));
    if (rg[re].type==REGION_POLYI || rg[re].type==REGION_POLYO)
    {
    sprintf(dummy23,"%g",rg[re].x[i]);
    dsp_text=QString(dummy23);
    if (DecimalPointToUse!='.') dsp_text.replace(QString("."),QString(DecimalPointToUse));
ledCoords[0][i-start_line]->setText(dsp_text);
    sprintf(dummy23,"%g",rg[re].y[i]);
    dsp_text=QString(dummy23);
    if (DecimalPointToUse!='.') dsp_text.replace(QString("."),QString(DecimalPointToUse));
ledCoords[1][i-start_line]->setText(dsp_text);
    //ledCoords[0][i]=new QLineEdit(QString::number(rg[re].x[i]),Empty);
    //ledCoords[1][i]=new QLineEdit(QString::number(rg[re].y[i]),Empty);
    }
    else
    {
        if (i==0)
        {
        sprintf(dummy23,"%g",rg[re].x1);
        dsp_text=QString(dummy23);
        if (DecimalPointToUse!='.') dsp_text.replace(QString("."),QString(DecimalPointToUse));
ledCoords[0][i-start_line]->setText(dsp_text);
        sprintf(dummy23,"%g",rg[re].y1);
        dsp_text=QString(dummy23);
        if (DecimalPointToUse!='.') dsp_text.replace(QString("."),QString(DecimalPointToUse));
ledCoords[1][i-start_line]->setText(dsp_text);
        }
        else
        {
        sprintf(dummy23,"%g",rg[re].x2);
        dsp_text=QString(dummy23);
        if (DecimalPointToUse!='.') dsp_text.replace(QString("."),QString(DecimalPointToUse));
ledCoords[0][i-start_line]->setText(dsp_text);
        sprintf(dummy23,"%g",rg[re].y2);
        dsp_text=QString(dummy23);
        if (DecimalPointToUse!='.') dsp_text.replace(QString("."),QString(DecimalPointToUse));
ledCoords[1][i-start_line]->setText(dsp_text);
        }
    }
}
for (int i=0;i<nr_of_lines;i++)
{
ledCoords[0][i]->disconnect();
ledCoords[1][i]->disconnect();
connect(ledCoords[0][i],SIGNAL(returnPressed()),this,SLOT(doApply()));
connect(ledCoords[1][i],SIGNAL(returnPressed()),this,SLOT(doApply()));
}

}

frmMasterRegionOperator_Operations::frmMasterRegionOperator_Operations(QWidget * parent):QWidget(parent)
{
int line=0;
layout=new QGridLayout();
//layout->setMargin(STD_MARGIN);
layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
layout->setSpacing(STD_SPACING);

lblGraph=new QLabel(tr("Source graph:"));
lblSet=new QLabel(tr("Source set:"));

graphList=new uniList(GRAPHLIST,this);
graphList->setBehavior(true,true,true);
setList=new uniList(SETLIST,this);
setList->setBehavior(true,true,true);
connect(graphList,SIGNAL(new_selection(int)),SLOT(newGraphSelection(int)));

int number=7;
QString entr[7];
entr[0]=tr("None");
entr[1]=tr("Region 0");
entr[2]=tr("Region 1");
entr[3]=tr("Region 2");
entr[4]=tr("Region 3");
entr[5]=tr("Region 4");
entr[6]=tr("Inside graph");
selRestriction=new StdSelector(this,tr("Restriction:"),number,entr);

chkNegRes=new QCheckBox(tr("Negated"),this);
chkNegRes->setChecked(FALSE);
number=3;
entr[0]=tr("No new set(s)");
entr[1]=tr("All in one new set");
entr[2]=tr("Several new set(s)");
selNewSets=new StdSelector(this,tr("New set(s):"),number,entr);

number=4;
entr[0]=tr("Delete sets in region");
entr[1]=tr("Delete points in region");
entr[2]=tr("Extract sets in region");
entr[3]=tr("Extract points in region");
selOperation=new StdSelector(this,tr("Operation:"),number,entr);
connect(selOperation->cmbSelect,SIGNAL(activated(int)),SLOT(OperationChanged(int)));

number=3;
entr[0]=tr("Current graph");
entr[1]=tr("Source graph");
entr[2]=tr("G0");
selTargetGraph=new StdSelector(this,tr("Target graph:"),number,entr);

cmdButtons=new stdButtonGroup(this);
connect(cmdButtons->cmdApply,SIGNAL(clicked()),SLOT(doApply()));
connect(cmdButtons->cmdAccept,SIGNAL(clicked()),SLOT(doAccept()));
connect(cmdButtons->cmdClose,SIGNAL(clicked()),SLOT(doClose()));

layout->addWidget(lblGraph,line,0,1,1);
layout->addWidget(lblSet,line++,1,1,1);
layout->addWidget(graphList,line,0,1,1);
layout->addWidget(setList,line++,1,1,1);
layout->addWidget(selOperation,line++,0,1,2);
layout->addWidget(selRestriction,line,0,1,1);
layout->addWidget(chkNegRes,line++,1,1,1);
layout->addWidget(selTargetGraph,line,0,1,1);
layout->addWidget(selNewSets,line++,1,1,1);
layout->addWidget(cmdButtons,line++,0,1,2);
setLayout(layout);
OperationChanged(0);
}

void frmMasterRegionOperator_Operations::init(void)
{
graphList->update_number_of_entries_preserve_selection();
setList->update_number_of_entries_preserve_selection();
int cur_gr[2]={cg,cg};
graphList->set_new_selection(1,cur_gr);
int number=number_of_graphs()+2;
QString * entries=new QString[number+2];
entries[0]=tr("Current graph");
entries[1]=tr("Source graph");
for (int i=0;i<number_of_graphs();i++)
{
entries[i+2]=QString("G")+QString::number(i);
}
selTargetGraph->setNewEntries(number,entries);
delete[] entries;
}

void frmMasterRegionOperator_Operations::newGraphSelection(int r)
{
    if (r-1<0)
    setList->set_graph_number(cg,true);
    else
    setList->set_graph_number(r-1,true);
}

void frmMasterRegionOperator_Operations::OperationChanged(int op)
{
int number=0;
QString entries[5];
    switch (op)
    {
    case 0://Delete sets in Region
    selTargetGraph->setVisible(false);
    selNewSets->setVisible(false);
    break;
    case 1://Delete points in Region
    selTargetGraph->setVisible(true);
    selNewSets->setVisible(true);
    number=2;
    entries[0]=tr("Operate on source set(s)");
    entries[1]=tr("Create new set(s)");
    selNewSets->setNewEntries(number,entries);
    break;
    case 2://Extract sets in Region
    selTargetGraph->setVisible(true);
    selNewSets->setVisible(true);
    number=3;
    entries[0]=tr("Copy set(s)");
    entries[1]=tr("Copy data into one new set");
    entries[2]=tr("Move set(s)");
    selNewSets->setNewEntries(number,entries);
    break;
    case 3://Extract points in Region
    selTargetGraph->setVisible(true);
    selNewSets->setVisible(true);
    number=4;
    entries[0]=tr("Copy points into new set(s)");
    entries[1]=tr("Copy points into one new set");
    entries[2]=tr("Move points into new set(s)");
    entries[3]=tr("Move points into one new set");
    selNewSets->setNewEntries(number,entries);
    break;
    }
}

void frmMasterRegionOperator_Operations::doApply(void)
{
int reg=RESTRICT_NONE;
    switch (selRestriction->currentIndex())
    {
    case 0:
    reg=RESTRICT_NONE;
    break;
    case 1:
    reg=RESTRICT_REG0;
    break;
    case 2:
    reg=RESTRICT_REG1;
    break;
    case 3:
    reg=RESTRICT_REG2;
    break;
    case 4:
    reg=RESTRICT_REG3;
    break;
    case 5:
    reg=RESTRICT_REG4;
    break;
    case 6:
    reg=RESTRICT_WORLD;
    break;
    }
    if (reg==RESTRICT_NONE)
    {
    errmsg(tr("Please select a region to operate with!").toLocal8Bit().data());
    return;
    }

int negate=(chkNegRes->isChecked()==true)?1:0;
int n_set_type=selNewSets->currentIndex();
int op=selOperation->currentIndex();
int target=selTargetGraph->currentIndex();
int tmp_target_graph,tmp_new_set;
int * g_sel=NULL,* s_sel=NULL;
int n_g_sel,n_s_sel;
bool allGraphsSelected,allSetsSelected;
graphList->get_selection(&n_g_sel,&g_sel);
setList->get_selection(&n_s_sel,&s_sel);
allGraphsSelected=graphList->all_entries_option_selected;
allSetsSelected=setList->all_entries_option_selected;
/*
    for (int i=0;i<n_g_sel;i++)
    {
    cout << "Graph: G" << g_sel[i] << endl;
    }
    cout << "all Graph entries=" << allGraphsSelected << endl;
    for (int i=0;i<n_s_sel;i++)
    {
    cout << "Set: S" << s_sel[i] << endl;
    }
    cout << "all Set entries=" << allSetsSelected << endl;
*/
int * source_graphs1=NULL;//just the graphs
int n_source_graphs1=0;

int * source_graphs=NULL;//source graphs and sets
int n_source_graphs=0;
int * source_sets=NULL;
int n_source_sets=0;
int * target_graphs=NULL;//target graphs and sets
int n_target_graphs=0;
int * target_sets=NULL;
int n_target_sets=0;

//first: determine all source sets that are part of the region
if (allGraphsSelected==true)
{
n_source_graphs1=number_of_graphs();
    source_graphs1=new int[n_source_graphs1];
    for (int i=0;i<n_source_graphs1;i++)
    {
    source_graphs1[i]=i;
    }
}
else
{
n_source_graphs1=n_g_sel;
    source_graphs1=new int[n_source_graphs1];
    for (int i=0;i<n_source_graphs1;i++)
    {
    source_graphs1[i]=g_sel[i];
    }
}
int contained,tmp_nr;
for (int i=0;i<n_source_graphs1;i++)
{
    if (allSetsSelected==true)
    {
        for (int j=0;j<number_of_sets(source_graphs1[i]);j++)//go through all sets in the graph --> j=set-id
        {
        contained=is_set_in_region(source_graphs1[i],j,reg,negate);
            if (contained==TRUE)
            {
            tmp_nr=source_graphs1[i];
            append_to_storage(&n_source_graphs,&source_graphs,1,&tmp_nr);
            tmp_nr=j;
            append_to_storage(&n_source_sets,&source_sets,1,&tmp_nr);
            }
        }
    }
    else
    {
        for (int j=0;j<n_s_sel;j++)//go through all selected sets in the current graph --> s_sel[j]=set-id
        {
        contained=is_set_in_region(source_graphs1[i],s_sel[j],reg,negate);
            if (contained==TRUE)
            {
            tmp_nr=source_graphs1[i];
            append_to_storage(&n_source_graphs,&source_graphs,1,&tmp_nr);
            tmp_nr=s_sel[j];
            append_to_storage(&n_source_sets,&source_sets,1,&tmp_nr);
            }
        }
    }
}

if (n_source_graphs>0)
{
/*cout << "SETS to operate on:" << endl;
for (int i=0;i<n_source_graphs;i++)
{
cout << "G" << source_graphs[i] << ".S" << source_sets[i] << endl;
}*/
ListOfChanges.clear();
//second: do the actual changes
    switch (op)
    {
    case 0://Delete sets in Region (the whole sets)
        for (int i=0;i<n_source_graphs;i++)
        {
            sprintf(dummy,"kill G%d.S%d",source_graphs[i],source_sets[i]);
            ListOfChanges << QString(dummy);
        }
        SetsDeleted(n_source_graphs,source_graphs,source_sets,UNDO_COMPLETE);//UndoStuff
        for (int i=0;i<n_source_graphs;i++)
        {
            killset(source_graphs[i],source_sets[i]);
        }
    break;
    case 2://Extract sets in Region (the whole sets)
        if (n_set_type==0)//"Copy set(s)"
        {
            for (int i=0;i<n_source_graphs;i++)
            {
                if (target==0) tmp_target_graph=cg;
                else if (target==1) tmp_target_graph=source_graphs[i];
                else tmp_target_graph=target-2;
                tmp_new_set=nextset(tmp_target_graph);
                do_copyset(source_graphs[i],source_sets[i],tmp_target_graph,tmp_new_set);
            append_to_storage(&n_target_graphs,&target_graphs,1,&tmp_target_graph);
            append_to_storage(&n_target_sets,&target_sets,1,&tmp_new_set);
            }
        if (n_target_graphs>0)
        SetsCreated(n_target_graphs,target_graphs,target_sets,UNDO_COMPLETE);
        }
        else if (n_set_type==1)//"Copy data into one new set"
        {
            if (target==0) tmp_target_graph=cg;
            else if (target==1) tmp_target_graph=source_graphs[0];
            else tmp_target_graph=target-2;
            tmp_new_set=nextset(tmp_target_graph);
            do_copyset(source_graphs[0],source_sets[0],tmp_target_graph,tmp_new_set);
            append_to_storage(&n_target_graphs,&target_graphs,1,&tmp_target_graph);
            append_to_storage(&n_target_sets,&target_sets,1,&tmp_new_set);
            if (source_graphs1!=NULL) delete[] source_graphs1;
            source_graphs1=new int[2];
            source_graphs1[0]=tmp_new_set;
            tmp_new_set=nextset(tmp_target_graph);
            source_graphs1[1]=tmp_new_set;
            for (int i=1;i<n_source_graphs;i++)
            {
                if (target==1 && tmp_target_graph!=source_graphs[i])//a new graph has started, but we want a new set in every graph
                {
                killsetdata(tmp_target_graph,tmp_new_set);
                tmp_target_graph=source_graphs[i];
                tmp_new_set=nextset(tmp_target_graph);
                do_copyset(source_graphs[i],source_sets[i],tmp_target_graph,tmp_new_set);
                append_to_storage(&n_target_graphs,&target_graphs,1,&tmp_target_graph);
                append_to_storage(&n_target_sets,&target_sets,1,&tmp_new_set);
                source_graphs1[0]=tmp_new_set;
                tmp_new_set=nextset(tmp_target_graph);
                source_graphs1[1]=tmp_new_set;
                continue;
                }
            do_copyset(source_graphs[i],source_sets[i],tmp_target_graph,tmp_new_set);
            join_sets(tmp_target_graph,source_graphs1,2);
            }
            killsetdata(tmp_target_graph,tmp_new_set);
        SetsCreated(n_target_graphs,target_graphs,target_sets,UNDO_COMPLETE);
        }
        else//"Move set(s)"
        {
        //first: copy sets
            for (int i=0;i<n_source_graphs;i++)
            {
                if (target==0) tmp_target_graph=cg;
                else if (target==1) tmp_target_graph=source_graphs[i];
                else tmp_target_graph=target-2;
                tmp_new_set=nextset(tmp_target_graph);
                do_copyset(source_graphs[i],source_sets[i],tmp_target_graph,tmp_new_set);
            append_to_storage(&n_target_graphs,&target_graphs,1,&tmp_target_graph);
            append_to_storage(&n_target_sets,&target_sets,1,&tmp_new_set);
            }
        if (n_target_graphs>0)
        SetsCreated(n_target_graphs,target_graphs,target_sets,UNDO_COMPLETE);
        //then: delete old sets
        SetsDeleted(n_source_graphs,source_graphs,source_sets,UNDO_COMPLETE);
            for (int i=0;i<n_source_graphs;i++)
            {
            killset(source_graphs[i],source_sets[i]);
            }
        }
    break;
    case 1://Delete points in Region
        //n_set_type==0 --> tr("Operate on source set(s)");
        //n_set_type==1 --> tr("Create new set(s)");
        if (n_set_type==0)//"Operate on source set(s)"
        {
        SaveSetStatesPrevious(n_source_graphs,source_graphs,source_sets,UNDO_DATA);
            for (int i=0;i<n_source_graphs;i++)
            {
            restrict_set_to_region(source_graphs[i],source_sets[i],reg,!negate);//!negate because we restrict the sets to the points outside the region (delete inside)
            }
        SetsModified(n_source_graphs,source_graphs,source_sets,UNDO_DATA);
        }
        else//"Create new set(s)"
        {
            for (int i=0;i<n_source_graphs;i++)
            {
                if (target==0) tmp_target_graph=cg;
                else if (target==1) tmp_target_graph=source_graphs[i];
                else tmp_target_graph=target-2;
                tmp_new_set=nextset(tmp_target_graph);
                do_copyset(source_graphs[i],source_sets[i],tmp_target_graph,tmp_new_set);
                restrict_set_to_region(tmp_target_graph,tmp_new_set,reg,!negate);
            append_to_storage(&n_target_graphs,&target_graphs,1,&tmp_target_graph);
            append_to_storage(&n_target_sets,&target_sets,1,&tmp_new_set);
            }
        if (n_target_graphs>0)
        SetsCreated(n_target_graphs,target_graphs,target_sets,UNDO_COMPLETE);
        }
    break;
    case 3://Extract points in Region
        if (n_set_type==0 || n_set_type==2)//"Copy points into new set(s)"
        {
            for (int i=0;i<n_source_graphs;i++)
            {
                if (target==0) tmp_target_graph=cg;
                else if (target==1) tmp_target_graph=source_graphs[i];
                else tmp_target_graph=target-2;
                tmp_new_set=nextset(tmp_target_graph);
                do_copyset(source_graphs[i],source_sets[i],tmp_target_graph,tmp_new_set);
                restrict_set_to_region(tmp_target_graph,tmp_new_set,reg,negate);//only take points inside region
            append_to_storage(&n_target_graphs,&target_graphs,1,&tmp_target_graph);
            append_to_storage(&n_target_sets,&target_sets,1,&tmp_new_set);
            }
        if (n_target_graphs>0)
        SetsCreated(n_target_graphs,target_graphs,target_sets,UNDO_COMPLETE);
        }
        else if (n_set_type==1 || n_set_type==3)//"Copy points into one new set"
        {
            if (target==0) tmp_target_graph=cg;
            else if (target==1) tmp_target_graph=source_graphs[0];
            else tmp_target_graph=target-2;
            tmp_new_set=nextset(tmp_target_graph);
            do_copyset(source_graphs[0],source_sets[0],tmp_target_graph,tmp_new_set);
            restrict_set_to_region(tmp_target_graph,tmp_new_set,reg,negate);
            append_to_storage(&n_target_graphs,&target_graphs,1,&tmp_target_graph);
            append_to_storage(&n_target_sets,&target_sets,1,&tmp_new_set);
            if (source_graphs1!=NULL) delete[] source_graphs1;
            source_graphs1=new int[2];
            source_graphs1[0]=tmp_new_set;
            tmp_new_set=nextset(tmp_target_graph);
            source_graphs1[1]=tmp_new_set;
            for (int i=1;i<n_source_graphs;i++)
            {
                if (target==1 && tmp_target_graph!=source_graphs[i])//a new graph has started, but we want a new set in every graph
                {
                killsetdata(tmp_target_graph,tmp_new_set);
                tmp_target_graph=source_graphs[i];
                tmp_new_set=nextset(tmp_target_graph);
                do_copyset(source_graphs[i],source_sets[i],tmp_target_graph,tmp_new_set);
                restrict_set_to_region(tmp_target_graph,tmp_new_set,reg,negate);
                append_to_storage(&n_target_graphs,&target_graphs,1,&tmp_target_graph);
                append_to_storage(&n_target_sets,&target_sets,1,&tmp_new_set);
                source_graphs1[0]=tmp_new_set;
                tmp_new_set=nextset(tmp_target_graph);
                source_graphs1[1]=tmp_new_set;
                continue;
                }
                do_copyset(source_graphs[i],source_sets[i],tmp_target_graph,tmp_new_set);
                restrict_set_to_region(tmp_target_graph,tmp_new_set,reg,negate);//only take points inside region
                join_sets(tmp_target_graph,source_graphs1,2);
            }
            killsetdata(tmp_target_graph,tmp_new_set);
        if (n_target_graphs>0)
        SetsCreated(n_target_graphs,target_graphs,target_sets,UNDO_COMPLETE);
        }

        if (n_set_type==2 || n_set_type==4)//"Move points into new set(s)" || "Move points into one new set" --> restrict old sets
        {
        SaveSetStatesPrevious(n_source_graphs,source_graphs,source_sets,UNDO_DATA);
            for (int i=0;i<n_source_graphs;i++)
            {
            restrict_set_to_region(source_graphs[i],source_sets[i],reg,!negate);//!negate because we restrict the sets to the points outside the region (delete inside)
            }
        SetsModified(n_source_graphs,source_graphs,source_sets,UNDO_DATA);
        }
    break;
    }
ListOfChanges.clear();
}
else
{
errmsg(tr("No data inside selected region found!").toLocal8Bit().data());
}
if (g_sel!=NULL) delete[] g_sel;
if (s_sel!=NULL) delete[] s_sel;
if (source_graphs1!=NULL) delete[] source_graphs1;
if (source_graphs!=NULL) delete[] source_graphs;
if (source_sets!=NULL) delete[] source_sets;
if (target_graphs!=NULL) delete[] target_graphs;
if (target_sets!=NULL) delete[] target_sets;
mainWin->mainArea->completeRedraw();
init();
}

void frmMasterRegionOperator_Operations::doAccept(void)
{
ApplyError=false;
doApply();
    if (ApplyError==false)
    doClose();
}

void frmMasterRegionOperator_Operations::doClose(void)
{
emit(closeWish());
}

frmMasterRegionOperator_Create::frmMasterRegionOperator_Create(QWidget * parent):QWidget(parent)
{
sel_gno=new int[2];
sel_sno=new int[2];
    lblGraph=new QLabel(tr("Source graph:"));
    lblSet=new QLabel(tr("Source set:"));
    lblInfo=new QLabel(QString(""),this);
    lblInfo->setVisible(false);
    graphList=new uniList(GRAPHLIST,this);
    graphList->setBehavior(true,false,false);
    setList=new uniList(SETLIST,this);
    setList->setBehavior(true,false,false);
connect(graphList,SIGNAL(new_selection(int)),SLOT(newGraphSelection(int)));
connect(setList,SIGNAL(new_selection(int)),SLOT(newSetSelection(int)));

int number=5;
QString entr[5];
    entr[0]=tr("Region 0");
    entr[1]=tr("Region 1");
    entr[2]=tr("Region 2");
    entr[3]=tr("Region 3");
    entr[4]=tr("Region 4");
selRestriction=new StdSelector(this,tr("Target region:"),number,entr);

number=1;
    entr[0]=tr("Create region from set");
    entr[1]=tr("Region 1");
selOperation=new StdSelector(this,tr("Operation:"),number,entr);

cmdButtons=new stdButtonGroup(this);
layout=new QGridLayout(this);
layout->setSpacing(STD_SPACING);
//layout->setMargin(STD_MARGIN);
layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);

layout->addWidget(lblGraph,0,0,1,1);
layout->addWidget(lblSet,0,1,1,1);
layout->addWidget(graphList,1,0,1,1);
layout->addWidget(setList,1,1,1,1);
layout->addWidget(lblInfo,2,0,1,2);
layout->addWidget(selOperation,3,0,1,2);
layout->addWidget(selRestriction,4,0,1,2);
layout->addWidget(cmdButtons,5,0,1,2);

setLayout(layout);

connect(cmdButtons->cmdApply,SIGNAL(clicked()),SLOT(doApply()));
connect(cmdButtons->cmdAccept,SIGNAL(clicked()),SLOT(doAccept()));
connect(cmdButtons->cmdClose,SIGNAL(clicked()),SLOT(doClose()));
connect(selOperation,SIGNAL(currentIndexChanged(int)),SLOT(operation_changed(int)));
}

void frmMasterRegionOperator_Create::showEvent(QShowEvent * event)
{
    (void)event;
QString entr[6];
int values[6];
int number=1;
entr[0]=tr("Create region from set");
values[0]=-1;
for (int i=0;i<MAXREGION;i++)
{
    if (is_region_defined(rg+i)==TRUE)
    {
    values[number]=i;
    entr[number++]=tr("Copy Region ")+QString::number(i);
    }
}
selOperation->setNewEntries(number,entr,values);
}

void frmMasterRegionOperator_Create::init(void)
{
graphList->update_number_of_entries_preserve_selection();
setList->update_number_of_entries_preserve_selection();
graphList->set_graph_number(get_cg(),false);
}

void frmMasterRegionOperator_Create::read_inputs(void)
{
    graphList->get_selection(&n_sel_gno,&sel_gno);
    setList->get_selection(&n_sel_sno,&sel_sno);
    target_operation=selOperation->currentValue();
    target_region=selRestriction->currentIndex();
}

void frmMasterRegionOperator_Create::newGraphSelection(int r)
{
if (!(this->isVisible())) return;//do nothing if the dialog is not visible anyway
    /*if (r-1<0)
    setList->set_graph_number(cg,true);
    else
    setList->set_graph_number(r-1,true);*/
    if (r<0)
    setList->clear();
    else
    setList->set_graph_number(r,true);
    read_inputs();
/*qDebug() << "Reagion Master1: New Selection";
if (n_sel_gno==1 && n_sel_sno>0)
{
    qDebug() << "Nr Of Selected Graphs=" << n_sel_gno;
        for (int i=0;i<n_sel_gno;i++)
        qDebug() << i << ": " << sel_gno[i];
    qDebug() << "Nr Of Selected Sets=" << n_sel_sno;
        for (int i=0;i<n_sel_sno;i++)
        qDebug() << i << ": " << sel_sno[i];
}
*/
}

void frmMasterRegionOperator_Create::newSetSelection(int r)
{
if (!(this->isVisible())) return;//do nothing if the dialog is not visible anyway
    read_inputs();
/*    qDebug() << "Reagion Master2: New Selection";
    if (n_sel_gno==1 && n_sel_sno>0)
    {
        qDebug() << "Nr Of Selected Graphs=" << n_sel_gno;
            for (int i=0;i<n_sel_gno;i++)
            qDebug() << i << ": " << sel_gno[i];
        qDebug() << "Nr Of Selected Sets=" << n_sel_sno;
            for (int i=0;i<n_sel_sno;i++)
            qDebug() << i << ": " << sel_sno[i];
    }
*/
    (void) r;
}

void frmMasterRegionOperator_Create::operation_changed(int r)
{
if (r==0)//create from set
{
graphList->setEnabled(true);
setList->setEnabled(true);
}
else//copy region
{
graphList->setEnabled(false);
setList->setEnabled(false);
}
}

void frmMasterRegionOperator_Create::doApply(void)
{
char dummy[128];
QString error_m;
bool is_defined;
    read_inputs();
    if (target_operation==-1)//create region from set
    {
        if (n_sel_sno<1 || n_sel_gno<1)
        {
        error_m=tr("No set selected to create new region.");
        strcpy(dummy,error_m.toLocal8Bit().constData());
        errmsg(dummy);
        }
        else if (target_region<0 || target_region>=MAXREGION)
        {
        error_m=tr("Invalid target region R")+QString::number(target_region)+QString(".");
        strcpy(dummy,error_m.toLocal8Bit().constData());
        errmsg(dummy);
        }
        else if (getsetlength(sel_gno[0],sel_sno[0])<2)
        {
        error_m=tr("Set has to few points.");
        strcpy(dummy,error_m.toLocal8Bit().constData());
        errmsg(dummy);
        }
        else
        {
        CopyRegion(rg+target_region,&undo_region);
        kill_region(target_region);
        rg[target_region].n = getsetlength(sel_gno[0],sel_sno[0]);
        rg[target_region].x = (double*)xcalloc(rg[target_region].n, sizeof(double));
        rg[target_region].y = (double*)xcalloc(rg[target_region].n, sizeof(double));
        memcpy(rg[target_region].x,getcol(sel_gno[0],sel_sno[0],DATA_X),sizeof(double)*rg[target_region].n);
        memcpy(rg[target_region].y,getcol(sel_gno[0],sel_sno[0],DATA_Y),sizeof(double)*rg[target_region].n);
        rg[target_region].active = TRUE;
        rg[target_region].type = REGION_POLYI;
        change_region_type(target_region,0);
        change_region_type(target_region,REGION_POLYI);
        RegionModified(target_region,rg+target_region,0);
        }
    }
    else//copy region
    {
        if (target_operation==target_region)
        {
        error_m=tr("Source-region and target-region are the same.");
        strcpy(dummy,error_m.toLocal8Bit().constData());
        errmsg(dummy);
        }
        else
        {
        CopyRegion(rg+target_region,&undo_region);
        CopyRegion(rg+target_operation,rg+target_region);
        RegionModified(target_region,rg+target_region,0);
        }
    }
mainWin->mainArea->completeRedraw();
}

void frmMasterRegionOperator_Create::doAccept(void)
{
ApplyError=false;
doApply();
    if (ApplyError==false)
    doClose();
}

void frmMasterRegionOperator_Create::doClose(void)
{
emit(closeWish());
}

frmMasterRegionOperator::frmMasterRegionOperator(QWidget * parent):QDialog(parent)
{
    setWindowTitle(tr("Regions"));
    setWindowIcon(QIcon(*GraceIcon));
tab_Main=new frmMasterRegionOperator_Main(this);
tab_Style=new frmMasterRegionOperator_Style(this);
tab_Edit=new frmMasterRegionOperator_Edit(this);
tab_Operations=new frmMasterRegionOperator_Operations(this);
tab_Create=new frmMasterRegionOperator_Create(this);
    tabs=new QTabWidget(this);
    tabs->addTab(tab_Main,tr("Main"));
    tabs->addTab(tab_Style,tr("Appearance"));
    tabs->addTab(tab_Edit,tr("Edit"));
    tabs->addTab(tab_Operations,tr("Operations"));
    tabs->addTab(tab_Create,tr("Create"));
connect(tab_Main,SIGNAL(closeWish()),SLOT(doClose()));
connect(tab_Style,SIGNAL(closeWish()),SLOT(doClose()));
connect(tab_Edit,SIGNAL(closeWish()),SLOT(doClose()));
connect(tab_Operations,SIGNAL(closeWish()),SLOT(doClose()));
connect(tab_Create,SIGNAL(closeWish()),SLOT(doClose()));
connect(tab_Edit,SIGNAL(initWish()),this,SLOT(init()));
layout=new QVBoxLayout();
//layout->setMargin(STD_MARGIN);
layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
layout->setSpacing(STD_SPACING);
layout->addWidget(tabs);
setLayout(layout);
resize(LastSize_FormRegionMaster);
}

frmMasterRegionOperator::~frmMasterRegionOperator()
{
    LastSize_FormRegionMaster=this->size();
}

void frmMasterRegionOperator::init(void)
{
tab_Main->init();
tab_Style->init();
tab_Edit->init();
tab_Operations->init();
tab_Create->init();
updateDecimalSeparator();
}

void frmMasterRegionOperator::updateDecimalSeparator(void)
{
QLocale newLocale=(DecimalPointToUse=='.')?(*dot_locale):(*comma_locale);
    for (int i=0;i<MAXREGION;i++)
    {
    tab_Style->selWidth[i]->spnLineWidth->setLocale(newLocale);
    }
tab_Edit->regionChanged(tab_Edit->selRegion->currentIndex());
tab_Main->layout->update();
tab_Style->layout->update();
tab_Edit->layout0->update();
tab_Edit->layout->update();
tab_Operations->layout->update();
tab_Create->layout->update();
}

void frmMasterRegionOperator::doClose(void)
{
hide();
}

void frmMasterRegionOperator::number_of_graphs_changed(void)
{
tab_Main->init();
tab_Operations->selOperation->setCurrentIndex(tab_Operations->selOperation->currentIndex());
}

frmMasterRegionOperator_Main::frmMasterRegionOperator_Main(QWidget * parent):QWidget(parent)
{
#if QT_VERSION >= 0x050000

#else
mapActive=new QSignalMapper(this);
//mapType=new QSignalMapper(this);
mapDefine=new QSignalMapper(this);
mapReportSet=new QSignalMapper(this);
mapReportPoints=new QSignalMapper(this);

connect(mapActive,SIGNAL(mapped(int)),this,SLOT(clickActive(int)));
//connect(mapType,SIGNAL(mapped(int)),this,SLOT(changeType(int)));
connect(mapDefine,SIGNAL(mapped(int)),this,SLOT(clickDefine(int)));
connect(mapReportSet,SIGNAL(mapped(int)),this,SLOT(clickReportSets(int)));
connect(mapReportPoints,SIGNAL(mapped(int)),this,SLOT(clickReportPoints(int)));
#endif
layout=new QGridLayout();
//layout->setMargin(STD_MARGIN);
layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
layout->setSpacing(STD_SPACING);
for (int i=0;i<6;i++)
lblTitles[i]=new QLabel(QString("_"),this);

lblTitles[0]->setText(tr("Region"));
lblTitles[1]->setText(tr("Graph"));
lblTitles[2]->setText(tr("Active"));
lblTitles[3]->setText(tr("Type"));
lblTitles[4]->setText(tr("Define"));
lblTitles[5]->setText(tr("Report on"));
layout->addWidget(lblTitles[0],0,0,1,1,Qt::AlignCenter);
layout->addWidget(lblTitles[1],0,1,1,1,Qt::AlignCenter);
layout->addWidget(lblTitles[2],0,2,1,1,Qt::AlignCenter);
layout->addWidget(lblTitles[3],0,3,1,1,Qt::AlignCenter);
layout->addWidget(lblTitles[4],0,4,1,1,Qt::AlignCenter);
layout->addWidget(lblTitles[5],0,5,1,2,Qt::AlignCenter);

for (int i=0;i<7;i++)
{
lblRegions[i]=new QLabel(QString::number(i),this);
layout->addWidget(lblRegions[i],1+i,0,1,1,Qt::AlignCenter);
spnLink[i]=new QSpinBox(this);
spnLink[i]->setRange(0,8);
spnLink[i]->setToolTip(tr("This sets the graph the region will be plotted in.\nIf -1 is set, the region will be shown in all graphs."));
layout->addWidget(spnLink[i],1+i,1,1,1,Qt::AlignCenter);
    connect(spnLink[i],SIGNAL(valueChanged(int)),this,SLOT(changeLink(int)));
cmdActive[i]=new QPushButton(tr("On"),this);
cmdActive[i]->setCheckable(true);
#if QT_VERSION >= 0x050000
    connect(cmdActive[i],&QPushButton::clicked,this,[this,i](){this->clickActive(i);});
#else
    connect(cmdActive[i], SIGNAL(clicked()), mapActive, SLOT(map()));
    mapActive->setMapping(cmdActive[i],i);
#endif
layout->addWidget(cmdActive[i],1+i,2,1,1,Qt::AlignCenter);
cmbType[i]=new QComboBox(this);
    for (int j=0;j<10;j++)
    cmbType[i]->addItem(regtypes[reg_order[j]]);
    connect(cmbType[i],SIGNAL(activated(int)),this,SLOT(changeType(int)));
layout->addWidget(cmbType[i],1+i,3,1,1,Qt::AlignCenter);
cmdDefine[i]=new QPushButton(tr("Define"),this);
#if QT_VERSION >= 0x050000
    connect(cmdDefine[i],&QPushButton::clicked,this,[this,i](){this->clickDefine(i);});
#else
    connect(cmdDefine[i], SIGNAL(clicked()), mapDefine, SLOT(map()));
    mapDefine->setMapping(cmdDefine[i],i);
#endif
layout->addWidget(cmdDefine[i],1+i,4,1,1,Qt::AlignCenter);
cmdReportSet[i]=new QPushButton(tr("Sets"),this);
#if QT_VERSION >= 0x050000
    connect(cmdReportSet[i],&QPushButton::clicked,this,[this,i](){this->clickReportSets(i);});
#else
    connect(cmdReportSet[i], SIGNAL(clicked()), mapReportSet, SLOT(map()));
    mapReportSet->setMapping(cmdReportSet[i],i);
#endif
layout->addWidget(cmdReportSet[i],1+i,5,1,1,Qt::AlignCenter);
cmdReportPoints[i]=new QPushButton(tr("Points"),this);
#if QT_VERSION >= 0x050000
    connect(cmdReportPoints[i],&QPushButton::clicked,this,[this,i](){this->clickReportPoints(i);});
#else
    connect(cmdReportPoints[i], SIGNAL(clicked()), mapReportPoints, SLOT(map()));
    mapReportPoints->setMapping(cmdReportPoints[i],i);
#endif
layout->addWidget(cmdReportPoints[i],1+i,6,1,1,Qt::AlignCenter);
}

lblRegions[MAXREGION]->setText(tr("Inside World"));
lblRegions[MAXREGION+1]->setText(tr("Outside World"));

cmdActive[MAXREGION]->setVisible(false);
cmbType[MAXREGION]->setVisible(false);
cmdDefine[MAXREGION]->setVisible(false);
spnLink[MAXREGION]->setVisible(false);
cmdActive[MAXREGION+1]->setVisible(false);
cmbType[MAXREGION+1]->setVisible(false);
cmdDefine[MAXREGION+1]->setVisible(false);
spnLink[MAXREGION+1]->setVisible(false);

cmdClearARegion=new QPushButton(tr("Clear one region"),this);
connect(cmdClearARegion,SIGNAL(clicked()),SLOT(doClearARegion()));
cmdClearAllRegions=new QPushButton(tr("Clear all regions"),this);
connect(cmdClearAllRegions,SIGNAL(clicked()),SLOT(doClearAllRegions()));
cmdClose=new QPushButton(tr("Close"),this);
cmdClose->setDefault(true);
connect(cmdClose,SIGNAL(clicked()),SLOT(doClose()));
layout->addWidget(cmdClearARegion,8,0,1,3);
layout->addWidget(cmdClearAllRegions,8,3,1,2);
layout->addWidget(cmdClose,8,5,1,2);

setLayout(layout);
}

void frmMasterRegionOperator_Main::init(void)
{
static bool init_running=false;
bool is_defined=false;
if (init_running==true) return;
init_running=true;
for (int i=0;i<7;i++)
{
disconnect(spnLink[i],SIGNAL(valueChanged(int)),this,SLOT(changeLink(int)));
disconnect(cmbType[i],SIGNAL(activated(int)),this,SLOT(changeType(int)));
}
    for (int i=0;i<MAXREGION;i++)
    {
    spnLink[i]->setRange(-1,number_of_graphs()-1);
    //cout << "number_o_graphs=" << number_of_graphs() << endl;
        if (rg[i].type==0 && rg[i].x1==0.0 && rg[i].x2==0.0 && rg[i].y1==0.0 && rg[i].y2==0.0)
        {//region not defined
        cmdActive[i]->setText(tr("---"));
        cmdActive[i]->setChecked(false);
        cmdActive[i]->setEnabled(false);
        lblRegions[i]->setText(QString::number(i));
        spnLink[i]->setValue(cg);
        is_defined=false;
        }
        else//region defined
        {
        is_defined=true;
        //lblRegions[i]->setText(QString::number(i)+QString(" (G")+QString::number(rg[i].linkto)+QString(")"));
        lblRegions[i]->setText(QString::number(i));
        cmdActive[i]->setEnabled(true);
        if (is_valid_gno(rg[i].linkto)==TRUE || rg[i].linkto==-1)
        spnLink[i]->setValue(rg[i].linkto);
        else
        {
        char dummy[1024];
        QString error_text;
        error_text=tr("Region ")+QString::number(i)+tr(" linked to invalid graph G")+QString::number(rg[i].linkto)+tr(". Setting link to current graph (G")+QString::number(cg)+tr(") and region deactivated.");
        strcpy(dummy,error_text.toLocal8Bit().constData());
        errmsg(dummy);
        rg[i].linkto=cg;
        spnLink[i]->setValue(rg[i].linkto);
        rg[i].active=false;
        }
            if (rg[i].active==true)
            {
            cmdActive[i]->setText(tr("On"));
            cmdActive[i]->setChecked(true);
            }
            else
            {
            cmdActive[i]->setText(tr("Off"));
            cmdActive[i]->setChecked(false);
            }
        }
        if (reg_order[cmbType[i]->currentIndex()]!=rg[i].type)
        {
            for (int j=0;j<10;j++)
            {
                if (is_defined==false)
                {
                    if (reg_order[j]==REGION_POLYI) cmbType[i]->setCurrentIndex(j);
                }
                else if (reg_order[j]==rg[i].type)
                {
                    cmbType[i]->setCurrentIndex(j);
                }
            }
        }
    }
for (int i=0;i<7;i++)
{
connect(spnLink[i],SIGNAL(valueChanged(int)),SLOT(changeLink(int)));
connect(cmbType[i],SIGNAL(activated(int)),SLOT(changeType(int)));
}
init_running=false;
}

void frmMasterRegionOperator_Main::doClearARegion(void)
{
bool ok;
int regno = QInputDialog::getInt(this, tr("Clear region"),tr("# of region to be cleared:"), 0, 0, 4, 1, &ok);
    if (ok)
    {
    //cout << "Clear Region " << regno << endl;
    RegionModified(regno,NULL,1);
    kill_region(regno);
    mainWin->mainArea->completeRedraw();
    init();
    }
}

void frmMasterRegionOperator_Main::doClearAllRegions(void)
{
    int i;
    int ret=QMessageBox::question(this,tr("Clear all regions"),tr("Do you really want to clear all regions?"),QMessageBox::Yes,QMessageBox::No);
    if (ret==QMessageBox::Yes)
    {
    //cout << "killing all" << endl;
    RegionModified(MAXREGION,NULL,1);
    for (i = 0; i < MAXREGION; i++)
    kill_region(i);
    mainWin->mainArea->completeRedraw();
    init();
    }
    else
    {
    ;//cout << "killing aborted" << endl;
    }
}

void frmMasterRegionOperator_Main::doClose(void)
{
emit(closeWish());
}

void frmMasterRegionOperator_Main::clickActive(int regno)
{
char com_dummy[128];
ListOfChanges.clear();
ListOfOldStates.clear();
sprintf(com_dummy,"r%1d %s",regno,(rg[regno].active?"on":"off"));
ListOfOldStates << com_dummy;
CopyRegion(rg+regno,&undo_region);
//change
    rg[regno].active=(!rg[regno].active);
sprintf(com_dummy,"r%1d %s",regno,(rg[regno].active?"on":"off"));
ListOfChanges << com_dummy;
RegionModified(regno,rg+regno,0);
    init();
    mainWin->mainArea->completeRedraw();
}

void frmMasterRegionOperator_Main::changeType(int reg_no)
{
    (void)reg_no;
int regno=0;
char com_dummy[128];
//first: find the one region that has a different type than what is displayed
    for (int i=0;i<5;i++)
    {
        if (rg[i].type!=reg_order[cmbType[i]->currentIndex()])
        {
        regno=i;
        break;
        }
    }
ListOfChanges.clear();
ListOfOldStates.clear();
region_type_string(regno,com_dummy);
ListOfOldStates << com_dummy;
CopyRegion(rg+regno,&undo_region);
//change
change_region_type(regno,reg_order[cmbType[regno]->currentIndex()]);
region_type_string(regno,com_dummy);
ListOfChanges << com_dummy;
RegionModified(regno,rg+regno,0);
//redisplay
    init();
//cout << "TypeChanged regno=" << regno << " type=" << rg[regno].type << endl;
mainWin->mainArea->completeRedraw();
}

void frmMasterRegionOperator_Main::changeLink(int reg_no)
{
    (void)reg_no;
static bool changeRunning=false;
if (changeRunning==true) return;
int regno=0;
char com_dummy[128];
changeRunning=true;
    for (int i=0;i<MAXREGION;i++)
    {
        if (rg[i].linkto!=spnLink[i]->value())
        {
        regno=i;
        break;
        }
    }
ListOfChanges.clear();
ListOfOldStates.clear();
sprintf(com_dummy,"link r%1d to g%d",regno,rg[regno].linkto);
ListOfOldStates << com_dummy;
CopyRegion(rg+regno,&undo_region);
//cout << "regno=" << regno << " reg_no=" << reg_no << " value=" << spnLink[regno]->value() << endl;
rg[regno].linkto=spnLink[regno]->value();
sprintf(com_dummy,"link r%1d to g%d",regno,rg[regno].linkto);
ListOfChanges << com_dummy;
RegionModified(regno,rg+regno,0);
init();
mainWin->mainArea->completeRedraw();
changeRunning=false;
}

void frmMasterRegionOperator_Main::clickDefine(int regno)
{
//cout << "define region: " << regno << endl;
define_region(regno,reg_order[cmbType[regno]->currentIndex()]);
}

void frmMasterRegionOperator_Main::clickReportSets(int regno)
{
//cout << "ReportSets: " << regno << endl;
reporton_region(get_cg(), regno, 0);
}

void frmMasterRegionOperator_Main::clickReportPoints(int regno)
{
//cout << "ReportPoints: " << regno << endl;
reporton_region(get_cg(), regno, 1);
}

frmRegionStatus::frmRegionStatus(QWidget * parent):QDialog(parent)
{
    background=new QWidget(this);
    setWindowTitle(tr("Status"));
    lblHeader=new QLabel(tr("Region # Active Type"),background);
    char dummy[16];
    QString nr;
    QString info;
    active=tr("on");
    inactive=tr("off");

    for (int i=0;i<MAXREGION;i++)
    {
        sprintf(dummy,"%i",i);
        nr=QString(dummy);
        info=QString("   ")+nr+QString("   ")+inactive+QString("   ")+regtypes[2];
        lblStatus[i]=new QLabel(info,background);
    }

    cmdClose=new QPushButton(tr("Close"),this);
    connect(cmdClose,SIGNAL(clicked()),SLOT(doClose()));
    cmdUpdate=new QPushButton(tr("Update"),this);
    connect(cmdUpdate,SIGNAL(clicked()),SLOT(doUpdate()));

    layout0=new QVBoxLayout;
    //layout0->setMargin(STD_MARGIN);
    layout0->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout0->addWidget(lblHeader);
    for (int i=0;i<MAXREGION;i++)
        layout0->addWidget(lblStatus[i]);

    background->setLayout(layout0);

    scroll=new QScrollArea(this);
    scroll->setWidget(background);

    layout=new QGridLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(scroll,0,0,1,2);
    layout->addWidget(cmdClose,1,1);
    layout->addWidget(cmdUpdate,1,0);
    setLayout(layout);
    resize(LastSize_FormRegionStatus);
}

frmRegionStatus::~frmRegionStatus()
{
    LastSize_FormRegionStatus=this->size();
}

void frmRegionStatus::init(void)
{
    doUpdate();
}

void frmRegionStatus::doUpdate(void)
{
    char dummy[16];
    QString nr;
    QString info;
    for (int rno=0;rno<MAXREGION;rno++)
    {
        sprintf(dummy,"%i",rno);
        nr=QString(dummy);
        info=QString("   ")+nr+QString("   ");
        if (rg[rno].active==true)
            info=info+active+QString("   ")+regtypes[rg[rno].type];
        else
            info=info+inactive+QString("   ")+regtypes[rg[rno].type];
        lblStatus[rno]->setText(info);
    }
    background->adjustSize();
    layout0->update();
    layout->update();
}

void frmRegionStatus::doClose(void)
{
    hide();
}

frmRegions::frmRegions(int type,QWidget * parent):QDialog(parent)
{
    char dummy[16];
    int number;
    QString entr[MAXREGION>12?MAXREGION+2:12];
//setFont(*stdFont);
    setWindowIcon(QIcon(*GraceIcon));
    setWindowTitle(tr(""));
    windowtype=type;

    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    buttonGroup=new stdButtonGroup(this,false,true,false);
    buttonGroup->cmdAccept->setDefault(TRUE);
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));

    switch (type)
    {
    case DEFINEREGIONWINDOW:
        setWindowTitle(tr("Define region"));
        buttonGroup->cmdAccept->setText(tr("Define"));
        number=MAXREGION;
        for (int i=0;i<MAXREGION;i++)
        {
            sprintf(dummy,"%i",i);
            entr[i]=QString(dummy);
        }
        selector0=new StdSelector(this,tr("Define region:"),number,entr);
        number=10;
        entr[0]=tr("Inside polygon");
        entr[1]=tr("Outside polygon");
        entr[2]=tr("Above line");
        entr[3]=tr("Below line");
        entr[4]=tr("Left of line");
        entr[5]=tr("Right of line");
        entr[6]=tr("In Horiz. Range");
        entr[7]=tr("In Vert. Range");
        entr[8]=tr("Out of Horiz. Range");
        entr[9]=tr("Out of Vert. Range");
        selector1=new StdSelector(this,tr("Region type:"),number,entr);
        connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doDefine()));
        layout->addWidget(selector0);
        layout->addWidget(selector1);
        break;
    case REPORTREGIONWINDOW:
        setWindowTitle(tr("Report on sets in region"));
        number=MAXREGION+2;
        for (int i=0;i<MAXREGION;i++)
        {
            sprintf(dummy,"%i",i);
            entr[i]=QString(dummy);
        }
        entr[MAXREGION]=tr("Inside world");
        entr[MAXREGION+1]=tr("Outside world");
        selector0=new StdSelector(this,tr("Report on sets in region:"),number,entr);
        number=2;
        entr[0]=tr("Sets");
        entr[1]=tr("Points");
        selector1=new StdSelector(this,tr("Report type:"),number,entr);
        connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
        layout->addWidget(selector0);
        layout->addWidget(selector1);
        break;
    case CLEARREGIONWINDOW:
        setWindowTitle(tr("Clear region"));
        number=MAXREGION+1;
        for (int i=0;i<MAXREGION;i++)
        {
            sprintf(dummy,"%i",i);
            entr[i]=QString(dummy);
        }
        entr[MAXREGION]=tr("All");
        selector0=new StdSelector(this,tr("Clear region:"),number,entr);
        selector1=new StdSelector(this,QString("dummy"),number,entr);
        connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
        layout->addWidget(selector0);
        selector1->setVisible(FALSE);
        break;
    }
    layout->addWidget(buttonGroup);
    setLayout(layout);
    switch (windowtype)
    {
    case DEFINEREGIONWINDOW:
    resize(LastSize_FormDefineRegion);
    break;
    case REPORTREGIONWINDOW:
    resize(LastSize_FormReportRegion);
    break;
    case CLEARREGIONWINDOW:
    resize(LastSize_FormClearRegion);
    break;
    }
}

frmRegions::~frmRegions()
{
    switch (windowtype)
    {
    case DEFINEREGIONWINDOW:
    LastSize_FormDefineRegion=this->size();
    break;
    case REPORTREGIONWINDOW:
    LastSize_FormReportRegion=this->size();
    break;
    case CLEARREGIONWINDOW:
    LastSize_FormClearRegion=this->size();
    break;
    }
}

void frmRegions::init(void)
{
    ;//Nothing to do here !(?)
}

void frmRegions::doAccept(void)
{
    int regno,type,i;
    switch (windowtype)
    {
    case CLEARREGIONWINDOW:
        regno=GetChoice(selector0);
        RegionModified(regno,NULL,1);
        if (regno == MAXREGION)
        {
            for (i = 0; i < MAXREGION; i++)
                kill_region(i);
        }
        else
        {
            kill_region(regno);
        }
        mainWin->mainArea->completeRedraw();
        break;
    case REPORTREGIONWINDOW:
        regno = GetChoice(selector0);
        type = GetChoice(selector1);
        reporton_region(get_cg(), regno, type);
        break;
    }

}

void frmRegions::doDefine(void)
{
    int rtype = GetChoice(selector1);
    nr = GetChoice(selector0);
    define_region(nr, reg_order[rtype]);
}

void define_region(int c_nr, int rtype)
{
    CopyRegion(rg+c_nr,&undo_region);//copy old region into temporary memory
    CopyRegion(rg+c_nr,&undo_region2);
    kill_region(c_nr);
    nr=c_nr;
    switch (rtype) {
    case REGION_POLYI:
        regiontype = REGION_POLYI;
        do_select_region();
        break;
    case REGION_POLYO:
        regiontype = REGION_POLYO;
        do_select_region();
        break;
    case REGION_ABOVE:
        regiontype = REGION_ABOVE;
        set_action(DO_NOTHING);
        set_action(DEF_REGION1ST);
        break;
    case REGION_BELOW:
        regiontype = REGION_BELOW;
        set_action(DO_NOTHING);
        set_action(DEF_REGION1ST);
        break;
    case REGION_TOLEFT:
        regiontype = REGION_TOLEFT;
        set_action(DO_NOTHING);
        set_action(DEF_REGION1ST);
        break;
    case REGION_TORIGHT:
        regiontype = REGION_TORIGHT;
        set_action(DO_NOTHING);
        set_action(DEF_REGION1ST);
        break;
    case REGION_HORIZI:
        regiontype= REGION_HORIZI;
        set_action(DO_NOTHING);
        set_action(DEF_REGION1ST);
        break;
    case REGION_VERTI:
        regiontype= REGION_VERTI;
        set_action(DO_NOTHING);
        set_action(DEF_REGION1ST);
        break;
    case REGION_HORIZO:
        regiontype= REGION_HORIZO;
        set_action(DO_NOTHING);
        set_action(DEF_REGION1ST);
        break;
    case REGION_VERTO:
        regiontype= REGION_VERTO;
        set_action(DO_NOTHING);
        set_action(DEF_REGION1ST);
        break;
    }
}

void frmRegions::doClose(void)
{
    hide();
}


frmAbout::frmAbout(QWidget * parent):QDialog(parent)
{
//setFont(*stdFont);
    setWindowTitle(tr("QtGrace: About"));
    setWindowIcon(QIcon(*GraceIcon));

    char buf[1024];
    int index=0,s_index,e_index;

    grpGrace=new QGroupBox(QString(""),this);
    layout0=new QVBoxLayout();
    //layout0->setMargin(STD_MARGIN);
    layout0->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout0->setSpacing(STD_SPACING);
    lblInfo[index]=new QLabel(QString(bi_version_string())+QString(" / QtGrace ") + QString(QTGRACE_VERSION_STRING),grpGrace);
    layout0->addWidget(lblInfo[index++]);
#ifdef DEBUG
    lblInfo[index]=new QLabel(tr("Debugging is enabled"),grpGrace);
    layout0->addWidget(lblInfo[index++]);
#endif
    grpGrace->setLayout(layout0);
    s_index=index;
    grpLegal=new QGroupBox(tr("Legal stuff"),this);
    layout1=new QVBoxLayout;
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout1->setSpacing(STD_SPACING);
    lblInfo[index++]=new QLabel(QString(bi_version_string())+QString(":"),grpLegal);
    lblInfo[index++]=new QLabel(tr("      Copyright (c) 1991-1995 Paul J Turner"),grpLegal);
    lblInfo[index++]=new QLabel(tr("      Copyright (c) 1996-2008 Grace Development Team"),grpLegal);
    lblInfo[index++]=new QLabel(tr("      Maintained by Evgeny Stambulchik"),grpLegal);
    lblInfo[index++]=new QLabel(tr("QtGrace:"),grpLegal);
    lblInfo[index++]=new QLabel(tr("      Copyright (c) 2008-2022 Andreas Winter"),grpLegal);
    lblInfo[index++]=new QLabel(tr("      Additional code by Vadim Engelson and Nimalendiran Kailasanathan, Wolfram MathCore AB"),grpLegal);
    lblInfo[index++]=new QLabel(tr("All rights reserved"),grpLegal);
    lblInfo[index++]=new QLabel(tr("The program is distributed under the terms of the GNU General Public License"),grpLegal);
    e_index=index;
    for (int i=s_index;i<e_index;i++)
        layout1->addWidget(lblInfo[i]);

    grpLegal->setLayout(layout1);
    s_index=index;
    grpThirdParty=new QGroupBox(tr("Third party copyright"),this);
    layout2=new QVBoxLayout;
    //layout2->setMargin(STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout2->setSpacing(STD_SPACING);
    /*lblInfo[index++]=new QLabel(QString("Tab widget, Copyright (c) 1997 Pralay Dakua"),grpThirdParty);
    lblInfo[index++]=new QLabel(QString("Xbae widget,"),grpThirdParty);
    lblInfo[index++]=new QLabel(QString("      Copyright (c) 1991, 1992 Bell Communications Research, Inc. (Bellcore)"),grpThirdParty);
    lblInfo[index++]=new QLabel(QString("      Copyright (c) 1995-1999 Andrew Lister"),grpThirdParty);*/
    lblInfo[index++]=new QLabel(QString("FFTW3 Library,"),grpThirdParty);
    lblInfo[index++]=new QLabel(QString("      Copyright (c) 2003, 2007-14 Matteo Frigo"),grpThirdParty);
    lblInfo[index++]=new QLabel(QString("      Copyright (c) 2003, 2007-14 Massachusetts Institute of Technology"),grpThirdParty);
    lblInfo[index++]=new QLabel(QString("Haru Free PDF Library,"),grpThirdParty);
    lblInfo[index++]=new QLabel(QString("      Copyright (c) 1999-2006 Takeshi Kanno"),grpThirdParty);
    lblInfo[index++]=new QLabel(QString("      Copyright (c) 2007-2009 Antony Dovgal"),grpThirdParty);
    lblInfo[index++]=new QLabel(QString("Raster driver based on the GD-1.3 library,"),grpThirdParty);
    lblInfo[index++]=new QLabel(QString("      Portions copyright (c) 1994-1998 Cold Spring Harbor Laboratory"),grpThirdParty);
    lblInfo[index++]=new QLabel(QString("      Portions copyright (c) 1996-1998 Boutell.Com, Inc"),grpThirdParty);

    e_index=index;
    for (int i=s_index;i<e_index;i++)
        layout2->addWidget(lblInfo[i]);

#ifdef HAVE_LIBPDF
    lblInfo[index]=new QLabel(QString("PDFlib library, Copyright (c) 1997-2002 Thomas Merz");
    layout2->addWidget(lblInfo[index++]);
#endif

    grpThirdParty->setLayout(layout2);
    s_index=index;
    grpBuildInfo=new QGroupBox(tr("Build info"),this);
    layout3=new QVBoxLayout;
    //layout3->setMargin(STD_MARGIN);
    layout3->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout3->setSpacing(STD_SPACING);
    sprintf(buf, "Host: %s", bi_system());
    lblInfo[index++]=new QLabel(QString(buf),grpBuildInfo);
    sprintf(buf, "Time: %s %s", bi_date(), __TIME__);
    lblInfo[index++]=new QLabel(QString(buf),grpBuildInfo);
    sprintf(buf, "GUI toolkit: %s ", bi_gui());
    lblInfo[index++]=new QLabel(QString(buf),grpBuildInfo);
    /*
    sprintf(buf, "Xbae version: %s ", bi_gui_xbae());
    lblInfo[19]=new QLabel(QString(buf),grpBuildInfo);
    */
    sprintf(buf, "Qt version: %s ", QT_VERSION_STR);
    lblInfo[index++]=new QLabel(QString(buf),grpBuildInfo);
    sprintf(buf, "T1lib: %s ", T1_GetLibIdent());
    lblInfo[index++]=new QLabel(QString(buf),grpBuildInfo);
    e_index=index;
    for (int i=s_index;i<e_index;i++)
        layout3->addWidget(lblInfo[i]);

#ifdef HAVE_LIBPNG
    sprintf(buf, "libpng: %s ", bi_pnglib());
    lblInfo[index]=new QLabel(QString(buf),grpBuildInfo);
    layout3->addWidget(lblInfo[index++]);
#endif
#ifdef HAVE_LIBJPEG
    sprintf(buf, "libjpeg: %s ", bi_libjpeg());
    lblInfo[index]=new QLabel(QString(buf),grpBuildInfo);
    layout3->addWidget(lblInfo[index++]);
#endif
#ifdef HAVE_LIBPDF
    sprintf(buf, "PDFlib: %s ", bi_libpdf());
    lblInfo[index]=new QLabel(QString(buf),grpBuildInfo);
    layout3->addWidget(lblInfo[index++]);
#endif

    grpBuildInfo->setLayout(layout3);

    grpHomePage=new QGroupBox(tr("Home pages"),this);
    layout4=new QVBoxLayout;
    //layout4->setMargin(0);
    layout4->setContentsMargins(0,0,0,0);
    layout4->setSpacing(0);
    cmdIAdr=new QPushButton(QString("http://plasma-gate.weizmann.ac.il/Grace/"),grpHomePage);
    cmdIAdr->setFlat(true);
    connect(cmdIAdr,SIGNAL(clicked()),SLOT(doShowHomepage()));
    cmdIAdr2=new QPushButton(QString("http://sourceforge.net/projects/qtgrace"),grpHomePage);
    cmdIAdr2->setFlat(true);
    connect(cmdIAdr2,SIGNAL(clicked()),SLOT(doShowHomepage2()));
    cmdIAdr3=new QPushButton(QString("http://www.qt.io/"),grpHomePage);
    cmdIAdr3->setFlat(true);
    connect(cmdIAdr3,SIGNAL(clicked()),SLOT(doShowHomepage3()));
    layout4->addWidget(cmdIAdr);
    layout4->addWidget(cmdIAdr2);
    layout4->addWidget(cmdIAdr3);
    grpHomePage->setLayout(layout4);

    cmdClose=new QPushButton(tr("Close"),this);
    connect(cmdClose,SIGNAL(clicked()),SLOT(doClose()));

    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(grpGrace);
    layout->addWidget(grpLegal);
    layout->addWidget(grpThirdParty);
    layout->addWidget(grpBuildInfo);
    layout->addWidget(grpHomePage);
    layout->addWidget(cmdClose);
    setLayout(layout);
    resize(LastSize_FormAbout);
}

frmAbout::~frmAbout()
{
    LastSize_FormAbout=this->size();
}

void frmAbout::doShowHomepage(void)
{
    HelpCB("http://plasma-gate.weizmann.ac.il/Grace/");
}

void frmAbout::doShowHomepage2(void)
{
    HelpCB("http://sourceforge.net/projects/qtgrace");
}

void frmAbout::doShowHomepage3(void)
{
    HelpCB("http://www.qt.io/");
}

void frmAbout::doClose(void)
{
    hide();
}


static CMap_entry base_colors[8] = {
    /*0 black  */
    {{0, 0, 0}, NULL, COLOR_MAIN, 0},
    /*1 red    */
    {{255, 0, 0}, NULL, COLOR_MAIN, 0},
    /*2 green  */
    {{0, 255, 0}, NULL, COLOR_MAIN, 0},
    /*3 yellow */
    {{255, 255, 0}, NULL, COLOR_MAIN, 0},
    /*4 blue   */
    {{0, 0, 255}, NULL, COLOR_MAIN, 0},
    /*5 magenta*/
    {{255, 0, 255}, NULL, COLOR_MAIN, 0},
    /*6 white  */
    {{255, 255, 255}, NULL, COLOR_MAIN, 0},
    /*7 cyan   */
    {{0, 255, 255}, NULL, COLOR_MAIN, 0}
};

char base_color_names[8][16];

frmColorManagement::frmColorManagement(QWidget * parent):QWidget(parent)
{
    int number,row;
    QString * entr;

strcpy(base_color_names[0],QString("black").toLatin1().constData());
strcpy(base_color_names[1],QString("red").toLatin1().constData());
strcpy(base_color_names[2],QString("green").toLatin1().constData());
strcpy(base_color_names[3],QString("yellow").toLatin1().constData());
strcpy(base_color_names[4],QString("blue").toLatin1().constData());
strcpy(base_color_names[5],QString("magenta").toLatin1().constData());
strcpy(base_color_names[6],QString("white").toLatin1().constData());
strcpy(base_color_names[7],QString("cyan").toLatin1().constData());
for (int i=0;i<8;i++) base_colors[i].cname=copy_string(base_colors[i].cname,base_color_names[i]);

    local_cmap_table=NULL;
    allocated_loc_colors=temp_spec_length=spectrum_path_length=0;
    temp_spec=NULL;
    path_positions=NULL;
    locColorIcons=NULL;
    locColorPixmaps=NULL;
    locColorNames=NULL;
    tmp_path_length=2;
    tmp_path_pos=new int[2];
    tmp_path_pos[0]=1;//0;
    tmp_path_pos[1]=0;//6;
    tmp_path_point_shown=0;

    int * real_colors=new int[4];
    int nr_of_aux_cols;
    map_entries=get_main_color_indices(&real_colors,&nr_of_aux_cols);
    N_DELETE2(real_colors);

//setFont(*stdFont);
    setWindowTitle(tr("QtGrace: Color Management"));
    setWindowIcon(QIcon(*GraceIcon));
    colorsel=new ColorSelector(this);//shows current palette
    colorsel->setAlpha(255);
    colorsel->alphaSelector->hide();
    colorsel->cmbColorSelect->panels->alphaSlider->hide();
    colorsel->lblText->setText(tr("Current color palette:"));
    ///colorsel->prevent_from_update=true;//I deactivated this, there is no real reason for having this and you get more problems with having this active
    connect(colorsel,SIGNAL(currentIndexChanged(int)),SLOT(curColorChanged(int)));
    colDial=new QColorDialog(this);
    colDial->hide();

    row=0;
    grpCurCol=new QGroupBox(tr("Single Color"),this);
    lblInstructions=new QLabel(tr("Edit single color in color palette."),this);
    lblIllustration=new QLabel("",this);
    ledRed=new stdIntSelector(this,tr("Red:"),0,255);
    ledGreen=new stdIntSelector(this,tr("Green:"),0,255);
    ledBlue=new stdIntSelector(this,tr("Blue:"),0,255);
    connect(ledRed,SIGNAL(currentValueChanged(int)),SLOT(colorCompositionChanged(int)));
    connect(ledGreen,SIGNAL(currentValueChanged(int)),SLOT(colorCompositionChanged(int)));
    connect(ledBlue,SIGNAL(currentValueChanged(int)),SLOT(colorCompositionChanged(int)));
    ledColName=new stdLineEdit(this,tr("Name:"));
    connect(ledColName,SIGNAL(changed()),SLOT(colorNameChanged()));
    cmdDelColor=new QPushButton(tr("Delete color"),this);
    connect(cmdDelColor,SIGNAL(clicked()),SLOT(doDelColor()));
    cmdAddColor=new QPushButton(tr("Add color"),this);
    connect(cmdAddColor,SIGNAL(clicked()),SLOT(doAddColor()));
    cmdEditColor=new QPushButton(tr("Edit color"),this);
    connect(cmdEditColor,SIGNAL(clicked()),SLOT(doEditColor()));
    cmdSelNewColor=new QPushButton(tr("Define a color"),this);
    connect(cmdSelNewColor,SIGNAL(clicked()),SLOT(doSelColor()));
    lblColNumber=new QLabel(tr("Number = -1"),this);
    layout0=new QGridLayout;
    //layout0->setMargin(STD_MARGIN);
    layout0->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout0->setSpacing(STD_SPACING);
    layout0->addWidget(lblInstructions,row,0,1,1);
    layout0->addWidget(lblColNumber,row++,1,1,1);

    layout0->addWidget(ledRed,row,0,1,1);
    layout0->addWidget(ledGreen,row++,1,1,1);
    layout0->addWidget(ledBlue,row,0,1,1);
    layout0->addWidget(ledColName,row++,1,1,1);

    layout0->addWidget(lblIllustration,row++,0,1,2,Qt::AlignHCenter);

    layout0->addWidget(cmdAddColor,row,0,1,1);
    layout0->addWidget(cmdDelColor,row++,1,1,1);
    layout0->addWidget(cmdEditColor,row,0,1,1);
    layout0->addWidget(cmdSelNewColor,row++,1,1,1);

    grpCurCol->setLayout(layout0);

    row=0;
    grpColSpectra=new QGroupBox(tr("Color Spectrum"),this);
    lblInstructions2=new QLabel(tr("Replace whole color palette with specified spectrum."),this);
    lblIllustration2=new QLabel("",this);
    cmdStdCols=new QPushButton(tr("Set standard Grace colors"),this);
    connect(cmdStdCols,SIGNAL(clicked()),SLOT(doSetStdColors()));
    cmdSetSpectrum=new QPushButton(tr("Set Spectrum"),this);
    connect(cmdSetSpectrum,SIGNAL(clicked()),SLOT(doSetSpectrum()));
    number=5;
    entr=new QString[number];
    entr[0]=QString("Color-spectrum");
    entr[1]=QString("Black-White-spectrum");
    entr[2]=QString("Cold-Hot-spectrum");
    entr[3]=QString("Traffic-Light-spectrum");
    entr[4]=QString("Custom...");
    selStdSpectra=new StdSelector(this,tr("Define spectrum:"),number,entr);
    connect(selStdSpectra,SIGNAL(currentIndexChanged(int)),SLOT(curSpectrumChanged(int)));
    chkInvert=new QCheckBox(tr("Invert spectrum"),this);
    connect(chkInvert,SIGNAL(toggled(bool)),SLOT(changedInvert(bool)));
    selNumberOfColors=new stdIntSelector(this,tr("Number of colors in spectrum:"),5,256);
    selNumberOfColors->setValue(52);
    connect(selNumberOfColors,SIGNAL(currentValueChanged(int)),SLOT(curNumberOfColorsChanged(int)));

    selNumberOfPathPoints=new stdIntSelector(this,tr("Path points:"),2,32);
    connect(selNumberOfPathPoints,SIGNAL(currentValueChanged(int)),SLOT(NrOfPathPointsChanged(int)));
    selNumberOfCurPathPoint=new stdIntSelector(this,tr("Current path point:"),0,1);
    connect(selNumberOfCurPathPoint,SIGNAL(currentValueChanged(int)),SLOT(CurrentPathPointChanged(int)));
    selPathCol=new ColorSelector(this);
    selPathCol->setAlpha(255);
    selPathCol->alphaSelector->hide();
    init_color_icons(8,base_colors,allocated_loc_colors,&locColorIcons,&locColorPixmaps,&locColorNames);
    selPathCol->updateColorIcons(8,locColorPixmaps,locColorNames);
    connect(selPathCol,SIGNAL(currentIndexChanged(int)),SLOT(CurrentPathColorChanged(int)));

    locColorIcons=NULL;
    locColorPixmaps=NULL;
    locColorNames=NULL;

    layout1=new QGridLayout;
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout1->setSpacing(STD_SPACING);
    layout1->addWidget(lblInstructions2,row++,0,1,2);
    layout1->addWidget(selStdSpectra,row++,0,1,2);

    layout1->addWidget(selNumberOfColors,row,0,1,1);
    layout1->addWidget(chkInvert,row++,1,1,1);

    layout1->addWidget(selNumberOfPathPoints,row,0,1,1);
    layout1->addWidget(selNumberOfCurPathPoint,row++,1,1,1);
    layout1->addWidget(selPathCol,row++,0,1,2);

    layout1->addWidget(lblIllustration2,row++,0,1,2,Qt::AlignHCenter);
    layout1->addWidget(cmdSetSpectrum,row,0,1,1);
    layout1->addWidget(cmdStdCols,row++,1,1,1);
    grpColSpectra->setLayout(layout1);

    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->setSpacing(STD_SPACING);

    cmdInverseColor=new QPushButton(tr("Invert color palette"),this);
    cmdInverseColor->setToolTip(tr("Invert all colors (has immediate effect)"));
    connect(cmdInverseColor,SIGNAL(clicked()),SLOT(doInvertColors()));

    aac=new stdButtonGroup(this,true,true,false);
    connect(aac->cmdApply,SIGNAL(clicked()),SLOT(doApply()));
    connect(aac->cmdAccept,SIGNAL(clicked()),SLOT(doAccept()));
    connect(aac->cmdClose,SIGNAL(clicked()),SLOT(doClose()));
    layout->addWidget(colorsel);
    layout->addWidget(grpCurCol);
    layout->addWidget(grpColSpectra);
    layout->addWidget(cmdInverseColor);
    layout->addStretch(2);
    layout->addWidget(aac);
    setLayout(layout);

colorsel->setToolTip(tr("The currently active color palette"));
grpCurCol->setToolTip(tr("The settings in this group are used to define or edit single colors"));
//lblInstructions->setToolTip(tr(""));
lblIllustration->setToolTip(tr("How the current color looks like"));
ledRed->setToolTip(tr("Red component of the current color"));
ledGreen->setToolTip(tr("Green component of the current color"));
ledBlue->setToolTip(tr("Blue component of the current color"));
ledColName->setToolTip(tr("Define name of color"));
cmdSelNewColor->setToolTip(tr("Select a new color from the color-selector of the operating system"));
cmdDelColor->setToolTip(tr("Delete the color at the current postion from the list"));
cmdAddColor->setToolTip(tr("Append the displayed color at the end of the list"));
cmdEditColor->setToolTip(tr("Set the color in the list at the current position to the color displayed here"));
lblColNumber->setToolTip(tr("Number of the color currently displayed / current position"));
grpColSpectra->setToolTip(tr("The settings in this dialog alter the whole list of colors at once"));
//lblInstructions2->setToolTip(tr(""));
lblIllustration2->setToolTip(tr("Displays the new colors as a spectrum"));
cmdStdCols->setToolTip(tr("Resets the colors to the default list used in Grace"));
cmdSetSpectrum->setToolTip(tr("Sets the currently displayed spectrum as the new list of colors for this project"));
selStdSpectra->setToolTip(tr("Select the type of spectrum to be used (predefined or Custom)"));
chkInvert->setToolTip(tr("Reverses the order the colors are put into the list"));
selNumberOfColors->setToolTip(tr("Number of colors to be put into the new list of colors.\nMinimum number of colors is 5, maximum is 256. White and black are always colors 0 and 1.\nDo not use too many colors, because some colors are needed internally for anti-aliasing and 256 colors is the maximum."));
selNumberOfPathPoints->setToolTip(tr("Number of points to use as a direction for generating the spectrum\nOnly used if Custom-spectrum is to be defined"));
selNumberOfCurPathPoint->setToolTip(tr("Color point in the path that is to be edited\nOnly used if Custom-spectrum is to be defined"));
selPathCol->setToolTip(tr("Select color for current path-point\nOnly used if Custom-spectrum is to be defined"));

    init();
    colorsel->setCurrentIndex(0);

    QPushButton * emptyButton=new QPushButton("",this);
    emptyButton->setDefault(true);
    emptyButton->hide();
    curColorChanged(0);
    curSpectrumChanged(0);
}

void frmColorManagement::doInvertColors(void)
{
reverse_video();
ReqUpdateColorSel=TRUE;
mainWin->mainArea->completeRedraw();
init();
set_dirtystate();
}

void frmColorManagement::generateSpectrum(int nr)
{
    int counter=2;
    if (path_positions!=NULL) delete[] path_positions;
    switch (nr)
    {
    case STD_COL_SPECTRUM:
        spectrum_path_length=5;
        path_positions=new int[spectrum_path_length];
        path_positions[0]=4;
        path_positions[1]=9;//7;
        path_positions[2]=3;//2;
        path_positions[3]=5;//3;
        path_positions[4]=2;//1;
        break;
    case BW_SPECTRUM:
        spectrum_path_length=2;
        path_positions=new int[spectrum_path_length];
        path_positions[0]=1;//0;
        path_positions[1]=0;//6;
        break;
    case COLD_HOT_SPECTRUM:
        spectrum_path_length=2;
        path_positions=new int[spectrum_path_length];
        path_positions[0]=4;
        path_positions[1]=2;//1;
        break;
    case GOOD_BAD_SPECTRUM:
        spectrum_path_length=3;
        path_positions=new int[spectrum_path_length];
        path_positions[0]=3;
        path_positions[1]=5;
        path_positions[2]=2;
        break;
    default:
    case CUSTOM_SPECTRUM:
        spectrum_path_length=tmp_path_length;
        path_positions=new int[tmp_path_length];
        memcpy(path_positions,tmp_path_pos,sizeof(int)*tmp_path_length);
        break;
    }
    if (chkInvert->isChecked()==true)
    {
        for (int i=0;i<spectrum_path_length/2;i++)
        {
        counter=path_positions[i];
        path_positions[i]=path_positions[spectrum_path_length-1-i];
        path_positions[spectrum_path_length-1-i]=counter;
        }
    }
    temp_spec_length=selNumberOfColors->value();
    int segments=spectrum_path_length-1;
    int points_per_segment=(temp_spec_length-2)/segments;
    int points_last_segment=temp_spec_length-2-(segments-1)*points_per_segment;
    if (temp_spec!=NULL) delete[] temp_spec;
    temp_spec=new CMap_entry[temp_spec_length];
    memcpy(temp_spec+0,base_colors+6,sizeof(CMap_entry));//white
    memcpy(temp_spec+1,base_colors+0,sizeof(CMap_entry));//black
    CMap_entry entry;
    memcpy(&entry,base_colors+6,sizeof(CMap_entry));//copy white as default
    counter=2;
    char col_dummy[64];
    double z=0.0;
    int r0,r1,g0,g1,b0,b1;
    /*
cout << "temp_spec_length=" << temp_spec_length << endl;
cout << "segments=" << segments << endl;
cout << "points_per_segment=" << points_per_segment << endl;
cout << "points_last_segment=" << points_last_segment << endl;
cout << "count_ziel=" << (segments-1)*points_per_segment+points_last_segment+2 << endl;
*/
    double z0=0.0,z1=1.0,dz=1.0/(points_per_segment);
    QColor tmp_color;
    for (int s=0;s<segments-1;s++)//go through all segments except the last one
    {
        tmp_color=get_Color(path_positions[s]);
        /*r0=base_colors[path_positions[s]].rgb.red;
        g0=base_colors[path_positions[s]].rgb.green;
        b0=base_colors[path_positions[s]].rgb.blue;*/
        r0=tmp_color.red();
        g0=tmp_color.green();
        b0=tmp_color.blue();
        tmp_color=get_Color(path_positions[s+1]);
        /*r1=base_colors[path_positions[s+1]].rgb.red;
        g1=base_colors[path_positions[s+1]].rgb.green;
        b1=base_colors[path_positions[s+1]].rgb.blue;*/
        r1=tmp_color.red();
        g1=tmp_color.green();
        b1=tmp_color.blue();
        //cout << "{" << r0 << "|" << g0 << "|" << b0 << "}-->{" << r1 << "|" << g1 << "|" << b1 << "}" << endl;
        for (z=z0;z<z1;z+=dz)
        {
            entry.rgb.red=int(r0*(1.0-z)+r1*z);
            entry.rgb.green=int(g0*(1.0-z)+g1*z);
            entry.rgb.blue=int(b0*(1.0-z)+b1*z);
            /*entry.cname=new char[16];
            sprintf(entry.cname,"Color %d",counter);*/
            entry.cname=NULL;
            sprintf(col_dummy,"Color %d",counter);
            entry.cname = copy_string(entry.cname,col_dummy);
            memcpy(temp_spec+counter,&entry,sizeof(CMap_entry));
            counter++;
        }
    }
    //last segment
    /*r0=base_colors[path_positions[segments-1]].rgb.red;
    g0=base_colors[path_positions[segments-1]].rgb.green;
    b0=base_colors[path_positions[segments-1]].rgb.blue;
    r1=base_colors[path_positions[segments]].rgb.red;
    g1=base_colors[path_positions[segments]].rgb.green;
    b1=base_colors[path_positions[segments]].rgb.blue;*/
    tmp_color=get_Color(path_positions[segments-1]);
    r0=tmp_color.red();
    g0=tmp_color.green();
    b0=tmp_color.blue();
    tmp_color=get_Color(path_positions[segments]);
    r1=tmp_color.red();
    g1=tmp_color.green();
    b1=tmp_color.blue();
    points_last_segment=(segments-1)*points_per_segment+points_last_segment+2-counter;
    dz=1.0/(points_last_segment-1.0);
    for (z=0.0;z<=1.0;z+=1.0/(points_last_segment-1.0))
    {
        entry.rgb.red=int(r0*(1.0-z)+r1*z);
        entry.rgb.green=int(g0*(1.0-z)+g1*z);
        entry.rgb.blue=int(b0*(1.0-z)+b1*z);
        /*entry.cname=new char[16];
        sprintf(entry.cname,"Color %d",counter);*/
        entry.cname=NULL;
        sprintf(col_dummy,"Color %d",counter);
        entry.cname = copy_string(entry.cname,col_dummy);
        memcpy(temp_spec+counter,&entry,sizeof(CMap_entry));
        counter++;
        if (counter==temp_spec_length) break;
    }
    //cout << "counter0=" << counter << endl;
    while (counter<temp_spec_length)
    {
        entry.rgb.red=int(r1);
        entry.rgb.green=int(g1);
        entry.rgb.blue=int(b1);
        /*entry.cname=new char[16];
        sprintf(entry.cname,"Color %d",counter);*/
        entry.cname=NULL;
        sprintf(col_dummy,"Color %d",counter);
        entry.cname = copy_string(entry.cname,col_dummy);
        memcpy(temp_spec+counter,&entry,sizeof(CMap_entry));
        counter++;
    }
    //cout << "counter1=" << counter << endl;
    /*
    if (chkInvert->isChecked()==true)
    {
        //cout << "spektrum invers" << endl;
        for (int i=2;i<(temp_spec_length-2)/2;i++)
        {
            memcpy(&entry,temp_spec+i,sizeof(CMap_entry));
            memcpy(temp_spec+i,temp_spec+(temp_spec_length-i+1),sizeof(CMap_entry));
            memcpy(temp_spec+(temp_spec_length-i+1),&entry,sizeof(CMap_entry));
        }
    }
    */
}

void frmColorManagement::showTempSpectrum(void)
{
    int target_length=2*256;
    int col_length=int(target_length*1.0/temp_spec_length);
    target_length=col_length*temp_spec_length;
    QColor colo;
    QPixmap pix(target_length,20);
    QPainter paint(&pix);
    for (int i=0;i<temp_spec_length;i++)
    {
        colo=QColor(temp_spec[i].rgb.red,temp_spec[i].rgb.green,temp_spec[i].rgb.blue);
        paint.setPen(colo);
        paint.setBrush(colo);
        paint.fillRect(i*col_length,0,col_length,20,colo);
    }
    paint.setBrush(Qt::NoBrush);
    paint.setPen(Qt::black);
    paint.drawLine(0,0,target_length,0);
    paint.drawLine(0,19,target_length,19);
    paint.drawLine(0,0,0,19);
    paint.drawLine(target_length-1,0,target_length-1,19);
    paint.end();
    lblIllustration2->setPixmap(pix);
}

void frmColorManagement::doAddColor(void)
{
    CMap_entry cmap=constructColor();
    addColorToLocal(&cmap);
    //cout << "add new color: " << map_entries-1 << endl;
    cur_col_num=map_entries-1;
    colorsel->updateColorIcons(allocated_loc_colors,locColorPixmaps,locColorNames);
    colorsel->alphaSelector->hide();
    colorsel->cmbColorSelect->panels->alphaSlider->hide();
    char dummy[8];
    sprintf(dummy,"%d",cur_col_num);
    lblColNumber->setText(tr("Number = ")+QString(dummy));
    //colorsel->update_color_panels();//wrong--> this would update the selector with the current palette not the new one!
}

void frmColorManagement::doDelColor(void)
{
    //cout << "delete current color: " << cur_col_num << endl;
    //delete_color(cur_col_num);
    delColorToLocal(cur_col_num);
    //init_color_icons();
    colorsel->updateColorIcons(allocated_loc_colors,locColorPixmaps,locColorNames);
    colorsel->setCurrentIndex(0);
    colorsel->alphaSelector->hide();
    colorsel->cmbColorSelect->panels->alphaSlider->hide();
    init_with_color(0);
}

void frmColorManagement::doEditColor(void)
{
    CMap_entry cmap=constructColor();
    editColorToLocal(&cmap,cur_col_num);
    //cout << "edit current color" << endl;
    //memcpy(&(cmap_table[cur_col_num].rgb),&(cmap.rgb),sizeof(RGB));
    //copy_string(cmap_table[cur_col_num].cname,cmap.cname);
    colorsel->updateColorIcons(allocated_loc_colors,locColorPixmaps,locColorNames);
    colorsel->setCurrentIndex(cur_col_num);
    colorsel->alphaSelector->hide();
    colorsel->cmbColorSelect->panels->alphaSlider->hide();
}

void frmColorManagement::doSelColor(void)
{
    //cout << "select a new color" << endl;
    CMap_entry entry=constructColor();
    QColor col(entry.rgb.red,entry.rgb.green,entry.rgb.blue);
    QColor newCol=colDial->getColor(col,this);
    //cout << newCol.red() << " " << newCol.green() << " " << newCol.blue() << endl;
    GlobalInhibitor=true;
    ledRed->setValue(newCol.red());
    ledGreen->setValue(newCol.green());
    ledBlue->setValue(newCol.blue());
    GlobalInhibitor=false;
    showSingleColor();
}

void frmColorManagement::doSetStdColors(void)
{
    for (int i=0;i<map_entries;i++)
    {
        local_cmap_table[i].cname = copy_string(local_cmap_table[i].cname,NULL);
        //delete[] local_cmap_table[i].cname;
    }
    delete[] local_cmap_table;
    map_entries=sizeof(cmap_init)/sizeof(CMap_entry);
    //cout << "set std colors: " << map_entries << endl;
    local_cmap_table=new CMap_entry[map_entries];
    for (int i=0;i<map_entries;i++)
    {
        memcpy(local_cmap_table+i,cmap_init+i,sizeof(CMap_entry));
        /*local_cmap_table[i].cname=new char[2+strlen(cmap_init[i].cname)];
        strcpy(local_cmap_table[i].cname,cmap_init[i].cname);*/
        local_cmap_table[i].cname = NULL;
        local_cmap_table[i].cname = copy_string(local_cmap_table[i].cname,cmap_init[i].cname);
    }
    init_color_icons(map_entries,local_cmap_table,allocated_loc_colors,&locColorIcons,&locColorPixmaps,&locColorNames);
    colorsel->updateColorIcons(map_entries,locColorPixmaps,locColorNames);
    colorsel->alphaSelector->hide();
    colorsel->cmbColorSelect->panels->alphaSlider->hide();
}

void frmColorManagement::doSetSpectrum(void)
{
    //cout << "set spectrum" << endl;
    generateSpectrum(selStdSpectra->currentIndex());
    showTempSpectrum();
    for (int i=0;i<map_entries;i++)
    {
        local_cmap_table[i].cname = copy_string(local_cmap_table[i].cname,NULL);
        //delete[] local_cmap_table[i].cname;
    }
    delete[] local_cmap_table;
    map_entries=temp_spec_length;
    local_cmap_table=new CMap_entry[map_entries];
    for (int i=0;i<map_entries;i++)
    {
        memcpy(local_cmap_table+i,temp_spec+i,sizeof(CMap_entry));
        /*local_cmap_table[i].cname=new char[2+strlen(temp_spec[i].cname)];
        strcpy(local_cmap_table[i].cname,temp_spec[i].cname);*/
        local_cmap_table[i].cname = NULL;
        local_cmap_table[i].cname = copy_string(local_cmap_table[i].cname,temp_spec[i].cname);
    }
    init_color_icons(map_entries,local_cmap_table,allocated_loc_colors,&locColorIcons,&locColorPixmaps,&locColorNames);
    colorsel->updateColorIcons(map_entries,locColorPixmaps,locColorNames);
    colorsel->alphaSelector->hide();
    colorsel->cmbColorSelect->panels->alphaSlider->hide();
    QMessageBox::information(this,tr("Warning"),tr("Local color palette changed. Check palette at top of dialog.\nIf you want to change the palette for the whole project please press Apply or Accept at bottom of dialog."));
}

void frmColorManagement::curColorChanged(int nr)
{
    if (GlobalInhibitor==true) return;
    //cout << "cur color changed --> " << nr << endl;
    init_with_color(nr);
    showSingleColor();
}

void frmColorManagement::curSpectrumChanged(int nr)
{
    if (GlobalInhibitor==true) return;
    //cout << "spectrum changed" << endl;
    generateSpectrum(nr);
    showTempSpectrum();
    if (nr==CUSTOM_SPECTRUM)
    {
    selNumberOfPathPoints->setEnabled(true);
    selNumberOfCurPathPoint->setEnabled(true);
    selPathCol->setEnabled(true);
    }
    else
    {
    selNumberOfPathPoints->setEnabled(false);
    selNumberOfCurPathPoint->setEnabled(false);
    selPathCol->setEnabled(false);
    }
}

void frmColorManagement::curNumberOfColorsChanged(int nr)
{
    (void)nr;
    if (GlobalInhibitor==true) return;
    //cout << "color count changed " << nr << endl;
    curSpectrumChanged(selStdSpectra->currentIndex());
}

void frmColorManagement::changedInvert(bool nr)
{
    (void)nr;
    curSpectrumChanged(selStdSpectra->currentIndex());
}

void frmColorManagement::colorCompositionChanged(int nr)
{
    (void)nr;
    if (GlobalInhibitor==true) return;
    //cout << "color composition changed" << endl;
    showSingleColor();
}

void frmColorManagement::colorNameChanged(void)
{
    if (GlobalInhibitor==true) return;
    //cout << "color name changed" << endl;
}

void frmColorManagement::showSingleColor(void)
{
    CMap_entry entry=constructColor();
    QColor colo(entry.rgb.red,entry.rgb.green,entry.rgb.blue);
    QPixmap pix(300,20);
    QPainter paint(&pix);
    paint.setPen(colo);
    paint.setBrush(colo);
    paint.fillRect(0,0,300,20,colo);
    paint.end();
    lblIllustration->setPixmap(pix);
}

void frmColorManagement::addColorToLocal(CMap_entry * entry)
{
    //cout << "old map_entries=" << map_entries << endl;
    CMap_entry * n_table=new CMap_entry[map_entries+1];
    memcpy(n_table,local_cmap_table,sizeof(CMap_entry)*map_entries);
    memcpy(n_table+map_entries,entry,sizeof(CMap_entry));
    delete[] local_cmap_table;
    local_cmap_table=n_table;
    map_entries++;
    //cout << "new map_entries=" << map_entries << endl;
    init_color_icons(map_entries,local_cmap_table,allocated_loc_colors,&locColorIcons,&locColorPixmaps,&locColorNames);
}

void frmColorManagement::editColorToLocal(CMap_entry * entry,int nr)
{
    memcpy(local_cmap_table+nr,entry,sizeof(CMap_entry));
    init_color_icons(map_entries,local_cmap_table,allocated_loc_colors,&locColorIcons,&locColorPixmaps,&locColorNames);
}

void frmColorManagement::delColorToLocal(int nr)
{
    CMap_entry * n_table=new CMap_entry[map_entries-1];
    int counter=0;
    for (int i=0;i<map_entries;i++)
    {
        if (i!=nr)
        {
            memcpy(n_table+counter,local_cmap_table+i,sizeof(CMap_entry));
            counter++;
        }
    }
    delete[] local_cmap_table;
    local_cmap_table=n_table;
    map_entries--;
    init_color_icons(map_entries,local_cmap_table,allocated_loc_colors,&locColorIcons,&locColorPixmaps,&locColorNames);
}

void frmColorManagement::NrOfPathPointsChanged(int nr)
{
    int * tmp_pointer=new int[nr];
    if (nr<tmp_path_length)
    {
        memcpy(tmp_pointer,tmp_path_pos,sizeof(int)*nr);
        delete[] tmp_path_pos;
        tmp_path_pos=tmp_pointer;
    }
    else//nr>=tmp_path_length
    {
        memcpy(tmp_pointer,tmp_path_pos,sizeof(int)*tmp_path_length);
        for (int i=tmp_path_length;i<nr;i++)
            tmp_pointer[i]=tmp_pointer[tmp_path_length-1];
        delete[] tmp_path_pos;
        tmp_path_pos=tmp_pointer;
    }
    tmp_path_length=nr;
    selNumberOfCurPathPoint->spnInt->setMaximum(nr-1);
    selNumberOfColors->spnInt->setMinimum(nr+2);
    if (selStdSpectra->currentIndex()==CUSTOM_SPECTRUM)
    {
        curSpectrumChanged(CUSTOM_SPECTRUM);
    }
}

void frmColorManagement::CurrentPathPointChanged(int nr)
{
    tmp_path_pos[tmp_path_point_shown]=selPathCol->currentIndex();
    selPathCol->setCurrentIndex(tmp_path_pos[nr]);
    tmp_path_point_shown=nr;
}

void frmColorManagement::CurrentPathColorChanged(int nr)
{
    tmp_path_pos[tmp_path_point_shown]=nr;
    if (selStdSpectra->currentIndex()==CUSTOM_SPECTRUM)
    {
        curSpectrumChanged(CUSTOM_SPECTRUM);
    }
}

CMap_entry frmColorManagement::constructColor(void)
{
    CMap_entry entry;
    entry.rgb.red=ledRed->value();
    entry.rgb.blue=ledBlue->value();
    entry.rgb.green=ledGreen->value();
    entry.ctype=COLOR_MAIN;
    entry.tstamp=0;
    entry.cname = NULL;
    entry.cname = copy_string(entry.cname,ledColName->text().toLocal8Bit().constData());
    /*entry.cname=new char[ledColName->text().length()+2];
    strcpy(entry.cname,ledColName->text().toLocal8Bit().constData());*/
    return entry;
}

void frmColorManagement::init_with_color(int nr)
{
    GlobalInhibitor=true;
    cur_col_num=nr;
    CMap_entry * entry=local_cmap_table+nr;
    ledRed->setValue(entry->rgb.red);
    ledBlue->setValue(entry->rgb.blue);
    ledGreen->setValue(entry->rgb.green);
        if (entry->cname)
        ledColName->setText(QString(entry->cname));
        else
        ledColName->setText(QString(""));
    char dummy[8];
    sprintf(dummy,"%d",nr);
    lblColNumber->setText(tr("Number = ")+QString(dummy));
    GlobalInhibitor=false;
}

void frmColorManagement::init(void)
{
    int * real_colors=new int[4];
    int nr_of_aux_cols;
    //qDebug() << "A init: " << map_entries;
    if (local_cmap_table!=NULL)
    {
        for (int i=0;i<map_entries;i++)
        {
            local_cmap_table[i].cname = copy_string(local_cmap_table[i].cname,NULL);
            //delete[] local_cmap_table[i].cname;
        }
        delete[] local_cmap_table;
    }
    map_entries=get_main_color_indices(&real_colors,&nr_of_aux_cols);
    //qDebug() << "B init: " << map_entries;
    local_cmap_table=new CMap_entry[map_entries];
    for (int i=0;i<map_entries;i++)
    {
        memcpy(local_cmap_table+i,cmap_table+real_colors[i],sizeof(CMap_entry));
        /*local_cmap_table[i].cname=new char[strlen(cmap_table[real_colors[i]].cname)+2];
        strcpy(local_cmap_table[i].cname,cmap_table[real_colors[i]].cname);*/
        local_cmap_table[i].cname = NULL;
        local_cmap_table[i].cname = copy_string(local_cmap_table[i].cname,cmap_table[real_colors[i]].cname);
    }
    init_color_icons(map_entries,local_cmap_table,allocated_loc_colors,&locColorIcons,&locColorPixmaps,&locColorNames);
    //Local colors are only main colors
    colorsel->updateColorIcons(map_entries,locColorPixmaps,locColorNames);
    colorsel->alphaSelector->hide();
    colorsel->cmbColorSelect->panels->alphaSlider->hide();
}

void frmColorManagement::doApply(void)
{
    int ret=QMessageBox::question(this,tr("Change color palette?"),tr("Do you really want to change the current color palette?"),QMessageBox::Ok,QMessageBox::Cancel);
    if (ret==QMessageBox::Ok)
    {
        //cout << "apply to color palette" << endl;
        SaveCurrentColorMap();
        //first: save aux colors
        int * real_colors=new int[4];
        int nr_of_aux_colors;//,new_nr_of_colors;
        (void)get_main_color_indices(&real_colors,&nr_of_aux_colors);
        //int real_map_entries=get_main_color_indices(&real_colors,&nr_of_aux_colors);
        int * aux_colors=new int[2+nr_of_aux_colors];//the old color-indices of auxilliary colors
        CMap_entry * aux_map_entries=new CMap_entry[2+nr_of_aux_colors];//the auxilliary colors
        //cout << "nr_of_aux_colors=" << nr_of_aux_colors << endl;
        nr_of_aux_colors=0;
        for (unsigned int i=0;i<number_of_colors();i++)
        {
            if (cmap_table[i].ctype!=COLOR_MAIN)
            {
                memcpy(aux_map_entries+nr_of_aux_colors,cmap_table+i,sizeof(CMap_entry));
                /*aux_map_entries[nr_of_aux_colors].cname=new char[2+sizeof(cmap_table[i].cname)];
                strcpy(aux_map_entries[nr_of_aux_colors].cname,cmap_table[i].cname);*/
                aux_map_entries[nr_of_aux_colors].cname = NULL;
                aux_map_entries[nr_of_aux_colors].cname = copy_string(aux_map_entries[nr_of_aux_colors].cname,cmap_table[i].cname);
                aux_colors[nr_of_aux_colors]=i;
                //cout << i << endl;
                nr_of_aux_colors++;
            }
        }
        /// the following two commands seem to be important for the t1-fonts
        nr_of_aux_colors=0;
        AAGrayLevelsOK = FALSE;
        /// we leave them in here because otherwise the grayscale-colors are not stored with the correct indices --> at the next redraw the grayscale colors will be reinstalled
        int number_of_new_cols=nr_of_aux_colors+map_entries;//aux-colors+(new)main-colors
        if (number_of_new_cols>MAXCOLORS) number_of_new_cols=MAXCOLORS;
        //cout << "nr_of_aux_colors=" << nr_of_aux_colors << " number_of_new_cols=" << number_of_new_cols << endl;
        realloc_colors(number_of_new_cols);
        int aux_counter=0,colors_counter=0;
        char * old_cname;
        for (int i=0;i<number_of_new_cols;i++)//fill new color table with new colors
        {
            if (aux_counter>=nr_of_aux_colors)//space for a main color
            {
                old_cname=cmap_table[i].cname;
                memcpy(cmap_table+i,local_cmap_table+colors_counter,sizeof(CMap_entry));
                cmap_table[i].cname = old_cname;
                cmap_table[i].cname = copy_string(cmap_table[i].cname, local_cmap_table[colors_counter].cname);
                colors_counter++;
            }
            else if (aux_colors[aux_counter]==i)//current number has been an aux-color
            {
                old_cname=cmap_table[i].cname;
                memcpy(cmap_table+i,aux_map_entries+aux_counter,sizeof(CMap_entry));
                cmap_table[i].cname = old_cname;
                cmap_table[i].cname = copy_string(cmap_table[i].cname, aux_map_entries[aux_counter].cname);
                aux_counter++;
            }
            else//space for a main color
            {
                old_cname=cmap_table[i].cname;
                memcpy(cmap_table+i,local_cmap_table+colors_counter,sizeof(CMap_entry));
                cmap_table[i].cname = old_cname;
                cmap_table[i].cname = copy_string(cmap_table[i].cname, local_cmap_table[colors_counter].cname);
                colors_counter++;
            }
        }
        ReqUpdateColorSel = TRUE;
        /*IntstallColorMap(map_entries,local_cmap_table);*/
        CurrentColorMapChanged();
        mainWin->mainArea->completeRedraw();
    }
    //else
    //cout << "do not change color palette" << endl;
}

void frmColorManagement::doAccept(void)
{
    ApplyError=false;
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmColorManagement::doClose(void)
{
    emit(close_wish());
    //hide();
}


frmUserDefinedFormats::frmUserDefinedFormats(QWidget * parent):QWidget(parent)
{
cur_Format=cur_field=-1;//invalid selection
int nr=2;
QString entries[4];
entries[0]=tr("Date and Time");
entries[1]=tr("None");
grpFormats=new QGroupBox(tr("User Defined Formats"),this);
selNrOfFormats=new stdIntSelector(grpFormats,tr("Nr of User Defined Formats:"),0,10);
selNrOfFormats->setValue(2);
selFormat=new StdSelector(grpFormats,tr("Select format:"),nr,entries);
cmdRemove=new QPushButton(tr("Remove"),grpFormats);
cmdUp=new QPushButton(tr("Up"),grpFormats);
cmdDown=new QPushButton(tr("Down"),grpFormats);
layout0=new QGridLayout(grpFormats);
layout0->setSpacing(STD_SPACING);
//layout0->setMargin(STD_MARGIN);
layout0->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
layout0->addWidget(selNrOfFormats,0,0,1,3);
layout0->addWidget(selFormat,1,0,1,3);
layout0->addWidget(cmdUp,2,0,1,1);
layout0->addWidget(cmdDown,2,1,1,1);
layout0->addWidget(cmdRemove,2,2,1,1);
grpFormats->setLayout(layout0);

grpEdit=new QGroupBox(tr("Edit Format"),this);
ledName=new stdLineEdit(grpEdit,tr("Name of format:"));
nr=3;
entries[0]=tr("Date and Time");
entries[1]=tr("Angle");
entries[3]=tr("Coordinates");
selGeneralType=new StdSelector(grpEdit,tr("General format type:"),nr,entries);
lblFields=new QLabel(tr("Fields in format (parts of the format):"),grpEdit);
cmdEdit=new QPushButton(tr("Edit"),grpEdit);
cmdPlus=new QPushButton(QString("+"),grpEdit);
cmdMinus=new QPushButton(QString("-"),grpEdit);
scroll=new QScrollArea(grpEdit);
empty=new QWidget(scroll);

fieldType=NULL;
EditField=NULL;

subLayout=new QGridLayout(empty);
subLayout->setSpacing(0);
//subLayout->setMargin(0);
subLayout->setContentsMargins(0,0,0,0);
empty->setLayout(subLayout);
scroll->setWidget(empty);

lenInputTest=new stdLineEdit(grpEdit,tr("Input test:"));
selOutputTest=new LineWidthSelector(grpEdit);
selOutputTest->lblText->setText(tr("Output test:"));

lblInputTest=new QLabel(QString("---"),grpEdit);
lblOutputTest=new QLabel(QString("---"),grpEdit);
#if QT_VERSION >= 0x050000

#else
mapper1=new QSignalMapper(this);
mapper2=new QSignalMapper(this);
#endif
layout1=new QGridLayout();
layout1->setSpacing(STD_SPACING);
//layout1->setMargin(STD_MARGIN);
layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
layout1->addWidget(ledName,0,0,1,3);
layout1->addWidget(lblFields,1,0,1,1);
layout1->addWidget(selGeneralType,1,2,1,1);
layout1->addWidget(scroll,2,0,2,3);
layout1->addWidget(cmdPlus,4,0,1,1);
layout1->addWidget(cmdMinus,4,1,1,1);
layout1->addWidget(cmdEdit,4,2,1,1);
layout1->addWidget(lenInputTest,5,0,1,2);
layout1->addWidget(lblInputTest,5,2,1,1);
layout1->addWidget(selOutputTest,6,0,1,2);
layout1->addWidget(lblOutputTest,6,2,1,1);
grpEdit->setLayout(layout1);

grpDetails=new QGroupBox(tr("Field Details"),this);
layout2=new QGridLayout(grpDetails);
layout2->setSpacing(STD_SPACING);
//layout2->setMargin(STD_MARGIN);
layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);

grpDetails->setLayout(layout2);

layout=new QGridLayout(this);
layout->setSpacing(STD_SPACING);
//layout->setMargin(STD_MARGIN);
layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
layout->addWidget(grpFormats,0,0);
layout->addWidget(grpEdit,1,0);
layout->addWidget(grpDetails,2,0);
layout->setRowStretch(0,1);
layout->setRowStretch(1,1);
layout->setRowStretch(2,5);
//stdButtonGroup * buttons;
this->setLayout(layout);
}

void frmUserDefinedFormats::doEdit(void)
{}

void frmUserDefinedFormats::doRemove(void)
{}

void frmUserDefinedFormats::doUp(void)
{}

void frmUserDefinedFormats::doDown(void)
{}

void frmUserDefinedFormats::doPlus(void)
{}

void frmUserDefinedFormats::doMinus(void)
{}

void frmUserDefinedFormats::doMapper1(int nr)
{
    (void) nr;
}

void frmUserDefinedFormats::doMapper2(int nr)
{
    (void) nr;
}

frmPreferences::frmPreferences(QWidget * parent):QWidget(parent)
{
    int number;
    QString entr[6>nr_of_translations?6:nr_of_translations];
//setFont(*stdFont);
    setWindowTitle(tr("QtGrace: Preferences"));
    setWindowIcon(QIcon(*GraceIcon));

    grpResponciveness=new QGroupBox(tr("Responsiveness"),this);
    noask_item=new QCheckBox(tr("Don't ask questions"),grpResponciveness);
    dc_item=new QCheckBox(tr("Allow double clicks on canvas"),grpResponciveness);
    dc_item->setChecked(TRUE);
    number=3;
    entr[0]=tr("Button press");
    entr[1]=tr("As set");
    entr[2]=tr("Follow mouse");
    graph_focus_choice_item=new StdSelector(grpResponciveness,tr("Graph focus switch:"),number,entr);
    graph_drawfocus_choice_item=new QCheckBox(tr("Display focus markers"),grpResponciveness);
    graph_drawfocus_choice_item->setChecked(TRUE);
    autoredraw_type_item=new QCheckBox(tr("Auto redraw"),grpResponciveness);
    autoredraw_type_item->setChecked(TRUE);
    cursor_type_item=new QCheckBox(tr("Crosshair cursor"),grpResponciveness);
    ///chkShowHideWorkaround=new QCheckBox(tr("Show/Hide workaround"),grpResponciveness);
    grpRestrictions=new QGroupBox(tr("Restrictions"),this);
    max_path_item=new stdIntSelector(grpRestrictions,tr("Max drawing path length:"),0,10000000);
    max_path_item->spnInt->setSingleStep(1000);
    max_path_item->setValue(20000);
    safe_mode_item=new QCheckBox(tr("Run in safe mode"),grpRestrictions);
    safe_mode_item->setChecked(TRUE);
    grpScrollZoom=new QGroupBox(tr("Scroll/zoom"),this);
    scrollper_item=new stdSlider(grpScrollZoom,tr("Scroll %"),0,200);
    scrollper_item->setValue(5);
    shexper_item=new stdSlider(grpScrollZoom,tr("Zoom %"),0,200);
    shexper_item->setValue(5);
    linkscroll_item=new QCheckBox(tr("Linked scrolling"),grpScrollZoom);
    grpDates=new QGroupBox(tr("Dates"),this);
    number=4;
    entr[0]=tr("ISO");
    entr[1]=tr("European");
    entr[2]=tr("US");
    entr[3]=tr("None");
    hint_item=new StdSelector(grpDates,tr("Date hint:"),number,entr);
    hint_item->setCurrentIndex(3);
    date_item=new stdLineEdit(grpDates,tr("Reference date:"));
    date_item->lenText->setText(QString("-4713-01-01 12:00:00"));
    number=3;
    entr[0]=tr("-");
    entr[1]=tr(".");
    entr[2]=tr("/");
    date_sep_item=new StdSelector(grpDates,tr("Date-separator:"),number,entr);
    date_sep_item->setCurrentIndex(0);
    date_sep_item->setToolTip(tr("Which separator-character is used between day, month and year"));
    wrap_year_item=new stdLineEdit(grpDates,tr("Wrap year:"));
    char dummy[5];
    strcpy(dummy,"1950");
    wrap_year_item->setText(QString(dummy));
    wrap_year_item->setEnabled(FALSE);
    two_digits_years_item=new QCheckBox(tr("Two-digit year span"),grpDates);
    connect(two_digits_years_item,SIGNAL(stateChanged(int)),SLOT(toggleWrapYear(int)));

    buttonGroup=new stdButtonGroup(this,true,true,false);//,true);
    //buttonGroup->cmdHelp->setText(QString("Extra"));
    //connect(buttonGroup->cmdHelp,SIGNAL(clicked()),this,SLOT(doExtraPreferences()));
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));
    ///buttonGroup->cmdHelp->hide();
    /*
for (int i=0;i<nr_of_translations;i++)
{
entr[i]=QString(translator_languages[i]);
}
selLanguage=new StdSelector(this,tr("Language:"),nr_of_translations,entr);
chkExternalHelpViewer=new QCheckBox(tr("Show help-files in external html-Viewer"),this);
*/

noask_item->setToolTip(tr("If this is activated, then every question is automatically answered by \"yes\"."));
dc_item->setToolTip(tr("Open dialogs on context when double clicking on the plot-area (sensitive to the graph currently selected)"));
graph_focus_choice_item->setToolTip(tr("How to switch between graphs"));
graph_drawfocus_choice_item->setToolTip(tr("Draw small squares on the graph edges to inidcate which graph is currently selected"));
autoredraw_type_item->setToolTip(tr("This option is currently useless in QtGrace! AutoRedraw is always on!"));
cursor_type_item->setToolTip(tr("Draw a crosshair over the whole plot on the cursor"));
max_path_item->setToolTip(tr("Maximum number of points plotted in a row (larger sets are plotted in a simplified representation)"));
safe_mode_item->setToolTip(tr("File modifications are disabled in safe mode when using parser-commands (GUI-operations are unaffected)."));
scrollper_item->setToolTip(tr("How far to move the axis if scoll buttons are pressed"));
shexper_item->setToolTip(tr("How far to zoom in and out on clicking a zoom button"));
linkscroll_item->setToolTip(tr("This option is currently useless in QtGrace! Use the Graph-selector in the toolbar for this!"));
hint_item->setToolTip(tr("Used as a default date format setting for cells where dates have to be entered"));
date_item->setToolTip(tr("Used internally to convert double-values to dates"));
wrap_year_item->setToolTip(tr("Year to be used to convert two-digit-years into four-digit years (and vice versa)"));
two_digits_years_item->setToolTip(tr("Use this to allow display of years in two-digit-format (instead of four digits)"));

    layout0=new QVBoxLayout();
    //layout0->setMargin(STD_MARGIN);
    layout0->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout0->addWidget(noask_item);
    layout0->addWidget(dc_item);
    layout0->addWidget(graph_focus_choice_item);
    layout0->addWidget(graph_drawfocus_choice_item);
    layout0->addWidget(autoredraw_type_item);
    layout0->addWidget(cursor_type_item);
    ///layout0->addWidget(chkShowHideWorkaround);
    grpResponciveness->setLayout(layout0);
    layout1=new QVBoxLayout();
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout1->addWidget(max_path_item);
    layout1->addWidget(safe_mode_item);
    grpRestrictions->setLayout(layout1);
    layout2=new QVBoxLayout();
    //layout2->setMargin(STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout2->addWidget(scrollper_item);
    layout2->addWidget(shexper_item);
    layout2->addWidget(linkscroll_item);
    grpScrollZoom->setLayout(layout2);
    grpScrollZoom->setToolTip(tr("The Scroll/zoom-settings are project-specific."));
    layout3=new QGridLayout();
    //layout3->setMargin(STD_MARGIN);
    layout3->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout3->addWidget(hint_item,0,0,1,2);
    layout3->addWidget(date_item,1,0,1,2);
    layout3->addWidget(two_digits_years_item,2,0);
    layout3->addWidget(wrap_year_item,2,1);
    layout3->addWidget(date_sep_item,3,0,1,2);
    grpDates->setLayout(layout3);
    grpDates->setToolTip(tr("The Dates-settings are project-specific."));
    layout=new QVBoxLayout();
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->setSpacing(STD_SPACING);
    layout->addWidget(grpResponciveness);
    layout->addWidget(grpRestrictions);
    layout->addWidget(grpScrollZoom);
    layout->addWidget(grpDates);
    layout->addStretch(3);
    layout->addWidget(buttonGroup);

    /*layout->setStretch(0,1);
    layout->setStretch(1,1);
    layout->setStretch(2,1);
    layout->setStretch(3,1);
    layout->setStretch(4,0);*/

    setLayout(layout);
}

void frmPreferences::doExtraPreferences(void)
{
    /*if (ExtraPreferences==NULL)
    {
        ExtraPreferences=new frmExtraPreferences(0);
        ExtraPreferences->init();
    }
    ExtraPreferences->show();
    ExtraPreferences->raise();
    ExtraPreferences->activateWindow();*/
}

void frmPreferences::update_props_items(void)
{
    int itest = 0;
    int iv;
    int y, m, d, h, mm, sec;
    char date_string[64], wrap_year_string[64];

    //if (props_frame) {
#ifdef DEBUG
    if (get_debuglevel() > 8) {
        errwin(tr("Debug level > 8, resetting to 0").toLocal8Bit().constData());
        set_debuglevel(0);
    }
    SetSpinChoice(debug_item, (double) get_debuglevel());
#endif
    SetToggleButtonState(noask_item, noask);
    SetToggleButtonState(dc_item, allow_dc);

    if (focus_policy == FOCUS_SET) {
        itest = 1;
    } else if (focus_policy == FOCUS_CLICK) {
        itest = 0;
    } else if (focus_policy == FOCUS_FOLLOWS) {
        itest = 2;
    }
    SetChoice(graph_focus_choice_item, itest);
    SetToggleButtonState(graph_drawfocus_choice_item, draw_focus_flag);

    SetToggleButtonState(linkscroll_item, scrolling_islinked);
    SetToggleButtonState(autoredraw_type_item, auto_redraw);
    SetToggleButtonState(cursor_type_item, cursortype);
    ///SetToggleButtonState(chkShowHideWorkaround, showhideworkaround);
#if defined WITH_XMHTML || defined WITH_LIBHELP
    SetToggleButtonState(force_external_viewer_item, force_external_viewer);
#endif
    /*SetSpinChoice(max_path_item, (double) get_max_path_limit());*/
    max_path_item->setValue(get_max_path_limit());
    SetToggleButtonState(safe_mode_item, safe_mode);
    iv = (int) rint(100*scrollper);
    SetScaleValue(scrollper_item, iv);
    iv = (int) rint(100*shexper);
    SetScaleValue(shexper_item, iv);
    switch (get_date_hint()) {
    case FMT_iso :
        itest = 0;
        break;
    case FMT_european :
        itest = 1;
        break;
    case FMT_us :
        itest = 2;
        break;
    default :
        itest = FMT_nohint;
        break;
    }
    SetChoice(hint_item, itest);
    SetChoice(date_sep_item,current_date_separator);
    jul_to_cal_and_time(0.0, ROUND_SECOND, &y, &m, &d, &h, &mm, &sec);
    sprintf(date_string, "%d-%02d-%02d %02d:%02d:%02d",y, m, d, h, mm, sec);
    xv_setstr(date_item, date_string);
    SetToggleButtonState(two_digits_years_item, two_digits_years_allowed());
    sprintf(wrap_year_string, "%04d", get_wrap_year());
    xv_setstr(wrap_year_item, wrap_year_string);
    SetSensitive(wrap_year_item, two_digits_years_allowed() ? true:false);
    ///selLanguage->setCurrentIndex(current_language);
    //}
}

void frmPreferences::props_define_notify_proc(void)
{
    ApplyError=false;
    double jul;
#ifdef DEBUG
    set_debuglevel((int) GetSpinChoice(debug_item));
#endif
    save_no_ask = noask = GetToggleButtonState(noask_item);
    allow_dc = GetToggleButtonState(dc_item);
    switch (GetChoice(graph_focus_choice_item)) {
    case 0:
        focus_policy = FOCUS_CLICK;
        break;
    case 1:
        focus_policy = FOCUS_SET;
        break;
    case 2:
        focus_policy = FOCUS_FOLLOWS;
        break;
    }

    draw_focus_flag = GetToggleButtonState(graph_drawfocus_choice_item);
    scrolling_islinked = GetToggleButtonState(linkscroll_item);
    auto_redraw = GetToggleButtonState(autoredraw_type_item);
    cursortype = GetToggleButtonState(cursor_type_item);
    ///showhideworkaround= GetToggleButtonState(chkShowHideWorkaround);
#if defined WITH_XMHTML || defined WITH_LIBHELP
    force_external_viewer = GetToggleButtonState(force_external_viewer_item);
#endif
    set_max_path_limit((int) GetSpinChoice(max_path_item));
    safe_mode = GetToggleButtonState(safe_mode_item);
    scrollper = (double) GetScaleValue(scrollper_item)/100.0;
    shexper   = (double) GetScaleValue(shexper_item)/100.0;

    switch (GetChoice(hint_item)) {
    case 0 :
        set_date_hint(FMT_iso);
        break;
    case 1 :
        set_date_hint(FMT_european);
        break;
    case 2 :
        set_date_hint(FMT_us);
        break;
    default :
        set_date_hint(FMT_nohint);
        break;
    }
    current_date_separator=GetChoice(date_sep_item);
    if (parse_date_or_number(xv_getstr(date_item), TRUE, &jul)== RETURN_SUCCESS) {
        set_ref_date(jul);
    } else {
        errmsg(tr("Invalid date").toLocal8Bit().constData());
        ApplyError=true;
    }
    allow_two_digits_years(GetToggleButtonState(two_digits_years_item));
    set_wrap_year(atoi(xv_getstr(wrap_year_item)));
    ///change_language(selLanguage->currentIndex());
//xdrawgraph();
}

void frmPreferences::init(void)
{
    update_props_items();
}

void frmPreferences::toggleWrapYear(int i)
{
    if (i==0)
        wrap_year_item->setEnabled(FALSE);
    else
        wrap_year_item->setEnabled(TRUE);
}

void frmPreferences::doApply(void)
{
    ApplyError=false;
    int save_dirty=dirtystate;
    props_define_notify_proc();
    //mainWin->mainArea->completeRedraw();
    dirtystate=save_dirty;
}

void frmPreferences::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmPreferences::doClose(void)
{
    emit(close_wish());
    //hide();
}

frm_Preferences::frm_Preferences(QWidget * parent):QDialog(parent)
{
//QFont fntPref;
//setFont(*stdFont);
min_w=680;
min_h=680;//630;
bar_w=bar_h=20;

int index=8;
QString * entries=new QString[index];

QVBoxLayout * m_layout=new QVBoxLayout();
//m_layout->setMargin(0);
m_layout->setContentsMargins(0,0,0,0);
m_layout->setSpacing(0);

flp=new QWidget(this);
scroll=new QScrollArea;
m_layout->addWidget(scroll);
scroll->setWidget(flp);

    setWindowTitle(tr("QtGrace: Preferences"));
    setWindowIcon(QIcon(*GraceIcon));

    vbox=new QVBoxLayout();
    //vbox->setMargin(STD_MARGIN);
    vbox->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    vbox->setSpacing(STD_SPACING);

tabs=new QTabWidget(flp);

tab_prefs=new frmPreferences();
tab_prefs->buttonGroup->hide();
tab_linestyles=new tabLinestyles();
tab_colors=new frmColorManagement();
tab_defaults=new frmDefaults();
tab_DefFormats=new frmUserDefinedFormats();
tab_DefFormats->hide();

///GUI
tab_GUI=new QWidget(this);

guiLayout=new QVBoxLayout(this);
//guiLayout->setMargin(STD_MARGIN);
guiLayout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
guiLayout->setSpacing(STD_SPACING);

grp_tool_bar=new QGroupBox(tr("Customize interface"),this);
//lblToolBar=new QLabel(tr("Tool bar contents:"),this);
//lblStatusBar=new QLabel(tr("Status bar contents:"),this);
layout2=new QGridLayout();
layout2->setSpacing(STD_SPACING);
//layout2->setMargin(STD_MARGIN);
layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);

grpToolBar=new QGroupBox(tr("Tool bar"));
layoutToolBar=new QGridLayout();
layoutToolBar->setSpacing(STD_SPACING);
//layoutToolBar->setMargin(STD_MARGIN);
layoutToolBar->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
grpToolBar->setLayout(layoutToolBar);
grpStatusBar=new QGroupBox(tr("Status bar contents"));
layoutStatusBar=new QGridLayout();
layoutStatusBar->setSpacing(STD_SPACING);
//layoutStatusBar->setMargin(STD_MARGIN);
layoutStatusBar->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
grpStatusBar->setLayout(layoutStatusBar);
grpAppFont=new QGroupBox(tr("Gui font"));
layoutAppFont=new QGridLayout();
layoutAppFont->setSpacing(STD_SPACING);
//layoutAppFont->setMargin(STD_MARGIN);
layoutAppFont->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
grpAppFont->setLayout(layoutAppFont);
grpBackgroundColor=new QGroupBox(tr("Background Color of Draw Area"));
grpBackgroundColor->setToolTip(tr("Select color for widget background behind plot area"));
layoutBackgroundColor=new QGridLayout();
layoutBackgroundColor->setSpacing(STD_SPACING);
//layoutBackgroundColor->setMargin(STD_MARGIN);
layoutBackgroundColor->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
grpBackgroundColor->setLayout(layoutBackgroundColor);

chkNewIcons=new QCheckBox(tr("Use new icons"),this);
chkNewIcons->setToolTip(tr("Use new PNG-icons for navigaion buttons"));
chkShowNavi=new QCheckBox(tr("Show navigation buttons"),this);
chkShowGraph=new QCheckBox(tr("Show graph list"),this);
chkShowSpecZoom=new QCheckBox(tr("Show special zoom buttons"),this);
//chkShowViewp=new QCheckBox(tr("Show viewport stack"),this);
chkShowPageZoom=new QCheckBox(tr("Show page zoom"),this);
chkShowPrintB=new QCheckBox(tr("Show print button"),this);
chkShowExportP=new QCheckBox(tr("Show export button"),this);

/*chkShowPanB=new QCheckBox(tr("Show pan button"),this);
chkShowPickB=new QCheckBox(tr("Show move button"),this);*/
index=3;
entries[0]=tr("Hide button");
entries[1]=tr("Show button");
entries[2]=tr("Always on");
selShowPan=new StdSelector(this,tr("Pan:"),index,entries);
selShowPick=new StdSelector(this,tr("Move:"),index,entries);
entries[0]=tr("Hide controls");
entries[1]=tr("Simple controls (Grace/XmGrace)");
entries[2]=tr("Complete list (QtGace)");
selShowViewp=new StdSelector(this,tr("Viewport:"),index,entries);

chkShowUndoRedo=new QCheckBox(tr("Show Undo/Redo buttons"),this);
selToolbarSize=new LineWidthSelector(this);
selToolbarSize->lblText->setText(tr("Toolbar size:"));
selToolbarSize->setToolTip(tr("Change size of toolbar - toolbar size\ndoes not automatically adjust to window size.\nThis setting also changes nearly all icon sizes."));
selToolbarSize->spnLineWidth->setRange(0.1,10.0);
selToolbarSize->spnLineWidth->setDecimals(1);
selToolbarSize->spnLineWidth->setSingleStep(0.1);

chkShowHostName=new QCheckBox(tr("Show host name"),this);
chkShowHostName->setToolTip(tr("Display the name of the current host"));
chkShowDisplay=new QCheckBox(tr("Show display name"),this);
chkShowDisplay->setToolTip(tr("Display the name of the display"));
index=3;
entries[0]=tr("None");
entries[1]=tr("Complete path");
entries[2]=tr("Filename only");
selFileDisplay1=new StdSelector(this,tr("Show project file name:"),index,entries);
selFileDisplay1->setToolTip(tr("How to display the name of the current project"));
selFileDisplay2=new StdSelector(this,tr("Show export file name:"),index,entries);
selFileDisplay2->setToolTip(tr("How to display the name used for file-export"));

delete[] entries;

cmdGraceDefaults=new QPushButton(tr("Grace-default-GUI"),this);
cmdGraceDefaults->setToolTip(tr("Reset GUI to be like Grace (immediate effect)"));
connect(cmdGraceDefaults,SIGNAL(clicked()),SLOT(doGraceDefaults()));

cmdQtGraceDefaults=new QPushButton(tr("QtGrace-default-GUI"),this);
cmdQtGraceDefaults->setToolTip(tr("Reset GUI to QtGrace-default-style (immediate effect)"));
connect(cmdQtGraceDefaults,SIGNAL(clicked()),SLOT(doQtGraceDefaults()));

cmdActDevs=new QPushButton(tr("Enable/Disable export formats"),this);
cmdActDevs->setToolTip(tr("Disable unwanted export formats to simplify the export dialog"));
connect(cmdActDevs,SIGNAL(clicked()),SLOT(doActDevs()));
diaDevAct=NULL;

cmdTest=new QPushButton(tr("DEBUG: TestDialog"),this);
connect(cmdTest,SIGNAL(clicked()),SLOT(doTest()));
cmdTest->hide();

frmTest=new TestDialog(0);
frmTest->hide();

index=0;
layoutToolBar->addWidget(chkNewIcons,0,0);
layoutToolBar->addWidget(chkShowNavi,1,0);
layoutToolBar->addWidget(chkShowGraph,2,0);
layoutToolBar->addWidget(chkShowSpecZoom,0,1);
//layoutToolBar->addWidget(chkShowViewp,1,1);
layoutToolBar->addWidget(chkShowPageZoom,1,1);
layoutToolBar->addWidget(chkShowExportP,2,1);
layoutToolBar->addWidget(chkShowPrintB,0,2);
layoutToolBar->addWidget(chkShowUndoRedo,1,2);
layoutToolBar->addWidget(selToolbarSize,2,2);
layoutToolBar->addWidget(selShowViewp,3,0);
layoutToolBar->addWidget(selShowPan,3,1);
layoutToolBar->addWidget(selShowPick,3,2);

layoutStatusBar->addWidget(chkShowHostName,0,0);
layoutStatusBar->addWidget(chkShowDisplay,0,1);
layoutStatusBar->addWidget(selFileDisplay1,1,0);
layoutStatusBar->addWidget(selFileDisplay2,1,1);

layout2->addWidget(grpToolBar,index++,0,1,3);
layout2->addWidget(grpStatusBar,index++,0,1,3);

cmdSelGuiFont=new QPushButton(tr("Select Gui Font"),this);
cmdSelGuiFont->setToolTip(tr("Select a new font to be used for the whole GUI"));
cmdResetGuiFont=new QPushButton(tr("Reset Gui Font"),this);
cmdResetGuiFont->setToolTip(tr("Reset the GUI-Font to the system-default"));
lblGuiFont=new QLabel(tr("Current Font"),this);
lblGuiFont->setToolTip(tr("Font name and font parameters used for GUI-font"));

layoutAppFont->addWidget(lblGuiFont,0,0,1,2);
layoutAppFont->addWidget(cmdSelGuiFont,1,0,1,1);
layoutAppFont->addWidget(cmdResetGuiFont,1,1,1,1);

cmdSelGUIBGColor=new QPushButton(tr("Select Color"),this);
cmdSelGUIBGColor->setToolTip(tr("Select a new color for the background of the drawing area"));
cmdSetGUIBGColor_to_PageBG=new QPushButton(tr("Set to Page-Background"),this);
cmdSetGUIBGColor_to_PageBG->setToolTip(tr("Set the background color of the drawing area to the background color of the plot"));
cmdSetGUIBGColor_to_Std=new QPushButton(tr("Set to GUI-Background"),this);
cmdSetGUIBGColor_to_Std->setToolTip(tr("Reset the background color of the drawing area to the system-default"));

layoutBackgroundColor->addWidget(cmdSelGUIBGColor,0,0);
layoutBackgroundColor->addWidget(cmdSetGUIBGColor_to_PageBG,0,1);
layoutBackgroundColor->addWidget(cmdSetGUIBGColor_to_Std,0,2);

//layout2->addWidget(lblToolBar,index++,0);
/*layout2->addWidget(chkNewIcons,index++,0);
layout2->addWidget(chkShowNavi,index++,0);
layout2->addWidget(chkShowGraph,index++,0);
layout2->addWidget(chkShowSpecZoom,index++,0);
layout2->addWidget(chkShowViewp,index++,0);
layout2->addWidget(chkShowPageZoom,index++,0);
layout2->addWidget(chkShowExportP,index++,0);
layout2->addWidget(chkShowPrintB,index++,0);*/

//layout2->addWidget(lblStatusBar,index++,1);
/*layout2->addWidget(chkShowHostName,index++,1);
layout2->addWidget(chkShowDisplay,index++,1);
layout2->addWidget(selFileDisplay1,index++,1);
layout2->addWidget(selFileDisplay2,index++,1);*/

layout2->addWidget(cmdGraceDefaults,index,0);
layout2->addWidget(cmdQtGraceDefaults,index,1);
layout2->addWidget(cmdActDevs,index++,2);
layout2->addWidget(grpAppFont,index++,0,1,3);
layout2->addWidget(grpBackgroundColor,index++,0,1,3);
index++;
index++;

//lblAppearance=new QLabel(tr("Appearance"),this);

/*
QWidget * empty2=new QWidget(this);
QGridLayout * layout4=new QGridLayout();
//layout4->setMargin(0);
layout4->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
layout4->setSpacing(STD_SPACING);
layout4->addWidget(lblGuiFont,0,0);
layout4->addWidget(cmdSelGuiFont,0,1);
layout4->addWidget(cmdResetGuiFont,0,2);
//lblBackground_Color_Text=new QLabel(tr("Background color of Draw-Area:"),this);
//layout4->addWidget(lblBackground_Color_Text,1,0,1,3);
layout4->addWidget(cmdSelGUIBGColor,2,0);
layout4->addWidget(cmdSetGUIBGColor_to_PageBG,2,1);
layout4->addWidget(cmdSetGUIBGColor_to_Std,2,2);
layout4->addWidget(cmdActDevs,3,0,1,3);
empty2->setLayout(layout4);
//layout1->addWidget(empty2);
*/

connect(cmdSelGuiFont,SIGNAL(clicked()),SLOT(changeGUIFont()));
connect(cmdResetGuiFont,SIGNAL(clicked()),SLOT(resetGUIFont()));

connect(cmdSelGUIBGColor,SIGNAL(clicked()),SLOT(select_BG_Color()));
connect(cmdSetGUIBGColor_to_Std,SIGNAL(clicked()),SLOT(set_BG_Color_to_Std()));
connect(cmdSetGUIBGColor_to_PageBG,SIGNAL(clicked()),SLOT(set_BG_Color_to_Page_BG()));

//layout2->addWidget(empty2,index++,0,1,2);

grp_tool_bar->setLayout(layout2);

grp_Startup=new QGroupBox(tr("Startup-Settings"),this);
layout3=new QVBoxLayout();
layout3->setSpacing(STD_SPACING);
//layout3->setMargin(STD_MARGIN);
layout3->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);

/*grpIniPos=new QGroupBox(tr("Initial Size and Position"),this);
layoutIniPos=new QGridLayout();
layoutIniPos->setSpacing(STD_SPACING);
//layoutIniPos->setMargin(STD_MARGIN);
layoutIniPos->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);*/

QWidget * empty=new QWidget(this);
QGridLayout * grid1=new QGridLayout(empty);
grid1->setSpacing(STD_SPACING);
//grid1->setMargin(STD_MARGIN);
grid1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);

QString * entr=new QString[8];
int nr=2;
entr[0]=QString("English");
entr[1]=QString("German");
selLanguage=new StdSelector(this,tr("Language:"),nr,entr);
//layout1->addWidget(selLanguage);
delete[] entr;

nr=number_of_devices();
entr=new QString[32+nr];
int * i_entr=new int[32+nr];
entr[0]=tr("Last selection");
i_entr[0]=-1;
for (int i=0;i<nr;i++)
{
    if (i!=DEVICE_SCREEN)
    {
    entr[i]=get_device_name(i);
    i_entr[i]=i;
    }
}
selDefaultPrintDevice=new StdSelector(this,tr("Default output format:"),nr,entr);
selDefaultPrintDevice->setToolTip(tr("Select an output format to be used as default"));
selDefaultPrintDevice->setValues(i_entr);

lenDefaultFile=new stdLineEdit(this,tr("Default startup file:"));
lenDefaultFile->setToolTip(tr("This file is loaded when \"File->New\" is clicked.\nThis file has to be located in the bin-folder."));
lenDefaultFile->setText(QString::fromLocal8Bit(default_grace_file));
cmdBrowseForDefault=new QPushButton(tr("Select default file"),this);
cmdBrowseForDefault->setToolTip(tr("Select a project file to be used as an empty project."));
connect(cmdBrowseForDefault,SIGNAL(clicked()),SLOT(doBrowseStartup()));
lblStartupWarning=new QLabel(tr("The settings in this group will take effect at the next restart of QtGrace."),this);
QFont tfont=lblStartupWarning->font();
tfont.setItalic(true);
lblStartupWarning->setFont(tfont);
grid1->addWidget(lblStartupWarning,0,0,1,3);
grid1->addWidget(selLanguage,1,0,1,3);
grid1->addWidget(selDefaultPrintDevice,2,0,1,3);
grid1->addWidget(lenDefaultFile,3,0,1,2);
grid1->addWidget(cmdBrowseForDefault,3,2,1,1);

int app_desktop_wi,app_desktop_hi;
#if QT_VERSION >= 0x060000
app_desktop_wi=qApp->primaryScreen()->geometry().width();
app_desktop_hi=qApp->primaryScreen()->geometry().height();
#else
app_desktop_wi=QApplication::desktop()->width();
app_desktop_hi=QApplication::desktop()->height();
#endif
selStdDpi=new stdIntSelector(this,tr("DPI:"),24,720);
selStdDpi->lblText->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
selStdDpi->setToolTip(tr("Use this dpi-value on loading a new file"));
//lblSelStartup=new QLabel(tr("Select initial position/size:"),this);
selStartupX=new stdIntSelector(this,tr("x:"),0,app_desktop_wi);
selStartupX->lblText->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
selStartupX->setToolTip(tr("Initial horizontal position of upper left hand corner of QtGrace window"));
selStartupY=new stdIntSelector(this,tr("y:"),0,app_desktop_hi);
selStartupY->lblText->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
selStartupY->setToolTip(tr("Initial vertical position of upper left hand corner of QtGrace window"));
selStartupWidth=new stdIntSelector(this,tr("Width:"),0,app_desktop_wi);
selStartupWidth->lblText->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
selStartupWidth->setToolTip(tr("Initial width of QtGrace window"));
selStartupHeight=new stdIntSelector(this,tr("Height:"),0,app_desktop_hi);
selStartupHeight->lblText->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
selStartupHeight->setToolTip(tr("Initial height of QtGrace window"));
cmdStartupCurrent=new QPushButton(tr("Initial = current"),this);
cmdStartupCurrent->setToolTip(tr("Use the current size and position of the QtGrace main window as default for startup."));
connect(cmdStartupCurrent,SIGNAL(clicked()),SLOT(doCurrentAsStartup()));

lblStartupMain=new QLabel(tr("Main window:"),this);
nr=2;
entr[0]=tr("Default size");
entr[1]=tr("Last size");
selStartupBehavior=new StdSelector(this,tr("Dialog startup size(s):"),nr,entr);
selStartupBehavior->setToolTip(tr("What size to set for the different dialogs on startup."));

grpIniPos=new QGroupBox(tr("Initial window size and position"),this);
layoutIniPos=new QGridLayout();
layoutIniPos->setSpacing(STD_SPACING);
//layoutIniPos->setMargin(STD_MARGIN);
layoutIniPos->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
grpIniPos->setLayout(layoutIniPos);

layoutIniPos->addWidget(lblStartupMain,0,0);
layoutIniPos->addWidget(cmdStartupCurrent,1,0);
layoutIniPos->addWidget(selStartupX,0,1);
layoutIniPos->addWidget(selStartupY,0,2);
layoutIniPos->addWidget(selStartupWidth,1,1);
layoutIniPos->addWidget(selStartupHeight,1,2);
layoutIniPos->addWidget(selStartupBehavior,2,0,1,3);

//grid1->addWidget(lblSelStartup,4,0,1,2);
/*grid1->addWidget(cmdStartupCurrent,4,2,1,1);
grid1->addWidget(selStdDpi,5,0,1,1);
grid1->addWidget(selStartupX,5,1,1,1);
grid1->addWidget(selStartupY,5,2,1,1);
grid1->addWidget(selStartupWidth,6,1,1,1);
grid1->addWidget(selStartupHeight,6,2,1,1);*/

empty->setLayout(grid1);

layout3->addWidget(empty);
layout3->addWidget(grpIniPos);
grp_Startup->setLayout(layout3);

guiLayout->addWidget(grp_tool_bar);
//guiLayout->addWidget(empty2);
guiLayout->addWidget(grp_Startup);
tab_GUI->setLayout(guiLayout);

/// BEHAVIOR
tab_Behaviour=new QWidget(this);
behavLayout=new QGridLayout();
//behavLayout->setMargin(STD_MARGIN);
behavLayout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
behavLayout->setSpacing(STD_SPACING);

QString * entr2=new QString[6];
entr2[0]=tr("Grace");
entr2[1]=tr("QtGrace");
selGeneral=new StdSelector(this,tr("General Behavior like"),2,entr2);
selGeneral->setToolTip(tr("Affects behavior settings used in Grace that are unusual. (Like the question asked on closing or loading of project fils if unsaved changes are present.)"));
delete[] entr2;

#if READ_UTF8_ONLY < 0
QString * entr3=new QString[avcod.length()];
for (int i=0;i<avcod.length();i++)
{
    entr3[i]=QString(avcod.at(i));
}
selCodec=new StdSelector(this,tr("File encoding:"),avcod.length(),entr3);
selCodec->setToolTip(tr("Change this setting if you want to enter non-Latin-characters directely via the keyboard.\n(When inappropriate encodings are used the characters are not saved/loaded correctely.)"));
#else
QString * entr3=new QString[4];
entr3[0]=QString("UTF-8");
selCodec=new StdSelector(this,tr("File encoding:"),1,entr3);
selCodec->setToolTip(tr("Previously used for different encodings of non-Latin-characters. QtGrace now uses UTF-8 exclusively."));
selCodec->setEnabled(false);
#endif
delete[] entr3;
entr3=new QString[4];
entr3[0]=tr("None");
entr3[1]=tr("Right of Color selection");
entr3[2]=tr("Inside Color selection");
selTranspSelection=new StdSelector(this,tr("Show transparency/opacity selctors:"),3,entr3);
selTranspSelection->setToolTip(tr("How to access the opacity value for all colors.\nIf deactivated(=None), the alpha-channel will be ignored."));
delete[] entr3;
chkActivateLaTeXSupport=new QCheckBox(tr("Support simple LaTeX-commands"),this);
chkActivateLaTeXSupport->setToolTip(tr("This option lets you enter some simple LaTeX-commands inside $$-tags for labels (like $$\\alpha$$)."));
chkImmediateUpdate=new QCheckBox(tr("Immediate updates"),this);
chkImmediateUpdate->setToolTip(tr("Immediately apply every option in every dialog without having to press \"Apply\"\n(this does not affect the Preferences dialog)"));
chkEnableInlineEditing=new QCheckBox(tr("Enable text editing inside main window"),this);
chkEnableInlineEditing->setToolTip(tr("If activated, text entries (labels, titles, texts)\ncan be edited directely on screen\n(not only in a separate dialog)."));
chkEnableContextMenuInMain=new QCheckBox(tr("Enable context-menu inside main window"),this);
chkEnableContextMenuInMain->setToolTip(tr("If activated a right-click on a set summons\na context-menu in the main window."));
chkQtFonts=new QCheckBox(tr("Use Qt Fonts"),this);
chkQtFonts->setToolTip(tr("Use this option if you want to use non-latin and non greek letters or special fonts.\nWarning: The QtFonts are not supported by every output driver.\nThe QtFonts are also not savely portable.\n(If you copy your project file to another platform, the selected fonts might not be present)."));
connect(chkQtFonts,SIGNAL(toggled(bool)),SLOT(toggleQtFonts(bool)));
chkSymbolSpecial=new QCheckBox(tr("'Symbol'-font is special"),this);
chkSymbolSpecial->setToolTip(tr("Enabling this option resorts the characters in the Symbol font to match the order of the characters in the original Grace-Symbol-font"));
chkAutoPackSets=new QCheckBox(tr("Auto-pack sets"),this);
chkAutoPackSets->setToolTip(tr("When a set is removed automatically pack all sets\n(so that the set-ids have no gaps)."));
chkErrorbarsAutoscale=new QCheckBox(tr("Consider error bars for autoscale"),this);
chkErrorbarsAutoscale->setToolTip(tr("If activated autoscale makes sure that\nall error bars are visible inside the graph."));
selAutoscaleOffset=new LineWidthSelector(this);
selAutoscaleOffset->lblText->setText(tr("Min. autoscale offset (fraction of range):"));
selAutoscaleOffset->spnLineWidth->setRange(0.0,0.5);
selAutoscaleOffset->spnLineWidth->setDecimals(3);
selAutoscaleOffset->setToolTip(tr("Minimum offset between a graph-border\nand a set-point or error-bar.\nThe \"range\" is the difference between min- and max-value of all sets."));
chkAllowGestures=new QCheckBox(tr("Allow gestures in main window"),this);
chkAllowGestures->setToolTip(tr("If activated the main window accepts Pinch-/Swipe-/Pan-Gestures\nto move the visible area of a graph."));

chkAllowWheelChanges=new QCheckBox(tr("Use mouse wheel for size changes"),this);
chkAllowWheelChanges->setToolTip(tr("If activated the mouse wheel can be\nused for changing font and object sizes."));
chkAllowWheel=new QCheckBox(tr("Use mouse wheel for zooming"),this);
chkAllowWheel->setToolTip(tr("If activated the mouse wheel can\nbe used for zooming in a graph."));

chkUndoActive=new QCheckBox(tr("Undo active"),this);
chkUndoActive->setToolTip(tr("Allows undo and redo of most operations (setting for current session)."));
chkUndoActiveStart=new QCheckBox(tr("Undo active on startup"),this);
chkUndoActiveStart->setToolTip(tr("Make Undo active, when QtGrace is started"));

nr=2;
entr[0]=QString(".");
entr[1]=QString(",");
selDecSep=new StdSelector(this,tr("Decimal separator:"),nr,entr);
selDropDecSep=new StdSelector(this,tr("Decimal separator:"),nr,entr);
selDecSep->setToolTip(tr("The decimal separator used in the QtGrace-GUI for input and display of decimal numbers."));
selDropDecSep->setToolTip(tr("The decimal separator used in QtGrace for reading from ascii-files dropped into the main window."));
histSize=new stdIntSelector(this,tr("Maximum history size:"),0,MAX_HISTORY);
histSize->setToolTip(tr("The maximum number of project files to be remembered in the ini-file and the \"File->Resently opened files\" menu"));
histSize->lblText->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
histSize->setValue(max_history);
chkAutoSetAgr=new QCheckBox(tr("Auto set .agr-file-extension"),this);
chkAutoSetAgr->setToolTip(tr("When entering a file name for saving automatically set the agr-extension."));
chkAutoSetExport=new QCheckBox(tr("Auto set export-file-extension"),this);
chkAutoSetExport->setToolTip(tr("When entering a file name for exporting automatically set the extension which is specific for the selected output format."));
chkAutoSetCWD=new QCheckBox(tr("Auto set current working directory (CWD)"),this);
chkAutoSetCWD->setToolTip(tr("On loading and saving files automatically set the Current Working Directory (CWD) to the folder where the file is in.\nDrag-and-drop counts as opening a file."));
chkAutoFitLoad=new QCheckBox(tr("Auto fit page after loading project"),this);
chkAutoFitLoad->setToolTip(tr("Automatically fit the page-zoom to accommodate the whole plot in the QtGrace window when loading a project file."));
chkWarnOnEncodingChange=new QCheckBox(tr("Display warning on encoding change"),this);
chkWarnOnEncodingChange->setToolTip(tr("Display an information message whenever a file is loaded that has been saved with a different encoding than that, which is currently used.\nIn this case the current encoding format is changed to the one used in the file."));/// Change Encoding or update contents to fit current encoding? Checken!

selCompression=new CompressionSelector(this);

selHighlightColor=new ColorSelector(this);
selHighlightColor->setToolTip(tr("Color to highlight a set or graph in the main window.\nThe linewidth for highlighting is always 2,5x the largest selected linewidth."));
nr=6;
entr[0]=tr("No limit");
entr[1]=QString("1 s");
entr[2]=QString("2 s");
entr[3]=QString("3 s");
entr[4]=QString("4 s");
entr[5]=QString("5 s");
selHighlightTime=new StdSelector(this,tr("Maximum time for highlight:"),nr,entr);
selHighlightTime->setToolTip(tr("After this time the highlight-blinking is stopped.\nWithout limit the highlight stops if the current list looses the focus."));
chkAutoHighlight=new QCheckBox(tr("Auto-highlight new selection"),this);
chkAutoHighlight->setToolTip(tr("Activate the highlighting automatically if a new selection is made."));
chkHighlightErrorbars=new QCheckBox(tr("Highlight error bars and symbols"),this);
chkHighlightErrorbars->setToolTip(tr("If activated the errorbars and symbols of a set are highlighted as well as the line of a set."));

nr=3;
entr[0]=tr("No Icons (just text)");
entr[1]=tr("Icons and text");
entr[2]=tr("Just icons (no text)");
selIconBehavior=new StdSelector(this,tr("Icons on buttons/in lists:"),nr,entr);
selIconBehavior->setToolTip(tr("Select whether you want icons displayed on buttons and in lists or not."));

/*selFontSize=new LineWidthSelector(this);
selFontSize->lblText->setText(tr("Global font size multiplicator:"));
selFontSize->spnLineWidth->setRange(0.0,1000.0);
selFontSize->spnLineWidth->setDecimals(2);
selFontSize->spnLineWidth->setValue(100.0);
selFontSize->spnLineWidth->setSingleStep(1.0);
selFontSize->spnLineWidth->setSuffix(tr(" %"));*/

grpLoadSave=new QGroupBox(tr("Load and save"),this);
layoutLoadSave=new QGridLayout();
//layoutLoadSave->setMargin(STD_MARGIN);
layoutLoadSave->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
layoutLoadSave->setSpacing(STD_SPACING);
grpInput=new QGroupBox(tr("Input-Behavior"),this);
layoutInput=new QGridLayout();
//layoutInput->setMargin(STD_MARGIN);
layoutInput->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
layoutInput->setSpacing(STD_SPACING);
grpResponse=new QGroupBox(tr("Responsiveness"),this);
layoutResponse=new QGridLayout();
//layoutResponse->setMargin(STD_MARGIN);
layoutResponse->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
layoutResponse->setSpacing(STD_SPACING);
grpHighlight=new QGroupBox(tr("Highlighting"),this);
layoutHighlight=new QGridLayout();
//layoutHighlight->setMargin(STD_MARGIN);
layoutHighlight->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
layoutHighlight->setSpacing(STD_SPACING);
int rindex=0;
layoutResponse->addWidget(selGeneral,rindex++,0,1,2);
layoutResponse->addWidget(selIconBehavior,rindex++,0,1,2);
layoutResponse->addWidget(selTranspSelection,rindex++,0,1,2);
layoutResponse->addWidget(chkImmediateUpdate,rindex,0,1,1);
layoutResponse->addWidget(chkEnableInlineEditing,rindex++,1,1,1);
layoutResponse->addWidget(chkAutoPackSets,rindex,0,1,1);
layoutResponse->addWidget(chkEnableContextMenuInMain,rindex++,1,1,1);
layoutResponse->addWidget(chkErrorbarsAutoscale,rindex,0,1,1);
layoutResponse->addWidget(selAutoscaleOffset,rindex++,1,1,1);
layoutResponse->addWidget(chkAllowGestures,rindex,0,1,1);
layoutResponse->addWidget(chkAllowWheel,rindex++,1,1,1);
layoutResponse->addWidget(chkAllowWheelChanges,rindex++,1,1,1);
layoutResponse->addWidget(chkUndoActive,rindex,0,1,1);
layoutResponse->addWidget(chkUndoActiveStart,rindex++,1,1,1);

layoutInput->addWidget(selDecSep,0,0,1,2);
layoutInput->addWidget(chkQtFonts,1,0,1,1);
layoutInput->addWidget(chkActivateLaTeXSupport,1,1,1,1);
layoutInput->addWidget(chkSymbolSpecial,2,0,1,1);

index=0;
layoutLoadSave->addWidget(selCodec,index,0);
layoutLoadSave->addWidget(chkWarnOnEncodingChange,index++,1);
layoutLoadSave->addWidget(histSize,index,0);
layoutLoadSave->addWidget(selStdDpi,index++,1);
layoutLoadSave->addWidget(chkAutoFitLoad,index,0);
layoutLoadSave->addWidget(chkAutoSetExport,index++,1);
layoutLoadSave->addWidget(chkAutoSetAgr,index,0);
layoutLoadSave->addWidget(chkAutoSetCWD,index++,1);
layoutLoadSave->addWidget(selCompression,index++,0,1,2);

layoutHighlight->addWidget(selHighlightColor,0,0);
layoutHighlight->addWidget(selHighlightTime,0,1);
layoutHighlight->addWidget(chkAutoHighlight,1,0);
layoutHighlight->addWidget(chkHighlightErrorbars,1,1);

grpLoadSave->setLayout(layoutLoadSave);
grpInput->setLayout(layoutInput);
grpResponse->setLayout(layoutResponse);
grpHighlight->setLayout(layoutHighlight);

grpDrop=new QGroupBox(tr("Dropping files"),this);
nr=6;
entr[0]=tr("Nothing");
entr[1]=tr("Path/Filename and column");
entr[2]=tr("Path/Filename only");
entr[3]=tr("Filename and column");
entr[4]=tr("Filename only");
entr[5]=tr("Column only");
selDropComment=new StdSelector(this,tr("Set-comment:"),nr,entr);
selDropComment->setToolTip(tr("What to put into the set-comment after a set has been read from a file dropped into QtGrace."));
selDropLegend=new StdSelector(this,tr("Set-legend:"),nr,entr);
selDropLegend->setToolTip(tr("What to put into the set-legend after a set has been read from a file dropped into QtGrace."));
chkDropAutoscale=new QCheckBox(tr("Autoscale"),this);
chkDropAutoscale->setToolTip(tr("Activate this if you want the axes of the current graph to be automatically adjusted to be able to see all sets."));
selCSVDropFormat=new multiFileSelector(this);
selCSVDropFormat->singleFileOnly=true;
selCSVDropFormat->defaultStartPath=qt_grace_templates_dir;
selCSVDropFormat->cmdLoadFile->setText(tr("Choose Format-File"));
selCSVDropFormat->lenFile->lblText->setText(tr("CSV-Format-File:"));
selCSVDropFormat->setToolTip(tr("Default file format for *.csv-files dropped into the QtGrace.\nOnly one format allowed here.\ncsv-files are recognized by the file-suffix *.csv."));
layoutDrop=new QGridLayout(grpDrop);
layoutDrop->setSpacing(STD_SPACING);
//layoutDrop->setMargin(STD_MARGIN);
layoutDrop->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
layoutDrop->addWidget(selDropComment,0,0);
layoutDrop->addWidget(selDropLegend,1,0);
layoutDrop->addWidget(selDropDecSep,0,1);
layoutDrop->addWidget(chkDropAutoscale,1,1);
layoutDrop->addWidget(selCSVDropFormat,2,0,1,2);
grpDrop->setLayout(layoutDrop);

behavLayout->addWidget(grpResponse,0,0);
behavLayout->addWidget(grpInput,1,0);
behavLayout->addWidget(grpLoadSave,2,0);
behavLayout->addWidget(grpHighlight,3,0);
behavLayout->addWidget(grpDrop,4,0);

//behavLayout->addWidget(cmdTest,5,0);//-->moved to different tab

//behavLayout->addWidget(selFontSize,3,0);
//behavLayout->addWidget(cmdActDevs,4,0);

/*int r_index=0;
behavLayout->addWidget(selGeneral,r_index++,0,1,2);
behavLayout->addWidget(chkQtFonts,r_index,0);
behavLayout->addWidget(chkActivateLaTeXSupport,r_index++,1);
behavLayout->addWidget(chkSymbolSpecial,r_index++,0);
behavLayout->addWidget(chkImmediateUpdate,r_index++,0);
behavLayout->addWidget(selCodec,r_index++,0,1,2);
behavLayout->addWidget(selDecSep,r_index++,0,1,2);
behavLayout->addWidget(histSize,r_index++,0,1,2);
behavLayout->addWidget(selFontSize,r_index++,0,1,2);
behavLayout->addWidget(chkAutoSetAgr,r_index,0);
behavLayout->addWidget(chkAutoSetExport,r_index++,1);
behavLayout->addWidget(chkAutoSetCWD,r_index,0);
behavLayout->addWidget(chkWarnOnEncodingChange,r_index++,1);
behavLayout->addWidget(cmdActDevs,r_index++,0,1,2);*/
QWidget * empty5=new QWidget(this);
behavLayout->addWidget(empty5,6,0);
behavLayout->setRowStretch(6,3);

tab_Behaviour->setLayout(behavLayout);

/// MISC
tab_Misc=new QWidget(this);
miscLayout=new QVBoxLayout(this);
//miscLayout->setMargin(STD_MARGIN);
miscLayout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
miscLayout->setSpacing(STD_SPACING);

//lblExtLibs=new QLabel(tr("External Libraries:"),this);
grp_libFFTW3=new QGroupBox(tr("libFFTW3 (for Fourier-transformations)"),this);
chkUseFFTW3=new QCheckBox(tr("Use FFTW3"),this);
lblFFTW3_found=new QLabel(tr("FFTW3 not usable"),this);
lblfftw3_static=new QLabel(tr("FFTW3 linked as static library"),this);
ledFFTW3_dll=new stdLineEdit(this,tr("Library:"));
ledFFTW3_dll->setText(QString(""));
cmdBrowseFFTW3=new QPushButton(tr("Browse"),this);
connect(cmdBrowseFFTW3,SIGNAL(clicked()),SLOT(doBrowseFFTW3_dll()));
layout_misc0=new QGridLayout();
layout_misc0->setSpacing(STD_SPACING);
//layout_misc0->setMargin(STD_MARGIN);
layout_misc0->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
layout_misc0->addWidget(chkUseFFTW3,0,0,1,2);
layout_misc0->addWidget(lblFFTW3_found,0,2,1,1);
#ifndef USE_STATIC_EXT_LIB_FFTW3
layout_misc0->addWidget(ledFFTW3_dll,1,0,1,2);
layout_misc0->addWidget(cmdBrowseFFTW3,1,2,1,1);
lblfftw3_static->hide();
#else
layout_misc0->addWidget(lblfftw3_static,1,0,1,3);
ledFFTW3_dll->hide();
cmdBrowseFFTW3->hide();
#endif
grp_libFFTW3->setLayout(layout_misc0);

grp_libHaru=new QGroupBox(tr("libHaru (for pdf-export)"),this);
chkUselibHaru=new QCheckBox(tr("Use libHaru"),this);
lblHaru_found=new QLabel(tr("libHaru not usable"),this);
lblHaru_static=new QLabel(tr("libHaru linked as static library"),this);
ledHaru_dll=new stdLineEdit(this,tr("Library:"));
ledHaru_dll->setText(QString(""));
cmdBrowseHaru=new QPushButton(tr("Browse"),this);
connect(cmdBrowseHaru,SIGNAL(clicked()),SLOT(doBrowseHaru_dll()));
layout_misc1=new QGridLayout();
layout_misc1->setSpacing(STD_SPACING);
//layout_misc1->setMargin(STD_MARGIN);
layout_misc1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
layout_misc1->addWidget(chkUselibHaru,0,0,1,2);
layout_misc1->addWidget(lblHaru_found,0,2,1,1);
#ifndef USE_STATIC_EXT_LIB_HARU
layout_misc1->addWidget(ledHaru_dll,1,0,1,2);
layout_misc1->addWidget(cmdBrowseHaru,1,2,1,1);
lblHaru_static->hide();
#else
layout_misc1->addWidget(lblHaru_static,1,0,1,3);
ledHaru_dll->hide();
cmdBrowseHaru->hide();
#endif
grp_libHaru->setLayout(layout_misc1);

grpMiscMisc=new QGroupBox(tr("Misc"),this);
misc2Layout=new QVBoxLayout();
//misc2Layout->setMargin(STD_MARGIN);
misc2Layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
misc2Layout->setSpacing(STD_SPACING);

chkExternalHelpViewer=new QCheckBox(tr("Show help-files in external html-viewer"),this);
lenHelpViewer=new stdLineEdit(this,tr("Help viewer:"));
lenHelpViewer->setEnabled(false);
chkShowHideException=new QCheckBox(tr("Show/Hide workaround"),this);
lblSmallScreen=new QLabel(tr("Activate scroll-bars for small screen:"),this);
chkSmallScreenMain=new QCheckBox(tr("Main window"),this);
chkSmallScreenDialogs=new QCheckBox(tr("Large dialogs"),this);

grpPrinting=new QGroupBox(tr("Printing"),this);
/*fntPref=grpPrinting->font();
fntPref.setPixelSize(fntPref.pixelSize()*1.3);
grpPrinting->setFont(fntPref);*/
misc3Layout=new QVBoxLayout();
//misc3Layout->setMargin(STD_MARGIN);
misc3Layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
misc3Layout->setSpacing(STD_SPACING);

chkUsePrintCommand=new QCheckBox(tr("Use Print command (if this is deselected the native printer dialog is used)"),this);
lenPrintCommand=new stdLineEdit(this,tr("Print command:"));
lenPrintCommand->setText("lpr");
lenPrintCommand->setEnabled(false);
chkHDPrinterOutput=new QCheckBox(tr("Use HD-output on physical printers"),this);
selPrintDpi=new stdIntSelector(this,tr("Printer resolution (dpi):"),72,4800);

//fntPref.setPixelSize(fntPref.pixelSize()/1.3);
//chkUsePrintCommand->setFont(fntPref);
//lenPrintCommand->setFont(fntPref);
//chkHDPrinterOutput->setFont(fntPref);

grpRemote=new QGroupBox(tr("Communication"),this);
layoutRemote=new QGridLayout();
//layoutRemote->setMargin(STD_MARGIN);
layoutRemote->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
layoutRemote->setSpacing(STD_SPACING);
chkAnnounceRemote=new QCheckBox(tr("Comment on Server/Client-communications"),this);
chkAnnounceRemote->setToolTip(tr("Shows messages on 'stderr' about ongoing\ncommunication via Server/Client-protocol."));
layoutRemote->addWidget(chkAnnounceRemote,0,0,1,1);
grpRemote->setLayout(layoutRemote);

chkUseFFTW3->setToolTip(tr("Use the fftw3-library for all fourier-transformations"));
lblFFTW3_found->setToolTip(tr("Indicates whether the fftw3-library has been found"));
lblfftw3_static->setToolTip(tr("Indicates that the fftw3-library has been statically linked to QtGrace"));
ledFFTW3_dll->setToolTip(tr("The location where the fftw3-library can be found (dynamic library)"));
cmdBrowseFFTW3->setToolTip(tr("Specify the location of the fftw3-library"));
chkUselibHaru->setToolTip(tr("Use the Haru-PDF-library in the file-output-dialog"));
lblHaru_found->setToolTip(tr("Indicates whether the Haru-PDF-library has been found"));
lblHaru_static->setToolTip(tr("Indicates that the Haru-PDF-library has been statically linked to QtGrace"));
ledHaru_dll->setToolTip(tr("The location where the Haru-PDF-library can be found (dynamic library)"));
cmdBrowseHaru->setToolTip(tr("Specify the location of the Haru-PDF-library"));
chkUsePrintCommand->setToolTip(tr("Use a system-command to address the printer"));
selPrintDpi->setToolTip(tr("The resolution used to print on a physcial printer.\nThis is necessary because the print-system of Qt does not always report the correct resolution."));
lenPrintCommand->setToolTip(tr("Specify the system-command to address the printer"));
chkHDPrinterOutput->setToolTip(tr("Use high-quality-output on physical printer\n(this may increases the resolution dramatically and result in much smaller fill-patterns)"));
chkExternalHelpViewer->setToolTip(tr("Use a different external html-viewer to display help-files\n(the default viewer of the operating system is used otherwise)"));
lenHelpViewer->setToolTip(tr("Specify alternative external html-viewer"));
chkShowHideException->setToolTip(tr("A workaround to dectivates and reactivates the plot-area on every redraw\n(this was neccessary on some Linux-systems)\nThis should not be needed any more"));
chkSmallScreenMain->setToolTip(tr("Activate a scrollbar for the tool bar\nif the height of the main window is too small.\nIf deactivated, the lower buttons may not be accessable on small screens."));
chkSmallScreenDialogs->setToolTip(tr("Activate scrollbars in some of the larger dialogs\nif the dialog is too small to display everything.\nIf deactivated, large dialogs have a minimal size."));

cmdSummonWizard=new QPushButton(tr("Summon setup wizard"),this);
connect(cmdSummonWizard,SIGNAL(clicked()),SLOT(doSummonWizard()));

misc2Layout->addWidget(chkExternalHelpViewer);
misc2Layout->addWidget(lenHelpViewer);
misc2Layout->addWidget(chkShowHideException);
misc2Layout->addWidget(lblSmallScreen);
misc2Layout->addWidget(chkSmallScreenMain);
misc2Layout->addWidget(chkSmallScreenDialogs);

//misc3Layout->addWidget(chkHDPrinterOutput);
chkHDPrinterOutput->setVisible(false);
misc3Layout->addWidget(selPrintDpi);
misc3Layout->addWidget(chkUsePrintCommand);
misc3Layout->addWidget(lenPrintCommand);

grpPrinting->setLayout(misc3Layout);
grpMiscMisc->setLayout(misc2Layout);

miscLayout->addWidget(grp_libFFTW3);
miscLayout->addWidget(grp_libHaru);
miscLayout->addWidget(grpPrinting);
miscLayout->addWidget(grpMiscMisc);
miscLayout->addWidget(grpRemote);
miscLayout->addWidget(cmdSummonWizard);
miscLayout->addWidget(cmdTest);
QWidget * empty6=new QWidget(this);
miscLayout->addWidget(empty6);
miscLayout->setStretch(6,3);

tab_Misc->setLayout(miscLayout);

tabs->addTab(tab_prefs,tr("Grace"));                    //0
tabs->addTab(tab_GUI,tr("QtGrace-GUI"));
tabs->addTab(tab_Behaviour,tr("QtGrace-Behavior"));
tabs->addTab(tab_Misc,tr("Miscellaneous"));
//tabs->addTab(tab_extra,tr("QtGrace (1)"));              //1
//tabs->addTab(tab_qtgrace_prefs2,tr("QtGrace (2)"));     //2
tabs->addTab(tab_defaults,tr("Defaults"));              //3
tabs->addTab(tab_linestyles,tr("Linestyles"));          //4
tabs->addTab(tab_colors,tr("Colors"));                  //5
/// tabs->addTab(tab_DefFormats,tr("Formats"));                  //6 --> currently deactivated

vbox->addWidget(tabs);

buttons=new stdButtonGroup(this);

connect(buttons->cmdApply,SIGNAL(clicked()),SLOT(doApply()));
connect(buttons->cmdAccept,SIGNAL(clicked()),SLOT(doAccept()));
connect(buttons->cmdClose,SIGNAL(clicked()),SLOT(doClose()));

vbox->addWidget(buttons);

flp->setLayout(vbox);

connect(tab_prefs,SIGNAL(close_wish()),SLOT(doClose()));
//connect(tab_extra,SIGNAL(close_wish()),SLOT(doClose()));
//connect(tab_qtgrace_prefs2,SIGNAL(close_wish()),SLOT(doClose()));
connect(tab_linestyles,SIGNAL(close_wish()),SLOT(doClose()));
connect(tab_colors,SIGNAL(close_wish()),SLOT(doClose()));
connect(tab_defaults,SIGNAL(close_wish()),SLOT(doClose()));
connect(tabs,SIGNAL(currentChanged(int)),SLOT(tab_changed(int)));
connect(chkExternalHelpViewer,SIGNAL(stateChanged(int)),SLOT(toggleHTMLviewer(int)));
connect(chkUsePrintCommand,SIGNAL(stateChanged(int)),SLOT(togglePrintCommand(int)));

delete[] entr;
delete[] i_entr;

setLayout(m_layout);

//resize(QSize(min_w,min_h));
resize(LastSize_Form_Preferences);
}

frm_Preferences::~frm_Preferences()
{
    LastSize_Form_Preferences=this->size();
}

void frm_Preferences::init(void)
{
tab_prefs->init();
//tab_extra->init();
//tab_qtgrace_prefs2->init();
init_GUI();
init_Behavior();
init_Misc();
tab_defaults->init();
tab_linestyles->init();
tab_colors->init();
tab_defaults->show_defaults=grdefaults;
tab_defaults->show_view=grview;
strcpy(tab_defaults->show_sformat,sformat);
redisplayContents();
}

void frm_Preferences::init_GUI(void)
{
int app_desktop_wi,app_desktop_hi;
#if QT_VERSION >= 0x060000
    app_desktop_wi=qApp->primaryScreen()->geometry().width();
    app_desktop_hi=qApp->primaryScreen()->geometry().height();
#else
    app_desktop_wi=QApplication::desktop()->width();
    app_desktop_hi=QApplication::desktop()->height();
#endif

    chkNewIcons->setChecked(use_new_icons);
    QString font_descr=qApp->font().toString();
    lblGuiFont->setText(font_descr);

    selStartupX->spnInt->setRange(0,app_desktop_wi*0.9);
    selStartupY->spnInt->setRange(0,app_desktop_hi*0.9);
    selStartupWidth->spnInt->setRange(100,app_desktop_wi);
    selStartupHeight->spnInt->setRange(100,app_desktop_hi);

    //Display the customization-Settings
    chkShowNavi->setChecked(show_Navi_B);
    chkShowGraph->setChecked(show_Graph_List);
    chkShowSpecZoom->setChecked(show_special_Zoom);
    //chkShowViewp->setChecked(show_Viewport_Stack);
    selShowViewp->setCurrentIndex(show_Viewport_Stack);
    chkShowPageZoom->setChecked(show_Page_Zoom);
    chkShowPrintB->setChecked(show_Print_B);
    chkShowExportP->setChecked(show_Export_B);
    /*chkShowPanB->setChecked(show_PanButton);
    chkShowPickB->setChecked(show_PickButton);*/
    selShowPan->setCurrentIndex(show_PanButton);
    selShowPick->setCurrentIndex(show_PickButton);
    chkShowUndoRedo->setChecked(show_UndoRedo_B);
    chkShowHostName->setChecked(show_host_name);
    chkShowDisplay->setChecked(show_display_name);
    selFileDisplay1->setCurrentIndex(displ_project_filename);
    selFileDisplay2->setCurrentIndex(displ_export_filename);
    selToolbarSize->setValue(toolBarSizeFactor);

    //Startup
    selDefaultPrintDevice->setCurrentValue(default_Print_Device);
    lenDefaultFile->lenText->setText(QString::fromLocal8Bit(default_grace_file));
    selLanguage->setCurrentIndex(current_language);//ok
    selStartupX->setValue(initial_x_pos);
    selStartupY->setValue(initial_y_pos);
    selStartupWidth->setValue(initial_width);
    selStartupHeight->setValue(initial_height);
    selStartupBehavior->setCurrentValue(initial_size_behavior);
    selStdDpi->setValue(start_dpi);

    show_Navi_B=chkShowNavi->isChecked();
    show_Graph_List=chkShowGraph->isChecked();
    show_special_Zoom=chkShowSpecZoom->isChecked();
    //show_Viewport_Stack=chkShowViewp->isChecked();
    show_Viewport_Stack=selShowViewp->currentIndex();
    show_Page_Zoom=chkShowPageZoom->isChecked();
    show_Print_B=chkShowPrintB->isChecked();
    show_Export_B=chkShowExportP->isChecked();
    show_host_name=chkShowHostName->isChecked();
    show_display_name=chkShowDisplay->isChecked();
    displ_project_filename=selFileDisplay1->currentIndex();
    displ_export_filename=selFileDisplay2->currentIndex();
}

void frm_Preferences::init_Behavior(void)
{
    selGeneral->setCurrentValue(general_behavior);
    histSize->setValue(max_history);//ok
    chkActivateLaTeXSupport->setChecked((bool)activateLaTeXsupport);
    chkImmediateUpdate->setChecked(immediateUpdate);
    chkEnableInlineEditing->setChecked(inline_editing);
    chkEnableContextMenuInMain->setChecked(context_menu_in_main);
    chkAutoPackSets->setChecked(autoPackSets);
    selIconBehavior->setCurrentIndex(icon_preferences);
    chkErrorbarsAutoscale->setChecked((bool)useErrorbarsInAutoscale);
    selAutoscaleOffset->setValue(minAutoscaleBorderOffset);
    chkAllowGestures->setChecked((bool)allow_gestures);
    chkAllowWheel->setChecked((bool)allow_wheel_zoom);
    chkAllowWheelChanges->setChecked((bool)allow_wheel_changes);
    chkUndoActive->setChecked(undo_active);
    chkUndoActiveStart->setChecked(start_with_undo_active);
    selTranspSelection->setCurrentValue(show_transparency_selector);

    //selFontSize->setValue(universal_font_size_factor*100.0);
    chkQtFonts->setChecked(useQtFonts);
    chkSymbolSpecial->setChecked(symbol_font_is_special);
        if (useQtFonts==true) chkSymbolSpecial->setEnabled(true);
        else chkSymbolSpecial->setEnabled(false);
    selDecSep->setCurrentIndex(DecimalPointToUse=='.'?0:1);

    chkAutoFitLoad->setChecked(autofit_on_load);
    chkAutoSetAgr->setChecked(auto_set_agr_extension);
    chkAutoSetCWD->setChecked(auto_set_cwd);
    chkAutoSetExport->setChecked(auto_set_export_extensions);
    chkWarnOnEncodingChange->setChecked(warn_on_encoding_change);

    if (default_compression_level==0 && default_compress_data!=COMPRESSION_NONE)
    {
    stufftext(tr("[Warning] Compression level set to 0. Compression turned off.").toLocal8Bit().constData());
    default_compress_data=COMPRESSION_NONE;
    }
    selCompression->init(default_compress_data,default_compression_level);

    selHighlightColor->setCurrentIndex(highlight_color);
    selHighlightColor->setAlpha(highlight_alpha);
    selHighlightTime->setCurrentIndex(highlight_time);
    chkAutoHighlight->setChecked(auto_highlight==TRUE?true:false);
    chkHighlightErrorbars->setChecked(highlight_errorbars==TRUE?true:false);

    /*
    int index=0;
    for (int i=0;i<avcod.length();i++)
    {
        if (FileCodec==QTextCodec::codecForName(avcod.at(i)))
        {
        index=i;
        break;
        }
    }
    selCodec->cmbSelect->setCurrentIndex(index);
    */
    selCodec->cmbSelect->setCurrentIndex(0);
    ///starting with v0.2.7 only UTF-8 is used (therefore there is only one entry)

    selDropComment->setCurrentIndex(drop_comment);
    selDropLegend->setCurrentIndex(drop_legend);
    chkDropAutoscale->setChecked(drop_autoscale==TRUE?true:false);
    selDropDecSep->setCurrentIndex(drop_dec_sep=='.'?0:1);
    selCSVDropFormat->lenFile->lenText->setText(QString::fromLocal8Bit(default_csv_format));
}

void frm_Preferences::init_Misc(void)
{
    QColor col_red(255,0,0);
    QColor col_green(0,255,0);
    QColor col_text;
    QPalette Pal(mainWin->palette());
    col_text=Pal.color(QPalette::Text);//tmp is the standard text color

        chkUseFFTW3->setChecked(use_fftw3);
        chkUselibHaru->setChecked(use_libHaru);
        ledFFTW3_dll->setText(path_to_fftw3_lib.fileName());
        ledHaru_dll->setText(path_to_libharu.fileName());
        check_external_lib_usability();
    /*#ifdef USE_STATIC_EXT_LIB_FFTW3
        cout << "USING STATIC LIBRARY FFTW3" << endl;
    #endif*/

    QPalette Pal1(ledFFTW3_dll->lenText->palette());
        if (path_to_fftw3_lib.exists()==true)
        {
        Pal1.setColor(QPalette::Text, col_text);
        }
        else
        {
        Pal1.setColor(QPalette::Text, col_red);
        }
    ledFFTW3_dll->lenText->setPalette(Pal1);

    /*#ifdef USE_STATIC_EXT_LIB_HARU
        cout << "USING STATIC LIBRARY HARU" << endl;
    #endif*/

    QPalette Pal2(ledHaru_dll->lenText->palette());
        if (path_to_libharu.exists()==true)
        {
        Pal2.setColor(QPalette::Text, col_text);
        }
        else
        {
        Pal2.setColor(QPalette::Text, col_red);
        }
    ledHaru_dll->lenText->setPalette(Pal1);

    QPalette Pal3(lblFFTW3_found->palette());
        if (have_fftw3==TRUE)
        {
        lblFFTW3_found->setText(tr("FFTW3 usable"));
        //Pal3.setColor(QPalette::Background, col_green);
        Pal3.setColor(QPalette::Window, col_green);
        }
        else//fftw3 not found
        {
        lblFFTW3_found->setText(tr("FFTW3 NOT usable"));
        //Pal3.setColor(QPalette::Background, col_red);
        Pal3.setColor(QPalette::Window, col_red);
        }
    lblFFTW3_found->setPalette(Pal3);
    lblFFTW3_found->setAlignment(Qt::AlignCenter);
    lblFFTW3_found->setAutoFillBackground(true);

    QPalette Pal4(lblHaru_found->palette());
        if (have_libHaru==TRUE)
        {
        lblHaru_found->setText(tr("libHaru usable"));
        //Pal4.setColor(QPalette::Background, col_green);
        Pal4.setColor(QPalette::Window, col_green);
        }
        else//libHaru not found
        {
        lblHaru_found->setText(tr("libHaru NOT usable"));
        //Pal4.setColor(QPalette::Background, col_red);
        Pal4.setColor(QPalette::Window, col_red);
        }
    lblHaru_found->setPalette(Pal4);
    lblHaru_found->setAlignment(Qt::AlignCenter);
    lblHaru_found->setAutoFillBackground(true);

    //lenHome->setText(QString(GRACE_HOME));
    chkExternalHelpViewer->setChecked(display_help_externally);//ok
    lenHelpViewer->setText(QString(qtgrace_help_viewer));
    chkShowHideException->setChecked(showhideworkaround);//ok
    chkUsePrintCommand->setChecked(use_print_command);
    lenPrintCommand->setText(get_print_cmd());
    chkHDPrinterOutput->setChecked(useHDPrinterOutput);
    selPrintDpi->setValue(typicalPrinterResolution);
    chkAnnounceRemote->setChecked(comment_server_client==0?false:true);

    if (small_screen_adjustments & 1)
    chkSmallScreenMain->setChecked(true);
    else
    chkSmallScreenMain->setChecked(false);

    if (small_screen_adjustments & 2)
    chkSmallScreenDialogs->setChecked(true);
    else
    chkSmallScreenDialogs->setChecked(false);
}

void frm_Preferences::read(void)
{
int saved_toolbarsize=toolBarSizeFactor;
immediateUpdate=false;//we set this to false in order not to interfere with other updates here
lock_dirtystate(TRUE);
//int save_gui_fontsize=qApp->font().pixelSize();
QFont savedFont=qApp->font();
    tab_prefs->doApply();
    read_GUI();
    read_Behavior();
    read_Misc();
immediateUpdate=chkImmediateUpdate->isChecked();//to update this in the dialog correctely
    init();
immediateUpdate=false;//to not interfere with the other updates
    mainWin->redisplayIcons();
//qDebug() << "Read, before Update All Window Contens";
if (saved_toolbarsize!=toolBarSizeFactor || savedFont!=qApp->font())// save_gui_fontsize!=qApp->font().pixelSize())
{
create_line_Patterns();
read_basic_icons();
updateAllSelectors();
if (FormSetAppearance!=NULL) FormSetAppearance->flp->tabMa->updateSymbolTypeIcons();
if (FormFontTool!=NULL) FormFontTool->updateGlyphSize();
}
UpdateAllWindowContents();
mainWin->mainArea->completeRedraw();
immediateUpdate=chkImmediateUpdate->isChecked();//finally read the immediate-update-settings
lock_dirtystate(FALSE);
}

void frm_Preferences::read_GUI(void)
{
    use_new_icons=chkNewIcons->isChecked();
    default_Print_Device=selDefaultPrintDevice->currentValue();

    //Display the customization-Settings
    show_Navi_B=chkShowNavi->isChecked();
    show_Graph_List=chkShowGraph->isChecked();
    show_special_Zoom=chkShowSpecZoom->isChecked();
    //show_Viewport_Stack=chkShowViewp->isChecked();
    show_Viewport_Stack=selShowViewp->currentIndex();
    show_Page_Zoom=chkShowPageZoom->isChecked();
    show_Print_B=chkShowPrintB->isChecked();
    show_Export_B=chkShowExportP->isChecked();
    /*show_PanButton=chkShowPanB->isChecked();
    show_PickButton=chkShowPickB->isChecked();*/
    show_PanButton=selShowPan->currentIndex();
    show_PickButton=selShowPick->currentIndex();
    show_UndoRedo_B=chkShowUndoRedo->isChecked();
    show_host_name=chkShowHostName->isChecked();
    show_display_name=chkShowDisplay->isChecked();
    displ_project_filename=selFileDisplay1->currentIndex();
    displ_export_filename=selFileDisplay2->currentIndex();
    toolBarSizeFactor=selToolbarSize->value();

    int newlanguage=selLanguage->currentIndex();
        if (newlanguage!=current_language)
        {
            current_language=newlanguage;
            /*char dummy[512];
            strcpy(dummy,"[Warning] Changing the language takes effekt after restart of QtGrace.");
            stufftext(dummy);*/
        }
        strcpy(default_grace_file,lenDefaultFile->lenText->text().toLocal8Bit().constData());
        initial_x_pos=selStartupX->value();
        initial_y_pos=selStartupY->value();
        initial_width=selStartupWidth->value();
        initial_height=selStartupHeight->value();
        initial_size_behavior=selStartupBehavior->currentValue();
        start_dpi=selStdDpi->value();
//init();
        mainWin->ManageBars();
        set_left_footer(NULL);
        FormDeviceSetup->changeDeviceList(2);
}

void frm_Preferences::read_Behavior(void)
{
    general_behavior=selGeneral->currentValue();
    //FileCodec=QTextCodec::codecForName(selCodec->cmbSelect->currentText().toLocal8Bit().constData());
    ///starting with v0.2.7 QtGrace always uses UTF-8
    auto_set_agr_extension=chkAutoSetAgr->isChecked()==true?TRUE:FALSE;
    auto_set_export_extensions=chkAutoSetExport->isChecked()==true?TRUE:FALSE;
    auto_set_cwd=chkAutoSetCWD->isChecked()==true?TRUE:FALSE;
    autofit_on_load=chkAutoFitLoad->isChecked()==true?TRUE:FALSE;
    warn_on_encoding_change=chkWarnOnEncodingChange->isChecked()==true?TRUE:FALSE;
    if (show_transparency_selector!=selTranspSelection->currentValue())
    {
    show_transparency_selector=selTranspSelection->currentValue();
    update_alpha_selectors();
    }
    activateLaTeXsupport=chkActivateLaTeXSupport->isChecked()==true?1:0;
//immediateUpdate=chkImmediateUpdate->isChecked();//we do update this at the end of the Preferences-reading because we do not want it to be active during all updates!!
    inline_editing=chkEnableInlineEditing->isChecked();

    default_compress_data=selCompression->method();
    default_compression_level=selCompression->level();
    if (default_compression_level==0 && default_compress_data!=COMPRESSION_NONE)
    {
    stufftext(tr("[Warning] Compression level set to 0. Compression turned off.").toLocal8Bit().constData());
    selCompression->setMethod(COMPRESSION_NONE);
    default_compress_data=COMPRESSION_NONE;
    }
    context_menu_in_main=chkEnableContextMenuInMain->isChecked()==true?TRUE:FALSE;
    autoPackSets=chkAutoPackSets->isChecked()==true?TRUE:FALSE;
    icon_preferences=selIconBehavior->currentIndex();

    useErrorbarsInAutoscale=chkErrorbarsAutoscale->isChecked()==true?TRUE:FALSE;
    minAutoscaleBorderOffset=selAutoscaleOffset->value();
    allow_gestures=chkAllowGestures->isChecked()==true?TRUE:FALSE;
    allow_wheel_zoom=chkAllowWheel->isChecked()==true?TRUE:FALSE;
    allow_wheel_changes=chkAllowWheelChanges->isChecked()==true?TRUE:FALSE;

    highlight_color=selHighlightColor->currentIndex();
    highlight_alpha=selHighlightColor->alpha();
    highlight_time=selHighlightTime->currentIndex();
    auto_highlight=((chkAutoHighlight->isChecked()==true)?TRUE:FALSE);
    highlight_errorbars=((chkHighlightErrorbars->isChecked()==true)?TRUE:FALSE);

    drop_comment=selDropComment->currentIndex();
    drop_legend=selDropLegend->currentIndex();
    drop_autoscale=(chkDropAutoscale->isChecked()==true?TRUE:FALSE);
    drop_dec_sep=(selDropDecSep->currentIndex()==0?'.':',');
    strcpy(default_csv_format,selCSVDropFormat->lenFile->lenText->text().toLocal8Bit().constData());

    if (undo_active!=chkUndoActive->isChecked())
    {
    undo_active=chkUndoActive->isChecked();
        if (FormUndoList!=NULL)
        FormUndoList->chkActive->setChecked(undo_active);
    }
    start_with_undo_active=chkUndoActiveStart->isChecked();
//cout << "Read: redisplay" << endl;
    if (FormSetAppearance!=NULL)
    {
    FormSetAppearance->flp->tabMa->cmbSymbType->redisplayItems();
    }
//cout << "Ende Read: redisplay" << endl;
    //universal_font_size_factor=selFontSize->value()*0.01;
    bool oldQtFonts=useQtFonts;
    bool old_symbol_special=symbol_font_is_special;
    useQtFonts=chkQtFonts->isChecked();
    symbol_font_is_special=chkSymbolSpecial->isChecked();

    //use_new_print_dialog=chkNewPrintDialog->isChecked();
    //DefaultFont=defaultFont->currentIndex();
    OldDecimalPoint=DecimalPointToUse;
    DecimalPointToUse=(selDecSep->currentIndex()==0)?'.':',';
    /// if (DecimalPointToUse!=OldDecimalPoint)
    /// {
    /// UpdateAllWindowContents();
    /// }
    max_history=histSize->value();
//dirtystate=save_dirty;
    mainWin->recreateHistory();
    if (oldQtFonts!=useQtFonts || old_symbol_special!=symbol_font_is_special)//we change from old to new qt-fonts
    {
        update_font_selectors(true);
        copy_LaTeX_to_Grace();
        convert_Grace_Strings();
    }
}

void frm_Preferences::read_Misc(void)
{
    use_fftw3=chkUseFFTW3->isChecked();
    use_libHaru=chkUselibHaru->isChecked();
    path_to_fftw3_lib.setFileName(ledFFTW3_dll->text());
    path_to_libharu.setFileName(ledHaru_dll->text());

    set_print_cmd(lenPrintCommand->text().toLatin1().constData());
    use_print_command=chkUsePrintCommand->isChecked();
    useHDPrinterOutput=chkHDPrinterOutput->isChecked()==true?TRUE:FALSE;
    typicalPrinterResolution=selPrintDpi->value();
    display_help_externally=chkExternalHelpViewer->isChecked();
    showhideworkaround=chkShowHideException->isChecked();
    comment_server_client=chkAnnounceRemote->isChecked()==true?TRUE:FALSE;
    strcpy(qtgrace_help_viewer,lenHelpViewer->text().toLocal8Bit().constData());

    int old_small_screen=small_screen_adjustments;
    small_screen_adjustments=0;
    if (chkSmallScreenMain->isChecked()==true) small_screen_adjustments|=1;
    if (chkSmallScreenDialogs->isChecked()==true) small_screen_adjustments|=2;

    if (small_screen_adjustments!=old_small_screen)
    {
        if (FormPlotAppearance!=NULL)
        {
            if (small_screen_adjustments & 2)
            {
            FormPlotAppearance->setMinimumSize(0,0);
            }
            else
            {
            FormPlotAppearance->setMinimumSize(FormPlotAppearance->min_w,FormPlotAppearance->min_h);
            }
        FormPlotAppearance->resize(FormPlotAppearance->min_w,FormPlotAppearance->min_h);
        }
        if (FormAxisProperties!=NULL)
        {
            if (small_screen_adjustments & 2)
            {
            FormAxisProperties->setMinimumSize(0,0);
            }
            else
            {
            FormAxisProperties->setMinimumSize(FormAxisProperties->min_w,FormAxisProperties->min_h);
            }
        FormAxisProperties->resize(FormAxisProperties->min_w,FormAxisProperties->min_h);
        }
        if (FormGraphAppearance!=NULL)
        {
            if (small_screen_adjustments & 2)
            {
            FormGraphAppearance->setMinimumSize(0,0);
            }
            else
            {
            FormGraphAppearance->setMinimumSize(FormGraphAppearance->min_w,FormGraphAppearance->min_h);
            }
        FormGraphAppearance->resize(FormGraphAppearance->min_w,FormGraphAppearance->min_h);
        }
        if (FormSetAppearance!=NULL)
        {
            if (small_screen_adjustments & 2)
            {
            FormSetAppearance->setMinimumSize(0,0);
            }
            else
            {
            FormSetAppearance->setMinimumSize(FormSetAppearance->min_w,FormSetAppearance->min_h);
            }
        FormSetAppearance->resize(FormSetAppearance->min_w,FormSetAppearance->min_h);
        }
            if (small_screen_adjustments & 2)
            {
            this->setMinimumSize(0,0);
            }
            else
            {
            this->setMinimumSize(this->min_w,this->min_h);
            }
            this->resize(min_w,min_h);
    }
}

void frm_Preferences::tab_changed(int nr)
{
    /*if (nr==4)
    {
    tab_linestyles->selStyles->setCurrentIndex(1);
    tab_linestyles->currentStyleChanged(1);
    }*/
if (nr>=4)
{
buttons->setVisible(false);
}
else
{
buttons->setVisible(true);
}

}

void frm_Preferences::redisplayContents(void)
{
    QLocale newLocale=(DecimalPointToUse=='.')?(*dot_locale):(*comma_locale);
    /*selFontSize->setLocale(newLocale);
    selFontSize->setValue(Form_Preferences->selFontSize->value());*/
    tab_defaults->selLineWidth->setLocale(newLocale);
    tab_defaults->selLineWidth->setValue(Form_Preferences->tab_defaults->selLineWidth->value());
    /*for (int i=0;i<4;i++)
    tab_defaults->selviewp[i]->ReplaceNumberContents();*/
    tab_defaults->selviewp[0]->setDoubleValue("%.3g",tab_defaults->show_view.xv1);
    tab_defaults->selviewp[1]->setDoubleValue("%.3g",tab_defaults->show_view.yv1);
    tab_defaults->selviewp[2]->setDoubleValue("%.3g",tab_defaults->show_view.xv2);
    tab_defaults->selviewp[3]->setDoubleValue("%.3g",tab_defaults->show_view.yv2);
    selAutoscaleOffset->setLocale(newLocale);
    selAutoscaleOffset->setValue(selAutoscaleOffset->value());
    //tab_defaults->selviewp[i]->RedisplayContents();
    selToolbarSize->setLocale(newLocale);
    selToolbarSize->setValue(selToolbarSize->value());

    tab_prefs->layout0->update();
    tab_prefs->layout1->update();
    tab_prefs->layout2->update();
    tab_prefs->layout3->update();
    tab_prefs->layout->update();

    tab_defaults->layout0->update();
    tab_defaults->layout->update();
    tab_defaults->layoutSetList->update();
    tab_defaults->layoutColumnFormat->update();

    tab_linestyles->hbox->update();
    tab_linestyles->layout->update();

    tab_colors->layout0->update();
    tab_colors->layout1->update();
    tab_colors->layout->update();

    tab_DefFormats->layout0->update();
    tab_DefFormats->subLayout->update();
    tab_DefFormats->layout1->update();
    tab_DefFormats->layout2->update();
    tab_DefFormats->layout->update();

    guiLayout->update();
    layoutToolBar->update();
    layoutStatusBar->update();
    layoutAppFont->update();
    layoutBackgroundColor->update();
    layout2->update();
    layout3->update();
    layoutIniPos->update();
    behavLayout->update();
    layoutLoadSave->update();
    layoutInput->update();
    layoutResponse->update();
    layoutHighlight->update();
    layoutDrop->update();
    miscLayout->update();
    layout_misc0->update();
    layout_misc1->update();
    misc2Layout->update();
    misc3Layout->update();
    layoutRemote->update();
    vbox->update();
}

void frm_Preferences::doClose(void)
{
hide();
}

void frm_Preferences::doApply(void)
{
/// Apply the different Applys --> do a special Apply for the new three tabs
    read();
}

void frm_Preferences::doAccept(void)
{
ApplyError=false;
doApply();
    if (ApplyError==false)
    hide();
}

void frm_Preferences::doGraceDefaults(void)
{
chkShowNavi->setChecked(true);
chkShowGraph->setChecked(false);
chkShowSpecZoom->setChecked(true);
//chkShowViewp->setChecked(true);
selShowViewp->setCurrentIndex(1);
chkShowPageZoom->setChecked(false);
chkShowPrintB->setChecked(false);
chkShowExportP->setChecked(false);
chkShowHostName->setChecked(true);
chkShowDisplay->setChecked(true);
selFileDisplay1->setCurrentIndex(1);
selFileDisplay2->setCurrentIndex(0);
    read_GUI();
}

void frm_Preferences::doQtGraceDefaults(void)
{
chkShowNavi->setChecked(true);
chkShowGraph->setChecked(true);
chkShowSpecZoom->setChecked(true);
//chkShowViewp->setChecked(true);
selShowViewp->setCurrentIndex(2);
chkShowPageZoom->setChecked(true);
chkShowPrintB->setChecked(false);
chkShowExportP->setChecked(false);
chkShowHostName->setChecked(false);
chkShowDisplay->setChecked(false);
selFileDisplay1->setCurrentIndex(2);
selFileDisplay2->setCurrentIndex(2);
    read_GUI();
}

void frm_Preferences::doActDevs(void)
{
    if (diaDevAct==NULL)
    {
    diaDevAct=new frmDeviceActivator(this);
    }
diaDevAct->init();
diaDevAct->show();
diaDevAct->activateWindow();
}

void frm_Preferences::changeGUIFont(void)
{
bool ok;
QFont n_font=QFontDialog::getFont(&ok,*GuiFont,Form_Preferences,tr("Select Gui Font"));
if (ok==true)
{
    QFontInfo fi(n_font);
    n_font.setPixelSize(fi.pixelSize());
    delete GuiFont;
    GuiFont=new QFont(n_font);
/*qDebug() << "font.pointSize=" << n_font.pointSize() << " fi.pointSize=" << fi.pointSize();
qDebug() << "font.pixelSize=" << n_font.pixelSize() << " fi.pixelSize=" << fi.pixelSize();*/
    //QApplication::setFont(n_font);
    qApp->setFont(n_font);
    QString font_descr=QApplication::font().toString();
//qDebug() << "setting Font=" << font_descr.toLatin1().constData();
    lblGuiFont->setText(font_descr);
    Form_Preferences->show();
    Form_Preferences->activateWindow();
    Form_Preferences->raise();
    updateAllSelectors();
    UpdateAllWindowContents();
}
}

void frm_Preferences::resetGUIFont(void)
{
    QString font_descr=stdGuiFont->toString();
    lblGuiFont->setText(font_descr);
    //cout << "setting Font=" << font_descr.toLatin1().constData() << endl;
    //QApplication::setFont(*stdGuiFont);
    qApp->setFont(*stdGuiFont);
    delete GuiFont;
    GuiFont=new QFont(*stdGuiFont);
    updateAllSelectors();
    UpdateAllWindowContents();
}

void frm_Preferences::select_BG_Color(void)
{
//QColor tmp;
//cout << "select a color" << endl;
    QColor tmp=QColorDialog::getColor();
    if (tmp.isValid())
    mainWin->mainArea->setBGtoColor(tmp);
}

void frm_Preferences::set_BG_Color_to_Std(void)
{
    QColor tmp;
    QPalette Pal(mainWin->palette());
    //tmp=Pal.color(QPalette::Background);
    tmp=Pal.color(QPalette::Window);
    mainWin->mainArea->setBGtoColor(tmp);
}

void frm_Preferences::set_BG_Color_to_Page_BG(void)
{
    QColor tmp;
    tmp=get_Color(getbgcolor());
    if (getbgfill()==FALSE)
    set_BG_Color_to_Std();
    else
    mainWin->mainArea->setBGtoColor(tmp);
}

void frm_Preferences::doBrowseStartup(void)
{
QString templ_dir=QString(qt_grace_templates_dir);
    QString def=QFileDialog::getOpenFileName(this,tr("Select default startup file"),templ_dir);
    if (!def.isEmpty())
    {
        int ret=QMessageBox::question(this,tr("Absolute or relative"),tr("Do you want to store the location relative to the QtGrace-templates-directory?\nIf you press 'No' the absolute path will be used."),QMessageBox::Yes,QMessageBox::No);
        if (ret==QMessageBox::Yes)//relative
        {
        //QFileInfo fi(def);
        //cout << "Absolute=" << fi.absoluteFilePath().toLocal8Bit().constData() << endl;
        QDir di(templ_dir);
        //cout << "Relative=" << relPath.toLocal8Bit().constData() << endl;
        lenDefaultFile->lenText->setText(di.relativeFilePath(def));
        }
        else
        {
        lenDefaultFile->lenText->setText(def);
        }
    }
}

void frm_Preferences::doCurrentAsStartup(void)
{
    selStartupX->setValue(mainWin->x());
    selStartupY->setValue(mainWin->y());
    selStartupWidth->setValue(mainWin->width());
    selStartupHeight->setValue(mainWin->height());
}

void frm_Preferences::toggleHTMLviewer(int entry)
{
lenHelpViewer->setEnabled((bool)entry);
}

void frm_Preferences::togglePrintCommand(int entry)
{
lenPrintCommand->setEnabled((bool)entry);
}

void frm_Preferences::toggleQtFonts(bool check)
{
    if (check==true)
    chkSymbolSpecial->setEnabled(true);
    else
    chkSymbolSpecial->setEnabled(false);
}

void frm_Preferences::doBrowseFFTW3_dll(void)
{
QFileInfo fi(path_to_fftw3_lib);
QString old_path=fi.absolutePath();
QString new_file;
#ifdef WINDOWS_SYSTEM
new_file=QFileDialog::getOpenFileName(this,tr("Select FFTW3-Library"),old_path,"*.dll");
#endif
#ifdef MAC_SYSTEM
new_file=QFileDialog::getOpenFileName(this,tr("Select FFTW3-Library"),old_path,"*.dylib");
#else
    #ifdef LINUX_SYSTEM
    new_file=QFileDialog::getOpenFileName(this,tr("Select FFTW3-Library"),old_path,"*.so");
    #endif
#endif
if (!new_file.isEmpty())
{
ledFFTW3_dll->setText(new_file);
path_to_fftw3_lib.setFileName(new_file);
init();
}
}

void frm_Preferences::doBrowseHaru_dll(void)
{
QFileInfo fi(path_to_libharu);
QString old_path=fi.absolutePath();
QString new_file;
#ifdef WINDOWS_SYSTEM
new_file=QFileDialog::getOpenFileName(this,tr("Select libHaru-PDF-Library"),old_path,"*.dll");
#endif
#ifdef MAC_SYSTEM
new_file=QFileDialog::getOpenFileName(this,tr("Select libHaru-PDF-Library"),old_path,"*.dylib");
#else
    #ifdef LINUX_SYSTEM
    new_file=QFileDialog::getOpenFileName(this,tr("Select libHaru-PDF-Library"),old_path,"*.so");
    #endif
#endif
if (!new_file.isEmpty())
{
ledHaru_dll->setText(new_file);
path_to_libharu.setFileName(new_file);
init();
}
}

void frm_Preferences::doSummonWizard(void)
{
show_setup_wizard=TRUE;
setup_wizard_manual=TRUE;
this->hide();
QTimer::singleShot(200,mainWin,SLOT(showWizard()));
}

void frm_Preferences::doTest(void)
{
frmTest->show();
}

void frm_Preferences::resizeEvent(QResizeEvent *event)
{
//cout << "resize: " << event->size().width() << "x" << event->size().height() << " bar_w=" << bar_w << " bar_h=" << bar_h << endl;
//return;
int n_size_w=event->size().width(),n_size_h=event->size().height();
int actual_space_w=n_size_w,actual_space_h=n_size_h;
if (small_screen_adjustments & 2)
{
    for (int i=0;i<2;i++)
    {
        if (actual_space_w<min_w)
        {
            n_size_w=min_w;
            actual_space_h=event->size().height()-bar_h;
            scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        }
        else
        {
            scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
        if (actual_space_h<min_h)
        {
            n_size_h=min_h;
            actual_space_w=event->size().width()-bar_w;
            scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        }
        else
        {
            scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }
if (scroll->verticalScrollBarPolicy()==Qt::ScrollBarAlwaysOn) n_size_w-=bar_w;
if (scroll->horizontalScrollBarPolicy()==Qt::ScrollBarAlwaysOn) n_size_h-=bar_h;
this->setMinimumSize(0,0);
}
else
{
scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
this->setMinimumSize(min_w,min_h);
}
flp->resize(QSize(n_size_w,n_size_h));
}


frmDefaults::frmDefaults(QWidget * parent):QWidget(parent)
{
grp_defaults=new QGroupBox(tr("Grace-Defaults"),this);
layout0=new QGridLayout(grp_defaults);
//layout0->setMargin(STD_MARGIN);
layout0->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
layout0->setSpacing(STD_SPACING);
selStdColor=new ColorSelector(grp_defaults);
selStdColor->lblText->setText(tr("Standard color:"));
selBGColor=new ColorSelector(grp_defaults);
selBGColor->lblText->setText(tr("Standard background color:"));
selStdPattern=new FillPatternSelector(grp_defaults);
selLineStyle=new LineStyleSelector(grp_defaults);
selLineWidth=new LineWidthSelector(grp_defaults);
sldStdCharSize=new stdSlider(grp_defaults,tr("Character size"),0,1000);
selStdFont=new FontSelector(grp_defaults);
sldStdSymSize=new stdSlider(grp_defaults,tr("Symbol size"),0,1000);
lenDefaultFormat=new stdLineEdit(this,tr("Default data point format:"));
cmdUseThisForCurrent=new QPushButton(tr("Use these defaults as current defaults"),grp_defaults);
cmdUseThisForProjectFile=new QPushButton(tr("Use these defaults for this project file"),grp_defaults);
int nr=4;
QString * entr=new QString[NUM_FMT_OPTION_ITEMS+4];
entr[0]=tr("Current");
entr[1]=tr("from project file");
entr[2]=tr("from ini file");
entr[3]=tr("Grace default");
selShowDefaults=new StdSelector(this,tr("Show defaults:"),nr,entr);
connect(selShowDefaults,SIGNAL(currentIndexChanged(int)),SLOT(currentShowDefaultsChanged(int)));

lblviewp=new QLabel(tr("Initial viewport for new graphs:"),this);
selviewp[0]=new stdLineEdit(this,tr("Xmin:"));
selviewp[1]=new stdLineEdit(this,tr("Ymin:"));
selviewp[2]=new stdLineEdit(this,tr("Xmax:"));
selviewp[3]=new stdLineEdit(this,tr("Ymax:"));

grp_defaults->setToolTip(tr("The settings in this group are applied as initial settings for new objects\n(like graphs, texts, lines, sets, ...)\nnot applied to existing objects and not applied to the default-startup-agr-file"));
/*selStdColor->setToolTip(tr(""));
selBGColor->setToolTip(tr(""));
selStdPattern->setToolTip(tr(""));
selLineStyle->setToolTip(tr(""));
selLineWidth->setToolTip(tr(""));
sldStdCharSize->setToolTip(tr(""));
selStdFont->setToolTip(tr(""));
sldStdSymSize->setToolTip(tr(""));*/
lenDefaultFormat->setToolTip(tr("Defaults data point format in C notation"));
cmdUseThisForCurrent->setToolTip(tr("Use the displayed defaults as the current ones\n(the current defaults are always stored in the ini-file)"));
cmdUseThisForProjectFile->setToolTip(tr("Store these defaults in the project file on next save"));
/*lblviewp->setToolTip(tr(""));
selviewp[0]->setToolTip(tr(""));
selviewp[1]->setToolTip(tr(""));
selviewp[2]->setToolTip(tr(""));
selviewp[3]->setToolTip(tr(""));*/
selShowDefaults->setToolTip(tr("Which defaults are to displayed here\n(only the current ones are used)"));

grp_SetLists=new QGroupBox(tr("List(s) of sets"),this);
layoutSetList=new QGridLayout();
//layoutSetList->setMargin(STD_MARGIN);
layoutSetList->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
layoutSetList->setSpacing(STD_SPACING);
chkShowDataLess=new QCheckBox(tr("Show data-less sets"),this);
chkShowDataLess->setToolTip(tr("Show sets with no datapoints in every list of sets (if present)."));
chkShowHidden=new QCheckBox(tr("Show hidden sets"),this);
chkShowHidden->setToolTip(tr("Show hidden sets in every list of sets (if present)."));
chkShowComment=new QCheckBox(tr("Show comments"),this);
chkShowComment->setToolTip(tr("Show the set comment in every list of sets."));
chkShowLegend=new QCheckBox(tr("Show legends"),this);
chkShowLegend->setToolTip(tr("Show the set legend in every list of sets."));
chkShowIcons=new QCheckBox(tr("Show icons"),this);
chkShowIcons->setToolTip(tr("Show an icon representing the set appearance in every list of sets."));
chkActivateDrag=new QCheckBox(tr("Allow set-drag/drop"),this);
chkActivateDrag->setToolTip(tr("Experimental: If this option is activated you can rearrange the order of the sets in a list with drag and drop.\nYou have to select the sets you want to rearrange and keep the left mouse button pressed to drag and drop the sets.\nYou can only drag/drop sets in the same list - to get data in or out you have to use the clipboard via Copy and Paste in the context menu.\nDrawback: With this option enabled you can not select multiple sets with pressed mouse button."));
layoutSetList->addWidget(chkShowDataLess,0,0);
layoutSetList->addWidget(chkShowHidden,0,1);
layoutSetList->addWidget(chkShowComment,1,0);
layoutSetList->addWidget(chkShowLegend,1,1);
layoutSetList->addWidget(chkShowIcons,2,0);
layoutSetList->addWidget(chkActivateDrag,3,0);
grp_SetLists->setLayout(layoutSetList);

layout0->addWidget(selStdColor,0,0,1,2);
layout0->addWidget(selBGColor,1,0,1,2);
layout0->addWidget(selStdPattern,2,0,1,2);
layout0->addWidget(selLineStyle,3,0,1,2);
layout0->addWidget(selLineWidth,4,0,1,2);
layout0->addWidget(sldStdCharSize,5,0,1,3);
layout0->addWidget(selStdFont,6,0,1,3);
layout0->addWidget(sldStdSymSize,7,0,1,3);
layout0->addWidget(lenDefaultFormat,8,0,1,3);
layout0->addWidget(cmdUseThisForCurrent,9,0,1,2);
layout0->addWidget(cmdUseThisForProjectFile,9,2,1,1);
layout0->addWidget(lblviewp,0,2,1,1);
layout0->addWidget(selviewp[0],1,2,1,1);
layout0->addWidget(selviewp[1],2,2,1,1);
layout0->addWidget(selviewp[2],3,2,1,1);
layout0->addWidget(selviewp[3],4,2,1,1);
layout0->setColumnStretch(0,2);
layout0->setColumnStretch(1,2);
layout0->setColumnStretch(2,1);
grp_defaults->setLayout(layout0);

grpColumnFormats=new QGroupBox(tr("Spreadsheet column format"),this);
layoutColumnFormat=new QGridLayout();
//layoutColumnFormat->setMargin(STD_MARGIN);
layoutColumnFormat->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
layoutColumnFormat->setSpacing(STD_SPACING);
/*nr=4;
entr[0]=QString("Decimal");
entr[1]=QString("General");
entr[2]=QString("Exponential");
entr[3]=QString("Date/time");*/
nr=NUM_FMT_OPTION_ITEMS;
for (int i=0;i<nr;i++)
{
entr[i]=QString(fmt_option_items[i].label);
}
selFormat=new StdSelector(grpColumnFormats,tr("Format:"),nr,entr);
selPrecision=new stdIntSelector(grpColumnFormats,tr("Precision:"),0,20);
layoutColumnFormat->addWidget(selFormat,0,0,1,1);
layoutColumnFormat->addWidget(selPrecision,0,1,1,1);
grpColumnFormats->setLayout(layoutColumnFormat);

layout=new QVBoxLayout(this);
//layout->setMargin(STD_MARGIN);
layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
layout->setSpacing(STD_SPACING);
buttonGroup=new stdButtonGroup(this);
layout->addWidget(selShowDefaults);
layout->addWidget(grp_defaults);
layout->addWidget(grp_SetLists);
layout->addWidget(grpColumnFormats);
layout->addStretch(5);
layout->addWidget(buttonGroup);
setLayout(layout);

connect(buttonGroup->cmdApply,SIGNAL(clicked()),SLOT(doApply()));
connect(buttonGroup->cmdAccept,SIGNAL(clicked()),SLOT(doAccept()));
connect(buttonGroup->cmdClose,SIGNAL(clicked()),SLOT(doClose()));

connect(cmdUseThisForCurrent,SIGNAL(clicked()),SLOT(doSetCurrent()));
connect(cmdUseThisForProjectFile,SIGNAL(clicked()),SLOT(doSetFile()));

currentShowDefaultsChanged(0);
}

void frmDefaults::show_current_defaults(void)
{
selStdColor->setCurrentIndex(show_defaults.color);
selBGColor->setCurrentIndex(show_defaults.bgcolor);
selStdColor->setAlpha(show_defaults.alpha);
selBGColor->setAlpha(show_defaults.bgalpha);
selStdPattern->setCurrentIndex(show_defaults.pattern);
selLineStyle->setCurrentIndex(show_defaults.lines);
selLineWidth->setValue(show_defaults.linew);
sldStdCharSize->setValue((int)rint_2(show_defaults.charsize*100.0));
selStdFont->setCurrentIndex(show_defaults.font);
sldStdSymSize->setValue((int)rint_2(show_defaults.symsize*100.0));

int ret=is_valid_format_specifier(show_sformat);
if (ret==0)//invalid
{
format_specifier_error(show_sformat);
strcpy(show_sformat,grace_sformat);
}

lenDefaultFormat->setText(QString(show_sformat));
selviewp[0]->setDoubleValue("%.3g",show_view.xv1);
selviewp[1]->setDoubleValue("%.3g",show_view.yv1);
selviewp[2]->setDoubleValue("%.3g",show_view.xv2);
selviewp[3]->setDoubleValue("%.3g",show_view.yv2);
/*selviewp[0]->setValue(show_view.xv1);
selviewp[1]->setValue(show_view.yv1);
selviewp[2]->setValue(show_view.xv2);
selviewp[3]->setValue(show_view.yv2);*/
}

void frmDefaults::init(void)
{
//defaults
/*selStdColor->setCurrentIndex(grdefaults.color);
selBGColor->setCurrentIndex(grdefaults.bgcolor);
selStdPattern->setCurrentIndex(grdefaults.pattern);
selLineStyle->setCurrentIndex(grdefaults.lines);
selLineWidth->setValue(grdefaults.linew);
sldStdCharSize->setValue(int(grdefaults.charsize*100.0));
selStdFont->setCurrentIndex(grdefaults.font);
sldStdSymSize->setValue(int(grdefaults.symsize*100.0));
lenDefaultFormat->setText(QString(sformat));*/
show_current_defaults();

chkShowDataLess->setChecked(DefaultSetListShowDataless);
chkShowHidden->setChecked(DefaultSetListShowHidden);
chkShowComment->setChecked(DefaultSetListShowComments);
chkShowLegend->setChecked(DefaultSetListShowLegends);
chkShowIcons->setChecked(DefaultSetListShowIcons);
chkActivateDrag->setChecked(allow_drag_in_lists);

selFormat->setCurrentValue(SpreadsheetColumnFormat);
selPrecision->setValue(SpreadsheetColumnPrecision);
}

void frmDefaults::doSetCurrent(void)
{
readDefaultSettings();
grdefaults=show_defaults;
grview=show_view;
strcpy(sformat,show_sformat);
selShowDefaults->setCurrentIndex(0);
}

void frmDefaults::doSetFile(void)
{
readDefaultSettings();
file_defaults=show_defaults;
file_view=show_view;
strcpy(file_sformat,show_sformat);
selShowDefaults->setCurrentIndex(1);
}

void frmDefaults::readDefaultSettings(void)
{
show_defaults.color=selStdColor->currentIndex();
show_defaults.bgcolor=selBGColor->currentIndex();
show_defaults.alpha=selStdColor->alpha();
show_defaults.bgalpha=selBGColor->alpha();
show_defaults.pattern=selStdPattern->currentIndex();
show_defaults.lines=selLineStyle->currentIndex();
show_defaults.linew=selLineWidth->value();
show_defaults.charsize=sldStdCharSize->value()/100.0;
show_defaults.font=selStdFont->currentIndex();
show_defaults.symsize=sldStdSymSize->value()/100.0;
show_view.xv1=selviewp[0]->getDoubleValue();
show_view.yv1=selviewp[1]->getDoubleValue();
show_view.xv2=selviewp[2]->getDoubleValue();
show_view.yv2=selviewp[3]->getDoubleValue();
strcpy(show_sformat,lenDefaultFormat->text().toLocal8Bit().constData());
int ret=is_valid_format_specifier(show_sformat);
if (ret==0)//invalid
{
format_specifier_error(show_sformat);
strcpy(show_sformat,grace_sformat);
}
}

void frmDefaults::doApply(void)
{
    if (selShowDefaults->currentIndex()==0)
    {
    doSetCurrent();
    }
    DefaultSetListShowDataless=chkShowDataLess->isChecked()==true?TRUE:FALSE;
    DefaultSetListShowHidden=chkShowHidden->isChecked()==true?TRUE:FALSE;
    DefaultSetListShowComments=chkShowComment->isChecked()==true?TRUE:FALSE;
    DefaultSetListShowLegends=chkShowLegend->isChecked()==true?TRUE:FALSE;
    DefaultSetListShowIcons=chkShowIcons->isChecked()==true?TRUE:FALSE;
    allow_drag_in_lists=chkActivateDrag->isChecked()==true?TRUE:FALSE;
    changeDragInUniLists();
    setDefaultsInSetList();

    SpreadsheetColumnFormat=selFormat->currentValue();
    SpreadsheetColumnPrecision=selPrecision->value();

    init();
    //update_set_lists(-1);
    mainWin->mainArea->completeRedraw();
}

void frmDefaults::doAccept(void)
{
doApply();
doClose();
}

void frmDefaults::doClose(void)
{
emit(close_wish());
}

void frmDefaults::currentShowDefaultsChanged(int index)
{
int ret;
    switch (index)
    {
    case 0://current
    show_defaults=grdefaults;
    show_view=grview;
    ret=is_valid_format_specifier(sformat);
    if (ret==0)//invalid
    {
    format_specifier_error(sformat);
    strcpy(sformat,"%.8g");
    }
    strcpy(show_sformat,sformat);
    //cmdResetToGraceStd->setEnabled(false);
    grp_defaults->setTitle(tr("Current defaults"));
    break;
    case 1://from project file
    show_defaults=file_defaults;
    show_view=file_view;
    ret=is_valid_format_specifier(file_sformat);
    if (ret==0)//invalid
    {
    format_specifier_error(file_sformat);
    strcpy(file_sformat,"%.8g");
    }
    strcpy(show_sformat,file_sformat);
    //cmdResetToGraceStd->setEnabled(true);
    grp_defaults->setTitle(tr("Defaults from project file"));
    break;
    case 2://from ini file
    show_defaults=ini_defaults;
    show_view=ini_view;
    ret=is_valid_format_specifier(ini_sformat);
    if (ret==0)//invalid
    {
    format_specifier_error(ini_sformat);
    strcpy(ini_sformat,"%.8g");
    }
    strcpy(show_sformat,ini_sformat);
    //cmdResetToGraceStd->setEnabled(true);
    grp_defaults->setTitle(tr("Defaults from ini file"));
    break;
    case 3://grace default
    show_defaults=d_d;
    show_view=d_v;
    ret=is_valid_format_specifier(grace_sformat);
    if (ret==0)//invalid
    {
    format_specifier_error(grace_sformat);
    strcpy(grace_sformat,"%.8g");
    }
    strcpy(show_sformat,grace_sformat);
    //cmdResetToGraceStd->setEnabled(true);
    grp_defaults->setTitle(tr("Grace defaults"));
    break;
    }
    init();
    if (index>=2)//grace defaults and ini-defaults are not editable
    {
    selStdColor->setEnabled(false);
    selBGColor->setEnabled(false);
    selStdPattern->setEnabled(false);
    selLineStyle->setEnabled(false);
    selLineWidth->setEnabled(false);
    sldStdCharSize->setEnabled(false);
    selStdFont->setEnabled(false);
    sldStdSymSize->setEnabled(false);
    lenDefaultFormat->setEnabled(false);
        for (int i=0;i<4;i++)
        selviewp[i]->setEnabled(false);
    }
    else
    {
    selStdColor->setEnabled(true);
    selBGColor->setEnabled(true);
    selStdPattern->setEnabled(true);
    selLineStyle->setEnabled(true);
    selLineWidth->setEnabled(true);
    sldStdCharSize->setEnabled(true);
    selStdFont->setEnabled(true);
    sldStdSymSize->setEnabled(true);
    lenDefaultFormat->setEnabled(true);
        for (int i=0;i<4;i++)
        selviewp[i]->setEnabled(true);
    }

}


