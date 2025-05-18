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

#include "fundamentals.h"
#include "MainWindow.h"
#include "allWidgets.h"
#include "windowWidgets.h"
#include "undo_module.h"
#include "compressdecompress.h"

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
extern autoLabeler * FormAutoLabeler;
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
extern QPixmap * Qt_sub_justifications[3];
extern QPixmap * Qt_matrixOrder[8];

extern bool useQtFonts;
extern QList<QFont> stdFontList;
extern QFontMetrics * stdFontMetrics;
extern QStringList ListOfChanges;
extern QStringList ListOfOldStates;
extern CMap_entry *cmap_table;
extern int allocated_colors;

extern graph * g;
extern int maxgraph;
extern int new_set_no;
extern int DefaultFont;
extern bool activate_id_replacing;
extern bool exchange_point_comma;

extern QSize LastSize_Bailout;
extern QSize LastSize_CreateAltAxis;

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

extern void strToUpper(char * tar,const char * ch);
extern void add_ColorSelector(ColorSelector * colSel);
extern void add_FontSelector(FontSelector * fontSel);
extern void add_LaTeX_Line(stdLineEdit * line);
extern void add_non_LaTeX_Line(stdLineEdit * line);
extern void remove_Line(stdLineEdit * line);
extern void add_Line_Style_Selector(LineStyleSelector * line);
extern void add_Slider(stdSlider * slider);
extern void addPatternSelector(FillPatternSelector * selector);
extern void addOrderSelector(OrderSelector * selector);
extern void addAlignmentSelector(AlignmentSelector * selector);
extern void addJustificationSelector(JustificationSelector * selector);
extern void addLineWidthSelector(LineWidthSelector * selector);
extern void addPositionSelector(PositionSelector * selector);
extern void addIntSelector(stdIntSelector * selector);
extern void addStdSelector(StdSelector * selector);
extern void remove_StdSelector(StdSelector * selector);
extern void removeLastStdSelector(void);
extern void parse_qtGrace_Additions(char * s);

extern void update_font_selectors(bool appearance);
extern void update_graph_selectors(void);
extern void showSetInSpreadSheet(int gno,int setno);
void close_ss_editor(int gno,int setno);
extern frmLoadEval * FormLoadAndEvaluate;
extern int find_x_for_y(QString formula,double * x,double guess,double y);

extern void add_SetSelector(uniList * setSel);
extern void add_GraphSelector(uniList * grSel);
extern void add_SetChoiceItem(uniList * setSel);
extern void HelpCB(char *data);
extern char dummy[];//universal dummy for general purposes
extern bool updateRunning;
QColor * stdTextColor=NULL;

extern char SystemsDecimalPoint;//the default decimal-separator
extern char OldDecimalPoint;
extern char DecimalPointToUse;//what the user wants to use as decimal separator
#ifndef READ_UTF8_ONLY
extern QTextCodec * FileCodec;
#endif
extern int maxboxes;
extern int maxlines;
extern int maxstr;
extern int maxellipses;

///todo: check if this is realy needed
QStringList LaTeXCommands;
QStringList equivalentCommands;
QList<char> equivalentFont;

extern double rint_v2(double x);
extern int recursive_replacer(QString & text);
extern void GeneralPaste(const QMimeData * mimeData);
extern void complete_LaTeX_to_Grace_Translator(QString & text);
extern QString generateDisplayStringFromGraceString(char * gracestring);
extern void generate_string_Qt_aware(char * str,QString text);
extern void createSymbolIcons(QIcon ** symb_icons);

extern int find_QtFont_in_List(const char * name,int whatlist);//you need a qt-font-name for this
extern int find_QtFont_in_List(QString fontname,int whatlist);//finds a qt-font-name
extern int find_default_id_of_Grace_Font(const char * name);//you need a Grace-font-name for this
extern int find_GraceFontID_of_QtFontName(QString fontname);//you need a qt-font-name for this
extern QString getNameOfDefaultQtFont(int index);
extern QString get_QtName_of_Default_Grace_Font(const char * name);//compares a Grace-font-name with the default font names and returns the qt-font-name
extern char * get_Grace_Font_Name_of_Default_Qt_Font(QString fontname);//returns the Grace-font-name of a Qt-Font-name
extern char * getNameOfStdQtFont(int index);//returns the Qt-Name of the Font in the StdList
extern char * getFamilyNameOfStdQtFont(int index);//returns the Qt-Name of the Font in the StdList
//this is what you need
//--> remember: whatlist=0-->default-list, whatlist=1-->std-list (i.e. the current list - you probably want to use this)
//--> convert Grace-name into QtFontID
extern int get_QtFontID_from_Grace_Name(const char * name,int whatlist);//get the index of a QtFont in a list by its Grace-name
//--> convert Grace-font-id into QtFontID
extern int get_QtFontID_from_GraceID(int font_id,int whatlist);

extern void copySet(int gno,int setno,plotarr * pa,int what);
extern void reinstallSet(int gno,int setno,plotarr * pa,int what);
extern void deleteSavedSet(plotarr * pa,int what);
extern bool dont_delete_saved_set_memory;

extern void Replace_Dec_Sep_In_Single_String(QString & te);
extern void prepare_highlight_segments_with_graphs(int n,int * gnos);
extern void prepare_highlight_segments_with_sets(int n,int * gnos,int * snos,int do_clipping);
extern int get_legend_entry_bb_for_set(int gno, int setno, view * bb);
extern double getOrientationAngle(int type,int gno,int nr);
extern void remove_beginning_whitespaces(QString & text);
extern void split_comparison(char * comparison, char * partA, char * partB, int & comparator,int & posA,int & posB);
extern void remove_end_comment(QString & command);
extern void removePointInFormat(char * token,int type);

void WriteFilterString(QString & filterCommand,int o_n_sets,int * o_gnos,int * o_snos,int n_sets,int * gnos,int * snos,int type,int realization,double * limits,int * orders,char * x_formula,double ripple,int absolute,int debug,int point_extension,int oversampling,int rno,int invr);
void WriteRegressionString(QString & regressionCommand,int n_sets,int * gnos,int * snos,int n_n_sets,int * n_gnos,int * n_snos,int ideg,int rno,int invr,double start,double stop,int points,int rx,char * formula);

void ParseFit2D(char * com,int & type,int & gno,int & sno,int & t_set,double & x0,double & y0,double & r1,double & r2,double & angle,double & phi0,double & phi1,int & n_phi,int & region,int & reg_inv);
QString WriteFit2DString(int type,int gno,int sno,int t_set,double x0,double y0,double r1,double r2,double angle,double phi0,double phi1,int n_phi,int region,int reg_inv);

extern void createSetList(int gno,QList<int> & new_index,int nr_of_sets,int * shift_sets,int after_set);
extern void reSortSets(int gno,QList<int> new_index);
extern void invertedList(QList<int> list, QList<int> & inv_list);

extern int force_four_digit_year(int year);

int nr_of_set_app_saved=0;
int nr_of_set_app_allocated=0;
plotarr * saved_set_app=NULL;

extern int nr_of_single_f_tokens;
extern class single_formula_token * formula_tokens;

extern char last_formula[];
extern char saved_formula[];
extern double last_fit_falues[];
extern double saved_value;

extern int current_origin_graph,current_origin_set;
extern int current_target_graph,current_target_set;

extern QLocale * comma_locale;//a setting where ',' is the decimal separator (we use the setting for Germany here)
extern QLocale * dot_locale;//a setting where '.' is the decimal separator (we use the setting for the USA here)

extern OptionItem fmt_option_items[NUM_FMT_OPTION_ITEMS];

int slider_status=0;//0=slider inactive; 1=sliding started; 2=slider start accepted; 3=sliding finished

extern bool GlobalInhibitor;
extern bool immediateUpdate;
extern bool updateRunning;

void drawSimpleSymbol(QPainter & paint1,int x,int y,int w,int sym,char sym_c);//paints a symbol on a painter in the size w x w (always a square) on middle-position x,y; color and linestyle have to be set beforehand

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

void delete_special_int(int ** target)
{
if (*target==NULL) return;
delete[] *target;
*target=NULL;
}

void resize_special_int(int ** target,int o_size,int n_size)
{
int to_copy=o_size<n_size?o_size:n_size;
if (*target==NULL) to_copy=0;
if (n_size==0)
{
delete[] *target;
return;
}
int * new_target=new int[n_size];
if (*target!=NULL)
{
memcpy(new_target,*target,sizeof(int)*to_copy);
delete[] *target;
}
*target=new_target;
}

int nr_of_specifier_chars=18;
char specifier_chars[18]={'d','i','u','o','x','X','f','F','e','E','g','G','a','A','c','s','p','n'};

int is_valid_format_specifier(char * s)
{
int len=strlen(s);
int ret=0;
if (len<2) return 0;//we have to have at least a % and a character
if (s[0]!='%') return 0;//a format specifier has to start with %
for (int i=0;i<nr_of_specifier_chars;i++)
{
    if (s[len-1]==specifier_chars[i])//the last character matches one of the required specifier-characters
    {
    ret=1;
    break;
    }
}
return ret;
}

void format_specifier_error(char * s)
{
char error_message[512];
sprintf(error_message,"The data format '%s' is no valid format-specifier.\nA specifier has to start with '%%' and has to end with one of the following characters:\n",s);
int counter=strlen(error_message);
for (int i=0;i<nr_of_specifier_chars;i++)
{
error_message[counter+i*2]=specifier_chars[i];
    if (i<nr_of_specifier_chars-1)
    error_message[counter+1+i*2]=',';
    else
    error_message[counter+1+i*2]='\0';
}
counter=strlen(error_message);
sprintf(error_message+counter,"\nData format reset to '%s'.",grace_sformat);
errmsg(error_message);
}

QPixmap messageBoxStdIcon(QMessageBox::Icon ic,int width)
{
QStyle *style = QApplication::style();
QIcon tmpIcon;
switch (ic)
{
case QMessageBox::Information:
tmpIcon = style->standardIcon(QStyle::SP_MessageBoxInformation, 0, NULL);
break;
case QMessageBox::Warning:
tmpIcon = style->standardIcon(QStyle::SP_MessageBoxWarning, 0, NULL);
break;
case QMessageBox::Critical:
tmpIcon = style->standardIcon(QStyle::SP_MessageBoxCritical, 0, NULL);
break;
default:
case QMessageBox::Question:
tmpIcon = style->standardIcon(QStyle::SP_MessageBoxQuestion, 0, NULL);
break;
}
return tmpIcon.pixmap(width, width);
}

void getSetIDFromText(char * text,int & gno,int & sno,int & column)
{
//tries to read a set-id from the text, if incomplete id,
//the gno/sno-values stay untouched (gno/sno should be initialized
//with a suitable default before this function is called or a useless
//value to detect unsuccessfull read-attempts)
int a,b;
char c_text[32];
//qDebug() << "text=" << text;
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
//qDebug() << "0a: a=" << a << " b=" << b << " column=" << column;
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
//qDebug() << "0b: b=" << b << " column=" << column;
sno=b;
}
else if (sscanf(text,"G%d.S%d",&a,&b)==2)
{
//qDebug() << "1: a=" << a << " b=" << b;
gno=a;
sno=b;
return;
}
else if (sscanf(text,"G%d",&a)==1)
{
//qDebug() << "2: a=" << a;
gno=a;
return;
}
else if (sscanf(text,"S%d",&b)==1)
{
//qDebug() << "3: b=" << b;
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
            if (fallback.fromString(QString("Times New Roman,10,-1,5,50,0,0,0,0,0"))==false)
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
    //new_entries are appended; len(l) is the current storage size, n the number of new numbers to append
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

void append_to_storage_single(int * l,int ** storage,int new_entry)
{
static int new_entries[2];
new_entries[0]=new_entry;
append_to_storage(l,storage,1,new_entries);
}

void append_to_double_storage(int * l,double ** storage,int n,double * new_entries)
{
static int m1,m2;
    double * temp;
    //the memory size of storage automatically increases by DELTASIZE if needed
    //new_entries are appended; len(l) is the current storage size, n the number of new numbers to append
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
    if (*storage==NULL)//no memory so far
    {
        *storage=new double[(m2)*DELTASIZE];
        *l=0;
    }
    else if (m2>m1)//more memory needed
    {
        temp=*storage;
        *storage=new double[(m2)*DELTASIZE];
        memcpy(*storage,temp,sizeof(double)*(m1)*DELTASIZE);
        delete[] temp;
    }//else: enough memory
    memcpy(*storage+*l,new_entries,sizeof(double)*n);
    *l+=n;
}

void append_to_char_storage(int * l,char *** storage,int n,char ** new_entries)
{
static int m1,m2;
    char ** temp;
    //the memory size of storage automatically increases by DELTASIZE if needed
    //new_entries are appended; len(l) is the current storage size, n the number of new numbers to append
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
    if (*storage==NULL)//no memory so far
    {
        *storage=new char*[(m2)*DELTASIZE];
        for (int i=0;i<(m2)*DELTASIZE;i++) (*storage)[i]=NULL;
        *l=0;
    }
    else if (m2>m1)//more memory needed
    {
        temp=*storage;
        *storage=new char*[(m2)*DELTASIZE];
        memcpy(*storage,temp,sizeof(char*)*(m1)*DELTASIZE);
        for (int i=(m1)*DELTASIZE;i<(m2)*DELTASIZE;i++) (*storage)[i]=NULL;
        delete[] temp;
    }//else: enough memory
    memcpy(*storage+*l,new_entries,sizeof(char*)*n);
        for (int i=*l;i<(m2)*DELTASIZE;i++)
        {
            if (i-(*l)<n)
            {
                if (new_entries[i-(*l)]!=NULL)
                {
                (*storage)[i]=new char[2+strlen(new_entries[i-(*l)])];
                strcpy((*storage)[i],new_entries[i-(*l)]);
                }
                else
                {
                (*storage)[i]=NULL;
                }
            }
            else
            {
            break;
            }
        }
    *l+=n;
}

void append_to_storage2(int * l,int ** storage_a,int ** storage_b,int n,int * new_entries_a,int * new_entries_b)
{
int tmp_n=*l;//old storage size
append_to_storage(l,storage_a,n,new_entries_a);
*l=tmp_n;
append_to_storage(l,storage_b,n,new_entries_b);
//qDebug() << "adding nr of entries=" << n;
}

void SetDecimalSeparatorToUserValue(char * str,bool remove_space=true)//we assume a string containing a number like "2.15" and we want to change it to a number with the user selected decimal separator like "2,15"
{
QString chain(str);
    if (remove_space)
    chain.remove(QString(" "));//remove any useless spaces
    if (DecimalPointToUse=='.')
    {
        //chain.replace(QString(","),QString("|"));
        return;//Nothing to do here --> everything is as it should be
    }
    chain.replace(QString(","),QString("|"));//replace the ','s
    chain.replace(QString("."),QString(","));//replace the '.'s
strcpy(str,chain.toLatin1().constData());
}

void ReplaceDecimalSeparator(char * str,bool remove_space=true)//replace current user specified decimal separator with the internally used '.'
{
QString chain(str);
    //cout << "to replace = " << str << endl;
    if (remove_space)
    chain.remove(QString(" "));//remove any useless spaces
    if (DecimalPointToUse!='.')//the input contains ',' instead of '.' - but the system always expects '.'
    {
        //chain.remove(QString("."));//at first: remove '.'
        chain.replace(QString(","),QString("."));
        chain.replace(QString("|"),QString(","));//replace the ',' with '.'
    }
    else//DecimalPoint is '.' - any ',' are not useful
    {
        //chain.remove(QString(","));//just remove all ','
        //chain.replace(QString(","),QString("|"));
        ;//should be ok as it is!
    }
strcpy(str,chain.toLatin1().constData());
    //cout << "result = " << str << endl;
}

void RedisplayString(char * str)//replaces the old decimal separator with the new one
{
    if (OldDecimalPoint==DecimalPointToUse) return;
    QString chain(str);
    Replace_Dec_Sep_In_Single_String(chain);
    strcpy(str,chain.toLatin1().constData());
}

void PrepareFormula(char * str)//replaces the decimal separator in a formula with the '.' for internal calculations
{
    QString chain(str);
    ///chain.toUpper();///toUpper deactivated at the moment
    if (DecimalPointToUse==',')//if '.' is set --> nothing to do
    {//if ',' is used --> just replace ','-->'.'
        chain.replace(QChar(','),QChar('.'));
        chain.replace(QChar('|'),QChar(','));
    }
    strcpy(str,chain.toLatin1().constData());
}

void RedisplayFormula(char * str)//replaces the decimal separator except in the set-ids: e.g. G0.S4.Y
{
    /*QRegExp rex1("G\\d,S\\d,");
    QRegExp rex2("S\\d,");*/
    QRegularExpressionMatch rm;
    QRegularExpression rex1("G\\d,S\\d,");
    QRegularExpression rex2("S\\d,");
    int pos,pos2;
    QString chain(str);
    ///chain.toUpper();///toUpper deactivated at the moment
    if (OldDecimalPoint==',' && DecimalPointToUse=='.')//the simple case
    {
        chain.replace(QChar(OldDecimalPoint),QChar(DecimalPointToUse));
        chain.replace(QChar('|'),QChar(','));
    }
    else if (OldDecimalPoint=='.' && DecimalPointToUse==',')//the complicated case
    {
        chain.replace(QChar(','),QChar('|'));
        chain.replace(QChar(OldDecimalPoint),QChar(DecimalPointToUse));//this replaces the decimal separators as well as the set-id-separators: G0.S4.Y-->G0,S4,Y
        rm=rex1.match(chain,0);
            if (rm.hasMatch()==true)
            pos=rm.capturedStart();
            else
            pos=-1;
        while (pos>=0)
        {
            pos2=chain.indexOf(",",pos);
            chain[pos2]='.';
            pos2=chain.indexOf(",",pos);
            chain[pos2]='.';
            rm=rex1.match(chain,0);
                if (rm.hasMatch()==true)
                pos=rm.capturedStart();
                else
                pos=-1;
        }
        rm=rex2.match(chain,0);
            if (rm.hasMatch()==true)
            pos=rm.capturedStart();
            else
            pos=-1;
        while (pos>=0)
        {
            pos2=chain.indexOf(",",pos);
            chain[pos2]='.';
            rm=rex2.match(chain,0);
                if (rm.hasMatch()==true)
                pos=rm.capturedStart();
                else
                pos=-1;
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
//static double pi=2.0*asin(1.0);
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

QString print_komplex(struct komplex a)
{
QString text;
text=QString::number(a.real);
if (a.imag>=0.0) text+=QString("+");
else text+=QString("-");
text+=QString("i*")+QString::number(fabs(a.imag));
return text;
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

void SetMemoryToText(char * t1,char * t2,QString text)
{
    strcpy(t2,text.toUtf8().constData());//t2 is always the original - internally always in UTF8
        if (activateLaTeXsupport==TRUE)
        complete_LaTeX_to_Grace_Translator(text);
    generate_string_Qt_aware(t1,text);//t1 is the Grace-version of the text
}

void DynSetMemoryToLineEdit(char * &t1,char * &t2,char *&c1,char * &c2,bool & displayStd,bool acceptLaTeX,QLineEdit * lenText)
{
    static QPalette pal;
    static QString text;
    text=lenText->text();
    t2 = copy_string(t2,lenText->text().toUtf8().constData());//t2 is always the original - always internally in UTF8
    pal=lenText->palette();
    displayStd=true;
    char * te=new char[MAX_STRING_LENGTH];//longer is not allowed! Do not write essays in Grace ;-) !
        if (acceptLaTeX==true && activateLaTeXsupport==TRUE)
        {
        complete_LaTeX_to_Grace_Translator(text);//make the LaTeX-to-Grace-Translation
        }
    generate_string_Qt_aware(te,text);
    t1 = copy_string(t1,te);//t1 is the Grace-version of the text
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
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
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

Panel::Panel(QPixmap * pix, QWidget * parent, bool jcol):QLabel(parent)
{
    p=pix;
    inpaint=false;
    justColor=jcol;
    setMouseTracking(TRUE);
}

void Panel::paintEvent(QPaintEvent *event)
{
static QImage img;
static QColor col;
    if (inpaint==false)
        inpaint=true;
    else
        return;
    if (justColor==true)
    {
    img=p->toImage();
#if QT_VERSION >= 0x050000
    col=img.pixelColor(2,2);
#else
    col=QColor(img.pixel(2,2));
#endif
    }
    QPainter paint(this);
        if (justColor==false)
        paint.drawPixmap(0,0,*p);
        else
        paint.fillRect(0,0,this->width()+2,this->height()+2,col);
    paint.end();
    QLabel::paintEvent(event);
    inpaint=false;
}

void Panel::mouseMoveEvent( QMouseEvent * event )
{
    (void)event;
    emit(mouseMoving());
}

void Panel::mouseReleaseEvent(QMouseEvent *event)
{
    (void)event;
    emit(mouseClicked());
}

panelWindow::panelWindow(int rows,int cols,int last_col,QPixmap ** pix,QWidget * parent,bool hasAlphaSlider):QWidget(parent)//QDialog(parent)
{
    if (hasAlphaSlider==true)
    {
    alphaSlider=new horizontalAlphaSlider(this);
    }
    else
    {
    alphaSlider=NULL;
    }
    frm=new QFrame(this);
    layout0=new QHBoxLayout(this);
    //layout0->setMargin(0);
    layout0->setContentsMargins(0,0,0,0);
    layout0->setSpacing(0);
    frm->setFrameStyle(QFrame::Raised | QFrame::WinPanel);
#if QT_VERSION >= 0x050000

#else
    signalMapper1=new QSignalMapper;
    signalMapper2=new QSignalMapper;
#endif
    pixmaps=pix;
    row_count=rows;
    last_col_count=last_col;
    col_count=cols;
    number_of_elements=row_count*(col_count-1)+last_col_count;
    elementWidth=pix[0]->width();
    elementHeight=pix[0]->height();
    marked=highlighted=-1;//-1 means nothing ist marked or highlighted
    layout=new QGridLayout;
    //layout->setMargin(0);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    int r=0,c=0;
    lblPanel=new class Panel*[number_of_elements];
    //cout << "number_of_elements: " << number_of_elements << endl;
    for (int i=0;i<number_of_elements;i++)
    {
        lblPanel[i]=new class Panel(pixmaps[i],this);
        lblPanel[i]->setMinimumSize(elementWidth,elementHeight);
        //lblPanel[i]->setToolTip(QString::number(i));
#if QT_VERSION >= 0x050000
        connect(lblPanel[i],&Panel::mouseMoving,this,[this,i](){this->mouseMovedIn(i);});
        connect(lblPanel[i],&Panel::mouseClicked,this,[this,i](){this->mouseClickedIn(i);});
#else
        connect(lblPanel[i], SIGNAL(mouseMoving()), signalMapper1, SLOT(map()));
        connect(lblPanel[i], SIGNAL(mouseClicked()), signalMapper2, SLOT(map()));
        signalMapper1->setMapping(lblPanel[i],i);
        signalMapper2->setMapping(lblPanel[i],i);
#endif
        layout->addWidget(lblPanel[i],r,c);
        r++;
        if (r>=row_count)
        {
            r=0;
            c++;
        }
    }
    if (alphaSlider!=NULL)
    {
    layout->addWidget(alphaSlider,row_count,0,1,col_count);
    }
#if QT_VERSION >= 0x050000

#else
    connect(signalMapper1, SIGNAL(mapped(int)),this, SLOT(mouseMovedIn(int)));
    connect(signalMapper2, SIGNAL(mapped(int)),this, SLOT(mouseClickedIn(int)));
#endif
    clickTime=new QElapsedTimer();
    clickTime->start();
    frm->setLayout(layout);
    setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
    //setWindowFlags(Qt::Popup);
    setMouseTracking(true);
    layout0->addWidget(frm);
    setLayout(layout0);
    /// setModal(false);/// Changed to false for v.0.2.6!! Does not need to be modal anymore, because focus-out-event works properly now.
}

panelWindow::~panelWindow()
{
    if (alphaSlider!=NULL) delete alphaSlider;
    for (int i=0;i<number_of_elements;i++)
        delete lblPanel[i];
    delete[] lblPanel;
    delete clickTime;
#if QT_VERSION >= 0x050000

#else
    delete signalMapper1;
    delete signalMapper2;
#endif
    delete layout0;
    delete layout;
    delete frm;
}

void panelWindow::setAlpha_extern(int a)
{
    alpha=a;
    alphaSlider->slider->setValue(a);
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
/// qDebug() << "mouseMovedIn: i=" << i << " highlighted=" << highlighted << " marked=" << marked << " slider_has_mouse=" << (alphaSlider==QWidget::mouseGrabber()?"YES":"NO");
    if (alphaSlider!=NULL && alphaSlider->hasFocus())
    {
    alphaSlider->releaseMouse();
    this->grabMouse();
        if (i!=-1)
        lblPanel[i]->setFocus();
    }
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
    if (alphaSlider!=NULL && alphaSlider->hasFocus())
    {
    this->setFocus();
    this->grabMouse();
        if (i!=-1)
        lblPanel[i]->setFocus();
    }
/// qDebug() << "Moved In: panel=" << i;
}

void panelWindow::leaveEvent( QEvent * event )
{
    (void)event;
if (alphaSlider!=NULL && alphaSlider->rect().contains(alphaSlider->mapFromGlobal(QCursor::pos())))
{
    /// qDebug() << "PanelWindow: Entered alphaSlider";
alphaSlider->setFocus();
return;
}
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
/// qDebug() << "Leave PanelWindow";
}

void panelWindow::mouseClickedIn(int i)
{
    hide();
/// qDebug() << "Panel clicked in = " << i << " alpha=" << alpha << " alphaSlider.value=" << alphaSlider->slider->value();
    emit(newSelection(i));
        if (alphaSlider!=NULL && alpha!=alphaSlider->slider->value())
        {
        //qDebug() << "E";
        alpha=alphaSlider->slider->value();
        emit(newAlpha(alpha));
        }
        //else if (alpha==alphaSlider->slider->value())
        //qDebug() << "E alpha=" << alpha;
}

void panelWindow::mousePressEvent(QMouseEvent *event)
{
cur_element=element(QCursor::pos().x(),QCursor::pos().y());
/// qDebug() << "PressedOn " << cur_element;
if (alphaSlider!=NULL && alphaSlider->rect().contains(alphaSlider->mapFromGlobal(QCursor::pos())))
{
this->releaseMouse();
alphaSlider->setFocus();
event->ignore();
/// qDebug() << "MousePress: PanelWindow: Inside alphaSlider";
/// qDebug() << "PanelWindow.Focus=" << this->hasFocus();
/// qDebug() << "alphaSlider.Focus=" << alphaSlider->hasFocus();
}
else
{
this->grabMouse();
event->accept();
pressed=true;
/// qDebug() << "MousePress: PanelWindow: x=" << event->x() << " y=" << event->y();
}
}

void panelWindow::mouseReleaseEvent(QMouseEvent *event)
{
static int curTime;
cur_element=element(QCursor::pos().x(),QCursor::pos().y());
/// qDebug() << "ReleasedOn " << cur_element;
if (alphaSlider!=NULL && alphaSlider->rect().contains(alphaSlider->mapFromGlobal(QCursor::pos())))
{
this->releaseMouse();
alphaSlider->setFocus();
event->ignore();
/// qDebug() << "MouseRelease: PanelWindow: Inside alphaSlider";
/// qDebug() << "PanelWindow.Focus=" << this->hasFocus();
/// qDebug() << "alphaSlider.Focus=" << alphaSlider->hasFocus();
return;
}
else
this->grabMouse();
/// qDebug() << "MouseRelease: Pos=" << event->pos().y() << " height=" << row_count*elementHeight;
    if (pressed==false) return;
    curTime=clickTime->elapsed();
    if (curTime<300)
    {
        pressed=false;
        return;
    }
    //int cur_element=element(movex,movey);
    /*int x=event->x();
    int y=event->y();
    x=movex;
    y=movey;
    int r=y/elementHeight;
    int c=x/elementWidth;
    int element=r+c*row_count;*/
/// qDebug() << "Release: PanelWindow: x=" << x << " y=" << y << " xmax=" << col_count*elementWidth << " ymax=" << row_count*elementHeight;
    //if (x<0 || y<0 || x>col_count*elementWidth || y>row_count*elementHeight)
    if (cur_element<0)
    {
        //element=-1;
        if (marked>=0)
            lblPanel[marked]->mouseReleaseEvent(event);
/// qDebug() << "a: marked=" << marked;
        if (alphaSlider!=NULL && alpha!=alphaSlider->slider->value())
        {
        //qDebug() << "D";
        alpha=alphaSlider->slider->value();
        emit(newAlpha(alphaSlider->slider->value()));
        }
        //else if (alpha==alphaSlider->slider->value())
        //qDebug() << "D alpha=" << alpha;
    }
    releaseMouse();
    if (cur_element>=0 && cur_element<number_of_elements)
    {
        lblPanel[cur_element]->mouseReleaseEvent(event);
/// qDebug() << "b: element=" << element;
        if (alphaSlider!=NULL && alpha!=alphaSlider->slider->value())
        {
        //qDebug() << "A";
        alpha=alphaSlider->slider->value();
        emit(newAlpha(alphaSlider->slider->value()));
        }
        //else if (alpha==alphaSlider->slider->value())
        //qDebug() << "A alpha=" << alpha;
    hide();
    }
    //hide();
}

void panelWindow::mouseMoveEvent(QMouseEvent * event)
{
cur_element=element(QCursor::pos().x(),QCursor::pos().y());
/// qDebug() << "MovedOn " << cur_element;
if (alphaSlider!=NULL && alphaSlider->rect().contains(alphaSlider->mapFromGlobal(QCursor::pos())))
{
this->releaseMouse();
alphaSlider->setFocus();
event->ignore();
/// qDebug() << "MouseMove: PanelWindow: Inside alphaSlider";
/// qDebug() << "PanelWindow.Focus=" << this->hasFocus();
/// qDebug() << "alphaSlider.Focus=" << alphaSlider->hasFocus();
return;
}
else
{
this->grabMouse();
}
    /*int x=event->x();
    int y=event->y();
    int r=y/elementHeight;
    int c=x/elementWidth;*/
    //int cur_element=r+c*row_count;
    movex=event->x();
    movey=event->y();
    //cur_element=element(movex,movey);
    //if (x<0 || y<0) element=-1;
    //if (x>col_count*elementWidth || y>row_count*elementHeight) element=-1;
    if (cur_element<0 || cur_element>=number_of_elements)
    {
        leaveEvent(event);
    }
    else
    {
        if (cur_element!=highlighted)
        {
            lblPanel[cur_element]->mouseMoveEvent(event);
        }
    }
    this->mouseMovedIn(cur_element);
event->accept();
}

void panelWindow::keyPressEvent(QKeyEvent * event)
{
    if (event->key()==Qt::Key_Escape)
    {
    event->accept();
        if (alphaSlider!=NULL && alpha!=alphaSlider->slider->value())
        {
        //qDebug() << "B";
        alpha=alphaSlider->slider->value();
        emit(newAlpha(alphaSlider->slider->value()));
        }
        //else if (alpha==alphaSlider->slider->value())
        //qDebug() << "B alpha=" << alpha;
    hide();
    }
}

void panelWindow::focusOutEvent(QFocusEvent * event)
{
    if (alphaSlider!=NULL && alphaSlider->hasFocus() && alphaSlider->rect().contains(alphaSlider->mapFromGlobal(QCursor::pos()))) return;
/// qDebug() << "PanelWindow: FocusOut";
    event->accept();
        if (alphaSlider!=NULL && alpha!=alphaSlider->slider->value())
        {
        //qDebug() << "C";
        alpha=alphaSlider->slider->value();
        emit(newAlpha(alphaSlider->slider->value()));
        }
        //else if (alpha==alphaSlider->slider->value())
        //qDebug() << "C alpha=" << alpha;
    hide();
}

int panelWindow::element(int x,int y)
{
QPoint pos1(x,y);
QPoint pos2;
int nr=-1;
for (int i=0;i<number_of_elements;i++)
{
    pos2=lblPanel[i]->mapFromGlobal(pos1);
    if (lblPanel[i]->rect().contains(pos2))
    {
    nr=i;
    break;
    }
}
if (nr==-1 && alphaSlider!=NULL)
{
pos2=alphaSlider->mapFromGlobal(pos1);
    if (alphaSlider->rect().contains(pos2))
    {
    nr=-2;
    }
}
return nr;
}

mySlider::mySlider(Qt::Orientation orientation, QWidget *parent):QSlider(orientation,parent)
{
}

void mySlider::focusOutEvent(QFocusEvent * event)
{
//qDebug() << "Focus Out Event";
event->accept();
emit(focusOut());
}

verticalSliderPopup::verticalSliderPopup(int width,QWidget * parent):QWidget(parent)
{
//slider=new QSlider(Qt::Vertical,this);
slider=new mySlider(Qt::Vertical,this);
connect(slider,SIGNAL(focusOut()),this,SLOT(focusOut()));
//setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
setWindowFlags(Qt::Popup);
setMouseTracking(false);
width_hint=width;
visible_timer=new QElapsedTimer();
visible_timer->start();
}

void verticalSliderPopup::setRange(int min_v,int max_v)
{
slider->setRange(min_v,max_v);
}

void verticalSliderPopup::showEvent(QShowEvent * event)
{
this->blockSignals(true);
slider->blockSignals(true);
event->accept();
    this->resize(width_hint,5*width_hint);
    slider->setGeometry(STD_MARGIN,STD_MARGIN,width_hint-2*STD_MARGIN,width_hint*5-2*STD_MARGIN);
    this->move(parentWidget()->mapToGlobal(QPoint(0,parentWidget()->height())));
visible_timer->elapsed();
//qDebug() << "vert. Slider: Show Event";
slider->blockSignals(false);
this->blockSignals(false);
}

void verticalSliderPopup::focusOutEvent(QFocusEvent * event)
{
    if (slider->isVisible()==false || this->rect().contains(this->mapFromGlobal(QCursor::pos()))) return;
event->accept();
hide();
visible_timer->elapsed();
//qDebug() << "vertical Slider focus Out";
}

void verticalSliderPopup::focusOut(void)
{
    if (slider->isVisible()==false || this->rect().contains(this->mapFromGlobal(QCursor::pos()))) return;
hide();
visible_timer->elapsed();
//qDebug() << "focus_out";
}

alphaFrame::alphaFrame(QWidget * parent, bool hasSlider):QFrame(parent)
{
setFrameStyle(QFrame::Raised | QFrame::WinPanel);
if (hasSlider)
{
slider=new verticalSliderPopup(this->width(),this);
slider->setRange(0,255);
slider->hide();
connect(slider->slider,SIGNAL(sliderMoved(int)),this,SLOT(setAlpha(int)));
connect(slider->slider,SIGNAL(valueChanged(int)),this,SLOT(setAlpha(int)));
}
else
{
slider=NULL;
}
alpha=255;
setMaximumSize(20,20);
setToolTip(tr("Opacity (alpha-channel)"));
}

void alphaFrame::mousePressEvent( QMouseEvent * event )

{
    if (slider==NULL) return;
slider->slider->blockSignals(true);
slider->blockSignals(true);
event->accept();
int restart_time=slider->visible_timer->elapsed();
//qDebug() << "alphaFrame: mousePress: restart_time=" << restart_time << " slider.visible=" << slider->isVisible();
    if (slider->isVisible() || restart_time<100)
    {
    ;//slider->hide();
    }
    else
    {
    slider->width_hint=this->width();
    slider->slider->setValue(alpha);
    slider->show();
    slider->raise();
    slider->activateWindow();
    slider->slider->setFocus();
    //slider->setFocus();
    }
slider->blockSignals(false);
slider->slider->blockSignals(false);
}

void alphaFrame::mouseReleaseEvent( QMouseEvent * event )
{
event->accept();
    return;
//if (slider==NULL) return;
}

void alphaFrame::paintEvent(QPaintEvent * event)
{
QPainter * painter=new QPainter(this);
QColor col(Qt::white);
    painter->setPen(col);
    painter->setBrush(col);
    painter->drawRect(0,0,width(),height());
col.setRgb(0,0,0,alpha);
    painter->setPen(col);
    painter->setBrush(col);
    painter->drawRect(2,2,width()-5,height()-5);
    painter->end();
    event->accept();
}

void alphaFrame::resizeEvent( QResizeEvent * event )
{
repaint();
    if (slider!=NULL)
    if (slider->isVisible())
    {
    slider->show();
    }
event->accept();
}

QSize alphaFrame::sizeHint() const
{
return QSize(10,10);
}

void alphaFrame::setAlpha_extern(int a)
{
alpha=a;
repaint();
}

void alphaFrame::setAlpha(int a)
{
setAlpha_extern(a);
emit(valueChanged(a));
}

horizontalAlphaSlider::horizontalAlphaSlider(QWidget * parent):QWidget(parent)
{
layout=new QHBoxLayout(this);
layout->setSpacing(0);
//layout->setMargin(STD_MARGIN);
layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);

lblAlpha=new alphaFrame(this,false);
lblAlpha->setAlpha_extern(255);
lblAlpha->setMinimumSize(18,18);
lblAlpha->setToolTip(QString());

slider=new QSlider(Qt::Horizontal,this);
slider->setRange(0,255);
slider->setValue(255);
lblSlider=new QLabel(tr("Opacity:"),this);

layout->addWidget(lblSlider);
layout->addWidget(slider);
layout->addWidget(lblAlpha);

setLayout(layout);
connect(slider,SIGNAL(valueChanged(int)),SLOT(setAlpha(int)));
setMouseTracking(true);
}

void horizontalAlphaSlider::setAlpha(int a)
{
lblAlpha->setAlpha_extern(a);
//qDebug() << "setAlpha=" << a;
emit(alphaChanged(a));
}

void horizontalAlphaSlider::setAlpha_extern(int a)
{
slider->setValue(a);
}

void horizontalAlphaSlider::mousePressEvent( QMouseEvent * event )
{
//qDebug() << "horizontal: press at " << this->mapFromGlobal(QCursor::pos());
    if (!this->rect().contains(this->mapFromGlobal(QCursor::pos())))
    {
        releaseMouse();
        event->ignore();
        parentWidget()->grabMouse();
    }
}

void horizontalAlphaSlider::mouseReleaseEvent( QMouseEvent * event )
{
//qDebug() << "horizontal: release at " << this->mapFromGlobal(QCursor::pos());
    if (!this->rect().contains(this->mapFromGlobal(QCursor::pos())))
    {
        releaseMouse();
        event->ignore();
        parentWidget()->grabMouse();
    }
}

void horizontalAlphaSlider::leaveEvent( QEvent * event )
{
//qDebug() << "horizontal: leaveEvent";
this->releaseMouse();
parentWidget()->grabMouse();
parentWidget()->setFocus();
parentWidget()->setMouseTracking(true);
event->accept();
}

newCombo::newCombo(int rows,int cols,int last_col,QPixmap ** pix,QString ** titles,bool showTextOnly,QWidget * parent,bool hasAlphaSlider):QComboBox(parent)
{
    text_only=showTextOnly;
    simpleColorIcons=NULL;
    comboType=COMBOTYPE_INVALID;
    if (text_only==false)
    {
        setIconSize(QSize(pix[0]->width(),pix[0]->height()));
    }
    panels=new panelWindow(rows,cols,last_col,pix,this,hasAlphaSlider);//actually initialize the panels
    panels->hide();
    connect(panels,SIGNAL(newSelection(int)),SLOT(wrapperSetCurrentIndex(int)));
    title_strings=new QString[panels->number_of_elements];
    for (int i=0;i<panels->number_of_elements;i++)
        title_strings[i]=*(titles[i]);
    setMinimumWidth(110);
    createSimpleColorIcons();
    setCurrentIndex(0);
}

QIcon createSingleSmallColorIcon(int col)
{
QPixmap map1(12*toolBarSizeFactor,12*toolBarSizeFactor);
QPainter paint1;
QColor tmp_color;
    tmp_color.setRgb(cmap_table[col].rgb.red,cmap_table[col].rgb.green,cmap_table[col].rgb.blue);
    paint1.begin(&map1);
    paint1.setBrush(tmp_color);
    paint1.setPen(Qt::NoPen);
    paint1.drawRect(0,0,1+12*toolBarSizeFactor,1+12*toolBarSizeFactor);
    paint1.end();
return QIcon(map1);
}

QIcon createSingleSmallLineStyleIcon(int col)
{
QPixmap map1(22*toolBarSizeFactor,12*toolBarSizeFactor);
QPainter paint1;
QPen pen;
paint1.begin(&map1);
paint1.setBrush(Qt::white);
paint1.setPen(Qt::white);
paint1.drawRect(0,0,23*toolBarSizeFactor,13*toolBarSizeFactor);
    if (col>0)
    {
    //qDebug() << "col=" << col << " count=" << PenDashPattern[0]->count();
    pen.setDashPattern(*PenDashPattern[col]);
    pen.setColor(Qt::black);
    pen.setWidth(1*toolBarSizeFactor);
    paint1.setPen(pen);
    paint1.drawLine(0,6*toolBarSizeFactor,23*toolBarSizeFactor,6*toolBarSizeFactor);
    }
paint1.end();
return QIcon(map1);
}

void newCombo::createSimpleColorIcons(void)
{
    /* small color icons */
    if (simpleColorIcons!=NULL) delete[] simpleColorIcons;
    simpleColorIcons=new QIcon[panels->number_of_elements];
    /*QPixmap map1(12,12);
    QPainter paint1;
    QColor tmp_color;*/
int limit=(unsigned int)panels->number_of_elements<number_of_colors()?panels->number_of_elements:(int)number_of_colors();
    for (int i=0;i<limit;i++)
    {
    //cout << "Color " << i << "/" << number_of_colors() << "=" << cmap_table[i].rgb.red << "|" << cmap_table[i].rgb.green << "|" << cmap_table[i].rgb.blue << endl;
    simpleColorIcons[i]=createSingleSmallColorIcon(i);
    /*
    tmp_color.setRgb(cmap_table[i].rgb.red,cmap_table[i].rgb.green,cmap_table[i].rgb.blue);
    paint1.begin(&map1);
    paint1.setBrush(tmp_color);
    paint1.setPen(Qt::NoPen);
    paint1.drawRect(0,0,13,13);
    paint1.end();
    simpleColorIcons[i]=QIcon(map1);
    */
    }
}

void newCombo::reinitializePanels(int rows,int cols,int last_col,QPixmap ** pix,QString ** titles,bool showTextOnly,bool hasAlphaSlider)
{
    text_only=showTextOnly;
    if (text_only==false)
        setIconSize(QSize(pix[0]->width(),pix[0]->height()));
    //qDebug() << "IconSize(Ist)=" << iconSize().width() << "x" << iconSize().height() << "IconSize(Soll)=" << pix[0]->width() << "x" << pix[0]->height();
    //first: delete old panels
    delete panels;//call destructor
    //second: create new panels
    panels=new panelWindow(rows,cols,last_col,pix,this,hasAlphaSlider);//reinitialize the panels
    panels->hide();
    connect(panels,SIGNAL(newSelection(int)),SLOT(wrapperSetCurrentIndex(int)));
    //third: create new titles
    delete[] title_strings;
    title_strings=new QString[panels->number_of_elements];
    for (int i=0;i<panels->number_of_elements;i++)
    {
        //qDebug() << "ReinPanels: Titles (" << i << "/" << panels->number_of_elements << ")=" << *(titles[i]);
        title_strings[i]=*(titles[i]);
    }
        if (comboType==COMBOTYPE_COLOR)
        createSimpleColorIcons();
    /*if (text_only==false)
        setIconSize(QSize(pix[0]->width(),pix[0]->height()));*/
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
    {
        if (icon_preferences==0 || simpleColorIcons==NULL)//text only
        addItem(title_strings[index]);
        else if (icon_preferences==1)//text + icons
        addItem(simpleColorIcons[index],title_strings[index]);
        else//icons only
        addItem(simpleColorIcons[index],QString(""));
    }
    else
    {
        addItem(QIcon(*panels->pixmaps[index]),"");
    }
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
    panels->clickTime->restart();//elapsed
    panels->pressed=true;
    panels->repaint();
    qApp->processEvents();
    panels->grabMouse();/// reactivated this for v0.2.6
    panels->setFocus();
    //hidePopup();
    qApp->processEvents();
}

void newCombo::wheelEvent(QWheelEvent * e)
{
#if QT_VERSION >= 0x050000
double degrees=e->angleDelta().y()/8.0/15.0;
#else
double degrees=double(e->delta())/8.0/15.0;
#endif
int val,max;
    if (degrees!=0.0)
    {
        val=currentIndex();
        max=panels->number_of_elements;
        if (degrees>0)//scroll up
        {
        val--;
        }
        else if (degrees<0)//scroll down
        {
        val++;
        }
        if (val>=max) val=max-1;
        if (val<0) val=0;
        if (val!=currentIndex())
        {
        blockSignals(true);
        setCurrentIndex(val);
        blockSignals(false);
        emit(current_Index_Changed(val));
        }
    }
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
    //layout->setMargin(2);
    layout->setContentsMargins(2,2,2,2);
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
    Replace_Dec_Sep_In_Single_String(te);
    //te.replace(QChar(OldDecimalPoint),QChar(DecimalPointToUse));
    ledValue->setText(te);
    te=ledLowerBound->text();
    Replace_Dec_Sep_In_Single_String(te);
    //te.replace(QChar(OldDecimalPoint),QChar(DecimalPointToUse));
    ledLowerBound->setText(te);
    te=ledHighterBound->text();
    Replace_Dec_Sep_In_Single_String(te);
    //te.replace(QChar(OldDecimalPoint),QChar(DecimalPointToUse));
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
    //layout->setMargin(2);
    layout->setContentsMargins(2,2,2,2);
    layout->addWidget(lblNr);
    layout->addWidget(ledLocation);
    layout->addWidget(ledLabel);
    setLayout(layout);
}

QString axisLine::LabelText(void)
{
    /*static int pos,pos2,ret;
    static QString str2;
    static QString ref("$$"),intermediate;*/
    static QString result,str;
    result=str=ledLabel->text();
        if (activateLaTeXsupport==TRUE)
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
    if (activateLaTeXsupport==FALSE) return;
    ClickedOnLabel(c1,c2,original,ledLabel);
}

void axisLine::mouseReleaseEvent(QMouseEvent * e)
{
    if (activateLaTeXsupport==FALSE) return;
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
#if QT_VERSION >= 0x050000

#else
    signalMapper=new QSignalMapper(this);
#endif
    layout=new QHBoxLayout;
    //layout->setMargin(2);
    layout->setContentsMargins(2,2,2,2);
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
#if QT_VERSION >= 0x050000
        connect(cmdColHead[i],&QPushButton::clicked,this,[this,i](){this->pressed(i);});
#else
        connect(cmdColHead[i],SIGNAL(clicked()),signalMapper,SLOT(map()));
        signalMapper->setMapping(cmdColHead[i],i);
#endif
        if (i<cols)
        {
            cmdColHead[i]->setVisible(TRUE);
            layout->addWidget(cmdColHead[i]);
        }
        else
            cmdColHead[i]->setVisible(FALSE);
    }
    setLayout(layout);
#if QT_VERSION >= 0x050000

#else
    connect(signalMapper,SIGNAL(mapped(int)),SLOT(pressed(int)));
#endif
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
    //layout->setMargin(2);
    layout->setContentsMargins(2,2,2,2);
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
    axislines=NULL;
    fitlines=NULL;
    headline=NULL;
    spreadlines=NULL;

    layout=new QVBoxLayout;
    //layout->setMargin(0);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    generate_layout();
    setLayout(layout);
/*stdWidth=ledInput[0][0]->width();
stdHeight=ledInput[0][0]->height();*/
}

void spreadSheet::generate_layout(void)
{
#if QT_VERSION >= 0x050000

#else
    signalMapper=new QSignalMapper(this);
#endif
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

    /*
    for (int i=0;i<rows;i++)
    {
        ;
        //layout->addWidget();
        //connect(VHeads[i],SIGNAL(clicked()),signalMapper1,SLOT(map()));
        //signalMapper1->setMapping(VHeads[i],i);
    }
    //connect(signalMapper,SIGNAL(mapped(int)),SLOT(clickHHeader(int)));
    */
}

spreadSheet::~spreadSheet()
{

}

void spreadSheet::clickHHeader(int nr)
{
    (void)nr;
/*EditColumn->selColumn->setCurrentIndex(nr);
EditColumn->show();
EditColumn->raise();*/
}

void spreadSheet::clickVHeader(int nr)
{
    (void)nr;
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

void spreadSheet::adjustToFontSize(void)
{
switch (sheet_type)
{
case 0://type=0-->spreadsheet
break;
case 1://type=1-->fit-spreadsheet
    for (int i=0;i<=MAXPARM;i++)
    {
    fitlines[i]->ledValue->setMinimumHeight(fitlines[i]->ledValue->font().pixelSize()+9);
    fitlines[i]->ledLowerBound->setMinimumHeight(fitlines[i]->ledLowerBound->font().pixelSize()+9);
    fitlines[i]->ledHighterBound->setMinimumHeight(fitlines[i]->ledHighterBound->font().pixelSize()+9);
    }
    this->adjustSize();
break;
case 2://type=2-->axis-properties
default:
break;
}

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
    ret_time=new QElapsedTimer();
    ret_time->start();
    textHeight=rec.height();
    slideType=type;
    ScalingFactor=factor;
    if (textHeight<16) textHeight=16;
    //setMinimumSize(QSize(180,textHeight*3));
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
    //setMinimumSize(60,3*textHeight);
    //setMinimumSize(20,20);
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
    add_Slider(this);//add this to the list of sliders
}

QSize stdSlider::sizeHint() const
{
int h,w;
    QFontMetrics fm(Indicator->font());
    QSize si=fm.boundingRect(QString("100g")).size();
    w=4*si.width();
    if (lblText->text().length()<1)
    h=2.5*(si.height()+fm.descent());
    else
    h=3.5*(si.height()+fm.descent());
    //return QSize(60,textHeight*3);
    //return QSize(20,84);
//qDebug() << "Slider: sizeHint=" << w << " x " << h;
    return QSize(w,h);
}

QSize stdSlider::minimumSizeHint() const
{
int h,w;
    QFontMetrics fm(Indicator->font());
    QSize si=fm.boundingRect(QString("100g")).size();
/*
    qDebug() << "descent=" << fm.descent() << " ascent=" << fm.ascent();
    qDebug() << "Indicator: 100g: size=" << si;
    qDebug() << "Indicator: 1000: size=" << fm.boundingRect(QString("1000")).size();
    fm=QFontMetrics(lblText->font());
    qDebug() << "Text: 100g: size=" << fm.boundingRect(QString("100g")).size();
    qDebug() << "Text: 1000: size=" << fm.boundingRect(QString("1000")).size();
*/
    w=3*si.width();
    if (lblText->text().length()<1)
    h=2*(si.height()+fm.descent());
    else
    h=3*(si.height()+fm.descent());
    //return QSize(60,textHeight*3);
    //return QSize(20,84);
    if (w<120) w=120;
    if (h<70) h=70;
//qDebug() << "Slider: MinSizeHint=" << w << " x " << h;
    return QSize(w,h);
}

void stdSlider::resizeEvent( QResizeEvent * e)
{
e->accept();
QWidget::resizeEvent(e);
resetFont(qApp->font());
}

/*void stdSlider::resizeEvent( QResizeEvent * e)
{
static int width,height,lblTextHeight,offset;
static QFont fo;
static QString indText;
    offset=2;
e->accept();
    width=e->size().width();
    height=e->size().height();

    if (lblText->text().length()<1)
    {
    lblText->setVisible(false);
    lblTextHeight=0;
    //qDebug() << "INVISIBLE";
    }
    else
    {
    lblText->setVisible(true);
    lblTextHeight=1;
    //qDebug() << "VISIBLE !!";
    }
//qDebug() << "A RESIZE SLIDER: " << lblText->text() << " (len=" << lblText->text().length() << ") w=" << width << " h=" << height << " Value=" << Indicator->text() << " visible=" << lblText->isVisible();
    fo=qApp->font();
    textHeight=(height-(1+lblTextHeight)*offset)/(2+lblTextHeight);
    if (fo.pixelSize()>(int)rint_2(0.8*textHeight))
    {
    if ((int)rint_2(0.8*textHeight)<14) fo.setPixelSize(14);
    else fo.setPixelSize((int)rint_2(0.8*textHeight));
    lblText->setFont(fo);
    Indicator->setFont(fo);
    }
    QFontMetrics fm(Indicator->font());
    //QSize si=fm.boundingRect(Indicator->text()).size();
    QSize si=fm.boundingRect(QString("100g")).size();
    textHeight=si.height()+fm.descent();
    //if (height<(2+lblTextHeight)*textHeight)
    //{
    //    fo.setPixelSize(fo.pixelSize()-2);
    //    textHeight-=2;
    //    lblText->setFont(fo);
    //    Indicator->setFont(fo);
    //}
    //else if (height<(2+lblTextHeight)*textHeight && fo.pixelSize()<14)
    //{
    //    fo.setPixelSize(14);
    //    textHeight-=2;
    //    lblText->setFont(fo);
    //    Indicator->setFont(fo);
    //}
    Indicator->setGeometry(Indicator->x(),0,Indicator->width(),textHeight);
    Indicator->setGeometry(Indicator->x(),0,si.width()+2,si.height()+2);
    sldSlider->setGeometry(0,textHeight+offset,width,height-(1+lblTextHeight)*(textHeight+offset));
        if (lblTextHeight)
        {
        lblText->setGeometry(0,height-textHeight-offset,lblText->width(),textHeight);
        //qDebug() << "lblText: (" << lblText->x() << "|" << lblText->y() << ") w=" << lblText->width() << " h=" << lblText->height();
        }
    indText=indicatorText();
    slideIndicator();
    Indicator->setText(indText);
//qDebug() << "Indicator: (" << Indicator->x() << "|" << Indicator->y() << ") w=" << Indicator->width() << " h=" << Indicator->height();
//qDebug() << "sldSlider: (" << sldSlider->x() << "|" << sldSlider->y() << ") w=" << sldSlider->width() << " h=" << sldSlider->height();
//qDebug() << "B RESIZE SLIDER: w=" << this->width() << " h=" << this->height();
    QWidget::resizeEvent(e);
}*/

void stdSlider::changed(int i)
{
static QString indText;
    (void)i;
    indText=indicatorText();
    slideIndicator();
    Indicator->setText(indText);
    emit(valueChanged(i));
    if (slideType!=SLIDE_LINEAR)
    sldSlider->setFocus();
}

void stdSlider::SliderMoved(int i)
{
blockSignals(true);
    //(void)i;
    changed(i);
    //cout << "moved " << i << " old=" << old_value << endl;
blockSignals(false);
}

void stdSlider::SliderReleased(void)
{
blockSignals(true);
    slider_status=3;
    changed(value());
    //cout << "released old=" << old_value << " new=" << value() << endl;
    old_value=value();
blockSignals(false);
}

void stdSlider::SliderPressed(void)
{
blockSignals(true);
    //cout << "Pressed old=" << old_value << endl;
    slider_status=1;
    sldSlider->setFocus();
    changed(value());
blockSignals(false);
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

void stdSlider::resetFont(QFont fn)
{
static int width,height,lblTextHeight,offset;
static QFont fo;
static QString indText;
    offset=2;
    width=this->width();
    height=this->height();
        if (lblText->text().length()<1)
        {
        lblText->setVisible(false);
        lblTextHeight=0;
        }
        else
        {
        lblText->setVisible(true);
        lblTextHeight=1;
        }
    fo=fn;
    textHeight=(height-(1+lblTextHeight)*offset)/(2+lblTextHeight);
        if (fo.pixelSize()>(int)rint_2(0.8*textHeight))//reduce Font Size if needed
        {
        if ((int)rint_2(0.8*textHeight)<14) fo.setPixelSize(14);
        else fo.setPixelSize((int)rint_2(0.8*textHeight));
        //qDebug() << "Reducing Font Size: " << fo.pixelSize();
        }
        lblText->setFont(fo);
        Indicator->setFont(fo);
    QFontMetrics fm(Indicator->font());
    QSize si=fm.boundingRect(QString("100g")).size();
    textHeight=si.height()+fm.descent();
    Indicator->setGeometry(Indicator->x(),0,si.width()+2,si.height()+2);
    sldSlider->setGeometry(0,textHeight+offset,width,height-(1+lblTextHeight)*(textHeight+offset));
        if (lblTextHeight)
        {
        lblText->setGeometry(0,height-textHeight-offset,lblText->width(),textHeight);
        }
        /*else
        {
        lblText->setGeometry(0,height-textHeight-offset,lblText->width(),2);
        }*/
    indText=indicatorText();
    slideIndicator();
    Indicator->setText(indText);
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
        if (pow(10.0,i*ScalingFactor)<1.0)
        sprintf(dummy,"%2.02f",pow(10.0,i*ScalingFactor));//"%#2.2g"
        else
        sprintf(dummy,"%2.01f",pow(10.0,i*ScalingFactor));//"%#2.2g"
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
    //qDebug() << "Indicator: EnterPressed";
}

void stdSlider::Indicator_Finished(void)
{
    //qDebug() << "Indicator: EditFinished";
    int time_since_enter=ret_time->elapsed();
    double val=Indicator->text().toDouble();
//cout << "manual change: old_value=" << old_value << endl;
    if (time_since_enter<100)//less then 100ms since last Return pressed
    {
        //qDebug() << "Return Pressed (" << val << ")" << endl;
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
        //qDebug() << "Indicator Finished without Return (" << val << ") time_since_enter=" << time_since_enter;
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
    for (unsigned int i=0;i<number_of_fonts();i++)
        cmbFontSelect->addItem(get_fontalias((int)i));
    connect(cmbFontSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(changed(int)));
    cmbFontSelect->setCurrentIndex(DefaultFont);
    layout=new QHBoxLayout;
    //layout->setMargin(2);
    layout->setContentsMargins(2,2,2,2);
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
        for (unsigned int i=0;i<number_of_fonts();i++)
            cmbFontSelect->addItem(get_fontalias((int)i));
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
    //layout->setMargin(2);
    layout->setContentsMargins(2,2,2,2);
    layout->addWidget(lblText);
    layout->addWidget(cmbPositionSelect);
    setLayout(layout);
addPositionSelector(this);
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
    just_names2=new QString*[3];

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

    just_names2[0]=new QString("left");
    values2[0]=JUST_LEFT;
    just_names2[1]=new QString("center");
    values2[1]=JUST_CENTER;
    just_names2[2]=new QString("right");
    values2[2]=JUST_RIGHT;

    cmbJustSelect=new newCombo(3,4,3,Qt_justifications,just_names,false,this);
    cmbJustSelect->comboType=COMBOTYPE_JUSTIFICATION;
    lblText=new QLabel(tr("Justification:"));

    cmbJustSelect2=new newCombo(3,1,3,Qt_sub_justifications,just_names2,false,this);
    cmbJustSelect2->comboType=COMBOTYPE_ALIGNMENT;
    lblText2=new QLabel(tr("Align:"));

    setToolTip(tr("Justification sets the origin of the whole text box\n(Please note: the first 3 options only refer to the first line of text)\nAlign sets how the text is positioned in each line"));

    layout=new QHBoxLayout;
    //layout->setMargin(2);
    layout->setContentsMargins(2,2,2,2);
    layout->addWidget(lblText);
    layout->addWidget(cmbJustSelect);
    layout->addWidget(lblText2);
    layout->addWidget(cmbJustSelect2);
    setLayout(layout);
addJustificationSelector(this);
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

int JustificationSelector::currentIndex2(void)
{
    return values2[cmbJustSelect2->currentIndex()];
}

void JustificationSelector::setCurrentIndex2(int i)
{
    int index=0;
    for (int j=0;j<3;j++)
    {
        if (values2[j]==i)
        {
            index=j;
            break;
        }
    }
    cmbJustSelect2->setCurrentIndex(index);
}

AlignmentSelector::AlignmentSelector(QWidget * parent):QWidget(parent)
{
    just_names2=new QString*[3];

    just_names2[0]=new QString("left");
    values2[0]=JUST_LEFT;
    just_names2[1]=new QString("center");
    values2[1]=JUST_CENTER;
    just_names2[2]=new QString("right");
    values2[2]=JUST_RIGHT;

    cmbJustSelect2=new newCombo(3,1,3,Qt_sub_justifications,just_names2,false,this);
    cmbJustSelect2->comboType=COMBOTYPE_ALIGNMENT;
    lblText2=new QLabel(tr("Alignment:"));

    setToolTip(tr("Set the text alignment in each line"));

    layout=new QHBoxLayout;
    //layout->setMargin(2);
    layout->setContentsMargins(2,2,2,2);
    layout->addWidget(lblText2);
    layout->addWidget(cmbJustSelect2);
    setLayout(layout);
addAlignmentSelector(this);
}

int AlignmentSelector::currentValue(void)
{
    return values2[cmbJustSelect2->currentIndex()];
}

void AlignmentSelector::setCurrentValue(int i)
{
    int index=0;
    for (int j=0;j<3;j++)
    {
        if (values2[j]==i)
        {
            index=j;
            break;
        }
    }
    cmbJustSelect2->setCurrentIndex(index);
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
    //layout->setMargin(2);
    layout->setContentsMargins(2,2,2,2);
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
/*
cmbColorSelect=new QComboBox();
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
    cmbColorSelect=new newCombo(rows,cols,last_col,ColorPixmaps,ColorNames,true,this,true);
    cmbColorSelect->comboType=COMBOTYPE_COLOR;
    connect(cmbColorSelect,SIGNAL(current_Index_Changed(int)),this,SLOT(panelIndexChanged(int)));
    connect(cmbColorSelect->panels,SIGNAL(newAlpha(int)),this,SLOT(alphaValueChanged2(int)));
    connect(cmbColorSelect->panels->alphaSlider,SIGNAL(alphaChanged(int)),this,SLOT(alphaValueChanged2(int)));
    delete[] real_colors;
    alphaSelector=new alphaFrame(this,true);
    connect(alphaSelector,SIGNAL(valueChanged(int)),this,SLOT(alphaValueChanged(int)));
    connect(cmbColorSelect->panels->alphaSlider,SIGNAL(alphaChanged(int)),alphaSelector,SLOT(setAlpha(int)));
    layout=new QHBoxLayout;
    layout->setAlignment(Qt::AlignVCenter);
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->setSpacing(STD_SPACING);
    layout->addWidget(lblText);
    layout->addWidget(cmbColorSelect);
    layout->addWidget(alphaSelector);
    setLayout(layout);

    add_ColorSelector(this);

bool vis_extern=(show_transparency_selector==1?true:false);
bool vis_intern=(show_transparency_selector==2?true:false);
alphaSelector->setVisible(vis_extern);
cmbColorSelect->panels->alphaSlider->setVisible(vis_intern);
}

void ColorSelector::update_color_panels(void)
{
    updateColorIcons(number_of_colors(),ColorPixmaps,ColorNames);
}

void ColorSelector::panelIndexChanged(int i)
{
//qDebug() << "color Changed in Panels i=" << i;
    emit(currentIndexChanged(i));
}

void ColorSelector::alphaValueChanged(int i)
{
    cmbColorSelect->panels->alpha=i;
    cmbColorSelect->panels->alphaSlider->slider->setValue(i);
    emit(alphaChanged(i));
}

void ColorSelector::alphaValueChanged2(int i)
{
//qDebug() << "alpha Changed in Panels i=" << i;
    alphaSelector->setAlpha(i);
}

int ColorSelector::currentIndex(void)
{
    return cmbColorSelect->currentIndex();
}

int ColorSelector::alpha(void)
{
    return alphaSelector->alpha;
}

void ColorSelector::setCurrentIndex(int i)
{
    cmbColorSelect->setCurrentIndex(i);
}

void ColorSelector::setAlpha(int i)
{
    if (i<0 || i>255)
    {
    alphaSelector->setAlpha_extern(255);
    cmbColorSelect->panels->setAlpha_extern(255);
    }
    else
    {
    alphaSelector->setAlpha_extern(i);
    cmbColorSelect->panels->setAlpha_extern(i);
    }
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
    cmbColorSelect->reinitializePanels(rows,cols,last_col,ColorPixmaps,ColorNames,cmbColorSelect->text_only,(cmbColorSelect->panels->alphaSlider!=NULL?true:false));
    alphaSelector->setMaximumSize(20*sqrt(toolBarSizeFactor),20*sqrt(toolBarSizeFactor));
}

StdSelector::StdSelector(QWidget * parent)
{
    int number=2;
    QString * entr=new QString[2];
    entr[0]=QString("");
    entr[1]=QString("");
    QString label("");
    StdSelector(parent,label,number,entr);
    entryIcons=NULL;
//addStdSelector(this);
}

StdSelector::StdSelector(QWidget * parent,QString label,int number,QString * entr):QWidget(parent)
{
    lblText=new QLabel(label);
    cmbSelect=new QComboBox();
    entryIcons=NULL;
    simple_graph_selector=false;
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
    //layout->setMargin(2);
    layout->setContentsMargins(2,2,2,2);
    layout->addWidget(lblText);
    layout->addWidget(cmbSelect);
    setLayout(layout);
addStdSelector(this);
}

StdSelector::~StdSelector()
{
    delete[] entries;
    delete[] entryValues;
    if (entryIcons!=NULL) delete[] entryIcons;
remove_StdSelector(this);
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

void StdSelector::setValuesToGraphs(int includeNONE,int preserve_selection)
{
int number=number_of_graphs();
int sel=0;
include_NONE=includeNONE;
simple_graph_selector=true;
if (includeNONE) number++;
    if (number_of_entries>0 && preserve_selection)
    sel=this->currentIndex();
delete[] entries;
delete[] entryValues;
number_of_entries=number;
entries=new QString[number];
entryValues=new int[number];
number=0;
if (includeNONE)
{
    entries[number]=tr("None");
    entryValues[number]=-1;
    number++;
}
for (int i=0;i<number_of_graphs();i++)
{
    entries[number]=QString("G")+QString::number(i);
    entryValues[number]=i;
    number++;
}
redisplayItems();
setCurrentIndex(sel);
}

void StdSelector::setNewEntries(int number,QString * entr)
{
    int * values=new int[number];
    for (int i=0;i<number;i++) values[i]=i;
    setNewEntries(number,entr,values);
    delete[] values;
}

void StdSelector::setNewEntries(int number,QString * entr,int * values)
{
    delete[] entries;
    delete[] entryValues;
    number_of_entries=number;
    entries=new QString[number];
    entryValues=new int[number];
    for (int i=0;i<number;i++)
    {
        entries[i]=entr[i];
        entryValues[i]=values[i];
    }
    redisplayItems();
}

void StdSelector::setIcons(QIcon * icons)
{
    if (entryIcons!=NULL) delete[] entryIcons;
    if (icons==NULL)
    {
    entryIcons=NULL;
    return;
    }
    entryIcons=new QIcon[number_of_entries+1];
    for (int i=0;i<number_of_entries;i++) entryIcons[i]=icons[i];
    redisplayItems();
}

void StdSelector::redisplayItems(void)
{
    /*int cur_sel=currentIndex();
cout << "cur_sel=" << cur_sel << endl;
    this->blockSignals(true);*/
    cmbSelect->clear();
    if (entryIcons==NULL || icon_preferences==0)
    {
        for (int i=0;i<number_of_entries;i++)//text only
        {
        cmbSelect->addItem(entries[i]);
        }
    }
    else if (icon_preferences==1)//text and icons
    {
        for (int i=0;i<number_of_entries;i++)
        {
        cmbSelect->addItem(entryIcons[i],entries[i]);
        }
    }
    else//icons only
    {
        for (int i=0;i<number_of_entries;i++)
        {
        cmbSelect->addItem(entryIcons[i],QString(""));
        }
    }
    /*this->blockSignals(false);
    setCurrentIndex(cur_sel);*/
}

int StdSelector::currentIndex(void)
{
    return cmbSelect->currentIndex();
}

int StdSelector::currentValue(void)
{
    return entryValues[cmbSelect->currentIndex()];
}

QString StdSelector::currentText(void)
{
return cmbSelect->currentText();
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
int index=indexOfValueInList(i);
//qDebug() << "trying to set current value=" << i << " index=" << index << " nr_of_entries=" << number_of_entries;
    if (index<0)
    cmbSelect->setCurrentIndex(0);
    else
    cmbSelect->setCurrentIndex(index);
}

int StdSelector::indexOfValueInList(int v)//returns the index of the value v in the entryValues-list (-1 if not found
{
int index=-1;
    for (int j=0;j<number_of_entries;j++)
    {
        if (v==entryValues[j])
        {
            index=j;
            break;
        }
    }
return index;
}

bool StdSelector::valueIsInList(int v)//returns true, if v is in the list of entryValues
{
return indexOfValueInList(v)==-1?FALSE:TRUE;
}

StdSelector * createColumnFormatSelector(QWidget * parent,QString text)
{
StdSelector * sel=NULL;
int values[NUM_FMT_OPTION_ITEMS+1];
QString entries[NUM_FMT_OPTION_ITEMS+1];
for (int i=0;i<NUM_FMT_OPTION_ITEMS;i++)
{
values[i]=i;
entries[i]=QString(fmt_option_items[i].label);
}
values[NUM_FMT_OPTION_ITEMS]=FORMAT_STRING;
entries[NUM_FMT_OPTION_ITEMS]=QObject::tr("String");
sel=new StdSelector(parent,text,NUM_FMT_OPTION_ITEMS+1,entries);
if (sel!=NULL) sel->setValues(values);
return sel;
}

DataFormatSelector::DataFormatSelector(QWidget * parent):QWidget(parent)
{
orientation=0;//0=horizontal, 1=vertical;
lblText=new QLabel(QString(""),this);
cmbSelect=new QComboBox(this);
layout=new QGridLayout();
//layout->setMargin(0);
layout->setContentsMargins(0,0,0,0);
layout->setSpacing(STD_SPACING);
nr_of_values=NUM_FMT_OPTION_ITEMS;
values=new int[1+nr_of_values];
for (int i=0;i<nr_of_values;i++)
{
cmbSelect->addItem(QString(fmt_option_items[i].label));
values[i]=i;
}
cmbSelect->addItem(tr("String"));
values[nr_of_values++]=FORMAT_STRING;
layout->addWidget(lblText,0,0);
layout->addWidget(cmbSelect,0,1);
setCurrentValue(FORMAT_GENERAL);
this->setLayout(layout);
}

void DataFormatSelector::setText(QString text)
{
lblText->setText(text);
}

void DataFormatSelector::setOrientation(int v)
{
orientation=v;
layout->removeWidget(lblText);
layout->removeWidget(cmbSelect);
    if (orientation==0)//0=horizontal
    {
    layout->addWidget(lblText,0,0);
    layout->addWidget(cmbSelect,0,1);
    }
    else//1=vertical
    {
    layout->addWidget(lblText,0,0);
    layout->addWidget(cmbSelect,1,0);
    }
}

int DataFormatSelector::currentIndex(void)
{
return cmbSelect->currentIndex();
}

int DataFormatSelector::currentValue(void)
{
return values[cmbSelect->currentIndex()];
}

void DataFormatSelector::setCurrentIndex(int i)
{
cmbSelect->setCurrentIndex(i);
}

void DataFormatSelector::setCurrentValue(int i)
{
if (i<-2 || i>=NUM_FMT_OPTION_ITEMS || i==-1) return;
int val=NUM_FMT_OPTION_ITEMS;
if (i>=0) val=i;
cmbSelect->setCurrentIndex(val);
}

MultiDataFormatSelector::MultiDataFormatSelector(QWidget * parent):QScrollArea(parent)
{
    typical_w=110;
    typical_h=25;
#if QT_VERSION >= 0x050000

#else
mapDel=new QSignalMapper();
mapCols=new QSignalMapper();
connect(mapDel, SIGNAL(mapped(int)),this, SLOT(deleteSet(int)));
connect(mapCols, SIGNAL(mapped(int)),this, SLOT(nrOfColsChanged(int)));
#endif
general_running=false;
layout=new QGridLayout();
//layout->setMargin(0);
layout->setContentsMargins(0,0,0,0);
layout->setSpacing(STD_SPACING);
col_selectors_allocated=selectors_allocated=sets_allocated=0;
selectors=NULL;
lblSetDescr=NULL;
nr_of_formats_in_set=NULL;
selNrOfSets=new stdIntSelector(this,tr("Number of sets in file:"),0,100000);
connect(selNrOfSets,SIGNAL(currentValueChanged(int)),this,SLOT(nrOfSetsChanged(int)));
cmdDelSet=NULL;
selNrOfCols=NULL;
empty=new QWidget(this);
empty->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
//empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
setWidget(empty);
layout->addWidget(selNrOfSets,0,0,1,2);
empty->setLayout(layout);
this->setWidgetResizable(false);
}

void MultiDataFormatSelector::reCreateSelectors(int sets,int * nr_of_columns,int * set_data)
{
static bool running=false;
    if (running==true) return;
    elements_width=0;
    typical_h=min_element_height;
    general_running=true;
    running=true;
    if (selNrOfSets->value()!=sets)
    selNrOfSets->setValue(sets);
//qDebug() << "size before=" << empty->size();
//qDebug() << "Before: Sets allocated=" << sets_allocated << "Selectors allocated=" << selectors_allocated;
for (int i=0;i<selectors_allocated;i++){layout->removeWidget(selectors[i]);N_DELETE(selectors[i]);}
for (int i=0;i<sets_allocated;i++){layout->removeWidget(lblSetDescr[i]);N_DELETE(lblSetDescr[i]);}
for (int i=0;i<sets_allocated;i++){layout->removeWidget(cmdDelSet[i]);N_DELETE(cmdDelSet[i]);}
stdIntSelector**sav_selNrOfCols;
    if (col_selectors_allocated>0)
    sav_selNrOfCols=new stdIntSelector*[col_selectors_allocated];
    else
    sav_selNrOfCols=NULL;
for (int i=0;i<col_selectors_allocated;i++){layout->removeWidget(selNrOfCols[i]);selNrOfCols[i]->setVisible(false);sav_selNrOfCols[i]=selNrOfCols[i];/*N_DELETE(selNrOfCols[i]);*/}
//for (int i=0;i<sets_allocated;i++){layout->removeWidget(selNrOfCols[i]);}
int max_cols=0,old_max_cols=0;
int old_sets=sets_allocated;
    for (int i=0;i<sets_allocated;i++)
    {
    if (nr_of_formats_in_set[i]>old_max_cols) old_max_cols=nr_of_formats_in_set[i];
    }
N_DELETE2(selectors);
N_DELETE2(lblSetDescr);
N_DELETE2(nr_of_formats_in_set);
N_DELETE2(cmdDelSet);
if (sets>col_selectors_allocated)//more needed
{
N_DELETE2(selNrOfCols);
selNrOfCols=new stdIntSelector*[sets];
for (int i=0;i<col_selectors_allocated;i++) selNrOfCols[i]=sav_selNrOfCols[i];
    for (int i=col_selectors_allocated;i<sets;i++)
    {
    selNrOfCols[i]=new stdIntSelector(this,tr("Columns:"),1,100000);
#if QT_VERSION >= 0x050000
    connect(selNrOfCols[i],QOverload<int>::of(&stdIntSelector::currentValueChanged),this,[this,i](int index){(void)index;this->nrOfColsChanged(i);});
#else
    connect(selNrOfCols[i], SIGNAL(currentValueChanged(int)), mapCols, SLOT(map()));
    mapCols->setMapping(selNrOfCols[i],i);
#endif
    }
col_selectors_allocated=sets;
}
selectors_allocated=0;
nr_of_formats_in_set=new int[sets];
lblSetDescr=new QLabel*[sets];
cmdDelSet=new QPushButton*[sets];
    for (int i=0;i<sets;i++)
    {
    selectors_allocated+=nr_of_columns[i];
    nr_of_formats_in_set[i]=nr_of_columns[i];
    if (nr_of_columns[i]>max_cols) max_cols=nr_of_columns[i];
    }
    sets_allocated=sets;
if (selectors_allocated<=0){running=false;general_running=false;return;}
selectors=new DataFormatSelector*[selectors_allocated];
int index=0;
QString text;
    for (int i=0;i<sets;i++)
    {
    text=tr("Set ")+QString::number(i);
        if (set_data[i]>=0)
        text+=QString(" (")+QString::number(set_data[i])+tr(" data points)");
    text+=QString(":");
    lblSetDescr[i]=new QLabel(text,this);
    cmdDelSet[i]=new QPushButton(tr("Delete set"),this);
    cmdDelSet[i]->setToolTip(tr("This button just deletes the set from the list of format settings (no data is deleted)."));
    //selNrOfCols[i]=new stdIntSelector(this,tr("Columns:"),0,100000);
    selNrOfCols[i]->setValue(nr_of_columns[i]);
    selNrOfCols[i]->setVisible(true);
#if QT_VERSION >= 0x050000
    connect(cmdDelSet[i], &QPushButton::clicked,this,[this,i](){this->deleteSet(i);});
#else
    connect(cmdDelSet[i], SIGNAL(clicked()), mapDel, SLOT(map()));
    mapDel->setMapping(cmdDelSet[i],i);
#endif

    layout->addWidget(lblSetDescr[i],1+i*2,0,1,1);
    layout->addWidget(selNrOfCols[i],1+i*2,1,1,1);
    layout->addWidget(cmdDelSet[i],1+i*2,2,1,1);
        for (int j=0;j<nr_of_columns[i];j++)
        {
        selectors[index]=new DataFormatSelector(this);
        selectors[index]->setMinimumWidth(typical_w);
        selectors[index]->setOrientation(1);
        text=tr("Column C")+QString::number(j)+QString(":");
        selectors[index]->setText(text);
        layout->addWidget(selectors[index],1+2*i+1,j);
        selectors[index]->adjustSize();
        elements_width+=selectors[index]->width();
        index++;
        }
    }
    index=0;
        /// if (empty->width()<=0 || empty->height()<=0)
        /// empty->adjustSize();
    /*int w=empty->width();
    int h=empty->height()-selNrOfSets->height();
    int n_w=w*max_cols;
    if (old_max_cols<=0) n_w=200;
    else n_w*=1.0/old_max_cols;
    int n_h=h*sets;
    if (old_sets<=0) n_h=100;
    else n_h*=1.0/old_sets;
    if (w<200 ||old_max_cols<=0) n_w=200;
    if (h<100 || old_sets<=0) n_h=100;
    n_h+=selNrOfSets->height();*/
    /// empty->setMinimumSize(n_w,n_h);
//qDebug() << "elements_width=" << elements_width;

    //Now we generate the typical width of the controls using the current font-size of the application
    QFontMetrics fm1(qApp->font());
    QString test_str(fmt_option_items[FORMAT_YYDYHMS].label);//this is the longest entry
    test_str+=QString("gy");//we add a few characters to get a bit of extra space and some space below
    QRect fm1_rect=fm1.boundingRect(test_str);
//qDebug() << "Vorher: w=" << typical_w << "h=" << typical_h;
    typical_w=fm1_rect.width()*1.05;
    typical_h=fm1_rect.height()*1.85;//test
//qDebug() << "Nachher: w=" << typical_w << "h=" << typical_h;
    int min_width=3*typical_w;
    if (max_cols>3) min_width=max_cols*typical_w;
    min_width*=1.1;
    int min_height=typical_h;
    min_height+=2.5*typical_h*selNrOfSets->value();
    min_height*=1.05;
//qDebug() << "typical_h=" << typical_h << "min_height=" << min_height;

    /*if (min_width<elements_width)
    {
    qDebug() << "minWidth=" << min_width << "elements_width=" << elements_width;
    min_width=elements_width;
    }*/
    empty->setMinimumSize(min_width,min_height);
    /*
    empty->adjustSize();
    for (int i=0;i<sets;i++)
    {
        lblSetDescr[i]->adjustSize();
        qDebug() << lblSetDescr[i]->minimumSizeHint() << " " << selNrOfCols[i]->minimumSizeHint() << " " << cmdDelSet[i]->minimumSizeHint();
        selNrOfCols[i]->adjustSize();
        cmdDelSet[i]->adjustSize();
        for (int j=0;j<nr_of_columns[i];j++)
        {
        qDebug() << selectors[index]->minimumSizeHint() << " ";
        selectors[index++]->adjustSize();
        }
    }*/
    //empty->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    /*for (int i=0;i<layout->rowCount();i++)
    {
    layout->setRowMinimumHeight(i,typical_h);
    }*/
    empty->adjustSize();
//qDebug() << "MinWidth=" << min_width << "MinHeight=" << min_height;
//qDebug() << "size after=" << empty->size();
    running=false;
    general_running=false;
//qDebug() << "After: Sets allocated=" << sets_allocated << "Selectors allocated=" << selectors_allocated;
}

void MultiDataFormatSelector::nrOfSetsChanged(int s)
{
static bool running=false;
    if (general_running==true) return;
    if (running==true) return;
    running=true;
QList< QList<int> > formats,n_formats;
getAllFormatSettings(formats);
n_formats=formats;
int sum=0;
    for (int i=0;i<formats.length();i++)
    sum+=formats.at(i).length();
int sets=((s>formats.length())?s:formats.length());
int nr_of_new_sets=0;
    if (s>formats.length())
    {
    nr_of_new_sets=s-formats.length();
    }
int * nr_of_columns=new int[sets+2];
int * set_data=new int[sum+2*(1+nr_of_new_sets)];
int index=0;
QList<int> tmp_list;
    for (int i=0;i<s;i++)
    {
    set_data[i]=-1;
        if (i>=formats.length())//new set(s)
        {
        nr_of_columns[i]=2;
        index+=2;
        tmp_list.clear();
        tmp_list << FORMAT_GENERAL << FORMAT_GENERAL;
        n_formats << tmp_list;
        }
        else
        {
        nr_of_columns[i]=formats.at(i).length();
        index+=formats.at(i).length();
        }
    }
    for (int i=formats.length()-1;i>=s;i--)
    {
    n_formats.removeAt(i);
    }
//qDebug() << "number of sets changed:" << s;
reCreateSelectors(s,nr_of_columns,set_data);
setAllFormatSettings(n_formats);
delete[] nr_of_columns;
delete[] set_data;
running=false;
}

void MultiDataFormatSelector::nrOfColsChanged(int c)
{
static bool running=false;
if (general_running==true) return;
if (running==true){/*qDebug() << "RUNNING";*/return;}
int line_with_new_col_count=-1;
running=true;
//qDebug() << "number of columns in sets"<< c << "changed: " << selNrOfCols[c]->value();
QList< QList<int> > formats,n_formats;
getAllFormatSettings(formats);
int sum=0;
    for (int i=0;i<formats.length();i++)
    sum+=formats.at(i).length();//number of old_formats
int sum2=0;
int * set_data=new int[sets_allocated];
    for (int i=0;i<sets_allocated;i++)
    {
        if (i<formats.length())
        {
        if (selNrOfCols[i]->value()!=formats.at(i).length()) line_with_new_col_count=i;
        }
    sum2+=selNrOfCols[i]->value();//number of new formats
    //qDebug() << i << "Col_Widget_pos=" << selNrOfCols[i]->pos() << "hasFocus=" << selNrOfCols[i]->hasFocus();
    set_data[i]=-1;
    }
    //qDebug() << "Formats: Old=" << sum << "New=" << sum2;
int sets=formats.length();
//qDebug() << "SetsAllocated=" << sets_allocated << "sets=" << sets;
int * nr_of_columns=new int[sets+2];
QList<int> tmp_list;
n_formats.clear();
for (int i=0;i<sets;i++)
{
tmp_list=formats.at(i);
//qDebug() << tmp_list;
    if (i==c)//the sets that is about to change
    {
        for (int j=nr_of_formats_in_set[i];j<selNrOfCols[i]->value();j++){/*qDebug() << "Adding:" << j;*/tmp_list << FORMAT_GENERAL;}
        for (int j=tmp_list.length()-1;j>=selNrOfCols[i]->value();j--){/*qDebug() << "Removing:" << j;*/tmp_list.removeAt(j);}
    }
n_formats << tmp_list;
nr_of_columns[i]=tmp_list.length();
}
/*qDebug() << "n_formats:" << n_formats;
qDebug() << "sets=" << sets;
    for (int i=0;i<sets;i++)
    qDebug() << "nr_of_cols=" << nr_of_columns[i] << "set_data=" << set_data[i];*/
reCreateSelectors(sets,nr_of_columns,set_data);
setAllFormatSettings(n_formats);
delete[] nr_of_columns;
delete[] set_data;
running=false;
//qDebug() << "w.x=" << w->x() << "w.y=" << w->y() << "new one=" << line_with_new_col_count;
if (line_with_new_col_count>=0)
ensureVisible(selNrOfCols[line_with_new_col_count]->x(),selNrOfCols[line_with_new_col_count]->y());
}

void MultiDataFormatSelector::deleteSet(int s)
{
//qDebug() << "delete set"<< s;
QList< QList<int> > formats,n_formats;
getAllFormatSettings(formats);
//qDebug() << formats;
int sum=0;
    for (int i=0;i<formats.length();i++)
    sum+=formats.at(i).length();//number of old_formats
//int sum2=sum-selNrOfCols[s]->value();//number of new formats;
int * set_data=new int[sets_allocated];
    for (int i=0;i<sets_allocated;i++)
    {
    set_data[i]=-1;
    }
    //qDebug() << "Formats: Old=" << sum << "New=" << sum2;
int sets=formats.length()-1;
int * nr_of_columns=new int[sets+2];
n_formats.clear();
int index=0;
for (int i=0;i<sets_allocated;i++)
{
    if (i!=s)//the sets that is about to change
    {
    n_formats << formats.at(i);
    nr_of_columns[index++]=formats.at(i).length();
    }
}
//qDebug() << n_formats;
reCreateSelectors(sets,nr_of_columns,set_data);
setAllFormatSettings(n_formats);
delete[] nr_of_columns;
delete[] set_data;
}

void MultiDataFormatSelector::getAllFormatSettings(QList< QList<int> > & formats)
{
int index=0;
QList<int> tmp_list;
formats.clear();
    for (int i=0;i<sets_allocated;i++)
    {
    tmp_list.clear();
        for (int j=0;j<nr_of_formats_in_set[i];j++)
        {
        tmp_list << selectors[index++]->currentValue();
        }
    formats << tmp_list;
    }
}

void MultiDataFormatSelector::setAllFormatSettings(QList< QList<int> > & formats)
{
int index=0;
    for (int i=0;i<formats.length();i++)
    {
        for (int j=0;j<formats.at(i).length();j++)
        {
            if (index>=selectors_allocated) continue;
            selectors[index++]->setCurrentValue(formats.at(i).at(j));
        }
    }
}

ColumnTargetSelector::ColumnTargetSelector(QWidget * parent):QGroupBox(parent)
{
this->setTitle(tr("Select source columns"));
int nr=NUMBER_OF_SETTYPES;
QString * entr=new QString[nr];
for (int i=0;i<nr;i++)
{
    entr[i]=QString(set_types(i));
    entr[i]=entr[i].toUpper();
}
selSetType=new StdSelector(this,tr("Set type:"),nr,entr);
chkAddString=new QCheckBox(tr("String-column"),this);
nr=2;
entr[0]=tr("Index");
entr[1]=QString("1");
selColumn[0]=new StdSelector(this,QString("X")+tr(" from column:"),nr,entr);
selColumn[1]=new StdSelector(this,QString("Y")+tr(" from column:"),nr,entr);
for (int i=2;i<6;i++)
selColumn[i]=new StdSelector(this,QString("Y")+QString::number(i-1)+tr(" from column:"),nr,entr);
entr[0]=tr("None");
selString=new StdSelector(this,tr("Strings from column:"),nr,entr);
layout=new QGridLayout();
//layout->setMargin(0);
layout->setContentsMargins(0,0,0,0);
layout->setSpacing(STD_SPACING);
layout->addWidget(selSetType,0,0,1,1);
layout->addWidget(chkAddString,0,1,1,1);
for (int i=0;i<6;i++)
layout->addWidget(selColumn[i],1+i,0,1,2);
layout->addWidget(selString,7,0,1,2);
connect(selSetType,SIGNAL(currentIndexChanged(int)),this,SLOT(setTypeChanged(int)));
connect(chkAddString,SIGNAL(toggled(bool)),this,SLOT(stringColumnToggled(bool)));
this->setLayout(layout);
resetColumnCount(2);
stringColumnToggled(false);
setTypeChanged(SET_XY);
delete[] entr;
}

void ColumnTargetSelector::resetColumnCount(int c)
{
QString * entr=new QString[1+c];
int *vals=new int[1+c];
entr[0]=tr("Index");
vals[0]=-1;
for (int i=0;i<c;i++)
{
entr[1+i]=QString::number(1+i);
vals[1+i]=i;
}
for (int i=0;i<6;i++)
selColumn[i]->setNewEntries(1+c,entr,vals);
entr[0]=tr("NONE");
selString->setNewEntries(1+c,entr,vals);
delete[] entr;
}

void ColumnTargetSelector::stringColumnToggled(bool b)
{
(void)b;
selString->setVisible(chkAddString->isChecked());
//this->adjustSize();
}

void ColumnTargetSelector::setTypeChanged(int t)
{
int vals=settype_cols(t);
    for (int i=0;i<6;i++)
    {
        if (vals<=i)
        selColumn[i]->setVisible(false);
        else
        selColumn[i]->setVisible(true);
    }
//this->adjustSize();
}

void ColumnTargetSelector::getTargetSettings(int & settype,QList<int> & sourcecols)
{
settype=selSetType->currentValue();
int vals=settype_cols(settype);
for (int i=0;i<vals;i++)//all target columns needed for the new set
{
sourcecols << selColumn[i]->currentValue();
}
    if (chkAddString->isChecked()==true && selString->currentValue()>=0)
    {
    sourcecols << selString->currentValue();
    }
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
    cmbStyleSelect->comboType=COMBOTYPE_LINESTYLE;
    connect(cmbStyleSelect,SIGNAL(current_Index_Changed(int)),SLOT(changed(int)));
    layout=new QHBoxLayout;
    //layout->setMargin(2);
    layout->setContentsMargins(2,2,2,2);
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
    cmbFillPattern->comboType=COMBOTYPE_PATTERN;
    connect(cmbFillPattern,SIGNAL(current_Index_Changed(int)),SLOT(changed(int)));
    layout=new QHBoxLayout;
    //layout->setMargin(2);
    layout->setContentsMargins(2,2,2,2);
    layout->addWidget(lblText);
    layout->addWidget(cmbFillPattern);
    setLayout(layout);
    addPatternSelector(this);
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
    cmbOrderPattern->comboType=COMBOTYPE_ORDER;
    connect(cmbOrderPattern,SIGNAL(current_Index_Changed(int)),SLOT(changed(int)));
    layout=new QHBoxLayout;
    //layout->setMargin(2);
    layout->setContentsMargins(2,2,2,2);
    layout->addWidget(lblText);
    layout->addWidget(cmbOrderPattern);
    setLayout(layout);
addOrderSelector(this);
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
QLocale newLocale=(DecimalPointToUse=='.')?(*dot_locale):(*comma_locale);
    lblText=new QLabel(tr("Line width:"));
    spnLineWidth=new QDoubleSpinBox(this);
    spnLineWidth->setLocale(newLocale);
    spnLineWidth->setRange(0.0,MAX_LINEWIDTH);
    spnLineWidth->setDecimals(1);
    spnLineWidth->setSingleStep(0.5);
    connect(spnLineWidth,SIGNAL(valueChanged(double)),SLOT(changed(double)));
    layout=new QHBoxLayout;
    //layout->setMargin(2);
    layout->setContentsMargins(2,2,2,2);
    layout->addWidget(lblText);
    layout->addWidget(spnLineWidth);
    setLayout(layout);
addLineWidthSelector(this);
}

void LineWidthSelector::setText(QString txt)
{
    lblText->setText(txt);
}

void LineWidthSelector::setRange(double a,double b)
{
    spnLineWidth->setRange(a,b);
}

void LineWidthSelector::setSingleStep(double a)
{
    spnLineWidth->setSingleStep(a);
}

void LineWidthSelector::setDecimals(int d)
{
    spnLineWidth->setDecimals(d);
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
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->setSpacing(STD_SPACING);
    layout->addWidget(lblText);
    layout->addWidget(spnInt);
    setLayout(layout);
addIntSelector(this);
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

void stdIntSelector::setRange(int min,int max)
{
    spnInt->setRange(min,max);
}

menuIntSelector::menuIntSelector(QWidget * parent,QString label,int min,int max):QWidgetAction(parent)
{
empty=new QWidget(NULL);
layout=new QHBoxLayout();
layout->setSpacing(0);
//layout->setMargin(0);
layout->setContentsMargins(0,0,0,0);
selector=new stdIntSelector(empty,label,min,max);
connect(selector,SIGNAL(currentValueChanged(int)),this,SLOT(changed(int)));
layout->addWidget(selector);
empty->setLayout(layout);
setDefaultWidget(empty);
}

void menuIntSelector::changed(int i)
{
    emit(currentValueChanged(i));
}

int menuIntSelector::value(void)
{
return selector->value();
}

void menuIntSelector::setValue(int i)
{
selector->setValue(i);
}

void menuIntSelector::setRange(int min,int max)
{
selector->setRange(min,max);
}

stdLineEdit::stdLineEdit(QWidget * parent, QString label, bool accLaTeX):QWidget(parent)
{
    c1=c2=NULL;
    displayStd=true;
    acceptLaTeX=accLaTeX;
    lblText=new QLabel(label,this);
    lenText=new QLineEdit("0",this);
    connect(lenText,SIGNAL(returnPressed()),this,SLOT(ContentChanged()));
    layout=new QHBoxLayout();
    //layout->setMargin(2);
    layout->setContentsMargins(2,2,2,2);
    layout->addWidget(lblText);
    layout->addWidget(lenText);
    setLayout(layout);
    QPalette pal=lenText->palette();
    if (stdTextColor==NULL)
        stdTextColor=new QColor(pal.text().color());
    if (acceptLaTeX==true)
        add_LaTeX_Line(this);
    else
        add_non_LaTeX_Line(this);
//qDebug() << "ADDING this:" << this;
    setAcceptDrops(false);
}

stdLineEdit::~stdLineEdit()
{
//qDebug() << "REMOVING this:" << this;
   remove_Line(this);
}

void stdLineEdit::ContentChanged(void)
{
    emit(changed());
}

QString stdLineEdit::text(void)
{
#ifndef READ_UTF8_ONLY
    if (useQtFonts)
        return FileCodec->fromUnicode(lenText->text());
    else
#endif
        return lenText->text();
    /*///Deactivated LaTeX-to-Grace-conversion
static int pos,pos2,ret;
static QString str,str2;
static QString ref("$$");
static QString result,intermediate;
result=str=lenText->text();
    if (acceptLaTeX==true && activateLaTeXsupport==true)
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
#ifndef READ_UTF8_ONLY
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
#else
    lenText->setText(text);
#endif
}

void stdLineEdit::setPlainText(QString text)
{
    displayStd=true;
    c1=c2=NULL;
    lenText->setText(text);
}

void stdLineEdit::adjustHeightToFont(void)
{
//qDebug() << "fontSize=" << lenText->font().pixelSize() << "TextEdit-Height=" << lenText->height();
lenText->setMinimumHeight(lenText->font().pixelSize()+9);
}

void stdLineEdit::mouseReleaseEvent(QMouseEvent * e)
{
    if (acceptLaTeX==false) return;
    e->accept();
    QPoint pos=e->pos();
    if (lblText->rect().contains(pos)==true) clickedOnLabel();
}

void stdLineEdit::clickedOnLabel(void)
{
    if (acceptLaTeX==false || activateLaTeXsupport==FALSE) return;
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
        if (activateLaTeXsupport==TRUE)
        {
        complete_LaTeX_to_Grace_Translator(old_text);
        }
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
        if (acceptLaTeX==true && activateLaTeXsupport==TRUE)
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
    DynSetMemoryToLineEdit(t1,t2,c1,c2,displayStd,acceptLaTeX,lenText);
    /*
QString text(lenText->text());
displayStd=true;
char * te=new char[text.length()+8];
strcpy(te,text.toLatin1().constData());
t2=copy_string(t2,te);//t2 is always the original
    if (acceptLaTeX==true && activateLaTeXsupport==true)
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
Replace_Dec_Sep_In_Single_String(te);
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

void stdLineEdit::setDoubleValue(double val)
{
setDoubleValue(sformat,val);
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

void stdLineEdit::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void stdLineEdit::dropEvent(QDropEvent * e)
{
e->accept();
if (!e->mimeData()->hasUrls()) return;
int len=e->mimeData()->urls().length();
QList<QUrl> list=e->mimeData()->urls();
QString n_text;
n_text.clear();
for (int i=0;i<len;i++)
{
//cout << "file=" << list.at(i).toLocalFile().toLocal8Bit().constData() << endl;
n_text+=list.at(i).toLocalFile();
if (i<len-1) n_text+=QString(";");
}
lenText->setText(n_text);
emit(changed());
}

dirList::dirList(QWidget * parent,int type):QTreeView(parent)
{
    listtype=type;

    model = new QFileSystemModel();

    if (listtype==DIRLIST)
        model->setFilter(QDir::AllDirs);
    else
        model->setFilter(QDir::Files | QDir::NoDotAndDotDot);

    setModel(model);
    setRootIndex(model->index(QDir::currentPath()));
    model->setRootPath(QDir::currentPath());
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
    entr[2]=tr("C:")+QDir::separator();
    onHighestLevel=false;
#endif
    entr[3]=tr("QtGrace-dir");
    grpChDir=new QGroupBox(QString(""),this);
    selChdir=new StdSelector(grpChDir,tr("Change directory to:"),number,entr);
    cmdSetCwd=new QPushButton(tr("Set as cwd"),grpChDir);
    cmdSetCwd->setToolTip(tr("Set the current working directory\nto the selected directory"));
    //cmdGetCwd=new QPushButton(tr("Get cwd"),grpChDir);
    chkShowHiddenFiles=new QCheckBox(tr("Show hidden files"),this);

    lblFilter=new QLabel(tr("Filter:"),this);
    lblDirs=new QLabel(tr("Directories:"),this);
    lblFiles=new QLabel(tr("Files:"),this);
    //ledFilter=new QLineEdit("",this);
    ledFilter=new stdLineEdit(this,tr(""));
    ledFilter->setToolTip(tr("Directory and filter for files"));
    ledFilter->lblText->setVisible(false);
    //ledFilter->layout->setMargin(0);
    ledFilter->layout->setContentsMargins(0,0,0,0);
    ledFilter->layout->setSpacing(0);
    ledFilter->setAcceptDrops(true);
    DirList=new QListView(this);
    FileList=new QListView(this);
    modelDirs=new QFileSystemModel();
    modelDirs->setFilter(QDir::AllDirs);
    modelDirs->setNameFilterDisables(false);
    modelFiles=new QFileSystemModel();
    modelFiles->setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::Drives | QDir::System);
    modelFiles->setNameFilterDisables(false);
    DirList->setModel(modelDirs);
    FileList->setModel(modelFiles);

    currentDir=QDir::currentPath();//Just for initialization

    DirList->setRootIndex(modelDirs->index(currentDir));
    FileList->setRootIndex(modelFiles->index(currentDir));
    //cmdGoUp=new QPushButton(tr("GoUp"),this);
    //cmdGoUp=new QPushButton(qApp->style()->standardPixmap(QStyle::SP_FileDialogToParent),QString(""),this);
    cmdGoUp=new QPushButton(qApp->style()->standardIcon(QStyle::SP_FileDialogToParent),QString(""),this);
    cmdGoUp->setToolTip(tr("Go upwards to parent directory"));
    //cmdStdDialog=new QPushButton(tr("StdDialog"),this);
    //cmdStdDialog=new QPushButton(qApp->style()->standardPixmap(QStyle::SP_DirOpenIcon),QString(""),this);
    cmdStdDialog=new QPushButton(qApp->style()->standardIcon(QStyle::SP_DirIcon),QString(""),this);
    cmdStdDialog->setToolTip(tr("Open default file dialog of the operating system"));

    connect(cmdGoUp,SIGNAL(clicked()),SLOT(doGoUp()));
    connect(cmdStdDialog,SIGNAL(clicked()),SLOT(doStdDialog()));

    connect(selChdir->cmbSelect,SIGNAL(currentIndexChanged(int)),SLOT(currentDirChanged(int)));
    connect(selChdir->cmbSelect,SIGNAL(highlighted(int)),SLOT(currentDirChanged(int)));

    connect(FileList->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),SLOT(FileActivated(QModelIndex,QModelIndex)));
    connect(FileList,SIGNAL(clicked(const QModelIndex &)),SLOT(FileSelected(const QModelIndex &)));
    connect(FileList,SIGNAL(doubleClicked(const QModelIndex &)),SLOT(FileDoubleClicked(const QModelIndex &)));
    connect(DirList,SIGNAL(clicked(const QModelIndex &)),SLOT(DirSelected(const QModelIndex &)));
    connect(DirList,SIGNAL(doubleClicked(const QModelIndex &)),SLOT(DirDoubleClicked(const QModelIndex &)));
    //connect(ledFilter,SIGNAL(returnPressed()),SLOT(newFilterEntered()));
    connect(ledFilter,SIGNAL(changed()),SLOT(newFilterEntered()));
    connect(chkShowHiddenFiles,SIGNAL(stateChanged(int)),SLOT(toggleHiddenFile(int)));
    connect(cmdSetCwd,SIGNAL(clicked()),SLOT(doSetCWD()));
    //connect(cmdGetCwd,SIGNAL(clicked()),SLOT(doGetCWD()));

    layout0=new QHBoxLayout;
    //layout0->setMargin(2);
    layout0->setContentsMargins(2,2,2,2);
    layout0->addWidget(selChdir);
    layout0->addWidget(cmdSetCwd);
    //layout0->addWidget(cmdGetCwd);
    grpChDir->setLayout(layout0);

    layout=new QGridLayout();
    //layout->setMargin(2);
    layout->setContentsMargins(2,2,2,2);
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

    cmdSetCwd->setAutoDefault(false);
    cmdSetCwd->setDefault(false);
    cmdGoUp->setAutoDefault(false);
    cmdGoUp->setDefault(false);
    cmdStdDialog->setAutoDefault(false);
    cmdStdDialog->setDefault(false);
    changeIconSizes();
}

void FileSelector::FileActivated(const QModelIndex & index,const QModelIndex & index2)
{
    (void)index2;
    FileSelected(index);
}

void FileSelector::FileSelected(const QModelIndex & index )
{
    if (doubleclickfile==true)
    {
        doubleclickfile=false;
        return;
    }
    selectedFile=index.data().toString();
//qDebug() << "FileSelected (raw)=" << selectedFile;
    QString toEmit=QDir::cleanPath(currentDir+separator+selectedFile);
//qDebug() << "FileSelected (path complete)=" << toEmit;
    toEmit=QDir::toNativeSeparators(toEmit);
//qDebug() << "FileSelected (path corrected)=" << toEmit;
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
    newFilter=QDir::toNativeSeparators(newFilter);
    filter=newFilter;
    ledFilter->lenText->setText(filter);
    emit(newSelection(newDir));
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
    newFilter=QDir::toNativeSeparators(newFilter);
    filter=newFilter;
    ledFilter->lenText->setText(filter);

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
    modelDirs->setRootPath(currentDir);
    modelFiles->setRootPath(currentDir);
    DirList->setRootIndex(modelDirs->index(currentDir));
    FileList->setRootIndex(modelFiles->index(currentDir));
}

void FileSelector::newFilterEntered(void)
{
    ///test for "exists"???
    QString entered=ledFilter->lenText->text();

    if (entered.isEmpty())
    {
    ledFilter->lenText->setText(QDir::toNativeSeparators(QString(user_home_dir)+QDir::separator()+QString("*")));
    entered=ledFilter->lenText->text();
    }

    QDir d1(entered);
    QChar last_char=entered.at(entered.length()-1);
#ifdef WINDOWS_SYSTEM
    onHighestLevel=false;
#endif
    if ( !entered.contains(QString("*")) && !entered.contains(QString("?")) && last_char==separator.at(0))
    {
        filterExtension=QString("*");
        ledFilter->lenText->setText(QDir::toNativeSeparators(entered + filterExtension));
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
//qDebug() << "newFilterEntered=" << filter;
    showFilesLikeFilter();
    QFileInfo fi1(filter);
emit(newSelection(fi1.absoluteDir().absolutePath()+QDir::separator()));
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
if (fi.suffix().isEmpty()) ext=QString("*.agr");
QString n_dir=QDir::toNativeSeparators(fi.absolutePath());
setFilterFromExtern(n_dir,ext);

QModelIndex mi=modelFiles->index(n_file);
/// qDebug() << "Extern: file=" << n_file << " index=" << mi.row() << "" << modelFiles->rowCount();
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
#ifdef WINDOWS_SYSTEM
    onHighestLevel=false;
#endif
    switch (i)
    {
    case 0://CWD
        currentDir=QString::fromLocal8Bit(get_workingdir());
        currentDir.chop(1);
        break;
    case 1://HOME
        currentDir=QString::fromLocal8Bit(get_userhome());
        currentDir.chop(1);
        break;
    case 2://'/'
#ifdef WINDOWS_SYSTEM
        onHighestLevel=true;
        currentDir=QString("C:");//+QDir::separator();
#else
        currentDir=QString("");
#endif
        break;
    case 3:
        currentDir=QString::fromLocal8Bit(qt_grace_share_dir);
        break;
    }
    filter=QDir::toNativeSeparators(QDir::cleanPath(currentDir+separator+filterExtension));
    filter=filter.replace(separator+separator,separator);
    ledFilter->setPlainText(filter);
    showFilesLikeFilter();
    QString tmp=QDir::toNativeSeparators(QDir::cleanPath(currentDir))+separator;
//qDebug() << "new Selection(tmp)=" << tmp << "currentDir=" << currentDir << "filterExtension=" << filterExtension;
    tmp=tmp.replace(separator+separator,separator);
    emit(newSelection(tmp));
}

void FileSelector::changeIconSizes(void)
{
cmdGoUp->setIconSize(QSize(16*toolBarSizeFactor,16*toolBarSizeFactor));
cmdStdDialog->setIconSize(QSize(16*toolBarSizeFactor,16*toolBarSizeFactor));
}

void FileSelector::GeneratePathWithExtension(QDir dir,QString & PathAndExtension,QString & DirectoryOnly)
{
    QString str=dir.cleanPath(dir.absolutePath());//.toLatin1();
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

/*
QString str2=currentDir;
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

    //qDebug() << "Up from" << currentDir << "upPossible=" << up_possible << "onHighestLevel=" << onHighestLevel;
    /*strcpy(strbuf,tdir.path().toLatin1());
cout << "updir2=#" << strbuf << "# possible=" << up_possible << endl;*/
#ifdef WINDOWS_SYSTEM
    if (up_possible==false || onHighestLevel==true)
#else
    if (up_possible==false)
#endif
    {
#ifdef WINDOWS_SYSTEM
        currentDirChanged(2);//the last "up" will go to "C:\"
#endif
        return;
    }

    currentDir=tdir.absolutePath();
    currentDir=QDir::cleanPath(currentDir);
    //qDebug() << "nextDirectoryUp=" << currentDir;
    QString newFilter,newPath;
    GeneratePathWithExtension(tdir,newFilter,newPath);
    //qDebug() << "newFilter=" << newFilter << "newPath=" << newPath;
    filter=newFilter;
    ledFilter->setPlainText(newFilter);
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
        s=QFileDialog::getOpenFileName(this,tr("Open File"),currentDir);
    else
        s=QFileDialog::getSaveFileName(this,tr("Save File"),currentDir);
    if (!s.isNull())
    {
        QFileInfo fi1(QDir::toNativeSeparators(s));
        QString suffix=fi1.completeSuffix();
        QString path_only=fi1.path();
        ledFilter->lenText->setText(fi1.absolutePath()+QDir::separator()+QString("*.")+fi1.suffix());
        setFilterFromExtern(path_only,suffix);
        setFileSelectionFromExtern(s);
        emit(newSelection(s));
        qApp->processEvents();
        emit(newSelectionManually());
        //qApp->processEvents();
        //emit(newSelection(s));
        return;
/// qDebug() << "StdDialog: s=" << s;
        /*lazy method of finding filename and directory*/
        ledFilter->lenText->setText(QDir::toNativeSeparators(s));
        newFilterEntered();
        //set and announce new selection and filters
        selectedFile=filter;
        //QFileInfo fi1(selectedFile);
        filterExtension=QString("*.")+fi1.completeSuffix();
        filter=currentDir+separator+filterExtension;
        emit(newSelection(selectedFile));
        //show selection again
        ///modify to do this only for existing files!!!
        ledFilter->lenText->setText(QDir::toNativeSeparators(filter));
        newFilterEntered();
/// qDebug() << "StdDialog: selectedFile=" << selectedFile;
        QModelIndex index=modelFiles->index(selectedFile);
        /*if (index.row()<0)
        {
        QModelIndex sav_index=index;
        index=modelFiles->index(fi1.fileName());
qDebug() << "StdDialog: Filename only=" << fi1.fileName();
        if (index.row()<0) index=sav_index;
        }*/
/// qDebug() << "index=" << index.row();
        FileList->setCurrentIndex(index);
        /*QString suffix=fi1.completeSuffix();
        QString path_only=fi1.path();
        setFilterFromExtern(path_only,suffix);
        setFileSelectionFromExtern(s);*/
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
    //layout->setMargin(2);
    layout->setContentsMargins(2,2,2,2);
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
    Replace_Dec_Sep_In_Single_String(te);
    ledStart->setText(te);
    te=ledStop->text();
    Replace_Dec_Sep_In_Single_String(te);
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
    simple_graph_selector=false;
    removeLastStdSelector();//this is needed, because this is derived from StdSelector and is therefore added to the list, but is by itself not a StdSelector
    lblText=new QLabel("Type:");
    cmbSelect=new QComboBox(this);
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
    //layout->setMargin(2);
    layout->setContentsMargins(2,2,2,2);
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
/*int row=index.row();
int col=index.column();
qDebug()  << "edit:" << row << "|" << col;*/
    bool ret=QTableView::edit(index,trigger,event);
    return ret;
}

void SetTableView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    (void)roles;
//qDebug() << "data changed";
    QTableView::dataChanged(topLeft, bottomRight);
}

SetTableModel::SetTableModel(int gno,int sno,int * colformat,int * colprecision,QObject * parent):QAbstractTableModel(parent)
{
    setnr=sno;
    graphnr=gno;
    col_format=colformat;
    col_precision=colprecision;
    blockUpdate=false;
}

bool SetTableModel::init(int gno,int sno)
{
    if (!is_valid_setno(gno,sno)) return false;
    setnr=sno;
    graphnr=gno;
    return true;
}

void SetTableModel::setStringsEnabled(bool a)
{
    showStrings=a;
}

int SetTableModel::rowCount(const QModelIndex & parent) const
{
    (void)parent;
    if (!is_valid_setno(graphnr,setnr))
        return 0;
    else
        return getsetlength(graphnr,setnr)+10;//10 empty lines
}

int SetTableModel::columnCount(const QModelIndex & parent) const
{
    (void)parent;
    if (!is_valid_setno(graphnr,setnr))
        return 0;
    else
    {
        if (showStrings==true)
        return 1+dataset_cols(graphnr,setnr);
        else
        return dataset_cols(graphnr,setnr);
    }
}

QVariant SetTableModel::data(const QModelIndex & index,int role) const
{
//if (blockUpdate==true) return QVariant();
char dummy[GR_MAXPATHLEN];
int row,col;
//unsigned long long dummy_size=0;
QVariant dat;
QString string_val;
#if QT_VERSION >= 0x060000
    dat=QVariant();
#else
    dat=QVariant(QVariant::Invalid);
#endif
    string_val.clear();
if (!is_valid_setno(graphnr,setnr) || !(role==Qt::DisplayRole || role==Qt::EditRole)) return dat;

    /*if (dummy_size<128)
    {
    N_DELETE2(dummy);
    dummy=new char[128];
    dummy_size=128;
    }*/

    row=index.row();
    col=index.column();
//qDebug() << "Data (display) row=" << row << "col=" << col << "role(Display=0/Edit=2)=" << role << flush;
    if (role==Qt::DisplayRole || role==Qt::EditRole)
    {
        //cout << "Data=VALID" << endl;
        if (row>=getsetlength(graphnr,setnr)) return QVariant(QString(""));
//qDebug() << "data: A";
            if (col>=settype_cols(dataset_type(graphnr, setnr)))//string
            {
                if (g[graphnr].p[setnr].data.s!=NULL)//orig_s
                {
                    if (g[graphnr].p[setnr].data.s[row]!=NULL)
                    {
//qDebug() << "data: B";
                        /*if (dummy_size<strlen(g[graphnr].p[setnr].data.orig_s[row])+1)
                        {
                        N_DELETE2(dummy);
                        dummy_size=strlen(g[graphnr].p[setnr].data.orig_s[row])+1;
//qDebug() << "increase size: " << dummy_size << flush;
                        dummy=new char[dummy_size];
                        }*/
                    //strcpy(dummy,g[graphnr].p[setnr].data.s[row]);
                    string_val=QString::fromUtf8(g[graphnr].p[setnr].data.orig_s[row]);
                    //qDebug() << "s=" << g[graphnr].p[setnr].data.s[row] << "orig_s=" << g[graphnr].p[setnr].data.orig_s[row];
                    //qDebug() << "UTF8: s=" << QString::fromUtf8(g[graphnr].p[setnr].data.s[row]) << "orig_s=" << QString::fromUtf8(g[graphnr].p[setnr].data.orig_s[row]);
                    }
                    else
                    {
                    //strcpy(dummy,"");
                    string_val=QString("");
                    }
                }
                else//no string
                {
                //strcpy(dummy,"");
                string_val=QString("");
                }
            ///string_val=QString(dummy);//QString::fromUtf8(dummy);
//qDebug() << "data: C";
            }
            else//data
            {
//qDebug() << "data: D";
                if (isnan(g[graphnr].p[setnr].data.ex[col][row]))
                {
                strcpy(dummy,"NaN");
                }
                else if (isinf(g[graphnr].p[setnr].data.ex[col][row]))
                {
                strcpy(dummy,"Inf");
                }
                else
                {
                //qDebug() << "data=" << g[graphnr].p[setnr].data.ex[col][row];
                construct_string_from_data(g[graphnr].p[setnr].data.ex[col][row],dummy,col_precision[col],col_format[col]);
                }
            }
//qDebug() << "End Data1, dummy=" << dummy;
            //qDebug() << "variant=" << QVariant(string_val) << "UTF8:" << string_val.toUtf8();
            if (col>=settype_cols(dataset_type(graphnr, setnr)))//string
            return QVariant(string_val);//return QVariant(string_val.toUtf8());
            else
            return QVariant(QString(dummy).toUtf8());
    }
    else
    {
//qDebug() << "End Data2 (invalid)" << flush;
#if QT_VERSION >= 0x060000
    return QVariant();
#else
    return QVariant(QVariant::Invalid);
#endif
    }
}

void construct_string_from_data(double value,char * target,int col_precision,int col_format)
{
char dummy[GR_MAXPATHLEN];
/*
static char setPrec[8];
static char out_format[16];
//qDebug << "construct: value=" << value << endl;
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
*/
strcpy(dummy,create_fstring(col_format,col_precision,value,LFORMAT_TYPE_PLAIN));
    if (strlen(dummy)>=GR_MAXPATHLEN)
    qDebug() << "DUMMY TOO SHORT! " << strlen(dummy);
switch (col_format)
{
case FORMAT_DDMMYY:
case FORMAT_MMDDYY:
case FORMAT_YYMMDD:
case FORMAT_MMYY:
case FORMAT_MMDD:
case FORMAT_MONTHDAY:
case FORMAT_DAYMONTH:
case FORMAT_MONTHS:
case FORMAT_MONTHSY:
case FORMAT_MONTHL:
case FORMAT_DAYOFWEEKS:
case FORMAT_DAYOFWEEKL:
case FORMAT_DAYOFYEAR:
case FORMAT_HMS:
case FORMAT_MMDDHMS:
case FORMAT_MMDDYYHMS:
case FORMAT_YYMMDDHMS:
//case FORMAT_MMSSLON:
//case FORMAT_MMSSLAT:
case FORMAT_YYYY:
case FORMAT_YYDYHMS:
;//do not change the separator
break;
default:
SetDecimalSeparatorToUserValue(dummy);
break;
}
strcpy(target,dummy);
}

bool SetTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
//if (blockUpdate==true) return false;
int row,col,ret;
double val;
bool ok,really_new_value,set_is_new,inStringColumn;
QVariant n_val;
int endpos,emptyresidual;
char oldval[GR_MAXPATHLEN],newval[GR_MAXPATHLEN];
int ret_datapoint_ok;
bool data_is_inf,data_is_nan;
QString str_val=value.toString();
    ok=true;
    if (!is_valid_setno(graphnr,setnr)) return false;//invalid set
    blockUpdate=true;

    really_new_value=false;
    //val=value.toDouble(&ok);
    row=index.row();
    col=index.column();
    inStringColumn=(col>=settype_cols(dataset_type(graphnr, setnr))?true:false);
//qDebug() << "InString=" << inStringColumn << "Row=" << row <<"Col=" << col;
    if (role==Qt::DisplayRole || !is_valid_setno(graphnr,setnr) || (ok==false && !inStringColumn))
    {
        blockUpdate=false;
        return false;
    }
///beginResetModel();
//qDebug() << "SetData gno=" << graphnr << "SetNr=" << setnr << "Row=" << row << "Col=" << col << "role=" << role << "DisplayRole=" << (int)Qt::DisplayRole << "EditRole=" << (int)Qt::EditRole << "ValidSet=" << is_valid_setno(graphnr,setnr) << flush;
    //oldval=new char[256];
    //newval=new char[2*value.toString().toLocal8Bit().length()+128];
    if (inStringColumn)
    {
    strcpy(newval,value.toString().toUtf8().constData());
    }
    else
    {
    strcpy(newval,value.toString().toLocal8Bit().constData());
    }
if (inStringColumn && g[graphnr].p[setnr].data.s==NULL)
{
//qDebug() << "initing Strings: ";
(void)init_set_string(graphnr,setnr);
emit(newStringColumn());
qApp->processEvents();
}

    if (row<getsetlength(graphnr,setnr))//data already exists --> do we need to override it?
    {
        if (inStringColumn)//we are in the String-column
        {
        //qDebug() << "s=" << g[graphnr].p[setnr].data.s;
        //strcpy(oldval,g[graphnr].p[setnr].data.s[row]);
        strcpy(oldval,g[graphnr].p[setnr].data.orig_s[row]);//orig_s is the UTF8-one
        }
        else
        {
            if (isnan(g[graphnr].p[setnr].data.ex[col][row]))
            {
            strcpy(oldval,"NaN");
            }
            else if (isinf(g[graphnr].p[setnr].data.ex[col][row]))
            {
            strcpy(oldval,"Inf");
            }
            else
            {
            construct_string_from_data(g[graphnr].p[setnr].data.ex[col][row],oldval,col_precision[col],col_format[col]);
            }
        }
        if (!strcmp(newval,oldval))
            really_new_value=false;
        else
            really_new_value=true;
    }
    else
        really_new_value=true;

    if (!inStringColumn)
    ReplaceDecimalSeparator(newval);

//qDebug() << "newValue=" << newval << "format?" << col_format[col];

    if (QString::compare(str_val,QString("NaN"),Qt::CaseInsensitive)==0)
    {
        val=NAN;
        ret=RETURN_SUCCESS;
    }
    else if (QString::compare(str_val,QString("Inf"),Qt::CaseInsensitive)==0)
    {
        val=INFINITY;
        ret=RETURN_SUCCESS;
    }
    else
    {
    ret=read_date_or_number_in_format(newval,col_format[col],TRUE,&val,&endpos,&emptyresidual);
//char test_c_string[512];
//sprintf(test_c_string,"%d",(int)(val));
//qDebug() << "value=" << test_c_string;

        if (ret==RETURN_FAILURE)
        {
        ret=guess_date_or_number(newval,TRUE,&val);
        //qDebug() << "ERROR: reading data in preset format failed, new format=" << ret;
        }
    }

    //qDebug() << "a) value=" << value.toString().toLatin1().constData() << endl;
    n_val=QVariant(QString(newval));
    //qDebug() << "b) value=" << n_val.toString().toLatin1().constData() << endl;
    if (really_new_value==false)
    {
        /*N_DELETE2(oldval);
        N_DELETE2(newval);*/
        blockUpdate=false;
        return true;//nothing has actually to be changed
    }
///val=n_val.toDouble(&ok);
    if (ok==false && !inStringColumn)
    {
        cerr << "Invalid double-conversion from #" << n_val.toString().toLatin1().constData() << "#" << endl;
        /*N_DELETE2(oldval);
        N_DELETE2(newval);*/
        blockUpdate=false;
        return false;//invalid conversion
    }

    if (row<getsetlength(graphnr,setnr))//change an existing data point
    {
        str_val=value.toString();
        int len=1,ncols;
        int * nrs=new int[2];
        nrs[0]=nrs[1]=row;
        Datapoint * po=new Datapoint[2];
        zero_datapoint(po+0);
        zero_datapoint(po+1);
        //int ret=get_datapoint(graphnr,setnr,row,&ncols,po);
//qDebug() << "Edit Datapoint in row=" << row;
    /*if (!inStringColumn)
    qDebug() << "Data: OldValue in set=" << g[graphnr].p[setnr].data.ex[col][row];
    else
    qDebug() << "String: OldValue in set=" << g[graphnr].p[setnr].data.s[row];*/
        ret_datapoint_ok=get_datapoint(graphnr,setnr,row,&ncols,po);//warning: this copies the ADRESS of the original string into po
//qDebug() << "ret_datapoint_ok=" << ret_datapoint_ok;
        if (ret_datapoint_ok!=RETURN_FAILURE)
        {
            if (!inStringColumn)
            {
            po[0].ex[col]=val;
            po[0].s=NULL;
            po[0].orig_s=NULL;
            }
            else//inStringColumn
            {
            po[0].s=copy_string(NULL,str_val.toLocal8Bit().constData());//we edit the string --> copy new data-string into po --> we do not edit the orignal string
            po[0].orig_s=copy_string(NULL,str_val.toUtf8().constData());
            complete_LaTeX_to_Grace_Translator(str_val);
            char * c_str_val=new char[8+4*str_val.length()];
            generate_string_Qt_aware(c_str_val,str_val);
            po[0].s=copy_string(po[0].s,c_str_val);
            delete[] c_str_val;
            }
        DataPointEdited(graphnr,setnr,nrs,len,po,2);
            if (!inStringColumn)
            {
            g[graphnr].p[setnr].data.ex[col][row]=val;
            }
            else//inStringColumn
            {
            //g[graphnr].p[setnr].data.s[row]=copy_string(g[graphnr].p[setnr].data.s[row],value.toString().toLocal8Bit().constData());
            //g[graphnr].p[setnr].data.orig_s[row]=copy_string(g[graphnr].p[setnr].data.orig_s[row],value.toString().toUtf8().constData());
            g[graphnr].p[setnr].data.s[row]=copy_string(g[graphnr].p[setnr].data.s[row],po[0].s);
            g[graphnr].p[setnr].data.orig_s[row]=copy_string(g[graphnr].p[setnr].data.orig_s[row],po[0].orig_s);
            }
            ok=true;
        }
        else
        {
            ok=false;
        }
    /*if (!inStringColumn)
    qDebug() << "Data: NewValue in set=" <<  g[graphnr].p[setnr].data.ex[col][row] << "newval=" << newval;
    else
    qDebug() << "String: NewValue in set=" <<  g[graphnr].p[setnr].data.s[row] << "newval=" << newval;*/
        if (inStringColumn)//if we do not(!) edit the string --> the string is the original --> do not delete the string
        {
        xfree(po[0].s);//we only delete the string if it is new (only if we edit in the string column)
        xfree(po[0].orig_s);//we only delete the string if it is new (only if we edit in the string column)
        }
//qDebug() << "A";
        delete[] nrs;
//qDebug() << "B";
        delete[] po;
//qDebug() << "C" << flush;
//qDebug() << "A1 dirty=" << is_dirtystate();
set_dirtystate();
//qDebug() << "B1 dirty=" << is_dirtystate();
    }
    else//adding rows (filled with zeros) and set the last one to the new value
    {
        str_val=value.toString();
//qDebug() << "Add Datapoint in row=" << row;
        if (getsetlength(graphnr,setnr)<=0)
            set_is_new=true;
        else
            set_is_new=false;
        Datapoint dpoint;

        zero_datapoint(&dpoint);
        if (g[graphnr].p[setnr].data.s!=NULL)
        {
        dpoint.s=copy_string(NULL,"");
        dpoint.orig_s=copy_string(NULL,"");
        }
        int len=row-getsetlength(graphnr,setnr)+1;//,ncols;
        beginInsertRows(index, getsetlength(graphnr,setnr), getsetlength(graphnr,setnr)+len);
        ///len is actually the number of NEW lines (not the number of existing lines!)
        int counter=0;
            /*if (len<2)
            qDebug() << "WARNING: len=" << len;
            else
            qDebug() << "OK: len=" << len;*/
        int * nrs=new int[len+2<=1?128:len+2];
        Datapoint * po=new Datapoint[len+2<=1?128:len+2];
        for (int i=0;i<len;i++)//init every new data with zero
        {
            zero_datapoint(po+i);
            if (g[graphnr].p[setnr].data.s!=NULL)
            {
            po[i].s=copy_string(po[i].s,"");
            po[i].orig_s=copy_string(po[i].orig_s,"");
            }
        }
//qDebug() << "row=" << row << "col=" << col << "len=" << len << "inStringColumn=" << inStringColumn << "g[graphnr].p[setnr].data.s=" << g[graphnr].p[setnr].data.s << "SetLength()=" << getsetlength(graphnr,setnr);
            if (inStringColumn)//enter the new data into the datapoint
            {
            po[len-1].s=copy_string(po[len-1].s,str_val.toLocal8Bit().constData());
            po[len-1].orig_s=copy_string(po[len-1].orig_s,str_val.toUtf8().constData());
            complete_LaTeX_to_Grace_Translator(str_val);
            char * c_str_val=new char[8+4*str_val.length()];
            generate_string_Qt_aware(c_str_val,str_val);
            po[len-1].s=copy_string(po[len-1].s,c_str_val);
            delete[] c_str_val;
            }
            else
            {
            po[len-1].ex[col]=val;
            }

        for (int i=getsetlength(graphnr,setnr);i<=row;i++)
        {
            nrs[counter++]=i;
            add_point_at(graphnr,setnr, i, &dpoint);//add empty point(s) to the set
        }
//qDebug() << "new setlength=" << getsetlength(graphnr,setnr);
            if (inStringColumn)
            {
            //g[graphnr].p[setnr].data.s[row]=copy_string(g[graphnr].p[setnr].data.s[row],value.toString().toLocal8Bit().constData());
            //g[graphnr].p[setnr].data.orig_s[row]=copy_string(g[graphnr].p[setnr].data.orig_s[row],value.toString().toUtf8().constData());
            g[graphnr].p[setnr].data.s[row]=copy_string(g[graphnr].p[setnr].data.s[row],po[len-1].s);
            g[graphnr].p[setnr].data.orig_s[row]=copy_string(g[graphnr].p[setnr].data.orig_s[row],po[len-1].orig_s);
            }
            else
            {
            g[graphnr].p[setnr].data.ex[col][row]=val;
            }

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
//qDebug() << "A2 dirty=" << is_dirtystate();
set_dirtystate();
//qDebug() << "B2 dirty=" << is_dirtystate();
            for (int i=0;i<len;i++)
            {
            xfree(po[i].s);
            xfree(po[i].orig_s);
            }
            xfree(dpoint.s);
            xfree(dpoint.orig_s);
        delete[] nrs;
        delete[] po;
    endInsertRows();
    ok=true;
    }
/*qDebug() << "D";
    N_DELETE2(oldval);
    N_DELETE2(newval);*/
//qDebug() << "E";

    //qApp->processEvents();
    //emit(item_entry_changed(row,col,really_new_value));
    //qApp->processEvents();
//qDebug() << "F";
//qDebug() << "End SetData, setlength=" << getsetlength(graphnr,setnr);
///endResetModel();
//insertRows(0,1,index);
    if (ok==true) emit(dataChanged(this->index(0,0),this->index(getsetlength(graphnr,setnr)+10,(inStringColumn==true?1:0)+settype_cols(dataset_type(graphnr, setnr)))));
    //(void)this->submit();
    ///mainWin->mainArea->completeRedraw();
    emit(row_or_col_count_changed());
    force_redraw_wo_dialog_updates();
    blockUpdate=false;
    update_set_lists(graphnr);
    update_explorer();
    //this->resetInternalData();
    //emit(item_entry_changed(row,col,really_new_value));
return ok;
}

QVariant SetTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
QVariant dat;
    dat=QVariant();//QVariant::Invalid);
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
            else if (section>=settype_cols(dataset_type(graphnr, setnr)))
            {
                dat=QVariant(tr("String"));
            }
            else
            {
                char dummy[8];
                sprintf(dummy,"Y%d",section-1);
                dat=QVariant((const char*)dummy);
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
//qDebug() << "insertingRows";
    beginInsertRows(parent,row,row+count-1);
    endInsertRows();
    return true;
}

bool SetTableModel::removeRows(int row, int count, const QModelIndex & parent)
{
//qDebug() << "removingRows";
    beginRemoveRows(parent,row,row+count-1);
    endRemoveRows();
    return true;
}

bool SetTableModel::insertColumns(int column, int count, const QModelIndex & parent)
{
    beginInsertColumns(parent,column,column+count);
    endInsertColumns();
    return false;
}

bool SetTableModel::removeColumns(int column, int count, const QModelIndex & parent)
{
    beginRemoveColumns(parent,column,column+count);
    endRemoveColumns();
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

int GetOptionChoice(AlignmentSelector * s)
{
    return s->currentValue();
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

void SetOptionChoice(AlignmentSelector * sel,int v)
{
    sel->setCurrentValue(v);
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
    sel->setValue(v);
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
    sel->setValue(int(rint_2(v)));
}

void SetAngleChoice(stdSlider * sel,double v)
{
    sel->setValue(int(rint_2(v)));
}

void SetTextString(stdLineEdit * led,const char * st)
{
    led->setText(st);
}

void SetLabel(QLabel * lab,const char * st)
{
    lab->setText(QString(st));
}

void xv_setstr(stdLineEdit * led,const char * text)
{
    led->setText(QString(text));
}

void xv_setstr(QLineEdit * led,const char * text)
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

askForRegion::askForRegion(QWidget * parent):QDialog(parent)
{
setWindowIcon(QIcon(*GraceIcon));
setWindowTitle(tr("QtGrace: Select Region"));
    layout=new QGridLayout();
    layout->setSpacing(STD_SPACING);
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    cmdOK=new QPushButton(tr("OK"),this);
    cmdCancel=new QPushButton(tr("Cancel"),this);
    int number=6;
    QString entr[6];
    entr[0]=tr("Region 0");//no none here --> only real regions here (Cancel==None)
    entr[1]=tr("Region 1");
    entr[2]=tr("Region 2");
    entr[3]=tr("Region 3");
    entr[4]=tr("Region 4");
    entr[5]=tr("Inside graph");
    selRegion=new StdSelector(this,tr("Restriction:"),number,entr);
    selRegion->setCurrentIndex(0);
    chkInvert=new QCheckBox(tr("Negated"),this);
    chkInvert->setChecked(FALSE);
    Region=0;
    Negated=0;
    layout->addWidget(selRegion,0,0,1,2);
    layout->addWidget(chkInvert,0,2,1,1);
    layout->addWidget(cmdOK,1,0,1,1);
    layout->addWidget(cmdCancel,1,2,1,1);
    setLayout(layout);
    connect(cmdOK,SIGNAL(clicked()),SLOT(doOk()));
    connect(cmdCancel,SIGNAL(clicked()),SLOT(doCancel()));
}

void askForRegion::doOk(void)
{
    switch (selRegion->currentIndex())
    {
    case 0:
    Region=RESTRICT_REG0;
    break;
    case 1:
    Region=RESTRICT_REG1;
    break;
    case 2:
    Region=RESTRICT_REG2;
    break;
    case 3:
    Region=RESTRICT_REG3;
    break;
    case 4:
    Region=RESTRICT_REG4;
    break;
    case 5:
    default:
    Region=RESTRICT_WORLD;
    break;
    }
Negated = chkInvert->isChecked()==TRUE?1:0;
done(QDialog::Accepted);
}

void askForRegion::doCancel(void)
{
done(QDialog::Rejected);
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
    par->activateHighlighting();
}

void GraphPopup::doFocusTo(void)
{
    ListOfChanges.clear();
    sprintf(dummy,"with g%d",selected_no);
    ListOfChanges << QString(dummy);
    simple_draw_setting|=SIMPLE_DRAW_GRAPH_FOCUS;
    switch_current_graph(selected_no);
    par->update_number_of_entries_preserve_selection();
    //update();
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
    if (yesno(QObject::tr("Kill selected graph(s)?").toLocal8Bit().data(), NULL, NULL, NULL))
    {
        ListOfChanges.clear();
        int * graphs=new int[number_of_selected_graphs];
        for (int i = number_of_selected_graphs - 1; i >= 0; i--)
        {
            sprintf(dummy,"%s G%d",QObject::tr("kill").toLocal8Bit().constData(),selected_graphs[i]);
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

void createSmallSetList(QString & text,int nr,int * ids)
{
text.clear();
text+=QString("S");
for (int i=0;i<nr;i++)
{
    text+=QString::number(ids[i]);
    if (i>2 && i!=nr-1)
    {
        if (i<nr-2)
        text+=QString(",...");
        text+=QString(", S")+QString::number(ids[nr-1]);
        break;
    }
    else
    {
        if (i!=nr-1)
        text+=QString(", S");
    }
}
}

double read_first_number_from_text(QString text,bool & ok)
{
double val=0.0;
int len=text.length();
char * dummy=new char[2+len*3];
strcpy(dummy,text.toLocal8Bit().constData());
len=strlen(dummy);
char * endpos=dummy;
int index=0;
ok=false;
while (ok==false && index<len)
{
val=strtod(dummy+index,&endpos);
if (endpos>dummy+index && endpos!=NULL) ok=true;
index++;
}
delete[] dummy;
if (ok==false) val=0.0;
return val;
}

SetPopup::SetPopup(uniList *parent):QMenu(parent)
{
    par=parent;
    number_of_selected_sets=0;
    selected_sets=new int[2];

    CreateActions();
    setTearOffEnabled(FALSE);

    //Create Menu itself
    addAction(actLabel);
    //addAction(actSyncListContents);
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
    addAction(actAutoLabel);
    addSeparator();

    mnuDisplay=new QMenu(tr("Display options"),this);
    mnuDisplay->setTearOffEnabled(FALSE);

    mnuSelectorOperations=new QMenu(tr("Select &operations"),this);
    mnuSelectorOperations->setTearOffEnabled(FALSE);

    actViewIcons->setCheckable(true);
    actViewIcons->setChecked(FALSE);
    actViewSetComments->setCheckable(true);
    actViewSetComments->setChecked(FALSE);
    actViewSetLegends->setCheckable(true);
    actViewSetLegends->setChecked(FALSE);
    actShowDataLess->setCheckable(true);
    actShowDataLess->setChecked(FALSE);
    actShowHidden->setCheckable(true);
    actShowHidden->setChecked(TRUE);

    /*mnuSelectorOperations->addAction(actViewIcons);
    mnuSelectorOperations->addAction(actViewSetComments);
    mnuSelectorOperations->addAction(actViewSetLegends);
    mnuSelectorOperations->addAction(actShowDataLess);
    mnuSelectorOperations->addAction(actShowHidden);*/
    mnuDisplay->addAction(actViewIcons);
    mnuDisplay->addAction(actViewSetComments);
    mnuDisplay->addAction(actViewSetLegends);
    mnuDisplay->addAction(actShowDataLess);
    mnuDisplay->addAction(actShowHidden);

    //mnuSelectorOperations->addSeparator();
    mnuSelectorOperations->addAction(actSelectAll);
    mnuSelectorOperations->addAction(actUnselectAll);
    mnuSelectorOperations->addAction(actInvertSelection);
    mnuSelectorOperations->addAction(actSelectEven);
    mnuSelectorOperations->addAction(actSelectOdd);
    mnuSelectorOperations->addAction(actSelectNth);
    mnuSelectorOperations->addAction(actSelectInRegion);
    mnuSelectorOperations->addAction(actSelectVisible);
    mnuSelectorOperations->addAction(actSelectInvisible);
    mnuSelectorOperations->addSeparator();
    mnuSelectorOperations->addAction(actUpdate);

    mnuColor=new QMenu(tr("Set line color"),this);
    mnuColor->setTearOffEnabled(FALSE);
    mnuLineStyle=new QMenu(tr("Set linestyle"),this);
    mnuLineStyle->setTearOffEnabled(FALSE);
    mnuSymbol=new QMenu(tr("Set set symbols"),this);
    mnuSymbol->setTearOffEnabled(FALSE);

    actColors=NULL;
    actLineStyles=NULL;
    actSymbols=NULL;
    nr_of_linestyles=nr_of_symbols=nr_of_col_items=0;
    maincolors=NULL;

#if QT_VERSION >= 0x050000

#else
    colMapper=new QSignalMapper(this);
    connect(colMapper,SIGNAL(mapped(int)),this,SLOT(colorClicked(int)));
#endif
    generateColorMenu();
#if QT_VERSION >= 0x050000

#else
    lineStyleMapper=new QSignalMapper(this);
    connect(lineStyleMapper,SIGNAL(mapped(int)),this,SLOT(lineStyleClicked(int)));
#endif
    generateLinesMenu();
#if QT_VERSION >= 0x050000

#else
    symbolMapper=new QSignalMapper(this);
    connect(symbolMapper,SIGNAL(mapped(int)),this,SLOT(symbolsClicked(int)));
#endif
    generateSymbolsMenu();

    addMenu(mnuColor);
    addMenu(mnuLineStyle);
    addMenu(mnuSymbol);
    addSeparator();
    addMenu(mnuDisplay);
    addMenu(mnuSelectorOperations);

    mnuSimpleOperations=new QMenu(tr("Set operations"),this);
    mnuSimpleOperations->setTearOffEnabled(FALSE);
    mnuSimpleOperations->addAction(actSimpleSortXUp);
    mnuSimpleOperations->addAction(actSimpleSortXDown);
    mnuSimpleOperations->addAction(actSimpleReverse);
    mnuSimpleOperations->addAction(actSimpleJoin);
    mnuSimpleOperations->addAction(actSimpleRestrict);
    mnuSimpleOperations->addAction(actSimpleSwapXY);
    mnuSimpleOperations->addAction(actSimpleAverage);
    addMenu(mnuSimpleOperations);
}

SetPopup::~SetPopup()
{
    if (selected_sets)
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
    actViewSetLegends=new QAction(tr("View set legends"),this);
    connect(actViewSetLegends,SIGNAL(triggered()), this,SLOT(doViewSetLegends()));
    actViewIcons=new QAction(tr("Show Icons"),this);
    connect(actViewIcons,SIGNAL(triggered()), this,SLOT(doViewIcons()));
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
    actSelectInRegion=new QAction(tr("Select sets in region"),this);
    connect(actSelectInRegion,SIGNAL(triggered()), this,SLOT(doSelectInRegion()));
    actUpdate=new QAction(tr("Update"),this);
    connect(actUpdate,SIGNAL(triggered()), this,SLOT(doUpdate()));
    actCopyClipBoard=new QAction(tr("Copy"),this);
    connect(actCopyClipBoard,SIGNAL(triggered()),SLOT(doCopyClipBoard()));
    actPasteClipBoard=new QAction(tr("Paste"),this);
    connect(actPasteClipBoard,SIGNAL(triggered()),SLOT(doPasteClipBoard()));
    actStoreAppearance=new QAction(tr("Store appearance(s)"),this);
    connect(actStoreAppearance,SIGNAL(triggered()),SLOT(doStoreAppearance()));
    actApplyStoredAppearance=new QAction(tr("Apply stored appearance(s)"),this);
    connect(actApplyStoredAppearance,SIGNAL(triggered()),SLOT(doApplyStoredAppearance()));

    actSimpleSortXUp=new QAction(tr("Sort X ascending"),this);
    connect(actSimpleSortXUp,SIGNAL(triggered()),SLOT(doSimpleSortXUp()));
    actSimpleSortXDown=new QAction(tr("Sort X descending"),this);
    connect(actSimpleSortXDown,SIGNAL(triggered()),SLOT(doSimpleSortXDown()));
    actSimpleReverse=new QAction(tr("Reverse X"),this);
    connect(actSimpleReverse,SIGNAL(triggered()),SLOT(doSimpleReverse()));
    actSimpleJoin=new QAction(tr("Join sets"),this);
    connect(actSimpleJoin,SIGNAL(triggered()),SLOT(doSimpleJoin()));
    actSimpleRestrict=new QAction(tr("Restrict to inside graph"),this);
    connect(actSimpleRestrict,SIGNAL(triggered()),SLOT(doSimpleRestrict()));
    actSimpleSwapXY=new QAction(tr("Swap X with Y"),this);
    connect(actSimpleSwapXY,SIGNAL(triggered()),SLOT(doSimpleSwapXY()));
    actSimpleAverage=new QAction(tr("Average set"),this);
    connect(actSimpleAverage,SIGNAL(triggered()),SLOT(doSimpleAverage()));

    actAutoLabel=new QAction(tr("Auto-label set(s)"),this);
    connect(actAutoLabel,SIGNAL(triggered()),SLOT(doAutoLabel()));
    /*actSyncListContents=new QAction(tr("Sync source and destination"),this);
    connect(actSyncListContents,SIGNAL(triggered()),SLOT(doSyncList()));*/
}

void SetPopup::prepareForAction(void)
{
//qDebug() << "A Prepare for Action gno=" << selected_graph << " nr_sel_sets=" << number_of_selected_sets;
    if (par!=NULL)
    {
    selected_graph=par->gr_no;
    par->get_selection(&number_of_selected_sets,&selected_sets);
    }
    else
    {
    number_of_selected_sets=1;
    selected_graph=main_win_gr_no;
    if (selected_sets!=NULL) delete[] selected_sets;
    selected_sets=new int[2];
    selected_sets[0]=main_win_set_no;
    }
//qDebug() << "B Prepare for Action gno=" << selected_graph << " nr_sel_sets=" << number_of_selected_sets;
}

void SetPopup::finishAction(int preserveSelection)
{
//qDebug() << "Finish Action";
    bool savUpRun=updateRunning;
    updateRunning=false;
    force_redraw();
    updateRunning=savUpRun;
    hide();/// we should not need this
    if (par!=NULL)
    {
    par->update_number_of_entries();
        if (preserveSelection==DO_PRESERVE_SELECTION)
        par->set_new_selection(number_of_selected_sets,selected_sets);
    }
    else
    {
    mainWin->stop_highlighting();
    drawgraph();
    }
}

void SetPopup::doNothing(void)
{
//qDebug() << "Nothing";
    if (par!=NULL)
    {
    par->activateHighlighting();
    }
    else
    {
    ;//mainWin->start_highlighting();//should be highlighted anyway
    }
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
    prepareForAction();
    if (number_of_selected_sets<1) return;
QList<int> list1,list2;
    list1.clear();
    list2.clear();
    for (int i=0;i<number_of_selected_sets;i++)
    {
    list1 << selected_sets[i];
    }
int nr_of_sets=number_of_sets(selected_graph);
    for (int i=0;i<nr_of_sets;i++)
    {
    if (!list1.contains(i)) list2 << i;
    }
    list1 << list2;//add the old values at the end
    reSortSets(selected_graph,list1);
    SetsReorderd(selected_graph,list1);
    QString text(tr("Set"));
    int mult=0;
    if (number_of_selected_sets>1)
    {
    mult=1;
    text+=tr("s");
    }
    else
    {
    text+=QString(" G")+QString::number(selected_graph)+QString(".S")+QString::number(selected_sets[0]);
    }
    text+=tr(" sent to back");
    addAditionalDescriptionToLastNode(-1,text,QString(),mult);
    /*for (int i=number_of_selected_sets-1;i>=0;i--)
    {
        ChangeSetOrdering(selected_graph,selected_sets[i],PUSH_SET_TOBACK);
        pushset(selected_graph, selected_sets[i], PUSH_SET_TOBACK);
    }  */
    mainWin->mainArea->completeRedraw();
    if (par!=NULL)
    {
    int *n_gnos=new int[number_of_selected_sets+1];
    for (int i=0;i<number_of_selected_sets;i++) n_gnos[i]=i;
    par->set_new_selection(number_of_selected_sets,n_gnos);
    delete[] n_gnos;
    par->new_selection();
    }
}

void SetPopup::doBringToFront(void)
{
    prepareForAction();
    if (number_of_selected_sets<1) return;
QList<int> list1,list2;
    list1.clear();
    list2.clear();
    for (int i=0;i<number_of_selected_sets;i++)
    {
    list1 << selected_sets[i];
    }
int nr_of_sets=number_of_sets(selected_graph);
    for (int i=0;i<nr_of_sets;i++)
    {
    if (!list1.contains(i)) list2 << i;
    }
    list2 << list1;//add the old values at the start
    reSortSets(selected_graph,list2);
    SetsReorderd(selected_graph,list2);
    QString text(tr("Set"));
    int mult=0;
    if (number_of_selected_sets>1)
    {
    mult=1;
    text+=tr("s");
    }
    else
    {
    text+=QString(" G")+QString::number(selected_graph)+QString(".S")+QString::number(selected_sets[0]);
    }
    text+=tr(" brought to front");
    addAditionalDescriptionToLastNode(-1,text,QString(),mult);
    /*for (int i=0;i<number_of_selected_sets;i++)
    {
        ChangeSetOrdering(selected_graph,selected_sets[i],PUSH_SET_TOFRONT);
        pushset(selected_graph, selected_sets[i], PUSH_SET_TOFRONT);
    }*/
    mainWin->mainArea->completeRedraw();
    if (par!=NULL)
    {
    int *n_gnos=new int[number_of_selected_sets+1];
    for (int i=0;i<number_of_selected_sets;i++) n_gnos[i]=new_set_no-i;
    par->set_new_selection(number_of_selected_sets,n_gnos);
    delete[] n_gnos;
    par->new_selection();
    }
}

void SetPopup::doDuplicate(void)
{
int setno,index;
    prepareForAction();
    if (number_of_selected_sets<1) return;
int * gnos=new int[number_of_selected_sets+2];
int * snos=new int[number_of_selected_sets+2];
    index=0;
    for (int i=0;i<number_of_selected_sets;i++)
    {
        setno = nextset(selected_graph);
        do_copyset(selected_graph, selected_sets[i], selected_graph, setno);
        gnos[index]=selected_graph;
        snos[index++]=setno;
    }
    SetsCreated(index,gnos,snos,UNDO_COMPLETE);
delete[] gnos;
delete[] snos;
/*    mainWin->mainArea->completeRedraw();
if (par!=NULL)
par->set_new_selection(number_of_selected_sets,selected_sets);*/
finishAction(DO_PRESERVE_SELECTION);
}

void SetPopup::doKill(void)
{
    prepareForAction();
    if (number_of_selected_sets<1) return;
    if (yesno(tr("Kill selected set(s)?").toLocal8Bit().data(), NULL, NULL, NULL))
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
        if (autoPackSets)
        {
            doPackAllSets();
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
    if (number_of_selected_sets<1) return;
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
    prepareForAction();
    if (number_of_selected_sets==2)
    {
        swapset(selected_graph, selected_sets[0], selected_graph, selected_sets[1]);
        UndoSwapSets(selected_graph, selected_sets[0], selected_graph, selected_sets[1]);
    }
        if (par!=NULL)
        par->update_number_of_entries();
finishAction(DO_PRESERVE_SELECTION);
}

void SetPopup::doPackAllSets(void)
{
    prepareForAction();
    SetsAboutToBePacked(selected_graph);
    packsets(selected_graph);
        if (par!=NULL)
        par->update_number_of_entries();
    //mainWin->mainArea->completeRedraw();
    finishAction(DO_PRESERVE_SELECTION);
}

void SetPopup::doEditInSpreadSheet(void)
{
    prepareForAction();
    for (int i=0;i<number_of_selected_sets;i++)
    {
        showSetInSpreadSheet(selected_graph,selected_sets[i]);
    }
}

void SetPopup::doEditInTextEditor(void)
{
    prepareForAction();
    int gno=this->selected_graph;
    int setno=-1;
    if (this->number_of_selected_sets>0)
    {
        setno=this->selected_sets[0];
    }
    if (setno!= -1)
    {
        setcomment(gno, setno,tr("Editor").toLocal8Bit().data());
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
    FormLoadAndEvaluate->parentList=this->par;
    FormLoadAndEvaluate->show();
    FormLoadAndEvaluate->raise();
    FormLoadAndEvaluate->activateWindow();
}

void SetPopup::doCreateInSpreadsheet(void)
{
    prepareForAction();
    int setno,gno=selected_graph;
    if ((setno = nextset(gno)) != -1) {
        setcomment(gno, setno, tr("Editor").toLocal8Bit().data());
        set_set_hidden(gno, setno, FALSE);
        //create_ss_frame(gno, setno);
        showSetInSpreadSheet(gno,setno);
    frmSpreadSheet * foundSpreadsheet=findOpenSpreadSheet(gno,setno);
        if (foundSpreadsheet!=NULL)
        foundSpreadsheet->parentList=this->par;
    } else {
        ;///cout << "error" << endl;
    }
}

void SetPopup::doCreateInTextEditor(void)
{
    prepareForAction();
    int gno=this->selected_graph;
    int setno;
    if ((setno = nextset(gno)) != -1)
    {
        setcomment(gno, setno, tr("Editor").toLocal8Bit().data());
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
    if (par!=NULL)
    par->selectAll();
}

void SetPopup::doUnSelectAll(void)
{
    if (par!=NULL)
    par->clearSelection();
}

void SetPopup::doSelectEven(void)
{
    if (par==NULL) return;
QModelIndexList list=par->selectedIndexes();
int number=list.size();
    int * ne_sel=new int[par->count()+2];
    int counter=0;
    bool in_list;
    for (int i=0;i<par->count();i++)
    {
        if (par->entries[i]==-1) continue;
        in_list=false;
        for (int j=0;j<number;j++)
        {
            if (list.at(j).row()==i)
            {
            in_list=true;
            break;
            }
        }
        if (par->entries[i]%2==0 || in_list==true)//index is even
        {
            ne_sel[counter++]=par->entries[i];
        }
    }
    par->set_new_selection(counter,ne_sel);
    par->new_selection();
    delete[] ne_sel;
}

void SetPopup::doSelectOdd(void)
{
    if (par==NULL) return;
QModelIndexList list=par->selectedIndexes();
int number=list.size();
    int * ne_sel=new int[par->count()+2];
    int counter=0;
    bool in_list;
    for (int i=0;i<par->count();i++)
    {
        if (par->entries[i]==-1) continue;
        in_list=false;
        for (int j=0;j<number;j++)
        {
            if (list.at(j).row()==i)
            {
            in_list=true;
            break;
            }
        }
        if (par->entries[i]%2!=0 || in_list==true)//index is odd
        {
            ne_sel[counter++]=par->entries[i];
        }
    }
    par->set_new_selection(counter,ne_sel);
    par->new_selection();
    delete[] ne_sel;
}

void SetPopup::doSelectVisible(void)
{
    if (par==NULL) return;
QModelIndexList list=par->selectedIndexes();
int number=list.size();
    int * ne_sel=new int[par->count()+2];
    int counter=0;
    bool in_list;
    for (int i=0;i<par->count();i++)
    {
        if (par->entries[i]==-1) continue;
        in_list=false;
        for (int j=0;j<number;j++)
        {
            if (list.at(j).row()==i)
            {
            in_list=true;
            break;
            }
        }
        if (is_set_hidden(par->gr_no,par->entries[i])==FALSE || in_list==true)//set is visible
        {
            ne_sel[counter++]=par->entries[i];
        }
    }
    par->set_new_selection(counter,ne_sel);
    par->new_selection();
    delete[] ne_sel;
}

void SetPopup::doSelectInVisible(void)
{
    if (par==NULL) return;
QModelIndexList list=par->selectedIndexes();
int number=list.size();
    int * ne_sel=new int[par->count()+2];
    int counter=0;
    bool in_list;
    for (int i=0;i<par->count();i++)
    {
        if (par->entries[i]==-1) continue;
        in_list=false;
        for (int j=0;j<number;j++)
        {
            if (list.at(j).row()==i)
            {
            in_list=true;
            break;
            }
        }
        if (is_set_hidden(par->gr_no,par->entries[i])==TRUE || in_list==true)//set is visible
        {
            ne_sel[counter++]=par->entries[i];
        }
    }
    par->set_new_selection(counter,ne_sel);
    par->new_selection();
    delete[] ne_sel;
}

void SetPopup::doSelectNth(void)
{
    if (par==NULL) return;
bool ok;
int	n=QInputDialog::getInt(this,tr("Index"),tr("Period n="),2,0,par->number_of_entries,1,&ok);
if (ok==false) return;
int offset=QInputDialog::getInt(this,tr("Offset"),tr("Start index n0="),0,0,par->number_of_entries,1,&ok);
if (ok==false) return;
if (n==0 || n==1)
{
doSelectAll();
return;
}
QModelIndexList list=par->selectedIndexes();
int number=list.size();
    int * ne_sel=new int[par->count()+2];
    int counter=0;
    bool in_list;
    for (int i=0;i<par->count();i++)
    {
        if (par->entries[i]==-1) continue;
        in_list=false;
        for (int j=0;j<number;j++)
        {
            if (list.at(j).row()==i)
            {
            in_list=true;
            break;
            }
        }
        if ((par->entries[i]+n-1-offset)%n==n-1 || in_list==true)
        {
            ne_sel[counter++]=par->entries[i];
        }
    }
    par->set_new_selection(counter,ne_sel);
    par->new_selection();
    delete[] ne_sel;
}

void SetPopup::doSelectInRegion(void)
{
    if (par==NULL) return;
int ok;
int reg,inv;//region,inverted
askForRegion * afr=new askForRegion(this);
ok=afr->exec();
if (ok==QDialog::Rejected)
{
delete afr;
//qDebug() << "Rejected";
return;
}
reg=afr->Region;
inv=afr->Negated;
//qDebug() << "Accepted: Region=" << reg << "neg=" << inv;
delete afr;
if (reg==RESTRICT_NONE)
{
return;//without restrictions this makes no sense
}
else if (reg==RESTRICT_WORLD)
{
reg=MAXREGION;
}
QModelIndexList list=par->selectedIndexes();
int number=list.size();
    int * ne_sel=new int[par->count()+2];
    int counter=0;
    bool in_list;
    for (int i=0;i<par->count();i++)
    {
        if (par->entries[i]==-1) continue;
        in_list=false;
        for (int j=0;j<number;j++)
        {
            if (list.at(j).row()==i)
            {
            in_list=true;
            break;
            }
        }
        if (is_set_in_region(par->gr_no,par->entries[i],reg,inv)==TRUE || in_list==true)
        {
            ne_sel[counter++]=par->entries[i];
            //qDebug() << "Set" << par->entries[i] << "IS IN graph";
        }
        /*else
        {
            qDebug() << "Set" << par->entries[i] << "is NOT in graph";
        }*/
    }
    par->set_new_selection(counter,ne_sel);
    par->new_selection();
    delete[] ne_sel;
}

void SetPopup::doUpdate(void)
{
    if (par==NULL) return;
    par->update_number_of_entries_preserve_selection();
}

void SetPopup::doInvertSelection(void)
{
    if (par==NULL) return;
    int number=par->count()-number_of_selected_sets;
    int * n_selected_sets=new int[number+2];
    bool in_list;
    in_list=false;
    for (int i=0;i<number_of_selected_sets;i++)//check for the "All sets"-entry
    {
        if (selected_sets[i]==-1)
        {
        in_list=true;
        break;
        }
    }
    if (in_list==true)//all-sets-entry is selected --> clear selection
    {
    par->clearSelection();
    }
    else
    {
        number=0;
    for (int i=0;i<par->count();i++)
    {
        if (par->entries[i]==-1) continue;//ignore "All sets"-entry
        in_list=false;
        for (int j=0;j<number_of_selected_sets;j++)
        {
            if (selected_sets[j]==par->entries[i])
            {
                in_list=true;
                break;
            }
        }
        if (in_list==false)
        {
            n_selected_sets[number++]=par->entries[i];
        }
    }
    par->set_new_selection(number,n_selected_sets);
    par->new_selection();
    }
    delete[] n_selected_sets;
}

void SetPopup::doShowHidden(void)
{
    if (par!=NULL)
    {
    par->show_hidden=!par->show_hidden;
    par->update_number_of_entries_preserve_selection();
    }
}

void SetPopup::doShowDataLess(void)
{
    if (par!=NULL)
    {
    par->show_data_less=!par->show_data_less;
    par->update_number_of_entries_preserve_selection();
    }
}

void SetPopup::doViewSetComments(void)
{
    if (par!=NULL)
    {
    par->show_comments=!par->show_comments;
    par->update_number_of_entries_preserve_selection();
    }
}

void SetPopup::doViewSetLegends(void)
{
    if (par!=NULL)
    {
    par->show_legends=!par->show_legends;
    par->update_number_of_entries_preserve_selection();
    }
}

void SetPopup::doViewIcons(void)
{
    if (par!=NULL)
    {
    par->show_icons=!par->show_icons;
    par->update_number_of_entries_preserve_selection();
    }
}

void SetPopup::doMove12(void)
{
    if (number_of_selected_sets==2 && par!=NULL)
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
    if (number_of_selected_sets==2 && par!=NULL)
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
    if (number_of_selected_sets==2 && par!=NULL)
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
    if (number_of_selected_sets==2 && par!=NULL)
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
    prepareForAction();
    if (number_of_selected_sets<1) return;
    QString text;
    text.clear();
    char dummy[2048];
    int gno,sno,col;
    for (int i=0;i<number_of_selected_sets;i++)
    {
        gno=selected_graph;
        sno=selected_sets[i];
        col=dataset_cols(gno,sno);
        for (int k=0;k<g[gno].p[sno].data.len;k++)
        {
            for (int j=0;j<col;j++)
            {
                //sprintf(dummy,"%.8g\t",g[gno].p[sno].data.ex[j][k]);
                //text+=QString(dummy);
                sprintf(dummy,sformat,g[gno].p[sno].data.ex[j][k]);
                text+=QString(dummy);
                if (j<col-1)
                text+=QString("\t");
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
//find out how many sets have been selected
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

void SetPopup::doSimpleSortXUp(void)
{
    prepareForAction();
int * sel_gr=NULL;
sel_gr=new int[number_of_selected_sets+1];
for (int i=0;i<number_of_selected_sets;i++) sel_gr[i]=selected_graph;
int ret=SmallSetOperations(number_of_selected_sets,sel_gr,selected_sets,DATASETOP_SORT,DATA_X,0,1,NULL);
if (sel_gr!=NULL) delete[] sel_gr;
if (ret==RETURN_SUCCESS)
mainWin->mainArea->completeRedraw();
}

void SetPopup::doSimpleSortXDown(void)
{
    prepareForAction();
int * sel_gr=NULL;
sel_gr=new int[number_of_selected_sets+1];
for (int i=0;i<number_of_selected_sets;i++) sel_gr[i]=selected_graph;
int ret=SmallSetOperations(number_of_selected_sets,sel_gr,selected_sets,DATASETOP_SORT,DATA_X,1,1,NULL);
if (sel_gr!=NULL) delete[] sel_gr;
if (ret==RETURN_SUCCESS)
mainWin->mainArea->completeRedraw();
}

void SetPopup::doSimpleReverse(void)
{
    prepareForAction();
int * sel_gr=NULL;
sel_gr=new int[number_of_selected_sets+1];
for (int i=0;i<number_of_selected_sets;i++) sel_gr[i]=selected_graph;
int ret=SmallSetOperations(number_of_selected_sets,sel_gr,selected_sets,DATASETOP_REVERSE,0,0,0,NULL);
if (sel_gr!=NULL) delete[] sel_gr;
if (ret==RETURN_SUCCESS)
mainWin->mainArea->completeRedraw();
}

void SetPopup::doSimpleJoin(void)
{
    prepareForAction();
int * sel_gr=NULL;
sel_gr=new int[number_of_selected_sets+1];
for (int i=0;i<number_of_selected_sets;i++) sel_gr[i]=selected_graph;
int ret=SmallSetOperations(number_of_selected_sets,sel_gr,selected_sets,DATASETOP_JOIN,0,0,0,NULL);
if (sel_gr!=NULL) delete[] sel_gr;
if (ret==RETURN_SUCCESS)
mainWin->mainArea->completeRedraw();
}

void SetPopup::doSimpleRestrict(void)
{
    prepareForAction();
int * sel_gr=NULL;
sel_gr=new int[number_of_selected_sets+1];
for (int i=0;i<number_of_selected_sets;i++) sel_gr[i]=selected_graph;
int ret=SmallSetOperations(number_of_selected_sets,sel_gr,selected_sets,DATASETOP_RESTRICT,RESTRICT_WORLD,0,0,NULL);
if (sel_gr!=NULL) delete[] sel_gr;
if (ret==RETURN_SUCCESS)
mainWin->mainArea->completeRedraw();
}

void SetPopup::doSimpleSwapXY(void)
{
    prepareForAction();
int * sel_gr=NULL;
sel_gr=new int[number_of_selected_sets+1];
for (int i=0;i<number_of_selected_sets;i++) sel_gr[i]=selected_graph;
int ret=SmallSetOperations(number_of_selected_sets,sel_gr,selected_sets,DATASETOP_SWAP_COLS,0,1,0,NULL);
if (sel_gr!=NULL) delete[] sel_gr;
if (ret==RETURN_SUCCESS)
mainWin->mainArea->completeRedraw();
}

void SetPopup::doSimpleAverage(void)
{
    prepareForAction();
int * sel_gr=NULL;
sel_gr=new int[number_of_selected_sets+1];
for (int i=0;i<number_of_selected_sets;i++) sel_gr[i]=selected_graph;
int ret=SmallSetOperations(number_of_selected_sets,sel_gr,selected_sets,DATASETOP_AVERAGE_SET,0,0,-1,NULL);//try to average without interpolation
if (sel_gr!=NULL) delete[] sel_gr;
if (ret==RETURN_SUCCESS)
mainWin->mainArea->completeRedraw();
}

void SetPopup::doAutoLabel(void)
{
    if (number_of_selected_sets<=0) return;
int * selected_graphs=new int[number_of_selected_sets];
for (int i=0;i<number_of_selected_sets;i++) selected_graphs[i]=selected_graph;
//Create a Label according to "Prepend-Text <Number> Append-Text"
    if (FormAutoLabeler==NULL)
    {
    FormAutoLabeler=new autoLabeler(0);
    }
FormAutoLabeler->init(actLabel->text(),number_of_selected_sets,selected_graphs,selected_sets);
FormAutoLabeler->show();
FormAutoLabeler->raise();
FormAutoLabeler->activateWindow();
delete[] selected_graphs;
}

void SetPopup::generateColorMenu(void)
{
prepareForAction();
    if (actColors!=NULL)//delete old entries
    {
        for (int i=0;i<nr_of_col_items;i++)
        {
        mnuColor->removeAction(actColors[i]);
        delete actColors[i];
        }
        delete[] actColors;
    }
int nr_of_aux_cols;
if (maincolors)
{
delete[] maincolors;
maincolors=NULL;
}
nr_of_col_items=get_main_color_indices(&maincolors,&nr_of_aux_cols);
//qDebug() << "cur number of colors=" << nr_of_col_items;
actColors=new QAction*[nr_of_col_items+2];
QIcon tmp_icon;
    for (int i=0;i<nr_of_col_items;i++)
    {
    tmp_icon=createSingleSmallColorIcon(i);
        if (cmap_table[i].cname)
        actColors[i]=new QAction(tmp_icon,QString(cmap_table[i].cname),this);
        else
        actColors[i]=new QAction(tmp_icon,tr("unnamed"),this);
    actColors[i]->setCheckable(true);
#if QT_VERSION >= 0x050000
    connect(actColors[i],&QAction::triggered,this,[this,i](){this->colorClicked(i);});
#else
    connect(actColors[i],SIGNAL(triggered()),colMapper,SLOT(map()));
#endif
        if (number_of_selected_sets>0 && is_valid_setno(selected_graph,selected_sets[0]))
        {
//qDebug() << "i=" << i << " selected_graph=" << selected_graph << " sel_set_nr=" << selected_sets[0];
            if (i==g[selected_graph].p[selected_sets[0]].linepen.color)
            actColors[i]->setChecked(true);
            else
            actColors[i]->setChecked(false);
        }
        else
        {
        actColors[i]->setChecked(false);
        }
    mnuColor->addAction( actColors[i] );
#if QT_VERSION >= 0x050000

#else
    colMapper->setMapping(actColors[i],i);
#endif
    }
mnuColor->update();
}

void SetPopup::generateLinesMenu(void)
{
prepareForAction();
    if (actLineStyles!=NULL)//delete old entries
    {
        for (int i=0;i<nr_of_linestyles;i++)
        {
        mnuLineStyle->removeAction(actLineStyles[i]);
        delete actLineStyles[i];
        }
        delete[] actLineStyles;
    }
nr_of_linestyles=nr_of_current_linestyles;
//qDebug() << "cur number of linestyles=" << nr_of_current_linestyles;
actLineStyles=new QAction*[nr_of_current_linestyles];
QString * linest_names=new QString[nr_of_current_linestyles];
linest_names[0]=tr("None");
linest_names[1]=tr("straight");
linest_names[2]=tr("narrow slashed");
linest_names[3]=tr("medium slashed");
linest_names[4]=tr("wide slashed");
linest_names[5]=tr("slash dot");
linest_names[6]=tr("wide slash dot");
linest_names[7]=tr("slash dot dot");
linest_names[8]=tr("slash slash dot");
for (int i=9;i<nr_of_current_linestyles;i++)
linest_names[i]=QString("");
QIcon tmp_icon;
    for (int i=0;i<nr_of_current_linestyles;i++)
    {
    //tmp_icon=QIcon(*LinePixmaps[i]);
    tmp_icon=createSingleSmallLineStyleIcon(i);
    actLineStyles[i]=new QAction(tmp_icon,linest_names[i],this);
    actLineStyles[i]->setCheckable(true);
#if QT_VERSION >= 0x050000
    connect(actLineStyles[i],&QAction::triggered,this,[this,i](){this->lineStyleClicked(i);});
#else
    connect(actLineStyles[i],SIGNAL(triggered()),lineStyleMapper,SLOT(map()));
#endif
        if (number_of_selected_sets>0 && is_valid_setno(selected_graph,selected_sets[0]))
        {
            if (i==g[selected_graph].p[selected_sets[0]].lines)
            actLineStyles[i]->setChecked(true);
            else
            actLineStyles[i]->setChecked(false);
        }
        else
        {
        actLineStyles[i]->setChecked(false);
        }
    mnuLineStyle->addAction( actLineStyles[i] );
#if QT_VERSION >= 0x050000

#else
    lineStyleMapper->setMapping( actLineStyles[i], i );
#endif
    }
delete[] linest_names;
mnuLineStyle->update();
}

void SetPopup::generateSymbolsMenu(void)
{
///nr_of_symbols
prepareForAction();
    if (actSymbols!=NULL)//delete old entries
    {
        for (int i=0;i<nr_of_symbols;i++)
        {
        mnuColor->removeAction(actSymbols[i]);
        delete actSymbols[i];
        }
        delete[] actSymbols;
    }
nr_of_symbols=11;
//qDebug() << "cur number of colors=" << nr_of_symbols;
    actSymbols=new QAction*[nr_of_symbols];
/* symbol-icons */
QString entr[nr_of_symbols];
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
QIcon * symb_icons=new QIcon[nr_of_symbols+1];
createSymbolIcons(&symb_icons);
/*
QIcon tmp_icon;
QPixmap map1(12,12);
QPainter paint1(&map1);
paint1.setBrush(Qt::white);
paint1.setPen(Qt::white);
paint1.drawRect(0,0,13,13);
paint1.setPen(Qt::black);
paint1.setBrush(Qt::black);
paint1.end();
tmp_icon=QIcon(map1);
*/
    for (int i=0;i<nr_of_symbols;i++)
    {
        /*if (i!=0)
        {
        paint1.begin(&map1);
        paint1.setBrush(Qt::white);
        paint1.setPen(Qt::white);
        paint1.drawRect(0,0,13,13);
        paint1.setPen(Qt::black);
        paint1.setBrush(Qt::black);
            drawSimpleSymbol(paint1,6,6,10,i,65);
        paint1.end();
        tmp_icon=QIcon(map1);
        }*/
    actSymbols[i]=new QAction(symb_icons[i],entr[i],this);
    actSymbols[i]->setCheckable(true);
#if QT_VERSION >= 0x050000
    connect(actSymbols[i],&QAction::triggered,this,[this,i](){this->symbolsClicked(i);});
#else
    connect(actSymbols[i],SIGNAL(triggered()),symbolMapper,SLOT(map()));
#endif
        if (number_of_selected_sets>0 && is_valid_setno(selected_graph,selected_sets[0]))
        {
            if (i==g[selected_graph].p[selected_sets[0]].sym)
            actSymbols[i]->setChecked(true);
            else
            actSymbols[i]->setChecked(false);
        }
        else
        {
        actSymbols[i]->setChecked(false);
        }
    mnuSymbol->addAction( actSymbols[i] );
#if QT_VERSION >= 0x050000

#else
    symbolMapper->setMapping(actSymbols[i],i);
#endif
    }
mnuSymbol->update();
delete[] symb_icons;
}

void SetPopup::colorClicked(int c)
{
    prepareForAction();
int * sel_gr=NULL;
char dummy[128];
sel_gr=new int[number_of_selected_sets+1];
for (int i=0;i<number_of_selected_sets;i++) sel_gr[i]=selected_graph;
SaveSetStatesPrevious(number_of_selected_sets,sel_gr,selected_sets,UNDO_APPEARANCE);
ListOfChanges.clear();
ListOfOldStates.clear();
sprintf(dummy,"with g%d",selected_graph);
ListOfChanges << QString(dummy);
ListOfOldStates << QString(dummy);
    for (int i=0;i<number_of_selected_sets;i++)
    {
        if (!is_valid_setno(sel_gr[i],selected_sets[i])) continue;
    sprintf(dummy,"    s%d line color %d",selected_sets[i],g[sel_gr[i]].p[selected_sets[i]].linepen.color);
    ListOfOldStates << QString(dummy);
        g[sel_gr[i]].p[selected_sets[i]].linepen.color=c;
    sprintf(dummy,"    s%d line color %d",selected_sets[i],c);
    ListOfChanges << QString(dummy);
    }
SetsModified(number_of_selected_sets,sel_gr,selected_sets,UNDO_APPEARANCE);
mainWin->mainArea->completeRedraw();
if (number_of_selected_sets>0)
{
    if (par!=NULL)
    par->set_new_selection(number_of_selected_sets,selected_sets);
updateSetAppearance(sel_gr[0],selected_sets[0]);
}
delete[] sel_gr;
}

void SetPopup::lineStyleClicked(int c)
{
    prepareForAction();
int * sel_gr=NULL;
char dummy[128];
sel_gr=new int[number_of_selected_sets+1];
for (int i=0;i<number_of_selected_sets;i++) sel_gr[i]=selected_graph;
SaveSetStatesPrevious(number_of_selected_sets,sel_gr,selected_sets,UNDO_APPEARANCE);
ListOfOldStates.clear();
ListOfChanges.clear();
sprintf(dummy,"with g%d",selected_graph);
ListOfChanges << QString(dummy);
ListOfOldStates << QString(dummy);
    for (int i=0;i<number_of_selected_sets;i++)
    {
        if (!is_valid_setno(sel_gr[i],selected_sets[i])) continue;
    sprintf(dummy,"    s%d line linestyle %d",selected_sets[i],g[sel_gr[i]].p[selected_sets[i]].lines);
    ListOfOldStates << QString(dummy);
        g[sel_gr[i]].p[selected_sets[i]].lines=c;
    sprintf(dummy,"    s%d line linestyle %d",selected_sets[i],c);
    ListOfChanges << QString(dummy);
    }
SetsModified(number_of_selected_sets,sel_gr,selected_sets,UNDO_APPEARANCE);
mainWin->mainArea->completeRedraw();
if (number_of_selected_sets>0)
{
    if (par!=NULL)
    par->set_new_selection(number_of_selected_sets,selected_sets);
updateSetAppearance(sel_gr[0],selected_sets[0]);
}
delete[] sel_gr;
}

void SetPopup::symbolsClicked(int c)
{
    prepareForAction();
int * sel_gr=NULL;
char dummy[128];
sel_gr=new int[number_of_selected_sets+1];
for (int i=0;i<number_of_selected_sets;i++) sel_gr[i]=selected_graph;
SaveSetStatesPrevious(number_of_selected_sets,sel_gr,selected_sets,UNDO_APPEARANCE);
ListOfOldStates.clear();
ListOfChanges.clear();
sprintf(dummy,"with g%d",selected_graph);
ListOfChanges << QString(dummy);
ListOfOldStates << QString(dummy);
    for (int i=0;i<number_of_selected_sets;i++)
    {
        if (!is_valid_setno(sel_gr[i],selected_sets[i])) continue;
    sprintf(dummy,"    s%d symbol %d",selected_sets[i],g[sel_gr[i]].p[selected_sets[i]].sym);
    ListOfOldStates << QString(dummy);
        g[sel_gr[i]].p[selected_sets[i]].sym=c;
    sprintf(dummy,"    s%d symbol %d",selected_sets[i],c);
    ListOfChanges << QString(dummy);
    }
SetsModified(number_of_selected_sets,sel_gr,selected_sets,UNDO_APPEARANCE);
mainWin->mainArea->completeRedraw();
if (number_of_selected_sets>0)
{
    if (par!=NULL)
    par->set_new_selection(number_of_selected_sets,selected_sets);
updateSetAppearance(sel_gr[0],selected_sets[0]);
}
delete[] sel_gr;
}

/*
void SetPopup::doSyncList(void)
{
    if (par!=NULL)
    par->doSyncNow();
}
*/

void SetPopup::update_menu_content(void)
{
prepareForAction();
//qDebug() << "UPDATE MENU: gno=" << selected_graph << " nr_of_sel_sets=" << number_of_selected_sets;// <<  selected_sets[i]
    char dummy[128];
    QString tmp_string;
    if (par!=NULL)
    {
    actViewSetComments->setChecked(par->show_comments);
    actViewSetLegends->setChecked(par->show_legends);
    actViewIcons->setChecked(par->show_icons);
    actShowDataLess->setChecked(par->show_data_less);
    actShowHidden->setChecked(par->show_hidden);
        /*if (par->allowSnycCommand)
        actSyncListContents->setVisible(true);
        else
        actSyncListContents->setVisible(false);*/
    }
    /*else
    actSyncListContents->setVisible(false);*/

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
        actAutoLabel->setEnabled(false);
        mnuSimpleOperations->setEnabled(false);
    }
    else//at least 1 set selected
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
        actAutoLabel->setEnabled(true);

        /*for (int i=0;i<number_of_selected_sets;i++)
        {
        qDebug() << i << ":" << selected_sets[i];
        }*/

        createSmallSetList(tmp_string,number_of_selected_sets,selected_sets);

        /*sprintf(dummy,"S");
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
        }*/
        actLabel->setText(tmp_string);

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
            actSimpleJoin->setEnabled(false);
            actSimpleAverage->setEnabled(false);
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
            /// temporarily deactivated
            /*actBringToFront->setEnabled(false);
            actSendToBack->setEnabled(false);*/
            actEditInSpreadsheet->setEnabled(false);
            actEditInTextEditor->setEnabled(false);
            actSimpleJoin->setEnabled(true);
            actSimpleAverage->setEnabled(true);
        }
        mnuSimpleOperations->setEnabled(true);
    }
    if (par==NULL)
    {
    mnuDisplay->setEnabled(false);
    mnuSelectorOperations->setEnabled(false);
    mnuSimpleOperations->setEnabled(true);
    }
    else
    {
    mnuDisplay->setEnabled(true);
    mnuSelectorOperations->setEnabled(true);
    mnuSimpleOperations->setEnabled(true);
    }
    if (QApplication::clipboard()->mimeData()->hasText() || QApplication::clipboard()->mimeData()->hasUrls())
        actPasteClipBoard->setEnabled(true);
    else
        actPasteClipBoard->setEnabled(false);
    generateColorMenu();
    generateLinesMenu();
    generateSymbolsMenu();
this->setFont(qApp->font());
mnuEdit->setFont(qApp->font());
mnuCreateNew->setFont(qApp->font());
mnuDisplay->setFont(qApp->font());
mnuColor->setFont(qApp->font());
mnuLineStyle->setFont(qApp->font());
mnuSymbol->setFont(qApp->font());
mnuSelectorOperations->setFont(qApp->font());
mnuSimpleOperations->setFont(qApp->font());
}

void SetPopup::hideEvent(QHideEvent * e)
{
e->accept();
    if (par==NULL)
    {
    mainWin->stop_highlighting();
    }
QMenu::hideEvent(e);
}

autoLabeler::autoLabeler(QWidget *parent):QWidget(parent)
{
    int nr=4,index=0;
    QString texts[8];
    nr_of_sets=0;
    gr_nrs=s_nrs=NULL;
    setWindowTitle(tr("QtGrace: Auto-labeling"));
    setWindowIcon(QIcon(*GraceIcon));

    lbldescription=new QLabel(tr("Generate automatic labels for sets"),this);

    texts[0]=tr("overwrite");
    texts[1]=tr("put at beginning");
    texts[2]=tr("put at end");
    texts[3]=tr("put at beginning AND end");
    selOldLabel=new StdSelector(this,tr("Existing label:"),nr,texts);
    ledPrepend=new stdLineEdit(this,tr("Prepend:"));
    ledAppend=new stdLineEdit(this,tr("Append:"));
    ledPrepend->setText(QString(""));
    ledAppend->setText(QString(""));
    layout=new QGridLayout(this);
    layout->setSpacing(STD_SPACING);
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    lblPreview=new QLabel(tr("Preview: "),this);

    grpNumber=new QGroupBox(tr("Number"));
    layout0=new QVBoxLayout(grpNumber);
    layout0->setSpacing(STD_SPACING);
    //layout0->setMargin(STD_MARGIN);
    layout0->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    lblNrFormula=new QLabel(tr("Number = offset + n * factor"),grpNumber);
    nr=4;
    texts[0]=tr("Index");
    texts[1]=tr("Set-ID");
    texts[2]=tr("Processed existing label");
    texts[3]=tr("No number");
    selNrFrom=new StdSelector(grpNumber,tr("Take n from:"),nr,texts);
    selNrFrom->setToolTip(tr("The index is just a number going from 0 to the number of selected sets;\nThe set-id is the number displayed in every set list (e.g. G0.S5 has set-id 5);\nIf n is to be taken from the old label the first number in the existing label is used\n(no matter whether there is text in front or after it), e.g. 'Measurement 12' yields n=12."));
    selNrOffset=new stdIntSelector(grpNumber,tr("Offset="),-32767,32768);
    selNrOffset->setValue(1);
    selNrFactor=new stdIntSelector(grpNumber,tr("Factor="),-32767,32768);
    selNrFactor->setValue(1);
    selDigits=new stdIntSelector(grpNumber,tr("Digits="),1,32768);
    selDigits->setValue(2);
    layout0->addWidget(lblNrFormula);
    layout0->addWidget(selNrFrom);
    layout0->addWidget(selNrOffset);
    layout0->addWidget(selNrFactor);
    layout0->addWidget(selDigits);
    grpNumber->setLayout(layout0);

    grpEdit=new QGroupBox(tr("Process existing label"));
    layout1=new QGridLayout(grpEdit);
    layout1->setSpacing(STD_SPACING);
    //layout1->setMargin(STD_MARGIN);
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    nr=7;
    texts[0]=tr("None");
    texts[1]=tr("Take first n characters");
    texts[2]=tr("Take last n characters");
    texts[3]=tr("Take middle characters");
    texts[4]=tr("Take everything before char");
    texts[5]=tr("Take everything after char");
    texts[6]=tr("Take everything between chars");
    selEditType=new StdSelector(grpEdit,tr("Type:"),nr,texts);
    selA=new stdIntSelector(grpEdit,tr("Number of characters:"),0,10000);
    selB=new stdIntSelector(grpEdit,tr("Number of characters:"),0,10000);
    ledA=new stdLineEdit(grpEdit,tr("Start-character:"));
    ledB=new stdLineEdit(grpEdit,tr("Stop-character:"));
    ledA->setText(tr(","));
    ledB->setText(tr(","));
    selA->setValue(1);
    selB->setValue(1);
    layout1->addWidget(selEditType,0,0,1,1);
    layout1->addWidget(selA,1,0,1,1);
    layout1->addWidget(selB,2,0,1,1);
    layout1->addWidget(ledA,3,0,1,1);
    layout1->addWidget(ledB,4,0,1,1);

    buttons=new stdButtonGroup(this,true,true);
    connect(buttons->cmdApply,SIGNAL(clicked()),this,SLOT(doOK()));
    connect(buttons->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttons->cmdClose,SIGNAL(clicked()),this,SLOT(doCancel()));

    connect(selEditType,SIGNAL(currentIndexChanged(int)),this,SLOT(editTypeChanged(int)));
    connect(selNrFrom,SIGNAL(currentIndexChanged(int)),this,SLOT(indexTypeChanged(int)));
    connect(selA,SIGNAL(currentValueChanged(int)),this,SLOT(initPreview(int)));
    connect(selB,SIGNAL(currentValueChanged(int)),this,SLOT(initPreview(int)));
    connect(ledA->lenText,SIGNAL(textEdited(QString)),this,SLOT(textLineEdited(QString)));
    connect(ledB->lenText,SIGNAL(textEdited(QString)),this,SLOT(textLineEdited(QString)));
    connect(ledPrepend->lenText,SIGNAL(textEdited(QString)),this,SLOT(textLineEdited(QString)));
    connect(ledAppend->lenText,SIGNAL(textEdited(QString)),this,SLOT(textLineEdited(QString)));
    connect(selOldLabel,SIGNAL(currentIndexChanged(int)),this,SLOT(initPreview(int)));
    connect(selNrOffset,SIGNAL(currentValueChanged(int)),this,SLOT(initPreview(int)));
    connect(selNrFactor,SIGNAL(currentValueChanged(int)),this,SLOT(initPreview(int)));
    connect(selDigits,SIGNAL(currentValueChanged(int)),this,SLOT(initPreview(int)));

    ledResult=new stdLineEdit(this,QString(""));//this is invisible! Just for simple copy-operations to label-strings
    ledResult->setVisible(false);

    index=0;
    layout->addWidget(lbldescription,index++,0,1,1);
    layout->addWidget(selOldLabel,index++,0,1,1);
    layout->addWidget(grpEdit,index++,0,1,1);
    layout->addWidget(ledPrepend,index++,0,1,1);
    layout->addWidget(ledAppend,index++,0,1,1);
    layout->addWidget(grpNumber,index++,0,1,1);
    layout->addWidget(lblPreview,index++,0,1,1);
    layout->addWidget(buttons,index++,0,1,1);
this->setLayout(layout);
}

void autoLabeler::init(QString text,int nr,int * gnos,int * snos)
{
QString text1=tr("Generate automatic label");
    if (nr>=2)
    {
    text1+=QString("s");
    }
text1+=tr(" for set");
    if (nr>=2)
    {
    text1+=QString("s");
    }
text1+=QString("\n");
lbldescription->setText(text1+text+tr("\nAuto-label=<Label?><Prepend><Number><Append><Label?>"));
SetNrText=text;
if (nr<=0) return;
if (gr_nrs!=NULL) delete[] gr_nrs;
if (s_nrs!=NULL) delete[] s_nrs;
nr_of_sets=nr;
gr_nrs=new int[nr];
s_nrs=new int[nr];
memcpy(gr_nrs,gnos,nr*sizeof(int));
memcpy(s_nrs,snos,nr*sizeof(int));
if (nr<=0)
{
ExistingLabel.clear();
ProcessedLabel.clear();
}
else
{
ExistingLabel=QString(g[gr_nrs[0]].p[s_nrs[0]].orig_lstr);
ProcessedLabel.clear();
}
editTypeChanged(selEditType->currentIndex());
//indexTypeChanged(selNrFrom->currentIndex());
showPreview();
}

QString generateNewLabel(int gno,int sno,int offset,int factor,int number,int digits,QString prepend,QString append,int labelPos,int oldLabelProcessing,int numberOrigin,QString startChar,QString stopChar,int n1,int n2)
{
QString new_label,old_label;
int n;
new_label.clear();
if (is_valid_setno(gno,sno))
{
//get old label
old_label=QString::fromLocal8Bit(g[gno].p[sno].orig_lstr);
///process string
old_label=process_a_text(old_label,oldLabelProcessing,startChar,n1,stopChar,n2,true);
}
else
{
old_label.clear();
}
    if (labelPos==1 || labelPos==3)
    new_label+=old_label;
new_label+=prepend;
if (numberOrigin==0)//Index
{
n=number;//just the current index
}
else if (numberOrigin==1)//Set-ID
{
n=sno;
}
else//Read number from old Label
{
bool extract_ok;
double t_val=read_first_number_from_text(old_label,extract_ok);
n=(int)t_val;
}
//qDebug() << "offset=" << offset << "factor=" << factor << "n=" << n;
    if (numberOrigin!=3)
    {
    QString tmp_number=QString::number(offset+n*factor);
    int len=tmp_number.length();
    while (len<digits)
    {
    tmp_number=QString("0")+tmp_number;
    len=tmp_number.length();
    }
    new_label+=tmp_number;
    }
new_label+=append;
if (labelPos==2 || labelPos==3)
new_label+=old_label;
//qDebug() << "newLabel=" << new_label;
return new_label;
}

void autoLabeler::doOK(void)
{
//qDebug() << "Generating labels";
QString old_label;
QString new_label;
//int index;
SaveSetStatesPrevious(nr_of_sets,gr_nrs,s_nrs,UNDO_APPEARANCE);
ListOfChanges.clear();
ListOfOldStates.clear();
//index=0;
for (int i=0;i<nr_of_sets;i++)
{
    if (is_valid_setno(gr_nrs[i],s_nrs[i])==FALSE) continue;//set does not exist!? --> ignore
sprintf(dummy,"with g%d",gr_nrs[i]);
ListOfChanges << QString(dummy);
ListOfOldStates << QString(dummy);
new_label=generateNewLabel(gr_nrs[i],s_nrs[i],selNrOffset->value(),selNrFactor->value(),i,selDigits->value(),ledPrepend->text(),ledAppend->text(),selOldLabel->currentIndex(),selEditType->currentIndex(),selNrFrom->currentIndex(),ledA->text(),ledB->text(),selA->value(),selB->value());
/*
//old_label=QString::fromLocal8Bit(g[gr_nrs[i]].p[s_nrs[i]].lstr);
old_label=QString::fromLocal8Bit(g[gr_nrs[i]].p[s_nrs[i]].orig_lstr);
//qDebug() << "G" << gr_nrs[i] << ".S" << s_nrs[i] << ".label=" << old_label;
new_label.clear();
if (selOldLabel->currentIndex()==1 || selOldLabel->currentIndex()==3)
new_label+=old_label;
new_label+=ledPrepend->text();
if (selNrFrom->currentIndex()==0)//Index
{
n=index++;//just the current index
}
else if (selNrFrom->currentIndex()==1)//Set-ID
{
n=s_nrs[i];
}
else//Read number from old Label
{
bool extract_ok;
double t_val=read_first_number_from_text(old_label,extract_ok);
n=(int)t_val;
}
new_label+=QString::number(offset+n*factor);
new_label+=ledAppend->text();
if (selOldLabel->currentIndex()==2 || selOldLabel->currentIndex()==3)
new_label+=old_label;
//qDebug() << "New Label=" << new_label;
*/
ledResult->setText(new_label);
sprintf(dummy,"    s%d legend  \"%s\"",s_nrs[i],g[gr_nrs[i]].p[s_nrs[i]].orig_lstr);
ListOfOldStates << QString(dummy);
ledResult->SetMemoryToText(g[gr_nrs[i]].p[s_nrs[i]].lstr,g[gr_nrs[i]].p[s_nrs[i]].orig_lstr);
sprintf(dummy,"    s%d legend  \"%s\"",s_nrs[i],g[gr_nrs[i]].p[s_nrs[i]].orig_lstr);
ListOfChanges << QString(dummy);
}
mainWin->mainArea->completeRedraw();
SetsModified(nr_of_sets,gr_nrs,s_nrs,UNDO_APPEARANCE);
addAditionalDescriptionToLastNode(-1,tr("Auto-labeled sets (")+SetNrText+QString(")"),QString(),-1);
set_dirtystate();

QString tmp_set_text=SetNrText;
int tmp_nr_of_sets=nr_of_sets;
int * tmp_g=new int[2+tmp_nr_of_sets];
int * tmp_s=new int[2+tmp_nr_of_sets];
memcpy(tmp_g,gr_nrs,nr_of_sets*sizeof(int));
memcpy(tmp_s,s_nrs,nr_of_sets*sizeof(int));
init(tmp_set_text,nr_of_sets,tmp_g,tmp_s);
delete[] tmp_g;
delete[] tmp_s;
}

void autoLabeler::doAccept(void)
{
    doOK();
    doCancel();
}

void autoLabeler::doCancel(void)
{
    this->hide();
}

void autoLabeler::showPreview(void)
{
QString new_label;
QString text(tr("Current label: ")+ExistingLabel+QString("\n")+tr("Processed label: "));
//process original label
ProcessedLabel=process_a_text(ExistingLabel,selEditType->currentIndex(),ledA->text(),selA->value(),ledB->text(),selB->value(),true);
//do the other stuff and generate the new autolabel

text+=ProcessedLabel;

if (nr_of_sets>0)
new_label=generateNewLabel(gr_nrs[0],s_nrs[0],selNrOffset->value(),selNrFactor->value(),0,selDigits->value(),ledPrepend->text(),ledAppend->text(),selOldLabel->currentIndex(),selEditType->currentIndex(),selNrFrom->currentIndex(),ledA->text(),ledB->text(),selA->value(),selB->value());
else
new_label.clear();

text+=QString("\nNew label: ")+new_label;

lblPreview->setText(text);
}

void autoLabeler::initPreview(int t)
{
(void)t;
showPreview();
}

void autoLabeler::editTypeChanged(int t)
{

switch (t)
{
case 0://none
    selA->hide();
    selB->hide();
    ledA->hide();
    ledB->hide();
break;
case 1://Take first n characters
    selA->show();
    selA->lblText->setText(tr("Number of characters:"));
    selB->hide();
    ledA->hide();
    ledB->hide();
break;
case 2://Take last n characters
    selA->show();
    selA->lblText->setText(tr("Number of characters:"));
    selB->hide();
    ledA->hide();
    ledB->hide();
break;
case 3://Take middle characters
    selA->show();
    selB->show();
    selA->lblText->setText(tr("Start at character:"));
    selB->lblText->setText(tr("Number of characters:"));
    ledA->hide();
    ledB->hide();
break;
case 4://Take everything before char
    selA->show();
    selB->hide();
    ledA->show();
    ledB->hide();
    selA->lblText->setText(tr("n-th character, n="));
    ledA->lblText->setText(tr("Stop-character:"));
break;
case 5://Take everything after char
    selA->show();
    selB->hide();
    ledA->show();
    ledB->hide();
    selA->lblText->setText(tr("n-th character, n="));
    ledA->lblText->setText(tr("Start-character:"));
break;
case 6://Take everything between chars
    selA->show();
    selB->show();
    ledA->show();
    ledB->show();
    ledA->lblText->setText(tr("Start-character:"));
    selA->lblText->setText(tr("n-th start-character, n="));
    ledB->lblText->setText(tr("Stop-character:"));
    selB->lblText->setText(tr("n-th stop-character, n="));
break;
}
showPreview();
}

void autoLabeler::indexTypeChanged(int t)
{
    if (t!=3)
    {
    selNrOffset->show();
    selNrFactor->show();
    selDigits->show();
    }
    else
    {
    selNrOffset->hide();
    selNrFactor->hide();
    selDigits->hide();
    }
    showPreview();
}

void autoLabeler::textLineEdited(QString text)
{
    (void) text;
    showPreview();
}

multiFileSelector::multiFileSelector(QWidget *parent):QWidget(parent)
{
    singleFileOnly=false;
    defaultStartPath=QDir::homePath();
    layout=new QHBoxLayout();
    //layout->setMargin(0);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(STD_SPACING);
    cmdLoadFile=new QPushButton(tr("Choose File(s)"),this);
    lenFile=new stdLineEdit(this,tr("File(s):"),false);
    lenFile->acceptDrops();
    lenFile->setText(QString(""));
    layout->addWidget(lenFile);
    layout->addWidget(cmdLoadFile);
    connect(cmdLoadFile,SIGNAL(clicked()),this,SLOT(doLoadFile()));
    connect(lenFile,SIGNAL(changed()),this,SLOT(enter_Pressed()));
    //connect(lenFile->lenText,SIGNAL(returnPressed()),this,SLOT(enter_Pressed()));
    this->setLayout(layout);
    this->setAcceptDrops(true);
}

void multiFileSelector::dropEvent(QDropEvent * event)
{
if (event->mimeData()->hasUrls()==true) event->accept();
else return;
QStringList list;
QString text;
list.clear();
text.clear();
if (singleFileOnly==true)
{
    if (event->mimeData()->urls().length()>0)
    {
    list << event->mimeData()->urls().at(0).toLocalFile();
    text=list.at(0);
    }
lenFile->lenText->setText(text);
emit(newSelection(list));
}
else
{
    for (int i=0;i<event->mimeData()->urls().length();i++)
    list << event->mimeData()->urls().at(i).toLocalFile();

    text=list.at(0);
        for (int i=1;i<list.length();i++)
        {
        text+=QString(";")+list.at(i);
        }
    lenFile->lenText->setText(text);
    emit(newSelection(list));
}
}

void multiFileSelector::dragEnterEvent(QDragEnterEvent *event)
{
if (event->mimeData()->hasUrls()==true) event->accept();
}

void multiFileSelector::doLoadFile(void)
{
    QStringList list;
    QString text;
    list.clear();
    text.clear();
    if (singleFileOnly==false)
    {
    list=QFileDialog::getOpenFileNames(this,tr("Open files"),defaultStartPath);
    }
    else
    {
    text=QFileDialog::getOpenFileName(this,tr("Open file"),defaultStartPath);
        if (!text.isEmpty())
        {
        list << text;
        }
    }
    if (list.length()>0)
    {
    text.clear();
    text=list.at(0);
        for (int i=1;i<list.length();i++)
        {
        text+=QString(";")+list.at(i);
        }
    lenFile->lenText->setText(text);
    emit(newSelection(list));
    }
}

void multiFileSelector::enter_Pressed(void)
{
    if (lenFile->lenText->text().isEmpty()) return;
QStringList list;
    getSelection(list);
//qDebug() << "Enter Pressed:" << list;
    emit(newSelection(list));
}

void multiFileSelector::getSelection(QStringList & list)
{
    QString text=lenFile->lenText->text();
    QFileInfo fi;
    QString errortext;
    list=text.split(";");
    errortext.clear();
    for (int i=list.length()-1;i>=0;i--)
    {
    fi.setFile(list.at(i));
        if (fi.exists()==false)
        {
        errortext+=tr("Selected File: ") + list.at(i) + tr(" does not exist! It will be removed from list.\n");
        list.removeAt(i);
        }
    }
    if (singleFileOnly==true && list.length()>1)
    {
    text=list.at(0);
    list.clear();
    list << text;
    errortext+=tr("Only a single file allowed. Other files removed from list.\n");
    }
    if (errortext.length()>0)
    {
        text.clear();
        if (list.length()>0)
        {
        text=list.at(0);
            for (int i=1;i<list.length();i++)
            {
            text+=QString(";")+list.at(i);
            }
        }
    lenFile->lenText->setText(text);
    errmsg(errortext.toLocal8Bit().constData());
    }
}

postProcessingSettings::postProcessingSettings(QWidget *parent):QWidget(parent)
{
    min_element_height=20;
    layout=new QGridLayout();
    //layout->setMargin(STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->setSpacing(STD_SPACING);
    nr_of_elements=nr_of_input_cols=0;
    file_set_columns.clear();
    element_values=NULL;
    element_sets.clear();
    allocatedLines=0;
    update_running=preserve=false;
    operations.clear();
    operations << QString("");//0
    operations << QString("+");//1
    operations << QString("-");//2
    operations << QString("*");//3
    operations << QString("/");//4
    operations << QString("^");//5
    operations << QString("*10^");//6
    operations << QString("*2^");//7

    int nr=NUMBER_OF_SETTYPES;
    QString * entr=new QString[nr];
    for (int i=0;i<nr;i++)
    {
        entr[i]=QString(set_types(i));
        entr[i]=entr[i].toUpper();
    }

    selSetType=new StdSelector(this,tr("Set type:"),nr,entr);
    chkStringCol=new QCheckBox(tr("String-column"),this);
    nr=4;
    entr[0]=tr("None");
    entr[1]=tr("X");
    entr[2]=tr("Y");
    entr[3]=tr("XY");
    selAutoScale=new StdSelector(this,tr("Autoscale graph on load:"),nr,entr);
    //chkAutoScale=new QCheckBox(tr("Autoscale"),this);

    radSingleSet=new QRadioButton(tr("Read a single set"),this);
    radSingleSet->setToolTip(tr("Just read a single set according to the format settings below."));
    radSingleSet->setChecked(true);
    radAllSets=new QRadioButton(tr("Read multiple sets"),this);
    radAllSets->setToolTip(tr("Read as many sets as there are data-columns in the file.\nThe set-type will be used for all imported sets.\nTherefore the columns in the file will be used one after the other to fill the set-columns.\nHow the different columns are generated is set by additional combo-boxes."));

    connect(selSetType,SIGNAL(currentIndexChanged(int)),this,SLOT(setTypeChanged(int)));
    connect(chkStringCol,SIGNAL(toggled(bool)),this,SLOT(stringToggled(bool)));
    connect(radSingleSet,SIGNAL(toggled(bool)),this,SLOT(radChanged(bool)));
    connect(radAllSets,SIGNAL(toggled(bool)),this,SLOT(radChanged(bool)));

    lblFile=new QLabel(QString("File:"),this);
    lblInCols=new QLabel(QString("Nr of Input-Columns:"),this);
    selLines=new stdIntSelector(this,tr("Lines:"),2,10);
    spnVal1=NULL;
    spnVal2=NULL;
    cmbType=NULL;
    cmbOutput=NULL;
    cmbSource1=NULL;
    cmbSource2=NULL;
    lblColumn=NULL;

    lblStringColumn=new QLabel(tr("String-Column"),this);
    lblStrEq=new QLabel(tr("="),this);
    cmbStringCol=new QComboBox(this);
    cmbStringAdvance=new QComboBox(this);
    cmbStringAdvance->setToolTip(tr("How to change the column-ids when more then one set is to be read.\n+0=unchanged, +1=increase column-id by 1,\n+n=keep relative column-order starting after last column used\nsame=use next column with same format\nnext set/same col=use the same column from the next set"));
    cmbStringAdvance->addItem(QString("+0"));
    cmbStringAdvance->addItem(QString("+1"));
    cmbStringAdvance->addItem(QString("+n"));
    cmbStringAdvance->addItem(QString("same"));
    cmbStringAdvance->addItem(QString("next set/same column"));
    lblStringColumn->setVisible(false);
    lblStrEq->setVisible(false);
    cmbStringCol->setVisible(false);
    cmbStringAdvance->setVisible(false);

    buttons=new stdButtonGroup(this,true,false,false);
    connect(buttons->cmdApply,SIGNAL(pressed()),this,SLOT(doApply()));
    connect(buttons->cmdClose,SIGNAL(pressed()),this,SLOT(doClose()));
    connect(selLines,SIGNAL(currentValueChanged(int)),this,SLOT(new_number_of_lines(int)));
    layout->addWidget(lblFile,0,0,1,7);
    layout->addWidget(radSingleSet,1,0,1,2);
    layout->addWidget(radAllSets,1,2,1,2);
    layout->addWidget(selAutoScale,1,4,1,2);
    layout->addWidget(selSetType,2,0,1,2);
    layout->addWidget(chkStringCol,2,2,1,2);
    layout->addWidget(selLines,2,5,1,2);
    layout->addWidget(lblInCols,3,0,1,3);

    //here would the actual lines go

    layout->addWidget(lblStringColumn,5,0,1,1);
    layout->addWidget(lblStrEq,5,1,1,1);
    layout->addWidget(cmbStringCol,5,2,1,4);
    layout->addWidget(cmbStringAdvance,5,6,1,1);
    layout->addWidget(buttons,6,0,1,7);
    setLayout(layout);
    initEntries(2);
    new_number_of_lines(2);
}

void postProcessingSettings::initEntries(int output_cols)
{
    if (nr_of_elements>0) nr_of_elements=0;
    if (element_values!=NULL) delete[] element_values;
    element_sets.clear();
    output_elements.clear();
    elements_out.clear();
    for (int i=0;i<output_cols;i++)
    {
    elements_out << QString(dataset_colname(i));
    output_elements << i;
    }
    for (int i=0;i<MAX_DUMMY_COLS;i++)
    {
    elements_out << QString(QChar(65+i));
    output_elements << -i-1;
    }

    nr_of_input_cols=0;
    for (int i=0;i<file_set_columns.length();i++)
    {
    nr_of_input_cols+=file_set_columns.at(i);
    }
//qDebug() << "We are here INITING; columns=" << nr_of_input_cols;
    element_values=new int[17+nr_of_input_cols];

    elements.clear();
    int counter=0,setcolcounter=0;
    for (int i=0;i<file_set_columns.length();i++)
    {
        for (int j=0;j<file_set_columns.at(i);j++)
        {
        elements << QString("Set ")+QString::number(i)+QString(" C")+QString::number(j);
        element_sets << i;
        element_values[counter++]=j;
        }
    }
    setcolcounter=counter;
    elements << QString("Index");
    element_sets << -1;
    element_values[counter++]=-200;
    elements << QString("Value");
    element_sets << -1;
    element_values[counter++]=-100;
    for (int i=0;i<15;i++)
    {
    elements << QString(QChar(65+i));
    element_sets << -1;
    element_values[counter++]=-i-1;
    }
    //qDebug() << "B0";
    storeDataForPreserve();
    for (int i=0;i<allocatedLines;i++)
    {
    cmbSource1[i]->clear();
    cmbSource2[i]->clear();
    cmbOutput[i]->clear();
        for (int j=0;j<elements.length();j++)
        {
        cmbSource1[i]->addItem(elements.at(j));
        cmbSource2[i]->addItem(elements.at(j));
        }
        for (int j=0;j<elements_out.length();j++)
        {
        cmbOutput[i]->addItem(elements_out.at(j));
        }
    }
    cmbStringCol->clear();
    for (int j=0;j<setcolcounter;j++)
    {
    cmbStringCol->addItem(elements.at(j));
    }
    cmbStringCol->setCurrentIndex(0);
    preserveData();
    //qDebug() << "B1";
    updateVisibilites();
}

void postProcessingSettings::init(int output_cols,struct csv_multi_import_infos & file_info)
{
    file_set_columns.clear();
    for (int i=0;i<file_info.nr_of_sets;i++)
    {
    file_set_columns << file_info.set[i].columns;
    }
//qDebug() << "FileSetColumns=" << file_set_columns;

initEntries(output_cols);

/*qDebug() << endl;
qDebug() << "Output-Elements:";
for (int i=0;i<output_elements.length();i++)
qDebug() << i << ":" << elements_out.at(i) << " | " << output_elements.at(i);
qDebug() << endl;
qDebug() << "Input-Elements:";
for (int i=0;i<output_elements.length();i++)
qDebug() << i << ":" << elements.at(i) << " | S" << element_sets.at(i) << "C" << element_values[i];
qDebug() << endl;
qDebug() << "Now We Init The GUI";*/

lblFile->setText(tr("File")+QString(": ")+QString(file_info.filename));
lblInCols->setText(tr("Nr. of Input-Columns: ")+QString::number(nr_of_input_cols));
selLines->setRange(output_cols,output_cols+15);

preserve=false;
selLines->setValue(output_cols);
//new_number_of_lines(output_cols);
preserve=true;
}

void postProcessingSettings::init_postprocessing_data(struct csv_multi_import_infos & file_info)
{
//0. initialisiere die Listen

//1. gehe durch alle Ziele-Columns (X/Y/Y1/...) der Reihe nach und suche die ursprungs-columns
//fuege zu Liste hinze falls noch nicht in der Liste
//falls eine der Quellen eine temporaere Variable is (A,B,C,...) dann weiter in deren Ursprung suchen, bis keine temp. Variable mehr vorkommt

//2. jetzt die Listen der Column-typen erstellen (String / nicht-string)

}

void postProcessingSettings::set_data_to_input(struct csv_multi_import_infos & file_info)
{
int index,probe,probe_set;
chkStringCol->setChecked(file_info.hasStringCol);
if (file_info.hasStringCol)
{
    probe=file_info.string_source_col;
    probe_set=file_info.string_source_set;
    index=-1;
        for (int j=0;j<element_sets.length();j++)
        {
            if (element_sets.at(j)==probe_set && element_values[j]==probe)
            {
            index=j;
            break;
            }
        }
    cmbStringCol->setCurrentIndex(index);
    cmbStringAdvance->setCurrentIndex(file_info.string_advancement);
}
selSetType->setCurrentIndex(file_info.targetSetType);
selLines->setValue(file_info.postprocess_target_col.length());
if (file_info.readAll==true) radAllSets->setChecked(true);
else radSingleSet->setChecked(true);
selAutoScale->setCurrentIndex(file_info.autoscale);
qApp->processEvents();
    for (int i=0;i<allocatedLines;i++)
    {
    if (file_info.postprocess_target_col.length()<=i) break;

    probe=file_info.postprocess_target_col.at(i);
    index=output_elements.indexOf(probe);
    cmbOutput[i]->setCurrentIndex(index);

    probe=file_info.postprocess_operation_source1.at(i);
    probe_set=file_info.postprocess_operation_source_set1.at(i);
    index=-1;
        for (int j=0;j<element_sets.length();j++)
        {
            if (element_sets.at(j)==probe_set && element_values[j]==probe)
            {
            index=j;
            break;
            }
        }
    cmbSource1[i]->setCurrentIndex(index);
    cmbAdvance1[i]->setCurrentIndex(file_info.postprocess_advancement1.at(i));

    cmbType[i]->setCurrentIndex(file_info.postprocess_operation_type.at(i));

    probe=file_info.postprocess_operation_source2.at(i);
    probe_set=file_info.postprocess_operation_source_set2.at(i);
    index=-1;
        for (int j=0;j<element_sets.length();j++)
        {
            if (element_sets.at(j)==probe_set && element_values[j]==probe)
            {
            index=j;
            break;
            }
        }
    cmbSource2[i]->setCurrentIndex(index);
    cmbAdvance2[i]->setCurrentIndex(file_info.postprocess_advancement2.at(i));
    spnVal1[i]->setValue(file_info.postprocess_val1.at(i));
    spnVal2[i]->setValue(file_info.postprocess_val2.at(i));
    }
}

void postProcessingSettings::replacement_col_indices(struct csv_multi_import_infos & file_info,QList<int> & old_i,QList<int> & new_i,int iteration)
{
if (iteration==0)
{
new_i=old_i;//nothing to replace here
}
else
{


}
}

void postProcessingSettings::resetColumnCount(int nr)
{
new_number_of_lines(nr);
}

void postProcessingSettings::readSettings(struct csv_multi_import_infos & file_info)
{
int lines=selLines->value();
int probe;
    if (radSingleSet->isChecked()==true)
    file_info.readAll=false;
    else
    file_info.readAll=true;
file_info.targetSetType=selSetType->currentIndex();
file_info.targetSetCols=settype_cols(file_info.targetSetType);

file_info.postprocess_target_col.clear();
file_info.postprocess_operation_type.clear();
file_info.postprocess_operation_source1.clear();
file_info.postprocess_operation_source2.clear();
file_info.postprocess_operation_source_set1.clear();
file_info.postprocess_operation_source_set2.clear();//a set of -1 means that the source is one of the temporary columns (A,B,...)
file_info.postprocess_val1.clear();
file_info.postprocess_val2.clear();
file_info.postprocess_advancement1.clear();
file_info.postprocess_advancement2.clear();
file_info.string_source_col=file_info.string_source_set=-1;
    for (int i=0;i<lines;i++)
    {
    file_info.postprocess_val1 << spnVal1[i]->value();
    file_info.postprocess_val2 << spnVal2[i]->value();
    file_info.postprocess_target_col << output_elements.at(cmbOutput[i]->currentIndex());
    file_info.postprocess_operation_type << cmbType[i]->currentIndex();
    probe=cmbSource1[i]->currentIndex();
    if (probe<0) probe=0;//for safety
    file_info.postprocess_operation_source1 << element_values[probe];
    file_info.postprocess_operation_source_set1 << element_sets.at(probe);
    probe=cmbSource2[i]->currentIndex();
    if (probe<0) probe=0;//for safety
    file_info.postprocess_operation_source2 << element_values[probe];
    file_info.postprocess_operation_source_set2 << element_sets.at(probe);
    file_info.postprocess_advancement1 << cmbAdvance1[i]->currentIndex();
    file_info.postprocess_advancement2 << cmbAdvance2[i]->currentIndex();
    }
file_info.hasStringCol=chkStringCol->isChecked();
    if (file_info.hasStringCol==true)
    {
    probe=cmbStringCol->currentIndex();
    if (probe<0) probe=0;//for safety
    file_info.string_source_col=element_values[probe];
    file_info.string_source_set=element_sets.at(probe);
    file_info.string_advancement=cmbStringAdvance->currentIndex();
    //qDebug() << "String Source: S" << file_info.string_source_set << ".C" << file_info.string_source_col;
    }
    else
    {
    file_info.string_source_col=file_info.string_source_set=file_info.string_advancement=-1;
    }
file_info.postprocess_operation_source1_sav=file_info.postprocess_operation_source1;
file_info.postprocess_operation_source2_sav=file_info.postprocess_operation_source2;
file_info.autoscale=selAutoScale->currentValue();
}

void postProcessingSettings::new_number_of_lines(int nr)
{
int first_line=4;
//qDebug() << "new nr of lines=" << nr;
storeDataForPreserve();
//delete old elements
if (allocatedLines>0)
{
    for (int i=allocatedLines-1;i>=0;i--)
    {
    delete spnVal2[i];
    delete cmbSource2[i];
    delete cmbType[i];
    delete spnVal1[i];
    delete cmbSource1[i];
    delete lblColumn[i];
    delete cmbOutput[i];
    delete cmbAdvance1[i];
    delete cmbAdvance2[i];
    }
    N_DELETE2(spnVal1);
    N_DELETE2(spnVal2);
    N_DELETE2(cmbType);
    N_DELETE2(cmbOutput);
    N_DELETE2(cmbSource1);
    N_DELETE2(cmbSource2);
    N_DELETE2(cmbAdvance1);
    N_DELETE2(cmbAdvance2);
    N_DELETE2(lblColumn);
    allocatedLines=0;
}
layout->removeWidget(lblStringColumn);
layout->removeWidget(lblStrEq);
layout->removeWidget(cmbStringCol);
layout->removeWidget(cmbStringAdvance);

//create new elements
spnVal1=new QDoubleSpinBox*[nr];
spnVal2=new QDoubleSpinBox*[nr];
cmbType=new QComboBox*[nr];
cmbOutput=new QComboBox*[nr];
cmbSource1=new QComboBox*[nr];
cmbSource2=new QComboBox*[nr];
cmbAdvance1=new QComboBox*[nr];
cmbAdvance2=new QComboBox*[nr];
lblColumn=new QLabel*[nr];
allocatedLines=nr;
for (int i=0;i<allocatedLines;i++)
{
cmbOutput[i]=new QComboBox(this);
cmbOutput[i]->addItems(elements_out);
layout->addWidget(cmbOutput[i],first_line+i,0,1,1);
lblColumn[i]=new QLabel(QString("="),this);
layout->addWidget(lblColumn[i],first_line+i,1,1,1);
cmbSource1[i]=new QComboBox(this);
cmbSource1[i]->addItems(elements);
connect(cmbSource1[i],QOverload<int>::of(&QComboBox::currentIndexChanged),this,[this,i](int index){(void)index;(void)i;this->updateVisibilites();});
layout->addWidget(cmbSource1[i],first_line+i,2,1,1);
spnVal1[i]=new QDoubleSpinBox(this);
spnVal1[i]->setDecimals(9);
spnVal1[i]->setRange(-DBL_MAX,DBL_MAX);
layout->addWidget(spnVal1[i],first_line+i,3,1,1);
cmbAdvance1[i]=new QComboBox(this);
cmbAdvance1[i]->addItem(QString("+0"));
cmbAdvance1[i]->addItem(QString("+1"));
cmbAdvance1[i]->addItem(QString("+n"));
cmbAdvance1[i]->addItem(QString("same"));
cmbAdvance1[i]->addItem(QString("next set/same col"));
cmbAdvance1[i]->setToolTip(tr("How to change the column-ids when more then one set is to be read.\n+0=unchanged, +1=increase column-id by 1,\n+n=keep relative column-order starting after last column used\nsame=use next column with same format\nnext set/same col=use the same column from the next set"));
layout->addWidget(cmbAdvance1[i],first_line+i,3,1,1);
cmbType[i]=new QComboBox(this);
cmbType[i]->addItems(operations);
connect(cmbType[i],QOverload<int>::of(&QComboBox::currentIndexChanged),this,[this,i](int index){(void)index;(void)i;this->updateVisibilites();});
layout->addWidget(cmbType[i],first_line+i,4,1,1);
cmbSource2[i]=new QComboBox(this);
cmbSource2[i]->addItems(elements);
connect(cmbSource2[i],QOverload<int>::of(&QComboBox::currentIndexChanged),this,[this,i](int index){(void)index;(void)i;this->updateVisibilites();});
layout->addWidget(cmbSource2[i],first_line+i,5,1,1);
spnVal2[i]=new QDoubleSpinBox(this);
spnVal2[i]->setDecimals(9);
spnVal2[i]->setRange(-DBL_MAX,DBL_MAX);
layout->addWidget(spnVal2[i],first_line+i,6,1,1);
cmbAdvance2[i]=new QComboBox(this);
cmbAdvance2[i]->addItem(QString("+0"));
cmbAdvance2[i]->addItem(QString("+1"));
cmbAdvance2[i]->addItem(QString("+n"));
cmbAdvance2[i]->addItem(QString("same"));
cmbAdvance2[i]->addItem(QString("next set/same col"));
cmbAdvance2[i]->setToolTip(tr("How to change the column-ids when more then one set is to be read.\n+0=unchanged, +1=increase column-id by 1,\n+n=keep relative column-order starting after last column used\nsame=use next column with same format\nnext set/same col=use the same column from the next set"));
layout->addWidget(cmbAdvance2[i],first_line+i,6,1,1);
}
cmbStringCol->clear();
for (int i=0;i<elements.length();i++)
{
    if (element_sets.at(i)>=0)
    cmbStringCol->addItem(elements.at(i));
}
layout->addWidget(lblStringColumn,first_line+nr,0,1,1);
layout->addWidget(lblStrEq,first_line+nr,1,1,1);
layout->addWidget(cmbStringCol,first_line+nr,2,1,4);
layout->addWidget(cmbStringAdvance,first_line+nr,6,1,1);
layout->addWidget(buttons,first_line+nr+1,0,1,7);

if (preserve)
{
preserveData();
}
else//default-initialization
{
    for (int i=0;i<allocatedLines;i++)
    {
    cmbOutput[i]->setCurrentIndex(i);
    cmbSource1[i]->setCurrentIndex(i);
    }
}
updateVisibilites();
this->adjustSize();
}

void postProcessingSettings::setTypeChanged(int nr)
{
int vals=settype_cols(nr);
//qDebug() << "A";
update_running=true;
selLines->setRange(vals,vals+15);
//qDebug() << "B";
update_running=true;
initEntries(vals);
//qDebug() << "Type Changed=" << nr << "--> Columns=" << vals;
update_running=true;
selLines->setValue(vals);
//qDebug() << "C";
update_running=false;
updateVisibilites();
//resetColumnCount(vals);
}

void postProcessingSettings::stringToggled(bool v)
{
//qDebug() << "String toggeled" << v;
lblStringColumn->setVisible(v);
lblStrEq->setVisible(v);
cmbStringCol->setVisible(v);
    if (radAllSets->isChecked()==true)
    {
    cmbStringAdvance->setVisible(v);
    }
    else
    {
    cmbStringAdvance->setVisible(false);
    }
}

void postProcessingSettings::radChanged(bool c)
{
(void) c;
updateVisibilites();
}

void postProcessingSettings::updateVisibilites(void)
{
static int probe;
if (update_running==true) return;
//qDebug() << "updateVisibilities()";
update_running=true;
if (chkStringCol->isChecked()==true && radAllSets->isChecked()==true)
cmbStringAdvance->setVisible(true);
else
cmbStringAdvance->setVisible(false);
    for (int i=0;i<allocatedLines;i++)
    {
    probe=element_values[cmbSource1[i]->currentIndex()];
    //qDebug() << "Line" << i << "Index=" << cmbSource1[i]->currentIndex() << "Probe1=" << probe;
        if (probe==-100) spnVal1[i]->setVisible(true);//we need to input a value
        else spnVal1[i]->setVisible(false);
    if (cmbType[i]->currentIndex()==0)//no further settings
    {
    cmbSource2[i]->setVisible(false);
    spnVal2[i]->setVisible(false);
    }
    else
    {
    cmbSource2[i]->setVisible(true);
    probe=element_values[cmbSource2[i]->currentIndex()];
    //qDebug() << "Line" << i << "Index=" << cmbSource2[i]->currentIndex() << "Probe2=" << probe;
        if (probe==-100) spnVal2[i]->setVisible(true);//we need to input a value
        else spnVal2[i]->setVisible(false);
    }
        if (radAllSets->isChecked()==true)
        {
        probe=element_values[cmbSource1[i]->currentIndex()];
        if (probe>=0)
        cmbAdvance1[i]->setVisible(true);
        else
        cmbAdvance1[i]->setVisible(false);
            if (cmbType[i]->currentIndex()==0)
            {
            cmbAdvance2[i]->setVisible(false);
            }
            else
            {
            probe=element_values[cmbSource2[i]->currentIndex()];
                if (probe>=0)
                cmbAdvance2[i]->setVisible(true);
                else
                cmbAdvance2[i]->setVisible(false);
            }
        }
        else
        {
        cmbAdvance1[i]->setVisible(false);
        cmbAdvance2[i]->setVisible(false);
        }
    }
update_running=false;
}

void postProcessingSettings::changeAppearance(bool show_filename,bool show_cols,bool show_buttons)
{
lblFile->setVisible(show_filename);
lblInCols->setVisible(show_cols);
/*if (show_cols==true)
{
layout->addWidget(lblInCols,2,0);
layout->addWidget(selLines,2,1);
}
else
{
layout->addWidget(lblInCols,2,1);
layout->addWidget(selLines,2,0);
}*/
buttons->setVisible(show_buttons);
}

void postProcessingSettings::storeDataForPreserve(void)
{
    vals1.clear();
    vals2.clear();
    out_vals.clear();
    source_vals1.clear();
    source_vals2.clear();
    type_vals.clear();
    for (int i=0;i<allocatedLines;i++)
    {
        if (cmbOutput[i]->isVisible()==true)
        {
        out_vals << cmbOutput[i]->currentIndex();
        //qDebug() << "Output" << i << "=" << out_vals.last();
        vals2 << spnVal2[i]->value();
        source_vals2 << cmbSource2[i]->currentIndex();
        type_vals << cmbType[i]->currentIndex();
        vals1 << spnVal1[i]->value();
        source_vals1 << cmbSource1[i]->currentIndex();
        }
    }
    string_val=cmbStringCol->currentIndex();
    string_adv=cmbStringAdvance->currentIndex();
}

void postProcessingSettings::preserveData(void)
{
update_running=true;
    for (int i=0;i<allocatedLines;i++)
    {
        if (out_vals.length()>i)
        {
        cmbOutput[i]->setCurrentIndex(out_vals.at(i));
        //qDebug() << i << " Setting output to stored Data=" << out_vals.at(i);
        spnVal2[i]->setValue(vals2.at(i));
        cmbSource2[i]->setCurrentIndex(source_vals2.at(i));
        cmbType[i]->setCurrentIndex(type_vals.at(i));
        spnVal1[i]->setValue(vals1.at(i));
        cmbSource1[i]->setCurrentIndex(source_vals1.at(i));
        }
        else if (i==0)
        {
            cmbOutput[i]->setCurrentIndex(0);
            spnVal2[i]->setValue(0.0);
            cmbSource2[i]->setCurrentIndex(0);
            cmbType[i]->setCurrentIndex(0);//cmbType[i-1]->currentIndex()+1);
            spnVal1[i]->setValue(0.0);
            cmbSource1[i]->setCurrentIndex(0);
        }
        else//no preserved data present
        {
        cmbOutput[i]->setCurrentIndex(cmbOutput[i-1]->currentIndex()+1);
        //qDebug() << i << " Setting output to new value=" << cmbOutput[i-1]->currentIndex()+1;
        spnVal2[i]->setValue(spnVal2[i-1]->value());
        cmbSource2[i]->setCurrentIndex(cmbSource2[i-1]->currentIndex()+1);
        cmbType[i]->setCurrentIndex(0);//cmbType[i-1]->currentIndex()+1);
        spnVal1[i]->setValue(spnVal1[i-1]->value());
        cmbSource1[i]->setCurrentIndex(cmbSource1[i-1]->currentIndex()+1);
        }
    }
        if (string_val<0)
        cmbStringCol->setCurrentIndex(0);
        else
        cmbStringCol->setCurrentIndex(string_val);
    cmbStringAdvance->setCurrentIndex(string_adv);
update_running=false;
}

void postProcessingSettings::doApply(void)
{

}

void postProcessingSettings::doClose(void)
{
hide();
}

QPen createPen(int lines,double linew,int color)
{
QPen tmp_pen;
tmp_pen.setColor(get_Color(color));
tmp_pen.setWidthF(qreal(linew));
if (lines==0)
tmp_pen.setStyle(Qt::NoPen);
else if (lines==1)
tmp_pen.setStyle(Qt::SolidLine);
else
{
tmp_pen.setStyle(Qt::CustomDashLine);
tmp_pen.setDashPattern(*PenDashPattern[lines]);
//GeneralPainter->setBackground(Qt::transparent);/// setzte beim Linienzeichnen den Hintergrund auf transparent!!
}
return tmp_pen;
}

void drawSimpleSymbol(QPainter & paint1,int x,int y,int w,int sym,char sym_c=65)//paints a symbol on a painter in the size w x w (always a square) on middle-position x,y; color and linestyle have to be set beforehand
{
int npoints=0;
int wh=w/2;
QPoint *p = new QPoint[5];
if (sym==SYM_CIRCLE)
{
paint1.drawEllipse(x-wh,y-wh,w,w);
}
else if (sym==SYM_CHAR)//draw some Text
{
//paint1.drawText(x-wh*1.8,y+wh*1.8,QString(sym_c));
paint1.drawText(x,y,QString(sym_c));
}
else if (sym==SYM_PLUS)//plus
{
paint1.drawLine(x-wh,y,x+wh,y);
paint1.drawLine(x,y-wh,x,y+wh);
}
else if (sym==SYM_X)//X
{
paint1.drawLine(x-wh,y-wh,x+wh,y+wh);
paint1.drawLine(x+wh,y-wh,x-wh,y+wh);
}
else if (sym==SYM_SPLAT)//star
{
paint1.drawLine(x-wh,y,x+wh,y);
paint1.drawLine(x,y-wh,x,y+wh);
if (w>8)
wh*=0.8;
paint1.drawLine(x-wh,y-wh,x+wh,y+wh);
paint1.drawLine(x+wh,y-wh,x-wh,y+wh);
}
else
{
    switch (sym)
    {
    case SYM_SQUARE://square
        p[0].setX(x-wh);
        p[0].setY(y-wh);
        p[1].setX(x+wh);
        p[1].setY(y-wh);
        p[2].setX(x+wh);
        p[2].setY(y+wh);
        p[3].setX(x-wh);
        p[3].setY(y+wh);
        p[4].setX(x-wh);
        p[4].setY(y-wh);
        npoints=5;
    break;
    case SYM_DIAMOND://diamond
        p[0].setX(x);
        p[0].setY(y-wh);
        p[1].setX(x+wh);
        p[1].setY(y);
        p[2].setX(x);
        p[2].setY(y+wh);
        p[3].setX(x-wh);
        p[3].setY(y);
        p[4].setX(x);
        p[4].setY(y-wh);
        npoints=5;
        break;
    case SYM_TRIANG1://triangle up
        p[0].setX(x);
        p[0].setY(y-wh);
        p[1].setX(x+wh);
        p[1].setY(y+w/4);
        p[2].setX(x-wh);
        p[2].setY(y+w/4);
        p[3].setX(x);
        p[3].setY(y-wh);
        npoints=4;
        break;
    case SYM_TRIANG2://triangle left
        p[0].setX(x-wh);
        p[0].setY(y);
        p[1].setX(x+w/4);
        p[1].setY(y-wh);
        p[2].setX(x+w/4);
        p[2].setY(y+wh);
        p[3].setX(x-wh);
        p[3].setY(y);
        npoints=4;
        break;
    case SYM_TRIANG3://triangle down
        p[0].setX(x);
        p[0].setY(y+wh);
        p[1].setX(x+wh);
        p[1].setY(y-w/4);
        p[2].setX(x-wh);
        p[2].setY(y-w/4);
        p[3].setX(x);
        p[3].setY(y+wh);
        npoints=4;
        break;
    case SYM_TRIANG4://triangle right
        p[0].setX(x+wh);
        p[0].setY(y);
        p[1].setX(x-w/4);
        p[1].setY(y-wh);
        p[2].setX(x-w/4);
        p[2].setY(y+wh);
        p[3].setX(x+wh);
        p[3].setY(y);
        npoints=4;
        break;
    }
paint1.drawPolygon(p,npoints);
}

}

#define LIST_ICON_WIDTH 26//24
#define LIST_ICON_HEIGHT 14
#define LIST_ICON_Y_MID 7
#define LIST_ICON_X_MID1 7
#define LIST_ICON_X_MID2 14//12
#define LIST_ICON_X_SHIFT 12//10
#define LIST_ICON_SYMBOL_SIZE 5

QIcon createIconForSetAppearance(int gno,int sno,double rel_factor)
{
QPixmap map1(LIST_ICON_WIDTH*rel_factor,LIST_ICON_HEIGHT*rel_factor);
QPainter paint1(&map1);
QPen tmp_pen;
bool only_one=false;
int has_x_errorbars,has_y_errorbars;
paint1.setPen(Qt::white);
paint1.setBrush(Qt::white);
paint1.drawRect(0,0,LIST_ICON_WIDTH*rel_factor+1,LIST_ICON_HEIGHT*rel_factor+1);
if (is_valid_setno(gno,sno))
{
has_x_errorbars=has_errorbar(gno, sno, 0);
has_y_errorbars=has_errorbar(gno, sno, 1);
/// draw a simple line
    if (g[gno].p[sno].lines!=0 && g[gno].p[sno].linet!=0)
    {
        if (g[gno].p[sno].sym==0)
        tmp_pen=createPen(g[gno].p[sno].lines,g[gno].p[sno].linew*rel_factor,g[gno].p[sno].linepen.color);
        else
        tmp_pen=createPen(g[gno].p[sno].lines,1.0*rel_factor,g[gno].p[sno].linepen.color);
paint1.setPen(tmp_pen);
paint1.setBackground(Qt::transparent);
        if (g[gno].p[sno].sym==0 && has_x_errorbars==FALSE && has_y_errorbars==FALSE)//no symbol and no error bars --> draw the line completely from one side to the other
        paint1.drawLine(0,LIST_ICON_Y_MID*rel_factor,LIST_ICON_WIDTH*rel_factor+1,LIST_ICON_Y_MID*rel_factor);
        else
        paint1.drawLine(LIST_ICON_X_MID1*rel_factor,LIST_ICON_Y_MID*rel_factor,LIST_ICON_WIDTH*rel_factor-LIST_ICON_X_MID1*rel_factor,LIST_ICON_Y_MID*rel_factor);
    only_one=false;
    }
    else
    {
    only_one=true;
    }
/// draw error-bars
if (g[gno].p[sno].errbar.active && has_y_errorbars)/// Y-error-bars
{
tmp_pen=createPen(g[gno].p[sno].errbar.lines,1.0*rel_factor,g[gno].p[sno].errbar.pen.color);
paint1.setPen(tmp_pen);
    if (g[gno].p[sno].errbar.ptype==PLACEMENT_NORMAL || g[gno].p[sno].errbar.ptype==PLACEMENT_BOTH)
    {
        if (only_one==false)
        {
        paint1.drawLine(4*rel_factor,1*rel_factor,4*rel_factor+LIST_ICON_SYMBOL_SIZE*rel_factor,1*rel_factor);//upper bar (left)
        paint1.drawLine(LIST_ICON_WIDTH*rel_factor-4*rel_factor,1*rel_factor,LIST_ICON_WIDTH*rel_factor-4*rel_factor-LIST_ICON_SYMBOL_SIZE*rel_factor,1*rel_factor);//upper bar (right)
        }
        else
        {
        paint1.drawLine(LIST_ICON_X_MID2*rel_factor-0.5*LIST_ICON_SYMBOL_SIZE*rel_factor,1*rel_factor,LIST_ICON_X_MID2*rel_factor+0.5*LIST_ICON_SYMBOL_SIZE*rel_factor,1*rel_factor);//upper bar
        }
    }
    if (g[gno].p[sno].errbar.ptype==PLACEMENT_OPPOSITE || g[gno].p[sno].errbar.ptype==PLACEMENT_BOTH)
    {
        if (only_one==false)
        {
        paint1.drawLine(4*rel_factor,LIST_ICON_HEIGHT*rel_factor-2*rel_factor,4*rel_factor+LIST_ICON_SYMBOL_SIZE*rel_factor,LIST_ICON_HEIGHT*rel_factor-2*rel_factor);//lower bar (left)
        paint1.drawLine(LIST_ICON_WIDTH*rel_factor-4*rel_factor,LIST_ICON_HEIGHT*rel_factor-2*rel_factor,LIST_ICON_WIDTH*rel_factor-4*rel_factor-LIST_ICON_SYMBOL_SIZE*rel_factor,LIST_ICON_HEIGHT*rel_factor-2*rel_factor);//lower bar (right)
        }
        else
        {
        paint1.drawLine(LIST_ICON_X_MID2*rel_factor-0.5*LIST_ICON_SYMBOL_SIZE*rel_factor,LIST_ICON_HEIGHT*rel_factor-2*rel_factor,LIST_ICON_X_MID2*rel_factor+0.5*LIST_ICON_SYMBOL_SIZE*rel_factor,LIST_ICON_HEIGHT*rel_factor-2*rel_factor);//lower bar
        }
    }
tmp_pen=createPen(g[gno].p[sno].errbar.riser_lines,1.0*rel_factor,g[gno].p[sno].errbar.pen.color);
paint1.setPen(tmp_pen);
    if (g[gno].p[sno].errbar.ptype==PLACEMENT_NORMAL || g[gno].p[sno].errbar.ptype==PLACEMENT_BOTH)
    {
        if (only_one==false)
        {
        paint1.drawLine(LIST_ICON_X_MID1*rel_factor,LIST_ICON_Y_MID*rel_factor,LIST_ICON_X_MID1*rel_factor,1*rel_factor);//left upper riser
        paint1.drawLine(LIST_ICON_X_MID1*rel_factor+LIST_ICON_X_SHIFT*rel_factor,LIST_ICON_Y_MID*rel_factor,LIST_ICON_X_MID1*rel_factor+LIST_ICON_X_SHIFT*rel_factor,1*rel_factor);//right upper riser
        }
        else
        {
        paint1.drawLine(LIST_ICON_X_MID2*rel_factor,LIST_ICON_Y_MID*rel_factor,LIST_ICON_X_MID2*rel_factor,1*rel_factor);//upper bar
        }
    }
    if (g[gno].p[sno].errbar.ptype==PLACEMENT_OPPOSITE || g[gno].p[sno].errbar.ptype==PLACEMENT_BOTH)
    {
        if (only_one==false)
        {
        paint1.drawLine(LIST_ICON_X_MID1*rel_factor,LIST_ICON_Y_MID*rel_factor,LIST_ICON_X_MID1*rel_factor,LIST_ICON_HEIGHT*rel_factor-2*rel_factor);//left lower riser
        paint1.drawLine(LIST_ICON_X_MID1*rel_factor+LIST_ICON_X_SHIFT*rel_factor,LIST_ICON_Y_MID*rel_factor,LIST_ICON_X_MID1*rel_factor+LIST_ICON_X_SHIFT*rel_factor,LIST_ICON_HEIGHT*rel_factor-2*rel_factor);//right lower riser
        }
        else
        {
        paint1.drawLine(LIST_ICON_X_MID2*rel_factor,LIST_ICON_Y_MID*rel_factor,LIST_ICON_X_MID2*rel_factor,LIST_ICON_HEIGHT*rel_factor-2*rel_factor);//lower riser
        }
    }
}
if (g[gno].p[sno].errbar.active && has_x_errorbars)/// X-error-bars
{
tmp_pen=createPen(g[gno].p[sno].errbar.lines,1.0*rel_factor,g[gno].p[sno].errbar.pen.color);
paint1.setPen(tmp_pen);
    if (g[gno].p[sno].errbar.ptype==PLACEMENT_NORMAL || g[gno].p[sno].errbar.ptype==PLACEMENT_BOTH)
    {
        if (only_one==false)
        {
        paint1.drawLine(LIST_ICON_X_MID1*rel_factor+2*rel_factor+LIST_ICON_SYMBOL_SIZE*rel_factor*0.5,LIST_ICON_Y_MID*rel_factor-0.5*LIST_ICON_SYMBOL_SIZE*rel_factor,LIST_ICON_X_MID1*rel_factor+2*rel_factor+LIST_ICON_SYMBOL_SIZE*0.5*rel_factor,LIST_ICON_Y_MID*rel_factor+0.5*LIST_ICON_SYMBOL_SIZE*rel_factor);//right bar (left)
        paint1.drawLine(LIST_ICON_X_MID1*rel_factor+LIST_ICON_X_SHIFT*rel_factor+2*rel_factor+LIST_ICON_SYMBOL_SIZE*0.5*rel_factor,LIST_ICON_Y_MID*rel_factor-0.5*LIST_ICON_SYMBOL_SIZE*rel_factor,LIST_ICON_X_MID1*rel_factor+LIST_ICON_X_SHIFT*rel_factor+2*rel_factor+LIST_ICON_SYMBOL_SIZE*0.5*rel_factor,LIST_ICON_Y_MID*rel_factor+0.5*LIST_ICON_SYMBOL_SIZE*rel_factor);//right bar (right)
        }
        else
        {
        paint1.drawLine(LIST_ICON_X_MID2*rel_factor+2*rel_factor+LIST_ICON_SYMBOL_SIZE*0.5*rel_factor,LIST_ICON_Y_MID*rel_factor-0.5*LIST_ICON_SYMBOL_SIZE*rel_factor,LIST_ICON_X_MID2*rel_factor+2*rel_factor+LIST_ICON_SYMBOL_SIZE*0.5*rel_factor,LIST_ICON_Y_MID*rel_factor+0.5*LIST_ICON_SYMBOL_SIZE*rel_factor);//right bar
        }
    }
    if (g[gno].p[sno].errbar.ptype==PLACEMENT_OPPOSITE || g[gno].p[sno].errbar.ptype==PLACEMENT_BOTH)
    {
        if (only_one==false)
        {
        paint1.drawLine(LIST_ICON_X_MID1*rel_factor-2*rel_factor-LIST_ICON_SYMBOL_SIZE*0.5*rel_factor,LIST_ICON_Y_MID*rel_factor-0.5*LIST_ICON_SYMBOL_SIZE*rel_factor,LIST_ICON_X_MID1*rel_factor-2*rel_factor-LIST_ICON_SYMBOL_SIZE*0.5*rel_factor,LIST_ICON_Y_MID*rel_factor+0.5*LIST_ICON_SYMBOL_SIZE*rel_factor);//left bar (left)
        paint1.drawLine(LIST_ICON_X_MID1*rel_factor+LIST_ICON_X_SHIFT*rel_factor-2*rel_factor-LIST_ICON_SYMBOL_SIZE*0.5*rel_factor,LIST_ICON_Y_MID*rel_factor-0.5*LIST_ICON_SYMBOL_SIZE*rel_factor,LIST_ICON_X_MID1*rel_factor+LIST_ICON_X_SHIFT*rel_factor-2*rel_factor-LIST_ICON_SYMBOL_SIZE*0.5*rel_factor,LIST_ICON_Y_MID*rel_factor+0.5*LIST_ICON_SYMBOL_SIZE*rel_factor);//left bar (right)
        }
        else
        {
        paint1.drawLine(LIST_ICON_X_MID2*rel_factor-2*rel_factor-LIST_ICON_SYMBOL_SIZE*0.5*rel_factor,LIST_ICON_Y_MID*rel_factor-0.5*LIST_ICON_SYMBOL_SIZE*rel_factor,LIST_ICON_X_MID2*rel_factor-2*rel_factor-LIST_ICON_SYMBOL_SIZE*0.5*rel_factor,LIST_ICON_Y_MID*rel_factor+0.5*LIST_ICON_SYMBOL_SIZE*rel_factor);//left bar
        }
    }
tmp_pen=createPen(g[gno].p[sno].errbar.riser_lines,1.0*rel_factor,g[gno].p[sno].errbar.pen.color);
paint1.setPen(tmp_pen);
    if (g[gno].p[sno].errbar.ptype==PLACEMENT_NORMAL || g[gno].p[sno].errbar.ptype==PLACEMENT_BOTH)
    {
        if (only_one==false)
        {
        paint1.drawLine(LIST_ICON_X_MID1*rel_factor,LIST_ICON_Y_MID*rel_factor,LIST_ICON_X_MID1*rel_factor+2*rel_factor+LIST_ICON_SYMBOL_SIZE*0.5*rel_factor,LIST_ICON_Y_MID*rel_factor);//right riser (left)
        paint1.drawLine(LIST_ICON_X_MID1*rel_factor+LIST_ICON_X_SHIFT*rel_factor,LIST_ICON_Y_MID*rel_factor,LIST_ICON_X_MID1*rel_factor+LIST_ICON_X_SHIFT*rel_factor+2*rel_factor+LIST_ICON_SYMBOL_SIZE*0.5*rel_factor,LIST_ICON_Y_MID*rel_factor);//right riser (right)
        }
        else
        {
        paint1.drawLine(LIST_ICON_X_MID2*rel_factor,LIST_ICON_Y_MID*rel_factor,LIST_ICON_X_MID2*rel_factor+2*rel_factor+LIST_ICON_SYMBOL_SIZE*0.5*rel_factor,LIST_ICON_Y_MID*rel_factor);//right riser
        }
    }
    if (g[gno].p[sno].errbar.ptype==PLACEMENT_OPPOSITE || g[gno].p[sno].errbar.ptype==PLACEMENT_BOTH)
    {
        if (only_one==false)
        {
        paint1.drawLine(LIST_ICON_X_MID1*rel_factor,LIST_ICON_Y_MID*rel_factor,LIST_ICON_X_MID1*rel_factor-2*rel_factor-LIST_ICON_SYMBOL_SIZE*0.5*rel_factor,LIST_ICON_Y_MID*rel_factor);//left riser (left)
        paint1.drawLine(LIST_ICON_X_MID1*rel_factor+LIST_ICON_X_SHIFT*rel_factor,LIST_ICON_Y_MID*rel_factor,LIST_ICON_X_MID1*rel_factor+LIST_ICON_X_SHIFT*rel_factor-2*rel_factor-LIST_ICON_SYMBOL_SIZE*0.5*rel_factor,LIST_ICON_Y_MID*rel_factor);//left riser (right)
        }
        else
        {
        paint1.drawLine(LIST_ICON_X_MID2*rel_factor,LIST_ICON_Y_MID*rel_factor,LIST_ICON_X_MID2*rel_factor-2*rel_factor-LIST_ICON_SYMBOL_SIZE*0.5*rel_factor,LIST_ICON_Y_MID*rel_factor);//left riser
        }
    }
}
/// draw symbols
if (g[gno].p[sno].sym!=0)
{
tmp_pen=createPen(g[gno].p[sno].symlines,1.0*rel_factor,g[gno].p[sno].sympen.color);
paint1.setPen(tmp_pen);
    if (g[gno].p[sno].symfillpen.pattern==0)
    {
    //paint1.setBrush(Qt::white);
    paint1.setBrush(Qt::NoBrush);
    }
    else
    {
    paint1.setBrush(get_Color(g[gno].p[sno].symfillpen.color));
    }
        if (g[gno].p[sno].sym==SYM_CHAR)
        {
        QFont dFont=getFontFromDatabase(g[gno].p[sno].charfont);
        dFont.setPixelSize(LIST_ICON_SYMBOL_SIZE*1.8*rel_factor);
        paint1.setFont(dFont);
        }
        if (only_one==false)
        {
        drawSimpleSymbol(paint1,LIST_ICON_X_MID1*rel_factor,LIST_ICON_Y_MID*rel_factor,LIST_ICON_SYMBOL_SIZE*rel_factor,g[gno].p[sno].sym,g[gno].p[sno].symchar);
        drawSimpleSymbol(paint1,LIST_ICON_X_MID1*rel_factor+LIST_ICON_X_SHIFT*rel_factor,LIST_ICON_Y_MID*rel_factor,LIST_ICON_SYMBOL_SIZE*rel_factor,g[gno].p[sno].sym,g[gno].p[sno].symchar);
        }
        else
        {
        drawSimpleSymbol(paint1,LIST_ICON_X_MID2*rel_factor,LIST_ICON_Y_MID*rel_factor,LIST_ICON_SYMBOL_SIZE*rel_factor,g[gno].p[sno].sym,g[gno].p[sno].symchar);
        }
}
paint1.end();
}

return QIcon(map1);
}

uniList::uniList(int type,QWidget *parent):QListWidget(parent)
{
setIconSize(QSize(LIST_ICON_WIDTH,LIST_ICON_HEIGHT));
    minimum_display=false;
    prevent_from_autoupdate=false;
    datType=type;
    partner=NULL;
    show_all_sets_marker=false;
    all_entries_option_selected=false;
    drag_start_row=drag_stop_row=-1;
    nr_sel=0;
    sel_entries=NULL;
    //allowSnycCommand=false;

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

    show_hidden=DefaultSetListShowHidden;
    show_data_less=DefaultSetListShowDataless;
    show_comments=DefaultSetListShowComments;
    show_legends=DefaultSetListShowLegends;
    show_icons=DefaultSetListShowIcons;

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

    if (allow_drag_in_lists==TRUE)
    {/// experimental
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::InternalMove);
    }
    else
    {
    setDragEnabled(false);
    viewport()->setAcceptDrops(false);
    setDropIndicatorShown(false);
    setDragDropMode(QAbstractItemView::NoDragDrop);
    }
}

uniList::~uniList()
{
    if (entries)
    delete[] entries;
    if (text_entries)
    delete[] text_entries;
}

void uniList::update_number_of_entries(void)
{
    char dummy[2048];
    char hidden_char;
    int cols;
    int index=0;
    QString itemtext;
    QIcon tmp_icon;
    //qDebug() << "Update Number of Entries";
    disconnect(this,SIGNAL(itemSelectionChanged()),this,SLOT(new_selection()));
    clear();
    if (datType==GRAPHLIST)
    {
        delete[] entries;
        entries=new int[number_of_graphs()>1?number_of_graphs()+1:2+1];
        if (show_all_sets_marker==true && number_of_graphs()>1)
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
        //we count the number of entries
        cols=0;
        for (int i=0;i<g[gr_no].maxplot;i++)
        {
        if (!((show_data_less == true || is_set_active(gr_no, i) == TRUE) && (show_hidden == true || is_set_hidden(gr_no, i) != TRUE ))) continue;
        cols++;
        }
        if (show_all_sets_marker==true && cols>1)//only show all-entries-marker if more than one set is present
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
                itemtext=QString(dummy);
                if (show_legends)
                {
                    /*strcat(dummy," \"");
                    strcat(dummy,QString::fromUtf8(g[gr_no].p[i].orig_lstr).toLocal8Bit().constData());
                    strcat(dummy,"\"");*/
                    itemtext+=QString(" \"")+QString::fromUtf8(g[gr_no].p[i].orig_lstr)+QString("\"");
                }
                if (show_comments)
                {
                    /*if (show_legends) strcat(dummy,", ");
                    strcat(dummy," \"");
                    strcat(dummy,QString::fromUtf8(g[gr_no].p[i].orig_comments).toLocal8Bit().constData());
                    strcat(dummy,"\"");*/
                    if (show_legends) itemtext+=QString(", ");
                    itemtext+=QString(" \"")+QString::fromUtf8(g[gr_no].p[i].orig_comments)+QString("\"");
                }
            }
            else//setChoice
            {
                sprintf(dummy, "S%d", i);
                itemtext=QString(dummy);
                if (minimum_display==false)
                {
                itemtext+=QString(" (N=")+QString::number(getsetlength(gr_no, i))+QString(", ")+QString::fromUtf8(g[gr_no].p[i].orig_comments)+QString(")");
                //sprintf(dummy, "S%d (N=%d, %s)", i, getsetlength(gr_no, i), getcomment(gr_no, i));
                }
            }
            if (show_icons)
            {
            double rel_factor=this->font().pixelSize()/13.0;
            //qDebug() << "pixelSize=" << this->font().pixelSize() << " rel_faktor=" << rel_factor;
            tmp_icon=createIconForSetAppearance(gr_no,i,rel_factor);
            this->setIconSize(QSize(LIST_ICON_WIDTH*rel_factor,LIST_ICON_HEIGHT*rel_factor));
            new QListWidgetItem(tmp_icon, itemtext, this);
            }
            else
            {
            new QListWidgetItem(itemtext, this);
            }
                if (g[gr_no].p[i].hidden==0)
                this->item(index)->setForeground(Qt::black);
                else
                this->item(index)->setForeground(Qt::gray);
            entries[index]=i;
            index++;
        }
    }
    number_of_entries=index;
this->setFont(qApp->font());
    connect(this,SIGNAL(itemSelectionChanged()),this,SLOT(new_selection()));
}

void uniList::update_number_of_entries_preserve_selection(void)
{
    int ns,*sel=new int[2];
    disconnect(this,SIGNAL(itemSelectionChanged()),this,SLOT(new_selection()));
    get_selection(&ns,&sel);

/*qDebug() << "selection:";
for (int j=0;j<ns;j++)
qDebug() << j << ": " << sel[j];
qDebug() << "number_of_entries=" << this->number_of_entries;*/

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
        {
            new QListWidgetItem(text_entries[i], this);
        //cout << "update TEXTLIST: #" << text_entries[i].toLocal8Bit().constData() << "#" << endl;
        }
    }
    else
    {
        update_number_of_entries();
    }
    set_new_selection(ns,sel);
    new_selection();
    //}
    if (sel) delete[] sel;
    connect(this,SIGNAL(itemSelectionChanged()),this,SLOT(new_selection()));
}

void uniList::set_graph_number(int gno,bool keep_selection)
{
    if (gno<0) gr_no=get_cg();
    else gr_no=gno;
int s_nr,*sel=new int[2];
    get_selection(&s_nr,&sel);
    update_number_of_entries();
    if (datType==GRAPHLIST)
        item(gno)->setSelected(TRUE);
    else if (keep_selection==true)
        set_new_selection(s_nr,sel);
    if (sel) delete[] sel;

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

void uniList::get_selection(int * number,int ** selection)
{
    if (count()<=0)
    {
    *number=0;
    return;
    }
    QModelIndexList list=selectedIndexes();
    *number=list.size();
    if (*number<=0) return;//return without deleting anything
        if (*selection!=NULL)
        {
        delete[] *selection;
        *selection=NULL;
        }
    if (entries[list.at(0).row()]<0)//all_entries
    {
        all_entries_option_selected=true;
        *number=number_of_entries-1;
        *selection=new int[*number+2];
        for (int i=0;i<*number;i++)
            *(*selection+i)=entries[i+1];
    }
    else
    {
        all_entries_option_selected=false;
        *selection=new int[*number+2];
        for (int i=0;i<*number;i++)
            *(*selection+i)=entries[list.at(i).row()];
    }
    sort(*number,*selection);
}

void uniList::get_all_entries(int * number,int ** selection)
{
    if (count()<=0)
    {
    *number=0;
    return;
    }
    if (show_all_sets_marker==true && number_of_entries>1)
    *number=number_of_entries-1;
    else
    *number=number_of_entries;
    if (*number<=0) return;//return without deleting anything
        if (*selection!=NULL)
        {
        delete[] *selection;
        *selection=new int[*number+2];
        }
    if (show_all_sets_marker==true && number_of_entries>1)//all_entries marker present -- copy all but first entry
    {
        for (int i=1;i<=*number;i++)
        *(*selection+i-1)=entries[i];
    }
    else//no all_entries-marker -- copy all
    {
        for (int i=0;i<*number;i++)
        *(*selection+i)=entries[i];
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
    if (mainWin->highlightTimer->isActive() && this->hasFocus())
    {
    mainWin->stop_highlighting();
    QKeyEvent ev_key(QEvent::KeyPress,Qt::Key_H,Qt::NoModifier);
    this->keyPressEvent(&ev_key);
    }
    else if (!mainWin->highlightTimer->isActive() && this->hasFocus() && auto_highlight==TRUE)
    {
    //qDebug() << "Now we should start the highlighting";
    activateHighlighting();
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
        e->accept();
        if (datType==SETLIST)
        {
            if (popupMenu1==NULL) return;
            popupMenu1->selected_graph=gr_no;
            get_selection(&popupMenu1->number_of_selected_sets,&popupMenu1->selected_sets);
            sort(popupMenu1->number_of_selected_sets,popupMenu1->selected_sets);
            popupMenu1->update_menu_content();
            (void)popupMenu1->exec(QCursor::pos());
            //popupMenu1->exec(e->globalPos());
        }
        else if (datType==GRAPHLIST)
        {
            if (popupMenu2==NULL) return;
            get_selection(&popupMenu2->number_of_selected_graphs,&popupMenu2->selected_graphs);
            sort(popupMenu2->number_of_selected_graphs,popupMenu2->selected_graphs);
            popupMenu2->selected_no=selected_no;
            popupMenu2->update_menu_content();
            (void)popupMenu2->exec(QCursor::pos());
            //QListWidget::mousePressEvent(e);
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

void uniList::activateHighlighting(void)
{
    if (mainWin->highlightTimer->isActive()) return;
//qDebug() << "start highlighting";
int nr_sel,*sel=new int[2],*g_sel=new int[2];
get_selection(&nr_sel,&sel);
    if (datType==GRAPHLIST)
    {
    prepare_highlight_segments_with_graphs(nr_sel,sel);
    mainWin->start_highlighting();
    }
    else if (datType==SETLIST)
    {
    delete[] g_sel;
    g_sel=new int[nr_sel];
    for (int j=0;j<nr_sel;j++) g_sel[j]=gr_no;
    prepare_highlight_segments_with_sets(nr_sel,g_sel,sel,TRUE);
    mainWin->start_highlighting();
    }
if (sel!=NULL) delete[] sel;
if (g_sel!=NULL) delete[] g_sel;
}

void uniList::keyPressEvent(QKeyEvent * e)
{
int k=e->key();
if (k==Qt::Key_H)
{
//qDebug() << "H-key pressed";
    if (mainWin->highlightTimer->isActive())
    mainWin->stop_highlighting();
    else
    activateHighlighting();
}
/*else if (k==Qt::Key_Escape)
{
mainWin->stop_highlighting();
}*/
else if (k==Qt::Key_Delete)
{
   if (datType==GRAPHLIST)
   {
   popupMenu2->doKill();
   }
   else if (datType==SETLIST)
   {
   popupMenu1->doKill();
   }
}
else if (k==Qt::Key_A && (e->modifiers()&Qt::ControlModifier))
{
    if (datType==SETLIST || (datType==GRAPHLIST && selectionMode()==QAbstractItemView::ExtendedSelection) )
    {
    popupMenu1->doSelectAll();
    }
}
else if (k==Qt::Key_Up || k==Qt::Key_Down)
{
e->accept();
int delta=(k==Qt::Key_Down?1:-1),scroll=-1;
QModelIndexList list=selectedIndexes();
if (list.length()<1) return;//nothing to do, because nothing is selected
int nr_of_sel=list.length(),*sel_ids=new int[2+list.length()];
blockSignals(true);
clearSelection();
//get_selection(&nr_of_sel,&sel_ids);
for (int i=0;i<nr_of_sel;i++)
{
sel_ids[i]=list.at(i).row()+delta;
}
//set_new_selection(nr_of_sel,sel_ids);
for (int i=0;i<nr_of_sel;i++)
{
    if (sel_ids[i]>=0 && sel_ids[i]<count())
    {
    item(sel_ids[i])->setSelected(TRUE);
    if (scroll==-1) scroll=sel_ids[i];
    }
}
if (scroll<0 && count()>0)//no selection any more but there is still something to select
{
    if (k==Qt::Key_Down) scroll=count()-1;
    else if (k==Qt::Key_Up) scroll=0;
item(scroll)->setSelected(TRUE);
}
if (scroll>=0 && scroll<count()) scrollToItem(item(scroll));
if (sel_ids) delete[] sel_ids;
blockSignals(false);
if (scroll>=0 && scroll<count()) emit(new_selection(scroll));
}
else e->ignore();
//QListWidget::keyPressEvent(e);
}

void uniList::keyReleaseEvent(QKeyEvent * e)
{
int k=e->key();
    if (datType==GRAPHLIST || datType==SETLIST)
    {
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
            {
                clearSelection();
            }
            else e->ignore();
        }
        else e->ignore();
    }
}

void uniList::focusOutEvent(QFocusEvent *event)
{
(void)event;
//qDebug() << "H-Key released";
/// qDebug() << "Stop highlight";
mainWin->stop_highlighting();
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
            simple_draw_setting|=SIMPLE_DRAW_GRAPH_FOCUS;
            switch_current_graph(sel[0]);
        //mainWin->mainArea->completeRedraw();
        }
        delete[] sel;
    }
}

void uniList::clear_text_items(void)
{
delete[] text_entries;
delete[] entries;
number_of_entries=0;
clear();
}

void uniList::add_Item(QString item)
{
    QString * str=new QString[number_of_entries+3];
    int * tentr=new int[number_of_entries+2];
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

void uniList::dragEnterEvent(QDragEnterEvent * event)
{
if (allow_drag_in_lists==0) return;

int n_row=this->row(this->itemAt(event->pos()));
//event->accept();
//qDebug() << "dragEnterEvent: row=" << this->currentRow();
//qDebug() << "drag-enter-event-row=" << this->row(this->itemAt(event->pos()));
if (n_row>=0)
{
    if (drag_start_row<0)
    {
    drag_start_row=n_row;
    drag_stop_row=-1;
    }

    if (nr_sel>0)
    {
    delete[] sel_entries;
    nr_sel=0;
    sel_entries=NULL;
    }
get_selection(&nr_sel,&sel_entries);

/*for (int i=0;i<number_of_entries;i++)
{
qDebug() << i << "dragEnabled" << bool(item(i)->flags() & Qt::ItemIsDragEnabled) << "selectEnabled"<< bool(item(i)->flags() & Qt::ItemIsSelectable);
}*/
}

//int mods=event->keyboardModifiers();

//QDragEnterEvent * event2=new QDragEnterEvent(event->pos(),event->dropAction(),event->mimeData(),event->mouseButtons(),0);
QListWidget::dragEnterEvent(event);
}

void uniList::dragLeaveEvent(QDragLeaveEvent * event)
{
if (allow_drag_in_lists==0) return;
//event->accept();
//qDebug() << "dragLeaveEvent: row=" << this->currentRow();
QListWidget::dragLeaveEvent(event);
}

void uniList::dragMoveEvent(QDragMoveEvent * event)
{
if (allow_drag_in_lists==0) return;
//event->accept();
//qDebug() << "dragMoveEvent: row=" << this->currentRow();
//QDragMoveEvent * event2=new QDragMoveEvent(event->pos(),event->dropAction(),event->mimeData(),event->mouseButtons(),0,QEvent::DragMove);
QListWidget::dragMoveEvent(event);
}

int uniList::getNearestDropRow(QPoint pos)
{
QListWidgetItem * item0=this->itemAt(pos);
int row0=this->row(item0);
if (row0<0)//this means we are in empty space, i.e. insert after last entry
{
return entries[this->number_of_entries-1];
}
QRect rec0=this->visualItemRect(item0);
//qDebug() << "Y=" << pos.y();
//qDebug() << "rec0.Y=" << rec0.y() << "y+height=" << rec0.y()+rec0.height() << "y+0.5*height=" << rec0.y()+0.5*rec0.height();
    if (pos.y()<rec0.y()+0.5*rec0.height())
    {//we are actually 'above' a row --> we have to find the next row above this one because we need to know below which row to insert entries
    //qDebug() << "Darüber";
    row0--;
    }//otherwise we already have to correct row
    /*else
    {
    qDebug() << "Darunter";
    }*/
if (row0<0 || row0>=this->number_of_entries)
{
return -1;
}
return entries[row0];
}

void uniList::dropEvent(QDropEvent * event)
{
if (allow_drag_in_lists==0) return;
int n_row=this->row(this->itemAt(event->pos()));
int target_set;
int ret=getNearestDropRow(event->pos());
//event->accept();
//qDebug() << "drop-event-row=" << this->row(this->itemAt(event->pos())) << endl;
//qDebug() << "dropEvent: row=" << this->currentRow();
if (drag_stop_row<0)
{
    if (n_row<0)
    drag_stop_row=this->number_of_entries-1;
    else
    drag_stop_row=n_row;
}
//qDebug() << "From" << drag_start_row << "to" << drag_stop_row;

target_set=entries[drag_stop_row];
//qDebug() << "After Set" << target_set;
//qDebug() << "After Set" << ret;

/*qDebug() << "Shift Set:";
for (int i=0;i<nr_sel;i++)
qDebug() << sel_entries[i];*/

drag_start_row=drag_stop_row=-1;
QListWidget::dropEvent(event);

QList<int> n_list;
createSetList(gr_no,n_list,nr_sel,sel_entries,ret);
//qDebug() << endl << "new_set_list" << n_list;// << endl;
reSortSets(gr_no,n_list);
//undo-stuff
SetsReorderd(gr_no,n_list);
mainWin->UpdateAll();
}

/*void uniList::doSyncNow(void)
{
    emit(syncSelection());
}*/

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
(void)parent;
(void)item;
}

void treeView::AddMinorChild(QTreeWidgetItem * parent,QTreeWidgetItem * item)
{
(void)parent;
(void)item;
}

void treeView::ClearAll(void)
{
    this->clear();
}

void treeView::RecreateCompleteTree(void)
{
    ClearAll();
    this->setIconSize(QSize(16*toolBarSizeFactor,16*toolBarSizeFactor));
    QPixmap tmpPix=ActiveIcon->scaled(this->iconSize());
    QIcon IconActive(tmpPix);
    tmpPix=HiddenIcon->scaled(this->iconSize());
    QIcon IconHidden(tmpPix);

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
    twi->setIcon(0,IconActive);
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
            twi->setIcon(0,IconHidden);
        else
            twi->setIcon(0,IconActive);
        addTopLevelItem(twi);
        //labels
        twi2=new QTreeWidgetItem(twi);
        twi2->setText(0,tr("Title"));
        twi2->setData(0,TREE_ROLE_GRAPH,QVariant(i));
        twi2->setData(0,TREE_ROLE_SET,QVariant(0));
        twi2->setData(0,TREE_ROLE_TYPE,QVariant('L'));
        twi2->setIcon(0,IconActive);
        twi->addChild(twi2);
        twi2=new QTreeWidgetItem(twi);
        twi2->setText(0,tr("Subtitle"));
        twi2->setData(0,TREE_ROLE_GRAPH,QVariant(i));
        twi2->setData(0,TREE_ROLE_SET,QVariant(1));
        twi2->setData(0,TREE_ROLE_TYPE,QVariant('L'));
        twi2->setIcon(0,IconActive);
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
                twi2->setIcon(0,IconHidden);
            else
                twi2->setIcon(0,IconActive);
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
                twi2->setIcon(0,IconHidden);
            else
                twi2->setIcon(0,IconActive);
            twi->addChild(twi2);
        }
        //legend
        twi2=new QTreeWidgetItem(twi);
        twi2->setText(0,tr("Legend"));
        twi2->setData(0,TREE_ROLE_GRAPH,QVariant(i));
        twi2->setData(0,TREE_ROLE_SET,QVariant(-1));
        twi2->setData(0,TREE_ROLE_TYPE,QVariant('E'));
        if (g[i].l.active!=true)
            twi2->setIcon(0,IconHidden);
        else
            twi2->setIcon(0,IconActive);
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
            twi->setIcon(0,IconActive);
        else
            twi->setIcon(0,IconHidden);
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
            twi->setIcon(0,IconActive);
        else
            twi->setIcon(0,IconHidden);
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
            twi->setIcon(0,IconActive);
        else
            twi->setIcon(0,IconHidden);
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
            twi->setIcon(0,IconActive);
        else
            twi->setIcon(0,IconHidden);
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
    (void)col;
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

InLineEditor::InLineEditor(QWidget * parent):QLineEdit(parent)
{
    old_active_value=0;
    old_text=NULL;
edit_type=-1;//0=text;1=title;2=axis-label;3=legend
id1=id2=-1;
//edit_type=0 --> id1=graph_nr, id2=text-id
//edit_type=1 --> id1=graph_nr, id2=(0=title,1=sub_title)
//edit_type=2 --> id1=graph_nr, id2=axis-nr(x,y,alt-x,alt-y)
//edit_type=3 --> id1=graph_nr, id2=set_nr
bounding_box.setRect(1,1,5,5);//just to set something
this->setFrame(false);
this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
connect(this,SIGNAL(returnPressed()),this,SLOT(finish_editor()));
//QPalette pal;
pal=this->palette();
pal.setColor(QPalette::Base,Qt::transparent);
pal.setColor(QPalette::AlternateBase,Qt::transparent);
pal.setColor(QPalette::Window,Qt::transparent);
pal.setColor(QPalette::Button,Qt::transparent);
pal.setColor(QPalette::Text,Qt::black);
setPalette(pal);
hide();
}

void InLineEditor::init_editor(int type,int id_1,int id_2,int id_3,int id_4)
{
edit_type=type;//0=text;1=title;2=axis-label;3=legend;4=custom-tick
    id1=id_1;
    id2=id_2;
    id3=id_3;
    id4=id_4;
view bb1;
QFont fo1;
pal=this->palette();
//QColor spec_red(255,0,0,100);/// for testing
//pal.setColor(QPalette::Base,spec_red);/// for testing
/// pal.setColor(QPalette::Base,Qt::transparent);
pal.setColor(QPalette::Base,get_Color(getbgcolor()));//because transparent can be obscured by the background (this is not 100% perfect, but effective)
Page_geometry cur_pg=get_page_geometry();
int rot;
double tmp_x,tmp_y,tmp_x2,tmp_y2;
double smaller=cur_pg.height<cur_pg.width?cur_pg.height:cur_pg.width;
this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
int delta=0;
if (edit_type==0)/// Text-Object
{
this->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    delta=0;
    if ( !is_valid_string(id_2) )
    {
    edit_type=-1;
    id1=id2=-1;
    }
    else
    {
    get_object_bb(OBJECT_STRING,id_2,&bb1);
    if (pstr[id_2].loctype==COORD_WORLD)//convert to viewport
    {
    tmp_x=pstr[id_2].x;
    tmp_y=pstr[id_2].y;
    world2view( tmp_x,tmp_y,&tmp_x2,&tmp_y2 );
    bounding_box=QRect(xconvxlib(tmp_x2),win_h-xconvxlib(tmp_y2),win_w,yconvxlib(bb1.yv1)-yconvxlib(bb1.yv2));
    }
    else
    bounding_box=QRect(xconvxlib(pstr[id_2].x),win_h-xconvxlib(pstr[id_2].y),win_w,yconvxlib(bb1.yv1)-yconvxlib(bb1.yv2));
//qDebug() << "Bounding_Box (viewport): (" << bb1.xv1 << "|" << bb1.yv1 << ")-(" << bb1.xv2 << "|" << bb1.yv2 << ")";
    pal.setColor(QPalette::Text,get_Color(pstr[id_2].color));
    fo1=getFontFromDatabase(pstr[id_2].font);
    fo1.setPixelSize(smaller*MAGIC_FONT_SCALE*pstr[id_2].charsize*GeneralPageZoomFactor*universal_font_size_factor);
/*qDebug() << "editor_font_size=" << fo1.pixelSize();
qDebug() << "Grace_font_size=" << pstr[id_2].charsize;
qDebug() << "global_factor=" << GeneralPageZoomFactor;
qDebug() << "global_font_factor=" << universal_font_size_factor;*/
    this->setText(QString::fromUtf8(pstr[id_2].alt_plotstring));
//qDebug() << "Bounding_Box (absolute): (" << bounding_box.x() << "|" << bounding_box.y() << "): w=" << bounding_box.width() << " h=" << bounding_box.height();
    }
}
else if (edit_type==1)/// Title
{
get_graph_viewport(id1,&bb1);
    if (id2==0)//title
    {
    bounding_box=QRect(xconvxlib(bb1.xv1),yconvxlib(bb1.yv2+0.1),xconvxlib(bb1.xv2)-xconvxlib(bb1.xv1),0.05*win_h);
    pal.setColor(QPalette::Text,get_Color(g[id1].labs.title.color));
    fo1=getFontFromDatabase(g[id1].labs.title.font);
    fo1.setPixelSize(smaller*MAGIC_FONT_SCALE*g[id1].labs.title.charsize*GeneralPageZoomFactor*universal_font_size_factor);
    this->setText(QString::fromUtf8(g[id1].labs.title.alt_plotstring));
    }
    else //subtitle
    {
    bounding_box=QRect(xconvxlib(bb1.xv1),yconvxlib(bb1.yv2+0.05),xconvxlib(bb1.xv2)-xconvxlib(bb1.xv1),0.05*win_h);
    pal.setColor(QPalette::Text,get_Color(g[id1].labs.stitle.color));
    fo1=getFontFromDatabase(g[id1].labs.stitle.font);
    fo1.setPixelSize(smaller*MAGIC_FONT_SCALE*g[id1].labs.stitle.charsize*GeneralPageZoomFactor*universal_font_size_factor);
    this->setText(QString::fromUtf8(g[id1].labs.stitle.alt_plotstring));
    }
delta=fo1.pixelSize()*2-bounding_box.height();
bounding_box.setHeight(2*fo1.pixelSize());
bounding_box.setY(bounding_box.y()-delta/2);
}
else if (edit_type==2)/// Axis Label --> y-Achse-bounding-box stimmt nicht!?
{

int ret=get_axis_label_viewport(id1,id2,&bb1,false,&rot,id4);
if (ret==RETURN_FAILURE)
{
edit_type=-1;//initialization failed
errmsg("Invalid axis label for inline-editing (label viewport not found)");
return;
}

bounding_box=QRect(xconvxlib(bb1.xv1),win_h-xconvxlib(bb1.yv1),win_w,yconvxlib(bb1.yv1)-yconvxlib(bb1.yv2));

//get_graph_viewport(id1,&bb1);
//double rot=getOrientationAngle(0,id1,id2);

pal.setColor(QPalette::Text,get_Color(g[id1].t[id2]->label.color));
fo1=getFontFromDatabase(g[id1].t[id2]->label.font);
fo1.setPixelSize(smaller*MAGIC_FONT_SCALE*g[id1].t[id2]->label.charsize*GeneralPageZoomFactor*universal_font_size_factor);

//qDebug() << "Axis Rotation=" << rot;

this->setText(QString::fromUtf8(g[id1].t[id2]->label.alt_plotstring));
}
else if (edit_type==3)/// Legend
{
legend l;
get_legend_entry_bb_for_set(id1,id2,&bb1);
bounding_box=QRect(xconvxlib(bb1.xv1),yconvxlib(bb1.yv1),2*abs(xconvxlib(bb1.xv2)-xconvxlib(bb1.xv1)),abs(xconvxlib(bb1.yv2)-xconvxlib(bb1.yv1)));
bounding_box.setX(bounding_box.x()+2);
bounding_box.setY(bounding_box.y()-4);
get_graph_legend(id1,&l);
pal.setColor(QPalette::Text,get_Color(l.color));
fo1=getFontFromDatabase(l.font);
fo1.setPixelSize(smaller*MAGIC_FONT_SCALE*l.charsize*GeneralPageZoomFactor*universal_font_size_factor);
pal.setColor(QPalette::Base,get_Color(l.boxfillpen.color));
this->setText(QString::fromUtf8(g[id1].p[id2].orig_lstr));
this->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
}
else if (edit_type==4)/// custom tick
{/// Position der bounding box ist etwas daneben!?
    bb1=g[id1].t[id2]->tloc[id3].bb;
    bounding_box=QRect(xconvxlib(bb1.xv1),yconvxlib(bb1.yv1),2*abs(xconvxlib(bb1.xv2)-xconvxlib(bb1.xv1)),abs(xconvxlib(bb1.yv2)-xconvxlib(bb1.yv1)));
    bounding_box.setX(bounding_box.x()+2);
    bounding_box.setY(bounding_box.y()-4);
    pal.setColor(QPalette::Text,get_Color(g[id1].t[id2]->tl_color));
    fo1=getFontFromDatabase(g[id1].t[id2]->tl_font);
    fo1.setPixelSize(smaller*MAGIC_FONT_SCALE*g[id1].t[id2]->tl_charsize*GeneralPageZoomFactor*universal_font_size_factor);
this->setText(QString::fromUtf8(g[id1].t[id2]->tloc[id3].orig_label));
this->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
}
bounding_box.setHeight(fo1.pixelSize()*2);
this->setFont(fo1);
this->setPalette(pal);
/*qDebug() << "bb: x=(" << bb1.xv1 << "|" << bb1.xv2 << ") y=(" << bb1.yv1 << "|" << bb1.yv2 << ")";*/
//qDebug() << "Rect: x0=" << bounding_box.x() << "|" << bounding_box.y() << " w=" << bounding_box.width() << " h=" << bounding_box.height();
}

void InLineEditor::show_editor(void)
{
    if (edit_type==0)//text-editor
    {
    old_active_value=pstr[id2].active;
    pstr[id2].active=0;
    }
    else if (edit_type==1)//title
    {
        if (id2==0)//main title
        {
        old_active_value=g[id1].labs.title.color;
        g[id1].labs.title.color=getbgcolor();
        }
        else//subtitle
        {
        old_active_value=g[id1].labs.stitle.color;
        g[id1].labs.stitle.color=getbgcolor();
        }
    }
    else if (edit_type==2)//axis-label
    {
    old_active_value=g[id1].t[id2]->label.color;
    g[id1].t[id2]->label.color=getbgcolor();
    }
    else if (edit_type==3)//legend
    {
    /*legend l;
    get_graph_legend(id1,&l);
    old_active_value=l.color;
    l.color=l.boxfillpen.color;
    set_graph_legend(id1,&l);*/

    /*if (old_text) delete[] old_text;
    old_text=new char[strlen(g[id1].p[id2].orig_lstr)+8];
    strcpy(old_text,g[id1].p[id2].orig_lstr);
    SetMemoryToText(g[id1].p[id2].lstr,g[id1].p[id2].orig_lstr,QString("  "));*/
    }
    else if (edit_type==4)//custom_tick
    {
        if (old_text!=NULL) delete[] old_text;
        if (g[id1].t[id2]->tloc[id3].label!=NULL)
        {
        old_text=new char[2+strlen(g[id1].t[id2]->tloc[id3].label)];
        strcpy(old_text,g[id1].t[id2]->tloc[id3].label);
        g[id1].t[id2]->tloc[id3].label[0]='\0';
        }
        else
        {
        old_text=NULL;
        }
    }
    mainWin->mainArea->completeRedraw();
    this->show();
    this->setFocus();
}

void InLineEditor::hide_editor(bool redraw=true)
{
    this->hide();
    if (edit_type==0)//text
    {
    pstr[id2].active=old_active_value;
    }
    else if (edit_type==1)//title
    {
        if (id2==0)//main title
        {
        g[id1].labs.title.color=old_active_value;
        }
        else//subtitle
        {
        g[id1].labs.stitle.color=old_active_value;
        }
    }
    else if (edit_type==2)//axis-label
    {
    g[id1].t[id2]->label.color=old_active_value;
    }
    else if (edit_type==3)//legend
    {
    /*legend l;
    get_graph_legend(id1,&l);
    l.color=old_active_value;
    set_graph_legend(id1,&l);*/
    //SetMemoryToText(g[id1].p[id2].lstr,g[id1].p[id2].orig_lstr,QString(old_text));
    }
    else if (edit_type==4)//custom-tick
    {
        if (old_text!=NULL)
        {
            g[id1].t[id2]->tloc[id3].label = copy_string(g[id1].t[id2]->tloc[id3].label,old_text);//just copy text back
            delete[] old_text;
            old_text=NULL;
        }
    }
    emit(editorClosed());
    if (redraw)
    mainWin->mainArea->completeRedraw();
}

void InLineEditor::finish_editor(void)
{
//qDebug() << "Edit and Hide";
char * c1,*c2;
bool displStd=true;
char dummy[16];
sprintf(dummy,"[G%d]",id1);
hide_editor();
if (edit_type==0)//text
{
SaveObjectData(id2,OBJECT_STRING);
    DynSetMemoryToLineEdit(pstr[id2].s_plotstring,pstr[id2].alt_plotstring,c1,c2,displStd,true,this);
ObjectDataModified(id2,OBJECT_STRING);
}
else if (edit_type==1)//title
{
SaveSingleGraphState(id1,UNDO_APPEARANCE);
    if (id2==0)//title
    DynSetMemoryToLineEdit(g[id1].labs.title.s_plotstring,g[id1].labs.title.alt_plotstring,c1,c2,displStd,true,this);
    else//subtitle
    DynSetMemoryToLineEdit(g[id1].labs.stitle.s_plotstring,g[id1].labs.stitle.alt_plotstring,c1,c2,displStd,true,this);
SingleGraphModified(id1,UNDO_APPEARANCE);
addAditionalDescriptionToLastNode(-1,tr("Graph title changed")+QString(dummy),QString(),-1);
}
else if (edit_type==2)//axis-label
{
SaveTickmarksStatesPrevious(id2,id2,id1,id1);
    DynSetMemoryToLineEdit(g[id1].t[id2]->label.s_plotstring,g[id1].t[id2]->label.alt_plotstring,c1,c2,displStd,true,this);
TickmarksModified(id2,id2,id1,id1);
sprintf(dummy,"[G%d.",id1);
switch (id2)
{
case 0:
strcat(dummy,QObject::tr("X-axis").toLocal8Bit().constData());
break;
case 1:
strcat(dummy,QObject::tr("Y-axis").toLocal8Bit().constData());
break;
case 2:
strcat(dummy,QObject::tr("Alt-X-axis").toLocal8Bit().constData());
break;
default:
strcat(dummy,QObject::tr("Alt-Y-axis").toLocal8Bit().constData());
break;
}
strcat(dummy,"]");
addAditionalDescriptionToLastNode(-1,tr("Axis label edited")+QString(dummy),QString(),-1);
}
else if (edit_type==3)//legend
{
SaveSingleSetStatePrevious(id1,id2,UNDO_APPEARANCE);
    SetMemoryToText(g[id1].p[id2].lstr,g[id1].p[id2].orig_lstr,text().toUtf8());
SingleSetModified(id1,id2,UNDO_APPEARANCE);
sprintf(dummy,"[G%d.S%d]",id1,id2);
addAditionalDescriptionToLastNode(-1,tr("Set legend changed")+QString(dummy),QString(),-1);
}
else if (edit_type==4)//custom-tick
{
    DynSetMemoryToLineEdit(g[id1].t[id2]->tloc[id3].label,g[id1].t[id2]->tloc[id3].orig_label,c1,c2,displStd,true,this);
    /*g[id1].t[id2]->tloc[id3].label = copy_string(g[id1].t[id2]->tloc[id3].label,old_text);
    g[id1].t[id2]->tloc[id3].orig_label = copy_string(g[id1].t[id2]->tloc[id3].orig_label,old_text);*/
    delete[] old_text;
    old_text=NULL;
}
set_dirtystate();
mainWin->mainArea->completeRedraw();
}

void InLineEditor::keyPressEvent(QKeyEvent * event)
{
event->accept();
    if (event->key()==Qt::Key_Escape)
    {
    hide_editor();
    }
    else
    {
    QLineEdit::keyPressEvent(event);
    }
}

void InLineEditor::focusOutEvent(QFocusEvent * event)
{
(void)event;
//qDebug() << "Focus Out";
hide_editor();
}

InLineEditor2::InLineEditor2(QWidget * parent):QGraphicsView(parent)
{
lenEdit=new InLineEditor(0);
scene=new QGraphicsScene(this);
proxy = scene->addWidget(lenEdit);
transpPalette=this->palette();
transpPalette.setColor(QPalette::Base,Qt::transparent);
transpPalette.setColor(QPalette::AlternateBase,Qt::transparent);
transpPalette.setColor(QPalette::Window,Qt::transparent);
transpPalette.setColor(QPalette::Button,Qt::transparent);
transpPalette.setColor(QPalette::Text,Qt::black);

scene->setPalette(transpPalette);
lenEdit->setPalette(transpPalette);
proxy->setPalette(transpPalette);
this->setPalette(transpPalette);

scene->setBackgroundBrush(Qt::transparent);
this->setBackgroundBrush(Qt::transparent);

setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

this->setScene(scene);
this->setFrameStyle(QFrame::NoFrame);
/// QRectF ref=scene->sceneRect();
/// qDebug() << "x=" << ref.x() << " y=" << ref.y() << " w=" << ref.width() << " h=" << ref.height();
hide();
connect(lenEdit,SIGNAL(editorClosed()),this,SLOT(hide()));
connect(lenEdit,SIGNAL(editorClosed()),this,SLOT(resetRotation()));
}

void InLineEditor2::init_editor(int type,int id_1,int id_2,int id_3,int id_4)
{
lenEdit->init_editor(type,id_1,id_2,id_3,id_4);
//qDebug() << "inited editor: x=" << lenEdit->bounding_box.x() << " y=" << lenEdit->bounding_box.y();
}

void InLineEditor2::show_editor(double angle)
{
    int x_shift,y_shift,xp,yp;
    double sa=sin(angle*M_PI/180.0);
    double ca=cos(angle*M_PI/180.0);
    double tmp1,tmp2;

#ifdef MAC_SYSTEM
    #if QT_VERSION >= 0x050000
    x_shift=0;
    y_shift=-1;
    #else
    x_shift=14;
    y_shift=-2;
    #endif
#endif
#ifdef WINDOWS_SYSTEM
    x_shift=2;
    y_shift=0;
#endif
#ifdef LINUX_SYSTEM
    x_shift=2;
    y_shift=0;
#endif

lenEdit->show_editor();
xp=lenEdit->bounding_box.x();
yp=lenEdit->bounding_box.y();

this->setGeometry(mainWin->mainArea->lblBackGr->rect());
scene->setSceneRect(mainWin->mainArea->lblBackGr->rect());

lenEdit->setGeometry(0,0,lenEdit->bounding_box.width(),lenEdit->bounding_box.height());

QRect rec=lenEdit->style()->itemTextRect(lenEdit->fontMetrics(),lenEdit->rect(),lenEdit->alignment(),true,lenEdit->text());
x_shift+=rec.x();
y_shift+=(lenEdit->height()-rec.height())/2+rec.height()-lenEdit->fontMetrics().descent();

proxy->resetTransform();
proxy->setRotation(-angle);

tmp1=ca*x_shift+sa*y_shift;
tmp2=-sa*x_shift+ca*y_shift;

x_shift=rint_2(tmp1);
y_shift=rint_2(tmp2);

qDebug() << "xp=" << xp << " yp(von unten)=" << yp << " yp(von oben)=" << this->height()-yp;
qDebug() << "x_shift=" << x_shift << " y_shift=" << y_shift;
qDebug() << "proxy.x=" << proxy->x() << " proxy.y=" << proxy->y();

/// proxy->moveBy(xp-proxy->x()-x_shift,this->height()-(yp-proxy->y()+y_shift));
/// proxy->moveBy(xp,this->height()-yp);
proxy->setPos(xp-x_shift,this->height()-yp-y_shift);
/*
qDebug() << "rec.x=" << rec.x() << " y=" << rec.y() << " w=" << rec.width() << " h=" << rec.height() << " baseline=" << baseline;
rec=lenEdit->style()->itemTextRect(lenEdit->fontMetrics(),lenEdit->rect(),Qt::AlignLeft,true,lenEdit->text());
qDebug() << "Align Left rec.x=" << rec.x() << " y=" << rec.y() << " w=" << rec.width() << " h=" << rec.height() << " baseline=" << baseline;
qDebug() << "lenEdit.rect=" << lenEdit->rect();

start_pos_x=rec.x();
start_pos_y=rec.y()+baseline;

qDebug() << "TextStart: x=" << start_pos_x << " y=" << start_pos_y;
//lenEdit->getTextMargins(&x,&y2,&x2,&y);
//qDebug() << "textMargin: x=" << x << " y=" << y << " x2=" << x2 << " y2=" << y2;
QPoint textPos=lenEdit->mapToParent(QPoint(start_pos_x,start_pos_y));
x=textPos.x();
y=textPos.y();
qDebug() << "lenEdit: position in parent: x=" << textPos.x() << " y=" << textPos.y();
QPointF textPos2=proxy->mapToParent(textPos);
textPos2=this->mapToScene(x,y);
qDebug() << "in scene: x=" << x << " y=" << y;
//textPos=this->mapFromScene(textPos2);
qDebug() << "proxy_pos: x=" << proxy->scenePos().x() << " y=" << proxy->scenePos().y();
textPos=this->mapFromScene(proxy->pos());
//textPos=textPos2.toPoint();
qDebug() << "proxy: position in Scene: x=" << textPos.x() << " y=" << textPos.y();

textPos=this->mapFromScene(QPoint(x,y));
qDebug() << "proxy: position in Scene: x=" << textPos.x() << " y=" << textPos.y();


textPos=this->mapToParent(textPos);
qDebug() << "this: position in parent: x=" << textPos.x() << " y=" << textPos.y();

QRect rec2=this->mapFromScene(proxy->rect()).boundingRect();
qDebug() << "lenEdit.rect=" << lenEdit->rect();
qDebug() << "proxy.rect  =" << proxy->rect();
qDebug() << "this.rect   =" << this->rect();
qDebug() << "rec2        =" << rec2;
*/

this->show();
this->setFocus();
proxy->setFocus();
lenEdit->setFocus(Qt::MouseFocusReason);
}

void InLineEditor2::resetRotation(void)
{
this->resetTransform();
}

void InLineEditor2::hide_editor(void)
{
lenEdit->hide_editor();
this->hide();
//this->resetTransform();
/*this->rotate(-rot_angle);
rot_angle=0.0;*/
}

void InLineEditor2::finish_editor(void)
{
lenEdit->finish_editor();
this->hide();
//this->resetTransform();
/*this->rotate(-rot_angle);
rot_angle=0.0;*/
}

void InLineEditor2::setRotationAngle(double angle)
{
this->rotate(angle);
}

void InLineEditor2::getTextOffset(int & x_shift,int & y_shift)
{
#ifdef MAC_SYSTEM
    #if QT_VERSION >= 0x050000
    x_shift=0;
    y_shift=0;
    #else
    double s=lenEdit->font().pixelSize();
    /*x_shift=(int)rint_2(-0.16765+0.89191*s);
    y_shift=-(int)rint_2(-0.26912+0.11103*s);*/
    x_shift=(int)rint_2(1.05477*s);//14
    y_shift=-(int)rint_2(-0.0150101*s);//-2;
    #endif
#endif
#ifdef WINDOWS_SYSTEM
    double s=lenEdit->font().pixelSize();
    x_shift=(int)rint_2(-0.144219*s);//2;
    y_shift=0;
#endif
#ifdef LINUX_SYSTEM
    x_shift=1;
    y_shift=0;
#endif
}

void InLineEditor2::init_and_show_editor(int type,int id_1,int id_2, int id_3,int id_4)
{
this->setGeometry(mainWin->mainArea->lblBackGr->rect());
//scene->setSceneRect(mainWin->mainArea->lblBackGr->rect());//important!!!
scene->setSceneRect(0,0,mainWin->mainArea->lblBackGr->width(),mainWin->mainArea->lblBackGr->height());
lenEdit->init_editor(type,id_1,id_2,id_3,id_4);
if (lenEdit->edit_type<0) return;//initialization failed
lenEdit->show_editor();
lenEdit->resize(lenEdit->bounding_box.width(),lenEdit->bounding_box.height());
proxy->setPos(scene->width()/2,scene->height()/2);//for safety
//find the angle
double rot=0.0;
int rot2;
view vp;
QPoint target_center;
QPoint current_center;
//type: 0=text;1=title;2=axis-label;3=legend;4=custom_tick
//type=0 --> id1=graph_nr, id2=text-id
//type=1 --> id1=graph_nr, id2=(0=title,1=sub_title)
//type=2 --> id1=graph_nr, id2=axis-nr(x,y,alt-x,alt-y)
//type=3 --> id1=graph_nr, id2=set_nr
//type=4 --> id1=graph_nr, id2=axis_nr, id3=tick_nr
switch (type)
{
case 0:
rot=getOrientationAngle(1,OBJECT_STRING,id_2);//type=0=axis, type=1=object / nr=axis-nr or object-nr / gno=graph-nr or object-type for objects
break;
case 1://title is never rotated
case 3://legend is never rotated
rot=0.0;
break;
case 2:
(void)get_axis_label_viewport(id_1,id_2,&vp,false,&rot2,id_4);
rot=1.0*rot2;
/*qDebug() << "viewport=" << vp.xv1 << "|" << vp.yv1 << " -- " << vp.xv2 << "|" << vp.yv2;
qDebug() << "pixel_values=" << xconvxlib(vp.xv1) << "|" << yconvxlib(vp.yv1) << " -- " << xconvxlib(vp.xv2) << "|" << yconvxlib(vp.yv2);*/
target_center=QPoint((xconvxlib(vp.xv1)+xconvxlib(vp.xv2))*0.5,(yconvxlib(vp.yv1)+yconvxlib(vp.yv2))*0.5);
//rot=getOrientationAngle(0,id_1,id_2);
break;
case 4:
rot=g[id_1].t[id_2]->tl_angle*180.0/M_PI;
break;
}
//now we position the editor
int xp=lenEdit->bounding_box.x(),yp=lenEdit->bounding_box.y();
    //this->centerOn(xp,yp);
//qDebug() << "Text=" << lenEdit->text() << "Position: xp=" << xp << " y=" << yp << " Rotation=" << rot;
    int x_shift,y_shift;
    double sa=sin(rot*M_PI/180.0);
    double ca=cos(rot*M_PI/180.0);
    double tmp1,tmp2;
/*#ifdef MAC_SYSTEM
    #if QT_VERSION >= 0x050000
    x_shift=0;
    y_shift=-1;
    #else
    x_shift=14;
    y_shift=-2;
    #endif
#endif
#ifdef WINDOWS_SYSTEM
    x_shift=2;
    y_shift=0;
#endif
#ifdef LINUX_SYSTEM
    x_shift=2;
    y_shift=0;
#endif*/

getTextOffset(x_shift,y_shift);

QRect rec=lenEdit->style()->itemTextRect(lenEdit->fontMetrics(),lenEdit->rect(),lenEdit->alignment(),true,lenEdit->text());
x_shift+=rec.x();
y_shift+=(lenEdit->height()-rec.height())/2+rec.height()-lenEdit->fontMetrics().descent();

proxy->resetTransform();
proxy->setRotation(-rot);

tmp1=ca*x_shift+sa*y_shift;
tmp2=-sa*x_shift+ca*y_shift;

x_shift=rint_2(tmp1);
y_shift=rint_2(tmp2);

//qDebug() << "x_shift=" << x_shift << " y_shift=" << y_shift;

if (lenEdit->alignment() & Qt::AlignHCenter)
proxy->moveBy(xp-proxy->x(),yp-proxy->y());
else
proxy->moveBy(xp-proxy->x()-x_shift,yp-proxy->y()-y_shift);

if (type==2)
{
    if (rot==0)
    {
    current_center=QPoint(proxy->x()+0.5*proxy->size().width(),proxy->y()+0.5*proxy->size().height());
    proxy->moveBy(target_center.x()-current_center.x(),target_center.y()-current_center.y());
    }
    else
    {
    y_shift=abs(xconvxlib(vp.yv1)-xconvxlib(vp.yv2));
    current_center=QPoint(proxy->x()+0.5*proxy->rect().height(),proxy->y()+0.5*proxy->rect().width());
    proxy->moveBy(target_center.x()-current_center.x(),-target_center.y()+current_center.y()-y_shift);
    }
/*
qDebug() << "current_center=" << current_center << " size=" << proxy->size() << " rect=" << proxy->rect();
qDebug() << "target_center =" << target_center;
    if (rot==0)
    current_center=QPoint(proxy->x()+0.5*proxy->size().width(),proxy->y()+0.5*proxy->size().height());
    else
    current_center=QPoint(proxy->x()+0.5*proxy->rect().height(),proxy->y()+0.5*proxy->rect().width());
qDebug() << "new current_center=" << current_center;
qDebug() << "proxy.rect=" << proxy->rect();
*/
}

//scene->setSceneRect(0,0,mainWin->mainArea->lblBackGr->width(),mainWin->mainArea->lblBackGr->height());
this->show();
this->setFocus();
proxy->setFocus();
lenEdit->setFocus(Qt::MouseFocusReason);
}

SpinBoxAction::SpinBoxAction(const QString & title1,const QString & title2,QWidget * parent):QWidgetAction(parent)
{
empty=new QWidget(NULL);
spnValue=new QSpinBox(NULL);
spnValue->setRange(100,3000);
spnValue->setSingleStep(100);
lblText1=new QLabel(title1);
lblText2=new QLabel(title2);
layout=new QHBoxLayout();
/*//layout->setMargin(0);
layout->setContentsMargins(0,0,0,0);
layout->setSpacing(0);*/
layout->addWidget(lblText1);
layout->addWidget(spnValue);
layout->addWidget(lblText2);
empty->setLayout(layout);
setDefaultWidget(empty);
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
    //int retval=activateset(gno,setno);
    (void)activateset(gno,setno);
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
        for (unsigned int j=0;j<i;j++)
            positions[i]+=inst.column_type[j].size;
        //cout << "column_offset[" << i << "]=" << positions[i] << endl;
    }
    QByteArray ba,ba_tmp;
    int res=LoadFileIntoByteArray(QString(filename),ba);
    if (res==RETURN_FAILURE)
    {
    qDebug() << "UNABLE TO LOAD FORM FILE " << QString(filename);
    }
    int general_offset=0;

    ifstream ifi;
    ifi.open(filename);

    if (inst.length_of_header>0)
    {
        ifi.seekg(inst.length_of_header);
        general_offset=inst.length_of_header;
    }

    while (startcount!=0)
    {
        ba_tmp=ba.mid(general_offset,length_of_one_column_set);
        memcpy(dest,ba_tmp.data(),sizeof(char)*length_of_one_column_set);
        general_offset+=length_of_one_column_set;/// changed to += (was +)

        ifi.read(dest,length_of_one_column_set);//read one set of columns in one go into dest
        readbytes=general_offset;
        //readbytes=(int)(ifi.gcount());
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
    int index=0,offset=0;//,a,b;
    char * tmpstr=NULL;
    char next_char;
    QStringList list;
    QString original(str);
    original=original.toUpper();
    //cout << "String=#" << original.toLatin1().constData() << "#" << endl;
    //QRegExp regex1("G\\d+\\.S\\d+");//what we search for as a regular expression --> we only search for complete set-ids
    QRegularExpression regex1("G\\d+\\.S\\d+");
    QRegularExpressionMatch rm;
    while (offset<strlength)
    {
        rm=regex1.match(original,offset);
        found[index].pos=rm.capturedStart();
        //found[index].pos=regex1.indexIn(original,offset);//find next set-id
        if (found[index].pos==-1)//nothing found
        {
            offset=strlength;
            break;
        }
        list=rm.capturedTexts();
        //list=regex1.capturedTexts();//get the found text
        found[index].len=rm.capturedLength();
        //found[index].len=regex1.matchedLength();//get the length of the found text
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
    int index=0,offset=0;//,a,b;
    char * tmpstr=NULL,du;
    char next_char;
    QStringList list;
    QString original(str);
    original=original.toUpper();
    //QRegExp regex1("\\sG\\d+(?!:\\.|\\S)");//what we search for as a regular expression
    QRegularExpression regex1("\\sG\\d+(?!:\\.|\\S)");//what we search for as a regular expression
    QRegularExpressionMatch rm;
    //cout << "String=#" << original.toLatin1().constData() << "# (start=47)" << endl;
    while (offset<strlength)
    {
        rm=regex1.match(original,offset);
        found[index].pos=rm.capturedStart()+1;
        //found[index].pos=regex1.indexIn(original,offset)+1;//find next set-id
        if (found[index].pos<=0)//nothing found
        {
            offset=strlength;
            break;
        }
        list=rm.capturedTexts();
        //list=regex1.capturedTexts();//get the found text
        found[index].len=rm.capturedLength()-1;
        //found[index].len=regex1.matchedLength()-1;//get the length of the found text
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
            //cout << "next_char=#" << next_char << "#" << endl;
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
                (*foundIds)[j].color=cur_col;
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

QString ColorToHtml(int r,int g,int b)
{
    static char dummy[16];
    sprintf(dummy,"%02x%02x%02x",r,g,b);
    return QString("<span style=\"color:#") + QString(dummy) + QString("\">");
}

QString ColorToHtml(QRgb col)
{
return ColorToHtml(qRed(col),qGreen(col),qBlue(col));
}

QString ColorHtmlEnd(void)
{
    return QString("</span>");
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
//cout << "tokens=" << nr_of_single_f_tokens << endl;
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
    //cout << formula_tokens[i].representation << " type=" << (formula_tokens[i].type==0?"COUNTER":"VALUE") << " --> n_value=" << n_value.toLatin1().constData() << endl;
    new_formula.replace(formula_tokens[i].representation,n_value);
    //cout << "replaced" << endl;
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
//static QRegExp exp1("N{1}\\d+[\\#\\$]{1}");
static QRegularExpression exp1("N{1}\\d+[\\#\\$]{1}");
static QRegularExpressionMatch rm;
rm=exp1.match(QString(formula));
int ret=rm.capturedStart();// exp1.indexIn(QString(formula));
    if (ret!=-1)
    {
    //QStringList list=exp1.capturedTexts();
    QStringList list=rm.capturedTexts();
    QString st=list.at(0);
    n_token->reset_token(st.toLatin1().data());
    }
return ret;
}

int find_all_single_formula_tokens(char * formula,class single_formula_token ** n_token)
{
int anz=0;
int anz_loc=10,pos,ret;
//int len=strlen(formula);
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
    //cout << (type==0?"COUNTER":"VALUE") << " reseting token to " << representation << endl;
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

//the following function goes throught text and tries to find the first set- or graph-id and returns what has been found (an ID), important is also the length len for the later replacement; ID-Type is returned
int searchTextForFirstID(QString text,int start,int & pos,int & g_no,int & s_no,char & control1,char & control2,char & col,int & leng)
{
/// Reminder on what special ID-commands mean:
/// G0.S0N --> generate a new set in G0
/// G0N.S0 / G0N.S0N --> generate a new graph (and a new set)
/// G0.S0$ --> do no replacing --> always use G0.S0
/// G0.S0# --> use G0.S0 the first time / G0.S1 the second time / G0.S2 the third time ... G0.S(k-1) the k-th time
/// G# --> just replace with graph-id (depends on selection) --> e.g. 0
/// S# --> just replace with set-id (depends on selection) --> e.g. 0
int ret=ID_NONE,pos2;
QStringList foundList;
QString foundString;
char * c_found_string=NULL;
QRegularExpressionMatch rm;
//for the following regular expressions '.X' or '.Y' at the end is optional
QRegularExpression RegularSetID("[Gg]{1}\\d+\\.{1}[Ss]\\d+\\.?[XYxy]?");//A complete regular set-ID
QRegularExpression NewGraphStdSetID("[Gg]{1}\\d+[N\\#\\$]{1}\\.{1}[Ss]{1}\\d+\\.?[XYxy]?");//New GraphID, regular set-ID
QRegularExpression StdGraphNewSetID("[Gg]{1}\\d+\\.{1}[Ss]{1}\\d+[N\\#\\$]{1}\\.?[XYxy]?");//Regular GraphID, new set-ID
QRegularExpression NewGraphNewSetID("[Gg]{1}\\d+[N\\#\\$]{1}\\.{1}[Ss]{1}\\d+[N\\#\\$]{1}\\.?[XYxy]?");//New GraphID, new set-ID
QRegularExpression SetIDOnly("[Ss]{1}\\d+\\.?[XYxy]?");//set-ID only
QRegularExpression NewSetIDOnly("[Ss]{1}\\d+[N\\#\\$]{1}\\.?[XYxy]?");//New set-ID only
QRegularExpression GraphIDOnly("[Gg]{1}\\d+(?!(\\.|N|\\#|\\$))");//Graph-ID only (not followed by anything else (especially no '.') )
QRegularExpression NewGraphIDOnly("[Gg]{1}\\d+[N\\#\\$]{1}(?!(\\.|N|\\#|\\$))");//New set-ID only (not followed by anything else (especially no '.') )
QRegularExpression ColIDOnly("[XYxy]{1}\\d?");
QRegularExpression GraphNrOnly("[Gg]{1}\\#{1}(?!(\\.|N|\\#|\\$))");
QRegularExpression SetNrOnly("[Ss]{1}\\#{1}(?!(\\.|N|\\#|\\$))");
QRegularExpression SpecialFormula("\\#{1}QTGRACE_SPECIAL");

QRegularExpression NewNewGraphIDOnly("[Gg]{1}\\d+[N]{2}(?!(\\.|N|\\#|\\$))");
QRegularExpression NewNewSetIDOnly("[Ss]{1}\\d+[N]{2}\\.?[XYxy]?");
QRegularExpression NewNewGraphStdSetID("[Gg]{1}\\d+[N]{2}\\.{1}[Ss]{1}\\d+\\.?[XYxy]?");
QRegularExpression NewNewGraphNewSetID("[Gg]{1}\\d+[N]{2}\\.{1}[Ss]{1}\\d+[N\\#\\$]{1}\\.?[XYxy]?");
QRegularExpression NewNewGraphNewNewSetID("[Gg]{1}\\d+[N]{2}\\.{1}[Ss]{1}\\d+[N]{2}\\.?[XYxy]?");
QRegularExpression StdGraphNewNewSetID("[Gg]{1}\\d+\\.{1}[Ss]{1}\\d+[N]{2}\\.?[XYxy]?");
QRegularExpression NewGraphNewNewSetID("[Gg]{1}\\d+[N]{1}\\.{1}[Ss]{1}\\d+[N]{2}\\.?[XYxy]?");

//G2.S1NN
#define ID_SGNNS 17
//G2N.S2NN
#define ID_NGNNS 18

pos=-1;//nothing found yet
leng=0;
g_no=s_no=-1;
control1=control2=col='\0';
foundString.clear();
foundList.clear();

rm=SpecialFormula.match(text,start);
pos2=rm.capturedStart();
//pos2=SpecialFormula.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=rm.capturedTexts();
//foundList=SpecialFormula.capturedTexts();
foundString=foundList.at(0);
ret=ID_SPECIAL_COMMAND;
}

rm=NewGraphNewSetID.match(text,start);
pos2=rm.capturedStart();
//pos2=NewGraphNewSetID.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=rm.capturedTexts();
//foundList=NewGraphNewSetID.capturedTexts();
foundString=foundList.at(0);
ret=ID_NGNS;
}

rm=NewGraphStdSetID.match(text,start);
pos2=rm.capturedStart();
//pos2=NewGraphStdSetID.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=rm.capturedTexts();
//foundList=NewGraphStdSetID.capturedTexts();
foundString=foundList.at(0);
ret=ID_NGSS;
}

rm=StdGraphNewSetID.match(text,start);
pos2=rm.capturedStart();
//pos2=StdGraphNewSetID.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=rm.capturedTexts();
//foundList=StdGraphNewSetID.capturedTexts();
foundString=foundList.at(0);
ret=ID_SGNS;
}

rm=RegularSetID.match(text,start);
pos2=rm.capturedStart();
//pos2=RegularSetID.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=rm.capturedTexts();
//foundList=RegularSetID.capturedTexts();
foundString=foundList.at(0);
ret=ID_REGULAR_COMPLETE;
}

rm=NewSetIDOnly.match(text,start);
pos2=rm.capturedStart();
//pos2=NewSetIDOnly.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=rm.capturedTexts();
//foundList=NewSetIDOnly.capturedTexts();
foundString=foundList.at(0);
ret=ID_NSET_ONLY;
}

rm=SetIDOnly.match(text,start);
pos2=rm.capturedStart();
//pos2=SetIDOnly.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=rm.capturedTexts();
//foundList=SetIDOnly.capturedTexts();
//qDebug() << foundList;
foundString=foundList.at(0);
ret=ID_SET_ONLY;
}

rm=NewGraphIDOnly.match(text,start);
pos2=rm.capturedStart();
//pos2=NewGraphIDOnly.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=rm.capturedTexts();
//foundList=NewGraphIDOnly.capturedTexts();
foundString=foundList.at(0);
ret=ID_NGRAPH_ONLY;
}

rm=GraphIDOnly.match(text,start);
pos2=rm.capturedStart();
//pos2=GraphIDOnly.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=rm.capturedTexts();
//foundList=GraphIDOnly.capturedTexts();
foundString=foundList.at(0);
ret=ID_GRAPH_ONLY;
}

rm=GraphNrOnly.match(text,start);
pos2=rm.capturedStart();
//pos2=GraphNrOnly.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=rm.capturedTexts();
//foundList=GraphNrOnly.capturedTexts();
foundString=foundList.at(0);
ret=ID_GRAPH_NR_ONLY;
}

rm=SetNrOnly.match(text,start);
pos2=rm.capturedStart();
//pos2=SetNrOnly.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=rm.capturedTexts();
//foundList=SetNrOnly.capturedTexts();
foundString=foundList.at(0);
ret=ID_SET_NR_ONLY;
}

rm=NewNewGraphIDOnly.match(text,start);
pos2=rm.capturedStart();
//pos2=NewNewGraphIDOnly.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=rm.capturedTexts();
//foundList=NewNewGraphIDOnly.capturedTexts();
foundString=foundList.at(0);
ret=ID_NNGRAPH_ONLY;
}

rm=NewNewSetIDOnly.match(text,start);
pos2=rm.capturedStart();
//pos2=NewNewSetIDOnly.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=rm.capturedTexts();
//foundList=NewNewSetIDOnly.capturedTexts();
foundString=foundList.at(0);
ret=ID_NNSET_ONLY;
}

rm=NewNewGraphStdSetID.match(text,start);
pos2=rm.capturedStart();
//pos2=NewNewGraphStdSetID.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=rm.capturedTexts();
//foundList=NewNewGraphStdSetID.capturedTexts();
foundString=foundList.at(0);
ret=ID_NNGSS;
}

rm=NewNewGraphNewSetID.match(text,start);
pos2=rm.capturedStart();
//pos2=NewNewGraphNewSetID.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=rm.capturedTexts();
//foundList=NewNewGraphNewSetID.capturedTexts();
foundString=foundList.at(0);
ret=ID_NNGNS;
}

rm=NewNewGraphNewNewSetID.match(text,start);
pos2=rm.capturedStart();
//pos2=NewNewGraphNewNewSetID.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=rm.capturedTexts();
//foundList=NewNewGraphNewNewSetID.capturedTexts();
foundString=foundList.at(0);
ret=ID_NNGNNS;
}

rm=StdGraphNewNewSetID.match(text,start);
pos2=rm.capturedStart();
//pos2=StdGraphNewNewSetID.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=rm.capturedTexts();
//foundList=StdGraphNewNewSetID.capturedTexts();
foundString=foundList.at(0);
ret=ID_SGNNS;
}

rm=NewGraphNewNewSetID.match(text,start);
pos2=rm.capturedStart();
//pos2=NewGraphNewNewSetID.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=rm.capturedTexts();
//foundList=NewGraphNewNewSetID.capturedTexts();
foundString=foundList.at(0);
ret=ID_NGNNS;
}

rm=ColIDOnly.match(text,start);
pos2=rm.capturedStart();
//pos2=ColIDOnly.indexIn(text,start);//if pos2>0 there may be a singular X or Y
//qDebug() << "column found: pos2=" << pos2 << " pos=" << pos;
if (pos2>=0 && (pos<0 || pos2<pos))
{
//qDebug() << "Accept Column maybe";
char testc1,testc2;
bool to_ignore=false;
    if (pos2==0)//an X or Y at the beginning
    {
        if (text.length()>1)
        {
        testc2=text.at(pos2+1).toLatin1();
            if (testc2=='_' || isalpha(testc2))//ignore (|| isdigit(testc2))
            {
            to_ignore=true;
            }
            else if (isdigit(testc2))
            {
            control1=testc2;
            }
        }//else=only character --> is ok
    }
    else if (pos2>0 && pos2<text.length()-1)//X or Y somewhere in the text but not at beginning or end)
    {
    testc1=text.at(pos2-1).toLatin1();//character before
    testc2=text.at(pos2+1).toLatin1();//character after
        if (testc1=='_' || testc1=='.' || testc2=='_' || isalpha(testc1) || isdigit(testc1) || isalpha(testc2))//ignore
        {
        to_ignore=true;
        }
        else if (isdigit(testc2))
        {
        control1=testc2;
        }
    }
    else if (pos2==text.length()-1)//last position (but not first, because this is the first case)
    {
    testc1=text.at(pos2-1).toLatin1();
        if (testc1=='_' || testc1=='.' || isalpha(testc1) || isdigit(testc1))//ignore
        {
        to_ignore=true;
        }
    }
//qDebug() << "foundString=" << foundString;
//qDebug() << "pos2=" << pos2 << " text=" << text;
        if (to_ignore==false)
        {
        foundList=rm.capturedTexts();
        //foundList=ColIDOnly.capturedTexts();
        foundString=foundList.at(0);
        col=text.at(pos2).toUpper().toLatin1();
        //col=foundString.at(0).toLatin1();
        ret=ID_COLUMN_ONLY;
        pos=pos2;
        }
        else//we found something, that does not fit the description --> go on searching
        {
        return searchTextForFirstID(text,pos2+1,pos,g_no,s_no,control1,control2,col,leng);
        }
}
//qDebug() << "foundString=" << foundString << " col=" << col;
leng=0;
g_no=s_no=-1;
if (ret!=ID_NONE)//we found something --> fill in all informations
{
leng=foundString.length();
c_found_string=new char[8+foundString.length()*2];
strcpy(c_found_string,foundString.toUpper().toLatin1().constData());//we are about to read the entries
int number_of_dots=foundString.count(QString("."));
char tmp_str1[8];
char * parameters;
//qDebug() << "c_found_string=" << c_found_string;
    switch (ret)
    {
    default:
    case ID_REGULAR_COMPLETE://G0.S0.Y
        if (number_of_dots==1)
        sscanf(c_found_string,"G%d.S%d",&g_no,&s_no);
        else
        sscanf(c_found_string,"G%d.S%d.%c",&g_no,&s_no,&col);
    break;
    case ID_NGSS://G0N.S0.Y
        if (number_of_dots==1)
        sscanf(c_found_string,"G%d%c.S%d",&g_no,&control1,&s_no);
        else
        sscanf(c_found_string,"G%d%c.S%d.%c",&g_no,&control1,&s_no,&col);
    break;
    case ID_SGNS://G0.S0N.Y
        if (number_of_dots==1)
        sscanf(c_found_string,"G%d.S%d%c",&g_no,&s_no,&control2);
        else
        sscanf(c_found_string,"G%d.S%d%c.%c",&g_no,&s_no,&control2,&col);
    break;
    case ID_NGNS://G0N.S0N.Y
        if (number_of_dots==1)
        sscanf(c_found_string,"G%d%c.S%d%c",&g_no,&control1,&s_no,&control2);
        else
        sscanf(c_found_string,"G%d%c.S%d%c.%c",&g_no,&control1,&s_no,&control2,&col);
    break;
    case ID_SET_ONLY://S0.Y
        if (number_of_dots==0)
        sscanf(c_found_string,"S%d",&s_no);
        else
        sscanf(c_found_string,"S%d.%c",&s_no,&col);
    break;
    case ID_NSET_ONLY://S0N.Y
        if (number_of_dots==0)
        sscanf(c_found_string,"S%d%c",&s_no,&control2);
        else
        sscanf(c_found_string,"S%d%c.%c",&s_no,&control2,&col);
    break;
    case ID_GRAPH_ONLY://G0
    sscanf(c_found_string,"G%d",&g_no);
    break;
    case ID_NGRAPH_ONLY://G0N
    sscanf(c_found_string,"G%d%c",&g_no,&control1);
    break;
    case ID_COLUMN_ONLY://X or Y
    ;//everthing done before
//qDebug() << "Column=" << col << " Found String=" << foundString << " len=" << leng;
        if (foundString.length()>1)
        {
        sprintf(tmp_str1,"%c",foundString.at(1).toLatin1());
//qDebug() << "ColumnNumber=" << tmp_str1;
        col+=atoi(tmp_str1);
        }
//qDebug() << "NEW Column=" << (int)col;
    break;
    case ID_GRAPH_NR_ONLY://G#
//qDebug() << "GraphNumber=" << foundString;
    break;
    case ID_SET_NR_ONLY://S#
//qDebug() << "SetNumber=" << foundString;
    break;
    case ID_SPECIAL_COMMAND:
    leng=text.length();
    if (c_found_string!=NULL) delete[] c_found_string;
    c_found_string=new char[8+text.length()*2];
    strcpy(c_found_string,text.toUpper().toLatin1().constData());
    control1=(char)containsSpecialCommand(c_found_string,&parameters);
//qDebug() << "Special Command found:" << c_found_string << "Parameters=" << parameters;
        if (control1==SPECIAL_APPEND)//append can be replaces normally
        {
        ret=searchTextForFirstID(text,pos+1,pos,g_no,s_no,control1,control2,col,leng);
        }
//qDebug() << "Special Command Found (" << (int)control1 << "): " << text;
    break;
    }
}
if (c_found_string!=NULL) delete[] c_found_string;
return ret;
}

QString createCompleteID(int type,int n_gno,int n_sno,int o_gno,int o_sno,char control1,char control2,char col)
{
    (void)o_gno;
    (void)control1;
QString ret;
ret.clear();
switch (type)
{
default:
case ID_REGULAR_COMPLETE://G0.S0.Y
case ID_NGSS://G0N.S0.Y
case ID_SGNS://G0.S0N.Y
case ID_NGNS://G0N.S0N.Y
case ID_GRAPH_ONLY://G0
case ID_NGRAPH_ONLY://G0N
case ID_GRAPH_NR_ONLY://G#
case ID_SET_NR_ONLY://S#
break;
case ID_SET_ONLY://S0.Y
    ret=QString("G")+QString::number(n_gno)+QString(".S")+QString::number(o_sno);
break;
case ID_NSET_ONLY://S0N.Y
    ret=QString("G")+QString::number(n_gno)+QString(".S")+QString::number(o_sno)+QString(control2);
break;
case ID_COLUMN_ONLY://X or Y
    ret=QString("G")+QString::number(n_gno)+QString(".S")+QString::number(n_sno);
    //qDebug() << "complete (column only): col=" << col;
break;
}
if (ret.length()>0 && (col=='X' || col=='Y'))
{
ret+=QString(".")+QString(col);
}
return ret;
}

QString createReplacementID(int type, int n_gno, int n_sno, int o_gno, int o_sno, char control1, char control2, char col)
{
QString ret;
ret.clear();
switch (type)
{
default:
case ID_REGULAR_COMPLETE://G0.S0.Y
    ret=QString("G")+QString::number(n_gno)+QString(".S")+QString::number(n_sno);/// ok
break;
case ID_NGSS://G0N.S0.Y
    if (control1=='$')//no replacing
    {
    ret=QString("G")+QString::number(o_gno)+QString(".S")+QString::number(n_sno);/// ok
    }
    else if (control1=='#')//count number upwards after every use
    {
    ;
    }
    else//generate o_gno
    {
    ret=QString("G")+QString::number(n_gno)+QString(".S")+QString::number(n_sno);
    }
break;
case ID_SGNS://G0.S0N.Y
    if (control2=='$')//no replacing
    {
    ret=QString("G")+QString::number(n_gno)+QString(".S")+QString::number(o_sno);/// ok
    }
    else if (control2=='#')//count number upwards after every use
    {
    ;
    }
    else//generate o_sno
    {
    ret=QString("G")+QString::number(n_gno)+QString(".S")+QString::number(n_sno);
    }
break;
case ID_NGNS://G0N.S0N.Y
    if (control1=='$')//no replacing
    {
    ret=QString("G")+QString::number(o_gno);/// ok
    }
    else if (control1=='#')//count number upwards after every use
    {
    ;
    }
    else//generate o_gno + o_sno
    {
    ret=QString("G")+QString::number(n_gno);
    }
    if (control2=='$')//no replacing
    {
    ret+=QString(".S")+QString::number(o_sno);/// ok
    }
    else if (control2=='#')//count number upwards after every use
    {
    ;
    }
    else//generate o_gno + o_sno
    {
    ret+=QString(".S")+QString::number(n_sno);
    }
break;
case ID_SET_ONLY://S0.Y
    ret=QString("G")+QString::number(n_gno)+QString(".S")+QString::number(n_sno);
break;
case ID_NSET_ONLY://S0N.Y
    ret=QString("G")+QString::number(n_gno);
    /// generate s_gno
    ret+=QString("S")+QString::number(o_sno);
break;
case ID_GRAPH_ONLY://G0
    ret=QString("G")+QString::number(n_gno);
break;
case ID_NGRAPH_ONLY://G0N
    if (control1=='$')//no replacing
    {
    ret=QString("G")+QString::number(o_gno);/// ok
    }
    else if (control1=='#')//count number upwards after every use
    {
    ;
    }
    else
    {
    ret=QString("G")+QString::number(n_gno);
    }
    /// generate n_gno
    ret=QString("G")+QString::number(o_gno);
break;
case ID_GRAPH_NR_ONLY://G#
    ret=QString::number(n_gno);
break;
case ID_SET_NR_ONLY://S#
    ret=QString::number(n_sno);
break;
}
if (col=='X' || col=='Y')
{
ret+=QString(".")+QString(col);
}
return ret;
}

formula_to_process::formula_to_process(void)
{
len_orig_formula=0;
original_formula.clear();
n_graph_id_pos=NULL;
nn_graph_id_pos=NULL;
n_set_id_pos=NULL;
nn_set_id_pos=NULL;
//when a formula is initialized, it is analysed for tokens - for easier replacement
clear_tokens();
}

formula_to_process::~formula_to_process()
{
clear_tokens();
}

void formula_to_process::convertStringToUpper(QString & f)
{
QString f2;
QChar qu1('\"'),qu2('\\');
QChar t1,t2;
f2.clear();
bool in_quote=false;
int len=f.length();
if (len<1) return;//nothing to do
for (int i=0;i<len;i++)
{
t2=f.at(i);
    if (i==0)
    t1=f.at(0);
    else
    t1=f.at(i-1);
    if (t2==qu1 && t1!=qu2)//a real quotation character
    {
    in_quote=(!in_quote);
    f2+=f.at(i);
    }
    else
    {
        if (in_quote==true)
        {
        f2+=f.at(i);
        }
        else
        {
        f2+=f.at(i).toUpper();
        }
    }
}
f=f2;
}

void formula_to_process::process_single_subcommand(QString text,int offset,QList<int> & t_startpos,QList<int> & t_type,QList<int> & t_len,QList<int> & t_id1,QList<int> & t_id2,QList<int> & t_pos,QList<char> & t_control1,QList<char> & t_control2,QList<int> & t_column)
{
QStringList subformulas2;
QList<int> suboffsets2;
QString sub_left,sub_right,sub_inner;//we have to split in parts inside and outside quotations
QString sub_left1,sub_right1;//we split each part again to look for '=' --> lhs/rhs
int sub_start=original_formula.indexOf("\"",0);
int sub_end=original_formula.indexOf("\"",sub_start+1);
int current_pos=0,comment_status=0;
bool quotes_found=false,fit_command=false;
//qDebug() << "Process Sub Command(" << offset << ")=" << text;

    fit_command=has_fit_formula_command(text);
    comment_status=is_comment(text);

    if (comment_status==1) return;//ignore this subcommand if it is a comment
    if (comment_status==3)//if-statement
    {
    init_if_statement(text,offset,t_startpos,t_type,t_len,t_id1,t_id2,t_pos,t_control1,t_control2,t_column);//special treatment of this complex construction
    return;
    }

QList<int> t_startpos2;
QList<int> t_type2;
QList<int> t_len2;
QList<int> t_id12;
QList<int> t_id22;
QList<char> t_control12;
QList<char> t_control22;
QList<int> t_column2;

    sub_start=text.indexOf("\"",0);
    sub_end=text.indexOf("\"",sub_start+1);

        if (sub_start==sub_end)
        {
            quotes_found=false;
        sub_left.clear();
        sub_right.clear();
        sub_start=-1;
        sub_end=text.length();
        sub_inner=text;
        }
        else
        {
            quotes_found=true;
        sub_left=text.left(sub_start);
        sub_inner=text.mid(sub_start+1,sub_end-sub_start-1);
        sub_right=text.mid(sub_end+1,-1);
        }
    //sub_start and sub_end are not included in sub_inner

    /*qDebug() << "Formula to analyze:" << text;
    qDebug() << "left=#" << sub_left << "#";
    qDebug() << "inner=#" << sub_inner << "#";
    qDebug() << "right=#" << sub_right << "#";*/

    /// Left part = only part if there is no "" inside the formula
    int equals_pos=sub_left.indexOf("=");
    //offset=0;//we start at the beginning
    if (equals_pos<0)//no '=' in left part
    {
    current_pos=0;
    process_single_string(sub_left,t_startpos2,t_type2,t_len2,t_id12,t_id22,t_control12,t_control22,t_column2);
    //qDebug() << "Left Part contains NO '='. Found tokens=" << t_startpos.length();
            for (int i=0;i<t_startpos2.length();i++)
            {
            t_startpos << t_startpos2.at(i)+offset;
            t_pos << current_pos;
            }
            t_type << t_type2;
            t_len << t_len2;
            t_id1 << t_id12;
            t_id2 << t_id22;
            t_control1 << t_control12;
            t_control2 << t_control22;
            t_column << t_column2;
    }
    else//'=' in left part --> split left part
    {
    sub_left1=sub_left.left(equals_pos);
    sub_right1=sub_left.mid(equals_pos+1,-1);
    current_pos=0;
    process_single_string(sub_left1,t_startpos2,t_type2,t_len2,t_id12,t_id22,t_control12,t_control22,t_column2);
    //qDebug() << "Left Part contains '='. Found tokens (left)=" << t_startpos.length();
    //qDebug() << "left1= #" << sub_left1 << "#";
                for (int i=0;i<t_startpos2.length();i++)
                {
                t_startpos << t_startpos2.at(i)+offset;
                t_pos << current_pos;
                }
                t_type << t_type2;
                t_len << t_len2;
                t_id1 << t_id12;
                t_id2 << t_id22;
                t_control1 << t_control12;
                t_control2 << t_control22;
                t_column << t_column2;
    current_pos=1;
    process_single_string(sub_right1,t_startpos2,t_type2,t_len2,t_id12,t_id22,t_control12,t_control22,t_column2);
    //qDebug() << "Found tokens (right)=" << t_startpos.length();
                    for (int i=0;i<t_startpos2.length();i++)
                    {
                    t_startpos << t_startpos2.at(i)+offset+sub_left1.length()+1;
                    t_pos << current_pos;
                    }
                    t_type << t_type2;
                    t_len << t_len2;
                    t_id1 << t_id12;
                    t_id2 << t_id22;
                    t_control1 << t_control12;
                    t_control2 << t_control22;
                    t_column << t_column2;
    }
        if (quotes_found==false)//no '\"' found
        offset+=sub_left.length();
        else
        offset+=sub_left.length()+1;//+1 because of the '"' at the beginning
    //qDebug() << "offset=start_inner=" << offset;
    /// Inner part -- if quotations are found this is the part inside them -- we ignore this then for any replacements
    get_List_of_Subformulas(sub_inner,subformulas2,suboffsets2);/// splitting formulas with ';' between commands
    for (int lmn=0;lmn<subformulas2.length();lmn++)
    {
    equals_pos=subformulas2.at(lmn).indexOf("=");
    if (equals_pos<0)//no '=' in inner part
    {
    current_pos=0;
    process_single_string(subformulas2.at(lmn),t_startpos2,t_type2,t_len2,t_id12,t_id22,t_control12,t_control22,t_column2);
    //qDebug() << "Inner Part contains NO '='. Found tokens=" << t_startpos.length();
        if (quotes_found==false)
        {
            for (int i=0;i<t_startpos2.length();i++)
            {
            t_startpos << t_startpos2.at(i)+offset+suboffsets2.at(lmn);
            t_pos << current_pos;
            }
            t_type << t_type2;
            t_len << t_len2;
            t_id1 << t_id12;
            t_id2 << t_id22;
            t_control1 << t_control12;
            t_control2 << t_control22;
            t_column << t_column2;
        }
    }
    else//'=' in inner part --> split inner part
    {
    sub_left1=subformulas2.at(lmn).left(equals_pos);
    sub_right1=subformulas2.at(lmn).mid(equals_pos+1,-1);
    current_pos=0;

    if (fit_command==false)
    {
    process_single_string(sub_left1,t_startpos2,t_type2,t_len2,t_id12,t_id22,t_control12,t_control22,t_column2);
    //qDebug() << "Inner Part contains '='. Found tokens (left)=" << t_startpos.length();
    //qDebug() << "left1= #" << sub_left1 << "#";
        if(quotes_found==false)
        {
            for (int i=0;i<t_startpos2.length();i++)
            {
            t_startpos << t_startpos2.at(i)+offset+suboffsets2.at(lmn);
            t_pos << current_pos;
            }
            t_type << t_type2;
            t_len << t_len2;
            t_id1 << t_id12;
            t_id2 << t_id22;
            t_control1 << t_control12;
            t_control2 << t_control22;
            t_column << t_column2;
        }
    //qDebug() << "right1=#" << sub_right1 << "#";
    current_pos=1;
    process_single_string(sub_right1,t_startpos2,t_type2,t_len2,t_id12,t_id22,t_control12,t_control22,t_column2);
    //qDebug() << "Found tokens (right)=" << t_startpos.length();
        if (quotes_found==false)
        {
            for (int i=0;i<t_startpos2.length();i++)
            {
            t_startpos << t_startpos2.at(i)+offset+sub_left1.length()+1+suboffsets2.at(lmn);
            t_pos << current_pos;
            }
            t_type << t_type2;
            t_len << t_len2;
            t_id1 << t_id12;
            t_id2 << t_id22;
            t_control1 << t_control12;
            t_control2 << t_control22;
            t_column << t_column2;
        }
    }

    }
    }//end (lmn)-loop through subformulas in inner part
        if (quotes_found==false)//no '\"' found
        offset+=sub_inner.length();
        else
        offset+=sub_inner.length()+1;//+1 because of the '"' at the beginning
    //qDebug() << "offset=start_right=" << offset;
    /// Right part
    equals_pos=sub_right.indexOf("=");
    if (equals_pos<0)//no '=' in left part
    {
    current_pos=0;
    process_single_string(sub_right,t_startpos2,t_type2,t_len2,t_id12,t_id22,t_control12,t_control22,t_column2);
    //qDebug() << "Right Part contains NO '='. Found tokens=" << t_startpos.length();
        for (int i=0;i<t_startpos2.length();i++)
        {
        t_startpos << t_startpos2.at(i)+offset;
        t_pos << current_pos;
        }
        t_type << t_type2;
        t_len << t_len2;
        t_id1 << t_id12;
        t_id2 << t_id22;
        t_control1 << t_control12;
        t_control2 << t_control22;
        t_column << t_column2;
    }
    else//'=' in left part --> split left part
    {
    sub_left1=sub_right.left(equals_pos);
    sub_right1=sub_right.mid(equals_pos+1,-1);
    current_pos=0;
    process_single_string(sub_left1,t_startpos2,t_type2,t_len2,t_id12,t_id22,t_control12,t_control22,t_column2);
    //qDebug() << "Right Part contains '='. Found tokens (left)=" << t_startpos.length();
    //qDebug() << "left1= #" << sub_left1 << "#";
        for (int i=0;i<t_startpos2.length();i++)
        {
        t_startpos << t_startpos2.at(i)+offset;
        t_pos << current_pos;
        }
        t_type << t_type2;
        t_len << t_len2;
        t_id1 << t_id12;
        t_id2 << t_id22;
        t_control1 << t_control12;
        t_control2 << t_control22;
        t_column << t_column2;
    //qDebug() << "right1=#" << sub_right1 << "#";
    current_pos=1;
    process_single_string(sub_right1,t_startpos2,t_type2,t_len2,t_id12,t_id22,t_control12,t_control22,t_column2);
    //qDebug() << "Found tokens (right)=" << t_startpos.length();
        for (int i=0;i<t_startpos2.length();i++)
        {
        t_startpos << t_startpos2.at(i)+offset+sub_left1.length()+1;
        t_pos << current_pos;
        }
        t_type << t_type2;
        t_len << t_len2;
        t_id1 << t_id12;
        t_id2 << t_id22;
        t_control1 << t_control12;
        t_control2 << t_control22;
        t_column << t_column2;
    }
/*t_startpos=token_startpos;
t_type=token_type;
t_len=token_len;
t_id1=token_id1;
t_id2=token_id2;
t_pos=token_pos;
t_control1=token_control1;
t_control2=token_control2;
t_column=token_column;*/
}

void formula_to_process::process_single_string(QString text,QList<int> & t_startpos,QList<int> & t_type,QList<int> & t_len,QList<int> & t_id1,QList<int> & t_id2,QList<char> & t_control1,QList<char> & t_control2,QList<int> & t_column)
{
int f_ret,f_pos,g_no,s_no,st_len;
char control1,control2,col;
int st_pos=0;
t_startpos.clear();
t_type.clear();
t_len.clear();
t_id1.clear();
t_id2.clear();
t_control1.clear();
t_control2.clear();
t_column.clear();
//qDebug() << "Processing single string=" << text;
    while ( (f_ret=searchTextForFirstID(text,st_pos,f_pos,g_no,s_no,control1,control2,col,st_len))!=ID_NONE )//as long as we find something
    {
    t_startpos << f_pos;
    t_type << f_ret;
    t_len << st_len;
    t_id1 << g_no;
    t_id2 << s_no;
    t_control1 << control1;
    t_control2 << control2;
    t_column << (int)(col-'X');
//qDebug() << "COLUMN=" << t_column.last();
    st_pos=f_pos+st_len;
    //qDebug() << "found single id:" << (int)t_control1;
    }
}

bool formula_to_process::has_fit_formula_command(QString f)
{
QString text=f;
remove_beginning_whitespaces(text);
text=text.toUpper();
return text.contains(QString("FIT FORMULA"));
}

int formula_to_process::is_comment(QString f)//0=no comment, 1=comment, 2=special, 3=if, 4=empty
{
QString text=f;
remove_beginning_whitespaces(text);
//qDebug() << "is_comment: text=" << text << "length=" << text.length();
if (text.length()<1) return 4;//only whitespaces
text=text.toUpper();
int is_special=text.indexOf(QString("#QTGRACE_SPECIAL"));
int is_if=text.indexOf(QString("#IF["));
int starts_with_comment=(text.at(0).toLatin1()=='#'?1:0);
if (is_special!=0)
{
is_special=text.indexOf(QString("#QTGRACE_ADDITIONAL_PARAMETER"));
}
return is_special==0?2:(is_if==0?3:(starts_with_comment==1?1:0));
}

void formula_to_process::init_if_statement(QString compl_statement,int offset,QList<int> & t_startpos,QList<int> & t_type,QList<int> & t_len,QList<int> & t_id1,QList<int> & t_id2,QList<int> & t_pos,QList<char> & t_control1,QList<char> & t_control2,QList<int> & t_column)
{
    if (compl_statement.length()<=4) return;//too short to be an if-statement
char * temp_c1=new char[8+2*compl_statement.length()];
strcpy(temp_c1,compl_statement.toLocal8Bit().constData());

char * temp_c=temp_c1;
while (temp_c[0]!='\0' && isspace(temp_c[0])) temp_c++;//this finds the first character that is not a whitespace

if (!(temp_c[0]=='#' && toupper(temp_c[1])=='I' && toupper(temp_c[2])=='F' && temp_c[3]=='['))
{
delete[] temp_c1;
return;//is no if-statement --> just return
}

QList<int> comp_pos;
QStringList comp_text;
int comparator;
int ret=split_if_statement(temp_c,comp_pos,comp_text,comparator);//split the if-statement in several parts for further analysis

if (ret==RETURN_FAILURE)//"no if" or "error"
{
qDebug() << "ERROR IN IF-STATEMENT!";
}
else//RETURN_SUCCESS
{//now we check the separate parts of the if-statement for ids to replace
//first part of if-formula
//qDebug() << comp_pos.at(0) << "FirstIf=" << comp_text.at(0);
process_single_subcommand(comp_text.at(0),offset+comp_pos.at(0),t_startpos,t_type,t_len,t_id1,t_id2,t_pos,t_control1,t_control2,t_column);
//possible second part of if-formula (after comparator)
if (comparator>0)
{
//qDebug() << comp_pos.at(1) << "SecondIf=" << comp_text.at(1);
process_single_subcommand(comp_text.at(1),offset+comp_pos.at(1),t_startpos,t_type,t_len,t_id1,t_id2,t_pos,t_control1,t_control2,t_column);
}
//the actual command after the if-statement
//qDebug() << comp_pos.at(2) << "Command After If=" << comp_text.at(2);
process_single_subcommand(comp_text.at(2),offset+comp_pos.at(2),t_startpos,t_type,t_len,t_id1,t_id2,t_pos,t_control1,t_control2,t_column);
}

delete[] temp_c;
}

void formula_to_process::init_formula(QString f)//clear everything and enter new formula
{
clear_tokens();
original_formula=f;
convertStringToUpper(original_formula);//This should make everything look clearer
len_orig_formula=original_formula.length();
int offset=0;

QStringList subformulas;
QList<int> suboffsets;

QList<int> t_startpos;
QList<int> t_type;
QList<int> t_len;
QList<int> t_id1;
QList<int> t_id2;
QList<int> t_pos;
QList<char> t_control1;
QList<char> t_control2;
QList<int> t_column;

//qDebug() << endl << "Formula to init=" << original_formula << endl;

get_List_of_Subformulas(original_formula,subformulas,suboffsets);/// splitting formulas with ';' between commands

//qDebug() << "Number of Subformulas=" << subformulas.length() << endl;

for (int ijk=0;ijk<subformulas.length();ijk++)
{
t_startpos.clear();
t_type.clear();
t_len.clear();
t_id1.clear();
t_id2.clear();
t_pos.clear();
t_control1.clear();
t_control2.clear();
t_column.clear();
    offset=suboffsets.at(ijk);
    process_single_subcommand(subformulas.at(ijk),offset,t_startpos,t_type,t_len,t_id1,t_id2,t_pos,t_control1,t_control2,t_column);
token_startpos << t_startpos;
token_type << t_type;
token_len << t_len;
token_id1 << t_id1;
token_id2 << t_id2;
token_pos << t_pos;
token_control1 << t_control1;
token_control2 << t_control2;
token_column << t_column;
}//for-loop of different commands

/*QString token_type_text[19];
token_type_text[0]="ID_REGULAR_COMPLETE";
token_type_text[1]="ID_NGSS";
token_type_text[2]="ID_SGNS";
token_type_text[3]="ID_NGNS";
token_type_text[4]="ID_SET_ONLY";
token_type_text[5]="ID_NSET_ONLY";
token_type_text[6]="ID_GRAPH_ONLY";
token_type_text[7]="ID_NGRAPH_ONLY";
token_type_text[8]="ID_COLUMN_ONLY";
token_type_text[9]="ID_GRAPH_NR_ONLY";
token_type_text[10]="ID_SET_NR_ONLY";
token_type_text[11]="ID_SPECIAL_COMMAND";
token_type_text[12]="ID_NNGRAPH_ONLY";
token_type_text[13]="ID_NNSET_ONLY";
token_type_text[14]="ID_NNGSS";
token_type_text[15]="ID_NNGNS";
token_type_text[16]="ID_NNGNNS";
token_type_text[17]="ID_SGNNS";
token_type_text[18]="ID_NGNNS";

qDebug() << endl << "AFTER INIT FORMULA" << endl;
qDebug() << "Nr Of Tokens=" << token_startpos.length();
for (int i=0;i<token_startpos.length();i++)
{
qDebug() << i << "Type=" << (token_type.at(i)<0?"?":token_type_text[token_type.at(i)]) <<  "StartPos=" << token_startpos.at(i) << "Token=" << f.mid(token_startpos.at(i),token_len.at(i));
}*/

clear_replacements_and_counters();
}

void formula_to_process::add_one_token(int startpos,int type,int len,int id1,int id2,int pos,char control1,char control2,int col)
{
token_startpos << startpos;
token_type << type;
token_len << len;
token_id1 << id1;
token_id2 << id2;
token_pos << pos;
token_control1 << control1;
token_control2 << control2;
token_column << col;
}

void formula_to_process::return_qtspecial_subcommand(int nr,int r_type,int g_lhs,int s_lhs,int g_rhs,int s_rhs,QString & f)
{
char * parameters;
    if (nr<0 || nr>=token_startpos.length() || token_type.at(nr)!=ID_SPECIAL_COMMAND)
    {
    f.clear();
    return;
    }
QString unchanged=original_formula.mid(token_startpos.at(nr),token_len.at(nr));
//qDebug() << "replacing qtSpecialCommand=" << unchanged << " control1=" << (int)token_control1.at(nr);
QString tmp_str;
//char tmp_cmd_str[512];
char * command=new char[8+2*token_len.at(nr)];
strcpy(command,unchanged.toLatin1().constData());
(void)containsSpecialCommand(command,&parameters);

int o_n_sets,n_sets,type,realization;
int * o_gnos=NULL,* o_snos=NULL;
int * gnos=NULL,* snos=NULL;
double limits[2];
double x0,y0;
int orders[2];
char x_formula[512];
double ripple;
int absolute,debug,point_extension,oversampling,rno,invr;
double start,stop;
int rx,ideg,iresid,ret;//,len2;
int pos,g_no,s_no,leng;
char control1,control2,col;
class formula_to_process * formula2;

    switch ((int)token_control1.at(nr))//type of special command
    {
    case SPECIAL_FILTER:
        if (r_type==2)
        {
ParseFilterCommand(command+28,o_n_sets,&o_gnos,&o_snos,n_sets,&gnos,&snos,type,realization,limits,orders,x_formula,ripple,absolute,debug,point_extension,oversampling,rno,invr);
o_n_sets=1;
n_sets=1;
if (o_gnos!=NULL) delete[] o_gnos;
if (o_snos!=NULL) delete[] o_snos;
if (gnos!=NULL) delete[] gnos;
if (snos!=NULL) delete[] snos;
o_gnos=new int[2];
o_snos=new int[2];
gnos=new int[2];
snos=new int[2];
o_gnos[0]=g_rhs;
o_snos[0]=s_rhs;
gnos[0]=g_lhs;
snos[0]=s_lhs;
WriteFilterString(f,o_n_sets,o_gnos,o_snos,n_sets,gnos,snos,type,realization,limits,orders,x_formula,ripple,absolute,debug,point_extension,oversampling,rno,invr);
        }
        else
        {
        f=unchanged;
        }
        break;
    case SPECIAL_REGRESSION:
        if (r_type==2)
        {
ParseRegression(command+28,o_n_sets,&o_gnos,&o_snos,n_sets,&gnos,&snos,ideg,iresid,rno,invr,start,stop,point_extension,rx,x_formula);
o_n_sets=1;
n_sets=1;
if (o_gnos!=NULL) delete[] o_gnos;
if (o_snos!=NULL) delete[] o_snos;
if (gnos!=NULL) delete[] gnos;
if (snos!=NULL) delete[] snos;
o_gnos=new int[2];
o_snos=new int[2];
gnos=new int[2];
snos=new int[2];
o_gnos[0]=g_rhs;
o_snos[0]=s_rhs;
gnos[0]=g_lhs;
snos[0]=s_lhs;
WriteRegressionString(f,o_n_sets,o_gnos,o_snos,n_sets,gnos,snos,ideg,rno,invr,start,stop,point_extension,rx,x_formula);
        }
        else
        {
        f=unchanged;
        }
        break;
    case SPECIAL_FIT2D:
        if (r_type==2)
        {
qDebug() << "Fit2D";
o_n_sets=1;
n_sets=1;
if (o_gnos!=NULL) delete[] o_gnos;
if (o_snos!=NULL) delete[] o_snos;
if (gnos!=NULL) delete[] gnos;
if (snos!=NULL) delete[] snos;
o_gnos=new int[2];
o_snos=new int[2];
gnos=new int[2];
snos=new int[2];
ParseFit2D(command+23,absolute,o_gnos[0],o_snos[0],snos[0],x0,y0,limits[0],limits[1],ripple,start,stop,point_extension,orders[0],orders[1]);
qDebug() << "Nach Parse. command=" << command+23;
o_gnos[0]=g_rhs;
o_snos[0]=s_rhs;
gnos[0]=g_lhs;
snos[0]=s_lhs;
f=WriteFit2DString(absolute,o_gnos[0],o_snos[0],snos[0],x0,y0,limits[0],limits[1],ripple,start,stop,point_extension,orders[0],orders[1]);
qDebug() << "Nach Write. f=" << f;
        }
        else
        {
        f=unchanged;
        }
        break;
    case SPECIAL_APPEND:
        if (r_type==2 || r_type==1)//replaced or completed
        {
tmp_str=unchanged.mid(24);
ret=searchTextForFirstID(tmp_str,0,pos,g_no,s_no,control1,control2,col,leng);
if (ret==ID_NONE)
{
f=unchanged;
}
else
{
//qDebug() << "Found in Append: G" << g_no << control1 << ".S" << s_no << control2;
tmp_str=unchanged.mid(24+pos+leng+1);
pos=tmp_str.lastIndexOf(QString("}"));
tmp_str=tmp_str.left(pos);
formula2=new formula_to_process;
formula2->init_formula(tmp_str);
formula2->sync_allocated_new_and_counter(this);
if (r_type==1)//complete id; original is already set up completely
formula2->return_completed_formula(g_rhs,s_rhs,g_rhs,s_rhs,tmp_str);
else if (r_type==2)
formula2->return_replaced_formula(g_rhs,s_rhs,g_rhs,s_rhs,tmp_str);
//qDebug() << "Append_value(original)=" << tmp_str;
        f=QString("#QTGRACE_SPECIAL APPEND G");
            if (control1=='N')//new graph
            {
            f+=QString::number(get_new_graph(g_no));
            }
            else if (control1=='#')//graph counter
            {
            f+=QString::number(get_graph_counter(g_no));
            }
            else if (control1=='$')//graph id unchanged
            {
            f+=QString::number(g_no);
            }
            else//normal graph-id
            {
                if (r_type==1)//complete = original here
                {
                f+=QString::number(g_no);
                }
                else
                {
                f+=QString::number(g_lhs);
                }
            }
        f+=QString(".S");
            if (control2=='N')//new set
            {
            f+=QString::number(get_new_set(g_no,control1,s_no));
            }
            else if (control2=='#')//set counter
            {
            f+=QString::number(get_set_counter(g_no,control1,s_no));
            }
            else if (control2=='$')//set id unchanged
            {
            f+=QString::number(s_no);
            }
            else//normal set-id
            {
                if (r_type==1)//complete = original here
                {
                f+=QString::number(s_no);
                }
                else
                {
                f+=QString::number(s_lhs);
                }
            }
        f+=QString("{");
//qDebug() << "Append_value(intended_replacement)=" << tmp_str << " f=" << f;
        f+=tmp_str;
        f+=QString("}");
//qDebug() << "f=" << f;
        this->sync_allocated_new_and_counter(formula2);
        delete formula2;
        }
        }
        else//original
        {
        f=unchanged;
        }
        break;
    case SPECIAL_ADD:
    case SPECIAL_MINUS:
    case SPECIAL_DIVIDE:
    case SPECIAL_MULTIPLY:
        f=unchanged;
        /*if (strcmp(parameters,"LAST_VALUE")==0)
        {
            //sprintf(temp_val,"%g",saved_value);
        tmp_str=QString::number(saved_value);
        }
        else if (strcmp(parameters,"LAST_FORMULA")==0)
        {
            formula2=new formula_to_process;
            tmp_str=QString(last_formula);//original
            qDebug() << "last_formula=" << last_formula;
            formula2->init_formula(tmp_str);
            formula2->sync_allocated_new_and_counter(this);
            if (r_type==1)//complete
            {
            formula2->return_completed_formula(g_lhs,s_lhs,g_rhs,s_rhs,tmp_str);
            }
            else if (r_type==2)//replaced
            {
            formula2->return_replaced_formula(g_lhs,s_lhs,g_rhs,s_rhs,tmp_str);
            }
            strcpy(tmp_cmd_str,tmp_str.toLatin1().constData());
            len2=strlen(tmp_cmd_str);
            pos=0;
            for (int i=0;i<len2;i++){if (tmp_cmd_str[i]=='='){pos=i+1; break;}}
        //f+=QString(tmp_cmd_str+pos);
        tmp_str=QString(tmp_cmd_str).mid(pos);
            this->sync_allocated_new_and_counter(formula2);
            delete formula2;
        }
        else//SAVED_FORMULA
        {
            formula2=new formula_to_process;
            tmp_str=QString(saved_formula);//original
            formula2->init_formula(tmp_str);
            formula2->sync_allocated_new_and_counter(this);
            if (r_type==1)//complete
            {
            formula2->return_completed_formula(g_lhs,s_lhs,g_rhs,s_rhs,tmp_str);
            }
            else if (r_type==2)//replaced
            {
            formula2->return_replaced_formula(g_lhs,s_lhs,g_rhs,s_rhs,tmp_str);
            }
            strcpy(tmp_cmd_str,tmp_str.toLatin1().constData());
            len2=strlen(tmp_cmd_str);
            pos=0;
            for (int i=0;i<len2;i++){if (tmp_cmd_str[i]=='='){pos=i+1; break;}}
        //f+=QString(tmp_cmd_str+pos);
        tmp_str=QString(tmp_cmd_str).mid(pos);
            this->sync_allocated_new_and_counter(formula2);
            delete formula2;
        }

        if (r_type==0)
        {
        f=QString("Y=Y");
        }
        else
        {
        f=QString("G")+QString::number(g_lhs)+QString(".S")+QString::number(s_lhs)+QString(".Y=")+QString("G")+QString::number(g_lhs)+QString(".S")+QString::number(s_lhs)+QString(".Y");
        }

        if (token_control1.at(nr)==SPECIAL_ADD)
        {
        f+=QString("+(");
        }
        else if (token_control1.at(nr)==SPECIAL_MINUS)
        {
        f+=QString("-(");
        }
        else if (token_control1.at(nr)==SPECIAL_MULTIPLY)
        {
        f+=QString("*(");
        }
        else if (token_control1.at(nr)==SPECIAL_DIVIDE)
        {
        f+=QString("/(");
        }

        f+=tmp_str;

        f+=QString(")");*/
        break;
    case SPECIAL_FORMULA://no replacing in these cases
    case SPECIAL_REMEMBER:
    case SPECIAL_USE:
    case SPECIAL_NONE:
    case SPECIAL_EXTRACT:
    f=unchanged;
        break;
    }
//qDebug() << "Replaced Special Formula=" << f;
if (command!=NULL) delete[] command;
if (o_gnos!=NULL) delete[] o_gnos;
if (o_snos!=NULL) delete[] o_snos;
if (gnos!=NULL) delete[] gnos;
if (snos!=NULL) delete[] snos;
}

void formula_to_process::return_completed_formula(int g_lhs,int s_lhs,int g_rhs,int s_rhs,QString & f)//returns a formula with completed ids (complete ids (like G0.S1.Y) stay untouched, incomplete ids (like just Y) get completed according to their position on the left hand side (lhs) or right hand side (rhs) of a '=')
{
static QString intermediate;
if (token_startpos.length()<1)
{
f=original_formula;
return;
}
f.clear();
for (int i=-1;i<token_startpos.length();i++)
{
    if (i>=0)
    {
    return_single_token(i,1,g_lhs,s_lhs,g_rhs,s_rhs,intermediate);
    f+=intermediate;
    }
get_string_after_token(i,intermediate);
f+=intermediate;
}
}

void formula_to_process::return_replaced_formula(int g_lhs,int s_lhs,int g_rhs,int s_rhs,QString & f)//completes incomplete ids and replaces existing ids
{
static QString intermediate;
if (token_startpos.length()<1)
{
f=original_formula;
return;
}
f.clear();
for (int i=-1;i<token_startpos.length();i++)
{
    if (i>=0)
    {
    //return_single_token(i,0,g_lhs,s_lhs,g_rhs,s_rhs,intermediate);
    //qDebug() << "Original=" << intermediate;
    return_single_token(i,2,g_lhs,s_lhs,g_rhs,s_rhs,intermediate);
    //qDebug() << "Replaced=" << intermediate;
    f+=intermediate;
    }
get_string_after_token(i,intermediate);
f+=intermediate;
//qDebug() << "return_replaced_formula: f=" << f;
}
}

void formula_to_process::check_destination_set_existence(int g_lhs,int s_lhs,int g_rhs,int s_rhs,int replace)
{
int cur_pos=-1;//location (left / right of '=')
int dest_g,dest_s,d_len,s_g,s_s,s_len;
QString ret_token;
char tokenID[128];
dest_g=dest_s=s_g=s_s=d_len=s_len=-1;
    for (int i=0;i<token_pos.length();i++)
    {
    return_single_token(i,replace,g_lhs,s_lhs,g_rhs,s_rhs,ret_token);
    strcpy(tokenID,ret_token.toUpper().toLatin1());
    //qDebug() << "tokenID=" << tokenID;
        if (cur_pos!=token_pos.at(i))
        {
        cur_pos=token_pos.at(i);
            if (cur_pos==0)
            {
            sscanf(tokenID,"G%d.S%d",&dest_g,&dest_s);
            }
            else
            {
            sscanf(tokenID,"G%d.S%d",&s_g,&s_s);
            }
        }
        if (dest_g>=0 && s_g>=0)
        {
        s_len=getsetlength(s_g,s_s);
        d_len=getsetlength(dest_g,dest_s);
        //qDebug() << "Destination=G" << dest_g << ".S" << dest_s << " Source=G" << s_g << ".S" << s_s << " slen=" << s_len << " dlen=" << d_len;
            if (s_len!=d_len && s_len>=0)
            {
            //(void)allocate_set(dest_g,dest_s);
            (void)copyset(s_g,s_s,dest_g,dest_s);
            }
        dest_g=dest_s=s_g=s_s=s_len=-1;
        }
    }
}

QString formula_to_process::execute_formula(int g_lhs,int s_lhs,int g_rhs,int s_rhs,int replace,int & result)
{
static QString intermediate;
int cur_p_gno,cur_p_sno,n_errors;
cur_p_gno=get_parser_gno();
cur_p_sno=get_parser_setno();
set_parser_setno(g_lhs,s_lhs);
//qDebug() << "Current Set=" << g_lhs << " , " << s_lhs;
    if (replace==1)
    {
    return_replaced_formula(g_lhs,s_lhs,g_rhs,s_rhs,intermediate);
//qDebug() << "A EXECUTE intermediate=" << intermediate << " [" << original_formula << "]";
    check_destination_set_existence(g_lhs,s_lhs,g_rhs,s_rhs,2);
    }
    else
    {
    return_completed_formula(g_lhs,s_lhs,g_rhs,s_rhs,intermediate);
    check_destination_set_existence(g_lhs,s_lhs,g_rhs,s_rhs,1);
    }
QStringList list2;
QList<int> offsets;
QString tmp_str;
//qDebug() << "B EXECUTE intermediate=" << intermediate << endl;
get_List_of_Subformulas(intermediate,list2,offsets);
char * formu=new char[8+intermediate.toLatin1().length()];
result=0;
n_errors=0;
bool old_active_replaceing=activate_id_replacing;
activate_id_replacing=false;
//qDebug() << "Current Set=G" << get_parser_gno() << ".S" << get_parser_setno();
//qDebug() << "Execute Formula: Replace=" << replace;
for (int i=0;i<list2.length();i++)
{
//qDebug() << "EXECUTE subformula=" << list2.at(i);
//qDebug() << i << "=" << list2.at(i);
tmp_str=list2.at(i);
remove_end_comment(tmp_str);
strcpy(formu,tmp_str.toLatin1().constData());
//qDebug() << "EXECUTE formu=" << formu;
set_parser_setno(g_lhs,s_lhs);//for special replay-commands like LAST_FORMULA
    if (formu[0]=='#')
    {
    //parse_qtGrace_Additions(formu);
    qtspecial_scanner(formu,&n_errors);
    result+=n_errors;
    }
    else
    {
    result+=scanner(formu);
    }
}
activate_id_replacing=old_active_replaceing;
increase_counters();
delete[] formu;
set_parser_setno(cur_p_gno,cur_p_sno);
return intermediate;
}

void formula_to_process::clear_replacements_and_counters(void)
{
    token_graph_new.clear();
    token_set_new.clear();
    token_graph_counter.clear();
    token_set_counter.clear();
    token_graph_counter_used.clear();
    token_set_counter_used.clear();
    last_set_id_in_graph.clear();
    for (int i=0;i<token_startpos.length();i++)
    {
    token_graph_new << -1;
    token_set_new << -1;
    token_graph_counter << token_id1.at(i);//initialize with first id
    token_set_counter << token_id2.at(i);//initialize with first id
    token_graph_counter_used << false;//only used ids will be increased later (usually all should get used)
    token_set_counter_used << false;
    }
    gather_new_graphs_and_sets();
}

void formula_to_process::gather_new_graphs_and_sets(void)
{
    int g_id_to_find=-1;
    int s_id_to_find=-1;
    int index=-1;
    n_graph_id.clear();
    nn_graph_id.clear();
    n_set_id.clear();
    nn_set_id.clear();
    if (n_graph_id_pos!=NULL) delete[] n_graph_id_pos;
    if (nn_graph_id_pos!=NULL) delete[] nn_graph_id_pos;
    if (n_set_id_pos!=NULL) delete[] n_set_id_pos;
    if (nn_set_id_pos!=NULL) delete[] nn_set_id_pos;
    //first: we go through the tokens and look for new graphs-ids and new set ids outside new graphs
    for (int i=0;i<token_startpos.length();i++)
    {
    g_id_to_find=token_id1.at(i);//graph-id
    s_id_to_find=token_id2.at(i);//set-id
        switch (token_type.at(i))
        {
        //the things that are not interesting here
        default:
        case ID_NONE:
        case ID_REGULAR_COMPLETE:
        case ID_SET_ONLY:
        case ID_GRAPH_ONLY:
        case ID_COLUMN_ONLY:
        case ID_GRAPH_NR_ONLY:
        case ID_SET_NR_ONLY:
        case ID_SPECIAL_COMMAND:
        ;
        break;
        //the things with new graphs only (no new sets)
        case ID_NGSS://G1N.S4
        case ID_NGRAPH_ONLY://G1N
        //new graph and new set
        case ID_NGNS://G1N.S3N
        //new graph with NN-new set
        case ID_NGNNS://G2N.S2NN
            if (!n_graph_id.contains(g_id_to_find))//if list does not contain the current new graph-id add it to the list
            {
            n_graph_id << g_id_to_find;
            }
        break;
        //the things with new sets only (no new graphs)
        case ID_SGNS://G2.S1N
        case ID_NSET_ONLY://S3N
            if (!n_set_id.contains(s_id_to_find))//if list does not contain the current new set-id add it to the list
            {
            n_set_id << s_id_to_find;
            }
        break;
        //the things with NN-new graphs only (no new sets)
        case ID_NNGRAPH_ONLY://G0NN
        case ID_NNGSS://G0NN.S2
        //NN-new graphs with new sets
        case ID_NNGNS://G0NN.S3N
        case ID_NNGNNS://G0NN.S4NN
            if (!nn_graph_id.contains(g_id_to_find))//if list does not contain the current new graph-id add it to the list
            {
            nn_graph_id << g_id_to_find;
            }
        break;
        //the things with NN-new sets only (no new graphs)
        case ID_NNSET_ONLY://S1NN
        case ID_SGNNS://G2.S1NN
            if (!nn_set_id.contains(s_id_to_find))//if list does not contain the current new graph-id add it to the list
            {
            nn_set_id << s_id_to_find;
            }
        break;
        }
    }
    //allocate enough memory to tore the locations
    if (n_graph_id.length()>0)
    {
    n_graph_id_pos=new QList<int>[n_graph_id.length()];
    }
    if (nn_graph_id.length()>0)
    {
    nn_graph_id_pos=new QList<int>[nn_graph_id.length()];
    }
    if (n_set_id.length()>0)
    {
    n_set_id_pos=new QList<int>[n_set_id.length()];
    }
    if (nn_set_id.length()>0)
    {
    nn_set_id_pos=new QList<int>[nn_set_id.length()];
    }
    //second: search all tokens again for the locations of the new ids
    for (int i=0;i<token_startpos.length();i++)
    {
    g_id_to_find=token_id1.at(i);//graph-id
    s_id_to_find=token_id2.at(i);//set-id
        switch (token_type.at(i))
        {
        //the things that are not interesting here
        default:
        case ID_NONE:
        case ID_REGULAR_COMPLETE:
        case ID_SET_ONLY:
        case ID_GRAPH_ONLY:
        case ID_COLUMN_ONLY:
        case ID_GRAPH_NR_ONLY:
        case ID_SET_NR_ONLY:
        case ID_SPECIAL_COMMAND:
        ;
        break;
        //the things with new graphs only (no new sets)
        case ID_NGSS://G1N.S4
        case ID_NGRAPH_ONLY://G1N
        //new graph and new set
        case ID_NGNS://G1N.S3N
        //new graph with NN-new set
        case ID_NGNNS://G2N.S2NN
        index=n_graph_id.indexOf(g_id_to_find);
            if (index==-1)
            {
            qDebug() << "New Graph ID not found in List: " << g_id_to_find;
            }
            else
            {
            n_graph_id_pos[index] << i;
            }
        break;
        //the things with new sets only (no new graphs)
        case ID_SGNS://G2.S1N
        case ID_NSET_ONLY://S3N
        index=n_set_id.indexOf(s_id_to_find);
            if (index==-1)
            {
            qDebug() << "New Set ID not found in List: " << s_id_to_find;
            }
            else
            {
            n_set_id_pos[index] << i;
            }
        break;
        //the things with NN-new graphs only (no new sets)
        case ID_NNGRAPH_ONLY://G0NN
        case ID_NNGSS://G0NN.S2
        //NN-new graphs with new sets
        case ID_NNGNS://G0NN.S3N
        case ID_NNGNNS://G0NN.S4NN
        index=nn_graph_id.indexOf(g_id_to_find);
            if (index==-1)
            {
            qDebug() << "New NN Graph ID not found in List: " << g_id_to_find;
            }
            else
            {
            nn_graph_id[index] << i;
            }
        break;
        //the things with NN-new sets only (no new graphs)
        case ID_NNSET_ONLY://S1NN
        case ID_SGNNS://G2.S1NN
        index=nn_set_id.indexOf(s_id_to_find);
            if (index==-1)
            {
            qDebug() << "New NN Set ID not found in List: " << s_id_to_find;
            }
            else
            {
            nn_set_id_pos[index] << i;
            }
        break;
        }
    }


/*#define ID_NONE -1
//G0.S1
#define ID_REGULAR_COMPLETE 0
//G1N.S4
#define ID_NGSS 1
//G2.S1N
#define ID_SGNS 2
//G1N.S3N
#define ID_NGNS 3
//S0
#define ID_SET_ONLY 4
//S3N
#define ID_NSET_ONLY 5
//G0
#define ID_GRAPH_ONLY 6
//G1N
#define ID_NGRAPH_ONLY 7
//Y
#define ID_COLUMN_ONLY 8
//G#
#define ID_GRAPH_NR_ONLY 9
//S#
#define ID_SET_NR_ONLY 10
//#QTGRACE_SPECIAL
#define ID_SPECIAL_COMMAND 11
//G0NN
#define ID_NNGRAPH_ONLY 12
//S1NN
#define ID_NNSET_ONLY 13
//G0NN.S2
#define ID_NNGSS 14
//G0NN.S3N
#define ID_NNGNS 15
//G0NN.S4NN
#define ID_NNGNNS 16
//G2.S1NN
#define ID_SGNNS 17
//G2N.S2NN
#define ID_NGNNS 18*/

}

void formula_to_process::clear_NN_g_and_s(void)
{

}

void formula_to_process::increase_counters(void)
{
int id1, id2, value;
char control1, control2;
//first: search graph-counters
    for (int i=0;i<token_startpos.length();i++)
    {
        if (token_graph_counter_used.at(i)==true)//found a used counter
        {
        value=token_graph_counter.at(i)+1;//the increased value
        id1=token_id1.at(i);
        control1=token_control1.at(i);
            for (int j=i;j<token_startpos.length();j++)
            {
                if (token_id1.at(j)==id1 && token_control1.at(j)==control1)
                {
                token_graph_counter.replace(j,value);
                token_graph_counter_used.replace(j,false);
                }
            }
        }
    }
//second: search set-counters
    for (int i=0;i<token_startpos.length();i++)
    {
        if (token_set_counter_used.at(i)==true)//found a used counter
        {
        value=token_set_counter.at(i)+1;//the increased value
        id1=token_id1.at(i);
        control1=token_control1.at(i);
        id2=token_id2.at(i);
        control2=token_control2.at(i);
            for (int j=i;j<token_startpos.length();j++)
            {
                if (token_id1.at(j)==id1 && token_control1.at(j)==control1 && token_id2.at(j)==id2 && token_control2.at(j)==control2)
                {
                token_set_counter.replace(j,value);
                token_set_counter_used.replace(j,false);
                }
            }
        }
    }
}

int formula_to_process::get_graph_counter(int id1)
{
QList<int> index;
int value=0;
index.clear();
for (int i=0;i<token_startpos.length();i++)
{
    if (token_id1.at(i)==id1 && token_control1.at(i)=='#')
    {
    index << i;
    }
}
if (index.length()<1)
{
    qDebug() << "G" << id1 << "# not found!";
    return -1;
}
else
{
    value=token_graph_counter.at(index.at(0));
    token_graph_counter_used.replace(index.at(0),true);
    //for (int i=0;i<index.length();i++) token_graph_counter.replace(index.at(i),value+1);//increase the counter
    return value;
}
return -1;
}

int formula_to_process::get_set_counter(int id1, char control1, int id2)
{
QList<int> index;
int value=0;
index.clear();
for (int i=0;i<token_startpos.length();i++)
{
    if (token_id1.at(i)==id1 && token_control1.at(i)==control1 && token_id2.at(i)==id2 && token_control2.at(i)=='#')
    {
    index << i;
    }
}
if (index.length()<1)
{
    qDebug() << "S" << id2 << "# not found!";
    return -1;
}
else
{
    value=token_set_counter.at(index.at(0));
    token_set_counter_used.replace(index.at(0),true);//get value and say that the counter has been used
    //for (int i=0;i<index.length();i++) token_set_counter.replace(index.at(i),value+1);//increase the counter
    return value;
}
}

int formula_to_process::get_new_graph(int id1)
{
QList<int> index;
index.clear();
for (int i=0;i<token_startpos.length();i++)
{
    if (token_id1.at(i)==id1 && token_control1.at(i)=='N')
    {
    index << i;
    }
}
if (index.length()<1)
{
    qDebug() << "G" << id1 << "N not found!";
    return -1;
}
else
{
    if (token_graph_new.at(index.at(0))==-1)//no new graph generated yet
    {
    /// create new graph, set id for all in index, return new graph-id
    token_graph_new.replace(index.at(0),create_new_graph());
        if (token_graph_new.at(index.at(0))!=-1)
        {
            for (int i=1;i<index.length();i++)
            {
            token_graph_new.replace(index.at(i),token_graph_new.at(index.at(0)));
            }
        return token_graph_new.at(index.at(0));
        }
    }
    else//new graph already created
    {
    return token_graph_new.at(index.at(0));
    }
}
return -1;
}

int formula_to_process::get_new_set(int id1, char control1, int id2)
{
QList<int> index;
int n_set,c_graph;
index.clear();
for (int i=0;i<token_startpos.length();i++)
{
    if (token_id1.at(i)==id1 && token_control1.at(i)==control1 && token_id2.at(i)==id2 && token_control2.at(i)=='N')
    {
    index << i;
    }
}
if (index.length()<1)
{
    qDebug() << "S" << id2 << "N not found!";
    return -1;
}
else
{
    if (token_set_new.at(index.at(0))==-1)//no new set generated yet
    {
    c_graph=get_current_graph_id(index.at(0));
    /// create new graph, set id for all in index, return new graph-id
        while (last_set_id_in_graph.length()<=c_graph) last_set_id_in_graph << 0;
    n_set=nextset_after(c_graph,last_set_id_in_graph.at(c_graph));
    last_set_id_in_graph.replace(c_graph,n_set);
//qDebug() << "GetNewSet: G" << id1 << (int)control1 << ".S" << id2 << "N --> G" << get_current_graph_id(index.at(0)) << ".S" << n_set;
//qDebug() << "c_graph=" << c_graph << "last_set_id_in_graph.length()=" << last_set_id_in_graph.length() << "last_index=" << last_set_id_in_graph.at(c_graph);
    token_set_new.replace(index.at(0),n_set);
        if (token_set_new.at(index.at(0))!=-1)
        {
            for (int i=1;i<index.length();i++)
            {
            token_set_new.replace(index.at(i),token_set_new.at(index.at(0)));
            }
        return token_set_new.at(index.at(0));
        }
    }
    else//new graph already created
    {
    return token_set_new.at(index.at(0));
    }
}
return -1;
}

int formula_to_process::get_current_graph_id(int nr)
{
int ret=-1;
    switch (token_type.at(nr))
    {
    case ID_REGULAR_COMPLETE:
    case ID_SGNS:
    case ID_GRAPH_ONLY:
        ret=token_id1.at(nr);
    break;
    case ID_NGRAPH_ONLY:
    case ID_NGSS:
    case ID_NGNS:
        if (token_control1.at(nr)=='N')
        ret=token_graph_new.at(nr);
        else if (token_control1.at(nr)=='$')
        ret=token_id1.at(nr);
        else
        ret=token_graph_counter.at(nr);
    break;
    case ID_SET_ONLY:
    case ID_NSET_ONLY:
    case ID_COLUMN_ONLY:
    case ID_GRAPH_NR_ONLY:
        ret=current_graph;
    break;
    case ID_SET_NR_ONLY:
        ret=-1;
    break;
    }
return ret;
}

int formula_to_process::get_current_set_id(int nr)
{
int ret=-1;
    switch (token_type.at(nr))
    {
    case ID_REGULAR_COMPLETE:
    case ID_NGSS:
    case ID_SET_ONLY:
        ret=token_id2.at(nr);
    break;
    case ID_SGNS:
    case ID_NGNS:
    case ID_NSET_ONLY:
        if (token_control2.at(nr)=='N')
        ret=token_set_new.at(nr);
        else if (token_control2.at(nr)=='$')
        ret=token_id2.at(nr);
        else
        ret=token_set_counter.at(nr);
    break;
    case ID_GRAPH_ONLY:
    case ID_GRAPH_NR_ONLY:
    case ID_NGRAPH_ONLY:
        ret=-1;
    break;
    case ID_COLUMN_ONLY:
    case ID_SET_NR_ONLY:
        ret=current_set;
    break;
    }
return ret;
}

void formula_to_process::clear_tokens(void)
{
token_startpos.clear();
token_type.clear();
token_len.clear();
token_id1.clear();
token_id2.clear();
token_pos.clear();
token_control1.clear();
token_control2.clear();
token_column.clear();
token_graph_new.clear();
token_set_new.clear();
token_graph_counter.clear();
token_set_counter.clear();
token_graph_counter_used.clear();
token_set_counter_used.clear();

n_graph_id.clear();
nn_graph_id.clear();
n_set_id.clear();
nn_set_id.clear();
if (n_graph_id_pos!=NULL) delete[] n_graph_id_pos;
if (nn_graph_id_pos!=NULL) delete[] nn_graph_id_pos;
if (n_set_id_pos!=NULL) delete[] n_set_id_pos;
if (nn_set_id_pos!=NULL) delete[] nn_set_id_pos;

}

void formula_to_process::return_single_token(int nr,int type,int g_lhs,int s_lhs,int g_rhs,int s_rhs,QString & new_token)
{
//type=0 --> original | type=1 --> completed if incomplete | type=2 --> replaced
static const char * cols[5]={"X","Y","Y1","Y2","Y3"};
new_token.clear();
if (nr<0||nr>=token_startpos.length())
{
qDebug() << "Not a valid token nr=" << nr << " token-count=" << token_startpos.length();
return;
}
int gno=0,sno=0;
if (token_pos.at(nr)==0)//lhs
{
gno=g_lhs;
sno=s_lhs;
}
else//rhs
{
gno=g_rhs;
sno=s_rhs;
}
current_graph=gno;
current_set=sno;
//qDebug() << "Nr=" << nr << " Type=" << token_type.at(nr);
switch (token_type.at(nr))
{
case ID_NONE:
qDebug() << "No Id here!?";
break;
case ID_REGULAR_COMPLETE:
    if (type==0 || type==1)//original and completed are the same, because original is complete
    new_token=QString("G")+QString::number(token_id1.at(nr))+QString(".S")+QString::number(token_id2.at(nr));
    else
    new_token=QString("G")+QString::number(gno)+QString(".S")+QString::number(sno);
    if (token_column.at(nr)>=0 && token_column.at(nr)<5)
    new_token+=QString(".")+QString(cols[token_column.at(nr)]);
break;
case ID_NGSS:
    //check whether the new graph already contains the Set-Id-mentioned
    /*if (!is_valid_setno(get_new_graph(token_id1.at(nr)),token_id2.at(nr)))
    {
    //qDebug() << "SET DOES NOT(!) EXIST: G" << get_new_graph(token_id1.at(nr)) << ".S" << token_id2.at(nr);
    (void)allocate_set(get_new_graph(token_id1.at(nr)),token_id2.at(nr));
    }
    else
    {
    qDebug() << "SET DOES EXIST: G" << get_new_graph(token_id1.at(nr)) << ".S" << token_id2.at(nr);
    }*/
case ID_SGNS:
case ID_NGNS:
    new_token=QString("G");
    if (token_type.at(nr)==ID_SGNS)
    {//standard-graph-id
        if (type==0 || type==1)//original or completed
        {
        new_token+=QString::number(token_id1.at(nr));
        }
        else//replaced
        {
        new_token+=QString::number(gno);
        }
    }
    else
    {//new-graph-id / special-id with control character
        if (type==0)//original
        {
        new_token+=QString::number(token_id1.at(nr))+QString(token_control1.at(nr));
        }
        else//completed or replaced (special ids are not to be replaced)
        {
            if (token_control1.at(nr)=='N')
            {
            new_token+=QString::number(get_new_graph(token_id1.at(nr)));
            }
            else if (token_control1.at(nr)=='#')
            {
            new_token+=QString::number(get_graph_counter(token_id1.at(nr)));
            }
            else if (token_control1.at(nr)=='$')
            {
            new_token+=QString::number(token_id1.at(nr));//unchanged
            }
        }
    }
    new_token+=QString(".S");
    if (token_type.at(nr)==ID_NGSS)
    {//standard-set-id
        if (type==0 || type==1)//original or completed
        {
        new_token+=QString::number(token_id2.at(nr));
        }
        else//replaced
        {
        new_token+=QString::number(sno);
        }
    }
    else
    {//new-set-id / special-id with control character
        if (type==0)//original
        {
        new_token+=QString::number(token_id1.at(nr))+QString(token_control1.at(nr));
        }
        else//completed or replaced are the same here
        {
            if (token_control2.at(nr)=='N')
            {
            new_token+=QString::number(get_new_set(token_id1.at(nr),token_control1.at(nr),token_id2.at(nr)));
            }
            else if (token_control2.at(nr)=='#')
            {
            new_token+=QString::number(get_set_counter(token_id1.at(nr),token_control1.at(nr),token_id2.at(nr)));
            }
            else if (token_control2.at(nr)=='$')
            {
            new_token+=QString::number(token_id2.at(nr));//unchanged
            }
        }
    }
    if (token_column.at(nr)>=0 && token_column.at(nr)<5)
    new_token+=QString(".")+QString(cols[token_column.at(nr)]);
break;
case ID_SET_ONLY:
    if (type==0)//original
    new_token=QString("S")+QString::number(token_id2.at(nr));
    else if (type==1)//completed
    new_token=QString("G")+QString::number(gno)+QString(".S")+QString::number(token_id2.at(nr));
    else//replaced
    new_token=QString("G")+QString::number(gno)+QString(".S")+QString::number(sno);
    if (token_column.at(nr)>=0 && token_column.at(nr)<5)
    new_token+=QString(".")+QString(cols[token_column.at(nr)]);
break;
case ID_NSET_ONLY:
    if (type==0)//original
    {
    new_token=QString("S")+QString::number(token_id2.at(nr))+QString(token_control2.at(nr));
    }
    else//completed or replaced (i.e. resolve control character)
    {
    new_token=QString("G")+QString::number(gno)+QString(".S");//the original id did not contain a graph-number
        if (token_control2.at(nr)=='$')//unchanged id
        {
        new_token+=QString::number(token_id2.at(nr));//return the id unchanged
        }
        else if (token_control2.at(nr)=='#')//id-counter
        {
        new_token+=QString::number(get_set_counter(token_id1.at(nr),token_control1.at(nr),token_id2.at(nr)));
        }
        else if (token_control2.at(nr)=='N')//new id
        {
        new_token+=QString::number(get_new_set(token_id1.at(nr),token_control1.at(nr),token_id2.at(nr)));
        }
    }
    if (token_column.at(nr)>=0 && token_column.at(nr)<5)//if needed add a X or Y or something
    new_token+=QString(".")+QString(cols[token_column.at(nr)]);
break;
case ID_GRAPH_ONLY:
    if (type==0 || type==1)//original or completed
    new_token=QString("G")+QString::number(token_id1.at(nr));
    else//replaced
    new_token=QString("G")+QString::number(gno);
break;
case ID_NGRAPH_ONLY:
    if (type==0)//original
    {
    new_token=QString("G")+QString::number(token_id1.at(nr))+QString(token_control1.at(nr));
    }
    else//completed or replaced (i.e. resolve control character)
    {
        if (token_control1.at(nr)=='$')//unchanged id
        {
        new_token=QString("G")+QString::number(token_id1.at(nr));//return the id unchanged
        }
        else if (token_control1.at(nr)=='#')//id-counter
        {
        new_token=QString("G")+QString::number(get_graph_counter(token_id1.at(nr)));
        }
        else if (token_control1.at(nr)=='N')//new id
        {
        new_token=QString("G")+QString::number(get_new_graph(token_id1.at(nr)));
        }
    }
break;
case ID_COLUMN_ONLY:
    if (type==0)//original
    new_token=QString(cols[token_column.at(nr)]);
    else//completed or replaced (is the same, because no ids have been given at all)
    new_token=QString("G")+QString::number(gno)+QString(".S")+QString::number(sno)+QString(".")+QString(cols[token_column.at(nr)]);
break;
case ID_GRAPH_NR_ONLY:
    if (type==0)
    new_token=QString("G#");
    else//completed or replaced --> just return current graph-nr
    new_token=QString::number(gno);
break;
case ID_SET_NR_ONLY:
    if (type==0)
    new_token=QString("S#");
    else//completed or replaced --> just return current set-nr
    new_token=QString::number(sno);
//qDebug() << "At=" << token_pos.at(nr) << "Replace Set-ID: gno=" << gno << "sno=" << sno << "g_lhs=" << g_lhs << "s_lhs=" << s_lhs << "g_rhs=" << g_rhs << "s_rhs=" << s_rhs;
break;
case ID_SPECIAL_COMMAND:
    return_qtspecial_subcommand(nr,type,g_lhs,s_lhs,g_rhs,s_rhs,new_token);
break;
}
}

void formula_to_process::get_List_of_Subformulas(QString text,QStringList & subtext,QList<int> & offsets)
{
subtext.clear();
offsets.clear();
int offset=0;
int citation_counter=0;
int brakets_counter[3]={0,0,0};
char c;
for (int i=0;i<text.length();i++)
{
c=text.at(i).toLatin1();
//qDebug() << c;
    if (c=='\"')
    {
    citation_counter=!citation_counter;
    }
    if (c=='{') brakets_counter[0]++;
    else if (c=='}') brakets_counter[0]--;
    else if (c=='[') brakets_counter[1]++;
    else if (c==']') brakets_counter[1]--;
    else if (c=='(') brakets_counter[2]++;
    else if (c==')') brakets_counter[2]--;

    if ((c==';' && citation_counter==0 && brakets_counter[0]==0 && brakets_counter[1]==0 && brakets_counter[2]==0) || i==text.length()-1)//end of a sub-formula
    {
        if (i==text.length()-1)
        subtext << text.mid(offset,i-offset+1);
        else
        subtext << text.mid(offset,i-offset);
    offsets << offset;
    offset=i+1;
    //qDebug() << "SAVED, OFFSET=" << offsets.last();
    }
}
}

void formula_to_process::get_string_after_token(int nr,QString & text)
{
    if (token_startpos.length()<1)//no tokens found --> return everything
    {
    text=original_formula;
    return;
    }
    if (nr==-1)//return the beginning
    {
    text=original_formula.left(token_startpos.at(0));
    }
    else if (nr>=token_startpos.length()-1)//text after last token
    {
    text=original_formula.right(original_formula.length()-(token_startpos.last()+token_len.last()));
    }
    else//some token inbetween
    {
    text=original_formula.mid(token_startpos.at(nr)+token_len.at(nr),token_startpos.at(nr+1)-(token_startpos.at(nr)+token_len.at(nr)));
    }
}

void formula_to_process::sync_allocated_new_and_counter(class formula_to_process * other_formula)
{
int found_id;
//we have to find an id in this formula, that has the same type as in the other one and sync the replacements ids (this is only useful for new ids or counters)
for (int i=0;i<token_startpos.length();i++)
{
    if (token_control1.at(i)=='#' || token_control1.at(i)=='N' || token_control2.at(i)=='#' || token_control2.at(i)=='N')//the current id is actually a counter or something new (all others are irrelevant here)
    {
    found_id=-1;
        for (int j=0;j<other_formula->token_startpos.length();j++)
        {//go through all other ids and find a similar one
            if (other_formula->token_type.at(j)==token_type.at(i) &&
                other_formula->token_id1.at(j)==token_id1.at(i) &&
                other_formula->token_control1.at(j)==token_control1.at(i) &&
                other_formula->token_id2.at(j)==token_id2.at(i) &&
                other_formula->token_control2.at(j)==token_control2.at(i) )
            {
            found_id=j;
            break;
            }
        }
        //if one suitable id is found: copy entries
        if (found_id>=0)
        {
        token_graph_new.replace(i,other_formula->token_graph_new.at(found_id));
        token_set_new.replace(i,other_formula->token_set_new.at(found_id));
        token_graph_counter.replace(i,other_formula->token_graph_counter.at(found_id));
        token_set_counter.replace(i,other_formula->token_set_counter.at(found_id));
        token_graph_counter_used.replace(i,other_formula->token_graph_counter_used.at(found_id));
        token_set_counter_used.replace(i,other_formula->token_set_counter_used.at(found_id));
        }
    }
}
}

show_text_file_widget::show_text_file_widget(QWidget * parent):QTextEdit(parent)
{
max_lines=10;
read_lines=0;
f_name=NULL;
text.clear();
this->setLineWrapMode(QTextEdit::NoWrap);
this->setReadOnly(true);
}

void show_text_file_widget::init(char * filename,int max_nr_of_lines)
{
char buffer[4096],buffer2[4096],non_white,len;
int line_counter=0;
bool dotdotdotshown=false,end_table_entry=true;
if (f_name!=NULL) delete[] f_name;
f_name=new char[8+strlen(filename)];
strcpy(f_name,filename);
max_lines=max_nr_of_lines;
QFileInfo fi(f_name);
//qDebug() << "Reading from file:" << fi.absoluteFilePath();
if (fi.exists()==false)
{
errmsg("File does not exist!");
return;
}
ifstream ifi;
text.clear();
ifi.open(f_name);
read_lines=0;
text+=QString("<table>");
    while (ifi.getline(buffer,4096))
    {
    len=strlen(buffer);
    non_white=0;
    buffer2[0]='\0';
    for (int i=0;i<len;i++)
    {
        if (buffer[i]=='#')
        {
        strcpy(buffer2,buffer+i);
        buffer[i]='\0';
        len=strlen(buffer);
        break;
        }
        else if (!isspace(buffer[i]))
        {
        non_white++;
        }
    }
//qDebug() << "read one line. buffer=" << buffer << "buffer2=" << buffer2;
    end_table_entry=false;
        if (buffer[0]=='#')
        {
        text+=QString("<tr><td>");
        end_table_entry=true;
        text+=ColorToHtml(3);
        text+=QString(buffer);
        text+=ColorHtmlEnd();
        }
        else if (non_white==0 && buffer2[0]=='#')
        {
        text+=QString("<tr><td>");
        end_table_entry=true;
        text+=ColorToHtml(1);
        text+=QString(buffer);
        text+=ColorHtmlEnd();
        text+=ColorToHtml(3);
        text+=QString(buffer2);
        text+=ColorHtmlEnd();
        }
        else if (non_white==0)
        {
        line_counter=0;//reset line-counter
        dotdotdotshown=false;//reset "..."-counter
        text+=QString("<tr><td>");
        end_table_entry=true;
        text+=ColorToHtml(2);
        text+=QString("=====");
        text+=ColorHtmlEnd();
        }
        else//a normal line
        {
            if (line_counter>=max_nr_of_lines && max_nr_of_lines>=0)
            {
                if (dotdotdotshown==false)
                {
                text+=QString("<tr><td>");
                end_table_entry=true;
                text+=ColorToHtml(1);
                text+=QString("...");
                text+=ColorHtmlEnd();
                dotdotdotshown=true;
                }
            }
            else
            {
            text+=QString("<tr><td>");
            end_table_entry=true;
            text+=ColorToHtml(1);
            text+=QString(buffer);
            text+=ColorHtmlEnd();
                if (buffer2[0]!='\0')
                {
                text+=ColorToHtml(3);
                text+=QString(buffer2);
                text+=ColorHtmlEnd();
                }
            }
        line_counter++;
        }
        if (end_table_entry)
        {
        text+=QString("</td></tr>");
        text+=QString("\n");
        }
    read_lines++;
    //if (read_lines>=max_lines && max_nr_of_lines>0) break;
    }
//text+=ColorHtmlEnd();
text+=QString("</table>");
ifi.close();
this->setHtml(text);

//Test output
/*ofstream ofi;
ofi.open("/Users/andreaswinter/testhtml.html");
ofi << text.toLocal8Bit().constData() << endl;
ofi.close();*/

}

void show_text_file_widget::set_Text(QString html)
{
if (f_name!=NULL) delete[] f_name;
f_name=NULL;
read_lines=0;
this->setHtml(html);
}

ImageSelector::ImageSelector(QWidget * parent):QWidget(parent)
{
layout=new QGridLayout();
//layout->setMargin(STD_MARGIN);
layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
layout->setSpacing(STD_SPACING);
cmdSelect=new QPushButton(tr("Browse"),this);
cmdSelect->setToolTip(tr("Select a file."));
connect(cmdSelect,SIGNAL(pressed()),SLOT(doSelectFile()));
lblFile=new QLabel(tr("File:"),this);
lenFile=new QLineEdit(QString(""),this);
lenFile->setToolTip(tr("Here the absolute file path of an image is shown."));
lblTransform=new QLabel(tr("Image transformation:"),this);
cmbTransform=new QComboBox(this);
cmbTransform->setToolTip(tr("What method to use to fit the image to the available space."));
cmbTransform->addItem(tr("None"));
cmbTransform->addItem(tr("Stretch to fit width"));
cmbTransform->addItem(tr("Stretch to fit height"));
cmbTransform->addItem(tr("Stretch to fill"));
cmbTransform->addItem(tr("Tile space"));
chkRelative=new QCheckBox(tr("Relative path"),this);
chkRelative->setToolTip(tr("If checked the file-path is stored relative to the path of the project-file;\notherwise an absolute path will be stored."));
layout->addWidget(lblFile,0,0,1,1);
layout->addWidget(lenFile,0,1,1,1);
layout->addWidget(cmdSelect,0,2,1,1);
layout->addWidget(lblTransform,1,0,1,1);
layout->addWidget(cmbTransform,1,1,1,1);
layout->addWidget(chkRelative,1,2,1,1);
setLayout(layout);
connect(cmbTransform,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
connect(chkRelative,SIGNAL(toggled(bool)),SLOT(update2(bool)));
connect(lenFile,SIGNAL(returnPressed()),SLOT(update0()));
}

void ImageSelector::doSelectFile(void)
{
QString fileName = QFileDialog::getOpenFileName(this, tr("Select Image File"), user_home_dir, tr("Images (*.png *.xpm *.jpg *.bmp *.tiff)"));
    if (!fileName.isEmpty())
    {
    lenFile->setText(fileName);
    }
if (immediateUpdate) emit(someChanges());
}

void ImageSelector::initToImage(imageinfo info)
{
chkRelative->setChecked(info.relpath==0?false:true);
cmbTransform->setCurrentIndex(info.imgtransform);
lenFile->setText(QString::fromLocal8Bit(info.imgpath));//todo: convert to absolute path --> only do this at loading!
}

void ImageSelector::readImageSettings(imageinfo & info)
{
info.relpath=(chkRelative->isChecked()?1:0);
info.imgtransform=cmbTransform->currentIndex();
strcpy(info.imgpath,lenFile->text().toLocal8Bit().constData());//todo: convert to relative path if necessary --> only do this at saving!
}

void ImageSelector::update0(void)
{
if (!immediateUpdate) return;
emit(someChanges());
}

void ImageSelector::update1(int t)
{
if (!immediateUpdate) return;
(void)t;
emit(someChanges());
}

void ImageSelector::update2(bool r)
{
if (!immediateUpdate) return;
(void)r;
emit(someChanges());
}

void setDefaultImage(imageinfo & info)
{
info.relpath=false;
info.imgtransform=0;
info.imgpath[0]='\0';//no file
}

void createImageInfoText(imageinfo & info,QString & text,QString project_path)
{
text.clear();
text=QString::number(info.imgtransform)+QString(" ")+QString::number(info.relpath)+QString(" \"");
if (info.relpath && !project_path.isEmpty())
{
QDir fi_dir(project_path);
text+=fi_dir.relativeFilePath(QString::fromLocal8Bit(info.imgpath)).toLocal8Bit();
}
else
text+=QString::fromLocal8Bit(info.imgpath);
text+=QString("\"");
}

int parseImageInfoText(imageinfo & info,QString & text, QString project_path)
{
int ret=RETURN_SUCCESS;
QString found_path;
QString error_msg;
found_path.clear();
int pos1=-1,pos2=-1;
int len=text.length();
for (int i=0;i<len;i++)
{
    if (text.at(i)==QChar('\"'))
    {
    pos1=i;
    break;
    }
}
if (pos1<0)
{
///PROBLEM
    error_msg=QObject::tr("Could not find path-start-position in image-info-string: ")+text;
    errmsg(error_msg.toLocal8Bit().constData());
return RETURN_FAILURE;
}
for (int i=pos1+1;i<len;i++)
{
    if (text.at(i)==QChar('\"'))
    {
    pos2=i;
    break;
    }
}
if (pos2<0)
{
///PROBLEM
    error_msg=QObject::tr("Could not find path-end-position in image-info-string: ")+text;
    errmsg(error_msg.toLocal8Bit().constData());
return RETURN_FAILURE;
}
QString tmp_path=text.mid(pos1+1,pos2-pos1-1);
//qDebug() << "tmp_path=" << tmp_path;
QDir fi_dir(project_path);
pos1=pos2=-1;
for (int i=0;i<len;i++)
{
    if (pos1==-1 && !text.at(i).isSpace())
    {
    pos1=0;
    }
    else if (pos1==0 && text.at(i).isSpace())
    {
    pos2=i;
    break;
    }
    else if (text.at(i)==QChar('\"'))
    {
    break;
    }
}
if (pos1<0 || pos2<0)
{
///PROBLEM
    error_msg=QObject::tr("Could not find transformation-setting in image-info-string: ")+text;
    errmsg(error_msg.toLocal8Bit().constData());
return RETURN_FAILURE;
}
QString first_number=text.mid(pos1,pos2-pos1);
pos1=pos2+1;
pos2=-1;
for (int i=pos1;i<len;i++)
{
    if (pos2==-1 && !text.at(i).isSpace())
    {
    pos2=pos1;
    }
    else if (pos2>=0 && text.at(i).isSpace())
    {
    pos2=i;
    break;
    }
    else if (text.at(i)==QChar('\"'))
    {
    break;
    }
}
if (pos1<0 || pos2<0)
{
///PROBLEM
    error_msg=QObject::tr("Could not find relative-path-setting in image-info-string: ")+text;
    errmsg(error_msg.toLocal8Bit().constData());
return RETURN_FAILURE;
}
QString second_number=text.mid(pos1,pos2-pos1);
bool ok;
info.imgtransform=first_number.toInt(&ok,10);
if (!ok)
{
///PROBLEM
    error_msg=QObject::tr("Could not convert transformation-setting to integer in image-info-string: ")+text;
    errmsg(error_msg.toLocal8Bit().constData());
return RETURN_FAILURE;
}
info.relpath=second_number.toInt(&ok,10);
if (!ok)
{
///PROBLEM
    error_msg=QObject::tr("Could not convert relative-path-setting to integer in image-info-string: ")+text;
    errmsg(error_msg.toLocal8Bit().constData());
return RETURN_FAILURE;
}

found_path=QString::fromLocal8Bit(tmp_path.toLatin1());

if (info.relpath==0)//path is already absolute
{
strcpy(info.imgpath,found_path.toLocal8Bit().constData());//tmp_path is already in local8bit
}
else
{
strcpy(info.imgpath,fi_dir.absoluteFilePath(found_path).toLocal8Bit().constData());
}
return ret;
}

void drawImageOnPainter(QPainter * paint,imageinfo & info,QRect av_space,QColor default_color)//important: activate clipping on paint before using this
{
QString str1=QDir::toNativeSeparators(QString::fromLocal8Bit(info.imgpath));
QFile fi(str1);
    if (fi.exists()==false)
    {
    QString error_message;
    error_message=QObject::tr("Image file ")+fi.fileName()+QObject::tr(" does not exist.");
    errmsg(error_message.toLocal8Bit().constData());
    return;
    }
QPixmap pm(str1);
/*QImage img1(str1);
qDebug() << "img1.size=" << img1.size() << " pm.size=" << pm.size();
pm=QPixmap::fromImage(img1);*/
//qDebug() << "File=" << fi.fileName() << " pm.size=" << pm.size();
/*bool load_ok=pm.load(fi.fileName());
qDebug() << "load_ok=" << load_ok << " size=" << pm.size();*/
QPixmap pm_to_draw;
QPainter * h_painter=NULL;
int x_numbers,y_numbers;
int x_pos,y_pos;
switch (info.imgtransform)
{
case 0://no transformation
    pm_to_draw=pm;
break;
case 1://fit to width
    pm_to_draw=pm.scaledToWidth(av_space.width(),Qt::SmoothTransformation);
break;
case 2://fit to height
    pm_to_draw=pm.scaledToHeight(av_space.height(),Qt::SmoothTransformation);
break;
case 3://fit to space (change aspect ration)
    pm_to_draw=pm.scaled(av_space.size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
break;
case 4://tile
    x_numbers=av_space.width()/pm.width()+1;
    y_numbers=av_space.height()/pm.height()+1;
    x_pos=y_pos=0;
    pm_to_draw=pm.scaled(x_numbers*pm.width(),y_numbers*pm.height());
    pm_to_draw.fill(default_color);
    h_painter=new QPainter(&pm_to_draw);
    for (int i=0;i<x_numbers;i++)
    {
        y_pos=0;
        for (int j=0;j<y_numbers;j++)
        {
        h_painter->drawPixmap(x_pos,y_pos,pm);
        y_pos+=pm.height();
        }
        x_pos+=pm.width();
    }
    h_painter->end();
break;
}
//qDebug() << "pm_to_draw.size=" << pm_to_draw.size();
if (h_painter!=NULL) delete h_painter;
paint->drawPixmap(av_space.x(),av_space.y(),pm_to_draw);
}

#ifdef __cplusplus
extern "C" {
#endif

/*splits the string s at the first occurance of c into t1 and t2, returns FAILURE if no c is found or one of the resulting strings has zero-length*/
int split_string_at_first_c(const char * s,char c,char * t1,char * t2)
{
int pivot=0,i;
int len=strlen(s);
t1[0]=t2[0]='\0';
    for (i=0;i<len;i++)
    {
        if (s[i]==c)
        {
        pivot=i;
        break;
        }
    }
if (pivot<=0 || pivot>=len-1) return RETURN_FAILURE;//one of the resulting strings will have zero length
strncpy(t1,s,pivot);
t1[pivot]='\0';
strcpy(t2,s+pivot+1);
return RETURN_SUCCESS;
}

//removes whitespaces from start and end of s and returns c_s
void remove_first_last_whitespaces(const char * s,char ** c_s)
{
int st=0,nr=0;
int len=strlen(s),i;
for (i=0;i<len;i++)
{
    if (isspace(s[i])) st++;
    else break;
}
if (st<len)
{
    for (i=st;i<len;i++)
    {
        if (!isspace(s[i])) nr=i;/*nr is always updated with the index of the last non-whitespace character*/
    }
//printf("st=%d nr=%d nr-st+1=%d",st,nr,nr-st+1);
    for (i=st;i<=nr;i++)
    {
    (*c_s)[i-st]=s[i];
    }
    (*c_s)[nr-st+1]='\0';
}
else/*only whitespaces*/
{
(*c_s)[0]='\0';/*return empty string*/
}
}

/*the following function counts the occurences of the characters in c_to_find in s and enters the counts in nr_found; the number of characters to look for has to be given in nr*/
void char_counter(const char * s,char * c_to_find,int * nr_found,int nr)
{
int len=strlen(s);
int i,j;
for (i=0;i<nr;i++) nr_found[i]=0;
for (i=0;i<len;i++)
{
    for (j=0;j<nr;j++)
    {
        if (s[i]==c_to_find[j])
        {
        nr_found[j]++;
        break;
        }
    }
}
}

/*the following function converts HH:MM::SS.s with HH and MM in double to HH and MM in integer; if the result is HH>23 or HH<00 it returns FAILURE otherwise SUCCESS*/
int double_hours_to_int_hours(double hh,double mm,double * ss,int * HH,int * MM)
{
double res;
(*HH)=(int)hh;
(*MM)=(int)mm;
//remove fractions of hours and minutes
(*MM)+=(int)(60*(hh-(double)(*HH)));
(*ss)+=60.0*(mm-(double)(*MM));
//qDebug() << "erste Umwandlung:" << *HH << ":" << *MM << ":" << *ss;
//correct seconds (maybe smaller than 0 or greater than 59):
if ((*ss)<0.0)
{
    res=(double)((int)(fabs(*ss)/60.0)+1.0);
    *ss+=60.0*res;
    (*MM)-=res;
}
else if ((*ss)>=60.0)
{
    res=fmod(*ss,60.0);
    (*MM)+=(int)(*ss/60.0);
    *ss=res;
}
//qDebug() << "zweite Umwandlung:" << *HH << ":" << *MM << ":" << *ss;
//correct minutes:
if ((*MM)<0)
{
    res=(double)((int)(abs(*MM)/60.0)+1.0);
    (*MM)+=60*res;
    (*HH)-=res;
}
else if ((*MM)>=60)
{
    res=(*MM)%60;
    (*HH)+=(*MM)/60;
    (*MM)=res;
}
//qDebug() << "dritte Umwandlung:" << *HH << ":" << *MM << ":" << *ss;
if ((*HH)>23 || (*HH)<0 || (*MM)<0 || (*ss)<0.0) return RETURN_FAILURE;
return RETURN_SUCCESS;
}

int power_factor_cnt=16,computing_factor_cnt=8;
int power_factors[24]={-24,-21,-18,-15,-12,-9,-3,3,6,9,12,15,18,21,24,-6};
char power_letters[24]={'y','z','a','f','p','n','m','k','M','G','T','P','E','Z','Y',char(181)};
int computing_factors[16]={10,20,30,40,50,60,70,80};
char computing_letters[16]={'K','M','G','T','P','E','Z','Y'};

/*
the following function tries to guess the date/time-format (or read a number) in s and returns the value
if -1 is returned the guessing failed, otherwise the guessed type is returned (some types are ambiguous), the guess may not be correct
*/
int guess_date_or_number(const char* s, int absolute, double *value)
{
int type=FORMAT_INVALID;//the format (-1=invalid)
char * c_to_find;
int * nr_found;
int nr=14,ab=0,ret=0,endpos,emptyresidual;
int nr_of_digits=0,nr_of_chars=0,nr_of_white=0,len=0,lc_pos_in_comp=-1,lc_pos_in_eng=-1;//nr_of_special=0
char last_char='\0';
Dates_format dh=get_date_hint();
c_to_find=(char*)xmalloc(sizeof(char)*nr);
nr_found=(int*)xmalloc(sizeof(int)*nr);
char * c_s=(char*)xmalloc(sizeof(char)*(strlen(s)+8));
    /*if (sizeof(s)<strlen(s))
    qDebug() << "SIZEOF-Problem: strlen(s)=" << strlen(s) << "sizeof(s)=" << sizeof(s);*/
remove_first_last_whitespaces(s,&c_s);
len=strlen(c_s);
if (len<1) return RETURN_FAILURE;
/*we count the different character types for a first decision*/
last_char=toupper(c_s[len-1]);
if (isalpha(last_char))
{
    for (int i=0;i<computing_factor_cnt;i++)
    {
        if (computing_letters[i]==c_s[len-1])
        {
        lc_pos_in_comp=i;
        break;
        }
    }
    for (int i=0;i<power_factor_cnt;i++)
    {
        if (power_letters[i]==c_s[len-1])
        {
        lc_pos_in_eng=i;
        break;
        }
    }
}
nr_of_white=nr_of_digits=nr_of_chars=0;
for (int i=0;i<len;i++)
{
    if (isalpha(c_s[i])) nr_of_chars++;
    else if (isdigit(c_s[i])) nr_of_digits++;
    else if (isspace(c_s[i])) nr_of_white++;
}
//nr_of_special=len-nr_of_white-nr_of_digits-nr_of_chars;
/*We further look for typical characters to identify different date formats*/
c_to_find[0]='-';
c_to_find[1]=':';
c_to_find[2]='\'';
c_to_find[3]='\"';
c_to_find[4]='^';
c_to_find[5]='N';
c_to_find[6]='S';
c_to_find[7]='E';
c_to_find[8]='W';
c_to_find[9]='x';
c_to_find[10]='X';
c_to_find[11]='/';
c_to_find[12]='.';
c_to_find[13]='\\';
char_counter(c_s,c_to_find,nr_found,nr);
//now we have to check different possible formats
/*
qDebug() << "to check:" << c_s;
qDebug() << "Alpha=" << nr_of_chars << "Digits=" << nr_of_digits << "White=" << nr_of_white << "Special=" << nr_of_special;
    for (int i=0;i<11;i++)
    qDebug() << c_to_find[i] << "=" << nr_found[i];
*/
if (last_char=='E' || last_char=='N' || last_char=='S' || last_char=='W')
{
        if (last_char=='E' || last_char=='W') ab=0;
        else ab=1;
//qDebug() << "We are here: ab=" << ab << "last_char=" << last_char << "' found=" << nr_found[2];
    switch (nr_found[2])//'
    {
    case 0://65E
    type=FORMAT_DEGREESLON;
    break;
    case 1://65 12'E
    type=FORMAT_DEGREESMMLON;
    if (nr_found[3]==1) type=FORMAT_DEGREESMMSSLON;
    break;
    case 3://65 12' 08''E or 12' 08''E
        ret=read_date_or_number_in_format(c_s,FORMAT_DEGREESMMSSLON,absolute,value,&endpos,&emptyresidual);//guess one and try to read
        if (ret==RETURN_SUCCESS)
        type=FORMAT_DEGREESMMSSLON;
        else
        {
        ret=read_date_or_number_in_format(c_s,FORMAT_DEGREESMMSSLAT,absolute,value,&endpos,&emptyresidual);//guess one and try to read
            if (ret==RETURN_SUCCESS)
            type=FORMAT_DEGREESMMSSLAT-4;
            else
            type=FORMAT_MMSSLON;
        }
    break;
    default:
    ;//2x' or more than 3x' --> I do not know what this could be ... type stays invalid
    break;
    }
    if (type>=0 && ab==1) type+=4;
    if (type>=0)
    {
    ret=read_date_or_number_in_format(c_s,type,absolute,value,&endpos,&emptyresidual);
    if (ret==RETURN_FAILURE) type=-1;
    }
}
else if (nr_of_chars==len)//only letters
{
    c_s[0]=toupper(c_s[0]);
        for (int i=1;i<len;i++) c_s[i]=tolower(c_s[i]);
    ab=which_day_of_week(c_s);
    if (len==3)//a short format
    {
        if (ab==-1)//not a week-day
        {
        type=FORMAT_MONTHS;
        }
        else//week-day
        {
        type=FORMAT_DAYOFWEEKS;
        }
    }
    else//a long format
    {
        if (ab==-1)//not a week-day
        {
        type=FORMAT_MONTHL;
        }
        else//week-day
        {
        type=FORMAT_DAYOFWEEKL;
        }
    }
    if (type>=0)
    {
    ret=read_date_or_number_in_format(c_s,type,absolute,value,&endpos,&emptyresidual);
    if (ret==RETURN_FAILURE) type=-1;
    }
}
else if (nr_found[1]==0 && (nr_found[0]==2 || nr_found[11]==2 || nr_found[12]==2))//2x- or 2x/ or 2x. and no :
{
//qDebug() << "we try to guess a date-format";
    ret=RETURN_FAILURE;
    if (dh==FMT_iso)//YY-MM-DD
    {
    type=FORMAT_YYMMDD;
    }
    else if (dh==FMT_european)//DD-MM-YY
    {
    type=FORMAT_DDMMYY;
    }
    else if (dh==FMT_us)//MM-DD-YY
    {
    type=FORMAT_MMDDYY;
    }

    if (dh!=FMT_nohint)
    {
    ret=read_date_or_number_in_format(c_s,type,absolute,value,&endpos,&emptyresidual);
    }
    else//no hint -- we have to guess
    {
    type=FORMAT_YYMMDD;
    ret=read_date_or_number_in_format(c_s,type,absolute,value,&endpos,&emptyresidual);
        if (ret==RETURN_FAILURE)
        {
        type=FORMAT_DDMMYY;
        ret=read_date_or_number_in_format(c_s,type,absolute,value,&endpos,&emptyresidual);
            if (ret==RETURN_FAILURE)
            {
            type=FORMAT_MMDDYY;
            ret=read_date_or_number_in_format(c_s,type,absolute,value,&endpos,&emptyresidual);
            }
        }
    }

    if (ret==RETURN_FAILURE) type=-1;
}
else if (nr_found[1]==0 && (nr_found[0]==1 || nr_found[11]==1) && nr_found[13]==0)//1x(- or /) and no ':' and no '\'
{
    type=FORMAT_MMYY;
    ret=read_date_or_number_in_format(c_s,type,absolute,value,&endpos,&emptyresidual);
    if (ret==RETURN_FAILURE)
    {
        if (isalpha(c_s[0]) && !isdigit(c_s[0])) // it starts with a letter (not a number)
        {
        ret=read_date_or_number_in_format(c_s, FORMAT_MONTHDAY, TRUE, value,&endpos,&emptyresidual);
            if (ret==RETURN_SUCCESS)
            {
            type=FORMAT_MONTHDAY;
            }
            else
            {
            ret=read_date_or_number_in_format(c_s, FORMAT_MONTHSY, TRUE, value,&endpos,&emptyresidual);
                if (ret==RETURN_SUCCESS)
                {
                type=FORMAT_MONTHSY;
                }
                else
                {
                type=-1;
                }
            }
        }
        else if (isdigit(c_s[0]) || c_s[0]=='-')// it starts with a number or a '-'
        {
        ret=read_date_or_number_in_format(c_s, FORMAT_DAYMONTH, TRUE, value,&endpos,&emptyresidual);
            if (ret==RETURN_SUCCESS)
            {
            type=FORMAT_DAYMONTH;
            }
            else
            {
            ret=read_date_or_number_in_format(c_s, FORMAT_EXPONENTIAL, TRUE, value,&endpos,&emptyresidual);//it could be a negative number in exponential format
                if (ret==RETURN_SUCCESS)
                type=FORMAT_EXPONENTIAL;
                else
                type=-1;
            }
        }
    }
}
else if (nr_found[1]==2 && (nr_found[0]==2 || nr_found[11]==2))//2x- or 2x/ and 2x:
{
    if (dh==FMT_iso)//YY-MM-DD HH:MM:SS
    {
    type=FORMAT_YYMMDDHMS;
    }
    /*else if (dh==FMT_european)//
    {
    type=;//none such format here (DD-MM-YY HH:MM:SS)
    }*/
    else//MM-DD-YY HH:MM:SS or no hint
    {
    type=FORMAT_MMDDYYHMS;
    }
    ret=read_date_or_number_in_format(c_s,type,absolute,value,&endpos,&emptyresidual);
        if (ret==RETURN_FAILURE)
        {
        ret=read_date_or_number_in_format(c_s, FORMAT_YYDYHMS, TRUE, value,&endpos,&emptyresidual);
            if (ret==RETURN_SUCCESS)
            type=FORMAT_YYDYHMS;
            else
            type=-1;
        }
}
else if (nr_found[1]==2 && (nr_found[0]==1 || nr_found[11]==1))//1x- or 1x/ and 2x:
{
    type=FORMAT_MMDDHMS;
    ret=read_date_or_number_in_format(c_s,type,absolute,value,&endpos,&emptyresidual);
    if (ret==RETURN_FAILURE) type=-1;
}
else if (nr_of_chars>2 && nr_found[1]==0 && (nr_found[0]==1 || nr_found[11]==1))//1x- or 1x/ and no :
{
    if (isalpha(c_s[0]))
    {
        ab=-1;
        for (int i=0;i<len;i++)
        {
            if (c_s[i]=='-' || c_s[i]=='/')
            {
            ab=i;
            break;
            }
        }
        if (ab==3)
        type=FORMAT_MONTHSY;
        else
        type=FORMAT_MONTHDAY;
    }
    else
    type=FORMAT_DAYMONTH;
    ret=read_date_or_number_in_format(c_s,type,absolute,value,&endpos,&emptyresidual);
    if (ret==RETURN_FAILURE) type=-1;
}
else if (nr_found[1]==2 && (nr_found[0]==0 || nr_found[11]==0))//no (- or /) and 2x:
{
    type=FORMAT_HMS;
    ret=read_date_or_number_in_format(c_s,type,absolute,value,&endpos,&emptyresidual);
    if (ret==RETURN_FAILURE) type=-1;
}
else if (lc_pos_in_comp>=0)
{
    type=FORMAT_COMPUTING;
    ret=read_date_or_number_in_format(c_s,type,absolute,value,&endpos,&emptyresidual);
    if (ret==RETURN_FAILURE) type=-1;
}
else if (lc_pos_in_eng>=0)
{
    type=FORMAT_ENGINEERING;
    ret=read_date_or_number_in_format(c_s,type,absolute,value,&endpos,&emptyresidual);
    if (ret==RETURN_FAILURE) type=-1;
}
else if (read_date_or_number_in_format(c_s,FORMAT_SCIENTIFIC,absolute,value,&endpos,&emptyresidual)==RETURN_SUCCESS)
{
    type=FORMAT_SCIENTIFIC;
}
else if (read_date_or_number_in_format(c_s,FORMAT_POWER,absolute,value,&endpos,&emptyresidual)==RETURN_SUCCESS)
{
    type=FORMAT_POWER;
}
else
{
    type=FORMAT_GENERAL;
    ret=read_date_or_number_in_format(c_s,type,absolute,value,&endpos,&emptyresidual);
    if (ret==RETURN_FAILURE) type=-1;
}
xfree(c_to_find);
xfree(nr_found);
return type;
}

/*the following function just reads from s a value in a given format and just returns whether it worked (RETURN_SUCCESS) or not (RETURN_FAILURE); s is assumed not to conatin leading whitespaces!*/
int read_date_or_number_in_format(const char *s1, int format, int absolute, double * value, int *endpos, int *emptyresidual)
{
//static int expected_tokens[1+FORMAT_YYDYHMS]={1,1,1,3,5,2,2,3,3,3,2,2,2,2,1,2,1,1,1,1,3,5,6,6,2,3,4,2,2,3,4,3,1,5};
char *s=new char[2+strlen(s1)];
strcpy(s,s1);
removePointInFormat(s,format);
int ret=RETURN_FAILURE;
int npos=0;
int read_tokens=0,i,len=strlen(s);
static double read_values[16];
static int read_values_i[16];
static char read_chars[16];//for single characters and strings
static char tmp_chars1[16],tmp_chars2[16],tmp_c;
char * tmp_str;
if (format<0 || format>FORMAT_YYDYHMS) return ret;
double d_refdate=get_ref_date(),fsecond=0.0;
if (!absolute) d_refdate=0.0;//No absolute dates wanted --> set ref-date to zero
int year,month,day,hour,minute,second,dayofyear;
int ref_year,ref_day,ref_month,ref_hour,ref_second,ref_minute,ref_day_of_year;
double ref_fsecond;
lock_dirtystate(TRUE);
jul_to_cal_and_time_with_yday(d_refdate,NO_ROUND,&ref_year,&ref_month,&ref_day,&ref_hour,&ref_minute,&ref_second,&ref_fsecond,&ref_day_of_year);
//jul_to_cal_and_time_with_yday(d_refdate,NO_ROUND,&year,&month,&day,&hour,&minute,&second,&fsecond,&dayofyear);
//printf("RefDate = %02d.%02d.%02d %02d:%02d:%02d\n",day,month,year,hour,minute,second);
*value=0.0;
switch (format)
{
case FORMAT_DECIMAL:
case FORMAT_EXPONENTIAL:
case FORMAT_GENERAL:
read_tokens=sscanf(s,"%lf%n",read_values,&npos);//just read a single double-value
    if (read_tokens==1)
    {
    ret=RETURN_SUCCESS;
    *value=read_values[0];
    }
break;
case FORMAT_DAYOFYEAR:
case FORMAT_YYYY:
read_tokens=sscanf(s,"%d%n",&ret,&npos);//just read a single int-value
    if (read_tokens==1)
    {
        if (format==FORMAT_DAYOFYEAR && (ret<1 || ret>366))
        ret=RETURN_FAILURE;
        else
        {
        *value=(double)ret;
        ret=RETURN_SUCCESS;
        }
    }
    else
    ret=RETURN_FAILURE;
break;
case FORMAT_POWER:
read_tokens=sscanf(s,"%lf ^%lf%n",read_values,read_values+1,&npos);
    if (read_tokens==2) ret=RETURN_SUCCESS;
    else
    {
    read_tokens=sscanf(s,"%lf \\S%lf%n",read_values,read_values+1,&npos);
        if (read_tokens==2) ret=RETURN_SUCCESS;
        else
        {
        read_tokens=sscanf(s,"%lf (%lf)%n",read_values,read_values+1,&npos);
        if (read_tokens==2) ret=RETURN_SUCCESS;
        }
    }
    if (ret==RETURN_SUCCESS) *value=pow(fabs(read_values[0]),read_values[1])*sign(read_values[0]);
break;
case FORMAT_SCIENTIFIC:
read_tokens=sscanf(s,"%lf%c10 ^%d%n",read_values,read_chars,read_values_i,&npos);/*1.5x10^3*/
read_chars[0]=toupper(read_chars[0]);
if (read_tokens==3 && (read_chars[0]=='X' || read_chars[0]=='*'))
ret=RETURN_SUCCESS;
else
{
read_tokens=sscanf(s,"%lf%c10 \\S%d%n",read_values,read_chars,read_values_i,&npos);/*1.5x10\\S3*/
read_chars[0]=toupper(read_chars[0]);
    if (read_tokens==3 && (read_chars[0]=='X' || read_chars[0]=='*'))
    ret=RETURN_SUCCESS;
    else
    {
    read_tokens=sscanf(s,"%lf%c10 (%d)%n",read_values,read_chars,read_values_i,&npos);/*1.5x10\\S3*/
    read_chars[0]=toupper(read_chars[0]);
        if (read_tokens==3 && (read_chars[0]=='X' || read_chars[0]=='*'))
        ret=RETURN_SUCCESS;
        else
        {
        read_tokens=sscanf(s,"%lf\\x\\c4\\C\\f{}10 \\S%d%n",read_values,read_values_i,&npos);/*1.5\\x\\c4\\C\\f{}10\\S3 - TeX-format*/
            if (read_tokens==2)
            ret=RETURN_SUCCESS;
        }
    }
}
if (ret==RETURN_SUCCESS) *value=read_values[0]*pow(10.0,(double)read_values_i[0]);
break;
case FORMAT_ENGINEERING:
read_tokens=sscanf(s,"%lf %c%n",read_values,read_chars,&npos);
if (read_tokens==2)
{
ret=-1;
//check for micro
    if (read_chars[0]=='m' || read_chars[0]=='\\')//could be micro
    {
    read_tokens=sscanf(s,"%lf %c%c%n",read_values,read_chars,read_chars+1,&npos);
        if (read_tokens<3)//not micro
        {
        read_tokens=sscanf(s,"%lf %c%n",read_values,read_chars,&npos);
        }
        else if (read_chars[0]=='m' && read_chars[1]=='k')
        {
        *value=read_values[0]*1e-6;
        ret=RETURN_SUCCESS;
        }
        else if (read_chars[0]=='\\' && read_chars[1]=='x')
        {
        read_tokens=sscanf(s,"%lf %c%c%c%n",read_values,read_chars,read_chars+1,read_chars+2,&npos);
            if (read_chars[0]=='\\' && read_chars[1]=='x' && (read_chars[2]=='m' || read_chars[2]=='l'))
            {
            *value=read_values[0]*1e-6;
            ret=RETURN_SUCCESS;
            }
            else
            {
            //cerr << "Number format not recognized in engineering-format: " << s << endl;
            ret=RETURN_FAILURE;
            }
        }
        else
        {
        //cerr << "Number format not recognized in engineering-format: " << s << endl;
        ret=RETURN_FAILURE;
        }
    }
    if (ret==-1)
    {
        for (i=0;i<power_factor_cnt;i++)
        {
            if (power_letters[i]==read_chars[0])
            {
            ret=i;
            break;
            }
        }
        if (ret>=0)
        {
        *value=read_values[0]*pow(10.0,power_factors[ret]);
        ret=RETURN_SUCCESS;
        }
        else
        {
        ret=RETURN_FAILURE;
        }
    }
}
else
{
read_tokens=sscanf(s,"%lf%n",read_values,&npos);
    if (read_tokens==1)
    {
    *value=read_values[0];
    ret=RETURN_SUCCESS;
    }
}
break;
case FORMAT_COMPUTING:
read_tokens=sscanf(s,"%lf %c%n",read_values,read_chars,&npos);
//qDebug() << read_tokens << "value=" << read_values[0] << "Char=" << read_chars[0];
if (read_tokens==2)
{
    ret=-1;
    read_chars[0]=toupper(read_chars[0]);
    for (i=0;i<computing_factor_cnt;i++)
    {
        if (computing_letters[i]==read_chars[0])
        {
        ret=i;
        break;
        }
    }
//qDebug() << read_chars[0] << "-->" << ret << "faktor=" << computing_factors[ret];
    if (ret>=0)
    {
    *value=read_values[0]*pow(2.0,computing_factors[ret]);
    ret=RETURN_SUCCESS;
    }
    else
    {
    ret=RETURN_FAILURE;
    }
}
else//no extension --> a simple double value
{
read_tokens=sscanf(s,"%lf%n",read_values,&npos);
    if (read_tokens==1)
    {
    *value=read_values[0];
        if (strlen(s)>1 && toupper(s[strlen(s)-1])=='E')
        {
        *value*=pow(2.0,60.0);
        }
    ret=RETURN_SUCCESS;
    }
}
break;
case FORMAT_DDMMYY:
case FORMAT_MMDDYY:
case FORMAT_YYMMDD:
//qDebug() << "h=" << hour << "m=" << minute << "s=" << second;
hour=ref_hour;
minute=second=0;
fsecond=0.0;
read_tokens=sscanf(s,"%d-%d-%d%n",read_values_i,read_values_i+1,read_values_i+2,&npos);
if (read_tokens!=3)
{
read_tokens=sscanf(s,"%d/%d/%d%n",read_values_i,read_values_i+1,read_values_i+2,&npos);
    if (read_tokens!=3)
    {
    read_tokens=sscanf(s,"%d.%d.%d%n",read_values_i,read_values_i+1,read_values_i+2,&npos);
        if (read_tokens!=3)//this can be dangerous, because it could rather be a time format and not a date format -- not good for autoguessing, but if the user specifies it...
        {
        read_tokens=sscanf(s,"%d:%d:%d%n",read_values_i,read_values_i+1,read_values_i+2,&npos);
        }
    }
}
if (read_tokens==3)
{
//ret=RETURN_SUCCESS;
    if (format==FORMAT_DDMMYY)
    {
    day=read_values_i[0];
    month=read_values_i[1];
    year=read_values_i[2];
    }
    else if (format==FORMAT_MMDDYY)
    {
    day=read_values_i[1];
    month=read_values_i[0];
    year=read_values_i[2];
    }
    else
    {
    day=read_values_i[2];
    month=read_values_i[1];
    year=read_values_i[0];
    }
year=force_four_digit_year(year);
//qDebug() << "d=" << day << "m=" << month << "y=" << year;
ret=simple_check_date(year,month,day);
//qDebug() << "d=" << day << "m=" << month << "y=" << year;
    if (ret==RETURN_FAILURE)
    *value=0.0;
    else
    *value=cal_and_time_to_jul(year,month,day,hour,minute,second*1.0+fsecond);
//qDebug() << "val=" << *value;
}
break;
case FORMAT_MMYY:
case FORMAT_MMDD:
    year=ref_year;
    month=ref_month;
    day=ref_day;
    hour=ref_hour;
    minute=second=0;
    fsecond=0.0;
read_tokens=sscanf(s,"%d-%d%n",read_values_i,read_values_i+1,&npos);
if (read_tokens!=2)
{
read_tokens=sscanf(s,"%d/%d%n",read_values_i,read_values_i+1,&npos);
    if (read_tokens!=2)
    {
    read_tokens=sscanf(s,"%d.%d%n",read_values_i,read_values_i+1,&npos);
        if (read_tokens!=2)//this can be dangerous, because it could rather be a time format and not a date format
        read_tokens=sscanf(s,"%d:%d%n",read_values_i,read_values_i+1,&npos);
    }
}
if (read_tokens==2)
{
//ret=RETURN_SUCCESS;
    if (format==FORMAT_MMYY)
    {
    month=read_values_i[0];
    year=read_values_i[1];
    }
    else
    {
    day=read_values_i[1];
    month=read_values_i[0];
    }
    year=force_four_digit_year(year);
ret=simple_check_date(year,month,day);
    if (ret==RETURN_FAILURE)
    *value=0.0;
    else
    *value=cal_and_time_to_jul(year,month,day,hour,minute,second*1.0+fsecond);
}
break;
case FORMAT_MONTHDAY:
case FORMAT_DAYMONTH:
case FORMAT_MONTHSY:
    year=ref_year;
    month=ref_month;
    day=ref_day;
    hour=ref_hour;
    minute=second=0;
    fsecond=0.0;
read_tokens=split_string_at_first_c(s,'-',tmp_chars1,tmp_chars2);
if (read_tokens==RETURN_FAILURE) read_tokens=split_string_at_first_c(s,'/',tmp_chars1,tmp_chars2);
if (read_tokens==RETURN_FAILURE) read_tokens=split_string_at_first_c(s,'.',tmp_chars1,tmp_chars2);
    if (read_tokens==RETURN_FAILURE)
    {
    ret=RETURN_FAILURE;
    }
    else
    {
        if (format==FORMAT_DAYMONTH)
        {
        read_tokens=sscanf(tmp_chars1,"%d%n",&day,&npos);
            if (read_tokens<1)
            {
            ret=RETURN_FAILURE;
            }
            else
            {
            len=strlen(tmp_chars2);
            tmp_chars2[0]=toupper(tmp_chars2[0]);
                for (i=1;i<len;i++) tmp_chars2[i]=tolower(tmp_chars2[i]);
            month=which_month_of_year(tmp_chars2);
            if (month<0) ret=RETURN_FAILURE;
            else ret=RETURN_SUCCESS;
            }
        npos+=1+strlen(tmp_chars2);
        }
        else if (format==FORMAT_MONTHSY)
        {
        read_tokens=sscanf(tmp_chars2,"%d%n",&year,&npos);
            if (read_tokens<1)
            {
            ret=RETURN_FAILURE;
            }
            else
            {
            len=strlen(tmp_chars1);
            tmp_chars1[0]=toupper(tmp_chars1[0]);
                for (i=1;i<len;i++) tmp_chars1[i]=tolower(tmp_chars1[i]);
            month=which_month_of_year(tmp_chars1);
            if (month<0) ret=RETURN_FAILURE;
            else ret=RETURN_SUCCESS;
            }
        npos+=1+strlen(tmp_chars1);
        }
        else//format==FORMAT_MONTHDAY
        {
        read_tokens=sscanf(tmp_chars2,"%d%n",&day,&npos);
            if (read_tokens<1)
            {
            ret=RETURN_FAILURE;
            }
            else
            {
            len=strlen(tmp_chars1);
            tmp_chars1[0]=toupper(tmp_chars1[0]);
                for (i=1;i<len;i++) tmp_chars1[i]=tolower(tmp_chars1[i]);
            month=which_month_of_year(tmp_chars1);
            if (month<0) ret=RETURN_FAILURE;
            else ret=RETURN_SUCCESS;
            }
        npos+=1+strlen(tmp_chars1);
        }
    }
//printf("gelesen: #%s#%s# read_tokens=%d#day=%d#month=%d\n",tmp_chars1,tmp_chars2,read_tokens,day,month);
if (ret!=RETURN_FAILURE)
{
    year=force_four_digit_year(year);
ret=simple_check_date(year,month,day);
    if (ret==RETURN_FAILURE)
    *value=0.0;
    else
    *value=cal_and_time_to_jul(year,month,day,hour,minute,second*1.0+fsecond);
}
break;
case FORMAT_MONTHS:
case FORMAT_MONTHL:
case FORMAT_DAYOFWEEKS:
case FORMAT_DAYOFWEEKL:
    year=month=day=hour=minute=second=0;
    fsecond=0.0;
read_tokens=sscanf(s,"%16s%n",read_chars,&npos);
if (read_tokens==1)
{
    len=strlen(read_chars);
    read_chars[0]=toupper(read_chars[0]);
        for (i=1;i<len;i++) read_chars[i]=tolower(read_chars[i]);
//printf("gelesen %s --> %s\n",s,read_chars);
    if (format==FORMAT_MONTHS || format==FORMAT_MONTHL)
    {
    month=which_month_of_year(read_chars);
        if (month<0) ret=RETURN_FAILURE;
        else
        {
        *value=month;
        ret=RETURN_SUCCESS;
        }
    }
    else
    {
    day=which_day_of_week(read_chars);
        if (day<0) ret=RETURN_FAILURE;
        else
        {
        *value=day;
        ret=RETURN_SUCCESS;
        }
    }
/*if (ret==RETURN_SUCCESS)
*value=cal_and_time_to_jul(year,month,day,hour,minute,second*1.0+fsecond);*/
}
break;
case FORMAT_HMS:
    year=ref_year;
    month=ref_month;
    day=ref_day;
    fsecond=0.0;
read_tokens=sscanf(s,"%lf:%lf:%lf%n",read_values,read_values+1,read_values+2,&npos);
if (read_tokens!=3)
{
read_tokens=sscanf(s,"%lf/%lf/%lf%n",read_values,read_values+1,read_values+2,&npos);
    if (read_tokens!=3)
    read_tokens=sscanf(s,"%lf-%lf-%lf%n",read_values,read_values+1,read_values+2,&npos);
}
    if (read_tokens==3)
    {
//qDebug() << "read=" << read_values[0] << ":" << read_values[1] << ":" << read_values[2];
    ret=double_hours_to_int_hours(read_values[0],read_values[1],read_values+2,read_values_i,read_values_i+1);
//qDebug() << "converted to=" << read_values_i[0] << ":" << read_values_i[1] << ":" << read_values[2];
    if (ret==RETURN_SUCCESS)
    *value=cal_and_time_to_jul(year,month,day,read_values_i[0],read_values_i[1],read_values[2]*1.0+fsecond);
    }
break;
case FORMAT_MMDDHMS:
    year=ref_year;
    month=day=hour=minute=second=0;
    fsecond=0.0;
read_tokens=sscanf(s,"%d-%d%c%lf:%lf:%lf%n",read_values_i,read_values_i+1,&tmp_c,read_values,read_values+1,read_values+2,&npos);
//qDebug() << read_tokens << " MMDDHMS: " << read_values_i[0] << " " << read_values_i[1] << " |" << tmp_c << "| " << read_values[0] << " " << read_values[1] << " " << read_values[2];
if (read_tokens!=6)
{
read_tokens=sscanf(s,"%d/%d%c%lf:%lf:%lf%n",read_values_i,read_values_i+1,&tmp_c,read_values,read_values+1,read_values+2,&npos);
    if (read_tokens!=6)
    {
    read_tokens=sscanf(s,"%d.%d%c%lf:%lf:%lf%n",read_values_i,read_values_i+1,&tmp_c,read_values,read_values+1,read_values+2,&npos);
        if (read_tokens!=6)
        {
        read_tokens=sscanf(s,"%d:%d%c%lf:%lf:%lf%n",read_values_i,read_values_i+1,&tmp_c,read_values,read_values+1,read_values+2,&npos);
            if (read_tokens!=6)
            {
            read_tokens=sscanf(s,"%d-%d%c%lf-%lf-%lf%n",read_values_i,read_values_i+1,&tmp_c,read_values,read_values+1,read_values+2,&npos);
                if (read_tokens!=6)
                {
                read_tokens=sscanf(s,"%d/%d%c%lf-%lf-%lf%n",read_values_i,read_values_i+1,&tmp_c,read_values,read_values+1,read_values+2,&npos);
                    if (read_tokens!=6)
                    {
                    read_tokens=sscanf(s,"%d.%d%c%lf-%lf-%lf%n",read_values_i,read_values_i+1,&tmp_c,read_values,read_values+1,read_values+2,&npos);
                        if (read_tokens!=6)
                        {
                        read_tokens=sscanf(s,"%d:%d%c%lf-%lf-%lf%n",read_values_i,read_values_i+1,&tmp_c,read_values,read_values+1,read_values+2,&npos);
                            if (read_tokens!=6)
                            {
                            read_tokens=sscanf(s,"%d-%d%c%lf/%lf/%lf%n",read_values_i,read_values_i+1,&tmp_c,read_values,read_values+1,read_values+2,&npos);
                                if (read_tokens!=6)
                                {
                                read_tokens=sscanf(s,"%d/%d%c%lf/%lf/%lf%n",read_values_i,read_values_i+1,&tmp_c,read_values,read_values+1,read_values+2,&npos);
                                    if (read_tokens!=6)
                                    {
                                    read_tokens=sscanf(s,"%d.%d%c%lf/%lf/%lf%n",read_values_i,read_values_i+1,&tmp_c,read_values,read_values+1,read_values+2,&npos);
                                        if (read_tokens!=6)
                                        read_tokens=sscanf(s,"%d:%d%c%lf/%lf/%lf%n",read_values_i,read_values_i+1,&tmp_c,read_values,read_values+1,read_values+2,&npos);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
    if (read_tokens==6 && (tmp_c==' ' || tmp_c=='T'))
    {
    ret=double_hours_to_int_hours(read_values[0],read_values[1],read_values+2,read_values_i+2,read_values_i+3);
    //qDebug() << "Year=" << year << "Month=" << read_values_i[0] << "Day=" << read_values_i[1] << "Hour=" << read_values_i[2] << "Min=" << read_values_i[3] << "Sec=" << read_values[2]*1.0+fsecond;
    if (ret==RETURN_SUCCESS)
    *value=cal_and_time_to_jul(year,read_values_i[0],read_values_i[1],read_values_i[2],read_values_i[3],read_values[2]*1.0+fsecond);
    else
    *value=0.0;
    }
break;
case FORMAT_MMDDYYHMS:
case FORMAT_YYMMDDHMS:
    year=month=day=hour=minute=second=0;
    fsecond=0.0;
read_tokens=sscanf(s,"%d-%d-%d%c%lf:%lf:%lf%n",read_values_i,read_values_i+1,read_values_i+2,&tmp_c,read_values,read_values+1,read_values+2,&npos);
if (read_tokens!=7)
{
read_tokens=sscanf(s,"%d/%d/%d%c%lf:%lf:%lf%n",read_values_i,read_values_i+1,read_values_i+2,&tmp_c,read_values,read_values+1,read_values+2,&npos);
    if (read_tokens!=7)
    {
    read_tokens=sscanf(s,"%d.%d.%d%c%lf:%lf:%lf%n",read_values_i,read_values_i+1,read_values_i+2,&tmp_c,read_values,read_values+1,read_values+2,&npos);
        if (read_tokens!=7)
        {
        read_tokens=sscanf(s,"%d:%d:%d%c%lf:%lf:%lf%n",read_values_i,read_values_i+1,read_values_i+2,&tmp_c,read_values,read_values+1,read_values+2,&npos);
            if (read_tokens!=7)
            {
            read_tokens=sscanf(s,"%d-%d-%d%c%lf-%lf-%lf%n",read_values_i,read_values_i+1,read_values_i+2,&tmp_c,read_values,read_values+1,read_values+2,&npos);
                if (read_tokens!=7)
                {
                read_tokens=sscanf(s,"%d/%d/%d%c%lf-%lf-%lf%n",read_values_i,read_values_i+1,read_values_i+2,&tmp_c,read_values,read_values+1,read_values+2,&npos);
                    if (read_tokens!=7)
                    {
                    read_tokens=sscanf(s,"%d.%d.%d%c%lf-%lf-%lf%n",read_values_i,read_values_i+1,read_values_i+2,&tmp_c,read_values,read_values+1,read_values+2,&npos);
                        if (read_tokens!=7)
                        {
                        read_tokens=sscanf(s,"%d:%d:%d%c%lf-%lf-%lf%n",read_values_i,read_values_i+1,read_values_i+2,&tmp_c,read_values,read_values+1,read_values+2,&npos);
                            if (read_tokens!=7)
                            {
                            read_tokens=sscanf(s,"%d-%d-%d%c%lf/%lf/%lf%n",read_values_i,read_values_i+1,read_values_i+2,&tmp_c,read_values,read_values+1,read_values+2,&npos);
                                if (read_tokens!=7)
                                {
                                read_tokens=sscanf(s,"%d/%d/%d%c%lf/%lf/%lf%n",read_values_i,read_values_i+1,read_values_i+2,&tmp_c,read_values,read_values+1,read_values+2,&npos);
                                    if (read_tokens!=7)
                                    {
                                    read_tokens=sscanf(s,"%d.%d.%d%c%lf/%lf/%lf%n",read_values_i,read_values_i+1,read_values_i+2,&tmp_c,read_values,read_values+1,read_values+2,&npos);
                                        if (read_tokens!=7)
                                        {
                                        read_tokens=sscanf(s,"%d:%d:%d%c%lf/%lf/%lf%n",read_values_i,read_values_i+1,read_values_i+2,&tmp_c,read_values,read_values+1,read_values+2,&npos);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
    if (read_tokens==7 && (tmp_c==' ' || tmp_c=='T'))
    {
    ret=double_hours_to_int_hours(read_values[0],read_values[1],read_values+2,read_values_i+3,read_values_i+4);
        if (format==FORMAT_MMDDYYHMS)
        {
            read_values_i[2]=force_four_digit_year(read_values_i[2]);
        *value=cal_and_time_to_jul(read_values_i[2],read_values_i[0],read_values_i[1],read_values_i[3],read_values_i[4],read_values[2]*1.0+fsecond);
        }
        else//FORMAT_YYMMDDHMS
        {
            read_values_i[0]=force_four_digit_year(read_values_i[0]);
        *value=cal_and_time_to_jul(read_values_i[0],read_values_i[1],read_values_i[2],read_values_i[3],read_values_i[4],read_values[2]*1.0+fsecond);
        }
    }
break;
case FORMAT_DEGREESLON:
case FORMAT_DEGREESLAT:
    tmp_str=new char[2+len];
for (i=0;i<len;i++)
{
    if (isalpha(s[i]) && !isdigit(s[i]))
    {
    tmp_c=s[i];
    tmp_str[i]='\0';
        if (s[i]=='\0')
        npos=i;
        else
        npos=i+1;
    break;
    }
    else tmp_str[i]=s[i];
}
read_tokens=sscanf(tmp_str,"%lf",read_values);
delete[] tmp_str;
    if (read_tokens==1)
    {
    ret=RETURN_SUCCESS;
        read_chars[0]=toupper(tmp_c);
        if ((read_chars[0]=='W' && format==FORMAT_DEGREESLON) || (read_chars[0]=='S' && format==FORMAT_DEGREESLAT))
        *value=-read_values[0];
        else if ((read_chars[0]=='E' && format==FORMAT_DEGREESLON) || (read_chars[0]=='N' && format==FORMAT_DEGREESLAT))
        *value=read_values[0];
        else
        ret=RETURN_FAILURE;
    }
    else
    {
    ret=RETURN_FAILURE;
    }
break;
case FORMAT_DEGREESMMLON:
case FORMAT_DEGREESMMLAT:
read_tokens=sscanf(s,"%lf %lf' %c%n",read_values,read_values+1,read_chars,&npos);//
if (read_tokens==3)
{
read_values[0]+=read_values[1]/60.0;
read_chars[0]=toupper(read_chars[0]);
    if ((read_chars[0]=='W' && format==FORMAT_DEGREESMMLON) || (read_chars[0]=='S' && format==FORMAT_DEGREESMMLAT))
    {
    *value=-read_values[0];
    ret=RETURN_SUCCESS;
    }
    else if ((read_chars[0]=='E' && format==FORMAT_DEGREESMMLON) || (read_chars[0]=='N' && format==FORMAT_DEGREESMMLAT))
    {
    *value=read_values[0];
    ret=RETURN_SUCCESS;
    }
    else
    ret=RETURN_FAILURE;
//printf("degrees_format: Read: %lf %c\n",read_values[0],read_chars[0]);
}
else
ret=RETURN_FAILURE;
break;
case FORMAT_DEGREESMMSSLON:
case FORMAT_DEGREESMMSSLAT:
read_tokens=sscanf(s,"%lf %lf' %lf'' %c%n",read_values,read_values+1,read_values+2,read_chars,&npos);
if (read_tokens!=4)
read_tokens=sscanf(s,"%lf %lf' %lf\" %c%n",read_values,read_values+1,read_values+2,read_chars,&npos);

if (read_tokens==4)
{
read_values[0]+=read_values[1]/60.0+read_values[2]/3600.0;
read_chars[0]=toupper(read_chars[0]);
    if ((read_chars[0]=='W' && format==FORMAT_DEGREESMMSSLON) || (read_chars[0]=='S' && format==FORMAT_DEGREESMMSSLAT))
    {
    *value=-read_values[0];
    ret=RETURN_SUCCESS;
    }
    else if ((read_chars[0]=='E' && format==FORMAT_DEGREESMMSSLON) || (read_chars[0]=='N' && format==FORMAT_DEGREESMMSSLAT))
    {
    *value=read_values[0];
    ret=RETURN_SUCCESS;
    }
    else
    ret=RETURN_FAILURE;
//printf("degrees_format: Read: %lf %c\n",read_values[0],read_chars[0]);
}
else
ret=RETURN_FAILURE;
break;
case FORMAT_MMSSLON:
case FORMAT_MMSSLAT:
read_tokens=sscanf(s,"%lf' %lf'' %c%n",read_values,read_values+1,read_chars,&npos);
if (read_tokens!=3)
read_tokens=sscanf(s,"%lf' %lf\" %c%n",read_values,read_values+1,read_chars,&npos);

if (read_tokens==3)
{
read_values[0]=read_values[0]/60.0+read_values[1]/3600.0;
read_chars[0]=toupper(read_chars[0]);
    if ((read_chars[0]=='W' && format==FORMAT_MMSSLON) || (read_chars[0]=='S' && format==FORMAT_MMSSLAT))
    {
    *value=-read_values[0];
    ret=RETURN_SUCCESS;
    }
    else if ((read_chars[0]=='E' && format==FORMAT_MMSSLON) || (read_chars[0]=='N' && format==FORMAT_MMSSLAT))
    {
    *value=read_values[0];
    ret=RETURN_SUCCESS;
    }
    else
    ret=RETURN_FAILURE;
//printf("degrees_format: Read: %lf %c\n",read_values[0],read_chars[0]);
}
break;
case FORMAT_YYDYHMS://a special format with fixed separators
    year=month=day=hour=minute=second=0;
    fsecond=0.0;
read_tokens=sscanf(s,"%d/%d/%lf:%lf:%lf%n",read_values_i,read_values_i+1,read_values,read_values+1,read_values+2,&npos);
if (read_tokens==5)
{
    read_values_i[0]=force_four_digit_year(read_values_i[0]);
ret=gregorian_dayofyear_to_monthday(read_values_i[1],read_values_i[0],read_values_i+2,read_values_i+3);
//i[2]=month, i[3]=day
if (ret!=RETURN_FAILURE)
ret=double_hours_to_int_hours(read_values[0],read_values[1],read_values+2,read_values_i+4,read_values_i+5);
//i[4]=HH, i[5]=MM
    if (ret!=RETURN_FAILURE)
    {
    read_values_i[0]=force_four_digit_year(read_values_i[0]);
    *value=cal_and_time_to_jul(read_values_i[0],read_values_i[2],read_values_i[3],read_values_i[4],read_values_i[5],read_values[2]*1.0+fsecond);
    }
}
break;
}

if (s[npos]!='\0' && ret!=RETURN_FAILURE)
{
int alphacount=0;
len=strlen(s+npos);
    for (int i=0;i<len;i++)
    {
    if (isprint(s[npos+i])) alphacount++;
    }
    /*if (format>=0)
qDebug() << fmt_option_items[format].label << "Rest beim Lesen:" << s+npos;
    else
qDebug() << "INVALID" << "Rest beim Lesen!" << s+npos;*/
    *endpos=npos;
    *emptyresidual=(alphacount==0?1:0);
}
else
{
*endpos=strlen(s);
*emptyresidual=1;
}

lock_dirtystate(FALSE);
/*if (ret==RETURN_FAILURE && format>=0 && format<NUM_FMT_OPTION_ITEMS)
{
sprintf(dummy,"Token '%s' does not fit input format '%s'.",s,fmt_option_items[format].label);
errmsg(dummy);
}*/
return ret;
}

#ifdef __cplusplus
}
#endif

double get_block_data_set_double_value(struct block_input_set & inp,int row,int col)
{
if (inp.nr_of_rows<=row || inp.nr_of_columns<=col || row<0 || col<0 || inp.read_results==NULL)
return 0.0;
else
return inp.read_results[inp.nr_of_columns*row+col];
}

int get_block_data_set_double_column(struct block_input_set & inp,int col,double ** data)//returns the column in data and the number of rows as return value
{
    if (*data!=NULL)
    {
    delete[] *data;
    *data=NULL;
    }
if (inp.nr_of_columns<=col || inp.nr_of_rows<=0 || col<0 || inp.read_results==NULL) return 0;
*data=new double[inp.nr_of_rows];
for (int i=0;i<inp.nr_of_rows;i++)
{
//qDebug() << inp.read_results[inp.nr_of_columns*i+col];
(*data)[i]=inp.read_results[inp.nr_of_columns*i+col];
}
return inp.nr_of_rows;
}

void add_block_data_set_double_row(struct block_input_set & inp,int n,double * vals)
{
append_to_double_storage(&(inp.data_allocated),&(inp.read_results),n,vals);
}

void clear_block_input_data(struct block_input_format * inp)//removes all space/memory from inp
{
    if (inp->set_infos!=NULL)
    {
        for (int i=0;i<inp->nr_of_sets;i++)
        {
        clear_block_input_set_data(inp->set_infos+i);
        }
    delete[] inp->set_infos;
    }
    if (inp->filename!=NULL) delete[] inp->filename;
    init_block_input_data(inp);
}

void clear_block_input_set_data(struct block_input_set * inp)
{
//qDebug() << "Clearing A";
    if (inp->read_results!=NULL)
    {
    delete[] inp->read_results;//simply delete the whole block
    inp->read_results=NULL;
    }
//qDebug() << "Clearing B";
    if (inp->read_strings!=NULL)
    {
        for (int i=0;i<inp->data_allocated;i++)
        {
        if (inp->read_strings[i]!=NULL) delete[] inp->read_strings[i];
        }
    delete[] inp->read_strings;
    inp->read_strings=NULL;
    }
//qDebug() << "Clearing C";
    if (inp->column_format!=NULL)
    {
    delete[] inp->column_format;
    inp->column_format=NULL;
    }
//qDebug() << "Clearing D";
    init_block_input_set_data(inp);
//qDebug() << "Clearing E";
}

void init_block_input_data(struct block_input_format * inp)//initializes inp for further use
{
inp->filename=NULL;
inp->nr_of_sets=inp->nr_of_errors=0;
inp->read_from_set=-1;//none --> when data is present, this value will be set to the first set by default
inp->set_infos=NULL;//no data so far
}

void init_block_input_set_data(struct block_input_set * inp)
{
inp->nr_of_columns=inp->nr_of_rows=0;
inp->start_row=-1;//invalid row
inp->column_format=NULL;
inp->data_allocated=0;
inp->read_results=NULL;//results are double values
inp->read_strings=NULL;//strings are used if the format can not be read, usually only NULL is stored here if a double value is present
inp->read_results_text.clear();
}

void add_single_set_block(struct block_input_format * inp,struct block_input_set * set)//increases the storage space in inp and fits set into it
{
struct block_input_set * old_sets;//just a pointer --> do not allocate data here, this is just for remembering an address
//qDebug() << "ADDING SINGLE SET BLOCK";
/*
if (inp->nr_of_sets>=0 || inp->set_infos==NULL)
{
    qDebug() << "nr_of_sets so far=" << inp->nr_of_sets;
    if (inp->set_infos==NULL)
    qDebug() << "set_infos==NULL";
    else
    qDebug() << "set_infos has data!!??";
}*/

    if (inp->nr_of_sets>0 && inp->set_infos!=NULL)
    {
//qDebug() << "add_single_set_block: ADD BLOCK" << inp->nr_of_sets;
    old_sets=inp->set_infos;//remember old set-address
    inp->set_infos=new struct block_input_set[inp->nr_of_sets+1];
        for (int i=0;i<inp->nr_of_sets;i++)
        {
        init_block_input_set_data(inp->set_infos+i);//to initialize the new memory
        copy_set_block(inp->set_infos+i,old_sets[i]);
        clear_block_input_set_data(old_sets+i);//prepare old memory for deleting
        }
    delete[] old_sets;//free old memory
    init_block_input_set_data(inp->set_infos+inp->nr_of_sets);//initialize the one new entry
    }
    else//a new (single) entry
    {
//qDebug() << "add_single_set_block: FIRST BLOCK";
    inp->set_infos=new struct block_input_set[1];
    init_block_input_set_data(inp->set_infos);
    }
copy_set_block(inp->set_infos+inp->nr_of_sets,*set);
inp->nr_of_sets++;
//qDebug() << "add_single_set_block: FINISHED ADDING, set-count:" << inp->nr_of_sets;
}

void copy_set_block(struct block_input_set * target,struct block_input_set & origin)
{
target->nr_of_columns=origin.nr_of_columns;
target->nr_of_rows=origin.nr_of_rows;
target->start_row=origin.start_row;
target->data_allocated=origin.data_allocated;
if (target->nr_of_columns>0)
{
target->column_format=new int[target->nr_of_columns];
memcpy(target->column_format,origin.column_format,sizeof(int)*target->nr_of_columns);
    if (target->data_allocated>0)
    {
    target->read_results=new double[target->data_allocated];
    memcpy(target->read_results,origin.read_results,sizeof(double)*target->data_allocated);
    //memcpy(target->read_strings,origin.read_strings,sizeof(char*)*target->data_allocated);
        if (origin.read_strings==NULL)
        {
        target->read_strings=NULL;
        }
        else
        {
        target->read_strings=new char *[target->data_allocated];
        for (int i=0;i<target->data_allocated;i++)
        {
            if (origin.read_strings[i]!=NULL)
            {
            target->read_strings[i]=new char[2+strlen(origin.read_strings[i])];
            strcpy(target->read_strings[i],origin.read_strings[i]);
            }
            else
            {
            target->read_strings[i]=NULL;
            }
        }
        }
    }
    else
    {
    target->read_results=NULL;
    target->read_strings=NULL;
    }
}
else//no columns in origin-set --> no data at all
{
target->column_format=NULL;
target->read_results=NULL;
target->read_strings=NULL;
}
target->read_results_text=origin.read_results_text;
}

//line_type=-1: invalid, line_type=0: regular line, line_type=1: comment, line_type=2: empty line
int read_line_and_split_in_tokens(ifstream & ifi,char *** tokens,int * token_cnt,int & line_type,char dec_sep)
{
static char buffer[1024],buffer2[1024];
static int len,t_len,len2,tokencount,*startpos=NULL,*endpos=NULL,status,quo_counter=0,comment_start=0;
static QString tmp_str;
*token_cnt=0;
startpos=endpos=NULL;
quo_counter=0;
comment_start=-1;//if there is a comment at the end of a line the comment becomes the last token
line_type=-1;//invalid line
//qDebug() << "Decimal Separator=" << dec_sep;
if (ifi.getline(buffer,1024))
{
len=strlen(buffer);
t_len=0;
    for (int i=0;i<len;i++)
    {
        if (isprint(buffer[i])==0 && isspace(buffer[i])==0) continue;
        buffer2[t_len++]=buffer[i];
    }
    buffer2[t_len++]='\0';
strcpy(buffer,buffer2);
len=strlen(buffer);
//qDebug() << "READ:" << len << "Characters, buffer=" << buffer;
quo_counter=0;
for (int i=0;i<len;i++)
{
    if (buffer[i]=='\"')
    {
    quo_counter=!quo_counter;
    }
    else if (quo_counter==0 && buffer[i]=='#')
    {
    comment_start=i;//save the position of the comment
    buffer[i]='\0';//set this as end --> this removes everything after # in a line for the rest of this function
    len=strlen(buffer);//new length
    break;
    }
}
//qDebug() << "#" << buffer << "# (" << len << ")";
startpos=new int[2+len];
endpos=new int[2+len];
tokencount=0;
status=0;//0=nothing found, 1=inside a token, 2=" found, waiting for second"
for (int i=0;i<=len;i++)//search for the different token-positions
{
    if ((isspace(buffer[i]) || buffer[i]=='\0') && (status==1))//we found a whitespace after a token
    {
    endpos[tokencount]=i-1;
    tokencount++;
    status=0;
    }
    else if (!isspace(buffer[i]) && status==0)//we found nothing yet, but the new character is the start of the token
    {
        if (buffer[i]=='#' || (buffer[i]=='\0' && comment_start>=0))//the token starts with a '#' --> forget the rest
        {
            if (tokencount==0)
            line_type=1;//comment-line because there are no other tokens
        break;
        }
        else if (buffer[i]=='\"')
        {//start new token inside "
        startpos[tokencount]=endpos[tokencount]=i;
        status=2;
        }
        else
        {//start new token
        startpos[tokencount]=endpos[tokencount]=i;
        status=1;
        }
    }
    else if (buffer[i]=='\"' && status==2)//we started with a " and were looking for another one and found it
    {
        endpos[tokencount]=i;
        startpos[tokencount]++;
        endpos[tokencount]--;
        tokencount++;
        status=0;
    }
}
//qDebug() << "first: count=" << tokencount << "line_type=" << line_type << "comment_start=" << comment_start;
if (line_type==-1 && tokencount==0 && comment_start>=0) line_type=1;//comment line
else if (line_type==-1 && tokencount==0) line_type=2;//empty line
else if (line_type==-1 && tokencount>0) line_type=0;//normal data line
//qDebug() << "tokens read:" << tokencount << "buffer=" << buffer << "Start/Stop:";
*token_cnt=tokencount;
(*tokens)=new char*[tokencount+1];//always one more than the found tokens
if (comment_start>=0)//copy comment into last token
{
    (*tokens)[tokencount]=new char[2+strlen(buffer+comment_start+1)];
    (*tokens)[tokencount][0]='#';
    strcpy( ((*tokens)[tokencount])+1 ,buffer+comment_start+1);
}
else//no comments
{
    (*tokens)[tokencount]=new char[2];
    (*tokens)[tokencount][0]='\0';
}
//copy the tokens separately in the array
if (tokencount>0)
{
    for (int i=0;i<tokencount;i++)
    {
    len2=endpos[i]-startpos[i]+1;
        if (len2>0)
        {
        //qDebug() << startpos[i] << "-" << endpos[i];
        (*tokens)[i]=new char[len2+1];
        strncpy((*tokens)[i],buffer+startpos[i],len2);
        (*tokens)[i][len2]='\0';
        //qDebug() << "Token=#" << (*tokens)[i] << "#";
        }
        else
        {
        //qDebug() << "EMPTY TOKEN";//somehow empty!?
        (*tokens)[i]=new char[2];
        (*tokens)[i][0]='\0';
        }
        //qDebug() << "Token=#" << (*tokens)[i] << "#";
        if (dec_sep!='.')
        {
        tmp_str=QString::fromLocal8Bit((*tokens)[i]);
        tmp_str.replace(dec_sep,'.');
        strcpy((*tokens)[i],tmp_str.toLocal8Bit().constData());
        }
    }
    /*if (comment_start>=0)//there is a comment at the end
    {
        buffer[comment_start]='#';
        len2=strlen(buffer+comment_start);
        (*tokens)[tokencount]=new char[len2+1];
        strcpy((*tokens)[tokencount],buffer+comment_start);
        //(*token_cnt)++;//a comment is not a real token
    }
    else
    {
    (*tokens)[tokencount]=new char[2];
    (*tokens)[tokencount][0]='\0';//just for safety
    }*/
}
//we coppied the comment already / the line-type is returned directely
/*else//tokencount==0 --> special line
{
    if (line_type==1)
    {
    strcpy((*tokens)[0],QObject::tr("comment line=ignored").toLocal8Bit().constData());
        if (comment_start>=0)
        {
        buffer[comment_start]='#';
        len2=strlen(buffer+comment_start);
        tokencount=2;//one token with a comment and the first token just says comment line (we have a special case here: comment_line and the comment itself)
        (*tokens)[1]=new char[len2+1];
        strcpy((*tokens)[1],buffer+comment_start);
        }
    }
    else if (line_type==2)
    {
    strcpy((*tokens)[0],QObject::tr("empty line=end of set").toLocal8Bit().constData());
    (*tokens)[1]=new char[2];
    (*tokens)[1][0]='\0';//just for safety
    }
    else
    {
    strcpy((*tokens)[0],QObject::tr("invalid line").toLocal8Bit().constData());
    (*tokens)[1]=new char[2];
    (*tokens)[1][0]='\0';//just for safety
    }
}*/
    if (startpos!=NULL) delete[] startpos;
    if (endpos!=NULL) delete[] endpos;
return RETURN_SUCCESS;
}
else
{//no line read into buffer
//qDebug() << endl << "----ERROR----" << endl;
    *token_cnt=0;
    (*tokens)=new char*[1];//we have to allocate at least one token
    (*tokens)[0]=new char[2];//just allocate a few bytes
    (*tokens)[0][0]='\0';//set contents to zero
    line_type=-1;//could not read from ifstream --> invalid line
    if (startpos!=NULL) delete[] startpos;
    if (endpos!=NULL) delete[] endpos;
return RETURN_FAILURE;
}
}

int read_block_data_from_file(char * filename,struct block_input_format & block,int max_lines,char dec_sep)//just read the data according to the format set in block (returns Success, if no errors occur, otherwise the number of errors)
{
int error_count=0;
int sav_sets=block.nr_of_sets;
int sav_read_from_set=block.read_from_set;
int * nr_of_rows,*nr_or_cols;
int ** sav_column_formats;
int lines_read=0;
char ** tokens=NULL;
int token_cnt=0,*formats_guessed=NULL,tmp_alloc,line_type;
double *col_dat_val=NULL;
char temp_value[32];
struct block_input_set temp_inp_set;
bool dotdotdot_shown=false;
int read_ok=RETURN_FAILURE,endpos,emptyresidual;
int * read_ok_cols=NULL;
QRgb good_color[2],bad_color;
good_color[0]=qRgb(90,90,90);
good_color[1]=qRgb(0,0,0);
bad_color=qRgb(255,0,0);

init_block_input_set_data(&temp_inp_set);

nr_of_rows=new int[2+sav_sets];
nr_or_cols=new int[2+sav_sets];
sav_column_formats=new int*[2+sav_sets];

//qDebug() << "SAVED-FORMATS for loading";
    for (int i=0;i<sav_sets;i++)
    {
    sav_column_formats[i]=new int[block.set_infos[i].nr_of_columns];
        for (int j=0;j<block.set_infos[i].nr_of_columns;j++)
        {
        sav_column_formats[i][j]=block.set_infos[i].column_format[j];
        qDebug() << "Set" << i << "Column" << j << "Format=" << sav_column_formats[i][j];
        }
    nr_of_rows[i]=block.set_infos[i].nr_of_rows;
    nr_or_cols[i]=block.set_infos[i].nr_of_columns;
    }
//qDebug() << "END SAVED-FORMATS for loading";

clear_block_input_data(&block);//we clear the old memory
block.nr_of_errors=0;

ifstream ifi;
ifi.open(filename);
if (block.filename!=NULL) delete[] block.filename;
block.filename=new char[2+strlen(filename)];
strcpy(block.filename,filename);

block.nr_of_sets=0;
temp_inp_set.read_results_text.clear();
temp_inp_set.read_results_text+=QString("<table>");
while (read_line_and_split_in_tokens(ifi,&tokens,&token_cnt,line_type,dec_sep)==RETURN_SUCCESS)
{
//qDebug() << "Read-Result: Count=" << token_cnt;
//line_type =-1: invalid
//line_type = 0: regular line
//line_type = 1: comment
//line_type = 2: empty line
lines_read++;
    if (token_cnt<=0)//unusual line
    {
        if (line_type==1)//tokens[0][0]=='c')//comment line
        {
        ;/// qDebug() << "Comment Line A";
        }
        else if (line_type==2)//tokens[0][0]=='e')//empty line
        {
        /// qDebug() << "Empty Line";
            if (temp_inp_set.nr_of_rows>0)
            {
            /// qDebug() << "we create a new set, because: empty-line after regular lines";
            add_single_set_block(&block,&temp_inp_set);
            clear_block_input_set_data(&temp_inp_set);
            }
        dotdotdot_shown=false;
        }
        else//invalid line
        {
        ;///qDebug() << "Invalid Line";
        }
    }
    else//a usual line width tokens
    {
        if (formats_guessed!=NULL) delete[] formats_guessed;
        if (col_dat_val!=NULL) delete[] col_dat_val;
        formats_guessed=new int[MAX2(token_cnt,temp_inp_set.nr_of_columns)];
        read_ok_cols=new int[MAX2(token_cnt,temp_inp_set.nr_of_columns)];
        col_dat_val=new double[MAX2(token_cnt,temp_inp_set.nr_of_columns)];
        for (int k=0;k<MAX2(token_cnt,temp_inp_set.nr_of_columns);k++)
        {
            if (block.nr_of_sets<sav_sets && k<nr_or_cols[block.nr_of_sets])
            formats_guessed[k]=sav_column_formats[block.nr_of_sets][k];
            else
            formats_guessed[k]=FORMAT_GENERAL;
        //qDebug() << "FORMAT TO USE: k=" << k << "Format=" << formats_guessed[k];
        col_dat_val[k]=0.0;
        read_ok_cols[k]=RETURN_FAILURE;
        }
    //qDebug() << "Columns=" << token_cnt;
        if (temp_inp_set.start_row==-1)//a new set begins here
        {
        temp_inp_set.start_row=lines_read;//the current row-number
        }
        //convert tokens in values and guess the column-format if necessary
            for (int i=0;i<token_cnt;i++)
            {
                if (block.nr_of_sets<sav_sets && i<nr_or_cols[block.nr_of_sets])
                {
                read_ok=read_date_or_number_in_format(tokens[i],formats_guessed[i],TRUE,col_dat_val+i,&endpos,&emptyresidual);
                if (read_ok==RETURN_FAILURE) error_count++;
                }
                else
                {
                formats_guessed[i]=guess_date_or_number(tokens[i],TRUE,col_dat_val+i);
                read_ok=RETURN_SUCCESS;
                }
            read_ok_cols[i]=read_ok;
            //qDebug() << i << ": format=" << formats_guessed[i] << "value=" << col_dat_val[i] << "token=" << tokens[i];
            }
            //qDebug() << "extra token=" << tokens[token_cnt];//this could be a comment
        temp_inp_set.nr_of_rows++;
        //compare formats and store data
        if (temp_inp_set.nr_of_rows==1)/// nothing read so far
        {
        temp_inp_set.nr_of_columns=token_cnt;
        //qDebug() << "set nr of columns=" << temp_inp_set.nr_of_columns;
        temp_inp_set.column_format=new int[token_cnt];
        memcpy(temp_inp_set.column_format,formats_guessed,sizeof(int)*token_cnt);
        temp_inp_set.read_results_text+=QString("<tr><th>Set ")+QString::number(block.nr_of_sets)+QString(":</th></tr>\n")+QString("<tr><th>Row |</th>\n");
        for (int i=0;i<temp_inp_set.nr_of_columns;i++)
        {
        temp_inp_set.read_results_text+=QString("<th>");
            if (formats_guessed[i]>=0)
            temp_inp_set.read_results_text+=QString(fmt_option_items[formats_guessed[i]].label);
            else
            temp_inp_set.read_results_text+=QString("InvalidFormat");
        if (i<temp_inp_set.nr_of_columns-1) temp_inp_set.read_results_text+=QString(" |");
        temp_inp_set.read_results_text+=QString("</th>\n");//QString("\t");
        }
        temp_inp_set.read_results_text+=QString("<th> </th>");//an empty column for the comments
        temp_inp_set.read_results_text+=QString("</tr>\n");
            temp_inp_set.read_results_text+=QString("<tr>\n<td>")+QString::number(temp_inp_set.nr_of_rows-1)+QString("</td>\n");
//qDebug() << "temp_inp_set.nr_of_columns=" << temp_inp_set.nr_of_columns << "token_cnt=" << token_cnt;
            tmp_alloc=temp_inp_set.data_allocated;
            append_to_double_storage(&(temp_inp_set.data_allocated),&(temp_inp_set.read_results),temp_inp_set.nr_of_columns,col_dat_val);
            temp_inp_set.data_allocated=tmp_alloc;
            append_to_char_storage(&(temp_inp_set.data_allocated),&(temp_inp_set.read_strings),temp_inp_set.nr_of_columns,tokens);
                for (int i=0;i<token_cnt;i++)
                {
                sprintf(temp_value,"%.4g\t",col_dat_val[i]);
                //temp_inp_set.read_results_text+=QString("<td>")+ColorToHtml(good_color[i%2])+QString(temp_value)+ColorHtmlEnd()+QString("</td>\n");
                    if (read_ok_cols[i]==RETURN_FAILURE)//(temp_inp_set.column_format[j]!=formats_guessed[j])
                    temp_inp_set.read_results_text+=QString("<td>")+ColorToHtml(bad_color)+QString(temp_value)+ColorHtmlEnd()+QString("</td>\n");
                    else
                    temp_inp_set.read_results_text+=QString("<td>")+ColorToHtml(good_color[i%2])+QString(temp_value)+ColorHtmlEnd()+QString("</td>\n");
                }
            temp_inp_set.read_results_text+=QString("<td> </td>\n");
            temp_inp_set.read_results_text+=QString("</tr>\n");
        }
        else/// compare (we have read formats already)
        {
            if (temp_inp_set.nr_of_columns!=token_cnt)/// column-count mismatch
            {
            error_count++;
            }
            else//exactely the same amount of columns as read before
            {
                for (int i=0;i<token_cnt;i++)
                {
                    if (temp_inp_set.column_format[i]!=formats_guessed[i])
                    {
                        if (temp_inp_set.column_format[i]==FORMAT_GENERAL)
                        {
                        temp_inp_set.column_format[i]=formats_guessed[i];
                        }
                        else if (formats_guessed[i]==FORMAT_GENERAL)
                        {
                        ;//do nothing, format guessed so far should be ok (hopefully)
                        }
                        else//total missmatch -> error
                        {
                        error_count++;
                        }
                    }
                }
                if (temp_inp_set.nr_of_rows>max_lines && dotdotdot_shown==false)
                {
                temp_inp_set.read_results_text+=QString("<tr>\n");
                temp_inp_set.read_results_text+=QString("<td>...</td>\n");
                temp_inp_set.read_results_text+=QString("</tr>\n");
                dotdotdot_shown=true;
                }
                else if (temp_inp_set.nr_of_rows>max_lines && dotdotdot_shown==true)
                {
                ;//nothing to show
                }
                else
                {
                temp_inp_set.read_results_text+=QString("<tr>\n");
                temp_inp_set.read_results_text+=QString("<td>")+QString::number(temp_inp_set.nr_of_rows-1)+QString("</td>\n");
                }
//qDebug() << "temp_inp_set.nr_of_columns=" << temp_inp_set.nr_of_columns << "token_cnt=" << token_cnt;
                tmp_alloc=temp_inp_set.data_allocated;
                append_to_double_storage(&(temp_inp_set.data_allocated),&(temp_inp_set.read_results),temp_inp_set.nr_of_columns,col_dat_val);
                temp_inp_set.data_allocated=tmp_alloc;
                append_to_char_storage(&(temp_inp_set.data_allocated),&(temp_inp_set.read_strings),temp_inp_set.nr_of_columns,tokens);
                if (temp_inp_set.nr_of_rows<=max_lines)
                {
                for (int j=0;j<temp_inp_set.nr_of_columns;j++)
                {
                sprintf(temp_value,"%.4g\t",col_dat_val[j]);
                    if (read_ok_cols[j]==RETURN_FAILURE)//(temp_inp_set.column_format[j]!=formats_guessed[j])
                    temp_inp_set.read_results_text+=QString("<td>")+ColorToHtml(bad_color)+QString(temp_value)+ColorHtmlEnd()+QString("</td>\n");
                    else
                    temp_inp_set.read_results_text+=QString("<td>")+ColorToHtml(good_color[j%2])+QString(temp_value)+ColorHtmlEnd()+QString("</td>\n");
                }
                temp_inp_set.read_results_text+=QString("<td> </td>\n");
                temp_inp_set.read_results_text+=QString("</tr>\n");
                }/// end temp_inp_set.nr_of_rows<=max_lines
            }
        }/// end format comparison
    }//end usual line
    if (tokens!=NULL)
    {
        for (int s=0;s<=token_cnt;s++) delete[] tokens[s];
    delete[] tokens;
    tokens=NULL;
    }
}
//qDebug() << "Lines Read=" << lines_read;
temp_inp_set.read_results_text+=QString("</table>\n");
    if (temp_inp_set.nr_of_rows>0 && temp_inp_set.nr_of_columns>0)// && block.nr_of_sets==0)//there is still unsaved data present
    {
    add_single_set_block(&block,&temp_inp_set);
    clear_block_input_set_data(&temp_inp_set);
    }
if (formats_guessed!=NULL) delete[] formats_guessed;
if (col_dat_val!=NULL) delete[] col_dat_val;
//qDebug() << "Sets=" << block.nr_of_sets;

ifi.close();

block.nr_of_errors=error_count;
block.read_from_set=sav_read_from_set;

return error_count;
}

//this function reads the file and tries to find the number of sets, number of rows and number of columns in each set and tries to guess the column-formats
int guess_block_data_column_format(const char * filename,struct block_input_format & block,int max_lines,char dec_sep)//reads the data from the complete file, guessing the column formats, shows only the first max_lines lines per set in the html-Result
{
int error_count=0,lines_read=0;
char ** tokens=NULL;
int token_cnt=0,*formats_guessed=NULL,tmp_alloc,line_type;
double * col_dat_val=NULL;
char temp_value[32];
struct block_input_set temp_inp_set;
bool dotdotdot_shown=false;//,end_table=false;
QRgb good_color[2],bad_color;
good_color[0]=qRgb(90,90,90);
good_color[1]=qRgb(0,0,0);
bad_color=qRgb(255,0,0);
//clear memory
init_block_input_set_data(&temp_inp_set);
clear_block_input_data(&block);

ifstream ifi;
ifi.open(filename);
block.nr_of_sets=0;
/*temp_inp_set.data_allocated=DELTASIZE;//we initialize some memory
temp_inp_set.read_results=new double[temp_inp_set.data_allocated];
temp_inp_set.read_strings=new char*[temp_inp_set.data_allocated];
for (int i=0;i<temp_inp_set.data_allocated;i++) temp_inp_set.read_strings[i]=NULL;*/
temp_inp_set.read_results_text.clear();
temp_inp_set.read_results_text+=QString("<table>");
while (read_line_and_split_in_tokens(ifi,&tokens,&token_cnt,line_type,dec_sep)==RETURN_SUCCESS)
{
//qDebug() << "Read-Result: Count=" << token_cnt;
//line_type =-1: invalid
//line_type = 0: regular line
//line_type = 1: comment
//line_type = 2: empty line
lines_read++;
    if (token_cnt<=0)//unusual line
    {
        if (line_type==1)//tokens[0][0]=='c')//comment line
        {
        ;/// qDebug() << "Comment Line A";
        }
        else if (line_type==2)//tokens[0][0]=='e')//empty line
        {
        //qDebug() << "Empty Line";
            if (temp_inp_set.nr_of_rows>0)//empty line after we already have loaded data --> store data as a new set
            {
            //qDebug() << "we create a new set, because: empty-line after regular lines";
            add_single_set_block(&block,&temp_inp_set);
            clear_block_input_set_data(&temp_inp_set);
            }
            /*else
            {
            qDebug() << "no data present --> just ignore empty line";
            }*/
        dotdotdot_shown=false;
        }
        else//invalid line
        {
        ;/// qDebug() << "Invalid Line";
        }
    }
    /*else if (token_cnt==2 && tokens[1][0]=='#')
    {
    qDebug() << "Comment Line B";
    }*/
    else//a usual line width tokens
    {
        if (formats_guessed!=NULL) delete[] formats_guessed;
        if (col_dat_val!=NULL) delete[] col_dat_val;
        formats_guessed=new int[MAX2(token_cnt,temp_inp_set.nr_of_columns)];
        col_dat_val=new double[MAX2(token_cnt,temp_inp_set.nr_of_columns)];
        for (int k=0;k<MAX2(token_cnt,temp_inp_set.nr_of_columns);k++)
        {
        formats_guessed[k]=FORMAT_GENERAL;
        col_dat_val[k]=0.0;
        }
    //qDebug() << "Columns=" << token_cnt;
        if (temp_inp_set.start_row==-1)//a new set begins here
        {
        temp_inp_set.start_row=lines_read;//the current row-number
        }
        //convert tokens in values and guess the column-format
            for (int i=0;i<token_cnt;i++)
            {
            formats_guessed[i]=guess_date_or_number(tokens[i],TRUE,col_dat_val+i);
//qDebug() << i << ": format=" << formats_guessed[i] << "value=" << col_dat_val[i] << "token=" << tokens[i];
//for (int ijk=0;ijk<strlen(tokens[i]);ijk++) qDebug() << "Char=" << tokens[i][ijk] << "Printable=" << isprint(tokens[i][ijk]) << "IsSpace=" << isspace(tokens[i][ijk]);
            }
//qDebug() << "extra token=" << tokens[token_cnt];//this could be a comment

        temp_inp_set.nr_of_rows++;

        //compare formats and store data
        if (temp_inp_set.nr_of_rows==1)/// nothing read so far
        {
        temp_inp_set.nr_of_columns=token_cnt;
        //qDebug() << "set nr of columns=" << temp_inp_set.nr_of_columns;
        temp_inp_set.column_format=new int[token_cnt];
        memcpy(temp_inp_set.column_format,formats_guessed,sizeof(int)*token_cnt);
        temp_inp_set.read_results_text+=QString("<tr><th>Set ")+QString::number(block.nr_of_sets)+QString(":</th></tr>\n")+QString("<tr><th>Row |</th>\n");
        for (int i=0;i<temp_inp_set.nr_of_columns;i++)
        {
        temp_inp_set.read_results_text+=QString("<th>");
            if (formats_guessed[i]>=0)
            temp_inp_set.read_results_text+=QString(fmt_option_items[formats_guessed[i]].label);
            else
            temp_inp_set.read_results_text+=QString("InvalidFormat");
        if (i<temp_inp_set.nr_of_columns-1) temp_inp_set.read_results_text+=QString(" |");
        temp_inp_set.read_results_text+=QString("</th>\n");//QString("\t");
        }
        temp_inp_set.read_results_text+=QString("<th> </th>");//an empty column for the comments
        temp_inp_set.read_results_text+=QString("</tr>\n");
        //block.read_results_text+=QString("\n");
            /*for (int i=0;i<temp_inp_set.rows_allocated;i++)
            temp_inp_set.read_results[i]=new double[token_cnt];*/
            temp_inp_set.read_results_text+=QString("<tr>\n<td>")+QString::number(temp_inp_set.nr_of_rows-1)+QString("</td>\n");
//qDebug() << "New Row: TokenCount=" << token_cnt << "Column-Count=" << temp_inp_set.nr_of_columns;
            tmp_alloc=temp_inp_set.data_allocated;
            append_to_double_storage(&(temp_inp_set.data_allocated),&(temp_inp_set.read_results),temp_inp_set.nr_of_columns,col_dat_val);
            temp_inp_set.data_allocated=tmp_alloc;
            append_to_char_storage(&(temp_inp_set.data_allocated),&(temp_inp_set.read_strings),temp_inp_set.nr_of_columns,tokens);
                for (int i=0;i<token_cnt;i++)
                {
                //temp_inp_set.read_results[0][i]=col_dat_val[i];
                sprintf(temp_value,"%.4g\t",col_dat_val[i]);
                temp_inp_set.read_results_text+=QString("<td>")+ColorToHtml(good_color[i%2])+QString(temp_value)+ColorHtmlEnd()+QString("</td>\n");
                }
            /*if (tokens[token_cnt][0]=='#')//there is a comment present -- we show no comments here, just data
            {
            temp_inp_set.read_results_text+=QString("<td>")+ColorToHtml(comment_color)+QString(tokens[token_cnt])+ColorHtmlEnd()+QString("</td>\n");
            }
            else
            {*/
            temp_inp_set.read_results_text+=QString("<td> </td>\n");
            //}
            temp_inp_set.read_results_text+=QString("</tr>\n");
        }
        else/// compare (we have read formats already)
        {
            if (temp_inp_set.nr_of_columns!=token_cnt)/// column-count mismatch
            {
            error_count++;
            }
            else//exactely the same amount of columns as read before
            {
                for (int i=0;i<token_cnt;i++)
                {
                    if (temp_inp_set.column_format[i]!=formats_guessed[i])
                    {
                        if (temp_inp_set.column_format[i]==FORMAT_GENERAL)
                        {
                        temp_inp_set.column_format[i]=formats_guessed[i];
                        }
                        else if (formats_guessed[i]==FORMAT_GENERAL)
                        {
                        ;//do nothing, format guessed so far should be ok (hopefully)
                        }
                        else//total missmatch -> error
                        {
                        error_count++;
                        }
                    }
                }

                /*if (temp_inp_set.nr_of_rows>temp_inp_set.rows_allocated)//increase storage space
                {
                double ** n_data=new double*[temp_inp_set.rows_allocated+10];
                    for (int i=0;i<temp_inp_set.rows_allocated+10;i++)
                    {
                    n_data[i]=new double[temp_inp_set.nr_of_columns];
                        if (i<temp_inp_set.rows_allocated)
                            for (int j=0;j<temp_inp_set.nr_of_columns;j++)
                            n_data[i][j]=temp_inp_set.read_results[i][j];
                    }
                for (int i=0;i<temp_inp_set.rows_allocated;i++) delete[] temp_inp_set.read_results[i];
                delete[] temp_inp_set.read_results;
                temp_inp_set.rows_allocated+=10;
                temp_inp_set.read_results=n_data;
                }*/
                if (temp_inp_set.nr_of_rows>max_lines && dotdotdot_shown==false)
                {
                temp_inp_set.read_results_text+=QString("<tr>\n");
                temp_inp_set.read_results_text+=QString("<td>...</td>\n");
                temp_inp_set.read_results_text+=QString("</tr>\n");
                dotdotdot_shown=true;
                }
                else if (temp_inp_set.nr_of_rows>max_lines && dotdotdot_shown==true)
                {
                ;//nothing to show
                }
                else
                {
                temp_inp_set.read_results_text+=QString("<tr>\n");
                temp_inp_set.read_results_text+=QString("<td>")+QString::number(temp_inp_set.nr_of_rows-1)+QString("</td>\n");
                }
                tmp_alloc=temp_inp_set.data_allocated;
                append_to_double_storage(&(temp_inp_set.data_allocated),&(temp_inp_set.read_results),temp_inp_set.nr_of_columns,col_dat_val);
//qDebug() << "ADD Row: TokenCount=" << token_cnt << "Column-Count=" << temp_inp_set.nr_of_columns;
                temp_inp_set.data_allocated=tmp_alloc;
                append_to_char_storage(&(temp_inp_set.data_allocated),&(temp_inp_set.read_strings),temp_inp_set.nr_of_columns,tokens);
                if (temp_inp_set.nr_of_rows<=max_lines)
                {
                for (int j=0;j<temp_inp_set.nr_of_columns;j++)
                {
                /*temp_inp_set.read_results[temp_inp_set.nr_of_rows-1][j]=col_dat_val[j];*/
                sprintf(temp_value,"%.4g\t",col_dat_val[j]);
                    if (temp_inp_set.column_format[j]!=formats_guessed[j])
                    temp_inp_set.read_results_text+=QString("<td>")+ColorToHtml(bad_color)+QString(temp_value)+ColorHtmlEnd()+QString("</td>\n");
                    else
                    temp_inp_set.read_results_text+=QString("<td>")+ColorToHtml(good_color[j%2])+QString(temp_value)+ColorHtmlEnd()+QString("</td>\n");
                }
                /*if (tokens[token_cnt][0]=='#')//we do not show comments here (even, if we could)
                {
                temp_inp_set.read_results_text+=QString("<td>")+ColorToHtml(comment_color)+QString(tokens[token_cnt])+ColorHtmlEnd()+QString("</td>\n");
                }
                else
                {*/
                temp_inp_set.read_results_text+=QString("<td> </td>\n");
                /*}*/
                temp_inp_set.read_results_text+=QString("</tr>\n");
                //block.read_results_text+=QString("\n");
                }/// end temp_inp_set.nr_of_rows<=max_lines
            }/// end: column-count matches
        }/// end format comparison
/// we read everything, we just do not add the result to the read_results_text
/// if (max_lines>0 && lines_read>=max_lines) break;//stop reading
    }//end usual line
    if (tokens!=NULL)//free the memory again for the next line
    {
        for (int s=0;s<=token_cnt;s++)
        delete[] tokens[s];
    delete[] tokens;
    tokens=NULL;
    }
}
//qDebug() << "Lines Read=" << lines_read;
temp_inp_set.read_results_text+=QString("</table>\n");
    if (temp_inp_set.nr_of_rows>0 && temp_inp_set.nr_of_columns>0)// && block.nr_of_sets==0)//there is still unsaved data present
    {
    add_single_set_block(&block,&temp_inp_set);
    clear_block_input_set_data(&temp_inp_set);
    }
if (formats_guessed!=NULL) delete[] formats_guessed;
if (col_dat_val!=NULL) delete[] col_dat_val;
//qDebug() << "Sets=" << block.nr_of_sets;
ifi.close();
return error_count;
}

int simplified_format(int complete_format)
{
int ret=FFORMAT_NUMBER;
switch(complete_format)
{
default:
case FORMAT_STRING:
case FORMAT_INVALID:
ret=FFORMAT_STRING;
break;
case FORMAT_DECIMAL:
case FORMAT_EXPONENTIAL:
case FORMAT_GENERAL:
case FORMAT_POWER:
case FORMAT_SCIENTIFIC:
case FORMAT_ENGINEERING:
case FORMAT_COMPUTING:
case FORMAT_DEGREESLON:
case FORMAT_DEGREESMMLON:
case FORMAT_DEGREESMMSSLON:
case FORMAT_MMSSLON:
case FORMAT_DEGREESLAT:
case FORMAT_DEGREESMMLAT:
case FORMAT_DEGREESMMSSLAT:
case FORMAT_MMSSLAT:
ret=FFORMAT_NUMBER;
break;
case FORMAT_DDMMYY:
case FORMAT_MMDDYY:
case FORMAT_YYMMDD:
case FORMAT_MMYY:
case FORMAT_MMDD:
case FORMAT_MONTHDAY:
case FORMAT_DAYMONTH:
case FORMAT_MONTHS:
case FORMAT_MONTHSY:
case FORMAT_MONTHL:
case FORMAT_DAYOFWEEKS:
case FORMAT_DAYOFWEEKL:
case FORMAT_DAYOFYEAR:
case FORMAT_HMS:
case FORMAT_MMDDHMS:
case FORMAT_MMDDYYHMS:
case FORMAT_YYMMDDHMS:
case FORMAT_YYYY:
case FORMAT_YYDYHMS:
ret=FFORMAT_DATE;
break;
}
return ret;
}

void transfer_set_data_to_blockData(struct block_input_set * set)
{
ss_data ssd;
double * d_ptr;
char ** sp;
int * formats=new int[2+set->nr_of_columns];
for (int i=0;i<set->nr_of_columns;i++)
{
formats[i]=simplified_format(set->column_format[i]);
}
init_ss_data(&ssd,set->nr_of_columns,formats);
    for (int i=0;i<set->nr_of_columns;i++)
    {
        if (formats[i]==FFORMAT_STRING)
        {
        ssd.data[i] = (void*)xmalloc(set->nr_of_rows*sizeof(char*));
        sp = (char **)ssd.data[i];
            for (int j=0;j<set->nr_of_rows;j++)
            {
            sp[j]=copy_string(NULL,set->read_strings[j*set->nr_of_columns+i]);
            }
        }
        else
        {
        ssd.data[i] = (void*)xmalloc(set->nr_of_rows*sizeof(double));
        d_ptr = (double*)ssd.data[i];
            for (int j=0;j<set->nr_of_rows;j++)
            {
            d_ptr[j] = set->read_results[j*set->nr_of_columns+i];
            }
        }
    }
ssd.nrows=set->nr_of_rows;
set_blockdata(&ssd);
}

void read_one_number(char * text,int * type,double * value)//tries to read data in any usual format
{
char * next=text;
double tmp_val=strtod(text,&next);
if (next[0]=='\0')
{
*type=FORMAT_GENERAL;
*value=tmp_val;
return;
}
*type=guess_date_or_number(text,TRUE,value);
}

void add_html_table_header(QString & html,int n,QString * headers,int * colors)
{
html+=QString("<table><tr>");
for (int i=0;i<n;i++)
{
html+=QString("<th>");
html+=ColorToHtml(colors[i]);
html+=headers[i];
html+=ColorHtmlEnd();
if (i<n-1)
html+=QString(" |");
html+=QString("</th>");
}
html+=QString("</tr>");
}

void add_html_table_header(QString & html,int n,QString * headers,QRgb * colors)
{
html+=QString("<table><tr>");
for (int i=0;i<n;i++)
{
html+=QString("<th>");
html+=ColorToHtml(qRed(colors[i]),qGreen(colors[i]),qBlue(colors[i]));
html+=headers[i];
html+=ColorHtmlEnd();
if (i<n-1)
html+=QString(" |");
html+=QString("</th>");
}
html+=QString("</tr>");
}

void add_html_table_line(QString & html,int n,QString * entries,int * colors)
{
html+=QString("<tr>");
for (int i=0;i<n;i++)
{
html+=QString("<td>");
html+=ColorToHtml(colors[i]);
html+=entries[i];
html+=ColorHtmlEnd();
html+=QString("</td>");
}
html+=QString("</tr>");
}

void add_html_table_line(QString & html,int n,QString * entries,QRgb * colors)
{
html+=QString("<tr>");
for (int i=0;i<n;i++)
{
html+=QString("<td>");
html+=ColorToHtml(qRed(colors[i]),qGreen(colors[i]),qBlue(colors[i]));
html+=entries[i];
html+=ColorHtmlEnd();
html+=QString("</td>");
}
html+=QString("</tr>");
}

void add_html_table_end(QString & html)
{
html+=QString("</table>\n");
}

void create_html_from_blockdata(QString & html)//just show the contents of the complete block data as a table (in html-format)
{
int rows,cols;
int * formats;
int * colors=NULL;
QRgb * rgb_colors=NULL;
QString * entries=NULL;
char dummy[512];
QString temp_string;
char ** string_col;
double * data_col;
QRgb good_color[2];
good_color[0]=qRgb(90,90,90);
good_color[1]=qRgb(0,0,0);
rows=get_blocknrows();
cols=get_blockncols();
formats=get_blockformats();
colors=new int[cols+1];
rgb_colors=new QRgb[cols+1];
entries=new QString[cols+1];
html.clear();
/// all colors set to black - it seems to look better than multi-color-columns
//write header
entries[0]=QObject::tr("Row");
colors[0]=1;
for (int i=0;i<cols;i++)
{
colors[1+i]=1;
entries[i+1]+=QString::number(i+1)+QString(":");
    if (formats[i]==FFORMAT_STRING)
    entries[1+i]+=QString("String");
    else if (formats[i]==FFORMAT_DATE)
    entries[1+i]+=QString("Date");
    else
    entries[1+i]+=QString("Number");
}
add_html_table_header(html,1+cols,entries,colors);
//add entries (starting with row number at beginning), color alternating between grey and black
for (int j=0;j<rows;j++)
{
colors[0]=1;
rgb_colors[0]=good_color[1];
entries[0]=QString::number(j);
    for (int i=0;i<cols;i++)
    {
        if (i%2)
        {
        colors[1+i]=1;
        rgb_colors[1+i]=good_color[1];
        }
        else
        {
        colors[1+i]=1;
        rgb_colors[1+i]=good_color[0];
        }
            if (formats[i]==FFORMAT_STRING)
            {
            string_col=get_block_data_column_s(i);
            //strcpy(dummy,string_col[j]);
            temp_string=QString::fromLocal8Bit(string_col[j]);
            }
            else if (formats[i]==FFORMAT_DATE)
            {
            data_col=get_blockdata_column_d(i);
            strcpy(dummy,create_fstring(FORMAT_MMDDYYHMS,5,data_col[j],0));
            temp_string=QString(dummy);
            }
            else
            {
            data_col=get_blockdata_column_d(i);
            strcpy(dummy,create_fstring(FORMAT_GENERAL,5,data_col[j],0));
            temp_string=QString(dummy);
            }
    entries[1+i]=temp_string;
    }
//add_html_table_line(html,1+cols,entries,colors);
add_html_table_line(html,1+cols,entries,rgb_colors);
}
//finish
add_html_table_end(html);
delete[] colors;
delete[] entries;
}

#ifdef __cplusplus
extern "C" {
#endif

void redim_int_array_and_preserve(int ** a,int old_n,int new_n)
{
if (*a==NULL) { N_REDIM_INT_ARRAY(a,new_n) }
else
{
int * temp=new int[new_n];
    for (int i=0;i<MIN2(old_n,new_n);i++) temp[i]=(*a)[i];
    for (int i=MIN2(old_n,new_n);i<new_n;i++) temp[i]=0;
delete[] *a;
*a=temp;
}
}

void redim_double_array_and_preserve(double ** a,int old_n,int new_n)
{
if (*a==NULL) { N_REDIM_DOUBLE_ARRAY(a,new_n) }
else
{
double * temp=new double[new_n];
    for (int i=0;i<MIN2(old_n,new_n);i++) temp[i]=(*a)[i];
    for (int i=MIN2(old_n,new_n);i<new_n;i++) temp[i]=0.0;
delete[] *a;
*a=temp;
}
}

#ifdef __cplusplus
}
#endif

QString stringFromSpecialChar(char c)
{
QString ret;
switch (c)
{
case '"':
ret=QString("\"");
break;
case '\n':
ret=QString("\\n");
break;
case '\t':
ret=QString("\\t");
break;
default:
ret=QString(c);
break;
}
return ret;
}

custom_number_format::custom_number_format(void)
{
clear_all_tokens();
}

custom_number_format::~custom_number_format()
{
clear_all_tokens();
}

void custom_number_format::clear_all_tokens(void)
{
tokens.clear();
token_types.clear();
token_precision.clear();
token_texts.clear();
token_enhancer_type.clear();
token_enhancer_list.clear();
decimal_separator=QString(".");
}

void custom_number_format::remove_single_token(int nr)
{
if (nr<0 || tokens.length()<nr) return;//this token does not exist --> just return
token_types.removeAt(nr);
token_precision.removeAt(nr);
token_texts.removeAt(nr);
token_enhancer_type.removeAt(nr);
token_enhancer_list.removeAt(nr);
tokens.removeAt(nr);
}

void custom_number_format::init_with_format_string(QString & str_format)
{

}

void custom_number_format::create_format_string(QString & str_format)
{
str_format.clear();
for (int i=0;i<tokens.length();i++)
{

}
}

int custom_number_format::findEnhancer(QString & text, int pos, int token_nr,int & len,QString & value)
{
int ret=RETURN_FAILURE;
int temp_pos=-1;
QMap<QString, QString>::const_iterator i = token_enhancer_list.at(token_nr).constBegin();
len=0;
value.clear();
  while (i != token_enhancer_list.at(token_nr).constEnd())
  {
  temp_pos=text.indexOf(i.key(),pos);
//qDebug() << "Looking for Enhancer=" << i.key() << ": " << i.value();// << endl;
    if (temp_pos==pos)
    {
    ret=RETURN_SUCCESS;
    len=i.key().length();
    value=i.value();
    break;
    }
  ++i;
  }
return ret;
}

int custom_number_format::isTextHere(QString & text, int pos, int token_nr, int & len)
{
int ret=RETURN_FAILURE;
int temp_pos=-1;
int ret2=findTextPos(text,pos,token_nr,temp_pos,len);
if (ret2==RETURN_SUCCESS && temp_pos==pos)
{
ret=RETURN_SUCCESS;
}
return ret;
}

int custom_number_format::findTextPos(QString & text, int pos, int token_nr,int & found_pos, int & len)
{
int ret=RETURN_FAILURE;
int temp_pos=text.indexOf(token_texts.at(token_nr),pos);
len=0;
found_pos=pos;
    if (temp_pos>=0)
    {
    len=token_texts.at(token_nr).length();
    ret=RETURN_SUCCESS;
    found_pos=temp_pos;
    }
return ret;
}

int custom_number_format::read_numbers_from_text(QString text,double * value)//tries to read a number from text with the format set here; read value is returned in value; funktion returns SUCCESS or FAILURE (depending whether the text fits the format or not); if the text is longer than the format, it is concidered a failure (text has to fit the format exactely, not only by coincidence, whitspaces at the end are allowed)
{
int ret=RETURN_FAILURE;
int year,month,day,hour,minute,sec;
year=month=day=hour=minute=sec=0;
double fracsec;
fracsec=0.0;
int degr,degr_min,degr_sec;
degr=degr_min=degr_sec=0;
int pos=0,endpos=0,temp_pos=0,temp_pos2=0,next_no_number=0;
int len=text.length(),tmp_len,i;
QString substring,enh_value;
//first: find first number-token
for (i=0;i<tokens.length();i++)
{
if (tokens.at(i)==FORMAT_TOKEN_ENHANCEMENT_RIGHT)
{
    qDebug() << "Invalid custom format; wrong type of enhancement given at beginning of format";
    return RETURN_FAILURE;
}
else if (tokens.at(i)==FORMAT_TOKEN_TEXT)
{
temp_pos=isTextHere(text,pos,i,tmp_len);
    if (temp_pos!=RETURN_SUCCESS)
    {
    qDebug() << "Format mismatch; text token not where it should be; text=" << text << "TextToken=" << token_texts.at(i) << "pos=" << pos << "found_pos=" << temp_pos;
    return RETURN_FAILURE;
    }
    else
    {
    pos+=tmp_len;
    }
}
else if (tokens.at(i)==FORMAT_TOKEN_ENHANCEMENT_LEFT)
{
temp_pos=findEnhancer(text,pos,i,tmp_len,enh_value);
    if (temp_pos==RETURN_SUCCESS)/// enhancement-treatment missing -- do not forget the empty enhancement (empty is valid, empty is the default)
    {
    pos+=tmp_len;
    }
}
else
{
break;
}
}
//second: go on reading
for (;i<tokens.length();i++)
{
    if (i==tokens.length())//this is the last token
    {
    endpos=len;
    }
    else//find next token that is not a number
    {
        next_no_number=i;
        for (int j=i+1;j<tokens.length();j++)
        {
            if (tokens.at(j)!=FORMAT_TOKEN_NUMBER)
            {
            next_no_number=j;
            break;
            }
        }
        if (next_no_number==i)
        {
        qDebug() << "only numbers - and more than one - ?";
        return RETURN_FAILURE;
        }
        else if (next_no_number!=i+1)
        {
        qDebug() << "two numbers after one another is not allowed!";
        return RETURN_FAILURE;
        }
        else//we found a token, that is not a number
        {
            if (tokens.at(next_no_number)==FORMAT_TOKEN_TEXT)
            {
                if ((temp_pos=findTextPos(text,pos,i,temp_pos2,tmp_len))!=RETURN_SUCCESS)
                {
                qDebug() << "Format mismatch; text token not where it should be; text=" << text << "TextToken=" << token_texts.at(i) << "pos=" << pos << "found_pos=" << temp_pos;
                return RETURN_FAILURE;
                }
                else
                {
                endpos=temp_pos2;
                }
            }
            else
            {

            }
        }
    }
substring=text.right(len-pos);
qDebug() << "Substring=" << substring;
pos=endpos;
}

return ret;
}

void fillStringWithZeros(QString & str,int length)
{
int len=str.length();
if (len>=length) return;
QString tmp2;
tmp2.clear();
for (int i=0;i<(length-len);i++) tmp2+=QString("0");
str=tmp2+str;
}

void custom_number_format::print_number_in_format(QString & text,double value)//creates a representation of value in the custom format
{
int year,month,day,hour,minute,sec,dayofyear;
double fracsec;
int degr,degr_min,degr_sec;
double tmp;
QString tmp_str;

jul_to_cal_and_time_with_yday(value,-3,&year,&month,&day,&hour,&minute,&sec,&fracsec,&dayofyear);

degr=int(value);
tmp=(value-(double)degr)*60.0;//sub-degrees in minutes
degr_min=(int)tmp;
tmp=(tmp-(double)degr_min)*60.0;//sub-degree-minutes in seconds
degr_sec=(int)tmp;

text.clear();
for (int i=0;i<tokens.length();i++)
{
    if (tokens.at(i)==FORMAT_TOKEN_TEXT)
    {
    text+=token_texts.at(i);
    }
    else if (tokens.at(i)==FORMAT_TOKEN_NUMBER)
    {
//qDebug() << "Token" << i << "Precision=" << format_str;
    switch (token_types.at(i))
    {
    case FORMAT_TOKEN_TYPE_NUMBER:
        tmp_str.setNum(value,'f',token_precision.at(i));
    break;
    case FORMAT_TOKEN_TYPE_DAYOFYEAR:
        tmp_str.setNum(dayofyear);
        fillStringWithZeros(tmp_str,token_precision.at(i));
    break;    
    case FORMAT_TOKEN_TYPE_YEAR:
        tmp_str.setNum(year);
        fillStringWithZeros(tmp_str,token_precision.at(i));
    break;
    case FORMAT_TOKEN_TYPE_MONTH:
        tmp_str.setNum(month+1);
        fillStringWithZeros(tmp_str,token_precision.at(i));
    break;
    case FORMAT_TOKEN_TYPE_DAY:
        tmp_str.setNum(day);
        fillStringWithZeros(tmp_str,token_precision.at(i));
    break;
    case FORMAT_TOKEN_TYPE_HOUR:
        tmp_str.setNum(hour);
        fillStringWithZeros(tmp_str,token_precision.at(i));
    break;
    case FORMAT_TOKEN_TYPE_MINUTE:
        tmp_str.setNum(minute);
        fillStringWithZeros(tmp_str,token_precision.at(i));
    break;
    case FORMAT_TOKEN_TYPE_SECOND:
        tmp_str.setNum(sec);
        fillStringWithZeros(tmp_str,token_precision.at(i));
        text+=tmp_str;
    break;
    case FORMAT_TOKEN_TYPE_FRACSEC:
        /*if (exchange_point_comma==true)
        text+=QString(",");
        else
        text+=QString(".");*/
        tmp_str.setNum((int)rint_v2(fracsec*pow(10.0,token_precision.at(i))));
        fillStringWithZeros(tmp_str,token_precision.at(i));
    break;
    case FORMAT_TOKEN_TYPE_DEGREES:
        tmp_str.setNum(degr);
        fillStringWithZeros(tmp_str,token_precision.at(i));
    break;
    case FORMAT_TOKEN_TYPE_DEGREE_MINUTES:
        tmp_str.setNum(degr_min);
        fillStringWithZeros(tmp_str,token_precision.at(i));
    break;
    case FORMAT_TOKEN_TYPE_DEGREE_SECONDS:
        tmp_str.setNum(degr_sec);
        fillStringWithZeros(tmp_str,token_precision.at(i));
    break;
    }
    text+=tmp_str;
    }
    else//enhancement
    {


    }
}

}

void custom_number_format::add_number_token(int type,int precision)
{
tokens << FORMAT_TOKEN_NUMBER;
token_types << type;
token_precision << precision;
token_texts << QString("");
token_enhancer_type << -1;
QMap<QString, QString> map1;
map1.insert(QString(""),QString(""));
token_enhancer_list << map1;
}

void custom_number_format::add_text_token(QString text)
{
tokens << FORMAT_TOKEN_TEXT;
token_texts << text;
token_types << -1;
token_precision << -1;
token_enhancer_type << -1;
QMap<QString, QString> map1;
map1.insert(QString(""),QString(""));
token_enhancer_list << map1;
}

void custom_number_format::add_enhancer_token(bool left,int type,QMap<QString, QString> map)
{
if (left==true)
tokens << FORMAT_TOKEN_ENHANCEMENT_LEFT;
else
tokens << FORMAT_TOKEN_ENHANCEMENT_RIGHT;
token_enhancer_type << type;
token_enhancer_list << map;
token_texts << QString("");
token_types << -1;
token_precision << -1;
}

showImageSequence::showImageSequence(QWidget * parent):QLabel(parent)
{
timer=new QTimer(this);
connect(timer,SIGNAL(timeout()),SLOT(advance()));
images.clear();
nr_of_images=0;
cur_nr=-1;
}

void showImageSequence::init(char * path,char * file,char * ext)
{
QDir dir(path);
QStringList nameFilters;
nameFilters.clear();
nameFilters << QString(file)+QString("*.")+QString(ext);
images=dir.entryList(nameFilters,QDir::Files | QDir::NoDotAndDotDot,QDir::Name);
}

void showImageSequence::advance(void)
{
if (cur_nr==-1) return;
cur_nr++;
if (cur_nr==nr_of_images) cur_nr=0;
}

void showImageSequence::rewind(void)
{
if (cur_nr==-1) return;
cur_nr--;
if (cur_nr<0) cur_nr=nr_of_images;
}

void showImageSequence::toStart(void)
{
if (nr_of_images>0) cur_nr=0;
else cur_nr=-1;
}

void showImageSequence::Start(int ms)
{
timer->start(ms);
}

void showImageSequence::Stop(void)
{
timer->stop();
}

void showImageSequence::paintEvent(QPaintEvent *event)
{
(void) event;
}

CompressionSelector::CompressionSelector(QWidget * parent):QWidget(parent)
{
int nr=2;
int values[2];
QString entr[2];
entr[0]=tr("No compression");
entr[1]=tr("Qt-internal compression");
values[0]=COMPRESSION_NONE;
values[1]=COMPRESSION_QTINTERNAL;
cmbCompression=new StdSelector(this,tr("File compression:"),nr,entr);
cmbCompression->setValues(values);
cmbCompression->setToolTip(tr("This sets whether (and how) the data in a QtGrace-project file should be compressed by default to save memory.\nThe current setting can be changed in the save-file-dialog.\nWarning: Compression breaks compatibility with original Grace or previous versions of QtGrace."));
selComprLevel=new stdIntSelector(this,tr("Compression level:"),-1,9);
selComprLevel->setToolTip(tr("A level of '-1' means the default value of the compression method.\nHigher values mean higher compression but take more time.\nValues around 2 or 3 seem to be sufficient for project files."));
layout=new QHBoxLayout(this);
//layout->setMargin(0);
layout->setContentsMargins(0,0,0,0);
layout->setSpacing(STD_SPACING);
layout->addWidget(cmbCompression);
layout->addWidget(selComprLevel);
setLayout(layout);
connect(cmbCompression,SIGNAL(currentIndexChanged(int)),SLOT(methodChanged(int)));
}

void CompressionSelector::init(int method,int level)
{
setMethod(method);
setLevel(level);
if (method==0) selComprLevel->setEnabled(false);
else selComprLevel->setEnabled(true);
}

void CompressionSelector::setMethod(int method)
{
cmbCompression->setCurrentValue(method);
}

void CompressionSelector::setLevel(int level)
{
selComprLevel->setValue(level);
}

int CompressionSelector::level(void)
{
return selComprLevel->value();
}

int CompressionSelector::method(void)
{
return cmbCompression->currentValue();
}

void CompressionSelector::methodChanged(int m)
{
    if (m==COMPRESSION_NONE) selComprLevel->setEnabled(false);
    else selComprLevel->setEnabled(true);
}

void CompressionSelector::levelChanged(int l)
{
    (void) l;
}

bailoutQuestion::bailoutQuestion(QWidget * parent):QDialog(parent)
{
lblIcon=new QLabel(QString(""),this);

QPixmap pixm=messageBoxStdIcon(QMessageBox::Question,6*qApp->font().pixelSize()).scaled(6*qApp->font().pixelSize(),6*qApp->font().pixelSize());
lblIcon->setPixmap(pixm);

lblText=new QLabel(QString(""),this);
cmdOk=new QPushButton(tr("OK"),this);
cmdCancel=new QPushButton(tr("Cancel"),this);
cmdSave=new QPushButton(tr("Save"),this);
cmdSave->setToolTip(tr("Save the project using its current file-name (overwriting existing file).\nQtGrace is closed afterwards."));
cmdSaveAs=new QPushButton(tr("Save As..."),this);
cmdSaveAs->setToolTip(tr("Open Save-As-dialog to save project before closing QtGrace.\nIf saving is cancelled, the closing of QtGrace will be cancelled."));
cmdYes=new QPushButton(tr("Yes"),this);
cmdNo=new QPushButton(tr("No"),this);

connect(cmdOk,SIGNAL(clicked()),this,SLOT(doOk()));
connect(cmdCancel,SIGNAL(clicked()),this,SLOT(doCancel()));
connect(cmdSave,SIGNAL(clicked()),this,SLOT(doSave()));
connect(cmdSaveAs,SIGNAL(clicked()),this,SLOT(doSaveAs()));
connect(cmdYes,SIGNAL(clicked()),this,SLOT(doYes()));
connect(cmdNo,SIGNAL(clicked()),this,SLOT(doNo()));

layout=new QGridLayout(this);
layout->setSpacing(2*STD_SPACING);
//layout->setMargin(3*STD_MARGIN);
layout->setContentsMargins(3*STD_MARGIN,3*STD_MARGIN,3*STD_MARGIN,3*STD_MARGIN);

layout->addWidget(lblIcon,0,0,2,1);
layout->addWidget(lblText,0,1,1,6);
layout->addWidget(cmdSave,1,1,1,1);
layout->addWidget(cmdSaveAs,1,2,1,1);
layout->addWidget(cmdYes,1,3,1,1);
layout->addWidget(cmdNo,1,4,1,1);
layout->addWidget(cmdOk,1,5,1,1);
layout->addWidget(cmdCancel,1,6,1,1);

setLayout(layout);
resize(LastSize_Bailout);
}

bailoutQuestion::~bailoutQuestion()
{
LastSize_Bailout=this->size();
}

int bailoutQuestion::askQuestion(QString title,QString text,bool is_exit,bool save_on,bool saveas_on,bool yes_on,bool no_on,bool ok_on,bool cancel_on)
{
setWindowTitle(title);
lblText->setText(text);
//Save / SaveAs / Yes / No / Ok / Cancel
int index=1;
layout->removeWidget(lblText);
layout->removeWidget(cmdSave);
layout->removeWidget(cmdSaveAs);
layout->removeWidget(cmdYes);
layout->removeWidget(cmdNo);
layout->removeWidget(cmdOk);
layout->removeWidget(cmdCancel);
cmdSave->setVisible(save_on);
cmdSaveAs->setVisible(saveas_on);
cmdYes->setVisible(yes_on);
cmdNo->setVisible(no_on);
cmdOk->setVisible(ok_on);
cmdCancel->setVisible(cancel_on);
if (save_on==true)
{
layout->addWidget(cmdSave,1,index++,1,1);
}
if (saveas_on==true)
{
layout->addWidget(cmdSaveAs,1,index++,1,1);
}
if (no_on==true)
{
layout->addWidget(cmdNo,1,index++,1,1);
}
if (yes_on==true)
{
layout->addWidget(cmdYes,1,index++,1,1);
}
if (ok_on==true)
{
layout->addWidget(cmdOk,1,index++,1,1);
}
if (cancel_on==true)
{
layout->addWidget(cmdCancel,1,index++,1,1);
}
layout->addWidget(lblText,0,1,1,index-1);
if (is_exit==true)//really exit QtGrace
{
    /*if (general_behavior==0)//like Grace
    {//exit losing unsaved changes?*/
    cmdYes->setToolTip(tr("Close QtGrace without saving\nDiscard changes"));
    cmdNo->setToolTip(tr("Cancel Close-operation"));
    cmdSave->setToolTip(tr("Save the project using its current file-name (overwriting existing file).\nQtGrace is closed afterwards."));
    cmdSaveAs->setToolTip(tr("Open Save-As-dialog to save project before closing QtGrace.\nIf saving is cancelled, the closing of QtGrace will be cancelled."));
    /*}
    else//QtGrace
    {//Abandon unsaved changes?
    cmdYes->setToolTip(tr(""));
    cmdNo->setToolTip(tr(""));
    }*/
}
else//load different file
{
    /*if (general_behavior==0)//like Grace
    {
    cmdYes->setToolTip(tr(""));
    cmdNo->setToolTip(tr(""));
    }
    else//QtGrace
    {
    cmdYes->setToolTip(tr(""));
    cmdNo->setToolTip(tr(""));
    }*/
    cmdYes->setToolTip(tr("Close current project, open new project\nDiscard changes"));
    //cmdCancel->setToolTip(tr("Cancel Load-operation"));
    cmdNo->setToolTip(tr("Cancel Load-operation"));
    cmdSave->setToolTip(tr("Save the project using its current file-name (overwriting existing file).\nNew project is opened afterwards."));
    cmdSaveAs->setToolTip(tr("Open Save-As-dialog to save project before closing QtGrace.\nIf saving is cancelled, the loading of the new file is cancelled."));
}
if (general_behavior==0)//like Grace
{
cmdYes->setText(tr("Yes"));
cmdNo->setText(tr("No"));
}
else//like QtGrace
{
    if (is_exit)
    cmdYes->setText(tr("Exit"));
    else
    cmdYes->setText(tr("Close"));
cmdNo->setText(tr("Cancel"));
}

setModal(true);
//show();
(void)exec();
return general_return;
}

int bailoutQuestion::askBailout(bool is_exit, bool save_on,bool saveas_on)
{
QString title_string,text_string;
    if (general_behavior==0)//like Grace
    {
        if (is_exit==true)
        {
        title_string=QObject::tr("Exit QtGrace");
        text_string=QObject::tr("Exit losing unsaved changes?");
        }
        else
        {
        title_string=QObject::tr("Close project");
        text_string=QObject::tr("Abandon unsaved changes?");
        }
    }
    else
    {
        if (is_exit==true)
        {
        title_string=QObject::tr("Exit QtGrace");
        }
        else
        {
        title_string=QObject::tr("Close project");
        }
        text_string=QObject::tr("Content of current project changed!\nYou may choose to save the current project now.\nExit without saving or Cancel closing operation?");
    }
return askQuestion(title_string,text_string,is_exit,save_on,saveas_on,true,true,false,false);
}

void bailoutQuestion::doOk(void)
{
general_return=0;
//hide();
accept();
}

void bailoutQuestion::doCancel(void)
{
general_return=1;
//hide();
reject();
}

void bailoutQuestion::doSave(void)
{
general_return=2;
//hide();
accept();
}

void bailoutQuestion::doSaveAs(void)
{
general_return=3;
//hide();
accept();
}

void bailoutQuestion::doYes(void)
{
general_return=0;
//hide();
accept();
}

void bailoutQuestion::doNo(void)
{
general_return=1;
//hide();
reject();
}

int helpPermutate(QList<int> & l,QList<int> & cur_pos,int & cur_line)
{
    if (cur_line<0) return 2;
int val=cur_pos.at(cur_line)+1;
    if (val>=l.at(cur_line))
    {
        if (l.at(cur_line)<=0)
        cur_pos.replace(cur_line,-1);
        else
        cur_pos.replace(cur_line,0);
    return 0;
    }
    else
    {
    cur_pos.replace(cur_line,val);
    return 1;
    }
}

void generatePermutations(QList<int> & l,QList< QList<int> > & perm)
{
perm.clear();
QList<int> cur_pos;
cur_pos.clear();
for (int i=0;i<l.length();i++)
{
    if (l.at(i)<=0)
    cur_pos << -1;//in every list we are at the beginning
    else
    cur_pos << 0;//in every list we are at the beginning
}
int line=l.length()-1,ret,left_at;
//now we start the loop
while (line>=0)
{
perm << cur_pos;
ret=helpPermutate(l,cur_pos,line);
if (ret==2){line=-1;continue;}
left_at=line;
while (ret==0)
{
line--;
ret=helpPermutate(l,cur_pos,line);
}
if (ret!=2)
line=left_at;
}

}

void splitStringIntoSubstrings(QString text,QChar separator,QStringList & results)
{
results.clear();
int len=text.length();
int start=0,stop=-1;
int quote_counter=0;
QChar probe,quote('\"');
QString temp;
for (int i=0;i<len;i++)
{
probe=text.at(i);
    if (probe==quote)
    {
    quote_counter++;
    if (start<0) start=i;
    }
    else if (probe==separator && quote_counter%2==0)//end found
    {
        if (start<0)
        {
        results << QString("");
        }
        else
        {
        stop=i-1;
        temp=text.mid(start,stop-start+1);
        results << temp;
        start=-1;
        }
    }
    else//something else
    {
    if (start<0) start=i;
    }
}
if (len>0 && start>=0)//we are at the end and there are more than 0 characters
{
stop=len-1;
temp=text.mid(start,stop-start+1);
results << temp;
}
else if(len>0 && start<0)
{
results << QString("");
}

}

//search the n-th string s in text (but not inside quotes), start search at start
int pos_in_text_with_quotes(QString text,QString s,int n,int start)
{
int pos=0,pos_counter=0,next=start;
int count_quotes;
QString subtext;
if (start>=text.length()) return -1;
if (n<1) return start;
while (pos_counter<n && next>=0)
{
pos=text.indexOf(s,next);
if (pos<0)
{
next=-1;
break;
}
subtext=text.left(pos);
count_quotes=subtext.count(QString("\""));
    if (count_quotes%2==0) //the quotes are closed before the found poition --> ok
    {
    pos_counter++;
    }
next=pos+1;
}
if (next<0) //failure
return -1;
else
return pos;
}

//search the occurences of s in text (but not inside quotes), start search at start
int count_in_text_with_quotes(QString text,QString s,int start)
{
int pos=0,pos_counter=0,next=start;
int count_quotes;
QString subtext;
if (start>=text.length()) return 0;
    while (next>=0)
    {
    pos=text.indexOf(s,next);
    if (pos<0)
    {
    break;
    }
    subtext=text.left(pos);
    count_quotes=subtext.count(QString("\""));
        if (count_quotes%2==0) //the quotes are closed before the found poition --> ok
        {
        pos_counter++;
        }
    next=pos+1;
    }
return pos_counter;
}

int pos_in_text_without_quotes(QString text,QString s,int n,int start)
{
int pos,pos_counter=0,next=start;
QString subtext;
if (start>=text.length()) return -1;
if (n<1) return start;
while (pos_counter<n && next>=0)
{
pos=text.indexOf(s,next);
if (pos<0)
{
next=-1;
break;
}
pos_counter++;
next=pos+1;
}
if (next<0) //failure
return -1;
else
return pos;
}

int count_in_text_without_quotes(QString text,QString s,int start)
{
int pos_count=0;
if (start>=text.length()) return 0;
QString subtext=text.mid(start);
pos_count=subtext.count(s);
return pos_count;
}

QString process_a_text(QString text,int type,QString startChar,int n1,QString stopChar,int n2,bool considerQuotes)
{
QString result;
int startpos,stoppos,counter;
startpos=stoppos=counter=0;
    if (type==0)//none
    {
    result=text;
    }
    else
    {
        if (type==1)//Take first n characters
        {
        result=text.left(n1);
        }
        else if (type==2)//Take last n characters
        {
        result=text.right(n1);
        }
        else if (type==3)//Take middle characters
        {
        result=text.mid(n1,n2);
        }
        else
        {
            if (considerQuotes==false)
            {
    switch (type)
    {
    default:
    break;
    case 4://Take everything before char
    startpos=0;
    stoppos=pos_in_text_without_quotes(text,stopChar,n1,0)-1;
        if (stoppos<0)//if not enough characters occur --> take everything
        {
        stoppos=text.length()-1;
        }
    break;
    case 5://Take everything after char
    startpos=pos_in_text_without_quotes(text,startChar,n1,0)+1;
        if (startpos<0)//if not enough characters occur --> take nothing
        {
        startpos=text.length();
        }
    stoppos=text.length()-1;
    break;
    case 6://Take everything between chars
    startpos=pos_in_text_without_quotes(text,startChar,n1,0)+1;
        if (startpos<0)//if not enough characters occur --> take everything
        {
        startpos=0;
        }
    stoppos=pos_in_text_without_quotes(text,stopChar,n2,startpos+1)-1;
        if (stoppos<0)//if not enough characters occur --> take everything
        {
        stoppos=text.length()-1;
        }
    break;
    }
            }
            else
            {
    switch (type)
    {
    default:
    break;
    case 4://Take everything before char
    startpos=0;
    stoppos=pos_in_text_with_quotes(text,stopChar,n1,0)-1;
        if (stoppos<0)//if not enough characters occur --> take everything
        {
        stoppos=text.length()-1;
        }
    break;
    case 5://Take everything after char
    startpos=pos_in_text_with_quotes(text,startChar,n1,0)+1;
        if (startpos<0)//if not enough characters occur --> take nothing
        {
        startpos=text.length();
        }
    stoppos=text.length()-1;
    break;
    case 6://Take everything between chars
    startpos=pos_in_text_with_quotes(text,startChar,n1,0)+1;
        if (startpos<0)//if not enough characters occur --> take everything
        {
        startpos=0;
        }
    stoppos=pos_in_text_with_quotes(text,stopChar,n2,startpos+1)-1;
        if (stoppos<0)//if not enough characters occur --> take everything
        {
        stoppos=text.length()-1;
        }
    break;
    }
            }
        result=text.mid(startpos,stoppos-startpos+1);
        }
    }
return result;
}

frmCreateAltAxis::frmCreateAltAxis(QWidget * parent):QDialog(parent)
{
setWindowTitle(tr("QtGrace: Create Alt-Axis"));
/*setWindowIcon(QIcon(*GraceIcon));*/

layout=new QGridLayout();
layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
layout->setSpacing(STD_SPACING);

lblText=new QLabel(tr("This dialog helps you to set up an alternative-axis.\nIt uses a transformation formula to calculate the positions of the alternative tick-marks\nand uses the number of tick-marks and start and stop to generate custom ticks at custom positions.\nYou may also set the new axis-label and set where the alternative axis goes."),this);

lenFormula=new stdLineEdit(this,tr("Formula for axis transformation:"));
lenFormula->setText(QString("$t/10"));
selNumber=new stdIntSelector(this,tr("Number of tick-labels:"),1,100);
selNumber->setValue(2);

selStart=new LineWidthSelector(this);
selStop=new LineWidthSelector(this);
selStart->setRange(-1e38,1e38);
selStart->setDecimals(5);
selStop->setRange(-1e38,1e38);
selStop->setDecimals(5);
selStart->lblText->setText(tr("Start of tick-labels:"));
selStop->lblText->setText(tr("Stop of tick-labels:"));
selStart->setValue(0.0);
selStop->setValue(1.0);
lenLabel=new stdLineEdit(this,tr("Axis-label:"));
lenLabel->setText(QString(""));
int nr=2;
QString entr[8];
entr[0]=tr("Opposite");
entr[1]=tr("Normal");
selSide=new StdSelector(this,tr("Side for alternative axis:"),nr,entr);
selSide->setToolTip(tr("The regular axis will be pushed to the opposite side as the alternative axis.\nThe tick-marks and tck-labels will restricted to the respective axis."));

cmdOk=new QPushButton(tr("Apply"),this);
cmdCancel=new QPushButton(tr("Cancel"),this);
connect(cmdOk,SIGNAL(clicked()),this,SLOT(doOk()));
connect(cmdCancel,SIGNAL(clicked()),this,SLOT(doCancel()));

int index=0;
layout->addWidget(lblText,index++,0,1,2);

layout->addWidget(lenLabel,index++,0,1,2);
layout->addWidget(lenFormula,index++,0,1,2);
layout->addWidget(selSide,index,0,1,1);
layout->addWidget(selNumber,index++,1,1,1);
layout->addWidget(selStart,index,0,1,1);
layout->addWidget(selStop,index++,1,1,1);

layout->addWidget(cmdOk,index,0,1,1);
layout->addWidget(cmdCancel,index++,1,1,1);
setLayout(layout);
resize(LastSize_CreateAltAxis);
}

frmCreateAltAxis::~frmCreateAltAxis()
{
LastSize_CreateAltAxis=this->size();
}

void frmCreateAltAxis::init(int gno,int ax)
{
orig_gno=gno;
orig_axis=ax;
}

void frmCreateAltAxis::doCancel(void)
{
this->hide();
}

void frmCreateAltAxis::doOk(void)
{
n_label=lenLabel->text();
n_formula=lenFormula->text();
number=selNumber->value();
n_start=selStart->value();
n_stop=selStop->value();
side=selSide->currentIndex();

world w;
int ret=get_graph_world(orig_gno,&w);
double x01,x02;
if (orig_axis==0 || orig_axis==2)//X-axis
{
x01=w.xg1;
x02=w.xg2;
}
else
{
x01=w.yg1;
x02=w.yg2;
}

switch (orig_axis)
{
case 0://X
target_alt_axis=2;
break;
case 1://Y
target_alt_axis=3;
break;
case 2://alt_X
target_alt_axis=0;
break;
case 3://alt_Y
target_alt_axis=1;
break;
}

double guess=(x01+x02)/2.0;
double * new_tick_marks=new double[2+number];
double * new_tick_pos=new double[2+number];
double delta=(number<=1?0.0:(n_stop-n_start)/(number-1.0));

qDebug() << "doOk; new ticks at:";
for (int i=0;i<number;i++)
{
new_tick_marks[i]=n_start+i*delta;
ret=find_x_for_y(n_formula,new_tick_pos+i,guess,new_tick_marks[i]);
qDebug() << i << "-->" << new_tick_pos[i] << "-->" << new_tick_marks[i];
}

delete[] new_tick_marks;
delete[] new_tick_pos;
}

