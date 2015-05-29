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

#include "fundamentals.h"
#include "MainWindow.h"
#include "allWidgets.h"
#include "undo_module.h"

extern MainWindow * mainWin;
extern frmTextProps * TextProps;
extern frmTextProps * EditTextProps;
extern frmLineProps * LineProps;
extern frmLineProps * EditLineProps;
extern frmEllipseProps * BoxProps;
extern frmEllipseProps * EditBoxProps;
extern frmEllipseProps * EllipseProps;
extern frmEllipseProps * EditEllipseProps;
extern frmSetEditor * FormSetEditor;
extern frmEditBlockData * FormEditBlockData;
//extern frmEditColumnProp * EditColumn;
extern QPixmap * GraceIcon;
extern QPixmap * ActiveIcon;
extern QPixmap * HiddenIcon;
extern QIcon ** ColorIcons;
extern QPixmap ** ColorPixmaps;
extern QString ** ColorNames;
/*extern QIcon * LineIcons[MAXLINESTYLES];
extern QPixmap * LinePixmaps[MAXLINESTYLES];*/
extern int nr_of_current_linestyles;
extern int * lenghts_of_linestyle_patterns;
extern char ** current_linestyle_patterns;
extern QIcon ** LineIcons;
extern QPixmap ** LinePixmaps;
extern QVector<qreal> ** PenDashPattern;
extern QBitmap * patterns[MAXPATTERNS];
extern QPixmap * PatternPixmaps[MAXPATTERNS];
extern QPixmap * Qt_justifications[12];
extern QPixmap * Qt_matrixOrder[8];

extern bool useQtFonts;
extern QList<QFont> stdFontList;
extern QFontMetrics * stdFontMetrics;
extern bool activateLaTeXsupport;
extern QStringList ListOfChanges;
extern QStringList ListOfOldStates;
extern CMap_entry *cmap_table;
extern int allocated_colors;

extern char user_home_dir[1024];
extern char qt_grace_exe_dir[1024];
extern graph * g;
extern int maxgraph;
extern int new_set_no;
extern int DefaultFont;

extern void strToUpper(char * tar,char * ch);
extern void add_ColorSelector(ColorSelector * colSel);
extern void add_FontSelector(FontSelector * fontSel);
extern void add_LaTeX_Line(stdLineEdit * line);
extern void add_Line_Style_Selector(LineStyleSelector * line);
extern void update_font_selectors(bool appearance);
extern void update_graph_selectors(void);
extern void showSetInSpreadSheet(int gno,int setno);
void close_ss_editor(int gno,int setno);
extern frmLoadEval * FormLoadAndEvaluate;

extern void add_SetSelector(uniList * setSel);
extern void add_GraphSelector(uniList * grSel);
extern void add_SetChoiceItem(uniList * setSel);
extern void HelpCB(char *data);
extern char dummy[];//universal dummy for general purposes
extern char dummy2[];
QColor * stdTextColor=NULL;

extern char SystemsDecimalPoint;//the default decimal-separator
extern char OldDecimalPoint;
extern char DecimalPointToUse;//what the user wants to use as decimal separator

extern QTextCodec * FileCodec;
extern bool updateRunning;

extern int maxboxes;
extern int maxlines;
extern int maxstr;
extern int maxellipses;

///todo: check if this is realy needed
QStringList LaTeXCommands;
QStringList equivalentCommands;
QList<char> equivalentFont;

extern int recursive_replacer(QString & text);
extern void GeneralPaste(const QMimeData * mimeData);
extern void complete_LaTeX_to_Grace_Translator(QString & text);
extern QString generateDisplayStringFromGraceString(char * gracestring);
extern void generate_string_Qt_aware(char * string,QString text);

extern int find_QtFont_in_List(char * name,int whatlist);//you need a qt-font-name for this
extern int find_QtFont_in_List(QString fontname,int whatlist);//finds a qt-font-name
extern int find_default_id_of_Grace_Font(char * name);//you need a Grace-font-name for this
extern int find_GraceFontID_of_QtFontName(QString fontname);//you need a qt-font-name for this
extern QString getNameOfDefaultQtFont(int index);
extern QString get_QtName_of_Default_Grace_Font(char * name);//compares a Grace-font-name with the default font names and returns the qt-font-name
extern char * get_Grace_Font_Name_of_Default_Qt_Font(QString fontname);//returns the Grace-font-name of a Qt-Font-name
extern char * getNameOfStdQtFont(int index);//returns the Qt-Name of the Font in the StdList
extern char * getFamilyNameOfStdQtFont(int index);//returns the Qt-Name of the Font in the StdList
//this is what you need
//--> remember: whatlist=0-->default-list, whatlist=1-->std-list (i.e. the current list - you probably want to use this)
//--> convert Grace-name into QtFontID
extern int get_QtFontID_from_Grace_Name(char * name,int whatlist);//get the index of a QtFont in a list by its Grace-name
//--> convert Grace-font-id into QtFontID
extern int get_QtFontID_from_GraceID(int font_id,int whatlist);

extern void copySet(int gno,int setno,plotarr * pa,int what);
extern void reinstallSet(int gno,int setno,plotarr * pa,int what);
extern void deleteSavedSet(plotarr * pa,int what);
extern bool dont_delete_saved_set_memory;

int nr_of_set_app_saved=0;
int nr_of_set_app_allocated=0;
plotarr * saved_set_app=NULL;

extern int nr_of_single_f_tokens;
extern class single_formula_token * formula_tokens;

int slider_status=0;//0=slider inactive; 1=sliding started; 2=slider start accepted; 3=sliding finished

using namespace std;

/* lookup table to determine if character is a floating point digit
 * only allowable char's [0-9.eE]
 */
unsigned char fpdigit[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void getSetIDFromText(char * text,int & gno,int & sno,int & column)
{
//tries to read a set-id from the text, if incomplete id,
//the gno/sno-values stay untouched (gno/sno should be initialized
//with a suitable default before this function is called or a useless
//value to detect unsuccessfull read-attempts)
int a,b;
char c_text[32];
cout << "text=" << text << endl;
if (sscanf(text,"G%d.S%d.%s",&a,&b,c_text)==3)
{
    if (c_text[0]=='X' || c_text[0]=='x')
    {
    column=DATA_X;
    }
    else if (c_text[0]=='y' || c_text[0]=='Y')
    {
    column=DATA_Y;
        if (c_text[1]!='\0')
        {
        column+=atoi(c_text+1);
        }
    }
cout << "0a: a=" << a << " b=" << b << " column=" << column << endl;
gno=a;
sno=b;
}
else if (sscanf(text,"S%d.%s",&b,c_text)==2)
{
    if (c_text[0]=='X' || c_text[0]=='x')
    {
    column=DATA_X;
    }
    else if (c_text[0]=='y' || c_text[0]=='Y')
    {
    column=DATA_Y;
        if (c_text[1]!='\0')
        {
        column+=atoi(c_text+1);
        }
    }
cout << "0b: b=" << b << " column=" << column << endl;
sno=b;
}
else if (sscanf(text,"G%d.S%d",&a,&b)==2)
{
cout << "1: a=" << a << " b=" << b << endl;
gno=a;
sno=b;
return;
}
else if (sscanf(text,"G%d",&a)==1)
{
cout << "2: a=" << a << endl;
gno=a;
return;
}
else if (sscanf(text,"S%d",&b)==1)
{
cout << "3: b=" << b << endl;
sno=b;
return;
}
}

void sort(int number,int * items)//bubble-sort (I know that I am stupid - but it works)
{
    int dummy;
    for (int i=0;i<number;i++)
    {
        for (int j=i+1;j<number;j++)
        {
            if (items[j]<items[i])
            {
                dummy=items[j];
                items[j]=items[i];
                items[i]=dummy;
            }
        }
    }
}

int indexOfFontInDatabase(QFont & f)
{
    QString s1=f.toString();
    QString s2;
    int index=-1;
    if (s1.compare(QString("Zapf,13,-1,5,50,0,0,0,0,0"))==0)//QString("Zapf Dingbats,10,-1,5,50,0,0,0,0,0")
    {
        //s1=QString("Zapf,13,-1,5,50,0,0,0,0,0");
        s1=QString("Zapf Dingbats,10,-1,5,50,0,0,0,0,0");
    }
    for (int i=0;i<stdFontList.length();i++)
    {
        s2=stdFontList.at(i).toString();
        if (s1.compare(s2)==0)
        {
            index=i;
            break;
        }
    }
    return index;
}

QFont getFontFromDatabase(int i)
{
    QFont fallback;
    if (stdFontList.length()==0)
    {
        if (fallback.fromString(QString("Times,10,-1,5,50,0,0,0,0,0"))==false)
        {
            fallback=qApp->font();
        }
        return fallback;
        //return QFont( QString("Times,10,-1,5,50,0,0,0,0,0"));//just for emergencies
    }
    else if (i<0 || i>=stdFontList.length())
        return stdFontList.at(0);
    else
        return stdFontList.at(i);
}

int addFontToDatabase(QFont & f)
{
    int font_index=indexOfFontInDatabase(f);
    if (font_index!=-1)//font already exists --> do not add this font
    {
        //cout << "font " << f.toString().toLatin1().constData() << " already exists=" << font_index << endl;
        return font_index;
    }
    else
    {
        //cout << "new font added=" << f.toString().toLatin1().constData() << endl;
        stdFontList << f;
        return stdFontList.length()-1;//new font has last index
    }
}

void append_to_storage(int * l,int ** storage,int n,int * new_entries)
{
    static int m1,m2;
    int * temp;
    //the memory size of storage automatically increases by DELTASIZE if needed
    //new_entries are appended; len is the current storage size, n the number of new numbers to append
    //if storage==NUll --> storage is empty, new storage space is allocated
    if (n<=0 || new_entries==NULL) return;
    if (*l<=0 && *storage!=NULL)
    {
        delete[] *storage;
        *storage=NULL;
        *l=0;
    }
    m1=(*l)/DELTASIZE;
    m2=(*l+n)/DELTASIZE;
    if (m1*DELTASIZE<*l) m1++;
    if (m2*DELTASIZE<*l+n) m2++;
    if (*storage==NULL)
    {
        *storage=new int[(m2)*DELTASIZE];
        *l=0;
    }
    else if (m2>m1)//more memory needed
    {
        temp=*storage;
        *storage=new int[(m2)*DELTASIZE];
        memcpy(*storage,temp,sizeof(int)*(m1)*DELTASIZE);
        delete[] temp;
    }//else: enough memory
    memcpy(*storage+*l,new_entries,sizeof(int)*n);
    *l+=n;
}

void SetDecimalSeparatorToUserValue(char * str,bool remove_space=true)//we assume a string containing a number like "2.15" and we want to change it to a number with the user selected decimal separator like "2,15"
{
    if (DecimalPointToUse=='.') return;//Nothing to do here --> everything is as it should be
    QString chain(str);
    if (remove_space)
        chain.remove(QString(" "));//remove any useless spaces
    chain.replace(QString("."),QString(","));//replace the '.'s
    strcpy(str,chain.toLatin1());
}

void ReplaceDecimalSeparator(char * str,bool remove_space=true)//replace current user specified decimal separator with the internally used '.'
{
    QString chain(str);
    //cout << "to replace = " << str << endl;
    if (remove_space)
        chain.remove(QString(" "));//remove any useless spaces
    if (DecimalPointToUse!='.')//the input contains ',' instead of '.' - but the system always expects '.'
    {
        chain.remove(QString("."));//at first: remove '.'
        chain.replace(QString(","),QString("."));//replace the ',' with '.'
    }
    else//DecimalPoint is '.' - any ',' are not useful
    {
        chain.remove(QString(","));//just remove all ','
    }
    strcpy(str,chain.toLatin1().constData());
    //cout << "result = " << str << endl;
}

void RedisplayString(char * str)//replaces the old decimal separator with the new one
{
    if (OldDecimalPoint==DecimalPointToUse) return;
    QString chain(str);
    chain.replace(QChar(OldDecimalPoint),QChar(DecimalPointToUse));
    strcpy(str,chain.toLatin1().constData());
}

void PrepareFormula(char * str)//replaces the decimal separator in a formula with the '.' for internal calculations
{
    QString chain(str);
    chain.toUpper();
    if (DecimalPointToUse==',')//if '.' is set --> nothing to do
    {//if ',' is used --> just replace ','-->'.'
        chain.replace(QChar(','),QChar('.'));
    }
    strcpy(str,chain.toLatin1().constData());
}

void RedisplayFormula(char * str)//replaces the decimal separator except in the set-ids: e.g. G0.S4.Y
{
    QRegExp rex1("G\\d,S\\d,");
    QRegExp rex2("S\\d,");
    int pos,pos2;
    QString chain(str);
    chain.toUpper();
    if (OldDecimalPoint==',' && DecimalPointToUse=='.')//the simple case
    {
        chain.replace(QChar(OldDecimalPoint),QChar(DecimalPointToUse));
    }
    else if (OldDecimalPoint=='.' && DecimalPointToUse==',')//the complicated case
    {
        chain.replace(QChar(OldDecimalPoint),QChar(DecimalPointToUse));//this replaces the decimal separators as well as the set-id-separators: G0.S4.Y-->G0,S4,Y
        while ((pos=rex1.indexIn(chain))>=0)
        {
            pos2=chain.indexOf(",",pos);
            chain[pos2]='.';
            pos2=chain.indexOf(",",pos);
            chain[pos2]='.';
        }
        while ((pos=rex2.indexIn(chain))>=0)
        {
            pos2=chain.indexOf(",",pos);
            chain[pos2]='.';
        }
    }
    strcpy(str,chain.toLatin1().constData());
}

int GetIntValueFromString(char * str)
{
    ReplaceDecimalSeparator(str);
    return atoi(str);
}

double GetDoubleValueFromString(char * str)
{
    ReplaceDecimalSeparator(str);
    return atof(str);
}

struct komplex add_komplex(struct komplex a,struct komplex b)
{
    struct komplex c={a.real+b.real,a.imag+b.imag};
    return c;
}

struct komplex sub_komplex(struct komplex a,struct komplex b)
{
    struct komplex c={a.real-b.real,a.imag-b.imag};
    return c;
}

struct komplex mult_komplex(struct komplex a,struct komplex b)
{
    //(ar+i*ai)*(br+i*bi)=(ar*br-ai*bi)+i*(ai*br+bi*ar)
    struct komplex c={a.real*b.real-a.imag*b.imag,a.imag*b.real+b.imag*a.real};
    return c;
}

struct komplex div_komplex(struct komplex a,struct komplex b)
{
    //(ar+i*ai)/(br+i*bi)=(ar+i*ai)/(br+i*bi)*(br-i*bi)/(br-i*bi)=((ar*br+ai*bi)+i*(ai*br-bi*ar))/(br*br+bi*bi)
    double betr=b.real*b.real+b.imag*b.imag;
    struct komplex c={(a.real*b.real+a.imag*b.imag)/betr,(a.imag*b.real-b.imag*a.real)/betr};
    return c;
}

struct komplex pow_komplex(struct komplex a,double n)
{
    static double pi=2.0*asin(1.0);
    double absval=pow(hypot(a.real,a.imag),n);
    double phase=atan2(a.imag,a.real)*n;
    /*if (a.real==0.0)
{
if (a.imag>=0.0) phase=0.5*pi;
else phase=1.5*pi;
}*/
    struct komplex c={absval*cos(phase),absval*sin(phase)};
    return c;
}

void SetLineEditToMemory(char * t1,char * t2,char * &c1,char * &c2,bool &displayStd,QLineEdit * lenText)
{
    static QPalette pal;
    c1=t1;//t1 and c1 are the grace-version of a string that is used internally!
    c2=t2;//t2 and c2 are the originals -- internally the original is always stored in UTF8 (this only changes before saving and after loading in which case the selected codec for external usage is used)
    displayStd=true;
    pal=lenText->palette();
    if (stdTextColor==NULL)
        stdTextColor=new QColor(pal.text().color());
    pal.setColor(QPalette::Text,*stdTextColor);
    if (t2!=NULL)//we display t2(the original) as a standard
    {
        lenText->setText(QString::fromUtf8(t2));//t2 = original = UTF8-encoding
        //if (useQtFonts==false)
        //lenText->setText(FileCodec->toUnicode(t2));//t2 = original
        //else
        //lenText->setText(FileCodec->toUnicode(t2));//t2 = original
    }
    else
    {
        lenText->setText("");
    }
    lenText->setPalette(pal);
}

void ClickedOnLabel(char * c1,char * c2,bool &displayStd,QLineEdit * lenText)
{
    static QPalette pal;
    if (c1==NULL || c2==NULL) return;
    displayStd=!displayStd;
    pal=lenText->palette();
    if (displayStd)//Std is the original user input (probably in LaTeX-format - but always in UTF8-encoding)
    {
        lenText->setText(QString::fromUtf8(c2));
        //if (useQtFonts==false)
        //lenText->setText(QString(c2));
        //else
        //lenText->setText(FileCodec->toUnicode(c2));
        pal.setColor(QPalette::Text,*stdTextColor);
    }
    else
    {
        lenText->setText(generateDisplayStringFromGraceString(c1));
        //if (useQtFonts==false)
        //lenText->setText(QString(c1));//converted input (Grace-format)
        //else
        //lenText->setText(FileCodec->toUnicode(c1));
        pal.setColor(QPalette::Text,Qt::red);
    }
    lenText->setPalette(pal);
}

void DynSetMemoryToLineEdit(char * &t1,char * &t2,char *&c1,char * &c2,bool & displayStd,bool acceptLaTex,QLineEdit * lenText)
{
    static QPalette pal;
    static QString text;
    text=lenText->text();
    t2=copy_string(t2,lenText->text().toUtf8().constData());//t2 is always the original - always internally in UTF8
    pal=lenText->palette();
    displayStd=true;
    char * te=new char[MAX_STRING_LENGTH];//longer is not allowed! Do not write essays in Grace ;-) !
    if (acceptLaTex==true && activateLaTeXsupport==true)
    {
        complete_LaTeX_to_Grace_Translator(text);//make the LaTeX-to-Grace-Translation
    }
    generate_string_Qt_aware(te,text);
    t1=copy_string(t1,te);//t1 is the Grace-version of the text
    c1=t1;
    c2=t2;
    pal.setColor(QPalette::Text,*stdTextColor);
    delete[] te;
}

SetCombo::SetCombo(QWidget * parent):QComboBox(parent)
{
    show_none=true;
    gno=-1;
    nr_of_entries=0;
    snos=new int[2];
}

void SetCombo::update_entries(int gr,bool preserve_selection)
{
    if (is_valid_gno(gr)==FALSE) return;
    int old=value();
    gno=gr;
    delete[] snos;
    snos=new int[3+g[gno].maxplot];
    clear();
    if (show_none==true)
    {
        snos[0]=-1;
        addItem(tr("None"));
        nr_of_entries=1;
    }
    else
        nr_of_entries=0;
    for (int i=0;i<g[gno].maxplot;i++)
    {
        if (is_set_active(gno,i)==true)
        {
            sprintf(dummy,"S%d",i);
            addItem(QString(dummy));
            snos[nr_of_entries++]=i;
        }
    }
    if (old!=-10 && preserve_selection==true)//reset old selection
    {
        for (int i=0;i<nr_of_entries;i++)
        {
            if (snos[i]==old)
            {
                setCurrentIndex(i);
                break;
            }
        }
    }
}

int SetCombo::value(void)
{
    if (gno==-1) return -10;
    else
        return snos[currentIndex()];
}

void SetCombo::setValue(int set)
{
    for (int i=0;i<nr_of_entries;i++)
    {
        if (snos[i]==set)
        {
            setCurrentIndex(i);
            break;
        }
    }
}

SetSelectorCombo::SetSelectorCombo(QString text,QWidget * parent):QWidget(parent)
{
    cmb=new SetCombo(this);
    lblCombo=new QLabel(text,this);
    layout=new QHBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->setSpacing(STD_SPACING);
    layout->addWidget(lblCombo);
    layout->addWidget(cmb);
    setLayout(layout);
}

void SetSelectorCombo::update_entries(int gno,bool preserve_selection)
{
    cmb->update_entries(gno,preserve_selection);
}

int SetSelectorCombo::value(void)
{
    return cmb->value();
}

void SetSelectorCombo::setValue(int set)
{
    cmb->setValue(set);
}

Panel::Panel(QPixmap * pix,QWidget * parent):QLabel(parent)
{
    p=pix;
    inpaint=false;
    setMouseTracking(TRUE);
}

void Panel::paintEvent(QPaintEvent *event)
{
    if (inpaint==false)
        inpaint=true;
    else
        return;
    QPainter paint(this);
    paint.drawPixmap(0,0,*p);
    paint.end();
    QLabel::paintEvent(event);
    inpaint=false;
}

void Panel::mouseMoveEvent( QMouseEvent * event )
{
    emit(mouseMoving());
}

void Panel::mouseReleaseEvent(QMouseEvent *event)
{
    emit(mouseClicked());
}

panelWindow::panelWindow(int rows,int cols,int last_col,QPixmap ** pix,QWidget * parent):QDialog(parent)
{
    frm=new QFrame(this);
    layout0=new QHBoxLayout(this);
    layout0->setMargin(0);
    layout0->setSpacing(0);
    frm->setFrameStyle(QFrame::Raised | QFrame::WinPanel);
    signalMapper1=new QSignalMapper;
    signalMapper2=new QSignalMapper;
    pixmaps=pix;
    row_count=rows;
    last_col_count=last_col;
    col_count=cols;
    number_of_elements=row_count*(col_count-1)+last_col_count;
    elementWidth=pix[0]->width();
    elementHeight=pix[0]->height();
    marked=highlighted=-1;//-1 means nothing ist marked or highlighted
    layout=new QGridLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    int r=0,c=0;
    lblPanel=new class Panel*[number_of_elements];
    //cout << "number_of_elements: " << number_of_elements << endl;
    for (int i=0;i<number_of_elements;i++)
    {
        lblPanel[i]=new class Panel(pixmaps[i],this);
        lblPanel[i]->setMinimumSize(elementWidth,elementHeight);

        connect(lblPanel[i], SIGNAL(mouseMoving()), signalMapper1, SLOT(map()));
        connect(lblPanel[i], SIGNAL(mouseClicked()), signalMapper2, SLOT(map()));
        signalMapper1->setMapping(lblPanel[i],i);
        signalMapper2->setMapping(lblPanel[i],i);

        layout->addWidget(lblPanel[i],r,c);
        r++;
        if (r>=row_count)
        {
            r=0;
            c++;
        }
    }
    connect(signalMapper1, SIGNAL(mapped(int)),this, SLOT(mouseMovedIn(int)));
    connect(signalMapper2, SIGNAL(mapped(int)),this, SLOT(mouseClickedIn(int)));
    clickTime=new QTime();
    clickTime->start();
    frm->setLayout(layout);
    setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
    setMouseTracking(TRUE);
    layout0->addWidget(frm);
    setLayout(layout0);
}

panelWindow::~panelWindow()
{
    for (int i=0;i<number_of_elements;i++)
        delete lblPanel[i];
    delete[] lblPanel;
    delete clickTime;
    delete signalMapper1;
    delete signalMapper2;
    delete layout0;
    delete layout;
    delete frm;
}

void panelWindow::setMarked(int i)
{
    if (marked==i) return;
    if (marked!=-1)
    {
        lblPanel[marked]->setFrameShape(QFrame::NoFrame);
        lblPanel[marked]->repaint();
    }
    marked=i;
    lblPanel[marked]->setFrameShape(QFrame::Panel);
    lblPanel[marked]->setFrameStyle(QFrame::Raised | QFrame::WinPanel);
    lblPanel[marked]->repaint();
}

void panelWindow::mouseMovedIn(int i)
{
    if (i==highlighted) return;
    //a new one is to be highlighted
    if (highlighted!=-1)
    {
        if (highlighted==marked)
        {
            lblPanel[highlighted]->setFrameShape(QFrame::Panel);
            lblPanel[highlighted]->setFrameStyle(QFrame::Raised | QFrame::WinPanel);
            lblPanel[highlighted]->repaint();
        }
        else
        {
            lblPanel[highlighted]->setFrameShape(QFrame::NoFrame);
            lblPanel[highlighted]->repaint();
        }
    }
    if (i!=-1)
    {
        lblPanel[i]->setFrameShape(QFrame::Panel);
        lblPanel[i]->setFrameStyle(QFrame::Sunken | QFrame::WinPanel);
        lblPanel[i]->repaint();
    }
    highlighted=i;
}

void panelWindow::leaveEvent( QEvent * event )
{
    if (highlighted!=-1)
    {
        if (highlighted==marked)
        {
            lblPanel[highlighted]->setFrameShape(QFrame::Panel);
            lblPanel[highlighted]->setFrameStyle(QFrame::Raised | QFrame::WinPanel);
            lblPanel[highlighted]->repaint();
        }
        else
        {
            lblPanel[highlighted]->setFrameShape(QFrame::NoFrame);
            lblPanel[highlighted]->repaint();
        }
    }
    highlighted=-1;
}

void panelWindow::mouseClickedIn(int i)
{
    hide();
    //cout << "clicked in = " << i << endl;
    emit(newSelection(i));
}

void panelWindow::mousePressEvent(QMouseEvent *event)
{
    pressed=true;
    //cout << "Preess: PanelWindow: x=" << event->x() << " y=" << event->y() << endl;
}

void panelWindow::mouseReleaseEvent(QMouseEvent *event)
{
    static int curTime;
    if (pressed==false) return;
    curTime=clickTime->restart();
    if (curTime<300)
    {
        pressed=false;
        return;
    }
    QPoint pos=QWidget::mapToParent(event->pos());
    int x=event->x();
    int y=event->y();
    x=movex;
    y=movey;
    int r=y/elementHeight;
    int c=x/elementWidth;
    int element=r+c*row_count;
    //cout << "Release: PanelWindow: x=" << x << " y=" << y << " xmax=" << col_count*elementWidth << " ymax=" << row_count*elementHeight << endl;
    //cout << pos.x() << " | " << pos.y() << endl;
    if (x<0 || y<0 || x>col_count*elementWidth || y>row_count*elementHeight)
    {
        element=-1;
        if (marked>=0)
            lblPanel[marked]->mouseReleaseEvent(event);
        //cout << "a: marked=" << marked << endl;
    }
    releaseMouse();
    if (element>=0 && element<number_of_elements)
    {
        lblPanel[element]->mouseReleaseEvent(event);
        //cout << "b: element=" << element << endl;
    }
    hide();
}

void panelWindow::mouseMoveEvent(QMouseEvent * event)
{
    int x=event->x();
    int y=event->y();
    int r=y/elementHeight;
    int c=x/elementWidth;
    int element=r+c*row_count;
    movex=event->x();
    movey=event->y();
    if (x<0 || y<0) element=-1;
    if (x>col_count*elementWidth || y>row_count*elementHeight) element=-1;
    if (element<0 || element>=number_of_elements)
    {
        leaveEvent(event);
    }
    else
    {
        if (element!=highlighted)
        {
            lblPanel[element]->mouseMoveEvent(event);
        }
    }
}

void panelWindow::keyPressEvent(QKeyEvent * event)
{
    if (event->key()==Qt::Key_Escape)
    {
    event->accept();
    hide();
    }
}

newCombo::newCombo(int rows,int cols,int last_col,QPixmap ** pix,QString ** titles,bool showTextOnly,QWidget * parent):QComboBox(parent)
{
    text_only=showTextOnly;
    if (text_only==false)
        setIconSize(QSize(pix[0]->width(),pix[0]->height()));
    panels=new panelWindow(rows,cols,last_col,pix,this);//actually initialize the panels
    panels->hide();
    connect(panels,SIGNAL(newSelection(int)),SLOT(wrapperSetCurrentIndex(int)));
    title_strings=new QString[panels->number_of_elements];
    for (int i=0;i<panels->number_of_elements;i++)
        title_strings[i]=*(titles[i]);
    setMinimumWidth(110);
    setCurrentIndex(0);
}

void newCombo::reinitializePanels(int rows,int cols,int last_col,QPixmap ** pix,QString ** titles,bool showTextOnly)
{
    text_only=showTextOnly;
    if (text_only==false)
        setIconSize(QSize(pix[0]->width(),pix[0]->height()));
    //first: delete old panels
    delete panels;//call destructor
    //second: create new panels
    panels=new panelWindow(rows,cols,last_col,pix,this);//reinitialize the panels
    panels->hide();
    connect(panels,SIGNAL(newSelection(int)),SLOT(wrapperSetCurrentIndex(int)));
    //third: create new titles
    delete[] title_strings;
    title_strings=new QString[panels->number_of_elements];
    for (int i=0;i<panels->number_of_elements;i++)
        title_strings[i]=*(titles[i]);
    setCurrentIndex(0);
}

void newCombo::wrapperSetCurrentIndex(int index)
{
    setCurrentIndex(index);
    emit(current_Index_Changed(index));
}

void newCombo::setCurrentIndex(int index)
{
    if (index<0 || index>=panels->number_of_elements)
    {
        cout << "Selection Index out of range!" << endl;
        return;
    }
    panels->setMarked(index);
    selection=index;
    clear();
    if (text_only==true)
        addItem(title_strings[index]);
    else
        addItem(QIcon(*panels->pixmaps[index]),"");
    QMouseEvent * a=new QMouseEvent(QEvent::MouseButtonRelease,QPoint(0,0),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
    QComboBox::mouseReleaseEvent(a);
}

int newCombo::currentIndex(void)
{
    return selection;
}

void newCombo::showPopup()
{
    int row_number=0;
    int col_number=0;
    for (int i=0;i<selection;i++)
    {
        row_number++;
        if (row_number>=panels->row_count)
        {
            row_number=0;
            col_number++;
        }
    }
    panels->setMarked(selection);
    panels->move(mapToGlobal(QPoint(-col_number*panels->elementWidth,-row_number*panels->elementHeight)));
    panels->show();
    //panels->pressed=false;
    panels->clickTime->restart();
    panels->pressed=true;
    panels->repaint();
    //panels->grabMouse();
    panels->setFocus();
    //hidePopup();
    qApp->processEvents();
}

fitLine::fitLine(QWidget * parent,int nr):QWidget(parent)
{
    char dummy[128];
    sprintf(dummy,"A%d:",nr);
    lblA=new QLabel(QString(dummy),this);
    sprintf(dummy,"< A%d <",nr);
    lblABounds=new QLabel(QString(dummy),this);
    chkBonds=new QCheckBox(tr("Bounds:"),this);
    connect(chkBonds,SIGNAL(stateChanged(int)),this,SLOT(constr_check(int)));
    ledValue=new QLineEdit(QString("1"),this);
    ledLowerBound=new QLineEdit(QString("1"),this);
    ledHighterBound=new QLineEdit(QString("1"),this);
    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(lblA);
    layout->addWidget(ledValue);
    layout->addWidget(chkBonds);
    layout->addWidget(ledLowerBound);
    layout->addWidget(lblABounds);
    layout->addWidget(ledHighterBound);
    setLayout(layout);
}

void fitLine::Redisplay(void)
{
    if (OldDecimalPoint==DecimalPointToUse) return;
    QString te;
    te=ledValue->text();
    te.replace(QChar(OldDecimalPoint),QChar(DecimalPointToUse));
    ledValue->setText(te);
    te=ledLowerBound->text();
    te.replace(QChar(OldDecimalPoint),QChar(DecimalPointToUse));
    ledLowerBound->setText(te);
    te=ledHighterBound->text();
    te.replace(QChar(OldDecimalPoint),QChar(DecimalPointToUse));
    ledHighterBound->setText(te);
}

void fitLine::constr_check(int t)
{
    if (t==0)
    {
        ledLowerBound->setEnabled(false);
        ledHighterBound->setEnabled(false);
        lblABounds->setEnabled(false);
    }
    else
    {
        ledLowerBound->setEnabled(true);
        ledHighterBound->setEnabled(true);
        lblABounds->setEnabled(true);
    }
}

void fitLine::getValues(double & value,bool & active,double & lowerBound,double & upperBound)
{
    value=atof(ledValue->text().toLatin1());
    active=chkBonds->isChecked()==TRUE?true:false;
    lowerBound=atof(ledLowerBound->text().toLatin1());
    upperBound=atof(ledHighterBound->text().toLatin1());
}

axisLine::axisLine(QWidget * parent,int nr):QWidget(parent)
{
    original=true;
    c1=c2=NULL;
    sprintf(dummy,"%d",nr);
    lblNr=new QLabel(QString(dummy),this);
    ledLocation=new QLineEdit(QString(""),this);
    ledLabel=new QLineEdit(QString(""),this);
    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(lblNr);
    layout->addWidget(ledLocation);
    layout->addWidget(ledLabel);
    setLayout(layout);
}

QString axisLine::LabelText(void)
{
    static int pos,pos2,ret;
    static QString str,str2;
    static QString ref("$$");
    static QString result,intermediate;
    result=str=ledLabel->text();
    if (activateLaTeXsupport==true)
    {//Latex-Commands-replacements
        complete_LaTeX_to_Grace_Translator(result);
        /*
pos=0;
result=str;
while ((pos=str.indexOf(ref,pos))>=0)
{
pos+=2;//beginning of commands after '$$'
pos2=str.indexOf(ref,pos);
str2=str.mid(pos,pos2-pos);
intermediate=ref+str2+ref;
pos=pos2+2;//beginning of text after closing '$$'
strcpy(dummy,str2.toLatin1());
ret=recursive_replacer(dummy);
result.replace(intermediate,QString(dummy));
}*/
        //cout << "end =#" << str.toLatin1().constData() << "#" << endl;
        //cout << "result =#" << result.toLatin1().constData() << "#" << endl;
    }
    return result;
}

QString axisLine::LocationText(void)
{
    return ledLocation->text();
}

void axisLine::SetTextToMemory(char * &t1,char * &t2)
{
    SetLineEditToMemory(t1,t2,c1,c2,original,ledLabel);
}

void axisLine::SetMemoryToText(char * &t1,char * &t2)
{
    DynSetMemoryToLineEdit(t1,t2,c1,c2,original,true,ledLabel);
}

void axisLine::LabelClicked(void)
{
    if (activateLaTeXsupport==false) return;
    ClickedOnLabel(c1,c2,original,ledLabel);
}

void axisLine::mouseReleaseEvent(QMouseEvent * e)
{
    if (activateLaTeXsupport==false) return;
    e->accept();
    QPoint pos=e->pos();
    if (lblNr->rect().contains(pos)==true) LabelClicked();
}

headLine::headLine(QWidget * parent,int columns):QWidget(parent)
{
    QString dummy;
    char dummy2[128];
    cols=columns;

    empty=new QWidget(this);
    signalMapper=new QSignalMapper(this);

    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(empty);

    for (int i=0;i<6;i++)
    {
        switch (i)
        {
        case 0:
            dummy=QString("X");
            break;
        case 1:
            dummy=QString("Y");
            break;
        default:
            sprintf(dummy2,"Y%d",i-1);
            dummy=QString(dummy2);
            break;
        }
        cmdColHead[i]=new QPushButton(dummy,this);
        connect(cmdColHead[i],SIGNAL(clicked()),signalMapper,SLOT(map()));
        signalMapper->setMapping(cmdColHead[i],i);
        if (i<cols)
        {
            cmdColHead[i]->setVisible(TRUE);
            layout->addWidget(cmdColHead[i]);
        }
        else
            cmdColHead[i]->setVisible(FALSE);
    }
    setLayout(layout);
    connect(signalMapper,SIGNAL(mapped(int)),SLOT(pressed(int)));
}

void headLine::resize(int columns)
{
    if (columns<cols)//the new one has lesser columns than the old one
    {
        for (int i=columns;i<cols;i++)
        {
            cmdColHead[i]->setVisible(FALSE);
            layout->removeWidget(cmdColHead[i]);
        }
    }
    else//more columns
    {
        for (int i=cols;i<columns;i++)
        {
            cmdColHead[i]->setVisible(TRUE);
            layout->addWidget(cmdColHead[i]);
        }
    }
    update();
    parentWidget()->updateGeometry();
    cols=columns;
}

void headLine::pressed(int i)
{
    emit(col_pressed(i));
}

spreadLine::spreadLine(QWidget * parent,int row_nr,int columns):QWidget(parent)
{
    char dummy[128];
    marked=false;
    row=row_nr;
    cols=columns;
    sprintf(dummy,"%d",row);
    cmdSelect=new QPushButton(QString(dummy),this);
    connect(cmdSelect,SIGNAL(clicked()),SLOT(clickSelect()));
    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(cmdSelect);
    for (int i=0;i<6;i++)
    {
        ledInput[i]=new QLineEdit(QString(""),this);
        if (i<cols)
        {
            ledInput[i]->setVisible(TRUE);
            layout->addWidget(ledInput[i]);
        }
        else
            ledInput[i]->setVisible(FALSE);

    }
    setLayout(layout);
}

spreadLine::~spreadLine()
{
    for (int i=0;i<6;i++)
    {
        if (i<cols)
            layout->removeWidget(ledInput[i]);
        ledInput[i]->disconnect();
        delete ledInput[i];
    }
}

void spreadLine::resize(int columns)
{
    if (cols>columns)//new are fewer columns
    {
        for (int i=columns;i<cols;i++)
        {
            layout->removeWidget(ledInput[i]);
            ledInput[i]->setVisible(FALSE);
        }
    }
    else
    {
        for (int i=cols;i<columns;i++)
        {
            layout->addWidget(ledInput[i]);
            ledInput[i]->setVisible(TRUE);
        }
    }
    cols=columns;
}

void spreadLine::clickSelect(void)
{
    marked=!marked;
    QPalette pal;
    QBrush t1,t2;
    for (int i=0;i<6;i++)
    {
        pal=ledInput[i]->palette();
        t1=pal.text();
        t2=pal.base();
        //exchange forground and background to show selection
        pal.setBrush(QPalette::Active,QPalette::Base,t1);
        pal.setBrush(QPalette::Active,QPalette::Text,t2);
        ledInput[i]->setPalette(pal);
    }
}

spreadSheet::spreadSheet(QWidget * parent,int type,int row_count,int col_count):QWidget(parent)
{
    sheet_type=type;
    rows=row_count;//rows with actual inputs
    cols=col_count;

    layout=new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    generate_layout();

    setLayout(layout);

    /*stdWidth=ledInput[0][0]->width();
stdHeight=ledInput[0][0]->height();*/

}

void spreadSheet::generate_layout(void)
{
    signalMapper=new QSignalMapper(this);
    switch (sheet_type)
    {
    case 0:
        headline=new headLine(this,cols);
        layout->addWidget(headline);
        spreadlines=new spreadLine*[rows];
        for (int i=0;i<rows;i++)
        {
            spreadlines[i]=new spreadLine(this,i,cols);
            layout->addWidget(spreadlines[i]);
        }
    /*if (rows>0)
    {
    headline->empty->setMaximumWidth(spreadlines[0]->cmdSelect->width());
    }*/
        break;
    case 1:
        fitlines=new fitLine*[MAXPARM+1];
        for (int i=0;i<=MAXPARM;i++)
        {
            fitlines[i]=new fitLine(this,i);
            if (i<rows)
            {
                fitlines[i]->setVisible(TRUE);
                layout->addWidget(fitlines[i]);
            }
            else
            {
                fitlines[i]->setVisible(FALSE);
            }
        }
        break;
    case 2:
        axislines=new axisLine*[rows];
        for (int i=0;i<rows;i++)
        {
            axislines[i]=new axisLine(this,i);
            layout->addWidget(axislines[i]);
        }
        break;
    }

    for (int i=0;i<rows;i++)
    {
        ;
        //layout->addWidget();
        //connect(VHeads[i],SIGNAL(clicked()),signalMapper1,SLOT(map()));
        //signalMapper1->setMapping(VHeads[i],i);
    }
    //connect(signalMapper,SIGNAL(mapped(int)),SLOT(clickHHeader(int)));
}

spreadSheet::~spreadSheet()
{

}

void spreadSheet::clickHHeader(int nr)
{
    /*EditColumn->selColumn->setCurrentIndex(nr);
EditColumn->show();
EditColumn->raise();*/
}

void spreadSheet::clickVHeader(int nr)
{
    /*marked[nr]=!marked[nr];
QPalette pal;
QBrush t1,t2;
    for (int i=0;i<cols;i++)
    {
    pal=ledInput[nr][i]->palette();
    t1=pal.text();
    t2=pal.base();
    //exchange forground and background to show selection
    pal.setBrush(QPalette::Active,QPalette::Base,t1);
    pal.setBrush(QPalette::Active,QPalette::Text,t2);
    ledInput[nr][i]->setPalette(pal);
    }*/
}

void spreadSheet::entryChanged(void)
{
    emit(changed());
}

void spreadSheet::resize(int row,int col)
{

    /*if (rows>0 && !sheet_type)
{
for (int i=0;i<6;i++)
headline->cmdColHead[i]->setMinimumSize(spreadlines[0]->ledInput[i]->width(),spreadlines[0]->ledInput[i]->height());
}*/

    switch (sheet_type)
    {
    case 0:

        setGeometry(0,0,width(),(row+1)*25);

        headline->resize(col);

        for (int i=0;i<rows;i++)
        {
            layout->removeWidget(spreadlines[i]);
            delete spreadlines[i];
        }
        delete[] spreadlines;

        spreadlines=new spreadLine*[row];
        for (int i=0;i<row;i++)
        {
            spreadlines[i]=new spreadLine(this,i,col);
            layout->addWidget(spreadlines[i]);
        }
        rows=row;
        cols=col;

        break;
    case 1:
        if (row>rows)
        {
            for (int i=rows;i<row;i++)
            {
                layout->addWidget(fitlines[i]);
                fitlines[i]->setVisible(TRUE);
            }
        }
        else
        {
            for (int i=row;i<rows;i++)
            {
                fitlines[i]->setVisible(FALSE);
                layout->removeWidget(fitlines[i]);
            }
        }
        rows=row;
        cols=col;
        break;
    case 2:

        break;
    }

    /*
setGeometry(0,0,(col+1)*stdWidth,(row+1)*stdHeight);
///delete current Elements
layout->removeWidget(empty);
delete empty;
delete[] marked;
for (int i=0;i<rows;i++)
{
layout->removeWidget(VHeads[i]);
VHeads[i]->disconnect();
delete VHeads[i];
    for (int j=0;j<cols;j++)
    {
    layout->removeWidget(ledInput[i][j]);
    ledInput[i][j]->disconnect();
    delete ledInput[i][j];
    }
delete[] ledInput[i];
}
delete[] ledInput;
delete[] VHeads;
for (int i=0;i<6;i++)
{
layout->removeWidget(HHeads[i]);
HHeads[i]->disconnect();
delete HHeads[i];
}
delete[] HHeads;

signalMapper1->disconnect();
signalMapper2->disconnect();

rows=row;
cols=col;

generate_layout();
*/
    update();
    parentWidget()->updateGeometry();
}

stdSlider::stdSlider(QWidget * parent,QString label,int min,int max,double factor,int type):QWidget(parent)
{
    QRect rec=stdFontMetrics->boundingRect(QString("100"));
    ret_time=new QTime();
    ret_time->start();
    textHeight=rec.height();
    slideType=type;
    ScalingFactor=factor;
    if (textHeight<16) textHeight=16;
    setMinimumSize(QSize(180,textHeight*3));
    //Indicator=new QLabel("0",this);
    Indicator=new QLineEdit("0",this);
    Indicator->setGeometry(0,0,38,textHeight);//0,0,32,20
    sldSlider=new QSlider(Qt::Horizontal,this);
    sldSlider->setRange(min,max);
    sldSlider->setGeometry(Indicator->x(),Indicator->y()+Indicator->height(),200,textHeight);//20
    connect(sldSlider,SIGNAL(sliderPressed()),SLOT(SliderPressed()));
    connect(sldSlider,SIGNAL(sliderReleased()),SLOT(SliderReleased()));
    connect(sldSlider,SIGNAL(sliderMoved(int)),SLOT(SliderMoved(int)));
    connect(sldSlider,SIGNAL(valueChanged(int)),SLOT(changed(int)));

    lblText=new QLabel(label,this);
    lblText->move(Indicator->x(),sldSlider->y()+sldSlider->height());//,150,20);
    lblText->setGeometry(Indicator->x(),sldSlider->y()+sldSlider->height(),sldSlider->width(),textHeight);//20
    setMinimumSize(60,3*textHeight);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::MinimumExpanding);
    /// BEGIN LineEdit decorations
    Indicator->setFrame(false);
    Indicator->setValidator(new QDoubleValidator(Indicator));
    QPalette pal1=lblText->palette();
    pal1.setBrush(QPalette::Base,Qt::NoBrush);
    Indicator->setPalette(pal1);
    /// END LineEdit decorations
    connect(Indicator,SIGNAL(returnPressed()),SLOT(Indicator_Enter_Pressed()));
    connect(Indicator,SIGNAL(editingFinished()),SLOT(Indicator_Finished()));
}

QSize stdSlider::sizeHint(void)
{
    return QSize(60,textHeight*3);//60
}

QSize stdSlider::minimumSizeHint(void)
{
    return QSize(60,textHeight*3);//60
}

void stdSlider::resizeEvent( QResizeEvent * e)
{
    static int width,height,pos;
    static QFont fo;
    static QString indText;
    e->accept();
    width=e->size().width();
    height=e->size().height();
    fo=font();
    if (height<3*textHeight)
    {
        fo.setPixelSize(fo.pixelSize()-2);
        textHeight-=2;
        setFont(fo);
    }
    else if (height<3*textHeight && fo.pixelSize()<14)
    {
        fo.setPixelSize(14);
        textHeight-=2;
        setFont(fo);
    }

    Indicator->setGeometry(Indicator->x(),0,Indicator->width(),textHeight);
    sldSlider->setGeometry(0,textHeight,width,height-2*textHeight);
    lblText->setGeometry(0,height-textHeight,lblText->width(),textHeight);

    indText=indicatorText();
    slideIndicator();
    Indicator->setText(indText);
}

void stdSlider::changed(int i)
{
    static QString indText;
    indText=indicatorText();
    slideIndicator();
    Indicator->setText(indText);
    emit(valueChanged(i));
}

void stdSlider::SliderMoved(int i)
{
    //cout << "moved " << i << " old=" << old_value << endl;
}

void stdSlider::SliderReleased(void)
{
    slider_status=3;
    changed(value());
    //cout << "released old=" << old_value << " new=" << value() << endl;
    old_value=value();
}

void stdSlider::SliderPressed(void)
{
    //cout << "Pressed old=" << old_value << endl;
    slider_status=1;
    sldSlider->setFocus();
    changed(value());
}

int stdSlider::value(void)
{
    return sldSlider->value();
}

void stdSlider::setValue(int i)
{
    old_value=i;
    sldSlider->setValue(i);
    changed(i);
}

void stdSlider::redisplay(void)
{
    setValue(value());
}

QString stdSlider::indicatorText(void)
{
    static char dummy[16];
    static int i;
    i=sldSlider->value();
    if (slideType==SLIDE_LINEAR)
    {
        if (ScalingFactor==1.0)
            sprintf(dummy,"%d",i);
        else
            sprintf(dummy,"%g",i*ScalingFactor);
    }
    else//logarithmic
    {
        sprintf(dummy,"%.2f",pow(10.0,i*ScalingFactor));
    }
    SetDecimalSeparatorToUserValue(dummy);
    return QString(dummy);
}

void stdSlider::slideIndicator(void)
{
    static int pos;
    pos=sldSlider->x()+(sldSlider->width()-Indicator->width())*(sldSlider->value()-sldSlider->minimum())/(sldSlider->maximum()-sldSlider->minimum());
    Indicator->move(pos,Indicator->y());
}

void stdSlider::Indicator_Enter_Pressed(void)
{
    ret_time->restart();
}

void stdSlider::Indicator_Finished(void)
{
    int time_since_enter=ret_time->restart();
    double val=Indicator->text().toDouble();
//cout << "manual change: old_value=" << old_value << endl;
    if (time_since_enter<100)//less then 100ms since last Return pressed
    {
        //cout << "Return Pressed (" << val << ")" << endl;
        if (slideType==SLIDE_LINEAR)
        {
            setValue((int)val);
        }
        else
        {
            setValue((int)(log10(val)/ScalingFactor));
        }
    }
    else
    {
        //cout << "Indicator Finished without Return (" << val << ")" << endl;
        Indicator->setText(indicatorText());
    }
//cout << "manual change: cur_val=" << value() << endl;
}

FontSelector::FontSelector(QWidget * parent):QWidget(parent)
{
    font=qApp->font();
    cmdSelFont=NULL;
    lblText=NULL;
    //if (useQtFonts==false)
    lblText=new QLabel(tr("Font:"));
    //else
    //{
    cmdSelFont=new QPushButton(tr("Font:"),this);
    connect(cmdSelFont,SIGNAL(clicked()),SLOT(selectNewFont()));
    displayFont();
    //}
    cmbFontSelect=new QComboBox();
    for (int i=0;i<number_of_fonts();i++)
        cmbFontSelect->addItem(get_fontalias(i));
    connect(cmbFontSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(changed(int)));
    cmbFontSelect->setCurrentIndex(DefaultFont);
    layout=new QHBoxLayout;
    layout->setMargin(2);
    //if (useQtFonts==false)
    layout->addWidget(lblText);
    //else
    layout->addWidget(cmdSelFont);
    layout->addWidget(cmbFontSelect);
    setLayout(layout);
    add_FontSelector(this);
    updateFonts(false);
    updateAppearance(useQtFonts);
}

void FontSelector::updateAppearance(bool QtIsNew)
{
    if (QtIsNew==true)//Qt-font-selector-button
    {
        lblText->setVisible(false);
        cmdSelFont->setVisible(true);
    }
    else//we switch back to Grace-standard
    {
        cmdSelFont->setVisible(false);
        lblText->setVisible(true);
    }
}

void FontSelector::setLabelText(QString s)
{
    if (useQtFonts==false)
        lblText->setText(s);
    else
        cmdSelFont->setText(s);
}

void FontSelector::updateFonts(bool preserve)
{
    disconnect(cmbFontSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(changed(int)));
    int old_index=cmbFontSelect->currentIndex();
    QString description;
    cmbFontSelect->clear();
    if (useQtFonts==false)
    {
        for (int i=0;i<number_of_fonts();i++)
            cmbFontSelect->addItem(get_fontalias(i));
    }
    else//useQtFonts=true
    {
        for (int i=0;i<stdFontList.length();i++)
        {
            description=stdFontList.at(i).family();
            if (stdFontList.at(i).italic()==true)
                description=description + tr("-italic");
            if (stdFontList.at(i).bold()==true)
                description=description + tr("-bold");
            cmbFontSelect->addItem(description);
        }
    }
    if (preserve==true)
        cmbFontSelect->setCurrentIndex(old_index);
    else
        cmbFontSelect->setCurrentIndex(DefaultFont);

    font=getFontFromDatabase(cmbFontSelect->currentIndex());

    connect(cmbFontSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(changed(int)));
}

void FontSelector::selectNewFont(void)
{
    int index;
    bool ok;
    font=QFontDialog::getFont(&ok,font,this);
    if (ok==true)//really a new font!
    {
        index=indexOfFontInDatabase(font);//does it exist?
        if (index<0)//no --> new font in database
        {
            index=addFontToDatabase(font);
            update_font_selectors(false);//update all Font selectors (just fonts, not apperance)
            changed(index);
        }
        cmbFontSelect->setCurrentIndex(index);
        displayFont();
    }
}

void FontSelector::displayFont(void)
{
    static QFont dFont;
    if (cmdSelFont!=NULL)
    {
        dFont=font;
        if (qApp->font().pixelSize()<2)
            dFont.setPixelSize(10);
        else
            dFont.setPixelSize(qApp->font().pixelSize());
        cmdSelFont->setFont(dFont);
        cmdSelFont->setText(cmdSelFont->text());
        //cmdSelFont->setText(font.family());
        //cout << "font=#" << font.toString().toLatin1().constData() << "#" << endl;
    }
}

void FontSelector::changed(int i)
{
    font=getFontFromDatabase(i);
    displayFont();
    emit(currentIndexChanged(i));
}

int FontSelector::currentIndex(void)
{
    /*if (useQtFonts==true)
    return indexOfFontInDatabase(font);
    else*/
    return cmbFontSelect->currentIndex();
}

void FontSelector::setCurrentIndex(int i)
{
    font=getFontFromDatabase(i);
    displayFont();
    cmbFontSelect->setCurrentIndex(i);
}

QFont FontSelector::getCurrentFont(void)
{
    return font;
}

void FontSelector::setCurrentFont(QFont & f)
{
    font = f;
    displayFont();
    int index=indexOfFontInDatabase(f);
    cmbFontSelect->setCurrentIndex(index);
}

PositionSelector::PositionSelector(QWidget * parent):QWidget(parent)
{
    lblText=new QLabel(tr("Position in:"));
    cmbPositionSelect=new QComboBox();
    cmbPositionSelect->addItem(tr("World coordinates"));
    cmbPositionSelect->addItem(tr("Viewport coordinates"));
    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(lblText);
    layout->addWidget(cmbPositionSelect);
    setLayout(layout);
}

int PositionSelector::currentIndex(void)
{
    return cmbPositionSelect->currentIndex();
}

int PositionSelector::currentValue(void)
{
    return cmbPositionSelect->currentIndex()==0?COORD_WORLD:COORD_VIEW;
}

void PositionSelector::setCurrentIndex(int i)
{
    cmbPositionSelect->setCurrentIndex(i);
}

JustificationSelector::JustificationSelector(QWidget * parent):QWidget(parent)
{
    just_names=new QString*[12];

    just_names[0]=new QString("lm");
    values[0]=JUST_LEFT | JUST_BLINE;
    just_names[1]=new QString("cm");
    values[1]=JUST_CENTER | JUST_BLINE;
    just_names[2]=new QString("rm");
    values[2]=JUST_RIGHT | JUST_BLINE;
    just_names[3]=new QString("lb");
    values[3]=JUST_LEFT | JUST_BOTTOM;
    just_names[4]=new QString("cb");
    values[4]=JUST_CENTER | JUST_BOTTOM;
    just_names[5]=new QString("rb");
    values[5]=JUST_RIGHT | JUST_BOTTOM;
    just_names[6]=new QString("lm");
    values[6]=JUST_LEFT | JUST_MIDDLE;
    just_names[7]=new QString("cm");
    values[7]=JUST_CENTER | JUST_MIDDLE;
    just_names[8]=new QString("rm");
    values[8]=JUST_RIGHT | JUST_MIDDLE;
    just_names[9]=new QString("lt");
    values[9]=JUST_LEFT | JUST_TOP;
    just_names[10]=new QString("ct");
    values[10]=JUST_CENTER | JUST_TOP;
    just_names[11]=new QString("rt");
    values[11]=JUST_RIGHT | JUST_TOP;

    cmbJustSelect=new newCombo(3,4,3,Qt_justifications,just_names,false,this);
    lblText=new QLabel(tr("Justification:"));

    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(lblText);
    layout->addWidget(cmbJustSelect);
    setLayout(layout);
}

int JustificationSelector::currentIndex(void)
{
    return values[cmbJustSelect->currentIndex()];
}

void JustificationSelector::setCurrentIndex(int i)
{
    int index=0;
    for (int j=0;j<12;j++)
    {
        if (values[j]==i)
        {
            index=j;
            break;
        }
    }
    cmbJustSelect->setCurrentIndex(index);
}

stdButtonGroup::stdButtonGroup(QWidget * parent,bool appl,bool acc,bool help):QWidget(parent)
{
    showAccept=acc;
    showApply=appl;
    showHelp=help;

    cmdAccept=new QPushButton(tr("Accept"));
    cmdApply=new QPushButton(tr("Apply"));
    cmdClose=new QPushButton(tr("Close"));
    cmdHelp=new QPushButton(tr("Help"));

    layout=new QHBoxLayout;
    layout->setMargin(2);
    if (showApply==true)
        layout->addWidget(cmdApply);
    else
        cmdApply->hide();
    if (showAccept==true)
        layout->addWidget(cmdAccept);
    else
        cmdAccept->hide();

    layout->addWidget(cmdClose);

    if (showHelp==true)
        layout->addWidget(cmdHelp);
    else
        cmdHelp->hide();
    setLayout(layout);
}

ColorSelector::ColorSelector(QWidget * parent):QWidget(parent)
{
    lblText=new QLabel(tr("Color:"));
    prevent_from_update=false;
    /*cmbColorSelect=new QComboBox();
for (int i=0;i<16;i++)
cmbColorSelect->addItem(*ColorIcons[i],"");
cmbColorSelect->setIconSize(QSize(82,16));
*/
    int * real_colors=new int[4];
    int aux_cols;
    int map_entries=get_main_color_indices(&real_colors,&aux_cols);
    int rows=int(sqrt(map_entries*1.0));
    int cols=rows;
    int last_col=rows;
    if (rows*cols<map_entries)
    {
        last_col=map_entries-rows*cols;
        cols+=1;
        while (last_col>rows)
        {
            cols+=1;
            last_col-=rows;
        }
    }
    cmbColorSelect=new newCombo(rows,cols,last_col,ColorPixmaps,ColorNames,true,this);
    connect(cmbColorSelect,SIGNAL(current_Index_Changed(int)),SLOT(panelIndexChanged(int)));
    delete[] real_colors;
    layout=new QHBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(lblText);
    layout->addWidget(cmbColorSelect);
    setLayout(layout);

    add_ColorSelector(this);
}

void ColorSelector::update_color_panels(void)
{
    updateColorIcons(number_of_colors(),ColorPixmaps,ColorNames);
}

void ColorSelector::panelIndexChanged(int i)
{
    emit(currentIndexChanged(i));
}

int ColorSelector::currentIndex(void)
{
    return cmbColorSelect->currentIndex();
}

void ColorSelector::setCurrentIndex(int i)
{
    cmbColorSelect->setCurrentIndex(i);
}

void ColorSelector::updateColorIcons(int nr_of_cols,QPixmap ** ColorPixmaps,QString ** ColorNames)
{
    int rows=int(sqrt(nr_of_cols*1.0));
    int cols=rows;
    int last_col=rows;
    if (rows*cols<nr_of_cols)
    {
        last_col=nr_of_cols-rows*cols;
        cols+=1;
        while (last_col>rows)
        {
            cols+=1;
            last_col-=rows;
        }
    }
    cmbColorSelect->reinitializePanels(rows,cols,last_col,ColorPixmaps,ColorNames,cmbColorSelect->text_only);
}

StdSelector::StdSelector(QWidget * parent)
{
    int number=2;
    QString * entr=new QString[2];
    entr[0]=QString("");
    entr[1]=QString("");
    QString label("");
    StdSelector(parent,label,number,entr);
}

StdSelector::StdSelector(QWidget * parent,QString label,int number,QString * entr):QWidget(parent)
{
    lblText=new QLabel(label);
    cmbSelect=new QComboBox();
if (number<1 || entr==NULL)
{
    number_of_entries=2;
    entries=new QString[number_of_entries];
    entryValues=new int[number_of_entries];
    for (int i=0;i<number_of_entries;i++)
    {
        entries[i]=QString("");
        entryValues[i]=i;
        cmbSelect->addItem(entries[i]);
    }
}
else
{
    number_of_entries=number;
    entries=new QString[number];
    entryValues=new int[number];
    for (int i=0;i<number;i++)
    {
        entries[i]=entr[i];
        entryValues[i]=i;
        cmbSelect->addItem(entr[i]);
    }
}
    connect(cmbSelect,SIGNAL(currentIndexChanged(int)),SLOT(changed(int)));
    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(lblText);
    layout->addWidget(cmbSelect);
    setLayout(layout);
}

StdSelector::~StdSelector()
{
    delete[] entries;
    delete[] entryValues;
}

void StdSelector::changed(int i)
{
    emit(currentIndexChanged(i));
}

void StdSelector::setValues(int * values)
{
    for (int i=0;i<number_of_entries;i++)
    {
        entryValues[i]=values[i];
    }
}

void StdSelector::setNewEntries(int number,QString * entr)
{
    int * values=new int[number];
    for (int i=0;i<number;i++)
        values[i]=i;

    setNewEntries(number,entr,values);
    delete[] values;
}

void StdSelector::setNewEntries(int number,QString * entr,int * values)
{
    cmbSelect->clear();
    delete[] entries;
    delete[] entryValues;
    number_of_entries=number;
    entries=new QString[number];
    entryValues=new int[number];
    for (int i=0;i<number;i++)
    {
        entries[i]=entr[i];
        entryValues[i]=values[i];
        cmbSelect->addItem(entr[i]);
    }
}

int StdSelector::currentIndex(void)
{
    return cmbSelect->currentIndex();
}

int StdSelector::currentValue(void)
{
    return entryValues[cmbSelect->currentIndex()];
}

void StdSelector::setCurrentIndex(int i)
{
    if (i>=0 && i<cmbSelect->count())
        cmbSelect->setCurrentIndex(i);
    else
        cmbSelect->setCurrentIndex(0);
}

void StdSelector::setCurrentValue(int i)
{
    int index=0;
    for (int j=0;j<number_of_entries;j++)
    {
        if (i==entryValues[j])
        {
            index=j;
            break;
        }
    }
    cmbSelect->setCurrentIndex(index);
}

LineStyleSelector::LineStyleSelector(QWidget * parent):QWidget(parent)
{
    add_Line_Style_Selector(this);
    lblText=new QLabel(tr("Line style:"));
    /*cmbStyleSelect=new QComboBox();
for (int i=0;i<MAXLINESTYLES;i++)
cmbStyleSelect->addItem(*LineIcons[i],"");
cmbStyleSelect->setIconSize(QSize(82,22));*/

    LineNames=new QString*[nr_of_current_linestyles];
    char dummy[48];
    for (int i=0;i<nr_of_current_linestyles;i++)
    {
        sprintf(dummy,"%d",i);
        LineNames[i]=new QString(dummy);
    }
    cmbStyleSelect=new newCombo(nr_of_current_linestyles,1,nr_of_current_linestyles,LinePixmaps,LineNames,false,this);
    connect(cmbStyleSelect,SIGNAL(current_Index_Changed(int)),SLOT(changed(int)));
    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(lblText);
    layout->addWidget(cmbStyleSelect);
    setLayout(layout);
}

void LineStyleSelector::changed(int i)
{
    emit(currentIndexChanged(i));
}

int LineStyleSelector::currentIndex(void)
{
    return cmbStyleSelect->currentIndex();
}

void LineStyleSelector::setCurrentIndex(int i)
{
    cmbStyleSelect->setCurrentIndex(i);
}

FillPatternSelector::FillPatternSelector(QWidget * parent):QWidget(parent)
{
    lblText=new QLabel(tr("Fill pattern:"),this);
    /*
cmbFillPattern=new QComboBox(this);
cmbFillPattern->addItem(tr("None"));
for (int i=1;i<MAXPATTERNS;i++)
cmbFillPattern->addItem(QIcon(patterns[i]->copy(patterns[i]->rect())),"");
*/
    char dummy[48];
    PatternNames=new QString*[MAXPATTERNS];
    sprintf(dummy,"None");
    PatternNames[0]=new QString(dummy);
    for (int i=1;i<MAXPATTERNS;i++)
    {
        sprintf(dummy,"%d",i);
        PatternNames[i]=new QString(dummy);
    }
    cmbFillPattern=new newCombo(8,4,8,PatternPixmaps,PatternNames,false,this);
    connect(cmbFillPattern,SIGNAL(current_Index_Changed(int)),SLOT(changed(int)));
    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(lblText);
    layout->addWidget(cmbFillPattern);
    setLayout(layout);
}

void FillPatternSelector::changed(int i)
{
    emit(currentIndexChanged(i));
}

int FillPatternSelector::currentIndex(void)
{
    return cmbFillPattern->currentIndex();
}

void FillPatternSelector::setCurrentIndex(int i)
{
    cmbFillPattern->setCurrentIndex(i);
}

OrderSelector::OrderSelector(QWidget * parent):QWidget(parent)
{
    OrderNames=new QString*[4*2];
    OrderNames[0]=new QString("hv_lr");
    OrderNames[1]=new QString("hv_lr");
    OrderNames[2]=new QString("hv_rl");
    OrderNames[3]=new QString("hv_rl");
    OrderNames[4]=new QString("vh_lr");
    OrderNames[5]=new QString("vh_lr");
    OrderNames[6]=new QString("vh_rl");
    OrderNames[7]=new QString("vh_rl");
    lblText=new QLabel(tr("Order:"),this);
    cmbOrderPattern=new newCombo(4,2,4,Qt_matrixOrder,OrderNames,false,this);
    connect(cmbOrderPattern,SIGNAL(current_Index_Changed(int)),SLOT(changed(int)));
    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(lblText);
    layout->addWidget(cmbOrderPattern);
    setLayout(layout);
}

void OrderSelector::changed(int i)
{
    emit(currentIndexChanged(i));
}

int OrderSelector::currentIndex(void)
{
    return cmbOrderPattern->currentIndex();
}

void OrderSelector::setCurrentIndex(int i)
{
    cmbOrderPattern->setCurrentIndex(i);
}


LineWidthSelector::LineWidthSelector(QWidget * parent):QWidget(parent)
{
    lblText=new QLabel(tr("Line width:"));
    spnLineWidth=new QDoubleSpinBox(this);
    spnLineWidth->setRange(0.0,MAX_LINEWIDTH);
    spnLineWidth->setDecimals(1);
    spnLineWidth->setSingleStep(0.5);
    connect(spnLineWidth,SIGNAL(valueChanged(double)),SLOT(changed(double)));
    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(lblText);
    layout->addWidget(spnLineWidth);
    setLayout(layout);
}

void LineWidthSelector::changed(double i)
{
    emit(currentValueChanged(i));
}

double LineWidthSelector::value(void)
{
    return spnLineWidth->value();
}

void LineWidthSelector::setValue(double i)
{
    spnLineWidth->setValue(i);
}

stdIntSelector::stdIntSelector(QWidget * parent,QString label,int min,int max):QWidget(parent)
{
    lblText=new QLabel(label,this);
    spnInt=new QSpinBox(this);
    spnInt->setRange(min,max);
    connect(spnInt,SIGNAL(valueChanged(int)),SLOT(changed(int)));
    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(lblText);
    layout->addWidget(spnInt);
    setLayout(layout);
}

void stdIntSelector::changed(int i)
{
    emit(currentValueChanged(i));
}

int stdIntSelector::value(void)
{
    return spnInt->value();
}

void stdIntSelector::setValue(int i)
{
    spnInt->setValue(i);
}

stdLineEdit::stdLineEdit(QWidget * parent,QString label,bool accLaTex):QWidget(parent)
{
    c1=c2=NULL;
    displayStd=true;
    acceptLaTex=accLaTex;
    lblText=new QLabel(label,this);
    lenText=new QLineEdit("0",this);
    connect(lenText,SIGNAL(returnPressed()),this,SLOT(ContentChanged()));
    layout=new QHBoxLayout();
    layout->setMargin(2);
    layout->addWidget(lblText);
    layout->addWidget(lenText);
    setLayout(layout);
    QPalette pal=lenText->palette();
    if (stdTextColor==NULL)
        stdTextColor=new QColor(pal.text().color());
    if (acceptLaTex==true)
        add_LaTeX_Line(this);
}

void stdLineEdit::ContentChanged(void)
{
    emit(changed());
}

QString stdLineEdit::text(void)
{
    if (useQtFonts)
        return FileCodec->fromUnicode(lenText->text());
    else
        return lenText->text();
    /*///Deactivated LaTeX-to-Grace-conversion
static int pos,pos2,ret;
static QString str,str2;
static QString ref("$$");
static QString result,intermediate;
result=str=lenText->text();
    if (acceptLaTex==true && activateLaTeXsupport==true)
    {//Latex-Commands-replacements
        cout << "I got used!" << endl;
    complete_LaTeX_to_Grace_Translator(result);
    pos=0;
        while ((pos=str.indexOf(ref,pos))>=0)
        {
        pos+=2;//beginning of commands after '$$'
        pos2=str.indexOf(ref,pos);
        str2=str.mid(pos,pos2-pos);
        intermediate=ref+str2+ref;
        pos=pos2+2;//beginning of text after closing '$$'
        strcpy(dummy,str2.toLatin1());
        ret=recursive_replacer(dummy);
        result.replace(intermediate,QString(dummy));
        }
    //cout << "end =#" << str.toLatin1().constData() << "#" << endl;
    //cout << "result =#" << result.toLatin1().constData() << "#" << endl;
    }
return result;
*/
}

void stdLineEdit::setText(QString text)
{
    displayStd=true;
    c1=c2=NULL;
    if (useQtFonts)
    {
        if (FileCodec==NULL) FileCodec=QTextCodec::codecForLocale();
        lenText->setText(FileCodec->toUnicode(text.toLatin1().constData()));
    }
    else
    {
        //lenText->setText(text);
        if (FileCodec==NULL) FileCodec=QTextCodec::codecForLocale();
        lenText->setText(FileCodec->toUnicode(text.toLatin1().constData()));
    }

}

void stdLineEdit::mouseReleaseEvent(QMouseEvent * e)
{
    if (acceptLaTex==false) return;
    e->accept();
    QPoint pos=e->pos();
    if (lblText->rect().contains(pos)==true) clickedOnLabel();
}

void stdLineEdit::clickedOnLabel(void)
{
    if (acceptLaTex==false || activateLaTeXsupport==false) return;
    ClickedOnLabel(c1,c2,displayStd,lenText);
    /*
if (c1==NULL || c2==NULL) return;
displayStd=!displayStd;
QPalette pal=lenText->palette();
if (displayStd)//Std is the original user input (probably in LaTeX-format)
{
lenText->setText(QString(c2));
pal.setColor(QPalette::Text,*stdTextColor);
}
else
{
lenText->setText(QString(c1));//converted input (Grace-format)
pal.setColor(QPalette::Text,Qt::red);
}
lenText->setPalette(pal);*/
}

void stdLineEdit::SetTextToMemory(char * t1,char * t2)
{
    SetLineEditToMemory(t1,t2,c1,c2,displayStd,lenText);
    /*c1=t1;
c2=t2;
displayStd=true;
QPalette pal=lenText->palette();
pal.setColor(QPalette::Text,*stdTextColor);
if (t2!=NULL)
lenText->setText(QString(t2));
else
lenText->setText("");
lenText->setPalette(pal);*/
}

void appendTextToLegendString(int gno,int sno,QString text)
{
    if (is_set_active(gno,sno)==false) return;
    QString old_text;
    plotarr * p=g[gno].p+sno;
    old_text=QString::fromUtf8(p->orig_lstr);//original is in UTF8
    old_text+=text;//append text
    strcpy(p->orig_lstr,old_text.toUtf8().constData());
    if (activateLaTeXsupport==true)
        complete_LaTeX_to_Grace_Translator(old_text);
    generate_string_Qt_aware(p->lstr,old_text);//t1 is the Grace-version of the text
}

void replace_directory_separators(char * filename)
{
    char sep=QDir::separator().toLatin1();
    int len=strlen(filename);
    for (int i=0;i<len;i++)
    {
        if (filename[i]=='/' || filename[i]=='\\')
            filename[i]=sep;
    }
}

void stdLineEdit::SetMemoryToText(char * t1,char * t2)
{
    static QPalette pal;
    QString text=lenText->text();
    displayStd=true;
    strcpy(t2,text.toUtf8().constData());//t2 is always the original - internally always in UTF8
    if (acceptLaTex==true && activateLaTeXsupport==true)
        complete_LaTeX_to_Grace_Translator(text);
    generate_string_Qt_aware(t1,text);//t1 is the Grace-version of the text
    c1=t1;
    c2=t2;
    pal=lenText->palette();
    if (stdTextColor==NULL)
        stdTextColor=new QColor(pal.text().color());
    pal.setColor(QPalette::Text,*stdTextColor);
    lenText->setPalette(pal);
}

void stdLineEdit::DynSetMemoryToText(char * &t1,char * &t2)
{
    DynSetMemoryToLineEdit(t1,t2,c1,c2,displayStd,acceptLaTex,lenText);
    /*
QString text(lenText->text());
displayStd=true;
char * te=new char[text.length()+8];
strcpy(te,text.toLatin1().constData());
t2=copy_string(t2,te);//t2 is always the original
    if (acceptLaTex==true && activateLaTeXsupport==true)
    {
    complete_LaTeX_to_Grace_Translator(text);
    delete[] te;
    te=new char[text.length()+8];
    strcpy(te,text.toLatin1().constData());
    }
t1=copy_string(t1,te);//t1 is the Grace-version of the text
c1=t1;
c2=t2;
delete[] te;
*/
}

void stdLineEdit::ReplaceNumberContents(void)
{
    if (OldDecimalPoint==DecimalPointToUse) return;//nothing to change!
    QString te=lenText->text();
    te.replace(QChar(OldDecimalPoint),QChar(DecimalPointToUse));
    lenText->setText(te);
}

void stdLineEdit::RedisplayContents(void)
{
    if (c1==NULL || c2==NULL)//NULL-addresses are critical --> close a text-editor in this case
    {
        emit(NoMoreValidAdr());
    }
    else
    {
        SetLineEditToMemory(c1,c2,c1,c2,displayStd,lenText);
    }
}

void stdLineEdit::setDoubleValue(const char * form,double val)
{
    static char buf[128];
    sprintf(buf, form, val);
    SetDecimalSeparatorToUserValue(buf);
    lenText->setText(QString(buf));
}

double stdLineEdit::getDoubleValue(void)
{
    //static int strlen;
    //static char * str;
    static double d;
    //strlen=4*lenText->text().length();
    //str=new char[strlen];
    //strcpy(str,lenText->text().toLatin1().constData());
    //d=GetDoubleValueFromString(str);
    //delete[] str;
    xv_evalexpr(lenText,&d);
    return d;
}

int stdLineEdit::getIntValue(void)
{
    static int strlen;
    static char * str;
    static int d;
    strlen=4*lenText->text().length();
    str=new char[strlen];
    strcpy(str,lenText->text().toLatin1().constData());
    d=GetIntValueFromString(str);
    delete[] str;
    return d;
}

double stdLineEdit::guessDoubleValue(void)
{
    static char buf[64];
    char f,l;
    int len;
    QString help;
    strncpy(buf,lenText->text().toLatin1().constData(),63);
    buf[63]='\0';
    len=strlen(buf);
    f=l='\0';
    for (int i=0;i<len;i++)
    {
        if (buf[i]=='.' || buf[i]==',')
        {
            if (f=='\0')
            {
                f=buf[i];
            }
            else
            {
                l=buf[i];
            }
        }
    }
    if ( !( (f==l && f=='\0') || (f=='.' && l=='\0') ) )//either no decimal separator found or only a '.' --> all right --> do not(!) go into this "if"
    {
        help=QString::fromLatin1(buf);
        if (l=='\0')//--> we only found a single ',' --> we guess that this should be the decimal separator and replace it by '.'
        {
            help.replace(QString(","),QString("."));
        }
        else if (l=='.' && f==',')//we found a string with (possibly) ',' as thousand-separator and '.' as decimal separator --> remove ','
        {
            help.remove(QString(","));
        }
        else if (l==',' && f=='.')//we found a string with (possibly) '.' as thousand-separator and ',' as decimal separator --> remove '.' and replace ',' by '.'
        {
            help.remove(QString("."));
            help.replace(QString(","),QString("."));
        }
        else if (f==l)// we found multiple '.' or ',' or a constallation like 1.345,678.9 which does not make sense! --> remove ALL '.' and ','
        {
            help.remove(QString("."));
            help.remove(QString(","));
        }
        strcpy(buf,help.toLatin1().constData());
    }
    return atof(buf);
}

dirList::dirList(QWidget * parent,int type):QTreeView(parent)
{
    listtype=type;

    model = new QDirModel();

    if (listtype==DIRLIST)
        model->setFilter(QDir::AllDirs);
    else
        model->setFilter(QDir::Files | QDir::NoDotAndDotDot);

    setModel(model);
    setRootIndex(model->index(QDir::currentPath()));
}

FileSelector::FileSelector(QWidget * parent):QWidget(parent)
{
    int number=4;
    QString entr[4];
    entr[0]=tr("Cwd");
    entr[1]=tr("Home");
#ifdef MAC_SYSTEM
    entr[2]=tr("/");
#endif
#ifdef LINUX_SYSTEM
    entr[2]=tr("/");
#endif
#ifdef WINDOWS_SYSTEM
    entr[2]=tr("C:\\");
    onHighestLevel=false;
#endif
    entr[3]=tr("QtGrace-dir");
    grpChDir=new QGroupBox(QString(""),this);
    selChdir=new StdSelector(grpChDir,tr("Chdir to:"),number,entr);
    cmdSetCwd=new QPushButton(tr("Set as cwd"),grpChDir);
    //cmdGetCwd=new QPushButton(tr("Get cwd"),grpChDir);
    chkShowHiddenFiles=new QCheckBox(tr("Show hidden files"),this);

    lblFilter=new QLabel(tr("Filter:"),this);
    lblDirs=new QLabel(tr("Directories:"),this);
    lblFiles=new QLabel(tr("Files:"),this);
    ledFilter=new QLineEdit("",this);
    DirList=new QListView(this);
    FileList=new QListView(this);
    modelDirs=new QDirModel();
    modelDirs->setFilter(QDir::AllDirs);
    modelFiles=new QDirModel();
    modelFiles->setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::Drives | QDir::System);
    DirList->setModel(modelDirs);
    FileList->setModel(modelFiles);

    currentDir=QDir::currentPath();//Just for initialization

    DirList->setRootIndex(modelDirs->index(currentDir));
    FileList->setRootIndex(modelFiles->index(currentDir));
    //cmdGoUp=new QPushButton(tr("GoUp"),this);
    cmdGoUp=new QPushButton(qApp->style()->standardPixmap(QStyle::SP_FileDialogToParent),QString(""),this);
    //cmdStdDialog=new QPushButton(tr("StdDialog"),this);
    cmdStdDialog=new QPushButton(qApp->style()->standardPixmap(QStyle::SP_DirOpenIcon),QString(""),this);

    connect(cmdGoUp,SIGNAL(clicked()),SLOT(doGoUp()));
    connect(cmdStdDialog,SIGNAL(clicked()),SLOT(doStdDialog()));

    //connect(selChdir->cmbSelect,SIGNAL(currentIndexChanged(int)),SLOT(currentDirChanged(int)));
    connect(selChdir->cmbSelect,SIGNAL(highlighted(int)),SLOT(currentDirChanged(int)));
    connect(FileList,SIGNAL(clicked(const QModelIndex &)),SLOT(FileSelected(const QModelIndex &)));
    connect(FileList,SIGNAL(doubleClicked(const QModelIndex &)),SLOT(FileDoubleClicked(const QModelIndex &)));
    connect(DirList,SIGNAL(clicked(const QModelIndex &)),SLOT(DirSelected(const QModelIndex &)));
    connect(DirList,SIGNAL(doubleClicked(const QModelIndex &)),SLOT(DirDoubleClicked(const QModelIndex &)));
    connect(ledFilter,SIGNAL(returnPressed()),SLOT(newFilterEntered()));
    connect(chkShowHiddenFiles,SIGNAL(stateChanged(int)),SLOT(toggleHiddenFile(int)));
    connect(cmdSetCwd,SIGNAL(clicked()),SLOT(doSetCWD()));
    //connect(cmdGetCwd,SIGNAL(clicked()),SLOT(doGetCWD()));

    layout0=new QHBoxLayout;
    layout0->setMargin(2);
    layout0->addWidget(selChdir);
    layout0->addWidget(cmdSetCwd);
    //layout0->addWidget(cmdGetCwd);
    grpChDir->setLayout(layout0);

    layout=new QGridLayout();
    layout->setMargin(2);
    layout->addWidget(lblFilter,0,0,1,4);
    layout->addWidget(ledFilter,1,0,1,4);
    layout->addWidget(lblDirs,2,0);
    layout->addWidget(cmdGoUp,2,1);
    layout->addWidget(lblFiles,2,2,1,2);
    layout->addWidget(DirList,3,0,1,2);
    layout->addWidget(FileList,3,2,1,2);
    layout->addWidget(chkShowHiddenFiles,4,0,1,2);
    layout->addWidget(cmdStdDialog,4,2,1,2);
    layout->addWidget(grpChDir,5,0,1,4);
    setLayout(layout);
    doubleclickdir=false;
    doubleclickfile=false;

    separator=QDir::separator();//QString("/");

    filterExtension=QString("*.dat");
    filter=currentDir+separator+filterExtension;
    ledFilter->setText(filter);
}

void FileSelector::FileSelected(const QModelIndex & index )
{
    if (doubleclickfile==true)
    {
        doubleclickfile=false;
        return;
    }
    selectedFile=index.data().toString();
    QString toEmit=QDir::cleanPath(currentDir+separator+selectedFile);
    toEmit=QDir::toNativeSeparators(toEmit);
    emit(newSelection(toEmit));
}

void FileSelector::FileDoubleClicked( const QModelIndex & index )
{
    doubleclickfile=true;
    selectedFile=index.data().toString();
    QString toEmit=QDir::cleanPath(currentDir+separator+selectedFile);
    toEmit=QDir::toNativeSeparators(toEmit);
    emit(newSelectionDoubleClick(toEmit));
}

void FileSelector::DirSelected(const QModelIndex & index )
{
    if (doubleclickdir==true)
    {
        doubleclickdir=false;
        return;
    }
    selectedDir=index.data().toString();
    QDir curDir(currentDir+separator+selectedDir);
    QString newFilter,newDir;
    GeneratePathWithExtension(curDir,newFilter,newDir);
    filter=newFilter;
    ledFilter->setText(filter);
}

void FileSelector::DirDoubleClicked( const QModelIndex & index )
{
    doubleclickdir=true;
    selectedDir=index.data().toString();
    if(selectedDir.compare(".")==0) return;//no directory change
    currentDir=currentDir+separator+selectedDir;//set a new directory
    QString newFilter,newDir;
    GeneratePathWithExtension(currentDir,newFilter,newDir);

    QDir d1(currentDir);
    d1.makeAbsolute();
    currentDir=d1.path();
    filter=newFilter;
    ledFilter->setText(filter);

    showFilesLikeFilter();

    /*TODO: IS THIS GOOD? See GeneratePathWithExtension(...)*/
    //newDir=newDir+separator;
    emit(newSelection(newDir));
    newFilterEntered();
}

void FileSelector::showFilesLikeFilter(void)
{
    QStringList filters;
    filters << filterExtension;
    modelDirs->setNameFilters(filters);
    modelFiles->setNameFilters(filters);
    DirList->setRootIndex(modelDirs->index(currentDir));
    FileList->setRootIndex(modelFiles->index(currentDir));
}

void FileSelector::newFilterEntered(void)
{
    ///test for "exists"???
    QString entered=ledFilter->text();
    if (entered.isEmpty())
    {
    ledFilter->setText(QDir::toNativeSeparators(QString(user_home_dir)+QDir::separator()+QString("*")));
    entered=ledFilter->text();
    }
    QDir d1(entered);
    QChar last_char=entered.at(entered.length()-1);
#ifdef WINDOWS_SYSTEM
    onHighestLevel=false;
#endif
    if ( !entered.contains(QString("*")) && !entered.contains(QString("?")) && last_char==separator.at(0))
    {
        filterExtension=QString("*");
        ledFilter->setText(QDir::toNativeSeparators(entered + filterExtension));
    }
    else
    {
        filterExtension=d1.dirName();
    }
    filter=entered;

    int pos=entered.lastIndexOf(separator);
    QString p=entered;
    p.chop(entered.length()-pos);

    QDir d2(p);
    d2.makeAbsolute();
    currentDir=d2.path();

    showFilesLikeFilter();
}

void FileSelector::setFilterFromExtern(QString & directory,QString & extension)
{
    currentDir=directory;
    filterExtension=extension;
    showFilesLikeFilter();
}

void FileSelector::setFileSelectionFromExtern(QString n_file)
{
QFileInfo fi(n_file);
QString ext=QString("*.")+fi.suffix();
QString n_dir=QDir::toNativeSeparators(fi.absolutePath());
setFilterFromExtern(n_dir,ext);

QModelIndex mi=modelFiles->index(n_file);
FileList->setCurrentIndex(mi);
/*
cout << "Files: rows=" << modelFiles->rowCount() << endl;
cout << "Files: cols=" << modelFiles->columnCount() << endl;
cout << "Dirs: rows=" << DirList->model()->rowCount() << endl;
*/
}

void FileSelector::doSetCWD(void)
{
    char * dummy=new char[currentDir.length()+1];
    strcpy(dummy,currentDir.toLatin1());
    set_workingdir(dummy);
    delete[] dummy;
}

/*void FileSelector::doGetCWD(void)
{
    currentDir=QString(get_workingdir());
    currentDir.chop(1);
    filter=currentDir+separator+filterExtension;
    ledFilter->setText(QDir::toNativeSeparators(filter));
    showFilesLikeFilter();
}*/

void FileSelector::toggleHiddenFile(int i)
{
    if (i==Qt::Checked)
    {
        modelDirs->setFilter(QDir::AllDirs | QDir::Hidden);
        modelFiles->setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);
    }
    else
    {
        modelDirs->setFilter(QDir::AllDirs);
        modelFiles->setFilter(QDir::Files | QDir::NoDotAndDotDot);
    }
    showFilesLikeFilter();
}

void FileSelector::currentDirChanged(int i)
{
    switch (i)
    {
    case 0://CWD
        currentDir=QString(get_workingdir());
        currentDir.chop(1);
        break;
    case 1://HOME
        currentDir=QString(get_userhome());
        currentDir.chop(1);
#ifdef WINDOWS_SYSTEM
        onHighestLevel=true;
#endif
        break;
    case 2://'/'
#ifdef WINDOWS_SYSTEM
        currentDir=QString("C:\\");
#else
        currentDir=QString("");
#endif
        break;
    case 3:
        currentDir=QString(qt_grace_exe_dir);
        break;
    }
    filter=currentDir+separator+filterExtension;
    ledFilter->setText(QDir::toNativeSeparators(filter));
    showFilesLikeFilter();
}

void FileSelector::GeneratePathWithExtension(QDir dir,QString & PathAndExtension,QString & DirectoryOnly)
{
    QString str=dir.cleanPath(dir.absolutePath()).toLatin1();
    QString current;
    if (str.startsWith("/.."))
    {
        while (str.startsWith("/.."))
        {
            current=str.right(str.length()-3);
            str=current;
        }
    }
    else
    {
        current=dir.absolutePath();
    }
#ifdef WINDOWS_SYSTEM
    while (current.count(QString(":"))>1)
    {
        str=current;
        current=str.right(str.length()-3);
    }
    if (current.contains(QString(":")))
    {
        current=current.mid(current.indexOf(QString(":"))-1);
    }
    onHighestLevel=false;
#endif
    PathAndExtension=current+separator+filterExtension;
    PathAndExtension=QDir::cleanPath(PathAndExtension);
    PathAndExtension=QDir::toNativeSeparators(PathAndExtension);
    DirectoryOnly=current+separator;
    DirectoryOnly=QDir::cleanPath(DirectoryOnly);
    DirectoryOnly=QDir::toNativeSeparators(DirectoryOnly)+separator;
}

void FileSelector::doGoUp(void)
{
    QDir cdir(currentDir);
    QDir tdir(cdir);
    /*QString str2=currentDir;
char strbuf[512];
strcpy(strbuf,str2.toLatin1());
if (strncmp(strbuf,"/..",3)==0)
{
char buf2[512];
strcpy(buf2,strbuf+3);
cout << "oh oh" << endl;
strcpy(strbuf,cdir.cleanPath(tdir.absolutePath()).toLatin1());
cout << "buf=" << strbuf << endl;
tdir=QDir(buf2);
}
cout << "currentdir=#" << strbuf << "#" << endl;
QString toEmit;
str2.chop(1+cdir.dirName().length());
//currentDir=str2;
strcpy(strbuf,str2.toLatin1());
cout << "updir1=#" << strbuf << "#" << endl;
*/
    bool up_possible=tdir.cdUp();
    /*strcpy(strbuf,tdir.path().toLatin1());
cout << "updir2=#" << strbuf << "# possible=" << up_possible << endl;*/
#ifdef WINDOWS_SYSTEM
    if (up_possible==false || onHighestLevel==true)
#else
    if (up_possible==false)
#endif
    {
#ifdef WINDOWS_SYSTEM
        currentDirChanged(1);
#endif
        return;
    }

    currentDir=tdir.absolutePath();
    currentDir=QDir::cleanPath(currentDir);
    QString newFilter,newPath;
    GeneratePathWithExtension(tdir,newFilter,newPath);
    filter=newFilter;
    ledFilter->setText(newFilter);
    emit(newSelection(newPath));
    /*currentDir=tdir.absolutePath();
filter=currentDir+separator+filterExtension;
toEmit=QDir::cleanPath(filter);
toEmit=QDir::toNativeSeparators(toEmit);
ledFilter->setText(toEmit);
toEmit=QDir::cleanPath(currentDir+separator);
toEmit=QDir::toNativeSeparators(toEmit);
emit(newSelection(toEmit));*/
    showFilesLikeFilter();
}

void FileSelector::doStdDialog(void)
{
    QString s;
    if (forRead==true)
        s=QFileDialog::getOpenFileName(this,currentDir);
    else
        s=QFileDialog::getSaveFileName(this,currentDir);
    if (!s.isNull())
    {
        /*lazy method of finding filename and directory*/
        ledFilter->setText(QDir::toNativeSeparators(s));
        newFilterEntered();
        //set and announce new selection and filters
        selectedFile=filter;
        filterExtension=QString("*");
        filter=currentDir+separator+filterExtension;
        emit(newSelection(selectedFile));
        //show selection again
        ///modify to do this only for existing files!!!
        ledFilter->setText(QDir::toNativeSeparators(filter));
        newFilterEntered();
        QModelIndex index=modelFiles->index(selectedFile);
        FileList->setCurrentIndex(index);
    }
}

stdStartStop::stdStartStop(QWidget * parent):QWidget(parent)
{
    lblStart=new QLabel(tr("Start load at:"),this);
    lblStop=new QLabel(tr("Stop load at:"),this);
    lblLength=new QLabel(tr("# of points:"),this);
    ledStart=new QLineEdit(QString(""),this);
    ///ledStart->setValidator(new QDoubleValidator(this));
    ledStop=new QLineEdit(QString(""),this);
    //ledStop->setValidator(new QDoubleValidator(this));
    ledLength=new QLineEdit(QString(""),this);
    QIntValidator * intval=new QIntValidator(this);
    intval->setBottom(1);
    ledLength->setValidator(intval);
    //ledLength->setValidator(new QRegExpValidator(rx, this));
    error=false;//state indicating conversion errors
    layout=new QHBoxLayout();
    layout->setMargin(2);
    layout->addWidget(lblStart);
    layout->addWidget(ledStart);
    layout->addWidget(lblStop);
    layout->addWidget(ledStop);
    layout->addWidget(lblLength);
    layout->addWidget(ledLength);
    setLayout(layout);
}

void stdStartStop::Redisplay(void)
{
    QString te;
    if (OldDecimalPoint==DecimalPointToUse) return;
    te=ledStart->text();
    te.replace(QChar(OldDecimalPoint),QChar(DecimalPointToUse));
    ledStart->setText(te);
    te=ledStop->text();
    te.replace(QChar(OldDecimalPoint),QChar(DecimalPointToUse));
    ledStop->setText(te);
}

double stdStartStop::start(void)
{
    double val;
    error=xv_evalexpr(ledStart, &val)==RETURN_SUCCESS?false:true;
    /*int len=ledStart->text().length();
if (len<=0)
{
error=true;
return 0.0;
}
char dummy[ledStart->text().length()];
strcpy(dummy,ledStart->text().toLatin1());
return atof(dummy);*/
    return val;
}

double stdStartStop::stop(void)
{
    double val;
    error=xv_evalexpr(ledStop, &val)==RETURN_SUCCESS?false:true;
    /*int len=ledStop->text().length();
if (len<=0)
{
error=true;
return 0.0;
}
char dummy[ledStop->text().length()];
strcpy(dummy,ledStop->text().toLatin1());
return atof(dummy);*/
    return val;
}

int stdStartStop::length(void)
{
    int val;
    error=xv_evalexpri(ledLength,&val)==RETURN_SUCCESS?false:true;
    /*int len=ledLength->text().length();
if (len<=0)
{
error=true;
return 0.0;
}
char dummy[ledLength->text().length()];
strcpy(dummy,ledLength->text().toLatin1());
return atoi(dummy);*/
    return val;
}

stdSetTypeSelector::stdSetTypeSelector(QWidget * parent):StdSelector(parent)
{
    lblText=new QLabel("Type:");
    cmbSelect=new QComboBox();
    number_of_entries=column_count=2;
    entries=new QString[column_count];
    entryValues=new int[column_count];
    entries[1]=QString("");
    entries[0]=QString("");
    entryValues[0]=0;
    entryValues[1]=1;
    cmbSelect->addItem(entries[0]);
    cmbSelect->addItem(entries[1]);

    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(lblText);
    layout->addWidget(cmbSelect);
    setLayout(layout);
    showAll=false;
    setColumnCount(column_count);
}

void stdSetTypeSelector::setColumnCount(int count)
{
    if (showAll==false)
        column_count=count;
    char dummy[512];
    ///SET APPROPRIATE SET_TYPE_CHOICES ACCORDING TO NUMBER OF COLUMNS --> SHOW ONLY AVAILABLE CHOICES, I DON'T KNOW HOW TO DISABLE CHOICES
    int number_of_Type_entries=0;
    for (int i=0;i<NUMBER_OF_SETTYPES;i++)
        if (settype_cols(i) == count || showAll==true) number_of_Type_entries++;
    int * Type_entries=new int[number_of_Type_entries];
    QString * entr=new QString[number_of_Type_entries];
    number_of_Type_entries=0;
    for (int i=0;i<NUMBER_OF_SETTYPES;i++)
    {
        if (settype_cols(i) == count || showAll==true)
        {
            strToUpper(dummy,set_types(i));
            entr[number_of_Type_entries]=QString(dummy);
            Type_entries[number_of_Type_entries]=i;
            number_of_Type_entries++;
        }
    }
    setNewEntries(number_of_Type_entries,entr,Type_entries);
}

void stdSetTypeSelector::setShowAll(bool sa)
{
    showAll=sa;
    setColumnCount(column_count);
}

int stdSetTypeSelector::getColumnCount(void)
{
    return settype_cols(currentValue());
}

SetTableView::SetTableView(int gno,int sno,int * colformat,int * colprecision,QWidget * parent):QTableView(parent)
{
    model=new SetTableModel(gno,sno,colformat,colprecision,parent);
    setModel(model);
}

bool SetTableView::edit(const QModelIndex &index, EditTrigger trigger, QEvent *event)
{
    int row=index.row();
    int col=index.column();
    //cout << "edit:" << row << "|" << col << endl;
    bool ret=QTableView::edit(index,trigger,event);
    return ret;
}

void SetTableView::dataChanged(const QModelIndex &topLeft,const QModelIndex &bottomRight)
{
    //cout << "data changed" << endl;
    QTableView::dataChanged(topLeft, bottomRight);
}

SetTableModel::SetTableModel(int gno,int sno,int * colformat,int * colprecision,QObject * parent):QAbstractTableModel(parent)
{
    setnr=sno;
    graphnr=gno;
    col_format=colformat;
    col_precision=colprecision;
}

bool SetTableModel::init(int gno,int sno)
{
    if (!is_valid_setno(gno,sno)) return false;
    setnr=sno;
    graphnr=gno;
    return true;
}

int SetTableModel::rowCount(const QModelIndex & parent) const
{
    if (!is_valid_setno(graphnr,setnr))
        return 0;
    else
        return getsetlength(graphnr,setnr)+10;//10 empty lines
}

int SetTableModel::columnCount(const QModelIndex & parent) const
{
    if (!is_valid_setno(graphnr,setnr))
        return 0;
    else
        return dataset_cols(graphnr,setnr);
}

QVariant SetTableModel::data(const QModelIndex & index,int role) const
{
    static char dummy[128];
    static int row,col;
    static QVariant dat;
    dat=QVariant(QVariant::Invalid);
    if (!is_valid_setno(graphnr,setnr)) return dat;
    row=index.row();
    col=index.column();
    //cout << "Data" << endl;
    if (role==Qt::DisplayRole || role==Qt::EditRole)
    {
        //cout << "Data=VALID" << endl;
        if (row>=getsetlength(graphnr,setnr)) return QVariant(QString(""));
        construct_string_from_data(g[graphnr].p[setnr].data.ex[col][row],dummy,col_precision[col],col_format[col]);
        return QVariant(QString(dummy));
    }
    else
    {
        //cout << "Data=invalid" << endl;
        return QVariant::Invalid;
    }
}

void construct_string_from_data(double value,char * target,int col_precision,int col_format)
{
    static char setPrec[8];
    static char out_format[16];
    static char dummy[128];
    //cout << "construct: value=" << value << endl;
    sprintf(setPrec,"%%.%d",col_precision);
    switch (col_format)
    {
    case 0://decimal
        sprintf(out_format,"%sf",setPrec);
        break;
    case 1://general
        sprintf(out_format,"%sg",setPrec);
        break;
    case 2://exponential
        sprintf(out_format,"%se",setPrec);
        break;
    case 3://Date/time
        sprintf(out_format,"%%t");///NOT CORRECT
        break;
    }
    sprintf(dummy,out_format,value);
    if (col_format!=3)//no changes if it is in date/time-format, otherwise-->correct the decimal separator
    {
        SetDecimalSeparatorToUserValue(dummy);
    }
    strcpy(target,dummy);
}

bool SetTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    static int row,col;
    static double val;
    static bool ok,really_new_value,set_is_new;
    static QVariant n_val;
    char * oldval,*newval;
    ok=true;
    //cout << "SetData" << endl;
    if (!is_valid_setno(graphnr,setnr)) return false;//invalid set
    really_new_value=false;
    val=value.toDouble(&ok);
    if (role==Qt::DisplayRole || !is_valid_setno(graphnr,setnr) || ok==false) return false;
    row=index.row();
    col=index.column();
    oldval=new char[256];
    newval=new char[256];
    strcpy(newval,value.toString().toLatin1().constData());
    //cout << "newValue=" << newval << endl;
    if (row<getsetlength(graphnr,setnr))//data already exists --> do we need to override it?
    {
        construct_string_from_data(g[graphnr].p[setnr].data.ex[col][row],oldval,col_precision[col],col_format[col]);
        if (!strcmp(newval,oldval))
            really_new_value=false;
        else
            really_new_value=true;
    }
    else
        really_new_value=true;
    ReplaceDecimalSeparator(newval);
    //cout << "a) value=" << value.toString().toLatin1().constData() << endl;
    n_val=QVariant(QString(newval));
    //cout << "b) value=" << n_val.toString().toLatin1().constData() << endl;
    delete[] oldval;
    delete[] newval;
    if (really_new_value==false) return true;//nothing has actually to be changed
    val=n_val.toDouble(&ok);
    if (ok==false)
    {
        cerr << "Invalid double-conversion from #" << n_val.toString().toLatin1().constData() << "#" << endl;
        return false;//invalid conversion
    }
    if (row<getsetlength(graphnr,setnr))//change an existing data point
    {
        int len=1,ncols;
        int * nrs=new int[2];
        nrs[0]=row;
        Datapoint * po=new Datapoint[2];
        int ret=get_datapoint(graphnr,setnr,row,&ncols,po);
        po->ex[col]=val;
        DataPointEdited(graphnr,setnr,nrs,len,po,2);
        g[graphnr].p[setnr].data.ex[col][row]=val;
        delete[] nrs;
        delete[] po;
    }
    else//adding rows (filled with zeros) and set the last one to the new value
    {
        if (getsetlength(graphnr,setnr)<=0)
            set_is_new=true;
        else
            set_is_new=false;
        Datapoint dpoint;
        zero_datapoint(&dpoint);
        int len=row-getsetlength(graphnr,setnr)+1,ncols;
        int counter=0;
        int * nrs=new int[len+1];
        Datapoint * po=new Datapoint[len+1];
        for (int i=0;i<len;i++)
        {
            zero_datapoint(po+i);
        }
        po[len-1].ex[col]=val;
        for (int i=getsetlength(graphnr,setnr);i<=row;i++)
        {
            nrs[counter++]=i;
            add_point_at(graphnr,setnr, i, &dpoint);
        }
        g[graphnr].p[setnr].data.ex[col][row]=val;
        if (set_is_new==true)
        {
            int klen=1;
            int * gnos=new int[2];
            int * snos=new int[2];
            gnos[0]=graphnr;
            snos[0]=setnr;
            SetsCreated(klen,gnos,snos,UNDO_COMPLETE);
            delete[] gnos;
            delete[] snos;
        }
        else
        {
            DataPointEdited(graphnr,setnr,nrs,len,po,0);
        }
    }
    ok=true;
    emit(item_entry_changed(row,col,really_new_value));
    return ok;
}

QVariant SetTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    static QVariant dat;
    dat=QVariant(QVariant::Invalid);
    if (role==Qt::DisplayRole)
    {
        if (orientation==Qt::Vertical)//row
        {
            dat=QVariant(section);//only the row number
        }
        else if (orientation==Qt::Horizontal)//column
        {
            if (section==0)
                dat=QVariant("X");
            else if (section==1)
                dat=QVariant("Y");
            else
            {
                char dummy[8];
                sprintf(dummy,"Y%d",section-1);
                dat=QVariant(dummy);
            }
        }
    }
    /*else
{
    cout << "role=" << (int)role << endl;
}*/
    return dat;
}

bool SetTableModel::insertRows(int row, int count, const QModelIndex & parent)
{
    beginInsertRows(parent,row,row+count-1);
    endInsertRows();
    return true;
}

bool SetTableModel::removeRows(int row, int count, const QModelIndex & parent)
{
    beginRemoveRows(parent,row,row+count-1);
    endRemoveRows();
    return true;
}

bool SetTableModel::insertColumns(int column, int count, const QModelIndex & parent)
{
    return false;
}

bool SetTableModel::removeColumns(int column, int count, const QModelIndex & parent)
{
    return false;
}

Qt::ItemFlags SetTableModel::flags( const QModelIndex & index ) const
{
    if (index.isValid())
        return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
    else
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void SetTableModel::major_changes(void)
{
    emit(layoutChanged());
}

/* Wrappers */
int GetSpinChoice(stdIntSelector * s)
{
    return s->value();
}

double GetSpinChoice(LineWidthSelector * s)
{
    return s->value();
}

double GetSpinChoice(QDoubleSpinBox * s)	
{
    return s->value();
}

bool GetToggleButtonState(QCheckBox * c)
{
    return c->isChecked();
}

int GetChoice(StdSelector * s)
{
    return s->currentValue();
}

int GetChoice(PositionSelector * s)
{
    return s->currentIndex();
}

int GetOptionChoice(ColorSelector * s)
{
    return s->currentIndex();
}

int GetOptionChoice(LineStyleSelector * s)
{
    return s->currentIndex();
}

int GetOptionChoice(FillPatternSelector * s)
{
    return s->currentIndex();
}

int GetOptionChoice(JustificationSelector * s)
{
    return s->currentIndex();
}

int GetOptionChoice(FontSelector * s)
{
    return s->currentIndex();
}

int GetOptionChoice(QComboBox * c)
{
    return c->currentIndex();
}

int GetOptionChoice(StdSelector * sel)
{
    return sel->currentValue();
}

double GetCharSizeChoice(stdSlider * sel)
{
    return sel->value();
}

double GetAngleChoice(stdSlider * sel)
{
    return sel->value();
}

char * GetTextString(stdLineEdit * led)
{
    static char dummy[MAX_STRING_LENGTH];
    //strcpy(dummy,led->lenText->text().toLatin1());
    //strcpy(dummy,led->text().toLatin1());
    strcpy(dummy,led->text().toUtf8().constData());
    return dummy;
}

void SetToggleButtonState(QCheckBox * cb, int v)
{
    cb->setChecked(bool(v));
}

void SetToggleButtonState(QAction * c, int v)
{
    c->setChecked(bool(v));
}

bool GetToggleButtonState(QAction * c)
{
    return c->isChecked();
}

void SetChoice(StdSelector * sel,int v)
{
    sel->setCurrentValue(v);
}

void SetOptionChoice(ColorSelector * sel,int v)
{
    sel->setCurrentIndex(v);
}

void SetOptionChoice(LineStyleSelector * sel,int v)
{
    sel->setCurrentIndex(v);
}

void SetOptionChoice(FillPatternSelector * sel,int v)
{
    sel->setCurrentIndex(v);
}

void SetOptionChoice(FontSelector * sel,int v)
{
    sel->setCurrentIndex(v);
}

void SetOptionChoice(JustificationSelector * sel,int v)
{
    sel->setCurrentIndex(v);
}

void SetOptionChoice(StdSelector * sel,int v)
{
    sel->setCurrentValue(v);
}

void SetSpinChoice(stdIntSelector * sel, double v)
{
    sel->setValue((int)v);
}

void SetSpinChoice(LineWidthSelector * sel, double v)
{
    sel->setValue((int)v);
}

void SetChoice(PositionSelector * sel,int v)
{
    sel->setCurrentIndex(v);
}

void SetScaleValue(stdSlider * sel, int v)
{
    sel->setValue(v);
}

void SetCharSizeChoice(stdSlider * sel,double v)
{
    sel->setValue(int(v));
}

void SetAngleChoice(stdSlider * sel,double v)
{
    sel->setValue((int)v);
}

void SetTextString(stdLineEdit * led,char * st)
{
    led->setText(st);
}

void SetLabel(QLabel * lab,char * st)
{
    lab->setText(QString(st));
}

void xv_setstr(stdLineEdit * led, char * text)
{
    led->setText(QString(text));
}

void xv_setstr(QLineEdit * led, char * text)
{
    led->setText(QString(text));
}

void SetSensitive(stdLineEdit * led, bool e)
{
    led->setEnabled(e);
}

void SetSensitive(QLineEdit * led, bool e)
{
    led->setEnabled(e);
}

void SetSensitive(StdSelector * led, bool e)
{
    led->setEnabled(e);
}

void SetSensitive(QPushButton * led, bool e)
{
    led->setEnabled(e);
}

void SetSensitive(QCheckBox * led, bool e)
{
    led->setEnabled(e);
}

int GetScaleValue(stdSlider * sel)
{
    return sel->value();
}

char * xv_getstr(stdLineEdit * led)
{
    /// WARNING POSSIBLY UNSAVE PROCEDURE!!!
    static char value[1024];
    strncpy(value,led->lenText->text().toLatin1(),1023);
    value[1023]='\0';
    return value;
}

char * xv_getstr(QLineEdit * led)
{
    /// WARNING POSSIBLY UNSAVE PROCEDURE!!!
    static char value[1024];
    strncpy(value,led->text().toLatin1(),1023);
    value[1023]='\0';
    return value;
}

int xv_evalexpr(stdLineEdit * w, double * answer )
{
    return xv_evalexpr(w->lenText,answer);
}

int xv_evalexpri(stdLineEdit * w,int * answer)
{
    return xv_evalexpri(w->lenText,answer);
}

/*
 * xv_evalexpr - take a text field and pass it to the parser if it needs to be
 * evaluated, else use atof().
 * place the double result in answer
 * if an error, return False, else True
 */
int xv_evalexpr(QLineEdit * w,double * answer)
{
static char *buf = NULL;
int ret=RETURN_FAILURE;
QString st=w->text();
buf=new char[st.length()+1];
strcpy(buf,st.toLatin1());//we assume, that all fields that have numbers in it for use as numbers can safely be interpreted in the iso-latin1-codec
ret=std_evalexpr(buf,answer);
delete[] buf;
return ret;
}

int std_evalexpr(char * buf,double * answer)
{
int i, len, ier = 0;
double result;

    ReplaceDecimalSeparator(buf);

    if (!(len = strlen( buf ) )) { /* check for zero length */
        *answer = 0;
        return RETURN_FAILURE;
    }
    /* first character may be a sign */
    if (!fpdigit[(int) buf[0]] && buf[0] != '-' && buf[0] != '+') {
        i = len +1;
    } else {
        i = 1;
    }

    for (; i<len; i++) {
        if (!fpdigit[(int) buf[i]]) {
            break;
        }
    }

    if (i == len) {         /* only floating point digits */
        *answer = atof( buf );
        return RETURN_SUCCESS;
    } else {                /* must evaluate an expression */
        ier = s_scanner(buf, &result);
        if( !ier ) {
            *answer = result;
            return RETURN_SUCCESS;
        } else {
            *answer = 0;
            return RETURN_FAILURE;
        }
    }
}

/*
 * xv_evalexpri - take a text field and pass it to the parser if it needs to
 * evaluated, else use atoi().
 * place the integer result in answer
 * if an error, return False, else True
 */
int xv_evalexpri(QLineEdit * w,int * answer)
{
    static char *buf = NULL;
    int i, len, ier = 0;
    double result;
    QString st=w->text();
    buf=new char[st.length()+1];
    strcpy(buf,st.toLatin1());
    ReplaceDecimalSeparator(buf);

    if (!(len = strlen( buf ) )) { /* check for zero length */
        *answer = 0;
        return RETURN_FAILURE;
    }
    /* first character may be a sign */
    if (!fpdigit[(int) buf[0]] && buf[0] != '-' && buf[0] != '+') {
        i = len +1;
    } else {
        i = 1;
    }

    for (; i<len; i++) {
        if (!fpdigit[(int) buf[i]]) {
            break;
        }
    }

    if (i == len) {             /* only floating point digits */
        *answer = atoi(buf);
        return RETURN_SUCCESS;
    } else {                    /* must evaluate an expression */
        ier = s_scanner(buf, &result);
        if( !ier ) {
            *answer = (int)result;
            return RETURN_SUCCESS;
        } else {
            *answer = 0;
            return RETURN_FAILURE;
        }
    }
}

void SelectListChoice(uniList * list,int nr)
{
    int count=1;
    int selection[2]={nr,1};
    list->set_new_selection(count,selection);
}

void SelectListChoices(uniList * list,int count,int * nrs)
{
    list->set_new_selection(count,nrs);
}

int GetListChoices(uniList * list, int ** sel)
{
    int nr;
    list->get_selection(&nr,sel);
    return nr;
}

int GetSelectedSets(uniList * list, int ** sel)
{
    int nr;
    list->get_selection(&nr,sel);
    return nr;
}

int GetSelectedSet(uniList * list)
{
    int nr,*sel=new int[3];
    list->get_selection(&nr,&sel);
    if (nr>0)
        nr=sel[0];
    else
        nr=SET_SELECT_ERROR;
    delete[] sel;
    return nr;
}

int GetSingleListChoice(uniList * list, int * sel)
{
    int nr;
    int * selection=new int[5];
    list->get_selection(&nr,&selection);
    if (nr<=0)
    {
        delete[] selection;
        return RETURN_FAILURE;
    }
    else
    {
        *sel=selection[0];
        delete[] selection;
        return RETURN_SUCCESS;
    }
}

GraphPopup::GraphPopup(uniList *parent):QMenu(parent)
{
    par=parent;
    CreateActions();
    setTearOffEnabled(FALSE);

    //Create Menu itself
    addAction(actLabel);
    addSeparator();
    addAction(actFocusTo);
    addAction(actHide);
    addAction(actShow);
    addAction(actDuplicate);
    addAction(actKill);
    addSeparator();
    addAction(actCopy12);
    addAction(actCopy21);
    addAction(actMove12);
    addAction(actMove21);
    addAction(actSwap);
    addSeparator();
    addAction(actCreateNew);

    number_of_selected_graphs=0;
    selected_graphs=new int[2];
}

GraphPopup::~GraphPopup()
{
    delete[] selected_graphs;
}

void GraphPopup::CreateActions(void)
{
    actLabel=new QAction(tr("G0"),this);
    connect(actLabel,SIGNAL(triggered()), this, SLOT(doNothing()));
    actFocusTo=new QAction(tr("&Focus to"),this);
    connect(actFocusTo,SIGNAL(triggered()), this, SLOT(doFocusTo()));
    actShow=new QAction(tr("&Show"),this);
    connect(actShow,SIGNAL(triggered()), this, SLOT(doShow()));
    actHide=new QAction(tr("&Hide"),this);
    connect(actHide,SIGNAL(triggered()), this, SLOT(doHide()));
    actDuplicate=new QAction(tr("&Duplicate"),this);
    connect(actDuplicate,SIGNAL(triggered()), this, SLOT(doDuplicate()));
    actKill=new QAction(tr("&Kill"),this);
    connect(actKill,SIGNAL(triggered()), this, SLOT(doKill()));
    actCopy12=new QAction(tr("Copy G0 to G1"),this);
    connect(actCopy12,SIGNAL(triggered()), this, SLOT(doCopy12()));
    actCopy21=new QAction(tr("Copy G1 to G0"),this);
    connect(actCopy21,SIGNAL(triggered()), this, SLOT(doCopy21()));
    actMove12=new QAction(tr("Move G0 to G1"),this);
    connect(actMove12,SIGNAL(triggered()), this, SLOT(doMove12()));
    actMove21=new QAction(tr("Move G1 to G0"),this);
    connect(actMove21,SIGNAL(triggered()), this, SLOT(doMove21()));
    actSwap=new QAction(tr("S&wap"),this);
    connect(actSwap,SIGNAL(triggered()), this, SLOT(doSwap()));
    actCreateNew=new QAction(tr("&Create new"),this);
    connect(actCreateNew,SIGNAL(triggered()), this, SLOT(doCreateNew()));
}

void GraphPopup::doNothing(void)
{
    ;
}

void GraphPopup::doFocusTo(void)
{
    ListOfChanges.clear();
    sprintf(dummy,"with g%d",selected_no);
    ListOfChanges << QString(dummy);
    switch_current_graph(selected_no);
    update();
}

void GraphPopup::doShow(void)
{
    ListOfChanges.clear();
    ListOfOldStates.clear();
    for (int i = 0; i < number_of_selected_graphs; i++)
    {
        if (is_graph_hidden(selected_graphs[i])==TRUE)//real change
        {
            sprintf(dummy,"g%d hidden false",selected_graphs[i]);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"g%d hidden true",selected_graphs[i]);
            ListOfOldStates << QString(dummy);
        }
    }
    ShowHideGraphs(number_of_selected_graphs,selected_graphs);
    ListOfChanges.clear();
    ListOfOldStates.clear();
    for (int i = 0; i < number_of_selected_graphs; i++)
        set_graph_hidden(selected_graphs[i], FALSE);
    mainWin->mainArea->completeRedraw();
}

void GraphPopup::doHide(void)
{
    ListOfChanges.clear();
    ListOfOldStates.clear();
    for (int i = 0; i < number_of_selected_graphs; i++)
    {
        if (is_graph_hidden(selected_graphs[i])==FALSE)//real change
        {
            sprintf(dummy,"g%d hidden true",selected_graphs[i]);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"g%d hidden false",selected_graphs[i]);
            ListOfOldStates << QString(dummy);
        }
    }
    ShowHideGraphs(number_of_selected_graphs,selected_graphs);
    ListOfChanges.clear();
    ListOfOldStates.clear();
    for (int i = 0; i < number_of_selected_graphs; i++)
        set_graph_hidden(selected_graphs[i], TRUE);
    mainWin->mainArea->completeRedraw();
}

void GraphPopup::doDuplicate(void)
{
    int * gnos=new int[number_of_selected_graphs+2];
    int index=0;
    for (int i = 0; i < number_of_selected_graphs; i++)
    {
        new_set_no=-1;
        duplicate_graph(selected_graphs[i]);
        if (new_set_no!=-1)
            gnos[index++]=new_set_no;
    }
    GraphsCreated(index,gnos,UNDO_COMPLETE);
    delete[] gnos;
    mainWin->mainArea->completeRedraw();
}

void GraphPopup::doKill(void)
{
    if (yesno("Kill selected graph(s)?", NULL, NULL, NULL))
    {
        ListOfChanges.clear();
        int * graphs=new int[number_of_selected_graphs];
        for (int i = number_of_selected_graphs - 1; i >= 0; i--)
        {
            sprintf(dummy,"kill G%d",selected_graphs[i]);
            ListOfChanges << QString(dummy);
            graphs[(number_of_selected_graphs - 1)-i]=selected_graphs[i];
        }
        GraphsDeleted(number_of_selected_graphs,graphs,UNDO_COMPLETE);
        delete[] graphs;
        for (int i = number_of_selected_graphs - 1; i >= 0; i--)
        {
            kill_graph(selected_graphs[i]);
        }
        ListOfChanges.clear();
    }
    mainWin->mainArea->completeRedraw();
}

void GraphPopup::doSwap(void)
{
    UndoSwapGraphs(selected_graphs[0], selected_graphs[1]);
    swap_graph(selected_graphs[0], selected_graphs[1]);
    update_graph_selectors();
}

void GraphPopup::doCreateNew(void)
{
    int * gnos=new int[2];
    gnos[1]=gnos[0]=number_of_graphs();
    set_graph_active(number_of_graphs());
    GraphsCreated(1,gnos,UNDO_COMPLETE);
    delete[] gnos;
    mainWin->mainArea->completeRedraw();
}

void GraphPopup::update(void)
{
    par->update_number_of_entries_preserve_selection();
    mainWin->mainArea->completeRedraw();
}

void GraphPopup::doMove12(void)
{
    char buf[32];
    if (number_of_selected_graphs==2)
    {
        sprintf(buf, "Replace G%d?", selected_graphs[1]);
        if (yesno(buf, NULL, NULL, NULL))
        {
            SaveGraphStatesPrevious(number_of_selected_graphs,selected_graphs,UNDO_COMPLETE);
            move_graph(selected_graphs[0], selected_graphs[1]);
            selected_graphs[0]=selected_graphs[1];
            GraphsModified(1,selected_graphs,UNDO_COMPLETE);
        }
        mainWin->mainArea->completeRedraw();
    }
}

void GraphPopup::doMove21(void)
{
    char buf[32];
    if (number_of_selected_graphs==2)
    {
        sprintf(buf, "Replace G%d?", selected_graphs[0]);
        if (yesno(buf, NULL, NULL, NULL))
        {
            SaveGraphStatesPrevious(number_of_selected_graphs,selected_graphs,UNDO_COMPLETE);
            move_graph(selected_graphs[1], selected_graphs[0]);
            GraphsModified(1,selected_graphs,UNDO_COMPLETE);
        }
        mainWin->mainArea->completeRedraw();
    }
}

void GraphPopup::doCopy12(void)
{
    char buf[32];
    if (number_of_selected_graphs == 2)
    {
        sprintf(buf, "Overwrite G%d?", selected_graphs[1]);
        if (yesno(buf, NULL, NULL, NULL))
        {
            SaveGraphStatesPrevious(number_of_selected_graphs,selected_graphs,UNDO_COMPLETE);
            copy_graph(selected_graphs[0], selected_graphs[1]);
            GraphsModified(number_of_selected_graphs,selected_graphs,UNDO_COMPLETE);
        }
        mainWin->mainArea->completeRedraw();
    }
}

void GraphPopup::doCopy21(void)
{
    char buf[32];
    if (number_of_selected_graphs == 2)
    {
        sprintf(buf, "Overwrite G%d?", selected_graphs[0]);
        if (yesno(buf, NULL, NULL, NULL))
        {
            SaveGraphStatesPrevious(number_of_selected_graphs,selected_graphs,UNDO_COMPLETE);
            copy_graph(selected_graphs[1], selected_graphs[0]);
            GraphsModified(number_of_selected_graphs,selected_graphs,UNDO_COMPLETE);
        }
        mainWin->mainArea->completeRedraw();
    }
}

void GraphPopup::update_menu_content(void)
{
    char dummy[64],dummy2[64];
    sprintf(dummy,"G");
    for (int i=0;i<number_of_selected_graphs;i++)
    {
        sprintf(dummy2,"%d",selected_graphs[i]);
        strcat(dummy,dummy2);
        if (i>2 && i!=number_of_selected_graphs-1)
        {
            strcat(dummy,",...");
            break;
        }
        else
        {
            if (i!=number_of_selected_graphs-1)
                strcat(dummy,", G");
        }
    }
    actLabel->setText(QString(dummy));
    if (number_of_selected_graphs==1)
    {
        actCopy12->setVisible(false);
        actCopy21->setVisible(false);
        actMove12->setVisible(false);
        actMove21->setVisible(false);
        actSwap->setEnabled(false);
        actFocusTo->setEnabled(true);
    }
    else
    {
        if (number_of_selected_graphs==2)
        {
            sprintf(dummy,"Copy G%d to G%d",selected_graphs[0],selected_graphs[1]);
            actCopy12->setText(QString(dummy));
            actCopy12->setVisible(true);
            sprintf(dummy,"Copy G%d to G%d",selected_graphs[1],selected_graphs[0]);
            actCopy21->setText(QString(dummy));
            actCopy21->setVisible(true);
            sprintf(dummy,"Move G%d to G%d",selected_graphs[0],selected_graphs[1]);
            actMove12->setText(QString(dummy));
            actMove12->setVisible(true);
            sprintf(dummy,"Move G%d to G%d",selected_graphs[1],selected_graphs[0]);
            actMove21->setText(QString(dummy));
            actMove21->setVisible(true);
            actSwap->setEnabled(true);
        }
        else
        {
            actCopy12->setVisible(false);
            actCopy21->setVisible(false);
            actMove12->setVisible(false);
            actMove21->setVisible(false);
            actSwap->setEnabled(false);
        }
        actFocusTo->setEnabled(false);
    }
}

SetPopup::SetPopup(uniList *parent):QMenu(parent)
{
    par=parent;
    CreateActions();
    setTearOffEnabled(FALSE);

    //Create Menu itself
    addAction(actLabel);
    addSeparator();
    addAction(actHide);
    addAction(actShow);
    addAction(actBringToFront);
    addAction(actSendToBack);
    addSeparator();
    addAction(actDuplicate);
    addAction(actKill);
    addAction(actKillData);
    addSeparator();
    addAction(actCopyClipBoard);
    addAction(actPasteClipBoard);
    addSeparator();
    addAction(actCopy12);
    addAction(actCopy21);
    addAction(actMove12);
    addAction(actMove21);
    addAction(actSwap);
    addSeparator();
    mnuEdit=new QMenu(tr("&Edit"),this);
    mnuEdit->setTearOffEnabled(FALSE);
    mnuEdit->addAction(actEditInSpreadsheet);
    mnuEdit->addAction(actEditInTextEditor);
    mnuCreateNew=new QMenu(tr("Create new"),this);
    mnuCreateNew->setTearOffEnabled(FALSE);
    mnuCreateNew->addAction(actCreateByFormula);
    mnuCreateNew->addAction(actCreateInSpreadsheet);
    mnuCreateNew->addAction(actCreateInTextEditor);
    mnuCreateNew->addAction(actCreateFromBlockData);
    addMenu(mnuEdit);
    addMenu(mnuCreateNew);
    addSeparator();
    addAction(actPackAllSets);
    addSeparator();
    addAction(actStoreAppearance);
    addAction(actApplyStoredAppearance);
    addSeparator();
    mnuSelectorOperations=new QMenu(tr("Select&or operations"),this);
    mnuSelectorOperations->setTearOffEnabled(FALSE);
    mnuSelectorOperations->addAction(actViewSetComments);
    actViewSetComments->setCheckable(true);
    actViewSetComments->setChecked(FALSE);
    mnuSelectorOperations->addAction(actShowDataLess);
    actShowDataLess->setCheckable(true);
    actShowDataLess->setChecked(FALSE);
    mnuSelectorOperations->addAction(actShowHidden);
    actShowHidden->setCheckable(true);
    actShowHidden->setChecked(TRUE);
    mnuSelectorOperations->addSeparator();
    mnuSelectorOperations->addAction(actSelectAll);
    mnuSelectorOperations->addAction(actUnselectAll);
    mnuSelectorOperations->addAction(actInvertSelection);
    mnuSelectorOperations->addAction(actSelectEven);
    mnuSelectorOperations->addAction(actSelectOdd);
    mnuSelectorOperations->addAction(actSelectNth);
    mnuSelectorOperations->addAction(actSelectVisible);
    mnuSelectorOperations->addAction(actSelectInvisible);
    mnuSelectorOperations->addSeparator();
    mnuSelectorOperations->addAction(actUpdate);
    addMenu(mnuSelectorOperations);

    number_of_selected_sets=0;
    selected_sets=new int[2];
}

SetPopup::~SetPopup()
{
    delete[] selected_sets;
}

void SetPopup::CreateActions(void)
{
    actLabel=new QAction(tr("S0"),this);
    connect(actLabel,SIGNAL(triggered()), this, SLOT(doNothing()));
    actHide=new QAction(tr("Hide"),this);
    connect(actHide,SIGNAL(triggered()), this,SLOT(doHide()));
    actBringToFront=new QAction(tr("Bring to front"),this);
    connect(actBringToFront,SIGNAL(triggered()), this,SLOT(doBringToFront()));
    actSendToBack=new QAction(tr("Send to back"),this);
    connect(actSendToBack,SIGNAL(triggered()), this,SLOT(doSendToBack()));
    actPackAllSets=new QAction(tr("Pack all sets"),this);
    connect(actPackAllSets,SIGNAL(triggered()), this,SLOT(doPackAllSets()));
    actShow=new QAction(tr("Show"),this);
    connect(actShow,SIGNAL(triggered()), this,SLOT(doShow()));
    actDuplicate=new QAction(tr("Duplicate"),this);
    connect(actDuplicate,SIGNAL(triggered()), this,SLOT(doDuplicate()));
    actKill=new QAction(tr("Kill"),this);
    connect(actKill,SIGNAL(triggered()), this,SLOT(doKill()));
    actKillData=new QAction(tr("Kill data"),this);
    connect(actKillData,SIGNAL(triggered()), this,SLOT(doKillData()));
    actCopy12=new QAction(tr("Copy S0 to S1"),this);
    connect(actCopy12,SIGNAL(triggered()), this,SLOT(doCopy12()));
    actCopy21=new QAction(tr("Copy S1 to S0"),this);
    connect(actCopy21,SIGNAL(triggered()), this,SLOT(doCopy21()));
    actMove12=new QAction(tr("Move S0 to S1"),this);
    connect(actMove12,SIGNAL(triggered()), this,SLOT(doMove12()));
    actMove21=new QAction(tr("Move S1 to S0"),this);
    connect(actMove21,SIGNAL(triggered()), this,SLOT(doMove21()));
    actSwap=new QAction(tr("Swap"),this);
    connect(actSwap,SIGNAL(triggered()), this,SLOT(doSwap()));
    actCreateByFormula=new QAction(tr("By formula"),this);
    connect(actCreateByFormula,SIGNAL(triggered()), this,SLOT(doCreateByFormula()));
    actCreateInSpreadsheet=new QAction(tr("In spreadsheet"),this);
    connect(actCreateInSpreadsheet,SIGNAL(triggered()), this,SLOT(doCreateInSpreadsheet()));
    actCreateInTextEditor=new QAction(tr("In text Editor"),this);
    connect(actCreateInTextEditor,SIGNAL(triggered()), this,SLOT(doCreateInTextEditor()));
    actCreateFromBlockData=new QAction(tr("From block data"),this);
    connect(actCreateFromBlockData,SIGNAL(triggered()), this,SLOT(doCreateFromBlockData()));
    actEditInSpreadsheet=new QAction(tr("In spreadsheet"),this);
    connect(actEditInSpreadsheet,SIGNAL(triggered()), this,SLOT(doEditInSpreadSheet()));
    actEditInTextEditor=new QAction(tr("In text editor"),this);
    connect(actEditInTextEditor,SIGNAL(triggered()), this,SLOT(doEditInTextEditor()));
    actViewSetComments=new QAction(tr("View set comments"),this);
    connect(actViewSetComments,SIGNAL(triggered()), this,SLOT(doViewSetComments()));
    actShowDataLess=new QAction(tr("Show data-less"),this);
    connect(actShowDataLess,SIGNAL(triggered()), this,SLOT(doShowDataLess()));
    actShowHidden=new QAction(tr("Show hidden"),this);
    connect(actShowHidden,SIGNAL(triggered()), this,SLOT(doShowHidden()));
    actSelectAll=new QAction(tr("Select all"),this);
    connect(actSelectAll,SIGNAL(triggered()), this,SLOT(doSelectAll()));
    actUnselectAll=new QAction(tr("Unselect all"),this);
    connect(actUnselectAll,SIGNAL(triggered()), this,SLOT(doUnSelectAll()));
    actInvertSelection=new QAction(tr("Invert selection"),this);
    connect(actInvertSelection,SIGNAL(triggered()), this,SLOT(doInvertSelection()));
    actSelectEven=new QAction(tr("Select even"),this);
    connect(actSelectEven,SIGNAL(triggered()), this,SLOT(doSelectEven()));
    actSelectOdd=new QAction(tr("Select odd"),this);
    connect(actSelectOdd,SIGNAL(triggered()), this,SLOT(doSelectOdd()));
    actSelectVisible=new QAction(tr("Select visible"),this);
    connect(actSelectVisible,SIGNAL(triggered()), this,SLOT(doSelectVisible()));
    actSelectInvisible=new QAction(tr("Select invisible"),this);
    connect(actSelectInvisible,SIGNAL(triggered()), this,SLOT(doSelectInVisible()));
    actSelectNth=new QAction(tr("Select n-th (from n0)"),this);
    connect(actSelectNth,SIGNAL(triggered()), this,SLOT(doSelectNth()));
    actUpdate=new QAction(tr("Update"),this);
    connect(actUpdate,SIGNAL(triggered()), this,SLOT(doUpdate()));
    actCopyClipBoard=new QAction(tr("Copy"),this);
    connect(actCopyClipBoard,SIGNAL(triggered()),SLOT(doCopyClipBoard()));
    actPasteClipBoard=new QAction(tr("Paste"),this);
    connect(actPasteClipBoard,SIGNAL(triggered()),SLOT(doPasteClipBoard()));
    actStoreAppearance=new QAction(tr("Store appearrance(s)"),this);
    connect(actStoreAppearance,SIGNAL(triggered()),SLOT(doStoreAppearance()));
    actApplyStoredAppearance=new QAction(tr("Apply stored appearance(s)"),this);
    connect(actApplyStoredAppearance,SIGNAL(triggered()),SLOT(doApplyStoredAppearance()));
}

void SetPopup::prepareForAction(void)
{
    selected_graph=par->gr_no;
    par->get_selection(&number_of_selected_sets,&selected_sets);
}

void SetPopup::finishAction(int preserveSelection)
{
    par->update_number_of_entries();
    mainWin->mainArea->completeRedraw();
    if (preserveSelection==DO_PRESERVE_SELECTION)
        par->set_new_selection(number_of_selected_sets,selected_sets);
}

void SetPopup::doNothing(void)
{
    ;
}

void SetPopup::doHide(void)
{
    prepareForAction();
    ListOfChanges.clear();
    ListOfOldStates.clear();
    int * gnos=new int[number_of_selected_sets];
    int * snos=new int[number_of_selected_sets];
    sprintf(dummy,"with g%d",selected_graph);
    ListOfChanges << QString(dummy);
    ListOfOldStates << QString(dummy);
    for (int i=0;i<number_of_selected_sets;i++)
    {
        gnos[i]=selected_graph;
        snos[i]=selected_sets[i];
        sprintf(dummy,"    s%d hidden true",selected_sets[i]);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    s%d hidden false",selected_sets[i]);
        ListOfOldStates << QString(dummy);
        set_set_hidden(selected_graph, selected_sets[i], TRUE);
    }
    ///Undo-Stuff
    ShowHideSets(number_of_selected_sets,gnos,snos);
    ListOfChanges.clear();
    ListOfOldStates.clear();
    delete[] gnos;
    delete[] snos;
    finishAction(DO_PRESERVE_SELECTION);
}

void SetPopup::doShow(void)
{
    prepareForAction();
    ListOfChanges.clear();
    ListOfOldStates.clear();
    int * gnos=new int[number_of_selected_sets];
    int * snos=new int[number_of_selected_sets];
    sprintf(dummy,"with g%d",selected_graph);
    ListOfChanges << QString(dummy);
    ListOfOldStates << QString(dummy);
    for (int i=0;i<number_of_selected_sets;i++)
    {
        gnos[i]=selected_graph;
        snos[i]=selected_sets[i];
        sprintf(dummy,"    s%d hidden false",selected_sets[i]);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    s%d hidden true",selected_sets[i]);
        ListOfOldStates << QString(dummy);
        set_set_hidden(selected_graph, selected_sets[i], FALSE);
    }
    ///Undo-Stuff
    ShowHideSets(number_of_selected_sets,gnos,snos);
    ListOfChanges.clear();
    ListOfOldStates.clear();
    delete[] gnos;
    delete[] snos;
    finishAction(DO_PRESERVE_SELECTION);
}

void SetPopup::doSendToBack(void)
{
    int number;
    int * selection=new int[2];
    int gno=par->gr_no;
    par->get_selection(&number,&selection);
    if (number==1)
    {
        ChangeSetOrdering(gno,selection[0],PUSH_SET_TOBACK);
        pushset(gno, selection[0], PUSH_SET_TOBACK);
    }
    delete[] selection;
    par->update_number_of_entries();
    mainWin->mainArea->completeRedraw();
}

void SetPopup::doBringToFront(void)
{
    int number;
    int * selection=new int[2];
    int gno=par->gr_no;
    par->get_selection(&number,&selection);
    if (number==1)
    {
        ChangeSetOrdering(gno,selection[0],PUSH_SET_TOFRONT);
        pushset(gno, selection[0], PUSH_SET_TOFRONT);
    }
    delete[] selection;
    par->update_number_of_entries();
    mainWin->mainArea->completeRedraw();
}

void SetPopup::doDuplicate(void)
{
    int number;
    int * selection=new int[2];
    int gno=par->gr_no;
    int setno;
    int index=0;
    par->get_selection(&number,&selection);
    int * gnos=new int[number+2];
    int * snos=new int[number+2];
    for (int i=0;i<number;i++)
    {
        setno = nextset(gno);
        do_copyset(gno, selection[i], gno, setno);
        gnos[index]=gno;
        snos[index++]=setno;
    }
    SetsCreated(index,gnos,snos,UNDO_COMPLETE);
    delete[] selection;
    delete[] gnos;
    delete[] snos;
    par->update_number_of_entries_preserve_selection();
    mainWin->mainArea->completeRedraw();
}

void SetPopup::doKill(void)
{
    prepareForAction();
    if (yesno("Kill selected set(s)?", NULL, NULL, NULL))
    {
        ListOfChanges.clear();
        int * gnos=new int[number_of_selected_sets];
        for (int i=0;i<number_of_selected_sets;i++)
        {
            sprintf(dummy,"kill G%d.S%d",selected_graph,selected_sets[i]);
            ListOfChanges << QString(dummy);
            gnos[i]=selected_graph;
        }
        SetsDeleted(number_of_selected_sets,gnos,selected_sets,UNDO_COMPLETE);//UndoStuff
        delete[] gnos;
        for (int i=0;i<number_of_selected_sets;i++)
        {
            killset(selected_graph, selected_sets[i]);
        }
        finishAction(DO_NOT_PRESERVE_SELECTION);
        ListOfChanges.clear();
    }
    else
        finishAction(DO_PRESERVE_SELECTION);
}

void SetPopup::doKillData(void)
{
    prepareForAction();
    if (yesno("Kill data in selected set(s)?", NULL, NULL, NULL))
    {
        int * gnos=new int[number_of_selected_sets];
        for (int i=0;i<number_of_selected_sets;i++)
            gnos[i]=selected_graph;
        SetsDeleted(number_of_selected_sets,gnos,selected_sets,UNDO_COMPLETE);//UndoStuff
        char dummy[128];
        sprintf(dummy,"[G%d.S%d]",gnos[0],selected_sets[0]);
        if (number_of_selected_sets>1)
        {
            addAditionalDescriptionToLastNode(-1,QObject::tr("Kill set data"),QString(),-1);
        }
        else
        {
            addAditionalDescriptionToLastNode(-1,QObject::tr("Kill set data")+QString(dummy),QString(),-1);
        }
        delete[] gnos;
        for (int i=0;i<number_of_selected_sets;i++)
        {
            killsetdata(selected_graph, selected_sets[i]);
        }
        finishAction(DO_NOT_PRESERVE_SELECTION);
    }
    else
        finishAction(DO_PRESERVE_SELECTION);
}

void SetPopup::doSwap(void)
{
    int number;
    int * selection=new int[2];
    int gno=par->gr_no;
    par->get_selection(&number,&selection);
    if (number==2)
    {
        swapset(gno, selection[0], gno, selection[1]);
        UndoSwapSets(gno, selection[0], gno, selection[1]);
    }
    delete[] selection;
    par->update_number_of_entries();
    mainWin->mainArea->completeRedraw();
}

void SetPopup::doPackAllSets(void)
{
    int gno=par->gr_no;
    SetsAboutToBePacked(gno);
    packsets(gno);
    par->update_number_of_entries();
    mainWin->mainArea->completeRedraw();
}

void SetPopup::doEditInSpreadSheet(void)
{
    int gno=par->gr_no;
    int number;
    int * selection=new int[5];
    par->get_selection(&number,&selection);
    for (int i=0;i<number;i++)
    {
        showSetInSpreadSheet(gno,selection[i]);
    }
    delete[] selection;
}

void SetPopup::doEditInTextEditor(void)
{
    int gno=this->selected_graph;
    int setno=-1;
    if (this->number_of_selected_sets>0)
    {
        setno=this->selected_sets[0];
    }
    if (setno!= -1)
    {
        setcomment(gno, setno, "Editor");
        set_set_hidden(gno, setno, FALSE);
        if (FormSetEditor==NULL)
        {
            FormSetEditor=new frmSetEditor(0);
        }
        FormSetEditor->init(gno,setno);
        FormSetEditor->show();
        FormSetEditor->raise();
        FormSetEditor->activateWindow();
        ///FormSetEditor->setModal(true);
    }
}

void SetPopup::doCreateByFormula(void)
{
    if (FormLoadAndEvaluate==NULL)
    {
        FormLoadAndEvaluate=new frmLoadEval(0);
    }
    FormLoadAndEvaluate->show();
    FormLoadAndEvaluate->raise();
    FormLoadAndEvaluate->activateWindow();
}

void SetPopup::doCreateInSpreadsheet(void)
{
    int setno,gno=selected_graph;
    if ((setno = nextset(gno)) != -1) {
        setcomment(gno, setno, "Editor");
        set_set_hidden(gno, setno, FALSE);
        //create_ss_frame(gno, setno);
        showSetInSpreadSheet(gno,setno);
    } else {
        ;///cout << "error" << endl;
    }
}

void SetPopup::doCreateInTextEditor(void)
{
    int gno=this->selected_graph;
    int setno;
    if ((setno = nextset(gno)) != -1)
    {
        setcomment(gno, setno, "Editor");
        set_set_hidden(gno, setno, FALSE);
        if (FormSetEditor==NULL)
        {
            FormSetEditor=new frmSetEditor(0);
        }
        FormSetEditor->init(gno,setno);
        FormSetEditor->show();
        FormSetEditor->raise();
        FormSetEditor->activateWindow();
        ///FormSetEditor->setModal(true);
    }
}

void SetPopup::doCreateFromBlockData(void)
{
    if (FormEditBlockData==NULL)
    {
        FormEditBlockData=new frmEditBlockData(mainWin);
    }
    FormEditBlockData->init();
    int blockncols = get_blockncols();
    if (!blockncols)
        FormEditBlockData->hide();
    else
    {
        FormEditBlockData->show();
        FormEditBlockData->raise();
        FormEditBlockData->activateWindow();
    }
}

void SetPopup::doSelectAll(void)
{
    par->selectAll();
}

void SetPopup::doUnSelectAll(void)
{
    par->clearSelection();
}

void SetPopup::doSelectEven(void)
{
    int nr,*sel=new int[2];
    par->get_selection(&nr,&sel);
    int * ne_sel=new int[par->count()+2];
    int counter=0;
    for (int i=0;i<par->count();i++)
    {
        if (par->entries[i]%2==0)//index is even
        {
            ne_sel[counter++]=par->entries[i];
        }
        else//odd index will be inserted if it has already been selected
        {
            for (int j=0;j<nr;j++)
            {
                if (sel[j]==par->entries[i])
                {
                    ne_sel[counter++]=par->entries[i];
                    break;
                }
            }
        }
    }
    par->set_new_selection(counter,ne_sel);
    delete[] sel;
    delete[] ne_sel;
}

void SetPopup::doSelectOdd(void)
{
    int nr,*sel=new int[2];
    par->get_selection(&nr,&sel);
    int * ne_sel=new int[par->count()+2];
    int counter=0;
    for (int i=0;i<par->count();i++)
    {
        if (par->entries[i]%2!=0)//index is odd
        {
            ne_sel[counter++]=par->entries[i];
        }
        else//even index will be inserted if it has already been selected
        {
            for (int j=0;j<nr;j++)
            {
                if (sel[j]==par->entries[i])
                {
                    ne_sel[counter++]=par->entries[i];
                    break;
                }
            }
        }
    }
    par->set_new_selection(counter,ne_sel);
    delete[] sel;
    delete[] ne_sel;
}

void SetPopup::doSelectVisible(void)
{
    int nr,*sel=new int[2];
    par->get_selection(&nr,&sel);
    int * ne_sel=new int[par->count()+2];
    int counter=0;
    for (int i=0;i<par->count();i++)
    {

        if (is_set_hidden(par->gr_no,par->entries[i])==FALSE)//set is visible
        {
            ne_sel[counter++]=par->entries[i];
        }
        else//other index will be inserted if it has already been selected
        {
            for (int j=0;j<nr;j++)
            {
                if (sel[j]==par->entries[i])
                {
                    ne_sel[counter++]=par->entries[i];
                    break;
                }
            }
        }
    }
    par->set_new_selection(counter,ne_sel);
    delete[] sel;
    delete[] ne_sel;
}

void SetPopup::doSelectInVisible(void)
{
    int nr,*sel=new int[2];
    par->get_selection(&nr,&sel);
    int * ne_sel=new int[par->count()+2];
    int counter=0;
    for (int i=0;i<par->count();i++)
    {

        if (is_set_hidden(par->gr_no,par->entries[i])==TRUE)//set is visible
        {
            ne_sel[counter++]=par->entries[i];
        }
        else//other index will be inserted if it has already been selected
        {
            for (int j=0;j<nr;j++)
            {
                if (sel[j]==par->entries[i])
                {
                    ne_sel[counter++]=par->entries[i];
                    break;
                }
            }
        }
    }
    par->set_new_selection(counter,ne_sel);
    delete[] sel;
    delete[] ne_sel;
}

void SetPopup::doSelectNth(void)
{
bool ok;
int	n=QInputDialog::getInt(this,tr("Index"),tr("n="),2,0,par->number_of_entries,1,&ok,0);
if (ok==false) return;
int offset=QInputDialog::getInt(this,tr("Offset"),tr("Start index n0="),0,0,par->number_of_entries,1,&ok,0);
if (ok==false) return;
if (n==0 || n==1)
{
doSelectAll();
return;
}
    int nr,*sel=new int[2];
    par->get_selection(&nr,&sel);
    int * ne_sel=new int[par->count()+2];
    int counter=0;
    for (int i=0;i<par->count();i++)
    {
        if ((par->entries[i]+n-1-offset)%n==n-1)
        {
            ne_sel[counter++]=par->entries[i];
        }
        else//unsuitable indices are selected if they are already selected
        {
            for (int j=0;j<nr;j++)
            {
                if (sel[j]==par->entries[i])
                {
                    ne_sel[counter++]=par->entries[i];
                    break;
                }
            }
        }
    }
    par->set_new_selection(counter,ne_sel);
    delete[] sel;
    delete[] ne_sel;
}

void SetPopup::doUpdate(void)
{
    par->update_number_of_entries_preserve_selection();
}

void SetPopup::doInvertSelection(void)
{
    int number=par->count()-number_of_selected_sets;
    int * n_selected_sets=new int[number];
    int index=0;
    bool in_list;
    for (int i=0;i<par->count();i++)
    {
        in_list=false;
        for (int j=0;j<number_of_selected_sets;j++)
            if (selected_sets[j]==par->entries[i])
            {
                in_list=true;
                break;
            }
        if (in_list==false)
        {
            n_selected_sets[index]=par->entries[i];
            index++;
        }
    }
    par->set_new_selection(number,n_selected_sets);
    delete[] n_selected_sets;
}

void SetPopup::doShowHidden(void)
{
    par->show_hidden=!par->show_hidden;
    par->update_number_of_entries_preserve_selection();
}

void SetPopup::doShowDataLess(void)
{
    par->show_data_less=!par->show_data_less;
    par->update_number_of_entries_preserve_selection();
}

void SetPopup::doViewSetComments(void)
{
    par->show_comments=!par->show_comments;
    par->update_number_of_entries_preserve_selection();
}

void SetPopup::doMove12(void)
{
    if (number_of_selected_sets==2)
    {
        int gno=par->gr_no;
        int * gnos=new int[2];
        gnos[0]=gnos[1]=gno;
        char dummy[64];
        sprintf(dummy, "Replace S%d?", selected_sets[1]);
        if (yesno(dummy, NULL, NULL, NULL))
        {
            SaveSetStatesPrevious(2,gnos,selected_sets,UNDO_COMPLETE);
            moveset(gno, selected_sets[0], gno, selected_sets[1]);
            SetsModified(1,gnos,selected_sets+1,UNDO_COMPLETE);
            sprintf(dummy,"Move [G%d.S%d]-->[G%d.S%d]",gno, selected_sets[0], gno, selected_sets[1]);
            addAditionalDescriptionToLastNode(-1,QString(dummy),QString(),0);
        }
        delete[] gnos;
        finishAction(DO_PRESERVE_SELECTION);
    }
}

void SetPopup::doMove21(void)
{
    if (number_of_selected_sets==2)
    {
        int gno=par->gr_no;
        int * gnos=new int[2];
        gnos[0]=gnos[1]=gno;
        char dummy[64];
        sprintf(dummy, "Replace S%d?", selected_sets[0]);
        if (yesno(dummy, NULL, NULL, NULL))
        {
            SaveSetStatesPrevious(2,gnos,selected_sets,UNDO_COMPLETE);
            moveset(gno, selected_sets[1], gno, selected_sets[0]);
            //selected_sets[0]=selected_sets[1];
            SetsModified(1,gnos,selected_sets,UNDO_COMPLETE);
            sprintf(dummy,"Move [G%d.S%d]-->[G%d.S%d]",gno, selected_sets[1], gno, selected_sets[0]);
            addAditionalDescriptionToLastNode(-1,QString(dummy),QString(),0);
        }
        delete[] gnos;
        finishAction(DO_PRESERVE_SELECTION);
    }
}

void SetPopup::doCopy12(void)
{
    if (number_of_selected_sets==2)
    {
        int gno=par->gr_no;
        int * gnos=new int[2];
        gnos[0]=gnos[1]=gno;
        char dummy[64];
        sprintf(dummy, "Overwrite S%d?", selected_sets[1]);
        if (yesno(dummy, NULL, NULL, NULL))
        {
            SaveSetStatesPrevious(2,gnos,selected_sets,UNDO_COMPLETE);
            do_copyset(gno, selected_sets[0], gno, selected_sets[1]);
            SetsModified(2,gnos,selected_sets,UNDO_COMPLETE);
            sprintf(dummy,"Copy [G%d.S%d]-->[G%d.S%d]",gno, selected_sets[0], gno, selected_sets[1]);
            addAditionalDescriptionToLastNode(-1,QString(dummy),QString(),0);
        }
        delete[] gnos;
        finishAction(DO_PRESERVE_SELECTION);
    }
}

void SetPopup::doCopy21(void)
{
    if (number_of_selected_sets==2)
    {
        int gno=par->gr_no;
        int * gnos=new int[2];
        gnos[0]=gnos[1]=gno;
        char dummy[64];
        sprintf(dummy, "Overwrite S%d?", selected_sets[0]);
        if (yesno(dummy, NULL, NULL, NULL))
        {
            SaveSetStatesPrevious(2,gnos,selected_sets,UNDO_COMPLETE);
            do_copyset(gno, selected_sets[1], gno, selected_sets[0]);
            SetsModified(2,gnos,selected_sets,UNDO_COMPLETE);
            sprintf(dummy,"Copy [G%d.S%d]-->[G%d.S%d]",gno, selected_sets[1], gno, selected_sets[0]);
            addAditionalDescriptionToLastNode(-1,QString(dummy),QString(),0);
        }
        delete[] gnos;
        finishAction(DO_PRESERVE_SELECTION);
    }
}

void SetPopup::doCopyClipBoard(void)
{
    if (number_of_selected_sets<1) return;
    QString text;
    text.clear();
    char dummy[2048];
    int gno,sno,col;
    for (int i=0;i<number_of_selected_sets;i++)
    {
        gno=this->par->gr_no;
        sno=selected_sets[i];
        col=dataset_cols(gno,sno);
        for (int k=0;k<g[gno].p[sno].data.len;k++)
        {
            for (int j=0;j<col;j++)
            {
                //sprintf(dummy,"%.8g\t",g[gno].p[sno].data.ex[j][k]);
                //text+=QString(dummy);
                sprintf(dummy,sformat,g[gno].p[sno].data.ex[j][k]);
                text+=QString(dummy)+QString("\t");
            }
            text+=QString("\n");
        }
        text+=QString("\n");
    }
    if (SystemsDecimalPoint!='.') text.replace(QString("."),QString(SystemsDecimalPoint));
    QApplication::clipboard()->setText(text);
}

void SetPopup::doPasteClipBoard(void)
{
    GeneralPaste(QApplication::clipboard()->mimeData());
}

void SetPopup::doStoreAppearance(void)
{
//at first: deleted already saved appearances
    bool ddssm=dont_delete_saved_set_memory;
    dont_delete_saved_set_memory=true;
    for (int i=0;i<nr_of_set_app_saved;i++)
    deleteSavedSet(saved_set_app+i,UNDO_APPEARANCE);
    dont_delete_saved_set_memory=ddssm;
//find out how many set hace been selected
prepareForAction();
//prepare space for storage
    if (number_of_selected_sets>nr_of_set_app_allocated)
    {
        if (saved_set_app!=NULL) delete[] saved_set_app;
        saved_set_app=new plotarr[number_of_selected_sets];
        nr_of_set_app_allocated=number_of_selected_sets;
    }
//store the appearance
    for (int i=0;i<number_of_selected_sets;i++)
    {
    copySet(selected_graph,selected_sets[i],saved_set_app+i,UNDO_APPEARANCE);
    }
    nr_of_set_app_saved=number_of_selected_sets;
}

void reset_set_appearance_only(int gno, int sno, plotarr * pn)
{
if (is_valid_setno(gno,sno)==FALSE) return;
plotarr * po=g[gno].p+sno;
po->sym=pn->sym;
po->symsize=pn->symsize;
po->symlines=pn->symlines;
po->symlinew=pn->symlinew;
po->symskip=pn->symskip;
po->symchar=pn->symchar;
po->charfont=pn->charfont;
po->linet=pn->linet;
po->lines=pn->lines;
po->linew=pn->linew;
po->baseline_type=pn->baseline_type;
po->baseline=pn->baseline;
po->dropline=pn->dropline;
po->filltype=pn->filltype;
po->polygone_base_set=pn->polygone_base_set;
po->fillrule=pn->fillrule;
po->sympen=pn->sympen;
po->symfillpen=pn->symfillpen;
po->linepen=pn->linepen;
po->setfillpen=pn->setfillpen;
memcpy(&(po->avalue),&(pn->avalue),sizeof(AValue));
memcpy(&(po->errbar),&(pn->errbar),sizeof(Errbar));
}

void SetPopup::doApplyStoredAppearance(void)
{
    prepareForAction();
if (nr_of_set_app_saved<=0 || number_of_selected_sets<=0) return;

int * selected_graphs=new int[number_of_selected_sets+1];
for (int i=0;i<number_of_selected_sets;i++) selected_graphs[i]=selected_graph;
SaveSetStatesPrevious(number_of_selected_sets,selected_graphs,selected_sets,UNDO_APPEARANCE);
    /*if (nr_of_set_app_saved==1)//one appearance save, many sets selected
    {
        for (int i=0;i<number_of_selected_sets;i++)
        {
        reset_set_appearance_only(selected_graph,selected_sets[i],saved_set_app[0]);
        }
    }
    else
    {*/
int counter=0;
for (int i=0;i<number_of_selected_sets;i++)
{
reset_set_appearance_only(selected_graph,selected_sets[i],saved_set_app+counter);
counter++;
if (counter>=nr_of_set_app_saved) counter=0;
}
    //}
ListOfChanges.clear();
ListOfOldStates.clear();
SetsModified(number_of_selected_sets,selected_graphs,selected_sets,UNDO_APPEARANCE);
delete[] selected_graphs;
mainWin->mainArea->completeRedraw();

}

void SetPopup::update_menu_content(void)
{
    char dummy[128];
    char dummy2[128];
    actViewSetComments->setChecked(par->show_comments);
    actShowDataLess->setChecked(par->show_data_less);
    actShowHidden->setChecked(par->show_hidden);

    if (number_of_selected_sets==0)
    {
        actLabel->setText(tr("None"));
        actHide->setEnabled(false);
        actBringToFront->setEnabled(false);
        actSendToBack->setEnabled(false);
        actPackAllSets->setEnabled(false);
        actShow->setEnabled(false);
        actDuplicate->setEnabled(false);
        actKill->setEnabled(false);
        actKillData->setEnabled(false);
        actSwap->setEnabled(false);
        actEditInSpreadsheet->setEnabled(false);
        actEditInTextEditor->setEnabled(false);
        actCopy12->setVisible(false);
        actCopy21->setVisible(false);
        actMove12->setVisible(false);
        actMove21->setVisible(false);
        actCopyClipBoard->setEnabled(false);
        /*actions()[2]->setEnabled(FALSE);
    actions()[3]->setEnabled(FALSE);
    actions()[4]->setEnabled(FALSE);
    actions()[5]->setEnabled(FALSE);
    actions()[7]->setEnabled(FALSE);
    actions()[8]->setEnabled(FALSE);
    actions()[9]->setEnabled(FALSE);
    actions()[11]->setEnabled(FALSE);
    mnuEdit->actions()[0]->setEnabled(FALSE);
    mnuEdit->actions()[1]->setEnabled(FALSE);*/
        actStoreAppearance->setEnabled(false);
        actApplyStoredAppearance->setEnabled(false);
    }
    else
    {
        actHide->setEnabled(true);
        actBringToFront->setEnabled(true);
        actSendToBack->setEnabled(true);
        actPackAllSets->setEnabled(true);
        actShow->setEnabled(true);
        actDuplicate->setEnabled(true);
        actKill->setEnabled(true);
        actKillData->setEnabled(true);
        actSwap->setEnabled(true);
        actEditInSpreadsheet->setEnabled(true);
        actEditInTextEditor->setEnabled(true);
        actCopyClipBoard->setEnabled(true);
        /*actions()[2]->setEnabled(TRUE);
    actions()[3]->setEnabled(TRUE);
    actions()[4]->setEnabled(TRUE);
    actions()[5]->setEnabled(TRUE);
    actions()[7]->setEnabled(TRUE);
    actions()[8]->setEnabled(TRUE);
    actions()[9]->setEnabled(TRUE);
    actions()[11]->setEnabled(TRUE);
    mnuEdit->actions()[0]->setEnabled(TRUE);
    mnuEdit->actions()[1]->setEnabled(TRUE);*/
        actStoreAppearance->setEnabled(true);
            if (nr_of_set_app_saved>0)
            actApplyStoredAppearance->setEnabled(true);
            else
            actApplyStoredAppearance->setEnabled(false);
        sprintf(dummy,"S");
        for (int i=0;i<number_of_selected_sets;i++)
        {
            sprintf(dummy2,"%d",selected_sets[i]);
            strcat(dummy,dummy2);
            if (i>2 && i!=number_of_selected_sets-1)
            {
                strcat(dummy,",...");
                break;
            }
            else
            {
                if (i!=number_of_selected_sets-1)
                    strcat(dummy,", S");
            }
        }
        actLabel->setText(QString(dummy));

        if (number_of_selected_sets==1)
        {
            actCopy12->setVisible(false);
            actCopy21->setVisible(false);
            actMove12->setVisible(false);
            actMove21->setVisible(false);
            actSwap->setEnabled(false);
            actBringToFront->setEnabled(true);
            actSendToBack->setEnabled(true);
            actEditInSpreadsheet->setEnabled(true);
            actEditInTextEditor->setEnabled(true);
        }
        else
        {
            if (number_of_selected_sets==2)
            {
                sprintf(dummy,"Copy S%d to S%d",selected_sets[0],selected_sets[1]);
                actCopy12->setText(QString(dummy));
                actCopy12->setVisible(true);
                sprintf(dummy,"Copy S%d to S%d",selected_sets[1],selected_sets[0]);
                actCopy21->setText(QString(dummy));
                actCopy21->setVisible(true);
                sprintf(dummy,"Move S%d to S%d",selected_sets[0],selected_sets[1]);
                actMove12->setText(QString(dummy));
                actMove12->setVisible(true);
                sprintf(dummy,"Move S%d to S%d",selected_sets[1],selected_sets[0]);
                actMove21->setText(QString(dummy));
                actMove21->setVisible(true);
                actSwap->setEnabled(true);
            }
            else
            {
                actCopy12->setVisible(false);
                actCopy21->setVisible(false);
                actMove12->setVisible(false);
                actMove21->setVisible(false);
                actSwap->setEnabled(false);
            }
            actBringToFront->setEnabled(false);
            actSendToBack->setEnabled(false);
            actEditInSpreadsheet->setEnabled(false);
            actEditInTextEditor->setEnabled(false);
        }
    }
    if (QApplication::clipboard()->mimeData()->hasText() || QApplication::clipboard()->mimeData()->hasUrls())
        actPasteClipBoard->setEnabled(true);
    else
        actPasteClipBoard->setEnabled(false);
}

uniList::uniList(int type,QWidget *parent):QListWidget(parent)
{
    minimum_display=false;
    prevent_from_autoupdate=false;
    datType=type;
    partner=NULL;
    show_all_sets_marker=false;
    all_entries_option_selected=false;
    //set the standard-behavior (can be changed especially for some widgets)
    if (type==GRAPHLIST)
    {
        setBehavior(true,false,false);
    }
    else if (type==SETCHOICE)
    {
        setBehavior(false,true,true);
    }
    else//SetList
    {
        setBehavior(false,true,false);
    }
    connect(this,SIGNAL(itemDoubleClicked(QListWidgetItem *)),SLOT(entryDoubleClicked(QListWidgetItem *)));
    connect(this,SIGNAL(itemSelectionChanged()),SLOT(new_selection()));

    popupMenu1=new SetPopup(this);
    popupMenu1->hide();
    popupMenu2=new GraphPopup(this);
    popupMenu2->hide();

    actCopyClipBoard=new QAction(tr("Copy"),this);
    actCopyClipBoard->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    actPasteClipBoard=new QAction(tr("Paste"),this);
    actPasteClipBoard->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
    if (datType==SETLIST)
    {
        this->addAction(actCopyClipBoard);
        this->addAction(actPasteClipBoard);
        connect(actCopyClipBoard,SIGNAL(triggered()),SLOT(doCopyClipBoard()));
        connect(actPasteClipBoard,SIGNAL(triggered()),SLOT(doPasteClipBoard()));
    }

    number_of_entries=0;
    gr_no=0;
    entries=new int[2];
    text_entries=new QString[2];
    show_hidden=true;
    show_data_less=false;
    show_comments=false; //not changed in v0.2.5

    if (type==GRAPHLIST)//register this List so that it may be updated later (live)
    {
        add_GraphSelector(this);
    }
    else if (type==SETCHOICE)
    {
        add_SetChoiceItem(this);
    }
    else
    {
        add_SetSelector(this);
    }
}

uniList::~uniList()
{
    delete[] entries;
    delete[] text_entries;
}

void uniList::update_number_of_entries(void)
{
    char dummy[2048];
    char hidden_char;
    int cols;
    int index=0;
    QString itemtext;
    disconnect(this,SIGNAL(itemSelectionChanged()),this,SLOT(new_selection()));
    clear();
    if (datType==GRAPHLIST)
    {
        delete[] entries;
        entries=new int[number_of_graphs()>1?number_of_graphs()+1:2+1];
        if (show_all_sets_marker==true)
        {
            if (minimum_display==false)
                itemtext=tr("All graphs");
            else
                itemtext=tr("All");
            new QListWidgetItem(itemtext, this);
            entries[index]=-1;
            index++;
        }
        for (int i=0;i<number_of_graphs();i++)
        {
            if (g[i].hidden!=0 && show_hidden==false) continue;//don't show hidden graphs
            hidden_char=is_graph_hidden(i)?'-':'+';
            ///sprintf(dummy,"(%c) G%d (%d sets)",hidden_char,i,number_of_sets(i));
            if (minimum_display==false)
                sprintf(dummy,"(%c) G%d (%d sets)",hidden_char,i,number_of_real_sets(i));
            else
                sprintf(dummy,"(%c) G%d",hidden_char,i);
            new QListWidgetItem(QString(dummy), this);
            entries[index]=i;
            index++;
        }
    }
    else if (datType==TEXTLIST)
    {
        index=number_of_entries;//This should by all that is needed.
    }
    else//SETLIST or SETCHOICE
    {
        if (gr_no<0 || gr_no>=number_of_graphs()) return;
        delete[] entries;
        entries=new int[g[gr_no].maxplot>1?g[gr_no].maxplot+1:2+1];
        if (show_all_sets_marker==true)
        {
            if (minimum_display==false)
                itemtext=tr("All sets");
            else
                itemtext=tr("All");
            new QListWidgetItem(itemtext, this);
            entries[index]=-1;
            index++;
        }
        for (int i=0;i<g[gr_no].maxplot;i++)
        {
            if (!((show_data_less == true || is_set_active(gr_no, i) == TRUE) && (show_hidden == true || is_set_hidden(gr_no, i) != TRUE ))) continue;
            ///if ((g[gr_no].p[i].hidden!=0 && show_hidden==false) || g[gr_no].p[i].data.len<=0) continue;//don't show hidden sets
            hidden_char=g[gr_no].p[i].hidden==0?'+':'-';
            cols=settype_cols(g[gr_no].p[i].type);
            if (datType==SETLIST)
            {
                if (minimum_display==false)
                    sprintf(dummy,"(%c) G%d.S%d[%d][%d]",hidden_char,gr_no,i,cols,g[gr_no].p[i].data.len);
                else
                    sprintf(dummy,"(%c) S%d",hidden_char,i);
                if (show_comments)
                {
                    strcat(dummy," \"");
                    strcat(dummy,g[gr_no].p[i].comments);
                    strcat(dummy,"\"");
                }
            }
            else
            {
                if (minimum_display==false)
                    sprintf(dummy, "S%d (N=%d, %s)", i, getsetlength(gr_no, i), getcomment(gr_no, i));
                else
                    sprintf(dummy, "S%d", i);
            }
            new QListWidgetItem(dummy, this);
            entries[index]=i;
            index++;
        }
    }
    number_of_entries=index;
    connect(this,SIGNAL(itemSelectionChanged()),this,SLOT(new_selection()));
}

void uniList::update_number_of_entries_preserve_selection(void)
{
    int ns,*sel=new int[2];
    disconnect(this,SIGNAL(itemSelectionChanged()),this,SLOT(new_selection()));
    get_selection(&ns,&sel);
    /*cout << "selection:" << endl;
    for (int j=0;j<ns;j++)
    cout << j << ": " << sel[j] << endl;*/
    /*
if (0)//partner!=NULL)//this part did not work as desired --> I think this could be deleted
{
disconnect(partner,SIGNAL(itemSelectionChanged()),partner,SLOT(new_selection()));
    int nsp,*selp=new int[2];
    partner->get_selection(&nsp,&selp);
    cout << "selection:" << endl;
    for (int j=0;j<ns;j++)
    cout << j << ": " << sel[j] << endl;
    cout << "partner_selection:" << endl;
    for (int j=0;j<nsp;j++)
    cout << j << ": " << selp[j] << endl;
   if (datType==GRAPHLIST)//nothing to be done here, everything is done, when set is updated
   {//partner is a set-list
        cout << "t1" << endl;
        update_number_of_entries();
        cout << "t2" << endl;
        set_new_selection(ns,sel);
        cout << "t3" << endl;
        partner->update_number_of_entries();
        cout << "t4" << endl;
        partner->set_new_selection(nsp,selp);
        cout << "t5" << endl;
        //cout << "test Graphlist-update" << endl;//update_number_of_entries();
   }
   else
   {
   cout << "test Setlist-update -- doing nothing!" << endl;
   }
   delete[] selp;
connect(partner,SIGNAL(itemSelectionChanged()),partner,SLOT(new_selection()));
}
else
{
*/
    if (datType==TEXTLIST)
    {
        clear();
        for (int i=0;i<number_of_entries;i++)
            new QListWidgetItem(QString(text_entries[i]), this);
    }
    else
    {
        update_number_of_entries();
    }
    set_new_selection(ns,sel);
    new_selection();
    //}
    delete[] sel;
    connect(this,SIGNAL(itemSelectionChanged()),this,SLOT(new_selection()));
}

void uniList::set_graph_number(int gno,bool keep_selection)
{
    gr_no=gno;
    int s_nr,*sel=new int[2];
    get_selection(&s_nr,&sel);
    update_number_of_entries();
    if (datType==GRAPHLIST)
        item(gno)->setSelected(TRUE);
    else if (keep_selection==true)
        set_new_selection(s_nr,sel);
    delete[] sel;
}

void uniList::get_selection(int * number,int ** selection)
{
    QModelIndexList list=selectedIndexes();
    *number=list.size();
    if (*number<=0) return;//return without deleting anything
        if (*selection!=NULL)
        delete[] *selection;
    if (entries[list.at(0).row()]<0)//all_entries
    {
        all_entries_option_selected=true;
        *number=number_of_entries-1;
        *selection=new int[*number];
        for (int i=0;i<*number;i++)
            *(*selection+i)=entries[i+1];
    }
    else
    {
        all_entries_option_selected=false;
        *selection=new int[*number];
        for (int i=0;i<*number;i++)
            *(*selection+i)=entries[list.at(i).row()];
    }
    sort(*number,*selection);
}

void uniList::set_new_selection(int number,int * selection)
{
    disconnect(this,SIGNAL(itemSelectionChanged()),this,SLOT(new_selection()));
    int scroll=-1;
    if (number<0) goto end_set_new_selection;
    clearSelection();
    if (number==0) goto end_set_new_selection;
    for (int i=0;i<number;i++)
    {
        //if (selection[i]>count()) continue;//this is useless because the count does not say what numbers are present!
        for (int j=0;j<count();j++)
        {
            if (selection[i]==entries[j])
            {
                item(j)->setSelected(TRUE);
                if (scroll==-1) scroll=j;
            }
        }
    }
    if (scroll!=-1)
        scrollToItem(item(scroll));
end_set_new_selection:
    connect(this,SIGNAL(itemSelectionChanged()),this,SLOT(new_selection()));
}

void uniList::new_selection(void)
{
    QModelIndexList list=selectedIndexes();
    if (list.size()>0)
    {
        emit(new_selection(list.at(0).row()));
    }
    else
    {
        emit(new_selection(-1));
    }
}

void uniList::mousePressEvent(QMouseEvent * e)
{
    Qt::KeyboardModifiers modif=e->modifiers();
    QCursor curs=mainWin->cursor();
    QCursor curs2(Qt::WhatsThisCursor);
    QCursor curs3=cursor();
    QCursor curs4=parentWidget()->cursor();
    if (curs.shape()==curs2.shape() || curs3.shape()==curs2.shape() || curs4.shape()==curs2.shape())
    {
        if (datType==SETLIST)
        {
            HelpCB("doc/UsersGuide.html#set-selector");
        }
        else if (datType==GRAPHLIST)
        {
            HelpCB("doc/UsersGuide.html#graph-selector");
        }
        else
        {
            HelpCB("doc/UsersGuide.html#list-selector");
        }
        unsetCursor();
        return;
    }
    QPoint p=e->pos();
    QModelIndexList list=selectedIndexes();
    int number_of_selected_entries=list.size();
    QListWidgetItem * clickedItem=itemAt(p);
    int selected_no=row(clickedItem);

    bool already_selected=false;
    if (clickedItem!=0)
        already_selected=item(selected_no)->isSelected();

    if (e->button() & Qt::RightButton)
    {
        if (datType==SETLIST)
        {
            popupMenu1->selected_graph=gr_no;
            get_selection(&popupMenu1->number_of_selected_sets,&popupMenu1->selected_sets);
            sort(popupMenu1->number_of_selected_sets,popupMenu1->selected_sets);
            popupMenu1->update_menu_content();
            popupMenu1->exec(QCursor::pos());
        }
        else if (datType==GRAPHLIST)
        {
            get_selection(&popupMenu2->number_of_selected_graphs,&popupMenu2->selected_graphs);
            sort(popupMenu2->number_of_selected_graphs,popupMenu2->selected_graphs);
            popupMenu2->selected_no=selected_no;
            popupMenu2->update_menu_content();
            popupMenu2->exec(QCursor::pos());
            QListWidget::mousePressEvent(e);
        }
    }
    else//Left Button clicked
    {
        if (clickedItem==0)
        {
            return;//nothing to do if someone clicked outside the entries
        }
        if (number_of_selected_entries>1 && ((int)(modif&Qt::ControlModifier))==0)
        {
            clearSelection();
            item(selected_no)->setSelected(true);
        }
        else if (deselect_possible==true && already_selected==true)
        {
            item(selected_no)->setSelected(false);
        }
        else
        {
            QListWidget::mousePressEvent(e);
        }
    }
}

void uniList::mouseReleaseEvent(QMouseEvent * e )
{
    QPoint p=e->pos();
    //QModelIndexList list=selectedIndexes();
    //int number_of_selected_entries=list.size();
    QListWidgetItem * clickedItem=itemAt(p);
    //int selected_no=row(clickedItem);
    if (clickedItem!=0)
        QListWidget::mouseReleaseEvent(e);
}

void uniList::keyReleaseEvent(QKeyEvent * e)
{
    if (datType==GRAPHLIST || datType==SETLIST)
    {
        int k=e->key();
        Qt::KeyboardModifiers mod=e->modifiers();
        if (mod==Qt::ControlModifier)
        {
            if (k==Qt::Key_I)
            {
                popupMenu1->selected_graph=gr_no;
                get_selection(&popupMenu1->number_of_selected_sets,&popupMenu1->selected_sets);
                sort(popupMenu1->number_of_selected_sets,popupMenu1->selected_sets);
                popupMenu1->doInvertSelection();
            }
            else if(k==Qt::Key_U)
                clearSelection();
        }
    }
}

void uniList::entryDoubleClicked(QListWidgetItem * c_item)
{
    if (datType==SETLIST)
    {
        int cl_index=-1;
        for (int i=0;i<number_of_entries;i++)
        {
            if (item(i)==c_item)
            {
                cl_index=i;
                break;
            }
        }
        if (cl_index>=0)
            showSetInSpreadSheet(gr_no,entries[cl_index]);
    }
    else if (datType==GRAPHLIST)
    {
        int nr_sel,*sel=new int[2];
        get_selection(&nr_sel,&sel);
        int cg = get_cg();
        if (sel[0] != cg) {
            switch_current_graph(sel[0]);
            mainWin->mainArea->completeRedraw();
        }
        delete[] sel;
    }
}

void uniList::add_Item(QString item)
{
    QString * str=new QString[number_of_entries+1];
    int * tentr=new int[number_of_entries];
    for (int i=0;i<number_of_entries;i++)
    {
        str[i]=text_entries[i];
        tentr[i]=entries[i];
    }
    str[number_of_entries]=item;
    tentr[number_of_entries]=number_of_entries;
    delete[] text_entries;
    delete[] entries;
    text_entries=str;
    entries=tentr;
    number_of_entries++;
    update_number_of_entries_preserve_selection();
}

void uniList::get_selection(int * number,QString ** selection)
{
    QModelIndexList list=selectedIndexes();
    *number=list.size();
    if (*number<=0) return;
    delete[] *selection;
    *selection=new QString[*number];
    for (int i=0;i<*number;i++)
        *(*selection+i)=text_entries[list.at(i).row()];
    //sort(*number,*selection);
}

void uniList::setBehavior(bool deselect,bool multi,bool all_sets)
{
    deselect_possible=deselect;
    show_all_sets_marker=all_sets;
    if (multi==true)
    {
        setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
    else
    {
        setSelectionMode(QAbstractItemView::SingleSelection);
    }
}

void uniList::doCopyClipBoard(void)
{
    if (datType!=SETLIST || selectedIndexes().size()<=0) return;//nothing suitable selected
    popupMenu1->selected_graph=gr_no;
    get_selection(&popupMenu1->number_of_selected_sets,&popupMenu1->selected_sets);
    sort(popupMenu1->number_of_selected_sets,popupMenu1->selected_sets);
    popupMenu1->update_menu_content();
    popupMenu1->doCopyClipBoard();
}

void uniList::doPasteClipBoard(void)
{
    if (QApplication::clipboard()->mimeData()->hasUrls() || QApplication::clipboard()->mimeData()->hasText())
    {
        GeneralPaste(QApplication::clipboard()->mimeData());
    }
}

TreePopup::TreePopup(treeView *parent):QMenu(parent)
{
    par2=NULL;
    par=parent;
    selected_type='0';
    gno=sno=-1;
    setTearOffEnabled(FALSE);
    CreateActions();
    //Create Menu itself
    addAction(actShow);
    addAction(actHide);
    addAction(actKill);
    addAction(actDuplicate);
}

TreePopup::~TreePopup()
{

}

void TreePopup::CreateActions(void)
{
    actShow=new QAction(tr("Show"),this);
    connect(actShow,SIGNAL(triggered()),SLOT(doShow()));
    actHide=new QAction(tr("Hide"),this);
    connect(actHide,SIGNAL(triggered()),SLOT(doHide()));
    actKill=new QAction(tr("Kill"),this);
    connect(actKill,SIGNAL(triggered()),SLOT(doKill()));
    actDuplicate=new QAction(tr("Duplicate"),this);
    connect(actDuplicate,SIGNAL(triggered()),SLOT(doDuplicate()));
}

void TreePopup::doNothing(void)
{}

void TreePopup::doShow(void)
{
    doAction(1);
}

void TreePopup::doHide(void)
{
    doAction(2);
}

void TreePopup::doKill(void)
{
    doAction(3);
}

void TreePopup::doDuplicate(void)
{
    doAction(4);
}

void TreePopup::doAction(int type)//type is the kind of action
{
    int obj_type;
    bool d1;
    tickmarks * t2;
    char descr_axis[512];
    int undoObjIds[2];
    int undoObjIds2[2];
    legend l2;
    ListOfChanges.clear();
    ListOfOldStates.clear();
    switch ((int)selected_type)
    {
    default:
    case (int)'0'://nothing
        ;
        break;
    case (int)'N'://String-Object
    case (int)'I'://Line-Object
    case (int)'B'://Box-Object
    case (int)'P'://Ellipse-Object
        if (selected_type=='N')
            obj_type=OBJECT_STRING;
        else if (selected_type=='I')
            obj_type=OBJECT_LINE;
        else if (selected_type=='B')
            obj_type=OBJECT_BOX;
        else//'P'
            obj_type=OBJECT_ELLIPSE;
        undoObjIds[0]=gno;
        if (type==1)//show
        {
            SaveObjectData(undoObjIds[0],obj_type);
            set_object_active(obj_type,undoObjIds[0],TRUE);
            ObjectDataModified(undoObjIds[0],obj_type);
        }
        else if (type==2)//hide
        {
            SaveObjectData(undoObjIds[0],obj_type);
            set_object_active(obj_type,undoObjIds[0],FALSE);
            ObjectDataModified(undoObjIds[0],obj_type);
        }
        else if (type==3)//kill
        {
            ObjectsDeleted(1,undoObjIds,obj_type);
            kill_object(obj_type,undoObjIds[0]);
        }
        else if (type==4)//duplicate
        {
            undoObjIds[1]=duplicate_object(obj_type,undoObjIds[0]);
            ObjectsCreated(1,undoObjIds+1,obj_type);
        }
        break;
    case (int)'E'://Legend
        if (type==1)//show
            d1=true;
        else//hide
            d1=false;
        get_graph_legend(gno,&l2);
        undoObjIds[0]=gno;
        SaveGraphStatesPrevious(1,undoObjIds,UNDO_APPEARANCE);
        sprintf(dummy,"with g%d",gno);
        ListOfChanges << QString(dummy);
        ListOfOldStates << QString(dummy);
        sprintf(dummy,"    legend %s",d1?"on":"off");
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    legend %s",l2.active?"on":"off");
        ListOfOldStates << QString(dummy);
        l2.active=(int)d1;
        set_graph_legend(gno, &l2);
        GraphsModified(1,undoObjIds,UNDO_APPEARANCE);
        addAditionalDescriptionToLastNode(-1,tr("Graph legend visibility changed"),QString(),-1);
        break;
    case (int)'S'://Set
        undoObjIds[0]=gno;
        undoObjIds2[0]=sno;
        if (type==1)//show
        {
            set_set_hidden(gno, sno, FALSE);
            ShowHideSets(1,undoObjIds,undoObjIds2);
        }
        else if (type==2)//hide
        {
            set_set_hidden(gno, sno, TRUE);
            ShowHideSets(1,undoObjIds,undoObjIds2);
        }
        else if (type==3)//kill
        {
            SetsDeleted(1,undoObjIds,undoObjIds2,UNDO_COMPLETE);
            killset(gno,sno);
        }
        else if (type==4)//duplicate
        {
            undoObjIds2[0]=nextset(gno);
            do_copyset(gno, sno, gno, undoObjIds2[0]);
            SetsCreated(1,undoObjIds,undoObjIds2,UNDO_COMPLETE);
        }
        break;
    case (int)'A'://Axis
        if (type==1)//show
            d1=true;
        else//hide
            d1=false;
        t2 = get_graph_tickmarks(gno,sno);
        switch (sno)
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
        sprintf(dummy,"with g%d",gno);
        ListOfChanges << QString(dummy);
        ListOfOldStates << QString(dummy);
        if (int(d1)!=t2->active)
        {
            sprintf(dummy,"    %s %s",descr_axis,d1?"on":"off");
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    %s %s",descr_axis,t2->active?"on":"off");
            ListOfOldStates << QString(dummy);
        }
        t2->active=(int)d1;
        break;
    case (int)'R'://General Settings
    case (int)'L'://Label/Title
        ;//no popup here --> this is not used at all
        break;
    case (int)'G'://Graph
        undoObjIds[0]=gno;
        undoObjIds2[0]=sno;
        if (type==1)//show
        {
            sprintf(dummy,"g%d hidden false",gno);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"g%d hidden %s",gno,is_graph_hidden(gno)?"true":"false");
            ListOfOldStates << QString(dummy);
            set_graph_hidden(gno, FALSE);
            ShowHideGraphs(1,undoObjIds);
        }
        else if (type==2)//hide
        {
            sprintf(dummy,"g%d hidden true",gno);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"g%d hidden %s",gno,is_graph_hidden(gno)?"true":"false");
            ListOfOldStates << QString(dummy);
            set_graph_hidden(gno, TRUE);
            ShowHideGraphs(1,undoObjIds);
        }
        else if (type==3)//kill
        {
            GraphsDeleted(1,undoObjIds,UNDO_COMPLETE);
            sprintf(dummy,"kill G%d",gno);
            ListOfChanges << QString(dummy);
            kill_graph(gno);
        }
        else if (type==4)//duplicate
        {
            new_set_no=-1;
            duplicate_graph(gno);
            //I don't know the command for duplicating graphs
            if (new_set_no!=-1)
            {
                undoObjIds[0]=new_set_no;
                GraphsCreated(1,undoObjIds,UNDO_COMPLETE);
            }
        }
        break;
    }
    ListOfChanges.clear();
    ListOfOldStates.clear();
    mainWin->mainArea->completeRedraw();
    QTreeWidgetItem * ite=par->findItem(selected_type,gno,sno);
    par->itemClickedAt(ite,0);
}

void TreePopup::updateEntries(char ty,int g,int s)
{
    selected_type=ty;
    gno=g;
    sno=s;
    possible=true;
    actShow->setVisible(true);
    actHide->setVisible(true);
    actKill->setVisible(true);
    actDuplicate->setVisible(true);
    switch (int(ty))
    {
    case (int)'0':
    case (int)'R'://General
    case (int)'L'://Label/Title
        possible=false;
        break;
    case (int)'A'://Axis
    case (int)'E'://Legend
        //only show and hide
        actKill->setVisible(false);
        actDuplicate->setVisible(false);
        break;
    default:
    case (int)'N'://String-Object
    case (int)'I'://Line-Object
    case (int)'B'://Box-Object
    case (int)'P'://Ellipse-Object
    case (int)'G'://Graph
    case (int)'S'://Set
        //everything possible
        break;
    }
}

treeView::treeView(QWidget * parent):QTreeWidget(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setColumnCount(1);
    setHeaderLabel(tr("Project..."));
    popup=new TreePopup(this);
    popup->hide();

    connect(this,SIGNAL(itemClicked(QTreeWidgetItem*,int)),SLOT(itemClickedAt(QTreeWidgetItem*,int)));
}

void treeView::NameProject(void)
{

}

void treeView::AddMajorChild(QTreeWidgetItem * parent,QTreeWidgetItem * item)
{

}

void treeView::AddMinorChild(QTreeWidgetItem * parent,QTreeWidgetItem * item)
{

}

void treeView::ClearAll(void)
{
    this->clear();
}

void treeView::RecreateCompleteTree(void)
{
    ClearAll();
    char dummy[2048];
    QString dstr;
    QTreeWidgetItem * twi;
    QTreeWidgetItem * twi2;
    //general
    twi=new QTreeWidgetItem(this);
    twi->setText(0,tr("General Settings"));
    twi->setData(0,TREE_ROLE_GRAPH,QVariant(-1));
    twi->setData(0,TREE_ROLE_SET,QVariant(-1));
    twi->setData(0,TREE_ROLE_TYPE,QVariant('R'));
    twi->setIcon(0,QIcon(*ActiveIcon));
    addTopLevelItem(twi);
    //graphs
    for (int i=0;i<number_of_graphs();i++)
    {
        sprintf(dummy,"Graph G%d, type=%s",i,graph_types(g[i].type));
        twi=new QTreeWidgetItem(this);
        twi->setText(0,QString(dummy));
        twi->setData(0,TREE_ROLE_GRAPH,QVariant(i));
        twi->setData(0,TREE_ROLE_SET,QVariant(-1));
        twi->setData(0,TREE_ROLE_TYPE,QVariant('G'));
        if (g[i].hidden==true)
            twi->setIcon(0,QIcon(*HiddenIcon));
        else
            twi->setIcon(0,QIcon(*ActiveIcon));
        addTopLevelItem(twi);
        //labels
        twi2=new QTreeWidgetItem(twi);
        twi2->setText(0,tr("Title"));
        twi2->setData(0,TREE_ROLE_GRAPH,QVariant(i));
        twi2->setData(0,TREE_ROLE_SET,QVariant(0));
        twi2->setData(0,TREE_ROLE_TYPE,QVariant('L'));
        twi2->setIcon(0,QIcon(*ActiveIcon));
        twi->addChild(twi2);
        twi2=new QTreeWidgetItem(twi);
        twi2->setText(0,tr("Subtitle"));
        twi2->setData(0,TREE_ROLE_GRAPH,QVariant(i));
        twi2->setData(0,TREE_ROLE_SET,QVariant(1));
        twi2->setData(0,TREE_ROLE_TYPE,QVariant('L'));
        twi2->setIcon(0,QIcon(*ActiveIcon));
        twi->addChild(twi2);
        //axis
        for (int j=0;j<MAXAXES;j++)
        {
            if (g[i].t[j] == NULL) continue;
            twi2=new QTreeWidgetItem(twi);
            switch (j)
            {
            case 0:
                dstr=tr("X axis");
                break;
            case 1:
                dstr=tr("Y axis");
                break;
            case 2:
                dstr=tr("Alt X axis");
                break;
            case 3:
                dstr=tr("Alt Y axis");
                break;
            }
            twi2->setText(0,dstr);
            twi2->setData(0,TREE_ROLE_GRAPH,QVariant(i));
            twi2->setData(0,TREE_ROLE_SET,QVariant(j));
            twi2->setData(0,TREE_ROLE_TYPE,QVariant('A'));
            if (g[i].t[j]->active==false)
                twi2->setIcon(0,QIcon(*HiddenIcon));
            else
                twi2->setIcon(0,QIcon(*ActiveIcon));
            twi->addChild(twi2);
        }
        //all sets
        for (int j=0;j<number_of_sets(i);j++)
        {
            if (is_set_active(i, j) == FALSE) continue;
            sprintf(dummy,"Set S%d, type=%s",j,set_types(g[i].p[j].type));
            twi2=new QTreeWidgetItem(twi);
            twi2->setText(0,QString(dummy));
            twi2->setData(0,TREE_ROLE_GRAPH,QVariant(i));
            twi2->setData(0,TREE_ROLE_SET,QVariant(j));
            twi2->setData(0,TREE_ROLE_TYPE,QVariant('S'));
            if (g[i].p[j].hidden==true)
                twi2->setIcon(0,QIcon(*HiddenIcon));
            else
                twi2->setIcon(0,QIcon(*ActiveIcon));
            twi->addChild(twi2);
        }
        //legend
        twi2=new QTreeWidgetItem(twi);
        twi2->setText(0,tr("Legend"));
        twi2->setData(0,TREE_ROLE_GRAPH,QVariant(i));
        twi2->setData(0,TREE_ROLE_SET,QVariant(-1));
        twi2->setData(0,TREE_ROLE_TYPE,QVariant('E'));
        if (g[i].l.active!=true)
            twi2->setIcon(0,QIcon(*HiddenIcon));
        else
            twi2->setIcon(0,QIcon(*ActiveIcon));
        twi->addChild(twi2);
    }//end graphs
    //strings
    for (int i = 0; i < maxstr; i++)
    {
        if (!isactive_string(i)) continue;
        twi=new QTreeWidgetItem(this);
        sprintf(dummy," \"%s\"",pstr[i].alt_plotstring);
        twi->setText(0,tr("String-Object")+QString(dummy));
        twi->setData(0,TREE_ROLE_GRAPH,QVariant(i));
        twi->setData(0,TREE_ROLE_SET,QVariant(-1));
        twi->setData(0,TREE_ROLE_TYPE,QVariant('N'));
        if (pstr[i].active==TRUE)
            twi->setIcon(0,QIcon(*ActiveIcon));
        else
            twi->setIcon(0,QIcon(*HiddenIcon));
        addTopLevelItem(twi);
    }
    //lines
    for (int i = 0; i < maxlines; i++)
    {
        if (!isactive_line(i)) continue;
        twi=new QTreeWidgetItem(this);
        twi->setText(0,tr("Line-Object"));
        twi->setData(0,TREE_ROLE_GRAPH,QVariant(i));
        twi->setData(0,TREE_ROLE_SET,QVariant(-1));
        twi->setData(0,TREE_ROLE_TYPE,QVariant('I'));
        if (lines[i].active==TRUE)
            twi->setIcon(0,QIcon(*ActiveIcon));
        else
            twi->setIcon(0,QIcon(*HiddenIcon));
        addTopLevelItem(twi);
    }
    //boxes
    for (int i = 0; i < maxboxes; i++)
    {
        if (!isactive_box(i)) continue;
        twi=new QTreeWidgetItem(this);
        twi->setText(0,tr("Box-Object"));
        twi->setData(0,TREE_ROLE_GRAPH,QVariant(i));
        twi->setData(0,TREE_ROLE_SET,QVariant(-1));
        twi->setData(0,TREE_ROLE_TYPE,QVariant('B'));
        if (boxes[i].active==TRUE)
            twi->setIcon(0,QIcon(*ActiveIcon));
        else
            twi->setIcon(0,QIcon(*HiddenIcon));
        addTopLevelItem(twi);
    }
    //ellipses
    for (int i = 0; i < maxellipses; i++)
    {
        if (!isactive_ellipse(i)) continue;
        twi=new QTreeWidgetItem(this);
        twi->setText(0,tr("Ellipse-Object"));
        twi->setData(0,TREE_ROLE_GRAPH,QVariant(i));
        twi->setData(0,TREE_ROLE_SET,QVariant(-1));
        twi->setData(0,TREE_ROLE_TYPE,QVariant('P'));
        if (ellip[i].active==TRUE)
            twi->setIcon(0,QIcon(*ActiveIcon));
        else
            twi->setIcon(0,QIcon(*HiddenIcon));
        addTopLevelItem(twi);
    }
    expandAll();
}

void treeView::getItemData(QTreeWidgetItem * item,char & type,int & gno,int & sno)
{
    type=item->data(0,TREE_ROLE_TYPE).toChar().toLatin1();
    gno=item->data(0,TREE_ROLE_GRAPH).toInt();
    sno=item->data(0,TREE_ROLE_SET).toInt();
}

QTreeWidgetItem * treeView::findItem(char type,int gno,int sno)
{
    char t;
    int g,s;
    QTreeWidgetItem * item=NULL;
    QTreeWidgetItem * item2=NULL;
    for (int i=0;i<topLevelItemCount();i++)
    {
        item=topLevelItem(i);
        getItemData(item,t,g,s);
        if (t==type && g==gno && s==sno) goto end_select_item;
        for (int j=0;j<item->childCount();j++)
        {
            item2=item->child(j);
            getItemData(item2,t,g,s);
            if (t==type && g==gno && s==sno)
            {
                item=item2;
                goto end_select_item;
            }
        }
    }
end_select_item:
    return item;
}

void treeView::itemClickedAt(QTreeWidgetItem * item,int col)
{
    char type;
    int gno;
    int sno;
    getItemData(item,type,gno,sno);
    //cout << "type=" << type << " col=" << col << " G[" << gno << "].S[" << sno << "]" << endl;
    emit(newItemClicked(type,gno,sno));
}

void treeView::mousePressEvent(QMouseEvent * e)
{
    char type;
    int gno;
    int sno;
    QPoint p=e->pos();
    QTreeWidgetItem * curItem=itemAt(p);
    if (curItem==NULL) return;
    clearSelection();
    curItem->setSelected(true);
    if (e->button() & Qt::RightButton)
    {
        getItemData(curItem,type,gno,sno);
        //cout << type << " " << gno << " " << sno;
        popup->updateEntries(type,gno,sno);
        if (popup->possible==true)
            popup->exec(QCursor::pos());
    }
    else
    {
        itemClickedAt(curItem,0);
    }
}

void treeView::selectItem(char type,int gno,int sno)
{
    QTreeWidgetItem * item=findItem(type,gno,sno);
    if (item!=NULL)
    {
        clearSelection();
        item->setSelected(true);
    }
}

void LoadBinaryData(int gno,int setno,int settype,char * filename,struct BinReadInstructions inst)
{
    plotarr * p;
    char dummy[256];
    int importcount=0;
    int readbytes;
    int allocated;
    int startcount;
    char * dest;
    int * positions=new int[inst.n_columns_to_read];//positions, where the different colums are in one complete column set
    int length_of_one_column_set=0;

    for (unsigned int i=0;i<inst.column_count;i++)//all columns in a set (to be read or not)
        length_of_one_column_set+=inst.column_type[i].size;

    dest=new char[length_of_one_column_set+8];//prepare space to read one set

    if (inst.read_until_end_is_reached==true)
    {
        startcount=-1;//we will count downwards --> 0 is never reached
    }
    else
    {
        startcount=inst.datasets_to_read;//we will count downwards until zero is reached
    }
    //prepare set for writing
    int retval=activateset(gno,setno);
    set_dataset_type(gno,setno,settype);
    /*cout << "setlength=" << getsetlength(gno, setno) << endl;
cout << "number of cols=" << dataset_cols(gno, setno) << endl;
cout << "number of cols new settype=" << settype_cols(settype) << endl;
cout << "filename='" << filename << "'" << endl;*/
    p = &g[gno].p[setno];
    allocated=100;
    //prepare some space for the data that is to be read an stored
    p->data.len=0;
    for (unsigned int i=0;i<inst.n_columns_to_read;i++)
    {
        p->data.ex[i] = (double *)xrealloc(p->data.ex[i], allocated*sizeof(double));
        positions[i]=0;
        for (int j=0;j<i;j++)
            positions[i]+=inst.column_type[j].size;
        //cout << "column_offset[" << i << "]=" << positions[i] << endl;
    }
    ifstream ifi;
    ifi.open(filename);

    if (inst.length_of_header>0)
    {
        ifi.seekg(inst.length_of_header);
    }

    while (startcount!=0)
    {
        ifi.read(dest,length_of_one_column_set);//read one set of columns in one go into dest
        readbytes=(int)(ifi.gcount());
        if (readbytes!=length_of_one_column_set)
        {
            //cout << "error reading datacolumn; " << importcount << " sets already read" << endl;
            sprintf(dummy,"[Information] Stopped reading after import of %d datasets.",importcount);
            stufftext(dummy);
            break;
        }
        else//normal read operation
        {
            startcount--;
            //convert columns into dataset
            for (unsigned int i=0;i<inst.n_columns_to_read;i++)
            {
                switch (inst.column_type[inst.col_numbers_to_read[i]].format)
                {//get format of colum that is to be read
                case COLUMN_SIG_CHAR:
                    p->data.ex[i][importcount]=(double)(
                                *(
                                    (signed char*)(
                                        (void*)(dest+positions[i])
                                        )
                                    )
                                );
                    break;
                case COLUMN_USIG_CHAR:
                    p->data.ex[i][importcount]=(double)(
                                *(
                                    (unsigned char*)(
                                        (void*)(dest+positions[i])
                                        )
                                    )
                                );
                    break;
                case COLUMN_SIG_SHORT:
                    p->data.ex[i][importcount]=(double)(
                                *(
                                    (signed short*)(
                                        (void*)(dest+positions[i])
                                        )
                                    )
                                );
                    break;
                case COLUMN_USIG_SHORT:
                    p->data.ex[i][importcount]=(double)(
                                *(
                                    (unsigned short*)(
                                        (void*)(dest+positions[i])
                                        )
                                    )
                                );
                    break;
                case COLUMN_SIG_INT:
                    p->data.ex[i][importcount]=(double)(
                                *(
                                    (signed int*)(
                                        (void*)(dest+positions[i])
                                        )
                                    )
                                );
                    break;
                case COLUMN_USIG_INT:
                    p->data.ex[i][importcount]=(double)(
                                *(
                                    (unsigned int*)(
                                        (void*)(dest+positions[i])
                                        )
                                    )
                                );
                    break;
                case COLUMN_SIG_LONG:
                    p->data.ex[i][importcount]=(double)(
                                *(
                                    (signed long*)(
                                        (void*)(dest+positions[i])
                                        )
                                    )
                                );
                    break;
                case COLUMN_USIG_LONG:
                    p->data.ex[i][importcount]=(double)(
                                *(
                                    (unsigned long*)(
                                        (void*)(dest+positions[i])
                                        )
                                    )
                                );
                    break;
                case COLUMN_FLOAT:
                    p->data.ex[i][importcount]=(double)(
                                *(
                                    (float*)(
                                        (void*)(dest+positions[i])
                                        )
                                    )
                                );
                    break;
                case COLUMN_DOUBLE:
                    p->data.ex[i][importcount]=(double)(
                                *(
                                    (double*)(
                                        (void*)(dest+positions[i])
                                        )
                                    )
                                );
                    break;
                case COLUMN_LONG_DOUBLE:
                    p->data.ex[i][importcount]=(double)(
                                *(
                                    (long double*)(
                                        (void*)(dest+positions[i])
                                        )
                                    )
                                );
                    break;
                }
            }
            //check for enough space for the next run
            importcount++;
            if (importcount==allocated)
            {
                allocated+=100;
                for (unsigned int i=0;i<inst.n_columns_to_read;i++)
                {
                    p->data.ex[i] = (double *)xrealloc(p->data.ex[i], allocated*sizeof(double));
                    if (!p->data.ex[i]) cout << "error in reallocation" << endl;
                }
            }
        }
        if (ifi.eof()) break;
    }

    for (unsigned int i=0;i<inst.n_columns_to_read;i++)
        p->data.ex[i] = (double *)xrealloc(p->data.ex[i], importcount*sizeof(double));
    p->data.len=importcount;

    delete[] positions;
}

void GenerateLaTeXCommands(void)
{
    LaTeXCommands.clear();
    equivalentCommands.clear();
    equivalentFont.clear();
}

/*void store_plot_string(plotstr * p,stdLineEdit * led)
{
QString text=led->text();
char * te=new char[text.length()+2];
strcpy(te,text.toLatin1().constData());
p->s = copy_string(p->s, te);
p->alt = copy_string(p->alt, te);
//ENTER LATEX-CONVERSION HERE
delete[] te;
}*/

QString CreateRichTextColorTable(void)
{
    char dummy[32];
    QString table=QString("{\\colortbl;");
    for (int i=0;i<allocated_colors;i++)
    {
        sprintf(dummy,"\\red%d\\green%d\\blue%d;",cmap_table[i].rgb.red,cmap_table[i].rgb.green,cmap_table[i].rgb.blue);
        table+=QString(dummy);
    }
    table+=QString("}");
    return table;
}

//void find_set_ids(char * str,int * nr,int ** startpos,int ** len,int ** gnos,int ** snos)
void find_set_ids(char * str,int * nr,struct FoundSetID ** foundIds)
{
    int strlength=strlen(str);
    int maxnr=strlength/5+8;
    struct FoundSetID * found=new struct FoundSetID[maxnr];
    int index=0,offset=0,a,b;
    char * tmpstr=NULL;
    char next_char;
    QStringList list;
    QString original(str);
    original=original.toUpper();
    //cout << "String=#" << original.toLatin1().constData() << "#" << endl;
    QRegExp regex1("G\\d*\\.S\\d*");//what we search for as a regular expression --> we only search for complete set-ids
    while (offset<strlength)
    {
        found[index].pos=regex1.indexIn(original,offset);//find next set-id
        if (found[index].pos==-1)//nothing found
        {
            offset=strlength;
            break;
        }
        list=regex1.capturedTexts();//get the found text
        found[index].len=regex1.matchedLength();//get the length of the found text
        found[index].characteristic=0;//we assume a basic set-id at first
        tmpstr=new char[found[index].len+4];
        strcpy(tmpstr,list.at(0).toLatin1().constData());//copy found text for analysis
        sscanf(tmpstr,"G%d.S%d",&(found[index].gno),&(found[index].sno));

        offset=found[index].pos+found[index].len;
        //cout << "tmpstr=#" << tmpstr <<  "# offset="<< offset << " len=" << strlength << " pos=" << found[index].pos << " einzellaenge=" << found[index].len << endl;
        if (offset<strlength && offset>=0)
        {
            //next_char=tmpstr[strlen(tmpstr)-1];
            next_char=original.at(offset).toLatin1();
            if (next_char=='#')
            {
                found[index].characteristic=2;
                offset++;
                found[index].len++;
            }
            else if (next_char=='$')
            {
                found[index].characteristic=1;
                offset++;
                found[index].len++;
            }
            //cout << "next_char=#" << next_char << "# characteristic=" << found[index].characteristic << endl;
        }

        delete[] tmpstr;
        index++;
    }//end while-loop

    /*for (int i=0;i<index;i++)
{
cout << i << ": pos=" << found[i].pos << " len=" << found[i].len << " G" << found[i].gno << " S" << found[i].sno << endl;
}*/

    *nr=index;
    if (index>0)
    {
        if ((*foundIds)!=NULL)
            delete[] *foundIds;
        *foundIds=new struct FoundSetID[index+1];
        memcpy(*foundIds,found,sizeof(struct FoundSetID)*index);
    }
    delete[] found;
}

void find_graph_ids(char * str,int * nr,struct FoundSetID ** foundIds)
{
    int strlength=strlen(str);
    int maxnr=strlength/5+8;
    struct FoundSetID * found=new struct FoundSetID[maxnr];
    int index=0,offset=0,a,b;
    char * tmpstr=NULL,du;
    char next_char;
    QStringList list;
    QString original(str);
    original=original.toUpper();
    QRegExp regex1("\\sG\\d+(?!:\\.|\\S)");//what we search for as a regular expression

    //cout << "String=#" << original.toLatin1().constData() << "# (start=47)" << endl;

    while (offset<strlength)
    {
        found[index].pos=regex1.indexIn(original,offset)+1;//find next set-id
        if (found[index].pos<=0)//nothing found
        {
            offset=strlength;
            break;
        }
        list=regex1.capturedTexts();//get the found text
        found[index].len=regex1.matchedLength()-1;//get the length of the found text
        found[index].characteristic=0;
        tmpstr=new char[found[index].len+4];
        strcpy(tmpstr,list.at(0).toLatin1().constData());//copy found text for analysis
        //cout << "#" << tmpstr << "#" << endl;
        sscanf(tmpstr,"%cG%d",&du,&(found[index].gno));
        found[index].sno=-1;
        delete[] tmpstr;
        offset+=found[index].pos+found[index].len;
        if (offset<strlength && offset>=0)
        {
            next_char=original.at(offset).toLatin1();
            cout << "next_char=#" << next_char << "#" << endl;
            if (next_char=='#')
            {
                found[index].characteristic=2;
                offset++;
                found[index].len++;
            }
            else if (next_char=='$')
            {
                found[index].characteristic=1;
                offset++;
                found[index].len++;
            }
        }
        index++;
    }//end while-loop

    /*for (int i=0;i<index;i++)
{
cout << i << ": pos=" << found[i].pos << " len=" << found[i].len << " G" << found[i].gno << " S" << found[i].sno << endl;
}*/

    *nr=index;
    if (index>0)
    {
        delete[] *foundIds;
        *foundIds=new struct FoundSetID[index+1];
        memcpy(*foundIds,found,sizeof(struct FoundSetID)*index);
    }
    delete[] found;
}

//void postprocess_found_set_ids(int nr,int * gnos,int * snos,int * nr_unique_ids,int * id_color,int * unique_id)
void postprocess_found_set_ids(int nr,struct FoundSetID ** foundIds,int * nr_unique_ids,int ** unique_id)
{
    int cur_col;
    int * uni_ids=new int[nr+2];
    *nr_unique_ids=0;
    for (int i=0;i<nr;i++)
    {
        (*foundIds)[i].color=-1;//no valid color
        uni_ids[i]=-1;//no valid number
    }
    for (int i=0;i<nr;i++)//look through every set-id
    {
        if ((*foundIds)[i].color>-1) continue;//we are done with this id already
        if (is_set_active((*foundIds)[i].gno,(*foundIds)[i].sno)==TRUE)//the set already exists-->we take the color from this set
        {
            cur_col=g[(*foundIds)[i].gno].p[(*foundIds)[i].sno].linepen.color;
        }
        else
        {
            cur_col = (i+2) % number_of_colors();
        }
        (*foundIds)[i].color=cur_col;
        uni_ids[(*nr_unique_ids)++]=i;
        for (int j=i+1;j<nr;j++)//compare with every other set-id
        {
            if ((*foundIds)[i].gno==(*foundIds)[j].gno && (*foundIds)[i].sno==(*foundIds)[j].sno)
            {
                (*foundIds)[j].color==cur_col;
            }
        }
    }
    delete[] *unique_id;
    (*unique_id)=new int[*nr_unique_ids+2];
    memcpy(*unique_id,uni_ids,sizeof(int)*(*nr_unique_ids));
    delete[] uni_ids;
}

QString ColorToHtml(int nr)
{
    static char dummy[16];
    sprintf(dummy,"%02x%02x%02x",cmap_table[nr].rgb.red,cmap_table[nr].rgb.green,cmap_table[nr].rgb.blue);
    return QString("<span style=\"color:#") + QString(dummy) + QString("\">");
}

QString ColorHtmlEnd(void)
{
    return QString("<\\span>");
}

QString PaintSetIds(char * str,int nr,struct FoundSetID * setIds)
{
    /*QString tmp=QString("{\\f1 \n")+RTColorTable+QString("\n\\cf2 ");
tmp+=QString("Dies \\cf3 ist \\cf4 ein \\cf5 Test\\cf2 !");
return tmp+QString("}\n");*/
    QString tmp;
    QString original(str);
    char dummy[32];
    int pos=0;
    tmp+=ColorToHtml(1);
    for (int i=0;i<nr;i++)
    {
        tmp+=original.mid(pos,setIds[i].pos-pos);
        tmp+=ColorHtmlEnd();
        tmp+=ColorToHtml(setIds[i].color);
        sprintf(dummy,"G%d.S%d",setIds[i].gno,setIds[i].sno);
        tmp+=QString(dummy);
        tmp+=ColorHtmlEnd();
        tmp+=ColorToHtml(1);
        pos=setIds[i].pos+setIds[i].len;
    }
    tmp+=original.mid(pos,strlen(str));
    tmp+=ColorHtmlEnd();
    return tmp;
}

QString ReplaceSetIds(char * str,int nr,struct FoundSetID * setIds)
{
    if (nr<1) return QString(str);//no ids found, no ids to replace --> replace original
    QString original=QString(str);
    QString result;
    char new_set_id[32];
    int pos=0;
    result.clear();
    for (int i=0;i<nr;i++)//replace every set-id
    {
        if (setIds[i].sno<0) continue;//no real set-id
        sprintf(new_set_id,"G%d.S%d",setIds[i].repl_gno,setIds[i].repl_sno);
        result+=original.mid(pos,setIds[i].pos-pos);
        result+=QString(new_set_id);
        pos=setIds[i].pos+setIds[i].len;
    }
    result+=original.mid(pos);
    return result;
}

QString ReplaceGraphIds(char * str,int nr,struct FoundSetID * setIds)
{
    if (nr<1) return QString(str);
    QString original=QString(str);
    QString result;
    char new_set_id[32];
    int pos=0;
    result.clear();
    for (int i=0;i<nr;i++)//replace every Graph-id
    {
        if (setIds[i].sno>=0) continue;//no graph id, but real set id
        sprintf(new_set_id,"G%d",setIds[i].repl_gno);
        result+=original.mid(pos,setIds[i].pos-pos);
        result+=QString(new_set_id);
        pos=setIds[i].pos+setIds[i].len;
    }
    result+=original.mid(pos);
    return result;
}

void replace_single_formula_tokens(QString old_formula,QString & new_formula)
{
QString n_value;
new_formula=old_formula;
cout << "tokens=" << nr_of_single_f_tokens << endl;
    for (int i=0;i<nr_of_single_f_tokens;i++)
    {
        switch (formula_tokens[i].type)
        {
        case 0://integer-counter
        n_value=QString::number(formula_tokens[i].get_counter_value());
            break;
        case 1://double-value
        n_value=QString::number(formula_tokens[i].get_d_value());
            break;
        default:
        n_value=QString("0");
            break;
        }
    n_value=QString("(")+n_value+QString(")");
    cout << formula_tokens[i].representation << " type=" << (formula_tokens[i].type==0?"COUNTER":"VALUE") << " --> n_value=" << n_value.toLatin1().constData() << endl;
    new_formula.replace(formula_tokens[i].representation,n_value);
    cout << "replaced" << endl;
    }
}

int find_single_formula_token_in_list(int anz,class single_formula_token * tokenlist,char * token)
{
int ret=-1;
for (int i=0;i<anz;i++)
{
    if (strcmp(tokenlist[i].representation,token)==0)
    {
    ret=i;
    break;
    }
}
return ret;
}

int find_next_single_formula_token(char * formula,class single_formula_token * n_token)//finds the first token in a formula, returns the token and its position in the formula
{
static QRegExp exp1("N{1}\\d+[#,$]");
int ret=exp1.indexIn(QString(formula));
    if (ret!=-1)
    {
    QStringList list=exp1.capturedTexts();
    QString st=list.at(0);
    n_token->reset_token(st.toLatin1().data());
    }
return ret;
}

int find_all_single_formula_tokens(char * formula,class single_formula_token ** n_token)
{
int anz=0;
int anz_loc=10;
int len=strlen(formula),pos,ret;
if ((*n_token)!=NULL) delete[] (*n_token);
(*n_token)=new class single_formula_token[anz_loc];
pos=0;
ret=1;
while (ret>=0)
{
ret=find_next_single_formula_token(formula+pos,(*n_token)+anz);
    if (ret>=0)//new token found
    {
    anz++;
    pos+=ret+1;
    ret=find_single_formula_token_in_list(anz,*n_token,(*n_token)[anz-1].representation);
    if (ret==-1)
    {
    cout << "Error! Single token not found in list." << endl;
    }
    else if (ret!=anz-1)//does already exist
    {
    anz--;
    }
        if (anz==anz_loc)
        {
        class single_formula_token * tm=new class single_formula_token[anz_loc+10];
            for (int j=0;j<anz;j++)
            {
            tm[j]=(*n_token)[j];
            }
        delete[] (*n_token);
        *n_token=tm;
        anz_loc+=10;
        }
    }
}
return anz;
}

int find_single_formula_tokens_in_list(QStringList list,int & nr_of_tokens_found,class single_formula_token ** n_token)
{
nr_of_tokens_found=0;
class single_formula_token * f_token=new class single_formula_token[4];
char * cur_command=NULL;
int single_found=0,pos;
    if ((*n_token)!=NULL) delete[] (*n_token);
for (int i=0;i<list.length();i++)
{
cur_command=new char[list.at(i).length()+2];
strcpy(cur_command,list.at(i).toLatin1().constData());
single_found=find_all_single_formula_tokens(cur_command,&f_token);//find all tokens in a single command
    for (int j=0;j<single_found;j++)//look up all found tokens
    {
    pos=find_single_formula_token_in_list(nr_of_tokens_found,(*n_token),f_token[j].representation);//find the found tokens in the previously found list
        if (pos>=0)//this tokens has previously been found --> shorten the list
        {
            for (int k=j+1;k<single_found;k++)
            f_token[k-1]=f_token[k];
        single_found--;
        }
    }
    if (single_found>0)//add the new found tokens to the list
    {
    class single_formula_token * f_token2=new class single_formula_token[single_found+nr_of_tokens_found+2];
        for (int k=0;k<nr_of_tokens_found;k++)//copy old ones
        f_token2[k]=(*n_token)[k];
        for (int k=0;k<single_found;k++)//add new found ones
        f_token2[nr_of_tokens_found+k]=f_token[k];
    delete[] (*n_token);
        (*n_token)=f_token2;
    nr_of_tokens_found+=single_found;
    }
delete[] cur_command;
}

return nr_of_tokens_found;
}

single_formula_token::single_formula_token(char * token)
{
type=2;
representation=NULL;
    if (token!=NULL)
    reset_token(token);
i_counter=0;
d_value=0.0;
}

single_formula_token::~single_formula_token()
{
    if (representation!=NULL)
    delete[] representation;
}

void single_formula_token::initialize(void * value,int n_type)
{
    if (n_type==-1)//no change in type
    {
        if (type==0)//counter
        {
        i_counter=*((int*)value);
        }
        else if (type==1)//double value
        {
        d_value=*((double*)value);
        cout << "d_val=" << d_value << endl;
        }
    }
    else if (n_type==0)
    {
        type=n_type;
        i_counter=*((int*)value);
    }
    else if (n_type==1)
    {
        type=n_type;
        d_value=*((double*)value);
    }
    else//make it invalid
    {
        type=n_type;
    }
}

void single_formula_token::changeCounter(int delta)
{
i_counter+=delta;
}

void single_formula_token::setValueToFormula(char * formula)
{
double r_d_value;
int ret=std_evalexpr(formula,&r_d_value);
    if (ret==RETURN_SUCCESS)
    d_value=r_d_value;
}

int single_formula_token::get_counter_value(void)
{
return i_counter;
}

void single_formula_token::raise_counter(void)
{
i_counter++;
}

void single_formula_token::lower_counter(void)
{
i_counter--;
}

double single_formula_token::get_d_value(void)
{
return d_value;
}

void single_formula_token::reset_token(char * token)
{
if (representation!=NULL) delete[] representation;
    if (token!=NULL)
    {
    representation=new char[strlen(token)+2];
    strcpy(representation,token);
       if (representation[strlen(representation)-1]=='#')//counter
       type=0;
       else if (representation[strlen(representation)-1]=='$')//a double value
       type=1;
    cout << (type==0?"COUNTER":"VALUE") << " reseting token to " << representation << endl;
    }
    else
    {
    representation=NULL;
    type=2;
    }
}

class single_formula_token & single_formula_token::operator=(class single_formula_token & a)
{
type=a.type;
if (representation!=NULL) delete[] representation;
representation=new char[strlen(a.representation)+1];
strcpy(representation,a.representation);
i_counter=a.i_counter;
d_value=a.d_value;
return *this;
}

