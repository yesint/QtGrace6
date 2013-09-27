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

#include "MainWindow.h"
#include <QtGui>
#include <QTranslator>
#include "globals.h"
#include "patterns.h"
#include "allWidgets.h"
#include "draw.h"
#include "xprotos.h"
#include "undo_module.h"
#include "device.h"


extern int replacement_main(int argc,char **argv);

char BI_VERSION[128];
char BI_SYSTEM[256];
char SystemsDecimalPoint='.';

DrawProps draw_props = {{1, 1}, 0, TRUE, 1, 0.0, LINECAP_BUTT, LINEJOIN_MITER, 1.0, 0, FILLRULE_WINDING};
int maxgraph;
bool useQtFonts=false;
bool hdeviceFlag;
int hardCopyDeviceNr;
QList<QFont> stdFontList;
QFont stdFont;
QFontMetrics stdFontMetrics(stdFont);
bool useQPrinter=false;
bool printing_in_file=false;
double GeneralPageZoomFactor=1.0;
long orig_page_w,orig_page_h;
QPrinter * stdPrinter;
int stdOutputFormat=1;
bool startupphase=true;
int rtiCheckTime=1000;
char dummy[4096];
QTextCodec * FileCodec;
QList<QByteArray> avcod;//all available file encodings

frmEllipseProps * EllipseProps;
frmEllipseProps * EditEllipseProps;
frmEllipseProps * BoxProps;
frmEllipseProps * EditBoxProps;
frmTextProps * TextProps;
frmTextProps * EditTextProps;
frmLineProps * LineProps;
frmLineProps * EditLineProps;
frmDrawObjects * FormDrawObjects;
frmConsole * FormConsole;
frmEvalExpr * FormEvalExpr;
frmSetAppearance * FormSetAppearance;
frmGraphApp * FormGraphAppearance;
frmPlotAppearance * FormPlotAppearance;
frmLocatorProps * FormLocatorProps;
frmAxisProp * FormAxisProperties;
frmPointExplorer * FormPointExplorer;
frmNonlinCurveFit * FormNonlinCurveFit;
//frmEditColumnProp * EditColumn;
frmInterpolation * FormInterpolation;
frmSetOp * FormSetOperations;
frmExplorer * FormExplorer;
frmColorManagement * FormColManage;
frmRealTimeInputManager * FormRTIManage;
frmCommands * FormCommands;
frmDeviceSetup * FormDeviceSetup;
frmPreferences * FormPreferences;
frmArrangeGraphs * FormArrangeGraphs;
frmOverlayGraphs * FormOverlayGraphs;
frmAutoscaleGraphs * FormAutoScaleGraphs;
frmDataSetProperties * FormDataSetProperties;
frmSetOperations * FormSetOPS;
frmFeatureExtract * FormFeatureExtraction;
frmInterpolation * FormHistograms;
frmFourier * FormFourier;
frmFourier2 * FormFourier2;
frmLoadEval * FormLoadAndEvaluate;
frmSmallCalc * FormIntegration;
frmSmallCalc * FormSeasonDiff;
frmSmallCalc * FormDifferences;
frmSmallCalc * FormAverages;
frmSmallCalc * FormRegression;
frmCorrelation * FormCorrelation;
frmCorrelation * FormConvolution;
frmCorrelation * FormDigitalFilter;
frmTransform * FormPruneData;
frmTransform * FormSamplePoints;
frmTransform * FormGeometricTransform;
frmHotLinks * FormHotLinks;
frmRegionStatus * FormRegionStatus;
frmRegions * FormReportRegion;
frmRegions * FormClearRegion;
frmRegions * FormDefineRegion;

frmNetCDF * FormNetCDF;
frmIOForm * FormReadSets;
frmIOForm * FormReadNetCDF;
frmIOForm * FormWriteSets;
frmIOForm * FormOpenProject;
frmIOForm * FormSaveProject;
frmIOForm * FormSelectHotLink;
frmIOForm * FormReadParameters;
frmIOForm * FormWriteParameters;
frmIOForm * FormSelectOutputFile;
frmIOForm * FormReadBinary;
frmIOForm * FormWriteBinary;
frmCSVImporter * FormCSVImport;

frmBinaryFormatInput * FormBinaryImportFilter;

frmEditBlockData * FormEditBlockData;

frmFontTool * FormFontTool;
frmAbout * FormAbout;
frmSetEditor * FormSetEditor;
frmUndoList * FormUndoList;

frmExtraPreferences * ExtraPreferences;
QSettings * allPrefs;
bool display_help_externally=false;
bool showhideworkaround=false;

linetype * default_line=NULL;
boxtype * default_box=NULL;
ellipsetype * default_ellipse=NULL;
plotstr * default_string=NULL;
void save_default_states(void);
void reset_default_states(void);

int number_of_opened_spreadsheets=0;
frmSpreadSheet ** FormSpreadSheets=new frmSpreadSheet*[2];
//frmSpreadSheet2 * FormTestSpreadSheet=NULL;

//Undo-Stuff
bool undo_active=true;
int NodeNr,used_Nodes,akt_Node;
int max_node_nr;
class undo_node * Node;


extern int maxstr;
extern double * old_ddata[16];//to store some data temporarely
extern int * old_idata[16];//to store some data temporarely
extern int old_data[16];
extern bool activateLaTeXsupport;
extern bool immediateUpdate;
extern bool updateRunning;
extern int default_Print_Device;
extern bool exchange_point_comma;
extern int new_set_no;
extern int * new_set_nos;
extern QStringList ListOfChanges;
extern QStringList ListOfOldStates;
extern Device_entry *device_table;
extern char print_cmd[GR_MAXPATHLEN];

//OptionItem *graph_select_items = NULL;
int ngraph_select_items = 0;
uniList ** graph_selectors=NULL;
int ngraph_selectors = 0,n_gr_sel_alloc=0;
//OptionItem *color_option_items = NULL;
int ncolor_option_items = 0;
ColorSelector **color_selectors = NULL;
int ncolor_selectors = 0,n_col_sel_alloc=0;
uniList ** set_selectors=NULL;
int nset_selectors = 0,n_set_sel_alloc=0;
uniList ** plist = NULL;
int nplist = 0,n_plist_alloc=0;
FontSelector ** font_selectors=NULL;
int nfont_selectors=0,n_font_sel_alloc=0;

struct ColumnFormat binaryImportFormat[NUMBER_OF_COLUMN_FORMATS];
char binaryImportFormatName[NUMBER_OF_COLUMN_FORMATS][32];
int ImportDestination[NUMBER_OF_IMPORT_DESTINATIONS];
char ImportDestinationName[NUMBER_OF_IMPORT_DESTINATIONS][32];
signed char ImportDestinationType[NUMBER_OF_IMPORT_DESTINATIONS];

MainWindow * mainWin=NULL;
QImage * MainPixmap;
QPainter * GeneralPainter;
QPixmap * Qt_justifications[12];
QPixmap * Qt_matrixOrder[8];
QBitmap * patterns[MAXPATTERNS];//Fill_Paterns for drawing
QPixmap * PatternPixmaps[MAXPATTERNS];
QBitmap * autoBitMap;
QBitmap * downBitMap;
QBitmap * expandBitMap;
QBitmap * leftBitMap;
QBitmap * rightBitMap;
QBitmap * shrinkBitMap;
QBitmap * upBitMap;
QBitmap * zoomBitMap;
QPixmap * GraceIcon;
QPixmap * ActiveIcon;
QPixmap * HiddenIcon;
QPixmap * Qt_j_lm_o_bits;
QPixmap * Qt_j_cm_o_bits;
QPixmap * Qt_j_rm_o_bits;
QPixmap * Qt_j_lb_b_bits;
QPixmap * Qt_j_lm_b_bits;
QPixmap * Qt_j_lt_b_bits;
QPixmap * Qt_j_ct_b_bits;
QPixmap * Qt_j_rt_b_bits;
QPixmap * Qt_j_rm_b_bits;
QPixmap * Qt_j_rb_b_bits;
QPixmap * Qt_j_cb_b_bits;
QPixmap * Qt_j_cm_b_bits;
QPixmap * Qt_m_hv_lr_bt_bits;
QPixmap * Qt_m_hv_lr_tb_bits;
QPixmap * Qt_m_hv_rl_bt_bits;
QPixmap * Qt_m_hv_rl_tb_bits;
QPixmap * Qt_m_vh_lr_bt_bits;
QPixmap * Qt_m_vh_lr_tb_bits;
QPixmap * Qt_m_vh_rl_bt_bits;
QPixmap * Qt_m_vh_rl_tb_bits;
int allocated_colors;
QIcon ** ColorIcons;
QPixmap ** ColorPixmaps;
QString ** ColorNames;
QIcon * LineIcons[MAXLINESTYLES];
QPixmap * LinePixmaps[MAXLINESTYLES];
QVector<qreal> * PenDashPattern[MAXLINESTYLES];
CMap_entry *cmap_table;

QCursor * wait_cursor;
QCursor * line_cursor;
QCursor * find_cursor;
QCursor * move_cursor;
QCursor * text_cursor;
QCursor * kill_cursor;
QCursor * what_cursor;
int cur_cursor = -1;

int action_flag = 0;
int region_def_under_way;

QColor * stdColors[16];//Colors for Drawing
QBrush * stdBrush;//standard Brush for Drawing
QPen * stdPen;//standard Pen for Drawing

char user_home_dir[1024];
char qt_grace_exe_dir[1024];
//extern QTime lastc;
extern void GenerateLaTeXCommands(void);
extern void recursive_replacer(char * text);
extern void find_first_command(char * text,int & start_command,int & command_length,int * brace_pos);
extern void splitter(char * text,int start_command,int command_length,int * brace_pos,char * left_part,char * middle_part,char * argument_part,char * right_part);
extern void complete_LaTeX_to_Grace_Translator(QString & text);
extern void update_font_selectors(void);

extern int gracebat;
int max_history=10;//maximum number of recently opend files to be stored
int current_history=0;//content of history
QString history[MAX_HISTORY];//actual history

//index=0 is reserved for english, which is the default
QTranslator translator[nr_of_translations];
char translator_files[nr_of_translations][64];
char translator_languages[nr_of_translations][64];
int current_language=0;

void copy_Grace_to_LaTeX(void);
void update_default_props(void);
void init_color_icons(void);
void read_settings(void);

void init(void)
{
    stdFontList.clear();
    QFont stfont;
    stfont.fromString(QString("Helvetica,10,-1,5,50,0,0,0,0,0"));
    stdFontList << stfont;
    stfont.fromString(QString("Times,10,-1,5,50,1,0,0,0,0"));
    stdFontList << stfont;
    stfont.fromString(QString("Times,10,-1,5,75,0,0,0,0,0"));
    stdFontList << stfont;
    stfont.fromString(QString("Times,10,-1,5,75,1,0,0,0,0"));
    stdFontList << stfont;
    stfont.fromString(QString("Times,10,-1,5,50,0,0,0,0,0"));
    stdFontList << stfont;
    stfont.fromString(QString("Helvetica,10,-1,5,50,1,0,0,0,0"));
    stdFontList << stfont;
    stfont.fromString(QString("Helvetica,10,-1,5,75,0,0,0,0,0"));
    stdFontList << stfont;
    stfont.fromString(QString("Helvetica,10,-1,5,75,1,0,0,0,0"));
    stdFontList << stfont;
    stfont.fromString(QString("Courier,10,-1,5,50,0,0,0,0,0"));
    stdFontList << stfont;
    stfont.fromString(QString("Courier,10,-1,5,50,1,0,0,0,0"));
    stdFontList << stfont;
    stfont.fromString(QString("Courier,10,-1,5,75,0,0,0,0,0"));
    stdFontList << stfont;
    stfont.fromString(QString("Courier,10,-1,5,75,1,0,0,0,0"));
    stdFontList << stfont;
    stfont.fromString(QString("Symbol,10,-1,5,50,0,0,0,0,0"));
    stdFontList << stfont;
    stfont.fromString(QString("Zapf Dingbats,10,-1,5,50,0,0,0,0,0"));
    stdFontList << stfont;

    for (int i=0;i<MAX_HISTORY;i++)
        history[i]=QString("");
    GenerateLaTeXCommands();
    region_def_under_way=-1;
    Qt_justifications[0]=Qt_j_lm_o_bits;
    Qt_justifications[1]=Qt_j_cm_o_bits;
    Qt_justifications[2]=Qt_j_rm_o_bits;
    Qt_justifications[3]=Qt_j_lb_b_bits;
    Qt_justifications[4]=Qt_j_cb_b_bits;
    Qt_justifications[5]=Qt_j_rb_b_bits;
    Qt_justifications[6]=Qt_j_lm_b_bits;
    Qt_justifications[7]=Qt_j_cm_b_bits;
    Qt_justifications[8]=Qt_j_rm_b_bits;
    Qt_justifications[9]=Qt_j_lt_b_bits;
    Qt_justifications[10]=Qt_j_ct_b_bits;
    Qt_justifications[11]=Qt_j_rt_b_bits;

    Qt_matrixOrder[0]=Qt_m_hv_lr_tb_bits;
    Qt_matrixOrder[1]=Qt_m_hv_lr_bt_bits;
    Qt_matrixOrder[2]=Qt_m_hv_rl_bt_bits;
    Qt_matrixOrder[3]=Qt_m_hv_rl_tb_bits;
    Qt_matrixOrder[4]=Qt_m_vh_lr_tb_bits;
    Qt_matrixOrder[5]=Qt_m_vh_lr_bt_bits;
    Qt_matrixOrder[6]=Qt_m_vh_rl_tb_bits;
    Qt_matrixOrder[7]=Qt_m_vh_rl_bt_bits;

#ifdef LINUX_SYSTEM
    showhideworkaround=true;
#else
    showhideworkaround=false;
#endif

    binaryImportFormat[0].format=COLUMN_OFFSET;
    binaryImportFormat[0].size=1;
    strcpy(binaryImportFormatName[0],"Offset");
    binaryImportFormat[1].format=COLUMN_SIG_CHAR;
    binaryImportFormat[1].size=sizeof(signed char);
    strcpy(binaryImportFormatName[1],"signed Char");
    binaryImportFormat[2].format=COLUMN_USIG_CHAR;
    binaryImportFormat[2].size=sizeof(unsigned char);
    strcpy(binaryImportFormatName[2],"unsigned Char");
    binaryImportFormat[3].format=COLUMN_SIG_SHORT;
    binaryImportFormat[3].size=sizeof(signed short);
    strcpy(binaryImportFormatName[3],"signed Short");
    binaryImportFormat[4].format=COLUMN_USIG_SHORT;
    binaryImportFormat[4].size=sizeof(unsigned short);
    strcpy(binaryImportFormatName[4],"unsigned Short");
    binaryImportFormat[5].format=COLUMN_SIG_INT;
    binaryImportFormat[5].size=sizeof(signed int);
    strcpy(binaryImportFormatName[5],"signed Int");
    binaryImportFormat[6].format=COLUMN_USIG_INT;
    binaryImportFormat[6].size=sizeof(unsigned int);
    strcpy(binaryImportFormatName[6],"unsigned Int");
    binaryImportFormat[7].format=COLUMN_SIG_LONG;
    binaryImportFormat[7].size=sizeof(signed long);
    strcpy(binaryImportFormatName[7],"signed Long");
    binaryImportFormat[8].format=COLUMN_USIG_LONG;
    binaryImportFormat[8].size=sizeof(unsigned long);
    strcpy(binaryImportFormatName[8],"unsigned Long");
    binaryImportFormat[9].format=COLUMN_FLOAT;
    binaryImportFormat[9].size=sizeof(float);
    strcpy(binaryImportFormatName[9],"Float");
    binaryImportFormat[10].format=COLUMN_DOUBLE;
    binaryImportFormat[10].size=sizeof(double);
    strcpy(binaryImportFormatName[10],"Double");
    binaryImportFormat[11].format=COLUMN_LONG_DOUBLE;
    binaryImportFormat[11].size=sizeof(long double);
    strcpy(binaryImportFormatName[11],"long Double");
    binaryImportFormat[12].format=COLUMN_STRING;
    binaryImportFormat[12].size=0;
    strcpy(binaryImportFormatName[12],"String");

    ImportDestination[0]=IMPORT_TO_NONE;
    strcpy(ImportDestinationName[0],"None");
    ImportDestinationType[0]=3;//every one
    ImportDestination[1]=IMPORT_TO_TITLE;
    strcpy(ImportDestinationName[1],"Title");
    ImportDestinationType[1]=1;//only for header
    ImportDestination[2]=IMPORT_TO_X;
    strcpy(ImportDestinationName[2],"X");
    ImportDestinationType[2]=2;//only for data
    ImportDestination[3]=IMPORT_TO_Y;
    strcpy(ImportDestinationName[3],"Y");
    ImportDestinationType[3]=2;//only for data
    ImportDestination[4]=IMPORT_TO_Y1;
    strcpy(ImportDestinationName[4],"Y1");
    ImportDestinationType[4]=2;//only for data
    ImportDestination[5]=IMPORT_TO_Y2;
    strcpy(ImportDestinationName[5],"Y2");
    ImportDestinationType[5]=2;//only for data
    ImportDestination[6]=IMPORT_TO_Y3;
    strcpy(ImportDestinationName[6],"Y3");
    ImportDestinationType[6]=2;//only for data
    ImportDestination[7]=IMPORT_TO_Y4;
    strcpy(ImportDestinationName[7],"Y4");
    ImportDestinationType[7]=2;//only for data
    ImportDestination[8]=IMPORT_TO_X0;
    strcpy(ImportDestinationName[8],"X0");
    ImportDestinationType[8]=1;//only for header
    ImportDestination[9]=IMPORT_TO_DELTAX;
    strcpy(ImportDestinationName[9],"delta X");
    ImportDestinationType[9]=1;//only for header
    ImportDestination[10]=IMPORT_TO_XTITLE;
    strcpy(ImportDestinationName[10],"X-Title");
    ImportDestinationType[10]=1;//only for header
    ImportDestination[11]=IMPORT_TO_YTITLE;
    strcpy(ImportDestinationName[11],"Y-Title");
    ImportDestinationType[11]=1;//only for header
    ImportDestination[12]=IMPORT_TO_XFACTOR;
    strcpy(ImportDestinationName[12],"X-Factor");
    ImportDestinationType[12]=1;//only for header
    ImportDestination[13]=IMPORT_TO_YFACTOR;
    strcpy(ImportDestinationName[13],"Y-Factor");
    ImportDestinationType[13]=1;//only for header
    ImportDestination[14]=IMPORT_TO_Y1FACTOR;
    strcpy(ImportDestinationName[14],"Y1-Factor");
    ImportDestinationType[14]=1;//only for header
    ImportDestination[15]=IMPORT_TO_Y2FACTOR;
    strcpy(ImportDestinationName[15],"Y2-Factor");
    ImportDestinationType[15]=1;//only for header
    ImportDestination[16]=IMPORT_TO_Y3FACTOR;
    strcpy(ImportDestinationName[16],"Y3-Factor");
    ImportDestinationType[16]=1;//only for header
    ImportDestination[17]=IMPORT_TO_Y4FACTOR;
    strcpy(ImportDestinationName[17],"Y4-Factor");
    ImportDestinationType[17]=1;//only for header
    ImportDestination[18]=IMPORT_TO_SUBTITLE;
    strcpy(ImportDestinationName[18],"SubTitle");
    ImportDestinationType[18]=1;//only for header
    ImportDestination[19]=IMPORT_TO_NUMBER_OF_DATA;
    strcpy(ImportDestinationName[19],"Sample Count");
    ImportDestinationType[19]=1;//only for header
    ImportDestination[20]=IMPORT_TO_NUMBER_OF_CHANNELS;
    strcpy(ImportDestinationName[20],"Channel Count");
    ImportDestinationType[20]=1;//only for header
    ImportDestination[21]=IMPORT_TO_DATA_SIZE;
    strcpy(ImportDestinationName[21],"Channel Size (Bytes)");
    ImportDestinationType[21]=1;//only for header
    ImportDestination[22]=IMPORT_TO_DATA_SIZE_BIT;
    strcpy(ImportDestinationName[22],"Channel Size (Bits)");
    ImportDestinationType[22]=1;//only for header
    ImportDestination[23]=IMPORT_TO_DATA_SAMPLING_RATE;
    strcpy(ImportDestinationName[23],"Sampling Rate");
    ImportDestinationType[23]=1;//only for header
    ImportDestination[24]=IMPORT_TO_WHOLE_DATA_BLOCK_SIZE;
    strcpy(ImportDestinationName[24],"Whole data block (Bytes)");
    ImportDestinationType[24]=1;//only for header
    ImportDestination[25]=IMPORT_TO_SINGLE_DATA_BLOCK_SIZE;
    strcpy(ImportDestinationName[25],"Single data point (Bytes)");
    ImportDestinationType[25]=1;//only for header
    ImportDestination[26]=IMPORT_TO_SET_LEGEND;
    strcpy(ImportDestinationName[26],"Set Legend");
    ImportDestinationType[26]=1;//only for header
    ImportDestination[27]=IMPORT_TO_TRIGGER;
    strcpy(ImportDestinationName[27],"Trigger Channel");
    ImportDestinationType[27]=2;//only for data
    ImportDestination[28]=IMPORT_TO_TRIGGER_FACTOR;
    strcpy(ImportDestinationName[28],"Factor for Trigger Channel");
    ImportDestinationType[28]=1;//only for header
}

int main( int argc, char **argv )
{
    startupphase=true;
    /*#ifdef WINDOWS_SYSTEM
    cout << "Windows" << endl;
#endif
#ifdef MAC_SYSTEM
    cout << "Mac OS X" << endl;
#endif
#ifdef LINUX_SYSTEM
    cout << "Linux" << endl;
#endif*/

    QApplication * a=new QApplication( argc, argv );



    //At first: Initialize everything with NULL-pointers
    useQPrinter=false;
    stdPrinter=NULL;
    FormAbout=NULL;
    FormEvalExpr=NULL;
    FormSetAppearance=NULL;
    FormGraphAppearance=NULL;
    FormPlotAppearance=NULL;
    FormAxisProperties=NULL;
    FormLocatorProps=NULL;
    FormDrawObjects=NULL;
    FormPointExplorer=NULL;
    FormNonlinCurveFit=NULL;
    FormInterpolation=NULL;
    FormSetOperations=NULL;
    FormCommands=NULL;
    FormDeviceSetup=NULL;
    FormArrangeGraphs=NULL;
    FormOverlayGraphs=NULL;
    FormAutoScaleGraphs=NULL;
    FormLoadAndEvaluate=NULL;
    FormDataSetProperties=NULL;
    FormSetOPS=NULL;
    FormFeatureExtraction=NULL;
    FormInterpolation=NULL;
    FormHistograms=NULL;
    FormFourier=NULL;
    FormFourier2=NULL;
    FormIntegration=NULL;
    FormSeasonDiff=NULL;
    FormDifferences=NULL;
    FormAverages=NULL;
    FormRegression=NULL;
    FormCorrelation=NULL;
    FormConvolution=NULL;
    FormDigitalFilter=NULL;
    FormPruneData=NULL;
    FormSamplePoints=NULL;
    FormGeometricTransform=NULL;
    FormSelectHotLink=NULL;
    FormReadParameters=NULL;
    FormWriteParameters=NULL;
    FormHotLinks=NULL;
    FormRegionStatus=NULL;
    FormReportRegion=NULL;
    FormClearRegion=NULL;
    FormDefineRegion=NULL;
    FormSelectOutputFile=NULL;
    FormReadSets=NULL;
    FormWriteSets=NULL;
    FormNetCDF=NULL;
    FormReadNetCDF=NULL;
    FormOpenProject=NULL;
    FormSaveProject=NULL;
    FormBinaryImportFilter=NULL;
    FormSetEditor=NULL;
    FormEditBlockData=NULL;
    FormUndoList=NULL;
    FormExplorer=NULL;
    FormColManage=NULL;
    FormRTIManage=NULL;
    EllipseProps=NULL;
    BoxProps=NULL;
    LineProps=NULL;
    TextProps=NULL;
    FormFontTool=NULL;
    FormCSVImport=NULL;

    FileCodec=QTextCodec::codecForName("System");
    avcod=QTextCodec::availableCodecs();

#if QT_VERSION < 0x050000
    QLocale * sysloc=new QLocale();
    QChar dpc=sysloc->decimalPoint();
    SystemsDecimalPoint=dpc.toAscii();
    delete[] sysloc;
#else
    QChar dpc='.';
    SystemsDecimalPoint='.';
#endif

    allocated_colors=0;
    ColorIcons=NULL;
    ColorPixmaps=NULL;
    ColorNames=NULL;

    Node=NULL;
    for (int i=0;i<16;i++)
    {
        old_ddata[i]=NULL;
        old_idata[i]=NULL;
        old_data[i]=0;
    }
    strcpy(user_home_dir,QDir::homePath().toAscii());
    strcpy(qt_grace_exe_dir,qApp->applicationDirPath().toAscii());
    
    //define the available languages
    strcpy(translator_files[0],"-");
    strcpy(translator_files[1],"qt_grace_ger");
    strcpy(translator_languages[0],"English");
    strcpy(translator_languages[1],"German");
    for (int i=1;i<nr_of_translations;i++)
    {
        translator[i].load(translator_files[i]);
        //a.installTranslator(&translator[i]);//-->install later
    }

    allPrefs=new QSettings(qt_grace_exe_dir+QString("/qtGrace_Settings.ini"),QSettings::IniFormat);
    allPrefs->sync();
    allPrefs->beginGroup(QString("ExtraPreferences"));
    int sellang=allPrefs->value(QString("language"),QVariant(0)).toInt();
    allPrefs->endGroup();
    delete allPrefs;
    if (sellang>0)
    {
        a->installTranslator(&translator[sellang]);
        current_language=sellang;
    }

    init_cursors();
    initNodes();//Undo-Stuff

    MainPixmap=new QImage(10,10,QImage::Format_ARGB32_Premultiplied);
    MainPixmap->fill(0);
    GeneralPainter=new QPainter(MainPixmap);

    stdFont=a->font();
    stdFont.setPixelSize(14);
    stdFontMetrics=QFontMetrics(stdFont);

    mainWin=new MainWindow();
    a->setWindowIcon(*GraceIcon);

    replacement_main(argc,argv);//set up all internal Grace-things

    //Here setup socket connection.
    if(connectToViewBeast){
        mainWin->SocketConnection = new LocalSocketIpcServer(sendToBeast,readFromBeast,mainWin);
    }else{
        fprintf(stderr, "Not able to start View Beast connection\n");
    }

    if (gracebat==TRUE)//no GUI wanted
    {
        delete mainWin;
        delete GeneralPainter;
        delete MainPixmap;
        return 0;
    }

    //2013-09-12 Nimalendiran Kailasanathan changed default window -and canvas size
    mainWin->setGeometry( 100, 100,1060,800 );
    set_page_dimensions(733,538,1);
    //      mainWin->setGeometry( 100, 100, 872, 670 );
    init();
    FormConsole=new frmConsole(mainWin);//needed for error-Messages
    FormPreferences=new frmPreferences(mainWin);
    FormPreferences->init();
    FormPreferences->hide();
    ExtraPreferences=new frmExtraPreferences(FormPreferences);
    ExtraPreferences->init();
    ExtraPreferences->hide();

    EditTextProps=new frmTextProps(mainWin,true);
    EditTextProps->hide();
    EditLineProps=new frmLineProps(mainWin,true);
    EditLineProps->hide();
    EditBoxProps=new frmEllipseProps(mainWin,true,false);
    EditBoxProps->hide();
    EditEllipseProps=new frmEllipseProps(mainWin,true,true);
    EditEllipseProps->hide();
    read_settings();

    if (get_pagelayout() == PAGE_FREE)
    {
        mainWin->sldPageZoom->setValue(0);
        mainWin->sldPageZoom->setEnabled(false);
        mainWin->cmdFitPage->setEnabled(false);
    }

    //cout << "useQtFonts=" << useQtFonts << endl;

    /*orig_page_w=device_table[0].pg.width;//save original size
orig_page_h=device_table[0].pg.height;
device_table[0].pg.width=orig_page_w*GeneralPageZoomFactor;//use Page Zoom
device_table[0].pg.height=orig_page_h*GeneralPageZoomFactor;*/

    mainWin->mainArea->completeRedraw();

    if(!hideMainWindow)
        mainWin->show();

    clear_dirtystate();
    CheckActive();

    mainWin->lstGraphs->update_number_of_entries();
    int sel[2]={0,1};
    mainWin->lstGraphs->set_new_selection(1,sel);
    mainWin->checkForRealTimeIO();
    update_font_selectors();
    mainWin->activateWindow();
    startupphase=false;

    int execVal=a->exec();

    if(mainWin&&mainWin->SocketConnection){
        delete  mainWin->SocketConnection;
    }

    return execVal;
}

extern int object_edit_popup(int obj, int id);
int object_edit_popup(int type, int id)
{
    switch (type) {
    case OBJECT_BOX:
        if (EditBoxProps==NULL)
        {
            EditBoxProps=new frmEllipseProps(mainWin,true,false);
        }
        EditBoxProps->init(id);
        EditBoxProps->show();
        EditBoxProps->raise();
        break;
    case OBJECT_ELLIPSE:
        if (EditEllipseProps==NULL)
        {
            EditEllipseProps=new frmEllipseProps(mainWin,true,false);
        }
        EditEllipseProps->init(id);
        EditEllipseProps->show();
        EditEllipseProps->raise();
        break;
    case OBJECT_LINE:
        if (EditLineProps==NULL)
        {
            EditLineProps=new frmLineProps(mainWin,true);
        }
        EditLineProps->init(id);
        EditLineProps->show();
        EditLineProps->raise();
        break;
    case OBJECT_STRING:
        if (EditTextProps==NULL)
        {
            EditTextProps=new frmTextProps(mainWin,true);
        }
        EditTextProps->init(id);
        EditTextProps->show();
        EditTextProps->raise();
        EditTextProps->activateWindow();
        EditTextProps->flp->string_item->lenText->setFocus();
        break;
    default:
        return RETURN_FAILURE;
        break;
    }
    return RETURN_SUCCESS;
}

void errwin(const char *s)
{
    if (FormConsole==NULL)
    {
        FormConsole=new frmConsole(mainWin);
    }
    FormConsole->errwin(s);
}

void add_GraphSelector(uniList * grSel)
{
    if (ngraph_selectors==n_gr_sel_alloc)
    {
        uniList ** dummy=graph_selectors;
        n_gr_sel_alloc+=5;
        graph_selectors=new uniList*[n_gr_sel_alloc];
        for (int i=0;i<ngraph_selectors;i++)
            graph_selectors[i]=dummy[i];
        delete[] dummy;//delete original entries
    }
    graph_selectors[ngraph_selectors]=grSel;
    ngraph_selectors++;
}

void set_graph_selectors(int gno)
{
    int i;

    for (i = 0; i < ngraph_selectors; i++)
    {
        SelectListChoice(graph_selectors[i], gno);
    }
    for (i = 0; i < nset_selectors; i++)
    {
        set_selectors[i]->set_graph_number(gno,true);
        ///WARNING: I DO NOT KNOW IF THIS IS GOOD!
        if (set_selectors[i]->prevent_from_autoupdate==false)
            set_selectors[i]->new_selection();
    }
}

void add_SetSelector(uniList * setSel)
{
    if (nset_selectors==n_set_sel_alloc)
    {
        uniList ** dummy=set_selectors;
        n_set_sel_alloc+=5;
        set_selectors=new uniList*[n_set_sel_alloc];
        for (int i=0;i<nset_selectors;i++)
            set_selectors[i]=dummy[i];
        delete[] dummy;//delete original entries
    }
    set_selectors[nset_selectors]=setSel;
    nset_selectors++;
}

void add_SetChoiceItem(uniList * setSel)
{
    if (nplist==n_plist_alloc)
    {
        uniList ** dummy=plist;
        n_plist_alloc+=5;
        plist=new uniList*[n_plist_alloc];
        for (int i=0;i<nplist;i++)
            plist[i]=dummy[i];
        delete[] dummy;//delete original entries
    }
    plist[nplist]=setSel;
    nplist++;
}

void add_ColorSelector(ColorSelector * colSel)
{
    if (ncolor_selectors==n_col_sel_alloc)
    {
        ColorSelector ** dummy=color_selectors;
        n_col_sel_alloc+=5;
        color_selectors=new ColorSelector*[n_col_sel_alloc];
        for (int i=0;i<ncolor_selectors;i++)
            color_selectors[i]=dummy[i];
        delete[] dummy;//delete original entries
    }
    color_selectors[ncolor_selectors]=colSel;
    ncolor_selectors++;
}

void add_FontSelector(FontSelector * fontSel)
{
    if (nfont_selectors==n_font_sel_alloc)
    {
        FontSelector ** dummy=font_selectors;
        n_font_sel_alloc+=5;
        font_selectors=new FontSelector*[n_font_sel_alloc];
        for (int i=0;i<nfont_selectors;i++)
            font_selectors[i]=dummy[i];
        delete[] dummy;//delete original entries
    }
    font_selectors[nfont_selectors]=fontSel;
    nfont_selectors++;
}

void change_language(int index)
{
    if (index>=nr_of_translations) return;
    if (current_language!=0)
    {
        qApp->removeTranslator(&translator[current_language]);
    }
    qApp->installTranslator(&translator[index]);
    current_language=index;
}

void read_settings(void)
{
    /*allPrefs=new QSettings(QString("Grace"),QString("qtGrace"));
    allPrefs->setPath(QSettings::IniFormat,QSettings::UserScope,qt_grace_exe_dir);*/
    allPrefs=new QSettings(qt_grace_exe_dir+QString("/qtGrace_Settings.ini"),QSettings::IniFormat);
    allPrefs->sync();
    allPrefs->beginGroup(QString("Preferences"));

    FormPreferences->noask_item->setChecked(allPrefs->value(QString("dontaskquestions"),QVariant(false)).toBool());
    FormPreferences->dc_item->setChecked(allPrefs->value(QString("allowdoubleclickoncanvas"),QVariant(true)).toBool());
    FormPreferences->graph_focus_choice_item->setCurrentIndex(allPrefs->value(QString("graphfocusswitch"),QVariant(0)).toInt());
    FormPreferences->graph_drawfocus_choice_item->setChecked(allPrefs->value(QString("displayfocusmarker"),QVariant(true)).toBool());
    FormPreferences->autoredraw_type_item->setChecked(allPrefs->value(QString("autoredraw"),QVariant(true)).toBool());
    FormPreferences->cursor_type_item->setChecked(allPrefs->value(QString("crosshaircursor"),QVariant(false)).toBool());

    FormPreferences->max_path_item->setValue(allPrefs->value(QString("maxdrawpathlength"),QVariant(1000000)).toInt());
    FormPreferences->safe_mode_item->setChecked(allPrefs->value(QString("runinsafemode"),QVariant(true)).toBool());

    FormPreferences->scrollper_item->setValue(allPrefs->value(QString("scrollpercent"),QVariant(5)).toInt());
    FormPreferences->shexper_item->setValue(allPrefs->value(QString("zoompercent"),QVariant(5)).toInt());
    FormPreferences->linkscroll_item->setChecked(allPrefs->value(QString("linkedscrolling"),QVariant(false)).toBool());
    FormPreferences->hint_item->setCurrentIndex(allPrefs->value(QString("datehint"),QVariant(3)).toInt());
    FormPreferences->date_item->setText(allPrefs->value(QString("referencedate"),QVariant("-4713-01-01 12:00:00")).toString());
    FormPreferences->two_digits_years_item->setChecked(allPrefs->value(QString("twodigityearspan"),QVariant(false)).toBool());
    FormPreferences->wrap_year_item->setText(allPrefs->value(QString("wrapyear"),QVariant("1950")).toString());
    allPrefs->endGroup();
    allPrefs->beginGroup(QString("ExtraPreferences"));
    ExtraPreferences->lenHome->setText(allPrefs->value(QString("gracehomedirectory"),QVariant(qt_grace_exe_dir)).toString());
    ExtraPreferences->chkExternalHelpViewer->setChecked(allPrefs->value(QString("showhelpexternally"),QVariant(false)).toBool());
    ExtraPreferences->lenHelpViewer->setText(allPrefs->value(QString("helpviewer"),QVariant("")).toString());
    ExtraPreferences->selLanguage->setCurrentIndex(allPrefs->value(QString("language"),QVariant(0)).toInt());
    ExtraPreferences->chkShowHideException->setChecked(allPrefs->value(QString("showhideworkaround"),QVariant(false)).toBool());
    allPrefs->endGroup();
    allPrefs->beginGroup(QString("History"));
    max_history=allPrefs->value(QString("maxhistory"),QVariant(MAX_HISTORY)).toInt();
    ExtraPreferences->histSize->setValue(max_history);
    current_history=allPrefs->value(QString("historysize"),QVariant(0)).toInt();
    char dummy[32];
    for (int i=0;i<MAX_HISTORY;i++)//always save MAX_HISTORY entries (possibly empty ones)
    {
        sprintf(dummy,"history%d",i);
        history[i]=allPrefs->value(QString(dummy),QVariant("")).toString();
    }
    allPrefs->endGroup();
    allPrefs->beginGroup(QString("General"));
    stdOutputFormat=allPrefs->value(QString("lastOutputFormat"),QVariant(1)).toInt();
    undo_active=allPrefs->value(QString("activateUndoRecords"),QVariant(false)).toBool();///undo deactivated as a default
    activateLaTeXsupport=allPrefs->value(QString("activateLaTeXSupport"),QVariant(true)).toBool();
    ExtraPreferences->chkActivateLaTeXSupport->setChecked(activateLaTeXsupport);
    immediateUpdate=allPrefs->value(QString("ImmediateUpdates"),QVariant(false)).toBool();
    ExtraPreferences->chkImmediateUpdate->setChecked(immediateUpdate);
    default_Print_Device=allPrefs->value(QString("DefaultPrintingDevice"),QVariant(0)).toInt();
    if(hdeviceFlag){
        default_Print_Device=hardCopyDeviceNr;
    }
    ExtraPreferences->selDefaultPrintDevice->setCurrentIndex(default_Print_Device+1);
    //no paint device yet --> we have to disconnect this, because redraw is inpossible now
    mainWin->disconnect(mainWin->sldPageZoom,SIGNAL(valueChanged(int)),mainWin,SLOT(doPageZoom(int)));
    mainWin->sldPageZoom->setValue(allPrefs->value(QString("PageViewZoom"),QVariant(0)).toInt());
    mainWin->connect(mainWin->sldPageZoom,SIGNAL(valueChanged(int)),mainWin,SLOT(doPageZoom(int)));
    GeneralPageZoomFactor=pow(10.0,mainWin->sldPageZoom->value()*mainWin->sldPageZoom->ScalingFactor);
    QString codName=allPrefs->value(QString("FileCodec"),QVariant("System")).toString();
    int index=0;
    for (int i=0;i<avcod.length();i++)
    {
        if (QString(avcod.at(i)).compare(codName)==0)
        {
            index=i;
            break;
        }
    }
    FileCodec=QTextCodec::codecForName(codName.toAscii().constData());
    ExtraPreferences->selCodec->setCurrentIndex(index);

    FormPreferences->doApply();
    ExtraPreferences->doApply();

    mainWin->recreateHistory();
    bool res_undo=undo_active;
    undo_active=false;
    if (FormDeviceSetup==NULL)
    {
        FormDeviceSetup=new frmDeviceSetup(1,mainWin);
        //initialize this only on startup
        if (default_Print_Device==-1)//last one
            FormDeviceSetup->devices_item->setCurrentIndex(stdOutputFormat);
        else
            FormDeviceSetup->devices_item->setCurrentIndex(default_Print_Device);
    }
    FormDeviceSetup->hide();
    //   FormDeviceSetup->print_string_item->setText( allPrefs->value(QString("PrintCommand"),QVariant("lpr")).toString() );
    FormDeviceSetup->doApply();

    allPrefs->endGroup();
    allPrefs->sync();
    delete allPrefs;
    undo_active=res_undo;
}

void write_settings(void)
{
    /*allPrefs=new QSettings(QString("Grace"),QString("qtGrace"));
    allPrefs->setPath(QSettings::IniFormat,QSettings::UserScope,qt_grace_exe_dir);*/
    allPrefs=new QSettings(qt_grace_exe_dir+QString("/qtGrace_Settings.ini"),QSettings::IniFormat);
    allPrefs->beginGroup(QString("Preferences"));
    allPrefs->setValue(QString("dontaskquestions"),QVariant(FormPreferences->noask_item->isChecked()));
    allPrefs->setValue(QString("allowdoubleclickoncanvas"),QVariant(FormPreferences->dc_item->isChecked()));
    allPrefs->setValue(QString("graphfocusswitch"),QVariant(FormPreferences->graph_focus_choice_item->currentIndex()));
    allPrefs->setValue(QString("displayfocusmarker"),QVariant(FormPreferences->graph_drawfocus_choice_item->isChecked()));
    allPrefs->setValue(QString("autoredraw"),QVariant(FormPreferences->autoredraw_type_item->isChecked()));
    allPrefs->setValue(QString("crosshaircursor"),QVariant(FormPreferences->cursor_type_item->isChecked()));

    allPrefs->setValue(QString("maxdrawpathlength"),QVariant(FormPreferences->max_path_item->value()));
    allPrefs->setValue(QString("runinsafemode"),QVariant(FormPreferences->safe_mode_item->isChecked()));

    allPrefs->setValue(QString("scrollpercent"),QVariant(FormPreferences->scrollper_item->value()));
    allPrefs->setValue(QString("zoompercent"),QVariant(FormPreferences->shexper_item->value()));
    allPrefs->setValue(QString("linkedscrolling"),QVariant(FormPreferences->linkscroll_item->isChecked()));

    allPrefs->setValue(QString("datehint"),QVariant(FormPreferences->hint_item->currentIndex()));
    allPrefs->setValue(QString("referencedate"),QVariant(FormPreferences->date_item->text()));
    allPrefs->setValue(QString("twodigityearspan"),QVariant(FormPreferences->two_digits_years_item->isChecked()));
    allPrefs->setValue(QString("wrapyear"),QVariant(FormPreferences->wrap_year_item->text()));
    allPrefs->endGroup();
    allPrefs->beginGroup(QString("ExtraPreferences"));
    allPrefs->setValue(QString("gracehomedirectory"),QVariant(ExtraPreferences->lenHome->text()));
    allPrefs->setValue(QString("showhelpexternally"),QVariant(ExtraPreferences->chkExternalHelpViewer->isChecked()));
    allPrefs->setValue(QString("helpviewer"),QVariant(ExtraPreferences->lenHelpViewer->text()));
    allPrefs->setValue(QString("language"),QVariant(ExtraPreferences->selLanguage->currentIndex()));
    allPrefs->setValue(QString("showhideworkaround"),QVariant(ExtraPreferences->chkShowHideException->isChecked()));
    //allPrefs->setValue(QString(""),QVariant());
    allPrefs->endGroup();
    allPrefs->beginGroup(QString("History"));
    allPrefs->setValue(QString("maxhistory"),QVariant(max_history));
    allPrefs->setValue(QString("historysize"),QVariant(current_history));
    char dummy[32];
    for (int i=0;i<MAX_HISTORY;i++)//always save MAX_HISTORY entries (possibly empty ones)
    {
        sprintf(dummy,"history%d",i);
        if (current_history>i && max_history>i)
            allPrefs->setValue(QString(dummy),QVariant(history[i]));
        else
            allPrefs->setValue(QString(dummy),QVariant(""));
    }
    allPrefs->endGroup();
    allPrefs->beginGroup(QString("General"));
    allPrefs->setValue(QString("lastOutputFormat"),QVariant(stdOutputFormat));
    allPrefs->setValue(QString("activateUndoRecords"),QVariant(undo_active));
    allPrefs->setValue(QString("activateLaTeXSupport"),QVariant(activateLaTeXsupport));
    allPrefs->setValue(QString("ImmediateUpdates"),QVariant(immediateUpdate));
    allPrefs->setValue(QString("DefaultPrintingDevice"),QVariant(default_Print_Device));
    allPrefs->setValue(QString("PageViewZoom"),QVariant(mainWin->sldPageZoom->value()));
    allPrefs->setValue(QString("FileCodec"),QVariant(FileCodec->name()));
    allPrefs->setValue(QString("PrintCommand"),QVariant(print_cmd));
    allPrefs->endGroup();
    allPrefs->sync();
    delete allPrefs;
}

void GeneralPaste(const QMimeData * mimeData)
{
    QTemporaryFile * temp_file=new QTemporaryFile;
    QString str,str2;
    int commas,fullstops,ret;
    char * filename=NULL;
    int files=0;
    char ** filenames=NULL;
    char dummy[1024];
    ofstream ofi;
    int * gnos,*snos,*sdos,len;
    int deltasize=32,m1,m2;
    gnos=new int[deltasize];
    snos=new int[deltasize];
    //cout << "General Paste Event" << endl;
    /*
QStringList sl=mimeData->formats();
for (int i=0;i<sl.length();i++)
cout << sl.at(i).toAscii().constData() << endl;
cout << endl;
*/
    set_wait_cursor();
    len=0;
    ListOfChanges.clear();
    ListOfOldStates.clear();
    if (mimeData->hasText() && !mimeData->hasUrls())//text implies ascii-data
    {
        if (temp_file->open())
        {
            str=mimeData->text();
            commas=str.count(",");
            fullstops=str.count(".");
            if (commas>fullstops*3)
                if (QMessageBox::question(mainWin,QObject::tr("Problem during data import"),QObject::tr("Significantly more commas than fullstops found.\nUse comma as decimal separator?"),QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
                    exchange_point_comma=true;
            filename=new char[temp_file->fileName().length()+2];
            strcpy(filename,temp_file->fileName().toAscii().constData());
            ofi.open(filename);
            ofi << mimeData->text().toAscii().constData() << endl;
            ofi.close();
            autoscale_onread=AUTOSCALE_XY;
            curtype=0;//XY
            new_set_no=0;
            if (new_set_nos!=NULL)
            {
                delete[] new_set_nos;
                new_set_nos=NULL;//important!!!
            }
            getdata(get_cg(), filename, 0, LOAD_NXY);
            if (new_set_no>0 && new_set_nos!=NULL)
            {
                m1=(len)/deltasize;
                m2=(len+new_set_no)/deltasize;
                if (m2>m1)//more memory needed
                {
                    sdos=gnos;
                    gnos=new int[(m2+1)*deltasize];
                    memcpy(gnos,sdos,sizeof(int)*(m1+1)*deltasize);
                    delete[] sdos;
                    sdos=snos;
                    snos=new int[(m2+1)*deltasize];
                    memcpy(snos,sdos,sizeof(int)*(m1+1)*deltasize);
                    delete[] sdos;
                }
                for (int k=0;k<new_set_no;k++)
                {
                    gnos[len+k]=get_cg();
                    snos[len+k]=new_set_nos[k];
                    QString str_imp=QObject::tr("Imported from Clipboard");
                    setcomment(gnos[len+k],snos[len+k],str_imp.toAscii().data());
                }
                len+=new_set_no;
            }
            delete[] filename;
            filename=NULL;
        }//end file->open
        set_dirtystate();
        copy_Grace_to_LaTeX();
        update_default_props();
        mainWin->mainArea->completeRedraw();
    }
    else if (mimeData->hasUrls())//import from files (probably more than one)
    {
        QList<QUrl> urls=mimeData->urls();
        len=0;
        files=0;
        filenames=new char*[urls.length()+1];
        for (int i=0;i<urls.length();i++)
        {
            filename=new char[urls.at(i).toLocalFile().length()+2];
            strcpy(filename,urls.at(i).toLocalFile().toAscii().constData());
            FILE * fn=fopen(filename,"r");
            QTextStream * ts=new QTextStream(fn);
            str=ts->readAll();
            str2=QString(str.toAscii());
            ret=QMessageBox::No;
            fclose(fn);
            if (str2.length()*3<str.length())//data could be in binary (??)
            {
                ret=QMessageBox::question(mainWin,QObject::tr("Warning!"),QObject::tr("Problem during import of file ") + urls.at(i).toLocalFile() + QObject::tr("\nFile seems to be in binary-format. Open binary-import-filter-dialog?"),QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
                if (ret==QMessageBox::Yes)
                {
                    if (FormBinaryImportFilter==NULL)
                    {
                        FormBinaryImportFilter=new frmBinaryFormatInput(mainWin);
                    }
                    FormBinaryImportFilter->init();
                    FormBinaryImportFilter->chkHeader->setChecked(false);
                    FormBinaryImportFilter->lenDataFile->setText(filename);
                    FormBinaryImportFilter->show();
                    FormBinaryImportFilter->raise();
                    FormBinaryImportFilter->activateWindow();
                    //remember: binary import does the undo-stuff here
                }
            }
            if (ret==QMessageBox::No)//data is likely to be point-data in ascii-format
            {
                filenames[files]=new char[2+strlen(filename)];
                strcpy(filenames[files],filename);
                files++;
                commas=str.count(",");
                fullstops=str.count(".");
                if (commas>fullstops*3)
                    if (QMessageBox::question(mainWin,QObject::tr("Warning!"),QObject::tr("Problem during import of file ") + urls.at(i).toLocalFile() + QObject::tr("\nSignificantly more commas than fullstops found.\nUse comma as decimal separator?"),QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
                        exchange_point_comma=true;
                autoscale_onread=AUTOSCALE_XY;
                curtype=0;//XY
                new_set_no=0;
                if (new_set_nos!=NULL)
                {
                    delete[] new_set_nos;
                    new_set_nos=NULL;
                }
                getdata(get_cg(), filename, 0, LOAD_NXY);
                sprintf(dummy,"READ NXY \"%s\"",filename);
                ListOfChanges << QString(dummy);
                if (new_set_no>0 && new_set_nos!=NULL)
                {
                    m1=(len)/deltasize;
                    m2=(len+new_set_no)/deltasize;
                    if (m2>m1)//more memory needed
                    {
                        sdos=gnos;
                        gnos=new int[(m2+1)*deltasize];
                        memcpy(gnos,sdos,sizeof(int)*(m1+1)*deltasize);
                        delete[] sdos;
                        sdos=snos;
                        snos=new int[(m2+1)*deltasize];
                        memcpy(snos,sdos,sizeof(int)*(m1+1)*deltasize);
                        delete[] sdos;
                    }
                    for (int k=0;k<new_set_no;k++)
                    {
                        gnos[len+k]=get_cg();
                        snos[len+k]=new_set_nos[k];
                        QString str_imp=QObject::tr("Imported from ") + QString(filename);
                        setcomment(gnos[len+k],snos[len+k],str_imp.toAscii().data());
                    }
                    len+=new_set_no;
                }
            }//end of simple ascii import
            delete[] filename;
            filename=NULL;
        }//end of loop trough all files
        set_dirtystate();
        copy_Grace_to_LaTeX();
        update_default_props();
        mainWin->mainArea->completeRedraw();
    }
    else
    {
        errwin(QObject::tr("Data format in file drop not recognized!").toAscii().constData());
        /*
    cout << "unknown datatype!" << endl;
    QStringList sl=mimeData->formats();
    for (int i=0;i<sl.length();i++)
    cout << sl.at(i).toAscii().constData() << endl;
    */
    }

    if (len>0)//Undo-Stuff
    {
        if (files==0)
        {
            SetsCreated(len,gnos,snos,UNDO_COMPLETE);//import from unknown source
        }
        else
        {
            if (len==1 && files==1)
                SetImported(gnos[0],snos[0],filenames[0],cursource,LOAD_NXY,autoscale_onread);
            else
                SetsImported(len,gnos,snos,files,filenames,cursource,LOAD_NXY,autoscale_onread);
        }
        if (len==1)
        {
            ret=0;
            sprintf(dummy,"[G%d.S%d] ",gnos[0],snos[0]);
        }
        else
        {
            ret=1;
            strcpy(dummy,"");
        }
        if (mimeData->hasUrls())
            addAditionalDescriptionToLastNode(-1,QObject::tr("Set ") + QString(dummy) + ("imported after file drop in"),QString(),ret);
        else
            addAditionalDescriptionToLastNode(-1,QObject::tr("Set ") + QString(dummy) + ("imported after data drop in"),QString(),ret);
    }//end undo-stuff
    delete[] gnos;
    delete[] snos;
    delete temp_file;
    if (filename!=NULL)
        delete[] filename;
    unset_wait_cursor();
    exchange_point_comma=false;
}

void copy_LaTeX_to_Grace(void)//copy the original LaTeX-texts into the Grace-variables to be saved later
{
    for (int i=0;i<number_of_graphs();i++)
    {
        //only copy here
        g[i].labs.title.s_plotstring = copy_string(g[i].labs.title.s_plotstring, g[i].labs.title.alt_plotstring);
        g[i].labs.stitle.s_plotstring = copy_string(g[i].labs.stitle.s_plotstring, g[i].labs.stitle.alt_plotstring);
        for (int j=0;j<MAXAXES;j++)
        {
            strcpy(g[i].t[j]->tl_appstr, g[i].t[j]->orig_tl_appstr);
            strcpy(g[i].t[j]->tl_prestr, g[i].t[j]->orig_tl_prestr);
            g[i].t[j]->label.s_plotstring = copy_string(g[i].t[j]->label.s_plotstring, g[i].t[j]->label.alt_plotstring);
            for (int k=0;k<MAX_TICKS;k++)
            {
                g[i].t[j]->tloc[k].label = copy_string(g[i].t[j]->tloc[k].label, g[i].t[j]->tloc[k].orig_label);
            }
        }
        for (int j=0;j<g[i].maxplot;j++)
        {
            if (is_set_active(i,j)==true)
            {
                strcpy(g[i].p[j].comments,g[i].p[j].orig_comments);
                strcpy(g[i].p[j].lstr,g[i].p[j].orig_lstr);
                strcpy(g[i].p[j].avalue.prestr,g[i].p[j].avalue.orig_prestr);
                strcpy(g[i].p[j].avalue.appstr,g[i].p[j].avalue.orig_appstr);
            }
        }
    }
    for (int i=0;i<maxstr;i++)
    {
        pstr[i].s_plotstring = copy_string(pstr[i].s_plotstring,pstr[i].alt_plotstring);
    }
}

void copy_Grace_to_LaTeX(void)//copy the LaTeX-texts from the Grace-variables into the save-storage and convert the original (after loading, before showing)
{
    QString temp;
    //copy
    for (int i=0;i<number_of_graphs();i++)
    {
        g[i].labs.title.alt_plotstring = copy_string(g[i].labs.title.alt_plotstring, g[i].labs.title.s_plotstring);
        g[i].labs.stitle.alt_plotstring = copy_string(g[i].labs.stitle.alt_plotstring, g[i].labs.stitle.s_plotstring);
        for (int j=0;j<MAXAXES;j++)
        {
            strcpy(g[i].t[j]->orig_tl_appstr, g[i].t[j]->tl_appstr);
            strcpy(g[i].t[j]->orig_tl_prestr, g[i].t[j]->tl_prestr);
            g[i].t[j]->label.alt_plotstring = copy_string(g[i].t[j]->label.alt_plotstring, g[i].t[j]->label.s_plotstring);
            for (int k=0;k<MAX_TICKS;k++)
            {
                g[i].t[j]->tloc[k].orig_label = copy_string(g[i].t[j]->tloc[k].orig_label, g[i].t[j]->tloc[k].label);
            }
        }
        for (int j=0;j<g[i].maxplot;j++)
        {
            if (is_set_active(i,j)==true)
            {
                strcpy(g[i].p[j].orig_comments,g[i].p[j].comments);
                strcpy(g[i].p[j].orig_lstr,g[i].p[j].lstr);
                strcpy(g[i].p[j].avalue.orig_prestr,g[i].p[j].avalue.prestr);
                strcpy(g[i].p[j].avalue.orig_appstr,g[i].p[j].avalue.appstr);
            }
        }
    }
    for (int i=0;i<maxstr;i++)
    {
        pstr[i].alt_plotstring = copy_string(pstr[i].alt_plotstring,pstr[i].s_plotstring);
    }
    //convert
    if (activateLaTeXsupport==true)
    {
        for (int i=0;i<number_of_graphs();i++)
        {
            temp=QString(g[i].labs.title.s_plotstring);
            complete_LaTeX_to_Grace_Translator(temp);
            g[i].labs.title.s_plotstring = copy_string(g[i].labs.title.s_plotstring, temp.toAscii().constData());
            temp=QString(g[i].labs.stitle.s_plotstring);
            complete_LaTeX_to_Grace_Translator(temp);
            g[i].labs.stitle.s_plotstring = copy_string(g[i].labs.stitle.s_plotstring, temp.toAscii().constData());
            for (int j=0;j<MAXAXES;j++)
            {
                temp=QString(g[i].t[j]->tl_appstr);
                complete_LaTeX_to_Grace_Translator(temp);
                strcpy(g[i].t[j]->tl_appstr, temp.toAscii().constData());
                temp=QString(g[i].t[j]->tl_prestr);
                complete_LaTeX_to_Grace_Translator(temp);
                strcpy(g[i].t[j]->tl_prestr, temp.toAscii().constData());
                temp=QString(g[i].t[j]->label.s_plotstring);
                complete_LaTeX_to_Grace_Translator(temp);
                g[i].t[j]->label.s_plotstring = copy_string(g[i].t[j]->label.s_plotstring, temp.toAscii().constData());
                for (int k=0;k<MAX_TICKS;k++)
                {
                    temp=QString(g[i].t[j]->tloc[k].label);
                    complete_LaTeX_to_Grace_Translator(temp);
                    g[i].t[j]->tloc[k].label = copy_string(g[i].t[j]->tloc[k].label, temp.toAscii().constData());
                }
            }
            for (int j=0;j<g[i].maxplot;j++)
            {
                if (is_set_active(i,j)==true)
                {
                    temp=QString(g[i].p[j].comments);
                    complete_LaTeX_to_Grace_Translator(temp);
                    strcpy(g[i].p[j].comments, temp.toAscii().constData());
                    temp=QString(g[i].p[j].lstr);
                    complete_LaTeX_to_Grace_Translator(temp);
                    strcpy(g[i].p[j].lstr, temp.toAscii().constData());
                    temp=QString(g[i].p[j].avalue.prestr);
                    complete_LaTeX_to_Grace_Translator(temp);
                    strcpy(g[i].p[j].avalue.prestr, temp.toAscii().constData());
                    temp=QString(g[i].p[j].avalue.appstr);
                    complete_LaTeX_to_Grace_Translator(temp);
                    strcpy(g[i].p[j].avalue.appstr, temp.toAscii().constData());
                }
            }
        }
        for (int i=0;i<maxstr;i++)
        {
            temp=QString(pstr[i].s_plotstring);
            complete_LaTeX_to_Grace_Translator(temp);
            pstr[i].s_plotstring = copy_string(pstr[i].s_plotstring, temp.toAscii().constData());
        }
    }
}

void save_default_states(void)
{
    if (default_line!=NULL) delete[] default_line;
    default_line=new linetype;
    if (default_box!=NULL) delete[] default_box;
    default_box=new boxtype;
    if (default_ellipse!=NULL) delete[] default_ellipse;
    default_ellipse=new ellipsetype;
    if (default_string!=NULL) delete[] default_string;
    default_string=new plotstr;

    set_default_line(default_line);
    set_default_box(default_box);
    set_default_ellipse(default_ellipse);
    set_default_string(default_string);

    //do not trust the defaults!
    default_line->color=line_color;
    default_line->lines=line_lines;
    default_line->linew=line_linew;
    default_line->arrow_end=line_arrow_end;
    default_line->arrow.type=line_atype;
    default_line->arrow.length=line_asize;
    default_line->arrow.dL_ff=line_a_dL_ff;
    default_line->arrow.lL_ff=line_a_lL_ff;
    default_line->loctype=line_loctype;

    default_box->color=box_color;
    default_box->lines=box_lines;
    default_box->linew=box_linew;
    default_box->fillpattern=box_fillpat;
    default_box->fillcolor=box_fillcolor;
    default_box->loctype=box_loctype;

    default_ellipse->color=ellipse_color;
    default_ellipse->lines=ellipse_lines;
    default_ellipse->linew=ellipse_linew;
    default_ellipse->fillpattern=ellipse_fillpat;
    default_ellipse->fillcolor=ellipse_fillcolor;
    default_ellipse->loctype=ellipse_loctype;

    default_string->font=string_font;
    default_string->color=string_color;
    default_string->charsize=string_size;
    default_string->rot=string_rot;
    default_string->loctype=string_loctype;
    default_string->just=string_just;

}

void reset_default_states(void)
{
    if (default_line==NULL) return;
    line_color=default_line->color;
    line_lines=default_line->lines;
    line_linew=default_line->linew;
    line_arrow_end=default_line->arrow_end;
    line_atype=default_line->arrow.type;
    line_asize=default_line->arrow.length;
    line_a_dL_ff=default_line->arrow.dL_ff;
    line_a_lL_ff=default_line->arrow.lL_ff;
    line_loctype=default_line->loctype;

    box_color=default_box->color;
    box_lines=default_box->lines;
    box_linew=default_box->linew;
    box_fillpat=default_box->fillpattern;
    box_fillcolor=default_box->fillcolor;
    box_loctype=default_box->loctype;

    ellipse_color=default_ellipse->color;
    ellipse_lines=default_ellipse->lines;
    ellipse_linew=default_ellipse->linew;
    ellipse_fillpat=default_ellipse->fillpattern;
    ellipse_fillcolor=default_ellipse->fillcolor;
    ellipse_loctype=default_ellipse->loctype;

    string_font=default_string->font;
    string_color=default_string->color;
    string_size=default_string->charsize;
    string_rot=default_string->rot;
    string_loctype=default_string->loctype;
    string_just=default_string->just;

}

void update_default_props(void)
{
    bool sav_imm_upd;
    sav_imm_upd=immediateUpdate;
    immediateUpdate=false;
    updateRunning=true;
    if (TextProps!=NULL) TextProps->init(0);
    if (LineProps!=NULL) LineProps->init(0);
    if (BoxProps!=NULL) BoxProps->init(0);
    if (EllipseProps!=NULL) EllipseProps->init(0);
    updateRunning=false;
    immediateUpdate=sav_imm_upd;
}

