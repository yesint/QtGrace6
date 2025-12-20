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
 *  declarations for devices
 *
 */
#ifndef __DEVICE_H_
#define __DEVICE_H_

#include "t1fonts.h"

/* default dimensions of the canvas */
#define DEFAULT_PAGE_WIDTH  733
#define DEFAULT_PAGE_HEIGHT 538

#define MM_PER_INCH	25.4
#define CM_PER_INCH	(MM_PER_INCH/10)

/* hardcopy or terminal device */
/* device output can be redirected to file/printer(both) */
#define DEVICE_TERM	0
#define DEVICE_FILE	1
#define DEVICE_PRINT	2

/* Page orientation */
#define PAGE_ORIENT_LANDSCAPE  0
#define PAGE_ORIENT_PORTRAIT   1

#ifdef __cplusplus
extern "C" {
#endif

/* Standard formats */
/*typedef enum {
    PAGE_FORMAT_CUSTOM, 
    PAGE_FORMAT_USLETTER,
    PAGE_FORMAT_A4     
} PageFormat;*/
typedef enum {
    PAGE_FORMAT_CUSTOM,
    PAGE_FORMAT_USLETTER,
    PAGE_FORMAT_USLEGAL,
    PAGE_FORMAT_USTABLOID,
    PAGE_FORMAT_A4,
    PAGE_FORMAT_A3,
    PAGE_FORMAT_A2,
    PAGE_FORMAT_A1,
    PAGE_FORMAT_A0,
    PAGE_FORMAT_B4,
    PAGE_FORMAT_B5,
    PAGE_FORMAT_B6
} PageFormat;

typedef struct {
    unsigned long width;
    unsigned long height;
    float dpi;
} Page_geometry;

typedef struct {//negative values mean: do not change existing settings
    int active;
    char name[128];
    signed char dev_nr;//the output-format
    signed char orientation;//0=Landscape, 1=Portrait, -1=no change
    double width;
    double height;
    signed char unit;//0=pix, 1=inch, 2=cm, -1=no change
    float dpi;
    signed char Antialiasing;//0=No Antialiasing,1=Font Antialiasing Only,2=Graph Antialiaing Only, 3=Graph and Font Antialiasing, -1=no change
    signed char ScaleLineWidth;//0=no Scaling, 1=Scaling, -1=no change
} UserDeviceGeometry;

typedef struct {
    int type;
    const char *name;       /* name of device */
    int (*init)(void);	    /* function to initialize device */
    int (*parser)(const char *);  /* function to parse device-specific commands */
    void (*setup)(void);    /* function (GUI interface) to setup device */
    const char *fext;             /* filename extension */
    int devfonts;           /* device has its own fonts */
    int fontaa;             /* font antialiasing */
    Page_geometry pg;       /* device defaults */
    void *data;             /* device private data */
    int active;             /* device selectable for output */
    const char *alt_name;   /* alternative name of device */
} Device_entry;

/* device exit */
extern void (*devleavegraphics) (void);
/* device pixel routine */
extern void (*devdrawpixel) (VPoint vp);  
/* device polyline routine */
extern void (*devdrawpolyline) (VPoint *vps, int n, int mode);  
/* device polygon routine */
extern void (*devfillpolygon) (VPoint *vps, int nc);   
/* device arc routine */
extern void (*devdrawarc) (VPoint vp1, VPoint vp2, int a1, int a2);	
/* device fill arc routine */
extern void (*devfillarc) (VPoint vp1, VPoint vp2, int a1, int a2, int mode);	
/* device pixmap drawing */
extern void (*devputpixmap) (VPoint vp, int width, int height, char *databits,int pixmap_bpp, int bitmap_pad, int pixmap_type);
/* device text typesetting */
extern void (*devputtext) (VPoint vp, char *s, int len, int font,TextMatrix *tm, int underline, int overline, int kerning);
/* update color map */
extern void (*devupdatecmap) (void);
extern double (*xy_xconv) (double wx);
extern double (*xy_yconv) (double wy);

int register_device(Device_entry device);
int select_device(int dindex);
int initgraphics (void);

Device_entry get_device_props(int device);
Device_entry get_curdevice_props(void);

void setDeviceActive(int nr, int active);
int isDeviceActive(int nr);

const char * get_device_name(int device);
const char * get_device_alt_name(int device);

void *get_curdevice_data(void);
void set_curdevice_data(void *data);

int set_device_props(int device, Device_entry dev);
void set_curdevice_props(Device_entry dev);
void copy_device_props(Device_entry * target_device,Device_entry * source_device,int settings_only);//intended just for temporary device-entries (if settings_only is set, only the dimensions are coppied, no type, no data, no function-pointers)

int is_valid_page_geometry(Page_geometry pg);
int set_page_geometry(Page_geometry pg);
Page_geometry get_page_geometry(void);

int set_page_dimensions(int wpp, int hpp, int rescale);
int get_device_page_dimensions(int dindex, int *wpp, int *hpp);

int get_device_by_name(const char *dname);

int parse_device_options(int dindex, char *options);

int set_printer(int device);
int set_printer_by_name(const char *dname);
void set_ptofile(int flag);
int get_ptofile(void);

int number_of_devices(void);

void get_page_viewport(double *vx, double *vy);

int terminal_device(void);

PageFormat get_page_format(int device);
void return_Page_Dimensions_cm(int format, int orientation, double *x, double *y);
void return_Page_Dimensions_in(int format,int orientation,double * x,double * y);
void return_Page_Dimensions_pix(int format,int orientation,double dpi,int * x,int * y);

#ifdef __cplusplus
}
#endif

/* some useful macros */
#define page_dpi       ((get_page_geometry()).dpi)

#define page_width     ((get_page_geometry()).width)
#define page_height    ((get_page_geometry()).height)

#define page_width_in  ((double) page_width/page_dpi)
#define page_height_in ((double) page_height/page_dpi)

#define page_width_mm  (MM_PER_INCH*page_width_in)
#define page_height_mm (MM_PER_INCH*page_height_in)

#define page_width_cm  (CM_PER_INCH*page_width_in)
#define page_height_cm (CM_PER_INCH*page_height_in)

#define page_width_pp  (72*page_width_in)
#define page_height_pp (72*page_height_in)

#endif /* __DEVICE_H_ */
