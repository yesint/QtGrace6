/*
 * Grace - GRaphing, Advanced Computation and Exploration of data
 *
 * Home page: http://plasma-gate.weizmann.ac.il/Grace/
 *
 * Copyright (c) 1991-1995 Paul J Turner, Portland, OR
 * Copyright (c) 1996-2007 Grace Development Team
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
 * Draw axis bars, axis labels, ticks and tick labels
 */

#include <iostream>
///#include <config.h>
#include <cmath>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "cmath.h"
#include "globals.h"
#include "utils.h"
#include "graphs.h"
#include "graphutils.h"
#include "draw.h"
#include "parser.h"
#include "noxprotos.h"
#include "mathstuff.h"
#include <QtCore>

using namespace std;

extern long double polar2_f,phi0,roffset,rmax;

extern void return_last_string_bounding_box(view * bb);
extern void SetDecimalSeparatorToUserValue(char * str,bool remove_space=true);
extern int currentStringAlignment;

int is_xaxis(int axis)
{
    return ((axis % 2 == 0));
}

int is_yaxis(int axis)
{
    return ((axis % 2 == 1));
}

int is_log_axis(int gno, int axis)
{
    if ((is_xaxis(axis) && islogx(gno)) ||
            (is_yaxis(axis) && islogy(gno))) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int is_logit_axis(int gno, int axis)
{
    if ((is_xaxis(axis) && islogitx(gno)) ||
            (is_yaxis(axis) && islogity(gno))) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void drawgrid(int gno)
{
    int caxis;
    tickmarks *t;
    tickprops tprops;
    int ttype;
    world w;
    view v;
    double wtpos;
    WPoint wp_grid_start = {0.0, 0.0}, wp_grid_stop = {0.0, 0.0};
    VPoint vp_grid_start, vp_grid_stop;
    VPoint vpc, vp1, vp2;
    double phi_start, phi_stop, rho;
    double wc_start, wc_stop; /* world coordinates */
    int ittype_loop, itick;
long double rtmp;

    setclipping(TRUE);
    
    /* TODO: add Pen to ticks and remove the following */
    setpattern(1);
    
    get_graph_viewport(gno, &v);
    get_graph_world(gno, &w);

    if (get_graph_type(gno) == GRAPH_POLAR2)
    {
    phi_start=w.xg1+phi0;
    phi_stop=w.xg2+phi0;
    }

    /* graph center; for polar plots */
    vpc.x = (v.xv1 + v.xv2)/2.0;
    vpc.y = (v.yv1 + v.yv2)/2.0;
    
    for (caxis = 0; caxis < MAXAXES; caxis++) {
        t = get_graph_tickmarks(gno, caxis);
        if (!t || t->active != TRUE) {
            continue;
        }
        if (is_xaxis(caxis)) { /* an X-axis */
            wc_start = w.xg1;
            wc_stop = w.xg2;
            wp_grid_start.y = w.yg1;
            wp_grid_stop.y = w.yg2;
        } else {              /* a Y-axis */
            wc_start = w.yg1;
            wc_stop = w.yg2;
            wp_grid_start.x = w.xg1;
            wp_grid_stop.x = w.xg2;
        }

        if (get_graph_type(gno) == GRAPH_POLAR2)
        {
        wp_grid_start.x = w.xg1;
        wp_grid_stop.x = w.xg2;
        wp_grid_start.y = w.yg1;
        wp_grid_stop.y = w.yg2;
        }

        for (ittype_loop = 0; ittype_loop < 2; ittype_loop++) {
            if (ittype_loop == 0) { /* minor ticks */
                ttype = TICK_TYPE_MINOR;
                tprops = t->mprops;
            } else {	  /* major ticks */
                ttype = TICK_TYPE_MAJOR;
                tprops = t->props;
            }
            if (tprops.gridflag == 0) {
                continue;
            }

            setcolor(tprops.color);
            setalpha(tprops.alpha);
            setlinewidth(tprops.linew);
            setlinestyle(tprops.lines);

            for (itick = 0; itick < t->nticks; itick++) {
                if (t->tloc[itick].type != ttype) {
                    continue;
                }
                
                wtpos = t->tloc[itick].wtpos;

                if ((wtpos < wc_start) || (wtpos > wc_stop)) {
                    continue;
                }

                if (is_xaxis(caxis)) { /* an X-axis */
                    wp_grid_start.x = wtpos;
                    wp_grid_stop.x = wtpos;
                } else {              /* a Y-axis */
                    wp_grid_start.y = wtpos;
                    wp_grid_stop.y = wtpos;
                }

                vp_grid_start = Wpoint2Vpoint(wp_grid_start);
                vp_grid_stop = Wpoint2Vpoint(wp_grid_stop);

                if (!is_xaxis(caxis) && get_graph_type(gno) == GRAPH_POLAR)
                {
                    xy2polar(vp_grid_start.x - vpc.x, vp_grid_start.y - vpc.y,
                             &phi_start, &rho);
                    xy2polar(vp_grid_stop.x - vpc.x, vp_grid_stop.y - vpc.y,
                             &phi_stop, &rho);
                    vp1.x = vpc.x - rho;
                    vp1.y = vpc.y + rho;
                    vp2.x = vpc.x + rho;
                    vp2.y = vpc.y - rho;
                    if (is_graph_xinvert(gno) == TRUE)
                    {
                        fswap(&phi_start, &phi_stop);
                    }
                    if (phi_stop < phi_start)
                    {
                        phi_stop += 2*M_PI;
                    }
                    DrawArc(vp1, vp2, (int) rint(180.0/M_PI*phi_start),
                            (int) rint(180.0/M_PI*phi_stop));
                }
                else if (!is_xaxis(caxis) && get_graph_type(gno) == GRAPH_POLAR2)///draw y-axis=r-axis
                {
                    //at the moment the same as polar, we have to change this!!!
                    /*xy2polar2(vp_grid_start.x - vpc.x, vp_grid_start.y - vpc.y,
                             &phi_start, &rho);
                    xy2polar2(vp_grid_stop.x - vpc.x, vp_grid_stop.y - vpc.y,
                             &phi_stop, &rho);
                    qDebug() << "vpgrid_start=" << vp_grid_start.x - vpc.x << "|" << vp_grid_start.y - vpc.y;
                    qDebug() << "vpgrid_stop =" << vp_grid_stop.x - vpc.x << "|" << vp_grid_stop.y - vpc.y;
qDebug() << "Start=" << rint(180.0/M_PI*(phi_start)) << " Stop=" << rint(180.0/M_PI*(phi_stop));
phi_stop=2*M_PI;
                    vp1.x = vpc.x - rho;
                    vp1.y = vpc.y + rho;
                    vp2.x = vpc.x + rho;
                    vp2.y = vpc.y - rho;*/
                    /*if (is_graph_xinvert(gno) == TRUE)
                    {
                        fswap(&phi_start, &phi_stop);
                    }
                    if (phi_stop < phi_start)
                    {
                        phi_stop += 2*M_PI;
                    }*/
                    //qDebug() << "Start=" << rint(180.0/M_PI*(phi_start+g[gno].phi0)) << " Stop=" << rint(180.0/M_PI*(phi_stop+g[gno].phi0));
                    /*DrawArc(vp1, vp2, (int) rint(180.0/M_PI*(phi_start)),
                            (int) rint(180.0/M_PI*(phi_stop)));*/

                    //rtmp=(vp_grid_start.x - vpc.x)*(rmax-roffset)/rmax+roffset;
                    //rtmp=(vp_grid_start.x - vpc.x);

                    rtmp=hypot(vp_grid_start.x - vpc.x,vp_grid_start.y - vpc.y);
                    vp1.x = vpc.x-rtmp;
                    vp1.y = vpc.y+rtmp;
                    vp2.x = vpc.x+rtmp;
                    vp2.y = vpc.y-rtmp;
                    DrawArc(vp1, vp2, (int) rint(180.0/M_PI*(phi_start)),(int) rint(180.0/M_PI*(phi_stop)));

                    /*setcolor(2);
                    vp1.x = vpc.x-rmax;
                    vp1.y = vpc.y+rmax;
                    vp2.x = vpc.x+rmax;
                    vp2.y = vpc.y-rmax;
                    DrawArc(vp1, vp2, (int) rint(180.0/M_PI*(g[gno].phi0)),(int) rint(180.0/M_PI*(2*M_PI+g[gno].phi0)));

                    vp1.x = vpc.x-roffset;
                    vp1.y = vpc.y+roffset;
                    vp2.x = vpc.x+roffset;
                    vp2.y = vpc.y-roffset;
                    DrawArc(vp1, vp2, (int) rint(180.0/M_PI*(g[gno].phi0)),(int) rint(180.0/M_PI*(2*M_PI+g[gno].phi0)));
                    */
                }
                /*else if (!is_yaxis(caxis) && get_graph_type(gno) == GRAPH_POLAR2)///draw x-axis=phi-axis
                {
                    DrawLine(vp_grid_start, vp_grid_stop);
                }*/
                else
                {
                    DrawLine(vp_grid_start, vp_grid_stop);
                }
            }
        }
    }
}

void rotate_viewpoint_around_point(VPoint * p,VPoint center,double angle)
{
static double ca,sa;
static VPoint tmp;
ca=cos(angle);
sa=sin(angle);
tmp.x=p->x-center.x;
tmp.y=p->y-center.y;
p->x=ca*tmp.x+sa*tmp.y;
p->y=-sa*tmp.x+ca*tmp.y;
p->x+=center.x;
p->y+=center.y;
}

void drawaxes(int gno)
{
    int caxis;
    tickmarks *t;
    tickprops tprops;
    world w;
    view v, bb, tmp_bb;
    bool tmp_bb_initialized=false;
    double vbase1, vbase2, vbase1_start, vbase1_stop, vbase2_start, vbase2_stop;
    double vbase_tlabel, vbase_tlabel1, vbase_tlabel2;
    double tsize=0.0, tlsize=0.0, wtpos=0.0, vtpos=0.0;
    double tl_offset, tl_trans;
    WPoint wp1_start, wp1_stop, wp2_start, wp2_stop;
    VPoint vp1_start, vp1_stop, vp2_start, vp2_stop;
    VPoint vp_tick1_start, vp_tick1_stop, vp_tick2_start, vp_tick2_stop;
    VPoint vp_tlabel, vp_label, vp_label_offset1, vp_label_offset2;
    VPoint vpc, vp1, vp2;
    double phi_start, phi_stop, rho;
    VVector ort_para, ort_perp;
    double wc_start, wc_stop, wc_start_labels, wc_stop_labels; /* world coordinates */
    int ittype_loop, itick, itcur;
    int ttype;
    char tlabel[MAX_STRING_LENGTH];
    int tlabel1_just, tlabel2_just, tlabel3_just, label1_just, label2_just, label3_just;
    int langle;
    int tick_dir_sign;
    double (*coord_conv) (double);
    double rtmp;

    vp_tick1_stop.x=vp_tick1_stop.y=0.0;
    setclipping(FALSE);

    /* TODO: add Pen to ticks and remove the following */
    setpattern(1);
    
    get_graph_viewport(gno, &v);
    get_graph_world(gno, &w);

    /* graph center; for polar plots */
    vpc.x = (v.xv1 + v.xv2)/2.0;
    vpc.y = (v.yv1 + v.yv2)/2.0;
    
    ///if (get_graph_type(gno) == GRAPH_POLAR2) return;

    for (caxis = 0; caxis < MAXAXES; caxis++) {
        t = get_graph_tickmarks(gno, caxis);
        tmp_bb_initialized=false;
        if (!t || t->active != TRUE) {
            continue;
        }
        
        if (t->zero == FALSE) {
            tick_dir_sign = +1;
        } else {
            tick_dir_sign = -1;
        }
        
        if (is_xaxis(caxis)) { /* an X-axis */
            ort_para.x = 1.0;
            ort_para.y = 0.0;
            ort_perp.x = 0.0;
            ort_perp.y = 1.0;
            
            coord_conv = xy_xconv;

            wc_start = w.xg1;
            wc_stop = w.xg2;

            wp1_start.x = w.xg1;
            wp1_stop.x  = w.xg2;
            wp2_start.x = w.xg1;
            wp2_stop.x  = w.xg2;
            if (t->zero == TRUE) {
                if (w.yg1 <= 0.0 && w.yg2 >= 0.0) {
                    wp1_start.y = 0.0;
                    wp1_stop.y  = 0.0;
                    wp2_start.y = 0.0;
                    wp2_stop.y  = 0.0;
                } else {
                    continue;
                }
            } else {
                wp1_start.y = w.yg1;
                wp1_stop.y  = w.yg1;
                wp2_start.y = w.yg2;
                wp2_stop.y  = w.yg2;
            }
            
            vp1_start = Wpoint2Vpoint(wp1_start);
            vp1_stop  = Wpoint2Vpoint(wp1_stop);
            vp2_start = Wpoint2Vpoint(wp2_start);
            vp2_stop  = Wpoint2Vpoint(wp2_stop);
            
            if (is_graph_yinvert(gno) == TRUE) {
                vpswap(&vp1_start, &vp2_start);
                vpswap(&vp1_stop, &vp2_stop);
            }

            /* TODO axis offset for polar plots */
            if (get_graph_type(gno) != GRAPH_POLAR) {
                vp1_start.y -= t->offsx;
                vp1_stop.y  -= t->offsx;
                vp2_start.y += t->offsy;
                vp2_stop.y  += t->offsy;
            }

            vbase1 = vp1_start.y;
            vbase2 = vp2_start.y;

            tlabel1_just = JUST_CENTER|JUST_TOP;
            tlabel2_just = JUST_CENTER|JUST_BOTTOM;
            tlabel3_just = JUST_CENTER|JUST_MIDDLE;

            switch (t->label_layout) {
            case LAYOUT_PARALLEL:
                langle =  0;
                break;
            case LAYOUT_PERPENDICULAR:
                langle = 90;
                break;
            default:
                errmsg("Internal error in drawaxes()");
                return;
            }
        } else {              /* a Y-axis */
            ort_para.x = 0.0;
            ort_para.y = 1.0;
            ort_perp.x = 1.0;
            ort_perp.y = 0.0;

            coord_conv = xy_yconv;

            wc_start = w.yg1;
            wc_stop = w.yg2;

            wp1_start.y = w.yg1;
            wp1_stop.y  = w.yg2;
            wp2_start.y = w.yg1;
            wp2_stop.y  = w.yg2;

            if (t->zero == TRUE) {
                if (w.xg1 <= 0.0 && w.xg2 >= 0.0) {
                    wp1_start.x = 0.0;
                    wp1_stop.x  = 0.0;
                    wp2_start.x = 0.0;
                    wp2_stop.x  = 0.0;
                } else {
                    continue;
                }
            } else {
                wp1_start.x = w.xg1;
                wp1_stop.x  = w.xg1;
                wp2_start.x = w.xg2;
                wp2_stop.x  = w.xg2;
            }
            
            vp1_start = Wpoint2Vpoint(wp1_start);
            vp1_stop  = Wpoint2Vpoint(wp1_stop);
            vp2_start = Wpoint2Vpoint(wp2_start);
            vp2_stop  = Wpoint2Vpoint(wp2_stop);

            if (is_graph_xinvert(gno) == TRUE) {
                vpswap(&vp1_start, &vp2_start);
                vpswap(&vp1_stop, &vp2_stop);
            }

            if (get_graph_type(gno) != GRAPH_POLAR) {
                vp1_start.x -= t->offsx;
                vp1_stop.x  -= t->offsx;
                vp2_start.x += t->offsy;
                vp2_stop.x  += t->offsy;
            }

            vbase1 = vp1_start.x;
            vbase2 = vp2_start.x;

            tlabel1_just = JUST_RIGHT|JUST_MIDDLE;
            tlabel2_just = JUST_LEFT|JUST_MIDDLE;
            tlabel3_just = JUST_CENTER|JUST_MIDDLE;

            switch (t->label_layout) {
            case LAYOUT_PARALLEL:
                langle = 90;
                break;
            case LAYOUT_PERPENDICULAR:
                langle =  0;
                break;
            default:
                errmsg("Internal error in drawaxes()");
                return;
            }
        }

        /* Begin axis bar stuff */
        if (t->t_drawbar) {
            setcolor(t->t_drawbarcolor);
            setalpha(t->t_drawbaralpha);
            setlinewidth(t->t_drawbarlinew);
            setlinestyle(t->t_drawbarlines);
            if (t->t_op == PLACEMENT_NORMAL || t->t_op == PLACEMENT_BOTH || (t->t_op == PLACEMENT_OPPOSITE && get_graph_type(gno) == GRAPH_POLAR2)) {
                if (is_xaxis(caxis) && get_graph_type(gno) == GRAPH_POLAR)
                {
                    xy2polar(vp1_start.x - vpc.x, vp1_start.y - vpc.y,
                             &phi_start, &rho);
                    xy2polar(vp1_stop.x - vpc.x, vp1_stop.y - vpc.y,
                             &phi_stop, &rho);
                    vp1.x = vpc.x - rho;
                    vp1.y = vpc.y + rho;
                    vp2.x = vpc.x + rho;
                    vp2.y = vpc.y - rho;
                    if (is_graph_xinvert(gno) == TRUE) {
                        fswap(&phi_start, &phi_stop);
                    }
                    if (phi_stop < phi_start) {
                        phi_stop += 2*M_PI;
                    }
                    DrawArc(vp1, vp2, (int) rint(180.0/M_PI*phi_start),
                            (int) rint(180.0/M_PI*phi_stop));
                }
                else if (is_xaxis(caxis) && get_graph_type(gno) == GRAPH_POLAR2)//phi-axis in PolarV2
                {
                //qDebug() << "Placement PLACEMENT_OPPOSITE, Polar2";
                    phi_start=w.xg1+phi0;
                    phi_stop=w.xg2+phi0;
                    rtmp=hypot(vp1_start.x - vpc.x,vp1_start.y - vpc.y);
                    vp1.x = vpc.x-rtmp;
                    vp1.y = vpc.y+rtmp;
                    vp2.x = vpc.x+rtmp;
                    vp2.y = vpc.y-rtmp;
                    DrawArc(vp1, vp2, (int) rint(180.0/M_PI*(phi_start)),(int) rint(180.0/M_PI*(phi_stop)));
                } else {
                    DrawLine(vp1_start, vp1_stop);
                }
            }
            if (t->t_op == PLACEMENT_OPPOSITE || t->t_op == PLACEMENT_BOTH || (t->t_op == PLACEMENT_NORMAL && get_graph_type(gno) == GRAPH_POLAR2))
            {
                if (is_xaxis(caxis) && get_graph_type(gno) == GRAPH_POLAR)
                {
                    xy2polar(vp2_start.x - vpc.x, vp2_start.y - vpc.y,
                             &phi_start, &rho);
                    xy2polar(vp2_stop.x - vpc.x, vp2_stop.y - vpc.y,
                             &phi_stop, &rho);
                    vp1.x = vpc.x - rho;
                    vp1.y = vpc.y + rho;
                    vp2.x = vpc.x + rho;
                    vp2.y = vpc.y - rho;
                    if (is_graph_xinvert(gno) == TRUE)
                    {
                        fswap(&phi_start, &phi_stop);
                    }
                    if (phi_stop < phi_start)
                    {
                        phi_stop += 2*M_PI;
                    }
                    DrawArc(vp1, vp2, (int) rint(180.0/M_PI*phi_start),
                            (int) rint(180.0/M_PI*phi_stop));
                }
                else if (is_xaxis(caxis) && get_graph_type(gno) == GRAPH_POLAR2)
                {
                //qDebug() << "Placement PLACEMENT_NORMAL, Polar2";
                    phi_start=w.xg1+phi0;
                    phi_stop=w.xg2+phi0;
                    rtmp=hypot(vp2_start.x - vpc.x,vp2_start.y - vpc.y);
                    vp1.x = vpc.x-rtmp;
                    vp1.y = vpc.y+rtmp;
                    vp2.x = vpc.x+rtmp;
                    vp2.y = vpc.y-rtmp;
                    DrawArc(vp1, vp2, (int) rint(180.0/M_PI*(phi_start)),(int) rint(180.0/M_PI*(phi_stop)));
                } else {
                    DrawLine(vp2_start, vp2_stop);
                }
            }
        }
        /* End axis bar stuff*/

        /* TODO ticks, labels and axis labels for polar plots */
        if (get_graph_type(gno) == GRAPH_POLAR)
        {
            continue;
        }

        activate_bbox(BBOX_TYPE_TEMP, TRUE);
        reset_bbox(BBOX_TYPE_TEMP);

        /* Begin axis tick stuff */
        if (t->t_flag)
        {
            for (ittype_loop = 0; ittype_loop < 2; ittype_loop++)
            {

                if (ittype_loop == 0) { /* minor ticks */
                    ttype = TICK_TYPE_MINOR;
                    tprops = t->mprops;
//qDebug() << "Minor Ticks phistart=" << phi_start << " phistop=" << phi_stop;
                } else {      /* major ticks */
                    ttype = TICK_TYPE_MAJOR;
                    tprops = t->props;
//qDebug() << "Major Ticks";
                }
                tsize = 0.02 * tprops.size;

                switch (t->t_inout)
                {
                case TICKS_IN:
                    vbase1_start = vbase1;
                    vbase1_stop  = vbase1 + tick_dir_sign*tsize;
                    vbase2_start = vbase2;
                    vbase2_stop  = vbase2 - tick_dir_sign*tsize;
                    break;
                case TICKS_OUT:
                    vbase1_start = vbase1;
                    vbase1_stop  = vbase1 - tick_dir_sign*tsize;
                    vbase2_start = vbase2;
                    vbase2_stop  = vbase2 + tick_dir_sign*tsize;
                    break;
                case TICKS_BOTH:
                    vbase1_start = vbase1 - tsize;
                    vbase1_stop  = vbase1 + tsize;
                    vbase2_start = vbase2 + tsize;
                    vbase2_stop  = vbase2 - tsize;
                    break;
                default:
                    errmsg("Internal error in drawaxes()");
                    return;
                }
                setcolor(tprops.color);
                setalpha(tprops.alpha);
                setlinewidth(tprops.linew);
                setlinestyle(tprops.lines);

                itcur = 0;
                for (itick = 0; itick < t->nticks; itick++)
                {
                    if (t->tloc[itick].type != ttype)
                    {
                        continue;
                    }

                    wtpos = t->tloc[itick].wtpos;

                    if ((wtpos < wc_start) || (wtpos > wc_stop))
                    {
                        continue;
                    }

                    vtpos = coord_conv(wtpos);
                    if (get_graph_type(gno)==GRAPH_POLAR2)
                    {
                        if (is_yaxis(caxis))//actually the R-axis
                        {
                        polar2xy2(phi_start-phi0,wtpos,&vp_tick1_start.x,&vp_tick1_start.y);
                        vp_tick1_start.x+=vpc.x;
                        vp_tick1_start.y+=vpc.y;
                            if (t->t_op == PLACEMENT_OPPOSITE || t->t_op == PLACEMENT_BOTH)
                            {
                            vp_tick1_stop.x=vp_tick1_start.x+cos(M_PI_2-phi0)*tsize*0.5;
                            vp_tick1_stop.y=vp_tick1_start.y-sin(M_PI_2-phi0)*tsize*0.5;
                            DrawLine(vp_tick1_start, vp_tick1_stop);
                            }
                            if (t->t_op == PLACEMENT_NORMAL || t->t_op == PLACEMENT_BOTH)
                            {
                            vp_tick1_stop.x=vp_tick1_start.x-cos(M_PI_2-phi0)*tsize*0.5;
                            vp_tick1_stop.y=vp_tick1_start.y+sin(M_PI_2-phi0)*tsize*0.5;
                            DrawLine(vp_tick1_start, vp_tick1_stop);
                            }
                        }
                        else//actually the Phi-axis
                        {
                        //qDebug() << "wtpos=" << wtpos;
                            if (t->t_op == PLACEMENT_NORMAL || t->t_op == PLACEMENT_BOTH)
                            {
                            polar2xy2(wtpos,w.yg2,&vp_tick1_start.x,&vp_tick1_start.y);
                            vp_tick1_start.x+=vpc.x;
                            vp_tick1_start.y+=vpc.y;
                                /*if (ittype_loop == 0)
                                DrawFilledCircle(vp_tick1_start,0.005);
                                else
                                DrawFilledCircle(vp_tick1_start,0.01);*/
                            vp_tick1_stop.x=vp_tick1_start.x+cos(wtpos+phi0)*tsize*0.5;
                            vp_tick1_stop.y=vp_tick1_start.y+sin(wtpos+phi0)*tsize*0.5;
                            DrawLine(vp_tick1_start, vp_tick1_stop);
                            }
                            if (t->t_op == PLACEMENT_OPPOSITE || t->t_op == PLACEMENT_BOTH)
                            {
                            polar2xy2(wtpos,w.yg1,&vp_tick1_start.x,&vp_tick1_start.y);
                            vp_tick1_start.x+=vpc.x;
                            vp_tick1_start.y+=vpc.y;
                                /*if (ittype_loop == 0)
                                DrawFilledCircle(vp_tick1_start,0.005);
                                else
                                DrawFilledCircle(vp_tick1_start,0.01);*/
                            vp_tick1_stop.x=vp_tick1_start.x-cos(wtpos+phi0)*tsize*0.5;
                            vp_tick1_stop.y=vp_tick1_start.y-sin(wtpos+phi0)*tsize*0.5;
                            DrawLine(vp_tick1_start, vp_tick1_stop);
                            }
                        }
                    }
                    else
                    {
                    if (t->t_op == PLACEMENT_NORMAL || t->t_op == PLACEMENT_BOTH)
                    {
                        vp_tick1_start.x = vtpos*ort_para.x + vbase1_start*ort_perp.x;
                        vp_tick1_start.y = vtpos*ort_para.y + vbase1_start*ort_perp.y;
                        vp_tick1_stop.x  = vtpos*ort_para.x + vbase1_stop*ort_perp.x;
                        vp_tick1_stop.y  = vtpos*ort_para.y + vbase1_stop*ort_perp.y;
                        DrawLine(vp_tick1_start, vp_tick1_stop);
                    }
                    if (t->t_op == PLACEMENT_OPPOSITE || t->t_op == PLACEMENT_BOTH)
                    {
                        vp_tick2_start.x = vtpos*ort_para.x + vbase2_start*ort_perp.x;
                        vp_tick2_start.y = vtpos*ort_para.y + vbase2_start*ort_perp.y;
                        vp_tick2_stop.x  = vtpos*ort_para.x + vbase2_stop*ort_perp.x;
                        vp_tick2_stop.y  = vtpos*ort_para.y + vbase2_stop*ort_perp.y;
                        DrawLine(vp_tick2_start, vp_tick2_stop);
                    }
                    }
                    itcur++;
                }
            }
        }
        /* End axis ticks stuff */

        /* Make sure we don't end up with an empty BBox if no ticks have
           been drawn */
        vp1.x = v.xv1;
        vp1.y = v.yv1;
        vp2.x = v.xv2;
        vp2.y = v.yv2;
        update_bbox(BBOX_TYPE_TEMP, vp1);
        update_bbox(BBOX_TYPE_TEMP, vp2);

        /* Begin tick label stuff */

        if (t->tl_gaptype==TYPE_AUTO)
        {
            /* hard coded offsets for autoplacement of tick labels */
            tl_trans=0.0;     /* parallel */
            tl_offset=0.01;  /* perpendicular */
            if (get_graph_type(gno)==GRAPH_POLAR2) tl_offset=0.0;//here we calculate this differently
        }
        else
        {
            tl_trans  = t->tl_gap.x;
            tl_offset = t->tl_gap.y;
        }

        if (t->tl_flag)
        {
            if (t->tl_starttype == TYPE_SPEC)
            {
                wc_start_labels = t->tl_start;
            }
            else
            {
                wc_start_labels = wc_start;
            }

            if (t->tl_stoptype == TYPE_SPEC)
            {
                wc_stop_labels = t->tl_stop;
            }
            else
            {
                wc_stop_labels = wc_stop;
            }

            tlsize = 0.02 * t->tl_charsize;
            tsize = 0.02 * t->props.size;

            switch (t->t_inout)
            {
            case TICKS_IN:
                vbase_tlabel1 = vbase1 - (1 - tick_dir_sign)/2*tsize - tl_offset;
                vbase_tlabel2 = vbase2 + (1 - tick_dir_sign)/2*tsize + tl_offset;
                break;
            case TICKS_OUT:
                vbase_tlabel1 = vbase1 - (1 + tick_dir_sign)/2*tsize - tl_offset;
                vbase_tlabel2 = vbase2 + (1 + tick_dir_sign)/2*tsize + tl_offset;
                break;
            case TICKS_BOTH:
                vbase_tlabel1 = vbase1 - tsize - tl_offset;
                vbase_tlabel2 = vbase2 + tsize + tl_offset;
                break;
            default:
                errmsg("Internal error in drawaxes()");
                return;
            }

            setfont(t->tl_font);
            setcharsize(t->tl_charsize);

            itcur = 0;

            switch (t->tl_align)
            {
            case JUST_LEFT:
            currentStringAlignment=Qt::AlignLeft;
            break;
            case JUST_RIGHT:
            currentStringAlignment=Qt::AlignRight;
            break;
            case JUST_CENTER:
            currentStringAlignment=Qt::AlignCenter;
            break;
            }
//char dummy[256];
            for (itick = 0; itick < t->nticks; itick++)
            {
                if (t->tloc[itick].type != TICK_TYPE_MAJOR)
                {
                    continue;
                }

                wtpos = t->tloc[itick].wtpos;
/*sprintf(dummy,"wtpos=%.17f start=%.17f stop=%.17f low_tol=%.17f high_tol=%.17f",wtpos,wc_start_labels,wc_stop_labels,low_tol_val(wc_stop_labels),high_tol_val(wc_stop_labels));
qDebug() << dummy;*/
                if (tolerant_lt(wtpos,wc_start_labels) || tolerant_gt(wtpos,wc_stop_labels))
                {
                    //qDebug() << "DO NOT PRINT lt=" <<tolerant_lt(wtpos,wc_start_labels) << "gt=" << tolerant_gt(wtpos,wc_stop_labels);
                    continue;
                }

                if (t->tl_prestr[0])
                {
                    strcpy(tlabel, t->tl_prestr);
                }
                else
                {
                    tlabel[0] = '\0';
                }
                if (t->tloc[itick].label != NULL)
                {
                    strcat(tlabel, t->tloc[itick].label);
                /*qDebug() << "itick=" << itick <<  " tlabel=" << tlabel << " Orig_label=" << t->tloc[itick].orig_label;*/
                }
                if (t->tl_appstr[0])
                {
                    strcat(tlabel, t->tl_appstr);
                }
                /// qDebug() << "strlen(tlabel)=" << strlen(tlabel);
                vtpos = coord_conv(wtpos);

                if (itcur % (t->tl_skip + 1) == 0)
                {
                    /* Set color before each tick label, since pre/app strings may change it */
                    setcolor(t->tl_color);
                    setalpha(t->tl_alpha);

                    SetDecimalSeparatorToUserValue(tlabel,false);
                    if (get_graph_type(gno)==GRAPH_POLAR2)
                    {
                    //qDebug() << "Tick Label: " << tlabel;
                        if (is_yaxis(caxis))//actually the R-axis
                        {
                        polar2xy2(phi_start-phi0,wtpos,&vp_tick1_start.x,&vp_tick1_start.y);
                        vp_tick1_start.x+=vpc.x;
                        vp_tick1_start.y+=vpc.y;
                            if (t->tl_op == PLACEMENT_OPPOSITE || t->tl_op == PLACEMENT_BOTH)
                            {
                            vp_tick1_stop.x=vp_tick1_start.x+cos(M_PI_2-phi0)*(tsize*1.5+tl_offset)+sin(M_PI_2-phi0)*(tl_trans);
                            vp_tick1_stop.y=vp_tick1_start.y-sin(M_PI_2-phi0)*(tsize*1.5+tl_offset)+cos(M_PI_2-phi0)*(tl_trans);
                                /*if (ittype_loop == 0)
                                DrawFilledCircle(vp_tick1_stop,0.005);
                                else
                                DrawFilledCircle(vp_tick1_stop,0.01);*/
                            //DrawLine(vp_tick1_start, vp_tick1_stop);
                                //if (tlabel && tlabel[0]!='\0')
                                //WriteString(vp_tick1_stop, t->tl_angle, tlabel3_just, tlabel);
                                if (tlabel[0]!='\0')
                                {
                                //qDebug() << "Write " << tlabel << " pos=(" << vp_tick1_stop.x << "|" << vp_tick1_stop.y << ")";
                                WriteString(vp_tick1_stop, t->tl_angle, tlabel3_just, tlabel);
                                return_last_string_bounding_box(&tmp_bb);
                                }
                            }
                            if (t->tl_op == PLACEMENT_NORMAL || t->tl_op == PLACEMENT_BOTH)
                            {
                            vp_tick1_stop.x=vp_tick1_start.x-cos(M_PI_2-phi0)*(tsize*1.5+tl_offset)-sin(M_PI_2-phi0)*(tl_trans);
                            vp_tick1_stop.y=vp_tick1_start.y+sin(M_PI_2-phi0)*(tsize*1.5+tl_offset)-cos(M_PI_2-phi0)*(tl_trans);
                                /*if (ittype_loop == 0)
                                DrawFilledCircle(vp_tick1_stop,0.005);
                                else
                                DrawFilledCircle(vp_tick1_stop,0.01);*/
                            //DrawLine(vp_tick1_start, vp_tick1_stop);
                                //if (tlabel && tlabel[0]!='\0')
                                //WriteString(vp_tick1_stop, t->tl_angle, tlabel3_just, tlabel);
                                if (tlabel[0]!='\0')
                                {
                                //qDebug() << "Write " << tlabel << " pos=(" << vp_tick1_stop.x << "|" << vp_tick1_stop.y << ")";
                                WriteString(vp_tick1_stop, t->tl_angle, tlabel3_just, tlabel);
                                return_last_string_bounding_box(&tmp_bb);
                                }
                            }
                        }
                        else//actually the Phi-axis
                        {
                        //qDebug() << "wtpos=" << wtpos;
                            if (t->tl_op == PLACEMENT_NORMAL || t->tl_op == PLACEMENT_BOTH)
                            {
                            polar2xy2(wtpos,w.yg2,&vp_tick1_start.x,&vp_tick1_start.y);
                            vp_tick1_start.x+=vpc.x;
                            vp_tick1_start.y+=vpc.y;
                                /*if (ittype_loop == 0)
                                DrawFilledCircle(vp_tick1_stop,0.005);
                                else
                                DrawFilledCircle(vp_tick1_stop,0.01);*/
                            vp_tick1_stop.x=vp_tick1_start.x+cos(wtpos+phi0)*(tsize*1.5+tl_offset);
                            vp_tick1_stop.y=vp_tick1_start.y+sin(wtpos+phi0)*(tsize*1.5+tl_offset);
                            //DrawLine(vp_tick1_start, vp_tick1_stop);
                                //if (tlabel && tlabel[0]!='\0')
                                //WriteString(vp_tick1_stop, t->tl_angle, tlabel3_just, tlabel);
                                if (t->tl_gaptype==TYPE_SPEC)//rotate
                                {
                                rotate_viewpoint_around_point(&vp_tick1_stop,vpc,tl_trans);
                                }
                                if (tlabel[0]!='\0')
                                {
                                //qDebug() << "Write " << tlabel << " pos=(" << vp_tick1_stop.x << "|" << vp_tick1_stop.y << ")";
                                WriteString(vp_tick1_stop, t->tl_angle, tlabel3_just, tlabel);
                                return_last_string_bounding_box(&tmp_bb);
                                }
                            }
                            if (t->tl_op == PLACEMENT_OPPOSITE || t->tl_op == PLACEMENT_BOTH)
                            {
                            polar2xy2(wtpos,w.yg1,&vp_tick1_start.x,&vp_tick1_start.y);
                            vp_tick1_start.x+=vpc.x;
                            vp_tick1_start.y+=vpc.y;
                                /*if (ittype_loop == 0)
                                DrawFilledCircle(vp_tick1_stop,0.005);
                                else
                                DrawFilledCircle(vp_tick1_stop,0.01);*/
                            vp_tick1_stop.x=vp_tick1_start.x-cos(wtpos+phi0)*(tsize*1.5+tl_offset);
                            vp_tick1_stop.y=vp_tick1_start.y-sin(wtpos+phi0)*(tsize*1.5+tl_offset);
                            //DrawLine(vp_tick1_start, vp_tick1_stop);
                                //if (tlabel && tlabel[0]!='\0')
                                //WriteString(vp_tick1_stop, t->tl_angle, tlabel3_just, tlabel);
                                if (t->tl_gaptype==TYPE_SPEC)//rotate
                                {
                                rotate_viewpoint_around_point(&vp_tick1_stop,vpc,tl_trans);
                                }
                                if (tlabel[0]!='\0')
                                {
                                //qDebug() << "Write " << tlabel << " pos=(" << vp_tick1_stop.x << "|" << vp_tick1_stop.y << ")";
                                WriteString(vp_tick1_stop, t->tl_angle, tlabel3_just, tlabel);
                                return_last_string_bounding_box(&tmp_bb);
                                }
                            }

                        }

                        /*if (tlabel[0]!='\0')
                        {
                        //qDebug() << "Write " << tlabel << " pos=(" << vp_tick1_stop.x << "|" << vp_tick1_stop.y << ")";
                            WriteString(vp_tick1_stop, t->tl_angle, tlabel3_just, tlabel);
                            return_last_string_bounding_box(&tmp_bb);
                        }*/
                    }
                    else//normal Graphs
                    {
                    /* Tick labels on normal side */
                    if (t->tl_op == PLACEMENT_NORMAL || t->tl_op == PLACEMENT_BOTH)
                    {
                        vbase_tlabel = vbase_tlabel1 - (tl_offset + tlsize)*(itcur % (t->tl_staggered + 1));
                        vp_tlabel.x = (vtpos + tl_trans)*ort_para.x +vbase_tlabel*ort_perp.x;
                        vp_tlabel.y = (vtpos + tl_trans)*ort_para.y +vbase_tlabel*ort_perp.y;
                        /// qDebug() << "Vor Write String TickLabel Normal " << tlabel << "x=" << vp_tlabel.x << " y=" << vp_tlabel.y << "angle=" << t->tl_angle << "just=" << tlabel1_just;
                            WriteString(vp_tlabel, t->tl_angle, tlabel1_just, tlabel);
                        /// qDebug() << "Nach Write String TickLabel Normal";
                        return_last_string_bounding_box(&tmp_bb);
                            if (tmp_bb_initialized==false)
                            {
                            tmp_bb_initialized=true;
                            t->tl_bb=tmp_bb;
                            }
                            else
                            {
                            t->tl_bb=merge_bboxes(t->tl_bb,tmp_bb);
                            }
                    }
                    /* Tick labels on opposite side */
                    if (t->tl_op == PLACEMENT_OPPOSITE || t->tl_op == PLACEMENT_BOTH)
                    {
                        vbase_tlabel = vbase_tlabel2 + (tl_offset + tlsize)*(itcur % (t->tl_staggered + 1));
                        vp_tlabel.x = (vtpos + tl_trans)*ort_para.x +vbase_tlabel*ort_perp.x;
                        vp_tlabel.y = (vtpos + tl_trans)*ort_para.y +vbase_tlabel*ort_perp.y;
                        /// qDebug() << "Vor Write String TickLabel Opposite";
                            WriteString(vp_tlabel, t->tl_angle, tlabel2_just, tlabel);
                        /// qDebug() << "Nach Write String TickLabel Opposite";
                        return_last_string_bounding_box(&tmp_bb);
                    }
                    }
                }
                itcur++;
                t->tloc[itick].bb=tmp_bb;
            }
            currentStringAlignment=Qt::AlignLeft;
        }
        /* End tick label stuff */

        bb = get_bbox(BBOX_TYPE_TEMP);

        /* Begin axis label stuff */
        if (t->label_place == TYPE_SPEC)
        {
            vp_label_offset1.x = t->label.x;
            vp_label_offset1.y = t->label.y;
            vp_label_offset2.x = t->label.x;
            vp_label_offset2.y = t->label.y;
            
            /* These settings are for backward compatibility */
            label1_just = JUST_CENTER|JUST_MIDDLE;
            label2_just = JUST_CENTER|JUST_MIDDLE;
            label3_just = JUST_CENTER|JUST_MIDDLE;
        }
        else
        {
            /* parallel is trivial ;-) */
            vp_label_offset1.x = 0.00;
            vp_label_offset2.x = 0.00;

            /* perpendicular */
            if (is_xaxis(caxis))
            {
                vp_label_offset1.y = vbase1 - bb.yv1;
                vp_label_offset2.y = bb.yv2 - vbase2;
            }
            else
            {
                vp_label_offset1.y = vbase1 - bb.xv1;
                vp_label_offset2.y = bb.xv2 - vbase2;
            }
            
            vp_label_offset1.y += tl_offset;
            vp_label_offset2.y += tl_offset;

            if (get_graph_type(gno)==GRAPH_POLAR2)//we calculate this differently here
            {
                vp_label_offset1.x = 0.0;
                vp_label_offset2.x = 0.0;
                vp_label_offset1.y = 0.0;
                vp_label_offset2.y = 0.0;
            /*vp_label_offset1.x = t->label.x;
            vp_label_offset2.x = t->label.y;
            vp_label_offset1.y = t->label.x;
            vp_label_offset2.y = t->label.y;*/
            }

            label1_just = tlabel1_just;
            label2_just = tlabel2_just;
            label3_just = tlabel3_just;
        }
        /*if (t->label.s_plotstring && t->label.s_plotstring[0])
        {*/ /*we always do the calculation, but we only draw the text, when there is text to draw*/
            setcharsize(t->label.charsize);
            setfont(t->label.font);
            setcolor(t->label.color);
            setalpha(t->label.alpha);
            switch (t->label.align)
            {
            case JUST_LEFT:
            currentStringAlignment=Qt::AlignLeft;
            break;
            case JUST_RIGHT:
            currentStringAlignment=Qt::AlignRight;
            break;
            case JUST_CENTER:
            currentStringAlignment=Qt::AlignCenter;
            break;
            }
            if (get_graph_type(gno)==GRAPH_POLAR2)
            {
                if (is_yaxis(caxis))//actually the R-axis
                {
                polar2xy2(w.xg1,(w.yg2+w.yg1)*0.5,&vp_tick1_start.x,&vp_tick1_start.y);
                vp_tick1_start.x+=vpc.x;
                vp_tick1_start.y+=vpc.y;
                    /*if (t->label_place == TYPE_SPEC)
                    {
                    vp_tick1_start.x+=vp_label_offset1.x;
                    vp_tick1_start.y-=vp_label_offset1.y;
                    }*/
                    //DrawFilledCircle(vp_tick1_start,0.01);
                //langle=phi0*180.0/M_PI;
                    if (t->t_op == PLACEMENT_OPPOSITE || t->t_op == PLACEMENT_BOTH)
                    {
                    vp_tick1_stop.x=vp_tick1_start.x+cos(M_PI_2-phi0)*(tsize*3+tl_offset+(t->label_place==TYPE_SPEC?t->label.y:0.0))+sin(M_PI_2-phi0)*((t->label_place==TYPE_SPEC)?t->label.x:0.0);
                    vp_tick1_stop.y=vp_tick1_start.y-sin(M_PI_2-phi0)*(tsize*3+tl_offset+(t->label_place==TYPE_SPEC?t->label.y:0.0))+cos(M_PI_2-phi0)*((t->label_place==TYPE_SPEC)?t->label.x:0.0);
                        /*if (ittype_loop == 0)
                        DrawFilledCircle(vp_tick1_stop,0.005);
                        else
                        DrawFilledCircle(vp_tick1_stop,0.01);*/
                    //DrawLine(vp_tick1_start, vp_tick1_stop);
                        //WriteString(vp_tick1_stop, t->tl_angle, tlabel3_just, tlabel);
                    }
                    if (t->t_op == PLACEMENT_NORMAL || t->t_op == PLACEMENT_BOTH)
                    {
                    vp_tick1_stop.x=vp_tick1_start.x-cos(M_PI_2-phi0)*(tsize*3+tl_offset+(t->label_place==TYPE_SPEC?t->label.y:0.0))-sin(M_PI_2-phi0)*((t->label_place==TYPE_SPEC)?t->label.x:0.0);
                    vp_tick1_stop.y=vp_tick1_start.y+sin(M_PI_2-phi0)*(tsize*3+tl_offset+(t->label_place==TYPE_SPEC?t->label.y:0.0))-cos(M_PI_2-phi0)*((t->label_place==TYPE_SPEC)?t->label.x:0.0);
                        /*if (ittype_loop == 0)
                        DrawFilledCircle(vp_tick1_stop,0.005);
                        else
                        DrawFilledCircle(vp_tick1_stop,0.01);*/
                    //DrawLine(vp_tick1_start, vp_tick1_stop);
                        //WriteString(vp_tick1_stop, t->tl_angle, tlabel3_just, tlabel);
                    }
                }
                else//actually the Phi-axis
                {
                polar2xy2((w.xg2-w.xg1)*0.5,w.yg2,&vp_tick1_start.x,&vp_tick1_start.y);
                //qDebug() << "wtpos=" << wtpos;
                vp_tick1_start.x+=vpc.x;
                vp_tick1_start.y+=vpc.y;
                /*if (t->label_place == TYPE_SPEC)
                {
                vp_tick1_start.x+=vp_label_offset1.x;
                vp_tick1_start.y+=vp_label_offset1.y;
                }*/
                //langle=-(M_PI_2-(w.xg2-w.xg1)*0.5)*180.0/M_PI;
                    if (t->t_op == PLACEMENT_NORMAL || t->t_op == PLACEMENT_BOTH)
                    {

                    vp_tick1_stop.x=vp_tick1_start.x+cos((w.xg2-w.xg1)*0.5+phi0)*(tsize*3+tl_offset+vp_label_offset1.y);
                    vp_tick1_stop.y=vp_tick1_start.y+sin((w.xg2-w.xg1)*0.5+phi0)*(tsize*3+tl_offset+vp_label_offset1.y);
                        /*if (ittype_loop == 0)
                        DrawFilledCircle(vp_tick1_stop,0.005);
                        else
                        DrawFilledCircle(vp_tick1_stop,0.01);*/
                    }
                    if (t->t_op == PLACEMENT_OPPOSITE || t->t_op == PLACEMENT_BOTH)
                    {

                    vp_tick1_stop.x=vp_tick1_start.x-cos((w.xg2-w.xg1)*0.5+phi0)*(tsize*3+tl_offset+vp_label_offset1.y);
                    vp_tick1_stop.y=vp_tick1_start.y-sin((w.xg2-w.xg1)*0.5+phi0)*(tsize*3+tl_offset+vp_label_offset1.y);
                        /*if (ittype_loop == 0)
                        DrawFilledCircle(vp_tick1_stop,0.005);
                        else
                        DrawFilledCircle(vp_tick1_stop,0.01);*/
                    }
                    if (t->label_place==TYPE_SPEC)//rotate
                    {
                    rotate_viewpoint_around_point(&vp_tick1_stop,vpc,vp_label_offset1.x);
                    }
                }

                    if (t->label.s_plotstring && t->label.s_plotstring[0])
                    {
                    //qDebug() << "Write " << t->label.s_plotstring << " pos=(" << vp_tick1_stop.x << "|" << vp_tick1_stop.y << ")";
                    WriteString(vp_tick1_stop, langle, label3_just, t->label.s_plotstring);
                    }
            }
            else//normal Graphs
            {
            /* Axis label on normal side */
            if (t->label_op == PLACEMENT_NORMAL || t->label_op == PLACEMENT_BOTH)
            {
                vp_label.x = (vp1_start.x + vp1_stop.x)/2
                        + vp_label_offset1.x*ort_para.x
                        - vp_label_offset1.y*ort_perp.x;
                vp_label.y = (vp1_start.y + vp1_stop.y)/2
                        + vp_label_offset1.x*ort_para.y
                        - vp_label_offset1.y*ort_perp.y;
                t->al_vp=vp_label;
                    if (t->label.s_plotstring && t->label.s_plotstring[0])
                    {
                        /// qDebug() << "Vor Write String Axis Label Normal";
                    WriteString(vp_label, langle, label1_just, t->label.s_plotstring);
                        /// qDebug() << "Nach Write String Axis Label Normal";
                    return_last_string_bounding_box(&(t->al_bb));
                    }
            }

            /* Axis label on opposite side */
            if (t->label_op == PLACEMENT_OPPOSITE || t->label_op == PLACEMENT_BOTH)
            {
                vp_label.x = (vp2_start.x + vp2_stop.x)/2
                        + vp_label_offset2.x*ort_para.x
                        + vp_label_offset2.y*ort_perp.x ;
                vp_label.y = (vp2_start.y + vp2_stop.y)/2
                        + vp_label_offset2.x*ort_para.y
                        + vp_label_offset2.y*ort_perp.y ;
                t->al_vp=vp_label;
                    if (t->label.s_plotstring && t->label.s_plotstring[0])
                    {
                        /// qDebug() << "Vor Write String Axis Label Opposite";
                    WriteString(vp_label, langle, label2_just, t->label.s_plotstring);
                        /// qDebug() << "Nach Write String Axis Label Opposite";
                    return_last_string_bounding_box(&(t->al_bb));
                    }
            }
            }
            currentStringAlignment=Qt::AlignLeft;
        /*}*/
    /* End axis label stuff */
    /* //show the BBs of tick labels and axis labels for testing
VPoint vp1,vp2;
    vp1.x=t->tl_bb.xv1;
    vp1.y=t->tl_bb.yv1;
    vp2.x=t->tl_bb.xv2;
    vp2.y=t->tl_bb.yv2;
qDebug() << "Tick_Labels: " << vp1.x << "|" << vp1.y << " - " << vp2.x << "|" << vp2.y;
    DrawRect(vp1,vp2);
    vp1.x=t->al_bb.xv1;
    vp1.y=t->al_bb.yv1;
    vp2.x=t->al_bb.xv2;
    vp2.y=t->al_bb.yv2;
qDebug() << "Axis_Label: " << vp1.x << "|" << vp1.y << " - " << vp2.x << "|" << vp2.y;
    DrawRect(vp1,vp2);*/
    t->label.bb=t->al_bb;
    }
}

void calculate_tickgrid(int gno)
{
    int caxis;
    int itick, imtick, itmaj;
    int nmajor;
    double swc_start, swc_stop, stmajor,dticks;
    int scale;
    double wtmaj;
    world w;
    tickmarks *t;
    int res, len;
    grarr *tvar;
    double *tt;

reenter:
    get_graph_world(gno, &w);
    /*
    Page_geometry pg=get_page_geometry();
    qDebug() << "CalcTickGrid: " << pg.width << "x" << pg.height << " dpi=" << pg.dpi << " factor=" << GeneralPageZoomFactor;
    */
    for (caxis = 0; caxis < MAXAXES; caxis++) {
        t = get_graph_tickmarks(gno, caxis);

        if (!t || t->active != TRUE) {
            continue;
        }

        if (t->t_spec == TICKS_SPEC_NONE) {
            if (is_xaxis(caxis)) {
                scale = get_graph_xscale(gno);
                if (scale == SCALE_LOG) {
                    swc_start = fscale(w.xg1, scale);
                    swc_stop  = fscale(w.xg2, scale);
                } else {
                    swc_start = w.xg1;
                    swc_stop  = w.xg2;
                }
            } else {
                scale = get_graph_yscale(gno);
                if (scale == SCALE_LOG) {
                    swc_start = fscale(w.yg1, scale);
                    swc_stop  = fscale(w.yg2, scale);
                } else {
                    swc_start = w.yg1;
                    swc_stop  = w.yg2;
                }
            }
            if (scale == SCALE_LOG) {
                stmajor = fscale(t->tmajor, scale);
            } else {
                stmajor = t->tmajor;
            }

            if (stmajor <= 0.0) {
                errmsg("Invalid major tick spacing, autoticking");
                autotick_axis(gno, caxis);
                goto reenter;
            }

            if (t->t_round == TRUE) {
                swc_start = floor(swc_start/stmajor)*stmajor;
            }
/*qDebug() << "swc_start=" << swc_start;
qDebug() << "swc_stop=" << swc_stop;
qDebug() << "stmajor=" << stmajor;*/
            dticks =  ceil((swc_stop - swc_start) / stmajor + 1);
//nmajor = (int) ceil((swc_stop - swc_start) / stmajor + 1);
            nmajor = (int) dticks;
            dticks = (dticks-1.0)*(t->nminor*1.0 + 1.0)+1.0;
            t->nticks = (nmajor - 1)*(t->nminor + 1) + 1;
/*qDebug() << "nmajor=" << nmajor;
qDebug() << "dticks=" << dticks;*/
            if (t->nticks > MAX_TICKS || dticks>1.0*MAX_TICKS) {
                errmsg("Too many ticks ( > MAX_TICKS ), autoticking");
                //qDebug() << "Too many ticks ( > MAX_TICKS ), autoticking";
                autotick_axis(gno, caxis);
                goto reenter;
            }

    /*
     *         if (t->nticks > MAX_TICKS) {
     *             t->nticks = MAX_TICKS;
     *         }
     */

            itick = 0;
            itmaj = 0;
            while (itick < t->nticks) {
                if (scale == SCALE_LOG) {
                    wtmaj = ifscale(swc_start + itmaj*stmajor, scale);
                } else {
                    wtmaj = swc_start + itmaj*stmajor;
                    if (t->t_round == TRUE && fabs(wtmaj) < 1.0e-6*stmajor) {
                        wtmaj = 0.0;
                    }
                }
                t->tloc[itick].wtpos = wtmaj;
                t->tloc[itick].type = TICK_TYPE_MAJOR;

                itick++;
                for (imtick = 0; imtick < t->nminor && itick < t->nticks; imtick++) {
                    if (scale == SCALE_LOG) {
                        t->tloc[itick].wtpos = wtmaj * (imtick + 2);
                    } else {
                        t->tloc[itick].wtpos = wtmaj + (imtick + 1)*stmajor/(t->nminor + 1);
                    }
                    t->tloc[itick].type = TICK_TYPE_MINOR;
                    XCFREE(t->tloc[itick].label);
                    itick++;
                }
                itmaj++;
            }
        }

        if (t->t_spec != TICKS_SPEC_BOTH) {
            nmajor = 0;
            for (itick = 0; itick < t->nticks; itick++) {
                if (t->tloc[itick].type == TICK_TYPE_MAJOR) {
                    nmajor++;
                }
            }
            if (t->tl_formula && t->tl_formula[0] != '\0') {

                tvar = get_parser_arr_by_name(dollar_t);
                if (tvar == NULL) {
                    tvar = define_parser_arr(dollar_t);
                    if (tvar == NULL) {
                        errmsg("Internal error");
                        return;
                    }
                }

                if (tvar->length != 0) {
                    xfree(tvar->data);
                    tvar->length = 0;
                }
                tvar->data = (double*)xmalloc(nmajor*sizeof(double));///SIZEOF_DOUBLE);
                if (tvar->data == NULL) {
                    return;
                }
                tvar->length = nmajor;
                
                itmaj = 0;
                for (itick = 0; itick < t->nticks; itick++) {
                    if (t->tloc[itick].type == TICK_TYPE_MAJOR) {
                        tvar->data[itmaj] = t->tloc[itick].wtpos;
                        itmaj++;
                    }
                }

                res = v_scanner(t->tl_formula, &len, &tt);
                XCFREE(tvar->data);
                tvar->length = 0;
                if (res != RETURN_SUCCESS || len != nmajor) {
                    errmsg("Error in tick transformation formula");
                    return;
                }

                itmaj = 0;
                for (itick = 0; itick < t->nticks; itick++) {
                    if (t->tloc[itick].type == TICK_TYPE_MAJOR) {
///cout << "A label=" << t->tloc[itick].label << " format=" << t->tl_format << endl;
                        t->tloc[itick].label = copy_string(t->tloc[itick].label,
                                                           create_fstring(t->tl_format, t->tl_prec,
                                                                          tt[itmaj], LFORMAT_TYPE_EXTENDED));
                    t->tloc[itick].orig_label = copy_string(t->tloc[itick].orig_label,t->tloc[itick].label);/// hier einfeuegt
///cout << "B label=" << t->tloc[itick].label << " format=" << t->tl_format << endl;
                        itmaj++;
                    }
                }
                xfree(tt);
            } else {
                for (itick = 0; itick < t->nticks; itick++) {
                    if (t->tloc[itick].type == TICK_TYPE_MAJOR) {
                        t->tloc[itick].label = copy_string(t->tloc[itick].label,
                                                           create_fstring(t->tl_format, t->tl_prec,
                                                                          t->tloc[itick].wtpos, LFORMAT_TYPE_EXTENDED));
                    t->tloc[itick].orig_label = copy_string(t->tloc[itick].orig_label,t->tloc[itick].label);/// hier einfeuegt
                    }
                }
            }
        }
    }
}
