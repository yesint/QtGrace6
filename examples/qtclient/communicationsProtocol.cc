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


#include "communicationsProtocol.h"

#include <QtWidgets>
#include <QtNetwork>
#include <iostream>
#include <cassert>
#include <ctime>
#include <cstring>
#include <vector>
#include <QApplication>
#include <QMessageBox>

CommunicationsProtocol::CommunicationsProtocol(QObject *parent) :
    QObject(parent)
  ,isDebugEnabled_m(false)
  ,isSendPlot_m(true)
  ,comMode_m(startState)
  ,remainingDataSize_m(0)
  ,messagePtr_m(0)
  ,availableBytesFromSocket_m(0)
  ,countNoOfRead_m(0)
  ,qtGraceDocStrNameLength_m(0)
  ,qtGraceDocStrNamePtr_m(NULL)
  ,paramDataLength_m(0)
  ,paramDataPtr_m(NULL)
  ,plotDataPtr_m(NULL)
  ,PLOT_INFO(1)
  ,WRITE_DATAVEC(2)
  ,WRITE_DATAVEC_FINISHED(12)
  ,REDRAW(4)
  ,PS_FILENAME(5)
  ,SET_SCALING_MODE(6)
  ,REDRAW_AND_WRITEPS(7)
  ,SET_LAYOUT_MODE(8)
  ,KILL_CHILD(42)
  ,END_COMM(99)

{

    readSocketPtr_m = new QTcpSocket(this);
    writeSocketPtr_m = new QTcpSocket(this);

    QTcpSocket *dummywriteToDataStream = new QTcpSocket(this);
    QTcpSocket *dummyReadSocket  = new QTcpSocket(this);

    dummywriteToDataStream->bind(QHostAddress("127.0.0.1"),0, QAbstractSocket::ShareAddress);
    dummyReadSocket->bind(QHostAddress("127.0.0.1"),0, QAbstractSocket::ShareAddress);

    writePortPtr_m = QString::number(dummywriteToDataStream->localPort());
    readPortPtr_m = QString::number(dummyReadSocket->localPort());
    hostAddressPtr_m = dummyReadSocket->localAddress().toString();

    delete dummywriteToDataStream;
    delete dummyReadSocket;

    connect(readSocketPtr_m, SIGNAL(readyRead()), this, SLOT(communicateWithQtGrace()));
    connect(writeSocketPtr_m, SIGNAL(bytesWritten(qint64)), this, SLOT(dataWritten(qint64)));

    connect(readSocketPtr_m, SIGNAL(disconnected()), this, SLOT(readSocketDisconnected()));
    connect(writeSocketPtr_m, SIGNAL(disconnected()), this, SLOT(writeSocketDisconnected()));
    connect(readSocketPtr_m, SIGNAL(connected()), this, SLOT(readSocketConnected()));
    connect(writeSocketPtr_m, SIGNAL(connected()), this, SLOT(writeSocketConnected()));

    connect(readSocketPtr_m, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));

    connect(writeSocketPtr_m, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));

}


CommunicationsProtocol::~CommunicationsProtocol()
{
}

void CommunicationsProtocol::writeSocketDisconnected()
{    printDebug("Client: write socket disconnected");
}

void CommunicationsProtocol::readSocketDisconnected()
{    printDebug("Client: read socket disconnected ");
}

void CommunicationsProtocol::writeSocketConnected()
{    printDebug("Client: write socket connected ");
     initializeCommunication();
}

void CommunicationsProtocol::readSocketConnected()
{    printDebug("Client: read socket connected ");
     initializeCommunication();
}

QString CommunicationsProtocol::getWritePortPtr_m() const
{
    return writePortPtr_m;
}

QString CommunicationsProtocol::getHostAddressPtr_m() const
{
    return hostAddressPtr_m;
}

QString CommunicationsProtocol::getReadPortPtr_m() const
{
    return readPortPtr_m;
}


void CommunicationsProtocol::startCommunicationToRead(){
    isSendPlot_m = false;
    dataFromQtGracePtr_m ="";
    startCommunication();
}

void CommunicationsProtocol::startCommunicationToSend(){
    isSendPlot_m = true;
    startCommunication();
}

void CommunicationsProtocol::startCommunication()
{
    comMode_m = endComm; // This is for initialization.
    //blockSize = 0;
    if(writeSocketPtr_m->state() == QAbstractSocket::ConnectedState ){
        writeSocketPtr_m->disconnectFromHost();
    }

    if(readSocketPtr_m->state() == QAbstractSocket::ConnectedState ){
        readSocketPtr_m->disconnectFromHost();
    }

 printDebug("Client: connect read socket");

    readSocketPtr_m->connectToHost(hostAddressPtr_m,
                                   readPortPtr_m.toInt());

    printDebug("Client: connect write socket");

    writeSocketPtr_m->connectToHost(hostAddressPtr_m,
                                    writePortPtr_m.toInt());

    waitToConnectedToServer();


}

void CommunicationsProtocol::dataWritten(qint64 iData)
{
    printDebug("bytes written to client: " + QString::number(iData));

}

void CommunicationsProtocol::initializeCommunication()
{
    if( comMode_m == endComm &&
            readSocketPtr_m->state() == QAbstractSocket::ConnectedState  &&
            writeSocketPtr_m->state() == QAbstractSocket::ConnectedState ){

        printDebug("Client: initialize connection");

        // Client initializes the communication.
        comMode_m = initComm;
        int rc = writeSocketPtr_m->write(reinterpret_cast<char*>(&comMode_m), sizeof(ComMode));
        assert(rc == sizeof(comMode_m));
    }
    else {
        printDebug("Client: not all sockets ready yet");
    }
}
void CommunicationsProtocol::communicateWithQtGrace()
{

    printDebug("Communicate with server");

    // Make sure that we have a working connection.
    assert( readSocketPtr_m->state() == QAbstractSocket::ConnectedState );
    assert( writeSocketPtr_m->state() == QAbstractSocket::ConnectedState );

    do {
        int rc = 0;
        if(remainingDataSize_m <= 0){

            // First the server sends the init signal, to tell that communication is ready!
            rc = readSocketPtr_m->read(reinterpret_cast<char*>(&comMode_m), sizeof(ComMode));
            assert(rc == sizeof(ComMode));

            printDebug("Client read: " +
                       QString::number(rc) +
                       " bytes, with command: "
                       + QString::number(comMode_m));

        }
        else {
            assert(comMode_m=readDataComm);
        }

        if( comMode_m == initComm ){

            printDebug("Client in initComm");


            // That was the initialization signal, we send some data

            if(isSendPlot_m){
                comMode_m = sendDataComm;
                sendDataToServer(plotDataPtr_m->str(),plotDataPtr_m->pcount());
                delete plotDataPtr_m;
                plotDataPtr_m = NULL;

            }
            else {
                assert(writeSocketPtr_m);
                // We want the server to send data, lets tell it
                ComMode command = readDataComm;
                int rc = writeSocketPtr_m->write(reinterpret_cast<char*>(&command), sizeof(ComMode));
                assert(rc == sizeof(comMode_m));

                printDebug("Client send read data command");

            }

        }

        if( comMode_m == readDataComm ){
            printDebug("Client ready to read data from client");
            readPlotFromQtGrace();
        }

        if( comMode_m == endComm ){
            // Here we are if the server is done with communication.
            // We close the sockets.

            printDebug("Client - in endComm");

            if(writeSocketPtr_m->state() == QAbstractSocket::ConnectedState ){
                writeSocketPtr_m->disconnectFromHost();
            }

            if(writeSocketPtr_m->state() == QAbstractSocket::ConnectedState ){
                writeSocketPtr_m->disconnectFromHost();
            }

            remainingDataSize_m = 0;

            if(!isSendPlot_m){
                // We are done, reset the read data counter.
                countNoOfRead_m = 0;
            }

            comMode_m = endState;

        }
    } while(readSocketPtr_m->bytesAvailable() > 0);
    // Note, that we need this while loop because the readyRead signal is NOT send recursively!
    // Meaning that if new data arrives while we are still in ReadyRead there will not be a signal submitted
    // thus we need to check if data is avaiable.
}

void CommunicationsProtocol::sendDataToServer(char* data, int bytesToSend)
{

    printDebug("Client is going to send packet with " +
               QString::number(bytesToSend) +
               " bytes");

    ComMode comSignal = sendDataComm;
    int rc = writeSocketPtr_m->write(reinterpret_cast<char*>(&comSignal), sizeof(ComMode));
    assert(rc == sizeof(ComMode));

    printDebug("Client has send data packet");

    rc = writeSocketPtr_m->write(reinterpret_cast<char*>(&bytesToSend), sizeof(int));
    assert(rc == sizeof(int));


    int bytesWritten = 0;
    do {
        rc = writeSocketPtr_m->write(reinterpret_cast<char*>(&data[bytesWritten]), bytesToSend-bytesWritten);
        if( rc >= 0) bytesWritten += rc;
    }while( rc >=0 && bytesWritten < bytesToSend);

    printDebug("Client is done sending, send end command");

    // Make sure data has been send before we delete memory.
    writeSocketPtr_m->flush();
}

void CommunicationsProtocol::readPlotFromQtGrace()
{
    assert(readSocketPtr_m);
    assert(remainingDataSize_m>=0);

    // We tell the server how much data we want to read.
    int bytesToRead = remainingDataSize_m;
    int rc = 0;

    printDebug("Client is reading data from client");

    if( remainingDataSize_m == 0 ){
        rc = readSocketPtr_m->read(reinterpret_cast<char*>(&bytesToRead), sizeof(int));
        assert(rc == sizeof(int));
    }

    char* data = new char[bytesToRead];

    printDebug("Client is going to read packet with " +
               QString::number(bytesToRead) +
               " bytes");


    int bytesRead = 0;

    while(bytesToRead > 0 && readSocketPtr_m->bytesAvailable() > 0){
        int packetSize = readSocketPtr_m->read(&data[bytesRead], bytesToRead);
        assert(packetSize >= 0 && packetSize <= bytesToRead);
        dataFromSocket_m.append(data,packetSize);
        bytesRead += packetSize;
        bytesToRead -= packetSize;

        printDebug("Client has read packet with " +
                   QString::number(packetSize) +
                   " bytes");
    }


    delete [] data;

    if( bytesToRead == 0){
        printDebug("Client is done reading, send end command");

        processDataFromQtGrace();
        dataFromSocket_m.clear();

    }


    remainingDataSize_m = bytesToRead;
    // Note, there might be remaining bytes to read, if so, there will be another readyRead signal send.
    // Therefore we store the amount of data that we still need to read and read it with the next signal.
}






void CommunicationsProtocol::readPlotParametersFromQtGrace(std::istrstream *dataFromClient)
{
    //Specifiy the amount of bytes to be read

    if(countNoOfRead_m==READ_DATA) {
        availableBytesFromSocket_m=paramDataLength_m;
    }
    else if(countNoOfRead_m==READ_PD_FILENAME){
        availableBytesFromSocket_m = qtGraceDocStrNameLength_m;
    } else{
        availableBytesFromSocket_m = (int)sizeof(int);
    }

    messagePtr_m=new char[availableBytesFromSocket_m+1];
    messagePtr_m[availableBytesFromSocket_m]=0; // ensure that
    // 0-terminated character strings
    // come here correctly.

    // Read all data from socket
    dataFromClient->read(messagePtr_m,availableBytesFromSocket_m);


    switch (countNoOfRead_m){
    case READ_PD_FILENAME_LENGTH:
    {
        qtGraceDocStrNameLength_m = *(int*)(messagePtr_m);
        availableBytesFromSocket_m = 0;

        break;
    }
    case READ_PD_FILENAME:
    {
        qtGraceDocStrNamePtr_m = messagePtr_m;
        availableBytesFromSocket_m = 0;

        break;
    }
    case READ_DATA_LENGTH:
    {
        paramDataLength_m = *(int*)(messagePtr_m);
        availableBytesFromSocket_m = 0;
        break;
    }

    case READ_DATA:
    {

        dataFromQtGracePtr_m =  (QString)messagePtr_m;
        break;
    }
    default:

    {
        exit(0);
        break;
    }

    }

}


void CommunicationsProtocol::displayError(QAbstractSocket::SocketError socketError)
{
    /*switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("QtGrace Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("QtGrace Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the QtGrace is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("QtGrace Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(readSocketPtr_m->errorString()));
    }
*/
}




// Clear view and plot data.
void CommunicationsProtocol::cleanPlot()
{
    createNewDataStream();
    sendPlotInfo("@FLUSH\n");
    enablePlot();

}

void CommunicationsProtocol::enablePlot()
{
    sendPlotInfo("@ g0 on\n");
}

// Notify qtGrace that the current transmission is finished. (Command: 99)
void CommunicationsProtocol::endTransmission()
{
    int command = END_COMM;

    writeToDataStream((char *)&command, sizeof(command));
    emit startCommunicationToSend();
}

// Kill qtGrace, stop plotting. (Command: 42)
void CommunicationsProtocol::killChild()
{
    int command = KILL_CHILD; // kill

    createNewDataStream();

    writeToDataStream((char *)&command, sizeof(command));

    startCommunicationToSend();


}

// Setup plotting information in qtGrace.
void CommunicationsProtocol::sendPlotInfo(QString plot_info)
{

    if(plot_info=="")return;

    int command = PLOT_INFO;

    QByteArray ba = plot_info.toLatin1();
    char *c_str2 = ba.data();

    // calculate the length of data
    int len = plot_info.length();

    //  send the graph and set information to QtGrace
    writeToDataStream((char *)&command, sizeof(int));
    writeToDataStream((char *)&len, sizeof(int));
    writeToDataStream(c_str2, len);      // the info itself
}



// Tell qtGrace about plot scaling, all data within the scaling bounds are visibile. (Command: 6)
void CommunicationsProtocol::sendScalingModeInfo(int graphNo, int mode, double xStart_value, double xEnd_value)
{
    int command = SET_SCALING_MODE ;
    writeToDataStream((char *)&command, sizeof(int));
    writeToDataStream((char *)&graphNo, sizeof(int)); // current graph number
    writeToDataStream((char *)&mode, sizeof(int));

    if(mode == 2 || mode == 7 ){                   // if y-Autoscale mode send start and end data
        writeToDataStream((char*)&xStart_value, sizeof(double));
        writeToDataStream((char*)&xEnd_value, sizeof(double));
    }
}

// Tell qtGrace about plot layout. (Command: 8)
void CommunicationsProtocol::sendLayoutModeInfo(int graphNo, int layout, int col, int numGraphs)
{
    int command = SET_LAYOUT_MODE ;
    writeToDataStream((char *)&command, sizeof(int)); // command for QtGrace
    writeToDataStream((char *)&graphNo, sizeof(int)); // current graph number
    writeToDataStream((char *)&layout, sizeof(int));

    if(layout == 3){                  // if arrange mode send number of columns, LAYOUTARRANGE
        writeToDataStream((char*)&col, sizeof(int)); // number of columns
        writeToDataStream((char*)&numGraphs, sizeof(int)); // total number of graphs in the arrangement
    }

}

void CommunicationsProtocol::sendDataVector(int dataLength, double*xData,double*yData )
{
    // Initialize data mode, prerequired for data sending.
    int startCommand = WRITE_DATAVEC;
    writeToDataStream((char *)&startCommand, sizeof(startCommand));

    // Send the data to QtGrace
    writeToDataStream((char *)& dataLength, sizeof(int)); // data length
    writeToDataStream((char *)xData, dataLength * sizeof(double));   // the x-data
    writeToDataStream((char *)yData, dataLength * sizeof(double));   // the y-data

    int endCommand = WRITE_DATAVEC_FINISHED;
    writeToDataStream((char *)&endCommand, sizeof(endCommand));

    setRedrawMode();
}


// Initialize data reading, prerequired for data reading.
void CommunicationsProtocol::setReadingMode()
{
    emit startCommunicationToRead();
}

// Send postscript default file for plotting to file. (Command: 5)
void CommunicationsProtocol::sendPSFileName(QString file_name)
{
    int command = PS_FILENAME;

    QByteArray ba = file_name.toLatin1();
    char *c_str2 = ba.data();
    int len = file_name.length();

    writeToDataStream((char *)&command, sizeof(int));
    writeToDataStream((char *)&len, sizeof(int));
    writeToDataStream((char *)c_str2, len);

}

// Tell qtGrace to update view. (Command: 4)
void CommunicationsProtocol::setRedrawMode()
{
    int command = REDRAW;
    writeToDataStream((char *)&command, sizeof(command));

}

// Redraw and automatically write postscript file.
void CommunicationsProtocol::setRedrawAndWritePSMode()
{
    int command = REDRAW_AND_WRITEPS;
    writeToDataStream((char *)&command, sizeof(command));

}

void CommunicationsProtocol::createNewDataStream()
{
    plotDataPtr_m = new std::ostrstream();

}



void CommunicationsProtocol::writeToDataStream(const char *data, int len)
{
    assert(plotDataPtr_m);
    plotDataPtr_m->write(data,len);

}


void CommunicationsProtocol::processDataFromQtGrace(){

    std::istrstream dataStreamFromSocket(dataFromSocket_m.data(),dataFromSocket_m.size());

    printDebug("Total amount of data from stream"
               + QString::number(dataFromSocket_m.size()));


    while((int)dataStreamFromSocket.tellg() < dataFromSocket_m.size()){
        availableBytesFromSocket_m = 0;
        readPlotParametersFromQtGrace(&dataStreamFromSocket);
        countNoOfRead_m++;
        printDebug("Read data from stream " +
                   QString::number(dataStreamFromSocket.tellg()));


    }
}

void CommunicationsProtocol::waitToConnectedToServer(){

    while( comMode_m != endState ){
        // We need to wait until the TCP connection signal has been receive
        QApplication::instance()->processEvents();
        QThread::sleep(0.01);
    }

}

void CommunicationsProtocol::printDebug(QString message)
{
    if(isDebugEnabled_m)
        std::cerr << DEBUGDETAILS.toStdString()  << " \nmessage: "<< message.toStdString() << std::endl;

}
void CommunicationsProtocol::sendDataSetToQtGrace(double *x,double*y){

    if(sizeof(x) != sizeof(y)){
        //Error
        return;
    }

    int noOfPoints = sizeof(x);

    cleanPlot();
    sendDataVector(noOfPoints,x,y);
    endTransmission();

}

QString CommunicationsProtocol::readPlotParametersFromQtGrace(){

    startCommunicationToRead();
    return dataFromQtGracePtr_m;

}

