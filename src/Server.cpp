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

extern bool startupphase;


// initialize server
LocalSocketIpcServer::LocalSocketIpcServer(QString writeServerName, QString readServerName, QObject *parent)    :QObject(parent)
  ,socketConnectedBusy(false)
  ,messagePtr(NULL)
  ,dataSet1Ptr(NULL)
  ,dataSet2Ptr(NULL)
  ,command(0)
  ,dataLength(0)
  ,graphNo(0)
  ,xmin(0)
  ,xmax(0)
  ,conditionToExitFunction(0)
  ,countNoOfRead(0)
  ,newDataSetReady(1)
  ,newSetNosPtr(NULL)
  ,writeToTmpFile(true)
  ,numGraphs(0)
  ,countNoOfDataSets(0)
  ,oldNoask(0)
  ,debugFile(NULL)
  ,debugOut(NULL)
  ,mode(0)
  ,debugFlag(false)
{
    if(getenv("QTGRACEDEBUG")) {
      cout<<"Detected QTGRACEDEBUG and writes to QTGRACEDEBUG.txt\n";
      debugFlag = true;
    }

    if(debugFlag){
        debugFile = new QFile("QTGRACEDEBUG.txt");
        debugFile->open(QIODevice::WriteOnly | QIODevice::Text);
        debugOut = new QTextStream(debugFile);
        *debugOut<<"***DEBUG MODE ENABLE***\n";
    }

    for(int i=0;i<10;i++){
        saveCountNoOfDataSets.append(0);
    }

    //Read from Beast
    messageFromBeastPtr = new QLocalServer(this);


    bool listenOK=messageFromBeastPtr->listen(writeServerName);
    if(listenOK){
        if(debugFlag){
            *debugOut<< "Start the Server (listen OK)\n"<<endl;
        }
    }  else{
        if(debugFlag){
            *debugOut<< "Not able to start the Server\n"<<endl;
        }
    }

   // messageFromBeastPtr->setMaxPendingConnections(300);

    connect(messageFromBeastPtr, SIGNAL(newConnection()), this, SLOT(readSocket()));

    //Buffer to save data from socket
    buffer.setBuffer(&dataFromBuffer);
    buffer.open(QIODevice::Append);

    readSocketIsLocked=false;

    //Write to Beasst
    readServer = readServerName;
    messageToBeastPtr =   new QLocalSocket(this);
    connect(messageToBeastPtr, SIGNAL(connected()), this, SLOT(sendDataToBeast()));
    connect(messageToBeastPtr, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    connect(messageToBeastPtr, SIGNAL(readyRead()), this, SLOT(socketReadReady()));

    connect(messageToBeastPtr, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(socketError(QLocalSocket::LocalSocketError)));
    if(debugFlag){
        *debugOut<<"Done constructor\n";
    }

}

void LocalSocketIpcServer::ConnectToBeast( const char* sendParam, int sendLen) {
    if(debugFlag){
        *debugOut<< "2) Connect to Server\n"+readServer;
        *debugOut<< "sendParam as int="<< *(int*)(sendParam)<<"\n";
    }


	//Wait to write to QLocalSocket buffer until previous data has been analysed by ViewBeast and sends a disconnect signal.
    while(messageToBeastPtr->state()==QLocalSocket::ConnectedState)
    {
		 if(!messageToBeastPtr->waitForDisconnected(60000)){
			 //We don't want to wait for a disconnect signal from ViewBeast forever.
            QMessageBox::information(0,"Communication Error",messageToBeastPtr->errorString() + ". Try to restart QtGrace");
            exit(0);
          }

    }

    messageSendGraphParam = sendParam;
    messageParamGraphLength = sendLen;
    messageToBeastPtr->connectToServer(readServer);

}

LocalSocketIpcServer::~LocalSocketIpcServer() {
    if(debugFlag){
        *debugOut<<"Server deletion\n";
    }
    messageFromBeastPtr->close();
    delete messageFromBeastPtr;
    messageFromBeastPtr = NULL;

    messageToBeastPtr->abort();
    delete messageToBeastPtr;
    messageToBeastPtr = NULL;

    if(debugFlag){
        debugFile->close();
        delete debugFile;
        delete debugOut;
    }

}



void LocalSocketIpcServer::readSocket() {
    if(debugFlag){
        *debugOut<<"readSocket() START\n";
    }


    conditionToExitFunction = 0;

    QLocalSocket *clientConnection = messageFromBeastPtr->nextPendingConnection();

    countNoOfRead++;

    //Specifiy the amount of bytes to be read
    if(debugFlag){
        *debugOut<<"countNoOfRead="<<countNoOfRead<<" command="<<command<<"\n";
    }
    int bytesNeeded;
    if(countNoOfRead==1 || countNoOfRead==2 ||(command == 6 && countNoOfRead==3) || command == 8){
        bytesNeeded=(int)sizeof(quint32);
    }else if (command==2){
        bytesNeeded= dataLength*8;
    }else if(command == 6 && mode==2){
        bytesNeeded=(int)sizeof(double);
    }else{
        bytesNeeded= dataLength;
    }

	while (clientConnection->bytesAvailable() < bytesNeeded){
        if(debugFlag){
            *debugOut<<"In loop : Needed="<<bytesNeeded<<" Available="<<clientConnection->bytesAvailable()<<"\n";
        }
        clientConnection->waitForReadyRead();
	
    }

    availableBytesFromSocket = clientConnection->bytesAvailable();
    messagePtr=new char[availableBytesFromSocket+1];

    // ensure that
    // 0-terminated character strings
    // come here correctly.
    messagePtr[availableBytesFromSocket]=0;

    int receivedFromRead=clientConnection->read(messagePtr,availableBytesFromSocket);

    if (receivedFromRead==-1) {
        if(debugFlag){
            *debugOut<<"readSocket() FAIL 2"<<"\n";
        }
        clientConnection->disconnectFromServer();
        delete clientConnection;
        return;
    }

    if(debugFlag){
        *debugOut<<"Afterreading bytesAvailable=" <<  clientConnection->bytesAvailable() << " bytes\n";
    }

    /* read all data from socket */

    saveDataFromSocket(countNoOfRead);
    delete[] messagePtr;

    if (conditionToExitFunction) {
        if(debugFlag){
            *debugOut<<"An argument countNoOfRead " << countNoOfRead<< " for cmd="<< command<<"\n";
        }
        clientConnection->disconnectFromServer();
        delete clientConnection;
        return;
    }


    //Execute task from ViewBeast
    if(debugFlag){
        *debugOut<<"Command No (" << command<< ")\n";
    }
    switch (command){

    case 1://Read PLOT_INFO(1)
    {

        if(debugFlag){
            *debugOut<<"Run Command" << command<<"\n";
        }
        buffer.write(dataSet1Ptr);
        writeToTmpFile=true;
        countNoOfRead = 0;
        break;
    }

    case 2://WRITE_DATAVEC(2)

    {
        if(debugFlag){
            *debugOut<<"Run Command" << command<<"\n";
        }
        readXYData(dataSet1Ptr, dataSet2Ptr);
        countNoOfRead = 0;
        newDataSetReady=0;
        break;
    }
    case 12://WRITE_DATAVEC_FINISH(2)

    {  if(debugFlag){ *debugOut<<"Run Command" << command<<"\n";}
        countNoOfRead = 0;
        buffer.write("\n");
        newDataSetReady=1;
        countNoOfDataSets++;

        break;
    }

    case 3://READ_MODE(3)

    {    if(debugFlag){    *debugOut<<"Run Command" << command<<"\n";
        }
        sendParam();
        countNoOfRead = 0;
        break;
    }
    case 4://REDRAW(4)
    {
        if(debugFlag){
            *debugOut<<"Run Command" << command<<"\n";
        }

        set_page_dimensions(733,538,1);
        //set_page_geometry()

        startupphase=true;
        oldNoask=noask;
        noask=true; // prevent questions
        writeDataToTmpFile();
        setScalingMode();
        noask=oldNoask;
        startupphase=false;

        //Update legend properties
        for(int igno = 0; igno < graphNo+1; igno++){
            for(int iSetNo = 0; iSetNo < saveCountNoOfDataSets.at(igno); iSetNo++){
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

        countNoOfDataSets = 0;
        countNoOfRead = 0;
        writeToTmpFile=false;

        break;
    }
    case 5://PS_FILENAME(5)
    {    if(debugFlag){
            *debugOut<<"Run Command" << command<<"\n";
        }
        //hdevice=1;
        readPsFileName();
        countNoOfRead = 0;
        break;
    }
    case 6://SET_SCALING_MODE(6)
    {
        if(debugFlag){
            *debugOut<<"Run Command" << command<<"\n";
        }
        startupphase=true;
        oldNoask=noask;
        noask=true; // prevent questions
        writeDataToTmpFile();
        setScalingMode();
        noask=oldNoask;
        startupphase=false;

        countNoOfRead = 0;
        writeToTmpFile=false;
        break;
    }

    case 7://REDRAW_AND_WRITEPS(7)
        {
        if(debugFlag){
            *debugOut<<"Run Command" << command<<"\n";
        }
        /* force a hardcopy */
        set_pagelayout(PAGE_FIXED);
        update_all();


        oldNoask=noask;
        noask=true; // prevent questions
        do_hardcopy();
        noask=oldNoask;
        countNoOfRead = 0;
        break;
    }
    case 8://SET_LAYOUT_MODE(8)
    {
        if(debugFlag){
            *debugOut<<"Run Command" << command<<"\n";
        }
        startupphase=true;
        setLayoutMode();
        startupphase=false;
        if(debugFlag){
            *debugOut<<"Was setLayoutMode" << command<<"\n";
        }
        countNoOfRead = 0;
        break;
    }
    case 9://Close connection to Beast(9)
    {
        if(debugFlag){
            *debugOut<<"Run Command" << command<<"\n";
        }
        messageFromBeastPtr->close();

        break;
    }

    case 42://KILL_CHILD(42)
    {
        if(debugFlag){
            *debugOut<<"Run Command" << command<<"\n";
        }
        /* kill me */
        /* printf("got killed"); */

        if(debugFlag){
        debugFile->close();
        delete debugFile;
        delete debugOut;
    }

        exit(0);
        countNoOfRead = 0;
        break;
    }
    case 99://END_COMM(99)
    {
        if(debugFlag){
            *debugOut<<"Run Command" << command<<"\n";
        }
        update_all();
        xdrawgraph();
        doPlotFit();

        writeToTmpFile=true;
        countNoOfRead = 0;
        break;
    }
    case 98://TEST_CONNECTION(98)
    {
        if(debugFlag){
            *debugOut<<"Run Command" << command<<"\n";
        }
        countNoOfRead = 0;
        break;
    }

    default:
    {
        if(debugFlag){
            *debugOut<<"INVALID COMMAND STOP" << command<<"\n";
        }
        QMessageBox::information(0,"Communication Error","Communication error: try to restart");

        exit(0);
        countNoOfRead = 0;
        break;
    }
    }
    if(debugFlag){
        *debugOut<<"Command was performed " << command<<"\n";
    }

    clientConnection->disconnectFromServer();
    delete clientConnection;
    if(debugFlag){
        *debugOut<<"readSocket() DONE\n";
    }




}

void LocalSocketIpcServer::sendDataToBeast(){

    messageToBeastPtr->write(messageSendGraphParam,messageParamGraphLength);  //Produces a QT warning: QWinEventNotifier: Cannot have more than 62 enabled at one time - Maybe a QT bug?

}

char* LocalSocketIpcServer::copyDataFromSocket(int availableBytes, char* dataFromSocket){

    char* data;
    data = new char[availableBytes+1];
    memcpy( data , dataFromSocket, availableBytes+1);
    return data;

}

void LocalSocketIpcServer::readDataFromSocket(char *newDataFromSocket, int availableBytes, int dataType){
    switch (dataType){

    case 1: //Read command
        {command = *((int*)(newDataFromSocket));

        if(debugFlag){*debugOut<< " The command is int, 4 bytes are "<<
                                  (int)(newDataFromSocket[0]) << " " <<
                                                                 (int)(newDataFromSocket[1]) << " " <<
                                                                                                (int)(newDataFromSocket[2]) << " " <<
                                                                                                                               (int)(newDataFromSocket[3]) << " ";
        }
        break;}

    case 2: //Read data length
        {dataLength = *((int*)(newDataFromSocket));
        if(debugFlag){
            *debugOut<< " Got data length= "<< dataLength <<"\n";
        }
        break;}

    case 3: //Read data set

        {if(command!=6 && command!=8){
            dataSet1Ptr = copyDataFromSocket(availableBytes,newDataFromSocket);
        }
        else{
            mode = *((int*)(newDataFromSocket));

            if(debugFlag){
                *debugOut<< " Got mode= "<< mode<<"\n" ;
            }

        }
        break;
        }

    case 4: //Read Plot settings from ViewBeast dialogue

        { if(command==6){ //Min x-axis length
            xminPtr = (double *)newDataFromSocket;
            xmin = xminPtr[0];
        }
        else if(command == 8){ // Numbers of columns
            columns = *((int*)(newDataFromSocket));
        }
        else{
            dataSet2Ptr = copyDataFromSocket(availableBytes,newDataFromSocket);
        }
        break;
        }

    case 5://Read Plot settings from ViewBeast dialogue
        {  if(command==6){  //Max x-axis length
            xmaxPtr = (double *)newDataFromSocket;
            xmax = xmaxPtr[0];

        }
        else if(command == 8) //numbers of graphs
        {
            numGraphs = *((int*)(newDataFromSocket));
        }
        else{}
        break;
        }
    default:
        break;
    }
}

void LocalSocketIpcServer::saveDataFromSocket(int numberOfRead){

    switch (numberOfRead){

    case 1:

        { readDataFromSocket(messagePtr,availableBytesFromSocket, READ_COMMAND);

        if(command == 3 || command == 4 || command == 7 || command == 42 || command == 99 || command == 98 || command == 9 || command == 12)
            conditionToExitFunction = 0;
        else
            conditionToExitFunction = 1;

        break;
        }
    case 2:
        { readDataFromSocket(messagePtr,availableBytesFromSocket, READ_DATALENGTH);
        conditionToExitFunction = 1;
        break;
        }
    case 3:
        {readDataFromSocket(messagePtr,availableBytesFromSocket, READ_DATASET_1);

        if(debugFlag){        *debugOut<< " Analysing(3) mode= "<< mode<<"\n" ;
        }
        if((command == 6 && mode == 2)|| (command == 8 && mode == 3) || command == 2)
            conditionToExitFunction = 1;
        break;
        }

    case 4:
        { readDataFromSocket(messagePtr,availableBytesFromSocket, READ_DATASET_2);
        if(debugFlag){   *debugOut<< " Analysing(4) mode= "<< mode<<"\n" ;
        }
        if((command == 6 && mode == 2)|| (command == 8 && mode == 3))
            conditionToExitFunction = 1;

        break;
        }
    case 5:
        { readDataFromSocket(messagePtr,availableBytesFromSocket, READ_DATASET_3);
        break;
        }

    default:
        {  conditionToExitFunction = 1;
        break;}
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

    if (newDataSetReady)
        buffer.write("@TYPE xy");

    //Check for invalid data and replace with 0;
    for(int i=0; i<dataLength; i++){
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

        buffer.write("\n \t");
        buffer.write(xValueChar);

        buffer.write("\t");

        yValueStr = QString::number(y[i],'g',20);
        yValueBa = yValueStr.toLocal8Bit();
        yValueChar = yValueBa.data();
        buffer.write(yValueChar);

    }


}

void LocalSocketIpcServer::readPsFileName(){

    // Read the item name from socket
    dataSet1Ptr[dataLength] = '\0';

    // Printstring length is 128, be careful
    if(dataLength > 125){ // three bytes for .ps
        printf("\nItem name for QtGrace document couldn't be read, line to long!\n"); // BZ629-3 beep removed
        delete[]dataSet1Ptr;
        return;
    }

    // set the document name
    if(dataSet1Ptr[0] != '\0'){
        qtGraceDocStrName = (string)dataSet1Ptr;
        set_docname(dataSet1Ptr);
    }

    delete[]dataSet1Ptr;
    set_ptofile(TRUE);

}

void LocalSocketIpcServer::setScalingMode()
{
    /* Autoscaling Information */
    //            autoscale_byset(int gno, int setno, int autos_type);

    /* mode 0 = no autoscale       */
    /* mode 1 = autoscale all axis */
    /* mode 2 = autoscale y axis   */


    switch(mode){
    case 2:
        { graphNo = dataLength;
        world w;
        w.xg2 = xmax;
        w.xg1 = xmin;
        set_graph_world(graphNo, w);

        /* autoscale y axis */
        autoscale_graph(graphNo, 2);
        //update_all();
        break;
        }
    case 1:
        { /* autoscale all axis - default*/
        autoscale_graph(graphNo, 3);
        //  update_all();
        break;
        }
    case 0:
        /* no autoscale */
		autoscale_onread = AUTOSCALE_NONE;

        break;
    default:
        { fprintf(stderr, "Wrong autoscale mode!\n");
        break;}
    }

    mode = 0;

}

void LocalSocketIpcServer::writeDataToTmpFile()
{

    if (writeToTmpFile){

        if(debugFlag){*debugOut<<"******START READ FROM TEMP FILE***********\n";
            *debugOut<<dataFromBuffer;
        }

        //Read data from tmp file and update QtGrace plot
        readDataFromClient(dataFromBuffer.data(),0,"plot");
        update_all();
        buffer.close();
        dataFromBuffer.clear();
        buffer.open(QIODevice::Append);
        if(debugFlag){
            *debugOut<<"******END READ FROM TEMP FILE***********\n";
        }
    }

}

void LocalSocketIpcServer::setLayoutMode(){

    //Set layout for graph: "graphNo"
    graphNo = dataLength;

    //Set QtGrace plot viewport
    view v;
    v.xv1 = 0.21;
    v.xv2 = 1.21;
    v.yv1 =0.15;
    v.yv2 = 0.85;

    //Update legend properties dialogue
    for(int iSetNo = 0; iSetNo < countNoOfDataSets; iSetNo++){
        set_legend_string(graphNo,iSetNo,get_legend_string(graphNo,iSetNo));
        QString gotCommentQString;
        char*gotComment=getcomment(graphNo, iSetNo);
        if(gotComment)gotCommentQString=gotComment;
        if( gotCommentQString.toLatin1() == fileNameStr.toLatin1())
        {
            setcomment(graphNo,iSetNo,gotComment);

        }else{
            setcomment(graphNo,iSetNo,get_legend_string(graphNo,iSetNo));}
    }


    if(mode ==1){
        //When join there will only be one graph, even ViewBeast sends it as more than one
        saveCountNoOfDataSets.replace(0,countNoOfDataSets);

    }else{
        saveCountNoOfDataSets.replace(graphNo,countNoOfDataSets);
        countNoOfDataSets = 0;

    }

    //Set graph position, i.e. numbers of columns and rows.

    switch(mode){

    case 3:{

        if(columns==0){
            rows = 1;
        }
        else {
            rows = numGraphs/columns;
            if(numGraphs%columns != 0)
                rows ++;
        }

        double offset = 0.15;
        double vgapArrangeGraph =0.2*numGraphs;
        double hgabArrangeGraph =0.2*numGraphs;

        arrange_graphs_simple(rows, columns,1, 1,offset,hgabArrangeGraph,vgapArrangeGraph);

        break;
    }
    case 2: //overlay
    {
        if(graphNo != 0)  { // Lay all on the first one.
            //#define GOVERLAY_SMART_AXES_DISABLED  0
            //#define GOVERLAY_SMART_AXES_NONE      1
            //#define GOVERLAY_SMART_AXES_X         2
            //#define GOVERLAY_SMART_AXES_Y         3
            //#define GOVERLAY_SMART_AXES_XY        4
            overlay_graphs(graphNo, 0, 4);
        }

        break;
    }
    case 1:
    {    // join, do nothing
        break;
    }
    case 0:{

        //do nothing

        break;
    }
    default:
        fprintf(stderr, "Wrong layout mode!\n");
        break;
    }
    mode = 0;
}

void LocalSocketIpcServer::sendParam(){

    //Send QtGrace document name to ViewBeast (PD file)

    int qtGraceDocStrNameLength = qtGraceDocStrName.length();

    ConnectToBeast((const char *)(&qtGraceDocStrNameLength),sizeof(int));
    ConnectToBeast(qtGraceDocStrName.data(),qtGraceDocStrNameLength);

    //Send QtGrace graph parameters settings (PD file)
    int graphParamToSendLength = 1000000;
    char *pp = new char[graphParamToSendLength];
    sprintf(pp, "\0");

    for(size_t i=0; i<graphParamToSendLength;i++){
        sprintf(pp+strlen(pp),"\0");
    }

    putparmbeast(-1,pp,TRUE);

    int paramLength = strlen(pp);
    ConnectToBeast((const char *)(&paramLength),sizeof(int));
    ConnectToBeast(pp,paramLength);
    delete[] pp;
}




void LocalSocketIpcServer::socketDisconnected() {
    if(debugFlag){*debugOut<< "socket_disconnected\n";}
}


void LocalSocketIpcServer::socketReadReady() {
    if(debugFlag){ *debugOut<< "socket_readReady\n";}
}

void LocalSocketIpcServer::socketError(QLocalSocket::LocalSocketError) {
    if(debugFlag){*debugOut<< "socket_error\n";}
}

const char* LocalSocketIpcServer::createUniqueFileName(){

    //get current date and time
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTimeString = dateTime.toString("hhmmsszzz");

    QString sendToBeast = "qtGrace"+dateTimeString;
    QByteArray fileNameBa1 = sendToBeast.toLocal8Bit();

    return fileNameBa1.data();



}
