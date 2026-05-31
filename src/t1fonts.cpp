/*
 * Grace - GRaphing, Advanced Computation and Exploration of data
 *
 * Home page: http://plasma-gate.weizmann.ac.il/Grace/
 *
 * Copyright (c) 1991-1995 Paul J Turner, Portland, OR
 * Copyright (c) 1996-2003 Grace Development Team
 *
 * Maintained by Evgeny Stambulchik
 *
 * Modified by Andreas Winter 2008-2022
 *
 *                           All Rights Reserved
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <iostream>
///#include <config.h>
#include <cmath>

#include <cstdlib>
#include <cstring>
#include <cctype>

#include "defines.h"
#include "draw.h"

#include "utils.h"
#include "files.h"
#include "device.h"
#include "t1fonts.h"
#include "cmath.h"
#include "globals.h"
#include "noxprotos.h"
#include <QPainter>
#include <QFontMetricsF>

using namespace std;

extern QPainter * GeneralPainter;

extern int AAGrayLevelsOK;
extern void xlibVPoint2dev(VPoint vp, int *x, int *y);
extern int curdevice;
extern bool useQtFonts;
extern VPoint global_bb_ll;
extern VPoint global_bb_ur;
extern QList<QFont> stdFontList;
extern QFont getFontFromDatabase(int i);
extern GLYPH *GetQtGlyph(CompositeString *cs, double dpv, int fontaa);
extern void errwin(const char *s);
//extern void WriteQtString(VPoint vp,int rot,int just,char * s);
extern QFont get_Std_QFont_From_GraceFontID(int fontID,bool & ok);
extern QFont get_Std_QFont_From_GraceFontName(const char * grace_font_name,int & font_nr,bool & ok);

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
//--> convert Grace-name into QtFontID
extern int get_QtFontID_from_Grace_Name(const char * name,int whatlist);//get the index of a QtFont in a list by its Grace-name
//--> convert Grace-font-id into QtFontID
extern int get_QtFontID_from_GraceID(int font_id,int whatlist);

extern void append_to_storage(int * l,int ** storage,int n,int * new_entries);
extern void append_to_storage_single(int * l,int ** storage,int new_entry);

int nfonts = 0;
static FontDB *FontDBtable = NULL;
static char **DefEncoding = NULL;
bool useQtFunctions=false;
unsigned int qtCharShift=0;
extern int currentStringAlignment;

/*
void (*devputpixmap) (VPoint vp, int width, int height, 
     char *databits, int pixmap_bpp, int bitmap_pad, int pixmap_type);
void (*devputtext) (VPoint vp, char *s, int len, int font,
     TextMatrix *tm, int underline, int overline, int kerning);
*/

void draw_simple_point(VPoint a,int col,double d)
{
if (get_draw_mode()!=TRUE) return;
VPoint debug_bounding_box[4];
    setcolor(col);
    debug_bounding_box[0].x=a.x+d;
    debug_bounding_box[0].y=a.y+d;
    debug_bounding_box[1].x=a.x-d;
    debug_bounding_box[1].y=a.y-d;
    (*devdrawpolyline)(debug_bounding_box,2,POLYLINE_OPEN);
    debug_bounding_box[0].x=a.x+d;
    debug_bounding_box[0].y=a.y-d;
    debug_bounding_box[1].x=a.x-d;
    debug_bounding_box[1].y=a.y+d;
    (*devdrawpolyline)(debug_bounding_box,2,POLYLINE_OPEN);
}

void draw_simple_rectangle(VPoint a, VPoint b,int col)
{
if (get_draw_mode()!=TRUE) return;
VPoint debug_bounding_box[4];
    setcolor(col);
    debug_bounding_box[0].x=a.x;
    debug_bounding_box[0].y=a.y;
    debug_bounding_box[1].x=b.x;
    debug_bounding_box[1].y=a.y;
    debug_bounding_box[2].x=b.x;
    debug_bounding_box[2].y=b.y;
    debug_bounding_box[3].x=a.x;
    debug_bounding_box[3].y=b.y;
(*devdrawpolyline)(debug_bounding_box,4,POLYLINE_CLOSED);
}

void draw_simple_line(VPoint a, VPoint b,int col)
{
if (get_draw_mode()!=TRUE) return;
VPoint debug_bounding_box[2];
    setcolor(col);
    debug_bounding_box[0].x=a.x;
    debug_bounding_box[0].y=a.y;
    debug_bounding_box[1].x=b.x;
    debug_bounding_box[1].y=b.y;
    qDebug() << a.x << "|" << a.y  << "-" << b.x << "|" << b.y;
(*devdrawpolyline)(debug_bounding_box,2,POLYLINE_OPEN);
}

// Standard Grace font aliases (PostScript names) — hardcoded for the 14 built-in fonts.
// Used for AGR file save/load and PostScript output; independent of rendering backend.
static const char *grace_font_aliases[] = {
    "Times-Roman", "Times-Italic", "Times-Bold", "Times-BoldItalic",
    "Helvetica", "Helvetica-Oblique", "Helvetica-Bold", "Helvetica-BoldOblique",
    "Courier", "Courier-Oblique", "Courier-Bold", "Courier-BoldOblique",
    "Symbol", "ZapfDingbats"
};

int init_t1(void)
{
    nfonts = 14;  // the 14 standard Grace fonts, now backed by Qt
    FontDBtable = (FontDB*)xmalloc(nfonts * sizeof(FontDB));
    for (int i = 0; i < nfonts; i++) {
        FontDBtable[i].mapped_id = i;
        FontDBtable[i].alias    = copy_string(NULL, grace_font_aliases[i]);
        FontDBtable[i].fallback = copy_string(NULL, grace_font_aliases[i]);
    }
    useQtFonts = true;
    return (RETURN_SUCCESS);
}

void map_fonts(int map)
{
    int i;
    if (map == FONT_MAP_ACEGR)
    {
        for (i = 0; i < nfonts; i++) FontDBtable[i].mapped_id = BAD_FONT_ID;
        map_font_by_name("Times-Roman", 0);
        map_font_by_name("Times-Bold", 1);
        map_font_by_name("Times-Italic", 2);
        map_font_by_name("Times-BoldItalic", 3);
        map_font_by_name("Helvetica", 4);
        map_font_by_name("Helvetica-Bold", 5);
        map_font_by_name("Helvetica-Oblique", 6);
        map_font_by_name("Helvetica-BoldOblique", 7);
        map_font_by_name("Symbol", 8);
        map_font_by_name("ZapfDingbats", 9);
    }
    else
    {
        for (i = 0; i < nfonts; i++) FontDBtable[i].mapped_id = i;
    }
}

int font_map_equals_font_ids(void)
{
    int equ=1;
    int i;
    for (i = 0; i < nfonts; i++)
    {
        if (FontDBtable[i].mapped_id != i) equ=0;
    }
    return equ;
}

int get_font_mapped_id(int font)
{
    if (font >= nfonts || font < 0) {
        return(BAD_FONT_ID);
    } else {
        return(FontDBtable[font].mapped_id);
    }
}

int get_mapped_font(int mapped_id)
{
    int i;
    if (useQtFonts==true) return mapped_id;//this function is needed to retrive the real number of a font with a specific mapped index --> in Qt we always generate the font-table to be sorted (font-id=mapped-id)
    for (i = 0; i < nfonts; i++)
    {
        if (FontDBtable[i].mapped_id == mapped_id)
        {
            return(i);
        }
    }
    return(BAD_FONT_ID);
}

int map_font(int font, int mapped_id)
{
    int i;
    if (font >= nfonts || font < 0)
    {
        return RETURN_FAILURE;
    }
    /* make sure the mapping is unique */
    for (i = 0; i < nfonts; i++)
    {
        if (FontDBtable[i].mapped_id == mapped_id)
        {
            FontDBtable[i].mapped_id = BAD_FONT_ID;
        }
    }
    FontDBtable[font].mapped_id = mapped_id;
    return RETURN_SUCCESS;
}

int map_font_by_name(const char *fname, int mapped_id)
{
    return(map_font(get_font_by_name(fname), mapped_id));
}

unsigned int number_of_fonts(void)
{
    if (useQtFonts)
        return stdFontList.length();
    else
        return (nfonts);
}

int get_font_by_name(const char * fname)
{
    int i,font_nr=-1;

    if (fname == NULL)
    {
        return(BAD_FONT_ID);
    }
    
    if (useQtFonts==true)
    {
        font_nr=get_QtFontID_from_Grace_Name(fname,1);//we search in the current qt-std-font-list for a font with the Grace-name stated
        if (font_nr==-1)//we have found nothing - it may be a Qt-font-name
        {
            font_nr=find_QtFont_in_List(QString(fname),1);
        }
        if (font_nr!=-1)//we found something
        {
            return font_nr;
        }
        else//we found nothing --> font not present!
        {
            char dummytext[128];
            sprintf(dummytext,"Font %s not found!",fname);
            errwin(dummytext);
            return BAD_FONT_ID;
        }
    }//end useQtFonts

    /*if (useQtFonts)
{//fname can be the name of a Grace-Std-Font
cout << "find=#" << fname << "# QT=" << useQtFonts << endl;
cout << "stdFontList.length()=" << stdFontList.length() << endl;

f=get_Std_QFont_From_GraceFontName(fname,font_nr,ok);
if (font_nr>0)
{
    cout << "found=" << font_nr << endl;
return font_nr;
}
cout << "NOT FOUND" << endl;
    for (i=0;i<stdFontList.length();i++)
    {
    cout << "name[" << i << "]=" << get_fontalias(i) << endl;
        if (strcmp(get_fontalias(i), fname) == 0)
        {
            return(i);
        }
    }
cout << "getfont/QtFonts/notFound! length=" << stdFontList.length() << endl;
}
else
{*/

    //only used if useQtFonts==false
    for (i = 0; i < nfonts; i++)
    {
        if (strcmp(get_fontalias(i), fname) == 0)
        {
            return(i);
        }
    }
    for (i = 0; i < nfonts; i++)
    {
        if (strcmp(get_fontfallback(i), fname) == 0)
        {
            return(i);
        }
    }
    //}
    return(BAD_FONT_ID);
}

char *get_fontfilename(int font, int abspath)
{
    (void)font; (void)abspath;
    return NULL;
}

char *get_afmfilename(int font, int abspath)
{
    (void)font; (void)abspath;
    return NULL;
}

char *get_fontname(int font)
{
    return getNameOfStdQtFont(font);
}

char *get_fontfullname(int font)
{
    return getNameOfStdQtFont(font);
}

char *get_fontfamilyname(int font)
{
    return getFamilyNameOfStdQtFont(font);
}

char *get_fontweight(int font)
{
    static char buf[32];
    QFont qf = getFontFromDatabase(font);
    int w = qf.weight();
    if (w >= QFont::Black)     strcpy(buf, "Black");
    else if (w >= QFont::ExtraBold) strcpy(buf, "ExtraBold");
    else if (w >= QFont::Bold)      strcpy(buf, "Bold");
    else if (w >= QFont::DemiBold)  strcpy(buf, "DemiBold");
    else if (w >= QFont::Medium)    strcpy(buf, "Medium");
    else                            strcpy(buf, "Roman");
    return buf;
}

char *get_fontalias(int font)
{
    static char aliasname[512];
    char *fpointer = getNameOfStdQtFont(font);
    if (fpointer == NULL)
        aliasname[0] = '\0';
    else
        strcpy(aliasname, fpointer);
    return aliasname;
}

char *get_fontfallback(int font)
{
    return get_fontalias(font);
}

char *get_encodingscheme(int font)
{
    // FontSpecific for Symbol and ZapfDingbats, WinAnsiEncoding for all others
    return (char*)((font == 12 || font == 13) ? "FontSpecific" : "WinAnsiEncoding");
}

double get_textline_width(int font)
{
    QFont qf = getFontFromDatabase(font);
    qf.setPixelSize(1000);
    return QFontMetricsF(qf).lineWidth() / 1000.0;
}

double get_underline_pos(int font)
{
    QFont qf = getFontFromDatabase(font);
    qf.setPixelSize(1000);
    return -QFontMetricsF(qf).underlinePos() / 1000.0;
}

double get_overline_pos(int font)
{
    QFont qf = getFontFromDatabase(font);
    qf.setPixelSize(1000);
    QFontMetricsF m(qf);
    return (m.ascent() + m.lineWidth()) / 1000.0;
}

double get_italic_angle(int font)
{
    (void)font;
    return 0.0;  // not needed with Qt fonts
}


static int tm_scale(TextMatrix *tm, double s)
{
    if (s != 0.0) {
        tm->cxx *= s; tm->cxy *= s; tm->cyx *= s; tm->cyy *= s;
        return RETURN_SUCCESS;
    } else {
        return RETURN_FAILURE;
    }
}

/* determinant */
static double tm_det(TextMatrix *tm)
{
    return tm->cxx*tm->cyy - tm->cxy*tm->cyx;
}

/* vertical size */
static double tm_size(TextMatrix *tm)
{
    return tm_det(tm)/sqrt(tm->cxx*tm->cxx + tm->cyx*tm->cyx);
}

static int tm_product(TextMatrix *tm, TextMatrix *p)
{
    TextMatrix tmp;
    
    if (tm_det(p) != 0.0) {
        tmp.cxx = p->cxx*tm->cxx + p->cxy*tm->cyx;
        tmp.cxy = p->cxx*tm->cxy + p->cxy*tm->cyy;
        tmp.cyx = p->cyx*tm->cxx + p->cyy*tm->cyx;
        tmp.cyy = p->cyx*tm->cxy + p->cyy*tm->cyy;
        *tm = tmp;
        return RETURN_SUCCESS;
    } else {
        return RETURN_FAILURE;
    }
}

static void tm_rotate(TextMatrix *tm, double angle)
{
    if (angle != 0.0) {
        double co, si;
        TextMatrix tmp;

        si = sin(M_PI/180.0*angle);
        co = cos(M_PI/180.0*angle);
        tmp.cxx = co; tmp.cyy = co; tmp.cxy = -si; tmp.cyx = si;
        tm_product(tm, &tmp);
    }
}

static void tm_slant(TextMatrix *tm, double slant)
{
    if (slant != 0.0) {
        TextMatrix tmp;

        tmp.cxx = 1.0; tmp.cyy = 1.0; tmp.cxy = slant; tmp.cyx = 0.0;
        tm_product(tm, &tmp);
    }
}

static void FreeCompositeString(CompositeString *cs, int nss)
{
    int i;
    for (i = 0; i < nss; i++) {
        xfree(cs[i].s);
        // cs[i].glyph is either a static from GetQtGlyph or NULL — never heap-allocated
    }
    xfree(cs);
}

static int get_escape_args(char *s, char *buf)
{
    int i = 0;
    
    if (*s == '{') {
        s++;
        while (*s != '\0') {
            if (*s == '}') {
                *buf = '\0';
                return i;
            } else {
                *buf = *s;
                buf++; s++; i++;
            }
        }
    }
    
    return -1;
}

static const TextMatrix unit_tm = UNIT_TM;

static CompositeString *String2Composite(char *string, int *nss)
{
    CompositeString *csbuf;

    char *ss, *buf, *acc_buf;
    int inside_escape = FALSE;
    int i, isub, j;
    int acc_len;
    int slen;
    char ccode;
    int upperset = FALSE;
    double scale;
    TextMatrix tm_buf;
    
    int font = BAD_FONT_ID, new_font = font;
    int color = BAD_COLOR, new_color = color;
    TextMatrix tm = unit_tm, tm_new = tm;
    double hshift = 0.0, new_hshift = hshift;
    double baseline = 0.0;//, baseline_old;
    double vshift = baseline, new_vshift = vshift;
    int underline = FALSE, overline = FALSE;
    int new_underline = underline, new_overline = overline;
    int kerning = FALSE, new_kerning = kerning;
    int direction = STRING_DIRECTION_LR, new_direction = direction;
    int advancing = TEXT_ADVANCING_LR, new_advancing = advancing;
    int ligatures = FALSE, new_ligatures = ligatures;
    int new_qtCharShift = CHAR_SHIFT_NONE, cur_qtCharShift = CHAR_SHIFT_NONE;

    int setmark = MARK_NONE;
    int gotomark = MARK_NONE, new_gotomark = gotomark;
    
    double val;

    csbuf = NULL;
    *nss = 0;
    
    if (string == NULL) {
        return NULL;
    }

    slen = strlen(string);
    
    if (slen == 0) {
        return NULL;
    }
    
    ss = (char*)xmalloc(slen + 1);
    buf = (char*)xmalloc(slen + 1);
    acc_buf = (char*)xmalloc(slen + 1);
    if (ss == NULL || buf == NULL || acc_buf == NULL) {
        xfree(acc_buf);
        xfree(buf);
        xfree(ss);
        return NULL;
    }

    isub = 0;
    ss[isub] = 0;
    
    for (i = 0; i <= slen; i++) {
        ccode = string[i];
        acc_len = 0;
        if (ccode < 32 && ccode > 0)
        {
            /* skip control codes */
            continue;
        }
        if (inside_escape) {
            inside_escape = FALSE;
            
            if (isdigit(ccode))
            {
                new_font = get_mapped_font(ccode - '0');
                if (useQtFonts==true && new_font == get_font_by_name("Symbol"))
                new_qtCharShift |= CHAR_SHIFT_TO_SYMBOL;
                else
                new_qtCharShift = CHAR_SHIFT_NONE;
                continue;
            } else if (ccode == 'd') {
                i++;
                switch (string[i]) {
                case 'l':
                    new_direction = STRING_DIRECTION_LR;
                    break;
                case 'r':
                    new_direction = STRING_DIRECTION_RL;
                    break;
                case 'L':
                    new_advancing = TEXT_ADVANCING_LR;
                    break;
                case 'R':
                    new_advancing = TEXT_ADVANCING_RL;
                    break;
                default:
                    /* undo advancing */
                    i--;
                    break;
                }
                continue;
            } else if (ccode == 'F') {
                i++;
                switch (string[i]) {
                case 'k':
                    new_kerning = TRUE;
                    break;
                case 'K':
                    new_kerning = FALSE;
                    break;
                case 'l':
                    new_ligatures = TRUE;
                    break;
                case 'L':
                    new_ligatures = FALSE;
                    break;
                default:
                    /* undo advancing */
                    i--;
                    break;
                }
                continue;
            } else if (isoneof(ccode, "cCsSNBxuUoO+-qQn")) {
                switch (ccode) {
                case 's':
                    new_vshift -= tm_size(&tm_new)*SUBSCRIPT_SHIFT;
                    tm_scale(&tm_new, SSCRIPT_SCALE);
                    break;
                case 'S':
                    new_vshift += tm_size(&tm_new)*SUPSCRIPT_SHIFT;
                    tm_scale(&tm_new, SSCRIPT_SCALE);
                    break;
                case 'N':
                    scale = 1.0/tm_size(&tm_new);
                    tm_scale(&tm_new, scale);
                    new_vshift = baseline;
                    break;
                case 'B':
                    new_font = BAD_FONT_ID;
                    new_qtCharShift &= ~CHAR_SHIFT_TO_SYMBOL;
                    break;
                case 'x':
                    if (useQtFonts==false)
                    {
                        new_font = get_font_by_name("Symbol");
                        new_qtCharShift = CHAR_SHIFT_NONE;
                    }
                    else
                    {
                        /// Hier sollte auch der Symbol-font stehen
                        new_font = get_font_by_name("Symbol");/// hinzugefuegt
                        new_qtCharShift |= CHAR_SHIFT_TO_SYMBOL;
                    }
                    break;
                case 'c':
                    if (useQtFonts==false)
                    {
                        upperset = TRUE;
                    }
                    else
                    {
                        new_qtCharShift |= CHAR_SHIFT_TO_UPPERSET;
                    }
                    break;
                case 'C':
                    if (useQtFonts==false)
                    {
                        upperset = FALSE;
                    }
                    else
                    {
                        new_qtCharShift &= ~CHAR_SHIFT_TO_UPPERSET;
                    }
                    break;
                case 'u':
                    new_underline = TRUE;
                    break;
                case 'U':
                    new_underline = FALSE;
                    break;
                case 'o':
                    new_overline = TRUE;
                    break;
                case 'O':
                    new_overline = FALSE;
                    break;
                case '-':
                    tm_scale(&tm_new, 1.0/ENLARGE_SCALE);
                    break;
                case '+':
                    tm_scale(&tm_new, ENLARGE_SCALE);
                    break;
                case 'q':
                    tm_slant(&tm_new, OBLIQUE_FACTOR);
                    break;
                case 'Q':
                    tm_slant(&tm_new, -OBLIQUE_FACTOR);
                    break;
                case 'n':
                    new_gotomark = MARK_CR;
                    baseline -= 1.0;
                    new_vshift = baseline;
                    new_hshift = 0.0;
                    break;
                }
                continue;
            } else if (isoneof(ccode, "fhvVzZmM#rltTR") &&
                       (j = get_escape_args(&(string[i + 1]), buf)) >= 0) {
                i += (j + 2);
                switch (ccode) {
                case 'f':
                    if (j == 0) {
                        new_font = BAD_FONT_ID;
                    } else if (isdigit(buf[0])) {
                        new_font = get_mapped_font(atoi(buf));
                    } else {
                        new_font = get_font_by_name(buf);
                    }
                        if (useQtFonts==false)
                        {
                        new_qtCharShift &= ~CHAR_SHIFT_TO_SYMBOL;
                        }
                        else if (new_font==get_font_by_name("Symbol"))
                        {
                        new_qtCharShift |= CHAR_SHIFT_TO_SYMBOL;
                        }
                    break;
                case 'v':
                    if (j == 0) {
                        new_vshift = baseline;
                    } else {
                        val = atof(buf);
                        new_vshift += tm_size(&tm_new)*val;
                    }
                    break;
                case 'V':
                    //baseline_old = baseline;
                    if (j == 0) {
                        baseline = 0.0;
                    } else {
                        val = atof(buf);
                        baseline += tm_size(&tm_new)*val;
                    }
                    new_vshift = baseline;
                    break;
                case 'h':
                    val = atof(buf);
                    new_hshift = tm_size(&tm_new)*val;
                    break;
                case 'z':
                    if (j == 0) {
                        scale = 1.0/tm_size(&tm_new);
                        tm_scale(&tm_new, scale);
                    } else {
                        scale = atof(buf);
                        tm_scale(&tm_new, scale);
                    }
                    break;
                case 'Z':
                    scale = atof(buf)/tm_size(&tm_new);
                    tm_scale(&tm_new, scale);
                    break;
                case 'r':
                    tm_rotate(&tm_new, atof(buf));
                    break;
                case 'l':
                    tm_slant(&tm_new, atof(buf));
                    break;
                case 't':
                    if (j == 0) {
                        tm_new = unit_tm;
                    } else {
                        if (sscanf(buf, "%lf %lf %lf %lf",
                                   &tm_buf.cxx, &tm_buf.cxy,
                                   &tm_buf.cyx, &tm_buf.cyy) == 4) {
                            tm_product(&tm_new, &tm_buf);
                        }
                    }
                    break;
                case 'T':
                    if (sscanf(buf, "%lf %lf %lf %lf",
                               &tm_buf.cxx, &tm_buf.cxy,
                               &tm_buf.cyx, &tm_buf.cyy) == 4) {
                        tm_new = tm_buf;
                    }
                    break;
                case 'm':
                    setmark = atoi(buf);
                    break;
                case 'M':
                    new_gotomark = atoi(buf);
                    new_vshift = baseline;
                    new_hshift = 0.0;
                    break;
                case 'R':
                    if (j == 0) {
                        new_color = BAD_COLOR;
                    } else if (isdigit(buf[0])) {
                        new_color = atof(buf);
                    } else {
                        new_color = get_color_by_name(buf);
                    }
                    break;
                case '#':
                    if (j % 2 == 0) {
                        int k;
                        char hex[3];
                        hex[2] = '\0';
                        for (k = 0; k < j; k += 2) {
                            hex[0] = buf[k];
                            hex[1] = buf[k + 1];
                            acc_buf[acc_len] = strtol(hex, NULL, 16);
                            acc_len++;
                        }
                    }
                    break;
                }

                if (ccode != '#') {
                    continue;
                }
            } else {
                /* store the char */
                acc_buf[0] = (ccode + (upperset*0x80)) & 0xff;
                acc_len = 1;
            }
        } else {
            if (ccode == '\\') {
                inside_escape = TRUE;
                continue;
            } else {
                /* store the char */
                acc_buf[0] = (ccode + (upperset*0x80)) & 0xff;
                acc_len = 1;
            }
        }

        if ((new_font      != font      ) ||
                (new_color     != color     ) ||
                (tm_new.cxx    != tm.cxx    ) ||
                (tm_new.cxy    != tm.cxy    ) ||
                (tm_new.cyx    != tm.cyx    ) ||
                (tm_new.cyy    != tm.cyy    ) ||
                (new_hshift    != 0.0       ) ||
                (new_vshift    != vshift    ) ||
                (new_underline != underline ) ||
                (new_overline  != overline  ) ||
                (new_kerning   != kerning   ) ||
                (new_qtCharShift != cur_qtCharShift   ) ||
                (new_direction != direction ) ||
                (new_advancing != advancing ) ||
                (new_ligatures != ligatures ) ||
                (setmark       >= 0         ) ||
                (new_gotomark  >= 0         ) ||
                (ccode         == 0         )) {

            if (isub != 0 || setmark >= 0) {	/* non-empty substring */

                csbuf = (CompositeString*)xrealloc(csbuf, (*nss + 1)*sizeof(CompositeString));
                csbuf[*nss].font = font;
                csbuf[*nss].color = color;
                csbuf[*nss].alpha = getalpha();
                csbuf[*nss].tm = tm;
                csbuf[*nss].hshift = hshift;
                csbuf[*nss].vshift = vshift;
                csbuf[*nss].underline = underline;
                csbuf[*nss].overline = overline;
                csbuf[*nss].kerning = kerning;
                csbuf[*nss].direction = direction;
                csbuf[*nss].advancing = advancing;
                csbuf[*nss].ligatures = ligatures;
                csbuf[*nss].qtCharShift = cur_qtCharShift;
                csbuf[*nss].setmark = setmark;
                setmark = MARK_NONE;
                csbuf[*nss].gotomark = gotomark;

                csbuf[*nss].s = (char*)xmalloc((isub+1)*sizeof(char));/// memory extended by 1 for safety
                memcpy(csbuf[*nss].s, ss, isub);
                csbuf[*nss].s[isub]='\0';/// incorporated for safety
                csbuf[*nss].len = isub;
                isub = 0;

                (*nss)++;
            }

            font = new_font;
            color = new_color;
            tm = tm_new;
            hshift = new_hshift;
            if (hshift != 0.0) {
                /* once a substring is manually advanced, all the following
                 * substrings will be advanced as well!
                 */
                new_hshift = 0.0;
            }
            vshift = new_vshift;
            underline = new_underline;
            overline = new_overline;
            kerning = new_kerning;
            direction = new_direction;
            advancing = new_advancing;
            ligatures = new_ligatures;
            cur_qtCharShift=new_qtCharShift;
            gotomark = new_gotomark;
            if (gotomark >= 0) {
                /* once a substring is manually advanced, all the following
                 * substrings will be advanced as well!
                 */
                new_gotomark = MARK_NONE;
            }
        }
        memcpy(&ss[isub], acc_buf, acc_len*sizeof(char));
        isub += acc_len;
    }
    
    xfree(acc_buf);
    xfree(buf);
    xfree(ss);

    return (csbuf);
}

static void reverse_string(char *s, int len)
{
    char cbuf;
    int i;
    
    if (s == NULL) {
        return;
    }
    
    for (i = 0; i < len/2; i++) {
        cbuf = s[i];
        s[i] = s[len - i - 1];
        s[len - i - 1] = cbuf;
    }
}


static VPoint bbox_ll, bbox_ur, bbox_ll_local, bbox_ur_local;

void return_last_string_bounding_box(view * bb)
{
    bb->xv1=MIN2(bbox_ll.x,bbox_ur.x);
    bb->xv2=MAX2(bbox_ll.x,bbox_ur.x);
    bb->yv1=MIN2(bbox_ll.y,bbox_ur.y);
    bb->yv2=MAX2(bbox_ll.y,bbox_ur.y);
}

void append_to_VPoint_storage(VPoint ** storage,int * alloc,int * nr,VPoint vp)
{
    //qDebug() << "Alloc=" << *alloc << "Nr=" << *nr;
    if (*storage==NULL || *alloc<=0)//no space so far
    {
    //qDebug() << "New Space";
        *alloc=8;
        *storage=new VPoint[*alloc];
        *nr=1;
        (*storage)[0]=vp;
    }
    else if (*alloc>*nr && *alloc>0)//we have space already and need no more
    {
    //qDebug() << "Enough Space";
        (*storage)[*nr]=vp;
        (*nr)++;
    }
    else//we have space already but need more space
    {
    //qDebug() << "Append Space";
        VPoint * n_storage=new VPoint[*alloc+8];
        memcpy(n_storage,*storage,sizeof(VPoint)*(*alloc));
        (*alloc)+=8;
        n_storage[*nr]=vp;
        (*nr)++;
        delete[] *storage;
        *storage=n_storage;
    }
}

void append_to_VPoint_storage2(VPoint ** storageA,VPoint ** storageB,int * alloc,int * nr,VPoint vpA,VPoint vpB)
{
int old_nr=*nr,old_alloc=*alloc;
append_to_VPoint_storage(storageA,alloc,nr,vpA);
*alloc=old_alloc;
*nr=old_nr;
append_to_VPoint_storage(storageB,alloc,nr,vpB);
}

/* this functions returns 4 points to indicate the bound-box of a string (the 4 points possibly represent a rotated rectangle) */
void create_string_edges(CompositeString * cstring,VPoint * string_edges,int rot)
{
double Angle = (double) rot;
double si = sin(M_PI/180.0*Angle);
double co = cos(M_PI/180.0*Angle);
    string_edges[0].x=cstring->start.x-cstring->ascent*si;
    string_edges[0].y=cstring->start.y+cstring->ascent*co;
    string_edges[1].x=cstring->stop.x-cstring->ascent*si;
    string_edges[1].y=cstring->stop.y+cstring->ascent*co;
    string_edges[2].x=cstring->stop.x+cstring->descent*si;
    string_edges[2].y=cstring->stop.y-cstring->descent*co;
    string_edges[3].x=cstring->start.x+cstring->descent*si;
    string_edges[3].y=cstring->start.y-cstring->descent*co;
}

/* this function takes 4 points and calculates the lower-left- and upper-right-point for a bounding-rectangle (the 4 points can possibly represent a rotated rectangle) */
void init_local_bounding_box(VPoint * string_edges,VPoint * ll,VPoint * ur)
{
ll->x=ur->x=string_edges[0].x;
ll->y=ur->y=string_edges[0].y;
    for (int i=1;i<4;i++)
    {
        if (string_edges[i].x<ll->x) ll->x=string_edges[i].x;
        if (string_edges[i].x>ur->x) ur->x=string_edges[i].x;
        if (string_edges[i].y<ll->y) ll->y=string_edges[i].y;
        if (string_edges[i].y>ur->y) ur->y=string_edges[i].y;
    }
}

void expand_local_bounding_box(VPoint * string_edges,VPoint * ll,VPoint * ur)
{
    if (ll->x<0.0 || ll->y<0.0 || ur->x<0.0 || ur->y<0.0)//this means that the whole box is invalid
    {
    ll->x=string_edges[0].x;
    ll->y=string_edges[0].y;
    ur->x=string_edges[0].x;
    ur->y=string_edges[0].y;
    }

    for (int i=0;i<4;i++)
    {
        if (string_edges[i].x<ll->x) ll->x=string_edges[i].x<0.0?0.0:string_edges[i].x;
        if (string_edges[i].x>ur->x) ur->x=string_edges[i].x<0.0?0.0:string_edges[i].x;
        if (string_edges[i].y<ll->y) ll->y=string_edges[i].y<0.0?0.0:string_edges[i].y;
        if (string_edges[i].y>ur->y) ur->y=string_edges[i].y<0.0?0.0:string_edges[i].y;
    }
}

void expand_local_bounding_box(VPoint string_edge,VPoint * ll,VPoint * ur)
{
    if (ll->x<0.0 || ll->y<0.0 || ur->x<0.0 || ur->y<0.0)
    {
    ll->x=string_edge.x;
    ll->y=string_edge.y;
    ur->x=string_edge.x;
    ur->y=string_edge.y;
    }

    if (string_edge.x<ll->x) ll->x=string_edge.x<0.0?0.0:string_edge.x;
    if (string_edge.x>ur->x) ur->x=string_edge.x<0.0?0.0:string_edge.x;
    if (string_edge.y<ll->y) ll->y=string_edge.y<0.0?0.0:string_edge.y;
    if (string_edge.y>ur->y) ur->y=string_edge.y<0.0?0.0:string_edge.y;
}

void expand_local_bounding_box(view string_edge,VPoint * ll,VPoint * ur)
{
    if (ll->x<0.0 || ll->y<0.0 || ur->x<0.0 || ur->y<0.0)
    {
    ll->x=string_edge.xv1;
    ll->y=string_edge.yv1;
    ur->x=string_edge.xv2;
    ur->y=string_edge.yv2;
    }

    if (string_edge.xv1<ll->x) ll->x=string_edge.xv1<0.0?0.0:string_edge.xv1;
    if (string_edge.xv1>ur->x) ur->x=string_edge.xv1<0.0?0.0:string_edge.xv1;
    if (string_edge.yv1<ll->y) ll->y=string_edge.yv1<0.0?0.0:string_edge.yv1;
    if (string_edge.yv1>ur->y) ur->y=string_edge.yv1<0.0?0.0:string_edge.yv1;

    if (string_edge.xv2<ll->x) ll->x=string_edge.xv2<0.0?0.0:string_edge.xv2;
    if (string_edge.xv2>ur->x) ur->x=string_edge.xv2<0.0?0.0:string_edge.xv2;
    if (string_edge.yv2<ll->y) ll->y=string_edge.yv2<0.0?0.0:string_edge.yv2;
    if (string_edge.yv2>ur->y) ur->y=string_edge.yv2<0.0?0.0:string_edge.yv2;
}

void WriteString(VPoint vp, int rot, int just, char * theString)
{
static int cur_lines=0;
static int lines_alloc=0;
static VPoint * line_st=NULL;
static VPoint * line_en=NULL;
static int nr_index=0;
static int * index=NULL;
static VPoint string_edges[4];

cur_lines=0;
nr_index=0;

    VPoint vptmp;
    double page_ipv, page_dpv;
    
    int def_font = getfont();
    int def_color = getcolor();
    int def_alpha = getalpha();

    double Angle = (double) rot;
    double si = sin(M_PI/180.0*Angle);
    double co = cos(M_PI/180.0*Angle);
    /* charsize (in VP units) */
    double charsize = MAGIC_FONT_SCALE*getcharsize();

    int text_advancing;

    int iss, nss;
    GLYPH *glyph;

    CompositeString *cstring = NULL;

    int pheight, pwidth;
    int hjust, vjust;
    double hfudge, vfudge;
    
    int setmark, gotomark;
    VPoint cs_marks[MAX_MARKS];
    VPoint rpoint, baseline_start, baseline_stop, offset;
    Device_entry dev;
    int nr_of_returns=0;

/// qDebug() << "WriteString=" << theString;

    if (theString == NULL || strlen(theString) == 0) {
        return;
    }
    
    if (charsize <= 0.0) {
        return;
    }

    dev = get_curdevice_props();

    useQtFunctions = true;  // all devices now use Qt font rendering

    /* inches per 1 unit of viewport */
    page_ipv = MIN2(page_width_in, page_height_in);

    /* dots per 1 unit of viewport */
    page_dpv = page_ipv*page_dpi;
//qDebug() << "dpv=" << page_dpv << "ipv=" << page_ipv << "dpi=" << page_dpi;

    hjust = just & 03;
    switch (hjust) {
    case JUST_LEFT:
        hfudge = 0.0;
        break;
    case JUST_RIGHT:
        hfudge = 1.0;
        break;
    case JUST_CENTER:
        hfudge = 0.5;
        break;
    default:
        errmsg(QObject::tr("Wrong justification type of string").toLocal8Bit().constData());
        return;
    }

    vjust = just & 014;
    switch (vjust) {
    case JUST_BOTTOM:
        vfudge = 0.0;
        break;
    case JUST_TOP:
        vfudge = 1.0;
        break;
    case JUST_MIDDLE:
        vfudge = 0.5;
        break;
    case JUST_BLINE:
        /* Not used; to make compiler happy */
        vfudge = 0.0;
        break;
    default:
        /* This can't happen; to make compiler happy */
        errmsg(QObject::tr("Internal error").toLocal8Bit().constData());
        return;
    }
    //cstring = String2Composite("A", &nss);

    cstring = String2Composite(theString, &nss);
    if (cstring == NULL) {
        return;
    }
    /* zero marks */
    for (gotomark = 0; gotomark < MAX_MARKS; gotomark++) {
        cs_marks[gotomark] = vp;
    }

    rpoint = vp;//reference point
    baseline_start = rpoint;//baseline = reference-point
    bbox_ll = rpoint;//initialize bound-box for whole string
    bbox_ur = rpoint;

    bbox_ll_local = rpoint;
    bbox_ur_local = rpoint;

append_to_VPoint_storage2(&line_st,&line_en,&lines_alloc,&cur_lines,rpoint,rpoint);
/*#ifdef DEBUG_SHOW_TEXT_BOUNDING_BOXES
//draw_simple_point(vp,2,0.007);
qDebug() << "Lines Alloc=" << lines_alloc << "cur_lines=" << cur_lines << line_st << " - " << line_en;
#endif*/
    reset_bbox(BBOX_TYPE_LOCAL);
    TextMatrix sav_text_matrix;
    for (iss = 0; iss < nss; iss++)
    {
    CompositeString *cs = &cstring[iss];
/*#ifdef DEBUG_SHOW_TEXT_BOUNDING_BOXES
    qDebug() << iss << "/" << nss-1 << "Write String, Substring:" << cs->s;
#endif*/
/*printf("WriteString in t1fonts: cs=#%s# font=%d\n",cs->s,cs->font);
fflush(stdout);*/
        /* Post-process the CS */
        if (cs->font == BAD_FONT_ID) {
            cs->font = def_font;
        }
        if (cs->color == BAD_COLOR) {
            cs->color = def_color;
        }
        if (cs->alpha<0 || cs->alpha>255) {
            cs->alpha = def_alpha;
        }
//qDebug() << "Write String alpha=" << def_alpha << " String=" << theString;
        // ligature processing removed with Type1
        if (cs->direction == STRING_DIRECTION_RL) {
            reverse_string(cs->s, cs->len);
        }
        sav_text_matrix=cs->tm;
        /// at first: we get the unrotated string (for the metrics)
        //tm_rotate(&cs->tm, Angle);
        tm_scale(&cs->tm, charsize);

        cs->vshift *= charsize;
        cs->hshift *= charsize;
        
        text_advancing = cs->advancing;
        gotomark = cs->gotomark;
        setmark = cs->setmark;

        /*cout << "first values=";
        for (int i=0;i<8;i++)
        cout << (int)((unsigned char)(cs->s[i])) << " ";
        cout << endl;*/

        glyph = GetQtGlyph(cs, page_dpv, dev.fontaa);
        if (glyph != NULL) {
            if (text_advancing == TEXT_ADVANCING_RL) {
                glyph->metrics.ascent  -= glyph->metrics.advanceY;
                glyph->metrics.descent -= glyph->metrics.advanceY;
            }
            cs->ascent = ((double) glyph->metrics.ascent)/page_dpv;
            cs->descent = -((double) glyph->metrics.descent)/page_dpv;
        }
        /// then: we get the actual rotated string (for printing)
        cs->tm=sav_text_matrix;
        tm_rotate(&cs->tm, Angle);
        tm_scale(&cs->tm, charsize);
        glyph = GetQtGlyph(cs, page_dpv, dev.fontaa);

        //cout << "s=#" << cs->s << "# asc=" << glyph->metrics.ascent << " dsc=" << glyph->metrics.descent << " lb=" << glyph->metrics.leftSideBearing << " rb=" <<  glyph->metrics.rightSideBearing << " ax=" << glyph->metrics.advanceX << " ay=" << glyph->metrics.advanceY << endl;
        //cout << "hs=" << cs->hshift << " vs=" << cs->vshift << " rot=" << rot << endl;
/*if (glyph==NULL)
{
printf("WriteString: GLYPH==NULL\n");
fflush(stdout);
}*/
        if (glyph != NULL) {
            VPoint hvpshift, vvpshift;

            if (text_advancing == TEXT_ADVANCING_RL) {
                glyph->metrics.leftSideBearing -= glyph->metrics.advanceX;
                glyph->metrics.rightSideBearing -= glyph->metrics.advanceX;
                glyph->metrics.advanceX *= -1;
                glyph->metrics.ascent  -= glyph->metrics.advanceY;
                glyph->metrics.descent -= glyph->metrics.advanceY;
                glyph->metrics.advanceY *= -1;
            }

            vvpshift.x = cs->tm.cxy*cs->vshift/tm_size(&cs->tm);
            vvpshift.y = cs->tm.cyy*cs->vshift/tm_size(&cs->tm);
            
            hvpshift.x = cs->tm.cxx*cs->hshift/tm_size(&cs->tm);
            hvpshift.y = cs->tm.cyx*cs->hshift/tm_size(&cs->tm);

            if (gotomark >= 0 && gotomark < MAX_MARKS) {
                rpoint = cs_marks[gotomark];
            } else if (gotomark == MARK_CR) {
                /* carriage return */
                rpoint = vp;
                nr_of_returns++;
                append_to_VPoint_storage2(&line_st,&line_en,&lines_alloc,&cur_lines,rpoint,rpoint);
                append_to_storage_single(&nr_index,&index,iss);
#ifdef DEBUG_SHOW_TEXT_BOUNDING_BOXES
                qDebug() << "we found a cr; lines_alloc=" << lines_alloc << "cur_lines=" << cur_lines;
                    /// draw_simple_rectangle(bbox_ll_local, bbox_ur_local,2);
                //we have a new text line now-->update complete bounding box!
                reset_bbox(BBOX_TYPE_LOCAL);
                bbox_ll_local = rpoint;
                bbox_ur_local = rpoint;
                bbox_ll_local.x += hvpshift.x;
                bbox_ll_local.y += hvpshift.y;
                bbox_ur_local.x += hvpshift.x;
                bbox_ur_local.y += hvpshift.y;
#endif
            }
            rpoint.x += hvpshift.x;
            rpoint.y += hvpshift.y;

            cs->start = rpoint;
            cs->start.x += vvpshift.x;
            cs->start.y += vvpshift.y;

        //draw_simple_point(cs->start,2,0.007);

            /* update bbox */
            vptmp.x = cs->start.x + (double) glyph->metrics.leftSideBearing/page_dpv;
            vptmp.y = cs->start.y + (double) glyph->metrics.descent/page_dpv;
            bbox_ll.x = MIN2(bbox_ll.x, vptmp.x);
            bbox_ll.y = MIN2(bbox_ll.y, vptmp.y);
            bbox_ll_local.x = MIN2(bbox_ll_local.x, vptmp.x);
            bbox_ll_local.y = MIN2(bbox_ll_local.y, vptmp.y);

            vptmp.x = cs->start.x + (double) glyph->metrics.rightSideBearing/page_dpv;
            vptmp.y = cs->start.y + (double) glyph->metrics.ascent/page_dpv;
            bbox_ur.x = MAX2(bbox_ur.x, vptmp.x);
            bbox_ur.y = MAX2(bbox_ur.y, vptmp.y);
            bbox_ur_local.x = MAX2(bbox_ur_local.x, vptmp.x);
            bbox_ur_local.y = MAX2(bbox_ur_local.y, vptmp.y);
            
            rpoint.x += (double) glyph->metrics.advanceX/page_dpv;
            rpoint.y += (double) glyph->metrics.advanceY/page_dpv;

            if (setmark >= 0 && setmark < MAX_MARKS) {
                cs_marks[setmark].x = rpoint.x;
                cs_marks[setmark].y = rpoint.y;
            }
            
            cs->stop = rpoint;
            cs->stop.x += vvpshift.x;
            cs->stop.y += vvpshift.y;

        //draw_simple_point(cs->stop,4,0.007);

            cs->glyph = glyph;  // GetQtGlyph returns a static — no copy needed
        } else {
            cs->glyph = NULL;
        }

        line_en[cur_lines-1].x=cs->stop.x;
        line_en[cur_lines-1].y=cs->stop.y;

    }//end loop over all substrings

    baseline_stop = rpoint;
    if (vjust == JUST_BLINE) {
        offset.x = baseline_start.x +
                   hfudge*(baseline_stop.x - baseline_start.x) - vp.x;
        offset.y = baseline_start.y +
                   hfudge*(baseline_stop.y - baseline_start.y) - vp.y;
    } else {
        offset.x = bbox_ll.x +
                   hfudge*(bbox_ur.x - bbox_ll.x) - vp.x;
        offset.y = bbox_ll.y +
                   vfudge*(bbox_ur.y - bbox_ll.y) - vp.y;
    }
    
    /* justification corrections */
    for (iss = 0; iss < nss; iss++)
    {
        glyph = cstring[iss].glyph;
        if (glyph == NULL)
        {
            continue;
        }
        cstring[iss].start.x -= offset.x;//these values are the baselines for each substring (just start and end, no rectangle)
        cstring[iss].start.y -= offset.y;
        cstring[iss].stop.x  -= offset.x;
        cstring[iss].stop.y  -= offset.y;

        //qDebug() << "overwriting:" <<  line_st[cur_lines-1].x << "|" << line_st[cur_lines-1].y << "-" << line_en[cur_lines-1].x << "|" << line_en[cur_lines-1].y;
        //draw_simple_line(line_st[cur_lines-1], line_en[cur_lines-1],6);
        /*line_en[cur_lines-1].x=cstring[iss].start.x;
        line_en[cur_lines-1].y=cstring[iss].start.y;*/

/*#ifdef DEBUG_SHOW_TEXT_BOUNDING_BOXES
//if wanted we draw the separate bounding boxes in black
draw_simple_line(cstring[iss].start, cstring[iss].stop,1);
#endif*/
    }

    for (iss=0;iss<cur_lines;iss++)
    {
        line_st[iss].x-= offset.x;
        line_st[iss].y-= offset.y;
        line_en[iss].x-= offset.x;
        line_en[iss].y-= offset.y;
    }

    /* update BB */
    bbox_ll.x -= offset.x;
    bbox_ll.y -= offset.y;
    bbox_ur.x -= offset.x;
    bbox_ur.y -= offset.y;

    bbox_ll_local.x -= offset.x;
    bbox_ll_local.y -= offset.y;
    bbox_ur_local.x -= offset.x;
    bbox_ur_local.y -= offset.y;

    /*
    //old version of bounding box calculation
    update_bboxes(bbox_ll);//now the bounding-box does include rotated text - the bounding box is also calculated a bit differently than before
    update_bboxes(bbox_ur);*/

//now we should know the complete bound-box of the complete string bbox_ll --> bbox_ur
#ifdef DEBUG_SHOW_TEXT_BOUNDING_BOXES
//if wanted we draw the complete bounding box in green
    //draw_simple_rectangle(bbox_ll, bbox_ur,3);//old bounding-box
qDebug() << "This Text contains" << nr_of_returns << "return-characters. CurLines=" << cur_lines;
#endif

    double * cur_len=NULL,max_len=0.0;
    double x_shift,y_shift;
    if (cur_lines>0) cur_len=new double[cur_lines];

#ifdef DEBUG_SHOW_TEXT_BOUNDING_BOXES
    qDebug() << "Nr of BreakIndex=" << nr_index;
    for (iss=0;iss<nr_index;iss++)
    {
    qDebug() << "Break_index:" << index[iss];
    }
#endif

    //we find the longest line in a text
    for (iss=0;iss<cur_lines;iss++)
    {
    cur_len[iss]=co*(line_en[iss].x-line_st[iss].x)+si*(line_en[iss].y-line_st[iss].y);
    //if wanted we draw the separate bounding boxes in black
    if (cur_len[iss]>max_len) max_len=cur_len[iss];
#ifdef DEBUG_SHOW_TEXT_BOUNDING_BOXES
    qDebug() << "Line:" << iss << "Length:" << cur_len[iss];
    line_st[iss].x=line_en[iss].x-cur_len[iss]*co;
    line_st[iss].y=line_en[iss].y-cur_len[iss]*si;
#endif
    }

    //now we update the bounding boxes of all lines (bbox_ll_local/bbox_ur_local) and of the whole text (bbox_ll/bbox_ur)

    //adjust alignment for all lines
    for (iss=0;iss<cur_lines;iss++)
    {

    if (currentStringAlignment==Qt::AlignRight)
    {
        x_shift=co*(max_len-cur_len[iss]);
        y_shift=si*(max_len-cur_len[iss]);
#ifdef DEBUG_SHOW_TEXT_BOUNDING_BOXES
    line_st[iss].x+=x_shift;
    line_st[iss].y+=y_shift;
    line_en[iss].x+=x_shift;
    line_en[iss].y+=y_shift;
#endif
    }
    else if (currentStringAlignment==Qt::AlignCenter)
    {
        x_shift=co*0.5*(max_len-cur_len[iss]);
        y_shift=si*0.5*(max_len-cur_len[iss]);
#ifdef DEBUG_SHOW_TEXT_BOUNDING_BOXES
    line_st[iss].x+=x_shift;
    line_st[iss].y+=y_shift;
    line_en[iss].x+=x_shift;
    line_en[iss].y+=y_shift;
#endif
    }
    else//Qt::AlignLeft
    {
    x_shift=y_shift=0.0;//no change
    }

    /*bbox_ll_local.x += x_shift;
    bbox_ll_local.y += y_shift;
    bbox_ur_local.x += x_shift;
    bbox_ur_local.y += y_shift;*/

    if (nr_index<=0)//only one line -->no change
    {
    ;
    }
    else//more than one line of text
    {
    if (iss<=0)//first line
    {
        for (int j=0;j<index[iss];j++)
        {
        cstring[j].start.x+=x_shift;
        cstring[j].stop.x+=x_shift;
        cstring[j].start.y+=y_shift;
        cstring[j].stop.y+=y_shift;
        create_string_edges(cstring+j,string_edges,rot);
            if (j==0)
            {
            init_local_bounding_box(string_edges,&bbox_ll_local,&bbox_ur_local);
            init_local_bounding_box(string_edges,&bbox_ll,&bbox_ur);
            }
            else
            {
            expand_local_bounding_box(string_edges,&bbox_ll_local,&bbox_ur_local);
            expand_local_bounding_box(string_edges,&bbox_ll,&bbox_ur);
            }
        }
#ifdef DEBUG_SHOW_TEXT_BOUNDING_BOXES
        qDebug() << "shift substring" << 0 << "-" << index[iss];
#endif
    }
    else if (iss>0 && iss<cur_lines-1)//middle lines
    {
        for (int j=index[iss-1];j<index[iss];j++)
        {
        cstring[j].start.x+=x_shift;
        cstring[j].stop.x+=x_shift;
        cstring[j].start.y+=y_shift;
        cstring[j].stop.y+=y_shift;
        create_string_edges(cstring+j,string_edges,rot);
        expand_local_bounding_box(string_edges,&bbox_ll,&bbox_ur);
            if (j==index[iss-1])
            {
            init_local_bounding_box(string_edges,&bbox_ll_local,&bbox_ur_local);
            }
            else
            {
            expand_local_bounding_box(string_edges,&bbox_ll_local,&bbox_ur_local);
            }
        }
#ifdef DEBUG_SHOW_TEXT_BOUNDING_BOXES
    qDebug() << "shift substring" << index[iss-1] << "-" << index[iss];
#endif
    }
    else//last line
    {
        for (int j=index[iss-1];j<nss;j++)
        {
        cstring[j].start.x+=x_shift;
        cstring[j].stop.x+=x_shift;
        cstring[j].start.y+=y_shift;
        cstring[j].stop.y+=y_shift;
        create_string_edges(cstring+j,string_edges,rot);
        expand_local_bounding_box(string_edges,&bbox_ll,&bbox_ur);
            if (j==index[iss-1])
            {
            init_local_bounding_box(string_edges,&bbox_ll_local,&bbox_ur_local);
            }
            else
            {
            expand_local_bounding_box(string_edges,&bbox_ll_local,&bbox_ur_local);
            }
        }
#ifdef DEBUG_SHOW_TEXT_BOUNDING_BOXES
        qDebug() << "shift substring" << index[iss-1] << "-" << nss;
#endif
    }
    }

    update_bbox(BBOX_TYPE_LOCAL, bbox_ll_local);
    update_bbox(BBOX_TYPE_LOCAL, bbox_ur_local);

#ifdef DEBUG_SHOW_TEXT_BOUNDING_BOXES
    draw_simple_line(line_st[iss], line_en[iss],4);
    draw_simple_rectangle(bbox_ll_local,bbox_ur_local,2);/// single-line: bounding box in red
#endif
    }//end loop over all line

    update_bboxes(bbox_ll);//now the bounding-box does include rotated text - the bounding box is also calculated a bit differently than before
    update_bboxes(bbox_ur);

#ifdef DEBUG_SHOW_TEXT_BOUNDING_BOXES
    draw_simple_rectangle(bbox_ll,bbox_ur,9);/// whole text: bounding box in violet
    //draw_simple_rectangle(bbox_ll_local, bbox_ur_local,2);
    qDebug() << "max_len=" << max_len << "cur_lines=" << cur_lines << "bb=" << bbox_ll.x << "|" << bbox_ll.y << "-" << bbox_ur.x << "|" << bbox_ur.y;
#endif
    if (get_draw_mode() == TRUE)
    {
        string_edges[0]=bbox_ll;
        string_edges[1]=bbox_ur;
        string_edges[2].x=bbox_ur.x;
        string_edges[3].y=bbox_ur.y;
        string_edges[2].y=bbox_ll.y;
        string_edges[3].x=bbox_ll.x;
        //qDebug() << "expanding bounding box: Text=" << theString << " --> bb=(" << string_edges[0].x << ")|(" << string_edges[0].y << ")-(" << string_edges[1].x << ")|(" << string_edges[1].y << ")-(" << string_edges[2].x << ")|(" << string_edges[2].y << ")-(" << string_edges[3].x << ")|(" << string_edges[3].y << ")";
    expand_local_bounding_box(string_edges,&global_bb_ll,&global_bb_ur);
    }

    //delete index-storage
    if (nr_index>0)
    {
    delete[] index;
    index=NULL;
    }
    if (cur_lines>0)
    delete[] cur_len;

    //VPoint debug_bounding_box[4];
    GeneralPainter->setRenderHint(QPainter::Antialiasing,false);
/// Now we should know all the bounding boxes
    for (iss = 0; iss < nss; iss++)
    {
        CompositeString * cs = &cstring[iss];
    /*draw_simple_point(cs->start,4,0.007);
    draw_simple_point(cs->stop,4,0.007);*/

        /*
#ifdef DEBUG_SHOW_TEXT_BOUNDING_BOXES
            setcolor(2);
            debug_bounding_box[0].x=cs->start.x-cs->ascent*si;
            debug_bounding_box[0].y=cs->start.y+cs->ascent*co;
            debug_bounding_box[1].x=cs->stop.x-cs->ascent*si;
            debug_bounding_box[1].y=cs->stop.y+cs->ascent*co;
            debug_bounding_box[2].x=cs->stop.x+cs->descent*si;
            debug_bounding_box[2].y=cs->stop.y-cs->descent*co;
            debug_bounding_box[3].x=cs->start.x+cs->descent*si;
            debug_bounding_box[3].y=cs->start.y-cs->descent*co;
        (*devdrawpolyline)(debug_bounding_box,4,POLYLINE_CLOSED);
#endif
            */

        /// now we print every substring --> SubString is also every String with different fonts
        /// qDebug() << "Substrings: cs=#" << cs->s << "#len=" << cs->len << " ShiftCode=" << cs->qtCharShift << "DrawMode=" << get_draw_mode();
        glyph = cs->glyph;
        if (glyph == NULL && useQtFunctions==false)
        {
            /// qDebug() << "Error: glyph=NULL";
            continue;
        }
        
        pheight = glyph->metrics.ascent - glyph->metrics.descent;
        pwidth  = glyph->metrics.rightSideBearing - glyph->metrics.leftSideBearing;

        if ((pheight <= 0 || pwidth <= 0))// && useQtFunctions==false)
        {
            qDebug() << "error, size below zero: h=" << pheight << " w=" << pwidth << " left=" << glyph->metrics.leftSideBearing << " right=" << glyph->metrics.rightSideBearing;
            continue;
        }
        /// qDebug() << "DrawMode=" << get_draw_mode();
        if (get_draw_mode() == TRUE)
        {
            /* No patterned texts yet */
            setpattern(1);
            setcolor(cs->color);
            setalpha(cs->alpha);
        //qDebug() << "Color=" << cs->color << " alpha=" << cs->alpha;
            //cout << "dev.devfonts=" << dev.devfonts << endl;
            //cout << "useQtFunctions=" << useQtFunctions << endl;
            //if (dev.devfonts == TRUE || useQtFunctions == true)/// I added 'useQtFunctions' to remove the necessity for devfonts
            if (devputtext != NULL)
            {
                vptmp = (cs->advancing == TEXT_ADVANCING_RL) ? cs->stop : cs->start;
                qtCharShift = cs->qtCharShift;
                (*devputtext)(vptmp, cs->s, cs->len, cs->font, &cs->tm, cs->underline, cs->overline, cs->kerning);
                qtCharShift = 0;
            }
        }
    }

    GeneralPainter->setRenderHint(QPainter::Antialiasing,general_antialiasing);

    FreeCompositeString(cstring, nss);
}
