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
 * event handler
 *
 */

//#include <config.h>
#include <cmath>

#include <stdio.h>
#include <string.h>

#include <iostream>
#include "globals.h"
#include "events.h"
#include "utils.h"
#include "graphs.h"
#include "draw.h"
#include "graphutils.h"
#include "x11drv.h"
#include "plotone.h"
#include "noxprotos.h"
#include "MainWindow.h"
#include "allWidgets.h"
#include "editWidgets.h"
#include "undo_module.h"

extern MainWindow * mainWin;
extern QImage * MainPixmap;
extern frmSetAppearance * FormSetAppearance;
extern frmPlotAppearance * FormPlotAppearance;
extern frmAxisProp * FormAxisProperties;
extern frmGraphApp * FormGraphAppearance;
extern frmPointExplorer * FormPointExplorer;
extern frmLocatorProps * FormLocatorProps;
extern frmMasterRegionOperator * FormRegionMaster;
extern frmDrawObjects * FormDrawObjects;
extern void hide_rubber_lines(void);
extern int object_edit_popup(int obj, int id);
extern int region_def_under_way;
extern int regiontype;
extern QPoint VPoint2XPoint(VPoint vp);
extern unsigned int win_h, win_w;
#define win_scale ((win_h < win_w) ? win_h:win_w)

extern QStringList ListOfChanges;
extern QStringList ListOfOldStates;
extern int simple_draw_setting;
extern int print_target;

extern int donut_type;//0=donut, 1=pie-segment
extern double zoom_donut1,zoom_donut2;//for zooming in polar graph v2

#if QT_VERSION >= 0x050000
QTime lastc=QTime::currentTime();  /* time of last mouse click */
#endif
QTimer click_echo;
QRect ShiftRect;
QPoint ShiftPoint;
//QTime GeneralPurposeTimer2;
static VPoint anchor_vp = {0.0, 0.0};
static int x, y;                /* pointer coordinates */
int anchor_x = 0;
int anchor_y = 0;
static view bb;
static int move_dir;
static int add_at;
static int track_setno;
extern CanvasAction action_flag;
extern char DecimalPointToUse;
extern int cur_cursor;

/*
 * for region, area and perimeter computation
 */
#define MAX_POLY_POINTS 200
static int region_pts = 0;
static int iax[MAX_POLY_POINTS];
static int iay[MAX_POLY_POINTS];
static WPoint region_wps[MAX_POLY_POINTS];

using namespace std;

void UpdateCursor(VPoint vp);
extern void get_tracking_props(int *setno, int *move_dir, int *add_at);
extern void set_graph_selectors(int gno);
extern void do_hotupdate_proc(void);
extern void SetDecimalSeparatorToUserValue(char * str,bool remove_space=true);
extern void GeneralPaste(const QMimeData * mimeData);
extern void limit_viewport(int & posx,int & posy,int & dx,int & dy);
extern void limit_viewport(double & x1,double & y1,double & x2,double & y2);
extern void prepare_highlight_segments_with_sets(int n,int * gnos,int * snos,int do_clipping);
extern int clicked_in_legend(VPoint vp,int * gno,int * setno, view * bb);
extern void update_StdSelectors(int new_gno);
#ifdef __cplusplus
extern "C" {
#endif
extern double rint_2(double v);
#ifdef __cplusplus
}
#endif

//this is for simple use of the undo-stuff - to be used with single objects only!
int nrOfUndoObj=1;
int undoObjIds[2];
Datapoint * undo_dp=NULL;
int * undo_nrs=NULL;
region undo_region,undo_region2;
extern bool immediateUpdate;
extern bool updateRunning;
extern bool point_explorer_activ;
bool Pick_and_Move_active=false;
int pan_modifier=-1;
int current_object_id,current_object_type;//used during object movement
int current_axis_restriction=NO_AXIS;
int next_line_id=-1;

double getOrientationAngle(int type,int gno,int nr)//type=0=axis, type=1=object / nr=axis-nr or object-nr / gno=graph-nr or object-type for objects
{//returns angle in degrees
double ang=0.0;
double ab_v=0.0;
    if (type==0)//axis
    {
        if (is_valid_axis(gno,nr))
        {
            if (is_yaxis(nr))
            ang=(1.0-g[gno].t[nr]->label_layout)*90.0;
            else
            ang=g[gno].t[nr]->label_layout*90.0;
        }
    }
    else if (type==1)//(text-)object
    {
        switch (gno)
        {
        case OBJECT_NONE:
        break;
        case OBJECT_BOX:
            if (is_valid_box(nr))
            ang=boxes[nr].rot;
        break;
        case OBJECT_ELLIPSE:
            if (is_valid_ellipse(nr))
            ang=ellip[nr].rot;
        break;
        case OBJECT_LINE:
            if (is_valid_line(nr))
            {
            ab_v=pow((lines[nr].x2-lines[nr].x1)*(lines[nr].x2-lines[nr].x1)+(lines[nr].y2-lines[nr].y1)*(lines[nr].y2-lines[nr].y1),-0.5);
            ang=acos((lines[nr].x2-lines[nr].x1)*ab_v)*180.0/M_PI;
            while (ang>90.0) ang=180.0-ang;
                if (lines[nr].x2-lines[nr].x1>=0.0)
                {
                    if (lines[nr].y2-lines[nr].y1>=0.0)
                    {
                    ;//all well
                    }
                    else
                    {
                    ang=-ang;
                    }
                }
                else
                {
                    if (lines[nr].y2-lines[nr].y1>0.0)
                    {
                    ang=180.0-ang;
                    }
                    else
                    {
                    ang=180.0+ang;
                    }
                }
            }
        break;
        case OBJECT_STRING:
            if (is_valid_string(nr))
            ang=pstr[nr].rot;
        break;
        }
    }
return ang;
}

bool is_legend_movable(int attach)
{
    if (attach==(G_LB_ATTACH_LEFT | G_LB_ATTACH_TOP) || attach==(G_LB_ATTACH_RIGHT | G_LB_ATTACH_TOP) || attach==(G_LB_ATTACH_LEFT | G_LB_ATTACH_BOTTOM) || attach==(G_LB_ATTACH_RIGHT | G_LB_ATTACH_BOTTOM))
    return false;
    else
    return true;
}

void anchor_point(int curx, int cury, VPoint curvp)
{
     anchor_vp = curvp;
     anchor_x = curx;
     anchor_y = cury;
}

void ForceRedraw(int busy)
{
updateRunning=false;
//mainWin->mainArea->compl_redraw_running=false;
if (busy)
{
    mainWin->mainArea->completeRedraw();
}
else
{
    print_target=PRINT_TARGET_SCREEN;//this is the standard (is only changed in do_hardcopy())
    mainWin->mainArea->compl_redraw_running=true;
        update_all();
        drawgraph();
    mainWin->mainArea->compl_redraw_running=false;
}
//mainWin->mainArea->compl_redraw_running=true;
updateRunning=true;
}

void reactivate_Pick_and_Move(void)
{
Pick_and_Move_active=false;
set_action(DO_NOTHING);
set_action(PICK_MOVE);
}

void my_proc(special_XEvent *event)
{
static bool dbl_click;
static WPoint wp, wp_new, wp_new2, last_left_click_point_wp;
static VPoint vp, vp_start, last_left_click_point_vp;
static VVector shift;
static view v;
static world wtmp0,wtmp,wtmp2,*wtmp3=NULL;
static double vp_to_wx, vp_to_wy;
static int cg, newg, loc, shift_legend_graph, graph_last_left_click_in,mouse_not_up=0;
static char dummy[1024];
static int track_loc;
static int type, id;   /* for objects */
static int axisno,rot;
static int nr_of_sel_graphs,*sel_graphs=new int[2];
static Datapoint dpoint;
static GLocator locator;
static legend tmp_l;
static ClickTarget ct;
static Page_geometry cur_pg;
static int parameters[4],parameters2[5];
static QMouseEvent * me=NULL;
static QPointF localPos;
/*static special_XEvent alt_event;*/
bool old_upd;
old_upd=immediateUpdate;
immediateUpdate=false;
updateRunning=true;
current_axis_restriction=event->axis_restrict;
//GeneralPurposeTimer2.restart();
//my_proc_start:
    cg = get_cg();
    get_tracking_props(&track_setno, &move_dir, &add_at);
    switch (event->type)
    {
    case QEvent::MouseMove://MotionNotify:
	x = event->xmotion.x;
	y = event->xmotion.y;
    //for debugging
    //sprintf(dummy,"Pixel: ( %d | %d )",x,y);
    //set_left_footer(dummy);
    //end for debugging
    //qDebug() << "MouseMove: cursortype=" << cursortype;
        if (cursortype != 0)
        {
        crosshair_motion(x, y);
        }
        vp = xlibdev2VPoint(x, y);
            UpdateCursor(vp);
        getpoints(&vp);
        if (focus_policy == FOCUS_FOLLOWS)
        {
            if ((newg = next_graph_containing(-1, vp)) != cg)
            {
                simple_draw_setting|=SIMPLE_DRAW_GRAPH_FOCUS;
                switch_current_graph(newg);
                cg = newg;
            }
        }
        if (action_flag==DO_NOTHING && mouse_not_up==1 && (show_PickButton==2 || show_PanButton==2))
        {
        ///we have a pan-operation here
        //qDebug() << "MouseNotUp=" << mouse_not_up << " PressPosition=" << last_left_click_point_wp.x << "|" << last_left_click_point_wp.y;
        mainWin->mainArea->scroll->doEvent();
            //qApp->processEvents();
            //goto my_proc_start;
        ///Check for Drag-operation
        }
        switch (action_flag) {
        case DO_NOTHING:
        case VIEW_2ND:
        case CORNER_2ND:
        case ZOOM_2ND:
        case ZOOMX_2ND:
        case ZOOMY_2ND:
        case MAKE_BOX_2ND:
        case MAKE_ELLIP_2ND:
            ;/// select_region(anchor_x, anchor_y, x, y, 1);
            break;
        case MOVE_OBJECT_2ND:
        case COPY_OBJECT2ND:
        case PLACE_LEGEND_2ND:
        case PLACE_TIMESTAMP_2ND:
            ;/// slide_region(bb, x - anchor_x, y - anchor_y, 1);
            break;
        case DEF_REGION2ND:
            select_line(anchor_x, anchor_y, x, y, 1);
            simple_draw_setting|=SIMPLE_DRAW_LINE;
            mainWin->mainArea->completeRedraw();
            break;
        case MAKE_LINE_2ND:
                if (event->axis_restrict==X_AXIS)
                {
                select_line(anchor_x, anchor_y, x, anchor_y, 1);
                }
                else if(event->axis_restrict==Y_AXIS)
                {
                select_line(anchor_x, anchor_y, anchor_x, y, 1);
                }
                else
                {
                select_line(anchor_x, anchor_y, x, y, 1);
                }
            simple_draw_setting|=SIMPLE_DRAW_LINE;
            //mainWin->mainArea->completeRedraw();
            ForceRedraw(FALSE);
            break;
        case DEF_REGION:
            if (region_pts > 0)
            {
                anchor_point(x, y, vp);
                iax[region_pts] = x;
                iay[region_pts] = y;
                view2world(vp.x, vp.y,&region_wps[region_pts].x, &region_wps[region_pts].y);
                select_line(anchor_x, anchor_y, x, y, 1);
                rg[nr].n = region_pts+1;
                //rg[nr].n = region_pts;
                //XCFREE(rg[nr].x);
                //XCFREE(rg[nr].y);
                //rg[nr].x = (double*)xcalloc(region_pts, sizeof(double));//SIZEOF_DOUBLE);
                //rg[nr].y = (double*)xcalloc(region_pts, sizeof(double));//SIZEOF_DOUBLE);
                //    for (int iii = 0; iii < region_pts; iii++) {
                    rg[nr].x[region_pts] = region_wps[region_pts].x;
                    rg[nr].y[region_pts] = region_wps[region_pts].y;
                //}
                simple_draw_setting|=SIMPLE_DRAW_REGION;
                mainWin->mainArea->completeRedraw();
            }
            break;
        case MOVE_POINT2ND:
            switch (move_dir)
            {
            case MOVE_POINT_XY:
                select_line(anchor_x, anchor_y, x, y, 1);
                break;
            case MOVE_POINT_X:
                select_line(x, anchor_y, anchor_x, anchor_y, 1);
                break;
            case MOVE_POINT_Y:
                select_line(anchor_x, anchor_y, anchor_x, y, 1);
                break;
            }
            simple_draw_setting|=SIMPLE_DRAW_LINE;
            mainWin->mainArea->completeRedraw();
            break;
        case PAN_2ND:
            mainWin->getselectedgraphs(&nr_of_sel_graphs,&sel_graphs);
            for (int ll=0;ll<nr_of_sel_graphs;ll++)
            {
            //(void)get_graph_world(sel_graphs[ll],&wtmp3);
            /*wp_new2.x=(wtmp2.xg1+wtmp2.xg2)*0.5-(vp.x-vp_start.x)*vp_to_wx;
            wp_new2.y=(wtmp2.yg1+wtmp2.yg2)*0.5-(vp.y-vp_start.y)*vp_to_wy;*/
                /// the classic version
            /*wp_new2.x=(wtmp3[ll].xg1+wtmp3[ll].xg2)*0.5-(vp.x-vp_start.x)*vp_to_wx;
            wp_new2.y=(wtmp3[ll].yg1+wtmp3[ll].yg2)*0.5-(vp.y-vp_start.y)*vp_to_wy;*/
                /// works if not inverted
            /*wp_new2.x=(fscale(wtmp3[ll].xg1,get_graph_xscale(ll))+fscale(wtmp3[ll].xg2,get_graph_xscale(ll)))*0.5-(vp.x-vp_start.x)*vp_to_wx;
            wp_new2.y=(fscale(wtmp3[ll].yg1,get_graph_yscale(ll))+fscale(wtmp3[ll].yg2,get_graph_yscale(ll)))*0.5-(vp.y-vp_start.y)*vp_to_wy;*/

                wp_new2.x=(fscale2(wtmp3[ll].xg1,ll,X_AXIS)+fscale2(wtmp3[ll].xg2,ll,X_AXIS))*0.5-(vp.x-vp_start.x)*vp_to_wx*(1.0-2.0*is_graph_xinvert(ll));
                wp_new2.y=(fscale2(wtmp3[ll].yg1,ll,Y_AXIS)+fscale2(wtmp3[ll].yg2,ll,Y_AXIS))*0.5-(vp.y-vp_start.y)*vp_to_wy*(1.0-2.0*is_graph_yinvert(ll));

            if (pan_modifier==X_AXIS)
            {
            wp_new2.y=(fscale2(wtmp3[ll].yg1,ll,Y_AXIS)+fscale2(wtmp3[ll].yg2,ll,Y_AXIS))*0.5-(0.0)*vp_to_wy*(1.0-2.0*is_graph_yinvert(ll));
            }
            else if (pan_modifier==Y_AXIS)
            {
            wp_new2.x=(fscale2(wtmp3[ll].xg1,ll,X_AXIS)+fscale2(wtmp3[ll].xg2,ll,X_AXIS))*0.5-(0.0)*vp_to_wx*(1.0-2.0*is_graph_xinvert(ll));
            }
            //zoomGraphWorld(get_cg(),wtmp2,wp_new2,1.0);
            zoomGraphWorld(sel_graphs[ll],wtmp3[ll],wp_new2,1.0);
            }
            //mainWin->mainArea->completeRedraw();
            drawgraph();
            break;
        default:
            immediateUpdate=old_upd;
            updateRunning=false;
            return;
            break;
        }
        break;
    case QEvent::MouseButtonRelease:
        if (event->xbutton.button==Qt::LeftButton)
        {
            mouse_not_up=0;
        switch (action_flag)
        {
        case PAN_2ND://we do nothing actually here, we just do the undo-stuff
            mainWin->getselectedgraphs(&nr_of_sel_graphs,&sel_graphs);
            ListOfChanges.clear();
            ListOfOldStates.clear();
            for (int ll=0;ll<nr_of_sel_graphs;ll++)
            {
            get_graph_world(sel_graphs[ll],&wtmp);
            sprintf(dummy,"with G%d",sel_graphs[ll]);
            ListOfChanges << QString(dummy);
            ListOfOldStates << QString(dummy);
                sprintf(dummy,"    world %g, %g, %g, %g",wtmp3[ll].xg1,wtmp3[ll].yg1,wtmp3[ll].xg2,wtmp3[ll].yg2);
                ListOfOldStates << QString(dummy);
                sprintf(dummy,"    world %g, %g, %g, %g",wtmp.xg1,wtmp.yg1,wtmp.xg2,wtmp.yg2);
                ListOfChanges << QString(dummy);
            }
            TickmarksModified(0,3,nr_of_sel_graphs,sel_graphs);
            //SingleGraphTickmarksModified(cg);
            if (nr_of_sel_graphs==1)
            addAditionalDescriptionToLastNode(-1,QString("Graph world changed [G")+QString::number(cg)+QObject::tr("] (panned)"),QString(),-1);
            else
            addAditionalDescriptionToLastNode(-1,QString("Graph world changed (panned)"),QString(),-1);
            if (FormAxisProperties!=NULL)
            {
            FormAxisProperties->create_axes_dialog(FormAxisProperties->flp->curaxis);
            }
                /*if (Pick_and_Move_active==true)
                reactivate_Pick_and_Move();
                else
                set_action(PAN_1ST);*/
            set_action(DO_NOTHING);
            break;
        default:
            mainWin->mainArea->scroll->haltEvent();
            break;
        }
        }
        break;
    case QEvent::MouseButtonPress://ButtonPress:
        mainWin->getselectedgraphs(&nr_of_sel_graphs,&sel_graphs);
	x = event->xbutton.x;
	y = event->xbutton.y;
	vp = xlibdev2VPoint(x, y);
	getpoints(&vp);
//qDebug() << "A Button Press=" << GeneralPurposeTimer2.restart() << "ms";
        switch (event->xbutton.button)
        {
        case Qt::LeftButton://Button1:
        mouse_not_up=1;
        last_left_click_point_vp.x=vp.x;
        last_left_click_point_vp.y=vp.y;
        graph_last_left_click_in=next_graph_containing(get_cg(),last_left_click_point_vp);
        view2_graph_world(graph_last_left_click_in,last_left_click_point_vp.x,last_left_click_point_vp.y,&last_left_click_point_wp.x,&last_left_click_point_wp.y);
//qDebug() << "B Button Press=" << GeneralPurposeTimer2.restart() << "ms";
            /* first, determine if it's double click --> replaced by QT-double-click function in Qt4.7, but reverted in Qt5.0*/
/*#if QT_VERSION >= 0x050000
            if (lastc.restart() < CLICKINT)
            {
                dbl_click = TRUE;
            } else {
                dbl_click = FALSE;
            }
#else*/
            dbl_click=event->doubleClick;
//#endif
//qDebug() << "Is Double Click=" << dbl_click << " timer_running=" << mainWin->mainArea->scroll->event_timer.isActive() << " timer=" << GeneralPurposeTimer2.restart() << "ms";
            switch (action_flag)
            {
            case DO_NOTHING:
/*qDebug() << "DoubleClick=" << dbl_click << "allow_dc=" << allow_dc;
qDebug() << "DoubleClick.find_point=" << find_point(cg, vp, &track_setno, &loc);
qDebug() << "Found:" << track_setno << "/" << loc;
qDebug() << "DoubleClick.axis:clicked=" << axis_clicked(cg, vp, &axisno);
qDebug() << "axisno=" << axisno;*/
                if (dbl_click == TRUE && allow_dc == TRUE)/// Double-Click
                {
                mainWin->mainArea->scroll->haltEvent();

                    track_setno = -1;
                    if (focus_clicked(cg, vp, &anchor_vp) == TRUE)
                    {
                        xlibVPoint2dev(anchor_vp, &anchor_x, &anchor_y);
                        set_action(VIEW_2ND);
                        get_graph_viewport(cg,&bb);
                        ShiftRect=QRect(xconvxlib(bb.xv1),yconvxlib(bb.yv2),xconvxlib(bb.xv2)-xconvxlib(bb.xv1),yconvxlib(bb.yv1)-yconvxlib(bb.yv2));
                        ShiftPoint=QPoint(x,y);
                        mainWin->mainArea->repositionRubber(ShiftRect);
                        mainWin->mainArea->rubber->show();
                    }
                    else if (lower_right_corner_clicked(vp) == TRUE)
                    {
                        bb.xv1=bb.xv2=bb.yv1=bb.yv2=0.0;//clear existing bounding box data
                        get_page_viewport(&bb.xv2,&bb.yv1);//vx2,yv1 is the lower right corner in viewport coordinates
                        anchor_vp.x=bb.xv2;
                        anchor_vp.y=bb.yv1;
                        set_action(CORNER_2ND);
                        ShiftRect=QRect(xconvxlib(bb.xv1),yconvxlib(bb.yv2),xconvxlib(bb.xv2)-xconvxlib(bb.xv1),yconvxlib(bb.yv1)-yconvxlib(bb.yv2));
                        ShiftPoint=QPoint(x,y);
                        mainWin->mainArea->repositionRubber(ShiftRect);
                        mainWin->mainArea->rubber->show();
                    }
                    else if (find_point(cg, vp, &track_setno, &loc) == RETURN_SUCCESS)
                    {
                        if (FormSetAppearance==NULL)
                        {
                        FormSetAppearance=new frmSetAppearance(mainWin);
                        FormSetAppearance->init();
                        }
                        FormSetAppearance->show();
                        FormSetAppearance->ShowSetData_external(cg,track_setno);
                        FormSetAppearance->raise();
                        FormSetAppearance->activateWindow();
                    }
                    else if (axis_clicked(cg, vp, &axisno) == TRUE)
                    {
                        if (FormAxisProperties==NULL)
                        {
                        FormAxisProperties=new frmAxisProp(mainWin);
                        }
                        FormAxisProperties->show();
                        FormAxisProperties->raise();
                        FormAxisProperties->create_axes_dialog(axisno);
                        FormAxisProperties->flp->tabs->setCurrentIndex(0);
                    }
                    else if (axis_custom_tick_clicked(cg,vp,&axisno,&newg) == TRUE)//only works with custom ticks!
                    {
                        if (inline_editing==true && !event->ctrl)
                        {
                        //qDebug() << "Graph=" << cg << " Axis=" << axisno << " TickNr=" << newg;
                        mainWin->mainArea->inlineedit2->init_and_show_editor(4,cg,axisno,newg);
                        }
                        else
                        {
                            if (FormAxisProperties==NULL)
                            {
                            FormAxisProperties=new frmAxisProp(mainWin);
                            }
                            FormAxisProperties->show();
                            FormAxisProperties->raise();
                            FormAxisProperties->create_axes_dialog(axisno);
                            FormAxisProperties->flp->tabs->setCurrentIndex(4);
                        }
                    }
                    else if (axis_label_clicked(&newg, vp, &axisno, &loc) == TRUE)
                    {
                        if (inline_editing==true && !event->ctrl)
                        {
                        mainWin->mainArea->inlineedit2->init_and_show_editor(2,newg,axisno,loc);
                        }
                        else
                        {
                            if (FormAxisProperties==NULL)
                            {
                            FormAxisProperties=new frmAxisProp(mainWin);
                            }
                            FormAxisProperties->show();
                            FormAxisProperties->raise();
                            FormAxisProperties->create_axes_dialog(axisno);
                            FormAxisProperties->flp->tabs->setCurrentIndex(0);
                        }
                    }
                    else if (title_clicked(cg, vp, &add_at) == TRUE)
                    {
                        if (inline_editing==true && !event->ctrl)
                        {
                        mainWin->mainArea->inlineedit2->init_and_show_editor(1,cg,add_at);
                        }
                        else
                        {
                        if (FormGraphAppearance==NULL)
                        {
                        FormGraphAppearance=new frmGraphApp(mainWin);
                        }
                        FormGraphAppearance->init();
                        FormGraphAppearance->show_graph_data_external(cg);
                        FormGraphAppearance->show();
                        FormGraphAppearance->raise();
                        FormGraphAppearance->activateWindow();
                        FormGraphAppearance->flp->tabs->setCurrentIndex(0);
                            if (add_at==1)//subtitle
                            FormGraphAppearance->flp->tabMain->ledSubtitle->lenText->setFocus();
                            else
                            FormGraphAppearance->flp->tabMain->ledTitle->lenText->setFocus();
                        }
                    }
                    ///else if (legend_clicked(cg, vp, &bb) == TRUE)
                    else if (clicked_in_legend(vp,&newg,&axisno,&bb)==RETURN_SUCCESS)
                    {
                        if (inline_editing==true && !event->ctrl)
                        {
                        mainWin->mainArea->inlineedit2->init_and_show_editor(3,newg,axisno);
                        }
                        else
                        {
                        if (FormGraphAppearance==NULL)
                        {
                        FormGraphAppearance=new frmGraphApp(mainWin);
                        }
                        FormGraphAppearance->init();
                        FormGraphAppearance->show_graph_data_external(cg);//if cg!=newg --> switching graph?
                        FormGraphAppearance->show();
                        FormGraphAppearance->raise();
                        FormGraphAppearance->flp->tabs->setCurrentIndex(3);
                        }
                    }
                    else if (find_item(cg, vp, &bb, &type, &id) == RETURN_SUCCESS)
                    {
                    //qDebug() << "Angle=" << getOrientationAngle(1,type,id);
                        if (type==OBJECT_STRING && inline_editing==1 && !event->ctrl)
                        {
                        mainWin->mainArea->inlineedit2->init_and_show_editor(0,cg,id);
                        }
                        else
                        {
                        object_edit_popup(type, id);
                        }
                    }
                    else if (timestamp_clicked(vp, &bb) == TRUE)
                    {
                        if (FormPlotAppearance==NULL)
                        {
                        FormPlotAppearance=new frmPlotAppearance(mainWin);
                        }
                        FormPlotAppearance->init();
                        FormPlotAppearance->show();
                        FormPlotAppearance->raise();
                    }
                    else if (graph_clicked(cg, vp) == TRUE)
                    {
                        if (FormSetAppearance==NULL)
                        {
                        FormSetAppearance=new frmSetAppearance(mainWin);
                        }
                        FormSetAppearance->init();
                        FormSetAppearance->show();
                        FormSetAppearance->raise();
                    }
                    else
                    {
                    newg = find_nearest_graph(vp);
                        if (focus_policy == FOCUS_CLICK && newg != cg)
                        {
                        updateRunning=false;
                        mainWin->mainArea->compl_redraw_running=false;
                        simple_draw_setting|=SIMPLE_DRAW_GRAPH_FOCUS;
                        switch_current_graph(newg);
                        updateRunning=true;
                        }
                        if (FormGraphAppearance==NULL)
                        {
                        FormGraphAppearance=new frmGraphApp(mainWin);
                        }
                        FormGraphAppearance->init();
                        FormGraphAppearance->show();
                        FormGraphAppearance->raise();
                    }
                }
                else//no double click- but single click
                {
                    if (show_PickButton==2)//pick-and-move always on
                    {
                    mainWin->mainArea->scroll->startEvent(PICK_MOVE,*event,vp);
                    }
                    else if (show_PanButton==2)//pan always on
                    {
                    mainWin->mainArea->scroll->startEvent(PAN_1ST,*event,vp);
                    }
                    //else
                    //{
                    track_setno = -1;
                    //qDebug() << "C Button Press=" << GeneralPurposeTimer2.restart() << "ms";
                    if (FormPointExplorer!=NULL)//if the point-explorer is not active --> do not use this because it costs a lot of time
                    if (FormPointExplorer->isVisible())
                    {
                    find_point(cg, vp, &track_setno, &loc);/// added in order to automatically update the point-explorer-dialog
                    }
                    //qDebug() << "D Button Press=" << GeneralPurposeTimer2.restart() << "ms";
                        if (focus_policy == FOCUS_CLICK)
                        {
                            if ((newg = next_graph_containing(cg, vp)) != cg)
                            {
                            updateRunning=false;
                            mainWin->mainArea->compl_redraw_running=false;
                            simple_draw_setting|=SIMPLE_DRAW_GRAPH_FOCUS;
                            switch_current_graph(newg);
                            updateRunning=true;
                            }
                        }
                    //}
                }
                break;
            case PICK_MOVE:
                //qDebug() << "Pick-Move active";
                ct=getClickTarget(vp,&bb,parameters);
                //debugClickTarget(ct,bb,parameters);
                switch(ct)
                {
                case TARGET_AXIS:
                //qDebug() << "moving Graph now";
                    newg=parameters[0];
                    ShiftRect=QRect(xconvxlib(bb.xv1),yconvxlib(bb.yv2),xconvxlib(bb.xv2)-xconvxlib(bb.xv1),yconvxlib(bb.yv1)-yconvxlib(bb.yv2));
                    ShiftPoint=QPoint(x,y);
                    //mainWin->mainArea->rubber->setGeometry(ShiftRect);
                    mainWin->mainArea->repositionRubber(ShiftRect);
                    mainWin->mainArea->rubber->show();
                    anchor_point(x, y, vp);
                    set_action(MOVE_GRAPH_2ND);
                    break;
                case TARGET_AXIS_LABEL:
                //qDebug() << "moving Axis label now";
                    newg=parameters[0];
                    axisno=loc=parameters[1];
                        ShiftRect=QRect(xconvxlib(bb.xv1),yconvxlib(bb.yv2),xconvxlib(bb.xv2)-xconvxlib(bb.xv1),yconvxlib(bb.yv1)-yconvxlib(bb.yv2));
                        ShiftPoint=QPoint(x,y);
                        //mainWin->mainArea->rubber->setGeometry(ShiftRect);
                        mainWin->mainArea->repositionRubber(ShiftRect);
                        mainWin->mainArea->rubber->show();
                        anchor_point(x, y, vp);
                        set_action(MOVE_AXIS_LABEL_2ND);
                    break;
                case TARGET_TITLE:
                //qDebug() << "moving Title now";
                    newg=parameters[0];
                    loc=parameters[1];
                    if (loc==0)
                    bb=g[newg].labs.title.bb;
                    else
                    bb=g[newg].labs.stitle.bb;
                //qDebug() << "gno=" << newg << " loc=" << loc << " bb=" << bb.xv1 << "," << bb.yv1 << "|" << bb.xv2 << "," << bb.yv2;
                        ShiftRect=QRect(xconvxlib(bb.xv1),yconvxlib(bb.yv2),xconvxlib(bb.xv2)-xconvxlib(bb.xv1),yconvxlib(bb.yv1)-yconvxlib(bb.yv2));
                        ShiftPoint=QPoint(x,y);
                        //mainWin->mainArea->rubber->setGeometry(ShiftRect);
                        mainWin->mainArea->repositionRubber(ShiftRect);
                        mainWin->mainArea->rubber->show();
                        anchor_point(x, y, vp);
                    set_action(MOVE_TITLE_2ND);
                    break;
                case TARGET_LEGEND:
                //qDebug() << "moving Legend now";
                legend_clicked(parameters[0],vp,&bb);
                shift_legend_graph=parameters[0];
                ShiftRect=QRect(xconvxlib(bb.xv1),yconvxlib(bb.yv2),xconvxlib(bb.xv2)-xconvxlib(bb.xv1),yconvxlib(bb.yv1)-yconvxlib(bb.yv2));
                ShiftPoint=QPoint(x,y);
                //mainWin->mainArea->rubber->setGeometry(ShiftRect);
                mainWin->mainArea->repositionRubber(ShiftRect);
                mainWin->mainArea->rubber->show();
                    anchor_point(x, y, vp);
                    /// slide_region(bb, x - anchor_x, y - anchor_y, 0);
                    set_action(PLACE_LEGEND_2ND);
                    break;
                case TARGET_OBJECT:
                ///qDebug() << "moving Object now";
                    type=parameters[0];
                    id  =parameters[1];
                ShiftRect=QRect(xconvxlib(bb.xv1),yconvxlib(bb.yv2),xconvxlib(bb.xv2)-xconvxlib(bb.xv1),yconvxlib(bb.yv1)-yconvxlib(bb.yv2));
                ShiftPoint=QPoint(x,y);
                //mainWin->mainArea->rubber->setGeometry(ShiftRect);
                mainWin->mainArea->repositionRubber(ShiftRect);
                mainWin->mainArea->rubber->show();
                anchor_point(x, y, vp);
                set_action(MOVE_OBJECT_2ND);
                    break;
                case TARGET_TIMESTAMP:
                //qDebug() << "moving Timestamp now";
                ShiftRect=QRect(xconvxlib(bb.xv1),yconvxlib(bb.yv2),xconvxlib(bb.xv2)-xconvxlib(bb.xv1),yconvxlib(bb.yv1)-yconvxlib(bb.yv2));
                ShiftPoint=QPoint(x,y);
                //mainWin->mainArea->rubber->setGeometry(ShiftRect);
                mainWin->mainArea->repositionRubber(ShiftRect);
                mainWin->mainArea->rubber->show();
                anchor_point(x, y, vp);
                /// slide_region(bb, x - anchor_x, y - anchor_y, 0);
                set_action(PLACE_TIMESTAMP_2ND);
                    break;
                case TARGET_GRAPH:
                case TARGET_SET:
                    pan_modifier=-1;
                        if (event->ctrl==true)
                        pan_modifier=Y_AXIS;
                        else if (event->alt==true)
                        pan_modifier=X_AXIS;
                //qDebug() << "Point/Graph clicked --> activate Pan 2nd";
                    mainWin->getselectedgraphs(&nr_of_sel_graphs,&sel_graphs);
                    if (wtmp3!=NULL) delete[] wtmp3;
                    wtmp3=new world[2+nr_of_sel_graphs];
                    for (int ll=0;ll<nr_of_sel_graphs;ll++)
                    {
                    (void)get_graph_world(sel_graphs[ll],wtmp3+ll);
                    }
                    SaveTickmarksStatesPrevious(0,3,nr_of_sel_graphs,sel_graphs);
                    //SaveSingleGraphTickmarks(cg);
                        get_graph_viewport(cg, &v);
                        get_graph_world(cg,&wtmp2);
                        /*vp_to_wx=(wtmp2.xg2-wtmp2.xg1)/(v.xv2-v.xv1);
                        vp_to_wy=(wtmp2.yg2-wtmp2.yg1)/(v.yv2-v.yv1);*/
                        vp_to_wx=(fscale2(wtmp2.xg2,cg,X_AXIS)-fscale2(wtmp2.xg1,cg,X_AXIS))/(v.xv2-v.xv1);
                        vp_to_wy=(fscale2(wtmp2.yg2,cg,Y_AXIS)-fscale2(wtmp2.yg1,cg,Y_AXIS))/(v.yv2-v.yv1);
                        vp_start=vp;
                    set_action(PAN_2ND);
                default:
                    break;
                }
                break;
            case PAN_1ST:
            //qDebug() << "Left Button Press PAN_1ST";
                pan_modifier=-1;
                    if (event->ctrl==true)
                    pan_modifier=Y_AXIS;
                    else if (event->alt==true)
                    pan_modifier=X_AXIS;
                if ((newg = next_graph_containing(cg, vp)) != cg)//switch graph if necessary
                {
                updateRunning=false;
                mainWin->mainArea->compl_redraw_running=false;
                simple_draw_setting|=SIMPLE_DRAW_GRAPH_FOCUS;
                switch_current_graph(newg);
                updateRunning=true;
                cg=get_cg();
                }
                mainWin->getselectedgraphs(&nr_of_sel_graphs,&sel_graphs);
                if (wtmp3!=NULL) delete[] wtmp3;
                wtmp3=new world[2+nr_of_sel_graphs];
                for (int ll=0;ll<nr_of_sel_graphs;ll++)
                {
                (void)get_graph_world(sel_graphs[ll],wtmp3+ll);
                }
                SaveTickmarksStatesPrevious(0,3,nr_of_sel_graphs,sel_graphs);
                //SaveSingleGraphTickmarks(cg);
                    get_graph_viewport(cg, &v);//we make a reference to the currently selected graph here and all other selected graphs will follow
                    get_graph_world(cg,&wtmp2);
                    vp_to_wx=(fscale2(wtmp2.xg2,cg,X_AXIS)-fscale2(wtmp2.xg1,cg,X_AXIS))/(v.xv2-v.xv1);
                    vp_to_wy=(fscale2(wtmp2.yg2,cg,Y_AXIS)-fscale2(wtmp2.yg1,cg,Y_AXIS))/(v.yv2-v.yv1);
                    vp_start=vp;
                set_action(PAN_2ND);
                break;
            case PAN_2ND:
            //qDebug() << "Left Button Press PAN_2ND";
                    if (Pick_and_Move_active==true)
                    reactivate_Pick_and_Move();
                    else
                    set_action(PAN_1ST);
                break;
            case MOVE_GRAPH_1ST:
                if (axis_clicked(cg,vp,&loc)==TRUE)
                {
                newg=cg;
                get_graph_viewport(cg,&bb);
//qDebug() << bb.xv1 << " - " << bb.xv2 << " | " << bb.yv1 << " x " << bb.yv2;
                ShiftRect=QRect(xconvxlib(bb.xv1),yconvxlib(bb.yv2),xconvxlib(bb.xv2)-xconvxlib(bb.xv1),yconvxlib(bb.yv1)-yconvxlib(bb.yv2));
                ShiftPoint=QPoint(x,y);
                //mainWin->mainArea->rubber->setGeometry(ShiftRect);
//qDebug() << "Shift Rect=" << ShiftRect;
                mainWin->mainArea->repositionRubber(ShiftRect);
                mainWin->mainArea->rubber->show();
                anchor_point(x, y, vp);
                set_action(MOVE_GRAPH_2ND);
                }
                break;
            case MOVE_GRAPH_2ND:
                shift.x = vp.x - anchor_vp.x;
                shift.y = vp.y - anchor_vp.y;

                if (current_axis_restriction==X_AXIS)
                {
                shift.y=0.0;
                }
                else if (current_axis_restriction==Y_AXIS)
                {
                shift.x=0.0;
                }

                move_graph_vp(newg,shift);
                mainWin->mainArea->rubber->hide();
                    if (Pick_and_Move_active==true)
                    reactivate_Pick_and_Move();
                    else
                    set_action(MOVE_GRAPH_1ST);
                break;
            case MOVE_TITLE_1ST:
                if (title_clicked(cg,vp,&loc)==TRUE)
                {
                newg=cg;
                    if (loc==0)
                    bb=g[newg].labs.title.bb;
                    else
                    bb=g[newg].labs.stitle.bb;
                ShiftRect=QRect(xconvxlib(bb.xv1),yconvxlib(bb.yv2),xconvxlib(bb.xv2)-xconvxlib(bb.xv1),yconvxlib(bb.yv1)-yconvxlib(bb.yv2));
                ShiftPoint=QPoint(x,y);
                //mainWin->mainArea->rubber->setGeometry(ShiftRect);
                mainWin->mainArea->repositionRubber(ShiftRect);
                mainWin->mainArea->rubber->show();
                anchor_point(x, y, vp);
                set_action(MOVE_TITLE_2ND);
                }
                break;
            case MOVE_TITLE_2ND:
                shift.x = vp.x - anchor_vp.x;
                shift.y = vp.y - anchor_vp.y;

                if (current_axis_restriction==X_AXIS)
                {
                shift.y=0.0;
                }
                else if (current_axis_restriction==Y_AXIS)
                {
                shift.x=0.0;
                }

                move_graph_title(newg,loc,shift);
                mainWin->mainArea->rubber->hide();
                    if (Pick_and_Move_active==true)
                    reactivate_Pick_and_Move();
                    else
                    set_action(MOVE_TITLE_1ST);
                break;
            case MOVE_AXIS_LABEL_1ST:
                if (axis_label_clicked(&newg, vp, &axisno, &loc) == TRUE)
                {
                    get_axis_label_viewport(newg,axisno,&bb,false,&rot,loc);
                    //bb=g[newg].t[axisno]->al_bb;
                    ShiftRect=QRect(xconvxlib(bb.xv1),yconvxlib(bb.yv2),xconvxlib(bb.xv2)-xconvxlib(bb.xv1),yconvxlib(bb.yv1)-yconvxlib(bb.yv2));
                    ShiftPoint=QPoint(x,y);
                    //mainWin->mainArea->rubber->setGeometry(ShiftRect);
                    mainWin->mainArea->repositionRubber(ShiftRect);
                    mainWin->mainArea->rubber->show();
                    anchor_point(x, y, vp);
                //slide_region(bb, x - anchor_x, y - anchor_y, 0);
                    set_action(MOVE_AXIS_LABEL_2ND);
                }
                break;
            case MOVE_AXIS_LABEL_2ND:
                shift.x = vp.x - anchor_vp.x;
                shift.y = vp.y - anchor_vp.y;

                if (current_axis_restriction==X_AXIS)
                {
                shift.y=0.0;
                }
                else if (current_axis_restriction==Y_AXIS)
                {
                shift.x=0.0;
                }

                move_axis_label(newg,axisno,shift);
                mainWin->mainArea->rubber->hide();
                    if (Pick_and_Move_active==true)
                    reactivate_Pick_and_Move();
                    else
                    set_action(MOVE_AXIS_LABEL_1ST);
                break;
            case VIEW_2ND:
                set_action(DO_NOTHING);
                v.xv1 = MIN2(vp.x, anchor_vp.x);
                v.yv1 = MIN2(vp.y, anchor_vp.y);
                v.xv2 = MAX2(vp.x, anchor_vp.x);
                v.yv2 = MAX2(vp.y, anchor_vp.y);
                limit_viewport(v.xv1,v.yv1,v.xv2,v.yv2);
                    ///Undo-Stuff
                    ViewportChanged(cg,v);
                set_graph_viewport(cg, v);
                    if (FormGraphAppearance!=NULL)
                    FormGraphAppearance->update_view(cg);
                mainWin->mainArea->rubber->hide();
                force_redraw();
                break;
            case CORNER_2ND:
                set_action(DO_NOTHING);
                //qDebug() << "Corner 2ND finished";
                SaveDeviceState(DEVICE_TERM,true);
                cur_pg=get_page_geometry();
                /*qDebug() << "Anchor= " << anchor_x << " | " << anchor_y;
                qDebug() << "BoxEnd= " << mainWin->mainArea->box_end_x << " | " << mainWin->mainArea->box_end_y;
                qDebug() << "MainPixmap: " << MainPixmap->width() << " x " << MainPixmap->height();
                qDebug() << "MainArea:   " << mainWin->mainArea->width() << " x " << mainWin->mainArea->height();*/
                ListOfChanges.clear();
                ListOfOldStates.clear();

                sprintf(dummy,"@page size %ld, %ld",cur_pg.width,cur_pg.height);
                ListOfOldStates << QString(dummy);
                vp_to_wx=(double)cur_pg.width;
                vp_to_wx*=((double)mainWin->mainArea->box_end_x)/((double)MainPixmap->width());
                vp_to_wy=(double)cur_pg.height;
                vp_to_wy*=((double)mainWin->mainArea->box_end_y)/((double)MainPixmap->height());
                cur_pg.width=rint_2(vp_to_wx);
                cur_pg.height=rint_2(vp_to_wy);
                sprintf(dummy,"@page size %ld, %ld",cur_pg.width,cur_pg.height);
                ListOfChanges << QString(dummy);
                set_page_geometry(cur_pg);
                //undo-stuff
                DeviceModified(DEVICE_TERM,true);
                mainWin->mainArea->rubber->hide();
                //mainWin->mainArea->completeRedraw();
                force_redraw();
                break;
            case ZOOM_1ST:
                anchor_point(x, y, vp);
                set_action(ZOOM_2ND);
            /// select_region(anchor_x, anchor_y, x, y, 0);
                break;
            case ZOOMX_1ST:
                anchor_point(x, y, vp);
                set_action(ZOOMX_2ND);
            /// select_region(anchor_x, anchor_y, x, y, 0);
                break;
            case ZOOMY_1ST:
                anchor_point(x, y, vp);
                set_action(ZOOMY_2ND);
            /// select_region(anchor_x, anchor_y, x, y, 0);
                break;
            case VIEW_1ST:
                anchor_point(x, y, vp);
                set_action(VIEW_2ND);
            /// select_region(anchor_x, anchor_y, x, y, 0);
                break;
            case ZOOM_2ND:
                set_action(DO_NOTHING);
                if (nr_of_sel_graphs<=0) break;
                parameters2[4]=1+ZY_AXIS;
                    for (int ll=0;ll<=ZY_AXIS;ll++)
                    parameters2[ll]=ll;
                SaveTickmarksStatesPrevious(parameters2[4],parameters2,nr_of_sel_graphs,sel_graphs);
                get_graph_world(get_cg(), &wtmp);//get unzoomed axes scale
                    if (get_graph_type(get_cg())==GRAPH_POLAR2)
                    {
                    wtmp2=wtmp;
                        if (donut_type==0)//donut --> R-scaling
                        {
                        wtmp2.yg1=zoom_donut1;
                        wtmp2.yg2=zoom_donut2;
                        }
                        else//pie-slice --> Phi-scaling
                        {
                        wtmp2.xg1=zoom_donut1;
                        wtmp2.xg2=zoom_donut2;
                        }
//qDebug() << "x1=" << wtmp.xg1/M_PI*180.0 << " x2=" << wtmp.xg2/M_PI*180.0 << " y1=" << wtmp.yg1 << " y2=" << wtmp.yg2;
                    set_graph_world(get_cg(),wtmp2);
                    }
                    else
                    {
                    newworld(get_cg(), ALL_AXES, anchor_vp, vp);//zoom all axes of current graph only
                    get_graph_world(get_cg(), &wtmp2);//save zoomed axes scale
                    }
                ListOfChanges.clear();
                ListOfOldStates.clear();
                for (int ll=0;ll<nr_of_sel_graphs;ll++)
                {
                cg=sel_graphs[ll];//get current graph-number
                        if (cg==get_cg())
                        wtmp0=wtmp;
                        else
                        get_graph_world(cg, &wtmp2);//get old axis-settings
                    sprintf(dummy,"with G%d",cg);
                    ListOfChanges << QString(dummy);
                    ListOfOldStates << QString(dummy);
                    sprintf(dummy,"    world %g, %g, %g, %g",wtmp0.xg1,wtmp0.yg1,wtmp0.xg2,wtmp0.yg2);
                    ListOfOldStates << QString(dummy);
                    wtmp0.xg1 = wtmp2.xg1;//all shall be zoomed --> copy all axes from current graph
                    wtmp0.xg2 = wtmp2.xg2;
                    wtmp0.yg1 = wtmp2.yg1;
                    wtmp0.yg2 = wtmp2.yg2;
                //newworld(cg, ALL_Y_AXES, anchor_vp, vp);
                set_graph_world(cg, wtmp0);//set zoom values of all graphs
                autotick_axis(cg, ALL_AXES);
                    sprintf(dummy,"    world %g, %g, %g, %g",wtmp0.xg1,wtmp0.yg1,wtmp0.xg2,wtmp0.yg2);
                    ListOfChanges << QString(dummy);
                }
                TickmarksModified(parameters2[4],parameters2,nr_of_sel_graphs,sel_graphs);
                    if (nr_of_sel_graphs==1)
                    {
                    sprintf(dummy,"[G%d]",cg);
                    addAditionalDescriptionToLastNode(-1,QObject::tr("Zoom graph ")+QString(dummy),QString(),0);
                    }
                    else
                    {
                    addAditionalDescriptionToLastNode(-1,QObject::tr("Zoom graphs"),QString(),1);
                    }
                force_redraw();
                break;
            case ZOOMX_2ND:
                set_action(DO_NOTHING);
                if (nr_of_sel_graphs<=0) break;
                parameters2[4]=2;
                parameters2[0]=X_AXIS;
                parameters2[1]=ZX_AXIS;
                SaveTickmarksStatesPrevious(parameters2[4],parameters2,nr_of_sel_graphs,sel_graphs);
                get_graph_world(get_cg(), &wtmp);//get unzoomed axes scale of current graph
                    if (get_graph_type(get_cg())==GRAPH_POLAR2)
                    {
                        wtmp2=wtmp;//wtmp2 will be the new axes scale of the current graph
                        //pie-slice --> Phi-scaling
                        wtmp2.xg1=zoom_donut1;
                        wtmp2.xg2=zoom_donut2;
//qDebug() << "x1=" << wtmp.xg1/M_PI*180.0 << " x2=" << wtmp.xg2/M_PI*180.0 << " y1=" << wtmp.yg1 << " y2=" << wtmp.yg2;
                    set_graph_world(get_cg(),wtmp2);
                    }
                    else
                    {
                    newworld(get_cg(), ALL_X_AXES, anchor_vp, vp);//zoom all axes of current graph only
                    get_graph_world(get_cg(), &wtmp2);
                    }
                ListOfChanges.clear();
                ListOfOldStates.clear();
                for (int ll=0;ll<nr_of_sel_graphs;ll++)
                {
                cg=sel_graphs[ll];
                        if (cg==get_cg())//the current graph has already been changed
                        wtmp0=wtmp;
                        else
                        get_graph_world(cg, &wtmp0);
                    sprintf(dummy,"with G%d",cg);
                    ListOfChanges << QString(dummy);
                    ListOfOldStates << QString(dummy);
                    sprintf(dummy,"    world %g, %g, %g, %g",wtmp0.xg1,wtmp0.yg1,wtmp0.xg2,wtmp0.yg2);//old state
                    ListOfOldStates << QString(dummy);
                wtmp0.xg1 = wtmp2.xg1;//x shall be zoomed --> copy x from current graph --> all selected graphs will have the same x-range afterwards
                wtmp0.xg2 = wtmp2.xg2;
                //wtmp2.yg1 = wtmp.yg1;
                //wtmp2.yg2 = wtmp.yg2;
                //newworld(cg, ALL_Y_AXES, anchor_vp, vp);
                set_graph_world(cg, wtmp0);//set zoom values of all graphs
                autotick_axis(cg, ALL_X_AXES);
                    sprintf(dummy,"    world %g, %g, %g, %g",wtmp0.xg1,wtmp0.yg1,wtmp0.xg2,wtmp0.yg2);//new states
                    ListOfChanges << QString(dummy);
                }
                TickmarksModified(parameters2[4],parameters2,nr_of_sel_graphs,sel_graphs);
                    if (nr_of_sel_graphs==1)
                    {
                    sprintf(dummy,"[G%d]",cg);
                    addAditionalDescriptionToLastNode(-1,QObject::tr("Zoom X-axes of ")+QString(dummy),QString(),0);
                    }
                    else
                    {
                    addAditionalDescriptionToLastNode(-1,QObject::tr("Zoom X-axes of graphs")+QString(dummy),QString(),1);
                    }
                force_redraw();
                break;
            case ZOOMY_2ND:
                set_action(DO_NOTHING);
                if (nr_of_sel_graphs<=0) break;
                parameters2[4]=2;
                parameters2[0]=Y_AXIS;
                parameters2[1]=ZY_AXIS;
                SaveTickmarksStatesPrevious(parameters2[4],parameters2,nr_of_sel_graphs,sel_graphs);
                get_graph_world(get_cg(), &wtmp);//get unzoomed axes scale
                    if (get_graph_type(get_cg())==GRAPH_POLAR2)
                    {
                        wtmp2=wtmp;
                        //donut --> R-scaling
                        wtmp2.yg1=zoom_donut1;
                        wtmp2.yg2=zoom_donut2;
//qDebug() << "x1=" << wtmp.xg1/M_PI*180.0 << " x2=" << wtmp.xg2/M_PI*180.0 << " y1=" << wtmp.yg1 << " y2=" << wtmp.yg2;
                    set_graph_world(get_cg(),wtmp2);
                    }
                    else
                    {
                    newworld(get_cg(), ALL_Y_AXES, anchor_vp, vp);//zoom all axes of current graph only
                    get_graph_world(get_cg(), &wtmp2);//save zoomed axes scale
                    }
                ListOfChanges.clear();
                ListOfOldStates.clear();
                for (int ll=0;ll<nr_of_sel_graphs;ll++)
                {
                cg=sel_graphs[ll];
                        if (cg==get_cg())
                        wtmp0=wtmp;
                        else
                        get_graph_world(cg, &wtmp0);
                    sprintf(dummy,"with G%d",cg);
                    ListOfChanges << QString(dummy);
                    ListOfOldStates << QString(dummy);
                    sprintf(dummy,"    world %g, %g, %g, %g",wtmp0.xg1,wtmp0.yg1,wtmp0.xg2,wtmp0.yg2);
                    ListOfOldStates << QString(dummy);
                //wtmp2.xg1 = wtmp.xg1;
                //wtmp2.xg2 = wtmp.xg2;
                wtmp0.yg1 = wtmp2.yg1;//y shall be zoomed --> copy y from current graph
                wtmp0.yg2 = wtmp2.yg2;
                //newworld(cg, ALL_Y_AXES, anchor_vp, vp);
                set_graph_world(cg, wtmp0);//set zoom values of all graphs
                autotick_axis(cg, ALL_Y_AXES);
                    sprintf(dummy,"    world %g, %g, %g, %g",wtmp0.xg1,wtmp0.yg1,wtmp0.xg2,wtmp0.yg2);
                    ListOfChanges << QString(dummy);
                }
                TickmarksModified(parameters2[4],parameters2,nr_of_sel_graphs,sel_graphs);
                    if (nr_of_sel_graphs==1)
                    {
                    sprintf(dummy,"[G%d]",cg);
                    addAditionalDescriptionToLastNode(-1,QObject::tr("Zoom Y-axes of ")+QString(dummy),QString(),0);
                    }
                    else
                    {
                    addAditionalDescriptionToLastNode(-1,QObject::tr("Zoom Y-axes of graphs")+QString(dummy),QString(),1);
                    }
                force_redraw();
                break;
            case EDIT_OBJECT:
                if (find_item(cg, vp, &bb, &type, &id) == RETURN_SUCCESS)
                {
                    object_edit_popup(type, id);
                }
                break;
            case DEL_OBJECT:
                if (find_item(cg, vp, &bb, &type, &id) == RETURN_SUCCESS)
                {
                    char * message=new char[32];
                    message[0]='\0';
                    sprintf(message, "Kill the %s?", object_types(type));
                        if (type==OBJECT_STRING)
                        {
                        char * n_message=new char[64+strlen(pstr[id].s_plotstring)];
                        n_message[0]='\0';
                        sprintf(n_message,"%s\n\"%s\"",message,pstr[id].s_plotstring);
                        delete[] message;
                        message=new char[strlen(n_message)+8];
                        message[0]='\0';
                        strcpy(message,n_message);
                        delete[] n_message;
                        }
                    if (yesno(message, NULL, NULL, NULL) == TRUE)
                    {
                        //Undo-Stuff
                        nrOfUndoObj=1;
                        undoObjIds[0]=id;
                        ObjectsDeleted(nrOfUndoObj,undoObjIds,type);
                    kill_object(type, id);
                    mainWin->mainArea->completeRedraw();
                    }
                    set_action(action_flag);//to reset the help-text in the left footer
                    delete[] message;
                }
                break;
            case MOVE_OBJECT_1ST:
                if (find_item(cg, vp, &bb, &type, &id) == RETURN_SUCCESS)
                {
                    current_object_id=id;
                    current_object_type=type;
                    if (type==OBJECT_STRING && is_valid_string(id)==TRUE)
                    {
                    parameters[0]=(pstr[id].master_align/100)%100;//none/graph/plot
                    parameters[1]=(pstr[id].master_align/10)%10;//x
                    parameters[2]=pstr[id].master_align%10;//y
                        if (parameters[0]!=0 && parameters[1]!=0 && parameters[2]!=0)//completely fixed!
                        {
                        errmsg(QObject::tr("The position of this string is fixed and therefore can not be moved!").toLocal8Bit().constData());
                        current_object_id=-1;
                        current_object_type=-1;
                        }
                    }
                    if (current_object_id>=0)
                    {
                    ShiftRect=QRect(xconvxlib(bb.xv1),yconvxlib(bb.yv2),xconvxlib(bb.xv2)-xconvxlib(bb.xv1),yconvxlib(bb.yv1)-yconvxlib(bb.yv2));
                    ShiftPoint=QPoint(x,y);
                    //mainWin->mainArea->rubber->setGeometry(ShiftRect);
                    mainWin->mainArea->repositionRubber(ShiftRect);
                    mainWin->mainArea->rubber->show();
                    anchor_point(x, y, vp);
                //slide_region(bb, x - anchor_x, y - anchor_y, 0);
                    set_action(MOVE_OBJECT_2ND);
                    }
                }
                break;
            case MOVE_OBJECT_2ND:
                shift.x = vp.x - anchor_vp.x;
                shift.y = vp.y - anchor_vp.y;

                    if (event->axis_restrict==X_AXIS)
                    {
                    shift.y=0.0;
                    }
                    else if (event->axis_restrict==Y_AXIS)
                    {
                    shift.x=0.0;
                    }

                //Undo-Stuff
                SaveObjectData(id,type);
                if (type==OBJECT_STRING)//strings can be fixed in position somehow (like centered or attached to x- or y-axis)
                {
                    if (is_valid_string(id)==TRUE)
                    {
                    parameters[0]=(pstr[id].master_align/100)%100;//none/graph/plot
                    parameters[1]=(pstr[id].master_align/10)%10;//x
                    parameters[2]=pstr[id].master_align%10;//y
                        if (parameters[0]!=0)
                        {
                            if (parameters[1]!=0)//fixed in x
                            {
                            shift.x=0;
                            }
                            if (parameters[2]!=0)//fixed in y
                            {
                            shift.y=0;
                            }
                        }
                    }
                }
                //Do action
                move_object(type, id, shift);
                //Undo-Stuff
                ObjectDataModified(id,type);
                addAditionalDescriptionToLastNode(UNDO_TYPE_OBJECT_MODIFIED,QString(),QObject::tr("moved"),-1);
                xdrawgraph();
                mainWin->mainArea->rubber->hide();
                mainWin->mainArea->completeRedraw();
                current_object_id=-1;
                current_object_type=-1;
                    if (Pick_and_Move_active==true)
                    reactivate_Pick_and_Move();
                    else
                    set_action(MOVE_OBJECT_1ST);
                break;
            case COPY_OBJECT1ST:
                if (find_item(cg, vp, &bb, &type, &id) == RETURN_SUCCESS)
                {
                    ShiftRect=QRect(xconvxlib(bb.xv1),yconvxlib(bb.yv2),xconvxlib(bb.xv2)-xconvxlib(bb.xv1),yconvxlib(bb.yv1)-yconvxlib(bb.yv2));
                    ShiftPoint=QPoint(x,y);
                    //mainWin->mainArea->rubber->setGeometry(ShiftRect);
                    mainWin->mainArea->repositionRubber(ShiftRect);
                    mainWin->mainArea->rubber->show();
                    anchor_point(x, y, vp);
                /// slide_region(bb, x - anchor_x, y - anchor_y, 0);
                    set_action(COPY_OBJECT2ND);
                    current_object_id=id;
                    current_object_type=type;
                }
                break;
            case COPY_OBJECT2ND:
                shift.x = vp.x - anchor_vp.x;
                shift.y = vp.y - anchor_vp.y;

                if (event->axis_restrict==X_AXIS)
                {
                shift.y=0.0;
                }
                else if (event->axis_restrict==Y_AXIS)
                {
                shift.x=0.0;
                }

                id = duplicate_object(type, id);
                if (type==OBJECT_STRING)//strings can be fixed in position somehow (like centered or attached to x- or y-axis)
                {
                    if (is_valid_string(id)==TRUE)
                    {
                    parameters[0]=(pstr[id].master_align/100)%100;//none/graph/plot
                    parameters[1]=(pstr[id].master_align/10)%10;//x
                    parameters[2]=pstr[id].master_align%10;//y
                        if (parameters[0]!=0)
                        {
                            if (parameters[1]!=0)//fixed in x
                            {
                            shift.x=0;
                            }
                            if (parameters[2]!=0)//fixed in y
                            {
                            shift.y=0;
                            }
                        }
                    }
                }
                move_object(type, id, shift);
                //Undo-Stuff
                undoObjIds[0]=id;
                ObjectsCreated(1,undoObjIds,type);
                addAditionalDescriptionToLastNode(UNDO_TYPE_OBJECT_INSERTED,QString(),QObject::tr("coppied"),-1);
                set_action(COPY_OBJECT1ST);
                mainWin->mainArea->rubber->hide();
                mainWin->mainArea->completeRedraw();
                current_object_id=-1;
                current_object_type=-1;
                break;
            case STR_LOC:
                id = next_string();
                init_string(id, vp);
                object_edit_popup(OBJECT_STRING, id);
                break;
            case MAKE_LINE_1ST:
                anchor_point(x, y, vp);
                rg[MAXREGION].linkto=cg;
                rg[MAXREGION].active=TRUE;
	        select_line(anchor_x, anchor_y, x, y, 0);
                simple_draw_setting=SIMPLE_DRAW_LINE;
                set_action(MAKE_LINE_2ND);
                break;
            case MAKE_LINE_2ND:
                simple_draw_setting&=(~SIMPLE_DRAW_LINE);
                if (event->axis_restrict==X_AXIS)
                {
                select_line(anchor_x, anchor_y, x, anchor_y, 0);
                vp = xlibdev2VPoint(x, anchor_y);
                }
                else if (event->axis_restrict==Y_AXIS)
                {
                select_line(anchor_x, anchor_y, anchor_x, y, 0);
                vp = xlibdev2VPoint(anchor_x, y);
                }
                else
                {
                select_line(anchor_x, anchor_y, x, y, 0);
                }
                if (next_line_id==-1)
                id = next_line();
                else
                id = next_line_id;
                init_line(id, anchor_vp, vp);
                next_line_id=-1;
                rg[MAXREGION].active=FALSE;
                    //Undo-Stuff
                    nrOfUndoObj=1;
                    undoObjIds[0]=id;
                    ObjectsCreated(nrOfUndoObj,undoObjIds,OBJECT_LINE);
                ForceRedraw(FALSE);
                check_for_dialog_open(OBJECT_LINE,id,false);
                set_action(MAKE_LINE_1ST);
                break;
            case MAKE_BOX_1ST:
                anchor_point(x, y, vp);
            /// select_region(anchor_x, anchor_y, x, y, 0);
                set_action(MAKE_BOX_2ND);
                break;
            case MAKE_BOX_2ND:
            /// select_region(anchor_x, anchor_y, x, y, 0);
                id = next_box();
                init_box(id, anchor_vp, vp);
                    //Undo-Stuff
                    nrOfUndoObj=1;
                    undoObjIds[0]=id;
                    ObjectsCreated(nrOfUndoObj,undoObjIds,OBJECT_BOX);
                mainWin->mainArea->completeRedraw();
                set_action(MAKE_BOX_1ST);
                break;
            case MAKE_ELLIP_1ST:
                anchor_point(x, y, vp);
            /// select_region(anchor_x, anchor_y, x, y, 0);
                set_action(MAKE_ELLIP_2ND);
                break;
            case MAKE_ELLIP_2ND:
            /// select_region(anchor_x, anchor_y, x, y, 0);
                id = next_ellipse();
                init_ellipse(id, anchor_vp, vp);
                    //Undo-Stuff
                    nrOfUndoObj=1;
                    undoObjIds[0]=id;
                    ObjectsCreated(nrOfUndoObj,undoObjIds,OBJECT_ELLIPSE);
                mainWin->mainArea->completeRedraw();
                set_action(MAKE_ELLIP_1ST);
                break;
            case AUTO_NEAREST:/*TODO: AUTOSCALE BY ONE SET-->RESCALE ALL OTHER GRAPHS TOO*/
                if (find_point(cg, vp, &track_setno, &loc) == RETURN_SUCCESS)
                {
                    SaveTickmarksStatesPrevious(X_AXIS,Y_AXIS,cg,cg);
                        autoscale_byset(cg, track_setno, AUTOSCALE_XY);
                        if (FormAxisProperties!=NULL)
                        FormAxisProperties->update_ticks(cg);
                        mainWin->mainArea->completeRedraw();
                    TickmarksModified(X_AXIS,Y_AXIS,cg,cg);
                    sprintf(dummy,"Autoscale [G%d] on [S%d]",get_cg(),track_setno);
                    addAditionalDescriptionToLastNode(-1,QString(dummy),QString(),0);
                    set_action(DO_NOTHING);
                }
                break;
            case TRACKER:
                track_point(cg, track_setno, &track_loc, -1);
                break;
            case DEL_POINT:
                if (find_point(cg, vp, &track_setno, &loc) == RETURN_SUCCESS)
                {
                    DataPointEdited(cg,track_setno,&loc,1,NULL,1);
                    del_point(cg, track_setno, loc);
                    update_set_lists(cg);
                    mainWin->mainArea->completeRedraw();
                    set_action(action_flag);
                }
                break;
            case MOVE_POINT1ST:
                if (find_point(cg, vp, &track_setno, &track_loc) == RETURN_SUCCESS)
                {
                //anchor_point(x, y, vp);
                    get_point(cg, track_setno, track_loc, &wp);
                    vp=Wpoint2Vpoint(wp);
                    xlibVPoint2dev(vp,&x,&y);
                //qDebug() << "movePoint1ST: (" << x << "|" << y << ") wp=(" << wp.x << "|" << wp.y << ")";
                    anchor_point(x, y, vp);
                    ///select_line(anchor_x, anchor_y, x, y, 0);
                    set_action(MOVE_POINT2ND);
                }
                break;
            case MOVE_POINT2ND:
                if (is_valid_setno(cg, track_setno))
                {
                    get_point(cg, track_setno, track_loc, &wp);
                    view2world(vp.x, vp.y, &wp_new.x, &wp_new.y);
                    switch (move_dir)
                    {
                    case 0:
                        if (event->axis_restrict==X_AXIS)
                        wp.x = wp_new.x;
                        else if (event->axis_restrict==Y_AXIS)
                        wp.y = wp_new.y;
                        else
                        wp = wp_new;
                    break;
                    case 1:
                        wp.x = wp_new.x;
                    break;
                    case 2:
                        wp.y = wp_new.y;
                    break;
                    }
                    ///get_datapoint(cg,track_setno,track_loc,&nrOfUndoObj,&undo_dp);
                        if (undo_dp!=NULL) delete[] undo_dp;
                        undo_dp=new Datapoint[2];
                        zero_datapoint(undo_dp);
                        zero_datapoint(undo_dp+1);
                        if (undo_nrs!=NULL) delete[] undo_nrs;
                        undo_nrs=new int[2];
                        undo_nrs[1]=undo_nrs[0]=track_loc;
                    copy_Datapoint(cg,track_setno,track_loc,undo_dp);
                        undo_dp[0].ex[0]=wp.x;
                        undo_dp[0].ex[1]=wp.y;
                    DataPointEdited(cg,track_setno,undo_nrs,1,undo_dp,2);
                    set_point(cg, track_setno, track_loc, wp);
                        if (FormPointExplorer!=NULL)
                        FormPointExplorer->update_point_locator(cg, track_setno, track_loc);
                    ForceRedraw(FALSE);
		    set_action(MOVE_POINT1ST);
                }
                break;
            case ADD_POINT:
            view2world(vp.x, vp.y, &wp.x, &wp.y);
                zero_datapoint(&dpoint);
                dpoint.ex[0] = wp.x;
                dpoint.ex[1] = wp.y;
                switch (add_at)
                {
                case ADD_POINT_BEGINNING: /* at the beginning */
                loc = 0;
                    break;
                case ADD_POINT_END: /* at the end */
                loc = getsetlength(cg, track_setno);
                    break;
                default: /* between nearest points */
                loc = find_insert_location(cg, track_setno, vp);
                    break;
                }
                if (undo_dp!=NULL) delete[] undo_dp;
                undo_dp=new Datapoint[2];
                zero_datapoint(undo_dp);
                zero_datapoint(undo_dp+1);
                if (undo_nrs!=NULL) delete[] undo_nrs;
                undo_nrs=new int[2];
                undo_nrs[1]=undo_nrs[0]=loc;
                memcpy(undo_dp,&dpoint,sizeof(Datapoint));
                    if (dpoint.s!=NULL)
                    {
                    undo_dp[0].s = copy_string(NULL,dpoint.s);
                    undo_dp[0].orig_s = copy_string(NULL,dpoint.orig_s);
                    }
                    else
                    {
                    undo_dp[0].s = NULL;
                    undo_dp[0].orig_s = NULL;
                    }
                DataPointEdited(cg,track_setno,&loc,1,undo_dp,0);
                //qDebug() << "AddPoint: " << dpoint.ex[0] << "|" << dpoint.ex[0] << " in G" << cg << ".S" << track_setno << " loc=" << loc;
                if (add_point_at(cg, track_setno, loc, &dpoint) == RETURN_SUCCESS)
                {
                update_set_lists(cg);
                ForceRedraw(FALSE);
                }
                set_action(action_flag);
                break;
            case PLACE_LEGEND_1ST:
                shift_legend_graph=-1;
                if (legend_clicked(cg, vp, &bb) == TRUE)
                {
                shift_legend_graph=cg;
                }
                else
                {
                    for (int kkll=0;kkll<number_of_graphs();kkll++)
                    {
                        if (legend_clicked(kkll, vp, &bb) == TRUE)
                        shift_legend_graph=kkll;
                    }
                    if (shift_legend_graph!=-1)
                    {
                    legend_clicked(shift_legend_graph, vp, &bb);
                    }
                }
                if (shift_legend_graph!=-1)
                {
                ShiftRect=QRect(xconvxlib(bb.xv1),yconvxlib(bb.yv2),xconvxlib(bb.xv2)-xconvxlib(bb.xv1),yconvxlib(bb.yv1)-yconvxlib(bb.yv2));
                ShiftPoint=QPoint(x,y);
                //mainWin->mainArea->rubber->setGeometry(ShiftRect);
                mainWin->mainArea->repositionRubber(ShiftRect);
                mainWin->mainArea->rubber->show();
                    anchor_point(x, y, vp);
                    /// slide_region(bb, x - anchor_x, y - anchor_y, 0);
                    set_action(PLACE_LEGEND_2ND);
                }
                break;
            case PLACE_LEGEND_2ND:
                SaveSingleGraphState(cg,UNDO_APPEARANCE);
                shift.x = vp.x - anchor_vp.x;
                shift.y = vp.y - anchor_vp.y;

                if (event->axis_restrict==X_AXIS)
                {
                shift.y=0.0;
                }
                else if (event->axis_restrict==Y_AXIS)
                {
                shift.x=0.0;
                }

                get_graph_legend(shift_legend_graph, &tmp_l);
                if (tmp_l.autoattach!=G_LB_ATTACH_NONE)
                {
                    if ( (tmp_l.autoattach & G_LB_ATTACH_LEFT)!=0 || (tmp_l.autoattach & G_LB_ATTACH_RIGHT)!=0 )
                    {
                    shift.x=0;
                    }
                    if ( (tmp_l.autoattach & G_LB_ATTACH_TOP)!=0 || (tmp_l.autoattach & G_LB_ATTACH_BOTTOM)!=0 )
                    {
                    shift.y=0;
                    }
                }
                move_legend(shift_legend_graph, shift);
                SingleGraphModified(cg,UNDO_APPEARANCE);
                    if (FormGraphAppearance!=NULL && cg==shift_legend_graph)
                    FormGraphAppearance->updatelegends(shift_legend_graph);
                mainWin->mainArea->rubber->hide();
                mainWin->mainArea->completeRedraw();
                    if (Pick_and_Move_active==true)
                    reactivate_Pick_and_Move();
                    else
                    set_action(PLACE_LEGEND_1ST);
                break;
            case PLACE_TIMESTAMP_1ST:
                if (timestamp_clicked(vp, &bb) == TRUE)
                {
                ShiftRect=QRect(xconvxlib(bb.xv1),yconvxlib(bb.yv2),xconvxlib(bb.xv2)-xconvxlib(bb.xv1),yconvxlib(bb.yv1)-yconvxlib(bb.yv2));
                ShiftPoint=QPoint(x,y);
                //mainWin->mainArea->rubber->setGeometry(ShiftRect);
                mainWin->mainArea->repositionRubber(ShiftRect);
                mainWin->mainArea->rubber->show();
                anchor_point(x, y, vp);
                /// slide_region(bb, x - anchor_x, y - anchor_y, 0);
                set_action(PLACE_TIMESTAMP_2ND);
                }
                break;
            case PLACE_TIMESTAMP_2ND:
                SavePlotAppearance();
                shift.x = vp.x - anchor_vp.x;
                shift.y = vp.y - anchor_vp.y;

                if (current_axis_restriction==X_AXIS)
                {
                shift.y=0.0;
                }
                else if (current_axis_restriction==Y_AXIS)
                {
                shift.x=0.0;
                }

                move_timestamp(shift);
                mainWin->mainArea->rubber->hide();
                mainWin->mainArea->completeRedraw();
                PlotAppearanceModified();
                    if (Pick_and_Move_active==true)
                    reactivate_Pick_and_Move();
                    else
                    set_action(PLACE_TIMESTAMP_1ST);
                break;
	    case SEL_POINT:
                if (get_graph_locator(cg, &locator) == RETURN_SUCCESS)
                {
		    view2world(vp.x, vp.y, &locator.dsx, &locator.dsy);
                    locator.pointset = TRUE;
                    SaveLocatorFixPoint(cg);
		    set_graph_locator(cg, &locator);
                    LocatorFixPointModified(cg);
                    if (FormLocatorProps!=NULL)
                    FormLocatorProps->update_locator_items(cg);
                    mainWin->mainArea->completeRedraw();
                }
		set_action(DO_NOTHING);
		break;
	    case DEF_REGION1ST:
            anchor_point(x, y, vp);
            select_line(anchor_x, anchor_y, x, y, 0);
            simple_draw_setting=SIMPLE_DRAW_LINE;
            set_action(DEF_REGION2ND);
		break;
	    case DEF_REGION2ND:
                simple_draw_setting&=(~SIMPLE_DRAW_REGION);
		set_action(DO_NOTHING);
                rg[MAXREGION].active=FALSE;
                //select_line(anchor_x, anchor_y, x, y, 0);
                    ///Undo_Stuff
                    undo_region2.active=true;
                    undo_region2.n = 0;
                    undo_region2.x=undo_region2.y=NULL;
                    undo_region2.type = regiontype;
                    undo_region2.linkto = cg;
                    view2world(anchor_vp.x, anchor_vp.y, &undo_region2.x1, &undo_region2.y1);
                    view2world(vp.x, vp.y, &undo_region2.x2, &undo_region2.y2);
                    RegionModified(nr,&undo_region2,0);
                activate_region(nr, regiontype, cg);
		view2world(anchor_vp.x, anchor_vp.y, &rg[nr].x1, &rg[nr].y1);
		view2world(vp.x, vp.y, &rg[nr].x2, &rg[nr].y2);
		mainWin->mainArea->completeRedraw();
            if (FormRegionMaster!=NULL)
            {
            FormRegionMaster->init();
            }
		break;
	    case DEF_REGION:
                if (rg[nr].n>0 || region_def_under_way==-1)
                {
                XCFREE(rg[nr].x);
                XCFREE(rg[nr].y);
                }
                region_def_under_way=nr;
                anchor_point(x, y, vp);
                rg[nr].type=regiontype;
                rg[nr].linkto = cg;
		iax[region_pts] = x;
		iay[region_pts] = y;
                view2world(vp.x, vp.y, &region_wps[region_pts].x, &region_wps[region_pts].y);
                if (region_pts < MAX_POLY_POINTS) {
                    region_pts++;
                } else {
                    errmsg("Too many points in polygon!");
                }
                rg[nr].n = region_pts;
                rg[nr].x = (double*)xcalloc(region_pts+1, sizeof(double));//SIZEOF_DOUBLE);
                rg[nr].y = (double*)xcalloc(region_pts+1, sizeof(double));//SIZEOF_DOUBLE);
                for (int iii = 0; iii < region_pts; iii++)
                {
                    rg[nr].x[iii] = region_wps[iii].x;
                    rg[nr].y[iii] = region_wps[iii].y;
                }
                select_line(anchor_x, anchor_y, x, y, 0);
                simple_draw_setting|=SIMPLE_DRAW_REGION;
                ForceRedraw(FALSE);
		break;
            default:
                break;
            }
            break;
#if QT_VERSION >= 0x060000
        case Qt::MiddleButton://Button2:
#else
        case Qt::MidButton://Button2:
#endif
            switch (action_flag) {
            case TRACKER:
                track_setno = -1;
                if (find_point(cg, vp, &track_setno, &track_loc) == RETURN_SUCCESS) {
                    track_point(cg, track_setno, &track_loc, 0);
                } else {
                    if (FormPointExplorer!=NULL)
                    FormPointExplorer->update_point_locator(cg, track_setno, track_loc);
                }
                break;
            default:
                break;
            }
            break;
        case Qt::RightButton://Button3:
            mainWin->mainArea->rubber->hide();
            switch (action_flag)
            {
            case DO_NOTHING:
/*
 *                 find_item(cg, vp, &anchor_vp, &type, &id);
 *                 sprintf(buf, "type = %d, id = %d", type, id);
 *                 set_left_footer(buf);
 */
                if (context_menu_in_main)
                {
                    if (find_point(cg, vp, &track_setno, &loc) == RETURN_SUCCESS)
                    {
                    if (sel_graphs!=NULL) delete[] sel_graphs;
                        sel_graphs=new int[2];
                        sel_graphs[0]=get_cg();
                        sel_graphs[1]=track_setno;
                        main_win_gr_no=get_cg();
                        main_win_set_no=track_setno;
                        prepare_highlight_segments_with_sets(1,sel_graphs,sel_graphs+1,TRUE);
                        mainWin->start_highlighting();
                    mainWin->mainArea->inlineSetPopup->prepareForAction();
                    mainWin->mainArea->inlineSetPopup->update_menu_content();
                    mainWin->mainArea->inlineSetPopup->exec(mainWin->mainArea->mapToGlobal(QPoint(event->xbutton.x,event->xbutton.y)));
                    }
                }
                break;
            case PICK_MOVE:
                Pick_and_Move_active=false;
                set_action(DO_NOTHING);
                break;
            case TRACKER:
                track_point(cg, track_setno, &track_loc, +1);
                break;
            case DEF_REGION:
                /* end region definition */
                region_def_under_way=-1;
                XCFREE(rg[nr].x);
                XCFREE(rg[nr].y);
                ///select_line(x, y, iax[0], iay[0], 0);
                ///Undo_Stuff
                if (region_pts>2)
                {
                    CopyRegion(&undo_region2,rg+nr);
                undo_region2.active=true;
                undo_region2.n = region_pts;
                XCFREE(undo_region2.x);
                XCFREE(undo_region2.y);
                undo_region2.type = regiontype;
                undo_region2.linkto = cg;
                undo_region2.x = (double*)xcalloc(region_pts, sizeof(double));///SIZEOF_DOUBLE);
                undo_region2.y = (double*)xcalloc(region_pts, sizeof(double));///SIZEOF_DOUBLE);
                    for (int i = 0; i < region_pts; i++)
                    {
                    undo_region2.x[i] = region_wps[i].x;
                    undo_region2.y[i] = region_wps[i].y;
                    }
                RegionModified(nr,&undo_region2,0);
                }
            load_poly_region(nr, cg, region_pts, region_wps);
                set_action(DO_NOTHING);
                    if (FormRegionMaster!=NULL)
                    {
                    FormRegionMaster->init();
                    }
                updateRunning=false;
                simple_draw_setting=SIMPLE_DRAW_NONE;
            force_redraw();/// modified from completeredraw()
                updateRunning=true;
                break;
            case PAN_2ND:
                get_graph_world(cg,&wtmp);
                    sprintf(dummy,"    world %g, %g, %g, %g",wtmp.xg1,wtmp.yg1,wtmp.xg2,wtmp.yg2);
                    ListOfChanges << QString(dummy);
                SingleGraphTickmarksModified(cg);//no 'break' here, because we want the default actions as well
                addAditionalDescriptionToLastNode(-1,QString("Graph world changed [G")+QString::number(cg)+QObject::tr("] (panned)"),QString(),-1);
            default:
                //qDebug() << "RightButton, deactivate everything A, simpleRD=" << simple_draw_setting;
                rg[MAXREGION].active=FALSE;
                //qDebug() << "RightButton, deactivate everything B, simpleRD=" << simple_draw_setting;
                simple_draw_setting=SIMPLE_DRAW_NONE;
                    /*if (Pick_and_Move_active==true)
                    reactivate_Pick_and_Move();
                    else*/
                set_action(DO_NOTHING);
                mainWin->mainArea->completeRedraw();
                break;
            }
            immediateUpdate=old_upd;
            updateRunning=false;
            return;
	default: /* TODO: wheel mice */
            break;
        }
	break;
    case QEvent::Wheel:
        ;/*cout << "wheel event" << endl;
            if (event->delta>0)
            cout << "zoom in" << endl;
            else
            cout << "zoom out" << endl;
        mainWin->mainArea->completeRedraw();*/
    break;
    case QEvent::KeyRelease:
        if ((event->key==Qt::Key_Alt || event->key==Qt::Key_Control) && (action_flag==MOVE_OBJECT_2ND ||
            action_flag==COPY_OBJECT2ND || 
            action_flag==PLACE_TIMESTAMP_2ND || 
            action_flag==MOVE_AXIS_LABEL_2ND || 
            action_flag==MOVE_GRAPH_2ND || 
            action_flag==MOVE_TITLE_2ND || 
            action_flag==PLACE_LEGEND_2ND))
        {
#if QT_VERSION >= 0x050000
        localPos=QPointF(mainWin->mainArea->mapFromGlobal(QCursor::pos()));
        me=new QMouseEvent(QEvent::MouseMove,localPos,Qt::NoButton,Qt::NoButton,Qt::NoModifier);
#else
        me=new QMouseEvent(QEvent::MouseMove,mainWin->mainArea->mapFromGlobal(QCursor::pos()),Qt::NoButton,Qt::NoButton,Qt::NoModifier);
#endif
        mainWin->mainArea->mouseMoveEvent(me);
        set_left_footer_to_action(action_flag,NO_AXIS);
            /*if (event->key==Qt::Key_Alt)
            {
            qDebug() << "Key Alt Released";
            }
            else if (event->key==Qt::Key_Control)
            {
            qDebug() << "Key Ctrl Released";
            }*/

        /*alt_event=*event;
        event->type=QEvent::MouseMove;
        event->key=0;
            if (event->key==Qt::Key_Alt)
            {
            event->alt=false;
            }
            else if (event->key==Qt::Key_Control)
            {
            event->ctrl=false;
            }
        my_proc(&alt_event);*/
        }
    break;
    case QEvent::KeyPress://KeyPress:
        //XLookupString((XKeyEvent *) event, &keybuf, 1, &keys, &compose);
        if (event->key==Qt::Key_Escape)
        { /* Esc */
            if (action_flag==DEF_REGION)
            {
            /* end region definition */
            region_def_under_way=-1;
            XCFREE(rg[nr].x);
            XCFREE(rg[nr].y);
            ///Undo_Stuff
            if (region_pts>2)
            {
            CopyRegion(&undo_region2,rg+nr);
            undo_region2.active=true;
            undo_region2.n = region_pts;
            XCFREE(undo_region2.x);
            XCFREE(undo_region2.y);
            undo_region2.type = regiontype;
            undo_region2.linkto = cg;
            undo_region2.x = (double*)xcalloc(region_pts, sizeof(double));///SIZEOF_DOUBLE);
            undo_region2.y = (double*)xcalloc(region_pts, sizeof(double));///SIZEOF_DOUBLE);
                for (int i = 0; i < region_pts; i++)
                {
                undo_region2.x[i] = region_wps[i].x;
                undo_region2.y[i] = region_wps[i].y;
                }
            RegionModified(nr,&undo_region2,0);
            }
                load_poly_region(nr, cg, region_pts, region_wps);
                if (FormRegionMaster!=NULL)
                {
                FormRegionMaster->init();
                }
            }
            else if (action_flag==PICK_MOVE)
            {
            Pick_and_Move_active=false;
            set_action(DO_NOTHING);
            }
            else if (action_flag==PAN_2ND)
            {
            get_graph_world(cg,&wtmp);
                sprintf(dummy,"    world %g, %g, %g, %g",wtmp.xg1,wtmp.yg1,wtmp.xg2,wtmp.yg2);
                ListOfChanges << QString(dummy);
            SingleGraphTickmarksModified(cg);
            addAditionalDescriptionToLastNode(-1,QString("Graph world changed [G")+QString::number(cg)+QObject::tr("] (panned)"),QString(),-1);
                if (Pick_and_Move_active==true)
                reactivate_Pick_and_Move();
                else
                set_action(PAN_1ST);
            return;
            }
            rg[MAXREGION].active=FALSE;
            mainWin->mainArea->rubber->hide();
            simple_draw_setting=SIMPLE_DRAW_NONE;
            immediateUpdate=old_upd;
            updateRunning=false;
            set_action(DO_NOTHING);
            mainWin->mainArea->completeRedraw();
            return;
        }
        else if (event->key==Qt::Key_L && event->ctrl==true && event->alt==false)
        {
            Pick_and_Move_active=false;
            if (is_legend_movable(g[get_cg()].l.autoattach)==true)
            {
            set_action(DO_NOTHING);
            set_action(PLACE_LEGEND_1ST);
            }
        }
        else if (event->key==Qt::Key_L && event->ctrl==true && event->alt==true)
        {
            Pick_and_Move_active=false;
        set_action(DO_NOTHING);
        set_action(MAKE_LINE_1ST);
        }
        else if (event->key==Qt::Key_A && event->ctrl==true && event->alt==false)
        {
        int cg = get_cg();
        autoscale_graph(cg, AUTOSCALE_XY);
            if (FormAxisProperties!=NULL)
            FormAxisProperties->update_ticks(cg);
        mainWin->mainArea->completeRedraw();
        }
        else if (event->key==Qt::Key_D && event->ctrl==true && event->alt==false)
        {
            Pick_and_Move_active=false;
        set_action(DO_NOTHING);
        set_action(DEL_OBJECT);
        }
        else if (event->key==Qt::Key_M && event->ctrl==true && event->alt==false)
        {
            Pick_and_Move_active=false;
        set_action(DO_NOTHING);
        set_action(MOVE_OBJECT_1ST);
        }
        else if (event->key==Qt::Key_T && event->ctrl==true && event->alt==false)
        {
            Pick_and_Move_active=false;
        set_action(DO_NOTHING);
        set_action(PLACE_TIMESTAMP_1ST);
        }
        else if (event->key==Qt::Key_I && event->ctrl==true && event->alt==false)
        {
            if (event->shift==true)
            strcpy(dummy,QObject::tr("Report on (all) visible intersection(s):").toLocal8Bit().constData());
            else
            strcpy(dummy,QObject::tr("Report on (first) visible intersection:").toLocal8Bit().constData());
        stufftext(dummy);
        double * intersection_x=new double[2];
        double * intersection_y=new double[2];
        double * intersection_a=new double[2];
        double cx,cy,ca;
        bool found_first;
        int vis_intersections;
        int n_intersections,total_n_intersections=0;
        //cout << "number_of_sets=" << number_of_sets(get_cg()) << endl;
            for (int klm=0;klm<number_of_sets(get_cg());klm++)
            {
                for (int klm2=klm+1;klm2<number_of_sets(get_cg());klm2++)
                {
                if (is_set_active(get_cg(),klm)==FALSE || is_set_active(get_cg(),klm2)==FALSE) continue;
                if (is_set_hidden(get_cg(),klm)==TRUE || is_set_hidden(get_cg(),klm2)==TRUE) continue;
                    if (getsetlength(get_cg(),klm)<2 || getsetlength(get_cg(),klm2)<2)
                    {
                    /*stufftext(QObject::tr("no intersection found").toLocal8Bit().constData());
                    cout << "no intersection A" << endl;*/
                    continue;
                    }
                /// get_all_intersection_points_between_two_sets(get_cg(),klm,get_cg(),klm2,&intersection_x,&intersection_y,&n_intersections);
                if (get_all_intersection_points_and_all_angles_between_two_sets(get_cg(),klm,get_cg(),klm2,&intersection_x,&intersection_y,&intersection_a,&n_intersections)==RETURN_FAILURE) continue;
                //cout << "intersections=" << n_intersections << endl;
                found_first=false;
                cx=cy=ca=0.0;
                vis_intersections=0;
                for (int klm3=0;klm3<n_intersections;klm3++)//find the visible intersection points
                {
                    if (inregion(MAXREGION,intersection_x[klm3],intersection_y[klm3]))
                    {
                    /*found_first=true;
                    cx=intersection_x[klm3];
                    cy=intersection_y[klm3];
                    ca=intersection_a[klm3];*/
                    intersection_x[vis_intersections]=intersection_x[klm3];
                    intersection_y[vis_intersections]=intersection_y[klm3];
                    intersection_a[vis_intersections++]=intersection_a[klm3];
                    total_n_intersections++;
                    //break;
                    }
                }
                    //if (n_intersections<1 || found_first==false)
                    if (vis_intersections<1)
                    {
                    /*stufftext(QObject::tr("no intersection found").toLocal8Bit().constData());
                    cout << "no intersection inside visible area" << endl;*/
                    continue;
                    }
                    if (event->shift==false)
                    {
                    vis_intersections=1;//we are only interested in the first one
                    }
                    //intersection_x[0]=cx;
                    //get_intersection_angles(get_cg(),klm,get_cg(),klm2,intersection_x,intersection_a,1);
                    //ca=intersection_a[0];
                sprintf(dummy,"G%d.S%d <--> G%d.S%d:",get_cg(),klm,get_cg(),klm2);
                stufftext(dummy);
                    //cout << dummy << endl;
                    for (int klm3=0;klm3<vis_intersections;klm3++)
                    {
                        if (DecimalPointToUse=='.')
                        {
                        sprintf(dummy,"[ %g , %g ] angle= %g rad | %g degrees",intersection_x[klm3],intersection_y[klm3],intersection_a[klm3],intersection_a[klm3]*57.295779513082325);
                        }
                        else//','
                        {
                        sprintf(dummy,"[ %g | %g ] angle= %g rad | %g degrees",intersection_x[klm3],intersection_y[klm3],intersection_a[klm3],intersection_a[klm3]*57.295779513082325);
                        SetDecimalSeparatorToUserValue(dummy);
                        }
                    stufftext(dummy);
                    }
                }//second loop through sets
            }//first loop through sets
            if (total_n_intersections<1)
            {
            stufftext(QObject::tr("No visible intersections found.").toLocal8Bit().constData());
            }
        if (intersection_x!=NULL) delete[] intersection_x;
        if (intersection_y!=NULL) delete[] intersection_y;
        if (intersection_a!=NULL) delete[] intersection_a;
        }
        else if (event->key==Qt::Key_U && event->ctrl==true && event->alt==false)
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
        /*
        for (gno = 0; gno < number_of_graphs(); gno++) {
            for (setno = 0; setno < number_of_sets(gno); setno++) {
            do_update_hotlink(gno, setno);
            }
        }*/
        unset_wait_cursor();
        mainWin->mainArea->completeRedraw();
        }
        else if (event->key==Qt::Key_V && event->ctrl==true && event->alt==true)
        {
            Pick_and_Move_active=false;
        set_action(DO_NOTHING);
        set_action(VIEW_1ST);
        }
        else if (event->key==Qt::Key_Z && event->ctrl==false && event->alt==true)
        {
            Pick_and_Move_active=false;
        set_action(DO_NOTHING);
        set_action(ZOOM_1ST);
        }
        else if (event->key==Qt::Key_B && event->ctrl==true && event->alt==true)
        {
            Pick_and_Move_active=false;
        set_action(DO_NOTHING);
        set_action(MAKE_BOX_1ST);
        }
        else if (event->key==Qt::Key_T && event->ctrl==true && event->alt==true)
        {
            Pick_and_Move_active=false;
        set_action(DO_NOTHING);
        set_action(STR_LOC);
        }
        else if (event->key==Qt::Key_E && event->ctrl==true && event->alt==true)
        {
            Pick_and_Move_active=false;
        set_action(DO_NOTHING);
        set_action(MAKE_ELLIP_1ST);
        }
        else if (event->key==Qt::Key_P && event->ctrl==false && event->alt==true)
        {
            Pick_and_Move_active=false;
        set_action(DO_NOTHING);
        set_action(PAN_1ST);
        }
        else if (event->key==Qt::Key_M && event->ctrl==false && event->alt==true)
        {
            Pick_and_Move_active=false;
        set_action(DO_NOTHING);
        set_action(PICK_MOVE);
        }
        else if (event->key==Qt::Key_Plus && event->ctrl==true && event->alt==false)
        {
        mainWin->dozz();
        }
        else if (event->key==Qt::Key_Minus && event->ctrl==true && event->alt==false)
        {
        mainWin->doZz();
        }
        else if (event->key==Qt::Key_V && event->ctrl==true && event->alt==false)//paste_event
        {
        GeneralPaste(QApplication::clipboard()->mimeData());
        }
        else if (event->key==Qt::Key_1 && event->ctrl==true && event->alt==false)//set Page Zoom back to '1'
        {
        mainWin->sldPageZoom->setValue(0);
        }
        else if ((event->key==Qt::Key_Alt || event->key==Qt::Key_Control) && (action_flag==MOVE_OBJECT_2ND ||
            action_flag==COPY_OBJECT2ND ||
            action_flag==PLACE_TIMESTAMP_2ND ||
            action_flag==MOVE_AXIS_LABEL_2ND ||
            action_flag==MOVE_GRAPH_2ND ||
            action_flag==MOVE_TITLE_2ND ||
            action_flag==PLACE_LEGEND_2ND))
        {
#if QT_VERSION >= 0x050000
        localPos=QPointF(mainWin->mainArea->mapFromGlobal(QCursor::pos()));
#endif
            if (event->key==Qt::Key_Alt)
            {
            //qDebug() << "Key Alt Pressed";
#if QT_VERSION >= 0x050000
            me=new QMouseEvent(QEvent::MouseMove,localPos,Qt::NoButton,Qt::NoButton,Qt::AltModifier);
#else
            me=new QMouseEvent(QEvent::MouseMove,mainWin->mainArea->mapFromGlobal(QCursor::pos()),Qt::NoButton,Qt::NoButton,Qt::AltModifier);
#endif
            }
            else if (event->key==Qt::Key_Control)
            {
            //qDebug() << "Key Ctrl Pressed";
#if QT_VERSION >= 0x050000
            me=new QMouseEvent(QEvent::MouseMove,localPos,Qt::NoButton,Qt::NoButton,Qt::ControlModifier);
#else
            me=new QMouseEvent(QEvent::MouseMove,mainWin->mainArea->mapFromGlobal(QCursor::pos()),Qt::NoButton,Qt::NoButton,Qt::ControlModifier);
#endif
            }
        mainWin->mainArea->mouseMoveEvent(me);
        /*alt_event=*event;
        event->type=QEvent::MouseMove;
        event->key=0;
            if (event->key==Qt::Key_Alt)
            {
            event->alt=true;
            }
            else if (event->key==Qt::Key_Control)
            {
            event->ctrl=true;
            }
        my_proc(&alt_event);*/
        if (event->key==Qt::Key_Alt)
        set_left_footer_to_action(action_flag,X_AXIS);
        else if (event->key==Qt::Key_Control)
        set_left_footer_to_action(action_flag,Y_AXIS);
        }
        /*else if (event->key==Qt::Key_Left && event->ctrl==true && event->alt==false)
        {
        mainWin->doLeft();
        }
        else if (event->key==Qt::Key_Right && event->ctrl==true && event->alt==false)
        {
        mainWin->doRight();
        }
        else if (event->key==Qt::Key_Down && event->ctrl==true && event->alt==false)
        {
        mainWin->doDown();
        }
        else if (event->key==Qt::Key_Up && event->ctrl==true && event->alt==false)
        {
        mainWin->doUp();
        }*/
        break;
    default:
        immediateUpdate=old_upd;
        updateRunning=false;
	break;
    }
immediateUpdate=old_upd;
updateRunning=false;
}

void unselect_buttons(void)
{
    mainWin->cmdPan->setDown(false);
    mainWin->cmdPick->setDown(false);
    mainWin->cmdZoom->setDown(false);
    mainWin->cmdZX->setDown(false);
    mainWin->cmdZY->setDown(false);
    mainWin->cmdAutoO->setDown(false);
        if (FormDrawObjects!=NULL)
        {
        FormDrawObjects->cmdEllipse->setDown(false);
        FormDrawObjects->cmdBox->setDown(false);
        FormDrawObjects->cmdDelObj->setDown(false);
        FormDrawObjects->cmdText->setDown(false);
        FormDrawObjects->cmdLine->setDown(false);
        FormDrawObjects->cmdEditObj->setDown(false);
        FormDrawObjects->cmdCopyObj->setDown(false);
        FormDrawObjects->cmdMoveObj->setDown(false);
        }
        if (FormPointExplorer!=NULL)
        {
        FormPointExplorer->cmdTrack->setDown(false);
        FormPointExplorer->cmdMove->setDown(false);
        FormPointExplorer->cmdMoveX->setDown(false);
        FormPointExplorer->cmdMoveY->setDown(false);
        FormPointExplorer->cmdDelete->setDown(false);
        FormPointExplorer->cmdInsert->setDown(false);
        FormPointExplorer->cmdAppend->setDown(false);
        FormPointExplorer->cmdPrepend->setDown(false);
        }
}

/*
 * set the action_flag to the desired action (actions are
 * defined in defines.h), if 0 then cleanup the results
 * from previous actions.
 */
void set_action(CanvasAction act)
{
/*
 * indicate what's happening with a message in the left footer
 */
    unselect_buttons();
    get_tracking_props(&track_setno, &move_dir, &add_at);
//qDebug() << "set_action=" << (int)act << " pan=" << (int)PAN_1ST << " pick=" << (int)PICK_MOVE;
    switch (act) {
    case DO_NOTHING:
        current_object_id=-1;
        current_object_type=-1;
        next_line_id=-1;
        switch (action_flag) {
        case ZOOM_2ND:
        case ZOOMX_2ND:
        case ZOOMY_2ND:
            if (get_graph_type(get_cg())==GRAPH_POLAR2)
            force_redraw_wo_dialog_updates();
            break;
        case VIEW_2ND:
        case CORNER_2ND:
            /// select_region(anchor_x, anchor_y, x, y, 0);
            break;
        case MOVE_OBJECT_2ND:
        case COPY_OBJECT2ND:
        case PLACE_LEGEND_2ND:
        case PLACE_TIMESTAMP_2ND:
            /// slide_region(bb, x - anchor_x, y - anchor_y, 0);
            break;
        case DEF_REGION2ND:
            //force_redraw();
            break;
        case MAKE_LINE_2ND:
            select_line(anchor_x, anchor_y, x, y, 0);
            force_redraw();
            break;
        case DEF_REGION:
            /*select_line(anchor_x, anchor_y, x, y, 0);
	    for (i = 0; i < region_pts - 1; i++) {
                select_line(iax[i], iay[i], iax[i + 1], iay[i + 1], 0); 
            }*/
            break;
        case MOVE_POINT2ND:
            switch (move_dir) {
            case 0:
                ///select_line(anchor_x, anchor_y, x, y, 0);
            break;
            case 1:
                ///select_line(anchor_x, anchor_y, x, anchor_y, 0);
	        break;
            case 2:
                ///select_line(anchor_x, anchor_y, anchor_x, y, 0);
	        break;
            }
            break;
        default:
            break;
        }
	hide_rubber_lines();
	set_cursor(-1);
    set_left_footer_to_action(act,current_axis_restriction);
    point_explorer_activ=false;
    updateRunning=false;
    mainWin->mainArea->compl_redraw_running=false;
        if (Pick_and_Move_active==true)// || show_PickButton==2)
        {
        set_action(PICK_MOVE);
        }
        else
        {
        mainWin->cmdPick->setDown(false);
            /*if (show_PanButton==2)
            set_action(PAN_1ST);*/
        }
	break;
    case PICK_MOVE:
    Pick_and_Move_active=true;
    set_cursor(CURSOR_POINTINGHAND);
    set_left_footer_to_action(act,current_axis_restriction);
    mainWin->cmdPick->setDown(true);
    break;
    case PAN_1ST:
    set_cursor(CURSOR_OPENHAND);
    set_left_footer_to_action(act,current_axis_restriction);
    mainWin->cmdPan->setDown(true);
    break;
    case PAN_2ND:
    set_cursor(CURSOR_CLOSEDHAND);
    set_left_footer_to_action(act,current_axis_restriction);
    mainWin->cmdPan->setDown(true);
    break;
    case ZOOM_1ST:
    set_cursor(CURSOR_LINE);
    set_left_footer_to_action(act,current_axis_restriction);
    mainWin->cmdZoom->setDown(true);
	break;
    case ZOOM_2ND:
    set_left_footer_to_action(act,current_axis_restriction);
    mainWin->cmdZoom->setDown(true);
	break;
    case ZOOMX_1ST:
    set_cursor(CURSOR_LINE);
    set_left_footer_to_action(act,current_axis_restriction);
    mainWin->cmdZX->setDown(true);
	break;
    case ZOOMX_2ND:
    set_left_footer_to_action(act,current_axis_restriction);
    mainWin->cmdZX->setDown(true);
	break;
    case ZOOMY_1ST:
    set_cursor(CURSOR_LINE);
    set_left_footer_to_action(act,current_axis_restriction);
    mainWin->cmdZY->setDown(true);
	break;
    case ZOOMY_2ND:
    set_left_footer_to_action(act,current_axis_restriction);
    mainWin->cmdZY->setDown(true);
	break;
    case VIEW_1ST:
    set_cursor(CURSOR_LINE);
    set_left_footer_to_action(act,current_axis_restriction);
	break;
    case VIEW_2ND:
    set_left_footer_to_action(act,current_axis_restriction);
	break;
    case CORNER_2ND:
    set_left_footer_to_action(act,current_axis_restriction);
    break;
    case EDIT_OBJECT:
    set_cursor(CURSOR_FIND);
    set_left_footer_to_action(act,current_axis_restriction);
        if (FormDrawObjects!=NULL)
        FormDrawObjects->cmdEditObj->setDown(true);
	break;
    case DEL_OBJECT:
    set_cursor(CURSOR_KILL);
    set_left_footer_to_action(act,current_axis_restriction);
        if (FormDrawObjects!=NULL)
        FormDrawObjects->cmdDelObj->setDown(true);
	break;
    case MOVE_OBJECT_1ST:
    set_cursor(CURSOR_FIND);
    set_left_footer_to_action(act,current_axis_restriction);
        if (FormDrawObjects!=NULL)
        FormDrawObjects->cmdMoveObj->setDown(true);
	break;
    case COPY_OBJECT1ST:
    set_cursor(CURSOR_FIND);
    set_left_footer_to_action(act,current_axis_restriction);
        if (FormDrawObjects!=NULL)
        FormDrawObjects->cmdCopyObj->setDown(true);
	break;
    case MOVE_OBJECT_2ND:
    set_cursor(CURSOR_MOVE);
    set_left_footer_to_action(act,current_axis_restriction);
        if (FormDrawObjects!=NULL)
        FormDrawObjects->cmdMoveObj->setDown(true);
    break;
    case COPY_OBJECT2ND:
    set_cursor(CURSOR_MOVE);
    set_left_footer_to_action(act,current_axis_restriction);
        if (FormDrawObjects!=NULL)
        FormDrawObjects->cmdCopyObj->setDown(true);
	break;
    case STR_LOC:
    set_cursor(CURSOR_TEXT);
    set_left_footer_to_action(act,current_axis_restriction);
        if (FormDrawObjects!=NULL)
        FormDrawObjects->cmdText->setDown(true);
	break;
    case MAKE_LINE_1ST:
    set_cursor(CURSOR_LINE);
    set_left_footer_to_action(act,current_axis_restriction);
        if (FormDrawObjects!=NULL)
        FormDrawObjects->cmdLine->setDown(true);
	break;
    case MAKE_LINE_2ND:
    set_left_footer_to_action(act,current_axis_restriction);
        if (FormDrawObjects!=NULL)
        FormDrawObjects->cmdLine->setDown(true);
	break;
    case MAKE_BOX_1ST:
    set_cursor(CURSOR_LINE);
    set_left_footer_to_action(act,current_axis_restriction);
        if (FormDrawObjects!=NULL)
        FormDrawObjects->cmdBox->setDown(true);
	break;
    case MAKE_BOX_2ND:
    set_left_footer_to_action(act,current_axis_restriction);
        if (FormDrawObjects!=NULL)
        FormDrawObjects->cmdBox->setDown(true);
	break;
    case MAKE_ELLIP_1ST:
    set_cursor(CURSOR_LINE);
    set_left_footer_to_action(act,current_axis_restriction);
        if (FormDrawObjects!=NULL)
        FormDrawObjects->cmdEllipse->setDown(true);
	break;
    case MAKE_ELLIP_2ND:
    set_left_footer_to_action(act,current_axis_restriction);
        if (FormDrawObjects!=NULL)
        FormDrawObjects->cmdEllipse->setDown(true);
	break;
    case AUTO_NEAREST:
    set_cursor(CURSOR_LINE);
    set_left_footer_to_action(act,current_axis_restriction);
    mainWin->cmdAutoO->setDown(true);
	break;
    case TRACKER:
    set_cursor(CURSOR_FIND);
    set_left_footer_to_action(act,current_axis_restriction);
    point_explorer_activ=true;
        if (FormPointExplorer!=NULL)
        FormPointExplorer->cmdTrack->setDown(true);
	break;
    case DEL_POINT:
    set_cursor(CURSOR_KILL);
    set_left_footer_to_action(act,current_axis_restriction);
    point_explorer_activ=true;
        if (FormPointExplorer!=NULL)
        FormPointExplorer->cmdDelete->setDown(true);
	break;
    case MOVE_AXIS_LABEL_1ST:
    set_cursor(CURSOR_CLOSEDHAND);
    set_left_footer_to_action(act,current_axis_restriction);
    break;
    case MOVE_AXIS_LABEL_2ND:
    set_cursor(CURSOR_OPENHAND);
    set_left_footer_to_action(act,current_axis_restriction);
    break;
    case MOVE_GRAPH_1ST:
    set_cursor(CURSOR_CLOSEDHAND);
    set_left_footer_to_action(act,current_axis_restriction);
    break;
    case MOVE_GRAPH_2ND:
    set_cursor(CURSOR_OPENHAND);
    set_left_footer_to_action(act,current_axis_restriction);
    break;
    case MOVE_TITLE_1ST:
    set_cursor(CURSOR_CLOSEDHAND);
    set_left_footer_to_action(act,current_axis_restriction);
    break;
    case MOVE_TITLE_2ND:
    set_cursor(CURSOR_OPENHAND);
    set_left_footer_to_action(act,current_axis_restriction);
    break;
    case MOVE_POINT1ST:
    set_cursor(CURSOR_LINE);
    set_left_footer_to_action(act,current_axis_restriction);
    point_explorer_activ=true;
        if (FormPointExplorer!=NULL)
        {
            switch (move_dir)
            {
            case MOVE_POINT_XY:
            FormPointExplorer->cmdMove->setDown(true);
                break;
            case MOVE_POINT_X:
            FormPointExplorer->cmdMoveX->setDown(true);
                break;
            case MOVE_POINT_Y:
            FormPointExplorer->cmdMoveY->setDown(true);
                break;
            }
        }
	break;
    case MOVE_POINT2ND:
    set_left_footer_to_action(act,current_axis_restriction);
    point_explorer_activ=true;
        if (FormPointExplorer!=NULL)
        {
            switch (move_dir)
            {
            case MOVE_POINT_XY:
            FormPointExplorer->cmdMove->setDown(true);
                break;
            case MOVE_POINT_X:
            FormPointExplorer->cmdMoveX->setDown(true);
                break;
            case MOVE_POINT_Y:
            FormPointExplorer->cmdMoveY->setDown(true);
                break;
            }
        }
	break;
    case ADD_POINT:
    set_cursor(CURSOR_LINE);
    set_left_footer_to_action(act,current_axis_restriction);
    point_explorer_activ=true;
        if (FormPointExplorer!=NULL)
        {
            switch (add_at)
            {
            case ADD_POINT_BEGINNING: /* at the beginning */
            FormPointExplorer->cmdPrepend->setDown(true);
                break;
            case ADD_POINT_END: /* at the end */
            FormPointExplorer->cmdAppend->setDown(true);
                break;
            default: /* between nearest points */
            FormPointExplorer->cmdInsert->setDown(true);
                break;
            }
        }
	break;
    case PLACE_LEGEND_1ST:
    set_cursor(CURSOR_FIND);
    set_left_footer_to_action(act,current_axis_restriction);
	break;
    case PLACE_LEGEND_2ND:
    set_cursor(CURSOR_MOVE);
    set_left_footer_to_action(act,current_axis_restriction);
	break;
    case PLACE_TIMESTAMP_1ST:
    set_cursor(CURSOR_FIND);
    set_left_footer_to_action(act,current_axis_restriction);
	break;
    case PLACE_TIMESTAMP_2ND:
    set_cursor(CURSOR_MOVE);
    set_left_footer_to_action(act,current_axis_restriction);
	break;
    case SEL_POINT:
    set_cursor(CURSOR_LINE);
    set_left_footer_to_action(act,current_axis_restriction);
	break;
    case DEF_REGION1ST:
    set_cursor(CURSOR_LINE);
    set_left_footer_to_action(act,current_axis_restriction);
	break;
    case DEF_REGION2ND:
    set_left_footer_to_action(act,current_axis_restriction);
	break;
    case DEF_REGION:
    set_cursor(CURSOR_LINE);
    set_left_footer_to_action(act,current_axis_restriction);
	break;
    case COMP_AREA:
    set_cursor(CURSOR_LINE);
    set_left_footer_to_action(act,current_axis_restriction);
	break;
    case COMP_PERIMETER:
    set_cursor(CURSOR_LINE);
    set_left_footer_to_action(act,current_axis_restriction);
	break;
    case DISLINE1ST:
    set_cursor(CURSOR_LINE);
    set_left_footer_to_action(act,current_axis_restriction);
	break;
    case DISLINE2ND:
    set_cursor(CURSOR_LINE);
    set_left_footer_to_action(act,current_axis_restriction);
	break;
    }
    action_flag = act;
    /*if (act==DO_NOTHING && show_PickButton==2)
    {
    set_action(PICK_MOVE);
    }
    else if (act==DO_NOTHING && show_PanButton==2)
    {
    set_action(PAN_1ST);
    }*/
}

CanvasAction get_action(void)
{
return action_flag;
}

void track_point(int gno, int setno, int *loc, int shift)
{
    int len;
    double *xtmp, *ytmp;
    WPoint wp;
    VPoint vp;
    world w;
    if ((len = getsetlength(gno, setno)) > 0) {
        *loc += shift;
        if (*loc < 0) {
            *loc += len;
        } else {
            *loc = *loc % len;
        }
        xtmp = getx(gno, setno);
        ytmp = gety(gno, setno);
        wp.x = xtmp[*loc];
        wp.y = ytmp[*loc];
       
        get_graph_world(gno, &w);
        wp.x = MAX2(wp.x, w.xg1);
        wp.x = MIN2(wp.x, w.xg2);
        wp.y = MAX2(wp.y, w.yg1);
        wp.y = MIN2(wp.y, w.yg2);
        vp = Wpoint2Vpoint(wp);

        setpointer(vp);
        if (FormPointExplorer!=NULL)
        FormPointExplorer->update_point_locator(gno, setno, *loc);
    }
}

/*
 * set format string for locator
 */
static const char *typestr[6] = {"X, Y",
                                 "DX, DY",
                                 "DIST",
                                 "Phi, Rho",
                                 "VX, VY",
                                 "SX, SY"};

/*
 * locator on main_panel
 */
void getpoints(VPoint *vpp)
{
    static VPoint vp = {0.0, 0.0};
    int cg = get_cg();
    double wx, wy, xtmp, ytmp;
    int x, y;
    double dsx = 0.0, dsy = 0.0;
    char buf[256], bufx[64], bufy[64], *s;
    GLocator locator;
    int formx = 0;
    int formy = 0;

    if (vpp != NULL)
    {
    vp = *vpp;
    }
    
    view2world(vp.x, vp.y, &wx, &wy);
        if (get_graph_locator(cg, &locator) != RETURN_SUCCESS)
        {
        mainWin->statLocBar->setText(QObject::tr("[No graphs]"));
        return;
        }
    
    if (locator.pointset)
    {
	dsx = locator.dsx;
	dsy = locator.dsy;
    }

    formx = locator.fx;
    formy = locator.fy;

    switch (locator.pt_type) {
    case 0://X,Y
        if (get_graph_type(cg) == GRAPH_POLAR) {
            polar2xy(wx, wy, &xtmp, &ytmp);
        } else if (get_graph_type(cg) == GRAPH_POLAR2) {
            //polar2xy2(wx, wy, &xtmp, &ytmp);
            xtmp = wx;
            ytmp = wy;
            if (xtmp<0.0) xtmp+=2*M_PI;
        } else {
            xtmp = wx;
            ytmp = wy;
        }
        break;
    case 1://DX,DY
        xtmp = wx - dsx;
        ytmp = wy - dsy;
        if (locator.pointset)
        {
            if (formx == FORMAT_YYMMDDHMS)
            {
            formx = FORMAT_DECIMAL;
            xtmp = xtmp*(24.0*3600.0); //scale to seconds
            }
            if (formy == FORMAT_YYMMDDHMS)
            {
            formy = FORMAT_DECIMAL;
            ytmp = ytmp*(24.0*3600.0); //scale to seconds
            }
        }
        break;
    case 2://Distance
        if (get_graph_type(cg) == GRAPH_POLAR) {
            polar2xy(wx, wy, &xtmp, &ytmp);
        } /*else if (get_graph_type(cg) == GRAPH_POLAR2) {
            polar2xy2(wx, wy, &xtmp, &ytmp);
        }*/ else {
            xtmp = wx;
            ytmp = wy;
        }
        xtmp = hypot(dsx - xtmp, dsy - ytmp);
        ytmp = 0.0;
        break;
    case 3://Phi,Rho
        /*if (get_graph_type(cg) == GRAPH_POLAR2)
        {
            xy2polar2(wx - dsx, wy - dsy, &xtmp, &ytmp);
        }
        else*/ if (dsx - wx != 0.0 || dsy - wy != 0.0) {
            xy2polar(wx - dsx, wy - dsy, &xtmp, &ytmp);
        } else {
            xtmp = 0.0;
            ytmp = 0.0;
        }
        break;
    case 4://Vx,Vy
        xtmp = vp.x;
        ytmp = vp.y;
        break;
    case 5://Sx,Sy
        xlibVPoint2dev(vp, &x, &y);
        xtmp = x;
        ytmp = y;
        break;
    default:
        return;
    }

    s = create_fstring(formx, locator.px, xtmp, LFORMAT_TYPE_PLAIN);
    strcpy(bufx, s);
    s = create_fstring(formy, locator.py, ytmp, LFORMAT_TYPE_PLAIN);
    strcpy(bufy, s);

    if (locator.pt_type==2)//Distance
    {
    sprintf(buf, "G%1d: %s = [%s]", cg, typestr[locator.pt_type], bufx);//y=0 (always)
    }
    else
    {
        if (DecimalPointToUse==',')
        {
            if (locator.fx>=FORMAT_DECIMAL && locator.fx<=FORMAT_COMPUTING)
            SetDecimalSeparatorToUserValue(bufx);
            if (locator.fy>=FORMAT_DECIMAL && locator.fy<=FORMAT_COMPUTING)
            SetDecimalSeparatorToUserValue(bufy);
        sprintf(buf, "G%1d: %s = [%s | %s]", cg, typestr[locator.pt_type], bufx, bufy);
        }
        else
        sprintf(buf, "G%1d: %s = [%s, %s]", cg, typestr[locator.pt_type], bufx, bufy);
    }
    mainWin->statLocBar->setText(QString(buf));
}

/*
 * switch on the area calculator
 */
void do_select_area(void)
{
    set_action(DO_NOTHING);
    set_action(COMP_AREA);
}

/*
 * switch on the perimeter calculator
 */
void do_select_peri(void)
{
    set_action(DO_NOTHING);
    set_action(COMP_PERIMETER);
}

void do_dist_proc(void)
{
    set_action(DO_NOTHING);
    set_action(DISLINE1ST);
}

/*
 * define a (polygon) region
 */
void do_select_region(void)
{
    region_pts = 0;
    set_action(DO_NOTHING);
    set_action(DEF_REGION);
}

/*
 * Given the graph gno, find the graph that contains
 * (wx, wy). Used for setting the graph focus.
 */
int next_graph_containing(int cg, VPoint vp)
{
    int i, j, ng, gno = -1;
    view v;
    ng = number_of_graphs();
    if (is_valid_gno(cg) == FALSE)
    {
        cg = -1;
    }
    for (i = 0; i < ng ; i++)
    {
	j = (i + cg + 1) % ng;
    get_graph_viewport(j, &v);
//qDebug() << "searching in Graph " << j << " viewport= x:[" << v.xv1 << "-" << v.xv2 << "] y:[" << v.yv1 << "-" << v.yv2 << "]";
        if (is_graph_hidden(j) == FALSE &&
            get_graph_viewport(j, &v) == RETURN_SUCCESS &&
            is_vpoint_inside(v, vp, MAXPICKDIST) == TRUE)
        {
            gno = j;
            break;
        }
    }
    return gno;
}

/*
 * Returns the graph that truly contains the vp (returns -1 if no graph found)
 * searches first in the current graph, then all other graphs
 */
int get_graph_truly_containing_vp(VPoint vp)
{
int gno=-1;
int current=get_cg();
int ng=number_of_graphs();
int ok;
view v;
    //look first inside the current graph
    if (is_graph_hidden(current) == FALSE)
    {
    ok=get_graph_viewport(current, &v);
        if (ok==RETURN_SUCCESS)
        {
            if (is_vpoint_truly_inside(v, vp) == TRUE)
            gno=current;
        }
    }
    if (gno>=0) return gno;
    //look in the other graphs as well
    for (int i=0;i<ng;i++)
    {
        if (i==current || is_graph_hidden(i)==TRUE) continue;
        ok=get_graph_viewport(i, &v);
        if (is_vpoint_truly_inside(v, vp) == TRUE)
        {
        gno=i;
        break;
        }
    }
return gno;
}

int legend_clicked(int gno, VPoint vp, view *bb)
{
    if (hideDialog() && inline_editing==FALSE)
    return FALSE; // hide dialog, if point explorer window is open.

    legend l;
    if (is_graph_hidden(gno) == FALSE) {
        get_graph_legend(gno, &l);
        if (l.autoattach!=G_LB_ATTACH_NONE)
        {
        l.bb.xv1+=l.xshift;
        l.bb.xv2+=l.xshift;
        l.bb.yv1+=l.yshift;
        l.bb.yv2+=l.yshift;
        }
	if (l.active && is_vpoint_inside(l.bb, vp, MAXPICKDIST)) {
	    *bb = l.bb;
            return TRUE;
	} else {
            return FALSE;
        }
    } else {
        return FALSE;
    }
}

int graph_clicked(int gno, VPoint vp)
{
    if (hideDialog() && inline_editing==FALSE)
    return FALSE; // hide dialog, if point explorer window is open.

    view v;
    if (is_graph_hidden(gno) == FALSE) {
        get_graph_viewport(gno, &v);
	if (is_vpoint_inside(v, vp, MAXPICKDIST)) {
            return TRUE;
	} else {
            return FALSE;
        }
    } else {
        return FALSE;
    }
}

int timestamp_clicked(VPoint vp, view *bb)
{
    if (hideDialog())
    return FALSE; // hide dialog, if point explorer window is open.

    if (timestamp.active && is_vpoint_inside(timestamp.bb, vp, MAXPICKDIST)) {
        *bb = timestamp.bb;
        return TRUE;
    } else {
        return FALSE;
    }
}

int focus_clicked(int cg, VPoint vp, VPoint *avp)
{
    if (hideDialog())
    return FALSE; // hide dialog, if point explorer window is open.

    view v;
    if (is_graph_hidden(cg) == TRUE) {
        return FALSE;
    }
    if (get_graph_viewport(cg, &v) != RETURN_SUCCESS) {
        return FALSE;
    }
    if (fabs(vp.x - v.xv1) < MAXPICKDIST && fabs(vp.y - v.yv1) < MAXPICKDIST) {
        avp->x = v.xv2;
        avp->y = v.yv2;
        return TRUE;
    } else if (fabs(vp.x - v.xv1) < MAXPICKDIST && fabs(vp.y - v.yv2) < MAXPICKDIST) {
        avp->x = v.xv2;
        avp->y = v.yv1;
        return TRUE;
    } else if (fabs(vp.x - v.xv2) < MAXPICKDIST && fabs(vp.y - v.yv1) < MAXPICKDIST) {
        avp->x = v.xv1;
        avp->y = v.yv2;
        return TRUE;
    } else if (fabs(vp.x - v.xv2) < MAXPICKDIST && fabs(vp.y - v.yv2) < MAXPICKDIST) {
        avp->x = v.xv1;
        avp->y = v.yv1;
        return TRUE;
    } else {
        return FALSE;
    }
}

int lower_right_corner_clicked(VPoint vp)
{
static double xv,yv;
    get_page_viewport(&xv,&yv);
    yv=0.0;//lower right  corner is y=0 and x=max
    if (fabs(vp.x - xv) < MAXPICKDIST && fabs(vp.y - yv) < MAXPICKDIST)
        return TRUE;
    else
        return FALSE;
}

int axis_clicked(int gno, VPoint vp, int *axisno)//work only if clicked directely on the axis-line
{
    if (hideDialog())
    return FALSE; // hide dialog, if point explorer window is open.

    view v;
    /* TODO: check for offsets, zero axes, polar graphs */
    if (is_graph_hidden(gno) == TRUE) {
        return FALSE;
    } else {
        get_graph_viewport(gno, &v);
        if (vp.x >= v.xv1 && vp.x <= v.xv2 &&
            (fabs(vp.y - v.yv1) < MAXPICKDIST ||
             fabs(vp.y - v.yv2) < MAXPICKDIST)) {
            *axisno = X_AXIS;
            return TRUE;
        } else if (vp.y >= v.yv1 && vp.y <= v.yv2 &&
            (fabs(vp.x - v.xv1) < MAXPICKDIST ||
             fabs(vp.x - v.xv2) < MAXPICKDIST)) {
            *axisno = Y_AXIS;
            return TRUE;
        } else {
            return FALSE;
        }
    }
}

int axis_custom_tick_clicked(int gno, VPoint vp, int * axisno, int * tick_nr)//only works with custom tick labels
{
    if (hideDialog() || !is_valid_gno(gno))
    return FALSE; // hide dialog, if point explorer window is open.
*tick_nr=-1;
*axisno=-1;
    /* TODO: check for offsets, zero axes, polar graphs */
    if (is_graph_hidden(gno) == TRUE) {
        return FALSE;
    } else {
        for (int i=0;i<MAXAXES;i++)
        {
        if (g[gno].t[i]->t_spec==0 || g[gno].t[i]->nticks<=0) continue;//no special ticks here
            for (int j=0;j<g[gno].t[i]->nticks;j++)
            {
                if (is_vpoint_inside(g[gno].t[i]->tloc[j].bb,vp,MAXPICKDIST))//clicked in tick-label
                {
                *tick_nr=j;
                *axisno=i;
                break;
                }
            }
            if (*axisno>=0) break;
        }
    }
if (*axisno>=0) return TRUE;
else return FALSE;
}

int get_axis_label_viewport(int gno, int ax,view * vp,bool for_click,int * rot,int alt)//for_click=true: returns a viewport that can be used for checking if the user clicked in it; =false: used for inline-editing; alt=0 --> return normal position if BOTH is set, alt=1 --> return opposite if BOTH is set
{
static const double v_height=0.03;
(void) for_click;
if (!is_valid_axis(gno,ax) || !is_axis_active(gno,ax)) return RETURN_FAILURE;
bool is_y_axis=is_yaxis(ax);
view v,v2;
get_graph_viewport(gno, &v);
*rot=0.0;
tickmarks * tick=get_graph_tickmarks(gno,ax);
if (tick==NULL) return RETURN_FAILURE;
/*
qDebug() << "gno=" << gno << " ax=" << ax <<
            " layout=" << tick->label_layout <<
            " label_place=" << tick->label_place <<
            " type=" << (tick->label_op==PLACEMENT_NORMAL?"Normal":(tick->label_op==PLACEMENT_OPPOSITE?"Opposite":"BOTH"));
*/
/*int label_layout;      // axis label orientation (h or v) */
/*int label_place;       // axis label placement (specfied or auto) */
/*PlacementType label_op;// tick labels on opposite side or both */

    if (!is_y_axis)
    {
    //x-axis
        if (tick->label_layout==0)
        {
        *rot=0.0;
        }
        else
        {
        *rot=90.0;
        }
    }
    else
    {
    //y-axis
        if (tick->label_layout==0)
        {
        *rot=90.0;
        }
        else
        {
        *rot=0.0;
        }
    }

*vp=tick->al_bb;
if (vp->xv1==vp->xv2 && vp->xv1==0.0)//no valid viewport set --> return default click-area
{
    if (!is_y_axis)
    {
    //x-axis
    v2.xv1=v.xv1;//v is the viewport of the graph
    v2.xv2=v.xv2;//for the x-axis, the relevant area is the whole width of the graph
        if (tick->label_op==PLACEMENT_NORMAL || (tick->label_op==PLACEMENT_BOTH && alt==0))
        {
        v2.yv2=tick->al_vp.y;//regular axis-label-start-position
        v2.yv1=v2.yv2-v_height;
        }
        else
        {
        v2.yv1=tick->al_vp.y;//opposite axis-label-start-position
        v2.yv2=v2.yv1+v_height;
        }
    }
    else
    {
    //y-axis
    v2.yv1=v.yv1;//v is the viewport of the graph
    v2.yv2=v.yv2;//for the y-axis, the relevant area is the whole height of the graph
        if (tick->label_op==PLACEMENT_NORMAL || (tick->label_op==PLACEMENT_BOTH && alt==0))
        {
        v2.xv2=tick->al_vp.x;
        v2.xv1=v2.xv2-v_height;
        }
        else
        {
        v2.xv1=tick->al_vp.x;
        v2.xv2=v2.xv1+v_height;
        }
    }
*vp=v2;
}
//qDebug() << vp->xv1 << "|" << vp->yv1 << " -- " << vp->xv2 << "|" << vp->yv2;
return RETURN_SUCCESS;
}

int axis_label_clicked(int *gno, VPoint vp, int *axisno,int * alt)//only works if someone clicks on an axis-label (not tick labels!), alt=0=normal position, alt=1=opposite position
{
    if (hideDialog() && inline_editing==FALSE)
    return FALSE; // hide dialog, if point explorer window is open.
//const double v_height=0.03;
*gno=-1;
*axisno=-1;
*alt=0;
    //view v,v2;
    view v3;
    int rot;
///qDebug() << "Search for vp=" << vp.x << " | " << vp.y;
    for (int i=0;i<number_of_graphs();i++)
    {
        ///We only check for the normal position (not in the oposite position)
        if (is_graph_hidden(i) == TRUE) continue;
            for (int j=0;j<MAXAXES;j++)
            {
            if (get_axis_label_viewport(i,j,&v3,true,&rot,0)==RETURN_FAILURE) continue;
                if (is_vpoint_inside(v3,vp,MAXPICKDIST))
                {
                *gno=i;
                *axisno=j;
                *alt=0;
                break;
                }
            if (get_graph_tickmarks(i,j)->label_op==PLACEMENT_BOTH)
            {
            if (get_axis_label_viewport(i,j,&v3,true,&rot,1)==RETURN_FAILURE) continue;
                if (is_vpoint_inside(v3,vp,MAXPICKDIST))
                {
                *gno=i;
                *axisno=j;
                *alt=1;
                break;
                }
            }
            }//end of axis-loop
    }//end of graphs-loop
if (*gno>=0) return TRUE;
else return FALSE;
}

int title_clicked(int gno, VPoint vp, int * lower)
{
    if (hideDialog() && inline_editing==FALSE)
    return FALSE; // hide dialog, if point explorer window is open.

    view v;
    /* a rude check; TODO: use right offsets */
    if (is_graph_hidden(gno) == TRUE) {
        return FALSE;
    } else {
        get_graph_viewport(gno, &v);
        if (vp.x >= v.xv1 && vp.x <= v.xv2 &&
            vp.y > v.yv2 && vp.y < v.yv2 + 0.1) {
                if (vp.y < v.yv2 + 0.05) *lower=1;
                else *lower=0;
            return TRUE;
        } else {
            *lower=0;
            return FALSE;
        }
    }
}

/*
 * locate a point and the set the point is in
 */
int find_point(int gno, VPoint vp, int *setno, int *loc)
{
    int i, start, stop, j, found=FALSE;
    double *xtmp, *ytmp;
    WPoint wptmp;
    VPoint vptmp;
    double dist, mindist = MAXPICKDIST;
    if (is_valid_gno(gno) != TRUE) {
        return RETURN_FAILURE;
    }
    if (is_valid_setno(gno, *setno)) {
        start = *setno;
        stop = *setno;
    } else {
        start = 0;
        stop = number_of_sets(gno) - 1;
    }
    found = FALSE;
    for (i = start; i <= stop; i++) {
        if (is_set_hidden(gno, i) == FALSE) {
	    xtmp = getx(gno, i);
	    ytmp = gety(gno, i);
            for (j = 0; j < getsetlength(gno, i); j++) {
            wptmp.x = xtmp[j];
            wptmp.y = ytmp[j];
            vptmp = Wpoint2Vpoint(wptmp);
            dist = MAX2(fabs(vp.x - vptmp.x), fabs(vp.y - vptmp.y));
                if (dist < mindist) {
                found = TRUE;
                *setno = i;
                *loc = j;
                mindist = dist;
                }
            }
        }
    }
    
    if (found == FALSE) {
        return RETURN_FAILURE;
    } else {
        if (FormPointExplorer!=NULL)
        FormPointExplorer->update_point_locator(gno, *setno, *loc);
        return RETURN_SUCCESS;
    }
}

int find_insert_location(int gno, int setno, VPoint vp)
{
    int j, loc = -1;
    double *xtmp, *ytmp;
    WPoint wptmp;
    VPoint vp1, vp2;
    double dist, mindist = 1.0, lowdist=-1.0;
    int lowdistloc=-1;
    wptmp=Vpoint2Wpoint(vp);
//qDebug() << "find nearest: G" << gno << ".S" << setno << " vp=" << vp.x << "|" << vp.y;
    if (is_valid_setno(gno, setno) == TRUE) {
        if (is_set_hidden(gno, setno) == FALSE) {
            xtmp = getx(gno, setno);
            ytmp = gety(gno, setno);
            if (getsetlength(gno, setno)==0)//no points so far
            {
            return 0;
            }
            else if (getsetlength(gno, setno)<2)//only one point
            {
                if (wptmp.x<xtmp[0])
                return 0;
                else
                return 1;
            }
            //at least two points
            for (j = 0; j < getsetlength(gno, setno) - 1; j++) {
                wptmp.x = xtmp[j];
                wptmp.y = ytmp[j];
                vp1 = Wpoint2Vpoint(wptmp);
                wptmp.x = xtmp[j + 1];
                wptmp.y = ytmp[j + 1];
                vp2 = Wpoint2Vpoint(wptmp);
 
                dist = hypot(vp.x - vp1.x, vp.y - vp1.y) +
                       hypot(vp.x - vp2.x, vp.y - vp2.y);
//qDebug() << j << "dist=" << dist;
                if (dist < mindist) {
                    loc = j + 1;
                    mindist = dist;
                }
                if (lowdist==-1 || dist<lowdist)
                {
                lowdistloc=j+1;
                lowdist=dist;
                }
            }
            if (loc==-1)//nothing found --> too far away
            {
            loc=lowdistloc;
            }
        }
    }
    
    return loc;
}

/*
 * find object containing vp inside its bb
 */
int find_item(int gno, VPoint vp, view *bb, int *type, int *id)
{
    (void)gno;
    int i;
    *type = OBJECT_NONE;
    for (i = 0; i < number_of_boxes(); i++) {
	if (isactive_box(i)) {
            get_object_bb(OBJECT_BOX, i, bb);
	    if (is_vpoint_inside(*bb, vp, MAXPICKDIST)) {
		*type = OBJECT_BOX;
		*id = i;
	    }
	}
    }
    for (i = 0; i < number_of_ellipses(); i++) {
	if (isactive_ellipse(i)) {
            get_object_bb(OBJECT_ELLIPSE, i, bb);
	    if (is_vpoint_inside(*bb, vp, MAXPICKDIST)) {
		*type = OBJECT_ELLIPSE;
		*id = i;
	    }
	}
    }
    for (i = 0; i < number_of_lines(); i++) {
	if (isactive_line(i)) {
            get_object_bb(OBJECT_LINE, i, bb);
	    if (is_vpoint_inside(*bb, vp, MAXPICKDIST)) {
		*type = OBJECT_LINE;
		*id = i;
	    }
	}
    }
    for (i = 0; i < number_of_strings(); i++) {
	if (isactive_string(i)) {
            get_object_bb(OBJECT_STRING, i, bb);
	    if (is_vpoint_inside(*bb, vp, MAXPICKDIST)) {
		*type = OBJECT_STRING;
		*id = i;
	    }
	}
    }
    
    if (*type == OBJECT_NONE) {
        return RETURN_FAILURE;
    } else {
        get_object_bb(*type, *id, bb);
        return RETURN_SUCCESS;
    }
}

/*
 * for zooms
 *
 */
void newworld(int gno, int axes, VPoint vp1, VPoint vp2)
{
    world w, wtmp;

    if (vp1.x == vp2.x && (axes == ALL_AXES || axes == ALL_X_AXES)) {
        errmsg("Zoomed rectangle is zero along X, zoom cancelled");
        return;
    }

    if (vp1.y == vp2.y && (axes == ALL_AXES || axes == ALL_Y_AXES)) {
        errmsg("Zoomed rectangle is zero along Y, zoom cancelled");
        return;
    }

    view2world(vp1.x, vp1.y, &w.xg1, &w.yg1);
    view2world(vp2.x, vp2.y, &w.xg2, &w.yg2);
    if (w.xg1 > w.xg2) {
        fswap(&w.xg1, &w.xg2);
    }
    if (w.yg1 > w.yg2) {
        fswap(&w.yg1, &w.yg2);
    }

    if (is_graph_active(gno))
    {
        get_graph_world(gno, &wtmp);
        switch (axes)
        {
        case ALL_AXES:
            wtmp.xg1 = w.xg1;
            wtmp.xg2 = w.xg2;
            wtmp.yg1 = w.yg1;
            wtmp.yg2 = w.yg2;
            break;
        case ALL_X_AXES:
            wtmp.xg1 = w.xg1;
            wtmp.xg2 = w.xg2;
            break;
        case ALL_Y_AXES:
            wtmp.yg1 = w.yg1;
            wtmp.yg2 = w.yg2;
            break;
        default:
            return;
            break;
        }
        set_graph_world(gno, wtmp);
        autotick_axis(gno, axes);
    }
    mainWin->mainArea->completeRedraw();
}

void switch_current_graph(int gno)
{
    if (is_graph_hidden(gno) == FALSE)
    {
        update_StdSelectors(gno);
        //qDebug() << "selecting " << gno;
        select_graph(gno);
        //cout << "setGraphSelector" << endl;
        set_graph_selectors(gno);
        //cout << "getpoints" << endl;
        getpoints(NULL);
        //cout << "going to redraw" << endl;
        //qDebug() << "switch cg=" << get_cg();
	mainWin->mainArea->completeRedraw();
    }
}

/* -------------------------------------------------------------- */
/* canvas_actions */
void autoscale_action(void)
{
    int cg = get_cg();
    
    autoscale_graph(cg, AUTOSCALE_XY);
    if (FormAxisProperties!=NULL)
    FormAxisProperties->update_ticks(cg);
mainWin->mainArea->completeRedraw();
}

void autoscale_on_near_action(void)
{
    set_action(DO_NOTHING);
    set_action(AUTO_NEAREST);       
}

void draw_line_action(void)
{
    set_action(DO_NOTHING);
    set_action(MAKE_LINE_1ST);
}

void draw_box_action(void)
{
    set_action(DO_NOTHING);
    set_action(MAKE_BOX_1ST);       
}

void draw_ellipse_action(void)
{
    set_action(DO_NOTHING);
    set_action(MAKE_ELLIP_1ST);
}

void write_string_action(void)
{
    set_action(DO_NOTHING);
    set_action(STR_LOC);
}

void delete_object_action(void)
{
    set_action(DO_NOTHING);
    set_action(DEL_OBJECT); 
}

void place_legend_action(void)
{
    set_action(DO_NOTHING);
    set_action(PLACE_LEGEND_1ST);
}

void place_timestamp_action(void)
{
    set_action(DO_NOTHING);
    set_action(PLACE_TIMESTAMP_1ST);
}

void move_object_action(void)
{
    set_action(DO_NOTHING);
    set_action(MOVE_OBJECT_1ST);
}

void refresh_hotlink_action(void)
{
    //do_hotupdate_proc();
    //this has been included directly in the event loop above
}

void set_viewport_action(void)
{
    set_action(DO_NOTHING);
    set_action(VIEW_1ST);
}

void enable_zoom_action(void)
{
    set_action(DO_NOTHING);
    set_action(ZOOM_1ST);
}

/*
 * world stack operations
 */
void push_and_zoom(void)
{
    push_world();
    set_action(DO_NOTHING);
    set_action(ZOOM_1ST);
}

int hideDialog(void)//this function is currently useless!
{
    /// TEST-VERSION -- IS ALWAYS FALSE
    return FALSE;
    if(FormPointExplorer!=NULL)
    {
        if(FormPointExplorer->isVisible())
        {
        return TRUE;
        }
        else
        {
        return FALSE;
        }
    }
    else
    {
    return FALSE;
    }
}

ClickTarget getClickTarget(VPoint vp, view * bb,int * parameters)
{
ClickTarget ct=TARGET_NOTHING;
int nrg=number_of_graphs();
int next_gr_cont;
//qDebug() << "Searching Click Target At (" << vp.x << "|" << vp.y << ")";
/*
   parameters[0]=GraphID
   parameters[1]=AxisID
   parameters[2]=Tick-Nr
*/
bool custom_tick_found=false;
for (int i=0;i<nrg;i++)
{
    if (is_graph_hidden(i)==TRUE) continue;
    if (axis_custom_tick_clicked(i,vp,parameters+1,parameters+2)==TRUE)
    {
    parameters[0]=i;
    custom_tick_found=true;
    break;
    }
}
if (custom_tick_found==true) return ct;

if  (timestamp_clicked(vp,bb)==TRUE)
{
ct=TARGET_TIMESTAMP;
}
else if (find_item(0,vp,bb,parameters+0,parameters+1)==RETURN_SUCCESS)
{
ct=TARGET_OBJECT;
/* parameters[0]=Type
   parameters[1]=ID*/
}
else if (axis_label_clicked(parameters,vp,parameters+1,parameters+2)==TRUE)
{
ct=TARGET_AXIS_LABEL;
/* parameters[0]=GraphID
   parameters[1]=axis-nr*/
*bb=g[parameters[0]].t[parameters[1]]->al_bb;
}
else if (clicked_in_legend(vp,parameters+0,parameters+1,bb)==RETURN_SUCCESS)
{
ct=TARGET_LEGEND;
/* parameters[0]=GraphID
   parameters[1]=SetID*/
}

if (ct==TARGET_NOTHING)//nothing found yet-->search further in the different graphs
{
next_gr_cont=next_graph_containing(get_cg(),vp);
//qDebug() << "nextGraphContainingPoint=" << next_gr_cont;
for (int i=0;i<nrg;i++)
{
    if (title_clicked(i,vp,parameters+1)==TRUE)
    {
    ct=TARGET_TITLE;
    parameters[0]=i;
    /* parameters[0]=GraphID
       parameters[1]: 0=Title, 1=SubTitle*/
    break;
    }
    else if (axis_clicked(i,vp,parameters+1)==TRUE)
    {
    ct=TARGET_AXIS;
    parameters[0]=i;
    get_graph_viewport(parameters[0],bb);
    /* parameters[0]=GraphID
       parameters[1]=axis-nr*/
    break;
    }
    else if (legend_clicked(i,vp,bb)==TRUE)
    {
    ct=TARGET_LEGEND_BOX;
    parameters[0]=i;
    /* parameters[0]=GraphID*/
    break;
    }
    else if (next_gr_cont==i && find_point(i,vp,parameters+1,parameters+2)==RETURN_SUCCESS)
    {
    ct=TARGET_SET;
    parameters[0]=i;
    /* parameters[0]=GraphID
       parameters[1]=SetID
       parameters[2]=Point-Location-In-Set*/
    //qDebug() << "set found: Parameters=" << parameters[0] << " " << parameters[1] << " " << parameters[2];
    }
}//end for every graph
}//end ct=TargetNorthing
if (ct==TARGET_NOTHING)//nothing specific found-->at least find the graph someone clicked into
{
parameters[0]=next_graph_containing(get_cg(),vp);
//qDebug() << "Nothing found yet g=" << parameters[0];
    if (parameters[0]>=0)
    {
    ct=TARGET_GRAPH;
    get_graph_viewport(parameters[0],bb);
    }
}
//qDebug() << (int)ct << "Parameters=" << parameters[0] << " " << parameters[1] << " " << parameters[2];
return ct;
}

void debugClickTarget(ClickTarget ct, view bb, int * parameters)
{
    switch (ct)
    {
    case TARGET_TIMESTAMP:
    qDebug() << "Timestamp clicked; bb=" << bb.xv1 << "|" << bb.yv1 << "-" << bb.xv2 << "|" << bb.yv2;
        break;
    case TARGET_GRAPH:
    qDebug() << "Graph clicked; G" << parameters[0];
        break;
    case TARGET_SET:
    qDebug() << "Set clicked; G" << parameters[0] << ".S" << parameters[1] << "[" << parameters[2] << "]";
        break;
    case TARGET_OBJECT:
    qDebug() << "Object clicked; Type=" << parameters[0] << "; ID=" << parameters[1] << "; bb=" << bb.xv1 << "|" << bb.yv1 << "-" << bb.xv2 << "|" << bb.yv2;
        break;
    case TARGET_AXIS:
    qDebug() << "Axis clicked; G" << parameters[0] << " AxisNr=" << parameters[1];
        break;
    case TARGET_AXIS_LABEL:
    qDebug() << "Axis Label clicked; G" << parameters[0] << " AxisNr=" << parameters[1] << "; bb=" << bb.xv1 << "|" << bb.yv1 << "-" << bb.xv2 << "|" << bb.yv2;
        break;
    case TARGET_TITLE:
    qDebug() << "Graph Title clicked; G" << parameters[0] << " Type=" << (parameters[1]==0?"Title":"SubTitle");
        break;
    case TARGET_LEGEND:
    qDebug() << "Legend clicked; G" << parameters[0] << " Set=" << parameters[1];
        break;
    default:
    qDebug() << "NO CLICK TARGET FOUND";
        break;
    }
}

void UpdateCursor(VPoint vp)
{
static QCursor loleft(Qt::SizeBDiagCursor);
static QCursor upleft(Qt::SizeFDiagCursor);
/*static QCursor arrow(Qt::ArrowCursor);
static QCursor cross(Qt::CrossCursor);*/
VPoint avp={-100,-100};
view v;
int is_focus=focus_clicked(get_cg(),vp,&avp);
int is_lower_right=lower_right_corner_clicked(vp);
if (is_focus==FALSE && is_lower_right==TRUE)
{
    mainWin->mainArea->setCursor(upleft);
}
else if (is_focus==TRUE && is_lower_right==FALSE)
{
(void)get_graph_viewport(get_cg(),&v);
    if (avp.x==v.xv1 && avp.y==v.yv1)
    mainWin->mainArea->setCursor(loleft);//qDebug() << "UpperRight";
    else if (avp.x==v.xv1 && avp.y==v.yv2)
    mainWin->mainArea->setCursor(upleft);//qDebug() << "LowerRight";
    else if (avp.x==v.xv2 && avp.y==v.yv1)
    mainWin->mainArea->setCursor(upleft);//qDebug() << "UpperLeft";
    else if (avp.x==v.xv2 && avp.y==v.yv2)
    mainWin->mainArea->setCursor(loleft);//qDebug() << "LowerLeft";
}
else
set_cursor(cur_cursor);
/*
else if (cursortype!=0 && mainWin->mainArea->cursor().shape()!=Qt::CrossCursor)
{
    mainWin->mainArea->setCursor(cross);
}
else if (cursortype==0 && mainWin->mainArea->cursor().shape()!=Qt::ArrowCursor)
{
    mainWin->mainArea->setCursor(arrow);
}*/

}
