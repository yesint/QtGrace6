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

#include "Server.h"
#include "undo_module.h"

#ifdef _MSC_VER
#include <windows.h>
// Sleep()
#else
#include <unistd.h>
#endif
#include <QMessageBox>
#include <QtNetwork>
#include <stdlib.h>
#include <iostream>
#include <cassert>
#include <cstring>
#include <QDebug>
#include "files.h"
#include "KeyAndMousePressFilter.h"
#include <QTimer>
#include <QProgressDialog>
#include <QTextCodec>

extern void convert_all_strings_from_encoding_to_UTF8(void);
extern void convert_all_strings_from_UTF8_to_encoding(void);
//extern QTextCodec * FileCodec;
extern QList<QByteArray> avcod;

#ifdef __cplusplus
extern "C" {
#endif
extern void prepare_strings_for_saving(void);
extern void resume_strings_after_load_or_save(void);
#ifdef __cplusplus
}
#endif

extern char startupphase;
extern Device_entry *device_table;
int QtGraceTcpServer::dataLength_m = 0;
int QtGraceTcpServer::dataLength_m1 = 0;

QtGraceTcpServer::QtGraceTcpServer(QString readTcpPort,
                                   QString sendTcpPort,
                                   QObject *parent)
    :QObject(parent)
    ,setPlottingStatus_(true)
    ,isDebugFlagOn_m(false)
    ,messagePtr_m(NULL)
    ,dataSet1Ptr_m(NULL)
    ,dataSet2Ptr_m(NULL)
    ,command_m(SET_POLAR_PLOT)
    ,graphNo_m(0)
    ,xmin_m(0)
    ,xmax_m(0)
    ,conditionToExitFunction_m(0)
    ,countNoOfRead_m(0)
    ,newDataSetReady_m(1)
    ,isWriteToTmpFile_m(true)
    ,numGraphs_m(0)
    ,countNoOfDataSets_m(0)
    ,oldNoask_m(0)
    ,debugFilePtr_m(NULL)
    ,debugOutPtr_m(NULL)
    ,mode_m(DEFAULT_LAYOUT)
    ,xminPtr_m(NULL)
    ,xmaxPtr_m(NULL)
    ,columns_m(0)
    ,rows_m(0)
    ,qtGraceDocStrName_m("Untitled")
    ,dataFromBuffer_m(" ")
    ,bytesNeededFromSocket_m(0)
    ,writeTcpPortFromClient_m(readTcpPort)
    ,readTcpPortFromClient_m(sendTcpPort)
    ,readServerPtr_m(0)
    ,writeServerPtr_m(0)
    ,writeConnectionPtr_m(0)
    ,dataFromSocket_m(0)
    ,graphDataStreamToSendPtr_m(NULL)
    ,comMode_m(endComm)
    ,remainingDataSize_m(0)
    ,enableFooterText_m(false)
    ,footerText_m(NULL)
    ,timer_(NULL)
    ,isAutoUpdate_(false)
    ,isTCPErrorToStatusBar_(false)
    ,keyPressAndMousePressfilter_(NULL)
    ,allowUpdate_(true)
    ,receiveDataProgressDialog_(NULL)
    ,progressCounter_(0)
    ,allowExitQtGrace_(true)

{
    setbuf(stdout, NULL);

    if(getenv("QTGRACEDEBUG")) {
        isDebugFlagOn_m = true;
    }

    keyPressAndMousePressfilter_ = new KeyAndMousePressFilter();


    if(isDebugFlagOn_m){
        debugFilePtr_m = new QFile("QTGRACEDEBUG.txt");

        QFileInfo info1(debugFilePtr_m->fileName());
        qDebug()<<"QTGRACEDEBUG flag detected and writes to: "+ info1.absoluteFilePath() +"\n";
        debugFilePtr_m->open(QIODevice::WriteOnly | QIODevice::Text);
        debugOutPtr_m = new QTextStream(debugFilePtr_m);

        writeToDebugFile("*******************************************************************","",true);
        writeToDebugFile("DEBUG MODE ENABLED","",true);
        writeToDebugFile("Host: " +  (QString)bi_system()+"\n","",true);
        writeToDebugFile("Time: " +  (QString)bi_date() + "," + __TIME__,"",true);
        writeToDebugFile("GUI toolkit: " +  (QString)bi_gui(),"",true);
        writeToDebugFile("Qt version: " +  (QString)QT_VERSION_STR,"",true);
        writeToDebugFile("\nTCP write server port from client: " + writeTcpPortFromClient_m,"",true);
        writeToDebugFile("TCP read server port from client: " + readTcpPortFromClient_m,"",true);
        writeToDebugFile("*******************************************************************\n","",true);

    }


    //WHY???
    for(int i=0;i<10;i++){
        saveCountNoOfDataSets_m.append(0);
    }

    //Initialize read and write servers

    readServerPtr_m = new QTcpServer(this);

    if (!readServerPtr_m->listen( QHostAddress("127.0.0.1"),readTcpPortFromClient_m.toInt())) {//Listen on LocalHost
        writeToDebugFile("Unable to start the write server: "
                         + readServerPtr_m->errorString(),DEBUGDETAILS);
        return;
    }else{
        writeToDebugFile("The read server is started to listen"
                         " on address: " + readServerPtr_m->serverAddress().toString()
                         + " and port: " + QString::number(readServerPtr_m->serverPort()),DEBUGDETAILS);
    }

    writeServerPtr_m = new QTcpServer(this);

    if (!writeServerPtr_m->listen( QHostAddress("127.0.0.1"),writeTcpPortFromClient_m.toInt())) {//Listen on LocalHost
        writeToDebugFile("Unable to start the write server: "+ writeServerPtr_m->errorString(),DEBUGDETAILS);
        return;
    }else{
        writeToDebugFile("The write server is started to listen"
                         " on address: " + writeServerPtr_m->serverAddress().toString()
                         + " and port: " + QString::number(writeServerPtr_m->serverPort()),DEBUGDETAILS);
    }


    //Read and write from/to Client
    connect(readServerPtr_m, SIGNAL(newConnection()), this, SLOT(initReadServer()));
    connect(writeServerPtr_m, SIGNAL(newConnection()), this, SLOT(initWriteServer()));

    //Buffer to save data from socket
    buffer_m.setBuffer(&dataFromBuffer_m);
    buffer_m.open(QIODevice::Append);

    writeToDebugFile("Done constructor\n",DEBUGDETAILS);

}



void QtGraceTcpServer::initReadServer()
{


    // Accept connection
    QTcpSocket *clientConnection = readServerPtr_m->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));

    connect(clientConnection, SIGNAL(readyRead()), this, SLOT(communicateWithClient()));
    connect(clientConnection, SIGNAL(disconnected()), this, SLOT(readSocketDisconnected()));
    connect(clientConnection, SIGNAL(connected()), this, SLOT(readSocketConnected()));

    writeToDebugFile("Read Server: connected\n",DEBUGDETAILS);
}

void QtGraceTcpServer::initWriteServer()
{
    // Accept connection
    writeConnectionPtr_m = writeServerPtr_m->nextPendingConnection();
    connect(writeConnectionPtr_m, SIGNAL(disconnected()),
            writeConnectionPtr_m, SLOT(deleteLater()));
    connect(writeConnectionPtr_m, SIGNAL(bytesWritten(qint64)), this, SLOT(dataWritten(qint64)));
    connect(writeConnectionPtr_m, SIGNAL(disconnected()), this, SLOT(writeSocketDisconnected()));
    connect(writeConnectionPtr_m, SIGNAL(connected()), this, SLOT(writeSocketConnected()));
    connect(writeConnectionPtr_m, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));

    writeToDebugFile("Write Server: connected\n",DEBUGDETAILS);
}


void QtGraceTcpServer::communicateWithClient()
{



    QTcpSocket *readConnection = dynamic_cast<QTcpSocket*>(sender());
    assert(readConnection);

    writeToDebugFile("Communicate with client",DEBUGDETAILS);

    do {
        if(comMode_m != readDataComm){
            writeToDebugFile("Server going to read command",DEBUGDETAILS);

            int rc = readConnection->read(reinterpret_cast<char*>(&comMode_m), sizeof(comMode_m));
            assert(rc == sizeof(comMode_m));

            writeToDebugFile("Server read: " +
                             QString::number(rc) +
                             " bytes, with command: "
                             + QString::number(comMode_m),DEBUGDETAILS);

        }
        else {
            assert(remainingDataSize_m>0);
        }

        if( comMode_m == initComm ){

            writeToDebugFile("Server send ready signal",DEBUGDETAILS);
            int rc = writeConnectionPtr_m->write(reinterpret_cast<char*>(&comMode_m), sizeof(ComMode));
            assert(rc == sizeof(ComMode));
            writeConnectionPtr_m->flush();
        }
        else if( comMode_m == readDataComm ){
            writeToDebugFile("Server ready to read data from client",DEBUGDETAILS);


            readData(readConnection);
            QTextStream out(stdout);


        }
        else if( comMode_m == sendDataCom ){
            writeToDebugFile("Server send data to client",DEBUGDETAILS);

            allowExitQtGrace_ = false;
            allowUpdate_ = false;

            sendParam();

            sendData(graphDataStreamToSendPtr_m->str().c_str(),(int)graphDataStreamToSendPtr_m->str().length());
            allowUpdate_ = true;
            delete graphDataStreamToSendPtr_m;


        }
        else {
            assert(false);
        }
    } while(readConnection->bytesAvailable() > 0);
    // Note, that we need this while loop because the readyRead signal is NOT send recursively!
    // Meaning that if new data arrives while we are still in ReadyRead there will not be a signal submitted
    // thus we need to check if data is avaiable.

}


void QtGraceTcpServer::writeToDataStream(const char *data, int len)
{
    graphDataStreamToSendPtr_m->write(data,len);
}



void QtGraceTcpServer::sendData(const char* data, int bytesToSend)
{
    writeToDebugFile("Server is sending data to client",DEBUGDETAILS);

    // First we need to tell the client what we are going to do.
    comMode_m = sendDataCom;
    int rc = writeConnectionPtr_m->write(reinterpret_cast<char*>(&comMode_m), sizeof(comMode_m));
    assert(rc == sizeof(comMode_m));

    writeToDebugFile("Server is going to send packet with " +
                     QString::number(bytesToSend) +
                     " bytes",DEBUGDETAILS);

    rc = writeConnectionPtr_m->write(reinterpret_cast<char*>(&bytesToSend), sizeof(int));
    assert(rc == sizeof(int));

    writeToDebugFile("Server has send data packet",DEBUGDETAILS);

    int bytesWritten = 0;
    do {
        rc = writeConnectionPtr_m->write(reinterpret_cast<const char*>(&data[bytesWritten]), bytesToSend-bytesWritten);
        if( rc >= 0) bytesWritten += rc;
    }while( rc >=0 && bytesWritten < bytesToSend);

    // Make sure data has been send before we delete memory.
    writeConnectionPtr_m->flush();

    writeToDebugFile("Server is done sending, send end command",DEBUGDETAILS);

    comMode_m = endComm;
    rc = writeConnectionPtr_m->write(reinterpret_cast<char*>(&comMode_m), sizeof(ComMode));
    assert(rc == sizeof(ComMode));
    writeConnectionPtr_m->flush();
    //XXX NIMAL SET A FLAG TO SAFE TO CLOSE QTGRACE

    allowExitQtGrace_ = true;
    // fprintf(stderr, "Done sending data");
    // fflush(stderr);
}

void QtGraceTcpServer::readData(QTcpSocket *readConnection)
{
    if(setPlottingStatus_){
        // QTextStream out(stdout);
        //  out << QObject::tr("readFromClient").toLocal8Bit().constData() << endl;
        set_footer_background_color("QtGrace is Plotting... "
                                    "While plotting you will not be able to use ViewBeast and QtGrace."
                                    ,0);

        //if(receiveDataProgressDialog_== NULL){
        //    createProgressDialog();
        qApp->installEventFilter(keyPressAndMousePressfilter_);
        //   receiveDataProgressDialog_->setValue(10);
        //   allowUpdate_ = false;

        // }else{
        //     receiveDataProgressDialog_->setValue(progressCounter_);
        //     progressCounter_=progressCounter_+1;

        // }

        setPlottingStatus_ = false;
    }
    assert(remainingDataSize_m>=0);

    int bytesToRead = remainingDataSize_m;
    int rc = 0;

    writeToDebugFile("Server is reading data from client",DEBUGDETAILS);

    // If remaining data size is 0 then we need to read total data size from the client.
    if( remainingDataSize_m == 0 ){
        rc = readConnection->read(reinterpret_cast<char*>(&bytesToRead), sizeof(int));
    }

    char* data = new char[bytesToRead];

    writeToDebugFile("Server is going to read packet with " +
                     QString::number(bytesToRead) +
                     " bytes",DEBUGDETAILS);

    int bytesRead = 0;

    while(bytesToRead > 0 && readConnection->bytesAvailable() > 0){
        int packetSize = readConnection->read(&data[bytesRead], bytesToRead);
        assert(packetSize >= 0 && packetSize <= bytesToRead);

        writeToDebugFile("Server has read packet with " +
                         QString::number(packetSize) +
                         " bytes",DEBUGDETAILS);

        dataFromSocket_m.append(data,packetSize);

        bytesRead += packetSize;
        bytesToRead -= packetSize;
    }

    delete [] data;

    if( bytesToRead == 0){
        writeToDebugFile("Server is done reading, send end command",DEBUGDETAILS);
        allowUpdate_ = true;

        processDataFromClient();
        dataFromSocket_m.clear();
        comMode_m = endComm;
        rc = writeConnectionPtr_m->write(reinterpret_cast<char*>(&comMode_m), sizeof(ComMode));
        assert(rc == sizeof(ComMode));
        writeConnectionPtr_m->flush();


    }

    // Note, there might be remaining bytes to read, if so, there will be another readyRead signal send.
    // Therefore we store the amount of data that we still need to read and read it with the next signal.
    remainingDataSize_m = bytesToRead;

}


QtGraceTcpServer::~QtGraceTcpServer() {

    writeToDebugFile("Server deletion",DEBUGDETAILS);


    if(isDebugFlagOn_m){
        debugFilePtr_m->close();
        delete debugFilePtr_m;
        delete debugOutPtr_m;
    }

}

void QtGraceTcpServer::processDataFromClient(){


    char* dataFromSocketCopy_m;

    dataFromSocketCopy_m = copyDataFromSocket(dataFromSocket_m.size(),dataFromSocket_m.data());
    string dataFromSocketString = string (dataFromSocketCopy_m,dataFromSocket_m.size());

    std::istringstream dataStreamFromSocket(dataFromSocketString);

    // For debug
    dataStreamFromSocket.seekg (0, ios::end);

    int length = dataStreamFromSocket.tellg();
    dataStreamFromSocket.seekg (0, ios::beg);

    writeToDebugFile("Total amount of data from socket "
                     + QString::number(dataFromSocket_m.size()),DEBUGDETAILS);

    writeToDebugFile("Length of stream " + QString::number(length),DEBUGDETAILS);


    while((int)dataStreamFromSocket.tellg() < dataFromSocket_m.size()){
        bytesNeededFromSocket_m = 0;
        countNoOfRead_m++;
        readFromClient(&dataStreamFromSocket);

        writeToDebugFile("Read data from stream " + QString::number(dataStreamFromSocket.tellg()),DEBUGDETAILS);

    }


}


void QtGraceTcpServer::readFromClient( std::istringstream *dataFromClient) {

    //Execute task from client





    conditionToExitFunction_m = 0;

    //Specifiy the amount of bytes to be read

    if(countNoOfRead_m==1 ||
            countNoOfRead_m==2 ||
            (command_m == SET_SCALING_MODE && countNoOfRead_m==3) ||
            command_m == SET_LAYOUT_MODE){
        bytesNeededFromSocket_m=(int)sizeof(quint32);

    }else if (command_m==WRITE_DATAVEC){
        bytesNeededFromSocket_m= QtGraceTcpServer::dataLength_m*8;

    }else if(command_m == SET_SCALING_MODE &&
             (mode_m==AUTOSCALE_Y_AXIS_OR_OVERLAY ||
              mode_m==AUTOSCALE_X_AXIS_OR_OVERLAY)){
        bytesNeededFromSocket_m=(int)sizeof(double);

    }else{
        bytesNeededFromSocket_m= QtGraceTcpServer::dataLength_m;
    }

    // ensure that
    // 0-terminated character strings
    // come here correctly.
    messagePtr_m=new char[bytesNeededFromSocket_m+1];
    messagePtr_m[bytesNeededFromSocket_m]=0;
    writeToDebugFile("Before Read data from stream " + QString::number(dataFromClient->tellg()),DEBUGDETAILS);

    // Read all data from socket
    dataFromClient->read(messagePtr_m,bytesNeededFromSocket_m);

    if (dataFromClient){
        writeToDebugFile("All characters read successfully.",DEBUGDETAILS);
    }
    else{
        writeToDebugFile("error: only " + QString::number(dataFromClient->gcount()) + " could be read",DEBUGDETAILS);
    }

    // Save data from socket
    saveDataFromSocket(countNoOfRead_m);

    //dataFromClient->close();
    delete[] messagePtr_m;

    if (conditionToExitFunction_m) {
        return;
    }


    writeToDebugFile("Executing command no :" +  QString::number(command_m) +"\n",DEBUGDETAILS);
    executeTaskFromClient();
    writeToDebugFile("Command was performed :" +  QString::number(command_m) +"\n",DEBUGDETAILS);

}


void QtGraceTcpServer::executeTaskFromClient()
{
    switch (command_m){

    case PLOT_INFO://Read PLOT_INFO(1)
    {





        writeToDebugFile("Command was performed:  (PLOT_INFO) "
                         + QString::number(command_m),DEBUGDETAILS);


        buffer_m.write(dataSet1Ptr_m);
        isWriteToTmpFile_m=true;
        countNoOfRead_m = 0;
        break;
    }

    case WRITE_DATAVEC://WRITE_DATAVEC(2)

    {


        writeToDebugFile("Command was performed:  (WRITE_DATAVEC) "
                         + QString::number(command_m),DEBUGDETAILS);


        //readXYData(dataSet1Ptr_m, dataSet2Ptr_m);
        double *x = NULL;
        double *y = NULL;

        x = (double *) dataSet1Ptr_m;
        y = (double *) dataSet2Ptr_m;

        if (newDataSetReady_m)
            buffer_m.write("@TYPE xy");

        writeToDebugFile("data length ("
                         + QString::number(QtGraceTcpServer::dataLength_m)
                         +") ",DEBUGDETAILS);

        if(QtGraceTcpServer::dataLength_m!=QtGraceTcpServer::dataLength_m1){
            writeToDebugFile(" ERROR dataLength_m = " +
                             QString::number(QtGraceTcpServer::dataLength_m),DEBUGDETAILS);

            writeToDebugFile("ERROR dataLength_m1 = " +
                             QString::number(QtGraceTcpServer::dataLength_m1),DEBUGDETAILS);

        }
        int tmp=QtGraceTcpServer::dataLength_m;
        for(int i=0; i<tmp; i++){
            char xp[100];
            sprintf(xp,"\n \t %1.16g \t %1.16g",x[i],y[i]);


            buffer_m.write(xp);
        }


        writeToDebugFile("end loop  = " +
                         QString::number(QtGraceTcpServer::dataLength_m1),DEBUGDETAILS);


        countNoOfRead_m = 0;
        newDataSetReady_m=0;





        break;
    }
    case WRITE_DATAVEC_FINISHED:

    {
        /*if(progressCounter_<8000){
            progressCounter_= progressCounter_+10;
        }*/

        writeToDebugFile("Command was performed:  (WRITE_DATAVEC_FINISHED) "
                         + QString::number(command_m),DEBUGDETAILS);

        countNoOfRead_m = 0;


        buffer_m.write("\n");

        newDataSetReady_m=1;
        countNoOfDataSets_m++;

        break;
    }

    case SET_POLAR_PLOT:
    {


        writeToDebugFile("Command was performed:  (SET_POLAR_PLOT) "
                         + QString::number(command_m),DEBUGDETAILS);
        startupphase=1;
        disableConsole = 1;
        oldNoask_m=noask;
        noask=true; // prevent questions


        //Plot for all graphs
        for(int igno = 0; igno < graphNo_m+1; igno++){
            //second: set polar plot v2
            set_graph_type(igno,GRAPH_POLAR2);
            //third: set plot options (here arbitrary values: phi0=0.9 and roffset=12%)
            set_parameters_polar_v2(igno,M_PI/2,0.12);
            //fourth: autoscale
            autoscale_graph(igno,AUTOSCALE_XY);
            //fifth: set nice graph settings
            make_nice_polar_plot_v2(igno);

            quickSetAxisFormat(igno,X_AXIS,QUICK_AXIS_TRANSFORM_DEGREES);


        }
        noask=oldNoask_m;
        disableConsole = 0;

        startupphase=0;

        countNoOfDataSets_m = 0;
        countNoOfRead_m = 0;
        isWriteToTmpFile_m=false;


        break;
    }

    case SET_QTGRACE_FOOTER:
    {

        writeToDebugFile("Command was performed:  (SET_QTGRACE_FOOTER) "
                         + QString::number(command_m),DEBUGDETAILS);
        startupphase=1;
        enableFooterText_m = true;
        readFooterText();
        startupphase=0;

        countNoOfDataSets_m = 0;
        countNoOfRead_m = 0;
        isWriteToTmpFile_m=false;


        break;
    }

    case START_AUTO_UPDATE:
    {

        /*  writeToDebugFile("Command was performed:  (START_AUTO_UPDATE) "
                         + QString::number(command_m),DEBUGDETAILS);
        startupphase=1;


        isAutoUpdate_ = true;
        timer_->start(5000);
        fprintf(stderr, "start update");

        startupphase=0;

        countNoOfDataSets_m = 0;
        countNoOfRead_m = 0;

        isWriteToTmpFile_m=false;
*/
        break;
    }

    case STOP_AUTO_UPDATE:
    {

        writeToDebugFile("Command was performed:  (STOP_AUTO_UPDATE) "
                         + QString::number(command_m),DEBUGDETAILS);
        /* startupphase=1;

        isAutoUpdate_ = false;
        timer_->stop();

        startupphase=0;

        countNoOfDataSets_m = 0;
        countNoOfRead_m = 0;
        isWriteToTmpFile_m=false;*/
        break;
    }



    case REDRAW:
    {


        writeToDebugFile("Command was performed:  (REDRAW) "
                         + QString::number(command_m),DEBUGDETAILS);
        startupphase=1;
        oldNoask_m=noask;
        noask=true; // prevent questions
        disableConsole = 1;

        writeDataToTmpFile();
        doPlotFit();



        set_graph_selectors(0); // In order to update QtGrace plot window (there is always one graph, therefore 0).

        setScalingMode();
        noask=oldNoask_m;
        startupphase=0;
        disableConsole = 0;




        //Update legend properties
        for(int igno = 0; igno < graphNo_m+1; igno++){
            for(int iSetNo = 0; iSetNo < saveCountNoOfDataSets_m.at(igno); iSetNo++){
                set_legend_string(igno,iSetNo,get_legend_string(igno,iSetNo));

                char* gotComment=getcomment(igno, iSetNo);

                string strComments;
                if(gotComment) strComments=gotComment;

                if(strComments.empty())
                {
                    setcomment(igno,iSetNo,getcomment(igno, iSetNo));
                } else{
                    setcomment(igno,iSetNo,get_legend_string(igno,iSetNo));

                }
            }
        }

        countNoOfDataSets_m = 0;
        countNoOfRead_m = 0;
        isWriteToTmpFile_m=false;



        break;
    }
    case PS_FILENAME:
    {

        writeToDebugFile("Command was performed:  (PS_FILENAME) "
                         + QString::number(command_m),DEBUGDETAILS);

        readPsFileName();
        /* if(progressCounter_<8000){
            progressCounter_= progressCounter_+10;
        }*/
        //   receiveDataProgressDialog_->setValue(progressCounter_);

        countNoOfRead_m = 0;
        break;
    }
    case SET_SCALING_MODE:
    {

        writeToDebugFile("Command was performed:  (SET_SCALING_MODE) "
                         + QString::number(command_m),DEBUGDETAILS);
        startupphase=1;
        oldNoask_m=noask;
        noask=true; // prevent questions
        disableConsole = 1;

        writeDataToTmpFile();

        setScalingMode();
        noask=oldNoask_m;
        startupphase=0;
        disableConsole = 0;


        countNoOfRead_m = 0;
        isWriteToTmpFile_m=false;
        break;
    }

    case REDRAW_AND_WRITEPS://REDRAW_AND_WRITEPS(7)
    {
        writeToDebugFile("Command was performed:  (REDRAW_AND_WRITEPS) "
                         + QString::number(command_m),DEBUGDETAILS);

        /* force a hardcopy */
        // set_pagelayout(PAGE_FREE);
        // update_all();

        oldNoask_m=noask;
        noask=true; // prevent questions
        disableConsole = 1;

        int w=1080,h=800;
        update_all();

        xdrawgraph();
        doPlotFit();

        do_hardcopy_external(get_exportname(),device_table[hdevice].name,start_dpi,w,h);

        noask=oldNoask_m;
        disableConsole = 0;

        countNoOfRead_m = 0;
        break;
    }
    case SET_LAYOUT_MODE:
    {

        writeToDebugFile("Command was performed:  (SET_LAYOUT_MODE) "
                         + QString::number(command_m),DEBUGDETAILS);
        startupphase=1;
        setLayoutMode();

        startupphase=0;
        countNoOfRead_m = 0;
        break;
    }

    case KILL_CHILD:
    {

        writeToDebugFile("Command was performed:  (KILL_CHILD) "
                         + QString::number(command_m),DEBUGDETAILS);

        if(isDebugFlagOn_m){
            debugFilePtr_m->close();
            delete debugFilePtr_m;
            delete debugOutPtr_m;
        }

        exit(0);
        countNoOfRead_m = 0;
        break;
    }
    case END_COMM:
    {

        /*  if(progressCounter_<8000){
            progressCounter_= progressCounter_+50;
        }*/
        // receiveDataProgressDialog_->setValue(progressCounter_);
        writeToDebugFile("Command was performed:  (END_COMM) "
                         + QString::number(command_m),DEBUGDETAILS);
        update_all();
        /* if(progressCounter_<8000){
            progressCounter_= progressCounter_+60;
        }*/
        //  receiveDataProgressDialog_->setValue(progressCounter_);
        startupphase=1;

        xdrawgraph();

        /* if(progressCounter_<8000){
            progressCounter_= progressCounter_+70;
        }*/
        // receiveDataProgressDialog_->setValue(progressCounter_);
        doPlotFit();
        /* if(progressCounter_<8000){
            progressCounter_= progressCounter_+80;
        }*/
        // receiveDataProgressDialog_->setValue(progressCounter_);
        if(enableFooterText_m){
            //set_left_footer(footerText_m);
            if(!isTCPErrorToStatusBar_){
                set_footer_background_color(footerText_m,0);
            }

            delete[]footerText_m;
            isTCPErrorToStatusBar_ = true;
            enableFooterText_m = false;
        }else{
            set_footer_background_color("done",2);

            QTimer::singleShot(4000, this, SLOT(startAutoUpdate()));
        }


        setPlottingStatus_ = true;

        qApp->removeEventFilter(keyPressAndMousePressfilter_);


        startupphase=0;
        isWriteToTmpFile_m=true;
        countNoOfRead_m = 0;
        QTextStream out(stdout);
        //     convert_all_strings_from_encoding_to_UTF8();
        out << QObject::tr("Done sending data").toLocal8Bit().constData();// << endl;

        convert_all_strings_from_UTF8_to_encoding();

        /* if(receiveDataProgressDialog_!=NULL){
            receiveDataProgressDialog_->setValue(8000);
            progressCounter_ = 0;
            delete receiveDataProgressDialog_;
            receiveDataProgressDialog_ = NULL;
        }
*/


        break;
    }
    case TEST_CONNECTION: //Not in use anymore
    {
        writeToDebugFile("Command was performed:  (TEST_CONNECTION) "
                         + QString::number(command_m),DEBUGDETAILS);

        countNoOfRead_m = 0;
        break;
    }

    default:
    {
        qApp->removeEventFilter(keyPressAndMousePressfilter_);
        // QMessageBox::information(0,"Communication Error4",tr("The following error occurred"));

        writeToDebugFile("Command was performed:  (default) "
                         + QString::number(command_m),DEBUGDETAILS);

        writeToDebugFile("Communication error: try to restart",DEBUGDETAILS);
        // QMessageBox::information(0,"Communication Error","Communication error: try to restart");

        //QMessageBox::information(0,"(1) Communication Error.", "Please close QtGrace and plot again.");

        if(!isTCPErrorToStatusBar_){
            set_footer_background_color("WARNING 1: The connection to ViewBeast has been aborted. "
                                        "Plot again, in ViewBeast, to start a new instance of "
                                        "QtGrace.",1);
        }

        isTCPErrorToStatusBar_ = true;

        //exit(0);
        countNoOfRead_m = 0;
        break;
    }
    }
}

void QtGraceTcpServer::startAutoUpdate(){
#if QT_VERSION >= 0x050100
    if(QGuiApplication::applicationState() == Qt::ApplicationActive){
        QTextStream out(stdout);
        out << QObject::tr("start update").toLocal8Bit().constData();// << endl;
    }
#endif
}

char* QtGraceTcpServer::copyDataFromSocket(int availableBytes, char* dataFromSocket){

    char* data;
    data = new char[availableBytes+1];
    memcpy( data , dataFromSocket, availableBytes+1);
    return data;

}

void QtGraceTcpServer::getCommandFromClient(int commandFromsocket)
{
    switch (commandFromsocket) {
    case 1:
        command_m = PLOT_INFO ;
        break;
    case 2:
        command_m = WRITE_DATAVEC ;
        break;
    case 12:
        command_m =  WRITE_DATAVEC_FINISHED;
        break;
    case 3:
        command_m =  SET_POLAR_PLOT;
        break;
    case 4:
        command_m =  REDRAW;
        break;
    case 5:
        command_m =  PS_FILENAME;
        break;
    case 6:
        command_m =  SET_SCALING_MODE;
        break;
    case 7:
        command_m =  REDRAW_AND_WRITEPS;
        break;
    case 8:
        command_m = SET_LAYOUT_MODE;
        break;
    case 42:
        command_m = KILL_CHILD;
        break;
    case 99:
        command_m = END_COMM;
        break;
    case 98:
        command_m = TEST_CONNECTION;
        break;
    case 9:
        command_m = SET_QTGRACE_FOOTER;
        break;
    case 10:
        command_m = START_AUTO_UPDATE;
        break;
    case 11:
        command_m = STOP_AUTO_UPDATE;
        break;
    default:{

        qApp->removeEventFilter(keyPressAndMousePressfilter_);
        //   QMessageBox::information(0,"Communication Error3",tr("The following error occurred"));

        writeToDebugFile("Communication error: Command ("
                         + QString::number(commandFromsocket)
                         +") notfound",DEBUGDETAILS);

        //QMessageBox::information(0,"Communication Error","Communication error: Command not found");
        //QMessageBox::information(0,"(2)Communication Error.", "Please close QtGrace and plot again.");


        if(!isTCPErrorToStatusBar_){
            set_footer_background_color("WARNING 2: The connection to ViewBeast has been aborted. "
                                        "Plot again, in ViewBeast, to start a new instance of "
                                        "QtGrace.",1);
        }


        isTCPErrorToStatusBar_ = true;


        //exit(0);
        break;
    }
    }


}

void QtGraceTcpServer::readDataFromSocket(char *newDataFromSocket, int availableBytes, readCommands readMode){
    switch (readMode){

    case START_READ:
    {
        int commandFromsocket = *((int*)(newDataFromSocket));

        getCommandFromClient(commandFromsocket);

        writeToDebugFile(" The command is int, 4 bytes are " +
                         QString::number((int)(newDataFromSocket[0])) + " " +
                QString::number((int)(newDataFromSocket[1])) + " " +
                QString::number((int)(newDataFromSocket[2])) + " " +
                QString::number((int)(newDataFromSocket[3])) + " "
                ,DEBUGDETAILS);



        break;
    }

    case READ_DATALENGTH: //Read data length
    {


        QtGraceTcpServer::dataLength_m = *((int*)(newDataFromSocket));
        QtGraceTcpServer::dataLength_m1 = *((int*)(newDataFromSocket));
        writeToDebugFile(" Got data length = " +
                         QString::number(QtGraceTcpServer::dataLength_m),DEBUGDETAILS);



        break;
    }

    case READ_DATASET_1:

    {
        if(command_m!=SET_SCALING_MODE && command_m!=SET_LAYOUT_MODE){
            dataSet1Ptr_m = copyDataFromSocket(availableBytes,newDataFromSocket);
        }
        else{

            int modeFromClient = *((int*)(newDataFromSocket));

            switch (modeFromClient) {
            case 0:
                mode_m = DEFAULT_LAYOUT;
                break;
            case 1:
                mode_m = AUTOSCALE_ALL_AXES_OR_JOIN_PLOT;
                break;
            case 2:
                mode_m = AUTOSCALE_Y_AXIS_OR_OVERLAY;
                break;
            case 3:
                mode_m = GRAPH_POSITION;
                break;
            case 7:
                mode_m = AUTOSCALE_X_AXIS_OR_OVERLAY;
            default:
                break;
            }

            writeToDebugFile(" Got mode = " +
                             QString::number(mode_m),DEBUGDETAILS);

        }
        break;
    }

    case READ_PLOT_SETTINGS_1_FROM_CLIENT: //Read Plot settings from client

    {
        if(command_m == SET_SCALING_MODE){ //Min x-axis length
            xminPtr_m = (double *)newDataFromSocket;
            xmin_m = xminPtr_m[0];
            writeToDebugFile("Read Plot settings 1. Got SET_SCALING_MODE command",DEBUGDETAILS);

        }
        else if(command_m == SET_LAYOUT_MODE){ // Numbers of columns
            columns_m = *((int*)(newDataFromSocket));
            writeToDebugFile("Read Plot settings 1. Got SET_LAYOUT_MODE command",DEBUGDETAILS);

        }
        else{
            dataSet2Ptr_m = copyDataFromSocket(availableBytes,newDataFromSocket);
        }

        writeToDebugFile("Read Plot settings 1 from. Got command = " +
                         QString::number(command_m),DEBUGDETAILS);

        break;
    }

    case READ_PLOT_SETTINGS_2_FROM_CLIENT://Read Plot settings from client dialogue
    {
        if(command_m == SET_SCALING_MODE){  //Max x-axis length
            xmaxPtr_m = (double *)newDataFromSocket;
            xmax_m = xmaxPtr_m[0];
            writeToDebugFile("Read Plot settings 2. Got SET_SCALING_MODE command",DEBUGDETAILS);
        }
        else if(command_m == SET_LAYOUT_MODE) //numbers of graphs
        {
            numGraphs_m = *((int*)(newDataFromSocket));
            writeToDebugFile("Read Plot settings 2. Got SET_LAYOUT_MODE command",DEBUGDETAILS);
        }


        break;
    }
    default:
    {
        qApp->removeEventFilter(keyPressAndMousePressfilter_);
        //  QMessageBox::information(0,"Communication Error2",tr("The following error occurred"));
        writeToDebugFile("Communication error: Command not found",DEBUGDETAILS);
        //        QMessageBox::information(0,"Communication Error","Communication error: Command not found");
        //QMessageBox::information(0,"(3)Communication Error.", "Please close QtGrace and plot again.");
        if(!isTCPErrorToStatusBar_){
            set_footer_background_color("WARNING 3: The connection to ViewBeast has been aborted. "
                                        "Plot again, in ViewBeast, to start a new instance of "
                                        "QtGrace.",1);
        }
        isTCPErrorToStatusBar_ = true;

        //exit(0);
        break;
    }
    }
}

void QtGraceTcpServer::saveDataFromSocket(int numberOfRead){

    switch (numberOfRead){

    case 1:
    {
        writeToDebugFile("Analysing(3) mode = "+QString::number(mode_m),DEBUGDETAILS);

        readDataFromSocket(messagePtr_m,bytesNeededFromSocket_m,  START_READ);


        if( command_m == START_AUTO_UPDATE ||
                command_m == STOP_AUTO_UPDATE || command_m == SET_POLAR_PLOT ||
                command_m == REDRAW_AND_WRITEPS || command_m == KILL_CHILD ||
                command_m == END_COMM || command_m == TEST_CONNECTION ||
                command_m == WRITE_DATAVEC_FINISHED || command_m == REDRAW )

            conditionToExitFunction_m = 0;
        else
            conditionToExitFunction_m = 1;

        break;
    }
    case 2:
    {
        writeToDebugFile("Analysing(3) mode = "+QString::number(mode_m),DEBUGDETAILS);

        readDataFromSocket(messagePtr_m,bytesNeededFromSocket_m, READ_DATALENGTH);
        conditionToExitFunction_m = 1;
        break;
    }
    case 3:
    {
        writeToDebugFile("Analysing(3) mode = "+QString::number(mode_m),DEBUGDETAILS);

        readDataFromSocket(messagePtr_m,bytesNeededFromSocket_m, READ_DATASET_1);

        if((command_m == SET_SCALING_MODE && (mode_m==AUTOSCALE_Y_AXIS_OR_OVERLAY || mode_m==AUTOSCALE_X_AXIS_OR_OVERLAY))||
                (command_m == SET_LAYOUT_MODE && mode_m == GRAPH_POSITION) ||
                command_m == WRITE_DATAVEC)

            conditionToExitFunction_m = 1;
        break;
    }

    case 4:
    {
        writeToDebugFile("Analysing(4) mode = "+QString::number(mode_m),DEBUGDETAILS);

        readDataFromSocket(messagePtr_m,bytesNeededFromSocket_m, READ_PLOT_SETTINGS_1_FROM_CLIENT);

        if((command_m == SET_SCALING_MODE && (mode_m==AUTOSCALE_Y_AXIS_OR_OVERLAY || mode_m==AUTOSCALE_X_AXIS_OR_OVERLAY))||
                (command_m == SET_LAYOUT_MODE && mode_m == GRAPH_POSITION))

            conditionToExitFunction_m = 1;

        break;
    }
    case 5:
    {
        writeToDebugFile("Analysing(5) mode = "+QString::number(mode_m),DEBUGDETAILS);

        readDataFromSocket(messagePtr_m,bytesNeededFromSocket_m, READ_PLOT_SETTINGS_2_FROM_CLIENT);

        break;
    }

    default:
    {
        writeToDebugFile("Analysing(default exit function) = "+QString::number(mode_m),DEBUGDETAILS);

        conditionToExitFunction_m = 1;
        break;
    }
    }

}

void QtGraceTcpServer::readXYData(char* xData, char* yData){

    double *x = NULL;
    double *y = NULL;
    QString xValueStr = "i";
    QString yValueStr = "i";
    QByteArray xValueBa("i");
    QByteArray yValueBa("i");
    char *xValueChar = NULL;
    char *yValueChar = NULL;

    x = (double *) xData;
    y = (double *) yData;

    if (newDataSetReady_m)
        buffer_m.write("@TYPE xy");

    //Check for invalid data and replace with 0;

    for(int i=0; i<QtGraceTcpServer::dataLength_m; i++){


        /*if(progressCounter_<8000){
            progressCounter_= progressCounter_+1;
        }*/

        if(fabs(x[i])>1e200 || (x[i]!=0 && fabs(x[i])<1e-200))  {
            cout<<"Invalid numeric data in x["<<i<<"]="<<x[i]<<endl;
            // exit(0);
            x[i]=0;
        }
        if(fabs(y[i])>1e200  || (y[i]!=0 && fabs(y[i])<1e-200))   {
            cout<<"Invalid numeric data in y["<<i<<"]="<<y[i]<<endl;
            // exit(0);
            y[i]=0;
        }

        if(qIsNaN(x[i]))
            x[i]=0;

        if(qIsNaN(y[i]))
            y[i]=0;

        //Save x and y data to data buffer

        //double to string
        xValueStr = QString::number(x[i],'g',16);
        //String to char
        xValueBa = xValueStr.toLocal8Bit();
        xValueChar = xValueBa.data();

        buffer_m.write("\n \t");
        buffer_m.write(xValueChar);

        buffer_m.write("\t");

        yValueStr = QString::number(y[i],'g',20);
        yValueBa = yValueStr.toLocal8Bit();
        yValueChar = yValueBa.data();
        buffer_m.write(yValueChar);

    }



}


void QtGraceTcpServer::readFooterText(){

    // Read the item name from socket
    dataSet1Ptr_m[QtGraceTcpServer::dataLength_m] = '\0';

    // Printstring length is 128, be careful
    if(QtGraceTcpServer::dataLength_m > 125){ // three bytes for .ps
        printf("\nItem name for QtGrace document couldn't be read, line to long!\n"); // BZ629-3 beep removed
        delete[]dataSet1Ptr_m;
        return;
    }

    // set the document name
    if(dataSet1Ptr_m[0] != '\0'){
        footerText_m =(char*) malloc(QtGraceTcpServer::dataLength_m + 1);
        strcpy(footerText_m, dataSet1Ptr_m);
    }

    delete[]dataSet1Ptr_m;
}


void QtGraceTcpServer::createProgressDialog(){
    receiveDataProgressDialog_ = new QProgressDialog();
    receiveDataProgressDialog_->setCancelButton(0);

    progressCounter_ = 0;
    receiveDataProgressDialog_->setLabelText(tr("Plotting..."));
    receiveDataProgressDialog_->setRange(0, 8000);
    receiveDataProgressDialog_->setModal(true);
    receiveDataProgressDialog_->setWindowFlags(Qt::WindowStaysOnTopHint);
    receiveDataProgressDialog_->setValue(0);

}

bool QtGraceTcpServer::getAllowExitQtGrace() const
{
    return allowExitQtGrace_;
}

void QtGraceTcpServer::readPsFileName(){

    // Read the item name from socket
    dataSet1Ptr_m[QtGraceTcpServer::dataLength_m] = '\0';

    // Printstring length is 128, be careful
    if(QtGraceTcpServer::dataLength_m > 125){ // three bytes for .ps
        printf("\nItem name for QtGrace document couldn't be read, line to long!\n"); // BZ629-3 beep removed
        delete[]dataSet1Ptr_m;
        return;
    }

    // set the document name
    if(dataSet1Ptr_m[0] != '\0'){
        qtGraceDocStrName_m = (string)dataSet1Ptr_m;

        QString docNameWithFullPath = QString::fromStdString(qtGraceDocStrName_m);

        if(!docNameWithFullPath.contains("%")){

            docNameWithFullPath = QDir::currentPath()+ "/" +docNameWithFullPath;
        }else{
            docNameWithFullPath.remove(QChar('%'));
        }


        //QString exportName = QString::fromStdString(qtGraceDocStrName_m);

        //  exportName = exportName.simplified();
        // exportName.replace( " ", "" );

        docNameWithFullPath = docNameWithFullPath.simplified();
        docNameWithFullPath.replace( " ", "" );

        // set_docname(dataSet1Ptr);
        //set_exportname_external(exportName.toStdString().c_str());
        set_docname_external(docNameWithFullPath.toStdString().c_str());


    }

    delete[]dataSet1Ptr_m;
    set_ptofile(TRUE);

}

void QtGraceTcpServer::setScalingMode()
{

    switch(mode_m){

    case AUTOSCALE_X_AXIS_OR_OVERLAY:
    {
        graphNo_m = QtGraceTcpServer::dataLength_m;

        //  world w;
        //  w.yg2 = xmax_m;
        //  w.yg1 = xmin_m;
        // set_graph_world(graphNo_m, w);
        /* autoscale x axis */
        autoscale_graph(graphNo_m, AUTOSCALE_X);
        autoscale_onread = AUTOSCALE_X;
        autotick_axis(graphNo_m, ALL_AXES);


        break;
    }


    case AUTOSCALE_Y_AXIS_OR_OVERLAY:
    {

        graphNo_m = QtGraceTcpServer::dataLength_m;
        //  world w;
        //    w.xg2 = xmax_m;
        //    w.xg1 = xmin_m;
        // set_graph_world(graphNo_m, w);
        /* autoscale y axis */
        autoscale_graph(graphNo_m, AUTOSCALE_Y);
        autoscale_onread = AUTOSCALE_Y;
        autotick_axis(graphNo_m, ALL_AXES);


        break;
    }
    case AUTOSCALE_ALL_AXES_OR_JOIN_PLOT:
    { /* autoscale all axis - default*/

        graphNo_m = QtGraceTcpServer::dataLength_m;
        autoscale_graph(graphNo_m, AUTOSCALE_XY);
        autoscale_onread = AUTOSCALE_XY;
        autotick_axis(graphNo_m, ALL_AXES);


        //update_all();
        break;
    }
    case DEFAULT_LAYOUT:
        /* no autoscale */

        autoscale_onread = AUTOSCALE_NONE;
        autotick_axis(graphNo_m, ALL_AXES);

        break;
    default:
    {
        fprintf(stderr, "Wrong autoscale mode!\n");
        fflush(stderr); // Will now print everything in the stderr buffer

        break;
    }
    }

    //    mode_m = DEFAULT_LAYOUT;

}

void QtGraceTcpServer::writeDataToTmpFile()
{

    if (isWriteToTmpFile_m){

        if(isDebugFlagOn_m){
            writeToDebugFile("\n******START READ FROM TEMP FILE***********"+
                             (QString)dataFromBuffer_m,DEBUGDETAILS);
        }

        // Save old FileCodec
        /*
        QTextCodec * FileCodecSave;
        FileCodecSave=FileCodec;
        FileCodec = QTextCodec::codecForName("UTF-8");
        */
        ///starting with v0.2.7 the encoding is always UTF-8

        //Read data from tmp file and update QtGrace plot
        readDataFromClient(dataFromBuffer_m.data(),0,"plot");

        update_all();

        //setResetExportDialogue(true);
        buffer_m.close();
        dataFromBuffer_m.clear();
        buffer_m.open(QIODevice::Append);

        writeToDebugFile("\n******END READ FROM TEMP FILE***********", DEBUGDETAILS,true);

       //Restore old FileCodec
       //FileCodec = FileCodecSave;

    }

}

void QtGraceTcpServer::setLayoutMode(){

    //Set layout for graph: "graphNo"
    graphNo_m = QtGraceTcpServer::dataLength_m;

    //Update legend properties dialogue
    for(int iSetNo = 0; iSetNo < countNoOfDataSets_m; iSetNo++){
        set_legend_string(graphNo_m,iSetNo,get_legend_string(graphNo_m,iSetNo));
        QString gotCommentQString;
        char*gotComment=getcomment(graphNo_m, iSetNo);
        if(gotComment)gotCommentQString=gotComment;
        if( gotCommentQString.toLatin1().isEmpty())
        {
            setcomment(graphNo_m,iSetNo,gotComment);

        }else{
            setcomment(graphNo_m,iSetNo,get_legend_string(graphNo_m,iSetNo));}
    }


    if(mode_m ==AUTOSCALE_ALL_AXES_OR_JOIN_PLOT){
        //When join there will only be one graph, even client sends it as more than one
        saveCountNoOfDataSets_m.replace(0,countNoOfDataSets_m);

    }else{
        saveCountNoOfDataSets_m.replace(graphNo_m,countNoOfDataSets_m);
        countNoOfDataSets_m = 0;

    }

    //Set graph position, i.e. numbers of columns and rows.

    switch(mode_m){

    case GRAPH_POSITION:
    {

        if(columns_m==0){
            rows_m = 1;
        }
        else {
            rows_m = numGraphs_m/columns_m;
            if(numGraphs_m%columns_m != 0)
                rows_m ++;
        }

        double offset = 0.15;
        double vgapArrangeGraph =0.2*numGraphs_m;
        double hgabArrangeGraph =0.2*numGraphs_m;

        int *graphs, i,  order, snake;
        int isFailure = false;

        order = 5;
        snake = 1;

        graphs = (int*)xmalloc(numGraphs_m*sizeof(int));///SIZEOF_INT);
        if (graphs == NULL) {
            isFailure = true;
        }

        if(!isFailure){
            for (i = 0; i < numGraphs_m; i++) {
                graphs[i] = i;
            }

            for (i = number_of_graphs() - 1; i >= numGraphs_m; i--) {
                kill_graph(i);
            }

            arrange_graphs(graphs, numGraphs_m, rows_m, columns_m, order, snake,
                           offset+0.06, offset-0.06, offset, offset, vgapArrangeGraph, hgabArrangeGraph, FALSE, FALSE,2,0.05,0.9, DEFAULT_TICK_PACK_X_INNER, DEFAULT_TICK_PACK_X_OUTER, DEFAULT_TICK_PACK_Y_INNER, DEFAULT_TICK_PACK_Y_OUTER,DEFAULT_TICK_PACK_X_INNER_DIR,DEFAULT_TICK_PACK_X_OUTER_DIR,DEFAULT_TICK_PACK_Y_INNER_DIR,DEFAULT_TICK_PACK_Y_OUTER_DIR);

            update_all();
            xfree(graphs);

        }



        break;
    }
    case AUTOSCALE_Y_AXIS_OR_OVERLAY: //overlay
    {
        if(graphNo_m != 0)  { // Lay all on the first one.
            overlay_graphs(graphNo_m, 0, GOVERLAY_SMART_AXES_XY);
        }

        break;
    }

    case AUTOSCALE_X_AXIS_OR_OVERLAY: //overlay
    {
        if(graphNo_m != 0)  { // Lay all on the first one.
            overlay_graphs(graphNo_m, 0, GOVERLAY_SMART_AXES_XY);
        }

        break;
    }

    case AUTOSCALE_ALL_AXES_OR_JOIN_PLOT:
    {
        // join, do nothing
        break;
    }
    case DEFAULT_LAYOUT:{
        //do nothing
        break;
    }
    default:
        fprintf(stderr, "Wrong layout mode!\n");
        break;
    }

    mode_m = DEFAULT_LAYOUT;
}

#if QT_VERSION >= 0x050100
void QtGraceTcpServer::applicationStateChanged(Qt::ApplicationState state){
    QTextStream out(stdout);

    if((Qt::ApplicationActive == state)){
        out << QObject::tr("has been active").toLocal8Bit().constData();// << endl;
    }

}
#endif

void QtGraceTcpServer::sendParam(){
    graphDataStreamToSendPtr_m = new std::ostringstream();

    int qtGraceDocStrNameLength = qtGraceDocStrName_m.length();
    //Send QtGrace graph parameters settings (PD file)
    int graphParamToSendLength = 1000000;
    char *pp = new char[graphParamToSendLength];
    char *ap = new char[graphParamToSendLength];

    //sprintf(pp, "\0");
    sprintf(pp, "%c",'\0');
    sprintf(ap, "%c",'\0');

    for(size_t i=0; i<(unsigned int)graphParamToSendLength;i++){
        //sprintf(pp+strlen(pp),"\0");
        sprintf(pp+strlen(pp),"%c",'\0');
        sprintf(ap+strlen(ap),"%c",'\0');

    }

    // Save old FileCodec
    /*
    QTextCodec * FileCodecSave;
    FileCodecSave=FileCodec;
    FileCodec = QTextCodec::codecForName("UTF-8");
    */
    ///starting with v0.2.7 the encoding always is UTF-8

    prepare_strings_for_saving();
    putparmbeast(-1,pp,TRUE);
    //START OF QTGRACE_SPECIALS_HERE!
    qtGrace_Additions_String(&ap);
    //END OF QTGRACE_SPECIALS_HERE!
    sprintf(pp+strlen(pp),"%s",ap);

    if(isDebugFlagOn_m){
        writeToDebugFile(pp, DEBUGDETAILS,true);
    }

    int paramLength = strlen(pp);

    writeToDataStream(( const char *)(&qtGraceDocStrNameLength),sizeof(int));
    writeToDataStream(qtGraceDocStrName_m.data(),qtGraceDocStrNameLength);
    writeToDataStream((const char *)(&paramLength), sizeof(int));
    writeToDataStream(pp, paramLength);

    resume_strings_after_load_or_save();
    //Restore old FileCode
    //FileCodec=FileCodecSave;

    delete[] pp;
    delete[] ap;
}

void QtGraceTcpServer::writeToDebugFile(QString message, QString debugDetails,bool isSkipLineNr){

    if(isDebugFlagOn_m){
        if(isSkipLineNr){
            *debugOutPtr_m << message;
            cerr << message.toStdString();

        }else{
            *debugOutPtr_m << debugDetails << " \nmessage: " << message;
            cerr << debugDetails.toStdString() << " \nmessage: " << message.toStdString();

        }
#if QT_VERSION>=0x051500
    *debugOutPtr_m << Qt::endl;
    cerr << Qt::endl;
#else
    *debugOutPtr_m << endl;
    cerr << endl;
#endif
        debugOutPtr_m->flush();
    }

}



void QtGraceTcpServer::writeSocketDisconnected()
{
    writeToDebugFile("Write socket disconnected",DEBUGDETAILS);
}

void QtGraceTcpServer::readSocketDisconnected()
{
    writeToDebugFile("Read socket disconnected",DEBUGDETAILS);
}

void QtGraceTcpServer::writeSocketConnected()
{

    writeToDebugFile("Write socket connected",DEBUGDETAILS);
}

void QtGraceTcpServer::readSocketConnected()
{

    writeToDebugFile("read socket connected",DEBUGDETAILS);
}

void QtGraceTcpServer::dataWritten(qint64 iData)
{
    writeToDebugFile("bytes written to client: " + QString::number(iData),DEBUGDETAILS);
}

void QtGraceTcpServer::socketError(QAbstractSocket::SocketError) {

    if(writeConnectionPtr_m->error()!=QAbstractSocket::RemoteHostClosedError)
    {
        writeToDebugFile("The following communicaton error occurred: " + writeConnectionPtr_m->errorString());

        qApp->removeEventFilter(keyPressAndMousePressfilter_);
        /*QMessageBox::information(0,"Communication Error1",tr("The following error occurred: %1.")
                                 .arg(writeConnectionPtr_m->errorString()));
              QMessageBox::information(0,"Communication Error",tr("The following error occurred: %1.")
                             .arg(writeConnectionPtr_m->errorString()));
        QMessageBox::information(0,"(4)Communication Error.", "Please close QtGrace and plot again.");
*/
        if(!isTCPErrorToStatusBar_){

            set_footer_background_color("WARNING 4: The connection to ViewBeast has been aborted. "
                                        "Please close QtGrace and press 'Plot New' in ViewBeast, to start a new instance of "
                                        "QtGrace.",1);
        }

        isTCPErrorToStatusBar_ = true;


    }

}

