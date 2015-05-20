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


enum dataCommands{

    //! Each communication with a client
    //! is initialized with a command to tell QtGrace
    //! how to handle incoming data.
    PLOT_INFO,
    WRITE_DATAVEC,
    WRITE_DATAVEC_FINISHED,
    READ_MODE,
    REDRAW,
    PS_FILENAME,
    SET_SCALING_MODE,
    REDRAW_AND_WRITEPS,
    SET_LAYOUT_MODE,
    KILL_CHILD,
    TEST_CONNECTION,
    END_COMM
};


enum plotModes{

    //! The client can send different plot layouts
    AUTOSCALE_ALL_AXES_OR_JOIN_PLOT,
    AUTOSCALE_Y_AXIS_OR_OVERLAY,
    DEFAULT_LAYOUT,
    GRAPH_POSITION
};


enum readCommands{
    //! Tell QtGrace what to read from the client
    START_READ,
    READ_DATALENGTH,
    READ_DATASET_1,
    READ_PLOT_SETTINGS_1_FROM_CLIENT,
    READ_PLOT_SETTINGS_2_FROM_CLIENT
};



class LocalSocketIpcServer: public QObject
{
    Q_OBJECT
public:
    LocalSocketIpcServer(QString receiveClientSocketName, QString sendServerSocketName, QObject *parent);
    ~LocalSocketIpcServer();


    void getCommandFromClient(int commandFromsocket);

    void executeTaskFromClient();

private slots:

    //! Create a unique file name
    const char *createUniqueFileName();
    //! Read data from socket (client) and process the data.
    void    readFromClient();
    //! Connect to client to send data from QtGrace
    void    ConnectToClient(const char* sendParam, int sendLen);
    //! Send data from QtGrace to client
    void    sendDataToClient();
    //! Check if socket is disconnected (for debug)
    void    socketDisconnected();
    //! Check if socket is ready to be read from (for debug)
    void    socketReadReady();
    //! Returns any socket errors (for debug)
    void    socketError(QLocalSocket::LocalSocketError);

    void    createNewSocketConnection();

private:
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
    char*   copyDataFromSocket(int availableBytes, char* dataFromSocket);
    //! Read graph data and plot settings from client
    void    readDataFromSocket(char *dataFromSocket, int availableBytes, readCommands readMode);
    //! Save the data read from the socket
    void    saveDataFromSocket(int numberOfRead);
    //! Read and clean x and y plot data
    void    readXYData(char* xData, char* yData);

private:
    //! To enable debug
    bool isDebugFlagOn_m;
    //! Graph data and settings (PD file)
    const char*         messageSendGraphParam_m;
    //! Length of: "messageSendGraphParam"
    int                 messageParamGraphLength_m;
    //! Message from client
    char                *messagePtr_m;
    //! The data received from the client part1
    char                *dataSet1Ptr;
    //! The data received from the client part2
    char                *dataSet2Ptr_m;
    //! What to read command
    dataCommands        command_m;
    //! Length of data received from Client
    int                 dataLength_m;
    //! Graph number
    int                 graphNo_m;
    //! Min plot x-axis length
    double              xmin_m;
    //! Max plot x-axis length
    double              xmax_m;
    //! Condition to exit read function
    int                 conditionToExitFunction_m;
    //! Count the number of time there has been read from the socket.
    //! Depending on the number different taks are performed
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
    QFile               *debugFile_m;
    //! Stream for debugging
    QTextStream         *debugOut_m;
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
    qint64              availableBytesFromSocket_m;
    //! New local server to read data from client
    QLocalServer*       messageFromClienttPtr_m;
    //! New local socket to send data to client
    QLocalSocket*       messageToServerPtr_;
    //! Name on the server (used to estabilish communication between Client and QtGrace
    QString             sendServerSocketName_m;
    //! Name on the client (used to estabilish communication between Server and QtGrace
    QString             receiveClientSocketName_m;
    //! Save the numbers of data sets
    QList<int>          saveCountNoOfDataSets_m;

    QLocalSocket *clientConnection;
    QByteArray dataFromSocket;


};




