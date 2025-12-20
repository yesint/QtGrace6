/***************************************************************************
 *   Copyright (C) 2015                                                    *
 *                                                                         *                                                                         *
 *   This file is free software; you can redistribute it and/or modify     *
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

#ifndef SERVER_H
#define SERVER_H

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
#include <QTcpSocket>
#include "graphutils.h"
#include "files.h"
#include "ssdata.h"
#include <QTime>

//#include <strstream>
#include <sstream>

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
#define  DEBUGDETAILS (QString("\nFile: ")+ __FILE__+ QString("\nFunction: ")+ __FUNCTION__ + QString("\nLine: ") + QString::number(__LINE__))

enum dataCommands{

    //! Each communication with a client
    //! is initialized with a command to tell QtGrace
    //! how to handle incoming data.
    PLOT_INFO,
    WRITE_DATAVEC,
    WRITE_DATAVEC_FINISHED,
    SET_POLAR_PLOT,
    REDRAW,
    PS_FILENAME,
    SET_SCALING_MODE,
    REDRAW_AND_WRITEPS,
    SET_LAYOUT_MODE,
    KILL_CHILD,
    TEST_CONNECTION,
    END_COMM,
    SET_QTGRACE_FOOTER,
    START_AUTO_UPDATE,
    STOP_AUTO_UPDATE
};


enum plotModes{

    //! The client can send different plot layouts
    AUTOSCALE_ALL_AXES_OR_JOIN_PLOT,
    AUTOSCALE_Y_AXIS_OR_OVERLAY,
    DEFAULT_LAYOUT,
    GRAPH_POSITION,
    AUTOSCALE_X_AXIS_OR_OVERLAY
};


enum readCommands{
    //! Tell QtGrace what to read from the client
    START_READ,
    READ_DATALENGTH,
    READ_DATASET_1,
    READ_PLOT_SETTINGS_1_FROM_CLIENT,
    READ_PLOT_SETTINGS_2_FROM_CLIENT
};

enum ComMode {
    //! Internal communication states
    initComm,
    endComm,
    readDataComm,
    sendDataCom,
    dataSizeComm
};

QT_BEGIN_NAMESPACE
class QTcpServer;
class KeyAndMousePressFilter;
class QProgressDialog;
QT_END_NAMESPACE

class QtGraceTcpServer: public QObject
{
    Q_OBJECT
public:
    QtGraceTcpServer(QString readTcpPort, QString sendServerSocketName, QObject *parent);
    ~QtGraceTcpServer();

    //! Length of data received from Client
   static int           dataLength_m;
   static int           dataLength_m1;

private slots:

    //! Returns any socket errors (for debug)
    void    socketError(QAbstractSocket::SocketError);

private:

    //! Convert the data received from the client to a plot command
    void getCommandFromClient(int commandFromsocket);

    //! Based on the received plot command from the client execute task.
    void executeTaskFromClient();

    //! Read .ps filename from socket and set the QtGrace document name.
    void    readPsFileName();

    //! Set layout settings received from client
    void    setLayoutMode();

    //! Set graph scalling received from client
    void    setScalingMode();

    //! Writes all the data received from client to a temp file. Following the data is read to QtGrace
    void    writeDataToTmpFile();

    //! Send graph data to client (for PD files)
    void    sendParam();

    //! Copy data from socket received from client
    char*   copyDataFromSocket(int availableBytes, char* dataFromSocket_m);

    //! Read graph data and plot settings from client
    void    readDataFromSocket(char *dataFromSocket_m, int availableBytes, readCommands readMode);

    //! Save the data read from the socket
    void    saveDataFromSocket(int numberOfRead);

    //! Read and clean x and y plot data
    void    readXYData(char* xData, char* yData);

    //! Read data from socket (client) and process the data.
    void    readFromClient(std::istringstream *dataFromClient);

private:

    bool enableOutput_;
    bool isPlotting_;
    bool setPlottingStatus_;
    //! To enable debug
    bool isDebugFlagOn_m;

    //! Message from client
    char                *messagePtr_m;

    //! The data received from the client part1
    char                *dataSet1Ptr_m;

    //! The data received from the client part2
    char                *dataSet2Ptr_m;

    //! What to read command
    dataCommands        command_m;


    int                 dataLengthToWrite_m;

    //! Graph number
    int                 graphNo_m;

    //! Min plot x-axis length
    double              xmin_m;

    //! Max plot x-axis length
    double              xmax_m;

    //! Condition to exit read function
    int                 conditionToExitFunction_m;

    //! Count the number of time there has been read from the socket.
    //! Depending on the number, different tasks are performed
    quint16             countNoOfRead_m;

    //! Indicator to new data is ready on the socket to be read
    quint16             newDataSetReady_m;

    //! Flag to tell QtGrace when to write data received from client to a temporary file
    bool                isWriteToTmpFile_m;

    //! Total numbers of graphs
    int                 numGraphs_m;

    //! Counter to count the numbers of dataset received from the client
    int                 countNoOfDataSets_m;

    //! Status of QtGrace warning messages (should there be a warning or not)
    int                 oldNoask_m;

    //! File for debug messages
    QFile               *debugFilePtr_m;

    //! Stream for debugging
    QTextStream         *debugOutPtr_m;

    //! Graph layout is given by the mode send by Client
    plotModes            mode_m;

    //! Pointer to min plot x-axis length
    double              *xminPtr_m;

    //! Point to max plot x-axis length
    double              *xmaxPtr_m;

    //! Numbers of columns for the plot
    int                 columns_m;

    //! Numbers of rows for the plot
    int                 rows_m;

    //! QtGrace document name. The name is used for the plot to
    //! file and when exporting to a PD file
    string              qtGraceDocStrName_m;

    //! Buffer to save data from client
    QByteArray          dataFromBuffer_m;

    //! buffer contains all the data received from client.
    //! The buffer is then loaded to QtGrace
    QBuffer             buffer_m;

    //! Number of bytes available on the socket
    qint64              bytesNeededFromSocket_m;

    //! Name on the server (used to estabilish communication between Client and QtGrace
    QString             writeTcpPortFromClient_m;

    //! Name on the client (used to estabilish communication between Server and QtGrace
    QString             readTcpPortFromClient_m;

    //! Save the numbers of data sets
    QList<int>          saveCountNoOfDataSets_m;

    //! New TCP server to read data from client
    QTcpServer *readServerPtr_m;

    //! New TCP socket to send data to client
    QTcpServer *writeServerPtr_m;

    //! Socket to write to client
    QTcpSocket *writeConnectionPtr_m;

    //! Save the data received from socket
    QByteArray dataFromSocket_m;

    //! Send data from QtGrace to client as a stream
    std::ostringstream *graphDataStreamToSendPtr_m;

    //! Internal state to receive and send data from/to client
    ComMode comMode_m;

    //! Remaining data on socket
    int remainingDataSize_m;

    //! Enable Footer text
    bool enableFooterText_m;

    //! Text revceived from
    char *footerText_m;

    //! Timer to automatic sending data to server
    QTimer *timer_;

    bool isAutoUpdate_;

    bool isTCPErrorToStatusBar_;

    KeyAndMousePressFilter *keyPressAndMousePressfilter_;

    bool allowUpdate_;

    //! function to read from footer text from server
    void readFooterText();

    QProgressDialog  *receiveDataProgressDialog_;

    int progressCounter_;

    void createProgressDialog();

    QTime t;

    bool allowExitQtGrace_;

public slots:

    #if QT_VERSION >= 0x050100
    void applicationStateChanged(Qt::ApplicationState state);
    #endif

    bool getAllowExitQtGrace() const;
    void startAutoUpdate();

protected:

    //!Function to write to a debug text file
    void writeToDebugFile(QString message,  QString debugDetails ="", bool isSkipLineNr = false);

    //! Read the data from the socket and process it
    void processDataFromClient();

    //! Write data to a data stream
    void writeToDataStream(const char *data, int len);

    //! Send data to client
    void sendData(const char *data, int bytesToSend);

    //! Read data from client
    void readData(QTcpSocket *readConnection);

protected slots:
    //! Main communication protocol function
    void communicateWithClient();

    //! Initialize server to read data from client
    void initReadServer();

    //! Initialize server to send data to client
    void initWriteServer();

    //! Number of bytes written to socket
    void dataWritten(qint64 iData);

    //! Server to write has been connected to client socket
    void writeSocketConnected();

    //! Server to write has been disconnected to client socket
    void writeSocketDisconnected();

    //! Server to read has been connected to client socket
    void readSocketConnected();

    //! Server to read has been disconnected to client socket
    void readSocketDisconnected();

};

#endif


