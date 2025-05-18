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

#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QTcpSocket>
#include <strstream>
#include <QProcess>
#include "communicationsProtocol.h"

#define  DEBUGDETAILS (QString("\nFile: ")+ __FILE__+ QString("\nFunction: ")+ __FUNCTION__ + QString("\nLine: ") + QString::number(__LINE__))


QT_BEGIN_NAMESPACE
class QComboBox;
class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;
class QNetworkSession;
class QTextEdit;
QT_END_NAMESPACE

class Client : public QDialog
{
    Q_OBJECT

public:
    Client(QWidget *parent = 0);

    //! Save all plot commands to a stream
    void savePlotExample();


    ~Client();
private slots:

    //! Start communication with QtGrace in order to send data
    void startCommunicationToRead();

    //! Start communication with QtGrace in order to read data
    void startCommunicationToSend();

    //! Start QtGrace
    void startQtGrace();

    //! File browser to save  path to QtGrace
   void pathToQtGrace();

   //!If the user has entered a path the start QtGrace button will be enabled.
   void enableStartQtGraceButton();

private:

    //!GUI variables
    QLabel *dataFromQtGraceLabelPtr_m;
    QLabel *hostAddressLabelPtr_m;
    QLabel *writePortLabelPtr_m;
    QLabel *readPortLabelPtr_m;
    QLabel *hostAddressPtr_m;
    QLabel *writePortLineLabelPtr_m;
    QLabel *readPortLineLabelPtr_m;
    QLabel *statusLabelPtr_m;
    QPushButton *browseQtGraceButtonPtr_m;
    QPushButton *sendToQtGraceButtonPtr_m;
    QPushButton *startQtGraceButtonPtr_m;
    QPushButton *readFromQtGraceButtonPtr_m;
    QPushButton *quitButtonPtr_m;
    QDialogButtonBox *buttonBoxPtr_m;
    QTextEdit *dataFromQtGraceTextEditPtr_m;
    QLineEdit *pathToQtGraceLineEdit_m;

    //! Protocol to communicate with QtGrace
    CommunicationsProtocol *communicationsProtocol_m;

    //! Start QtGrace
    QProcess *qtGraceProcessPtr_m;

    //! Path to QtGrace
    QString qtGracePath_m;

    //! Check if the connection to QtGrace is valid
    bool checkQtGraceConnection();
};

#endif
