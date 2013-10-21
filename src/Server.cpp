#include "Server.h"
#include "undo_module.h"
//#include <MainWindow.h>
#ifdef _MSC_VER
#include <windows.h>
// Sleep()
#else
#include <unistd.h>
#endif
#include <QtNetwork/QLocalSocket>

extern bool startupphase;


// initialize server
LocalSocketIpcServer::LocalSocketIpcServer(QString writeServerName, QString readServerName, QObject *parent)    :QObject(parent)
  ,socket_connected_busy(false)
  ,command(0),dataLength(0)
  ,graphNo(0)
  ,xmin(0)
  ,xmax(0)
  ,conditionToExitFunction(0)
  ,countNoOfRead(0)
  ,newDataSetReady(1)
  ,exchange_point_comma(false)
  ,new_set_nos(NULL)
  ,countNoOfReadData(0)
  ,writeToTmpFile(true)
  ,gno(0)
  ,load(0)
  ,cursource(0)
  ,countNoOfDataSets(0){

    //Read from Beast
    m_fromBeast = new QLocalServer(this);


    bool listenOK=m_fromBeast->listen(writeServerName);
    /*  if(listenOK)
        cout<< "Start the Server (listen OK)"<<endl;

    else

        cout<< "Not able to start the Server"<<endl;
*/

    m_fromBeast->setMaxPendingConnections(300);

    connect(m_fromBeast, SIGNAL(newConnection()), this, SLOT(readSocket()));

    //Buffer to save data from socket
    buffer.setBuffer(&dataFromBuffer);
    buffer.open(QIODevice::Append);

    readSocketIsLocked=false;

    //Write to Beasst
    readServer = readServerName;
    m_toBeast =   new QLocalSocket(this);
    connect(m_toBeast, SIGNAL(connected()), this, SLOT(sendDataToGrace()));
    connect(m_toBeast, SIGNAL(disconnected()), this, SLOT(socket_disconnected()));
    connect(m_toBeast, SIGNAL(readyRead()), this, SLOT(socket_readReady()));

    connect(m_toBeast, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(socket_error(QLocalSocket::LocalSocketError)));
    //qDebug()<<"Done constructor";


}

void LocalSocketIpcServer::ConnectToBeast( const char* sendParam, int sendLen) {
    qDebug() << "2) Connect to Server"+readServer;
    qDebug() << "sendParam as int="<< *(int*)(sendParam);

    m_toBeast->abort();
    m_sendParam = sendParam;
    m_paramLen = sendLen;
    m_toBeast->connectToServer(readServer);

}

LocalSocketIpcServer::~LocalSocketIpcServer() {
    qDebug() << "Server deletion";

    m_fromBeast->close();
    delete m_fromBeast;
    m_fromBeast = NULL;

    m_toBeast->abort();
    delete m_toBeast;
    m_toBeast = NULL;
}



void LocalSocketIpcServer::readSocket() {


    // qDebug()<<"readSocket() START";
    if(readSocketIsLocked){
        //   qDebug()<<"readSocket() SLEEPING because pervious invpcation has not finished yet";
        //sleep(1);
        qApp->processEvents();
        // qDebug()<<"readSocket() ENDSLEEPING because pervious invpcation has not finished yet";

        // waiting because the previous call is not done
        // all the work it should do.
    }
    readSocketIsLocked=true;

    conditionToExitFunction = 0;

    QLocalSocket *clientConnection = m_fromBeast->nextPendingConnection();

    countNoOfRead++;

    //Specifiy the amount of bytes to be read
    //qDebug()<<"countNoOfRead="<<countNoOfRead<<" command="<<command;
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

    while (clientConnection->bytesAvailable() != bytesNeeded){
        //  qDebug()<<"In loop : Needed="<<bytesNeeded<<" Available="<<clientConnection->bytesAvailable();
        clientConnection->waitForReadyRead(5000);
    }
    //qDebug()<<"Needed="<<bytesNeeded<<" Available="<<clientConnection->bytesAvailable();


    QDataStream in(clientConnection);
    
#if QT_VERSION >= 0x040700
    in.setVersion(QDataStream::Qt_4_7);
#else
    in.setVersion(QDataStream::Qt_4_0);
#endif 	





    if (clientConnection->bytesAvailable() < (int)sizeof(quint16)) {
        //   qDebug()<<"readSocket() FAIL 2";

        return;
    }

    availableBytesFromSocket = clientConnection->bytesAvailable();
    message=new char[availableBytesFromSocket+1];
    message[availableBytesFromSocket]=0; // ensure that
    // 0-terminated character strings
    // come here correctly.

    //    int receivedFromRead=clientConnection->read(message,availableBytesFromSocket);
    in.readRawData(message,availableBytesFromSocket);

    /*qDebug()<<"Reads " << receivedFromRead << " bytes";
    for(int i=0;i<receivedFromRead;i++)
      qDebug()<<"Pos "<< i <<" byte="<< (int)(message[i]) ;

    if(availableBytesFromSocket!=receivedFromRead){
        fprintf(stderr, "All available data not read!\n");
    }
*/
    

    //qDebug()<<"10:35 Afterreading bytesAvailable=" <<  clientConnection->bytesAvailable() << " bytes";


    /* read all data from socket */

    saveDataFromSocket(countNoOfRead);

    if (conditionToExitFunction) {
        //  qDebug()<<"An argument countNoOfRead " << countNoOfRead<< " for cmd="<< command;
        readSocketIsLocked=false; // should be unlocked when "returns" from
        // this function.
        return;
    }

    //Execute task from ViewBeast
    //qDebug()<<"Command No (" << command<< ")";
    switch (command){

    case 1://Read PLOT_INFO(1)
    {
        //qDebug()<<"Run Command" << command;
        buffer.write(dataSet1);
        writeToTmpFile=true;
        countNoOfRead = 0;
        break;
    }

    case 2://WRITE_DATAVEC(2)
    {//qDebug()<<"Run Command" << command;
        readXYData(dataSet1, dataSet2);
        countNoOfRead = 0;
        newDataSetReady=0;
        break;
    }
    case 12://WRITE_DATAVEC_FINISH(2)
    {   //qDebug()<<"Run Command" << command;
        countNoOfRead = 0;
        buffer.write("\n");
        newDataSetReady=1;
        countNoOfDataSets++;

        //Debug to test data from ViewBeast
        /*  QFile file("/home/nimal/test/qtgrace/qtgracegraphtest.txt");
        file.open(QIODevice::WriteOnly);
        file.write(buffer.data());
        file.close();
      */
        break;
    }

    case 3://READ_MODE(3)
    {       //qDebug()<<"Run Command" << command;
        sendParam();
        countNoOfRead = 0;
        break;
    }
    case 4://REDRAW(4)
    {//qDebug()<<"Run Command" << command;


        set_page_dimensions(733,538,1);
        //set_page_geometry()

        writeDataToTmpFile();

        //Update legend properties
        for(int igno = 0; igno < graphNo+1; igno++){
            for(int iSetNo = 0; iSetNo < saveCountNoOfDataSets[igno]; iSetNo++){
                set_legend_string(igno,iSetNo,get_legend_string(igno,iSetNo));
                setcomment(igno,iSetNo,get_legend_string(igno,iSetNo));

            }}

        countNoOfDataSets = 0;
        countNoOfRead = 0;
        writeToTmpFile=false;

        break;
    }
    case 5://PS_FILENAME(5)
    {       //qDebug()<<"Run Command" << command;

        //hdevice=1;
        readPsFileName();
        countNoOfRead = 0;
        break;
    }
    case 6://SET_SCALING_MODE(6)
    {
        //qDebug()<<"Run Command" << command;

        startupphase=true;
        int oldNoask=noask;
        noask=true; // prevent questions
        writeDataToTmpFile();
        setScalingMode();
        noask=oldNoask;
        startupphase=false;

        countNoOfRead = 0;
        writeToTmpFile=false;
        break;
    }

    case 7:{//REDRAW_AND_WRITEPS(7)
        //qDebug()<<"Run Command" << command;
        /* force a hardcopy */
        set_pagelayout(PAGE_FIXED);
        update_all();


        int oldNoask=noask;
        noask=true; // prevent questions
        do_hardcopy();
        noask=oldNoask;
        countNoOfRead = 0;
        break;
    }
    case 8://SET_LAYOUT_MODE(8)
    {       //qDebug()<<"Run Command" << command;
        startupphase=true;
        setLayoutMode();
        startupphase=false;
        //qDebug()<<"Was setLayoutMode" << command;
        countNoOfRead = 0;
        break;
    }
    case 9://Close connection to Beast(9)
    {
        qDebug()<<"Run Command" << command;
        m_fromBeast->close();
        m_toBeast->abort();
        break;
    }

    case 42://KILL_CHILD(42)
    {       //qDebug()<<"Run Command" << command;
        /* kill me */
        /* printf("got killed"); */
        exit(0);
        countNoOfRead = 0;
        break;
    }
    case 99://END_COMM(99)
    {
        //qDebug()<<"Run Command" << command;
        update_all();
        xdrawgraph();

        writeToTmpFile=true;
        countNoOfRead = 0;

        break;
    }
    case 98://TEST_CONNECTION(98)
    {
        //qDebug()<<"Run Command" << command;
        countNoOfRead = 0;
        break;
    }

    default:
    {        //qDebug()<<"INVALID COMMAND STOP" << command;

        QMessageBox::information(0,"Communication Error","Communication error: try to restart");

        exit(0);
        countNoOfRead = 0;
        break;
    }
    }
    //   qDebug()<<"Command was performed " << command;

    readSocketIsLocked=false; // should be unlocked when "returns" from
    // this function.


    //qDebug()<<"readSocket() DONE";

}

void LocalSocketIpcServer::sendDataToGrace(){

    if(socket_connected_busy){
        cerr << " It is busy already ! "<< endl;
        exit(0);
    }
    socket_connected_busy=true;

#ifdef _MSC_VER
    Sleep(1);
#else
    usleep(1000);
#endif

    std::cerr << "writeRawData "<<m_paramLen<<" bytes "<<std::endl;
    std::cerr << " data are: ";
    //if(m_len>0 && m_sendMessage [0] == 8) sleep(1);
    for(int i=0;i<20;i++){
        if (i<m_paramLen) {
            std::cerr << (int)(m_sendParam[i]) << " ";
        } else {
            std::cerr <<  "...";
        }

    }
    std::cerr << std::endl;

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    

#if QT_VERSION >= 0x040700
    out.setVersion(QDataStream::Qt_4_7);
#else
    out.setVersion(QDataStream::Qt_4_0);
#endif 

    out.writeRawData(m_sendParam,m_paramLen);
    out.device()->seek(0);
    m_toBeast->write(block);
    m_toBeast->flush();
    m_toBeast->waitForBytesWritten(20000);
    socket_connected_busy=false;
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
        //        command = newDataFromSocket[0];
        command = *((int*)(newDataFromSocket));
        //qDebug() << " The command is int, 4 bytes are "<<
        //          (int)(newDataFromSocket[0]) << " " <<
        //        (int)(newDataFromSocket[1]) << " " <<
        //      (int)(newDataFromSocket[2]) << " " <<
        //    (int)(newDataFromSocket[3]) << " ";
        break;

    case 2: //Read data length
        // dataLength = newDataFromSocket[0];
        dataLength = *((int*)(newDataFromSocket));
        break;

    case 3: //Read data set

        if(command!=6 && command!=8){
            dataSet1 = copyDataFromSocket(availableBytes,newDataFromSocket);
        }
        else{
            mode = *((int*)(newDataFromSocket));
            //qDebug() << " Got mode= "<< mode ;
        }
        break;

    case 4:
        if(command==6){
            xminPtr = (double *)newDataFromSocket;
            xmin = xminPtr[0];
        }
        else if(command == 8){
            columns = *((int*)(newDataFromSocket));
        }
        else{
            dataSet2 = copyDataFromSocket(availableBytes,newDataFromSocket);
        }
        break;

    case 5:
        if(command==6){
            xmaxPtr = (double *)newDataFromSocket;
            xmax = xmaxPtr[0];

        }
        else if(command == 8)
        {
            numGraphs = *((int*)(newDataFromSocket));
        }
        else{}
        break;

    default:
        break;
    }
}

void LocalSocketIpcServer::saveDataFromSocket(int numberOfRead){

    switch (numberOfRead){

    case 1:

        readDataFromSocket(message,availableBytesFromSocket, READ_COMMAND);


        if(command == 3 || command == 4 || command == 7 || command == 42 || command == 99 || command == 98 || command == 9 || command == 12)
            conditionToExitFunction = 0;
        else
            conditionToExitFunction = 1;

        break;

    case 2:
        readDataFromSocket(message,availableBytesFromSocket, READ_DATALENGTH);
        conditionToExitFunction = 1;
        break;

    case 3:
        readDataFromSocket(message,availableBytesFromSocket, READ_DATASET_1);

        //qDebug() << " Analysing(3) mode= "<< mode ;
        if((command == 6 && mode == 2)|| (command == 8 && mode == 3) || command == 2)
            conditionToExitFunction = 1;
        break;


    case 4:
        readDataFromSocket(message,availableBytesFromSocket, READ_DATASET_2);
        //qDebug() << " Analysing(4) mode= "<< mode ;

        if((command == 6 && mode == 2)|| (command == 8 && mode == 3))
            conditionToExitFunction = 1;

        break;

    case 5:
        readDataFromSocket(message,availableBytesFromSocket, READ_DATASET_3);
        break;


    default:
        conditionToExitFunction = 1;
        break;
    }

}

void LocalSocketIpcServer::readXYData(char* xData, char* yData){
    x = (double *) xData;
    y = (double *) yData;

    if (newDataSetReady)
        buffer.write("@TYPE xy");

    //print double pointer elements

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

        //double to string
        xValueStr = QString::number(x[i]);
        //String to char
        xValueBa = xValueStr.toLocal8Bit();
        xValueChar = xValueBa.data();
        buffer.write("\n \t");
        buffer.write(xValueChar);

        buffer.write("\t");

        yValueStr = QString::number(y[i]);
        yValueBa = yValueStr.toLocal8Bit();
        yValueChar = yValueBa.data();
        buffer.write(yValueChar);

    }


}

void LocalSocketIpcServer::readPsFileName(){
    /* Read the item name for print settings */

    /* read the item name from socket */
    dataSet1[dataLength] = '\0';

    /* printstring length is 128, be careful */
    if(dataLength > 125){ /* three bytes for .ps */
        printf("\nItem name for printer settings coudn't read, line to long!\n"); // BZ629-3 beep removed
        free(dataSet1);
        return;
    }

    /* set the printstring with the read data */
    if(dataSet1[0] != '\0'){
        printStrName = (string)dataSet1;
        set_docname(dataSet1);
    }
    free(dataSet1);
    set_ptofile(TRUE);
    /* default print is fileprint */

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
        graphNo = dataLength;
        world w;
        w.xg2 = xmax;
        w.xg1 = xmin;
        set_graph_world(graphNo, w);

        /* autoscale y axis */
        autoscale_graph(graphNo, 2);
        //update_all();
        break;

    case 1:
        /* autoscale all axis - default*/
        autoscale_graph(graphNo, 3);
        //  update_all();
        break;

    case 0:
        /* no autoscale */

        break;
    default:
        fprintf(stderr, "Wrong autoscale mode!\n");
        break;
    }

    mode = 0;

}

void LocalSocketIpcServer::


writeDataToTmpFile()
{
    //kill_all_sets(0);
    // kill_all_graphs();
    //Write to tmp file

    if (writeToTmpFile){
        // writeToTmpFile=false;
        //Write to tmp file and update graph in qtGrace
        //A text stream to save the data from beast
        QTemporaryFile   file;
        QTextStream out(&file);

        if (file.open()) {
            //Convert temporary filename to a char
            fileNameStr = file.fileName();
            fileNameBa = fileNameStr.toLocal8Bit();
            fileNameChar = fileNameBa.data();
            //qDebug()<<file.fileName();
        }
        out <<dataFromBuffer;
        file.close();
        //qDebug()<<dataFromBuffer;

        if (new_set_nos!=NULL)
        {
            delete[] new_set_nos;
            new_set_nos=NULL;
        }

        new_set_no=0;

        getdata(gno, fileNameChar,cursource,load);
        update_all();
        buffer.close();
        dataFromBuffer.clear();
        buffer.open(QIODevice::Append);

        //buffer.disconnect();
        //    byteArray.clear();
        // buffer.setBuffer(&byteArray);
        //buffer.open(QIODevice::Append);


        //xdrawgraph();
        //  file.close();
        // doCreateNew();
    }

}

void LocalSocketIpcServer::setLayoutMode(){

    graphNo = dataLength;
    view v;

    //Update legend properties

    for(int iSetNo = 0; iSetNo < countNoOfDataSets; iSetNo++){
        set_legend_string(graphNo,iSetNo,get_legend_string(graphNo,iSetNo));
        setcomment(graphNo,iSetNo,get_legend_string(graphNo,iSetNo));
    }

    saveCountNoOfDataSets[graphNo]=countNoOfDataSets;
    countNoOfDataSets = 0;

    v.xv1 = 0.21;
    v.xv2 = 1.21;
    v.yv1 =0.15;
    v.yv2 = 0.85;


    switch(mode){
    case 3:{



        if(columns==0)
            rows = 1;
        else {
            rows = numGraphs/columns;
            if(numGraphs%columns != 0)
                rows ++;
        }
        // define_arrange(rows, columns, 0, 0.05, 0.05, 0.15, 0.15, 0.75/rows, 0.75/columns);
        // arrange_graphs(rows, columns);

        double offset = 0.15;
        double vgapArrangeGraph =0.2;
        double hgabArrangeGraph =0.2;

        vgapArrangeGraph = vgapArrangeGraph*numGraphs;
        hgabArrangeGraph = hgabArrangeGraph*numGraphs;

        /*int arrange_graphs_simple(int nrows, int ncols,
            int order, int snake, double offset, double hgap, double vgap)*/
        arrange_graphs_simple(rows, columns,1, 1,offset,hgabArrangeGraph,vgapArrangeGraph);


        //update_all();
        /* arrange_graphs(graphNo, numGraphs,
                           rows, columns, 0 ,0,
                           0.15,0.15,0.15,0.15,
                           0.05, 0.05,
                           0, 0);
*/
        break;
    }
    case 2: //overlay4
    {
        if(graphNo != 0)  { // Lay all on the first one.
            //#define GOVERLAY_SMART_AXES_DISABLED  0
            //#define GOVERLAY_SMART_AXES_NONE      1
            //#define GOVERLAY_SMART_AXES_X         2
            //#define GOVERLAY_SMART_AXES_Y         3
            //#define GOVERLAY_SMART_AXES_XY        4
            overlay_graphs(graphNo, 0, 4);
        }

        set_graph_viewport(graphNo, v);

        break;
    }
    case 1:
    {    // join, do nothing
        set_graph_viewport(graphNo, v);
        break;
    }
    case 0:{

        set_graph_viewport(graphNo, v);


        break;
    }
    default:
        fprintf(stderr, "Wrong layout mode!\n");
        break;
    }
    mode = 0;
}

void LocalSocketIpcServer::sendParam(){
    QTemporaryFile  parFile;

    if (parFile.open()) {
        //Convert temporary filename to a char
        fileNameStr = parFile.fileName();
        fileNameBa = fileNameStr.toLocal8Bit();
        fileNameChar = fileNameBa.data();
        //qDebug()<<parFile.fileName();
    }

    parFile.close();

    FILE* pFile = fopen(fileNameChar, "w");
    putparms(-1, pFile, TRUE);

    fclose(pFile);
    parFile.open();



    QByteArray paramToSend = parFile.readAll();

    parFile.close();

    printStrNameLength = printStrName.length();
    int printStrNameLengthToSend=printStrNameLength;

    const char *printStrNameToSend = printStrName.data();

    paramLength = paramToSend.length();
    int paramLengthToSend=paramLength;

    ConnectToBeast((const char *)(&printStrNameLengthToSend),sizeof(int));
    ConnectToBeast(printStrNameToSend,printStrNameLength);

    ConnectToBeast((const char *)(&paramLengthToSend),sizeof(int));
    ConnectToBeast(paramToSend.data(),paramLength);

}


int LocalSocketIpcServer::uniread(FILE *fp, int load_type, char *label)
{
    int nrows, ncols, nncols, nscols, nncols_req;
    int *formats = NULL;
    int breakon, readerror;
    ss_data ssd;
    char *s, tbuf[128];
    char *linebuf=NULL;
    int linelen=0;   /* a misleading name ... */
    int linecount;

    //we have to reserve some memory here for input-data
    int * maj_new_nrs=NULL;
    int nr_count=0;

    linecount = 0;
    readerror = 0;
    nrows = 0;

    breakon = TRUE;

    memset(&ssd, 0, sizeof(ssd));

    while (read_long_line(fp, &linebuf, &linelen) == RETURN_SUCCESS)
    {
        linecount++;
        s = linebuf;
        while (*s == ' ' || *s == '\t' || *s == '\n')
        {
            s++;
        }
        /* skip comments */
        if (*s == '#') {
            parse_qtGrace_Additions(s);
            continue;
        }
        if (exchange_point_comma && (*s==',' || *s=='.'))
        {
            if (*s=='.') *s=',';
            else *s='.';
        }
        /*   command     end-of-set      EOL   */
        if (*s == '@' || *s == '&' || *s == '\0')
        {
            /* a data break line */
            if (breakon != TRUE)
            {
                /* free excessive storage */
                realloc_ss_data(&ssd, nrows);

                new_set_no=0;
                if (new_set_nos!=NULL)
                {
                    delete[] new_set_nos;
                    new_set_nos=NULL;
                }

                /* store accumulated data in set(s) */
                if (store_data(&ssd, load_type, label) != RETURN_SUCCESS)
                {
                    xfree(linebuf);
                    return RETURN_FAILURE;
                }

                append_to_storage(&nr_count,&maj_new_nrs,new_set_no,new_set_nos);

                /* reset state registers */
                nrows = 0;
                readerror = 0;
                breakon = TRUE;
            }
            if (*s == '@')
            {
                /*int retval = */scanner(s + 1);
                /*cout << "s=" << s << " | " << retval << endl;*/
                continue;
            }
        }
        else
        {
            if (breakon)
            {
                /* parse the data line */
                XCFREE(formats);
                if (parse_ss_row(s, &nncols, &nscols, &formats) != RETURN_SUCCESS)
                {
                    errmsg("Can't parse data");
                    xfree(linebuf);
                    return RETURN_FAILURE;
                }

                if (load_type == LOAD_SINGLE)
                {
                    nncols_req = settype_cols(curtype);
                    if (nncols_req <= nncols)
                    {
                        nncols = nncols_req;
                    }
                    else if (nncols_req == nncols + 1)
                    {
                        /* X from index, OK */
                        ;
                    }
                    else
                    {
                        errmsg("Column count incorrect");
                        xfree(linebuf);
                        return RETURN_FAILURE;
                    }
                }

                ncols = nncols + nscols;

                /* init the data storage */
                if (init_ss_data(&ssd, ncols, formats) != RETURN_SUCCESS)
                {
                    errmsg("Malloc failed in uniread()");
                    xfree(linebuf);
                    return RETURN_FAILURE;
                }

                breakon = FALSE;
            }
            if (nrows % BUFSIZE == 0)
            {
                if (realloc_ss_data(&ssd, nrows + BUFSIZE) != RETURN_SUCCESS)
                {
                    errmsg("Malloc failed in uniread()");
                    free_ss_data(&ssd);
                    xfree(linebuf);
                    return RETURN_FAILURE;
                }
            }

            if (insert_data_row(&ssd, nrows, s) != RETURN_SUCCESS)
            {
                sprintf(tbuf, "Error parsing line %d, skipped", linecount);
                errmsg(tbuf);
                readerror++;
                if (readerror > MAXERR)
                {
                    if (yesno("Lots of errors, abort?", NULL, NULL, NULL))
                    {
                        free_ss_data(&ssd);
                        xfree(linebuf);
                        return RETURN_FAILURE;
                    }
                    else
                    {
                        readerror = 0;
                    }
                }
            }
            else
            {
                nrows++;
            }
        }
    }

    if (nrows > 0)
    {
        /* free excessive storage */
        realloc_ss_data(&ssd, nrows);

        new_set_no=0;
        if (new_set_nos!=NULL)
        {
            delete[] new_set_nos;
            new_set_nos=NULL;
        }

        /* store accumulated data in set(s) */
        if (store_data(&ssd, load_type, label) != RETURN_SUCCESS)
        {
            xfree(linebuf);
            return RETURN_FAILURE;
        }

        append_to_storage(&nr_count,&maj_new_nrs,new_set_no,new_set_nos);
    }

    xfree(linebuf);
    xfree(formats);

    new_set_no=nr_count;
    if (new_set_nos!=NULL) delete[] new_set_nos;
    new_set_nos=maj_new_nrs;
    maj_new_nrs=NULL;

    return RETURN_SUCCESS;
}

int LocalSocketIpcServer::read_long_line(FILE * fp, char **linebuf, int *buflen)
{
    char *cursor;
    int  available;
    int  nbread, retval;

    cursor    = *linebuf;
    available = *buflen;
    retval    = RETURN_FAILURE;
    do {
        /* do we have enough space to store the characters ? */
        if (available < 2) {
            if (expand_line_buffer(linebuf, buflen, &cursor)
                    != RETURN_SUCCESS) {
                return RETURN_FAILURE;
            }
        }
        available = (int)(*linebuf-cursor) + *buflen;

        /* read as much as possible */
        if (grace_fgets(cursor, available, fp) == NULL) {
            return retval;
        }
        nbread = strlen(cursor);
        if (nbread < 1) {
            return retval;
        } else {
            retval = RETURN_SUCCESS;
        }

        /* prepare next read */
        cursor    += nbread;
        available -= nbread;

    } while (*(cursor - 1) != '\n');

    return retval;
}

int LocalSocketIpcServer::getdata(int gno, char *fn, int src, int load_type)
{
    //  kill_all_graphs();
    FILE *fp;
    int retval;
    int save_version, cur_version;
    char fn2[1024];
#ifdef WINDOWS_SYSTEM
    int p,p2=0;
    for (p=0;p<=strlen(fn);p++)
    {
        if (fn[p]=='\\')
        {
            fn2[p2]=fn[p];
            p2++;
            fn2[p2]=fn[p];
            p2++;
        }
        else
        {
            fn2[p2]=fn[p];
            p2++;
        }
    }
#else
    strcpy(fn2,fn);
#endif
    fp = grace_openr(fn2, src);
    if (fp == NULL) {
        return RETURN_FAILURE;
    }

    save_version = get_project_version();
    set_project_version(0);

    set_parser_gno(gno);

    retval = uniread(fp, load_type, fn2);

    grace_close(fp);

    cur_version = get_project_version();
    if (cur_version != 0) {
        /* a complete project */
        postprocess_project(cur_version);
        // autoscale_graph(gno, autoscale_onread); //2013-07-03 disable - Nimal Kailasanathan

    } else if (load_type != LOAD_BLOCK) {
        /* just a few sets */
        //autoscale_graph(gno, autoscale_onread); //2013-07-03 disable - Nimal Kailasanathan
    }
    set_project_version(save_version);

    return retval;
}

void LocalSocketIpcServer::parse_qtGrace_Additions(char * s)
{
    char c='\0';
    int read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: %c",&c);
    int data[3];
    char sdata[128];
    if (read_data>0)
    {
        //cout << "s=#" << s << "# --> read_data=" << read_data << " c=" << c << endl;
        switch (c)
        {
        case 'B':
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: BOX %d ROT %d",data,data+1);
            if (read_data>0)
            {
                if (is_valid_box(data[0]))
                    boxes[data[0]].rot=data[1];
            }
            break;
        case 'E':
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: ELLIPSE %d ROT %d",data,data+1);
            if (read_data>0)
            {
                if (is_valid_ellipse(data[0]))
                    ellip[data[0]].rot=data[1];
            }
            break;
        case 'G':
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: G %d S %d POLYGONEBASESET %d",data,data+1,data+2);
            if (read_data>0)
            {
                if (is_valid_setno(data[0],data[1]))
                    g[data[0]].p[data[1]].polygone_base_set=data[2];
            }
            break;
        case 'Q':
            read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: QTFONT_RESET %d",data);
            if (read_data>0)
            {
                if (data>0)
                {
                    stdFontList.clear();
                }
            }
            else
            {
                read_data=sscanf(s,"#QTGRACE_ADDITIONAL_PARAMETER: QTFONT \"%s\"",sdata);
                if (read_data>0)
                {
                    QFont stfont;
                    stfont.fromString(QString(sdata));
                    (void)addFontToDatabase(stfont);
                }
            }
            break;
        }
    }
}
/*
 * expand a line buffer
 */

int LocalSocketIpcServer::expand_line_buffer(char **adrBuf, int *ptrSize, char **adrPtr)
{
    char *newbuf;
    int   newsize;

    newsize = *ptrSize + CHUNKSIZE;
    newbuf = (char*)xmalloc(newsize);
    if (newbuf == 0) {
        return RETURN_FAILURE;
    }

    if (*ptrSize == 0) {
        /* this is the first time through */
        if (adrPtr) {
            *adrPtr = newbuf;
        }
    } else {
        /* we are expanding an existing line */
        strncpy(newbuf, *adrBuf, *ptrSize);
        if (adrPtr) {
            *adrPtr += newbuf - *adrBuf;
        }
        xfree(*adrBuf);
    }

    *adrBuf  = newbuf;
    *ptrSize = newsize;

    return RETURN_SUCCESS;
}

void LocalSocketIpcServer::doCreateNew()
{
    int * gnos=new int[2];
    gnos[1]=gnos[0]=number_of_graphs();
    set_graph_active(number_of_graphs());
    GraphsCreated(1,gnos,UNDO_COMPLETE);
    delete[] gnos;
}


void LocalSocketIpcServer::socket_disconnected() {
    //qDebug() << "socket_disconnected";
}


void LocalSocketIpcServer::socket_readReady() {
    //qDebug() << "socket_readReady";
}

void LocalSocketIpcServer::socket_error(QLocalSocket::LocalSocketError) {
    //qDebug() << "socket_error";
}
