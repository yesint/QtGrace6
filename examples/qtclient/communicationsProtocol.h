/***************************************************************************
 *   Copyright (C) 2015 by Nimalendiran Kailasanathan                      *
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


#ifndef COMMUNICATIONSPROTOCOL_H
#define COMMUNICATIONSPROTOCOL_H

#include <QTcpSocket>
#include <strstream>

#define  DEBUGDETAILS (QString("\nFile: ")+ __FILE__+ QString("\nFunction: ")+ __FUNCTION__ + QString("\nLine: ") + QString::number(__LINE__))

enum ComMode {

    //! Internal communication states
    initComm,
    endComm,
    sendDataComm,
    readDataComm,
    dataSizeComm,
    startState,
    endState
};

enum readCommands{

    //! All of commands the below are understood by the client. Each
    //! communication is initialized with a command to tell the client
    //! how to handle incoming data.

    READ_PD_FILENAME_LENGTH,
    READ_PD_FILENAME,
    READ_DATA_LENGTH,
    READ_DATA

};

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QNetworkSession;
QT_END_NAMESPACE

class CommunicationsProtocol : public QObject
{
    Q_OBJECT

public:
    CommunicationsProtocol(QObject *parent);

    //! Save all plot commands to a stream
    void sendPlotExample();

    //! Start communication with QtGrace in order to send data
    void startCommunicationToRead();

    //! Start communication with QtGrace in order to read data
    void startCommunicationToSend();

    QString getHostAddressPtr_m() const;
    QString getWritePortPtr_m() const;
    QString getReadPortPtr_m() const;

    ~CommunicationsProtocol();

private slots:

    //! Main communication protocol function
    void communicateWithQtGrace();

    //!Display socket error
    void displayError(QAbstractSocket::SocketError socketError);

    //! Number of bytes written to socket
    void dataWritten(qint64 iData);

    void writeSocketDisconnected();

    //! Server to read has been disconnected to client socket
    void readSocketDisconnected();

    //! Server to write has been connected to client socket
    void writeSocketConnected();

    //! Server to read has been connected to client socket
    void readSocketConnected();



private:

    //!GUI variables
    QString hostAddressPtr_m;
    QString writePortPtr_m;
    QString readPortPtr_m;

    QString dataFromQtGracePtr_m;

    //! Client socket to read data from QtGrace
    QTcpSocket *readSocketPtr_m;

    //! Client socket to send data to QtGrace
    QTcpSocket *writeSocketPtr_m;

    //! Save the data received from socket
    QByteArray dataFromSocket_m;

    //! is debug flag enabled
    bool isDebugEnabled_m;

    //! We want to send data to QtGrace
    bool isSendPlot_m;

    //! Internal communication state
    ComMode comMode_m;

    //! Remaining data on socket
    int remainingDataSize_m;

    //! Count number of commands send to QtGrace
    int sendPlotData_m;

    //! Message from QtGrace
    char                *messagePtr_m;

    //! Number of bytes available on the socket
    qint64              availableBytesFromSocket_m;

    //! counter to read from QtGrace
    int                 countNoOfRead_m;

    //! Length of QtGrace document name
    int                 qtGraceDocStrNameLength_m;

    //! QtGrace document name
    char                *qtGraceDocStrNamePtr_m;

    //! Length of .arg file
    int                 paramDataLength_m;

    //! data from .agr file
    char                *paramDataPtr_m;

    //! Send data from client to QtGrace as a stream
    std::ostrstream *plotDataPtr_m;


private:
    //! Start socket communication
    void startCommunication();

    //! Initialize socket communication
    void initializeCommunication();

    //! Send data to QtGrace
    void sendDataToServer(char* data, int bytesToSend);

    //! Read data from QtGrace
    void readPlotFromQtGrace();

    //! Start QtGrace
    void startQtGrace();

    //! Write data to a stream
    void writeToDataStream(const char *data, int len);

    //! Read data from QtGrace
    void readPlotParametersFromQtGrace(std::istrstream *dataFromClient);

    //! Read the data from the socket and process it
    void processDataFromQtGrace();

    //! This function is used to wait until the connected socket signal has been emitted.
    void waitToConnectedToServer();

    //! Print debug to cerr
    void printDebug(QString message);

    void createNewDataStream();

    void waitForDisconnect();

public:

    void cleanPlot();
    void enablePlot();
    void endTransmission();
    void killChild();
    void sendPlotInfo(QString plot_info);
    void sendScalingModeInfo(int graphNo, int mode, double xStart_value, double xEnd_value);
    void sendLayoutModeInfo(int graphNo, int layout, int col, int numGraphs);
    void sendDataVector(int dataLength, double *xData, double *yData);
    void setReadingMode();
    void sendPSFileName(QString file_name);
    void setRedrawMode();
    void setRedrawAndWritePSMode();
    void sendDataSetToQtGrace(double *x, double *y);

    QString readPlotParametersFromQtGrace();

protected:
    int PLOT_INFO;
    int WRITE_DATAVEC;
    int WRITE_DATAVEC_FINISHED;
    int REDRAW;
    int PS_FILENAME;
    int SET_SCALING_MODE;
    int REDRAW_AND_WRITEPS;
    int SET_LAYOUT_MODE;
    int KILL_CHILD;
    int END_COMM;
    //! All of the above are commands understood by QtGrace.  Each
    //! communication is initialized with a command to tell QtGrace
    //! how to handle incoming data.
};


#endif // COMMUNICATIONSPROTOCOL_H
