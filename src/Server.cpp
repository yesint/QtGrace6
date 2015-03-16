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
#include <QtNetwork/QLocalSocket>
#include <QMessageBox>

extern char startupphase;

// initialize server
LocalSocketIpcServer::LocalSocketIpcServer(QString writeServerName, QString readServerName, QObject *parent)    :QObject(parent)
  ,isDebugFlagOn_m(false)
  ,messageSendGraphParam_m(NULL)
  ,messageParamGraphLength_m(0)
  ,messagePtr_m(NULL)
  ,dataSet1Ptr(NULL)
  ,dataSet2Ptr_m(NULL)
  ,command_m(READ_MODE)
  ,dataLength_m(0)
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
  ,debugFile_m(NULL)
  ,debugOut_m(NULL)
  ,mode_m(DEFAULT_LAYOUT)
  ,xminPtr_m(NULL)
  ,xmaxPtr_m(NULL)
  ,columns_m(0)
  ,rows_m(0)
  ,qtGraceDocStrName_m("Untitled")
  ,dataFromBuffer_m(" ")
  ,availableBytesFromSocket_m(0)
  ,messageFromClienttPtr_m(NULL)
  ,messageToClientPtr_(NULL)
  ,readServer_m("")
  ,clientConnection(NULL)

{
    if(getenv("QTGRACEDEBUG")) {
      cout<<"Detected QTGRACEDEBUG and writes to QTGRACEDEBUG.txt\n";
      isDebugFlagOn_m = true;
    }

    if(isDebugFlagOn_m){
        debugFile_m = new QFile("QTGRACEDEBUG.txt");
        debugFile_m->open(QIODevice::WriteOnly | QIODevice::Text);
        debugOut_m = new QTextStream(debugFile_m);
        *debugOut_m<<"***DEBUG MODE ENABLE***\n";
        debugOut_m->flush();
       }

    for(int i=0;i<10;i++){
        saveCountNoOfDataSets_m.append(0);
    }

    //Read from Beast
    messageFromClienttPtr_m = new QLocalServer(this);
    qDebug()<<"Start Init\n";

    bool listenOK=messageFromClienttPtr_m->listen(writeServerName);
    if(listenOK){
      //  QMessageBox::information(0,"Communication Error", ". Try to restart QtGrace");

       qDebug()<<"Start the Server (listen OK)\n";
        if(isDebugFlagOn_m){
            *debugOut_m<< "Start the Server (listen OK)\n"<<endl;
            debugOut_m->flush();
        }



    }  else{
        qDebug()<<"Not able to start the Server\n";

        if(isDebugFlagOn_m){
            *debugOut_m<< "Not able to start the Server\n"<<endl;
            debugOut_m->flush();
        }



    }

    connect(messageFromClienttPtr_m, SIGNAL(newConnection()), this, SLOT(createNewSocketConnection()));

    //Buffer to save data from socket
    buffer_m.setBuffer(&dataFromBuffer_m);
    buffer_m.open(QIODevice::Append);

    //Write to client
    messageToClientPtr_ = new QLocalSocket(this);
    readServer_m = readServerName;
    connect(messageToClientPtr_, SIGNAL(connected()), this, SLOT(sendDataToClient()));
    connect(messageToClientPtr_, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    connect(messageToClientPtr_, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(socketError(QLocalSocket::LocalSocketError)));

    if(isDebugFlagOn_m){
        *debugOut_m<<"Done constructor\n";
        debugOut_m->flush();
    }

}

void LocalSocketIpcServer::ConnectToClient( const char* sendParam, int sendLen) {

    qDebug()<<"connect To client line 126";

    if(isDebugFlagOn_m){
        *debugOut_m<< "2) Connect to Server\n"+readServer_m;
        *debugOut_m<< "sendParam as int="<< *(int*)(sendParam)<<"\n";
        debugOut_m->flush();
    }


    //Wait to write to QLocalSocket buffer until previous data has been analysed by client and sends a disconnect signal.
    while(messageToClientPtr_->state()==QLocalSocket::ConnectedState)
    {
         if(!messageToClientPtr_->waitForDisconnected(60000)){
             //We don't want to wait for a disconnect signal from client forever.
            QMessageBox::information(0,"Communication Error",messageToClientPtr_->errorString() + ". Try to restart QtGrace");
            exit(0);
          }

    }

    messageSendGraphParam_m = sendParam;
    messageParamGraphLength_m = sendLen;
    messageToClientPtr_->connectToServer(readServer_m);

}

LocalSocketIpcServer::~LocalSocketIpcServer() {
    if(isDebugFlagOn_m){
        *debugOut_m<<"Server deletion\n";
        debugOut_m->flush();
    }
    messageFromClienttPtr_m->close();
    delete messageFromClienttPtr_m;
    messageFromClienttPtr_m = NULL;

    messageToClientPtr_->abort();
    delete messageToClientPtr_;
    messageToClientPtr_ = NULL;

    if(clientConnection!=NULL){
    clientConnection->abort();
    delete clientConnection;
    clientConnection = NULL;
}

    if(isDebugFlagOn_m){
        debugFile_m->close();
        delete debugFile_m;
        delete debugOut_m;
    }

}

void LocalSocketIpcServer::createNewSocketConnection(){
    clientConnection = messageFromClienttPtr_m->nextPendingConnection();

    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));

    connect(clientConnection, SIGNAL(readyRead()),
            this, SLOT(readFromClient()));

    connect(clientConnection, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(socketError(QLocalSocket::LocalSocketError)));

    countNoOfRead_m++;
}


void LocalSocketIpcServer::readFromClient() {

    if(isDebugFlagOn_m){
        *debugOut_m<<"readSocket() START\n";
        debugOut_m->flush();
    }

    conditionToExitFunction_m = 0;

    //Specifiy the amount of bytes to be read
    if(isDebugFlagOn_m){
        *debugOut_m<<"countNoOfRead="<<countNoOfRead_m<<" command="<<command_m<<"\n";
         debugOut_m->flush();
    }
    int bytesNeeded;
    if(countNoOfRead_m==1 ||
            countNoOfRead_m==2 ||
            (command_m == SET_SCALING_MODE && countNoOfRead_m==3) ||
            command_m == SET_LAYOUT_MODE){
        bytesNeeded=(int)sizeof(quint32);
    }else if (command_m==WRITE_DATAVEC){
        bytesNeeded= dataLength_m*8;
    }else if(command_m == SET_SCALING_MODE && mode_m==AUTOSCALE_Y_AXIS_OR_OVERLAY){
        bytesNeeded=(int)sizeof(double);
    }else{
        bytesNeeded= dataLength_m;
    }

    if(clientConnection->bytesAvailable() < bytesNeeded){
        if(isDebugFlagOn_m){
            *debugOut_m<<"In loop : Needed="<<bytesNeeded<<" Available="<<clientConnection->bytesAvailable()<<"\n";
             debugOut_m->flush();
        }

               clientConnection->waitForReadyRead();

    }else{

    availableBytesFromSocket_m = clientConnection->bytesAvailable();
    messagePtr_m=new char[availableBytesFromSocket_m+1];

    // ensure that
    // 0-terminated character strings
    // come here correctly.
    messagePtr_m[availableBytesFromSocket_m]=0;

    int receivedFromRead=clientConnection->read(messagePtr_m,availableBytesFromSocket_m);

    if (receivedFromRead==-1) {
        if(isDebugFlagOn_m){
            *debugOut_m<<"readSocket() FAIL 2"<<"\n";
             debugOut_m->flush();
        }
        clientConnection->disconnectFromServer();
        return;
    }

    if(isDebugFlagOn_m){
        *debugOut_m<<"Afterreading bytesAvailable=" <<  clientConnection->bytesAvailable() << " bytes\n";
        debugOut_m->flush();
    }

    // Read all data from socket

    saveDataFromSocket(countNoOfRead_m);
    delete[] messagePtr_m;

    if (conditionToExitFunction_m) {
        if(isDebugFlagOn_m){
            *debugOut_m<<"An argument countNoOfRead " << countNoOfRead_m<< " for cmd="<< command_m<<"\n";
             debugOut_m->flush();
        }
        clientConnection->disconnectFromServer();
        return;
    }

    //Execute task from client
    if(isDebugFlagOn_m){
        *debugOut_m<<"Command No (" << command_m<< ")\n";
    }

    executeTaskFromClient();

    if(isDebugFlagOn_m){
        *debugOut_m<<"Command was performed " << command_m<<"\n";
        debugOut_m->flush();
    }

    clientConnection->disconnectFromServer();
    if(isDebugFlagOn_m){
        *debugOut_m<<"readSocket() DONE\n";
        debugOut_m->flush();
    }

}
}
void LocalSocketIpcServer::executeTaskFromClient()
{
    switch (command_m){

    case PLOT_INFO://Read PLOT_INFO(1)
    {
        if(isDebugFlagOn_m){
            *debugOut_m<<"Run Command" << command_m<<"\n";
             debugOut_m->flush();
        }
        buffer_m.write(dataSet1Ptr);
        isWriteToTmpFile_m=true;
        countNoOfRead_m = 0;
        break;
    }

    case WRITE_DATAVEC://WRITE_DATAVEC(2)

    {
        if(isDebugFlagOn_m){
            *debugOut_m<<"Run Command" << command_m<<"\n";
             debugOut_m->flush();
        }
        readXYData(dataSet1Ptr, dataSet2Ptr_m);
        countNoOfRead_m = 0;
        newDataSetReady_m=0;
        break;
    }
    case WRITE_DATAVEC_FINISHED:

    {  if(isDebugFlagOn_m){
            *debugOut_m<<"Run Command" << command_m<<"\n";
             debugOut_m->flush();}
        countNoOfRead_m = 0;
        buffer_m.write("\n");
        newDataSetReady_m=1;
        countNoOfDataSets_m++;

        break;
    }

    case READ_MODE:

    {    if(isDebugFlagOn_m){    *debugOut_m<<"Run Command" << command_m<<"\n";
       debugOut_m->flush();
        }
        sendParam();
        countNoOfRead_m = 0;
        break;
    }
    case REDRAW:
    {
        if(isDebugFlagOn_m){
            *debugOut_m<<"Run Command" << command_m<<"\n";
        }

        set_page_dimensions(733,538,1);
        //set_page_geometry()

        startupphase=1;
        oldNoask_m=noask;
        noask=true; // prevent questions
        writeDataToTmpFile();
        setScalingMode();
        noask=oldNoask_m;
        startupphase=0;

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
    {    if(isDebugFlagOn_m){
            *debugOut_m<<"Run Command" << command_m<<"\n";
             debugOut_m->flush();
        }
        readPsFileName();

        countNoOfRead_m = 0;
        break;
    }
    case SET_SCALING_MODE:
    {
        if(isDebugFlagOn_m){
            *debugOut_m<<"Run Command" << command_m<<"\n";
            debugOut_m->flush();
        }
        startupphase=1;
        oldNoask_m=noask;
        noask=true; // prevent questions
        writeDataToTmpFile();
        setScalingMode();
        noask=oldNoask_m;
        startupphase=0;

        countNoOfRead_m = 0;
        isWriteToTmpFile_m=false;
        break;
    }

    case REDRAW_AND_WRITEPS://REDRAW_AND_WRITEPS(7)
        {
        if(isDebugFlagOn_m){
            *debugOut_m<<"Run Command" << command_m<<"\n";
            *debugOut_m<<"fileName" <<   get_docname()<<"\n";
            debugOut_m->flush();
        }
        /* force a hardcopy */
        set_pagelayout(PAGE_FIXED);
        update_all();

        oldNoask_m=noask;
        noask=true; // prevent questions
        do_hardcopy();
        noask=oldNoask_m;
        countNoOfRead_m = 0;
        break;
    }
    case SET_LAYOUT_MODE:
    {
        if(isDebugFlagOn_m){
            *debugOut_m<<"Run Command" << command_m<<"\n";
            debugOut_m->flush();
        }
        startupphase=1;
        setLayoutMode();
        startupphase=0;
        if(isDebugFlagOn_m){
            *debugOut_m<<"Was setLayoutMode" << command_m<<"\n";
            debugOut_m->flush();
        }
        countNoOfRead_m = 0;
        break;
    }

    case KILL_CHILD:
    {
        if(isDebugFlagOn_m){
            *debugOut_m<<"Run Command" << command_m<<"\n";
            debugOut_m->flush();
        }
        /* kill me */
        /* printf("got killed"); */

        if(isDebugFlagOn_m){
        debugFile_m->close();
        delete debugFile_m;
        delete debugOut_m;
    }

        exit(0);
        countNoOfRead_m = 0;
        break;
    }
    case END_COMM:
    {
        if(isDebugFlagOn_m){
            *debugOut_m<<"Run Command" << command_m<<"\n";
            debugOut_m->flush();
        }
        update_all();
        xdrawgraph();
        doPlotFit();

        isWriteToTmpFile_m=true;
        countNoOfRead_m = 0;
        break;
    }
    case TEST_CONNECTION:
    {
        if(isDebugFlagOn_m){
            *debugOut_m<<"Run Command" << command_m<<"\n";
            debugOut_m->flush();
        }
        countNoOfRead_m = 0;
        break;
    }

    default:
    {
        if(isDebugFlagOn_m){
            *debugOut_m<<"INVALID COMMAND STOP" << command_m<<"\n";
            debugOut_m->flush();
        }
        QMessageBox::information(0,"Communication Error","Communication error: try to restart");

        exit(0);
        countNoOfRead_m = 0;
        break;
    }
    }
}


void LocalSocketIpcServer::sendDataToClient(){

    messageToClientPtr_->write(messageSendGraphParam_m,messageParamGraphLength_m);  //Produces a QT warning: QWinEventNotifier: Cannot have more than 62 enabled at one time - Maybe a QT bug?

}

char* LocalSocketIpcServer::copyDataFromSocket(int availableBytes, char* dataFromSocket){

    char* data;
    data = new char[availableBytes+1];
    memcpy( data , dataFromSocket, availableBytes+1);
    return data;

}

void LocalSocketIpcServer::getCommandFromClient(int commandFromsocket)
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
        command_m =  READ_MODE;
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
    default:
        QMessageBox::information(0,"Communication Error","Communication error: Command not found");
        exit(0);
        break;
    }


}

void LocalSocketIpcServer::readDataFromSocket(char *newDataFromSocket, int availableBytes, readCommands readMode){
    switch (readMode){

    case START_READ:
    {
        int commandFromsocket = *((int*)(newDataFromSocket));

        getCommandFromClient(commandFromsocket);

        if(isDebugFlagOn_m){*debugOut_m<< " The command is int, 4 bytes are "<<
                                  (int)(newDataFromSocket[0]) << " " <<
                                  (int)(newDataFromSocket[1]) << " " <<
                                  (int)(newDataFromSocket[2]) << " " <<
                                  (int)(newDataFromSocket[3]) << " ";
            debugOut_m->flush();
        }
        break;
    }

    case READ_DATALENGTH: //Read data length
        {
        dataLength_m = *((int*)(newDataFromSocket));
        if(isDebugFlagOn_m){
            *debugOut_m<< " Got data length= "<< dataLength_m <<"\n";
        debugOut_m->flush();
        }
        break;
    }

    case READ_DATASET_1:

        {
        if(command_m!=SET_SCALING_MODE && command_m!=SET_LAYOUT_MODE){
            dataSet1Ptr = copyDataFromSocket(availableBytes,newDataFromSocket);
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
            default:
                break;
            }

            if(isDebugFlagOn_m){
                *debugOut_m<< " Got mode= "<< mode_m<<"\n" ;
           debugOut_m->flush();
            }

        }
        break;
        }

    case READ_PLOT_SETTINGS_1_FROM_CLIENT: //Read Plot settings from client dialogue

        {
        if(command_m == SET_SCALING_MODE){ //Min x-axis length
            xminPtr_m = (double *)newDataFromSocket;
            xmin_m = xminPtr_m[0];
        }
        else if(command_m == SET_LAYOUT_MODE){ // Numbers of columns
            columns_m = *((int*)(newDataFromSocket));
        }
        else{
            dataSet2Ptr_m = copyDataFromSocket(availableBytes,newDataFromSocket);
        }
        break;
        }

    case READ_PLOT_SETTINGS_2_FROM_CLIENT://Read Plot settings from client dialogue
        {
        if(command_m == SET_SCALING_MODE){  //Max x-axis length
            xmaxPtr_m = (double *)newDataFromSocket;
            xmax_m = xmaxPtr_m[0];

        }
        if(command_m == SET_LAYOUT_MODE) //numbers of graphs
        {
            numGraphs_m = *((int*)(newDataFromSocket));
        }

        break;
        }
    default:
        QMessageBox::information(0,"Communication Error","Communication error: Command not found");
        exit(0);
        break;
    }
}

void LocalSocketIpcServer::saveDataFromSocket(int numberOfRead){

    switch (numberOfRead){

    case 1:
        {
        readDataFromSocket(messagePtr_m,availableBytesFromSocket_m,  START_READ);

        if(command_m == READ_MODE || command_m == REDRAW ||
                command_m == REDRAW_AND_WRITEPS || command_m == KILL_CHILD ||
                command_m == END_COMM || command_m == TEST_CONNECTION ||
                command_m == WRITE_DATAVEC_FINISHED)

            conditionToExitFunction_m = 0;
        else
            conditionToExitFunction_m = 1;

        break;
        }
    case 2:
        {
        readDataFromSocket(messagePtr_m,availableBytesFromSocket_m, READ_DATALENGTH);
        conditionToExitFunction_m = 1;
        break;
        }
    case 3:
        {
        readDataFromSocket(messagePtr_m,availableBytesFromSocket_m, READ_DATASET_1);

        if(isDebugFlagOn_m){
            *debugOut_m<< " Analysing(3) mode= "<< mode_m<<"\n" ;
         debugOut_m->flush();
        }

        if((command_m == SET_SCALING_MODE && mode_m == AUTOSCALE_Y_AXIS_OR_OVERLAY)||
                (command_m == SET_LAYOUT_MODE && mode_m == GRAPH_POSITION) ||
                command_m == WRITE_DATAVEC)

            conditionToExitFunction_m = 1;
        break;
        }

    case 4:
        {
        readDataFromSocket(messagePtr_m,availableBytesFromSocket_m, READ_PLOT_SETTINGS_1_FROM_CLIENT);

        if(isDebugFlagOn_m){
            *debugOut_m<< " Analysing(4) mode= "<< mode_m<<"\n" ;
             debugOut_m->flush();
        }

        if((command_m == SET_SCALING_MODE && mode_m == AUTOSCALE_Y_AXIS_OR_OVERLAY)||
                (command_m == SET_LAYOUT_MODE && mode_m == GRAPH_POSITION))

            conditionToExitFunction_m = 1;

        break;
        }
    case 5:
        {
        readDataFromSocket(messagePtr_m,availableBytesFromSocket_m, READ_PLOT_SETTINGS_2_FROM_CLIENT);
        break;
        }

    default:
        {
        conditionToExitFunction_m = 1;
        break;
    }
    }

}

void LocalSocketIpcServer::readXYData(char* xData, char* yData){

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
    for(int i=0; i<dataLength_m; i++){
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
        xValueStr = QString::number(x[i],'g',20);
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

void LocalSocketIpcServer::readPsFileName(){

    // Read the item name from socket
    dataSet1Ptr[dataLength_m] = '\0';

    // Printstring length is 128, be careful
    if(dataLength_m > 125){ // three bytes for .ps
        printf("\nItem name for QtGrace document couldn't be read, line to long!\n"); // BZ629-3 beep removed
        delete[]dataSet1Ptr;
        return;
    }

    // set the document name
    if(dataSet1Ptr[0] != '\0'){
        qtGraceDocStrName_m = (string)dataSet1Ptr;
        set_docname(dataSet1Ptr);
    }

    delete[]dataSet1Ptr;
    set_ptofile(TRUE);

}

void LocalSocketIpcServer::setScalingMode()
{

    switch(mode_m){
    case AUTOSCALE_Y_AXIS_OR_OVERLAY:
        { graphNo_m = dataLength_m;
        world w;
        w.xg2 = xmax_m;
        w.xg1 = xmin_m;
        set_graph_world(graphNo_m, w);

        /* autoscale y axis */
        autoscale_graph(graphNo_m, 2);
        break;
        }
    case AUTOSCALE_ALL_AXES_OR_JOIN_PLOT:
        { /* autoscale all axis - default*/
        autoscale_graph(graphNo_m, 3);
        //  update_all();
        break;
        }
    case DEFAULT_LAYOUT:
        /* no autoscale */
        autoscale_onread = AUTOSCALE_NONE;

        break;
    default:
        { fprintf(stderr, "Wrong autoscale mode!\n");
        break;}
    }

    mode_m = DEFAULT_LAYOUT;

}

void LocalSocketIpcServer::writeDataToTmpFile()
{

    if (isWriteToTmpFile_m){

        if(isDebugFlagOn_m){*debugOut_m<<"******START READ FROM TEMP FILE***********\n";
            *debugOut_m<<dataFromBuffer_m;
            debugOut_m->flush();
        }

        //Read data from tmp file and update QtGrace plot
        readDataFromClient(dataFromBuffer_m.data(),0,"plot");
        update_all();
     //   setResetExportDialogue(true);
        buffer_m.close();
        dataFromBuffer_m.clear();
        buffer_m.open(QIODevice::Append);
        if(isDebugFlagOn_m){
            *debugOut_m<<"******END READ FROM TEMP FILE***********\n";
            debugOut_m->flush();
        }
    }

}

void LocalSocketIpcServer::setLayoutMode(){

    //Set layout for graph: "graphNo"
    graphNo_m = dataLength_m;

    //Set QtGrace plot viewport
    view v;
    v.xv1 = 0.21;
    v.xv2 = 1.21;
    v.yv1 =0.15;
    v.yv2 = 0.85;

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

        arrange_graphs_simple(rows_m, columns_m,1, 1,offset,hgabArrangeGraph,vgapArrangeGraph);

        break;
    }
    case AUTOSCALE_Y_AXIS_OR_OVERLAY: //overlay
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

void LocalSocketIpcServer::sendParam(){

    //Send QtGrace document name to client (PD file)

    int qtGraceDocStrNameLength = qtGraceDocStrName_m.length();

    ConnectToClient((const char *)(&qtGraceDocStrNameLength),sizeof(int));
    ConnectToClient(qtGraceDocStrName_m.data(),qtGraceDocStrNameLength);

    //Send QtGrace graph parameters settings (PD file)
    int graphParamToSendLength = 1000000;
    char *pp = new char[graphParamToSendLength];
    sprintf(pp, "\0");

    for(size_t i=0; i<graphParamToSendLength;i++){
        sprintf(pp+strlen(pp),"\0");
    }

    putparmbeast(-1,pp,TRUE);

    int paramLength = strlen(pp);
    ConnectToClient((const char *)(&paramLength),sizeof(int));
    ConnectToClient(pp,paramLength);
    delete[] pp;
}


void LocalSocketIpcServer::socketDisconnected() {
    if(isDebugFlagOn_m){*debugOut_m<< "socket_disconnected\n";
    debugOut_m->flush();
    }
}


void LocalSocketIpcServer::socketReadReady() {
    if(isDebugFlagOn_m){ *debugOut_m<< "socket_readReady\n";
    debugOut_m->flush();}
}

void LocalSocketIpcServer::socketError(QLocalSocket::LocalSocketError) {
    if(isDebugFlagOn_m){*debugOut_m<< "socket_error\n";
    debugOut_m->flush();
    }

    if(messageToClientPtr_->error()!=QAbstractSocket::RemoteHostClosedError)
        QMessageBox::information(0,"Communication Error",messageToClientPtr_->errorString() + ". Try to restart QtGrace");

}

const char* LocalSocketIpcServer::createUniqueFileName(){

    //get current date and time
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTimeString = dateTime.toString("hhmmsszzz");

    QString sendToBeast = "qtGrace"+dateTimeString;
    QByteArray fileNameBa1 = sendToBeast.toLocal8Bit();

    return fileNameBa1.data();



}
