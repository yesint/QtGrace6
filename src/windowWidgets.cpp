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

#include "windowWidgets.h"

#include "undo_module.h"
#include "editWidgets.h"
#include "appearanceWidgets.h"
#include "MainWindow.h"

#define cg get_cg()

int track_setno=-1;
int track_add_at;    /* where to begin inserting points in the set */
int track_move_dir;  /* direction on point movement */
int nrOfUndoObjs;
int * undoObjs;

static T1_TMATRIX UNITY_MATRIX = {1.0, 0.0, 0.0, 1.0};

extern int slider_status;
extern char SystemsDecimalPoint;
extern char OldDecimalPoint;
extern char DecimalPointToUse;
extern QLocale * comma_locale;//a setting where ',' is the decimal separator (we use the setting for Germany here)
extern QLocale * dot_locale;//a setting where '.' is the decimal separator (we use the setting for the USA here)
extern bool ApplyError;
extern bool GlobalInhibitor;
extern bool immediateUpdate;
extern bool updateRunning;
extern QStringList ListOfChanges;
extern QStringList ListOfOldStates;
extern MainWindow * mainWin;
extern char command[];
extern char dummy[];
extern QPixmap * GraceIcon;
extern CanvasAction action_flag;
extern imageinfo bg_fill_image;
extern imageinfo bg_box_fill_image;
extern imageinfo bg_ellipse_fill_image;
extern bool useQtFonts;
extern bool symbol_font_is_special;
extern QMap<unsigned char,ushort> key_for_greek;
extern int nb_rt;
extern Input_buffer *ib_tbl;
extern int ib_tblsize;
extern int rtiCheckTime;
extern char last_formula[];
extern char saved_formula[];
extern double last_fit_falues[];
extern double saved_value;
extern bool activate_id_replacing;
extern int replace_o_set_ids,replace_n_set_ids;//number of set ids (original and new set ids - meaning right and left hand side of an equation)
//if set-id is '-1' a new set is to be allocated and the new id is writen in the id-arrays
//gnos always have to be real ids and no '-1'
extern int * replace_o_gnos;
extern int * replace_o_snos;
extern int * replace_n_gnos;
extern int * replace_n_snos;

extern QSize LastSize_EllipseProps;
extern QSize LastSize_EditEllipseProps;
extern QSize LastSize_BoxProps;
extern QSize LastSize_EditBoxProps;
extern QSize LastSize_TextProps;
extern QSize LastSize_EditTextProps;
extern QSize LastSize_LineProps;
extern QSize LastSize_EditLineProps;
extern QSize LastSize_FormDrawObjects;
extern QSize LastSize_FormConsole;
extern QSize LastSize_FormPointExplorer;
extern QSize LastSize_FormFontTool;
extern QSize LastSize_FormRTIManage;
extern QSize LastSize_FormCommands;

extern int maxboxes;
extern int maxlines;
extern int maxstr;
extern int maxellipses;
extern int swap_line_coords;
extern int next_line_id;

extern frmSetAppearance * FormSetAppearance;
extern frmConsole * FormConsole;
extern frmTextProps * TextProps;
extern frmTextProps * EditTextProps;
extern frmLineProps * LineProps;
extern frmLineProps * EditLineProps;
extern frmEllipseProps * BoxProps;
extern frmEllipseProps * EditBoxProps;
extern frmEllipseProps * EllipseProps;
extern frmEllipseProps * EditEllipseProps;
extern frmDrawObjects * FormDrawObjects;
extern frmFontTool * FormFontTool;
extern frmCommands * FormCommands;
extern frmReportOnFitParameters * FormReportFitParameters;
extern frmProgressWin * FormProgress;

extern void SetDecimalSeparatorToUserValue(char * str,bool remove_space=true);
extern int nr_of_true_changes(QStringList & list);
extern int get_QtFontID_from_Grace_Name(const char * name,int whatlist);
extern int openPipe(char * pname,int * fd);
extern int should_open_grace_file_dialog(void);
extern void sort(int number,int * items);
extern void parse_qtGrace_Additions(char * s);
extern void clear_new_set_ids(void);
extern double rint_v2(double x);
extern void remove_beginning_whitespaces(QString & text);
extern void remove_beginning_whitespaces2(char * text);
extern void save_set_comments(QList<int> & gnos,QList<int> & snos);
extern void restore_set_comments(int c);//0=comments only, 1=legends only, 2=comments and legends

#ifdef __cplusplus
extern "C" {
#endif
extern int current_origin_graph,current_origin_set;
extern int current_target_graph,current_target_set;
extern int error_count;
#ifdef __cplusplus
}
#endif

frmCommands::frmCommands(QWidget * parent):QDialog(parent)
{
//setFont(*stdFont);
    min_w=500;
    min_h=500;
    bar_w=bar_h=20;
    scroll=new QScrollArea();
    flp=new QWidget();
    scroll->setWidget(flp);
    QVBoxLayout * m_layout=new QVBoxLayout();
    //m_layout->setMargin(0);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    m_layout->addWidget(scroll);

    setWindowTitle(tr("QtGrace: Commands"));

    setWindowIcon(QIcon(*GraceIcon));

    FormReadHistory=NULL;
    FormWriteHistory=NULL;

    lblCommand=new QLabel(tr("Command:"),this);
    lenCommand=new QLineEdit(QString(""),this);
    //txtCommands=new QTextEdit(this);
    list=new QListWidget(this);
    connect(list,SIGNAL(itemDoubleClicked(QListWidgetItem*)),SLOT(doDoubleClick(QListWidgetItem*)));
    grpBox1=new QGroupBox(this);
    grpBox2=new QGroupBox(this);

    grpBox3=new QGroupBox(this);
    layout3=new QGridLayout();
    //layout3->setMargin(STD_MARGIN);
    layout3->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout3->setSpacing(STD_SPACING);
    grpSource=new grpSelect(tr("Source"),this);
    grpDestination=new grpSelect(tr("Destination"),this);
    grpDestination->enable_sync(grpSource->listSet);
    cmdReplayWithReplace=new QPushButton(tr("Replay replacing set-Ids"),this);
    connect(cmdReplayWithReplace,SIGNAL(clicked()),SLOT(doReplayWithReplace()));
    layout3->addWidget(grpSource,0,0);
    layout3->addWidget(grpDestination,0,1);
    layout3->addWidget(cmdReplayWithReplace,1,0,1,2);
    grpBox3->setLayout(layout3);

    layout1=new QHBoxLayout();
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout1->setSpacing(STD_SPACING);
    layout2=new QHBoxLayout();
    //layout2->setMargin(STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout2->setSpacing(STD_SPACING);
    layout=new QVBoxLayout();
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->setSpacing(STD_SPACING);

    cmdAdd=new QPushButton(tr("Add"),grpBox1);
    connect(cmdAdd,SIGNAL(clicked()),SLOT(doAdd()));
    cmdDelete=new QPushButton(tr("Delete"),grpBox1);
    connect(cmdDelete,SIGNAL(clicked()),SLOT(doDelete()));
    cmdReplace=new QPushButton(tr("Replace"),grpBox1);
    connect(cmdReplace,SIGNAL(clicked()),SLOT(doReplace()));
    cmdUp=new QPushButton(tr("Up"),grpBox1);
    connect(cmdUp,SIGNAL(clicked()),SLOT(doUp()));
    cmdDown=new QPushButton(tr("Down"),grpBox1);
    connect(cmdDown,SIGNAL(clicked()),SLOT(doDown()));

    layout1->addWidget(cmdAdd);
    layout1->addWidget(cmdDelete);
    layout1->addWidget(cmdReplace);
    layout1->addWidget(cmdUp);
    layout1->addWidget(cmdDown);
    grpBox1->setLayout(layout1);

    cmdRead=new QPushButton(tr("Read..."),grpBox2);
    connect(cmdRead,SIGNAL(clicked()),SLOT(doRead()));
    cmdSave=new QPushButton(tr("Save..."),grpBox2);
    connect(cmdSave,SIGNAL(clicked()),SLOT(doSave()));
    cmdClear=new QPushButton(tr("Clear"),grpBox2);
    connect(cmdClear,SIGNAL(clicked()),SLOT(doClear()));
    cmdReplay=new QPushButton(tr("Replay"),grpBox2);
    connect(cmdReplay,SIGNAL(clicked()),SLOT(doReplay()));
    cmdClose=new QPushButton(tr("Close"),grpBox2);
    connect(cmdClose,SIGNAL(clicked()),SLOT(doClose()));
    cmdHelp=new QPushButton(tr("Help"),grpBox2);
    connect(cmdHelp,SIGNAL(clicked()),SLOT(doHelp()));

    layout2->addWidget(cmdRead);
    layout2->addWidget(cmdSave);
    layout2->addWidget(cmdClear);
    layout2->addWidget(cmdReplay);
    layout2->addWidget(cmdClose);
    layout2->addWidget(cmdHelp);
    grpBox2->setLayout(layout2);

    grpSpecial=new QGroupBox(this);
    layout4=new QHBoxLayout(this);
    layout4->setSpacing(STD_SPACING);
    //layout4->setMargin(STD_MARGIN);
    layout4->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    lblSpecial=new QLabel(tr("Special Command:"),this);
    cmbSpecial=new QComboBox(this);
    cmbSpecial->addItem("None");
    cmbSpecial->addItem("Add last formula");
    cmbSpecial->addItem("Minus last formula");
    cmbSpecial->addItem("Multiply with last formula");
    cmbSpecial->addItem("Divide by last formula");
    cmbSpecial->addItem("Remember value");
    cmbSpecial->addItem("Add last value");
    cmbSpecial->addItem("Minus last Value");
    cmbSpecial->addItem("Multiply with value");
    cmbSpecial->addItem("Divide by value");
    cmbSpecial->addItem("Use last formula");
    cmbSpecial->addItem("Use remembered formula");
    cmbSpecial->addItem("Create new Set");
    cmbSpecial->addItem("Extract feature");
    cmbSpecial->addItem("Special formula");
    cmbSpecial->addItem("Append data point");
    cmbSpecial->addItem("Filter");
    cmbSpecial->addItem("Regression");
    cmbSpecial->addItem("if-clause");

    cmbSpecial2=new QComboBox(this);
    cmbSpecial2->addItem(tr("---"));

    cmdSpecial=new QPushButton(tr("Insert"),this);
    connect(cmbSpecial,SIGNAL(currentIndexChanged(int)),SLOT(Special1Changed(int)));
    connect(cmbSpecial2,SIGNAL(currentIndexChanged(int)),SLOT(Special2Changed(int)));
    connect(cmdSpecial,SIGNAL(clicked()),SLOT(doInsertSpecial()));
    layout4->addWidget(lblSpecial);
    layout4->addWidget(cmbSpecial);
    layout4->addWidget(cmbSpecial2);
    layout4->addWidget(cmdSpecial);
    grpSpecial->setLayout(layout4);

    //layout->addWidget(txtCommands);
    layout->addWidget(grpBox3);
    layout->addWidget(list);
    layout->addWidget(grpBox1);
    layout->addWidget(grpBox2);
    layout->addWidget(grpSpecial);
    layout->addWidget(lblCommand);
    layout->addWidget(lenCommand);
    layout->setStretchFactor(grpBox3,3);
    layout->setStretchFactor(list,3);
    layout->setStretchFactor(grpBox1,0);
    layout->setStretchFactor(grpBox2,0);
    layout->setStretchFactor(lblCommand,0);
    layout->setStretchFactor(lenCommand,0);

flp->setLayout(layout);
setLayout(m_layout);
    //this->resize(QSize(min_w+5,min_h+5));
    resize(LastSize_FormCommands);
}

frmCommands::~frmCommands()
{
    LastSize_FormCommands=this->size();
}

void frmCommands::doAdd(void)
/*
 * copy the contents of the command line to the story list without executing it
 */
{
    int npos, *pos=new int[2], newpos;
    QString comtxt=lenCommand->text();
    if (!comtxt.isEmpty())
    {
        getListSelection(&npos,&pos);
        if (npos==0)
        {
            newpos=0;
        }
        else
        {
            newpos=pos[0]+1;
        }
        list->insertItem(newpos,comtxt);
        updateRowColor(newpos);
        list->setCurrentRow(newpos);
        list->scrollToItem(list->item(newpos));
    }
    lenCommand->clear();
    delete[] pos;
}

void frmCommands::getListSelection(int * number,int ** selection)
{
    *number=0;
    for (int i=0;i<list->count();i++)
        if (list->item(i)->isSelected()) *number=*number+1;
    //QModelIndexList listentries=selectedIndexes();
    //*number=listentries.size();
    if (*number<=0) return;
    delete[] *selection;
    *selection=new int[*number];
    *number=0;
    for (int i=0;i<list->count();i++)
    {
        //*(*selection+i)=listentries.at(i)->data( listentries.at(i).row();
        if (list->item(i)->isSelected())
        {
            *(*selection+*number)=i;
            *number=*number+1;
        }
    }
    sort(*number,*selection);
}

void frmCommands::doDelete(void)
/* delete a entry from the history list */
{
    QListWidgetItem * wi;
    for (int i=0;i<list->count();i++)
    {
        if (list->item(i)->isSelected())
        {
            wi=list->takeItem(i);
            delete wi;
        }
    }
    list->hide();
    list->show();
}

void frmCommands::doReplace(void)
/*
 * replace a entry in the history list with the command line
 * without executing it
 */
{
    int nr,*sel=new int[5];
    getListSelection(&nr,&sel);
    if (nr!=1)
    {
        delete[] sel;
        return;
    }
    list->insertItem(sel[0]+1,lenCommand->text());
    updateRowColor(sel[0]+1);
    list->takeItem(sel[0]);
    list->scrollToItem(list->item(sel[0]));
    delete[] sel;
}

void frmCommands::doUp(void)
{
    int sel_numb=-1;
    for (int i=0;i<list->count();i++)
    {
        if (list->item(i)->isSelected())
        {
            sel_numb=i;
        }
    }
    if (sel_numb>0 && sel_numb<=list->count())
    {
        QListWidgetItem * wi,*wi2;
        wi=list->takeItem(sel_numb-1);
        wi2=list->takeItem(sel_numb-1);
        list->insertItem(sel_numb-1,wi);
        list->insertItem(sel_numb-1,wi2);
        list->setCurrentItem(wi2);
        list->hide();
        list->show();
    }
}

void frmCommands::doDown(void)
{
    int sel_numb=-1;
    for (int i=0;i<list->count();i++)
    {
        if (list->item(i)->isSelected())
        {
            sel_numb=i;
        }
    }
    if (sel_numb>=0 && sel_numb<list->count())
    {
        QListWidgetItem * wi,*wi2;
        wi=list->takeItem(sel_numb);
        wi2=list->takeItem(sel_numb);
        list->insertItem(sel_numb,wi);
        list->insertItem(sel_numb,wi2);
        list->setCurrentItem(wi);
        list->hide();
        list->show();
    }
}

void frmCommands::doRead(void)
{
    QString FileExtension("*.com");
    QString sep=QDir::separator();
    QString script_dir(qt_grace_script_dir);
    QString init_file;
    script_dir=QDir::toNativeSeparators(QDir::cleanPath(script_dir));
    script_dir=script_dir.replace(sep+sep,sep);
    script_dir=script_dir+sep;
//qDebug() << "script_dir=" << script_dir;
//qDebug() << "LastDialogPath=" << Last_Dialog_Path[READ_COMMANDS_FILE];
if (should_open_grace_file_dialog())
{
    if (FormReadHistory==NULL)
    {
        FormReadHistory=new frmIOForm(READ_PARAMETERS,this);
        connect(FormReadHistory,SIGNAL(newFileSelectedForIO(int,QString,bool,bool,bool)),SLOT(IOrequested(int,QString,bool,bool,bool)));
        //FormReadHistory->selector->filterExtension=FileExtension;
        FormReadHistory->setWindowTitle(tr("QtGrace: Read history"));
        FormReadHistory->grpParamGraph->hide();
        FormReadHistory->selector->showFilesLikeFilter();
    }
    //FormReadHistory->init(Last_Dialog_Path[READ_COMMANDS_FILE].toLocal8Bit().data());
    init_file=script_dir+QString("*.com");
//qDebug() << "init_file=" << init_file;
    FormReadHistory->init(init_file.toLocal8Bit().data());
    FormReadHistory->selector->setFilterFromExtern(script_dir,FileExtension);
    FormReadHistory->show();
    FormReadHistory->raise();
    FormReadHistory->activateWindow();
}
else
{
mainWin->UseOperatingSystemFileDialog(READ_COMMANDS_FILE,tr("QtGrace: Read history"),script_dir,tr("Command files (*.com);;All files (*)"));
}
}

void frmCommands::doSave(void)
{
    QString FileExtension("*.com");
    QString sep=QDir::separator();
    QString script_dir(qt_grace_script_dir);
    QString init_file;
    script_dir=QDir::toNativeSeparators(QDir::cleanPath(script_dir));
    script_dir=script_dir.replace(sep+sep,sep);
    script_dir=script_dir+sep;
//qDebug() << "script_dir=" << script_dir;
//qDebug() << "LastDialogPath=" << Last_Dialog_Path[READ_COMMANDS_FILE];
if (should_open_grace_file_dialog())
{
    if (FormWriteHistory==NULL)
    {
        FormWriteHistory=new frmIOForm(WRITE_PARAMETERS,this);
        connect(FormWriteHistory,SIGNAL(newFileSelectedForIO(int,QString,bool,bool,bool)),SLOT(IOrequested(int,QString,bool,bool,bool)));
        //FormWriteHistory->selector->setFilterFromExtern(script_dir,FileExtension);
        //FormWriteHistory->selector->filterExtension=FileExtension;
        FormWriteHistory->setWindowTitle(tr("QtGrace: Write history"));
        FormWriteHistory->grpParamGraph->hide();
        FormWriteHistory->selector->showFilesLikeFilter();
    }
    //FormWriteHistory->init(Last_Dialog_Path[WRITE_COMMANDS_FILE].toLocal8Bit().data());
    init_file=script_dir+QString("*.com");
//qDebug() << "init_file=" << init_file;
    FormWriteHistory->init(init_file.toLocal8Bit().data());
    FormWriteHistory->selector->setFilterFromExtern(script_dir,FileExtension);
    FormWriteHistory->show();
    FormWriteHistory->raise();
    FormWriteHistory->activateWindow();
}
else
{
mainWin->UseOperatingSystemFileDialog(WRITE_COMMANDS_FILE,tr("QtGrace: Write history"),script_dir,tr("Command files (*.com);;All files (*)"));
}
}

void frmCommands::IOrequested(int type,QString file,bool exists,bool writeable,bool readable)
{
    char filename[256];
    strcpy(filename,file.toLocal8Bit());
    char buf[512];
    FILE *fp,*pp;
    (void)exists;
    (void)writeable;
    (void)readable;
    if (type==WRITE_PARAMETERS || type==WRITE_COMMANDS_FILE)
    {
        int hc=list->count(),i;
        QListWidgetItem * lwid;
        pp = grace_openw(filename);
        if (pp != NULL) {
            for (i = 0; i < hc; i++) {
                lwid=list->item(i);
                strcpy(buf,lwid->text().toLocal8Bit());
                fprintf(pp, "%s\n", buf);
            }
            grace_close(pp);
        }
        if (FormWriteHistory)
        FormWriteHistory->hide();
    }
    else//READ_PARAMETERS || READ_COMMMANDS_FILE
    {
        int sl;
        if ((fp = grace_openr(filename, SOURCE_DISK)) != NULL) {
            while (grace_fgets(buf, 255, fp) != NULL) {
                sl = strlen(buf);
                buf[sl - 1] = 0;
                if (strlen(buf) == 0) {
                    continue;
                }
                list->addItem(QString::fromLocal8Bit(buf));
                updateRowColor(list->count()-1);
                list->scrollToItem(list->item(list->count()-1));
            }
            grace_close(fp);
        }
        if (FormReadHistory)
        FormReadHistory->hide();
    }
}

int frmCommands::next_unused_new_set(void)
{
    int hc=list->count();
    int * nr_list=new int[hc+2];
    int len_nr_list=0;
    int i,nex=0,tm,tm2;
    bool exists;
    QListWidgetItem * lwid;
    char * ts,ts2[64],ts3[64];
    strcpy(ts2,"#QTGRACE_SPECIAL CREATE NEW_SET ");
    for (i = 0; i < hc; i++)
    {
        lwid=list->item(i);
        ts=new char[2+strlen(lwid->text().toLatin1())];
        strcpy(ts,lwid->text().toLatin1());//commands should be representable in Latin1-encoding
        strncpy(ts3,ts,strlen(ts2));
        ts3[strlen(ts2)]='\0';
        if (strcmp(ts2,ts3)==0)//we found a special command to create a set
        {
        tm2=sscanf(ts+strlen(ts2),"S%dN",&tm);
        if (tm2==1)
        nr_list[len_nr_list++]=tm;
        }
        delete[] ts;
    }
    if (len_nr_list<=0) return 0;
    tm=nr_list[0];//we search for the maximum
    for (i=0;i<len_nr_list;i++) if (nr_list[i]>tm) tm=nr_list[i];
    nex=tm+1;
    for (i=0;i<nex;i++)//look for an unoccupied value lower than nex
    {
    exists=false;
        for (int j=0;j<len_nr_list;j++)
        {
            if (nr_list[j]==i)
            {
            exists=true;
            break;
            }
        }
        if (exists==false)
        {
        nex=i;
        break;
        }
    }
    delete[] nr_list;
return nex;
}

void frmCommands::updateRowColor(int r)
{
int comment_type=formula_to_process::is_comment(list->item(r)->text());
switch (comment_type)
{
case 0://0=no comment
list->item(r)->setForeground(Qt::black);
break;
case 1://1=comment
list->item(r)->setForeground(Qt::darkGreen);
break;
case 2://2=special
list->item(r)->setForeground(Qt::magenta);
break;
case 3://3=if
list->item(r)->setForeground(Qt::blue);
break;
}
}

void frmCommands::resizeEvent(QResizeEvent * event)
{
//cout << "resize: " << event->size().width() << "x" << event->size().height() << " bar_w=" << bar_w << " bar_h=" << bar_h << endl;
//return;
int n_size_w=event->size().width(),n_size_h=event->size().height();
int actual_space_w=n_size_w,actual_space_h=n_size_h;
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
flp->resize(QSize(n_size_w,n_size_h));
}

void frmCommands::doClear(void)
{
    int ret=QMessageBox::question(this,tr("Are you sure?"),tr("Delete all entries from list of commands?"),QMessageBox::Yes,QMessageBox::No);
    if (ret==QMessageBox::Yes)
        list->clear();
}

#define MAXERR 5

void frmCommands::doReplay(void)
{
    int errpos;
    char ts[512];//,ts2[32];
    int i;
    int hc=list->count();
    QListWidgetItem * lwid;
    error_count = 0;
    activate_id_replacing=false;
    clear_new_set_ids();//prepare for the generation of new set-ids
    for (i = 0; i < hc; i++)
    {
        lwid=list->item(i);
        strcpy(ts,lwid->text().toLatin1());//commands should be representable in Latin1-encoding
        /*strncpy(ts2,ts,16);
        ts2[16]='\0';
        if (strcmp(ts2,"#QTGRACE_SPECIAL") == 0)
            errpos = special_Scanner(ts+17,false);
            else*/
        //qDebug() << "Replay: ts=#" << ts << "#";
            if (ts[0]=='#')
            parse_qtGrace_Additions(ts);
            else
            errpos = scanner(ts);//the scanner is able to process the #QTGRACE_SPECIAL-commands
        if (errpos)
        {
            error_count++;
        }
        if (error_count > MAXERR)
        {
            if (yesno("Lots of errors, cancel?", NULL, NULL, NULL))
            {
                break;
            }
            else
            {
                error_count = 0;
            }
        }
    }
    mainWin->mainArea->completeRedraw();
    if (FormReportFitParameters!=NULL)
    {
    FormReportFitParameters->init();
    }
}

int frmCommands::special_Scanner(char * command,bool replace)
{
    QString replayed_command;
    char operation[512],operand[512],temp_val[512];
    int pos=0,len=0;
//char * com;
    int o_n_sets,n_sets,type,realization,absolute,debug,point_extension,oversampling,rno,invr;
    int * o_gnos=NULL;
    int * o_snos=NULL;
    int * gnos=NULL;
    int * snos=NULL;
    double limits[2];
    int orders[2];
    char x_formula[512];
    double ripple;
    (void)replace;
    cout << "#" << command << "#" << endl;
    sscanf(command,"%s %s",operation,operand);
    cout << "#"<< operation << "#" << operand << "#" << endl;
    if (strcmp(operation,"USE")==0)
    {
        if (strcmp(operand,"LAST_FORMULA")==0)
        {
            replayed_command=QString(last_formula);
        }
        else//Remembered Formula
        {
            replayed_command=QString(saved_formula);
        }
    }
    else if (strcmp(operation,"REMEMBER")==0)
    {
        if (strcmp(operand,"Formula")==0)
        {
            strcpy(saved_formula,last_formula);
        }
        else if (operand[4]=='a')
        {
            pos=atoi(operand+5);
            saved_value=last_fit_falues[pos];
            //cout << "saved_value=" << saved_value << endl;
        }
        else//special case
        {

        }
        /// Save something
    }
    else if (strcmp(operation,"FILTER_SET")==0)
    {
        cout << "parse filter" << endl;

        ParseFilterCommand(operand,o_n_sets,&o_gnos,&o_snos,n_sets,&gnos,&snos,type,realization,limits,orders,x_formula,ripple,absolute,debug,point_extension,oversampling,rno,invr);

    }
    else if (strcmp(operation,"REGRESSION")==0)
    {
        cout << "parse regression" << endl;

    }
    else//not "USE" and not "REMEMBER"-command
    {
        replayed_command=QString("Y=Y");
        if (strcmp(operation,"ADD")==0)
        {
            replayed_command+=QString("+(");
        }
        else if (strcmp(operation,"MINUS")==0)
        {
            replayed_command+=QString("-(");
        }
        else if (strcmp(operation,"MULTIPLY")==0)
        {
            replayed_command+=QString("*(");
        }
        else if (strcmp(operation,"DIVIDE")==0)
        {
            replayed_command+=QString("/(");
        }
        if (strcmp(operand,"LAST_VALUE")==0)
        {
            sprintf(temp_val,"%g",saved_value);
            replayed_command+=QString(temp_val);
        }
        else if (strcmp(operand,"LAST_FORMULA")==0)
        {
            len=strlen(last_formula);
            pos=0;
            for (int i=0;i<len;i++){if (last_formula[i]=='='){pos=i; break;}}
            replayed_command+=QString(last_formula+pos);
        }
        else//SAVED_FORMULA
        {
            len=strlen(saved_formula);
            pos=0;
            for (int i=0;i<len;i++){if (saved_formula[i]=='='){pos=i; break;}}
            replayed_command+=QString(saved_formula+pos);
        }
        replayed_command+=QString(")");
    }
    /// replace set ids
    /// execute command (scanner...)
qDebug() << "replayed Command #" << replayed_command.toLatin1().constData() << "#";
    strcpy(temp_val,replayed_command.toLatin1().constData());
    return scanner(temp_val);
}

void replace_set_nrs_and_graph_nrs(QString & command,int setnr,int graphnr)
{
static QString s_rep("S#");
static QString g_rep("G#");
QString s_nr=QString::number(setnr);
QString g_nr=QString::number(graphnr);
command.replace(s_rep,s_nr);
command.replace(g_rep,g_nr);
}

void frmCommands::doReplayWithReplace(void)
{
    int nr_src,nr_dest;
    int src_gno,dest_gno;
    int * srcSets=new int[2];
    int * destSets=new int[2];
    int stop_with_error=0;
    int i,return_value;
    int hc=list->count();
    QListWidgetItem * lwid;
    QStringList ListOfCommands;
    QString commandString,lhs,rhs;
    QString replacedString;
    /*int errpos;
    char ts[1024];
    char dummy_repl_str[512];
    bool containsEquals;
    int specialType;
    struct FoundSetID * foundIDs=new struct FoundSetID[4];
    char * parameters;
    int nr=0;*/
    class formula_to_process formula1;
    QString all_formulas_in_one,temp_string;

    inhibit_set_legend=1;
    /*int nr_unique_ids=0,*unique_id=new int[2];
    int o_n_sets,n_sets;
    int * o_gnos=NULL;
    int * o_snos=NULL;
    int * gnos=NULL;
    int * snos=NULL;
    int type,realization,absolute,debug,point_extension,oversampling,rno,invr;
    double limits[2];
    int orders[2];
    char x_formula[MAX_STRING_LENGTH];
    double ripple;*/
//qDebug() << "Start of ReplayReplace: g0.s0.legend=" << get_legend_string(0,0);
    grpSource->listSet->get_selection(&nr_src,&srcSets);
    grpDestination->listGraph->get_selection(&nr_dest,&destSets);
    src_gno=grpSource->listSet->gr_no;
    if (nr_src<1)
    {
        errmsg(tr("Please select at least one source set!").toLocal8Bit().constData());
        stop_with_error=1;
        goto end_of_replay_with_replace;
    }
    if (nr_dest!=1)
    {
        errmsg(tr("Please select a single graph as destination!").toLocal8Bit().constData());
        stop_with_error=1;
        goto end_of_replay_with_replace;
    }
    else
    {
        dest_gno=destSets[0];
    }
    grpDestination->listSet->get_selection(&nr_dest,&destSets);
    if (nr_dest>0 && nr_dest!=nr_src)
    {
        errmsg(tr("Number of destination sets does not match number of source sets!").toLocal8Bit().constData());
        stop_with_error=1;
        goto end_of_replay_with_replace;
    }

    /// do we need this?
    if (nr_dest>0)
    {
    QList<int> sets_to_save;
    QList<int> gnos_to_save;
    sets_to_save.clear();
    gnos_to_save.clear();
        for (int i=0;i<nr_dest;i++)
        {
        gnos_to_save << dest_gno;
        sets_to_save << destSets[i];
        }
    save_set_comments(gnos_to_save,sets_to_save);
    }

    replace_o_set_ids=nr_src;
    replace_n_set_ids=nr_src;//the number of sets in source and destination have to be the same -- set-id=-1 if no destination has been selected
    //number of set ids (original and new set ids - meaning right and left hand side of an equation)
    //if set-id is '-1' a new set is to be allocated and the new id is writen in the id-arrays
    //gnos always have to be real ids and no '-1'
    if (replace_o_gnos!=NULL) delete[] replace_o_gnos;
    if (replace_o_snos!=NULL) delete[] replace_o_snos;
    if (replace_n_gnos!=NULL) delete[] replace_n_gnos;
    if (replace_n_snos!=NULL) delete[] replace_n_snos;
    replace_o_gnos=new int[nr_src];
    replace_n_gnos=new int[nr_src];
    replace_o_snos=new int[nr_src];
    replace_n_snos=new int[nr_src];

    for (int i=0;i<nr_src;i++)
    {
        replace_o_gnos[i]=src_gno;
        replace_n_gnos[i]=dest_gno;
        replace_o_snos[i]=srcSets[i];
        if (nr_dest==0)
        {
            replace_n_snos[i]=nextset(dest_gno);
            copyset(src_gno,srcSets[i],dest_gno,replace_n_snos[i]);
        }
        else
        {
            replace_n_snos[i]=destSets[i];
        }
    }

    activate_id_replacing=true;
    error_count = 0;

    ListOfCommands.clear();
    all_formulas_in_one.clear();
    for (i = 0; i < hc; i++)//copy every command in the list (needed to be able to alter the commands on the fly)
    {
    lwid=list->item(i);
    temp_string=lwid->text();
    ListOfCommands << lwid->text();
    remove_beginning_whitespaces(temp_string);
        if (temp_string.length()>0)
        {
            //if (temp_string.at(0).toLatin1()!='#')
            if (formula_to_process::is_comment(temp_string)!=1)
            {
            //qDebug() << "temp_string=" << temp_string;
            all_formulas_in_one+=temp_string;
            if (i<hc-1) all_formulas_in_one+=QString(";");
            }
        }
    }
    formula1.init_formula(all_formulas_in_one);

//qDebug() << "REPLAY: AllFormulas=" << all_formulas_in_one;
/// this command is obsolete
clear_new_set_ids();//prepare for the generation of new set-ids
    FormProgress->init(tr("Processing commands..."),nr_src-1);
    FormProgress->show();
    FormProgress->raise();
    FormProgress->activateWindow();
    qApp->processEvents();
    for (int jj=0;jj<nr_src;jj++)//do the commands for every set in the source-list
    {//loop over all selected sets
    FormProgress->increase();
        replace_o_set_ids=replace_n_set_ids=1;
        replace_o_snos[0]=srcSets[jj];
            if (nr_dest==0)
            replace_n_snos[0]=-1;
            else
            replace_n_snos[0]=destSets[jj];

        current_origin_set=replace_o_snos[0];
        current_origin_graph=replace_o_gnos[0];
        current_target_set=replace_n_snos[0];
        current_target_graph=replace_n_gnos[0];

        (void)formula1.execute_formula(current_target_graph,current_target_set,current_origin_graph,current_origin_set,1,return_value);
        /*replacedString=formula1.execute_formula(current_target_graph,current_target_set,current_origin_graph,current_origin_set,1,return_value);
qDebug() << "REPLAY: replacedFormula=" << replacedString;*/
/*strcpy(dummy_repl_str,replacedString.toLatin1().constData());
qDebug() << "REPLAY: dummy_repl_str=" << dummy_repl_str;*/

    //formula1.increase_counters();//this one is already part of the execute_formula-command
    }//end of loop through all set-IDs
    FormProgress->hide();
end_of_replay_with_replace:
    inhibit_set_legend=0;
//qDebug() << "End1 of ReplayReplace: g0.s0.legend=" << get_legend_string(0,0);
    /// do we need this?
    if (nr_dest>0 && stop_with_error==0)
    {
    restore_set_comments(0);
    }
    mainWin->mainArea->completeRedraw();
//qDebug() << "End2 of ReplayReplace: g0.s0.legend=" << get_legend_string(0,0);
    activate_id_replacing=false;
    if (FormReportFitParameters!=NULL)
    {
    FormReportFitParameters->init();
    }
    delete[] srcSets;
    delete[] destSets;
}

void frmCommands::doClose(void)
{
    hide();
}

void frmCommands::doHelp(void)
{
    HelpCB("doc/UsersGuide.html#commands");
}

QString specialExtractCommand(int type,int gno,int sno,double data)
{
    QString newCommand("#QTGRACE_SPECIAL ");
    QString ID=QString("G")+QString::number(gno)+QString(".S")+QString::number(sno);
    char s_dummy[1024];
    sprintf(s_dummy,sformat,data);
    QString dbl_value(s_dummy);
    newCommand+=QString("EXTRACT a19=");
        switch (type)
        {
        case 0:
            newCommand=QString("a19=MIN(")+ID+QString(".Y)");
            break;
        case 1:
            newCommand=QString("a19=MAX(")+ID+QString(".Y)");
            break;
        case 2:
            newCommand=QString("a19=AVG(")+ID+QString(".Y)");
            break;
        case 3:
            newCommand=QString("a19=SD(")+ID+QString(".Y)");
            break;
        case 4:
            newCommand+=QString("MEDIAN(Y)");
            break;
        case 5:
            newCommand=QString("a19=MIN(")+ID+QString(".X)");
            break;
        case 6:
            newCommand=QString("a19=MAX(")+ID+QString(".X)");
            break;
        case 7:
            newCommand=QString("a19=AVG(")+ID+QString(".X)");
            break;
        case 8:
            newCommand=QString("a19=SD(")+ID+QString(".X)");
            break;
        case 9:
            newCommand+=QString("MEDIAN(X)");
            break;
        case 10:
            newCommand+=QString("FREQUENCY()");
            break;
        case 11:
            newCommand+=QString("PERIOD()");
            break;
        case 12:
            newCommand+=QString("ZERO_CROSSING()");
            break;
        case 13:
            newCommand+=QString("RISE_TIME()");
            break;
        case 14:
            newCommand+=QString("FALL_TIME()");
            break;
        case 15:
            newCommand+=QString("SLOPE()");
            break;
        case 16:
            newCommand+=QString("Y_INTERCEPTION()");
            break;
        case 17:
            newCommand+=QString("SET_LENGTH()");
            break;
        case 18:
            newCommand+=QString("HALF_MAX_WIDTH()");
            break;
        case 19:
            newCommand+=QString("BARYCENTER_X()");
            break;
        case 20:
            newCommand+=QString("BARYCENTER_Y()");
            break;
        case 21:
            newCommand=QString("a19=")+ID+QString(".X[IMAX(")+ID+QString(".Y)]");
            break;
        case 22:
            newCommand=QString("a19=")+ID+QString(".Y[IMAX(")+ID+QString(".X)]");
            break;
        case 23:
            newCommand=QString("a19=INT(")+ID+QString(".X,")+ID+QString(".Y)");
            break;
        case 24:
            newCommand+=QString("Y_VALUE_CROSSING(")+dbl_value+QString(")");
            break;
        case 25:
            newCommand+=QString("X_VALUE_CROSSING(")+dbl_value+QString(")");
            break;
        case 26:
            newCommand=QString("a19=")+ID+QString(".X[IMIN(")+ID+QString(".Y)]");
            break;
        case 27:
            newCommand=QString("a19=")+ID+QString(".Y[IMIN(")+ID+QString(".X)]");
            break;
        }
return newCommand;
}

void frmCommands::doInsertSpecial(void)
{
    QString newCommand("#QTGRACE_SPECIAL ");
    int nr=cmbSpecial->currentIndex();
    if (nr==0) return;
    int n_set;
//cout << "inserting" << endl;
    switch (nr)
    {
    case 1://"Add last Formula"
        newCommand+=QString("ADD LAST_FORMULA");
        break;
    case 2://"Minus last Formula"
        newCommand+=QString("MINUS LAST_FORMULA");
        break;
    case 3://"Multiply with last Formula"
        newCommand+=QString("MULTIPLY LAST_FORMULA");
        break;
    case 4://"Divide by last Formula"
        newCommand+=QString("DIVIDE LAST_FORMULA");
        break;
    case 5://"Remember Value"
        newCommand+=QString("REMEMBER ")+cmbSpecial2->currentText();
        /// Hier fehlt VILLEICHT was!
        break;
    case 6://"Add last Value"
        newCommand+=QString("ADD LAST_VALUE");
        break;
    case 7://"Minus last Value"
        newCommand+=QString("MINUS LAST_VALUE");
        break;
    case 8://"Multiply with Value"
        newCommand+=QString("MULTIPLY LAST_VALUE");
        break;
    case 9://"Divide by Value"
        newCommand+=QString("DIVIDE LAST_VALUE");
        break;
    case 10://"Use Last Formula"
        newCommand+=QString("USE LAST_FORMULA");
        break;
    case 11://"Use Remembered Formula"
        newCommand+=QString("USE SAVED_FORMULA");
        break;
    case 12://"Create New Set"
        newCommand+=QString("CREATE NEW_SET ");
        n_set=next_unused_new_set();
        newCommand=QString("S")+QString::number(n_set)+QString("N");//I removed the '+' at the beginning (command=S0N without CREATE_NEW)
        break;
    case 13://"Special Formula"
        /// INSERT FUNCTION HERE
        newCommand=specialExtractCommand(cmbSpecial2->currentIndex(),0,0,0.0);
        break;
    case 15://"APPEND New Point"
        newCommand+=QString("APPEND G0.S0{0.0;0.0}");
        break;
    case 16://"Filter"
        newCommand+=QString("FILTER_SET <nr_of_source_sets>,<nr_of_target_sets>{<first_source_graph>,<first_source_set>;<second_source_graph>,<second_source_set>;...}{<first_destination_graph>,<first_destination_set>;...}{<type>;<realization>;<order_first_cutoff>;<order_second_cutoff>;<absolute_values>;<debug>;<point_extension>;<oversampling>;<region_nr>;<negate_region>;<first_frequency(Hz)>;<second_frequency(Hz)>;<chebychev_ripples(dB)>;<x_transformation_formula>}");
        break;
    case 17://"Regression"
        newCommand+=QString("REGRESSION <nr_of_source_sets>,<nr_of_target_sets>{<first_source_graph>,<first_source_set>;<second_source_graph>,<second_source_set>;...}{<Type_of_fit>,<restriction>,<invert_region>,<nr_of_points>,<Load>,<start>,<stop>,<x_$t_formula>}");
        break;
    case 18://"if"
        newCommand=QString("#if[a19>1] G0.S0.Y=G0.S0.Y*1");
        break;
    default://"None"
        /*Do nothing*/
        break;
    }

    int npos, *pos=new int[2], newpos;
    getListSelection(&npos,&pos);
    if (npos==0)
    {
        newpos=0;
    }
    else
    {
        newpos=pos[0]+1;
    }
    list->insertItem(newpos,newCommand);
    updateRowColor(newpos);
    list->setCurrentRow(newpos);
    list->scrollToItem(list->item(newpos));
    delete[] pos;
}

void frmCommands::Special1Changed(int nr)
{
    //cout << "special 1 changed = " << nr << endl;
    cmbSpecial2->clear();
    if (nr==5)//"Remember Value"
    {
        cmbSpecial2->addItem(tr("Formula"));
        cmbSpecial2->addItem(tr("Fit_a0"));
        cmbSpecial2->addItem(tr("Fit_a1"));
        cmbSpecial2->addItem(tr("Fit_a2"));
        cmbSpecial2->addItem(tr("Fit_a3"));
        cmbSpecial2->addItem(tr("Fit_a4"));
        cmbSpecial2->addItem(tr("Fit_a5"));
        cmbSpecial2->addItem(tr("Fit_a6"));
        cmbSpecial2->addItem(tr("Fit_a7"));
        cmbSpecial2->addItem(tr("Fit_a8"));
        cmbSpecial2->addItem(tr("Fit_a9"));
        /*cmbSpecial2->addItem(tr("Sum"));
        cmbSpecial2->addItem(tr("Mean"));*/
    }
    else if (nr==13)//"Special formula"
    {
        cmbSpecial2->addItem(tr("Y min"));
        cmbSpecial2->addItem(tr("Y max"));
        cmbSpecial2->addItem(tr("Y average"));
        cmbSpecial2->addItem(tr("Y std. deviation"));
        cmbSpecial2->addItem(tr("Y median"));
        cmbSpecial2->addItem(tr("X min"));
        cmbSpecial2->addItem(tr("X max"));
        cmbSpecial2->addItem(tr("X average"));
        cmbSpecial2->addItem(tr("X std. deviation"));
        cmbSpecial2->addItem(tr("X median"));
        cmbSpecial2->addItem(tr("Frequency"));
        cmbSpecial2->addItem(tr("Period"));
        cmbSpecial2->addItem(tr("Zero crossing"));
        cmbSpecial2->addItem(tr("Rise time"));
        cmbSpecial2->addItem(tr("Fall time"));
        cmbSpecial2->addItem(tr("Slope"));
        cmbSpecial2->addItem(tr("Y intercept"));
        cmbSpecial2->addItem(tr("Set length"));
        cmbSpecial2->addItem(tr("Half max width"));
        cmbSpecial2->addItem(tr("Barycenter X"));
        cmbSpecial2->addItem(tr("Barycenter Y"));
        cmbSpecial2->addItem(tr("X of Ymax"));
        cmbSpecial2->addItem(tr("Y of Xmax"));
        cmbSpecial2->addItem(tr("Integral"));
        cmbSpecial2->addItem(tr("Y Value crossing"));
        cmbSpecial2->addItem(tr("X Value crossing"));
        cmbSpecial2->addItem(tr("X of Ymin"));
        cmbSpecial2->addItem(tr("Y of Xmin"));
    }
    else
    {
        cmbSpecial2->addItem(tr("---"));
    }
}

void frmCommands::Special2Changed(int nr)
{
    (void)nr;
    //cout << "special 2 changed = " << nr << endl;
}

void frmCommands::doDoubleClick(QListWidgetItem * index)
{
    lenCommand->setText(index->text());
}

void get_tracking_props(int *setno, int *move_dir, int *add_at)
{
    *setno = track_setno;
    *move_dir = track_move_dir;
    *add_at = track_add_at;
}

frmLine_Props::frmLine_Props(QWidget * parent,bool edit):QWidget(parent)
{
    editWindow=edit;
    int number;
    QString * entr=new QString[5];

    /*setFont(*stdFont);
if (edit)
setWindowTitle(tr("Edit Line"));
else
setWindowTitle(tr("Lines"));
setWindowIcon(QIcon(*GraceIcon));*/

    lines_color_item=new ColorSelector(this);
    lines_width_item=new LineWidthSelector(this);
    lines_style_item=new LineStyleSelector(this);

    fraArrow=new QGroupBox(tr("Arrow"),this);
    number=4;
    entr[0]=tr("None");
    entr[1]=tr("Start");
    entr[2]=tr("End");
    entr[3]=tr("Both ends");
    lines_arrow_item=new StdSelector(fraArrow,tr("Place at:"),number,entr);
    number=3;
    entr[0]=tr("Line");
    entr[1]=tr("Filled");
    entr[2]=tr("Opaque");
    lines_atype_item=new StdSelector(fraArrow,tr("Type:"),number,entr);
    lines_asize_item=new LineWidthSelector(fraArrow);
    lines_asize_item->lblText->setText(tr("Length:"));
    lines_asize_item->spnLineWidth->setRange(-10.0,10.0);
    lines_a_dL_ff_item=new LineWidthSelector(fraArrow);
    lines_a_dL_ff_item->lblText->setText(tr("d/L form factor:"));
    lines_a_dL_ff_item->spnLineWidth->setRange(0.0,10.0);
    lines_a_dL_ff_item->spnLineWidth->setSingleStep(0.1);
    lines_a_lL_ff_item=new LineWidthSelector(fraArrow);
    lines_a_lL_ff_item->lblText->setText(tr("l/L form factor:"));
    lines_a_lL_ff_item->spnLineWidth->setRange(-1.0,1.0);
    lines_a_lL_ff_item->spnLineWidth->setSingleStep(0.1);

    selGno=new StdSelector(this,tr("Link to graph:"),number,entr);
    selGno->simple_graph_selector=true;
    selGno->setValuesToGraphs(0,0);

    lines_loc_item=new PositionSelector(this);

    buttonGroup=new stdButtonGroup(this,false);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));

    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout2=new QVBoxLayout;
    //layout2->setMargin(STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout2->addWidget(lines_arrow_item);
    layout2->addWidget(lines_atype_item);
    layout2->addWidget(lines_asize_item);
    layout2->addWidget(lines_a_dL_ff_item);
    layout2->addWidget(lines_a_lL_ff_item);
    fraArrow->setLayout(layout2);

    layout->addWidget(lines_color_item);
    layout->addWidget(lines_width_item);
    layout->addWidget(lines_style_item);
    layout->addWidget(fraArrow);
    layout->addWidget(selGno);
    layout->addWidget(lines_loc_item);
    if (edit)
    {
        ledCoords[0]=new stdLineEdit(this,QString("X1="));
        layout->addWidget(ledCoords[0]);
        ledCoords[1]=new stdLineEdit(this,QString("Y1="));
        layout->addWidget(ledCoords[1]);
        ledCoords[2]=new stdLineEdit(this,QString("X2="));
        layout->addWidget(ledCoords[2]);
        ledCoords[3]=new stdLineEdit(this,QString("Y2="));
        layout->addWidget(ledCoords[3]);

        empty=new QWidget(this);
        cmbStartEnd=new QComboBox(empty);
        cmbStartEnd->addItem(QString("X1 / Y1"));
        cmbStartEnd->addItem(QString("X2 / Y2"));
        cmbStartEnd->setToolTip(tr("Point to edit"));
        cmdEdit=new QPushButton(tr("Edit point"),this);
        cmdEdit->setToolTip(tr("Edit a point by dragging\nit with the cursor.\nThis sets the cursor back to line-creation-mode."));
        layout3=new QHBoxLayout();
        //layout3->setMargin(0);
        layout3->setContentsMargins(0,0,0,0);
        layout3->setSpacing(STD_SPACING);
        layout3->addWidget(cmdEdit);
        layout3->addWidget(cmbStartEnd);
        empty->setLayout(layout3);
        connect(cmdEdit,SIGNAL(clicked()),this,SLOT(doEdit()));
        layout->addWidget(empty);
    }
    layout->addWidget(buttonGroup);
    setLayout(layout);

    lines_loc_item->setCurrentIndex(1);
    lines_color_item->setCurrentIndex(1);
    lines_width_item->setValue(1.0);
    lines_style_item->setCurrentIndex(1);
    lines_asize_item->setValue(1.0);
    lines_a_dL_ff_item->setValue(1.0);
    lines_a_lL_ff_item->setValue(1.0);

    connect(lines_arrow_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(lines_atype_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(lines_asize_item,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(lines_a_dL_ff_item,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(lines_a_lL_ff_item,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(lines_color_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(lines_color_item,SIGNAL(alphaChanged(int)),SLOT(update1(int)));
    connect(selGno,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(lines_width_item,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(lines_style_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(lines_loc_item->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(viewCoordsChanged(int)));
}

void frmLine_Props::init(int id)
{
    if (id>=number_of_lines() || (id<0 && editWindow==true)) return;
    bool sav_imm_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    //char buf[256];
    obj_id=id;
    disconnect(lines_loc_item->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(viewCoordsChanged(int)));
    if (editWindow==false)
    {
        selGno->setVisible(false);
        SetOptionChoice(lines_color_item, line_color);
        lines_color_item->setAlpha(line_alpha);
        SetOptionChoice(lines_style_item, line_lines);
        SetSpinChoice(lines_width_item, line_linew);
        SetChoice(lines_arrow_item, line_arrow_end);
        SetChoice(lines_atype_item, line_atype);
        SetSpinChoice(lines_asize_item, line_asize);
        SetSpinChoice(lines_a_dL_ff_item, line_a_dL_ff);
        SetSpinChoice(lines_a_lL_ff_item, line_a_lL_ff);
        lines_loc_item->setCurrentIndex(line_loctype == COORD_VIEW ? 1 : 0);
    }
    else
    {
        int lineno = obj_id;
        selGno->setValuesToGraphs(0,0);
        selGno->setVisible(true);
        selGno->setCurrentValue(lines[lineno].gno);
        SetOptionChoice(lines_color_item, lines[lineno].color);
        lines_color_item->setAlpha(lines[lineno].alpha);
        SetOptionChoice(lines_style_item, lines[lineno].lines);
        SetSpinChoice(lines_width_item, lines[lineno].linew);
        SetChoice(lines_arrow_item, lines[lineno].arrow_end);
        SetChoice(lines_atype_item, lines[lineno].arrow.type);
        SetSpinChoice(lines_asize_item, lines[lineno].arrow.length);
        SetSpinChoice(lines_a_dL_ff_item, lines[lineno].arrow.dL_ff);
        SetSpinChoice(lines_a_lL_ff_item, lines[lineno].arrow.lL_ff);
        lines_loc_item->setCurrentIndex(lines[lineno].loctype == COORD_VIEW ? 1 : 0);
        ledCoords[0]->setDoubleValue("%.12f",lines[lineno].x1);
        ledCoords[1]->setDoubleValue("%.12f",lines[lineno].y1);
        ledCoords[2]->setDoubleValue("%.12f",lines[lineno].x2);
        ledCoords[3]->setDoubleValue("%.12f",lines[lineno].y2);
    }
    connect(lines_loc_item->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(viewCoordsChanged(int)));
    immediateUpdate=sav_imm_upd;
    updateRunning=false;
}

void frmLine_Props::reDisplayContents(void)
{
QLocale newLocale=(DecimalPointToUse=='.')?(*dot_locale):(*comma_locale);
    lines_a_dL_ff_item->spnLineWidth->setLocale(newLocale);
    lines_a_lL_ff_item->spnLineWidth->setLocale(newLocale);
    lines_width_item->spnLineWidth->setLocale(newLocale);
    lines_asize_item->spnLineWidth->setLocale(newLocale);
    init(obj_id);
    layout->update();
    layout2->update();
}

void frmLine_Props::viewCoordsChanged(int i)
{
    (void)i;
    if (editWindow==false) return;
    if (obj_id>=number_of_lines() || (obj_id<0 && editWindow==true)) return;
    int lineno = obj_id;
    int old_cur_graph=get_cg();
    double k[4];
    if( lines[lineno].loctype == lines_loc_item->currentValue()) return;
    bool old_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;

    xv_evalexpr(ledCoords[0], k+0);
    xv_evalexpr(ledCoords[1], k+1);
    xv_evalexpr(ledCoords[2], k+2);
    xv_evalexpr(ledCoords[3], k+3);

    select_graph(lines[lineno].gno);
    if( lines[lineno].loctype == COORD_VIEW )
    {
        //lines[lineno].gno = get_cg();
        lines[lineno].loctype = COORD_WORLD;
        //view2world( atof(xv_getstr(ledCoords[0])), atof(xv_getstr(ledCoords[1])),&lines[lineno].x1,&lines[lineno].y1 );
        //view2world( atof(xv_getstr(ledCoords[2])), atof(xv_getstr(ledCoords[3])),&lines[lineno].x2,&lines[lineno].y2 );
        view2world( k[0],k[1],&lines[lineno].x1,&lines[lineno].y1 );
        view2world( k[2],k[3],&lines[lineno].x2,&lines[lineno].y2 );
    }
    else
    {
        lines[lineno].loctype = COORD_VIEW;
        //world2view( atof(xv_getstr(ledCoords[0])), atof(xv_getstr(ledCoords[1])),&lines[lineno].x1,&lines[lineno].y1 );
        //world2view( atof(xv_getstr(ledCoords[2])), atof(xv_getstr(ledCoords[3])),&lines[lineno].x2,&lines[lineno].y2 );
        world2view( k[0],k[1],&lines[lineno].x1,&lines[lineno].y1 );
        world2view( k[2],k[3],&lines[lineno].x2,&lines[lineno].y2 );
    }
    select_graph(old_cur_graph);
    init(obj_id);
    immediateUpdate=old_upd;
    updateRunning=false;
}

void frmLine_Props::doAccept(void)
{
if (obj_id>=number_of_lines() || (obj_id<0 && editWindow==true)) return;
    int lineno = obj_id;
    double k[4];
    bool changes=false;
    ListOfChanges.clear();
    ListOfOldStates.clear();
    if (editWindow==false)//edit preferences
    {
        line_asize = GetSpinChoice(lines_asize_item);
        line_color = GetOptionChoice(lines_color_item);
        line_alpha = lines_color_item->alpha();
        line_arrow_end = GetChoice(lines_arrow_item);
        line_atype = GetChoice(lines_atype_item);
        line_a_dL_ff = GetSpinChoice(lines_a_dL_ff_item);
        line_a_lL_ff = GetSpinChoice(lines_a_lL_ff_item);
        line_lines = GetOptionChoice(lines_style_item);
        line_linew = GetSpinChoice(lines_width_item);
        line_loctype = lines_loc_item->currentValue();
    }
    else//edit object
    {
        SaveObjectData(lineno,OBJECT_LINE);
        xv_evalexpr(ledCoords[0], k+0);
        xv_evalexpr(ledCoords[1], k+1);
        xv_evalexpr(ledCoords[2], k+2);
        xv_evalexpr(ledCoords[3], k+3);

        sprintf(dummy,"with line %d",lineno);
        ListOfChanges << QString(dummy);
        ListOfOldStates << QString(dummy);
        if (lines[lineno].color != GetOptionChoice(lines_color_item))
        {
            changes=true;
        }
        sprintf(dummy,"    line color %d",GetOptionChoice(lines_color_item));
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    line color %d",lines[lineno].color);
        ListOfOldStates << QString(dummy);
        //}
        if (lines[lineno].loctype != lines_loc_item->currentValue())
        {
            changes=true;
        }
        sprintf(dummy,"    line loctype %s",!lines_loc_item->currentValue()?"view":"world");
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    line loctype %s",!lines[lineno].loctype?"view":"world");
        ListOfOldStates << QString(dummy);
        //}
        if (lines[lineno].lines != GetOptionChoice(lines_style_item))
        {
            changes=true;
        }
        sprintf(dummy,"    line linestyle %d",GetOptionChoice(lines_style_item));
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    line linestyle %d",lines[lineno].lines);
        ListOfOldStates << QString(dummy);
        //}

//qDebug() << "linew=" << lines[lineno].linew << " Spin=" << GetSpinChoice(lines_width_item);

        if (lines[lineno].linew != GetSpinChoice(lines_width_item))
        {
            changes=true;
        }
        sprintf(dummy,"    line linewidth %f",GetSpinChoice(lines_width_item));
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    line linewidth %f",lines[lineno].linew);
        ListOfOldStates << QString(dummy);
        //}
        if (lines[lineno].arrow_end != GetChoice(lines_arrow_item))
        {
            changes=true;
        }
        sprintf(dummy,"    line arrow %d",GetChoice(lines_arrow_item));
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    line arrow %d",lines[lineno].arrow_end);
        ListOfOldStates << QString(dummy);
        //}
        if (lines[lineno].arrow.type != GetChoice(lines_atype_item))
        {
            changes=true;
        }
        sprintf(dummy,"    line arrow type %d",GetChoice(lines_atype_item));
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    line arrow type %d",lines[lineno].arrow.type);
        ListOfOldStates << QString(dummy);
        //}
        if (lines[lineno].arrow.length != GetSpinChoice(lines_asize_item))
        {
            changes=true;
        }
        sprintf(dummy,"    line arrow length %f",GetSpinChoice(lines_asize_item));
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    line arrow length %f",lines[lineno].arrow.length);
        ListOfOldStates << QString(dummy);
        //}
        if (lines[lineno].arrow.dL_ff != GetSpinChoice(lines_a_dL_ff_item) || lines[lineno].arrow.lL_ff != GetSpinChoice(lines_a_lL_ff_item))
        {
            changes=true;
        }
        sprintf(dummy,"    line arrow layout %f, %f",GetSpinChoice(lines_a_dL_ff_item),GetSpinChoice(lines_a_lL_ff_item));
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    line arrow layout %f, %f",lines[lineno].arrow.dL_ff,lines[lineno].arrow.lL_ff);
        ListOfOldStates << QString(dummy);
        //}
        if (lines[lineno].alpha != lines_color_item->alpha())
        {
            changes=true;
        }
        sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: LINE %d ALPHA %d\n",lineno,lines_color_item->alpha());
        ListOfChanges << QString(dummy);
        sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: LINE %d ALPHA %d\n",lineno,lines[lineno].alpha);
        ListOfOldStates << QString(dummy);
        if (lines[lineno].gno!=selGno->currentValue())
        {
            changes=true;
        }
        sprintf(dummy,"    line g%1d",selGno->currentValue());
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    line g%1d",lines[lineno].gno);
        ListOfOldStates << QString(dummy);
        if (lines[lineno].x1!=k[0] || lines[lineno].y1!=k[1] || lines[lineno].x2!=k[2] || lines[lineno].y2!=k[3])
        {
            changes=true;
        }
        sprintf(dummy,"    line %f, %f, %f, %f",k[0],k[1],k[2],k[3]);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    line %f, %f, %f, %f",lines[lineno].x1,lines[lineno].y1,lines[lineno].x2,lines[lineno].y2);
        ListOfOldStates << QString(dummy);
        //}
        //if (ListOfChanges.size()>1)
        //{
        ListOfChanges << QString("    line on");
        ListOfChanges << QString("line def");
        ListOfOldStates << QString("    line on");
        ListOfOldStates << QString("line def");
        //}
        lines[lineno].color = GetOptionChoice(lines_color_item);
        lines[lineno].alpha = lines_color_item->alpha();
        lines[lineno].loctype = lines_loc_item->currentValue();
        lines[lineno].lines = GetOptionChoice(lines_style_item);
        lines[lineno].linew = GetSpinChoice(lines_width_item);
        lines[lineno].arrow_end = GetChoice(lines_arrow_item);
        lines[lineno].arrow.type = GetChoice(lines_atype_item);
        lines[lineno].arrow.length = GetSpinChoice(lines_asize_item);
        lines[lineno].arrow.dL_ff = GetSpinChoice(lines_a_dL_ff_item);
        lines[lineno].arrow.lL_ff = GetSpinChoice(lines_a_lL_ff_item);
        lines[lineno].x1=k[0];
        lines[lineno].y1=k[1];
        lines[lineno].x2=k[2];
        lines[lineno].y2=k[3];
        lines[lineno].gno = selGno->currentValue();
        ObjectDataModified(lineno,OBJECT_LINE);
        set_dirtystate();
        if (GlobalInhibitor==false) mainWin->mainArea->completeRedraw();
        //force_redraw();

        if (changes==false)
        {
            ListOfChanges.clear();
            ListOfOldStates.clear();
        }
    }
    set_action(action_flag);
}

void frmLine_Props::doEdit(void)
{
    if (editWindow==false) return;
VPoint vp;
WPoint wp;
double x,y;
int anchor_x,anchor_y,x2,y2;
double k[4];
    doAccept();
    switch_current_graph(lines[obj_id].gno);
    if( lines[obj_id].loctype == COORD_VIEW )
    {
    view2world(lines[obj_id].x1,lines[obj_id].y1,k+0,k+1);
    view2world(lines[obj_id].x2,lines[obj_id].y2,k+2,k+3);
    }
    else//data already in world-coordinates
    {
    k[0]=lines[obj_id].x1;
    k[1]=lines[obj_id].y1;
    k[2]=lines[obj_id].x2;
    k[3]=lines[obj_id].y2;
    }

    if (cmbStartEnd->currentIndex()==1)// X2/Y2
    {
    x=k[0];
    y=k[1];
    }
    else//X1/Y1
    {
    x=k[2];
    y=k[3];
    swap_line_coords=1;
    }
    wp.x=x;
    wp.y=y;
    vp = Wpoint2Vpoint(wp);
    xlibVPoint2dev(vp, &anchor_x, &anchor_y);
    anchor_point(anchor_x, anchor_y, vp);
    next_line_id=obj_id;
    rg[MAXREGION].linkto=lines[obj_id].gno;
    rg[MAXREGION].active=TRUE;
    if (cmbStartEnd->currentIndex()==1)// X2/Y2
    {
    x=k[2];
    y=k[3];
    }
    else//X1/Y1
    {
    x=k[0];
    y=k[1];
    }
    wp.x=x;
    wp.y=y;
    vp = Wpoint2Vpoint(wp);
    xlibVPoint2dev(vp, &x2, &y2);
    select_line(anchor_x, anchor_y, x2, y2, 0);
    //qDebug() << "anchor_x=" << anchor_x << "anchor_y=" << anchor_y << "x=" << x << "y=" << y;
    wp.x=x;
    wp.y=y;
    vp = Wpoint2Vpoint(wp);
    setpointer(vp);
    simple_draw_setting=SIMPLE_DRAW_LINE;
    set_action(MAKE_LINE_2ND);
}

void frmLine_Props::doClose(void)
{
    parentWidget()->hide();
    set_action(action_flag);
}

void frmLine_Props::update0(void)
{
    static int i,nr;//,errpos;
    if (!immediateUpdate) return;
    bool sav_im_up;
    sav_im_up=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    GlobalInhibitor=true;
    ListOfChanges.clear();
    ListOfOldStates.clear();
    doAccept();//do this without actually doing something
    nr=nr_of_true_changes(ListOfChanges);
    //errpos=0;
    if (nr>0)
    {
        for (i=0;i<ListOfChanges.size();i++)
        {
            strcpy(command,ListOfChanges.at(i).toLocal8Bit().constData());
            //errpos = scanner(command);
            (void)scanner(command);
        }
        set_dirtystate();
        mainWin->mainArea->completeRedraw();
    }
    ListOfChanges.clear();
    ListOfOldStates.clear();
    GlobalInhibitor=false;
    immediateUpdate=sav_im_up;
    updateRunning=false;
}

void frmLine_Props::update1(int v)
{
    (void)v;
    update0();
}

void frmLine_Props::update2(QString v)
{
    (void)v;
    update0();
}

void frmLine_Props::update3(bool v)
{
    (void)v;
    update0();
}

void frmLine_Props::update4(double v)
{
    (void)v;
    update0();
}

frmLineProps::frmLineProps(QWidget * parent,bool edit):QDialog(parent)
{
//setFont(*stdFont);
    if (edit)
    setWindowTitle(tr("Edit Line"));
    else
    setWindowTitle(tr("Lines"));
    setWindowIcon(QIcon(*GraceIcon));

    QVBoxLayout * layout=new QVBoxLayout;
    //layout->setMargin(0);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    flp=new frmLine_Props(this,edit);
    layout->addWidget(flp);
    setLayout(layout);
    if (edit)
    resize(LastSize_EditLineProps);
    else
    resize(LastSize_LineProps);
}

frmLineProps::~frmLineProps()
{
    if (flp->editWindow)
    LastSize_EditLineProps=this->size();
    else
    LastSize_LineProps=this->size();
}

void frmLineProps::init(int id)
{
    flp->init(id);
}

void frmLineProps::doAccept(void)
{
    flp->doAccept();
}

void frmLineProps::doClose(void)
{
    //flp->doClose();
    hide();
}

frmEllipse_Props::frmEllipse_Props(QWidget * parent,bool edit,bool ellip):QWidget(parent)
{
    obj_id=-1;//invalid id!! call init!!
    //setFont(*stdFont);
    //setWindowIcon(QIcon(*GraceIcon));
    editWindow=edit;
    ellipse=ellip;
    /*if (edit)
{
    if (ellipse)
    setWindowTitle(tr("Edit ellipse"));
    else
    setWindowTitle(tr("Edit box"));
}
else
{
    if (ellipse)
    setWindowTitle(tr("Ellipses"));
    else
    setWindowTitle(tr("Boxes"));
}*/
    ellip_color_item=new ColorSelector(this);
    ellip_linew_item=new LineWidthSelector(this);
    ellip_lines_item=new LineStyleSelector(this);
    ellip_fillpat_item=new FillPatternSelector(this);
    ellip_fillcol_item=new ColorSelector(this);
    ellip_loc_item=new PositionSelector(this);
    ellip_rot_item=new stdSlider(this,tr("Rotation"),0,180);
    QString entries[2];
    entries[0]=tr("Color pattern");
    entries[1]=tr("Image");
    selFill=new StdSelector(this,tr("Fill with:"),2,entries);
    selGno=new StdSelector(this,tr("Link to graph:"),2,entries);
    selGno->simple_graph_selector=true;
    imgSelect=new ImageSelector(this);
    imgSelect->setVisible(false);
    buttonGroup=new stdButtonGroup(this,false);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));

    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(ellip_color_item);
    layout->addWidget(ellip_linew_item);
    layout->addWidget(ellip_lines_item);
    layout->addWidget(selFill);
    layout->addWidget(imgSelect);
    layout->addWidget(ellip_fillpat_item);
    layout->addWidget(ellip_fillcol_item);
    layout->addWidget(selGno);
    layout->addWidget(ellip_loc_item);
    layout->addWidget(ellip_rot_item);
    if (edit)
    {//I will not use these for immediate updates!
        if (ellipse)//Ellipse
        {
            ledCoords[0]=new stdLineEdit(this,tr("Xcentre="));
            layout->addWidget(ledCoords[0]);
            ledCoords[1]=new stdLineEdit(this,tr("Ycentre="));
            layout->addWidget(ledCoords[1]);
            ledCoords[2]=new stdLineEdit(this,tr("Width="));
            layout->addWidget(ledCoords[2]);
            ledCoords[3]=new stdLineEdit(this,tr("Height="));
            layout->addWidget(ledCoords[3]);
        }
        else//box
        {
            ledCoords[0]=new stdLineEdit(this,tr("Xmin="));
            layout->addWidget(ledCoords[0]);
            ledCoords[1]=new stdLineEdit(this,tr("Ymin="));
            layout->addWidget(ledCoords[1]);
            ledCoords[2]=new stdLineEdit(this,tr("Xmax="));
            layout->addWidget(ledCoords[2]);
            ledCoords[3]=new stdLineEdit(this,tr("Ymax="));
            layout->addWidget(ledCoords[3]);
        }
        ellip_rot_item->setVisible(true);
    }
    else
        ellip_rot_item->setVisible(false);

    layout->addWidget(buttonGroup);
    setLayout(layout);

    ellip_color_item->setCurrentIndex(1);
    ellip_lines_item->setCurrentIndex(1);
    ellip_fillcol_item->setCurrentIndex(1);
    ellip_loc_item->setCurrentIndex(1);
    ellip_linew_item->setValue(1.0);
    selGno->setValuesToGraphs(0,0);
    connect(ellip_color_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(ellip_color_item,SIGNAL(alphaChanged(int)),SLOT(update1(int)));
    connect(ellip_linew_item,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(ellip_lines_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(ellip_fillpat_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(ellip_fillcol_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(ellip_fillcol_item,SIGNAL(alphaChanged(int)),SLOT(update1(int)));
    connect(ellip_rot_item,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(selGno,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(selFill,SIGNAL(currentIndexChanged(int)),SLOT(fillingChanged(int)));
    //do not use the following for immediate updates!!
    //connect(ellip_loc_item->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(ellip_loc_item->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),SLOT(viewCoordsChanged(int)));
    connect(imgSelect,SIGNAL(someChanges()),SLOT(update0()));
}

void frmEllipse_Props::init(int id)
{
    if ( (id>=number_of_boxes() && ellipse==false) || (id>=number_of_ellipses() && ellipse==true) || (id<0 && editWindow==true)) return;
    bool sav_imm_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    char buf[256];
    obj_id=id;
    selGno->setValuesToGraphs(0,0);
    disconnect(ellip_loc_item->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(viewCoordsChanged(int)));
    if (editWindow==false)
    {
        selGno->setVisible(false);
        if (ellipse==false)
        {
            imgSelect->initToImage(bg_box_fill_image);
            SetOptionChoice(ellip_color_item, box_color);
            ellip_color_item->setAlpha(box_alpha);
            SetOptionChoice(ellip_lines_item, box_lines);
            SetSpinChoice(ellip_linew_item, box_linew);
            SetOptionChoice(ellip_fillpat_item, box_fillpat);
            SetOptionChoice(ellip_fillcol_item, box_fillcolor);
            ellip_fillcol_item->setAlpha(box_fillcolor);
            ellip_rot_item->setValue(0);
            ellip_loc_item->setCurrentIndex(box_loctype == COORD_VIEW ? 1 : 0);
            selFill->setCurrentIndex(box_filltype);
        }
        else
        {
            imgSelect->initToImage(bg_ellipse_fill_image);
            SetOptionChoice(ellip_color_item, ellipse_color);
            ellip_color_item->setAlpha(ellipse_alpha);
            SetOptionChoice(ellip_lines_item, ellipse_lines);
            SetSpinChoice(ellip_linew_item, ellipse_linew);
            SetOptionChoice(ellip_fillpat_item, ellipse_fillpat);
            SetOptionChoice(ellip_fillcol_item, ellipse_fillcolor);
            ellip_fillcol_item->setAlpha(ellipse_fillalpha);
            ellip_rot_item->setValue(0);
            ellip_loc_item->setCurrentIndex(ellipse_loctype == COORD_VIEW ? 1 : 0);
            selFill->setCurrentIndex(ellipse_filltype);
        }
    }
    else
    {
        int ellipno = id;
        selGno->setValuesToGraphs(0,0);
        selGno->setVisible(true);
        if (ellipse==true)
        {
            imgSelect->initToImage(ellip[ellipno].fillimage);
            SetOptionChoice(ellip_color_item, ellip[ellipno].color);
            ellip_color_item->setAlpha(ellip[ellipno].alpha);
            SetOptionChoice(ellip_lines_item, ellip[ellipno].lines);
            SetSpinChoice(ellip_linew_item, ellip[ellipno].linew);
            SetOptionChoice(ellip_fillpat_item, ellip[ellipno].fillpattern);
            SetOptionChoice(ellip_fillcol_item, ellip[ellipno].fillcolor);
            ellip_fillcol_item->setAlpha(ellip[ellipno].fillalpha);
            ellip_rot_item->setValue(ellip[ellipno].rot);
            ellip_loc_item->setCurrentIndex(ellip[ellipno].loctype == COORD_VIEW ? 1 : 0);
            sprintf(buf, "%.12f", 0.5*(ellip[ellipno].x1+ellip[ellipno].x2));
            SetDecimalSeparatorToUserValue(buf);
            xv_setstr(ledCoords[0], buf);
            sprintf(buf, "%.12f", 0.5*(ellip[ellipno].y1+ellip[ellipno].y2));
            SetDecimalSeparatorToUserValue(buf);
            xv_setstr(ledCoords[1], buf);
            sprintf(buf, "%.12f", fabs(ellip[ellipno].x1-ellip[ellipno].x2) );
            SetDecimalSeparatorToUserValue(buf);
            xv_setstr(ledCoords[2], buf);
            sprintf(buf, "%.12f", fabs(ellip[ellipno].y1-ellip[ellipno].y2) );
            SetDecimalSeparatorToUserValue(buf);
            xv_setstr(ledCoords[3], buf);
            selFill->setCurrentIndex(ellip[ellipno].filltype);
            selGno->setCurrentValue(ellip[ellipno].gno);
        }
        else//box
        {
            imgSelect->initToImage(boxes[ellipno].fillimage);
            SetOptionChoice(ellip_color_item, boxes[ellipno].color);
            ellip_color_item->setAlpha(boxes[ellipno].alpha);
            SetOptionChoice(ellip_lines_item, boxes[ellipno].lines);
            SetSpinChoice(ellip_linew_item, boxes[ellipno].linew);
            SetOptionChoice(ellip_fillpat_item, boxes[ellipno].fillpattern);
            SetOptionChoice(ellip_fillcol_item, boxes[ellipno].fillcolor);
            ellip_fillcol_item->setAlpha(boxes[ellipno].fillalpha);
            ellip_rot_item->setValue(boxes[ellipno].rot);
            ellip_loc_item->setCurrentIndex(boxes[ellipno].loctype == COORD_VIEW ? 1 : 0);
            sprintf(buf, "%.12f", boxes[ellipno].x1);
            SetDecimalSeparatorToUserValue(buf);
            xv_setstr(ledCoords[0], buf);
            sprintf(buf, "%.12f", boxes[ellipno].y1);
            SetDecimalSeparatorToUserValue(buf);
            xv_setstr(ledCoords[1], buf);
            sprintf(buf, "%.12f", boxes[ellipno].x2);
            SetDecimalSeparatorToUserValue(buf);
            xv_setstr(ledCoords[2], buf);
            sprintf(buf, "%.12f", boxes[ellipno].y2);
            SetDecimalSeparatorToUserValue(buf);
            xv_setstr(ledCoords[3], buf);
            selFill->setCurrentIndex(boxes[ellipno].filltype);
            selGno->setCurrentValue(boxes[ellipno].gno);
        }
    }
    connect(ellip_loc_item->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(viewCoordsChanged(int)));
    immediateUpdate=sav_imm_upd;
    updateRunning=false;
}

void frmEllipse_Props::reDisplayContents(void)
{
QLocale newLocale=(DecimalPointToUse=='.')?(*dot_locale):(*comma_locale);
    ellip_linew_item->setLocale(newLocale);
    init(obj_id);
    layout->update();
}

void frmEllipse_Props::viewCoordsChanged(int i)
{
    (void)i;
    if (editWindow==false) return;//there are no coordinates visible --> nothing to do
    if ( (obj_id>=number_of_boxes() && ellipse==false) || (obj_id>=number_of_ellipses() && ellipse==true) || (obj_id<0 && editWindow==true)) return;
    int boxno = obj_id;
    int ellipno = obj_id;
    int old_cur_graph=get_cg();
    double x1, x2, y1, y2;
    bool old_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    if (ellipse==false)//it is a box
    {
        if( ellip_loc_item->currentValue() == boxes[boxno].loctype )
        {
            return;
        }

        xv_evalexpr(ledCoords[0], &x1);
        xv_evalexpr(ledCoords[2], &x2);
        xv_evalexpr(ledCoords[1], &y1);
        xv_evalexpr(ledCoords[3], &y2);

        select_graph(boxes[boxno].gno);
        if( boxes[boxno].loctype == COORD_VIEW )
        {
            //boxes[boxno].gno = get_cg();
            boxes[boxno].loctype = COORD_WORLD;
            //view2world( atof(xv_getstr(ledCoords[0])), atof(xv_getstr(ledCoords[1])),&boxes[boxno].x1,&boxes[boxno].y1 );
            //view2world( atof(xv_getstr(ledCoords[2])), atof(xv_getstr(ledCoords[3])),&boxes[boxno].x2,&boxes[boxno].y2 );
            view2world( x1,y1,&boxes[boxno].x1,&boxes[boxno].y1 );
            view2world( x2,y2,&boxes[boxno].x2,&boxes[boxno].y2 );
        }
        else
        {
            boxes[boxno].loctype = COORD_VIEW;
            //world2view( atof(xv_getstr(ledCoords[0])), atof(xv_getstr(ledCoords[1])),&boxes[boxno].x1,&boxes[boxno].y1 );
            //world2view( atof(xv_getstr(ledCoords[2])), atof(xv_getstr(ledCoords[3])),&boxes[boxno].x2,&boxes[boxno].y2 );
            world2view( x1,y1,&boxes[boxno].x1,&boxes[boxno].y1 );
            world2view( x2,y2,&boxes[boxno].x2,&boxes[boxno].y2 );
        }
    }//end of it is a box
    else
    {//it is an ellipse
        if(ellip_loc_item->currentValue() == ellip[ellipno].loctype)
        {
            return;
        }
        xv_evalexpr(ledCoords[0], &x1);
        xv_evalexpr(ledCoords[2], &x2);
        xv_evalexpr(ledCoords[1], &y1);
        xv_evalexpr(ledCoords[3], &y2);

        select_graph(ellip[ellipno].gno);
        if( ellip[ellipno].loctype == COORD_VIEW )
        {
            //ellip[ellipno].gno = get_cg();
            ellip[ellipno].loctype = COORD_WORLD;
            view2world( x1-x2/2., y1-y2/2., &ellip[ellipno].x1,&ellip[ellipno].y1 );
            view2world( x1+x2/2., y1+y2/2., &ellip[ellipno].x2,&ellip[ellipno].y2 );
        }
        else
        {
            ellip[ellipno].loctype = COORD_VIEW;
            world2view( x1-x2/2., y1-y2/2., &ellip[ellipno].x1,&ellip[ellipno].y1 );
            world2view( x1+x2/2., y1+y2/2., &ellip[ellipno].x2,&ellip[ellipno].y2 );
        }
    }
    select_graph(old_cur_graph);
    init(obj_id);
    immediateUpdate=old_upd;
    updateRunning=false;
}

void frmEllipse_Props::fillingChanged(int i)
{
    (void)i;
    if (selFill->currentIndex()==0)//color
    {
        imgSelect->setVisible(false);
        ellip_fillpat_item->setVisible(true);
        ellip_fillcol_item->setVisible(true);
    }
    else//image
    {
        imgSelect->setVisible(true);
        ellip_fillpat_item->setVisible(false);
        ellip_fillcol_item->setVisible(false);
    }
}

void frmEllipse_Props::doAccept(void)
{
if ( (obj_id>=number_of_boxes() && ellipse==false) || (obj_id>=number_of_ellipses() && ellipse==true) || (obj_id<0 && editWindow==true)) return;
    int ellipno = obj_id;
    double a, b, c, d;
    bool changes=false;
    QString tmp_info_string;
    imageinfo tmp_image_info;
    ListOfChanges.clear();
    ListOfOldStates.clear();
    if (editWindow==false)//edit preferences
    {
        if (ellipse==true)
        {
            ellipse_color = GetOptionChoice(ellip_color_item);
            ellipse_alpha = ellip_color_item->alpha();
            ellipse_loctype = ellip_loc_item->currentValue();
            ellipse_lines = GetOptionChoice(ellip_lines_item);
            ellipse_linew = GetSpinChoice(ellip_linew_item);
            ellipse_fillcolor = GetOptionChoice(ellip_fillcol_item);
            ellipse_fillalpha = ellip_fillcol_item->alpha();
            ellipse_fillpat = GetOptionChoice(ellip_fillpat_item);
            ellipse_filltype = selFill->currentIndex();
            imgSelect->readImageSettings(bg_ellipse_fill_image);
        }
        else
        {
            box_color = GetOptionChoice(ellip_color_item);
            box_alpha = ellip_color_item->alpha();
            box_loctype = ellip_loc_item->currentValue();
            box_lines = GetOptionChoice(ellip_lines_item);
            box_linew = GetSpinChoice(ellip_linew_item);
            box_fillcolor = GetOptionChoice(ellip_fillcol_item);
            box_fillalpha = ellip_fillcol_item->alpha();
            box_fillpat = GetOptionChoice(ellip_fillpat_item);
            box_filltype = selFill->currentIndex();
            imgSelect->readImageSettings(bg_box_fill_image);
        }
    }
    else//edit Object
    {

        if (xv_evalexpr(ledCoords[0], &a ) != RETURN_SUCCESS ||
                xv_evalexpr(ledCoords[2], &b ) != RETURN_SUCCESS ||
                xv_evalexpr(ledCoords[1], &c ) != RETURN_SUCCESS ||
                xv_evalexpr(ledCoords[3], &d ) != RETURN_SUCCESS ) {
            return;
        }
        if (ellipse==true)//it's an ellipse
        {
            SaveObjectData(ellipno,OBJECT_ELLIPSE);
            sprintf(dummy,"with ellipse %d",ellipno);
            ListOfChanges << QString(dummy);
            ListOfOldStates << QString(dummy);
            if (ellip[ellipno].color != GetOptionChoice(ellip_color_item))
            {
                changes=true;
            }
            sprintf(dummy,"    ellipse color %d",GetOptionChoice(ellip_color_item));
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    ellipse color %d",ellip[ellipno].color);
            ListOfOldStates << QString(dummy);
            //}
            if (ellip[ellipno].loctype != ellip_loc_item->currentValue())
            {
                changes=true;
            }
            sprintf(dummy,"    ellipse loctype %s",!ellip_loc_item->currentValue()?"view":"world");
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    ellipse loctype %s",!ellip[ellipno].loctype?"view":"world");
            ListOfOldStates << QString(dummy);
            //}
            if (ellip[ellipno].lines != GetOptionChoice(ellip_lines_item))
            {
                changes=true;
            }
            sprintf(dummy,"    ellipse linestyle %d",GetOptionChoice(ellip_lines_item));
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    ellipse linestyle %d",ellip[ellipno].lines);
            ListOfOldStates << QString(dummy);
            //}
            if (ellip[ellipno].linew != GetSpinChoice(ellip_linew_item))
            {
                changes=true;
            }
            sprintf(dummy,"    ellipse linewidth %f",GetSpinChoice(ellip_linew_item));
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    ellipse linewidth %f",ellip[ellipno].linew);
            ListOfOldStates << QString(dummy);
            //}
            if (ellip[ellipno].fillcolor != GetOptionChoice(ellip_fillcol_item))
            {
                changes=true;
            }
            sprintf(dummy,"   ellipse fill color %d",GetOptionChoice(ellip_fillcol_item));
            ListOfChanges << QString(dummy);
            sprintf(dummy,"   ellipse fill color %d",ellip[ellipno].fillcolor);
            ListOfOldStates << QString(dummy);
            //}
            if (ellip[ellipno].fillpattern != GetOptionChoice(ellip_fillpat_item))
            {
                changes=true;
            }
            sprintf(dummy,"    ellipse fill pattern %d",GetOptionChoice(ellip_fillpat_item));
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    ellipse fill pattern %d",ellip[ellipno].fillpattern);
            ListOfOldStates << QString(dummy);
            //}
            if (ellip[ellipno].alpha != ellip_color_item->alpha() || ellip[ellipno].fillalpha != ellip_fillcol_item->alpha())
            {
                changes=true;
            }
            sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: ELLIPSE %d ALPHA %d %d\n",ellipno,ellip_color_item->alpha(),ellip_fillcol_item->alpha());
            ListOfChanges << QString(dummy);
            sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: ELLIPSE %d ALPHA %d %d\n",ellipno,ellip[ellipno].alpha,ellip[ellipno].fillalpha);
            ListOfOldStates << QString(dummy);

            imgSelect->readImageSettings(tmp_image_info);
            if (tmp_image_info.imgpath!=ellip[ellipno].fillimage.imgpath || tmp_image_info.imgtransform!=ellip[ellipno].fillimage.imgtransform || tmp_image_info.relpath!=ellip[ellipno].fillimage.relpath)
            {
                changes=true;
            }
            sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: ELLIPSE %d IMAGE %d ",ellipno,selFill->currentIndex());
            createImageInfoText(tmp_image_info,tmp_info_string,QString());
            ListOfChanges << QString(dummy)+tmp_info_string+QString("\n");
            sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: ELLIPSE %d IMAGE %d ",ellipno,ellip[ellipno].filltype);
            createImageInfoText(ellip[ellipno].fillimage,tmp_info_string,QString());
            ListOfOldStates << QString(dummy)+tmp_info_string+QString("\n");

            if (ellip[ellipno].x1 != a - b/2. || ellip[ellipno].x2 != a + b/2. || ellip[ellipno].y1 != c - d/2. || ellip[ellipno].y2 != c + d/2.)
            {
                changes=true;
            }
            sprintf(dummy,"    ellipse %f, %f, %f, %f",a - b/2.,c - d/2.,a + b/2.,c + d/2.);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    ellipse %f, %f, %f, %f",ellip[ellipno].x1,ellip[ellipno].y1,ellip[ellipno].x2,ellip[ellipno].y2);
            ListOfOldStates << QString(dummy);

            if (ellip[ellipno].gno != selGno->currentValue())
            {
                changes=true;
            }
            sprintf(dummy,"    ellipse g%1d",selGno->currentIndex());
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    ellipse g%1d",ellip[ellipno].gno);
            ListOfOldStates << QString(dummy);
            //}
            //if (ListOfChanges.size()>1)
            //{
            ListOfChanges << QString("    ellipse on");
            ListOfChanges << QString("ellipse def");
            ListOfOldStates << QString("    ellipse on");
            ListOfOldStates << QString("ellipse def");
            //}
            if (changes==false)
            {
                ListOfOldStates.clear();
                ListOfChanges.clear();
            }
            ellip[ellipno].color = GetOptionChoice(ellip_color_item);
            ellip[ellipno].alpha = ellip_color_item->alpha();
            ellip[ellipno].loctype = ellip_loc_item->currentValue();
            ellip[ellipno].lines = GetOptionChoice(ellip_lines_item);
            ellip[ellipno].linew = GetSpinChoice(ellip_linew_item);
            ellip[ellipno].fillcolor = GetOptionChoice(ellip_fillcol_item);
            ellip[ellipno].fillalpha = ellip_fillcol_item->alpha();
            ellip[ellipno].fillpattern = GetOptionChoice(ellip_fillpat_item);
            ellip[ellipno].filltype = selFill->currentIndex();
            ellip[ellipno].rot = ellip_rot_item->value();
            ellip[ellipno].x1 = a - b/2.;
            ellip[ellipno].x2 = a + b/2.;
            ellip[ellipno].y1 = c - d/2.;
            ellip[ellipno].y2 = c + d/2.;
            ellip[ellipno].gno = selGno->currentValue();
            memcpy(&ellip[ellipno].fillimage,&tmp_image_info,sizeof(imageinfo));
            ObjectDataModified(ellipno,OBJECT_ELLIPSE);
        }
        else//it's a box
        {
            SaveObjectData(ellipno,OBJECT_BOX);
            sprintf(dummy,"with box %d",ellipno);
            ListOfChanges << QString(dummy);
            ListOfOldStates << QString(dummy);
            if (boxes[ellipno].color != GetOptionChoice(ellip_color_item))
            {
                changes=true;
            }
            sprintf(dummy,"    box color %d",GetOptionChoice(ellip_color_item));
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    box color %d",boxes[ellipno].color);
            ListOfOldStates << QString(dummy);
            //}
            if (boxes[ellipno].loctype != ellip_loc_item->currentValue())
            {
                changes=true;
            }
            sprintf(dummy,"    box loctype %s",!ellip_loc_item->currentValue()?"view":"world");
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    box loctype %s",!boxes[ellipno].loctype?"view":"world");
            ListOfOldStates << QString(dummy);
            //}
            if (boxes[ellipno].lines != GetOptionChoice(ellip_lines_item))
            {
                changes=true;
            }
            sprintf(dummy,"    box linestyle %d",GetOptionChoice(ellip_lines_item));
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    box linestyle %d",boxes[ellipno].lines);
            ListOfOldStates << QString(dummy);
            //}
            if (boxes[ellipno].linew != GetSpinChoice(ellip_linew_item))
            {
                changes=true;
            }
            sprintf(dummy,"    box linewidth %f",GetSpinChoice(ellip_linew_item));
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    box linewidth %f",boxes[ellipno].linew);
            ListOfOldStates << QString(dummy);
            //}
            if (boxes[ellipno].fillcolor != GetOptionChoice(ellip_fillcol_item))
            {
                changes=true;
            }
            sprintf(dummy,"   box fill color %d",GetOptionChoice(ellip_fillcol_item));
            ListOfChanges << QString(dummy);
            sprintf(dummy,"   box fill color %d",boxes[ellipno].fillcolor);
            ListOfOldStates << QString(dummy);
            //}
            if (boxes[ellipno].fillpattern != GetOptionChoice(ellip_fillpat_item))
            {
                changes=true;
            }
            sprintf(dummy,"    box fill pattern %d",GetOptionChoice(ellip_fillpat_item));
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    box fill pattern %d",boxes[ellipno].fillpattern);
            ListOfOldStates << QString(dummy);
            //}
            if (boxes[ellipno].alpha != ellip_color_item->alpha() || boxes[ellipno].fillalpha != ellip_fillcol_item->alpha())
            {
                changes=true;
            }
            sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: BOX %d ALPHA %d %d\n",ellipno,ellip_color_item->alpha(),ellip_fillcol_item->alpha());
            ListOfChanges << QString(dummy);
            sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: BOX %d ALPHA %d %d\n",ellipno,boxes[ellipno].alpha,boxes[ellipno].fillalpha);
            ListOfOldStates << QString(dummy);

            imgSelect->readImageSettings(tmp_image_info);
            if (tmp_image_info.imgpath!=boxes[ellipno].fillimage.imgpath || tmp_image_info.imgtransform!=boxes[ellipno].fillimage.imgtransform || tmp_image_info.relpath!=boxes[ellipno].fillimage.relpath)
            {
                changes=true;
            }
            sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: BOX %d IMAGE %d ",ellipno,selFill->currentIndex());
            createImageInfoText(tmp_image_info,tmp_info_string,QString());
            ListOfChanges << QString(dummy)+tmp_info_string+QString("\n");
            sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: BOX %d IMAGE %d ",ellipno,boxes[ellipno].filltype);
            createImageInfoText(boxes[ellipno].fillimage,tmp_info_string,QString());
            ListOfOldStates << QString(dummy)+tmp_info_string+QString("\n");

            if (boxes[ellipno].x1 != a || boxes[ellipno].x2 != b || boxes[ellipno].y1 != c || boxes[ellipno].y2 != d)
            {
                changes=true;
            }
            sprintf(dummy,"    box %f, %f, %f, %f",a,c,b,d);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    box %f, %f, %f, %f",boxes[ellipno].x1,boxes[ellipno].y1,boxes[ellipno].x2,boxes[ellipno].y2);
            ListOfOldStates << QString(dummy);
            if (boxes[ellipno].gno != selGno->currentValue())
            {
                changes=true;
            }
            sprintf(dummy,"    box g%1d",selGno->currentIndex());
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    box g%1d",boxes[ellipno].gno);
            ListOfOldStates << QString(dummy);
            //}
            //if (ListOfChanges.size()>1)
            //{
            ListOfChanges << QString("    box on");
            ListOfChanges << QString("box def");
            ListOfOldStates << QString("    box on");
            ListOfOldStates << QString("box def");
            //}
            if (changes==false)
            {
                ListOfChanges.clear();
                ListOfOldStates.clear();
            }
            boxes[ellipno].color = GetOptionChoice(ellip_color_item);
            boxes[ellipno].alpha = ellip_color_item->alpha();
            boxes[ellipno].loctype = ellip_loc_item->currentValue();
            boxes[ellipno].lines = GetOptionChoice(ellip_lines_item);
            boxes[ellipno].linew = GetSpinChoice(ellip_linew_item);
            boxes[ellipno].fillcolor = GetOptionChoice(ellip_fillcol_item);
            boxes[ellipno].fillalpha = ellip_fillcol_item->alpha();
            boxes[ellipno].fillpattern = GetOptionChoice(ellip_fillpat_item);
            boxes[ellipno].filltype = selFill->currentIndex();
            boxes[ellipno].rot = ellip_rot_item->value();
            boxes[ellipno].x1 = a;
            boxes[ellipno].x2 = b;
            boxes[ellipno].y1 = c;
            boxes[ellipno].y2 = d;
            boxes[ellipno].gno = selGno->currentValue();
            memcpy(&boxes[ellipno].fillimage,&tmp_image_info,sizeof(imageinfo));
            ObjectDataModified(ellipno,OBJECT_BOX);
        }
        set_dirtystate();
        if (GlobalInhibitor==false)
        {
            mainWin->mainArea->completeRedraw();
            ListOfChanges.clear();
            ListOfOldStates.clear();
        }
    }
    set_action(action_flag);
}

void frmEllipse_Props::doClose(void)
{
    parentWidget()->hide();
    set_action(action_flag);
}

void frmEllipse_Props::update0(void)
{
    static int i,nr;//errpos;
    static int sav_settings_i[16];
    static double sav_settings_d[2];
    static imageinfo tmp_img_info1,tmp_img_info2;
    bool imm_upd_sav;
    if (!immediateUpdate) return;
    imm_upd_sav=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    GlobalInhibitor=true;
    ListOfChanges.clear();
    ListOfOldStates.clear();
    doAccept();//do this without actually doing something
    nr=nr_of_true_changes(ListOfChanges);
    //errpos=0;
    if (nr>0)
    {
        sav_settings_i[0]=ellipse_color;
        sav_settings_i[1]=ellipse_loctype;
        sav_settings_i[2]=ellipse_lines;
        sav_settings_d[0]=ellipse_linew;
        sav_settings_i[3]=ellipse_fillcolor;
        sav_settings_i[4]=ellipse_fillpat;
        sav_settings_i[5]=box_color;
        sav_settings_i[6]=box_loctype;
        sav_settings_i[7]=box_lines;
        sav_settings_d[1]=box_linew;
        sav_settings_i[8]=box_fillcolor;
        sav_settings_i[9]=box_fillpat;
        sav_settings_i[10]=ellipse_alpha;
        sav_settings_i[11]=ellipse_fillalpha;
        sav_settings_i[12]=box_alpha;
        sav_settings_i[13]=box_fillalpha;
        sav_settings_i[14]=box_filltype;
        sav_settings_i[15]=ellipse_filltype;
        memcpy(&tmp_img_info1,&bg_ellipse_fill_image,sizeof(imageinfo));
        memcpy(&tmp_img_info2,&bg_box_fill_image,sizeof(imageinfo));

        for (i=0;i<ListOfChanges.size();i++)
        {
            strcpy(command,ListOfChanges.at(i).toLocal8Bit().constData());
            //errpos = scanner(command);
            (void)scanner(command);
        }
        set_dirtystate();

        ellipse_color=sav_settings_i[0];
        ellipse_loctype=sav_settings_i[1];
        ellipse_lines=sav_settings_i[2];
        ellipse_linew=sav_settings_d[0];
        ellipse_fillcolor=sav_settings_i[3];
        ellipse_fillpat=sav_settings_i[4];
        box_color=sav_settings_i[5];
        box_loctype=sav_settings_i[6];
        box_lines=sav_settings_i[7];
        box_linew=sav_settings_d[1];
        box_fillcolor=sav_settings_i[8];
        box_fillpat=sav_settings_i[9];
        ellipse_alpha=sav_settings_i[10];
        ellipse_fillalpha=sav_settings_i[11];
        box_alpha=sav_settings_i[12];
        box_fillalpha=sav_settings_i[13];
        box_filltype=sav_settings_i[14];
        ellipse_filltype=sav_settings_i[15];
        memcpy(&bg_ellipse_fill_image,&tmp_img_info1,sizeof(imageinfo));
        memcpy(&bg_box_fill_image,&tmp_img_info2,sizeof(imageinfo));
    }
    mainWin->mainArea->completeRedraw();
    ListOfChanges.clear();
    ListOfOldStates.clear();
    GlobalInhibitor=false;
    immediateUpdate=imm_upd_sav;
    updateRunning=false;
}

void frmEllipse_Props::update1(int v)
{
    (void)v;
    update0();
}

void frmEllipse_Props::update2(QString v)
{
    (void)v;
    update0();
}

void frmEllipse_Props::update3(bool v)
{
    (void)v;
    update0();
}

void frmEllipse_Props::update4(double v)
{
    (void)v;
    update0();
}

frmEllipseProps::frmEllipseProps(QWidget * parent,bool edit,bool ellip):QDialog(parent)
{
//setFont(*stdFont);
    setWindowIcon(QIcon(*GraceIcon));

    QVBoxLayout * layout=new QVBoxLayout;
    //layout->setMargin(0);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    flp=new frmEllipse_Props(this,edit,ellip);
    layout->addWidget(flp);
    setLayout(layout);
    if (edit)
    {
        if (ellip)
        {
            setWindowTitle(tr("Edit ellipse"));
            resize(LastSize_EditEllipseProps);
        }
        else
        {
            setWindowTitle(tr("Edit box"));
            resize(LastSize_EditBoxProps);
        }
    }
    else
    {
        if (ellip)
        {
            setWindowTitle(tr("Ellipses"));
            resize(LastSize_EllipseProps);
        }
        else
        {
            setWindowTitle(tr("Boxes"));
            resize(LastSize_BoxProps);
        }
    }
}

frmEllipseProps::~frmEllipseProps()
{
    if (flp->editWindow)
    {
        if (flp->ellipse)
            LastSize_EditEllipseProps=this->size();
        else
            LastSize_EditBoxProps=this->size();
    }
    else
    {
        if (flp->ellipse)
            LastSize_EllipseProps=this->size();
        else
            LastSize_BoxProps=this->size();
    }
}

void frmEllipseProps::init(int id)
{
    flp->init(id);
}

void frmEllipseProps::doAccept(void)
{
    flp->doAccept();
}

void frmEllipseProps::doClose(void)
{
    //flp->doClose();
    hide();
}

frmPointExplorer::frmPointExplorer(QWidget * parent):QDialog(parent)
{
    int minwidth=72;
    track_setno=-1;
    gno=sno=loc_pos=-1;//invalid
//setFont(*stdFont);
    setWindowTitle(tr("QtGrace: Point explorer"));
    setWindowIcon(QIcon(*GraceIcon));

    lblInstructions=new QLabel(tr("Point explorer"),this);
    lblInstructions->setFrameStyle(QFrame::Box | QFrame::Raised);
    lblRestrToSet=new QLabel(tr("Restrict to set:"),this);
    list=new uniList(SETLIST,this);
    list->setBehavior(true,false,false);
    list->set_graph_number(get_cg(),false);
    list->update_number_of_entries();
    list->setGeometry(0,0,400,80);
    connect(list,SIGNAL(new_selection(int)),SLOT(selectionChanged(int)));
    ledPointLocation=new stdLineEdit(this,tr("Point location:"));
    ledPointLocation->lenText->setText(QString(""));
    ledPointData=new stdLineEdit(this,tr("Point data:"));
    ledPointData->lenText->setText(QString(""));

    grpButtons=new QGroupBox(this);
    layout1=new QHBoxLayout;
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout1->setSpacing(STD_SPACING);

    cmdGoTo=new QPushButton(tr("Goto point"),this);
    connect(cmdGoTo,SIGNAL(clicked()),SLOT(doGoTo()));
    cmdTrack=new QPushButton(tr("Track"),this);
    cmdTrack->setMinimumWidth(minwidth);
    connect(cmdTrack,SIGNAL(clicked()),SLOT(doTrack()));
    cmdMove=new QPushButton(tr("Move"),this);
    cmdMove->setMinimumWidth(minwidth);
    connect(cmdMove,SIGNAL(clicked()),SLOT(doMove()));
    cmdMoveX=new QPushButton(tr("Move X"),this);
    cmdMoveX->setMinimumWidth(minwidth);
    connect(cmdMoveX,SIGNAL(clicked()),SLOT(doMoveX()));
    cmdMoveY=new QPushButton(tr("Move Y"),this);
    cmdMoveY->setMinimumWidth(minwidth);
    connect(cmdMoveY,SIGNAL(clicked()),SLOT(doMoveY()));
    cmdPrepend=new QPushButton(tr("Prepend"),this);
    cmdPrepend->setMinimumWidth(minwidth);
    connect(cmdPrepend,SIGNAL(clicked()),SLOT(doPrepend()));
    cmdAppend=new QPushButton(tr("Append"),this);
    cmdAppend->setMinimumWidth(minwidth);
    connect(cmdAppend,SIGNAL(clicked()),SLOT(doAppend()));
    cmdInsert=new QPushButton(tr("Insert"),this);
    cmdInsert->setMinimumWidth(minwidth);
    connect(cmdInsert,SIGNAL(clicked()),SLOT(doInsert()));
    cmdDelete=new QPushButton(tr("Delete"),this);
    cmdDelete->setMinimumWidth(minwidth);
    connect(cmdDelete,SIGNAL(clicked()),SLOT(doDelete()));
    cmdClose=new QPushButton(tr("Close"),this);
    cmdClose->setMinimumWidth(minwidth);
    connect(cmdClose,SIGNAL(clicked()),SLOT(doClose()));
    layout1->addWidget(cmdTrack);
    layout1->addWidget(cmdMove);
    layout1->addWidget(cmdMoveX);
    layout1->addWidget(cmdMoveY);
    layout1->addWidget(cmdPrepend);
    layout1->addWidget(cmdAppend);
    layout1->addWidget(cmdInsert);
    layout1->addWidget(cmdDelete);
    layout1->addWidget(cmdClose);

    for (int i=0;i<9;i++)
        layout1->setStretch(i,1);
    grpButtons->setLayout(layout1);

    layout=new QGridLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(lblInstructions,0,0,1,2);
    layout->addWidget(lblRestrToSet,1,0,1,2);
    layout->addWidget(list,2,0,3,2);
    layout->addWidget(ledPointLocation,5,0,1,1);
    layout->addWidget(cmdGoTo,5,1,1,1);
    layout->addWidget(ledPointData,6,0,1,2);
    layout->addWidget(grpButtons,7,0,1,2);
    for (int i=0;i<2;i++)
        layout->setRowStretch(i,0);
    layout->setRowStretch(2,1);
    setLayout(layout);
    resize(LastSize_FormPointExplorer);
}

frmPointExplorer::~frmPointExplorer()
{
    LastSize_FormPointExplorer=this->size();
}

void frmPointExplorer::selectionChanged(int a)
{
    (void)a;
    int count;
    int * numbers=new int[2];
    list->get_selection(&count,&numbers);
    if (count >= 1) {
        track_setno = numbers[0];
    } else {
        ;
        //track_setno = -1;
    }
    delete[] numbers;
}

void frmPointExplorer::init(void)
{
    list->set_graph_number(get_cg(),false);
    list->update_number_of_entries();
    lblInstructions->setText(tr("Point explorer"));
}

void frmPointExplorer::doGoTo(void)
{
    int * selection=new int[3];
    int number;
    list->get_selection(&number,&selection);
    if (number!=1)
        return;
    else
        track_setno=selection[0];
    int ind;
    WPoint wp;
    VPoint vp;
    xv_evalexpri(ledPointLocation, &ind);
    if (get_point(cg, track_setno, ind, &wp) == RETURN_SUCCESS)
    {
        gno=cg;
        sno=track_setno;
        loc_pos=ind;
        update_point_locator(cg,track_setno,ind);
        vp = Wpoint2Vpoint(wp);
        setpointer(vp);
    }
    else
    {
        SetTextString(ledPointData, QString("").toLocal8Bit().data());
        errmsg(tr("Point index out of range").toLocal8Bit().constData());
    }
}

void frmPointExplorer::doTrack(void)
{
    if (action_flag==TRACKER)
    {
    set_action(DO_NOTHING);
    }
    else
    {
    int * selection=new int[3];
    int number;
    list->get_selection(&number,&selection);
    if (number!=1)
        return;
    else
        track_setno=selection[0];
    set_action(DO_NOTHING);
    SetLabel(lblInstructions, tr("Track points").toLocal8Bit().data());
    set_action(TRACKER);
    }
}

void frmPointExplorer::update_point_locator(int g_no, int setno, int loc)
{
    int col, ncols;
    Datapoint dpoint;
    char *s=new char[128], buf[64];
    if (get_datapoint(g_no, setno, loc, &ncols, &dpoint) == RETURN_SUCCESS)
    {
        SelectListChoice(list,setno);
        gno=g_no;
        sno=setno;
        loc_pos=loc;
        //s = copy_string(NULL, "(");
        strcpy(s,"(");
        for (col = 0; col < ncols; col++)
        {
            sprintf(buf, "%g", dpoint.ex[col]);
            SetDecimalSeparatorToUserValue(buf);
            //s = concat_strings(s, buf);
            strcat(s,buf);
            if (col != ncols - 1)
            {
                //s = concat_strings(s, ", ");
                if (DecimalPointToUse=='.')
                    strcat(s,", ");
                else//','
                    strcat(s," | ");
            }
        }
        if (dpoint.s != NULL)
        {
            if (DecimalPointToUse=='.')
                strcat(s,",\"");
            else
                strcat(s," | \"");

            strcat(s,dpoint.s);
            strcat(s,"\"");
            //s = concat_strings(s, ", \"");
            //s = concat_strings(s, dpoint.s);
            //s = concat_strings(s, "\"");
        }
        //s = concat_strings(s, ")");
        strcat(s,")");
        SetTextString(ledPointData, s);
        //xfree(s);
        sprintf(buf, "%d", loc);
        xv_setstr(ledPointLocation, buf);
        track_setno=setno;
    }
    else
    {
        track_setno = -1;
        ///SelectListChoices(track_set_sel, 0, NULL);
        SelectListChoices(list,0,NULL);
        SetTextString(ledPointData, QString("").toLocal8Bit().data());
        xv_setstr(ledPointLocation, QString("").toLocal8Bit().data());
    }
}

void frmPointExplorer::doMove(void)
{
    set_action(DO_NOTHING);
    track_move_dir = MOVE_POINT_XY;
    SetLabel(lblInstructions, tr("Move points").toLocal8Bit().data());
    set_action(MOVE_POINT1ST);
    selectionChanged(0);
}

void frmPointExplorer::doMoveX(void)
{
    set_action(DO_NOTHING);
    track_move_dir = MOVE_POINT_X;
    SetLabel(lblInstructions, tr("Move points along x").toLocal8Bit().data());
    set_action(MOVE_POINT1ST);
    selectionChanged(0);
}

void frmPointExplorer::doMoveY(void)
{
    set_action(DO_NOTHING);
    track_move_dir = MOVE_POINT_Y;
    SetLabel(lblInstructions, tr("Move points along y").toLocal8Bit().data());
    set_action(MOVE_POINT1ST);
    selectionChanged(0);
}

void frmPointExplorer::doPrepend(void)
{
    set_action(DO_NOTHING);
    track_add_at = ADD_POINT_BEGINNING;
    SetLabel(lblInstructions, tr("Add points at the beginning of set").toLocal8Bit().data());
    set_action(ADD_POINT);
    selectionChanged(0);
}

void frmPointExplorer::doAppend(void)
{
    set_action(DO_NOTHING);
    track_add_at = ADD_POINT_END;
    SetLabel(lblInstructions, tr("Add points to the end of set").toLocal8Bit().data());
    set_action(ADD_POINT);
    selectionChanged(0);
}

void frmPointExplorer::doInsert(void)
{
    set_action(DO_NOTHING);
    track_add_at = ADD_POINT_NEAREST;
    SetLabel(lblInstructions, tr("Add points to the nearest position").toLocal8Bit().data());
    set_action(ADD_POINT);
    selectionChanged(0);
}

void frmPointExplorer::doDelete(void)
{
    set_action(DO_NOTHING);
    SetLabel(lblInstructions, tr("Delete points").toLocal8Bit().data());
    set_action(DEL_POINT);
    selectionChanged(0);
}

void frmPointExplorer::doClose(void)
{
    set_action(DO_NOTHING);
    hide();
}

frmText_Props::frmText_Props(QWidget * parent,bool edit):QWidget(parent)
{
    int nr;
    int entry_v[8];
    QString entry_s[8];

    obj_id=-1;
    editWindow=edit;
    /*setFont(*stdFont);
if (edit)
setWindowTitle(tr("Edit String"));
else
setWindowTitle(tr("Strings"));
setWindowIcon(QIcon(*GraceIcon));*/

    strings_font_item=new FontSelector(this);
    strings_color_item=new ColorSelector(this);
    strings_just_item=new JustificationSelector(this);
    strings_loc_item=new PositionSelector(this);
    strings_rot_item=new stdSlider(this,tr("Rotation"),0,360);
    strings_size_item=new stdSlider(this,tr("Size"),0,1000);
    buttonGroup=new stdButtonGroup(this,false);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));
    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->setSpacing(STD_SPACING);
    if (edit)
    {
        string_item=new stdLineEdit(this,tr("String:"),true);
        selGno=new StdSelector(this,tr("Linked to graph:"),0,NULL);
        selGno->setValuesToGraphs(0,0);
        selGno->setToolTip(tr("Used if text-position is in World-coordinates,\nworld-coordintes are the axes of a graph,\nthe viewport refers to the whole page."));
        layout->addWidget(string_item);
        layout->addWidget(strings_color_item);
        layout->addWidget(strings_font_item);
    }
    else//properties in general
    {
        string_item=NULL;
        selGno=NULL;
        layout->addWidget(strings_font_item);
        layout->addWidget(strings_color_item);
    }
    layout->addWidget(strings_just_item);
    nr=3;
    entry_v[0]=MASTER_ALIGN_TARGET_NONE;
    entry_v[1]=MASTER_ALIGN_TARGET_GRAPH;
    entry_v[2]=MASTER_ALIGN_TARGET_PAGE;
    entry_s[0]=tr("None");
    entry_s[1]=tr("Graph");
    entry_s[2]=tr("Page");
    selMainAlignTarget=new StdSelector(this,tr("Align text with:"),nr,entry_s);
    selMainAlignTarget->setValues(entry_v);
    selMainAlignX=new StdSelector(this,tr("X-Align:"),nr,entry_s);
    selMainAlignX->setValues(entry_v);
    selMainAlignY=new StdSelector(this,tr("Y-Align:"),nr,entry_s);
    selMainAlignY->setValues(entry_v);
    layout->addWidget(selMainAlignTarget);
    layout->addWidget(selMainAlignX);
    layout->addWidget(selMainAlignY);
    if (edit)
    {
        layout->addWidget(selGno);
    }
    layout->addWidget(strings_loc_item);
    if (edit)
    {
        ledCoords[0]=new stdLineEdit(this,QString("X="));
        layout->addWidget(ledCoords[0]);
        ledCoords[1]=new stdLineEdit(this,QString("Y="));
        layout->addWidget(ledCoords[1]);
    }
    else
    {
        ledCoords[0]=NULL;
        ledCoords[1]=NULL;
    }
    layout->addWidget(strings_rot_item);
    layout->addWidget(strings_size_item);
    layout->addWidget(buttonGroup);
    setLayout(layout);
    strings_size_item->setValue(100);
    strings_color_item->setCurrentIndex(1);
    strings_loc_item->setCurrentIndex(1);
        if (edit)
        {
        connect(string_item->lenText,SIGNAL(returnPressed()),SLOT(update0()));
        connect(selGno->cmbSelect,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
            for (int i=0;i<2;i++)
            connect(ledCoords[i]->lenText,SIGNAL(returnPressed()),SLOT(update0()));
        }
    connect(strings_font_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(strings_color_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(strings_color_item,SIGNAL(alphaChanged(int)),SLOT(update1(int)));
    connect(strings_just_item->cmbJustSelect,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(strings_just_item->cmbJustSelect2,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(strings_loc_item->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(strings_rot_item,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(strings_size_item,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(strings_loc_item->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(viewCoordsChanged(int)));
    updateMasterAlign(MASTER_ALIGN_TARGET_NONE,FALSE);
    connect(selMainAlignTarget,SIGNAL(currentIndexChanged(int)),SLOT(updateMasterAlign(int)));
    connect(selMainAlignX,SIGNAL(currentIndexChanged(int)),SLOT(updateXYafterAlign(int)));
    connect(selMainAlignY,SIGNAL(currentIndexChanged(int)),SLOT(updateXYafterAlign(int)));
}

void frmText_Props::init(int id)
{
    if (id>=number_of_strings() || (id<0 && editWindow==true)) return;
    bool sav_imm_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    char buf[1024];
    int align_v[3];
    plotstr *pstring;
    obj_id=id;
    disconnect(strings_loc_item->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(viewCoordsChanged(int)));
    if (editWindow==false)
    {
        SetOptionChoice(strings_font_item, string_font);
        SetOptionChoice(strings_color_item, string_color);
        strings_color_item->setAlpha(string_alpha);
        SetCharSizeChoice(strings_size_item, string_size*100);
        SetAngleChoice(strings_rot_item, string_rot);
        strings_loc_item->setCurrentIndex(string_loctype == COORD_VIEW ? 1 : 0);
        SetOptionChoice(strings_just_item, string_just);
        strings_just_item->setCurrentIndex2(string_align);
        align_v[2]=string_master_align%10;
        align_v[1]=(string_master_align/10)%10;
        align_v[0]=(string_master_align/100)%10;
        selMainAlignTarget->setCurrentValue(align_v[0]);
        updateMasterAlign(align_v[0],FALSE);
        selMainAlignX->setCurrentValue(align_v[1]);
        selMainAlignY->setCurrentValue(align_v[2]);
        selGno->setEnabled(false);
    }
    else//edit existing string/text
    {
        selGno->setValuesToGraphs(0,0);
        pstring = &pstr[obj_id];
    //qDebug() << "Init: g=" << pstring->gno;
        align_v[2]=pstring->master_align%10;
        align_v[1]=(pstring->master_align/10)%10;
        align_v[0]=(pstring->master_align/100)%10;
    //qDebug() << "setting to master_align=" << pstring->master_align;
    //qDebug() << align_v[0] << "|" << align_v[1] << "|" << align_v[2];
        selMainAlignTarget->setCurrentValue(align_v[0]);
        updateMasterAlign(align_v[0],FALSE);
        selMainAlignX->setCurrentValue(align_v[1]);
        selMainAlignY->setCurrentValue(align_v[2]);
        selGno->setCurrentValue(pstring->gno);
        //SetTextString(string_item, pstring->s);
        string_item->SetTextToMemory(pstring->s_plotstring,pstring->alt_plotstring);
        SetOptionChoice(strings_color_item, pstring->color);
        strings_color_item->setAlpha(pstring->alpha);
        SetOptionChoice(strings_just_item, pstring->just);
        strings_just_item->setCurrentIndex2(pstring->align);
        SetOptionChoice(strings_font_item, pstring->font );
        SetCharSizeChoice(strings_size_item, pstring->charsize*100);
        SetAngleChoice(strings_rot_item, pstring->rot);
        strings_loc_item->setCurrentIndex(pstring->loctype == COORD_VIEW ? 1 : 0);
        sprintf(buf, "%.12f", pstring->x);
        SetDecimalSeparatorToUserValue(buf);
        xv_setstr(ledCoords[0], buf);
        sprintf(buf, "%.12f", pstring->y);
        SetDecimalSeparatorToUserValue(buf);
        xv_setstr(ledCoords[1], buf);
            /*if( pstring->loctype == COORD_VIEW && align_v[0]!=MASTER_ALIGN_TARGET_GRAPH)
            selGno->setEnabled(false);
            else*/
            selGno->setEnabled(true);
    }
    connect(strings_loc_item->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(viewCoordsChanged(int)));
    immediateUpdate=sav_imm_upd;
    updateRunning=false;
}

void frmText_Props::reDisplayContents(void)
{
    if (editWindow==true)
    init(obj_id);
    layout->update();
}

void frmText_Props::viewCoordsChanged(int i)
{
    (void)i;
    int stringno = obj_id;
    if (editWindow==false) return;
    if (editWindow==true && is_valid_string(stringno)==FALSE) return;
    if (pstr[stringno].loctype == strings_loc_item->currentValue()) return;
    bool old_upd=immediateUpdate;
    double d1,d2;
    int old_cur_graph=get_cg();
    immediateUpdate=false;
    updateRunning=true;
    xv_evalexpr(ledCoords[0],&d1);
    xv_evalexpr(ledCoords[1],&d2);

    select_graph(pstr[stringno].gno);
    if( pstr[stringno].loctype == COORD_VIEW )//old as VIEW, new coordinates are in world-coordinates (relative to graph-coordinates)
    {
    //selGno->setEnabled(false);
        //pstr[stringno].gno = get_cg();
        pstr[stringno].loctype = COORD_WORLD;
        //view2world( atof(xv_getstr(ledCoords[0])),atof(xv_getstr(ledCoords[1])),&pstr[stringno].x,&pstr[stringno].y );
        view2world( d1,d2,&pstr[stringno].x,&pstr[stringno].y );
    }
    else//old was WORLD, new coordinates are in viewport-coordinates (relative to page)
    {
    //selGno->setEnabled(true);
        pstr[stringno].loctype = COORD_VIEW;
        //world2view( atof(xv_getstr(ledCoords[0])),atof(xv_getstr(ledCoords[1])),&pstr[stringno].x,&pstr[stringno].y );
        world2view( d1,d2,&pstr[stringno].x,&pstr[stringno].y );
    }
    select_graph(old_cur_graph);
    init(obj_id);
    if ((pstr[stringno].master_align/100)%10==MASTER_ALIGN_TARGET_GRAPH) selGno->setEnabled(true);
    immediateUpdate=old_upd;
    updateRunning=false;
}

void frmText_Props::updateMasterAlign(int i,int keep_xy_setting)
{
(void)i;
//Warning: this function does NOT conserve selection of X-/Y-Alignment! They are reset to "None"
int nr;
int entry_v[8];
int old_x=selMainAlignX->currentValue();
int old_y=selMainAlignY->currentValue();
QString entry_s[8];
        if (editWindow)
        selGno->setEnabled(true);
        else
        selGno->setEnabled(false);

    if (i==MASTER_ALIGN_TARGET_NONE)
    {
    selMainAlignX->setVisible(false);
    selMainAlignY->setVisible(false);
    }
    else if (i==MASTER_ALIGN_TARGET_GRAPH)
    {
    nr=6;
    entry_v[0]=MASTER_ALIGN_X_NONE;
    entry_v[1]=MASTER_ALIGN_X_LEFT_IN;
    entry_v[2]=MASTER_ALIGN_X_LEFT_OUT;
    entry_v[3]=MASTER_ALIGN_X_CENTER;
    entry_v[4]=MASTER_ALIGN_X_RIGHT_IN;
    entry_v[5]=MASTER_ALIGN_X_RIGHT_OUT;
    entry_s[0]=tr("None");
    entry_s[1]=tr("Left, inwards");
    entry_s[2]=tr("Left, outwards");
    entry_s[3]=tr("Center");
    entry_s[4]=tr("Right, inwards");
    entry_s[5]=tr("Right, outwards");
    selMainAlignX->setNewEntries(nr,entry_s,entry_v);
    entry_v[0]=MASTER_ALIGN_Y_NONE;
    entry_v[1]=MASTER_ALIGN_Y_DOWN_IN;
    entry_v[2]=MASTER_ALIGN_Y_DOWN_OUT;
    entry_v[3]=MASTER_ALIGN_Y_CENTER;
    entry_v[4]=MASTER_ALIGN_Y_UP_IN;
    entry_v[5]=MASTER_ALIGN_Y_UP_OUT;
    entry_s[0]=tr("None");
    entry_s[1]=tr("Bottom, inwards");
    entry_s[2]=tr("Bottom, outwards");
    entry_s[3]=tr("Center");
    entry_s[4]=tr("Top, inwards");
    entry_s[5]=tr("Top, outwards");
    selMainAlignY->setNewEntries(nr,entry_s,entry_v);
    selMainAlignX->setVisible(true);
    selMainAlignY->setVisible(true);
    }
    else//MASTER_ALIGN_TARGET_PAGE
    {
    nr=4;
    entry_v[0]=MASTER_ALIGN_X_NONE;
    entry_v[1]=MASTER_ALIGN_X_LEFT_IN;
    entry_v[2]=MASTER_ALIGN_X_CENTER;
    entry_v[3]=MASTER_ALIGN_X_RIGHT_IN;
    entry_s[0]=tr("None");
    entry_s[1]=tr("Left, inwards");
    entry_s[2]=tr("Center");
    entry_s[3]=tr("Right, inwards");
    selMainAlignX->setNewEntries(nr,entry_s,entry_v);
    entry_v[0]=MASTER_ALIGN_Y_NONE;
    entry_v[1]=MASTER_ALIGN_Y_DOWN_IN;
    entry_v[2]=MASTER_ALIGN_Y_CENTER;
    entry_v[3]=MASTER_ALIGN_Y_UP_IN;
    entry_s[0]=tr("None");
    entry_s[1]=tr("Bottom, inwards");
    entry_s[2]=tr("Center");
    entry_s[3]=tr("Top, inwards");
    selMainAlignY->setNewEntries(nr,entry_s,entry_v);
    selMainAlignX->setVisible(true);
    selMainAlignY->setVisible(true);
    }
    if (keep_xy_setting==TRUE)
    {
    selMainAlignX->setCurrentValue(old_x);
    selMainAlignY->setCurrentValue(old_y);
    }
    updateXYafterAlign(0);
}

void frmText_Props::updateXYafterAlign(int i)
{
(void)i;
int m_align_t=selMainAlignTarget->currentValue();
int m_align_x=selMainAlignX->currentValue();
int m_align_y=selMainAlignY->currentValue();
if (editWindow)
{
if (m_align_t==MASTER_ALIGN_TARGET_NONE)//no special alignment --> everything as usual
{
    ledCoords[0]->setEnabled(true);
    ledCoords[1]->setEnabled(true);
}
else
{
    if (m_align_x==MASTER_ALIGN_X_NONE)
    ledCoords[0]->setEnabled(true);
    else
    ledCoords[0]->setEnabled(false);

    if (m_align_y==MASTER_ALIGN_Y_NONE)
    ledCoords[1]->setEnabled(true);
    else
    ledCoords[1]->setEnabled(false);
}
}
update0();
}

void frmText_Props::doAccept(void)
{
    if (editWindow==false)//edit preferences
    {
        string_font = GetOptionChoice(strings_font_item);
        string_color = GetOptionChoice(strings_color_item);
        string_alpha = strings_color_item->alpha();
        string_size = GetCharSizeChoice(strings_size_item)/100.0;
        string_rot = GetAngleChoice(strings_rot_item);
        string_loctype = GetChoice(strings_loc_item) ? COORD_VIEW : COORD_WORLD;
        string_just = GetOptionChoice(strings_just_item);
        string_align = strings_just_item->currentIndex2();
        string_master_align = selMainAlignTarget->currentValue()*100+selMainAlignX->currentValue()*10+selMainAlignY->currentValue();
    }
    else//edit a single string-object
    {
        if (is_valid_string(obj_id)==FALSE) return;
        int stringno=obj_id;
        double d1,d2;
        int counter=0;
        bool changes=false;
        ListOfChanges.clear();
        ListOfOldStates.clear();
        sprintf(dummy,"with string %d",stringno);
        ListOfChanges << QString(dummy);
        ListOfOldStates << QString(dummy);
        if (pstr[stringno].active==false)
        {
            changes=true;
        }
        ListOfChanges << QString("    string on");
        ListOfOldStates << QString("    string off");
        counter++;
        //}
        if (pstr[stringno].color != GetOptionChoice(strings_color_item))
        {
            changes=true;
        }
        sprintf(dummy,"    string color %d",GetOptionChoice(strings_color_item));
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    string color %d",pstr[stringno].color);
        ListOfOldStates << QString(dummy);
        counter++;
        //}
        if (pstr[stringno].loctype != (GetChoice(strings_loc_item)? COORD_VIEW : COORD_WORLD) )
        {
            changes=true;
        }
        sprintf(dummy,"    string loctype %s",GetChoice(strings_loc_item)?"view":"world");
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    string loctype %s",pstr[stringno].loctype?"view":"world");
        ListOfOldStates << QString(dummy);
        counter++;
        //}
        if (pstr[stringno].gno != selGno->currentValue())
        {
            changes=true;
        }
        sprintf(dummy,"    string g%1d",selGno->currentValue());
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    string g%1d",pstr[stringno].gno);
        ListOfOldStates << QString(dummy);
        counter++;
        //}
        if (pstr[stringno].font != GetOptionChoice(strings_font_item))
        {
            changes=true;
        }
        sprintf(dummy,"    string font %d",GetOptionChoice(strings_font_item));
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    string font %d",pstr[stringno].font);
        ListOfOldStates << QString(dummy);
        counter++;
        //}
        if (pstr[stringno].just != GetOptionChoice(strings_just_item))
        {
            changes=true;
        }
        sprintf(dummy,"    string just %d",GetOptionChoice(strings_just_item));
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    string just %d",pstr[stringno].just);
        ListOfOldStates << QString(dummy);
        counter++;
        //}
        if (pstr[stringno].rot != GetAngleChoice(strings_rot_item))
        {
            changes=true;
        }
        sprintf(dummy,"    string rot %f",GetAngleChoice(strings_rot_item));
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    string rot %d",pstr[stringno].rot);
        ListOfOldStates << QString(dummy);
        counter++;
        //}
        if (pstr[stringno].charsize != GetCharSizeChoice(strings_size_item)/100.0)
        {
            changes=true;
        }
        sprintf(dummy,"    string char size %f",GetCharSizeChoice(strings_size_item)/100.0);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    string char size %f",pstr[stringno].charsize);
        ListOfOldStates << QString(dummy);
        counter++;
        //}
        if (pstr[stringno].alpha != strings_color_item->alpha())
        {
            changes=true;
        }
        sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: STRING %d ALPHA %d\n",stringno,strings_color_item->alpha());
        ListOfChanges << QString(dummy);
        sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: STRING %d ALPHA %d\n",stringno,pstr[stringno].alpha);
        ListOfOldStates << QString(dummy);

        int m_align=selMainAlignTarget->currentValue()*100+selMainAlignX->currentValue()*10+selMainAlignY->currentValue();
        if (pstr[stringno].align != strings_just_item->currentIndex2() || pstr[stringno].master_align != m_align)
        {
            changes=true;
        }
        sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: STRING %d ALIGN %d %d\n",stringno,strings_just_item->currentIndex2(),m_align);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: STRING %d ALIGN %d %d\n",stringno,pstr[stringno].align,pstr[stringno].master_align);
        ListOfOldStates << QString(dummy);

        xv_evalexpr(ledCoords[0],&d1);
        xv_evalexpr(ledCoords[1],&d2);
        if (d1!=pstr[stringno].x || d2!=pstr[stringno].y)
        {
            changes=true;
        }
        sprintf(dummy,"    string %f, %f",d1,d2);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    string %f, %f",pstr[stringno].x,pstr[stringno].y);
        ListOfOldStates << QString(dummy);
        counter++;
        //}
        char * ch1,*ch2;
        ch1=pstr[stringno].alt_plotstring;//alt=original in UTF8
        ch2=GetTextString(string_item);
        if (!(ch1==NULL || ch2==NULL))
        {
            if (strcmp(ch1,ch2) || counter>0)
            {
                changes=true;
            }
        }
        else if (ch1!=ch2 || counter>0) changes=true;//counter is always >0 --> changes always==true
        sprintf(dummy,"    string def \"%s\"",GetTextString(string_item));
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    string def \"%s\"",pstr[stringno].alt_plotstring);
        ListOfOldStates << QString(dummy);
        //}
        if (slider_status<=1)//this means: no slider action or slider just started to slide and we haven't saved the last state yet
        {
            //cout << "slider_status=" << slider_status << " saving settings" << endl;
            SaveObjectData(stringno,OBJECT_STRING);
            if (slider_status==1)//we saved the old state before slider action, no more saving dring slider action
            {
                slider_status=2;
                /*if (strings_rot_item->sldSlider->isSliderDown()==true)
                    {
                    slider_start_value=pstr[stringno].rot;//value before slider change
                    cout << "RotationChanged" << endl;
                    }
                    else if (strings_size_item->sldSlider->isSliderDown()==true)
                    {
                    slider_start_value=pstr[stringno].charsize;
                    cout << "CharSizeChanged" << endl;
                    }*/
            }
            else//slider_status=0;
            {
                strings_rot_item->old_value=strings_rot_item->value();
                strings_size_item->old_value=strings_size_item->value();
            }
        }
        /// do the actual changes
        string_item->DynSetMemoryToText(pstr[stringno].s_plotstring,pstr[stringno].alt_plotstring);
        //pstr[stringno].s = copy_string(pstr[stringno].s, GetTextString(string_item));
        pstr[stringno].color = GetOptionChoice(strings_color_item);
        pstr[stringno].alpha = strings_color_item->alpha();
        pstr[stringno].loctype = GetChoice(strings_loc_item) ? COORD_VIEW : COORD_WORLD;
        pstr[stringno].gno = selGno->currentValue();
        pstr[stringno].font = GetOptionChoice(strings_font_item);
        pstr[stringno].just = GetOptionChoice(strings_just_item);
        pstr[stringno].align = strings_just_item->currentIndex2();
        pstr[stringno].master_align = m_align;
        //qDebug() << "setting master_align=" << m_align;
        pstr[stringno].x=d1;
        pstr[stringno].y=d2;
        pstr[stringno].charsize = GetCharSizeChoice(strings_size_item)/100.0;
        pstr[stringno].rot = GetAngleChoice(strings_rot_item);
        /// finished doing the changes
        if (slider_status==0 || slider_status==3)//no slider action or the slider just stopped to slide
        {
            //cout << "slider status = " << slider_status << endl;
            if (slider_status==3)//slider action stopped
            {
                ListOfOldStates.clear();//during slider action only the slider values could have been changed
                sprintf(dummy,"with string %d",stringno);
                ListOfOldStates << QString(dummy);

                if (strings_rot_item->old_value!=strings_rot_item->value())
                {
                    //cout << "rotation changed: " << strings_rot_item->old_value << " --> " << strings_rot_item->value() << endl;
                    sprintf(dummy,"    string rot %d",strings_rot_item->old_value);
                    ListOfOldStates << QString(dummy);
                    strings_rot_item->old_value=strings_rot_item->value();
                    ObjectDataModified(stringno,OBJECT_STRING);
                }
                else if (strings_size_item->old_value!=strings_size_item->value())
                {
                    //cout << "char size changed: " << strings_size_item->old_value << " --> " << strings_size_item->value() << endl;
                    sprintf(dummy,"    string char size %f",strings_size_item->old_value/100.0);
                    ListOfOldStates << QString(dummy);
                    strings_size_item->old_value=strings_size_item->value();
                    ObjectDataModified(stringno,OBJECT_STRING);
                }
                /*if (strings_rot_item->sldSlider->isSliderDown()==true)
                {
                //slider_start_value=pstr[stringno].rot;//value before slider change
                cout << "RotationChanged" << endl;
                }
                else if (strings_size_item->sldSlider->isSliderDown()==true)
                {
                //slider_start_value=pstr[stringno].charsize;
                cout << "CharSizeChanged" << endl;
                }*/

            }
            else if (slider_status==0)
            {
                ObjectDataModified(stringno,OBJECT_STRING);
            }
            slider_status=0;//we saved the new settings of the string
        }
        set_dirtystate();
        if (GlobalInhibitor==false)
            mainWin->mainArea->completeRedraw();
        if (changes==false)
        {
            ListOfChanges.clear();
            ListOfOldStates.clear();
        }
    }
    set_action(action_flag);
}

void frmText_Props::doClose(void)
{
    parentWidget()->hide();
    set_action(action_flag);
}

//immediate updates
void frmText_Props::update0(void)
{
static int i,nr;
    if (!immediateUpdate) return;
    GlobalInhibitor=true;
    ListOfChanges.clear();
    ListOfOldStates.clear();
    doAccept();//do this without actually doing something
    nr=nr_of_true_changes(ListOfChanges);
    //errpos=0;
    if (nr>0 && is_valid_string(obj_id)==TRUE)
    {
        for (i=0;i<ListOfChanges.size();i++)
        {
            strcpy(command,ListOfChanges.at(i).toLocal8Bit().constData());
            //errpos = scanner(command);
            (void)scanner(command);
            if (ListOfChanges.at(i).contains(QString("string def")))
            {
                string_item->DynSetMemoryToText(pstr[obj_id].s_plotstring,pstr[obj_id].alt_plotstring);
            }
        }
        set_dirtystate();
        mainWin->mainArea->completeRedraw();
    }
    ListOfChanges.clear();
    ListOfOldStates.clear();
    GlobalInhibitor=false;
}

void frmText_Props::update1(int v)
{
    (void)v;
    update0();
}

void frmText_Props::update2(QString v)
{
    (void)v;
    update0();
}

void frmText_Props::update3(bool v)
{
    (void)v;
    update0();
}

void frmText_Props::update4(double v)
{
    (void)v;
    update0();
}

frmTextProps::frmTextProps(QWidget * parent,bool edit):QDialog(parent)
{
//setFont(*stdFont);
    if (edit)
        setWindowTitle(tr("Edit String"));
    else
        setWindowTitle(tr("Strings"));
    setWindowIcon(QIcon(*GraceIcon));
    QVBoxLayout * layout=new QVBoxLayout;
    //layout->setMargin(0);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    flp=new frmText_Props(this,edit);
    layout->addWidget(flp);
    setLayout(layout);
    if (edit==true)
    {
        connect(flp->string_item,SIGNAL(NoMoreValidAdr()),SLOT(doClose()));
        resize(LastSize_EditTextProps);
    }
    else
    {
        resize(LastSize_TextProps);
    }
}

frmTextProps::~frmTextProps()
{
    if (flp->editWindow)
    LastSize_EditTextProps=this->size();
    else
    LastSize_TextProps=this->size();
}

void frmTextProps::init(int id)
{
    flp->init(id);
}

void frmTextProps::doAccept(void)
{
    flp->doAccept();
}

void frmTextProps::doClose(void)
{
    //flp->doClose();
    hide();
}

frmDrawObjects::frmDrawObjects(QWidget * parent):QDialog(parent)
{
//setFont(*stdFont);
    setWindowTitle(tr("QtGrace: Objects"));
    setWindowIcon(QIcon(*GraceIcon));
    cmdText=new QPushButton(tr("Text"));
    cmdText->setToolTip(tr("Create a text annotation (Crtl+Alt+T)"));
    connect(cmdText,SIGNAL(clicked()),SLOT(doText()));
    cmdTextProp=new QPushButton(tr("Text props..."));
    cmdTextProp->setToolTip(tr("Edit general text properties"));
    connect(cmdTextProp,SIGNAL(clicked()),SLOT(doTextProp()));
    cmdLine=new QPushButton(tr("Line"));
    cmdLine->setToolTip(tr("Create a line or an arrow (Crtl+Alt+L)"));
    connect(cmdLine,SIGNAL(clicked()),SLOT(doLine()));
    cmdLineProp=new QPushButton(tr("Line props..."));
    cmdLineProp->setToolTip(tr("Edit general line properties"));
    connect(cmdLineProp,SIGNAL(clicked()),SLOT(doLineProp()));
    cmdBox=new QPushButton(tr("Box"));
    cmdBox->setToolTip(tr("Create a box (Crtl+Alt+B)"));
    connect(cmdBox,SIGNAL(clicked()),SLOT(doBox()));
    cmdBoxProp=new QPushButton(tr("Box props..."));
    cmdBoxProp->setToolTip(tr("Edit general box properties"));
    connect(cmdBoxProp,SIGNAL(clicked()),SLOT(doBoxProp()));
    cmdEllipse=new QPushButton(tr("Ellipse"));
    cmdEllipse->setToolTip(tr("Create an ellipse (Crtl+Alt+E)"));
    connect(cmdEllipse,SIGNAL(clicked()),SLOT(doEllipse()));
    cmdEllipseProp=new QPushButton(tr("Ellipse props..."));
    cmdEllipseProp->setToolTip(tr("Edit general ellipse properties"));
    connect(cmdEllipseProp,SIGNAL(clicked()),SLOT(doEllipseProp()));
    cmdEditObj=new QPushButton(tr("Edit object"));
    cmdEditObj->setToolTip(tr("Edit an existing object"));
    connect(cmdEditObj,SIGNAL(clicked()),SLOT(doEditObj()));
    cmdMoveObj=new QPushButton(tr("Move object"));
    cmdMoveObj->setToolTip(tr("Move an existing object (Crtl+M)"));
    connect(cmdMoveObj,SIGNAL(clicked()),SLOT(doMoveObj()));
    cmdCopyObj=new QPushButton(tr("Copy object"));
    cmdCopyObj->setToolTip(tr("Copy an object"));
    connect(cmdCopyObj,SIGNAL(clicked()),SLOT(doCopyObj()));
    cmdDelObj=new QPushButton(tr("Delete object"));
    cmdDelObj->setToolTip(tr("Delete an object (Crtl+D)"));
    connect(cmdDelObj,SIGNAL(clicked()),SLOT(doDelObj()));
    cmdClearAllText=new QPushButton(tr("Clear all text"));
    cmdClearAllText->setToolTip(tr("Delete all text objects"));
    connect(cmdClearAllText,SIGNAL(clicked()),SLOT(doClearAllText()));
    cmdClearAllLines=new QPushButton(tr("Clear all lines"));
    cmdClearAllLines->setToolTip(tr("Delete all lines and arrows"));
    connect(cmdClearAllLines,SIGNAL(clicked()),SLOT(doClearAllLines()));
    cmdClearAllBoxes=new QPushButton(tr("Clear all boxes"));
    cmdClearAllBoxes->setToolTip(tr("Delete all box objects"));
    connect(cmdClearAllBoxes,SIGNAL(clicked()),SLOT(doClearAllBoxes()));
    cmdClearAllEllipses=new QPushButton(tr("Clear all ellipses"));
    cmdClearAllEllipses->setToolTip(tr("Delete all ellipse objects"));
    connect(cmdClearAllEllipses,SIGNAL(clicked()),SLOT(doClearAllEllipses()));
    cmdClose=new QPushButton(tr("Close"));
    cmdClose->setToolTip(tr("Close this dialog"));
    connect(cmdClose,SIGNAL(clicked()),SLOT(doClose()));
    CreateIcons();
    layout=new QGridLayout;
    layout->setSpacing(STD_SPACING);
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(cmdText,0,0);
    layout->addWidget(cmdTextProp,1,0);
    layout->addWidget(cmdLine,2,0);
    layout->addWidget(cmdLineProp,3,0);
    layout->addWidget(cmdBox,4,0);
    layout->addWidget(cmdBoxProp,5,0);
    layout->addWidget(cmdEllipse,6,0);
    layout->addWidget(cmdEllipseProp,7,0);
    layout->addWidget(cmdEditObj,0,1);
    layout->addWidget(cmdMoveObj,1,1);
    layout->addWidget(cmdCopyObj,2,1);
    layout->addWidget(cmdDelObj,3,1);
    layout->addWidget(cmdClearAllText,4,1);
    layout->addWidget(cmdClearAllLines,5,1);
    layout->addWidget(cmdClearAllBoxes,6,1);
    layout->addWidget(cmdClearAllEllipses,7,1);
    layout->addWidget(cmdClose,8,1);
    setLayout(layout);
    resize(LastSize_FormDrawObjects);
    redisplayButtons();
}

frmDrawObjects::~frmDrawObjects()
{
    LastSize_FormDrawObjects=this->size();
}

QIcon CreateIconFromPNG(QString png_file,QSize s)
{
QImage img1;
img1.load(png_file);
QImage img2(img1.size(),QImage::Format_ARGB32);
QRgb r1;
QColor col1;
QPainter paint1;
img2.fill(Qt::transparent);
int w,h;
w=img1.width();
h=img1.height();
paint1.begin(&img2);
for (int i=0;i<h;i++)
{
    for (int j=0;j<w;j++)
    {
    r1=img1.pixel(j,i);
        if (qRed(r1)==255 && qGreen(r1)==255 && qBlue(r1)==255)
        {
        ;
        }
        else
        {
        col1=QColor(r1);
        paint1.setPen(col1);
        paint1.drawPoint(j,i);
        }
    }
}
paint1.end();
if (s.width()==0 && s.height()==0)//unscaled
return QIcon(QPixmap::fromImage(img2,Qt::AutoColor));
else
return QIcon(QPixmap::fromImage(img2,Qt::AutoColor).scaled(s,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
}

QIcon CreateScaledIconFromPNG(QString png_file, QSize s)
{
static QIcon tmp1;
static QPixmap pm1,pm2;
tmp1=QIcon(png_file);
pm1=tmp1.pixmap(s);
pm2=pm1.scaled(s,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
return QIcon(pm2);
}

void frmDrawObjects::CreateIcons(void)
{
    QString icondir=QString(qt_grace_icons_dir);//grace_path("fonts/icons");
    icondir+=QDir::separator();
    QSize newIconSize(mainWin->default_IconW*toolBarSizeFactor,mainWin->default_IconH*toolBarSizeFactor);
/*buttonIcons[0]=QIcon(icondir+"CreateText.png");
buttonIcons[1]=QIcon(icondir+"TextProps.png");
buttonIcons[2]=QIcon(icondir+"DrawLine.png");
buttonIcons[3]=QIcon(icondir+"LineProps.png");
buttonIcons[4]=QIcon(icondir+"DrawBox.png");
buttonIcons[5]=QIcon(icondir+"BoxProps.png");
buttonIcons[6]=QIcon(icondir+"DrawEllipse.png");
buttonIcons[7]=QIcon(icondir+"EllipseProps.png");
buttonIcons[8]=QIcon(icondir+"EditObject.png");
buttonIcons[9]=QIcon(icondir+"MoveObject.png");
buttonIcons[10]=QIcon(icondir+"CopyObject.png");
buttonIcons[11]=QIcon(icondir+"DeleteObject.png");
buttonIcons[12]=QIcon(icondir+"ClearText2.png");
buttonIcons[13]=QIcon(icondir+"ClearLines.png");
buttonIcons[14]=QIcon(icondir+"ClearBoxes.png");
buttonIcons[15]=QIcon(icondir+"ClearEllipses.png");
buttonIcons[16]=QIcon(icondir+"CloseObjects.png");*/
    buttonIcons[0]=CreateIconFromPNG(icondir+"CreateText.png",newIconSize);
    buttonIcons[1]=CreateIconFromPNG(icondir+"TextProps.png",newIconSize);
    buttonIcons[2]=CreateIconFromPNG(icondir+"DrawLine.png",newIconSize);
    buttonIcons[3]=CreateIconFromPNG(icondir+"LineProps.png",newIconSize);
    buttonIcons[4]=CreateIconFromPNG(icondir+"DrawBox.png",newIconSize);
    buttonIcons[5]=CreateIconFromPNG(icondir+"BoxProps.png",newIconSize);
    buttonIcons[6]=CreateIconFromPNG(icondir+"DrawEllipse.png",newIconSize);
    buttonIcons[7]=CreateIconFromPNG(icondir+"EllipseProps.png",newIconSize);
    buttonIcons[8]=CreateIconFromPNG(icondir+"EditObject.png",newIconSize);
    buttonIcons[9]=CreateIconFromPNG(icondir+"MoveObject.png",newIconSize);
    buttonIcons[10]=CreateIconFromPNG(icondir+"CopyObject.png",newIconSize);
    buttonIcons[11]=CreateIconFromPNG(icondir+"DeleteObject.png",newIconSize);
    buttonIcons[12]=CreateIconFromPNG(icondir+"ClearText2.png",newIconSize);
    buttonIcons[13]=CreateIconFromPNG(icondir+"ClearLines.png",newIconSize);
    buttonIcons[14]=CreateIconFromPNG(icondir+"ClearBoxes.png",newIconSize);
    buttonIcons[15]=CreateIconFromPNG(icondir+"ClearEllipses.png",newIconSize);
    buttonIcons[16]=CreateIconFromPNG(icondir+"CloseObjects.png",newIconSize);
}

void frmDrawObjects::redisplayButtons(void)
{
    CreateIcons();
if (icon_preferences==2)//icons only
{
    cmdText->setText(QString(""));
    cmdTextProp->setText(QString(""));
    cmdLine->setText(QString(""));
    cmdLineProp->setText(QString(""));
    cmdBox->setText(QString(""));
    cmdBoxProp->setText(QString(""));
    cmdEllipse->setText(QString(""));
    cmdEllipseProp->setText(QString(""));
    cmdEditObj->setText(QString(""));
    cmdMoveObj->setText(QString(""));
    cmdCopyObj->setText(QString(""));
    cmdDelObj->setText(QString(""));
    cmdClearAllText->setText(QString(""));
    cmdClearAllLines->setText(QString(""));
    cmdClearAllBoxes->setText(QString(""));
    cmdClearAllEllipses->setText(QString(""));
    cmdClose->setText(QString(""));
    layout->setColumnMinimumWidth(0,30);
    layout->setColumnMinimumWidth(1,30);
    this->setMaximumWidth(70);
    this->resize(70,this->height());
}
else//text or text+icons
{
    cmdText->setText(tr("Text"));
    cmdTextProp->setText(tr("Text props..."));
    cmdLine->setText(tr("Line"));
    cmdLineProp->setText(tr("Line props..."));
    cmdBox->setText(tr("Box"));
    cmdBoxProp->setText(tr("Box props..."));
    cmdEllipse->setText(tr("Ellipse"));
    cmdEllipseProp->setText(tr("Ellipse props..."));
    cmdEditObj->setText(tr("Edit object"));
    cmdMoveObj->setText(tr("Move object"));
    cmdCopyObj->setText(tr("Copy object"));
    cmdDelObj->setText(tr("Delete object"));
    cmdClearAllText->setText(tr("Clear all text"));
    cmdClearAllLines->setText(tr("Clear all lines"));
    cmdClearAllBoxes->setText(tr("Clear all boxes"));
    cmdClearAllEllipses->setText(tr("Clear all ellipses"));
    cmdClose->setText(tr("Close"));
    layout->setColumnMinimumWidth(0,100);
    layout->setColumnMinimumWidth(1,100);
    this->setMaximumWidth(250);
    this->resize(230,this->height());
}

if (icon_preferences==0)//text only
{
    cmdText->setIcon(QIcon());
    cmdTextProp->setIcon(QIcon());
    cmdLine->setIcon(QIcon());
    cmdLineProp->setIcon(QIcon());
    cmdBox->setIcon(QIcon());
    cmdBoxProp->setIcon(QIcon());
    cmdEllipse->setIcon(QIcon());
    cmdEllipseProp->setIcon(QIcon());
    cmdEditObj->setIcon(QIcon());
    cmdMoveObj->setIcon(QIcon());
    cmdCopyObj->setIcon(QIcon());
    cmdDelObj->setIcon(QIcon());
    cmdClearAllText->setIcon(QIcon());
    cmdClearAllLines->setIcon(QIcon());
    cmdClearAllBoxes->setIcon(QIcon());
    cmdClearAllEllipses->setIcon(QIcon());
    cmdClose->setIcon(QIcon());
}
else//icons only or icons+text
{
    QSize newIconSize(mainWin->default_IconW*toolBarSizeFactor,mainWin->default_IconH*toolBarSizeFactor);
    cmdText->setIcon(buttonIcons[0]);
    cmdTextProp->setIcon(buttonIcons[1]);
    cmdLine->setIcon(buttonIcons[2]);
    cmdLineProp->setIcon(buttonIcons[3]);
    cmdBox->setIcon(buttonIcons[4]);
    cmdBoxProp->setIcon(buttonIcons[5]);
    cmdEllipse->setIcon(buttonIcons[6]);
    cmdEllipseProp->setIcon(buttonIcons[7]);
    cmdEditObj->setIcon(buttonIcons[8]);
    cmdMoveObj->setIcon(buttonIcons[9]);
    cmdCopyObj->setIcon(buttonIcons[10]);
    cmdDelObj->setIcon(buttonIcons[11]);
    cmdClearAllText->setIcon(buttonIcons[12]);
    cmdClearAllLines->setIcon(buttonIcons[13]);
    cmdClearAllBoxes->setIcon(buttonIcons[14]);
    cmdClearAllEllipses->setIcon(buttonIcons[15]);
    cmdClose->setIcon(buttonIcons[16]);

    cmdText->setIconSize(newIconSize);
    cmdTextProp->setIconSize(newIconSize);
    cmdLine->setIconSize(newIconSize);
    cmdLineProp->setIconSize(newIconSize);
    cmdBox->setIconSize(newIconSize);
    cmdBoxProp->setIconSize(newIconSize);
    cmdEllipse->setIconSize(newIconSize);
    cmdEllipseProp->setIconSize(newIconSize);
    cmdEditObj->setIconSize(newIconSize);
    cmdMoveObj->setIconSize(newIconSize);
    cmdCopyObj->setIconSize(newIconSize);
    cmdDelObj->setIconSize(newIconSize);
    cmdClearAllText->setIconSize(newIconSize);
    cmdClearAllLines->setIconSize(newIconSize);
    cmdClearAllBoxes->setIconSize(newIconSize);
    cmdClearAllEllipses->setIconSize(newIconSize);
    cmdClose->setIconSize(newIconSize);
}
this->setFont(qApp->font());
this->adjustSize();
}

void frmDrawObjects::doText(void)
{
    if (action_flag==STR_LOC)
    set_action(DO_NOTHING);
    else
    set_action(STR_LOC);
}

void frmDrawObjects::doTextProp(void)
{
    if (TextProps==NULL)
    {
        TextProps=new frmTextProps(mainWin);
    }
    TextProps->show();
    TextProps->raise();
}

void frmDrawObjects::doLine(void)
{
    if (action_flag==MAKE_LINE_1ST || action_flag==MAKE_LINE_2ND)
    set_action(DO_NOTHING);
    else
    set_action(MAKE_LINE_1ST);
}

void frmDrawObjects::doLineProp(void)
{
    if (LineProps==NULL)
    {
        LineProps=new frmLineProps(mainWin);
    }
    LineProps->show();
    LineProps->raise();
}

void frmDrawObjects::doBox(void)
{
    if (action_flag==MAKE_BOX_1ST || action_flag==MAKE_BOX_2ND)
    set_action(DO_NOTHING);
    else
    set_action(MAKE_BOX_1ST);
}

void frmDrawObjects::doBoxProp(void)
{
    if (BoxProps==NULL)
    {
        BoxProps=new frmEllipseProps(mainWin,false,false);
    }
    BoxProps->init(0);//id is ignored here
    BoxProps->show();
    BoxProps->raise();
}

void frmDrawObjects::doEllipse(void)
{
    if (action_flag==MAKE_ELLIP_1ST || action_flag==MAKE_ELLIP_2ND)
    set_action(DO_NOTHING);
    else
    set_action(MAKE_ELLIP_1ST);
}

void frmDrawObjects::doEllipseProp(void)
{
    if (EllipseProps==NULL)
    {
        EllipseProps=new frmEllipseProps(mainWin,false,true);
    }
    EllipseProps->init(0);//id is ignored here
    EllipseProps->show();
    EllipseProps->raise();
}

void frmDrawObjects::doEditObj(void)
{
    if (action_flag==EDIT_OBJECT)
    set_action(DO_NOTHING);
    else
    set_action(EDIT_OBJECT);
}

void frmDrawObjects::doMoveObj(void)
{
    if (action_flag==MOVE_OBJECT_1ST || action_flag==MOVE_OBJECT_2ND)
    set_action(DO_NOTHING);
    else
    set_action(MOVE_OBJECT_1ST);
}

void frmDrawObjects::doCopyObj(void)
{
    if (action_flag==COPY_OBJECT1ST || action_flag==COPY_OBJECT2ND)
    set_action(DO_NOTHING);
    else
    set_action(COPY_OBJECT1ST);
}

void frmDrawObjects::doDelObj(void)
{
    if (action_flag==DEL_OBJECT)
    set_action(DO_NOTHING);
    else
    set_action(DEL_OBJECT);
}

void frmDrawObjects::doClearAllText(void)
{
    if (yesno("Delete all text strings?", NULL, NULL, NULL))
    {
        nrOfUndoObjs=0;
        undoObjs=new int[maxstr];
        for (int i=0;i<maxstr;i++)
        {
            if (pstr[i].active==TRUE)
            {
                undoObjs[nrOfUndoObjs++]=i;
            }
        }
        if (nrOfUndoObjs>0)
            ObjectsDeleted(nrOfUndoObjs,undoObjs,OBJECT_STRING);
        delete[] undoObjs;
        do_clear_text();
        mainWin->mainArea->completeRedraw();
    }
}

void frmDrawObjects::doClearAllLines(void)
{
    if (yesno("Delete all lines?", NULL, NULL, NULL))
    {
        nrOfUndoObjs=0;
        undoObjs=new int[maxlines];
        for (int i=0;i<maxlines;i++)
        {
            if (lines[i].active==TRUE)
            {
                undoObjs[nrOfUndoObjs++]=i;
            }
        }
        if (nrOfUndoObjs>0)
            ObjectsDeleted(nrOfUndoObjs,undoObjs,OBJECT_LINE);
        delete[] undoObjs;
        do_clear_lines();
        mainWin->mainArea->completeRedraw();
    }
}

void frmDrawObjects::doClearAllBoxes(void)
{
    if (yesno("Delete all boxes?", NULL, NULL, NULL))
    {
        nrOfUndoObjs=0;
        undoObjs=new int[maxboxes];
        for (int i=0;i<maxboxes;i++)
        {
            if (boxes[i].active==TRUE)
            {
                undoObjs[nrOfUndoObjs++]=i;
            }
        }
        if (nrOfUndoObjs>0)
            ObjectsDeleted(nrOfUndoObjs,undoObjs,OBJECT_BOX);
        delete[] undoObjs;
        do_clear_boxes();
        mainWin->mainArea->completeRedraw();
    }
}

void frmDrawObjects::doClearAllEllipses(void)
{
    if (yesno("Delete all ellipses?", NULL, NULL, NULL))
    {
        nrOfUndoObjs=0;
        undoObjs=new int[maxellipses];
        for (int i=0;i<maxellipses;i++)
        {
            if (ellip[i].active==TRUE)
            {
                undoObjs[nrOfUndoObjs++]=i;
            }
        }
        if (nrOfUndoObjs>0)
            ObjectsDeleted(nrOfUndoObjs,undoObjs,OBJECT_ELLIPSE);
        delete[] undoObjs;
        do_clear_ellipses();
        mainWin->mainArea->completeRedraw();
    }
}

void frmDrawObjects::doClose(void)
{
    set_action(DO_NOTHING);
    hide();
}

frmConsole::frmConsole(QWidget * parent):QDialog(parent)
{
//setFont(*stdFont);
    setWindowTitle(tr("QtGrace: Console"));
    setWindowIcon(QIcon(*GraceIcon));
    CreateActions();

    menuBar=new QMenuBar(this);
    mnuFile=new QMenu("&File",this);
    mnuFile->setTearOffEnabled(TRUE);
    mnuFile->addAction(actSave);
    mnuFile->addAction(actClose);
    mnuEdit=new QMenu("&Edit",this);
    mnuEdit->setTearOffEnabled(TRUE);
    mnuEdit->addAction(actCopy);
    mnuEdit->addAction(actClear);
    mnuOptions=new QMenu("&Options",this);
    mnuOptions->setTearOffEnabled(TRUE);
    mnuOptions->addAction(actPopOnError);
    mnuHelp=new QMenu("&Help",this);
    mnuHelp->setTearOffEnabled(TRUE);
    mnuHelp->addAction(actHelpOnContext);
    mnuHelp->addSeparator();
    mnuHelp->addAction(actHelpOnConsole);

    menuBar->addMenu(mnuFile);
    menuBar->addMenu(mnuEdit);
    menuBar->addMenu(mnuOptions);
    menuBar->addSeparator();
    menuBar->addMenu( mnuHelp );

    lblMessages=new QLabel(tr("Messages:"));
    lblMessages->setAlignment(Qt::AlignHCenter);
    txtMessages=new QTextEdit();
    txtMessages->setReadOnly(TRUE);
    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
#ifndef MAC_SYSTEM
    layout->addWidget(menuBar);
#endif
    layout->addWidget(lblMessages);
    layout->addWidget(txtMessages);
    setLayout(layout);
    resize(LastSize_FormConsole);
//qDebug() << "Console_Konstruktor=" << this->size() << "LastSize_FormConsole=" << LastSize_FormConsole;
}

frmConsole::~frmConsole()
{
    LastSize_FormConsole=this->size();
}

void frmConsole::CreateActions(void)
{
    actSave=new QAction(tr("&Save..."),this);
    actSave->setShortcut(tr("Ctrl+S"));
    actSave->setStatusTip(tr("Save content to file"));
    connect(actSave,SIGNAL(triggered()), this, SLOT(doSave()));
    actClose=new QAction(tr("&Close"),this);
    actClose->setShortcut(tr("Esc"));
    actClose->setStatusTip(tr("Close console"));
    connect(actClose,SIGNAL(triggered()), this, SLOT(doClose()));
    actHelpOnContext=new QAction(tr("On Conte&xt"),this);
    actHelpOnContext->setShortcut(tr("Shift+F1"));
    connect(actHelpOnContext,SIGNAL(triggered()), this, SLOT(doHelpOnContext()));
    actHelpOnConsole=new QAction(tr("On &console"),this);
    connect(actHelpOnConsole,SIGNAL(triggered()), this, SLOT(doHelpOnConsole()));
    actPopOnError=new QAction(tr("Popup only on &errors"),this);
    actPopOnError->setCheckable(TRUE);
    actPopOnError->setChecked(FALSE);
    connect(actPopOnError,SIGNAL(triggered()), this, SLOT(doPopOnError()));
    actClear=new QAction(tr("&Clear"),this);
    connect(actClear,SIGNAL(triggered()), this, SLOT(doClear()));
    actCopy=new QAction(tr("Copy"),this);
    connect(actCopy,SIGNAL(triggered()), this, SLOT(doCopy()));
}

void frmConsole::errwin(const char *msg)
{
    txtMessages->append(tr("[Error] ")+msg);
    show();
    raise();
}

void frmConsole::msgwin(const char *msg)
{
    txtMessages->append(msg);
    show();
    raise();
}

void frmConsole::doSave(void)
{
    QString text=txtMessages->toPlainText();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Messages"),"/",tr("Text files (*.txt)"));
    if (!fileName.isNull())
    {
        ofstream ofi;
        ofi.open(fileName.toLocal8Bit());
        ofi << text.toLocal8Bit().data() << endl;
        ofi.close();
    }
}

void frmConsole::doClose(void)
{
    hide();
}

void frmConsole::doCopy(void)
{
    txtMessages->copy();
}

void frmConsole::doClear(void)
{
    txtMessages->clear();
}

void frmConsole::doHelpOnContext(void)
{
    mainWin->setCursor(QCursor(Qt::WhatsThisCursor));
}

void frmConsole::doHelpOnConsole(void)
{
    HelpCB("doc/UsersGuide.html#console");
}

void frmConsole::doPopOnError(void)
{
    ;
}

void frmConsole::resizeEvent(QResizeEvent * event)
{
//qDebug() << "Console_ResizeEvent=" << event->size();
QDialog::resizeEvent(event);
}


GlyphPanel::GlyphPanel(QWidget * parent):QLabel(parent)
{
    number=-1;
    marked=false;
    pix=new QPixmap(25*toolBarSizeFactor,25*toolBarSizeFactor);
    pix->fill(QApplication::palette().window().color());
    setPixmap(*pix);
    setFrameStyle(QFrame::Box | QFrame::Raised);
    setMinimumSize(25*toolBarSizeFactor,25*toolBarSizeFactor);
    setMaximumSize(32*toolBarSizeFactor,32*toolBarSizeFactor);
}

void GlyphPanel::setMarked(bool mark)
{
    marked=mark;
    if (mark==true)
    {
        setFrameStyle(QFrame::WinPanel | QFrame::Raised);
    }
    else
    {
        setFrameStyle(QFrame::Box | QFrame::Raised);
    }
}

void GlyphPanel::setCurrentPixmap(int font_nr,int char_nr)
{
    number=char_nr;
    font=font_nr;

    if (useQtFonts!=true)
    {
        *pix=DrawCB((unsigned char)number,font,valid_char);
    }
    else
    {
        pix=new QPixmap(25*toolBarSizeFactor,25*toolBarSizeFactor);
        pix->fill(QApplication::palette().window().color());
        QChar car(char_nr);
        QPainter paint1;
        QFont dfont=getFontFromDatabase(font_nr);
        dfont.setPixelSize(15*toolBarSizeFactor);
        paint1.begin(pix);
        paint1.setFont(dfont);
        paint1.drawText(5*toolBarSizeFactor,20*toolBarSizeFactor,QString(car));
        paint1.end();
        valid_char=true;
    }

    setPixmap(*pix);
}

void GlyphPanel::mousePressEvent(QMouseEvent *event)
{
    (void)event;
    emit(panelClicked(number));
}

QPixmap GlyphPanel::DrawCB(unsigned char c,int FontID,bool & valid_char)
{
    float Size = 16.8*toolBarSizeFactor;//who knows where this value comes from?
    BBox bbox;
    GLYPH *glyph;
    int height, width;// hshift, vshift;
    QPixmap tmpBitmap;
    QColor bg;
    QPainter paint1;
    QPainter paint2;
    height=width=0;

    bg=QApplication::palette().window().color();

    if (FontID == BAD_FONT_ID) {
        glyph = NULL;
    } else {
        glyph = T1_SetChar(FontID, c, Size, &UNITY_MATRIX);
    }

    bbox = T1_GetFontBBox(FontID);
    /* check if bbox is zero or invalid and then calculate it ourselves */
    if (bbox.llx >= bbox.urx || bbox.lly >= bbox.ury) {
        int c;
        memset(&bbox, 0, sizeof(bbox));
        for (c = 0; c < 256; c++) {
            BBox bbox_tmp = T1_GetCharBBox(FontID, c);
            bbox.llx = MIN2(bbox.llx, bbox_tmp.llx);
            bbox.lly = MIN2(bbox.lly, bbox_tmp.lly);
            bbox.urx = MAX2(bbox.urx, bbox_tmp.urx);
            bbox.ury = MAX2(bbox.ury, bbox_tmp.ury);
        }
    }

    if (glyph != NULL && glyph->bits != NULL) {
        valid_char = TRUE;
        height = glyph->metrics.ascent - glyph->metrics.descent;
        width = glyph->metrics.rightSideBearing - glyph->metrics.leftSideBearing;
        /*hshift = MAX2(glyph->metrics.leftSideBearing - bbox.llx, 0);
        vshift = MAX2(bbox.ury - glyph->metrics.ascent, 0);*/

        /*XtVaGetValues(w, XmNbackground, &bg, XmNforeground, &fg, NULL);
     XSetForeground(disp, gc, bg);*/

        tmpBitmap=QPixmap(width,height);
        tmpBitmap.fill(bg);
        paint2.begin(&tmpBitmap);
        char kkk;
        char s;
        int c_counter;
        int nr_of_bytes= 8*(width/8)==width ? width/8 : width/8+1;
        for (int jjj=0;jjj<height;jjj++)
        {
            c_counter=0;
            for (int iii=0;iii<nr_of_bytes;iii++)//bytes
            {
                kkk=glyph->bits[jjj*nr_of_bytes+iii];
                for (int lll=0;lll<8;lll++)//8bits per byte
                {
                    s=1<<lll;
                    if (kkk&s)
                        paint2.drawPoint(c_counter,jjj);
                    c_counter++;
                    if (c_counter>=width) break;
                }
            }
        }
        paint2.end();

        /*ptmp = XCreateBitmapFromData(disp, root,(char *) glyph->bits, width, height);
     XSetBackground(disp, gc, bg);
        pixmap = XCreatePixmap(disp, root, bbox.urx - bbox.llx, bbox.ury - bbox.lly, depth);
        XFillRectangle(disp, pixmap, gc, 0, 0, bbox.urx - bbox.llx, bbox.ury - bbox.lly);
     XSetForeground(disp, gc, fg);
        XCopyPlane(disp, ptmp, pixmap, gc, 0, 0, width, height, hshift, vshift, 1);
        XFreePixmap(disp, ptmp);*/

    } else {
        if (c == ' ') {
            valid_char = TRUE;
        } else {
            valid_char = FALSE;
        }
        //        pixmap =  XCreateBitmapFromData(disp, root,dummy_bits, 1, 1);
    }
    if (width<=0)
        width=25;
    if (height<=0)
        height=25;

    QPixmap pixmap(25*toolBarSizeFactor,25*toolBarSizeFactor);
    pixmap.fill(bg);

    if (glyph != NULL && glyph->bits != NULL)
    {
        paint1.begin(&pixmap);
        paint1.drawPixmap((25*toolBarSizeFactor-width)/2+glyph->metrics.leftSideBearing,(25*toolBarSizeFactor-height)/2-glyph->metrics.descent,QPixmap(tmpBitmap));
        paint1.end();
    }

    /* Assign it a pixmap */
    //    cbs->pixmap = pixmap;
    //    cbs->type = XbaePixmap;
    //    XbaeMatrixSetCellUserData(w, cbs->row, cbs->column, (XtPointer) valid_char);
    return pixmap;
}

frmFontTool::frmFontTool(QWidget * parent):QDialog(parent)
{
    //0x152f   = 338 * 16 = 5423
//setFont(*stdFont);
    setWindowTitle(tr("QtGrace: Font tool"));
    setWindowIcon(QIcon(*GraceIcon));

    selFont=new FontSelector(this);
    scroll=new QScrollArea(this);
    background=new QWidget(scroll);
    layout0=new QGridLayout;
    //layout0->setMargin(0);
    layout0->setContentsMargins(0,0,0,0);
    layout0->setSpacing(0);
    int r=0,c=0;
    tileCount=16*338;//we always generate all glyph-panels
    for (int i=0;i<tileCount;i++)
    {
        panel[i]=new GlyphPanel(background);
        panel[i]->number=i;
        layout0->addWidget(panel[i],r,c);
        c++;
        if (c>=16)
        {
            r++;
            c=0;
        }
        connect(panel[i],SIGNAL(panelClicked(int)),SLOT(newClick(int)));
    }
    tileCount=16*16;
    if (useQtFonts==true)
        tileCount=16*338;
    background->setLayout(layout0);
    scroll->setWidget(background);

    ledString=new stdLineEdit(this,tr("CString:"));
    connect(ledString->lenText,SIGNAL(textEdited(QString)),SLOT(characterInserted(QString)));

    ledAscii=new stdLineEdit(this,tr("Ascii:"));
    ledAscii->setText("");
    ledAscii->lenText->setEnabled(false);
    ledUnicode=new stdLineEdit(this,tr("Unicode:"));
    ledUnicode->setText("");
    ledUnicode->lenText->setEnabled(false);

    /*cmdUseSymbolValue=new QPushButton(tr("Use ascii-value for symbol-character"),this);
    cmdUseSymbolValue->setToolTip(tr("Transfer the ascii-value of the last selected character\nas symbol to the current set-appearance.\nThis dialog is closed afterwards."));
    connect(cmdUseSymbolValue,SIGNAL(clicked(bool)),SLOT(doUseSymbolValue()));*/

    buttonGroup=new stdButtonGroup(this,true,true,false);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));

    layout=new QGridLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->setSpacing(STD_SPACING);
    layout->addWidget(selFont,0,0,1,2);
    layout->addWidget(scroll,1,0,2,2);
    layout->addWidget(ledString,3,0,1,2);
    layout->addWidget(ledAscii,4,0,1,1);
    layout->addWidget(ledUnicode,4,1,1,1);
    //layout->addWidget(cmdUseSymbolValue,4,2,1,1);
    layout->addWidget(buttonGroup,5,0,1,2);

        for (int i=0;i<6;i++)
        layout->setRowStretch(i,1);
        layout->setRowStretch(1,30);
        layout->setRowStretch(2,30);

    setLayout(layout);
    connect(selFont->cmbFontSelect,SIGNAL(currentIndexChanged(int)),SLOT(FontChanged(int)));
    FontChanged(0);
    marked=-1;
    ledString->setText(QString(""));
    textString=QString("");
    resize(LastSize_FormFontTool);
}

frmFontTool::~frmFontTool()
{
    LastSize_FormFontTool=this->size();
}

void frmFontTool::FontChanged(int i)
{
    if (i<0) return;

    if (useQtFonts==false)
    {
        tileCount=16*16;
        for (int j=0;j<tileCount;j++)
            panel[j]->setCurrentPixmap(i,j);
        for (int j=tileCount;j<16*338;j++)
            panel[j]->setVisible(false);
        background->resize(QSize(16*panel[0]->width(),16*panel[0]->height()));
    }
    else
    {
        tileCount=16*338;
        QPainter paint1;
        QFont dfont=getFontFromDatabase(i);
        QChar car;
        dfont.setPixelSize(15*toolBarSizeFactor);
        car=QChar(0);
        panel[0]->valid_char=car.isPrint();
        paint1.begin(panel[0]->pix);
        panel[0]->pix->fill(QApplication::palette().window().color());
        paint1.setFont(dfont);
        paint1.setPen(Qt::black);
        paint1.drawText(5*toolBarSizeFactor,20*toolBarSizeFactor,QString(car));
        paint1.end();
        panel[0]->number=0;
        panel[0]->setPixmap(*(panel[0]->pix));
        panel[0]->valid_char=false;
        for (int j=1;j<tileCount;j++)
        {
                if (i==get_QtFontID_from_Grace_Name("Symbol",0) && symbol_font_is_special==true)
                car=QChar(key_for_greek.value(j,j));//new_ascii=key_for_greek.value(ascii,ascii);
                else
                car=QChar(j);
            paint1.begin(panel[j]->pix);
            panel[j]->pix->fill(QApplication::palette().window().color());
            paint1.setFont(dfont);
            paint1.drawText(5*toolBarSizeFactor,20*toolBarSizeFactor,QString(car));
            paint1.end();
            panel[j]->setPixmap(*(panel[j]->pix));
            panel[j]->valid_char=car.isPrint();
            panel[j]->number=j;
            panel[j]->setVisible(true);
        }
        background->resize(QSize(16*panel[0]->width(),338*panel[0]->height()));
    }
    layout0->update();
    char * buf;
    buf = copy_string(NULL, "\\f{");
    buf = concat_strings(buf, get_fontalias(i));
    buf = concat_strings(buf, "}");
    textString=ledString->text();
    QString charText=QString(buf);
    insertAtCursor(charText);
    xfree(buf);
}

void frmFontTool::newClick(int i)
{
    QString charText;
    unsigned char c;
    char s[7];
    //cout << "marked=" << marked << " clicked=" << i << endl;
    if (marked>=0 && marked<16*338)
    {
        panel[marked]->setMarked(false);
    }
    marked=i;
    if (marked>=0 && marked<16*338)
    {
        panel[marked]->setMarked(true);
    }
    last_character=i;
    /// cout << "char-number=" << i << endl;
    if (panel[i]->valid_char==false)
    {
        QApplication::beep();
    }
    else if (useQtFonts==true)
    {
        textString=ledString->text();
        charText=QString(QChar(i));
        //ledString->lenText->insert(charText);
        insertAtCursor(charText);
        return;
        //char t2(i);
        //charText=FileCodec->toUnicode(&t2);
        /// cout << i << " #" << charText.toLocal8Bit().constData() << "# " << sizeof(char) << endl;
        //insertAtCursor(charText);
    }
    else
    {
        textString=ledString->text();
        c = panel[i]->number;
        /* TODO: check for c being displayable in the _X_ font */
        if (c > 31)
        {
            s[0] = (char) c;
            s[1] = '\0';
        }
        else
        {
            sprintf(s, "\\#{%02x}", c);
        }
        if (c<128)
        {
            charText=QString(s);
        }
        else
        {
            charText=QString("\\c")+QString((char)(c-128))+QString("\\C");
        }
        insertAtCursor(charText);
    }
}

void frmFontTool::doUseSymbolValue(void)
{
int val=atoi(ledAscii->text().toLocal8Bit().constData());

//if (!QChar::isPrint(val))
if (!isprint(val))
{
errmsg(tr("No printable ascii-character selected.").toLocal8Bit().constData());
return;
}
if (FormSetAppearance==NULL)
{
errmsg(tr("No set appearance dialog opened.").toLocal8Bit().constData());
return;
}
else if (FormSetAppearance->isVisible()==false)
{
errmsg(tr("No set appearance dialog opened.").toLocal8Bit().constData());
return;
}

FormSetAppearance->flp->tabMa->selSymbChar->setCurrentValue(val);
//FormSetAppearance->flp->tabMa->ledSymbChar->setText(ledAscii->text());
FormSetAppearance->flp->tabMa->cmbSymbType->setCurrentIndex(SYM_CHAR);

//qDebug() << "val=" << val << " last_char=" << last_character << " printable=" << QChar::isPrint(last_character);

hide();
FormSetAppearance->show();
FormSetAppearance->raise();
FormSetAppearance->activateWindow();
FormSetAppearance->setFocus();
}

void frmFontTool::insertAtCursor(QString c)
{
    //if(last_character>=0 && last_character<256)
    ledAscii->setText(QString::number(last_character));
    /*else
    ledAscii->setText(QString("-"));*/
    ledUnicode->setText(QString::number(last_character));

    int position=ledString->lenText->cursorPosition();
    textString=textString.insert(position,c);
    ledString->lenText->setText(textString);
    ledString->lenText->setCursorPosition(position+c.length());
}

void frmFontTool::characterInserted(QString text)
{
    int i;
    char c='\0';
    int len1=textString.length(),len2=text.length();
    int minlength= len1<len2 ? len1 : len2;
    if (text.length()>0)
    {
    QChar l_c=text.at(text.length()-1);
    ledUnicode->setText(QString::number(l_c.unicode()));
    if (l_c.unicode()<256) ledAscii->setText(QString::number(l_c.unicode()));
    else ledAscii->setText(QString("-"));
    }
    for (i=0;i<minlength;i++)
    {
        if (text.at(i)!=textString.at(i)) break;
    }
    if (i==minlength)
    {//the new character is the last character
        if (len1>len2)
        {
            c=textString.at(i).toLatin1();
        }
        else
        {
            c=text.at(i).toLatin1();
        }
    }
    else
    {
        c=text.at(i).toLatin1();
    }
    textString=text;
    /*if (c>=0 && c<256)//Ascii-Character
        {*/
    if (marked>=0 && marked<16*338)
    {
        panel[marked]->setMarked(false);
    }
    marked=(int) c;
    if (marked>=0 && marked<16*16)
    {
        panel[marked]->setMarked(true);
    }
    last_character=marked;
    /*}*/
}

void frmFontTool::doApply(void)
{
    /* I don't know what this widget should do when Apply is pressed!??? */
}

void frmFontTool::doClose(void)
{
    hide();
}

void frmFontTool::doAccept(void)
{
    doApply();
    doClose();
}

void frmFontTool::updateGlyphSize(void)
{
    for (int i=0;i<tileCount;i++)
    {
    panel[i]->setMinimumSize(25*toolBarSizeFactor,25*toolBarSizeFactor);
    panel[i]->setMaximumSize(32*toolBarSizeFactor,32*toolBarSizeFactor);
    }
    FontChanged(selFont->currentIndex());
}


frmRealTimeInputManager::frmRealTimeInputManager(QWidget * parent):QDialog(parent)
{
//setFont(*stdFont);
    setWindowTitle(tr("QtGrace: Real Time Input"));
    setWindowIcon(QIcon(*GraceIcon));
    frmOpen=NULL;
    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->setSpacing(STD_SPACING);

    selTimeout=new stdIntSelector(this,tr("Update interval (ms):"),10,10000);
    selTimeout->setValue(rtiCheckTime);
    selTimeout->spnInt->setSingleStep(100);
    lstInputs=new uniList(TEXTLIST,this);

    lblRTI=new QLabel(tr("Registered real time input pipes"),this);
    chkRTIactive=new QCheckBox(tr("Real time input active"),this);
    connect(chkRTIactive,SIGNAL(toggled(bool)),SLOT(toggle_RTI(bool)));
    aac=new stdButtonGroup(this,true,true,false);
    aac->cmdApply->setText(tr("Open pipe"));
    aac->cmdAccept->setText(tr("Close pipe"));
    connect(aac->cmdApply,SIGNAL(clicked()),SLOT(doOpen()));
    connect(aac->cmdAccept,SIGNAL(clicked()),SLOT(doDelete()));
    connect(aac->cmdClose,SIGNAL(clicked()),SLOT(doClose()));
    layout->addWidget(lblRTI);
    layout->addWidget(lstInputs);
    layout->addWidget(chkRTIactive);
    layout->addWidget(selTimeout);
    layout->addWidget(aac);
    setLayout(layout);
    resize(LastSize_FormRTIManage);
}

frmRealTimeInputManager::~frmRealTimeInputManager()
{
    LastSize_FormRTIManage=this->size();
}

void frmRealTimeInputManager::updateRTIs(void)
{
    lstInputs->clear();
    //nb_rt   (ib_tblsize)   ib_tbl
    for (int i=0;i<nb_rt;i++)
    {
        if (ib_tbl[i].name!=NULL)
        {
            lstInputs->add_Item(QString(ib_tbl[i].name));
        }
    }
    int index=0;
    for (int i=0;i<nb_rt;i++)
    {
        if (ib_tbl[i].name!=NULL)
        {
            lstInputs->entries[index++]=i;
        }
    }

    selTimeout->setValue(rtiCheckTime);
}

void frmRealTimeInputManager::toggle_RTI(bool s)
{
    mainWin->rtiTimer->stop();//we stop in any case
    rtiCheckTime=selTimeout->value();
    if (s==true)
    {
        mainWin->rtiTimer->start(rtiCheckTime);
    }
}

void frmRealTimeInputManager::init(void)
{
    updateRTIs();
}

void frmRealTimeInputManager::doOpen(void)
{
    int fd;
    char * dummy;
    if (frmOpen==NULL)
    {
        frmOpen=new QFileDialog(this);
    }
    QString fname=frmOpen->getOpenFileName(this,tr("Open pipe"));
    if (fname.isEmpty())
    {
        return;
    }
    dummy=new char[fname.size()+2];
    strcpy(dummy,fname.toLocal8Bit().constData());
    if (openPipe(dummy,&fd)!=RETURN_SUCCESS)
    {
        errmsg(tr("Unable to open pipe!").toLocal8Bit().constData());
    }
    updateRTIs();
    delete[] dummy;
}

void frmRealTimeInputManager::doDelete(void)
{
    int nr_of_selected_elements,*el_nr=new int[2];
    lstInputs->get_selection(&nr_of_selected_elements,&el_nr);
    for (int i=nr_of_selected_elements-1;i>=0;i--)
    {
        unregister_real_time_input(ib_tbl[el_nr[i]].name);
    }
    delete[] el_nr;
}

void frmRealTimeInputManager::doClose(void)
{
    hide();
}
