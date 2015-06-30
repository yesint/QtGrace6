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

#ifdef __cplusplus
extern "C" {
#endif

char *get_format_types(int f);

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
                   int hpack, int vpack, int move_legend);
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

#ifdef __cplusplus
}
#endif

#endif /* __GRAPHUTILS_H_ */
