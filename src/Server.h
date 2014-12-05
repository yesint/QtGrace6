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
#include <QFile>
#include <QTextStream>

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

    //! Create a unique file name
    const char *createUniqueFileName();
    //! Read data from socket (ViewBeast) and process the data.
    void    readSocket();
    //! Connect to ViewBeast to send data from QtGrace to ViewBeast
    void    ConnectToBeast(const char* sendParam, int sendLen);
    //! Send data from QtGrace to ViewBeast
    void    sendDataToBeast();
    //! Check if socket is disconnected (for debug)
    void    socketDisconnected();
    //! Check if socket is ready to be read from (for debug)
    void    socketReadReady();
    //! Returns any socket errors (for debug)
    void    socketError(QLocalSocket::LocalSocketError);

private:
    //! Read .ps filename from socket and set the QtGrace document name.
    void    readPsFileName();
    //! Set layout settings received from ViewBeast
    void    setLayoutMode();
    //! Set graph scalling received from ViewBeast
    void    setScalingMode();
    //! Writes all the data received from ViewBeast to a temp file. Following the data is read to QtGrace
    void    writeDataToTmpFile();
    //! Send graph data to ViewBeast (for PD files)
    void    sendParam();
    //! Copy data from socket received from ViewBeast
    char*   copyDataFromSocket(int availableBytes, char* dataFromSocket);
    //! Read graph data and plot settings from ViewBeast
    void    readDataFromSocket(char *dataFromSocket,int availableBytes, int dataType);
    //! Save the data read from the socket
    void    saveDataFromSocket(int numberOfRead);
    //! Read data from a file and load it to QtGrace
   // int     getdata(int gno, char *fn, int src, int load_type);
    //!
   // void    parse_qtGrace_Additions(char * s);
    //!
  //  int     uniread(FILE *fp, int load_type, char *label);
    //!
  //  int     read_long_line(FILE *fp, char **linebuf, int *buflen);
    //!
  //  int     expand_line_buffer(char **adrBuf, int *ptrSize, char **adrPtr);
    //! Read and clean x and y plot data
    void    readXYData(char* xData, char* yData);

private:
    //! New local server to read data from ViewBeast
    QLocalServer*       messageFromBeastPtr;
    //! New local socket to send data to ViewBeast
    QLocalSocket*       messageToBeastPtr;
    //! Name on the server (used to estabilish communication between ViewBeast and QtGrace
    QString             readServer;
    //! Variable to ensure new data is not send before the old data has been send from QtGrace to ViewBeast
    bool                socketConnectedBusy;
    //! Graph data and settings (PD file)
    const char*         messageSendGraphParam;
    //! Length of: "messageSendGraphParam"
    int                 messageParamGraphLength;
    //!
    char                *dataSet1Ptr;
    //!
    char                *dataSet2Ptr;
    //!
    int                 *newSetNosPtr;
    //!
    graph               *graphPtr;
    //!
    char                *messagePtr;
    //! Pointer to min plot x-axis length
    double              *xminPtr;
    //! Point to max plot x-axis length
    double              *xmaxPtr;
    //! What to read command
    int                 command;
    //! Length of data received from ViewBeast
    int                 dataLength;
    //! Graph layout is given by the mode send by ViewBeast
    int                 mode;
    //! Numbers of columns for the plot
    int                 columns;
    //! Graph number
    int                 graphNo;
    //! Numbers of rows for the plot
    int                 rows;
    //! Total numbers of graphs
    int                 numGraphs;
    //! Min plot x-axis length
    double              xmin;
    //! Max plot x-axis length
    double              xmax;
    //! QtGrace document name. The name is used for the plot to file and when exporting to a PD file
    string              qtGraceDocStrName;
    //! Condition to exit read function
    int                 conditionToExitFunction;
    //! Count the number of time there has been read from the socket. Depending on the number different taks are performed
    quint16             countNoOfRead;
    //! Indicator to new data is ready on the socket to be read
    quint16             newDataSetReady;
    //!
    int                 new_set_no;
    //!
    QList<QFont>        stdFontList;
    //!
    QString             fileNameStr;
    //!
    QByteArray          dataFromBuffer;
    //! buffer contains all the data received from ViewBeast. The buffer is then loaded to QtGrace
    QBuffer             buffer;

    //!
    bool                writeToTmpFile;
    //!
    int                 countNoOfDataSets;
    //!
    QList<int>          saveCountNoOfDataSets;
    //! Status of QtGrace warning messages (should there be a warning or not)
    int                 oldNoask;
    //! Number of bytes available on the socket
    qint64              availableBytesFromSocket;
    //! Byte array used to save a temporary file name
    QByteArray          fileNameBa;

    //! files for debug
    QFile      *debugFile;
    QTextStream         *debugOut;
    bool debugFlag;
    bool readSocketIsLocked;
};




