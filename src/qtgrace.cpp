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

#include "MainWindow.h"
#include <QtGui>
#include <QTranslator>
#include <QSvgGenerator>
#include <QSvgRenderer>
#include <QStyleFactory>
#include "globals.h"
#include "patterns.h"
#include "allWidgets.h"
#include "editWidgets.h"
#include "appearanceWidgets.h"
#include "windowWidgets.h"
#include "draw.h"
#include "xprotos.h"
#include "undo_module.h"
#include "device.h"
#include "compressdecompress.h"

#ifdef WINDOWS_SYSTEM
#include <Windows.h>
#include <direct.h>
#else
#include <unistd.h>
#endif

#ifdef _MSC_VER
#define chdir _chdir
#define getcwd _getcwd
#endif

extern int replacement_main(int argc,char **argv);

char BI_VERSION[128];
char BI_SYSTEM[256];
char SystemsDecimalPoint='.';//the default decimal-separator
char OldDecimalPoint='.';//the setting of the decimal point before a change (this is necessary to recognize a real(!) change in the decimal separator)
char DecimalPointToUse='.';//what the user wants to use as decimal separator

DrawProps draw_props = {{1, 1, 255}, 0, 255, TRUE, 1, 0.0, LINECAP_BUTT, LINEJOIN_MITER, 1.0, 0, FILLRULE_WINDING};
VPoint global_bb_ll={-1.0,-1.0};
VPoint global_bb_ur={-1.0,-1.0};
imageinfo bg_fill_image;//for the plot
imageinfo bg_box_fill_image;//default for boxes
imageinfo bg_ellipse_fill_image;//default for ellipses
int maxgraph;
unsigned int unicode_greek_shift=0;
QMap<unsigned char,ushort> key_for_greek;
bool useQtFonts=false;
bool symbol_font_is_special=true;
int DefaultFont=0;//the default ist Times Roman - or the first font in the list
int border_percent=5;
//the defaultFontNames and the defaultFontList always match!
int nr_of_default_fonts=14;
const char * defaultFontNames[14]={"Times-Roman","Times-Italic","Times-Bold","Times-BoldItalic","Helvetica","Helvetica-Oblique","Helvetica-Bold","Helvetica-BoldOblique","Courier","Courier-Oblique","Courier-Bold","Courier-BoldOblique","Symbol","ZapfDingbats"};
QList<QFont> stdFontList,defaultFontList;
//QFont * stdFont;
QFont * GuiFont, * stdGuiFont;
QFontMetrics * stdFontMetrics;
bool qtfonts_have_been_reset=false;
bool useQPrinter=false;
int print_target=PRINT_TARGET_SCREEN;//where to print to
bool printing_in_file=false;
bool point_explorer_activ=false;//blocks some actions if point-explorer-actions are currently selected
#if QT_VERSION >= 0x050000
extern ulong os_dbl_click_dt;
#endif

//long orig_page_w,orig_page_h;
QPrinter * stdPrinter;
#if QT_VERSION >= 0x050300
QPdfWriter * stdPDFWriter;
#endif
QSvgGenerator * stdGenerator;
int stdOutputFormat=1;
int rtiCheckTime=1000;
char dummy[4096];
char dummy2[4096];
char last_formula[1024];//the last formula used for computation (like regressions, fits, evaluate expression)
char saved_formula[1024];//the formula to remember
double last_fit_falues[MAXPARM];//the parameters of the last fit
double saved_value;//a constant value to remember

bool hdeviceFlag;
int hardCopyDeviceNr;
int save_successful=false;

QString newLegendString,newCommentString;
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
void try_loading_Gould(char * filename, int &nr_of_points, double **x, double **y);
void initDefaultWindowSizes(void);
extern void read_basic_icons(void);
extern void ParseFit2D(char * com,int & type,int & gno,int & sno,int & t_set,double & x0,double & y0,double & r1,double & r2,double & angle,double & phi0,double & phi1,int & n_phi,int & region,int & reg_inv);

QLocale * cur_loc;//the Locale-settings as set in the operating system

char new_encoding_name[128];
#if READ_UTF8_ONLY == 0
QTextCodec * FileCodec;//standard codec of the operating system (depends on user settings in the operating system)
QTextCodec * FileCodecSave;//to save the old codec in case it is changes during file-loading-operation
QTextCodec * IsoLatin1Codec;//codec for Iso-Latin-1-encoding which is identical to the T1-codec Grace uses internally (iso latin 1 is always available in Qt)
QList<QByteArray> avcod;//all available file encodings
#endif

QLocale * comma_locale;//a setting where ',' is the decimal separator (we use the setting for Germany here)
QLocale * dot_locale;//a setting where '.' is the decimal separator (we use the setting for the USA here)

QString Last_Dialog_Path[NR_OF_FILE_DIALOGS];//the last used path/filename for every file-dialog

QSize LastSize_EllipseProps;
QSize LastSize_EditEllipseProps;
QSize LastSize_BoxProps;
QSize LastSize_EditBoxProps;
QSize LastSize_TextProps;
QSize LastSize_EditTextProps;
QSize LastSize_LineProps;
QSize LastSize_EditLineProps;
QSize LastSize_FormDrawObjects;
QSize LastSize_FormConsole;
QSize LastSize_FormEvalExpr;
QSize LastSize_FormSetAppearance;
QSize LastSize_FormGraphAppearance;
QSize LastSize_FormPlotAppearance;
QSize LastSize_FormLocatorProps;
QSize LastSize_FormAxisProperties;
QSize LastSize_FormPointExplorer;
QSize LastSize_FormNonlinCurveFit;
QSize LastSize_Form2DFit;
QSize LastSize_FormInterpolation;
QSize LastSize_FormSetOperations;
QSize LastSize_FormExplorer;
QSize LastSize_FormRTIManage;
QSize LastSize_FormCommands;
QSize LastSize_FormDeviceOptions;
QSize LastSize_FormDeviceSetup;
QSize LastSize_FormUserDefaultDevice;
QSize LastSize_FormArrangeGraphs;
QSize LastSize_FormOverlayGraphs;
QSize LastSize_FormAutoScaleGraphs;
QSize LastSize_FormDataSetProperties;
QSize LastSize_FormSetOPS;
QSize LastSize_FormFeatureExtraction;
QSize LastSize_FormHistograms;
QSize LastSize_FormFourier;
QSize LastSize_FormFourier2;
QSize LastSize_FormLoadAndEvaluate;
QSize LastSize_FormIntegration;
QSize LastSize_FormSeasonDiff;
QSize LastSize_FormDifferences;
QSize LastSize_FormAverages;
QSize LastSize_FormRegression;
QSize LastSize_FormCorrelation;
QSize LastSize_FormConvolution;
QSize LastSize_FormDigitalFilter;
QSize LastSize_FormPruneData;
QSize LastSize_FormSamplePoints;
QSize LastSize_FormGeometricTransform;
QSize LastSize_FormHotLinks;
QSize LastSize_FormRegionMaster;
QSize LastSize_FormRegionStatus;
QSize LastSize_FormReportRegion;
QSize LastSize_FormClearRegion;
QSize LastSize_FormDefineRegion;
QSize LastSize_FormNetCDF;
QSize LastSize_FormReadSets;
QSize LastSize_FormReadNetCDF;
QSize LastSize_FormWriteSets;
QSize LastSize_FormOpenProject;
QSize LastSize_FormSaveProject;
QSize LastSize_FormSelectHotLink;
QSize LastSize_FormReadParameters;
QSize LastSize_FormWriteParameters;
QSize LastSize_FormSelectOutputFile;
QSize LastSize_FormOpenFitPara;
QSize LastSize_FormSaveFitPara;
QSize LastSize_FormReadBinary;
QSize LastSize_FormWriteBinary;
QSize LastSize_FormCSVImport;
QSize LastSize_FormCSVImportV2;
QSize LastSize_FormProgress;
QSize LastSize_FormQuestion;
QSize LastSize_FormSimpleListSel;
QSize LastSize_FormReportFitParameters;
QSize LastSize_FormGeometricEvaluation;
QSize LastSize_FormBinaryImportFilter;
QSize LastSize_FormEditBlockData;
QSize LastSize_FormFontTool;
QSize LastSize_FormAbout;
QSize LastSize_FormSetEditor;
QSize LastSize_FormUndoList;
QSize LastSize_DeviceActivator;
QSize LastSize_Form_Preferences;
QSize LastSize_Form_AgrInfo;
QSize LastSize_Form_SetupWizard;
QSize LastSize_Form_InitWizard;
QSize LastSize_FormIOForm;
QSize LastSize_EditColumnProp;
QSize LastSize_Spreadsheets;
QSize LastSize_mainWin;
QSize LastSize_Bailout;
QSize LastSize_CreateAltAxis;

UserDeviceGeometry DefaultUserDeviceGeometry[NR_OF_USER_DEFAULT_DEVICE_GEOMETRIES];
UserDeviceGeometry DefaultSystemDeviceGeometry[NR_OF_QUICKSELECTOPTIONS];

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
frm2DFit * Form2DFit;
//frmEditColumnProp * EditColumn;
frmInterpolation * FormInterpolation;
frmSetOp * FormSetOperations;
frmExplorer * FormExplorer;
frmRealTimeInputManager * FormRTIManage;
frmCommands * FormCommands;
frmDeviceSetup * FormDeviceSetup;
frmUserDefaultGeometries * FormUserDefaultGeometries;
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
frmCSVImporterV2 * FormCSVImportV2;
frmProgressWin * FormProgress;
frmQuestionDialog * FormQuestion;
frmSimpleListSelectionDialog * FormSimpleListSel;
frmReportOnFitParameters * FormReportFitParameters;
frmGeometricEvaluation * FormGeometricEvaluation;

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
SetupWizard * Form_SetupWizard;
InitQtGraceWizard * Form_InitWizard;
autoLabeler * FormAutoLabeler;
bailoutQuestion * FormBailoutQuestion;
frmCreateAltAxis * FormCreateAltAxis;

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
int NodeNr,used_Nodes,akt_Node;
int max_node_nr;
class undo_node * Node;

extern graph * g;
extern int maxstr;
extern double * old_ddata[16];//to store some data temporarely
extern int * old_idata[16];//to store some data temporarely
extern int old_data[16];
extern bool immediateUpdate;
extern bool updateRunning;
extern int default_Print_Device;
extern bool exchange_point_comma;
extern int new_set_no;
extern int * new_set_nos;
extern int new_set_blocks;
extern int * new_set_block_size;
extern QStringList ListOfChanges;
extern QStringList ListOfOldStates;
extern Device_entry * device_table;
extern char print_cmd[GR_MAXPATHLEN];
extern int nfonts;
extern int lastPrintDevice;
int currentStringAlignment;

extern defaults d_d;
extern defaults file_defaults;
extern defaults ini_defaults;
extern defaults tmp_defaults;

extern view d_v;
extern view file_view;
extern view ini_view;
extern view tmp_view;

extern OptionItem fmt_option_items[NUM_FMT_OPTION_ITEMS];

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

extern QString regtypes[10];
extern int reg_order[10];

//int ngraph_select_items = 0;
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
stdLineEdit ** Non_LaTeX_Lines=NULL;
int nr_of_non_latex_lines=0,n_of_non_latex_lines_alloc=0;
LineStyleSelector ** line_style_selectors=NULL;
int nr_of_line_style_selectors=0,n_of_line_style_selectors_alloc=0;
stdSlider ** all_sliders=NULL;
int nr_of_sliders=0,nr_of_sliders_alloc=0;
FillPatternSelector ** all_pattern_selectors=NULL;
int nr_of_pattern_selectors=0,n_of_pattern_selectors_alloc=0;
//New selectors
OrderSelector ** all_order_selectors=NULL;
int nr_of_order_selectors=0,n_of_order_selectors_alloc=0;
LineWidthSelector ** all_linewidth_selectors=NULL;
int nr_of_linewidth_selectors=0,n_of_linewidth_selectors_alloc=0;
stdIntSelector ** all_int_selectors=NULL;
int nr_of_int_selectors=0,n_of_int_selectors_alloc=0;
StdSelector ** all_std_selectors=NULL;
int nr_of_std_selectors=0,n_of_std_selectors_alloc=0;
AlignmentSelector ** all_alignment_selectors=NULL;
int nr_of_alignment_selectors=0,n_of_alignment_selectors_alloc=0;
JustificationSelector ** all_justification_selectors=NULL;
int nr_of_justification_selectors=0,n_of_justification_selectors_alloc=0;
PositionSelector ** all_position_selectors=NULL;
int nr_of_position_selectors=0,n_of_position_selectors_alloc=0;

struct ColumnFormat binaryImportFormat[NUMBER_OF_COLUMN_FORMATS];
char binaryImportFormatName[NUMBER_OF_COLUMN_FORMATS][32];
int ImportDestination[NUMBER_OF_IMPORT_DESTINATIONS];
char ImportDestinationName[NUMBER_OF_IMPORT_DESTINATIONS][32];
signed char ImportDestinationType[NUMBER_OF_IMPORT_DESTINATIONS];

struct csv_multi_import_infos def_csv_import_format;//this setting is used for csv-files that are entered via drag-and-drop

const char * formula_token_types[11]={"CompleteID","NGSS","SGNS","NGNS","SetOnly","NSetOnly","GraphOnly","NGraphOnly","ColumnOnly","Graph#","Set#"};

#ifdef DEBUG_OUT_LOG
ofstream debug_out;
#endif

int bin_format_size(int nr)
{
return binaryImportFormat[nr].size;
}

int find_import_destination(char * name,signed char type)
{
int ret=-1;
int size,size2=strlen(name);
for (int i=0;i<NUMBER_OF_IMPORT_DESTINATIONS;i++)
{
size=strlen(ImportDestinationName[i]);
//cout << "destination" << i << "=#"<< ImportDestinationName[i] << "#" << endl;
    if (size==size2 && (ImportDestinationType[i]&type)!=0 && strncmp(ImportDestinationName[i],name,size)==0)
    {
    ret=i;
    break;
    }
}
//cout << "to look for=#"<< name << "# --> ret=" << ret << " Type=" << int(type) << endl;
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
QPixmap * Qt_sub_justifications[3];
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

QBitmap * redrawMap;
QBitmap * autoMap;
QBitmap * zoomXMap;
QBitmap * zoomYMap;
QBitmap * autoXMap;
QBitmap * autoYMap;
QBitmap * autoTickMap;
QBitmap * exitMap;

QPixmap * GraceIcon;
QPixmap * ActiveIcon=NULL;
QPixmap * HiddenIcon=NULL;
QPixmap * Qt_j_lm_o_bits=NULL;
QPixmap * Qt_j_cm_o_bits=NULL;
QPixmap * Qt_j_rm_o_bits=NULL;
QPixmap * Qt_j_lb_b_bits=NULL;
QPixmap * Qt_j_lm_b_bits=NULL;
QPixmap * Qt_j_lt_b_bits=NULL;
QPixmap * Qt_j_ct_b_bits=NULL;
QPixmap * Qt_j_rt_b_bits=NULL;
QPixmap * Qt_j_rm_b_bits=NULL;
QPixmap * Qt_j_rb_b_bits=NULL;
QPixmap * Qt_j_cb_b_bits=NULL;
QPixmap * Qt_j_cm_b_bits=NULL;
QPixmap * Qt_j_left_bits=NULL;
QPixmap * Qt_j_right_bits=NULL;
QPixmap * Qt_j_center_bits=NULL;
QPixmap * Qt_m_hv_lr_bt_bits=NULL;
QPixmap * Qt_m_hv_lr_tb_bits=NULL;
QPixmap * Qt_m_hv_rl_bt_bits=NULL;
QPixmap * Qt_m_hv_rl_tb_bits=NULL;
QPixmap * Qt_m_vh_lr_bt_bits=NULL;
QPixmap * Qt_m_vh_lr_tb_bits=NULL;
QPixmap * Qt_m_vh_rl_bt_bits=NULL;
QPixmap * Qt_m_vh_rl_tb_bits=NULL;

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

CanvasAction action_flag = DO_NOTHING;
int region_def_under_way;

QColor * stdColors[16];//Colors for Drawing
QBrush * stdBrush;//standard Brush for Drawing
QPen * stdPen;//standard Pen for Drawing

/*char user_home_dir[1024];
char qt_grace_share_dir[1024];*/

QFile path_to_fftw3_lib,path_to_libharu;

//extern QTime lastc;
extern void GenerateLaTeXCommands(void);
extern void find_first_command(char * text,int & start_command,int & command_length,int * brace_pos);
extern void splitter(char * text,int start_command,int command_length,int * brace_pos,char * left_part,char * middle_part,char * argument_part,char * right_part);
extern void complete_LaTeX_to_Grace_Translator(QString & text);
extern void update_font_selectors(bool appearance);
extern void update_ss_editors(int gno);
extern void update_line_style_selectors(void);
extern void RecreatePatternIcons(int w,int h);

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

int find_QtFont_in_List(const char * name,int whatlist);//you need a qt-font-name for this
int find_QtFont_in_List(QString fontname,int whatlist);//finds a qt-font-name
int find_default_id_of_Grace_Font(const char * name);//you need a Grace-font-name for this
int find_GraceFontID_of_QtFontName(QString fontname);//you need a qt-font-name for this
QString getNameOfDefaultQtFont(int index);
QString get_QtName_of_Default_Grace_Font(const char *name);//compares a Grace-font-name with the default font names and returns the qt-font-name
char * get_Grace_Font_Name_of_Default_Qt_Font(QString fontname);//returns the Grace-font-name of a Qt-Font-name
char * getNameOfStdQtFont(int index);//returns the Qt-Name of the Font in the StdList
char * getFamilyNameOfStdQtFont(int index);//returns the Qt-Name of the Font in the StdList

//this is what you need
//--> remember: whatlist=0-->default-list, whatlist=1-->std-list (i.e. the current list - you probably want to use this)
//--> convert Grace-name into QtFontID
int get_QtFontID_from_Grace_Name(const char *name,int whatlist);//get the index of a QtFont in a list by its Grace-name
//--> convert Grace-font-id into QtFontID
int get_QtFontID_from_GraceID(int font_id,int whatlist);

void update_default_props(void);
void init_color_icons(void);
void read_settings(void);
void initialize_default_csv_format(void);
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
extern bool is_diadem(char * file,char ** header_name);
extern int is_agr_file(char * file);
extern int is_text_file(char * file);
extern void read_DIAdem_header(char * header_file,struct DIAdem_Header & header);
extern void postprocess_diadem_header(struct DIAdem_Header & header,struct importSettings & bin_import,int & nr_of_new_sets,int ** n_gnos,int ** n_snos);
/*
extern void read_INI_header(struct importSettings & imp_set,struct importSettings & imp_schema);
extern void read_BINARY_header(struct importSettings & imp_set,struct importSettings & imp_schema);
extern void read_ASCII_header(struct importSettings & imp_set,struct importSettings & imp_schema);
extern int postprocess_bin_import_data(struct importSettings & imp_set,int & nr_of_new_sets,int ** n_gnos,int ** n_snos);
*/
extern void copy_import_settings(struct importSettings * from, struct importSettings * to);
extern void prependSetID(char * text,int leftset,int leftgraph,int rightset,int rightgraph);
extern void prependAllIDs_V2(QString & text,int n_gno,int n_sno);//just complete incomplete IDs (no replacement and no resolvement of S0N or S# or similar)
extern void replaceAllIDs_V2(QString & text,int n_gno,int n_sno);

extern QString get_text_in_first_quotations(QString text);
extern QString get_text_in_quotations(QString text);
extern QList<int> get_text_positions(QString text,QString subtext);

extern void simpleReadCSVFileLikeFormat(QString filename,struct csv_multi_import_infos & input,QList<int> & target_graphs,QList<int> & target_sets);

long evaluate_version_id(const char * version)
{
int m1,m2,m3;
char sub;
sub=version[strlen(version)-1];
if (isalpha(sub)==0) sub=0;
else sub-='a';
sscanf(version+1,"%d.%d.%d",&m1,&m2,&m3);
return ((long)sub)+10l*m3+100l*m2+1000l*m1;
}

QStringList get_all_std_bin_format_files(void)
{
//cout << "qt_grace_share_dir=" << qt_grace_share_dir << endl;
    QString templ_path=QString(qt_grace_templates_dir);
    QDir ex_dir(templ_path);
    QStringList filters;
    filters << "QtGrace_std_bin_format_*.fmt";
    //filters << "*.fmt";
    QStringList list=ex_dir.entryList(filters,QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
    filters.clear();
    for (int i=0;i<list.length();i++)
    {
    filters << (QString(qt_grace_templates_dir)+QDir::separator()+list.at(i));
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

//qDebug() << "StdBinSchema=" << i << "Name=" << std_bin_import_settings[i].name;

/*qDebug() << "fname=#" << fname << "# imp-token=" <<  std_bin_import_settings[i].nr_of_import_tokens;
qDebug() << "Channels=" << std_bin_import_settings[i].channels;
for (int kl=0;kl<std_bin_import_settings[i].channels;kl++)
qDebug() << i << "|" << kl << "Target=" << std_bin_import_settings[i].channel_target[kl];*/
    //cout << "StdBinImportFormat=" << i << " DataSuffix=" << std_bin_import_settings[i].DataSuffix.toLocal8Bit().constData() << " HeaderSuffix=" << std_bin_import_settings[i].HeaderSuffix.toLocal8Bit().constData() << " import_keys=" << std_bin_import_settings[i].keys.length() << endl;
        delete[] fname;
    }
}

}

//the following function tries to guess the most suitable binary file-format for filename; returns RETURN_SUCCSESS or RETURN_FAILURE; the format-nr is returned in std_format_nr and it is returned whether this is a header or not (i.e. a binary file)
//the guessing is done based on the suffix!
int guess_bin_format(QString filename,int & std_format_nr,bool & is_header)
{
int ret=-1;
QString f_suffix;
QFileInfo finfo(QDir::toNativeSeparators(filename));
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
if (ret==-1)//suffix does not fit any known data-file
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
/*
int point_pos=fpath.lastIndexOf(".");
if (point_pos==-1)//no point found --> just add a suffix
{
fpath=fpath+QString(".")+n_suffix;
}
else
{
fpath=fpath.left(point_pos+1)+n_suffix;
}
*/
QFileInfo fileInfo(fpath);
fpath = QDir::toNativeSeparators(fileInfo.path()+QDir::separator()+fileInfo.completeBaseName()+(".")+n_suffix);
}

void copy_bin_settings_to_current_bin_import(QString filename,bool is_header,importSettings * imp_set)
{
    QString f_name;//=QString::fromLocal8Bit(filename);
    f_name=filename;
initSettings(current_bin_import_settings);//clear settings
copy_import_settings(imp_set,&current_bin_import_settings);//copy settings from imp_set to current settings
if (imp_set->header_present)//there should be a header
{
    if (imp_set->header_format==1)//header in same file as bin-data
    {
    current_bin_import_settings.DataFile=f_name;
    current_bin_import_settings.HeaderFile=current_bin_import_settings.DataFile;
    }
    else
    {
        if (is_header==false)
        {
        current_bin_import_settings.DataFile=f_name;
        current_bin_import_settings.HeaderFile=current_bin_import_settings.DataFile;
        replaceSuffix(current_bin_import_settings.HeaderFile,imp_set->HeaderSuffix);
        }
        else
        {
        current_bin_import_settings.HeaderFile=f_name;
        current_bin_import_settings.DataFile=current_bin_import_settings.HeaderFile;
        replaceSuffix(current_bin_import_settings.DataFile,imp_set->DataSuffix);
        }
    }
}
else//no header
{
current_bin_import_settings.DataFile=f_name;
current_bin_import_settings.HeaderFile=QString("");
}

//cout << "current_bin_import: data=#" << current_bin_import_settings.DataFile.toLocal8Bit().constData() << "#" << endl;
//cout << "current_bin_import: header=#" << current_bin_import_settings.HeaderFile.toLocal8Bit().constData() << "#" << endl;

/*
if (current_bin_import_settings.header_present)
{
    if (current_bin_import_settings.header_format==HEADER_FORMAT_INI_FILE)//ini-File
    {
    read_INI_header(current_bin_import_settings,*imp_set);
    }
    else if (current_bin_import_settings.header_format==HEADER_FORMAT_DATA_FILE)//binary-header in bin-file
    {
    read_BINARY_header(current_bin_import_settings,*imp_set);
    }
    else if (current_bin_import_settings.header_format==HEADER_FORMAT_ASCII_FILE)//ascii-header-file
    {
    read_ASCII_header(current_bin_import_settings,*imp_set);
    }
}

cout << "std=" << imp_set->nr_of_header_values << " current=" << current_bin_import_settings.nr_of_header_values << endl;
cout << "import_format=" << imp_set->channel_format[0] << "-->" << binaryImportFormatName[imp_set->channel_format[0]] << endl;
*/
//now we should begin reading...
}

void copy_std_settings_to_current_bin_import(QString filename,int std_format_nr,bool is_header)
{
copy_bin_settings_to_current_bin_import(filename,is_header,std_bin_import_settings+std_format_nr);
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
/*
cout << "read bin files:" << endl;
cout << "Header-File=#" << current_bin_import_settings.HeaderFile.toLocal8Bit().constData() << "#" << endl;
cout << "Data - File=#" << current_bin_import_settings.DataFile.toLocal8Bit().constData() << "#" << endl;
cout << "before reading bin-file:" << current_bin_import_settings.headersize << endl;
*/
QByteArray ba;
QBuffer buffer;
int ret=LoadFileIntoByteArray(current_bin_import_settings.DataFile,ba);
if (ret==RETURN_FAILURE) qDebug() << "Unable to load file into ByteArray";

switch (current_bin_import_settings.data_is_compressed)
{
default:
case COMPRESSION_NONE:
    ;//Do nothing - data is already uncompressed
break;
case COMPRESSION_QTINTERNAL:
    DecompressByteArray(ba,current_bin_import_settings.data_is_compressed,0);
break;
case COMPRESSION_EXTERNAL_ZLIB:
    //DecompressByteArray(ba,current_bin_import_settings.data_is_compressed,10);
    DecompressByteArray(ba,current_bin_import_settings.data_is_compressed,current_bin_import_settings.whole_size);
break;
}
buffer.setBuffer(&ba);
buffer.open(QIODevice::ReadOnly);

/// ifstream ifi;
/// ifi.open(current_bin_import_settings.DataFile.toLocal8Bit().constData(),ios::binary);
//cout << "reading from Bin-File=#" << current_bin_import_settings.DataFile.toLocal8Bit().constData() << "# opened?" << ifi.is_open() << endl;
//cout << "std bin import: channels=" << current_bin_import_settings.channels << " points=" << current_bin_import_settings.points << endl;
readBinaryFromFile(buffer,current_bin_import_settings,&current_bin_import_settings.first_data);
//cout << "std bin import: columns=" << current_bin_import_settings.columns_read << " points=" << current_bin_import_settings.points_read << endl;
/// ifi.close();

buffer.close();

int nr_of_new_sets=0;
int *n_snos=NULL,*n_gnos=NULL;
//int ret=postprocess_bin_import_data(current_bin_import_settings,nr_of_new_sets,&n_gnos,&n_snos);
(void)postprocess_bin_import_data(current_bin_import_settings,nr_of_new_sets,&n_gnos,&n_snos);
//cout << "Postprocessing: " << ret << endl;
//qDebug() << "nr_of_new_sets=" << nr_of_new_sets;

///Undo-Stuff
/*SaveSetStatesPrevious(0,n_gnos,n_snos,UNDO_COMPLETE);//all sets will be new --> no previous sets --> 0
SetsModified(nr_of_new_sets,n_gnos,n_snos,UNDO_COMPLETE);*/
SetsCreated(nr_of_new_sets,n_gnos,n_snos,UNDO_COMPLETE);
    if (nr_of_new_sets==1)//max_nr_of_sets==1)
    {
    sprintf(dummy,"[G%d.S%d]",n_gnos[0],n_snos[0]);
    addAditionalDescriptionToLastNode(-1,QObject::tr("Binary data import --> ")+QString(dummy),QString(),0);
    }
    else
    {
    addAditionalDescriptionToLastNode(-1,QObject::tr("Binary data import"),QString(),0);
    }
        /*for (int i=0;i<nr_of_new_sets;i++)
        {
        cout << "G" << n_gnos[i] << ".S" << n_snos[i] << endl;
        }*/
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
    return QString(user_home_dir)+QDir::separator()+QString("QtGrace_Settings.ini");
#else
    return QString(user_home_dir)+QDir::separator()+QString(".QtGrace_Settings.ini");
#endif
}

QString path_to_read_settings(void)//we try to read from the users home directory, if this fails, we use the QtGrace-executable-directory instead
{
QString path;
#ifdef WINDOWS_SYSTEM
    QString path1=QString(user_home_dir)+QDir::separator()+QString("QtGrace_Settings.ini");
#else
    QString path1=QString(user_home_dir)+QDir::separator()+QString(".QtGrace_Settings.ini");
#endif
    QString path2=QString(qt_grace_share_dir)+QDir::separator()+QString("QtGrace_Settings.ini");//always visible
long version1=0,version2=0;
bool exists1,exists2;
QDateTime dt1,dt2;
QSettings * allPrefs;
QString cur_version_string;
    QFileInfo fi1(path1);
    exists1=fi1.exists();
    if (exists1)
    {
    dt1=fi1.lastModified();
    allPrefs=new QSettings(path1,QSettings::IniFormat);
    allPrefs->sync();
    allPrefs->beginGroup(QString("General"));
    cur_version_string=allPrefs->value(QString("QtGrace_Version"),QVariant("v0.2.4")).toString();
    version1=evaluate_version_id(cur_version_string.toLocal8Bit().data());
    allPrefs->endGroup();
    delete allPrefs;
    //cout << "path1=" << path1.toLocal8Bit().constData() << " Version=" << cur_version_string.toLocal8Bit().constData() << " Version=" << version1 << " date=" << dt1.toString().toLocal8Bit().constData() << endl;
    }
    QFileInfo fi2(path2);
    exists2=fi2.exists();
    if (exists2)
    {
    dt2=fi2.lastModified();
    allPrefs=new QSettings(path2,QSettings::IniFormat);
    allPrefs->sync();
    allPrefs->beginGroup(QString("General"));
    cur_version_string=allPrefs->value(QString("QtGrace_Version"),QVariant("v0.2.4")).toString();
    version2=evaluate_version_id(cur_version_string.toLocal8Bit().data());
    allPrefs->endGroup();
    delete allPrefs;
    //cout << "path2=" << path2.toLocal8Bit().constData() << " Version=" << cur_version_string.toLocal8Bit().constData() << " Version=" << version2 << " date=" << dt1.toString().toLocal8Bit().constData() << endl;
    }
//QFile fil(path);
//if (fil.exists()==false)
    if (exists1==true && exists2==false)
    {
        if (dt2>dt1)
        path=path2;
        else
        path=path1;
    }
    else if (exists1==false && exists2==true)
    {
    path=path2;
    }
    else if (exists1==false && exists2==false)
    {
    path=path2;
    }
    else//both exist
    {
        if (version1<version2)
        {
        path=path2;
        }
        else if (version2<version1)
        {
        path=path1;
        }
        else
        {
            if (dt1<dt2)
            {
            path=path2;
            }
            else if (dt2<dt1)
            {
            path=path1;
            }
            else
            {
            path=path2;
            }
        }
    }
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
    default:
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
//QPixmap templIcon(82,16);//22
int x_size=82*toolBarSizeFactor;
int y_size=16*toolBarSizeFactor;
if (y_size%2!=0) y_size++;
QPixmap templIcon(x_size,y_size);
QPainter templPainter;
QPen pen1(Qt::black);
pen1.setCapStyle(Qt::FlatCap);
templIcon.fill(QApplication::palette().window().color());
templPainter.begin(&templIcon);
QFont fnt=qApp->font();
//fnt.setPixelSize(y_size-4);
fnt.setPixelSize(12*toolBarSizeFactor);
templPainter.setFont(fnt);
if ((*pendash)!=NULL) delete (*pendash);
(*pendash)=new QVector<qreal>();
//qDebug() << "Len=" << len;
for (int j=0;j<len;j++)
{
*(*pendash) << linestyle_pattern[j];
//qDebug() << (int)linestyle_pattern[j];
}
if (linestyle_pattern[0]==0)//starts with white although it should start with black
{
    templPainter.setPen(pen1);
    //templPainter.drawText(10,13,"None");//15
    //templPainter.drawText(10*toolBarSizeFactor,y_size-2,"None");
    templPainter.drawText(10*toolBarSizeFactor,13*toolBarSizeFactor,"None");
}
else
{
    pen1.setColor(Qt::black);
    pen1.setDashPattern(**pendash);
    pen1.setWidth(3*toolBarSizeFactor);
    pen1.setCapStyle(Qt::FlatCap);
    templPainter.setPen(pen1);
    //templPainter.drawLine(0,8,82,8);//11
    templPainter.drawLine(0,y_size/2,x_size,y_size/2);
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

int find_QtFont_in_List(const char *name,int whatlist)
{
    QString fname(name);
    return find_QtFont_in_List(fname,whatlist);
}

int find_default_id_of_Grace_Font(const char * name)//looks in the defaultFontNames for the grace-name of a default-QtFont and returns its position in the default QtFont-List
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

QString get_QtName_of_Default_Grace_Font(const char * name)
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

int get_QtFontID_from_Grace_Name(const char * name,int whatlist)//get the index of a QtFont in a list by its Grace-name
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

/*
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
*/

int FindFirstNewSetCommand(QString command,int offset,QString & found,int & index)//finds the first occurance of a regular expression for new-set-command and returns the type (1-5), -1=nothing found
{
int type=-1;
QRegularExpressionMatch rm;
//Type 1
const QRegularExpression rex1("G\\d+.S\\d+N");//G0.S1N
//Type 2
const QRegularExpression rex2("S\\d+N");//only S1N (without graph-ID)
//Type 3
const QRegularExpression rex3("G\\d+N");//G13N (without set-ID)
//Type 4
const QRegularExpression rex4("G\\d+N.S\\d+");//G2N.S3
//Type 5
const QRegularExpression rex5("G\\d+N.S\\d+N");//G2N.S5N
rm=rex1.match(command,offset);
//index=rex1.indexIn(command,offset);
if (rm.hasMatch()==true)//Type 1 found
{
type=1;
//found=rex1.capturedTexts().at(0);
found=rm.captured();
index=rm.capturedStart();
}
else//Type 1 not found
{
    rm=rex5.match(command,offset);
    //index=rex5.indexIn(command,offset);
    if (rm.hasMatch()==true)//Type 5 found
    {
    type=5;
    //found=rex5.capturedTexts().at(0);
    found=rm.captured();
    index=rm.capturedStart();
    }
    else//Type 5 not found
    {
        rm=rex4.match(command,offset);
        //index=rex4.indexIn(command,offset);
        if (rm.hasMatch()==true)//Type 4 found
        {
        type=4;
        //found=rex4.capturedTexts().at(0);
        found=rm.captured();
        index=rm.capturedStart();
        }
        else//Type 4 not found
        {
            rm=rex3.match(command,offset);
            //index=rex3.indexIn(command,offset);
            if (rm.hasMatch()==true)//Type 3 found
            {
            type=3;
            //found=rex3.capturedTexts().at(0);
            found=rm.captured();
            index=rm.capturedStart();
            }
            else//Type 3 not found
            {
                rm=rex2.match(command,offset);
                //index=rex2.indexIn(command,offset);
                if (rm.hasMatch()==true)//Type 2 found
                {
                type=2;
                //found=rex2.capturedTexts().at(0);
                found=rm.captured();
                index=rm.capturedStart();
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
int ret=-1,should=-1;
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
/// command=command.toUpper();///toupper currently deactivated
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
//cout << "READ:#" << found.toLatin1().constData() << "# type=" << type << " ID=G" << i_gno << ".S" << i_sno << endl;
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
/// add_point(n_gno,n_sno,0.0,0.0);
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
/// add_point(n_gno,n_sno,0.0,0.0);
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
/// add_point(n_gno,n_sno,0.0,0.0);
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

/*void test_write_pdf(void)
{

QPrinter * pr1=new QPrinter();
pr1->setOutputFormat(QPrinter::PdfFormat);
pr1->setOutputFileName(QString("/Users/andreaswinter/test_pdf_out.pdf"));
QPainter * paint1=new QPainter(pr1);
     //if (! paint1->begin(&printer))
     //{ failed to open file
     //qWarning("failed to open file, is it writable?");
     //return 1;
     //}
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
*/

/*
#if QT_VERSION >= 0x050000
QPdfWriter * writer=new QPdfWriter("D:\\PdfWriterTest.pdf");
QMessageBox::information(0,QString("filename="),QString("D:\\PdfWriterTest.pdf"));
//writer->setPageLayout(QPageLayout::Portrait);
#if QT_VERSION >= 0x050300
writer->setResolution(300);
#endif
writer->setPageSizeMM(QSizeF(120.0,290.0));
writer->newPage();

QPainter * paint=new QPainter();
paint->begin(writer);
paint->setPen(Qt::blue);
paint->setBrush(Qt::red);
paint->drawRect(10,10,300,300);
paint->setPen(Qt::black);
paint->drawText(100,100,QString("Test String"));
paint->end();

delete paint;
delete writer;
#endif

}*/

/*void test_write_svg(void)
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
}*/

void init(void)
{
    regtypes[REGION_POLYI]=QObject::tr("Inside polygon");
    regtypes[REGION_POLYO]=QObject::tr("Outside polygon");
    regtypes[REGION_ABOVE]=QObject::tr("Above line");
    regtypes[REGION_BELOW]=QObject::tr("Below line");
    regtypes[REGION_TOLEFT]=QObject::tr("Left of line");
    regtypes[REGION_TORIGHT]=QObject::tr("Right of line");
    regtypes[REGION_HORIZI]=QObject::tr("In Horiz. Range");
    regtypes[REGION_VERTI]=QObject::tr("In Vert. Range");
    regtypes[REGION_HORIZO]=QObject::tr("Out of Horiz. Range");
    regtypes[REGION_VERTO]=QObject::tr("Out of Vert. Range");

    reg_order[0]=REGION_POLYI;
    reg_order[1]=REGION_POLYO;
    reg_order[2]=REGION_ABOVE;
    reg_order[3]=REGION_BELOW;
    reg_order[4]=REGION_TOLEFT;
    reg_order[5]=REGION_TORIGHT;
    reg_order[6]=REGION_HORIZI;
    reg_order[7]=REGION_VERTI;
    reg_order[8]=REGION_HORIZO;
    reg_order[9]=REGION_VERTO;

QChar unicode[] = {char(0x03B1),'a'};
unicode_greek_shift = unicode[0].unicode()-unicode[1].unicode();//difference between 'alpha' and 'a'

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

    QFont stfont;
    defaultFontList.clear();
    //"Times New Roman,10,-1,5,50,0,0,0,0,0"
    if (stfont.fromString(QString("Times New Roman,10,-1,5,50,0,0,0,0,0"))==false) stfont.fromString(QString("Times,10,-1,5,50,0,0,0,0,0"));
    defaultFontList << stfont;
    if (stfont.fromString(QString("Times New Roman,10,-1,5,50,1,0,0,0,0"))==false) stfont.fromString(QString("Times,10,-1,5,50,1,0,0,0,0"));
    defaultFontList << stfont;
    if (stfont.fromString(QString("Times New Roman,10,-1,5,75,0,0,0,0,0"))==false) stfont.fromString(QString("Times,10,-1,5,75,0,0,0,0,0"));
    defaultFontList << stfont;
    if (stfont.fromString(QString("Times New Roman,10,-1,5,75,1,0,0,0,0"))==false) stfont.fromString(QString("Times,10,-1,5,75,1,0,0,0,0"));
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

    GenerateLaTeXCommands();
    region_def_under_way=-1;


/*#ifdef LINUX_SYSTEM
    showhideworkaround=true;
#else
    showhideworkaround=false;
#endif*/

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
    ImportDestinationType[27]=8;//2 only for data, set to 8 --> unused!

    ImportDestination[28]=IMPORT_TO_X_OFFSET;
    strcpy(ImportDestinationName[28],"X-Offset");
    ImportDestinationType[28]=1;//only for header
    ImportDestination[29]=IMPORT_TO_Y_OFFSET;
    strcpy(ImportDestinationName[29],"Y-Offset");
    ImportDestinationType[29]=1;//only for header
    ImportDestination[30]=IMPORT_TO_Y1_OFFSET;
    strcpy(ImportDestinationName[30],"Y1-Offset");
    ImportDestinationType[30]=1;//only for header
    ImportDestination[31]=IMPORT_TO_Y2_OFFSET;
    strcpy(ImportDestinationName[31],"Y2-Offset");
    ImportDestinationType[31]=1;//only for header
    ImportDestination[32]=IMPORT_TO_Y3_OFFSET;
    strcpy(ImportDestinationName[32],"Y3-Offset");
    ImportDestinationType[32]=1;//only for header
    ImportDestination[33]=IMPORT_TO_Y4_OFFSET;
    strcpy(ImportDestinationName[33],"Y4-Offset");
    ImportDestinationType[33]=1;//only for header
    ImportDestination[34]=IMPORT_TO_DATA_START_OFFSET;
    strcpy(ImportDestinationName[34],"Start offset (bytes)");
    ImportDestinationType[34]=1;//only for header

    strcpy(last_formula,"");
    strcpy(saved_formula,"");
        for (int i=0;i<MAXPARM;i++)
        last_fit_falues[i]=0.0;
    saved_value=0.0;
}

void init_phase2(void)
{

    init();//done a second time here for the pixmaps
    read_std_bin_import_settings();

    /*FormConsole=new frmConsole(mainWin);//needed for error-Messages
    Form_Preferences=new frm_Preferences(0);
    Form_Preferences->hide();
    Form_AgrInfo=new frmAgrInfos(0);
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
    FormQuestion->hide();*/
/// Moved to init_gui()

/*cout << "Before read_settings(): autofit_on_load=" << autofit_on_load << endl;
cout << "                warn_on_encoding_change=" << warn_on_encoding_change << endl;
cout << "Before read_settings(): hdevice="<< hdevice << endl;*/

    read_settings();

    initialize_default_csv_format();
/*cout << "After  read_settings(): hdevice="<< hdevice << endl;
cout << "After read_settings():  autofit_on_load=" << autofit_on_load << endl;
cout << "                warn_on_encoding_change=" << warn_on_encoding_change << endl;*/

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
    reset_default_states();

/*qDebug() << "grdefaults=" << grdefaults.font << endl;
qDebug() << "String Font=" << string_font << endl;
qDebug() << "default_string->font=" << default_string->font << endl;*/

    check_external_lib_usability();
    setlocale(LC_NUMERIC,"C");
}

void init_SystemDefaultGeometries(void)
{
    for (int i=0;i<NR_OF_QUICKSELECTOPTIONS;i++)
    {
    set_default_userdevicegeometry(DefaultSystemDeviceGeometry+i);
    DefaultSystemDeviceGeometry[i].unit=0;//set to pix
    DefaultSystemDeviceGeometry[i].orientation=0;//all the formats are landscape-formats
    }
    strcpy(DefaultSystemDeviceGeometry[0].name,QObject::tr("Letter, 72dpi").toLocal8Bit().constData());
    DefaultSystemDeviceGeometry[0].width=792.0;
    DefaultSystemDeviceGeometry[0].height=612.0;
    DefaultSystemDeviceGeometry[0].dpi=72.0;
        strcpy(DefaultSystemDeviceGeometry[1].name,QObject::tr("Letter, 300dpi").toLocal8Bit().constData());
        DefaultSystemDeviceGeometry[1].width=3300.0;
        DefaultSystemDeviceGeometry[1].height=2550.0;
        DefaultSystemDeviceGeometry[1].dpi=300.0;
    strcpy(DefaultSystemDeviceGeometry[2].name,QObject::tr("A4, 72dpi").toLocal8Bit().constData());
    DefaultSystemDeviceGeometry[2].width=842.0;
    DefaultSystemDeviceGeometry[2].height=595.0;
    DefaultSystemDeviceGeometry[2].dpi=72.0;
        strcpy(DefaultSystemDeviceGeometry[3].name,QObject::tr("A4, 300dpi").toLocal8Bit().constData());
        DefaultSystemDeviceGeometry[3].width=3510.0;
        DefaultSystemDeviceGeometry[3].height=2483.0;
        DefaultSystemDeviceGeometry[3].dpi=300.0;

    strcpy(DefaultSystemDeviceGeometry[4].name,QObject::tr("QVGA").toLocal8Bit().constData());
    DefaultSystemDeviceGeometry[4].width=320.0;
    DefaultSystemDeviceGeometry[4].height=240.0;
        strcpy(DefaultSystemDeviceGeometry[5].name,QObject::tr("QSVGA").toLocal8Bit().constData());
        DefaultSystemDeviceGeometry[5].width=400.0;
        DefaultSystemDeviceGeometry[5].height=300.0;
    strcpy(DefaultSystemDeviceGeometry[6].name,QObject::tr("HVGA").toLocal8Bit().constData());
    DefaultSystemDeviceGeometry[6].width=480.0;
    DefaultSystemDeviceGeometry[6].height=320.0;
        strcpy(DefaultSystemDeviceGeometry[7].name,QObject::tr("EGA").toLocal8Bit().constData());
        DefaultSystemDeviceGeometry[7].width=640.0;
        DefaultSystemDeviceGeometry[7].height=350.0;
    strcpy(DefaultSystemDeviceGeometry[8].name,QObject::tr("HSVGA").toLocal8Bit().constData());
    DefaultSystemDeviceGeometry[8].width=600.0;
    DefaultSystemDeviceGeometry[8].height=400.0;
        strcpy(DefaultSystemDeviceGeometry[9].name,QObject::tr("WGA").toLocal8Bit().constData());
        DefaultSystemDeviceGeometry[9].width=720.0;
        DefaultSystemDeviceGeometry[9].height=400.0;
    strcpy(DefaultSystemDeviceGeometry[10].name,QObject::tr("VGA").toLocal8Bit().constData());
    DefaultSystemDeviceGeometry[10].width=640.0;
    DefaultSystemDeviceGeometry[10].height=480.0;
        strcpy(DefaultSystemDeviceGeometry[11].name,QObject::tr("SVGA").toLocal8Bit().constData());
        DefaultSystemDeviceGeometry[11].width=800.0;
        DefaultSystemDeviceGeometry[11].height=600.0;
    strcpy(DefaultSystemDeviceGeometry[12].name,QObject::tr("WSVGA").toLocal8Bit().constData());
    DefaultSystemDeviceGeometry[12].width=1072.0;
    DefaultSystemDeviceGeometry[12].height=600.0;
        strcpy(DefaultSystemDeviceGeometry[13].name,QObject::tr("XGA").toLocal8Bit().constData());
        DefaultSystemDeviceGeometry[13].width=1024.0;
        DefaultSystemDeviceGeometry[13].height=768.0;
    strcpy(DefaultSystemDeviceGeometry[14].name,QObject::tr("720p=HD").toLocal8Bit().constData());
    DefaultSystemDeviceGeometry[14].width=1280.0;
    DefaultSystemDeviceGeometry[14].height=720.0;
        strcpy(DefaultSystemDeviceGeometry[15].name,QObject::tr("DSVGA").toLocal8Bit().constData());
        DefaultSystemDeviceGeometry[15].width=1200.0;
        DefaultSystemDeviceGeometry[15].height=800.0;
    strcpy(DefaultSystemDeviceGeometry[16].name,QObject::tr("SXVGA").toLocal8Bit().constData());
    DefaultSystemDeviceGeometry[16].width=1280.0;
    DefaultSystemDeviceGeometry[16].height=960.0;
        strcpy(DefaultSystemDeviceGeometry[17].name,QObject::tr("UXGA").toLocal8Bit().constData());
        DefaultSystemDeviceGeometry[17].width=1600.0;
        DefaultSystemDeviceGeometry[17].height=1200.0;
    strcpy(DefaultSystemDeviceGeometry[18].name,QObject::tr("1080p=FullHD").toLocal8Bit().constData());
    DefaultSystemDeviceGeometry[18].width=1920.0;
    DefaultSystemDeviceGeometry[18].height=1080.0;
        strcpy(DefaultSystemDeviceGeometry[19].name,QObject::tr("WUXGA").toLocal8Bit().constData());
        DefaultSystemDeviceGeometry[19].width=1920.0;
        DefaultSystemDeviceGeometry[19].height=1200.0;
    strcpy(DefaultSystemDeviceGeometry[20].name,QObject::tr("SUXGA").toLocal8Bit().constData());
    DefaultSystemDeviceGeometry[20].width=2048.0;
    DefaultSystemDeviceGeometry[20].height=1536.0;
        strcpy(DefaultSystemDeviceGeometry[21].name,QObject::tr("1440p").toLocal8Bit().constData());
        DefaultSystemDeviceGeometry[21].width=3440.0;
        DefaultSystemDeviceGeometry[21].height=1440.0;
    strcpy(DefaultSystemDeviceGeometry[22].name,QObject::tr("2160p=4K=UHD").toLocal8Bit().constData());
    DefaultSystemDeviceGeometry[22].width=3840.0;
    DefaultSystemDeviceGeometry[22].height=2160.0;
        strcpy(DefaultSystemDeviceGeometry[23].name,QObject::tr("UHD+=5K").toLocal8Bit().constData());
        DefaultSystemDeviceGeometry[23].width=5120.0;
        DefaultSystemDeviceGeometry[23].height=2880.0;
    strcpy(DefaultSystemDeviceGeometry[24].name,QObject::tr("4320p=8K").toLocal8Bit().constData());
    DefaultSystemDeviceGeometry[24].width=7680.0;
    DefaultSystemDeviceGeometry[24].height=4320.0;
        strcpy(DefaultSystemDeviceGeometry[25].name,QObject::tr("8640p=16K").toLocal8Bit().constData());
        DefaultSystemDeviceGeometry[25].width=15360.0;
        DefaultSystemDeviceGeometry[25].height=8640.0;
    for (int i=0;i<NR_OF_QUICKSELECTOPTIONS;i++)
    {
    sprintf(dummy," (%.0fx%.0f)",DefaultSystemDeviceGeometry[i].width,DefaultSystemDeviceGeometry[i].height);
    strcat(DefaultSystemDeviceGeometry[i].name,dummy);
    }
}

void init_gui(void)
{
    mainWin=new MainWindow();
    init();//for all the default pixmaps
    //qDebug() << "LastSize=" << LastSize_FormConsole;
    FormConsole=new frmConsole(mainWin);//needed for error-Messages
    Form_Preferences=new frm_Preferences(0);
    Form_Preferences->init();
    Form_Preferences->hide();
    Form_AgrInfo=new frmAgrInfos(0);
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
    FormBailoutQuestion=new bailoutQuestion(mainWin);
    FormBailoutQuestion->hide();

//qDebug() << "A Console.size=" << FormConsole->size();

        if (autofit_on_load==0)
        {
        //use_new_print_dialog=allPrefs->value(QString("UseNewPrintingDialog"),QVariant(false)).toBool();
        //no paint device yet --> we have to disconnect this, because redraw is inpossible now
        mainWin->disconnect(mainWin->sldPageZoom,SIGNAL(valueChanged(int)),mainWin,SLOT(doPageZoom(int)));
        mainWin->sldPageZoom->setValue(page_view_zoom);
        mainWin->connect(mainWin->sldPageZoom,SIGNAL(valueChanged(int)),mainWin,SLOT(doPageZoom(int)));
        GeneralPageZoomFactor=pow(10.0,mainWin->sldPageZoom->value()*mainWin->sldPageZoom->ScalingFactor);
        //qDebug() << "page_view_zoom=" << page_view_zoom << " General=" << GeneralPageZoomFactor;
        }

        QString codName=QString::fromLocal8Bit(file_codec_name);
        /*if (codName.compare(QString("UTF-8"))==0)
        {
        qDebug() << "Text Codec == UTF-8";
        }
        else
        {
        qDebug() << "Text Codec NOT UTF-8";
        }*/

//qDebug() << "codName=" << codName;
        int index=0;
#if READ_UTF8_ONLY == 0
        for (int i=0;i<avcod.length();i++)
        {
            if (QString(avcod.at(i)).compare(codName)==0)
            {
                index=i;
                break;
            }
        }
        FileCodec=QTextCodec::codecForName(codName.toLocal8Bit().constData());
//qDebug() << "CodName=" << codName.toLocal8Bit().constData() << " Adress=" << FileCodec;
        if (FileCodec==NULL)
        {
            qDebug() << "Warning, invalid FileCodec! " << codName.toLocal8Bit().constData();
            FileCodec=QTextCodec::codecForLocale();//fallback is the systems usual codec
        }
#endif
        Form_Preferences->selCodec->setCurrentIndex(index);
        //Form_Preferences->tab_extra->init();
        Form_Preferences->tab_prefs->doApply();
        //Form_Preferences->tab_extra->doApply();

        mainWin->recreateHistory();
//qDebug() << "B Console.size=" << FormConsole->size();
//int res_undo=undo_active;
        undo_active=FALSE;
        if (FormDeviceSetup==NULL)
        {
            FormDeviceSetup=new frmDeviceSetup(mainWin);
//qDebug() << "B-2 Console.size=" << FormConsole->size();
            //initialize this only on startup
            if (default_Print_Device==-1)//last one
                FormDeviceSetup->devices_item->setCurrentValue(stdOutputFormat);
            else
                FormDeviceSetup->devices_item->setCurrentValue(default_Print_Device);
            /*cout << "default_Print_Device=" << default_Print_Device << endl;
            cout << "stdOutput=" << stdOutputFormat << endl;*/
            lastPrintDevice=FormDeviceSetup->devices_item->currentValue();
            FormDeviceSetup->printfile_item->lenText->setText(get_filename_with_extension(FormDeviceSetup->cur_dev));
        }
        FormDeviceSetup->hide();
        FormDeviceSetup->print_string_item->setText( get_print_cmd() );
//qDebug() << "B-1 Console.size=" << FormConsole->size();
        FormDeviceSetup->doApply();
//qDebug() << "B0 Console.size=" << FormConsole->size();
        //FormDeviceSetup->resize(400,400);
//qDebug() << "B1 Console.size=" << FormConsole->size();
int col1,col2,col3;
    sscanf(background_color,"%d;%d;%d",&col1,&col2,&col3);
//qDebug() << "background=" << background_color << " r=" << col1 << " g=" << col2 << " b=" << col3;
QColor col=QColor::fromRgb(col1,col2,col3);
    mainWin->mainArea->setBGtoColor(col);
//qDebug() << "B2 Console.size=" << FormConsole->size();
    mainWin->ManageBars();
    Form_Preferences->init();
    set_title(mybasename(get_docname()));
//qDebug() << "B3 Console.size=" << FormConsole->size();
    /*if (autofit_pending==1)
    {
    mainWin->doFitPage();
    autofit_pending=0;
    }*/
//qDebug() << "B4 Console.size=" << FormConsole->size();
    mainWin->mainArea->completeRedraw();///-->init gui
//qDebug() << "B5 Console.size=" << FormConsole->size();
    undo_active=start_with_undo_active;

//qDebug() << "C Console.size=" << FormConsole->size();
    FormConsole->resize(LastSize_FormConsole);
}

int main( int argc, char **argv )
{
    startupphase=1;
    strcpy(default_grace_file,"Default.agr");//the standard-Grace-File
    grdefaults=ini_defaults=file_defaults=d_d;//at the beginning the grace-defaults are to be used for all defaults
    grview=ini_view=file_view=d_v;
    stop_repaint=TRUE;
    dollar_t=new char[8];
    strcpy(dollar_t,"$t");

    xy_xconv = xy_xconv_general;
    xy_yconv = xy_yconv_general;

/*cout << endl;
cout << "Default at start:       autofit_on_load=" << autofit_on_load << endl;
cout << "                warn_on_encoding_change=" << warn_on_encoding_change << endl;*/

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

// For Qt > 4.8 plugin search
        if (getenv("BEAST") && getenv("ABI"))
        {
        //qDebug()<<"START 1";
            string qt5DevDir = string(getenv("BEAST")) + "/lib/qt5/" + string(getenv("ABI")) + "/plugins";
            string qt4DevDir = string(getenv("BEAST")) + "/lib/qt4/" + string(getenv("ABI")) + "/plugins";
            string qtRunDir = string(getenv("BEAST")) + "/bin/" + string(getenv("ABI")) + "/plugins";
            // If we use a Qt installation outside BEAST directory tree, then it should
            // appear first in the list, not the last one !
            QStringList list=QApplication::libraryPaths(); // Qt installation
            // BEAST tree Qt "installations"
            list << QString(qt5DevDir.c_str()) << QString(qt4DevDir.c_str()) << QString(qtRunDir.c_str());
            QApplication::setLibraryPaths(list);
        }
#if QT_VERSION >= 0x050600 && QT_VERSION < 0x060000
QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
QString arg_test_str;
bool style_option_present=false;
for (int i=0;i<argc;i++)
{
arg_test_str=QString(argv[i]).toLower();
    if (arg_test_str=="-style")
    {
    style_option_present=true;
        if (i<argc-1)
        {
        arg_test_str=QString(argv[i+1]).toLower();
        break;
        }
        else
        {
        style_option_present=false;
        }
    }
}
if (style_option_present==false)
{
QApplication::setStyle(QStyleFactory::create("Fusion"));//necessary for upscaling on high-dpi-screens --> only activate this if the user does not set a different style
}
else
{
QApplication::setStyle(QStyleFactory::create(arg_test_str));
}
QApplication * a=new QApplication( argc, argv );
//if (style_option_present==false)
//a->setStyle(QString("Fusion"));//necessary for upscaling on high-dpi-screens --> only activate this if the user does not set a different style

#if QT_VERSION >= 0x050000
os_dbl_click_dt=(ulong)a->doubleClickInterval();
#endif
strcpy(user_home_dir,QDir::homePath().toLocal8Bit());
for (int i=0;i<MAXPATTERNS;i++)
patterns[i]=NULL;

//GuiFont=new QFont();//the stdFont is used for the GUI; size 14 is used because everythings fits neatly this way
GuiFont=new QFont(QApplication::font());
GuiFont->setPixelSize(13);

allPrefs=new QSettings(path_to_read_settings(),QSettings::IniFormat);
allPrefs->sync();
allPrefs->beginGroup(QString("Appearance"));
toolBarSizeFactor=allPrefs->value(QString("toolBarSizeFactor"),QVariant(1.0)).toDouble();
QString pat_lens=allPrefs->value(QString("GuiFont"),QVariant(qApp->font().toString())).toString();
GuiFont->fromString(pat_lens);
qApp->setFont(*GuiFont);
allPrefs->endGroup();
delete allPrefs;

read_basic_icons();//needed for error-messages

#ifdef DEBUG_OUT_LOG
char dumy1[GR_MAXPATHLEN];
QString dumy2(user_home_dir);
dumy2 += QDir::separator()+QString("qtgrace_debug_log.txt");
strcpy(dumy1,dumy2.toLocal8Bit().constData());
//sprintf(dumy1,"%s\\qtgrace_debug_log.txt",user_home_dir);
qDebug() << "Opening" << dumy1;
debug_out.open(dumy1);
debug_out << "QtGrace " << QTGRACE_VERSION_STRING <<  " Debug Log" << endl;
debug_out << QDateTime::currentDateTime().toString().toLocal8Bit().constData() << endl;
debug_out << "Now Starting QtGrace with the following arguments:" << endl;
for (int i=0;i<argc;i++)
{
debug_out << argv[i] << " ";
}
debug_out << endl;
#endif

    /*QStringList k12=QStyleFactory::keys();
    cout << "keys=" << endl;
    for (int i=0;i<k12.length();i++)
    {
    cout << k12.at(i).toLocal8Bit().constData() << endl;
    }*/

/*a->setStyle("macintosh");*/
/*"windows", "motif", "cde", "plastique", "windowsxp", or "macintosh"*/

//char * wdir1=new char[GR_MAXPATHLEN+1];
    (void)getcwd(wdir1, GR_MAXPATHLEN - 1);
    set_workingdir(wdir1);

    for (int i=0;i<NR_OF_USER_DEFAULT_DEVICE_GEOMETRIES;i++)
    set_default_userdevicegeometry(DefaultUserDeviceGeometry+i);
    init_SystemDefaultGeometries();

    for (int i=0;i<MAX_HISTORY;i++)
    history[i]=QString("");

    initSettings(current_bin_import_settings,false);//for binary import --> to clear everything

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
#if QT_VERSION >= 0x050300
    stdPDFWriter=NULL;
#endif
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
    Form2DFit=NULL;
    FormSetOperations=NULL;
    FormCommands=NULL;
    FormDeviceSetup=NULL;
    FormUserDefaultGeometries=NULL;
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
    FormCSVImportV2=NULL;
    FormProgress=NULL;
    FormQuestion=NULL;
    Form_Preferences=NULL;
    Form_AgrInfo=NULL;
    FormSimpleListSel=NULL;
    FormReportFitParameters=NULL;
    FormGeometricEvaluation=NULL;
    Form_SetupWizard=NULL;
    Form_InitWizard=NULL;
    FormAutoLabeler=NULL;
    FormCreateAltAxis=NULL;
    initDefaultWindowSizes();

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
#if READ_UTF8_ONLY == 0
    FileCodec=QTextCodec::codecForLocale();//get the codec most suitable for this system
    IsoLatin1Codec=QTextCodec::codecForName("ISO-8859-1");//get standard iso-latin-1 codec
    avcod=QTextCodec::availableCodecs();

//qDebug() << "nr of codecs-before:" << avcod.length();
/*now we clear the aliases*/
    QList<QByteArray> list_aliases;
    QTextCodec * one_codec_name;
    for (int i=0;i<avcod.length();i++)
    {
    one_codec_name=QTextCodec::codecForName(avcod.at(i));
    list_aliases=one_codec_name->aliases();
        for (int j=i+1;j<avcod.length();j++)
        {
            if (list_aliases.contains(avcod.at(j)) || QString(avcod.at(i)).compare(avcod.at(j))==0)
            {
            avcod.removeAt(j);
            j--;
            }
        }
    }
    QStringList st_avcod;
    st_avcod.clear();
    for (int i=0;i<avcod.length();i++) st_avcod << QString(avcod.at(i));
    st_avcod.sort();
    avcod.clear();
    for (int i=0;i<st_avcod.length();i++) avcod << st_avcod.at(i).toLocal8Bit();
#endif

/*finished cleaning up*/
//qDebug() << "nr of codecs-after:" << avcod.length();
//qDebug() << QTextCodec::codecForName("windows-857")->aliases();
#if QT_VERSION >= 0x060000
    QChar dpc=cur_loc->decimalPoint().at(0);
#else
    QChar dpc=cur_loc->decimalPoint();
#endif
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


#ifdef MAC_SYSTEM
    strcpy(bin_dir,"MacOS");
#endif
#ifdef LINUX_SYSTEM
    //strcpy(bin_dir,"templates");//there is no bin-directory on Linux, because the executable goes elsewhere
    strcpy(bin_dir,"bin");
#endif
#ifdef WINDOWS_SYSTEM
    strcpy(bin_dir,"bin");
#endif

/// #ifdef LINUX_SYSTEM
///    strcpy(qt_grace_share_dir,"/usr/share/qtgrace");
/// #else
    QDir applDir(qApp->applicationDirPath());
    applDir.cdUp();
    strcpy(qt_grace_share_dir,applDir.absolutePath().toLocal8Bit().constData());
//qDebug() << "Share_path=" << qt_grace_share_dir;
/// #endif

strcpy(qt_grace_example_dir,qt_grace_share_dir);
strcat(qt_grace_example_dir,"/examples");
strcpy(qt_grace_script_dir,qt_grace_share_dir);
strcat(qt_grace_script_dir,"/scripts");
strcpy(qt_grace_icons_dir,qt_grace_share_dir);
strcat(qt_grace_icons_dir,"/fonts/icons");
strcpy(qt_grace_templates_dir,qt_grace_share_dir);
strcat(qt_grace_templates_dir,"/templates");

#ifdef WINDOWS_SYSTEM
    strcpy(qt_grace_doc_dir,qt_grace_share_dir);
    strcat(qt_grace_doc_dir,"/doc");
    path_to_fftw3_lib.setFileName(QString(qt_grace_share_dir)+QDir::separator()+QString("libfftw3-3.dll"));
    path_to_libharu.setFileName(QString(qt_grace_share_dir)+QDir::separator()+QString("libhpdf.dll"));
#endif
#ifdef MAC_SYSTEM
    strcpy(qt_grace_doc_dir,qt_grace_share_dir);
    strcat(qt_grace_doc_dir,"/doc");
    path_to_fftw3_lib.setFileName(QString(qt_grace_share_dir)+QDir::separator()+QString("libfftw3-3.dylib"));
    path_to_libharu.setFileName(QString(qt_grace_share_dir)+QDir::separator()+QString("libhpdf.dylib"));
#endif
#ifdef LINUX_SYSTEM
    /// strcpy(qt_grace_doc_dir,"/usr/share/doc/qtgrace");
    /// strcpy(qt_grace_icons_dir,"/usr/share/icons/qtgrace");

    strcpy(qt_grace_doc_dir,qt_grace_share_dir);
    strcat(qt_grace_doc_dir,"/doc");

    path_to_fftw3_lib.setFileName(QString(qt_grace_share_dir)+QDir::separator()+QString("libfftw3-3.so"));
    path_to_libharu.setFileName(QString(qt_grace_share_dir)+QDir::separator()+QString("libhpdf.so"));
#endif

    //define the available languages
    strcpy(translator_files[0],"-");
    strcpy(translator_files[1],"qt_grace_ger");
    strcpy(translator_languages[0],"English");
    strcpy(translator_languages[1],"German");
    for (int i=1;i<nr_of_translations;i++)
    {
        translator[i].load(QString(qt_grace_share_dir)+QDir::separator()+translator_files[i]);
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
    autofit_on_load=allPrefs->value(QString("AutoFitPageOnLoad"),QVariant(FALSE)).toInt();
    int sellang=allPrefs->value(QString("language"),QVariant(0)).toInt();
    useQtFonts=allPrefs->value(QString("useQtFonts"),QVariant(false)).toBool();
    symbol_font_is_special=allPrefs->value(QString("SymbolFontIsSpecial"),QVariant(true)).toBool();

    use_fftw3=allPrefs->value(QString("UseLibFFTW3"),QVariant(FALSE)).toInt();
    path_to_fftw3_lib.setFileName(allPrefs->value(QString("LibFFTW3_Path"),QVariant(path_to_fftw3_lib.fileName())).toString());
    use_libHaru=allPrefs->value(QString("UseLibHaru"),QVariant(FALSE)).toInt();
    path_to_libharu.setFileName(allPrefs->value(QString("LibHaru_Path"),QVariant(path_to_libharu.fileName())).toString());

    allPrefs->endGroup();
    allPrefs->beginGroup(QString("General"));
QString IniVersionString=allPrefs->value(QString("QtGrace_Version"),QVariant("v0.2.4")).toString();
QString ExeVersionString(QTGRACE_VERSION_STRING);//the true value
/// QString ExeVersionString("v0.3.1");//this will always show the setup-wizard
if (evaluate_version_id(IniVersionString.toLatin1().constData())<evaluate_version_id(ExeVersionString.toLatin1().constData()))
{
show_setup_wizard=TRUE;
}
else
{
show_setup_wizard=FALSE;
}
setup_wizard_manual=FALSE;

    lastPrintDevice=stdOutputFormat=allPrefs->value(QString("lastOutputFormat"),QVariant(1)).toInt();
    default_Print_Device=allPrefs->value(QString("DefaultPrintingDevice"),QVariant(DEVICE_PNG)).toInt();//changed from -1 to PNG
    activateLaTeXsupport=allPrefs->value(QString("activateLaTeXSupport"),QVariant(false)).toBool()==true?1:0;

    warn_on_encoding_change=allPrefs->value(QString("Warn_On_Encoding_Change"),QVariant(TRUE)).toInt();
    DefaultFont=allPrefs->value(QString("DefaultFont"),QVariant(0)).toInt();
    strcpy(default_grace_file,allPrefs->value(QString("DefaultAgrFile"),QVariant(QString("Default.agr"))).toString().toLocal8Bit().constData());
    comment_server_client=allPrefs->value(QString("CommentServerClient"),QVariant(FALSE)).toInt();
    allPrefs->endGroup();
    allPrefs->beginGroup(QString("Appearance"));
    use_new_icons=allPrefs->value(QString("UseNewIcons"),QVariant(false)).toBool();
#if QT_VERSION >= 0x060000
    start_dpi=allPrefs->value(QString("StartDPI"),QVariant(qApp->primaryScreen()->physicalDotsPerInchX())).toInt();
#else
    start_dpi=allPrefs->value(QString("StartDPI"),QVariant(qApp->desktop()->physicalDpiX())).toInt();
#endif
    allPrefs->endGroup();
    delete allPrefs;

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
//QApplication * a=new QApplication( argc, argv );
    if (sellang>0)
    {
    a->installTranslator(&translator[sellang]);
    current_language=sellang;
    }

    stop_repaint=FALSE;

    if (default_Print_Device==-1)//last one
    hardCopyDeviceNr=stdOutputFormat;
    else
    hardCopyDeviceNr=default_Print_Device;

/// set up all internal Grace-things
replacement_main(argc,argv);//contains read_settings() in init_phase2()

QString CriticalVersionString=QString("v0.2.7");
if (evaluate_version_id(IniVersionString.toLatin1().constData())<evaluate_version_id(CriticalVersionString.toLatin1().constData()))
{//we changed the ids for the spreadsheet-column-formats
switch (SpreadsheetColumnFormat)
{
default:
case 0://Decimal
SpreadsheetColumnFormat=FORMAT_DECIMAL;
break;
case 1://General
SpreadsheetColumnFormat=FORMAT_GENERAL;
break;
case 2://Exponential
SpreadsheetColumnFormat=FORMAT_EXPONENTIAL;
break;
case 3://Date/time
    switch (get_date_hint())
    {
    default:
    case FMT_iso:
    case FMT_european:
    SpreadsheetColumnFormat=FORMAT_MMDDYYHMS;
    break;
    case FMT_us:
    case FMT_nohint:
    SpreadsheetColumnFormat=FORMAT_YYMMDDHMS;
    break;
    }
break;
}
}

if (gracebat==TRUE)//no GUI wanted
{
    /// delete mainWin;
    delete GeneralPainter;
    delete MainPixmap;
    return 0;
}

    for (int i=0;i<NR_OF_FILE_DIALOGS;i++)
    {
    Last_Dialog_Path[i]=QString(get_workingdir());
    }
    Last_Dialog_Path[READ_COMMANDS_FILE]=qt_grace_script_dir;
    Last_Dialog_Path[WRITE_COMMANDS_FILE]=qt_grace_script_dir;

    if (block_setup_wizard==TRUE)
    show_setup_wizard=FALSE;

    stop_repaint=TRUE;

//qDebug() << "A autofit_pending=" << autofit_pending;

    init_gui();//finish initialization
    a->setWindowIcon(*GraceIcon);

/*
qDebug() << "GraceIcon-Size=" << GraceIcon->size();
QPixmap tpix;
GraceIcon->save(QString("/Users/andreaswinter/Desktop/qtgrace_w_undo/GraceIcon_Original_48.png"));
//Fast Transformation
tpix=GraceIcon->scaledToWidth(512,Qt::FastTransformation);
tpix.save(QString("/Users/andreaswinter/Desktop/qtgrace_w_undo/GraceIcon_Fast_512.png"));
tpix=GraceIcon->scaledToWidth(256,Qt::FastTransformation);
tpix.save(QString("/Users/andreaswinter/Desktop/qtgrace_w_undo/GraceIcon_Fast_256.png"));
tpix=GraceIcon->scaledToWidth(128,Qt::FastTransformation);
tpix.save(QString("/Users/andreaswinter/Desktop/qtgrace_w_undo/GraceIcon_Fast_128.png"));
tpix=GraceIcon->scaledToWidth(64,Qt::FastTransformation);
tpix.save(QString("/Users/andreaswinter/Desktop/qtgrace_w_undo/GraceIcon_Fast_64.png"));
tpix=GraceIcon->scaledToWidth(32,Qt::FastTransformation);
tpix.save(QString("/Users/andreaswinter/Desktop/qtgrace_w_undo/GraceIcon_Fast_32.png"));
tpix=GraceIcon->scaledToWidth(16,Qt::FastTransformation);
tpix.save(QString("/Users/andreaswinter/Desktop/qtgrace_w_undo/GraceIcon_Fast_16.png"));
//Fast Transformation
tpix=GraceIcon->scaledToWidth(512,Qt::SmoothTransformation);
tpix.save(QString("/Users/andreaswinter/Desktop/qtgrace_w_undo/GraceIcon_Smooth_512.png"));
tpix=GraceIcon->scaledToWidth(256,Qt::SmoothTransformation);
tpix.save(QString("/Users/andreaswinter/Desktop/qtgrace_w_undo/GraceIcon_Smooth_256.png"));
tpix=GraceIcon->scaledToWidth(128,Qt::SmoothTransformation);
tpix.save(QString("/Users/andreaswinter/Desktop/qtgrace_w_undo/GraceIcon_Smooth_128.png"));
tpix=GraceIcon->scaledToWidth(64,Qt::SmoothTransformation);
tpix.save(QString("/Users/andreaswinter/Desktop/qtgrace_w_undo/GraceIcon_Smooth_64.png"));
tpix=GraceIcon->scaledToWidth(32,Qt::SmoothTransformation);
tpix.save(QString("/Users/andreaswinter/Desktop/qtgrace_w_undo/GraceIcon_Smooth_32.png"));
tpix=GraceIcon->scaledToWidth(16,Qt::SmoothTransformation);
tpix.save(QString("/Users/andreaswinter/Desktop/qtgrace_w_undo/GraceIcon_Smooth_16.png"));
*/

    //Here setup socket connection.
    if (enableServerMode)
    {
        mainWin->SocketConnection = new QtGraceTcpServer(sendToBeast,readFromBeast,mainWin);
        #if QT_VERSION >= 0x050100
        QObject::connect(a, SIGNAL(applicationStateChanged(Qt::ApplicationState)), mainWin->SocketConnection, SLOT(applicationStateChanged(Qt::ApplicationState)));
        #endif
        /// mainWin->SocketConnection = new LocalSocketIpcServer(sendToBeast,readFromBeast,mainWin);
    }
    else// if (enableServerMode )
    {
        if (comment_server_client)
        cerr << QObject::tr("QtGrace not connected to a client.").toLocal8Bit().constData() << endl;
        //fprintf(stderr, QObject::tr("QtGrace not connected to a client.\n").toLocal8Bit().constData());
    }

/*  if (enableServerMode==TRUE)
    {
#if QT_VERSION < 0x050000
    QApplication::setStyle(new skfStyle(3,3));
#else
    QApplication::setStyle(new skfStyle(3,3));
#endif
    }*/

/// init_phase2();

//cout << "AFTER READ SETTINGS: useQtFonts=" << useQtFonts << endl;

    if (qApp->font()!=*GuiFont)
    {
    //cout << "Aendere Font von: " << qApp->font().toString().toLatin1().constData() << endl;
    //cout << "            nach: " << GuiFont->toString().toLatin1().constData() << endl;
    QApplication::setFont(*GuiFont);//change the Gui-Font
    }

    if (hdeviceFlag==true)//overwrite hdevice
    {
    set_printer(hardCopyDeviceNr);
    stdOutputFormat=hardCopyDeviceNr;
    //FormDeviceSetup->devices_item->setCurrentValue(hardCopyDeviceNr);
    }

//cout << "After overwrite: hdevice=" << hdevice << endl;

int maxwi,maxhi;
#if QT_VERSION >= 0x060000
maxwi=qApp->primaryScreen()->geometry().width()*0.9;
maxhi=qApp->primaryScreen()->geometry().height()*0.9;
#else
maxwi=qApp->desktop()->width()*0.9;
maxhi=qApp->desktop()->height()*0.9;
#endif
if (initial_x_pos>maxwi)
{
initial_x_pos=0;
errwin(QObject::tr("Initial x-position too great for screen, x-position reset to 0.").toLocal8Bit().constData());
}
if (initial_y_pos>maxhi)
{
initial_y_pos=0;
errwin(QObject::tr("Initial y-position too great for screen, y-position reset to 0.").toLocal8Bit().constData());
}

    mainWin->setGeometry(initial_x_pos, initial_y_pos, initial_width, initial_height );
    //mainWin->move(initial_x_pos, initial_y_pos);
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

set_workingdir(wdir1);//just for safety...

if (initial_size_behavior==0)//revert to default size(s)
{
initDefaultWindowSizes();
}

//resize the dialogs that have already been initialized
Form_Preferences->resize(LastSize_Form_Preferences);
//qDebug() << "A FormConsole=" << FormConsole->size();
FormConsole->resize(LastSize_FormConsole);
//qDebug() << "B FormConsole=" << FormConsole->size();
Form_AgrInfo->resize(LastSize_Form_AgrInfo);
EditTextProps->resize(LastSize_EditTextProps);
EditLineProps->resize(LastSize_EditLineProps);
EditBoxProps->resize(LastSize_EditBoxProps);
EditEllipseProps->resize(LastSize_EditEllipseProps);
FormProgress->resize(LastSize_FormProgress);
FormQuestion->resize(LastSize_FormQuestion);

    /*if (autofit_pending==1)
    {
    mainWin->doFitPage();
    autofit_pending=0;
    }*/

//cout << "B) Working dir=#" << get_workingdir() << "#" << endl;
//delete[] wdir1;

//qDebug() << "B autofit_pending=" << autofit_pending;

    startupphase=0;

/*START OF TEST AREA*/

/*
char testresult[1024];
double d_val1;
int end1,empty1;

strcpy(testresult,"12:00:00");
(void) read_date_or_number_in_format(testresult,FORMAT_HMS,TRUE,&d_val1,&end1,&empty1);
qDebug() << "testresult=" << testresult << "value=" << d_val1;
strcpy(testresult,"22:41:06");
(void) read_date_or_number_in_format(testresult,FORMAT_HMS,TRUE,&d_val1,&end1,&empty1);
qDebug() << "testresult=" << testresult << "value=" << d_val1;
strcpy(testresult,"07:02:01");
(void) read_date_or_number_in_format(testresult,FORMAT_HMS,TRUE,&d_val1,&end1,&empty1);
qDebug() << "testresult=" << testresult << "value=" << d_val1;
strcpy(testresult,"10:35:56");
(void) read_date_or_number_in_format(testresult,FORMAT_HMS,TRUE,&d_val1,&end1,&empty1);
qDebug() << "testresult=" << testresult << "value=" << d_val1;

double jul;
strcpy(testresult,"-4713-01-01 12:00:00");
(void) parse_date_or_number(testresult, TRUE, &jul);
qDebug() << "jul=" << jul;
strcpy(testresult,"-4713-01-02 12:00:00");
(void) parse_date_or_number(testresult, TRUE, &jul);
qDebug() << "jul=" << jul;
strcpy(testresult,"-4713-01-03 12:00:00");
(void) parse_date_or_number(testresult, TRUE, &jul);
qDebug() << "jul=" << jul;
strcpy(testresult,"-4713-01-04 12:00:00");
(void) parse_date_or_number(testresult, TRUE, &jul);
qDebug() << "jul=" << jul;
set_ref_date(jul);
qDebug() << "refDateSet";

strcpy(testresult,"-4713-01-02 12:00:00");
(void) parse_date_or_number(testresult, TRUE, &jul);
qDebug() << "TRUE: jul=" << jul;
(void) parse_date_or_number(testresult, FALSE, &jul);
qDebug() << "FALSE: jul=" << jul;

strcpy(testresult,"-4713-01-01 12:00:00");
(void) parse_date_or_number(testresult, TRUE, &jul);
set_ref_date(jul);

strcpy(testresult,"-4713-01-02");
qDebug() << "Date-parse-test:" << parse_date_or_number(testresult, TRUE, &jul);
qDebug() << "jul=" << jul;
strcpy(testresult,"12:00:00");
qDebug() << "Time-parse-test:" << parse_date_or_number(testresult, TRUE, &jul);
qDebug() << "jul=" << jul;

strcpy(testresult,"-4713-01-01 13:00:00");
(void) parse_date_or_number(testresult, TRUE, &jul);
qDebug() << "jul=" << jul;
strcpy(testresult,"-4713-01-01 14:00:00");
(void) parse_date_or_number(testresult, TRUE, &jul);
qDebug() << "jul=" << jul;
strcpy(testresult,"-4713-01-01 18:00:00");
(void) parse_date_or_number(testresult, TRUE, &jul);
qDebug() << "jul=" << jul;
strcpy(testresult,"-4713-01-11 13:05");
(void) parse_date_or_number(testresult, TRUE, &jul);
qDebug() << "jul=" << jul;*/

/*
double d_test=2448161.0,d_val1;//09/27/90
double d_test2=2448162.0,d_val2;//09/28/90
char c_testresult[1024];
char * testresult=create_fstring(FORMAT_MMDDYY,2,d_test,LFORMAT_TYPE_EXTENDED);

sprintf(c_testresult,"%d",(int)(d_test));
qDebug() << Qt::endl << "d_test=" << c_testresult;
qDebug() << "testresult=" << testresult << "(soll=09/27/90)";
int end1,empty1;
int ret0=read_date_or_number_in_format(testresult,FORMAT_MMDDYY,TRUE,&d_val1,&end1,&empty1);
sprintf(c_testresult,"%d",(int)(d_val1));
qDebug() << "ABSOLUTE= TRUE, d_val1 =" << c_testresult << "ret0=" << ret0;
ret0=read_date_or_number_in_format(testresult,FORMAT_MMDDYY,FALSE,&d_val1,&end1,&empty1);
sprintf(c_testresult,"%d",(int)(d_val1));
qDebug() << "ABSOLUTE=FALSE, d_val1 =" << c_testresult << "ret0=" << ret0;
testresult=create_fstring(FORMAT_MMDDYY,2,d_val1,LFORMAT_TYPE_EXTENDED);
qDebug() << "rueckkonversion=" << testresult << "(soll=09/27/90)";

sprintf(c_testresult,"%d",(int)(d_test2));
qDebug() << Qt::endl << "d_test2=" << c_testresult;
testresult=create_fstring(FORMAT_MMDDYY,2,d_test2,LFORMAT_TYPE_EXTENDED);
qDebug() << "testresult=" << testresult << "(soll=09/28/90)";
ret0=read_date_or_number_in_format(testresult,FORMAT_MMDDYY,TRUE,&d_val2,&end1,&empty1);
sprintf(c_testresult,"%d",(int)(d_val2));
qDebug() << "d_val2=" << c_testresult << "ret0=" << ret0;
testresult=create_fstring(FORMAT_MMDDYY,2,d_val2,LFORMAT_TYPE_EXTENDED);
qDebug() << "rueckkonversion=" << testresult << "(soll=09/28/90)" << Qt::endl;
*/

//jul_to_cal_and_time(loc, ROUND_DAY, &y, &m, &d, &h, &mm, &sec);
//sprintf(s, format, d, m, yprec, y);

/*
QString test1("D:\\+54°C\\test\\äüößtest.pdf");
char * test1_ch=new char[1024];
char * test2_ch=new char[1024];
qDebug() << "test1=" << test1;
strcpy(test1_ch,test1.toLocal8Bit().constData());
qDebug() << "test1_ch=" << test1_ch;
test1=QString::fromLocal8Bit(test1_ch);
qDebug() << "test1=" << test1;
//strcpy(test2_ch,test1_ch);
sprintf(test2_ch,"neu: %s neu",test1_ch);
test1=QString::fromLocal8Bit(test2_ch);
qDebug() << "test1=" << test1;
delete[] test1_ch;
delete[] test2_ch;
*/

/*
QList<int> list_lens;
list_lens.clear();
list_lens << 2;
list_lens << 1;
list_lens << 0;
list_lens << 3;

QList< QList<int> > perm;
perm.clear();
generatePermutations(list_lens,perm);
qDebug() << "Permutations: list_lens=" << list_lens;
qDebug() << "Permutations=" << perm;
*/

/*ofstream ofi44;
ifstream ifi44;
int probe44=-1;
double probe45=-1.0;
ofi44.open("tesfile44.txt");
ofi44 << "Testfile44 V1" << endl;
ofi44 << "Save_Number= " << 21 << endl;
sprintf(dummy,sformat,4.0*atan(1.0));
ofi44 << "Save_Number2= " << dummy << endl;
ofi44.close();
ifi44.open("tesfile44.txt");
ifi44 >> dummy >> dummy2;
ifi44 >> dummy >> probe44;
ifi44 >> dummy >> probe45;
ifi44.close();
qDebug() << "Zahl1=" << probe44 << "Zahl2=" << probe45;*/

/*
    frmGroupTestWidget * fgtw=new frmGroupTestWidget(0);
    fgtw->show();
*/

/*
QString testtext("formula: \"a+2*b\" = \"sin(x)+y\"");
    qDebug() << endl << "testtext=" << testtext;
QString inquotation=get_text_in_first_quotations(testtext);
    qDebug() << "first: inquotation=" << inquotation;
    inquotation=get_text_in_quotations(testtext);
    qDebug() << "last: inquotation=" << inquotation;
QList<int> pos_list=get_text_positions(testtext,QString("\""));
    qDebug() << "count=" << pos_list.length();
    for (int i=0;i<pos_list.length();i++)
    qDebug() << pos_list.at(i);
*/

/*QString testtext("G0 SF1.X S5.Y A19=AVG(G0.S1N.Y)+G7N.S7+G3.S2.X+G4.S1N.X+G0+S1.X+S3N.Y2-S5N+G0.S2N-G1N.S4-Y=YOU+DELAY+G8#");
int found_offset=0,found_index;
QString found_string;
int found_type=FindFirstNewSetCommand(testtext,found_offset,found_string,found_index);
qDebug() << endl << "testtext=" << testtext;
qDebug() << "found_type=" << found_type << "found_string=" << found_string << "found_index=" << found_index;
while (found_type>=0)
{
found_offset=found_index+found_string.length();
found_type=FindFirstNewSetCommand(testtext,found_offset,found_string,found_index);
qDebug() << "testtext=" << testtext;
qDebug() << "found_type=" << found_type << "found_string=" << found_string << "found_index=" << found_index;
}

found_offset=0;
found_type=FindFirstNewSetCommand2(testtext,found_offset,found_string,found_index);
qDebug() << endl << "testtext=" << testtext;
qDebug() << "found_type=" << found_type << "found_string=" << found_string << "found_index=" << found_index;
while (found_type>=0)
{
found_offset=found_index+found_string.length();
found_type=FindFirstNewSetCommand2(testtext,found_offset,found_string,found_index);
qDebug() << "testtext=" << testtext;
qDebug() << "found_type=" << found_type << "found_string=" << found_string << "found_index=" << found_index;
}*/

//QString testtext("G0.S0.Y G0N S5.Y G5.S2.X");
/*QString testtext("G0 SF1.X S5.Y A19=AVG(G0.S1N.Y)+G7N.S7+G3.S2.X+G4.S1N.X+G0+S1.X+S3N.Y2-S5N+G0.S2N-G1N.S4-Y=YOU+DELAY+G8#");
QString testStrings[8];
testStrings[0]=QString("G0.S0.Y");
testStrings[1]=QString("G0N.S0.Y");
testStrings[2]=QString("G0.S0N.Y");
testStrings[3]=QString("G0N.S0N.Y");
testStrings[4]=QString("S0.Y");
testStrings[5]=QString("S0N.Y");
testStrings[6]=QString("G0");
testStrings[7]=QString("G0N");

qDebug() << endl << "testText=" << testtext;
QRegExp RegularSetID("G\\d+\\.{1}S\\d+\\.?[XY]?");//A complete regular set-ID
QRegExp NewGraphStdSetID("G\\d+[N\\#\\$]{1}\\.{1}S\\d+\\.?[XY]?");//New GraphID, regular set-ID
QRegExp StdGraphNewSetID("G\\d+\\.{1}S\\d+[N\\#\\$]{1}\\.?[XY]?");//Regular GraphID, new set-ID
QRegExp NewGraphNewSetID("G\\d+[N\\#\\$]{1}\\.{1}S\\d+[N\\#\\$]{1}\\.?[XY]?");//New GraphID, new set-ID
QRegExp SetIDOnly("S\\d+\\.?[XY]?");//set-ID only
QRegExp NewSetIDOnly("S\\d+[N\\#\\$]{1}\\.?[XY]?");//New set-ID only
QRegExp GraphIDOnly("G\\d+(?!(\\.|N|\\#|\\$))");//Graph-ID only (not followed by anything else (especially no '.') )
QRegExp NewGraphIDOnly("G\\d+[N\\#\\$]{1}(?!(\\.|N|\\#|\\$))");//New set-ID only (not followed by anything else (especially no '.') )

int pos=RegularSetID.indexIn(testtext);
qDebug() << "Regular pos=" << pos;
QStringList list123=RegularSetID.capturedTexts();
    for (int i=0;i<list123.count();i++)
    qDebug() << "Catched=" << list123.at(i);

pos=NewGraphStdSetID.indexIn(testtext);
qDebug() << "New Graph Old Set pos=" << pos;
list123=NewGraphStdSetID.capturedTexts();
    for (int i=0;i<list123.count();i++)
    qDebug() << "Catched=" << list123.at(i);

pos=StdGraphNewSetID.indexIn(testtext);
qDebug() << "Old Graph New Set pos=" << pos;
list123=StdGraphNewSetID.capturedTexts();
    for (int i=0;i<list123.count();i++)
    qDebug() << "Catched=" << list123.at(i);

pos=NewGraphNewSetID.indexIn(testtext);
qDebug() << "New Graph New Set pos=" << pos;
list123=NewGraphNewSetID.capturedTexts();
    for (int i=0;i<list123.count();i++)
    qDebug() << "Catched=" << list123.at(i);

pos=SetIDOnly.indexIn(testtext);
qDebug() << "Set ID Only pos=" << pos;
list123=SetIDOnly.capturedTexts();
    for (int i=0;i<list123.count();i++)
    qDebug() << "Catched=" << list123.at(i);

pos=NewSetIDOnly.indexIn(testtext);
qDebug() << "New Set ID Only pos=" << pos;
list123=NewSetIDOnly.capturedTexts();
    for (int i=0;i<list123.count();i++)
    qDebug() << "Catched=" << list123.at(i);

pos=GraphIDOnly.indexIn(testtext);
qDebug() << "Graph ID Only pos=" << pos;
list123=GraphIDOnly.capturedTexts();
    for (int i=0;i<list123.count();i++)
    qDebug() << "Catched=" << list123.at(i);

pos=NewGraphIDOnly.indexIn(testtext);
qDebug() << "New Graph ID Only pos=" << pos;
list123=NewGraphIDOnly.capturedTexts();
    for (int i=0;i<list123.count();i++)
    qDebug() << "Catched=" << list123.at(i);

int f_ret,f_pos,g_no,s_no,st_len;
char control1,control2,col;
for (int i=0;i<8;i++)
{
qDebug() << endl << "teststring[" << i << "]=" << testStrings[i];
f_ret=searchTextForFirstID(testStrings[i],0,f_pos,g_no,s_no,control1,control2,col,st_len);
    if (f_ret>=0)
    {
    qDebug() << "f_ret=" << f_ret;
    qDebug() << "f_pos=" << f_pos;
    qDebug() << "gno=" << g_no;
    qDebug() << "sno=" << s_no;
    qDebug() << "control1=" << control1;
    qDebug() << "control2=" << control2;
    qDebug() << "length=" << st_len;
    qDebug() << "col=" << col;
    qDebug() << "Replacement=" << createReplacementID(f_ret,3,5,g_no,s_no,control1,control2,col);
    }
    else
    {
    qDebug() << "No ID found" << endl;
    }
}*/

/*
testtext=QString("Formula G0 S1N.X S5.Y A19=AVG(G0.S1N.Y)+G7N.S7+G3.S2.X+G4.S1N.X+G0+S1.X+S3N.Y2-S5N-S#+G0.S2N-G1N.S4-Y[G#]=YOU+DELAY+G8#");
qDebug() << "testtext before =" << testtext;
prependAllIDs_V2(testtext,8,9);//just complete incomplete IDs (no replacement and no resolvement of S0N or S# or similar)
qDebug() << "testtext after (complete)=" << testtext;
replaceAllIDs_V2(testtext,8,9);
qDebug() << "testtext after (replace) =" << testtext;
*/

    /*qDebug() << wdir1;
            (void)getcwd(wdir1, GR_MAXPATHLEN - 1);
    qDebug() << wdir1;*/

/// preprocessor-test
/*
class formula_to_process formula1;
QString zwischen_string;
//formula1.init_formula(QString("fit formula \"y=A0+A1*y3+A2*sin(2*PI*A3*x+A4)+A5*sin(2*PI*A6*y1+A7+A8*y2)\""));
//formula1.init_formula(QString("y=A0+A1*y3+A2*sin(2*PI*A3*x+A4)+A5*sin(2*PI*A6*y1+A7+A8*y2)"));
zwischen_string=QString("y=a0+a1*sin(a2*x+a3);fit formula \"y=a0+a1*x+a2*x^2\";formula2 \"X=2*x;Y=3*x\" set;y1=exp(x*y+y2)");
formula1.init_formula(zwischen_string);

qDebug() << "Original=" << formula1.original_formula;
qDebug() << formula1.token_startpos.length() << " Found Tokens; Positions:";
for (int i=0;i<formula1.token_startpos.length();i++)
{
qDebug() << formula1.token_startpos.at(i) << " Type=" << (formula1.token_type.at(i)<0?"NONE":formula_token_types[formula1.token_type.at(i)]) << " Column=" << formula1.token_column.at(i) << " length=" << formula1.token_len.at(i);
}
qDebug() << "Zwischentext:";
for (int i=-1;i<formula1.token_startpos.length();i++)
{
formula1.get_string_after_token(i,zwischen_string);
qDebug() << i << "#" << zwischen_string;
}

formula1.return_completed_formula(20,21,22,23,zwischen_string);
qDebug() << "Completed Formula=" << zwischen_string;

*/

/*
qDebug() << endl;
zwischen_string=QString("y=a0+a1*sin(a2*x+a3);fit formula \"y=a0+a1*x+a2*x^2\";formula2 \"a=2*x;b=3*x\" set;y1=exp(x*y+y2)");
formula1.init_formula(zwischen_string);

qDebug() << "Original2=" << formula1.original_formula;
qDebug() << formula1.token_startpos.length() << " Found Tokens; Positions:";
for (int i=0;i<formula1.token_startpos.length();i++)
{
qDebug() << formula1.token_startpos.at(i) << " Type=" << formula1.token_type.at(i) << " Column=" << formula1.token_column.at(i);
}
qDebug() << "Zwischentext2:";
for (int i=-1;i<formula1.token_startpos.length();i++)
{
formula1.get_string_after_token(i,zwischen_string);
qDebug() << i << "#" << zwischen_string;
}

formula1.return_completed_formula(0,1,1,2,zwischen_string);
qDebug() << "Completed Formula2=" << zwischen_string;
*/

/*zwischen_string=QString("y=a0+a1*sin(a2*x+a3);fit formula \"y=a0+a1*x+a2*x^2\";formula2 \"a=2*x;b=3*x\" set;y1=exp(x*y+y2)");
//zwischen_string=QString("y=a0+a1*sin(a2*x+a3)");

qDebug() << "String=" << zwischen_string;
*/

/*QStringList teilstrings;
QList<int> teiloffsets;
formula1.get_List_of_Subformulas(zwischen_string,teilstrings,teiloffsets);

qDebug() << "Teilstrings:";
for (int i=0;i<teilstrings.length();i++)
{
qDebug() << teiloffsets.at(i) << " - " << teilstrings.at(i);
}*/

/*double start_angle=0.5;
double stop_angle=6.4;
double probe=0.4;
double twopi=2.0*M_PI;
qDebug() << endl << "Inside-Test";
qDebug() << "[" << start_angle << "-" << stop_angle << "]";
qDebug() << "[" << start_angle-2.0*M_PI << "-" << stop_angle-2.0*M_PI << "]";
qDebug() << "[" << start_angle+2.0*M_PI << "-" << stop_angle+2.0*M_PI << "]";
double offsets[4]={0.0,-twopi,3.0*twopi,-14.0*twopi};
for (int i=0;i<4;i++)
{
qDebug() << endl << "Offset=" << offsets[i];
probe=0.4+offsets[i];
qDebug() << probe << "(0) --> " << is_inside_angle_range(probe,start_angle,stop_angle);
probe=0.1+offsets[i];
qDebug() << probe << "(1) --> " << is_inside_angle_range(probe,start_angle,stop_angle);
probe=12.6+offsets[i];
qDebug() << probe << "(1) --> " << is_inside_angle_range(probe,start_angle,stop_angle);
probe=12.7+offsets[i];
qDebug() << probe << "(0) --> " << is_inside_angle_range(probe,start_angle,stop_angle);
probe=0.55+offsets[i];
qDebug() << probe << "(1) --> " << is_inside_angle_range(probe,start_angle,stop_angle);
}*/
/// different number-formats test

/*double jul222,jul223;
int ret_val=RETURN_FAILURE;
int types=34,probe_format;
char ** formats_v=new char*[types];
for (int i=0;i<types;i++)
formats_v[i]=new char[32];

char ** formats_v2=new char*[types];
for (int i=0;i<types;i++)
formats_v2[i]=new char[32];

strcpy(formats_v[0],"1.52");//Decimal
strcpy(formats_v[1],"1e5");//Exponential
strcpy(formats_v[2],"-0.58e2");//General
strcpy(formats_v[3],"10\\S-0.528");//Power
strcpy(formats_v[4],"2.5x10\\S3");//Scientific
strcpy(formats_v[5],"235m");//Engineering
strcpy(formats_v[6],"30K");//Computing
strcpy(formats_v[7],"12-03-79");//DD-MM-YY
strcpy(formats_v[8],"03-12-79");//MM-DD-YY
strcpy(formats_v[9],"79-03-12");//YY-MM-DD
strcpy(formats_v[10],"03-79");//MM-YY
strcpy(formats_v[11],"03-12");//MM-DD
strcpy(formats_v[12],"March-12");//Month-DD
strcpy(formats_v[13],"12-March");//DD-Month
strcpy(formats_v[14],"Mar");//Month (abrev.)
strcpy(formats_v[15],"Mar-79");//Month (abrev.)-YY
strcpy(formats_v[16],"March");//Month
strcpy(formats_v[17],"Fri");//Day of week (abrev.)
strcpy(formats_v[18],"Friday");//Day of week
strcpy(formats_v[19],"128");//Day of year
strcpy(formats_v[20],"08:46:13.124");//HH:MM:SS
strcpy(formats_v[21],"03-12 08:46:13");//MM-DD HH:MM:SS
strcpy(formats_v[22],"03-12-79 08:46:13");//MM-DD-YY HH:MM:SS
strcpy(formats_v[23],"79-03-12 08:46:13");//YY-MM-DD HH:MM:SS
strcpy(formats_v[24],"65E");//Degrees (lon)
strcpy(formats_v[25],"65 15'E");//DD MM' (lon)
strcpy(formats_v[26],"65 15' 13.2\"E");//DD MM' SS.s" (lon)
strcpy(formats_v[27],"15' 13.2''E");//MM' SS.s" (lon)
strcpy(formats_v[28],"65N");//Degrees (lat)
strcpy(formats_v[29],"65 15'N");//DD MM' (lat)
strcpy(formats_v[30],"65 15' 13.2''N");//DD MM' SS.s" (lat)
strcpy(formats_v[31],"15' 13.2''N");//MM' SS.s" (lat)
strcpy(formats_v[32],"1979");//Year (YYYY)
strcpy(formats_v[33],"79/128/07:15:22");//YY/YDAY/HH:MM:SS

strcpy(formats_v2[0],"-152");//Decimal
strcpy(formats_v2[1],"0.18e-3");//Exponential
strcpy(formats_v2[2],"-7.8E5");//General
strcpy(formats_v2[3],"10^2.5");//Power
strcpy(formats_v2[4],"-1.1*10^4");//Scientific
strcpy(formats_v2[5],"235n");//Engineering
strcpy(formats_v2[6],"1.5M");//Computing
strcpy(formats_v2[7],"12/03/79");//DD-MM-YY
strcpy(formats_v2[8],"03.12.79");//MM-DD-YY
strcpy(formats_v2[9],"79/03/12");//YY-MM-DD
strcpy(formats_v2[10],"03/79");//MM-YY
strcpy(formats_v2[11],"03.12");//MM-DD
strcpy(formats_v2[12],"March/12");//Month-DD
strcpy(formats_v2[13],"12.March");//DD-Month
strcpy(formats_v2[14],"Oct");//Month (abrev.)
strcpy(formats_v2[15],"Oct/79");//Month (abrev.)-YY
strcpy(formats_v2[16],"April");//Month
strcpy(formats_v2[17],"Mon");//Day of week (abrev.)
strcpy(formats_v2[18],"Monday");//Day of week
strcpy(formats_v2[19],"221");//Day of year
strcpy(formats_v2[20],"10:11:12");//HH:MM:SS
strcpy(formats_v2[21],"03/12 08:46:13");//MM-DD HH:MM:SS
strcpy(formats_v2[22],"03.12.79 08:46:13");//MM-DD-YY HH:MM:SS
strcpy(formats_v2[23],"79/03/12 08:46:13");//YY-MM-DD HH:MM:SS
strcpy(formats_v2[24],"65 W");//Degrees (lon)
strcpy(formats_v2[25],"65 15' W");//DD MM' (lon)
strcpy(formats_v2[26],"65 15'13.2\"W");//DD MM' SS.s" (lon)
strcpy(formats_v2[27],"15'13.2''W");//MM' SS.s" (lon)
strcpy(formats_v2[28],"65 S");//Degrees (lat)
strcpy(formats_v2[29],"65 15' S");//DD MM' (lat)
strcpy(formats_v2[30],"65 15' 13.2'' S");//DD MM' SS.s" (lat)
strcpy(formats_v2[31],"15'13.2'' S");//MM' SS.s" (lat)
strcpy(formats_v2[32],"2009");//Year (YYYY)
strcpy(formats_v2[33],"79/128/07:15:22");//YY/YDAY/HH:MM:SS
*/

/*qDebug() << endl << "Format parse test (grace-auto):";
for (int i=0;i<types;i++)
{
ret_val=parse_date_or_number(formats_v[i], TRUE, &jul222);
qDebug() << (ret_val==RETURN_FAILURE?"?":" ") << i << " " << formats_v[i] << " success=" << ret_val << " val=" << jul222;
}*/

/*qDebug() << endl << "Format read test 1 (QtGrace specified):";
for (int i=0;i<types;i++)
{
ret_val=read_date_or_number_in_format(formats_v[i], i, TRUE, &jul222);
qDebug() << (ret_val==RETURN_FAILURE?"?":" ") << i << " " << formats_v[i] << " success=" << ret_val << " val=" << jul222;
}
qDebug() << endl << "Format read test 2 (QtGrace specified):";
for (int i=0;i<types;i++)
{
ret_val=read_date_or_number_in_format(formats_v2[i], i, TRUE, &jul222);
qDebug() << (ret_val==RETURN_FAILURE?"?":" ") << i << " " << formats_v2[i] << " success=" << ret_val << " val=" << jul222;
}*/

/*
for (probe_format=7;probe_format<34;probe_format++)
{
qDebug() << endl << "Cross-Read-Test 1: Probe-Format=" << probe_format;
for (int i=0;i<types;i++)
{
ret_val=read_date_or_number_in_format(formats_v[i], i, TRUE, &jul223);
ret_val=read_date_or_number_in_format(formats_v[i], probe_format, TRUE, &jul222);
    if (ret_val!=RETURN_FAILURE && jul222==jul223)//only show a result if the reading worked correctely
    qDebug() << (ret_val==RETURN_FAILURE?"?":" ") << i << " " << formats_v[i] << " success=" << ret_val << " val=" << jul222;
}
qDebug() << endl << "Cross-Read-Test 2: Probe-Format=" << probe_format;
for (int i=0;i<types;i++)
{
ret_val=read_date_or_number_in_format(formats_v2[i], i, TRUE, &jul223);
ret_val=read_date_or_number_in_format(formats_v2[i], probe_format, TRUE, &jul222);
    if (ret_val!=RETURN_FAILURE && jul222==jul223)//only show a result if the Reading worked
    qDebug() << (ret_val==RETURN_FAILURE?"?":" ") << i << " " << formats_v2[i] << " success=" << ret_val << " val=" << jul222;
}
qDebug() << "----------------------";
}*/

/*
qDebug() << endl << "Format read test (QtGrace old universal guessing (slow)):";
for (int i=0;i<types;i++)
{
ret_val=guess_date_or_number(formats_v[i],TRUE,&jul222);
qDebug() << (ret_val<0?"?":" ") << i << " " << formats_v[i] << " guessed_format=" << ret_val << "val=" << jul222;
}

qDebug() << endl << "Format read test (QtGrace new guessing (strtod)):";
for (int i=0;i<types;i++)
{
read_one_number(formats_v[i],&ret_val,&jul222);
qDebug() << (ret_val<0?"?":" ") << i << " " << formats_v[i] << " guessed_format=" << ret_val << " val=" << jul222;
}
qDebug() << endl;

qDebug() << "Degree Lat.-Test:";
strcpy(formats_v[28],"65.4 N");//Power
ret_val=read_date_or_number_in_format(formats_v[28], 28, TRUE, &jul222);
qDebug() << 28 << " " << formats_v[28] << " success=" << ret_val << " val=" << jul222;
strcpy(formats_v[28],"65.4W");//Power
ret_val=read_date_or_number_in_format(formats_v[28], 28, TRUE, &jul222);
qDebug() << 28 << " " << formats_v[28] << " success=" << ret_val << " val=" << jul222;
strcpy(formats_v[28],"65.4      S");//Power
ret_val=read_date_or_number_in_format(formats_v[28], 28, TRUE, &jul222);
qDebug() << 28 << " " << formats_v[28] << " success=" << ret_val << " val=" << jul222;
*/

/*
double date_read_val;
int date_read_ret;
char * text_s=new char[512];
strcpy(text_s," \t\n 13.2 E\t \n ");
char * text_s2=new char[512];
strcpy(text_s2,text_s);
*/

/*
remove_first_last_whitespaces(text_s,&text_s2);
cout << "text_s =#" << text_s << "#" << endl;
cout << "text_s2=#" << text_s2 << "#" << endl;

date_read_ret=read_date_or_number_in_format(text_s2,FORMAT_DEGREESLON,1,&date_read_val);
cout << "date_read_ret=" << date_read_ret << " date_read_val=" << date_read_val << endl;
strcpy(text_s,"13.2W");
remove_first_last_whitespaces(text_s,&text_s2);
cout << "text_s =#" << text_s << "#" << endl;
cout << "text_s2=#" << text_s2 << "#" << endl;
date_read_ret=read_date_or_number_in_format(text_s2,FORMAT_DEGREESLON,1,&date_read_val);
cout << "date_read_ret=" << date_read_ret << " date_read_val=" << date_read_val << endl;

cout << "Engineering test:" << endl;
strcpy(text_s,"13.2m");
remove_first_last_whitespaces(text_s,&text_s2);
date_read_ret=read_date_or_number_in_format(text_s2,FORMAT_ENGINEERING,1,&date_read_val);
cout << "Input=" << text_s << " date_read_ret=" << date_read_ret << " date_read_val=" << date_read_val << endl;

strcpy(text_s,"13.2mk");
remove_first_last_whitespaces(text_s,&text_s2);
date_read_ret=read_date_or_number_in_format(text_s2,FORMAT_ENGINEERING,1,&date_read_val);
cout << "Input=" << text_s << " date_read_ret=" << date_read_ret << " date_read_val=" << date_read_val << endl;

strcpy(text_s,"13.2");
text_s[4]=char(181);
text_s[5]='\0';
remove_first_last_whitespaces(text_s,&text_s2);
date_read_ret=read_date_or_number_in_format(text_s2,FORMAT_ENGINEERING,1,&date_read_val);
cout << "Input=" << text_s << " date_read_ret=" << date_read_ret << " date_read_val=" << date_read_val << endl;
*/

/*int doy,ye,mont,da;

ye=2017;
for (int i=0;i<=367;i++)
{
doy=i;
date_read_ret=gregorian_dayofyear_to_monthday(doy,ye,&mont,&da);
qDebug() << i << ":" << da << "." << mont << "." << ye << "(" << date_read_ret << ")";
}*/

/*strcpy(formats_v[0],"10:23:04.243");
strcpy(formats_v[1],"23:59:61.3");
strcpy(formats_v[2],"1:5:2");
strcpy(formats_v[3],"08: 02: 04.78");
strcpy(formats_v[4],"7:78:1e1");
strcpy(formats_v[5],"12:-65.5:10");
strcpy(formats_v[6],"-8:1:50");
strcpy(formats_v[7],"08:45:13.8");*/

/*qDebug() << endl << "Teste FORMAT_HMS:";
for (int i=0;i<8;i++)
{
date_read_ret=read_date_or_number_in_format(formats_v[i],FORMAT_HMS,1,&date_read_val);
/// qDebug() << "TEST" << formats_v[i] << "ret=" << date_read_ret << "gelesener Wert=" << date_read_val;
}*/

/*int max_lines_to_read=45;
struct block_input_format block1;
init_block_input_data(&block1);//to prepare the memory
qDebug() << endl << "Guessing: error_count=" << guess_block_data_column_format("D:\\old_data\\qtgrace_w_undo\\test_dataformat_3.txt",block1,max_lines_to_read);
qDebug() << " NrOfSets=" << block1.nr_of_sets;
for (int i=0;i<block1.nr_of_sets;i++)
{
qDebug() << "Set" << i << "StartAtLine=" << block1.set_infos[i].start_row << "Columns=" << block1.set_infos[i].nr_of_columns << "Rows=" << block1.set_infos[i].nr_of_rows;
    for (int j=0;j<block1.set_infos[i].nr_of_columns;j++)
    {
        if (block1.set_infos[i].column_format[j]>=0)
        qDebug() << "    Column[" << j << "]=" << get_format_option_item_label(block1.set_infos[i].column_format[j]) << "(" << block1.set_infos[i].column_format[j] << ")";
        else
        qDebug() << "    Column[" << j << "]= INVALID" << "(" << block1.set_infos[i].column_format[j] << ")";
    }
}

ofstream ofi23;
if (block1.nr_of_sets>0)
{
ofi23.open("D:\\old_data\\qtgrace_w_undo\\test_dataformat_3a.html");
ofi23 << block1.set_infos[0].read_results_text.toLocal8Bit().constData();
ofi23.close();
}
if (block1.nr_of_sets>1)
{
ofi23.open("D:\\old_data\\qtgrace_w_undo\\test_dataformat_3b.html");
ofi23 << block1.set_infos[1].read_results_text.toLocal8Bit().constData();
ofi23.close();
}*/

/*
qDebug() << endl << "Teste FORMAT_DAYMONTH:";
for (int i=0;i<8;i++)
{
date_read_ret=read_date_or_number_in_format(formats_v[i],FORMAT_DAYMONTH,1,&date_read_val);
qDebug() << formats_v[i] << "ret=" << date_read_ret << "gelesener Wert=" << date_read_val;
}
qDebug() << endl << "Teste FORMAT_MONTHSY:";
for (int i=0;i<8;i++)
{
date_read_ret=read_date_or_number_in_format(formats_v[i],FORMAT_MONTHSY,1,&date_read_val);
qDebug() << formats_v[i] << "ret=" << date_read_ret << "gelesener Wert=" << date_read_val;
}
qDebug() << endl;
*/

/*
strcpy(formats_v[8],"12:-5.5:10");
date_read_ret=read_date_or_number_in_format(formats_v[8],FORMAT_HMS,1,&date_read_val);
qDebug() << "TEST" << formats_v[8] << "ret=" << date_read_ret << "gelesener Wert=" << date_read_val;

qDebug() << "Teste Lat-Degrees (N/S)";
for (int i=4;i<11;i++)
{
date_read_ret=read_date_or_number_in_format(formats_v[i],FORMAT_MMSSLAT,1,&date_read_val);
qDebug() << formats_v[i] << "ret=" << date_read_ret << "gelesener Wert=" << date_read_val;
}*/

/*
delete[] text_s;
delete[] text_s2;
*/

/*
for (int i=0;i<types;i++)
delete[] formats_v[i];
delete[] formats_v;

for (int i=0;i<types;i++)
delete[] formats_v2[i];
delete[] formats_v2;
*/

/*
show_text_file_widget * stfw=new show_text_file_widget(0);
stfw->init("D:\\old_data\\qtgrace_w_undo\\test_dataformat.txt",10);
stfw->show();

ifstream ifi12;
ifi12.open("D:\\old_data\\qtgrace_w_undo\\test_dataformat_3.txt");
char ** tokens;
int token_cnt,*formats_guessed=NULL;
double *col_dat_val=NULL;
for (int j=0;j<10;j++)
{
date_read_ret=read_line_and_split_in_tokens(ifi12,&tokens,&token_cnt);
if (formats_guessed!=NULL) delete[] formats_guessed;
if (col_dat_val!=NULL) delete[] col_dat_val;
formats_guessed=new int[2+token_cnt];
col_dat_val=new double[2+token_cnt];
qDebug() << "Read=" << date_read_ret << "Columns=" << token_cnt;
    for (int i=0;i<token_cnt;i++)
    {
    formats_guessed[i]=guess_date_or_number(tokens[i],TRUE,col_dat_val+i);
    qDebug() << i<< ": format=" << formats_guessed[i] << "value=" << col_dat_val[i];
    }
}
if (formats_guessed!=NULL) delete[] formats_guessed;
if (col_dat_val!=NULL) delete[] col_dat_val;
ifi12.close();*/

//class custom_number_format cust_form1;

/*QString s_string;
s_string.clear();
cust_form1.add_single_token(FORMAT_TOKEN_FORMAT_INT,FORMAT_TOKEN_TYPE_DAY,2,s_string);
s_string=QString(".");
cust_form1.add_single_token(FORMAT_TOKEN_FORMAT_INT,FORMAT_TOKEN_TYPE_MONTH,2,s_string);
cust_form1.add_single_token(FORMAT_TOKEN_FORMAT_INT,FORMAT_TOKEN_TYPE_YEAR,4,s_string);
s_string=QString(" / ");
cust_form1.add_single_token(FORMAT_TOKEN_FORMAT_INT,FORMAT_TOKEN_TYPE_HOUR,2,s_string);
s_string=QString(":");
cust_form1.add_single_token(FORMAT_TOKEN_FORMAT_INT,FORMAT_TOKEN_TYPE_MINUTE,2,s_string);
cust_form1.add_single_token(FORMAT_TOKEN_FORMAT_DOUBLE,FORMAT_TOKEN_TYPE_SECOND,3,s_string);
*/

/*
time_t time_value;
struct tm * timeinfo;
(void) time(&time_value);
timeinfo = localtime(&time_value);
double cur_time=cal_and_time_to_jul(timeinfo->tm_year+1900,timeinfo->tm_mon,timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);

QString test_String("16.08.2017;14:58:02;");
cust_form1.decimal_separator=QString(".");
cust_form1.add_number_token(FORMAT_TOKEN_TYPE_DAY,2);
cust_form1.add_text_token(QString("."));
cust_form1.add_number_token(FORMAT_TOKEN_TYPE_MONTH,2);
cust_form1.add_text_token(QString("."));
cust_form1.add_number_token(FORMAT_TOKEN_TYPE_YEAR,4);
cust_form1.add_text_token(QString(";"));
cust_form1.add_number_token(FORMAT_TOKEN_TYPE_HOUR,2);
cust_form1.add_text_token(QString(":"));
cust_form1.add_number_token(FORMAT_TOKEN_TYPE_MINUTE,2);
cust_form1.add_text_token(QString(":"));
cust_form1.add_number_token(FORMAT_TOKEN_TYPE_SECOND,2);
cust_form1.add_text_token(QString(";"));

QString format_text;
cust_form1.print_number_in_format(format_text,cur_time);
qDebug() << endl;
qDebug() << "Write-Test:";
qDebug() << "Current Date and Time=" << format_text;

double test_result;
int result=cust_form1.read_numbers_from_text(test_String,&test_result);
qDebug() << endl;
qDebug() << "Read-Test: testString=" << test_String;
qDebug() << "Result=" << result << "read_value=" << test_result;
*/

/*
QString test_upper_command[5];
test_upper_command[0]=QString("test G0.S0N legend \"neu\\\" test\" legend");
test_upper_command[1]=QString("TEST 2 G1.S14.x TeST2");
test_upper_command[2]=QString("\"Legend 1 \\s #Special\" #Special");
test_upper_command[3]=QString("legend 1 \"testA TestB\" legend 2 \"TestC TestD\" legend3");
qDebug() << endl << "Convert To Upper - special" << endl;
for (int i=0;i<4;i++)
{
test_upper_command[4]=test_upper_command[i];
formula_to_process::convertStringToUpper(test_upper_command[4]);
qDebug() << "Original="<< test_upper_command[i] << endl << "Converted=" << test_upper_command[4];
}
qDebug() << "End Convert Upper" << endl;
*/

/*
QString test_ifs[4];
test_ifs[0]=QString("#if[S#=1]x=x+5");
test_ifs[1]=QString("#If[2+a1=G0.S0.X[0]]   S0N.Y=S0N.Y*2.3");
test_ifs[2]=QString("    #IF[   S#%5<>0] y=X*3");
test_ifs[3]=QString("#if[5-4=1] #if[a2=0] G0.S0 legend \"TestFile\"");
qDebug() << endl << "If-Statement-Test:";
int test_result,test_return;
char test_if_statement[1024];
char test_command[512];
for (int i=0;i<4;i++)
{
strcpy(test_if_statement,test_ifs[i].toLocal8Bit().constData());
test_return=startIfStatement(test_if_statement,test_result,test_command);
qDebug() << "Return=" << test_return << "Result=" << test_result << "Command=" << test_command << endl;
}
*/

/*
importSettings test_import_setting;
QStringList test_match_files;
QString search_match_file_test("D:\\old_data\\qtgrace_w_undo\\Compressor_Decompressor\\Binary_Example\\multiple_files_example1\\1.mprz");
QFileInfo test_file_info;
QString test_file_search_extension;
int match_result,match_type;
initSettings(test_import_setting,false);
bool test_search_for_header=true;
for (int k=0;k<4;k++)
{
qDebug() << "---------------------------------------------------------------------------------------------";
    if (k==1)
    {
    search_match_file_test=QString("D:\\old_data\\qtgrace_w_undo\\Compressor_Decompressor\\Binary_Example\\multiple_files_example1\\Legend.ini");
    test_search_for_header=false;
    }
    else if (k==2)
    {
    search_match_file_test=QString("D:\\old_data\\qtgrace_w_undo\\Compressor_Decompressor\\Binary_Example\\multiple_files_example2\\3.mprz");
    test_search_for_header=true;
    }
    else if (k==3)
    {
    search_match_file_test=QString("D:\\old_data\\qtgrace_w_undo\\Compressor_Decompressor\\Binary_Example\\multiple_files_example3_wrong\\3.mprz");
    test_search_for_header=true;
    }
test_file_info.setFile(search_match_file_test);
test_file_search_extension=test_file_info.suffix();
qDebug() << endl << "We search for " << search_match_file_test << " (" << test_file_search_extension << ")" << endl;
for (int i=0;i<nr_of_std_bin_import_settings;i++)
{
copy_import_settings(std_bin_import_settings+i,&test_import_setting);
    if (test_search_for_header)
    test_import_setting.DataFile=search_match_file_test;
    else
    test_import_setting.HeaderFile=search_match_file_test;
qDebug() << i+1 << "/" << nr_of_std_bin_import_settings << ": Settings-Name=" << test_import_setting.name;
qDebug() << "Search-suffix=" << test_file_search_extension << "Format-suffix=" << test_import_setting.DataSuffix;
    if ((test_search_for_header && test_file_search_extension==test_import_setting.DataSuffix) || (!test_search_for_header && test_file_search_extension==test_import_setting.HeaderSuffix))
    {
    qDebug() << "Data-File-Extension DOES match";
    }
    else
    {
    qDebug() << "Data-File-Extension does NOT match";
    continue;
    }

match_result=find_matching_files(test_import_setting,test_search_for_header,test_match_files,match_type);
if (match_result==RETURN_SUCCESS)
{
qDebug() << "MATCH FOUND: " << test_match_files.length() << test_match_files << "Match-Type=" << match_type;
}
else
{
qDebug() << "NO SUITABLE MATCH FOUND";
}
qDebug() << endl;
}

}
*/
/// simple data extraction from text-test
/*QString test_str[5];
bool extract_ok;
double extract_val;
test_str[0]=QString("3.1415927");
test_str[1]=QString("   3.1415927 test");
test_str[2]=QString("Message 1");
test_str[3]=QString("Test Text -5.98e2 Ende");
test_str[4]=QString("");
qDebug() << "START simple extraction test";
for (int i=0;i<5;i++)
{
extract_val=read_first_number_from_text(test_str[i],extract_ok);
qDebug() << test_str[i] << "-->" << extract_val << "(" << extract_ok << ")";
}
qDebug() << "END simple extraction test";*/

/// read-csv-file-test
/*
#ifdef MAC_SYSTEM
const char * testfilesfolder="/Users/andreaswinter/C_Programme/qtgrace_w_undo/testfiles/";//Mac OS X (iMac)
//const char * testfilesfolder="/Users/andreaswinter/Desktop/qtgrace_w_undo/testfiles/";//Mac OS X (Mac Pro)
//const char * testfilesfolder="/Users/andwin/qtgrace_w_undo/testfiles/";//Mac OS X(Laptop)
#else
const char * testfilesfolder="D:\\old_data\\qtgrace_w_undo\\";//Windows 7
#endif

struct csv_import_infos info_test;
info_test.filename=new char[1024];
strcpy(info_test.filename,testfilesfolder);
strcat(info_test.filename,"test_dataformat2.txt");///[34r x 2c --> 1 set] format: ["HH:MM:SS" | General]' '
//strcat(info_test.filename,"test_dataformat_1.txt");///[34r x 2c --> 1 set] format: ["HH:MM:SS" | General]' '
//strcat(info_test.filename,"test_dataformat_2.txt");///[34r x 2c --> 1 set] format: ["HH:MM:SS" | General]\t
//strcat(info_test.filename,"test_dataformat_3.txt");///[17r x 3c + 18r x 2c --> 3 sets] format: [HH:MM:SS | "DD MM' (lon)" | General]' ', danach zweites Set mit [HH:MM:SS | General]\t, es gibt viele Comments!
//strcat(info_test.filename,"test_dataformat.txt");///[34r x 2c --> 1 set] format: [HH:MM:SS | General]\t
//strcat(info_test.filename,"test_dataformat_orig.txt");///[34r x 2c --> 1 set] format: [HH:MM:SS | General]\t --> wie ...2.txt ohne " / identisch zu dem File ohne Ende (test_dataformat.txt)
//strcat(info_test.filename,"test_dataformat_error.txt");///[34r x 2c --> 1 set] format: [HH:MM:SS | General]\t --> wie ...2.txt, aber ein Eintrag ist falsch (falsches Format)
//strcat(info_test.filename,"test_dataformat_4.txt");///[34r x 2c --> 1 set (first string-col auto-imported as string-column)] format: [HH:MM:SS | String | General]\t
//strcat(info_test.filename,"test_dataformat_5.txt");///[34r x 2c --> 1 set] format: [General | General] ',' / ';' (completely different separators than usual)
//strcat(info_test.filename,"test_dataformat_error2.txt");///[34r x 2c --> 1 set] format: [HH:MM:SS | General]\t --> wie ...2.txt, aber drei Eintraege sind falsch (falsches Format)
//strcat(info_test.filename,"test_dataformat7.txt");//[12r x 2c --> 1 set] format: [Engineering | "DD MM' (lon)"]' '-Mac-line-ending
//strcat(info_test.filename,"test_dataformat8.txt");//[66r x 8c --> 1 set] format: [General | ... | General]' Windows-Line-Ending (Zufallszahlen, mit ',' als Dezimalseparator)
//strcat(info_test.filename,"test_dataformat9.txt");//[66r x 8c --> 1 set] format: [General | ... | General]' Windows-Line-Ending (Zufallszahlen, mit '.' als Dezimalseparator)
*/

/*info_test.headerlines=0;
info_test.datsep=';';//'\n';
info_test.colsep=',';//'\t';//;' '
info_test.invalid_value=-10;*/

/*qDebug() << "ASCII-Werte:";
qDebug() << "tab  =" << (int)'\t';
qDebug() << "LF   =" << (int)'\n';
qDebug() << "CR   =" << (int)'\r';
qDebug() << "space=" << (int)' ';
qDebug() << ";    =" << (int)';';
qDebug() << ",    =" << (int)',';

qDebug() << "----------------------------------------";*/

/*
qDebug() << "FileReadTest:";
int test_lines;
char test_guessed_line_sep;
QList<int> separate_positions1,separate_positions2;
QList<char> guessed_separator;
int ret;
*/

/*ret=read_and_guess_csv_file(info_test.filename,test_guessed_line_sep,separate_positions1,separate_positions2,guessed_separator,test_lines,1);
qDebug() << "Reading and guessing file:" << info_test.filename;
qDebug() << "Number_of_lines=" << test_lines;
qDebug() << "Line-Separator=" << ((int)(test_guessed_line_sep));
qDebug() << "Number_of_sets=" << separate_positions1.length();
    for (int i=0;i<separate_positions1.length();i++)
    {
    qDebug() << "Set" << i << "von" << separate_positions1.at(i) << "bis" << separate_positions2.at(i) << "Column_separator=" << ((int)guessed_separator.at(i));
    }
qDebug() << "----------------------------------------";

info_test.datsep=test_guessed_line_sep;
info_test.colsep=guessed_separator.at(0);

qDebug() << endl << "Read into csv-Tokens:";
ret=read_csv_file_into_tokens(info_test,true);
qDebug() << "Read File:" << info_test.filename << "ret=" << ret;
qDebug() << "Zeilen=" << info_test.rows << "Spalten=" << info_test.columns;
qDebug() << "StartAt=" << info_test.csv_read_started << "StopAt=" << info_test.csv_read_stopped;
qDebug() << "Errors=" << info_test.number_of_errors;
if (info_test.number_of_errors>0)
{
qDebug() << "Error-Positions:";
for (int i=0;i<info_test.error_pos1.length();i++)
qDebug() << info_test.error_pos1.at(i) << "|" << info_test.error_pos2.at(i);
}
qDebug() << "Guessed Column-Formats=";
for (int i=0;i<info_test.csv_column_formats.length();i++)
{
if (info_test.csv_column_formats.at(i)==FORMAT_STRING)
qDebug() << info_test.csv_column_formats.at(i) << " String";
else if (info_test.csv_column_formats.at(i)==FORMAT_INVALID || info_test.csv_column_formats.at(i)>FORMAT_YYDYHMS)
qDebug() << info_test.csv_column_formats.at(i) << " INVALID";
else
qDebug() << info_test.csv_column_formats.at(i) << " " << fmt_option_items[info_test.csv_column_formats.at(i)].label;
}

QString result_html=convert_csv_contents_to_html(info_test,0,-1,2,true);
QString result_html2=result_html+QString("<p> \n </p>")+result_html;
QString error_html=convert_csv_errors_to_html(info_test);
QString error_html2;
QString error_text=convert_csv_errors_to_text(info_test);
QString error_text2;

QTextEdit * texed=new QTextEdit(0);
QTextEdit * texed2=new QTextEdit(0);
QTextEdit * texed3=new QTextEdit(0);
texed->setReadOnly(true);
texed2->setReadOnly(true);*/

/*texed->setHtml(result_html2);
texed->show();*/

/*
qDebug() << endl << "----------------------------------------";
*/

/*
struct csv_multi_import_infos m_info_test;
init_mcsv_import_infos(&m_info_test);
m_info_test.filename=new char[1024];
strcpy(m_info_test.filename,info_test.filename);
//qDebug() << "single_filename=" << info_test.filename << "multi_filename=" << m_info_test.filename;
/// AUTO-LOAD-FILE
qDebug() << endl << "BEFORE AUTOLOADER";
ret=autoload_csv_file(m_info_test,true);//The new (silent) auto-loader
qDebug() << "AFTER AUTOLOADER" << endl;
/// FINISHED AUTOLOADER
*/

/*
ret=read_csv_file_into_sets(m_info_test,true);

result_html2=convert_csv_contents_to_html(m_info_test,0,17,2,false);
error_html2=convert_csv_errors_to_html(m_info_test);
error_text2=convert_csv_errors_to_text(m_info_test);
texed->setHtml(result_html2);
texed2->setHtml(error_html2);
texed3->setText(QString("TEXT-VERSION DER ERRORS:\n\n")+error_text2);
texed->show();
texed2->show();
texed3->show();
qDebug() << "Comments:";
for (int i=0;i<m_info_test.nr_of_sets;i++)
{
qDebug() << "Set " << i << "; number_of_comments=" << m_info_test.set[i].csv_comments.length();
    for (int j=0;j<m_info_test.set[i].csv_comments.length();j++)
    {
    qDebug() << m_info_test.set[i].csv_comment_line_nr.at(j) << "; end=" << m_info_test.set[i].csv_comment_at_end.at(j) << ";" << m_info_test.set[i].csv_comments.at(j);
    }
}
qDebug() << "----------------------------------------";
double * new_cols=new double[m_info_test.set[0].rows];
QStringList slist;
int read_rows=get_csv_string_column(m_info_test,0,2,slist);
qDebug() << "StringColumn=" << slist;
*/

/*int read_rows=get_csv_column(m_info_test.set[0],2,new_cols);
qDebug() << "read_rows=" << read_rows;
    for (int i=0;i<read_rows;i++)
    {
    qDebug() << new_cols[i];
    }*/
//qDebug() << "----------------------------------------";

/*postProcessingSettings * postproset=new postProcessingSettings(0);
postproset->init(3,m_info_test);
postproset->show();*/

/*delete[] info_test.filename;*/

//delete[] m_info_test.filename;
//delete[] new_cols;
//multiFileSelector * mfsel=new multiFileSelector(0);
//mfsel->show();

/*qDebug() << "Logical  DPI X=" << qApp->desktop()->logicalDpiX();
qDebug() << "Physical DPI X=" << qApp->desktop()->physicalDpiX();

qDebug() << "Logical  DPI Y=" << qApp->desktop()->logicalDpiY();
qDebug() << "Physical DPI Y=" << qApp->desktop()->physicalDpiY();*/

/*DataFormatSelector * format_sel1=new DataFormatSelector(0);
format_sel1->setText(QObject::tr("Hoizontal Selection:"));
format_sel1->setOrientation(0);
DataFormatSelector * format_sel2=new DataFormatSelector(0);
format_sel2->setText(QObject::tr("Vertical Selection:"));
format_sel2->setOrientation(1);
format_sel1->show();
format_sel2->show();*/

/*int test_sets=3;
int * test_nr_of_columns=new int[test_sets];
int * test_set_data=new int[test_sets];
test_nr_of_columns[0]=4;
test_nr_of_columns[1]=1;
test_nr_of_columns[2]=2;
test_set_data[0]=36;
test_set_data[1]=112;
test_set_data[2]=17;
MultiDataFormatSelector * mult_format_sel=new MultiDataFormatSelector(0);
mult_format_sel->reCreateSelectors(test_sets,test_nr_of_columns,test_set_data);
mult_format_sel->show();
ColumnTargetSelector * ColTargetSel=new ColumnTargetSelector(0);
ColTargetSel->resetColumnCount(4);
ColTargetSel->show();*/

/*QPixmap pixm1=messageBoxStdIcon(QMessageBox::Question,200);
QPixmap pixm2=messageBoxStdIcon(QMessageBox::Critical,200);
QPixmap pixm3=messageBoxStdIcon(QMessageBox::Warning,200);
QPixmap pixm4=messageBoxStdIcon(QMessageBox::Information,200);
QLabel * lblPix1=new QLabel(QString(""),0);
QLabel * lblPix2=new QLabel(QString(""),0);
QLabel * lblPix3=new QLabel(QString(""),0);
QLabel * lblPix4=new QLabel(QString(""),0);
lblPix1->setPixmap(pixm1);
lblPix2->setPixmap(pixm2);
lblPix3->setPixmap(pixm3);
lblPix4->setPixmap(pixm4);
lblPix1->show();
lblPix1->raise();
lblPix2->show();
lblPix2->raise();
lblPix3->show();
lblPix3->raise();
lblPix4->show();
lblPix4->raise();*/

/*
bailoutQuestion * bq1=new bailoutQuestion(0);
int ba_answer=bq1->askBailout(true,true,false);
qDebug() << "ba_answer=" << ba_answer;// << "gen_ret=" << bq1->general_return;
*/

/*QString test_strings[8];
QStringList test_string_results;
QChar sep_c(',');
test_strings[0]=QString("Test,100,200,300");
test_strings[1]=QString(",20,Test,40,");
test_strings[2]=QString(",");
test_strings[3]=QString("");
test_strings[4]=QString("Es,\"wird, Zeit\",jetzt");
test_strings[5]=QString("Test1");
test_strings[6]=QString("Test;100,200;300");
test_strings[7]=QString("Test,,,,200,300,\"test\"");
qDebug() << "Separation-Test:";
for (int i=0;i<8;i++)
{
splitStringIntoSubstrings(test_strings[i],sep_c,test_string_results);
qDebug() << i << "string=" << test_strings[i] << "NrOfEntries=" << test_string_results.length() << "results=" << test_string_results;
}*/

/*QString quote_text_test("Test1,Test2,Test3,\"Test4,Test5\",Test6");
int result_pos=pos_in_text_with_quotes(quote_text_test,QString(","),2,0);
qDebug() << "quote_text_test=" << quote_text_test;
for (int i=0;i<7;i++)
{
qDebug() << i << ":" << process_a_text(quote_text_test,i,QString(","),2,QString(","),2,true);
}*/

/*qDebug() << "WITH QUOTES";
qDebug() << "Start at 0: count=" << count_in_text_with_quotes(quote_text_test,QString(","),0);
for (int i=0;i<6;i++)
{
result_pos=pos_in_text_with_quotes(quote_text_test,QString(","),i,0);
qDebug() << "index=" << i << " --> pos=" << result_pos;
}
qDebug() << "Start at 10: count=" << count_in_text_with_quotes(quote_text_test,QString(","),10);
for (int i=0;i<6;i++)
{
result_pos=pos_in_text_with_quotes(quote_text_test,QString(","),i,10);
qDebug() << "index=" << i << " --> pos=" << result_pos;
}
qDebug() << "Start at 20: count=" << count_in_text_with_quotes(quote_text_test,QString(","),20);
for (int i=0;i<6;i++)
{
result_pos=pos_in_text_with_quotes(quote_text_test,QString(","),i,20);
qDebug() << "index=" << i << " --> pos=" << result_pos;
}

qDebug() << "WITHOUT QUOTES";
qDebug() << "Start at 0: count=" << count_in_text_without_quotes(quote_text_test,QString(","),0);
for (int i=0;i<6;i++)
{
result_pos=pos_in_text_without_quotes(quote_text_test,QString(","),i,0);
qDebug() << "index=" << i << " --> pos=" << result_pos;
}
qDebug() << "Start at 10: count=" << count_in_text_without_quotes(quote_text_test,QString(","),10);
for (int i=0;i<6;i++)
{
result_pos=pos_in_text_without_quotes(quote_text_test,QString(","),i,10);
qDebug() << "index=" << i << " --> pos=" << result_pos;
}
qDebug() << "Start at 20: count=" << count_in_text_without_quotes(quote_text_test,QString(","),20);
for (int i=0;i<6;i++)
{
result_pos=pos_in_text_without_quotes(quote_text_test,QString(","),i,20);
qDebug() << "index=" << i << " --> pos=" << result_pos;
}*/

/*END OF TEST AREA*/


stop_repaint=FALSE;
mainWin->mainArea->completeRedraw();
mainWin->redisplayIcons();
mainWin->ManageBars();
mainWin->move(initial_x_pos,initial_y_pos);

//the following workaround seems to be necessary on some linux-systems !?
#ifdef LINUX_SYSTEM
mainWin->scroll->hide();
a->processEvents();
mainWin->scroll->show();
a->processEvents();
#endif

//qDebug() << "C autofit_pending=" << autofit_pending;

if (autofit_pending==1)
{
mainWin->doFitPage();
autofit_pending=0;
}

/*
qDebug() << "MainArea.scroll.size=" << mainWin->mainArea->scroll->size();
qDebug() << "MainArea.lblBackgr.size=" << mainWin->mainArea->lblBackGr->size();
*/

int execVal=a->exec();//actually do the main application loop
delete a;
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

void updateGraphAppearance(int gno)
{
if (FormGraphAppearance==NULL) return;//no graph appearance dialog open
int nr_of_graphs;
int * gr_nos=new int[2];
FormGraphAppearance->flp->listGraph->get_selection(&nr_of_graphs,&gr_nos);
    if (nr_of_graphs<1 || gr_nos[0]!=gno)//graph-appearance does not show graph (gno)
    {
    delete[] gr_nos;
    return;
    }
FormGraphAppearance->flp->show_graph_data_external(gno);
delete[] gr_nos;
}

void updateSetAppearance(int gno,int set)
{
if (FormSetAppearance==NULL) return;//no set appearance dialog open
if (FormSetAppearance->flp->listSet->gr_no!=gno || FormSetAppearance->flp->cset!=set) return;//set appearance does not show set (set) from graph (gno)
//maybe we have to save the current selection??
FormSetAppearance->flp->showSetData(gno,set);
}

void updateObjectData(int type,int id)
{
    switch (type)
    {
    case OBJECT_BOX:
        if (EditBoxProps==NULL) return;
        if (EditBoxProps->flp->obj_id!=id) return;
        EditBoxProps->init(id);
    break;
    case OBJECT_ELLIPSE:
        if (EditEllipseProps==NULL) return;
        if (EditEllipseProps->flp->obj_id!=id) return;
        EditEllipseProps->init(id);
        break;
    case OBJECT_LINE:
        if (EditLineProps==NULL) return;
        if (EditLineProps->flp->obj_id!=id) return;
        EditLineProps->init(id);
    break;
    case OBJECT_STRING:
        if (EditTextProps==NULL) return;
        if (EditTextProps->flp->obj_id!=id) return;
        EditTextProps->init(id);
    break;
    }
}

void updateAxisProps(int gno,int axis)
{
if (FormAxisProperties==NULL) return;
if (FormAxisProperties->flp->curaxis!=axis || gno!=get_cg()) return;
FormAxisProperties->create_axes_dialog(axis);
}

void updatePlotAppearance(void)
{
if (FormPlotAppearance==NULL) return;//no plot appearance dialog open
FormPlotAppearance->init();//show current set of plot
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
        if (dummy!=NULL)
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
        if (dummy!=NULL)
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
        if (dummy!=NULL)
        delete[] dummy;//delete original entries
    }
    plist[nplist]=setSel;
    nplist++;
}

void updateUniLists(void)
{
QFont f1=qApp->font();
QFont f2;
for (int i=0;i<ngraph_selectors;i++)
{
f2=graph_selectors[i]->font();
    if (f1!=f2)
    {
    graph_selectors[i]->setFont(f1);
    //graph_selectors[i]->update_number_of_entries_preserve_selection();
    }
}
for (int i=0;i<nset_selectors;i++)
{
f2=set_selectors[i]->font();
    if (f1!=f2)
    {
    set_selectors[i]->setFont(f1);
    //set_selectors[i]->update_number_of_entries_preserve_selection();
    }
}
for (int i=0;i<nplist;i++)
{
f2=plist[i]->font();
    if (f1!=f2)
    {
    plist[i]->setFont(f1);
    //plist[i]->update_number_of_entries_preserve_selection();
    }
}
}

void changeDragInUniLists(void)
{
for (int i=0;i<ngraph_selectors;i++)
{
    if (allow_drag_in_lists==TRUE)
    {/// experimental
    graph_selectors[i]->setDragEnabled(true);
    graph_selectors[i]->viewport()->setAcceptDrops(true);
    graph_selectors[i]->setDropIndicatorShown(true);
    graph_selectors[i]->setDragDropMode(QAbstractItemView::InternalMove);
    }
    else
    {
    graph_selectors[i]->setDragEnabled(false);
    graph_selectors[i]->viewport()->setAcceptDrops(false);
    graph_selectors[i]->setDropIndicatorShown(false);
    graph_selectors[i]->setDragDropMode(QAbstractItemView::NoDragDrop);
    }
}
for (int i=0;i<nset_selectors;i++)
{
    if (allow_drag_in_lists==TRUE)
    {/// experimental
    set_selectors[i]->setDragEnabled(true);
    set_selectors[i]->viewport()->setAcceptDrops(true);
    set_selectors[i]->setDropIndicatorShown(true);
    set_selectors[i]->setDragDropMode(QAbstractItemView::InternalMove);
    }
    else
    {
    set_selectors[i]->setDragEnabled(false);
    set_selectors[i]->viewport()->setAcceptDrops(false);
    set_selectors[i]->setDropIndicatorShown(false);
    set_selectors[i]->setDragDropMode(QAbstractItemView::NoDragDrop);
    }
}
for (int i=0;i<nplist;i++)
{
    if (allow_drag_in_lists==TRUE)
    {/// experimental
    plist[i]->setDragEnabled(true);
    plist[i]->viewport()->setAcceptDrops(true);
    plist[i]->setDropIndicatorShown(true);
    plist[i]->setDragDropMode(QAbstractItemView::InternalMove);
    }
    else
    {
    plist[i]->setDragEnabled(false);
    plist[i]->viewport()->setAcceptDrops(false);
    plist[i]->setDropIndicatorShown(false);
    plist[i]->setDragDropMode(QAbstractItemView::NoDragDrop);
    }
}
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
        if (dummy!=NULL)
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
        if (dummy!=NULL)
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
        if (dummy!=NULL)
        delete[] dummy;//delete original entries (only addresses, no real stdLineEdit-ors)
    }
    LaTeX_Lines[nr_of_latex_lines]=line;
    nr_of_latex_lines++;
}

void add_non_LaTeX_Line(stdLineEdit * line)
{
    if (nr_of_non_latex_lines==n_of_non_latex_lines_alloc)
    {
        stdLineEdit ** dummy=Non_LaTeX_Lines;
        n_of_non_latex_lines_alloc+=5;
        Non_LaTeX_Lines=new stdLineEdit*[n_of_non_latex_lines_alloc];
            for (int i=0;i<nr_of_non_latex_lines;i++)
            Non_LaTeX_Lines[i]=dummy[i];
        if (dummy!=NULL)
        delete[] dummy;//delete original entries (only addresses, no real stdLineEdit-ors)
    }
    Non_LaTeX_Lines[nr_of_non_latex_lines]=line;
    nr_of_non_latex_lines++;
}

void remove_Line(stdLineEdit * line)
{
    for (int i=0;i<nr_of_non_latex_lines;i++)
    {
        if (Non_LaTeX_Lines[i]==line)
        {
            for (int j=i+1;j<nr_of_non_latex_lines;j++)
            {
            Non_LaTeX_Lines[j-1]=Non_LaTeX_Lines[j];
            }
            nr_of_non_latex_lines--;
        }
    }
    for (int i=0;i<nr_of_latex_lines;i++)
    {
        if (LaTeX_Lines[i]==line)
        {
            for (int j=i+1;j<nr_of_latex_lines;j++)
            {
            LaTeX_Lines[j-1]=LaTeX_Lines[j];
            }
            nr_of_latex_lines--;
        }
    }
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
        if (dummy!=NULL)
        delete[] dummy;//delete original entries (only addresses, no real stdLineEdit-ors)
    }
    line_style_selectors[nr_of_line_style_selectors]=line;
    nr_of_line_style_selectors++;
}

void add_Slider(stdSlider * slider)
{
    if (nr_of_sliders==nr_of_sliders_alloc)
    {
        stdSlider ** dummy=all_sliders;
        nr_of_sliders_alloc+=5;
        all_sliders=new stdSlider*[nr_of_sliders_alloc];
            for (int i=0;i<nr_of_sliders;i++)
            all_sliders[i]=dummy[i];
        if (dummy!=NULL)
        delete[] dummy;//delete original entries (only addresses, no real sliders)
    }
    all_sliders[nr_of_sliders]=slider;
    nr_of_sliders++;
}

void update_sliders(void)//update the fonts
{
//qDebug() << "Update Sliders";
    for (int i=0;i<nr_of_sliders;i++)
    {
    /*all_sliders[i]->Indicator->setFont(qApp->font());
    all_sliders[i]->lblText->setFont(qApp->font());*/
    all_sliders[i]->resetFont(qApp->font());
    /*QResizeEvent re(all_sliders[i]->size(),all_sliders[i]->size());
    all_sliders[i]->resizeEvent(&re);
    all_sliders[i]->adjustSize();*/
    }
    //mainWin->ManageBars();
}

void update_Line_Editors(void)
{
    for (int i=0;i<nr_of_non_latex_lines;i++)
    {
    //qDebug() << "Trying to access" << Non_LaTeX_Lines[i];
    Non_LaTeX_Lines[i]->adjustHeightToFont();
    }
    for (int i=0;i<nr_of_latex_lines;i++)
    {
    LaTeX_Lines[i]->adjustHeightToFont();
    }
}

void addPatternSelector(FillPatternSelector * selector)
{
    if (nr_of_pattern_selectors==n_of_pattern_selectors_alloc)
    {
        FillPatternSelector ** dummy=all_pattern_selectors;
        n_of_pattern_selectors_alloc+=5;
        all_pattern_selectors=new FillPatternSelector*[n_of_pattern_selectors_alloc];
            for (int i=0;i<nr_of_pattern_selectors;i++)
            all_pattern_selectors[i]=dummy[i];
        if (dummy!=NULL)
        delete[] dummy;//delete original entries (only addresses, no real sliders)
    }
    all_pattern_selectors[nr_of_pattern_selectors]=selector;
    nr_of_pattern_selectors++;
}

void update_PatternSelectors(void)
{
    QString ** titles=NULL;
    int old_w=PatternPixmaps[0]->width();
    int old_h=PatternPixmaps[0]->height();
    bool new_dimension=false;
    RecreatePatternIcons(38*toolBarSizeFactor,16*toolBarSizeFactor);
    if (38*toolBarSizeFactor!=old_w || 16*toolBarSizeFactor!=old_h) new_dimension=true;
//qDebug() << "new dimension=" << new_dimension;
    int last_pat_val;
    for (int i=0;i<nr_of_pattern_selectors;i++)
    {
    //if (titles!=NULL) delete[] titles;
    titles=new QString*[all_pattern_selectors[i]->cmbFillPattern->panels->number_of_elements];
        for (int j=0;j<all_pattern_selectors[i]->cmbFillPattern->panels->number_of_elements;j++)
        {
        titles[j]=new QString(all_pattern_selectors[i]->cmbFillPattern->title_strings[j]);
        //qDebug() << "Title=" << *(titles[j]);
        }
    last_pat_val=all_pattern_selectors[i]->currentIndex();
    all_pattern_selectors[i]->cmbFillPattern->reinitializePanels(
             all_pattern_selectors[i]->cmbFillPattern->panels->row_count,all_pattern_selectors[i]->cmbFillPattern->panels->col_count,
             all_pattern_selectors[i]->cmbFillPattern->panels->last_col_count,PatternPixmaps,titles,all_pattern_selectors[i]->cmbFillPattern->text_only,false);
    all_pattern_selectors[i]->setCurrentIndex(last_pat_val);
        if (new_dimension==true)
        all_pattern_selectors[i]->adjustSize();//needed because Patern-dimensions may change
    }
    //if (titles!=NULL) delete[] titles;
}

void addOrderSelector(OrderSelector * selector)
{
    if (nr_of_order_selectors==n_of_order_selectors_alloc)
    {
        OrderSelector ** dummy=all_order_selectors;
        n_of_order_selectors_alloc+=5;
        all_order_selectors=new OrderSelector*[n_of_order_selectors_alloc];
            for (int i=0;i<nr_of_order_selectors;i++)
            all_order_selectors[i]=dummy[i];
        if (dummy!=NULL)
        delete[] dummy;//delete original entries (only addresses, no real sliders)
    }
    all_order_selectors[nr_of_order_selectors]=selector;
    nr_of_order_selectors++;
}

void update_OrderSelectors(void)
{
    int last_val;
    bool new_dimensions=false;
    for (int i=0;i<nr_of_order_selectors;i++)
    {
        if (i==0)
        {
            if (all_order_selectors[i]->cmbOrderPattern->iconSize()!=QSize(Qt_matrixOrder[0]->width(),Qt_matrixOrder[0]->height()))
            {
            new_dimensions=true;
            }
        }
    last_val=all_order_selectors[i]->currentIndex();
    all_order_selectors[i]->cmbOrderPattern->reinitializePanels(4,2,4,Qt_matrixOrder,all_order_selectors[i]->OrderNames,false);
    all_order_selectors[i]->setCurrentIndex(last_val);
        if (new_dimensions==true)
        all_order_selectors[i]->adjustSize();
    }
//qDebug() << "OrderSelectors: new_dimensions=" << new_dimensions;
}

void addAlignmentSelector(AlignmentSelector * selector)
{
    if (nr_of_alignment_selectors==n_of_alignment_selectors_alloc)
    {
        AlignmentSelector ** dummy=all_alignment_selectors;
        n_of_alignment_selectors_alloc+=5;
        all_alignment_selectors=new AlignmentSelector*[n_of_alignment_selectors_alloc];
            for (int i=0;i<nr_of_alignment_selectors;i++)
            all_alignment_selectors[i]=dummy[i];
        if (dummy!=NULL)
        delete[] dummy;//delete original entries (only addresses, no real sliders)
    }
    all_alignment_selectors[nr_of_alignment_selectors]=selector;
    nr_of_alignment_selectors++;
}

void update_AlignmentSelectors(void)
{
    int last_val;
    bool new_dimensions=false;
    for (int i=0;i<nr_of_alignment_selectors;i++)
    {
        if (i==0)
        {
            if (all_alignment_selectors[i]->cmbJustSelect2->iconSize()!=QSize(Qt_sub_justifications[0]->width(),Qt_sub_justifications[0]->height()))
            {
            new_dimensions=true;
            }
        }
    last_val=all_alignment_selectors[i]->currentValue();
    all_alignment_selectors[i]->cmbJustSelect2->reinitializePanels(3,1,3,Qt_sub_justifications,all_alignment_selectors[i]->just_names2,false,false);
    all_alignment_selectors[i]->setCurrentValue(last_val);
        if (new_dimensions==true)
        all_alignment_selectors[i]->adjustSize();
    }
//qDebug() << "AlignmentSelectors: new_dimensions=" << new_dimensions;
}

void addJustificationSelector(JustificationSelector * selector)
{
    if (nr_of_justification_selectors==n_of_justification_selectors_alloc)
    {
        JustificationSelector ** dummy=all_justification_selectors;
        n_of_justification_selectors_alloc+=5;
        all_justification_selectors=new JustificationSelector*[n_of_justification_selectors_alloc];
            for (int i=0;i<nr_of_justification_selectors;i++)
            all_justification_selectors[i]=dummy[i];
        if (dummy!=NULL)
        delete[] dummy;//delete original entries (only addresses, no real sliders)
    }
    all_justification_selectors[nr_of_justification_selectors]=selector;
    nr_of_justification_selectors++;
}

void update_JustificationSelectors(void)
{
    int last_val,last_val2;
    bool new_dimensions=false;
    for (int i=0;i<nr_of_justification_selectors;i++)
    {
         if (i==0)
         {
             if (all_justification_selectors[i]->cmbJustSelect->iconSize()!=QSize(Qt_justifications[0]->width(),Qt_justifications[0]->height()))
             {
             new_dimensions=true;
             }
             if (all_justification_selectors[i]->cmbJustSelect2->iconSize()!=QSize(Qt_sub_justifications[0]->width(),Qt_sub_justifications[0]->height()))
             {
             new_dimensions=true;
             }
         }
    last_val=all_justification_selectors[i]->currentIndex();
    last_val2=all_justification_selectors[i]->currentIndex2();
    all_justification_selectors[i]->cmbJustSelect->reinitializePanels(3,4,3,Qt_justifications,all_justification_selectors[i]->just_names,false);
    all_justification_selectors[i]->cmbJustSelect2->reinitializePanels(3,1,3,Qt_sub_justifications,all_justification_selectors[i]->just_names2,false);
    all_justification_selectors[i]->setCurrentIndex(last_val);
    all_justification_selectors[i]->setCurrentIndex2(last_val2);
        if (new_dimensions==true)
        {
        all_justification_selectors[i]->cmbJustSelect->adjustSize();
        all_justification_selectors[i]->cmbJustSelect2->adjustSize();
        }
    }
//qDebug() << "JustificationSelectors: new_dimensions=" << new_dimensions;
}

void addLineWidthSelector(LineWidthSelector * selector)
{
    if (nr_of_linewidth_selectors==n_of_linewidth_selectors_alloc)
    {
        LineWidthSelector ** dummy=all_linewidth_selectors;
        n_of_linewidth_selectors_alloc+=5;
        all_linewidth_selectors=new LineWidthSelector*[n_of_linewidth_selectors_alloc];
            for (int i=0;i<nr_of_linewidth_selectors;i++)
            all_linewidth_selectors[i]=dummy[i];
        if (dummy!=NULL)
        delete[] dummy;//delete original entries (only addresses, no real sliders)
    }
    all_linewidth_selectors[nr_of_linewidth_selectors]=selector;
    nr_of_linewidth_selectors++;
}

void removeLineWidthSelector(LineWidthSelector * selector)
{
LineWidthSelector ** dummy=new LineWidthSelector*[n_of_linewidth_selectors_alloc];
int new_nr=0;
for (int i=0;i<nr_of_linewidth_selectors;i++)
{
    if (all_linewidth_selectors[i]!=selector)
    {
    dummy[new_nr++]=all_linewidth_selectors[i];
    }
}
nr_of_linewidth_selectors=new_nr;
for (int i=0;i<nr_of_linewidth_selectors;i++)
{
    all_linewidth_selectors[i]=dummy[i];
}
for (int i=nr_of_linewidth_selectors;i<n_of_linewidth_selectors_alloc;i++)
{
    all_linewidth_selectors[i]=NULL;
}
}

void update_LineWidthSelectors(void)
{
QLocale newLocale=(DecimalPointToUse=='.')?(*dot_locale):(*comma_locale);
    for (int i=0;i<nr_of_linewidth_selectors;i++)
    {
    all_linewidth_selectors[i]->spnLineWidth->setLocale(newLocale);
    //all_linewidth_selectors[i]->adjustSize();
    }
}

void addPositionSelector(PositionSelector * selector)
{
    if (nr_of_position_selectors==n_of_position_selectors_alloc)
    {
        PositionSelector ** dummy=all_position_selectors;
        n_of_position_selectors_alloc+=5;
        all_position_selectors=new PositionSelector*[n_of_position_selectors_alloc];
            for (int i=0;i<nr_of_position_selectors;i++)
            all_position_selectors[i]=dummy[i];
        if (dummy!=NULL)
        delete[] dummy;//delete original entries (only addresses, no real sliders)
    }
    all_position_selectors[nr_of_position_selectors]=selector;
    nr_of_position_selectors++;
}

void update_PositionSelectors(void)
{
    for (int i=0;i<nr_of_position_selectors;i++)
    {
    ;//all_position_selectors[i]->
    }
}

void addIntSelector(stdIntSelector * selector)
{
    if (nr_of_int_selectors==n_of_int_selectors_alloc)
    {
        stdIntSelector ** dummy=all_int_selectors;
        n_of_int_selectors_alloc+=5;
        all_int_selectors=new stdIntSelector*[n_of_int_selectors_alloc];
            for (int i=0;i<nr_of_int_selectors;i++)
            all_int_selectors[i]=dummy[i];
        if (dummy!=NULL)
        delete[] dummy;//delete original entries (only addresses, no real sliders)
    }
    all_int_selectors[nr_of_int_selectors]=selector;
    nr_of_int_selectors++;
}

void update_IntSelectors(void)
{
    for (int i=0;i<nr_of_int_selectors;i++)
    {
    ;//all_int_selectors[i]->
    }
}

void addStdSelector(StdSelector * selector)
{
//qDebug() << "Adding a StdSeletor=" << selector->lblText->text() << "Adresse=" << selector;
    if (nr_of_std_selectors==n_of_std_selectors_alloc)
    {
        StdSelector ** dummy=all_std_selectors;
        n_of_std_selectors_alloc+=5;
        all_std_selectors=new StdSelector*[n_of_std_selectors_alloc];
            for (int i=0;i<nr_of_std_selectors;i++)
            all_std_selectors[i]=dummy[i];
        if (dummy!=NULL)
        delete[] dummy;//delete original entries (only addresses, no real sliders)
    }
    all_std_selectors[nr_of_std_selectors]=selector;
    nr_of_std_selectors++;
}

void remove_StdSelector(StdSelector * selector)
{
int i;
    for (i=0;i<nr_of_std_selectors;i++) if (all_std_selectors[i]==selector) break;
    if (i<nr_of_std_selectors)
    {
        for (;i<nr_of_std_selectors-1;i++)
        {
        all_std_selectors[i]=all_std_selectors[i+1];
        }
    }
    nr_of_std_selectors--;
//qDebug() << "Selector removed, Nr now=" << nr_of_std_selectors;
}

void removeLastStdSelector(void)
{
    if (nr_of_std_selectors>0)
    {
    nr_of_std_selectors--;
    }
}

void update_StdSelectors(int new_gno=-2)
{
//int counter=0;
    for (int i=0;i<nr_of_std_selectors;i++)
    {
        if (all_std_selectors[i]->simple_graph_selector==true)
        {
        all_std_selectors[i]->setValuesToGraphs(all_std_selectors[i]->include_NONE,1);
        if (new_gno>=-1)
        {
        all_std_selectors[i]->setCurrentValue(new_gno);
        }
        //counter++;
        }
    //all_std_selectors[i]->
    }
//qDebug() << "Updateing" << counter << "/" << nr_of_std_selectors << "StdSelectors";
}

void updateAllSelectors(void)
{
    update_sliders();
    update_StdSelectors();
    update_line_style_selectors();
    update_color_selectors();
    update_PatternSelectors();
    update_OrderSelectors();
    update_AlignmentSelectors();
    update_JustificationSelectors();
    update_LineWidthSelectors();
}

void CheckLaTeXLinesForAddress(char * o_adr,char * n_adr)//look through all LineEdits that may have dynamic char-addresses associated and replaces o_adr with n_adr
{
    //if (o_adr==n_adr) return;//nothing to do ( || o_adr==NULL)
    if (o_adr==n_adr || o_adr==NULL) return;//nothing to do
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
double jul;
tmp_defaults=grdefaults;
tmp_view=grview;
strcpy(tmp_sformat,sformat);

    allPrefs=new QSettings(path_to_read_settings(),QSettings::IniFormat);
    allPrefs->sync();
    allPrefs->beginGroup(QString("Preferences"));

    save_no_ask=noask=(int)allPrefs->value(QString("dontaskquestions"),QVariant(false)).toBool();
    allow_dc=(int)allPrefs->value(QString("allowdoubleclickoncanvas"),QVariant(true)).toBool();
    focus_policy=allPrefs->value(QString("graphfocusswitch"),QVariant(0)).toInt();
    draw_focus_flag=(int)allPrefs->value(QString("displayfocusmarker"),QVariant(true)).toBool();
    auto_redraw=(int)allPrefs->value(QString("autoredraw"),QVariant(true)).toBool();
    cursortype=(int)allPrefs->value(QString("crosshaircursor"),QVariant(false)).toBool();
    set_max_path_limit(allPrefs->value(QString("maxdrawpathlength"),QVariant(get_max_path_limit())).toInt());
    safe_mode=(int)allPrefs->value(QString("runinsafemode"),QVariant(true)).toBool();
    scrollper=allPrefs->value(QString("scrollpercent"),QVariant(5.0)).toDouble();
    shexper=allPrefs->value(QString("zoompercent"),QVariant(5.0)).toDouble();
    scrolling_islinked=(int)allPrefs->value(QString("linkedscrolling"),QVariant(false)).toBool();

    switch (allPrefs->value(QString("datehint"),QVariant(3)).toInt())
    {
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

    current_date_separator=allPrefs->value(QString("dateseparator"),QVariant(0)).toInt();

    if (parse_date_or_number(allPrefs->value(QString("referencedate"),QVariant("-4713-01-01 12:00:00")).toString().toLatin1().data(), TRUE, &jul)== RETURN_SUCCESS) {
        set_ref_date(jul);
    } else {
        errmsg(QObject::tr("Invalid reference date in ini-file").toLocal8Bit().constData());
    }

    allow_two_digits_years(allPrefs->value(QString("twodigityearspan"),QVariant(false)).toInt());
    set_wrap_year(allPrefs->value(QString("wrapyear"),QVariant("1950")).toString().toInt());

    /*
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

    */

    allPrefs->endGroup();/// end Grace-preferences

    allPrefs->beginGroup(QString("ExtraPreferences"));
    //Form_Preferences->tab_extra->lenHome->setText(allPrefs->value(QString("gracehomedirectory"),QVariant(qt_grace_share_dir)).toString());
    display_help_externally=allPrefs->value(QString("showhelpexternally"),QVariant(false)).toBool();
    strcpy(qtgrace_help_viewer,allPrefs->value(QString("helpviewer"),QVariant("")).toString().toLocal8Bit().constData());
    current_language=allPrefs->value(QString("language"),QVariant(0)).toInt();
    showhideworkaround=allPrefs->value(QString("showhideworkaround"),QVariant(false)).toBool();
    useQtFonts=allPrefs->value(QString("useQtFonts"),QVariant(false)).toBool();
    symbol_font_is_special=allPrefs->value(QString("SymbolFontIsSpecial"),QVariant(true)).toBool();
    strcpy(dummy,allPrefs->value(QString("DecimalSeparator"),QVariant(".")).toString().toLatin1().constData());
    DecimalPointToUse=dummy[0];
    set_print_cmd(allPrefs->value(QString("PrintCommand"),QVariant((const char*)get_print_cmd())).toString().toLatin1().constData());
    use_print_command=allPrefs->value(QString("UsePrintCommand"),QVariant(false)).toBool();
    auto_set_agr_extension=allPrefs->value(QString("AutoSetAgrExtension"),QVariant(TRUE)).toInt();
    auto_set_cwd=allPrefs->value(QString("AutoSetCWD"),QVariant(TRUE)).toInt();
    auto_set_export_extensions=allPrefs->value(QString("AutoSetExportExtensions"),QVariant(TRUE)).toInt();
    autofit_on_load=allPrefs->value(QString("AutoFitPageOnLoad"),QVariant(FALSE)).toInt();
    useHDPrinterOutput=allPrefs->value(QString("UseHDPrinterOutput"),QVariant(FALSE)).toInt();
    typicalPrinterResolution=allPrefs->value(QString("TypicalPrinterResolution"),QVariant(300)).toInt();

    default_compress_data=allPrefs->value(QString("DataCompression"),QVariant(default_compress_data)).toInt();
    default_compression_level=allPrefs->value(QString("CompressionLevel"),QVariant(default_compression_level)).toInt();

    use_fftw3=allPrefs->value(QString("UseLibFFTW3"),QVariant(FALSE)).toInt();
    path_to_fftw3_lib.setFileName(allPrefs->value(QString("LibFFTW3_Path"),QVariant(path_to_fftw3_lib.fileName())).toString());
    use_libHaru=allPrefs->value(QString("UseLibHaru"),QVariant(FALSE)).toInt();
    path_to_libharu.setFileName(allPrefs->value(QString("LibHaru_Path"),QVariant(path_to_libharu.fileName())).toString());

    allPrefs->endGroup();

    allPrefs->beginGroup(QString("Defaults"));
    strcpy(sformat,allPrefs->value(QString("Default_SaveFormat"),QVariant("%.8g")).toString().toLatin1().constData());
    grdefaults.color=allPrefs->value(QString("Default_Color"),QVariant(d_d.color)).toInt();
    grdefaults.bgcolor=allPrefs->value(QString("Default_BGColor"),QVariant(d_d.bgcolor)).toInt();
    grdefaults.alpha=allPrefs->value(QString("Default_Alpha"),QVariant(d_d.alpha)).toInt();
    grdefaults.bgalpha=allPrefs->value(QString("Default_BGAlpha"),QVariant(d_d.bgalpha)).toInt();
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

    DefaultSetListShowHidden=allPrefs->value(QString("SetListShowHidden"),QVariant(TRUE)).toInt();
    DefaultSetListShowDataless=allPrefs->value(QString("SetListShowDataless"),QVariant(FALSE)).toInt();
    DefaultSetListShowIcons=allPrefs->value(QString("SetListShowIcons"),QVariant(FALSE)).toInt();
    DefaultSetListShowComments=allPrefs->value(QString("SetListShowComments"),QVariant(FALSE)).toInt();
    DefaultSetListShowLegends=allPrefs->value(QString("SetListShowLegends"),QVariant(FALSE)).toInt();
    allow_drag_in_lists=allPrefs->value(QString("SetListAllowItemDrag"),QVariant(0)).toInt();

    SpreadsheetColumnFormat=allPrefs->value(QString("SpreadsheetColumnFormat"),QVariant(1)).toInt();
    SpreadsheetColumnPrecision=allPrefs->value(QString("SpreadsheetColumnPrecision"),QVariant(5)).toInt();

    allPrefs->endGroup();

    allPrefs->beginGroup(QString("History"));
    max_history=allPrefs->value(QString("maxhistory"),QVariant(MAX_HISTORY)).toInt();
    current_history=allPrefs->value(QString("historysize"),QVariant(0)).toInt();
    for (int i=0;i<MAX_HISTORY;i++)//always save MAX_HISTORY entries (possibly empty ones)
    {
        sprintf(dummy,"history%d",i);
        history[i]=allPrefs->value(QString(dummy),QVariant("")).toString();
        history[i]=QDir::cleanPath(history[i]);
        history[i]=QDir::toNativeSeparators(history[i]);
    }
    allPrefs->endGroup();

    allPrefs->beginGroup(QString("General"));
//QString cur_version_string=allPrefs->value(QString("QtGrace_Version"),QVariant("v0.2.4")).toString();
    general_behavior=allPrefs->value(QString("Behavior"),QVariant(0)).toInt();
    lastPrintDevice=stdOutputFormat=allPrefs->value(QString("lastOutputFormat"),QVariant(1)).toInt();
    //undo_active=allPrefs->value(QString("activateUndoRecords"),QVariant(false)).toInt();///undo deactivated as a default
    start_with_undo_active=allPrefs->value(QString("activateUndoRecords"),QVariant(false)).toInt();///undo deactivated as a default
    activateLaTeXsupport=allPrefs->value(QString("activateLaTeXSupport"),QVariant(false)).toBool()==true?1:0;
    immediateUpdate=allPrefs->value(QString("ImmediateUpdates"),QVariant(false)).toBool();
    default_Print_Device=allPrefs->value(QString("DefaultPrintingDevice"),QVariant(DEVICE_PNG)).toInt();//changed from -1 to PNG
//Form_Preferences->selDefaultPrintDevice->setCurrentIndex(default_Print_Device+1);//+1 because -1=last
    page_view_zoom=allPrefs->value(QString("PageViewZoom"),QVariant(0)).toInt();
    clip_borders_for_output=allPrefs->value(QString("ClipOutputEdges"),QVariant(0)).toInt();

    QString codName=allPrefs->value(QString("FileCodec"),QVariant("UTF-8")).toString();
    strcpy(file_codec_name,codName.toLocal8Bit().constData());

QString pat_lens,pat_styles;
int nr_of_l_styles;

    DefaultFont=allPrefs->value(QString("DefaultFont"),QVariant(0)).toInt();
    strcpy(default_grace_file,allPrefs->value(QString("DefaultAgrFile"),QVariant(QString("Default.agr"))).toString().toLocal8Bit().constData());
    strcpy(default_csv_format,allPrefs->value(QString("DefaultCSVFormatFile"),QVariant(QString(""))).toString().toLocal8Bit().constData());
    warn_on_encoding_change=allPrefs->value(QString("Warn_On_Encoding_Change"),QVariant(TRUE)).toInt();

    sync_device_dims=allPrefs->value(QString("Sync_Page_Dimensions_On_Devices"),QVariant(true)).toBool();
    rescale_plot_on_size_change=allPrefs->value(QString("Rescale_Plot_On_Page_Size_Change"),QVariant(false)).toBool();

    ScaleLineWidthByResolution=allPrefs->value(QString("Scale_LineWith_By_Resolution"),QVariant(FALSE)).toInt();
    general_antialiasing=allPrefs->value(QString("Antialiasing"),QVariant(FALSE)).toInt();
    inline_editing=allPrefs->value(QString("InlineTextEditing"),QVariant(FALSE)).toInt();
    comment_server_client=allPrefs->value(QString("CommentServerClient"),QVariant(FALSE)).toInt();
    autoPackSets=allPrefs->value(QString("AutoPackSets"),QVariant(FALSE)).toInt();
    context_menu_in_main=allPrefs->value(QString("ContextMenuesInMainWindow"),QVariant(FALSE)).toInt();
    icon_preferences=allPrefs->value(QString("IconDisplayPreferences"),QVariant(0)).toInt();
    useErrorbarsInAutoscale=allPrefs->value(QString("UseErrorbarsInAutoscale"),QVariant(TRUE)).toInt();
    minAutoscaleBorderOffset=allPrefs->value(QString("MinAutoscaleOffset"),QVariant(0.0)).toDouble();
    allow_gestures=allPrefs->value(QString("Allow_Gestures"),QVariant(FALSE)).toInt();
    allow_wheel_zoom=allPrefs->value(QString("AllowWheelZooming"),QVariant(FALSE)).toInt();
    allow_wheel_changes=allPrefs->value(QString("AllowWheelChanges"),QVariant(FALSE)).toInt();
    allPrefs->endGroup();

    allPrefs->beginGroup(QString("Appearance"));

    use_new_icons=allPrefs->value(QString("UseNewIcons"),QVariant(false)).toBool();
#if QT_VERSION >= 0x060000
    QColor col=QApplication::palette().color(QPalette::Window);
#else
    QColor col=QApplication::palette().color(QPalette::Background);
#endif
    sprintf(dummy,"%d;%d;%d",col.red(),col.green(),col.blue());
    pat_lens=allPrefs->value(QString("DrawAreaBackground"),QVariant(QString(dummy))).toString();
    strcpy(background_color,pat_lens.toLatin1().constData());
    //universal_font_size_factor=allPrefs->value(QString("UniversalFontSizeFactor"),QVariant(1.0)).toDouble();//universal factor is to be written into the file not in general for the whole Application
    pat_lens=allPrefs->value(QString("GuiFont"),QVariant(qApp->font().toString())).toString();
    GuiFont->fromString(pat_lens);

    initial_x_pos=allPrefs->value(QString("InitialXPos"),QVariant(65)).toInt();
    initial_y_pos=allPrefs->value(QString("InitialYPos"),QVariant(65)).toInt();
    initial_width=allPrefs->value(QString("InitialWidth"),QVariant(872)).toInt();
    initial_height=allPrefs->value(QString("InitialHeight"),QVariant(670)).toInt();
    initial_size_behavior=allPrefs->value(QString("DialogSizeBehavior"),QVariant(0)).toInt();
    toolBarSizeFactor=allPrefs->value(QString("toolBarSizeFactor"),QVariant(1.0)).toDouble();
#if QT_VERSION >= 0x060000
    start_dpi=allPrefs->value(QString("StartDPI"),QVariant(qApp->primaryScreen()->physicalDotsPerInchX())).toInt();
#else
    start_dpi=allPrefs->value(QString("StartDPI"),QVariant(qApp->desktop()->physicalDpiX())).toInt();
#endif
    small_screen_adjustments=allPrefs->value(QString("SmallScreenAdjustments"),QVariant(3)).toInt();

device_table[DEVICE_X11].pg.dpi=start_dpi;

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
    show_PanButton=allPrefs->value(QString("Show_Pan_Button"),QVariant(TRUE)).toInt();
    show_PickButton=allPrefs->value(QString("Show_Pick_Button"),QVariant(FALSE)).toInt();
    show_UndoRedo_B=allPrefs->value(QString("Show_UndoRedo_Buttons"),QVariant(FALSE)).toInt();
    highlight_color=allPrefs->value(QString("HighlightColor"),QVariant(2)).toInt();
    highlight_alpha=allPrefs->value(QString("HighlightAlpha"),QVariant(255)).toInt();
    highlight_time=allPrefs->value(QString("HighlightTime"),QVariant(FALSE)).toInt();
    auto_highlight=allPrefs->value(QString("AutoHighlight"),QVariant(FALSE)).toInt();
    highlight_errorbars=allPrefs->value(QString("HighlightErrorbars"),QVariant(FALSE)).toInt();
    showPageSettings=allPrefs->value(QString("ShowPageSettings"),QVariant(TRUE)).toInt();
    showFontSettings=allPrefs->value(QString("ShowFontSettings"),QVariant(TRUE)).toInt();

    drop_comment=allPrefs->value(QString("DropFileComment"),QVariant(DROP_LEGEND_PATH_FILE_COLUMN)).toInt();
    drop_legend=allPrefs->value(QString("DropFileLegend"),QVariant(DROP_LEGEND_PATH_FILE_COLUMN)).toInt();
    drop_autoscale=allPrefs->value(QString("DropAutoscale"),QVariant(TRUE)).toInt();
    drop_dec_sep=(char)allPrefs->value(QString("DropFileDecimalSeparator"),QVariant('.')).toInt();

//qDebug() << "Drop-Decimal-Separator=" << drop_dec_sep;

    allPrefs->endGroup();

    allPrefs->beginGroup(QString("OutputDrivers"));
    for (int i=0;i<number_of_devices();i++)
    {
    setDeviceActive(i,allPrefs->value(QString(get_device_name(i)),QVariant(1)).toInt());
    }
    allPrefs->endGroup();

    allPrefs->beginGroup(QString("UserDefaultGeometries"));
    nr_of_l_styles=allPrefs->value(QString("UserDefaultGeometry_NR"),QVariant(0)).toInt();
    if (NR_OF_USER_DEFAULT_DEVICE_GEOMETRIES<nr_of_l_styles) nr_of_l_styles=NR_OF_USER_DEFAULT_DEVICE_GEOMETRIES;
    for (int i=0;i<nr_of_l_styles;i++)
    {
    strcpy(DefaultUserDeviceGeometry[i].name,allPrefs->value(QString("UserDefaultGeometry_NAME")+QString::number(i),QVariant((const char*)(DefaultUserDeviceGeometry[i].name))).toString().toLocal8Bit().constData());
    DefaultUserDeviceGeometry[i].active=allPrefs->value(QString("UserDefaultGeometry_ACTIVE")+QString::number(i),QVariant(DefaultUserDeviceGeometry[i].active)).toInt();
    strcpy(dummy,allPrefs->value(QString("UserDefaultGeometry_DEVICE")+QString::number(i),QVariant("None")).toString().toLocal8Bit().constData());
        if (!strcmp(dummy,"None"))
        DefaultUserDeviceGeometry[i].dev_nr=-1;
        else
        DefaultUserDeviceGeometry[i].dev_nr=get_device_by_name(dummy);
    DefaultUserDeviceGeometry[i].orientation=(signed char)allPrefs->value(QString("UserDefaultGeometry_ORIENTATION")+QString::number(i),QVariant(DefaultUserDeviceGeometry[i].orientation)).toInt();
    DefaultUserDeviceGeometry[i].width=allPrefs->value(QString("UserDefaultGeometry_WIDTH")+QString::number(i),QVariant(DefaultUserDeviceGeometry[i].width)).toDouble();
    DefaultUserDeviceGeometry[i].height=allPrefs->value(QString("UserDefaultGeometry_HEIGHT")+QString::number(i),QVariant(DefaultUserDeviceGeometry[i].height)).toDouble();
    DefaultUserDeviceGeometry[i].unit=(signed char)allPrefs->value(QString("UserDefaultGeometry_UNIT")+QString::number(i),QVariant(DefaultUserDeviceGeometry[i].unit)).toInt();
    DefaultUserDeviceGeometry[i].dpi=allPrefs->value(QString("UserDefaultGeometry_DPI")+QString::number(i),QVariant(DefaultUserDeviceGeometry[i].dpi)).toFloat();
    DefaultUserDeviceGeometry[i].Antialiasing=(signed char)allPrefs->value(QString("UserDefaultGeometry_ANTIALIASING")+QString::number(i),QVariant(DefaultUserDeviceGeometry[i].Antialiasing)).toInt();
    DefaultUserDeviceGeometry[i].ScaleLineWidth=(signed char)allPrefs->value(QString("UserDefaultGeometry_SCALELINEWIDTH")+QString::number(i),QVariant(DefaultUserDeviceGeometry[i].ScaleLineWidth)).toInt();
        //if (DefaultUserDeviceGeometry[i].dev_nr<0)
        //qDebug() << "i=" << i << " name=" << DefaultUserDeviceGeometry[i].name << " device=None";
        //else
        //qDebug() << "i=" << i << " name=" << DefaultUserDeviceGeometry[i].name << " device=" << get_device_name(DefaultUserDeviceGeometry[i].dev_nr);
    }
    unsigned int internal_quick_selections=0;
    for (int i=0;i<NR_OF_QUICKSELECTOPTIONS;i++)
    {
        if (DefaultSystemDeviceGeometry[i].active)
        internal_quick_selections+=(1<<i);
    }
    internal_quick_selections=allPrefs->value(QString("InternalDefaultGeometries_ACTIVE"),QVariant(internal_quick_selections)).toUInt();
    for (int i=0;i<NR_OF_QUICKSELECTOPTIONS;i++)
    {
        if (internal_quick_selections & (1<<i))
        DefaultSystemDeviceGeometry[i].active=TRUE;
        else
        DefaultSystemDeviceGeometry[i].active=FALSE;
    }
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

    allPrefs->beginGroup(QString("DialogSizes"));
    LastSize_EllipseProps=allPrefs->value(QString("EllipseProps"),QVariant(LastSize_EllipseProps)).toSize();
    LastSize_EditEllipseProps=allPrefs->value(QString("EditEllipseProps"),QVariant(LastSize_EditEllipseProps)).toSize();
    LastSize_BoxProps=allPrefs->value(QString("BoxProps"),QVariant(LastSize_BoxProps)).toSize();
    LastSize_EditBoxProps=allPrefs->value(QString("EditBoxProps"),QVariant(LastSize_EditBoxProps)).toSize();
    LastSize_TextProps=allPrefs->value(QString("TextProps"),QVariant(LastSize_TextProps)).toSize();
    LastSize_EditTextProps=allPrefs->value(QString("EditTextProps"),QVariant(LastSize_EditTextProps)).toSize();
    LastSize_LineProps=allPrefs->value(QString("LineProps"),QVariant(LastSize_LineProps)).toSize();
    LastSize_EditLineProps=allPrefs->value(QString("EditLineProps"),QVariant(LastSize_EditLineProps)).toSize();
    LastSize_FormDrawObjects=allPrefs->value(QString("FormDrawObjects"),QVariant(LastSize_FormDrawObjects)).toSize();
    LastSize_FormConsole=allPrefs->value(QString("FormConsole"),QVariant(LastSize_FormConsole)).toSize();
    LastSize_FormEvalExpr=allPrefs->value(QString("FormEvalExpr"),QVariant(LastSize_FormEvalExpr)).toSize();
    LastSize_FormSetAppearance=allPrefs->value(QString("FormSetAppearance"),QVariant(LastSize_FormSetAppearance)).toSize();
    LastSize_FormGraphAppearance=allPrefs->value(QString("FormGraphAppearance"),QVariant(LastSize_FormGraphAppearance)).toSize();
    LastSize_FormPlotAppearance=allPrefs->value(QString("FormPlotAppearance"),QVariant(LastSize_FormPlotAppearance)).toSize();
    LastSize_FormLocatorProps=allPrefs->value(QString("FormLocatorProps"),QVariant(LastSize_FormLocatorProps)).toSize();
    LastSize_FormAxisProperties=allPrefs->value(QString("FormAxisProperties"),QVariant(LastSize_FormAxisProperties)).toSize();
    LastSize_FormPointExplorer=allPrefs->value(QString("FormPointExplorer"),QVariant(LastSize_FormPointExplorer)).toSize();
    LastSize_FormNonlinCurveFit=allPrefs->value(QString("FormNonlinCurveFit"),QVariant(LastSize_FormNonlinCurveFit)).toSize();
    LastSize_Form2DFit=allPrefs->value(QString("Form2DFit"),QVariant(LastSize_Form2DFit)).toSize();
    LastSize_FormInterpolation=allPrefs->value(QString("FormInterpolation"),QVariant(LastSize_FormInterpolation)).toSize();
    LastSize_FormSetOperations=allPrefs->value(QString("FormSetOperations"),QVariant(LastSize_FormSetOperations)).toSize();
    LastSize_FormExplorer=allPrefs->value(QString("FormExplorer"),QVariant(LastSize_FormExplorer)).toSize();
    LastSize_FormRTIManage=allPrefs->value(QString("FormRTIManage"),QVariant(LastSize_FormRTIManage)).toSize();
    LastSize_FormCommands=allPrefs->value(QString("FormCommands"),QVariant(LastSize_FormCommands)).toSize();
    LastSize_FormDeviceOptions=allPrefs->value(QString("FormDeviceOptions"),QVariant(LastSize_FormDeviceOptions)).toSize();
    LastSize_FormDeviceSetup=allPrefs->value(QString("FormDeviceSetup"),QVariant(LastSize_FormDeviceSetup)).toSize();
    LastSize_FormUserDefaultDevice=allPrefs->value(QString("FormUserDefaultDevice"),QVariant(LastSize_FormUserDefaultDevice)).toSize();
    LastSize_FormArrangeGraphs=allPrefs->value(QString("FormArrangeGraphs"),QVariant(LastSize_FormArrangeGraphs)).toSize();
    LastSize_FormOverlayGraphs=allPrefs->value(QString("FormOverlayGraphs"),QVariant(LastSize_FormOverlayGraphs)).toSize();
    LastSize_FormAutoScaleGraphs=allPrefs->value(QString("FormAutoScaleGraphs"),QVariant(LastSize_FormAutoScaleGraphs)).toSize();
    LastSize_FormDataSetProperties=allPrefs->value(QString("FormDataSetProperties"),QVariant(LastSize_FormDataSetProperties)).toSize();
    LastSize_FormSetOPS=allPrefs->value(QString("FormSetOPS"),QVariant(LastSize_FormSetOPS)).toSize();
    LastSize_FormFeatureExtraction=allPrefs->value(QString("FormFeatureExtraction"),QVariant(LastSize_FormFeatureExtraction)).toSize();
    LastSize_FormHistograms=allPrefs->value(QString("FormHistograms"),QVariant(LastSize_FormHistograms)).toSize();
    LastSize_FormFourier=allPrefs->value(QString("FormFourier"),QVariant(LastSize_FormFourier)).toSize();
    LastSize_FormFourier2=allPrefs->value(QString("FormFourier2"),QVariant(LastSize_FormFourier2)).toSize();
    LastSize_FormLoadAndEvaluate=allPrefs->value(QString("FormLoadAndEvaluate"),QVariant(LastSize_FormLoadAndEvaluate)).toSize();
    LastSize_FormIntegration=allPrefs->value(QString("FormIntegration"),QVariant(LastSize_FormIntegration)).toSize();
    LastSize_FormSeasonDiff=allPrefs->value(QString("FormSeasonDiff"),QVariant(LastSize_FormSeasonDiff)).toSize();
    LastSize_FormDifferences=allPrefs->value(QString("FormDifferences"),QVariant(LastSize_FormDifferences)).toSize();
    LastSize_FormAverages=allPrefs->value(QString("FormAverages"),QVariant(LastSize_FormAverages)).toSize();
    LastSize_FormRegression=allPrefs->value(QString("FormRegression"),QVariant(LastSize_FormRegression)).toSize();
    LastSize_FormCorrelation=allPrefs->value(QString("FormCorrelation"),QVariant(LastSize_FormCorrelation)).toSize();
    LastSize_FormConvolution=allPrefs->value(QString("FormConvolution"),QVariant(LastSize_FormConvolution)).toSize();
    LastSize_FormDigitalFilter=allPrefs->value(QString("FormDigitalFilter"),QVariant(LastSize_FormDigitalFilter)).toSize();
    LastSize_FormPruneData=allPrefs->value(QString("FormPruneData"),QVariant(LastSize_FormPruneData)).toSize();
    LastSize_FormSamplePoints=allPrefs->value(QString("FormSamplePoints"),QVariant(LastSize_FormSamplePoints)).toSize();
    LastSize_FormGeometricTransform=allPrefs->value(QString("FormGeometricTransform"),QVariant(LastSize_FormGeometricTransform)).toSize();
    LastSize_FormHotLinks=allPrefs->value(QString("FormHotLinks"),QVariant(LastSize_FormHotLinks)).toSize();
    LastSize_FormRegionMaster=allPrefs->value(QString("FormRegionMaster"),QVariant(LastSize_FormRegionMaster)).toSize();
    LastSize_FormRegionStatus=allPrefs->value(QString("FormRegionStatus"),QVariant(LastSize_FormRegionStatus)).toSize();
    LastSize_FormReportRegion=allPrefs->value(QString("FormReportRegion"),QVariant(LastSize_FormReportRegion)).toSize();
    LastSize_FormClearRegion=allPrefs->value(QString("FormClearRegion"),QVariant(LastSize_FormClearRegion)).toSize();
    LastSize_FormDefineRegion=allPrefs->value(QString("FormDefineRegion"),QVariant(LastSize_FormDefineRegion)).toSize();
    LastSize_FormNetCDF=allPrefs->value(QString("FormNetCDF"),QVariant(LastSize_FormNetCDF)).toSize();
    LastSize_FormReadSets=allPrefs->value(QString("FormReadSets"),QVariant(LastSize_FormReadSets)).toSize();
    LastSize_FormReadNetCDF=allPrefs->value(QString("FormReadNetCDF"),QVariant(LastSize_FormReadNetCDF)).toSize();
    LastSize_FormWriteSets=allPrefs->value(QString("FormWriteSets"),QVariant(LastSize_FormWriteSets)).toSize();
    LastSize_FormOpenProject=allPrefs->value(QString("FormOpenProject"),QVariant(LastSize_FormOpenProject)).toSize();
    LastSize_FormSaveProject=allPrefs->value(QString("FormSaveProject"),QVariant(LastSize_FormSaveProject)).toSize();
    LastSize_FormSelectHotLink=allPrefs->value(QString("FormSelectHotLink"),QVariant(LastSize_FormSelectHotLink)).toSize();
    LastSize_FormReadParameters=allPrefs->value(QString("FormReadParameters"),QVariant(LastSize_FormReadParameters)).toSize();
    LastSize_FormWriteParameters=allPrefs->value(QString("FormWriteParameters"),QVariant(LastSize_FormWriteParameters)).toSize();
    LastSize_FormSelectOutputFile=allPrefs->value(QString("FormSelectOutputFile"),QVariant(LastSize_FormSelectOutputFile)).toSize();
    LastSize_FormOpenFitPara=allPrefs->value(QString("FormOpenFitPara"),QVariant(LastSize_FormOpenFitPara)).toSize();
    LastSize_FormSaveFitPara=allPrefs->value(QString("FormSaveFitPara"),QVariant(LastSize_FormSaveFitPara)).toSize();
    LastSize_FormReadBinary=allPrefs->value(QString("FormReadBinary"),QVariant(LastSize_FormReadBinary)).toSize();
    LastSize_FormWriteBinary=allPrefs->value(QString("FormWriteBinary"),QVariant(LastSize_FormWriteBinary)).toSize();
    LastSize_FormCSVImport=allPrefs->value(QString("FormCSVImport"),QVariant(LastSize_FormCSVImport)).toSize();
    LastSize_FormCSVImportV2=allPrefs->value(QString("FormCSVImportV2"),QVariant(LastSize_FormCSVImportV2)).toSize();
    LastSize_FormProgress=allPrefs->value(QString("FormProgress"),QVariant(LastSize_FormProgress)).toSize();
    LastSize_FormQuestion=allPrefs->value(QString("FormQuestion"),QVariant(LastSize_FormQuestion)).toSize();
    LastSize_Bailout=allPrefs->value(QString("FormBailout"),QVariant(LastSize_Bailout)).toSize();
    LastSize_CreateAltAxis=allPrefs->value(QString("FormCreateAltAxis"),QVariant(LastSize_CreateAltAxis)).toSize();
    LastSize_FormSimpleListSel=allPrefs->value(QString("FormSimpleListSel"),QVariant(LastSize_FormSimpleListSel)).toSize();
    LastSize_FormReportFitParameters=allPrefs->value(QString("FormReportFitParameters"),QVariant(LastSize_FormReportFitParameters)).toSize();
    LastSize_FormGeometricEvaluation=allPrefs->value(QString("FormGeometricEvaluation"),QVariant(LastSize_FormGeometricEvaluation)).toSize();
    LastSize_FormBinaryImportFilter=allPrefs->value(QString("FormBinaryImportFilter"),QVariant(LastSize_FormBinaryImportFilter)).toSize();
    LastSize_FormEditBlockData=allPrefs->value(QString("FormEditBlockData"),QVariant(LastSize_FormEditBlockData)).toSize();
    LastSize_FormFontTool=allPrefs->value(QString("FormFontTool"),QVariant(LastSize_FormFontTool)).toSize();
    LastSize_FormAbout=allPrefs->value(QString("FormAbout"),QVariant(LastSize_FormAbout)).toSize();
    LastSize_FormSetEditor=allPrefs->value(QString("FormSetEditor"),QVariant(LastSize_FormSetEditor)).toSize();
    LastSize_FormUndoList=allPrefs->value(QString("FormUndoList"),QVariant(LastSize_FormUndoList)).toSize();
    LastSize_DeviceActivator=allPrefs->value(QString("DeviceActivator"),QVariant(LastSize_DeviceActivator)).toSize();
    LastSize_Form_Preferences=allPrefs->value(QString("Form_Preferences"),QVariant(LastSize_Form_Preferences)).toSize();
    LastSize_Form_AgrInfo=allPrefs->value(QString("Form_AgrInfo"),QVariant(LastSize_Form_AgrInfo)).toSize();
    LastSize_Form_SetupWizard=allPrefs->value(QString("Form_SetupWizard"),QVariant(LastSize_Form_SetupWizard)).toSize();
    LastSize_Form_InitWizard=allPrefs->value(QString("Form_InitWizard"),QVariant(LastSize_Form_InitWizard)).toSize();
    LastSize_FormIOForm=allPrefs->value(QString("FormIOForm"),QVariant(LastSize_FormIOForm)).toSize();
    LastSize_EditColumnProp=allPrefs->value(QString("EditColumnProp"),QVariant(LastSize_EditColumnProp)).toSize();
    LastSize_Spreadsheets=allPrefs->value(QString("Spreadsheets"),QVariant(LastSize_Spreadsheets)).toSize();
    allPrefs->endGroup();

    allPrefs->sync();
    delete allPrefs;
    //undo_active=res_undo;
    /// cout << "B) Read Settings: DecSepToUse=#" << DecimalPointToUse << "#" << endl;
ini_defaults=grdefaults;
ini_view=grview;
strcpy(ini_sformat,sformat);

//if the ini file is read, the defaults are to be used forthwith as standard
}

void write_settings(void)
{
//if we write the defaults in the ini file we use the current defaults
ini_defaults=grdefaults;
ini_view=grview;
strcpy(ini_sformat,sformat);

if (Form_Preferences)
LastSize_Form_Preferences=Form_Preferences->size();
if (FormDeviceSetup)
{
showPageSettings=FormDeviceSetup->grpPage->isChecked();
showFontSettings=FormDeviceSetup->grpFonts->isChecked();
LastSize_FormDeviceSetup=FormDeviceSetup->size();
}
if (FormUserDefaultGeometries)
{
LastSize_FormUserDefaultDevice=FormUserDefaultGeometries->size();
}
if (FormBinaryImportFilter)
{
LastSize_FormBinaryImportFilter=FormBinaryImportFilter->size();
}
if (FormConsole)
LastSize_FormConsole=FormConsole->size();
//qDebug() << "Writing size=" << LastSize_FormConsole;

if (Form_Preferences->diaDevAct)
{
LastSize_DeviceActivator=Form_Preferences->diaDevAct->size();
}
for (int i=0;i<7;i++)
if (FormDeviceSetup->DevOptions[i]) LastSize_FormDeviceOptions=FormDeviceSetup->DevOptions[i]->size();
for (int i=0;i<number_of_opened_spreadsheets;i++)
{
    if (FormSpreadSheets[i]!=NULL)
    {
    LastSize_Spreadsheets=FormSpreadSheets[i]->size();
        if (FormSpreadSheets[i]->EditColumn)
        LastSize_EditColumnProp=FormSpreadSheets[i]->EditColumn->size();
    }
}

    QString pat_lens,pat_styles;
    int y,m,d,h,mm,sec;
    allPrefs=new QSettings(path_to_write_settings(),QSettings::IniFormat);

    allPrefs->beginGroup(QString("Preferences"));
    noask=save_no_ask;
    allPrefs->setValue(QString("dontaskquestions"),QVariant((bool)noask));
    allPrefs->setValue(QString("allowdoubleclickoncanvas"),QVariant((bool)allow_dc));
    allPrefs->setValue(QString("graphfocusswitch"),QVariant(focus_policy));
    allPrefs->setValue(QString("displayfocusmarker"),QVariant((bool)draw_focus_flag));
    allPrefs->setValue(QString("autoredraw"),QVariant((bool)auto_redraw));
    allPrefs->setValue(QString("crosshaircursor"),QVariant((bool)cursortype));
    allPrefs->setValue(QString("maxdrawpathlength"),QVariant(get_max_path_limit()));
    allPrefs->setValue(QString("runinsafemode"),QVariant((bool)safe_mode));

    allPrefs->setValue(QString("scrollpercent"),QVariant(scrollper));
    allPrefs->setValue(QString("zoompercent"),QVariant(shexper));
    allPrefs->setValue(QString("linkedscrolling"),QVariant((bool)scrolling_islinked));

    switch (get_date_hint()) {
    case FMT_iso :
        y = 0;
        break;
    case FMT_european :
        y = 1;
        break;
    case FMT_us :
        y = 2;
        break;
    default :
        y = FMT_nohint;
        break;
    }
    allPrefs->setValue(QString("datehint"),QVariant(y));
    allPrefs->setValue(QString("dateseparator"),QVariant(current_date_separator));
    jul_to_cal_and_time(0.0, ROUND_SECOND, &y, &m, &d, &h, &mm, &sec);
    sprintf(dummy, "%d-%02d-%02d %02d:%02d:%02d",y, m, d, h, mm, sec);
    allPrefs->setValue(QString("referencedate"),QVariant((const char*)dummy));
    allPrefs->setValue(QString("twodigityearspan"),QVariant(two_digits_years_allowed()));
    allPrefs->setValue(QString("wrapyear"),QVariant(get_wrap_year()));
    allPrefs->endGroup();

    allPrefs->beginGroup(QString("ExtraPreferences"));
    //allPrefs->setValue(QString("gracehomedirectory"),QVariant(Form_Preferences->tab_extra->lenHome->text()));
    allPrefs->setValue(QString("showhelpexternally"),QVariant(display_help_externally));
    allPrefs->setValue(QString("helpviewer"),QVariant((const char*)qtgrace_help_viewer));
    allPrefs->setValue(QString("language"),QVariant(current_language));
    allPrefs->setValue(QString("showhideworkaround"),QVariant(showhideworkaround));
    allPrefs->setValue(QString("useQtFonts"),QVariant(useQtFonts));
    allPrefs->setValue(QString("SymbolFontIsSpecial"),QVariant(symbol_font_is_special));
    allPrefs->setValue(QString("DecimalSeparator"),QVariant(QString(DecimalPointToUse)));
    allPrefs->setValue(QString("PrintCommand"),QVariant((const char*)get_print_cmd()));
    allPrefs->setValue(QString("UsePrintCommand"),QVariant(use_print_command));
    allPrefs->setValue(QString("AutoSetAgrExtension"),QVariant(auto_set_agr_extension));
    allPrefs->setValue(QString("AutoSetCWD"),QVariant(auto_set_cwd));
    allPrefs->setValue(QString("AutoSetExportExtensions"),QVariant(auto_set_export_extensions));
    allPrefs->setValue(QString("AutoFitPageOnLoad"),QVariant(autofit_on_load));
    allPrefs->setValue(QString("UseHDPrinterOutput"),QVariant(useHDPrinterOutput));
    allPrefs->setValue(QString("TypicalPrinterResolution"),QVariant(typicalPrinterResolution));

    allPrefs->setValue(QString("DataCompression"),QVariant(default_compress_data));
    allPrefs->setValue(QString("CompressionLevel"),QVariant(default_compression_level));

    allPrefs->setValue(QString("UseLibFFTW3"),QVariant(use_fftw3));
    allPrefs->setValue(QString("LibFFTW3_Path"),QVariant(path_to_fftw3_lib.fileName()));
    allPrefs->setValue(QString("UseLibHaru"),QVariant(use_libHaru));
    allPrefs->setValue(QString("LibHaru_Path"),QVariant(path_to_libharu.fileName()));

    allPrefs->endGroup();
    allPrefs->beginGroup(QString("Defaults"));
    allPrefs->setValue(QString("Default_SaveFormat"),QVariant((const char*)sformat));
    allPrefs->setValue(QString("Default_Color"),QVariant(grdefaults.color));
    allPrefs->setValue(QString("Default_BGColor"),QVariant(grdefaults.bgcolor));
    allPrefs->setValue(QString("Default_Alpha"),QVariant(grdefaults.alpha));
    allPrefs->setValue(QString("Default_BGAlpha"),QVariant(grdefaults.bgalpha));
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

    allPrefs->setValue(QString("SetListShowHidden"),QVariant(DefaultSetListShowHidden));
    allPrefs->setValue(QString("SetListShowDataless"),QVariant(DefaultSetListShowDataless));
    allPrefs->setValue(QString("SetListShowIcons"),QVariant(DefaultSetListShowIcons));
    allPrefs->setValue(QString("SetListShowComments"),QVariant(DefaultSetListShowComments));
    allPrefs->setValue(QString("SetListShowLegends"),QVariant(DefaultSetListShowLegends));
    allPrefs->setValue(QString("SetListAllowItemDrag"),QVariant(allow_drag_in_lists));

    allPrefs->setValue(QString("SpreadsheetColumnFormat"),QVariant(SpreadsheetColumnFormat));
    allPrefs->setValue(QString("SpreadsheetColumnPrecision"),QVariant(SpreadsheetColumnPrecision));

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
    allPrefs->setValue(QString("QtGrace_Version"),QVariant(QTGRACE_VERSION_STRING));
    allPrefs->setValue(QString("Behavior"),QVariant(general_behavior));
    allPrefs->setValue(QString("lastOutputFormat"),QVariant(stdOutputFormat));
    allPrefs->setValue(QString("activateUndoRecords"),QVariant(start_with_undo_active));
    allPrefs->setValue(QString("activateLaTeXSupport"),QVariant((bool)activateLaTeXsupport));
    allPrefs->setValue(QString("ImmediateUpdates"),QVariant(immediateUpdate));
    allPrefs->setValue(QString("DefaultPrintingDevice"),QVariant(default_Print_Device));
    //allPrefs->setValue(QString("UseNewPrintingDialog"),QVariant(use_new_print_dialog));
    page_view_zoom=mainWin->sldPageZoom->value();
    allPrefs->setValue(QString("PageViewZoom"),QVariant(page_view_zoom));
    //allPrefs->setValue(QString("FileCodec"),QVariant(FileCodec->name()));
    allPrefs->setValue(QString("FileCodec"),QVariant("UTF-8"));
    allPrefs->setValue(QString("PrintCommand"),QVariant((const char*)print_cmd));
    allPrefs->setValue(QString("DefaultFont"),QVariant(DefaultFont));
    allPrefs->setValue(QString("DefaultAgrFile"),QVariant(QString::fromLocal8Bit(default_grace_file)));
    allPrefs->setValue(QString("DefaultCSVFormatFile"),QVariant(QString::fromLocal8Bit(default_csv_format)));
    allPrefs->setValue(QString("Warn_On_Encoding_Change"),QVariant(warn_on_encoding_change));
    allPrefs->setValue(QString("Sync_Page_Dimensions_On_Devices"),QVariant(sync_device_dims));
    allPrefs->setValue(QString("Rescale_Plot_On_Page_Size_Change"),QVariant(rescale_plot_on_size_change));
    allPrefs->setValue(QString("Scale_LineWith_By_Resolution"),QVariant(ScaleLineWidthByResolution));
    allPrefs->setValue(QString("Antialiasing"),QVariant(general_antialiasing));
    allPrefs->setValue(QString("InlineTextEditing"),QVariant(inline_editing));
    allPrefs->setValue(QString("CommentServerClient"),QVariant(comment_server_client));
    allPrefs->setValue(QString("AutoPackSets"),QVariant(autoPackSets));
    allPrefs->setValue(QString("ContextMenuesInMainWindow"),QVariant(context_menu_in_main));
    allPrefs->setValue(QString("IconDisplayPreferences"),QVariant(icon_preferences));
    allPrefs->setValue(QString("UseErrorbarsInAutoscale"),QVariant(useErrorbarsInAutoscale));
    allPrefs->setValue(QString("MinAutoscaleOffset"),QVariant(minAutoscaleBorderOffset));
    allPrefs->setValue(QString("Allow_Gestures"),QVariant(allow_gestures));
    allPrefs->setValue(QString("AllowWheelZooming"),QVariant(allow_wheel_zoom));
    allPrefs->setValue(QString("AllowWheelChanges"),QVariant(allow_wheel_changes));
    allPrefs->setValue(QString("ClipOutputEdges"),QVariant(clip_borders_for_output));
    allPrefs->endGroup();

    allPrefs->beginGroup(QString("Appearance"));
    allPrefs->setValue(QString("UseNewIcons"),QVariant(use_new_icons));
    QColor col=mainWin->mainArea->scroll->palette().color(QPalette::Window);//Background
    sprintf(dummy,"%d;%d;%d",col.red(),col.green(),col.blue());
    allPrefs->setValue(QString("DrawAreaBackground"),QVariant(QString(dummy)));
    //allPrefs->setValue(QString("UniversalFontSizeFactor"),QVariant(universal_font_size_factor));//universal factor is to be written into the file not in general for the whole Application
    pat_lens=GuiFont->toString();
    allPrefs->setValue(QString("GuiFont"),QVariant(pat_lens));
    allPrefs->setValue(QString("InitialXPos"),QVariant(initial_x_pos));
    allPrefs->setValue(QString("InitialYPos"),QVariant(initial_y_pos));
    allPrefs->setValue(QString("InitialWidth"),QVariant(initial_width));
    allPrefs->setValue(QString("InitialHeight"),QVariant(initial_height));
    allPrefs->setValue(QString("DialogSizeBehavior"),QVariant(initial_size_behavior));
    allPrefs->setValue(QString("toolBarSizeFactor"),QVariant(toolBarSizeFactor));
    allPrefs->setValue(QString("StartDPI"),QVariant(start_dpi));
    allPrefs->setValue(QString("SmallScreenAdjustments"),QVariant(small_screen_adjustments));
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
    allPrefs->setValue(QString("Show_Pan_Button"),QVariant(show_PanButton));
    allPrefs->setValue(QString("Show_Pick_Button"),QVariant(show_PickButton));
    allPrefs->setValue(QString("Show_UndoRedo_Buttons"),QVariant(show_UndoRedo_B));
    allPrefs->setValue(QString("HighlightColor"),QVariant(highlight_color));
    allPrefs->setValue(QString("HighlightAlpha"),QVariant(highlight_alpha));
    allPrefs->setValue(QString("HighlightTime"),QVariant(highlight_time));
    allPrefs->setValue(QString("AutoHighlight"),QVariant(auto_highlight));
    allPrefs->setValue(QString("HighlightErrorbars"),QVariant(highlight_errorbars));
    allPrefs->setValue(QString("ShowPageSettings"),QVariant(showPageSettings));
    allPrefs->setValue(QString("ShowFontSettings"),QVariant(showFontSettings));

    allPrefs->setValue(QString("DropFileComment"),QVariant(drop_comment));
    allPrefs->setValue(QString("DropFileLegend"),QVariant(drop_legend));
    allPrefs->setValue(QString("DropAutoscale"),QVariant(drop_autoscale));
    allPrefs->setValue(QString("DropFileDecimalSeparator"),QVariant(drop_dec_sep));

    allPrefs->endGroup();

    allPrefs->beginGroup(QString("OutputDrivers"));
    for (int i=0;i<number_of_devices();i++)
    {
    allPrefs->setValue(QString(get_device_name(i)),QVariant(isDeviceActive(i)));
    }
    allPrefs->endGroup();

    allPrefs->beginGroup(QString("UserDefaultGeometries"));
    allPrefs->setValue(QString("UserDefaultGeometry_NR"),QVariant(NR_OF_USER_DEFAULT_DEVICE_GEOMETRIES));
    for (int i=0;i<NR_OF_USER_DEFAULT_DEVICE_GEOMETRIES;i++)
    {
    allPrefs->setValue(QString("UserDefaultGeometry_NAME")+QString::number(i),QVariant(QString::fromLocal8Bit(DefaultUserDeviceGeometry[i].name)));
    allPrefs->setValue(QString("UserDefaultGeometry_ACTIVE")+QString::number(i),QVariant(DefaultUserDeviceGeometry[i].active));
        if (DefaultUserDeviceGeometry[i].dev_nr<0)
        allPrefs->setValue(QString("UserDefaultGeometry_DEVICE")+QString::number(i),QVariant("None"));
        else
        allPrefs->setValue(QString("UserDefaultGeometry_DEVICE")+QString::number(i),QVariant(get_device_name(DefaultUserDeviceGeometry[i].dev_nr)));
    allPrefs->setValue(QString("UserDefaultGeometry_ORIENTATION")+QString::number(i),QVariant(DefaultUserDeviceGeometry[i].orientation));
    allPrefs->setValue(QString("UserDefaultGeometry_WIDTH")+QString::number(i),QVariant(DefaultUserDeviceGeometry[i].width));
    allPrefs->setValue(QString("UserDefaultGeometry_HEIGHT")+QString::number(i),QVariant(DefaultUserDeviceGeometry[i].height));
    allPrefs->setValue(QString("UserDefaultGeometry_UNIT")+QString::number(i),QVariant(DefaultUserDeviceGeometry[i].unit));
    allPrefs->setValue(QString("UserDefaultGeometry_DPI")+QString::number(i),QVariant(DefaultUserDeviceGeometry[i].dpi));
    allPrefs->setValue(QString("UserDefaultGeometry_ANTIALIASING")+QString::number(i),QVariant(DefaultUserDeviceGeometry[i].Antialiasing));
    allPrefs->setValue(QString("UserDefaultGeometry_SCALELINEWIDTH")+QString::number(i),QVariant(DefaultUserDeviceGeometry[i].ScaleLineWidth));
    }
    unsigned int internal_quick_selections=0;
    for (int i=0;i<NR_OF_QUICKSELECTOPTIONS;i++)
    {
        if (DefaultSystemDeviceGeometry[i].active)
        internal_quick_selections+=(1<<i);
    }
    allPrefs->setValue(QString("InternalDefaultGeometries_ACTIVE"),QVariant(internal_quick_selections));
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

    allPrefs->beginGroup(QString("DialogSizes"));
    allPrefs->setValue(QString("EllipseProps"),QVariant(LastSize_EllipseProps));
    allPrefs->setValue(QString("EditEllipseProps"),QVariant(LastSize_EditEllipseProps));
    allPrefs->setValue(QString("BoxProps"),QVariant(LastSize_BoxProps));
    allPrefs->setValue(QString("EditBoxProps"),QVariant(LastSize_EditBoxProps));
    allPrefs->setValue(QString("TextProps"),QVariant(LastSize_TextProps));
    allPrefs->setValue(QString("EditTextProps"),QVariant(LastSize_EditTextProps));
    allPrefs->setValue(QString("LineProps"),QVariant(LastSize_LineProps));
    allPrefs->setValue(QString("EditLineProps"),QVariant(LastSize_EditLineProps));
    allPrefs->setValue(QString("FormDrawObjects"),QVariant(LastSize_FormDrawObjects));
    allPrefs->setValue(QString("FormConsole"),QVariant(LastSize_FormConsole));
    allPrefs->setValue(QString("FormEvalExpr"),QVariant(LastSize_FormEvalExpr));
    allPrefs->setValue(QString("FormSetAppearance"),QVariant(LastSize_FormSetAppearance));
    allPrefs->setValue(QString("FormGraphAppearance"),QVariant(LastSize_FormGraphAppearance));
    allPrefs->setValue(QString("FormPlotAppearance"),QVariant(LastSize_FormPlotAppearance));
    allPrefs->setValue(QString("FormLocatorProps"),QVariant(LastSize_FormLocatorProps));
    allPrefs->setValue(QString("FormAxisProperties"),QVariant(LastSize_FormAxisProperties));
    allPrefs->setValue(QString("FormPointExplorer"),QVariant(LastSize_FormPointExplorer));
    allPrefs->setValue(QString("FormNonlinCurveFit"),QVariant(LastSize_FormNonlinCurveFit));
    allPrefs->setValue(QString("Form2DFit"),QVariant(LastSize_Form2DFit));
    allPrefs->setValue(QString("FormInterpolation"),QVariant(LastSize_FormInterpolation));
    allPrefs->setValue(QString("FormSetOperations"),QVariant(LastSize_FormSetOperations));
    allPrefs->setValue(QString("FormExplorer"),QVariant(LastSize_FormExplorer));
    allPrefs->setValue(QString("FormRTIManage"),QVariant(LastSize_FormRTIManage));
    allPrefs->setValue(QString("FormCommands"),QVariant(LastSize_FormCommands));
    allPrefs->setValue(QString("FormDeviceOptions"),QVariant(LastSize_FormDeviceOptions));
    allPrefs->setValue(QString("FormDeviceSetup"),QVariant(LastSize_FormDeviceSetup));
    allPrefs->setValue(QString("FormUserDefaultDevice"),QVariant(LastSize_FormUserDefaultDevice));
    allPrefs->setValue(QString("FormArrangeGraphs"),QVariant(LastSize_FormArrangeGraphs));
    allPrefs->setValue(QString("FormOverlayGraphs"),QVariant(LastSize_FormOverlayGraphs));
    allPrefs->setValue(QString("FormAutoScaleGraphs"),QVariant(LastSize_FormAutoScaleGraphs));
    allPrefs->setValue(QString("FormDataSetProperties"),QVariant(LastSize_FormDataSetProperties));
    allPrefs->setValue(QString("FormSetOPS"),QVariant(LastSize_FormSetOPS));
    allPrefs->setValue(QString("FormFeatureExtraction"),QVariant(LastSize_FormFeatureExtraction));
    allPrefs->setValue(QString("FormHistograms"),QVariant(LastSize_FormHistograms));
    allPrefs->setValue(QString("FormFourier"),QVariant(LastSize_FormFourier));
    allPrefs->setValue(QString("FormFourier2"),QVariant(LastSize_FormFourier2));
    allPrefs->setValue(QString("FormLoadAndEvaluate"),QVariant(LastSize_FormLoadAndEvaluate));
    allPrefs->setValue(QString("FormIntegration"),QVariant(LastSize_FormIntegration));
    allPrefs->setValue(QString("FormSeasonDiff"),QVariant(LastSize_FormSeasonDiff));
    allPrefs->setValue(QString("FormDifferences"),QVariant(LastSize_FormDifferences));
    allPrefs->setValue(QString("FormAverages"),QVariant(LastSize_FormAverages));
    allPrefs->setValue(QString("FormRegression"),QVariant(LastSize_FormRegression));
    allPrefs->setValue(QString("FormCorrelation"),QVariant(LastSize_FormCorrelation));
    allPrefs->setValue(QString("FormConvolution"),QVariant(LastSize_FormConvolution));
    allPrefs->setValue(QString("FormDigitalFilter"),QVariant(LastSize_FormDigitalFilter));
    allPrefs->setValue(QString("FormPruneData"),QVariant(LastSize_FormPruneData));
    allPrefs->setValue(QString("FormSamplePoints"),QVariant(LastSize_FormSamplePoints));
    allPrefs->setValue(QString("FormGeometricTransform"),QVariant(LastSize_FormGeometricTransform));
    allPrefs->setValue(QString("FormHotLinks"),QVariant(LastSize_FormHotLinks));
    allPrefs->setValue(QString("FormRegionMaster"),QVariant(LastSize_FormRegionMaster));
    allPrefs->setValue(QString("FormRegionStatus"),QVariant(LastSize_FormRegionStatus));
    allPrefs->setValue(QString("FormReportRegion"),QVariant(LastSize_FormReportRegion));
    allPrefs->setValue(QString("FormClearRegion"),QVariant(LastSize_FormClearRegion));
    allPrefs->setValue(QString("FormDefineRegion"),QVariant(LastSize_FormDefineRegion));
    allPrefs->setValue(QString("FormNetCDF"),QVariant(LastSize_FormNetCDF));
    allPrefs->setValue(QString("FormReadSets"),QVariant(LastSize_FormReadSets));
    allPrefs->setValue(QString("FormReadNetCDF"),QVariant(LastSize_FormReadNetCDF));
    allPrefs->setValue(QString("FormWriteSets"),QVariant(LastSize_FormWriteSets));
    allPrefs->setValue(QString("FormOpenProject"),QVariant(LastSize_FormOpenProject));
    allPrefs->setValue(QString("FormSaveProject"),QVariant(LastSize_FormSaveProject));
    allPrefs->setValue(QString("FormSelectHotLink"),QVariant(LastSize_FormSelectHotLink));
    allPrefs->setValue(QString("FormReadParameters"),QVariant(LastSize_FormReadParameters));
    allPrefs->setValue(QString("FormWriteParameters"),QVariant(LastSize_FormWriteParameters));
    allPrefs->setValue(QString("FormSelectOutputFile"),QVariant(LastSize_FormSelectOutputFile));
    allPrefs->setValue(QString("FormOpenFitPara"),QVariant(LastSize_FormOpenFitPara));
    allPrefs->setValue(QString("FormSaveFitPara"),QVariant(LastSize_FormSaveFitPara));
    allPrefs->setValue(QString("FormReadBinary"),QVariant(LastSize_FormReadBinary));
    allPrefs->setValue(QString("FormWriteBinary"),QVariant(LastSize_FormWriteBinary));
    allPrefs->setValue(QString("FormCSVImport"),QVariant(LastSize_FormCSVImport));
    allPrefs->setValue(QString("FormCSVImportV2"),QVariant(LastSize_FormCSVImportV2));
    allPrefs->setValue(QString("FormProgress"),QVariant(LastSize_FormProgress));
    allPrefs->setValue(QString("FormCreateAltAxis"),QVariant(LastSize_CreateAltAxis));
    allPrefs->setValue(QString("FormBailout"),QVariant(LastSize_Bailout));
    allPrefs->setValue(QString("FormQuestion"),QVariant(LastSize_FormQuestion));
    allPrefs->setValue(QString("FormSimpleListSel"),QVariant(LastSize_FormSimpleListSel));
    allPrefs->setValue(QString("FormReportFitParameters"),QVariant(LastSize_FormReportFitParameters));
    allPrefs->setValue(QString("FormGeometricEvaluation"),QVariant(LastSize_FormGeometricEvaluation));
    allPrefs->setValue(QString("FormBinaryImportFilter"),QVariant(LastSize_FormBinaryImportFilter));
    allPrefs->setValue(QString("FormEditBlockData"),QVariant(LastSize_FormEditBlockData));
    allPrefs->setValue(QString("FormFontTool"),QVariant(LastSize_FormFontTool));
    allPrefs->setValue(QString("FormAbout"),QVariant(LastSize_FormAbout));
    allPrefs->setValue(QString("FormSetEditor"),QVariant(LastSize_FormSetEditor));
    allPrefs->setValue(QString("FormUndoList"),QVariant(LastSize_FormUndoList));
    allPrefs->setValue(QString("DeviceActivator"),QVariant(LastSize_DeviceActivator));
    allPrefs->setValue(QString("Form_Preferences"),QVariant(LastSize_Form_Preferences));
    allPrefs->setValue(QString("Form_AgrInfo"),QVariant(LastSize_Form_AgrInfo));
    allPrefs->setValue(QString("Form_SetupWizard"),QVariant(LastSize_Form_SetupWizard));
    allPrefs->setValue(QString("Form_InitWizard"),QVariant(LastSize_Form_InitWizard));
    allPrefs->setValue(QString("FormIOForm"),QVariant(LastSize_FormIOForm));
    allPrefs->setValue(QString("EditColumnProp"),QVariant(LastSize_EditColumnProp));
    allPrefs->setValue(QString("Spreadsheets"),QVariant(LastSize_Spreadsheets));
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

void initDefaultWindowSizes(void)
{
LastSize_EllipseProps=QSize(230,520);//QSize(224,224);
LastSize_EditEllipseProps=QSize(230,520);//QSize(100,30);
LastSize_BoxProps=QSize(230,520);//QSize(224,224);
LastSize_EditBoxProps=QSize(230,520);//QSize(100,30);
LastSize_TextProps=QSize(350,340);//QSize(224,275);
LastSize_EditTextProps=QSize(350,454);//QSize(100,30);
LastSize_LineProps=QSize(224,342);
LastSize_EditLineProps=QSize(230,580);//QSize(100,30);
LastSize_FormDrawObjects=QSize(239,236);
LastSize_FormConsole=QSize(500,250);
LastSize_FormEvalExpr=QSize(534,700);
LastSize_FormSetAppearance=QSize(502,600);
LastSize_FormGraphAppearance=QSize(484,594);
LastSize_FormPlotAppearance=QSize(405,605);//QSize(326,549);
LastSize_FormLocatorProps=QSize(327,316);
LastSize_FormAxisProperties=QSize(545,623);
LastSize_FormPointExplorer=QSize(710,260);//QSize(701,233);
LastSize_FormNonlinCurveFit=QSize(652,700);
LastSize_Form2DFit=QSize(500,500);
LastSize_FormInterpolation=QSize(614,700);
LastSize_FormSetOperations=QSize(534,530);
LastSize_FormExplorer=QSize(536,630);
LastSize_FormRTIManage=QSize(260,294);
LastSize_FormCommands=QSize(505,505);
LastSize_FormDeviceOptions=QSize(245,134);
LastSize_FormDeviceSetup=QSize(692,421);
LastSize_FormUserDefaultDevice=QSize(1064,153);
LastSize_FormArrangeGraphs=QSize(440,500);//QSize(414,540);
LastSize_FormOverlayGraphs=QSize(281,504);
LastSize_FormAutoScaleGraphs=QSize(260,316);
LastSize_FormDataSetProperties=QSize(586,561);
LastSize_FormSetOPS=QSize(260,343);
LastSize_FormFeatureExtraction=QSize(250,600);
LastSize_FormHistograms=QSize(601,700);
LastSize_FormFourier=QSize(350,438);
LastSize_FormFourier2=QSize(534,700);
LastSize_FormLoadAndEvaluate=QSize(580,174);
LastSize_FormIntegration=QSize(260,300);
LastSize_FormSeasonDiff=QSize(260,272);
LastSize_FormDifferences=QSize(260,272);
LastSize_FormAverages=QSize(260,350);
LastSize_FormRegression=QSize(628,384);
LastSize_FormCorrelation=QSize(260,530);
LastSize_FormConvolution=QSize(260,472);
LastSize_FormDigitalFilter=QSize(260,472);
LastSize_FormPruneData=QSize(260,444);
LastSize_FormSamplePoints=QSize(260,380);
LastSize_FormGeometricTransform=QSize(260,508);
LastSize_FormHotLinks=QSize(430,400);//QSize(403,542);
LastSize_FormRegionMaster=QSize(620,276);
LastSize_FormRegionStatus=QSize(160,166);
LastSize_FormReportRegion=QSize(261,96);
LastSize_FormClearRegion=QSize(180,72);//QSize(164,64);
LastSize_FormDefineRegion=QSize(230,110);
LastSize_FormNetCDF=QSize(407,504);
LastSize_FormReadSets=QSize(526,700);
LastSize_FormReadNetCDF=QSize(526,428);
LastSize_FormWriteSets=QSize(526,700);
LastSize_FormOpenProject=QSize(526,700);
LastSize_FormSaveProject=QSize(526,686);
LastSize_FormSelectHotLink=QSize(526,428);
LastSize_FormReadParameters=QSize(526,428);
LastSize_FormWriteParameters=QSize(526,466);
LastSize_FormSelectOutputFile=QSize(526,428);
LastSize_FormOpenFitPara=QSize(526,428);
LastSize_FormSaveFitPara=QSize(526,466);
LastSize_FormReadBinary=QSize(100,100);//should be unused
LastSize_FormWriteBinary=QSize(526,700);
LastSize_FormCSVImport=QSize(580,482);
LastSize_FormCSVImportV2=QSize(780,500);///update!
LastSize_FormProgress=QSize(150,50);//QSize(100,30);
LastSize_FormQuestion=QSize(150,50);//QSize(100,30);
LastSize_FormSimpleListSel=QSize(532,92);
LastSize_FormReportFitParameters=QSize(420,350);
LastSize_FormGeometricEvaluation=QSize(270,400);
LastSize_FormBinaryImportFilter=QSize(540,510);//QSize(534,452);
LastSize_FormEditBlockData=QSize(266,750);
LastSize_FormFontTool=QSize(460,570);
LastSize_FormAbout=QSize(560,700);
LastSize_FormSetEditor=QSize(367,254);
LastSize_FormUndoList=QSize(260,274);
LastSize_DeviceActivator=QSize(170,350);
LastSize_Form_Preferences=QSize(752,820);//770;694
LastSize_Form_AgrInfo=QSize(640,480);
LastSize_Form_SetupWizard=QSize(100,100);//Unused
LastSize_Form_InitWizard=QSize(800,600);
LastSize_FormIOForm=QSize(100,100);//should be unused
LastSize_EditColumnProp=QSize(265,140);
LastSize_Spreadsheets=QSize(410,350);//QSize(260,325);
LastSize_Bailout=QSize(440,100);//QSize(380,90);
LastSize_CreateAltAxis=QSize(575,260);
}

void removeAllWindows(void)//calls the destructor of every existing dialog
{
    N_DELETE(EllipseProps);
    N_DELETE(EditEllipseProps);
    N_DELETE(BoxProps);
    N_DELETE(EditBoxProps);
    N_DELETE(TextProps);
    N_DELETE(EditTextProps);
    N_DELETE(LineProps);
    N_DELETE(EditLineProps);
    N_DELETE(FormDrawObjects);
    N_DELETE(FormConsole);
    N_DELETE(FormEvalExpr);
    N_DELETE(FormSetAppearance);
    N_DELETE(FormGraphAppearance);
    N_DELETE(FormPlotAppearance);
    N_DELETE(FormLocatorProps);
    N_DELETE(FormAxisProperties);
    N_DELETE(FormPointExplorer);
    N_DELETE(FormNonlinCurveFit);
    N_DELETE(Form2DFit);
    N_DELETE(FormInterpolation);
    N_DELETE(FormSetOperations);
    N_DELETE(FormExplorer);
    N_DELETE(FormRTIManage);
    N_DELETE(FormCommands);
    //N_DELETE(FormDeviceSetup);
    N_DELETE(FormArrangeGraphs);
    N_DELETE(FormOverlayGraphs);
    N_DELETE(FormAutoScaleGraphs);
    N_DELETE(FormDataSetProperties);
    N_DELETE(FormSetOPS);
    N_DELETE(FormFeatureExtraction);
    N_DELETE(FormHistograms);
    N_DELETE(FormFourier);
    N_DELETE(FormFourier2);
    N_DELETE(FormLoadAndEvaluate);
    N_DELETE(FormIntegration);
    N_DELETE(FormSeasonDiff);
    N_DELETE(FormDifferences);
    N_DELETE(FormAverages);
    N_DELETE(FormRegression);
    N_DELETE(FormCorrelation);
    N_DELETE(FormConvolution);
    N_DELETE(FormDigitalFilter);
    N_DELETE(FormPruneData);
    N_DELETE(FormSamplePoints);
    N_DELETE(FormGeometricTransform);
    N_DELETE(FormHotLinks);
    //N_DELETE(FormSelectHotLink);
    N_DELETE(FormRegionMaster);
    N_DELETE(FormRegionStatus);
    N_DELETE(FormReportRegion);
    N_DELETE(FormClearRegion);
    N_DELETE(FormDefineRegion);
    N_DELETE(FormNetCDF);
    //N_DELETE(FormReadNetCDF);
    N_DELETE(FormReadSets);
    N_DELETE(FormWriteSets);
    N_DELETE(FormOpenProject);
    N_DELETE(FormSaveProject);
    N_DELETE(FormReadParameters);
    N_DELETE(FormWriteParameters);
    N_DELETE(FormSelectOutputFile);
    N_DELETE(FormReadBinary);
    N_DELETE(FormWriteBinary);
    N_DELETE(FormCSVImport);
    N_DELETE(FormCSVImportV2);
    N_DELETE(FormProgress);
    N_DELETE(FormQuestion);
    N_DELETE(FormSimpleListSel);
    N_DELETE(FormReportFitParameters);
    N_DELETE(FormGeometricEvaluation);
    //N_DELETE(FormBinaryImportFilter);
    N_DELETE(FormEditBlockData);
    N_DELETE(FormFontTool);
    N_DELETE(FormAbout);
    N_DELETE(FormSetEditor);
    N_DELETE(FormUndoList);
    //N_DELETE(Form_Preferences);
    N_DELETE(Form_AgrInfo);
    N_DELETE(Form_SetupWizard);
    N_DELETE(Form_InitWizard);
    N_DELETE(FormAutoLabeler);
    N_DELETE(FormCreateAltAxis);
}

void UpdateAllWindowContents(void)//a "repaint"-funktion for all widgets
{
QLocale newLocale=(DecimalPointToUse=='.')?(*dot_locale):(*comma_locale);
//qDebug() << "updating all window contents; DecSep=#" << DecimalPointToUse << "#OldDecimalPoint=#" << OldDecimalPoint << "#";
    if (Form2DFit)//This has to be updated before anything else or the updates give Sytax-Errors (because , or . is wrong either way)
    {
        Form2DFit->selNrOfPoints->setLocale(newLocale);
        for (int i=0;i<8;i++)
        {
            Form2DFit->selParameter[i]->ReplaceNumberContents();
        }
    }
    update_all();//updates everything else
    update_Line_Editors();
    updateUniLists();
    if (mainWin)
    {
        mainWin->sldPageZoom->redisplay();
        mainWin->setFont(qApp->font());
    }
    if (EllipseProps)
    {
        EllipseProps->flp->reDisplayContents();
    }
    if (EditEllipseProps)
    {
        EditEllipseProps->flp->reDisplayContents();
    }
    if (BoxProps)
    {
        BoxProps->flp->reDisplayContents();
    }
    if (EditBoxProps)
    {
        EditBoxProps->flp->reDisplayContents();
    }
    if (TextProps)
    {
        TextProps->flp->reDisplayContents();
    }
    if (EditTextProps)
    {
        EditTextProps->flp->reDisplayContents();
    }
    if (LineProps)
    {
        LineProps->flp->reDisplayContents();
    }
    if (EditLineProps)
    {
        EditLineProps->flp->reDisplayContents();
    }
    if (FormDrawObjects)
    {
        FormDrawObjects->redisplayButtons();
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
        FormPlotAppearance->flp->redisplayContents();
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
        FormNonlinCurveFit->adjustToFontSize();
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
        FormExplorer->PlotAppearance->redisplayContents();
        FormExplorer->GraphProperties->redisplayContents();
        FormExplorer->SetProperties->redisplayContents();
        FormExplorer->AxisProperties->redisplayContents();
        FormExplorer->LineProperties->reDisplayContents();
        FormExplorer->TextProperties->reDisplayContents();
        FormExplorer->EllipseProperties->reDisplayContents();
        FormExplorer->BoxProperties->reDisplayContents();
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
        FormArrangeGraphs->selUpperOffset->spnLineWidth->setLocale(newLocale);
        FormArrangeGraphs->selUpperOffset->setValue(FormArrangeGraphs->selUpperOffset->value());
        FormArrangeGraphs->selLowerOffset->spnLineWidth->setLocale(newLocale);
        FormArrangeGraphs->selLowerOffset->setValue(FormArrangeGraphs->selLowerOffset->value());
        FormArrangeGraphs->selLeftOffset->spnLineWidth->setLocale(newLocale);
        FormArrangeGraphs->selLeftOffset->setValue(FormArrangeGraphs->selLeftOffset->value());
        FormArrangeGraphs->selRightOffset->spnLineWidth->setLocale(newLocale);
        FormArrangeGraphs->selRightOffset->setValue(FormArrangeGraphs->selRightOffset->value());
        FormArrangeGraphs->selHGap->spnLineWidth->setLocale(newLocale);
        FormArrangeGraphs->selHGap->setValue(FormArrangeGraphs->selHGap->value());
        FormArrangeGraphs->selVGap->spnLineWidth->setLocale(newLocale);
        FormArrangeGraphs->selVGap->setValue(FormArrangeGraphs->selVGap->value());
        FormArrangeGraphs->layout0->update();
        FormArrangeGraphs->layout1->update();
        FormArrangeGraphs->layout2->update();
        FormArrangeGraphs->layout3->update();
        FormArrangeGraphs->layout->update();
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
        FormDataSetProperties->adjustSizeToFont();
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
        FormRegionStatus->background->adjustSize();
        FormRegionStatus->layout0->update();
        FormRegionStatus->layout->update();
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
    if (FormRegionMaster)
    {
        FormRegionMaster->updateDecimalSeparator();
    }
    if (FormNetCDF)
    {
        //Nothing to change here!
    }
    if (FormReadSets)
    {
        FormReadSets->selector->changeIconSizes();
    }
    if (FormReadNetCDF)
    {
        FormReadNetCDF->selector->changeIconSizes();
    }
    if (FormWriteSets)
    {
        FormWriteSets->selector->changeIconSizes();
    }
    if (FormOpenProject)
    {
        FormOpenProject->selector->changeIconSizes();
    }
    if (FormSaveProject)
    {
        FormSaveProject->selector->changeIconSizes();
        /// Encoding of project description like in plot appearance
    }
    if (FormSelectHotLink)
    {
        FormSelectHotLink->selector->changeIconSizes();
    }
    if (FormReadParameters)
    {
        FormReadParameters->selector->changeIconSizes();
    }
    if (FormWriteParameters)
    {
        FormWriteParameters->selector->changeIconSizes();
    }
    if (FormSelectOutputFile)
    {
        FormSelectOutputFile->selector->changeIconSizes();
    }
    if (FormReadBinary)
    {
        FormReadBinary->selector->changeIconSizes();
    }
    if (FormWriteBinary)
    {
        FormWriteBinary->selector->changeIconSizes();
    }
    if (FormCSVImport)
    {
        //Nothing to change here!
    }
    if (FormCSVImportV2)
    {
        //Nothing to change here!
    }
    if (FormBinaryImportFilter)
    {
        //FormBinaryImportFilter->tabDataInfo->selTriggerValue->spnLineWidth->setLocale(newLocale);
        FormBinaryImportFilter->tabDataInfo->selTriggerValue->ReplaceNumberContents();
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
    if (FormReportFitParameters)
    {
        for (int i=0;i<MAXPARM;i++)
        {
        FormReportFitParameters->spnPara[i]->setLocale(newLocale);
        }
        FormReportFitParameters->init();
        FormReportFitParameters->layout->update();
    }
    if (FormUserDefaultGeometries)
    {
        FormUserDefaultGeometries->redisplaySeparators();
    }
    update_encoding();
    //Switching complete --> set old=new to prevent unnecessary switching
    OldDecimalPoint=DecimalPointToUse;
}

void generateCommentAndLegendAfterImport(const char * filename,int set,int col1,int col2,char * new_comment,char * new_legend)
{
    //qDebug() << "GENERATING COMMENT AND LEGEND";
QFileInfo fi(QString::fromLocal8Bit(filename));//filename is including the path
//qDebug() << "generateCommentAndLegendAfterImport: fi.filename" << QDir::toNativeSeparators(fi.absoluteFilePath());
char * filename_only=new char[GR_MAXPATHLEN];
char * filename_w_path=new char[GR_MAXPATHLEN];
strcpy(filename_only,fi.fileName().toLocal8Bit().constData());
QString tmp_filename_w_path=QDir::toNativeSeparators(fi.absoluteFilePath());
newLegendString.clear();
newCommentString.clear();
if (QDir::separator()=='\\')
{
tmp_filename_w_path.replace("\\","\\\\");
}
strcpy(filename_w_path,tmp_filename_w_path.toLocal8Bit().constData());
switch (drop_comment)
{
case DROP_LEGEND_NONE:
strcpy(new_comment,"");
break;
case DROP_LEGEND_PATH_FILE_COLUMN:
sprintf(new_comment,"Imported from %s, Set %d, Columns %d and %d",filename_w_path,set,col1,col2);
newCommentString=QObject::tr("Imported from ")+QDir::toNativeSeparators(fi.absoluteFilePath())+QObject::tr(", Set ")+QString::number(set)+QObject::tr(", Columns ")+QString::number(col1)+QObject::tr(" and ")+QString::number(col2);
break;
case DROP_LEGEND_PATH_FILE:
sprintf(new_comment,"Imported from %s",filename_w_path);
newCommentString=QObject::tr("Imported from ")+QDir::toNativeSeparators(fi.absoluteFilePath());
break;
case DROP_LEGEND_FILE_COLUMN:
sprintf(new_comment,"Imported from %s, Set %d, Columns %d and %d",filename_only,set,col1,col2);
newCommentString=QObject::tr("Imported from ")+fi.fileName()+QObject::tr(", Set ")+QString::number(set)+QObject::tr(", Columns ")+QString::number(col1)+QObject::tr(" and ")+QString::number(col2);
break;
case DROP_LEGEND_FILE:
sprintf(new_comment,"Imported from %s",filename_only);
newCommentString=QObject::tr("Imported from ")+fi.fileName();
break;
case DROP_LEGEND_COLUMN:
sprintf(new_comment,"Set %d, Columns %d and %d",set,col1,col2);
newCommentString=QObject::tr("Set ")+QString::number(set)+QObject::tr(", Columns ")+QString::number(col1)+QObject::tr(" and ")+QString::number(col2);
break;
}
switch (drop_legend)
{
case DROP_LEGEND_NONE:
strcpy(new_legend,"");
break;
case DROP_LEGEND_PATH_FILE_COLUMN:
sprintf(new_legend,"Imported from %s, Set %d, Columns %d and %d",filename_w_path,set,col1,col2);
newLegendString=QObject::tr("Imported from ")+QDir::toNativeSeparators(fi.absoluteFilePath())+QObject::tr(", Set ")+QString::number(set)+QObject::tr(", Columns ")+QString::number(col1)+QObject::tr(" and ")+QString::number(col2);
break;
case DROP_LEGEND_PATH_FILE:
sprintf(new_legend,"Imported from %s",filename_w_path);
newLegendString=QObject::tr("Imported from ")+QDir::toNativeSeparators(fi.absoluteFilePath());
break;
case DROP_LEGEND_FILE_COLUMN:
sprintf(new_legend,"Imported from %s, Set %d, Columns %d and %d",filename_only,set,col1,col2);
newLegendString=QObject::tr("Imported from ")+fi.fileName()+QObject::tr(", Set ")+QString::number(set)+QObject::tr(", Columns ")+QString::number(col1)+QObject::tr(" and ")+QString::number(col2);
break;
case DROP_LEGEND_FILE:
sprintf(new_legend,"Imported from %s",filename_only);
newLegendString=QObject::tr("Imported from ")+fi.fileName();
break;
case DROP_LEGEND_COLUMN:
sprintf(new_legend,"Set %d, Columns %d and %d",set,col1,col2);
newLegendString=QObject::tr("Set ")+QString::number(set)+QObject::tr(", Columns ")+QString::number(col1)+QObject::tr(" and ")+QString::number(col2);
break;
}
if (QDir::separator()=='\\')
{
newCommentString.replace("\\","\\\\");
newLegendString.replace("\\","\\\\");
}
strcpy(new_comment,newCommentString.toLocal8Bit().constData());
strcpy(new_legend,newLegendString.toLocal8Bit().constData());
delete[] filename_only;
delete[] filename_w_path;
}

void generateCommentAndLegendAfterImportV2(const char * filename,QStringList target_cols,QStringList target_col_origins,char * new_comment,char * new_legend)
{
    //qDebug() << "GENERATING COMMENT AND LEGEND V2";
QFileInfo fi(QString::fromLocal8Bit(filename));//filename is including the path
char * filename_only=new char[GR_MAXPATHLEN];
char * filename_w_path=new char[GR_MAXPATHLEN];
strcpy(filename_only,fi.fileName().toLocal8Bit().constData());
QString tmp_filename_w_path=QDir::toNativeSeparators(fi.absoluteFilePath());
QString colsource;
colsource=QString("[ ");
for (int i=0;i<target_cols.length();i++)
{
if (i>0) colsource+=QString(", ");
colsource+=target_cols.at(i)+QString(" = ")+target_col_origins.at(i);
}
colsource+=QString(" ]");
if (QDir::separator()=='\\')
{
tmp_filename_w_path.replace("\\","\\\\");
}
strcpy(filename_w_path,tmp_filename_w_path.toLocal8Bit().constData());
switch (drop_comment)
{
case DROP_LEGEND_NONE:
strcpy(new_comment,"");
break;
case DROP_LEGEND_PATH_FILE_COLUMN:
sprintf(new_comment,"Imported from %s, %s",filename_w_path,colsource.toLocal8Bit().constData());
break;
case DROP_LEGEND_PATH_FILE:
sprintf(new_comment,"Imported from %s",filename_w_path);
break;
case DROP_LEGEND_FILE_COLUMN:
sprintf(new_comment,"Imported from %s, %s",filename_only,colsource.toLocal8Bit().constData());
break;
case DROP_LEGEND_FILE:
sprintf(new_comment,"Imported from %s",filename_only);
break;
case DROP_LEGEND_COLUMN:
sprintf(new_comment,"%s",colsource.toLocal8Bit().constData());
break;
}
switch (drop_legend)
{
case DROP_LEGEND_NONE:
strcpy(new_legend,"");
break;
case DROP_LEGEND_PATH_FILE_COLUMN:
sprintf(new_legend,"Imported from %s, %s",filename_w_path,colsource.toLocal8Bit().constData());
break;
case DROP_LEGEND_PATH_FILE:
sprintf(new_legend,"Imported from %s",filename_w_path);
break;
case DROP_LEGEND_FILE_COLUMN:
sprintf(new_legend,"Imported from %s, %s",filename_only,colsource.toLocal8Bit().constData());
break;
case DROP_LEGEND_FILE:
sprintf(new_legend,"Imported from %s",filename_only);
break;
case DROP_LEGEND_COLUMN:
sprintf(new_legend,"%s",colsource.toLocal8Bit().constData());
break;
}
delete[] filename_only;
delete[] filename_w_path;
}

void GeneralPaste(const QMimeData * mimeData)
{
    QTemporaryFile * temp_file=new QTemporaryFile;
    QString str,str2,str_imp;
    //QFileInfo finfo;
    //QString suffix;
    char * header_name;
    bool is_diadem_file,is_header_file,is_agr,is_gould,is_csv,is_text;//,got_dirty=true;
    int bin_format_found;
    int std_schema_nr,t_id;
    int commas,fullstops,crs,lfs,ret,ret2;
    char * filename=NULL;
    int files=0,n_of_imp_sets=0,n_of_imp_agrs=0;
    char ** filenames=NULL;

    char dummy[GR_MAXPATHLEN];
    char newcomment[GR_MAXPATHLEN];
    char newlegend[GR_MAXPATHLEN];
    ofstream ofi;
    int * gnos,*snos,*sdos,len;
    int deltasize=32,m1,m2;
    struct block_input_format block;
    double * tmp_col=NULL;
    bool temp_file_used;

    QStringList find_list;
    QString find_filename;
    QStringList found_bin_data_files,found_bin_header_files;

    QList<int> target_graphs,target_sets;

    gnos=new int[deltasize];
    snos=new int[deltasize];
    init_block_input_data(&block);
    //cout << "General Paste Event" << endl;
/*
QStringList sl=mimeData->formats();
for (int i=0;i<sl.length();i++)
cout << sl.at(i).toLocal8Bit().constData() << endl;
cout << endl;
*/
    yestoall=0;
    if (noask==TRUE)
    {
    yestoall=1;
    }
    Form_AgrInfo->reset_import_counters();
    stop_repaint=TRUE;
    set_wait_cursor();
    len=0;
    ListOfChanges.clear();
    ListOfOldStates.clear();
    if (mimeData->hasText() && !mimeData->hasUrls())//text implies ascii-data - or a header-file
    {
    /// todo: check for header file --> should not be necessary: if it is a header, we do not know to which data-file it belongs
        if (temp_file->open())
        {
            str=mimeData->text();
            commas=str.count(",");
            fullstops=str.count(".");
            crs=str.count(QChar(0x0D));
            lfs=str.count(QChar(0x0A));
//qDebug() << "CR=" << crs << " LF=" << lfs;
                if (lfs<=0 && crs>0)//no line feeds, but carridge return --> insert cr
                {
                str.replace(QChar(0x0D),QString(QChar(0x0D))+QString(QChar(0x0A)));
                }
//crs=str.count(QChar(0x0D));
//lfs=str.count(QChar(0x0A));
//qDebug() << "CR=" << crs << " LF=" << lfs;
            if (commas>fullstops*3)
                if (QMessageBox::question(mainWin,QObject::tr("Problem during data import"),QObject::tr("Significantly more commas ',' than fullstops '.' found.\nUse comma ',' as decimal separator?"),QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
                    exchange_point_comma=true;
            filename=new char[temp_file->fileName().length()+2];
            strcpy(filename,temp_file->fileName().toLocal8Bit().constData());
            ofi.open(filename);
            //ofi << mimeData->text().toLocal8Bit().constData() << endl;
            ofi << str.toLocal8Bit().constData() << endl;
            ofi.close();
            temp_file_used=true;
                if (drop_autoscale==1)
                autoscale_onread=AUTOSCALE_XY;
                else
                autoscale_onread=AUTOSCALE_NONE;
            curtype=0;//XY
            new_set_no=0;
            if (new_set_nos!=NULL)
            {
                delete[] new_set_nos;
                new_set_nos=NULL;//important!!!
            }
            prepare_strings_for_saving();
            getdata(get_cg(), filename, 0, LOAD_NXY);//0=FILE
            if (new_set_no<=0) getdata(get_cg(), filename, 0, LOAD_SINGLE);
            if (new_set_no>0 && auto_set_cwd)
            {
                QFileInfo finfo(QString::fromLocal8Bit(filename));
                QString pa=finfo.absolutePath();
                set_workingdir(pa.toLocal8Bit().constData());
            }
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
        temp_file_used=false;
        filename=new char[urls.at(i).toLocalFile().length()+2];
        strcpy(filename,urls.at(i).toLocalFile().toLocal8Bit().constData());
        ///Test binary-file-recognition and -completion
//qDebug() << "============================";
        find_filename=urls.at(i).toLocalFile();
//qDebug() << "Testing file for best match:" << find_filename;
        bin_format_found=find_std_format_and_matching_files(find_filename,std_schema_nr,find_list,is_header_file);
/*qDebug() << "============================";
qDebug() << "Result=" << bin_format_found << "StdFormat=" << std_schema_nr << "Is_Header=" << is_header_file;
if (bin_format_found==RETURN_SUCCESS)
qDebug() << "IsCompressed=" << std_bin_import_settings[std_schema_nr].data_is_compressed;
qDebug() << "Matching Files:" << find_list;*/
        found_bin_data_files.clear();
        found_bin_header_files.clear();
        if (bin_format_found==RETURN_SUCCESS)
        {
            if (is_header_file==true || (std_bin_import_settings[std_schema_nr].header_present==true && std_bin_import_settings[std_schema_nr].header_format>1))//in this case: we have one file but need at least one other file
            {
                for (int kl=0;kl<find_list.length();kl++)
                {
                    if (is_header_file)
                    {
                    found_bin_header_files << urls.at(i).toLocalFile();
                    found_bin_data_files << find_list.at(kl);
                    }
                    else//binary data file
                    {
                    found_bin_data_files << urls.at(i).toLocalFile();
                    found_bin_header_files << find_list.at(kl);
                    }
                }
            }
            else//we have a binary data file and need nothing else
            {
            found_bin_data_files << urls.at(i).toLocalFile();
            found_bin_header_files << QString("");
            }
        }
        /// Todo: check every file for beeing agr or header file
        //finfo=QFileInfo(filename);
        //suffix=finfo.suffix();
        is_gould=is_agr=is_csv=is_diadem_file=false;
        header_name=NULL;
//at first: test for diadem-format and binary data
        is_gould=(bool)is_Gould_file(filename);
        is_text=(bool)is_text_file(filename);
        //bin_format_found=guess_bin_format(urls.at(i).toLocalFile(),std_schema_nr,is_header_file);
        if (is_gould==false)
        {
        is_agr=(bool)is_agr_file(filename);
            if (is_agr==false)
            {
                is_csv=(bool)is_csv_file(filename);
                if (is_csv==false)
                {
                is_diadem_file=is_diadem(filename,&header_name);
                }
            }
        }
//qDebug() << "is_text=" << is_text << "BinFormatFound:" << bin_format_found << "StdFormat=" << std_schema_nr;
//qDebug() << "is_agr=" << is_agr << "is_csv=" << is_csv << "is_diadem_file=" << is_diadem_file;
        if (is_diadem_file==false && header_name!=NULL)
        {
        delete[] header_name;
        header_name=NULL;
        }

        if (is_agr==true)//QString::compare(suffix,QString("agr"),Qt::CaseInsensitive)==0)//it is an .agr-file
        {
        Form_AgrInfo->init(filename);
        Form_AgrInfo->exec();
//qDebug() << "GeneralPaste: Comment=" << g[0].p[0].comments;
        continue;
        }
        else if (is_gould==true)//special case Gould-Trace
        {
        //cout << "Gould-File" << endl;
        int nr_of_trace_points=0;
        double * trace_x=new double[2];
        double * trace_y=new double[2];
        try_loading_Gould(filename,nr_of_trace_points,&trace_x,&trace_y);//dummy2 is set here
        /*cout << "legend1=" << dummy << endl;
        cout << "legend2=" << dummy2 << endl;*/
        if (nr_of_trace_points>0)
        {
            if (auto_set_cwd)
            {
                QFileInfo finfo(QString::fromLocal8Bit(filename));
                QString pa=finfo.absolutePath();
                set_workingdir(pa.toLocal8Bit().constData());
            }
        t_id=nextset(get_cg());
        setlength(get_cg(),t_id,nr_of_trace_points);
        if (inhibit_set_legend==0)
        set_legend_string(get_cg(),t_id,dummy2);
        //cout << "legend=" << dummy << endl;
            for (int ijk=0;ijk<nr_of_trace_points;ijk++)
            {
            g[get_cg()].p[t_id].data.ex[0][ijk]=trace_x[ijk];
            g[get_cg()].p[t_id].data.ex[1][ijk]=trace_y[ijk];
            }
        if (number_of_real_sets(get_cg())>0 && drop_autoscale==1)
        mainWin->doAutoScale();
        }
            delete[] trace_x;
            delete[] trace_y;
        continue;
        }
        else if (is_csv==true)
        {//guessing does reading as well
            if (def_csv_import_format.valid_settings)
            {
            target_graphs.clear();
            target_sets.clear();
            target_graphs << get_cg();
            target_sets << -1;
//qDebug() << "simpleReadCSVFileLikeFormat start";
            simpleReadCSVFileLikeFormat(urls.at(i).toLocalFile(),def_csv_import_format,target_graphs,target_sets);
//qDebug() << "simpleReadCSVFileLikeFormat finished";
                if (number_of_real_sets(get_cg())>0 && drop_autoscale==1)
                mainWin->doAutoScale();
            continue;
            }
            else
            {
            block.nr_of_errors=guess_block_data_column_format(urls.at(i).toLocalFile().toLocal8Bit().constData(),block,5,drop_dec_sep);

//qDebug() << "Reading csv-File; nr_of_errors=" << block.nr_of_errors << "nr_of_sets=" << block.nr_of_sets;
//qDebug() << "default_format=" << def_csv_import_format.valid_settings << "suggestions=" << def_csv_import_format.csv_column_formats_suggestion;
//def_csv_import_format

        if (block.nr_of_sets>0 && auto_set_cwd)
        {
            QFileInfo finfo(urls.at(i).toLocalFile());
            QString pa=finfo.absolutePath();
            set_workingdir(pa.toLocal8Bit().constData());
        }

            for (int i=0;i<block.nr_of_sets;i++)
            {
                if (block.set_infos[i].nr_of_rows>0)
                {
                    for (int jkl=1;jkl<block.set_infos[i].nr_of_columns;jkl++)
                    {
                    t_id=nextset(get_cg());
                    setlength(get_cg(),t_id,block.set_infos[i].nr_of_rows);
                    //sprintf(dummy2,"Imported from %s, Set %d, Columns 0 and %d",urls.at(i).toLocalFile().toLocal8Bit().constData(),i,jkl);
                    //generateCommentAndLegendAfterImport(urls.at(i).toLocalFile().toUtf8().constData(),i,0,jkl,newcomment,newlegend);
                    generateCommentAndLegendAfterImport(urls.at(i).toLocalFile().toLocal8Bit().constData(),i,0,jkl,newcomment,newlegend);
                    if (inhibit_set_legend==0)
                    set_legend_string(get_cg(),t_id,newlegend);
                    setcomment(get_cg(),t_id,newcomment);
                    (void)get_block_data_set_double_column(block.set_infos[i],0,&tmp_col);
                    memcpy(g[get_cg()].p[t_id].data.ex[0],tmp_col,sizeof(double)*block.set_infos[i].nr_of_rows);
                    (void)get_block_data_set_double_column(block.set_infos[i],jkl,&tmp_col);
                    memcpy(g[get_cg()].p[t_id].data.ex[1],tmp_col,sizeof(double)*block.set_infos[i].nr_of_rows);
                    }
//qDebug() << "Set" << i << ":" << block.set_infos[i].nr_of_rows << "x" <<  block.set_infos[i].nr_of_columns;
                }
            }
            if (tmp_col!=NULL)
            {
            delete[] tmp_col;
            tmp_col=NULL;
            }
            if (number_of_real_sets(get_cg())>0 && drop_autoscale==1)
            mainWin->doAutoScale();
            /*mainWin->ImportCSV();
            FormCSVImport->ledFileName->setText(urls.at(i).toLocalFile());
            FormCSVImport->doReguess();*/
            continue;
            }
        }
        else if (is_diadem_file==true)//special case: diadem file
        {
        struct DIAdem_Header dia_header_infos;
        struct importSettings bin_import;
        initSettings(bin_import,false);
        //qDebug() << "DIADEM recognized";
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
        if (number_of_real_sets(get_cg())>0 && drop_autoscale==1)
        mainWin->doAutoScale();

        if (n_snos!=NULL) delete[] n_snos;
        if (n_gnos!=NULL) delete[] n_gnos;
        if (header_name!=NULL) delete[] header_name;
        continue;//next url/file
        }
        else if (bin_format_found==RETURN_SUCCESS)// && (is_text==false || is_header_file==true))//look for a std binary format
        {/// std-binary-format
/*qDebug() << "GUESSED BINARY SCHEMA=" << std_schema_nr << "Schema-Channels=" << std_bin_import_settings[std_schema_nr].channels;
for (int kl=0;kl<std_bin_import_settings[std_schema_nr].channels;kl++)
{
qDebug() << "Target=" << std_bin_import_settings[std_schema_nr].channel_target[kl];
}*/
            for (int kl=0;kl<found_bin_data_files.length();kl++)
            {
            copy_std_settings_to_current_bin_import(urls.at(i).toLocalFile(),std_schema_nr,is_header_file);
            current_bin_import_settings.HeaderFile=found_bin_header_files.at(kl);
            current_bin_import_settings.DataFile=found_bin_data_files.at(kl);
            if (current_bin_import_settings.HeaderFile.isEmpty()) current_bin_import_settings.HeaderFile=current_bin_import_settings.DataFile;
            current_bin_import_settings.valid_status=0;
                //qDebug() << "Reading: Header=" << current_bin_import_settings.HeaderFile;
                //qDebug() << "Reading: Data=" << current_bin_import_settings.DataFile;
            //we now have the settings
            //-->read the header data (if present)
            readHeaderData(current_bin_import_settings,std_bin_import_settings[std_schema_nr]);
            //now read the actual binary data
                if (i==urls.length()-1 && kl==found_bin_data_files.length()-1)//last file --> allow autoscale, but no replot (stop_repaint=TRUE; prevents replot)
                {
                //read_std_bin_file(filename,std_schema_nr,is_header_file,false);
                    if (drop_autoscale==1)
                    read_bin_file_by_current_settings(false);
                    else
                    read_bin_file_by_current_settings(true);
                }
                else//no autoscale, no replot
                {
                //read_std_bin_file(filename,std_schema_nr,is_header_file,true);
                read_bin_file_by_current_settings(true);
                }

                if (auto_set_cwd)
                {
                    QFileInfo finfo(urls.at(i).toLocalFile());
                    QString pa=finfo.absolutePath();
                    set_workingdir(pa.toLocal8Bit().constData());
                }
            }//end kl-loop over all found matching bin-files
        continue;//next url/file
        }
//qDebug() << "try loading file as text file:" << filename;
        //normal file (unrecognized)
            FILE * fn=fopen(filename,"r");
            QTextStream * ts=new QTextStream(fn);
            str=ts->readAll();
            str2=QString(str.toLocal8Bit());
            ret=QMessageBox::No;
            fclose(fn);
            int print_counter=0;
            int string_len=str.length();
                for (int jkl=0;jkl<string_len;jkl++)
                {
                if (str.at(jkl).isPrint()==true || str.at(jkl).isSpace()==true) print_counter++;
                }
//qDebug() << "str2.length()=" << str2.length() << "str.length()=" << str.length() << "print_counter/string_len=" << print_counter*100.0/string_len << "%";
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
                //str_imp=QObject::tr("Imported from ") + QString(filename);
                sprintf(dummy,"READ NXY \"%s\"",filename);
                files++;
                commas=str.count(",");
                fullstops=str.count(".");
                crs=str.count(QChar(0x0D));
                lfs=str.count(QChar(0x0A));
//qDebug() << "CR=" << crs << " LF=" << lfs;

                if (lfs<=0 && crs>0)//no line feeds, but carridge return --> insert cr
                {
                str.replace(QChar(0x0D),QString(QChar(0x0D))+QString(QChar(0x0A)));
                }

                if (temp_file->open())
                {
                    delete[] filename;
                    filename=new char[temp_file->fileName().length()+2];
                    strcpy(filename,temp_file->fileName().toLocal8Bit().constData());
                    ofi.open(filename);
                    //ofi << mimeData->text().toLocal8Bit().constData() << endl;
                    ofi << str.toLocal8Bit().constData() << endl;
                    ofi.close();
                    temp_file_used=true;
                }

                exchange_point_comma=false;
//qDebug() << "0 exchange_point_comma=" << exchange_point_comma;
                if (commas>fullstops*3 && drop_dec_sep=='.')//more commas than points, but points selected
                {
                    if (yestoall==1)
                    ret2=QMessageBox::Yes;
                    else if (yestoall==2)
                    ret2=QMessageBox::No;
                    else
                    {
                    ret2=QMessageBox::question(mainWin,QObject::tr("Warning!"),QObject::tr("Problem during import of file ") + urls.at(i).toLocalFile() + QObject::tr("\nSignificantly more commas ',' than fullstops '.' found.\nUse comma ',' as decimal separator?\n(If you select 'No', fullstops '.' will be used.)"),QMessageBox::Yes|QMessageBox::No|QMessageBox::YesToAll|QMessageBox::NoToAll);
                        if (ret2==QMessageBox::YesToAll)
                        {
                        yestoall=1;
                        ret2=QMessageBox::Yes;
                        }
                        else if (ret2==QMessageBox::NoToAll)
                        {
                        yestoall=2;
                        ret2=QMessageBox::No;
                        }
                    }

                    if (ret2==QMessageBox::Yes)
                    {
                    exchange_point_comma=true;
                    }
                }
//qDebug() << "A exchange_point_comma=" << exchange_point_comma;
                if (commas>=fullstops*3 && drop_dec_sep==',') exchange_point_comma=true;//more commas than points found and actually commas selected
//qDebug() << "B exchange_point_comma=" << exchange_point_comma;
                if (commas<fullstops*3 && drop_dec_sep==',')//less points than commas, but commas selected
                {
                    if (yestoall==1)
                    ret2=QMessageBox::Yes;
                    else if (yestoall==2)
                    ret2=QMessageBox::No;
                    else
                    {
                    ret2=QMessageBox::question(mainWin,QObject::tr("Warning!"),QObject::tr("Problem during import of file ") + urls.at(i).toLocalFile() + QObject::tr("\nSignificantly more fullstops than commas found.\nReally use comma as decimal separator?\n(If you select 'No', points '.' will be used.)"),QMessageBox::Yes|QMessageBox::No|QMessageBox::YesToAll|QMessageBox::NoToAll);
                        if (ret2==QMessageBox::YesToAll)
                        {
                        yestoall=1;
                        ret2=QMessageBox::Yes;
                        }
                        else if (ret2==QMessageBox::NoToAll)
                        {
                        yestoall=2;
                        ret2=QMessageBox::No;
                        }
                    }

                    if (ret2==QMessageBox::Yes)
                    {
                    exchange_point_comma=true;
                    }
                }
//qDebug() << "C exchange_point_comma=" << exchange_point_comma;
                    if (drop_autoscale==1)
                    autoscale_onread=AUTOSCALE_XY;
                    else
                    autoscale_onread=AUTOSCALE_NONE;
                curtype=0;//XY
                new_set_no=0;
                if (new_set_nos!=NULL)
                {
                    delete[] new_set_nos;
                    new_set_nos=NULL;
                }
                prepare_strings_for_saving();
                getdata(get_cg(), filename, 0, LOAD_NXY);//actually do the reading
                if (new_set_no<=0) getdata(get_cg(), filename, 0, LOAD_SINGLE);//if nothing has been read we try to read again with SINGLE-SET-setting (this is needed if only a single data-column is present)
//qDebug() << "D exchange_point_comma=" << exchange_point_comma;
                if (new_set_no>0 && auto_set_cwd)
                {
                    QFileInfo finfo(QString::fromLocal8Bit(filename));
                    QString pa=finfo.absolutePath();
                    set_workingdir(pa.toLocal8Bit().constData());
                }

                resume_strings_after_load_or_save();

                /*if (new_set_block_size!=NULL)
                {
                qDebug() << "NrOfSetBlocks=" << new_set_blocks;
                    for (int hhh=0;hhh<new_set_blocks;hhh++)
                    {
                    qDebug() << hhh << "BlockSize=" << new_set_block_size[hhh];
                    }
                }
                else
                {
                qDebug() << "Warning: new_set_block_size==NULL";
                }*/

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
                    int blk_cnt=0,loc_cnt=0;
                    for (int k=0;k<new_set_no;k++)
                    {
                        gnos[len+k]=get_cg();
                        snos[len+k]=new_set_nos[k];
                        generateCommentAndLegendAfterImport(filenames[files-1],blk_cnt,0,1+loc_cnt++,newcomment,newlegend);
//qDebug() << "Filename:" << QString::fromLocal8Bit(filenames[files-1]) << endl << "NewComment=" << newcomment << endl << "NewLegend=" << newlegend;
                        if (loc_cnt>=new_set_block_size[blk_cnt])//switch to next block
                        {
                        blk_cnt++;
                        loc_cnt=0;
                        }
                        //str_imp=QString::fromLocal8Bit(newcomment);
                        if (strlen(getcomment(gnos[len+k],snos[len+k]))<=0 || temp_file_used==true)
                        {
                        setcomment(gnos[len+k],snos[len+k],newcomment);
                        }
                        //str_imp=QString::fromLocal8Bit(newlegend);
                        if (strlen(get_legend_string(gnos[len+k],snos[len+k]))<=0 && inhibit_set_legend==0)
                        {
                        set_legend_string(gnos[len+k],snos[len+k],newlegend);
                        }
                    }
                    len+=new_set_no;
                }
            }//end of simple ascii import
            delete[] filename;
            filename=NULL;
        }//end of loop trough all files
        Form_AgrInfo->get_import_counts(&n_of_imp_sets,&n_of_imp_agrs);
        n_of_imp_sets+=len;
            if (n_of_imp_sets<=0 && n_of_imp_agrs>0)
            clear_dirtystate();
            else if (n_of_imp_sets>0)
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
    yestoall=0;
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
                if (g[i].p[j].data.s!=NULL)
                {
                    if (g[i].p[j].data.orig_s==NULL)
                    {
                    g[i].p[j].data.orig_s = (char**)xrealloc(g[i].p[j].data.orig_s, g[i].p[j].data.len*sizeof(char *));
                        for (int k=0;k<g[i].p[j].data.len;k++)
                        {
                        g[i].p[j].data.orig_s[k] = copy_string(NULL, "");
                        }
                    }
                    for (int k=0;k<g[i].p[j].data.len;k++)
                    {
                    g[i].p[j].data.s[k]=copy_string(g[i].p[j].data.s[k],g[i].p[j].data.orig_s[k]);
                    }
                }
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
#if READ_UTF8_ONLY == 1
    (void) text;
    ///starting with v0.2.7 QtGrace will always convert to UTF8 and no other encoding, i.e. UTF8=encoding
#else
    static QString temp;
    temp=FileCodec->toUnicode(*text);//convert text from encoding to unicode
    *text = copy_string(*text,temp.toUtf8().constData());//convert QString in UTF-8 to C-String in UTF-8
#endif
}

void convert_single_string_from_encoding_to_UTF8_static(char * text)
{
#if READ_UTF8_ONLY == 1
    (void) text;
    ///starting with v0.2.7 QtGrace will always convert to UTF8 and no other encoding, i.e. UTF8=encoding
#else
    static QString temp;
    temp=FileCodec->toUnicode(text);//convert text from encoding to unicode
    strcpy(text,temp.toUtf8().constData());//convert QString in UTF-8 to C-String in UTF-8
#endif
}

void convert_single_string_from_UTF8_to_encoding(char ** text)
{
    (void) text;
    ///starting with v0.2.7 QtGrace will always convert to UTF8 and no other encoding
    /*
    static QString temp;
    temp=QString::fromUtf8(*text);//convert C-string in UTF-8 encoding in a QString in UTF-8
    *text = copy_string(*text,FileCodec->fromUnicode(temp).constData());//convert to encoding
    */
}

void convert_single_string_from_UTF8_to_encoding_static(char * text)
{
    (void) text;
    ///starting with v0.2.7 QtGrace will always convert to UTF8 and no other encoding
    /*
    static QString temp;
    temp=QString::fromUtf8(text);//convert C-string in UTF-8 encoding in a QString in UTF-8
    strcpy(text,FileCodec->fromUnicode(temp).constData());//convert to encoding
    */
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
                if (g[i].p[j].data.s!=NULL)
                {
                    for (int k=0;k<g[i].p[j].data.len;k++)
                    {
                    convert_single_string_from_encoding_to_UTF8_static(g[i].p[j].data.s[k]);
                    }
                }
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
                if (g[i].p[j].data.s!=NULL)
                {
                    for (int k=0;k<g[i].p[j].data.len;k++)
                    {
                    convert_single_string_from_UTF8_to_encoding_static(g[i].p[j].data.s[k]);
                    }
                }
            }
        }
    }
    for (int i=0;i<maxstr;i++)
    {
        convert_single_string_from_UTF8_to_encoding(&pstr[i].s_plotstring);
    }
}

//text --> string (warning: this function assumes string to be large! --> not to use with dynamic memory)
void generate_string_Qt_aware(char * str,QString text)//converts the text (assumed to be UTF-8) into a C-string in UTF-8 if QtFonts are to be used and a C-string in isoLatin1 if no QtFonts are to be used
{
    if (useQtFonts==true)
        strcpy(str,text.toUtf8().constData());
    else
        strcpy(str,text.toLatin1().constData());//Latin1 is the standard-codec for Grace-strings (T1-encoding)
}

void update_grace_string_from_UTF8(char ** string)//assumes string to be a dynamic c-string in UTF-8-format; we do LaTeX-conversions and convert to UTF-8(if QtFonts==true) or isolatin1
{
    static QString temp;
    static char temp2[MAX_STRING_LENGTH];
    temp=QString::fromUtf8(*string);//generate a QString from original c-string in UTF-8
    if (activateLaTeXsupport==TRUE)
        complete_LaTeX_to_Grace_Translator(temp);//do LaTeX-conversion (if needed)
    generate_string_Qt_aware(temp2,temp);//generate a suitable C-string from the QString
    *string = copy_string(*string, temp2);//copy converted sting to Grace-string-location
}

void update_grace_string_from_UTF8_static(char * string)//same as above, but with static memory
{
    static QString temp;
    static char temp2[MAX_STRING_LENGTH];
    temp=QString::fromUtf8(string);//generate a QString from original c-string in UTF-8
    if (activateLaTeXsupport==TRUE)
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
                if (g[i].p[j].data.s!=NULL)
                {
                    for (int k=0;k<g[i].p[j].data.len;k++)
                    {
                    update_grace_string_from_UTF8_static(g[i].p[j].data.s[k]);
                    }
                }
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
                if (g[i].p[j].data.s!=NULL)
                {
                    if (g[i].p[j].data.orig_s==NULL)
                    {
                    g[i].p[j].data.orig_s = (char**)xrealloc(g[i].p[j].data.orig_s, g[i].p[j].data.len*sizeof(char *));
                        for (int k=0;k<g[i].p[j].data.len;k++)
                        {
                        g[i].p[j].data.orig_s[k] = copy_string(NULL, "");
                        }
                    }
                    //qDebug() << "i=" << i << "j=" << "s=" << g[i].p[j].data.s << "orig_s=" << g[i].p[j].data.orig_s;
                    for (int k=0;k<g[i].p[j].data.len;k++)
                    {
                    g[i].p[j].data.orig_s[k] = copy_string(g[i].p[j].data.orig_s[k], g[i].p[j].data.s[k]);
                    }
                }
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

int read_id_from_text(char * text,int * gno,int * sno,int cur_graph,int cur_set)
{
int read_number;
read_number=sscanf(text,"G%dN.S%dN",gno,sno);
    if (read_number==2)
    {
    return RETURN_SUCCESS;
    }
read_number=sscanf(text,"G%d.S%dN",gno,sno);
    if (read_number==2)
    {
    return RETURN_SUCCESS;
    }
read_number=sscanf(text,"G%d.S%d",gno,sno);
    if (read_number==2)
    {
    return RETURN_SUCCESS;
    }
read_number=sscanf(text,"G%dN",gno);
    if (read_number==1)
    {
    return RETURN_SUCCESS;
    }
read_number=sscanf(text,"G%d",gno);
    if (read_number==1)
    {
    *sno=cur_set;
    return RETURN_SUCCESS;
    }
read_number=sscanf(text,"S%dN",gno);
    if (read_number==1)
    {
    return RETURN_SUCCESS;
    }
read_number=sscanf(text,"S%d",sno);
    if (read_number==1)
    {
    *gno=cur_graph;
    return RETURN_SUCCESS;
    }
return RETURN_FAILURE;
}

int read_datapoint_from_text(char * text,int * cols,Datapoint * p)
{
int suc=RETURN_SUCCESS;
QString tmp_txt(text);
char * ts=NULL;
tmp_txt.chop(1);
tmp_txt=tmp_txt.mid(1);
//cout << "tmp_txt=#" << tmp_txt.toLocal8Bit().constData() << "#" << endl;
QStringList list=tmp_txt.split(";");
set_parser_setno(current_origin_graph,current_origin_set);
    for (int i=0;i<(list.length()<MAX_SET_COLS?list.length():MAX_SET_COLS-1);i++)
    {
        if (ts!=NULL) delete[] ts;
        ts=new char[list.at(i).length()*2+8];
        strcpy(ts,list.at(i).toLatin1().constData());
//qDebug() << "ts=" << ts;
        prependSetID(ts,current_origin_set,current_origin_graph,current_origin_set,current_origin_graph);
//qDebug() << "ts after prepend=" << ts;
    if (std_evalexpr(ts,p->ex+i)==RETURN_FAILURE) suc=RETURN_FAILURE;
    }
*cols=list.length();
if (ts!=NULL) delete[] ts;
return suc;
}

void remove_end_comment(QString & command)//removes a comment at the end of a command
{
int index=command.indexOf(QString("#"),0,Qt::CaseInsensitive);
if (index<0) return;//no comments --> nothing to do
int index2=command.indexOf(QString("#if["),0,Qt::CaseInsensitive);
int index3=command.indexOf(QString("#QTGRACE_SPECIAL"),0,Qt::CaseInsensitive);
int index4=command.indexOf(QString("#QTGRACE_ADDITIONAL_PARAMETER"),0,Qt::CaseInsensitive);
if (index==index2 || index==index3 || index==index4) return;
command=command.left(index);
}

int qtspecial_scanner(char * command,int * errors)
{
    static int len,len2,pos;
    len=strlen(command);
//qDebug() << "special scanner=" << command;
    char * parameters;
    char formula_arg[MAX_STRING_LENGTH];
    int parameter_vals1[8],if_result,if_present;
    double parameter_vals2[8];
    int retval2,nr_of_replacements=0,eq_pos;//,extract_err;
    QString replayed_command,errormsg;
    Datapoint dp;
    formula_arg[0]='\0';
    if_present=startIfStatement(command,if_result,formula_arg);
//qDebug() << "if_present=" << if_present << "if_result=" << if_result << "formula_arg=" << formula_arg;
    if (if_present==1)
    {
        if (if_result==0)
        {
        //qDebug() << "if-command ignored";
        return RETURN_SUCCESS;//ignore the command
        }
        else//comparison==1 --> do the command
        {
        replayed_command=QString(formula_arg);
        nr_of_replacements=process_command_for_new_sets(replayed_command,current_target_graph);
        //qDebug() << "if-command-ok: formula=" << replayed_command;
        remove_end_comment(replayed_command);
        return scanner(replayed_command.toLatin1().data());
        }
    }
    int retval=containsSpecialCommand(command,&parameters);
//cout << "New command: vorher: command=#" << command << "#" << endl;
//qDebug() << "parser: G" << get_parser_gno() << ".S" << get_parser_setno();
    replayed_command=QString(command);
    nr_of_replacements=process_command_for_new_sets(replayed_command,current_target_graph);
    if (nr_of_replacements>0)//we made replacements-->send it back to the scanner
    {
    //strcpy(command,replayed_command.toLatin1().constData());
    remove_end_comment(replayed_command);
    scanner(replayed_command.toLatin1().data());
    return RETURN_SUCCESS;//we report that everything is alright --> no further parsing needed
    }
    //else: no replacements --> command is ok as it is

    if (len<16) return RETURN_FAILURE;//can not be a special command

//qDebug() << "New command: nachher: command=#" << command << "#";
//cout << "special scanner=" << command << " retval=" << retval << endl;
    if (retval==SPECIAL_NONE) return RETURN_FAILURE;

    char temp_val[MAX_STRING_LENGTH];

    int found_o_sets,found_n_sets;
    int * found_o_gnos=NULL;
    int * found_o_snos=NULL;
    int * found_n_gnos=NULL;
    int * found_n_snos=NULL;
    int type,realization,absolute,debug,point_extension,oversampling,rno,invr;
    double limits[2];
    int orders[4];
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
//qDebug() << "special_scanner: " << command;
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
//qDebug() << "replayed Command=" << replayed_command;
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
//qDebug() << "Extract-command: " << "parameters=#" << parameters << "# eq_pos=" << eq_pos;

            orders[0]=get_parser_gno();//Warning! This is only a default.
            orders[1]=get_parser_setno();

            if (activate_id_replacing==true)
            {
            orders[0]=replace_o_gnos[0];
            orders[1]=replace_o_snos[0];
            }

            if (eq_pos<0)
            {
            retval2=ParseExtractCommand(parameters,formula_arg);//no '=' in command
            }
            else
            {
            retval2=ParseExtractCommand(parameters+eq_pos+1,formula_arg);
            }
            std_evalexpr(formula_arg,limits);

            //cout << "read set-id: G" << orders[0] << ".S" << orders[1] << " limit=" << limits[0] << endl;

                if (retval2==4)//median -- special traetment because I extended the possible usage of the median (all columns, not only X or Y)
                {
                    orders[0]=-1;//Warning! Invalid ids!
                    orders[1]=-1;
                    orders[2]=-1;
                getSetIDFromText(formula_arg,orders[0],orders[1],orders[2]);
                getmedian(orders[0],orders[1],orders[2],limits);
                //cout << "read set-id: G" << orders[0] << ".S" << orders[1] << "." << orders[2] << endl;
                //cout << "median=" << limits[0] << endl;
                }
                else
                {
                //extract_err=extract_single_feature(retval2,orders[0],orders[1],limits);
                (void)extract_single_feature(retval2,orders[0],orders[1],RESTRICT_NONE,0,limits);
                //cout << "read_value=" << limits[0] << endl;
                }
        //cout << "parameters=" << parameters << " retval2=" << retval2 << " Formula_arg=" << formula_arg << " result=" << limits[0] << endl;
            if (eq_pos>=0)
            {
            sprintf(parameters+eq_pos+1,sformat,limits[0]);
            scanner(parameters);
            }
        break;
    case SPECIAL_APPEND:
        //cout << "to append: parameters=#" << parameters << "#" << endl;
        orders[0]=-1;
            for (unsigned int i=0;i<strlen(parameters);i++)
            {
                if (parameters[i]!='{') formula_arg[i]=parameters[i];
                else
                {
                formula_arg[i]='\0';
                orders[0]=(int)i;
                break;
                }
            }
//qDebug() << "to Append: id=#" << formula_arg << "#";
//qDebug() << "to Append: point=#" << parameters+orders[0] << "#";
        errormsg.clear();
        zero_datapoint(&dp);
            if (read_id_from_text(formula_arg,orders+1,orders+2,replace_n_gnos[0],replace_n_snos[0])==RETURN_FAILURE)
            {
            errormsg+=QObject::tr("Unable to read set-id from ")+QString(formula_arg)+QString("\n");
            }
            else
            {
                if (read_datapoint_from_text(parameters+orders[0],orders+3,&dp)==RETURN_FAILURE)
                {
                errormsg+=QObject::tr("Unable to read point data from ")+QString(parameters+orders[0])+QString("\n");
                }
            }
            if (!errormsg.isEmpty())
            {
            errmsg(errormsg.toLocal8Bit().constData());
            }
            else
            {
            //cout << "read-id=G" << orders[1] << ".S" << orders[2] << endl;
            add_point_at(orders[1],orders[2],getsetlength(orders[1],orders[2]),&dp);
            }
        break;
    case SPECIAL_FORMULA:
        retval2=ParseSpecialFormula(parameters,formula_arg);

        break;
    case SPECIAL_FIT2D:

/*operation 0
QString::number(sel_gno)1
QString::number(sel_sno)2
QString::number(n_sno)3

QString::number(x0)0
QString::number(y0)1
QString::number(a)2
QString::number(b)3
QString::number(alpha)4
QString::number(restr_type)5
QString::number(restr_negate)6

QString::number(x0)0
QString::number(y0)1
QString::number(r)2
QString::number(restr_type)3
QString::number(restr_negate)4*/

        //parameter_vals1[4]=sscanf(parameters,"%d;%d;%d;%d;%lf;%lf;%lf;%lf;%lf;%lf;%lf",parameter_vals1+0,parameter_vals1+1,parameter_vals1+2,parameter_vals1+3,parameter_vals2+0,parameter_vals2+1,parameter_vals2+2,parameter_vals2+3,parameter_vals2+4,parameter_vals2+5,parameter_vals2+6);
        //qDebug() << "Anzahl gelesene Parameter=" << parameter_vals1[4];
found_o_sets=1;
found_n_sets=1;
found_o_gnos=new int[2];
found_o_snos=new int[2];
found_n_gnos=new int[2];
found_n_snos=new int[2];

ParseFit2D(parameters,parameter_vals1[0],parameter_vals1[1],parameter_vals1[2],parameter_vals1[3],parameter_vals2[0],parameter_vals2[1],parameter_vals2[3],parameter_vals2[4],parameter_vals2[5],parameter_vals2[6],parameter_vals2[7],parameter_vals1[4],parameter_vals1[5],parameter_vals1[6]);
/*
Type=parameter_vals1[0]
gno=parameter_vals1[1]
sno=parameter_vals1[2]
target_set=parameter_vals1[3]
x0=parameter_vals2[0]
y0=parameter_vals2[1]
r1/a=parameter_vals2[3]
r2/b=parameter_vals2[4]
angle=parameter_vals2[5]
phi0=parameter_vals2[6]
phi1=parameter_vals2[7]
n_phi=parameter_vals1[4]
region=parameter_vals1[5]
reg_inv=parameter_vals1[6]
*/
found_o_gnos[0]=found_n_gnos[0]=parameter_vals1[1];
found_o_snos[0]=found_n_snos[0]=parameter_vals1[2];
//parameter_vals1[3];//this is the target set

//qDebug() << "2DFit-Script: Type=" << parameter_vals1[0] << "G" << found_o_gnos[0] << ".S" << found_o_snos[0] << "Target-Set=" << parameter_vals1[3];

replace_found_ids_with_new_ones(found_o_sets,&found_o_gnos,&found_o_snos,found_n_sets,&found_n_gnos,&found_n_snos);
        if (parameter_vals1[0]==0)//circle
        {
        found_o_gnos[1]=parameter_vals2[3];
        found_o_snos[1]=parameter_vals2[4];
//qDebug() << "Circle vor Fit: G" << found_o_gnos[0] << ".S" << found_o_snos[0] << "Target-Set=" << found_n_snos[0];
        fit_circle(found_o_gnos[0],found_o_snos[0],found_n_snos+0,parameter_vals2+0,parameter_vals2+1,parameter_vals2+3,parameter_vals1[5],parameter_vals1[6]);
            if (parameter_vals1[3]>=0)//a specific target set id is given
            {
            copyset(found_o_gnos[0],found_n_snos[0],found_o_gnos[0],(int)parameter_vals1[3]);
            killset(found_o_gnos[0],found_n_snos[0]);
            makeSetACircle(found_o_gnos[0],parameter_vals1[3],parameter_vals2[0],parameter_vals2[1],parameter_vals2[3]);
            }
            else if (parameter_vals1[3]==-1)//just keep the new set
            {
            makeSetACircle(found_o_gnos[0],found_n_snos[0],parameter_vals2[0],parameter_vals2[1],parameter_vals2[3]);
            }
            else if (parameter_vals1[3]==-2)//no new set
            {
            killset(found_o_gnos[0],found_n_snos[0]);
            }
        }
        else if (parameter_vals1[0]==1)//ellipse
        {
        found_o_gnos[1]=parameter_vals2[5];
        found_o_snos[1]=parameter_vals2[6];
        fit_ellipse(found_o_gnos[0],found_o_snos[0],found_n_snos+0,parameter_vals2+0,parameter_vals2+1,parameter_vals2+3,parameter_vals2+4,parameter_vals2+5,parameter_vals1[5],parameter_vals1[6]);
            if (parameter_vals1[3]>=0)//a specific target set id is given
            {
            copyset(found_o_gnos[0],found_n_snos[0],found_o_gnos[0],(int)parameter_vals1[3]);
            killset(found_o_gnos[0],found_n_snos[0]);
            makeSetAnEllipse(found_o_gnos[0],parameter_vals1[3],parameter_vals2[0],parameter_vals2[1],parameter_vals2[3],parameter_vals2[4],parameter_vals2[5]);
            }
            else if (parameter_vals1[3]==-1)//just keep the new set
            {
            makeSetAnEllipse(found_o_gnos[0],found_n_snos[0],parameter_vals2[0],parameter_vals2[1],parameter_vals2[3],parameter_vals2[4],parameter_vals2[5]);
            }
            else if (parameter_vals1[3]==-2)//no new set
            {
            killset(found_o_gnos[0],found_n_snos[0]);
            }
        }
        else
        {
        errmsg(QObject::tr("Error while parsing 2D-Fit-function: Parameter count incorrect!").toLocal8Bit().constData());
        }
        break;
    }

N_DELETE(found_o_gnos);
N_DELETE(found_o_snos);
N_DELETE(found_n_gnos);
N_DELETE(found_n_snos);

//qDebug() << "replayed Command2=" << replayed_command;
//qDebug() << "parser: G" << get_parser_gno() << ".S" << get_parser_setno();
    if (replayed_command.isEmpty()==false)//there is a command left -- we have to do this for all sets if replacement is wanted/activated
    {
        if (activate_id_replacing==true)
        {
            for (int ii=0;ii<replace_o_set_ids;ii++)
            {
                replace_set_ids_in_command(replayed_command,replace_o_gnos[ii],replace_o_snos[ii],replace_n_gnos[ii],replace_n_snos[ii],0);
                remove_end_comment(replayed_command);
                strcpy(temp_val,replayed_command.toLatin1().constData());
                //qDebug() << "replace=true: new formula=#" << temp_val << "#";
                retval2=scanner(temp_val);
                if (retval2!=RETURN_SUCCESS) (*errors)++;
            }
        }
        else//execute the commands just as they appear
        {
            remove_end_comment(replayed_command);
            strcpy(temp_val,replayed_command.toLatin1().constData());
            //qDebug() << "replace=false: new formula=#" << temp_val << "#";
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
    (void)relative;//should maybe implemented later!
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
    ///strcpy(new_encoding_name,FileCodec->name().constData());//starting with v0.2.7 only UTF-8 is used
    copy_LaTeX_to_Grace();
    ///convert_all_strings_from_UTF8_to_encoding();//UTF-8 is already the final encoding (no further encoding needed)
    /// Save old FileCodec just in case it is overwritten
#if READ_UTF8_ONLY == 0
    FileCodecSave=FileCodec;
#endif
}

void resume_strings_after_load_or_save(void)
{
    QString error_text;
#if READ_UTF8_ONLY == 0
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
#endif
    convert_all_strings_from_encoding_to_UTF8();
    /// Restore the codec originally set
#if READ_UTF8_ONLY == 0
    FileCodec=FileCodecSave;
#endif
    copy_Grace_to_LaTeX();
}

void save_default_states(void)
{
    if (default_line!=NULL) delete default_line;
    default_line=new linetype;
    if (default_box!=NULL) delete default_box;
    default_box=new boxtype;
    if (default_ellipse!=NULL) delete default_ellipse;
    default_ellipse=new ellipsetype;
    if (default_string!=NULL) delete default_string;
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
    default_string->align=string_align;
    default_string->master_align=string_master_align;
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
    string_align=default_string->align;
    string_master_align=default_string->master_align;
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

void set_docname_external(const char *s)//set docfilename and update the dialogs --> WARNING: this updates the exportfilename as well!
{
set_docname(s);
set_left_footer(NULL);
FormDeviceSetup->printfile_item->lenText->setText(QString(s));
}

void set_exportname_external(const char *s)//set exportfilename and update the dialogs
{
set_exportname(s);
set_left_footer(NULL);
FormDeviceSetup->printfile_item->lenText->setText(QString(s));
}

class Gould_token
{
public:
Gould_token(QByteArray contents,int version=-1);

char ident[3];
int nr_parameters;
int * p_length;
char ** parameters;
int v;
};

Gould_token::Gould_token(QByteArray contents,int version)
{
ident[0]=ident[1]=ident[2]='\0';
nr_parameters=0;
parameters=NULL;
v=version;
if (contents.length()<3) return;
    ident[0]=contents.at(1);
    ident[1]=contents.at(2);
int pos1=0;
int pos2=0;
int n_target_params=0;
bool cs=false;
    if (ident[0]=='C')
    {
        if (ident[1]=='F' || ident[1]=='T' || ident[1]=='E') n_target_params=1;
        else if (ident[1]=='D') n_target_params=7;
        else if (ident[1]=='R') n_target_params=13;
        else if (ident[1]=='P') n_target_params=4;
        else if (ident[1]=='J' || ident[1]=='A') n_target_params=2;
        else if (ident[1]=='S')
        {
        n_target_params=3;
        cs=true;
        }
    }
    else if (ident[0]=='K' && ident[0]=='S')
    {
    n_target_params=0;
    }
    else if (ident[0]=='N')
    {
        if (ident[1]=='L' || ident[1]=='O') n_target_params=2;
        else if (ident[1]=='T') n_target_params=5;
    }
    else if (ident[0]=='U')
    {
    n_target_params=3;
    }
    if (n_target_params<1) return;
nr_parameters=n_target_params;
parameters=new char*[nr_parameters];
p_length=new int[nr_parameters];
pos1=contents.indexOf(",");
pos2=contents.indexOf(",",pos1+1);
if (pos2==-1) pos2=contents.length()-1;
for (int i=0;i<nr_parameters;i++)
{
    parameters[i]=new char[pos2-pos1];
    parameters[i][pos2-pos1-1]='\0';
    p_length[i]=pos2-pos1-1;
    for (int j=pos1+1;j<pos2;j++)
    {
    parameters[i][j-(pos1+1)]=contents.at(j);
    }
pos1=pos2;
pos2=contents.indexOf(",",pos1+1);
if (pos2==-1) pos2=contents.length()-1;
cout << "subtoken length=" << p_length[i] << endl;
cout << "subtoken " << i << " #" << parameters[i] << "#" << endl;
}

if (cs==true)
{
ofstream ofi;
ofi.open("gould_trace_text.txt");
int min=0,max=0;
min=max=parameters[2][0];
for (int i=0;i<p_length[2];i++)
{
ofi << i << " " << (int)(parameters[2][i]) << endl;
if (min>(int)(parameters[2][i])) min=(int)(parameters[2][i]);
if (max<(int)(parameters[2][i])) max=(int)(parameters[2][i]);
}
/*cout << "min=" << min << endl;
cout << "max=" << max << endl;*/
ofi.close();
}

}

void find_data_in_Gould_tokens(int nr,Gould_token ** gt,double ** x,double ** y,int & nr_of_points)
{
int data_index=0;
//int format_version=-1;
//cout << "nr_of_tokens=" << nr << endl;
//first: look for the data
    for (int i=0;i<nr;i++)
    {
    //cout << gt[i]->ident << endl;
        if (gt[i]->ident[0]=='C' && gt[i]->ident[1]=='S')
        {
        data_index=i;
        break;
        }
    }
//cout << "Data is in " << data_index << endl;
nr_of_points=gt[data_index]->p_length[2];
if (*x!=NULL) delete[] *x;
if (*y!=NULL) delete[] *y;
*x=new double[nr_of_points];
*y=new double[nr_of_points];
//second: extract raw-data
int min=0,max=0;
min=max=gt[data_index]->parameters[2][0];
for (int i=0;i<gt[data_index]->p_length[2];i++)
{
(*x)[i]=(double)i;
(*y)[i]=(double)(gt[data_index]->parameters[2][i]);
if (min>(int)(gt[data_index]->parameters[2][i])) min=(int)(gt[data_index]->parameters[2][i]);
if (max<(int)(gt[data_index]->parameters[2][i])) max=(int)(gt[data_index]->parameters[2][i]);
}
/*cout << "min=" << min << endl;
cout << "max=" << max << endl;*/
//third: find and include t-axis
    for (int i=0;i<nr;i++)
    {
    //cout << gt[i]->ident << endl;
        if (gt[i]->ident[0]=='C' && gt[i]->ident[1]=='D')
        {
        data_index=i;
        break;
        }
    }
//cout << "t-data is in " << data_index << endl;
double x0,dx;
x0=-atof(gt[data_index]->parameters[3]);
dx=atof(gt[data_index]->parameters[1]);
    for (int i=0;i<nr_of_points;i++)
    {
    (*x)[i]=x0+dx*((*x)[i]);
    }
//fourth: find and include y-axis
    for (int i=0;i<nr;i++)
    {
    //cout << gt[i]->ident << endl;
        if (gt[i]->ident[0]=='C' && gt[i]->ident[1]=='R')
        {
        data_index=i;
        break;
        }
    }
//cout << "y-Data is in " << data_index << endl;
double y0,dy;
y0=atof(gt[data_index]->parameters[9]);
dy=atof(gt[data_index]->parameters[8]);
    for (int i=0;i<nr_of_points;i++)
    {
    (*y)[i]=y0+dy*((*y)[i]-min);
    }

}

void try_loading_Gould(char * filename,int & nr_of_points,double ** x,double ** y)
{
QString tmp_str;
QFileInfo info(filename);
QFile fi(filename);
fi.open(QIODevice::ReadOnly);
//QByteArray ba=fi.read(info.size());
QByteArray ba=fi.readAll();
QByteArray Probe,Probe2,Probe3;
QList<QByteArray> subtokenlist;
fi.close();
//cout << "filename=" << filename << " size=" << info.size() << endl;
//cout << "ba.length()=" << ba.length() << endl;
int max_nr_of_tokens=ba.count("|");
//cout << "Tokens=" << max_nr_of_tokens << endl;
if (max_nr_of_tokens<1) return;
/*Gould_token ** g_tokens=new Gould_token*[1+max_nr_of_tokens];
int g_token_count=0;
int pos3=0;
int komma_pos=0,komma_count=0,coun=0;*/
int pos1=0,pos2=0,index;
int format_version=-1;
bool is_cf,is_cs,is_cr,is_cd;
char dummy22[64];
double samplerate=0.0,trigger_delay=0.0,v1=0.0,offset=0.0;
int samplecount=0;

while (pos1<ba.length() && pos1>=0)
{
pos1=ba.indexOf("|",pos1);
if (pos1==-1) break;
pos2=ba.indexOf(";",pos1+1);
if (pos2==-1) pos2=ba.length();
else pos2++;

is_cf=is_cs=is_cr=is_cd=false;

Probe.clear();
for (int j=pos1;j<pos2;j++)
{
Probe.append(ba.at(j));
}
if (Probe.length()>=5)
{
    if (Probe.at(1)=='C')
    {
        if (Probe.at(2)=='F') is_cf=true;
        else if (Probe.at(2)=='S') is_cs=true;
        else if (Probe.at(2)=='R') is_cr=true;
        else if (Probe.at(2)=='D') is_cd=true;
    }
}
if (is_cf==true)
{
dummy22[0]=Probe.at(4);
dummy22[1]='\0';
format_version=atoi(dummy22);
//cout << "Version found: " << format_version << endl;
}
else if (format_version==2 || is_cs==true)//reread using the size_convention
{
Probe.clear();
Probe2.clear();
Probe3.clear();
index=pos1+6;
    while (ba.at(index)!=',')
    {
    Probe2.append(ba.at(index));
    index++;
    }
Probe2.append('\0');
samplecount=Probe2.toInt();
//cout << "number of remaining chars: " << Probe2.toInt() << endl;
for (int i=pos1;i<=index;i++) Probe.append(ba.at(i));
for (int i=0;i<=samplecount;i++) Probe.append(ba.at(index+1+i));
    if (format_version==1)
    {
    index++;
    for (int i=0;i<samplecount;i++) Probe3.append(ba.at(index+i));
    }
    else if (format_version==2)
    {
    index+=3;
    samplecount-=2;
    for (int i=0;i<samplecount;i++) Probe3.append(ba.at(index+i));
    }
}

if (is_cd)//time-settings
{
    //cout << "CD" << endl;
    subtokenlist=Probe.split(',');
    if (format_version==1)
    {
    //|CD,1,1.000000E-05,1,0.000000E+000,1,1,s;
    samplerate=subtokenlist.at(2).toDouble();
    trigger_delay=subtokenlist.at(4).toDouble();
    }
    else if (format_version==2)
    {
    //|CD,1,32,4.000000330961484E-7,1,1,s,0,0,0;
    samplerate=subtokenlist.at(3).toDouble();
    trigger_delay=subtokenlist.at(7).toDouble();
    }
    //cout << "samplerate=" << samplerate << " trigger=" << trigger_delay << endl;
}
else if (is_cr)//y-settings
{
    //cout << "CR" << endl;
    subtokenlist=Probe.split(',');
    if (format_version==1)
    {
    //|CR,1,1,0,1,0,255,0,255,8.533333e-02,-5.060000e-02,1,1,V;
    v1=subtokenlist.at(9).toDouble();
    offset=subtokenlist.at(10).toDouble();
    }
    else if (format_version==2)
    {
    //|CR,1,50,1,1.286069429388233E-2,-3.061960697174072E+0,1,1,V;
    v1=subtokenlist.at(4).toDouble();
    offset=subtokenlist.at(5).toDouble();
    }
    //cout << "v1=" << v1 << " offset=" << offset << endl;
}
else if (is_cs)//the actual data
{
    //cout << "CS" << endl;
    //cout << "Probe3.length=" << Probe3.length() << endl;
    nr_of_points=samplecount=Probe3.length();
    delete[] *x;
    delete[] *y;
    (*x)=new double[samplecount];
    (*y)=new double[samplecount];
    for (int i=0;i<samplecount;i++)
    {
    //(*x)[i]=samplerate * i - trigger_delay;
    //(*y)[i]=2.0 * v1 * (double)((unsigned char)Probe3.at(i))/256.0 - v1 - offset;
    (*x)[i]=samplerate * i;
    (*y)[i]=(double)((unsigned char)Probe3.at(i));
    }
//sprintf(dummy2,"Gould-trace-file: %s\\ndelay=%g,y-formula=2.0/256.0*Y*(%g)-(%g)-(%g)",filename,trigger_delay,v1,v1,offset);
tmp_str=QObject::tr("Gould-trace-file: ")+QString::fromLocal8Bit(filename)+QObject::tr("\\ndelay=")+QString::number(trigger_delay)+QObject::tr(",y-formula=2.0/256.0*Y*(")+QString::number(v1)+QObject::tr(")-(")+QString::number(v1)+QObject::tr(")-(")+QString::number(offset)+QObject::tr(")");
strcpy(dummy2,tmp_str.toLocal8Bit().constData());
//cout << "in file:" << dummy2 << endl;
}

/*cout << "Version=" << format_version << " Gould_Token:" << endl;
for (int j=0;j<Probe.length();j++)
cout << Probe.at(j);
cout << endl;*/

/*g_tokens[g_token_count]=new Gould_token(Probe);
g_token_count++;
cout << "Token so far = " << g_token_count << endl;*/

/*
    Probe.clear();
    Probe2.clear();
    komma_pos=komma_count=0;
    coun=0;
    for (int j=pos1+1;j<pos2;j++)
    {
    Probe2+=ba.at(j);
    coun++;
        if (ba.at(j)==',')
        {
        komma_count++;
        if (komma_count==4) komma_pos=coun;
        }
    }
    //Probe << ba.at(j);
//Probe2=QString(Probe);
Probe3=Probe2.split(",");
    for (int j=0;j<Probe3.length();j++)
    cout << "#" << Probe3.at(j).toLocal8Bit().constData();
    if (Probe3.at(0).compare("CS")==0)
    {
    cout << "found: komma_pos=" << komma_pos << " count=" << coun-komma_pos << " pos3-pos1=" << pos3-pos1 << endl;
    }
cout << endl;*/

if (is_cs==true) break;

if (pos2>=0 && pos1>=0)
pos1=pos2;
else pos1=pos2=-1;
}

/*
find_data_in_Gould_tokens(g_token_count,g_tokens,x,y,nr_of_points);
ofstream ofi;
ofi.open("gould_trace_text_conv.txt");
for (int i=0;i<nr_of_points;i++)
{
ofi << (*x)[i] << " " << (*y)[i] << endl;
}
ofi.close();
*/

/*delete[] x;
delete[] y;*/
}

void replacer(char * c,const char * a,const char * b)
{
QString tmp1(c);
QString tmp2(a);
QString tmp3(b);
tmp1.replace(tmp2,tmp3);
strcpy(c,tmp1.toLocal8Bit().constData());
}

int write_set_to_bytearray(int gno, int setno, QByteArray * cp, char * format, int rawdata)
{
    static char tmp_str[1024];
    int i, n, col, ncols;
    double *x[MAX_SET_COLS];
    char **s;

    if (cp == NULL) {
        return RETURN_FAILURE;
    }

    if (is_set_active(gno, setno) == TRUE) {
        n = getsetlength(gno, setno);
        ncols = dataset_cols(gno, setno);
        for (col = 0; col < ncols; col++) {
            x[col] = getcol(gno, setno, col);
        }
        s = get_set_strings(gno, setno);

        if (format == NULL) {
            format = sformat;
        }

        if (!rawdata) {
            sprintf(tmp_str, "@target G%d.S%d\n", gno, setno);
            sprintf(tmp_str, "@type %s\n", set_types(dataset_type(gno, setno)));
        }

        for (i = 0; i < n; i++) {
            for (col = 0; col < ncols; col++) {
                if (col != 0) {
                    strcat(tmp_str," ");
                    //fputs(" ", cp);
                }
                sprintf(tmp_str, format, x[col][i]);
            }
            if (s != NULL) {
                sprintf(tmp_str, " \"%s\"", PSTRING(s[i]));
            }
            strcat(tmp_str,"\n");
            //fputs("\n", cp);
        }
        if (rawdata) {
            sprintf(tmp_str, "\n");
        } else {
            sprintf(tmp_str, "&\n");
        }
    }

    return RETURN_SUCCESS;
}

void initialize_default_csv_format(void)
{
ifstream ifi;
//first: clear all settings
init_mcsv_import_infos(&def_csv_import_format);
QFileInfo fi(QString::fromLocal8Bit(default_csv_format));
if (fi.exists()==true)
{
//qDebug() << "Default-CSV-Format-File:" << QString::fromLocal8Bit(default_csv_format) << "EXISTS!";
ifi.open(default_csv_format);
loadCSV_multiSetFormat_FromFile(ifi,&def_csv_import_format);
ifi.close();
}
/*else
{
qDebug() << "Default-CSV-Format-File:" << QString::fromLocal8Bit(default_csv_format) << "DOES NOT EXIST!";
}*/

}

#ifdef __cplusplus
extern "C" {
#endif
void parser_preprocessor(char * c)
{
replacer(c,"**","^");
}
#ifdef __cplusplus
}
#endif
