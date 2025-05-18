#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QApplication>
#include <QtCore>

#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QListWidget>

#include "compressdecompress.h"

#define STD_SPACING 2
#define STD_MARGIN 2

using namespace std;

class DropZone:public QLabel
{
    Q_OBJECT
public:
    DropZone(QWidget *parent = 0);
public slots:
virtual void paintEvent( QPaintEvent * event );
virtual void dragEnterEvent( QDragEnterEvent * event );
virtual void dropEvent( QDropEvent * event );
signals:
void newFilesEntered(QStringList list);
};

class MainWindow : public QDialog
{
    Q_OBJECT
private:
    QMenuBar * mnuBar;
    QMenu * mnuFile;
    QAction * actOpen,*actSave,*actExit;
    class DropZone * dz;
    QListWidget * lst;
    QLabel * lblList;
    QLabel * lblResult;
    QLabel * lblCompLevel;
    QSpinBox * spnLevel;
    QComboBox * cmbResult;
    QGroupBox * grpCompr;
    QRadioButton * radCompress;
    QRadioButton * radDeCompress;
    QHBoxLayout * layout1;
    QGroupBox * grpType;
    QRadioButton * radWhole;
    QRadioButton * radProject;
    QRadioButton * radExternal;
    QHBoxLayout * layout2;
    QGridLayout * layout;
    QPushButton * cmdStart;
    QPushButton * cmdExit;
public:
    QStringList selectedFiles;
    bool toCompress;
    int CompressionLevel;
    int InputType;
    int OutputType;

    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void ReadInputs(void);
    void doStart(void);
    void doExit(void);
    void doOpen(void);
    void doSave(void);
    void newFilesEntered(QStringList files);
};

#endif // MAINWINDOW_H
