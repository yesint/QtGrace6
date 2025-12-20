/*
 * Grace - GRaphing, Advanced Computation and Exploration of data
 * 
 * Home page: http://plasma-gate.weizmann.ac.il/Grace/
 * 
 * Copyright (c) 1991-1995 Paul J Turner, Portland, OR
 * Copyright (c) 1996-2000 Grace Development Team
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

/*
 *
 * set defaults - changes to the types in defines.h
 * will require changes in here also
 *
 */

#include <QObject>
///#include <config.h>
#include <cmath>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "draw.h"
#include "defines.h"
#include "globals.h"
#include "graphs.h"
#include "graphutils.h"
#include "utils.h"
#include "noxprotos.h"
#include "device.h"
#include "fundamentals.h"

defaults d_d =
{1, 255, 0, 255, 1, 1, 1, 1.0, 0, 1.0};

/* defaults:
    int color;
    int bgcolor;
    int pattern;
    int lines;
    double linew;
    double charsize;
    int font;
    double symsize;
*/
/* d_d = std-Grace-defaults*/
/* file_defaults = defaults stored in the project file */
/* grdefaults = defaults currently in use */
/* ini_defaults = the defaults loaded from the ini-file*/

defaults file_defaults;//The defaults stored in the current project (agr) file
defaults ini_defaults;//Used to load and store data (ini-file)
defaults tmp_defaults;//only for temporary storage (save and restore)

static world d_w =
{0.0, 1.0, 0.0, 1.0};

view d_v =
{0.15, 0.85, 0.15, 0.85};

view file_view;
view ini_view;
view tmp_view;

void set_program_defaults(void)
{
    int i;
    
    grdefaults = d_d;
    grview=d_v;
        
    for (i = 0; i <= MAXREGION; i++) {
        set_region_defaults(i);
    }
    
    set_default_string(&timestamp);
    
    timestamp.x = 0.03;
    timestamp.y = 0.03;
    
    target_set.gno = -1;
    target_set.setno = -1;
}

void set_region_defaults(int rno)
{
    rg[rno].active = FALSE;
    rg[rno].type = 0;
    rg[rno].color = grdefaults.color;
    rg[rno].alpha = grdefaults.alpha;
    rg[rno].lines = grdefaults.lines;
    rg[rno].linew = grdefaults.linew;
    rg[rno].n = 0;
    rg[rno].x = rg[rno].y = NULL;
    rg[rno].x1 = rg[rno].y1 = rg[rno].x2 = rg[rno].y2 = 0.0;
    rg[rno].linkto = 0;
}

void set_default_framep(framep * f)
{
    f->type = 0;                /* frame type */
    f->lines = grdefaults.lines;
    f->linew = grdefaults.linew;
    f->pen.color = grdefaults.color;
    f->pen.alpha = grdefaults.alpha;
    f->pen.pattern = grdefaults.pattern;
    f->fillpen.color = grdefaults.bgcolor;      /* fill background */
    f->fillpen.alpha = grdefaults.bgalpha;
    f->fillpen.pattern = 0;
}

void set_default_world(world * w)
{
    memcpy(w, &d_w, sizeof(world));
}

void set_default_view(view * v)
{
    memcpy(v, &grview, sizeof(view));
}

void set_default_string(plotstr * s)
{
    s->active = FALSE;
    s->path = FALSE;
    s->loctype = COORD_VIEW;
    s->gno = 0;
    s->x = s->y = 0.0;
    s->color = grdefaults.color;
    s->alpha = grdefaults.alpha;
    s->rot = 0;
    s->font = grdefaults.font;
    s->just = JUST_LEFT|JUST_BLINE;
    s->align = JUST_LEFT;
    s->master_align = 0;
    s->charsize = grdefaults.charsize;
    s->s_plotstring = NULL;
    s->alt_plotstring = NULL;
}

void set_default_arrow(Arrow *arrowp)
{
    arrowp->type = line_atype;
    arrowp->length = line_asize;
    arrowp->dL_ff = line_a_dL_ff;
    arrowp->lL_ff = line_a_lL_ff;
}

void set_default_line(linetype * l)
{
    l->active = FALSE;
    l->loctype = COORD_VIEW;
    l->gno = 0;
    l->x1 = l->y1 = l->x2 = l->y2 = 0.0;
    l->lines = grdefaults.lines;
    l->linew = grdefaults.linew;
    l->color = grdefaults.color;
    l->alpha = grdefaults.alpha;
    l->arrow_end = 0;
    set_default_arrow(&l->arrow);
}

void set_default_box(boxtype * b)
{
    b->active = FALSE;
    b->loctype = COORD_VIEW;
    b->gno = 0;
    b->rot = 0;
    b->x1 = b->y1 = b->x2 = b->y2 = 0.0;
    b->lines = grdefaults.lines;
    b->linew = grdefaults.linew;
    b->color = grdefaults.color;
    b->alpha = grdefaults.alpha;
    b->fillcolor = grdefaults.color;
    b->fillalpha = grdefaults.alpha;
    b->fillpattern = grdefaults.pattern;
    b->filltype = 0;//static color (no image)
    setDefaultImage(b->fillimage);
}

void set_default_ellipse(ellipsetype * e)
{
    e->active = FALSE;
    e->loctype = COORD_VIEW;
    e->gno = 0;
    e->rot = 0;
    e->x1 = e->y1 = e->x2 = e->y2 = 0.0;
    e->lines = grdefaults.lines;
    e->linew = grdefaults.linew;
    e->color = grdefaults.color;
    e->alpha = grdefaults.alpha;
    e->fillcolor = grdefaults.color;
    e->fillalpha = grdefaults.alpha;
    e->fillpattern = grdefaults.pattern;
    e->filltype = 0;//static color (no image)
    setDefaultImage(e->fillimage);
}

void set_default_legend(int gno, legend * l)
{
    l->active = TRUE;
    //l->nr_of_entries = 0;
    l->loctype = COORD_VIEW;
    l->vgap = 1;
    l->hgap = 1;
    l->len = 4;
    l->invert = FALSE;
    l->legx = 0.5;
    l->legy = 0.8;
    l->font = grdefaults.font;
    l->align = JUST_LEFT;
    l->charsize = grdefaults.charsize;
    l->color = grdefaults.color;
    l->alpha = grdefaults.alpha;
    l->boxpen.color = grdefaults.color;
    l->boxpen.alpha = grdefaults.alpha;
    l->boxpen.pattern = grdefaults.pattern;
    l->boxfillpen.color = 0;
    l->boxfillpen.alpha = grdefaults.alpha;
    l->boxfillpen.pattern = grdefaults.pattern;
    l->boxlinew = grdefaults.linew;
    l->boxlines = grdefaults.lines;
    l->bb.xv1 = l->bb.xv2 = l->bb.yv1 = l->bb.yv2 = 0.0;
    l->autoattach = G_LB_ATTACH_NONE;/*no auto-attachment*/
    l->autoattachG = gno;
    l->xshift=l->yshift=0.0;
}

void set_default_plotarr(plotarr * p)
{
    int i;
    p->hidden = FALSE;                          /* hidden set */
    p->type = SET_XY;                           /* dataset type */
    p->hotlink = FALSE;                         /* hot linked set */
    p->hotfile[0] = '\0';                       /* hot linked file name */

    p->sym = SYM_NONE;                          /* set plot symbol */
    p->symlines = grdefaults.lines;             /* set plot sym line style */
    p->symsize = grdefaults.symsize;            /* size of symbols */
    p->symlinew = grdefaults.linew;             /* set plot sym line width */
    p->sympen.color = grdefaults.color;         /* color for symbol line */
    p->sympen.alpha = grdefaults.alpha;
    p->sympen.pattern = grdefaults.pattern;     /* pattern */
    p->symfillpen.color = grdefaults.color;     /* color for symbol fill */
    p->symfillpen.alpha = grdefaults.alpha;
    p->symfillpen.pattern = 0;                  /* pattern for symbol fill */

    p->symchar = 'A';
    p->charfont = grdefaults.font;

    p->symskip = 0;                             /* How many symbols to skip */

    p->avalue.active = FALSE;                   /* active or not */
    p->avalue.type = AVALUE_TYPE_Y;             /* type */
    p->avalue.size = 1.0;                       /* char size */
    p->avalue.font = grdefaults.font;           /* font */
    p->avalue.align = JUST_LEFT;
    p->avalue.color = grdefaults.color;         /* color */
    p->avalue.alpha = grdefaults.alpha;
    p->avalue.angle = 0;                        /* rotation angle */
    p->avalue.format = FORMAT_GENERAL;          /* format */
    p->avalue.prec = 3;                         /* precision */
    p->avalue.prestr[0] = '\0';
    p->avalue.appstr[0] = '\0';
    p->avalue.orig_appstr[0] = '\0';
    p->avalue.orig_prestr[0] = '\0';
    p->avalue.offset.x = 0.0;
    p->avalue.offset.y = 0.0;

    p->linet = LINE_TYPE_STRAIGHT;
    p->lines = grdefaults.lines;
    p->linew = grdefaults.linew;
    p->linepen.color = grdefaults.color;
    p->linepen.alpha = grdefaults.alpha;
    p->linepen.pattern = grdefaults.pattern;
    
    p->baseline_type = BASELINE_TYPE_0;
    p->baseline = FALSE;
    p->dropline = FALSE;

    p->filltype = SETFILL_NONE;                 /* fill type */
    p->fillrule = FILLRULE_WINDING;             /* fill type */
    p->setfillpen.color = grdefaults.color;     /* fill color */
    p->setfillpen.alpha = grdefaults.alpha;
    p->setfillpen.pattern = grdefaults.pattern; /* fill pattern */
    p->polygone_base_set=-1;                    /* don't fill area compared to other set */

    p->errbar.active = TRUE;                      /* on by default */
    p->errbar.connect_bars = FALSE;               /* by default the error bars are bars, 'connect' draws lines instead of the bar-lines*/
    p->errbar.show_in_legend = FALSE;             /* by default do not show error bars in legends */
    p->errbar.ptype = PLACEMENT_BOTH;             /* error bar placement */
    p->errbar.pen.color = grdefaults.color;       /* color */
    p->errbar.pen.alpha = grdefaults.alpha;
    p->errbar.pen.pattern = grdefaults.pattern;   /* pattern */
    p->errbar.lines = grdefaults.lines;           /* error bar line width */
    p->errbar.linew = grdefaults.linew;           /* error bar line style */
    p->errbar.riser_linew = grdefaults.linew;     /* riser line width */
    p->errbar.riser_lines = grdefaults.lines;     /* riser line style */
    p->errbar.barsize = 1.0;                      /* size of error bar */
    p->errbar.arrow_clip = FALSE;                 /* draw arrows if clipped */
    p->errbar.cliplen = 0.1;                      /* max v.p. riser length */

    p->comments[0] = 0;                           /* how did this set originate */
    p->lstr[0] = 0;                               /* legend string */
    p->orig_comments[0] = 0;
    p->orig_lstr[0] = 0;

    p->data.len = 0;                              /* dataset length */
    for (i = 0; i < MAX_SET_COLS; i++) {
        p->data.ex[i] = NULL;
        p->pref_col_format[i] = SpreadsheetColumnFormat;
        p->pref_col_prec[i] = SpreadsheetColumnPrecision;
    }
    p->data.s = NULL;                   /* pointer to strings */
    p->data.orig_s = NULL;
    p->ignore_in_autoscale = FALSE;
}

void set_default_ticks(tickmarks *t)
{
    int i;

    if (t == NULL) {
        return;
    }
    
    t->active = TRUE;
    t->zero = FALSE;
    t->tl_flag = TRUE;
    t->t_flag = TRUE;
    set_default_string(&t->label);
    t->label.x = 0.0;
    t->label.y = 0.08;
    t->tmajor = 0.2;
    t->nminor = 1;
    t->t_round = TRUE;
    t->offsx = 0.0;
    t->offsy = 0.0;
    t->label_layout = LAYOUT_PARALLEL;
    t->label_place = TYPE_AUTO;
    t->label_op = PLACEMENT_NORMAL;
    t->tl_format = FORMAT_GENERAL;
    t->tl_prec = 5;
    t->tl_formula = NULL;
    t->tl_angle = 0;
    t->tl_skip = 0;
    t->tl_staggered = 0;
    t->tl_starttype = TYPE_AUTO;
    t->tl_stoptype = TYPE_AUTO;
    t->tl_start = 0.0;
    t->tl_stop = 0.0;
    t->tl_op = PLACEMENT_NORMAL;
    t->tl_gaptype = TYPE_AUTO;
    t->tl_gap.x = 0.0;
    t->tl_gap.y = 0.01;
    t->tl_font = grdefaults.font;
    t->tl_align = JUST_LEFT;
    t->tl_charsize = grdefaults.charsize;
    t->tl_color = grdefaults.color;
    t->tl_alpha = grdefaults.alpha;
    t->tl_appstr[0] = 0;
    t->tl_prestr[0] = 0;
    t->orig_tl_appstr[0] = 0;
    t->orig_tl_prestr[0] = 0;
    t->t_spec = TICKS_SPEC_NONE;
    t->t_autonum = 6;
    t->t_inout = TICKS_IN;
    t->t_op = PLACEMENT_BOTH;
    t->props.size = grdefaults.charsize;
    t->mprops.size = grdefaults.charsize / 2;
    t->t_drawbar = TRUE;
    t->t_drawbarcolor = grdefaults.color;
    t->t_drawbaralpha = grdefaults.alpha;
    t->t_drawbarlines = grdefaults.lines;
    t->t_drawbarlinew = grdefaults.linew;
    t->props.gridflag = FALSE;
    t->mprops.gridflag = FALSE;
    t->props.color = grdefaults.color;
    t->props.alpha = grdefaults.alpha;
    t->props.lines = grdefaults.lines;
    t->props.linew = grdefaults.linew;
    t->mprops.color = grdefaults.color;
    t->mprops.alpha = grdefaults.alpha;
    t->mprops.lines = grdefaults.lines;
    t->mprops.linew = grdefaults.linew;
    t->nticks = 0;
    for (i = 0; i < MAX_TICKS; i++)
    {
        t->tloc[i].wtpos = 0.0;
        t->tloc[i].label = NULL;
        t->tloc[i].orig_label=NULL;
    }
}

void set_default_userdevicegeometry(UserDeviceGeometry * udg)
{
    udg->active=TRUE;
    strcpy(udg->name,QObject::tr("No Changes").toLocal8Bit().constData());
    udg->dev_nr=-1;
    udg->orientation=-1;//0=Landscape, 1=Portrait, -1=no change
    udg->width=-1;
    udg->height=-1;
    udg->unit=-1;//0=pix, 1=inch, 2=cm, -1=no change
    udg->dpi=-1;
    udg->Antialiasing=-1;//0=No Antialiasing,1=Font Antialiasing Only,2=Graph Antialiaing Only, 3=Graph and Font Antialiasing, -1=no change
    udg->ScaleLineWidth=-1;//0=no Scaling, 1=Scaling, -1=no change
}

