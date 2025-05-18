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


#include "client.h"

#include <QtWidgets>
#include <QtNetwork>
#include <iostream>
#include <cassert>
#include <ctime>
#include <cstring>
#include <vector>
#include <QApplication>

Client::Client(QWidget *parent) :
    QDialog(parent)
  ,qtGraceProcessPtr_m(NULL)
  ,communicationsProtocol_m(NULL)

{

    pathToQtGraceLineEdit_m = new QLineEdit();

    dataFromQtGraceLabelPtr_m = new QLabel(tr("Data From QtGrace:"));

    hostAddressLabelPtr_m = new QLabel(tr("QtGrace Host Address:"));
    writePortLabelPtr_m = new QLabel(tr("QtGrace port to write:"));
    readPortLabelPtr_m = new QLabel(tr("QtGrace port to read:"));

    hostAddressPtr_m = new QLabel(tr("127.0.0.1"));//LocalHost

    writePortLineLabelPtr_m = new QLabel;
    readPortLineLabelPtr_m = new QLabel;
    dataFromQtGraceTextEditPtr_m = new QTextEdit;
    hostAddressPtr_m = new QLabel;

    hostAddressLabelPtr_m->setBuddy(hostAddressPtr_m);
    writePortLabelPtr_m->setBuddy(writePortLineLabelPtr_m);
    readPortLabelPtr_m->setBuddy(readPortLineLabelPtr_m);

    connect(pathToQtGraceLineEdit_m, SIGNAL(textChanged(QString)),
            this, SLOT(enableStartQtGraceButton()));

    statusLabelPtr_m = new QLabel(tr("Path to QtGrace:"));

    browseQtGraceButtonPtr_m = new QPushButton(tr("Browse"));
    browseQtGraceButtonPtr_m->setDefault(true);
    browseQtGraceButtonPtr_m->setEnabled(true);


    startQtGraceButtonPtr_m = new QPushButton(tr("Start QtGrace"));
    startQtGraceButtonPtr_m->setDefault(true);
    startQtGraceButtonPtr_m->setEnabled(false);

    sendToQtGraceButtonPtr_m = new QPushButton(tr("Send Plot"));
    sendToQtGraceButtonPtr_m->setDefault(true);
    sendToQtGraceButtonPtr_m->setEnabled(false);

    readFromQtGraceButtonPtr_m = new QPushButton(tr("Read Plot"));
    readFromQtGraceButtonPtr_m->setDefault(true);
    readFromQtGraceButtonPtr_m->setEnabled(false);

    quitButtonPtr_m = new QPushButton(tr("Quit"));

    buttonBoxPtr_m = new QDialogButtonBox;

    buttonBoxPtr_m->addButton(startQtGraceButtonPtr_m, QDialogButtonBox::ActionRole);
    buttonBoxPtr_m->addButton(sendToQtGraceButtonPtr_m, QDialogButtonBox::ActionRole);
    buttonBoxPtr_m->addButton(readFromQtGraceButtonPtr_m, QDialogButtonBox::ActionRole);

    buttonBoxPtr_m->addButton(quitButtonPtr_m, QDialogButtonBox::RejectRole);

    communicationsProtocol_m = new CommunicationsProtocol(this);

    writePortLineLabelPtr_m->setText(communicationsProtocol_m->getReadPortPtr_m());
    readPortLineLabelPtr_m->setText(communicationsProtocol_m->getWritePortPtr_m());
    hostAddressPtr_m->setText(communicationsProtocol_m->getHostAddressPtr_m());


    connect(browseQtGraceButtonPtr_m, SIGNAL(clicked()),
            this, SLOT(pathToQtGrace()));

    connect(startQtGraceButtonPtr_m, SIGNAL(clicked()),
            this, SLOT(startQtGrace()));

    connect(sendToQtGraceButtonPtr_m, SIGNAL(clicked()),
            this, SLOT(startCommunicationToSend()));

    connect(readFromQtGraceButtonPtr_m, SIGNAL(clicked()),
            this, SLOT(startCommunicationToRead()));


    connect(quitButtonPtr_m, SIGNAL(clicked()), this, SLOT(close()));

    QGridLayout *mainLayout = new QGridLayout;



    mainLayout->addWidget(statusLabelPtr_m, 0, 0, 1,1);

    mainLayout->addWidget(pathToQtGraceLineEdit_m, 0,1 , 1,1);
    mainLayout->addWidget(browseQtGraceButtonPtr_m, 0, 2, 1, 2);

    mainLayout->addWidget(hostAddressLabelPtr_m, 1, 0);
    mainLayout->addWidget(hostAddressPtr_m, 1, 1);
    mainLayout->addWidget(writePortLabelPtr_m, 2, 0);
    mainLayout->addWidget(writePortLineLabelPtr_m, 2, 1);
    mainLayout->addWidget(readPortLabelPtr_m, 3, 0);
    mainLayout->addWidget(readPortLineLabelPtr_m, 3, 1);
    mainLayout->addWidget(dataFromQtGraceLabelPtr_m, 4, 0, 1, 2);

    mainLayout->addWidget(dataFromQtGraceTextEditPtr_m, 5, 0, 1, 2);
    mainLayout->addWidget(buttonBoxPtr_m, 6, 0, 1, 2);

    setLayout(mainLayout);

    setWindowTitle(tr("QtGrace Client"));
}

Client::~Client()
{
    if(qtGraceProcessPtr_m != NULL)
        qtGraceProcessPtr_m->close();
}

void Client::startCommunicationToSend(){

    if(checkQtGraceConnection()){

        double*xData=new double[10];
        double*yData=new double[10];

        for(int i=0;i<10;i++){
            xData[i] = i;
            yData[i] = i;
        }

        communicationsProtocol_m->sendDataSetToQtGrace(xData,yData);

        delete [] xData;
        delete [] yData;

    }else{
        sendToQtGraceButtonPtr_m->setEnabled(false);
        readFromQtGraceButtonPtr_m->setEnabled(false);
    }

}

void Client::enableStartQtGraceButton()
{
    startQtGraceButtonPtr_m->setEnabled(!pathToQtGraceLineEdit_m->text().isEmpty());
    qtGracePath_m = pathToQtGraceLineEdit_m->text();

}

void Client::startCommunicationToRead(){

    if(checkQtGraceConnection()){
        QString dataFromQtGrace = communicationsProtocol_m->readPlotParametersFromQtGrace();
        dataFromQtGraceTextEditPtr_m->setText(dataFromQtGrace);
    }else{
        sendToQtGraceButtonPtr_m->setEnabled(false);
        readFromQtGraceButtonPtr_m->setEnabled(false);
    }
}

void Client::pathToQtGrace(){
    QStringList fileNames ;

#ifdef _MSC_VER
    fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"),"/path/to/file/",tr("exe Files (*.exe)"));

#else
    fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"),"/path/to/file/");
#endif

    if(!fileNames.isEmpty()){
        qtGracePath_m = fileNames[0];
        pathToQtGraceLineEdit_m->setText(qtGracePath_m);
    }
}

void Client::startQtGrace()
{


    if(qtGraceProcessPtr_m == NULL){

        if(qtGraceProcessPtr_m != NULL){
            qtGraceProcessPtr_m->close();
            qtGraceProcessPtr_m->kill();
            qtGraceProcessPtr_m == NULL;
        }

        //Start QtGrace
        QString program = qtGracePath_m;
        QStringList arguments;

        arguments << "-ServerMode";
        arguments << communicationsProtocol_m->getReadPortPtr_m();
        arguments << communicationsProtocol_m->getWritePortPtr_m();

        qtGraceProcessPtr_m = new QProcess;
        qtGraceProcessPtr_m->start(program, arguments);

        sendToQtGraceButtonPtr_m->setEnabled(true);
        readFromQtGraceButtonPtr_m->setEnabled(true);

        if (!qtGraceProcessPtr_m->waitForStarted()) {

            QMessageBox::warning(this,"Wrong path to QtGrace","QtGrace not found");

            if(qtGraceProcessPtr_m!=NULL){
                qtGraceProcessPtr_m->close();
                qtGraceProcessPtr_m->kill();
                qtGraceProcessPtr_m = NULL;
            }

            sendToQtGraceButtonPtr_m->setEnabled(false);
            readFromQtGraceButtonPtr_m->setEnabled(false);
        }

    }else{

        checkQtGraceConnection();

    }

}

bool Client::checkQtGraceConnection(){

    if(qtGraceProcessPtr_m->state()!=2){

        QMessageBox::warning(this,"Communication Error",
                             "Communication to QtGracee could not be estabilsed try to restart the clientt");

        sendToQtGraceButtonPtr_m->setEnabled(false);
        readFromQtGraceButtonPtr_m->setEnabled(false);
        startQtGraceButtonPtr_m->setEnabled(false);

        return 0;

    }else{
        return 1;
    }
}
