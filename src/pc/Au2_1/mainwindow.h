#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <pthread.h>
#include <QMutex>
#include "xboxcontroller.h"

namespace Ui {
class MainWindow;
}

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    sig_getData();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void Au2connect();
    void konfigurerIP();
    void AKSstatus();
    void maksHastighed();
    void readSocket();
    void connectionLost();
    void connected();
    void kalibrerStyretoj();
    void shutDown();
    void controllerIsConnected();
    void controllerLostConnection();

private: 
    void controller();
    void *controllerStream();
    void *getData(void);
    static void *getDataHelper(void *context);
    static void *controllerStreamHelper(void *context);
    void openPlayer();
    void writeDataToFile();
    void readDataFromFile();
    void updateData();
    bool isConnected;
    bool controllerConnected;
    int socketDescriptor;
    char data[6];
    QMutex mutex;
    QTcpSocket *socket;
    QTcpSocket *controllerSocket;
    QString IP;
    QString videoUrl;
    Ui::MainWindow *ui;
    VlcInstance *instance;
    VlcMedia *media;
    VlcMediaPlayer *player;
    pthread_t dataThread;
    pthread_t controllerThread;
    XboxController *XboxController_;
};

#endif
