#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <pthread.h>
#include <QMutex>

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

private: 
    void* getData(void);
    static void* getDataHelper(void *context);
    void openPlayer();
    void writeDataToFile();
    void readDataFromFile();
    void updateData();
    bool isConnected;
    int socketDescriptor;
    char data[6];
    QMutex mutex;
    QTcpSocket *socket;
    QString IP;
    QString videoUrl;
    Ui::MainWindow *ui;
    VlcInstance *instance;
    VlcMedia *media;
    VlcMediaPlayer *player;
    pthread_t dataThread;
};

#endif
