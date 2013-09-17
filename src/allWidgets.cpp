/***************************************************************************
 *   Copyright (C) 2008-2012 by Andreas Winter                             *
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

#include "allWidgets.h"
#include "MainWindow.h"
#include "events.h"
#include "device.h"
#include "globals.h"
#include "t1fonts.h"
#include "t1lib.h"
#include "ssdata.h"
#include "graphutils.h"
#include "events.h"
#include "parser.h"
#include "undo_module.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _MSC_VER
#include "rint.h"
#else
#include <unistd.h>
#endif

#include <string.h>
#include <fcntl.h>

#define OPTYPE_COPY 0
#define OPTYPE_MOVE 1
#define OPTYPE_SWAP 2
#define OPTYPE_COPY_LEGEND 3

/* nonlprefs.load possible values */
#define LOAD_VALUES         0
#define LOAD_RESIDUALS      1
#define LOAD_FUNCTION       2

#define  WEIGHT_NONE    0
#define  WEIGHT_Y       1
#define  WEIGHT_Y2      2
#define  WEIGHT_DY      3
#define  WEIGHT_CUSTOM  4

struct komplex one={1.0,0.0};
struct komplex zero={0.0,0.0};
struct komplex i_one={0.0,1.0};
double pi=2.0*asin(1.0);
int new_set_no;
int * new_set_nos=NULL;

extern bool useQtFonts;
bool ApplyError=false;
bool GlobalInhibitor=false;
bool activateLaTeXsupport=true; //2013-07-03 Enable LaTex support as default- Nimal Kailasanathan
bool immediateUpdate=false;
bool updateRunning=false;
int default_Print_Device=-1;//-1=last one
extern int rtiCheckTime;
extern int stdOutputFormat;//actually: last Output Format
extern char dummy[];

QStringList ListOfChanges;
QStringList ListOfOldStates;

extern CMap_entry *cmap_table;
extern CMap_entry cmap_init[16];
extern int dirtystate;
extern bool exchange_point_comma;
extern bool display_help_externally;
extern char qt_grace_exe_dir[1024];
extern long orig_page_w,orig_page_h;
extern double GeneralPageZoomFactor;
extern Device_entry *device_table;
extern MainWindow * mainWin;
extern frmCommands * FormCommands;
extern bool useQPrinter;
extern QPrinter * stdPrinter;
extern frmTextProps * TextProps;
extern frmTextProps * EditTextProps;
extern frmLineProps * LineProps;
extern frmLineProps * EditLineProps;
extern frmEllipseProps * BoxProps;
extern frmEllipseProps * EditBoxProps;
extern frmEllipseProps * EllipseProps;
extern frmEllipseProps * EditEllipseProps;
extern frmEditColumnProp * EditColumn;
extern frmIOForm * FormReadNetCDF;
extern QPixmap * GraceIcon;
extern QIcon * LineIcons[MAXLINESTYLES];
extern QBitmap * patterns[MAXPATTERNS];
extern QBitmap * Qt_justifications[12];
extern QFont stdFont;
extern frmIOForm * FormSelectOutputFile;
extern int number_of_opened_spreadsheets;
extern frmSpreadSheet ** FormSpreadSheets;
extern QPainter * GeneralPainter;
extern QImage * MainPixmap;
extern QBitmap * Qt_justifications[12];
extern QBitmap * Qt_matrixOrder[8];
extern QFont stdFont;
extern frmIOForm * FormSelectHotLink;
extern frmConsole * FormConsole;

extern void HelpCB(char *data);
extern int get_cg(void);
extern frmSetOperations * FormSetOPS;
extern frmSetAppearance * FormSetAppearance;
extern frmSetEditor * FormSetEditor;
extern frmAxisProp * FormAxisProperties;

extern frmExtraPreferences * ExtraPreferences;
extern bool showhideworkaround;
extern char SystemsDecimalPoint;

extern int max_history;
extern int current_history;
extern QString history[MAX_HISTORY];
extern QList<QByteArray> avcod;
extern QTextCodec * FileCodec;

extern int current_language;
extern QTranslator translator[nr_of_translations];
extern char translator_files[nr_of_translations][64];
extern char translator_languages[nr_of_translations][64];
extern void change_language(int index);
extern void copy_Grace_to_LaTeX(void);
extern void update_default_props(void);
extern char *next_token(char *s, char **token, int *quoted);
extern void exchangePointComma(char * token);

static T1_TMATRIX UNITY_MATRIX = {1.0, 0.0, 0.0, 1.0};
extern QBitmap generate_Bitmap_form_Bits(unsigned char * bits,int length,int rows,int cols);
extern bool undo_active;
extern int max_node_nr;
extern region undo_region;
extern region undo_region2;

extern int maxboxes;
extern int maxlines;
extern int maxstr;
extern int maxellipses;

extern int nb_rt;
extern Input_buffer *ib_tbl;
extern int ib_tblsize;

int nr_of_set_concerned=0;
int * sets_concerned_g=NULL;
int * sets_concerned_s=NULL;

int nrOfUndoObjs;
int * undoObjs;

extern int arrange_graphs(int *graphs, int ngraphs,
                          int nrows, int ncols, int order, int snake,
                          double loff, double roff, double toff, double boff,
                          double vgap, double hgap,
                          int hpack, int vpack);
extern void init_color_icons(int nr_of_cols,CMap_entry * entries,int & allocated_colors,QIcon *** ColorIcons,QPixmap *** ColorPixmaps,QString *** ColorNames);
extern void complete_LaTeX_to_Grace_Translator(QString & text);

extern int regiontype;
extern bool wait_till_update;
extern int ReqUpdateColorSel;

int track_setno=-1;
int track_add_at;    /* where to begin inserting points in the set */
int track_move_dir;  /* direction on point movement */
extern int curformat;
extern int curdevice;
extern unsigned long page_scale;
extern double pixel_size;
extern float page_scalef;
extern int page_orientation;
extern int *psfont_status;
extern int ps_color;
extern int ps_pattern;
extern double ps_linew;
extern int ps_lines;
extern int ps_linecap;
extern int ps_linejoin;
extern int ps_grayscale;
extern int ps_level2;
extern int docdata;
extern int ps_setup_offset_x;
extern int ps_setup_offset_y;
extern int ps_setup_grayscale;
extern int ps_setup_level2;
extern int ps_setup_docdata;
extern int ps_setup_feed;
extern int ps_setup_hwres;
extern int eps_setup_grayscale;
extern int eps_setup_level2;
extern int eps_setup_tight_bb;
extern int eps_setup_docdata;
extern int tight_bb;
extern int pnm_setup_format;
extern int pnm_setup_rawbits;


extern int maxgraph;
extern int cursortype;

int current_page_units = 0;
#if defined WITH_XMHTML || defined WITH_LIBHELP
extern int force_external_viewer;
#endif

extern void xdrawgraph(void);
extern void sort(int number,int * items);
extern char *set_types(int it);
extern nonlparms nonl_parms[];
extern nonlopts nonl_opts;

extern struct ColumnFormat binaryImportFormat[NUMBER_OF_COLUMN_FORMATS];
extern char binaryImportFormatName[NUMBER_OF_COLUMN_FORMATS][32];
extern int ImportDestination[NUMBER_OF_IMPORT_DESTINATIONS];
extern char ImportDestinationName[NUMBER_OF_IMPORT_DESTINATIONS][32];
extern signed char ImportDestinationType[NUMBER_OF_IMPORT_DESTINATIONS];
extern int openPipe(char * pname,int * fd);

/* prefs for non-linear fit */
typedef struct {
    int autoload;       /* do autoload */
    int load;           /* load to... */
    int npoints;        /* # of points to evaluate function at */
    double start;       /* start... */
    double stop;        /* stop ... */
} nonlprefs;

char buf[256];
char command[1024];
nonlprefs nonl_prefs = {TRUE, LOAD_VALUES, 10, 0.0, 1.0};

int outputQuality=75;
//bool outputGrayscale=false;
int init_null(void){return 0;}	    /* function to initialize device */
int parser_null(char * v){return 0;}  /* function to parse device-specific commands */
void setup_null(void){;}    /* function (GUI interface) to setup device */

QString get_filename_with_extension(int device)//generates a filename from the name of the document, using the extension of the given device
{
    Device_entry dev = get_device_props(device);
    /*char printfile[GR_MAXPATHLEN];
strcpy(printfile,get_docname());*/

    QFileInfo fi(get_docname());

    /*cout << endl;
cout << "DocName = #" << printfile << "# abs=" << fi.isAbsolute() << endl;
cout << "AbsFilePath = #" << fi.absoluteFilePath().toAscii().constData() << "#" << endl;
cout << "FileNameOnly = #" << fi.fileName().toAscii().constData() << "#" << endl;
cout << "PathOnly = #" << fi.absolutePath().toAscii().constData() << "#" << endl;
cout << "ComplBaseName = #" << fi.completeBaseName().toAscii().constData() << "#" << endl;*/

    QString fwe;
    fwe=fi.absolutePath() + QDir::separator() + fi.completeBaseName() + QString(".") + QString(dev.fext);
    //cout << "newFileName = #" << fwe.toAscii().constData() << "#" << endl;
    return fwe;

    /*if (!fi.isAbsolute())
{
strcpy(printfile,fi.absoluteFilePath().toAscii().constData());
}

char * bufptr = strrchr(printfile, '.');//search for the last '.'
if (!strcmp(get_docname(),"Untitled") || bufptr==NULL)//no real document-name, just the default OR document-name does not contain a '.'
{
strcat(printfile,".");//add a '.' at the end
}
else if (bufptr)
{
*(bufptr + 1) = '\0';//remove extensions
}
strcat(printfile,dev.fext);//add the appropriate extension
return QString(printfile);*/
}

int nr_of_true_changes(QStringList & list)
{
    static int nr,len;
    static QString ref("with");
    nr=list.length();
    len=nr;
    for (int i=0;i<nr;i++)
    {
        if (list.at(i).indexOf(ref)==0)//contains with at the beginning
            len--;
    }
    return len;
}

void prependAllSetID(QString * text,int sno,int gno)
{
    static char * tosuppl[7]={"X","Y","Y0","Y1","Y2","Y3","Y4"};
    QString str(text->toLatin1());
    QString l1;
    QString toinsert;
    QRegExp regex1("G\\d*.S\\d*.$");//what we search for as a regular expression
    QRegExp regex2("S\\d*.$");
    char dummy1[128];
    char dummy2[128];
    sprintf(dummy1,"G%d.",gno);
    sprintf(dummy2,"G%d.S%d.",gno,sno);
    int pos=0,last=0,inlen;
    int p1;
    for (int i=0;i<2;i++)//find replacements for all substrings (Y0...Y4 are automatically replaced during the search for Y)
    {
        pos=0;
        while (pos<str.length())
        {
            //cout << "#" << str.toAscii().constData() << "#" << pos << "#" << tosuppl[i] << endl;
            last=pos;//last is position at beginning of search
            pos=str.indexOf(tosuppl[i],pos);//pos is now the position of the string we search for
            if (pos==-1)
            {
                last=pos=str.length()+1;//nothing found
            }
            else
            {
                l1=str.left(pos);
                p1=regex1.indexIn(l1);
                if (p1==-1)//expression not found --> id not complete
                {
                    p1=regex2.indexIn(l1);
                    if (p1==-1)//expression not found --> no id at all
                    {
                        toinsert=QString(dummy2);
                        inlen=toinsert.length();
                        str.insert(pos,toinsert);
                        pos+=inlen+1;
                    }
                    else//expression found --> id only simplified --> 'G0.' missing
                    {
                        toinsert=QString(dummy1);
                        inlen=toinsert.length();
                        str.insert(p1,toinsert);
                        pos+=inlen+1;
                    }
                }
                else//complete id found --> do nothing
                {
                    pos=pos+1;//search on from last position
                }
            }//end of if
        }//end while
    }//end of go throught all substrings
    *text=str;
}

void prependSetID(char * text,int leftset,int leftgraph,int rightset,int rightgraph)
{
    //left id is target id
    //right id is source id
    QString original(text);
    original=original.toUpper();
    int len=original.length();
    int eq_pos=original.indexOf(QString("="));
    QString leftpart=original.left(eq_pos);
    QString rightpart=original.right(len-eq_pos-1);
    //cout << "G[" << leftgraph << "].S[" << leftset << "]" << endl;
    //cout << "G[" << rightgraph << "].S[" << rightset << "]" << endl;
    //cout << "#" << leftpart.toAscii().constData() << "#=#" << rightpart.toAscii().constData() << "#" << endl;
    prependAllSetID(&leftpart,leftset,leftgraph);
    prependAllSetID(&rightpart,rightset,rightgraph);
    QString result=leftpart+QString("=")+rightpart;
    strcpy(text,result.toAscii().constData());
    //cout << "result=#" << text << "#" << endl;
}

/*int find_pos_of_inclomplete_setid(char * text,char * tofind,char ** pos)
{
int ret=-1;//-1=nothing found; 0=fully incomplete; 1=only GraphId missing; 2=found and complete
int pos2;
*pos=strstr(text,tofind);
while (pos!=NULL)//go on until suitable command found
{//we found a position
ret=*pos-text;
if (ret>0)
{
pos2=ret-1;
    if (text[pos2]!='.')//-->incomplete, ret is position of incomplete description
    {
    ret=0;
    break;
    }
    else//may be incomplete, but '.' before
    {
    pos2--;
    while (pos2>0 && isdigit(text[pos2])) pos2--;//try to find description before Column-Description
        if (pos2<=1)//incomplete
        {
        *pos=text;
        ret=0;
        }
        else//we presume: we have found a 'S'
        {
        pos2--;
        if (text[pos2]!='.')
        {
        ret=1;
        *pos=text+pos2+1;
        break;
        }
        else
        {
        ret=2;
        break;
        }
        }
    }
}
else
{
ret=0;
break;//found at first character --> not complete!
}
*pos=strstr(*pos+1,tofind);
}
return ret;
}

void include_str(char * text,char * pos,char * toinclude)
{
if (!pos) return;
char * dummy=new char[strlen(text)+strlen(toinclude)+2];
//cout << "include#" << text << "#" << pos << "#" << toinclude << "#" << endl;
strcpy(dummy,text);
strcpy(dummy+(pos-text),toinclude);
strcpy(dummy+(pos-text+strlen(toinclude)),pos);
strcpy(text,dummy);
delete[] dummy;
}

void correct_formula(char * text,int gno,int sno)//takes a formula and supplements all x,y,y1,y2,y3,y4 by 'G0.S0.' or something apropriate according to gno and sno
{
static char suppl[32], suppl2[16], temp[1024];
static char * tosuppl[7]={"X","Y","Y0","Y1","Y2","Y3","Y4"};
static char * n_pos,*n_pos2;
static unsigned int len;
static int pos,retu;
sprintf(suppl,"G%d.S%d.",gno,sno);
sprintf(suppl2,"G%d.",gno);
len=strlen(text);
char result[128+strlen(text)];
    for (unsigned int i=0;i<len;i++)
    result[i]=toupper(text[i]);
for (int i=0;i<7;i++)
{//supplement the entries
n_pos2=result;
while (n_pos2)
{
retu=find_pos_of_inclomplete_setid(n_pos2,tosuppl[i],&n_pos);
cout << "text=#" << result << "#retu=" << retu << endl;
    switch (retu)
    {
    case 0://0=fully incomplete
    include_str(result,n_pos,suppl);
        if (n_pos)
        n_pos+=strlen(suppl);
    break;
    case 1://1=only GraphId missing
    include_str(result,n_pos,suppl2);
        if (n_pos)
        n_pos+=strlen(suppl2);
    break;
    default:
    n_pos=NULL;
    break;
    }
cout << "text=#" << result << "#retu=" << retu << endl;
if (n_pos) cout << "npos=#" << n_pos << "#" << endl;
        if (n_pos)
        n_pos2=n_pos+1;
        else
        n_pos2=NULL;
}//end while loop

}//end supplements
strcpy(text,result);
}*/

/* 'Legacy' FFT code from Grace v5.99*/
int bit_swap_v6(int i, int nu);
int ilog2_v6(int n);
int fft_v6(double *jr, double *ji, int n, int nu, int iflag);

int fourier_v6(double *jr, double *ji, int n, int iflag)
{
    int i2= ilog2(n);

    /*if ((i2 = ilog2(n)) > 0) {*/
    return fft_v6(jr, ji, n, i2, iflag);
    /*} else {
        return dft(jr, ji, n, iflag);
    }*/
}

/*
   real_data ... ptr. to real part of data to be transformed
   imag_data ... ptr. to imag  "   "   "   "  "      "
   inv ..... Switch to flag normal or inverse transform
   n_pts ... Number of real data points
   nu ...... logarithm in base 2 of n_pts e.g. nu = 5 if n_pts = 32.
*/
int fft_v6(double *real_data, double *imag_data, int n_pts, int nu, int inv)
{
    int n2, i, ib, mm, k;
    int sgn, tstep;
    double tr, ti, arg; /* intermediate values in calcs. */
    double c, s;        /* cosine & sine components of Fourier trans. */
    static double *sintab = NULL;
    static int last_n = 0;

    n2 = n_pts / 2;

    if (n_pts != last_n) { /* allocate new sin table */
        arg = 2*M_PI/n_pts;
        last_n = 0;
        sintab = (double *)xrealloc(sintab, n_pts*sizeof(double));
        if (sintab == NULL) {
            return RETURN_FAILURE;
        }
        for (i = 0; i < n_pts; i++) {
            sintab[i] = sin(arg*i);
        }
        last_n = n_pts;
    }

    /*
 * sign change for inverse transform
 */
    sgn = inv ? 1:-1;

    /* do bit reversal of data in advance */
    for (k = 0; k != n_pts; k++) {
        ib = bit_swap_v6(k, nu);
        if (ib > k) {
            fswap((real_data + k), (real_data + ib));
            fswap((imag_data + k), (imag_data + ib));
        }
    }
    /*
* Calculate the componets of the Fourier series of the function
*/
    tstep = n2;
    for (mm = 1; mm < n_pts; mm *= 2) {
        int sinidx = 0, cosidx = n_pts/4;
        for (k=0; k<mm; k++) {
            c = sintab[cosidx];
            s = sgn*sintab[sinidx];
            sinidx += tstep;
            cosidx += tstep;
            if (sinidx >= n_pts) {
                sinidx -= n_pts;
            }
            if (cosidx >= n_pts) {
                cosidx -= n_pts;
            }
            for (i = k; i < n_pts; i += mm*2) {
                double re1, re2, im1, im2;
                re1 = real_data[i];
                im1 = imag_data[i];
                re2 = real_data[i + mm];
                im2 = imag_data[i + mm];

                tr = re2*c + im2*s;
                ti = im2*c - re2*s;
                real_data[i+mm] = re1 - tr;
                imag_data[i+mm] = im1 - ti;
                real_data[i] = re1 + tr;
                imag_data[i] = im1 + ti;
            }
        }
        tstep /= 2;
    }

    return RETURN_SUCCESS;
}

/*
* Bit swapping routine in which the bit pattern of the integer i is reordered.
* See Brigham's book for details
*/
int bit_swap_v6(int i, int nu)
{
    int ib, i1, i2;

    ib = 0;

    for (i1 = 0; i1 != nu; i1++) {
        i2 = i/2;
        ib = ib*2 + i - 2*i2;
        i = i2;
    }
    return (ib);
}

/*
 * log base 2
 */
int ilog2_v6(int n)
{
    int i = 0;
    int n1 = n;

    while (n1 >>= 1) {
        i++;
    }
    if (1 << i != n) {
        return -1;
    } else {
        return i;
    }
}

int monospaced_v6(double *array, int len, double *space)
{
    int i;
    double eps;
    if (len < 2) {
        //errmsg(tr("Monospacing of an array of length < 2 is meaningless").toAscii().constData());
        return FALSE;
    }
    *space = array[1] - array[0];
    eps = fabs((array[len - 1] - array[0]))*1.0e-6; /* FIXME */
    for (i = 2; i < len; i++) {
        if (fabs(array[i] - array[i - 1] - *space) > eps) {
            return FALSE;
        }
    }
    return TRUE;
}

void strToUpper(char * tar,char * ch)
{
    int len=strlen(ch);
    for (int i=0;i<len;i++)
        tar[i]=toupper(ch[i]);
    tar[len]=ch[len];
}

int find_dev_nr(char * dev_name)/*my own number of devices*/
{
    int nr=-1;
    if (!strcmp(dev_name,"JPEG"))
    {
        nr=DEVICE_JPEG;
    }
    else if (!strcmp(dev_name,"BMP"))
    {
        nr=DEVICE_BMP;
    }
    else if (!strcmp(dev_name,"EPS"))
    {
        nr=DEVICE_EPS;
    }
    else if ((!strcmp(dev_name,"PS")) || (!strcmp(dev_name,"PostScript")))
    {
        nr=DEVICE_PS;
    }
    else if (!strcmp(dev_name,"PNM"))
    {
        nr=DEVICE_PNM;
    }
    else if (!strcmp(dev_name,"PNG"))
    {
        nr=DEVICE_PNG;
    }
    else if (!strcmp(dev_name,"X11"))
    {
        nr=0;
    }
    return nr;
}

/*with QT the JPG and PNG and BMP file types are easily accessible, but only very few options are available*/
static Device_entry dev_jpg = {DEVICE_FILE,
                               "JPEG",
                               init_null,
                               parser_null,
                               setup_null,
                               "jpg",
                               FALSE,
                               TRUE,
                               {DEFAULT_PAGE_WIDTH, DEFAULT_PAGE_HEIGHT, 72.0},
                               NULL
                              };

static Device_entry dev_png = {DEVICE_FILE,
                               "PNG",
                               init_null,
                               parser_null,
                               setup_null,
                               "png",
                               FALSE,
                               TRUE,
                               {DEFAULT_PAGE_WIDTH, DEFAULT_PAGE_HEIGHT, 72.0},
                               NULL
                              };

static Device_entry dev_bmp = {DEVICE_FILE,
                               "BMP",
                               init_null,
                               parser_null,
                               setup_null,
                               "bmp",
                               FALSE,
                               TRUE,
                               {DEFAULT_PAGE_WIDTH, DEFAULT_PAGE_HEIGHT, 72.0},
                               NULL
                              };

#define DATASETOP_SORT 0
#define DATASETOP_REVERSE 1
#define DATASETOP_JOIN 2
#define DATASETOP_SPLIT 3
#define DATASETOP_DROP 4

#define cg get_cg()

void register_qt_devices(void)
{

#ifdef _MSC_VER 
    //Missing JPEG libraries for Windows
#else
    register_device(dev_jpg);
#endif

    register_device(dev_png);
    register_device(dev_bmp);
}

frmEditColumnProp::frmEditColumnProp(QWidget * parent):QDialog(parent)
{
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Edit set properties"));
    setWindowIcon(QIcon(*GraceIcon));
    int number;
    QString entr[35];
    number=6;
    entr[0]=QString("X");
    entr[1]=QString("Y");
    entr[2]=QString("Y1");
    entr[3]=QString("Y2");
    entr[4]=QString("Y3");
    entr[5]=QString("Y4");
    selColumn=new StdSelector(this,tr("Column:"),number,entr);
    number=4;
    entr[0]=QString("Decimal");
    entr[1]=QString("General");
    entr[2]=QString("Exponential");
    entr[3]=QString("Date/time");
    selFormat=new StdSelector(this,tr("Format:"),number,entr);
    selFormat->setCurrentIndex(1);
    selPrecision=new stdIntSelector(this,tr("Precision:"),0,20);
    buttonGroup=new stdButtonGroup(this,true,true,false);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));

    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(selColumn);
    layout->addWidget(selFormat);
    layout->addWidget(selPrecision);
    layout->addWidget(buttonGroup);
    setLayout(layout);
    setModal(TRUE);
}

void frmEditColumnProp::doApply(void)
{
    ApplyError=false;
    int col=selColumn->currentIndex();
    int format=selFormat->currentValue();
    int prec=selPrecision->value();
    emit(newColumnFormat(col,format,prec));
}

void frmEditColumnProp::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmEditColumnProp::doClose(void)
{
    hide();
}

void frmEditColumnProp::initToColumn(int i)
{
    selColumn->setCurrentIndex(i);
    selFormat->setCurrentIndex(col_format[i]);
    selPrecision->setValue(col_precision[i]);
}

frmSpreadSheet::frmSpreadSheet(QWidget * parent,int g_no,int s_no):QDialog(parent)
{
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Spreadsheet dataset editor"));
    setWindowIcon(QIcon(*GraceIcon));
    CreateActions();
    gno=g_no;
    sno=s_no;
    EditColumn=new frmEditColumnProp(this);
    EditColumn->hide();
    connect(EditColumn,SIGNAL(newColumnFormat(int,int,int)),SLOT(setColumnFormat(int,int,int)));

    menuBar=new QMenuBar(this);
    mnuFile=new QMenu(tr("&File"));
    mnuFile->setTearOffEnabled(TRUE);
    mnuFile->addAction(actClose);
    mnuEdit=new QMenu(tr("&Edit"));
    mnuEdit->setTearOffEnabled(TRUE);
    mnuEdit->addAction(actAddRow);
    mnuEdit->addAction(actDelSelRow);
    mnuEdit->addSeparator();
    mnuEdit->addAction(actColumnFormat);
    mnuEdit->addSeparator();
    mnuEdit->addAction(actSelectEven);
    mnuEdit->addAction(actSelectOdd);
    mnuEdit->addAction(actCopySelected);
    mnuHelp=new QMenu(tr("&Help"));
    mnuHelp->setTearOffEnabled(TRUE);
    mnuHelp->addAction(actHelpOnContext);
    mnuHelp->addSeparator();
    mnuHelp->addAction(actHelpOnDataSetEditor);
    menuBar->addMenu(mnuFile);
    menuBar->addMenu(mnuEdit);
    menuBar->addSeparator();
    menuBar->addMenu(mnuHelp);

    for (int i=0;i<6;i++)
    {
        col_format[i]=1;
        col_precision[i]=5;
        EditColumn->col_format[i]=col_format[i];
        EditColumn->col_precision[i]=col_precision[i];
    }

    sprintf(dummy," G%d.S%d",gno,sno);
    fraDataset=new QGroupBox(tr("Dataset") + QString(dummy),this);

    int number=NUMBER_OF_SETTYPES;
    QString entr[NUMBER_OF_SETTYPES];
    for (int i=0;i<NUMBER_OF_SETTYPES;i++)
    {
        strToUpper(dummy,set_types(i));
        entr[i]=QString(dummy);
    }
    selType=new StdSelector(fraDataset,tr("Type:"),number,entr);
    connect(selType->cmbSelect,SIGNAL(currentIndexChanged(int)),SLOT(changeDataSet(int)));

    ledComment=new stdLineEdit(fraDataset,tr("Comment:"),true);
    ledComment->lenText->setText(tr("Editor"));
    layout0=new QVBoxLayout;
    layout0->setMargin(STD_MARGIN);
    layout0->addWidget(selType);
    layout0->addWidget(ledComment);
    fraDataset->setLayout(layout0);

    buttonGroup=new stdButtonGroup(this,true,true,false);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));

    //table=new SetTableView(gno,sno,col_format,col_precision,this);

    model=new SetTableModel(gno,sno,col_format,col_precision,this);
    table=new QTableView(this);
    table->setModel(model);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(model,SIGNAL(item_entry_changed(int,int,bool)),this,SLOT(itemChanged(int,int,bool)));
    connect(table->horizontalHeader(),SIGNAL(sectionClicked(int)),SLOT(HHeaderClicked(int)));

    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->setSpacing(0);
    layout->addWidget(menuBar);
    layout->addWidget(fraDataset);
    layout->addWidget(table);
    layout->addWidget(buttonGroup);
    setLayout(layout);
    update_entries();
}

void frmSpreadSheet::HHeaderClicked(int i)
{
    EditColumn->initToColumn(i);
    EditColumn->show();
    EditColumn->raise();
}

void frmSpreadSheet::init(int g_no,int s_no)
{
    gno=g_no;
    sno=s_no;
    model->init(gno,sno);
    update_entries();
}

void frmSpreadSheet::CreateActions(void)
{
    actClose= new QAction(tr("&Close"), this);
    actClose->setShortcut(tr("Esc"));
    connect(actClose, SIGNAL(triggered()), this, SLOT(doClose()));
    actAddRow= new QAction(tr("&Add row"), this);
    connect(actAddRow, SIGNAL(triggered()), this, SLOT(doAddRow()));
    actDelSelRow= new QAction(tr("&Delete selected rows"), this);
    connect(actDelSelRow, SIGNAL(triggered()), this, SLOT(doDelSelRow()));
    actColumnFormat= new QAction(tr("&Column format..."), this);
    connect(actColumnFormat, SIGNAL(triggered()), this, SLOT(doColumnFormat()));
    actHelpOnContext= new QAction(tr("On conte&xt"), this);
    actHelpOnContext->setShortcut(tr("Shift+F1"));
    connect(actHelpOnContext, SIGNAL(triggered()), this, SLOT(doHelpOnContext()));
    actHelpOnDataSetEditor= new QAction(tr("On datas&et editor"), this);
    connect(actHelpOnDataSetEditor, SIGNAL(triggered()), this, SLOT(doHelpOnDataSetEditor()));
    actSelectEven= new QAction(tr("Select even rows"), this);
    connect(actSelectEven, SIGNAL(triggered()), this, SLOT(doSelectEven()));
    actSelectOdd= new QAction(tr("Select odd rows"), this);
    connect(actSelectOdd, SIGNAL(triggered()), this, SLOT(doSelectOdd()));
    actCopySelected= new QAction(tr("Copy selection to clipboard"), this);
    connect(actCopySelected, SIGNAL(triggered()), this, SLOT(doCopySelected()));
}

void frmSpreadSheet::changeDataSet(int type)//to change the set-type
{
    if (!is_valid_setno(gno,sno)) return;//do nothing if set is invalid
    int nsets=1;
    int * gnos=new int[2],*snos=new int[2];
    SaveSetStatesPrevious(nsets,gnos,snos,UNDO_COMPLETE);
    set_dataset_type(this->gno, this->sno, type);
    gnos[0]=this->gno;
    snos[0]=this->sno;
    SetsModified(nsets,gnos,snos,UNDO_COMPLETE);
    sprintf(dummy," (G%d.S%d)",gnos[0],snos[0]);
    addAditionalDescriptionToLastNode(-1,QObject::tr("Data set type changed") + QString(dummy),QString());
    mainWin->mainArea->completeRedraw();
    delete[] gnos;
    delete[] snos;
}

void frmSpreadSheet::itemChanged(int row,int col,bool realy_new_value)
{
    /*static char setPrec[8];
    static char out_format[16];
    static char dummy[128];
    static int graphnr,setnr;*/
    if (!realy_new_value || !is_valid_setno(gno,sno)) return;
    /*char dummy2[128];
    QModelIndex modind=model->index(row,col);*/
    gno=model->graphnr;
    sno=model->setnr;
    /*strcpy(dummy2,model->data(modind).toString().toAscii());
    sprintf(setPrec,"%%.%d",col_precision[col]);
    switch (col_format[col])
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
    sprintf(dummy,out_format,g[graphnr].p[setnr].data.ex[col][row]);*/
    ledComment->SetMemoryToText(g[gno].p[sno].comments,g[gno].p[sno].orig_comments);
    set_dirtystate();
    mainWin->mainArea->completeRedraw();
}

void frmSpreadSheet::setColumnFormat(int col,int format,int precision)
{
    col_format[col]=format;
    col_precision[col]=precision;
    update_entries();
}

void frmSpreadSheet::doApply(void)
{
    ApplyError=true;
    if (!is_valid_setno(gno,sno)) return;//do nothing if set is invalid
    ApplyError=false;
    ledComment->SetMemoryToText(g[model->graphnr].p[model->setnr].comments,g[model->graphnr].p[model->setnr].orig_comments);
    mainWin->mainArea->completeRedraw();
}

void frmSpreadSheet::doAccept(void)
{
    ApplyError=false;
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmSpreadSheet::doClose(void)
{
    hide();
}

void frmSpreadSheet::doAddRow(void)
{
    int * sels;
    int nsels,gno,sno;
    int * gnos=new int[3];
    int * snos=new int[3];
    getSelection(&nsels,&sels);
    if (nsels<=0)
    {
        errmsg(tr("No rows selected!").toAscii().constData());
        delete[] gnos;
        delete[] snos;
        return;
    }
    gnos[0]=gno=model->graphnr;
    snos[0]=sno=model->setnr;
    SaveSetStatesPrevious(1,gnos,snos,UNDO_DATA);
    //for (int ii=0;ii<nsels;ii++)
    for (int ii=nsels-1;ii>=0;ii--)
    {
        int i=sels[ii], k;
        int nrows=getsetlength(gno,sno), ncols=dataset_cols(gno,sno);
        char **s;
        Datapoint dpoint;
        if (i > nrows || i < 0)
        {
            errmsg(tr("Selected row out of range!").toAscii().constData());
            delete[] gnos;
            delete[] snos;
            return;
        }
        zero_datapoint(&dpoint);
        if (i < nrows)
        {
            for (k = 0; k < ncols; k++)
            {
                dpoint.ex[k] = g[gno].p[sno].data.ex[k][i];//*(getcol(gno, sno, k) + i);
            }
            if ((s = get_set_strings(gno, sno)) != NULL)
            {
                dpoint.s = s[i];
            }
            add_point_at(gno, sno, i+1, &dpoint);
            //model->insertRow(i+1);
        }
        else
        {
            add_point_at(gno, sno, i, &dpoint);
            //model->insertRow(i);
        }
        update_set_lists(gno);
        /// init(gno, setno);
        update_entries();
        mainWin->mainArea->completeRedraw();
    }
    SetsModified(1,gnos,snos,UNDO_DATA);
    delete[] sels;
    delete[] gnos;
    delete[] snos;
}

void frmSpreadSheet::doDelSelRow(void)
{
    int i;
    int * srows;
    int nsrows;
    int * gnos=new int[3];
    int * snos=new int[3];
    char buf[64];
    getSelection(&nsrows,&srows);
    gnos[0]=model->graphnr;
    snos[0]=model->setnr;
    if (srows[0] < 0 || srows[0] >= getsetlength(gnos[0],snos[0]) || nsrows<=0)
    {
        errmsg(tr("Nothing to delete").toAscii().constData());
        delete[] gnos;
        delete[] snos;
        return;
    }
    sprintf(buf, "Delete %d selected row(s)?", nsrows);
    if (yesno(buf, NULL, NULL, NULL))
    {
        SaveSetStatesPrevious(1,gnos,snos,UNDO_DATA);
        for (i = nsrows - 1; i >= 0; i--)
        {
            if (srows[i]>=0 && srows[i]<getsetlength(gnos[0],snos[0]))
            {
                del_point(gnos[0],snos[0],srows[i]);
                //model->removeRow(srows[i]);
            }
        }
        update_set_lists(gnos[0]);
        /// init(gno, setno);
        update_entries();
        mainWin->mainArea->completeRedraw();
        SetsModified(1,gnos,snos,UNDO_DATA);
    }
    delete[] srows;
    delete[] gnos;
    delete[] snos;
}

void frmSpreadSheet::doColumnFormat(void)
{
    EditColumn->initToColumn(0);
    EditColumn->show();
    EditColumn->raise();
}

void frmSpreadSheet::doHelpOnContext(void)
{
    setCursor(QCursor(Qt::WhatsThisCursor));
}

void frmSpreadSheet::doHelpOnDataSetEditor(void)
{
    HelpCB("doc/UsersGuide.html#SSEditor");
}

void frmSpreadSheet::getSelection(int * n_of_sel,int ** sel)
{
    QModelIndexList list=table->selectionModel()->selectedIndexes();
    *sel=new int[list.length()];
    int counter=0;
    int current;
    bool found;
    for (int i=0;i<list.length();i++)
    {
        current=list.at(i).row();
        found=false;
        for (int j=0;j<counter;j++)
        {
            if ((*sel)[j]==current)
            {
                found=true;
                break;
            }
        }
        if (found==false)
            (*sel)[counter++]=current;
    }
    *n_of_sel=counter;
    sort(*n_of_sel,*sel);
}

void frmSpreadSheet::setSelection(int n_of_sel,int * sel)
{
    QModelIndex index;
    table->clearSelection();
    for (int i=0;i<n_of_sel;i++)
    {
        index=table->model()->index(sel[i],0);
        table->selectionModel()->select(index,QItemSelectionModel::Select|QItemSelectionModel::Rows);
    }
}

void frmSpreadSheet::update_entries(void)
{
    static int rows,cols;
    static char dum[64];
    if (is_valid_setno(gno,sno))
    {
        gno=model->graphnr;
        sno=model->setnr;
        rows=getsetlength(gno,sno);
        cols=dataset_cols(gno,sno);
        sprintf(dum," G%d.S%d ( %d x %d )",gno,sno,rows,cols);
        fraDataset->setTitle(tr("Dataset") + QString(dum));
        ledComment->SetTextToMemory(g[gno].p[sno].comments,g[gno].p[sno].orig_comments);
        model->major_changes();
    }
    else//no valid data present
    {
        sprintf(dum,"Invalid set!");
        fraDataset->setTitle(tr("Dataset") + QString(dum));
    }
}

void frmSpreadSheet::doSelectEven(void)
{
    int nr,*sel=new int[2];
    getSelection(&nr,&sel);
    int rows=getsetlength(gno,sno);
    int * n_sel=new int[nr+rows/2+2];
    memcpy(n_sel,sel,nr*sizeof(int));
    for (int i=0;i<rows;i+=2)
    {
        n_sel[nr++]=i;
    }
    setSelection(nr,n_sel);
    delete[] sel;
    delete[] n_sel;
}

void frmSpreadSheet::doSelectOdd(void)
{
    int nr,*sel=new int[2];
    getSelection(&nr,&sel);
    int rows=getsetlength(gno,sno);
    int * n_sel=new int[nr+rows/2+2];
    memcpy(n_sel,sel,nr*sizeof(int));
    for (int i=1;i<rows;i+=2)
    {
        n_sel[nr++]=i;
    }
    setSelection(nr,n_sel);
    delete[] sel;
    delete[] n_sel;
}

void frmSpreadSheet::doCopySelected(void)
{
    int nr,*sel=new int[2];
    getSelection(&nr,&sel);

    QString text;
    text.clear();
    int rows=getsetlength(gno,sno);
    int col=dataset_cols(gno,sno);

    for (int k=0;k<nr;k++)
    {
        for (int j=0;j<col;j++)
        {
            sprintf(dummy,"%.8g\t",g[gno].p[sno].data.ex[j][sel[k]]);
            text+=QString(dummy);
        }
        text+=QString("\n");
    }

    if (SystemsDecimalPoint!='.') text.replace(QString("."),QString(SystemsDecimalPoint));
    QApplication::clipboard()->setText(text);

    delete[] sel;
}

frmSpreadSheet2::frmSpreadSheet2(QWidget * parent):QDialog(parent)
{
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Spreadsheet dataset editor"));
    setWindowIcon(QIcon(*GraceIcon));
    number_of_rows=50;//visible rows --> this should be enough...
    start_row=0;
    number_of_columns=3;
    allocated_rows=number_of_rows;
    visible_row_count=5;
    length=0;
    initiated=false;
    g_no=s_no=-1;
    table=new QTableWidget(number_of_rows,number_of_columns,this);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    Items=new QTableWidgetItem**[number_of_rows];
    for (int i=0;i<number_of_rows;i++)
    {
        Items[i]=new QTableWidgetItem*[number_of_columns];
        for (int j=0;j<number_of_columns;j++)
        {
            Items[i][j]=new QTableWidgetItem(QString(dummy),QTableWidgetItem::Type);
            table->setItem(i, j, Items[i][j]);
        }
    }
    QStringList vertHeads;
    QStringList horHeads;
    horHeads << "X" << "Y";
    for (int i=0;i<number_of_rows;i++)
    {
        sprintf(dummy,"%7d",i);
        vertHeads << QString(dummy);
    }
    table->setHorizontalHeaderLabels(horHeads);
    table->setVerticalHeaderLabels(vertHeads);
    scroll=new QScrollBar(Qt::Vertical,this);
    scroll->setSingleStep(1);
    fraDataset=new QGroupBox(tr("Dataset"),this);
    selType=new stdSetTypeSelector(this);
    ledComment=new stdLineEdit(fraDataset,tr("Comment:"),true);
    ledComment->lenText->setText(tr("Editor"));
    layout0=new QVBoxLayout;
    layout0->setMargin(STD_MARGIN);
    layout0->addWidget(selType);
    layout0->addWidget(ledComment);
    fraDataset->setLayout(layout0);

    buttonGroup=new stdButtonGroup(this,true,true,false);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));

    layout=new QGridLayout;
    layout->setMargin(STD_MARGIN);
    layout->setSpacing(STD_SPACING);
    //layout->addWidget(menuBar);
    layout->addWidget(fraDataset,0,0,1,2);
    layout->addWidget(table,1,0,1,1);
    layout->addWidget(scroll,1,1,1,1);
    layout->addWidget(buttonGroup,2,0,1,1);
    setLayout(layout);
    connect(scroll,SIGNAL(valueChanged(int)),SLOT(position_changed(int)));
    /*QTableWidget * table;
QTableWidgetItem *** Items;//the Entries!
stdSetTypeSelector *selType
QGroupBox * fraDataset;
stdLineEdit * ledComment;
QVBoxLayout * layout0;
stdButtonGroup * buttonGroup;
QVBoxLayout * layout;*/
}

void frmSpreadSheet2::resizeEvent(QResizeEvent * event)
{
    static double guessed_number_of_rows;
    static int real_height;
    event->accept();
    QSize si=event->size();
    real_height=Items[0][0]->tableWidget()->height();

    //cout << si.width() << " x " << si.height() << "/" << real_height << " one line=" << table->horizontalHeader()->height() << "/";
    //cout << "rowHeight()=" << table->rowHeight(1) << " WidgetHeight=" << table->height() << " scrollHeight=" << table->horizontalScrollBar()->height();
    guessed_number_of_rows=(table->height()-table->horizontalHeader()->height()-table->horizontalScrollBar()->height()-STD_SPACING)*1.0/(table->rowHeight(1));
    //cout << " --> " << guessed_number_of_rows << endl;
    visible_row_count=int(guessed_number_of_rows+1);
    scroll->setRange(0,length-visible_row_count+6);
    scroll->setPageStep(visible_row_count);
    cout << length << " / " << int(guessed_number_of_rows) << endl;
}

void frmSpreadSheet2::position_changed(int pos)
{
    start_row=pos;
    update_entries();
}

void frmSpreadSheet2::init(int gno,int sno)
{
    g_no=gno;
    s_no=sno;
    start_row=0;
    length=getsetlength(gno,sno);
    initiated=true;
    update_entries();
}

void frmSpreadSheet2::update_entries(void)
{
    static char dummy[128];
    static QStringList vertHeads;
    static QString emptyEntry("");
    if (initiated==false) return;
    vertHeads.clear();
    for (int i=start_row;i<start_row+visible_row_count;i++)
    {
        sprintf(dummy,"%7d",i);
        vertHeads << QString(dummy);
        for (int j=0;j<2;j++)
        {
            //sprintf(dummy,out_format,p.data.ex[col][ro]);
            if (i<length)
            {
                sprintf(dummy,"%g",g[g_no].p[s_no].data.ex[j][i]);
                Items[i-start_row][j]->setText(QString(dummy));
            }
            else
            {
                Items[i-start_row][j]->setText(emptyEntry);
            }
        }
    }
    table->setVerticalHeaderLabels(vertHeads);

}

void frmSpreadSheet2::doApply(void)
{

}

void frmSpreadSheet2::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmSpreadSheet2::doClose(void)
{
    hide();
}

myTableWidget::myTableWidget(int r, int c,QWidget * parent):QTableWidget(r,c,parent)
{
}

void myTableWidget::wheelEvent(QWheelEvent * e)
{
    e->accept();
    emit(wheeled(e->delta()));
}

void myTableWidget::keyPressEvent(QKeyEvent * event)
{
    static int keynr;
    event->accept();
    keynr=event->key();
    //cout << "press " << char(event->key()) << "|" << event->key() << endl;
    //if (!(keynr==Qt::Key_Tab || keynr==Qt::Key_Backtab || keynr==Qt::Key_Return || keynr==Qt::Key_Enter || keynr==Qt::Key_PageUp || keynr==Qt::Key_PageDown || keynr==Qt::Key_Down || keynr==Qt::Key_Up || keynr==Qt::Key_Home || keynr==Qt::Key_End))
    if (!(keynr==Qt::Key_Tab || keynr==Qt::Key_Backtab || keynr==Qt::Key_PageUp || keynr==Qt::Key_PageDown || keynr==Qt::Key_Down || keynr==Qt::Key_Up || keynr==Qt::Key_Home || keynr==Qt::Key_End))
        QTableWidget::keyPressEvent(event);
    else
        emit newDirection(keynr);
}

void myTableWidget::keyReleaseEvent(QKeyEvent * event)
{
    static int keynr;
    event->accept();
    keynr=event->key();
    //cout << "release " << char(event->key()) << " | " << event->key() << " | row= " << currentRow() << endl;
    //if (!(keynr==Qt::Key_Tab || keynr==Qt::Key_Backtab || keynr==Qt::Key_Return || keynr==Qt::Key_Enter || keynr==Qt::Key_PageUp || keynr==Qt::Key_PageDown || keynr==Qt::Key_Down || keynr==Qt::Key_Up || keynr==Qt::Key_Home || keynr==Qt::Key_End))
    if (!(keynr==Qt::Key_Tab || keynr==Qt::Key_Backtab || keynr==Qt::Key_PageUp || keynr==Qt::Key_PageDown || keynr==Qt::Key_Down || keynr==Qt::Key_Up || keynr==Qt::Key_Home || keynr==Qt::Key_End))
        QTableWidget::keyReleaseEvent(event);
    //else
    //emit newDirection(keynr);
}

frmSpreadSheet3::frmSpreadSheet3(QWidget * parent):QDialog(parent)
{
    working=false;
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Spreadsheet dataset editor"));
    setWindowIcon(QIcon(*GraceIcon));

    CreateActions();

    EditColumn=new frmEditColumnProp(this);
    EditColumn->hide();
    connect(EditColumn,SIGNAL(newColumnFormat(int,int,int)),SLOT(setColumnFormat(int,int,int)));

    int number;
    char dummy[50];
    QString entr[NUMBER_OF_SETTYPES+2];
    menuBar=new QMenuBar(this);
    mnuFile=new QMenu(tr("&File"));
    mnuFile->setTearOffEnabled(TRUE);
    mnuFile->addAction(actClose);
    mnuEdit=new QMenu(tr("&Edit"));
    mnuEdit->setTearOffEnabled(TRUE);
    mnuEdit->addAction(actAddRow);
    mnuEdit->addAction(actDelSelRow);
    mnuEdit->addSeparator();
    mnuEdit->addAction(actColumnFormat);
    mnuHelp=new QMenu(tr("&Help"));
    mnuHelp->setTearOffEnabled(TRUE);
    mnuHelp->addAction(actHelpOnContext);
    mnuHelp->addSeparator();
    mnuHelp->addAction(actHelpOnDataSetEditor);

    menuBar->addMenu(mnuFile);
    menuBar->addMenu(mnuEdit);
    menuBar->addSeparator();
    menuBar->addMenu(mnuHelp);

    /*scroll=new QScrollArea(this);
spread=new spreadSheet(scroll,0,5,3);
scroll->setWidget(spread);*/

    //4number_of_rows=100;//std-Row-count
    //filled_rows=0;
    number_of_visible_entries=5;//just a guess
    number_of_columns=6;//maximum number of columns
    //setColumnHidden( int column, bool hide );

    for (int i=0;i<6;i++)
    {
        col_format[i]=1;
        col_precision[i]=5;
        EditColumn->col_format[i]=col_format[i];
        EditColumn->col_precision[i]=col_precision[i];
    }

    strcpy(dummy,"");

    //table=new QTableWidget(number_of_rows,number_of_columns,this);
    //the number of lines will not change later!!!
    table=new myTableWidget(MAX_NUMBER_OF_LINES_IN_SPREADSHEET,number_of_columns,this);
    connect(table,SIGNAL(newDirection(int)),this,SLOT(changeDirection(int)));
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    Items=new QTableWidgetItem**[MAX_NUMBER_OF_LINES_IN_SPREADSHEET+1];
    for (int i=0;i<=MAX_NUMBER_OF_LINES_IN_SPREADSHEET;i++)
    {
        Items[i]=new QTableWidgetItem*[number_of_columns+1];
        for (int j=0;j<=number_of_columns;j++)
        {
            Items[i][j]=new QTableWidgetItem(QString(dummy),QTableWidgetItem::Type);
            table->setItem(i, j, Items[i][j]);
        }
    }
    connect(table,SIGNAL(currentCellChanged(int,int,int,int)),SLOT(currentCellChangedWrapper(int,int,int,int)));
    connect(table,SIGNAL(cellChanged(int,int)),SLOT(itemChanged(int,int)));
    connect(table,SIGNAL(wheeled(int)),SLOT(wheel_used(int)));
    QStringList vertHeads;
    QStringList horHeads;
    horHeads << "X" << "Y";
    for (int i=0;i<MAX_NUMBER_OF_LINES_IN_SPREADSHEET;i++)//initialize everything here
    {
        sprintf(dummy,"%7d",i);
        vertHeads << QString(dummy);
    }
    table->setHorizontalHeaderLabels(horHeads);
    table->setVerticalHeaderLabels(vertHeads);
    connect(table->horizontalHeader(),SIGNAL(sectionClicked(int)),SLOT(HHeaderClicked(int)));

    scroll=new QScrollBar(Qt::Vertical,this);
    //scroll=table->verticalScrollBar();
    scroll->setSingleStep(1);
    connect(scroll,SIGNAL(valueChanged(int)),SLOT(position_changed(int)));

    fraDataset=new QGroupBox(tr("Dataset"),this);

    number=NUMBER_OF_SETTYPES;
    for (int i=0;i<NUMBER_OF_SETTYPES;i++)
    {
        strToUpper(dummy,set_types(i));
        entr[i]=QString(dummy);
    }
    selType=new StdSelector(fraDataset,tr("Type:"),number,entr);
    connect(selType->cmbSelect,SIGNAL(currentIndexChanged(int)),SLOT(changeDataSet(int)));
    //selType=new stdSetTypeSelector(this);

    ledComment=new stdLineEdit(fraDataset,tr("Comment:"),true);
    ledComment->lenText->setText(tr("Editor"));
    layout0=new QVBoxLayout;
    layout0->setMargin(STD_MARGIN);
    layout0->addWidget(selType);
    layout0->addWidget(ledComment);
    fraDataset->setLayout(layout0);

    buttonGroup=new stdButtonGroup(this,true,true,false);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));

    layout=new QGridLayout;
    layout->setMargin(STD_MARGIN);
    layout->setSpacing(0);
    layout->addWidget(menuBar,0,0,1,2);
    layout->addWidget(fraDataset,1,0,1,2);
    layout->addWidget(table,2,0,1,1);
    layout->addWidget(scroll,2,1,1,1);
    layout->addWidget(buttonGroup,3,0,1,2);
    setLayout(layout);
    start_row=0;
    inited=false;
}

void frmSpreadSheet3::HHeaderClicked(int i)
{
    EditColumn->initToColumn(i);
    EditColumn->show();
    EditColumn->raise();
}

void frmSpreadSheet3::init(int g_no,int set_no)
{
    if (inited==true && isVisible()==false) return;//do nothing, as long as nothing is visible!
    inited=true;//just for the first init...
    start_row=0;
    disconnect(table,SIGNAL(cellChanged(int,int)), this, SLOT(itemChanged(int,int)));
    char dummy[64];
    QString ds1=tr("Dataset");
    sprintf(dummy," G%d.S%d",g_no,set_no);
    QString ds2=ds1+QString(dummy);
    fraDataset->setTitle(ds2);
    //set new graph- and set-number
    gno=g_no;
    setno=set_no;
    plotarr p;
    //get set data
    get_graph_plotarr(g_no, set_no, &p);
    //Set the comment
    ledComment->SetTextToMemory(g[g_no].p[set_no].comments,g[g_no].p[set_no].orig_comments);
    //ledComment->lenText->setText(getcomment(g_no, set_no));

    //delete everything
    //for (int i=number_of_columns-1;i>=0;i--)
    //table->removeColumn(i);
    //for (int i=0;i<number_of_rows;i++)
    //delete[] Items[i];
    //delete[] Items;

    //renew all entries
    setLength=getsetlength(g_no, set_no);

    /*
if (filled_rows<30)//Minimum is 30 lines (see below)
number_of_rows=30;
else
number_of_rows=5+filled_rows;//at least 5 more than needed
*/

    number_of_columns=settype_cols(p.type);
    if (number_of_columns<2)
    {
        cout << "WARNING: number_of_columns=" << number_of_columns << endl;
        number_of_columns=2;
    }
    table->setColumnCount(number_of_columns);

    //recreate whole spreadsheet
    /*
table->setRowCount(number_of_rows);
table->setColumnCount(number_of_columns);
    Items=new QTableWidgetItem**[number_of_rows];
    for (int i=0;i<number_of_rows;i++)
    {
    Items[i]=new QTableWidgetItem*[number_of_columns];
        for (int j=0;j<number_of_columns;j++)
        {
        Items[i][j]=new QTableWidgetItem(QString(""),QTableWidgetItem::Type);
        table->setItem(i, j, Items[i][j]);
        }
        }
*/
    disconnect(selType->cmbSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(changeDataSet(int)));
    selType->setCurrentIndex(p.type);
    connect(selType->cmbSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(changeDataSet(int)));
    connect(table,SIGNAL(cellChanged(int,int)),this,SLOT(itemChanged(int,int)));
    //resize(this->size());
    //update_entries(); --> will be done in 'position_changed(0);' below
    scroll->setValue(0);
    position_changed(0);
}

void frmSpreadSheet3::changeDataSet(int type)
{
    set_dataset_type(this->gno, this->setno, type);
    mainWin->mainArea->completeRedraw();
}

void frmSpreadSheet3::setColumnFormat(int col,int format,int precision)
{
    col_format[col]=format;
    col_precision[col]=precision;
    update_entries();
}

void frmSpreadSheet3::resizeEvent(QResizeEvent * event)
{
    event->accept();
    static double guessed_number_of_visible_entries;
    guessed_number_of_visible_entries=(table->size().height()-table->horizontalHeader()->height()-table->horizontalScrollBar()->height())*1.0/table->rowHeight(1);
    number_of_visible_entries=int(guessed_number_of_visible_entries);//before: '+1'
    scroll->setPageStep(number_of_visible_entries);
    scroll->setRange(0,setLength-number_of_visible_entries+12);//'+10' because 10 empty rows at the end
    //cout << "number_of_visible_entries = " << number_of_visible_entries << endl;
    update_entries();
}

void frmSpreadSheet3::wheel_used(int d)//translate mouse-wheel to scroll events
{
    if (d<0)
        scroll->setValue(scroll->value()+1);
    else
        scroll->setValue(scroll->value()-1);
}

void frmSpreadSheet3::position_changed(int pos)
{
    start_row=pos;//only start_row actually changed here!
    update_entries();
}

void frmSpreadSheet3::update_entries(void)//show data in table
{
    if (working==true) return;
    working=true;
    static double guessed_number_of_visible_entries;
    guessed_number_of_visible_entries=(table->size().height()-table->horizontalHeader()->height()-table->horizontalScrollBar()->height())*1.0/table->rowHeight(1);
    number_of_visible_entries=int(guessed_number_of_visible_entries);//'+1'

    disconnect (table,SIGNAL(cellChanged(int,int)), this, SLOT(itemChanged(int,int)));
    char setPrec[5];
    char out_format[16];
    char dummy[128];
    plotarr * p=NULL;
    QString emptyEntry("");
    //get_graph_plotarr(gno, setno, &p);//we do not need a copy here, we only read data
    if (is_valid_gno(gno) == TRUE && is_valid_setno(gno,setno) == TRUE)
    {
        p=g[gno].p+setno;
    }

    /*
int setcolumns=settype_cols(p->type);
if (setcolumns!=number_of_columns)
{
    cout << "wrong column-count: " << setcolumns << "<->" << number_of_columns << endl;
    return;
}
*/

    QStringList vertHeads;
    QStringList horHeads;
    horHeads << QString("X") << QString("Y");
    for (int i=1;i<number_of_columns-1;i++)
    {
        sprintf(dummy,"Y%d",i);
        horHeads << QString(dummy);
    }
    for (int i=0;i<=number_of_visible_entries+5;i++)//we will only update what is visible!
    {
        sprintf(dummy,"%7d",i+start_row);
        vertHeads << QString(dummy);
        /*if (i<number_of_visible_entries)
    table->setRowHidden(i,false);
    else
    table->setRowHidden(i,true);*/
    }
    table->setHorizontalHeaderLabels(horHeads);
    table->setVerticalHeaderLabels(vertHeads);

    if (is_valid_setno(gno,setno) == FALSE || p->data.len<=0)//entries not valid any more since set not valid
    {
        //table->clearContents();
        for (int i=0;i<=number_of_visible_entries+1;i++)//deleting the visible entries
        {
            for (int j=0;j<number_of_columns;j++)
            {
                Items[i][j]->setText(emptyEntry);
            }
        }
    }
    else//valid set
    {
        setLength=getsetlength(gno,setno);
        //if (filled_rows<0) filled_rows=0;
        //else if (filled_rows>100) filled_rows=100;//filled rows is the maximum number of entries to be filled...probably
        for (int col=0;col<number_of_columns;col++)
        {
            table->setColumnHidden(col,false);
            sprintf(setPrec,"%%.%d",col_precision[col]);
            switch (col_format[col])
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
            for (int ro=0;ro<=number_of_visible_entries+5;ro++)//fill the visible rows
            {
                if (ro+start_row<setLength)//fill only entries that are realy present
                {
                    sprintf(dummy,out_format,p->data.ex[col][ro+start_row]);
                    Items[ro][col]->setText(QString(dummy));
                }
                else//otherwise fill in empty entries
                    Items[ro][col]->setText(emptyEntry);
            }
        }
        /*for (int col=number_of_columns;col<6;col++)//--> we have set row- and column-count separately --> this is unneccessary
    table->setColumnHidden(col,true);*/
        table->resizeColumnsToContents();
    }
    connect(table,SIGNAL(cellChanged(int,int)),SLOT(itemChanged(int,int)));
    updateScrollBar();//we have to update this, because the number of entries could have changed
    working=false;
}

void frmSpreadSheet3::itemChanged(int row,int c)
{
    setLength=getsetlength(gno,setno);
    if (working==true || row>setLength || row<0 || c>table->columnCount() || c<0)
    {//do not disturb while working or with useless indices
        return;
    }
    ///working=true;
    ApplyError=false;
    //cout << "item changed " << row << "|" << c << endl;
    QString dummy1=Items[row][c]->text();
    double dummy2=atof(dummy1.toAscii());
    Datapoint * ps=NULL;
    int * nrs=NULL;
    Datapoint dpoint;
    int r=atoi(table->verticalHeaderItem(row)->text().toAscii().constData());//we determine the real row-number here
    zero_datapoint(&dpoint);
    if ( r < setLength )
    {
        ///Undo-Stuff
        for (int i=0;i<settype_cols(g[gno].p[setno].type);i++)
            dpoint.ex[i]=g[gno].p[setno].data.ex[i][r];
        if (g[gno].p[setno].data.s!=NULL)
            dpoint.s=copy_string(NULL,g[gno].p[setno].data.s[r]);
        dpoint.ex[c]=dummy2;
        ps=new Datapoint[2];
        memcpy(ps,&dpoint,sizeof(Datapoint));
        nrs=new int[2];
        nrs[0]=r;
        DataPointEdited(gno,setno,nrs,1,ps,2);
        delete[] ps;
        delete[] nrs;
        g[gno].p[setno].data.ex[c][r]=dummy2;
    }
    else
    {
        //char dummy[128];
        //char setPrec[5];
        //char out_format[16];
        //int n_rows=(number_of_rows-filled_rows+1)-number_of_rows+r;
        int n_rows=r-setLength+1;
        int ret;
        //cout << "n_rows=" << n_rows << endl;
        for (int i=setLength;i<r;i++)
        {
            ret=add_point_at(gno, setno, i, &dpoint);
        }
        dpoint.ex[c]=dummy2;
        ret=add_point_at(gno, setno, r, &dpoint);
        //filled_rows+=n_rows;
        setLength=getsetlength(gno,setno);
        /*if (number_of_rows>=filled_rows+5)
{
;//enough space
}
else
{//in the current state: number of items is not changes
QTableWidgetItem *** Items_d=new QTableWidgetItem**[number_of_rows];
for (int i=0;i<number_of_rows+n_rows;i++)
Items_d[i]=new QTableWidgetItem*[number_of_columns];
for (int i=0;i<number_of_rows;i++)
{
    for (int j=0;j<number_of_columns;j++)
    Items_d[i][j]=Items[i][j];
    delete[] Items[i];
}
delete[] Items;
Items=new QTableWidgetItem**[number_of_rows+n_rows];

for (int i=0;i<number_of_rows+n_rows;i++)
{
    Items[i]=new QTableWidgetItem*[number_of_columns];
    if (i<number_of_rows)
    {
        for (int j=0;j<number_of_columns;j++)
        Items[i][j]=Items_d[i][j];
    }
    else
    {
        table->insertRow(i);
        for (int j=0;j<number_of_columns;j++)
        {
        Items[i][j]=new QTableWidgetItem(QString(""),QTableWidgetItem::Type);
        table->setItem(i, j, Items[i][j]);
        }
    }
}
}*///end: not enough space

        /*plotarr * p=g[gno].p+setno;
//get_graph_plotarr(gno, setno, &p);
for (int col=0;col<number_of_columns;col++)
{
sprintf(setPrec,"%%.%d",col_precsision[col]);
switch (col_format[col])
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
        for (int ro=number_of_rows;ro<r;ro++)
        {
        sprintf(dummy,out_format,p->data.ex[col][ro]);
        Items[ro][col]->setText(QString(dummy));
        }
}
    if (number_of_rows<filled_rows+5)
    {
    QStringList vertHeads;
    number_of_rows+=n_rows;
        for (int i=0;i<number_of_rows;i++)
        {
        sprintf(dummy,"%7d",i);
        vertHeads << QString(dummy);
        }
    table->setVerticalHeaderLabels(vertHeads);
    }*/
    }


    /*if (start_row<=(r-number_of_visible_entries+1))
{
    cout << "start_row=" << start_row;
    start_row-=(start_row-(r-number_of_visible_entries+1))-1;
    cout << " jetzt muesste ich eigentlich --> " << start_row << endl;

    working=false;
}
cout << "a) start_row=" << start_row << endl;*/
    update_entries();
    //cout << "b) start_row=" << start_row << endl;
    updateScrollBar();
    //cout << "c) start_row=" << start_row << endl;
    int start_sav=start_row;
    spreadSheetEntryChanged();
    start_row=start_sav;
    //cout << "len=" << getsetlength(gno,setno) << " start=" << start_row << " vis=" << number_of_visible_entries << " cur_r=" << r << endl;
    working=false;
}

void frmSpreadSheet3::CreateActions(void)
{
    actClose= new QAction(tr("&Close"), this);
    actClose->setShortcut(tr("Esc"));
    connect(actClose, SIGNAL(triggered()), this, SLOT(doClose()));
    actAddRow= new QAction(tr("&Add row"), this);
    connect(actAddRow, SIGNAL(triggered()), this, SLOT(doAddRow()));
    actDelSelRow= new QAction(tr("&Delete selected rows"), this);
    connect(actDelSelRow, SIGNAL(triggered()), this, SLOT(doDelSelRow()));
    actColumnFormat= new QAction(tr("&Column format..."), this);
    connect(actColumnFormat, SIGNAL(triggered()), this, SLOT(doColumnFormat()));
    actHelpOnContext= new QAction(tr("On conte&xt"), this);
    actHelpOnContext->setShortcut(tr("Shift+F1"));
    connect(actHelpOnContext, SIGNAL(triggered()), this, SLOT(doHelpOnContext()));
    actHelpOnDataSetEditor= new QAction(tr("On datas&et editor"), this);
    connect(actHelpOnDataSetEditor, SIGNAL(triggered()), this, SLOT(doHelpOnDataSetEditor()));
}

void frmSpreadSheet3::doApply(void)
{
    ApplyError=false;
    int r=table->currentRow();
    int c=table->currentColumn();
    itemChanged(r,c);
}

void frmSpreadSheet3::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmSpreadSheet3::doClose(void)
{
    hide();
    emit(spreadSheetClosed(gno,setno));
}

void frmSpreadSheet3::doAddRow(void)
{
    int * sels;
    int nsels;
    int * gnos=new int[3];
    int * snos=new int[3];
    getSelection(&nsels,&sels);
    gnos[0]=gno;
    snos[0]=setno;
    SaveSetStatesPrevious(1,gnos,snos,UNDO_DATA);
    for (int ii=0;ii<nsels;ii++)
    {
        int i=sels[ii]+start_row, k;
        int nrows=setLength, ncols=number_of_columns;// scols;
        char **s;
        Datapoint dpoint;

        if (i > nrows || i < 0){
            errmsg(tr("Selected row out of range").toAscii().constData());
            return;
        }

        zero_datapoint(&dpoint);

        if (i < nrows) {
            for (k = 0; k < ncols; k++) {
                dpoint.ex[k] = *(getcol(gno, setno, k) + i);
            }
            if ((s = get_set_strings(gno, setno)) != NULL) {
                dpoint.s = s[i];
            }
            add_point_at(gno, setno, i + 1, &dpoint);
        } else {
            add_point_at(gno, setno, i, &dpoint);
        }

        update_set_lists(gno);
        init(gno, setno);
        mainWin->mainArea->completeRedraw();
    }
    SetsModified(1,gnos,snos,UNDO_DATA);
    delete[] sels;
    delete[] gnos;
    delete[] snos;
}

void frmSpreadSheet3::doDelSelRow(void)
{
    int i;
    int * srows;
    int nsrows;
    int ndatapoints=getsetlength(gno,setno);
    int * gnos=new int[3];
    int * snos=new int[3];
    gnos[0]=gno;
    snos[0]=setno;
    SaveSetStatesPrevious(1,gnos,snos,UNDO_DATA);
    getSelection(&nsrows,&srows);
    char buf[64];
    if (srows[0] < 0 || srows[0] >= setLength) {
        errmsg(tr("Nothing to delete").toAscii().constData());
        return;
    }
    sprintf(buf, "Delete %d selected row(s)?", nsrows);
    if (yesno(buf, NULL, NULL, NULL)) {
        for (i = nsrows - 1; i >= 0; i--) {
            if (srows[i]+start_row>=0 && srows[i]+start_row<ndatapoints)
                del_point(gno,setno,srows[i]+start_row);
        }
        update_set_lists(gno);
        init(gno, setno);
        mainWin->mainArea->completeRedraw();
    }
    delete[] srows;
    SetsModified(1,gnos,snos,UNDO_DATA);
    delete[] gnos;
    delete[] snos;
}

void frmSpreadSheet3::doColumnFormat(void)
{
    EditColumn->initToColumn(0);
    EditColumn->show();
    EditColumn->raise();
}

void frmSpreadSheet3::doHelpOnContext(void)
{
    setCursor(QCursor(Qt::WhatsThisCursor));
}

void frmSpreadSheet3::doHelpOnDataSetEditor(void)
{
    HelpCB("doc/UsersGuide.html#SSEditor");
}

void frmSpreadSheet3::write_set(void)
{
    plotarr *p=g[gno].p+setno;
    QString dummy1;
    double dummy2;
    for (int col=0;col<number_of_columns;col++)
    {
        for (int ro=0;ro<number_of_visible_entries;ro++)
        {
            if (ro+start_row>=getsetlength(gno,setno)) break;
            dummy1=Items[ro][col]->text();
            dummy2=atof(dummy1.toAscii());
            p->data.ex[col][ro+start_row]=dummy2;
        }
    }
    spreadSheetEntryChanged();
}

void frmSpreadSheet3::spreadSheetEntryChanged(void)
{
    ledComment->SetMemoryToText(g[gno].p[setno].comments,g[gno].p[setno].orig_comments);
    //static string used
    //char * buf=new char[ledComment->text().length()+8];
    //strcpy(buf,ledComment->text().toAscii().constData());
    //setcomment(gno,setno,buf);
    set_dirtystate();
    mainWin->mainArea->completeRedraw();
    //delete[] buf;
}

void frmSpreadSheet3::getSelection(int * n_of_sel,int ** sel)
{
    int nsel=10;
    int * sels=new int[nsel];
    int * old_sel;
    bool row_active;
    *n_of_sel=0;
    for (int i=0;i<MAX_NUMBER_OF_LINES_IN_SPREADSHEET;i++)//look in every line that has been allocated
    {
        row_active=false;
        for (int j=0;j<number_of_columns;j++)
        {
            if (Items[i][j]->isSelected()==true)
            {
                row_active=true;
                break;
            }
        }
        if (row_active==true)
        {//new selected row found
            if (nsel==*n_of_sel)
            {//not enough data for selection-storage
                old_sel=sels;
                sels=new int[nsel+30];
                nsel+=30;
                for (int k=0;k<*n_of_sel;k++)
                    sels[k]=old_sel[k];
                delete[] old_sel;
            }
            sels[*n_of_sel]=i;
            (*n_of_sel)++;
        }
    }
    *sel=new int[*n_of_sel];
    for (int i=0;i<*n_of_sel;i++)
        (*sel)[i]=sels[i];
    delete[] sels;
}

void frmSpreadSheet3::updateScrollBar(void)
{
    disconnect(scroll,SIGNAL(valueChanged(int)),this,SLOT(position_changed(int)));
    //scroll->setRange(0,getsetlength(gno,setno)-number_of_visible_entries+1);
    scroll->setRange(0,getsetlength(gno,setno)-1);
    scroll->setValue(start_row);
    scroll->setPageStep(number_of_visible_entries);
    connect(scroll,SIGNAL(valueChanged(int)),SLOT(position_changed(int)));
}

void frmSpreadSheet3::changeDirection(int keynr)
{
    if (working==true) return;
    working=true;
    nextScroll=scroll->value();
    nextRow=table->currentRow();
    nextCol=table->currentColumn();
    //cout << "start_row=" << start_row << " current_row=" << table->currentRow() << " scroll=" << scroll->value() << endl;
    switch (keynr)
    {
    case Qt::Key_Tab:
        //cout << "Tab" << endl;
        StepRight(nextRow,nextCol);
        break;
    case Qt::Key_Backtab:
        //cout << "BackTab" << endl;
        StepLeft(nextRow,nextCol);
        break;
        /*case Qt::Key_Return:
break;
case Qt::Key_Enter:
break;*/
    case Qt::Key_PageUp:
        nextScroll-=number_of_visible_entries-1;
        break;
    case Qt::Key_PageDown:
        nextScroll+=number_of_visible_entries-1;
        break;
    case Qt::Key_Down:
        if (table->currentRow()<number_of_visible_entries)
        {
            nextRow++;
            //cout << "new row" << endl;
        }
        else
        {
            nextScroll++;
            //cout << "new scroll "<< nextScrollValue << endl;
        }
        break;
    case Qt::Key_Up:
        if (table->currentRow()>0)
            nextRow--;
        else
            nextScroll--;
        break;
    case Qt::Key_Home:
        nextRow=0;
        nextScroll=0;
        break;
    case Qt::Key_End:
        nextRow=number_of_visible_entries-2;
        nextScroll=scroll->maximum();
        break;
        //defaut:
        //cout << keynr << endl;
        break;
    }

    if (nextRow!=table->currentRow() || nextCol!=table->currentColumn())
        table->setCurrentCell(nextRow,nextCol);
    working=false;
    if (nextScroll<0) nextScroll=0;
    if (nextScroll!=scroll->value())
        scroll->setValue(nextScroll);


}

void frmSpreadSheet3::currentCellChangedWrapper(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    bool setEdit;
    if (working==true) return;
    working=true;
    setEdit=false;
    nextRow=currentRow;
    nextCol=currentColumn;
    nextScroll=scroll->value();
    ///cout << "(" << previousRow << "|" << previousColumn << ") --> (" << currentRow << "|" << currentColumn << ") vis_entr.=" << number_of_visible_entries << endl;
    if (currentRow>previousRow && currentRow>=number_of_visible_entries)
    {
        nextRow=number_of_visible_entries-1;
        nextScroll+=currentRow-previousRow;
        //cout << "Row hat Bereich nach unten verlassen!" << endl;
        setEdit=true;
        if (nextRow!=table->currentRow() || nextCol!=table->currentColumn())
            table->setCurrentCell(nextRow,nextCol);
    }
    else if (currentRow<previousRow && currentRow==0 && scroll->value()>0)
    {
        nextRow=1;
        nextScroll+=currentRow-previousRow;
        //cout << "Gone up" << endl;
        setEdit=true;
        if (nextRow!=table->currentRow() || nextCol!=table->currentColumn())
            table->setCurrentCell(nextRow,nextCol);
    }
    working=false;
    if (nextScroll<0) nextScroll=0;
    if (nextScroll!=scroll->value())
        scroll->setValue(nextScroll);
    if (setEdit==true)
        table->editItem(Items[nextRow][nextCol]);
}

void frmSpreadSheet3::StepLeft(int curRow,int curCol)//rows are in visible coordinates!
{
    nextRow=curRow;
    nextCol=curCol-1;
    nextScroll=scroll->value();
    if (nextCol<0)
    {
        nextCol=number_of_columns-1;
        nextRow--;
    }
    //column-changes are always within visible range / row-changes only sometimes
    if (nextRow<0)//row would leave visible area now
    {
        nextRow=0;//set actual row to last visible one
        nextScroll--;//scroll contents down
    }
    /// actually do something here
}

void frmSpreadSheet3::StepRight(int curRow,int curCol)//rows are in visible coordinates!
{
    nextRow=curRow;
    nextCol=curCol+1;
    nextScroll=scroll->value();
    if (nextCol>=number_of_columns)
    {
        nextCol=0;
        nextRow++;
    }
    //column-changes are always within visible range / row-changes only sometimes
    if (nextRow>=number_of_visible_entries)//row would leave visible area now
    {
        nextRow=number_of_visible_entries-1;//set actual row to last visible one
        nextScroll++;//scroll contents down
    }
    /// actually do something here
}

frmLine_Props::frmLine_Props(QWidget * parent,bool edit):QWidget(parent)
{
    editWindow=edit;
    int number;
    QString * entr=new QString[5];

    /*setFont(stdFont);
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

    lines_loc_item=new PositionSelector(this);

    buttonGroup=new stdButtonGroup(this,false);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));

    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout2=new QVBoxLayout;
    layout2->setMargin(STD_MARGIN);
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
    connect(lines_width_item,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(lines_style_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));

    connect(lines_loc_item->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(viewCoordsChanged(int)));
}

void frmLine_Props::init(int id)
{
    bool sav_imm_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    char buf[256];
    obj_id=id;
    disconnect(lines_loc_item->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(viewCoordsChanged(int)));
    if (editWindow==false)
    {
        SetOptionChoice(lines_color_item, line_color);
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
        SetOptionChoice(lines_color_item, lines[lineno].color);
        SetOptionChoice(lines_style_item, lines[lineno].lines);
        SetSpinChoice(lines_width_item, lines[lineno].linew);
        SetChoice(lines_arrow_item, lines[lineno].arrow_end);
        SetChoice(lines_atype_item, lines[lineno].arrow.type);
        SetSpinChoice(lines_asize_item, lines[lineno].arrow.length);
        SetSpinChoice(lines_a_dL_ff_item, lines[lineno].arrow.dL_ff);
        SetSpinChoice(lines_a_lL_ff_item, lines[lineno].arrow.lL_ff);
        lines_loc_item->setCurrentIndex(lines[lineno].loctype == COORD_VIEW ? 1 : 0);
        sprintf(buf, "%.12f", lines[lineno].x1);
        xv_setstr(ledCoords[0], buf);
        sprintf(buf, "%.12f", lines[lineno].y1);
        xv_setstr(ledCoords[1], buf);
        sprintf(buf, "%.12f", lines[lineno].x2);
        xv_setstr(ledCoords[2], buf);
        sprintf(buf, "%.12f", lines[lineno].y2);
        xv_setstr(ledCoords[3], buf);
    }
    connect(lines_loc_item->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(viewCoordsChanged(int)));
    immediateUpdate=sav_imm_upd;
    updateRunning=false;
}

void frmLine_Props::viewCoordsChanged(int i)
{
    int lineno = obj_id;

    if (editWindow==false) return;
    if( lines[lineno].loctype == lines_loc_item->currentValue()) return;

    bool old_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    if( lines[lineno].loctype == COORD_VIEW )
    {
        lines[lineno].gno = get_cg();
        lines[lineno].loctype = COORD_WORLD;
        view2world( atof(xv_getstr(ledCoords[0])), atof(xv_getstr(ledCoords[1])),
                &lines[lineno].x1,&lines[lineno].y1 );
        view2world( atof(xv_getstr(ledCoords[2])), atof(xv_getstr(ledCoords[3])),
                &lines[lineno].x2,&lines[lineno].y2 );
    }
    else
    {
        lines[lineno].loctype = COORD_VIEW;
        world2view( atof(xv_getstr(ledCoords[0])), atof(xv_getstr(ledCoords[1])),
                &lines[lineno].x1,&lines[lineno].y1 );
        world2view( atof(xv_getstr(ledCoords[2])), atof(xv_getstr(ledCoords[3])),
                &lines[lineno].x2,&lines[lineno].y2 );
    }
    init(obj_id);
    immediateUpdate=old_upd;
    updateRunning=false;
}

void frmLine_Props::doAccept(void)
{
    int lineno = obj_id;
    double k[4];
    bool changes=false;
    ListOfChanges.clear();
    ListOfOldStates.clear();
    if (editWindow==false)//edit preferences
    {
        line_asize = GetSpinChoice(lines_asize_item);
        line_color = GetOptionChoice(lines_color_item);
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
        ObjectDataModified(lineno,OBJECT_LINE);
        set_dirtystate();
        if (GlobalInhibitor==false)
            mainWin->mainArea->completeRedraw();
        if (changes==false)
        {
            ListOfChanges.clear();
            ListOfOldStates.clear();
        }
    }
}

void frmLine_Props::doClose(void)
{
    //hide();
    parentWidget()->hide();
}

void frmLine_Props::update0(void)
{
    static int i,nr,errpos;
    if (!immediateUpdate) return;
    bool sav_im_up;
    sav_im_up=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    GlobalInhibitor=true;
    doAccept();//do this without actually doing something
    nr=nr_of_true_changes(ListOfChanges);
    errpos=0;
    if (nr>0)
    {
        for (i=0;i<ListOfChanges.size();i++)
        {
            strcpy(command,ListOfChanges.at(i).toAscii().constData());
            errpos = scanner(command);
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
    update0();
}

void frmLine_Props::update2(QString v)
{
    update0();
}

void frmLine_Props::update3(bool v)
{
    update0();
}

void frmLine_Props::update4(double v)
{
    update0();
}

frmLineProps::frmLineProps(QWidget * parent,bool edit):QDialog(parent)
{
    setFont(stdFont);
    if (edit)
        setWindowTitle(tr("Edit Line"));
    else
        setWindowTitle(tr("Lines"));
    setWindowIcon(QIcon(*GraceIcon));

    QVBoxLayout * layout=new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    flp=new frmLine_Props(this,edit);
    layout->addWidget(flp);
    setLayout(layout);
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
    //setFont(stdFont);
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
    buttonGroup=new stdButtonGroup(this,false);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));

    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(ellip_color_item);
    layout->addWidget(ellip_linew_item);
    layout->addWidget(ellip_lines_item);
    layout->addWidget(ellip_fillpat_item);
    layout->addWidget(ellip_fillcol_item);
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

    connect(ellip_color_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(ellip_linew_item,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(ellip_lines_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(ellip_fillpat_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(ellip_fillcol_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(ellip_rot_item,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    //do not use the following for immediate updates!!
    //connect(ellip_loc_item->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(ellip_loc_item->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),SLOT(viewCoordsChanged(int)));
}

void frmEllipse_Props::init(int id)
{
    bool sav_imm_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    char buf[256];
    obj_id=id;
    disconnect(ellip_loc_item->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(viewCoordsChanged(int)));
    if (editWindow==false)
    {
        if (ellipse==false)
        {
            SetOptionChoice(ellip_color_item, box_color);
            SetOptionChoice(ellip_lines_item, box_lines);
            SetSpinChoice(ellip_linew_item, box_linew);
            SetOptionChoice(ellip_fillpat_item, box_fillpat);
            SetOptionChoice(ellip_fillcol_item, box_fillcolor);
            ellip_rot_item->setValue(0);
            ellip_loc_item->setCurrentIndex(box_loctype == COORD_VIEW ? 1 : 0);
        }
        else
        {
            SetOptionChoice(ellip_color_item, ellipse_color);
            SetOptionChoice(ellip_lines_item, ellipse_lines);
            SetSpinChoice(ellip_linew_item, ellipse_linew);
            SetOptionChoice(ellip_fillpat_item, ellipse_fillpat);
            SetOptionChoice(ellip_fillcol_item, ellipse_fillcolor);
            ellip_rot_item->setValue(0);
            ellip_loc_item->setCurrentIndex(ellipse_loctype == COORD_VIEW ? 1 : 0);
        }
    }
    else
    {
        int ellipno =id;
        if (ellipse==true)
        {
            SetOptionChoice(ellip_color_item, ellip[ellipno].color);
            SetOptionChoice(ellip_lines_item, ellip[ellipno].lines);
            SetSpinChoice(ellip_linew_item, ellip[ellipno].linew);
            SetOptionChoice(ellip_fillpat_item, ellip[ellipno].fillpattern);
            SetOptionChoice(ellip_fillcol_item, ellip[ellipno].fillcolor);
            ellip_rot_item->setValue(ellip[ellipno].rot);
            ellip_loc_item->setCurrentIndex(ellip[ellipno].loctype == COORD_VIEW ? 1 : 0);
            sprintf(buf, "%.12f", 0.5*(ellip[ellipno].x1+ellip[ellipno].x2));
            xv_setstr(ledCoords[0], buf);
            sprintf(buf, "%.12f", 0.5*(ellip[ellipno].y1+ellip[ellipno].y2));
            xv_setstr(ledCoords[1], buf);
            sprintf(buf, "%.12f", fabs(ellip[ellipno].x1-ellip[ellipno].x2) );
            xv_setstr(ledCoords[2], buf);
            sprintf(buf, "%.12f", fabs(ellip[ellipno].y1-ellip[ellipno].y2) );
            xv_setstr(ledCoords[3], buf);
        }
        else//box
        {
            SetOptionChoice(ellip_color_item, boxes[ellipno].color);
            SetOptionChoice(ellip_lines_item, boxes[ellipno].lines);
            SetSpinChoice(ellip_linew_item, boxes[ellipno].linew);
            SetOptionChoice(ellip_fillpat_item, boxes[ellipno].fillpattern);
            SetOptionChoice(ellip_fillcol_item, boxes[ellipno].fillcolor);
            ellip_rot_item->setValue(boxes[ellipno].rot);
            ellip_loc_item->setCurrentIndex(boxes[ellipno].loctype == COORD_VIEW ? 1 : 0);
            sprintf(buf, "%.12f", boxes[ellipno].x1);
            xv_setstr(ledCoords[0], buf);
            sprintf(buf, "%.12f", boxes[ellipno].y1);
            xv_setstr(ledCoords[1], buf);
            sprintf(buf, "%.12f", boxes[ellipno].x2);
            xv_setstr(ledCoords[2], buf);
            sprintf(buf, "%.12f", boxes[ellipno].y2);
            xv_setstr(ledCoords[3], buf);
        }
    }
    connect(ellip_loc_item->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(viewCoordsChanged(int)));
    immediateUpdate=sav_imm_upd;
    updateRunning=false;
}

void frmEllipse_Props::viewCoordsChanged(int i)
{
    int boxno = obj_id;
    int ellipno = obj_id;
    double x1, x2, y1, y2;
    if (editWindow==false) return;//there are no coordinates visible --> nothing to do
    bool old_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    if (ellipse==false)//it is a box
    {
        if( ellip_loc_item->currentValue() == boxes[boxno].loctype )
        {
            return;
        }

        if( boxes[boxno].loctype == COORD_VIEW )
        {
            boxes[boxno].gno = get_cg();
            boxes[boxno].loctype = COORD_WORLD;
            view2world( atof(xv_getstr(ledCoords[0])), atof(xv_getstr(ledCoords[1])),
                    &boxes[boxno].x1,&boxes[boxno].y1 );
            view2world( atof(xv_getstr(ledCoords[2])), atof(xv_getstr(ledCoords[3])),
                    &boxes[boxno].x2,&boxes[boxno].y2 );
        }
        else
        {
            boxes[boxno].loctype = COORD_VIEW;
            world2view( atof(xv_getstr(ledCoords[0])), atof(xv_getstr(ledCoords[1])),
                    &boxes[boxno].x1,&boxes[boxno].y1 );
            world2view( atof(xv_getstr(ledCoords[2])), atof(xv_getstr(ledCoords[3])),
                    &boxes[boxno].x2,&boxes[boxno].y2 );
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

        if( ellip[ellipno].loctype == COORD_VIEW ) {
            ellip[ellipno].gno = get_cg();
            ellip[ellipno].loctype = COORD_WORLD;
            view2world( x1-x2/2., y1-y2/2., &ellip[ellipno].x1,&ellip[ellipno].y1 );
            view2world( x1+x2/2., y1+y2/2., &ellip[ellipno].x2,&ellip[ellipno].y2 );
        } else {
            ellip[ellipno].loctype = COORD_VIEW;
            world2view( x1-x2/2., y1-y2/2., &ellip[ellipno].x1,&ellip[ellipno].y1 );
            world2view( x1+x2/2., y1+y2/2., &ellip[ellipno].x2,&ellip[ellipno].y2 );
        }
    }
    init(obj_id);
    immediateUpdate=old_upd;
    updateRunning=false;
}

void frmEllipse_Props::doAccept(void)
{
    int ellipno = obj_id;
    double a, b, c, d;
    bool changes=false;
    ListOfChanges.clear();
    ListOfOldStates.clear();
    if (editWindow==false)//edit preferences
    {
        if (ellipse==true)
        {
            ellipse_color = GetOptionChoice(ellip_color_item);
            ellipse_loctype = ellip_loc_item->currentValue();
            ellipse_lines = GetOptionChoice(ellip_lines_item);
            ellipse_linew = GetSpinChoice(ellip_linew_item);
            ellipse_fillcolor = GetOptionChoice(ellip_fillcol_item);
            ellipse_fillpat = GetOptionChoice(ellip_fillpat_item);
        }
        else
        {
            box_color = GetOptionChoice(ellip_color_item);
            box_loctype = ellip_loc_item->currentValue();
            box_lines = GetOptionChoice(ellip_lines_item);
            box_linew = GetSpinChoice(ellip_linew_item);
            box_fillcolor = GetOptionChoice(ellip_fillcol_item);
            box_fillpat = GetOptionChoice(ellip_fillpat_item);
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
            if (ellip[ellipno].x1 != a - b/2. || ellip[ellipno].x2 != a + b/2. || ellip[ellipno].y1 != c - d/2. || ellip[ellipno].y2 != c + d/2.)
            {
                changes=true;
            }
            sprintf(dummy,"    ellipse %f, %f, %f, %f",a - b/2.,c - d/2.,a + b/2.,c + d/2.);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    ellipse %f, %f, %f, %f",ellip[ellipno].x1,ellip[ellipno].y1,ellip[ellipno].x2,ellip[ellipno].y2);
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
            ellip[ellipno].loctype = ellip_loc_item->currentValue();
            ellip[ellipno].lines = GetOptionChoice(ellip_lines_item);
            ellip[ellipno].linew = GetSpinChoice(ellip_linew_item);
            ellip[ellipno].fillcolor = GetOptionChoice(ellip_fillcol_item);
            ellip[ellipno].fillpattern = GetOptionChoice(ellip_fillpat_item);
            ellip[ellipno].rot = ellip_rot_item->value();
            ellip[ellipno].x1 = a - b/2.;
            ellip[ellipno].x2 = a + b/2.;
            ellip[ellipno].y1 = c - d/2.;
            ellip[ellipno].y2 = c + d/2.;
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
            if (boxes[ellipno].x1 != a || boxes[ellipno].x2 != b || boxes[ellipno].y1 != c || boxes[ellipno].y2 != d)
            {
                changes=true;
            }
            sprintf(dummy,"    box %f, %f, %f, %f",a,c,b,d);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    box %f, %f, %f, %f",boxes[ellipno].x1,boxes[ellipno].y1,boxes[ellipno].x2,boxes[ellipno].y2);
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
            boxes[ellipno].loctype = ellip_loc_item->currentValue();
            boxes[ellipno].lines = GetOptionChoice(ellip_lines_item);
            boxes[ellipno].linew = GetSpinChoice(ellip_linew_item);
            boxes[ellipno].fillcolor = GetOptionChoice(ellip_fillcol_item);
            boxes[ellipno].fillpattern = GetOptionChoice(ellip_fillpat_item);
            boxes[ellipno].rot = ellip_rot_item->value();
            boxes[ellipno].x1 = a;
            boxes[ellipno].x2 = b;
            boxes[ellipno].y1 = c;
            boxes[ellipno].y2 = d;
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
}

void frmEllipse_Props::doClose(void)
{
    parentWidget()->hide();
}

void frmEllipse_Props::update0(void)
{
    static int i,nr,errpos;
    static int sav_settings_i[10];
    static double sav_settings_d[2];
    bool imm_upd_sav;
    if (!immediateUpdate) return;
    imm_upd_sav=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    GlobalInhibitor=true;
    doAccept();//do this without actually doing something
    nr=nr_of_true_changes(ListOfChanges);
    errpos=0;
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

        for (i=0;i<ListOfChanges.size();i++)
        {
            strcpy(command,ListOfChanges.at(i).toAscii().constData());
            errpos = scanner(command);
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
    update0();
}

void frmEllipse_Props::update2(QString v)
{
    update0();
}

void frmEllipse_Props::update3(bool v)
{
    update0();
}

void frmEllipse_Props::update4(double v)
{
    update0();
}

frmEllipseProps::frmEllipseProps(QWidget * parent,bool edit,bool ellip):QDialog(parent)
{
    setFont(stdFont);
    if (edit)
    {
        if (ellip)
            setWindowTitle(tr("Edit ellipse"));
        else
            setWindowTitle(tr("Edit box"));
    }
    else
    {
        if (ellip)
            setWindowTitle(tr("Ellipses"));
        else
            setWindowTitle(tr("Boxes"));
    }
    setWindowIcon(QIcon(*GraceIcon));

    QVBoxLayout * layout=new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    flp=new frmEllipse_Props(this,edit,ellip);
    layout->addWidget(flp);
    setLayout(layout);
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
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Point explorer"));
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
    layout1->setMargin(STD_MARGIN);
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
    layout->setMargin(STD_MARGIN);
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
}

void frmPointExplorer::selectionChanged(int a)
{
    int count;
    int * numbers=new int[2];
    list->get_selection(&count,&numbers);
    if (count == 1) {
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
    if (get_point(cg, track_setno, ind, &wp) == RETURN_SUCCESS) {
        vp = Wpoint2Vpoint(wp);
        setpointer(vp);
    } else {
        errmsg(tr("Point index out of range").toAscii().constData());
    }
}

void frmPointExplorer::doTrack(void)
{
    int * selection=new int[3];
    int number;
    list->get_selection(&number,&selection);
    if (number!=1)
        return;
    else
        track_setno=selection[0];
    set_action(DO_NOTHING);
    SetLabel(lblInstructions, "Track points");
    set_action(TRACKER);
}

void frmPointExplorer::update_point_locator(int gno, int setno, int loc)
{
    int col, ncols;
    Datapoint dpoint;
    char *s=new char[512], buf[64];
    if (get_datapoint(gno, setno, loc, &ncols, &dpoint) == RETURN_SUCCESS) {
        SelectListChoice(list,setno);
        //s = copy_string(NULL, "(");
        strcpy(s,"(");
        for (col = 0; col < ncols; col++) {
            sprintf(buf, "%g", dpoint.ex[col]);
            //s = concat_strings(s, buf);
            strcat(s,buf);
            if (col != ncols - 1) {
                //s = concat_strings(s, ", ");
                strcat(s,", ");
            }
        }
        if (dpoint.s != NULL) {
            strcat(s,",\"");
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
    } else {
        track_setno = -1;
        ///SelectListChoices(track_set_sel, 0, NULL);
        SelectListChoices(list,0,NULL);
        SetTextString(ledPointData, "");
        xv_setstr(ledPointLocation, "");
    }
}

void frmPointExplorer::doMove(void)
{
    set_action(DO_NOTHING);
    track_move_dir = MOVE_POINT_XY;
    SetLabel(lblInstructions, "Move points");
    set_action(MOVE_POINT1ST);
}

void frmPointExplorer::doMoveX(void)
{
    set_action(DO_NOTHING);
    track_move_dir = MOVE_POINT_X;
    SetLabel(lblInstructions, "Move points along x");
    set_action(MOVE_POINT1ST);
}

void frmPointExplorer::doMoveY(void)
{
    set_action(DO_NOTHING);
    track_move_dir = MOVE_POINT_Y;
    SetLabel(lblInstructions, "Move points along y");
    set_action(MOVE_POINT1ST);
}

void frmPointExplorer::doPrepend(void)
{
    set_action(DO_NOTHING);
    track_add_at = ADD_POINT_BEGINNING;
    SetLabel(lblInstructions, "Add points at the beginning of set");
    set_action(ADD_POINT);
}

void frmPointExplorer::doAppend(void)
{
    set_action(DO_NOTHING);
    track_add_at = ADD_POINT_END;
    SetLabel(lblInstructions, "Add points to the end of set");
    set_action(ADD_POINT);
}

void frmPointExplorer::doInsert(void)
{
    set_action(DO_NOTHING);
    track_add_at = ADD_POINT_NEAREST;
    SetLabel(lblInstructions, "Add points to the nearest position");
    set_action(ADD_POINT);
}

void frmPointExplorer::doDelete(void)
{
    set_action(DO_NOTHING);
    SetLabel(lblInstructions, "Delete points");
    set_action(DEL_POINT);
}

void frmPointExplorer::doClose(void)
{
    set_action(DO_NOTHING);
    hide();
}

frmText_Props::frmText_Props(QWidget * parent,bool edit):QWidget(parent)
{
    obj_id=-1;
    editWindow=edit;
    /*setFont(stdFont);
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
    layout->setMargin(STD_MARGIN);
    if (edit)
    {
        string_item=new stdLineEdit(this,QString("String:"),true);
        layout->addWidget(string_item);
        layout->addWidget(strings_color_item);
        layout->addWidget(strings_font_item);
    }
    else//properties in general
    {
        layout->addWidget(strings_font_item);
        layout->addWidget(strings_color_item);
    }
    layout->addWidget(strings_just_item);
    layout->addWidget(strings_loc_item);
    if (edit)
    {
        ledCoords[0]=new stdLineEdit(this,QString("X="));
        layout->addWidget(ledCoords[0]);
        ledCoords[1]=new stdLineEdit(this,QString("Y="));
        layout->addWidget(ledCoords[1]);
    }
    layout->addWidget(strings_rot_item);
    layout->addWidget(strings_size_item);
    layout->addWidget(buttonGroup);
    setLayout(layout);
    strings_size_item->setValue(100);
    strings_color_item->setCurrentIndex(1);
    strings_loc_item->setCurrentIndex(1);
    if (edit)
        connect(string_item->lenText,SIGNAL(returnPressed()),SLOT(update0()));
    connect(strings_font_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(strings_color_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(strings_just_item->cmbJustSelect,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(strings_loc_item->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    for (int i=0;i<2;i++)
        connect(ledCoords[i]->lenText,SIGNAL(returnPressed()),SLOT(update0()));
    connect(strings_rot_item,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(strings_size_item,SIGNAL(valueChanged(int)),SLOT(update1(int)));

    connect(strings_loc_item->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(viewCoordsChanged(int)));
}

void frmText_Props::init(int id)
{
    bool sav_imm_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    char buf[1024];
    plotstr *pstring;
    obj_id=id;
    disconnect(strings_loc_item->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(viewCoordsChanged(int)));
    if (editWindow==false)
    {
        SetOptionChoice(strings_font_item, string_font);
        SetOptionChoice(strings_color_item, string_color);
        SetCharSizeChoice(strings_size_item, string_size*100);
        SetAngleChoice(strings_rot_item, string_rot);
        strings_loc_item->setCurrentIndex(string_loctype == COORD_VIEW ? 1 : 0);
        SetOptionChoice(strings_just_item, string_just);
    }
    else
    {
        pstring = &pstr[obj_id];
        //SetTextString(string_item, pstring->s);
        string_item->SetTextToMemory(pstring->s_plotstring,pstring->alt_plotstring);
        SetOptionChoice(strings_color_item, pstring->color);
        SetOptionChoice(strings_just_item, pstring->just);
        SetOptionChoice(strings_font_item, pstring->font );
        SetCharSizeChoice(strings_size_item, pstring->charsize*100);
        SetAngleChoice(strings_rot_item, pstring->rot);
        strings_loc_item->setCurrentIndex(pstring->loctype == COORD_VIEW ? 1 : 0);
        sprintf(buf, "%.12f", pstring->x);
        xv_setstr(ledCoords[0], buf);
        sprintf(buf, "%.12f", pstring->y);
        xv_setstr(ledCoords[1], buf);
    }
    connect(strings_loc_item->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(viewCoordsChanged(int)));
    immediateUpdate=sav_imm_upd;
    updateRunning=false;
}

void frmText_Props::viewCoordsChanged(int i)
{
    int stringno = obj_id;
    if (editWindow==false) return;
    bool old_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    if(pstr[stringno].loctype == strings_loc_item->currentValue())
    {
        return;
    }
    if( pstr[stringno].loctype == COORD_VIEW ) {
        pstr[stringno].gno = get_cg();
        pstr[stringno].loctype = COORD_WORLD;
        view2world( atof(xv_getstr(ledCoords[0])), atof(xv_getstr(ledCoords[1])),
                &pstr[stringno].x, &pstr[stringno].y );
    }
    else
    {
        pstr[stringno].loctype = COORD_VIEW;
        world2view( atof(xv_getstr(ledCoords[0])), atof(xv_getstr(ledCoords[1])),
                &pstr[stringno].x, &pstr[stringno].y );
    }
    init(obj_id);
    immediateUpdate=old_upd;
    updateRunning=false;
}

void frmText_Props::doAccept(void)
{
    if (editWindow==false)//edit preferences
    {
        string_font = GetOptionChoice(strings_font_item);
        string_color = GetOptionChoice(strings_color_item);
        string_size = GetCharSizeChoice(strings_size_item)/100.0;
        string_rot = GetAngleChoice(strings_rot_item);
        string_loctype = GetChoice(strings_loc_item) ? COORD_VIEW : COORD_WORLD;
        string_just = GetOptionChoice(strings_just_item);
    }
    else
    {
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
        ch1=pstr[stringno].alt_plotstring;
        ch2=GetTextString(string_item);
        if (!(ch1==NULL || ch2==NULL))
        {
            if (strcmp(ch1,ch2) || counter>0)
            {
                changes=true;
            }
        }
        else if (ch1!=ch2 || counter>0) changes=true;
        sprintf(dummy,"    string def \"%s\"",GetTextString(string_item));
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    string def \"%s\"",pstr[stringno].alt_plotstring);
        ListOfOldStates << QString(dummy);
        //}
        SaveObjectData(stringno,OBJECT_STRING);
        string_item->DynSetMemoryToText(pstr[stringno].s_plotstring,pstr[stringno].alt_plotstring);
        //pstr[stringno].s = copy_string(pstr[stringno].s, GetTextString(string_item));
        ///strings used
        pstr[stringno].color = GetOptionChoice(strings_color_item);
        pstr[stringno].loctype = GetChoice(strings_loc_item) ? COORD_VIEW : COORD_WORLD;
        pstr[stringno].font = GetOptionChoice(strings_font_item);
        pstr[stringno].just = GetOptionChoice(strings_just_item);
        pstr[stringno].x=d1;
        pstr[stringno].y=d2;
        pstr[stringno].charsize = GetCharSizeChoice(strings_size_item)/100.0;
        pstr[stringno].rot = GetAngleChoice(strings_rot_item);
        ObjectDataModified(stringno,OBJECT_STRING);
        set_dirtystate();
        if (GlobalInhibitor==false)
            mainWin->mainArea->completeRedraw();
        if (changes==false)
        {
            ListOfChanges.clear();
            ListOfOldStates.clear();
        }
    }
}

void frmText_Props::doClose(void)
{
    parentWidget()->hide();
}

//immediate updates
void frmText_Props::update0(void)
{
    static int i,nr,errpos;
    if (!immediateUpdate) return;
    GlobalInhibitor=true;
    ListOfChanges.clear();
    doAccept();//do this without actually doing something
    nr=nr_of_true_changes(ListOfChanges);
    errpos=0;
    if (nr>0)
    {
        for (i=0;i<ListOfChanges.size();i++)
        {
            strcpy(command,ListOfChanges.at(i).toAscii().constData());
            errpos = scanner(command);
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
    update0();
}

void frmText_Props::update2(QString v)
{
    update0();
}

void frmText_Props::update3(bool v)
{
    update0();
}

void frmText_Props::update4(double v)
{
    update0();
}

frmTextProps::frmTextProps(QWidget * parent,bool edit):QDialog(parent)
{
    setFont(stdFont);
    if (edit)
        setWindowTitle(tr("Edit String"));
    else
        setWindowTitle(tr("Strings"));
    setWindowIcon(QIcon(*GraceIcon));
    QVBoxLayout * layout=new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    flp=new frmText_Props(this,edit);
    layout->addWidget(flp);
    setLayout(layout);
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
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Objects"));
    setWindowIcon(QIcon(*GraceIcon));
    cmdText=new QPushButton(tr("Text"));
    connect(cmdText,SIGNAL(clicked()),SLOT(doText()));
    cmdTextProp=new QPushButton(tr("Text props..."));
    connect(cmdTextProp,SIGNAL(clicked()),SLOT(doTextProp()));
    cmdLine=new QPushButton(tr("Line"));
    connect(cmdLine,SIGNAL(clicked()),SLOT(doLine()));
    cmdLineProp=new QPushButton(tr("Line props..."));
    connect(cmdLineProp,SIGNAL(clicked()),SLOT(doLineProp()));
    cmdBox=new QPushButton(tr("Box"));
    connect(cmdBox,SIGNAL(clicked()),SLOT(doBox()));
    cmdBoxProp=new QPushButton(tr("Box props..."));
    connect(cmdBoxProp,SIGNAL(clicked()),SLOT(doBoxProp()));
    cmdEllipse=new QPushButton(tr("Ellipse"));
    connect(cmdEllipse,SIGNAL(clicked()),SLOT(doEllipse()));
    cmdEllipseProp=new QPushButton(tr("Ellipse props..."));
    connect(cmdEllipseProp,SIGNAL(clicked()),SLOT(doEllipseProp()));
    cmdEditObj=new QPushButton(tr("Edit object"));
    connect(cmdEditObj,SIGNAL(clicked()),SLOT(doEditObj()));
    cmdMoveObj=new QPushButton(tr("Move object"));
    connect(cmdMoveObj,SIGNAL(clicked()),SLOT(doMoveObj()));
    cmdCopyObj=new QPushButton(tr("Copy object"));
    connect(cmdCopyObj,SIGNAL(clicked()),SLOT(doCopyObj()));
    cmdDelObj=new QPushButton(tr("Delete object"));
    connect(cmdDelObj,SIGNAL(clicked()),SLOT(doDelObj()));
    cmdClearAllText=new QPushButton(tr("Clear all text"));
    connect(cmdClearAllText,SIGNAL(clicked()),SLOT(doClearAllText()));
    cmdClearAllLines=new QPushButton(tr("Clear all lines"));
    connect(cmdClearAllLines,SIGNAL(clicked()),SLOT(doClearAllLines()));
    cmdClearAllBoxes=new QPushButton(tr("Clear all boxes"));
    connect(cmdClearAllBoxes,SIGNAL(clicked()),SLOT(doClearAllBoxes()));
    cmdClearAllEllipses=new QPushButton(tr("Clear all ellipses"));
    connect(cmdClearAllEllipses,SIGNAL(clicked()),SLOT(doClearAllEllipses()));
    cmdClose=new QPushButton(tr("Close"));
    connect(cmdClose,SIGNAL(clicked()),SLOT(doClose()));
    layout=new QGridLayout;
    layout->setMargin(STD_MARGIN);
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
    layout->setColumnMinimumWidth(0,100);
    layout->setColumnMinimumWidth(1,100);
    setLayout(layout);
}

frmDrawObjects::~frmDrawObjects()
{
}

void frmDrawObjects::doText(void)
{
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
    set_action(EDIT_OBJECT);
}

void frmDrawObjects::doMoveObj(void)
{
    set_action(MOVE_OBJECT_1ST);
}

void frmDrawObjects::doCopyObj(void)
{
    set_action(COPY_OBJECT1ST);
}

void frmDrawObjects::doDelObj(void)
{
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

frmPlot_Appearance::frmPlot_Appearance(QWidget * parent):QWidget(parent)
{
    //setFont(stdFont);
    //setWindowTitle(tr("qtGrace: Plot appearance"));
    //setWindowIcon(QIcon(*GraceIcon));

    fraPageBackgr=new QGroupBox(tr("Page background"),this);
    bg_color_item=new ColorSelector(fraPageBackgr);
    bg_color_item->setCurrentIndex(0);
    bg_fill_item=new QCheckBox(tr("Fill"),fraPageBackgr);
    bg_fill_item->setChecked(TRUE);
    layout1=new QHBoxLayout;
    layout1->setMargin(STD_MARGIN);
    layout1->setSpacing(STD_SPACING);
    layout1->addWidget(bg_color_item);
    layout1->addWidget(bg_fill_item);
    fraPageBackgr->setLayout(layout1);

    fraTimeStamp=new QGroupBox(tr("Time stamp"),this);
    timestamp_active_item=new QCheckBox(tr("Enable"),fraTimeStamp);
    timestamp_active_item->setChecked(FALSE);
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
    layout2->setMargin(STD_MARGIN);
    layout2->addWidget(timestamp_active_item);
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
    layout3->setMargin(STD_MARGIN);
    txtDescription=new QTextEdit(fraDescription);
    layout3->addWidget(txtDescription);
    fraDescription->setLayout(layout3);

    buttonGroup=new stdButtonGroup(this);
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));

    connect(timestamp_rotate_item,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(timestamp_size_item,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(bg_fill_item,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(timestamp_active_item,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(bg_color_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(timestamp_color_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(timestamp_font_item,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(ledStampCoords[0],SIGNAL(changed()),SLOT(update0()));
    connect(ledStampCoords[1],SIGNAL(changed()),SLOT(update0()));

    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->setSpacing(STD_SPACING);
    layout->addWidget(fraPageBackgr);
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
    static int i,nr,errpos;
    bool imm_upd_sav;
    if (!immediateUpdate) return;
    imm_upd_sav=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    GlobalInhibitor=true;
    SavePlotAppearance();
    doApply();//do this without actually doing something
    nr=nr_of_true_changes(ListOfChanges);
    errpos=0;
    /*cout << "nr=" << nr << endl;
for (i=0;i<ListOfChanges.size();i++)
cout << ListOfChanges.at(i).toAscii().constData() << endl;*/
    if (nr>0)
    {
        for (i=0;i<ListOfChanges.size();i++)
        {
            strcpy(command,ListOfChanges.at(i).toAscii().constData());
            errpos = scanner(command);
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
    update0();
}

void frmPlot_Appearance::update2(QString v)
{
    update0();
}

void frmPlot_Appearance::update3(bool v)
{
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
    strcpy(nDescr,strDescr.toAscii().constData());
    ///Undo-Stuff
    if (!GlobalInhibitor)
        SavePlotAppearance();

    ListOfChanges.clear();
    ListOfOldStates.clear();

    if (GetOptionChoice(bg_color_item)!=getbgcolor())
    {
        sprintf(dummy,"background color %d",GetOptionChoice(bg_color_item));
        ListOfChanges << QString(dummy);
        sprintf(dummy,"background color %d",getbgcolor());
        ListOfOldStates << QString(dummy);
    }
    if (GetToggleButtonState(bg_fill_item)!=getbgfill())
    {
        sprintf(dummy,"page background fill %s",GetToggleButtonState(bg_fill_item)?"on":"off");
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
    if (!GlobalInhibitor)
    {
        setbgcolor(GetOptionChoice(bg_color_item));
        setbgfill(GetToggleButtonState(bg_fill_item));
        timestamp.active = GetToggleButtonState(timestamp_active_item);
        timestamp.font = GetOptionChoice(timestamp_font_item);
        timestamp.color = GetOptionChoice(timestamp_color_item);
        timestamp.charsize = GetCharSizeChoice(timestamp_size_item)/100.0;
        timestamp.rot = GetAngleChoice(timestamp_rotate_item);
        xv_evalexpr(ledStampCoords[0], &timestamp.x);
        xv_evalexpr(ledStampCoords[1], &timestamp.y);
        set_project_description(nDescr);
        ///Undo-Stuff
        PlotAppearanceModified();
        set_dirtystate();
        mainWin->mainArea->completeRedraw();
        ListOfChanges.clear();
        ListOfOldStates.clear();

    }
    /*for (int i=0;i<ListOfChanges.size();i++)
cout << ListOfChanges.at(i).toAscii().constData() << endl;*/
}

void frmPlot_Appearance::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmPlot_Appearance::doClose(void)
{
    parentWidget()->hide();
}

void frmPlot_Appearance::init(void)
{
    char buf[256];
    SetOptionChoice(bg_color_item, getbgcolor());
    SetToggleButtonState(bg_fill_item, getbgfill());
    SetToggleButtonState(timestamp_active_item, timestamp.active);
    SetOptionChoice(timestamp_font_item, timestamp.font);
    SetOptionChoice(timestamp_color_item, timestamp.color);
    SetCharSizeChoice(timestamp_size_item, timestamp.charsize*100);
    SetAngleChoice(timestamp_rotate_item, timestamp.rot);
    char * oDescr=get_project_description();
    txtDescription->setText(QString(oDescr));
    sprintf(buf, "%g", timestamp.x);
    xv_setstr(ledStampCoords[0], buf);
    sprintf(buf, "%g", timestamp.y);
    xv_setstr(ledStampCoords[1], buf);
}

frmPlotAppearance::frmPlotAppearance(QWidget * parent):QDialog(parent)
{
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Plot appearance"));
    setWindowIcon(QIcon(*GraceIcon));

    QVBoxLayout * layout=new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    flp=new frmPlot_Appearance(this);
    layout->addWidget(flp);
    setLayout(layout);
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


frmLocatorProps::frmLocatorProps(QWidget * parent):QDialog(parent)
{
    int number;
    QString * entr=new QString[33];
    char dummy[10];

    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Locator properties"));
    setWindowIcon(QIcon(*GraceIcon));

    fraXProp=new QGroupBox(tr("X properties"),this);
    fraYProp=new QGroupBox(tr("Y properties"),this);
    fraFixedPoint=new QGroupBox(tr("Fixed Point"),this);

    number=6;
    entr[0]=QString("[X, Y]"),
            entr[1]=QString("[DX, DY]"),
            entr[2]=QString("[DISTANCE]"),
            entr[3]=QString("[Phi, Rho]"),
            entr[4]=QString("[VX, VY]"),
            entr[5]=QString("[SX, SY]"),

            selLocDisplType=new StdSelector(this,tr("Locator display type:"),number,entr);
    number=32;
    for (int i=0;i<number;i++)
    {
        entr[i]=QString(fmt_option_items[i].label);
    }
    selXFormat=new StdSelector(fraXProp,tr("Format:"),number,entr);
    selXFormat->setCurrentIndex(2);
    selYFormat=new StdSelector(fraYProp,tr("Format:"),number,entr);
    selYFormat->setCurrentIndex(2);
    number=10;
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
    ledFixedCoords[0]=new stdLineEdit(fraFixedPoint,tr("X:"));
    ledFixedCoords[1]=new stdLineEdit(fraFixedPoint,tr("Y:"));

    layout1=new QVBoxLayout;
    layout1->setMargin(STD_MARGIN);
    layout1->addWidget(selXFormat);
    layout1->addWidget(selXPrecision);
    fraXProp->setLayout(layout1);
    layout2=new QVBoxLayout;
    layout2->setMargin(STD_MARGIN);
    layout2->addWidget(selYFormat);
    layout2->addWidget(selYPrecision);
    fraYProp->setLayout(layout2);

    layout3=new QGridLayout;
    layout3->setMargin(STD_MARGIN);
    layout3->addWidget(chkEnableFixed,0,0);
    layout3->addWidget(ledFixedCoords[0],1,0);
    layout3->addWidget(ledFixedCoords[1],1,1);
    fraFixedPoint->setLayout(layout3);

    buttonGroup=new stdButtonGroup(this);
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));

    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
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
    delete[] entr;
}

frmLocatorProps::~frmLocatorProps()
{}

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
    else
        get_graph_locator(gno, &locator2);
    SaveLocatorFixPoint(gno);
    locator.pt_type = selLocDisplType->currentIndex();
    locator.fx = selXFormat->currentIndex();
    locator.fy = selYFormat->currentIndex();
    locator.px = selXPrecision->currentIndex();
    locator.py = selYPrecision->currentIndex();
    locator.pointset = chkEnableFixed->isChecked()==TRUE?1:0;
    xv_evalexpr(ledFixedCoords[0], &locator.dsx );
    xv_evalexpr(ledFixedCoords[1], &locator.dsy );

    ListOfChanges.clear();;
    sprintf(dummy,"with g%d",get_cg());
    ListOfChanges << QString(dummy);

    if (locator.pt_type != locator2.pt_type)
    {
        sprintf(dummy,"g%d fixedpoint type %d",get_cg(),locator.pt_type);
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
        sprintf(dummy,"g%d fixedpoint %s",get_cg(),locator.pointset?"on":"off");
        ListOfChanges << QString(dummy);
    }
    if (locator.dsx!=locator2.dsx || locator.dsy!=locator2.dsy)
    {
        sprintf(dummy,"g%d fixedpoint xy %f, %f",get_cg(),locator.dsx,locator.dsy);
        cout << locator2.dsx << " " << locator2.dsy << endl;
        ListOfChanges << QString(dummy);
    }

    for (int i=0;i<ListOfChanges.size();i++)
        cout << ListOfChanges.at(i).toAscii().constData() << endl;

    set_graph_locator(gno, &locator);
    LocatorFixPointModified(gno);
    mainWin->mainArea->completeRedraw();
    return 0;
}

void frmLocatorProps::update_locator_items(int gno)
{
    GLocator locator;
    char buf[256];

    if (get_graph_locator(gno, &locator) != RETURN_SUCCESS) {
        return;
    }

    selLocDisplType->setCurrentIndex(locator.pt_type);

    if (locator.pointset==1)
        chkEnableFixed->setChecked(TRUE);
    else
        chkEnableFixed->setChecked(FALSE);

    selXFormat->setCurrentIndex(locator.fx);
    selYFormat->setCurrentIndex(locator.fy);
    /*SetOptionChoice(loc_formatx, locator.fx);
    SetOptionChoice(loc_formaty, locator.fy);*/
    selXPrecision->setCurrentIndex(locator.px);
    selYPrecision->setCurrentIndex(locator.py);
    /*SetChoice(loc_precx, locator.px);
    SetChoice(loc_precy, locator.py);*/
    sprintf(buf, "%g", locator.dsx);
    ledFixedCoords[0]->setText(QString(buf));
    //xv_setstr(locx_item, buf);
    sprintf(buf, "%g", locator.dsy);
    ledFixedCoords[1]->setText(QString(buf));
    //xv_setstr(locy_item, buf);
}

frmConsole::frmConsole(QWidget * parent):QDialog(parent)
{
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Console"));
    setWindowIcon(QIcon(*GraceIcon));
    CreateActions();

    menuBar=new QMenuBar();
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
    layout->setMargin(STD_MARGIN);
    layout->addWidget(menuBar);
    layout->addWidget(lblMessages);
    layout->addWidget(txtMessages);
    setLayout(layout);
    resize(500,250);
}

frmConsole::~frmConsole()
{}

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
        ofi.open(fileName.toAscii());
        ofi << text.toAscii().data() << endl;
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

frmSetOp::frmSetOp(QWidget * parent):QDialog(parent)
{
    int number;
    QString entr[6];

    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Data set operations"));
    setWindowIcon(QIcon(*GraceIcon));
    CreateActions();
    prev_operation=0;

    lblDataSet=new QLabel(tr("Data sets:"),this);
    listSets=new uniList(SETLIST,this);

    menuBar=new QMenuBar();
    mnuFile=new QMenu("&File",this);
    mnuFile->setTearOffEnabled(TRUE);
    mnuFile->addAction(actClose);
    mnuHelp=new QMenu("&Help",this);
    mnuHelp->setTearOffEnabled(TRUE);
    mnuHelp->addAction(actHelpOnContext);
    mnuHelp->addSeparator();
    mnuHelp->addAction(actHelpOnSetOp);

    menuBar->addMenu(mnuFile);
    menuBar->addSeparator();
    menuBar->addMenu( mnuHelp );

    number=5;
    entr[0]=tr("Sort");
    entr[1]=tr("Reverse");
    entr[2]=tr("Join");
    entr[3]=tr("Split");
    entr[4]=tr("Drop points");
    selOperation=new StdSelector(this,tr("Operation type:"),number,entr);
    selOperation->entryValues[0]=DATASETOP_SORT;
    selOperation->entryValues[1]=DATASETOP_REVERSE;
    selOperation->entryValues[2]=DATASETOP_JOIN;
    selOperation->entryValues[3]=DATASETOP_SPLIT;
    selOperation->entryValues[4]=DATASETOP_DROP;
    connect(selOperation->cmbSelect,SIGNAL(currentIndexChanged(int)),SLOT(changeSelection(int)));
    number=6;
    entr[0]=QString("X");
    entr[1]=QString("Y");
    entr[2]=QString("Y1");
    entr[3]=QString("Y2");
    entr[4]=QString("Y3");
    entr[5]=QString("Y4");
    selSortOn=new StdSelector(this,tr("Sort on:"),number,entr);
    number=2;
    entr[0]=tr("Ascending");
    entr[1]=tr("Descending");
    selSortOrder=new StdSelector(this,tr("Order:"),number,entr);

    ledLength=new stdLineEdit(this,tr("Length:"));
    ledLength->setVisible(FALSE);
    ledStart=new stdLineEdit(this,tr("Start at:"));
    ledStart->setVisible(FALSE);
    ledStop=new stdLineEdit(this,tr("Stop at:"));
    ledStop->setVisible(FALSE);

    buttonGroup=new stdButtonGroup(this);
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));

    layout=new QGridLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(menuBar,0,0,1,2);
    layout->addWidget(lblDataSet,1,0,1,2);
    layout->addWidget(listSets,2,0,3,2);
    layout->addWidget(selOperation,5,0,1,2);
    layout->addWidget(selSortOn,6,0);
    layout->addWidget(selSortOrder,6,1);
    layout->addWidget(ledLength,6,0,1,2);
    layout->addWidget(ledStart,6,0);
    layout->addWidget(ledStop,6,1);
    layout->addWidget(buttonGroup,7,0,1,2);

    layout->setRowStretch(0,0);
    layout->setRowStretch(1,0);
    layout->setRowStretch(2,1);
    layout->setRowStretch(3,1);
    layout->setRowStretch(4,1);
    layout->setRowStretch(5,0);
    layout->setRowStretch(6,0);
    layout->setRowStretch(7,0);
    setLayout(layout);
}

void frmSetOp::init(void)
{
    listSets->set_graph_number(get_cg(),false);
    listSets->update_number_of_entries();
}

void frmSetOp::changeSelection(int i)
{
    if (i==prev_operation)return;

    switch (prev_operation)
    {
    case 0://Sort
        selSortOn->setVisible(FALSE);
        selSortOrder->setVisible(FALSE);
        break;
    case 1://Reverse
        break;
    case 2://Join
        break;
    case 3://Split
        ledLength->setVisible(FALSE);
        break;
    case 4://Drop points
        ledStart->setVisible(FALSE);
        ledStop->setVisible(FALSE);
        break;
    }

    switch (i)
    {
    case 0://Sort
        selSortOn->setVisible(TRUE);
        selSortOrder->setVisible(TRUE);
        break;
    case 1://Reverse
        break;
    case 2://Join
        break;
    case 3://Split
        ledLength->setVisible(TRUE);
        break;
    case 4://Drop points
        ledStart->setVisible(TRUE);
        ledStop->setVisible(TRUE);
        break;
    }
    prev_operation=i;
}

void frmSetOp::CreateActions(void)
{
    actClose=new QAction(tr("&Close"),this);
    connect(actClose,SIGNAL(triggered()), this, SLOT(doClose()));
    actHelpOnContext=new QAction(tr("On conte&xt"),this);
    actHelpOnContext->setShortcut(tr("Shift+F1"));
    connect(actHelpOnContext,SIGNAL(triggered()), this, SLOT(doHelpOnContext()));
    actHelpOnSetOp=new QAction(tr("On data&set operations"),this);
    connect(actHelpOnSetOp,SIGNAL(triggered()), this, SLOT(doHelpOnSetOp()));
}

void frmSetOp::doClose(void)
{
    hide();
}

void frmSetOp::doApply(void)
{
    ApplyError=false;
    int *selset, nsets, i, setno;
    int sorton, stype;
    int lpart;
    int startno, endno;
    char dummy[64];
    static int son[MAX_SET_COLS] = {DATA_X, DATA_Y, DATA_Y1, DATA_Y2, DATA_Y3, DATA_Y4};
    int optype;
    selset=new int[2];
    listSets->get_selection(&nsets,&selset);
    int * gnos=new int[nsets+1];
    for (int i=0;i<nsets+1;i++)
        gnos[i]=cg;
    int * snos=NULL;
    int counter;
    if (nsets < 1) {
        ApplyError=true;
        errmsg(tr("No set selected").toAscii().constData());
    } else {
        optype=selOperation->currentIndex();
        ///Undo-Stuff
        SaveSetStatesPrevious(nsets,gnos,selset,UNDO_COMPLETE);
        switch (optype) {
        case DATASETOP_SORT:
            sorton = son[selSortOn->currentIndex()];
            stype = selSortOrder->currentIndex();
            for (i = 0; i < nsets; i++)
            {
                setno = selset[i];
                do_sort(setno, sorton, stype);
            }
            SetsModified(nsets,gnos,selset,UNDO_DATA);
            if (nsets>1)
            {
                addAditionalDescriptionToLastNode(-1,QObject::tr("Sort data sets"),QString(),-1);
            }
            else
            {
                sprintf(dummy,"[G%d.S%d]",cg,selset[0]);
                addAditionalDescriptionToLastNode(-1,QObject::tr("Sort data set")+QString(dummy),QString(),-1);
            }
            break;
        case DATASETOP_REVERSE:
            for (i = 0; i < nsets; i++)
            {
                setno = selset[i];
                reverse_set(cg, setno);
            }
            SetsModified(nsets,gnos,selset,UNDO_DATA);
            if (nsets>1)
            {
                addAditionalDescriptionToLastNode(-1,QObject::tr("Data sets reversed"),QString(),-1);
            }
            else
            {
                sprintf(dummy,"[G%d.S%d]",cg,selset[0]);
                addAditionalDescriptionToLastNode(-1,QObject::tr("Data set reversed")+QString(dummy),QString(),-1);
            }
            break;
        case DATASETOP_JOIN:
            join_sets(cg, selset, nsets);
            SetsModified(1,gnos,selset,UNDO_COMPLETE);
            if (nsets>1)
            {
                addAditionalDescriptionToLastNode(-1,QObject::tr("Sets joined"),QString(),-1);
            }
            else
            {
                sprintf(dummy,"[G%d.S%d]",cg,selset[0]);
                addAditionalDescriptionToLastNode(-1,QObject::tr("Set joined")+QString(dummy),QString(),-1);
            }
            break;
        case DATASETOP_SPLIT:
            xv_evalexpri(ledLength, &lpart);
            /*strcpy(dummy,ledLength->text().toAscii());
                lpart=atoi(dummy);*/
            //xv_evalexpri(datasetopui.length_item, &lpart);
            counter=0;
            snos=new int[2*nsets];
            delete[] gnos;
            gnos=new int[2*nsets];
            for (i = 0; i < nsets; i++)
            {
                setno = selset[i];
                do_splitsets(cg, setno, lpart);
                gnos[counter+1]=gnos[counter]=cg;
                snos[counter++]=setno;
                snos[counter++]=new_set_no;
            }
            SetsModified(counter,gnos,snos,UNDO_COMPLETE);
            if (nsets>1)
            {
                addAditionalDescriptionToLastNode(-1,QObject::tr("Sets split"),QString(),-1);
            }
            else
            {
                sprintf(dummy,"[G%d.S%d]",cg,selset[0]);
                addAditionalDescriptionToLastNode(-1,QObject::tr("Set split")+QString(dummy),QString(),0);
            }
            break;
        case DATASETOP_DROP:
            xv_evalexpri(ledStart, &startno);
            xv_evalexpri(ledStop, &endno);
            /*strcpy(dummy,ledStart->text().toAscii());
                startno=atoi(dummy);*/
            //xv_evalexpri(datasetopui.start_item, &startno);
            /*strcpy(dummy,ledStop->text().toAscii());
                endno=atoi(dummy);*/
            //xv_evalexpri(datasetopui.stop_item, &endno);
            for (i = 0; i < nsets; i++) {
                setno = selset[i];
                do_drop_points(cg, setno, startno, endno);
            }
            SetsModified(nsets,gnos,selset,UNDO_DATA);
            if (nsets>1)
            {
                addAditionalDescriptionToLastNode(-1,QObject::tr("Points dropped"),QString(),-1);
            }
            else
            {
                sprintf(dummy,"[G%d.S%d]",cg,selset[0]);
                addAditionalDescriptionToLastNode(-1,QObject::tr("Points dropped in ")+QString(dummy),QString(),-1);
            }
            break;
        }
        listSets->update_number_of_entries();
        if (optype==DATASETOP_JOIN && nsets>0)
            listSets->set_new_selection(1,selset);
        else if (optype==DATASETOP_SPLIT && nsets>0)
        {
            listSets->set_new_selection(counter,snos);
            delete[] snos;
        }
        else
            listSets->set_new_selection(nsets,selset);
        update_set_lists(cg);
        mainWin->mainArea->completeRedraw();
    }
    delete[] selset;
    delete[] gnos;
}

void frmSetOp::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmSetOp::doHelpOnContext(void)
{
    setCursor(QCursor(Qt::WhatsThisCursor));
}

void frmSetOp::doHelpOnSetOp(void)
{
    HelpCB("doc/UsersGuide.html#data-set-operations");
}

frmCommands::frmCommands(QWidget * parent):QDialog(parent)
{
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Commands"));
    setWindowIcon(QIcon(*GraceIcon));

    FormReadHistory=NULL;
    FormWriteHistory=NULL;

    lblCommand=new QLabel(tr("Command:"),this);
    lenCommand=new QLineEdit(QString(""),this);
    //txtCommands=new QTextEdit(this);
    list=new QListWidget(this);
    connect(list,SIGNAL(itemDoubleClicked(QListWidgetItem*)),SLOT(doDoubleClick(QListWidgetItem*)));
    grp1=new QGroupBox(this);
    grp2=new QGroupBox(this);

    layout1=new QHBoxLayout();
    layout1->setMargin(STD_MARGIN);
    layout2=new QHBoxLayout();
    layout2->setMargin(STD_MARGIN);
    layout=new QVBoxLayout();
    layout->setMargin(STD_MARGIN);

    cmdAdd=new QPushButton(tr("Add"),grp1);
    connect(cmdAdd,SIGNAL(clicked()),SLOT(doAdd()));
    cmdDelete=new QPushButton(tr("Delete"),grp1);
    connect(cmdDelete,SIGNAL(clicked()),SLOT(doDelete()));
    cmdReplace=new QPushButton(tr("Replace"),grp1);
    connect(cmdReplace,SIGNAL(clicked()),SLOT(doReplace()));
    cmdUp=new QPushButton(tr("Up"),grp1);
    connect(cmdUp,SIGNAL(clicked()),SLOT(doUp()));
    cmdDown=new QPushButton(tr("Down"),grp1);
    connect(cmdDown,SIGNAL(clicked()),SLOT(doDown()));

    layout1->addWidget(cmdAdd);
    layout1->addWidget(cmdDelete);
    layout1->addWidget(cmdReplace);
    layout1->addWidget(cmdUp);
    layout1->addWidget(cmdDown);
    grp1->setLayout(layout1);

    cmdRead=new QPushButton(tr("Read..."),grp2);
    connect(cmdRead,SIGNAL(clicked()),SLOT(doRead()));
    cmdSave=new QPushButton(tr("Save..."),grp2);
    connect(cmdSave,SIGNAL(clicked()),SLOT(doSave()));
    cmdClear=new QPushButton(tr("Clear"),grp2);
    connect(cmdClear,SIGNAL(clicked()),SLOT(doClear()));
    cmdReplay=new QPushButton(tr("Replay"),grp2);
    connect(cmdReplay,SIGNAL(clicked()),SLOT(doReplay()));
    cmdClose=new QPushButton(tr("Close"),grp2);
    connect(cmdClose,SIGNAL(clicked()),SLOT(doClose()));
    cmdHelp=new QPushButton(tr("Help"),grp2);
    connect(cmdHelp,SIGNAL(clicked()),SLOT(doHelp()));

    layout2->addWidget(cmdRead);
    layout2->addWidget(cmdSave);
    layout2->addWidget(cmdClear);
    layout2->addWidget(cmdReplay);
    layout2->addWidget(cmdClose);
    layout2->addWidget(cmdHelp);
    grp2->setLayout(layout2);

    //layout->addWidget(txtCommands);
    layout->addWidget(list);
    layout->addWidget(grp1);
    layout->addWidget(grp2);
    layout->addWidget(lblCommand);
    layout->addWidget(lenCommand);
    setLayout(layout);
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
        list->setCurrentRow(newpos);
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
    list->takeItem(sel[0]);
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
    if (FormReadHistory==NULL)
    {
        FormReadHistory=new frmIOForm(READ_PARAMETERS,this);
        connect(FormReadHistory,SIGNAL(newFileSelectedForIO(int,QString,bool,bool,bool)),SLOT(IOrequested(int,QString,bool,bool,bool)));
        QString FileExtension("*.com");
        FormReadHistory->selector->filterExtension=FileExtension;
        FormReadHistory->setWindowTitle(tr("qtGrace: Read history"));
        FormReadHistory->init();
        FormReadHistory->grpParamGraph->hide();
        FormReadHistory->selector->showFilesLikeFilter();
    }
    FormReadHistory->show();
    FormReadHistory->raise();
    FormReadHistory->activateWindow();
}

void frmCommands::doSave(void)
{
    if (FormWriteHistory==NULL)
    {
        FormWriteHistory=new frmIOForm(WRITE_PARAMETERS,this);
        connect(FormWriteHistory,SIGNAL(newFileSelectedForIO(int,QString,bool,bool,bool)),SLOT(IOrequested(int,QString,bool,bool,bool)));
        QString FileExtension("*.com");
        FormWriteHistory->selector->filterExtension=FileExtension;
        FormWriteHistory->setWindowTitle(tr("qtGrace: Write history"));
        FormWriteHistory->init();
        FormWriteHistory->grpParamGraph->hide();
        FormWriteHistory->selector->showFilesLikeFilter();
    }
    FormWriteHistory->show();
    FormWriteHistory->raise();
    FormWriteHistory->activateWindow();
}

void frmCommands::IOrequested(int type,QString file,bool exists,bool writeable,bool readable)
{
    char filename[256];
    strcpy(filename,file.toAscii());
    char buf[512];
    FILE *fp,*pp;

    if (type==WRITE_PARAMETERS)
    {
        int hc=list->count(),i;
        QListWidgetItem * lwid;
        pp = grace_openw(filename);
        if (pp != NULL) {
            for (i = 0; i < hc; i++) {
                lwid=list->item(i);
                strcpy(buf,lwid->text().toAscii());
                fprintf(pp, "%s\n", buf);
            }
            grace_close(pp);
        }
        FormWriteHistory->hide();
    }
    else//READ_PARAMETERS
    {
        int sl;
        if ((fp = grace_openr(filename, SOURCE_DISK)) != NULL) {
            while (grace_fgets(buf, 255, fp) != NULL) {
                sl = strlen(buf);
                buf[sl - 1] = 0;
                if (strlen(buf) == 0) {
                    continue;
                }
                list->addItem(QString(buf));
            }
            grace_close(fp);
        }
        FormReadHistory->hide();
    }
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
    static int errcount;
    char ts[512];
    int i;
    int hc=list->count();
    QListWidgetItem * lwid;
    errcount = 0;
    for (i = 0; i < hc; i++)
    {
        lwid=list->item(i);
        strcpy(ts,lwid->text().toAscii());
        errpos = scanner(ts);
        if (errpos) {
            errcount++;
        }
        if (errcount > MAXERR) {
            if (yesno("Lots of errors, cancel?", NULL, NULL, NULL)) {
                break;
            } else {
                errcount = 0;
            }
        }
    }
    ///mainWin->mainArea->completeRedraw();
}

void frmCommands::doClose(void)
{
    hide();
}

void frmCommands::doHelp(void)
{
    HelpCB("doc/UsersGuide.html#commands");
}

void frmCommands::doDoubleClick(QListWidgetItem * index)
{
    lenCommand->setText(index->text());
}

frmDeviceOptions::frmDeviceOptions(int device,QWidget * parent):QDialog(parent)
{
    int number;
    QString entr[5];

    Device=device;
    setFont(stdFont);
    setWindowIcon(QIcon(*GraceIcon));

    grpPSoptions=new QGroupBox(tr("PS options"),this);
    layout0=new QVBoxLayout;
    layout0->setMargin(STD_MARGIN);
    ps_setup_grayscale_item=new QCheckBox(tr("Grayscale output"),grpPSoptions);
    layout0->addWidget(ps_setup_grayscale_item);
    ps_setup_level2_item=new QCheckBox(tr("PS Level 2"),grpPSoptions);
    ps_setup_level2_item->setChecked(TRUE);
    layout0->addWidget(ps_setup_level2_item);
    number=3;
    entr[0]=tr("7bit");
    entr[1]=tr("8bit");
    entr[2]=tr("Binary");
    ps_setup_docdata_item=new StdSelector(grpPSoptions,tr("Document data:"),number,entr);
    ps_setup_docdata_item->setCurrentIndex(1);
    layout0->addWidget(ps_setup_docdata_item);
    grpPSoptions->setLayout(layout0);
    grpPSoptions->setVisible(FALSE);
    grpPageOffset=new QGroupBox(tr("Page offset (pt)"),this);
    layout1=new QHBoxLayout;
    layout1->setMargin(STD_MARGIN);
    ps_setup_offset_x_item=new stdIntSelector(grpPageOffset,tr("X:"),-999,999);
    ps_setup_offset_x_item->spnInt->setSingleStep(10);
    layout1->addWidget(ps_setup_offset_x_item);
    ps_setup_offset_y_item=new stdIntSelector(grpPageOffset,tr("Y:"),-999,999);
    ps_setup_offset_y_item->spnInt->setSingleStep(10);
    layout1->addWidget(ps_setup_offset_y_item);
    grpPageOffset->setLayout(layout1);
    grpPageOffset->setVisible(FALSE);
    grpHardware=new QGroupBox(tr("Hardware"),this);
    layout2=new QVBoxLayout;
    layout2->setMargin(STD_MARGIN);
    number=3;
    entr[0]=tr("Automatic");
    entr[1]=tr("Match size");
    entr[2]=tr("Manual");
    ps_setup_feed_item=new StdSelector(grpHardware,tr("Media feed:"),number,entr);
    layout2->addWidget(ps_setup_feed_item);
    ps_setup_hwres_item=new QCheckBox(tr("Set hardware resolution"),grpHardware);
    layout2->addWidget(ps_setup_hwres_item);
    grpHardware->setLayout(layout2);
    grpHardware->setVisible(FALSE);

    grpPNMoptions=new QGroupBox(tr("PNM options"),this);
    layout3=new QVBoxLayout;
    layout3->setMargin(STD_MARGIN);
    number=3;
    entr[0]=tr("1-bit mono (PBM)");
    entr[1]=tr("8-bit grayscale (PGM)");
    entr[2]=tr("8-bit color (PPM)");
    pnm_setup_format_item=new StdSelector(grpPNMoptions,tr("Format:"),number,entr);
    pnm_setup_format_item->setCurrentIndex(2);
    layout3->addWidget(pnm_setup_format_item);
    pnm_setup_rawbits_item=new QCheckBox(tr("\"Rawbits\""),grpPNMoptions);
    pnm_setup_rawbits_item->setChecked(TRUE);
    layout3->addWidget(pnm_setup_rawbits_item);
    grpPNMoptions->setLayout(layout3);
    grpPNMoptions->setVisible(FALSE);

    grpEPSoptions=new QGroupBox(tr("EPS options"),this);
    layout4=new QVBoxLayout;
    layout4->setMargin(STD_MARGIN);
    eps_setup_grayscale_item=new QCheckBox(tr("Grayscale output"),grpEPSoptions);
    layout4->addWidget(eps_setup_grayscale_item);
    eps_setup_level2_item=new QCheckBox(tr("PS Level 2"),grpEPSoptions);
    eps_setup_level2_item->setChecked(TRUE);
    layout4->addWidget(eps_setup_level2_item);
    eps_setup_tight_bb_item=new QCheckBox(tr("Tight BBox"),grpEPSoptions);
    eps_setup_tight_bb_item->setChecked(TRUE);
    layout4->addWidget(eps_setup_tight_bb_item);
    number=3;
    entr[0]=tr("7bit");
    entr[1]=tr("8bit");
    entr[2]=tr("Binary");
    eps_setup_docdata_item=new StdSelector(grpPSoptions,tr("Document data:"),number,entr);
    eps_setup_docdata_item->setCurrentIndex(1);
    layout4->addWidget(eps_setup_docdata_item);
    grpEPSoptions->setLayout(layout4);
    grpEPSoptions->setVisible(FALSE);

    grpJPEGoptions=new QGroupBox(tr("JPEG options"),this);
    layout5=new QVBoxLayout;
    layout5->setMargin(STD_MARGIN);
    selQuality=new stdIntSelector(grpJPEGoptions,tr("Quality:"),0,100);
    selQuality->setValue(75);
    selQuality->spnInt->setSingleStep(5);
    layout5->addWidget(selQuality);
    chkOptimize=new QCheckBox(tr("Optimize"),grpJPEGoptions);
    layout5->addWidget(chkOptimize);
    chkProgressive=new QCheckBox(tr("Progressive"),grpJPEGoptions);
    layout5->addWidget(chkProgressive);
    // chkJPG_Grayscale=new QCheckBox(tr("Grayscale"),grpJPEGoptions);
    // layout5->addWidget(chkJPG_Grayscale);
    grpJPEGoptions->setLayout(layout5);
    grpJPEGoptions->setVisible(FALSE);
    grpJPEGadvoptions=new QGroupBox(tr("JPEG advanced options"),this);
    layout6=new QVBoxLayout;
    layout6->setMargin(STD_MARGIN);
    selSmoothing=new stdIntSelector(grpJPEGadvoptions,tr("Smoothing:"),0,100);
    selSmoothing->spnInt->setSingleStep(10);
    layout6->addWidget(selSmoothing);
    chkForceBaseline=new QCheckBox(tr("Force baseline"),grpJPEGadvoptions);
    layout6->addWidget(chkForceBaseline);
    number=3;
    entr[0]=tr("Fast integer");
    entr[1]=tr("Slow integer");
    entr[2]=tr("Float");
    selDCT=new StdSelector(grpJPEGadvoptions,tr("DCT:"),number,entr);
    selDCT->setCurrentIndex(1);
    layout6->addWidget(selDCT);
    grpJPEGadvoptions->setLayout(layout6);
    grpJPEGadvoptions->setVisible(FALSE);

    grpPNGoptions=new QGroupBox(tr("PNG options"),this);
    layout7=new QVBoxLayout;
    layout7->setMargin(STD_MARGIN);
    chkInterlaced=new QCheckBox(tr("Interlaced"),grpPNGoptions);
    layout7->addWidget(chkInterlaced);
    chkTransparent=new QCheckBox(tr("Transparent"),grpPNGoptions);
    layout7->addWidget(chkTransparent);
    selCompression=new stdIntSelector(grpPNGoptions,tr("Compression:"),0,9);
    selCompression->setValue(4);
    layout7->addWidget(selCompression);
    grpPNGoptions->setLayout(layout7);
    grpPNGoptions->setVisible(FALSE);

    grpBMPoptions=new QGroupBox(tr("BMP options"),this);
    layout8=new QVBoxLayout;
    layout8->setMargin(STD_MARGIN);
    sldQuality=new stdSlider(grpBMPoptions,tr("Quality"),0,100);
    layout8->addWidget(sldQuality);
    sldQuality->setValue(outputQuality);
    //   chkGrayscale=new QCheckBox(tr("Grayscale"),grpBMPoptions);
    //layout8->addWidget(chkGrayscale);
    grpBMPoptions->setLayout(layout8);
    grpBMPoptions->setVisible(FALSE);

    buttonGroup=new stdButtonGroup(this);
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));

    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    switch (device)
    {
    case DEVICE_PS:
        setWindowTitle(tr("qtGrace: PS options"));
        grpPSoptions->setVisible(TRUE);
        grpPageOffset->setVisible(TRUE);
        grpHardware->setVisible(TRUE);
        layout->addWidget(grpPSoptions);
        layout->addWidget(grpPageOffset);
        layout->addWidget(grpHardware);
        break;
    case DEVICE_EPS:
        setWindowTitle(tr("qtGrace: EPS options"));
        grpEPSoptions->setVisible(TRUE);
        layout->addWidget(grpEPSoptions);
        break;
    case DEVICE_PNM:
        setWindowTitle(tr("qtGrace: PNM options"));
        grpPNMoptions->setVisible(TRUE);
        layout->addWidget(grpPNMoptions);
        break;
    case DEVICE_JPEG:
        setWindowTitle(tr("qtGrace: JPEG options"));
        grpJPEGoptions->setVisible(TRUE);
        grpJPEGadvoptions->setVisible(TRUE);
        layout->addWidget(grpJPEGoptions);
        layout->addWidget(grpJPEGadvoptions);
        break;
    case DEVICE_PNG:
        setWindowTitle(tr("qtGrace: PNG options"));
        grpPNGoptions->setVisible(TRUE);
        layout->addWidget(grpPNGoptions);
        break;
    case DEVICE_BMP:
        setWindowTitle(tr("qtGrace: BMP options"));
        grpBMPoptions->setVisible(TRUE);
        layout->addWidget(grpBMPoptions);
        break;
    }
    layout->addWidget(buttonGroup);
    setLayout(layout);
}

void frmDeviceOptions::init(void)
{
    if (Device==DEVICE_BMP)
    {
        sldQuality->setValue(outputQuality);
        //    chkGrayscale->setChecked(outputGrayscale);
    }
    else if (Device==DEVICE_PS)
    {
        SetToggleButtonState(ps_setup_grayscale_item, ps_setup_grayscale);
        SetToggleButtonState(ps_setup_level2_item, ps_setup_level2);
        SetSpinChoice(ps_setup_offset_x_item, (double) ps_setup_offset_x);
        SetSpinChoice(ps_setup_offset_y_item, (double) ps_setup_offset_y);
        SetOptionChoice(ps_setup_feed_item, ps_setup_feed);
        SetToggleButtonState(ps_setup_hwres_item, ps_setup_hwres);
        SetOptionChoice(ps_setup_docdata_item, ps_setup_docdata);
    }
    else if (Device==DEVICE_EPS)
    {
        SetToggleButtonState(eps_setup_grayscale_item, eps_setup_grayscale);
        SetToggleButtonState(eps_setup_level2_item, eps_setup_level2);
        SetToggleButtonState(eps_setup_tight_bb_item, eps_setup_tight_bb);
        SetOptionChoice(eps_setup_docdata_item, eps_setup_docdata);
    }
    else if (Device==DEVICE_PNM)
    {
        SetChoice(pnm_setup_format_item, pnm_setup_format);
        SetToggleButtonState(pnm_setup_rawbits_item, pnm_setup_rawbits);
    }

    else
    {

    }
}

void frmDeviceOptions::doApply(void)
{
    ApplyError=false;
    if (Device==DEVICE_BMP)
    {
        outputQuality=sldQuality->value();
        // outputGrayscale=chkGrayscale->isChecked();
    }
    else if (Device==DEVICE_PS)
    {
        ps_setup_grayscale = GetToggleButtonState(ps_setup_grayscale_item);
        ps_setup_level2    = GetToggleButtonState(ps_setup_level2_item);
        ps_setup_offset_x  = (int) GetSpinChoice(ps_setup_offset_x_item);
        ps_setup_offset_y  = (int) GetSpinChoice(ps_setup_offset_y_item);
        ps_setup_feed      = GetOptionChoice(ps_setup_feed_item);
        ps_setup_hwres     = GetToggleButtonState(ps_setup_hwres_item);
        ps_setup_docdata   = GetOptionChoice(ps_setup_docdata_item);
    }
    else if (Device==DEVICE_EPS)
    {
        eps_setup_grayscale = GetToggleButtonState(eps_setup_grayscale_item);
        eps_setup_level2 = GetToggleButtonState(eps_setup_level2_item);
        eps_setup_tight_bb = GetToggleButtonState(eps_setup_tight_bb_item);
        eps_setup_docdata = GetOptionChoice(eps_setup_docdata_item);
    }
    else if (Device==DEVICE_PNM)
    {
        pnm_setup_format = GetChoice(pnm_setup_format_item);
        pnm_setup_rawbits = GetToggleButtonState(pnm_setup_rawbits_item);
    }

    else
    {

    }
}

void frmDeviceOptions::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmDeviceOptions::doClose(void)
{
    hide();
}

frmDeviceSetup::frmDeviceSetup(int windowTitle, QWidget * parent):QDialog(parent)
{
    int number;
    QString entr[13];
    int i_entr[13];
    setFont(stdFont);

    //2013-07-03 Change window title for the new dialogs- Nimal Kailasanathan

    if(windowTitle == 1){
        setWindowTitle(tr("qtGrace: Screen Setup"));
    }else if (windowTitle == 2){
        setWindowTitle(tr("qtGrace: Print"));
    }else if (windowTitle == 3){
        setWindowTitle(tr("qtGrace: Export to File"));
    }
    else{

    }

    setWindowIcon(QIcon(*GraceIcon));
    cur_dev=0;

    CreateActions();


    if (windowTitle ==1){
        grpDevSetup=new QGroupBox(tr("Screen setup"),this);
    }else if (windowTitle == 2){
        grpDevSetup=new QGroupBox(tr("Print"),this);
    }else if (windowTitle == 3){
        grpDevSetup=new QGroupBox(tr("Export to file"),this);
    }
    else{

    }


    grpOutput=new QGroupBox(tr("Output"),this);

    if (windowTitle ==1){
        grpPage=new QGroupBox(tr("Display"),this);
    }else if (windowTitle == 2){
        grpPage=new QGroupBox(tr("Page"),this);
    }else if (windowTitle == 3){
        grpPage=new QGroupBox(tr("Format size"),this);
    }
    else{

    }

    grpFonts=new QGroupBox(tr("Fonts"),this);

    //2013-07-03 Changed window titles - Nimal Kailasanathan

    if (windowTitle ==1){
        device_opts_item=new QPushButton(tr(""),grpDevSetup);
    }else if (windowTitle == 2){
        device_opts_item=new QPushButton(tr(""),grpDevSetup);
    }else if (windowTitle == 3){
        device_opts_item=new QPushButton(tr("Format options..."),grpDevSetup);
    }
    else{

    }

    connect(device_opts_item,SIGNAL(clicked()),this,SLOT(doDevOpt()));
    wbut=new QPushButton(tr("Browse..."),grpOutput);
    connect(wbut,SIGNAL(clicked()),this,SLOT(doBrowse()));

    menuBar=new QMenuBar();
    mnuFile=new QMenu("&File",this);
    mnuFile->setTearOffEnabled(TRUE);
    mnuFile->addAction(actPrint);
    mnuFile->addSeparator();
    mnuFile->addAction(actClose);
    mnuOptions=new QMenu("&Options",this);
    mnuOptions->setTearOffEnabled(TRUE);
    mnuOptions->addAction(dsync_item);
    mnuOptions->addAction(psync_item);
    mnuHelp=new QMenu("&Help",this);
    mnuHelp->setTearOffEnabled(TRUE);
    mnuHelp->addAction(actHelpOnContext);
    mnuHelp->addSeparator();
    mnuHelp->addAction(actHelpOnDevSetup);

    menuBar->addMenu(mnuFile);
    menuBar->addMenu(mnuOptions);
    menuBar->addSeparator();
    menuBar->addMenu(mnuHelp);


    number=number_of_devices();
    for (int i=0;i<number;i++)
    {
        entr[i]=get_device_name(i);
    }

    if (windowTitle ==1){
        devices_item=new StdSelector(grpDevSetup,tr(""),number,entr);
        emit devices_item->currentIndexChanged(9); //use screen from devices item
    }else if (windowTitle == 2){
        devices_item=new StdSelector(grpDevSetup,tr("Print to:"),number,entr);
    }else if (windowTitle == 3){
        devices_item=new StdSelector(grpDevSetup,tr("Select file format:"),number-1,entr); //-1 remove screen from list
    }
    else{

    }


    connect(devices_item->cmbSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(DeviceChanged(int)));
    number=2;
    entr[0]=tr("Landscape");
    entr[1]=tr("Portrait");
    i_entr[0]=PAGE_ORIENT_LANDSCAPE;
    i_entr[1]=PAGE_ORIENT_PORTRAIT;
    page_orient_item=new StdSelector(grpPage,tr("Orientation:"),number,entr);
    page_orient_item->setValues(i_entr);
    connect(page_orient_item->cmbSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(OrientationChanged(int)));
    //2013-07-03 Added new page formats - Nimal Kailasanathan
    number=12;

    entr[0]=tr("Custom");
    entr[1]=tr("A4");
    entr[2]=tr("A3");
    entr[3]=tr("A2");
    entr[4]=tr("A1");
    entr[5]=tr("A0");
    entr[6]=tr("B4");
    entr[7]=tr("B5");
    entr[8]=tr("B6");
    entr[9]=tr("US Letter");
    entr[10]=tr("US Legal");
    entr[11]=tr("US Tabloid");

    i_entr[0]=PAGE_FORMAT_A4;
    i_entr[1]=PAGE_FORMAT_A3;
    i_entr[2]=PAGE_FORMAT_A2;
    i_entr[3]=PAGE_FORMAT_A1;
    i_entr[4]=PAGE_FORMAT_A0;
    i_entr[5]=PAGE_FORMAT_B4;
    i_entr[6]=PAGE_FORMAT_B5;
    i_entr[7]=PAGE_FORMAT_B6;
    i_entr[8]=PAGE_FORMAT_CUSTOM;
    i_entr[9]=PAGE_FORMAT_USLETTER;
    i_entr[10]=PAGE_FORMAT_USLEGAL;
    i_entr[11]=PAGE_FORMAT_USTABLOID;

    //
    page_format_item=new StdSelector(grpPage,tr("Size:"),number,entr);
    connect(page_format_item->cmbSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(SizeChanged(int)));
    printfile_item=new stdLineEdit(grpOutput,tr("File name:"));
    printfile_item->lenText->setText(QString(""));
    page_x_item=new stdLineEdit(grpPage,tr("Dimensions:"));
    page_y_item=new stdLineEdit(grpPage,tr("x"));
    dev_res_item=new stdLineEdit(grpPage,tr("Resolution (dpi):"));
    page_size_unit_item=new QComboBox(grpPage);
    page_size_unit_item->addItem(tr("pix"));
    page_size_unit_item->addItem(tr("in"));
    page_size_unit_item->addItem(tr("cm"));
    connect(page_size_unit_item,SIGNAL(currentIndexChanged(int)),this,SLOT(DimChanged(int)));
    chkDontChangeSize=new QCheckBox(tr("Don't reposition Graph(s)"),this);

    fontaa_item=new QCheckBox(tr("Enable font antialiasing"),grpFonts);
    fontaa_item->setChecked(TRUE);
    devfont_item=new QCheckBox(tr("Use device fonts"),grpFonts);


    actNativePrinterDialog=new QAction(this);
    connect(actNativePrinterDialog,SIGNAL(triggered()),this,SLOT(doNativePrinterDialog()));

    if (windowTitle ==1){
        cmdDoPrint=new QPushButton(tr(""),this);
    }else if (windowTitle == 2){
        cmdDoPrint=new QPushButton(tr("Print to File"),this);
    }else if (windowTitle == 3){
        cmdDoPrint=new QPushButton(tr("Export"),this);
    }
    else{

    }
    connect(cmdDoPrint,SIGNAL(clicked()),SLOT(doPrint2()));

    if (windowTitle ==1){

        buttonGroup=new stdButtonGroup(this);
        connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));
        connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
        connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));
    }else if (windowTitle == 2){

    }else if (windowTitle == 3){

    }
    else{

    }


    for (int i=0;i<6;i++)
        DevOptions[i]=NULL;

    layout0=new QHBoxLayout();
    layout0->setMargin(STD_MARGIN);
    layout0->addWidget(devices_item);
    layout0->addWidget(device_opts_item);
    grpDevSetup->setLayout(layout0);
    layout1=new QGridLayout();
    layout1->setMargin(STD_MARGIN);
    layout1->addWidget(cmdDoPrint,0,1,1,1);
    layout1->addWidget(printfile_item,2,0,1,2);
    layout1->addWidget(wbut,2,2);
    grpOutput->setLayout(layout1);
    layout2=new QGridLayout();
    layout2->setMargin(STD_MARGIN);
    layout2->addWidget(page_orient_item,0,0,1,2);
    layout2->addWidget(page_format_item,0,2,1,2);
    layout2->addWidget(page_x_item,1,0,1,2);
    layout2->addWidget(page_y_item,1,2);
    layout2->addWidget(page_size_unit_item,1,3);
    layout2->addWidget(dev_res_item,2,0);//,1,2
    layout2->addWidget(chkDontChangeSize,2,1,1,2);
    grpPage->setLayout(layout2);
    layout3=new QVBoxLayout();
    layout3->setMargin(STD_MARGIN);
    layout3->addWidget(fontaa_item);
    layout3->addWidget(devfont_item);
    grpFonts->setLayout(layout3);
    layout=new QVBoxLayout();
    layout->setMargin(STD_MARGIN);
    layout->addWidget(menuBar);
    layout->addWidget(grpDevSetup);
    layout->addWidget(grpOutput);
    layout->addWidget(grpPage);
    layout->addWidget(grpFonts);

    if (windowTitle ==1){
        layout->addWidget(buttonGroup);
    }else if (windowTitle == 2){

    }else if (windowTitle == 3){

    }
    else{

    }

    layout->setStretch(0,0);
    layout->setStretch(1,1);
    layout->setStretch(2,1);
    layout->setStretch(3,1);
    layout->setStretch(4,1);
    layout->setStretch(5,0);
    setLayout(layout);

    page_format_item->setCurrentIndex(1);
    DeviceChanged(0);
}

void frmDeviceSetup::init(int dev)
{
    SetOptionChoice(devices_item, dev);
    DeviceChanged(dev);
}


void frmDeviceSetup::CreateActions(void)
{
    actPrint=new QAction(tr("&Export"),this);
    actPrint->setShortcut(tr("Ctrl+Alt+E"));
    connect(actPrint,SIGNAL(triggered()), this, SLOT(doPrint()));
    dsync_item=new QAction(tr("&Sync page size of all devices"),this);
    dsync_item->setCheckable(TRUE);
    dsync_item->setChecked(TRUE);
    connect(dsync_item,SIGNAL(triggered()), this, SLOT(doSyncPage()));
    psync_item=new QAction(tr("&Rescale plot on page size change"),this);
    psync_item->setCheckable(TRUE);
    psync_item->setChecked(FALSE);
    connect(psync_item,SIGNAL(triggered()), this, SLOT(doRescalePlot()));
    actClose=new QAction(tr("&Close"),this);
    actClose->setShortcut(tr("Esc"));
    connect(actClose,SIGNAL(triggered()), this, SLOT(doClose()));
    actHelpOnContext=new QAction(tr("On conte&xt"),this);
    actHelpOnContext->setShortcut(tr("Shift+F1"));
    connect(actHelpOnContext,SIGNAL(triggered()), this, SLOT(doHelpOnContext()));
    actHelpOnDevSetup=new QAction(tr("On &device setup"),this);
    connect(actHelpOnDevSetup,SIGNAL(triggered()), this, SLOT(doHelpOnDevSetup()));
}

void frmDeviceSetup::DeviceChanged(int device_id)
{
    char buf[GR_MAXPATHLEN], *bufptr;
    int page_units;
    double page_x, page_y;
    PageFormat pf;

    Page_geometry pg;
    Device_entry dev;

    cur_dev=device_id;

    dev = get_device_props(device_id);
    pg = dev.pg;

    if (dev.setup == NULL) {
        SetSensitive(device_opts_item, false);
    } else {
        SetSensitive(device_opts_item, true);
    }

    if (print_file == NULL || print_file[0] == '\0') {
        strcpy(print_file, mybasename(get_docname()));
    }

    /// * Replace existing filename extension */
    /*bufptr = strrchr(print_file, '.');
            if (bufptr) {
                *(bufptr + 1) = '\0';
            } else {
                strcat(print_file, ".");
            }
            strcat(print_file, dev.fext);*/
    /// sprintf(print_file,"%s.%s",get_docbname(),dev.fext);

    QString print_file_with_extension=get_filename_with_extension(device_id);
    strcpy(print_file,print_file_with_extension.toAscii().constData());

    xv_setstr(printfile_item, print_file);
    //  xv_setstr(print_string_item, get_print_cmd());

    switch (dev.type) {

    case DEVICE_TERM:
        grpOutput->setVisible(false);
        grpDevSetup->setVisible(false);
        grpFonts->setVisible(true);
        grpPage->setVisible(true);

        break;
    case DEVICE_FILE:
        grpDevSetup->setVisible(true);
        grpFonts->setVisible(true);
        grpPage->setVisible(true);
        grpOutput->setVisible(true);
        SetSensitive(printfile_item, true);
        SetSensitive(wbut, true);


        //            SetToggleButtonState(printto_item, true);
        //            SetSensitive(printto_item, true);
        //SetSensitive(printto_item, false);
        //  SetSensitive(print_string_item, false);
        //SetSensitive(rc_filesel, true);
        //SetSensitive(printfile_item, true);
        //SetSensitive(wbut, true);
        break;
    case DEVICE_PRINT:
        grpDevSetup->setVisible(true);
        grpFonts->setVisible(true);
        grpPage->setVisible(true);
        grpOutput->setVisible(true);
        //SetToggleButtonState(printto_item, get_ptofile());
        //SetSensitive(printto_item, true);
        if (get_ptofile() == true) {
            //SetSensitive(rc_filesel, true);
            SetSensitive(printfile_item, true);
            SetSensitive(wbut, true);
            //                SetSensitive(printto_item, true);
            //SetSensitive(printto_item, false);
            //SetSensitive(print_string_item, false);
        }// else {
        //SetSensitive(rc_filesel, false);
        // SetSensitive(printfile_item, true);
        // SetSensitive(wbut, false);
        //                SetSensitive(printto_item, true);
        //  SetSensitive(print_string_item, true);
        //}
        break;
    }

    SetOptionChoice(page_orient_item, pg.width < pg.height ? PAGE_ORIENT_PORTRAIT : PAGE_ORIENT_LANDSCAPE);

    pf = get_page_format(device_id);
    SetOptionChoice(page_format_item, pf);
    if (pf == PAGE_FORMAT_CUSTOM) {
        SetSensitive(page_x_item, true);
        SetSensitive(page_y_item, true);
        SetSensitive(page_orient_item, false);
    } else {
        SetSensitive(page_x_item, false);
        SetSensitive(page_y_item, false);
        SetSensitive(page_orient_item, true);
    }

    sprintf (buf, "%.0f", pg.dpi);
    xv_setstr(dev_res_item, buf);

    page_units = GetOptionChoice(page_size_unit_item);

    switch (page_units) {
    case 0:     /* pixels */
        page_x = (float) pg.width;
        page_y = (float) pg.height;
        break;
    case 1:      /* inches */
        page_x = (float) pg.width / pg.dpi;
        page_y = (float) pg.height / pg.dpi;
        break;
    case 2:      /* cm */
        page_x = (float) CM_PER_INCH * pg.width / pg.dpi;
        page_y = (float) CM_PER_INCH * pg.height / pg.dpi;
        break;
    default:
        errmsg(tr("Internal error").toAscii().constData());
        return;
    }

    sprintf (buf, "%.2f", page_x);
    xv_setstr(page_x_item, buf);
    sprintf (buf, "%.2f", page_y);
    xv_setstr(page_y_item, buf);

    SetToggleButtonState(fontaa_item, dev.fontaa);
    SetToggleButtonState(devfont_item, dev.devfonts);
}

void frmDeviceSetup::OrientationChanged(int value)
{
    int orientation = value;
    double px, py;
    char buf[256];

    if (xv_evalexpr(page_x_item, &px) != RETURN_SUCCESS ||
            xv_evalexpr(page_y_item, &py) != RETURN_SUCCESS ) {
        errmsg(tr("Invalid page dimension(s)").toAscii().constData());
        return;
    }

    if ((orientation == PAGE_ORIENT_LANDSCAPE && px > py) ||
            (orientation == PAGE_ORIENT_PORTRAIT  && px < py) ) {
        sprintf (buf, "%.2f", px);
        xv_setstr(page_x_item, buf);
        sprintf (buf, "%.2f", py);
        xv_setstr(page_y_item, buf);
    } else {
        sprintf (buf, "%.2f", py);
        xv_setstr(page_x_item, buf);
        sprintf (buf, "%.2f", px);
        xv_setstr(page_y_item, buf);
    }
}

void frmDeviceSetup::SizeChanged(int value)
{
    int orientation;
    int x, y;
    double px, py;
    int page_units;
    double dpi;
    char buf[256];

    if (value == PAGE_FORMAT_CUSTOM) {
        SetSensitive(page_x_item, true);
        SetSensitive(page_y_item, true);
        SetSensitive(page_orient_item, false);
    } else {
        SetSensitive(page_x_item, false);
        SetSensitive(page_y_item, false);
        SetSensitive(page_orient_item, true);
    }
    switch (value) {
    case PAGE_FORMAT_USLETTER:
        x = 595.00;
        y = 770.00;
        break;
        //Nimal
    case PAGE_FORMAT_USLEGAL:
        x = 595.00;
        y = 980.00;
        break;
    case PAGE_FORMAT_USTABLOID:
        x = 1067.00;
        y = 1649.00;
        break;

    case PAGE_FORMAT_A4:
        x = 595;
        y = 842;
        break;
    case PAGE_FORMAT_A3:
        x = 842;
        y = 1180;
        break;
    case PAGE_FORMAT_A2:
        x = 1180;
        y = 1684;
        break;
    case PAGE_FORMAT_A1:
        x = 1684;
        y = 2360;
        break;
    case PAGE_FORMAT_A0:
        x = 2360;
        y = 3368;
        break;
    case PAGE_FORMAT_B4:
        x = 688.8;
        y = 973;
        break;
    case PAGE_FORMAT_B5:
        x = 485.1;
        y = 688.8;
        break;
    case PAGE_FORMAT_B6:
        x = 344.4;
        y = 485.1;
        break;
        //
    case PAGE_FORMAT_CUSTOM:
    default:
        return;
    }
    page_units = GetOptionChoice(page_size_unit_item);
    switch (page_units) {
    case 0:      /* pixels */
        if (xv_evalexpr(dev_res_item, &dpi) != RETURN_SUCCESS) {
            errmsg(tr("Invalid dpi").toAscii().constData());
            return;
        }
        px = (float) x*dpi/72.0;
        py = (float) y*dpi/72.0;
        break;
    case 1:      /* inches */
        px = (float) x/72.0;
        py = (float) y/72.0;
        break;
    case 2:      /* cm */
        px = (float) x/72.0*CM_PER_INCH;
        py = (float) y/72.0*CM_PER_INCH;
        break;
    default:
        errmsg(tr("Internal error").toAscii().constData());
        return;
    }

    orientation = GetOptionChoice(page_orient_item);

    if ((orientation == PAGE_ORIENT_LANDSCAPE && px > py) ||
            (orientation == PAGE_ORIENT_PORTRAIT  && px < py) ) {
        sprintf (buf, "%.2f", px);
        xv_setstr(page_x_item, buf);
        sprintf (buf, "%.2f", py);
        xv_setstr(page_y_item, buf);
    } else {
        sprintf (buf, "%.2f", py);
        xv_setstr(page_x_item, buf);
        sprintf (buf, "%.2f", px);
        xv_setstr(page_y_item, buf);
    }

    /*if (i==0)
{
page_orient_item->setEnabled(FALSE);
page_size_unit_item->setEnabled(TRUE);
page_x_item->setEnabled(TRUE);
page_y_item->setEnabled(TRUE);
}
else
{
page_orient_item->setEnabled(TRUE);
page_size_unit_item->setEnabled(FALSE);
page_x_item->setEnabled(FALSE);
page_y_item->setEnabled(FALSE);
}*/
}

void frmDeviceSetup::DimChanged(int i)
{
    char buf[256];
    double page_x, page_y;
    double dev_res;
    int page_units = i;

    if (xv_evalexpr(page_x_item, &page_x) != RETURN_SUCCESS ||
            xv_evalexpr(page_y_item, &page_y) != RETURN_SUCCESS ) {
        errmsg(tr("Invalid page dimension(s)").toAscii().constData());
        return;
    }

    if (xv_evalexpr(dev_res_item, &dev_res) != RETURN_SUCCESS) {
        errmsg(tr("Invalid device resolution(s)").toAscii().constData());
        return;
    }

    if (dev_res <= 0.0) {
        errmsg(tr("Device resolution(s) <= 0").toAscii().constData());
        return;
    }

    if (current_page_units == page_units) {
        ;
    } else if (current_page_units == 0 && page_units == 1) {
        page_x /= dev_res;
        page_y /= dev_res;
    } else if (current_page_units == 0 && page_units == 2) {
        page_x /= (dev_res/CM_PER_INCH);
        page_y /= (dev_res/CM_PER_INCH);
    } else if (current_page_units == 1 && page_units == 0) {
        page_x *= dev_res;
        page_y *= dev_res;
    } else if (current_page_units == 1 && page_units == 2) {
        page_x *= CM_PER_INCH;
        page_y *= CM_PER_INCH;
    } else if (current_page_units == 2 && page_units == 0) {
        page_x *= (dev_res/CM_PER_INCH);
        page_y *= (dev_res/CM_PER_INCH);
    } else if (current_page_units == 2 && page_units == 1) {
        page_x /= CM_PER_INCH;
        page_y /= CM_PER_INCH;
    } else {
        errmsg(tr("Internal error").toAscii().constData());
        return;
    }

    current_page_units = page_units;

    sprintf (buf, "%.2f", page_x);
    xv_setstr(page_x_item, buf);
    sprintf (buf, "%.2f", page_y);
    xv_setstr(page_y_item, buf);
}

void frmDeviceSetup::doApply(void)
{
    ApplyError=false;
    int seldevice;
    double page_x, page_y;
    double dpi;
    int page_units;
    Device_entry dev;
    Page_geometry pg;
    int do_redraw = FALSE;

    int nr_of_gr=number_of_graphs();
    double old_small,new_small;
    Page_geometry cur_pg=get_page_geometry();
    Page_geometry old_pg=cur_pg;
    old_small=cur_pg.width<cur_pg.height?cur_pg.width:cur_pg.height;

    stdOutputFormat = seldevice = GetOptionChoice(devices_item);

    SaveDeviceState(seldevice,GetToggleButtonState(dsync_item));

    dev = get_device_props(seldevice);

    if (dev.type != DEVICE_TERM)
    {
        hdevice = seldevice;
        //set_ptofile(TRUE);
        //set_ptofile(GetToggleButtonState(printto_item));
        if (get_ptofile())
        {

            strcpy(print_file, xv_getstr(printfile_item));
        }
        else
        {
            //set_print_cmd(xv_getstr(print_string_item));
        }
    }

    dev.devfonts = GetToggleButtonState(devfont_item);
    dev.fontaa = GetToggleButtonState(fontaa_item);

    if (xv_evalexpr(page_x_item, &page_x) != RETURN_SUCCESS ||
            xv_evalexpr(page_y_item, &page_y) != RETURN_SUCCESS  )
    {
        errmsg(tr("Invalid page dimension(s)").toAscii().constData());
        ApplyError=true;
        goto end_dev_setup_apply;
    }

    if (xv_evalexpr(dev_res_item, &dpi) != RETURN_SUCCESS || dpi <= 0.0)
    {
        errmsg(tr("Invalid dpi").toAscii().constData());
        ApplyError=true;
        goto end_dev_setup_apply;
    }

    page_units = GetOptionChoice(page_size_unit_item);

    switch (page_units) {
    case 0:
        pg.width =  (long) page_x;
        pg.height = (long) page_y;
        break;
    case 1:
        pg.width =  (long) (page_x * dpi);
        pg.height = (long) (page_y * dpi);
        break;
    case 2:
        pg.width =  (long) (page_x * dpi / CM_PER_INCH);
        pg.height = (long) (page_y * dpi / CM_PER_INCH);
        break;
    default:
        errmsg(tr("Internal error").toAscii().constData());
        ApplyError=true;
        goto end_dev_setup_apply;
    }

    pg.dpi = dpi;

    if (GetToggleButtonState(dsync_item) == TRUE)
    {
        set_page_dimensions((int) rint(72.0*pg.width/pg.dpi),
                            (int) rint(72.0*pg.height/pg.dpi),
                            GetToggleButtonState(psync_item) == TRUE);
        do_redraw = TRUE;
    }

    dev.pg = pg;
    if (set_device_props(seldevice, dev) != RETURN_SUCCESS)
    {
        errmsg(tr("Invalid page dimensions or DPI").toAscii().constData());
        ApplyError=true;
        goto end_dev_setup_apply;
    }

    if (seldevice == tdevice)
    {
        do_redraw = TRUE;
    }

    DeviceModified(seldevice,GetToggleButtonState(dsync_item));

    cur_pg=get_page_geometry();
    new_small=cur_pg.width<cur_pg.height?cur_pg.width:cur_pg.height;

    if (new_small!=old_small && chkDontChangeSize->isChecked()==true && nr_of_gr>0)//relation between viewport and pixels is changed and the users wants the graphs not to be changed
    {
        int * gnos=new int[nr_of_gr+1];
        for (int i=0;i<nr_of_gr;i++)
            gnos[i]=i;
        SaveGraphStatesPrevious(nr_of_gr,gnos,UNDO_APPEARANCE);//call this before the modification
        for (int i=0;i<nr_of_gr;i++)
        {
            g[i].v.xv1/=new_small/old_small;
            g[i].v.xv2/=new_small/old_small;
            g[i].v.yv1/=new_small/old_small;
            g[i].v.yv2/=new_small/old_small;
            g[i].v.yv1-=((double)old_pg.height-(double)cur_pg.height)/new_small;
            g[i].v.yv2-=((double)old_pg.height-(double)cur_pg.height)/new_small;
        }
        GraphsModified(nr_of_gr,gnos,UNDO_APPEARANCE);//call this after the modification
        addAditionalDescriptionToLastNode(-1,tr("Graph(s) repositioned"),QString());
        delete[] gnos;
    }

    orig_page_w=device_table[0].pg.width;//save original size
    orig_page_h=device_table[0].pg.height;
    device_table[0].pg.width=orig_page_w*GeneralPageZoomFactor;//use Page Zoom
    device_table[0].pg.height=orig_page_h*GeneralPageZoomFactor;

    if (do_redraw)
    {
        set_dirtystate();
        mainWin->mainArea->completeRedraw();
    }
end_dev_setup_apply:
    ;//end
}

void frmDeviceSetup::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmDeviceSetup::doClose(void)
{
    hide();
}

void frmDeviceSetup::doPrint2(void)
{
    //bool old_state=printto_item->isChecked();

    doPrint();

    //printto_item->setChecked(old_state);
    //printto_item->setEnabled(true);
}

void frmDeviceSetup::doPrint(void)
{
    doApply();
    mainWin->Print();
}

void frmDeviceSetup::doSyncPage(void)
{}

void frmDeviceSetup::doRescalePlot(void)
{}

void frmDeviceSetup::doHelpOnContext(void)
{
    setCursor(QCursor(Qt::WhatsThisCursor));
}

void frmDeviceSetup::doHelpOnDevSetup(void)
{
    HelpCB("doc/UsersGuide.html#print-setup");
}

void frmDeviceSetup::doBrowse(void)
{
    //bool first_time=false;
    if (FormSelectOutputFile==NULL)
    {
        //first_time=true;
        FormSelectOutputFile=new frmIOForm(SELECT_PRINT_FILE,this);
        connect(FormSelectOutputFile,SIGNAL(newFileSelectedForIO(int,QString,bool,bool,bool)),SLOT(IOrequested(int,QString,bool,bool,bool)));
    }

    QFileInfo qf(printfile_item->text());

    int seldevice = GetOptionChoice(devices_item);
    Device_entry dev = get_device_props(seldevice);
    QString FileExtension(dev.fext);
    QString absPath=qf.absoluteDir().absolutePath();
    FileExtension=QString("*.")+FileExtension;
    FormSelectOutputFile->selector->setFilterFromExtern(absPath,FileExtension);

    FormSelectOutputFile->init();
    FormSelectOutputFile->ledSelection->setText(qf.absoluteFilePath());

    FormSelectOutputFile->show();
    FormSelectOutputFile->raise();
    FormSelectOutputFile->activateWindow();
}

void frmDeviceSetup::IOrequested(int type,QString file,bool exists,bool writeable,bool readable)
{
    printfile_item->setText(file);
    FormSelectOutputFile->hide();
}

void frmDeviceSetup::doDevOpt(void)
{
    int device_id;
    int dev_nr=-1;
    Device_entry dev;
    device_id = GetOptionChoice(devices_item);
    dev = get_device_props(device_id);
    if (dev.setup == NULL) {
        /* Should never come to here */
        errmsg(tr("No options can be set for this device").toAscii().constData());
    } else {
        dev_nr=find_dev_nr(dev.name);
        //(dev.setup)();
    }

    switch (dev_nr)
    {
    case DEVICE_PS:
        if (DevOptions[0]==NULL)
        {
            DevOptions[0]=new frmDeviceOptions(DEVICE_PS,this);
        }
        DevOptions[0]->init();
        DevOptions[0]->show();
        DevOptions[0]->raise();
        break;
    case DEVICE_EPS:
        if (DevOptions[1]==NULL)
        {
            DevOptions[1]=new frmDeviceOptions(DEVICE_EPS,this);
        }
        DevOptions[1]->init();
        DevOptions[1]->show();
        DevOptions[1]->raise();
        break;
    case DEVICE_PNM:
        if (DevOptions[2]==NULL)
        {
            DevOptions[2]=new frmDeviceOptions(DEVICE_PNM,this);
        }
        DevOptions[2]->init();
        DevOptions[2]->show();
        DevOptions[2]->raise();
        break;
    case DEVICE_JPEG:
        if (DevOptions[3]==NULL)
        {
            //DevOptions[3]=new frmDeviceOptions(DEVICE_JPEG,this);
            DevOptions[3]=new frmDeviceOptions(DEVICE_BMP,this);
            DevOptions[3]->setWindowTitle(tr("qtGrace: JPEG options"));
            DevOptions[3]->grpBMPoptions->setTitle(tr("JPEG options"));
        }
        DevOptions[3]->init();
        DevOptions[3]->show();
        DevOptions[3]->raise();
        break;
    case DEVICE_PNG:
        if (DevOptions[4]==NULL)
        {
            //DevOptions[4]=new frmDeviceOptions(DEVICE_PNG,this);
            DevOptions[4]=new frmDeviceOptions(DEVICE_BMP,this);
            DevOptions[4]->setWindowTitle(tr("qtGrace: PNG options"));
            DevOptions[4]->grpBMPoptions->setTitle(tr("PNG options"));
        }
        DevOptions[4]->init();
        DevOptions[4]->show();
        DevOptions[4]->raise();
        break;
    case DEVICE_BMP:
        if (DevOptions[5]==NULL)
        {
            DevOptions[5]=new frmDeviceOptions(DEVICE_BMP,this);
        }
        DevOptions[5]->init();
        DevOptions[5]->show();
        DevOptions[5]->raise();
        break;

    default:
        ;//Do nothing
        break;
    }
}

void frmDeviceSetup::doNativePrinterDialog(void)
{
    static char dummy[1024];
    this->hide();

    if (stdPrinter==NULL)
    {
        stdPrinter=new QPrinter();
        stdPrinter->setFromTo(1,1);
    }

    if (page_orient_item->currentIndex()==0)//landscape
    {
        stdPrinter->setOrientation(QPrinter::Landscape);
    }
    else//Portrait
    {
        stdPrinter->setOrientation(QPrinter::Portrait);
    }

    QPrintDialog printDialog(stdPrinter, this);
    if (printDialog.exec() == QDialog::Accepted)
    {
        if (printDialog.printer()->isValid())
        {
            useQPrinter=true;
            xdrawgraph();
            GeneralPainter->end();
            useQPrinter=false;
            xdrawgraph();
        }
        else
        {
            if (stdPrinter->outputFileName().isEmpty() || stdPrinter->outputFileName().isNull())
                strcpy(dummy,QString(QString(qt_grace_exe_dir) +QDir::separator()+ QString("qtGracePdfOutput.pdf")).toAscii());
            else
                strcpy(dummy,stdPrinter->outputFileName().toAscii());

            QSizeF size(MainPixmap->width()*1.0,MainPixmap->height()*1.0);
            stdPrinter->setPaperSize(size,QPrinter::DevicePixel);
            stdPrinter->setPageMargins(0.0,0.0,0.0,0.0,QPrinter::DevicePixel);
            stdPrinter->setResolution(72);

            if (stdPrinter->outputFormat()!=QPrinter::PdfFormat)
                stdPrinter->setOutputFormat(QPrinter::PdfFormat);

            if (stdPrinter->outputFileName().isEmpty() || stdPrinter->outputFileName().isNull())
                stdPrinter->setOutputFileName(dummy);

            useQPrinter=true;
            xdrawgraph();
            useQPrinter=false;
            xdrawgraph();
        }
    }
    else
    {
        this->show();
    }
    useQPrinter=false;
}

frmPreferences::frmPreferences(QWidget * parent):QDialog(parent)
{
    int number;
    QString entr[6>nr_of_translations?6:nr_of_translations];
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Preferences"));
    setWindowIcon(QIcon(*GraceIcon));

    grpResponciveness=new QGroupBox(tr("Responsiveness"),this);
    noask_item=new QCheckBox(tr("Don't ask questions"),grpResponciveness);
    noask_item->setChecked(FALSE); //ask questions default - Nimal
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
    max_path_item=new stdIntSelector(grpRestrictions,tr("Max drawing path length:"),0,1000000); //changed from 20000 - Nimal
    max_path_item->spnInt->setSingleStep(1000);
    max_path_item->setValue(1000000);
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
    wrap_year_item=new stdLineEdit(grpDates,tr("Wrap year:"));
    char dummy[5];
    strcpy(dummy,"1950");
    wrap_year_item->setText(QString(dummy));
    wrap_year_item->setEnabled(FALSE);
    two_digits_years_item=new QCheckBox(tr("Two-digit year span"),grpDates);
    connect(two_digits_years_item,SIGNAL(stateChanged(int)),SLOT(toggleWrapYear(int)));

    buttonGroup=new stdButtonGroup(this,true,true,true);
    buttonGroup->cmdHelp->setText(QString("Extra"));
    connect(buttonGroup->cmdHelp,SIGNAL(clicked()),this,SLOT(doExtraPreferences()));
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
    layout0=new QVBoxLayout();
    layout0->setMargin(STD_MARGIN);
    layout0->addWidget(noask_item);
    layout0->addWidget(dc_item);
    layout0->addWidget(graph_focus_choice_item);
    layout0->addWidget(graph_drawfocus_choice_item);
    layout0->addWidget(autoredraw_type_item);
    layout0->addWidget(cursor_type_item);
    ///layout0->addWidget(chkShowHideWorkaround);
    grpResponciveness->setLayout(layout0);
    layout1=new QVBoxLayout();
    layout1->setMargin(STD_MARGIN);
    layout1->addWidget(max_path_item);
    layout1->addWidget(safe_mode_item);
    grpRestrictions->setLayout(layout1);
    layout2=new QVBoxLayout();
    layout2->setMargin(STD_MARGIN);
    layout2->addWidget(scrollper_item);
    layout2->addWidget(shexper_item);
    layout2->addWidget(linkscroll_item);
    grpScrollZoom->setLayout(layout2);
    layout3=new QGridLayout();
    layout3->setMargin(STD_MARGIN);
    layout3->addWidget(hint_item,0,0,1,2);
    layout3->addWidget(date_item,1,0,1,2);
    layout3->addWidget(two_digits_years_item,2,0);
    layout3->addWidget(wrap_year_item,2,1);
    grpDates->setLayout(layout3);
    layout=new QVBoxLayout();
    layout->setMargin(STD_MARGIN);
    layout->addWidget(grpResponciveness);
    layout->addWidget(grpRestrictions);
    layout->addWidget(grpScrollZoom);
    layout->addWidget(grpDates);
    layout->addWidget(buttonGroup);

    layout->setStretch(0,1);
    layout->setStretch(1,1);
    layout->setStretch(2,1);
    layout->setStretch(3,1);
    layout->setStretch(4,0);

    setLayout(layout);
}

void frmPreferences::doExtraPreferences(void)
{
    if (ExtraPreferences==NULL)
    {
        ExtraPreferences=new frmExtraPreferences(this);
        ExtraPreferences->init();
    }
    ExtraPreferences->show();
    ExtraPreferences->raise();
    ExtraPreferences->activateWindow();
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
        errwin(tr("Debug level > 8, resetting to 0").toAscii().constData());
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
    SetSpinChoice(max_path_item, (double) get_max_path_limit());
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
    jul_to_cal_and_time(0.0, ROUND_SECOND, &y, &m, &d, &h, &mm, &sec);
    sprintf(date_string, "%d-%02d-%02d %02d:%02d:%02d",
            y, m, d, h, mm, sec);
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
    noask = GetToggleButtonState(noask_item);
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
    if (parse_date_or_number(xv_getstr(date_item), TRUE, &jul)
            == RETURN_SUCCESS) {
        set_ref_date(jul);
    } else {
        errmsg(tr("Invalid date").toAscii().constData());
        ApplyError=true;
    }
    allow_two_digits_years(GetToggleButtonState(two_digits_years_item));
    set_wrap_year(atoi(xv_getstr(wrap_year_item)));
    ///change_language(selLanguage->currentIndex());

    xdrawgraph();
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
    mainWin->mainArea->completeRedraw();
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
    hide();
}

frmExtraPreferences::frmExtraPreferences(QWidget * parent):QDialog(parent)
{
    setWindowTitle(tr("qtGrace: Extra Preferences"));

    actClose=new QAction(tr("&Close"),this);
    actClose->setShortcut(tr("Esc"));
    connect(actClose,SIGNAL(triggered()), this, SLOT(doClose()));
    actLoad=new QAction(tr("&Load"),this);
    connect(actLoad,SIGNAL(triggered()), this, SLOT(doLoad()));
    actSave=new QAction(tr("&Save"),this);
    connect(actSave,SIGNAL(triggered()), this, SLOT(doSave()));

    /*menuBar=new QMenuBar();
mnuFile=new QMenu("&File",this);
mnuFile->setTearOffEnabled(TRUE);
mnuFile->addAction(actLoad);
mnuFile->addAction(actSave);
mnuFile->addSeparator();
mnuFile->addAction(actClose);
menuBar->addMenu(mnuFile);*/

    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    //layout->addWidget(menuBar);
    lenHome=new stdLineEdit(this,tr("Grace Home directory:"));
    layout->addWidget(lenHome);
    chkExternalHelpViewer=new QCheckBox(tr("Show help-files in external html-viewer"),this);
    layout->addWidget(chkExternalHelpViewer);
    connect(chkExternalHelpViewer,SIGNAL(stateChanged(int)),SLOT(toggleHTMLviewer(int)));
    lenHelpViewer=new stdLineEdit(this,tr("Help viewer:"));
    layout->addWidget(lenHelpViewer);
    lenHelpViewer->setEnabled(false);
    int nr=2;
    QString entr[32];
    entr[0]=QString("English");
    entr[1]=QString("German");
    selLanguage=new StdSelector(this,tr("Language"),nr,entr);
    layout->addWidget(selLanguage);

    QString * entr3=new QString[avcod.length()];
    for (int i=0;i<avcod.length();i++)
    {
        entr3[i]=QString(avcod.at(i));
    }
    selCodec=new StdSelector(this,tr("File encoding"),avcod.length(),entr3);
    layout->addWidget(selCodec);
    delete[] entr3;

    chkShowHideException=new QCheckBox(tr("Show/Hide workaround"),this);
    layout->addWidget(chkShowHideException);
    chkActivateLaTeXSupport=new QCheckBox(tr("Support simple LaTeX-commands"),this);
    layout->addWidget(chkActivateLaTeXSupport);
    chkImmediateUpdate=new QCheckBox(tr("Immediate updates"),this);
    layout->addWidget(chkImmediateUpdate);
    histSize=new stdIntSelector(this,tr("Maximum history size:"),0,MAX_HISTORY);
    histSize->setValue(max_history);
    layout->addWidget(histSize);
    nr=number_of_devices();
    entr[0]=tr("Last selection");
    for (int i=0;i<nr;i++)
    {
        entr[i+1]=get_device_name(i);
    }
    selDefaultPrintDevice=new StdSelector(this,tr("Default printing device:"),nr,entr);
    layout->addWidget(selDefaultPrintDevice);

    buttonGroup=new stdButtonGroup(this,true,true,false);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));
    layout->addWidget(buttonGroup);
    lenHome->setVisible(false);
    setLayout(layout);
}

void frmExtraPreferences::init(void)
{
    lenHome->setText(QString(GRACE_HOME));
    lenHelpViewer->setText(QString(GRACE_HELPVIEWER));
    selLanguage->setCurrentIndex(current_language);
    chkShowHideException->setChecked(showhideworkaround);
    histSize->setValue(max_history);
    chkActivateLaTeXSupport->setChecked(activateLaTeXsupport);
    chkImmediateUpdate->setChecked(immediateUpdate);
    selDefaultPrintDevice->setCurrentIndex(default_Print_Device+1);
}

void frmExtraPreferences::doLoad(void)
{
    //cout << "load parameters" << endl;
}

void frmExtraPreferences::doSave(void)
{
    //cout << "save parameters" << endl;
}

void frmExtraPreferences::doApply(void)
{
    ApplyError=false;
    int save_dirty=dirtystate;
    int newlanguage=selLanguage->currentIndex();
    FileCodec=QTextCodec::codecForName(selCodec->cmbSelect->currentText().toAscii().constData());
    display_help_externally=chkExternalHelpViewer->isChecked();
    showhideworkaround=chkShowHideException->isChecked();
    activateLaTeXsupport=chkActivateLaTeXSupport->isChecked();
    immediateUpdate=chkImmediateUpdate->isChecked();
    default_Print_Device=selDefaultPrintDevice->currentIndex()-1;
    max_history=histSize->value();
    if (newlanguage!=current_language)
    {
        current_language=newlanguage;
        char dummy[512];
        strcpy(dummy,"[Warning] Changing the language takes effekt after restart of QtGrace.");
        stufftext(dummy);
    }
    dirtystate=save_dirty;
    mainWin->recreateHistory();
}

void frmExtraPreferences::toggleHTMLviewer(int entry)
{
    lenHelpViewer->setEnabled((bool)entry);
}

void frmExtraPreferences::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmExtraPreferences::doClose(void)
{
    hide();
}

int yesnowin(char * msg,char * s1,char * s2,char * help_anchor)
{
    if (s1==NULL)
        return QMessageBox::question(0,QString("Error"),QString(msg),QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes?1:0;
    else
        return QMessageBox::question(0,QString(s1),QString(msg) + QString(s2),QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes?1:0;
}

void showSetInSpreadSheet(int gno,int setno)
{
    int found=-1;

    /// remove this!!
    /*frmSpreadSheet3 * spread3=new frmSpreadSheet3(0,gno,setno);
spread3->show();
spread3->table->setEditTriggers(QAbstractItemView::AllEditTriggers);
spread3->table->update();*/

    for (int i=0;i<number_of_opened_spreadsheets;i++)
    {
        if (FormSpreadSheets[i]!=NULL)
            if (FormSpreadSheets[i]->gno==gno && FormSpreadSheets[i]->sno==setno)
            {
                found=i;
                break;
            }
    }
    if (found>=0)
    {
        FormSpreadSheets[found]->show();
        qApp->processEvents();
        FormSpreadSheets[found]->init(FormSpreadSheets[found]->gno,FormSpreadSheets[found]->sno);
        FormSpreadSheets[found]->raise();
    }
    else//new one needed
    {
        frmSpreadSheet ** sheets=new frmSpreadSheet *[number_of_opened_spreadsheets+1];
        for (int i=0;i<number_of_opened_spreadsheets;i++)
            sheets[i]=FormSpreadSheets[i];
        delete[] FormSpreadSheets;
        FormSpreadSheets=new frmSpreadSheet *[number_of_opened_spreadsheets+1];
        for (int i=0;i<number_of_opened_spreadsheets;i++)
            FormSpreadSheets[i]=sheets[i];
        delete[] sheets;
        FormSpreadSheets[number_of_opened_spreadsheets]=new frmSpreadSheet(0);
        FormSpreadSheets[number_of_opened_spreadsheets]->init(gno,setno);
        FormSpreadSheets[number_of_opened_spreadsheets]->show();
        FormSpreadSheets[number_of_opened_spreadsheets]->raise();
        FormSpreadSheets[number_of_opened_spreadsheets]->activateWindow();
        number_of_opened_spreadsheets++;
    }
}

void deleteSpreadSheet(int gno,int setno)
{
    int found=-1;
    for (int i=0;i<number_of_opened_spreadsheets;i++)
    {
        if (FormSpreadSheets[i]->gno==gno && FormSpreadSheets[i]->sno==setno)
        {
            found=i;
            break;
        }
    }
    //not needed any more !?
}

void get_tracking_props(int *setno, int *move_dir, int *add_at)
{
    *setno = track_setno;
    *move_dir = track_move_dir;
    *add_at = track_add_at;
}

frmArrangeGraphs::frmArrangeGraphs(QWidget * parent):QDialog(parent)
{
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Arrange Graphs"));
    setWindowIcon(QIcon(*GraceIcon));

    grpArrGraphs=new QGroupBox(QString(""),this);
    grpMatrix=new QGroupBox(tr("Matrix"),this);
    grpPageOffset=new QGroupBox(tr("Page offset"),this);
    grpSpacing=new QGroupBox(tr("Spacing"),this);

    lblArrGraphs=new QLabel(tr("Arrange graphs:"),grpArrGraphs);

    graphlist=new uniList(GRAPHLIST,grpArrGraphs);
    graphlist->setSelectionMode(QAbstractItemView::ExtendedSelection);

    selCols=new stdIntSelector(grpMatrix,tr("Cols:"),1,99);
    selRows=new stdIntSelector(grpMatrix,tr("Rows:"),1,99);
    selUpperOffset=new QDoubleSpinBox(grpPageOffset);
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
    selRightOffset->setValue(0.15);

    selHGap=new LineWidthSelector(grpSpacing);
    selHGap->lblText->setText(tr("Hgap/width:"));
    selHGap->spnLineWidth->setRange(0.0,9.0);
    selHGap->spnLineWidth->setSingleStep(0.1);
    selHGap->spnLineWidth->setValue(0.2);
    selVGap=new LineWidthSelector(grpSpacing);
    selVGap->lblText->setText(tr("Vgap/height:"));
    selVGap->spnLineWidth->setRange(0.0,9.0);
    selVGap->spnLineWidth->setSingleStep(0.1);
    selVGap->spnLineWidth->setValue(0.2);

    chkAddGraphs=new QCheckBox(tr("Add graphs as needed to fill the matrix"),grpArrGraphs);
    chkAddGraphs->setChecked(TRUE);
    chkKillGraphs=new QCheckBox(tr("Kill extra graphs"),grpArrGraphs);
    chkSnakeFill=new QCheckBox(tr("Snake fill"),grpMatrix);
    chkHPack=new QCheckBox(tr("Pack"),grpSpacing);
    chkVPack=new QCheckBox(tr("Pack"),grpSpacing);

    selOrder=new OrderSelector(grpMatrix);

    buttonGroup=new stdButtonGroup(this);
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));

    layout0=new QVBoxLayout();
    layout0->setMargin(STD_MARGIN);
    layout0->addWidget(lblArrGraphs);
    layout0->addWidget(graphlist);
    layout0->addWidget(chkAddGraphs);
    layout0->addWidget(chkKillGraphs);
    grpArrGraphs->setLayout(layout0);

    layout1=new QHBoxLayout();
    layout1->setMargin(STD_MARGIN);
    layout1->addWidget(selCols);
    layout1->addWidget(selRows);
    layout1->addWidget(selOrder);
    layout1->addWidget(chkSnakeFill);
    grpMatrix->setLayout(layout1);

    layout2=new QGridLayout();
    layout2->setMargin(STD_MARGIN);
    layout2->addWidget(selUpperOffset,0,2);
    layout2->addWidget(selLowerOffset,2,2);
    layout2->addWidget(selLeftOffset,1,0);
    layout2->addWidget(selRightOffset,1,4);
    grpPageOffset->setLayout(layout2);

    layout3=new QHBoxLayout();
    layout3->setMargin(STD_MARGIN);
    layout3->addWidget(selHGap);
    layout3->addWidget(chkHPack);
    layout3->addWidget(selVGap);
    layout3->addWidget(chkVPack);
    grpSpacing->setLayout(layout3);

    layout=new QVBoxLayout();
    layout->setMargin(STD_MARGIN);
    layout->addWidget(grpArrGraphs);
    layout->addWidget(grpMatrix);
    layout->addWidget(grpPageOffset);
    layout->addWidget(grpSpacing);
    layout->addWidget(buttonGroup);
    setLayout(layout);
}

void frmArrangeGraphs::init(void)
{
    graphlist->update_number_of_entries();
    int *sel=new int[2];
    sel[0]=get_cg();
    graphlist->set_new_selection(1,sel);
    delete[] sel;
}

void frmArrangeGraphs::doApply(void)
{
    ApplyError=false;
    int ngraphs, * graphs=new int[2];
    int nrows, ncols, order, snake;
    int hpack, vpack, add, kill;
    double toff, loff, roff, boff, vgap, hgap;

    nrows = (int) GetSpinChoice(selRows);
    ncols = (int) GetSpinChoice(selCols);

    if (nrows < 1 || ncols < 1)
    {
        errmsg(tr("# of rows and columns must be > 0").toAscii().constData());
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
                   hpack, vpack);

    //Undo-Staff
    GraphsModified(ngraphs,graphs,UNDO_APPEARANCE);
    addAditionalDescriptionToLastNode(UNDO_TYPE_GRAPH_APPEARANCE,QString(),tr("rearranged"));

    update_all();
    
    ///SelectListChoices(ui->graphs, ngraphs, graphs);
    //xfree(graphs);
    graphlist->update_number_of_entries();
    graphlist->set_new_selection(ngraphs,graphs);

    mainWin->mainArea->completeRedraw();
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

frmOverlayGraphs::frmOverlayGraphs(QWidget * parent):QDialog(parent)
{
    int number=5;
    QString entr[5];
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Overlay Graphs"));
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
    layout->setMargin(STD_MARGIN);
    layout->addWidget(lblOverlayGraph);
    layout->addWidget(listOverlayGraph);
    layout->addWidget(lblOntoGraph);
    layout->addWidget(listOntoGraph);
    layout->addWidget(selSmartAxisHint);
    layout->addWidget(buttonGroup);
    setLayout(layout);
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
        errmsg(tr("Please select a single graph").toAscii().constData());
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
        errmsg(tr("Please select a single graph").toAscii().constData());
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
        errmsg(tr("Can't overlay a graph onto itself").toAscii().constData());
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
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Autoscale Graphs"));
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
    layout->setMargin(STD_MARGIN);
    layout->addWidget(selAutoscale);
    layout->addWidget(lblUseSet);
    layout->addWidget(listSets);
    layout->addWidget(selApplyToGraph);
    layout->addWidget(buttonGroup);
    setLayout(layout);
}

void frmAutoscaleGraphs::init(void)
{
    selAutoscale->setCurrentIndex(3);
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
        errmsg(tr("No set selected").toAscii().constData());
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
        errmsg(tr("Set not active").toAscii().constData());
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
            errmsg(tr("Current graph is not active!").toAscii().constData());
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

frmDataSetProperties::frmDataSetProperties(QWidget * parent):QDialog(parent)
{
    int number;
    char dummy[128];
    QString entr[50];
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Data set properties"));
    setWindowIcon(QIcon(*GraceIcon));

    CreateActions();

    menuBar=new QMenuBar();
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
    layout0->setMargin(STD_MARGIN);
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
    layout1->setMargin(STD_MARGIN);
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
    layout->setMargin(STD_MARGIN);
    layout->addWidget(menuBar,0,0,1,2);
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
    int gno=listDataSets->gr_no;
    int sno=nr;
    int ncols = dataset_cols(gno, sno);
    int number_of_sel_sets=-1;
    int * selection=new int[2];
    listDataSets->get_selection(&number_of_sel_sets,&selection);
    if (number_of_sel_sets!=1)
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
    bool act_sav=undo_active;
    undo_active=false;
    listDataSets->hide();
    listDataSets->show();
    undo_active=act_sav;
    repaint();
}

void frmDataSetProperties::doDuplicate(void)
{
    listDataSets->popupMenu1->doDuplicate();
    bool act_sav=undo_active;
    undo_active=false;
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
    char *s;
    int * gnos;
    int index;

    nsets = GetListChoices(listDataSets, &selset);

    if (nsets < 1) {
        errmsg(tr("No set selected").toAscii().constData());
        ApplyError=true;
    } else {
        type = GetOptionChoice(selType);
        xv_evalexpri(ledLength, &len);
        if (len < 0)
        {
            errmsg(tr("Negative set length!").toAscii().constData());
            ApplyError=true;
            error = TRUE;
        }
        s = xv_getstr(ledComment);

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
                setcomment(get_cg(), setno, s);
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
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Set operations"));
    setWindowIcon(QIcon(*GraceIcon));

    grpSource=new grpSelect(tr("Source"),this);
    grpDestination=new grpSelect(tr("Destination"),this);

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
    layout->setMargin(STD_MARGIN);

    layout->addWidget(grpSource,0,0);
    layout->addWidget(grpDestination,0,1);
    layout->addWidget(selType,1,0,1,2);
    layout->addWidget(buttonGroup,2,0,1,2);
    setLayout(layout);
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
        errmsg(tr("Please select single source and destination graphs").toAscii().constData());
    } else if (ns1 == 0) {
        error = TRUE;
        ApplyError=true;
        errmsg(tr("No source sets selected").toAscii().constData());
    } else if (ns2 == 0 && (optype == OPTYPE_SWAP || optype == OPTYPE_COPY_LEGEND)) {
        error = TRUE;
        ApplyError=true;
        errmsg(tr("No destination sets selected").toAscii().constData());
    } else if (ns1 != ns2 && (optype == OPTYPE_SWAP || optype == OPTYPE_COPY_LEGEND || ns2 != 0)) {
        error = TRUE;
        ApplyError=true;
        errmsg(tr("Different number of source and destination sets").toAscii().constData());
    } else if (gno1 == gno2 && ns2 == 0 && optype == OPTYPE_MOVE) {
        error = TRUE;
        ApplyError=true;
        errmsg(tr("Can't move a set to itself").toAscii().constData());
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

frmFeatureExtract::frmFeatureExtract(QWidget * parent):QDialog(parent)
{
    int number;
    QString entr[32];
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Feature extraction"));
    setWindowIcon(QIcon(*GraceIcon));

    lblResultGraph=new QLabel(tr("Results to graph:"),this);
    listResultGraph=new uniList(GRAPHLIST,this);
    lblAbscissaGraph=new QLabel(tr("Abscissa from graph:"),this);
    listAbscissaGraph=new uniList(GRAPHLIST,this);
    lblSet=new QLabel(tr("set:"),this);
    listSet=new uniList(SETLIST,this);

    buttonGroup=new stdButtonGroup(this,false,true,false);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));

    number=24;
    entr[0]=tr("Y minimum");
    entr[1]=tr("Y maximum");
    entr[2]=tr("Y average");
    entr[3]=tr("Y std. dev.");
    entr[4]=tr("Y median");
    entr[5]=tr("X minimum");
    entr[6]=tr("X maximum");
    entr[7]=tr("X average");
    entr[8]=tr("X std. dev.");
    entr[9]=tr("X median");
    entr[10]=tr("Frequency");
    entr[11]=tr("Period");
    entr[12]=tr("Zero crossing");
    entr[13]=tr("Rise time");
    entr[14]=tr("Fall time");
    entr[15]=tr("Slope");
    entr[16]=tr("Y intercept");
    entr[17]=tr("Set length");
    entr[18]=tr("Half maximal width");
    entr[19]=tr("Barycenter X");
    entr[20]=tr("Barycenter Y");
    entr[21]=tr("X(Y max)");
    entr[22]=tr("Y(X max)");
    entr[23]=tr("Integral");
    selFeature=new StdSelector(this,tr("Feature:"),number,entr);
    number=4;
    entr[0]=tr("Index");
    entr[1]=tr("Legends");
    entr[2]=tr("X from Set");
    entr[3]=tr("Y from Set");
    selXValue=new StdSelector(this,tr("X values from:"),number,entr);
    connect(selXValue->cmbSelect,SIGNAL(currentIndexChanged(int)),SLOT(XValueChanged(int)));

    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(lblResultGraph);
    layout->addWidget(listResultGraph);
    layout->addWidget(selFeature);
    layout->addWidget(selXValue);
    layout->addWidget(lblAbscissaGraph);
    layout->addWidget(listAbscissaGraph);
    layout->addWidget(lblSet);
    layout->addWidget(listSet);
    layout->addWidget(buttonGroup);
    setLayout(layout);
    lblAbscissaGraph->setEnabled(FALSE);
    listAbscissaGraph->setEnabled(FALSE);
    lblSet->setEnabled(FALSE);
    listSet->setEnabled(FALSE);
}

void frmFeatureExtract::init(void)
{
    listResultGraph->update_number_of_entries();
    listAbscissaGraph->update_number_of_entries();

    int nr_sel=1;
    int sel=get_cg();
    listResultGraph->set_new_selection(nr_sel,&sel);
    listAbscissaGraph->set_new_selection(nr_sel,&sel);
    listSet->set_graph_number(sel,true);
    //listSet->update_number_of_entries();
}

void frmFeatureExtract::XValueChanged(int i)
{
    if (i == 2 || i == 3)
    {
        lblAbscissaGraph->setEnabled(TRUE);
        listAbscissaGraph->setEnabled(TRUE);
        lblSet->setEnabled(TRUE);
        listSet->setEnabled(TRUE);
    }
    else
    {
        lblAbscissaGraph->setEnabled(FALSE);
        listAbscissaGraph->setEnabled(FALSE);
        lblSet->setEnabled(FALSE);
        listSet->setEnabled(FALSE);
    }
}

void frmFeatureExtract::doClose(void)
{
    hide();
}

void frmFeatureExtract::doAccept(void)
{
    int gto, feature, abs_graph = -1, abs_set = -1, abs_src;
    int n_sel,*sel=new int[2];

    feature = selFeature->currentIndex();//GetOptionChoice(ui->feature_item);
    //GetSingleListChoice(ui->tograph, &gto);
    listResultGraph->get_selection(&n_sel,&sel);
    if (n_sel<=0)
    {gto=-1;}
    else
    {gto=sel[0];}
    if( gto == -1 )
        gto = get_cg();

    abs_src =selXValue->currentIndex();// (int) GetChoice(ui->xval_item);
    if( abs_src ==2 || abs_src==3 )
    {
        listSet->get_selection(&n_sel,&sel);
        if (n_sel<=0)
        {abs_set=-1;}
        else
        {abs_set=sel[0];}
        //abs_set = GetSelectedSet(ui->absic_set);
        listAbscissaGraph->get_selection(&n_sel,&sel);
        if (n_sel<=0)
        {abs_graph=get_cg();}
        else
        {abs_graph=sel[0];}
        //GetSingleListChoice(ui->absic_graph, &abs_graph);
    }
    new_set_no=-1;
    fext_routine( gto, feature, abs_src, abs_set, abs_graph );

    ///Undo-Stuff
    int gnos[2]={gto,gto};
    int snos[2]={new_set_no,new_set_no};
    SetsCreated(1,gnos,snos,UNDO_COMPLETE);
    addAditionalDescriptionToLastNode(-1,QObject::tr("Feature extraction"),QString());

    update_set_lists(gto);
    mainWin->mainArea->completeRedraw();
    listResultGraph->update_number_of_entries();
    listAbscissaGraph->update_number_of_entries();
    listSet->update_number_of_entries();
    delete[] sel;
}

void frmFeatureExtract::fext_routine( int gto, int feature, int abs_src, int abs_set, int abs_graph )
{
    int i, cs, ns, fts, ncurves, extract_err;
    double datum, dummy, *absy;
    double y1, y2;
    int iy1, iy2;
    char tbuf[1024];
    float *abscissa;
    double xmin, xmax, ymin, ymax;
    double *x;

    abscissa = (float*)xmalloc(number_of_sets(cg)*sizeof(float));

    if( !is_graph_active( gto )	){
        FormConsole->errwin(tr("Graph for results must be active").toAscii().constData());
        return;
    }
    if( (new_set_no=ns=nextset( gto ) )== -1 ) {
        FormConsole->errwin(tr("Choose a new graph or kill sets!").toAscii().constData());
        return;
    }
    ncurves = nactive(get_cg());
    //cout << "gto=" << gto << " feature=" << feature << " abs_src=" << abs_src << " abs_set=" << abs_set << " abs_graph=" << abs_graph << " ncurves=" << ncurves << endl;
    switch( abs_src ) {
    case 0:		/* use index */
        for( i=0; i<ncurves; i++ )
            abscissa[i] = i;
        break;
    case 1:		/* use legend label */
        cs = 0;
        for( i=0; i<ncurves; i++ ){
            while( !is_set_active( get_cg(), cs ) )
                cs++;
            if(!sscanf( get_legend_string(get_cg(), cs), "%f", &abscissa[i]))
                break;
            cs++;
        }
        if( i != ncurves ) {
            FormConsole->errwin(tr("Bad legend label").toAscii().constData());
            return;
        }
        break;
    case 2:     /* use X from set */
        if( !is_set_active( abs_graph, abs_set ) ){
            FormConsole->errwin(tr("Abscissa set not active").toAscii().constData());
            return;
        }
        if( getsetlength( abs_graph, abs_set ) < ncurves ) {
            FormConsole->errwin(tr("Not enough points in set").toAscii().constData());
            return;
        }
        absy = getx( abs_graph, abs_set );
        for( i=0; i<ncurves; i++ )
            abscissa[i] = absy[i];
        break;
    case 3:     /* use Y from set */
        if( !is_set_active( abs_graph, abs_set ) ){
            FormConsole->errwin(tr("Abscissa set not active").toAscii().constData());
            return;
        }
        if( getsetlength( abs_graph, abs_set ) < ncurves ) {
            FormConsole->errwin(tr("Not enough points in set").toAscii().constData());
            return;
        }
        absy = gety( abs_graph, abs_set );
        for( i=0; i<ncurves; i++ )
            abscissa[i] = absy[i];
        break;
    }

    cs = 0;
    tbuf[0] = '\0';
    for( i=0; i<ncurves; i++ ) {
        while( !is_set_active( get_cg(), cs ) )
            cs++;
        extract_err = 0;

        getsetminmax(get_cg(), cs, &xmin, &xmax, &ymin, &ymax);
        switch( feature ) {
        case 0:			/* Y minimum */
            datum = ymin;
            break;
        case 1: 		/* Y maximum */
            datum = ymax;
            break;
        case 2: 		/* Y mean    */
            stasum(gety(cg, cs), getsetlength(cg, cs), &datum, &dummy);
            break;
        case 3:			/* Y std dev */
            stasum(gety(cg, cs), getsetlength(cg, cs), &dummy, &datum);
            break;
        case 4: 		/* Y median  */
            getmedian( cg, cs, DATA_Y, &datum );
            break;
        case 5:			/* X minimum */
            datum = xmin;
            break;
        case 6: 		/* X maximum */
            datum = xmax;
            break;
        case 7: 		/* X mean    */
            stasum(getx(cg, cs), getsetlength(cg, cs), &datum, &dummy);
            break;
        case 8:			/* X std dev */
            stasum(getx(cg, cs), getsetlength(cg, cs), &dummy, &datum);
            break;
        case 9:			/* X median  */
            getmedian( cg, cs, DATA_X, &datum );
            break;
        case 10: 		/* frequency and period */
        case 11:
            //cout << cg << " " << cs << " " << getsetlength(cg, cs) << " " << ilog2(getsetlength(cg, cs)) << endl;
            if ( ilog2(getsetlength(cg, cs)) <= 0)   	 /* only DFT */
                do_fourier(cg, cs, 0, 0, 1, 0, 0, 0);
            else		                                 /* FFT      */
                do_fourier(cg, cs, 1, 0, 1, 0, 0, 0);
            if (ilog2(getsetlength(cg, cs)) != 0)
            {
                sprintf( tbuf, "FT of set %d", cs );
                fts = 0;
                while( strcmp( tbuf, getcomment(cg, fts)+1 ) ){
                    fts++;
                }
                minmax(gety(cg, fts), getsetlength(cg, fts),&y1,&y2,&iy1,&iy2);
                x = getx(cg, fts);
                if( feature == 10 )
                    datum = x[iy2];
                else
                    datum = 1./x[iy2];
                killset( cg, fts );				/* get rid of Fourier set */
            }
            break;
        case 12:		/* first zero crossing */
            if( get_zero_crossing( getsetlength( cg, cs ), getx( cg, cs ),gety( cg, cs ), &datum ) ){
                sprintf( tbuf+strlen(tbuf),"%s%d\n",tr("Unable to find zero crossing of set ").toAscii().constData(), cs );
                FormConsole->errwin( tbuf );
                extract_err = 1;
            }
            break;
        case 13:		/* rise time   */
            getsetminmax(cg, cs, &xmin, &xmax, &ymin, &ymax);
            if( get_rise_time( getsetlength(cg,cs), getx(cg,cs),
                               gety(cg,cs), ymin, ymax, &datum ) ){
                sprintf( tbuf+strlen(tbuf),"%s%d\n",tr("Unable to find rise time of set ").toAscii().constData(), cs );
                FormConsole->errwin( tbuf );
                extract_err = 1;
            }
            break;
        case 14: 		/* fall time   */
            getsetminmax(cg, cs, &xmin, &xmax, &ymin, &ymax);
            if( get_fall_time( getsetlength(cg,cs), getx(cg,cs),
                               gety(cg,cs), ymin, ymax, &datum ) ){
                sprintf( tbuf+strlen(tbuf),"%s%d\n",tr("Unable to find fall time of set ").toAscii().constData(), cs );
                extract_err = 1;
                FormConsole->errwin( tbuf );
            }
            break;
        case 15:		/* slope       */
            if( mute_linear_regression( getsetlength( cg, cs ),
                                        getx( cg, cs ),gety( cg, cs ), &datum, &dummy ) ) {
                sprintf( tbuf+strlen(tbuf),"%s%d\n",tr("Unable to find slope of set ").toAscii().constData(), cs );
                FormConsole->errwin( tbuf );
                extract_err = 1;
            }
            break;
        case 16:		/* Y intercept */
            if( mute_linear_regression( getsetlength( cg, cs ),getx( cg, cs ), gety( cg, cs ), &dummy, &datum ) ) {
                sprintf( tbuf+strlen(tbuf),"%s%d\n",tr("Unable to find y-intercept of set ").toAscii().constData(), cs );
                FormConsole->errwin( tbuf );
                extract_err = 1;
            }
            break;
        case 17:		/* set length  */
            datum = getsetlength( cg, cs );
            break;
        case 18:		/* half maximal widths */
            if (get_half_max_width(getsetlength(cg, cs), getx(cg,cs),
                                   gety(cg,cs), &datum) != RETURN_SUCCESS) {
                sprintf( tbuf+strlen(tbuf),"%s%d\n",tr("Unable to find half maximal width of set ").toAscii().constData(), cs );
                extract_err = 1;
                FormConsole->errwin( tbuf );
            }
            break;
        case 19:		/* Barycenter X */
            get_barycenter( getsetlength( cg, cs ), gety(cg,cs), getx(cg,cs), &datum );
            break;
        case 20:		/* Barycenter Y */
            get_barycenter( getsetlength( cg, cs ), getx(cg,cs), gety(cg,cs), &datum );
            break;
        case 21:		/* X of Maximum Y */
            getsetminmax(cg, cs, &xmin, &xmax, &ymin, &ymax);
            get_max_pos( gety(cg, cs), getx( cg, cs ), getsetlength( cg, cs ), ymax, &datum );
            break;
        case 22:		/* Y of Maximum X */
            getsetminmax(cg, cs, &xmin, &xmax, &ymin, &ymax);
            get_max_pos( getx(cg, cs), gety( cg, cs ), getsetlength( cg, cs ), xmax, &datum );
            break;
        case 23:		/* cumulative sum */
            datum = do_int(cg, cs, 1);
            break;
        }
        if( !extract_err )
            add_point(gto, ns, abscissa[i], datum);
        cs++;
    }

    /* set comment */
    switch( feature ) {
    case 0:			/* Y minimum */
        sprintf(tbuf,"Y minima of graph %d",cg);
        break;
    case 1: 		/* Y maximum */
        sprintf(tbuf,"Y maxima of graph %d",cg);
        break;
    case 2: 		/* Y mean    */
        sprintf(tbuf,"Y means of graph %d",cg);
        break;
    case 3:			/* Y std dev */
        sprintf(tbuf,"Y std. dev.'s of graph %d",cg);
        break;
    case 4:			/* Y median  */
        sprintf(tbuf,"Y medians of graph %d",cg);
        break;
    case 5:			/* X minimum */
        sprintf(tbuf,"X minima of graph %d",cg);
        break;
    case 6: 		/* X maximum */
        sprintf(tbuf,"X maxima of graph %d",cg);
        break;
    case 7: 		/* X mean    */
        sprintf(tbuf,"X means of graph %d",cg);
        break;
    case 8:			/* X std dev */
        sprintf(tbuf,"X std. dev.'s of graph %d",cg);
        break;
    case 9:			/* X median  */
        sprintf(tbuf,"X medians of graph %d",cg);
        break;
    case 10: 		/* frequency and period */
        sprintf(tbuf,"frequencies of graph %d",cg);
        break;
    case 11:
        sprintf(tbuf,"periods of graph %d",cg);
        break;
    case 12:		/* first zero crossing */
        sprintf(tbuf,"zero crossings of graph %d",cg);
        break;
    case 13:		/* rise time */
        sprintf(tbuf,"rise times of graph %d",cg);
        break;
    case 14: 		/* fall time */
        sprintf(tbuf,"fall times of graph %d",cg);
        break;
    case 15: 		/* slopes     */
        sprintf(tbuf,"slopes of graph %d",cg);
        break;
    case 16: 		/* Y intercepts */
        sprintf(tbuf,"Y intercepts of graph %d",cg);
        break;
    case 17: 		/* set lengths */
        sprintf(tbuf,"set lengths of graph %d",cg);
        break;
    case 18: 		/* 1/2 maximal widths */
        sprintf(tbuf,"half maximal widths of graph %d",cg);
        break;
    case 19: 		/* barycenter X */
        sprintf(tbuf,"X barycenters of graph %d",cg);
        break;
    case 20: 		/* barycenter Y */
        sprintf(tbuf,"Y barycenters of graph %d",cg);
        break;
    case 21:		/* X of maximum Y */
        sprintf(tbuf,"X positions of maximum Y's of graph %d",cg);
        break;
    case 22:		/* Y of maximum X */
        sprintf(tbuf,"Y positions of maximum X's of graph %d",cg);
        break;
    case 23:		/* integral */
        sprintf(tbuf,"integrals of sets of graph %d",cg);
        break;
    }
    set_set_hidden(gto, ns, FALSE);
    setcomment( gto, ns, tbuf );
    xfree( abscissa );
}

/* linear regression without posting results to log */
int frmFeatureExtract::mute_linear_regression(int n, double *x, double *y, double *slope, double *intercept)
{
    double xbar, ybar;		/* sample means */
    double sdx, sdy;		/* sample standard deviations */
    double SXX, SYY, SXY;	/* sums of squares */
    int i;

    if (n <= 3) {
        return 1;
    }
    xbar = ybar = 0.0;
    SXX = SYY = SXY = 0.0;
    for (i = 0; i < n; i++) {
        xbar = xbar + x[i];
        ybar = ybar + y[i];
    }
    xbar = xbar / n;
    ybar = ybar / n;
    for (i = 0; i < n; i++) {
        SXX = SXX + (x[i] - xbar) * (x[i] - xbar);
        SYY = SYY + (y[i] - ybar) * (y[i] - ybar);
        SXY = SXY + (x[i] - xbar) * (y[i] - ybar);
    }
    sdx = sqrt(SXX / (n - 1));
    sdy = sqrt(SYY / (n - 1));
    if (sdx == 0.0) {
        return 2;
    }
    if (sdy == 0.0) {
        return 2;
    }
    *slope = SXY / SXX;
    *intercept = ybar - *slope * xbar;
    return 0;
}

/*
 * assume graph starts off at ymin and rises to ymax
 * Determine time to go from 10% to 90% of rise
 */
int frmFeatureExtract::get_rise_time( int setl, double *xv, double *yv, double min, double max,double *width )
{
    int x10=0, x90;
    double amp10, amp90;

    amp10 = min + (max-min)*0.1;
    amp90 = min + (max-min)*0.9;
    while( x10<setl && yv[x10]<amp10  )
        x10++;

    if( x10==setl || x10==0)
        return 1;

    x90 = x10+1;

    while( x90<setl && yv[x90]<amp90 )
        x90++;

    *width = linear_interp( yv[x90-1], xv[x90-1], yv[x90], xv[x90], amp90 ) -
            linear_interp( yv[x10-1], xv[x10-1], yv[x10], xv[x10], amp10 );
    return 0;
}

/* assume graph starts off at ymax and drops to ymin 
   Determine time to go from 90% to 10% of drop			*/
int frmFeatureExtract::get_fall_time( int setl, double *xv, double *yv, double min, double max,double *width )
{
    int x10, x90=0;
    double amp10, amp90;

    amp10 = min + (max-min)*0.1;
    amp90 = min + (max-min)*0.9;
    while( x90<setl && yv[x90]>amp90 )
        x90++;

    if( x90==setl || x90==0)
        return 1;

    x10= x90+1;

    while( x10<setl && yv[x10]>amp10  )
        x10++;

    if( x10==setl )
        return 1;

    *width = linear_interp( yv[x10-1], xv[x10-1], yv[x10], xv[x10], amp10 )-
            linear_interp( yv[x90-1], xv[x90-1], yv[x90], xv[x90], amp90 );
    return 0;
}

int frmFeatureExtract::get_zero_crossing( int setl, double *xv, double *yv, double *crossing )
{
    int i=0;

    while( i<setl && yv[i] != 0. && yv[i]*yv[i+1]>0. )
        i++;

    if( i==setl )
        return 1;

    if( yv[i] == 0 )
        *crossing = xv[i];
    else
        *crossing = linear_interp( yv[i], xv[i], yv[i+1], xv[i+1], 0 );

    return 0;
}

/* Get FWHM of the highest peak */
int frmFeatureExtract::get_half_max_width(int len, double *x, double *y, double *width)
{
    int i, imin, imax;
    double ymin, ymax, yavg;
    double x_u, x_d;

    minmax(y, len, &ymin, &ymax, &imin, &imax);
    yavg = (ymin + ymax)/2.0;

    i = imax;
    while (i >= 0 && y[i] > yavg) {
        i--;
    }
    if (i < 0) {
        return RETURN_FAILURE;
    }
    x_d = linear_interp(y[i], x[i], y[i + 1], x[i + 1], yavg);

    i = imax;
    while (i < len && y[i] > yavg) {
        i++;
    }
    if (i == len) {
        return RETURN_FAILURE;
    }
    x_u = linear_interp(y[i - 1], x[i - 1], y[i], x[i], yavg);

    *width = fabs(x_u - x_d);
    
    return RETURN_SUCCESS;
}

/* linear interpolate between two points, return a y value given an x */
double frmFeatureExtract::linear_interp( double x1, double y1, double x2, double y2, double x )
{
    return y1 + ( x-x1 )*(y2-y1)/(x2-x1);
}

int dbl_comp( const void *a, const void *b )
{
    if( *(double *)a < *(double *)b )
        return -1;
    else if( *(double *)a > *(double *)b )
        return 1;
    else
        return 0;
}

/* get the median of the X or Y portion of a set */
int frmFeatureExtract::getmedian( int grno, int setno, int sorton, double *median )
{
    int setlen;
    double *setdata;

    setlen = getsetlength( get_cg(), setno );
    setdata = (double *)xmalloc( setlen*sizeof(double) );
    if( sorton == DATA_X )
        memcpy( setdata, getx( grno, setno ), setlen*sizeof(double) );
    else
        memcpy( setdata, gety( grno, setno ), setlen*sizeof(double) );

    qsort(
                setdata,
                setlen,
                sizeof(double),
                dbl_comp );

    if( setlen%2 )		/* odd set */
        *median = setdata[(setlen+1)/2-1];
    else
        *median = ( setdata[setlen/2-1] + setdata[setlen/2] )/2.;

    xfree( setdata );
    return 0;
}

int frmFeatureExtract::get_barycenter( int n, double *x, double *y, double *barycenter )
{
    double wsum=0;

    *barycenter = 0;
    for( n--; n>=0; n-- ) {
        wsum += x[n]*y[n];
        *barycenter += x[n];
    }
    *barycenter = wsum/(*barycenter);
    return 0;
}

/* given maximum of set a, find the corresponding entry in set b */
void frmFeatureExtract::get_max_pos( double *a, double *b, int n, double max, double *d )
{
    int i=-1;

    while( ++i<n && a[i] != max  ){;}

    if( i==n )
        return;
    else
        *d = b[i];
}

frmAbout::frmAbout(QWidget * parent):QDialog(parent)
{
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: About"));
    setWindowIcon(QIcon(*GraceIcon));

    char buf[1024];
    grpGrace=new QGroupBox(QString(""),this);
    layout0=new QVBoxLayout();
    layout0->setMargin(STD_MARGIN);
    lblInfo[0]=new QLabel(QString(bi_version_string())+QString(" / qtGrace v0.2.3"),grpGrace);
    layout0->addWidget(lblInfo[0]);
#ifdef DEBUG
    lblInfo[1]=new QLabel(tr("Debugging is enabled"),grpGrace);
    layout0->addWidget(lblInfo[1]);
#endif
    grpGrace->setLayout(layout0);

    grpLegal=new QGroupBox(tr("Legal stuff"),this);
    layout1=new QVBoxLayout;
    layout1->setMargin(STD_MARGIN);
    lblInfo[2]=new QLabel(tr("Copyright (c) 1991-1995 Paul J Turner"),grpLegal);
    lblInfo[3]=new QLabel(tr("Copyright (c) 1996-2008 Grace Development Team"),grpLegal);
    lblInfo[4]=new QLabel(tr("Maintained by Evgeny Stambulchik"),grpLegal);
    lblInfo[5]=new QLabel(tr("qtGrace by Andreas Winter, 2008-2012"),grpLegal);
    lblInfo[6]=new QLabel(tr("All rights reserved"),grpLegal);
    lblInfo[7]=new QLabel(tr("The program is distributed under the terms of the GNU General Public License"),grpLegal);

    for (int i=2;i<=7;i++)
        layout1->addWidget(lblInfo[i]);

    grpLegal->setLayout(layout1);

    grpThirdParty=new QGroupBox(tr("Third party copyright"),this);
    layout2=new QVBoxLayout;
    layout2->setMargin(STD_MARGIN);
    lblInfo[8]=new QLabel(QString("Tab widget, Copyright (c) 1997 Pralay Dakua"),grpThirdParty);
    lblInfo[9]=new QLabel(QString("Xbae widget,"),grpThirdParty);
    lblInfo[10]=new QLabel(QString("      Copyright (c) 1991, 1992 Bell Communications Research, Inc. (Bellcore)"),grpThirdParty);
    lblInfo[11]=new QLabel(QString("      Copyright (c) 1995-1999 Andrew Lister"),grpThirdParty);
    lblInfo[12]=new QLabel(QString("Raster driver based on the GD-1.3 library,"),grpThirdParty);
    lblInfo[13]=new QLabel(QString("      Portions copyright (c) 1994-1998 Cold Spring Harbor Laboratory"),grpThirdParty);
    lblInfo[14]=new QLabel(QString("      Portions copyright (c) 1996-1998 Boutell.Com, Inc"),grpThirdParty);

    for (int i=8;i<=14;i++)
        layout2->addWidget(lblInfo[i]);

#ifdef HAVE_LIBPDF
    lblInfo[15]=new QLabel(QString("PDFlib library, Copyright (c) 1997-2002 Thomas Merz");
            layout2->addWidget(lblInfo[15]);
#endif
    grpThirdParty->setLayout(layout2);

    grpBuildInfo=new QGroupBox(tr("Build info"),this);
    layout3=new QVBoxLayout;
    layout3->setMargin(STD_MARGIN);

    sprintf(buf, "Host: %s", bi_system());
    lblInfo[16]=new QLabel(QString(buf),grpBuildInfo);
    sprintf(buf, "Time: %s ; \"allWidgets.cpp\" compiled %s %s ", bi_date(),__DATE__,__TIME__);
    lblInfo[17]=new QLabel(QString(buf),grpBuildInfo);
    sprintf(buf, "GUI toolkit: %s ", bi_gui());
    lblInfo[18]=new QLabel(QString(buf),grpBuildInfo);
    /*
    sprintf(buf, "Xbae version: %s ", bi_gui_xbae());
lblInfo[19]=new QLabel(QString(buf),grpBuildInfo);
*/
    sprintf(buf, "Qt version: %s ", QT_VERSION_STR);
    lblInfo[19]=new QLabel(QString(buf),grpBuildInfo);

    sprintf(buf, "T1lib: %s ", T1_GetLibIdent());
    lblInfo[20]=new QLabel(QString(buf),grpBuildInfo);

    for (int i=16;i<=20;i++)
        layout3->addWidget(lblInfo[i]);

#ifdef HAVE_LIBPNG
    sprintf(buf, "libpng: %s ", bi_pnglib());
    lblInfo[21]=new QLabel(QString(buf),grpBuildInfo);
    layout3->addWidget(lblInfo[21]);
#endif
#ifdef HAVE_LIBJPEG
    sprintf(buf, "libjpeg: %s ", bi_libjpeg());
    lblInfo[22]=new QLabel(QString(buf),grpBuildInfo);
    layout3->addWidget(lblInfo[22]);
#endif
#ifdef HAVE_LIBPDF
    sprintf(buf, "PDFlib: %s ", bi_libpdf());
    lblInfo[23]=new QLabel(QString(buf),grpBuildInfo);
    layout3->addWidget(lblInfo[23]);
#endif

    grpBuildInfo->setLayout(layout3);

    grpHomePage=new QGroupBox(tr("Home page"),this);
    layout4=new QVBoxLayout;
    layout4->setMargin(STD_MARGIN);
    cmdIAdr=new QPushButton(QString("http://plasma-gate.weizmann.ac.il/Grace/"),grpHomePage);
    cmdIAdr->setFlat(true);
    connect(cmdIAdr,SIGNAL(clicked()),SLOT(doShowHomepage()));
    cmdIAdr2=new QPushButton(QString("http://sourceforge.net/projects/qtgrace"),grpHomePage);
    cmdIAdr2->setFlat(true);
    connect(cmdIAdr2,SIGNAL(clicked()),SLOT(doShowHomepage2()));
    cmdIAdr3=new QPushButton(QString("http://qt.nokia.com/"),grpHomePage);
    cmdIAdr3->setFlat(true);
    connect(cmdIAdr3,SIGNAL(clicked()),SLOT(doShowHomepage3()));
    layout4->addWidget(cmdIAdr);
    layout4->addWidget(cmdIAdr2);
    layout4->addWidget(cmdIAdr3);
    grpHomePage->setLayout(layout4);

    cmdClose=new QPushButton(tr("Close"),this);
    connect(cmdClose,SIGNAL(clicked()),SLOT(doClose()));

    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(grpGrace);
    layout->addWidget(grpLegal);
    layout->addWidget(grpThirdParty);
    layout->addWidget(grpBuildInfo);
    layout->addWidget(grpHomePage);
    layout->addWidget(cmdClose);
    setLayout(layout);
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
    HelpCB("http://qt.nokia.com/");
}

void frmAbout::doClose(void)
{
    hide();
}

frmIOForm::frmIOForm(int type,QWidget * parent):QDialog(parent)
{
    formType=type;
    frmEdBlock=NULL;
    int number;
    QString entr[32];
    int values[32];
    int number2,offset=0,offset2=0;
    QString entr2[32];
    char dummy[32];
    setFont(stdFont);
    setWindowIcon(QIcon(*GraceIcon));

    grpRead=new QGroupBox(QString(""),this);
    grpWrite=new QGroupBox(QString(""),this);
    grpDescription=new QGroupBox(QString(""),this);
    grpTitle=new QGroupBox(QString(""),this);
    grpParamGraph=new QGroupBox(QString(""),this);

    lblReadGraph=new QLabel(tr("Read to graph:"),grpRead);
    lblWriteSet=new QLabel(tr("Write set(s):"),grpWrite);
    lblSelection=new QLabel(tr("Selection:"),this);
    lblDataSource=new QLabel(tr("Data source:"),grpRead);
    lblDescription=new QLabel(tr("Project description:"),grpDescription);

    ledFormat=new stdLineEdit(grpWrite,tr("Format:"));
    ledFormat->lenText->setText(QString("\%.8g"));
    ledFormat2=new stdLineEdit(grpWrite,tr("Data format:"));
    ledFormat2->lenText->setText(QString("\%.8g"));
    ledTitle=new stdLineEdit(grpTitle,tr("Title:"));
    ledTitle->lenText->setText(QString("A fit"));

    ledSelection=new QLineEdit(tr(""),this);
    txtDescription=new QTextEdit(this);

    number=2;
    entr[0]=tr("Current");
    entr[1]=tr("All");
    selParamGraph=new StdSelector(grpRead,tr("Write parameters from graph:"),number,entr);

    number=3;
    entr[0]=tr("Single set");
    entr[1]=tr("NXY");
    entr[2]=tr("Block data");
    selLoadAs=new StdSelector(grpRead,tr("Load as:"),number,entr);
    number=4;
    entr[0]=tr("None");
    entr[1]=tr("X");
    entr[2]=tr("Y");
    entr[3]=tr("XY");
    selAutoscale=new StdSelector(grpRead,tr("Autoscale on read:"),number,entr);
    selAutoscale->setCurrentIndex(3);
    number=NUMBER_OF_SETTYPES;
    for (int i=0;i<number;i++)
    {
        strToUpper(dummy,set_types(i));
        entr[i]=QString(dummy);
    }
    selType=new StdSelector(grpRead,tr("Type:"),number,entr);

    radPipe=new QRadioButton(tr("Pipe"),grpRead);
    radDisk=new QRadioButton(tr("Disk"),grpRead);
    radDisk->setChecked(TRUE);

    graphList=new uniList(GRAPHLIST,grpRead);
    setList=new uniList(SETLIST,grpWrite);

    //Specials for Binary-File-Interaction
    if (type==READ_BINARY_FILE || type==WRITE_BINARY_FILE)
    {
        grpBinary=new QGroupBox(tr("Binary conversion"),this);
        layout6=new QGridLayout;
        layout6->setMargin(STD_MARGIN);
        chkHeader=new QCheckBox(tr("Header"),this);
        connect(chkHeader,SIGNAL(stateChanged(int)),SLOT(headerChecked(int)));
        layout6->addWidget(chkHeader,0,0,1,2);
        selHeaderLength=new stdIntSelector(this,tr("Length of header in bytes:"),0,100000);
        layout6->addWidget(selHeaderLength,1,0,1,2);
        selHeaderLength->setEnabled(false);
        selTotalColumnNumber=new stdIntSelector(this,tr("Number of data Columns:"),1,200);
        layout6->addWidget(selTotalColumnNumber,2,0,1,2);
        selector_count=5;
        selTotalColumnNumber->setValue(selector_count);
        connect(selTotalColumnNumber->spnInt,SIGNAL(valueChanged(int)),this,SLOT(columnCountChanged(int)));
        selColFormats=new StdSelector*[selector_count];
        selImportTo=new StdSelector*[selector_count];
        selColSize=new QSpinBox*[selector_count];
        number=NUMBER_OF_COLUMN_FORMATS;
        for (int i=0;i<number;i++)
        {
            entr[i]=QString(binaryImportFormatName[i]);
            values[i]=binaryImportFormat[i].format;
        }
        layout7=new QGridLayout;
        layout7->setMargin(STD_MARGIN);
        layout7->setSpacing(STD_SPACING);
        lblFormat=new QLabel(tr("Column format:"),this);
        lblBytes=new QLabel(tr("Bytes:"),this);
        lblImport=new QLabel(tr("Import to:"),this);
        layout7->addWidget(lblFormat,0,0,1,1);
        layout7->addWidget(lblBytes,0,1,1,1);
        layout7->addWidget(lblImport,0,2,1,1);
        empty=new QWidget;
        number2=3;
        entr2[0]=QString("None");
        entr2[1]=QString("X");
        entr2[2]=QString("Y");
        offset2=1;
        for (int i=0;i<selector_count;i++)
        {
            sprintf(dummy,"%d:",i);
            selColFormats[i]=new StdSelector(this,QString(dummy),number,entr);
            selColFormats[i]->setValues(values);
            selImportTo[i]=new StdSelector(this,QString(""),number2,entr2);
            selImportTo[i]->lblText->setVisible(false);
            selColSize[i]=new QSpinBox(this);
            selColSize[i]->setMinimum(1);
            selColSize[i]->setMaximum(5000);
            layout7->addWidget(selColFormats[i],offset2,0,1,1);
            layout7->addWidget(selColSize[i],offset2,1,1,1);
            layout7->addWidget(selImportTo[i],(offset2++),2,1,1);
        }
        area=new QScrollArea;
        area->setWidget(empty);
        empty->setGeometry(0,0,320,180);
        empty->setLayout(layout7);
        layout6->addWidget(area,3,0,1,2);
        chkImportToEOF=new QCheckBox(tr("Import until EOF"),this);
        connect(chkImportToEOF,SIGNAL(stateChanged(int)),SLOT(readUntilEOFChecked(int)));
        layout6->addWidget(chkImportToEOF,4,0,1,2);
        selMaxImportData=new stdIntSelector(this,tr("Max number of data sets to import:"),1,500000);
        selMaxImportData->setValue(1000);
        layout6->addWidget(selMaxImportData,5,0,1,2);
        grpBinary->setLayout(layout6);
        grpBinary->setVisible(false);
    }

    cmdFilter=new QPushButton(tr("Filter"),this);
    cmdFilter->setDefault(true);
    connect(cmdFilter,SIGNAL(clicked()),SLOT(doFilter()));
    cmdOK=new QPushButton(tr("OK"),this);
    connect(cmdOK,SIGNAL(clicked()),SLOT(doOK()));
    cmdCancel=new QPushButton(tr("Cancel"),this);
    connect(cmdCancel,SIGNAL(clicked()),SLOT(doCancel()));
    cmdHelp=new QPushButton(tr("Help"),this);
    connect(cmdHelp,SIGNAL(clicked()),SLOT(doHelp()));

    chkExchangeCommaPoint=new QCheckBox(tr("Use ',' instead of '.' as decimal seperator"),this);

    layout1=new QGridLayout;
    layout1->setMargin(STD_MARGIN);
    layout1->addWidget(lblReadGraph,0,0,1,4);
    layout1->addWidget(graphList,1,0,1,4);
    layout1->addWidget(selLoadAs,2,0,1,2);
    layout1->addWidget(selType,2,2,1,2);
    layout1->addWidget(lblDataSource,3,0,1,2);
    layout1->addWidget(radDisk,3,2);
    layout1->addWidget(radPipe,3,3);
    layout1->addWidget(selAutoscale,4,0,1,4);
    layout1->addWidget(chkExchangeCommaPoint,5,0,1,4);
    grpRead->setLayout(layout1);

    layout2=new QVBoxLayout;
    layout2->setMargin(STD_MARGIN);
    layout2->addWidget(lblWriteSet);
    layout2->addWidget(setList);
    layout2->addWidget(ledFormat);
    grpWrite->setLayout(layout2);

    layout3=new QVBoxLayout;
    layout3->setMargin(STD_MARGIN);
    layout3->addWidget(lblDescription);
    layout3->setAlignment(lblDescription,Qt::AlignHCenter);
    layout3->addWidget(txtDescription);
    layout3->addWidget(ledFormat2);
    grpDescription->setLayout(layout3);

    layout4=new QVBoxLayout;
    layout4->setMargin(STD_MARGIN);
    layout4->addWidget(ledTitle);
    grpTitle->setLayout(layout4);

    layout5=new QVBoxLayout;
    layout5->setMargin(STD_MARGIN);
    layout5->addWidget(selParamGraph);
    grpParamGraph->setLayout(layout5);

    layout=new QGridLayout;
    layout->setMargin(STD_MARGIN);

    selector=new FileSelector(this);
    connect(selector,SIGNAL(newSelection(QString)),SLOT(gotNewSelection(QString)));
    connect(selector,SIGNAL(newSelectionDoubleClick(QString)),SLOT(gotNewSelectionDoubleClick(QString)));

    layout->addWidget(selector,0,0,1,4);

    switch (type)
    {
    case READ_SET_FORM:
        setWindowTitle(tr("qtGrace: Read sets"));
        stdExtension=QString("*.dat");
        layout->addWidget(grpRead,6,0,1,4);
        offset=1;
        grpWrite->setVisible(FALSE);
        grpDescription->setVisible(FALSE);
        grpTitle->setVisible(FALSE);
        grpParamGraph->setVisible(FALSE);
        selector->forRead=true;
        break;
    case READ_NETCDF_FORM:
        setWindowTitle(tr("qtGrace: Select netCDF file"));
        stdExtension=QString("*.nc");
        grpRead->setVisible(FALSE);
        grpWrite->setVisible(FALSE);
        grpDescription->setVisible(FALSE);
        grpTitle->setVisible(FALSE);
        grpParamGraph->setVisible(FALSE);
        selector->forRead=true;
        break;
    case READ_PROJECT_FORM:
        setWindowTitle(tr("qtGrace: Open project"));
        stdExtension=QString("*.agr");
        grpRead->setVisible(FALSE);
        grpWrite->setVisible(FALSE);
        grpDescription->setVisible(FALSE);
        /*
grpDescription->setVisible(TRUE);//I changed this!!
grpDescription->setEnabled(FALSE);//I changed this!!
layout->addWidget(grpDescription,6,0,1,4);//I added this!!
*/
        grpTitle->setVisible(FALSE);
        grpParamGraph->setVisible(FALSE);
        selector->forRead=true;
        break;
    case WRITE_SET_FORM:
        setWindowTitle(tr("qtGrace: Write sets"));
        stdExtension=QString("*.dat");
        layout->addWidget(grpWrite,6,0,1,4);
        offset=1;
        grpRead->setVisible(FALSE);
        grpDescription->setVisible(FALSE);
        grpTitle->setVisible(FALSE);
        grpParamGraph->setVisible(FALSE);
        selector->forRead=false;
        break;
    case WRITE_PROJECT_FORM:
        setWindowTitle(tr("qtGrace: Save project"));
        stdExtension=QString("*.agr");
        layout->addWidget(grpDescription,6,0,1,4);
        offset=1;
        grpRead->setVisible(FALSE);
        grpWrite->setVisible(FALSE);
        grpTitle->setVisible(FALSE);
        grpParamGraph->setVisible(FALSE);
        selector->forRead=false;
        break;
    case SELECT_HOT_LINK_FILE:
        setWindowTitle(tr("qtGrace: Select hot link file"));
        stdExtension=QString("*.dat");
        grpRead->setVisible(FALSE);
        grpWrite->setVisible(FALSE);
        grpDescription->setVisible(FALSE);
        grpTitle->setVisible(FALSE);
        grpParamGraph->setVisible(FALSE);
        selector->forRead=true;
        break;
    case READ_PARAMETERS:
        setWindowTitle(tr("qtGrace: Read parameters"));
        stdExtension=QString("*.par");
        grpRead->setVisible(FALSE);
        grpWrite->setVisible(FALSE);
        grpDescription->setVisible(FALSE);
        grpTitle->setVisible(FALSE);
        grpParamGraph->setVisible(FALSE);
        selector->forRead=true;
        break;
    case WRITE_PARAMETERS:
        setWindowTitle(tr("qtGrace: Write parameters"));
        stdExtension=QString("*.par");
        layout->addWidget(grpParamGraph,6+offset,0,1,4);
        offset=1;
        grpRead->setVisible(FALSE);
        grpWrite->setVisible(FALSE);
        grpDescription->setVisible(FALSE);
        grpTitle->setVisible(FALSE);
        grpParamGraph->setVisible(TRUE);
        selector->forRead=false;
        break;
    case SELECT_PRINT_FILE:
        setWindowTitle(tr("qtGrace: Select print file"));
        stdExtension=QString("*.dat");
        grpRead->setVisible(FALSE);
        grpWrite->setVisible(FALSE);
        grpDescription->setVisible(FALSE);
        grpTitle->setVisible(FALSE);
        grpParamGraph->setVisible(FALSE);
        selector->forRead=false;
        break;
    case READ_FIT_PARAM:
        setWindowTitle(tr("qtGrace: Open fit parameter file"));
        stdExtension=QString("*.fit");
        grpRead->setVisible(FALSE);
        grpWrite->setVisible(FALSE);
        grpDescription->setVisible(FALSE);
        grpTitle->setVisible(FALSE);
        grpParamGraph->setVisible(FALSE);
        selector->forRead=true;
        break;
    case WRITE_FIT_PARAM:
        setWindowTitle(tr("qtGrace: Save fit parameter file"));
        stdExtension=QString("*.fit");
        grpRead->setVisible(FALSE);
        grpWrite->setVisible(FALSE);
        grpDescription->setVisible(FALSE);
        layout->addWidget(grpTitle,6+offset,0,1,4);
        grpTitle->setVisible(TRUE);
        grpParamGraph->setVisible(FALSE);
        offset=1;
        selector->forRead=false;
        break;
    case READ_BINARY_FILE:
        setWindowTitle(tr("qtGrace: Read binary file"));
        stdExtension=QString("*.dat");
        layout->addWidget(grpRead,6,0,1,2);
        layout->addWidget(grpBinary,6,2,1,2);
        grpBinary->setVisible(true);
        offset=1;
        grpWrite->setVisible(FALSE);
        grpDescription->setVisible(FALSE);
        grpTitle->setVisible(FALSE);
        grpParamGraph->setVisible(FALSE);
        selector->forRead=true;
        connect(selType->cmbSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(setTypeChanged(int)));
        setTypeChanged(0);
        selTotalColumnNumber->setValue(2);
        selColFormats[0]->setCurrentIndex(10);
        selColFormats[1]->setCurrentIndex(10);
        selImportTo[0]->setCurrentIndex(1);
        selImportTo[1]->setCurrentIndex(2);
        chkImportToEOF->setChecked(true);
        break;
    case WRITE_BINARY_FILE:
        setWindowTitle(tr("qtGrace: Write binary file"));
        stdExtension=QString("*.dat");
        layout->addWidget(grpWrite,6,0,1,4);
        offset=1;
        grpRead->setVisible(FALSE);
        grpBinary->setVisible(FALSE);
        ledFormat->setVisible(false);
        setList->setBehavior(true,false,false);
        grpWrite->setTitle(tr("Write set:"));
        grpWrite->setVisible(TRUE);
        grpDescription->setVisible(FALSE);
        grpTitle->setVisible(FALSE);
        grpParamGraph->setVisible(FALSE);
        selector->forRead=false;
        break;
    }
    layout->addWidget(lblSelection,6+offset,0,1,4);
    layout->addWidget(ledSelection,7+offset,0,1,4);
    layout->addWidget(cmdOK,8+offset,0);
    layout->addWidget(cmdFilter,8+offset,1);
    layout->addWidget(cmdCancel,8+offset,2);
    layout->addWidget(cmdHelp,8+offset,3);
    setLayout(layout);
    QString cdir=QString(get_workingdir());
    cdir.chop(1);
    selector->setFilterFromExtern(cdir,stdExtension);
}

void frmIOForm::gotNewSelection(QString selection)
{
    ledSelection->setText(selection);
}

void frmIOForm::gotNewSelectionDoubleClick(QString selection)
{
    gotNewSelection(selection);
    doOK();
}

void frmIOForm::init(void)
{
    selector->ledFilter->setText(selector->currentDir+selector->separator+selector->filterExtension);
    ledSelection->setText(selector->currentDir+selector->separator);
    graphList->update_number_of_entries();
    setList->update_number_of_entries();
    int gno=get_cg();
    graphList->set_new_selection(1,&gno);
}

void frmIOForm::doOK(void)
{
    static char dummy[512];
    int gno, setno;
    int *selset=new int[5], cd, i;
    int graphno,cursource;
    char filename[2048];
    selectedFile=ledSelection->text();
    if (radPipe->isChecked()==true)
        cursource=1;
    else
        cursource=0;
    autoscale_onread = GetOptionChoice(selAutoscale);
    QFile fileTest(selectedFile);
    QFileInfo fileInfo(fileTest);
    if (fileInfo.isFile()==false)
    {
        FileExists=false;
        isWriteable=false;
        isReadable=false;
    }
    else
    {
        FileExists=fileTest.exists();
        isWriteable=fileInfo.isWritable();
        isReadable=fileInfo.isReadable();
    }

    //strcpy(filename,selectedFile.toAscii());//ascii-only
    QString encodedString(QFile::encodeName(selectedFile));//should work with unicode-characters as well
    strcpy(filename,encodedString.toAscii().constData());

    /*
QMessageBox::information(this,"selected File",selectedFile);
QString nonUniCodeFile=QString(FileCodec->fromUnicode(selectedFile));
strcpy(filename,nonUniCodeFile.toAscii().constData());
QMessageBox::information(this,"nonUniCodeFile",nonUniCodeFile);
QString recon=FileCodec->toUnicode(filename);//so geht's, wenn die richtige Kodierung eingestellt wurde!
QMessageBox::information(this,"recon",recon);
QMessageBox::information(this,"encoded",encodedString);
*/

    exchange_point_comma=chkExchangeCommaPoint->isChecked();

    if (FileExists && (formType==WRITE_SET_FORM || formType==WRITE_BINARY_FILE))
    {
        int ret=QMessageBox::question(this,tr("Warning!"),tr("File exists!\nOverwrite?"),QMessageBox::Ok,QMessageBox::Cancel);
        if (ret==QMessageBox::Cancel)
        {
            delete[] selset;
            exchange_point_comma=false;
            return;
        }
    }

    if (formType==READ_SET_FORM)
    {
        set_wait_cursor();
        int load = GetOptionChoice(selLoadAs);
        if (cursource==1)//source=pipe
        {//we load from a pipe here
            int fd;
            if (openPipe(filename,&fd)!=RETURN_SUCCESS)
                errmsg(tr("Unable to open pipe!").toAscii().constData());
            mainWin->checkForRealTimeIO();
            goto end_fromIOFormOK;
        }

        if (GetSingleListChoice(graphList, &graphno) != RETURN_SUCCESS) {
            errmsg(tr("Please select a single graph!").toAscii().constData());
        } else {

            if (load == LOAD_SINGLE)
            {
                curtype = GetOptionChoice(selType);
            }

            if (new_set_nos!=NULL)
            {
                delete[] new_set_nos;
                new_set_nos=NULL;
            }
            new_set_no=0;
            //cout << "A3 " << graphno << " " << filename << " " << cursource << " " << load << endl;
            getdata(graphno, filename, cursource, load);
            if (selLoadAs->currentIndex()==2)//Block data
            {
                if (frmEdBlock==NULL)
                {
                    frmEdBlock=new frmEditBlockData(mainWin);
                }
                frmEdBlock->filename=QString(filename);
                frmEdBlock->source=cursource;
                frmEdBlock->init();
                frmEdBlock->show();
                frmEdBlock->raise();
            }
            else if (cursource==0)//source=file
            {
                ///Undo-Stuff
                if (new_set_no==1)//only one new set
                {
                    SetImported(graphno,new_set_nos[0],filename,cursource,load,autoscale_onread);
                }
                else if (new_set_no>1)//more than one set imported
                {
                    int * gnos=new int[new_set_no+2];
                    char ** filenames=new char *[2];
                    for (int i=0;i<new_set_no;i++)
                    {
                        gnos[i]=graphno;
                    }
                    filenames[0]=new char[2+strlen(filename)];
                    strcpy(filenames[0],filename);
                    SetsImported(new_set_no,gnos,new_set_nos,1,filenames,cursource,load,autoscale_onread);
                    delete[] filenames[0];
                    delete[] filenames;
                    delete[] gnos;
                }
                mainWin->mainArea->completeRedraw();
            }
            /*else
        {
        mainWin->checkForRealTimeIO();
        }*/
            copy_Grace_to_LaTeX();
            update_default_props();
        }
    }
    else if (formType==WRITE_SET_FORM)
    {
        set_wait_cursor();
        char format[32];
        FILE *cp;

        cp = grace_openw(filename);
        if (cp == NULL) {
            return;
        }
        cd = GetListChoices(setList, &selset);
        if (cd < 1) {
            errmsg(tr("No set selected!").toAscii().constData());
        } else {
            gno = get_cg();
            strncpy(format, xv_getstr(ledFormat), 31);
            for(i = 0; i < cd; i++) {
                setno = selset[i];
                write_set(gno, setno, cp, format, TRUE);
            }
        }
        grace_close(cp);
    }
    else if (formType==READ_BINARY_FILE)
    {
        set_wait_cursor();
        if (!FileExists || !isReadable)
        {
            errmsg((tr("File ")+QString(filename)+tr(" does not exist or is unreadable!")).toAscii().constData());
            unset_wait_cursor();
            return;
        }
        struct BinReadInstructions inst;

        autoscale_onread = GetOptionChoice(selAutoscale);

        if (GetSingleListChoice(graphList, &graphno) != RETURN_SUCCESS) {
            errmsg(tr("Please select a single graph!").toAscii().constData());
        }else{
            setno=nextset(graphno);
            int curtype = GetOptionChoice(selType);
            int number_of_cols = settype_cols(curtype);

            if (chkHeader->isChecked())
                inst.length_of_header=selHeaderLength->value();
            else
                inst.length_of_header=0;

            inst.column_count=selTotalColumnNumber->value();
            inst.read_until_end_is_reached=chkImportToEOF->isChecked();
            inst.datasets_to_read=selMaxImportData->value();

            inst.col_numbers_to_read=new int[number_of_cols];
            for (int i=0;i<number_of_cols;i++)
                inst.col_numbers_to_read[i]=-1;//invalid number
            inst.column_type=new struct ColumnFormat[inst.column_count];//format of all columns
            inst.n_columns_to_read=number_of_cols;

            for (unsigned int i=0;i<inst.column_count;i++)
            {
                inst.column_type[i].format=selColFormats[i]->currentValue();
                inst.column_type[i].size=selColSize[i]->value();
                if (selImportTo[i]->currentValue()>=0 && selImportTo[i]->currentValue()<number_of_cols)
                    inst.col_numbers_to_read[selImportTo[i]->currentValue()]=i;
            }
            //checking entries
            int ok=-1;
            for (int i=0;i<number_of_cols;i++)
            {
                //cout << dataset_colname(i) << "=" << inst.col_numbers_to_read[i] << endl;
                if (inst.col_numbers_to_read[i]==-1){ok=i;break;}
            }
            if (ok>-1)
            {
                sprintf(dummy,"%s%s%s",tr("Please select an import-column for every set-column! Column ").toAscii().constData(),dataset_colname(ok),tr(" is missing!").toAscii().constData());
                errmsg(dummy);
            }
            else
            {
                ok=-1;
                for (unsigned int i=0;i<inst.column_count;i++)
                {
                    if (selImportTo[i]->currentValue()==-1) continue;
                    for (unsigned int j=i+1;j<inst.column_count;j++)
                    {
                        if (selImportTo[i]->currentValue()==selImportTo[j]->currentValue())
                        {
                            ok=selImportTo[i]->currentValue();
                            i=inst.column_count;
                            break;
                        }
                    }
                }
                if (ok>-1)
                {
                    sprintf(dummy,"%s%s%s",tr("Import-column for ").toAscii().constData(),dataset_colname(ok),tr(" is ambiguous! Binary import aborted!").toAscii().constData());
                    errmsg(dummy);
                }
                else
                {
                    LoadBinaryData(graphno,setno,selType->currentValue(),filename,inst);
                    autoscale_graph(graphno, autoscale_onread);
                    mainWin->mainArea->completeRedraw();
                }
            }
            delete[] inst.column_type;
            delete[] inst.col_numbers_to_read;
        }
    }
    else if (formType==WRITE_BINARY_FILE)
    {
        set_wait_cursor();
        cd = GetListChoices(setList, &selset);
        if (cd < 1) {
            errmsg(tr("No set selected!").toAscii().constData());
        } else {
            gno = get_cg();
            plotarr pa;
            get_graph_plotarr(gno, selset[0], &pa);
            ofstream ofi;
            ofi.open(filename,ios::binary);
            unsigned int channels=dataset_cols(gno,selset[0]);
            unsigned int len=pa.data.len;
            ofi.write((char*)((void*)&channels),sizeof(unsigned int));
            ofi.write((char*)((void*)&len),sizeof(unsigned int));
            for (unsigned int i=0;i<channels;i++)
            {
                for (unsigned int j=0;j<len;j++)
                {
                    ofi.write((char*)((void*)(pa.data.ex[i]+j)),sizeof(double));
                }
            }
            ofi.close();
        }
    }
    else
    {
        if (formType==WRITE_PROJECT_FORM)
        {
            QString nDescr=txtDescription->toPlainText();
            char * neDescr=new char[2+nDescr.length()];
            strcpy(neDescr,nDescr.toAscii().constData());
            set_project_description(neDescr);
            delete[] neDescr;
        }
        emit(newFileSelectedForIO(formType,selectedFile,FileExists,isWriteable,isReadable));
    }
end_fromIOFormOK:
    exchange_point_comma=false;
    unset_wait_cursor();
    doFilter();
    delete[] selset;
}

void frmIOForm::doFilter(void)
{
    selector->newFilterEntered();
}

void frmIOForm::doCancel(void)
{
    hide();
}

void frmIOForm::doHelp(void)
{
    HelpCB("doc/UsersGuide.html#FS-dialog");
}

void frmIOForm::headerChecked(int c)
{
    selHeaderLength->setEnabled((bool)c);
}

void frmIOForm::readUntilEOFChecked(int c)
{
    selMaxImportData->setEnabled(!((bool)c));
}

void frmIOForm::setTypeChanged(int c)
{
    int curtype = GetOptionChoice(selType);
    int number_of_cols = settype_cols(curtype),index;
    int * values=new int[number_of_cols+2];
    QString * entries=new QString[number_of_cols+2];
    values[0]=-1;
    entries[0]=tr("None");
    for (int i=0;i<number_of_cols;i++)
    {
        entries[i+1]=QString(dataset_colname(i));
        values[i+1]=i;
    }
    for (int i=0;i<selector_count;i++)
    {
        index=selImportTo[i]->currentIndex();
        selImportTo[i]->setNewEntries(number_of_cols+1,entries,values);
        selImportTo[i]->setCurrentIndex(index);
    }
    delete[] values;
    delete[] entries;
}

void frmIOForm::columnCountChanged(int c)
{
    int * sel_ind=new int[selector_count];
    int * sel_ind2=new int[selector_count];
    int * sel_ind3=new int[selector_count];
    for (int i=0;i<selector_count;i++)
    {
        sel_ind[i]=selColFormats[i]->currentIndex();
        sel_ind2[i]=selImportTo[i]->currentIndex();
        sel_ind3[i]=selColSize[i]->value();
        delete selColFormats[i];
        delete selImportTo[i];
        delete selColSize[i];
    }
    delete[] selColFormats;
    delete[] selImportTo;
    delete[] selColSize;
    selColFormats=new StdSelector*[c];
    selImportTo=new StdSelector*[c];
    selColSize=new QSpinBox*[c];

    empty->setGeometry(0,0,empty->width(),(int)(empty->height()*c*1.0/selector_count));

    int number=NUMBER_OF_COLUMN_FORMATS;
    char dummy[128];
    QString entr[NUMBER_OF_COLUMN_FORMATS];
    int vals[NUMBER_OF_COLUMN_FORMATS];
    for (int i=0;i<number;i++)
    {
        entr[i]=QString(binaryImportFormatName[i]);
        vals[i]=binaryImportFormat[i].format;
    }

    int offset=1;
    for (int i=0;i<c;i++)
    {
        sprintf(dummy,"%d:",i);
        selColFormats[i]=new StdSelector(this,QString(dummy),number,entr);
        selColFormats[i]->setValues(vals);
        selImportTo[i]=new StdSelector(this,QString(""),number,entr);
        selImportTo[i]->lblText->setVisible(false);
        selColSize[i]=new QSpinBox(this);
        selColSize[i]->setMinimum(1);
        selColSize[i]->setMaximum(5000);
        layout7->addWidget(selColFormats[i],offset,0,1,1);
        layout7->addWidget(selColSize[i],offset,1,1,1);
        layout7->addWidget(selImportTo[i],offset++,2,1,1);
    }

    int curtype = GetOptionChoice(selType);
    int number_of_cols = settype_cols(curtype);
    int * values=new int[number_of_cols+2];
    QString * entries=new QString[number_of_cols+2];
    values[0]=-1;
    entries[0]=tr("None");
    for (int i=0;i<number_of_cols;i++)
    {
        entries[i+1]=QString(dataset_colname(i));
        values[i+1]=i;
    }
    for (int i=0;i<c;i++)
    {
        selImportTo[i]->setNewEntries(number_of_cols+1,entries,values);
        connect(selColFormats[i]->cmbSelect,SIGNAL(currentIndexChanged(int)),SLOT(columnSizeChanged(int)));
        if (i<selector_count)
        {
            selColFormats[i]->setCurrentIndex(sel_ind[i]);
            selImportTo[i]->setCurrentIndex(sel_ind2[i]);
            selColSize[i]->setValue(sel_ind3[i]);
            if (selColFormats[i]->currentValue())
                selColSize[i]->setEnabled(false);
            else
                selColSize[i]->setEnabled(true);
        }
        else
        {
            selColSize[i]->setValue(1);
            selColSize[i]->setEnabled(true);
        }
    }

    selector_count=c;
    delete[] sel_ind;
    delete[] sel_ind2;
}

void frmIOForm::columnSizeChanged(int c)
{
    for (int i=0;i<selector_count;i++)
    {
        if (selColFormats[i]->currentValue())
        {
            selColSize[i]->setEnabled(false);
            selColSize[i]->setValue(binaryImportFormat[selColFormats[i]->currentValue()].size);
            selImportTo[i]->setEnabled(true);
        }
        else
        {
            selColSize[i]->setEnabled(true);
            selImportTo[i]->setEnabled(false);
            selImportTo[i]->setCurrentIndex(0);
        }
    }
}

GlyphPanel::GlyphPanel(QWidget * parent):QLabel(parent)
{
    number=-1;
    marked=false;
    pix=new QPixmap(25,25);
    pix->fill(QApplication::palette().window().color());
    setPixmap(*pix);
    setFrameStyle(QFrame::Box | QFrame::Raised);
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
        pix=new QPixmap(25,25);
        pix->fill(Qt::white);
        QChar car(char_nr);
        QPainter paint1;
        QFont dfont=getFontFromDatabase(font_nr);
        dfont.setPixelSize(15);
        paint1.begin(pix);
        paint1.setFont(dfont);
        paint1.drawText(5,20,QString(car));
        paint1.end();
        valid_char=true;
    }

    setPixmap(*pix);
}

void GlyphPanel::mousePressEvent(QMouseEvent *event)
{
    emit(panelClicked(number));
}

QPixmap GlyphPanel::DrawCB(unsigned char c,int FontID,bool & valid_char)
{
    float Size = 16.8;//who knows where this value comes from?
    BBox bbox;
    GLYPH *glyph;
    int height, width, hshift, vshift;
    QPixmap tmpBitmap;
    QColor bg;
    QPainter paint1;
    QPainter paint2;

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
        hshift = MAX2(glyph->metrics.leftSideBearing - bbox.llx, 0);
        vshift = MAX2(bbox.ury - glyph->metrics.ascent, 0);

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

    QPixmap pixmap(25,25);
    pixmap.fill(bg);

    if (glyph != NULL && glyph->bits != NULL)
    {
        paint1.begin(&pixmap);
        paint1.drawPixmap((25-width)/2+glyph->metrics.leftSideBearing,(25-height)/2-glyph->metrics.descent,QPixmap(tmpBitmap));
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
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Font tool"));
    setWindowIcon(QIcon(*GraceIcon));

    selFont=new FontSelector(this);
    scroll=new QScrollArea(this);
    background=new QWidget(scroll);
    layout0=new QGridLayout;
    layout0->setMargin(0);
    layout0->setSpacing(0);
    int r=0,c=0;
    tileCount=16*16;
    if (useQtFonts==true)
        tileCount=16*338;
    for (int i=0;i<tileCount;i++)
    {
        panel[i]=new GlyphPanel(background);
        layout0->addWidget(panel[i],r,c);
        c++;
        if (c>=16)
        {
            r++;
            c=0;
        }
        connect(panel[i],SIGNAL(panelClicked(int)),SLOT(newClick(int)));
    }
    background->setLayout(layout0);
    scroll->setWidget(background);

    ledString=new stdLineEdit(this,tr("CString:"));
    connect(ledString->lenText,SIGNAL(textEdited(QString)),SLOT(characterInserted(QString)));
    buttonGroup=new stdButtonGroup(this,true,true,false);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));

    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(selFont);
    layout->addWidget(scroll);
    layout->addWidget(ledString);
    layout->addWidget(buttonGroup);
    setLayout(layout);
    connect(selFont->cmbFontSelect,SIGNAL(currentIndexChanged(int)),SLOT(FontChanged(int)));
    FontChanged(0);
    marked=-1;
    ledString->setText(QString(""));
    textString=QString("");
}

void frmFontTool::FontChanged(int i)
{
    if (i<0) return;
    if (useQtFonts==false)
    {
        for (int j=0;j<tileCount;j++)
            panel[j]->setCurrentPixmap(i,j);
    }
    else
    {
        QPainter paint1;
        QFont dfont=getFontFromDatabase(i);
        QChar car;
        dfont.setPixelSize(15);
        car=QChar(0);
        panel[0]->valid_char=car.isPrint();
        paint1.begin(panel[0]->pix);
        paint1.setFont(dfont);
        paint1.setPen(Qt::black);
        paint1.setBrush(Qt::white);
        paint1.drawRect(0,0,25,25);
        paint1.drawText(5,20,QString(car));
        paint1.end();
        panel[0]->number=0;
        panel[0]->setPixmap(*(panel[0]->pix));
        panel[0]->valid_char=false;
        for (int j=1;j<tileCount;j++)
        {
            car=QChar(j);
            paint1.begin(panel[j]->pix);
            paint1.setFont(dfont);
            paint1.setPen(Qt::black);
            paint1.setBrush(Qt::white);
            paint1.drawRect(0,0,25,25);
            paint1.drawText(5,20,QString(car));
            paint1.end();
            panel[j]->setPixmap(*(panel[j]->pix));
            panel[j]->valid_char=car.isPrint();
            panel[j]->number=j;
        }
    }
    char *buf;
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
    if (marked!=-1)
    {
        panel[marked]->setMarked(false);
    }
    marked=i;
    if (marked>=0 && marked<16*16)
    {
        panel[marked]->setMarked(true);
    }
    /// cout << "char-number=" << i << endl;
    if (panel[i]->valid_char==false)
    {
        QApplication::beep();
    }
    else if (useQtFonts==true)
    {
        charText=QString(QChar(i));
        ledString->lenText->insert(charText);
        return;
        char t2(i);
        //charText=FileCodec->toUnicode(&t2);
        cout << i << " #" << charText.toAscii().constData() << "# " << sizeof(char) << endl;
        insertAtCursor(charText);
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

void frmFontTool::insertAtCursor(QString c)
{
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
    for (i=0;i<minlength;i++)
    {
        if (text.at(i)!=textString.at(i)) break;
    }
    if (i==minlength)
    {//the new character is the last character
        if (len1>len2)
        {
            c=textString.at(i).toAscii();
        }
        else
        {
            c=text.at(i).toAscii();
        }
    }
    else
    {
        c=text.at(i).toAscii();
    }
    textString=text;
    /*if (c>=0 && c<256)//Ascii-Character
        {*/
    if (marked!=-1)
    {
        panel[marked]->setMarked(false);
    }
    marked=(int) c;
    if (marked>=0 && marked<16*16)
    {
        panel[marked]->setMarked(true);
    }
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

frmFourier::frmFourier(QWidget * parent):QDialog(parent)
{
    int number;
    QString entr[7];
    setFont(stdFont);
    setWindowIcon(QIcon(*GraceIcon));
    setWindowTitle(tr("qtGrace: Fourier transforms"));

    lblApplyTo=new QLabel(tr("Apply to set:"),this);
    sel=new uniList(SETCHOICE,this);

    buttonGroup=new stdButtonGroup(this,true,true,true);
    buttonGroup->cmdAccept->setText(tr("FFT"));
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doFFT()));
    buttonGroup->cmdApply->setText(tr("DFT"));
    buttonGroup->cmdApply->setDefault(TRUE);
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doDFT()));
    buttonGroup->cmdClose->setText(tr("Window only"));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doWindowOnly()));
    buttonGroup->cmdHelp->setText(tr("Close"));
    connect(buttonGroup->cmdHelp,SIGNAL(clicked()),this,SLOT(doClose()));

    number=7;
    entr[0]=tr("None (Rectangular)");
    entr[1]=tr("Triangular");
    entr[2]=tr("Hanning");
    entr[3]=tr("Welch");
    entr[4]=tr("Hamming");
    entr[5]=tr("Blackman");
    entr[6]=tr("Parzen");
    selectors[0]=new StdSelector(this,tr("Data window:"),number,entr);
    number=3;
    entr[0]=tr("Magnitude");
    entr[1]=tr("Phase");
    entr[2]=tr("Coefficients");
    selectors[1]=new StdSelector(this,tr("Load result as:"),number,entr);
    number=3;
    entr[0]=tr("Index");
    entr[1]=tr("Frequency");
    entr[2]=tr("Period");
    selectors[2]=new StdSelector(this,tr("Let result X="),number,entr);
    number=2;
    entr[0]=tr("Transform");
    entr[1]=tr("Inverse transform");
    selectors[3]=new StdSelector(this,tr("Perform:"),number,entr);
    number=2;
    entr[0]=tr("Real");
    entr[1]=tr("Complex");
    selectors[4]=new StdSelector(this,tr("Data is:"),number,entr);
    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(lblApplyTo);
    layout->addWidget(sel);
    layout->addWidget(selectors[0]);
    layout->addWidget(selectors[1]);
    layout->addWidget(selectors[2]);
    layout->addWidget(selectors[3]);
    layout->addWidget(selectors[4]);
    layout->addWidget(buttonGroup);
    setLayout(layout);
}

void frmFourier::init(void)
{
    sel->update_number_of_entries();
}

void frmFourier::doDFT(void)
/*
 * DFT
 */
{
    int gno = get_cg();
    int *selsets=new int[5];
    int i, cnt;
    int setno, load, loadx, invflag, type, wind;
    cnt = GetSelectedSets(sel, &selsets);
    if (cnt == SET_SELECT_ERROR)
    {
        errwin(tr("No sets selected").toAscii().constData());
        return;
    }
    wind = GetChoice(selectors[0]);
    load = GetChoice(selectors[1]);
    loadx = GetChoice(selectors[2]);
    invflag = GetChoice(selectors[3]);
    type = GetChoice(selectors[4]);
    set_wait_cursor();
    int *gnos=new int[cnt];
    int *snos=new int[cnt];
    int counter=0;
    char dummy[64];
    for (i = 0; i < cnt; i++)
    {
        setno = selsets[i];
        new_set_no=-1;
        do_fourier(gno, setno, 0, load, loadx, invflag, type, wind);
        if (new_set_no>=0)
        {
            gnos[counter]=gno;
            snos[counter++]=new_set_no;
        }
    }
    if (counter>0)
    {
        ///Undo-Stuff
        SetsCreated(counter,gnos,snos,UNDO_COMPLETE);
        if (counter==1)
        {
            sprintf(dummy,"[G%d.S%d]",gno,selsets[0]);
            addAditionalDescriptionToLastNode(-1,QObject::tr("Fourier transformation (DFT)")+QString(dummy),QString());
        }
        else
        {
            addAditionalDescriptionToLastNode(-1,QObject::tr("Fourier transformation (DFT)"),QString());
        }
    }
    delete[] snos;
    delete[] gnos;
    update_set_lists(gno);
    delete[] selsets;
    unset_wait_cursor();
    mainWin->mainArea->completeRedraw();
}

void frmFourier::doFFT(void)
/*
 * DFT by FFT
 */
{
    int gno = get_cg();
    int *selsets=new int[5];
    int i, cnt;
    int setno, load, loadx, invflag, type, wind;
    cnt = GetSelectedSets(sel, &selsets);
    if (cnt == SET_SELECT_ERROR)
    {
        errwin(tr("No sets selected").toAscii().constData());
        return;
    }
    wind = GetChoice(selectors[0]);
    load = GetChoice(selectors[1]);
    loadx = GetChoice(selectors[2]);
    invflag = GetChoice(selectors[3]);
    type = GetChoice(selectors[4]);
    set_wait_cursor();
    int *gnos=new int[cnt];
    int *snos=new int[cnt];
    int counter=0;
    char dummy[64];
    for (i = 0; i < cnt; i++)
    {
        setno = selsets[i];
        new_set_no=-1;
        do_fourier(gno, setno, 1, load, loadx, invflag, type, wind);
        if (new_set_no>=0)
        {
            gnos[counter]=gno;
            snos[counter++]=new_set_no;
        }
    }
    if (counter>0)
    {
        ///Undo-Stuff
        SetsCreated(counter,gnos,snos,UNDO_COMPLETE);
        if (counter==1)
        {
            sprintf(dummy,"[G%d.S%d]",gno,selsets[0]);
            addAditionalDescriptionToLastNode(-1,QObject::tr("Fourier transformation (FFT)")+QString(dummy),QString());
        }
        else
        {
            addAditionalDescriptionToLastNode(-1,QObject::tr("Fourier transformation (FFT)"),QString());
        }
    }
    delete[] snos;
    delete[] gnos;
    update_set_lists(gno);
    delete[] selsets;
    unset_wait_cursor();
    mainWin->mainArea->completeRedraw();
}

void frmFourier::doWindowOnly(void)
/*
 * Apply data window only
 */
{
    int *selsets=new int[5];
    int i, cnt;
    int setno, type, wind;
    //Four_ui *ui = (Four_ui *) client_data;
    cnt = GetSelectedSets(sel, &selsets);
    if (cnt == SET_SELECT_ERROR) {
        errwin(tr("No sets selected").toAscii().constData());
        return;
    }
    wind = GetChoice(selectors[0]);
    type = GetChoice(selectors[4]);
    set_wait_cursor();
    for (i = 0; i < cnt; i++) {
        setno = selsets[i];
        do_window(setno, type, wind);
    }
    update_set_lists(get_cg());
    delete[] selsets;
    unset_wait_cursor();
    mainWin->mainArea->completeRedraw();
}

void frmFourier::doClose(void)
{
    hide();
}

void generateLowPass_Brickwall(double * f,int n,double f_cutoff,double * cutoff_Function_r,double * cutoff_Function_i)
{
    for (int i=0;i<n;i++)
        cutoff_Function_i[i]=cutoff_Function_r[i]=((fabs(f[i])<f_cutoff)?1.0:(fabs(f[i])==f_cutoff?0.5:0.0));
}

void generateLowPass_Butterworth_simple(double * f,int n,double f_cutoff,int order,double * cutoff_Function_r,double * cutoff_Function_i)
{
    for (int i=0;i<n;i++)
        cutoff_Function_i[i]=cutoff_Function_r[i]=1.0/sqrt(1.0+pow(f[i]/f_cutoff,2.0*order));
}

void generateLowPass_Butterworth(double * f,int n,double f_cutoff,int order,double * cutoff_Function_r,double * cutoff_Function_i)
{
    struct komplex * ko=new struct komplex[n];
    struct komplex help1;
    double help2;
    double ai,bi;
    for (int i=0;i<n;i++)
        ko[i]=one;
    for (int j=1;j<=(order+1)/2;j++)
    {
        if (1-order%2)//even
        {
            if (j>order/2) continue;
            bi=1.0;
            ai=2.0*cos((2.0*j-1.0)*pi/order*0.5);
        }
        else//odd
        {
            if (j==1)
            {
                bi=0.0;
                ai=1.0;
            }
            else
            {
                bi=1.0;
                ai=2.0*cos((j-1.0)*pi/order);
            }
        }
        for (int i=0;i<n;i++)
        {
            help2=f[i]/f_cutoff;
            help1.real=1.0-bi*help2*help2;
            help1.imag=ai*help2;
            ko[i]=mult_komplex(ko[i],help1);
        }
    }
    for (int i=0;i<n;i++)
    {
        ko[i]=div_komplex(one,ko[i]);
        cutoff_Function_r[i]=ko[i].real;
        cutoff_Function_i[i]=ko[i].imag;
    }
    delete[] ko;
}

void generateLowPass_Bessel(double * f,int n,double f_cutoff,int order,double * cutoff_Function_r,double * cutoff_Function_i)
{
    struct komplex * ko=new struct komplex[n];
    struct komplex help1=zero;
    struct komplex help2=zero;
    double ci=1.0;
    for (int i=0;i<n;i++)
    {
        ko[i].real=1.0;
        ko[i].imag=ci*f[i]/f_cutoff;
    }
    for (int j=2;j<=order;j++)
    {
        ci*=(2.0*(order-j+1.0))/(j*(2.0*order-j+1.0));
        for (int i=0;i<n;i++)
        {
            help2.imag=f[i]/f_cutoff;
            help1=pow_komplex(help2,j);
            help1.real*=ci;
            help1.imag*=ci;
            ko[i]=add_komplex(ko[i],help1);
        }
    }
    for (int i=0;i<n;i++)
    {
        ko[i]=div_komplex(one,ko[i]);
        cutoff_Function_r[i]=ko[i].real;
        cutoff_Function_i[i]=ko[i].imag;
    }
    delete[] ko;
}


double 
asinh_portable(double x)
{ 
#ifdef _MSC_VER
    return log(x+sqrt(x*x+1));
#else
    return asinh(x);
#endif
}

void generateLowPass_Chebychev(double * f,int n,double f_cutoff,int order,double ripple,double * cutoff_Function_r,double * cutoff_Function_i)
{
    struct komplex help1;
    struct komplex * ko=new struct komplex[n];
    double gamma=asinh_portable(ripple)/order;//ripples are in dB
    double ai,bi;
    double help;
    for (int i=0;i<n;i++)
        ko[i]=one;
    for (int j=1;j<=order;j++)
    {
        if (1-order%2)//even
        {
            help=cos((2.0*j-1.0)*pi*0.5/order);
            bi=1.0/(cosh(gamma)*cosh(gamma)-help*help);
            ai=2.0*bi*sinh(gamma)*help;
        }
        else//odd
        {
            if (j==1)
            {
                bi=0.0;
                ai=1.0/sinh(gamma);
            }
            else
            {
                help=cos((j-1.0)*pi/order);
                bi=1.0/(cosh(gamma)*cosh(gamma)-help*help);
                ai=2.0*bi*sinh(gamma)*help;
            }
        }
        for (int i=0;i<n;i++)
        {
            help=f[i]/f_cutoff;
            help1.real=1.0-bi*help*help;
            help1.imag=ai*help;
            ko[i]=mult_komplex(ko[i],help1);
        }
    }
    for (int i=0;i<n;i++)
    {
        ko[i]=div_komplex(one,ko[i]);
        cutoff_Function_r[i]=ko[i].real;
        cutoff_Function_i[i]=ko[i].imag;
    }
    delete[] ko;
}

void generateLowPass_Chebychev2(double * f,int n,double f_cutoff,int order,double ripple,double * cutoff_Function_r,double * cutoff_Function_i)
{
    double epsilon=1.0/sqrt(pow(10.0,0.1*ripple)-1.0);
    komplex t1={1.0,0.0};
    komplex t2={1.0,0.0};
    komplex T;
    komplex x;
    for (int i=0;i<n;i++)
    {
        x.imag=t2.imag=f[i]/f_cutoff;
        x.real=t2.real=0.0;
        if (order==1)
            T=t1;//order=1
        else
            T=t2;//order=2 or greater
        for (int j=3;j<order;j++)
        {
            T=mult_komplex(t2,x);
            T.real*=2.0;
            T.imag*=2.0;
            T=sub_komplex(T,t1);
            ///T=2.0*x*t2-t1;//order=3 --> T=T3=2*x*T2-t1
            t1=t2;
            t2=T;
        }
        x=T;
        x.real*=epsilon;
        x.imag*=epsilon;
        ///x=epsilon*T;
        x=mult_komplex(x,x);
        x.real+=1.0;
        T=pow_komplex(x,-0.5);
        cutoff_Function_r[i]=T.real;
        cutoff_Function_i[i]=T.imag;
        //cutoff_Function_r[i]=cutoff_Function_i[i]=1.0/sqrt(1.0+x);
    }
}

void generateLowPass_Gaussian(double * f,int n,double f_cutoff,int order,double * cutoff_Function_r,double * cutoff_Function_i)
{
    static double alpha=pi/sqrt(log(2.0));
    for (int i=0;i<n;i++)
        cutoff_Function_r[i]=cutoff_Function_i[i]=exp(-f[i]*f[i]/(2.0*alpha*f_cutoff*f_cutoff))/sqrt(2.0);
}

frmFourier2::frmFourier2(QWidget * parent):QDialog(parent)
{
    int rowcount=0;
    int number;
    char dummy[128];
    QString entr[9];
    int values[9];
    setFont(stdFont);
    setWindowIcon(QIcon(*GraceIcon));
    setWindowTitle(tr("qtGrace: Filters"));

    buttonGroup=new stdButtonGroup(this,true,true,false);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    buttonGroup->cmdApply->setDefault(TRUE);
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doFilter()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));

    grpSource=new grpSelect(tr("Source"),this);
    grpDestination=new grpSelect(tr("Destination"),this);

    entr[0]=tr("Low-Pass");
    values[0]=FILTER_LOW_PASS;
    entr[1]=tr("High-Pass");
    values[1]=FILTER_HIGH_PASS;
    entr[2]=tr("Band-Pass");
    values[2]=FILTER_BAND_PASS;
    entr[3]=tr("Band-Stop");
    values[3]=FILTER_BAND_STOP;
    number=4;
    selType=new StdSelector(this,tr("Filter type:"),number,entr);
    selType->setValues(values);
    connect(selType->cmbSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(filterTypeChanged(int)));

    entr[0]=tr("Brickwall");
    values[0]=FILTER_BRICKWALL;
    entr[1]=tr("Butterworth");
    values[1]=FILTER_BUTTERWORTH;
    entr[2]=tr("Bessel");
    values[2]=FILTER_BESSEL;
    entr[3]=tr("Chebychev");
    values[3]=FILTER_CHEBYCHEV;
    entr[4]=tr("Gaussian");
    values[4]=FILTER_GAUSSIAN;
    number=5;
    selKind=new StdSelector(this,tr("Realization:"),number,entr);
    selKind->setValues(values);
    connect(selKind->cmbSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(filterKindChanged(int)));

    entr[0]=tr("No");
    for (int i=1;i<=7;i++)
    {
        sprintf(dummy,"%dx",1<<i);
        entr[i]=QString(dummy);
    }
    number=8;
    selOversampling=new StdSelector(this,tr("Oversampling:"),number,entr);
    chkAbsValue=new QCheckBox(tr("only absolute value of transfer-function"),this);
    chkDebug=new QCheckBox(tr("Debug"),this);
    lenRipple=new stdLineEdit(this,tr("Ripples in transfer-function in dB:"));

    gpbLow=new QGroupBox(tr("First Cut-Off"),this);
    gpbHigh=new QGroupBox(tr("Second Cut-Off"),this);
    layout1=new QHBoxLayout;
    layout1->setMargin(STD_MARGIN);
    layout2=new QHBoxLayout;
    layout2->setMargin(STD_MARGIN);

    lenLowFrequency=new stdLineEdit(gpbLow,tr("Frequency (kHz):"));
    selLowOrder=new stdIntSelector(gpbLow,tr("Order:"),1,30);
    layout1->addWidget(lenLowFrequency);
    layout1->addWidget(selLowOrder);

    lenHighFrequency=new stdLineEdit(gpbHigh,tr("Frequency (kHz):"));
    selHighOrder=new stdIntSelector(gpbHigh,tr("Order:"),1,30);
    layout2->addWidget(lenHighFrequency);
    layout2->addWidget(selHighOrder);

    gpbLow->setLayout(layout1);
    gpbHigh->setLayout(layout2);

    gpbProcessing=new QGroupBox(tr("FFT-processing options"),this);
    layout3=new QGridLayout;
    layout3->setMargin(STD_MARGIN);
    lenTimeFormula=new stdLineEdit(this,tr("conversion-factor for x-axis to time(s): X=X*"));
    entr[0]=tr("Interpolation");
    values[0]=PROCESSING_INTERPOLATION;
    entr[1]=tr("Zero Padding");
    values[1]=PROCESSING_ZERO_PADDING;
    number=2;
    selPoints=new StdSelector(this,tr("Point extension:"),number,entr);
    selPoints->setValues(values);
    connect(selPoints->cmbSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(selPointsChanged(int)));

    layout3->addWidget(lenTimeFormula,0,0,1,2);
    layout3->addWidget(selPoints,1,0,1,1);
    layout3->addWidget(selOversampling,1,1,1,1);
    layout3->addWidget(chkAbsValue,2,0,1,1);
    layout3->addWidget(chkDebug,2,1,1,1);
    layout3->addWidget(lenRipple,3,0,1,2);
    gpbProcessing->setLayout(layout3);

    fraSourceDatFilt=new QGroupBox(tr("Source data filtering"),this);
    number=7;
    entr[0]=tr("None");
    entr[1]=tr("Region 0");
    entr[2]=tr("Region 1");
    entr[3]=tr("Region 2");
    entr[4]=tr("Region 3");
    entr[5]=tr("Region 4");
    entr[6]=tr("Inside graph");
    selRestriction=new StdSelector(fraSourceDatFilt,tr("Restriction:"),number,entr);
    chkNeg=new QCheckBox(tr("Negated"),fraSourceDatFilt);
    chkNeg->setChecked(FALSE);
    layout0=new QHBoxLayout;
    layout0->setMargin(STD_MARGIN);
    layout0->addWidget(selRestriction);
    layout0->addWidget(chkNeg);
    fraSourceDatFilt->setLayout(layout0);

    layout=new QGridLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(grpSource,rowcount,0,1,1);
    layout->addWidget(grpDestination,rowcount++,1,1,1);
    layout->addWidget(selType,rowcount,0,1,1);
    layout->addWidget(selKind,rowcount++,1,1,1);

    layout->addWidget(gpbLow,rowcount++,0,1,2);
    layout->addWidget(gpbHigh,rowcount++,0,1,2);
    layout->addWidget(gpbProcessing,rowcount++,0,1,2);

    layout->addWidget(fraSourceDatFilt,rowcount++,0,1,2);
    layout->addWidget(buttonGroup,rowcount++,0,1,2);
    setLayout(layout);
}

void frmFourier2::init(void)
{
    lenLowFrequency->setText(QString("50"));
    lenHighFrequency->setText(QString("100"));
    lenTimeFormula->setText(QString("1"));
    lenRipple->setText(QString("10"));
    selOversampling->setCurrentIndex(0);
    selLowOrder->setValue(2);
    selHighOrder->setValue(2);
    selKind->setCurrentIndex(FILTER_BUTTERWORTH);
    filterKindChanged(FILTER_BUTTERWORTH);
    selType->setCurrentIndex(FILTER_LOW_PASS);
    filterTypeChanged(FILTER_LOW_PASS);
    grpSource->update_number_of_entries();
    grpDestination->update_number_of_entries();
}

void frmFourier2::doAccept(void)
{
    doFilter();
    if (ApplyError==false)
        doClose();
}

void frmFourier2::doFilter(void)
{
    ApplyError=false;
    double f1,f2,ripple;
    int order1,order2;
    double factor=1.0;
    if (selKind->currentValue()==FILTER_BRICKWALL && chkAbsValue->isChecked())
    {
        factor=1.0/sqrt(2.0);
    }

    int error, resno;
    int i, g1_ok, g2_ok, ns1, ns2, * svalues1=new int[2], * svalues2=new int[2],gno1, gno2, setno1, setno2;
    char fstr[256];
    char dummy[256];
    int restr_type, restr_negate;
    char * rarray;
    int * sel_graphs=new int[2];
    int setlen;
    int n_setlen;
    int * snos,*gnos,n_n_sets=0;

    double xmin, xmax,ymin,ymax;
    int workset1,workset2,workset3,workset4,workset5;
    double * mesh = NULL;

    set_wait_cursor();

    strcpy(dummy,lenLowFrequency->text().toAscii());
    f1=atof(dummy)*1000.0;
    strcpy(dummy,lenHighFrequency->text().toAscii());
    f2=atof(dummy)*1000.0;
    order1=selLowOrder->value();
    order2=selHighOrder->value();
    strcpy(dummy,lenRipple->text().toAscii());
    ripple=atof(dummy);

    switch (selRestriction->currentIndex())
    {
    case 0:
        restr_type=RESTRICT_NONE;
        break;
    case 1:
        restr_type=RESTRICT_REG0;
        break;
    case 2:
        restr_type=RESTRICT_REG1;
        break;
    case 3:
        restr_type=RESTRICT_REG2;
        break;
    case 4:
        restr_type=RESTRICT_REG3;
        break;
    case 5:
        restr_type=RESTRICT_REG4;
        break;
    case 6:
        restr_type=RESTRICT_WORLD;
        break;
    }

    restr_negate = chkNeg->isChecked()==TRUE?1:0;
    grpSource->listGraph->get_selection(&g1_ok,&sel_graphs);
    if (g1_ok==1)
    {
        gno1=sel_graphs[0];
        g1_ok=RETURN_SUCCESS;
    }
    else
        g1_ok=RETURN_FAILURE;

    grpDestination->listGraph->get_selection(&g2_ok,&sel_graphs);
    if (g2_ok==1)
    {
        gno2=sel_graphs[0];
        g2_ok=RETURN_SUCCESS;
    }
    else
        g2_ok=RETURN_FAILURE;

    grpSource->listSet->get_selection(&ns1,&svalues1);
    grpDestination->listSet->get_selection(&ns2,&svalues2);

    if (ns1>0)
    {
        snos=new int[ns1];
        gnos=new int[ns1];
        for (int i=0;i<ns1;i++)
            gnos[i]=gno2;
        n_n_sets=ns2;
        for (int i=0;i<ns2;i++)
            snos[i]=svalues2[i];
        SaveSetStatesPrevious(n_n_sets,gnos,snos,UNDO_COMPLETE);
        n_n_sets=0;
    }

    error = FALSE;
    if (g1_ok == RETURN_FAILURE || g2_ok == RETURN_FAILURE) {
        error = TRUE;
        ApplyError=true;
        errmsg(tr("Please select single source and destination graphs").toAscii().constData());
    } else if (ns1 == 0) {
        error = TRUE;
        ApplyError=true;
        errmsg(tr("No source sets selected").toAscii().constData());
    } else if (ns1 != ns2 && ns2 != 0) {
        error = TRUE;
        ApplyError=true;
        errmsg(tr("Different number of source and destination sets").toAscii().constData());
    } else {

        for (i = 0; i < ns1; i++) ///go through all source-sets
        {
            sprintf(fstr,"X=X*%s",lenTimeFormula->text().toAscii().constData());
            setno1 = svalues1[i];
            if (ns2 != 0)
            {
                setno2 = svalues2[i];
                snos[n_n_sets++]=setno2;
            } else {
                setno2 = nextset(gno2);
                set_set_hidden(gno2, setno2, FALSE);
                snos[n_n_sets++]=setno2;
            }
            workset5=-1;
            if (setno2==setno1 && gno2==gno1)//source and destination are the same
            {
                workset5=setno2;
                setno2=nextset(gno2);//we need an extra set here!
            }
            //first set to be created is setno2 which will be the set number for the result
            resno = get_restriction_array(gno1, setno1,restr_type, restr_negate, &rarray);//generate restriction array
            if (resno != RETURN_SUCCESS) {
                errmsg(tr("Error in evaluation of restriction").toAscii().constData());
                ApplyError=true;
                break;
            }
            set_parser_setno(gno1,setno1);
            //setlen=getsetlength(gno1, setno1);
            //n_setlen=(int)(pow(2.0,ceil(log((double)setlen)/log(2.0))));

            ///STEPS TO DO:
            //Calculate the real X-values (has to be time in seconds) and save them in new set --> setno2
            resno = do_compute(gno1, setno1, gno2, setno2, rarray, fstr);

            /*workset5= nextset(gno2);
            cout << "G" << gno2 << ".S" << setno2 << "=Zielset, fstr=" << fstr << "Ergebnis kopiert in G" << gno2 << ".S" << workset5 << endl;
            do_copyset(gno2, setno2, gno2, workset5);*/

            //-->setno2 not interpolated yet! --> x-values will be needed at the end
            /*do_copyset(gno2, setno2, gno2, workset1);*/

            if (resno != RETURN_SUCCESS) {
                errmsg(tr("Error in evaluation of x-axis conversion").toAscii().constData());
                ApplyError=true;
                break;
            }
            setlen=getsetlength(gno2, setno2);
            //n_setlen=(int) (pow(2.0,ceil(log((double)setlen)/log(2.0))));
            n_setlen=1<<((int)ceil(log((double)setlen)/log(2.0)));//get next power of two for interpolation
            ///cout << "neues umgerechnetes set hat " << setlen << " Punkte und die naechste Potenz von 2 ist " << n_setlen << endl;

            //interpolate or add zeros (get min_x and max_x first) --> n_setlen will be new setlength
            workset1= nextset(gno2);
            resno = getsetminmax(gno2, setno2,&xmin,&xmax,&ymin,&ymax);
            double xspace;
            bool mono=monospaced_v6(g[gno2].p[setno2].data.ex[0], setlen, &xspace);
            if (!mono && selPoints->currentValue()==PROCESSING_ZERO_PADDING)
            {
                errmsg(tr("Error! Set not monospaced in x! Switching to interpolation!").toAscii().constData());
                ApplyError=true;
            }
            if (selPoints->currentValue()==PROCESSING_ZERO_PADDING && mono==RETURN_SUCCESS)
            {
                //zero padding
                do_copyset(gno2, setno2,gno2, workset1);
                for (int i=setlen;i<n_setlen;i++)
                    add_point(gno2,workset1,xmax+xspace*(i-setlen+1),0.0);
            }
            else
            {
                //interpolation
                n_setlen*=(1<<selOversampling->currentIndex());//Oversampling
                if (resno != RETURN_SUCCESS) {
                    errmsg(tr("Error in computation of min and max").toAscii().constData());
                    ApplyError=true;
                    break;
                }
                mesh = allocate_mesh(xmin, xmax, n_setlen);
                resno = do_interp(gno2, setno2, gno2, workset1, mesh, n_setlen, 0, 0);
                if (resno != RETURN_SUCCESS) {
                    errmsg(tr("Error in generation of interpolated set").toAscii().constData());
                    ApplyError=true;
                    break;
                }
            }

            //-->workset1 is a set with the points selected and the length a power of 2
            //cout << "G" << gno2 << ".S" << workset1 << "= Arbeitsset = originaldaten, richtige x-Achse, richtige Laenge" << endl;
            /*
               real_data ... ptr. to real part of data to be transformed
               imag_data ... ptr. to imag  "   "   "   "  "      "
               inv ..... Switch to flag normal or inverse transform
               n_pts ... Number of real data points
               nu ...... logarithm in base 2 of n_pts e.g. nu = 5 if n_pts = 32.
               int fft_v6(double *real_data, double *imag_data, int n_pts, int nu, int inv);
            */
            workset2= nextset(gno2);
            do_copyset(gno2, workset1, gno2, workset2);
            workset3= nextset(gno2);
            do_copyset(gno2, workset1, gno2, workset3);
            workset4= nextset(gno2);
            do_copyset(gno2, workset1, gno2, workset4);
            int n_pts=n_setlen;
            int nu=(int)(ceil(log((double)n_setlen)/log(2.0)));
            double * real_data=new double[n_pts];
            double * imag_data=new double[n_pts];
            double * cutoff_function=new double[n_pts];
            double * cut_r=new double[n_pts];
            double * cut_i=new double[n_pts];
            //copy data into real part for fft
            memset(imag_data,0,sizeof(double)*n_pts);
            memcpy(real_data,g[gno2].p[workset1].data.ex[1],sizeof(double)*n_pts);
            mono=monospaced_v6(g[gno2].p[workset1].data.ex[0], n_pts, &xspace);
            if (!mono)
            {
                errmsg(tr("Error! Set not monospaced in x!").toAscii().constData());
                ApplyError=true;
            }
            //do fft
            resno = fft_v6(real_data,imag_data,n_pts,nu,0);
            if (resno != RETURN_SUCCESS) {
                errmsg(tr("Error in forward fft").toAscii().constData());
                ApplyError=true;
                break;
            }
            //generate cutoff function --> according to Filter-parameters set
            /* amplitude correction due to the zero padding etc */
            double amp_correction = 1.0/sqrt((double)n_pts);//(double) buflen/inlen;
            for (int i=0;i<n_pts;i++)
            {
                g[gno2].p[workset2].data.ex[0][i]=((double)i)/(xspace*n_pts);//x-values in frequency-space
                /*if (i<n_pts/2)
            cutoff_function[i]=(g[gno2].p[workset2].data.ex[0][i]>f1)?0.0:1.0;//LOW_PASS order 0
            else
            cutoff_function[i]=(g[gno2].p[workset2].data.ex[0][n_pts-i]>f1)?0.0:1.0;*/
                g[gno2].p[workset2].data.ex[1][i]=amp_correction*hypot(real_data[i], imag_data[i]);
            }
            for (int i=n_pts/2;i<n_pts;i++)
                g[gno2].p[workset2].data.ex[0][i]=-g[gno2].p[workset2].data.ex[0][n_pts-i-1];

            //cout << "G" << gno2 << ".S" << workset2 << "= Arbeitsset = Ergebins der Transformation" << endl;

            ///order=n*20dB/Dekade
            ///--> 1.order = decrease by factor 10 at frequency*10
            ///--> 2.order = decrease by factor 100 at frequency*10
            ///--> n-th Order= decrease by factor 10^n at frequency*10
            ///LowPass-cutoff-function:
            ///V(f) = 1.0/sqrt( 1.0 + (f/f_cut)^(2.0*n) )

            //We always generate a low pass at the beginning
            switch (selKind->currentValue())
            {
            case FILTER_BRICKWALL:
                generateLowPass_Brickwall(g[gno2].p[workset2].data.ex[0],n_pts,f1,cut_r,cut_i);
                break;
            case FILTER_BUTTERWORTH:
                generateLowPass_Butterworth(g[gno2].p[workset2].data.ex[0],n_pts,f1,order1,cut_r,cut_i);
                break;
            case FILTER_BESSEL:
                generateLowPass_Bessel(g[gno2].p[workset2].data.ex[0],n_pts,f1,order1,cut_r,cut_i);
                break;
            case FILTER_CHEBYCHEV:
                generateLowPass_Chebychev(g[gno2].p[workset2].data.ex[0],n_pts,f1,order1,ripple,cut_r,cut_i);
                break;
            case FILTER_GAUSSIAN:
                //generateLowPass_Butterworth_simple(g[gno2].p[workset2].data.ex[0],n_pts/2,f1,order1,cut_r,cut_i);
                generateLowPass_Gaussian(g[gno2].p[workset2].data.ex[0],n_pts,f1,order1,cut_r,cut_i);
                //generateLowPass_Gaussian(double * f,int n,double f_cutoff,int order,double * cutoff_Function)
                break;
            }

            if (selType->currentValue()==FILTER_BAND_PASS || selType->currentValue()==FILTER_BAND_STOP)
            {//in this case we have to generate another cut-off-function for the second filter frequency
                //At first we always generate a band-pass-filter
                double * cut_r2=new double[n_pts];
                double * cut_i2=new double[n_pts];

                switch (selKind->currentValue())
                {
                case FILTER_BRICKWALL:
                    generateLowPass_Brickwall(g[gno2].p[workset2].data.ex[0],n_pts,f2,cut_r2,cut_i2);
                    break;
                case FILTER_BUTTERWORTH:
                    generateLowPass_Butterworth(g[gno2].p[workset2].data.ex[0],n_pts,f2,order2,cut_r2,cut_i2);
                    break;
                case FILTER_BESSEL:
                    generateLowPass_Bessel(g[gno2].p[workset2].data.ex[0],n_pts,f2,order2,cut_r2,cut_i2);
                    break;
                case FILTER_CHEBYCHEV:
                    generateLowPass_Chebychev(g[gno2].p[workset2].data.ex[0],n_pts,f2,order2,ripple,cut_r2,cut_i2);
                    break;
                case FILTER_GAUSSIAN:
                    generateLowPass_Gaussian(g[gno2].p[workset2].data.ex[0],n_pts,f2,order2,cut_r2,cut_i2);
                    break;
                }
                for(int i=0;i<n_pts;i++)
                {
                    //cutoff_function[i]=(1.0-cutoff_function[i])*(1.0/sqrt(1.0+pow(g[gno2].p[workset2].data.ex[0][i]/f2,2.0*order2)));
                    cut_r[i]=(1.0-cut_r[i])*cut_r2[i];
                    cut_i[i]=(1.0-cut_i[i])*cut_i2[i];
                }
                /*if (order2==0)
                {
                    for(int i=0;i<n_pts/2;i++)
                    {
                    cutoff_function[i]=(1.0-cutoff_function[i])*(g[gno2].p[workset2].data.ex[0][i]<f2?0.0:1.0);
                    cutoff_function[n_pts-1-i]=cutoff_function[i];
                    }
                }
                else//order2>0
                {
                    for(int i=0;i<n_pts/2;i++)
                    {
                    cutoff_function[i]=(1.0-cutoff_function[i])*(1.0/sqrt(1.0+pow(g[gno2].p[workset2].data.ex[0][i]/f2,2.0*order2)));
                    cutoff_function[n_pts-1-i]=cutoff_function[i];
                    }
                }*/
                delete[] cut_r2;
                delete[] cut_i2;
            }

            for(int i=0;i<n_pts;i++)//fft-spectra symetrical --> fill the second half
            {
                cutoff_function[i]=hypot(cut_r[i],cut_i[i]);
                //cutoff_function[n_pts-1-i]=cutoff_function[i];
                //cut_r[n_pts-1-i]=cut_r[i];
                //cut_i[n_pts-1-i]=cut_i[i];
            }

            for(int i=0;i<n_pts;i++)
                g[gno2].p[workset4].data.ex[0][i]=g[gno2].p[workset2].data.ex[0][i];

            switch (selType->currentValue())
            {
            case FILTER_LOW_PASS:
            case FILTER_BAND_PASS:
                for(int i=0;i<n_pts;i++)
                    g[gno2].p[workset4].data.ex[1][i]=cutoff_function[i];
                //g[gno2].p[workset4].data.ex[1][i]=180.0/pi*atan2(cut_i[i],cut_r[i]);//cutoff_function[i];
                break;
            case FILTER_HIGH_PASS:
            case FILTER_BAND_STOP:
                for(int i=0;i<n_pts;i++)
                {
                    cut_r[i]=1.0-cut_r[i];
                    cut_i[i]=1.0-cut_i[i];
                    cutoff_function[i]=1.0-cutoff_function[i];
                    g[gno2].p[workset4].data.ex[1][i]=cutoff_function[i];
                    //g[gno2].p[workset4].data.ex[1][i]=180.0/pi*atan2(cut_i[i],cut_r[i]);//1.0-cutoff_function[i];
                }
                break;
            }

            //cout << "G" << gno2 << ".S" << workset4 << "=Abschneidefunktion" << endl;
            //multiply fft-coefficients by cutoff function
            if (chkAbsValue->isChecked())
            {
                for (int i=0;i<n_pts;i++)
                {
                    real_data[i]*=amp_correction*cutoff_function[i]*factor;
                    imag_data[i]*=amp_correction*cutoff_function[i]*factor;
                }
            }
            else
            {
                struct komplex tmp_ko,tmp_ko2;
                for (int i=0;i<n_pts;i++)
                {
                    tmp_ko.real=real_data[i];
                    tmp_ko.imag=imag_data[i];
                    tmp_ko2.real=cut_r[i];
                    tmp_ko2.imag=cut_i[i];
                    tmp_ko=mult_komplex(tmp_ko,tmp_ko2);
                    real_data[i]=amp_correction*tmp_ko.real*factor;
                    imag_data[i]=amp_correction*tmp_ko.imag*factor;
                }
            }

            //do inverse-fft
            resno = fft_v6(real_data,imag_data,n_pts,nu,1);
            if (resno != RETURN_SUCCESS)
            {
                errmsg(tr("Error in forward fft").toAscii().constData());
                ApplyError=true;
                break;
            }

            //copy result to workset3
            for (int i=0;i<n_pts;i++)
            {
                //g[gno2].p[workset3].data.ex[0][i]=((double)i)/(xspace*n_pts);
                g[gno2].p[workset3].data.ex[1][i]=amp_correction*real_data[i];//-->workset3 ist das zuruecktransormierte
            }
            //cout << "G" << gno2 << ".S" << workset3 << "= Arbeitsset = Rücktransformierte" << endl;

            //interpolate result to original X-values
            //delete worksets if necessary

            do_copyset(gno2, workset2, gno2, workset1);//save workset2 to workset1
            sprintf(fstr,"X=X/%s",lenTimeFormula->text().toAscii().constData());
            set_parser_setno(gno2,workset3);
            resno = do_compute(gno2, workset3, gno2, workset2, NULL, fstr);//transform X-axis back to original scale-->resulting data present in workset2 but probably to many points
            //cout << "G" << gno2 << ".S" << workset2 << "= Arbeitsset = x-achse transformiert" << endl;
            //meshlen = getsetlength(gsampl, setnosampl);
            xfree(mesh);
            mesh = getcol(gno1, setno1, DATA_X);//get original x-values
            resno = do_interp(gno2, workset2, gno2, setno2, mesh, setlen, 0, 0);//interpolate back to original set axis
            if (resno != RETURN_SUCCESS)
            {
                errmsg(tr("Error in generation of interpolated set").toAscii().constData());
                ApplyError=true;
                break;
            }
            //do_copyset(gno2, workset2, gno2, setno2);//HIER FEHLT DAS INTERPOLIEREN! MIT MESH VON ANDEREM SET!
            //set_parser_setno(gno2, setno2);
            ///copycol2(src_gno, src_setno, dest_gno, dest_setno, DATA_X);
            ///resno = do_compute(gno1, setno1, gno2, setno2, rarray, fstr);

            if (!chkDebug->isChecked())
            {
                killset(gno2,workset2);
                killset(gno2,workset3);
                killset(gno2,workset1);
                killset(gno2,workset4);
            }

            if (workset5==setno1)//source and destination are the same
            {
                //do_copyset(gno2, setno2, gno2, workset5);
                copysetdata(gno2, setno2, gno2, workset5);
                killset(gno2,setno2);
            }

            resno=RETURN_SUCCESS;
            XCFREE(rarray);
            delete[] real_data;
            delete[] imag_data;
            delete[] cutoff_function;
            delete[] cut_r;
            delete[] cut_i;
            if (resno != RETURN_SUCCESS) {
                errmsg(tr("Error in do_compute(), check formula").toAscii().constData());
                ApplyError=true;
                break;
            }
        }
    }

    if (error == FALSE)
    {
        if (gno1 != gno2)
        {
            update_set_lists(gno1);
            update_set_lists(gno2);
        } else {
            update_set_lists(gno1);
        }

        SetsModified(n_n_sets,gnos,snos,UNDO_COMPLETE);

        mainWin->mainArea->completeRedraw();
        grpSource->mark_multiple_sets(gno1,ns1,svalues1);
        if (ns2==0)
        {
            ns2=1;
            svalues2[0]=setno2;
        }
        if (n_n_sets>0)
            grpDestination->mark_multiple_sets(gno2,n_n_sets,snos);
    }
    unset_wait_cursor();
    delete[] sel_graphs;
    delete[] svalues1;
    delete[] svalues2;
}

void frmFourier2::selPointsChanged(int i)
{
    switch (selPoints->currentIndex())
    {
    case 0:
        selOversampling->setEnabled(true);
        break;
    case 1:
        selOversampling->setEnabled(false);
        break;
    }
}

void frmFourier2::filterKindChanged(int i)
{
    if (selKind->currentValue()==FILTER_CHEBYCHEV)
    {
        lenRipple->setVisible(true);
    }
    else
    {
        lenRipple->setVisible(false);
    }
    switch (selKind->currentIndex())
    {
    case FILTER_BRICKWALL:
        selLowOrder->setEnabled(false);
        selHighOrder->setEnabled(false);
        break;
    default:
        selLowOrder->setEnabled(true);
        selHighOrder->setEnabled(true);
        break;
    }
}

void frmFourier2::filterTypeChanged(int i)
{
    switch (selType->currentValue())
    {
    case FILTER_LOW_PASS:
        gpbLow->setVisible(true);
        gpbHigh->setVisible(false);
        gpbLow->setTitle(tr("Cut-Off"));
        break;
    case FILTER_HIGH_PASS:
        gpbLow->setVisible(true);
        gpbHigh->setVisible(false);
        gpbLow->setTitle(tr("Cut-Off"));

        break;
    case FILTER_BAND_PASS:
        gpbLow->setVisible(true);
        gpbHigh->setVisible(true);
        gpbLow->setTitle(tr("High Pass Cut-Off"));
        gpbHigh->setTitle(tr("Low Pass Cut-Off"));
        break;
    case FILTER_BAND_STOP:
        gpbLow->setVisible(true);
        gpbHigh->setVisible(true);
        gpbLow->setTitle(tr("Low Pass Cut-Off"));
        gpbHigh->setTitle(tr("High Pass Cut-Off"));
        break;
    }
}

void frmFourier2::doClose(void)
{
    hide();
}

frmHotLinks::frmHotLinks(QWidget * parent):QDialog(parent)
{
    int number;
    QString entr[2];
    setFont(stdFont);
    setWindowIcon(QIcon(*GraceIcon));
    setWindowTitle(tr("qtGrace: Hot links"));
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

    layout=new QGridLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(hotlink_list_item,0,0,1,5);
    layout->addWidget(lblLinkSet,1,0,1,5);
    layout->addWidget(hotlink_set_item,2,0,1,5);
    layout->addWidget(hotlink_file_item,3,0,1,5);
    layout->addWidget(hotlink_source_item,4,0,1,5);
    layout->addWidget(buttons[0],5,0);
    layout->addWidget(buttons[1],5,1);
    layout->addWidget(buttons[2],5,2);
    layout->addWidget(buttons[3],5,3);
    layout->addWidget(buttons[4],5,4);
    setLayout(layout);
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
    char fname[256];
    char buf[256];
    QString xms;

    set_wait_cursor();

    numset = GetSelectedSets(hotlink_set_item, &sets);
    src = GetChoice(hotlink_source_item);
    //strcpy(fname, xv_getstr(hotlink_file_item));
    strcpy(fname,hotlink_file_item->text().toAscii());

    if (numset <= 0){//if (numset == SET_SELECT_ERROR) {
        errwin(tr("No set selected").toAscii().constData());
        goto end_doLink;
    }
    if (fname[0] == '\0') {
        errwin(tr("No source selected").toAscii().constData());
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
            if ( strcpy(cstr,cs.toAscii()) )
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
        if (is_hotlinked(get_cg(), j)) {
            sprintf(buf, "G%d.S%d -> %s -> %s:%d", get_cg(), j,
                    get_hotlink_src(get_cg(), j) == SOURCE_DISK ? "DISK" : "PIPE",
                    get_hotlink_file(get_cg(), j), is_hotlinked(get_cg(),j) );
            xms=QString(buf);
            hotlink_list_item->add_Item(xms);
        }
    }
    hotlink_list_item->update_number_of_entries_preserve_selection();
    unset_wait_cursor();
    //}
}

void frmHotLinks::doClose()
{
    hide();
}

void frmHotLinks::newLinkFileSelected(int type,QString file,bool exists,bool writeable,bool readable)
{
    char *dummy=new char[file.length()+1];// Win port
    strcpy(dummy,file.toAscii());
    /*//for testing -- maybe some tests needed (readable files...)
cout << "type=" << type << endl;
cout << "file=" << dummy << endl;
cout << "exists=" << exists << endl;
cout << "writeable=" << writeable << endl;
cout << "readable=" << readable << endl;*/
    delete[]dummy;
    FormSelectHotLink->hide();
    hotlink_file_item->setText(file);
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

    regtypes[REGION_POLYI]=tr("INSIDE POLY");
    regtypes[REGION_POLYO]=tr("OUTSIDE POLY");
    regtypes[REGION_ABOVE]=tr("REGION_ABOVE");
    regtypes[REGION_BELOW]=tr("REGION_BELOW");
    regtypes[REGION_TOLEFT]=tr("REGION_TOLEFT");
    regtypes[REGION_TORIGHT]=tr("REGION_TORIGHT");
    regtypes[REGION_HORIZI]=tr("REGION_HORIZI");
    regtypes[REGION_VERTI]=tr("REGION_VERTI");
    regtypes[REGION_HORIZO]=tr("REGION_HORIZO");
    regtypes[REGION_VERTO]=tr("REGION_VERTO");

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
    layout0->setMargin(STD_MARGIN);
    layout0->addWidget(lblHeader);
    for (int i=0;i<MAXREGION;i++)
        layout0->addWidget(lblStatus[i]);

    background->setLayout(layout0);

    scroll=new QScrollArea(this);
    scroll->setWidget(background);

    layout=new QGridLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(scroll,0,0,1,2);
    layout->addWidget(cmdClose,1,0);
    layout->addWidget(cmdUpdate,1,1);
    setLayout(layout);
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
    setFont(stdFont);
    setWindowIcon(QIcon(*GraceIcon));
    setWindowTitle(tr(""));
    windowtype=type;

    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);

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
    define_region(nr, rtype);
}

void frmRegions::define_region(int nr, int rtype)
{
    CopyRegion(rg+nr,&undo_region);//copy old region into temporary memory
    CopyRegion(rg+nr,&undo_region2);
    kill_region(nr);
    switch (rtype) {
    case 0:
        regiontype = REGION_POLYI;
        do_select_region();
        break;
    case 1:
        regiontype = REGION_POLYO;
        do_select_region();
        break;
    case 2:
        regiontype = REGION_ABOVE;
        set_action(DO_NOTHING);
        set_action(DEF_REGION1ST);
        break;
    case 3:
        regiontype = REGION_BELOW;
        set_action(DO_NOTHING);
        set_action(DEF_REGION1ST);
        break;
    case 4:
        regiontype = REGION_TOLEFT;
        set_action(DO_NOTHING);
        set_action(DEF_REGION1ST);
        break;
    case 5:
        regiontype = REGION_TORIGHT;
        set_action(DO_NOTHING);
        set_action(DEF_REGION1ST);
        break;
    case 6:
        regiontype= REGION_HORIZI;
        set_action(DO_NOTHING);
        set_action(DEF_REGION1ST);
        break;
    case 7:
        regiontype= REGION_VERTI;
        set_action(DO_NOTHING);
        set_action(DEF_REGION1ST);
        break;
    case 8:
        regiontype= REGION_HORIZO;
        set_action(DO_NOTHING);
        set_action(DEF_REGION1ST);
        break;
    case 9:
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

frmEditBlockData::frmEditBlockData(QWidget * parent):QDialog(parent)
{
    char dummy[96];
    int number;
    QString entr[NUMBER_OF_SETTYPES>20?NUMBER_OF_SETTYPES:20];
    QString ending=tr(" from column:");
    setFont(stdFont);
    setWindowIcon(QIcon(*GraceIcon));
    setWindowTitle(tr("qtGrace: Edit block data"));

    block_curtype = SET_XY;
    begining=tr("Block data: ");
    middle=tr(" column(s) of length ");

    grpLoadTo=new grpSelect(tr("Load to:"),this);
    grpLoadTo->listSet->setSelectionMode(QAbstractItemView::SingleSelection);
    lblData=new QLabel(QString(""),this);
    lblData->setFrameStyle(QFrame::Box | QFrame::Sunken);
    grpColumns=new QGroupBox(QString(""),this);
    number=5;
    entr[0]=QString("Index");
    for (int i=0;i<number;i++)
    {
        sprintf(dummy,"%d",i+1);
        entr[i+1]=QString(dummy);
    }
    for (int i=0;i<6;i++)
        columnSelector[i]=new StdSelector(grpColumns,QString(""),number,entr);
    columnSelector[0]->lblText->setText(QString("X")+ending);
    columnSelector[1]->lblText->setText(QString("Y")+ending);
    columnSelector[2]->lblText->setText(QString("Y1")+ending);
    columnSelector[3]->lblText->setText(QString("Y2")+ending);
    columnSelector[4]->lblText->setText(QString("Y3")+ending);
    columnSelector[5]->lblText->setText(QString("Y4")+ending);
    number=NUMBER_OF_SETTYPES;
    for (int i=0;i<NUMBER_OF_SETTYPES;i++)
    {
        strToUpper(dummy,set_types(i));
        entr[i]=QString(dummy);
    }
    selType=new StdSelector(grpColumns,tr("Set type:"),number,entr);
    connect(selType->cmbSelect,SIGNAL(currentIndexChanged(int)),SLOT(setTypeChanged(int)));
    number=5;
    for (int i=0;i<number;i++)
    {
        sprintf(dummy,"%d",i);
        entr[i]=QString(dummy);
    }
    entr[0]=tr("None");
    selStringColumn=new StdSelector(grpColumns,tr("Strings from column:"),number,entr);
    number=4;
    entr[1]=QString("X");
    entr[2]=QString("Y");
    entr[3]=QString("XY");
    selAutoscale=new StdSelector(this,tr("Autoscale graph on load:"),number,entr);
    selAutoscale->setCurrentIndex(3);
    buttonGroup=new stdButtonGroup(this,true,true,false);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));

    layout0=new QVBoxLayout;
    layout0->setMargin(STD_MARGIN);
    layout0->addWidget(selType);
    for (int i=0;i<6;i++)
        layout0->addWidget(columnSelector[i]);
    layout0->addWidget(selStringColumn);
    grpColumns->setLayout(layout0);
    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(lblData);
    layout->addWidget(grpLoadTo);
    layout->addWidget(grpColumns);
    layout->addWidget(selAutoscale);
    layout->addWidget(buttonGroup);
    setLayout(layout);
    setTypeChanged(0);
}

void frmEditBlockData::setCompleteData(int columns,int length)
{
    char dummy[32],dummy2[32];
    sprintf(dummy,"%d",columns);
    sprintf(dummy2,"%d",length);
    lblData->setText(begining+QString(dummy)+middle+QString(dummy2));
}

void frmEditBlockData::setTypeChanged(int i)
{
    int ncols=settype_cols(i);
    block_curtype = i;
    for (int i=0;i<ncols;i++)
        columnSelector[i]->setEnabled(TRUE);
    for (int i=ncols;i<6;i++)
        columnSelector[i]->setEnabled(FALSE);
}

void frmEditBlockData::init(void)
{
    grpLoadTo->listGraph->update_number_of_entries();
    grpLoadTo->listSet->update_number_of_entries();
    update_eblock(get_cg());
}

void frmEditBlockData::update_eblock(int gno)
{
    //init();
    int number;
    QString * entr1,*entr2;
    int * values1,*values2;
    int blocklen, blockncols;
    int *blockformats;
    int i, ncols, nncols, nscols;
    char buf[128];

    blockncols = get_blockncols();
    if (blockncols == 0) {
        QString err=tr("Need to read block data first");
        char *dummy_err=new char[err.length()+1];// Win port
        strcpy(dummy_err,err.toAscii());
        errmsg(dummy_err);
        delete [] dummy_err;
        return;
    }
    blocklen = get_blocknrows();
    blockformats = get_blockformats();
    if (is_valid_gno(gno))
    {
        ///SelectListChoice(eblock_graphset_item->graph_sel, gno);
        int selection[2]={gno,0};
        grpLoadTo->listGraph->set_new_selection(1,selection);
        grpLoadTo->listSet->set_graph_number(gno,false);
    }
    setCompleteData(blockncols,blocklen);
    ///sprintf(buf, "Block data: %d column(s) of length %d",blockncols, blocklen);
    ///SetLabel(eblock_ncols_item, buf);
    /* TODO: check if new data arrived */
    if (1) {
        number=blockncols+1;
        entr1=new QString[number];
        values1=new int[number];
        entr2=new QString[number];
        values2=new int[number];
        entr1[0]=tr("Index");
        entr2[0]=tr("None");
        values1[0]=values2[0]=-1;

        /*blockitems  = xmalloc((blockncols + 1)*sizeof(OptionItem));
        sblockitems = xmalloc((blockncols + 1)*sizeof(OptionItem));
        blockitems[0].value = -1;
        blockitems[0].label = copy_string(NULL, "Index");
        sblockitems[0].value = -1;
        sblockitems[0].label = copy_string(NULL, "None");*/
        nncols = 0;
        nscols = 0;

        for (i = 0; i < blockncols; i++) {
            sprintf(buf, "%d", i + 1);
            if (blockformats[i] != FFORMAT_STRING) {
                nncols++;
                values1[nncols]=i;
                entr1[nncols]=QString(buf);
                /*blockitems[nncols].value = i;
                blockitems[nncols].label = copy_string(NULL, buf);*/
            } else {
                nscols++;
                values2[nncols]=i;
                entr2[nncols]=QString(buf);
                /*sblockitems[nscols].value = i;
                sblockitems[nscols].label = copy_string(NULL, buf);*/
            }
        }

        for (i = 0; i < MAX_SET_COLS; i++)
        {
            int oldchoice = columnSelector[i]->currentValue();///GetOptionChoice(eblock_nchoice_items[i]);
            ///UpdateOptionChoice(eblock_nchoice_items[i],nncols + 1, blockitems);
            columnSelector[i]->setNewEntries(nncols+1,entr1,values1);
            if (oldchoice < blockncols){
                ///SetOptionChoice(eblock_nchoice_items[i], oldchoice);
                columnSelector[i]->setCurrentValue(oldchoice);
            } else if (i < blockncols) {
                ///SetOptionChoice(eblock_nchoice_items[i], i);
                columnSelector[i]->setCurrentValue(i);
            }
        }
        ///UpdateOptionChoice(eblock_schoice_item, nscols + 1, sblockitems);
        selStringColumn->setNewEntries(nscols+1,entr2,values2);

        /*for (i = 0; i < nncols + 1; i++) {
            xfree(blockitems[i].label);
        }
        xfree(blockitems);
        for (i = 0; i < nscols + 1; i++) {
            xfree(sblockitems[i].label);
        }
        xfree(sblockitems);*/
        delete[] entr1;
        delete[] values1;
        delete[] entr2;
        delete[] values2;
    }

    ncols = settype_cols(block_curtype);
    setTypeChanged(block_curtype);
    /*for (i = 0; i < MAX_SET_COLS; i++) {
        SetSensitive(eblock_nchoice_items[i]->menu, (i < ncols));
    }*/
}


void frmEditBlockData::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmEditBlockData::doApply(void)
{
    ApplyError=false;
    int i, gno, setno;
    int cs[MAX_SET_COLS], nncols, scol, autoscale;
    int * graphs=new int[2];
    int anz_graphs;
    int * sets=new int[2];
    int anz_sets;
    set_wait_cursor();
    //get current selection
    grpLoadTo->listGraph->get_selection(&anz_graphs,&graphs);
    grpLoadTo->listSet->get_selection(&anz_sets,&sets);
    if (anz_graphs==0)
    {
        errmsg(tr("Please select a single graph").toAscii().constData());
        ApplyError=true;
        return;
    }
    else
    {
        gno=graphs[0];
    }
    if (anz_sets==0)
    {
        setno = NEW_SET;
    }
    else
    {
        setno=sets[0];
    }

    /*if (GetSingleListChoice(eblock_graphset_item->graph_sel, &gno)
        != RETURN_SUCCESS) {
        errmsg(tr("Please select a single graph").toAscii().constData());
        return RETURN_FAILURE;
    }*/
    /*if (GetSingleListChoice(eblock_graphset_item->set_sel, &setno) !=
        RETURN_SUCCESS) {
        // no set selected; allocate new one
        setno = NEW_SET;
    }*/
    
    nncols = settype_cols(block_curtype);
    for (i = 0; i < nncols; i++)
    {
        cs[i] = columnSelector[i]->currentValue();//-1 means "Index"
    }
    scol = selStringColumn->currentValue();
    autoscale = selAutoscale->currentValue();

    char * filen=new char[filename.length()+2];
    strcpy(filen,filename.toAscii());
    SetImportBlockData(gno,setno,filen,source,nncols, cs, scol,block_curtype,autoscale);
    new_set_no=-1;
    create_set_fromblock(gno, setno, block_curtype, nncols, cs, scol, autoscale);
    SetNewSetNo(new_set_no);
    mainWin->mainArea->completeRedraw();
    delete[] filen;
    delete[] graphs;
    delete[] sets;
    unset_wait_cursor();
}

void frmEditBlockData::doClose(void)
{
    hide();
}

frmLoadEval::frmLoadEval(QWidget * parent):QDialog(parent)
{
    QString entr[MAX_SET_COLS];
    entr[0]=QString("X =");
    entr[1]=QString("Y =");
    entr[2]=QString("Y1 =");
    entr[3]=QString("Y2 =");
    entr[4]=QString("Y3 =");
    entr[5]=QString("Y4 =");
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Load & evaluate"));
    setWindowIcon(QIcon(*GraceIcon));
    grpParameterMesh=new QGroupBox(tr("Parameter mesh ($t)"),this);
    ststst=new stdStartStop(this);
    ststst->lblStart->setText(tr("Start at:"));
    ststst->lblStop->setText(tr("Stop at:"));
    ststst->lblLength->setText(tr("Length:"));
    layout0=new QHBoxLayout;
    layout0->setMargin(STD_MARGIN);
    layout0->addWidget(ststst);
    grpParameterMesh->setLayout(layout0);
    selType=new stdSetTypeSelector(this);
    selType->setShowAll(true);
    selType->lblText->setText(tr("Set type:"));
    connect(selType->cmbSelect,SIGNAL(currentIndexChanged(int)),SLOT(typeChanged(int)));
    layout=new QGridLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(grpParameterMesh,0,0,1,6);
    layout->addWidget(selType,1,0,1,3);
    for (int i=0;i<MAX_SET_COLS;i++)
    {
        lblText[i]=new QLabel(entr[i],this);
        ledText[i]=new QLineEdit(QString(""),this);
        layout->addWidget(lblText[i],i+2,0);
        layout->addWidget(ledText[i],i+2,1,1,5);
        if (i>1)
        {
            lblText[i]->setVisible(false);
            ledText[i]->setVisible(false);
        }
    }
    ledText[0]->setText(QString("$t"));
    buttonGroup=new stdButtonGroup(this);
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));
    layout->addWidget(buttonGroup,8,0,1,6);
    setLayout(layout);
}

void frmLoadEval::typeChanged(int i)
{
    int cols=selType->getColumnCount();
    for (int i=0;i<cols;i++)
    {
        lblText[i]->setVisible(true);
        ledText[i]->setVisible(true);
    }
    for (int i=cols;i<MAX_SET_COLS;i++)
    {
        lblText[i]->setVisible(false);
        ledText[i]->setVisible(false);
    }
}

void frmLoadEval::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmLoadEval::doApply(void)
{
    ApplyError=false;
    double start=ststst->start();
    if (ststst->error==true)
    {
        errmsg(tr("Start item undefined").toAscii().constData());
        ApplyError=true;
        return;
    }
    double stop=ststst->stop();
    if (ststst->error==true)
    {
        errmsg(tr("Stop item undefined").toAscii().constData());
        ApplyError=true;
        return;
    }
    long npts=ststst->length();
    int res;
    if (ststst->error==true)
    {
        errmsg(tr("Number of points undefined").toAscii().constData());
        ApplyError=true;
        return;
    }

    char * formula[MAX_SET_COLS];
    QString dummy;
    int cols=selType->getColumnCount();
    int type=selType->currentValue();
    bool conv_error=false;//error during conversion?
    grarr *t;
    int setno, gno=get_cg();

    for (int i=0;i<cols;i++)//load formulas
    {
        dummy=ledText[i]->text();
        dummy=dummy.trimmed();
        if (dummy.isEmpty())
        {
            conv_error=true;
            formula[i]=new char[2];
        }
        else
        {
            formula[i]=new char[dummy.length()+1];
            strcpy(formula[i],ledText[i]->text().toAscii());
        }
    }
    if (conv_error==true)
    {
        errmsg(tr("Formula not recognized!").toAscii().constData());
        ApplyError=true;
        return;
    }

    t = get_parser_arr_by_name("$t");
    if (t == NULL)
    {
        t = define_parser_arr("$t");
        if (t == NULL)
        {
            errmsg(tr("Internal error").toAscii().constData());
            ApplyError=true;
            return;
        }
    }
    
    if (t->length != 0)
    {
        xfree(t->data);
        t->length = 0;
    }
    t->data = allocate_mesh(start, stop, npts);
    if (t->data == NULL)
    {
        errmsg(tr("Internal error").toAscii().constData());
        ApplyError=true;
        return;
    }
    t->length = npts;
    
    setno = nextset(gno);
    set_dataset_type(gno, setno, type);
    set_set_hidden(gno, setno, FALSE);
    if (setlength(gno, setno, npts) != RETURN_SUCCESS)
    {
        killset(gno, setno);
        XCFREE(t->data);
        t->length = 0;
        errmsg(tr("Internal error").toAscii().constData());
        ApplyError=true;
        return;
    }
    
    set_parser_setno(gno, setno);

    for (int i = 0; i < cols; i++) {
        char buf[32], *expr;
        /* preparing the expression */
        sprintf(buf, "GRAPH[%d].SET[%d].%s = ", gno, setno, dataset_colname(i));
        expr = copy_string(NULL, buf);
        expr = concat_strings(expr, formula[i]);
        /* evaluate the expression */
        res = scanner(expr);
        
        xfree(expr);
        
        if (res != RETURN_SUCCESS) {
            killset(gno, setno);
            XCFREE(t->data);
            t->length = 0;
            errmsg(tr("Internal error").toAscii().constData());
            ApplyError=true;
            return;
        }
    }
    
    XCFREE(t->data);
    t->length = 0;

    //Undo-Stuff
    int gnos[2]={gno,gno};
    int snos[2]={setno,setno};
    SetsCreated(1,gnos,snos,UNDO_COMPLETE);
    QString str;
    for (int i=0;i<cols;i++)
    {
        str+=QString(dataset_colname(i))+QString("=")+QString(formula[i]);
        if (i<cols-1)
            str+=QString("; ");
    }
    addAditionalDescriptionToLastNode(-1,QString(),str);
    mainWin->mainArea->completeRedraw();
    for (int i=0;i<cols;i++)
        delete[] formula[i];

}

void frmLoadEval::doClose(void)
{
    hide();
}

grpSelect::grpSelect(QString title,QWidget *parent):QGroupBox( parent )
{
    setFont(stdFont);
    titleString=title;
    setTitle(title);

    lblGraph=new QLabel(tr("Graph:"),this);
    listGraph=new uniList(GRAPHLIST,this);
    connect(listGraph,SIGNAL(itemSelectionChanged()),listGraph,SLOT(new_selection()));

    lblSet=new QLabel(tr("Set:"),this);
    listSet=new uniList(SETLIST,this);
    connect(listGraph,SIGNAL(new_selection(int)),this,SLOT(set_graph_nr(int)));
    listSet->partner=listGraph;
    listGraph->partner=listSet;

    layout=new QGridLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(lblGraph,0,0);
    layout->addWidget(listGraph,1,0,1,1);
    layout->addWidget(lblSet,2,0);
    layout->addWidget(listSet,3,0,3,1);
    layout->setRowStretch(1,1);
    layout->setRowStretch(3,5);
    setLayout(layout);
}

void grpSelect::set_graph_nr(int gno)
{
    listSet->set_graph_number(gno,true);
}

void grpSelect::update_number_of_entries(void)
{
    int nr,*sel=new int[2];
    listSet->get_selection(&nr,&sel);
    listGraph->update_number_of_entries_preserve_selection();
    listSet->update_number_of_entries();
    listSet->set_new_selection(nr,sel);
}

void grpSelect::mark_single_set(int gno,int setno)
{
    int dummy=gno;
    update_number_of_entries();
    listGraph->set_new_selection(1,&dummy);
    dummy=setno;
    listSet->set_new_selection(1,&setno);
}

void grpSelect::mark_multiple_sets(int gno,int nsets,int * setnos)
{
    int dummy=gno;
    update_number_of_entries();
    listGraph->set_new_selection(1,&dummy);
    listSet->set_new_selection(nsets,setnos);
}

frmEvalExpr::frmEvalExpr(MainWindow * parWind,QWidget *parent):QDialog( parent )
{
    setFont(stdFont);
    setWindowIcon(QIcon(*GraceIcon));
    parentWindow=parWind;
    setWindowTitle(tr("qtGrace: evaluateExpression"));

    grpSource=new grpSelect(tr("Source"),this);
    grpDestination=new grpSelect(tr("Destination"),this);

    lblFormula=new QLabel(tr("Formula:"),this);
    ledFormula=new QTextEdit("",this);

    fraSourceDatFilt=new QGroupBox(tr("Source data filtering"),this);
    fraSourceDatFilt->setGeometry(ledFormula->x(),ledFormula->y()+ledFormula->height(),width(),64);

    int number=7;
    QString entr[7];
    entr[0]=tr("None");
    entr[1]=tr("Region 0");
    entr[2]=tr("Region 1");
    entr[3]=tr("Region 2");
    entr[4]=tr("Region 3");
    entr[5]=tr("Region 4");
    entr[6]=tr("Inside graph");
    selRestriction=new StdSelector(fraSourceDatFilt,tr("Restriction:"),number,entr);

    chkNeg=new QCheckBox(tr("Negated"),fraSourceDatFilt);
    chkNeg->setChecked(FALSE);

    layout0=new QHBoxLayout;
    layout0->setMargin(STD_MARGIN);
    layout0->addWidget(selRestriction);
    layout0->addWidget(chkNeg);
    fraSourceDatFilt->setLayout(layout0);

    buttonGroup=new stdButtonGroup(this);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));
    layout=new QGridLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(grpSource,0,0);
    layout->addWidget(grpDestination,0,1);
    layout->addWidget(lblFormula,1,0,1,2);
    layout->addWidget(ledFormula,2,0,1,2);
    layout->addWidget(fraSourceDatFilt,3,0,1,2);
    layout->addWidget(buttonGroup,4,0,1,2);
    layout->setAlignment(lblFormula,Qt::AlignHCenter);
    setLayout(layout);
}

frmEvalExpr::~frmEvalExpr()
{

}

void frmEvalExpr::doApply(void)
{
    compute_aac();
}

void frmEvalExpr::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmEvalExpr::doClose(void)
{
    hide();
}

void frmEvalExpr::update(void)
{
    grpSource->listGraph->update_number_of_entries();
    grpSource->listSet->update_number_of_entries();
    grpDestination->listGraph->update_number_of_entries();
    grpDestination->listSet->update_number_of_entries();
}

void frmEvalExpr::init(void)
{
    update();
    grpSource->listGraph->setCurrentItem(grpSource->listGraph->item(0));
    grpDestination->listGraph->setCurrentRow(0);
    //grpSource->listGraph->setCurrentIndex(0);
}

/*
 * evaluate a formula
 */
void frmEvalExpr::compute_aac(void)
{
    int error, resno;
    int i, g1_ok, g2_ok, ns1, ns2, *svalues1=new int[2], *svalues2=new int[2],gno1, gno2, setno1, setno2;
    char fstr[256];
    char fstr2[512];
    int restr_type, restr_negate;
    char *rarray;
    int * sel_graphs=new int[2];
    char dummy[128];
    set_wait_cursor();
    ApplyError=false;
    switch (selRestriction->currentIndex())
    {
    case 0:
        restr_type=RESTRICT_NONE;
        break;
    case 1:
        restr_type=RESTRICT_REG0;
        break;
    case 2:
        restr_type=RESTRICT_REG1;
        break;
    case 3:
        restr_type=RESTRICT_REG2;
        break;
    case 4:
        restr_type=RESTRICT_REG3;
        break;
    case 5:
        restr_type=RESTRICT_REG4;
        break;
    case 6:
        restr_type=RESTRICT_WORLD;
        break;
    }
    restr_negate = chkNeg->isChecked()==TRUE?1:0;
    //get the source and destiantion graphs
    grpSource->listGraph->get_selection(&g1_ok,&sel_graphs);
    if (g1_ok==1)
    {
        gno1=sel_graphs[0];
        g1_ok=RETURN_SUCCESS;
    }
    else
        g1_ok=RETURN_FAILURE;
    grpDestination->listGraph->get_selection(&g2_ok,&sel_graphs);
    if (g2_ok==1)
    {
        gno2=sel_graphs[0];
        g2_ok=RETURN_SUCCESS;
    }
    else
        g2_ok=RETURN_FAILURE;
    //get the source and destiantion sets
    grpSource->listSet->get_selection(&ns1,&svalues1);
    grpDestination->listSet->get_selection(&ns2,&svalues2);

    error = FALSE;
    if (g1_ok == RETURN_FAILURE || g2_ok == RETURN_FAILURE) {
        error = TRUE;
        ApplyError=true;
        errmsg(tr("Please select single source and destination graphs").toAscii().constData());
    } else if (ns1 == 0) {
        error = TRUE;
        ApplyError=true;
        errmsg(tr("No source sets selected").toAscii().constData());
    } else if (ns1 != ns2 && ns2 != 0) {
        error = TRUE;
        ApplyError=true;
        errmsg(tr("Different number of source and destination sets").toAscii().constData());
    } else {
        ///Undo-Stuff: only save destination sets, because only they are modified!
        int * gnos=new int[ns2>0?ns2:ns1];
        if (ns2==0)
        {
            delete[] svalues2;
            svalues2=new int[ns1];
        }
        for (i=0;i<ns1;i++)//ns1==ns2 || ns2==0
        {
            gnos[i]=grpDestination->listSet->gr_no;
        }
        SaveSetStatesPrevious(ns2,gnos,svalues2,UNDO_DATA);//it is possible that there are no sets to be saved here!
        strcpy(fstr, ledFormula->toPlainText().toAscii());
        ListOfChanges.clear();
        ListOfOldStates.clear();
        for (i = 0; i < ns1; i++)//go through all source-sets
        {
            setno1 = svalues1[i];
            if (ns2 != 0) {//ns2==ns1
                setno2 = svalues2[i];
            } else {
                setno2 = nextset(gno2);
                //new set --> copy command needed for commands-window
                sprintf(fstr2,"COPY G%d.S%d TO G%d.S%d",gno1, setno1, gno2, setno2);
                ListOfChanges << QString(fstr2);
                set_set_hidden(gno2, setno2, FALSE);
                svalues2[i]=setno2;//because I need this for Undo-function!
            }
            resno = get_restriction_array(gno1, setno1,restr_type, restr_negate, &rarray);
            if (resno != RETURN_SUCCESS) {
                errmsg(tr("Error in evaluation restriction").toAscii().constData());
                ApplyError=true;
                break;
            }
            strcpy(fstr2,fstr);
            prependSetID(fstr2,setno2,gno2,setno1,gno1);//modify formula for use with commands-window
            /// set_parser_setno(gno1,setno1);
            resno = do_compute(gno1, setno1, gno2, setno2, rarray, fstr2);//fstr replaced by fstr2
            XCFREE(rarray);
            if (resno != RETURN_SUCCESS) {
                errmsg(tr("Error in do_compute(), check formula").toAscii().constData());
                ApplyError=true;
                break;
            }
            ListOfChanges << QString(fstr2);
        }
        if (ApplyError==false)
        {
            SetsModified(ns1,gnos,svalues2,UNDO_DATA);//the new sets
            if (ns1==1)
            {
                sprintf(dummy,"[G%d.S%d]",gnos[0],svalues2[0]);
                addAditionalDescriptionToLastNode(UNDO_TYPE_SET_MODIFIED,tr("Evaluate expression")+QString(dummy),QString(fstr),0);//usefull for adding a formula
            }
            else
            {
                addAditionalDescriptionToLastNode(UNDO_TYPE_SET_MODIFIED,tr("Evaluate expression"),QString(fstr),-1);//usefull for adding a formula
            }
            CheckActive();
        }
        delete[] gnos;
    }
    
    if (error == FALSE)
    {
        if (gno1 != gno2)
        {
            update_set_lists(gno1);
            update_set_lists(gno2);
        } else {
            update_set_lists(gno1);
        }
        mainWin->mainArea->completeRedraw();
        //grpSource->mark_multiple_sets(gno1,ns1,svalues1);//selection of source-sets should not be changed!
        grpDestination->mark_multiple_sets(gno2,ns1,svalues2);//I changed this to ns1, because svalues2 now contains the new set-ids
    }
    unset_wait_cursor();
    delete[] sel_graphs;
    delete[] svalues1;
    delete[] svalues2;
}

nlcfTabMain::nlcfTabMain(QWidget * parent):QWidget(parent)
{
    int number;
    char dummy[12];
    QString entr[12];
    ledFormula=new stdLineEdit(this,tr("Formula:"));
    ledFormula->lenText->setText(QString("y = "));
    ledTolerance=new stdLineEdit(this,tr("Tolerance:"));
    ledTolerance->lenText->setText(QString("0.01"));
    number=11;
    for (int i=0;i<number;i++)
    {
        sprintf(dummy,"%d",i);
        entr[i]=QString(dummy);
    }
    selParameters=new StdSelector(this,tr("Parameters:"),number,entr);
    connect(selParameters->cmbSelect,SIGNAL(currentIndexChanged(int)),SLOT(paraNumberChanged(int)));
    selIterations=new stdIntSelector(this,tr("Iterations:"),0,500);
    selIterations->spnInt->setSingleStep(5);
    selIterations->setValue(5);
    scroll=new QScrollArea;
    spreadParameters=new spreadSheet(scroll,1,1,3);
    scroll->setWidget(spreadParameters);

    layout=new QGridLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(ledFormula,0,0,1,3);
    layout->addWidget(selParameters,1,0);
    layout->addWidget(ledTolerance,1,1);
    layout->addWidget(selIterations,1,2);
    layout->addWidget(scroll,2,0,3,3);
    setLayout(layout);
    paraNumberChanged(0);
};

void nlcfTabMain::paraNumberChanged(int i)
{
    spreadParameters->setGeometry(0,0,width()-10,30*i);
    spreadParameters->resize(i,3);
}

nlcfTabAdvanced::nlcfTabAdvanced(QWidget * parent):QWidget(parent)
{
    int number;
    QString entr[10];

    grpSourceDataFilter=new QGroupBox(tr("Source data filtering"),this);
    number=7;
    entr[0]=tr("None");
    entr[1]=tr("Region 0");
    entr[2]=tr("Region 1");
    entr[3]=tr("Region 2");
    entr[4]=tr("Region 3");
    entr[5]=tr("Region 4");
    entr[6]=tr("Inside graph");
    selRestrictions=new StdSelector(this,tr("Restriction:"),number,entr);
    chkNegated=new QCheckBox(tr("Negated"),this);
    layout0=new QHBoxLayout;
    layout0->setMargin(STD_MARGIN);
    layout0->addWidget(selRestrictions);
    layout0->addWidget(chkNegated);
    grpSourceDataFilter->setLayout(layout0);

    grpWeighting=new QGroupBox(tr("Weighting"),this);
    number=5;
    entr[0]=tr("None");
    entr[1]=tr("1/Y");
    entr[2]=tr("1/Y^2");
    entr[3]=tr("1/dY^2");
    entr[4]=tr("Custom");
    selWeights=new StdSelector(grpWeighting,tr("Weights:"),number,entr);
    connect(selWeights->cmbSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(weights_changed(int)));
    ledFunction=new stdLineEdit(grpWeighting,tr("Function:"));
    ledFunction->setText(QString(""));
    layout1=new QHBoxLayout;
    layout1->setMargin(STD_MARGIN);
    layout1->addWidget(selWeights);
    layout1->addWidget(ledFunction);
    grpWeighting->setLayout(layout1);

    grpLoadOptions=new QGroupBox(tr("Load options"),this);
    number=3;
    entr[0]=tr("Fitted values");
    entr[1]=tr("Residuals");
    entr[2]=tr("Function");
    selLoad=new StdSelector(grpLoadOptions,tr("Load:"),number,entr);
    connect(selLoad->cmbSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(load_changed(int)));
    ledStartLoadAt=new stdLineEdit(grpLoadOptions,tr("Start load at:"));
    ledStopLoadAt=new stdLineEdit(grpLoadOptions,tr("Stop load at:"));
    ledNumberOfPoints=new stdLineEdit(grpLoadOptions,tr("# of points:"));
    layout2=new QGridLayout;
    layout2->setMargin(STD_MARGIN);
    layout2->addWidget(selLoad,0,0);
    layout2->addWidget(ledStartLoadAt,1,0);
    layout2->addWidget(ledStopLoadAt,1,1);
    layout2->addWidget(ledNumberOfPoints,1,2);
    grpLoadOptions->setLayout(layout2);

    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(grpSourceDataFilter);
    layout->addWidget(grpWeighting);
    layout->addWidget(grpLoadOptions);
    setLayout(layout);
}

void nlcfTabAdvanced::weights_changed(int i)
{
    if (i==WEIGHT_CUSTOM)
    {
        ledFunction->setEnabled(true);
    }
    else
    {
        ledFunction->setEnabled(false);
    }
}

void nlcfTabAdvanced::load_changed(int i)
{
    if (i==LOAD_FUNCTION)
    {
        ledStartLoadAt->setEnabled(true);
        ledStopLoadAt->setEnabled(true);
        ledNumberOfPoints->setEnabled(true);
    }
    else
    {
        ledStartLoadAt->setEnabled(false);
        ledStopLoadAt->setEnabled(false);
        ledNumberOfPoints->setEnabled(false);
    }
}

frmNonlinCurveFit::frmNonlinCurveFit(QWidget * parent):QDialog(parent)
{
    frmOpenFitPara=NULL;
    frmSaveFitPara=NULL;
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Non-linear curve fitting"));
    setWindowIcon(QIcon(*GraceIcon));

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
    mnuEdit->addAction(actResetFit);
    mnuEdit->addSeparator();
    mnuEdit->addAction(actLoadFit);
    mnuView=new QMenu(tr("&View"));
    mnuView->setTearOffEnabled(TRUE);
    mnuView->addAction(actAutoLoad);
    mnuView->addSeparator();
    mnuView->addAction(actUpdate);
    mnuHelp=new QMenu(tr("&Help"));
    mnuHelp->setTearOffEnabled(TRUE);
    mnuHelp->addAction(actHelpOnContext);
    mnuHelp->addSeparator();
    mnuHelp->addAction(actHelpOnFit);

    menuBar->addMenu(mnuFile);
    menuBar->addMenu(mnuEdit);
    menuBar->addMenu(mnuView);
    menuBar->addSeparator();
    menuBar->addMenu(mnuHelp);

    empty=new QWidget(this);
    grpSource=new grpSelect(tr("Source"),empty);
    grpDestination=new grpSelect(tr("Destination"),empty);

    lblAFit=new QLabel(tr("A fit"),this);
    lblAFit->setFrameStyle(QFrame::Box | QFrame::Raised);
    tabs=new QTabWidget(this);
    tabMain=new nlcfTabMain(tabs);
    tabAdvanced=new nlcfTabAdvanced(tabs);
    tabs->addTab(tabMain,tr("Main"));
    tabs->addTab(tabAdvanced,tr("Advanced"));

    buttonGroup=new stdButtonGroup(this,true,true,false);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));

    layout0=new QHBoxLayout;
    layout0->setMargin(0);
    layout0->addWidget(grpSource);
    layout0->addWidget(grpDestination);
    empty->setLayout(layout0);
    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    /*
layout->addWidget(menuBar,0,0,1,2);
layout->addWidget(grpSource,1,0);
layout->addWidget(grpDestination,1,1);
layout->addWidget(lblAFit,2,0,1,2);
layout->setAlignment(lblAFit,Qt::AlignHCenter);
layout->addWidget(tabs,3,0,1,2);
layout->addWidget(buttonGroup,4,0,1,2);
*/
    layout->addWidget(menuBar);
    layout->addWidget(empty);
    layout->addWidget(lblAFit);
    lblAFit->setAlignment(Qt::AlignHCenter);
    ///layout->setAlignment(lblAFit,Qt::AlignHCenter);
    layout->addWidget(tabs);
    layout->addWidget(buttonGroup);
    setLayout(layout);
    initialize_nonl();
}

void frmNonlinCurveFit::CreateActions(void)
{
    actClose = new QAction(tr("&Close"), this);
    //actClose->setShortcut(tr("Esc"));
    actClose->setStatusTip(tr("Close this Window"));
    connect(actClose, SIGNAL(triggered()), this, SLOT(doClose()));
    actOpen= new QAction(tr("&Open"), this);
    actOpen->setShortcut(tr("Ctrl+O"));
    connect(actOpen, SIGNAL(triggered()), this, SLOT(doOpen()));
    actSave= new QAction(tr("&Save"), this);
    actSave->setShortcut(tr("Ctrl+S"));
    connect(actSave, SIGNAL(triggered()), this, SLOT(doSave()));
    actResetFit= new QAction(tr("&Reset fit parameters"), this);
    connect(actResetFit, SIGNAL(triggered()), this, SLOT(doResetFit()));
    actLoadFit= new QAction(tr("&Load current fit"), this);
    connect(actLoadFit, SIGNAL(triggered()), this, SLOT(doLoadFit()));
    actAutoLoad= new QAction(tr("&Autoload"), this);
    actAutoLoad->setCheckable(TRUE);
    actAutoLoad->setChecked(TRUE);
    connect(actAutoLoad, SIGNAL(triggered()), this, SLOT(doAutoLoad()));
    actUpdate= new QAction(tr("&Update"), this);
    connect(actUpdate, SIGNAL(triggered()), this, SLOT(doUpdate()));
    actHelpOnContext= new QAction(tr("&On context"), this);
    actHelpOnContext->setShortcut(tr("Shift+F1"));
    connect(actHelpOnContext, SIGNAL(triggered()), this, SLOT(doHelpContext()));
    actHelpOnFit = new QAction(tr("&On fit"), this);
    connect(actHelpOnFit, SIGNAL(triggered()), this, SLOT(doHelpFit()));
}

void frmNonlinCurveFit::update_nonl_frame(void)
{
    int i;
    //XmString str = XmStringCreateLocalized(nonl_opts.title);
    //XtVaSetValues(nonl_title_item, XmNlabelString, str, NULL);
    lblAFit->setText(QString(nonl_opts.title));
    /*
 * If I define only XmALIGNMENT_CENTER (default!) then it's ignored - bug in Motif???
 */
    //XtVaSetValues(nonl_title_item, XmNalignment, XmALIGNMENT_BEGINNING, NULL);
    //XtVaSetValues(nonl_title_item, XmNalignment, XmALIGNMENT_CENTER, NULL);
    //XmStringFree(str);

    SetTextString(tabMain->ledFormula, nonl_opts.formula);
    sprintf(buf, "%g", nonl_opts.tolerance);
    xv_setstr(tabMain->ledTolerance, buf);
    SetOptionChoice(tabMain->selParameters, nonl_opts.parnum);
    tabMain->paraNumberChanged(nonl_opts.parnum);//to rescale the parameter-entries
    for (i = 0; i < MAXPARM; i++) {
        sprintf(buf, "%g", nonl_parms[i].value);
        //tabMain->spreadParameters->fitlines[i]->ledValue
        xv_setstr(tabMain->spreadParameters->fitlines[i]->ledValue, buf);
        SetToggleButtonState(tabMain->spreadParameters->fitlines[i]->chkBonds, nonl_parms[i].constr);
        sprintf(buf, "%g", nonl_parms[i].min);
        xv_setstr(tabMain->spreadParameters->fitlines[i]->ledLowerBound, buf);
        SetSensitive(tabMain->spreadParameters->fitlines[i]->ledLowerBound, nonl_parms[i].constr);
        sprintf(buf, "%g", nonl_parms[i].max);
        xv_setstr(tabMain->spreadParameters->fitlines[i]->ledHighterBound, buf);
        SetSensitive(tabMain->spreadParameters->fitlines[i]->ledHighterBound, nonl_parms[i].constr);
        /*if (i < nonl_opts.parnum) {
                if (!XtIsManaged (nonl_parm_item[i])) {
                    ManageChild(nonl_parm_item[i]);
                }
            } else {
                if (XtIsManaged (nonl_parm_item[i])) {
                    UnmanageChild(nonl_parm_item[i]);
                }
            }*/
    }

    actAutoLoad->setChecked(nonl_prefs.autoload);
    SetOptionChoice(tabAdvanced->selLoad, nonl_prefs.load);

    if (nonl_prefs.load == LOAD_FUNCTION) {
        SetSensitive(tabAdvanced->ledStartLoadAt, true);
        SetSensitive(tabAdvanced->ledStopLoadAt, true);
        SetSensitive(tabAdvanced->ledNumberOfPoints, true);
    } else {
        SetSensitive(tabAdvanced->ledStartLoadAt, false);
        SetSensitive(tabAdvanced->ledStopLoadAt, false);
        SetSensitive(tabAdvanced->ledNumberOfPoints, false);
    }

    if (GetOptionChoice(tabAdvanced->selWeights) == WEIGHT_CUSTOM) {
        SetSensitive(tabAdvanced->ledFunction, true);
    } else {
        SetSensitive(tabAdvanced->ledFunction, false);
    }

    sprintf(buf, "%g", nonl_prefs.start);
    xv_setstr(tabAdvanced->ledStartLoadAt, buf);
    sprintf(buf, "%g", nonl_prefs.stop);
    xv_setstr(tabAdvanced->ledStopLoadAt, buf);
    sprintf(buf, "%d", nonl_prefs.npoints);
    xv_setstr(tabAdvanced->ledNumberOfPoints, buf);

    grpSource->update_number_of_entries();
    grpDestination->update_number_of_entries();
}

void frmNonlinCurveFit::init(void)
{
    update_nonl_frame();
    int nr=1,sel=get_cg();
    grpSource->listGraph->set_new_selection(nr,&sel);
    grpSource->listSet->set_graph_number(sel,false);
    grpDestination->listGraph->set_new_selection(nr,&sel);
    grpDestination->listSet->set_graph_number(sel,false);

}

void frmNonlinCurveFit::doApply(void)
{
    int i;
    int nsteps;
    int src_gno, src_setno;
    int resno;
    char *fstr;
    int nlen, wlen;
    int weight_method;
    double *ytmp, *warray;
    int restr_type, restr_negate;
    char *rarray;
    ApplyError=false;
    if (GetSingleListChoice(grpSource->listGraph, &src_gno) !=
            RETURN_SUCCESS) {
        errmsg(tr("No source graph selected").toAscii().constData());
        ApplyError=true;
        return;// RETURN_FAILURE;
    }
    if (GetSingleListChoice(grpSource->listSet, &src_setno) !=
            RETURN_SUCCESS) {
        errmsg(tr("No source set selected").toAscii().constData());
        ApplyError=true;
        return;// RETURN_FAILURE;
    }

    nonl_opts.formula = copy_string(nonl_opts.formula,tabMain->ledFormula->text().toAscii());// GetTextString(nonl_formula_item));
    nsteps = (int) GetSpinChoice(tabMain->selIterations);//nonl_nsteps_item);
    nonl_opts.tolerance = atof(xv_getstr(tabMain->ledTolerance));

    nonl_opts.parnum = GetOptionChoice(tabMain->selParameters);
    for (i = 0; i < nonl_opts.parnum; i++) {
        strcpy(buf, xv_getstr(tabMain->spreadParameters->fitlines[i]->ledValue));// nonl_value_item[i]));
        if (sscanf(buf, "%lf", &nonl_parms[i].value) != 1) {
            errmsg(tr("Invalid input in parameter field").toAscii().constData());
            ApplyError=true;
            return;// RETURN_FAILURE;
        }

        nonl_parms[i].constr = GetToggleButtonState(tabMain->spreadParameters->fitlines[i]->chkBonds);// nonl_constr_item[i]);
        if (nonl_parms[i].constr) {
            strcpy(buf, xv_getstr(tabMain->spreadParameters->fitlines[i]->ledLowerBound));// nonl_lowb_item[i]));
            if (sscanf(buf, "%lf", &nonl_parms[i].min) != 1) {
                errmsg(tr("Invalid input in low-bound field").toAscii().constData());
                ApplyError=true;
                return;// RETURN_FAILURE;
            }
            strcpy(buf, xv_getstr(tabMain->spreadParameters->fitlines[i]->ledHighterBound));// nonl_uppb_item[i]));
            if (sscanf(buf, "%lf", &nonl_parms[i].max) != 1) {
                errmsg(tr("Invalid input in upper-bound field").toAscii().constData());
                ApplyError=true;
                return;// RETURN_FAILURE;
            }
            if ((nonl_parms[i].value < nonl_parms[i].min) || (nonl_parms[i].value > nonl_parms[i].max)) {
                errmsg(tr("Initial values must be within bounds").toAscii().constData());
                ApplyError=true;
                return;// RETURN_FAILURE;
            }
        }
    }//end i-loop
    ///Undo-Stuff
    SaveFitSettings();
    ListOfOldStates.clear();
    ListOfChanges.clear();
    if (nsteps) {
        /* apply weigh function */
        nlen = getsetlength(src_gno, src_setno);
        weight_method = GetOptionChoice(tabAdvanced->selWeights);// nonl_weigh_item);
        switch (weight_method) {
        case WEIGHT_Y:
        case WEIGHT_Y2:
            ytmp = getcol(src_gno, src_setno, DATA_Y);
            for (i = 0; i < nlen; i++) {
                if (ytmp[i] == 0.0) {
                    errmsg(tr("Divide by zero while calculating weights").toAscii().constData());
                    ApplyError=true;
                    return;// RETURN_FAILURE;
                }
            }
            warray = (double*) xmalloc(nlen*sizeof(double));//SIZEOF_DOUBLE);
            if (warray == NULL) {
                errmsg(tr("xmalloc failed in do_nonl_proc()").toAscii().constData());
                ApplyError=true;
                return;// RETURN_FAILURE;
            }
            for (i = 0; i < nlen; i++) {
                if (weight_method == WEIGHT_Y) {
                    warray[i] = 1/ytmp[i];
                } else {
                    warray[i] = 1/(ytmp[i]*ytmp[i]);
                }
            }
            break;
        case WEIGHT_DY:
            ytmp = getcol(src_gno, src_setno, DATA_Y1);
            if (ytmp == NULL) {
                errmsg(tr("The set doesn't have dY data column").toAscii().constData());
                ApplyError=true;
                return;// RETURN_FAILURE;
            }
            for (i = 0; i < nlen; i++) {
                if (ytmp[i] == 0.0) {
                    errmsg(tr("Divide by zero while calculating weights").toAscii().constData());
                    ApplyError=true;
                    return;// RETURN_FAILURE;
                }
            }
            warray = (double*)xmalloc(nlen*sizeof(double));//SIZEOF_DOUBLE);
            if (warray == NULL) {
                errmsg(tr("xmalloc failed in do_nonl_proc()").toAscii().constData());
                ApplyError=true;
            }
            for (i = 0; i < nlen; i++) {
                warray[i] = 1/(ytmp[i]*ytmp[i]);
            }
            break;
        case WEIGHT_CUSTOM:
            if (set_parser_setno(src_gno, src_setno) != RETURN_SUCCESS) {
                errmsg(tr("Bad set").toAscii().constData());
                ApplyError=true;
                return;// RETURN_FAILURE;
            }

            fstr = xv_getstr(tabAdvanced->ledFunction);// nonl_wfunc_item);
            if (v_scanner(fstr, &wlen, &warray) != RETURN_SUCCESS) {
                errmsg(tr("Error evaluating expression for weights").toAscii().constData());
                ApplyError=true;
                return;// RETURN_FAILURE;
            }
            if (wlen != nlen) {
                errmsg(tr("The array of weights has different length").toAscii().constData());
                xfree(warray);
                ApplyError=true;
                return;// RETURN_FAILURE;
            }
            break;
        default:
            warray = NULL;
            break;
        }
        /* apply restriction */
        //restr_type = GetOptionChoice(tabAdvanced->selRestrictions);//restr_item->r_sel);
        switch (tabAdvanced->selRestrictions->currentIndex())//special type for restrictions
        {
        case 0:
            restr_type=RESTRICT_NONE;
            break;
        case 1:
            restr_type=RESTRICT_REG0;
            break;
        case 2:
            restr_type=RESTRICT_REG1;
            break;
        case 3:
            restr_type=RESTRICT_REG2;
            break;
        case 4:
            restr_type=RESTRICT_REG3;
            break;
        case 5:
            restr_type=RESTRICT_REG4;
            break;
        case 6:
            restr_type=RESTRICT_WORLD;
            break;
        }
        restr_negate = GetToggleButtonState(tabAdvanced->chkNegated);// restr_item->negate);
        resno = get_restriction_array(src_gno, src_setno,restr_type, restr_negate, &rarray);
        if (resno != RETURN_SUCCESS) {
            errmsg(tr("Error in restriction evaluation").toAscii().constData());
            xfree(warray);
            ApplyError=true;
            return;// RETURN_FAILURE;
        }
        sprintf(dummy,"with G%d",src_gno);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"fit formula \"%s\"",nonl_opts.formula);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"fit with %d parameters",nonl_opts.parnum);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"fit prec %f",nonl_opts.tolerance);
        ListOfChanges << QString(dummy);
        for (i=0;i<nonl_opts.parnum;i++)
        {
            sprintf(dummy,"a%d = %f",i,nonl_parms[i].value);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"a%d constraints %s",i,nonl_parms[i].constr==true?"on":"off");
            ListOfChanges << QString(dummy);
            sprintf(dummy,"a%dmin = %f",i,nonl_parms[i].min);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"a%dmax = %f",i,nonl_parms[i].max);
            ListOfChanges << QString(dummy);
        }
        sprintf(dummy,"nonlfit (s%d, %d)",src_setno,nsteps);
        ListOfChanges << QString(dummy);

        /* The fit itself! */
        resno = do_nonlfit(src_gno, src_setno, warray, rarray, nsteps);
        xfree(warray);
        xfree(rarray);
        if (resno != RETURN_SUCCESS) {
            errmsg(tr("Fatal error in do_nonlfit()").toAscii().constData());
            ApplyError=true;
            return;// RETURN_FAILURE;
        }
        for (i = 0; i < nonl_opts.parnum; i++) {
            sprintf(buf, "%g", nonl_parms[i].value);
            xv_setstr(tabMain->spreadParameters->fitlines[i]->ledValue, buf);
        }
    }

    /*
 * Select & activate a set to load results to
 */
    load_nonl_fit(src_gno, src_setno, FALSE);
    ///Undo-Stuff
    SaveFitAfter();
    return;// RETURN_SUCCESS;
}

void frmNonlinCurveFit::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmNonlinCurveFit::doClose(void)
{
    hide();
}

void frmNonlinCurveFit::doOpen(void)
{
    if (frmOpenFitPara==NULL)
    {
        frmOpenFitPara=new frmIOForm(READ_FIT_PARAM,this);
        connect(frmOpenFitPara,SIGNAL(newFileSelectedForIO(int,QString,bool,bool,bool)),SLOT(IOrequested(int,QString,bool,bool,bool)));
    }
    frmOpenFitPara->init();
    frmOpenFitPara->show();
    frmOpenFitPara->raise();
}

void frmNonlinCurveFit::doSave(void)
{
    if (frmSaveFitPara==NULL)
    {
        frmSaveFitPara=new frmIOForm(WRITE_FIT_PARAM,this);
        connect(frmSaveFitPara,SIGNAL(newFileSelectedForIO(int,QString,bool,bool,bool)),SLOT(IOrequested(int,QString,bool,bool,bool)));
    }
    frmSaveFitPara->init();
    frmSaveFitPara->show();
    frmSaveFitPara->raise();
}

void frmNonlinCurveFit::doResetFit(void)
{
    reset_nonl();
    update_nonl_frame();
}

void frmNonlinCurveFit::doLoadFit(void)
{
    int src_gno, src_setno;
    if (GetSingleListChoice(grpSource->listGraph, &src_gno) !=
            RETURN_SUCCESS) {
        errmsg(tr("No source graph selected").toAscii().constData());
        return;
    }
    if (GetSingleListChoice(grpSource->listSet, &src_setno) !=
            RETURN_SUCCESS) {
        errmsg(tr("No source set selected").toAscii().constData());
        return;
    }
    load_nonl_fit(src_gno, src_setno, TRUE);
}

void frmNonlinCurveFit::doAutoLoad(void)
{}

void frmNonlinCurveFit::doUpdate(void)
{
    update_nonl_frame();
}

void frmNonlinCurveFit::doHelpContext(void)
{
    setCursor(QCursor(Qt::WhatsThisCursor));
}

void frmNonlinCurveFit::doHelpFit(void)
{
    HelpCB("doc/UsersGuide.html#non-linear-fit");
}

void frmNonlinCurveFit::load_nonl_fit(int src_gno, int src_setno, int force)
{
    int dest_gno, dest_setno;
    int i, npts = 0;
    double delx, *xfit, *y, *yfit;

    if (GetSingleListChoice(grpDestination->listGraph, &dest_gno) !=
            RETURN_SUCCESS) {
        errmsg(tr("No destination graph selected").toAscii().constData());
        return;// RETURN_FAILURE;
    }
    if (GetSingleListChoice(grpDestination->listSet, &dest_setno) !=
            RETURN_SUCCESS) {
        /* no dest sel selected; allocate new one */
        dest_setno = nextset(dest_gno);
        if (dest_setno == -1) {
            return;// RETURN_FAILURE;
        } else {
            activateset(dest_gno, dest_setno);
        }
    }

    nonl_prefs.autoload = actAutoLoad->isChecked()==true?1:0;
    nonl_prefs.load = GetOptionChoice(tabAdvanced->selLoad);

    if (nonl_prefs.load == LOAD_FUNCTION) {
        if (xv_evalexpr(tabAdvanced->ledStartLoadAt, &nonl_prefs.start) != RETURN_SUCCESS) {
            errmsg(tr("Invalid input in start field").toAscii().constData());
            return;// RETURN_FAILURE;
        }
        if (xv_evalexpr(tabAdvanced->ledStopLoadAt, &nonl_prefs.stop) != RETURN_SUCCESS) {
            errmsg(tr("Invalid input in start field").toAscii().constData());
            return;// RETURN_FAILURE;
        }
        if (xv_evalexpri(tabAdvanced->ledNumberOfPoints, &nonl_prefs.npoints) != RETURN_SUCCESS) {
            errmsg(tr("Invalid input in start field").toAscii().constData());
            return;// RETURN_FAILURE;
        }
        if (nonl_prefs.npoints <= 1) {
            errmsg(tr("Number of points must be > 1").toAscii().constData());
            return;// RETURN_FAILURE;
        }
    }

    if (force || nonl_prefs.autoload) {
        switch (nonl_prefs.load) {
        case LOAD_VALUES:
        case LOAD_RESIDUALS:
            npts = getsetlength(src_gno, src_setno);
            setlength(dest_gno, dest_setno, npts);
            copycol2(src_gno, src_setno, dest_gno, dest_setno, DATA_X);
            break;
        case LOAD_FUNCTION:
            npts  = nonl_prefs.npoints;

            setlength(dest_gno, dest_setno, npts);

            delx = (nonl_prefs.stop - nonl_prefs.start)/(npts - 1);
            xfit = getx(dest_gno, dest_setno);
            for (i = 0; i < npts; i++) {
                xfit[i] = nonl_prefs.start + i * delx;
            }
            break;
        }

        setcomment(dest_gno, dest_setno, nonl_opts.formula);

        do_compute(dest_gno, dest_setno, dest_gno, dest_setno, NULL, nonl_opts.formula);

        if (nonl_prefs.load == LOAD_RESIDUALS) { /* load residuals */
            y = gety(src_gno, src_setno);
            yfit = gety(dest_gno, dest_setno);
            for (i = 0; i < npts; i++) {
                yfit[i] -= y[i];
            }
        }
        mainWin->mainArea->completeRedraw();
        update_set_lists(dest_gno);
        grpSource->mark_single_set(src_gno, src_setno);
        grpDestination->mark_single_set(dest_gno,dest_setno);
    }

    return;// RETURN_SUCCESS;
}

void frmNonlinCurveFit::IOrequested(int type,QString file,bool exists,bool writeable,bool readable)
{
    char filename[1024];
    int res;
    FILE *pp;
    strcpy(filename,file.toAscii());
    /*//for testing -- maybe tests needed (file readable ...)
    cout << "file=" << buf << endl;
    cout << "type=" << type << endl;
    cout << "exists=" << exists << endl;
    cout << "writeable=" << writeable << endl;
    cout << "readable=" << readable << endl;*/
    if (type==WRITE_FIT_PARAM)
    {
        frmSaveFitPara->hide();
        pp = grace_openw(filename);
        if (pp != NULL)
        {
            nonl_opts.title = copy_string(nonl_opts.title, xv_getstr(frmSaveFitPara->ledTitle));
            put_fitparms(pp, 0);
            grace_close(pp);
        }
    }
    else//read fit parameters
    {
        frmOpenFitPara->hide();
        reset_nonl();
        res = getparms(filename);
        update_nonl_frame();
    }

}

frmInterpolation::frmInterpolation(int type,QWidget * parent):QDialog(parent)
{
    int number;
    char dummy[128];
    WindowType=type;
    QString * entr=new QString[maxgraph>4?maxgraph+1:5];
    setFont(stdFont);
    setWindowIcon(QIcon(*GraceIcon));
    if (type==INTERPOLATIONWINDOW)
        setWindowTitle(tr("qtGrace: Interpolation"));
    else
        setWindowTitle(tr("qtGrace: Histograms"));

    grpSource=new grpSelect(tr("Source"),this);
    grpDestination=new grpSelect(tr("Destination"),this);

    grpSampling=new QGroupBox(QString(""),this);

    sampSet=new uniList(SETLIST,grpSampling);

    chkStrict=new QCheckBox(tr("Strict (within source set bounds)"),grpSampling);
    chkCumulHist=new QCheckBox(tr("Cumulative histogram"),grpSampling);
    chkNormalize=new QCheckBox(tr("Normalize"),grpSampling);

    number=3;
    entr[0]=tr("Linear");
    entr[1]=tr("Cubic spline");
    entr[2]=tr("Akima spline");
    selMethod=new StdSelector(grpSampling,tr("Method:"),number,entr);
    number=2;
    entr[0]=tr("Linear mesh");
    entr[1]=tr("Abscissas of another set");
    selSampling=new StdSelector(grpSampling,tr("Sampling:"),number,entr);
    connect(selSampling->cmbSelect,SIGNAL(currentIndexChanged(int)),SLOT(selSamplingChanged(int)));
    number=maxgraph;
    for (int i=0;i<number;i++)
    {
        sprintf(dummy,"G%d",i);
        entr[i]=QString(dummy);
    }
    selSamplingGraph=new StdSelector(grpSampling,tr("Graph:"),number,entr);
    selSamplingGraph->setEnabled(FALSE);
    connect(selSamplingGraph->cmbSelect,SIGNAL(currentIndexChanged(int)),SLOT(selSamplGraphChanged(int)));

    lenStart=new stdLineEdit(grpSampling,tr("Start at:"));
    lenStop=new stdLineEdit(grpSampling,tr("Stop at:"));
    lenLength=new stdLineEdit(grpSampling,tr("Length:"));
    lenStart->lenText->setText(QString(""));
    lenStop->lenText->setText(QString(""));
    lenLength->lenText->setText(QString(""));

    lblSamplSet=new QLabel(tr("Sampling set:"),grpSampling);

    sampSet->setEnabled(FALSE);
    buttonGroup=new stdButtonGroup(this);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));

    layout1=new QGridLayout;
    layout1->setMargin(STD_MARGIN);

    if (type==INTERPOLATIONWINDOW)
    {
        layout1->addWidget(selMethod,0,0,1,2);
        layout1->addWidget(chkStrict,0,2,1,3);
        chkCumulHist->setVisible(FALSE);
        chkNormalize->setVisible(FALSE);
    }
    else//HISTOGRAMSWINDOW
    {
        layout1->addWidget(chkCumulHist,0,0,1,3);
        layout1->addWidget(chkNormalize,0,3,1,2);
        selMethod->setVisible(FALSE);
        chkStrict->setVisible(FALSE);
        selSampling->lblText->setText(tr("Bin sampling:"));
        lenLength->lblText->setText(tr("# of bins:"));
    }

    layout1->addWidget(selSampling,1,0,1,3);
    layout1->addWidget(selSamplingGraph,1,3,1,2);
    layout1->addWidget(lenStart,2,0,1,2);
    layout1->addWidget(lenStop,2,2,1,2);
    layout1->addWidget(lenLength,2,4);
    layout1->addWidget(lblSamplSet,3,0);
    layout1->addWidget(sampSet,4,0,3,5);
    grpSampling->setLayout(layout1);

    layout=new QGridLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(grpSource,0,0);
    layout->addWidget(grpDestination,0,1);
    layout->addWidget(grpSampling,1,0,1,2);
    layout->addWidget(buttonGroup,2,0,1,2);
    setLayout(layout);
    delete[] entr;
}

void frmInterpolation::init(void)
{
    int nr=1,sel=get_cg();
    grpSource->listGraph->update_number_of_entries();
    grpSource->listGraph->set_new_selection(nr,&sel);
    grpSource->listSet->set_graph_number(sel,false);
    //grpSource->listSet->update_number_of_entries();
    grpDestination->listGraph->update_number_of_entries();
    grpDestination->listGraph->set_new_selection(nr,&sel);
    grpDestination->listSet->set_graph_number(sel,false);
    //grpDestination->listSet->update_number_of_entries();
    sampSet->update_number_of_entries();
    selSamplingGraph->cmbSelect->clear();
    char dummy[128];
    for (int i=0;i<maxgraph;i++)
    {
        sprintf(dummy,"G%d",i);
        selSamplingGraph->cmbSelect->addItem(QString(dummy));
    }

}

void frmInterpolation::selSamplingChanged(int i)
{
    if (i==0)
    {
        sampSet->setEnabled(FALSE);
        selSamplingGraph->setEnabled(FALSE);
        lenStart->setEnabled(TRUE);
        lenStop->setEnabled(TRUE);
        lenLength->setEnabled(TRUE);
    }
    else
    {
        sampSet->setEnabled(TRUE);
        selSamplingGraph->setEnabled(TRUE);
        lenStart->setEnabled(FALSE);
        lenStop->setEnabled(FALSE);
        lenLength->setEnabled(FALSE);
    }

}

void frmInterpolation::selSamplGraphChanged(int i)
{
    sampSet->set_graph_number(i,false);
    //sampSet->update_number_of_entries();
}

void frmInterpolation::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmInterpolation::doApply(void)
{
    ApplyError=false;
    int error, res;
    int nssrc, nsdest, *svaluessrc=new int[5], *svaluesdest=new int[5], gsrc, gdest;
    int method, sampling, strict;
    int cumulative, normalize;
    int i, meshlen,nbins;
    double *bins = NULL;
    double *mesh = NULL;
    //Interp_ui *ui = (Interp_ui *) data;
    /*res = GetTransformDialogSettings(ui->tdialog, TRUE,&gsrc, &gdest,&nssrc, &svaluessrc, &nsdest, &svaluesdest);
    if (res != RETURN_SUCCESS) {
        return;// RETURN_FAILURE;
    }*/
    int count,*selection=new int[5];
    grpSource->listGraph->get_selection(&count,&selection);
    if (count!=1)
    {
        errmsg(tr("Please select single source graph").toAscii().constData());
        ApplyError=true;
        delete[] svaluessrc;
        delete[] svaluesdest;
        delete[] selection;
        return;
    }
    else
    {
        gsrc=selection[0];
    }
    grpDestination->listGraph->get_selection(&count,&selection);
    if (count!=1)
    {
        errmsg(tr("Please select single destination graph").toAscii().constData());
        ApplyError=true;
        delete[] svaluessrc;
        delete[] svaluesdest;
        delete[] selection;
        return;
    }
    else
    {
        gdest=selection[0];
    }
    grpSource->listSet->get_selection(&nssrc,&svaluessrc);
    grpDestination->listSet->get_selection(&nsdest,&svaluesdest);
    delete[] selection;
    if (nssrc!=nsdest && nsdest!=0)
    {
        errmsg(tr("Number of destination sets does not match number of source sets").toAscii().constData());
        ApplyError=true;
        delete[] svaluessrc;
        delete[] svaluesdest;
        return;
    }

    error = FALSE;

    if (WindowType==INTERPOLATIONWINDOW)
    {
        method = GetOptionChoice(selMethod);
        sampling = GetOptionChoice(selSampling);
        strict = GetToggleButtonState(chkStrict);
    }
    else
    {
        cumulative = GetToggleButtonState(chkCumulHist);
        normalize  = GetToggleButtonState(chkNormalize);
        sampling   = GetOptionChoice(selSampling);
    }

    if (sampling == SAMPLING_SET) {
        int gsampl, setnosampl;
        gsampl = selSamplingGraph->currentIndex();//get_cg();
        res = GetSingleListChoice(sampSet, &setnosampl);
        if (res != RETURN_SUCCESS) {
            errmsg(tr("Please select single sampling set").toAscii().constData());
            ApplyError=true;
            error = TRUE;
        } else {
            if (WindowType==INTERPOLATIONWINDOW)
            {
                meshlen = getsetlength(gsampl, setnosampl);
                mesh = getcol(gsampl, setnosampl, DATA_X);
            }
            else
            {
                nbins = getsetlength(gsampl, setnosampl) - 1;
                bins = getcol(gsampl, setnosampl, DATA_X);
            }
        }
    } else {
        double start, stop;
        if (xv_evalexpr(lenStart, &start)     != RETURN_SUCCESS ||
                xv_evalexpr(lenStop,  &stop)      != RETURN_SUCCESS ||
                xv_evalexpri(lenLength, &meshlen) != RETURN_SUCCESS ) {
            errmsg(tr("Can't parse mesh settings").toAscii().constData());
            ApplyError=true;
            error = TRUE;
        } else {
            if (WindowType==INTERPOLATIONWINDOW)
            {
                mesh = allocate_mesh(start, stop, meshlen);
                if (mesh == NULL) {
                    errmsg(tr("Can't allocate mesh").toAscii().constData());
                    ApplyError=true;
                    error = TRUE;
                }
            }
            else
            {
                nbins=meshlen;
                bins = allocate_mesh(start, stop, nbins + 1);
                if (bins == NULL) {
                    errmsg(tr("Can't allocate mesh").toAscii().constData());
                    ApplyError=true;
                    error = TRUE;
                }
            }
        }
    }

    if (error)
    {
        delete[] svaluessrc;
        if (nsdest > 0)
        {
            delete[] svaluesdest;
        }
        return;// RETURN_FAILURE;
    }

    int * gnos=new int[nssrc];//nssrc is either greater than nsdest or equal
    int * snos=new int[nssrc];
    char dummy[256];
    int set_no=0;

    if (nsdest>0)//no new sets will be created --> old sets will be modified
    {
        for (int i=0;i<nsdest;i++)
        {
            gnos[i]=gdest;
            snos[i]=svaluesdest[i];
        }
        SaveSetStatesPrevious(nsdest,gnos,snos,UNDO_DATA);
    }

    for (i = 0; i < nssrc; i++) {
        int setnosrc, setnodest;
        setnosrc = svaluessrc[i];
        if (nsdest != 0) {
            setnodest = svaluesdest[i];
        } else {
            setnodest = SET_SELECT_NEXT;
        }

        if (WindowType==INTERPOLATIONWINDOW)
            res = do_interp(gsrc, setnosrc, gdest, setnodest,mesh, meshlen, method, strict);
        else
            res = do_histo(gsrc, setnosrc, gdest, setnodest,bins, nbins, cumulative, normalize);

        if (new_set_no!=-1)
        {
            gnos[set_no]=gdest;
            snos[set_no]=new_set_no;
            set_no++;
        }

        if (res != RETURN_SUCCESS) {
            errmsg(tr("Error in do_interp()").toAscii().constData());
            error = TRUE;
            ApplyError=true;
            break;
        }
    }

    delete[] svaluessrc;
    if (nsdest > 0)
    {
        delete[] svaluesdest;
    }
    if (sampling == SAMPLING_MESH)
    {
        if (mesh!=NULL)
            delete[] mesh;
        if (bins!=NULL)
            delete[] bins;
    }
    if (set_no>0)
    {
        if (nsdest > 0)//old sets modified
        {
            SetsModified(set_no,gnos,snos,UNDO_DATA);
        }
        else//new sets
        {
            SetsCreated(set_no,gnos,snos,UNDO_COMPLETE);
        }
        if (set_no==1)
        {
            sprintf(dummy," [G%d.S%d]",gsrc, svaluessrc[0]);
            if (WindowType==INTERPOLATIONWINDOW)
                addAditionalDescriptionToLastNode(-1,QObject::tr("Interpolation of")+QString(dummy),QString());
            else
                addAditionalDescriptionToLastNode(-1,QObject::tr("Histogram of")+QString(dummy),QString());
        }
        else
        {
            if (WindowType==INTERPOLATIONWINDOW)
                addAditionalDescriptionToLastNode(-1,QObject::tr("Interpolation"),QString());
            else
                addAditionalDescriptionToLastNode(-1,QObject::tr("Histogram"),QString());
        }
    }
    update_set_lists(gdest);
    grpSource->listSet->update_number_of_entries_preserve_selection();
    grpDestination->listSet->update_number_of_entries_preserve_selection();
    mainWin->mainArea->completeRedraw();
}

void frmInterpolation::doClose(void)
{
    hide();
}

frmSmallCalc::frmSmallCalc(int type,QWidget * parent):QDialog(parent)
{
    int number;
    QString entr[15];
    setFont(stdFont);
    setWindowIcon(QIcon(*GraceIcon));

    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);

    WindowType=type;
    lblApplyTo=new QLabel(tr("Apply to set:"),this);
    listSets=new uniList(SETCHOICE,this);
    //listSets->show_all_sets_marker=true;
    lenText=new stdLineEdit(this,QString(""));
    lenText->lenText->setText(QString(""));
    chkInvert=new QCheckBox(tr("Invert region"),this);
    chkInvert->setVisible(FALSE);
    buttonGroup=new stdButtonGroup(this,false,true,false);
    buttonGroup->cmdAccept->setDefault(TRUE);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));
    StartStop=new stdStartStop(this);
    StartStop->setVisible(FALSE);
    layout->addWidget(lblApplyTo);
    layout->addWidget(listSets);

    switch (type)
    {
    case INTEGRATIONWINDOW:
        setWindowTitle(tr("qtGrace: Integration"));
        number=2;
        entr[0]=tr("Cumulative sum");
        entr[1]=tr("Sum only");
        selector1=new StdSelector(this,tr("Load:"),number,entr);
        layout->addWidget(selector1);
        lenText->lblText->setText(tr("Sum:"));
        layout->addWidget(lenText);
        selector2=new StdSelector(this,QString(""),number,entr);
        selector3=new StdSelector(this,QString(""),number,entr);
        selector2->setVisible(FALSE);
        selector3->setVisible(FALSE);
        break;
    case SEASONALWINDOW:
        setWindowTitle(tr("qtGrace: Seasonal differences"));
        number=1;
        entr[0]=QString("dummy");
        selector1=new StdSelector(this,QString(""),number,entr);
        selector2=new StdSelector(this,QString(""),number,entr);
        selector3=new StdSelector(this,QString(""),number,entr);
        selector1->setVisible(FALSE);
        selector2->setVisible(FALSE);
        selector3->setVisible(FALSE);
        lenText->lblText->setText(tr("Period:"));
        layout->addWidget(lenText);
        break;
    case DIFFERENCESWINDOW:
        setWindowTitle(tr("qtGrace: Differences"));
        number=3;
        entr[0]=tr("Forward difference");
        entr[1]=tr("Backward difference");
        entr[2]=tr("Centered difference");
        selector1=new StdSelector(this,tr("Method:"),number,entr);
        selector2=new StdSelector(this,QString(""),number,entr);
        selector3=new StdSelector(this,QString(""),number,entr);
        selector2->setVisible(FALSE);
        selector3->setVisible(FALSE);
        layout->addWidget(selector1);
        lenText->setVisible(FALSE);
        break;
    case AVERAGESWINDOW:
        setWindowTitle(tr("qtGrace: Running averages"));
        number=5;
        entr[0]=tr("Average");
        entr[1]=tr("Median");
        entr[2]=tr("Minimum");
        entr[3]=tr("Maximum");
        entr[4]=tr("Std. dev.");
        selector1=new StdSelector(this,tr("Running:"),number,entr);
        number=8;
        entr[0]=tr("None");
        entr[1]=tr("Region 0");
        entr[2]=tr("Region 1");
        entr[3]=tr("Region 2");
        entr[4]=tr("Region 3");
        entr[5]=tr("Region 4");
        entr[6]=tr("Inside graph");
        entr[7]=tr("Outside graph");
        selector2=new StdSelector(this,tr("Restrictions:"),number,entr);
        selector3=new StdSelector(this,QString(""),number,entr);
        selector3->setVisible(FALSE);
        chkInvert->setVisible(TRUE);
        lenText->lblText->setText(tr("Length of average:"));
        layout->addWidget(selector1);
        layout->addWidget(lenText);
        layout->addWidget(selector2);
        layout->addWidget(chkInvert);
        break;
    case REGRESSIONWINDOW:
        setWindowTitle(tr("qtGrace: Regression"));
        lenText->setVisible(FALSE);
        number=15;
        entr[0]=tr("Linear");
        entr[1]=tr("Quadratic");
        entr[2]=tr("Cubic");
        entr[3]=tr("4th degree");
        entr[4]=tr("5th degree");
        entr[5]=tr("6th degree");
        entr[6]=tr("7th degree");
        entr[7]=tr("8th degree");
        entr[8]=tr("9th degree");
        entr[9]=tr("10th degree");
        entr[10]=tr("1-10");
        entr[11]=tr("Power y=A*x^B");
        entr[12]=tr("Exponential y=A*exp(B*x)");
        entr[13]=tr("Logarithmic y=A+B*ln(x)");
        entr[14]=tr("Inverse y=1/(A+B*x)");
        selector1=new StdSelector(this,tr("Type of fit:"),number,entr);
        number=3;
        entr[0]=tr("Fitted values");
        entr[1]=tr("Residuals");
        entr[2]=tr("Function");
        selector2=new StdSelector(this,tr("Load:"),number,entr);
        connect(selector2->cmbSelect,SIGNAL(currentIndexChanged(int)),SLOT(loadChanged(int)));
        number=8;
        entr[0]=tr("None");
        entr[1]=tr("Region 0");
        entr[2]=tr("Region 1");
        entr[3]=tr("Region 2");
        entr[4]=tr("Region 3");
        entr[5]=tr("Region 4");
        entr[6]=tr("Inside graph");
        entr[7]=tr("Outside graph");
        selector3=new StdSelector(this,tr("Restrictions:"),number,entr);
        chkInvert->setVisible(TRUE);
        StartStop->setVisible(TRUE);
        StartStop->setEnabled(FALSE);
        layout->addWidget(selector1);
        layout->addWidget(selector2);
        layout->addWidget(selector3);
        layout->addWidget(chkInvert);
        layout->addWidget(StartStop);
        break;
    }
    layout->addWidget(buttonGroup);
    setLayout(layout);
}

void frmSmallCalc::loadChanged(int i)
{
    if (i==2)
        StartStop->setEnabled(TRUE);
    else
        StartStop->setEnabled(FALSE);
}

void frmSmallCalc::init(void)
{
    listSets->update_number_of_entries();
}

void frmSmallCalc::doAccept(void)
{
    int gno = get_cg();
    int *selsets=new int[5];
    int i, cnt,ideg, iresid, j, k;
    int setno, itype,period;
    int runlen, runtype, rno, invr;
    int nstep = 0, rx, rset = 0;
    double sum;
    double xstart, xstop, stepsize = 0.0, *xr;
    char buf[32];
    set_wait_cursor();
    cnt = GetSelectedSets(listSets, &selsets);
    if (cnt == SET_SELECT_ERROR)
    {
        errwin(tr("No sets selected").toAscii().constData());
        delete[] selsets;
        return;
    }
    int * gnos=new int[cnt>0?cnt:2];
    int * snos=new int[cnt>0?cnt:2];
    int new_sets=0;
    char dummy[256];
    switch (WindowType)
    {
    case INTEGRATIONWINDOW:
        itype = GetChoice(selector1);
        for (i = 0; i < cnt; i++)
        {
            setno = selsets[i];
            sum = do_int(gno, setno, itype);
            if (new_set_no!=-1)
            {
                gnos[new_sets]=gno;
                snos[new_sets]=new_set_no;
                new_sets++;
            }
            sprintf(buf, "%g", sum);
            xv_setstr(lenText, buf);
        }
        if (new_sets>0)
        {
            SetsCreated(new_sets,gnos,snos,UNDO_COMPLETE);
            if (new_sets==1)
            {
                sprintf(dummy," [G%d.S%d]",gno, selsets[0]);
                addAditionalDescriptionToLastNode(UNDO_TYPE_NEW_SET,QObject::tr("Integration of")+QString(dummy),QString());
            }
            else
            {
                addAditionalDescriptionToLastNode(UNDO_TYPE_NEW_SET,QObject::tr("Integration"),QString());
            }
        }
        break;
    case SEASONALWINDOW:
        if(xv_evalexpri(lenText, &period ) != RETURN_SUCCESS)
            return;
        for (i = 0; i < cnt; i++)
        {
            setno = selsets[i];
            do_seasonal_diff(setno, period);
            if (new_set_no!=-1)
            {
                gnos[new_sets]=gno;
                snos[new_sets]=new_set_no;
                new_sets++;
            }
        }
        if (new_sets>0)
        {
            SetsCreated(new_sets,gnos,snos,UNDO_COMPLETE);
            if (new_sets==1)
            {
                sprintf(dummy," [G%d.S%d]",gno, selsets[0]);
                addAditionalDescriptionToLastNode(UNDO_TYPE_NEW_SET,QObject::tr("Seasonal differences of")+QString(dummy),QString());
            }
            else
            {
                addAditionalDescriptionToLastNode(UNDO_TYPE_NEW_SET,QObject::tr("Seasonal differences"),QString());
            }
        }
        break;
    case DIFFERENCESWINDOW:
        itype = (int) GetChoice(selector1);
        for (i = 0; i < cnt; i++)
        {
            setno = selsets[i];
            do_differ(gno, setno, itype);
            if (new_set_no!=-1)
            {
                gnos[new_sets]=gno;
                snos[new_sets]=new_set_no;
                new_sets++;
            }
        }
        if (new_sets>0)
        {
            SetsCreated(new_sets,gnos,snos,UNDO_COMPLETE);
            if (new_sets==1)
            {
                sprintf(dummy," [G%d.S%d]",gno, selsets[0]);
                addAditionalDescriptionToLastNode(UNDO_TYPE_NEW_SET,QObject::tr("Differences of")+QString(dummy),QString());
            }
            else
            {
                addAditionalDescriptionToLastNode(UNDO_TYPE_NEW_SET,QObject::tr("Differences"),QString());
            }
        }
        break;
    case AVERAGESWINDOW:
        if (xv_evalexpri(lenText, &runlen ) != RETURN_SUCCESS) {
            return;
        }
        runtype = GetChoice(selector1);
        rno = GetChoice(selector2) - 1;
        invr = GetToggleButtonState(chkInvert);
        for (i = 0; i < cnt; i++) {
            setno = selsets[i];
            do_runavg(gno, setno, runlen, runtype, rno, invr);
            if (new_set_no!=-1)
            {
                gnos[new_sets]=gno;
                snos[new_sets]=new_set_no;
                new_sets++;
            }
        }
        if (new_sets>0)
        {
            SetsCreated(new_sets,gnos,snos,UNDO_COMPLETE);
            if (new_sets==1)
            {
                sprintf(dummy," [G%d.S%d]",gno, selsets[0]);
                addAditionalDescriptionToLastNode(UNDO_TYPE_NEW_SET,QObject::tr("Averages of")+QString(dummy),QString());
            }
            else
            {
                addAditionalDescriptionToLastNode(UNDO_TYPE_NEW_SET,QObject::tr("Averages"),QString());
            }
        }
        break;
    case REGRESSIONWINDOW:
        rno = GetChoice(selector3) - 1;
        invr = GetToggleButtonState(chkInvert);
        ideg = (int) GetChoice(selector1) + 1;

        if (ideg==11)
        {
            delete[] gnos;
            delete[] snos;
            gnos=new int[11*(cnt>0?cnt:2)];
            snos=new int[11*(cnt>0?cnt:2)];
        }

        switch(rx=GetChoice(selector2) )
        {
        case 0:				/* evaluate fitted function at original x's */
            iresid = 0;
            rset = -1;
            break;
        case 1:				/* load residue at original x points */
            iresid = 1;
            rset = -1;
            break;
        case 2:		/* evaluate fitted function at new x points */
            iresid = 0;
            if(xv_evalexpri(StartStop->ledLength, &nstep) != RETURN_SUCCESS || nstep < 2 ) {
                errwin(tr("Number points < 2").toAscii().constData());
                return;
            }
            if(xv_evalexpr(StartStop->ledStart, &xstart ) != RETURN_SUCCESS) {
                errwin(tr("Specify starting value").toAscii().constData());
                return;
            }
            if(xv_evalexpr(StartStop->ledStop, &xstop) != RETURN_SUCCESS) {
                errwin(tr("Specify stopping value").toAscii().constData());
                return;
            } else {
                stepsize = (xstop - xstart)/(nstep-1);
            }
            break;
        default:
            errwin(tr("Internal error").toAscii().constData());
            return;
        }
        new_set_no=0;
        for (i = (ideg==11?1:ideg); i <= (ideg==11?10:ideg); i++)
        {
            for (j = 0; j < cnt; j++)
            {
                setno = selsets[j];
                if( rx == 2 )
                {
                    if( (rset = nextset( gno )) == -1 )
                    {
                        errwin(tr("Not enough sets").toAscii().constData());
                        return;
                    }
                    activateset( gno, rset );
                    setlength( gno, rset, nstep);
                    xr = getx( gno, rset );
                    for( k=0; k<nstep; k++ )
                        xr[k] = xstart+k*stepsize;
                }
                do_regress(gno, setno, i, iresid, rno, invr, rset);
                if (!is_set_active(gno,new_set_no)) new_set_no=-1;
                if (new_set_no!=-1)
                {
                    gnos[new_sets]=gno;
                    snos[new_sets]=new_set_no;
                    new_sets++;
                }
            }
        }
        if (new_sets>0)
        {
            SetsCreated(new_sets,gnos,snos,UNDO_COMPLETE);
            if (new_sets==1)
            {
                sprintf(dummy," [G%d.S%d]",gno, selsets[0]);
                addAditionalDescriptionToLastNode(UNDO_TYPE_NEW_SET,QObject::tr("Regression of")+QString(dummy),QString());
            }
            else
            {
                addAditionalDescriptionToLastNode(UNDO_TYPE_NEW_SET,QObject::tr("Regression"),QString());
            }
        }
        break;
    }
    //listSets->update_number_of_entries_preserve_selection();
    update_set_lists(gno);
    unset_wait_cursor();
    mainWin->mainArea->completeRedraw();
    delete[] selsets;
    delete[] snos;
    delete[] gnos;
}

void frmSmallCalc::doClose(void)
{
    hide();
}

frmCorrelation::frmCorrelation(int type,QWidget * parent):QDialog(parent)
{
    setFont(stdFont);
    setWindowIcon(QIcon(*GraceIcon));

    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    WindowType=type;

    lblSelect1=new QLabel(QString(""),this);
    listSet1=new uniList(SETCHOICE,this);
    listSet1->setBehavior(true,false,false);
    lblSelect2=new QLabel(QString(""),this);
    listSet2=new uniList(SETCHOICE,this);
    listSet2->setBehavior(true,false,false);
    lenLag=new stdLineEdit(this,tr("Maximum lag:"));
    lenLag->lenText->setText(QString(""));
    chkCovariance=new QCheckBox(tr("Calculate covariance"),this);
    buttonGroup=new stdButtonGroup(this,false,true,false);
    buttonGroup->cmdAccept->setDefault(TRUE);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));
    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(lblSelect1);
    layout->addWidget(listSet1);
    layout->addWidget(lblSelect2);
    layout->addWidget(listSet2);
    switch (type)
    {
    case CORRELATIONWINDOW:
        setWindowTitle(tr("qtGrace: Correlation/Covariance"));
        lblSelect1->setText(tr("Select set:"));
        lblSelect2->setText(tr("Select set:"));
        layout->addWidget(lenLag);
        layout->addWidget(chkCovariance);
        break;
    case CONVOLUTIONWINDOW:
        setWindowTitle(tr("qtGrace: Linear convolution"));
        lblSelect1->setText(tr("Convolve set:"));
        lblSelect2->setText(tr("With set:"));
        chkCovariance->setVisible(FALSE);
        lenLag->setVisible(FALSE);
        break;
    case FILTERWINDOW:
        setWindowTitle(tr("qtGrace: Digital Filter"));
        lblSelect1->setText(tr("Filter set:"));
        lblSelect2->setText(tr("With weights from set:"));
        chkCovariance->setVisible(FALSE);
        lenLag->setVisible(FALSE);
        break;
    }
    layout->addWidget(buttonGroup);
    setLayout(layout);
}

void frmCorrelation::init(void)
{
    listSet1->update_number_of_entries();
    listSet2->update_number_of_entries();
}

void frmCorrelation::doAccept(void)
{
    int set1, set2, maxlag, covar;
    set_wait_cursor();
    set1 = GetSelectedSet(listSet1);
    set2 = GetSelectedSet(listSet2);
    if (set1 == SET_SELECT_ERROR || set2 == SET_SELECT_ERROR) {
        errwin(tr("Select 2 sets").toAscii().constData());
        unset_wait_cursor();
        return;
    }
    int * gnos=new int[2];
    int * snos=new int[2];
    gnos[0]=gnos[1]=get_cg();
    char dummy[256];
    switch (WindowType)
    {
    case CORRELATIONWINDOW:
        if(xv_evalexpri(lenLag, &maxlag) != RETURN_SUCCESS) {
            unset_wait_cursor();
            delete[] gnos;
            delete[] snos;
            return;
        }
        covar = GetToggleButtonState(chkCovariance);
        do_xcor(get_cg(), set1, get_cg(), set2, maxlag, covar);
        break;
    case CONVOLUTIONWINDOW:
        do_linearc(get_cg(), set1, get_cg(), set2);
        break;
    case FILTERWINDOW:
        do_digfilter(set1, set2);
        break;
    }
    snos[0]=snos[1]=new_set_no;
    if (new_set_no!=-1)
    {
        SetsCreated(1,gnos,snos,UNDO_COMPLETE);
        sprintf(dummy," [G%d S%d<->S%d]",get_cg(),set1,set2);
        addAditionalDescriptionToLastNode(UNDO_TYPE_NEW_SET,this->windowTitle().mid(9,-1)+QString(dummy),QString());
    }
    update_set_lists(get_cg());
    unset_wait_cursor();
    mainWin->mainArea->completeRedraw();
    delete[] gnos;
    delete[] snos;
}

void frmCorrelation::doClose(void)
{
    hide();
}

frmTransform::frmTransform(int type,QWidget * parent):QDialog(parent)
{
    int number;
    QString entr[6];
    setFont(stdFont);
    setWindowIcon(QIcon(*GraceIcon));
    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    WindowType=type;
    lblApplyTo=new QLabel(tr("Apply to sets:"),this);
    layout->addWidget(lblApplyTo);
    listSets=new uniList(SETCHOICE,this);
    listSets->setBehavior(false,true,true);
    layout->addWidget(listSets);

    for (int i=0;i<7;i++)
        lenText[i]=new stdLineEdit(this,QString(""));

    switch (type)
    {
    case PRUNEWINDOW:
        setWindowTitle(tr("qtGrace: Prune data"));
        number=4;
        entr[0]=tr("Interpolation");
        entr[1]=tr("Circle");
        entr[2]=tr("Ellipse");
        entr[3]=tr("Rectangle");
        selector[0]=new StdSelector(this,tr("Prune type:"),number,entr);
        connect(selector[0]->cmbSelect,SIGNAL(currentIndexChanged(int)),SLOT(PruneTypeChanged(int)));
        layout->addWidget(selector[0]);
        lenText[0]->lblText->setText(tr("Delta X:"));
        lenText[0]->lenText->setText(QString(""));
        layout->addWidget(lenText[0]);
        lenText[1]->lblText->setText(tr("Delta Y:"));
        lenText[1]->lenText->setText(QString(""));
        layout->addWidget(lenText[1]);
        for (int i=2;i<7;i++)
            lenText[i]->setVisible(FALSE);
        lenText[0]->setEnabled(FALSE);
        lenText[1]->setEnabled(TRUE);
        number=2;
        entr[0]=tr("Viewport");
        entr[1]=tr("World");
        selector[1]=new StdSelector(this,tr("Type of Delta coordinates:"),number,entr);
        connect(selector[1]->cmbSelect,SIGNAL(currentIndexChanged(int)),SLOT(CoordsChanged(int)));
        layout->addWidget(selector[1]);
        number=2;
        entr[0]=tr("Linear");
        entr[1]=tr("Logarithmic");
        selector[2]=new StdSelector(this,tr("Scaling of Delta X:"),number,entr);
        selector[3]=new StdSelector(this,tr("Scaling of Delta Y:"),number,entr);
        layout->addWidget(selector[2]);
        layout->addWidget(selector[3]);
        selector[2]->setEnabled(FALSE);
        selector[3]->setEnabled(FALSE);
        break;
    case SAMPLEPOINTSWINDOW:
        setWindowTitle(tr("qtGrace: Sample points"));
        number=2;
        entr[0]=tr("Start/step");
        entr[1]=tr("Expression");
        selector[0]=new StdSelector(this,tr("Sample type:"),number,entr);
        layout->addWidget(selector[0]);
        for (int i=1;i<4;i++)
        {
            selector[i]=new StdSelector(this,QString("dummy"),number,entr);
            selector[i]->setVisible(FALSE);
        }
        lenText[0]->lblText->setText(tr("Start:"));
        lenText[0]->lenText->setText(QString(""));
        layout->addWidget(lenText[0]);
        lenText[1]->lblText->setText(tr("Step:"));
        lenText[1]->lenText->setText(QString(""));
        layout->addWidget(lenText[1]);
        lenText[2]->lblText->setText(tr("Logical expression:"));
        lenText[2]->lenText->setText(QString(""));
        lenText[2]->setEnabled(false);
        layout->addWidget(lenText[2]);
        for (int i=3;i<7;i++)
            lenText[i]->setVisible(FALSE);
        connect(selector[0]->cmbSelect,SIGNAL(currentIndexChanged(int)),SLOT(selectorChanged(int)));
        break;
    case GEOMETRICWINDOW:
        setWindowTitle(tr("qtGrace: Geometric transformations"));
        lenText[0]->lblText->setText(tr("Rotation (degrees):"));
        lenText[1]->lblText->setText(tr("Rotate about X = :"));
        lenText[2]->lblText->setText(tr("Rotate about Y = :"));
        lenText[3]->lblText->setText(tr("Scale X:"));
        lenText[3]->lenText->setText(QString("1.0"));
        lenText[4]->lblText->setText(tr("Scale Y:"));
        lenText[4]->lenText->setText(QString("1.0"));
        lenText[5]->lblText->setText(tr("Translate X:"));
        lenText[6]->lblText->setText(tr("Translate Y:"));
        for (int i=0;i<7;i++)
            layout->addWidget(lenText[i]);
        number=6;
        entr[0]=tr("Rotate, translate, scale");
        entr[1]=tr("Rotate, scale, translate");
        entr[2]=tr("Translate, scale, rotate");
        entr[3]=tr("Translate, rotate, scale");
        entr[4]=tr("Scale, translate, rotate");
        entr[5]=tr("Scale, rotate, translate");
        selector[0]=new StdSelector(this,tr("Apply in order:"),number,entr);
        for (int i=1;i<4;i++)
        {
            selector[i]=new StdSelector(this,QString("dummy"),number,entr);
            selector[i]->setVisible(FALSE);
        }
        layout->addWidget(selector[0]);
        break;
    }
    if (type!=GEOMETRICWINDOW)
    {
        buttonGroup=new stdButtonGroup(this,false,true,false);
        buttonGroup->cmdAccept->setDefault(TRUE);
        connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
        connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));
    }
    else
    {
        buttonGroup=new stdButtonGroup(this,true,true,false);
        buttonGroup->cmdAccept->setText(tr("Reset"));
        connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doAccept()));
        connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doReset()));
        connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));
    }
    layout->addWidget(buttonGroup);
    setLayout(layout);
}

void frmTransform::CoordsChanged(int i)
{
    if (i==0)//Viewport
    {
        selector[2]->setEnabled(FALSE);
        selector[3]->setEnabled(FALSE);
    }
    else//World
    {
        switch (selector[0]->currentIndex())
        {
        case 0:
            selector[2]->setEnabled(FALSE);
            selector[3]->setEnabled(TRUE);
            break;
        case 1:
            selector[2]->setEnabled(TRUE);
            selector[3]->setEnabled(FALSE);
            break;
        case 2:
        case 3:
            selector[2]->setEnabled(TRUE);
            selector[3]->setEnabled(TRUE);
            break;
        }
    }
}

void frmTransform::PruneTypeChanged(int i)
{
    switch (i)
    {
    case 0:
        lenText[0]->setEnabled(FALSE);
        lenText[1]->setEnabled(TRUE);
        break;
    case 1:
        lenText[0]->setEnabled(TRUE);
        lenText[1]->setEnabled(FALSE);
        break;
    case 2:
    case 3:
        lenText[0]->setEnabled(TRUE);
        lenText[1]->setEnabled(TRUE);
        break;
    }
    CoordsChanged(selector[1]->currentIndex());
}

void frmTransform::init(void)
{
    listSets->update_number_of_entries();
}

void frmTransform::doAccept(void)
{
    int i, j, k, cnt, order[3], ord;
    double degrees, sx, sy, rotx, roty, tx, ty, xtmp, ytmp, *x, *y;
    double cosd, sind;
    int *selsets=new int[5];
    int setno, typeno, deltatypeno;
    int dxtype, dytype;
    double deltax, deltay;
    char *exprstr;
    int startno, stepno;
    set_wait_cursor();
    cnt = GetSelectedSets(listSets, &selsets);
    if (cnt<=0){// == SET_SELECT_ERROR) {
        errwin(tr("No sets selected").toAscii().constData());
        unset_wait_cursor();
        return;
    }
    int * gnos=new int[cnt>0?cnt:2];
    int * snos=new int[cnt>0?cnt:2];
    int set_no=0;
    char dummy[256];
    switch (WindowType)
    {
    case PRUNEWINDOW:
        typeno = (int) GetChoice(selector[0]);
        deltatypeno = (int) GetChoice(selector[1]);
        dxtype = (int) GetChoice(selector[2]);
        dytype = (int) GetChoice(selector[3]);
        if(lenText[0]->isEnabled() == true ){
            if(xv_evalexpr(lenText[0], &deltax) != RETURN_SUCCESS)
            {
                errmsg(tr("Invalid Delta X!").toAscii().constData());
                unset_wait_cursor();
                return;
            }
        } else
            deltax = 0;
        if( lenText[1]->isEnabled() == true ){
            if(xv_evalexpr(lenText[1], &deltay) != RETURN_SUCCESS )
            {
                errmsg(tr("Invalid Delta Y!").toAscii().constData());
                unset_wait_cursor();
                return;
            }
        } else
            deltay = 0;
        for (i = 0; i < cnt; i++)
        {
            setno = selsets[i];
            do_prune(setno, typeno, deltatypeno, deltax, deltay, dxtype, dytype);
            if (new_set_no!=-1)
            {
                gnos[set_no]=get_cg();
                snos[set_no]=new_set_no;
                set_no++;
            }
        }
        update_set_lists(get_cg());
        if (set_no>0)
        {
            SetsCreated(set_no,gnos,snos,UNDO_COMPLETE);
            if (set_no==1)
            {
                sprintf(dummy," [G%d.S%d]",get_cg(), selsets[0]);
                addAditionalDescriptionToLastNode(UNDO_TYPE_NEW_SET,QObject::tr("Prune data")+QString(dummy),QString());
            }
            else
            {
                addAditionalDescriptionToLastNode(UNDO_TYPE_NEW_SET,QObject::tr("Prune data"),QString());
            }
        }
        break;
    case SAMPLEPOINTSWINDOW:
        typeno = GetChoice(selector[0]);
        if (typeno == 0) {
            exprstr = "";
            if (xv_evalexpri(lenText[0], &startno) != RETURN_SUCCESS ||
                    xv_evalexpri(lenText[1], &stepno)   != RETURN_SUCCESS) {
                errmsg(tr("Please select start and step values").toAscii().constData());
                unset_wait_cursor();
                return;
            }
        } else {
            exprstr = xv_getstr(lenText[2]);
            startno = stepno = 1;
        }
        for (i = 0; i < cnt; i++) {
            setno = selsets[i];
            do_sample(setno, typeno, exprstr, startno, stepno);
            if (new_set_no!=-1 && is_set_active(get_cg(),new_set_no))
            {
                gnos[set_no]=get_cg();
                snos[set_no]=new_set_no;
                set_no++;
            }
        }
        update_set_lists(get_cg());
        if (set_no>0)
        {
            SetsCreated(set_no,gnos,snos,UNDO_COMPLETE);
            if (set_no==1)
            {
                sprintf(dummy," [G%d.S%d]",get_cg(), selsets[0]);
                addAditionalDescriptionToLastNode(UNDO_TYPE_NEW_SET,QObject::tr("Sample points")+QString(dummy),QString());
            }
            else
            {
                addAditionalDescriptionToLastNode(UNDO_TYPE_NEW_SET,QObject::tr("Sample points"),QString());
            }
        }
        break;
    case GEOMETRICWINDOW:
        ord = (int) GetChoice(selector[0]);
        switch (ord) {
        case 0:
            order[0] = 0;		/* rotate */
            order[1] = 1;		/* translate */
            order[2] = 2;		/* scale */
            break;
        case 1:
            order[0] = 0;
            order[1] = 2;
            order[2] = 1;
            break;
        case 2:
            order[0] = 1;
            order[1] = 2;
            order[2] = 0;
            break;
        case 3:
            order[0] = 1;
            order[1] = 0;
            order[2] = 2;
            break;
        case 4:
            order[0] = 2;
            order[1] = 1;
            order[2] = 0;
            break;
        case 5:
            order[0] = 2;
            order[1] = 0;
            order[2] = 1;
            break;
        }
        /* check input fields */
        if (xv_evalexpr(lenText[0], &degrees) != RETURN_SUCCESS ||
                xv_evalexpr(lenText[1], &rotx)    != RETURN_SUCCESS ||
                xv_evalexpr(lenText[2], &roty)    != RETURN_SUCCESS ||
                xv_evalexpr(lenText[5], &tx)      != RETURN_SUCCESS ||
                xv_evalexpr(lenText[6], &ty)      != RETURN_SUCCESS ||
                xv_evalexpr(lenText[3], &sx)      != RETURN_SUCCESS ||
                xv_evalexpr(lenText[4], &sy)      != RETURN_SUCCESS )
            return;
        degrees = M_PI / 180.0 * degrees;
        cosd = cos(degrees);
        sind = sin(degrees);

        for (i=0;i<cnt;i++)
            gnos[i]=get_cg();

        SaveSetStatesPrevious(cnt,gnos,selsets,UNDO_DATA);
        for (k = 0; k < cnt; k++) {
            setno = selsets[k];
            if (is_set_active(get_cg(), setno)) {
                x = getx(get_cg(), setno);
                y = gety(get_cg(), setno);
                for (j = 0; j < 3; j++) {
                    switch (order[j]) {
                    case 0:			/* rotate */
                        if (degrees == 0.0) {
                            break;
                        }
                        for (i = 0; i < getsetlength(get_cg(), setno); i++) {
                            xtmp = x[i] - rotx;
                            ytmp = y[i] - roty;
                            x[i] = rotx + cosd * xtmp - sind * ytmp;
                            y[i] = roty + sind * xtmp + cosd * ytmp;
                        }
                        break;
                    case 1:			/* translate */
                        for (i = 0; i < getsetlength(get_cg(), setno); i++) {
                            x[i] += tx;
                            y[i] += ty;
                        }
                        break;
                    case 2:					/* scale */
                        for (i = 0; i < getsetlength(get_cg(), setno); i++) {
                            x[i] *= sx;
                            y[i] *= sy;
                        }
                        break;
                    }		/* end case */
                }			/* end for j */
                update_set_lists(get_cg());
            }			/* end if */
        }				/* end for k */
        if (cnt>0)
        {
            SetsModified(cnt,gnos,selsets,UNDO_DATA);
            if (cnt==1)
            {
                sprintf(dummy," [G%d.S%d]",get_cg(), selsets[0]);
                addAditionalDescriptionToLastNode(UNDO_TYPE_NEW_SET,QObject::tr("Geometric transformation of")+QString(dummy),QString());
            }
            else
            {
                addAditionalDescriptionToLastNode(UNDO_TYPE_NEW_SET,QObject::tr("Geometric transformation"),QString());
            }
        }
        set_dirtystate();
        break;
    }
    delete[] selsets;
    listSets->update_number_of_entries_preserve_selection();
    unset_wait_cursor();
    mainWin->mainArea->completeRedraw();
    delete[] gnos;
    delete[] snos;
}

void frmTransform::doReset(void)
{
    xv_setstr(lenText[0], "0.0");
    xv_setstr(lenText[1], "0.0");
    xv_setstr(lenText[2], "0.0");
    xv_setstr(lenText[3], "1.0");
    xv_setstr(lenText[4], "1.0");
    xv_setstr(lenText[5], "0.0");
    xv_setstr(lenText[6], "0.0");
}

void frmTransform::doClose(void)
{
    hide();
}

void frmTransform::selectorChanged(int i)
{
    if (WindowType==SAMPLEPOINTSWINDOW)
    {
        if (i==0)//start/stop
        {
            lenText[0]->setEnabled(true);
            lenText[1]->setEnabled(true);
            lenText[2]->setEnabled(false);
        }
        else//expression
        {
            lenText[0]->setEnabled(false);
            lenText[1]->setEnabled(false);
            lenText[2]->setEnabled(true);
        }
    }
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
    layout0->setMargin(STD_MARGIN);
    layout0->addWidget(cmbType);
    fraSetPres->setLayout(layout0);

    fraSymbProp=new QGroupBox(tr("Symbol properties"),this);
    layout1=new QVBoxLayout;
    layout1->setMargin(STD_MARGIN);
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
    sldSymbSize=new stdSlider(fraSymbProp,tr("Size"),0,1000);
    layout1->addWidget(sldSymbSize);
    cmbSymbColor=new ColorSelector(fraSymbProp);
    layout1->addWidget(cmbSymbColor);
    connect(cmbSymbColor,SIGNAL(currentIndexChanged(int)),SLOT(SymbColorChanged(int)));

    ledSymbChar=new stdLineEdit(fraSymbProp,tr("Symbol char:"));
    ledSymbChar->lenText->setText(QString(""));
    layout1->addWidget(ledSymbChar);
    fraSymbProp->setLayout(layout1);

    fraLineProp=new QGroupBox(tr("Line properties"),this);
    layout2=new QVBoxLayout;
    layout2->setMargin(STD_MARGIN);
    entr[0]=tr("None");
    entr[1]=tr("Straight");
    entr[2]=tr("Left stairs");
    entr[3]=tr("Right stairs");
    entr[4]=tr("Segments");
    entr[5]=tr("3-Segments");
    number=6;
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
    layout3->setMargin(STD_MARGIN);
    ledString=new stdLineEdit(fraLegend,tr("String:"),true);
    ledString->lenText->setText(QString(""));
    layout3->addWidget(ledString);
    fraLegend->setLayout(layout3);

    fraDispOpt=new QGroupBox(tr("Display options"),this);
    layout4=new QHBoxLayout;
    layout4->setMargin(STD_MARGIN);
    chkAnnVal=new QCheckBox(tr("Annotate values"),fraDispOpt);
    layout4->addWidget(chkAnnVal);
    chkDispErrBars=new QCheckBox(tr("Display error bars"),fraDispOpt);
    layout4->addWidget(chkDispErrBars);
    fraDispOpt->setLayout(layout4);
    layout=new QGridLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(fraSetPres,0,0,1,2);
    layout->addWidget(fraSymbProp,1,0);
    layout->addWidget(fraLineProp,1,1);
    layout->addWidget(fraLegend,2,0,1,2);
    layout->addWidget(fraDispOpt,3,0,1,2);
    setLayout(layout);
}

tabMain::~tabMain()
{
    delete[] Type_entries;
}

void tabMain::LineColorChanged(int val)
{
    emit(colorChanged(val));
}

void tabMain::SymbColorChanged(int val)
{
    emit(colorChanged2(val));
}

tabSymbol::tabSymbol(QWidget * parent):QWidget(parent)
{
    fraSymbOutl=new QGroupBox(tr("Symbol outline"),this);
    layout0=new QGridLayout;
    layout0->setMargin(STD_MARGIN);
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
    layout1->setMargin(STD_MARGIN);
    cmbFillColor=new ColorSelector(this);
    cmbFillColor->lblText->setText(tr("Color:"));
    layout1->addWidget(cmbFillColor);
    cmbFillPattern=new FillPatternSelector(this);
    cmbFillPattern->lblText->setText(tr("Pattern:"));
    layout1->addWidget(cmbFillPattern);
    fraSymbFill->setLayout(layout1);
    fraExtra=new QGroupBox(tr("Extra"),this);
    layout2=new QVBoxLayout;
    layout2->setMargin(STD_MARGIN);
    spnSymbSkip=new stdIntSelector(this,tr("Symbol skip:"),0,100000);
    layout2->addWidget(spnSymbSkip);
    cmbSymbFont=new FontSelector(this);
    cmbSymbFont->setLabelText(tr("Font for char symbol:"));
    //cmbSymbFont->lblText->setText(tr("Font for char symbol:"));
    layout2->addWidget(cmbSymbFont);
    fraExtra->setLayout(layout2);
    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
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
    layout0->setMargin(STD_MARGIN);
    cmbPattern=new FillPatternSelector(this);
    cmbPattern->lblText->setText(tr("Pattern:"));
    layout0->addWidget(cmbPattern);
    chkDrawDropLines=new QCheckBox(tr("Draw drop lines"),fraLineProp);
    layout0->addWidget(chkDrawDropLines);
    fraLineProp->setLayout(layout0);
    fraFillProp=new QGroupBox(tr("Fill properties"),this);
    layout1=new QGridLayout;
    layout1->setMargin(STD_MARGIN);
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
    layout2->setMargin(STD_MARGIN);
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
    layout->setMargin(STD_MARGIN);
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
    QString entr[33];
    char dummy[500];
    fraTextProp=new QGroupBox(tr("Text properties"),this);
    layout0=new QGridLayout;
    layout0->setMargin(STD_MARGIN);
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
    fraTextProp->setLayout(layout0);
    fraFormatOpt=new QGroupBox(tr("Format options"),this);
    layout1=new QGridLayout;
    layout1->setMargin(STD_MARGIN);
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
    for (int i=0;i<32;i++)
        entr[i]=QString(fmt_option_items[i].label);
    number=32;
    cmbFormat=new StdSelector(this,tr("Format:"),number,entr);
    layout1->addWidget(cmbFormat,1,0,1,2);
    fraFormatOpt->setLayout(layout1);
    fraPlacement=new QGroupBox(tr("Placement"),this);
    layout2=new QHBoxLayout;
    layout2->setMargin(STD_MARGIN);
    ledXOffs=new stdLineEdit(this,tr("X offset:"));
    ledXOffs->lenText->setText(QString(""));
    layout2->addWidget(ledXOffs);
    ledYOffs=new stdLineEdit(this,tr("Y offset:"));
    ledYOffs->lenText->setText(QString(""));
    layout2->addWidget(ledYOffs);
    fraPlacement->setLayout(layout2);
    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
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
    layout0->setMargin(STD_MARGIN);
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
    fraCommon->setLayout(layout0);
    fraClipping=new QGroupBox(tr("Clipping"),this);
    layout1=new QVBoxLayout;
    layout1->setMargin(STD_MARGIN);
    chkArrowClip=new QCheckBox(tr("Arrow clip"),fraClipping);
    layout1->addWidget(chkArrowClip);
    spnMaxLength=new LineWidthSelector(this);
    spnMaxLength->lblText->setText(tr("Max length:"));
    layout1->addWidget(spnMaxLength);
    fraClipping->setLayout(layout1);
    fraBarLine=new QGroupBox(tr("Bar line"),this);
    layout2=new QVBoxLayout;
    layout2->setMargin(STD_MARGIN);
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
    layout3->setMargin(STD_MARGIN);
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
    layout->setMargin(STD_MARGIN);
    layout->addWidget(fraCommon,0,0);
    layout->addWidget(fraClipping,1,0);
    layout->addWidget(fraBarLine,0,1);
    layout->addWidget(fraRiserLine,1,1);
    layout->addWidget(empty,2,0,1,2);
    setLayout(layout);
}

frmSet_Appearance::frmSet_Appearance(QWidget * parent):QWidget(parent)
{
    //setFont(stdFont);
    //setWindowTitle(tr("qtGrace: Set Appearance"));
    //setWindowIcon(QIcon(*GraceIcon));
    CreateActions();
    updating=false;
    cset=0;

    menuBar=new QMenuBar(this);

    mnuFile=new QMenu(tr("&File"),this);
    mnuFile->setTearOffEnabled(TRUE);
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
    connect(tabMa->cmbLineStyle,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabMa->cmbLineType,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabMa->cmbSymbColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabMa->cmbSymbType,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabMa->ledString,SIGNAL(changed()),SLOT(update0()));
    connect(tabMa->sldSymbSize,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(tabMa->spnLineWidth,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    //tabSymbol
    connect(tabSy->cmbFillColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabSy->cmbFillPattern,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabSy->cmbSymbFont,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabSy->cmbSymbPattern,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabSy->cmbSymbStyle,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabSy->spnSymbSkip,SIGNAL(currentValueChanged(int)),SLOT(update1(int)));
    connect(tabSy->spnSymbWidth,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    //tabLine
    connect(tabLi->cmbBaseType,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLi->cmbFillColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLi->cmbFillPattern,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLi->cmbPattern,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLi->cmbRule,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLi->cmbType,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLi->chkDrawDropLines,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(tabLi->chkDrawLine,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(tabLi->cmbSet->cmb,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    //tabtabAnnVal
    connect(tabAnVa->cmbColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
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
    //tabErrorBars
    connect(tabErBa->cmbBarStyle,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabErBa->cmbColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabErBa->cmbPattern,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabErBa->cmbPlacement,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabErBa->cmbRiserStyle,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabErBa->chkArrowClip,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(tabErBa->spnbarWidth,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(tabErBa->spnMaxLength,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(tabErBa->spnRiserWidth,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(tabErBa->sldBarSize,SIGNAL(valueChanged(int)),SLOT(update1(int)));

    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(menuBar);
    layout->addWidget(lblSelSet);
    layout->addWidget(listSet);
    layout->addWidget(tabs);
    layout->addWidget(buttonGroup);
    setLayout(layout);
}

frmSet_Appearance::~frmSet_Appearance()
{
}

void frmSet_Appearance::update0(void)
{
    static int i,nr,errpos;
    bool sav_imm_iupd;
    if (!immediateUpdate) return;
    GlobalInhibitor=true;
    sav_imm_iupd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    doApply();//do this without actually doing something
    nr=nr_of_true_changes(ListOfChanges);
    errpos=0;
    if (nr>0)
    {
        for (i=0;i<ListOfChanges.size();i++)
        {
            strcpy(command,ListOfChanges.at(i).toAscii().constData());
            errpos = scanner(command);
        }
        set_dirtystate();
        mainWin->mainArea->completeRedraw();
        //undo-Stuff
        SetsModified(nr_of_set_concerned,sets_concerned_g,sets_concerned_s,UNDO_APPEARANCE);
    }
    ListOfChanges.clear();
    ListOfOldStates.clear();
    GlobalInhibitor=false;
    immediateUpdate=sav_imm_iupd;
    updateRunning=false;
}

void frmSet_Appearance::update1(int v)
{
    update0();
}

void frmSet_Appearance::update2(QString v)
{
    update0();
}

void frmSet_Appearance::update3(bool v)
{
    update0();
}

void frmSet_Appearance::update4(double v)
{
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
    acthelponcontext= new QAction(tr("On onte&xt"), this);
    acthelponcontext->setShortcut(tr("Crtl+F1"));
    connect(acthelponcontext, SIGNAL(triggered()), this, SLOT(doHelpOnContext()));
    acthelponsetappearance= new QAction(tr("On set appearance"), this);
    connect(acthelponsetappearance, SIGNAL(triggered()), this, SLOT(doHelpOnSetAppearance()));
    actdupllegends= new QAction(tr("&Duplicate legends"), this);
    actdupllegends->setCheckable(TRUE);
    actdupllegends->setChecked(FALSE);
    connect(actdupllegends, SIGNAL(triggered()), this, SLOT(doDuplLegends()));
    actcolorsync= new QAction(tr("Color &sync"), this);
    actcolorsync->setCheckable(TRUE);
    actcolorsync->setChecked(TRUE);
    connect(actcolorsync, SIGNAL(triggered()), this, SLOT(doColorSync()));
    actsetdiffcolors= new QAction(tr("Set different &colors"), this);
    connect(actsetdiffcolors, SIGNAL(triggered()), this, SLOT(doSetDiffColors()));
    actsetdifflinestyles= new QAction(tr("Set different line st&yles"), this);
    connect(actsetdifflinestyles, SIGNAL(triggered()), this, SLOT(doSetDiffLineStyles()));
    actsetdifflinewidths= new QAction(tr("Set different line &widths"), this);
    connect(actsetdifflinewidths, SIGNAL(triggered()), this, SLOT(doSetDiffLineWidths()));
    actsetdiffsymbols= new QAction(tr("Set different &symbols"), this);
    connect(actsetdiffsymbols, SIGNAL(triggered()), this, SLOT(doSetDiffSymbols()));
    actsetbaw= new QAction(tr("Set black & white"), this);
    connect(actsetbaw, SIGNAL(triggered()), this, SLOT(doSetBlackAndWhite()));
    actloadcoments= new QAction(tr("Load co&mments"), this);
    connect(actloadcoments, SIGNAL(triggered()), this, SLOT(doLoadComments()));
    actstriplegends= new QAction(tr("Strip &legends"), this);
    connect(actstriplegends, SIGNAL(triggered()), this, SLOT(doStripLegends()));
}

void frmSet_Appearance::doApply(void)
{
    char dummy[MAX_STRING_LENGTH];
    char dummy2[MAX_STRING_LENGTH];

    ApplyError=false;
    int i;
    int duplegs;
    int type;
    int sym, symskip, symlines;
    double symlinew;
    int line, linet, color, pattern;
    double wid;
    int dropline, filltype, fillrule, fillpat, fillcol, polybase;
    int symcolor, sympattern, symfillcolor, symfillpattern;
    double symsize;
    int baseline, baselinetype;
    Errbar errbar;
    AValue avalue;
    char symchar;
    int charfont;
    plotarr p;
    
    int setno,grano;
    int *selset=new int[2], cd;
    listSet->get_selection(&cd,&selset);//you get the actual set-id (G0.S2 --> 2), not the entry number in the list (e.g. 0 if S0 and S1 have been deleted)
    if (cd < 1)
    {
        errmsg(tr("No set selected").toAscii().constData());
        ApplyError=true;
        delete[] selset;
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

    duplegs=actdupllegends->isChecked()==TRUE?1:0;

    type = tabMa->Type_entries[tabMa->cmbType->currentIndex()];
    symsize =tabMa->sldSymbSize->value()/100.0;
    sym = tabMa->cmbSymbType->currentIndex();
    color = tabMa->cmbLineColor->currentIndex();

    pattern = tabLi->cmbPattern->currentIndex();
    wid = tabMa->spnLineWidth->value();
    baseline = tabLi->chkDrawLine->isChecked()?1:0;
    baselinetype = tabLi->cmbBaseType->currentIndex();
    dropline = tabLi->chkDrawDropLines->isChecked()==TRUE?1:0;
    line = tabMa->cmbLineStyle->currentIndex();
    linet = tabMa->cmbLineType->currentIndex();

    filltype = tabLi->cmbType->currentIndex();
    fillrule = tabLi->cmbRule->currentIndex();
    fillpat = tabLi->cmbFillPattern->currentIndex();
    fillcol = tabLi->cmbFillColor->currentIndex();
    polybase = tabLi->cmbSet->value();
    symskip = tabSy->spnSymbSkip->value();
    symcolor = tabMa->cmbSymbColor->currentIndex();
    sympattern = tabSy->cmbSymbPattern->currentIndex();
    symfillcolor = tabSy->cmbFillColor->currentIndex();
    symfillpattern = tabSy->cmbFillPattern->currentIndex();
    symlinew = tabSy->spnSymbWidth->value();
    symlines = tabSy->cmbSymbStyle->currentIndex();
    symchar = atoi(tabMa->ledSymbChar->text().toAscii());
    charfont = tabSy->cmbSymbFont->currentIndex();

    errbar.active = tabMa->chkDispErrBars->isChecked()?1:0;
    avalue.active = tabMa->chkAnnVal->isChecked()?1:0;

    avalue.font = tabAnVa->cmbFont->currentIndex();
    avalue.size = tabAnVa->sldFontSize->value()/100.0;
    avalue.color = tabAnVa->cmbColor->currentIndex();
    avalue.angle = tabAnVa->sldFontAngle->value();
    avalue.format = tabAnVa->cmbFormat->currentIndex();
    avalue.type = tabAnVa->cmbType->currentIndex();
    avalue.prec = tabAnVa->cmbPrecision->currentIndex();

    tabAnVa->ledPrepend->SetMemoryToText(avalue.prestr,avalue.orig_prestr);
    tabAnVa->ledAppend->SetMemoryToText(avalue.appstr,avalue.orig_appstr);
    /*strcpy(avalue.prestr, tabAnVa->ledPrepend->text().toAscii());
    strcpy(avalue.appstr, tabAnVa->ledAppend->text().toAscii());*/
    ///setting strings

    /*avalue.offset.x=atof(tabAnVa->ledXOffs->text().toAscii());
avalue.offset.y=atof(tabAnVa->ledYOffs->text().toAscii());*/
    xv_evalexpr(tabAnVa->ledXOffs,&avalue.offset.x);
    xv_evalexpr(tabAnVa->ledYOffs,&avalue.offset.y);

    errbar.pen.color = tabErBa->cmbColor->currentIndex();
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
    errbar.pen.pattern = tabErBa->cmbPattern->currentIndex();
    errbar.arrow_clip = tabErBa->chkArrowClip->isChecked()==TRUE?1:0;
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
        errmsg(tr("No set selected").toAscii().constData());
        ApplyError=true;
    } else {
        //compare changes with previous states
        setno = -111;//selset[0];//first set selected --> NO, WE USE AN INVALID NUMBER HERE
        get_graph_plotarr(grano, selset[0], &p);//to obtain the previous settings
        if (dataset_type(grano, selset[0]) != type)
        {
            sprintf(dummy,"    s%d type %s",setno,set_types(type));
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d type %s",setno,set_types(dataset_type(grano, setno)));
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
            sprintf(dummy,"    s%d fill pattern %d",setno,pattern);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    s%d fill pattern %d",setno,p.linepen.pattern);
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
        strcpy(dummy2,tabMa->ledString->text().toAscii());
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
        /*if (p.polygone_base_set!=polybase)///Because there is no command I used a different one
        {
            cout << "polybase" << endl;
        sprintf(dummy,"    s%d fill color %d",setno,fillcol);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    s%d fill color %d",setno,p.setfillpen.color);
        ListOfOldStates << QString(dummy);
        }*/
        //end comparison
        /*for (i=0;i<ListOfChanges.size();i++)
cout << ListOfChanges.at(i).toAscii().constData() << endl;*/
        QStringList help_list1=ListOfChanges;
        QStringList help_list2=help_list1;
        ListOfChanges.clear();
        for (i = 0; i < cd; i++)
        {
            sprintf(dummy,"s%d",selset[i]);
            help_list2.replaceInStrings("s-111",dummy);//-111 does not exist --> I use this to replace it be different set-ids
            ListOfChanges << help_list2;
            help_list2=help_list1;
        }

        for(i = 0; i < cd; i++)
            g[grano].p[selset[i]].avalue=avalue;//necessary

        if (GlobalInhibitor==false)//if GlobalInhibitor==true --> do nothing for real
        {
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
                if (cd == 1 || duplegs)
                {
                    //strcpy(p.lstr,tabMa->ledString->text().toAscii());
                    tabMa->ledString->SetMemoryToText(p.lstr,p.orig_lstr);
                    ///setting a string
                }
                p.sym = sym;
                p.linet = linet;
                p.lines = line;
                p.linew = wid;
                p.linepen.color = color;
                p.linepen.pattern = pattern;
                p.sympen.color = symcolor;
                p.sympen.pattern = sympattern;
                p.symfillpen.color = symfillcolor;
                p.symfillpen.pattern = symfillpattern;
                p.dropline = dropline;
                p.baseline = baseline;
                p.baseline_type = baselinetype;
                p.polygone_base_set = polybase;
                p.errbar = errbar;
                p.avalue = avalue;

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
        }//end global_inhibitor
        else if (p.polygone_base_set!=polybase)//a special addition for filling the area between different sets
        {
            for(i = 0; i < cd; i++)
            {
                setno = selset[i];
                get_graph_plotarr(grano, setno, &p);
                p.polygone_base_set = polybase;
                set_graph_plotarr(grano, setno, &p);
            }
        }
        if (gnos!=NULL) delete[] gnos;
        if (snos!=NULL) delete[] snos;
    }
    delete[] selset;
    mainWin->mainArea->completeRedraw();
}

void frmSet_Appearance::doAccept(void)
{
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmSet_Appearance::doClose(void)
{
    parentWidget()->hide();
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
    selection[0]=set_number;
    if (listSet->gr_no!=graph_number)
        listSet->set_graph_number(graph_number,false);

    listSet->set_new_selection(number,selection);

    listSet->get_selection(&number,&selection);
    showSetData(graph_number,set_number);
    delete[] selection;
    immediateUpdate=old_upd;
    updateRunning=false;
}

void frmSet_Appearance::showSetData(int graph_number,int set_number)
{
    if (graph_number<0 || graph_number>number_of_graphs()) return;
    if (set_number<0 || set_number>=g[graph_number].maxplot) return;
    updating=true;
    cset=set_number;
    char val[24];
    char dummy[2000];
    plotarr p;
    get_graph_plotarr(graph_number, set_number, &p);

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
    tabMa->cmbSymbType->setCurrentIndex(p.sym);
    tabMa->sldSymbSize->setValue((int)(p.symsize*100));
    sprintf(val, "%d", p.symchar);
    tabMa->ledSymbChar->setText(QString(val));
    tabMa->cmbLineType->setCurrentIndex(p.linet);
    tabMa->cmbLineStyle->setCurrentIndex(p.lines);
    tabMa->cmbLineColor->setCurrentIndex(p.linepen.color);
    tabMa->spnLineWidth->setValue(p.linew);
    //tabMa->ledString->setText(QString(p.lstr));
    tabMa->ledString->SetTextToMemory(g[graph_number].p[set_number].lstr,g[graph_number].p[set_number].orig_lstr);
    tabMa->chkAnnVal->setChecked(p.avalue.active);
    tabMa->chkDispErrBars->setChecked(p.errbar.active);

    tabSy->spnSymbWidth->setValue(p.symlinew);
    tabSy->cmbSymbStyle->setCurrentIndex(p.symlines);
    tabSy->cmbSymbPattern->setCurrentIndex(p.sympen.pattern);
    tabSy->cmbFillColor->setCurrentIndex(p.symfillpen.color);
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
    tabLi->cmbSet->setValue(p.polygone_base_set);

    tabAnVa->cmbFont->setCurrentIndex(p.avalue.font);
    tabAnVa->sldFontSize->setValue((int)(p.avalue.size*100));
    tabAnVa->cmbColor->setCurrentIndex(p.avalue.color);
    tabAnVa->sldFontAngle->setValue(p.avalue.angle);
    //tabAnVa->ledPrepend->setText(QString(p.avalue.prestr));
    //tabAnVa->ledAppend->setText(QString(p.avalue.appstr));
    tabAnVa->ledPrepend->SetTextToMemory(g[graph_number].p[set_number].avalue.prestr,g[graph_number].p[set_number].avalue.orig_prestr);
    tabAnVa->ledAppend->SetTextToMemory(g[graph_number].p[set_number].avalue.appstr,g[graph_number].p[set_number].avalue.orig_appstr);
    tabAnVa->cmbType->setCurrentIndex(p.avalue.type);
    tabAnVa->cmbPrecision->setCurrentIndex(p.avalue.prec);
    tabAnVa->cmbFormat->setCurrentIndex(p.avalue.format);
    sprintf(val, "%f", p.avalue.offset.x);
    tabAnVa->ledXOffs->setText(QString(val));
    sprintf(val, "%f", p.avalue.offset.y);
    tabAnVa->ledYOffs->setText(QString(val));

    tabErBa->cmbColor->setCurrentIndex(p.errbar.pen.color);
    tabErBa->cmbPattern->setCurrentIndex(p.errbar.pen.pattern);
    tabErBa->chkArrowClip->setChecked(p.errbar.arrow_clip);
    tabErBa->spnMaxLength->setValue(p.errbar.cliplen);
    tabErBa->spnbarWidth->setValue(p.errbar.linew);
    tabErBa->cmbBarStyle->setCurrentIndex(p.errbar.lines);
    tabErBa->spnRiserWidth->setValue(p.errbar.riser_linew);
    tabErBa->cmbRiserStyle->setCurrentIndex(p.errbar.riser_lines);
    tabErBa->sldBarSize->setValue((int)(p.errbar.barsize*100));

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
        sprintf(err_txt,"%s%d%s%d!",tr("Invalid Graph number: ").toAscii().constData(),graph_number,tr(", can't write to Set ").toAscii().constData(),set_number);
        errmsg(err_txt);
        return;
    }
    if (set_number<0 || set_number>=g[graph_number].maxplot)
    {
        sprintf(err_txt,"%s%d%s%d!",tr("Invalid Set number: ").toAscii().constData(),set_number,tr("; No such Set in Graph ").toAscii().constData(),graph_number);
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
        errmsg(tr("No set selected").toAscii().constData());
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
                set_legend_string(cg, setno, mybasename(get_legend_string(cg, setno)));
                tmp_po=get_legend_string(cg, setno);
                if (new_comm!=NULL) delete[] new_comm;
                new_comm=new char[strlen(tmp_po)+2];
                strcpy(new_comm,tmp_po);
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
            }
        }//end for
        showSetData(cg, cset);
        ///UpdateSymbols(cg, cset);
        set_dirtystate();
        mainWin->mainArea->completeRedraw();
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

frmSetAppearance::frmSetAppearance(QWidget * parent):QDialog(parent)
{
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Set Appearance"));
    setWindowIcon(QIcon(*GraceIcon));
    QVBoxLayout * layout=new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    flp=new frmSet_Appearance(this);
    layout->addWidget(flp->menuBar);
    layout->addWidget(flp);
    listSet=flp->listSet;
    setLayout(layout);
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

GrTabMain::GrTabMain(QWidget * parent):QWidget(parent)
{
    int number=6;
    QString types[6];
    types[0]=tr("XY graph");
    types[1]=tr("XY chart");
    types[2]=tr("Polar graph");
    types[3]=tr("Smith chart (N/I)");
    types[4]=tr("Fixed");
    types[5]=tr("Pie chart");

    grpPres=new QGroupBox(tr("Presentation"),this);
    grpTitles=new QGroupBox(tr("Titles"),this);
    grpViewport=new QGroupBox(tr("Viewport"),this);
    grpDispOpt=new QGroupBox(tr("Display options"),this);

    selType=new StdSelector(grpPres,tr("Type:"),number,types);

    chkStackChart=new QCheckBox(tr("Stacked chart"),grpPres);
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

    layout1=new QHBoxLayout;
    layout1->setMargin(STD_MARGIN);
    layout1->addWidget(selType);
    layout1->addWidget(chkStackChart);
    grpPres->setLayout(layout1);

    layout2=new QVBoxLayout;
    layout2->setMargin(STD_MARGIN);
    layout2->addWidget(ledTitle);
    layout2->addWidget(ledSubtitle);
    grpTitles->setLayout(layout2);

    layout3=new QGridLayout;
    layout3->setMargin(STD_MARGIN);
    layout3->addWidget(ledCoords[0],0,0);
    layout3->addWidget(ledCoords[1],0,1);
    layout3->addWidget(ledCoords[2],1,0);
    layout3->addWidget(ledCoords[3],1,1);
    grpViewport->setLayout(layout3);

    layout4=new QHBoxLayout;
    layout4->setMargin(STD_MARGIN);
    layout4->addWidget(chkDisplLegend);
    layout4->addWidget(chkFlipXY);
    grpDispOpt->setLayout(layout4);

    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(grpPres);
    layout->addWidget(grpTitles);
    layout->addWidget(grpViewport);
    layout->addWidget(grpDispOpt);
    setLayout(layout);
}

GrTabTitles::GrTabTitles(QWidget * parent):QWidget(parent)
{
    grpTitle=new QGroupBox(tr("Title"),this);
    grpSubTitle=new QGroupBox(tr("Subtitle"),this);

    selTitleFont=new FontSelector(grpTitle);
    sldTitleCharSize=new stdSlider(grpTitle,tr("Character size"),0,1000);
    sldTitleCharSize->setValue(150);
    selTitleColor=new ColorSelector(grpTitle);
    selTitleColor->setCurrentIndex(1);

    selSubFont=new FontSelector(grpSubTitle);
    sldSubCharSize=new stdSlider(grpSubTitle,tr("Character size"),0,1000);
    sldSubCharSize->setValue(100);
    selSubColor=new ColorSelector(grpSubTitle);
    selSubColor->setCurrentIndex(1);
    QWidget * emptyArea=new QWidget();
    emptyArea->setMaximumHeight(40);
    layout1=new QVBoxLayout;
    layout1->setMargin(STD_MARGIN);
    layout1->addWidget(selTitleFont);
    layout1->addWidget(sldTitleCharSize);
    layout1->addWidget(selTitleColor);
    grpTitle->setLayout(layout1);
    layout2=new QVBoxLayout;
    layout2->setMargin(STD_MARGIN);
    layout2->addWidget(selSubFont);
    layout2->addWidget(sldSubCharSize);
    layout2->addWidget(selSubColor);
    grpSubTitle->setLayout(layout2);
    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
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

    emptyArea=new QWidget(this);
    emptyArea->setMinimumHeight(120);

    layout1=new QGridLayout;
    layout1->setMargin(STD_MARGIN);
    layout1->addWidget(selFrameType,0,0,1,2);
    layout1->addWidget(selBoxColor,1,0);
    layout1->addWidget(selFrameBoxPattern,1,1);
    layout1->addWidget(selFrameBoxWidth,2,0);
    layout1->addWidget(selFrameBoxStyle,2,1);
    grpFrameBox->setLayout(layout1);
    layout2=new QHBoxLayout;
    layout2->setMargin(STD_MARGIN);
    layout2->addWidget(selFillColor);
    layout2->addWidget(selFrameFillPattern);
    grpFrameFill->setLayout(layout2);
    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(grpFrameBox);
    layout->addWidget(grpFrameFill);
    layout->addWidget(emptyArea);
    layout->setStretch(0,1);
    layout->setStretch(1,1);
    layout->setStretch(2,2);
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

    emptyArea=new QWidget(this);
    emptyArea->setMinimumHeight(30);

    layout1=new QGridLayout;
    layout1->setMargin(STD_MARGIN);
    layout1->addWidget(selLoc,0,0,1,2);
    layout1->addWidget(ledX,1,0);
    layout1->addWidget(ledY,1,1);
    grpLocation->setLayout(layout1);
    layout2=new QGridLayout;
    layout2->setMargin(STD_MARGIN);
    layout2->addWidget(selFrameLineColor,0,0);
    layout2->addWidget(selFrameLinePattern,0,1);
    layout2->addWidget(selFrameLineWidth,1,0);
    layout2->addWidget(selFrameLineStyle,1,1);
    grpFrameLine->setLayout(layout2);
    layout3=new QHBoxLayout;
    layout3->setMargin(STD_MARGIN);
    layout3->addWidget(selFrameFillColor);
    layout3->addWidget(selFrameFillPattern);
    grpFrameFill->setLayout(layout3);
    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(grpLocation);
    layout->addWidget(grpFrameLine);
    layout->addWidget(grpFrameFill);
    layout->addWidget(emptyArea);
    setLayout(layout);

    selLoc->setCurrentIndex(1);
    selFrameLineColor->setCurrentIndex(1);
    selFrameLinePattern->setCurrentIndex(1);
    selFrameLineStyle->setCurrentIndex(1);
    selFrameFillPattern->setCurrentIndex(1);
    selFrameLineWidth->setValue(1.0);
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
    sldTextSize=new stdSlider(grpTextProp,tr("Char size"),0,1000);
    sldTextSize->setValue(100);
    selTextColor=new ColorSelector(grpTextProp);

    number=6;
    selVGap=new StdSelector(grpPlacement,tr("V-gap:"),number,entries);
    selHGap=new StdSelector(grpPlacement,tr("H-gap:"),number,entries);
    number=9;
    selLineLength=new StdSelector(grpPlacement,tr("Legend line length:"),number,entries);
    chkPutRevOrder=new QCheckBox(tr("Put in reverse order"),grpPlacement);

    emptyArea=new QWidget(this);
    emptyArea->setMinimumHeight(20);

    layout1=new QVBoxLayout;
    layout1->setMargin(STD_MARGIN);
    layout1->addWidget(selTextFont);
    layout1->addWidget(sldTextSize);
    layout1->addWidget(selTextColor);
    grpTextProp->setLayout(layout1);
    layout2=new QGridLayout;
    layout2->setMargin(STD_MARGIN);
    layout2->addWidget(selVGap,0,0);
    layout2->addWidget(selHGap,0,1);
    layout2->addWidget(selLineLength,1,0,1,2);
    layout2->addWidget(chkPutRevOrder,2,0,1,2);
    grpPlacement->setLayout(layout2);
    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(grpTextProp);
    layout->addWidget(grpPlacement);
    layout->addWidget(emptyArea);
    layout->setStretch(0,1);
    layout->setStretch(1,1);
    layout->setStretch(2,4);
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

    emptyArea=new QWidget(this);
    emptyArea->setMinimumHeight(220);

    layout1=new QHBoxLayout;
    layout1->setMargin(STD_MARGIN);
    layout1->addWidget(ledZnormal);
    grp2Dplusgraphs->setLayout(layout1);
    layout2=new QHBoxLayout;
    layout2->setMargin(STD_MARGIN);
    layout2->addWidget(selBarGap);
    grpXYcharts->setLayout(layout2);
    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(grp2Dplusgraphs);
    layout->addWidget(grpXYcharts);
    layout->addWidget(emptyArea);
    setLayout(layout);
}

frmGraph_App::frmGraph_App(QWidget * parent):QWidget(parent)
{
    frmOpenPara=NULL;
    frmSavePara=NULL;
    //setFont(stdFont);
    //setWindowTitle(tr("qtGrace: Graph Appearance"));
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
    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(menuBar);
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
    connect(tabTitles->sldTitleCharSize,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(tabTitles->selTitleColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTitles->selSubFont,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTitles->sldSubCharSize,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(tabTitles->selSubColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));

    connect(tabFrame->selFrameType,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabFrame->selBoxColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabFrame->selFrameBoxPattern,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabFrame->selFrameBoxWidth,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(tabFrame->selFrameBoxStyle,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabFrame->selFillColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabFrame->selFrameFillPattern,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));

    connect(tabLegBox->selLoc->cmbPositionSelect,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLegBox->ledX->lenText,SIGNAL(returnPressed()),SLOT(update0()));
    connect(tabLegBox->ledY->lenText,SIGNAL(returnPressed()),SLOT(update0()));
    connect(tabLegBox->selFrameLineColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLegBox->selFrameLinePattern,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLegBox->selFrameLineWidth,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(tabLegBox->selFrameLineStyle,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLegBox->selFrameFillColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLegBox->selFrameFillPattern,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));

    connect(tabLegends->selTextFont,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLegends->sldTextSize,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(tabLegends->selTextColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLegends->selVGap,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLegends->selHGap,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLegends->selLineLength,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLegends->chkPutRevOrder,SIGNAL(toggled(bool)),SLOT(update3(bool)));

    connect(tabSpec->ledZnormal->lenText,SIGNAL(returnPressed()),SLOT(update0()));
    connect(tabSpec->selBarGap,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));

    //end immediate updates stuff
    init();

}

void frmGraph_App::newSelection(int i)
{
    int values[2]={i,0};
    if (i>=0)
    {
        update_graphapp_items(1,values);
    }
}

frmGraph_App::~frmGraph_App()
{}

void frmGraph_App::init(void)
{

    listGraph->update_number_of_entries();
    int n=1;
    int values[2]={0,0};

    int sel=1,*selection=new int[2];
    selection[0]=get_cg();
    listGraph->set_new_selection(sel,selection);
    delete[] selection;

    update_graphapp_items(n,values);
}

void frmGraph_App::CreateActions(void)
{
    actClose = new QAction(tr("&Close"), this);
    actClose->setShortcut(tr("Esc"));
    actClose->setStatusTip(tr("Close this Window"));
    connect(actClose, SIGNAL(triggered()), this, SLOT(doClose()));
    actOpen= new QAction(tr("&Open"), this);
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
    parentWidget()->hide();
}

void frmGraph_App::doApply(void)
{
    (void)graphapp_aac_cb();
}

void frmGraph_App::doOpen(void)
{
    if (frmOpenPara==NULL)
    {
        frmOpenPara=new frmIOForm(READ_PARAMETERS,this);
        connect(frmOpenPara,SIGNAL(newFileSelectedForIO(int,QString,bool,bool,bool)),SLOT(IOrequested(int,QString,bool,bool,bool)));
    }
    frmOpenPara->init();
    frmOpenPara->show();
    frmOpenPara->raise();
}

void frmGraph_App::doSave(void)
{
    if (frmSavePara==NULL)
    {
        frmSavePara=new frmIOForm(WRITE_PARAMETERS,this);
        connect(frmSavePara,SIGNAL(newFileSelectedForIO(int,QString,bool,bool,bool)),SLOT(IOrequested(int,QString,bool,bool,bool)));
    }
    frmSavePara->init();
    frmSavePara->show();
    frmSavePara->raise();
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
    char buf[1024];
    /*
 *     int flipxy;
 */

    graphtype = tabMain->selType->currentIndex();//GetChoice(graph_type_choice_item);

    /*v.xv1=atof(tabMain->ledCoords[0]->text().toAscii());
v.xv2=atof(tabMain->ledCoords[1]->text().toAscii());
v.yv1=atof(tabMain->ledCoords[2]->text().toAscii());
v.yv2=atof(tabMain->ledCoords[3]->text().toAscii());*/
    xv_evalexpr(tabMain->ledCoords[0], &v.xv1);
    xv_evalexpr(tabMain->ledCoords[1], &v.xv2);
    xv_evalexpr(tabMain->ledCoords[2], &v.yv1);
    xv_evalexpr(tabMain->ledCoords[3], &v.yv2);

    if (isvalid_viewport(v) == FALSE) {
        errmsg(tr("Invalid viewport coordinates").toAscii().constData());
        ApplyError=true;
        return RETURN_FAILURE;
    }

    set_default_string(&labs.title);
    //set_plotstr_string(&labs.title, GetTextString(label_title_text_item));

    tabMain->ledTitle->DynSetMemoryToText(labs.title.s_plotstring,labs.title.alt_plotstring);
    //strcpy(buf,tabMain->ledTitle->text().toAscii());
    //set_plotstr_string(&labs.title,buf);
    ///setting plotstring

    labs.title.charsize = tabTitles->sldTitleCharSize->value()/100.0;//GetCharSizeChoice(title_size_item);
    labs.title.color = tabTitles->selTitleColor->currentIndex();//GetOptionChoice(title_color_item);
    labs.title.font = tabTitles->selTitleFont->currentIndex();//GetOptionChoice(title_font_item);

    set_default_string(&labs.stitle);
    //set_plotstr_string(&labs.stitle, GetTextString(label_subtitle_text_item));

    tabMain->ledSubtitle->DynSetMemoryToText(labs.stitle.s_plotstring,labs.stitle.alt_plotstring);
    //strcpy(buf,tabMain->ledSubtitle->text().toAscii());
    //set_plotstr_string(&labs.stitle,buf);
    ///setting plotstring

    labs.stitle.charsize = tabTitles->sldSubCharSize->value()/100.0;//GetCharSizeChoice(stitle_size_item);
    labs.stitle.color = tabTitles->selSubColor->currentIndex();//GetOptionChoice(stitle_color_item);
    labs.stitle.font = tabTitles->selSubFont->currentIndex();//GetOptionChoice(stitle_font_item);

    f.type = tabFrame->selFrameType->currentIndex();//GetChoice(frame_framestyle_choice_item);
    f.pen.color = tabFrame->selBoxColor->currentIndex();//GetOptionChoice(frame_color_choice_item);
    f.pen.pattern = tabFrame->selFrameBoxPattern->currentIndex();//GetOptionChoice(frame_pattern_choice_item);
    f.linew = tabFrame->selFrameBoxWidth->value();//GetSpinChoice(frame_linew_choice_item);
    f.lines = tabFrame->selFrameBoxStyle->currentIndex();//GetOptionChoice(frame_lines_choice_item);
    f.fillpen.color = tabFrame->selFillColor->currentIndex();//GetOptionChoice(frame_fillcolor_choice_item);
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
    /*l.legx=atof(tabLegBox->ledX->text().toAscii());
    l.legy=atof(tabLegBox->ledY->text().toAscii());*/
    l.font = tabLegends->selTextFont->currentIndex();//GetOptionChoice(legend_font_item);
    l.color = tabLegends->selTextColor->currentIndex();//GetOptionChoice(legend_color_item);
    l.boxfillpen.color = tabLegBox->selFrameFillColor->currentIndex();//GetOptionChoice(legend_boxfillcolor_item);
    l.boxfillpen.pattern = tabLegBox->selFrameFillPattern->currentIndex();//GetOptionChoice(legend_boxfillpat_item);
    l.boxpen.color = tabLegBox->selFrameLineColor->currentIndex();//GetOptionChoice(legend_boxcolor_item);
    l.boxpen.pattern = tabLegBox->selFrameLinePattern->currentIndex();//GetOptionChoice(legend_boxpattern_item);
    l.boxlinew = tabLegBox->selFrameLineWidth->value();//GetSpinChoice(legend_boxlinew_item);
    l.boxlines = tabLegBox->selFrameLineStyle->currentIndex();//GetOptionChoice(legend_boxlines_item);

    stacked = (bool)tabMain->chkStackChart->isChecked();//GetToggleButtonState(stacked_item);

    bargap = tabSpec->selBarGap->value();//GetSpinChoice(bargap_item);

    xv_evalexpr(tabSpec->ledZnormal, &znorm);
    //znorm=atof(tabSpec->ledZnormal->text().toAscii());
    flipxy = (int)tabMain->chkFlipXY->isChecked();
    /*
 *     flipxy = GetToggleButtonState(graph_flipxy_item);
 */

    //n = GetListChoices(graph_selector, &values);
    listGraph->get_selection(&n,&values);

    if (GlobalInhibitor==false)
        SaveGraphStatesPrevious(n,values,UNDO_APPEARANCE);

    ListOfChanges.clear();
    ListOfOldStates.clear();

    get_graph_viewport(values[0],&v2);
    get_graph_legend(values[0],&l2);
    get_graph_labels(values[0],&labs2);
    get_graph_framep(values[0],&f2);

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
    /*
for (int i=0;i<ListOfChanges.size();i++)
cout << ListOfChanges.at(i).toAscii().constData() << endl;
*/
    for (j = 0; j < n; j++)
        set_graph_labels(values[j], &labs);//title and subtitle

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
                set_graph_legend(gno, &l);
                g[gno].xyflip = flipxy;
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

void frmGraph_App::update_graphapp_items(int n, int *values)
{
    bool sav_imm_upd=immediateUpdate;
    int gno;
    labels labs;
    char buf[32];
    
    if (n != 1) {
        return;
    } else {
        gno = values[0];
    }

    if (is_valid_gno(gno) != TRUE) {
        return;
    }
    immediateUpdate=false;
    updateRunning=true;
    //if (graphapp_dialog != NULL) {

    update_view(gno);

    update_frame_items(gno);

    updatelegends(gno);

    get_graph_labels(gno, &labs);

    tabMain->selType->setCurrentIndex(get_graph_type(gno));
    //SetChoice(graph_type_choice_item, get_graph_type(gno));
    tabSpec->selBarGap->setValue(get_graph_bargap(gno));
    //SetSpinChoice(bargap_item, get_graph_bargap(gno));
    tabMain->chkStackChart->setChecked((bool)is_graph_stacked(gno));
    //SetToggleButtonState(stacked_item, is_graph_stacked(gno));
    sprintf(buf, "%g", get_graph_znorm(gno));
    tabSpec->ledZnormal->setText(QString(buf));
    //xv_setstr(znorm_item, buf);
    tabMain->ledTitle->SetTextToMemory(g[gno].labs.title.s_plotstring,g[gno].labs.title.alt_plotstring);
    tabMain->ledSubtitle->SetTextToMemory(g[gno].labs.stitle.s_plotstring,g[gno].labs.stitle.alt_plotstring);

    /*tabMain->ledTitle->setText(QString(labs.title.s));
tabMain->ledSubtitle->setText(QString(labs.stitle.s));*/

    //SetTextString(label_title_text_item, labs.title.s);
    //SetTextString(label_subtitle_text_item, labs.stitle.s);
    tabTitles->sldTitleCharSize->setValue(int(labs.title.charsize*100));
    tabTitles->sldSubCharSize->setValue(int(labs.stitle.charsize*100));
    //SetCharSizeChoice(title_size_item, labs.title.charsize);
    //SetCharSizeChoice(stitle_size_item, labs.stitle.charsize);
    tabTitles->selTitleColor->setCurrentIndex(labs.title.color);
    tabTitles->selSubColor->setCurrentIndex(labs.stitle.color);
    //SetOptionChoice(title_color_item, labs.title.color);
    //SetOptionChoice(stitle_color_item, labs.stitle.color);
    tabTitles->selTitleFont->setCurrentIndex(labs.title.font);
    tabTitles->selSubFont->setCurrentIndex(labs.stitle.font);
    //SetOptionChoice(title_font_item, labs.title.font);
    //SetOptionChoice(stitle_font_item, labs.stitle.font);
    tabMain->chkFlipXY->setChecked(bool(g[gno].xyflip));
    /*
 *         SetToggleButtonState(graph_flipxy_item, g[gno].xyflip);
 */
    //}
    immediateUpdate=sav_imm_upd;
    updateRunning=false;
}

/*
 * Viewport update
 */
void frmGraph_App::update_view(int gno)
{
    view v;
    char buf[32];
    
    //if (graphapp_dialog) {
    get_graph_viewport(gno, &v);

    sprintf(buf, "%.9g", v.xv1);
    ///cout << "v.xv1=" << v.xv1 << endl;
    tabMain->ledCoords[0]->setText(QString(buf));
    //xv_setstr(define_view_xv1, buf);
    sprintf(buf, "%.9g", v.xv2);
    ///cout << "v.xv2=" << v.xv2 << endl;
    tabMain->ledCoords[1]->setText(QString(buf));
    //xv_setstr(define_view_xv2, buf);
    sprintf(buf, "%.9g", v.yv1);
    ///cout << "v.yv1=" << v.yv1 << endl;
    tabMain->ledCoords[2]->setText(QString(buf));
    //xv_setstr(define_view_yv1, buf);
    sprintf(buf, "%.9g", v.yv2);
    ///cout << "v.yv2=" << v.yv2 << endl;
    tabMain->ledCoords[3]->setText(QString(buf));
    //xv_setstr(define_view_yv2, buf);
    //}
}

/*
 * legend popup
 */
void frmGraph_App::updatelegends(int gno)
{
    legend l;
    char buf[32];
    
    //if (graphapp_dialog != NULL) {
    get_graph_legend(gno, &l);
    tabLegends->sldTextSize->setValue(int(l.charsize*100));
    //SetCharSizeChoice(legend_charsize_item, l.charsize);
    tabMain->chkDisplLegend->setChecked(l.active);
    //SetToggleButtonState(toggle_legends_item, l.active == TRUE);

    sprintf(buf, "%.9g", l.legx);
    tabLegBox->ledX->setText(QString(buf));
    //	xv_setstr(legend_x_item, buf);
    sprintf(buf, "%.9g", l.legy);
    tabLegBox->ledY->setText(QString(buf));
    //	xv_setstr(legend_y_item, buf);

    tabLegends->selVGap->setCurrentIndex(l.vgap);
    tabLegends->selHGap->setCurrentIndex(l.hgap);
    tabLegends->selLineLength->setCurrentIndex(l.len);
    tabLegends->chkPutRevOrder->setChecked(bool(l.invert));
    tabLegBox->selLoc->setCurrentIndex(!l.loctype);
    tabLegends->selTextFont->setCurrentIndex(l.font);
    tabLegends->selTextColor->setCurrentIndex(l.color);
    tabLegBox->selFrameLineColor->setCurrentIndex(l.boxpen.color);
    tabLegBox->selFrameLinePattern->setCurrentIndex(l.boxpen.pattern);
    tabLegBox->selFrameLineWidth->setValue(l.boxlinew);
    tabLegBox->selFrameLineStyle->setCurrentIndex(l.boxlines);
    tabLegBox->selFrameFillColor->setCurrentIndex(l.boxfillpen.color);
    tabLegBox->selFrameFillPattern->setCurrentIndex(l.boxfillpen.pattern);
}

void frmGraph_App::update_frame_items(int gno)
{
    framep f;
    get_graph_framep(gno, &f);
    tabFrame->selFrameType->setCurrentIndex(f.type);
    tabFrame->selBoxColor->setCurrentIndex(f.pen.color);
    tabFrame->selFrameBoxPattern->setCurrentIndex(f.pen.pattern);
    tabFrame->selFrameBoxWidth->setValue(f.linew);
    tabFrame->selFrameBoxStyle->setCurrentIndex(f.lines);
    tabFrame->selFillColor->setCurrentIndex(f.fillpen.color);
    tabFrame->selFrameFillPattern->setCurrentIndex(f.fillpen.pattern);
}

void frmGraph_App::show_graph_data_external(int n_gno)
{
    int number=1;
    int sel[2]={n_gno,0};
    listGraph->set_new_selection(number,sel);
}

void frmGraph_App::IOrequested(int type,QString file,bool exists,bool writeable,bool readable)
{
    char filename[512];
    strcpy(filename,file.toAscii());
    if (type==READ_PARAMETERS)
    {
        getparms(filename);
        frmOpenPara->hide();
        mainWin->mainArea->completeRedraw();
    }
    else if (type==WRITE_PARAMETERS)
    {
        int gno;
        FILE *pp;
        if (GetChoice(frmSavePara->selParamGraph) == 0) {
            gno = get_cg();
        } else {
            gno = ALL_GRAPHS;
        }
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
    static int i,nr,errpos;
    bool imm_upd_sav;
    if (!immediateUpdate) return;
    imm_upd_sav=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    GlobalInhibitor=true;
    doApply();//do this without actually doing something
    nr=nr_of_true_changes(ListOfChanges);
    errpos=0;
    if (nr>0)
    {
        for (i=0;i<ListOfChanges.size();i++)
        {
            strcpy(command,ListOfChanges.at(i).toAscii().constData());
            errpos = scanner(command);
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
    update0();
}

void frmGraph_App::update2(QString v)
{
    update0();
}

void frmGraph_App::update3(bool v)
{
    update0();
}

void frmGraph_App::update4(double v)
{
    update0();
}

frmGraphApp::frmGraphApp(QWidget * parent):QDialog(parent)
{
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Graph Appearance"));
    setWindowIcon(QIcon(*GraceIcon));

    QVBoxLayout * layout=new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    flp=new frmGraph_App(this);
    layout->addWidget(flp->menuBar);
    layout->addWidget(flp);
    listGraph=flp->listGraph;
    setLayout(layout);
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

AxisTabMain::AxisTabMain(QWidget * parent):QWidget(parent)
{
    int number;
    char dummy[10];
    QString * entr=new QString[35];
    grpAxisLabel=new QGroupBox(tr("Axis label"),this);
    ledAxisLabel=new stdLineEdit(grpAxisLabel,tr("Label string:"),true);
    layout0=new QHBoxLayout;
    layout0->setMargin(STD_MARGIN);
    layout0->addWidget(ledAxisLabel);
    grpAxisLabel->setLayout(layout0);

    grpTickProp=new QGroupBox(tr("Tick properties"),this);
    ledMajorSpacing=new stdLineEdit(grpTickProp,tr("Major spacing:"));
    selMinTicks=new stdIntSelector(grpTickProp,tr("Minor ticks:"),0,MAX_TICKS - 1);
    number=32;
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
    layout1->setMargin(STD_MARGIN);
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
    layout2->setMargin(STD_MARGIN);
    layout2->addWidget(chkDisplTickLabels,0,0);
    layout2->addWidget(chkDisplAxixBar,0,1);
    layout2->addWidget(chkDisplTickMarks,1,0);
    grpDisplOpt->setLayout(layout2);

    grpAxisPlace=new QGroupBox(tr("Axis placement"),this);
    chkZeroAxis=new QCheckBox(tr("Zero axis"),grpAxisPlace);
    ledOffsetNormal=new stdLineEdit(grpAxisPlace,tr("Offsets - Normal:"));
    ledOffsetOpposite=new stdLineEdit(grpAxisPlace,tr("Opposite:"));
    layout3=new QHBoxLayout;
    layout3->setMargin(STD_MARGIN);
    layout3->addWidget(chkZeroAxis);
    layout3->addWidget(ledOffsetNormal);
    layout3->addWidget(ledOffsetOpposite);
    grpAxisPlace->setLayout(layout3);

    grpTickLabelProp=new QGroupBox(tr("Tick label properties"),this);
    selTickLabelFont=new FontSelector(grpTickLabelProp);
    selTickLabelColor=new ColorSelector(grpTickLabelProp);
    layout4=new QHBoxLayout;
    layout4->setMargin(STD_MARGIN);
    layout4->addWidget(selTickLabelFont);
    layout4->addWidget(selTickLabelColor);
    grpTickLabelProp->setLayout(layout4);


    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
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
    layout1=new QGridLayout;
    layout1->setMargin(STD_MARGIN);
    layout1->addWidget(selLabelFont,0,0);
    layout1->addWidget(selLabelColor,0,1);
    layout1->addWidget(sldCharSize,1,0);
    layout1->addWidget(selLayout,1,1);
    layout1->addWidget(selSide,2,0);
    layout1->addWidget(selLocation,2,1);
    layout1->addWidget(ledParaOffs,3,0);
    layout1->addWidget(ledPerpendOffs,3,1);
    grpLabelProperties->setLayout(layout1);

    grpBarProperties=new QGroupBox(tr("Bar properties:"),this);
    selBarColor=new ColorSelector(grpBarProperties);
    selBarStyle=new LineStyleSelector(grpBarProperties);
    selBarWidth=new LineWidthSelector(grpBarProperties);
    selBarWidth->lblText->setText(tr("Width:"));
    layout2=new QGridLayout;
    layout2->setMargin(STD_MARGIN);
    layout2->addWidget(selBarColor,0,0);
    layout2->addWidget(selBarWidth,0,1);
    layout2->addWidget(selBarStyle,1,0);
    grpBarProperties->setLayout(layout2);

    empty=new QWidget(this);
    empty->setMinimumHeight(140);

    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(grpLabelProperties);
    layout->addWidget(grpBarProperties);
    layout->addWidget(empty);
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
    layout0=new QHBoxLayout;
    layout0->setMargin(STD_MARGIN);
    layout0->addWidget(sldCharSize);
    layout0->addWidget(sldCharAngle);
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
    layout1->setMargin(STD_MARGIN);
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
    layout2->setMargin(STD_MARGIN);
    layout2->addWidget(selSkipEvery,0,0);
    layout2->addWidget(ledAxisTransform,0,1);
    layout2->addWidget(ledPrepend,1,0);
    layout2->addWidget(ledAppend,1,1);
    layout2->addWidget(selLocation,2,0);
    layout2->addWidget(ledParaOffs,3,0);
    layout2->addWidget(ledPerpendOffs,3,1);
    grpExtra->setLayout(layout2);

    empty=new QWidget(this);
    empty->setMinimumHeight(70);

    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(grpLabels);
    layout->addWidget(grpPlacement);
    layout->addWidget(grpExtra);
    layout->addWidget(empty);
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
    layout0->setMargin(STD_MARGIN);
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
    layout1->setMargin(STD_MARGIN);
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
    layout2->setMargin(STD_MARGIN);
    layout2->addWidget(chkDrawMinGrid);
    layout2->addWidget(sldMinTickLength);
    layout2->addWidget(selMinTickColor);
    layout2->addWidget(selMinTickWidth);
    layout2->addWidget(selMinTickStyle);
    grpMinorTicks->setLayout(layout2);

    empty=new QWidget(this);
    empty->setMinimumHeight(80);

    layout=new QGridLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(grpPlacement,0,0,1,2);
    layout->addWidget(grpMajorTicks,1,0);
    layout->addWidget(grpMinorTicks,1,1);
    layout->addWidget(empty,2,0,1,2);
    setLayout(layout);
    delete[] entr;
}

AxisTabSpecial::AxisTabSpecial(QWidget * parent):QWidget(parent)
{
    int number;
    QString * entr=new QString[12];
    number=3;
    entr[0]=tr("None");
    entr[1]=tr("Tick marks");
    entr[2]=tr("Tick marks and labels");
    selSpecTicks=new StdSelector(this,tr("Special ticks:"),number,entr);
    selSpecTicks->entryValues[0]=TICKS_SPEC_NONE;
    selSpecTicks->entryValues[1]=TICKS_SPEC_MARKS;
    selSpecTicks->entryValues[2]=TICKS_SPEC_BOTH;
    selNumber=new stdIntSelector(this,tr("Number of user ticks to use:"),0,MAX_TICKS - 1);
    lblTickLocLabel=new QLabel(tr("Tick location - Label:"),this);

    scroll=new QScrollArea(this);
    spreadSpecLabels=new spreadSheet(scroll,2,256,3);
    spreadSpecLabels->setMinimumWidth(400);
    scroll->setWidget(spreadSpecLabels);

    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(selSpecTicks);
    layout->addWidget(selNumber);
    layout->addWidget(lblTickLocLabel);
    layout->addWidget(scroll);
    setLayout(layout);
    delete[] entr;
}

frmAxis_Prop::frmAxis_Prop(QWidget * parent):QWidget(parent)
{
    int number;
    curaxis=X_AXIS;
    QString * entr=new QString[5];
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Axis"));
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
    /*for (int i=0;i<4;i++)
{
empty[i]=new QWidget(this);
empty[i]->setMinimumWidth(50);
}*/
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
    tabs->addTab(tabSpecial, tr("Special"));
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
    number=4;
    entr[0]=tr("Current axis");
    entr[1]=tr("All axes, current graph");
    entr[2]=tr("Current axis, all graphs");
    entr[3]=tr("All axes, all graphs");
    selApplyTo=new StdSelector(this,tr("Apply to:"),number,entr);
    buttonGroup=new stdButtonGroup(this);
    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doAccept()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(doClose()));
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(doApply()));
    layout=new QGridLayout;
    layout->setMargin(STD_MARGIN);
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

    connect(tabLabelsBars->selLabelFont,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLabelsBars->selLabelColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLabelsBars->sldCharSize,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(tabLabelsBars->selLayout,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLabelsBars->selSide,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLabelsBars->selLocation,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLabelsBars->ledParaOffs,SIGNAL(changed()),SLOT(update0()));
    connect(tabLabelsBars->ledPerpendOffs,SIGNAL(changed()),SLOT(update0()));
    connect(tabLabelsBars->selBarColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLabelsBars->selBarStyle,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabLabelsBars->selBarWidth,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));

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

    connect(tabTickMarks->selPointing,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTickMarks->selDrawOn,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTickMarks->setAutotickDiv,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTickMarks->chkPlaceRoundPos,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(tabTickMarks->chkDrawMajGrid,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(tabTickMarks->sldMajTickLength,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(tabTickMarks->selMajTickColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTickMarks->selMajTickWidth,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(tabTickMarks->selMajTickStyle,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTickMarks->chkDrawMinGrid,SIGNAL(toggled(bool)),SLOT(update3(bool)));
    connect(tabTickMarks->sldMinTickLength,SIGNAL(valueChanged(int)),SLOT(update1(int)));
    connect(tabTickMarks->selMinTickColor,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabTickMarks->selMinTickWidth,SIGNAL(currentValueChanged(double)),SLOT(update4(double)));
    connect(tabTickMarks->selMinTickStyle,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));

    connect(tabSpecial->selSpecTicks,SIGNAL(currentIndexChanged(int)),SLOT(update1(int)));
    connect(tabSpecial->selNumber,SIGNAL(currentValueChanged(int)),SLOT(update1(int)));
    connect(tabSpecial->spreadSpecLabels,SIGNAL(changed()),SLOT(update0()));
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
    axes_aac_cb();
}

void frmAxis_Prop::doClose(void)
{
    parentWidget()->hide();
}

/*
 * Callback function for definition of tick marks and axis labels.
 */
int frmAxis_Prop::axes_aac_cb(void)
{
    ApplyError=false;
    QString text;
    char dummy[MAX_STRING_LENGTH],dummy2[MAX_STRING_LENGTH],descr_axis[32];
    //double ddummy[4];
    int i, j;
    int applyto;
    int axis_start, axis_stop, graph_start, graph_stop;
    int scale, invert;
    tickmarks *t,*t2;
    double axestart, axestop;
    //char *cp;
    world w,w2;

    applyto = selApplyTo->currentValue();

    t = new_graph_tickmarks();
    if (!t)
    {
        return RETURN_FAILURE;
    }

    t->active = chkActive->isChecked();
    t->zero = tabMain->chkZeroAxis->isChecked();
    if (xv_evalexpr(tabMain->ledMajorSpacing, &t->tmajor) != RETURN_SUCCESS)
    {
        errmsg(tr("Specify major tick spacing").toAscii().constData());
        free_graph_tickmarks(t);
        ApplyError=true;
        return RETURN_FAILURE;
    }
    t->nminor =tabMain->selMinTicks->value();
    t->tl_flag = tabMain->chkDisplTickLabels->isChecked();
    t->t_flag = tabMain->chkDisplTickMarks->isChecked();
    t->t_drawbar = tabMain->chkDisplAxixBar->isChecked();

    tabMain->ledAxisLabel->DynSetMemoryToText(t->label.s_plotstring,t->label.alt_plotstring);
    //strcpy(dummy,tabMain->ledAxisLabel->text().toAscii());
    //set_plotstr_string(&t->label, dummy);
    ///setting plotstring

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
    t->label.color = tabLabelsBars->selLabelColor->currentIndex();
    t->label.charsize = tabLabelsBars->sldCharSize->value()/100.0;

    /* somehow the value of axislabelop gets automagically correctly
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
    t->tl_skip = tabTickLabels->selSkipEvery->currentIndex();
    t->tl_prec = tabMain->selPrecision->currentIndex();
    t->tl_staggered = tabTickLabels->selStagger->currentIndex();

    tabTickLabels->ledAppend->SetMemoryToText(t->tl_appstr,t->orig_tl_appstr);
    tabTickLabels->ledPrepend->SetMemoryToText(t->tl_prestr,t->orig_tl_prestr);
    //strcpy(t->tl_appstr, tabTickLabels->ledAppend->text().toAscii());
    //strcpy(t->tl_prestr, tabTickLabels->ledPrepend->text().toAscii());
    ///setting strings

    t->tl_starttype =  tabTickLabels->selStartAt->currentIndex()== 0 ? TYPE_AUTO : TYPE_SPEC;
    if (t->tl_starttype == TYPE_SPEC)
    {
        if(xv_evalexpr(tabTickLabels->ledStart, &t->tl_start) != RETURN_SUCCESS)
        {
            errmsg(tr("Specify tick label start").toAscii().constData());
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
            errmsg(tr("Specify tick label stop").toAscii().constData());
            free_graph_tickmarks(t);
            ApplyError=true;
            return RETURN_FAILURE;
        }
    }
    t->tl_format = tabMain->selFormat->currentIndex();
    strcpy(dummy,tabTickLabels->ledAxisTransform->text().toAscii());
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
    
    t->props.color = tabTickMarks->selMajTickColor->currentIndex();
    t->props.linew = tabTickMarks->selMajTickWidth->value();
    t->props.lines = tabTickMarks->selMajTickStyle->currentIndex();
    t->mprops.color = tabTickMarks->selMinTickColor->currentIndex();
    t->mprops.linew = tabTickMarks->selMinTickWidth->value();
    t->mprops.lines = tabTickMarks->selMinTickStyle->currentIndex();
    t->props.size = tabTickMarks->sldMajTickLength->value()/100.0;
    t->mprops.size = tabTickMarks->sldMinTickLength->value()/100.0;
    t->t_autonum = 2+tabTickMarks->setAutotickDiv->currentIndex();
    t->t_round = tabTickMarks->chkPlaceRoundPos->isChecked();
    t->props.gridflag = tabTickMarks->chkDrawMajGrid->isChecked();
    t->mprops.gridflag = tabTickMarks->chkDrawMinGrid->isChecked();
    t->t_drawbarcolor = tabLabelsBars->selBarColor->currentIndex();
    t->t_drawbarlinew = tabLabelsBars->selBarWidth->value();
    t->t_drawbarlines = tabLabelsBars->selBarStyle->currentIndex();

    t->t_spec = tabSpecial->selSpecTicks->currentValue();
    /* only read special info if special ticks used */
    if (t->t_spec != TICKS_SPEC_NONE)
    {
        t->nticks = tabSpecial->selNumber->value();
        /* ensure that enough tick positions have been specified */
        for (i = 0; i < t->nticks; i++)
        {
            if (xv_evalexpr(tabSpecial->spreadSpecLabels->axislines[i]->ledLocation, &t->tloc[i].wtpos) == RETURN_SUCCESS)
            {
                strcpy(dummy,tabSpecial->spreadSpecLabels->axislines[i]->ledLabel->text().toAscii());//copy text directly
                //strcpy(dummy,tabSpecial->spreadSpecLabels->axislines[i]->LabelText().toAscii());
                if (dummy[0] == '\0')
                {
                    t->tloc[i].type = TICK_TYPE_MINOR;
                }
                else
                {
                    t->tloc[i].type = TICK_TYPE_MAJOR;
                }
                if (t->t_spec == TICKS_SPEC_BOTH)
                {
                    tabSpecial->spreadSpecLabels->axislines[i]->SetMemoryToText(t->tloc[i].label,t->tloc[i].orig_label);
                    /*
                    t->tloc[i].orig_label = copy_string(t->tloc[i].label, dummy);
                    t->tloc[i].label = copy_string(t->tloc[i].label, dummy);
                    if (activateLaTeXsupport==true)
                    {
                    text=QString(t->tloc[i].label);
                    complete_LaTeX_to_Grace_Translator(text);
                    t->tloc[i].label=copy_string(t->tloc[i].label,text.toAscii().constData());
                    }
                    */
                }
                else
                {//simply copy everything
                    t->tloc[i].orig_label = copy_string(t->tloc[i].label, NULL);
                    t->tloc[i].label = copy_string(t->tloc[i].label, NULL);
                }
            }
            else
            {
                errmsg(tr("Not enough tick locations specified").toAscii().constData());
                free_graph_tickmarks(t);
                ApplyError=true;
                return RETURN_FAILURE;
            }
        }//end for-loop
    }//end if !TICK_SPEC_NONE
    
    switch (applyto) {
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
    default:
        axis_start = curaxis;
        axis_stop  = curaxis;
        graph_start = cg;
        graph_stop  = cg;
        break;
    }

    if (xv_evalexpr(ledStart, &axestart) != RETURN_SUCCESS ||
            xv_evalexpr(ledStop,  &axestop)  != RETURN_SUCCESS)
    {
        errmsg(tr("Axis start/stop values undefined").toAscii().constData());
        free_graph_tickmarks(t);
        ApplyError=true;
        return RETURN_FAILURE;
    }

    //Undo-Stuff
    SaveTickmarksStatesPrevious(axis_start,axis_stop,graph_start,graph_stop);
    ListOfChanges.clear();
    ListOfOldStates.clear();

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
            t2 = get_graph_tickmarks(i,j);//t2 is the original
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
            switch(tabTickLabels->selSide->currentIndex())
            {
            case 0:
                t->tl_op = PLACEMENT_NORMAL;
                strcpy(dummy2,"normal");
                break;
            case 1:
                t->tl_op = PLACEMENT_OPPOSITE;
                strcpy(dummy2,"opposite");
                break;
            case 2:
                t->tl_op = PLACEMENT_BOTH;
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
            switch(tabTickMarks->selDrawOn->currentIndex())
            {
            case 0:
                t->t_op = PLACEMENT_NORMAL;
                strcpy(dummy2,"normal");
                break;
            case 1:
                t->t_op = PLACEMENT_OPPOSITE;
                strcpy(dummy2,"opposite");
                break;
            case 2:
                t->t_op = PLACEMENT_BOTH;
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
            if (t->active!=t2->active)
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
                if ((t->label.s_plotstring!=NULL && t2->label.s_plotstring==NULL) || (t->label.s_plotstring==NULL && t2->label.s_plotstring!=NULL) || (t->label.s_plotstring!=NULL && t2->label.s_plotstring!=NULL && strcmp(t->label.s_plotstring,t2->label.s_plotstring)))
                {
                    sprintf(dummy,"    %s  label \"%s\"",descr_axis,t->label.s_plotstring);
                    ListOfChanges << QString(dummy);
                    sprintf(dummy,"    %s  label \"%s\"",descr_axis,t2->label.s_plotstring);
                    ListOfOldStates << QString(dummy);
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
            if (!(t->tl_appstr==NULL && t2->tl_appstr==NULL))
                if ((t->tl_appstr!=NULL && t2->tl_appstr==NULL)|| (t->tl_appstr==NULL && t2->tl_appstr!=NULL) || (t->tl_appstr!=NULL && t2->tl_appstr!=NULL && strcmp(t->tl_appstr,t2->tl_appstr)))
                {
                    sprintf(dummy,"    %s  ticklabel append \"%s\"",descr_axis,t->tl_appstr);
                    ListOfChanges << QString(dummy);
                    sprintf(dummy,"    %s  ticklabel append \"%s\"",descr_axis,t2->tl_appstr);
                    ListOfOldStates << QString(dummy);
                }
            if (!(t->tl_prestr==NULL && t2->tl_prestr==NULL))
                if ((t->tl_prestr!=NULL && t2->tl_prestr==NULL) || (t->tl_prestr==NULL && t2->tl_prestr!=NULL) || (t->tl_prestr!=NULL && t2->tl_prestr!=NULL && strcmp(t->tl_prestr,t2->tl_prestr)))
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
                sprintf(dummy,"    %s  ticklabel format %s",descr_axis,fmt_option_items[t->tl_format].label);
                ListOfChanges << QString(dummy);
                sprintf(dummy,"    %s  ticklabel format %s",descr_axis,fmt_option_items[t2->tl_format].label);
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
            set_graph_tickmarks(i, j, t);//actually do something!!!
            //the following commands are neccessary for LaTeX-support (because dynamic strings are coppied dynamically)
            if (i==get_cg() && j==curaxis)
            {
                tabMain->ledAxisLabel->SetTextToMemory(g[i].t[j]->label.s_plotstring,g[i].t[j]->label.alt_plotstring);
                tabTickLabels->ledAppend->SetTextToMemory(g[i].t[j]->tl_appstr,g[i].t[j]->orig_tl_appstr);
                tabTickLabels->ledPrepend->SetTextToMemory(g[i].t[j]->tl_prestr,g[i].t[j]->orig_tl_prestr);
                if (g[i].t[j]->t_spec == TICKS_SPEC_BOTH)
                {
                    for (int k=0;k<g[i].t[j]->nticks;k++)
                        tabSpecial->spreadSpecLabels->axislines[k]->SetTextToMemory(g[i].t[j]->tloc[k].label,g[i].t[j]->tloc[k].orig_label);
                }
            }

        }//end axes
    }//end graphs
    free_graph_tickmarks(t);
    if (GlobalInhibitor==false)
        mainWin->mainArea->completeRedraw();
    update_ticks(cg);
    //Show actual list of changes
    /*cout << "changes" << endl;
for (i=0;i<ListOfChanges.size();i++)
cout << ListOfChanges.at(i).toAscii().constData() << endl;
    cout << "old states" << endl;
for (i=0;i<ListOfOldStates.size();i++)
cout << ListOfOldStates.at(i).toAscii().constData() << endl;*/
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
    //char dummy[128];
    xv_evalexpr(tabMain->ledMajorSpacing, &major_space);
    xv_evalexpr(ledStart, &axestart);
    xv_evalexpr(ledStop,  &axestop);
    auton = 2+tabTickMarks->setAutotickDiv->currentIndex();
    switch (scale) {
    case SCALE_NORMAL:
        if (major_space <= 0.0) {
            sprintf(buf, "%g", (axestop - axestart)/auton);
            //xv_setstr(tmajor, buf);
            tabMain->ledMajorSpacing->setText(QString(buf));
        }
        break;
    case SCALE_LOG:
        if (axestart <= 0.0 && axestop <= 0.0) {
            errmsg(tr("Can't set logarithmic scale for negative coordinates").toAscii().constData());
            //SetOptionChoice(axis_scale, SCALE_NORMAL);
            ApplyError=true;
            selScale->setCurrentValue(SCALE_NORMAL);
            return;
        } else if (axestart <= 0.0) {
            axestart = axestop/1.0e3;
            sprintf(buf, "%g", axestart);
            //xv_setstr(axis_world_start, buf);
            ledStart->setText(QString(buf));
        }
        //xv_setstr(tmajor, "10");
        tabMain->ledMajorSpacing->setText(QString("10"));
        //SetSpinChoice(nminor, 9);
        tabMain->selMinTicks->setValue(9);
        break;
    case SCALE_LOGIT:
        if (axestart <= 0.0 && axestop <= 0.0) {
            errmsg(tr("Can't set logit scale for values outside 0 and 1").toAscii().constData());
            ApplyError=true;
            selScale->setCurrentValue(SCALE_NORMAL);//SetOptionChoice(axis_scale, SCALE_NORMAL);
            return;
        }
        if (axestart <= 0.0) {
            axestart = 0.1;
            sprintf(buf, "%g", axestart);
            //xv_setstr(axis_world_start, buf);
            ledStart->setText(QString(buf));
        }
        if (axestop >= 1.0) {
            axestop = 0.95;
            sprintf(buf, "%g", axestop);
            //xv_setstr(axis_world_stop, buf);
            ledStop->setText(QString(buf));
        }
        if (major_space >= 1.0) {
            //xv_setstr(tmajor, "0.6");
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
    t = get_graph_tickmarks(gno, curaxis);
    if (!t) {
        immediateUpdate=old_upd;
        updateRunning=false;
        return;
    }
    chkActive->setChecked(is_axis_active(gno, curaxis));
    if (is_axis_active(gno, curaxis) == FALSE) {
        tabs->setEnabled(false);
    } else {
        tabs->setEnabled(true);
    }
    selEdit->setCurrentValue(curaxis);
    tabMain->chkZeroAxis->setChecked(is_zero_axis(gno, curaxis));
    get_graph_world(gno, &w);
    if (is_xaxis(curaxis)) {
        sprintf(buf, "%.9g", w.xg1);
        ledStart->setText(QString(buf));
        //xv_setstr(axis_world_start, buf);
        sprintf(buf, "%.9g", w.xg2);
        ledStop->setText(QString(buf));
        //xv_setstr(axis_world_stop, buf);
        selScale->setCurrentValue(get_graph_xscale(gno));
        chkInvAxis->setChecked(is_graph_xinvert(gno));
    } else {
        sprintf(buf, "%.9g", w.yg1);
        ledStart->setText(QString(buf));
        //xv_setstr(axis_world_start, buf);
        sprintf(buf, "%.9g", w.yg2);
        ledStop->setText(QString(buf));
        //xv_setstr(axis_world_stop, buf);
        selScale->setCurrentValue(get_graph_yscale(gno));
        chkInvAxis->setChecked(is_graph_yinvert(gno));
    }
    sprintf(buf, "%.4f", t->offsx);
    //xv_setstr(offx, buf);
    tabMain->ledOffsetNormal->setText(QString(buf));
    sprintf(buf, "%.4f", t->offsy);
    //xv_setstr(offy, buf);
    tabMain->ledOffsetOpposite->setText(QString(buf));
    tabLabelsBars->selLayout->setCurrentIndex(t->label_layout == LAYOUT_PERPENDICULAR ? 1 : 0);
    tabLabelsBars->selLocation->setCurrentIndex(t->label_place);
    sprintf(buf, "%.4f", t->label.x);
    //xv_setstr(axislabelspec_para, buf);
    tabLabelsBars->ledParaOffs->setText(QString(buf));
    sprintf(buf, "%.4f", t->label.y);
    //xv_setstr(axislabelspec_perp, buf);
    tabLabelsBars->ledPerpendOffs->setText(QString(buf));
    tabLabelsBars->ledParaOffs->setEnabled(t->label_place == TYPE_SPEC);
    tabLabelsBars->ledPerpendOffs->setEnabled(t->label_place == TYPE_SPEC);
    tabLabelsBars->selLabelFont->setCurrentIndex(t->label.font);
    tabLabelsBars->selLabelColor->setCurrentIndex(t->label.color);
    tabLabelsBars->sldCharSize->setValue(100*t->label.charsize);
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

    if (is_log_axis(gno, curaxis)) {
        if (t->tmajor <= 1.0) {
            t->tmajor = 10.0;
        }
        sprintf(buf, "%g", t->tmajor);
    } else if (is_logit_axis(gno, curaxis)) {
        if (t->tmajor <= 0.0) {
            t->tmajor = 0.1;
        }
        else if (t->tmajor >= 0.5) {
            t->tmajor = 0.4;
        }
        sprintf(buf, "%g", t->tmajor);
    } else if (t->tmajor > 0) {
        sprintf(buf, "%g", t->tmajor);
    } else {
        strcpy(buf, "UNDEFINED");
    }
    tabMain->ledMajorSpacing->setText(QString(buf));
    //xv_setstr(tmajor, buf);
    tabMain->selMinTicks->setValue(t->nminor);
    //SetSpinChoice(nminor, t->nminor);
    tabMain->selTickLabelFont->setCurrentIndex(t->tl_font);
    tabMain->selTickLabelColor->setCurrentIndex(t->tl_color);
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
    if (t->tl_starttype == TYPE_SPEC) {
        sprintf(buf, "%f", t->tl_start);
        tabTickLabels->ledStart->setText(QString(buf));
        //xv_setstr(tlstart, buf);
        /*sprintf(buf, "%f", t->tl_stop);
            xv_setstr(tlstop, buf);*/
    }
    tabTickLabels->selStopAt->setCurrentIndex(t->tl_stoptype== TYPE_SPEC?1:0);
    //SetChoice(tlstoptype, t->tl_stoptype == TYPE_SPEC);
    if (t->tl_stoptype == TYPE_SPEC) {
        sprintf(buf, "%f", t->tl_stop);
        tabTickLabels->ledStop->setText(QString(buf));
        //xv_setstr(tlstop, buf);
    }
    tabMain->selFormat->setCurrentIndex(t->tl_format);
    tabMain->selPrecision->setCurrentIndex(t->tl_prec);
    tabTickLabels->ledAxisTransform->setText(QString(t->tl_formula));
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
    sprintf(buf, "%.4f", t->tl_gap.x);
    tabTickLabels->ledParaOffs->setText(QString(buf));
    //xv_setstr(tlgap_para, buf);
    sprintf(buf, "%.4f", t->tl_gap.y);
    tabTickLabels->ledPerpendOffs->setText(QString(buf));
    //xv_setstr(tlgap_perp, buf);
    tabTickLabels->ledParaOffs->setEnabled(t->tl_gaptype == TYPE_SPEC);
    tabTickLabels->ledPerpendOffs->setEnabled(t->tl_gaptype == TYPE_SPEC);
    tabTickLabels->sldCharAngle->setValue(t->tl_angle);
    tabTickLabels->sldCharSize->setValue(t->tl_charsize*100);
    switch (t->t_inout) {
    case TICKS_IN:
        //SetChoice(tinout, 0);
        tabTickMarks->selPointing->setCurrentIndex(0);
        break;
    case TICKS_OUT:
        //SetChoice(tinout, 1);
        tabTickMarks->selPointing->setCurrentIndex(1);
        break;
    case TICKS_BOTH:
        //SetChoice(tinout, 2);
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
    tabTickMarks->selMajTickWidth->setValue(t->props.linew);
    tabTickMarks->selMajTickStyle->setCurrentIndex(t->props.lines);
    tabTickMarks->selMinTickColor->setCurrentIndex(t->mprops.color);
    tabTickMarks->selMinTickWidth->setValue(t->mprops.linew);
    tabTickMarks->selMinTickStyle->setCurrentIndex(t->mprops.lines);
    tabTickMarks->sldMajTickLength->setValue(100*t->props.size);
    tabTickMarks->sldMinTickLength->setValue(100*t->mprops.size);
    tabTickMarks->setAutotickDiv->setCurrentIndex(t->t_autonum - 2);
    tabTickMarks->chkPlaceRoundPos->setChecked((bool)t->t_round);
    tabTickMarks->chkDrawMajGrid->setChecked((bool)t->props.gridflag);
    tabTickMarks->chkDrawMinGrid->setChecked((bool)t->mprops.gridflag);
    tabLabelsBars->selBarColor->setCurrentIndex(t->t_drawbarcolor);
    tabLabelsBars->selBarWidth->setValue(t->t_drawbarlinew);
    tabLabelsBars->selBarStyle->setCurrentIndex(t->t_drawbarlines);
    tabSpecial->selSpecTicks->setCurrentValue(t->t_spec);
    tabSpecial->selNumber->setValue(t->nticks);
    for (i = 0; i < t->nticks; i++)
    {
        sprintf(buf, "%.9g", t->tloc[i].wtpos);
        tabSpecial->spreadSpecLabels->axislines[i]->ledLocation->setText(QString(buf));
        //xv_setstr(specloc[i], buf);
        if (t->tloc[i].type == TICK_TYPE_MAJOR)
        {
            tabSpecial->spreadSpecLabels->axislines[i]->SetTextToMemory(g[gno].t[curaxis]->tloc[i].label,g[gno].t[curaxis]->tloc[i].orig_label);
            //tabSpecial->spreadSpecLabels->axislines[i]->ledLabel->setText(QString(t->tloc[i].label));
            //xv_setstr(speclabel[i], t->tloc[i].label);
        }
        else
        {
            tabSpecial->spreadSpecLabels->axislines[i]->ledLabel->setText(QString(""));
            //xv_setstr(speclabel[i], "");
        }
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

void frmAxis_Prop::update0(void)
{
    static int i,nr,errpos;
    bool imm_upd_sav;
    if (!immediateUpdate) return;
    imm_upd_sav=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    GlobalInhibitor=true;
    doApply();//do this without actually doing something
    nr=nr_of_true_changes(ListOfChanges);
    errpos=0;
    if (nr>0)
    {
        for (i=0;i<ListOfChanges.size();i++)
        {
            strcpy(command,ListOfChanges.at(i).toAscii().constData());
            errpos = scanner(command);
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
    update0();
}

void frmAxis_Prop::update2(QString v)
{
    update0();
}

void frmAxis_Prop::update3(bool v)
{
    update0();
}

void frmAxis_Prop::update4(double v)
{
    update0();
}

frmAxisProp::frmAxisProp(QWidget * parent):QDialog(parent)
{
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Axis"));
    setWindowIcon(QIcon(*GraceIcon));
    QVBoxLayout * layout=new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    flp=new frmAxis_Prop(this);
    layout->addWidget(flp);
    setLayout(layout);
}

void frmAxisProp::update_ticks(int gno)
{
    flp->update_ticks(gno);
}

void frmAxisProp::create_axes_dialog(int axisno)
{
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

frmNetCDF::frmNetCDF(QWidget * parent):QDialog(parent)
{
    cmdQuery=new QPushButton(tr("Close"),this);
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: netCDF"));
    setWindowIcon(QIcon(*GraceIcon));
    lblSelX=new QLabel(tr("Select set X:"));
    lblSelY=new QLabel(tr("Select set Y:"));
    netcdf_listx_item=new uniList(TEXTLIST,this);
    netcdf_listy_item=new uniList(TEXTLIST,this);
    netcdf_listx_item->setBehavior(true,false,false);
    netcdf_listy_item->setBehavior(true,false,false);
    netcdf_file_item=new stdLineEdit(this,tr("netCDF file:"));
    netcdf_file_item->lenText->setText(QString(""));
    buttonGroup=new stdButtonGroup(this,true,true,true);
    buttonGroup->layout->addWidget(cmdQuery);
    buttonGroup->cmdApply->setText(tr("Accept"));
    buttonGroup->cmdAccept->setText(tr("Files..."));
    buttonGroup->cmdClose->setText(tr("Update"));
    buttonGroup->cmdHelp->setText(tr("Query"));
    cmdQuery->setDefault(false);
    buttonGroup->cmdApply->setDefault(true);

    connect(buttonGroup->cmdAccept,SIGNAL(clicked()),this,SLOT(doFiles()));
    connect(buttonGroup->cmdClose,SIGNAL(clicked()),this,SLOT(update_netcdfs()));
    connect(buttonGroup->cmdApply,SIGNAL(clicked()),this,SLOT(do_netcdf_proc()));
    connect(buttonGroup->cmdHelp,SIGNAL(clicked()),this,SLOT(do_netcdfquery_proc()));
    connect(cmdQuery,SIGNAL(clicked()),this,SLOT(doClose()));

    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->addWidget(lblSelX);
    layout->addWidget(netcdf_listx_item);
    layout->addWidget(lblSelY);
    layout->addWidget(netcdf_listy_item);
    layout->addWidget(netcdf_file_item);
    layout->addWidget(buttonGroup);
    setLayout(layout);
}

void frmNetCDF::init(void)
{
    update_netcdfs();
}

void frmNetCDF::doFiles(void)
{
    if (FormReadNetCDF==NULL)
    {
        FormReadNetCDF=new frmIOForm(READ_NETCDF_FORM,this);
        connect(FormReadNetCDF,SIGNAL(newFileSelectedForIO(int,QString,bool,bool,bool)),SLOT(IOrequested(int,QString,bool,bool,bool)));
    }
    FormReadNetCDF->init();
    FormReadNetCDF->show();
    FormReadNetCDF->raise();
    FormReadNetCDF->activateWindow();
}

void frmNetCDF::IOrequested(int type,QString file,bool exists,bool writeable,bool readable)
{
    char * filename=new char[file.length()+5];
    strcpy(filename,file.toAscii());
    char *dummy=new char[strlen(filename)+30];// Win port
    sprintf(dummy,"%s%s.",tr("Can't open file ").toAscii().constData(),filename);
    if (exists==true && readable==true)
    {
        xv_setstr(netcdf_file_item, filename);
        update_netcdfs();
        FormReadNetCDF->hide();
    }
    else
    {
        errwin(dummy);
    }
    delete[] filename;
    delete[] dummy;
}

void frmNetCDF::doClose(void)
{
    hide();
}

void frmNetCDF::update_netcdfs(void)
{
    int i;
    char buf[256], fname[512];
    QString xms;
    int cdfid;			/* netCDF id */
    int ndims, nvars, ngatts, recdim;
    int var_id;
    char varname[256];
    nc_type datatype = 0;
    int dim[100], natts;
    long dimlen[100];
    long len;

    set_wait_cursor();

    ncopts = 0;			/* no crash on error */

    //if (netcdf_frame != NULL) {
    strcpy(fname, xv_getstr(netcdf_file_item));
    set_wait_cursor();
    netcdf_listx_item->clear();
    //XmListDeleteAllItems(netcdf_listx_item);
    netcdf_listy_item->clear();
    //XmListDeleteAllItems(netcdf_listy_item);
    xms = tr("INDEX");
    netcdf_listx_item->add_Item(xms);
    //XmListAddItemUnselected(netcdf_listx_item, xms, 0);
    //XmStringFree(xms);

    if (strlen(fname) < 2) {
        unset_wait_cursor();
        return;
    }
#ifdef HAVE_NETCDF
    if ((cdfid = ncopen(fname, NC_NOWRITE)) == -1) {
        errmsg(tr("Can't open file.").toAscii().constData());
        unset_wait_cursor();
        return;
    }

    ncinquire(cdfid, &ndims, &nvars, &ngatts, &recdim);
    for (i = 0; i < ndims; i++) {
        ncdiminq(cdfid, i, NULL, &dimlen[i]);
    }
#endif
    for (i = 0; i < nvars; i++) {
#ifdef HAVE_NETCDF
        ncvarinq(cdfid, i, varname, &datatype, &ndims, dim, &natts);

        if ((var_id = ncvarid(cdfid, varname)) == -1) {
            char ebuf[256];
            sprintf(ebuf, "update_netcdfs(): %s %s",tr("No such variable").toAscii().constData(), varname);
            errmsg(ebuf);
            continue;
        }
#endif
        if (ndims != 1) {
            continue;
        }
#ifdef HAVE_NETCDF
        ncdiminq(cdfid, dim[0], (char *) NULL, &len);
#endif
        sprintf(buf, "%s", varname);
        xms = QString(buf);
        netcdf_listx_item->addItem(xms);
        //XmListAddItemUnselected(netcdf_listx_item, xms, 0);
        netcdf_listy_item->addItem(xms);
        //XmListAddItemUnselected(netcdf_listy_item, xms, 0);
        //XmStringFree(xms);
    }
#ifdef HAVE_NETCDF
    ncclose(cdfid);
#endif
    unset_wait_cursor();
    //}
}

void frmNetCDF::do_netcdf_proc(void)
{
    int setno;
    char xvar[256], yvar[256];
    QString *s=new QString(), cs;
    int *pos_list=new int[5];
    int j, pos_cnt, cnt, retval;
    char cstr[256];
    set_wait_cursor();
    /*
 * setno == -1, then next set
 */
    setno = -1;
    netcdf_listx_item->get_selection(&pos_cnt,&pos_list);
    if (pos_cnt) {//XmListGetSelectedPos(netcdf_listx_item, &pos_list, &pos_cnt)) {
        /*XtVaGetValues(netcdf_listx_item,
                      XmNselectedItemCount, &cnt,
                      XmNselectedItems, &s,
                      NULL);*/
        cnt=pos_cnt;
        sprintf(cstr,"%d",pos_list[0]);
        *s=QString(cstr);
        cs = *s;//XmStringCopy(*s);
        strcpy(cstr,cs.toAscii());
        //if ((cstr = GetStringSimple(cs))) {
        if (strlen(cstr)) {
            strcpy(xvar, cstr);
            //XtFree(cstr);
        }
        //XmStringFree(cs);
    } else {
        errmsg(tr("Need to select X, either variable name or INDEX").toAscii().constData());
        unset_wait_cursor();
        return;
    }
    netcdf_listy_item->get_selection(&pos_cnt,&pos_list);
    if (pos_cnt) {//XmListGetSelectedPos(netcdf_listy_item, &pos_list, &pos_cnt)) {
        j = pos_list[0];
        /*XtVaGetValues(netcdf_listy_item,
                      XmNselectedItemCount, &cnt,
                      XmNselectedItems, &s,
                      NULL);*/
        cnt=pos_cnt;
        sprintf(cstr,"%d",pos_list[0]);
        *s=QString(cstr);
        cs = *s;//XmStringCopy(*s);
        strcpy(cstr,cs.toAscii());
        //if ((cstr = GetStringSimple(cs))) {
        if (strlen(cstr)) {
            strcpy(yvar, cstr);
            //XtFree(cstr);
        }
        //XmStringFree(cs);
    } else {
        errmsg(tr("Need to select Y").toAscii().constData());
        unset_wait_cursor();
        return;
    }
    if (strcmp(xvar, "INDEX") == 0) {
#ifdef HAVE_NETCDF
        retval = readnetcdf(get_cg(), setno, fname, NULL, yvar, -1, -1, 1);
#endif
    } else {
#ifdef HAVE_NETCDF
        retval = readnetcdf(get_cg(), setno, fname, xvar, yvar, -1, -1, 1);
#endif
    }
    if (retval) {
        mainWin->mainArea->completeRedraw();//xdrawgraph();
    }
    delete[] pos_list;
    delete s;

    unset_wait_cursor();
}

char * frmNetCDF::getcdf_type(nc_type datatype)
{
    switch (datatype) {
    case NC_SHORT:
        return "NC_SHORT";
        break;
    case NC_LONG:
        return "NC_LONG";
        break;
    case NC_FLOAT:
        return "NC_FLOAT";
        break;
    case NC_DOUBLE:
        return "NC_DOUBLE";
        break;
    default:
        return "UNKNOWN (can't read this)";
        break;
    }
}

void frmNetCDF::do_netcdfquery_proc(void)
{
    char xvar[256], yvar[256];
    char buf[256], fname[512];
    QString *s, cs;
    int *pos_list=new int[5];
    int i, pos_cnt, cnt;
    char cstr[256];

    int cdfid;			/* netCDF id */
    nc_type datatype = 0;
    float f;
    double d;

    int x_id, y_id;
    nc_type xdatatype = 0;
    nc_type ydatatype = 0;
    int xndims, xdim[10], xnatts;
    int yndims, ydim[10], ynatts;
    long nx, ny;

    int atlen;
    char attname[256];
    char atcharval[256];

    ncopts = 0;			/* no crash on error */

    set_wait_cursor();

    strcpy(fname, xv_getstr(netcdf_file_item));
#ifdef HAVE_NETCDF
    if ((cdfid = ncopen(fname, NC_NOWRITE)) == -1) {
        errmsg(tr("Can't open file.").toAscii().constData());
        unset_wait_cursor();
        return;
    }
#endif
    netcdf_listx_item->get_selection(&pos_cnt,&pos_list);
    if (pos_cnt) {//XmListGetSelectedPos(netcdf_listx_item, &pos_list, &pos_cnt)) {
        /*XtVaGetValues(netcdf_listx_item,
                      XmNselectedItemCount, &cnt,
                      XmNselectedItems, &s,
                      NULL);*/
        cnt=pos_cnt;
        *s=pos_list[0];
        cs = *s;//XmStringCopy(*s);
        strcpy(cstr,cs.toAscii());
        if (strlen(cstr)) {//(cstr = GetStringSimple(cs))) {
            strcpy(xvar, cstr);
            //XtFree(cstr);
        }
        //XmStringFree(cs);
    } else {
        errmsg(tr("Need to select X, either variable name or INDEX").toAscii().constData());
        goto out1;
    }
    netcdf_listy_item->get_selection(&pos_cnt,&pos_list);
    if (pos_cnt) {//XmListGetSelectedPos(netcdf_listy_item, &pos_list, &pos_cnt)) {
        /*XtVaGetValues(netcdf_listy_item,
                      XmNselectedItemCount, &cnt,
                      XmNselectedItems, &s,
                      NULL);*/
        cnt=pos_cnt;
        *s=pos_list[0];
        cs = *s;//XmStringCopy(*s);
        strcpy(cstr,cs.toAscii());
        if (strlen(cstr)) {//(cstr = GetStringSimple(cs))) {
            strcpy(xvar, cstr);
            //XtFree(cstr);
        }
        //XmStringFree(cs);
    } else {
        errmsg(tr("Need to select Y").toAscii().constData());
        goto out1;
    }
    if (strcmp(xvar, "INDEX") == 0) {
        stufftext("X is the index of the Y variable\n");
    } else {
#ifdef HAVE_NETCDF
        if ((x_id = ncvarid(cdfid, xvar)) == -1) {
            char ebuf[256];
            sprintf(ebuf, "do_query(): %s %s%s",tr("No such variable").toAscii().constData(),xvar,tr(" for X").toAscii().constData());
            errmsg(ebuf);
            goto out1;
        }
        ncvarinq(cdfid, x_id, NULL, &xdatatype, &xndims, xdim, &xnatts);
        ncdiminq(cdfid, xdim[0], NULL, &nx);
#endif
        sprintf(buf, "X is %s, data type %s \t length [%ld]\n", xvar, getcdf_type(xdatatype), nx);
        stufftext(buf);
        sprintf(buf, "\t%d Attributes:\n", xnatts);
        stufftext(buf);
        for (i = 0; i < xnatts; i++) {
            atcharval[0] = 0;
#ifdef HAVE_NETCDF
            ncattname(cdfid, x_id, i, attname);
            ncattinq(cdfid, x_id, attname, &datatype, &atlen);
#endif
            switch (datatype) {
            case NC_CHAR:
#ifdef HAVE_NETCDF
                ncattget(cdfid, x_id, attname, (void *) atcharval);
#endif
                atcharval[atlen] = 0;
                sprintf(buf, "\t\t%s: %s\n", attname, atcharval);
                stufftext(buf);
                break;
            case NC_FLOAT:
#ifdef HAVE_NETCDF
                ncattget(cdfid, x_id, attname, (void *) &f);
#endif
                sprintf(buf, "\t\t%s: %f\n", attname, f);
                stufftext(buf);
                break;
            case NC_DOUBLE:
#ifdef HAVE_NETCDF
                ncattget(cdfid, x_id, attname, (void *) &d);
#endif
                sprintf(buf, "\t\t%s: %f\n", attname, d);
                stufftext(buf);
                break;
            default:
                break;
            }
        }
    }
#ifdef HAVE_NETCDF
    if ((y_id = ncvarid(cdfid, yvar)) == -1) {
        char ebuf[256];
        sprintf(ebuf, "do_query(): %s %s%s",tr("No such variable").toAscii().constData(),yvar,tr(" for Y").toAscii().constData());
        errmsg(ebuf);
        goto out1;
    }
    ncvarinq(cdfid, y_id, NULL, &ydatatype, &yndims, ydim, &ynatts);
    ncdiminq(cdfid, ydim[0], NULL, &ny);
#endif
    sprintf(buf, "Y is %s, data type %s \t length [%ld]\n", yvar, getcdf_type(ydatatype), ny);
    stufftext(buf);
    sprintf(buf, "\t%d Attributes:\n", ynatts);
    stufftext(buf);
    for (i = 0; i < ynatts; i++) {
        atcharval[0] = 0;
#ifdef HAVE_NETCDF
        ncattname(cdfid, y_id, i, attname);
        ncattinq(cdfid, y_id, attname, &datatype, &atlen);
#endif
        switch (datatype) {
        case NC_CHAR:
#ifdef HAVE_NETCDF
            ncattget(cdfid, y_id, attname, (void *) atcharval);
#endif
            atcharval[atlen] = 0;
            sprintf(buf, "\t\t%s: %s\n", attname, atcharval);
            stufftext(buf);
            break;
        case NC_FLOAT:
#ifdef HAVE_NETCDF
            ncattget(cdfid, y_id, attname, (void *) &f);
#endif
            sprintf(buf, "\t\t%s: %f\n", attname, f);
            stufftext(buf);
            break;
        case NC_DOUBLE:
#ifdef HAVE_NETCDF
            ncattget(cdfid, y_id, attname, (void *) &d);
#endif
            sprintf(buf, "\t\t%s: %f\n", attname, d);
            stufftext(buf);
            break;
        default:
            break;
        }
    }

out1:;
#ifdef HAVE_NETCDF
    ncclose(cdfid);
#endif
    stufftext("\n");
    delete[] pos_list;
    delete s;
    unset_wait_cursor();
}

/*binary format input*/
inputLine::inputLine(int t,QWidget * parent):QWidget(parent)
{
    char dummy[256];
    type=t;//type of input line: input from header(0) or input from data(1)
    layout=new QHBoxLayout;
    layout->setMargin(1);
    layout->setSpacing(1);
    lblOffset=new QLabel(QString("0:"),this);
    cmdNew=new QPushButton(QString("-"),this);
    if (type)
    {
        //lblOffset->setVisible(false);
        cmdNew->setVisible(false);
    }
    spnSize=new QSpinBox(this);
    spnSize->setMinimum(0);
    spnSize->setMaximum(10000);
    cmbFormat=new QComboBox(this);
    //Fill in formats
    for (int i=0;i<NUMBER_OF_COLUMN_FORMATS;i++)
    {
        if (i==0 || i==COLUMN_STRING)
            sprintf(dummy,"%s",binaryImportFormatName[i]);
        else
            sprintf(dummy,"%s (%d)",binaryImportFormatName[i],binaryImportFormat[i].size);
        cmbFormat->addItem(QString(dummy));
    }
    connect(cmbFormat,SIGNAL(currentIndexChanged(int)),SLOT(formatChanged(int)));
    cmbImportAs=new QComboBox(this);
    //Fill in targets
    int index=0;
    for (int i=0;i<NUMBER_OF_IMPORT_DESTINATIONS;i++)
    {
        if (ImportDestinationType[i]&(type+1))
        {
            cmbImportAs->addItem(QString(ImportDestinationName[i]));
            datas[index++]=i;
        }
    }
    layout->addWidget(lblOffset);
    layout->addWidget(cmbFormat);
    layout->addWidget(spnSize);
    spnSize->setEnabled(false);
    layout->addWidget(cmbImportAs);
    layout->addWidget(cmdNew);
    setLayout(layout);
    cmbFormat->setCurrentIndex(10);//set Standard-Format to double
}

int inputLine::getSize(void)
{
    if (cmbFormat->currentIndex()==0 || cmbFormat->currentIndex()==COLUMN_STRING)
        return spnSize->value();
    else
        return binaryImportFormat[cmbFormat->currentIndex()].size;
}

int inputLine::getType(void)
{
    return cmbFormat->currentIndex();
}

int inputLine::getTarget(void)
{
    return datas[cmbImportAs->currentIndex()];
}

void inputLine::formatChanged(int i)
{
    spnSize->setEnabled(!((bool)i));
    if (i==COLUMN_STRING) spnSize->setEnabled(true);
    cmbImportAs->setEnabled((bool)i);
    /*if (i==0)
{
}
else
{
}*/
}

pageHeaderInfo::pageHeaderInfo(QWidget * parent):QWidget(parent)
{
    grid=new QVBoxLayout;
    grid->setMargin(STD_MARGIN);
    grid->setSpacing(STD_SPACING);
    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->setSpacing(STD_SPACING);
    empty0=new QWidget;
    empty1=new QWidget;
    QHBoxLayout * layout0=new QHBoxLayout;
    layout0->setMargin(STD_MARGIN);
    layout0->setSpacing(STD_SPACING);
    QHBoxLayout * layout1=new QHBoxLayout;
    layout1->setMargin(STD_MARGIN);
    layout1->setSpacing(STD_SPACING);
    empty0->setLayout(layout0);
    empty1->setLayout(layout1);
    empty=new QWidget;
    scroll=new QScrollArea(this);
    scroll->setWidget(empty);
    cmdAdd=new QPushButton(QString("+"),this);
    connect(cmdAdd,SIGNAL(clicked()),SLOT(doNew()));
    Headers=new QLabel*[5];
    for (int i=0;i<5;i++)
    {
        Headers[i]=new QLabel(QString(""),this);
        layout0->addWidget(Headers[i]);
    }
    Headers[0]->setText(QString("Offset"));
    Headers[1]->setText(QString("Format (Bytes)"));
    Headers[2]->setText(QString("Bytes"));
    Headers[3]->setText(QString("InputAs"));
    Headers[4]->setText(QString("Remove"));
    grid->addWidget(empty0);
    grid->addWidget(scroll);
    lblEndChar=new QLabel(QString("String-End-Char (Ascii-code):"),this);
    layout1->addWidget(lblEndChar);
    lenEndChar=new QLineEdit(QString("0"),this);
    layout1->addWidget(lenEndChar);
    cmdTestLoad=new QPushButton(QString("TestLoad"),this);
    layout1->addWidget(cmdTestLoad);
    connect(cmdTestLoad,SIGNAL(clicked()),SLOT(doTestLoad()));
    grid->addWidget(empty1);
    setLayout(grid);
    map=new QSignalMapper;
    number_of_lines=1;
    inFormats=new inputLine*[number_of_lines];
    inFormats[0]=new inputLine(0,this);
    inFormats[0]->cmdNew->setEnabled(false);//never delete first line
    connect(inFormats[0]->cmdNew,SIGNAL(clicked()),map,SLOT(map()));
    connect(inFormats[0]->cmbFormat,SIGNAL(currentIndexChanged(int)),SLOT(offsetChanged(int)));
    connect(inFormats[0]->spnSize,SIGNAL(valueChanged(int)),SLOT(offsetChanged(int)));
    map->setMapping(inFormats[0]->cmdNew,0);
    connect(map,SIGNAL(mapped(int)),SLOT(doDelete(int)));
    layout->addWidget(inFormats[0]);
    layout->addWidget(cmdAdd);
    empty->setLayout(layout);
    empty->setGeometry(0,0,500,35*(number_of_lines+1));

    nr_of_entries=0;
    for (int i=0;i<NUMBER_OF_IMPORT_DESTINATIONS;i++)
    {
        if (ImportDestinationType[i]&1)
        {
            entries[nr_of_entries]=QString(ImportDestinationName[i]);
            entry_values[nr_of_entries++]=ImportDestination[i];
        }
    }

    nr_of_sels=0;
    cmdReadIni=new QPushButton(tr("Read Ini File"),this);
    connect(cmdReadIni,SIGNAL(clicked()),SLOT(doReadIni()));
    cmdReadIni->setVisible(false);
    scroll2=new QScrollArea;
    empty2=new QWidget;
    layout2=new QVBoxLayout;
    layout2->setMargin(STD_MARGIN);
    layout2->setSpacing(STD_SPACING);
    empty2->setLayout(layout2);

    scroll2->setWidget(empty2);
    scroll2->setVisible(false);
    grid->addWidget(cmdReadIni);
    grid->addWidget(scroll2);
}

void pageHeaderInfo::changeRepresentation(int r)
{
    if (r==0)//binary-Header
    {
        cmdReadIni->setVisible(false);
        /*for (int i=0;i<nr_of_sels;i++)
sels[i]->setVisible(false);*/
        scroll2->setVisible(false);
        scroll->setVisible(true);
        empty0->setVisible(true);
        empty1->setVisible(true);
    }
    else//ini-Header
    {
        cmdReadIni->setVisible(true);
        /*for (int i=0;i<nr_of_sels;i++)
sels[i]->setVisible(true);*/
        scroll2->setVisible(true);
        scroll->setVisible(false);
        empty0->setVisible(false);
        empty1->setVisible(false);
    }
}

void pageHeaderInfo::doReadIni(void)
{
    if (nr_of_sels>0)
    {
        for (int i=nr_of_sels-1;i>=0;i--)
        {
            delete sels[i];
        }
        delete[] sels;
        delete[] readValues;
        par_wid->tabHeader->nr_of_sels=0;
    }
    par_wid->initSettings(par_wid->imp_set);
    par_wid->transmitInfos();
    if (par_wid->headerFileName[0]!='\0')
    {
        QSettings settings(par_wid->headerFileName,QSettings::IniFormat);
        QStringList keys=settings.allKeys();
        QStringList val3;
        QString val,tval,tval2;
        QVariant val2;
        nr_of_sels=keys.length();
        if (nr_of_sels>0)
        {
            readValues=new QString[nr_of_sels];
            sels=new StdSelector*[nr_of_sels];
            for (int i=0;i<nr_of_sels;i++)
            {
                val2=settings.value(keys.at(i));
                val=val2.toString();
                val3=val2.toStringList();
                if (val2.type()==QVariant::String)
                {
                    tval=val;
                }
                else if (val2.type()==QVariant::StringList)
                {
                    tval=val3.at(0)+QString(".")+val3.at(1);
                }
                else
                {
                    tval=tr("unidentified");
                }
                readValues[i]=tval;
                tval2=tr("Import key [ ") + keys.at(i) + QString(" ]=( ") + readValues[i] + tr(" ) as ");
                sels[i]=new StdSelector(this,tval2,nr_of_entries,entries);
                sels[i]->setValues(entry_values);
                sels[i]->setVisible(true);
                layout2->addWidget(sels[i]);
            }
            int wid=600;
            if (scroll2->width()>610) wid=scroll2->width()-10;
            empty2->setGeometry(0,0,wid,35*nr_of_sels);
        }
    }
    else
    {
        errwin(tr("No ini-file set!").toAscii().constData());
    }
}

void pageHeaderInfo::doTestLoad(void)
{
    par_wid->determine_string_size=true;
    //this is for reading the header only and importing all the informations to check whether the file-structure matches the informations set
    emit(readHeader());
}

void pageHeaderInfo::updateOffsets(void)
{
    int offset=0;
    char dummy[32];
    for (int i=0;i<number_of_lines;i++)
    {
        sprintf(dummy,"%d:",offset);
        inFormats[i]->lblOffset->setText(QString(dummy));
        offset+=inFormats[i]->getSize();
    }
}

void pageHeaderInfo::doNew(void)
{
    inputLine ** nlines=new inputLine*[number_of_lines+1];
    for (int i=0;i<number_of_lines;i++)
        nlines[i]=inFormats[i];
    nlines[number_of_lines]=new inputLine(0,this);
    connect(nlines[number_of_lines]->cmdNew,SIGNAL(clicked()),map,SLOT(map()));
    connect(nlines[number_of_lines]->cmbFormat,SIGNAL(currentIndexChanged(int)),SLOT(offsetChanged(int)));
    connect(nlines[number_of_lines]->spnSize,SIGNAL(valueChanged(int)),SLOT(offsetChanged(int)));
    map->setMapping(nlines[number_of_lines]->cmdNew,number_of_lines);
    layout->insertWidget(number_of_lines,nlines[number_of_lines]);
    number_of_lines++;
    delete[] inFormats;
    inFormats=nlines;
    empty->setGeometry(0,0,500,35*(number_of_lines+1));
    updateOffsets();
}

void pageHeaderInfo::doDelete(int j)
{
    inputLine ** nlines=new inputLine*[number_of_lines-1];
    int ofs=0;
    for (int i=0;i<number_of_lines;i++)
    {
        if (i!=j)
            nlines[ofs++]=inFormats[i];
        map->removeMappings(inFormats[i]);
    }
    layout->removeWidget(inFormats[j]);
    delete inFormats[j];
    delete[] inFormats;
    inFormats=nlines;
    number_of_lines--;
    empty->setGeometry(0,0,500,35*(number_of_lines+1));
    for (int i=0;i<number_of_lines;i++)
    {
        connect(inFormats[i]->cmdNew,SIGNAL(clicked()),map,SLOT(map()));
        map->setMapping(inFormats[i]->cmdNew,i);
    }
    updateOffsets();
}

void pageHeaderInfo::offsetChanged(int i)
{
    updateOffsets();
}

pageDataInfo::pageDataInfo(QWidget * parent):QWidget(parent)
{
    grid=new QGridLayout;
    grid->setMargin(STD_MARGIN);
    grid->setSpacing(STD_SPACING);
    empty=new QWidget;
    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->setSpacing(STD_SPACING);
    empty->setLayout(layout);
    lblComment=new QLabel(tr("Structure of Dataset in binary file:"),this);
    lblChannelCount=new QLabel(tr("Channel-Count (number of sets):"),this);
    spnChannelCount=new QSpinBox(this);
    spnChannelCount->setMinimum(1);
    spnChannelCount->setMaximum(1000);
    spnChannelCount->setValue(1);
    connect(spnChannelCount,SIGNAL(valueChanged(int)),SLOT(channelCountChanged(int)));
    lblDataSetCount=new QLabel(tr("Data-Set-Count (setlength):"),this);
    spnDataSetCount=new QSpinBox(this);
    spnDataSetCount->setMinimum(0);
    spnDataSetCount->setMaximum(10000000);
    scroll=new QScrollArea;
    scroll->setWidget(empty);
    number_of_lines=1;
    inFormats=new inputLine*[number_of_lines];
    inFormats[0]=new inputLine(1,this);
    layout->addWidget(inFormats[0]);
    empty->setGeometry(0,0,500,35);
    chkReadToEOF=new QCheckBox(tr("Read Data until EOF"),this);
    chkKeppTrigger=new QCheckBox(tr("KeepTriggerChannel"),this);
    connect(chkReadToEOF,SIGNAL(toggled(bool)),SLOT(eofToggled(bool)));
    int nr_of_entries=2;
    QString * entries=new QString[2];
    entries[0]=QString("a0 b0 c0 | a1 b1 c1 | ... | an bn cn");
    entries[1]=QString("a0 a1 ... an | b0 b1 ... bn | c0 c1 ... cn");
    selOrder=new StdSelector(this,tr("Channel ordering in file:"),nr_of_entries,entries);
    grid->addWidget(lblChannelCount,0,0);
    grid->addWidget(spnChannelCount,0,1);
    grid->addWidget(lblDataSetCount,1,0);
    grid->addWidget(spnDataSetCount,1,1);
    grid->addWidget(selOrder,2,0,1,2);
    grid->addWidget(lblComment,3,0,1,2);
    grid->addWidget(scroll,4,0,1,2);
    grid->addWidget(chkReadToEOF,5,0,1,1);
    grid->addWidget(chkKeppTrigger,5,1,1,1);
    setLayout(grid);
}

void pageDataInfo::channelCountChanged(int i)
{
    inputLine ** inLines=new inputLine*[i];
    char dummy[32];
    if (i>number_of_lines)//new line
    {
        for (int j=0;j<number_of_lines;j++)
        {
            inLines[j]=inFormats[j];
        }
        for (int j=number_of_lines;j<i;j++)
        {
            inLines[j]=new inputLine(1,this);
            layout->addWidget(inLines[j]);
            sprintf(dummy,"%d:",j);
            inLines[j]->lblOffset->setText(QString(dummy));
        }
        delete[] inFormats;
        inFormats=inLines;
    }
    else if(i<number_of_lines)
    {
        for (int j=i;j<number_of_lines;j++)
            delete  inFormats[j];
        delete[] inLines;
    }

    number_of_lines=i;
    empty->setGeometry(0,0,500,35*number_of_lines);

    emit(newChannelCount(i));
}

void pageDataInfo::eofToggled(bool i)
{
    lblDataSetCount->setEnabled(!i);
    spnDataSetCount->setEnabled(!i);
}

void pageDataInfo::readDataSettings(importSettings & imp_set)
{
    if (imp_set.channel_format!=NULL) delete[] imp_set.channel_format;
    if (imp_set.channel_size!=NULL) delete[] imp_set.channel_size;
    if (imp_set.channel_target!=NULL) delete[] imp_set.channel_target;
    imp_set.channels=spnChannelCount->value();
    if (chkReadToEOF->isChecked()) imp_set.points=-1;
    else imp_set.points=spnDataSetCount->value();
    imp_set.channel_target=new int[imp_set.channels];
    imp_set.channel_size=new int[imp_set.channels];
    imp_set.channel_format=new int[imp_set.channels];
    for (int i=0;i<imp_set.channels;i++)
    {
        imp_set.channel_target[i]=inFormats[i]->getTarget();
        imp_set.channel_size[i]=inFormats[i]->getSize();
        imp_set.channel_format[i]=inFormats[i]->getType();
    }

}

pageFileInfo::pageFileInfo(QWidget * parent):QWidget(parent)
{
    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->setSpacing(STD_SPACING);
    lblText=new QLabel(tr("Informations about file / Informations read from header"),this);
    lenText=new QTextEdit(QString(""),this);
    lenText->setReadOnly(true);
    lenText->setWordWrapMode(QTextOption::NoWrap);
    layout->addWidget(lblText);
    layout->addWidget(lenText);
    setLayout(layout);
    headerContents=QString("");
    DatFile=NULL;
    HeaderFile=NULL;
}

void pageFileInfo::showEvent(QShowEvent * event)
{
    emit(requestInfos());
    qApp->processEvents();
    ShowInfos();
}

void pageFileInfo::ShowInfos(void)
{
    lenText->clear();
    char dummy[256];
    if (DatFile)
    {
        if (DatFile==HeaderFile)
            lenText->append(QString("Headerfile=Datafile=\"")+QString(DatFile)+QString("\""));
        else
            lenText->append(QString("Datafile=\"")+QString(DatFile)+QString("\""));
        QFileInfo * dfi=new QFileInfo(QString(DatFile));
        if (dfi->exists())
        {
            sprintf(dummy,"FileSize= %ld Bytes",(long)dfi->size());
            lenText->append(QString(dummy));
        }
        else
        {
            lenText->append(tr("This File does not exist!"));
        }
    }
    else
    {
        if (DatFile==HeaderFile)
            lenText->append(tr("No Datafile and Headerfile selected!"));
        else
            lenText->append(tr("No Datafile selected!"));
    }
    if (DatFile!=HeaderFile)
    {
        lenText->append(QString(" "));
        if (HeaderFile)
        {
            lenText->append(QString("Headerfile=\"")+QString(HeaderFile)+QString("\""));
            QFileInfo * hfi=new QFileInfo(QString(HeaderFile));
            if (hfi->exists())
            {
                sprintf(dummy,"Filesize= %ld Bytes",(long)hfi->size());
                lenText->append(QString(dummy));
            }
            else
            {
                lenText->append(tr("This File does not exist!"));
            }
        }
        else
            lenText->append(tr("No Headerfile selected!"));
    }
    lenText->append(QString(" "));
    lenText->append(headerContents);
}

pageImportInfo::pageImportInfo(QWidget * parent):QWidget(parent)
{
    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->setSpacing(STD_SPACING);
    selType=new stdSetTypeSelector(this);
    selType->setShowAll(true);
    selType->lblText->setText(tr("Set-Type for imported data:"));
    lblImportTo=new QLabel(tr("Read to graph:"),this);
    importGraph=new uniList(GRAPHLIST,this);
    int number=4;
    QString entr[4];
    entr[0]=tr("None");
    entr[1]=tr("X");
    entr[2]=tr("Y");
    entr[3]=tr("XY");
    selAutoscale=new StdSelector(this,tr("Autoscale on read:"),number,entr);
    selAutoscale->setCurrentIndex(3);
    layout->addWidget(selType);
    layout->addWidget(selAutoscale);
    layout->addWidget(lblImportTo);
    layout->addWidget(importGraph);
    setLayout(layout);
}

void pageImportInfo::newChannelCount(int i)
{

}

void frmBinaryFormatInput::initSettings(struct importSettings & iset)
{
    if (iset.title)
        delete[] iset.title;
    if (iset.subtitle)
        delete[] iset.subtitle;
    if (iset.x_title)
        delete[] iset.x_title;
    if (iset.y_title)
        delete[] iset.y_title;
    if (iset.first_data)
        delete[] iset.first_data;
    if (iset.channel_format)
        delete[] iset.channel_format;
    if (iset.channel_size)
        delete[] iset.channel_size;
    if (iset.channel_target)
        delete[] iset.channel_target;
    if (iset.set_title)
        delete[] iset.set_title;
    iset.channel_format=NULL;
    iset.channel_size=NULL;
    iset.channel_target=NULL;
    iset.first_data=NULL;
    iset.title=NULL;
    iset.subtitle=NULL;
    iset.x_title=NULL;
    iset.y_title=NULL;
    iset.set_title=NULL;
    iset.first_data=NULL;

    iset.read_to_eof=false;
    iset.string_end_char=0;
    imp_set.setorder=0;
    iset.x0set=iset.deltaxset=iset.fset=false;
    /*iset.title=new char[2];
    iset.subtitle=new char[2];*/
    iset.x0=-1.0;//negative values suggest, that this value has not been set
    iset.deltax=-1.0;
    iset.f=-1.0;
    iset.bytesize=-1;
    iset.bitsize=-1;
    iset.whole_size=-1;
    iset.single_size=-1;
    iset.channels=-1;
    iset.points=-1;
    for (int i=0;i<7;i++)
        iset.factors[i]=1.0;
    /*iset.x_title=new char[2];
    iset.y_title=new char[2];
    iset.title[0]=iset.subtitle[0]=iset.y_title[0]=iset.x_title[0]='\0';*/
    iset.target_gno=0;
    iset.set_type=1;

    iset.setorder=tabDataInfo->selOrder->currentIndex();
    char dummy[8];
    strncpy(dummy,tabHeader->lenEndChar->text().toAscii(),2);
    iset.string_end_char=dummy[0];
    iset.read_to_eof=tabDataInfo->chkReadToEOF->isChecked();
}

frmBinaryFormatInput::frmBinaryFormatInput(QWidget * parent):QDialog(parent)
{
    imp_set.title=new char[2];
    imp_set.subtitle=new char[2];
    imp_set.x_title=new char[2];
    imp_set.y_title=new char[2];
    imp_set.set_title=new char[2];
    imp_set.first_data=new double*[2];
    imp_set.channel_format=new int[2];
    imp_set.channel_size=new int[2];
    imp_set.channel_target=new int[2];

    determine_string_size=auto_transfer_from_header=true;

    setWindowTitle(tr("qtGrace: Binary file input"));
    grid=new QGridLayout;
    grid->setMargin(STD_MARGIN);
    grid->setSpacing(STD_SPACING);
    QHBoxLayout * layout=new QHBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->setSpacing(STD_SPACING);
    QWidget * empty=new QWidget;
    QWidget * empty2=new QWidget;
    QHBoxLayout * layout2=new QHBoxLayout;
    layout2->setMargin(STD_MARGIN);
    layout2->setSpacing(STD_SPACING);
    QLabel * lblempty=new QLabel(QString(""),this);
    layout2->addWidget(lblempty);
    empty2->setLayout(layout2);

    cmdSave=new QPushButton(tr("Save File Format"),this);
    connect(cmdSave,SIGNAL(clicked()),SLOT(doSaveFileFormat()));
    cmdLoad=new QPushButton(tr("Load File Format"),this);
    connect(cmdLoad,SIGNAL(clicked()),SLOT(doLoadFileFormat()));

    lblFormatSource=new QLabel(tr("Format Source:"),this);
    cmbFormatSource=new QComboBox(this);
    cmbFormatSource->addItem(tr("manual"));
    cmbFormatSource->addItem(tr("Datafile"));
    cmbFormatSource->addItem(tr("seperate file"));
    cmbFormatSource->setCurrentIndex(1);
    connect(cmbFormatSource,SIGNAL(currentIndexChanged(int)),SLOT(formatSourceChanged(int)));
    lblDataFile=new QLabel(tr("Datafile:"),this);
    lenDataFile=new QLineEdit(QString(""),this);
    lblHeaderFile=new QLabel(tr("Headerfile:"),this);
    lenHeaderFile=new QLineEdit(QString(""),this);

    chkHeader=new QCheckBox(tr("Header informations present"),this);
    chkHeader->setChecked(true);
    connect(chkHeader,SIGNAL(toggled(bool)),SLOT(headerToggled(bool)));
    lblHeaderFileFormat=new QLabel(tr("Headerfile-Format:"),this);
    cmbHeaderFileFormat=new QComboBox(this);
    cmbHeaderFileFormat->addItem(tr("binary-file"));
    cmbHeaderFileFormat->addItem(tr("ini-file"));
    connect(cmbHeaderFileFormat,SIGNAL(currentIndexChanged(int)),SLOT(HeaderFormatChanged(int)));

    tabs=new QTabWidget;

    tabHeader=new pageHeaderInfo(this);
    tabHeader->par_wid=this;
    tabDataInfo=new pageDataInfo(this);
    tabFileInfo=new pageFileInfo(this);
    tabImportInfo=new pageImportInfo(this);
    connect(tabHeader,SIGNAL(readHeader()),this,SLOT(readHeader()));
    connect(tabFileInfo,SIGNAL(requestInfos()),SLOT(transmitInfos()));
    tabs->addTab(tabHeader,tr("Header-Format"));
    tabs->addTab(tabFileInfo,tr("File-Info"));
    tabs->addTab(tabDataInfo,tr("Data-Format"));
    tabs->addTab(tabImportInfo,tr("Import"));

    aac=new stdButtonGroup(this,true,true,false);
    connect(aac->cmdApply,SIGNAL(clicked()),SLOT(doOK()));
    connect(aac->cmdAccept,SIGNAL(clicked()),SLOT(doAccept()));
    connect(aac->cmdClose,SIGNAL(clicked()),SLOT(doClose()));

    cmdSelectDataFile=new QPushButton(tr("Browse..."),this);
    connect(cmdSelectDataFile,SIGNAL(clicked()),SLOT(SelectDataFile()));
    cmdSelectHeaderFile=new QPushButton(tr("Browse..."),this);
    connect(cmdSelectHeaderFile,SIGNAL(clicked()),SLOT(SelectHeaderFile()));

    layout->addWidget(cmdLoad);
    layout->addWidget(cmdSave);
    empty->setLayout(layout);

    grid->addWidget(empty,0,0,1,3);
    grid->addWidget(chkHeader,1,0,1,2);
    grid->addWidget(lblFormatSource,2,0);
    grid->addWidget(cmbFormatSource,2,1);
    grid->addWidget(lblDataFile,3,0);
    grid->addWidget(lenDataFile,3,1);
    grid->addWidget(cmdSelectDataFile,3,2);
    grid->addWidget(lblHeaderFile,4,0);
    grid->addWidget(lenHeaderFile,4,1);
    grid->addWidget(cmdSelectHeaderFile,4,2);
    grid->addWidget(lblHeaderFileFormat,5,0);
    grid->addWidget(cmbHeaderFileFormat,5,1);
    grid->addWidget(empty2,6,0,1,3);
    grid->addWidget(tabs,7,0,1,3);
    grid->addWidget(aac,8,0,1,3);
    setLayout(grid);
    cmbFormatSource->setCurrentIndex(1);
    formatSourceChanged(1);
    initSettings(imp_set);
}

void frmBinaryFormatInput::HeaderFormatChanged(int i)
{
    tabHeader->changeRepresentation(i);
}

void frmBinaryFormatInput::doSaveFileFormat(void)
{
    QString str=QFileDialog::getSaveFileName(this,tr("Save Format Info File"));
    if (!str.isEmpty())
    {
        char dummy[512];
        strcpy(dummy,str.toAscii());
        ofstream ofi;
        ofi.open(dummy);
        ofi << "Binary_Format_Info_File_V 1" << endl;
        ofi << (chkHeader->isChecked()==true?1:0) << endl;
        ofi << cmbFormatSource->currentIndex() << endl;
        /*//Don't save the file names, we only save the file format!
strcpy(dummy,lenDataFile->text().toAscii());
ofi << dummy << endl;
strcpy(dummy,lenHeaderFile->text().toAscii());
ofi << dummy << endl;*/
        ofi << cmbHeaderFileFormat->currentIndex() << endl;
        strcpy(dummy,tabHeader->lenEndChar->text().toAscii());
        ofi << dummy << endl;
        ofi << tabHeader->number_of_lines << endl;
        for (int i=0;i<tabHeader->number_of_lines;i++)
        {
            ofi << tabHeader->inFormats[i]->cmbFormat->currentIndex() << endl;
            ofi << tabHeader->inFormats[i]->spnSize->value() << endl;
            ofi << tabHeader->inFormats[i]->cmbImportAs->currentIndex() << endl;
        }
        ofi << (tabDataInfo->chkReadToEOF->isChecked()==true?1:0) << endl;
        ofi << tabDataInfo->spnChannelCount->value() << endl;
        ofi << tabDataInfo->spnDataSetCount->value() << endl;
        ofi << tabDataInfo->selOrder->currentIndex() << endl;
        ofi << tabDataInfo->number_of_lines << endl;
        for (int i=0;i<tabDataInfo->number_of_lines;i++)
        {
            ofi << tabDataInfo->inFormats[i]->cmbFormat->currentIndex() << endl;
            ofi << tabDataInfo->inFormats[i]->spnSize->value() << endl;
            ofi << tabDataInfo->inFormats[i]->cmbImportAs->currentIndex() << endl;
        }
        ofi << tabImportInfo->selType->currentValue() << endl;
        ofi.close();
    }
}

void frmBinaryFormatInput::doLoadFileFormat(void)
{
    QString str=QFileDialog::getOpenFileName(this,tr("Load Format Info File"));
    if (!str.isEmpty())
    {
        int version;
        int nr_of_channels,nr_of_datasets,nr_of_lines;
        int dint;
        char dummy[512];
        strcpy(dummy,str.toAscii());
        ifstream ifi;
        ifi.open(dummy);
        ifi >> dummy >> version;
        //cout << "dummy=#" << dummy << "# version=" << version << endl;
        if (strcmp(dummy,"Binary_Format_Info_File_V"))
        {
            errwin(tr("File not a Grace-format description!").toAscii().constData());
            ifi.close();
            return;
        }
        ifi >> dint;
        chkHeader->setChecked((bool)dint);
        ifi >> dint;
        cmbFormatSource->setCurrentIndex(dint);
        ifi >> dint;
        cmbHeaderFileFormat->setCurrentIndex(dint);
        ifi >> dummy;
        //cout << "dummy=#" << dummy << "#" << endl;
        tabHeader->lenEndChar->setText(QString(dummy));
        ifi >> nr_of_lines;
        //cout << "nr_of_lines=" << nr_of_lines << endl;
        while (tabHeader->number_of_lines<nr_of_lines)
        {
            //cout << "tabHeader_nroflines=" << tabHeader->number_of_lines << endl;
            tabHeader->doNew();
        }
        while (tabHeader->number_of_lines>nr_of_lines)
        {
            //cout << "tabHeader_nroflines=" << tabHeader->number_of_lines << endl;
            tabHeader->doDelete(tabHeader->number_of_lines-1);
        }
        for (int i=0;i<nr_of_lines;i++)
        {
            ifi >> dint;
            tabHeader->inFormats[i]->cmbFormat->setCurrentIndex(dint);
            ifi >> dint;
            tabHeader->inFormats[i]->spnSize->setValue(dint);
            ifi >> dint;
            tabHeader->inFormats[i]->cmbImportAs->setCurrentIndex(dint);
        }
        ifi >> dint;
        tabDataInfo->chkReadToEOF->setChecked((bool)dint);
        ifi >> nr_of_channels;
        tabDataInfo->spnChannelCount->setValue(nr_of_channels);
        ifi >> nr_of_datasets;
        tabDataInfo->spnDataSetCount->setValue(nr_of_datasets);
        ifi >> dint;
        tabDataInfo->selOrder->setCurrentIndex(dint);
        ifi >> nr_of_lines;
        //cout << "nr_of_lines=" << nr_of_lines << endl;
        //cout << "tabDataInfo->number_of_lines=" << tabDataInfo->number_of_lines << endl;
        tabDataInfo->channelCountChanged(nr_of_lines);
        for (int i=0;i<nr_of_lines;i++)
        {
            ifi >> dint;
            tabDataInfo->inFormats[i]->cmbFormat->setCurrentIndex(dint);
            ifi >> dint;
            tabDataInfo->inFormats[i]->spnSize->setValue(dint);
            ifi >> dint;
            tabDataInfo->inFormats[i]->cmbImportAs->setCurrentIndex(dint);
        }
        ifi >> dint;
        if (nr_of_channels<=1)
            tabImportInfo->selType->setColumnCount(2);
        else
            tabImportInfo->selType->setColumnCount(nr_of_channels);
        tabImportInfo->selType->setCurrentValue(dint);
        ifi.close();

    }//end of !str.isEmpty()
}

void frmBinaryFormatInput::SelectDataFile(void)
{
    //QString str=QFileDialog::getOpenFileName(this,tr("Select Data File"));
    QStringList str=QFileDialog::getOpenFileNames(this,tr("Select Data File(s)"));
    //if (!str.isEmpty())
    if (str.length()>0)
    {
        QString filesText=str.join(QString(";"));
        lenDataFile->setText(filesText);
    }
}

void frmBinaryFormatInput::SelectHeaderFile(void)
{
    QString str=QFileDialog::getOpenFileName(this,tr("Select Header File"));
    if (!str.isEmpty())
    {
        lenHeaderFile->setText(str);
    }
}

void frmBinaryFormatInput::formatSourceChanged(int i)
{
    if (i==0 || i==1)
    {
        lblHeaderFile->setEnabled(false);
        lenHeaderFile->setEnabled(false);
        lblHeaderFileFormat->setEnabled(false);
        cmbHeaderFileFormat->setEnabled(false);
        if (cmbHeaderFileFormat->currentIndex()==1)
            cmbHeaderFileFormat->setCurrentIndex(0);
        cmdSelectHeaderFile->setEnabled(false);
    }
    else if (i==2)
    {
        lblHeaderFile->setEnabled(true);
        lenHeaderFile->setEnabled(true);
        lblHeaderFileFormat->setEnabled(true);
        cmbHeaderFileFormat->setEnabled(true);
        cmdSelectHeaderFile->setEnabled(true);
    }
}

void frmBinaryFormatInput::init(void)
{
    update_all();
    int nr=1,*sel=new int[2];
    sel[0]=get_cg();
    tabImportInfo->importGraph->set_new_selection(nr,sel);
}

void frmBinaryFormatInput::doOK(void)
{
    ApplyError=false;
    int target_gno;
    int nr_sel,*selection=new int[2];
    int * gnos=NULL;
    int * snos=NULL;
    int max_nr_of_sets,setno,ret;
    int collect_nr_of_sets;
    int * col_gnos=NULL;
    int * col_snos=NULL;
    ifstream ifi;

    determine_string_size=false;
    tabImportInfo->importGraph->get_selection(&nr_sel,&selection);
    if (nr_sel!=1)
    {
        errwin(tr("Please select a single graph to import binary data.").toAscii().constData());
        ApplyError=true;
    }
    else
    {
        target_gno=selection[0];

        ///Undo-Stuff
        SaveSetStatesPrevious(0,selection,selection,UNDO_COMPLETE);//all sets will be new --> no previous sets --> 0

        auto_transfer_from_header=false;
        readHeader();

        if (datFileName[0]=='\0')
        {
            errwin(tr("No binary input file selected.").toAscii().constData());
            ApplyError=true;
            return;
        }

        tabDataInfo->readDataSettings(imp_set);

        bool contains_trigger=false;
        for (int i=0;i<imp_set.channels;i++)
        {
            //cout << "i=" << i << " target=" << imp_set.channel_target[i] << endl;
            if (imp_set.channel_target[i]==IMPORT_TO_TRIGGER)
            {
                contains_trigger=true;
                break;
            }
        }
        if (contains_trigger==true)
        {
            bool ok;
            imp_set.triggervalue=QInputDialog::getDouble(this,tr("Trigger"),tr("Enter trigger-level:"),0.5,-2147483647,2147483647,3,&ok);
            if (ok==false) imp_set.triggervalue=0.5;
        }
        set_wait_cursor();

        collect_nr_of_sets=0;
        for (int kkk=0;kkk<datFileNames.length();kkk++)//go through all selected files
        {
            strcpy(datFileName,datFileNames.at(kkk).toAscii().constData());

            if (cmbFormatSource->currentIndex()==1)//header in datafile
            {
                strcpy(headerFileName,datFileName);
            }

            QFileInfo info(datFileName);

            if (info.isReadable()==false)
            {
                QString dummy;
                dummy=tr("Selected binary input file (")+QString(datFileName)+tr(") is unreadable. Please select a suitable input file.");
                errwin(dummy.toAscii().constData());
                ApplyError=true;
                continue;
            }

            if (cmbHeaderFileFormat->currentIndex()==1)//ini-file
                ifi.open(headerFileName);
            else
                ifi.open(headerFileName, ios::binary );
            //read data from header to imp_set
            if (chkHeader->isChecked()==true)//read only if there is really a header
                doReadDataFromHeader(ifi);
            /*
imp_set.channels=tabDataInfo->spnChannelCount->value();
if (tabDataInfo->chkReadToEOF->isChecked()) imp_set.points=-1;
else imp_set.points=tabDataInfo->spnDataSetCount->value();

imp_set.channel_target=new int[imp_set.channels];
imp_set.channel_size=new int[imp_set.channels];
imp_set.channel_format=new int[imp_set.channels];
for (int i=0;i<imp_set.channels;i++)
{
imp_set.channel_target[i]=tabDataInfo->inFormats[i]->getTarget();
imp_set.channel_size[i]=tabDataInfo->inFormats[i]->getSize();
imp_set.channel_format[i]=tabDataInfo->inFormats[i]->getType();
}
*/
            tabDataInfo->readDataSettings(imp_set);

            int columns_read,points_read;

            if (cmbFormatSource->currentIndex()==2)//header is a seperate file
            {
                ifi.close();
                ifi.open(datFileName, ios::binary );
            }//0-->manual, 1-->datafile=headerfile
            if (chkHeader->isChecked()==false)//no header
            {
                ifi.open(datFileName, ios::binary );
                //ifi.seekg(0,ios_base::beg);
            }

            //if header is part of the file, then the header is already read here --> we are at the beginning of the data-stream now
            readBinaryFromFile(ifi,imp_set,&imp_set.first_data,&columns_read,&points_read);//does the actual read operations --> now we have to enter the data into the sets
            //cout << "have read: columns=" << columns_read << " points=" << points_read << endl;
            ifi.close();

            int number_of_sets_with_column[6]={0,0,0,0,0,0};
            int number_of_x_columns=0;
            int triggerChannel=-1;
            int triggerSet=-1;
            for (int i=0;i<imp_set.channels;i++)
            {
                if (imp_set.channel_target[i]!=IMPORT_TO_NONE)
                {
                    if (imp_set.channel_target[i]==IMPORT_TO_X)
                        number_of_sets_with_column[0]++;
                    else if (imp_set.channel_target[i]==IMPORT_TO_Y)
                        number_of_sets_with_column[1]++;
                    else if (imp_set.channel_target[i]==IMPORT_TO_Y1)
                        number_of_sets_with_column[2]++;
                    else if (imp_set.channel_target[i]==IMPORT_TO_Y2)
                        number_of_sets_with_column[3]++;
                    else if (imp_set.channel_target[i]==IMPORT_TO_Y3)
                        number_of_sets_with_column[4]++;
                    else if (imp_set.channel_target[i]==IMPORT_TO_Y4)
                        number_of_sets_with_column[5]++;
                    else if (imp_set.channel_target[i]==IMPORT_TO_TRIGGER)
                        triggerChannel=i;
                }
            }
            number_of_x_columns=number_of_sets_with_column[0];//save this, because we need to know it and the array will be a counter in future
            max_nr_of_sets=0;
            int col_count_import_set=settype_cols(tabImportInfo->selType->currentValue());
            //cout << "col_count_import_set=" << col_count_import_set << endl;
            for (int i=0;i<6;i++)
            {
                //cout << i << " = " << number_of_sets_with_column[i] << endl;
                if (number_of_sets_with_column[i]>0 && i>=col_count_import_set)
                {
                    sprintf(dummy,"[Warning] Maximum number of columns for import-set-type %s = %d. The column %s will not be imported!",set_types(tabImportInfo->selType->currentValue()),col_count_import_set,dataset_colname(i));
                    stufftext(dummy);
                }
                if (max_nr_of_sets<number_of_sets_with_column[i]) max_nr_of_sets=number_of_sets_with_column[i];
                number_of_sets_with_column[i]=0;//we will need this as a counter...
            }
            //cout << "number_of_new_sets=" << max_nr_of_sets << endl;

            snos=new int[max_nr_of_sets+1];
            gnos=new int[max_nr_of_sets+1];
            labels labs;
            tickmarks *t;

            for (int i=0;i<max_nr_of_sets;i++)
            {
                setno = nextset(target_gno);//allocate new sets
                snos[i]=setno;
                gnos[i]=target_gno;
                set_set_hidden(target_gno, setno, FALSE);
                get_graph_labels(target_gno, &labs);
                if (imp_set.title!=NULL)
                {
                    set_plotstr_string(&labs.title,imp_set.title);
                }
                if (imp_set.subtitle!=NULL)
                {
                    set_plotstr_string(&labs.stitle,imp_set.subtitle);
                }
                if (imp_set.x_title!=NULL)
                {
                    t = get_graph_tickmarks(target_gno, 0);//X
                    set_plotstr_string(&t->label, imp_set.x_title);
                }
                if (imp_set.y_title!=NULL)
                {
                    t = get_graph_tickmarks(target_gno, 1);//Y
                    set_plotstr_string(&t->label, imp_set.y_title);
                }
                ret=setlength(target_gno,setno,points_read);
                ret=set_dataset_type(target_gno,setno,tabImportInfo->selType->currentValue());
            }//end for-loop over all new sets
            if (triggerChannel>-1)
            {
                triggerSet=nextset(target_gno);
                ret=setlength(target_gno,triggerSet,points_read);
                ret=set_dataset_type(target_gno,triggerSet,tabImportInfo->selType->currentValue());
            }
            //now we have generated more than one set!
            int col;
            bool contains_x=false;
            for (int i=0;i<imp_set.channels;i++)
            {
                if (imp_set.channel_target[i]!=IMPORT_TO_NONE)
                {
                    if (imp_set.channel_target[i]==IMPORT_TO_TRIGGER)
                    {
                        memcpy(g[target_gno].p[triggerSet].data.ex[1],imp_set.first_data[i],sizeof(double)*points_read);
                        continue;
                    }
                    //double * n_data=(double*)malloc(sizeof(double)*points_read);
                    //memcpy(n_data,imp_set.first_data[i],sizeof(double)*points_read);
                    switch (imp_set.channel_target[i])
                    {
                    case IMPORT_TO_X:
                        col=0;
                        contains_x=true;
                        break;
                    case IMPORT_TO_Y:
                        col=1;
                        break;
                    case IMPORT_TO_Y1:
                        col=2;
                        break;
                    case IMPORT_TO_Y2:
                        col=3;
                        break;
                    case IMPORT_TO_Y3:
                        col=4;
                        break;
                    case IMPORT_TO_Y4:
                        col=5;
                        break;
                    }
                    if (col>=col_count_import_set)
                    {
                        continue;//go to next channel because the allocated set does not have enough space for this column
                    }
                    memcpy(g[target_gno].p[snos[number_of_sets_with_column[col]]].data.ex[col],imp_set.first_data[i],sizeof(double)*points_read);
                    ///memcpy(g[target_gno].p[setno].data.ex[col],imp_set.first_data[i],sizeof(double)*points_read);
                    if (imp_set.factors[col]!=1.0)
                        for (int k=0;k<points_read;k++)
                            g[target_gno].p[snos[number_of_sets_with_column[col]]].data.ex[col][k]*=imp_set.factors[col];
                    ///g[target_gno].p[setno].data.ex[col][k]*=imp_set.factors[col];
                    number_of_sets_with_column[col]++;
                }
            }//data imported into sets
            if (contains_x==false)//no X-column set --> initialize every X in every set with header-data
            {
                col=0;
                double dx=1.0;
                double X0=0.0;
                if (imp_set.deltaxset==false && imp_set.fset==true)
                {
                    dx=1.0/imp_set.f;
                }
                else if (imp_set.deltaxset==true && imp_set.fset==false)
                {
                    dx=imp_set.deltax;
                }
                else if (imp_set.deltaxset==true && imp_set.fset==true)
                {
                    dx=imp_set.deltax;
                }
                if (imp_set.x0set==true) X0=imp_set.x0;
                for (int j=0;j<max_nr_of_sets;j++)
                {
                    for (int i=0;i<points_read;i++)
                    {
                        g[target_gno].p[snos[j]].data.ex[col][i]=i*dx+X0;
                    }
                }
            }//ende contains_x==false
            else//at least one x-column
            {
                if (number_of_x_columns==1)//only one X for all sets
                {
                    for (int j=1;j<max_nr_of_sets;j++)
                    {
                        memcpy(g[target_gno].p[snos[j]].data.ex[0],g[target_gno].p[snos[0]].data.ex[0],sizeof(double)*points_read);//copy X-column
                    }
                }
                else //some X have been imported and some others not --> give the rest an index as X-column
                {
                    for (int j=number_of_x_columns;j<max_nr_of_sets;j++)
                    {
                        for (int i=0;i<points_read;i++)
                        {
                            g[target_gno].p[snos[j]].data.ex[col][i]=i*1.0;//write only index numbers into X-columns
                        }
                    }
                }
            }

            //do trigger-channel-actions
            if (triggerChannel>-1)
            {
                char fstr2[64];
                int resno;
                char *rarray;
                double trigger_shift=0.0;
                resno = get_restriction_array(target_gno,triggerSet,RESTRICT_NONE,0,&rarray);
                sprintf(fstr2,"G%d.S%d.X=G%d.S%d.X",target_gno,triggerSet,target_gno,snos[0]);
                //copy X-values from first set
                resno = do_compute(target_gno, triggerSet, target_gno, triggerSet, rarray, fstr2);
                if (imp_set.factors[6]!=1.0)
                {
                    sprintf(fstr2,"G%d.S%d.Y=G%d.S%d.Y*%lf",target_gno,triggerSet,target_gno,snos[0],imp_set.factors[6]);
                    //rescale trigger-channel
                    resno = do_compute(target_gno, triggerSet, target_gno, triggerSet, rarray, fstr2);
                }
                for (int lll=1;lll<getsetlength(target_gno,triggerSet);lll++)
                {
                    if (g[target_gno].p[triggerSet].data.ex[1][lll-1]<imp_set.triggervalue && g[target_gno].p[triggerSet].data.ex[1][lll]>=imp_set.triggervalue)
                    {
                        trigger_shift=g[target_gno].p[triggerSet].data.ex[0][lll];
                        break;
                    }
                }
                //cout << "trigger_shift=" << trigger_shift << endl;
                for (int lll=0;lll<max_nr_of_sets;lll++)
                {
                    resno = get_restriction_array(target_gno,snos[lll],RESTRICT_NONE,0,&rarray);
                    sprintf(fstr2,"G%d.S%d.X=G%d.S%d.X-%lf",target_gno,snos[lll],target_gno,snos[lll],trigger_shift);
                    resno = do_compute(target_gno, snos[lll], target_gno, snos[lll], rarray, fstr2);
                }
                if (tabDataInfo->chkKeppTrigger->isChecked()==false)
                    killsetdata(target_gno, triggerSet);
                else
                {
                    resno = get_restriction_array(target_gno,triggerSet,RESTRICT_NONE,0,&rarray);
                    sprintf(fstr2,"G%d.S%d.X=G%d.S%d.X-%lf",target_gno,triggerSet,target_gno,triggerSet,trigger_shift);
                    resno = do_compute(target_gno, triggerSet, target_gno, triggerSet, rarray, fstr2);
                }
            }

            append_to_storage(&collect_nr_of_sets,&col_gnos,max_nr_of_sets,gnos);//save the set-ids for the undo-operations
            collect_nr_of_sets-=max_nr_of_sets;//because collect_nr_of_sets has to be increased in the next function
            append_to_storage(&collect_nr_of_sets,&col_snos,max_nr_of_sets,snos);

        }//end for(all files to import)

        auto_transfer_from_header=true;

        //Undo-Stuff
        ///SetImportBinaryData(target_gno, setno,datFileName,&imp_set);
        ///SetsModified(1,&target_gno,&setno,UNDO_COMPLETE);
        //SetsModified(max_nr_of_sets,gnos,snos,UNDO_COMPLETE);//INFORMATION FOR BINARY IMPORT NOT INCLUDED IN UNDO! WARNING!
        SetsModified(collect_nr_of_sets,col_gnos,col_snos,UNDO_COMPLETE);

        //char dummy[1024];
        if (collect_nr_of_sets==1)//max_nr_of_sets==1)
        {
            sprintf(dummy,"[G%d.S%d]",col_gnos[0],col_snos[0]);
            addAditionalDescriptionToLastNode(-1,QObject::tr("Binary data import --> ")+QString(dummy),QString(),0);
        }
        else
        {
            addAditionalDescriptionToLastNode(-1,QObject::tr("Binary data import"),QString(),0);
        }
        ret=tabImportInfo->selAutoscale->currentValue();
        autoscale_graph(target_gno,ret);
    }//end of 'there has really been a graph selected for import'

    update_all();
    unset_wait_cursor();
    mainWin->mainArea->completeRedraw();
    if (gnos!=NULL) delete[] gnos;
    if (snos!=NULL) delete[] snos;
    if (col_gnos!=NULL) delete[] col_gnos;
    if (col_snos!=NULL) delete[] col_snos;
}

void frmBinaryFormatInput::doClose(void)
{
    hide();
}

void frmBinaryFormatInput::doAccept(void)
{
    doOK();
    if (ApplyError==false)
        doClose();
}

void frmBinaryFormatInput::getDatFilesFromString(QString * origin,QStringList * lst)
{
    (*lst).clear();
    (*lst)=origin->split(QString(";"));
    if (lst->length()>0)
    {
        QString dummy;
        QChar sep=QDir::separator();
        QString dir=lst->at(0).left(lst->at(0).lastIndexOf(sep));
        //cout << "dir=#" << dir.toAscii().constData() << "#" << endl;
        for (int i=1;i<lst->length();i++)
        {
            if (lst->at(i).contains(sep)==false)
            {
                dummy=lst->at(i);
                lst->replace(i,dir+sep+dummy);
            }
        }
    }
    //cout << lst->at(0).toAscii().constData() << endl;
}

void frmBinaryFormatInput::readHeader(void)
{
    //initialize the import settings
    initSettings(imp_set);

    if (!lenDataFile->text().isEmpty())
    {
        QString help=lenDataFile->text().toAscii();
        getDatFilesFromString(&help,&datFileNames);
        strcpy(datFileName,datFileNames.at(0).toAscii());
    }
    else
    {
        datFileName[0]='\0';
    }

    if (!chkHeader->isChecked())//no header
    {
        headerFileName[0]='\0';

    }

    if (cmbFormatSource->currentIndex()==1)//header in datafile
    {
        strcpy(headerFileName,datFileName);
    }
    else if (cmbFormatSource->currentIndex()==2)//header in separate file
    {
        if (!lenHeaderFile->text().isEmpty())
            strcpy(headerFileName,lenHeaderFile->text().toAscii());
        else
            headerFileName[0]='\0';
    }

    if (headerFileName[0]!='\0')
    {
        ifstream ifi;
        ifi.open(headerFileName);
        //read data from header to imp_set
        doReadDataFromHeader(ifi);
        //read a few datapoints for showing
        //cout << "channels=" << imp_set.channels << " bytes=" << imp_set.bytesize << " bits=" << imp_set.bitsize << " suggestion=" << first_suggestion << endl;
        if (imp_set.channels>0 && (imp_set.bytesize!=-1 || imp_set.bitsize!=-1))
        {
            int sav_points=imp_set.points;
            imp_set.points=10;
            imp_set.channel_target=new int[imp_set.channels];
            imp_set.channel_size=new int[imp_set.channels];
            imp_set.channel_format=new int[imp_set.channels];
            for (int i=0;i<imp_set.channels;i++)
            {
                imp_set.channel_target[i]=IMPORT_TO_X;
                imp_set.channel_size[i]=imp_set.bytesize!=-1?imp_set.bytesize:imp_set.bitsize/8;
                imp_set.channel_format[i]=first_suggestion;
            }
            int columns_read,points_read;
            readBinaryFromFile(ifi,imp_set,&imp_set.first_data,&columns_read,&points_read);
            imp_set.points=sav_points;
        }
        else
        {
            imp_set.first_data=NULL;
        }
        ifi.close();

        if (imp_set.x0set==true && imp_set.deltaxset==false && imp_set.fset==false)
        {
            imp_set.deltax=1.0;
            imp_set.deltaxset=true;
        }
        else if (imp_set.x0set==false && imp_set.deltaxset==true && imp_set.fset==false)
        {
            imp_set.x0=0.0;
            imp_set.x0set=true;
        }
        else if (imp_set.x0set==false && imp_set.deltaxset==false && imp_set.fset==true)
        {
            imp_set.x0=0.0;
            imp_set.x0set=true;
        }

        if (auto_transfer_from_header)
        {
            convertSettingsToString();
            if (imp_set.channels!=-1) tabDataInfo->spnChannelCount->setValue(imp_set.channels);
            if (imp_set.points!=-1)
            {
                tabDataInfo->spnDataSetCount->setValue(imp_set.points);
                tabDataInfo->chkReadToEOF->setChecked(false);//if a number of sample points is set, we won't need to read until end
            }
            qApp->processEvents();
            if (first_suggestion!=-1)
            {
                for (int i=0;i<tabDataInfo->number_of_lines;i++)
                    tabDataInfo->inFormats[i]->cmbFormat->setCurrentIndex(first_suggestion);
            }
            tabFileInfo->headerContents=settingString;
        }

    }//end header file selected

    if (auto_transfer_from_header)
        tabs->setCurrentWidget(tabFileInfo);

}

void frmBinaryFormatInput::headerToggled(bool t)
{
    if (t)
    {
        cmbFormatSource->setCurrentIndex(1);
        tabHeader->setEnabled(true);
        lblHeaderFile->setVisible(true);
        lenHeaderFile->setVisible(true);
        cmdSelectHeaderFile->setVisible(true);
        cmbHeaderFileFormat->setVisible(true);
        lblHeaderFileFormat->setVisible(true);
    }
    else
    {
        cmbFormatSource->setCurrentIndex(0);
        tabHeader->setEnabled(false);
        lblHeaderFile->setVisible(false);
        lenHeaderFile->setVisible(false);
        cmdSelectHeaderFile->setVisible(false);
        cmbHeaderFileFormat->setVisible(false);
        lblHeaderFileFormat->setVisible(false);
    }
}

void frmBinaryFormatInput::doReadDataFromHeader(ifstream & ifi)
{
    char dummy[512];
    bool integer_type;
    double d_value;
    long *headerDatas=new long[tabHeader->number_of_lines];
    long double* ldHeaderDatas=new long double[tabHeader->number_of_lines];
    char buffer[16];
    char * stringText=new char[2];
    int size,readbytes,global_size=0;
    first_suggestion=0;
    if (cmbFormatSource->currentIndex()==2 && cmbHeaderFileFormat->currentIndex()==1)
    {//ini-header
        /*for (int i=0;i<6;i++)
        imp_set.factors[i]=1.0;*/
        initSettings(imp_set);
        for (int i=0;i<tabHeader->nr_of_sels;i++)
        {
            if (tabHeader->sels[i]->currentValue()!=IMPORT_TO_NONE)
            {
                strcpy(dummy,tabHeader->readValues[i].toAscii());
                d_value=atof(dummy);
                switch (tabHeader->sels[i]->currentValue())
                {
                case IMPORT_TO_TITLE:
                    if (imp_set.title!=NULL)
                        delete[] imp_set.title;
                    imp_set.title=new char[tabHeader->readValues[i].length()+1];
                    strcpy(imp_set.title,dummy);
                    break;
                case IMPORT_TO_SET_LEGEND:
                    if (imp_set.set_title!=NULL)
                        delete[] imp_set.set_title;
                    imp_set.set_title=new char[tabHeader->readValues[i].length()+1];
                    strcpy(imp_set.set_title,dummy);
                    break;
                case IMPORT_TO_X0:
                    imp_set.x0=d_value;
                    imp_set.x0set=true;
                    break;
                case IMPORT_TO_DELTAX:
                    imp_set.deltax=d_value;
                    imp_set.deltaxset=true;
                    break;
                case IMPORT_TO_XTITLE:
                    if (imp_set.x_title!=NULL)
                        delete[] imp_set.x_title;
                    imp_set.x_title=new char[tabHeader->readValues[i].length()+1];
                    strcpy(imp_set.x_title,dummy);
                    break;
                case IMPORT_TO_YTITLE:
                    if (imp_set.y_title!=NULL)
                        delete[] imp_set.y_title;
                    imp_set.y_title=new char[tabHeader->readValues[i].length()+1];
                    strcpy(imp_set.y_title,dummy);
                    break;
                case IMPORT_TO_XFACTOR:
                    imp_set.factors[0]*=d_value;
                    break;
                case IMPORT_TO_YFACTOR:
                    imp_set.factors[1]*=d_value;
                    break;
                case IMPORT_TO_Y1FACTOR:
                    imp_set.factors[2]*=d_value;
                    break;
                case IMPORT_TO_Y2FACTOR:
                    imp_set.factors[3]*=d_value;
                    break;
                case IMPORT_TO_Y3FACTOR:
                    imp_set.factors[4]*=d_value;
                    break;
                case IMPORT_TO_Y4FACTOR:
                    imp_set.factors[5]*=d_value;
                    break;
                case IMPORT_TO_TRIGGER_FACTOR:
                    imp_set.factors[6]*=d_value;
                    break;
                case IMPORT_TO_SUBTITLE:
                    if (imp_set.subtitle!=NULL)
                        delete[] imp_set.subtitle;
                    imp_set.subtitle=new char[tabHeader->readValues[i].length()+1];
                    strcpy(imp_set.subtitle,dummy);
                    break;
                case IMPORT_TO_NUMBER_OF_DATA:
                    imp_set.points=atoi(dummy);
                    break;
                case IMPORT_TO_NUMBER_OF_CHANNELS:
                    imp_set.channels=atoi(dummy);
                    break;
                case IMPORT_TO_DATA_SIZE:
                    imp_set.bytesize=atoi(dummy);
                    break;
                case IMPORT_TO_DATA_SIZE_BIT:
                    imp_set.bitsize=atoi(dummy);
                    break;
                case IMPORT_TO_DATA_SAMPLING_RATE:
                    imp_set.f=d_value;
                    imp_set.fset=true;
                    break;
                case IMPORT_TO_WHOLE_DATA_BLOCK_SIZE:
                    imp_set.whole_size=atoi(dummy);
                    break;
                case IMPORT_TO_SINGLE_DATA_BLOCK_SIZE:
                    imp_set.single_size=atoi(dummy);
                    break;
                }
            }
        }
    }
    else//binary header
    {
        for (int i=0;i<tabHeader->number_of_lines;i++)
        {
            size=tabHeader->inFormats[i]->getSize();
            global_size+=size;
            if (tabHeader->inFormats[i]->getType()!=COLUMN_OFFSET && tabHeader->inFormats[i]->getType()!=COLUMN_STRING)
            {
                ifi.read(buffer,size);
                readbytes=(int)(ifi.gcount());
                if (readbytes!=size)
                {
                    sprintf(dummy,"[Information] Stopped reading header item after import of %d Bytes (of %d bytes).",readbytes,size);
                    stufftext(dummy);
                }
            }
            switch (tabHeader->inFormats[i]->getType())
            {
            case COLUMN_OFFSET:
                integer_type=false;
                for (int j=0;j<size;j++)
                    ifi.read(buffer,1);//read byte after byte into the same buffer (we dont need it)
                break;
            case COLUMN_SIG_CHAR:
                integer_type=true;
                headerDatas[i]=(long)(*((signed char*)((void*)buffer)));
                break;
            case COLUMN_USIG_CHAR:
                integer_type=true;
                headerDatas[i]=(long)(*((unsigned char*)((void*)buffer)));
                break;
            case COLUMN_SIG_SHORT:
                integer_type=true;
                headerDatas[i]=(long)(*((signed short*)((void*)buffer)));
                break;
            case COLUMN_USIG_SHORT:
                integer_type=true;
                headerDatas[i]=(long)(*((unsigned short*)((void*)buffer)));
                break;
            case COLUMN_SIG_INT:
                integer_type=true;
                headerDatas[i]=(long)(*((signed int*)((void*)buffer)));
                break;
            case COLUMN_USIG_INT:
                integer_type=true;
                headerDatas[i]=(long)(*((unsigned int*)((void*)buffer)));
                break;
            case COLUMN_SIG_LONG:
                integer_type=true;
                headerDatas[i]=(long)(*((signed long*)((void*)buffer)));
                break;
            case COLUMN_USIG_LONG:
                integer_type=true;
                headerDatas[i]=(long)(*((unsigned long*)((void*)buffer)));
                break;
            case COLUMN_FLOAT:
                integer_type=false;
                ldHeaderDatas[i]=(long double)(*((float*)((void*)buffer)));
                break;
            case COLUMN_DOUBLE:
                integer_type=false;
                ldHeaderDatas[i]=(long double)(*((double*)((void*)buffer)));
                break;
            case COLUMN_LONG_DOUBLE:
                integer_type=false;
                ldHeaderDatas[i]=(long double)(*((long double*)((void*)buffer)));
                break;
            case COLUMN_STRING:
                integer_type=false;
                if (determine_string_size==true)
                {
                    int res=16;
                    int read_s=0;
                    char * targ=new char[res+1];
                    char read_c=imp_set.string_end_char-1;
                    while (read_c!=imp_set.string_end_char)
                    {
                        ifi.read(&read_c,1);
                        targ[read_s]=read_c;
                        read_s++;
                        if (read_s==res)
                        {
                            char * targ2=new char[res+17];
                            memcpy(targ2,targ,sizeof(char)*read_s);
                            delete[] targ;
                            targ=targ2;
                            res+=16;
                        }
                    }
                    targ[read_s]='\0';
                    global_size-=size;
                    size=read_s;
                    tabHeader->inFormats[i]->spnSize->setValue(size);
                    global_size+=size;
                    delete[] stringText;
                    stringText=new char[size+2];
                    strcpy(stringText,targ);
                }
                else
                {
                    delete[] stringText;
                    stringText=new char[size+2];
                    ifi.read(stringText,size);
                    stringText[size]='\0';
                    readbytes=(int)(ifi.gcount());
                    if (readbytes!=size)
                    {
                        sprintf(dummy,"[Information] Stopped reading header item after import of %d Bytes (of %d bytes).",readbytes,size);
                        stufftext(dummy);
                    }
                }
                break;
            }//end switch-block

            if (tabHeader->inFormats[i]->getTarget()!=IMPORT_TO_NONE)
                switch (tabHeader->inFormats[i]->getTarget())
                {
                case IMPORT_TO_TITLE:
                    delete[] imp_set.title;
                    imp_set.title=new char[size+1];
                    strcpy(imp_set.title,stringText);
                    break;
                case IMPORT_TO_SET_LEGEND:
                    delete[] imp_set.set_title;
                    imp_set.set_title=new char[size+1];
                    strcpy(imp_set.set_title,stringText);
                    break;
                case IMPORT_TO_X0:
                    if (integer_type)
                        imp_set.x0=headerDatas[i];
                    else
                        imp_set.x0=ldHeaderDatas[i];
                    imp_set.x0set=true;
                    break;
                case IMPORT_TO_DELTAX:
                    if (integer_type)
                        imp_set.deltax=headerDatas[i];
                    else
                        imp_set.deltax=ldHeaderDatas[i];
                    imp_set.deltaxset=true;
                    break;
                case IMPORT_TO_XTITLE:
                    delete[] imp_set.x_title;
                    imp_set.x_title=new char[size+1];
                    strcpy(imp_set.x_title,stringText);
                    break;
                case IMPORT_TO_YTITLE:
                    delete[] imp_set.y_title;
                    imp_set.y_title=new char[size+1];
                    strcpy(imp_set.y_title,stringText);
                    break;
                case IMPORT_TO_XFACTOR:
                    if (integer_type)
                        imp_set.factors[0]*=headerDatas[i];
                    else
                        imp_set.factors[0]*=ldHeaderDatas[i];
                    break;
                case IMPORT_TO_YFACTOR:
                    if (integer_type)
                        imp_set.factors[1]*=headerDatas[i];
                    else
                        imp_set.factors[1]*=ldHeaderDatas[i];
                    break;
                case IMPORT_TO_Y1FACTOR:
                    if (integer_type)
                        imp_set.factors[2]*=headerDatas[i];
                    else
                        imp_set.factors[2]*=ldHeaderDatas[i];
                    break;
                case IMPORT_TO_Y2FACTOR:
                    if (integer_type)
                        imp_set.factors[3]*=headerDatas[i];
                    else
                        imp_set.factors[3]*=ldHeaderDatas[i];
                    break;
                case IMPORT_TO_Y3FACTOR:
                    if (integer_type)
                        imp_set.factors[4]*=headerDatas[i];
                    else
                        imp_set.factors[4]*=ldHeaderDatas[i];
                    break;
                case IMPORT_TO_Y4FACTOR:
                    if (integer_type)
                        imp_set.factors[5]*=headerDatas[i];
                    else
                        imp_set.factors[5]*=ldHeaderDatas[i];
                    break;
                case IMPORT_TO_TRIGGER_FACTOR:
                    if (integer_type)
                        imp_set.factors[6]*=headerDatas[i];
                    else
                        imp_set.factors[6]*=ldHeaderDatas[i];
                    break;
                case IMPORT_TO_SUBTITLE:
                    delete[] imp_set.subtitle;
                    imp_set.subtitle=new char[size+1];
                    strcpy(imp_set.subtitle,stringText);
                    break;
                case IMPORT_TO_NUMBER_OF_DATA:
                    imp_set.points=headerDatas[i];
                    break;
                case IMPORT_TO_NUMBER_OF_CHANNELS:
                    imp_set.channels=headerDatas[i];
                    break;
                case IMPORT_TO_DATA_SIZE:
                    imp_set.bytesize=headerDatas[i];
                    break;
                case IMPORT_TO_DATA_SIZE_BIT:
                    imp_set.bitsize=headerDatas[i];
                    break;
                case IMPORT_TO_DATA_SAMPLING_RATE:
                    if (integer_type)
                        imp_set.f=headerDatas[i];
                    else
                        imp_set.f=ldHeaderDatas[i];
                    imp_set.fset=true;
                    break;
                case IMPORT_TO_WHOLE_DATA_BLOCK_SIZE:
                    imp_set.whole_size=headerDatas[i];
                    break;
                case IMPORT_TO_SINGLE_DATA_BLOCK_SIZE:
                    imp_set.single_size=headerDatas[i];
                    break;
                }
        }//end loop through lines
    }//end reading binary header

    delete[] stringText;
    int bsize;
    //cout << "first_suggestion=" << first_suggestion << " bits=" << imp_set.bitsize << endl;
    first_suggestion=-1;
    if (imp_set.bitsize!=-1)
    {
        bsize=imp_set.bitsize/8;
        if (bsize*8==imp_set.bitsize)//add suggestions for import
        {
            for (int i=0;i<NUMBER_OF_COLUMN_FORMATS;i++)
            {
                if (binaryImportFormat[i].size==bsize)
                {
                    if (first_suggestion==-1)
                        first_suggestion=i;
                }
            }
        }
    }
    //cout << "first_suggestion=" << first_suggestion << " bytes=" << imp_set.bytesize << endl;
    if (imp_set.bytesize!=-1)
    {
        first_suggestion=-1;
        for (int i=0;i<NUMBER_OF_COLUMN_FORMATS;i++)
        {
            if (binaryImportFormat[i].size==imp_set.bytesize)
            {
                if (first_suggestion==-1)
                    first_suggestion=i;
            }
        }
    }
    //cout << "first_suggestion=" << first_suggestion << endl;
    //cout << imp_set.points << " " << imp_set.whole_size << " " << imp_set.bytesize << " " << imp_set.bitsize << endl;
    if (imp_set.points<=0 && imp_set.whole_size!=-1 && (imp_set.bytesize!=-1 || imp_set.bitsize!=-1))
    {
        int size=imp_set.bytesize!=-1?imp_set.bytesize:imp_set.bitsize/8;
        imp_set.points=imp_set.whole_size/size;
        if (imp_set.channels>0)
            imp_set.points/=imp_set.channels;
    }
    //cout << "points=" << imp_set.points << " whole=" << imp_set.whole_size << " byte=" << imp_set.bytesize << " bit=" << imp_set.bitsize << " global_size=" << global_size << endl;
    headersize=global_size;
    delete[]headerDatas;
    delete[]ldHeaderDatas;
}

void frmBinaryFormatInput::transmitInfos(void)
{
    if (!lenDataFile->text().isEmpty())
    {
        strcpy(datFileName,lenDataFile->text().toAscii());
        tabFileInfo->DatFile=datFileName;
    }
    else
    {
        tabFileInfo->DatFile=NULL;
        datFileName[0]='\0';
    }
    if (cmbFormatSource->currentIndex()==2)
    {
        if (!lenHeaderFile->text().isEmpty())
        {
            strcpy(headerFileName,lenHeaderFile->text().toAscii());
            tabFileInfo->HeaderFile=headerFileName;
            ifstream ifi;
            ifi.open(headerFileName,ios::binary);
            doReadDataFromHeader(ifi);
            ifi.close();
            convertSettingsToString();
            tabFileInfo->headerContents=settingString;
            if (imp_set.channels>0) tabDataInfo->spnChannelCount->setValue(imp_set.channels);
            if (imp_set.points>0) tabDataInfo->spnDataSetCount->setValue(imp_set.points);
            qApp->processEvents();
            if (first_suggestion!=-1)
            {
                for (int i=0;i<tabDataInfo->number_of_lines;i++)
                {
                    tabDataInfo->inFormats[i]->cmbFormat->setCurrentIndex(first_suggestion);
                }
            }
        }
        else
        {
            tabFileInfo->HeaderFile=NULL;
            headerFileName[0]='\0';
        }
    }
    else if (cmbFormatSource->currentIndex()==1)
    {
        tabFileInfo->HeaderFile=tabFileInfo->DatFile;
        strcpy(headerFileName,datFileName);
    }
    else
    {
        tabFileInfo->HeaderFile=NULL;
        headerFileName[0]='\0';
    }
}

void frmBinaryFormatInput::convertSettingsToString(void)
{
    int bsize;
    first_suggestion=-1;
    char dummy[512];
    char dummy2[4];
    QString result(tr("Data read from Header:\n"));

    if (imp_set.title!=NULL)
        if (imp_set.title[0]!='\0')
            result.append(tr("Set title= ")+QString(imp_set.title)+QString("\n"));
    if (imp_set.subtitle!=NULL)
        if (imp_set.subtitle[0]!='\0')
            result.append(tr("Set subtitle= ")+QString(imp_set.subtitle)+QString("\n"));
    if (imp_set.x_title!=NULL)
        if (imp_set.x_title[0]!='\0')
            result.append(tr("Set x-title= ")+QString(imp_set.x_title)+QString("\n"));
    if (imp_set.y_title!=NULL)
        if (imp_set.y_title[0]!='\0')
            result.append(tr("Set y-title= ")+QString(imp_set.y_title)+QString("\n"));
    if (imp_set.set_title!=NULL)
        if (imp_set.set_title[0]!='\0')
            result.append(tr("Set-title= ")+QString(imp_set.set_title)+QString("\n"));

    if (imp_set.x0set)
    {
        sprintf(dummy,"%g\n",imp_set.x0);
        result.append(tr("X0= ")+QString(dummy));
    }
    if (imp_set.deltaxset)
    {
        sprintf(dummy,"%g\n",imp_set.deltax);
        result.append(tr("deltaX= ")+QString(dummy));
    }
    if (imp_set.fset)
    {
        sprintf(dummy,"%g samples/sec\n",imp_set.f);
        result.append(tr("Sampling rate= ")+QString(dummy));
    }
    if (imp_set.channels!=-1)
    {
        sprintf(dummy,"%d\n",imp_set.channels);
        result.append(tr("Channels= ")+QString(dummy));
    }
    if (imp_set.points!=-1)
    {
        sprintf(dummy,"%d\n",imp_set.points);
        result.append(tr("Sample count= ")+QString(dummy));
    }
    for (int i=0;i<6;i++)
    {
        if (i==0) strcpy(dummy2,"X");
        else if (i==1) strcpy(dummy2,"Y");
        else sprintf(dummy2,"Y%d",i-1);
        if (imp_set.factors[i]!=1.0)
        {
            sprintf(dummy,"Factor for %s = %g\n",dummy2,imp_set.factors[i]);
            result.append(QString(dummy));
        }
    }
    if (imp_set.bitsize!=-1)
    {
        bsize=imp_set.bitsize/8;
        if (bsize*8==imp_set.bitsize)
            sprintf(dummy,"%d bits\n",imp_set.bitsize);
        else
            sprintf(dummy,"%d bits (not a multiple of 8)\n",imp_set.bitsize);
        result.append(tr("Size of sample= ")+QString(dummy));
        if (bsize*8==imp_set.bitsize)//add suggestions for import
        {
            for (int i=0;i<NUMBER_OF_COLUMN_FORMATS;i++)
            {
                if (binaryImportFormat[i].size==bsize)
                {
                    result.append(tr("Suggested import format= ")+QString(binaryImportFormatName[binaryImportFormat[i].format])+QString("\n"));
                    if (first_suggestion==-1)
                        first_suggestion=i;
                }
            }
        }

    }
    if (imp_set.bytesize!=-1)
    {
        first_suggestion=-1;
        sprintf(dummy,"%d bytes\n",imp_set.bytesize);
        result.append(tr("Size of sample= ")+QString(dummy));
        for (int i=0;i<NUMBER_OF_COLUMN_FORMATS;i++)
        {
            if (binaryImportFormat[i].size==imp_set.bytesize)
            {
                result.append(tr("Suggested import format= ")+QString(binaryImportFormatName[binaryImportFormat[i].format])+QString("\n"));
                if (first_suggestion==-1)
                    first_suggestion=i;
            }
        }
    }

    if ((imp_set.bytesize!=-1 || imp_set.bitsize!=-1) && imp_set.channels!=-1 && imp_set.points!=-1)
    {
        int bsize=imp_set.bytesize!=-1?imp_set.bytesize:imp_set.bitsize/8;
        int whole=bsize*imp_set.channels*imp_set.points;
        sprintf(dummy,"Data block size= %d Bytes\n",whole);
        result.append(QString(dummy));
        sprintf(dummy,"File section to be read= %d Bytes\n",whole+headersize);
        result.append(QString(dummy));
    }

    if (imp_set.first_data!=NULL)
    {
        result.append(QString("\n"));
        result.append(tr("First 10 datapoints:\n"));
        QString strli=QString("Nr.:\t");
        for (int i=0;i<imp_set.channels;i++)
        {
            sprintf(dummy,"ch%d\t",i);
            strli=strli+QString(dummy);
        }
        strli=strli+QString("\n");
        result.append(strli);
        for (int j=0;j<10;j++)
        {
            sprintf(dummy,"%d:\t",j);
            strli=QString(dummy);
            for (int i=0;i<imp_set.channels;i++)
            {
                sprintf(dummy,"%g\t",imp_set.first_data[i][j]);
                strli=strli+QString(dummy);
            }
            strli=strli+QString("\n");
            result.append(strli);
        }
    }
    settingString=result;
}

void readBinaryFromFile(ifstream & ifi,importSettings imp_set,double *** data,int * columns_read,int * points_read)
{
    char buffer[64];
    (*data)=new double*[imp_set.channels];
    long position=ifi.tellg();//current position
    long length;
    ifi.seekg(0,ios::end);
    length=ifi.tellg();//length complete
    //cout << "length=" << length << " position=" << position << endl;
    length-=position;//length without header
    ifi.seekg(position);//go to first byte after header
    //cout << "resulting length=" << length << endl;
    long size_of_one_point=0;
    long *size_of_one_set=new long[imp_set.channels];
    long calc_samp_count;
    for (int i=0;i<imp_set.channels;i++)
        size_of_one_point+=(long)(imp_set.channel_size[i]);
    calc_samp_count=length/(size_of_one_point);
    if (imp_set.points>=0 && imp_set.points<calc_samp_count)
        *points_read=imp_set.points;
    else
        *points_read=calc_samp_count;
    if (imp_set.read_to_eof==true) *points_read=calc_samp_count;

    for (int i=0;i<imp_set.channels;i++)
        (*data)[i]=new double[*points_read+1];//we need a bit more space because we need to store extra data into dummy location

    for (int i=0;i<imp_set.channels;i++)
        size_of_one_set[i]=calc_samp_count*imp_set.channel_size[i];//the byte-size in the file
    //cout << "size_of_one_point=" << size_of_one_point << " calc.samp.count=" << calc_samp_count << endl;
    *columns_read=imp_set.channels;
    int i=0;//channel_nr
    int read=0;//current number of read data
    int real_read=0;//counts on even, if the channel has more data than is to be read
    //start reading with channel=0 and read=0
    while (!ifi.eof())
    {
        ifi.read(buffer,imp_set.channel_size[i]);//read current input
        //and save it as double in the right position
        switch (imp_set.channel_format[i])
        {
        case COLUMN_OFFSET:
        case COLUMN_STRING:
            (*data)[i][read]=0.0;
            break;
        case COLUMN_SIG_CHAR:
            (*data)[i][read]=(double)(*((signed char*)((void*)(buffer))));
            break;
        case COLUMN_USIG_CHAR:
            (*data)[i][read]=(double)(*((unsigned char*)((void*)(buffer))));
            break;
        case COLUMN_SIG_SHORT:
            (*data)[i][read]=(double)(*((signed short*)((void*)(buffer))));
            break;
        case COLUMN_USIG_SHORT:
            (*data)[i][read]=(double)(*((unsigned short*)((void*)(buffer))));
            break;
        case COLUMN_SIG_INT:
            (*data)[i][read]=(double)(*((signed int*)((void*)(buffer))));
            break;
        case COLUMN_USIG_INT:
            (*data)[i][read]=(double)(*((unsigned int*)((void*)(buffer))));
            break;
        case COLUMN_SIG_LONG:
            (*data)[i][read]=(double)(*((signed long*)((void*)(buffer))));
            break;
        case COLUMN_USIG_LONG:
            (*data)[i][read]=(double)(*((unsigned long*)((void*)(buffer))));
            break;
        case COLUMN_FLOAT:
            (*data)[i][read]=(double)(*((float*)((void*)(buffer))));
            break;
        case COLUMN_DOUBLE:
            (*data)[i][read]=(double)(*((double*)((void*)(buffer))));
            break;
        case COLUMN_LONG_DOUBLE:
            (*data)[i][read]=(double)(*((long double*)((void*)(buffer))));
            break;
        }//end switch
        if (imp_set.setorder==1)//channel1|channel2|channel3| etc.
        {
            //next point in channel
            real_read++;//this counts the sample-points actually read
            if (real_read>=*points_read)//we have reached a point where there is no more data for this channel that is to be read
            {
                if (calc_samp_count>*points_read)//there is more data in the file than we actually need to read
                {
                    if (real_read>=calc_samp_count)//we have read enough
                    {
                        read=real_read=0;
                        i++;//next channel
                    }
                    else
                        read=*points_read;//we write all new data to the space just outside the read data we actually need
                }
                else//calc_samp_count==*points_read --> we are actually at the end of the data for one channel
                {
                    real_read=read=0;
                    i++;
                }

                if (i>=imp_set.channels)//no more channels available
                {
                    read=*points_read;
                    break;
                }
            }
            else
                read++;//no problems here --> read_count is identical to real_read_count
        }
        else //x1y1z1|x2y2z2|x3y3z3|x4y4z4| etc.
        {
            //read one point after the other
            //next channel
            i++;
            if (i>=imp_set.channels)//next point will start
            {
                i=0;//next point starts with ch0
                if (read<*points_read)//we have read less points than needed
                    read++;//read next point
                else
                    break;//we have read enough --> stop reading
            }
        }//ende channel_order
    }//end while(!eof)
    if (read!=*points_read)
    {
        cout << "error in reading: wrong count after eof: read=" << read << " points=" << *points_read << endl;
        *points_read=read;
    }

    delete[]size_of_one_set;
}

frmSetEditor::frmSetEditor(QWidget * parent):QDialog(parent)
{
    fname=new char[GR_MAXPATHLEN];
    fname[0]='\0';
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: SetEditor"));
    setWindowIcon(QIcon(*GraceIcon));
    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->setSpacing(STD_SPACING);
    layout1=new QHBoxLayout;
    layout1->setMargin(STD_MARGIN);
    layout1->setSpacing(STD_SPACING);
    empty=new QWidget;
    empty->setLayout(layout1);
    lenFromGraph=new QLineEdit(QString("0"),empty);
    lenFromSet=new QLineEdit(QString("0"),empty);
    labDataFrom1=new QLabel(tr("Data in Set G"),empty);
    labDataFrom2=new QLabel(QString(".S"),empty);
    layout1->addWidget(labDataFrom1);
    layout1->addWidget(lenFromGraph);
    layout1->addWidget(labDataFrom2);
    layout1->addWidget(lenFromSet);
    text=new QTextEdit(this);
    aac=new stdButtonGroup(this,true,false,false);
    aac->cmdApply->setText(tr("Update"));
    connect(aac->cmdApply,SIGNAL(clicked()),SLOT(doUpdate()));
    connect(aac->cmdClose,SIGNAL(clicked()),SLOT(doClose()));
    gno=0;
    setno=0;
    layout->addWidget(empty);
    layout->addWidget(text);
    layout->addWidget(aac);
    setLayout(layout);
}

frmSetEditor::~frmSetEditor()
{
    delete[] fname;
}

void frmSetEditor::init(int g_no,int set_no)
{
    //write set data to file and read file contents
    char dummy[512];
    gno=g_no;
    setno=set_no;
    if (fname[0]=='\0')
    {
        QTemporaryFile * temp_file=new QTemporaryFile;
        if (temp_file->open())
        {
            delete[] fname;
            fname=new char[temp_file->fileName().size()+8];
            strcpy(fname,temp_file->fileName().toAscii());
            temp_file->close();
            delete temp_file;
        }
    }
    QFile * fi=new QFile(QString(fname));
    fi->remove();
    delete fi;
    //fname = tmpnam(NULL);//get a temporary file name
    sprintf(dummy,"%d",g_no);
    lenFromGraph->setText(QString(dummy));
    sprintf(dummy,"%d",set_no);
    lenFromSet->setText(QString(dummy));
    FILE *cp;
    cp = grace_openw(fname);
    if (cp == NULL) {
        return;
    }
    write_set(gno, setno, cp, sformat, FALSE);
    grace_close(cp);
    QString str("");
    dummy[0]='\0';
    int counter=0;
    FILE * ifi;
    ifi=fopen(fname,"r");
    while (!feof(ifi))
    {
        fscanf(ifi,"%c",dummy+counter++);
        dummy[511]='\0';
        if (counter==511)
        {
            str=str+QString(dummy);
            counter=0;
        }
    }
    dummy[counter-1]='\0';
    str=str+QString(dummy);
    fclose(ifi);
    text->setText(str);
}

void frmSetEditor::doUpdate(void)
{
    //write data from QTextEdit to file and read file into Grace
    QString content=text->toPlainText();
    char * dummy=new char[content.size()+2];
    strcpy(dummy,content.toAscii());
    ofstream ofi;
    ofi.open(fname);
    ofi << dummy << endl;
    ofi.close();
    delete[] dummy;
    dummy=new char[256];
    strcpy(dummy,lenFromGraph->text().toAscii());
    gno=atoi(dummy);
    strcpy(dummy,lenFromSet->text().toAscii());
    setno=atoi(dummy);
    /* temporarily disable autoscale */
    int save_autos = autoscale_onread;
    autoscale_onread = AUTOSCALE_NONE;
    int gnos[2],snos[2],created;
    gnos[0]=gno;
    snos[0]=setno;
    //it has been here
    if ((bool)(created=is_set_active(gno, setno)))
    {
        //cout << "set already exists-->update only" << endl;
        SaveSetStatesPrevious(1,gnos,snos,UNDO_COMPLETE);
        curtype = dataset_type(gno, setno);
        killsetdata(gno, setno);
    }
    /*else
        cout << "new set" << endl;*/
    getdata(gno, fname, SOURCE_DISK, LOAD_SINGLE);
    setcomment(gno, setno, "Editor");
    autoscale_onread = save_autos;

    if (created==TRUE)//'created' is not the right word...
    {
        //cout<< "update" << endl;
        SetsModified(1,gnos,snos,UNDO_COMPLETE);
    }
    else
    {
        //cout << "new set" << endl;
        SetsCreated(1,gnos,snos,UNDO_COMPLETE);
    }

    sprintf(dummy," [G%d.S%d]",gno,setno);
    if (!created==false)
        addAditionalDescriptionToLastNode(-1,QObject::tr("Edit set in text editor")+QString(dummy),QString(),-1);
    else
        addAditionalDescriptionToLastNode(-1,QObject::tr("Create set in text editor")+QString(dummy),QString(),-1);
    delete[] dummy;
    mainWin->mainArea->completeRedraw();
}

void frmSetEditor::doClose(void)
{
    //delete files and all entries and hide this Window
    hide();
    text->clear();
    //fname=NULL;
}

frmUndoList::frmUndoList(QWidget * parent):QDialog(parent)
{
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: UndoList"));
    setWindowIcon(QIcon(*GraceIcon));
    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
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
}

void frmUndoList::init(void)
{
    int nr_of_Nodes=getNrOfNodes();
    list->clear();
    chkActive->setChecked(undo_active);
    QStringList StrList;
    char dummy[256];
    sprintf(dummy," (%d / %d)",nr_of_Nodes,max_node_nr);
    setWindowTitle(tr("qtGrace: UndoList")+QString(dummy));
    for (int i=0;i<nr_of_Nodes;i++)
    {
        StrList << getNodeDescription(i);
    }
    list->addItems(StrList);
    list->update(list->model()->index(0,0));
    list->repaint();
}

void frmUndoList::doUndo(void)
{
    int nr=list->count();
    int * selection=new int[nr>0?nr:2];
    int index=0;
    bool sav_und_act=undo_active;
    bool old_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    undo_active=false;
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
    mainWin->mainArea->completeRedraw();
    undo_active=sav_und_act;
    wait_till_update=false;
    chkActive->setChecked(undo_active);
    immediateUpdate=old_upd;
    updateRunning=false;
}

void frmUndoList::doRedo(void)
{
    int nr=list->count();
    int * selection=new int[nr>0?nr:2];
    int index=0;
    bool sav_und_act=undo_active;
    bool old_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    undo_active=false;
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
    int nr=list->count(),read_count;
    int * selection=new int[nr>0?nr:2];
    int index=0,gr_n,new_n;
    for (int i=0;i<nr;i++)
    {
        if (list->item(i)->isSelected()==true)
        {
            selection[index++]=i;
        }
    }
    //index = number of selected entries, selection = indices of selection
    if (FormCommands==NULL)
    {
        FormCommands=new frmCommands(mainWin);
    }
    new_n=gr_n=-1;//current graph number is invalid; in this way the first 'with'-command is included!
    for (int i=0;i<index;i++)
    {
        Node=getNodeByNr(selection[i]);
        nr=nr_of_true_changes(Node->Changes);
        if (nr>0)
        {
            for (int j=0;j<Node->Changes.length();j++)
            {
                if (Node->Changes.at(j).indexOf("with")==0)//commands begins with 'with' --> new graph-nr?
                {
                    strcpy(dummy,Node->Changes.at(j).toAscii());
                    read_count=sscanf(dummy,"with G%d",&new_n);
                    if (new_n!=gr_n || read_count<=0)//include 'with'-commands only if there is really a new graph-nr
                    {
                        FormCommands->list->addItem(Node->Changes.at(j));
                        gr_n=new_n;
                    }
                }
                else//command does not begin with 'with'
                {
                    FormCommands->list->addItem(Node->Changes.at(j));
                }
            }
        }
    }
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
    undo_active=chkActive->isChecked();
}

frmExplorer::frmExplorer(QWidget * parent):QDialog(parent)
{
    GlobalInhibitor=true;
    oldSetting='0';
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Explorer"));
    setWindowIcon(QIcon(*GraceIcon));
    layout=new QGridLayout;
    layout->setMargin(STD_MARGIN);
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
    empty=new QWidget(GraphProperties);
    empty->setMinimumHeight(100);
    GraphProperties->tabMain->layout->addWidget(empty);

    PlotAppearance=new frmPlot_Appearance(this);
    PlotAppearance->buttonGroup->hide();
    PlotAppearance->hide();

    aac=new stdButtonGroup(this);
    connect(aac->cmdAccept,SIGNAL(clicked()),SLOT(doAccept()));
    connect(aac->cmdApply,SIGNAL(clicked()),SLOT(doApply()));
    connect(aac->cmdClose,SIGNAL(clicked()),SLOT(doClose()));
    layout->addWidget(tree,0,0,1,1);

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
        break;
    case (int)'I'://Line-Object
        LineProperties->init(gno);
        break;
    case (int)'B'://Box-Object
        BoxProperties->init(gno);
        break;
    case (int)'P'://Ellipse-Object
        EllipseProperties->init(gno);
        break;
    case (int)'E'://Legend
        GraphProperties->show_graph_data_external(gno);
        break;
    case (int)'S'://Set
        SetProperties->ShowSetData_external(gno,sno);
        break;
    case (int)'A'://Axis
        AxisProperties->curaxis = sno;
        AxisProperties->update_ticks(gno);
        break;
    case (int)'R'://General Settings
        PlotAppearance->init();
        break;
    case (int)'G'://Graph
        GraphProperties->show_graph_data_external(gno);
        break;
    case (int)'L'://Label/Title
        GraphProperties->show_graph_data_external(gno);
        break;
    }
    immediateUpdate=imUpd;
    updateRunning=false;
}

void frmExplorer::itemClickedInTree(char type,int gno,int sno)
{

    setItemVisible(oldSetting,false,gno,sno);
    initItem(type,gno,sno);
    setItemVisible(type,true,gno,sno);

    oldSetting=type;
}

static CMap_entry base_colors[8] = {
    /*0 black  */
    {{0, 0, 0}, "black", COLOR_MAIN, 0},
    /*1 red    */
    {{255, 0, 0}, "red", COLOR_MAIN, 0},
    /*2 green  */
    {{0, 255, 0}, "green", COLOR_MAIN, 0},
    /*3 yellow */
    {{255, 255, 0}, "yellow", COLOR_MAIN, 0},
    /*4 blue   */
    {{0, 0, 255}, "blue", COLOR_MAIN, 0},
    /*5 magenta*/
    {{255, 0, 255}, "magenta", COLOR_MAIN, 0},
    /*6 white  */
    {{255, 255, 255}, "white", COLOR_MAIN, 0},
    /*7 cyan   */
    {{0, 255, 255}, "cyan", COLOR_MAIN, 0}
};

frmColorManagement::frmColorManagement(QWidget * parent):QDialog(parent)
{
    int number,row;
    QString * entr;

    local_cmap_table=NULL;
    allocated_loc_colors=temp_spec_lenght=spectrum_path_length=0;
    temp_spec=NULL;
    path_positions=NULL;
    locColorIcons=NULL;
    locColorPixmaps=NULL;
    locColorNames=NULL;
    tmp_path_length=2;
    tmp_path_pos=new int[2];
    tmp_path_pos[0]=0;
    tmp_path_pos[1]=6;
    tmp_path_point_shown=0;

    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Color Management"));
    setWindowIcon(QIcon(*GraceIcon));
    colorsel=new ColorSelector(this);//shows current palette
    colorsel->lblText->setText(tr("Current color palette:"));
    colorsel->prevent_from_update=true;
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
    layout0->setMargin(STD_MARGIN);
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
    number=4;
    entr=new QString[number];
    entr[0]=QString("Color-spectrum");
    entr[1]=QString("Black-White-spectrum");
    entr[2]=QString("Cold-Hot-spectrum");
    entr[3]=QString("Custom...");
    selStdSpectra=new StdSelector(this,tr("Define spectrum:"),number,entr);
    connect(selStdSpectra,SIGNAL(currentIndexChanged(int)),SLOT(curSpectrumChanged(int)));
    chkInvert=new QCheckBox(tr("Invert spectrum"),this);
    connect(chkInvert,SIGNAL(toggled(bool)),SLOT(changedInvert(bool)));
    selNumberOfColors=new stdIntSelector(this,tr("Number of colors in spectrum:"),10,256);
    selNumberOfColors->setValue(256);
    connect(selNumberOfColors,SIGNAL(currentValueChanged(int)),SLOT(curNumberOfColorsChanged(int)));

    selNumberOfPathPoints=new stdIntSelector(this,tr("Path points:"),2,32);
    connect(selNumberOfPathPoints,SIGNAL(currentValueChanged(int)),SLOT(NrOfPathPointsChanged(int)));
    selNumberOfCurPathPoint=new stdIntSelector(this,tr("Current path point:"),0,1);
    connect(selNumberOfCurPathPoint,SIGNAL(currentValueChanged(int)),SLOT(CurrentPathPointChanged(int)));
    selPathCol=new ColorSelector(this);
    init_color_icons(8,base_colors,allocated_loc_colors,&locColorIcons,&locColorPixmaps,&locColorNames);
    selPathCol->updateColorIcons(8,locColorPixmaps,locColorNames);
    connect(selPathCol,SIGNAL(currentIndexChanged(int)),SLOT(CurrentPathColorChanged(int)));

    locColorIcons=NULL;
    locColorPixmaps=NULL;
    locColorNames=NULL;

    layout1=new QGridLayout;
    layout1->setMargin(STD_MARGIN);
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
    layout->setMargin(STD_MARGIN);
    layout->setSpacing(STD_SPACING);
    aac=new stdButtonGroup(this,true,true,false);
    connect(aac->cmdApply,SIGNAL(clicked()),SLOT(doApply()));
    connect(aac->cmdAccept,SIGNAL(clicked()),SLOT(doAccept()));
    connect(aac->cmdClose,SIGNAL(clicked()),SLOT(doClose()));
    layout->addWidget(colorsel);
    layout->addWidget(grpCurCol);
    layout->addWidget(grpColSpectra);
    layout->addWidget(aac);
    setLayout(layout);

    init();
    colorsel->setCurrentIndex(0);
    QPushButton * emptyButton=new QPushButton("",this);
    emptyButton->setDefault(true);
    emptyButton->hide();
    curColorChanged(0);
    generateSpectrum(0);
    showTempSpectrum();
}

void frmColorManagement::generateSpectrum(int nr)
{
    if (path_positions!=NULL) delete[] path_positions;
    switch (nr)
    {
    case STD_COL_SPECTRUM:
        spectrum_path_length=5;
        path_positions=new int[spectrum_path_length];
        path_positions[0]=4;
        path_positions[1]=7;
        path_positions[2]=2;
        path_positions[3]=3;
        path_positions[4]=1;
        break;
    case BW_SPECTRUM:
        spectrum_path_length=2;
        path_positions=new int[spectrum_path_length];
        path_positions[0]=0;
        path_positions[1]=6;
        break;
    case COLD_HOT_SPECTRUM:
        spectrum_path_length=2;
        path_positions=new int[spectrum_path_length];
        path_positions[0]=4;
        path_positions[1]=1;
        break;
    default:
    case CUSTOM_SPECTRUM:
        spectrum_path_length=tmp_path_length;
        path_positions=new int[tmp_path_length];
        memcpy(path_positions,tmp_path_pos,sizeof(int)*tmp_path_length);
        break;
    }
    temp_spec_lenght=selNumberOfColors->value();
    int segments=spectrum_path_length-1;
    int points_per_segment=(temp_spec_lenght-2)/segments;
    int points_last_segment=temp_spec_lenght-2-(segments-1)*points_per_segment;
    if (temp_spec!=NULL) delete[] temp_spec;
    temp_spec=new CMap_entry[temp_spec_lenght];
    memcpy(temp_spec+0,base_colors+6,sizeof(CMap_entry));//white
    memcpy(temp_spec+1,base_colors+0,sizeof(CMap_entry));//black
    CMap_entry entry;
    memcpy(&entry,base_colors+6,sizeof(CMap_entry));//copy white as default
    int counter=2;
    double z=0.0;
    int r0,r1,g0,g1,b0,b1;
    /*
cout << "temp_spec_lenght=" << temp_spec_lenght << endl;
cout << "segments=" << segments << endl;
cout << "points_per_segment=" << points_per_segment << endl;
cout << "points_last_segment=" << points_last_segment << endl;
cout << "count_ziel=" << (segments-1)*points_per_segment+points_last_segment+2 << endl;
*/
    double z0=0.0,z1=1.0,dz=1.0/(points_per_segment);
    for (int s=0;s<segments-1;s++)//go through all segments except the last one
    {
        r0=base_colors[path_positions[s]].rgb.red;
        g0=base_colors[path_positions[s]].rgb.green;
        b0=base_colors[path_positions[s]].rgb.blue;
        r1=base_colors[path_positions[s+1]].rgb.red;
        g1=base_colors[path_positions[s+1]].rgb.green;
        b1=base_colors[path_positions[s+1]].rgb.blue;
        //cout << "{" << r0 << "|" << g0 << "|" << b0 << "}-->{" << r1 << "|" << g1 << "|" << b1 << "}" << endl;
        for (z=z0;z<z1;z+=dz)
        {
            entry.rgb.red=int(r0*(1.0-z)+r1*z);
            entry.rgb.green=int(g0*(1.0-z)+g1*z);
            entry.rgb.blue=int(b0*(1.0-z)+b1*z);
            entry.cname=new char[16];
            sprintf(entry.cname,"Color %d",counter);
            memcpy(temp_spec+counter,&entry,sizeof(CMap_entry));
            counter++;
        }
    }
    //last segment
    r0=base_colors[path_positions[segments-1]].rgb.red;
    g0=base_colors[path_positions[segments-1]].rgb.green;
    b0=base_colors[path_positions[segments-1]].rgb.blue;
    r1=base_colors[path_positions[segments]].rgb.red;
    g1=base_colors[path_positions[segments]].rgb.green;
    b1=base_colors[path_positions[segments]].rgb.blue;
    points_last_segment=(segments-1)*points_per_segment+points_last_segment+2-counter;
    dz=1.0/(points_last_segment-1.0);
    for (z=0.0;z<=1.0;z+=1.0/(points_last_segment-1.0))
    {
        entry.rgb.red=int(r0*(1.0-z)+r1*z);
        entry.rgb.green=int(g0*(1.0-z)+g1*z);
        entry.rgb.blue=int(b0*(1.0-z)+b1*z);
        entry.cname=new char[16];
        sprintf(entry.cname,"Color %d",counter);
        memcpy(temp_spec+counter,&entry,sizeof(CMap_entry));
        counter++;
        if (counter==temp_spec_lenght) break;
    }
    //cout << "counter0=" << counter << endl;
    while (counter<temp_spec_lenght)
    {
        entry.rgb.red=int(r1);
        entry.rgb.green=int(g1);
        entry.rgb.blue=int(b1);
        entry.cname=new char[16];
        sprintf(entry.cname,"Color %d",counter);
        memcpy(temp_spec+counter,&entry,sizeof(CMap_entry));
        counter++;
    }
    //cout << "counter1=" << counter << endl;
    if (chkInvert->isChecked()==true)
    {
        //cout << "spektrum invers" << endl;
        for (int i=2;i<(temp_spec_lenght-2)/2;i++)
        {
            memcpy(&entry,temp_spec+i,sizeof(CMap_entry));
            memcpy(temp_spec+i,temp_spec+(temp_spec_lenght-i+1),sizeof(CMap_entry));
            memcpy(temp_spec+(temp_spec_lenght-i+1),&entry,sizeof(CMap_entry));
        }
    }
}

void frmColorManagement::showTempSpectrum(void)
{
    int target_length=2*256;
    int col_length=int(target_length*1.0/temp_spec_lenght);
    target_length=col_length*temp_spec_lenght;
    QColor colo;
    QPixmap pix(target_length,20);
    QPainter paint(&pix);
    for (int i=0;i<temp_spec_lenght;i++)
    {
        colo=QColor(temp_spec[i].rgb.red,temp_spec[i].rgb.green,temp_spec[i].rgb.blue);
        paint.setPen(colo);
        paint.setBrush(colo);
        paint.fillRect(i*col_length,0,col_length,20,colo);
    }
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
        delete[] local_cmap_table[i].cname;
    }
    delete[] local_cmap_table;
    map_entries=sizeof(cmap_init)/sizeof(CMap_entry);
    //cout << "set std colors: " << map_entries << endl;
    local_cmap_table=new CMap_entry[map_entries];
    for (int i=0;i<map_entries;i++)
    {
        memcpy(local_cmap_table+i,cmap_init+i,sizeof(CMap_entry));
        local_cmap_table[i].cname=new char[2+strlen(cmap_init[i].cname)];
        strcpy(local_cmap_table[i].cname,cmap_init[i].cname);
    }
    init_color_icons(map_entries,local_cmap_table,allocated_loc_colors,&locColorIcons,&locColorPixmaps,&locColorNames);
    colorsel->updateColorIcons(map_entries,locColorPixmaps,locColorNames);
}

void frmColorManagement::doSetSpectrum(void)
{
    //cout << "set spectrum" << endl;
    generateSpectrum(selStdSpectra->currentIndex());
    showTempSpectrum();
    for (int i=0;i<map_entries;i++)
    {
        delete[] local_cmap_table[i].cname;
    }
    delete[] local_cmap_table;
    map_entries=temp_spec_lenght;
    local_cmap_table=new CMap_entry[map_entries];
    for (int i=0;i<map_entries;i++)
    {
        memcpy(local_cmap_table+i,temp_spec+i,sizeof(CMap_entry));
        local_cmap_table[i].cname=new char[2+strlen(temp_spec[i].cname)];
        strcpy(local_cmap_table[i].cname,temp_spec[i].cname);
    }
    init_color_icons(map_entries,local_cmap_table,allocated_loc_colors,&locColorIcons,&locColorPixmaps,&locColorNames);
    colorsel->updateColorIcons(map_entries,locColorPixmaps,locColorNames);
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
}

void frmColorManagement::curNumberOfColorsChanged(int nr)
{
    if (GlobalInhibitor==true) return;
    //cout << "color count changed " << nr << endl;
    curSpectrumChanged(selStdSpectra->currentIndex());
}

void frmColorManagement::changedInvert(bool nr)
{
    curSpectrumChanged(selStdSpectra->currentIndex());
}

void frmColorManagement::colorCompositionChanged(int nr)
{
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
    entry.cname=new char[ledColName->text().length()+2];
    strcpy(entry.cname,ledColName->text().toAscii().constData());
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
    ledColName->setText(QString(entry->cname));
    char dummy[8];
    sprintf(dummy,"%d",nr);
    lblColNumber->setText(tr("Number = ")+QString(dummy));
    GlobalInhibitor=false;
}

void frmColorManagement::init(void)
{
    int * real_colors=new int[4];
    int nr_of_aux_cols;
    map_entries=get_main_color_indices(&real_colors,&nr_of_aux_cols);
    //cout << "init: " << map_entries << endl;
    if (local_cmap_table!=NULL)
    {
        for (int i=0;i<map_entries;i++)
            delete[] local_cmap_table[i].cname;
        delete[] local_cmap_table;
    }
    local_cmap_table=new CMap_entry[map_entries];
    for (int i=0;i<map_entries;i++)
    {
        memcpy(local_cmap_table+i,cmap_table+real_colors[i],sizeof(CMap_entry));
        local_cmap_table[i].cname=new char[strlen(cmap_table[real_colors[i]].cname)+2];
        strcpy(local_cmap_table[i].cname,cmap_table[real_colors[i]].cname);
    }
    init_color_icons(map_entries,local_cmap_table,allocated_loc_colors,&locColorIcons,&locColorPixmaps,&locColorNames);
    //Local colors are only main colors
    colorsel->updateColorIcons(map_entries,locColorPixmaps,locColorNames);
}

void frmColorManagement::doApply(void)
{
    int ret=QMessageBox::question(this,tr("Change color palette?"),tr("Do you really want to change the current color palette?"),QMessageBox::Ok,QMessageBox::Cancel);
    if (ret==QMessageBox::Ok)
    {
        cout << "apply to color palette" << endl;
        SaveCurrentColorMap();
        //first: save aux colors
        int * real_colors=new int[4];
        int nr_of_aux_colors,new_nr_of_colors;
        int real_map_entries=get_main_color_indices(&real_colors,&nr_of_aux_colors);
        int * aux_colors=new int[2+nr_of_aux_colors];//the old color-indices of auxilliary colors
        CMap_entry * aux_map_entries=new CMap_entry[2+nr_of_aux_colors];//the auxilliary colors
        cout << "nr_of_aux_colors=" << nr_of_aux_colors << endl;
        nr_of_aux_colors=0;
        for (int i=0;i<number_of_colors();i++)
        {
            if (cmap_table[i].ctype!=COLOR_MAIN)
            {
                memcpy(aux_map_entries+nr_of_aux_colors,cmap_table+i,sizeof(CMap_entry));
                aux_map_entries[nr_of_aux_colors].cname=new char[2+sizeof(cmap_table[i].cname)];
                strcpy(aux_map_entries[nr_of_aux_colors].cname,cmap_table[i].cname);
                aux_colors[nr_of_aux_colors]=i;
                cout << i << endl;
                nr_of_aux_colors++;
            }
        }

        int number_of_new_cols=nr_of_aux_colors+map_entries;//aux-colors+(new)main-colors
        if (number_of_new_cols>MAXCOLORS) number_of_new_cols=MAXCOLORS;
        cout << "nr_of_aux_colors=" << nr_of_aux_colors << " number_of_new_cols=" << number_of_new_cols << endl;
        realloc_colors(number_of_new_cols);
        int aux_counter=0,colors_counter=0;
        for (int i=0;i<number_of_new_cols;i++)//fill new color table with new colors
        {
            if (aux_counter>=nr_of_aux_colors)//space for a main color
            {
                memcpy(cmap_table+i,local_cmap_table+colors_counter,sizeof(CMap_entry));
                cmap_table[i].cname = copy_string(cmap_table[i].cname, local_cmap_table[colors_counter].cname);
                colors_counter++;
            }
            else if (aux_colors[aux_counter]==i)//current number has been an aux-color
            {
                memcpy(cmap_table+i,aux_map_entries+aux_counter,sizeof(CMap_entry));
                cmap_table[i].cname = copy_string(cmap_table[i].cname, aux_map_entries[aux_counter].cname);
                aux_counter++;
            }
            else//space for a main color
            {
                memcpy(cmap_table+i,local_cmap_table+colors_counter,sizeof(CMap_entry));
                cmap_table[i].cname = copy_string(cmap_table[i].cname, local_cmap_table[colors_counter].cname);
                colors_counter++;
            }
        }
        ReqUpdateColorSel = TRUE;
        /*IntstallColorMap(map_entries,local_cmap_table);*/
        CurrentColorMapChanged();
        mainWin->mainArea->completeRedraw();
    }
    else
        cout << "do not change color palette" << endl;
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
    hide();
}

frmRealTimeInputManager::frmRealTimeInputManager(QWidget * parent):QDialog(parent)
{
    setFont(stdFont);
    setWindowTitle(tr("qtGrace: Real Time Input"));
    setWindowIcon(QIcon(*GraceIcon));
    frmOpen=NULL;
    layout=new QVBoxLayout;
    layout->setMargin(STD_MARGIN);
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
    strcpy(dummy,fname.toAscii().constData());
    if (openPipe(dummy,&fd)!=RETURN_SUCCESS)
    {
        errmsg(tr("Unable to open pipe!").toAscii().constData());
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

#define MAX_INPUT_LINE_LENGTH 2048
char linebuffer[MAX_INPUT_LINE_LENGTH];
int nextlinebufferpos=0;

int readLineFromFile(QFile * fi,struct cvs_import_infos & import_info,char ** tline)
{//read from fi until EOF is reached or a lineSeparator-char is found (uses linebuffer if lineSeparator!='\n')
    static int counter,stringcounter,tokenlength,colcounter;
    static QByteArray line;
    static char * tmp_pointer;
    tmp_pointer=NULL;
    tokenlength=stringcounter=counter=0;//stringcounter counts '"', counter counts tokens (only 0 or 1 token read)
    if (fi->atEnd()==true && nextlinebufferpos==0)
    {
        //cout << "File at End !" << endl;
        return RETURN_FAILURE;//EOF and no more data in buffer
    }
    if (import_info.datsep=='\n')//simple case
    {
        //cout << "simple case" << endl;
        while (!fi->atEnd() && counter<1)
        {
            line = fi->readLine(MAX_INPUT_LINE_LENGTH);
            if (line.length()<1) return RETURN_FAILURE;
            delete[] *tline;
            (*tline)=new char[line.length()+2];
            strncpy(*tline,line.constData(),line.length());
            (*tline)[line.length()-1]='\0';//-1 because new-line-character is included and we don't need it!
            (*tline)[line.length()]='\0';
            (*tline)[line.length()+1]='\0';
            counter++;
        }
    }
    else//special character for line-separation
    {
        //cout << "complex case = #" << linebuffer << "#" << endl;
        while ((!fi->atEnd() || nextlinebufferpos>0) && counter<1)
        {
            if (!fi->atEnd())//more data in file-->read data to buffer
            {
                line = fi->readLine(MAX_INPUT_LINE_LENGTH-nextlinebufferpos);
                strncpy(linebuffer+nextlinebufferpos,line.constData(),line.length());/// Possible Problem: line.length() > MAX_INPUT_LINE_LENGTH !!
                nextlinebufferpos+=line.length();
                if (nextlinebufferpos>MAX_INPUT_LINE_LENGTH-1) nextlinebufferpos>MAX_INPUT_LINE_LENGTH-1;//if this is necessary, the program will crash anyway
                linebuffer[nextlinebufferpos]='\0';
                //cout << "new Linebuffer = #" << linebuffer << "#" << endl;
            }
            //nextlinebufferpos is the amount of data still present in linebuffer
            //use linebuffer to get data
            colcounter=0;
            //now we have to look for a suitable lineSeparator
            for (int k=0;k<nextlinebufferpos;k++)
            {
                if (linebuffer[k]==import_info.textsep)
                {
                    stringcounter++;
                }
                else if (linebuffer[k]==import_info.colsep)
                {
                    colcounter++;
                }
                else if (linebuffer[k]==import_info.datsep && stringcounter%2==0)
                {
                    /// TODO: INSERT MISSING SEARCH RESTRICTIONS ABOUT string-separators
                    tokenlength=k;
                    break;
                }
            }
            //tokenlength should now give an indication how long a line is
            if (tokenlength<1 && colcounter>0)
            {
                //cout << "colcounter>0 nextlinebufpos=" << nextlinebufferpos << endl;
                tokenlength=nextlinebufferpos-1;
            }
            else if (tokenlength<1)
            {
                return RETURN_FAILURE;
            }
            delete[] *tline;
            (*tline)=new char[tokenlength+2];
            strncpy(*tline,linebuffer,tokenlength);
            (*tline)[tokenlength]='\0';
            (*tline)[tokenlength+1]='\0';
            tmp_pointer=new char[nextlinebufferpos+2];
            strncpy(tmp_pointer,linebuffer+tokenlength+1,nextlinebufferpos);
            tmp_pointer[nextlinebufferpos]='\0';
            strncpy(linebuffer,tmp_pointer,nextlinebufferpos-tokenlength-1);
            linebuffer[nextlinebufferpos-tokenlength-1]='\0';
            nextlinebufferpos-=tokenlength+1;
            delete[] tmp_pointer;
            counter++;
        }//end while data present
    }//end if (complex-case)
    return RETURN_SUCCESS;
}

int get_next_token(char * tokenline,struct cvs_import_infos & import_info,char * n_token,int * read_token_length)
{//searches for the next token and replaces the text-separator if neccessary
    static int len,stringcounter,dat_sep_occurrance,index;
    static char * tmp_string;
    len=strlen(tokenline);
    if (len<1) return RETURN_FAILURE;
    tmp_string=new char[2*len+2];
    dat_sep_occurrance=0;
    stringcounter=0;
    for ((*read_token_length)=0;(*read_token_length)<len;(*read_token_length)++)
    {
        if (tokenline[*read_token_length]==import_info.textsep)
        {
            stringcounter++;
        }
        else if (tokenline[*read_token_length]==import_info.colsep && stringcounter%2==0)
        {
            dat_sep_occurrance++;
            //(*read_token_length)++;
            break;
        }
    }
    strncpy(tmp_string,tokenline,(*read_token_length));
    tmp_string[(*read_token_length)]='\0';
    (*read_token_length)++;
    dat_sep_occurrance=stringcounter;
    stringcounter=0;
    len=strlen(tmp_string);
    index=0;
    if (dat_sep_occurrance>0)
    {
        for (int i=0;i<len;i++)
        {
            if (tmp_string[i]==import_info.textsep)
            {
                stringcounter++;
                if (stringcounter==1 || stringcounter==dat_sep_occurrance)
                {
                    n_token[index++]='"';
                }
                else
                {
                    n_token[index++]=tmp_string[i];
                }
            }
            else
            {
                n_token[index++]=tmp_string[i];
            }
        }
    }
    else
    {
        strcpy(n_token,tmp_string);
    }
    len=strlen(n_token);
    for (int l=0;l<4;l++)
    {
        n_token[len+l]='\0';
    }
    delete[] tmp_string;
    return RETURN_SUCCESS;
}

void redesign_tokenline(char * tokenline,struct cvs_import_infos & import_info)
{
    static int stringcounter,len,index;
    char * newtokenline;
    index=stringcounter=0;
    len=strlen(tokenline);
    newtokenline=new char[2*len+2];
    newtokenline[index++]=import_info.textsep;
    for (int i=0;i<len;i++)
    {
        if (tokenline[i]==import_info.textsep)
        {
            stringcounter++;
            newtokenline[index++]=tokenline[i];
        }
        else if (tokenline[i]==import_info.datsep && stringcounter%2==0)
        {
            newtokenline[index++]=import_info.textsep;
            newtokenline[index++]=' ';
            newtokenline[index++]=import_info.textsep;
        }
        else
        {
            newtokenline[index++]=tokenline[i];
        }
    }
    newtokenline[index++]=import_info.textsep;
    newtokenline[index++]='\0';
    strcpy(tokenline,newtokenline);
    delete[] newtokenline;
}

int guess_column_format(struct cvs_import_infos & import_info, int * ncols, int ** formats)
{
    char * tokenline=new char[MAX_INPUT_LINE_LENGTH*2+4];
    char * linetoken=new char[2];
    char * n_token=new char[MAX_INPUT_LINE_LENGTH];
    char * token;
    int counter,quoted,reserved,len;
    double value;
    (*ncols)=0;
    reserved=2;
    delete[] (*formats);
    (*formats)=new int[reserved];
    Dates_format df_pref, ddummy;
    const char *sdummy;
    int read_token_length,offset;
    int * int_tmp;
    int * old_format=NULL,old_f_cols;
    QByteArray line;
    QFile * file=new QFile(import_info.filename);
    df_pref=get_date_hint();
    counter=0;
    nextlinebufferpos=0;
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        errmsg(QObject::tr("Unable to open csv-file.").toAscii().constData());
        delete[] tokenline;
        delete[] linetoken;
        delete[] n_token;
        return RETURN_FAILURE;
    }
    while (!file->atEnd() && counter<import_info.headerlines)//read header and ignore it!
    {
        line = file->readLine(MAX_INPUT_LINE_LENGTH);
        counter++;
    }
    counter=0;
    while (readLineFromFile(file,import_info,&linetoken)==RETURN_SUCCESS && counter<3)//read max three lines!
    {
        *ncols=0;
        //cout << "Line " << counter << ": linetoken=#" << linetoken << "#" << endl;
        strcpy(tokenline,linetoken);
        len=strlen(tokenline);
        for (int l=0;l<4;l++)
        {
            tokenline[len+l]='\0';
        }
        offset=0;
        while (get_next_token(tokenline+offset,import_info,n_token,&read_token_length)==RETURN_SUCCESS)
        {//loop through tokens in one line
            offset+=read_token_length;
            //cout << "vor exchange:  n_token=#" << n_token << "#" << int('\n') << endl;
            if (import_info.dec_sep_komma==true) exchangePointComma(n_token);
            //cout << "nach exchange: n_token=#" << n_token << "#" << tokenline+offset << "#" << int(tokenline[offset]) << endl;
            //cout << "n_token=#" << n_token << "# next=#" << tokenline+offset << "#" << endl;
            next_token(n_token, &token, &quoted);
            if (quoted)
            {
                (*formats)[*ncols] = FFORMAT_STRING;
                (*ncols)++;
            }
            else if (parse_date(token, df_pref, FALSE, &value, &ddummy) == RETURN_SUCCESS)
            {
                (*formats)[*ncols] = FFORMAT_DATE;
                (*ncols)++;
            }
            else if (parse_float(token, &value, &sdummy) == RETURN_SUCCESS)
            {
                (*formats)[*ncols] = FFORMAT_NUMBER;
                (*ncols)++;
            }
            else
            {
                /* last resort - treat the field as string, even if not quoted */
                (*formats)[*ncols] = FFORMAT_STRING;
                (*ncols)++;
            }
            //cout << "token=#" << n_token << "# Format=" << (*formats)[(*ncols)-1] << endl;
            if (*ncols==reserved)//expand space for formats
            {
                int_tmp=new int[reserved*2];
                memcpy(int_tmp,*formats,sizeof(int)*(reserved));
                reserved*=2;
                delete[] (*formats);
                *formats=int_tmp;
            }
        }//end loop through tokens in one line
        //we have read one line!
        if (counter==0)
        {
            old_format=new int[*ncols];
            old_f_cols=*ncols;
            memcpy(old_format,*formats,sizeof(int)*(*ncols));//save format of first lines
        }
        else
        {
            if (old_f_cols!=*ncols) goto end_guess_col_f;
            for (int l=0;l<*ncols;l++)
            {
                if (((*formats)[l])!=old_format[l]) goto end_guess_col_f;
            }
        }
        counter++;//line-counter
    }
    file->close();//we opended the file for guessing only
    delete file;
    if (old_format!=NULL) delete[] old_format;
    delete[] tokenline;
    delete[] linetoken;
    delete[] n_token;
    return RETURN_SUCCESS;
end_guess_col_f:
    if (old_format!=NULL) delete[] old_format;
    delete[] tokenline;
    delete[] linetoken;
    delete[] n_token;
    return RETURN_FAILURE;
}

int csvImport(struct cvs_import_infos & import_info)
{
    nextlinebufferpos=0;
    //first action: activate sets to have access to memory for results
    //cout << "Import: Activating sets nr=" << import_info.nr_of_target_sets << endl;
    for (int i=0;i<import_info.nr_of_target_sets;i++)
    {
        import_info.target_sets[i]=nextset(import_info.gno);
        setlength(import_info.gno,import_info.target_sets[i],1024);//we set a default length
        //cout << i << ": set=" << import_info.target_sets[i] << " length=" << getsetlength(import_info.gno,import_info.target_sets[i]) << endl;
        if (import_info.max_col_per_set[i]==2)
            set_dataset_type(import_info.gno,import_info.target_sets[i],SET_XY);
        else if (import_info.max_col_per_set[i]==3)
            set_dataset_type(import_info.gno,import_info.target_sets[i],SET_XYDY);
        else if (import_info.max_col_per_set[i]==4)
            set_dataset_type(import_info.gno,import_info.target_sets[i],SET_XYDXDY);
        else if (import_info.max_col_per_set[i]==5)
            set_dataset_type(import_info.gno,import_info.target_sets[i],SET_XYHILO);
        else
            set_dataset_type(import_info.gno,import_info.target_sets[i],SET_XYDXDXDYDY);
    }
    char * tokenline=new char[MAX_INPUT_LINE_LENGTH*2+4];
    char * linetoken=new char[2];
    char * n_token=new char[MAX_INPUT_LINE_LENGTH];
    int counter,col_counter,len;
    double value;
    Dates_format df_pref, ddummy;
    const char *sdummy;
    int read_token_length,offset;
    QByteArray line;
    QFile * file=new QFile(import_info.filename);
    counter=0;
    df_pref=get_date_hint();
    //second action: try to actually open file
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        errmsg(QObject::tr("Unable to open csv-file.").toAscii().constData());
        delete[] tokenline;
        delete[] linetoken;
        delete[] n_token;
        return RETURN_FAILURE;
    }
    //third action: read and ignore header lines
    while (!file->atEnd() && counter<import_info.headerlines)
    {
        line = file->readLine(MAX_INPUT_LINE_LENGTH);
        counter++;
    }
    counter=0;
    //fourth action: do the import
    while (readLineFromFile(file,import_info,&linetoken)==RETURN_SUCCESS)
    {
        ///cout << "Line " << counter << ": linetoken=#" << linetoken << "#" << endl;
        strcpy(tokenline,linetoken);
        len=strlen(tokenline);
        for (int l=0;l<4;l++)
        {
            tokenline[len+l]='\0';
        }
        offset=0;
        col_counter=0;
        while (get_next_token(tokenline+offset,import_info,n_token,&read_token_length)==RETURN_SUCCESS)
        {//loop through tokens in one line
            offset+=read_token_length;
            ///cout << "n_token=#" << n_token << "# next=#" << tokenline+offset << "#" << endl;
            if (import_info.col_format[col_counter]==FFORMAT_NUMBER)
            {
                //cout << "vor exchange" << endl;
                if (import_info.dec_sep_komma==true) exchangePointComma(n_token);
                value=0.0;
                //cout << "nach exchange#" << n_token << "#" << endl;
                parse_float(n_token, &value, &sdummy);
            }
            else if (import_info.col_format[col_counter]==FFORMAT_DATE)
            {
                parse_date(n_token, df_pref, FALSE, &value, &ddummy);
            }
            else
            {
                /// cout << "TOKEN-STRING=" << n_token << endl;
                value=0.0;
            }
            //we should now have a 'value' for our column-entry
            //cout << "determined value = " << value << endl;
            if (import_info.import_to_set[col_counter]==-2)//all sets
            {
                //cout << "value=" << value << " allsets nr=" << import_info.nr_of_target_sets << endl;
                for (int l=0;l<import_info.nr_of_target_sets;l++)
                {
                    /*if (import_info.target_sets[l]>=0)
                    {
                    cout << "set S" << import_info.target_sets[l] << " value=" << value << endl;*/
                    g[import_info.gno].p[import_info.target_sets[l]].data.ex[import_info.target_cols[col_counter]][counter]=value;
                    /*}
                    else
                    {
                    cout << "ERROR: target < 0" << endl;//this should never happen
                    }*/
                }
            }
            else if (import_info.import_to_set[col_counter]>=0)//normal set numbers
            {
                //cout << "target-set:" << import_info.import_to_set[col_counter] << " target_cols:" << import_info.target_cols[col_counter] << " value=" << value << endl;
                g[import_info.gno].p[import_info.import_to_set[col_counter]].data.ex[import_info.target_cols[col_counter]][counter]=value;
            }
            col_counter++;//column-counter
        }//end loop through tokens in one line
        counter++;//line-counter
        if (counter%1024==0)//we have to expand the amount of space in the sets
        {
            for (int l=0;l<import_info.nr_of_target_sets;l++)
                setlength(import_info.gno,import_info.target_sets[l],1024+getsetlength(import_info.gno,import_info.target_sets[l]));
        }
    }//end loop through lines in file
    for (int l=0;l<import_info.nr_of_target_sets;l++)
        setlength(import_info.gno,import_info.target_sets[l],counter);//reset the set-length to the actual number of datapoints read
    file->close();
    delete file;
    delete[] tokenline;
    delete[] linetoken;
    delete[] n_token;
    return RETURN_SUCCESS;
}

frmCSVImporter::frmCSVImporter(QWidget * parent):QDialog(parent)
{
    import_info.filename=NULL;
    import_info.gno=0;
    import_info.headerlines=0;
    import_info.columns_to_read=0;
    import_info.target_cols=NULL;
    import_info.col_format=NULL;
    import_info.import_to_set=NULL;
    import_info.nr_of_target_sets=0;
    import_info.target_sets=NULL;
    import_info.max_col_per_set=NULL;
    import_info.colsep=';';
    import_info.datsep='\n';
    import_info.textsep='"';
    import_info.dec_sep_komma=false;

    setFont(stdFont);
    setWindowTitle(tr("qtGrace: CSV-import"));
    setWindowIcon(QIcon(*GraceIcon));

    // It is not good idea to use "this" in constructor.
    // Moved code to frmCSVImporter::doOpen(void)
    //fileDialog=new QFileDialog(this,tr("Open CSV file"));
    //fileDialog->hide();

    layout=new QGridLayout;
    layout->setMargin(STD_MARGIN);
    layout->setSpacing(STD_SPACING);
    cmdOpenFile=new QPushButton(tr("Select CSV file"),this);
    connect(cmdOpenFile,SIGNAL(clicked()),SLOT(doOpen()));
    cmdReguess=new QPushButton(tr("Re-guess columns"),this);
    connect(cmdReguess,SIGNAL(clicked()),SLOT(doReguess()));
    ledFileName=new stdLineEdit(this,tr("Filename:"),false);
    ledFileName->setText(QString(""));
    lblFileContents=new QTextEdit(QString(""),this);
    lblFileContents->setReadOnly(TRUE);
    lblFileContents->setMinimumHeight(120);
    ledColSep=new stdLineEdit(this,tr("Column-separator:"),false);
    ledColSep->setText(QString(";"));
    ledSetSep=new stdLineEdit(this,tr("Data-set-separator:"),false);
    ledSetSep->setText(QString("\\n"));
    ledTextSep=new stdLineEdit(this,tr("Text-separator:"),false);
    ledTextSep->setText(QString("\""));
    selHeaderLines=new stdIntSelector(this,tr("Number of header-lines to ignore:"),0,100);
    connect(selHeaderLines,SIGNAL(currentValueChanged(int)),SLOT(headerLinesChanged(int)));
    selSetsToRead=new stdIntSelector(this,tr("Number of sets to read:"),1,512);
    connect(selSetsToRead,SIGNAL(currentValueChanged(int)),SLOT(setCountChanged(int)));
    graphselector=new uniList(GRAPHLIST,this);
    lblGraphSelector=new QLabel(tr("Target graph:"),this);
    lblFileCont=new QLabel(tr("File contents:"),this);
    chkKommaDecimalSep=new QCheckBox(tr("Decimal separator is ','"),this);
    chkAutoscaleOnRead=new QCheckBox(tr("Autoscale on read"),this);
    chkAutoscaleOnRead->setChecked(true);//autscale is default
    aac=new stdButtonGroup(this,true,true,false);
    connect(aac->cmdAccept,SIGNAL(clicked()),SLOT(doAccept()));
    connect(aac->cmdApply,SIGNAL(clicked()),SLOT(doApply()));
    connect(aac->cmdClose,SIGNAL(clicked()),SLOT(doClose()));

    int index=0,index2=0;
    layout->addWidget(ledFileName,index,0,1,1);
    layout->addWidget(cmdOpenFile,index++,1,1,1);
    layout->addWidget(lblFileCont,index,0,1,1);
    layout->addWidget(lblGraphSelector,index++,1,1,1);
    layout->addWidget(lblFileContents,index,0,1,1);
    layout->addWidget(graphselector,index++,1,1,1);
    layout->addWidget(selHeaderLines,index,0,1,1);
    layout->addWidget(ledSetSep,index++,1,1,1);
    layout->addWidget(ledColSep,index,0,1,1);
    layout->addWidget(ledTextSep,index++,1,1,1);
    layout->addWidget(selSetsToRead,index,0,1,1);
    layout->addWidget(cmdReguess,index++,1,1,1);
    layout->addWidget(chkKommaDecimalSep,index,0,1,1);
    layout->addWidget(chkAutoscaleOnRead,index++,1,1,1);

    scroll=new QScrollArea(this);
    empty=new QLabel(this);
    empty->setMinimumSize(600,100);
    grid1=new QGridLayout(this);
    grid1->setMargin(STD_MARGIN);
    grid1->setSpacing(STD_SPACING);

    lblDescription=NULL;
    recreate_input_selectors();

    empty->setLayout(grid1);
    scroll->setWidget(empty);

    layout->addWidget(scroll,index++,0,1,2);
    layout->addWidget(aac,index++,0,1,2);
    setLayout(layout);
}

void frmCSVImporter::read_inputs(void)
{
    if (import_info.filename!=NULL) delete[] import_info.filename;
    import_info.filename=new char[ledFileName->text().length()+2];
    strcpy(import_info.filename,ledFileName->text().toAscii().constData());
    import_info.headerlines=selHeaderLines->value();
    import_info.colsep=ledColSep->text().at(0).toAscii();
    if (!ledColSep->text().compare(QString("\\n")))
        import_info.colsep='\n';
    else if (!ledColSep->text().compare(QString("\\t")))
        import_info.colsep='\t';
    import_info.datsep=ledSetSep->text().at(0).toAscii();
    if (!ledSetSep->text().compare(QString("\\n")))//every data point is in a seperate line
        import_info.datsep='\n';
    else if (!ledSetSep->text().compare(QString("\\t")))
        import_info.datsep='\t';
    import_info.textsep=ledTextSep->text().at(0).toAscii();
    if (!ledTextSep->text().compare(QString("\\n")))
        import_info.textsep='\n';
    else if (!ledTextSep->text().compare(QString("\\t")))
        import_info.textsep='\t';
    import_info.nr_of_target_sets=selSetsToRead->value();
    import_info.dec_sep_komma=chkKommaDecimalSep->isChecked();
    int nr_of_sel,*sel=new int[2],maxplot,setno,free_sets_found=0;
    graphselector->get_selection(&nr_of_sel,&sel);
    if (nr_of_sel<1)
    {
        errmsg(tr("Please select a single graph!").toAscii().constData());
        ApplyError=true;
    }
    else if (nr_of_sel>1)
    {
        errmsg(tr("Please select only one graph!").toAscii().constData());
        ApplyError=true;
    }
    else
    {
        import_info.gno=sel[0];
        if (import_info.target_sets!=NULL) delete[] import_info.target_sets;
        import_info.target_sets=new int[import_info.nr_of_target_sets];
        maxplot = number_of_sets(import_info.gno);
        free_sets_found=0;
        for (setno = 0; setno < maxplot; setno++)
        {
            if (!is_set_active(import_info.gno, setno))
            {
                import_info.target_sets[free_sets_found++]=setno;
            }
            if (import_info.nr_of_target_sets==free_sets_found)//we found enough
                break;
        }
        if (import_info.nr_of_target_sets>free_sets_found)//not enough sets found
        {
            import_info.target_sets[free_sets_found++]=maxplot;//first new set is number of allocated sets
            while (import_info.nr_of_target_sets>free_sets_found)
            {
                import_info.target_sets[free_sets_found]=import_info.target_sets[free_sets_found-1]+1;
                free_sets_found++;
            }
        }
        /*cout << "Predicted set-numbers:" << endl;
    for (int i=0;i<import_info.nr_of_target_sets;i++)
    {
    cout << i << ": S" << import_info.target_sets[i] << endl;
    }*/

    }
}

void frmCSVImporter::read_import_settings(void)//this will only be called directly before reading action begins
{//we have to read the column-import-settings and look for the set-numbers really in use
    if (import_info.col_format!=NULL) delete[] import_info.col_format;
    import_info.col_format=new int[allocated_selectors];//allocated selectors = columns_to_read = guessed number of columns in the file
    if (import_info.target_cols!=NULL) delete[] import_info.target_cols;
    import_info.target_cols=new int[allocated_selectors];
    if (import_info.import_to_set!=NULL) delete[] import_info.import_to_set;
    import_info.import_to_set=new int[allocated_selectors];
    if (import_info.target_sets!=NULL) delete[] import_info.target_sets;
    import_info.target_sets=new int[allocated_selectors];
    if (import_info.max_col_per_set!=NULL) delete[] import_info.max_col_per_set;
    import_info.max_col_per_set=new int[allocated_selectors];
    import_info.nr_of_target_sets=0;
    bool found;
    int j;
    for (int i=0;i<allocated_selectors;i++)//go through the columns
    {
        import_info.target_sets[i]=-1;
        import_info.max_col_per_set[i]=2;//minimum =2 columns per set =XY
        import_info.col_format[i]=selFormat[i]->currentValue();
        import_info.import_to_set[i]=selTargetSet[i]->currentValue();
        import_info.target_cols[i]=selTargetColumn[i]->currentValue();
        if (import_info.import_to_set[i]>=0)//new real set-number
        {
            found=false;
            for (j=0;j<import_info.nr_of_target_sets;j++)//look trough all previous sets for the new set-id
            {
                if (import_info.import_to_set[i]==import_info.target_sets[j])
                {
                    found=true;
                    break;
                }
            }
            if (found==false)//set-id is really new
            {
                import_info.target_sets[import_info.nr_of_target_sets++]=import_info.import_to_set[i];
            }
        }
    }

    sort(import_info.nr_of_target_sets,import_info.target_sets);//sort set-IDs for simplicity

    /*if (import_info.nr_of_target_sets<=0)
    {
    cout << "no suitable set numbers" << endl;
    }
    else*/
    if (import_info.nr_of_target_sets>0)
    {
        //cout << "found set numbers:" << endl;
        for (int i=0;i<import_info.nr_of_target_sets;i++)
        {
            for (j=0;j<allocated_selectors;j++)//look through the selectors for a max-column-number for every set
            {
                if (import_info.import_to_set[j]==import_info.target_sets[i] && import_info.target_cols[j]+1>import_info.max_col_per_set[i]) import_info.max_col_per_set[i]=import_info.target_cols[j]+1;
            }
        }
        /*for (int i=0;i<import_info.nr_of_target_sets;i++)
        {
        cout << "S" << import_info.target_sets[i] << " max_cols=" << import_info.max_col_per_set[i] << endl;
        }*/
    }
}

void frmCSVImporter::recreate_input_selectors(void)
{
    if (import_info.columns_to_read<=0) return;//columns_to_read is the guessed number of columns in the file --> we create new input-selectors for every column
    if (lblDescription!=NULL)//we already had selectors --> delete the old ones
    {
        for (int i=0;i<allocated_selectors;i++)
        {
            delete lblDescription[i];
            delete selFormat[i];
            delete selTargetSet[i];
            delete selTargetColumn[i];
        }
        delete[] lblDescription;
        delete[] selFormat;
        delete[] selTargetSet;
        delete[] selTargetColumn;
    }
    allocated_selectors=import_info.columns_to_read;
    empty->setMinimumSize(this->width()-18,allocated_selectors*30+10);
    empty->setMaximumSize(this->width()-18,allocated_selectors*30+10);
    lblDescription=new QLabel *[allocated_selectors];
    selFormat=new StdSelector *[allocated_selectors];
    selTargetSet=new StdSelector *[allocated_selectors];
    selTargetColumn=new StdSelector *[allocated_selectors];
    QString * entr=new QString[3];
    entr[0]=tr("Number");
    entr[1]=tr("Date/Time");
    entr[2]=tr("String");
    QString * entr2=new QString[2+import_info.nr_of_target_sets];
    int * entry_values2=new int[2+import_info.nr_of_target_sets];
    QString * entr3=new QString[6];
    entr2[0]=QString("None");
    entr2[1]=QString("All");
    entry_values2[0]=-1;
    entry_values2[1]=-2;
    for (int i=0;i<import_info.nr_of_target_sets;i++)
    {
        sprintf(dummy,"G%d.S%d",import_info.gno,import_info.target_sets[i]);
        entr2[i+2]=QString(dummy);
        entry_values2[i+2]=import_info.target_sets[i];
    }
    entr3[0]=QString("X");
    entr3[1]=QString("Y");
    entr3[2]=QString("Y1");
    entr3[3]=QString("Y2");
    entr3[4]=QString("Y3");
    entr3[5]=QString("Y4");
    int entry_values[3]={FFORMAT_NUMBER,FFORMAT_DATE,FFORMAT_STRING};
    int index2=0;
    for (int i=0;i<allocated_selectors;i++)
    {
        sprintf(dummy," %d:",i);
        lblDescription[i]=new QLabel(tr("Column")+QString(dummy),this);
        selFormat[i]=new StdSelector(this,tr("Format:"),3,entr);
        selFormat[i]->setValues(entry_values);
        selTargetSet[i]=new StdSelector(this,tr("Import to set:"),2+import_info.nr_of_target_sets,entr2);
        selTargetSet[i]->setValues(entry_values2);
        selTargetColumn[i]=new StdSelector(this,tr("Import as:"),6,entr3);
        grid1->addWidget(lblDescription[i],index2,0);
        grid1->addWidget(selFormat[i],index2,1);
        grid1->addWidget(selTargetSet[i],index2,2);
        grid1->addWidget(selTargetColumn[i],index2++,3);
        selFormat[i]->setCurrentValue(import_info.col_format[i]);
    }
    if (allocated_selectors>0)
    {
        selTargetSet[0]->setCurrentValue(-2);//All
        selTargetColumn[0]->setCurrentValue(0);//X
        for (int i=1;i<allocated_selectors;i++)
        {
            if (i-1<import_info.nr_of_target_sets)
                selTargetSet[i]->setCurrentValue(import_info.target_sets[i-1]);//the set
            else
                selTargetSet[i]->setCurrentValue(-1);
            selTargetColumn[i]->setCurrentValue(1);//Y
        }
    }
    delete[] entr;
    delete[] entr2;
    delete[] entr3;
    delete[] entry_values2;

}

void frmCSVImporter::doReguess(void)
{
    readFirstLines();
    selSetsToRead->setValue(import_info.columns_to_read-1);
    read_inputs();
    recreate_input_selectors();
}

void frmCSVImporter::doOpen(void)
{
    fileDialog=new QFileDialog(this,tr("Open CSV file"));
    //fileDialog->hide();

    QString filename=fileDialog->getOpenFileName(this,tr("Open CSV file"));
    if (!filename.isEmpty())
    {
        ledFileName->setText(filename);
        readFirstLines();
        selSetsToRead->setValue(import_info.columns_to_read-1);
        read_inputs();
        recreate_input_selectors();
    }
}

void frmCSVImporter::readFirstLines(void)
{
    read_inputs();
    QByteArray line,firstLine;
    QFile file(import_info.filename);
    QString text1;
    int counter=0,ret;
    lblFileContents->clear();
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    while (!file.atEnd() && counter<import_info.headerlines)
    {
        line = file.readLine(1024);
        text1=QString(line.constData());
        text1.chop(1);
        lblFileContents->append(text1);
        counter++;
    }
    lblFileContents->append(QString("-#-#-#-#-#-#-#-#-"));
    counter=0;
    while (!file.atEnd() && counter<6)//read at most 6 lines
    {
        line = file.readLine(1024);
        text1=QString(line.constData());
        text1.chop(1);
        lblFileContents->append(text1);
        if (counter==0) firstLine=line;
        counter++;
    }
    lblFileContents->append(QString("..."));
    /*text1=QString(firstLine.constData());
text1.chop(1);
ind=text1.indexOf(import_info.datsep);
if (ind==-1) ind=text1.length();
text1=text1.left(ind);
ind=text1.count(import_info.colsep)+1;
sprintf(dummy,"--> %d data columns found per set",ind);
lblFileContents->append(tr("Analyzed line:"));
lblFileContents->append(text1);
lblFileContents->append(QString(dummy));*/
    file.close();

    int ncols,*colformats=NULL;

    ret=guess_column_format(import_info,&ncols,&colformats);
    //if (colformats==NULL) cout << "colformat==NULL n=" << ncols << endl;
    if (ret==RETURN_FAILURE || colformats==NULL)
    {
        lblFileContents->append(QString("Unable to guess column formats..."));
    }
    else
    {
        import_info.columns_to_read=ncols;
        if (import_info.col_format!=NULL)
            delete[] import_info.col_format;
        import_info.col_format=new int[import_info.columns_to_read];
        memcpy(import_info.col_format,colformats,sizeof(int)*ncols);

        sprintf(dummy,"Guessed number of columns=%d\nGuessed column formats:",ncols);
        lblFileContents->append(QString(dummy));
        for (int i=0;i<ncols;i++)
        {
            if (colformats[i]==FFORMAT_STRING)
            {
                sprintf(dummy,"Column %d = %s",i,"String");
            }
            else if (colformats[i]==FFORMAT_DATE)
            {
                sprintf(dummy,"Column %d = %s",i,"Date/Time");
            }
            else
            {
                sprintf(dummy,"Column %d = %s",i,"Number");
            }
            lblFileContents->append(QString(dummy));
        }
    }
    if (colformats!=NULL)
        delete[] colformats;
}

void frmCSVImporter::init(void)
{
    graphselector->update_number_of_entries();
    graphselector->set_graph_number(get_cg(),false);
}

void frmCSVImporter::headerLinesChanged(int c)
{
    readFirstLines();
    selSetsToRead->setValue(import_info.columns_to_read-1);
    read_inputs();
    recreate_input_selectors();
}

void frmCSVImporter::setCountChanged(int c)
{
    read_inputs();
    recreate_input_selectors();
}

void frmCSVImporter::doApply(void)
{
    ApplyError=false;
    read_inputs();//read all settings
    read_import_settings();//read the import-settings for every column
    if (import_info.nr_of_target_sets<=0)
    {
        errmsg(tr("No suitable target sets for import selected!").toAscii().constData());
        ApplyError=true;
    }
    if (ApplyError==true) return;
    if (csvImport(import_info)==RETURN_SUCCESS)//do the actual import
    {
        if (chkAutoscaleOnRead->isChecked()) mainWin->doAutoScale();//if import worked --> rescale
        mainWin->mainArea->completeRedraw();//redraw
    }
    else
    {
        ApplyError=true;//tell Acceptr, than an error occured
    }
}

void frmCSVImporter::doAccept(void)
{
    ApplyError=false;
    doApply();
    if (ApplyError==false)
        doClose();
}

void frmCSVImporter::doClose(void)
{
    hide();
}


