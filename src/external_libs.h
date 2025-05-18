/***************************************************************************
 *   Copyright (C) 2008-2022 by Andreas Winter                             *
 *   andreas.f.winter@web.de                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef EXTERNAL_LIBS_H
#define EXTERNAL_LIBS_H
#include "fftw3.h"
#include <QString>
#ifdef WINDOWS_SYSTEM
#include "include\hpdf.h"
#else
#include "include/hpdf.h"
#endif

/* FFTW3 */
typedef void (*Prototype_fftw_free)(void *p);
typedef int (*Prototype_fftw_import_wisdom_from_file)(FILE *input_file);
typedef void (*Prototype_fftw_export_wisdom_to_file)(FILE *output_file);
typedef char* (*Prototype_fftw_export_wisdom_to_string)(void);
typedef void (*Prototype_fftw_destroy_plan)(fftw_plan p);
typedef void* (*Prototype_fftw_malloc)(size_t n);
typedef void (*Prototype_fftw_execute)(const fftw_plan p);
typedef fftw_plan (*Prototype_fftw_plan_dft_1d)(int n, fftw_complex *in, fftw_complex *out, int sign, unsigned flags);

bool init_fftw3_library(void);
bool is_fftw3_lib_usable(QString lib);

/* libHARU */
typedef HPDF_STATUS (*Prototype_HPDF_Page_SetHeight)(HPDF_Page page, HPDF_REAL value);
typedef HPDF_STATUS (*Prototype_HPDF_Page_SetWidth)(HPDF_Page page, HPDF_REAL value);
typedef HPDF_Page (*Prototype_HPDF_AddPage)(HPDF_Doc pdf);
typedef void (*Prototype_HPDF_Free)(HPDF_Doc pdf);
typedef void (*Prototype_HPDF_ResetError)(HPDF_Doc pdf);
typedef HPDF_STATUS (*Prototype_HPDF_SaveToFile)(HPDF_Doc pdf, const char *file_name);
typedef HPDF_Doc (*Prototype_HPDF_New)(HPDF_Error_Handler user_error_fn, void *user_data);
typedef HPDF_REAL (*Prototype_HPDF_Page_GetWidth)(HPDF_Page page);
typedef HPDF_REAL (*Prototype_HPDF_Page_GetHeight)(HPDF_Page page);
typedef HPDF_Font (*Prototype_HPDF_GetFont)(HPDF_Doc pdf, const char *font_name, const char *encoding_name);
typedef HPDF_STATUS (*Prototype_HPDF_Page_SetFontAndSize)(HPDF_Page page, HPDF_Font font, HPDF_REAL size);
typedef HPDF_STATUS (*Prototype_HPDF_Page_SetLineWidth)(HPDF_Page page, HPDF_REAL line_width);
typedef HPDF_STATUS (*Prototype_HPDF_Page_SetLineCap)(HPDF_Page page, HPDF_LineCap line_cap);
typedef HPDF_STATUS (*Prototype_HPDF_Page_SetLineJoin)(HPDF_Page page, HPDF_LineJoin line_join);
typedef HPDF_STATUS (*Prototype_HPDF_Page_SetMiterLimit)(HPDF_Page page, HPDF_REAL miter_limit);
typedef HPDF_STATUS (*Prototype_HPDF_Page_SetDash)(HPDF_Page page, const HPDF_UINT16 *dash_ptn, HPDF_UINT num_param, HPDF_UINT phase);
typedef HPDF_REAL (*Prototype_HPDF_Page_GetLineWidth)(HPDF_Page page);
typedef HPDF_STATUS (*Prototype_HPDF_Page_MoveTo)(HPDF_Page page, HPDF_REAL x, HPDF_REAL y);
typedef HPDF_STATUS (*Prototype_HPDF_Page_LineTo)(HPDF_Page page, HPDF_REAL x, HPDF_REAL y);
typedef HPDF_STATUS (*Prototype_HPDF_Page_Stroke)(HPDF_Page page);
typedef HPDF_STATUS (*Prototype_HPDF_Page_Fill)(HPDF_Page page);
typedef HPDF_STATUS (*Prototype_HPDF_Page_BeginText)(HPDF_Page page);
typedef HPDF_STATUS (*Prototype_HPDF_Page_EndText)(HPDF_Page page);
typedef HPDF_STATUS (*Prototype_HPDF_Page_MoveTextPos)(HPDF_Page page, HPDF_REAL x, HPDF_REAL y);
typedef HPDF_STATUS (*Prototype_HPDF_Page_ShowText)(HPDF_Page page, const char *text);
typedef HPDF_STATUS (*Prototype_HPDF_Page_ClosePathStroke)(HPDF_Page page);
typedef HPDF_STATUS (*Prototype_HPDF_Page_SetRGBStroke)(HPDF_Page page, HPDF_REAL r, HPDF_REAL g, HPDF_REAL b);
typedef HPDF_STATUS (*Prototype_HPDF_Page_SetRGBFill)(HPDF_Page page, HPDF_REAL r, HPDF_REAL g, HPDF_REAL b);
typedef HPDF_STATUS (*Prototype_HPDF_Page_Ellipse)(HPDF_Page page, HPDF_REAL x, HPDF_REAL y, HPDF_REAL xray, HPDF_REAL yray);
typedef HPDF_STATUS (*Prototype_HPDF_Page_Arc)(HPDF_Page page, HPDF_REAL x, HPDF_REAL y, HPDF_REAL ray, HPDF_REAL ang1, HPDF_REAL ang2);
typedef HPDF_STATUS (*Prototype_HPDF_Page_Rectangle)(HPDF_Page page, HPDF_REAL x, HPDF_REAL y, HPDF_REAL width, HPDF_REAL height);
typedef HPDF_STATUS (*Prototype_HPDF_Page_GSave)(HPDF_Page page);
typedef HPDF_STATUS (*Prototype_HPDF_Page_GRestore)(HPDF_Page page);
typedef HPDF_STATUS (*Prototype_HPDF_SetCompressionMode)(HPDF_Doc pdf, HPDF_UINT mode);
typedef HPDF_STATUS (*Prototype_HPDF_Page_SetTextMatrix)(HPDF_Page page, HPDF_REAL a, HPDF_REAL b, HPDF_REAL c, HPDF_REAL d, HPDF_REAL x, HPDF_REAL y);
typedef HPDF_STATUS (*Prototype_HPDF_Page_Concat)(HPDF_Page page, HPDF_REAL a, HPDF_REAL b, HPDF_REAL c, HPDF_REAL d, HPDF_REAL x, HPDF_REAL y);
typedef HPDF_STATUS (*Prototype_HPDF_Page_Eofill)(HPDF_Page page);
typedef HPDF_REAL (*Prototype_HPDF_Page_TextWidth)(HPDF_Page page,const char *text);
typedef HPDF_STATUS (*Prototype_HPDF_SetInfoAttr)(HPDF_Doc pdf, HPDF_InfoType type,const char *value);
//typedef HPDF_REAL (*Prototype_HPDF_SetInfoAttr)(HPDF_Doc pdf, HPDF_InfoType type,const char *value);
typedef const char* (*Prototype_HPDF_LoadType1FontFromFile)(HPDF_Doc pdf, const char *afm_file_name, const char *data_file_name);
typedef HPDF_FontDef (*Prototype_HPDF_GetTTFontDefFromFile)(HPDF_Doc pdf, const char *file_name, HPDF_BOOL embedding);
typedef const char* (*Prototype_HPDF_LoadTTFontFromFile)(HPDF_Doc pdf, const char *file_name, HPDF_BOOL embedding);
typedef const char* (*Prototype_HPDF_LoadTTFontFromFile2)(HPDF_Doc pdf, const char *file_name, HPDF_UINT index, HPDF_BOOL embedding);
typedef HPDF_STATUS (*Prototype_HPDF_SetCurrentEncoder)(HPDF_Doc pdf, const char  *encoding_name);
//typedef HPDF_Outline (*Prototype_HPDF_SetCurrentEncoder)(HPDF_Doc pdf, const char  *encoding_name);

typedef HPDF_Image (*Prototype_HPDF_LoadPngImageFromFile)(HPDF_Doc pdf, const char *filename);
typedef HPDF_Image (*Prototype_HPDF_LoadJpegImageFromFile)(HPDF_Doc pdf, const char *filename);
typedef HPDF_STATUS (*Prototype_HPDF_Image_SetMaskImage)(HPDF_Image image, HPDF_Image mask_image);

bool init_libHaru_library(void);
bool is_Haru_lib_usable(QString lib);

#endif // EXTERNAL_LIBS_H
