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

#ifndef COMPRESSDECOMPRESS_H
#define COMPRESSDECOMPRESS_H

#include <QByteArray>
#include <fstream>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QInputDialog>
#include <QtEndian>
#include <QtDebug>
#include <QMessageBox>
#else
#include <QtGui>
#endif

//FileType
#define WHOLE_FILE 0
#define QTGRACE_PROJECT_FILE 1
#define EXTERNAL_FILE 2

//OutputType
#define OUTPUT_ADDCOMPRDECOMPR 0
#define OUTPUT_OVERWRITE 1
#define OUTPUT_ASKTARGETFILE 2

//Compression-method
#define COMPRESSION_NONE 0
#define COMPRESSION_QTINTERNAL 1
#define COMPRESSION_EXTERNAL_ZLIB 2

using namespace std;

int LoadFileIntoByteArray(QString FileName,QByteArray & ba);
int SaveByteArrayToFile(QString FileName,QByteArray & ba);
void SeparateByteArray(QByteArray & ba,int & start_compr_line,int & stop_compr_line,int & compr_method);//takes the ByteArray and tries to find the position where the QtGrace-Project-File-Header ends
int DeCompressByteArray_simple(QByteArray & ba);//this decompresses data in a file if it is a QtGrace-project file with compressed data; otherwise: ba will stay untouched

void CompressFile(QString FileName,int FileType,int CompressionLevel,int OutputType);//Compresses a file with the internal Qt-Compression method. FileType can be a whole file (from beginning to end), QtGrace-project file (only data is compressed, not the header) and external file (which is already compressed - therefore nothing is done with these files)
void DeCompressFile(QString FileName,int FileType,int OutputType,int method);//Decompressed a File using the internal Qt-Compression method; FileType ca be a whole file (compressed with the internal Qt-compression method), a QtGrace-project file (only data is decompressed, not the header) and external file (something that has been compressed with an external library like zlib, only for this setting the mehod is needed. 0=none, 1=QtInternal, 2=zlib, 3=...)
void ExternalDeCompressFileZlib(QString FileName,int OutputType,int predictedSize);

int DecompressByteArray(QByteArray & ba,int method,quint32 predictedSize);
int CompressByteArray(QByteArray & ba, int method, int level);

#endif // COMPRESSDECOMPRESS_H
