/*
 * Grace - GRaphing, Advanced Computation and Exploration of data
 * 
 * Home page: http://plasma-gate.weizmann.ac.il/Grace/
 * 
 * Copyright (c) 1991-1995 Paul J Turner, Portland, OR
 * Copyright (c) 1996-2002 Grace Development Team
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
 * Graph utils
 *
 */

#ifndef __GRAPHUTILS_H_
#define __GRAPHUTILS_H_

#define GSCROLL_LEFT    0
#define GSCROLL_RIGHT   1
#define GSCROLL_DOWN    2
#define GSCROLL_UP      3

#define GZOOM_SHRINK    0
#define GZOOM_EXPAND    1

#define GOVERLAY_SMART_AXES_DISABLED  0
#define GOVERLAY_SMART_AXES_NONE      1
#define GOVERLAY_SMART_AXES_X         2
#define GOVERLAY_SMART_AXES_Y         3
#define GOVERLAY_SMART_AXES_XY        4

/* Order of matrix fill (inversion mask bits) */
#define GA_ORDER_V_INV  1
#define GA_ORDER_H_INV  2
#define GA_ORDER_HV_INV 4

/* Default page offsets and gaps for graph arranging */
#define GA_OFFSET_DEFAULT    0.15
#define GA_GAP_DEFAULT       0.2

/*Attachement types for a graphs legend box*/
#define G_LB_ATTACH_NONE   0
#define G_LB_ATTACH_LEFT   1
#define G_LB_ATTACH_RIGHT  2
#define G_LB_ATTACH_TOP    4
#define G_LB_ATTACH_BOTTOM 8

#include "defines.h"

#ifdef __cplusplus
extern "C" {
#endif

const char *get_format_types(int f);

int wipeout(void);

void scroll_proc(int value);
void scrollinout_proc(int value);
int graph_scroll(int type);
int graph_zoom(int type);
//int graph_zoom(int type,int nr_of_graphs,int * graph_nrs);

int overlay_graphs(int gsec, int gpri, int type);

int arrange_graphs(int *graphs, int ngraphs,
                   int nrows, int ncols, int order, int snake,
                   double loff, double roff, double toff, double boff,
                   double vgap, double hgap,
                   int hpack, int vpack, int move_legend,
                   double legendX, double legendY,
                   int packXInner, int packXOuter, int packYInner, int packYOuter,
                   int packXInnerDir, int packXOuterDir, int packYInnerDir, int packYOuterDir);
int arrange_graphs_simple(int nrows, int ncols,
    int order, int snake, double offset, double hgap, double vgap);

void autotick_axis(int gno, int axis);
void autoscale_byset(int gno, int setno, int autos_type);
int autoscale_graph(int gno, int autos_type);

void move_legend(int gno, VVector shift);
void move_timestamp(VVector shift);

void rescale_viewport(double ext_x, double ext_y);

//get the position (nvx,nvy) in viewport-coordinates needed to align the legendbox from the graph from_gno to the attachement-axis of graph to_gno
void position_leg_box(int from_gno,int to_gno,int attachement,double * nvx,double * nvy);
int CheckWorldIntegrity(int gno);
void zoomGraphWorld(int gno, world orig_world, WPoint wp, double factor);//sets the center of the Graph gno to wp and scales by factor (orig_world is the initial graph world)
void zoomGraphWorldKeepingFixedPoint(int gno,world orig_world,WPoint wp,double factor,int axis);//same as above, but with the wp not in the center, but at the same viewport-position as before (courtesy of Francesco Sanfilippo)
int find_nearest_graph(VPoint vp);//finds the graph nearest to a point (in viewport coordinates); inside a graph or outside a graph does not matter - the one with the lowest distance is found
void quickSetAxisFormat(int gno,int axis,int quickformat);//select an axis transformation (normel/degrees/multiples of PI)

/*shifts an axis-label by a vector (and sets the axis-label-position to special)*/
void move_axis_label(int gno,int axis,VVector shift);
/*moves a graph to a new viewport (the shift is defined by a vector) */
void move_graph_vp(int gno,VVector shift);
/*moves a title of a graph (title_subtitle=0=Title, title_subtitle=1=Subtitle)*/
void move_graph_title(int gno,int title_subtitle,VVector shift);
/*increases or decreases font-size for axis-label by factor*/
void change_axis_label_size(int gno,int axis,double factor);
/*increases or decreases font-size for axis-tick-labels by factor*/
void change_axis_tick_label_size(int gno,int axis,double factor);
/*increases or decreases font-size for axis-label by factor*/
void change_title_size(int gno,int title_subtitle,double factor);
/*increases or decreases font-size for the timestamp by factor*/
void change_timestamp_size(double factor);
/*increases or decreases font-size for the legend by factor*/
void change_legend_font_size(int gno,double factor);
/*increases or decreases size of an object (font size for text)*/
void change_object_size(int type,int id,double factor);

#ifdef __cplusplus
}
#endif

#endif /* __GRAPHUTILS_H_ */
