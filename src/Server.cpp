#include "Server.h"
#include <QtNetwork/QLocalSocket>


// initialize server
LocalSocketIpcServer::LocalSocketIpcServer(QString writeServerName, QString readServerName, QObject *parent)
    :QObject(parent) {
    m_fromBeast = new QLocalServer(this);
    m_toBeast = new QLocalServer(this);

    if (!m_fromBeast->listen(writeServerName)) {
        qDebug() << "Not able to start the Server";
    }

    if (!m_toBeast->listen(readServerName)) {
        qDebug() << "Not able to start the Server";
    }

    m_data = "Data From Server Read: Hej Nimal";
    connect(m_toBeast, SIGNAL(newConnection()), this, SLOT(sendData()));
    connect(m_fromBeast, SIGNAL(newConnection()), this, SLOT(readSocket()));

//Buffer to save data from socket
    buffer.setBuffer(&byteArray);
    buffer.open(QIODevice::Append);

}

LocalSocketIpcServer::~LocalSocketIpcServer() {}

//Read from client/Beast
void LocalSocketIpcServer::readSocket() {

    QLocalSocket *clientConnection = m_fromBeast->nextPendingConnection();

    while (clientConnection->bytesAvailable() < (int)sizeof(quint32))
        clientConnection->waitForReadyRead();

    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));

    QDataStream in(clientConnection);
    in.setVersion(QDataStream::Qt_4_0);
    if (clientConnection->bytesAvailable() < (int)sizeof(quint16)) {
        return;
    }

//A text stream to save the data from beast
    QTextStream out(&file);

    if (file.open()) {
        //Convert temporary filename to a char
        str1 = file.fileName();
        ba = str1.toLocal8Bit();
        filename = ba.data();
        qDebug()<<file.fileName();
    }

//Read data from socket
    in >> message;






    //  if (readFirstMessage)
    // command = message[0];

    countNoOfRead++;

    switch (countNoOfRead){

    //Read the command and determine how many times to continuous read

    case 1:
        command = message[0];
        break;
    case 2:
        dataLength = message[0];
        break;
    case 3:
        socketData1 = message;
        break;
    case 4:
        socketData2 = message;
        break;
    default:  break;

    }


//sockRead case 1: read parameters
    if (countNoOfRead == 3 && command == 1){
            qDebug()<<"Read parameters";
            countNoOfRead =0;
            buffer.write(socketData1);
            countNoOfRead = 0;
           }

 //sockRead case 2: read XYdata set
    if (countNoOfRead == 4 && command == 2){

        x = (double *) socketData1;
        y = (double *) socketData2;

        buffer.write("@TYPE xy");
        //print double pointer elements

        for(int i=0; i<7; i++){

            if(qIsNaN(x[i]))
                x[i]=0;

            if(qIsNaN(y[i]))
                y[i]=0;

            //double to string
            str2 = QString::number(x[i]);
            //String to char
            ba2 = str2.toLocal8Bit();
            doubletest = ba2.data();
            buffer.write("\n \t");
            buffer.write(doubletest);

            buffer.write("\t");

            str2 = QString::number(y[i]);
            ba2 = str2.toLocal8Bit();
            doubletest = ba2.data();
            buffer.write(doubletest);
                    }
            buffer.write("\n");
            countNoOfRead = 0;

        }


/*
case 6:
            /* Autoscaling Information */
//            autoscale_byset(int gno, int setno, int autos_type);

    //sockRead case 99: end transmission
    if (countNoOfRead == 1 && command == 6 ){
        out <<byteArray;
        buffer.close();
        file.close();
        qDebug()<<byteArray;

        if (new_set_nos!=NULL)
        {
            delete[] new_set_nos;
            new_set_nos=NULL;
        }
        new_set_no=0;
        ////    int load = 0;
        ////    int graphno = 0;
        ////    int cursource = 0;
        ////        //cout << "A3 " << graphno << " " << filename << " " << cursource << " " << load << endl;
        getdata(0, filename,0,0);

        update_all();
        xdrawgraph();
        file.close();
        // mainWin->mainArea->completeRedraw();

    }



}
//    //////////////////////////////////////







////////////////////


//Send data to client/Beast
void LocalSocketIpcServer::sendData()
{

    QByteArray block;
    QDataStream out(&block, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;

    // out << m_data.at(qrand() % m_data.size());
    out << m_data;


    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    QLocalSocket *clientConnection = m_toBeast->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));

    clientConnection->write(block);
    clientConnection->flush();
    clientConnection->disconnectFromServer();
}

void LocalSocketIpcServer::readToQtGrace(){


}


////////////////////////////////////////////////////////////////////////////////////
//LOCAL FUNCTIONS
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
    } else if (load_type != LOAD_BLOCK) {
        /* just a few sets */
        autoscale_graph(gno, autoscale_onread);
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

