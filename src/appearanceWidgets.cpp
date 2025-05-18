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

#include "appearanceWidgets.h"

#define cg get_cg()

extern frmPlotAppearance * FormPlotAppearance;
extern frmSetAppearance * FormSetAppearance;
extern frmGraphApp * FormGraphAppearance;
extern frmAxisProp * FormAxisProperties;
extern frmCreateAltAxis * FormCreateAltAxis;
extern frmFontTool * FormFontTool;
extern QSize LastSize_FormSetAppearance;
extern QSize LastSize_FormPlotAppearance;
extern QSize LastSize_FormGraphAppearance;
extern QSize LastSize_FormAxisProperties;
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
extern imageinfo bg_fill_image;
extern imageinfo bg_box_fill_image;
extern imageinfo bg_ellipse_fill_image;
extern QPixmap * GraceIcon;
extern OptionItem fmt_option_items[];
extern int nr_of_set_concerned;
extern int * sets_concerned_g;
extern int * sets_concerned_s;
extern char dummy[];
extern QString Last_Dialog_Path[NR_OF_FILE_DIALOGS];

extern void parse_qtGrace_Additions(char * s);
extern int nr_of_true_changes(QStringList & list);
extern void strToUpper(char * tar,const char * ch);
extern void createSymbolIcons(QIcon ** symb_icons);
extern void update_grace_string_from_UTF8_static(char * string);
extern int should_open_grace_file_dialog(void);
extern void SetDecimalSeparatorToUserValue(char * str,bool remove_space=true);
extern void ReplaceDecimalSeparator(char * str,bool remove_space=true);
extern void RedisplayString(char * str);
extern void PrepareFormula(char * str);
extern void RedisplayFormula(char * str);
extern void sort(int number,int * items);
extern void Replace_Dec_Sep_In_Single_String(QString & te);

#ifdef __cplusplus
extern "C" {
#endif
extern int qtspecial_scanner(char * command, int * errors);
extern void prepare_strings_for_saving(void);
extern void resume_strings_after_load_or_save(void);
extern double rint_2(double v);
#ifdef __cplusplus
}
#endif

frmPlot_Appearance::frmPlot_Appearance(QWidget * parent):QWidget(parent)
{
int number;
QString entries[8];
    //setFont(*stdFont);
    //setWindowTitle(tr("QtGrace: Plot appearance"));
    //setWindowIcon(QIcon(*GraceIcon));

    fraFont=new QGroupBox(tr("Font"),this);

    QLocale newLocale=(DecimalPointToUse=='.')?(*dot_locale):(*comma_locale);
    selFontSize=new LineWidthSelector(this);
    selFontSize->lblText->setText(tr("Global font size multiplicator:"));
    selFontSize->spnLineWidth->setRange(0.0,1000.0);
    selFontSize->spnLineWidth->setDecimals(2);
    selFontSize->spnLineWidth->setValue(100.0);
    selFontSize->spnLineWidth->setSingleStep(1.0);
    selFontSize->spnLineWidth->setSuffix(tr(" %"));
    selFontSize->setLocale(newLocale);

    layout4=new QVBoxLayout;
    layout4->setSpacing(STD_SPACING);
    //layout4->setMargin(STD_MARGIN);
    layout4->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout4->addWidget(selFontSize);
    fraFont->setLayout(layout4);

    fraPageBackgr=new QGroupBox(tr("Page background"),this);
    bg_color_item=new ColorSelector(fraPageBackgr);
    bg_color_item->setCurrentIndex(0);

    /*bg_fill_item=new QCheckBox(tr("Fill"),fraPageBackgr);
    bg_fill_item->setChecked(TRUE);*/
    number=3;
    entries[0]=tr("transparent");
    entries[1]=tr("solid fill");
    entries[2]=tr("image");
    selFill=new StdSelector(this,tr("Background:"),number,entries);
    imgSelect=new ImageSelector(this);
    imgSelect->setVisible(false);

    layout1=new QGridLayout;
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout1->setSpacing(STD_SPACING);
    layout1->addWidget(bg_color_item,1,0,1,2);
    //layout1->addWidget(bg_fill_item,0,2,1,1);
    layout1->addWidget(selFill,0,0,1,2);
    layout1->addWidget(imgSelect,2,0,1,2);
    fraPageBackgr->setLayout(layout1);

    fraTimeStamp=new QGroupBox(tr("Time stamp"),this);
    timestamp_active_item=new QCheckBox(tr("Show time stamp"),fraTimeStamp);
    timestamp_active_item->setChecked(FALSE);
    chkPathStamp=new QCheckBox(tr("Show file path"),fraTimeStamp);
    chkPathStamp->setChecked(FALSE);
    timestamp_font_item=new FontSelector(fraTimeStamp);
    timestamp_color_item=new ColorSelector(fraTimeStamp);
    timestamp_color_item->setCurrentIndex(1);
    timestamp_size_item=new stdSlider(fraTimeStamp,tr("Character size"),0,1000);
    timestamp_size_item->setValue(100);
    timestamp_rotate_item=new stdSlider(fraTimeStamp,tr("Angle"),0,360);
    timestamp_rotate_item->setValue(0);
    ledStampCoords[0]=new stdLineEdit(fraTimeStamp,tr("Timestamp X:"));
    ledStampCoords[0]->setText(QString("0.03"));
    ledStampCoords[1]=new stdLineEdit(fraTimeStamp,tr("Timestamp Y:"));
    ledStampCoords[1]->setText(QString("0.03"));
    layout2=new QVBoxLayout;
    layout2->setSpacing(STD_SPACING);
    //layout2->setMargin(STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout2->addWidget(timestamp_active_item);
    layout2->addWidget(chkPathStamp);
    layout2->addWidget(timestamp_font_item);
    layout2->addWidget(timestamp_color_item);
    layout2->addWidget(timestamp_size_item);
    layout2->addWidget(timestamp_rotate_item);
    layout2->addWidget(ledStampCoords[0]);
    layout2->addWidget(ledStampCoords[1]);
    fraTimeStamp->setLayout(layout2);

    fraDescription=new QGroupBox(tr("Project description"),this);
    layout3=new QVBoxLayout;
    layout3->setSpacing(STD_SPACING);
    //layout3->setMargin(STD_MARGIN);
    layout3->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    txtDescription=new QTextEdit(fraDescription);
    layout3->addWidget(txtDescription);
    fraDescription->setLayout(layout3);

    buttonGroup=new stdButtonGroup(this);
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));

    connect(timestamp_rotate_item,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(timestamp_size_item,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    //connect(bg_fill_item,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(selFill,SIGNAL(currentIndexChanged(int)),SLOT(bg_fill_changed(int)));
    connect(timestamp_active_item,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(chkPathStamp,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(bg_color_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(bg_color_item,SIGNAL(alphaChanged(int)),SLOT(update1(int)));
    connect(timestamp_color_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(timestamp_color_item,SIGNAL(alphaChanged(int)),SLOT(update1(int)));
    connect(timestamp_font_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(ledStampCoords[0],SIGNAL(changed()),SLOT(update0()));
    connect(ledStampCoords[1],SIGNAL(changed()),SLOT(update0()));
    connect(selFontSize,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(imgSelect,SIGNAL(someChanges()),SLOT(update0()));

    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->setSpacing(STD_SPACING);
    layout->addWidget(fraPageBackgr);
    layout->addWidget(fraFont);
    layout->addWidget(fraTimeStamp);
    layout->addWidget(fraDescription);
    layout->addWidget(buttonGroup);
    layout->setStretch(0,0);
    layout->setStretch(1,1);
    layout->setStretch(2,0);
    setLayout(layout);
}

frmPlot_Appearance::~frmPlot_Appearance()
{
}

void frmPlot_Appearance::update0(void)
{
    static int i,nr;//,errpos;
    bool imm_upd_sav;
    if (!immediateUpdate) return;
    imm_upd_sav=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    GlobalInhibitor=true;
    SavePlotAppearance();
    doApply();//do this without actually doing something
    nr=nr_of_true_changes(ListOfChanges);
    //errpos=0;
    /*cout << "nr=" << nr << endl;
for (i=0;i<ListOfChanges.size();i++)
cout << ListOfChanges.at(i).toLocal8Bit().constData() << endl;*/
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
    PlotAppearanceModified();
    ListOfChanges.clear();
    ListOfOldStates.clear();
    GlobalInhibitor=false;
    immediateUpdate=imm_upd_sav;
    updateRunning=false;
}

void frmPlot_Appearance::update1(int v)
{
    (void)v;
    update0();
}

void frmPlot_Appearance::update2(QString v)
{
    (void)v;
    update0();
}

void frmPlot_Appearance::update3(bool v)
{
    (void)v;
    update0();
}

void frmPlot_Appearance::update4(double v)
{
    (void)v;
    update0();
}

void frmPlot_Appearance::doApply(void)
{
    char dummy[MAX_STRING_LENGTH];
    double ddummy1,ddummy2;
    ApplyError=false;
    QString strDescr=txtDescription->toPlainText();
    int descrLen=strDescr.length();
    char * nDescr=new char[2+descrLen];
    char * oDescr=get_project_description();
    imageinfo tmp_image_info;
    QString tmp_info_string;
    imgSelect->readImageSettings(tmp_image_info);
    strcpy(nDescr,strDescr.toLocal8Bit().constData());
    ///Undo-Stuff
    if (!GlobalInhibitor)
    {
    qDebug() << "SavePlotAppearance";
    SavePlotAppearance();
    }

    ListOfChanges.clear();
    ListOfOldStates.clear();

    if (GetOptionChoice(bg_color_item)!=getbgcolor())
    {
        sprintf(dummy,"background color %d",GetOptionChoice(bg_color_item));
        ListOfChanges << QString(dummy);
        sprintf(dummy,"background color %d",getbgcolor());
        ListOfOldStates << QString(dummy);
    }
    /// ToDo: image-fill != "fill on"
    if (selFill->currentIndex()!=getbgfill())
    {
        sprintf(dummy,"page background fill %s",selFill->currentIndex()?"on":"off");
        ListOfChanges << QString(dummy);
        sprintf(dummy,"page background fill %s",getbgfill()?"on":"off");
        ListOfOldStates << QString(dummy);
    }
    if (timestamp.active != GetToggleButtonState(timestamp_active_item))
    {
        sprintf(dummy,"timestamp %s",GetToggleButtonState(timestamp_active_item)?"on":"off");
        ListOfChanges << QString(dummy);
        sprintf(dummy,"timestamp %s",timestamp.active?"on":"off");
        ListOfOldStates << QString(dummy);
    }
    if (timestamp.font != GetOptionChoice(timestamp_font_item))
    {
        sprintf(dummy,"timestamp font %d",GetOptionChoice(timestamp_font_item));
        ListOfChanges << QString(dummy);
        sprintf(dummy,"timestamp font %d",timestamp.font);
        ListOfOldStates << QString(dummy);
    }
    if (timestamp.color != GetOptionChoice(timestamp_color_item))
    {
        sprintf(dummy,"timestamp color %d",GetOptionChoice(timestamp_color_item));
        ListOfChanges << QString(dummy);
        sprintf(dummy,"timestamp color %d",timestamp.color);
        ListOfOldStates << QString(dummy);
    }
    if (timestamp.charsize != GetCharSizeChoice(timestamp_size_item)/100.0)
    {
        sprintf(dummy,"timestamp char size %f",GetCharSizeChoice(timestamp_size_item)/100.0);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"timestamp char size %f",timestamp.charsize);
        ListOfOldStates << QString(dummy);
    }
    if (timestamp.rot != GetAngleChoice(timestamp_rotate_item))
    {
        sprintf(dummy,"timestamp rot %d",(int)GetAngleChoice(timestamp_rotate_item));
        ListOfChanges << QString(dummy);
        sprintf(dummy,"timestamp rot %d",timestamp.rot);
        ListOfOldStates << QString(dummy);
    }
    xv_evalexpr(ledStampCoords[0], &ddummy1);
    xv_evalexpr(ledStampCoords[1], &ddummy2);
    if (timestamp.x!=ddummy1 || timestamp.y!=ddummy2)
    {
        sprintf(dummy,"timestamp %f, %f",ddummy1,ddummy2);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"timestamp %f, %f",timestamp.x,timestamp.y);
        ListOfOldStates << QString(dummy);
    }
    if (!compare_strings(nDescr,oDescr))
    {
        sprintf(dummy,"description \"%s\"",nDescr);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"description \"%s\"",oDescr);
        ListOfOldStates << QString(dummy);
    }
    if (timestamp.alpha!=timestamp_color_item->alpha() || getbgalpha()!=bg_color_item->alpha())
    {
        sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: PLOT_ALPHA %d %d\n",bg_color_item->alpha(),timestamp_color_item->alpha());
        ListOfChanges << QString(dummy);
        sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: PLOT_ALPHA %d %d\n",getbgalpha(),timestamp.alpha);
        ListOfOldStates << QString(dummy);
    }
    if (tmp_image_info.imgpath!=bg_fill_image.imgpath || tmp_image_info.imgtransform!=bg_fill_image.imgtransform || tmp_image_info.relpath!=bg_fill_image.relpath)
    {
    sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: PLOT_IMAGE %d ",selFill->currentIndex());
    createImageInfoText(tmp_image_info,tmp_info_string,QString());
    ListOfChanges << QString(dummy)+tmp_info_string+QString("\n");
    createImageInfoText(bg_fill_image,tmp_info_string,QString());
    ListOfOldStates << QString(dummy)+tmp_info_string+QString("\n");
    }

    if (!GlobalInhibitor)
    {
        universal_font_size_factor=selFontSize->value()*0.01;
        setbgcolor(GetOptionChoice(bg_color_item));
        memcpy(&bg_fill_image,&tmp_image_info,sizeof(imageinfo));
        /// setbgfill(GetToggleButtonState(bg_fill_item));
        setbgfill(selFill->currentIndex());
        setbgalpha(bg_color_item->alpha());
        timestamp.active = GetToggleButtonState(timestamp_active_item);
        timestamp.path = GetToggleButtonState(chkPathStamp);
        timestamp.font = GetOptionChoice(timestamp_font_item);
        timestamp.color = GetOptionChoice(timestamp_color_item);
        timestamp.alpha = timestamp_color_item->alpha();
        timestamp.charsize = GetCharSizeChoice(timestamp_size_item)/100.0;
        timestamp.rot = GetAngleChoice(timestamp_rotate_item);
        xv_evalexpr(ledStampCoords[0], &timestamp.x);
        xv_evalexpr(ledStampCoords[1], &timestamp.y);
        set_project_description(nDescr);
        ///Undo-Stuff
        qDebug() << "PlotAppearanceModified";
        PlotAppearanceModified();
        set_dirtystate();
        mainWin->mainArea->completeRedraw();
        ListOfChanges.clear();
        ListOfOldStates.clear();
    }
    /*for (int i=0;i<ListOfChanges.size();i++)
cout << ListOfChanges.at(i).toLocal8Bit().constData() << endl;*/
}

void frmPlot_Appearance::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmPlot_Appearance::doClose(void)
{
//parentWidget()->hide();
    emit(closeWish());
}

void frmPlot_Appearance::bg_fill_changed(int a)
{
    (void) a;
if (selFill->currentIndex()==0)//transparent
{
bg_color_item->setVisible(false);
imgSelect->setVisible(false);
}
else if (selFill->currentIndex()==1)//solid fill
{
bg_color_item->setVisible(true);
imgSelect->setVisible(false);
}
else//fill with an image
{
bg_color_item->setVisible(false);
imgSelect->setVisible(true);
}
}

void frmPlot_Appearance::init(void)
{
    SetOptionChoice(bg_color_item, getbgcolor());
    bg_color_item->setAlpha(getbgalpha());
    //SetToggleButtonState(bg_fill_item, getbgfill());
    imgSelect->initToImage(bg_fill_image);
    selFill->setCurrentIndex(getbgfill());
    SetToggleButtonState(timestamp_active_item, timestamp.active);
    SetToggleButtonState(chkPathStamp, timestamp.path);
    SetOptionChoice(timestamp_font_item, timestamp.font);
    SetOptionChoice(timestamp_color_item, timestamp.color);
    timestamp_color_item->setAlpha(timestamp.alpha);
    SetCharSizeChoice(timestamp_size_item, timestamp.charsize*100);
    SetAngleChoice(timestamp_rotate_item, timestamp.rot);
    char * oDescr=get_project_description();
    txtDescription->setText(QString(oDescr));
    ledStampCoords[0]->setDoubleValue("%g",timestamp.x);
    ledStampCoords[1]->setDoubleValue("%g",timestamp.y);
    selFontSize->setValue(universal_font_size_factor*100.0);
}

void frmPlot_Appearance::redisplayContents(void)
{
QLocale newLocale=(DecimalPointToUse=='.')?(*dot_locale):(*comma_locale);
/// Encoding of Project-description missing
ledStampCoords[0]->ReplaceNumberContents();
ledStampCoords[1]->ReplaceNumberContents();
selFontSize->setLocale(newLocale);
selFontSize->setValue(selFontSize->value());
layout->update();
layout1->update();
layout2->update();
layout3->update();
layout4->update();
}

frmPlotAppearance::frmPlotAppearance(QWidget * parent):QDialog(parent)
{
//setFont(*stdFont);
    min_w=326;
    min_h=549;
    bar_w=bar_h=20;
    setWindowTitle(tr("QtGrace: Plot appearance"));
    setWindowIcon(QIcon(*GraceIcon));
    QVBoxLayout * layout=new QVBoxLayout;
    //layout->setMargin(0);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    flp=new frmPlot_Appearance(this);
    connect(flp,SIGNAL(closeWish()),SLOT(doClose()));
    scroll=new QScrollArea;
    scroll->setWidget(flp);
    layout->addWidget(scroll);
    //layout->addWidget(flp);
    setLayout(layout);
    /// flp->buttonGroup->cmdAccept->setDefault(true); //deactivated --> apply is default (otherwise the editable slider behave strange)
    /// flp->buttonGroup->cmdAccept->setFocus();
    //resize(QSize(min_w,min_h));
    resize(LastSize_FormPlotAppearance);
}

frmPlotAppearance::~frmPlotAppearance()
{
    LastSize_FormPlotAppearance=this->size();
}

void frmPlotAppearance::init(void)
{
    flp->init();
}

void frmPlotAppearance::doApply(void)
{
    flp->doApply();
}

void frmPlotAppearance::doAccept(void)
{
    flp->doAccept();
}

void frmPlotAppearance::doClose(void)
{
    //flp->doClose();
    hide();
}

void frmPlotAppearance::resizeEvent(QResizeEvent * event)
{
//cout << "resize: " << event->size().width() << "x" << event->size().height() << " bar_w=" << bar_w << " bar_h=" << bar_h << endl;
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
this->setMinimumSize(0,0);
if (scroll->verticalScrollBarPolicy()==Qt::ScrollBarAlwaysOn) n_size_w-=bar_w;
if (scroll->horizontalScrollBarPolicy()==Qt::ScrollBarAlwaysOn) n_size_h-=bar_h;
}
else
{
this->setMinimumSize(min_w,min_h);
scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}
flp->resize(QSize(n_size_w,n_size_h));
}


tabMain::tabMain(QWidget * parent):QWidget(parent)
{
    int number=NUMBER_OF_SETTYPES;
    QString entr[NUMBER_OF_SETTYPES];

    char dummy[2000];
    fraSetPres=new QGroupBox(tr("Set presentation"),this);

    number_of_Type_entries=NUMBER_OF_SETTYPES;
    Type_entries=new int[NUMBER_OF_SETTYPES];
    for (int i=0;i<NUMBER_OF_SETTYPES;i++)
    {
        strToUpper(dummy,set_types(i));
        entr[i]=QString(dummy);
    }
    cmbType=new StdSelector(fraSetPres,tr("Type:"),number,entr);
    layout0=new QHBoxLayout;
    //layout0->setMargin(STD_MARGIN);
    layout0->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout0->addWidget(cmbType);
    fraSetPres->setLayout(layout0);

    fraSymbProp=new QGroupBox(tr("Symbol properties"),this);
    layout1=new QVBoxLayout;
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    entr[0]=tr("None");
    entr[1]=tr("Circle");
    entr[2]=tr("Square");
    entr[3]=tr("Diamond");
    entr[4]=tr("Triangle up");
    entr[5]=tr("Triangle left");
    entr[6]=tr("Triangle down");
    entr[7]=tr("Triangle right");
    entr[8]=tr("Plus");
    entr[9]=tr("X");
    entr[10]=tr("Star");
    entr[11]=tr("Char");
    number=12;
    cmbSymbType=new StdSelector(fraSymbProp,tr("Type:"),number,entr);
    layout1->addWidget(cmbSymbType);

    updateSymbolTypeIcons();

    sldSymbSize=new stdSlider(fraSymbProp,tr("Size"),0,1000);
    layout1->addWidget(sldSymbSize);
    cmbSymbColor=new ColorSelector(fraSymbProp);
    layout1->addWidget(cmbSymbColor);
    connect(cmbSymbColor,SIGNAL(currentIndexChanged(int)),SLOT(SymbColorChanged(int)));

    ///addition for v0.2.6
    int nr_of_char_entries=0;
    int char_entry_values[256];
    QString char_entries[256];
    for (int i=0;i<256;i++)
    {
        if (isprint(i))
        {
        char_entry_values[nr_of_char_entries]=i;
        char_entries[nr_of_char_entries]=QChar(i);
        nr_of_char_entries++;
        }
    }
    selSymbChar=new StdSelector(this,tr("Symbol char:"),nr_of_char_entries,char_entries);
    selSymbChar->setValues(char_entry_values);
    selSymbChar->setEnabled(false);
    layout1->addWidget(selSymbChar);

    //ledSymbChar=new stdLineEdit(fraSymbProp,tr("Symbol char:"));
    //ledSymbChar->lenText->setText(QString(""));
    //layout1->addWidget(ledSymbChar);
    fraSymbProp->setLayout(layout1);

    fraLineProp=new QGroupBox(tr("Line properties"),this);
    layout2=new QVBoxLayout;
    //layout2->setMargin(STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    entr[0]=tr("None");
    entr[1]=tr("Straight");
    entr[2]=tr("Left stairs");
    entr[3]=tr("Right stairs");
    entr[4]=tr("Segments");
    entr[5]=tr("3-Segments");
    entr[6]=tr("Increasing X only");
    entr[7]=tr("Decreasing X only");
    number=8;
    cmbLineType=new StdSelector(fraLineProp,tr("Type:"),number,entr);
    layout2->addWidget(cmbLineType);
    cmbLineStyle=new LineStyleSelector(fraLineProp);
    cmbLineStyle->lblText->setText(tr("Style:"));
    layout2->addWidget(cmbLineStyle);
    spnLineWidth=new LineWidthSelector(fraLineProp);
    spnLineWidth->lblText->setText(tr("Width:"));
    layout2->addWidget(spnLineWidth);
    cmbLineColor=new ColorSelector(fraLineProp);
    layout2->addWidget(cmbLineColor);
    connect(cmbLineColor,SIGNAL(currentIndexChanged(int)),SLOT(LineColorChanged(int)));
    fraLineProp->setLayout(layout2);

    fraLegend=new QGroupBox(tr("Legend"),this);
    layout3=new QHBoxLayout;
    //layout3->setMargin(STD_MARGIN);
    layout3->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    ledString=new stdLineEdit(fraLegend,tr("String:"),true);
    ledString->lenText->setText(QString(""));
    layout3->addWidget(ledString);
    fraLegend->setLayout(layout3);

    fraDispOpt=new QGroupBox(tr("Display options"),this);
    layout4=new QHBoxLayout;
    //layout4->setMargin(STD_MARGIN);
    layout4->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    chkAnnVal=new QCheckBox(tr("Annotate values"),fraDispOpt);
    layout4->addWidget(chkAnnVal);
    chkDispErrBars=new QCheckBox(tr("Display error bars"),fraDispOpt);
    layout4->addWidget(chkDispErrBars);
    chkIgnoreInAutoscale=new QCheckBox(tr("Ignore during autoscale"),fraDispOpt);;
    layout4->addWidget(chkIgnoreInAutoscale);
    fraDispOpt->setLayout(layout4);
    layout=new QGridLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(fraSetPres,0,0,1,2);
    layout->addWidget(fraSymbProp,1,0);
    layout->addWidget(fraLineProp,1,1);
    layout->addWidget(fraLegend,2,0,1,2);
    layout->addWidget(fraDispOpt,3,0,1,2);
    setLayout(layout);
    connect(cmbSymbType,SIGNAL(currentIndexChanged(int)),SLOT(SymbTypeChanged(int)));
}

tabMain::~tabMain()
{
    delete[] Type_entries;
}

void tabMain::SymbTypeChanged(int val)
{
    if (val!=SYM_CHAR)
    {
    selSymbChar->setEnabled(false);
    }
    else
    {
    selSymbChar->setEnabled(true);
    }
}

void tabMain::LineColorChanged(int val)
{
    emit(colorChanged(val));
}

void tabMain::SymbColorChanged(int val)
{
    emit(colorChanged2(val));
}

void tabMain::updateSymbolTypeIcons(void)
{
/* symbol-icons */
QIcon * symb_icons=new QIcon[12];
    createSymbolIcons(&symb_icons);
    cmbSymbType->setIcons(symb_icons);
    cmbSymbType->cmbSelect->setIconSize(QSize(12*toolBarSizeFactor,12*toolBarSizeFactor));
delete[] symb_icons;
}

tabSymbol::tabSymbol(QWidget * parent):QWidget(parent)
{
    fraSymbOutl=new QGroupBox(tr("Symbol outline"),this);
    layout0=new QGridLayout;
    //layout0->setMargin(STD_MARGIN);
    layout0->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    cmbSymbStyle=new LineStyleSelector(this);
    cmbSymbStyle->lblText->setText(tr("Style:"));
    layout0->addWidget(cmbSymbStyle,0,0);
    cmbSymbPattern=new FillPatternSelector(this);
    cmbSymbPattern->lblText->setText(tr("Pattern:"));
    layout0->addWidget(cmbSymbPattern,1,0);
    spnSymbWidth=new LineWidthSelector(this);
    spnSymbWidth->lblText->setText(tr("Width:"));
    layout0->addWidget(spnSymbWidth,0,1);
    fraSymbOutl->setLayout(layout0);
    fraSymbFill=new QGroupBox(tr("Symbol fill"),this);
    layout1=new QHBoxLayout;
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    cmbFillColor=new ColorSelector(this);
    cmbFillColor->lblText->setText(tr("Color:"));
    layout1->addWidget(cmbFillColor);
    cmbFillPattern=new FillPatternSelector(this);
    cmbFillPattern->lblText->setText(tr("Pattern:"));
    layout1->addWidget(cmbFillPattern);
    fraSymbFill->setLayout(layout1);
    fraExtra=new QGroupBox(tr("Extra"),this);
    layout2=new QVBoxLayout;
    //layout2->setMargin(STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    spnSymbSkip=new stdIntSelector(this,tr("Symbol skip:"),0,100000);
    layout2->addWidget(spnSymbSkip);
    cmbSymbFont=new FontSelector(this);
    cmbSymbFont->setLabelText(tr("Font for char symbol:"));
    //cmbSymbFont->lblText->setText(tr("Font for char symbol:"));
    layout2->addWidget(cmbSymbFont);
    fraExtra->setLayout(layout2);
    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(fraSymbOutl);
    layout->addWidget(fraSymbFill);
    layout->addWidget(fraExtra);
    empty=new QWidget(this);
    layout->addWidget(empty);
    setLayout(layout);
}

tabLine::tabLine(QWidget * parent):QWidget(parent)
{
    int number=3;
    QString entr[6];
    fraLineProp=new QGroupBox(tr("Line properties"),this);
    //fraLineProp->setGeometry(2,2,parent->width()-8,63);
    layout0=new QHBoxLayout;
    //layout0->setMargin(STD_MARGIN);
    layout0->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    cmbPattern=new FillPatternSelector(this);
    cmbPattern->lblText->setText(tr("Pattern:"));
    layout0->addWidget(cmbPattern);
    chkDrawDropLines=new QCheckBox(tr("Draw drop lines"),fraLineProp);
    layout0->addWidget(chkDrawDropLines);
    fraLineProp->setLayout(layout0);
    fraFillProp=new QGroupBox(tr("Fill properties"),this);
    layout1=new QGridLayout;
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    number=3;
    entr[0]=tr("None");
    entr[1]=tr("As polygon");
    entr[2]=tr("To baseline");
    cmbType=new StdSelector(this,tr("Type:"),number,entr);
    layout1->addWidget(cmbType,0,0);
    number=2;
    entr[0]=tr("Winding");
    entr[1]=tr("Even-Odd");
    cmbRule=new StdSelector(this,tr("Rule:"),number,entr);
    layout1->addWidget(cmbRule,0,1);
    cmbFillPattern=new FillPatternSelector(this);
    cmbFillPattern->lblText->setText(tr("Pattern:"));
    layout1->addWidget(cmbFillPattern,1,0);
    cmbFillColor=new ColorSelector(this);
    layout1->addWidget(cmbFillColor,1,1);
    cmbSet=new SetSelectorCombo(tr("Polygon base set:"),this);
    layout1->addWidget(cmbSet,2,0);
    fraFillProp->setLayout(layout1);
    fraBaseLine=new QGroupBox(tr("Base line"),this);
    layout2=new QHBoxLayout;
    //layout2->setMargin(STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    number=6;
    entr[0]=tr("Zero");
    entr[1]=tr("Set min");
    entr[2]=tr("Set max");
    entr[3]=tr("Graph min");
    entr[4]=tr("Graph max");
    entr[5]=tr("Set average");
    cmbBaseType=new StdSelector(this,tr("Type:"),number,entr);
    layout2->addWidget(cmbBaseType);
    chkDrawLine=new QCheckBox(tr("Draw line"),fraBaseLine);
    layout2->addWidget(chkDrawLine);
    fraBaseLine->setLayout(layout2);
    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(fraLineProp);
    layout->addWidget(fraFillProp);
    layout->addWidget(fraBaseLine);
    empty=new QWidget(this);
    layout->addWidget(empty);
    setLayout(layout);
}

tabAnnVal::tabAnnVal(QWidget * parent):QWidget(parent)
{
    int number=6;
    QString entr[NUM_FMT_OPTION_ITEMS+2];
    char dummy[500];
    fraTextProp=new QGroupBox(tr("Text properties"),this);
    layout0=new QGridLayout;
    //layout0->setMargin(STD_MARGIN);
    layout0->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    cmbFont=new FontSelector(this);
    layout0->addWidget(cmbFont,0,0);
    sldFontSize=new stdSlider(this,tr("Char size"),0,1000);
    layout0->addWidget(sldFontSize,0,1);
    cmbColor=new ColorSelector(this);
    layout0->addWidget(cmbColor,1,0);
    sldFontAngle=new stdSlider(this,tr("Angle"),0,360);
    layout0->addWidget(sldFontAngle,1,1);
    ledPrepend=new stdLineEdit(this,tr("Prepend:"),true);
    ledPrepend->lenText->setText(QString(""));
    layout0->addWidget(ledPrepend,2,0);
    ledAppend=new stdLineEdit(this,tr("Append:"),true);
    ledAppend->lenText->setText(QString(""));
    layout0->addWidget(ledAppend,2,1);
    selAlign=new AlignmentSelector(this);
    selAlign->setToolTip(tr("Sets the text-alignment for each line\n(only useful if you have annotations with more than one line)"));
    layout0->addWidget(selAlign,3,0);
    fraTextProp->setLayout(layout0);
    fraFormatOpt=new QGroupBox(tr("Format options"),this);
    layout1=new QGridLayout;
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    number=6;
    entr[0]=tr("None");
    entr[1]=tr("X");
    entr[2]=tr("Y");
    entr[3]=tr("X,Y");
    entr[4]=tr("String");
    entr[5]=tr("Z");
    cmbType=new StdSelector(this,tr("Type:"),number,entr);
    layout1->addWidget(cmbType,0,0);
    number=10;
    for (int i=0;i<number;i++)
    {
        sprintf(dummy,"%d",i);
        entr[i]=QString(dummy);
    }
    cmbPrecision=new StdSelector(this,tr("Precision:"),number,entr);
    layout1->addWidget(cmbPrecision,0,1);
    for (int i=0;i<NUM_FMT_OPTION_ITEMS;i++)
        entr[i]=QString(fmt_option_items[i].label);
    number=NUM_FMT_OPTION_ITEMS;
    cmbFormat=new StdSelector(this,tr("Format:"),number,entr);
    layout1->addWidget(cmbFormat,1,0,1,2);
    fraFormatOpt->setLayout(layout1);
    fraPlacement=new QGroupBox(tr("Placement"),this);
    layout2=new QHBoxLayout;
    //layout2->setMargin(STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    ledXOffs=new stdLineEdit(this,tr("X offset:"));
    ledXOffs->lenText->setText(QString(""));
    layout2->addWidget(ledXOffs);
    ledYOffs=new stdLineEdit(this,tr("Y offset:"));
    ledYOffs->lenText->setText(QString(""));
    layout2->addWidget(ledYOffs);
    fraPlacement->setLayout(layout2);
    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(fraTextProp);
    layout->addWidget(fraFormatOpt);
    layout->addWidget(fraPlacement);
    setLayout(layout);
}

tabErrorBars::tabErrorBars(QWidget * parent):QWidget(parent)
{
    int number=3;
    QString entr[33];
    fraCommon=new QGroupBox(tr("Common"),this);
    layout0=new QVBoxLayout;
    //layout0->setMargin(STD_MARGIN);
    layout0->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    number=3;
    entr[0]=tr("Normal");
    entr[1]=tr("Opposite");
    entr[2]=tr("Both");
    cmbPlacement=new StdSelector(this,tr("Placement:"),number,entr);
    layout0->addWidget(cmbPlacement);
    cmbColor=new ColorSelector(this);
    layout0->addWidget(cmbColor);
    cmbPattern=new FillPatternSelector(this);
    cmbPattern->lblText->setText(tr("Pattern:"));
    layout0->addWidget(cmbPattern);
    entr[0]=tr("None");
    entr[1]=tr("X- and Y-bars");
    entr[2]=tr("Y-bars only");
    entr[3]=tr("X-bars only");
    entr[4]=tr("Fill Y-bars as polygon");
    entr[5]=tr("Fill X-bars as polygon");
    entr[6]=tr("Fill X- and Y-bars as polygons");
    number=7;
    chkConnectErrorBars=new StdSelector(this,tr("Connect errorbars:"),number,entr);
    layout0->addWidget(chkConnectErrorBars);
    chkShowInLegend=new QCheckBox(tr("Show error bars in legend"),this);
    layout0->addWidget(chkShowInLegend);
    fraCommon->setLayout(layout0);
    fraClipping=new QGroupBox(tr("Clipping"),this);
    layout1=new QVBoxLayout;
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    chkArrowClip=new QCheckBox(tr("Arrow clip"),fraClipping);
    layout1->addWidget(chkArrowClip);
    spnMaxLength=new LineWidthSelector(this);
    spnMaxLength->lblText->setText(tr("Max length:"));
    layout1->addWidget(spnMaxLength);
    fraClipping->setLayout(layout1);
    fraBarLine=new QGroupBox(tr("Bar line"),this);
    layout2=new QVBoxLayout;
    //layout2->setMargin(STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    sldBarSize=new stdSlider(this,tr("Size"),0,1000);
    layout2->addWidget(sldBarSize);
    spnbarWidth=new LineWidthSelector(this);
    spnbarWidth->lblText->setText(tr("Width:"));
    layout2->addWidget(spnbarWidth);
    cmbBarStyle=new LineStyleSelector(this);
    cmbBarStyle->lblText->setText(tr("Style:"));
    layout2->addWidget(cmbBarStyle);
    fraBarLine->setLayout(layout2);
    fraRiserLine=new QGroupBox(tr("Riser line"),this);
    layout3=new QVBoxLayout;
    //layout3->setMargin(STD_MARGIN);
    layout3->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    spnRiserWidth=new LineWidthSelector(this);
    spnRiserWidth->lblText->setText(tr("Width:"));
    layout3->addWidget(spnRiserWidth);
    cmbRiserStyle=new LineStyleSelector(this);
    cmbRiserStyle->lblText->setText(tr("Style:"));
    layout3->addWidget(cmbRiserStyle);
    empty=new QWidget(this);
    empty->setMinimumHeight(30);
    fraRiserLine->setLayout(layout3);
    layout=new QGridLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(fraCommon,0,0);
    layout->addWidget(fraClipping,1,0);
    layout->addWidget(fraBarLine,0,1);
    layout->addWidget(fraRiserLine,1,1);
    layout->addWidget(empty,2,0,1,2);
    setLayout(layout);
}

frmSet_Appearance::frmSet_Appearance(QWidget * parent):QWidget(parent)
{
    //setFont(*stdFont);
    //setWindowTitle(tr("QtGrace: Set Appearance"));
    //setWindowIcon(QIcon(*GraceIcon));
    CreateActions();
    updating=false;
    cset=0;

    menuBar=new QMenuBar(this);

    mnuFile=new QMenu(tr("&File"),this);
    mnuFile->setTearOffEnabled(TRUE);
    /*mnuFile->addAction(actOpenFontTool);
    mnuFile->addSeparator();*/
    mnuFile->addAction(actclose);

    mnuEdit=new QMenu(tr("&Edit"),this);
    mnuEdit->setTearOffEnabled(TRUE);
    mnuEdit->addAction(actsetdiffcolors);
    mnuEdit->addAction(actsetdiffsymbols);
    mnuEdit->addAction(actsetdifflinestyles);
    mnuEdit->addAction(actsetdifflinewidths);
    mnuEdit->addAction(actsetbaw);
    mnuEdit->addSeparator();
    mnuEdit->addAction(actloadcoments);
    mnuEdit->addAction(actstriplegends);

    mnuOptions=new QMenu(tr("&Options"),this);
    mnuOptions->setTearOffEnabled(TRUE);
    mnuOptions->addAction(actdupllegends);
    mnuOptions->addAction(actcolorsync);
    mnuOptions->addAction(actapplyall);

    mnuHelp=new QMenu(tr("&Help"),this);
    mnuHelp->setTearOffEnabled(TRUE);
    mnuHelp->addAction(acthelponcontext);
    mnuHelp->addAction(acthelponsetappearance);

    menuBar->addMenu(mnuFile);
    menuBar->addMenu(mnuEdit);
    menuBar->addMenu(mnuOptions);
    menuBar->addSeparator();
    menuBar->addMenu(mnuHelp);

    lblSelSet=new QLabel(tr("Select set:"),this);
    listSet=new uniList(SETLIST,this);
    connect(listSet,SIGNAL(new_selection(int)),SLOT(newListSelection(int)));

    tabs=new QTabWidget(this);
    tabMa=new tabMain(tabs);
    connect(tabMa,SIGNAL(colorChanged(int)),SLOT(SyncColors(int)));
    connect(tabMa,SIGNAL(colorChanged2(int)),SLOT(SyncColors2(int)));
    tabSy=new tabSymbol(tabs);
    tabLi=new tabLine(tabs);
    tabAnVa=new tabAnnVal(tabs);
    tabErBa=new tabErrorBars(tabs);

    tabs->addTab(tabMa, tr("Main"));
    tabs->addTab(tabSy, tr("Symbols"));
    tabs->addTab(tabLi, tr("Line"));
    tabs->addTab(tabAnVa, tr("Ann. values"));
    tabs->addTab(tabErBa, tr("Error bars"));

    buttonGroup=new stdButtonGroup(this);
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));

    //tabMain
    connect(tabMa->chkAnnVal,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(tabMa->chkDispErrBars,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(tabMa->cmbType,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabMa->cmbLineColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabMa->cmbLineColor,SIGNAL(alphaChanged(int)),SLOT(update1(int)));
    connect(tabMa->cmbLineStyle,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabMa->cmbLineType,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabMa->cmbSymbColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabMa->cmbSymbColor,SIGNAL(alphaChanged(int)),SLOT(update1(int)));
    connect(tabMa->cmbSymbType,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabMa->ledString,SIGNAL(changed()),SLOT(update0()));
    connect(tabMa->sldSymbSize,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(tabMa->spnLineWidth,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    //tabSymbol
    connect(tabSy->cmbFillColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabSy->cmbFillColor,SIGNAL(alphaChanged(int)),SLOT(update1(int)));
    connect(tabSy->cmbFillPattern,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabSy->cmbSymbFont,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabSy->cmbSymbPattern,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabSy->cmbSymbStyle,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabSy->spnSymbSkip,SIGNAL(currentValueChanged(int)),SLOT(update1(int)));
    connect(tabSy->spnSymbWidth,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    //tabLine
    connect(tabLi->cmbBaseType,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLi->cmbFillColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLi->cmbFillColor,SIGNAL(alphaChanged(int)),SLOT(update1(int)));
    connect(tabLi->cmbFillPattern,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLi->cmbPattern,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLi->cmbRule,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLi->cmbType,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLi->chkDrawDropLines,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(tabLi->chkDrawLine,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(tabLi->cmbSet->cmb,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    //tabtabAnnVal
    connect(tabAnVa->cmbColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabAnVa->cmbColor,SIGNAL(alphaChanged(int)),SLOT(update1(int)));
    connect(tabAnVa->cmbFont,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabAnVa->cmbFormat,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabAnVa->cmbPrecision,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabAnVa->cmbType,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabAnVa->ledAppend,SIGNAL(changed()),SLOT(update0()));
    connect(tabAnVa->ledPrepend,SIGNAL(changed()),SLOT(update0()));
    connect(tabAnVa->ledXOffs,SIGNAL(changed()),SLOT(update0()));
    connect(tabAnVa->ledYOffs,SIGNAL(changed()),SLOT(update0()));
    connect(tabAnVa->sldFontAngle,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(tabAnVa->sldFontSize,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(tabAnVa->selAlign->cmbJustSelect2,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    //tabErrorBars
    connect(tabErBa->cmbBarStyle,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabErBa->cmbColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabErBa->cmbColor,SIGNAL(alphaChanged(int)),SLOT(update1(int)));
    connect(tabErBa->cmbPattern,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabErBa->cmbPlacement,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabErBa->cmbRiserStyle,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabErBa->chkArrowClip,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(tabErBa->spnbarWidth,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(tabErBa->spnMaxLength,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(tabErBa->spnRiserWidth,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(tabErBa->sldBarSize,SIGNAL(valueChanged(int)),SLOT(update1(int)));

    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
#ifndef MAC_SYSTEM
    layout->addWidget(menuBar);
#endif
    layout->addWidget(lblSelSet);
    layout->addWidget(listSet);
    layout->addWidget(tabs);
    layout->addWidget(buttonGroup);
    setLayout(layout);
}

frmSet_Appearance::~frmSet_Appearance()
{
}

void frmSet_Appearance::ApplyListOfChanges(void)
{
static int i,nr;//,errpos;
    nr=nr_of_true_changes(ListOfChanges);
    //errpos=0;
    if (nr>0)
    {
        for (i=0;i<ListOfChanges.size();i++)
        {
            strcpy(command,ListOfChanges.at(i).toLocal8Bit().constData());
            //qDebug() << "command=" << command;
            //errpos = scanner(command);
            if (command[0]=='#')
            parse_qtGrace_Additions(command);
            else
            (void)scanner(command);
        }
        set_dirtystate();
        mainWin->mainArea->completeRedraw();
        //undo-Stuff
        SetsModified(nr_of_set_concerned,sets_concerned_g,sets_concerned_s,UNDO_APPEARANCE);
    }
    ListOfChanges.clear();
    ListOfOldStates.clear();
}

void frmSet_Appearance::update0(void)
{
bool sav_imm_iupd;
static bool cur_update_running=false;
int nr_sets,*selset=new int[2];
this->listSet->get_selection(&nr_sets,&selset);
    if (!immediateUpdate || cur_update_running==true || nr_sets<=0)
    {
    if (selset) delete[] selset;
    return;
    }
cur_update_running=true;
    GlobalInhibitor=true;
    sav_imm_iupd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
//qDebug() << "update0 before doApply";
    blockSignals(true);

        doApply();//do this without actually doing something
//qDebug() << "update0 after doApply";
//qDebug() << "update0/Before: l=" << g[this->listSet->gr_no].p[this->cset].lstr << " --- orig_l=" << g[this->listSet->gr_no].p[this->cset].orig_lstr;
        ApplyListOfChanges();
//qDebug() << "update0/After : l=" << g[this->listSet->gr_no].p[this->cset].lstr << " --- orig_l=" << g[this->listSet->gr_no].p[this->cset].orig_lstr;
//qDebug() << "update0 after ApplyListOfChanges";
            for(int i = 0; i < nr_sets; i++)
            {
            strcpy(g[this->listSet->gr_no].p[selset[i]].orig_lstr,g[this->listSet->gr_no].p[selset[i]].lstr);
            convert_single_string_from_encoding_to_UTF8_static(g[this->listSet->gr_no].p[selset[i]].lstr);
            update_grace_string_from_UTF8_static(g[this->listSet->gr_no].p[selset[i]].lstr);
            }
        //tabMa->ledString->SetTextToMemory(g[this->listSet->gr_no].p[this->cset].lstr,g[this->listSet->gr_no].p[this->cset].orig_lstr);
if (selset) delete[] selset;

    GlobalInhibitor=false;
    immediateUpdate=sav_imm_iupd;
    updateRunning=false;
//qDebug() << "update0 before updateSetAppearance";
    updateSetAppearance(this->listSet->gr_no,cset);
//qDebug() << "update0 after updateSetAppearance";
    blockSignals(false);
cur_update_running=false;
}

void frmSet_Appearance::update1(int v)
{
    (void)v;
    update0();
}

void frmSet_Appearance::update2(QString v)
{
    (void)v;
    update0();
}

void frmSet_Appearance::update3(bool v)
{
    (void)v;
    update0();
}

void frmSet_Appearance::update4(double v)
{
    (void)v;
    update0();
}

void frmSet_Appearance::init(void)
{
    bool old_upd;
    old_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    listSet->update_number_of_entries();
    immediateUpdate=old_upd;
    updateRunning=false;
}

void frmSet_Appearance::CreateActions(void)
{
    //Actions for File menu
    actclose = new QAction(tr("&Close"), this);
    actclose->setShortcut(tr("Esc"));
    actclose->setStatusTip(tr("Close this Window"));
    connect(actclose, SIGNAL(triggered()), this, SLOT(doClose()));
    actOpenFontTool = new QAction(tr("&Open font tool"), this);
    actOpenFontTool->setStatusTip(tr("Open the font tool to select a charater for the symbols"));
    connect(actOpenFontTool, SIGNAL(triggered()), this, SLOT(doOpenFontTool()));
    acthelponcontext= new QAction(tr("On onte&xt"), this);
    acthelponcontext->setShortcut(tr("Crtl+F1"));
    connect(acthelponcontext, SIGNAL(triggered()), this, SLOT(doHelpOnContext()));
    acthelponsetappearance= new QAction(tr("On set appearance"), this);
    connect(acthelponsetappearance, SIGNAL(triggered()), this, SLOT(doHelpOnSetAppearance()));
    actdupllegends= new QAction(tr("&Duplicate legends"), this);
    actdupllegends->setToolTip(tr("If Apply is clicked with more than one\nselected set the legend will also be\napplied to all selected sets."));
    actdupllegends->setCheckable(TRUE);
    actdupllegends->setChecked(FALSE);
    connect(actdupllegends, SIGNAL(triggered()), this, SLOT(doDuplLegends()));
    actcolorsync= new QAction(tr("Color &sync"), this);
    actcolorsync->setToolTip(tr("Changing the line-color will also change\nthe color of symbols and error-bars."));
    actcolorsync->setCheckable(TRUE);
    actcolorsync->setChecked(TRUE);
    connect(actcolorsync, SIGNAL(triggered()), this, SLOT(doColorSync()));
    actapplyall= new QAction(tr("Apply &all settings"), this);
    actapplyall->setToolTip(tr("If this is activated, all settings will be\napplied to all sets, otherwise only\nmodified settings are applied."));
    actapplyall->setCheckable(TRUE);
    actapplyall->setChecked(TRUE);
    connect(actapplyall, SIGNAL(triggered()), this, SLOT(doApplyAll()));
    actsetdiffcolors= new QAction(tr("Set different &colors"), this);
    connect(actsetdiffcolors, SIGNAL(triggered()), this, SLOT(doSetDiffColors()));
    actsetdifflinestyles= new QAction(tr("Set different line st&yles"), this);
    connect(actsetdifflinestyles, SIGNAL(triggered()), this, SLOT(doSetDiffLineStyles()));
    actsetdifflinewidths= new QAction(tr("Set different line &widths"), this);
    connect(actsetdifflinewidths, SIGNAL(triggered()), this, SLOT(doSetDiffLineWidths()));
    actsetdiffsymbols= new QAction(tr("Set different &symbols"), this);
    connect(actsetdiffsymbols, SIGNAL(triggered()), this, SLOT(doSetDiffSymbols()));
    actsetbaw= new QAction(tr("Set black && white"), this);
    connect(actsetbaw, SIGNAL(triggered()), this, SLOT(doSetBlackAndWhite()));
    actloadcoments= new QAction(tr("Load co&mments"), this);
    connect(actloadcoments, SIGNAL(triggered()), this, SLOT(doLoadComments()));
    actstriplegends= new QAction(tr("Strip &legends"), this);
    connect(actstriplegends, SIGNAL(triggered()), this, SLOT(doStripLegends()));
}

void frmSet_Appearance::doApply(void)
{
static bool apply_running=false;
if (apply_running==true) return;
apply_running=true;
    char dummy[MAX_STRING_LENGTH];
    char dummy2[MAX_STRING_LENGTH];
    ApplyError=false;
    int i;
    int duplegs;
    int type;
    int sym, symskip, symlines;
    double symlinew;
    int line, linet, color, alpha, pattern;
    double wid;
    int dropline, filltype, fillrule, fillpat, fillcol, fillalpha, polybase;
    int symcolor, symalpha, sympattern, symfillcolor, symfillalpha, symfillpattern;
    double symsize;
    int baseline, baselinetype, ignore_autoscale;
    Errbar errbar;
    AValue avalue;
    char symchar;
    int charfont;
    plotarr p;
    int setno,grano,probeset;
    int *selset=new int[2], cd;
    listSet->get_selection(&cd,&selset);//you get the actual set-id (G0.S2 --> 2), not the entry number in the list (e.g. 0 if S0 and S1 have been deleted)
    if (cd < 1)
    {
        if (immediateUpdate==false)
        errmsg(tr("No set selected").toLocal8Bit().constData());
        ApplyError=true;
        delete[] selset;
        apply_running=false;
        return;
    }
    int * gnos=NULL;
    int * snos=NULL;
    grano=listSet->gr_no;
    if (cd>0)
    {
        gnos=new int[cd];
        snos=new int[cd];
        for (int i=0;i<cd;i++)
        {
            gnos[i]=grano;
            snos[i]=selset[i];
        }
        nr_of_set_concerned=cd;
        if (sets_concerned_g!=NULL) delete[] sets_concerned_g;
        if (sets_concerned_s!=NULL) delete[] sets_concerned_s;
        sets_concerned_g=new int[cd];
        sets_concerned_s=new int[cd];
        memcpy(sets_concerned_g,gnos,sizeof(int)*cd);
        memcpy(sets_concerned_s,snos,sizeof(int)*cd);
        SaveSetStatesPrevious(cd,gnos,snos,UNDO_APPEARANCE);
    }
    else
    {
        nr_of_set_concerned=0;
    }

    duplegs=actdupllegends->isChecked()==true?1:0;

    type = tabMa->Type_entries[tabMa->cmbType->currentIndex()];
    symsize =tabMa->sldSymbSize->value()/100.0;
    sym = tabMa->cmbSymbType->currentIndex();
    color = tabMa->cmbLineColor->currentIndex();
    alpha = tabMa->cmbLineColor->alpha();

    pattern = tabLi->cmbPattern->currentIndex();
    wid = tabMa->spnLineWidth->value();
    baseline = tabLi->chkDrawLine->isChecked()==true?1:0;
    baselinetype = tabLi->cmbBaseType->currentIndex();
    dropline = tabLi->chkDrawDropLines->isChecked()==true?1:0;
    line = tabMa->cmbLineStyle->currentIndex();
    linet = tabMa->cmbLineType->currentIndex();

    filltype = tabLi->cmbType->currentIndex();
    fillrule = tabLi->cmbRule->currentIndex();
    fillpat = tabLi->cmbFillPattern->currentIndex();
    fillcol = tabLi->cmbFillColor->currentIndex();
    fillalpha = tabLi->cmbFillColor->alpha();
    polybase = tabLi->cmbSet->value();
    symskip = tabSy->spnSymbSkip->value();
    symcolor = tabMa->cmbSymbColor->currentIndex();
    symalpha = tabMa->cmbSymbColor->alpha();
    sympattern = tabSy->cmbSymbPattern->currentIndex();
    symfillcolor = tabSy->cmbFillColor->currentIndex();
    symfillalpha = tabSy->cmbFillColor->alpha();
    symfillpattern = tabSy->cmbFillPattern->currentIndex();
    symlinew = tabSy->spnSymbWidth->value();
    symlines = tabSy->cmbSymbStyle->currentIndex();
    symchar = tabMa->selSymbChar->currentValue();
    //symchar = atoi(tabMa->ledSymbChar->text().toLatin1().constData());
    charfont = tabSy->cmbSymbFont->currentIndex();

    errbar.active = tabMa->chkDispErrBars->isChecked()==true?1:0;
    avalue.active = tabMa->chkAnnVal->isChecked()==true?1:0;
    ignore_autoscale = tabMa->chkIgnoreInAutoscale->isChecked()==true?1:0;

    avalue.font = tabAnVa->cmbFont->currentIndex();
    avalue.size = tabAnVa->sldFontSize->value()/100.0;
    avalue.color = tabAnVa->cmbColor->currentIndex();
    avalue.alpha = tabAnVa->cmbColor->alpha();
    avalue.angle = tabAnVa->sldFontAngle->value();
    avalue.format = tabAnVa->cmbFormat->currentIndex();
    avalue.type = tabAnVa->cmbType->currentIndex();
    avalue.prec = tabAnVa->cmbPrecision->currentIndex();
    avalue.align = tabAnVa->selAlign->currentValue();

    tabAnVa->ledPrepend->SetMemoryToText(avalue.prestr,avalue.orig_prestr);
    tabAnVa->ledAppend->SetMemoryToText(avalue.appstr,avalue.orig_appstr);

    xv_evalexpr(tabAnVa->ledXOffs,&avalue.offset.x);
    xv_evalexpr(tabAnVa->ledYOffs,&avalue.offset.y);

    errbar.pen.color = tabErBa->cmbColor->currentIndex();
    errbar.pen.alpha = tabErBa->cmbColor->alpha();
    switch (tabErBa->cmbPlacement->currentIndex())
    {
    case 1:
        errbar.ptype = PLACEMENT_OPPOSITE;
        break;
    case 2:
        errbar.ptype = PLACEMENT_BOTH;
        break;
    default:
        errbar.ptype = PLACEMENT_NORMAL;
        break;
    }
    errbar.show_in_legend = tabErBa->chkShowInLegend->isChecked()==true?1:0;
    errbar.connect_bars = tabErBa->chkConnectErrorBars->currentValue();
    errbar.pen.pattern = tabErBa->cmbPattern->currentIndex();
    errbar.arrow_clip = tabErBa->chkArrowClip->isChecked()==true?1:0;
    errbar.cliplen = tabErBa->spnMaxLength->value();
    errbar.barsize = tabErBa->sldBarSize->value()/100.0;
    errbar.linew = tabErBa->spnbarWidth->value();
    errbar.riser_linew = tabErBa->spnRiserWidth->value();
    errbar.lines = tabErBa->cmbBarStyle->currentIndex();
    errbar.riser_lines = tabErBa->cmbRiserStyle->currentIndex();

    ListOfChanges.clear();
    ListOfOldStates.clear();

    sprintf(dummy,"with g%d",grano);
    ListOfChanges << QString(dummy);
    ListOfOldStates << QString(dummy);

    if (cd < 1)
    {
        if (immediateUpdate==false)
        errmsg(tr("No set selected").toLocal8Bit().constData());
        ApplyError=true;
    }
    else
    {
    for (int kkk=0;kkk<cd;kkk++)
    {
        //compare changes with previous states
        setno=selset[kkk];
        /// setno = -1111111;//selset[0];//first set selected --> NO, WE USE AN INVALID NUMBER HERE
        // the following is to obtain the previous settings
        if (actapplyall->isChecked()==true)//we compare before and after for every set
        {
        probeset=selset[kkk];
        get_graph_plotarr(grano, selset[kkk], &p);
        }
        else//we compare every time with the settings in the first set (the one that is displayed)
        {
        probeset=selset[0];
        get_graph_plotarr(grano, selset[0], &p);
        }

//qDebug() << "G" << grano << ".S" << selset[kkk] << ".type=" << dataset_type(grano, selset[kkk]) << " type =" << type;

        if (dataset_type(grano, probeset) != type)
        {
            sprintf(dummy,"    s%d type %s",setno,set_types(type));
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d type %s",setno,set_types(dataset_type(grano, selset[kkk])));
            ListOfOldStates << QString(dummy);
        }
        if (p.symskip != symskip)
        {
            sprintf(dummy,"    s%d symbol skip %d",setno,symskip);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d symbol skip %d",setno,p.symskip);
            ListOfOldStates << QString(dummy);
        }
        if (p.symsize != symsize)
        {
            sprintf(dummy,"    s%d symbol size %f",setno,symsize);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d symbol size %f",setno,p.symsize);
            ListOfOldStates << QString(dummy);
        }
//qDebug() << "p.symsize=" << p.symsize << " symsize=" << symsize;
        if (p.symlinew != symlinew)
        {
            sprintf(dummy,"    s%d symbol linewidth %f",setno,symlinew);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d symbol linewidth %f",setno,p.symlinew);
            ListOfOldStates << QString(dummy);
        }
        if (p.symlines != symlines)
        {
            sprintf(dummy,"    s%d symbol linestyle %d",setno,symlines);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d symbol linestyle %d",setno,p.symlines);
            ListOfOldStates << QString(dummy);
        }
        if (p.symchar != symchar)
        {
            sprintf(dummy,"    s%d symbol char %d",setno,(int)symchar);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d symbol char %d",setno,(int)(p.symchar));
            ListOfOldStates << QString(dummy);
        }
        if (p.charfont != charfont)
        {
            sprintf(dummy,"    s%d symbol char font %d",setno,charfont);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d symbol char font %d",setno,p.charfont);
            ListOfOldStates << QString(dummy);
        }
        if (p.filltype != filltype)
        {
            sprintf(dummy,"    s%d fill type %d",setno,filltype);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d fill type %d",setno,p.filltype);
            ListOfOldStates << QString(dummy);
        }
        if (p.fillrule != fillrule)
        {
            sprintf(dummy,"    s%d fill rule %d",setno,fillrule);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d fill rule %d",setno,p.fillrule);
            ListOfOldStates << QString(dummy);
        }
        if (p.setfillpen.pattern != fillpat)
        {
            sprintf(dummy,"    s%d fill pattern %d",setno,fillpat);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d fill pattern %d",setno,p.setfillpen.pattern);
            ListOfOldStates << QString(dummy);
        }
        if (p.setfillpen.color != fillcol)
        {
            sprintf(dummy,"    s%d fill color %d",setno,fillcol);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d fill color %d",setno,p.setfillpen.color);
            ListOfOldStates << QString(dummy);
        }
        if (p.sym != sym)
        {
            sprintf(dummy,"    s%d symbol %d",setno,sym);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d symbol %d",setno,p.sym);
            ListOfOldStates << QString(dummy);
        }
        if (p.linet != linet)
        {
            sprintf(dummy,"    s%d line type %d",setno,linet);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d line type %d",setno,p.linet);
            ListOfOldStates << QString(dummy);
        }
        if (p.lines != line)
        {
            sprintf(dummy,"    s%d line linestyle %d",setno,line);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d line linestyle %d",setno,p.lines);
            ListOfOldStates << QString(dummy);
        }
        if (p.linew != wid)
        {
            sprintf(dummy,"    s%d line linewidth %f",setno,wid);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d line linewidth %f",setno,p.linew);
            ListOfOldStates << QString(dummy);
        }
        if (p.linepen.color != color)
        {
            sprintf(dummy,"    s%d line color %d",setno,color);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d line color %d",setno,p.linepen.color);
            ListOfOldStates << QString(dummy);
        }
        if (p.linepen.pattern != pattern)
        {
            sprintf(dummy,"    s%d line pattern %d",setno,pattern);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d line pattern %d",setno,p.linepen.pattern);
            ListOfOldStates << QString(dummy);
        }
        if (p.sympen.color != symcolor)
        {
            sprintf(dummy,"    s%d symbol color %d",setno,symcolor);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d symbol color %d",setno,p.sympen.color);
            ListOfOldStates << QString(dummy);
        }
        if (p.sympen.pattern != sympattern)
        {
            sprintf(dummy,"    s%d symbol pattern %d",setno,sympattern);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d symbol pattern %d",setno,p.sympen.pattern);
            ListOfOldStates << QString(dummy);
        }
        if (p.symfillpen.color != symfillcolor)
        {
            sprintf(dummy,"    s%d symbol fill color %d",setno,symfillcolor);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d symbol fill color %d",setno,p.symfillpen.color);
            ListOfOldStates << QString(dummy);
        }
        if (p.symfillpen.pattern != symfillpattern)
        {
            sprintf(dummy,"    s%d symbol fill pattern %d",setno,symfillpattern);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d symbol fill pattern %d",setno,p.symfillpen.pattern);
            ListOfOldStates << QString(dummy);
        }
        if (p.dropline != dropline)
        {
            sprintf(dummy,"    s%d dropline %s",setno,dropline?"on":"off");
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d dropline %s",setno,p.dropline?"on":"off");
            ListOfOldStates << QString(dummy);
        }
        if (p.baseline != baseline)
        {
            sprintf(dummy,"    s%d baseline %s",setno,baseline?"on":"off");
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d baseline %s",setno,p.baseline?"on":"off");
            ListOfOldStates << QString(dummy);
        }
        if (p.baseline_type != baselinetype)
        {
            sprintf(dummy,"    s%d baseline type %d",setno,baselinetype);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d baseline type %d",setno,p.baseline_type);
            ListOfOldStates << QString(dummy);
        }

        strcpy(dummy2,tabMa->ledString->text().toLocal8Bit());
        if (actapplyall->isChecked()==false && GlobalInhibitor==false)//only apply the changes
        {
        strcpy(dummy2,tabMa->ledString->text().toUtf8());
        convert_single_string_from_UTF8_to_encoding_static(dummy2);
        }
        if (strcmp(p.orig_lstr,dummy2) && (cd == 1 || duplegs))
        {
            sprintf(dummy,"    s%d legend  \"%s\"",setno,dummy2);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d legend  \"%s\"",setno,p.orig_lstr);
            ListOfOldStates << QString(dummy);
        }
        //Annotated-values-properties
        if (p.avalue.active!=avalue.active)
        {
            sprintf(dummy,"    s%d avalue %s",setno,avalue.active?"on":"off");
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d avalue %s",setno,p.avalue.active?"on":"off");
            ListOfOldStates << QString(dummy);
        }
        if (p.avalue.font!=avalue.font)
        {
            sprintf(dummy,"    s%d avalue font %d",setno,avalue.font);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d avalue font %d",setno,p.avalue.font);
            ListOfOldStates << QString(dummy);
        }
        if (p.avalue.size!=avalue.size)
        {
            sprintf(dummy,"    s%d avalue char size %f",setno,avalue.size);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d avalue char size %f",setno,p.avalue.size);
            ListOfOldStates << QString(dummy);
        }
        if (p.avalue.color!=avalue.color)
        {
            sprintf(dummy,"    s%d avalue color %d",setno,avalue.color);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d avalue color %d",setno,p.avalue.color);
            ListOfOldStates << QString(dummy);
        }
        if (p.avalue.angle!=avalue.angle)
        {
            sprintf(dummy,"    s%d avalue rot %d",setno,avalue.angle);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d avalue rot %d",setno,p.avalue.angle);
            ListOfOldStates << QString(dummy);
        }
        if (p.avalue.format!=avalue.format)
        {
            strcpy(dummy2,fmt_option_items[avalue.format].label);
            for (unsigned int j=0;j<strlen(dummy2);j++)
                dummy2[j]=tolower(dummy2[j]);
            sprintf(dummy,"    s%d avalue format %s",setno,dummy2);
            ListOfChanges << QString(dummy);
            strcpy(dummy2,fmt_option_items[p.avalue.format].label);
            for (unsigned int j=0;j<strlen(dummy2);j++)
                dummy2[j]=tolower(dummy2[j]);
            sprintf(dummy,"    s%d avalue format %s",setno,dummy2);
            ListOfOldStates << QString(dummy);
        }
        if (p.avalue.type!=avalue.type)
        {
            sprintf(dummy,"    s%d avalue type %d",setno,avalue.type);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d avalue type %d",setno,p.avalue.type);
            ListOfOldStates << QString(dummy);
        }
        if (p.avalue.prec!=avalue.prec)
        {
            sprintf(dummy,"    s%d avalue prec %d",setno,avalue.prec);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d avalue prec %d",setno,p.avalue.prec);
            ListOfOldStates << QString(dummy);
        }
        if (strcmp(p.avalue.prestr,avalue.prestr))
        {
            sprintf(dummy,"    s%d avalue prepend \"%s\"",setno,avalue.prestr);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d avalue prepend \"%s\"",setno,p.avalue.prestr);
            ListOfOldStates << QString(dummy);
        }
        if (strcmp(p.avalue.appstr,avalue.appstr))
        {
            sprintf(dummy,"    s%d avalue append \"%s\"",setno,avalue.appstr);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d avalue append \"%s\"",setno,p.avalue.appstr);
            ListOfOldStates << QString(dummy);
        }
        if (p.avalue.offset.x!=avalue.offset.x || p.avalue.offset.y!=avalue.offset.y)
        {
            sprintf(dummy,"    s%d avalue offset %f , %f",setno,avalue.offset.x,avalue.offset.y);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d avalue offset %f , %f",setno,p.avalue.offset.x,p.avalue.offset.y);
            ListOfOldStates << QString(dummy);
        }
        if (p.ignore_in_autoscale!=ignore_autoscale)
        {
            sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d IGNOREINAUTOSCALE %d",grano,setno,ignore_autoscale);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d IGNOREINAUTOSCALE %d",grano,setno,p.ignore_in_autoscale);
            ListOfOldStates << QString(dummy);
        }
        if (p.avalue.align!=avalue.align)
        {
            sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d ALIGN_ANNOTATE_VALUES %d",grano,setno,avalue.align);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d ALIGN_ANNOTATE_VALUES %d",grano,setno,p.avalue.align);
            ListOfOldStates << QString(dummy);
        }
        //errorbar-properties
        if (p.errbar.active!=errbar.active)
        {
            sprintf(dummy,"    s%d errorbar %s",setno,errbar.active?"on":"off");
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d errorbar %s",setno,p.errbar.active?"on":"off");
            ListOfOldStates << QString(dummy);
        }
        if (p.errbar.pen.color!=errbar.pen.color)
        {
            sprintf(dummy,"    s%d errorbar color %d",setno,errbar.pen.color);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d errorbar color %d",setno,p.errbar.pen.color);
            ListOfOldStates << QString(dummy);
        }
        if (p.errbar.ptype!=errbar.ptype)
        {
            sprintf(dummy,"    s%d errorbar place %s",setno,errbar.ptype==PLACEMENT_BOTH?"both":(errbar.ptype==PLACEMENT_NORMAL?"normal":"opposite"));
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d errorbar place %s",setno,p.errbar.ptype==PLACEMENT_BOTH?"both":(p.errbar.ptype==PLACEMENT_NORMAL?"normal":"opposite"));
            ListOfOldStates << QString(dummy);
        }
        if (p.errbar.pen.pattern!=errbar.pen.pattern)
        {
            sprintf(dummy,"    s%d errorbar pattern %d",setno,errbar.pen.pattern);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d errorbar pattern %d",setno,p.errbar.pen.pattern);
            ListOfOldStates << QString(dummy);
        }
        if (p.errbar.arrow_clip!=errbar.arrow_clip)
        {
            sprintf(dummy,"    s%d errorbar riser clip %s",setno,errbar.arrow_clip?"on":"off");
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d errorbar riser clip %s",setno,p.errbar.arrow_clip?"on":"off");
            ListOfOldStates << QString(dummy);
        }
        if (p.errbar.cliplen!=errbar.cliplen)
        {
            sprintf(dummy,"    s%d errorbar riser clip length %f",setno,errbar.cliplen);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d errorbar riser clip length %f",setno,p.errbar.cliplen);
            ListOfOldStates << QString(dummy);
        }
        if (p.errbar.barsize!=errbar.barsize)
        {
            sprintf(dummy,"    s%d errorbar size %f",setno,errbar.barsize);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d errorbar size %f",setno,p.errbar.barsize);
            ListOfOldStates << QString(dummy);
        }
        if (p.errbar.linew!=errbar.linew)
        {
            sprintf(dummy,"    s%d errorbar linewidth %f",setno,errbar.linew);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d errorbar linewidth %f",setno,p.errbar.linew);
            ListOfOldStates << QString(dummy);
        }
        if (p.errbar.riser_linew!=errbar.riser_linew)
        {
            sprintf(dummy,"    s%d errorbar riser linewidth %f",setno,errbar.riser_linew);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d errorbar riser linewidth %f",setno,p.errbar.riser_linew);
            ListOfOldStates << QString(dummy);
        }
        if (p.errbar.lines!=errbar.lines)
        {
            sprintf(dummy,"    s%d errorbar linestyle %d",setno,errbar.lines);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d errorbar linestyle %d",setno,p.errbar.lines);
            ListOfOldStates << QString(dummy);
        }
        if (p.errbar.riser_lines!=errbar.riser_lines)
        {
            sprintf(dummy,"    s%d errorbar riser linestyle %d",setno,errbar.riser_lines);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d errorbar riser linestyle %d",setno,p.errbar.riser_lines);
            ListOfOldStates << QString(dummy);
        }
        //alpha-channels
        if (alpha!=p.linepen.alpha || fillalpha!=p.setfillpen.alpha || symalpha!=p.sympen.alpha || symfillalpha!=p.symfillpen.alpha || avalue.alpha!=p.avalue.alpha || errbar.pen.alpha!=p.errbar.pen.alpha)
        {
            sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d ALPHA_CHANNELS %s",grano,setno,create_list_of_arguments(6, alpha,fillalpha,symalpha,symfillalpha,avalue.alpha,errbar.pen.alpha));
            ListOfChanges << QString(dummy);
            sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d ALPHA_CHANNELS %s",grano,setno,create_list_of_arguments(6, p.linepen.alpha,p.setfillpen.alpha,p.sympen.alpha,p.symfillpen.alpha,p.avalue.alpha,p.errbar.pen.alpha));
            ListOfOldStates << QString(dummy);
        }
        if (p.errbar.show_in_legend!=errbar.show_in_legend)
        {
            sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d SHOWERRORBARINLEGEND %d",grano,setno,errbar.show_in_legend);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d SHOWERRORBARINLEGEND %d",grano,setno,p.errbar.show_in_legend);
            ListOfOldStates << QString(dummy);
        }
        if (p.errbar.connect_bars!=errbar.connect_bars)
        {
            sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d CONNECTERRORBARS %d",grano,setno,errbar.connect_bars);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d CONNECTERRORBARS %d",grano,setno,p.errbar.connect_bars);
            ListOfOldStates << QString(dummy);
        }
        if (p.polygone_base_set!=polybase)
        {
            sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d POLYGONEBASESET %d\n",grano,setno,polybase);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d POLYGONEBASESET %d\n",grano,setno,p.polygone_base_set);
            ListOfOldStates << QString(dummy);
        }
    }//end loop over all selected sets
        //end comparison
        /*qDebug() << "Commands:";
        for (i=0;i<ListOfChanges.size();i++)
        qDebug() << ListOfChanges.at(i);*/
        /// the following has been deleted because all commands are written for all sets! (not only one)
        /*
        QStringList help_list1=ListOfChanges;
        QStringList help_list2=help_list1;
        ListOfChanges.clear();
        for (i = 0; i < cd; i++)
        {
            sprintf(dummy,"s%d",selset[i]);
            help_list2.replaceInStrings("s-1111111",dummy);//-1111111 does not exist --> I use this to replace it by different set-ids
            sprintf(dummy,"S %d",selset[i]);
            help_list2.replaceInStrings("S -1111111",dummy);
            ListOfChanges << help_list2;
            help_list2=help_list1;
        }
        */

        /*
        for(i = 0; i < cd; i++)
            g[grano].p[selset[i]].avalue=avalue;//necessary
        */
        if (GlobalInhibitor==false && actapplyall->isChecked()==true)//if GlobalInhibitor==true --> do nothing for real
        {
            blockSignals(true);
            //start update
            for(i = 0; i < cd; i++)
            {
                setno = selset[i];
                get_graph_plotarr(grano, setno, &p);
                p.symskip = symskip;
                p.symsize = symsize;
                p.symlinew = symlinew;
                p.symlines = symlines;
                p.symchar = symchar;
                p.charfont = charfont;
                p.filltype = filltype;
                p.fillrule = fillrule;
                p.setfillpen.pattern = fillpat;
                p.setfillpen.color = fillcol;
                p.setfillpen.alpha = fillalpha;
                if (cd == 1 || duplegs)
                {
                    //strcpy(p.lstr,tabMa->ledString->text().toLocal8Bit());
                    tabMa->ledString->SetMemoryToText(p.lstr,p.orig_lstr);
                    //setting a string
                }
                p.sym = sym;
                p.linet = linet;
                p.lines = line;
                p.linew = wid;
                p.linepen.color = color;
                p.linepen.alpha = alpha;
                p.linepen.pattern = pattern;
                p.sympen.color = symcolor;
                p.sympen.alpha = symalpha;
                p.sympen.pattern = sympattern;
                p.symfillpen.color = symfillcolor;
                p.symfillpen.alpha = symfillalpha;
                p.symfillpen.pattern = symfillpattern;
                p.dropline = dropline;
                p.baseline = baseline;
                p.baseline_type = baselinetype;
                p.polygone_base_set = polybase;
                p.errbar = errbar;
                p.avalue = avalue;
                p.ignore_in_autoscale = ignore_autoscale;

                set_graph_plotarr(grano, setno, &p);
                set_dataset_type(grano, setno, type);

                if (cd == 1 || duplegs)//with dynamic strings-->we have to reload from the NEW address
                {
                    tabMa->ledString->SetTextToMemory(g[grano].p[setno].lstr,g[grano].p[setno].orig_lstr);
                }

            }
            SetsModified(cd,gnos,snos,UNDO_APPEARANCE);
            ListOfChanges.clear();
            ListOfOldStates.clear();
            blockSignals(false);
        }//end global_inhibitor
        /// the following removed in QtGrace v0.2.6
        /*else if (p.polygone_base_set!=polybase)//a special addition for filling the area between different sets
        {
        /// this should be inside the other if-clause
            for(i = 0; i < cd; i++)
            {
                setno = selset[i];
                get_graph_plotarr(grano, setno, &p);
                p.polygone_base_set = polybase;
                set_graph_plotarr(grano, setno, &p);
            }
        }*/
        if (gnos!=NULL) delete[] gnos;
        if (snos!=NULL) delete[] snos;
    }
    blockSignals(true);
//qDebug() << "before apply and update";
    if (actapplyall->isChecked()==false && GlobalInhibitor==false)//only apply the changes
    {
        /*for(i = 0; i < cd; i++)
        {
        strcpy(g[grano].p[selset[i]].lstr,g[grano].p[selset[i]].orig_lstr);
        }*/
        mainWin->mainArea->compl_redraw_running=true;
        prepare_strings_for_saving();
//qDebug() << "Apply/Before: l=" << g[this->listSet->gr_no].p[this->cset].lstr << " --- orig_l=" << g[this->listSet->gr_no].p[this->cset].orig_lstr;
            /*for(i = 0; i < cd; i++)
            qDebug() << "vor apply: orig_lstr=" << g[grano].p[selset[i]].orig_lstr << " lstr=" << g[grano].p[selset[i]].lstr;*/

    ApplyListOfChanges();
            /*for(i = 0; i < cd; i++)
            qDebug() << "nach apply: orig_lstr=" << g[grano].p[selset[i]].orig_lstr << " lstr=" << g[grano].p[selset[i]].lstr;*/

//qDebug() << "Apply/After : l=" << g[this->listSet->gr_no].p[this->cset].lstr << " --- orig_l=" << g[this->listSet->gr_no].p[this->cset].orig_lstr;
        resume_strings_after_load_or_save();
        mainWin->mainArea->compl_redraw_running=false;
        /*for(i = 0; i < cd; i++)
        {
        qDebug() << "orig_lstr=" << g[grano].p[selset[i]].orig_lstr << " lstr=" << g[grano].p[selset[i]].lstr;
        strcpy(g[grano].p[selset[i]].orig_lstr,g[grano].p[selset[i]].lstr);
        qDebug() << "orig_lstr=" << g[grano].p[selset[i]].orig_lstr << " lstr=" << g[grano].p[selset[i]].lstr;
        convert_single_string_from_encoding_to_UTF8_static(g[grano].p[selset[i]].lstr);
        update_grace_string_from_UTF8_static(g[grano].p[selset[i]].lstr);
        }*/
    //tabMa->ledString->SetTextToMemory(g[this->listSet->gr_no].p[this->cset].lstr,g[this->listSet->gr_no].p[this->cset].orig_lstr);
    mainWin->mainArea->completeRedraw();
    }
    else if (GlobalInhibitor==false)//we already did the changes
    {
    mainWin->mainArea->completeRedraw();
    }
//qDebug() << "before set_new_selection";
    listSet->set_new_selection(cd,selset);
//qDebug() << "before showSetData";
    if (cd>0)
    {
    this->showSetData(grano,selset[0]);
    }
    delete[] selset;
//qDebug() << "after apply and update";
    blockSignals(false);
apply_running=false;
}

void frmSet_Appearance::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmSet_Appearance::doClose(void)
{
//parentWidget()->hide();
    emit(closeWish());
}

void frmSet_Appearance::doOpenFontTool(void)
{
    if (FormFontTool==NULL)
    {
        FormFontTool=new frmFontTool(this);
        FormFontTool->resize(460,570);
    }
    FormFontTool->show();
    FormFontTool->raise();
    FormFontTool->activateWindow();
}

void frmSet_Appearance::doHelpOnContext(void)
{
    mainWin->setCursor(QCursor(Qt::WhatsThisCursor));
}

void frmSet_Appearance::doHelpOnSetAppearance(void)
{
    HelpCB("doc/UsersGuide.html#set-appearance");
}

void frmSet_Appearance::doDuplLegends(void)
{
    ;
}

void frmSet_Appearance::doColorSync(void)
{
    ;
}

void frmSet_Appearance::doApplyAll(void)
{
    ;
}

void frmSet_Appearance::doSetDiffColors(void)
{
    setapp_data_proc(SETAPP_ALL_COLORS);
}

void frmSet_Appearance::doSetDiffLineStyles(void)
{
    setapp_data_proc(SETAPP_ALL_LINES);
}

void frmSet_Appearance::doSetDiffLineWidths(void)
{
    setapp_data_proc(SETAPP_ALL_LINEW);
}

void frmSet_Appearance::doSetDiffSymbols(void)
{
    setapp_data_proc(SETAPP_ALL_SYMBOLS);
}

void frmSet_Appearance::doSetBlackAndWhite(void)
{
    setapp_data_proc(SETAPP_ALL_BW);
}

void frmSet_Appearance::doLoadComments(void)
{
    setapp_data_proc(SETAPP_LOAD_COMMENTS);
}

void frmSet_Appearance::doStripLegends(void)
{
    setapp_data_proc(SETAPP_STRIP_LEGENDS);
}

void frmSet_Appearance::newListSelection(int a)
{
    bool old_upd;
    old_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    showSetData(listSet->gr_no,listSet->entries[a]);
    immediateUpdate=old_upd;
    updateRunning=false;
}

void frmSet_Appearance::ShowSetData_external(int graph_number,int set_number)
{
    bool old_upd;
    old_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    int * selection=new int[2];
    int number=1;
/*int old_gno,old_selection;
    listSet->get_selection(&number,&selection);
    if (number<1)
    {
    old_gno=old_selection=-100;
    }
    else
    {
    old_gno=listSet->gr_no;
    old_selection=selection[0];
    }
if (old_gno==graph_number && old_selection==set_number)
{//just redisplay the old data
showSetData(graph_number,set_number);
}
else
{
    //reset selection and display (new) data
    if (selection!=NULL)
    delete[] selection;
selection=new int[2];*/
selection[0]=set_number;

    if (listSet->gr_no!=graph_number)
    listSet->set_graph_number(graph_number,false);

    listSet->set_new_selection(number,selection);

    listSet->get_selection(&number,&selection);
    showSetData(graph_number,set_number);
//}
    if (selection!=NULL)
    delete[] selection;
    immediateUpdate=old_upd;
    updateRunning=false;
}

void frmSet_Appearance::showSetData(int graph_number,int set_number)
{
    if (is_valid_setno(graph_number,set_number)==FALSE) return;

    updating=true;
    cset=set_number;

    char dummy[2048];
    plotarr p;
    get_graph_plotarr(graph_number, set_number, &p);

    redisplayContents();//to switch the decimal separator

    tabLi->cmbSet->update_entries(graph_number,true);

    ///SET APPROPRIATE SET_TYPE_CHOICES ACCORDING TO NUMBER OF COLUMNS --> SHOW ONLY AVAILABLE CHOICES, I DON'T KNOW HOW TO DISABLE CHOICES
    tabMa->cmbType->cmbSelect->clear();
    tabMa->number_of_Type_entries=0;
    delete[] tabMa->Type_entries;
    for (int i=0;i<NUMBER_OF_SETTYPES;i++)
        if (settype_cols(i) == settype_cols(p.type)) tabMa->number_of_Type_entries++;
    tabMa->Type_entries=new int[tabMa->number_of_Type_entries];
    tabMa->number_of_Type_entries=0;
    for (int i=0;i<NUMBER_OF_SETTYPES;i++)
    {
        if (settype_cols(i) == settype_cols(p.type))
        {
            strToUpper(dummy,set_types(i));
            tabMa->cmbType->cmbSelect->addItem(QString(dummy));
            tabMa->Type_entries[tabMa->number_of_Type_entries]=i;
            tabMa->number_of_Type_entries++;
        }
    }
    /*
SetOptionChoice(type_item, p.type);
        for (i = 0; i < type_item->nchoices; i++) {
            if (settype_cols(type_item->options[i].value) ==
                                            settype_cols(p.type)) {
                SetSensitive(type_item->options[i].widget, True);
            } else {
                SetSensitive(type_item->options[i].widget, False);
            }
        }
tabMa->cmbType
*/
    for (int i=0;i<tabMa->number_of_Type_entries;i++)
    {
        if (p.type==tabMa->Type_entries[i])
        {
            tabMa->cmbType->setCurrentIndex(i);
            break;
        }
    }
    tabMa->cmbSymbColor->setCurrentIndex(p.sympen.color);
    tabMa->cmbSymbColor->setAlpha(p.sympen.alpha);
    tabMa->cmbSymbType->setCurrentIndex(p.sym);
    tabMa->sldSymbSize->setValue((int)rint_2(p.symsize*100.0));
    //qDebug() << "p.symsize=" << p.symsize << " sliderValue: " << (int)(p.symsize*100.0) << " rint2()=" << rint_2(p.symsize*100.0);
    //tabMa->ledSymbChar->setDoubleValue("%d",(int)p.symchar);
        if (tabMa->selSymbChar->valueIsInList(p.symchar)==FALSE)
        tabMa->selSymbChar->setCurrentValue(65);//revert the char to 'A', whenever the value is 0 (which is unprintable)
        else
        tabMa->selSymbChar->setCurrentValue(p.symchar);
    tabMa->SymbTypeChanged(p.sym);
    //sprintf(val, "%d", p.symchar);
    //tabMa->ledSymbChar->setText(QString(val));
    tabMa->cmbLineType->setCurrentIndex(p.linet);
    tabMa->cmbLineStyle->setCurrentIndex(p.lines);
    tabMa->cmbLineColor->setCurrentIndex(p.linepen.color);
    tabMa->cmbLineColor->setAlpha(p.linepen.alpha);
    tabMa->spnLineWidth->setValue(p.linew);
    //tabMa->ledString->setText(QString(p.lstr));
    tabMa->ledString->SetTextToMemory(g[graph_number].p[set_number].lstr,g[graph_number].p[set_number].orig_lstr);
    tabMa->chkAnnVal->setChecked(p.avalue.active);
    tabMa->chkDispErrBars->setChecked(p.errbar.active);
    tabMa->chkIgnoreInAutoscale->setChecked(p.ignore_in_autoscale);

    tabSy->spnSymbWidth->setValue(p.symlinew);
    tabSy->cmbSymbStyle->setCurrentIndex(p.symlines);
    tabSy->cmbSymbPattern->setCurrentIndex(p.sympen.pattern);
    tabSy->cmbFillColor->setCurrentIndex(p.symfillpen.color);
    tabSy->cmbFillColor->setAlpha(p.symfillpen.alpha);
    tabSy->cmbFillPattern->setCurrentIndex(p.symfillpen.pattern);
    tabSy->spnSymbSkip->setValue(p.symskip);
    tabSy->cmbSymbFont->setCurrentIndex(p.charfont);

    tabLi->cmbPattern->setCurrentIndex(p.linepen.pattern);
    tabLi->chkDrawDropLines->setChecked(p.dropline);
    tabLi->chkDrawLine->setChecked(p.baseline);
    tabLi->cmbBaseType->setCurrentIndex(p.baseline_type);
    tabLi->cmbType->setCurrentIndex(p.filltype);
    tabLi->cmbRule->setCurrentIndex(p.fillrule);
    tabLi->cmbFillPattern->setCurrentIndex(p.setfillpen.pattern);
    tabLi->cmbFillColor->setCurrentIndex(p.setfillpen.color);
    tabLi->cmbFillColor->setAlpha(p.setfillpen.alpha);
    tabLi->cmbSet->setValue(p.polygone_base_set);

    tabAnVa->cmbFont->setCurrentIndex(p.avalue.font);
    tabAnVa->sldFontSize->setValue((int)rint_2(p.avalue.size*100.0));
    tabAnVa->cmbColor->setCurrentIndex(p.avalue.color);
    tabAnVa->cmbColor->setAlpha(p.avalue.alpha);
    tabAnVa->sldFontAngle->setValue(p.avalue.angle);
    //tabAnVa->ledPrepend->setText(QString(p.avalue.prestr));
    //tabAnVa->ledAppend->setText(QString(p.avalue.appstr));
    tabAnVa->ledPrepend->SetTextToMemory(g[graph_number].p[set_number].avalue.prestr,g[graph_number].p[set_number].avalue.orig_prestr);
    tabAnVa->ledAppend->SetTextToMemory(g[graph_number].p[set_number].avalue.appstr,g[graph_number].p[set_number].avalue.orig_appstr);
    tabAnVa->selAlign->setCurrentValue(p.avalue.align);
    tabAnVa->cmbType->setCurrentIndex(p.avalue.type);
    tabAnVa->cmbPrecision->setCurrentIndex(p.avalue.prec);
    tabAnVa->cmbFormat->setCurrentIndex(p.avalue.format);
    tabAnVa->ledXOffs->setDoubleValue("%g", p.avalue.offset.x);
    tabAnVa->ledYOffs->setDoubleValue("%g", p.avalue.offset.y);
    //sprintf(val, "%f", p.avalue.offset.x);
    //tabAnVa->ledXOffs->setText(QString(val));
    //sprintf(val, "%f", p.avalue.offset.y);
    //tabAnVa->ledYOffs->setText(QString(val));
    tabErBa->cmbColor->setCurrentIndex(p.errbar.pen.color);
    tabErBa->cmbColor->setAlpha(p.errbar.pen.alpha);
    tabErBa->cmbPattern->setCurrentIndex(p.errbar.pen.pattern);
    tabErBa->chkArrowClip->setChecked(p.errbar.arrow_clip);
    tabErBa->chkShowInLegend->setChecked(p.errbar.show_in_legend);
    tabErBa->chkConnectErrorBars->setCurrentValue(p.errbar.connect_bars);
    tabErBa->spnMaxLength->setValue(p.errbar.cliplen);
    tabErBa->spnbarWidth->setValue(p.errbar.linew);
    tabErBa->cmbBarStyle->setCurrentIndex(p.errbar.lines);
    tabErBa->spnRiserWidth->setValue(p.errbar.riser_linew);
    tabErBa->cmbRiserStyle->setCurrentIndex(p.errbar.riser_lines);
    tabErBa->sldBarSize->setValue((int)rint_2(p.errbar.barsize*100.0));
    tabErBa->cmbPlacement->setCurrentIndex(p.errbar.ptype);
    ///SET ITEM 4 ENABLED OR NOT --> ITEM 4 IS "NULL" --> DON'T KNOW WHAT THIS IS FOR!?
    /*
        switch (p.type) {
        case SET_XYDXDX:
        case SET_XYDYDY:
        case SET_XYDXDXDYDY:
            SetSensitive(errbar_ptype_item[4], False);
            break;
        default:
            SetSensitive(errbar_ptype_item[4], True);
            break;
        }
*/
    updating=false;
}

void frmSet_Appearance::SyncColors(int val)
{
    if (updating==true || actcolorsync->isChecked()==FALSE) return;//Color Change is internal or no sync-ing intended
    tabMa->cmbSymbColor->setCurrentIndex(val);
    tabSy->cmbFillColor->setCurrentIndex(val);
    tabErBa->cmbColor->setCurrentIndex(val);
}

void frmSet_Appearance::SyncColors2(int val)
{
    if (updating==true || actcolorsync->isChecked()==FALSE) return;//Color Change is internal or no sync-ing intended
    tabSy->cmbFillColor->setCurrentIndex(val);
    tabErBa->cmbColor->setCurrentIndex(val);
}

void frmSet_Appearance::writeSetData(int graph_number,int set_number)
{
    char err_txt[512];
    if (graph_number<0 || graph_number>number_of_graphs())
    {
        sprintf(err_txt,"%s%d%s%d!",tr("Invalid Graph number: ").toLocal8Bit().constData(),graph_number,tr(", can't write to Set ").toLocal8Bit().constData(),set_number);
        errmsg(err_txt);
        return;
    }
    if (set_number<0 || set_number>=g[graph_number].maxplot)
    {
        sprintf(err_txt,"%s%d%s%d!",tr("Invalid Set number: ").toLocal8Bit().constData(),set_number,tr("; No such Set in Graph ").toLocal8Bit().constData(),graph_number);
        errmsg(err_txt);
        return;
    }
}

void frmSet_Appearance::setapp_data_proc(int dat)
{
    bool sav_imm_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    int proc_type;
    int *selset=new int[2], cd;
    int i, setno;
    int old_val,new_val;
    double d_old_val,d_new_val;
    char * old_comm=NULL,*new_comm=NULL,*tmp_po;
    plotarr p;
    int c = 0, bg = getbgcolor();

    proc_type = dat;
    int * gnos=NULL;
    int * snos=NULL;

    listSet->get_selection(&cd,&selset);

    if (cd < 1)
    {
        if (sav_imm_upd==false)
        errmsg(tr("No set selected").toLocal8Bit().constData());
        immediateUpdate=sav_imm_upd;
        updateRunning=false;
        return;
    }
    else
    {
        gnos=new int[cd+1];
        snos=new int[cd+1];
        for (i=0;i<cd;i++)
        {
            gnos[i]=listSet->gr_no;
            snos[i]=selset[i];
        }
        SaveSetStatesPrevious(cd,gnos,snos,UNDO_APPEARANCE);
        ListOfChanges.clear();
        ListOfOldStates.clear();
        sprintf(dummy,"with g%d",listSet->gr_no);
        ListOfChanges << QString(dummy);
        ListOfOldStates << QString(dummy);
        for(i = 0; i < cd; i++)
        {
            setno = selset[i];
            switch (proc_type)
            {
            case SETAPP_STRIP_LEGENDS:
                tmp_po=get_legend_string(cg, setno);
                if (old_comm!=NULL) delete[] old_comm;
                old_comm=new char[strlen(tmp_po)+2];
                strcpy(old_comm,tmp_po);
                    if (!strncmp(tmp_po,"binary import from: ",20))
                    {
                    if (new_comm!=NULL) delete[] new_comm;
                    new_comm=new char[strlen(tmp_po)];
                    strcpy(new_comm,tmp_po+20);
                    }
                    else
                    {
                    if (inhibit_set_legend==0)
                    set_legend_string(cg, setno, mybasename(get_legend_string(cg, setno)));
                    tmp_po=get_legend_string(cg, setno);
                    if (new_comm!=NULL) delete[] new_comm;
                    new_comm=new char[strlen(tmp_po)+2];
                    strcpy(new_comm,tmp_po);
                    }
                sprintf(dummy,"    s%d legend  \"%s\"",setno,new_comm);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    s%d legend  \"%s\"",setno,old_comm);
                ListOfOldStates << QString(dummy);
                break;
            case SETAPP_LOAD_COMMENTS:
                tmp_po=get_legend_string(cg, setno);
                if (old_comm!=NULL) delete[] old_comm;
                old_comm=new char[strlen(tmp_po)+2];
                strcpy(old_comm,tmp_po);
                load_comments_to_legend(cg, setno);
                tmp_po=get_legend_string(cg, setno);
                if (new_comm!=NULL) delete[] new_comm;
                new_comm=new char[strlen(tmp_po)+2];
                strcpy(new_comm,tmp_po);
                sprintf(dummy,"    s%d legend  \"%s\"",setno,new_comm);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    s%d legend  \"%s\"",setno,old_comm);
                ListOfOldStates << QString(dummy);
                break;
            case SETAPP_ALL_COLORS:
                while (c == bg || get_colortype(c) != COLOR_MAIN)
                {
                    c++;
                    c %= number_of_colors();
                }
                sprintf(dummy,"    s%d line color %d",setno,g[cg].p[setno].linepen.color);
                ListOfOldStates << QString(dummy);
                sprintf(dummy,"    s%d symbol color %d",setno,g[cg].p[setno].sympen.color);
                ListOfOldStates << QString(dummy);
                sprintf(dummy,"    s%d symbol fill color %d",setno,g[cg].p[setno].symfillpen.color);
                ListOfOldStates << QString(dummy);
                sprintf(dummy,"    s%d errorbar color %d",setno,g[cg].p[setno].errbar.pen.color);
                ListOfOldStates << QString(dummy);
                set_set_colors(cg, setno, c);
                sprintf(dummy,"    s%d line color %d",setno,g[cg].p[setno].linepen.color);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    s%d symbol color %d",setno,g[cg].p[setno].sympen.color);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    s%d symbol fill color %d",setno,g[cg].p[setno].symfillpen.color);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    s%d errorbar color %d",setno,g[cg].p[setno].errbar.pen.color);
                ListOfChanges << QString(dummy);
                c++;
                break;
            case SETAPP_ALL_SYMBOLS:
                get_graph_plotarr(cg, setno, &p);
                old_val=p.sym;
                p.sym = (i % (MAXSYM - 2)) + 1;
                new_val=p.sym;
                sprintf(dummy,"    s%d symbol %d",setno,new_val);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    s%d symbol %d",setno,old_val);
                ListOfOldStates << QString(dummy);
                set_graph_plotarr(cg, setno, &p);
                break;
            case SETAPP_ALL_LINEW:
                get_graph_plotarr(cg, setno, &p);
                d_old_val=p.linew;
                p.linew = ((i % (2*((int) MAX_LINEWIDTH) - 1)) + 1)/2.0;
                d_new_val=p.linew;
                sprintf(dummy,"    s%d symbol linewidth %f",setno,d_new_val);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    s%d symbol linewidth %f",setno,d_old_val);
                ListOfOldStates << QString(dummy);
                set_graph_plotarr(cg, setno, &p);
                break;
            case SETAPP_ALL_LINES:
                get_graph_plotarr(cg, setno, &p);
                old_val=p.lines;
                p.lines = (i % (number_of_linestyles() - 1)) + 1;
                new_val=p.lines;
                sprintf(dummy,"    s%d symbol linestyle %d",setno,old_val);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    s%d symbol linestyle %d",setno,new_val);
                ListOfOldStates << QString(dummy);
                set_graph_plotarr(cg, setno, &p);
                break;
            case SETAPP_ALL_BW:
                sprintf(dummy,"    s%d line color %d",setno,g[cg].p[setno].linepen.color);
                ListOfOldStates << QString(dummy);
                sprintf(dummy,"    s%d symbol color %d",setno,g[cg].p[setno].sympen.color);
                ListOfOldStates << QString(dummy);
                sprintf(dummy,"    s%d symbol fill color %d",setno,g[cg].p[setno].symfillpen.color);
                ListOfOldStates << QString(dummy);
                sprintf(dummy,"    s%d errorbar color %d",setno,g[cg].p[setno].errbar.pen.color);
                ListOfOldStates << QString(dummy);
                set_set_colors(cg, setno, 1);
                sprintf(dummy,"    s%d line color %d",setno,g[cg].p[setno].linepen.color);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    s%d symbol color %d",setno,g[cg].p[setno].sympen.color);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    s%d symbol fill color %d",setno,g[cg].p[setno].symfillpen.color);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    s%d errorbar color %d",setno,g[cg].p[setno].errbar.pen.color);
                ListOfChanges << QString(dummy);
                break;
            }//end switch
        }//end for
        //qDebug() << "cg=" << cg << " cset=" << cset;
        showSetData(cg, cset);
        ///UpdateSymbols(cg, cset);
        set_dirtystate();
        //mainWin->mainArea->completeRedraw();
        force_redraw();
        SetsModified(cd,gnos,snos,UNDO_APPEARANCE);
    }//end if c>=1
    if (gnos!=NULL) delete[] gnos;
    if (snos!=NULL) delete[] snos;
    if (old_comm!=NULL) delete[] old_comm;
    if (new_comm!=NULL) delete[] new_comm;
    delete[] selset;
    immediateUpdate=sav_imm_upd;
    updateRunning=false;
}

void frmSet_Appearance::redisplayContents(void)
{
/*tabMa->spnLineWidth->blockSignals(true);
tabSy->spnSymbWidth->blockSignals(true);
tabAnVa->ledXOffs->blockSignals(true);
tabAnVa->ledYOffs->blockSignals(true);
tabErBa->spnMaxLength->blockSignals(true);
tabErBa->spnbarWidth->blockSignals(true);
tabErBa->spnRiserWidth->blockSignals(true);*/
    if (DecimalPointToUse=='.')
    {
        tabMa->spnLineWidth->spnLineWidth->setLocale(*dot_locale);
        tabSy->spnSymbWidth->spnLineWidth->setLocale(*dot_locale);
        tabErBa->spnMaxLength->spnLineWidth->setLocale(*dot_locale);
        tabErBa->spnbarWidth->spnLineWidth->setLocale(*dot_locale);
        tabErBa->spnRiserWidth->spnLineWidth->setLocale(*dot_locale);
    }
    else
    {
        tabMa->spnLineWidth->spnLineWidth->setLocale(*comma_locale);
        tabSy->spnSymbWidth->spnLineWidth->setLocale(*comma_locale);
        tabErBa->spnMaxLength->spnLineWidth->setLocale(*comma_locale);
        tabErBa->spnbarWidth->spnLineWidth->setLocale(*comma_locale);
        tabErBa->spnRiserWidth->spnLineWidth->setLocale(*comma_locale);
    }
//if (OldDecimalPoint==DecimalPointToUse) return;
    tabMa->spnLineWidth->setValue(tabMa->spnLineWidth->value());
    tabSy->spnSymbWidth->setValue(tabSy->spnSymbWidth->value());
    tabAnVa->ledXOffs->ReplaceNumberContents();
    tabAnVa->ledYOffs->ReplaceNumberContents();
    tabErBa->spnMaxLength->setValue(tabErBa->spnMaxLength->value());
    tabErBa->spnbarWidth->setValue(tabErBa->spnbarWidth->value());
    tabErBa->spnRiserWidth->setValue(tabErBa->spnRiserWidth->value());
/*tabMa->spnLineWidth->blockSignals(false);
tabSy->spnSymbWidth->blockSignals(false);
tabAnVa->ledXOffs->blockSignals(false);
tabAnVa->ledYOffs->blockSignals(false);
tabErBa->spnMaxLength->blockSignals(false);
tabErBa->spnbarWidth->blockSignals(false);
tabErBa->spnRiserWidth->blockSignals(false);*/

    /*tabMa->adjustSize();
    tabSy->adjustSize();
    tabLi->adjustSize();
    tabAnVa->adjustSize();
    tabErBa->adjustSize();*/

    tabMa->layout->update();
    tabMa->layout0->update();
    tabMa->layout1->update();
    tabMa->layout2->update();
    tabMa->layout3->update();
    tabMa->layout4->update();

    tabSy->layout->update();
    tabSy->layout0->update();
    tabSy->layout1->update();
    tabSy->layout2->update();

    tabLi->layout->update();
    tabLi->layout0->update();
    tabLi->layout1->update();
    tabLi->layout2->update();

    tabAnVa->layout->update();
    tabAnVa->layout0->update();
    tabAnVa->layout1->update();
    tabAnVa->layout2->update();

    tabErBa->layout->update();
    tabErBa->layout0->update();
    tabErBa->layout1->update();
    tabErBa->layout2->update();
    tabErBa->layout3->update();
}

dialogScrollArea::dialogScrollArea(QWidget * parent):QScrollArea(parent)
{
//nothing special here
}

void dialogScrollArea::keyPressEvent(QKeyEvent * k)
{
    switch (k->key())
    {
    case Qt::Key_Up:
    case Qt::Key_Down:
//qDebug() << "Parent KeyPressed=" << k->key();
        k->ignore();
        break;
    default:
//qDebug() << "Scroll Area KeyPressed=" << k->key();
        k->accept();
        QScrollArea::keyPressEvent(k);
        break;
    }
}

frmSetAppearance::frmSetAppearance(QWidget * parent):QDialog(parent)
{
//setFont(*stdFont);
    min_w=502;
    min_h=600;
    bar_w=bar_h=20;//15
    setWindowTitle(tr("QtGrace: Set Appearance"));
    setWindowIcon(QIcon(*GraceIcon));
    QVBoxLayout * layout=new QVBoxLayout;
    //layout->setMargin(0);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    flp=new frmSet_Appearance(this);
    connect(flp,SIGNAL(closeWish()),SLOT(close()));
    layout->addWidget(flp->menuBar);
    //layout->addWidget(flp);
    scroll=new dialogScrollArea(this);
    scroll->setWidget(flp);
    layout->addWidget(scroll);
    listSet=flp->listSet;
    setLayout(layout);
    //resize(min_w,min_h);
    resize(LastSize_FormSetAppearance);
}

frmSetAppearance::~frmSetAppearance()
{
    LastSize_FormSetAppearance=this->size();
}

void frmSetAppearance::init(void)
{
    flp->init();
}

void frmSetAppearance::ShowSetData_external(int graph_number,int set_number)
{
    flp->ShowSetData_external(graph_number,set_number);
}

void frmSetAppearance::doApply(void)
{
    flp->doApply();
}

void frmSetAppearance::doAccept(void)
{
    flp->doAccept();
}

void frmSetAppearance::doClose(void)
{
    hide();
}

void frmSetAppearance::resizeEvent(QResizeEvent * event)
{
//cout << "resize: " << event->size().width() << "x" << event->size().height() << " bar_w=" << bar_w << " bar_h=" << bar_h << endl;

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
#if defined(WINDOWS_SYSTEM) || defined(LINUX_SYSTEM)
n_size_h-=flp->menuBar->height();
#endif
setMinimumSize(0,0);
}
else
{
scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
setMinimumSize(min_w,min_h);
}
flp->resize(QSize(n_size_w,n_size_h));
}

void frmSetAppearance::showEvent(QShowEvent * event)
{
    (void)event;
int * selected=new int[2];
int nr_sel=0;
flp->listSet->get_selection(&nr_sel,&selected);
if (nr_sel<1)
{
flp->listSet->set_graph_number(cg,false);
//cout << "no set selected" << endl;
    if (number_of_sets(cg)>0)
    {
    delete[] selected;
    selected=new int[2];
    selected[0]=selected[1]=first_set(cg,1,0);
    ShowSetData_external(cg,selected[0]);
    //cout << "selecting G[" << cg << "].S[" << selected[0] << "]" << endl;
    }
}
delete[] selected;
}


GrTabMain::GrTabMain(QWidget * parent):QWidget(parent)
{
    int number=7;
    QString types[7];
    types[0]=tr("XY graph");
    types[1]=tr("XY chart");
    types[2]=tr("Polar graph");
    types[3]=tr("Smith chart (N/I)");
    types[4]=tr("Fixed");
    types[5]=tr("Pie chart");
    types[6]=tr("Polar graph v2");

    grpPres=new QGroupBox(tr("Presentation"),this);
    grpTitles=new QGroupBox(tr("Titles"),this);
    grpViewport=new QGroupBox(tr("Viewport"),this);
    grpDispOpt=new QGroupBox(tr("Display options"),this);

    selType=new StdSelector(grpPres,tr("Type:"),number,types);
    connect(selType,SIGNAL(currentIndexChanged(int)),SLOT(serviceGrTypeSel(int)));
    chkStackChart=new QCheckBox(tr("Stacked chart"),grpPres);
    selphi0=new stdLineEdit(this,tr("Angle for phi=0:"));
    selphi0->setToolTip(tr("Set the position of the phi=0-line (X=0) relative to the 3 o'clock-position in rad."));
    selphi0->setVisible(false);
    selroffset=new LineWidthSelector(this);
    selroffset->lblText->setText(tr("Radius-offset (percentage):"));
    selroffset->spnLineWidth->setDecimals(2);
    selroffset->spnLineWidth->setSingleStep(0.1);
    selroffset->spnLineWidth->setRange(0.0,1.0);
    selroffset->setToolTip(tr("Define the size of the region from -infinity to Ymin."));
    selroffset->setVisible(false);

    ledTitle=new stdLineEdit(grpTitles,tr("Title:"),true);
    ledTitle->setText("");
    ledSubtitle=new stdLineEdit(grpTitles,tr("Subtitle:"),true);
    ledSubtitle->setText("");

    ledCoords[0]=new stdLineEdit(grpTitles,tr("Xmin:"));
    ledCoords[1]=new stdLineEdit(grpTitles,tr("Xmax:"));
    ledCoords[2]=new stdLineEdit(grpTitles,tr("Ymin:"));
    ledCoords[3]=new stdLineEdit(grpTitles,tr("Ymax:"));

    chkDisplLegend=new QCheckBox(tr("Display legend"),grpDispOpt);
    chkDisplLegend->setChecked(TRUE);
    chkFlipXY=new QCheckBox(tr("Flip XY (N/I)"),grpDispOpt);

    layout1=new QGridLayout;
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout1->setSpacing(STD_SPACING);
    layout1->addWidget(selType,0,0);
    layout1->addWidget(chkStackChart,0,1);
    layout1->addWidget(selphi0,1,0);
    layout1->addWidget(selroffset,1,1);
    grpPres->setLayout(layout1);

    layout2=new QVBoxLayout;
    //layout2->setMargin(STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout2->addWidget(ledTitle);
    layout2->addWidget(ledSubtitle);
    grpTitles->setLayout(layout2);

    layout3=new QGridLayout;
    //layout3->setMargin(STD_MARGIN);
    layout3->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout3->addWidget(ledCoords[0],0,0);
    layout3->addWidget(ledCoords[1],0,1);
    layout3->addWidget(ledCoords[2],1,0);
    layout3->addWidget(ledCoords[3],1,1);
    grpViewport->setLayout(layout3);

    layout4=new QHBoxLayout;
    //layout4->setMargin(STD_MARGIN);
    layout4->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout4->addWidget(chkDisplLegend);
    layout4->addWidget(chkFlipXY);
    grpDispOpt->setLayout(layout4);

    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(grpPres);
    layout->addWidget(grpTitles);
    layout->addWidget(grpViewport);
    layout->addWidget(grpDispOpt);
    layout->addStretch(10);
    setLayout(layout);
}

void GrTabMain::serviceGrTypeSel(int nr)
{
    if (nr==6)
    {
        selphi0->setVisible(true);
        selroffset->setVisible(true);
    }
    else
    {
        selphi0->setVisible(false);
        selroffset->setVisible(false);
    }
}

GrTabTitles::GrTabTitles(QWidget * parent):QWidget(parent)
{
    grpTitle=new QGroupBox(tr("Title"),this);
    grpSubTitle=new QGroupBox(tr("Subtitle"),this);

    selTitleFont=new FontSelector(grpTitle);
    selAlign=new AlignmentSelector(grpTitle);
    selAlign->setToolTip(tr("Set the text-alignment for each text-line\n(only useful if your title has more than one line)"));
    sldTitleCharSize=new stdSlider(grpTitle,tr("Character size"),0,1000);
    sldTitleCharSize->setValue(150);
    selTitleColor=new ColorSelector(grpTitle);
    selTitleColor->setCurrentIndex(1);
    ledTitleShiftX=new stdLineEdit(this,tr("X-shift="));
    ledTitleShiftY=new stdLineEdit(this,tr("Y-shift="));

    selSubFont=new FontSelector(grpSubTitle);
    selSubAlign=new AlignmentSelector(grpTitle);
    selSubAlign->setToolTip(tr("Set the text-alignment for each text-line\n(only useful if your sub-title has more than one line)"));
    sldSubCharSize=new stdSlider(grpSubTitle,tr("Character size"),0,1000);
    sldSubCharSize->setValue(100);
    selSubColor=new ColorSelector(grpSubTitle);
    selSubColor->setCurrentIndex(1);
    ledSubTitleShiftX=new stdLineEdit(this,tr("X-shift="));
    ledSubTitleShiftY=new stdLineEdit(this,tr("Y-shift="));

    QWidget * emptyArea=new QWidget();
    emptyArea->setMaximumHeight(40);
    layout1=new QGridLayout;
    layout1->setSpacing(STD_SPACING);
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout1->addWidget(selTitleFont,0,0,1,1);
    layout1->addWidget(selAlign,0,1,1,1);
    layout1->addWidget(sldTitleCharSize,1,0,1,2);
    layout1->addWidget(selTitleColor,2,0,1,2);
    layout1->addWidget(ledTitleShiftX,3,0,1,1);
    layout1->addWidget(ledTitleShiftY,3,1,1,1);
    grpTitle->setLayout(layout1);
    layout2=new QGridLayout;
    layout2->setSpacing(STD_SPACING);
    //layout2->setMargin(STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout2->addWidget(selSubFont,0,0,1,1);
    layout2->addWidget(selSubAlign,0,1,1,1);
    layout2->addWidget(sldSubCharSize,1,0,1,2);
    layout2->addWidget(selSubColor,2,0,1,2);
    layout2->addWidget(ledSubTitleShiftX,3,0,1,1);
    layout2->addWidget(ledSubTitleShiftY,3,1,1,1);
    grpSubTitle->setLayout(layout2);
    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(grpTitle);
    layout->addWidget(grpSubTitle);
    layout->addWidget(emptyArea);
    layout->setStretch(0,1);
    layout->setStretch(1,1);
    layout->setStretch(2,2);
    setLayout(layout);
}

GrTabFrame::GrTabFrame(QWidget * parent):QWidget(parent)
{
    int number=6;
    QString types[6];
    types[0]=tr("Closed");
    types[1]=tr("Half open");
    types[2]=tr("Break top");
    types[3]=tr("Break bottom");
    types[4]=tr("Break left");
    types[5]=tr("Break right");
    grpFrameBox=new QGroupBox(tr("Frame box"),this);
    grpFrameFill=new QGroupBox(tr("Frame fill"),this);

    selFrameType=new StdSelector(grpFrameBox,tr("Frame type:"),number,types);
    selBoxColor=new ColorSelector(grpFrameBox);
    selFrameBoxPattern=new FillPatternSelector(grpFrameBox);
    selFrameBoxPattern->lblText->setText(tr("Pattern:"));
    selFrameBoxWidth=new LineWidthSelector(grpFrameBox);
    selFrameBoxWidth->lblText->setText(tr("Width:"));
    selFrameBoxStyle=new LineStyleSelector(grpFrameBox);
    selFrameBoxStyle->lblText->setText(tr("Style:"));

    selFillColor=new ColorSelector(grpFrameFill);
    selFrameFillPattern=new FillPatternSelector(grpFrameFill);
    selFrameFillPattern->lblText->setText(tr("Pattern:"));

    layout1=new QGridLayout;
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout1->addWidget(selFrameType,0,0,1,2);
    layout1->addWidget(selBoxColor,1,0);
    layout1->addWidget(selFrameBoxPattern,1,1);
    layout1->addWidget(selFrameBoxWidth,2,0);
    layout1->addWidget(selFrameBoxStyle,2,1);
    grpFrameBox->setLayout(layout1);
    layout2=new QHBoxLayout;
    //layout2->setMargin(STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout2->addWidget(selFillColor);
    layout2->addWidget(selFrameFillPattern);
    grpFrameFill->setLayout(layout2);
    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(grpFrameBox);
    layout->addWidget(grpFrameFill);
    layout->addStretch(10);
    setLayout(layout);

    selFillColor->setCurrentIndex(0);
    selBoxColor->setCurrentIndex(1);
    selFrameBoxWidth->setValue(1.0);
    selFrameBoxPattern->setCurrentIndex(1);
    selFrameBoxStyle->setCurrentIndex(1);
}

GrTabLegBox::GrTabLegBox(QWidget * parent):QWidget(parent)
{
    grpLocation=new QGroupBox(tr("Location"),this);
    grpFrameLine=new QGroupBox(tr("Frame line"),this);
    grpFrameFill=new QGroupBox(tr("Frame fill"),this);

    selLoc=new PositionSelector(grpLocation);
    selLoc->lblText->setText(tr("Locate in:"));
    ledX=new stdLineEdit(grpLocation,QString("X:"));
    ledY=new stdLineEdit(grpLocation,QString("Y:"));

    int nr=9;
    QString entries[9];
    int * i_entries=new int[9];
    entries[0]=QString("None");
    i_entries[0]=G_LB_ATTACH_NONE;
    entries[1]=QString("Left");
    i_entries[1]=G_LB_ATTACH_LEFT;
    entries[2]=QString("Right");
    i_entries[2]=G_LB_ATTACH_RIGHT;
    entries[3]=QString("Top");
    i_entries[3]=G_LB_ATTACH_TOP;
    entries[4]=QString("Bottom");
    i_entries[4]=G_LB_ATTACH_BOTTOM;
    entries[5]=QString("Left+Top");
    i_entries[5]=G_LB_ATTACH_LEFT | G_LB_ATTACH_TOP;
    entries[6]=QString("Left+Bottom");
    i_entries[6]=G_LB_ATTACH_LEFT | G_LB_ATTACH_BOTTOM;
    entries[7]=QString("Right+Top");
    i_entries[7]=G_LB_ATTACH_RIGHT | G_LB_ATTACH_TOP;
    entries[8]=QString("Right+Bottom");
    i_entries[8]=G_LB_ATTACH_RIGHT | G_LB_ATTACH_BOTTOM;
    selLegBoxAttachement=new StdSelector(this,tr("Auto-attach to:"),nr,entries);
    selLegBoxAttachement->setValues(i_entries);
    selLegBoxAttachement->setToolTip(tr("This setting fixes the legend box to an endge of the graph\neven if the graph is moved around the page or the size of the legend box changes"));
    connect(selLegBoxAttachement,SIGNAL(currentIndexChanged(int)),SLOT(autoAttachChanged(int)));
    delete[] i_entries;

    cmdAttachLeft=new QPushButton(tr("Left"),this);
    cmdAttachLeft->setToolTip(tr("Set the X-value of the left hand side of the legend box to the current left hand border of the graph.\nNo automatic change when the graph is repositioned."));
    connect(cmdAttachLeft,SIGNAL(clicked()),SLOT(doAttachLeft()));
    cmdAttachTop=new QPushButton(tr("Top"),this);
    cmdAttachTop->setToolTip(tr("Set the Y-value of the top of the legend box to the current top border of the graph.\nNo automatic change when the graph is repositioned."));
    connect(cmdAttachTop,SIGNAL(clicked()),SLOT(doAttachTop()));
    cmdAttachRight=new QPushButton(tr("Right"),this);
    cmdAttachRight->setToolTip(tr("Set the X-value of the right hand side of the legend box to the current right hand border of the graph.\nNo automatic change when the graph is repositioned."));
    connect(cmdAttachRight,SIGNAL(clicked()),SLOT(doAttachRight()));
    cmdAttachBottom=new QPushButton(tr("Bottom"),this);
    cmdAttachBottom->setToolTip(tr("Set the Y-value of the bottom side of the legend box to the current bottom border of the graph.\nNo automatic change when the graph is repositioned."));
    connect(cmdAttachBottom,SIGNAL(clicked()),SLOT(doAttachBottom()));

#ifdef MAC_SYSTEM
    QString mac_symb_cmd(QChar(0x2318));
    QString ti=tr("Move Legend (") + mac_symb_cmd + QString("+L)");
    cmdMoveLegend=new QPushButton(ti,this);
#else
    cmdMoveLegend=new QPushButton(tr("Move Legend (crtl+L)"),this);
#endif
    cmdMoveLegend->setToolTip(tr("Click here and then click on a legend to move it around with the mouse cursor.\nPress Esc or right-click to cancel."));
    connect(cmdMoveLegend,SIGNAL(clicked()),SLOT(doMoveLegend()));

    selFrameLineColor=new ColorSelector(grpFrameLine);
    selFrameLinePattern=new FillPatternSelector(grpFrameLine);
    selFrameLinePattern->lblText->setText(tr("Pattern:"));
    selFrameLineWidth=new LineWidthSelector(grpFrameLine);
    selFrameLineWidth->lblText->setText(tr("Width:"));
    selFrameLineStyle=new LineStyleSelector(grpFrameLine);
    selFrameLineStyle->lblText->setText(tr("Style:"));

    selFrameFillColor=new ColorSelector(grpFrameFill);
    selFrameFillPattern=new FillPatternSelector(grpFrameFill);
    selFrameFillPattern->lblText->setText(tr("Pattern:"));

    /*emptyArea=new QWidget(this);
    emptyArea->setMinimumHeight(30);*/

    layout1=new QGridLayout;
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout1->setSpacing(0);
    layout1->addWidget(selLoc,0,0,1,2);
    layout1->addWidget(ledX,1,0);
    layout1->addWidget(ledY,1,1);
    layout1->addWidget(selLegBoxAttachement,2,0,1,2);
    layout1->addWidget(cmdAttachLeft,3,0,1,1);
    layout1->addWidget(cmdAttachRight,3,1,1,1);
    layout1->addWidget(cmdAttachTop,4,0,1,1);
    layout1->addWidget(cmdAttachBottom,4,1,1,1);
    layout1->addWidget(cmdMoveLegend,5,0,1,2);
    grpLocation->setLayout(layout1);

    layout2=new QGridLayout;
    //layout2->setMargin(STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout2->addWidget(selFrameLineColor,0,0);
    layout2->addWidget(selFrameLinePattern,0,1);
    layout2->addWidget(selFrameLineWidth,1,0);
    layout2->addWidget(selFrameLineStyle,1,1);
    grpFrameLine->setLayout(layout2);
    layout3=new QHBoxLayout;
    //layout3->setMargin(STD_MARGIN);
    layout3->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout3->addWidget(selFrameFillColor);
    layout3->addWidget(selFrameFillPattern);
    grpFrameFill->setLayout(layout3);
    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(grpLocation);
    layout->addWidget(grpFrameLine);
    layout->addWidget(grpFrameFill);
    /*layout->addWidget(emptyArea);*/
    layout->addStretch(10);
    setLayout(layout);

    selLoc->setCurrentIndex(1);
    selFrameLineColor->setCurrentIndex(1);
    selFrameLinePattern->setCurrentIndex(1);
    selFrameLineStyle->setCurrentIndex(1);
    selFrameFillPattern->setCurrentIndex(1);
    selFrameLineWidth->setValue(1.0);
    connect(selLoc->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(viewCoordsChanged(int)));
}

void GrTabLegBox::viewCoordsChanged(int index)
{
int new_coords;
double ox,oy,nx,ny;
if (index==0) new_coords = COORD_WORLD;
else new_coords = COORD_VIEW;
ox=ledX->getDoubleValue();
oy=ledY->getDoubleValue();
    if( new_coords == COORD_WORLD )//new is world --> convert from viewport to world
    {
    view2world(ox,oy,&nx,&ny);
    }
    else// from world to viewport
    {
    world2view(ox,oy,&nx,&ny);
    }
ledX->setDoubleValue(sformat,nx);
ledY->setDoubleValue(sformat,ny);
}

void GrTabLegBox::autoAttachChanged(int index)
{
    (void)index;
int sel_val=selLegBoxAttachement->currentValue();
selLoc->setEnabled(true);
    if (sel_val==G_LB_ATTACH_NONE)//enable all
    {
        ledY->setEnabled(true);
        ledX->setEnabled(true);
        cmdAttachLeft->setEnabled(true);
        cmdAttachTop->setEnabled(true);
        cmdAttachRight->setEnabled(true);
        cmdAttachBottom->setEnabled(true);
        cmdMoveLegend->setEnabled(true);
    }
    else // some selections are useless
    {
        if (((sel_val & G_LB_ATTACH_LEFT) != 0) || ((sel_val & G_LB_ATTACH_RIGHT) != 0))
        {
        ledX->setEnabled(false);
        cmdAttachLeft->setEnabled(false);
        cmdAttachRight->setEnabled(false);
        }
        else
        {
        ledX->setEnabled(true);
        cmdAttachLeft->setEnabled(true);
        cmdAttachRight->setEnabled(true);
        }
        if (((sel_val & G_LB_ATTACH_TOP) != 0) || ((sel_val & G_LB_ATTACH_BOTTOM) != 0))
        {
        ledY->setEnabled(false);
        cmdAttachTop->setEnabled(false);
        cmdAttachBottom->setEnabled(false);
        }
        else
        {
        ledY->setEnabled(true);
        cmdAttachTop->setEnabled(true);
        cmdAttachBottom->setEnabled(true);
        }
        if (sel_val!=G_LB_ATTACH_LEFT && sel_val!=G_LB_ATTACH_RIGHT && sel_val!=G_LB_ATTACH_TOP && sel_val!=G_LB_ATTACH_BOTTOM)
        cmdMoveLegend->setEnabled(false);
        else
        cmdMoveLegend->setEnabled(true);
        /*cmdMoveLegend->setEnabled(false);*/
    }
}

void GrTabLegBox::doAttachLeft(void)
{
//cout << "attaching Legend to Left" << endl;
if (selLoc->currentIndex()!=1) selLoc->setCurrentIndex(1);//always change to viewport
double nx,ny;
position_leg_box(get_cg(),get_cg(),G_LB_ATTACH_LEFT,&nx,&ny);
ledX->setDoubleValue(sformat,nx);
emit(doApply());
}

void GrTabLegBox::doAttachTop(void)
{
//cout << "attaching Legend to Top" << endl;
if (selLoc->currentIndex()!=1) selLoc->setCurrentIndex(1);//always change to viewport
double nx,ny;
position_leg_box(get_cg(),get_cg(),G_LB_ATTACH_TOP,&nx,&ny);
ledY->setDoubleValue(sformat,ny);
emit(doApply());
}

void GrTabLegBox::doAttachBottom(void)
{
//cout << "attaching Legend to Bottom" << endl;
if (selLoc->currentIndex()!=1) selLoc->setCurrentIndex(1);//always change to viewport
double nx,ny;
position_leg_box(get_cg(),get_cg(),G_LB_ATTACH_BOTTOM,&nx,&ny);
ledY->setDoubleValue(sformat,ny);
emit(doApply());
}

void GrTabLegBox::doAttachRight(void)
{
//cout << "attaching Legend to Right" << endl;
if (selLoc->currentIndex()!=1) selLoc->setCurrentIndex(1);//always change to viewport
double nx,ny;
position_leg_box(get_cg(),get_cg(),G_LB_ATTACH_RIGHT,&nx,&ny);
ledX->setDoubleValue(sformat,nx);
emit(doApply());
}

void GrTabLegBox::doMoveLegend(void)
{
set_action(DO_NOTHING);
set_action(PLACE_LEGEND_1ST);
}

GrTabLegends::GrTabLegends(QWidget * parent):QWidget(parent)
{
    int number=1;
    char dummy[3];
    QString entries[10];
    for (int i=0;i<10;i++)
    {
        sprintf(dummy,"%d",i);
        entries[i]=QString(dummy);
    }

    grpTextProp=new QGroupBox(tr("Text properties"),this);
    grpPlacement=new QGroupBox(tr("Placement"),this);

    selTextFont=new FontSelector(grpTextProp);
    selAlign=new AlignmentSelector(grpTextProp);
    selAlign->setToolTip(tr("Set the text-alignment for each line in the legend\n(only useful for legend text with more than one line)"));
    sldTextSize=new stdSlider(grpTextProp,tr("Char size"),0,1000);
    sldTextSize->setValue(100);
    selTextColor=new ColorSelector(grpTextProp);

    number=6;
    selVGap=new StdSelector(grpPlacement,tr("V-gap:"),number,entries);
    selHGap=new StdSelector(grpPlacement,tr("H-gap:"),number,entries);
    number=9;
    selLineLength=new StdSelector(grpPlacement,tr("Legend line length:"),number,entries);
    chkPutRevOrder=new QCheckBox(tr("Put in reverse order"),grpPlacement);

    layout1=new QGridLayout;
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout1->setSpacing(STD_SPACING);
    layout1->addWidget(selTextFont,0,0);
    layout1->addWidget(selAlign,0,1);
    layout1->addWidget(sldTextSize,1,0,1,2);
    layout1->addWidget(selTextColor,2,0,1,2);
    grpTextProp->setLayout(layout1);
    layout2=new QGridLayout;
    //layout2->setMargin(STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout2->addWidget(selVGap,0,0);
    layout2->addWidget(selHGap,0,1);
    layout2->addWidget(selLineLength,1,0,1,2);
    layout2->addWidget(chkPutRevOrder,2,0,1,2);
    grpPlacement->setLayout(layout2);
    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(grpTextProp);
    layout->addWidget(grpPlacement);
    layout->addStretch(10);
    setLayout(layout);

    selVGap->setCurrentIndex(1);
    selHGap->setCurrentIndex(1);
    selLineLength->setCurrentIndex(4);
    selTextColor->setCurrentIndex(1);
}

GrTabSpecial::GrTabSpecial(QWidget * parent):QWidget(parent)
{
    grp2Dplusgraphs=new QGroupBox(tr("2D+ graphs"),this);
    grpXYcharts=new QGroupBox(tr("XY charts"),this);

    ledZnormal=new stdLineEdit(grp2Dplusgraphs,tr("Z normalization:"));
    ledZnormal->lenText->setText(QString("1"));
    selBarGap=new LineWidthSelector(grpXYcharts);
    selBarGap->lblText->setText(tr("Bar gap:"));
    selBarGap->spnLineWidth->setRange(-1.0,1.0);
    selBarGap->spnLineWidth->setDecimals(3);
    selBarGap->spnLineWidth->setSingleStep(0.005);

    layout1=new QHBoxLayout;
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout1->addWidget(ledZnormal);
    grp2Dplusgraphs->setLayout(layout1);
    layout2=new QHBoxLayout;
    //layout2->setMargin(STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout2->addWidget(selBarGap);
    grpXYcharts->setLayout(layout2);
    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(grp2Dplusgraphs);
    layout->addWidget(grpXYcharts);
    layout->addStretch(10);
    setLayout(layout);
}

frmGraph_App::frmGraph_App(QWidget * parent):QWidget(parent)
{
    frmOpenPara=NULL;
    frmSavePara=NULL;
    //setFont(*stdFont);
    //setWindowTitle(tr("QtGrace: Graph Appearance"));
    //setWindowIcon(QIcon(*GraceIcon));
    CreateActions();

    menuBar=new QMenuBar(this);
    mnuFile=new QMenu(tr("&File"));
    mnuFile->setTearOffEnabled(TRUE);
    mnuFile->addAction(actOpen);
    mnuFile->addAction(actSave);
    mnuFile->addSeparator();
    mnuFile->addAction(actClose);
    mnuEdit=new QMenu(tr("&Edit"));
    mnuEdit->setTearOffEnabled(TRUE);
    mnuEdit->addAction(actFocusTo);
    mnuEdit->addSeparator();
    mnuEdit->addAction(actHide);
    mnuEdit->addAction(actShow);
    mnuEdit->addAction(actDuplicate);
    mnuEdit->addAction(actKill);
    mnuEdit->addSeparator();
    mnuEdit->addAction(actCreateNew);
    mnuHelp=new QMenu(tr("&Help"));
    mnuHelp->setTearOffEnabled(TRUE);
    mnuHelp->addAction(actHelpOnContext);
    mnuHelp->addSeparator();
    mnuHelp->addAction(actHelpGraphApp);

    menuBar->addMenu(mnuFile);
    menuBar->addMenu(mnuEdit);
    menuBar->addSeparator();
    menuBar->addMenu(mnuHelp);
    lblTitle=new QLabel(tr("Graph:"),this);
    listGraph=new uniList(GRAPHLIST,this);
    listGraph->setMaximumHeight(100);
    connect(listGraph,SIGNAL(new_selection(int)),SLOT(newSelection(int)));

    tabs=new QTabWidget(this);
    tabMain=new GrTabMain(tabs);
    tabTitles=new GrTabTitles(tabs);
    tabFrame=new GrTabFrame(tabs);
    tabLegBox=new GrTabLegBox(tabs);
    tabLegends=new GrTabLegends(tabs);
    tabSpec=new GrTabSpecial(tabs);
    tabs->addTab(tabMain, tr("Main"));
    tabs->addTab(tabTitles, tr("Titles"));
    tabs->addTab(tabFrame, tr("Frame"));
    tabs->addTab(tabLegBox, tr("Leg. box"));
    tabs->addTab(tabLegends, tr("Legends"));
    tabs->addTab(tabSpec, tr("Special"));

    buttonGroup=new stdButtonGroup(this,true,true,false);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));
    connect(tabLegBox,SIGNAL(doApply()),this,SLOT(doApply()));
    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
#ifndef MAC_SYSTEM
    layout->addWidget(menuBar);
#endif
    layout->addWidget(lblTitle);
    layout->addWidget(listGraph);
    layout->addWidget(tabs);
    layout->addWidget(buttonGroup);
    setLayout(layout);
    //immediate updates stuff
    connect(tabMain->selType,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabMain->chkStackChart,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(tabMain->ledTitle->lenText,SIGNAL(returnPressed()),SLOT(update0()));
    connect(tabMain->ledSubtitle->lenText,SIGNAL(returnPressed()),SLOT(update0()));
    for (int i=0;i<4;i++)
        connect(tabMain->ledCoords[i]->lenText,SIGNAL(returnPressed()),SLOT(update0()));
    connect(tabMain->chkDisplLegend,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(tabMain->chkFlipXY,SIGNAL(toggled(bool)),SLOT(update3(bool)));

    connect(tabTitles->selTitleFont,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTitles->selAlign->cmbJustSelect2,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTitles->sldTitleCharSize,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(tabTitles->selTitleColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTitles->selTitleColor,SIGNAL(alphaChanged(int)),SLOT(update1(int)));
    connect(tabTitles->selSubFont,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTitles->selSubAlign->cmbJustSelect2,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTitles->sldSubCharSize,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(tabTitles->selSubColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTitles->selSubColor,SIGNAL(alphaChanged(int)),SLOT(update1(int)));

    connect(tabFrame->selFrameType,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabFrame->selBoxColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabFrame->selBoxColor,SIGNAL(alphaChanged(int)),SLOT(update1(int)));
    connect(tabFrame->selFrameBoxPattern,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabFrame->selFrameBoxWidth,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(tabFrame->selFrameBoxStyle,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabFrame->selFillColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabFrame->selFillColor,SIGNAL(alphaChanged(int)),SLOT(update1(int)));
    connect(tabFrame->selFrameFillPattern,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));

    connect(tabLegBox->selLoc->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLegBox->ledX->lenText,SIGNAL(returnPressed()),SLOT(update0()));
    connect(tabLegBox->ledY->lenText,SIGNAL(returnPressed()),SLOT(update0()));
    connect(tabLegBox->selFrameLineColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLegBox->selFrameLineColor,SIGNAL(alphaChanged(int)),SLOT(update1(int)));
    connect(tabLegBox->selFrameLinePattern,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLegBox->selFrameLineWidth,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(tabLegBox->selFrameLineStyle,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLegBox->selFrameFillColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLegBox->selFrameFillColor,SIGNAL(alphaChanged(int)),SLOT(update1(int)));
    connect(tabLegBox->selFrameFillPattern,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));

    connect(tabLegends->selTextFont,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLegends->selAlign->cmbJustSelect2,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLegends->sldTextSize,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(tabLegends->selTextColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLegends->selTextColor,SIGNAL(alphaChanged(int)),SLOT(update1(int)));
    connect(tabLegends->selVGap,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLegends->selHGap,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLegends->selLineLength,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLegends->chkPutRevOrder,SIGNAL(toggled(bool)),SLOT(update3(bool)));

    connect(tabSpec->ledZnormal->lenText,SIGNAL(returnPressed()),SLOT(update0()));
    connect(tabSpec->selBarGap,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));

    setMinimumWidth(480);
    //end immediate updates stuff
    init();
}

void frmGraph_App::newSelection(int i)
{
    (void)i;
int sel=1,*selection=new int[2];
listGraph->get_selection(&sel,&selection);
update_graphapp_items(sel,selection);
    /*if (i<=0)
    switch_current_graph(0);
    else
    switch_current_graph(i);*/
if (sel>0)
    if (selection[0]!=get_cg())
    {
        simple_draw_setting|=SIMPLE_DRAW_GRAPH_FOCUS;
        switch_current_graph(selection[0]);
    }
delete[] selection;
}

frmGraph_App::~frmGraph_App()
{}

void frmGraph_App::init(void)
{
    listGraph->update_number_of_entries();
    int n=1;
    int values[2]={0,0};

    int sel=1,*selection=new int[2];
    values[0]=selection[0]=get_cg();
    listGraph->set_new_selection(sel,selection);

    update_graphapp_items(n,values);
    delete[] selection;
}

void frmGraph_App::CreateActions(void)
{
    actClose = new QAction(tr("&Close"), this);
    actClose->setShortcut(tr("Esc"));
    actClose->setStatusTip(tr("Close this Window"));
    connect(actClose, SIGNAL(triggered()), this, SLOT(doClose()));
    actOpen= new QAction(tr("&Open..."), this);
    actOpen->setShortcut(tr("Ctrl+O"));
    connect(actOpen, SIGNAL(triggered()), this, SLOT(doOpen()));
    actSave= new QAction(tr("&Save"), this);
    actSave->setShortcut(tr("Ctrl+S"));
    connect(actSave, SIGNAL(triggered()), this, SLOT(doSave()));
    actHelpOnContext= new QAction(tr("On conte&xt"), this);
    actHelpOnContext->setShortcut(tr("Shift+F1"));
    connect(actHelpOnContext, SIGNAL(triggered()), this, SLOT(doHelpOnContext()));
    actHelpGraphApp= new QAction(tr("On &graph appearance"), this);
    connect(actHelpGraphApp, SIGNAL(triggered()), this, SLOT(doHelpGraphApp()));
    actFocusTo= new QAction(tr("&Focus to"), this);
    connect(actFocusTo, SIGNAL(triggered()), this, SLOT(doFocus()));
    actDuplicate= new QAction(tr("&Duplicate"), this);
    connect(actDuplicate, SIGNAL(triggered()), this, SLOT(doDuplicate()));
    actCreateNew= new QAction(tr("&Create new"), this);
    connect(actCreateNew, SIGNAL(triggered()), this, SLOT(doCreateNew()));
    actShow= new QAction(tr("&Show"), this);
    connect(actShow, SIGNAL(triggered()), this, SLOT(doShow()));
    actHide= new QAction(tr("&Hide"), this);
    connect(actHide, SIGNAL(triggered()), this, SLOT(doHide()));
    actKill= new QAction(tr("&Kill"), this);
    connect(actKill, SIGNAL(triggered()), this, SLOT(doKill()));
}

void frmGraph_App::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmGraph_App::doClose(void)
{
//parentWidget()->hide();
    emit(closeWish());
}

void frmGraph_App::doApply(void)
{
    (void)graphapp_aac_cb();
}

void frmGraph_App::doOpen(void)
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
    if (frmOpenPara==NULL)
    {
        frmOpenPara=new frmIOForm(READ_PARAMETERS,this);
        connect(frmOpenPara,SIGNAL(newFileSelectedForIO(int,QString,bool,bool,bool)),SLOT(IOrequested(int,QString,bool,bool,bool)));
    }
    frmOpenPara->init(openfilename.toLocal8Bit().data());
    frmOpenPara->show();
    frmOpenPara->raise();
}
else
{
mainWin->UseOperatingSystemFileDialog(READ_PARAMETERS,tr("QtGrace: Read parameters"),openfilename,tr("Parameter files (*.par);;All files (*)"));
}
}

void frmGraph_App::doSave(void)
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
    if (frmSavePara==NULL)
    {
        frmSavePara=new frmIOForm(WRITE_PARAMETERS,this);
        connect(frmSavePara,SIGNAL(newFileSelectedForIO(int,QString,bool,bool,bool)),SLOT(IOrequested(int,QString,bool,bool,bool)));
    }
    frmSavePara->init(openfilename.toLocal8Bit().data());
    frmSavePara->show();
    frmSavePara->raise();
}
else
{
mainWin->UseOperatingSystemFileDialog(WRITE_PARAMETERS,tr("QtGrace: Write parameters"),openfilename,tr("Parameter files (*.par);;All files (*)"));
}
}

void frmGraph_App::doHelpOnContext(void)
{
    mainWin->setCursor(QCursor(Qt::WhatsThisCursor));
}

void frmGraph_App::doHelpGraphApp(void)
{
    HelpCB("doc/UsersGuide.html#graph-appearance");
}

void frmGraph_App::doPrepare(void)
{
    listGraph->get_selection(&listGraph->popupMenu2->number_of_selected_graphs,&listGraph->popupMenu2->selected_graphs);
    sort(listGraph->popupMenu2->number_of_selected_graphs,listGraph->popupMenu2->selected_graphs);
    listGraph->popupMenu2->update_menu_content();
}

void frmGraph_App::doHide(void)
{
    doPrepare();
    listGraph->popupMenu2->doHide();
}

void frmGraph_App::doShow(void)
{
    doPrepare();
    listGraph->popupMenu2->doShow();
}

void frmGraph_App::doKill(void)
{
    doPrepare();
    listGraph->popupMenu2->doKill();
}

void frmGraph_App::doDuplicate(void)
{
    doPrepare();
    listGraph->popupMenu2->doDuplicate();
}

void frmGraph_App::doFocus(void)
{
    doPrepare();
    if (listGraph->popupMenu2->number_of_selected_graphs==1)
    {
        listGraph->popupMenu2->selected_no=listGraph->popupMenu2->selected_graphs[0];
        listGraph->popupMenu2->doFocusTo();
    }
}

void frmGraph_App::doCreateNew(void)
{
    doPrepare();
    listGraph->popupMenu2->doCreateNew();
}

int frmGraph_App::graphapp_aac_cb(void)
{
    ApplyError=false;
    char dummy[MAX_STRING_LENGTH];
    int j, gno, n, *values=new int[5];
    view v,v2;
    labels labs;
    labels labs2;
    framep f,f2;
    legend l,l2;
    int graphtype;
    int stacked;
    double bargap;
    double znorm;
    int flipxy;
    double new_roffset,new_phi0;
/*  char buf[1024];
 *  int flipxy;
 */
    graphtype = tabMain->selType->currentIndex();//GetChoice(graph_type_choice_item);
    /*v.xv1=atof(tabMain->ledCoords[0]->text().toLocal8Bit());
v.xv2=atof(tabMain->ledCoords[1]->text().toLocal8Bit());
v.yv1=atof(tabMain->ledCoords[2]->text().toLocal8Bit());
v.yv2=atof(tabMain->ledCoords[3]->text().toLocal8Bit());*/

    xv_evalexpr(tabMain->ledCoords[0], &v.xv1);
    xv_evalexpr(tabMain->ledCoords[1], &v.xv2);
    xv_evalexpr(tabMain->ledCoords[2], &v.yv1);
    xv_evalexpr(tabMain->ledCoords[3], &v.yv2);
    if (isvalid_viewport(v) == FALSE)
    {
        errmsg(tr("Invalid viewport coordinates").toLocal8Bit().constData());
        ApplyError=true;
        return RETURN_FAILURE;
    }

    set_default_string(&labs.title);
    //set_plotstr_string(&labs.title, GetTextString(label_title_text_item));

    tabMain->ledTitle->DynSetMemoryToText(labs.title.s_plotstring,labs.title.alt_plotstring);
    //strcpy(buf,tabMain->ledTitle->text().toLocal8Bit());
    //set_plotstr_string(&labs.title,buf);
    ///setting plotstring

    labs.title.charsize = tabTitles->sldTitleCharSize->value()/100.0;//GetCharSizeChoice(title_size_item);
    labs.title.color = tabTitles->selTitleColor->currentIndex();//GetOptionChoice(title_color_item);
    labs.title.alpha = tabTitles->selTitleColor->alpha();
    labs.title.font = tabTitles->selTitleFont->currentIndex();//GetOptionChoice(title_font_item);
    labs.title.align = tabTitles->selAlign->currentValue();

    xv_evalexpr(tabTitles->ledTitleShiftX,&(labs.shift_title.x));
    xv_evalexpr(tabTitles->ledTitleShiftY,&(labs.shift_title.y));
    xv_evalexpr(tabTitles->ledSubTitleShiftX,&(labs.shift_subtitle.x));
    xv_evalexpr(tabTitles->ledSubTitleShiftY,&(labs.shift_subtitle.y));

    set_default_string(&labs.stitle);
    //set_plotstr_string(&labs.stitle, GetTextString(label_subtitle_text_item));

    tabMain->ledSubtitle->DynSetMemoryToText(labs.stitle.s_plotstring,labs.stitle.alt_plotstring);
    //strcpy(buf,tabMain->ledSubtitle->text().toLocal8Bit());
    //set_plotstr_string(&labs.stitle,buf);
    ///setting plotstring

    labs.stitle.charsize = tabTitles->sldSubCharSize->value()/100.0;//GetCharSizeChoice(stitle_size_item);
    labs.stitle.color = tabTitles->selSubColor->currentIndex();//GetOptionChoice(stitle_color_item);
    labs.stitle.alpha = tabTitles->selSubColor->alpha();
    labs.stitle.font = tabTitles->selSubFont->currentIndex();//GetOptionChoice(stitle_font_item);
    labs.stitle.align = tabTitles->selSubAlign->currentValue();

    f.type = tabFrame->selFrameType->currentIndex();//GetChoice(frame_framestyle_choice_item);
    f.pen.color = tabFrame->selBoxColor->currentIndex();//GetOptionChoice(frame_color_choice_item);
    f.pen.alpha = tabFrame->selBoxColor->alpha();
    f.pen.pattern = tabFrame->selFrameBoxPattern->currentIndex();//GetOptionChoice(frame_pattern_choice_item);
    f.linew = tabFrame->selFrameBoxWidth->value();//GetSpinChoice(frame_linew_choice_item);
    f.lines = tabFrame->selFrameBoxStyle->currentIndex();//GetOptionChoice(frame_lines_choice_item);
    f.fillpen.color = tabFrame->selFillColor->currentIndex();//GetOptionChoice(frame_fillcolor_choice_item);
    f.fillpen.alpha = tabFrame->selFillColor->alpha();
    f.fillpen.pattern = tabFrame->selFrameFillPattern->currentIndex();//GetOptionChoice(frame_fillpattern_choice_item);

    l.charsize = tabLegends->sldTextSize->value()/100.0;//GetCharSizeChoice(legend_charsize_item);
    l.active = (int)tabMain->chkDisplLegend->isChecked();//GetToggleButtonState(toggle_legends_item);
    l.vgap = tabLegends->selVGap->currentIndex();//GetChoice(legends_vgap_item);
    l.hgap = tabLegends->selHGap->currentIndex();//GetChoice(legends_hgap_item);
    l.len = tabLegends->selLineLength->currentIndex();//GetChoice(legends_len_item);
    l.invert = (int)tabLegends->chkPutRevOrder->isChecked();//GetToggleButtonState(legends_invert_item);
    l.loctype = tabLegBox->selLoc->currentIndex()==1? COORD_VIEW : COORD_WORLD;//GetChoice(toggle_legendloc_item)
    xv_evalexpr(tabLegBox->ledX, &l.legx);
    xv_evalexpr(tabLegBox->ledY, &l.legy);
    l.autoattach=tabLegBox->selLegBoxAttachement->currentValue();
    if (l.autoattach!=G_LB_ATTACH_NONE) set_action(DO_NOTHING);
    /*l.legx=atof(tabLegBox->ledX->text().toLocal8Bit());
    l.legy=atof(tabLegBox->ledY->text().toLocal8Bit());*/
    l.font = tabLegends->selTextFont->currentIndex();//GetOptionChoice(legend_font_item);
    l.align = tabLegends->selAlign->currentValue();
    l.color = tabLegends->selTextColor->currentIndex();//GetOptionChoice(legend_color_item);
    l.alpha = tabLegends->selTextColor->alpha();
    l.boxfillpen.color = tabLegBox->selFrameFillColor->currentIndex();//GetOptionChoice(legend_boxfillcolor_item);
    l.boxfillpen.alpha = tabLegBox->selFrameFillColor->alpha();
    l.boxfillpen.pattern = tabLegBox->selFrameFillPattern->currentIndex();//GetOptionChoice(legend_boxfillpat_item);
    l.boxpen.color = tabLegBox->selFrameLineColor->currentIndex();//GetOptionChoice(legend_boxcolor_item);
    l.boxpen.alpha = tabLegBox->selFrameLineColor->alpha();
    l.boxpen.pattern = tabLegBox->selFrameLinePattern->currentIndex();//GetOptionChoice(legend_boxpattern_item);
    l.boxlinew = tabLegBox->selFrameLineWidth->value();//GetSpinChoice(legend_boxlinew_item);
    l.boxlines = tabLegBox->selFrameLineStyle->currentIndex();//GetOptionChoice(legend_boxlines_item);

    stacked = (bool)tabMain->chkStackChart->isChecked();//GetToggleButtonState(stacked_item);
    bargap = tabSpec->selBarGap->value();//GetSpinChoice(bargap_item);

    xv_evalexpr(tabSpec->ledZnormal, &znorm);
    flipxy = (int)tabMain->chkFlipXY->isChecked();

/*
 *     flipxy = GetToggleButtonState(graph_flipxy_item);
 */

    //n = GetListChoices(graph_selector, &values);
    listGraph->get_selection(&n,&values);

    if (n<=0)
    {
    errmsg(tr("No graph selected!").toLocal8Bit().constData());
    ApplyError=true;
    return RETURN_FAILURE;
    }

    if (GlobalInhibitor==false)
        SaveGraphStatesPrevious(n,values,UNDO_APPEARANCE);

    ListOfChanges.clear();
    ListOfOldStates.clear();

    get_graph_viewport(values[0],&v2);
    get_graph_legend(values[0],&l2);
    get_graph_labels(values[0],&labs2);
    get_graph_framep(values[0],&f2);

    xv_evalexpr(tabMain->selphi0, &new_phi0);
    new_roffset=tabMain->selroffset->value();

    if (graphtype!=g[values[0]].type)
    {
        sprintf(dummy,"g%d type %s",values[0],graph_types(graphtype));
        ListOfChanges << QString(dummy);
        sprintf(dummy,"g%d type %s",values[0],graph_types(g[values[0]].type));
        ListOfOldStates << QString(dummy);
    }
    if (stacked!=g[values[0]].stacked)
    {
        sprintf(dummy,"g%d stacked %s",values[0],stacked?"true":"false");
        ListOfChanges << QString(dummy);
        sprintf(dummy,"g%d stacked %s",values[0],g[values[0]].stacked?"true":"false");
        ListOfOldStates << QString(dummy);
    }
    if (bargap!=g[values[0]].bargap)
    {
        sprintf(dummy,"g%d bar hgap %f",values[0],bargap);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"g%d bar hgap %f",values[0],g[values[0]].bargap);
        ListOfOldStates << QString(dummy);
    }

    sprintf(dummy,"with g%d",values[0]);
    ListOfChanges << QString(dummy);
    ListOfOldStates << QString(dummy);

    if (znorm!=g[values[0]].znorm)
    {
        sprintf(dummy,"    znorm %f",znorm);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    znorm %f",g[values[0]].znorm);
        ListOfOldStates << QString(dummy);
    }
    if (v.xv1 != v2.xv1 || v.xv2 != v2.xv2 || v.yv1 != v2.yv1 || v.yv2 != v2.yv2)//v2==original
    {
        sprintf(dummy,"    view %f, %f, %f, %f",v.xv1,v.yv1,v.xv2,v.yv2);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    view %f, %f, %f, %f",v2.xv1,v2.yv1,v2.xv2,v2.yv2);
        ListOfOldStates << QString(dummy);
    }
    if (labs.stitle.charsize!=labs2.stitle.charsize)
    {
        sprintf(dummy,"    subtitle size %f",labs.stitle.charsize);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    subtitle size %f",labs2.stitle.charsize);
        ListOfOldStates << QString(dummy);
    }
    if (labs.stitle.font!=labs2.stitle.font)
    {
        sprintf(dummy,"    subtitle font %d",labs.stitle.font);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    subtitle font %d",labs2.stitle.font);
        ListOfOldStates << QString(dummy);
    }
    if (labs.stitle.color!=labs2.stitle.color)
    {
        sprintf(dummy,"    subtitle color %d",labs.stitle.color);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    subtitle color %d",labs2.stitle.color);
        ListOfOldStates << QString(dummy);
    }
    if (labs.title.charsize!=labs2.title.charsize)
    {
        sprintf(dummy,"    title size %f",labs.title.charsize);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    title size %f",labs2.title.charsize);
        ListOfOldStates << QString(dummy);
    }
    if (labs.title.font!=labs2.title.font)
    {
        sprintf(dummy,"    title font %d",labs.title.font);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    title font %d",labs2.title.font);
        ListOfOldStates << QString(dummy);
    }
    if (labs.title.color!=labs2.title.color)
    {
        sprintf(dummy,"    title color %d",labs.title.color);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    title color %d",labs2.title.color);
        ListOfOldStates << QString(dummy);
    }
    if (!(labs.title.s_plotstring==NULL && labs2.title.s_plotstring==NULL))
        if ((labs.title.s_plotstring!=NULL && labs2.title.s_plotstring==NULL) || (labs.title.s_plotstring==NULL && labs2.title.s_plotstring!=NULL) || strcmp(labs.title.s_plotstring,labs2.title.s_plotstring))
        {
            sprintf(dummy,"    title \"%s\"",labs.title.s_plotstring);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    title \"%s\"",labs2.title.s_plotstring);
            ListOfOldStates << QString(dummy);
        }
    if (!(labs.stitle.s_plotstring==NULL && labs2.stitle.s_plotstring==NULL))
        if ((labs.stitle.s_plotstring!=NULL && labs2.stitle.s_plotstring==NULL) || (labs.stitle.s_plotstring==NULL && labs2.stitle.s_plotstring!=NULL) || strcmp(labs.stitle.s_plotstring,labs2.stitle.s_plotstring))
        {
            sprintf(dummy,"    subtitle \"%s\"",labs.stitle.s_plotstring);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    subtitle \"%s\"",labs2.stitle.s_plotstring);
            ListOfOldStates << QString(dummy);
        }
    if (f.type!=f2.type)
    {
        sprintf(dummy,"    frame type %d",f.type);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    frame type %d",f2.type);
        ListOfOldStates << QString(dummy);
    }
    if (f.pen.color!=f2.pen.color)
    {
        sprintf(dummy,"    frame color %d",f.pen.color);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    frame color %d",f2.pen.color);
        ListOfOldStates << QString(dummy);
    }
    if (f.pen.pattern!=f2.pen.pattern)
    {
        sprintf(dummy,"    frame pattern %d",f.pen.pattern);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    frame pattern %d",f2.pen.pattern);
        ListOfOldStates << QString(dummy);
    }
    if (f.linew!=f2.linew)
    {
        sprintf(dummy,"    frame linewidth %f",f.linew);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    frame linewidth %f",f2.linew);
        ListOfOldStates << QString(dummy);
    }
    if (f.lines!=f2.lines)
    {
        sprintf(dummy,"    frame linestyle %d",f.lines);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    frame linestyle %d",f2.lines);
        ListOfOldStates << QString(dummy);
    }
    if (f.fillpen.color!=f2.fillpen.color)
    {
        sprintf(dummy,"    frame background color %d",f.fillpen.color);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    frame background color %d",f2.fillpen.color);
        ListOfOldStates << QString(dummy);
    }
    if (f.fillpen.pattern!=f2.fillpen.pattern)
    {
        sprintf(dummy,"    frame background pattern %d",f.fillpen.pattern);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    frame background pattern %d",f2.fillpen.pattern);
        ListOfOldStates << QString(dummy);
    }
    if (l.charsize!=l2.charsize)
    {
        sprintf(dummy,"    legend char size %f",l.charsize);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    legend char size %f",l2.charsize);
        ListOfOldStates << QString(dummy);
    }
    if (l.active!=l2.active)
    {
        sprintf(dummy,"    legend %s",l.active?"on":"off");
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    legend %s",l2.active?"on":"off");
        ListOfOldStates << QString(dummy);
    }
    if (l.vgap!=l2.vgap)
    {
        sprintf(dummy,"    legend vgap %d",l.vgap);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    legend vgap %d",l2.vgap);
        ListOfOldStates << QString(dummy);
    }
    if (l.hgap!=l2.hgap)
    {
        sprintf(dummy,"    legend hgap %d",l.hgap);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    legend hgap %d",l2.hgap);
        ListOfOldStates << QString(dummy);
    }
    if (l.len!=l2.len)
    {
        sprintf(dummy,"    legend length %d",l.len);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    legend length %d",l2.len);
        ListOfOldStates << QString(dummy);
    }
    if (l.invert!=l2.invert)
    {
        sprintf(dummy,"    legend invert %s",l.invert?"true":"false");
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    legend invert %s",l2.invert?"true":"false");
        ListOfOldStates << QString(dummy);
    }
    if (l.loctype!=l2.loctype)
    {
        sprintf(dummy,"    legend loctype %s",l.loctype?"world":"view");
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    legend loctype %s",l2.loctype?"world":"view");
        ListOfOldStates << QString(dummy);
    }
    if (l.legx!=l2.legx || l.legy!=l2.legy)
    {
        sprintf(dummy,"    legend %f, %f",l.legx,l.legy);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    legend %f, %f",l2.legx,l2.legy);
        ListOfOldStates << QString(dummy);
    }
    if (l.font!=l2.font)
    {
        sprintf(dummy,"    legend font %d",l.font);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    legend font %d",l2.font);
        ListOfOldStates << QString(dummy);
    }
    if (l.color!=l2.color)
    {
        sprintf(dummy,"    legend color %d",l.color);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    legend color %d",l2.color);
        ListOfOldStates << QString(dummy);
    }
    if (l.boxfillpen.color!=l2.boxfillpen.color)
    {
        sprintf(dummy,"    legend box fill color %d",l.boxfillpen.color);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    legend box fill color %d",l2.boxfillpen.color);
        ListOfOldStates << QString(dummy);
    }
    if (l.boxfillpen.pattern!=l2.boxfillpen.pattern)
    {
        sprintf(dummy,"    legend box fill pattern %d",l.boxfillpen.pattern);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    legend box fill pattern %d",l2.boxfillpen.pattern);
        ListOfOldStates << QString(dummy);
    }
    if (l.boxpen.color!=l2.boxpen.color)
    {
        sprintf(dummy,"    legend box color %d",l.boxpen.color);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    legend box color %d",l2.boxpen.color);
        ListOfOldStates << QString(dummy);
    }
    if (l.boxpen.pattern!=l2.boxpen.pattern)
    {
        sprintf(dummy,"    legend box pattern %d",l.boxpen.pattern);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    legend box pattern %d",l2.boxpen.pattern);
        ListOfOldStates << QString(dummy);
    }
    if (l.boxlinew!=l2.boxlinew)
    {
        sprintf(dummy,"    legend box linewidth %f",l.boxlinew);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    legend box linewidth %f",l2.boxlinew);
        ListOfOldStates << QString(dummy);
    }
    if (l.boxlines!=l2.boxlines)
    {
        sprintf(dummy,"    legend box linestyle %d",l.boxlines);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    legend box linestyle %d",l2.boxlines);
        ListOfOldStates << QString(dummy);
    }
    if (labs.shift_title.x!=labs2.shift_title.x || labs.shift_title.y!=labs2.shift_title.y)
    {
        sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: TITLE_SHIFT G %d %g %g\n",values[0],labs.shift_title.x,labs.shift_title.y);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: TITLE_SHIFT G %d %g %g\n",values[0],labs2.shift_title.x,labs2.shift_title.y);
        ListOfOldStates << QString(dummy);
    }
    if (labs.shift_subtitle.x!=labs2.shift_subtitle.x || labs.shift_subtitle.y!=labs2.shift_subtitle.y)
    {
        sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: SUBTITLE_SHIFT G %d %g %g\n",values[0],labs.shift_subtitle.x,labs.shift_subtitle.y);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: SUBTITLE_SHIFT G %d %g %g\n",values[0],labs2.shift_subtitle.x,labs2.shift_subtitle.y);
        ListOfOldStates << QString(dummy);
    }
    if (labs.title.align!=labs2.title.align || labs.stitle.align!=labs2.stitle.align)
    {
        sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: ALIGN_TITLE G %d %d %d\n",values[0],labs.title.align,labs.stitle.align);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: ALIGN_TITLE G %d %d %d\n",values[0],labs2.title.align,labs2.stitle.align);
        ListOfOldStates << QString(dummy);
    }
    if (l.align!=l2.align)
    {
        sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: ALIGN_LEGEND G %d %d\n",values[0],l.align);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: ALIGN_LEGEND G %d %d\n",values[0],l2.align);
        ListOfOldStates << QString(dummy);
    }
    if (g[values[0]].phi0!=new_phi0 || g[values[0]].roffset!=new_roffset)
    {
        sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: POLAR_V2 G %d %g %g\n",values[0],new_phi0,new_roffset);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: POLAR_V2 G %d %g %g\n",values[0],g[values[0]].phi0,g[values[0]].roffset);
        ListOfOldStates << QString(dummy);
    }
    if (labs.title.alpha != labs2.title.alpha || labs.stitle.alpha != labs2.stitle.alpha || f.pen.alpha != f2.pen.alpha || f.fillpen.alpha != f2.fillpen.alpha || l.alpha != l2.alpha || l.boxpen.alpha != l2.boxpen.alpha || l.boxfillpen.alpha != l2.boxfillpen.alpha)
    {
        sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: GRAPH_ALPHA G %d %s\n",values[0],create_list_of_arguments(7, labs.title.alpha,labs.stitle.alpha,f.pen.alpha,f.fillpen.alpha,l.alpha,l.boxpen.alpha,l.boxfillpen.alpha));
        ListOfChanges << QString(dummy);
        sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: GRAPH_ALPHA G %d %s\n",values[0],create_list_of_arguments(7, labs2.title.alpha,labs2.stitle.alpha,f2.pen.alpha,f2.fillpen.alpha,l2.alpha,l2.boxpen.alpha,l2.boxfillpen.alpha));
        ListOfOldStates << QString(dummy);
    }
    /*
for (int i=0;i<ListOfChanges.size();i++)
cout << ListOfChanges.at(i).toLocal8Bit().constData() << endl;
*/
/*///I think is command is here a second time - should only be once
    for (j = 0; j < n; j++)
        set_graph_labels(values[j], &labs);//title and subtitle
*/
    if (GlobalInhibitor==false)
    {
        for (j = 0; j < n; j++)
        {
            gno = values[j];
            if (is_valid_gno(gno))
            {
                set_graph_type(gno, graphtype);
                set_graph_stacked(gno, stacked);
                set_graph_bargap(gno, bargap);
                set_graph_znorm(gno, znorm);
                set_graph_viewport(gno, v);
                set_graph_labels(gno, &labs);
                set_graph_framep(gno, &f);
                l.autoattachG=gno;
                set_graph_legend(gno, &l);
                g[gno].xyflip = flipxy;
                g[gno].phi0=new_phi0;
                g[gno].roffset=new_roffset;
/*
 *             g[gno].xyflip = flipxy;
 */
            }
        }
        GraphsModified(n,values,UNDO_APPEARANCE);
        ListOfChanges.clear();
        ListOfOldStates.clear();
        mainWin->mainArea->completeRedraw();
    }
    delete[] values;
    return RETURN_SUCCESS;
}

void frmGraph_App::redisplayContents(void)
{
    if (DecimalPointToUse=='.')
    {
        tabSpec->selBarGap->spnLineWidth->setLocale(*dot_locale);
        tabFrame->selFrameBoxWidth->spnLineWidth->setLocale(*dot_locale);
        tabLegBox->selFrameLineWidth->spnLineWidth->setLocale(*dot_locale);
        tabMain->selroffset->spnLineWidth->setLocale(*dot_locale);
    }
    else
    {
        tabSpec->selBarGap->spnLineWidth->setLocale(*comma_locale);
        tabFrame->selFrameBoxWidth->spnLineWidth->setLocale(*comma_locale);
        tabLegBox->selFrameLineWidth->spnLineWidth->setLocale(*comma_locale);
        tabMain->selroffset->spnLineWidth->setLocale(*comma_locale);
    }
    //if (OldDecimalPoint==DecimalPointToUse) return;
    tabMain->selphi0->ReplaceNumberContents();
    for (int i=0;i<4;i++)
        tabMain->ledCoords[i]->ReplaceNumberContents();
    tabFrame->selFrameBoxWidth->setValue(tabFrame->selFrameBoxWidth->value());
    tabLegBox->ledX->ReplaceNumberContents();
    tabLegBox->ledY->ReplaceNumberContents();
    tabLegBox->selFrameLineWidth->setValue(tabLegBox->selFrameLineWidth->value());
    tabSpec->ledZnormal->ReplaceNumberContents();
    tabSpec->selBarGap->setValue(tabSpec->selBarGap->value());

    tabMain->layout->update();
    tabMain->layout1->update();
    tabMain->layout2->update();
    tabMain->layout3->update();
    tabMain->layout4->update();
    tabTitles->layout->update();
    tabTitles->layout1->update();
    tabTitles->layout2->update();
    tabFrame->layout->update();
    tabFrame->layout1->update();
    tabFrame->layout2->update();
    tabLegBox->layout->update();
    tabLegBox->layout1->update();
    tabLegBox->layout2->update();
    tabLegBox->layout3->update();
    tabLegends->layout->update();
    tabLegends->layout1->update();
    tabLegends->layout2->update();
    tabSpec->layout->update();
    tabSpec->layout1->update();
    tabSpec->layout2->update();
}

void frmGraph_App::update_graphapp_items(int n, int *values)
{
    bool sav_imm_upd=immediateUpdate;
    int gno;
    labels labs;

    if (n != 1)
    {
        return;
    }
    else
    {
        gno = values[0];
    }

    if (is_valid_gno(gno) != TRUE)
    {
        return;
    }

    immediateUpdate=false;
    updateRunning=true;

    redisplayContents();//to switch the decimal point (if necessary)

    update_view(gno);
    update_frame_items(gno);
    updatelegends(gno);
    get_graph_labels(gno, &labs);

    tabMain->selType->setCurrentIndex(get_graph_type(gno));
    tabMain->selphi0->setDoubleValue(g[gno].phi0);
    tabMain->selroffset->setValue(g[gno].roffset);
    tabSpec->selBarGap->setValue(get_graph_bargap(gno));
    tabMain->chkStackChart->setChecked((bool)is_graph_stacked(gno));
    tabSpec->ledZnormal->setDoubleValue("%g",get_graph_znorm(gno));
    tabMain->ledTitle->SetTextToMemory(g[gno].labs.title.s_plotstring,g[gno].labs.title.alt_plotstring);
    tabMain->ledSubtitle->SetTextToMemory(g[gno].labs.stitle.s_plotstring,g[gno].labs.stitle.alt_plotstring);
    tabTitles->sldTitleCharSize->setValue((int)rint_2(labs.title.charsize*100.0));
    tabTitles->sldSubCharSize->setValue((int)rint_2(labs.stitle.charsize*100.0));
    tabTitles->selTitleColor->setCurrentIndex(labs.title.color);
    tabTitles->selSubColor->setCurrentIndex(labs.stitle.color);
    tabTitles->selTitleColor->setAlpha(labs.title.alpha);
    tabTitles->selSubColor->setAlpha(labs.stitle.alpha);
    tabTitles->selTitleFont->setCurrentIndex(labs.title.font);
    tabTitles->selAlign->setCurrentValue(labs.title.align);
    tabTitles->selSubFont->setCurrentIndex(labs.stitle.font);
    tabTitles->selSubAlign->setCurrentValue(labs.stitle.align);
    tabTitles->ledTitleShiftX->setDoubleValue(labs.shift_title.x);
    tabTitles->ledTitleShiftY->setDoubleValue(labs.shift_title.y);
    tabTitles->ledSubTitleShiftX->setDoubleValue(labs.shift_subtitle.x);
    tabTitles->ledSubTitleShiftY->setDoubleValue(labs.shift_subtitle.y);
    tabMain->chkFlipXY->setChecked(bool(g[gno].xyflip));
/*
 *         SetToggleButtonState(graph_flipxy_item, g[gno].xyflip);
 */
    immediateUpdate=sav_imm_upd;
    updateRunning=false;
}

/*
 * Viewport update
 */
void frmGraph_App::update_view(int gno)
{
    view v;
    get_graph_viewport(gno, &v);
    tabMain->ledCoords[0]->setDoubleValue("%.9g",v.xv1);
    tabMain->ledCoords[1]->setDoubleValue("%.9g",v.xv2);
    tabMain->ledCoords[2]->setDoubleValue("%.9g",v.yv1);
    tabMain->ledCoords[3]->setDoubleValue("%.9g",v.yv2);
}

/*
 * legend popup
 */
void frmGraph_App::updatelegends(int gno)
{
    legend l;
    get_graph_legend(gno, &l);
    tabLegends->sldTextSize->setValue((int)rint_2(l.charsize*100.0));
    tabMain->chkDisplLegend->setChecked(l.active);
    tabLegBox->ledX->setDoubleValue("%.9g",l.legx);
    //sprintf(buf, "%.9g", l.legx);
    //tabLegBox->ledX->setText(QString(buf));
    //xv_setstr(legend_x_item, buf);
    tabLegBox->ledY->setDoubleValue("%.9g",l.legy);
    //sprintf(buf, "%.9g", l.legy);
    //tabLegBox->ledY->setText(QString(buf));
    //xv_setstr(legend_y_item, buf);
    tabLegBox->selLegBoxAttachement->setCurrentValue(l.autoattach);
    tabLegends->selVGap->setCurrentIndex(l.vgap);
    tabLegends->selHGap->setCurrentIndex(l.hgap);
    tabLegends->selLineLength->setCurrentIndex(l.len);
    tabLegends->chkPutRevOrder->setChecked(bool(l.invert));
    tabLegBox->selLoc->setCurrentIndex(!l.loctype);
    tabLegends->selTextFont->setCurrentIndex(l.font);
    tabLegends->selAlign->setCurrentValue(l.align);
    tabLegends->selTextColor->setCurrentIndex(l.color);
    tabLegends->selTextColor->setAlpha(l.alpha);
    tabLegBox->selFrameLineColor->setCurrentIndex(l.boxpen.color);
    tabLegBox->selFrameLineColor->setAlpha(l.boxpen.alpha);
    tabLegBox->selFrameLinePattern->setCurrentIndex(l.boxpen.pattern);
    tabLegBox->selFrameLineWidth->setValue(l.boxlinew);
    tabLegBox->selFrameLineStyle->setCurrentIndex(l.boxlines);
    tabLegBox->selFrameFillColor->setCurrentIndex(l.boxfillpen.color);
    tabLegBox->selFrameFillColor->setAlpha(l.boxfillpen.alpha);
    tabLegBox->selFrameFillPattern->setCurrentIndex(l.boxfillpen.pattern);
}

void frmGraph_App::update_frame_items(int gno)
{
    framep f;
    get_graph_framep(gno, &f);
    tabFrame->selFrameType->setCurrentIndex(f.type);
    tabFrame->selBoxColor->setCurrentIndex(f.pen.color);
    tabFrame->selBoxColor->setAlpha(f.pen.alpha);
    tabFrame->selFrameBoxPattern->setCurrentIndex(f.pen.pattern);
    tabFrame->selFrameBoxWidth->setValue(f.linew);
    tabFrame->selFrameBoxStyle->setCurrentIndex(f.lines);
    tabFrame->selFillColor->setCurrentIndex(f.fillpen.color);
    tabFrame->selFillColor->setAlpha(f.fillpen.alpha);
    tabFrame->selFrameFillPattern->setCurrentIndex(f.fillpen.pattern);
}

void frmGraph_App::show_graph_data_external(int n_gno)
{
int number=1;
int * sel=new int[2];
int old_gno;
    listGraph->get_selection(&number,&sel);
    if (number<1) old_gno=-100;
    else old_gno=sel[0];
        if (sel) delete[] sel;
if (old_gno==n_gno)//already selected (somehow)
{
listGraph->new_selection();//force a redisplay
}
else//set new selection --> will automatically force a redisplay
{
sel=new int[2];
sel[0]=n_gno;
number=1;
    listGraph->set_new_selection(number,sel);
if (sel) delete[] sel;
}

}

void frmGraph_App::IOrequested(int type,QString file,bool exists,bool writeable,bool readable)
{
    char filename[512];
    (void)exists;
    (void)writeable;
    (void)readable;
    strcpy(filename,file.toLocal8Bit());
    if (type==READ_PARAMETERS)
    {
        getparms(filename);
        if (frmOpenPara)
        frmOpenPara->hide();
        mainWin->mainArea->completeRedraw();
    }
    else if (type==WRITE_PARAMETERS)
    {
        int gno;
        FILE *pp;
        if (!frmSavePara)
        {
            gno = ALL_GRAPHS;
        }
        else
        {
            if (GetChoice(frmSavePara->selParamGraph) == 0) {
            gno = get_cg();
            } else {
            gno = ALL_GRAPHS;
            }
        }
        if (frmSavePara)
        frmSavePara->hide();
        pp = grace_openw(filename);
        if (pp != NULL) {
            putparms(gno, pp, 0);
            grace_close(pp);
        }
    }
}

void frmGraph_App::update0(void)
{
    static int i,nr;//,errpos;
    bool imm_upd_sav;
    if (!immediateUpdate) return;
    imm_upd_sav=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    GlobalInhibitor=true;
    (void)graphapp_aac_cb();//do this without actually doing something
    nr=nr_of_true_changes(ListOfChanges);
    //errpos=0;
    if (nr>0)
    {
        for (i=0;i<ListOfChanges.size();i++)
        {
            strcpy(command,ListOfChanges.at(i).toLocal8Bit().constData());
            //errpos = scanner(command);
            if (command[0]=='#')
            parse_qtGrace_Additions(command);
            else
            (void)scanner(command);
        }
        set_dirtystate();
        mainWin->mainArea->completeRedraw();
    }
    ListOfChanges.clear();
    ListOfOldStates.clear();
    GlobalInhibitor=false;
    immediateUpdate=imm_upd_sav;
    updateRunning=false;
}

void frmGraph_App::update1(int v)
{
    (void)v;
    update0();
}

void frmGraph_App::update2(QString v)
{
    (void)v;
    update0();
}

void frmGraph_App::update3(bool v)
{
    (void)v;
    update0();
}

void frmGraph_App::update4(double v)
{
    (void)v;
    update0();
}

frmGraphApp::frmGraphApp(QWidget * parent):QDialog(parent)
{
//setFont(*stdFont);
    min_w=484;
    min_h=594;
    bar_w=bar_h=20;//15
    setWindowTitle(tr("QtGrace: Graph Appearance"));
    setWindowIcon(QIcon(*GraceIcon));
    QVBoxLayout * layout=new QVBoxLayout;
    //layout->setMargin(0);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    flp=new frmGraph_App(this);
    connect(flp,SIGNAL(closeWish()),SLOT(close()));
    layout->addWidget(flp->menuBar);
    //layout->addWidget(flp);
    scroll=new dialogScrollArea(this);
    scroll->setWidget(flp);
    layout->addWidget(scroll);
    listGraph=flp->listGraph;
    setLayout(layout);
    //resize(min_w,min_h);
    resize(LastSize_FormGraphAppearance);
}

frmGraphApp::~frmGraphApp()
{
    LastSize_FormGraphAppearance=this->size();
}

void frmGraphApp::init(void)
{
    flp->init();
}

void frmGraphApp::show_graph_data_external(int n_gno)
{
    flp->show_graph_data_external(n_gno);
}

void frmGraphApp::update_view(int gno)
{
    flp->update_view(gno);
}

void frmGraphApp::updatelegends(int gno)
{
    flp->updatelegends(gno);
}

void frmGraphApp::doApply(void)
{
    flp->doApply();
}

void frmGraphApp::doAccept(void)
{
    flp->doAccept();
}

void frmGraphApp::doClose(void)
{
    hide();
}

void frmGraphApp::resizeEvent(QResizeEvent * event)
{
//cout << "resize: " << event->size().width() << "x" << event->size().height() << " bar_w=" << bar_w << " bar_h=" << bar_h << endl;
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
#if defined(WINDOWS_SYSTEM) || defined(LINUX_SYSTEM)
n_size_h-=flp->menuBar->height();
#endif
setMinimumSize(0,0);
}
else
{
scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
setMinimumSize(min_w,min_h);
}
flp->resize(QSize(n_size_w,n_size_h));
}

AxisTabMain::AxisTabMain(QWidget * parent):QWidget(parent)
{
    int number;
    char dummy[10];
    QString * entr=new QString[NUM_FMT_OPTION_ITEMS+2];
    grpAxisLabel=new QGroupBox(tr("Axis label"),this);
    ledAxisLabel=new stdLineEdit(grpAxisLabel,tr("Label string:"),true);
    layout0=new QHBoxLayout;
    //layout0->setMargin(STD_MARGIN);
    layout0->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout0->addWidget(ledAxisLabel);
    grpAxisLabel->setLayout(layout0);

    grpTickProp=new QGroupBox(tr("Tick properties"),this);
    ledMajorSpacing=new stdLineEdit(grpTickProp,tr("Major spacing:"));
    selMinTicks=new stdIntSelector(grpTickProp,tr("Minor ticks:"),0,MAX_TICKS - 1);
    number=NUM_FMT_OPTION_ITEMS;
    for (int i=0;i<number;i++)
    {
        entr[i]=QString(fmt_option_items[i].label);
    }
    selFormat=new StdSelector(grpTickProp,tr("Format:"),number,entr);
    number=10;
    for (int i=0;i<number;i++)
    {
        sprintf(dummy,"%d",i);
        entr[i]=QString(dummy);
    }
    selPrecision=new StdSelector(grpTickProp,tr("Precision:"),number,entr);
    layout1=new QGridLayout;
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout1->addWidget(ledMajorSpacing,0,0);
    layout1->addWidget(selMinTicks,0,1);
    layout1->addWidget(selFormat,1,0);
    layout1->addWidget(selPrecision,1,1);
    grpTickProp->setLayout(layout1);

    grpDisplOpt=new QGroupBox(tr("Display options"),this);
    chkDisplTickLabels=new QCheckBox(tr("Display tick labels"),grpDisplOpt);
    chkDisplAxixBar=new QCheckBox(tr("Display axis bar"),grpDisplOpt);
    chkDisplTickMarks=new QCheckBox(tr("Display tick marks"),grpDisplOpt);
    layout2=new QGridLayout;
    //layout2->setMargin(STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout2->addWidget(chkDisplTickLabels,0,0);
    layout2->addWidget(chkDisplAxixBar,0,1);
    layout2->addWidget(chkDisplTickMarks,1,0);
    grpDisplOpt->setLayout(layout2);

    grpAxisPlace=new QGroupBox(tr("Axis placement"),this);
    chkZeroAxis=new QCheckBox(tr("Zero axis"),grpAxisPlace);
    ledOffsetNormal=new stdLineEdit(grpAxisPlace,tr("Offsets - Normal:"));
    ledOffsetOpposite=new stdLineEdit(grpAxisPlace,tr("Opposite:"));
    layout3=new QHBoxLayout;
    //layout3->setMargin(STD_MARGIN);
    layout3->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout3->addWidget(chkZeroAxis);
    layout3->addWidget(ledOffsetNormal);
    layout3->addWidget(ledOffsetOpposite);
    grpAxisPlace->setLayout(layout3);

    grpTickLabelProp=new QGroupBox(tr("Tick label properties"),this);
    selTickLabelFont=new FontSelector(grpTickLabelProp);
    selTickLabelColor=new ColorSelector(grpTickLabelProp);
    layout4=new QHBoxLayout;
    //layout4->setMargin(STD_MARGIN);
    layout4->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout4->addWidget(selTickLabelFont);
    layout4->addWidget(selTickLabelColor);
    grpTickLabelProp->setLayout(layout4);


    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(grpAxisLabel);
    layout->addWidget(grpTickProp);
    layout->addWidget(grpDisplOpt);
    layout->addWidget(grpAxisPlace);
    layout->addWidget(grpTickLabelProp);
    setLayout(layout);
    delete[] entr;
}

AxisTabLabelBars::AxisTabLabelBars(QWidget * parent):QWidget(parent)
{
    int number;
    QString * entr=new QString[5];
    grpLabelProperties=new QGroupBox(tr("Label properties"),this);
    selLabelFont=new FontSelector(grpLabelProperties);
    selLabelColor=new ColorSelector(grpLabelProperties);
    sldCharSize=new stdSlider(grpLabelProperties,tr("Char size"),0,1000);
    number=2;
    entr[0]=tr("Parallel to axis");
    entr[1]=tr("Perpendicular to axis");
    selLayout=new StdSelector(grpLabelProperties,tr("Layout:"),number,entr);
    number=3;
    entr[0]=tr("Normal");
    entr[1]=tr("Opposite");
    entr[2]=tr("Both");
    selSide=new StdSelector(grpLabelProperties,tr("Side:"),number,entr);
    number=2;
    entr[0]=tr("Auto");
    entr[1]=tr("Specified");
    selLocation=new StdSelector(grpLabelProperties,tr("Location:"),number,entr);
    connect(selLocation->cmbSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(locationChanged(int)));
    ledParaOffs=new stdLineEdit(grpLabelProperties,tr("Parallel offset:"));
    ledPerpendOffs=new stdLineEdit(grpLabelProperties,tr("Perpendicular offset:"));
    ledParaOffs->setEnabled(FALSE);
    ledPerpendOffs->setEnabled(FALSE);
    selAlign=new AlignmentSelector(grpLabelProperties);
    selAlign->setToolTip(tr("Sets the text-alignment for each line\n(only useful in case you have an axis-labels with more than one line of text)"));
    layout1=new QGridLayout;
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout1->addWidget(selLabelFont,0,0);
    layout1->addWidget(selLabelColor,0,1);
    layout1->addWidget(sldCharSize,1,0);
    layout1->addWidget(selLayout,1,1);
    layout1->addWidget(selSide,2,0);
    layout1->addWidget(selLocation,2,1);
    layout1->addWidget(ledParaOffs,3,0);
    layout1->addWidget(ledPerpendOffs,3,1);
    layout1->addWidget(selAlign,4,0,1,1);
    grpLabelProperties->setLayout(layout1);

    grpBarProperties=new QGroupBox(tr("Bar properties:"),this);
    selBarColor=new ColorSelector(grpBarProperties);
    selBarStyle=new LineStyleSelector(grpBarProperties);
    selBarWidth=new LineWidthSelector(grpBarProperties);
    selBarWidth->lblText->setText(tr("Width:"));
    layout2=new QGridLayout;
    //layout2->setMargin(STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout2->addWidget(selBarColor,0,0);
    layout2->addWidget(selBarWidth,0,1);
    layout2->addWidget(selBarStyle,1,0);
    grpBarProperties->setLayout(layout2);

    //empty=new QWidget(this);
    //empty->setMinimumHeight(140);
    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(grpLabelProperties);
    layout->addWidget(grpBarProperties);
    layout->addStretch(3);
    //layout->addWidget(empty);
    setLayout(layout);
    delete[] entr;
}

void AxisTabLabelBars::locationChanged(int i)
{
    if (i==0)
    {
        ledParaOffs->setEnabled(FALSE);
        ledPerpendOffs->setEnabled(FALSE);
    }
    else
    {
        ledParaOffs->setEnabled(TRUE);
        ledPerpendOffs->setEnabled(TRUE);
    }
}

AxisTabTickLabels::AxisTabTickLabels(QWidget * parent):QWidget(parent)
{
    int number;
    char dummy[10];
    QString * entr=new QString[12];

    grpLabels=new QGroupBox(tr("Labels"),this);
    sldCharSize=new stdSlider(grpLabels,tr("Char size"),0,1000);
    sldCharAngle=new stdSlider(grpLabels,tr("Angle"),0,360);
    selAlign=new AlignmentSelector(grpLabels);
    selAlign->setToolTip(tr("Sets the text-alignment for each line\n(only useful in case you have tick-labels with more than one line of text)"));
    layout0=new QHBoxLayout;
    //layout0->setMargin(STD_MARGIN);
    layout0->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout0->addWidget(sldCharSize);
    layout0->addWidget(sldCharAngle);
    layout0->addWidget(selAlign);
    grpLabels->setLayout(layout0);

    grpPlacement=new QGroupBox(tr("Placement"),this);
    number=3;
    entr[0]=tr("Normal");
    entr[1]=tr("Opposite");
    entr[2]=tr("Both");
    selSide=new StdSelector(grpPlacement,tr("Side:"),number,entr);
    number=2;
    entr[0]=tr("Axis min");
    entr[1]=tr("Specified:");
    selStartAt=new StdSelector(grpPlacement,tr("Start at:"),number,entr);
    entr[0]=tr("Axis max");
    selStopAt=new StdSelector(grpPlacement,tr("Stop at:"),number,entr);
    number=10;
    for (int i=0;i<number;i++)
    {
        sprintf(dummy,"%d",i);
        entr[i]=QString(dummy);
    }
    selStagger=new StdSelector(grpPlacement,tr("Stagger:"),number,entr);
    ledStart=new QLineEdit(QString(""),grpPlacement);
    ledStop=new QLineEdit(QString(""),grpPlacement);
    layout1=new QGridLayout;
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout1->addWidget(selSide,0,0);
    layout1->addWidget(selStartAt,0,1);
    layout1->addWidget(ledStart,0,2);
    layout1->addWidget(selStagger,1,0);
    layout1->addWidget(selStopAt,1,1);
    layout1->addWidget(ledStop,1,2);
    grpPlacement->setLayout(layout1);

    grpExtra=new QGroupBox(tr("Extra"),this);
    ledPrepend=new stdLineEdit(grpExtra,tr("Prepend:"),true);
    ledAppend=new stdLineEdit(grpExtra,tr("Append:"),true);
    ledAxisTransform=new stdLineEdit(grpExtra,tr("Axis transform:"));
    ledParaOffs=new stdLineEdit(grpExtra,tr("Parallel offset:"));
    ledPerpendOffs=new stdLineEdit(grpExtra,tr("Perpendicular offset:"));
    ledParaOffs->setEnabled(FALSE);
    ledPerpendOffs->setEnabled(FALSE);
    selSkipEvery=new StdSelector(grpExtra,tr("Skip every:"),number,entr);
    number=2;
    entr[0]=tr("Auto");
    entr[1]=tr("Specified");
    selLocation=new StdSelector(grpExtra,tr("Location:"),number,entr);
    connect(selLocation->cmbSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(locationChanged(int)));
    layout2=new QGridLayout;
    //layout2->setMargin(STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout2->addWidget(selSkipEvery,0,0);
    layout2->addWidget(ledAxisTransform,0,1);
    layout2->addWidget(ledPrepend,1,0);
    layout2->addWidget(ledAppend,1,1);
    layout2->addWidget(selLocation,2,0);
    layout2->addWidget(ledParaOffs,3,0);
    layout2->addWidget(ledPerpendOffs,3,1);
    grpExtra->setLayout(layout2);

    grpQuick=new QGroupBox(tr("Quick transformation"),this);
    grpQuick->setToolTip(tr("The buttons in this group assume that the data is present in radians.\nThey just set axis transformations for different representations of angles."));
    layout3=new QHBoxLayout;
    cmdQuickNormal=new QPushButton(tr("Normal"),this);
    cmdQuickNormal->setToolTip(tr("Reset axis transformation to default\n(i.e. no transformation)"));
    cmdQuickDegrees=new QPushButton(tr("Degrees"),this);
    cmdQuickDegrees->setToolTip(tr("Set tick labels to degrees\n(with spacing = 90 degrees)"));
    cmdQuickPis=new QPushButton(tr("Multiples of Pi"),this);
    cmdQuickPis->setToolTip(tr("Set tick labels to multiples of PI\n(with spacing = PI/2)"));
    cmdQuickAlt=new QPushButton(tr("Alt-Axis"),this);
    cmdQuickAlt->setToolTip(tr("Generate an alternative axis by an axis-transformation.\nThis opens a dialog that helps in setting up a secondary axis with a different scale."));
    connect(cmdQuickNormal,SIGNAL(clicked()),SLOT(doQuickNormal()));
    connect(cmdQuickDegrees,SIGNAL(clicked()),SLOT(doQuickDegrees()));
    connect(cmdQuickPis,SIGNAL(clicked()),SLOT(doQuickPis()));
    connect(cmdQuickAlt,SIGNAL(clicked()),SLOT(doQuickAlt()));
    //layout3->setMargin(STD_MARGIN);
    layout3->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout3->addWidget(cmdQuickNormal);
    layout3->addWidget(cmdQuickDegrees);
    layout3->addWidget(cmdQuickPis);
    layout3->addWidget(cmdQuickAlt);
    grpQuick->setLayout(layout3);

    //empty=new QWidget(this);
    //empty->setMinimumHeight(70);
    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(grpLabels);
    layout->addWidget(grpPlacement);
    layout->addWidget(grpExtra);
    layout->addWidget(grpQuick);
    layout->addStretch(3);
    //layout->addWidget(empty);
    setLayout(layout);
    delete[] entr;
}

void AxisTabTickLabels::locationChanged(int i)
{
    if (i==0)
    {
        ledParaOffs->setEnabled(FALSE);
        ledPerpendOffs->setEnabled(FALSE);
    }
    else
    {
        ledParaOffs->setEnabled(TRUE);
        ledPerpendOffs->setEnabled(TRUE);
    }
}

void AxisTabTickLabels::doQuickNormal(void)
{
emit(quickSetNormal());
}

void AxisTabTickLabels::doQuickDegrees(void)
{
emit(quickSetDegrees());
}

void AxisTabTickLabels::doQuickPis(void)
{
emit(quickSetPis());
}

void AxisTabTickLabels::doQuickAlt(void)
{
emit(quickSetAlt());
}

AxisTabTickMarks::AxisTabTickMarks(QWidget * parent):QWidget(parent)
{
    int number;
    char dummy[10];
    QString * entr=new QString[12];

    grpPlacement=new QGroupBox(tr("Placement"),this);
    number=3;
    entr[0]=tr("In");
    entr[1]=tr("Out");
    entr[2]=tr("Both");
    selPointing=new StdSelector(grpPlacement,tr("Pointing"),number,entr);
    entr[0]=tr("Normal side");
    entr[1]=tr("Opposite side");
    entr[2]=tr("Both sides");
    selDrawOn=new StdSelector(grpPlacement,tr("Draw on:"),number,entr);
    number=11;
    for (int i=2;i<13;i++)
    {
        sprintf(dummy,"%d",i);
        entr[i-2]=QString(dummy);
    }
    setAutotickDiv=new StdSelector(grpPlacement,tr("Autotick divisions:"),number,entr);
    chkPlaceRoundPos=new QCheckBox(tr("Place at rounded positions"),grpPlacement);
    layout0=new QGridLayout;
    //layout0->setMargin(STD_MARGIN);
    layout0->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout0->addWidget(selPointing,0,0);
    layout0->addWidget(selDrawOn,0,1);
    layout0->addWidget(chkPlaceRoundPos,1,0);
    layout0->addWidget(setAutotickDiv,1,1);
    grpPlacement->setLayout(layout0);

    grpMajorTicks=new QGroupBox(tr("Major ticks"),this);
    chkDrawMajGrid=new QCheckBox(tr("Draw grid lines"),grpMajorTicks);
    sldMajTickLength=new stdSlider(grpMajorTicks,tr("Tick length"),0,1000);
    selMajTickColor=new ColorSelector(grpMajorTicks);
    selMajTickWidth=new LineWidthSelector(grpMajorTicks);
    selMajTickStyle=new LineStyleSelector(grpMajorTicks);
    layout1=new QVBoxLayout;
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout1->addWidget(chkDrawMajGrid);
    layout1->addWidget(sldMajTickLength);
    layout1->addWidget(selMajTickColor);
    layout1->addWidget(selMajTickWidth);
    layout1->addWidget(selMajTickStyle);
    grpMajorTicks->setLayout(layout1);

    grpMinorTicks=new QGroupBox(tr("Minor ticks"),this);
    chkDrawMinGrid=new QCheckBox(tr("Draw grid lines"),grpMinorTicks);
    sldMinTickLength=new stdSlider(grpMinorTicks,tr("Tick length"),0,1000);
    selMinTickColor=new ColorSelector(grpMinorTicks);
    selMinTickWidth=new LineWidthSelector(grpMinorTicks);
    selMinTickStyle=new LineStyleSelector(grpMinorTicks);
    layout2=new QVBoxLayout;
    //layout2->setMargin(STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout2->addWidget(chkDrawMinGrid);
    layout2->addWidget(sldMinTickLength);
    layout2->addWidget(selMinTickColor);
    layout2->addWidget(selMinTickWidth);
    layout2->addWidget(selMinTickStyle);
    grpMinorTicks->setLayout(layout2);

    empty=new QWidget(this);
    empty->setMinimumHeight(40);

    layout=new QGridLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(grpPlacement,0,0,1,2);
    layout->addWidget(grpMajorTicks,1,0);
    layout->addWidget(grpMinorTicks,1,1);
    layout->addWidget(empty,2,0,1,2);
    layout->setRowStretch(0,1);
    layout->setRowStretch(1,1);
    layout->setRowStretch(2,5);
    setLayout(layout);
    delete[] entr;
}

AxisTabSpecial::AxisTabSpecial(QWidget * parent):QWidget(parent)
{
    int number;
    QString * entr=new QString[12];
    number=3;
    entr[0]=tr("None");
    entr[1]=tr("Tick marks only");
    entr[2]=tr("Tick marks and labels");
    selSpecTicks=new StdSelector(this,tr("Custom ticks:"),number,entr);
    selSpecTicks->entryValues[0]=TICKS_SPEC_NONE;
    selSpecTicks->entryValues[1]=TICKS_SPEC_MARKS;
    selSpecTicks->entryValues[2]=TICKS_SPEC_BOTH;
    selNumber=new stdIntSelector(this,tr("Number of user ticks to use:"),0,MAX_TICKS - 1);
    //lblTickLocLabel=new QLabel(tr("Nr. - Tick location - Label:"),this);

    connect(selSpecTicks,SIGNAL(currentIndexChanged(int)),SLOT(updateSpreadSheet(int)));
    connect(selNumber,SIGNAL(currentValueChanged(int)),SLOT(updateSpreadSheet(int)));

    scroll=new QScrollArea(this);
    empty=new QWidget(this);
    layout0=new QGridLayout;
    //layout0->setMargin(STD_MARGIN);
    layout0->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout0->setSpacing(STD_SPACING);
    int index=0;
    lblTitles[0]=new QLabel(tr("Nr.  "),this);
    lblTitles[1]=new QLabel(tr("Tick Location"),this);
    lblTitles[2]=new QLabel(tr("Tick Label"),this);
        for (int i=0;i<3;i++) layout0->addWidget(lblTitles[i],index,i);
        index++;
    for (int i=0;i<MAX_TICKS;i++)
    {
    original[i]=true;
    orig_text[i]=text[i]=NULL;
    lblNr[i]=new QLabel(QString::number(i),this);
    ledLocation[i]=new QLineEdit(QString(""),this);
    ledLabel[i]=new QLineEdit(QString(""),this);
    layout0->addWidget(lblNr[i],index,0);
    layout0->addWidget(ledLocation[i],index,1);
    layout0->setRowStretch(index,0);
    layout0->addWidget(ledLabel[i],index++,2);
    }
    empty->setLayout(layout0);
    scroll->setWidget(empty);

    /*spreadSpecLabels=new spreadSheet(scroll,2,256,3);
    spreadSpecLabels->setMinimumWidth(400);
    scroll->setWidget(spreadSpecLabels);*/

    layout=new QVBoxLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(selSpecTicks);
    layout->addWidget(selNumber);
    //layout->addWidget(lblTickLocLabel);
    layout->addWidget(scroll);
    setLayout(layout);
    delete[] entr;
}

void AxisTabSpecial::updateSpreadSheet(int i)
{
    (void)i;
int custom_type=selSpecTicks->currentIndex();//0=none,1=TickMarks only,2=TickMarks and Labels
int custom_nr=selNumber->value();

headerHeight=lblTitles[0]->height();
stdHeight=lblNr[0]->height();
if (headerHeight<16) headerHeight=16;
if (stdHeight<22) stdHeight=22;

int w,h;
w=lblTitles[0]->width()+lblTitles[1]->width()+lblTitles[2]->width();
if (w<316) w=316;
w+=2*(STD_MARGIN+STD_SPACING);
h=2*STD_MARGIN+(STD_SPACING+stdHeight)*custom_nr+headerHeight;
empty->resize(w,h);

if (custom_type==0)//none
{
    for (int i=0;i<MAX_TICKS;i++)
    {
    lblNr[i]->setEnabled(false);
    ledLocation[i]->setEnabled(false);
    ledLabel[i]->setEnabled(false);
    }
}
else if (custom_type==1)//TickMarks only
{
    for (int i=0;i<MAX_TICKS;i++)
    {
    lblNr[i]->setEnabled(true);
    ledLocation[i]->setEnabled(true);
    ledLabel[i]->setEnabled(false);
    }
}
else//TickMarks and Labels
{
    for (int i=0;i<MAX_TICKS;i++)
    {
    lblNr[i]->setEnabled(true);
    ledLocation[i]->setEnabled(true);
    ledLabel[i]->setEnabled(true);
    }
}

for (int i=0;i<custom_nr;i++)
{
    lblNr[i]->setVisible(true);
    ledLocation[i]->setVisible(true);
    ledLabel[i]->setVisible(true);
}
for (int i=custom_nr;i<MAX_TICKS;i++)
{
    lblNr[i]->setVisible(false);
    ledLocation[i]->setVisible(false);
    ledLabel[i]->setVisible(false);
}

}

frmAxis_Prop::frmAxis_Prop(QWidget * parent):QWidget(parent)
{
    int number;
    curaxis=X_AXIS;
    QString * entr=new QString[8];
//setFont(*stdFont);
    setWindowTitle(tr("QtGrace: Axis"));
    setWindowIcon(QIcon(*GraceIcon));

    number=4;
    entr[0]=tr("X axis");
    entr[1]=tr("Y axis");
    entr[2]=tr("Alt X axis");
    entr[3]=tr("Alt Y axis");
    selEdit=new StdSelector(this,tr("Edit:"),number,entr);
    selEdit->entryValues[0]=X_AXIS;
    selEdit->entryValues[1]=Y_AXIS;
    selEdit->entryValues[2]=ZX_AXIS;
    selEdit->entryValues[3]=ZY_AXIS;
    connect(selEdit->cmbSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(selEditChanged(int)));
    chkActive=new QCheckBox(tr("Active"),this);
    chkActive->setChecked(TRUE);
    connect(chkActive,SIGNAL(stateChanged(int)),SLOT(set_active_proc(int)));

    ledStart=new stdLineEdit(this,tr("Start:"));
    ledStop=new stdLineEdit(this,tr("Stop:"));
    chkInvAxis=new QCheckBox(tr("Invert axis"),this);
    chkInvAxis->setChecked(FALSE);
    tabs=new QTabWidget(this);
    tabMain=new AxisTabMain(tabs);
    tabLabelsBars=new AxisTabLabelBars(tabs);
    tabTickLabels=new AxisTabTickLabels(tabs);
    tabTickMarks=new AxisTabTickMarks(tabs);
    tabSpecial=new AxisTabSpecial(tabs);
    tabs->addTab(tabMain, tr("Main"));
    tabs->addTab(tabLabelsBars, tr("Axis label && bar"));
    tabs->addTab(tabTickLabels, tr("Tick labels"));
    tabs->addTab(tabTickMarks, tr("Tick marks"));
    tabs->addTab(tabSpecial, tr("Custom ticks"));
    number=4;
    entr[0]=tr("Linear");
    entr[1]=tr("Logarithmic");
    entr[2]=tr("Reciprocal");
    entr[3]=tr("Logit");
    selScale=new StdSelector(this,tr("Scale:"),number,entr);
    selScale->entryValues[0]=SCALE_NORMAL;
    selScale->entryValues[1]=SCALE_LOG;
    selScale->entryValues[2]=SCALE_REC;
    selScale->entryValues[3]=SCALE_LOGIT;
    connect(selScale->cmbSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(axis_scale_cb(int)));
    number=8;
    entr[0]=tr("Current axis");
    entr[1]=tr("All axes, current graph");
    entr[2]=tr("Current axis, all graphs");
    entr[3]=tr("All axes, all graphs");
    entr[4]=tr("x+y axes, current graph");
    entr[5]=tr("x+y axes, all graphs");
    entr[6]=tr("alt x+y axes, current graph");
    entr[7]=tr("alt x+y axes, all graphs");
    selApplyTo=new StdSelector(this,tr("Apply to:"),number,entr);
    selApplyTo->lblText->setVisible(true);
    cmdApplyTo=new QPushButton(tr("Apply to:"),selApplyTo);
    cmdApplyTo->setToolTip(tr("The Apply-button below always applies the Axis-settings to the current axis. With this button (Apply-to) you are able to select multiple axes to apply the settings to at the same time."));
    selApplyTo->layout->removeWidget(selApplyTo->lblText);
    selApplyTo->layout->removeWidget(selApplyTo->cmbSelect);
    selApplyTo->layout->addWidget(cmdApplyTo);
    selApplyTo->layout->addWidget(selApplyTo->cmbSelect);
    chkTransferLabel=new QCheckBox(tr("Incl.axis-label"),this);
    chkTransferLabel->setToolTip(tr("If activated the label and its appearance on the current axis will be copied to other axes as well.\nTo deactivate this is useful if different graphs share an axis (pack graphs) because some axes have their axis-label disabled on purpose.\nIf deactivated the new settings of the label will be applied to the current axis only, the other settings will be applied to the other axes as intended."));
    chkTransferTickLabel=new QCheckBox(tr("Incl.ticks"),this);
    chkTransferTickLabel->setToolTip(tr("If activated the tick-settings of the current axis will be copied to other axes as well.\nTo deactivate this is useful if different graphs share an axis (pack graphs) because some axes have their tick-labels disabled on purpose.\nIf deactivated the new settings for the ticks will be applied to the current axis only, the other settings will be applied to the other axes as intended."));
    chkTransferLabel->setChecked(true);
    chkTransferTickLabel->setChecked(true);
    selApplyTo->layout->addWidget(chkTransferLabel);
    selApplyTo->layout->addWidget(chkTransferTickLabel);
    connect(cmdApplyTo,SIGNAL(clicked()),SLOT(doApplyTo()));

    buttonGroup=new stdButtonGroup(this);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));
    buttonGroup->cmdApply->setDefault(true);
    layout=new QGridLayout;
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->addWidget(selEdit,0,0);
    layout->addWidget(chkActive,0,1);
    //layout->addWidget(empty[0],0,2);
    layout->addWidget(ledStart,1,0);
    layout->addWidget(ledStop,1,1);
    //layout->addWidget(empty[1],1,2);
    layout->addWidget(selScale,2,0);
    layout->addWidget(chkInvAxis,2,1);
    //layout->addWidget(empty[2],2,2);
    layout->addWidget(tabs,3,0,1,3);
    layout->addWidget(selApplyTo,4,0,1,2);
    //layout->addWidget(empty[3],4,2);
    layout->addWidget(buttonGroup,5,0,1,3);
    setLayout(layout);

    //immediateUpdate
    connect(chkActive,SIGNAL(stateChanged(int)),SLOT(update1(int)));
    connect(chkInvAxis,SIGNAL(stateChanged(int)),SLOT(update1(int)));

    connect(tabMain->ledAxisLabel,SIGNAL(changed()),SLOT(update0()));
    connect(tabMain->ledMajorSpacing,SIGNAL(changed()),SLOT(update0()));
    connect(tabMain->selMinTicks,SIGNAL(currentValueChanged(int)),SLOT(update1(int)));
    connect(tabMain->selFormat,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabMain->selPrecision,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabMain->chkDisplTickLabels,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(tabMain->chkDisplAxixBar,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(tabMain->chkDisplTickMarks,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(tabMain->chkZeroAxis,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(tabMain->ledOffsetNormal,SIGNAL(changed()),SLOT(update0()));
    connect(tabMain->ledOffsetOpposite,SIGNAL(changed()),SLOT(update0()));
    connect(tabMain->selTickLabelFont,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabMain->selTickLabelColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabMain->selTickLabelColor,SIGNAL(alphaChanged(int)),SLOT(update1(int)));

    connect(tabLabelsBars->selLabelFont,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLabelsBars->selLabelColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLabelsBars->selLabelColor,SIGNAL(alphaChanged(int)),SLOT(update1(int)));
    connect(tabLabelsBars->sldCharSize,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(tabLabelsBars->selLayout,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLabelsBars->selSide,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLabelsBars->selLocation,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLabelsBars->ledParaOffs,SIGNAL(changed()),SLOT(update0()));
    connect(tabLabelsBars->ledPerpendOffs,SIGNAL(changed()),SLOT(update0()));
    connect(tabLabelsBars->selBarColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLabelsBars->selBarColor,SIGNAL(alphaChanged(int)),SLOT(update1(int)));
    connect(tabLabelsBars->selBarStyle,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLabelsBars->selBarWidth,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(tabLabelsBars->selAlign->cmbJustSelect2,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));

    connect(tabTickLabels->sldCharSize,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(tabTickLabels->sldCharAngle,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(tabTickLabels->selSide,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTickLabels->selStartAt,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTickLabels->selStopAt,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTickLabels->selStagger,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTickLabels->ledStart,SIGNAL(returnPressed()),SLOT(update0()));
    connect(tabTickLabels->ledStop,SIGNAL(returnPressed()),SLOT(update0()));
    connect(tabTickLabels->ledPrepend,SIGNAL(changed()),SLOT(update0()));
    connect(tabTickLabels->ledAppend,SIGNAL(changed()),SLOT(update0()));
    connect(tabTickLabels->ledAxisTransform,SIGNAL(changed()),SLOT(update0()));
    connect(tabTickLabels->ledParaOffs,SIGNAL(changed()),SLOT(update0()));
    connect(tabTickLabels->ledPerpendOffs,SIGNAL(changed()),SLOT(update0()));
    connect(tabTickLabels->selSkipEvery,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTickLabels->selLocation,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTickLabels->selAlign->cmbJustSelect2,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));

    connect(tabTickMarks->selPointing,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTickMarks->selDrawOn,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTickMarks->setAutotickDiv,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTickMarks->chkPlaceRoundPos,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(tabTickMarks->chkDrawMajGrid,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(tabTickMarks->sldMajTickLength,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(tabTickMarks->selMajTickColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTickMarks->selMajTickColor,SIGNAL(alphaChanged(int)),SLOT(update1(int)));
    connect(tabTickMarks->selMajTickWidth,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(tabTickMarks->selMajTickStyle,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTickMarks->chkDrawMinGrid,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(tabTickMarks->sldMinTickLength,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(tabTickMarks->selMinTickColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTickMarks->selMinTickColor,SIGNAL(alphaChanged(int)),SLOT(update1(int)));
    connect(tabTickMarks->selMinTickWidth,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(tabTickMarks->selMinTickStyle,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));

    connect(tabSpecial->selSpecTicks,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabSpecial->selNumber,SIGNAL(currentValueChanged(int)),SLOT(update1(int)));

    connect(tabTickLabels,SIGNAL(quickSetNormal()),SLOT(doQuickNormalTicks()));
    connect(tabTickLabels,SIGNAL(quickSetDegrees()),SLOT(doQuickDegreesTicks()));
    connect(tabTickLabels,SIGNAL(quickSetPis()),SLOT(doQuickPisTricks()));
    connect(tabTickLabels,SIGNAL(quickSetAlt()),SLOT(doQuickAltTricks()));
//connect(tabSpecial->spreadSpecLabels,SIGNAL(changed()),SLOT(update0()));
    //end immediateUpdate
    delete[] entr;
}

frmAxis_Prop::~frmAxis_Prop()
{}

void frmAxis_Prop::selEditChanged(int i)
{
    set_axis_proc(i);
}

void frmAxis_Prop::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmAxis_Prop::doApply(void)
{
    apply_to_selection = 0;//only apply to the current Axis!
    transfer_tick_labels = 1;//if only apply to the current axis --> do always apply
    transfer_label = 1;//if only apply to the current axis --> do always apply
    axes_aac_cb();
}

void frmAxis_Prop::doApplyTo(void)
{
    apply_to_selection = selApplyTo->currentValue();
    transfer_tick_labels = (chkTransferTickLabel->isChecked()==true?1:0);
    transfer_label = (chkTransferLabel->isChecked()==true?1:0);
    axes_aac_cb();
}

void frmAxis_Prop::doClose(void)
{
//parentWidget()->hide();
    emit(closeWish());
}

/*
 * Callback function for definition of tick marks and axis labels.
 */
int frmAxis_Prop::axes_aac_cb(void)
{
    ApplyError=false;
    //QString text;
    char dummy[MAX_STRING_LENGTH],dummy2[MAX_STRING_LENGTH],descr_axis[32];
    int i, j;
    int applyto;//,tmp_active;
    int axis_start, axis_stop, graph_start, graph_stop;
    int scale, invert,ret;
    tickmarks *t,*t2=NULL,*t3=NULL;
    double axestart, axestop;
    world w,w2;
    bool tt_l=true,ta_l=true;

    //applyto = selApplyTo->currentValue();
    applyto = apply_to_selection;

    t  = new_graph_tickmarks();
    if (!t)
    {
        return RETURN_FAILURE;
    }
    t->active = chkActive->isChecked();
    t->zero = tabMain->chkZeroAxis->isChecked();
    if (xv_evalexpr(tabMain->ledMajorSpacing, &t->tmajor) != RETURN_SUCCESS)
    {
        errmsg(tr("Specify major tick spacing").toLocal8Bit().constData());
        free_graph_tickmarks(t);
        ApplyError=true;
        return RETURN_FAILURE;
    }
    t->nminor = tabMain->selMinTicks->value();

    t->tl_flag = tabMain->chkDisplTickLabels->isChecked();
    t->t_flag = tabMain->chkDisplTickMarks->isChecked();
    t->t_drawbar = tabMain->chkDisplAxixBar->isChecked();

    ///setting plotstring
    tabMain->ledAxisLabel->DynSetMemoryToText(t->label.s_plotstring,t->label.alt_plotstring);
    xv_evalexpr(tabMain->ledOffsetNormal, &t->offsx);
    xv_evalexpr(tabMain->ledOffsetOpposite, &t->offsy);
    t->label_layout = tabLabelsBars->selLayout->currentIndex() ? LAYOUT_PERPENDICULAR : LAYOUT_PARALLEL;
    t->label_place = tabLabelsBars->selLocation->currentIndex();
    if (t->label_place == TYPE_SPEC)//or auto
    {
        xv_evalexpr(tabLabelsBars->ledParaOffs, &t->label.x);
        xv_evalexpr(tabLabelsBars->ledPerpendOffs, &t->label.y);
    }
    t->label.font = tabLabelsBars->selLabelFont->currentIndex();
    t->label.align = tabLabelsBars->selAlign->currentValue();
    t->label.color = tabLabelsBars->selLabelColor->currentIndex();
    t->label.alpha = tabLabelsBars->selLabelColor->alpha();
    t->label.charsize = tabLabelsBars->sldCharSize->value()/100.0;

    /* somehow the value of axislabelop gets automatically correctly
       applied to all selected axes without checking for the value of
       applyto directly here (strange...) */
    switch(tabLabelsBars->selSide->currentIndex())
    {
    case 0:
        t->label_op = PLACEMENT_NORMAL;
        break;
    case 1:
        t->label_op = PLACEMENT_OPPOSITE;
        break;
    case 2:
        t->label_op = PLACEMENT_BOTH;
        break;
    }

    t->tl_font = tabMain->selTickLabelFont->currentIndex();
    t->tl_color = tabMain->selTickLabelColor->currentIndex();
    t->tl_alpha = tabMain->selTickLabelColor->alpha();
    t->tl_skip = tabTickLabels->selSkipEvery->currentIndex();
    t->tl_align = tabTickLabels->selAlign->currentValue();
    t->tl_prec = tabMain->selPrecision->currentIndex();
    t->tl_staggered = tabTickLabels->selStagger->currentIndex();

    tabTickLabels->ledAppend->SetMemoryToText(t->tl_appstr,t->orig_tl_appstr);
    tabTickLabels->ledPrepend->SetMemoryToText(t->tl_prestr,t->orig_tl_prestr);
    //strcpy(t->tl_appstr, tabTickLabels->ledAppend->text().toLocal8Bit());
    //strcpy(t->tl_prestr, tabTickLabels->ledPrepend->text().toLocal8Bit());
    ///setting strings
    t->tl_starttype =  tabTickLabels->selStartAt->currentIndex()== 0 ? TYPE_AUTO : TYPE_SPEC;
    if (t->tl_starttype == TYPE_SPEC)
    {
        if(xv_evalexpr(tabTickLabels->ledStart, &t->tl_start) != RETURN_SUCCESS)
        {
            errmsg(tr("Specify tick label start").toLocal8Bit().constData());
            free_graph_tickmarks(t);
            ApplyError=true;
            return RETURN_FAILURE;
        }
    }
    t->tl_stoptype =  tabTickLabels->selStopAt->currentIndex()== 0 ? TYPE_AUTO : TYPE_SPEC;
    if (t->tl_stoptype == TYPE_SPEC)
    {
        if(xv_evalexpr(tabTickLabels->ledStop, &t->tl_stop) != RETURN_SUCCESS)
        {
            errmsg(tr("Specify tick label stop").toLocal8Bit().constData());
            free_graph_tickmarks(t);
            ApplyError=true;
            return RETURN_FAILURE;
        }
    }
    t->tl_format = tabMain->selFormat->currentIndex();
    strcpy(dummy,tabTickLabels->ledAxisTransform->text().toLatin1().constData());
    PrepareFormula(dummy);//replace ',' with '.' if necessary
    t->tl_formula = copy_string(NULL, dummy);
    t->tl_gaptype = tabTickLabels->selLocation->currentIndex()== 0 ? TYPE_AUTO : TYPE_SPEC;
    if (t->tl_gaptype == TYPE_SPEC)
    {
        xv_evalexpr(tabTickLabels->ledParaOffs, &t->tl_gap.x);
        xv_evalexpr(tabTickLabels->ledPerpendOffs, &t->tl_gap.y);
    }

    t->tl_angle = tabTickLabels->sldCharAngle->value();
    t->tl_charsize = tabTickLabels->sldCharSize->value()/100.0;

    switch (tabTickMarks->selPointing->currentIndex())
    {
    case 0:
        t->t_inout = TICKS_IN;
        break;
    case 1:
        t->t_inout = TICKS_OUT;
        break;
    case 2:
        t->t_inout = TICKS_BOTH;
        break;
    }
    switch(tabTickLabels->selSide->currentIndex())
    {
    case 0:
        t->tl_op = PLACEMENT_NORMAL;
        break;
    case 1:
        t->tl_op = PLACEMENT_OPPOSITE;
        break;
    case 2:
        t->tl_op = PLACEMENT_BOTH;
        break;
    }
    switch(tabTickMarks->selDrawOn->currentIndex())
    {
    case 0:
        t->t_op = PLACEMENT_NORMAL;
        break;
    case 1:
        t->t_op = PLACEMENT_OPPOSITE;
        break;
    case 2:
        t->t_op = PLACEMENT_BOTH;
        break;
    }

    t->props.color = tabTickMarks->selMajTickColor->currentIndex();
    t->props.alpha = tabTickMarks->selMajTickColor->alpha();
    t->props.linew = tabTickMarks->selMajTickWidth->value();
    t->props.lines = tabTickMarks->selMajTickStyle->currentIndex();
    t->mprops.color = tabTickMarks->selMinTickColor->currentIndex();
    t->mprops.alpha = tabTickMarks->selMinTickColor->alpha();
    t->mprops.linew = tabTickMarks->selMinTickWidth->value();
    t->mprops.lines = tabTickMarks->selMinTickStyle->currentIndex();
    t->props.size = tabTickMarks->sldMajTickLength->value()/100.0;
    t->mprops.size = tabTickMarks->sldMinTickLength->value()/100.0;
    t->t_autonum = 2+tabTickMarks->setAutotickDiv->currentIndex();
    t->t_round = tabTickMarks->chkPlaceRoundPos->isChecked();
    t->props.gridflag = tabTickMarks->chkDrawMajGrid->isChecked();
    t->mprops.gridflag = tabTickMarks->chkDrawMinGrid->isChecked();
    t->t_drawbarcolor = tabLabelsBars->selBarColor->currentIndex();
    t->t_drawbaralpha = tabLabelsBars->selBarColor->alpha();
    t->t_drawbarlinew = tabLabelsBars->selBarWidth->value();
    t->t_drawbarlines = tabLabelsBars->selBarStyle->currentIndex();

    t->t_spec = tabSpecial->selSpecTicks->currentValue();
    /* only read special info if special ticks used */
    //if (t->t_spec != TICKS_SPEC_NONE)
    //{
        t->nticks = tabSpecial->selNumber->value();
        /* ensure that enough tick positions have been specified */
        for (i = 0; i < t->nticks; i++)
        {
            //if (xv_evalexpr(tabSpecial->spreadSpecLabels->axislines[i]->ledLocation, &t->tloc[i].wtpos) == RETURN_SUCCESS)
            if (xv_evalexpr(tabSpecial->ledLocation[i], &t->tloc[i].wtpos) == RETURN_SUCCESS)
            {
                //strcpy(dummy,tabSpecial->spreadSpecLabels->axislines[i]->ledLabel->text().toLatin1().constData());//copy text directly
                //strcpy(dummy,tabSpecial->ledLabel[i]->text().toLatin1().constData());//copy text directly
                /// Warning --> use encoding here! (instead of latin1)
                //strcpy(dummy,tabSpecial->spreadSpecLabels->axislines[i]->LabelText().toLocal8Bit());
                //if (dummy[0] == '\0')
                if (tabSpecial->ledLabel[i]->text().isEmpty()==true)
                {
                    t->tloc[i].type = TICK_TYPE_MINOR;
                }
                else
                {
                    t->tloc[i].type = TICK_TYPE_MAJOR;
                }
                if (t->t_spec == TICKS_SPEC_BOTH)
                {
                    DynSetMemoryToLineEdit(t->tloc[i].label,t->tloc[i].orig_label,tabSpecial->text[i],tabSpecial->orig_text[i],tabSpecial->original[i],true,tabSpecial->ledLabel[i]);
                    //tabSpecial->spreadSpecLabels->axislines[i]->SetMemoryToText(t->tloc[i].label,t->tloc[i].orig_label);
                    /*
                    t->tloc[i].orig_label = copy_string(t->tloc[i].label, dummy);
                    t->tloc[i].label = copy_string(t->tloc[i].label, dummy);
                    if (activateLaTeXsupport==true)
                    {
                    text=QString(t->tloc[i].label);
                    complete_LaTeX_to_Grace_Translator(text);
                    t->tloc[i].label=copy_string(t->tloc[i].label,text.toLocal8Bit().constData());
                    }
                    */
                }
                else
                {//simply copy everything - delete entries
                    t->tloc[i].orig_label = copy_string(t->tloc[i].label, NULL);
                    t->tloc[i].label = copy_string(t->tloc[i].label, NULL);
                }
            }
            else
            {
                errmsg(tr("Not enough tick locations specified").toLocal8Bit().constData());
                free_graph_tickmarks(t);
                ApplyError=true;
                return RETURN_FAILURE;
            }
        }//end for-loop
    //}//end if !TICK_SPEC_NONE

    switch (applyto)
    {
    case 0:                     /* current axis */
        axis_start = curaxis;
        axis_stop  = curaxis;
        graph_start = cg;
        graph_stop  = cg;
        break;
    case 1:                     /* all axes, current graph */
        axis_start = 0;
        axis_stop  = MAXAXES - 1;
        graph_start = cg;
        graph_stop  = cg;
        break;
    case 2:                     /* current axis, all graphs */
        axis_start = curaxis;
        axis_stop  = curaxis;
        graph_start = 0;
        graph_stop  = number_of_graphs() - 1;
        break;
    case 3:                     /* all axes, all graphs */
        axis_start = 0;
        axis_stop  = MAXAXES - 1;
        graph_start = 0;
        graph_stop  = number_of_graphs() - 1;
        break;
    case 4:                     /* x+y axes, current graph */
        axis_start = 0;
        axis_stop  = 1;
        graph_start = cg;
        graph_stop  = cg;
        break;
    case 5:                     /* x+y axes, all graphs */
        axis_start = 0;
        axis_stop  = 1;
        graph_start = 0;
        graph_stop  = number_of_graphs() - 1;
        break;
    case 6:                     /* alt x+y axes, current graph */
        axis_start = 2;
        axis_stop  = 3;
        graph_start = cg;
        graph_stop  = cg;
        break;
    case 7:                     /* alt x+y axes, all graphs */
        axis_start = 2;
        axis_stop  = 3;
        graph_start = 0;
        graph_stop  = number_of_graphs() - 1;
        break;
    default:
        axis_start = curaxis;
        axis_stop  = curaxis;
        graph_start = cg;
        graph_stop  = cg;
        break;
    }

    if (xv_evalexpr(ledStart,&axestart)!=RETURN_SUCCESS || xv_evalexpr(ledStop,&axestop)!=RETURN_SUCCESS)
    {
        errmsg(tr("Axis start/stop values undefined").toLocal8Bit().constData());
        free_graph_tickmarks(t);
        ApplyError=true;
        return RETURN_FAILURE;
    }

    //Undo-Stuff
    SaveTickmarksStatesPrevious(axis_start,axis_stop,graph_start,graph_stop);
    ListOfChanges.clear();
    ListOfOldStates.clear();
    t3=copy_graph_tickmarks(t);//copy the original t to t3 in order to be able to modify some settings in t before applying it

    for (i = graph_start; i <= graph_stop; i++)
    {
        for (j = axis_start; j <= axis_stop; j++)
        {
            switch (j)
            {
            case 0:
                strcpy(descr_axis,"xaxis");
                break;
            case 1:
                strcpy(descr_axis,"yaxis");
                break;
            case 2:
                strcpy(descr_axis,"altxaxis");
                break;
            case 3:
                strcpy(descr_axis,"altyaxis");
                break;
            }
            free_graph_tickmarks(t);
            t=copy_graph_tickmarks(t3);//renew the copy
            t2 = get_graph_tickmarks(i,j);

            tt_l=ta_l=true;//usually everything is applied
            if (apply_to_selection > 0 && (transfer_tick_labels==FALSE || transfer_label==FALSE))
            {
            tt_l=(transfer_tick_labels==FALSE?false:true);
            ta_l=(transfer_label==FALSE?false:true);
            }
            if (curaxis==j && cg==i)//in the case of the current axis: apply everything (regardless)
            {
            tt_l=ta_l=true;
            }
            //override some settings if no transfer of some settings wanted
            ret=copy_part_of_graph_tickmarks(t,t2,ta_l==true?FALSE:TRUE,tt_l==true?FALSE:TRUE);
            if (ret==RETURN_FAILURE)
            {
            errmsg((tr("Unable to copy parts of tickmark-settings of graph G")+QString::number(cg)+QString(", ")+QString(descr_axis)).toLocal8Bit().constData());
            }
            //t2 is the original (before 'Apply')
            //t3 is a copy of the new settings as set in the dialog (everything as set in the dialog)
            //t is the new axis-setting (similar to t3)

            sprintf(dummy,"with g%d",i);
            ListOfChanges << QString(dummy);
            ListOfOldStates << QString(dummy);
            get_graph_world(i, &w2);//w2 is the original
            get_graph_world(i, &w);
            if (is_xaxis(j))
            {
                w.xg1 = axestart;
                w.xg2 = axestop;
            }
            else
            {
                w.yg1 = axestart;
                w.yg2 = axestop;
            }
            set_graph_world(i, w);
            if (w.xg1!=w2.xg1 || w.xg2!=w2.xg2 || w.yg1!=w2.yg1 || w.yg2!=w2.yg2)
            {
                sprintf(dummy,"    world %f, %f, %f, %f",w.xg1,w.yg1,w.xg2,w.yg2);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    world %f, %f, %f, %f",w2.xg1,w2.yg1,w2.xg2,w2.yg2);
                ListOfOldStates << QString(dummy);
            }
            scale = selScale->currentValue();
            switch (scale)
            {
            case 0:
                strcpy(dummy2,"Normal");
                break;
            case 1:
                strcpy(dummy2,"Logarithmic");
                break;
            case 2:
                strcpy(dummy2,"Reciprocal");
                break;
            case 3:
                strcpy(dummy2,"Logit");
                break;
            }
            if (is_xaxis(j))
            {
                if (g[i].xscale!=scale)
                {
                    sprintf(dummy,"    xaxes scale %s",dummy2);
                    ListOfChanges << QString(dummy);
                    switch (g[i].xscale)
                    {
                    case 0:
                        strcpy(dummy2,"Normal");
                        break;
                    case 1:
                        strcpy(dummy2,"Logarithmic");
                        break;
                    case 2:
                        strcpy(dummy2,"Reciprocal");
                        break;
                    case 3:
                        strcpy(dummy2,"Logit");
                        break;
                    }
                    sprintf(dummy,"    xaxes scale %s",dummy2);
                    ListOfOldStates << QString(dummy);
                }
                set_graph_xscale(i, scale);
            }
            else
            {
                if (g[i].yscale!=scale)
                {
                    sprintf(dummy,"    yaxes scale %s",dummy2);
                    ListOfChanges << QString(dummy);
                    switch (g[i].yscale)
                    {
                    case 0:
                        strcpy(dummy2,"Normal");
                        break;
                    case 1:
                        strcpy(dummy2,"Logarithmic");
                        break;
                    case 2:
                        strcpy(dummy2,"Reciprocal");
                        break;
                    case 3:
                        strcpy(dummy2,"Logit");
                        break;
                    }
                    sprintf(dummy,"    yaxes scale %s",dummy2);
                    ListOfOldStates << QString(dummy);
                }
                set_graph_yscale(i, scale);
            }

            invert = chkInvAxis->isChecked();
            strcpy(dummy2,invert?"on":"off");
            if (is_xaxis(j))
            {
                if (g[i].xinvert!=invert)
                {
                    sprintf(dummy,"    xaxes invert %s",dummy2);
                    ListOfChanges << QString(dummy);
                    strcpy(dummy2,g[i].xinvert?"on":"off");
                    sprintf(dummy,"    xaxes invert %s",dummy2);
                    ListOfOldStates << QString(dummy);
                }
                set_graph_xinvert(i, invert);
            }
            else
            {
                if (g[i].yinvert!=invert)
                {
                    sprintf(dummy,"    yaxes invert %s",dummy2);
                    ListOfChanges << QString(dummy);
                    strcpy(dummy2,g[i].yinvert?"on":"off");
                    sprintf(dummy,"    yaxes invert %s",dummy2);
                    ListOfOldStates << QString(dummy);
                }
                set_graph_yinvert(i, invert);
            }
            switch(t->tl_op)
            {
            case PLACEMENT_NORMAL:
                strcpy(dummy2,"normal");
                break;
            case PLACEMENT_OPPOSITE:
                strcpy(dummy2,"opposite");
                break;
            case PLACEMENT_BOTH:
                strcpy(dummy2,"both");
                break;
            }
            if (t2->tl_op!=t->tl_op)
            {
                sprintf(dummy,"    %s  label place %s",descr_axis,dummy2);
                ListOfChanges << QString(dummy);
                switch(t2->tl_op)
                {
                case PLACEMENT_NORMAL:
                    strcpy(dummy2,"normal");
                    break;
                case PLACEMENT_OPPOSITE:
                    strcpy(dummy2,"opposite");
                    break;
                case PLACEMENT_BOTH:
                    strcpy(dummy2,"both");
                    break;
                }
                sprintf(dummy,"    %s  label place %s",descr_axis,dummy2);
                ListOfOldStates << QString(dummy);
            }
            switch(t->t_op)
            {
            case PLACEMENT_NORMAL:
                strcpy(dummy2,"normal");
                break;
            case PLACEMENT_OPPOSITE:
                strcpy(dummy2,"opposite");
                break;
            case PLACEMENT_BOTH:
                strcpy(dummy2,"both");
                break;
            }
            if (t2->t_op!=t->t_op)
            {
                sprintf(dummy,"    %s  tick place %s",descr_axis,dummy2);
                ListOfChanges << QString(dummy);
                switch(t2->t_op)
                {
                case PLACEMENT_NORMAL:
                    strcpy(dummy2,"normal");
                    break;
                case PLACEMENT_OPPOSITE:
                    strcpy(dummy2,"opposite");
                    break;
                case PLACEMENT_BOTH:
                    strcpy(dummy2,"both");
                    break;
                }
                sprintf(dummy,"    %s  tick place %s",descr_axis,dummy2);
                ListOfOldStates << QString(dummy);
            }
            if (t->active!=t2->active)// && applyto==0)//active will only be changed if we apply to current axis
            {
                sprintf(dummy,"    %s %s",descr_axis,t->active?"on":"off");
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s %s",descr_axis,t2->active?"on":"off");
                ListOfOldStates << QString(dummy);
            }
            if (t->zero!=t2->zero)
            {
                sprintf(dummy,"    %s  type zero %s",descr_axis,t->zero?"true":"false");
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  type zero %s",descr_axis,t2->zero?"true":"false");
                ListOfOldStates << QString(dummy);
            }
            if (t->tmajor!=t2->tmajor)
            {
                sprintf(dummy,"    %s  tick major %f",descr_axis,t->tmajor);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  tick major %f",descr_axis,t2->tmajor);
                ListOfOldStates << QString(dummy);
            }
            if (t->nminor!=t2->nminor)
            {
                sprintf(dummy,"    %s  tick minor ticks %d",descr_axis,t->nminor);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  tick minor ticks %d",descr_axis,t2->nminor);
                ListOfOldStates << QString(dummy);
            }
            if (t->tl_flag!=t2->tl_flag)
            {
                sprintf(dummy,"    %s  ticklabel %s",descr_axis,t->tl_flag?"on":"off");
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  ticklabel %s",descr_axis,t2->tl_flag?"on":"off");
                ListOfOldStates << QString(dummy);
            }
            if (t->t_flag!=t2->t_flag)
            {
                sprintf(dummy,"    %s  tick %s",descr_axis,t->t_flag?"on":"off");
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  tick %s",descr_axis,t2->t_flag?"on":"off");
                ListOfOldStates << QString(dummy);
            }
            if (t->t_drawbar!=t2->t_drawbar)
            {
                sprintf(dummy,"    %s  bar %s",descr_axis,t->t_drawbar?"on":"off");
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  bar %s",descr_axis,t2->t_drawbar?"on":"off");
                ListOfOldStates << QString(dummy);
            }
            if (t->t_drawbarlines!=t2->t_drawbarlines)
            {
                sprintf(dummy,"    %s  bar linestyle %d",descr_axis,t->t_drawbarlines);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  bar linestyle %d",descr_axis,t2->t_drawbarlines);
                ListOfOldStates << QString(dummy);
            }
            if (t->t_drawbarcolor!=t2->t_drawbarcolor)
            {
                sprintf(dummy,"    %s  bar color %d",descr_axis,t->t_drawbarcolor);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  bar color %d",descr_axis,t2->t_drawbarcolor);
                ListOfOldStates << QString(dummy);
            }
            if (t->t_drawbarlinew!=t2->t_drawbarlinew)
            {
                sprintf(dummy,"    %s  bar linewidth %f",descr_axis,t->t_drawbarlinew);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  bar linewidth %f",descr_axis,t2->t_drawbarlinew);
                ListOfOldStates << QString(dummy);
            }
            if (!(t->label.s_plotstring==NULL && t2->label.s_plotstring==NULL))
            {
                if ((t->label.s_plotstring!=NULL && t2->label.s_plotstring==NULL) || (t->label.s_plotstring==NULL && t2->label.s_plotstring!=NULL) || (t->label.s_plotstring!=NULL && t2->label.s_plotstring!=NULL && strcmp(t->label.s_plotstring,t2->label.s_plotstring)))
                {
                    sprintf(dummy,"    %s  label \"%s\"",descr_axis,t->label.s_plotstring);
                    ListOfChanges << QString(dummy);
                    sprintf(dummy,"    %s  label \"%s\"",descr_axis,t2->label.s_plotstring);
                    ListOfOldStates << QString(dummy);
                }
            }
            if (t->offsx!=t2->offsx || t->offsy!=t2->offsy)
            {
                sprintf(dummy,"    %s  offset %f , %f",descr_axis,t->offsx,t->offsy);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  offset %f , %f",descr_axis,t2->offsx,t2->offsy);
                ListOfOldStates << QString(dummy);
            }
            if (t->label_layout!=t2->label_layout)
            {
                sprintf(dummy,"    %s  label layout %s",descr_axis,t->label_layout?"para":"perp");
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  label layout %s",descr_axis,t2->label_layout?"para":"perp");
                ListOfOldStates << QString(dummy);
            }
            if (t->label_place!=t2->label_place)
            {
                sprintf(dummy,"    %s  label place %s",descr_axis,t->label_place==TYPE_SPEC?"spec":"auto");
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  label place %s",descr_axis,t2->label_place==TYPE_SPEC?"spec":"auto");
                ListOfOldStates << QString(dummy);
                if (t->label_place==TYPE_SPEC)
                {
                    sprintf(dummy,"    %s  label place %f, %f",descr_axis,t->label.x,t->label.y);
                    ListOfChanges << QString(dummy);
                }
                if (t2->label_place==TYPE_SPEC)
                {
                    sprintf(dummy,"    %s  label place %f, %f",descr_axis,t2->label.x,t2->label.y);
                    ListOfOldStates << QString(dummy);
                }
            }
            if (t->label.font!=t2->label.font)
            {
                sprintf(dummy,"    %s  label font %d",descr_axis,t->label.font);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  label font %d",descr_axis,t2->label.font);
                ListOfOldStates << QString(dummy);
            }
            if (t->label.color!=t2->label.color)
            {
                sprintf(dummy,"    %s  label color %d",descr_axis,t->label.color);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  label color %d",descr_axis,t2->label.color);
                ListOfOldStates << QString(dummy);
            }
            if (t->label.charsize!=t2->label.charsize)
            {
                sprintf(dummy,"    %s  label char size %f",descr_axis,t->label.charsize);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  label char size %f",descr_axis,t2->label.charsize);
                ListOfOldStates << QString(dummy);
            }
            if (t->label_op!=t2->label_op)
            {
                sprintf(dummy,"    %s  ticklabel place %s",descr_axis,t->label_op==PLACEMENT_NORMAL?"normal":(t->label_op==PLACEMENT_OPPOSITE?"opposite":"both"));
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  ticklabel place %s",descr_axis,t2->label_op==PLACEMENT_NORMAL?"normal":(t2->label_op==PLACEMENT_OPPOSITE?"opposite":"both"));
                ListOfOldStates << QString(dummy);
            }
            if (t->tl_font!=t2->tl_font)
            {
                sprintf(dummy,"    %s  ticklabel font %d",descr_axis,t->tl_font);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  ticklabel font %d",descr_axis,t2->tl_font);
                ListOfOldStates << QString(dummy);
            }
            if (t->tl_color!=t2->tl_color)
            {
                sprintf(dummy,"    %s  ticklabel color %d",descr_axis,t->tl_color);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  ticklabel color %d",descr_axis,t2->tl_color);
                ListOfOldStates << QString(dummy);
            }
            if (t->tl_skip!=t2->tl_skip)
            {
                sprintf(dummy,"    %s  ticklabel skip %d",descr_axis,t->tl_skip);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  ticklabel skip %d",descr_axis,t2->tl_skip);
                ListOfOldStates << QString(dummy);
            }
            if (t->tl_prec!=t2->tl_prec)
            {
                sprintf(dummy,"    %s  ticklabel prec %d",descr_axis,t->tl_prec);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  ticklabel prec %d",descr_axis,t2->tl_prec);
                ListOfOldStates << QString(dummy);
            }
            if (t->tl_staggered!=t2->tl_staggered)
            {
                sprintf(dummy,"    %s  ticklabel stagger %d",descr_axis,t->tl_staggered);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  ticklabel stagger %d",descr_axis,t2->tl_staggered);
                ListOfOldStates << QString(dummy);
            }
            //if (!(t->tl_appstr==NULL && t2->tl_appstr==NULL))
            //if ((t->tl_appstr!=NULL && t2->tl_appstr==NULL)|| (t->tl_appstr==NULL && t2->tl_appstr!=NULL) || (t->tl_appstr!=NULL && t2->tl_appstr!=NULL && strcmp(t->tl_appstr,t2->tl_appstr)))

                if (strcmp(t->tl_appstr,t2->tl_appstr))
                {
                    sprintf(dummy,"    %s  ticklabel append \"%s\"",descr_axis,t->tl_appstr);
                    ListOfChanges << QString(dummy);
                    sprintf(dummy,"    %s  ticklabel append \"%s\"",descr_axis,t2->tl_appstr);
                    ListOfOldStates << QString(dummy);
                }
             //if (!(t->tl_prestr==NULL && t2->tl_prestr==NULL))
             //if ((t->tl_prestr!=NULL && t2->tl_prestr==NULL) || (t->tl_prestr==NULL && t2->tl_prestr!=NULL) || (t->tl_prestr!=NULL && t2->tl_prestr!=NULL && strcmp(t->tl_prestr,t2->tl_prestr)))
                if (strcmp(t->tl_prestr,t2->tl_prestr))
                {
                    sprintf(dummy,"    %s  ticklabel prepend \"%s\"",descr_axis,t->tl_prestr);
                    ListOfChanges << QString(dummy);
                    sprintf(dummy,"    %s  ticklabel prepend \"%s\"",descr_axis,t2->tl_prestr);
                    ListOfOldStates << QString(dummy);
                }
            if (t->tl_starttype!=t2->tl_starttype)
            {
                sprintf(dummy,"    %s  ticklabel start type %s",descr_axis,t->tl_starttype==TYPE_AUTO?"auto":"spec");
                ListOfChanges << QString(dummy);
                if (t->tl_starttype!=TYPE_AUTO)
                {
                    sprintf(dummy,"    %s  ticklabel start %f",descr_axis,t->tl_start);
                    ListOfChanges << QString(dummy);
                }
                sprintf(dummy,"    %s  ticklabel start type %s",descr_axis,t2->tl_starttype==TYPE_AUTO?"auto":"spec");
                ListOfOldStates << QString(dummy);
                if (t2->tl_starttype!=TYPE_AUTO)
                {
                    sprintf(dummy,"    %s  ticklabel start %f",descr_axis,t2->tl_start);
                    ListOfOldStates << QString(dummy);
                }
            }
            if (t->tl_stoptype!=t2->tl_stoptype)
            {
                sprintf(dummy,"    %s  ticklabel stop type %s",descr_axis,t->tl_stoptype==TYPE_AUTO?"auto":"spec");
                ListOfChanges << QString(dummy);
                if (t->tl_stoptype!=TYPE_AUTO)
                {
                    sprintf(dummy,"    %s  ticklabel stop %f",descr_axis,t->tl_stop);
                    ListOfChanges << QString(dummy);
                }
                sprintf(dummy,"    %s  ticklabel stop type %s",descr_axis,t2->tl_stoptype==TYPE_AUTO?"auto":"spec");
                ListOfOldStates << QString(dummy);
                if (t2->tl_stoptype!=TYPE_AUTO)
                {
                    sprintf(dummy,"    %s  ticklabel stop %f",descr_axis,t2->tl_stop);
                    ListOfOldStates << QString(dummy);
                }
            }
            if (t->tl_format!=t2->tl_format)
            {
                sprintf(dummy,"    %s  ticklabel format %s",descr_axis,get_format_types(t->tl_format));
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  ticklabel format %s",descr_axis,get_format_types(t2->tl_format));
                ListOfOldStates << QString(dummy);
            }
            if (!(t->tl_formula==NULL && t2->tl_formula==NULL))
                if ((t->tl_formula==NULL && t2->tl_formula!=NULL) || (t->tl_formula!=NULL && t2->tl_formula==NULL) || (t->tl_formula!=NULL && t2->tl_formula!=NULL && strcmp(t->tl_formula,t2->tl_formula)))
                {
                    sprintf(dummy,"    %s  ticklabel formula \"%s\"",descr_axis,t->tl_formula);
                    ListOfChanges << QString(dummy);
                    sprintf(dummy,"    %s  ticklabel formula \"%s\"",descr_axis,t2->tl_formula);
                    ListOfOldStates << QString(dummy);
                }
            if (t->tl_gaptype!=t2->tl_gaptype)
            {
                sprintf(dummy,"    %s  ticklabel offset %s",descr_axis,t->tl_gaptype==TYPE_AUTO?"auto":"spec");
                ListOfChanges << QString(dummy);
                if (t->tl_gaptype == TYPE_SPEC)
                {
                    sprintf(dummy,"    %s  ticklabel offset %f , %f",descr_axis,t->tl_gap.x,t->tl_gap.y);
                    ListOfChanges << QString(dummy);
                }
                sprintf(dummy,"    %s  ticklabel offset %s",descr_axis,t2->tl_gaptype==TYPE_AUTO?"auto":"spec");
                ListOfOldStates << QString(dummy);
                if (t2->tl_gaptype == TYPE_SPEC)
                {
                    sprintf(dummy,"    %s  ticklabel offset %f , %f",descr_axis,t2->tl_gap.x,t2->tl_gap.y);
                    ListOfOldStates << QString(dummy);
                }
            }
            if (t->tl_angle!=t2->tl_angle)
            {
                sprintf(dummy,"    %s  ticklabel angle %d",descr_axis,t->tl_angle);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  ticklabel angle %d",descr_axis,t2->tl_angle);
                ListOfOldStates << QString(dummy);
            }
            if (t->tl_charsize!=t2->tl_charsize)
            {
                sprintf(dummy,"    %s  ticklabel char size %f",descr_axis,t->tl_charsize);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  ticklabel char size %f",descr_axis,t2->tl_charsize);
                ListOfOldStates << QString(dummy);
            }
            if (t->t_inout!=t2->t_inout)
            {
                sprintf(dummy,"    %s  tick %s",descr_axis,t->t_inout==TICKS_IN?"in":(t->t_inout==TICKS_OUT?"out":"both"));
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  tick %s",descr_axis,t2->t_inout==TICKS_IN?"in":(t2->t_inout==TICKS_OUT?"out":"both"));
                ListOfOldStates << QString(dummy);
            }
            if (t->props.color!=t2->props.color)
            {
                sprintf(dummy,"    %s  tick major color %d",descr_axis,t->props.color);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  tick major color %d",descr_axis,t2->props.color);
                ListOfOldStates << QString(dummy);
            }
            if (t->mprops.color!=t2->mprops.color)
            {
                sprintf(dummy,"    %s  tick minor color %d",descr_axis,t->mprops.color);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  tick minor color %d",descr_axis,t2->mprops.color);
                ListOfOldStates << QString(dummy);
            }
            if (t->props.linew!=t2->props.linew)
            {
                sprintf(dummy,"    %s  tick major linewidth %f",descr_axis,t->props.linew);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  tick major linewidth %f",descr_axis,t2->props.linew);
                ListOfOldStates << QString(dummy);
            }
            if (t->mprops.linew!=t2->mprops.linew)
            {
                sprintf(dummy,"    %s  tick minor linewidth %f",descr_axis,t->mprops.linew);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  tick minor linewidth %f",descr_axis,t2->mprops.linew);
                ListOfOldStates << QString(dummy);
            }
            if (t->props.lines!=t2->props.lines)
            {
                sprintf(dummy,"    %s  tick major linestyle %d",descr_axis,t->props.lines);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  tick major linestyle %d",descr_axis,t2->props.lines);
                ListOfOldStates << QString(dummy);
            }
            if (t->mprops.lines!=t2->mprops.lines)
            {
                sprintf(dummy,"    %s  tick minor linestyle %d",descr_axis,t->mprops.lines);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  tick minor linestyle %d",descr_axis,t2->mprops.lines);
                ListOfOldStates << QString(dummy);
            }
            if (t->props.size!=t2->props.size)
            {
                sprintf(dummy,"    %s  tick major size %f",descr_axis,t->props.size);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  tick major size %f",descr_axis,t2->props.size);
                ListOfOldStates << QString(dummy);
            }
            if (t->mprops.size!=t2->mprops.size)
            {
                sprintf(dummy,"    %s  tick minor size %f",descr_axis,t->mprops.size);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  tick minor size %f",descr_axis,t2->mprops.size);
                ListOfOldStates << QString(dummy);
            }
            if (t->t_autonum!=t2->t_autonum)
            {
                sprintf(dummy,"    %s  tick default %d",descr_axis,t->t_autonum);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  tick default %d",descr_axis,t2->t_autonum);
                ListOfOldStates << QString(dummy);
            }
            if (t->t_round!=t2->t_round)
            {
                sprintf(dummy,"    %s  tick place rounded %s",descr_axis,t->t_round?"true":"false");
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  tick place rounded %s",descr_axis,t2->t_round?"true":"false");
                ListOfOldStates << QString(dummy);
            }
            if (t->props.gridflag!=t2->props.gridflag)
            {
                sprintf(dummy,"    %s  tick major grid %s",descr_axis,t->props.gridflag?"on":"off");
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  tick major grid %s",descr_axis,t2->props.gridflag?"on":"off");
                ListOfOldStates << QString(dummy);
            }
            if (t->mprops.gridflag!=t2->mprops.gridflag)
            {
                sprintf(dummy,"    %s  tick minor grid %s",descr_axis,t->mprops.gridflag?"on":"off");
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  tick minor grid %s",descr_axis,t2->mprops.gridflag?"on":"off");
                ListOfOldStates << QString(dummy);
            }
            if (t->t_spec!=t2->t_spec || t->nticks!=t2->nticks)///TODO: Check for further small differences between t and t2 here
            {
                sprintf(dummy,"    %s  tick spec type %s",descr_axis,t->t_spec==TICKS_SPEC_NONE?"none":(t->t_spec==TICKS_SPEC_BOTH?"both":"ticks"));
                ListOfChanges << QString(dummy);
                if (t->t_spec!=TICKS_SPEC_NONE)
                {
                    sprintf(dummy,"    %s  tick spec %d",descr_axis,t->nticks);
                    ListOfChanges << QString(dummy);
                    for (int l=0;l<t->nticks;l++)
                    {
                        if (t->tloc[l].type==TICK_TYPE_MINOR)
                            strcpy(dummy2,"minor");
                        else
                            strcpy(dummy2,"major");
                        sprintf(dummy,"    %s  tick %s %d, %f",descr_axis,dummy2,l,t->tloc[l].wtpos);
                        ListOfChanges << QString(dummy);
                        if (t->t_spec == TICKS_SPEC_BOTH)
                        {
                            sprintf(dummy,"    %s  ticklabel %d, \"%s\"",descr_axis,l,t->tloc[l].label);
                            ListOfChanges << QString(dummy);
                        }
                    }
                }
                sprintf(dummy,"    %s  tick spec type %s",descr_axis,t2->t_spec==TICKS_SPEC_NONE?"none":(t2->t_spec==TICKS_SPEC_BOTH?"both":"ticks"));
                ListOfOldStates << QString(dummy);
            }
            if (t->tl_alpha != t2->tl_alpha || t->label.alpha != t2->label.alpha || t->t_drawbaralpha != t2->t_drawbaralpha || t->props.alpha != t2->props.alpha || t->mprops.alpha != t2->mprops.alpha)
            {
                sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: AXIS_ALPHA G %d A %d %s\n",i,j,create_list_of_arguments(5, t->tl_alpha,t->label.alpha,t->t_drawbaralpha,t->props.alpha,t->mprops.alpha));
                ListOfChanges << QString(dummy);
                sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: AXIS_ALPHA G %d A %d %s\n",i,j,create_list_of_arguments(5, t2->tl_alpha,t2->label.alpha,t2->t_drawbaralpha,t2->props.alpha,t2->mprops.alpha));
                ListOfOldStates << QString(dummy);
            }
            if (t->tl_align != t2->tl_align)
            {
                sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: AXIS_TL_ALIGN G %d A %d %d\n",i,j,t->tl_align);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: AXIS_TL_ALIGN G %d A %d %d\n",i,j,t2->tl_align);
                ListOfOldStates << QString(dummy);
            }
            if (t->label.align != t2->label.align)
            {
                sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: AXIS_LABEL_ALIGN G %d A %d %d\n",i,j,t->label.align);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"#QTGRACE_ADDITIONAL_PARAMETER: AXIS_LABEL_ALIGN G %d A %d %d\n",i,j,t2->label.align);
                ListOfOldStates << QString(dummy);
            }

            /*if (applyto>0 && !(i==get_cg() && j==curaxis))//apply to more than one axis and we are not on the current axis
            {
            tmp_active=t->active;
            t->active=t2->active;
            }*/
            set_graph_tickmarks(i, j, t);//actually do something!!!
            /*if (applyto>0 && !(i==get_cg() && j==curaxis))//apply to more than one axis and we are not on the current axis
            {
            t->active=tmp_active;
            }*/

            //the following commands are neccessary for LaTeX-support (because dynamic strings are coppied dynamically)
            if (i==get_cg() && j==curaxis)
            {
                tabMain->ledAxisLabel->SetTextToMemory(g[i].t[j]->label.s_plotstring,g[i].t[j]->label.alt_plotstring);
                tabTickLabels->ledAppend->SetTextToMemory(g[i].t[j]->tl_appstr,g[i].t[j]->orig_tl_appstr);
                tabTickLabels->ledPrepend->SetTextToMemory(g[i].t[j]->tl_prestr,g[i].t[j]->orig_tl_prestr);
                if (g[i].t[j]->t_spec == TICKS_SPEC_BOTH)
                {
                    for (int k=0;k<g[i].t[j]->nticks;k++)
                    {
                    SetLineEditToMemory(g[i].t[j]->tloc[k].label,g[i].t[j]->tloc[k].orig_label,tabSpecial->text[k],tabSpecial->orig_text[k],tabSpecial->original[k],tabSpecial->ledLabel[k]);
                    //tabSpecial->spreadSpecLabels->axislines[k]->SetTextToMemory(g[i].t[j]->tloc[k].label,g[i].t[j]->tloc[k].orig_label);
                    }
                }
            }

        }//end axes
    }//end graphs
    free_graph_tickmarks(t);
    //do not free t2 because it is just an address
    free_graph_tickmarks(t3);
    if (GlobalInhibitor==false)
        mainWin->mainArea->completeRedraw();
    update_ticks(cg);
    //Show actual list of changes
    /*cout << "changes" << endl;
for (i=0;i<ListOfChanges.size();i++)
cout << ListOfChanges.at(i).toLocal8Bit().constData() << endl;
    cout << "old states" << endl;
for (i=0;i<ListOfOldStates.size();i++)
cout << ListOfOldStates.at(i).toLocal8Bit().constData() << endl;*/
    //Undo-Stuff
    TickmarksModified(axis_start,axis_stop,graph_start,graph_stop);
    if (GlobalInhibitor==false)
    {
        ListOfChanges.clear();
        ListOfOldStates.clear();
    }
    return RETURN_SUCCESS;
}

/*
 * This CB services the axis "Scale" selector
 */
void frmAxis_Prop::axis_scale_cb(int value)
{
    ApplyError=false;
    int scale = value;
    double major_space, axestart, axestop;
    int auton;
    char buf[32];
    xv_evalexpr(tabMain->ledMajorSpacing, &major_space);
    xv_evalexpr(ledStart, &axestart);
    xv_evalexpr(ledStop,  &axestop);
    auton = 2+tabTickMarks->setAutotickDiv->currentIndex();
    switch (scale) {
    case SCALE_NORMAL:
        if (major_space <= 0.0) {
            sprintf(buf, "%g", (axestop - axestart)/auton);
            tabMain->ledMajorSpacing->setText(QString(buf));
        }
        tabMain->selMinTicks->setValue(1);/// Changed for 0.2.6
        break;
    case SCALE_LOG:
        if (axestart <= 0.0 && axestop <= 0.0) {
            errmsg(tr("Can't set logarithmic scale for negative coordinates").toLocal8Bit().constData());
            ApplyError=true;
            selScale->setCurrentValue(SCALE_NORMAL);
            return;
        } else if (axestart <= 0.0) {
            axestart = axestop/1.0e3;
            sprintf(buf, "%g", axestart);
            ledStart->setText(QString(buf));
        }
        tabMain->ledMajorSpacing->setText(QString("10"));
        tabMain->selMinTicks->setValue(9);
        break;
    case SCALE_LOGIT:
        if (axestart <= 0.0 && axestop <= 0.0) {
            errmsg(tr("Can't set logit scale for values outside 0 and 1").toLocal8Bit().constData());
            ApplyError=true;
            selScale->setCurrentValue(SCALE_NORMAL);//SetOptionChoice(axis_scale, SCALE_NORMAL);
            return;
        }
        if (axestart <= 0.0) {
            axestart = 0.1;
            sprintf(buf, "%g", axestart);
            ledStart->setText(QString(buf));
        }
        if (axestop >= 1.0) {
            axestop = 0.95;
            sprintf(buf, "%g", axestop);
            ledStop->setText(QString(buf));
        }
        if (major_space >= 1.0) {
            tabMain->ledMajorSpacing->setText(QString("0.6"));
        }
        break;
    }
}

/*
 * Fill 'Axes' dialog with values
 */
void frmAxis_Prop::update_ticks(int gno)
{
    bool old_upd;
    tickmarks *t;
    world w;
    char buf[128];
    int i;
    old_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;

//setWindowTitle(tr("QtGrace: Axis (G")+QString::number(gno)+QString(")"));

    redisplayContents();

    t = get_graph_tickmarks(gno, curaxis);
//qDebug() << "ticks=" << t->nticks;
    if (!t)
    {
        immediateUpdate=old_upd;
        updateRunning=false;
        return;
    }
    chkActive->setChecked(is_axis_active(gno, curaxis));
    if (is_axis_active(gno, curaxis) == FALSE)
    {
        tabs->setEnabled(false);
    }
    else
    {
        tabs->setEnabled(true);
    }
    selEdit->setCurrentValue(curaxis);
    tabMain->chkZeroAxis->setChecked(is_zero_axis(gno, curaxis));
    get_graph_world(gno, &w);
    if (is_xaxis(curaxis))
    {
        ledStart->setDoubleValue("%.16g", w.xg1);
        //sprintf(buf, "%.9g", w.xg1);
        //ledStart->setText(QString(buf));
        //xv_setstr(axis_world_start, buf);
        ledStop->setDoubleValue("%.16g", w.xg2);
        //sprintf(buf, "%.9g", w.xg2);
        //ledStop->setText(QString(buf));
        //xv_setstr(axis_world_stop, buf);
        selScale->setCurrentValue(get_graph_xscale(gno));
        chkInvAxis->setChecked(is_graph_xinvert(gno));
    }
    else
    {
        ledStart->setDoubleValue("%.16g", w.yg1);
        //sprintf(buf, "%.9g", w.yg1);
        //ledStart->setText(QString(buf));
        //xv_setstr(axis_world_start, buf);
        ledStop->setDoubleValue("%.16g", w.yg2);
        //sprintf(buf, "%.9g", w.yg2);
        //ledStop->setText(QString(buf));
        //xv_setstr(axis_world_stop, buf);
        selScale->setCurrentValue(get_graph_yscale(gno));
        chkInvAxis->setChecked(is_graph_yinvert(gno));
    }
    tabMain->ledOffsetNormal->setDoubleValue("%.4f",t->offsx);
    //sprintf(buf, "%.4f", t->offsx);
    //tabMain->ledOffsetNormal->setText(QString(buf));
    //xv_setstr(offx, buf);
    tabMain->ledOffsetOpposite->setDoubleValue("%.4f",t->offsy);
    //sprintf(buf, "%.4f", t->offsy);
    //tabMain->ledOffsetOpposite->setText(QString(buf));
    //xv_setstr(offy, buf);
    tabLabelsBars->selLayout->setCurrentIndex(t->label_layout == LAYOUT_PERPENDICULAR ? 1 : 0);
    tabLabelsBars->selLocation->setCurrentIndex(t->label_place);
    tabLabelsBars->ledParaOffs->setDoubleValue("%.4f",t->label.x);
    //tabLabelsBars->ledParaOffs->setText(QString(buf));
    //sprintf(buf, "%.4f", t->label.x);
    //xv_setstr(axislabelspec_para, buf);
    tabLabelsBars->ledPerpendOffs->setDoubleValue("%.4f",t->label.y);
    //tabLabelsBars->ledPerpendOffs->setText(QString(buf));
    //sprintf(buf, "%.4f", t->label.y);
    //xv_setstr(axislabelspec_perp, buf);
    tabLabelsBars->ledParaOffs->setEnabled(t->label_place == TYPE_SPEC);
    tabLabelsBars->ledPerpendOffs->setEnabled(t->label_place == TYPE_SPEC);
    tabLabelsBars->selLabelFont->setCurrentIndex(t->label.font);
    tabLabelsBars->selAlign->setCurrentValue(t->label.align);
    //qDebug() << "setting Label Alignment:" << t->label.align;
    tabLabelsBars->selLabelColor->setCurrentIndex(t->label.color);
    tabLabelsBars->selLabelColor->setAlpha(t->label.alpha);
    tabLabelsBars->sldCharSize->setValue((int)rint_2(100.0*t->label.charsize));
    switch(t->label_op)
    {
    case PLACEMENT_NORMAL:
        tabLabelsBars->selSide->setCurrentIndex(0);
        break;
    case PLACEMENT_OPPOSITE:
        tabLabelsBars->selSide->setCurrentIndex(1);
        break;
    case PLACEMENT_BOTH:
        tabLabelsBars->selSide->setCurrentIndex(2);
        break;
    }
    tabMain->chkDisplTickLabels->setChecked((bool)t->tl_flag);
    tabMain->chkDisplTickMarks->setChecked((bool)t->t_flag);
    tabMain->chkDisplAxixBar->setChecked((bool)t->t_drawbar);
    tabMain->ledAxisLabel->SetTextToMemory(g[gno].t[curaxis]->label.s_plotstring,g[gno].t[curaxis]->label.alt_plotstring);
    //tabMain->ledAxisLabel->setText(QString(t->label.s));
    if (is_log_axis(gno, curaxis))
    {
        if (t->tmajor <= 1.0)
        {
            t->tmajor = 10.0;
        }
        //sprintf(buf, "%g", t->tmajor);
        tabMain->ledMajorSpacing->setDoubleValue("%g",t->tmajor);
    }
    else if (is_logit_axis(gno, curaxis))
    {
        if (t->tmajor <= 0.0)
        {
            t->tmajor = 0.1;
        }
        else if (t->tmajor >= 0.5)
        {
            t->tmajor = 0.4;
        }
        tabMain->ledMajorSpacing->setDoubleValue("%g",t->tmajor);
        //sprintf(buf, "%g", t->tmajor);
    }
    else if (t->tmajor > 0)
    {
        tabMain->ledMajorSpacing->setDoubleValue("%g",t->tmajor);
        //sprintf(buf, "%g", t->tmajor);
    }
    else
    {
        tabMain->ledMajorSpacing->setText(tr("UNDEFINED"));
    }
    //tabMain->ledMajorSpacing->setText(QString(buf));
    //xv_setstr(tmajor, buf);
    tabMain->selMinTicks->setValue(t->nminor);
    //SetSpinChoice(nminor, t->nminor);
    tabMain->selTickLabelFont->setCurrentIndex(t->tl_font);
    tabMain->selTickLabelColor->setCurrentIndex(t->tl_color);
    tabMain->selTickLabelColor->setAlpha(t->tl_alpha);
    tabTickLabels->selSkipEvery->setCurrentIndex(t->tl_skip);
    tabTickLabels->selStagger->setCurrentIndex(t->tl_staggered);

    //tabTickLabels->ledAppend->setText(QString(t->tl_appstr));
    //tabTickLabels->ledPrepend->setText(QString(t->tl_prestr));
    tabTickLabels->ledAppend->SetTextToMemory(g[gno].t[curaxis]->tl_appstr,g[gno].t[curaxis]->orig_tl_appstr);
    tabTickLabels->ledPrepend->SetTextToMemory(g[gno].t[curaxis]->tl_prestr,g[gno].t[curaxis]->orig_tl_prestr);

    tabTickLabels->selStartAt->setCurrentIndex(t->tl_starttype == TYPE_SPEC?1:0);
    /*xv_setstr(tlappstr, t->tl_appstr);
        xv_setstr(tlprestr, t->tl_prestr);*/
    //SetChoice(tlstarttype, t->tl_starttype == TYPE_SPEC);
    if (t->tl_starttype == TYPE_SPEC)
    {
        sprintf(buf, "%g", t->tl_start);
        SetDecimalSeparatorToUserValue(buf);
        tabTickLabels->ledStart->setText(QString(buf));
    }
    /*else
    tabTickLabels->ledStart->setText("");*/
    tabTickLabels->selStopAt->setCurrentIndex(t->tl_stoptype== TYPE_SPEC?1:0);
    if (t->tl_stoptype == TYPE_SPEC)
    {
        sprintf(buf, "%g", t->tl_stop);
        SetDecimalSeparatorToUserValue(buf);
        tabTickLabels->ledStop->setText(QString(buf));
    }
    /*else
    tabTickLabels->ledStop->setText("");*/
    tabMain->selFormat->setCurrentIndex(t->tl_format);
    tabMain->selPrecision->setCurrentIndex(t->tl_prec);
    if (t->tl_formula!=NULL)
        strcpy(buf,t->tl_formula);
    else
        strcpy(buf,"");
    RedisplayFormula(buf);
    tabTickLabels->ledAxisTransform->setText(QString(buf));
    switch (t->tl_op)
    {
    case PLACEMENT_NORMAL:
        tabTickLabels->selSide->setCurrentIndex(0);
        break;
    case PLACEMENT_OPPOSITE:
        tabTickLabels->selSide->setCurrentIndex(1);
        break;
    case PLACEMENT_BOTH:
        tabTickLabels->selSide->setCurrentIndex(2);
        break;
    }
    tabTickLabels->selLocation->setCurrentIndex(t->tl_gaptype==TYPE_AUTO?0:1);
    tabTickLabels->ledParaOffs->setDoubleValue("%.4f",t->tl_gap.x);
    //sprintf(buf, "%.4f", t->tl_gap.x);
    //tabTickLabels->ledParaOffs->setText(QString(buf));
    //xv_setstr(tlgap_para, buf);
    tabTickLabels->ledPerpendOffs->setDoubleValue("%.4f",t->tl_gap.y);
    //sprintf(buf, "%.4f", t->tl_gap.y);
    //tabTickLabels->ledPerpendOffs->setText(QString(buf));
    //xv_setstr(tlgap_perp, buf);
    tabTickLabels->ledParaOffs->setEnabled(t->tl_gaptype == TYPE_SPEC);
    tabTickLabels->ledPerpendOffs->setEnabled(t->tl_gaptype == TYPE_SPEC);
    tabTickLabels->sldCharAngle->setValue(t->tl_angle);
    tabTickLabels->sldCharSize->setValue((int)rint_2(t->tl_charsize*100.0));
    tabTickLabels->selAlign->setCurrentValue(t->tl_align);
    //qDebug() << "setting Tick Label Alignment:" << t->tl_align;
    switch (t->t_inout)
    {
    case TICKS_IN:
        tabTickMarks->selPointing->setCurrentIndex(0);
        break;
    case TICKS_OUT:
        tabTickMarks->selPointing->setCurrentIndex(1);
        break;
    case TICKS_BOTH:
        tabTickMarks->selPointing->setCurrentIndex(2);
        break;
    }
    switch (t->t_op)
    {
    case PLACEMENT_NORMAL:
        tabTickMarks->selDrawOn->setCurrentIndex(0);
        break;
    case PLACEMENT_OPPOSITE:
        tabTickMarks->selDrawOn->setCurrentIndex(1);
        break;
    case PLACEMENT_BOTH:
        tabTickMarks->selDrawOn->setCurrentIndex(2);
        break;
    }
    tabTickMarks->selMajTickColor->setCurrentIndex(t->props.color);
    tabTickMarks->selMajTickColor->setAlpha(t->props.alpha);
    tabTickMarks->selMajTickWidth->setValue(t->props.linew);
    tabTickMarks->selMajTickStyle->setCurrentIndex(t->props.lines);
    tabTickMarks->selMinTickColor->setCurrentIndex(t->mprops.color);
    tabTickMarks->selMinTickColor->setAlpha(t->mprops.alpha);
    tabTickMarks->selMinTickWidth->setValue(t->mprops.linew);
    tabTickMarks->selMinTickStyle->setCurrentIndex(t->mprops.lines);
    tabTickMarks->sldMajTickLength->setValue((int)rint_2(100.0*t->props.size));
    tabTickMarks->sldMinTickLength->setValue((int)rint_2(100.0*t->mprops.size));
    tabTickMarks->setAutotickDiv->setCurrentIndex(t->t_autonum - 2);
    tabTickMarks->chkPlaceRoundPos->setChecked((bool)t->t_round);
    tabTickMarks->chkDrawMajGrid->setChecked((bool)t->props.gridflag);
    tabTickMarks->chkDrawMinGrid->setChecked((bool)t->mprops.gridflag);
    tabLabelsBars->selBarColor->setCurrentIndex(t->t_drawbarcolor);
    tabLabelsBars->selBarColor->setAlpha(t->t_drawbaralpha);
    tabLabelsBars->selBarWidth->setValue(t->t_drawbarlinew);
    tabLabelsBars->selBarStyle->setCurrentIndex(t->t_drawbarlines);
    tabSpecial->selSpecTicks->setCurrentValue(t->t_spec);
    tabSpecial->selNumber->setValue(t->nticks);
char * ni=NULL;
    for (i = 0; i < t->nticks; i++)
    {
        sprintf(buf, "%.9g", t->tloc[i].wtpos);
        SetDecimalSeparatorToUserValue(buf);
        //tabSpecial->spreadSpecLabels->axislines[i]->ledLocation->setText(QString(buf));
        tabSpecial->ledLocation[i]->setText(QString(buf));
        //xv_setstr(specloc[i], buf);
        if (t->tloc[i].type == TICK_TYPE_MAJOR)
        {
        SetLineEditToMemory(g[gno].t[curaxis]->tloc[i].label,g[gno].t[curaxis]->tloc[i].orig_label,tabSpecial->text[i],tabSpecial->orig_text[i],tabSpecial->original[i],tabSpecial->ledLabel[i]);
        //tabSpecial->spreadSpecLabels->axislines[i]->SetTextToMemory(g[gno].t[curaxis]->tloc[i].label,g[gno].t[curaxis]->tloc[i].orig_label);
            //tabSpecial->spreadSpecLabels->axislines[i]->ledLabel->setText(QString(t->tloc[i].label));
            //xv_setstr(speclabel[i], t->tloc[i].label);
        }
        else
        {
        SetLineEditToMemory(ni,ni,tabSpecial->text[i],tabSpecial->orig_text[i],tabSpecial->original[i],tabSpecial->ledLabel[i]);
        //tabSpecial->spreadSpecLabels->axislines[i]->SetTextToMemory(ni,ni);
            //tabSpecial->spreadSpecLabels->axislines[i]->ledLabel->setText(QString(""));
            //xv_setstr(speclabel[i], "");
        }
    }
    strcpy(buf,"");
    for (i = (t->nticks>=0?t->nticks:0); i < MAX_TICKS; i++)
    {
    SetLineEditToMemory(ni,ni,tabSpecial->text[i],tabSpecial->orig_text[i],tabSpecial->original[i],tabSpecial->ledLabel[i]);
    //tabSpecial->spreadSpecLabels->axislines[i]->SetTextToMemory(ni,ni);//only affects the label
        tabSpecial->ledLocation[i]->setText(QString(buf));
        //tabSpecial->spreadSpecLabels->axislines[i]->ledLocation->setText(QString(buf));
        //tabSpecial->spreadSpecLabels->axislines[i]->ledLabel->setText(QString(buf));
    }
    immediateUpdate=old_upd;
    updateRunning=false;
}

void frmAxis_Prop::set_active_proc(int onoff)
{
    tabs->setEnabled((bool)onoff);
}

void frmAxis_Prop::set_axis_proc(int value)
{
    curaxis = value;
    update_ticks(cg);
}

void frmAxis_Prop::create_axes_dialog(int axisno)
{
    if (axisno >= 0 && axisno < MAXAXES)
    {
        curaxis = axisno;
    }
    update_ticks(cg);
}

void frmAxis_Prop::redisplayContents(void)
{
    if (DecimalPointToUse=='.')
    {
        tabLabelsBars->selBarWidth->spnLineWidth->setLocale(*dot_locale);
        tabTickMarks->selMajTickWidth->spnLineWidth->setLocale(*dot_locale);
        tabTickMarks->selMinTickWidth->spnLineWidth->setLocale(*dot_locale);
    }
    else
    {
        tabLabelsBars->selBarWidth->spnLineWidth->setLocale(*comma_locale);
        tabTickMarks->selMajTickWidth->spnLineWidth->setLocale(*comma_locale);
        tabTickMarks->selMinTickWidth->spnLineWidth->setLocale(*comma_locale);
    }
    //if (OldDecimalPoint==DecimalPointToUse) return;
    tabMain->ledMajorSpacing->ReplaceNumberContents();
    ledStart->ReplaceNumberContents();
    ledStop->ReplaceNumberContents();
    tabMain->ledOffsetNormal->ReplaceNumberContents();
    tabMain->ledOffsetOpposite->ReplaceNumberContents();
    tabLabelsBars->ledParaOffs->ReplaceNumberContents();
    tabLabelsBars->ledPerpendOffs->ReplaceNumberContents();
    tabLabelsBars->selBarWidth->setValue(tabLabelsBars->selBarWidth->value());
    tabTickLabels->ledParaOffs->ReplaceNumberContents();
    tabTickLabels->ledPerpendOffs->ReplaceNumberContents();
    QString te=tabTickLabels->ledAxisTransform->text();
    strcpy(dummy,te.toLatin1().constData());
    RedisplayFormula(dummy);
    tabTickLabels->ledAxisTransform->setText(QString(dummy));/// Formula-transformation???
    te=tabTickLabels->ledStart->text();
    Replace_Dec_Sep_In_Single_String(te);
    //te.replace(QChar(OldDecimalPoint),QChar(DecimalPointToUse));
    tabTickLabels->ledStart->setText(te);/// Formula-transformation???
    te=tabTickLabels->ledStop->text();
    Replace_Dec_Sep_In_Single_String(te);
    //te.replace(QChar(OldDecimalPoint),QChar(DecimalPointToUse));
    tabTickLabels->ledStop->setText(te);/// Formula-transformation???
    tabTickMarks->selMajTickWidth->setValue(tabTickMarks->selMajTickWidth->value());
    tabTickMarks->selMinTickWidth->setValue(tabTickMarks->selMinTickWidth->value());
    //for (int i = 0; i < tabSpecial->spreadSpecLabels->rows; i++)
    for (int i = 0; i < MAX_TICKS; i++)
    {
        //te=tabSpecial->spreadSpecLabels->axislines[i]->ledLocation->text();
        te=tabSpecial->ledLocation[i]->text();
        if (!te.isEmpty())
        {
            //te.replace(QChar(OldDecimalPoint),QChar(DecimalPointToUse));
            Replace_Dec_Sep_In_Single_String(te);
            tabSpecial->ledLocation[i]->setText(te);
            //tabSpecial->spreadSpecLabels->axislines[i]->ledLocation->setText(te);
        }
    }

    tabMain->layout0->update();
    tabMain->layout1->update();
    tabMain->layout2->update();
    tabMain->layout3->update();
    tabMain->layout4->update();
    tabMain->layout->update();

    tabLabelsBars->layout1->update();
    tabLabelsBars->layout2->update();
    tabLabelsBars->layout->update();

    tabTickLabels->layout0->update();
    tabTickLabels->layout1->update();
    tabTickLabels->layout2->update();
    tabTickLabels->layout3->update();
    tabTickLabels->layout->update();

    tabTickMarks->layout0->update();
    tabTickMarks->layout1->update();
    tabTickMarks->layout2->update();
    tabTickMarks->layout->update();

    tabSpecial->layout0->update();
    tabSpecial->layout->update();
}

void frmAxis_Prop::doQuickNormalTicks(void)
{
int cur_gr=get_cg();
//int sav_undo_active=undo_active;
    //Undo-Stuff
    SaveTickmarksStatesPrevious(curaxis,curaxis,cur_gr,cur_gr);
    ListOfChanges.clear();
    ListOfOldStates.clear();
//qDebug() << "Resetting ticks to default";
    quickSetAxisFormat(cur_gr,curaxis,QUICK_AXIS_TRANSFORM_NORMAL);
/*tabTickLabels->ledAxisTransform->setText(QString(""));
strcpy(g[cur_gr].t[curaxis]->tl_appstr,"");
strcpy(g[cur_gr].t[curaxis]->orig_tl_appstr,"");
tabTickLabels->ledAppend->SetTextToMemory(g[cur_gr].t[curaxis]->tl_appstr,g[cur_gr].t[curaxis]->orig_tl_appstr);*/

//undo_active=FALSE;
//mainWin->mainArea->compl_redraw_running=true;
doApply();
//autotick_axis(cur_gr,curaxis);
//mainWin->mainArea->compl_redraw_running=false;
//undo_active=sav_undo_active;
    //Undo-Stuff
/*    TickmarksModified(curaxis,curaxis,cur_gr,cur_gr);
    ListOfChanges.clear();
    ListOfOldStates.clear();
mainWin->mainArea->completeRedraw();*/
}

void frmAxis_Prop::doQuickDegreesTicks(void)
{
int cur_gr=get_cg();
    //Undo-Stuff
    SaveTickmarksStatesPrevious(curaxis,curaxis,cur_gr,cur_gr);
    ListOfChanges.clear();
    ListOfOldStates.clear();
//qDebug() << "Setting ticks to degrees";
    quickSetAxisFormat(cur_gr,curaxis,QUICK_AXIS_TRANSFORM_DEGREES);
/*tabTickLabels->ledAxisTransform->setText(QString("$t*180.0/PI"));
strcpy(g[cur_gr].t[curaxis]->tl_appstr,"\\c:");
strcpy(g[cur_gr].t[curaxis]->orig_tl_appstr,"\\c:");
tabTickLabels->ledAppend->SetTextToMemory(g[cur_gr].t[curaxis]->tl_appstr,g[cur_gr].t[curaxis]->orig_tl_appstr);
tabMain->ledMajorSpacing->setText(QString("PI/2"));*/
doApply();
}

void frmAxis_Prop::doQuickPisTricks(void)
{
int cur_gr=get_cg();
    //Undo-Stuff
    SaveTickmarksStatesPrevious(curaxis,curaxis,cur_gr,cur_gr);
    ListOfChanges.clear();
    ListOfOldStates.clear();
//qDebug() << "Setting ticks to Multiples of Pi";
    quickSetAxisFormat(cur_gr,curaxis,QUICK_AXIS_TRANSFORM_MULT_PI);
/*tabTickLabels->ledAxisTransform->setText(QString("$t/PI"));
strcpy(g[cur_gr].t[curaxis]->tl_appstr,"\\xp");//"\\c7\\C\\xp"
strcpy(g[cur_gr].t[curaxis]->orig_tl_appstr,"\\xp");
tabTickLabels->ledAppend->SetTextToMemory(g[cur_gr].t[curaxis]->tl_appstr,g[cur_gr].t[curaxis]->orig_tl_appstr);
tabMain->ledMajorSpacing->setText(QString("PI/2"));*/
doApply();
}

void frmAxis_Prop::doQuickAltTricks(void)
{
//int cur_gr=get_cg();
//Undo-Stuff
//SaveTickmarksStatesPrevious(curaxis,curaxis,cur_gr,cur_gr);
//ListOfChanges.clear();
//ListOfOldStates.clear();
    quickSetAxisFormat(get_cg(),curaxis,QUICK_AXIS_TRANSFORM_ALT_AXIS);
//doApply();
}

void frmAxis_Prop::update0(void)
{
    static int i,nr;//,errpos;
    bool imm_upd_sav;
    if (!immediateUpdate) return;
    imm_upd_sav=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    GlobalInhibitor=true;
    doApply();//do this without actually doing something
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
    immediateUpdate=imm_upd_sav;
    updateRunning=false;
}

void frmAxis_Prop::update1(int v)
{
    (void)v;
    update0();
}

void frmAxis_Prop::update2(QString v)
{
    (void)v;
    update0();
}

void frmAxis_Prop::update3(bool v)
{
    (void)v;
    update0();
}

void frmAxis_Prop::update4(double v)
{
    (void)v;
    update0();
}

frmAxisProp::frmAxisProp(QWidget * parent):QDialog(parent)
{
//setFont(*stdFont);
     min_w=545;
     min_h=623;
    setWindowTitle(tr("QtGrace: Axis"));
    setWindowIcon(QIcon(*GraceIcon));
    QVBoxLayout * layout=new QVBoxLayout;
    //layout->setMargin(0);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    flp=new frmAxis_Prop(this);
    connect(flp,SIGNAL(closeWish()),SLOT(close()));
    //layout->addWidget(flp);

    scroll=new QScrollArea(this);
    scroll->setWidget(flp);
    layout->addWidget(scroll);

     //bar_w=scroll->verticalScrollBar()->width();
     //bar_h=scroll->horizontalScrollBar()->height();
     bar_w=bar_h=20;//15
    setLayout(layout);
    //resize(min_w,min_h);
    resize(LastSize_FormAxisProperties);
}

frmAxisProp::~frmAxisProp()
{
    LastSize_FormAxisProperties=this->size();
}

void frmAxisProp::update_ticks(int gno)
{
    setWindowTitle(tr("QtGrace: Axis (G")+QString::number(gno)+QString(")"));
    flp->update_ticks(gno);
}

void frmAxisProp::create_axes_dialog(int axisno)
{
    setWindowTitle(tr("QtGrace: Axis (G")+QString::number(get_cg())+QString(")"));
    flp->create_axes_dialog(axisno);
}

void frmAxisProp::doAccept(void)
{
    flp->doAccept();
}

void frmAxisProp::doClose(void)
{
    hide();
}

void frmAxisProp::resizeEvent(QResizeEvent * event)
{
//cout << "resize: " << event->size().width() << "x" << event->size().height() << " bar_w=" << bar_w << " bar_h=" << bar_h << endl;
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
scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
this->setMinimumSize(min_w,min_h);
}
flp->resize(QSize(n_size_w,n_size_h));
}


