/*
 * Grace - Graphics for Exploratory Data Analysis
 * 
 * Home page: http://plasma-gate.weizmann.ac.il/Grace/
 * 
 * Copyright (c) 1991-95 Paul J Turner, Portland, OR
 * Copyright (c) 1996-99 Grace Development Team
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
 * Canvas events
 *
 */

#ifndef __EVENTS_H_
#define __EVENTS_H_

/*#include <config.h>*/

/*#include <X11/Xlib.h>
#include <X11/Intrinsic.h>*/

/* for canvas event proc */
typedef enum {
    DO_NOTHING,
    ZOOM_1ST,
    ZOOM_2ND,
    VIEW_1ST,
    VIEW_2ND,
    STR_LOC,
    PLACE_LEGEND_1ST,
    PLACE_LEGEND_2ND,
    DEL_POINT,
    MOVE_POINT1ST,
    MOVE_POINT2ND,
    ADD_POINT,
    DEL_OBJECT,
    MOVE_OBJECT_1ST,
    MOVE_OBJECT_2ND,
    MAKE_BOX_1ST,
    MAKE_BOX_2ND,
    MAKE_LINE_1ST,
    MAKE_LINE_2ND,
    MAKE_ELLIP_1ST,
    MAKE_ELLIP_2ND,
    SEL_POINT,
    COMP_AREA,
    COMP_PERIMETER,
    TRACKER,
    DEF_REGION,
    DEF_REGION1ST,
    DEF_REGION2ND,
    EDIT_OBJECT,
    PLACE_TIMESTAMP_1ST,
    PLACE_TIMESTAMP_2ND,
    COPY_OBJECT1ST,
    COPY_OBJECT2ND,
    AUTO_NEAREST,
    ZOOMX_1ST,
    ZOOMX_2ND,
    ZOOMY_1ST,
    ZOOMY_2ND,
    DISLINE1ST,
    DISLINE2ND,
    PAN_1ST,
    PAN_2ND,
    PICK_MOVE,
    MOVE_AXIS_LABEL_1ST,
    MOVE_AXIS_LABEL_2ND,
    MOVE_GRAPH_1ST,
    MOVE_GRAPH_2ND,
    MOVE_TITLE_1ST,
    MOVE_TITLE_2ND,
    CORNER_2ND
} CanvasAction;

typedef enum {
    TARGET_NOTHING,
    TARGET_TIMESTAMP,
    TARGET_GRAPH,
    TARGET_SET,
    TARGET_OBJECT,
    TARGET_AXIS,
    TARGET_AXIS_LABEL,
    TARGET_TITLE,
    TARGET_LEGEND,
    TARGET_LEGEND_BOX
} ClickTarget;

typedef struct {
int type;
struct{int x;int y;} xmotion;
struct{int x;int y;int button;} xbutton;
int key;
bool alt,ctrl,shift;
int delta,axis_restrict;
bool doubleClick;
} special_XEvent;

/* add points at */
#define ADD_POINT_BEGINNING 0
#define ADD_POINT_END       1
#define ADD_POINT_NEAREST   2

/* move points */
#define MOVE_POINT_XY   0
#define MOVE_POINT_X    1
#define MOVE_POINT_Y    2

/*
 * double click detection interval (ms)
 */
#define CLICKINT 400

#define MAXPICKDIST 0.015     /* the maximum distance away from an object */
                              /* you may be when picking it (in viewport  */
                              /* coordinates)                             */  

void anchor_point(int curx, int cury, VPoint curvp);
void my_proc(special_XEvent *event);
///void set_actioncb(void *data);
void set_action(CanvasAction act);
CanvasAction get_action(void);
void track_point(int gno, int setno, int *loc, int shift);
void getpoints(VPoint *vpp);
void set_stack_message(void);
void do_select_area(void);
void do_select_peri(void);
void do_dist_proc(void);
void do_select_region(void);
int next_graph_containing(int cg, VPoint vp);
int get_graph_truly_containing_vp(VPoint vp);
int graph_clicked(int gno, VPoint vp);
int focus_clicked(int cg, VPoint vp, VPoint *avp);
int lower_right_corner_clicked(VPoint vp);
int legend_clicked(int gno, VPoint vp, view *bb);
int timestamp_clicked(VPoint vp, view *bb);
int axis_clicked(int gno, VPoint vp, int *axisno);
int axis_custom_tick_clicked(int gno, VPoint vp, int * axisno, int * tick_nr);
int get_axis_label_viewport(int gno, int ax,view * vp,bool for_click,int * rot,int alt);
int axis_label_clicked(int *gno, VPoint vp, int *axisno, int * alt);
int title_clicked(int gno, VPoint vp, int *lower);
int find_insert_location(int gno, int setno, VPoint vp);
int find_point(int gno, VPoint vp, int *setno, int *loc);
void newworld(int gno, int axes, VPoint vp1, VPoint vp2);
void push_and_zoom(void);
int hideDialog(void);
ClickTarget getClickTarget(VPoint vp, view * bb, int * parameters);/*parameters return the ids of the different objects (max parameters = 4)*/
void debugClickTarget(ClickTarget ct, view bb, int * parameters);/*just for debug to report on the found click target*/

/* action routines */
void set_viewport_action(void);
void enable_zoom_action(void);
void autoscale_action(void);
void autoscale_on_near_action(void);
void draw_line_action(void);
void draw_box_action(void);
void draw_ellipse_action(void);
void write_string_action(void);
void delete_object_action(void);
void move_object_action(void);
void place_legend_action(void);
void place_timestamp_action(void);
void refresh_hotlink_action(void);
void exit_abruptly_action(void);

void update_point_locator(int gno, int setno, int loc);
void get_tracking_props(int *setno, int *move_dir, int *add_at);

#endif /* __EVENTS_H_ */
