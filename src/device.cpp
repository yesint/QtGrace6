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

///#include <config.h>

#include <cstdlib>
#include <cstring>
#include <iostream>

#include "defines.h"
#include "globals.h"
#include "graphutils.h"
#include "utils.h"
#include "device.h"

using namespace std;

extern double rint_v2(double x);
extern double GeneralPageZoomFactor;
//extern long orig_page_w,orig_page_h;
unsigned int ndevices = 0;
int curdevice = DEVICE_SCREEN;
Device_entry *device_table = NULL;

Device_entry dev_null = {DEVICE_NULL,
          "NULL",
          NULL,
          NULL,
          NULL,
          "",
          FALSE,
          TRUE,
          {DEFAULT_PAGE_WIDTH, DEFAULT_PAGE_HEIGHT, 72.0},
          NULL,
          1,
          ""
         };

int is_valid_page_geometry(Page_geometry pg)
{
    if (pg.width  > 0 &&
	pg.height > 0 &&
        pg.dpi > 0.0) {
	return TRUE;
    } else {
        return FALSE;
    }
}

int set_page_geometry(Page_geometry pg)
{
    if (is_valid_page_geometry(pg) == TRUE) {
        device_table[curdevice].pg = pg;
	return RETURN_SUCCESS;
    } else {
        return RETURN_FAILURE;
    }
}

Page_geometry get_page_geometry(void)
{
/*static Page_geometry pg2;
pg2=device_table[curdevice].pg;
pg2.height=(unsigned long)(pg2.height*GeneralPageZoomFactor);
pg2.width=(unsigned long)(pg2.width*GeneralPageZoomFactor);
return pg2;*/
return (device_table[curdevice].pg);
}

int set_page_dimensions(int wpp, int hpp, int rescale)
{
unsigned int i;
    if (wpp <= 0 || hpp <= 0)
    {
        return RETURN_FAILURE;
    }
    else
    {
        if (rescale)
        {
            int wpp_old=0, hpp_old=0;
            
            get_device_page_dimensions(curdevice, &wpp_old, &hpp_old);
            if (hpp*wpp_old - wpp*hpp_old != 0) {
                /* aspect ratio changed */
                double ext_x, ext_y;
                double old_aspectr, new_aspectr;
                
                old_aspectr = (double) wpp_old/hpp_old;
                new_aspectr = (double) wpp/hpp;
                if (old_aspectr >= 1.0 && new_aspectr >= 1.0) {
                    ext_x = new_aspectr/old_aspectr;
                    ext_y = 1.0;
                } else if (old_aspectr <= 1.0 && new_aspectr <= 1.0) {
                    ext_x = 1.0;
                    ext_y = old_aspectr/new_aspectr;
                } else if (old_aspectr >= 1.0 && new_aspectr <= 1.0) {
                    ext_x = 1.0/old_aspectr;
                    ext_y = 1.0/new_aspectr;
                } else {
                    ext_x = new_aspectr;
                    ext_y = old_aspectr;
                }

                rescale_viewport(ext_x, ext_y);
            } 
        }
        for (i = 0; i < ndevices; i++)
        {
            Page_geometry *pg = &device_table[i].pg;
            pg->width  = (unsigned long) rint_v2((double) wpp*(pg->dpi/72));
            pg->height = (unsigned long) rint_v2((double) hpp*(pg->dpi/72));
        //cout << "i=" << i << " w=" << pg->width << " h=" << pg->height << " dpi=" << pg->dpi << endl;
            /*if (i==DEVICE_SCREEN)
            {
            orig_page_w=pg->width;
            orig_page_h=pg->height;
            device_table[DEVICE_SCREEN].pg.width=orig_page_w*GeneralPageZoomFactor;
            device_table[DEVICE_SCREEN].pg.height=orig_page_h*GeneralPageZoomFactor;
            }*/
        }
        return RETURN_SUCCESS;
    }
}

int get_device_page_dimensions(int dindex, int *wpp, int *hpp)
{
    if (dindex >= (int)ndevices || dindex < 0) {
        return RETURN_FAILURE;
    } else {
        Page_geometry *pg = &device_table[dindex].pg;
        *wpp = (int) rint_v2((double) pg->width*72/pg->dpi);
        *hpp = (int) rint_v2((double) pg->height*72/pg->dpi);
        return RETURN_SUCCESS;
    }
}

int register_device(Device_entry device)
{
    int dindex;
    
    ndevices++;
    dindex = ndevices - 1;
    device_table = (Device_entry*)xrealloc(device_table, ndevices*sizeof(Device_entry));

    device_table[dindex] = device;
    device_table[dindex].name = copy_string(NULL, device.name);
    device_table[dindex].fext = copy_string(NULL, device.fext);
    
    return dindex;
}

int select_device(int dindex)
{
    if (dindex >= (int)ndevices || dindex < 0) {
        return RETURN_FAILURE;
    } else {
        //cout << "select: " << dindex << " ndevices=" << ndevices << endl;
        curdevice = dindex;
	return RETURN_SUCCESS;
    }
}

/*
 * set the current print device
 */
int set_printer(int device)
{
    if (device >= (int)ndevices || device < 0 ||
        device_table[device].type == DEVICE_TERM) {
        return RETURN_FAILURE;
    } else {
        hdevice = device;
        if (device_table[device].type != DEVICE_PRINT) {
            set_ptofile(TRUE);
        }
	return RETURN_SUCCESS;
    }
}

int set_printer_by_name(const char *dname)
{
    int device;
    
    device = get_device_by_name(dname);
    
    return set_printer(device);
}

int get_device_by_name(const char *dname)
{
    int i,len=strlen(dname);
    char * up_dname=new char[len+2];
        for (i=0;i<len;i++)
        up_dname[i]=toupper(dname[i]);
        up_dname[len]='\0';
    i = 0;
    while (i < (int)ndevices) {
        if (strncmp(device_table[i].name, dname, len) == 0 || strncmp(device_table[i].name, up_dname, len) == 0) {
            break;
        } else if (strncmp(device_table[i].alt_name, dname, len) == 0 || strncmp(device_table[i].alt_name, up_dname, len) == 0) {
            break;
        } else {
            i++;
        }
    }
    delete[] up_dname;
    if (i >= (int)ndevices) {
        return -1;
    } else {
	return i;
    }
}

int initgraphics(void)
{
//cout << "init graphics" << endl;
    return ((*device_table[curdevice].init)());
}

Device_entry get_device_props(int device)
{
    return (device_table[device]);
}

Device_entry get_curdevice_props()
{
    return (device_table[curdevice]);
}

const char * get_device_name(int device)
{
    return (device_table[device].name);
}

const char * get_device_alt_name(int device)
{
    return (device_table[device].alt_name);
}

void setDeviceActive(int nr,int active)
{
if (nr<0 || nr>=number_of_devices()) return;
device_table[nr].active=active;
}

int isDeviceActive(int nr)
{
    if (nr<0 || nr>=number_of_devices()) return 0;
    else
    return (device_table[nr].active);
}

void *get_curdevice_data(void)
{
    return (device_table[curdevice].data);
}

void set_curdevice_data(void *data)
{
    device_table[curdevice].data = data;
}

int set_device_props(int deviceid, Device_entry device)
{
    if (deviceid >= (int)ndevices || deviceid < 0 ||
        is_valid_page_geometry(device.pg) != TRUE) {
        return RETURN_FAILURE;
    }
    
    device_table[deviceid].type = device.type;
/*
 *     device_table[deviceid].init = device.init;
 *     device_table[deviceid].parser = device.parser;
 *     device_table[deviceid].setup = device.setup;
 */
    device_table[deviceid].devfonts = device.devfonts;
    device_table[deviceid].fontaa = device.fontaa;
    device_table[deviceid].pg = device.pg;
    device_table[deviceid].data = device.data;

    return RETURN_SUCCESS;
}

void copy_device_props(Device_entry * target_device,Device_entry * source_device,int settings_only)
{
if (settings_only==0)
{
    target_device->type=source_device->type;
    /*if (source_device->name!=NULL && target_device->name!=NULL)
    {
    delete[] target_device->name;
    target_device->name=NULL;
    }
    if (source_device->name!=NULL)
    {
    target_device->name=new char[1+strlen(source_device->name)];
    strcpy(target_device->name,source_device->name);*/
    target_device->name=source_device->name;
    /*}*/
target_device->init=source_device->init;
target_device->parser=source_device->parser;
target_device->setup=source_device->setup;
target_device->data=source_device->data;//we do not actually copy "data"
    /*if (source_device->fext!=NULL && target_device->fext!=NULL)
    {
    delete[] target_device->fext;
    target_device->fext=NULL;
    }
    if (source_device->fext!=NULL)
    {
    target_device->fext=new char[1+strlen(source_device->fext)];
    strcpy(target_device->fext,source_device->fext);*/
    target_device->fext=source_device->fext;
    /*}*/
}
target_device->devfonts=source_device->devfonts;
target_device->fontaa=source_device->fontaa;
target_device->pg=source_device->pg;
}

void set_curdevice_props(Device_entry device)
{
    set_device_props(curdevice, device);
}

int parse_device_options(int dindex, char *options)
{
    char *p, *oldp, opstring[64];
    int n;
        
    if (dindex >= (int)ndevices || dindex < 0 ||
            device_table[dindex].parser == NULL) {
        return RETURN_FAILURE;
    } else {
        oldp = options;
        while ((p = strchr(oldp, ',')) != NULL) {
	    n = MIN2((p - oldp), 64 - 1);
            strncpy(opstring, oldp, n);
            opstring[n] = '\0';
            if (device_table[dindex].parser(opstring) != RETURN_SUCCESS) {
                return RETURN_FAILURE;
            }
            oldp = p + 1;
        }
        return device_table[dindex].parser(oldp);
    }
}

int number_of_devices(void)
{
    return (ndevices);
}

void get_page_viewport(double *vx, double *vy)
{
    *vx = device_table[curdevice].pg.width/device_table[curdevice].pg.dpi;
    *vy = device_table[curdevice].pg.height/device_table[curdevice].pg.dpi;
    if (*vx < *vy) {
        *vy /= *vx;
        *vx = 1.0;
    } else {
        *vx /= *vy;
        *vy = 1.0;
    }
}

int terminal_device(void)
{
    if (device_table[curdevice].type == DEVICE_TERM) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void return_Page_Dimensions_cm(int format,int orientation,double * x,double * y)
{
switch (format)
{
case PAGE_FORMAT_CUSTOM:
return;//do nothing
break;
case PAGE_FORMAT_USLETTER:
*x=8.5*CM_PER_INCH;
*y=11.0*CM_PER_INCH;
break;
case PAGE_FORMAT_USLEGAL:
*x=8.5*CM_PER_INCH;
*y=14.0*CM_PER_INCH;
break;
case PAGE_FORMAT_USTABLOID:
*x=11.0*CM_PER_INCH;
*y=17.0*CM_PER_INCH;
break;
case PAGE_FORMAT_A4:
*x=84.1/4.0;
*y=118.9/4.0;
break;
case PAGE_FORMAT_A3:
*x=118.9/4.0;
*y=84.1/2.0;
break;
case PAGE_FORMAT_A2:
*x=84.1/2.0;
*y=118.9/2.0;
break;
case PAGE_FORMAT_A1:
*x=118.9/2.0;
*y=84.1;
break;
case PAGE_FORMAT_A0:
*x=84.1;
*y=118.9;
break;
case PAGE_FORMAT_B4:
*x=25.0;
*y=35.3;
break;
case PAGE_FORMAT_B5:
*x=17.6;
*y=25.0;
break;
case PAGE_FORMAT_B6:
*x=12.5;
*y=17.6;
break;
}
    if (orientation==PAGE_ORIENT_LANDSCAPE)
    {
    double tmp;
    tmp=*x;
    *x=*y;
    *y=tmp;
    }
}

void return_Page_Dimensions_in(int format, int orientation, double *x, double *y)
{
return_Page_Dimensions_cm(format,orientation,x,y);
*x/=CM_PER_INCH;
*y/=CM_PER_INCH;
}

void return_Page_Dimensions_pix(int format,int orientation,double dpi,int * x,int * y)
{
double a,b;
a=b=0.0;
return_Page_Dimensions_in(format,orientation,&a,&b);
*x=(int)(a*dpi);
*y=(int)(b*dpi);
}

PageFormat get_page_format(int device)
{
    Page_geometry pg;
    int width_pp, height_pp;
    
    pg = device_table[device].pg;
    width_pp  = (int) rint_v2((double) 72*pg.width/pg.dpi);
    height_pp = (int) rint_v2((double) 72*pg.height/pg.dpi);
    
    if ((width_pp == 612 && height_pp == 792) ||
        (height_pp == 612 && width_pp == 792)) {
        return PAGE_FORMAT_USLETTER;
    } else if ((width_pp == 595 && height_pp == 842) ||
               (height_pp == 595 && width_pp == 842)) {
        return PAGE_FORMAT_A4;
    } else {
        return PAGE_FORMAT_CUSTOM;
    }
}

/*
 * flag to indicate destination of hardcopy output,
 * ptofile = 0 means print to printer, otherwise print to file
 */

static int ptofile = FALSE;
                           
void set_ptofile(int flag)
{
    ptofile = flag;
}

int get_ptofile(void)
{
    return ptofile;
}
