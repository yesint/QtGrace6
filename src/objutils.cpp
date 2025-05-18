/*
 * Grace - GRaphing, Advanced Computation and Exploration of data
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
 * operations on objects (strings, lines, and boxes)
 *
 */

///#include <config.h>
#include <cmath>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "draw.h"
#include "globals.h"
#include "graphs.h"
#include "utils.h"
#include "noxprotos.h"
#include "allWidgets.h"
#include "windowWidgets.h"

#include <QObject>

#include <iostream>

using namespace std;

extern void CheckLaTeXLinesForAddress(char * o_adr,char * n_adr);

extern frmEllipseProps * EditEllipseProps;
extern frmEllipseProps * EditBoxProps;
extern frmTextProps * EditTextProps;
extern frmLineProps * EditLineProps;

int maxboxes = 0;
int maxlines = 0;
int maxstr = 0;
int maxellipses = 0;
int swap_line_coords = 0;

int number_of_lines(void)
{
    return maxlines;
}

int number_of_boxes(void)
{
    return maxboxes;
}

int number_of_ellipses(void)
{
    return maxellipses;
}

int number_of_strings(void)
{
    return maxstr;
}

int is_valid_line(int line)
{
    return (line >= 0 && line < maxlines);
}

int is_valid_box(int box)
{
    return (box >= 0 && box < maxboxes);
}

int is_valid_ellipse(int ellipse)
{
    return (ellipse >= 0 && ellipse < maxellipses);
}

int is_valid_string(int string)
{
    return (string >= 0 && string < maxstr);
}

int find_objects_associated_with_graph(int ** id,int ** type,int gno)
{
int max_size=number_of_boxes()+number_of_lines()+number_of_ellipses()+number_of_strings();
if (max_size==0) return 0;//no objects --> nothing to do here
int i,index=0;
int * s_id=new int[max_size];
int * s_type=new int[max_size];
    for (i = 0; i < number_of_boxes(); i++)
    {
        if (boxes[i].gno==gno)
        {
        s_type[index]=OBJECT_BOX;
        s_id[index++]=i;
        }
    }
    for (i = 0; i < number_of_ellipses(); i++)
    {
        if (ellip[i].gno==gno)
        {
        s_type[index]=OBJECT_ELLIPSE;
        s_id[index++]=i;
        }
    }
    for (i = 0; i < number_of_lines(); i++)
    {
        if (lines[i].gno==gno)
        {
        s_type[index]=OBJECT_LINE;
        s_id[index++]=i;
        }
    }
    for (i = 0; i < number_of_strings(); i++)
    {
        if (pstr[i].gno==gno)
        {
        s_type[index]=OBJECT_STRING;
        s_id[index++]=i;
        }
    }
if (index==0) return 0;//no suitable objects found
if (*id!=NULL) delete[] *id;
if (*type!=NULL) delete[] *type;
*id=new int[index];
*type=new int[index];
memcpy(*id,s_id,sizeof(int)*index);
memcpy(*type,s_type,sizeof(int)*index);
delete[] s_id;
delete[] s_type;
return index;
}

void change_object_association(int a,int b)
{
    int i;
        for (i = 0; i < number_of_boxes(); i++)
        {
            if (boxes[i].gno==a) boxes[i].gno=b;
        }
        for (i = 0; i < number_of_ellipses(); i++)
        {
            if (ellip[i].gno==a) ellip[i].gno=b;
        }
        for (i = 0; i < number_of_lines(); i++)
        {
            if (lines[i].gno==a) lines[i].gno=b;
        }
        for (i = 0; i < number_of_strings(); i++)
        {
            if (pstr[i].gno==a) pstr[i].gno=b;
        }
}

void swap_object_association(int a,int b)
{
int i;
    for (i = 0; i < number_of_boxes(); i++)
    {
        if (boxes[i].gno==a) boxes[i].gno=b;
        else if (boxes[i].gno==b) boxes[i].gno=a;
    }
    for (i = 0; i < number_of_ellipses(); i++)
    {
        if (ellip[i].gno==a) ellip[i].gno=b;
        else if (ellip[i].gno==b) ellip[i].gno=a;
    }
    for (i = 0; i < number_of_lines(); i++)
    {
        if (lines[i].gno==a) lines[i].gno=b;
        else if (lines[i].gno==b) lines[i].gno=a;
    }
    for (i = 0; i < number_of_strings(); i++)
    {
        if (pstr[i].gno==a) pstr[i].gno=b;
        else if (pstr[i].gno==b) pstr[i].gno=a;
    }
}

/*tries to find out whether the edit-dialog for a specific object is opened. If the object has been deleted-->close dialog; if object has been edited-->update dialog*/
void check_for_dialog_open(int type,int id,int deleted)
{
    switch (type)
    {
    case OBJECT_BOX:
        if (EditBoxProps!=NULL)
        {
            if (EditBoxProps->flp->obj_id==id && EditBoxProps->flp->ellipse==false)
            {
                if (deleted==TRUE)
                {
                EditBoxProps->hide();
                }
                else
                {
                EditBoxProps->init(id);
                }
            }
        }
    break;
    case OBJECT_ELLIPSE:
        if (EditEllipseProps!=NULL)
        {
            if (EditEllipseProps->flp->obj_id==id && EditEllipseProps->flp->ellipse==true)
            {
                if (deleted==TRUE)
                {
                EditEllipseProps->hide();
                }
                else
                {
                EditEllipseProps->init(id);
                }
            }
        }
    break;
    case OBJECT_STRING:
        if (EditTextProps!=NULL)
        {
            if (EditTextProps->flp->obj_id==id)
            {
                if (deleted==TRUE)
                {
                EditTextProps->hide();
                }
                else
                {
                EditTextProps->init(id);
                }
            }
        }
    break;
    case OBJECT_LINE:
        if (EditLineProps!=NULL)
        {
            if (EditLineProps->flp->obj_id==id)
            {
                if (deleted==TRUE)
                {
                EditLineProps->hide();
                }
                else
                {
                EditLineProps->init(id);
                }
            }
        }
    break;
    }
}

void move_object(int type, int id, VVector shift)
{
    double xtmp, ytmp;
    boxtype box;
    ellipsetype ellipse;
    linetype line;
    plotstr str;

    switch (type) {
    case OBJECT_BOX:
	if (isactive_box(id)) {
	    get_graph_box(id, &box);
	    if (box.loctype == COORD_VIEW) {
		box.x1 += shift.x;
		box.y1 += shift.y;
		box.x2 += shift.x;
		box.y2 += shift.y;
	    } else {
		world2view(box.x1, box.y1, &xtmp, &ytmp);
		xtmp += shift.x;
		ytmp += shift.y;
                view2world(xtmp, ytmp, &box.x1, &box.y1);
		world2view(box.x2, box.y2, &xtmp, &ytmp);
		xtmp += shift.x;
		ytmp += shift.y;
                view2world(xtmp, ytmp, &box.x2, &box.y2);
	    }
            set_graph_box(id, &box);
            set_dirtystate();
        }
	break;
    case OBJECT_ELLIPSE:
	if (isactive_ellipse(id)) {
	    get_graph_ellipse(id, &ellipse);
	    if (ellipse.loctype == COORD_VIEW) {
		ellipse.x1 += shift.x;
		ellipse.y1 += shift.y;
		ellipse.x2 += shift.x;
		ellipse.y2 += shift.y;
	    } else {
		world2view(ellipse.x1, ellipse.y1, &xtmp, &ytmp);
		xtmp += shift.x;
		ytmp += shift.y;
                view2world(xtmp, ytmp, &ellipse.x1, &ellipse.y1);
		world2view(ellipse.x2, ellipse.y2, &xtmp, &ytmp);
		xtmp += shift.x;
		ytmp += shift.y;
                view2world(xtmp, ytmp, &ellipse.x2, &ellipse.y2);
	    }
            set_graph_ellipse(id, &ellipse);
            set_dirtystate();
        }
	break;
    case OBJECT_LINE:
	if (isactive_line(id)) {
	    get_graph_line(id, &line);
	    if (line.loctype == COORD_VIEW) {
		line.x1 += shift.x;
		line.y1 += shift.y;
		line.x2 += shift.x;
		line.y2 += shift.y;
	    } else {
		world2view(line.x1, line.y1, &xtmp, &ytmp);
		xtmp += shift.x;
		ytmp += shift.y;
                view2world(xtmp, ytmp, &line.x1, &line.y1);
		world2view(line.x2, line.y2, &xtmp, &ytmp);
		xtmp += shift.x;
		ytmp += shift.y;
                view2world(xtmp, ytmp, &line.x2, &line.y2);
	    }
            set_graph_line(id, &line);
            set_dirtystate();
        }
	break;
    case OBJECT_STRING:
	if (isactive_string(id)) {
	    get_graph_string(id, &str);
	    if (str.loctype == COORD_VIEW) {
		str.x += shift.x;
		str.y += shift.y;
	    } else {
		world2view(str.x, str.y, &xtmp, &ytmp);
		xtmp += shift.x;
		ytmp += shift.y;
                view2world(xtmp, ytmp, &str.x, &str.y);
	    }
            set_graph_string(id, &str);
            set_dirtystate();
        }
	break;
    }
check_for_dialog_open(type,id,false);
}

int isactive_line(int lineno)
{
    if (is_valid_line(lineno)) {
	return lines[lineno].active;
    } else {
        return FALSE;
    }
}

int isactive_box(int boxno)
{
    if (is_valid_box(boxno)) {
	return boxes[boxno].active;
    } else {
        return FALSE;
    }
}

int isactive_ellipse(int ellipno)
{
    if (is_valid_ellipse(ellipno)) {
	return ellip[ellipno].active;
    } else {
        return FALSE;
    }
}

int isactive_string(int strno)
{
    if (is_valid_string(strno) && pstr[strno].s_plotstring && pstr[strno].s_plotstring[0]) {
	return TRUE;
    } else {
        return FALSE;
    }
}


int next_line(void)
{
    int i, maxold;

    for (i = 0; i < maxlines; i++) {
	if (!isactive_line(i)) {
	    lines[i].active = TRUE;
	    set_dirtystate();
	    return (i);
	}
    }
    maxold = maxlines;
    if (realloc_lines(maxlines + OBJECT_BUFNUM) == RETURN_SUCCESS) {
        return maxold;
    } else {
        errmsg(QObject::tr("Error - no lines available").toLocal8Bit().constData());
        return (-1);
    }
}

int next_box(void)
{
    int i, maxold;

    for (i = 0; i < maxboxes; i++) {
	if (!isactive_box(i)) {
	    boxes[i].active = TRUE;
	    set_dirtystate();
	    return (i);
	}
    }
    maxold = maxboxes;
    if (realloc_boxes(maxboxes + OBJECT_BUFNUM) == RETURN_SUCCESS) {
        return maxold;
    } else {
        errmsg(QObject::tr("Error - no boxes available").toLocal8Bit().constData());
        return (-1);
    }
}

int next_string(void)
{
    int i, maxold;

    for (i = 0; i < maxstr; i++) {
	if (!isactive_string(i)) {
	    set_dirtystate();
	    return (i);
	}
    }
    maxold = maxstr;
    if (realloc_strings(maxstr + OBJECT_BUFNUM) == RETURN_SUCCESS) {
        return maxold;
    } else {
        errmsg(QObject::tr("Error - no strings available").toLocal8Bit().constData());
        return (-1);
    }
}

int next_ellipse(void)
{
    int i, maxold;

    for (i = 0; i < maxellipses; i++) {
	if (!isactive_ellipse(i)) {
	    ellip[i].active = TRUE;
	    set_dirtystate();
	    return (i);
	}
    }
    maxold = maxellipses;
    if (realloc_ellipses(maxellipses + OBJECT_BUFNUM) == RETURN_SUCCESS) {
        return maxold;
    } else {
        errmsg(QObject::tr("Error - no ellipses available").toLocal8Bit().constData());
        return (-1);
    }
}

int next_object(int type)
{
    switch (type) {
    case OBJECT_BOX:
        return next_box();
        break;
    case OBJECT_ELLIPSE:
        return next_ellipse();
        break;
    case OBJECT_LINE:
        return next_line();
        break;
    case OBJECT_STRING:
        return next_string();
        break;
    default:
        return -1;
        break;
    }
}

int set_object_active(int type, int id, int active)
{
    switch (type) {
    case OBJECT_BOX:
            if (is_valid_box(id))
            boxes[id].active=active;
            else
            return RETURN_FAILURE;
        break;
    case OBJECT_ELLIPSE:
            if (is_valid_ellipse(id))
            ellip[id].active=active;
            else
            return RETURN_FAILURE;
        break;
    case OBJECT_LINE:
            if (is_valid_line(id))
            lines[id].active=active;
            else
            return RETURN_FAILURE;
        break;
    case OBJECT_STRING:
            if (is_valid_string(id))
            pstr[id].active=active;
            else
            return RETURN_FAILURE;
        break;
    default:
        return RETURN_FAILURE;
        break;
    }
    return RETURN_SUCCESS;
}

int kill_object(int type, int id)
{
    switch (type) {
    case OBJECT_BOX:
        kill_box(id);
        break;
    case OBJECT_ELLIPSE:
        kill_ellipse(id);
        break;
    case OBJECT_LINE:
        kill_line(id);
        break;
    case OBJECT_STRING:
        kill_string(id);
        break;
    default:
        return RETURN_FAILURE;
        break;
    }
check_for_dialog_open(type,id,true);
return RETURN_SUCCESS;
}

void kill_all_objects_linked_to_graph(int gno)
{
    for (int i=maxboxes-1;i>=0;i--)
    {
        if (boxes[i].gno==gno)
        {
        (void)kill_object(OBJECT_BOX,i);
        }
    }
    for (int i=maxlines-1;i>=0;i--)
    {
        if (lines[i].gno==gno)
        {
        (void)kill_object(OBJECT_LINE,i);
        }
    }
    for (int i=maxstr-1;i>=0;i--)
    {
        if (pstr[i].gno==gno)
        {
        (void)kill_object(OBJECT_STRING,i);
        }
    }
    for (int i=maxellipses-1;i>=0;i--)
    {
        if (ellip[i].gno==gno)
        {
        (void)kill_object(OBJECT_ELLIPSE,i);
        }
    }
}

void copy_object(int type, int from, int to)
{
    switch (type) {
	case OBJECT_BOX:
	boxes[to] = boxes[from];
	break;
 	case OBJECT_ELLIPSE:
	ellip[to] = ellip[from];
	break;
    case OBJECT_LINE:
	lines[to] = lines[from];
	break;
    case OBJECT_STRING:
	kill_string(to);
	pstr[to] = pstr[from];
    pstr[to].s_plotstring = copy_string(NULL, pstr[from].s_plotstring);
    pstr[to].alt_plotstring = copy_string(NULL, pstr[from].alt_plotstring);
	break;
    }
    set_dirtystate();
}

int duplicate_object(int type, int id)
{
int newid;
    if ((newid = next_object(type)) >= 0)
    {
        copy_object(type, id, newid);
    }
    else
    {
        newid = -1;
    }
return newid;
}

void kill_box(int boxno)
{
    boxes[boxno].active = FALSE;
    set_dirtystate();
}

void kill_ellipse(int ellipseno)
{
    ellip[ellipseno].active = FALSE;
    set_dirtystate();
}

void kill_line(int lineno)
{
    lines[lineno].active = FALSE;
    set_dirtystate();
}

void kill_string(int stringno)
{
    CheckLaTeXLinesForAddress(pstr[stringno].s_plotstring,NULL);
    CheckLaTeXLinesForAddress(pstr[stringno].alt_plotstring,NULL);
    XCFREE(pstr[stringno].s_plotstring);
    XCFREE(pstr[stringno].alt_plotstring);
    pstr[stringno].active = FALSE;
    set_dirtystate();
}

int get_object_bb(int type, int id, view *bb)
{
    switch (type) {
    case OBJECT_BOX:
        *bb = boxes[id].bb;
        break;
    case OBJECT_ELLIPSE:
        *bb = ellip[id].bb;
        break;
    case OBJECT_LINE:
        *bb = lines[id].bb;
        break;
    case OBJECT_STRING:
        *bb = pstr[id].bb;
        break;
    default:
        return RETURN_FAILURE;
        break;
    }
    return RETURN_SUCCESS;
}

void set_plotstr_string(plotstr *pstr, char *buf)
{
    pstr->s_plotstring = copy_string(pstr->s_plotstring, buf);//the Grace-version to be printed
    pstr->alt_plotstring = copy_string(pstr->alt_plotstring, buf);//the original version in UTF-8 encoding
}

void init_line(int id, VPoint vp1, VPoint vp2)
{
    if (id < 0 || id > number_of_lines())
    {
    return;
    }
    lines[id].active = TRUE;
    lines[id].color = line_color;
    lines[id].lines = line_lines;
    lines[id].linew = line_linew;
    lines[id].loctype = line_loctype;
    if (line_loctype == COORD_VIEW)
    {
        lines[id].x1 = vp1.x;
        lines[id].y1 = vp1.y;
        lines[id].x2 = vp2.x;
        lines[id].y2 = vp2.y;
        lines[id].gno = -1;
    }
    else
    {
        lines[id].gno = get_cg();
        view2world(vp1.x, vp1.y, &lines[id].x1, &lines[id].y1);
        view2world(vp2.x, vp2.y, &lines[id].x2, &lines[id].y2);
    }
    if (swap_line_coords==1)
    {
    double tmp;
    tmp=lines[id].x1;
    lines[id].x1=lines[id].x2;
    lines[id].x2=tmp;
    tmp=lines[id].y1;
    lines[id].y1=lines[id].y2;
    lines[id].y2=tmp;
    swap_line_coords=0;
    }
    lines[id].arrow_end = line_arrow_end;
    set_default_arrow(&lines[id].arrow);
    set_dirtystate();
}

void init_box(int id, VPoint vp1, VPoint vp2)
{
    if (id < 0 || id > number_of_boxes())
    {
        return;
    }
    boxes[id].color = box_color;
    boxes[id].fillcolor = box_fillcolor;
    boxes[id].fillpattern = box_fillpat;
    boxes[id].lines = box_lines;
    boxes[id].linew = box_linew;
    boxes[id].loctype = box_loctype;
    boxes[id].active = TRUE;
    boxes[id].rot = 0.0;
    if (box_loctype == COORD_VIEW)
    {
        boxes[id].x1 = vp1.x;
        boxes[id].y1 = vp1.y;
        boxes[id].x2 = vp2.x;
        boxes[id].y2 = vp2.y;
        boxes[id].gno = 0;//we need a default that always exists
    }
    else
    {
        boxes[id].gno = get_cg();
        view2world(vp1.x, vp1.y, &boxes[id].x1, &boxes[id].y1);
        view2world(vp2.x, vp2.y, &boxes[id].x2, &boxes[id].y2);
    }
    set_dirtystate();
}

void init_ellipse(int id, VPoint vp1, VPoint vp2)
{
    if (id < 0 || id > number_of_ellipses())
    {
        return;
    }
    ellip[id].color = ellipse_color;
    ellip[id].fillcolor = ellipse_fillcolor;
    ellip[id].fillpattern = ellipse_fillpat;
    ellip[id].lines = ellipse_lines;
    ellip[id].linew = ellipse_linew;
    ellip[id].loctype = ellipse_loctype;
    ellip[id].active = TRUE;
    ellip[id].rot = 0.0;
    if (ellipse_loctype == COORD_VIEW)
    {
        ellip[id].x1 = vp1.x;
        ellip[id].y1 = vp1.y;
        ellip[id].x2 = vp2.x;
        ellip[id].y2 = vp2.y;
        ellip[id].gno = 0;
    }
    else
    {
        ellip[id].gno = get_cg();
        view2world(vp1.x, vp1.y, &ellip[id].x1, &ellip[id].y1);
        view2world(vp2.x, vp2.y, &ellip[id].x2, &ellip[id].y2);
    }
    set_dirtystate();
}

void init_string(int id, VPoint vp)
{
    if (id < 0 || id > number_of_strings())
    {
        return;
    }
    pstr[id].s_plotstring = copy_string(NULL, "\0");
    pstr[id].alt_plotstring = copy_string(NULL, "\0");
    pstr[id].font = string_font;
    pstr[id].color = string_color;
    pstr[id].rot = string_rot;
    pstr[id].charsize = string_size;
    pstr[id].loctype = string_loctype;
    pstr[id].just = string_just;
    pstr[id].align = string_align;
    pstr[id].master_align = string_master_align;
    pstr[id].active = TRUE;
    if (string_loctype == COORD_VIEW)
    {
        pstr[id].x = vp.x;
        pstr[id].y = vp.y;
        pstr[id].gno = 0;
    }
    else
    {
        pstr[id].gno = get_cg();
        view2world(vp.x, vp.y, &pstr[id].x, &pstr[id].y);
    }
    set_dirtystate();
}

void do_clear_lines(void)
{
    int i;

    for (i = 0; i < maxlines; i++) {
	kill_line(i);
    }
}

void do_clear_boxes(void)
{
    int i;

    for (i = 0; i < maxboxes; i++) {
	kill_box(i);
    }
}

void do_clear_ellipses(void)
{
    int i;

    for (i = 0; i < maxellipses; i++) {
	kill_ellipse(i);
    }
}

void do_clear_text(void)
{
    int i;

    for (i = 0; i < maxstr; i++) {
	kill_string(i);
    }
}

int realloc_lines(int n)
{
    int i;
    void *ptmp;

    if (n > maxlines) {
	ptmp = xrealloc(lines, n * sizeof(linetype));
	if (ptmp != NULL) {
            lines = (linetype*)ptmp;
            for (i = maxlines; i < n; i++) {
	        set_default_line(&lines[i]);
	    }
	    maxlines = n;
            return RETURN_SUCCESS;
        } else {
            return RETURN_FAILURE;
        }
    }
    
    return RETURN_SUCCESS;
}

int realloc_boxes(int n)
{
    int i;
    void *ptmp;
    
    if (n > maxboxes) {
	ptmp = xrealloc(boxes, n * sizeof(boxtype));
	if (ptmp != NULL) {
            boxes = (boxtype*)ptmp;
            for (i = maxboxes; i < n; i++) {
	        set_default_box(&boxes[i]);
	    }
	    maxboxes = n;
            return RETURN_SUCCESS;
        } else {
            return RETURN_FAILURE;
        }
    }
    
    return RETURN_SUCCESS;
}

int realloc_ellipses(int n)
{
    int i;
    void *ptmp;
    
    if (n > maxellipses) {
	ptmp = xrealloc(ellip, n * sizeof(ellipsetype));
	if (ptmp != NULL) {
            ellip = (ellipsetype*)ptmp;
            for (i = maxellipses; i < n; i++) {
	        set_default_ellipse(&ellip[i]);
	    }
	    maxellipses = n;
            return RETURN_SUCCESS;
        } else {
            return RETURN_FAILURE;
        }
    }
    
    return RETURN_SUCCESS;
}

int realloc_strings(int n)
{
int i;
void *ptmp;
    if (n > maxstr)
    {
    char ** old_s=new char*[maxstr];
    char ** old_alt=new char*[maxstr];
        for (i=0;i<maxstr;i++)
        {
        old_s[i]=pstr[i].s_plotstring;//save old addresses
        old_alt[i]=pstr[i].alt_plotstring;
        }
	ptmp = xrealloc(pstr, n * sizeof(plotstr));
        if (ptmp != NULL)
        {
            pstr = (plotstr*)ptmp;
            for (i = maxstr; i < n; i++)
            {
	        set_default_string(&pstr[i]);
            }
            for (i=0;i<maxstr;i++)
            {
            CheckLaTeXLinesForAddress(old_s[i],pstr[i].s_plotstring);
            CheckLaTeXLinesForAddress(old_alt[i],pstr[i].alt_plotstring);
            }
	    maxstr = n;
            delete[] old_s;
            delete[] old_alt;
        return RETURN_SUCCESS;
        }
        else
        {
        errmsg(QObject::tr("Can not reallocate strings - not enough free memory?").toLocal8Bit().constData());
        //cout << "ERROR: REALLOCATION NOT SUCCESSFULL!" << endl;
            delete[] old_s;
            delete[] old_alt;
        return RETURN_FAILURE;
        }
    }
    return RETURN_SUCCESS;
}

void get_graph_box(int i, boxtype * b)
{
    memcpy(b, &boxes[i], sizeof(boxtype));
}

void get_graph_ellipse(int i, ellipsetype * b)
{
    memcpy(b, &ellip[i], sizeof(ellipsetype));
}

void get_graph_line(int i, linetype * l)
{
    memcpy(l, &lines[i], sizeof(linetype));
}

void get_graph_string(int i, plotstr * s)
{
    memcpy(s, &pstr[i], sizeof(plotstr));
}

void set_graph_box(int i, boxtype * b)
{
    memcpy(&boxes[i], b, sizeof(boxtype));
}

void set_graph_line(int i, linetype * l)
{
    memcpy(&lines[i], l, sizeof(linetype));
}

void set_graph_ellipse(int i, ellipsetype * e)
{
    memcpy(&ellip[i], e, sizeof(ellipsetype));
}

void set_graph_string(int i, plotstr * s)
{
    memcpy(&pstr[i], s, sizeof(plotstr));
}

const char *object_types(int type)
{
const char *stype;
    switch (type)
    {
    case OBJECT_LINE:
        stype = "line";
        break;
    case OBJECT_BOX:
        stype = "box";
        break;
    case OBJECT_ELLIPSE:
        stype = "ellipse";
        break;
    case OBJECT_STRING:
        stype = "string";
        break;
    default:
        stype = "";
        break;
    }
return stype;
}
