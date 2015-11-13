#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QtCore"
#include "QtGui"
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <vlc-qt/Common.h>
#include <vlc-qt/Instance.h>
#include <vlc-qt/Media.h>
#include <vlc-qt/MediaPlayer.h>
#include <windows.h>

/*
 data[0] = makshastighed
 data[1] = hastighed
 data[2] = afstand
 data[3] = acceleration
 data[4] = AKSstatus
 data[5] = styretoj
*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),media(0)
{
    isConnected=false;
    ui->setupUi(this);
    this->setWindowTitle("Au2");

    instance = new VlcInstance(VlcCommon::args(), this);
    player = new VlcMediaPlayer(instance);
    player->setVideoWidget(ui->video);

    readDataFromFile();

    data[1] = 0;//hastighed
    data[2] = 0;//afstand
    data[3] = 0;//acceleration
    data[4] = 1;//AKS = on

    ui->AKS->setText("AKS-On");

    updateData();

    connect(ui->OpretForbindelse, SIGNAL(clicked()), this, SLOT(Au2connect()));
    connect(ui->KonfigurerIP, SIGNAL(clicked()), this, SLOT(konfigurerIP()));
    connect(ui->AKS, SIGNAL(clicked()), this, SLOT(AKSstatus()));
    connect(ui->IndstilMaksHastighed, SIGNAL(clicked()), this, SLOT(maksHastighed()));
    connect(ui->KalibrerStyretoj, SIGNAL(clicked()), this, SLOT(kalibrerStyretoj()));
    connect(ui->LukNed, SIGNAL(clicked()), this, SLOT(shutDown()));
    connect(this, SIGNAL(sig_getData()), this, SLOT(readSocket()));
}
void MainWindow::readSocket()
{
    mutex.lock();
    socket->write(data,6);
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    socket->read(data,6);
    mutex.unlock();
}

void MainWindow::openPlayer()
{
    media = new VlcMedia(videoUrl, instance);
    player->open(media);
}

void MainWindow::Au2connect()
{
    if(isConnected)
    {
        QMessageBox messageBox;
        messageBox.information(0,"Status","Forbindelsen er allerede oprettet!");
        messageBox.setFixedSize(500,200);
    }
    else
    {
        socket = new QTcpSocket(this);

        connect(socket,SIGNAL(connected()),this,SLOT(connected()),Qt::DirectConnection);
        connect(socket,SIGNAL(disconnected()),this,SLOT(connectionLost()),Qt::DirectConnection);

        socket->connectToHost(IP,1234);

        if(!socket->waitForConnected(1000))
        {
            QMessageBox messageBox;
            messageBox.critical(0,"Fejl","Forbindelsen blev ikke oprettet!");
            messageBox.setFixedSize(500,200);
        }
        else
        {
            int error = pthread_create(&dataThread, NULL, this->getDataHelper ,this);
               if(error !=0)
              {
                qDebug()<<"Error on pthread_create"<<endl;
                return;
              }

        openPlayer();
         }
     }
}

void MainWindow::konfigurerIP()
{
    QString copy = IP;
    IP = QInputDialog::getText(this, tr("Konfigurer IP"), tr("Indtast IP adressen"), QLineEdit::Normal,IP);
    if (IP.isEmpty()){
    IP = copy;
    return;
    }
    videoUrl = "http://"+IP+":8081/stream.mjpeg";
}

void MainWindow::AKSstatus()
{
    data[4] =!data[4];
    if(data[4])
    ui->AKS->setText("AKS-On");
    else
    ui->AKS->setText("AKS-Off");
}

void MainWindow::maksHastighed()
{
    int copy = (int)data[0];
    bool ok;

    data[0] = (char)QInputDialog::getInt(this, tr("Makshastighed"),tr("Indtast makshastigheden"),
                                         (int)data[0], 0, 20, 1, &ok);
    if (!ok)
        data[0] = (char)copy;

    ui->lcdMakshastighed->display((int)data[0]);
}


void MainWindow::updateData()
{
    ui->lcdAcceleration->display((int)data[3]);
    ui->lcdHastighed->display((int)data[1]);
    ui->lcdMakshastighed->display((int)data[0]);
    ui->lcdAfstand->display((int)data[2]);
}

void MainWindow::writeDataToFile()
{
    QFile file("Au2Data.txt");
        if(!file.open(QIODevice::WriteOnly))
            return;

    QTextStream out(&file);
    out << IP << "\r\n";
    out << videoUrl << "\r\n";
    out << (int)data[5] << "\r\n";
    out << (int)data[0] << "\r\n";
}

void MainWindow::readDataFromFile()
{
    QFile file("Au2Data.txt");
        if(!file.open(QIODevice::ReadOnly))
            return;

    QTextStream in(&file);
    IP = in.readLine();
    videoUrl = in.readLine();
    data[5] = (char)in.readLine().toInt();
    data[0] = (char)in.readLine().toInt();
}

void MainWindow::connectionLost()
{
    isConnected=false;
    delete socket;
    QMessageBox messageBox;
    messageBox.critical(0,"Fejl","Forbindelsen er tabt!\n Prøv at forbind igen");
    messageBox.setFixedSize(500,200);
}

void MainWindow::connected()
{
    isConnected=true;
    QMessageBox messageBox;
    messageBox.information(0,"Ok","Forbindelsen er oprettet");
    messageBox.setFixedSize(500,200);
}

void MainWindow::kalibrerStyretoj()
{
    int copy = (int)data[5];
    bool ok;

    data[5] = (char)QInputDialog::getInt(this, tr("Styretoj"),tr("Indstil styretoj"),
                                         (int)data[5], -50, 50, 1, &ok);
    if (!ok)
        data[5] = (char)copy;
}

void* MainWindow::getData(void)
{
    while(isConnected)
    {
        sig_getData();
        updateData();
        QThread::msleep(100);
    }
    return NULL;
}

void* MainWindow::getDataHelper(void* context)
{
    return ((MainWindow *)context)->getData();
}

void MainWindow::shutDown()
{
    if(!isConnected)
        close();

    char sdata[6];
    mutex.lock();
    socket->write("dwnnow",6);
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    socket->read(sdata,6);
    if(sdata[0]=='d' && sdata[1]=='w' && sdata[2]=='n' && sdata[3]=='n' && sdata[4]=='o' && sdata[5]=='w')
    {
        socket->disconnect();
        mutex.unlock();
        close();
    }
    else
    {
        mutex.unlock();
        QMessageBox messageBox;
        messageBox.critical(0,"Fejl","Bilen kan ikke lukke ned!\n Prøv igen");
        messageBox.setFixedSize(500,200);
    }

}

MainWindow::~MainWindow()
{
    writeDataToFile();

    if(socket != NULL)
        delete socket;

    if(media != NULL)
        delete media;

    delete player;
    delete instance;
    delete ui;
}











