/*
 * Grace - GRaphing, Advanced Computation and Exploration of data
 *
 * Modified by Andreas Winter 2008-2022
 * Qt6 port and Type1 removal: QtGrace6 project
 *
 * GNU General Public License v2+
 */

#ifndef __T1_FONTS_H_
#define __T1_FONTS_H_

#include <math.h>
#include "defines.h"

#define BAD_FONT_ID     -1

/* Font mappings */
#define FONT_MAP_DEFAULT    0
#define FONT_MAP_ACEGR      1

#define MAGIC_FONT_SCALE    0.028

#define SSCRIPT_SCALE M_SQRT1_2
#define SUBSCRIPT_SHIFT 0.4
#define SUPSCRIPT_SHIFT 0.6
#define ENLARGE_SCALE sqrt(M_SQRT2)
#define OBLIQUE_FACTOR 0.25

#define TEXT_ADVANCING_LR   0
#define TEXT_ADVANCING_RL   1

#define STRING_DIRECTION_LR 0
#define STRING_DIRECTION_RL 1

#define MARK_NONE   -1
#define MAX_MARKS   32
#define MARK_CR     MAX_MARKS

#define UNIT_TM {1.0, 0.0, 0.0, 1.0}

#ifdef __cplusplus
extern "C" {
#endif

/* Minimal glyph metrics — populated by GetQtGlyph(), no bitmap data */
typedef struct {
    struct {
        int ascent;
        int descent;
        int leftSideBearing;
        int rightSideBearing;
        int advanceX;
        int advanceY;
    } metrics;
    char *bits;  /* always NULL — Qt renders directly, no bitmap */
    int   bpp;   /* always 0 */
} GLYPH;

typedef struct {
    double cxx, cxy;
    double cyx, cyy;
} TextMatrix;

typedef struct {
    char *s;
    int len;
    int font;
    int color;
    int alpha;
    TextMatrix tm;
    double hshift;
    double vshift;
    int underline;
    int overline;
    int setmark;
    int gotomark;
    int direction;
    int advancing;
    int ligatures;
    int kerning;
    int qtCharShift;
    VPoint start;
    VPoint stop;
    double ascent, descent;
    GLYPH *glyph;
} CompositeString;

typedef struct {
    int mapped_id;
    char *alias;
    char *fallback;
} FontDB;

int init_t1(void);

unsigned int number_of_fonts(void);
char *get_fontname(int font);
char *get_fontfullname(int font);
char *get_fontfamilyname(int font);
char *get_fontweight(int font);
char *get_fontfilename(int font, int abspath);
char *get_afmfilename(int font, int abspath);
char *get_fontalias(int font);
char *get_fontfallback(int font);
char *get_encodingscheme(int font);
double get_textline_width(int font);
double get_underline_pos(int font);
double get_overline_pos(int font);
double get_italic_angle(int font);

int get_font_by_name(const char *fname);
int get_font_mapped_id(int font);
int get_mapped_font(int mapped_id);
int map_font(int font, int mapped_id);
int map_font_by_name(const char *fname, int mapped_id);
void map_fonts(int map);
int font_map_equals_font_ids(void);

#ifdef __cplusplus
}
#endif

#endif /* __T1_FONTS_H_ */
