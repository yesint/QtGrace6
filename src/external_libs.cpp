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

#include <QtCore>
#include <QLibrary>
#include <iostream>
#include "globals.h"
#include "external_libs.h"

using namespace std;

extern QFile path_to_fftw3_lib,path_to_libharu;

void check_external_lib_usability(void)//test whether the external libs are usable (exist and have the functions needed)
{
have_fftw3=init_fftw3_library();
have_libHaru=init_libHaru_library();
}

/* FFTW3 */

Prototype_fftw_export_wisdom_to_string fftw_dll_export_wisdom_to_string=NULL;
Prototype_fftw_export_wisdom_to_file fftw_dll_export_wisdom_to_file=NULL;
Prototype_fftw_import_wisdom_from_file fftw_dll_import_wisdom_from_file=NULL;
Prototype_fftw_destroy_plan fftw_dll_destroy_plan=NULL;
Prototype_fftw_free fftw_dll_free=NULL;
Prototype_fftw_malloc fftw_dll_malloc=NULL;
Prototype_fftw_execute fftw_dll_execute=NULL;
Prototype_fftw_plan_dft_1d fftw_dll_plan_dft_1d=NULL;

bool init_fftw3_library(void)
{
bool ret=true;

#ifdef USE_STATIC_EXT_LIB_FFTW3
//cout << "using static library fftw3" << endl;
fftw_dll_export_wisdom_to_string=fftw_export_wisdom_to_string;
fftw_dll_export_wisdom_to_file=fftw_export_wisdom_to_file;
fftw_dll_import_wisdom_from_file=fftw_import_wisdom_from_file;
fftw_dll_destroy_plan=fftw_destroy_plan;
fftw_dll_free=fftw_free;
fftw_dll_malloc=fftw_malloc;
fftw_dll_execute=fftw_execute;
fftw_dll_plan_dft_1d=fftw_plan_dft_1d;
return ret;
#endif

QLibrary lib_fftw(path_to_fftw3_lib.fileName());
(void)lib_fftw.load();
/// cout << "loading dll-library: " << lib_fftw.fileName().toLocal8Bit().constData() << " isLoaded=" << lib_fftw.isLoaded() << endl;

ret=ret & lib_fftw.isLoaded();

fftw_dll_free = (Prototype_fftw_free) lib_fftw.resolve("fftw_free");
if (fftw_dll_free==NULL)
{
//cout << "Could NOT resolve fftw_free !" << endl;
ret=false;
}
/*else
{
cout << "fftw_free RESOLVED !" << endl;
}*/

fftw_dll_export_wisdom_to_string = (Prototype_fftw_export_wisdom_to_string) lib_fftw.resolve("fftw_export_wisdom_to_string");
if (fftw_dll_export_wisdom_to_string==NULL)
{
//cout << "Could NOT resolve fftw_export_wisdom_to_string !" << endl;
ret=false;
}
/*else
{
cout << "fftw_export_wisdom_to_string RESOLVED !" << endl;
}*/

fftw_dll_export_wisdom_to_file = (Prototype_fftw_export_wisdom_to_file) lib_fftw.resolve("fftw_export_wisdom_to_file");
if (fftw_dll_export_wisdom_to_file==NULL)
{
//cout << "Could NOT resolve fftw_export_wisdom_to_file !" << endl;
ret=false;
}
/*else
{
cout << "fftw_export_wisdom_to_file RESOLVED !" << endl;
}*/

fftw_dll_import_wisdom_from_file = (Prototype_fftw_import_wisdom_from_file) lib_fftw.resolve("fftw_import_wisdom_from_file");
if (fftw_dll_import_wisdom_from_file==NULL)
{
//cout << "Could NOT resolve fftw_import_wisdom_from_file !" << endl;
ret=false;
}
/*else
{
cout << "fftw_import_wisdom_from_file RESOLVED !" << endl;
}*/

fftw_dll_destroy_plan = (Prototype_fftw_destroy_plan) lib_fftw.resolve("fftw_destroy_plan");
if (fftw_dll_destroy_plan==NULL)
{
//cout << "Could NOT resolve fftw_destroy_plan !" << endl;
ret=false;
}
/*else
{
cout << "fftw_destroy_plan RESOLVED !" << endl;
}*/

fftw_dll_malloc = (Prototype_fftw_malloc) lib_fftw.resolve("fftw_malloc");
if (fftw_dll_malloc==NULL)
{
//cout << "Could NOT resolve fftw_malloc !" << endl;
ret=false;
}
/*else
{
cout << "fftw_malloc RESOLVED !" << endl;
}*/

fftw_dll_execute = (Prototype_fftw_execute) lib_fftw.resolve("fftw_execute");
if (fftw_dll_execute==NULL)
{
//cout << "Could NOT resolve fftw_execute !" << endl;
ret=false;
}
/*else
{
cout << "fftw_execute RESOLVED !" << endl;
}*/

fftw_dll_plan_dft_1d = (Prototype_fftw_plan_dft_1d) lib_fftw.resolve("fftw_plan_dft_1d");
if (fftw_dll_plan_dft_1d==NULL)
{
//cout << "Could NOT resolve fftw_plan_dft_1d !" << endl;
ret=false;
}
/*else
{
cout << "fftw_plan_dft_1d RESOLVED !" << endl;
}*/

/// if (ret==true) cout << "Everything RESOLVED in libFFTW3" << endl;

return ret;
}

bool is_fftw3_lib_usable(QString lib)
{
QString old_filename=path_to_fftw3_lib.fileName();
path_to_fftw3_lib.setFileName(lib);
bool ret=init_fftw3_library();
path_to_fftw3_lib.setFileName(old_filename);
(void) init_fftw3_library();
return ret;
}

/* libHARU */

Prototype_HPDF_Page_SetHeight libHaru_HPDF_Page_SetHeight=NULL;
Prototype_HPDF_Page_SetWidth libHaru_HPDF_Page_SetWidth=NULL;
Prototype_HPDF_AddPage libHaru_HPDF_AddPage=NULL;
Prototype_HPDF_Free libHaru_HPDF_Free=NULL;
Prototype_HPDF_ResetError libHaru_HPDF_ResetError=NULL;
Prototype_HPDF_SaveToFile libHaru_HPDF_SaveToFile=NULL;
Prototype_HPDF_New libHaru_HPDF_New=NULL;
Prototype_HPDF_Page_GetWidth libHaru_HPDF_Page_GetWidth=NULL;
Prototype_HPDF_Page_GetHeight libHaru_HPDF_Page_GetHeight=NULL;
Prototype_HPDF_GetFont libHaru_HPDF_GetFont=NULL;
Prototype_HPDF_Page_SetFontAndSize libHaru_HPDF_Page_SetFontAndSize=NULL;
Prototype_HPDF_Page_SetLineWidth libHaru_HPDF_Page_SetLineWidth=NULL;
Prototype_HPDF_Page_SetLineCap libHaru_HPDF_Page_SetLineCap=NULL;
Prototype_HPDF_Page_SetLineJoin libHaru_HPDF_Page_SetLineJoin=NULL;
Prototype_HPDF_Page_SetMiterLimit libHaru_HPDF_Page_SetMiterLimit=NULL;
Prototype_HPDF_Page_SetDash libHaru_HPDF_Page_SetDash=NULL;
Prototype_HPDF_Page_GetLineWidth libHaru_HPDF_Page_GetLineWidth=NULL;
Prototype_HPDF_Page_MoveTo libHaru_HPDF_Page_MoveTo=NULL;
Prototype_HPDF_Page_LineTo libHaru_HPDF_Page_LineTo=NULL;
Prototype_HPDF_Page_Stroke libHaru_HPDF_Page_Stroke=NULL;
Prototype_HPDF_Page_Fill libHaru_HPDF_Page_Fill=NULL;
Prototype_HPDF_Page_BeginText libHaru_HPDF_Page_BeginText=NULL;
Prototype_HPDF_Page_EndText libHaru_HPDF_Page_EndText=NULL;
Prototype_HPDF_Page_MoveTextPos libHaru_HPDF_Page_MoveTextPos=NULL;
Prototype_HPDF_Page_ShowText libHaru_HPDF_Page_ShowText=NULL;
Prototype_HPDF_Page_ClosePathStroke libHaru_HPDF_Page_ClosePathStroke=NULL;
Prototype_HPDF_Page_SetRGBStroke libHaru_HPDF_Page_SetRGBStroke=NULL;
Prototype_HPDF_Page_SetRGBFill libHaru_HPDF_Page_SetRGBFill=NULL;
Prototype_HPDF_Page_Ellipse libHaru_HPDF_Page_Ellipse=NULL;
Prototype_HPDF_Page_Arc libHaru_HPDF_Page_Arc=NULL;
Prototype_HPDF_Page_Rectangle libHaru_HPDF_Page_Rectangle=NULL;
Prototype_HPDF_Page_GSave libHaru_HPDF_Page_GSave=NULL;
Prototype_HPDF_Page_GRestore libHaru_HPDF_Page_GRestore=NULL;
Prototype_HPDF_SetCompressionMode libHaru_HPDF_SetCompressionMode=NULL;
Prototype_HPDF_Page_SetTextMatrix libHaru_HPDF_Page_SetTextMatrix=NULL;
Prototype_HPDF_Page_Concat libHaru_HPDF_Page_Concat=NULL;
Prototype_HPDF_Page_Eofill libHaru_HPDF_Page_Eofill=NULL;
Prototype_HPDF_Page_TextWidth libHaru_HPDF_Page_TextWidth=NULL;
Prototype_HPDF_SetInfoAttr libHaru_HPDF_SetInfoAttr=NULL;
Prototype_HPDF_LoadType1FontFromFile libHaru_HPDF_LoadType1FontFromFile=NULL;
Prototype_HPDF_GetTTFontDefFromFile libHaru_HPDF_GetTTFontDefFromFile=NULL;
Prototype_HPDF_LoadTTFontFromFile libHaru_HPDF_LoadTTFontFromFile=NULL;
Prototype_HPDF_LoadTTFontFromFile2 libHaru_HPDF_LoadTTFontFromFile2=NULL;
Prototype_HPDF_SetCurrentEncoder libHaru_HPDF_SetCurrentEncoder=NULL;
Prototype_HPDF_LoadPngImageFromFile libHaru_HPDF_LoadPngImageFromFile=NULL;
Prototype_HPDF_LoadJpegImageFromFile libHaru_HPDF_LoadJpegImageFromFile=NULL;
Prototype_HPDF_Image_SetMaskImage libHaru_HPDF_Image_SetMaskImage=NULL;

bool init_libHaru_library(void)
{
bool ret=true;

#ifdef USE_STATIC_EXT_LIB_HARU
//cout << "using static library libHaru" << endl;
libHaru_HPDF_Page_SetHeight=HPDF_Page_SetHeight;
libHaru_HPDF_Page_SetWidth=HPDF_Page_SetWidth;
libHaru_HPDF_AddPage=HPDF_AddPage;
libHaru_HPDF_Free=HPDF_Free;
libHaru_HPDF_ResetError=HPDF_ResetError;
libHaru_HPDF_SaveToFile=HPDF_SaveToFile;
libHaru_HPDF_New=HPDF_New;
libHaru_HPDF_Page_GetWidth=HPDF_Page_GetWidth;
libHaru_HPDF_Page_GetHeight=HPDF_Page_GetHeight;
libHaru_HPDF_GetFont=HPDF_GetFont;
libHaru_HPDF_Page_SetFontAndSize=HPDF_Page_SetFontAndSize;
libHaru_HPDF_Page_SetLineWidth=HPDF_Page_SetLineWidth;
libHaru_HPDF_Page_SetLineCap=HPDF_Page_SetLineCap;
libHaru_HPDF_Page_SetLineJoin=HPDF_Page_SetLineJoin;
libHaru_HPDF_Page_SetMiterLimit=HPDF_Page_SetMiterLimit;
libHaru_HPDF_Page_SetDash=HPDF_Page_SetDash;
libHaru_HPDF_Page_GetLineWidth=HPDF_Page_GetLineWidth;
libHaru_HPDF_Page_MoveTo=HPDF_Page_MoveTo;
libHaru_HPDF_Page_LineTo=HPDF_Page_LineTo;
libHaru_HPDF_Page_Stroke=HPDF_Page_Stroke;
libHaru_HPDF_Page_Fill=HPDF_Page_Fill;
libHaru_HPDF_Page_BeginText=HPDF_Page_BeginText;
libHaru_HPDF_Page_EndText=HPDF_Page_EndText;
libHaru_HPDF_Page_MoveTextPos=HPDF_Page_MoveTextPos;
libHaru_HPDF_Page_ShowText=HPDF_Page_ShowText;
libHaru_HPDF_Page_ClosePathStroke=HPDF_Page_ClosePathStroke;
libHaru_HPDF_Page_SetRGBStroke=HPDF_Page_SetRGBStroke;
libHaru_HPDF_Page_SetRGBFill=HPDF_Page_SetRGBFill;
libHaru_HPDF_Page_Ellipse=HPDF_Page_Ellipse;
libHaru_HPDF_Page_Arc=HPDF_Page_Arc;
libHaru_HPDF_Page_Rectangle=HPDF_Page_Rectangle;
libHaru_HPDF_Page_GSave=HPDF_Page_GSave;
libHaru_HPDF_Page_GRestore=HPDF_Page_GRestore;
libHaru_HPDF_SetCompressionMode=HPDF_SetCompressionMode;
libHaru_HPDF_Page_SetTextMatrix=HPDF_Page_SetTextMatrix;
libHaru_HPDF_Page_Concat=HPDF_Page_Concat;
libHaru_HPDF_Page_Eofill=HPDF_Page_Eofill;
libHaru_HPDF_Page_TextWidth=HPDF_Page_TextWidth;
libHaru_HPDF_SetInfoAttr=HPDF_SetInfoAttr;
libHaru_HPDF_LoadType1FontFromFile=HPDF_LoadType1FontFromFile;
libHaru_HPDF_GetTTFontDefFromFile=HPDF_GetTTFontDefFromFile;
libHaru_HPDF_LoadTTFontFromFile=HPDF_LoadTTFontFromFile;
libHaru_HPDF_LoadTTFontFromFile2=HPDF_LoadTTFontFromFile2;
libHaru_HPDF_SetCurrentEncoder=HPDF_SetCurrentEncoder;

libHaru_HPDF_LoadPngImageFromFile=HPDF_LoadPngImageFromFile;
libHaru_HPDF_LoadJpegImageFromFile=HPDF_LoadJpegImageFromFile;
libHaru_HPDF_Image_SetMaskImage=HPDF_Image_SetMaskImage;
return ret;
#endif

QLibrary libHaru(path_to_libharu.fileName());
if (libHaru.isLoaded())
{
;/// cout << "unloading Library: " << libHaru.unload() << endl;
}
else
{
(void)libHaru.load();
}
/// cout << "loading dll-library: " << libHaru.fileName().toLocal8Bit().constData() << " isLoaded=" << libHaru.isLoaded() << endl;

ret=ret & libHaru.isLoaded();

libHaru_HPDF_Page_SetHeight = (Prototype_HPDF_Page_SetHeight) libHaru.resolve("HPDF_Page_SetHeight");
if (libHaru_HPDF_Page_SetHeight==NULL)
{
//cout << "Could NOT resolve HPDF_Page_SetHeight !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_SetHeight RESOLVED !" << endl;
}*/

libHaru_HPDF_Page_SetWidth = (Prototype_HPDF_Page_SetWidth) libHaru.resolve("HPDF_Page_SetWidth");
if (libHaru_HPDF_Page_SetWidth==NULL)
{
//cout << "Could NOT resolve HPDF_Page_SetWidth !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_SetWidth RESOLVED !" << endl;
}*/
libHaru_HPDF_AddPage = (Prototype_HPDF_AddPage) libHaru.resolve("HPDF_AddPage");
if (libHaru_HPDF_AddPage==NULL)
{
//cout << "Could NOT resolve HPDF_AddPage !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_AddPage RESOLVED !" << endl;
}*/
libHaru_HPDF_Free = (Prototype_HPDF_Free) libHaru.resolve("HPDF_Free");
if (libHaru_HPDF_Free==NULL)
{
//cout << "Could NOT resolve HPDF_Free !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Free RESOLVED !" << endl;
}*/
libHaru_HPDF_ResetError = (Prototype_HPDF_ResetError) libHaru.resolve("HPDF_ResetError");
if (libHaru_HPDF_ResetError==NULL)
{
//cout << "Could NOT resolve HPDF_ResetError !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_ResetError RESOLVED !" << endl;
}*/
libHaru_HPDF_SaveToFile = (Prototype_HPDF_SaveToFile) libHaru.resolve("HPDF_SaveToFile");
if (libHaru_HPDF_SaveToFile==NULL)
{
//cout << "Could NOT resolve HPDF_SaveToFile !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_SaveToFile RESOLVED !" << endl;
}*/
libHaru_HPDF_New = (Prototype_HPDF_New) libHaru.resolve("HPDF_New");
if (libHaru_HPDF_New==NULL)
{
//cout << "Could NOT resolve HPDF_New !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_New RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_GetWidth = (Prototype_HPDF_Page_GetWidth) libHaru.resolve("HPDF_Page_GetWidth");
if (libHaru_HPDF_Page_GetWidth==NULL)
{
//cout << "Could NOT resolve HPDF_Page_GetWidth !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_GetWidth RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_GetHeight = (Prototype_HPDF_Page_GetHeight) libHaru.resolve("HPDF_Page_GetHeight");
if (libHaru_HPDF_Page_GetHeight==NULL)
{
//cout << "Could NOT resolve HPDF_Page_GetHeight !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_GetHeight RESOLVED !" << endl;
}*/
libHaru_HPDF_GetFont = (Prototype_HPDF_GetFont) libHaru.resolve("HPDF_GetFont");
if (libHaru_HPDF_GetFont==NULL)
{
//cout << "Could NOT resolve HPDF_GetFont !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_GetFont RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_SetFontAndSize = (Prototype_HPDF_Page_SetFontAndSize) libHaru.resolve("HPDF_Page_SetFontAndSize");
if (libHaru_HPDF_Page_SetFontAndSize==NULL)
{
//cout << "Could NOT resolve HPDF_Page_SetFontAndSize !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_SetFontAndSize RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_SetLineWidth = (Prototype_HPDF_Page_SetLineWidth) libHaru.resolve("HPDF_Page_SetLineWidth");
if (libHaru_HPDF_Page_SetLineWidth==NULL)
{
//cout << "Could NOT resolve HPDF_Page_SetLineWidth !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_SetLineWidth RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_SetLineCap = (Prototype_HPDF_Page_SetLineCap) libHaru.resolve("HPDF_Page_SetLineCap");
if (libHaru_HPDF_Page_SetLineCap==NULL)
{
//cout << "Could NOT resolve HPDF_Page_SetLineCap !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_SetLineCap RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_SetLineJoin = (Prototype_HPDF_Page_SetLineJoin) libHaru.resolve("HPDF_Page_SetLineJoin");
if (libHaru_HPDF_Page_SetLineJoin==NULL)
{
//cout << "Could NOT resolve HPDF_Page_SetLineJoin !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_SetLineJoin RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_SetMiterLimit = (Prototype_HPDF_Page_SetMiterLimit) libHaru.resolve("HPDF_Page_SetMiterLimit");
if (libHaru_HPDF_Page_SetMiterLimit==NULL)
{
//cout << "Could NOT resolve HPDF_Page_SetMiterLimit !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_SetMiterLimit RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_SetDash = (Prototype_HPDF_Page_SetDash) libHaru.resolve("HPDF_Page_SetDash");
if (libHaru_HPDF_Page_SetDash==NULL)
{
//cout << "Could NOT resolve HPDF_Page_SetDash !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_SetDash RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_GetLineWidth = (Prototype_HPDF_Page_GetLineWidth) libHaru.resolve("HPDF_Page_GetLineWidth");
if (libHaru_HPDF_Page_GetLineWidth==NULL)
{
//cout << "Could NOT resolve HPDF_Page_GetLineWidth !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_GetLineWidth RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_MoveTo = (Prototype_HPDF_Page_MoveTo) libHaru.resolve("HPDF_Page_MoveTo");
if (libHaru_HPDF_Page_MoveTo==NULL)
{
//cout << "Could NOT resolve HPDF_Page_MoveTo !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_MoveTo RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_LineTo = (Prototype_HPDF_Page_LineTo) libHaru.resolve("HPDF_Page_LineTo");
if (libHaru_HPDF_Page_LineTo==NULL)
{
//cout << "Could NOT resolve HPDF_Page_LineTo !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_LineTo RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_Stroke = (Prototype_HPDF_Page_Stroke) libHaru.resolve("HPDF_Page_Stroke");
if (libHaru_HPDF_Page_Stroke==NULL)
{
//cout << "Could NOT resolve HPDF_Page_Stroke !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_Stroke RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_Fill = (Prototype_HPDF_Page_Fill) libHaru.resolve("HPDF_Page_Fill");
if (libHaru_HPDF_Page_Fill==NULL)
{
//cout << "Could NOT resolve HPDF_Page_Fill !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_Fill RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_BeginText = (Prototype_HPDF_Page_BeginText) libHaru.resolve("HPDF_Page_BeginText");
if (libHaru_HPDF_Page_BeginText==NULL)
{
//cout << "Could NOT resolve HPDF_Page_BeginText !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_BeginText RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_EndText = (Prototype_HPDF_Page_EndText) libHaru.resolve("HPDF_Page_EndText");
if (libHaru_HPDF_Page_EndText==NULL)
{
//cout << "Could NOT resolve HPDF_Page_EndText !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_EndText RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_MoveTextPos = (Prototype_HPDF_Page_MoveTextPos) libHaru.resolve("HPDF_Page_MoveTextPos");
if (libHaru_HPDF_Page_MoveTextPos==NULL)
{
//cout << "Could NOT resolve HPDF_Page_MoveTextPos !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_MoveTextPos RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_ShowText = (Prototype_HPDF_Page_ShowText) libHaru.resolve("HPDF_Page_ShowText");
if (libHaru_HPDF_Page_ShowText==NULL)
{
//cout << "Could NOT resolve HPDF_Page_ShowText !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_ShowText RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_ClosePathStroke = (Prototype_HPDF_Page_ClosePathStroke) libHaru.resolve("HPDF_Page_ClosePathStroke");
if (libHaru_HPDF_Page_ClosePathStroke==NULL)
{
//cout << "Could NOT resolve HPDF_Page_ClosePathStroke !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_ClosePathStroke RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_SetRGBStroke = (Prototype_HPDF_Page_SetRGBStroke) libHaru.resolve("HPDF_Page_SetRGBStroke");
if (libHaru_HPDF_Page_SetRGBStroke==NULL)
{
//cout << "Could NOT resolve HPDF_Page_SetRGBStroke !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_SetRGBStroke RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_SetRGBFill = (Prototype_HPDF_Page_SetRGBFill) libHaru.resolve("HPDF_Page_SetRGBFill");
if (libHaru_HPDF_Page_SetRGBFill==NULL)
{
//cout << "Could NOT resolve HPDF_Page_SetRGBFill !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_SetRGBFill RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_Ellipse = (Prototype_HPDF_Page_Ellipse) libHaru.resolve("HPDF_Page_Ellipse");
if (libHaru_HPDF_Page_Ellipse==NULL)
{
//cout << "Could NOT resolve HPDF_Page_Ellipse !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_Ellipse RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_Arc = (Prototype_HPDF_Page_Arc) libHaru.resolve("HPDF_Page_Arc");
if (libHaru_HPDF_Page_Arc==NULL)
{
//cout << "Could NOT resolve HPDF_Page_Arc !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_Arc RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_Rectangle = (Prototype_HPDF_Page_Rectangle) libHaru.resolve("HPDF_Page_Rectangle");
if (libHaru_HPDF_Page_Rectangle==NULL)
{
//cout << "Could NOT resolve HPDF_Page_Rectangle !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_Rectangle RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_GSave = (Prototype_HPDF_Page_GSave) libHaru.resolve("HPDF_Page_GSave");
if (libHaru_HPDF_Page_GSave==NULL)
{
//cout << "Could NOT resolve HPDF_Page_GSave !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_GSave RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_GRestore = (Prototype_HPDF_Page_GRestore) libHaru.resolve("HPDF_Page_GRestore");
if (libHaru_HPDF_Page_GRestore==NULL)
{
//cout << "Could NOT resolve HPDF_Page_GRestore !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_GRestore RESOLVED !" << endl;
}*/
libHaru_HPDF_SetCompressionMode = (Prototype_HPDF_SetCompressionMode) libHaru.resolve("HPDF_SetCompressionMode");
if (libHaru_HPDF_SetCompressionMode==NULL)
{
//cout << "Could NOT resolve HPDF_SetCompressionMode !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_SetCompressionMode RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_SetTextMatrix = (Prototype_HPDF_Page_SetTextMatrix) libHaru.resolve("HPDF_Page_SetTextMatrix");
if (libHaru_HPDF_Page_SetTextMatrix==NULL)
{
//cout << "Could NOT resolve HPDF_Page_SetTextMatrix !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_SetTextMatrix RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_Concat = (Prototype_HPDF_Page_Concat) libHaru.resolve("HPDF_Page_Concat");
if (libHaru_HPDF_Page_Concat==NULL)
{
//cout << "Could NOT resolve HPDF_Page_Concat !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_Concat RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_Eofill = (Prototype_HPDF_Page_Eofill) libHaru.resolve("HPDF_Page_Eofill");
if (libHaru_HPDF_Page_Eofill==NULL)
{
//cout << "Could NOT resolve HPDF_Page_Eofill !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_Eofill RESOLVED !" << endl;
}*/
libHaru_HPDF_Page_TextWidth = (Prototype_HPDF_Page_TextWidth) libHaru.resolve("HPDF_Page_TextWidth");
if (libHaru_HPDF_Page_TextWidth==NULL)
{
//cout << "Could NOT resolve HPDF_Page_TextWidth !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Page_TextWidth RESOLVED !" << endl;
}*/
libHaru_HPDF_SetInfoAttr = (Prototype_HPDF_SetInfoAttr) libHaru.resolve("HPDF_SetInfoAttr");
if (libHaru_HPDF_SetInfoAttr==NULL)
{
//cout << "Could NOT resolve HPDF_SetInfoAttr !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_SetInfoAttr RESOLVED !" << endl;
}*/
libHaru_HPDF_LoadType1FontFromFile = (Prototype_HPDF_LoadType1FontFromFile) libHaru.resolve("HPDF_LoadType1FontFromFile");
if (libHaru_HPDF_LoadType1FontFromFile==NULL)
{
//cout << "Could NOT resolve HPDF_LoadType1FontFromFile !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_LoadType1FontFromFile RESOLVED !" << endl;
}*/
libHaru_HPDF_GetTTFontDefFromFile = (Prototype_HPDF_GetTTFontDefFromFile) libHaru.resolve("HPDF_GetTTFontDefFromFile");
if (libHaru_HPDF_GetTTFontDefFromFile==NULL)
{
//cout << "Could NOT resolve HPDF_GetTTFontDefFromFile !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_GetTTFontDefFromFile RESOLVED !" << endl;
}*/
libHaru_HPDF_LoadTTFontFromFile = (Prototype_HPDF_LoadTTFontFromFile) libHaru.resolve("HPDF_LoadTTFontFromFile");
if (libHaru_HPDF_LoadTTFontFromFile==NULL)
{
//cout << "Could NOT resolve HPDF_LoadTTFontFromFile !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_LoadTTFontFromFile RESOLVED !" << endl;
}*/
libHaru_HPDF_LoadTTFontFromFile2 = (Prototype_HPDF_LoadTTFontFromFile2) libHaru.resolve("HPDF_LoadTTFontFromFile2");
if (libHaru_HPDF_LoadTTFontFromFile2==NULL)
{
//cout << "Could NOT resolve HPDF_LoadTTFontFromFile2 !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_LoadTTFontFromFile2 RESOLVED !" << endl;
}*/
libHaru_HPDF_SetCurrentEncoder = (Prototype_HPDF_SetCurrentEncoder) libHaru.resolve("HPDF_SetCurrentEncoder");
if (libHaru_HPDF_SetCurrentEncoder==NULL)
{
//cout << "Could NOT resolve HPDF_SetCurrentEncoder !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_SetCurrentEncoder RESOLVED !" << endl;
}*/
libHaru_HPDF_LoadPngImageFromFile = (Prototype_HPDF_LoadPngImageFromFile) libHaru.resolve("HPDF_LoadPngImageFromFile");
if (libHaru_HPDF_LoadPngImageFromFile==NULL)
{
//cout << "Could NOT resolve HPDF_LoadPngImageFromFile !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_LoadPngImageFromFile RESOLVED !" << endl;
}*/
libHaru_HPDF_LoadJpegImageFromFile = (Prototype_HPDF_LoadJpegImageFromFile) libHaru.resolve("HPDF_LoadJpegImageFromFile");
if (libHaru_HPDF_LoadJpegImageFromFile==NULL)
{
//cout << "Could NOT resolve HPDF_LoadJpegImageFromFile !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_LoadJpegImageFromFile RESOLVED !" << endl;
}*/
libHaru_HPDF_Image_SetMaskImage = (Prototype_HPDF_Image_SetMaskImage) libHaru.resolve("HPDF_Image_SetMaskImage");
if (libHaru_HPDF_Image_SetMaskImage==NULL)
{
//cout << "Could NOT resolve HPDF_Image_SetMaskImage !" << endl;
ret=false;
}
/*else
{
cout << "HPDF_Image_SetMaskImage RESOLVED !" << endl;
}*/

/// if (ret==true) cout << "Everything RESOLVED in libHaru" << endl;

return ret;
}

bool is_Haru_lib_usable(QString lib)
{
QString old_filename=path_to_libharu.fileName();
path_to_libharu.setFileName(lib);
bool ret=init_libHaru_library();
path_to_libharu.setFileName(old_filename);
(void) init_libHaru_library();
return ret;
}

