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
 * File I/O 
 */

#ifndef __FILES_H_
#define __FILES_H_

#include <stdio.h>

/* data load types */
#define LOAD_SINGLE 0
#define LOAD_NXY    1
#define LOAD_BLOCK  2

#ifdef __cplusplus
extern "C" {
#endif

int add_io_filter( int type, int method, char *id, char *comm );
int add_input_filter( int method, char *id, char *comm );
int add_output_filter( int method, char *id, char *comm );
void clear_io_filters( int f );
FILE *filter_read( const char *fn );
FILE *filter_write( const char *fn );

const char *grace_path(const char *fn);
const char *grace_exe_path(const char *fn);

int look_for_file(char * fn);

FILE *grace_openw(char *fn);
FILE *grace_openr(const char *fn, int src);
void grace_close(FILE *fp);

int getparms(char *plfile);
int getdata(int gno, char *fn, int src, int type);
int readDataFromClient(char* dataIn, int load_type,const char *label);
int update_set_from_file(int gno, int setno, char *fn, int src);

int readblockdata(int gno, char *fn, FILE * fp);

int load_project_file(char *fn, int as_n_template);

int new_project(char *n_template);
int load_project(char *fn);
int save_project(char *fn);

int write_set(int gno, int setno, FILE *cp, char *format, int rawdata,int write_to_cp);
void outputset(int gno, int setno, char *fname, char *dformat);

int readnetcdf(int gno,
	       int setno,
	       char *netcdfname,
	       char *xvar,
	       char *yvar,
	       int nstart,
	       int nstop,
	       int nstride);
int write_netcdf(char *fname);

char *grace_fgets(char *s, int size, FILE *stream);
int is_Gould_file(char * file);
int is_agr_file(char * file);
int is_csv_file(char * file);
void write_data_as_wav(char * filename,int nr_of_sets,int * gnos,int * snos);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <QtCore>
#include <QList>
#include <QString>
#include "defines.h"

struct agr_file_info
{
char filename[GR_MAXPATHLEN];
int nr_of_graphs,nr_of_sets,target_gno,headerlines;
QList<int> g_ids,s_ids;//graph- and set-ids
QStringList set_comments,set_legends,set_types;
QString project_description;
QList<int> import,color,lines,pattern,linet;
QList<double> linew;
QString encoding;
};

void init_agr_file_info(struct agr_file_info & afi);
void read_header_from_agr(char * filename, struct agr_file_info & afi);
void read_datasets_from_agr(struct agr_file_info afi);

#endif

#endif /* __FILES_H_ */
