#ifndef SERVER_H
#define SERVER_H

#endif // SERVER_H

#include <QtNetwork/QLocalServer>
#include <stdlib.h>
#include <stdio.h>
#include "globals.h"
#include <sys/types.h>
#include "device.h"
#include "plotone.h"
#include "graphs.h"
#include "fundamentals.h"
#include "xprotos.h"
#include "utils.h"
#include <sys/types.h>
#include <assert.h>
#include <sys/stat.h>

#include <graphutils.h>
#include <files.h>
#include <ssdata.h>
#define MAXERR 5
/*
 * number of rows to allocate for each call to realloc
 */
#define BUFSIZE  512

/*
 * number of bytes in each line chunk
 * (should be related to system pipe size, typically 4K)
 */
#ifndef PIPE_BUF
#  define PIPE_BUF 4096
#endif
#define CHUNKSIZE 2*PIPE_BUF

class LocalSocketIpcServer: public QObject
{
    Q_OBJECT
public:
    LocalSocketIpcServer(QString writeServerName,QString readServerName, QObject *parent);
    ~LocalSocketIpcServer();


public slots:
    void readSocket();
    void sendData();
    void readToQtGrace();
    int getdata(int gno, char *fn, int src, int load_type);
    void parse_qtGrace_Additions(char * s);
    int uniread(FILE *fp, int load_type, char *label);
    int read_long_line(FILE *fp, char **linebuf, int *buflen);
    int expand_line_buffer(char **adrBuf, int *ptrSize, char **adrPtr);


private:
    QLocalServer*       m_fromBeast;
    QLocalServer*       m_toBeast;
    QString             m_data;
    char                *socketData1;
    char                *socketData2;
    int                 command=0;
    int                 doneFlg = 0;
    int                 dataLength = 0;
    char                *message;
    QByteArray          dataLengthByte;
    quint16             countNoOfRead = 0;
    double              *x;
    double              *y;
    int                 k=0;
    bool                exchange_point_comma=false;//exchange ',' for '.' as decimal-seperator
    int                 new_set_no;
    int                 *new_set_nos=NULL;
    graph               *g;
    QList<QFont>        stdFontList;
    //QByteArray          data1;
    QString             str1;
    QByteArray          ba;
    char                *filename;
    QByteArray          byteArray;
    QBuffer             buffer;
    QTemporaryFile      file;

    QString             str2;
    QByteArray          ba2;
    char                *doubletest;

    quint16             countNoOfReadData = 0;
    quint16             readFirstMessage = 1;


};




