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

#include "compressdecompress.h"

int LoadFileIntoByteArray(QString FileName,QByteArray & ba)
{
ba.clear();
//qDebug() << "LoadFileIntoByteArray: Loading from " << FileName;
QFile inFile(FileName);
inFile.open(QIODevice::ReadOnly);
ba=inFile.readAll();
inFile.close();
    //qDebug() << "loadedBytes=" << ba.length() << "loaded from" << FileName;
if (ba.length()<=0) return 1;
else return 0;
}

int SaveByteArrayToFile(QString FileName,QByteArray & ba)
{
    //qDebug() << "Saving to " << FileName;
QFile outFile(FileName);
outFile.open(QIODevice::WriteOnly);
qint64 res=outFile.write(ba);
outFile.close();
    //qDebug() << "ba.size=" << ba.length() << "writteBytes=" << res;
if (res==-1) return 1;
else
return 0;
}

void SeparateByteArray(QByteArray & ba,int & start_compr_line,int & stop_compr_line,int & compr_method)
{
start_compr_line=stop_compr_line=0;
compr_method=0;//no compression by default
QString st("@target");//present in Grace-project-files that contain data (not only header) -- if not present: no compressable data present
QString st2("#QTGRACE_ADDITIONAL_PARAMETER: DATA_COMPRESSED");//only present in files that actually have compressed data --> this shows where the compressed data begins
int a=ba.indexOf(st);
int b=ba.indexOf(st2);
int c=ba.indexOf(QString(st).toUpper());
char * compression_line=NULL;
//qDebug() << "Length=" << ba.length();
//qDebug() << "a=" << a << "b=" << b << "c=" << c;
if (b<0)//not a QtGrace-project file with compressed data (either no project file, or data is uncompressed)
{
    if (a<0 && c<0)//no data present
    {
    start_compr_line=stop_compr_line=ba.length();//we 'split' at the end, because there is no data left to compress
    }
    else//data present --> split here
    {
        if (a>=0)
        start_compr_line=stop_compr_line=a;
        else
        start_compr_line=stop_compr_line=c;
    }
}
else//we have a project file with compressed data in it
{
start_compr_line=b;
b=ba.indexOf(QChar('\n'),b);
stop_compr_line=b;
compression_line=new char[stop_compr_line-start_compr_line+8];
strcpy(compression_line,ba.mid(start_compr_line,stop_compr_line-start_compr_line));
//qDebug() << "compression_line=" << compression_line;
sscanf(compression_line,"#QTGRACE_ADDITIONAL_PARAMETER: DATA_COMPRESSED %d",&compr_method);
//qDebug() << "Project File. Method=" << compr_method;
delete[] compression_line;
}
}

int DeCompressByteArray_simple(QByteArray & ba)
{
int start_compr_line,stop_compr_line,compr_method;
QByteArray ba1,ba2,result;
SeparateByteArray(ba,start_compr_line,stop_compr_line,compr_method);
if (compr_method==0) return 0;//no compression --> return without doing anything
ba1=ba.left(start_compr_line);
ba2=ba.mid(stop_compr_line+1);
ba2=qUncompress(ba2);
if (ba2.isEmpty()) return 1;//unable to decompress
ba=QByteArray("#QTGRACE_ADDITIONAL_PARAMETER: DATA_COMPRESSED 0\n");
result=ba1+ba+ba2;
ba=result;
return 0;
}

void CompressFile(QString FileName,int FileType,int CompressionLevel,int OutputType)
{
QByteArray ba,ba1,ba2,result;
QString n_File;
QFileInfo fi(FileName);
int res=0,start_comp_line=0,stop_compr_line=0,comr_method=0,is_compressed=0;
if (FileType==EXTERNAL_FILE) return;//We can not compress a file that has already been compressed externally

    if (fi.exists()==false)
    {
    QMessageBox::information(0,QObject::tr("Error!"),QObject::tr("File ")+FileName+QObject::tr(" does not exist!"));
    return;
    }
res=LoadFileIntoByteArray(FileName,ba);
    if (FileType==WHOLE_FILE)
    {
    ba1=qCompress(ba,CompressionLevel);
    result=ba1;
    }
    else//QTGRACE_PROJECT_FILE
    {
    SeparateByteArray(ba,start_comp_line,stop_compr_line,comr_method);
    ba1=ba.left(start_comp_line);
    //ba2=ba.mid(stop_compr_line+1);
    ba2=ba.mid(stop_compr_line);
is_compressed=isCompressedProject(ba,start_comp_line,stop_compr_line);
//qDebug() << "File already compressed?" << is_compressed;
    if (is_compressed==1)
    {
    QMessageBox::information(0,QObject::tr("Warning!"),QObject::tr("File ")+FileName+QObject::tr(" already compressed!"));
    return;
    }

    /*qDebug() << "split: end of start    =" << ba1.right(10);
    qDebug() << "split: begin of second =" << ba2.left(10);
    qDebug() << "Split-File: HEADER" << endl << ba1;*/
    //qDebug() << "Compression-Line=" << endl << ba.mid(start_comp_line,stop_compr_line-start_comp_line);
    /*qDebug() << "Split-File: DATA" << endl << ba2;
    qDebug() << "Split-File: END";*/

    ba=QByteArray("#QTGRACE_ADDITIONAL_PARAMETER: DATA_COMPRESSED 1\n");
    ba2=qCompress(ba2,CompressionLevel);
    result=ba1+ba+ba2;
    }
    switch (OutputType)
    {
    default:
    case OUTPUT_ADDCOMPRDECOMPR://Add Compr/DeCompr To File Name(s)
    n_File=fi.absolutePath()+QDir::separator()+fi.baseName()+QString("_compr");
            if (fi.completeSuffix().length()>0)
            n_File+=QString(".")+fi.completeSuffix();
    break;
    case OUTPUT_OVERWRITE://Overwrite File(s)
    n_File=FileName;
    break;
    case OUTPUT_ASKTARGETFILE://Ask For Target File(s)
    n_File=QFileDialog::getSaveFileName(0,QObject::tr("Save File"),fi.absolutePath());
    break;
    }
res=SaveByteArrayToFile(n_File,result);
}

void DeCompressFile(QString FileName,int FileType,int OutputType,int method)
{
QByteArray ba,ba1,ba2,result;
QString n_File;
QFileInfo fi(FileName);
int res=0,start_comp_line=0,stop_compr_line=0,comr_method=0,is_compressed=0;
res=LoadFileIntoByteArray(FileName,ba);
    if (FileType==WHOLE_FILE)
    {
    ba1=qUncompress(ba);
    result=ba1;
    }
    else if (FileType==QTGRACE_PROJECT_FILE)//QTGRACE_PROJECT_FILE
    {
    SeparateByteArray(ba,start_comp_line,stop_compr_line,comr_method);
    ba1=ba.left(start_comp_line);
is_compressed=isCompressedProject(ba,start_comp_line,stop_compr_line);
//qDebug() << "File already compressed?" << is_compressed;
    if (is_compressed==0)
    {
    QMessageBox::information(0,QObject::tr("Warning!"),QObject::tr("File ")+FileName+QObject::tr(" already uncompressed!"));
    return;
    }
    //qDebug() << "Compression-Line=" << endl << ba.mid(start_comp_line,stop_compr_line-start_comp_line);
    ba2=ba.mid(stop_compr_line+1);
    ba2=qUncompress(ba2);
    ba=QByteArray("#QTGRACE_ADDITIONAL_PARAMETER: DATA_COMPRESSED 0\n");
    result=ba1+ba+ba2;
    }
    else//EXTERNAL_FILE
    {
        if (method==COMPRESSION_EXTERNAL_ZLIB)
        {
        ExternalDeCompressFileZlib(FileName,OutputType,4*fi.size());
        }
    }
    switch (OutputType)
    {
    default:
    case OUTPUT_ADDCOMPRDECOMPR://Add Compr/DeCompr To File Name(s)
    n_File=fi.absolutePath()+QDir::separator()+fi.baseName()+QString("_decompr");
            if (fi.completeSuffix().length()>0)
            n_File+=QString(".")+fi.completeSuffix();
    break;
    case OUTPUT_OVERWRITE://Overwrite File(s)
    n_File=FileName;
    break;
    case OUTPUT_ASKTARGETFILE://Ask For Target File(s)
    n_File=QFileDialog::getSaveFileName(0,QObject::tr("Save File"),fi.absolutePath());
    break;
    }
res=SaveByteArrayToFile(n_File,result);
}

void ExternalDeCompressFileZlib(QString FileName, int OutputType, int predictedSize)
{
QByteArray ba,ba1,ba2,result;
QString n_File;
QFileInfo fi(FileName);
quint32 res=0,len=0;
res=LoadFileIntoByteArray(FileName,ba);
//Note: If you want to use this function to uncompress external data
//that was compressed using zlib, you first need to prepend a
//four byte header to the byte array containing the data.
//The header must contain the expected length (in bytes) of the uncompressed data,
//expressed as an unsigned, big-endian, 32-bit integer.

if (predictedSize<=0)
{
bool ok;
len=QInputDialog::getInt(0,QObject::tr("Expected Length"),QObject::tr("What would be the length (in Bytes) of the uncompressed data?"),1024,-2147483647,2147483647,1,&ok,0);
if (!ok) return;
}
else
{
len=predictedSize;
}
quint32 exp_size=len;
quint8 dest[8];
qToBigEndian(exp_size,dest);
ba2=QByteArray((const char*)dest,4);
/// ba2=length...
ba1=ba2+ba;
result=qUncompress(ba1);
switch (OutputType)
{
default:
case OUTPUT_ADDCOMPRDECOMPR://Add Compr/DeCompr To File Name(s)
n_File=fi.absolutePath()+QDir::separator()+fi.baseName()+QString("_decompr");
        if (fi.completeSuffix().length()>0)
        n_File+=QString(".")+fi.completeSuffix();
break;
case OUTPUT_OVERWRITE://Overwrite File(s)
n_File=FileName;
break;
case OUTPUT_ASKTARGETFILE://Ask For Target File(s)
n_File=QFileDialog::getSaveFileName(0,QObject::tr("Save File"),fi.absolutePath());
break;
}
res=SaveByteArrayToFile(n_File,result);
}

int DecompressByteArray(QByteArray & ba, int method, quint32 predictedSize)
{
QByteArray ba2,ba3;
quint8 dest[8];
//qDebug() << "DeCompression Byte Array, predicted size=" << predictedSize;
switch (method)
{
default:
case COMPRESSION_NONE:
    ba2=ba;//Do nothing, Byte-Array already decompressed
break;
case COMPRESSION_QTINTERNAL:
    ba2=qUncompress(ba);
break;
case COMPRESSION_EXTERNAL_ZLIB:
    qToBigEndian(predictedSize,dest);
    ba2=QByteArray((const char*)dest,4);
    /// ba2=length...
    ba3=ba2+ba;
    ba2=qUncompress(ba3);
break;
}
if (ba2.isEmpty()) return 1;
ba=ba2;
return 0;
}

int CompressByteArray(QByteArray & ba,int method, int level)
{
QByteArray ba2;
switch (method)
{
default:
case COMPRESSION_NONE:
    ba2=ba;//Do nothing, Byte-Array already decompressed
break;
case COMPRESSION_QTINTERNAL:
    ba2=qCompress(ba,level);
break;
case COMPRESSION_EXTERNAL_ZLIB:
    /// EXTERNAL COMPRESSION MISSING
    ba2=ba;
break;
}
if (ba2.isEmpty()) return 1;
ba=ba2;
return 0;
}

int isCompressedProject(QByteArray & ba,int start_compr_line,int stop_compr_line)
{
QByteArray compr_line=ba.mid(start_compr_line,stop_compr_line-start_compr_line);
//qDebug() << "Compression-Line=" << endl << compr_line;
if (compr_line.length()<=0) return 0;//line not present --> uncompressed
int pos=compr_line.indexOf("COMPRESSED");
if (pos<0) return 0;//line not present --> uncompressed
QByteArray right_part=compr_line.mid(pos+10);
//qDebug() << "Right-part=#" << right_part << "#";
bool ok;
int val=right_part.toInt(&ok);
if (ok==false) return 0;//line not present --> uncompressed
return val;
}

