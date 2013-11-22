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
 * Modified by Andreas Winter 2008-2012
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
#ifdef _MSC_VER
#else
#include <unistd.h>
#endif
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
#include "undo_module.h"

extern MainWindow * mainWin;
extern frmSetAppearance * FormSetAppearance;
extern frmPlotAppearance * FormPlotAppearance;
extern frmAxisProp * FormAxisProperties;
extern frmGraphApp * FormGraphAppearance;
extern frmPointExplorer * FormPointExplorer;
extern frmLocatorProps * FormLocatorProps;
extern void hide_rubber_lines(void);
extern int object_edit_popup(int obj, int id);
extern int region_def_under_way;
extern int regiontype;
extern QPoint VPoint2XPoint(VPoint vp);
extern unsigned int win_h, win_w;
#define win_scale ((win_h < win_w) ? win_h:win_w)

/*#include <X11/X.h>
#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/keysym.h>

#include "motifinc.h"*/

/*extern Widget loclab;
extern Widget arealab;
extern Widget perimlab;*/

int cursortype = 0;
//QTime lastc=QTime::currentTime();  /* time of last mouse click */
QRect ShiftRect;
QPoint ShiftPoint;
static VPoint anchor_vp = {0.0, 0.0};
static int x, y;                /* pointer coordinates */
int anchor_x = 0;
int anchor_y = 0;
static view bb;
static int move_dir;
extern int action_flag;

/*
 * for region, area and perimeter computation
 */
#define MAX_POLY_POINTS 200
static int region_pts = 0;
static int iax[MAX_POLY_POINTS];
static int iay[MAX_POLY_POINTS];
static WPoint region_wps[MAX_POLY_POINTS];

using namespace std;

extern void get_tracking_props(int *setno, int *move_dir, int *add_at);
extern void set_graph_selectors(int gno);
extern void do_hotupdate_proc(void);

extern void GeneralPaste(const QMimeData * mimeData);

//this is for simple use of the undo-stuff - to be used with single objects only!
int nrOfUndoObj=1;
int undoObjIds[2];
Datapoint * undo_dp=NULL;
int * undo_nrs=NULL;
region undo_region,undo_region2;
extern bool immediateUpdate;
extern bool updateRunning;

void anchor_point(int curx, int cury, VPoint curvp)
{
     anchor_vp = curvp;
     anchor_x = curx;
     anchor_y = cury;
}

void my_proc(special_XEvent *event)
{
static char keybuf;
static bool dbl_click;
static WPoint wp, wp_new;
static VPoint vp;
static VVector shift;
static view v;
static world wtmp,wtmp2;
static int cg, newg, loc;
static char dummy[256];
static int track_setno;
static int track_loc;
static int add_at;
static int type, id;   /* for objects */
static int axisno;
static int nr_of_sel_graphs,*sel_graphs=new int[2];
static Datapoint dpoint;
static GLocator locator;
bool old_upd;
old_upd=immediateUpdate;
immediateUpdate=false;
updateRunning=true;
    cg = get_cg();
    get_tracking_props(&track_setno, &move_dir, &add_at);
    switch (event->type)
    {
    case QEvent::MouseMove://MotionNotify:
	x = event->xmotion.x;
	y = event->xmotion.y;
        if (cursortype != 0)
        {
        crosshair_motion(x, y);
        }
        vp = xlibdev2VPoint(x, y);
        getpoints(&vp);
        if (focus_policy == FOCUS_FOLLOWS)
        {
            if ((newg = next_graph_containing(-1, vp)) != cg)
            {
                switch_current_graph(newg);
                cg = newg;
            }
        }
        switch (action_flag) {
        case DO_NOTHING:
            break;
        case VIEW_2ND:
        case ZOOM_2ND:
        case ZOOMX_2ND:
        case ZOOMY_2ND:
        case MAKE_BOX_2ND:
        case MAKE_ELLIP_2ND:
	    select_region(anchor_x, anchor_y, x, y, 1);
            break;
        case MOVE_OBJECT_2ND:
        case COPY_OBJECT2ND:
        case PLACE_LEGEND_2ND:
        case PLACE_TIMESTAMP_2ND:
            slide_region(bb, x - anchor_x, y - anchor_y, 1);
            break;
        case DEF_REGION2ND:
            select_line(anchor_x, anchor_y, x, y, 1);
            mainWin->mainArea->completeRedraw();
            break;
        case MAKE_LINE_2ND:
            select_line(anchor_x, anchor_y, x, y, 1);
            mainWin->mainArea->completeRedraw();
            break;
        case DEF_REGION:
            if (region_pts > 0)
            {
                anchor_point(x, y, vp);
                iax[region_pts] = x;
                iay[region_pts] = y;
                view2world(vp.x, vp.y,
                &region_wps[region_pts].x, &region_wps[region_pts].y);
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
                mainWin->mainArea->completeRedraw();
            }
            break;
        case MOVE_POINT2ND:
            switch (move_dir)
            {
	    case MOVE_POINT_XY:
                ///select_line(anchor_x, anchor_y, x, y, 1);
                break;
	    case MOVE_POINT_X:
                ///select_line(x, anchor_y, anchor_x, anchor_y, 1);
	        break;
	    case MOVE_POINT_Y:
                ///select_line(anchor_x, anchor_y, anchor_x, y, 1);
	        break;
	    }
            break;
        default:
            immediateUpdate=old_upd;
            updateRunning=false;
            return;
            break;
        }

        break;
    case QEvent::MouseButtonPress://ButtonPress:
        mainWin->getselectedgraphs(&nr_of_sel_graphs,&sel_graphs);
	x = event->xbutton.x;
	y = event->xbutton.y;
	vp = xlibdev2VPoint(x, y);
        getpoints(&vp);
        //cout << x << " " << y << endl;
        switch (event->xbutton.button)
        {
        case Qt::LeftButton://Button1:
            /* first, determine if it's double click --> replaced by QT-double-click function*/
            /*if (lastc.restart() < CLICKINT)
	    {
                dbl_click = TRUE;
            } else {
                dbl_click = FALSE;
            }*/
//            if(FormPointExplorer==NULL){
//                dbl_click=event->doubleClick;
//            }else{
//                dbl_click=TRUE;
//                allow_dc = TRUE;

//            }

          dbl_click=event->doubleClick;



          switch (action_flag)
            {
            case 0:
                if (dbl_click == TRUE && allow_dc == TRUE)
                {
                    track_setno = -1;
                    if (focus_clicked(cg, vp, &anchor_vp) == TRUE)
                    {
                        xlibVPoint2dev(anchor_vp, &anchor_x, &anchor_y);
                        set_action(VIEW_2ND);
                    select_region(anchor_x, anchor_y, x, y, 0);
                        get_graph_viewport(cg,&bb);
                        ShiftRect=QRect(xconvxlib(bb.xv1),yconvxlib(bb.yv2),xconvxlib(bb.xv2)-xconvxlib(bb.xv1),yconvxlib(bb.yv1)-yconvxlib(bb.yv2));
                        ShiftPoint=QPoint(x,y);
                        mainWin->mainArea->rubber->setGeometry(ShiftRect);
                        mainWin->mainArea->rubber->show();
                    }
                    else if (find_point(cg, vp, &track_setno, &loc) == RETURN_SUCCESS)
                    {
            if (FormSetAppearance==NULL)
            {
                        FormSetAppearance=new frmSetAppearance(mainWin);
                        FormSetAppearance->init();
                        }
            if(FormPointExplorer=NULL){ //BZ Point explorer fix.
                        FormSetAppearance->show();
                        FormSetAppearance->ShowSetData_external(cg,track_setno);
                        FormSetAppearance->raise();
                        FormSetAppearance->activateWindow();
}
                        if(FormPointExplorer!=NULL){
                             if(FormPointExplorer->isVisible()){
                            FormSetAppearance->hide();
                             }
                        }

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
                    }
                    else if (title_clicked(cg, vp) == TRUE)
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
                        FormGraphAppearance->flp->tabMain->ledTitle->lenText->setFocus();
                    }
                    else if (legend_clicked(cg, vp, &bb) == TRUE)
                    {
			if (FormGraphAppearance==NULL)
			{
			FormGraphAppearance=new frmGraphApp(mainWin);
			}
			FormGraphAppearance->init();
			FormGraphAppearance->show_graph_data_external(cg);
			FormGraphAppearance->show();
                        FormGraphAppearance->raise();
                    }
                    else if (find_item(cg, vp, &bb, &type, &id) == RETURN_SUCCESS)
                    {
                        object_edit_popup(type, id);
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

                        if(FormPointExplorer!=NULL){
                            if(FormPointExplorer->isVisible()){
                           FormSetAppearance->hide();
                            }
                        }
                    }
                }
                else
                {
                    if (focus_policy == FOCUS_CLICK)
		    {
                        if ((newg = next_graph_containing(cg, vp)) != cg)
			{
                        switch_current_graph(newg);
                        }
                    }
                }
                break;
            case VIEW_2ND:
                set_action(DO_NOTHING);
                v.xv1 = MIN2(vp.x, anchor_vp.x);
		v.yv1 = MIN2(vp.y, anchor_vp.y);
		v.xv2 = MAX2(vp.x, anchor_vp.x);
		v.yv2 = MAX2(vp.y, anchor_vp.y);
                    ///Undo-Stuff
                    ViewportChanged(cg,v);
                set_graph_viewport(cg, v);
                    if (FormGraphAppearance!=NULL)
                    FormGraphAppearance->update_view(cg);
                mainWin->mainArea->rubber->hide();
		mainWin->mainArea->completeRedraw();
                break;
            case ZOOM_1ST:
                anchor_point(x, y, vp);
                set_action(ZOOM_2ND);
	        select_region(anchor_x, anchor_y, x, y, 0);
                break;
            case ZOOMX_1ST:
                anchor_point(x, y, vp);
                set_action(ZOOMX_2ND);
	        select_region(anchor_x, anchor_y, x, y, 0);
                break;
            case ZOOMY_1ST:
                anchor_point(x, y, vp);
                set_action(ZOOMY_2ND);
	        select_region(anchor_x, anchor_y, x, y, 0);
                break;
            case VIEW_1ST:
                anchor_point(x, y, vp);
                set_action(VIEW_2ND);
	        select_region(anchor_x, anchor_y, x, y, 0);
                break;
            case ZOOM_2ND:
                set_action(DO_NOTHING);
                if (nr_of_sel_graphs<=0) break;
                SaveTickmarksStatesPrevious(X_AXIS,Y_AXIS,nr_of_sel_graphs,sel_graphs);
                    newworld(get_cg(), ALL_AXES, anchor_vp, vp);//zoom all axes of current graph only
                    get_graph_world(get_cg(), &wtmp);//save zoomed axes scale
                for (int ll=0;ll<nr_of_sel_graphs;ll++)
                {
                cg=sel_graphs[ll];
                get_graph_world(cg, &wtmp2);
                wtmp2.xg1 = wtmp.xg1;//all shall be zoomed --> copy all axes from current graph
                wtmp2.xg2 = wtmp.xg2;
                wtmp2.yg1 = wtmp.yg1;
                wtmp2.yg2 = wtmp.yg2;
                //newworld(cg, ALL_Y_AXES, anchor_vp, vp);
                set_graph_world(cg, wtmp2);//set zoom values of all graphs
                autotick_axis(cg, ALL_AXES);
                }
                TickmarksModified(X_AXIS,Y_AXIS,nr_of_sel_graphs,sel_graphs);
                    if (nr_of_sel_graphs==1)
                    {
                    sprintf(dummy,"[G%d]",cg);
                    addAditionalDescriptionToLastNode(-1,QObject::tr("Zoom graph ")+QString(dummy),QString(),0);
                    }
                    else
                    {
                    addAditionalDescriptionToLastNode(-1,QObject::tr("Zoom graphs")+QString(dummy),QString(),1);
                    }
                break;
            case ZOOMX_2ND:
                set_action(DO_NOTHING);
                if (nr_of_sel_graphs<=0) break;
                SaveTickmarksStatesPrevious(X_AXIS,X_AXIS,nr_of_sel_graphs,sel_graphs);
                    newworld(get_cg(), ALL_X_AXES, anchor_vp, vp);//zoom x of current graph only
                    get_graph_world(get_cg(), &wtmp);//save zoomed x scale
                for (int ll=0;ll<nr_of_sel_graphs;ll++)
                {
                cg=sel_graphs[ll];
                get_graph_world(cg, &wtmp2);
                wtmp2.xg1 = wtmp.xg1;//x shall be zoomed --> copy x from current graph
                wtmp2.xg2 = wtmp.xg2;
                //wtmp2.yg1 = wtmp.yg1;
                //wtmp2.yg2 = wtmp.yg2;
                //newworld(cg, ALL_Y_AXES, anchor_vp, vp);
                set_graph_world(cg, wtmp2);//set zoom values of all graphs
                autotick_axis(cg, ALL_X_AXES);
                }
                TickmarksModified(X_AXIS,X_AXIS,nr_of_sel_graphs,sel_graphs);
                    if (nr_of_sel_graphs==1)
                    {
                    sprintf(dummy,"[G%d]",cg);
                    addAditionalDescriptionToLastNode(-1,QObject::tr("Zoom X-axes of ")+QString(dummy),QString(),0);
                    }
                    else
                    {
                    addAditionalDescriptionToLastNode(-1,QObject::tr("Zoom X-axes of graphs")+QString(dummy),QString(),1);
                    }
                break;
            case ZOOMY_2ND:
                set_action(DO_NOTHING);
                if (nr_of_sel_graphs<=0) break;
                SaveTickmarksStatesPrevious(Y_AXIS,Y_AXIS,nr_of_sel_graphs,sel_graphs);
                    newworld(get_cg(), ALL_Y_AXES, anchor_vp, vp);//zoom y of current graph only
                    get_graph_world(get_cg(), &wtmp);//save zoomed y scale
                for (int ll=0;ll<nr_of_sel_graphs;ll++)
                {
                cg=sel_graphs[ll];
                get_graph_world(cg, &wtmp2);
                //wtmp2.xg1 = wtmp.xg1;
                //wtmp2.xg2 = wtmp.xg2;
                wtmp2.yg1 = wtmp.yg1;//y shall be zoomed --> copy y from current graph
                wtmp2.yg2 = wtmp.yg2;
                //newworld(cg, ALL_Y_AXES, anchor_vp, vp);
                set_graph_world(cg, wtmp2);//set zoom values of all graphs
                autotick_axis(cg, ALL_Y_AXES);
                }
                TickmarksModified(Y_AXIS,Y_AXIS,nr_of_sel_graphs,sel_graphs);
                    if (nr_of_sel_graphs==1)
                    {
                    sprintf(dummy,"[G%d]",cg);
                    addAditionalDescriptionToLastNode(-1,QObject::tr("Zoom Y-axes of ")+QString(dummy),QString(),0);
                    }
                    else
                    {
                    addAditionalDescriptionToLastNode(-1,QObject::tr("Zoom Y-axes of graphs")+QString(dummy),QString(),1);
                    }
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
                    char message[32];
                    sprintf(message, "Kill the %s?", object_types(type));
                    if (yesno(message, NULL, NULL, NULL) == TRUE)
                    {
                            //Undo-Stuff
                            nrOfUndoObj=1;
                            undoObjIds[0]=id;
                            ObjectsDeleted(nrOfUndoObj,undoObjIds,type);
                    kill_object(type, id);
                    mainWin->mainArea->completeRedraw();
                    }
                }
                break;
            case MOVE_OBJECT_1ST:
                if (find_item(cg, vp, &bb, &type, &id) == RETURN_SUCCESS)
                {
                    ShiftRect=QRect(xconvxlib(bb.xv1),yconvxlib(bb.yv2),xconvxlib(bb.xv2)-xconvxlib(bb.xv1),yconvxlib(bb.yv1)-yconvxlib(bb.yv2));
                    ShiftPoint=QPoint(x,y);
                    mainWin->mainArea->rubber->setGeometry(ShiftRect);
                    mainWin->mainArea->rubber->show();
                    anchor_point(x, y, vp);
	            slide_region(bb, x - anchor_x, y - anchor_y, 0);
                    set_action(MOVE_OBJECT_2ND);
                }
                break;
            case MOVE_OBJECT_2ND:
                shift.x = vp.x - anchor_vp.x;
                shift.y = vp.y - anchor_vp.y;
                //Undo-Stuff
                SaveObjectData(id,type);
                //Do action
                move_object(type, id, shift);
                //Undo-Stuff
                ObjectDataModified(id,type);
                addAditionalDescriptionToLastNode(UNDO_TYPE_OBJECT_MODIFIED,QString(),QObject::tr("moved"),-1);
                xdrawgraph();
                set_action(MOVE_OBJECT_1ST);
		mainWin->mainArea->rubber->hide();
		mainWin->mainArea->completeRedraw();
                break;
            case COPY_OBJECT1ST:
                if (find_item(cg, vp, &bb, &type, &id) == RETURN_SUCCESS)
                {
                    ShiftRect=QRect(xconvxlib(bb.xv1),yconvxlib(bb.yv2),xconvxlib(bb.xv2)-xconvxlib(bb.xv1),yconvxlib(bb.yv1)-yconvxlib(bb.yv2));
                    ShiftPoint=QPoint(x,y);
                    mainWin->mainArea->rubber->setGeometry(ShiftRect);
                    mainWin->mainArea->rubber->show();
                    anchor_point(x, y, vp);
	            slide_region(bb, x - anchor_x, y - anchor_y, 0);
                    set_action(COPY_OBJECT2ND);
                }
                break;
            case COPY_OBJECT2ND:
                shift.x = vp.x - anchor_vp.x;
                shift.y = vp.y - anchor_vp.y;
                id = duplicate_object(type, id);
                move_object(type, id, shift);
                //Undo-Stuff
                undoObjIds[0]=id;
                ObjectsCreated(1,undoObjIds,type);
                addAditionalDescriptionToLastNode(UNDO_TYPE_OBJECT_INSERTED,QString(),QObject::tr("coppied"),-1);
                set_action(COPY_OBJECT1ST);
		mainWin->mainArea->rubber->hide();
                mainWin->mainArea->completeRedraw();
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
                set_action(MAKE_LINE_2ND);
                break;
            case MAKE_LINE_2ND:
                select_line(anchor_x, anchor_y, x, y, 0);
                id = next_line();
                init_line(id, anchor_vp, vp);
                rg[MAXREGION].active=FALSE;
                    //Undo-Stuff
                    nrOfUndoObj=1;
                    undoObjIds[0]=id;
                    ObjectsCreated(nrOfUndoObj,undoObjIds,OBJECT_LINE);
		mainWin->mainArea->completeRedraw();
                set_action(MAKE_LINE_1ST);
                break;
            case MAKE_BOX_1ST:
                anchor_point(x, y, vp);
	        select_region(anchor_x, anchor_y, x, y, 0);
                set_action(MAKE_BOX_2ND);
                break;
            case MAKE_BOX_2ND:
	        select_region(anchor_x, anchor_y, x, y, 0);
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
	        select_region(anchor_x, anchor_y, x, y, 0);
                set_action(MAKE_ELLIP_2ND);
                break;
            case MAKE_ELLIP_2ND:
	        select_region(anchor_x, anchor_y, x, y, 0);
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
                }
                break;
            case MOVE_POINT1ST:
                if (find_point(cg, vp, &track_setno, &track_loc) == RETURN_SUCCESS)
                {
                    anchor_point(x, y, vp);
                    get_point(cg, track_setno, track_loc, &wp);
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
                    mainWin->mainArea->completeRedraw();
		    set_action(MOVE_POINT1ST);
                }
                break;
            case ADD_POINT:
		view2world(vp.x, vp.y, &wp.x, &wp.y);
                zero_datapoint(&dpoint);
                dpoint.ex[0] = wp.x;
                dpoint.ex[1] = wp.y;
                switch (add_at) {
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
                if (undo_nrs!=NULL) delete[] undo_nrs;
                undo_nrs=new int[2];
                undo_nrs[1]=undo_nrs[0]=loc;
                memcpy(undo_dp,&dpoint,sizeof(Datapoint));
                DataPointEdited(cg,track_setno,&loc,1,undo_dp,0);
                if (add_point_at(cg, track_setno, loc, &dpoint)== RETURN_SUCCESS)
                {
                 update_set_lists(cg);
                 mainWin->mainArea->completeRedraw();
                }
                break;
            case PLACE_LEGEND_1ST:
                if (legend_clicked(cg, vp, &bb) == TRUE)
                {
                ShiftRect=QRect(xconvxlib(bb.xv1),yconvxlib(bb.yv2),xconvxlib(bb.xv2)-xconvxlib(bb.xv1),yconvxlib(bb.yv1)-yconvxlib(bb.yv2));
                ShiftPoint=QPoint(x,y);
                mainWin->mainArea->rubber->setGeometry(ShiftRect);
                mainWin->mainArea->rubber->show();
                    anchor_point(x, y, vp);
                    slide_region(bb, x - anchor_x, y - anchor_y, 0);
                    set_action(PLACE_LEGEND_2ND);
                }
                break;
            case PLACE_LEGEND_2ND:
                shift.x = vp.x - anchor_vp.x;
                shift.y = vp.y - anchor_vp.y;
                move_legend(cg, shift);
                    if (FormGraphAppearance!=NULL)
                    FormGraphAppearance->updatelegends(cg);
                mainWin->mainArea->rubber->hide();
		mainWin->mainArea->completeRedraw();
                set_action(PLACE_LEGEND_1ST);
                break;
            case PLACE_TIMESTAMP_1ST:
                if (timestamp_clicked(vp, &bb) == TRUE)
                {
                ShiftRect=QRect(xconvxlib(bb.xv1),yconvxlib(bb.yv2),xconvxlib(bb.xv2)-xconvxlib(bb.xv1),yconvxlib(bb.yv1)-yconvxlib(bb.yv2));
                ShiftPoint=QPoint(x,y);
                mainWin->mainArea->rubber->setGeometry(ShiftRect);
                mainWin->mainArea->rubber->show();
                anchor_point(x, y, vp);
                slide_region(bb, x - anchor_x, y - anchor_y, 0);
                set_action(PLACE_TIMESTAMP_2ND);
                }
                break;
            case PLACE_TIMESTAMP_2ND:
                shift.x = vp.x - anchor_vp.x;
                shift.y = vp.y - anchor_vp.y;
                move_timestamp(shift);
                mainWin->mainArea->rubber->hide();
		mainWin->mainArea->completeRedraw();
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
		set_action(DEF_REGION2ND);
		break;
	    case DEF_REGION2ND:
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
                view2world(vp.x, vp.y,
                &region_wps[region_pts].x, &region_wps[region_pts].y);
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
                mainWin->mainArea->completeRedraw();
		break;
            default:
                break;
            }
            break;
        case Qt::MidButton://Button2:
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
            switch (action_flag)
            {
            case DO_NOTHING:
/*
 *                 find_item(cg, vp, &anchor_vp, &type, &id);
 *                 sprintf(buf, "type = %d, id = %d", type, id);
 *                 set_left_footer(buf);
 */
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
		mainWin->mainArea->completeRedraw();
                break;
            default:
                rg[MAXREGION].active=FALSE;
                set_action(DO_NOTHING);
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
    case QEvent::KeyPress://KeyPress:
        //XLookupString((XKeyEvent *) event, &keybuf, 1, &keys, &compose);
        if (keybuf == 27)
        { /* Esc */
            set_action(DO_NOTHING);
            immediateUpdate=old_upd;
            updateRunning=false;
            return;
        }
        else if (event->key==Qt::Key_L && event->ctrl==true && event->alt==false)
        {
        set_action(DO_NOTHING);
        set_action(PLACE_LEGEND_1ST);
        }
        else if (event->key==Qt::Key_L && event->ctrl==true && event->alt==true)
        {
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
        set_action(DO_NOTHING);
        set_action(DEL_OBJECT);
        }
        else if (event->key==Qt::Key_M && event->ctrl==true && event->alt==false)
        {
        set_action(DO_NOTHING);
        set_action(MOVE_OBJECT_1ST);
        }
        else if (event->key==Qt::Key_T && event->ctrl==true && event->alt==false)
        {
        set_action(DO_NOTHING);
        set_action(PLACE_TIMESTAMP_1ST);
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
        else if (event->key==Qt::Key_V && event->ctrl==false && event->alt==true)
        {
        set_action(DO_NOTHING);
        set_action(VIEW_1ST);
        }
        else if (event->key==Qt::Key_Z && event->ctrl==false && event->alt==true)
        {
        set_action(DO_NOTHING);
        set_action(ZOOM_1ST);
        }
        else if (event->key==Qt::Key_B && event->ctrl==true && event->alt==true)
        {
        set_action(DO_NOTHING);
        set_action(MAKE_BOX_1ST);
        }
        else if (event->key==Qt::Key_T && event->ctrl==true && event->alt==true)
        {
        set_action(DO_NOTHING);
        set_action(STR_LOC);
        }
        else if (event->key==Qt::Key_E && event->ctrl==true && event->alt==true)
        {
        set_action(DO_NOTHING);
        set_action(MAKE_ELLIP_1ST);
        }
        else if (event->key==Qt::Key_V && event->ctrl==true && event->alt==false)//paste_event
        {
        GeneralPaste(QApplication::clipboard()->mimeData());
        }
        else if (event->key==Qt::Key_1 && event->ctrl==true && event->alt==false)//set Page Zoom back to '1'
        {
        mainWin->sldPageZoom->setValue(0);
        }
        break;
    default:
	break;
    }
immediateUpdate=old_upd;
updateRunning=false;
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
    switch (act) {
    case 0:
        switch (action_flag) {
        case ZOOM_2ND:
        case ZOOMX_2ND:
        case ZOOMY_2ND:
        case VIEW_2ND:
            select_region(anchor_x, anchor_y, x, y, 0);
            break;
        case MOVE_OBJECT_2ND:
        case COPY_OBJECT2ND:
        case PLACE_LEGEND_2ND:
        case PLACE_TIMESTAMP_2ND:
            slide_region(bb, x - anchor_x, y - anchor_y, 0);
            break;
        case DEF_REGION2ND:
            break;
        case MAKE_LINE_2ND:
	    select_line(anchor_x, anchor_y, x, y, 0);
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
	set_left_footer(NULL);
	break;
    case ZOOM_1ST:
	set_cursor(0);
	set_left_footer("Pick first corner for zoom");
	break;
    case ZOOM_2ND:
	set_left_footer("Pick second corner for zoom");
	break;
    case ZOOMX_1ST:
	set_cursor(0);
	set_left_footer("Pick first point for zoom along X-axis");
	break;
    case ZOOMX_2ND:
	set_left_footer("Pick second point for zoom along X-axis");
	break;
    case ZOOMY_1ST:
	set_cursor(0);
	set_left_footer("Pick first point for zoom along Y-axis");
	break;
    case ZOOMY_2ND:
	set_left_footer("Pick second point for zoom along Y-axis");
	break;
    case VIEW_1ST:
	set_cursor(0);
	set_left_footer("Pick first corner of viewport");
	break;
    case VIEW_2ND:
	set_left_footer("Pick second corner of viewport");
	break;
    case EDIT_OBJECT:
	set_cursor(1);
	set_left_footer("Pick object to edit");
	break;
    case DEL_OBJECT:
	set_cursor(3);
	set_left_footer("Delete object");
	break;
    case MOVE_OBJECT_1ST:
	set_cursor(1);
	set_left_footer("Pick object to move");
	break;
    case COPY_OBJECT1ST:
	set_cursor(1);
	set_left_footer("Pick object to copy");
	break;
    case MOVE_OBJECT_2ND:
    case COPY_OBJECT2ND:
	set_cursor(4);
	set_left_footer("Place object");
	break;
    case STR_LOC:
	set_cursor(2);
	set_left_footer("Pick beginning of text");
	break;
    case MAKE_LINE_1ST:
	set_cursor(0);
	set_left_footer("Pick beginning of line");
	break;
    case MAKE_LINE_2ND:
	set_left_footer("Pick end of line");
	break;
    case MAKE_BOX_1ST:
	set_cursor(0);
	set_left_footer("First corner of box");
	break;
    case MAKE_BOX_2ND:
	set_left_footer("Second corner of box");
	break;
    case MAKE_ELLIP_1ST:
	set_cursor(0);
	set_left_footer("Pick beginning of bounding box for ellipse");
	break;
    case MAKE_ELLIP_2ND:
	set_left_footer("Pick opposite corner");
	break;
    case AUTO_NEAREST:
	set_cursor(0);
	set_left_footer("Autoscale on nearest set - click near a point of the set to autoscale");
	break;
    case TRACKER:
	set_cursor(1);
	set_left_footer("Tracker");
	break;
    case DEL_POINT:
	set_cursor(3);
	set_left_footer("Delete point");
	break;
    case MOVE_POINT1ST:
	set_cursor(4);
	set_left_footer("Pick point to move");
	break;
    case MOVE_POINT2ND:
	set_left_footer("Pick final location");
	break;
    case ADD_POINT:
	set_cursor(0);
	set_left_footer("Add point");
	break;
    case PLACE_LEGEND_1ST:
	set_cursor(1);
	set_left_footer("Pick legend");
	break;
    case PLACE_LEGEND_2ND:
	set_cursor(4);
	set_left_footer("Move legend");
	break;
    case PLACE_TIMESTAMP_1ST:
	set_cursor(1);
	set_left_footer("Pick timestamp");
	break;
    case PLACE_TIMESTAMP_2ND:
	set_cursor(4);
	set_left_footer("Place timestamp");
	break;
    case SEL_POINT:
	set_cursor(0);
	set_left_footer("Pick reference point");
	break;
    case DEF_REGION1ST:
	set_cursor(0);
	set_left_footer("Pick first point for region");
	break;
    case DEF_REGION2ND:
	set_left_footer("Pick second point for region");
	break;
    case DEF_REGION:
	set_cursor(0);
	set_left_footer("Define region");
	break;
    case COMP_AREA:
	set_cursor(0);
	set_left_footer("Compute area");
	break;
    case COMP_PERIMETER:
	set_cursor(0);
	set_left_footer("Compute perimeter");
	break;
    case DISLINE1ST:
	set_cursor(0);
	set_left_footer("Pick start of line for distance computation");
	break;
    case DISLINE2ND:
	set_cursor(0);
	set_left_footer("Pick ending point");
	break;
    }
    action_flag = act;
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
static char *typestr[6] = {"X, Y",
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
    
    if (vpp != NULL) {
        vp = *vpp;
    }
    
    view2world(vp.x, vp.y, &wx, &wy);
    if (get_graph_locator(cg, &locator) != RETURN_SUCCESS) {
	mainWin->statLocBar->setText(QString("[No graphs]"));
        return;
    }
    
    if (locator.pointset) {
	dsx = locator.dsx;
	dsy = locator.dsy;
    }
    
    switch (locator.pt_type) {
    case 0:
        if (get_graph_type(cg) == GRAPH_POLAR) {
            polar2xy(wx, wy, &xtmp, &ytmp);
        } else {
            xtmp = wx;
            ytmp = wy;
        }
        break;
    case 1:
        xtmp = wx - dsx;
        ytmp = wy - dsy;
        break;
    case 2:
        if (get_graph_type(cg) == GRAPH_POLAR) {
            polar2xy(wx, wy, &xtmp, &ytmp);
        } else {
            xtmp = wx;
            ytmp = wy;
        }
        xtmp = hypot(dsx - xtmp, dsy - ytmp);
        ytmp = 0.0;
        break;
    case 3:
        if (dsx - wx != 0.0 || dsy - wy != 0.0) {
            xy2polar(wx - dsx, wy - dsy, &xtmp, &ytmp);
        } else {
            xtmp = 0.0;
            ytmp = 0.0;
        }
        break;
    case 4:
        xtmp = vp.x;
        ytmp = vp.y;
        break;
    case 5:
        xlibVPoint2dev(vp, &x, &y);
        xtmp = x;
        ytmp = y;
        break;
    default:
        return;
    }
    s = create_fstring(locator.fx, locator.px, xtmp, LFORMAT_TYPE_PLAIN);
    strcpy(bufx, s);
    s = create_fstring(locator.fy, locator.py, ytmp, LFORMAT_TYPE_PLAIN);
    strcpy(bufy, s);
    sprintf(buf, "G%1d: %s = [%s, %s]", cg, typestr[locator.pt_type], bufx, bufy);
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
    if (is_valid_gno(cg) == FALSE) {
        cg = -1;
    }
    for (i = 0; i < ng ; i++) {
	j = (i + cg + 1) % ng;
	if (is_graph_hidden(j)        == FALSE &&
            get_graph_viewport(j, &v) == RETURN_SUCCESS &&
            is_vpoint_inside(v, vp, MAXPICKDIST)   == TRUE) {
	    
            gno = j;
            break;
	}
    }
    return gno;
}

int legend_clicked(int gno, VPoint vp, view *bb)
{
    legend l;
    if (is_graph_hidden(gno) == FALSE) {
        get_graph_legend(gno, &l);
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
    if (timestamp.active && is_vpoint_inside(timestamp.bb, vp, MAXPICKDIST)) {
        *bb = timestamp.bb;
        return TRUE;
    } else {
        return FALSE;
    }
}

int focus_clicked(int cg, VPoint vp, VPoint *avp)
{
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

int axis_clicked(int gno, VPoint vp, int *axisno)
{
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

int title_clicked(int gno, VPoint vp)
{
    view v;
    /* a rude check; TODO: use right offsets */
    if (is_graph_hidden(gno) == TRUE) {
        return FALSE;
    } else {
        get_graph_viewport(gno, &v);
        if (vp.x >= v.xv1 && vp.x <= v.xv2 &&
            vp.y > v.yv2 && vp.y < v.yv2 + 0.1) {
            return TRUE;
        } else {
            return FALSE;
        }
    }
}

/*
 * locate a point and the set the point is in
 */
int find_point(int gno, VPoint vp, int *setno, int *loc)
{
    int i, start, stop, j, found;
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
    double dist, mindist = 1.0;
    
    if (is_valid_setno(gno, setno) == TRUE) {
        if (is_set_hidden(gno, setno) == FALSE) {
            xtmp = getx(gno, setno);
            ytmp = gety(gno, setno);
            for (j = 0; j < getsetlength(gno, setno) - 1; j++) {
                wptmp.x = xtmp[j];
                wptmp.y = ytmp[j];
                vp1 = Wpoint2Vpoint(wptmp);
                wptmp.x = xtmp[j + 1];
                wptmp.y = ytmp[j + 1];
                vp2 = Wpoint2Vpoint(wptmp);
 
                dist = hypot(vp.x - vp1.x, vp.y - vp1.y) +
                       hypot(vp.x - vp2.x, vp.y - vp2.y);
                if (dist < mindist) {
                    loc = j + 1;
                    mindist = dist;
                }
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
        select_graph(gno);
        set_graph_selectors(gno);
        getpoints(NULL);
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
