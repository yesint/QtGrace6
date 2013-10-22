#ifndef SERVER_H
#define SERVER_H

#endif // SERVER_H

#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>
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

#include "graphutils.h"
#include "files.h"
#include "ssdata.h"
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


private slots:
    void    readSocket();
    void    ConnectToBeast(const char* sendParam, int sendLen);
    void    sendDataToGrace();
    void    socket_disconnected();
    void    socket_readReady();
    void    socket_error(QLocalSocket::LocalSocketError);

private:
    void    doCreateNew();
    void    readPsFileName();
    void    setLayoutMode();
    void    setScalingMode();
    void    writeDataToTmpFile();
    void    sendParam();
    char*   copyDataFromSocket(int availableBytes, char* dataFromSocket);
    void    readDataFromSocket(char *dataFromSocket,int availableBytes, int dataType);
    void    saveDataFromSocket(int numberOfRead);
    int     getdata(int gno, char *fn, int src, int load_type);
    void    parse_qtGrace_Additions(char * s);
    int     uniread(FILE *fp, int load_type, char *label);
    int     read_long_line(FILE *fp, char **linebuf, int *buflen);
    int     expand_line_buffer(char **adrBuf, int *ptrSize, char **adrPtr);
    void    readXYData(char* xData, char* yData);

private:
    QLocalServer*       m_fromBeast;
    QLocalSocket*       m_toBeast;
    QString             readServer;
    QString             m_data;

    bool                socket_connected_busy;
    const char*         m_sendParam;
    int                 m_paramLen;

    //Data types
    char                *dataSet1;
    char                *dataSet2;
    char                *dataSet3;
    int                 command;
    int                 dataLength;
    int                 mode;
    int                 columns;
    int                 graphNo;
    int                 rows;
    int                 numGraphs;
    double*             xminPtr;
    double*             xmaxPtr;
    double              xmin;
    double              xmax;
    int                 printStrNameLength;
    string              printStrName;
    int                 conditionToExitFunction;
    char                *message;
    quint16             countNoOfRead;
    quint16             newDataSetReady;
    double              *x;
    double              *y;
    bool                exchange_point_comma;//exchange ',' for '.' as decimal-seperator
    int                 new_set_no;
    int                 *new_set_nos;
    graph               *g;
    QList<QFont>        stdFontList;
    QString             xValueStr;
    QString             yValueStr;
    QString             fileNameStr;
    QByteArray          fileNameBa;
    QByteArray          xValueBa;
    QByteArray          yValueBa;
    char                *xValueChar;
    char                *yValueChar;
    char                *fileNameChar;
    QByteArray          dataFromBuffer;
    QBuffer             buffer;
    QBuffer             xDataSetbuffer;
    QBuffer             yDataSetbuffer;
    quint16             countNoOfReadData;
    qint64              availableBytesFromSocket;
    bool                readSocketIsLocked;
    bool                writeToTmpFile;
    int                 gno;
    int                 load; //read single set
    int                 cursource; // read from temp file
    int                 paramLength;
    int                 countNoOfDataSets;
    QList<int>          saveCountNoOfDataSets;
};




