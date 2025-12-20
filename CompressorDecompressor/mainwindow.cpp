#include "mainwindow.h"


DropZone::DropZone(QWidget *parent):QLabel(parent)
{
this->setAcceptDrops(true);
}

void DropZone::paintEvent( QPaintEvent * event )
{
    (void) event;
QPainter paint(this);
QPen pen;
pen.setWidth(2);
pen.setColor(Qt::black);
paint.setPen(pen);
int w,h,d,r;
w=this->width();
h=this->height();
d=(w<h?w:h);
r=(int)(0.15*d);
d=(int)(0.1*d);
paint.drawRoundedRect(d,d,w-2*d,h-2*d,r,r);
paint.drawText(QRect(0,0,w,h),Qt::AlignHCenter|Qt::AlignVCenter,tr("Drop Files Here!"));
paint.end();
}

void DropZone::dragEnterEvent( QDragEnterEvent * event )
{
    if (event->mimeData()->hasUrls())
    event->accept();
}

void DropZone::dropEvent( QDropEvent * event )
{
QStringList list;
list.clear();
    if (event->mimeData()->hasUrls())
    {
        for (int i=0;i<event->mimeData()->urls().length();i++)
        {
        //qDebug() << "File " << i << ": " << event->mimeData()->urls().at(i);
        list << event->mimeData()->urls().at(i).toLocalFile();
        }
            if (list.length()>0)
            emit(newFilesEntered(list));
    }
}

MainWindow::MainWindow(QWidget *parent):QDialog(parent)
{
    setWindowTitle(tr("QtGrace File Compressor/DeCompressor"));

    grpCompr=new QGroupBox(tr("Action"),this);
    grpType=new QGroupBox(tr("File-Type"),this);

    layout1=new QHBoxLayout(grpCompr);
    layout2=new QHBoxLayout(grpType);
    layout=new QGridLayout(this);

    /*layout1->setMargin(STD_MARGIN);
    layout2->setMargin(STD_MARGIN);
    layout->setMargin(STD_MARGIN);*/
    layout1->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout2->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout->setContentsMargins(STD_MARGIN,STD_MARGIN,STD_MARGIN,STD_MARGIN);
    layout1->setSpacing(STD_SPACING);
    layout2->setSpacing(STD_SPACING);
    layout->setSpacing(STD_SPACING);

    mnuBar=new QMenuBar(this);
    mnuFile=new QMenu(mnuBar);

    actOpen=new QAction(tr("Open File(s)"),this);
    actSave=new QAction(tr("Save File(s)"),this);
    actExit=new QAction(tr("Exit"),this);

    mnuFile->addAction(actOpen);
    mnuFile->addAction(actSave);
    mnuFile->addSeparator();
    mnuFile->addAction(actExit);

    mnuBar->addMenu(mnuFile);

    dz=new DropZone(this);

    lblList=new QLabel(tr("List Of Files:"),this);
    lst=new QListWidget(this);
    lst->setSelectionMode(QAbstractItemView::MultiSelection);

    lblResult=new QLabel(tr("Output-File-Scheme:"),this);
    cmbResult=new QComboBox(this);
    cmbResult->addItem(tr("Add Compr/DeCompr To File Name(s)"));
    cmbResult->addItem(tr("Overwrite File(s)"));
    cmbResult->addItem(tr("Ask For Target File(s)"));

    radCompress=new QRadioButton(tr("Compress"),grpCompr);
    radDeCompress=new QRadioButton(tr("De-Compress"),grpCompr);
    radCompress->setChecked(true);
    layout1->addWidget(radCompress);
    layout1->addWidget(radDeCompress);
    grpCompr->setToolTip(tr("What to do with the file(s).\nCompress it or decompress it."));
    grpCompr->setLayout(layout1);

    radWhole=new QRadioButton(tr("Whole File"),grpType);
    radProject=new QRadioButton(tr("QtGrace Project File"),grpType);
    radExternal=new QRadioButton(tr("External File"),grpType);
    radProject->setChecked(true);
    layout2->addWidget(radProject);
    layout2->addWidget(radWhole);
    layout2->addWidget(radExternal);
    grpType->setToolTip(tr("File-Type sets how the compression is used:\n'Whole file' (de)compresses the file as it is as a whole.\n'Project file' means that only the data is (de)compressed, the header stays uncompressed."));
    grpType->setLayout(layout2);

    lblCompLevel=new QLabel(tr("Compression-Level:"),this);
    spnLevel=new QSpinBox(this);
    spnLevel->setToolTip(tr("Level of compression. '-1' means the default compression of zlib."));
    spnLevel->setRange(-1,9);
    spnLevel->setValue(3);//default=-1, seems to be Level 6 --> very slow, not much more compression

    cmdStart=new QPushButton(tr("Start"),this);
    cmdExit=new QPushButton(tr("Exit"),this);

    layout->addWidget(mnuBar,0,0,1,2);
    layout->addWidget(dz,1,0,2,1);
    layout->addWidget(lblList,1,1,1,1);
    layout->addWidget(lst,2,1,1,1);
    layout->addWidget(grpCompr,3,0,1,2);
    layout->addWidget(lblCompLevel,4,0,1,1);
    layout->addWidget(spnLevel,4,1,1,1);
    layout->addWidget(grpType,5,0,1,2);
    layout->addWidget(lblResult,6,0,1,1);
    layout->addWidget(cmbResult,6,1,1,1);
    layout->addWidget(cmdStart,7,0,1,1);
    layout->addWidget(cmdExit,7,1,1,1);

    connect(cmdExit,SIGNAL(clicked()),SLOT(doExit()));
    connect(cmdStart,SIGNAL(clicked()),SLOT(doStart()));
    connect(actOpen,SIGNAL(triggered()),SLOT(doOpen()));
    connect(actSave,SIGNAL(triggered()),SLOT(doSave()));
    connect(dz,SIGNAL(newFilesEntered(QStringList)),this,SLOT(newFilesEntered(QStringList)));

    this->setLayout(layout);
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::ReadInputs(void)
{
    selectedFiles.clear();

    QList<QListWidgetItem *> sel_list=lst->selectedItems();

    for (int i=0;i<sel_list.length();i++)
    {
    selectedFiles << sel_list.at(i)->text();
    }

    toCompress=(radCompress->isChecked()?true:false);
    CompressionLevel=spnLevel->value();
    InputType=(radWhole->isChecked()?WHOLE_FILE:(radProject->isChecked()?QTGRACE_PROJECT_FILE:EXTERNAL_FILE));
    OutputType=cmbResult->currentIndex();
}

void MainWindow::doStart(void)
{
    this->setCursor(Qt::WaitCursor);
    ReadInputs();
    if (selectedFiles.length()<=0)
    {
    (void)QMessageBox::information(this,tr("Warning"),tr("Please select at least one file to compress/decompress"),QMessageBox::Ok);
    goto end_of_start;
    }


        /*qDebug() << "Selected Files:";
        for (int i=0;i<selectedFiles.length();i++)
        {
        qDebug() << selectedFiles.at(i);
        }*/
/*QTime tim;
tim.start();*/

if (InputType==EXTERNAL_FILE)
{
qDebug() << "External File! Decompress Only!";
for (int i=0;i<selectedFiles.length();i++)
ExternalDeCompressFileZlib(selectedFiles.at(i),OutputType,1024);//warning: wrong size
}
else
{
    if (toCompress)
    {
        for (int i=0;i<selectedFiles.length();i++)
        CompressFile(selectedFiles.at(i),InputType,CompressionLevel,OutputType);
    }
    else//deCompress
    {
        for (int i=0;i<selectedFiles.length();i++)
        DeCompressFile(selectedFiles.at(i),InputType,OutputType,COMPRESSION_QTINTERNAL);
    }
}
end_of_start:
this->unsetCursor();
//qDebug() << "Time=" << tim.restart();
}

void MainWindow::doExit(void)
{
qApp->exit(0);
}

void MainWindow::doOpen(void)
{

}

void MainWindow::doSave(void)
{

}

void MainWindow::newFilesEntered(QStringList files)
{
//qDebug() << endl << files.length() << " Files Entered";
    lst->clear();
    for (int i=0;i<files.length();i++)
    {
    //qDebug() << files.at(i);
    new QListWidgetItem(files.at(i), lst);
    }
}
