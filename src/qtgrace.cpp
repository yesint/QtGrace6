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

#include "MainWindow.h"
#include <QtGui>
#include <QTranslator>
#include <QSvgGenerator>
#include <QSvgRenderer>
#include "globals.h"
#include "patterns.h"
#include "allWidgets.h"
#include "draw.h"
#include "xprotos.h"
#include "undo_module.h"
#include "device.h"
#include <QMessageBox>
#ifdef WINDOWS_SYSTEM
#include <Windows.h>
#endif

extern int replacement_main(int argc,char **argv);

char BI_VERSION[128];
char BI_SYSTEM[256];
char SystemsDecimalPoint='.';//the default decimal-separator
char OldDecimalPoint='.';//the setting of the decimal point before a change (this is necessary to recognize a real(!) change in the decimal separator)
char DecimalPointToUse='.';//what the user wants to use as decimal separator

DrawProps draw_props = {{1, 1}, 0, TRUE, 1, 0.0, LINECAP_BUTT, LINEJOIN_MITER, 1.0, 0, FILLRULE_WINDING};
int maxgraph;
unsigned int unicode_greek_shift=0;
QMap<unsigned char,ushort> key_for_greek;
bool useQtFonts=false;
bool symbol_font_is_special=true;
int DefaultFont=0;//the default ist Times Roman - or the first font in the list
int border_percent=5;
//the defaultFontNames and the defaultFontList always match!
int nr_of_default_fonts=14;
char * defaultFontNames[14]={"Times-Roman","Times-Italic","Times-Bold","Times-BoldItalic","Helvetica","Helvetica-Oblique","Helvetica-Bold","Helvetica-BoldOblique","Courier","Courier-Oblique","Courier-Bold","Courier-BoldOblique","Symbol","ZapfDingbats"};
QList<QFont> stdFontList,defaultFontList;
//QFont * stdFont;
QFont * GuiFont,*stdGuiFont;
QFontMetrics * stdFontMetrics;
bool qtfonts_have_been_reset=false;
bool useQPrinter=false;
int print_target=PRINT_TARGET_SCREEN;//where to print to
bool printing_in_file=false;
bool point_explorer_activ=false;//blocks some actions if point-explorer-actions are currently selected

//long orig_page_w,orig_page_h;
QPrinter * stdPrinter;
QSvgGenerator * stdGenerator;
int stdOutputFormat=1;
int rtiCheckTime=1000;
char dummy[4096];
char dummy2[4096];
char default_grace_file[128];//only the file-name of the default file to load at the beginning and after File->New is clicked
char last_formula[512];//the last formula used for computation (like regressions, fits, evaluate expression)
char saved_formula[512];//the formula to remember
double last_fit_falues[MAXPARM];//the parameters of the last fit
double saved_value;//a constant value to remember

bool hdeviceFlag;
int hardCopyDeviceNr;
int initial_x_pos=65,initial_y_pos=65;
int initial_width=872,initial_height=670;

bool activate_id_replacing=false;//makes the qt_special_scanner replace the found set ids with new ones
int replace_o_set_ids,replace_n_set_ids;//number of set ids (original and new set ids - meaning right and left hand side of an equation)
//if set-id is '-1' a new set is to be allocated and the new id is writen in the id-arrays
//gnos always have to be real ids and no '-1'
int * replace_o_gnos=NULL;
int * replace_o_snos=NULL;
int * replace_n_gnos=NULL;
int * replace_n_snos=NULL;
int nr_of_single_f_tokens=0;
class single_formula_token * formula_tokens=NULL;//counters and double-values
extern int find_single_formula_token_in_list(int anz,class single_formula_token * tokenlist,char * token);
extern int find_next_single_formula_token(char * formula,class single_formula_token * n_token);
extern int find_all_single_formula_tokens(char * formula,class single_formula_token ** n_token);
extern int find_single_formula_tokens_in_list(QStringList list,int & nr_of_tokens_found,class single_formula_token ** n_token);
extern void replace_single_formula_tokens(QString old_formula,QString & new_formula);
extern bool init_fftw3_library(void);
extern void check_external_lib_usability(void);
extern QString get_filename_with_extension(int device);

QLocale * cur_loc;//the Locale-settings as set in the operating system
QTextCodec * FileCodec;//standard codec of the operating system (depends on user settings in the operating system)
QTextCodec * FileCodecSave;//to save the old codec in case it is changes during file-loading-operation
char new_encoding_name[128];
QTextCodec * IsoLatin1Codec;//codec for Iso-Latin-1-encoding which is identical to the T1-codec Grace uses internally (iso latin 1 is always available in Qt)
QList<QByteArray> avcod;//all available file encodings

QLocale * comma_locale;//a setting where ',' is the decimal separator (we use the setting for Germany here)
QLocale * dot_locale;//a setting where '.' is the decimal separator (we use the setting for the USA here)

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
frmRealTimeInputManager * FormRTIManage;
frmCommands * FormCommands;
frmDeviceSetup * FormDeviceSetup;
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
frmMasterRegionOperator * FormRegionMaster;
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
frmProgressWin * FormProgress;
frmQuestionDialog * FormQuestion;
frmSimpleListSelectionDialog * FormSimpleListSel;

frmBinaryFormatInput * FormBinaryImportFilter;

frmEditBlockData * FormEditBlockData;

frmFontTool * FormFontTool;
frmAbout * FormAbout;
frmSetEditor * FormSetEditor;
frmUndoList * FormUndoList;

frm_Preferences * Form_Preferences;
//frmColorManagement * FormColManage;
//frmPreferences * FormPreferences;
//frmExtraPreferences * ExtraPreferences;
frmAgrInfos * Form_AgrInfo;

QSettings * allPrefs;
//bool use_new_print_dialog=false;
bool display_help_externally=false;
bool showhideworkaround=false;
bool use_new_icons=false;
bool use_print_command=false;
//char desired_hardcopy_filename[GR_MAXPATHLEN];

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

extern graph * g;
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
extern Device_entry * device_table;
extern char print_cmd[GR_MAXPATHLEN];
extern int nfonts;
extern int lastPrintDevice;

extern defaults d_d;
extern defaults file_defaults;
extern defaults ini_defaults;
extern defaults tmp_defaults;

extern view d_v;
extern view file_view;
extern view ini_view;
extern view tmp_view;

//linestyle-settings for temporary use
int n_linestyles_ini=0,n_linestyles_tmp=0,n_linestyles_file=0;
int *l_linestyle_pat_ini=NULL,*l_linestyle_pat_tmp=NULL,*l_linestyle_pat_file=NULL;
char **linestyle_pat_ini=NULL,**linestyle_pat_tmp=NULL,**linestyle_pat_file=NULL;

//currently used linestyles
int nr_of_current_linestyles=0;
int * lenghts_of_linestyle_patterns=NULL;
char ** current_linestyle_patterns=NULL;

//current linestyle representations
QIcon ** LineIcons=NULL;
QPixmap ** LinePixmaps=NULL;
QVector<qreal> ** PenDashPattern=NULL;

int ngraph_select_items = 0;
uniList ** graph_selectors=NULL;
int ngraph_selectors = 0,n_gr_sel_alloc=0;
int ncolor_option_items = 0;
ColorSelector **color_selectors = NULL;
int ncolor_selectors = 0,n_col_sel_alloc=0;
uniList ** set_selectors=NULL;
int nset_selectors = 0,n_set_sel_alloc=0;
uniList ** plist = NULL;
int nplist = 0,n_plist_alloc=0;
FontSelector ** font_selectors=NULL;
int nfont_selectors=0,n_font_sel_alloc=0;
stdLineEdit ** LaTeX_Lines=NULL;
int nr_of_latex_lines=0,n_of_latex_lines_alloc=0;
LineStyleSelector ** line_style_selectors=NULL;
int nr_of_line_style_selectors=0,n_of_line_style_selectors_alloc=0;

struct ColumnFormat binaryImportFormat[NUMBER_OF_COLUMN_FORMATS];
char binaryImportFormatName[NUMBER_OF_COLUMN_FORMATS][32];
int ImportDestination[NUMBER_OF_IMPORT_DESTINATIONS];
char ImportDestinationName[NUMBER_OF_IMPORT_DESTINATIONS][32];
signed char ImportDestinationType[NUMBER_OF_IMPORT_DESTINATIONS];

int bin_format_size(int nr)
{
return binaryImportFormat[nr].size;
}

int find_import_destination(char * name,signed char type)
{
int ret=-1;
int size;
for (int i=0;i<NUMBER_OF_IMPORT_DESTINATIONS;i++)
{
size=strlen(ImportDestinationName[i]);
//cout << "destination" << i << "=#"<< ImportDestinationName[i] << "#" << endl;
    if ((ImportDestinationType[i]&type)!=0 && strncmp(ImportDestinationName[i],name,size)==0)
    {
    ret=i;
    break;
    }
}
///cout << "to look for=#"<< name << "# --> ret=" << ret << " Type=" << int(type) << endl;
return ret;
}

int find_import_format(char * name)
{
int ret=-1;
int size;
    for (int i=0;i<NUMBER_OF_COLUMN_FORMATS;i++)
    {
    size=strlen(binaryImportFormatName[i]);
        if (strncmp(binaryImportFormatName[i],name,size)==0)
        {
        ret=i;
        break;
        }
    }
return ret;
}

MainWindow * mainWin=NULL;
QImage * MainPixmap=NULL;
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

/*char user_home_dir[1024];
char qt_grace_exe_dir[1024];*/

QFile path_to_fftw3_lib,path_to_libharu;

//extern QTime lastc;
extern void GenerateLaTeXCommands(void);
extern void find_first_command(char * text,int & start_command,int & command_length,int * brace_pos);
extern void splitter(char * text,int start_command,int command_length,int * brace_pos,char * left_part,char * middle_part,char * argument_part,char * right_part);
extern void complete_LaTeX_to_Grace_Translator(QString & text);
extern void update_font_selectors(bool appearance);
extern void update_ss_editors(int gno);
extern void update_line_style_selectors(void);

int max_history=10;//maximum number of recently opend files to be stored
int current_history=0;//content of history
QString history[MAX_HISTORY];//actual history

//index=0 is reserved for english, which is the default
QTranslator translator[nr_of_translations];
char translator_files[nr_of_translations][64];
char translator_languages[nr_of_translations][64];
int current_language=0;

//for new sets allocated during command execution
QStringList NewGraphSetCommands;//commands used to create a new graph/set
QStringList NewGraphSetReplacement;//the replacement for the commands (i.e. the new graph id or set id)
QList<int> NewGraphSetCommandTypeIndex[5];//tells the indexes of the 5 different NewGraphSet-types
QList<int> IndexNewGraphCommand;
QList<int> IndexNewSetCommand;
QList<int> IndexNewGraphReplacement;
QList<int> IndexNewSetReplacement;
#ifdef __cplusplus
extern "C" {
#endif
int current_origin_graph=0,current_origin_set=0;
int current_target_graph=0,current_target_set=0;
int error_count=0;
#ifdef __cplusplus
}
#endif
void copy_Grace_to_LaTeX(void);
#ifdef __cplusplus
extern "C" {
#endif
int qtspecial_scanner(char * command, int * errors);
void prepare_strings_for_saving(void);
void resume_strings_after_load_or_save(void);
#ifdef __cplusplus
}
#endif

int find_QtFont_in_List(char * name,int whatlist);//you need a qt-font-name for this
int find_QtFont_in_List(QString fontname,int whatlist);//finds a qt-font-name
int find_default_id_of_Grace_Font(char * name);//you need a Grace-font-name for this
int find_GraceFontID_of_QtFontName(QString fontname);//you need a qt-font-name for this
QString getNameOfDefaultQtFont(int index);
QString get_QtName_of_Default_Grace_Font(char * name);//compares a Grace-font-name with the default font names and returns the qt-font-name
char * get_Grace_Font_Name_of_Default_Qt_Font(QString fontname);//returns the Grace-font-name of a Qt-Font-name
char * getNameOfStdQtFont(int index);//returns the Qt-Name of the Font in the StdList
char * getFamilyNameOfStdQtFont(int index);//returns the Qt-Name of the Font in the StdList

//this is what you need
//--> remember: whatlist=0-->default-list, whatlist=1-->std-list (i.e. the current list - you probably want to use this)
//--> convert Grace-name into QtFontID
int get_QtFontID_from_Grace_Name(char * name,int whatlist);//get the index of a QtFont in a list by its Grace-name
//--> convert Grace-font-id into QtFontID
int get_QtFontID_from_GraceID(int font_id,int whatlist);

void update_default_props(void);
void init_color_icons(void);
void read_settings(void);
void generate_string_Qt_aware(char * string,QString text);
void convert_single_string_from_encoding_to_UTF8(char ** text);
void convert_single_string_from_UTF8_to_encoding(char ** text);
void convert_all_strings_from_encoding_to_UTF8(void);//translation from user selected file encoding into UTF-8 (usually only after load- or save-operations)
void convert_all_strings_from_UTF8_to_encoding(void);//translation from UTF-8 into user selected file encoding (usually only before save-operations)
QString generateDisplayStringFromGraceString(char * gracestring);
extern void RedisplayString(char * str);
extern void RedisplayFormula(char * str);
extern int containsSpecialCommand(char * com,char ** parameters);
void replace_set_ids_in_command(QString & commandString,int o_gno,int o_sno,int n_gno,int n_sno,int relative);
void update_fonts_to_default_ordering(void);

//standard-settings for importing binary files - used for drag and drop
int nr_of_std_bin_import_settings=0;
struct importSettings * std_bin_import_settings=NULL;
struct importSettings current_bin_import_settings;

extern void initSettings(struct importSettings & iset,bool remove_old_settings=true);
extern void LoadFileFormat(char * fname,struct importSettings & imp_set);
extern bool is_diadem(char * file,char ** header_name);
extern int is_agr_file(char * file);
extern void read_DIAdem_header(char * header_file,struct DIAdem_Header & header);
extern void postprocess_diadem_header(struct DIAdem_Header & header,struct importSettings & bin_import,int & nr_of_new_sets,int ** n_gnos,int ** n_snos);
extern void read_INI_header(struct importSettings & imp_set,struct importSettings & imp_schema);
extern void read_BINARY_Header(struct importSettings & imp_set,struct importSettings & imp_schema);
extern int postprocess_bin_import_data(struct importSettings & imp_set,int & nr_of_new_sets,int ** n_gnos,int ** n_snos);
extern void copy_import_settings(struct importSettings * from, struct importSettings * to);

QStringList get_all_std_bin_format_files(void)
{
//cout << "qt_grace_exe_dir=" << qt_grace_exe_dir << endl;
    QDir ex_dir(qt_grace_exe_dir);
    QStringList filters;
    filters << "QtGrace_std_bin_format_*.fmt";
    //filters << "*.fmt";
    QStringList list=ex_dir.entryList(filters,QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
    filters.clear();
    for (int i=0;i<list.length();i++)
    {
    filters << (QString(qt_grace_exe_dir)+QDir::separator()+list.at(i));
    }
return filters;
}

QStringList reduce_bin_formats_to_names(QStringList list1)
{
QStringList list2;
list2.clear();
int pos,help=strlen("QtGrace_std_bin_format_");
QString tmp;
for (int i=0;i<list1.length();i++)
{
pos=list1.at(i).indexOf("QtGrace_std_bin_format_");
    if (pos>=0)
    {
    tmp=list1.at(i).mid(pos+help,list1.at(i).length()-4-(pos+help));
    list2 << tmp;
    }
}
return list2;
}

void read_std_bin_import_settings(void)//reads all format-settings for binary files that are present in the grace-executable-directory
{
QStringList list=get_all_std_bin_format_files();
/*cout << "std bin import settings=" << list.length() << endl;
for (int i=0;i<list.length();i++)
{
cout << "file=#" << list.at(i).toLocal8Bit().constData() << "#" << endl;
}*/
QStringList list2=reduce_bin_formats_to_names(list);
/*for (int i=0;i<list2.length();i++)
{
cout << "name=#" << list2.at(i).toLocal8Bit().constData() << "#" << endl;
}*/

if (list.length()>0)
{
    if (std_bin_import_settings!=NULL)
    {
        for (int i=0;i<nr_of_std_bin_import_settings;i++)
        initSettings(std_bin_import_settings[i],true);
    delete[] std_bin_import_settings;
    }
    nr_of_std_bin_import_settings=list.length();
    std_bin_import_settings=new importSettings[nr_of_std_bin_import_settings];
    for (int i=0;i<nr_of_std_bin_import_settings;i++)
    {
        char * fname=new char[2+list.at(i).length()];
        strcpy(fname,list.at(i).toLocal8Bit().constData());
        initSettings(std_bin_import_settings[i],false);
    LoadFileFormat(fname,std_bin_import_settings[i]);
        std_bin_import_settings[i].valid_status=0;
        std_bin_import_settings[i].filename=list.at(i);
        std_bin_import_settings[i].name=list2.at(i);
///cout << "fname=#" << fname << "# imp-token=" <<  std_bin_import_settings[i].nr_of_import_tokens << endl;
    //cout << "StdBinImportFormat=" << i << " DataSuffix=" << std_bin_import_settings[i].DataSuffix.toLocal8Bit().constData() << " HeaderSuffix=" << std_bin_import_settings[i].HeaderSuffix.toLocal8Bit().constData() << " import_keys=" << std_bin_import_settings[i].keys.length() << endl;
        delete[] fname;
    }
}

}

//the following function tries to guess the most suitable binary file-format for filename; returns RETURN_SUCCSESS or RETURN_FAILURE; the format-nr is returned in std_format_nr and it is returned whether this is a header or not (i.e. a binary file)
//the guessing is done based on the suffix!
int guess_bin_format(char * filename,int & std_format_nr,bool & is_header)
{
int ret=-1;
QString f_suffix;
QFileInfo finfo(filename);
f_suffix=finfo.suffix();
//cout << "f_suffix=" << f_suffix.toLocal8Bit().constData() << endl;
for (int i=0;i<nr_of_std_bin_import_settings;i++)//search through all data-file-suffixes
{
    //cout << "DataSuffix=" << std_bin_import_settings[i].DataSuffix.toLocal8Bit().constData() << endl;
    if (QString::compare(f_suffix,std_bin_import_settings[i].DataSuffix,Qt::CaseInsensitive)==0)
    {
    ret=std_format_nr=i;
    is_header=false;
    break;
    }
}
if (ret==-1)
{
    for (int i=0;i<nr_of_std_bin_import_settings;i++)//search through all header-file-suffixes
    {
        //cout << "HeaderSuffix=" << std_bin_import_settings[i].HeaderSuffix.toLocal8Bit().constData() << endl;
        if (QString::compare(f_suffix,std_bin_import_settings[i].HeaderSuffix,Qt::CaseInsensitive)==0)
        {
        ret=std_format_nr=i;
        is_header=true;
        break;
        }
    }
}
if (ret==-1)
return RETURN_FAILURE;
else
return RETURN_SUCCESS;
}

void replaceSuffix(QString & fpath,QString n_suffix)
{
int point_pos=fpath.lastIndexOf(".");
if (point_pos==-1)//no point found --> just add a suffix
{
fpath=fpath+QString(".")+n_suffix;
}
else
{
fpath=fpath.left(point_pos+1)+n_suffix;
}
}

void copy_std_settings_to_current_bin_import(char * filename,int std_format_nr,bool is_header)
{
initSettings(current_bin_import_settings);//clear settings
copy_import_settings(std_bin_import_settings+std_format_nr,&current_bin_import_settings);//copy settings from std-bin-format to current settings
if (std_bin_import_settings[std_format_nr].header_present)//there should be a header
{
    if (std_bin_import_settings[std_format_nr].header_format==1)//header in same file as bin-data
    {
    current_bin_import_settings.DataFile=QString(filename);
    current_bin_import_settings.HeaderFile=current_bin_import_settings.DataFile;
    }
    else
    {
    if (is_header==false)
    {
    current_bin_import_settings.DataFile=QString(filename);
    current_bin_import_settings.HeaderFile=current_bin_import_settings.DataFile;
    replaceSuffix(current_bin_import_settings.HeaderFile,std_bin_import_settings[std_format_nr].HeaderSuffix);
    }
    else
    {
    current_bin_import_settings.HeaderFile=QString(filename);
    current_bin_import_settings.DataFile=current_bin_import_settings.HeaderFile;
    replaceSuffix(current_bin_import_settings.DataFile,std_bin_import_settings[std_format_nr].DataSuffix);
    }
    }
}
else//no header
{
current_bin_import_settings.DataFile=QString(filename);
current_bin_import_settings.HeaderFile=QString("");
}

if (current_bin_import_settings.header_present)
{
    if (current_bin_import_settings.header_format==HEADER_FORMAT_INI_FILE)//ini-File
    {
    read_INI_header(current_bin_import_settings,std_bin_import_settings[std_format_nr]);
    }
    else if (current_bin_import_settings.header_format==HEADER_FORMAT_DATA_FILE)//binary-header in bin-file
    {
    read_BINARY_Header(current_bin_import_settings,std_bin_import_settings[std_format_nr]);
    }
    else if (current_bin_import_settings.header_format==HEADER_FORMAT_ASCII_FILE)//ascii-header-file
    {

    }
}
cout << "std=" << std_bin_import_settings[std_format_nr].nr_of_header_values << " current=" << current_bin_import_settings.nr_of_header_values << endl;
cout << "import_format=" << std_bin_import_settings[std_format_nr].channel_format[0] << "-->" << binaryImportFormatName[std_bin_import_settings[std_format_nr].channel_format[0]] << endl;

//now we should begin reading...
}
/*
void read_std_bin_file(char * filename,int std_format_nr,bool is_header,bool halt_autoscale)
{//read the data from a std-binary-file (read header and data according to the schema set as std)
initSettings(current_bin_import_settings);

copy_import_settings(std_bin_import_settings+std_format_nr,&current_bin_import_settings);

    if (std_bin_import_settings[std_format_nr].header_present)//there should be a header
    {
        if (std_bin_import_settings[std_format_nr].header_format==1)//header in same file as bin-data
        {
        current_bin_import_settings.DataFile=QString(filename);
        current_bin_import_settings.HeaderFile=current_bin_import_settings.DataFile;
        }
        else
        {
        if (is_header==false)
        {
        current_bin_import_settings.DataFile=QString(filename);
        current_bin_import_settings.HeaderFile=current_bin_import_settings.DataFile;
        replaceSuffix(current_bin_import_settings.HeaderFile,std_bin_import_settings[std_format_nr].HeaderSuffix);
        }
        else
        {
        current_bin_import_settings.HeaderFile=QString(filename);
        current_bin_import_settings.DataFile=current_bin_import_settings.HeaderFile;
        replaceSuffix(current_bin_import_settings.DataFile,std_bin_import_settings[std_format_nr].DataSuffix);
        }
        }
    }
    else//no header
    {
    current_bin_import_settings.DataFile=QString(filename);
    current_bin_import_settings.HeaderFile=QString("");
    }
/// ToDo: etwas anderes als ein ini-header
if (current_bin_import_settings.header_present)
{
    if (current_bin_import_settings.header_format==3)//ini-File
    {
    read_INI_header(current_bin_import_settings,std_bin_import_settings[std_format_nr]);
    }
    else if (current_bin_import_settings.header_format==1)//binary-header in bin-file
    {
    read_BINARY_Header(current_bin_import_settings,std_bin_import_settings[std_format_nr]);
    }
    else if (current_bin_import_settings.header_format==4)//ascii-header-file
    {

    }
}
*/

void read_bin_file_by_current_settings(bool halt_autoscale)
{
cout << "read bin files:" << endl;
cout << "Header-File=#" << current_bin_import_settings.HeaderFile.toLocal8Bit().constData() << "#" << endl;
cout << "Data - File=#" << current_bin_import_settings.DataFile.toLocal8Bit().constData() << "#" << endl;
cout << "before reading bin-file:" << current_bin_import_settings.headersize << endl;

ifstream ifi;
ifi.open(current_bin_import_settings.DataFile.toLocal8Bit().constData(),ios::binary);
cout << "reading from Bin-File=#" << current_bin_import_settings.DataFile.toLocal8Bit().constData() << "# opened?" << ifi.is_open() << endl;
cout << "std bin import: channels=" << current_bin_import_settings.channels << " points=" << current_bin_import_settings.points << endl;
readBinaryFromFile(ifi,current_bin_import_settings,&current_bin_import_settings.first_data);
cout << "std bin import: columns=" << current_bin_import_settings.columns_read << " points=" << current_bin_import_settings.points_read << endl;
ifi.close();
int nr_of_new_sets=0;
int *n_snos=NULL,*n_gnos=NULL;
cout << "Postprocessing: " << postprocess_bin_import_data(current_bin_import_settings,nr_of_new_sets,&n_gnos,&n_snos) << endl;
cout << "nr_of_new_sets=" << nr_of_new_sets << " NEW sets:" << endl;

///Undo-Stuff
SaveSetStatesPrevious(0,n_gnos,n_snos,UNDO_COMPLETE);//all sets will be new --> no previous sets --> 0
SetsModified(nr_of_new_sets,n_gnos,n_snos,UNDO_COMPLETE);
    if (nr_of_new_sets==1)//max_nr_of_sets==1)
    {
    sprintf(dummy,"[G%d.S%d]",n_gnos[0],n_snos[0]);
    addAditionalDescriptionToLastNode(-1,QObject::tr("Binary data import --> ")+QString(dummy),QString(),0);
    }
    else
    {
    addAditionalDescriptionToLastNode(-1,QObject::tr("Binary data import"),QString(),0);
    }
        for (int i=0;i<nr_of_new_sets;i++)
        {
        cout << "G" << n_gnos[i] << ".S" << n_snos[i] << endl;
        }
//cout << "AUTOSCALE: " << current_bin_import_settings.autoscale << endl;
    if (nr_of_new_sets>0 && current_bin_import_settings.autoscale!=AUTOSCALE_NONE && halt_autoscale==false)
    {
    mainWin->autoscale_proc(current_bin_import_settings.autoscale);
    }
    if (n_snos!=NULL) delete[] n_snos;
    if (n_gnos!=NULL) delete[] n_gnos;
}

double rint_v2(double x)
{
    //middle value point test
    if (ceil(x+0.5) == floor(x+0.5))
    {
        int a = (int)ceil(x);
        if (a%2 == 0)
        {return ceil(x);}
        else
        {return floor(x);}
    }
    else return floor(x+0.5);
}

QString path_to_write_settings(void)//we always write to the users home-directory
{
#ifdef WINDOWS_SYSTEM
    return QString(qt_grace_exe_dir)+QDir::separator()+QString("QtGrace_Settings.ini");

#else
    return QString(user_home_dir)+QDir::separator()+QString(".QtGrace_Settings.ini");
#endif
}

QString path_to_read_settings(void)//we try to read from the users home directory, if this fails, we use the QtGrace-executable-directory instead
{
#ifdef WINDOWS_SYSTEM
    QString path=QString(qt_grace_exe_dir)+QDir::separator()+QString("QtGrace_Settings.ini");

#else
    QString path=QString(user_home_dir)+QDir::separator()+QString(".QtGrace_Settings.ini");
#endif
    QFile fil(path);
    if (fil.exists()==false)
        path=QString(qt_grace_exe_dir)+QDir::separator()+QString("QtGrace_Settings.ini");//always visible

     return path;
}

void create_line_Patterns(void);

void copy_std_line_style_patterns(int * length,int ** style_lengths,char *** patterns)
{
    *length=MAXLINESTYLES;
    (*style_lengths)=new int[MAXLINESTYLES];
    (*patterns)=new char*[MAXLINESTYLES];
    (*style_lengths)[0]=2;
    (*style_lengths)[1]=2;
    (*style_lengths)[2]=2;
    (*style_lengths)[3]=2;
    (*style_lengths)[4]=2;
    (*style_lengths)[5]=4;
    (*style_lengths)[6]=4;
    (*style_lengths)[7]=6;
    (*style_lengths)[8]=6;
    for (int i=0;i<MAXLINESTYLES;i++)
    {
    (*patterns)[i]=new char[(*style_lengths)[i]];
    memcpy((*patterns)[i],dash_array[i],sizeof(char)*(*style_lengths)[i]);
    }
}

void init_std_line_style_patterns(void)//sets the current line-style-patterns to the Grace-standard patterns
{
    if (nr_of_current_linestyles>0)
    {
        for (int i=0;i<nr_of_current_linestyles;i++)
        {
        delete[] current_linestyle_patterns[i];
        }
        delete[] current_linestyle_patterns;
        delete[] lenghts_of_linestyle_patterns;
    }
copy_std_line_style_patterns(&nr_of_current_linestyles,&lenghts_of_linestyle_patterns,&current_linestyle_patterns);
}

//target=0 --> current
//target=1 --> tmp
//target=2 --> file
//target=3 --> ini
void copy_line_style_patterns_to_target(int n_length,int * n_style_lengths,char ** n_patterns,int target)
{
int * target_n_linestyles;
int ** target_l_linestyle_lengths;
char *** target_linestyle_patterns;
    switch (target)
    {
    case 0://current
        target_n_linestyles=&nr_of_current_linestyles;
        target_l_linestyle_lengths=&lenghts_of_linestyle_patterns;
        target_linestyle_patterns=&current_linestyle_patterns;
    break;
    case 1://tmp
        target_n_linestyles=&n_linestyles_tmp;
        target_l_linestyle_lengths=&l_linestyle_pat_tmp;
        target_linestyle_patterns=&linestyle_pat_tmp;
    break;
    case 2://file
        target_n_linestyles=&n_linestyles_file;
        target_l_linestyle_lengths=&l_linestyle_pat_file;
        target_linestyle_patterns=&linestyle_pat_file;
    break;
    case 3://ini
        target_n_linestyles=&n_linestyles_ini;
        target_l_linestyle_lengths=&l_linestyle_pat_ini;
        target_linestyle_patterns=&linestyle_pat_ini;
    break;
    }

    if ((*target_n_linestyles)>0)
    {
        for (int i=0;i<(*target_n_linestyles);i++)
        {
        delete[] (*target_linestyle_patterns)[i];
        }
        delete[] (*target_linestyle_patterns);
        delete[] (*target_l_linestyle_lengths);
    }

    (*target_n_linestyles)=n_length;
    (*target_l_linestyle_lengths)=new int[n_length];
    (*target_linestyle_patterns)=new char*[n_length];

    for (int i=0;i<(*target_n_linestyles);i++)
    {
    (*target_l_linestyle_lengths)[i]=n_style_lengths[i];
    (*target_linestyle_patterns)[i]=new char[(*target_l_linestyle_lengths)[i]];
    memcpy((*target_linestyle_patterns)[i],n_patterns[i],sizeof(char)*(*target_l_linestyle_lengths)[i]);
    }
}

void copy_line_style_patterns_to_current(int n_length,int * n_style_lengths,char ** n_patterns)//sets the current line-style-patterns to some new patterns
{
    /*if (nr_of_current_linestyles>0)
    {
        for (int i=0;i<nr_of_current_linestyles;i++)
        {
        delete[] current_linestyle_patterns[i];
        }
        delete[] current_linestyle_patterns;
        delete[] lenghts_of_linestyle_patterns;
    }
    nr_of_current_linestyles=n_length;
    lenghts_of_linestyle_patterns=new int[n_length];
    current_linestyle_patterns=new char*[n_length];
    for (int i=0;i<nr_of_current_linestyles;i++)
    {
    lenghts_of_linestyle_patterns[i]=n_style_lengths[i];
    current_linestyle_patterns[i]=new char[lenghts_of_linestyle_patterns[i]];
    memcpy(current_linestyle_patterns[i],n_patterns[i],sizeof(char)*lenghts_of_linestyle_patterns[i]);
    }*/
copy_line_style_patterns_to_target(n_length,n_style_lengths,n_patterns,0);
create_line_Patterns();
update_line_style_selectors();
}

void create_one_line_pattern(int len,char * linestyle_pattern,QPixmap ** pm,QIcon ** ic,QVector<qreal> ** pendash)
{
QPixmap templIcon(82,16);//22
QPainter templPainter;
QPen pen1(Qt::black);
templIcon.fill(QApplication::palette().window().color());
templPainter.begin(&templIcon);
if ((*pendash)!=NULL) delete (*pendash);
(*pendash)=new QVector<qreal>();
for (int j=0;j<len;j++)
*(*pendash) << linestyle_pattern[j];
if (linestyle_pattern[0]==0)//starts with white although it should start with black
{
    templPainter.setPen(pen1);
    templPainter.drawText(10,13,"None");//15
}
else
{
    pen1.setColor(Qt::black);
    pen1.setDashPattern(**pendash);
    pen1.setWidth(2);
    templPainter.setPen(pen1);
    templPainter.drawLine(0,8,82,8);//11
}
templPainter.end();
    if ((*ic)!=NULL)
    delete (*ic);
    if ((*pm)!=NULL)
    delete (*pm);
(*ic)=new QIcon(templIcon);
(*pm)=new QPixmap(templIcon);
}

void create_line_Patterns(void)//creates icon and dash-patterns for the current linestyles
{
if (LineIcons!=NULL) delete[] LineIcons;
if (LinePixmaps!=NULL) delete[] LinePixmaps;
if (PenDashPattern!=NULL) delete[] PenDashPattern;

LineIcons=new QIcon*[nr_of_current_linestyles];
LinePixmaps=new QPixmap*[nr_of_current_linestyles];
PenDashPattern=new QVector<qreal>*[nr_of_current_linestyles];

    for (int i=0;i<nr_of_current_linestyles;i++)
    {
    LineIcons[i]=NULL;
    LinePixmaps[i]=NULL;
    PenDashPattern[i]=NULL;
    create_one_line_pattern(lenghts_of_linestyle_patterns[i],current_linestyle_patterns[i],LinePixmaps+i,LineIcons+i,PenDashPattern+i);
    }
}

void reset_stdFontList(bool clear_only=false)
{
    bool save_useQtFonts=useQtFonts;
    int font_index,invalid;
    QString fname;
    QFont stfont;

    update_fonts_to_default_ordering();

    stdFontList.clear();
    //cout << "stdFontList cleared" << endl;
    if (clear_only==true)//insert default fonts
    {
        //cout << "insert default fonts" << endl;
        stdFontList=defaultFontList;//reset to default list
        return;
    }
    else//insert Grace fonts
    {
        //cout << "insert Grace font ordering" << endl;
        //reset to match the Grace-font list (so far the font list is empty)
        useQtFonts=false;//we need to access the original Grace fonts here
        for (int i=0;i<nfonts;i++)
        {
            font_index=find_default_id_of_Grace_Font(get_fontalias(i));
            if (font_index!=-1)
            {
                fname=getNameOfDefaultQtFont(font_index);
                invalid=(int)stfont.fromString(fname);
                if (invalid==0)
                {
                    char ermsg[128];
                    sprintf(ermsg,"Could not find font %s!",fname.toLatin1().constData());
                    errwin(ermsg);
                }
                else
                {
                    stdFontList << stfont;
                }
            }
            else
            {
                fname=QString();
            }
            //cout << "Looking for Grace Font #" << get_fontalias(i) << "#=" << font_index << " QtFontName=" << fname.toLatin1().constData() << endl;
        }
        useQtFonts=save_useQtFonts;
    }
    /*
    invalid=0;
    invalid+=(int)stfont.fromString(QString("Times,10,-1,5,50,0,0,0,0,0"));
    stdFontList << stfont;
    invalid+=(int)stfont.fromString(QString("Times,10,-1,5,50,1,0,0,0,0"));
    stdFontList << stfont;
    invalid+=(int)stfont.fromString(QString("Times,10,-1,5,75,0,0,0,0,0"));
    stdFontList << stfont;
    invalid+=(int)stfont.fromString(QString("Times,10,-1,5,75,1,0,0,0,0"));
    stdFontList << stfont;
    invalid+=(int)stfont.fromString(QString("Helvetica,10,-1,5,50,0,0,0,0,0"));
    stdFontList << stfont;
    invalid+=(int)stfont.fromString(QString("Helvetica,10,-1,5,50,1,0,0,0,0"));
    stdFontList << stfont;
    invalid+=(int)stfont.fromString(QString("Helvetica,10,-1,5,75,0,0,0,0,0"));
    stdFontList << stfont;
    invalid+=(int)stfont.fromString(QString("Helvetica,10,-1,5,75,1,0,0,0,0"));
    stdFontList << stfont;
    invalid+=(int)stfont.fromString(QString("Courier,10,-1,5,50,0,0,0,0,0"));
    stdFontList << stfont;
    invalid+=(int)stfont.fromString(QString("Courier,10,-1,5,50,1,0,0,0,0"));
    stdFontList << stfont;
    invalid+=(int)stfont.fromString(QString("Courier,10,-1,5,75,0,0,0,0,0"));
    stdFontList << stfont;
    invalid+=(int)stfont.fromString(QString("Courier,10,-1,5,75,1,0,0,0,0"));
    stdFontList << stfont;
    invalid+=(int)stfont.fromString(QString("Symbol,10,-1,5,50,0,0,0,0,0"));
    stdFontList << stfont;
    invalid+=(int)stfont.fromString(QString("Zapf Dingbats,10,-1,5,50,0,0,0,0,0"));
    stdFontList << stfont;
    */
    //cout << "List reinitialized with default fonts in default order!" << endl;// Number of valid fonts=" << invalid << " number of invalid fonts=" << stdFontList.length()-invalid << endl;
}

void post_load_font_status(void)//prints the fonts currently used
{
    int use_qt_fonts=useQtFonts;
    useQtFonts=false;
    cout << "Settings after load: (if qtfonts=false)" << endl;
    int nrf=number_of_fonts();
    cout << "Grace: (" << nrf << ")" << endl;
    for (int i=0;i<nrf;i++)
    {
        cout << i << ": " << get_fontalias(i) << endl;
    }
    useQtFonts=true;
    cout << "Settings after load: (if qtfonts=true)" << endl;
    for (int i=0;i<nrf;i++)
    {
        cout << i << ": " << get_fontalias(i) << endl;
    }
    useQtFonts=use_qt_fonts;
    cout << "Settings after load: (what really is set)" << endl;
    for (int i=0;i<nrf;i++)
    {
        cout << i << ": " << get_fontalias(i) << endl;
    }
    cout << "QT: (" << stdFontList.length() << ")" << endl;
    for (int i=0;i<stdFontList.length();i++)
    {
        cout << i << ": " << stdFontList.at(i).toString().toLatin1().constData() << endl;
    }

}

int find_QtFont_in_List(QString fname,int whatlist)//searches a QStingList for a QtFont with a specific Qt-font-name; whatlist=0-->in default, whatlist=1-->stdList (current fonts)
{
    QString cname;
    int len,found=-1;
    if (whatlist==0)//default
    {
        len=defaultFontList.length();
        for (int i=0;i<len;i++)
        {
            cname=defaultFontList.at(i).toString();
            if (cname.compare(fname)==0)
            {
                found=i;
                break;
            }
        }
    }
    else
    {
        len=stdFontList.length();
        for (int i=0;i<len;i++)
        {
            cname=stdFontList.at(i).toString();
            if (cname.compare(fname)==0)
            {
                found=i;
                break;
            }
        }
    }
    return found;
}

int find_QtFont_in_List(char * name,int whatlist)
{
    QString fname(name);
    return find_QtFont_in_List(fname,whatlist);
}

int find_default_id_of_Grace_Font(char * name)//looks in the defaultFontNames for the grace-name of a default-QtFont and returns its position in the default QtFont-List
{
    static int font_nr;
    font_nr=-1;
    for (int i=0;i<nr_of_default_fonts;i++)
    {
        if (strcmp(name,defaultFontNames[i])==0)
        {
            font_nr=i;
            break;
        }
    }
    return font_nr;
}

char * getNameOfStdQtFont(int index)//returns the Qt-Name of the Font in the StdList
{
    static char fname[64];
    if (index<0 || index>=stdFontList.length())
        return NULL;//fname[0]='\0';//an empty Sting
    else
        strcpy(fname,stdFontList.at(index).toString().toLocal8Bit().constData());
    return fname;
}

char * getFamilyNameOfStdQtFont(int index)//returns the Qt-Name of the Font in the StdList
{
    static char fname[64];
    if (index<0 || index>=stdFontList.length())
        return NULL;//fname[0]='\0';//an empty Sting
    else
        strcpy(fname,stdFontList.at(index).family().toLocal8Bit().constData());
    return fname;
}

QString getNameOfDefaultQtFont(int index)//returns the Qt-Name of the Font in the defaultList
{
    if (index<0 || index>=defaultFontList.length())
        return QString();//an empty Sting
    else
        return defaultFontList.at(index).toString();
}

QString get_QtName_of_Default_Grace_Font(char * name)
{
    return getNameOfDefaultQtFont(find_default_id_of_Grace_Font(name));
}

int find_QtFontId_in_List(QString fontname,int whatlist)//finds a qt-font-name in the stdFontList
{
    static int found_id,len;
    QString cname;
    found_id=-1;
    if (whatlist==0)
    {
        len=defaultFontList.length();
        for (int i=0;i<len;i++)
        {
            cname=defaultFontList.at(i).toString();
            if (cname.compare(fontname)==0)
            {
                found_id=i;
                break;
            }
        }
    }
    else
    {
        len=stdFontList.length();
        for (int i=0;i<len;i++)
        {
            cname=stdFontList.at(i).toString();
            if (cname.compare(fontname)==0)
            {
                found_id=i;
                break;
            }
        }
    }
    return found_id;
}

char * get_Grace_Font_Name_of_Default_Qt_Font(QString fontname)
{
    static char gracefontname[32];
    int len=defaultFontList.length(),found_id=-1;
    QString cname;
    for (int i=0;i<len;i++)
    {
        cname=defaultFontList.at(i).toString();
        if (cname.compare(fontname)==0)
        {
            found_id=i;
            strcpy(gracefontname,defaultFontNames[i]);
            break;
        }
    }
    if (found_id==-1)
    {
        return NULL;
    }
    else
    {
        return gracefontname;
    }
}

int get_QtFontID_from_Grace_Name(char * name,int whatlist)//get the index of a QtFont in a list by its Grace-name
{
    return find_QtFont_in_List(get_QtName_of_Default_Grace_Font(name),whatlist);//search the index of the font in the default-list or the std-list
}

int get_QtFontID_from_GraceID(int font_id,int whatlist)
{
    bool saved_use_QtFonts=useQtFonts;
    useQtFonts=false;
    int found_id=get_QtFontID_from_Grace_Name(get_fontalias(font_id),whatlist);
    useQtFonts=saved_use_QtFonts;
    return found_id;
}

int find_GraceFontID_of_QtFontName(QString fontname)
{
    bool saved_use_QtFonts=useQtFonts;
    int found_id=-1;
    useQtFonts=false;
    char * fname=get_Grace_Font_Name_of_Default_Qt_Font(fontname);
    if (fname!=NULL)
    {
        found_id=get_font_by_name(fname);//get the original font id
        /// found_id=get_font_mapped_id(found_id);//get the position in the mapped font list
    }
    useQtFonts=saved_use_QtFonts;
    return found_id;
}

//These functions convert fonts between Qt and Grace-standard (this means the implemented fonts); current=true-->search in stdFontList; current=false-->search in defaultFontList
int get_Std_GraceFontID_From_QFont(QFont & font,bool current,bool & ok)
{
    static int font_nr;
    QString fontdescription=font.toString();
    font_nr=-1;
    for (int i=0;i<nr_of_default_fonts;i++)
    {
        if (defaultFontList.at(i).toString()==fontdescription && current==false)
        {
            font_nr=i;
            break;
        }
        else if (stdFontList.at(i).toString()==fontdescription && current==true)
        {
            font_nr=i;
            break;
        }
    }
    if (font_nr==-1)
    {
        ok=false;
        return -1;//invalid font
    }
    else
    {
        ok=true;
        return font_nr;
    }
}

QFont get_Std_QFont_From_GraceFontName(char * grace_font_name,int & font_nr,bool & ok)
{
    bool saved_useQtFonts=useQtFonts;
    useQtFonts=false;
    useQtFonts=saved_useQtFonts;
    QFont stfont;
    font_nr=-1;
    if (grace_font_name==NULL)
    {
        ok=false;
        useQtFonts=saved_useQtFonts;
        return stfont;
    }
    //cout << endl << fontID << " ich suche |" << grace_font_name << "|" << endl;
    for (int i=0;i<nr_of_default_fonts;i++)
    {
        //cout << "vergleiche mit |" << defaultFontNames[i] << "|" << endl;
        if (strcmp(grace_font_name,defaultFontNames[i])==0)
        {
            font_nr=i;
            break;
        }
    }
    //cout << "font_nr in suchfunktion: " << font_nr << " length of default List:" << defaultFontList.length() << endl;
    if (font_nr==-1)
    {
        ok=false;
        useQtFonts=saved_useQtFonts;
        return QFont();
    }
    else
    {
        ok=true;
        stfont=defaultFontList.at(font_nr);
        useQtFonts=saved_useQtFonts;
        return stfont;
    }
}

QFont get_Std_QFont_From_GraceFontID(int fontID,bool & ok)
{
    static int font_nr;
    char * grace_font_name=get_fontalias(fontID);// T1_GetFontName(fontID);
    return get_Std_QFont_From_GraceFontName(grace_font_name,font_nr,ok);
}

void clear_new_set_ids(void)
{
NewGraphSetCommands.clear();
NewGraphSetReplacement.clear();
IndexNewGraphCommand.clear();
IndexNewSetCommand.clear();
IndexNewGraphReplacement.clear();
IndexNewSetReplacement.clear();
    for (int i=0;i<5;i++)
    NewGraphSetCommandTypeIndex[i].clear();
}

void ReplaceExistingNewGraphSetCommands(QString & command)
{
int type[5]={4,3,0,2,1};
for (int j=0;j<5;j++)
{
    for (int i=0;i<NewGraphSetCommandTypeIndex[type[j]].length();i++)
    {
    command.replace(NewGraphSetCommands.at(NewGraphSetCommandTypeIndex[type[j]].at(i)),NewGraphSetReplacement.at(NewGraphSetCommandTypeIndex[type[j]].at(i)));
    }
}
}

int FindFirstNewSetCommand(QString command,int offset,QString & found,int & index)//finds the first occurance of a regular expression for new-set-command and returns the type (1-5), -1=nothing found
{
int type=-1;
//Type 1
const QRegExp rex1("G\\d+.S\\d+N");//G0.S1N
//Type 2
const QRegExp rex2("S\\d+N");//only S1N (without graph-ID)
//Type 3
const QRegExp rex3("G\\d+N");//G13N (without set-ID)
//Type 4
const QRegExp rex4("G\\d+N.S\\d+");//G2N.S3
//Type 5
const QRegExp rex5("G\\d+N.S\\d+N");//G2N.S5N
index=rex1.indexIn(command,offset);
if (index>=0)//Type 1 found
{
type=1;
found=rex1.capturedTexts().at(0);
}
else//Type 1 not found
{
    index=rex5.indexIn(command,offset);
    if (index>=0)//Type 5 found
    {
    type=5;
    found=rex5.capturedTexts().at(0);
    }
    else//Type 5 not found
    {
        index=rex4.indexIn(command,offset);
        if (index>=0)//Type 4 found
        {
        type=4;
        found=rex4.capturedTexts().at(0);
        }
        else//Type 4 not found
        {
            index=rex3.indexIn(command,offset);
            if (index>=0)//Type 3 found
            {
            type=3;
            found=rex3.capturedTexts().at(0);
            }
            else//Type 3 not found
            {
                index=rex2.indexIn(command,offset);
                if (index>=0)//Type 2 found
                {
                type=2;
                found=rex2.capturedTexts().at(0);
                }
                else//Type 2 not found
                {
                //nothing found
                type=-1;
                found.clear();//set an empty string
                index=0;
                }
            }
        }
    }
}
return type;
}

int read_g_s_id_from_new_command(QString found,int type,int & i_gno,int & i_sno)
{
char * s_text=new char[8+2*found.length()];
int ret=-1,should;
i_gno=i_sno=-1;
strcpy(s_text,found.toLatin1().constData());
switch (type)
{
case 1:
should=2;
ret=sscanf(s_text,"G%d.S%dN",&i_gno,&i_sno);
break;
case 2:
should=1;
i_gno=-1;
ret=sscanf(s_text,"S%dN",&i_sno);
break;
case 3:
should=1;
i_sno=-1;
ret=sscanf(s_text,"G%dN",&i_gno);
break;
case 4:
should=2;
ret=sscanf(s_text,"G%dN.S%d",&i_gno,&i_sno);
break;
case 5:
should=2;
ret=sscanf(s_text,"G%dN.S%dN",&i_gno,&i_sno);
break;
}
    if (ret==should) ret=RETURN_SUCCESS;
    else ret=RETURN_FAILURE;
delete[] s_text;
return ret;
}

int process_command_for_new_sets(QString & command,int target_graph)//returns the number of replacements
{
command.toUpper();
    //cout << "COMMAND AT BEGINNING=#" << command.toLocal8Bit().constData() << "#" << endl;

/*
//Replace known existing commands
ReplaceExistingNewGraphSetCommands(command);
*/

//now we have to find new NewGraphSetCommands
int index,offset,type,ret,nr_of_replacemants=0;
int i_gno,i_sno;
int n_gno,n_sno;
char s_text[64];
QString found;
offset=0;
type=0;
current_target_graph=target_graph;
while (type>-1)
{
type=FindFirstNewSetCommand(command,offset,found,index);

if (type>0)
{
nr_of_replacemants++;
ret=read_g_s_id_from_new_command(found,type,i_gno,i_sno);
cout << "READ:#" << found.toLatin1().constData() << "# type=" << type << " ID=G" << i_gno << ".S" << i_sno << endl;
ret=NewGraphSetCommands.indexOf(found);
    if (ret>=0)//we already know about that --> replace this and find the next
    {
    command.replace(NewGraphSetCommands.at(ret),NewGraphSetReplacement.at(ret));
    continue;
    }
}

//we now should know the id used in the new set command (i_gno, i_sno)
n_gno=n_sno=-1;
for (int i=0;i<IndexNewGraphCommand.length();i++)
{
    if (IndexNewGraphCommand.at(i)==i_gno)
    {
    n_gno=IndexNewGraphReplacement.at(i);
        if (IndexNewSetCommand.at(i)==i_sno)
        {
        n_sno=IndexNewSetReplacement.at(i);
        }
    }
}

switch (type)
{
case 1://G0.S1N --> generate a new set in Graph 0
NewGraphSetCommands << found;
IndexNewGraphCommand << i_gno;
IndexNewSetCommand << i_sno;
n_gno=i_gno;
n_sno=nextset(n_gno);
activateset(n_gno,n_sno);
add_point(n_gno,n_sno,0.0,0.0);
SetsCreated(1,&n_gno,&n_sno,UNDO_COMPLETE);
sprintf(s_text,"G%d.S%d",n_gno,n_sno);
IndexNewGraphReplacement << n_gno;
IndexNewSetReplacement << n_sno;
NewGraphSetReplacement << QString(s_text);
    //cout << "Replacement=" << s_text << endl;
NewGraphSetCommandTypeIndex[type-1] << NewGraphSetCommands.length()-1;
//replace all of this new GraphSetCommands (in order to not find it again!)
command.replace(found,QString(s_text));
break;
case 2://S2N --> generate a new set in target_graph
n_gno=target_graph;
NewGraphSetCommands << found;
IndexNewGraphCommand << -1;//no graph set explicitely
IndexNewSetCommand << i_sno;
n_sno=nextset(n_gno);
activateset(n_gno,n_sno);
add_point(n_gno,n_sno,0.0,0.0);
SetsCreated(1,&n_gno,&n_sno,UNDO_COMPLETE);
sprintf(s_text,"G%d.S%d",n_gno,n_sno);
IndexNewGraphReplacement << n_gno;
IndexNewSetReplacement << n_sno;
NewGraphSetReplacement << QString(s_text);
    //cout << "Replacement=" << s_text << endl;
NewGraphSetCommandTypeIndex[type-1] << NewGraphSetCommands.length()-1;
//replace all of this new GraphSetCommands (in order to not find it again!)
command.replace(found,QString(s_text));
break;
case 3://G1N --> generate a new graph only
NewGraphSetCommands << found;
IndexNewGraphCommand << i_gno;
IndexNewSetCommand << -1;//no set set explicitely
n_gno=create_new_graph();
GraphsCreated(1,&n_gno,UNDO_COMPLETE);
sprintf(s_text,"G%d",n_gno);
IndexNewGraphReplacement << n_gno;
IndexNewSetReplacement << -1;
NewGraphSetReplacement << QString(s_text);
    //cout << "Replacement=" << s_text << endl;
NewGraphSetCommandTypeIndex[type-1] << NewGraphSetCommands.length()-1;
//replace all of this new GraphSetCommands (in order to not find it again!)
command.replace(found,QString(s_text));
break;
case 4://G1N.S0
case 5://G2N.S3N --> generate a new Graph and a new set!
NewGraphSetCommands << found;
IndexNewGraphCommand << i_gno;
IndexNewSetCommand << i_sno;
if (n_gno<0)
{
n_gno=create_new_graph();
GraphsCreated(1,&n_gno,UNDO_COMPLETE);
}
if (n_sno==-1 || is_set_active(n_gno,n_sno)==FALSE)
{
n_sno=nextset(n_gno);
activateset(n_gno,n_sno);
add_point(n_gno,n_sno,0.0,0.0);
SetsCreated(1,&n_gno,&n_sno,UNDO_COMPLETE);
}
sprintf(s_text,"G%d.S%d",n_gno,n_sno);
IndexNewGraphReplacement << n_gno;
IndexNewSetReplacement << n_sno;
NewGraphSetReplacement << QString(s_text);
    //cout << "Replacement=" << s_text << endl;
NewGraphSetCommandTypeIndex[type-1] << NewGraphSetCommands.length()-1;
//replace all of this new GraphSetCommands (in order to not find it again!)
command.replace(found,QString(s_text));
break;
}//end switch
}//end while-loop
    //cout << "COMMAND AT END=#" << command.toLocal8Bit().constData() << "#" << endl;
return nr_of_replacemants;
}

void test_write_pdf(void)
{
QPrinter * pr1=new QPrinter();
pr1->setOutputFormat(QPrinter::PdfFormat);
pr1->setOutputFileName(QString("/Users/andreaswinter/test_pdf_out.pdf"));
QPainter * paint1=new QPainter(pr1);
     /*if (! paint1->begin(&printer))
     { // failed to open file
         qWarning("failed to open file, is it writable?");
     //return 1;
     }*/
     paint1->drawText(10, 10, "Test 1");
     if (! pr1->newPage())
     {
         qWarning("failed in flushing page to disk, disk full?");
     //return 1;
     }
     paint1->drawText(10, 10, "Test 2");
     paint1->end();

QTextDocument * textDoc=new QTextDocument();
textDoc->setPlainText(QString("Test Content"));
pr1->setOutputFileName(QString("/Users/andreaswinter/test_pdf_out_b.pdf"));
textDoc->print(pr1);
}

void test_write_svg(void)
{
    QSvgGenerator generator;
    generator.setFileName("/Users/andreaswinter/svg_test_example.svg");
         generator.setSize(QSize(200, 200));
         generator.setViewBox(QRect(0, 0, 200, 200));
         generator.setTitle(QObject::tr("SVG Generator Example Drawing"));
         generator.setDescription(QObject::tr("An SVG drawing created by the SVG Generator Example provided with Qt."));

QFont fo;
QString teString;
fo.fromString(QString("Courier,10,-1,5,50,0,0,0,0,0"));

    QPainter painter;
    painter.begin(&generator);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::black);
    painter.drawRect(0,0,200,200);
    painter.drawText(20,50,QString("Test Std"));
    painter.setFont(fo);
    painter.drawText(20,100,QString("Test Courier"));
    fo.fromString(QString("Symbol,10,-1,5,50,0,0,0,0,0"));
    painter.setFont(fo);
    painter.drawText(20,150,QString("Test Symbol"));
    unsigned short unicode=956;
    char c[4];
    strncpy(c,(char*)((void*)&unicode),2);
    c[2]=c[3]='\0';
    painter.drawText(20,175,QString::fromUtf16(&unicode));
    painter.end();

    QString fileName = QString("/Users/andreaswinter/svg_test_example.svg");
    QSvgRenderer renderer(fileName);
    // Prepare a QImage with desired characteritisc
    QImage image(renderer.defaultSize(), QImage::Format_RGB32);
    // Get QPainter that paints to the image
    QPainter painter2(&image);
    renderer.render(&painter2);
    painter2.end();
    // Save, image format based on file extension
    image.save("/Users/andreaswinter/svg_test_example.png","PNG",85);
}

void init(void)
{
QChar unicode[] = {0x03B1,'a'};
unicode_greek_shift = unicode[0].unicode()-unicode[1].unicode();//difference between 'alpha' and 'a'
QString n_list[128]={
    "€",/// 32+128=160
    "ϒ",
    "ʹ",
    "≤",
    "∕",
    "∞",
    "f",
    "♣",
    "♦",
    "♥",
    "♠",
    "↔",
    "←",
    "↑",
    "→",
    "↓",//};/// 32+128+15=175

"±",
" ",
"↲",

"ℵ",
"ℑ",
"ℜ",
"℘",

"〈",
"〉",
"⌈",
"⌵",
"⌉",
"⌊",
"⌋",

"≥",
"⊂",
"⊃",
"⊄",




"×",
"∝",
"∂",
"∙",
"∠",
"∇",

"≠",
"≡",
"≈",

"⋯",

"⊗",
"⊕",


"∅",

"∈",

"®",
"©",

"Ⓒ",
"Ⓡ",

"∏",
"√",
"·",

"⇔",
"⇐",
"⇑",
"⇒",
"⇓",
"⋄",

"™",
"∑",
"⎛",
"⎜",
"⎝",
"⎡",
"⎢",
"⎣",

"⎨",

"⎬",

"∀",
"∴"};

/// <Key,value>, Key=ascii, value=unicode
key_for_greek.clear();
/// lower set
key_for_greek.insert(34,8704);
key_for_greek.insert(36,8707);
key_for_greek.insert(39,8717);//8715);
key_for_greek.insert(42,8727);

key_for_greek.insert(64,8773);
key_for_greek.insert(65,913);
key_for_greek.insert(66,914);
key_for_greek.insert(67,935);
key_for_greek.insert(68,916);
key_for_greek.insert(69,917);
key_for_greek.insert(70,934);
key_for_greek.insert(71,915);
key_for_greek.insert(72,919);
key_for_greek.insert(73,921);
key_for_greek.insert(74,977);
key_for_greek.insert(75,922);
key_for_greek.insert(76,923);
key_for_greek.insert(77,924);
key_for_greek.insert(78,925);
key_for_greek.insert(79,927);

key_for_greek.insert(80,928);
key_for_greek.insert(81,920);
key_for_greek.insert(82,929);
key_for_greek.insert(83,931);
key_for_greek.insert(84,932);
key_for_greek.insert(85,933);
key_for_greek.insert(86,962);
key_for_greek.insert(87,937);
key_for_greek.insert(88,926);
key_for_greek.insert(89,936);
key_for_greek.insert(90,918);

key_for_greek.insert(92,8756);

key_for_greek.insert(94,8869);

key_for_greek.insert(96,63717);
key_for_greek.insert(97,945);
key_for_greek.insert(98,946);
key_for_greek.insert(99,967);
key_for_greek.insert(100,948);
key_for_greek.insert(101,949);
key_for_greek.insert(102,966);
key_for_greek.insert(103,947);
key_for_greek.insert(104,951);
key_for_greek.insert(105,953);
key_for_greek.insert(106,981);
key_for_greek.insert(107,954);
key_for_greek.insert(108,955);
key_for_greek.insert(109,956);
key_for_greek.insert(110,957);
key_for_greek.insert(111,959);

key_for_greek.insert(112,960);
key_for_greek.insert(113,952);
key_for_greek.insert(114,961);
key_for_greek.insert(115,963);
key_for_greek.insert(116,964);
key_for_greek.insert(117,965);
key_for_greek.insert(118,982);
key_for_greek.insert(119,969);
key_for_greek.insert(120,958);
key_for_greek.insert(121,968);
key_for_greek.insert(122,950);

/// upper set
key_for_greek.insert(160,8364);
key_for_greek.insert(161,978);
key_for_greek.insert(162,884);
key_for_greek.insert(163,8804);
key_for_greek.insert(164,8260);//8725);
key_for_greek.insert(165,8734);
key_for_greek.insert(166,402);
key_for_greek.insert(167,9827);
key_for_greek.insert(168,9830);
key_for_greek.insert(169,9829);
key_for_greek.insert(170,9824);
key_for_greek.insert(171,8596);
key_for_greek.insert(172,8592);
key_for_greek.insert(173,8593);
key_for_greek.insert(174,8594);
key_for_greek.insert(175,8595);

key_for_greek.insert(176,176);
key_for_greek.insert(177,177);
key_for_greek.insert(178,8243);//8706);//"
key_for_greek.insert(179,8805);
key_for_greek.insert(180,215);
key_for_greek.insert(181,8733);
key_for_greek.insert(182,8706);
key_for_greek.insert(183,8226);//8729);
key_for_greek.insert(184,247);
key_for_greek.insert(185,8800);
key_for_greek.insert(186,8801);
key_for_greek.insert(187,8776);
key_for_greek.insert(188,8230);//8943);
key_for_greek.insert(189,63718);//8960);
key_for_greek.insert(190,63719);//9188);
key_for_greek.insert(191,8626);

key_for_greek.insert(192,8501);
key_for_greek.insert(193,8465);
key_for_greek.insert(194,8476);
key_for_greek.insert(195,8472);
key_for_greek.insert(196,8855);
key_for_greek.insert(197,8853);
key_for_greek.insert(198,8709);
key_for_greek.insert(199,8745);
key_for_greek.insert(200,8746);
key_for_greek.insert(201,8835);
key_for_greek.insert(202,8839);
key_for_greek.insert(203,8836);
key_for_greek.insert(204,8834);
key_for_greek.insert(205,8838);
key_for_greek.insert(206,8712);
key_for_greek.insert(207,8713);

key_for_greek.insert(208,8736);
key_for_greek.insert(209,8711);
key_for_greek.insert(210,63720);//174);
key_for_greek.insert(211,63721);//69);
key_for_greek.insert(212,63722);//TM
key_for_greek.insert(213,8719);
key_for_greek.insert(214,8730);
key_for_greek.insert(215,8231);//183);
key_for_greek.insert(216,172);
key_for_greek.insert(217,8743);
key_for_greek.insert(218,8744);
key_for_greek.insert(219,8660);
key_for_greek.insert(220,8656);
key_for_greek.insert(221,8657);
key_for_greek.insert(222,8658);
key_for_greek.insert(223,8659);

key_for_greek.insert(224,9674);//9826);//8900);
key_for_greek.insert(225,9001);
key_for_greek.insert(226,174);//9415);
key_for_greek.insert(227,169);//9400);
key_for_greek.insert(228,8482);//TM????
key_for_greek.insert(229,8721);
key_for_greek.insert(230,9115);
key_for_greek.insert(231,9116);
key_for_greek.insert(232,9117);
key_for_greek.insert(233,9121);
key_for_greek.insert(234,9122);
key_for_greek.insert(235,9123);
key_for_greek.insert(236,9127);
key_for_greek.insert(237,9128);
key_for_greek.insert(238,9129);
key_for_greek.insert(239,63732);

key_for_greek.insert(240,63568);//unprintable character?
key_for_greek.insert(241,9002);
key_for_greek.insert(242,8747);
key_for_greek.insert(243,8992);
key_for_greek.insert(244,9134);
key_for_greek.insert(245,8993);
key_for_greek.insert(246,9118);
key_for_greek.insert(247,9119);
key_for_greek.insert(248,9120);
key_for_greek.insert(249,9124);
key_for_greek.insert(250,9125);
key_for_greek.insert(251,9126);
key_for_greek.insert(252,9131);
key_for_greek.insert(253,9132);
key_for_greek.insert(254,9133);

/*qDebug() << "Char-List=";
for (int i=16+32+16;i<16+32+32;i++)
{
qDebug() << n_list[i] << " --> " << n_list[i].at(0).unicode();
}*/

/*for (int i=0;i<16;i++)
{
qDebug() << (i*16);
for (int j=0;j<16;j++)
{
qDebug() << QChar(key_for_greek.value(i*16+j,i*16+j));
}
qDebug() << endl;
}

cout << "hex-numbers" << endl;

cout << "189 = " << 0x2300 << endl;
cout << "190 = " << 0x23E4 << endl;
cout << "184 = " << 0x00F7 << endl;
cout << "236 = " << 0x23A7 << endl;
cout << "238 = " << 0x23A9 << endl;
cout << "206 = " << 0x2208 << endl;
cout << "199 = " << 0x2229 << endl;
cout << "200 = " << 0x222A << endl;
cout << "202 = " << 0x2287 << endl;
cout << "205 = " << 0x2286 << endl;
cout << "207 = " << 0x2209 << endl;
cout << "216 = " << 0x00AC << endl;
cout << "224 = " << 0x22C4 << endl;
cout << "36 = " << 0x2203 << endl;
cout << "39 = " << 0x220B << endl;
cout << "64 = " << 0x2245 << endl;
cout << "94 = " << 0x22A5 << endl;
cout << "217 = " << 0x2227 << endl;
cout << "218 = " << 0x2228 << endl;

cout << "243 = " << 0x2320 << endl;
cout << "244 = " << 0x23AE << endl;
cout << "245 = " << 0x2321 << endl;

cout << "42  = " << 0x2217 << endl;
cout << "242 = " << 0x222B << endl;
cout << "188 = " << 0x2026 << " " << 0x20E8 << endl;

cout << "249 = " << 0x23A4 << endl;
cout << "250 = " << 0x23A5 << endl;
cout << "251 = " << 0x23A6 << endl;
*/

    QFont stfont;
    defaultFontList.clear();
    stfont.fromString(QString("Times,10,-1,5,50,0,0,0,0,0"));
    defaultFontList << stfont;
    stfont.fromString(QString("Times,10,-1,5,50,1,0,0,0,0"));
    defaultFontList << stfont;
    stfont.fromString(QString("Times,10,-1,5,75,0,0,0,0,0"));
    defaultFontList << stfont;
    stfont.fromString(QString("Times,10,-1,5,75,1,0,0,0,0"));
    defaultFontList << stfont;
    stfont.fromString(QString("Helvetica,10,-1,5,50,0,0,0,0,0"));
    defaultFontList << stfont;
    stfont.fromString(QString("Helvetica,10,-1,5,50,1,0,0,0,0"));
    defaultFontList << stfont;
    stfont.fromString(QString("Helvetica,10,-1,5,75,0,0,0,0,0"));
    defaultFontList << stfont;
    stfont.fromString(QString("Helvetica,10,-1,5,75,1,0,0,0,0"));
    defaultFontList << stfont;
    stfont.fromString(QString("Courier,10,-1,5,50,0,0,0,0,0"));
    defaultFontList << stfont;
    stfont.fromString(QString("Courier,10,-1,5,50,1,0,0,0,0"));
    defaultFontList << stfont;
    stfont.fromString(QString("Courier,10,-1,5,75,0,0,0,0,0"));
    defaultFontList << stfont;
    stfont.fromString(QString("Courier,10,-1,5,75,1,0,0,0,0"));
    defaultFontList << stfont;
    stfont.fromString(QString("Symbol,10,-1,5,50,0,0,0,0,0"));
    defaultFontList << stfont;
    stfont.fromString(QString("Zapf Dingbats,10,-1,5,50,0,0,0,0,0"));
    defaultFontList << stfont;

    reset_stdFontList();

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

    /*for (int i=0;i<13;i++)
    {
    cout << "import_format=" << binaryImportFormatName[i] << " size=" << 8*binaryImportFormat[i].size << endl;
    }*/

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
    strcpy(ImportDestinationName[23],"Sampling Rate (1/s)");
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
    ImportDestination[29]=IMPORT_TO_CHANNEL0_FACTOR;
    strcpy(ImportDestinationName[29],"Factor for Channel 0");
    ImportDestinationType[29]=1;//only for header
    ImportDestination[30]=IMPORT_TO_CHANNEL1_FACTOR;
    strcpy(ImportDestinationName[30],"Factor for Channel 1");
    ImportDestinationType[30]=1;//only for header
    ImportDestination[31]=IMPORT_TO_CHANNEL2_FACTOR;
    strcpy(ImportDestinationName[31],"Factor for Channel 2");
    ImportDestinationType[31]=1;//only for header
    ImportDestination[32]=IMPORT_TO_CHANNEL3_FACTOR;
    strcpy(ImportDestinationName[32],"Factor for Channel 3");
    ImportDestinationType[32]=1;//only for header
    ImportDestination[33]=IMPORT_TO_CHANNEL4_FACTOR;
    strcpy(ImportDestinationName[33],"Factor for Channel 4");
    ImportDestinationType[33]=1;//only for header
    ImportDestination[34]=IMPORT_TO_CHANNEL5_FACTOR;
    strcpy(ImportDestinationName[34],"Factor for Channel 5");
    ImportDestinationType[34]=1;//only for header
    ImportDestination[35]=IMPORT_TO_CHANNEL6_FACTOR;
    strcpy(ImportDestinationName[35],"Factor for Channel 6");
    ImportDestinationType[35]=1;//only for header
    ImportDestination[36]=IMPORT_TO_CHANNEL7_FACTOR;
    strcpy(ImportDestinationName[36],"Factor for Channel 7");
    ImportDestinationType[36]=1;//only for header
    ImportDestination[37]=IMPORT_TO_CHANNEL8_FACTOR;
    strcpy(ImportDestinationName[37],"Factor for Channel 8");
    ImportDestinationType[37]=1;//only for header
    ImportDestination[38]=IMPORT_TO_CHANNEL9_FACTOR;
    strcpy(ImportDestinationName[38],"Factor for Channel 9");
    ImportDestinationType[38]=1;//only for header
    ImportDestination[39]=IMPORT_TO_CHANNEL10_FACTOR;
    strcpy(ImportDestinationName[39],"Factor for Channel 10");
    ImportDestinationType[39]=1;//only for header
    ImportDestination[40]=IMPORT_TO_CHANNEL11_FACTOR;
    strcpy(ImportDestinationName[40],"Factor for Channel 11");
    ImportDestinationType[40]=1;//only for header
    ImportDestination[41]=IMPORT_TO_CHANNEL12_FACTOR;
    strcpy(ImportDestinationName[41],"Factor for Channel 12");
    ImportDestinationType[41]=1;//only for header
    ImportDestination[42]=IMPORT_TO_CHANNEL13_FACTOR;
    strcpy(ImportDestinationName[42],"Factor for Channel 13");
    ImportDestinationType[42]=1;//only for header
    ImportDestination[43]=IMPORT_TO_CHANNEL14_FACTOR;
    strcpy(ImportDestinationName[43],"Factor for Channel 14");
    ImportDestinationType[43]=1;//only for header
    ImportDestination[44]=IMPORT_TO_CHANNEL15_FACTOR;
    strcpy(ImportDestinationName[44],"Factor for Channel 15");
    ImportDestinationType[44]=1;//only for header
    ImportDestination[45]=IMPORT_TO_CHANNEL0_OFFSET;
    strcpy(ImportDestinationName[45],"Offset for Channel 0");
    ImportDestinationType[45]=1;//only for header
    ImportDestination[46]=IMPORT_TO_CHANNEL1_OFFSET;
    strcpy(ImportDestinationName[46],"Offset for Channel 1");
    ImportDestinationType[46]=1;//only for header
    ImportDestination[47]=IMPORT_TO_CHANNEL2_OFFSET;
    strcpy(ImportDestinationName[47],"Offset for Channel 2");
    ImportDestinationType[47]=1;//only for header
    ImportDestination[48]=IMPORT_TO_CHANNEL3_OFFSET;
    strcpy(ImportDestinationName[48],"Offset for Channel 3");
    ImportDestinationType[48]=1;//only for header
    ImportDestination[49]=IMPORT_TO_CHANNEL4_OFFSET;
    strcpy(ImportDestinationName[49],"Offset for Channel 4");
    ImportDestinationType[49]=1;//only for header
    ImportDestination[50]=IMPORT_TO_CHANNEL5_OFFSET;
    strcpy(ImportDestinationName[50],"Offset for Channel 5");
    ImportDestinationType[50]=1;//only for header
    ImportDestination[51]=IMPORT_TO_CHANNEL6_OFFSET;
    strcpy(ImportDestinationName[51],"Offset for Channel 6");
    ImportDestinationType[51]=1;//only for header
    ImportDestination[52]=IMPORT_TO_CHANNEL7_OFFSET;
    strcpy(ImportDestinationName[52],"Offset for Channel 7");
    ImportDestinationType[52]=1;//only for header
    ImportDestination[53]=IMPORT_TO_CHANNEL8_OFFSET;
    strcpy(ImportDestinationName[53],"Offset for Channel 8");
    ImportDestinationType[53]=1;//only for header
    ImportDestination[54]=IMPORT_TO_CHANNEL9_OFFSET;
    strcpy(ImportDestinationName[54],"Offset for Channel 9");
    ImportDestinationType[54]=1;//only for header
    ImportDestination[55]=IMPORT_TO_CHANNEL10_OFFSET;
    strcpy(ImportDestinationName[55],"Offset for Channel 10");
    ImportDestinationType[55]=1;//only for header
    ImportDestination[56]=IMPORT_TO_CHANNEL11_OFFSET;
    strcpy(ImportDestinationName[56],"Offset for Channel 11");
    ImportDestinationType[56]=1;//only for header
    ImportDestination[57]=IMPORT_TO_CHANNEL12_OFFSET;
    strcpy(ImportDestinationName[57],"Offset for Channel 12");
    ImportDestinationType[57]=1;//only for header
    ImportDestination[58]=IMPORT_TO_CHANNEL13_OFFSET;
    strcpy(ImportDestinationName[58],"Offset for Channel 13");
    ImportDestinationType[58]=1;//only for header
    ImportDestination[59]=IMPORT_TO_CHANNEL14_OFFSET;
    strcpy(ImportDestinationName[59],"Offset for Channel 14");
    ImportDestinationType[59]=1;//only for header
    ImportDestination[60]=IMPORT_TO_CHANNEL15_OFFSET;
    strcpy(ImportDestinationName[60],"Offset for Channel 15");
    ImportDestinationType[60]=1;//only for header

    strcpy(last_formula,"");
    strcpy(saved_formula,"");
        for (int i=0;i<MAXPARM;i++)
        last_fit_falues[i]=0.0;
    saved_value=0.0;

}

int main( int argc, char **argv )
{
    startupphase=1;
    strcpy(default_grace_file,"Default.agr");//the standard-Grace-File
    grdefaults=ini_defaults=file_defaults=d_d;//at the beginning the grace-defaults are to be used for all defaults
    grview=ini_view=file_view=d_v;
    stop_repaint=TRUE;
/*
#ifdef WINDOWS_SYSTEM
    cout << "Windows" << endl;
#endif
#ifdef MAC_SYSTEM
    cout << "Mac OS X" << endl;
#endif
#ifdef LINUX_SYSTEM
    cout << "Linux" << endl;
#endif
*/

    QApplication * a=new QApplication( argc, argv );

    /*a->setStyle("macintosh");
    "windows", "motif", "cde", "plastique", "windowsxp", or "macintosh"*/

    initSettings(current_bin_import_settings,false);//for binary import

    //GuiFont=new QFont();//the stdFont is used for the GUI; size 14 is used because everythings fits neatly this way
    GuiFont=new QFont(a->font());
    GuiFont->setPixelSize(13);
    stdFontMetrics=new QFontMetrics(*GuiFont);
    //GuiFont=new QFont(a->font());
    stdGuiFont=new QFont(*GuiFont);
    QApplication::setFont(*GuiFont);

    cur_loc=new QLocale();

    comma_locale=new QLocale(QLocale::German,QLocale::Germany);
    dot_locale=new QLocale(QLocale::English,QLocale::UnitedStates);

    init();//done a first time here for the fonts

    init_std_line_style_patterns();//initialize to Grace-standard
    create_line_Patterns();//create icons and dash-patterns
    for (int i=1;i<=3;i++)//just fill the other settings up for savety
    copy_line_style_patterns_to_target(nr_of_current_linestyles,lenghts_of_linestyle_patterns,current_linestyle_patterns,i);

    //At first: Initialize everything with NULL-pointers
    useQPrinter=false;
    stdPrinter=NULL;
    stdGenerator=NULL;
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
    FormRegionMaster=NULL;
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
    //FormColManage=NULL;
    FormRTIManage=NULL;
    EllipseProps=NULL;
    BoxProps=NULL;
    LineProps=NULL;
    TextProps=NULL;
    FormFontTool=NULL;
    FormCSVImport=NULL;
    FormProgress=NULL;
    FormQuestion=NULL;
    Form_Preferences=NULL;
    Form_AgrInfo=NULL;
    FormSimpleListSel=NULL;
/*
#!/bin/sh
#xmgrace-Version mit Punkten statt Kommas
env LC_NUMERIC=C LANG=de_DE xmgrace $1
*/
    //test_write_pdf();
/*
QList<QPrinterInfo> printerlist=QPrinterInfo::availablePrinters();
cout << "Available_Printers=" << printerlist.length() << endl;
for (int i=0;i<printerlist.length();i++)
{
#if QT_VERSION >= 0x050000
cout << printerlist.at(i).description().constData() << endl;
#else
cout << printerlist.at(i).printerName().constData() << endl;
#endif
}
struct lconv * lc = localeconv();
cout << lc->decimal_point << endl;
*/

setlocale(LC_NUMERIC,"C");/// we set the local decimal separator for this application to '.' (only for internal use) --> do not alter this!

/*
lc = localeconv();
cout << lc->decimal_point << endl;
setlocale(LC_NUMERIC,"");
lc = localeconv();
cout << lc->decimal_point << endl;
*/

    FileCodec=QTextCodec::codecForLocale();//get the codec most suitable for this system
    IsoLatin1Codec=QTextCodec::codecForName("ISO 8859-1");//get standard iso-latin-1 codec
    avcod=QTextCodec::availableCodecs();
    QChar dpc=cur_loc->decimalPoint();
    SystemsDecimalPoint=dpc.toLatin1();

/*
cout << "System Decimal Point=" << SystemsDecimalPoint << endl;
cout << "comma_locale = " << comma_locale->decimalPoint().toLatin1() << endl;
cout << "dot_locale = " << dot_locale->decimalPoint().toLatin1() << endl;
*/

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
    strcpy(user_home_dir,QDir::homePath().toLocal8Bit());
    strcpy(qt_grace_exe_dir,qApp->applicationDirPath().toLocal8Bit());

#ifdef WINDOWS_SYSTEM
    path_to_fftw3_lib.setFileName(QString(qt_grace_exe_dir)+QDir::separator()+QString("libfftw3-3.dll"));
    path_to_libharu.setFileName(QString(qt_grace_exe_dir)+QDir::separator()+QString("libhpdf.dll"));
#endif
#ifdef MAC_SYSTEM
    path_to_fftw3_lib.setFileName(QString(qt_grace_exe_dir)+QDir::separator()+QString("libfftw3-3.dylib"));
    path_to_libharu.setFileName(QString(qt_grace_exe_dir)+QDir::separator()+QString("libhpdf.dylib"));
#else
    #ifdef LINUX_SYSTEM
    path_to_fftw3_lib.setFileName(QString(qt_grace_exe_dir)+QDir::separator()+QString("libfftw3-3.so"));
    path_to_libharu.setFileName(QString(qt_grace_exe_dir)+QDir::separator()+QString("libhpdf.so"));
    #endif
#endif

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

    allPrefs=new QSettings(path_to_read_settings(),QSettings::IniFormat);
    allPrefs->sync();
/*BEGIN LINESTYLES*/
/*
init_std_line_style_patterns();//initialize to Grace-standard
create_line_Patterns();
*/
/*END LINESTYLES*/
    allPrefs->beginGroup(QString("ExtraPreferences"));
    int sellang=allPrefs->value(QString("language"),QVariant(0)).toInt();

    use_fftw3=allPrefs->value(QString("UseLibFFTW3"),QVariant(FALSE)).toInt();
    path_to_fftw3_lib.setFileName(allPrefs->value(QString("LibFFTW3_Path"),QVariant(path_to_fftw3_lib.fileName())).toString());
    use_libHaru=allPrefs->value(QString("UseLibHaru"),QVariant(FALSE)).toInt();
    path_to_libharu.setFileName(allPrefs->value(QString("LibHaru_Path"),QVariant(path_to_libharu.fileName())).toString());

    allPrefs->endGroup();
    allPrefs->beginGroup(QString("General"));
    DefaultFont=allPrefs->value(QString("DefaultFont"),QVariant(0)).toInt();
    strcpy(default_grace_file,allPrefs->value(QString("DefaultAgrFile"),QVariant(QString("Default.agr"))).toString().toLocal8Bit().constData());
    allPrefs->endGroup();
    allPrefs->beginGroup(QString("Appearance"));
    use_new_icons=allPrefs->value(QString("UseNewIcons"),QVariant(false)).toBool();
    allPrefs->endGroup();
    delete allPrefs;
        if (sellang>0)
        {
        a->installTranslator(&translator[sellang]);
        current_language=sellang;
        }

    init_cursors();
    initNodes();//Undo-Stuff

    check_external_lib_usability();

    if (MainPixmap!=NULL) delete MainPixmap;

    MainPixmap=new QImage(10,10,QImage::Format_ARGB32_Premultiplied);
    MainPixmap->fill(0);
    GeneralPainter=new QPainter(MainPixmap);
/*
*=a->font();
stdFont->setPixelSize(14);
stdFontMetrics=new QFontMetrics(*stdFont);
*/
    mainWin=new MainWindow();
    a->setWindowIcon(*GraceIcon);
    stop_repaint=FALSE;
    replacement_main(argc,argv);//set up all internal Grace-things
    stop_repaint=TRUE;
    //Here setup socket connection.
    if (enableServerMode)
    {
        mainWin->SocketConnection = new LocalSocketIpcServer(sendToBeast,readFromBeast,mainWin);
    }
    else// if (enableServerMode )
    {
        fprintf(stderr, QObject::tr("QtGrace not connected to a client.\n").toLocal8Bit().constData());
    }

    if (gracebat==TRUE)//no GUI wanted
    {
        delete mainWin;
        delete GeneralPainter;
        delete MainPixmap;
        return 0;
    }
    init();//done a second time here for the pixmaps
    read_std_bin_import_settings();
    FormConsole=new frmConsole(mainWin);//needed for error-Messages
    Form_Preferences=new frm_Preferences(0);
    /*FormPreferences=new frmPreferences(0);
    FormPreferences->init();
    FormPreferences->hide();
    ExtraPreferences=new frmExtraPreferences(0);
    ExtraPreferences->init();
    ExtraPreferences->hide();*/
    Form_AgrInfo=new frmAgrInfos(0);
    //Form_AgrInfo->setWindowTitle(QString("QtGrace: ")+QObject::tr("Import set(s) from agr-file"));
    Form_AgrInfo->hide();
    EditTextProps=new frmTextProps(mainWin,true);
    EditTextProps->hide();
    EditLineProps=new frmLineProps(mainWin,true);
    EditLineProps->hide();
    EditBoxProps=new frmEllipseProps(mainWin,true,false);
    EditBoxProps->hide();
    EditEllipseProps=new frmEllipseProps(mainWin,true,true);
    EditEllipseProps->hide();
    FormProgress=new frmProgressWin(mainWin);
    FormProgress->hide();
    FormQuestion=new frmQuestionDialog(mainWin);
    FormQuestion->hide();

    read_settings();
    check_external_lib_usability();

//cout << "AFTER READ SETTINGS: useQtFonts=" << useQtFonts << endl;

    if (qApp->font()!=*GuiFont)
    {
    //cout << "Aendere Font von: " << qApp->font().toString().toLatin1().constData() << endl;
    //cout << "            nach: " << GuiFont->toString().toLatin1().constData() << endl;
    QApplication::setFont(*GuiFont);//change the Gui-Font
    }

int maxwi,maxhi;
maxwi=qApp->desktop()->width()*0.9;
maxhi=qApp->desktop()->height()*0.9;
if (initial_x_pos>maxwi)
{
initial_x_pos=0;
errwin(QObject::tr("Initial x-position to great for screen, x-position reset to 0.").toLocal8Bit().constData());
}
if (initial_y_pos>maxhi)
{
initial_y_pos=0;
errwin(QObject::tr("Initial y-position to great for screen, y-position reset to 0.").toLocal8Bit().constData());
}

    mainWin->setGeometry(initial_x_pos, initial_y_pos, initial_width, initial_height );
    mainWin->move(initial_x_pos, initial_y_pos);
    if (get_pagelayout() == PAGE_FREE)
    {
        mainWin->sldPageZoom->setValue(0);
        mainWin->sldPageZoom->setEnabled(false);
        mainWin->cmdFitPage->setEnabled(false);
    }

/*
orig_page_w=device_table[0].pg.width;//save original size
orig_page_h=device_table[0].pg.height;
device_table[0].pg.width=orig_page_w*GeneralPageZoomFactor;//use Page Zoom
device_table[0].pg.height=orig_page_h*GeneralPageZoomFactor;
*/

setlocale(LC_NUMERIC,"C");/// we set the local decimal separator for this application to '.' (only for internal use) --> do not alter this!

mainWin->mainArea->completeRedraw();
if (!hideMainWindow)
mainWin->show();
else
mainWin->hide();
    clear_dirtystate();
    CheckActive();

    mainWin->lstGraphs->update_number_of_entries();
    int sel[2]={0,1};
    mainWin->lstGraphs->set_new_selection(1,sel);
    mainWin->checkForRealTimeIO();
    update_font_selectors(true);
    //update_line_style_selectors();

if (!hideMainWindow)
{
mainWin->activateWindow();
mainWin->raise();
}
    startupphase=0;

    /*START TEST HERE*/
/*int bin_f_nr;
bool bin_f_header;

cout << "Wav: format gefunden=" << guess_bin_format("/Users/andreaswinter/QtSDK/Demos/4.7/mobile/quickhit/plugins/LevelTwo/sound/laser.wav",bin_f_nr,bin_f_header) << endl;
cout << "Format-Nr=" << bin_f_nr << " is_header=" << bin_f_header << endl;
cout << "Mpr: format gefunden=" << guess_bin_format("/Users/andreaswinter/bin_testfile.mpr",bin_f_nr,bin_f_header) << endl;
cout << "Format-Nr=" << bin_f_nr << " is_header=" << bin_f_header << endl;

bool diadem_header_found;
struct DIAdem_Header temp_header;
char dia_head1[512],*dia_head2=NULL;
strcpy(dia_head1,"/Users/andreaswinter/Desktop/qtgrace_w_undo/Diadem/test_0001_S000.I32");

diadem_header_found=is_diadem(dia_head1,&dia_head2);
cout << "is DIAdem=" << diadem_header_found << endl;
cout << "headerFile=" << dia_head2 << endl;

if (diadem_header_found==true)
{
read_DIAdem_header(dia_head2,temp_header);
cout << "Loaded: Channels=" << temp_header.nr_of_channels << endl;
    for (int i=0;i<temp_header.nr_of_channels;i++)
    {
    cout << "Channel " << i << ": points=" << temp_header.channels[i].points << " Type=" << temp_header.channels[i].ch_type << endl;
    cout << "start/offset=" << temp_header.channels[i].start << endl;
    cout << "stop/factor=" << temp_header.channels[i].step << endl;
    cout << "min=" << temp_header.channels[i].minimum << " max=" << temp_header.channels[i].maximum << endl;
    }
}

if (dia_head2!=NULL) delete[] dia_head2;

char teststring[128];*/

/*
for (double kkk=-3.1;kkk<=3.1;kkk+=0.01)
{
cout << kkk << " --> rint=" << rint(kkk) << " round=" << round(kkk) << " rint2=" << rint2(kkk)<< endl;
}

QString richTextColorTable=CreateRichTextColorTable();
int nr=0,nr_unique_ids=0,*unique_id=new int[2];
struct FoundSetID * foundIDs=new struct FoundSetID[4];

strcpy(teststring,"Dies ist ein Test: G0.S5, G1.S50Ende des Tests G5G6 TEST3G2 G8 !\n");
find_set_ids(teststring,&nr,&foundIDs);
postprocess_found_set_ids(nr,&foundIDs,&nr_unique_ids,&unique_id);//tries to identifiy multiple occurences of the same set and gives every set-id a color matching the color in the graph - if possible; unique_id gives the position of a singular id in the set-ids
*/

/*
QString ColoredString=PaintSetIds(teststring,nr,foundIDs);//,richTextColorTable);
QTextEdit * texEdit=new QTextEdit(0);
texEdit->setReadOnly(true);
texEdit->clear();
texEdit->insertHtml(ColoredString);
foundIDs[0].repl_gno=7;
foundIDs[0].repl_sno=13;
foundIDs[1].repl_gno=13;
foundIDs[1].repl_sno=7;
QString ReplacedString=ReplaceSetIds(teststring,nr,foundIDs);
texEdit->insertHtml(ReplacedString);
texEdit->show();
*/

/*
strcpy(teststring,"Dies ist ein Test: G0.S5, G1.S50Ende des Tests G5G6 TEST3G2 G8 !\n");
find_graph_ids(teststring,&nr,&foundIDs);
cout << "phys. x-dpi=" << QApplication::desktop()->physicalDpiX() << endl;
cout << "phys. y-dpi=" << QApplication::desktop()->physicalDpiY() << endl;
cout << "log. x-dpi =" << QApplication::desktop()->logicalDpiX() << endl;
cout << "log. y-dpi =" << QApplication::desktop()->logicalDpiY() << endl;
*/

/*
char teststring[128];
class single_formula_token * n_token=new class single_formula_token[10];
strcpy(teststring,"Y=sin(G0.S1.Y)+N0$+G0.S1.Y[N1#]-cos(N0$*N1#)");
cout << "TestString= # " << teststring << " #" << endl;
int anz_tokens=find_all_single_formula_tokens(teststring,&n_token);
if (formula_tokens!=NULL) delete[] formula_tokens;
formula_tokens=new class single_formula_token[anz_tokens+2];
nr_of_single_f_tokens=anz_tokens;

cout << "Anz_Tokens=" << anz_tokens << endl;
for (int k=0;k<anz_tokens;k++)
{
cout << "Token[" << k << "]=" << n_token[k].representation << endl;
formula_tokens[k].reset_token(n_token[k].representation);
}
double v=4.123;
formula_tokens[0].initialize(((void*)(&v)));
formula_tokens[1].raise_counter();
formula_tokens[1].raise_counter();
formula_tokens[1].raise_counter();
QString old_formula(teststring),new_formula;
replace_single_formula_tokens(old_formula,new_formula);
cout << "TestString(nachher)=" << new_formula.toLatin1().constData() << endl;
*/

/*class single_formula_token * n_token=new class single_formula_token();
int ret_pos=find_next_single_formula_token(teststring,n_token);
int ret_pos2;
if (ret_pos!=-1)
{
    ret_pos2=ret_pos+1;
    while (ret_pos!=-1)
    {
        cout << "ret_pos=" << ret_pos2 << endl;
        cout << "n_token.type=" << n_token->type << endl;
        cout << "n_token.rep=" << n_token->representation << endl;
    ret_pos=find_next_single_formula_token(teststring+ret_pos2,n_token);
    ret_pos2+=ret_pos+1;
    }
}*/

    /*END TEST HERE*/
/*
qDebug() << "Writer:" << QImageWriter::supportedImageFormats() << "\n";
qDebug() << "Reader:" << QImageReader::supportedImageFormats();*/
/*
/// test_write_svg();*/

/*QStringList keys,vals;
if (FormBinaryImportFilter==NULL)
{
FormBinaryImportFilter=new frmBinaryFormatInput(mainWin);
}
FormBinaryImportFilter->hide();
FormBinaryImportFilter->tabHeader->get_all_settings_from_ini_file("/Users/andreaswinter/bin_testfile.ini",keys,vals);
for (int i=0;i<keys.length();i++)
{
    qDebug() << keys.at(i) << " --> " << vals.at(i);
}*/

/*unsigned int anzahl=1000000;
void * retval;
double * retval2;*/
/// limit seems to be 160.000.000 double values on 64-bit system!?
    /*retval2=new double[1300000000];
    for (int i=0;i<1300000000;i++)
    retval2[i]=i*1.0;
    cout << "Groesse:" << (sizeof(double)*((double)1300000000.0))/1e9 << "GB" << endl;
    cout << "retval2=" << retval2 << " val=" << retval2[1299999999] << " sizeof(double)=" << sizeof(double)<< endl;
    delete[] retval2;*/
/*    retval=calloc(1300000000,sizeof(double));
    retval2=(double*)retval;
    for (int i=0;i<1300000000;i++)
    retval2[i]=i*1.0;

    cout << "retval2=" << retval2 << " val=" << retval2[1299999999] << " sizeof(double)=" << sizeof(double)<< endl;

    read_datasets_from_agr("/Users/andreaswinter/Desktop/qtgrace_w_undo/TestExpFunction.agr",0);
*/

/*
QString testNewSet[6];
testNewSet[0]=QString("Y=2+G0.S1N.x");
testNewSet[1]=QString("x=asin(S2N)");
testNewSet[2]=QString("y=G0.S1+G1N");
testNewSet[3]=QString("G0.S0.X=G1N.S0.Y");
testNewSet[4]=QString("X=sin(8*x)+G1N.S1N+G0.S2N");
testNewSet[5]=QString("G1N=sin(8*x)+G1N.S0.Y+G0.S1N");
int noffset=0;
int nindex=0;
QString found_text;
int ntype;

clear_new_set_ids();

for (int i=0;i<=5;i++)
{
//ntype=FindFirstNewSetCommand(testNewSet[i],noffset,found_text,nindex);
//cout << i << ": type=" << ntype << " index=" << nindex << " found=" << found_text.toLocal8Bit().constData() << endl;
;/// process_command_for_new_sets(testNewSet[i],0);
}*/

/*
char text[512];
int gno=0,sno=0,column=0;
strcpy(text,"G15.S4.Y3");
getSetIDFromText(text,gno,sno,column);
*/
    stop_repaint=FALSE;
mainWin->mainArea->completeRedraw();

    int execVal=a->exec();

    return execVal;
}

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
        EditTextProps->flp->buttonGroup->cmdAccept->setFocus();
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

void add_LaTeX_Line(stdLineEdit * line)
{
    if (nr_of_latex_lines==n_of_latex_lines_alloc)
    {
        stdLineEdit ** dummy=LaTeX_Lines;
        n_of_latex_lines_alloc+=5;
        LaTeX_Lines=new stdLineEdit*[n_of_latex_lines_alloc];
        for (int i=0;i<nr_of_latex_lines;i++)
            LaTeX_Lines[i]=dummy[i];
        delete[] dummy;//delete original entries (only addresses, no real stdLineEdit-ors)
    }
    LaTeX_Lines[nr_of_latex_lines]=line;
    nr_of_latex_lines++;
}

void add_Line_Style_Selector(LineStyleSelector * line)
{
    if (nr_of_line_style_selectors==n_of_line_style_selectors_alloc)
    {
        LineStyleSelector ** dummy=line_style_selectors;
        n_of_line_style_selectors_alloc+=5;
        line_style_selectors=new LineStyleSelector*[n_of_line_style_selectors_alloc];
        for (int i=0;i<nr_of_line_style_selectors;i++)
            line_style_selectors[i]=dummy[i];
        delete[] dummy;//delete original entries (only addresses, no real stdLineEdit-ors)
    }
    line_style_selectors[nr_of_line_style_selectors]=line;
    nr_of_line_style_selectors++;
}

void CheckLaTeXLinesForAddress(char * o_adr,char * n_adr)//look through all LineEdits that may have dynamic char-addresses associated and replaces o_adr with n_adr
{
    if (o_adr==n_adr) return;//nothing to do ( || o_adr==NULL)
    bool found=false;
    for (int i=0;i<nr_of_latex_lines;i++)
    {
        found=false;
        if (LaTeX_Lines[i]->c1==o_adr)
        {
            //cout << "found one, c1 --> replaced" << endl;
            LaTeX_Lines[i]->c1=n_adr;
            found=true;
        }
        if (LaTeX_Lines[i]->c2==o_adr)
        {
            //cout << "found one, c2 --> replaced" << endl;
            LaTeX_Lines[i]->c2=n_adr;
            found=true;
        }
        if (found==true)
        {
            LaTeX_Lines[i]->RedisplayContents();
        }
    }
}

void update_encoding(void)//an update for an encoding-change
{

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

void generate_strings_for_linestyles(QString & pat_len,QString & patterns)
{
pat_len.clear();
patterns.clear();
pat_len=QString("{");
    for (int i=0;i<nr_of_current_linestyles;i++)
    {
    pat_len+=QString::number(lenghts_of_linestyle_patterns[i]);
    if (i<nr_of_current_linestyles-1) pat_len+=QString(";");
    }
pat_len+=QString("}");

patterns=QString("{");
for (int i=0;i<nr_of_current_linestyles;i++)
{
patterns+=QString("{");
    for (int j=0;j<lenghts_of_linestyle_patterns[i];j++)
    {
    patterns+=QString::number((int)(current_linestyle_patterns[i][j]));
    if (j<lenghts_of_linestyle_patterns[i]-1) patterns+=QString(";");
    }
patterns+=QString("}");
if (i<nr_of_current_linestyles-1) patterns+=QString(",");
}
patterns+=QString("}");
}

void parse_strings_for_linestyles(int len,QString pat_len,QString patterns, int ** lens_of_styles,char *** line_styles)
{
QString tmp;
int len_p=pat_len.length();
tmp=pat_len.mid(1,len_p-2);
//cout << "verkuerzt1=" << tmp.toLatin1().constData() << endl;
QStringList list=tmp.split(QString(";"));
QStringList list2;
(*lens_of_styles)=new int[len];
(*line_styles)=new char*[len];
for (int i=0;i<len;i++)
{
(*lens_of_styles)[i]=list.at(i).toInt();
(*line_styles)[i]=new char[(*lens_of_styles)[i]];
}
len_p=patterns.length();
tmp=patterns.mid(1,len_p-2);
//cout << "verkuerzt2=" << tmp.toLatin1().constData() << endl;
list=tmp.split(QString(","));
for (int i=0;i<len;i++)
{
len_p=list.at(i).length();
tmp=list.at(i).mid(1,len_p-2);
list2=tmp.split(QString(";"));
    for (int j=0;j<(*lens_of_styles)[i];j++)
    {
    (*line_styles)[i][j]=list2.at(j).toInt();
    }
}
/*int nr_of_current_linestyles=0;
int * lenghts_of_linestyle_patterns=NULL;
char ** current_linestyle_patterns=NULL;*/
}

void read_settings(void)
{
tmp_defaults=grdefaults;
tmp_view=grview;
strcpy(tmp_sformat,sformat);

    allPrefs=new QSettings(path_to_read_settings(),QSettings::IniFormat);
    allPrefs->sync();
    allPrefs->beginGroup(QString("Preferences"));

    Form_Preferences->tab_prefs->noask_item->setChecked(allPrefs->value(QString("dontaskquestions"),QVariant(false)).toBool());
    Form_Preferences->tab_prefs->dc_item->setChecked(allPrefs->value(QString("allowdoubleclickoncanvas"),QVariant(true)).toBool());
    Form_Preferences->tab_prefs->graph_focus_choice_item->setCurrentIndex(allPrefs->value(QString("graphfocusswitch"),QVariant(0)).toInt());
    Form_Preferences->tab_prefs->graph_drawfocus_choice_item->setChecked(allPrefs->value(QString("displayfocusmarker"),QVariant(true)).toBool());
    Form_Preferences->tab_prefs->autoredraw_type_item->setChecked(allPrefs->value(QString("autoredraw"),QVariant(true)).toBool());
    Form_Preferences->tab_prefs->cursor_type_item->setChecked(allPrefs->value(QString("crosshaircursor"),QVariant(false)).toBool());

    Form_Preferences->tab_prefs->max_path_item->setValue(allPrefs->value(QString("maxdrawpathlength"),QVariant(1000000)).toInt());
    Form_Preferences->tab_prefs->safe_mode_item->setChecked(allPrefs->value(QString("runinsafemode"),QVariant(true)).toBool());

    Form_Preferences->tab_prefs->scrollper_item->setValue(allPrefs->value(QString("scrollpercent"),QVariant(5)).toInt());
    Form_Preferences->tab_prefs->shexper_item->setValue(allPrefs->value(QString("zoompercent"),QVariant(5)).toInt());
    Form_Preferences->tab_prefs->linkscroll_item->setChecked(allPrefs->value(QString("linkedscrolling"),QVariant(false)).toBool());
    Form_Preferences->tab_prefs->hint_item->setCurrentIndex(allPrefs->value(QString("datehint"),QVariant(3)).toInt());
    Form_Preferences->tab_prefs->date_item->setText(allPrefs->value(QString("referencedate"),QVariant("-4713-01-01 12:00:00")).toString());
    Form_Preferences->tab_prefs->two_digits_years_item->setChecked(allPrefs->value(QString("twodigityearspan"),QVariant(false)).toBool());
    Form_Preferences->tab_prefs->wrap_year_item->setText(allPrefs->value(QString("wrapyear"),QVariant("1950")).toString());
    allPrefs->endGroup();

    allPrefs->beginGroup(QString("ExtraPreferences"));
    //Form_Preferences->tab_extra->lenHome->setText(allPrefs->value(QString("gracehomedirectory"),QVariant(qt_grace_exe_dir)).toString());
    display_help_externally=allPrefs->value(QString("showhelpexternally"),QVariant(false)).toBool();
    Form_Preferences->tab_extra->lenHelpViewer->setText(allPrefs->value(QString("helpviewer"),QVariant("")).toString());
    current_language=allPrefs->value(QString("language"),QVariant(0)).toInt();
    showhideworkaround=allPrefs->value(QString("showhideworkaround"),QVariant(false)).toBool();
    useQtFonts=allPrefs->value(QString("useQtFonts"),QVariant(false)).toBool();
    symbol_font_is_special=allPrefs->value(QString("SymbolFontIsSpecial"),QVariant(true)).toBool();
    strcpy(dummy,allPrefs->value(QString("DecimalSeparator"),QVariant(".")).toString().toLatin1().constData());
    DecimalPointToUse=dummy[0];
    Form_Preferences->tab_extra->selDecSep->setCurrentIndex(DecimalPointToUse=='.'?0:1);
    set_print_cmd(allPrefs->value(QString("PrintCommand"),QVariant(get_print_cmd())).toString().toLatin1().constData());
    use_print_command=allPrefs->value(QString("UsePrintCommand"),QVariant(false)).toBool();
    auto_set_agr_extension=allPrefs->value(QString("AutoSetAgrExtension"),QVariant(TRUE)).toInt();
    auto_set_cwd=allPrefs->value(QString("AutoSetCWD"),QVariant(TRUE)).toInt();
    auto_set_export_extensions=allPrefs->value(QString("AutoSetExportExtensions"),QVariant(TRUE)).toInt();
    useHDPrinterOutput=allPrefs->value(QString("UseHDPrinterOutput"),QVariant(FALSE)).toInt();

    use_fftw3=allPrefs->value(QString("UseLibFFTW3"),QVariant(FALSE)).toInt();
    path_to_fftw3_lib.setFileName(allPrefs->value(QString("LibFFTW3_Path"),QVariant(path_to_fftw3_lib.fileName())).toString());
    use_libHaru=allPrefs->value(QString("UseLibHaru"),QVariant(FALSE)).toInt();
    path_to_libharu.setFileName(allPrefs->value(QString("LibHaru_Path"),QVariant(path_to_libharu.fileName())).toString());

    allPrefs->endGroup();

    allPrefs->beginGroup(QString("Defaults"));
    strcpy(sformat,allPrefs->value(QString("Default_SaveFormat"),QVariant("%.8g")).toString().toLatin1().constData());
    grdefaults.color=allPrefs->value(QString("Default_Color"),QVariant(d_d.color)).toInt();
    grdefaults.bgcolor=allPrefs->value(QString("Default_BGColor"),QVariant(d_d.bgcolor)).toInt();
    grdefaults.pattern=allPrefs->value(QString("Default_Pattern"),QVariant(d_d.pattern)).toInt();
    grdefaults.lines=allPrefs->value(QString("Default_Lines"),QVariant(d_d.lines)).toInt();
    grdefaults.linew=allPrefs->value(QString("Default_Linew"),QVariant(d_d.linew)).toDouble();
    grdefaults.charsize=allPrefs->value(QString("Default_Charsize"),QVariant(d_d.charsize)).toDouble();
    grdefaults.font=allPrefs->value(QString("Default_Font"),QVariant(d_d.font)).toInt();
    grdefaults.symsize=allPrefs->value(QString("Default_Symbolsize"),QVariant(d_d.symsize)).toDouble();
    grview.xv1=allPrefs->value(QString("Default_Viewport_xv1"),QVariant(d_v.xv1)).toDouble();
    grview.yv1=allPrefs->value(QString("Default_Viewport_yv1"),QVariant(d_v.yv1)).toDouble();
    grview.xv2=allPrefs->value(QString("Default_Viewport_xv2"),QVariant(d_v.xv2)).toDouble();
    grview.yv2=allPrefs->value(QString("Default_Viewport_yv2"),QVariant(d_v.yv2)).toDouble();
    allPrefs->endGroup();

    allPrefs->beginGroup(QString("History"));
    max_history=allPrefs->value(QString("maxhistory"),QVariant(MAX_HISTORY)).toInt();
    Form_Preferences->tab_extra->histSize->setValue(max_history);
    current_history=allPrefs->value(QString("historysize"),QVariant(0)).toInt();
    for (int i=0;i<MAX_HISTORY;i++)//always save MAX_HISTORY entries (possibly empty ones)
    {
        sprintf(dummy,"history%d",i);
        history[i]=allPrefs->value(QString(dummy),QVariant("")).toString();
    }
    allPrefs->endGroup();

    allPrefs->beginGroup(QString("General"));
    lastPrintDevice=stdOutputFormat=allPrefs->value(QString("lastOutputFormat"),QVariant(1)).toInt();
    undo_active=allPrefs->value(QString("activateUndoRecords"),QVariant(false)).toBool();///undo deactivated as a default
    activateLaTeXsupport=allPrefs->value(QString("activateLaTeXSupport"),QVariant(false)).toBool();
    Form_Preferences->tab_extra->chkActivateLaTeXSupport->setChecked(activateLaTeXsupport);
    immediateUpdate=allPrefs->value(QString("ImmediateUpdates"),QVariant(false)).toBool();
    Form_Preferences->tab_extra->chkImmediateUpdate->setChecked(immediateUpdate);
    default_Print_Device=allPrefs->value(QString("DefaultPrintingDevice"),QVariant(-1)).toInt();
    Form_Preferences->tab_qtgrace_prefs2->selDefaultPrintDevice->setCurrentIndex(default_Print_Device+1);
    //use_new_print_dialog=allPrefs->value(QString("UseNewPrintingDialog"),QVariant(false)).toBool();
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
    FileCodec=QTextCodec::codecForName(codName.toLocal8Bit().constData());
    /// cout << "CodName=" << codName.toLocal8Bit().constData() << " Adress=" << FileCodec << endl;
    if (FileCodec==NULL)
    {
        cout << "Warning, invalid FileCodec! " << codName.toLocal8Bit().constData() << endl;
        FileCodec=QTextCodec::codecForLocale();//fallback is the systems usual codec
    }
    Form_Preferences->tab_extra->selCodec->setCurrentIndex(index);
    Form_Preferences->tab_extra->init();
    Form_Preferences->tab_prefs->doApply();
    Form_Preferences->tab_extra->doApply();

    mainWin->recreateHistory();
    bool res_undo=undo_active;
    undo_active=false;
    if (FormDeviceSetup==NULL)
    {
        FormDeviceSetup=new frmDeviceSetup(mainWin);
        //initialize this only on startup
        if (default_Print_Device==-1)//last one
            FormDeviceSetup->devices_item->setCurrentValue(stdOutputFormat);
        else
            FormDeviceSetup->devices_item->setCurrentValue(default_Print_Device);
        /*cout << "default_Print_Device=" << default_Print_Device << endl;
        cout << "stdOutput=" << stdOutputFormat << endl;*/
        lastPrintDevice=FormDeviceSetup->devices_item->currentValue();
        FormDeviceSetup->printfile_item->setText(get_filename_with_extension(FormDeviceSetup->cur_dev));
    }
    FormDeviceSetup->hide();
    FormDeviceSetup->print_string_item->setText( allPrefs->value(QString("PrintCommand"),QVariant("lpr")).toString() );
    FormDeviceSetup->doApply();

QString pat_lens,pat_styles;
int nr_of_l_styles;

    DefaultFont=allPrefs->value(QString("DefaultFont"),QVariant(0)).toInt();
    strcpy(default_grace_file,allPrefs->value(QString("DefaultAgrFile"),QVariant(QString("Default.agr"))).toString().toLocal8Bit().constData());
    //Form_Preferences->tab_extra->defaultFont->setCurrentIndex(DefaultFont);
    Form_Preferences->tab_qtgrace_prefs2->lenDefaultFile->setText(default_grace_file);
    warn_on_encoding_change=allPrefs->value(QString("Warn_On_Encoding_Change"),QVariant(TRUE)).toInt();
    SetToggleButtonState(FormDeviceSetup->dsync_item,allPrefs->value(QString("Sync_Page_Dimensions_On_Devices"),QVariant(true)).toBool());
    SetToggleButtonState(FormDeviceSetup->psync_item,
    allPrefs->value(QString("Rescale_Plot_On_Page_Size_Change"),QVariant(false)).toBool());
    allPrefs->endGroup();

    allPrefs->beginGroup(QString("Appearance"));

    use_new_icons=allPrefs->value(QString("UseNewIcons"),QVariant(false)).toBool();

    QColor col=mainWin->palette().color(QPalette::Background);
    sprintf(dummy,"%d;%d;%d",col.red(),col.green(),col.blue());
    pat_lens=allPrefs->value(QString("DrawAreaBackground"),QVariant(QString(dummy))).toString();
    strcpy(dummy,pat_lens.toLatin1().constData());
int col1,col2,col3;
    sscanf(dummy,"%d;%d;%d",&col1,&col2,&col3);
    col=QColor::fromRgb(col1,col2,col3);
    mainWin->mainArea->setBGtoColor(col);

    //universal_font_size_factor=allPrefs->value(QString("UniversalFontSizeFactor"),QVariant(1.0)).toDouble();//universal factor is to be written into the file not in general for the whole Application
    pat_lens=allPrefs->value(QString("GuiFont"),QVariant(qApp->font().toString())).toString();
    GuiFont->fromString(pat_lens);

    initial_x_pos=allPrefs->value(QString("InitialXPos"),QVariant(65)).toInt();
    initial_y_pos=allPrefs->value(QString("InitialYPos"),QVariant(65)).toInt();
    initial_width=allPrefs->value(QString("InitialWidth"),QVariant(872)).toInt();
    initial_height=allPrefs->value(QString("InitialHeight"),QVariant(670)).toInt();
    start_dpi=allPrefs->value(QString("StartDPI"),QVariant(qApp->desktop()->physicalDpiX())).toInt();
    Form_Preferences->tab_qtgrace_prefs2->selStartupX->setValue(initial_x_pos);
    Form_Preferences->tab_qtgrace_prefs2->selStartupY->setValue(initial_y_pos);
    Form_Preferences->tab_qtgrace_prefs2->selStartupWidth->setValue(initial_width);
    Form_Preferences->tab_qtgrace_prefs2->selStartupHeight->setValue(initial_height);
    Form_Preferences->tab_qtgrace_prefs2->selStdDpi->setValue(start_dpi);

device_table[DEVICE_X11].pg.dpi=start_dpi;
mainWin->mainArea->completeRedraw();

    show_ToolBar=allPrefs->value(QString("Show_Tool_Bar"),QVariant(TRUE)).toInt();
    show_StatusBar=allPrefs->value(QString("Show_Status_Bar"),QVariant(TRUE)).toInt();
    show_LocatorBar=allPrefs->value(QString("Show_Locator_Bar"),QVariant(TRUE)).toInt();
    show_Navi_B=allPrefs->value(QString("Show_Navigation_Buttons"),QVariant(TRUE)).toInt();
    show_Graph_List=allPrefs->value(QString("Show_Graph_List"),QVariant(TRUE)).toInt();
    show_special_Zoom=allPrefs->value(QString("Show_Special_Zoom_Buttons"),QVariant(TRUE)).toInt();
    show_Viewport_Stack=allPrefs->value(QString("Show_Viewport_Stack"),QVariant(TRUE)).toInt();
    show_Page_Zoom=allPrefs->value(QString("Show_Page_Zoom"),QVariant(TRUE)).toInt();
    show_Print_B=allPrefs->value(QString("Show_Print_Button"),QVariant(FALSE)).toInt();
    show_Export_B=allPrefs->value(QString("Show_Export_Button"),QVariant(FALSE)).toInt();
    show_host_name=allPrefs->value(QString("Show_Host_Name"),QVariant(TRUE)).toInt();
    show_display_name=allPrefs->value(QString("Show_Display_Name"),QVariant(TRUE)).toInt();
    displ_project_filename=allPrefs->value(QString("Show_Project_File"),QVariant(1)).toInt();
    displ_export_filename=allPrefs->value(QString("Show_Export_File"),QVariant(0)).toInt();

    Form_Preferences->tab_qtgrace_prefs2->init();

    allPrefs->endGroup();

    allPrefs->beginGroup(QString("LineStyles"));
    nr_of_l_styles=allPrefs->value(QString("NrOfLineStyles"),QVariant(nr_of_current_linestyles)).toInt();
    generate_strings_for_linestyles(pat_lens,pat_styles);//just to have a default
    pat_lens=allPrefs->value(QString("LineStyleLenghts"),QVariant(pat_lens)).toString();
    pat_styles=allPrefs->value(QString("LineStyles"),QVariant(pat_styles)).toString();
    int * lens_of_styles;
    char ** line_styles;
    //cout << "lengths=" << pat_lens.toLatin1().constData() << " patterns=" << pat_styles.toLatin1().constData() << endl;
    parse_strings_for_linestyles(nr_of_l_styles,pat_lens,pat_styles,&lens_of_styles,&line_styles);
    //save linestyles from ini-style
    copy_line_style_patterns_to_target(nr_of_l_styles,lens_of_styles,line_styles,3);
    //use the styles from the ini as current styles and generate icons and dash-patterns
    copy_line_style_patterns_to_current(nr_of_l_styles,lens_of_styles,line_styles);

    /*cout << "Laengen der Linestyles=" << endl;
    for (int i=0;i<nr_of_l_styles;i++)
    {
    cout << lens_of_styles[i] << endl;
    }*/

    allPrefs->endGroup();

    allPrefs->sync();
    delete allPrefs;
    undo_active=res_undo;
    /// cout << "B) Read Settings: DecSepToUse=#" << DecimalPointToUse << "#" << endl;
ini_defaults=grdefaults;
ini_view=grview;
strcpy(ini_sformat,sformat);
mainWin->ManageBars();
//if the ini file is read, the defaults are to be used forthwith as standard
}

void write_settings(void)
{
//if we write the defaults in the ini file we use the current defaults
ini_defaults=grdefaults;
ini_view=grview;
strcpy(ini_sformat,sformat);

    QString pat_lens,pat_styles;
    allPrefs=new QSettings(path_to_write_settings(),QSettings::IniFormat);

    allPrefs->beginGroup(QString("Preferences"));
    allPrefs->setValue(QString("dontaskquestions"),QVariant(Form_Preferences->tab_prefs->noask_item->isChecked()));
    allPrefs->setValue(QString("allowdoubleclickoncanvas"),QVariant(Form_Preferences->tab_prefs->dc_item->isChecked()));
    allPrefs->setValue(QString("graphfocusswitch"),QVariant(Form_Preferences->tab_prefs->graph_focus_choice_item->currentIndex()));
    allPrefs->setValue(QString("displayfocusmarker"),QVariant(Form_Preferences->tab_prefs->graph_drawfocus_choice_item->isChecked()));
    allPrefs->setValue(QString("autoredraw"),QVariant(Form_Preferences->tab_prefs->autoredraw_type_item->isChecked()));
    allPrefs->setValue(QString("crosshaircursor"),QVariant(Form_Preferences->tab_prefs->cursor_type_item->isChecked()));

    allPrefs->setValue(QString("maxdrawpathlength"),QVariant(Form_Preferences->tab_prefs->max_path_item->value()));
    allPrefs->setValue(QString("runinsafemode"),QVariant(Form_Preferences->tab_prefs->safe_mode_item->isChecked()));

    allPrefs->setValue(QString("scrollpercent"),QVariant(Form_Preferences->tab_prefs->scrollper_item->value()));
    allPrefs->setValue(QString("zoompercent"),QVariant(Form_Preferences->tab_prefs->shexper_item->value()));
    allPrefs->setValue(QString("linkedscrolling"),QVariant(Form_Preferences->tab_prefs->linkscroll_item->isChecked()));

    allPrefs->setValue(QString("datehint"),QVariant(Form_Preferences->tab_prefs->hint_item->currentIndex()));
    allPrefs->setValue(QString("referencedate"),QVariant(Form_Preferences->tab_prefs->date_item->text()));
    allPrefs->setValue(QString("twodigityearspan"),QVariant(Form_Preferences->tab_prefs->two_digits_years_item->isChecked()));
    allPrefs->setValue(QString("wrapyear"),QVariant(Form_Preferences->tab_prefs->wrap_year_item->text()));
    allPrefs->endGroup();

    allPrefs->beginGroup(QString("ExtraPreferences"));
    //allPrefs->setValue(QString("gracehomedirectory"),QVariant(Form_Preferences->tab_extra->lenHome->text()));
    allPrefs->setValue(QString("showhelpexternally"),QVariant(Form_Preferences->tab_extra->chkExternalHelpViewer->isChecked()));
    allPrefs->setValue(QString("helpviewer"),QVariant(Form_Preferences->tab_extra->lenHelpViewer->text()));
    allPrefs->setValue(QString("language"),QVariant(Form_Preferences->tab_qtgrace_prefs2->selLanguage->currentIndex()));
    allPrefs->setValue(QString("showhideworkaround"),QVariant(Form_Preferences->tab_extra->chkShowHideException->isChecked()));
    allPrefs->setValue(QString("useQtFonts"),QVariant(Form_Preferences->tab_extra->chkQtFonts->isChecked()));
    allPrefs->setValue(QString("SymbolFontIsSpecial"),QVariant(Form_Preferences->tab_extra->chkSymbolSpecial->isChecked()));
    allPrefs->setValue(QString("DecimalSeparator"),QVariant(QString(DecimalPointToUse)));
    allPrefs->setValue(QString("PrintCommand"),QVariant(get_print_cmd()));
    allPrefs->setValue(QString("UsePrintCommand"),QVariant(use_print_command));
    allPrefs->setValue(QString("AutoSetAgrExtension"),QVariant(auto_set_agr_extension));
    allPrefs->setValue(QString("AutoSetCWD"),QVariant(auto_set_cwd));
    allPrefs->setValue(QString("AutoSetExportExtensions"),QVariant(auto_set_export_extensions));
    allPrefs->setValue(QString("UseHDPrinterOutput"),QVariant(useHDPrinterOutput));

    allPrefs->setValue(QString("UseLibFFTW3"),QVariant(use_fftw3));
    allPrefs->setValue(QString("LibFFTW3_Path"),QVariant(path_to_fftw3_lib.fileName()));
    allPrefs->setValue(QString("UseLibHaru"),QVariant(use_libHaru));
    allPrefs->setValue(QString("LibHaru_Path"),QVariant(path_to_libharu.fileName()));

    allPrefs->endGroup();
    allPrefs->beginGroup(QString("Defaults"));
    allPrefs->setValue(QString("Default_SaveFormat"),QVariant(sformat));
    allPrefs->setValue(QString("Default_Color"),QVariant(grdefaults.color));
    allPrefs->setValue(QString("Default_BGColor"),QVariant(grdefaults.bgcolor));
    allPrefs->setValue(QString("Default_Pattern"),QVariant(grdefaults.pattern));
    allPrefs->setValue(QString("Default_Lines"),QVariant(grdefaults.lines));
    allPrefs->setValue(QString("Default_Linew"),QVariant(grdefaults.linew));
    allPrefs->setValue(QString("Default_Charsize"),QVariant(grdefaults.charsize));
    allPrefs->setValue(QString("Default_Font"),QVariant(grdefaults.font));
    allPrefs->setValue(QString("Default_Symbolsize"),QVariant(grdefaults.symsize));
    allPrefs->setValue(QString("Default_Viewport_xv1"),QVariant(grview.xv1));
    allPrefs->setValue(QString("Default_Viewport_yv1"),QVariant(grview.yv1));
    allPrefs->setValue(QString("Default_Viewport_xv2"),QVariant(grview.xv2));
    allPrefs->setValue(QString("Default_Viewport_yv2"),QVariant(grview.yv2));
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
    //allPrefs->setValue(QString("UseNewPrintingDialog"),QVariant(use_new_print_dialog));
    allPrefs->setValue(QString("PageViewZoom"),QVariant(mainWin->sldPageZoom->value()));
    allPrefs->setValue(QString("FileCodec"),QVariant(FileCodec->name()));
    allPrefs->setValue(QString("PrintCommand"),QVariant(print_cmd));
    allPrefs->setValue(QString("DefaultFont"),QVariant(DefaultFont));
    allPrefs->setValue(QString("DefaultAgrFile"),QVariant(QString(default_grace_file)));
    allPrefs->setValue(QString("Warn_On_Encoding_Change"),QVariant(warn_on_encoding_change));
    allPrefs->setValue(QString("Sync_Page_Dimensions_On_Devices"),QVariant(GetToggleButtonState(FormDeviceSetup->dsync_item)));
    allPrefs->setValue(QString("Rescale_Plot_On_Page_Size_Change"),QVariant(GetToggleButtonState(FormDeviceSetup->psync_item)));
    allPrefs->endGroup();

    allPrefs->beginGroup(QString("Appearance"));
    allPrefs->setValue(QString("UseNewIcons"),QVariant(use_new_icons));
    QColor col=mainWin->mainArea->scroll->palette().color(QPalette::Background);
    sprintf(dummy,"%d;%d;%d",col.red(),col.green(),col.blue());
    allPrefs->setValue(QString("DrawAreaBackground"),QVariant(QString(dummy)));
    //allPrefs->setValue(QString("UniversalFontSizeFactor"),QVariant(universal_font_size_factor));//universal factor is to be written into the file not in general for the whole Application
    pat_lens=GuiFont->toString();
    allPrefs->setValue(QString("GuiFont"),QVariant(pat_lens));
    allPrefs->setValue(QString("InitialXPos"),QVariant(initial_x_pos));
    allPrefs->setValue(QString("InitialYPos"),QVariant(initial_y_pos));
    allPrefs->setValue(QString("InitialWidth"),QVariant(initial_width));
    allPrefs->setValue(QString("InitialHeight"),QVariant(initial_height));
    allPrefs->setValue(QString("StartDPI"),QVariant(start_dpi));
    allPrefs->setValue(QString("Show_Tool_Bar"),QVariant(show_ToolBar));
    allPrefs->setValue(QString("Show_Status_Bar"),QVariant(show_StatusBar));
    allPrefs->setValue(QString("Show_Locator_Bar"),QVariant(show_LocatorBar));
    allPrefs->setValue(QString("Show_Navigation_Buttons"),QVariant(show_Navi_B));
    allPrefs->setValue(QString("Show_Graph_List"),QVariant(show_Graph_List));
    allPrefs->setValue(QString("Show_Special_Zoom_Buttons"),QVariant(show_special_Zoom));
    allPrefs->setValue(QString("Show_Viewport_Stack"),QVariant(show_Viewport_Stack));
    allPrefs->setValue(QString("Show_Page_Zoom"),QVariant(show_Page_Zoom));
    allPrefs->setValue(QString("Show_Print_Button"),QVariant(show_Print_B));
    allPrefs->setValue(QString("Show_Export_Button"),QVariant(show_Export_B));
    allPrefs->setValue(QString("Show_Host_Name"),QVariant(show_host_name));
    allPrefs->setValue(QString("Show_Display_Name"),QVariant(show_display_name));
    allPrefs->setValue(QString("Show_Project_File"),QVariant(displ_project_filename));
    allPrefs->setValue(QString("Show_Export_File"),QVariant(displ_export_filename));
    allPrefs->endGroup();

//save current linestyles into tmp
copy_line_style_patterns_to_target(nr_of_current_linestyles,lenghts_of_linestyle_patterns,current_linestyle_patterns,1);
//copy linestyle settings from ini into current settings
copy_line_style_patterns_to_target(n_linestyles_ini,l_linestyle_pat_ini,linestyle_pat_ini,0);

    allPrefs->beginGroup(QString("LineStyles"));
    allPrefs->setValue(QString("NrOfLineStyles"),QVariant(nr_of_current_linestyles));
    generate_strings_for_linestyles(pat_lens,pat_styles);
    allPrefs->setValue(QString("LineStyleLenghts"),QVariant(pat_lens));
    allPrefs->setValue(QString("LineStyles"),QVariant(pat_styles));
    allPrefs->endGroup();

//copy tmp-linestyle-settings back into current
copy_line_style_patterns_to_target(n_linestyles_tmp,l_linestyle_pat_tmp,linestyle_pat_tmp,0);

    allPrefs->sync();
    delete allPrefs;
#ifdef WINDOWS_SYSTEM
    //Set the settings-file invisible
    //strcpy(dummy,path_to_write_settings().toLocal8Bit().constData());
    //SetFileAttributes(dummy,FILE_ATTRIBUTE_HIDDEN);
#endif
}

void UpdateAllWindowContents(void)//a "repaint"-funktion for all widgets
{
    QLocale newLocale=(DecimalPointToUse=='.')?(*dot_locale):(*comma_locale);
    //cout << "updating all window contents; DecSep=#" << DecimalPointToUse << "#" << endl;
    update_all();//updates everything else
    if (mainWin)
    {
        mainWin->sldPageZoom->redisplay();
    }
    if (EllipseProps)
    {
        EllipseProps->flp->ellip_linew_item->setLocale(newLocale);
        EllipseProps->flp->init(EllipseProps->flp->obj_id);
    }
    if (EditEllipseProps)
    {
        EditEllipseProps->flp->ellip_linew_item->setLocale(newLocale);
        EditEllipseProps->flp->init(EditEllipseProps->flp->obj_id);
    }
    if (BoxProps)
    {
        BoxProps->flp->ellip_linew_item->setLocale(newLocale);
        BoxProps->flp->init(BoxProps->flp->obj_id);
    }
    if (EditBoxProps)
    {
        EditBoxProps->flp->ellip_linew_item->setLocale(newLocale);
        EditBoxProps->flp->init(EditBoxProps->flp->obj_id);
    }
    if (TextProps)
    {
        //Rotation and Size are integer and don't need a decimal separator
        //Nothing to change here!
    }
    if (EditTextProps)
    {
        EditTextProps->flp->init(EditTextProps->flp->obj_id);
    }
    if (LineProps)
    {
        LineProps->flp->lines_a_dL_ff_item->spnLineWidth->setLocale(newLocale);
        LineProps->flp->lines_a_lL_ff_item->spnLineWidth->setLocale(newLocale);
        LineProps->flp->lines_width_item->spnLineWidth->setLocale(newLocale);
        LineProps->flp->lines_asize_item->spnLineWidth->setLocale(newLocale);
        LineProps->init(LineProps->flp->obj_id);
    }
    if (EditLineProps)
    {
        EditLineProps->flp->lines_a_dL_ff_item->spnLineWidth->setLocale(newLocale);
        EditLineProps->flp->lines_a_lL_ff_item->spnLineWidth->setLocale(newLocale);
        EditLineProps->flp->lines_width_item->spnLineWidth->setLocale(newLocale);
        EditLineProps->flp->lines_asize_item->spnLineWidth->setLocale(newLocale);
        EditLineProps->init(EditLineProps->flp->obj_id);
    }
    if (FormDrawObjects)
    {
        //Nothing to change here!
    }
    if (FormConsole)
    {
        //Nothing to change here!
    }
    if (FormEvalExpr)
    {
        strcpy(dummy,FormEvalExpr->ledFormula->toPlainText().toLatin1());
        RedisplayFormula(dummy);
        FormEvalExpr->ledFormula->setText(dummy);
    }
    if (FormSetAppearance)
    {
        FormSetAppearance->flp->redisplayContents();
    }
    if (FormGraphAppearance)
    {
        FormGraphAppearance->flp->redisplayContents();
    }
    if (FormPlotAppearance)
    {
        /// Encoding fuer Project description fehlt
        FormPlotAppearance->flp->ledStampCoords[0]->ReplaceNumberContents();
        FormPlotAppearance->flp->ledStampCoords[1]->ReplaceNumberContents();
    }
    if (FormLocatorProps)
    {
        FormLocatorProps->init();
    }
    if (FormAxisProperties)
    {
        FormAxisProperties->flp->redisplayContents();
    }
    if (FormPointExplorer)
    {
        FormPointExplorer->update_point_locator(FormPointExplorer->gno,FormPointExplorer->sno,FormPointExplorer->loc_pos);
    }
    if (FormNonlinCurveFit)
    {
        FormNonlinCurveFit->tabMain->ledTolerance->ReplaceNumberContents();
        FormNonlinCurveFit->tabMain->ledFormula->ReplaceNumberContents();
        FormNonlinCurveFit->tabAdvanced->ledFunction->ReplaceNumberContents();
        FormNonlinCurveFit->tabAdvanced->ledStartLoadAt->ReplaceNumberContents();
        FormNonlinCurveFit->tabAdvanced->ledStopLoadAt->ReplaceNumberContents();
        for (int i=0;i<FormNonlinCurveFit->tabMain->spreadParameters->rows;i++)
        {
            FormNonlinCurveFit->tabMain->spreadParameters->fitlines[i]->Redisplay();
        }
    }
    if (FormInterpolation)
    {
        FormInterpolation->lenStart->ReplaceNumberContents();
        FormInterpolation->lenStop->ReplaceNumberContents();
    }
    if (FormHistograms)
    {
        FormHistograms->lenStart->ReplaceNumberContents();
        FormHistograms->lenStop->ReplaceNumberContents();
    }
    if (FormSetOperations)
    {
        //Nothing to change here!
    }
    if (FormExplorer)
    {
        FormExplorer->PlotAppearance->ledStampCoords[0]->ReplaceNumberContents();
        FormExplorer->PlotAppearance->ledStampCoords[1]->ReplaceNumberContents();
        FormExplorer->GraphProperties->redisplayContents();
        FormExplorer->SetProperties->redisplayContents();
        FormExplorer->AxisProperties->redisplayContents();

        FormExplorer->LineProperties->lines_a_dL_ff_item->spnLineWidth->setLocale(newLocale);
        FormExplorer->LineProperties->lines_a_lL_ff_item->spnLineWidth->setLocale(newLocale);
        FormExplorer->LineProperties->lines_width_item->spnLineWidth->setLocale(newLocale);
        FormExplorer->LineProperties->lines_asize_item->spnLineWidth->setLocale(newLocale);

        FormExplorer->LineProperties->init(FormExplorer->LineProperties->obj_id);
        FormExplorer->TextProperties->init(FormExplorer->TextProperties->obj_id);

        FormExplorer->EllipseProperties->ellip_linew_item->setLocale(newLocale);
        FormExplorer->EllipseProperties->init(FormExplorer->EllipseProperties->obj_id);
        FormExplorer->BoxProperties->ellip_linew_item->setLocale(newLocale);
        FormExplorer->BoxProperties->init(FormExplorer->BoxProperties->obj_id);

    }
    if (FormRTIManage)
    {
        //Nothing to change here!
    }
    if (FormCommands)
    {
        /// checked, wenn eine Formel eingegeben wird, ob die dann mit '.' oder mit ',' kommt
    }
    if (FormDeviceSetup)
    {
        FormDeviceSetup->init(FormDeviceSetup->devices_item->currentValue());
    }
    if (Form_Preferences)
    {
        Form_Preferences->redisplayContents();
    }
    if (FormArrangeGraphs)
    {
        FormArrangeGraphs->selUpperOffset->setLocale(newLocale);
        FormArrangeGraphs->selUpperOffset->setValue(FormArrangeGraphs->selUpperOffset->value());
        FormArrangeGraphs->selLowerOffset->setLocale(newLocale);
        FormArrangeGraphs->selLowerOffset->setValue(FormArrangeGraphs->selLowerOffset->value());
        FormArrangeGraphs->selLeftOffset->setLocale(newLocale);
        FormArrangeGraphs->selLeftOffset->setValue(FormArrangeGraphs->selLeftOffset->value());
        FormArrangeGraphs->selRightOffset->setLocale(newLocale);
        FormArrangeGraphs->selRightOffset->setValue(FormArrangeGraphs->selRightOffset->value());
        FormArrangeGraphs->selHGap->setLocale(newLocale);
        FormArrangeGraphs->selHGap->setValue(FormArrangeGraphs->selHGap->value());
        FormArrangeGraphs->selVGap->setLocale(newLocale);
        FormArrangeGraphs->selVGap->setValue(FormArrangeGraphs->selVGap->value());
    }
    if (FormOverlayGraphs)
    {
        //Nothing to change here!
    }
    if (FormAutoScaleGraphs)
    {
        //Nothing to change here!
    }
    if (FormDataSetProperties)
    {
        /// Comment-Encoding aendern
        FormDataSetProperties->setChanged(FormDataSetProperties->sno);
    }
    if (FormSetOPS)
    {
        //Nothing to change here!
    }
    if (FormFeatureExtraction)
    {
        FormFeatureExtraction->ledValue->ReplaceNumberContents();
    }
    if (FormFourier)
    {
        //Nothing to change here!
    }
    if (FormFourier2)
    {
        FormFourier2->lenLowFrequency->ReplaceNumberContents();
        FormFourier2->lenHighFrequency->ReplaceNumberContents();
        FormFourier2->lenTimeFormula->ReplaceNumberContents();
        FormFourier2->lenRipple->ReplaceNumberContents();
    }
    if (FormLoadAndEvaluate)
    {
        FormLoadAndEvaluate->Redisplay();
    }
    if (FormIntegration)
    {
        FormIntegration->lenText->ReplaceNumberContents();
    }
    if (FormSeasonDiff)
    {
        FormSeasonDiff->lenText->ReplaceNumberContents();
    }
    if (FormDifferences)
    {
        //Nothing to change here!
    }
    if (FormAverages)
    {
        FormAverages->lenText->ReplaceNumberContents();//should be useless because only integers are used here
    }
    if (FormRegression)
    {
        FormRegression->StartStop->Redisplay();
    }
    if (FormCorrelation)
    {
        FormCorrelation->lenLag->ReplaceNumberContents();
    }
    if (FormConvolution)
    {
        //Nothing to change here!
    }
    if (FormDigitalFilter)
    {
        //Nothing to change here!
    }
    if (FormPruneData)
    {
        for (int i=0;i<7;i++)
            FormPruneData->lenText[i]->ReplaceNumberContents();
    }
    if (FormSamplePoints)
    {
        for (int i=0;i<7;i++)
            FormSamplePoints->lenText[i]->ReplaceNumberContents();
    }
    if (FormGeometricTransform)
    {
        for (int i=0;i<7;i++)
            FormGeometricTransform->lenText[i]->ReplaceNumberContents();
    }
    if (FormHotLinks)
    {
        //Nothing to change here!
    }
    if (FormRegionStatus)
    {
        //Nothing to change here!
    }
    if (FormReportRegion)
    {
        //Nothing to change here!
    }
    if (FormClearRegion)
    {
        //Nothing to change here!
    }
    if (FormDefineRegion)
    {
        //Nothing to change here!
    }
    if (FormNetCDF)
    {
        //Nothing to change here!
    }
    if (FormReadSets)
    {
        //Nothing to change here!
    }
    if (FormReadNetCDF)
    {
        //Nothing to change here!
    }
    if (FormWriteSets)
    {
        //Nothing to change here!
    }
    if (FormOpenProject)
    {
        //Nothing to change here!
    }
    if (FormSaveProject)
    {
        /// Encoding der Project description wie in plot appearance
    }
    if (FormSelectHotLink)
    {
        //Nothing to change here!
    }
    if (FormReadParameters)
    {
        //Nothing to change here!
    }
    if (FormWriteParameters)
    {
        //Nothing to change here!
    }
    if (FormSelectOutputFile)
    {
        //Nothing to change here!
    }
    if (FormReadBinary)
    {
        //Nothing to change here!
    }
    if (FormWriteBinary)
    {
        //Nothing to change here!
    }
    if (FormCSVImport)
    {
        //Nothing to change here!
    }
    if (FormBinaryImportFilter)
    {
        //Nothing to change here!
    }
    if (FormEditBlockData)
    {
        //Nothing to change here!
    }
    if (FormFontTool)
    {
        /// Encoding!!!
    }
    if (FormAbout)
    {
        //Nothing to change here!
    }
    if (FormSetEditor)
    {
        /// Kompliziertes Update wegen Grace-Befehlen --> Sonderfunktionen
        FormSetEditor->convertText(OldDecimalPoint,DecimalPointToUse);
    }
    if (FormUndoList)
    {
        //Nothing to change here!
    }
    for (int i=0;i<number_of_opened_spreadsheets;i++)
    {
        if (FormSpreadSheets[i]!=NULL)
        {
            ///The comment is not preserved! --> try to preserve this --> change encoding...
            ///latest inputs are not preserved either...
            FormSpreadSheets[i]->init(FormSpreadSheets[i]->gno,FormSpreadSheets[i]->sno);
        }
    }
    update_encoding();
    //Switching complete --> set old=new to prevent unnecessary switching
    OldDecimalPoint=DecimalPointToUse;
}

void GeneralPaste(const QMimeData * mimeData)
{
    QTemporaryFile * temp_file=new QTemporaryFile;
    QString str,str2;
    QFileInfo finfo;
    QString suffix;
    char * header_name;
    bool is_diadem_file,is_header_file,is_agr;
    int std_schema_nr;
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
cout << sl.at(i).toLocal8Bit().constData() << endl;
cout << endl;
*/
    stop_repaint=TRUE;
    set_wait_cursor();
    len=0;
    ListOfChanges.clear();
    ListOfOldStates.clear();
    if (mimeData->hasText() && !mimeData->hasUrls())//text implies ascii-data - or a header-file
    {
    /// todo: check for header file
        if (temp_file->open())
        {
            str=mimeData->text();
            commas=str.count(",");
            fullstops=str.count(".");
            if (commas>fullstops*3)
                if (QMessageBox::question(mainWin,QObject::tr("Problem during data import"),QObject::tr("Significantly more commas than fullstops found.\nUse comma as decimal separator?"),QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
                    exchange_point_comma=true;
            filename=new char[temp_file->fileName().length()+2];
            strcpy(filename,temp_file->fileName().toLocal8Bit().constData());
            ofi.open(filename);
            ofi << mimeData->text().toLocal8Bit().constData() << endl;
            ofi.close();
            autoscale_onread=AUTOSCALE_XY;
            curtype=0;//XY
            new_set_no=0;
            if (new_set_nos!=NULL)
            {
                delete[] new_set_nos;
                new_set_nos=NULL;//important!!!
            }
            prepare_strings_for_saving();
            getdata(get_cg(), filename, 0, LOAD_NXY);
            resume_strings_after_load_or_save();
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
                    setcomment(gnos[len+k],snos[len+k],str_imp.toLocal8Bit().data());
                }
                len+=new_set_no;
            }
            delete[] filename;
            filename=NULL;
        }//end file->open
        set_dirtystate();
        update_default_props();
        ///mainWin->mainArea->completeRedraw();
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
        strcpy(filename,urls.at(i).toLocalFile().toLocal8Bit().constData());
        /// Todo: check every file for beeing agr or header file
        finfo=QFileInfo(filename);
        suffix=finfo.suffix();
        is_diadem_file=false;
        header_name=NULL;
//at first: test for diadem-format and binary data
        is_diadem_file=is_diadem(filename,&header_name);
        is_agr=(bool)is_agr_file(filename);

        if (is_diadem_file==false && header_name!=NULL) delete[] header_name;

        if (is_agr==true)//QString::compare(suffix,QString("agr"),Qt::CaseInsensitive)==0)//it is an .agr-file
        {
        Form_AgrInfo->init(filename);
        /*Form_AgrInfo->show();
        Form_AgrInfo->raise();
        Form_AgrInfo->activateWindow();*/
        Form_AgrInfo->exec();
        continue;
        /*
        ret=QMessageBox::question(mainWin,QObject::tr("Load or import agr-file?"),QObject::tr("agr-file detected.\nDo you want to load it as a regular Grace-file (Ok)\nor import the data-sets only (Cancel)?"),QMessageBox::Ok|QMessageBox::Cancel);
            if (ret==QMessageBox::Ok)
            {
            load_project_file(filename,FALSE);
            continue;//next url/file
            }
            else
            {
            read_datasets_from_agr(filename,get_cg());
            mainWin->autoscale_proc(AUTOSCALE_XY);
            continue;
            }
        */
        //cout << "file=#" << filename << "# ret=" << ret << " --> treat it as data stream" << endl;
        }
        else if (is_diadem_file==true)//special case: diadem file
        {
        struct DIAdem_Header dia_header_infos;
        struct importSettings bin_import;
        initSettings(bin_import,false);
        cout << "DIADEM recognized" << endl;
        read_DIAdem_header(header_name,dia_header_infos);//read the diadem header
        bin_import.HeaderFile=QString(header_name);

        int nr_of_new_sets=0;
        int * n_snos=NULL,*n_gnos=NULL;
        postprocess_diadem_header(dia_header_infos,bin_import,nr_of_new_sets,&n_gnos,&n_snos);//convert the diadem import-settings to the usual binary-import-settings and read the data!

        ///Undo-Stuff
        SaveSetStatesPrevious(0,n_gnos,n_snos,UNDO_COMPLETE);//all sets will be new --> no previous sets --> 0
        SetsModified(nr_of_new_sets,n_gnos,n_snos,UNDO_COMPLETE);
            if (nr_of_new_sets==1)
            {
            sprintf(dummy,"[G%d.S%d]",n_gnos[0],n_snos[0]);
            addAditionalDescriptionToLastNode(-1,QObject::tr("Binary data import --> ")+QString(dummy),QString(),0);
            }
            else
            {
            addAditionalDescriptionToLastNode(-1,QObject::tr("Binary data import"),QString(),0);
            }

        mainWin->doAutoScale();

        if (n_snos!=NULL) delete[] n_snos;
        if (n_gnos!=NULL) delete[] n_gnos;
        if (header_name!=NULL) delete[] header_name;
        continue;//next url/file
        }
        else if (guess_bin_format(filename,std_schema_nr,is_header_file)==RETURN_SUCCESS)//look for a std binary format
        {
        copy_std_settings_to_current_bin_import(filename,std_schema_nr,is_header_file);
            if (i==urls.length()-1)//last file --> allow autoscale, but no replot (stop_repaint=TRUE; prevents replot)
            {
            //read_std_bin_file(filename,std_schema_nr,is_header_file,false);
            read_bin_file_by_current_settings(false);
            }
            else//no autoscale, no replot
            {
            //read_std_bin_file(filename,std_schema_nr,is_header_file,true);
            read_bin_file_by_current_settings(true);
            }
        continue;//next url/file
        }
        //normal file (unrecognized)
            FILE * fn=fopen(filename,"r");
            QTextStream * ts=new QTextStream(fn);
            str=ts->readAll();
            str2=QString(str.toLocal8Bit());
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
                prepare_strings_for_saving();
                getdata(get_cg(), filename, 0, LOAD_NXY);
                resume_strings_after_load_or_save();
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
                        setcomment(gnos[len+k],snos[len+k],str_imp.toLocal8Bit().data());
                    }
                    len+=new_set_no;
                }
            }//end of simple ascii import
            delete[] filename;
            filename=NULL;
        }//end of loop trough all files
        set_dirtystate();
        update_default_props();
        ///mainWin->mainArea->completeRedraw();
    }
    else
    {
        errwin(QObject::tr("Data format in file drop not recognized!").toLocal8Bit().constData());
    /*
    cout << "unknown datatype!" << endl;
    QStringList sl=mimeData->formats();
    for (int i=0;i<sl.length();i++)
    cout << sl.at(i).toLocal8Bit().constData() << endl;
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
    stop_repaint=FALSE;
    mainWin->mainArea->completeRedraw();
    exchange_point_comma=false;
}

void change_font_at_location(int * font)
{
    int mapped=(*font);
    if (mapped!=(*font))
    {
        *font=mapped;
    }
}

void set_font_ordering_to_default(void)//call this after loading and after update_fonts_to_default_ordering() to remove the font-map
{
    map_fonts(FONT_MAP_DEFAULT);
}

void update_fonts_to_default_ordering(void)//call this after loading to remove the mapping of fonts (change fonts from maps to default ordering)
{
    int equ=font_map_equals_font_ids();
    if (equ==1)
    {
        //cout << "Font-map == Font ids" << endl;
        return;//nothing to do here!
    }
    /*else
{
cout << "Font-map != Font ids" << endl;
}*/
    int save_use_qt_fonts=useQtFonts;
    useQtFonts=false;//we have to operate on the original fonts here -- do not use qtFonts here
    for (int i=0;i<number_of_graphs();i++)
    {
        change_font_at_location(&(g[i].labs.title.font));
        change_font_at_location(&(g[i].labs.stitle.font));
        change_font_at_location(&(g[i].l.font));
        for (int j=0;j<MAXAXES;j++)
        {
            change_font_at_location(&(g[i].t[j]->tl_font));
            change_font_at_location(&(g[i].t[j]->label.font));
            for (int k=0;k<MAX_TICKS;k++)
            {
                ;//g[i].t[j]->tloc[k].label = copy_string(g[i].t[j]->tloc[k].label, g[i].t[j]->tloc[k].orig_label);
            }
        }
        for (int j=0;j<g[i].maxplot;j++)
        {
            if (is_set_active(i,j)==true)
            {
                change_font_at_location(&(g[i].p[j].charfont));
                change_font_at_location(&(g[i].p[j].avalue.font));
            }
        }
    }
    for (int i=0;i<maxstr;i++)
    {
        change_font_at_location(&(pstr[i].font));
    }
    set_font_ordering_to_default();
    useQtFonts=save_use_qt_fonts;
}

//do not use this function ('copy_LaTeX_to_Grace();') alone!
//before saving: call 'this' and 'convert_all_strings_from_UTF8_to_encoding();' afterwards! Then save!
//the Grace-strings are in the save/load-state afterwards --> this means that they contain the original text in the user selected encoding
//after loading or before continuing the work after saving do:
//'convert_all_strings_from_encoding_to_UTF8();'
//'copy_Grace_to_LaTeX();' --> remember: this is no backward-converter that generates LaTeX-commands, this just copies the originals from the Grace-strings to the original locations and prepares the Grace-strings for display
void copy_LaTeX_to_Grace(void)//copy the original LaTeX-texts into the Grace-variables to be saved later (please remember: the originals are always in UTF-8, but the Grace-texts are usually in isoLatin1 or in the user selected codec for saving)
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

void convert_single_string_from_encoding_to_UTF8(char ** text)
{
    static QString temp;
    temp=FileCodec->toUnicode(*text);//convert text from encoding to unicode
    *text = copy_string(*text,temp.toUtf8().constData());//convert QString in UTF-8 to C-String in UTF-8
}

void convert_single_string_from_encoding_to_UTF8_static(char * text)
{
    static QString temp;
    temp=FileCodec->toUnicode(text);//convert text from encoding to unicode
    strcpy(text,temp.toUtf8().constData());//convert QString in UTF-8 to C-String in UTF-8
}

void convert_single_string_from_UTF8_to_encoding(char ** text)
{
    static QString temp;
    temp=QString::fromUtf8(*text);//convert C-string in UTF-8 encoding in a QString in UTF-8
    *text = copy_string(*text,FileCodec->fromUnicode(temp).constData());//convert to encoding
}

void convert_single_string_from_UTF8_to_encoding_static(char * text)
{
    static QString temp;
    temp=QString::fromUtf8(text);//convert C-string in UTF-8 encoding in a QString in UTF-8
    strcpy(text,FileCodec->fromUnicode(temp).constData());//convert to encoding
}

//do this after loading
void convert_all_strings_from_encoding_to_UTF8(void)//translation from user selected file encoding into UTF-8 (usually only after load- or save-operations)
{//this funtion only affects the original Grace-text-string storage
    for (int i=0;i<number_of_graphs();i++)
    {
        convert_single_string_from_encoding_to_UTF8(&g[i].labs.title.s_plotstring);
        convert_single_string_from_encoding_to_UTF8(&g[i].labs.stitle.s_plotstring);
        for (int j=0;j<MAXAXES;j++)
        {
            convert_single_string_from_encoding_to_UTF8_static(g[i].t[j]->tl_appstr);
            convert_single_string_from_encoding_to_UTF8_static(g[i].t[j]->tl_prestr);
            convert_single_string_from_encoding_to_UTF8(&g[i].t[j]->label.s_plotstring);
            for (int k=0;k<MAX_TICKS;k++)
            {
                convert_single_string_from_encoding_to_UTF8(&g[i].t[j]->tloc[k].label);
            }
        }
        for (int j=0;j<g[i].maxplot;j++)
        {
            if (is_set_active(i,j)==true)
            {
                convert_single_string_from_encoding_to_UTF8_static(g[i].p[j].comments);
                convert_single_string_from_encoding_to_UTF8_static(g[i].p[j].lstr);
                convert_single_string_from_encoding_to_UTF8_static(g[i].p[j].avalue.prestr);
                convert_single_string_from_encoding_to_UTF8_static(g[i].p[j].avalue.appstr);
            }
        }
    }
    for (int i=0;i<maxstr;i++)
    {
        convert_single_string_from_encoding_to_UTF8(&pstr[i].s_plotstring);
    }
}

//do this before saving
void convert_all_strings_from_UTF8_to_encoding(void)//translation from UTF-8 into user selected file encoding (usually only before save-operations)
{//this funtion only affects the original Grace-text-string storage
    for (int i=0;i<number_of_graphs();i++)
    {
        convert_single_string_from_UTF8_to_encoding(&g[i].labs.title.s_plotstring);
        convert_single_string_from_UTF8_to_encoding(&g[i].labs.stitle.s_plotstring);
        for (int j=0;j<MAXAXES;j++)
        {
            convert_single_string_from_UTF8_to_encoding_static(g[i].t[j]->tl_appstr);
            convert_single_string_from_UTF8_to_encoding_static(g[i].t[j]->tl_prestr);
            convert_single_string_from_UTF8_to_encoding(&g[i].t[j]->label.s_plotstring);
            for (int k=0;k<MAX_TICKS;k++)
            {
                convert_single_string_from_UTF8_to_encoding(&g[i].t[j]->tloc[k].label);
            }
        }
        for (int j=0;j<g[i].maxplot;j++)
        {
            if (is_set_active(i,j)==true)
            {
                convert_single_string_from_UTF8_to_encoding_static(g[i].p[j].comments);
                convert_single_string_from_UTF8_to_encoding_static(g[i].p[j].lstr);
                convert_single_string_from_UTF8_to_encoding_static(g[i].p[j].avalue.prestr);
                convert_single_string_from_UTF8_to_encoding_static(g[i].p[j].avalue.appstr);
            }
        }
    }
    for (int i=0;i<maxstr;i++)
    {
        convert_single_string_from_UTF8_to_encoding(&pstr[i].s_plotstring);
    }
}

//text --> string (warning: this function assumes string to be large! --> not to use with dynamic memory)
void generate_string_Qt_aware(char * string,QString text)//converts the text (assumed to be UTF-8) into a C-string in UTF-8 if QtFonts are to be used and a C-string in isoLatin1 if no QtFonts are to be used
{
    if (useQtFonts==true)
        strcpy(string,text.toUtf8().constData());
    else
        strcpy(string,text.toLatin1().constData());//Latin1 is the standard-codec for Grace-strings (T1-encoding)
}

void update_grace_string_from_UTF8(char ** string)//assumes string to be a dynamic c-string in UTF-8-format; we do LaTeX-conversions and convert to UTF-8(if QtFonts==true) or isolatin1
{
    static QString temp;
    static char temp2[MAX_STRING_LENGTH];
    temp=QString::fromUtf8(*string);//generate a QString from original c-string in UTF-8
    if (activateLaTeXsupport==true)
        complete_LaTeX_to_Grace_Translator(temp);//do LaTeX-conversion (if needed)
    generate_string_Qt_aware(temp2,temp);//generate a suitable C-string from the QString
    *string = copy_string(*string, temp2);//copy converted sting to Grace-string-location
}

void update_grace_string_from_UTF8_static(char * string)//same as above, but with static memory
{
    static QString temp;
    static char temp2[MAX_STRING_LENGTH];
    temp=QString::fromUtf8(string);//generate a QString from original c-string in UTF-8
    if (activateLaTeXsupport==true)
        complete_LaTeX_to_Grace_Translator(temp);//do LaTeX-conversion (if needed)
    generate_string_Qt_aware(temp2,temp);//generate a suitable C-string from the QString
    strcpy(string, temp2);//copy converted sting to Grace-string-location
}

void convert_Grace_Strings(void)
{
    for (int i=0;i<number_of_graphs();i++)
    {
        //cout << "Label s=" << g[i].labs.title.s << endl;
        update_grace_string_from_UTF8(&g[i].labs.title.s_plotstring);
        /// temp=QString::fromUtf8(g[i].labs.title.s);//generate a QString from original c-string in UTF-8
        ///     if (activateLaTeXsupport==true)
        ///     complete_LaTeX_to_Grace_Translator(temp);//do LaTeX-conversion (if needed)
        /// generate_string_Qt_aware(temp2,temp);//generate a suitable C-string from the QString
        /// g[i].labs.title.s = copy_string(g[i].labs.title.s, temp2);//copy converted sting to Grace-string-location
        update_grace_string_from_UTF8(&g[i].labs.stitle.s_plotstring);
        /// temp=QString(g[i].labs.stitle.s);
        ///     if (activateLaTeXsupport==true)
        ///     complete_LaTeX_to_Grace_Translator(temp);
        /// g[i].labs.stitle.s = copy_string(g[i].labs.stitle.s, temp.toLocal8Bit().constData());
        for (int j=0;j<MAXAXES;j++)
        {
            update_grace_string_from_UTF8_static(g[i].t[j]->tl_appstr);
            /// temp=QString(g[i].t[j]->tl_appstr);
            ///     if (activateLaTeXsupport==true)
            ///     complete_LaTeX_to_Grace_Translator(temp);
            /// strcpy(g[i].t[j]->tl_appstr, temp.toLocal8Bit().constData());
            update_grace_string_from_UTF8_static(g[i].t[j]->tl_prestr);
            /// temp=QString(g[i].t[j]->tl_prestr);
            ///     if (activateLaTeXsupport==true)
            ///     complete_LaTeX_to_Grace_Translator(temp);
            /// strcpy(g[i].t[j]->tl_prestr, temp.toLocal8Bit().constData());
            update_grace_string_from_UTF8(&g[i].t[j]->label.s_plotstring);
            /// temp=QString(g[i].t[j]->label.s);
            ///     if (activateLaTeXsupport==true)
            ///     complete_LaTeX_to_Grace_Translator(temp);
            /// g[i].t[j]->label.s = copy_string(g[i].t[j]->label.s, temp.toLocal8Bit().constData());
            for (int k=0;k<MAX_TICKS;k++)
            {
                update_grace_string_from_UTF8(&g[i].t[j]->tloc[k].label);
                ///     temp=QString(g[i].t[j]->tloc[k].label);
                ///         if (activateLaTeXsupport==true)
                ///         complete_LaTeX_to_Grace_Translator(temp);
                ///     g[i].t[j]->tloc[k].label = copy_string(g[i].t[j]->tloc[k].label, temp.toLocal8Bit().constData());
            }
        }
        for (int j=0;j<g[i].maxplot;j++)
        {
            if (is_set_active(i,j)==true)
            {
                update_grace_string_from_UTF8_static(g[i].p[j].comments);
                /// temp=QString(g[i].p[j].comments);
                ///     if (activateLaTeXsupport==true)
                ///     complete_LaTeX_to_Grace_Translator(temp);
                /// strcpy(g[i].p[j].comments, temp.toLocal8Bit().constData());
                update_grace_string_from_UTF8_static(g[i].p[j].lstr);
                /// temp=QString(g[i].p[j].lstr);
                ///     if (activateLaTeXsupport==true)
                ///     complete_LaTeX_to_Grace_Translator(temp);
                /// strcpy(g[i].p[j].lstr, temp.toLocal8Bit().constData());
                update_grace_string_from_UTF8_static(g[i].p[j].avalue.prestr);
                /// temp=QString(g[i].p[j].avalue.prestr);
                ///     if (activateLaTeXsupport==true)
                ///     complete_LaTeX_to_Grace_Translator(temp);
                /// strcpy(g[i].p[j].avalue.prestr, temp.toLocal8Bit().constData());
                update_grace_string_from_UTF8_static(g[i].p[j].avalue.appstr);
                /// temp=QString(g[i].p[j].avalue.appstr);
                ///     if (activateLaTeXsupport==true)
                ///     complete_LaTeX_to_Grace_Translator(temp);
                /// strcpy(g[i].p[j].avalue.appstr, temp.toLocal8Bit().constData());
            }
        }
    }
    for (int i=0;i<maxstr;i++)
    {
        update_grace_string_from_UTF8(&pstr[i].s_plotstring);
        /// temp=QString(pstr[i].s);
        ///     if (activateLaTeXsupport==true)
        ///     complete_LaTeX_to_Grace_Translator(temp);
        /// pstr[i].s = copy_string(pstr[i].s, temp.toLocal8Bit().constData());
    }
}

void copy_Grace_to_LaTeX(void)//copy the LaTeX-texts from the Grace-variables into the save-storage and convert the original (after loading, before showing)
{
    //variables ending in 'alt' or beginning with 'orig' are the originals in UTF-8 format (always UTF-8 / always original user input without LaTeX-conversion)
    //in the original grace-strings (like title.s or .label) text has to be in isoLatin1 (or UTF-8 if QtFonts are to be used)
    //important for the usage of this function: when calling this function it is assumed, that all Grace-strings are filled with the original text in UTF-8-encoding (for example after loading)
    //before: s=UTF-8, alt=nothing; after: s=isoLatin1/UTF-8(if QtFonts), alt=UTF-8

    //copy --> no conversions here, because UTF-8 allround, alt is ok after this copy operation (no further operations needed, alt always stores the original in UTF-8)
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
    //convert --> change s to isoLatin1 if not QtFonts, otherwise keep s in UTF-8; if LaTeX: use Translator on original s in UTF-8-format first!
    convert_Grace_Strings();
    //conversion finished
}

QString generateDisplayStringFromGraceString(char * gracestring)//to diplay the internally used Grace string in a QLineEdit-Widget
{
    static QString st;
    if (useQtFonts==true)//the c-string 'gracestring' is in UTF8-codec --> it should be simple
        st=QString::fromUtf8(gracestring);
    else//c-string is in isoLatin1-codec --> different conversion
        st=QString::fromLatin1(gracestring);
    return st;
}

void direct_single_string_postprocessing_after_loading(char ** s,char ** alt,bool dynamic)//does the complete postprocessing operation of a single string after loading operations (intended for use inside the parser!)
{//we assume s is the original in the user selected encoding, after this function: alt contains the original text in UTF-8-encoding and s the converted version (including LaTeX-processing) in the appropriate encoding (isoLatin1 without QtFonts/UTF-8 with QtFonts)
    if (dynamic==true)
    {
        convert_single_string_from_encoding_to_UTF8(s);//convert from user selected encoding to UTF-8
        *alt = copy_string(*alt,*s);//copy original string in UTF-8 to save location (alt)
        update_grace_string_from_UTF8(s);//convert Grace-string into displayable format
    }
    else
    {
        convert_single_string_from_encoding_to_UTF8_static(*s);
        strcpy(*alt,*s);
        update_grace_string_from_UTF8_static(*s);
    }
}

void replace_found_ids_with_new_ones(int & found_o_sets,int ** found_o_gnos,int ** found_o_snos,int & found_n_sets,int ** found_n_gnos,int ** found_n_snos)
{
    if (activate_id_replacing==true)//replace set ids
    {
        found_o_sets=replace_o_set_ids;
        if ((*found_o_gnos)!=NULL)
            delete[] (*found_o_gnos);
        if ((*found_o_snos)!=NULL)
            delete[] (*found_o_snos);
        (*found_o_gnos)=new int[found_o_sets+1];
        (*found_o_snos)=new int[found_o_sets+1];
        for (int ii=0;ii<found_o_sets;ii++)
        {
            (*found_o_gnos)[ii]=replace_o_gnos[ii];
            (*found_o_snos)[ii]=replace_o_snos[ii];
        }
        found_n_sets=replace_n_set_ids;
        if ((*found_n_gnos)!=NULL)
            delete[] (*found_n_gnos);
        if ((*found_n_snos)!=NULL)
            delete[] (*found_n_snos);
        (*found_n_gnos)=new int[found_n_sets+1];
        (*found_n_snos)=new int[found_n_sets+1];
        for (int ii=0;ii<found_n_sets;ii++)
        {
            (*found_n_gnos)[ii]=replace_n_gnos[ii];
            (*found_n_snos)[ii]=replace_n_snos[ii];
        }
    }
    //replace '-1' with new sets
    for (int ii=0;ii<found_n_sets;ii++)
    {
        if ((*found_n_snos)[ii] == -1) //no real set-id set --> get new one
        {
            (*found_n_snos)[ii]=nextset((*found_n_gnos)[ii]);
            if ( setlength((*found_n_gnos)[ii],(*found_n_snos)[ii],getsetlength((*found_o_gnos)[ii],(*found_o_snos)[ii])) == RETURN_FAILURE ) errwin(QObject::tr("Unable to allocate new set!").toLatin1().constData());
            /// TODO: SAVE NEW SET-ID FOR UNDO-OPERATION
        }
        (*found_n_gnos)[ii]=replace_n_gnos[ii];
        (*found_n_snos)[ii]=replace_n_snos[ii];
    }
}

int qtspecial_scanner(char * command,int * errors)
{
    static int len,len2,pos;
    len=strlen(command);
    if (len<16) return RETURN_FAILURE;//can not be a special command
    char * parameters;
    char formula_arg[MAX_STRING_LENGTH];
    int retval=containsSpecialCommand(command,&parameters);
    int retval2,nr_of_replacements=0,eq_pos,extract_err;
    QString replayed_command;

//cout << "New command: vorher: command=" << command << endl;
    replayed_command=QString(command);
    nr_of_replacements=process_command_for_new_sets(replayed_command,current_target_graph);
    if (nr_of_replacements>0)//we made replacements-->send it back to the scanner
    {
    //strcpy(command,replayed_command.toLatin1().constData());
    scanner(replayed_command.toLatin1().data());
    return RETURN_SUCCESS;//we report that everything is alright --> no further parsing needed
    }
    //else: no replacements --> command is ok as it is

//cout << "New command: nachher: command=" << command << endl;

    /// cout << "special scanner=" << command << " retval=" << retval << endl;
    if (retval==SPECIAL_NONE) return RETURN_FAILURE;

    char temp_val[MAX_STRING_LENGTH];

    int found_o_sets,found_n_sets;
    int * found_o_gnos=NULL;
    int * found_o_snos=NULL;
    int * found_n_gnos=NULL;
    int * found_n_snos=NULL;
    int type,realization,absolute,debug,point_extension,oversampling,rno,invr;
    double limits[2];
    int orders[3];
    char x_formula[MAX_STRING_LENGTH];
    double ripple;

    replayed_command.clear();

    switch (retval)
    {
    case SPECIAL_FILTER:
        ParseFilterCommand(parameters,found_o_sets,&found_o_gnos,&found_o_snos,found_n_sets,&found_n_gnos,&found_n_snos,type,realization,limits,orders,x_formula,ripple,absolute,debug,point_extension,oversampling,rno,invr);
        replace_found_ids_with_new_ones(found_o_sets,&found_o_gnos,&found_o_snos,found_n_sets,&found_n_gnos,&found_n_snos);//does the set-ID replacing if necessary

        for (int ii=0;ii<found_o_sets;ii++)
        {
            retval2=do_filter_on_one_set(found_n_gnos[ii],found_n_snos[ii],found_o_gnos[ii],found_o_snos[ii],type,realization,rno,invr,absolute,debug,x_formula,point_extension,oversampling,ripple,orders[0],orders[1],limits[0],limits[1]);
            if (retval2!=RETURN_SUCCESS) (*errors)++;
        }

        break;
    case SPECIAL_REGRESSION:
        ParseRegression(parameters,found_o_sets,&found_o_gnos,&found_o_snos,found_n_sets,&found_n_gnos,&found_n_snos,type,realization,rno,invr,limits[0],limits[1],point_extension,oversampling,x_formula);
        replace_found_ids_with_new_ones(found_o_sets,&found_o_gnos,&found_o_snos,found_n_sets,&found_n_gnos,&found_n_snos);

        for (int ii=0;ii<found_o_sets;ii++)
        {
            if (is_set_active(found_o_gnos[ii],found_o_snos[ii])==TRUE)
            {//set exists --> do regression
                if( oversampling == 2 )//rx
                {
                    if (generate_x_mesh_from_formula(found_n_gnos[ii],found_n_snos[ii],limits[0],limits[1],point_extension,x_formula,SET_XY)==RETURN_FAILURE)
                    {
                        errwin(QObject::tr("Not enough sets").toLocal8Bit().constData());
                        return RETURN_FAILURE;
                    }
                }
                do_regress(found_o_gnos[ii],found_o_snos[ii],type,realization,rno,invr,found_n_snos[ii]);
            }
        }

        break;
    case SPECIAL_USE:
        if (strcmp(parameters,"LAST_FORMULA")==0)
        {
            replayed_command=QString(last_formula);
        }
        else//Remembered Formula
        {
            replayed_command=QString(saved_formula);
        }
        break;
    case SPECIAL_REMEMBER:
        if (strcmp(parameters,"Formula")==0)
        {
            strcpy(saved_formula,last_formula);
        }
        else if (parameters[4]=='a')
        {
            pos=atoi(parameters+5);
                if (pos>=0 && pos<MAXPARM)
                saved_value=last_fit_falues[pos];
        }
        else//special case
        {
            ;//UNUSED YET
        }
        break;
    case SPECIAL_ADD:
    case SPECIAL_MINUS:
    case SPECIAL_DIVIDE:
    case SPECIAL_MULTIPLY:

        if (activate_id_replacing==false)
            replayed_command=QString("Y=Y");
        else
            replayed_command=QString("G0.S0.Y=G0.S0.Y");//will be replaced anyway!

        if (retval==SPECIAL_ADD)
        {
            replayed_command+=QString("+(");
        }
        else if (retval==SPECIAL_MINUS)
        {
            replayed_command+=QString("-(");
        }
        else if (retval==SPECIAL_MULTIPLY)
        {
            replayed_command+=QString("*(");
        }
        else if (retval==SPECIAL_DIVIDE)
        {
            replayed_command+=QString("/(");
        }
        if (strcmp(parameters,"LAST_VALUE")==0)
        {
            sprintf(temp_val,"%g",saved_value);
            replayed_command+=QString(temp_val);
        }
        else if (strcmp(parameters,"LAST_FORMULA")==0)
        {
            len2=strlen(last_formula);
            pos=0;
            for (int i=0;i<len2;i++){if (last_formula[i]=='='){pos=i+1; break;}}
            replayed_command+=QString(last_formula+pos);
        }
        else//SAVED_FORMULA
        {
            len2=strlen(saved_formula);
            pos=0;
            for (int i=0;i<len2;i++){if (saved_formula[i]=='='){pos=i+1; break;}}
            replayed_command+=QString(saved_formula+pos);
        }
        replayed_command+=QString(")");
        break;
    case SPECIAL_EXTRACT:
        eq_pos=-1;
            for (unsigned int i=0;i<strlen(parameters);i++)
            {
                if (parameters[i]=='=')
                {
                eq_pos=i;
                break;
                }
            }
            orders[0]=0;/// Warning! This is wrong!
            orders[1]=0;
            if (eq_pos<0)
            {
            retval2=ParseExtractCommand(parameters,formula_arg);
            }
            else
            {
            retval2=ParseExtractCommand(parameters+eq_pos+1,formula_arg);
            }
                if (retval2==4)//median -- special traetment because I extended the possible usage of the median (all columns, not only X or Y)
                {
                    orders[0]=-1;//Warning! Invalid ids!
                    orders[1]=-1;
                    orders[2]=-1;
                getSetIDFromText(formula_arg,orders[0],orders[1],orders[2]);
                getmedian(orders[0],orders[1],orders[2],limits);
                cout << "read set-id: G" << orders[0] << ".S" << orders[1] << "." << orders[2] << endl;
                cout << "median=" << limits[0] << endl;
                }
                else
                {
                extract_err=extract_single_feature(retval2,orders[0],orders[1],limits);
                cout << "read_value=" << limits[0] << endl;
                }
        cout << "parameters=" << parameters << " retval2=" << retval2 << " Formula_arg=" << formula_arg << " result=" << limits[0] << endl;

        break;
    case SPECIAL_FORMULA:
        retval2=ParseSpecialFormula(parameters,formula_arg);

        break;
    }
    if (replayed_command.isEmpty()==false)//there is a command left -- we have to do this for all sets if replacement is wanted/activated
    {
        if (activate_id_replacing==true)
        {
            for (int ii=0;ii<replace_o_set_ids;ii++)
            {
                replace_set_ids_in_command(replayed_command,replace_o_gnos[ii],replace_o_snos[ii],replace_n_gnos[ii],replace_n_snos[ii],0);
                strcpy(temp_val,replayed_command.toLatin1().constData());
                //cout << "new formula=#" << temp_val << "#" << endl;
                retval2=scanner(temp_val);
                if (retval2!=RETURN_SUCCESS) (*errors)++;
            }
        }
        else//execute the commands just as they appear
        {
            strcpy(temp_val,replayed_command.toLatin1().constData());
            //cout << "new formula=#" << temp_val << "#" << endl;
            retval2=scanner(temp_val);
            if (retval2!=RETURN_SUCCESS) (*errors)++;
        }
        return RETURN_SUCCESS;
    }
    else
        return RETURN_SUCCESS;
}

void replace_set_ids_in_command(QString & commandString,int o_gno,int o_sno,int n_gno,int n_sno,int relative)//only replace-operations, no command execution; replaces ALL set-IDs with new ones, but possibly different IDs for source and destination
{
    static bool containsEquals;
    static QString lhs,rhs,replacedString;
    static int nr;
    char * ts=new char[2+2*commandString.length()];
    FoundSetID * foundIDs=NULL;
    strcpy(ts,commandString.toLatin1().constData());//convert QString into C-String
    find_set_ids(ts,&nr,&foundIDs);//look for set-ids
    containsEquals=commandString.contains(QString("="));//look for a '='
    if (containsEquals)//we have to split the command into two sections before further processing
    {
        nr=commandString.indexOf(QString("="));
        lhs=commandString.left(nr);//separate the command into left hand side and right hand side of '='
        rhs=commandString.mid(nr+1);
        //now we replace the set-ids and recreate the command
        //generate a C-String for the left hand side and look for set ids (should only be one...)
        strcpy(ts,lhs.toLatin1().constData());
        find_set_ids(ts,&nr,&foundIDs);
        for (int k=0;k<nr;k++)//set the 'new' target-set-ids according to the destination sets
        {
            foundIDs[k].repl_gno=n_gno;
            foundIDs[k].repl_sno=n_sno;
            /// Warning: relative not recognized yet
        }
        replacedString=ReplaceSetIds(ts,nr,foundIDs);//do the replacing
        commandString=replacedString+QString("=");//recreate left hand side of original command with new set ids
        //do the same with the right hand side
        strcpy(ts,rhs.toLatin1().constData());
        find_set_ids(ts,&nr,&foundIDs);
        for (int k=0;k<nr;k++)//set the 'new' set-ids according to the source sets
        {
            foundIDs[k].repl_gno=o_gno;
            foundIDs[k].repl_sno=o_sno;
        }
        replacedString=ReplaceSetIds(ts,nr,foundIDs);//do the replacing
        commandString+=replacedString;//put the right hand side after the left hand side
    }//end contains(=)
    else if (nr>0)//a command without a '=' but at least 1 set-id
    {
        for (int k=0;k<nr;k++)//set the 'new' set-ids according to the source sets
        {
            foundIDs[k].repl_gno=o_gno;
            foundIDs[k].repl_sno=o_sno;
        }
        replacedString=ReplaceSetIds(ts,nr,foundIDs);//do the replacing
        commandString=replacedString;
    }
    else//no complete set-id and no '='
    {
        find_graph_ids(ts,&nr,&foundIDs);//search for sinuglar graph-ids
        if (nr>0)//we found something
        {
            for (int i=0;i<nr;i++) foundIDs[i].repl_gno=n_gno;
            replacedString=ReplaceGraphIds(ts,nr,foundIDs);//do the replacing
            commandString=replacedString;
        }
    }

}

void prepare_strings_for_saving(void)
{
    strcpy(new_encoding_name,FileCodec->name().constData());
    copy_LaTeX_to_Grace();
    convert_all_strings_from_UTF8_to_encoding();
    /// Save old FileCodec just in case it is overwritten
    FileCodecSave=FileCodec;
}

void resume_strings_after_load_or_save(void)
{
    QString error_text;
    if (FileCodec==NULL)//Codec unavailaable
    {
        error_text=QObject::tr("Text-Codec UNAVAILABLE: ")+QString(new_encoding_name);
        errwin(error_text.toLatin1().constData());
        //cout << "codec in file UNAVAILABLE" << endl;
        FileCodec=FileCodecSave;
        error_text=FileCodec->name()+QObject::tr(" used as Text-Codec");
        errwin(error_text.toLatin1().constData());
    }
    else if (FileCodec!=FileCodecSave && warn_on_encoding_change==true)
    {
        error_text=QObject::tr("[Information] Changing Text-Codec: ") + FileCodecSave->name().constData() + QString(" --> ") + FileCodec->name();
        stufftext(error_text.toLatin1().constData());
        //cout << "codec in file=" << FileCodec->name().constData() << endl;
        //cout << "set codec    =" << FileCodecSave->name().constData() << endl;
    }
    convert_all_strings_from_encoding_to_UTF8();
    /// Restore the codec originally set
    FileCodec=FileCodecSave;
    copy_Grace_to_LaTeX();
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


