#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windows.h"
#include <QLibrary>
#include <QString>
#include <QDateTime>
#include <QTimer>
#include "axis.h"
#include <QThread>
#include <QImage>
#include <QDir>
#include "lightbutton.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    QLibrary lib("gts.lib");


    this->setWindowTitle("MotionStage");
    axis1=new axis(this,"Axis1");
    axis2=new axis(this,"Axis2");
    axis3=new axis(this,"Axis3");
    ui->axis1Widget->addTab(axis1,"Axis1");
    ui->axis2Widget->addTab(axis2,"Axis2");
    ui->axis3Widget->addTab(axis3,"Axis3");
    QImage image("../googolMotion/Resources/UPLogo.jpg");
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    ui->imageLabel->setGeometry(0,0,image.width(),image.height());


    //初始化部分参数
    connect(this,&MainWindow::axis1Init,axis1,&axis::uiInit);
    connect(this,&MainWindow::axis2Init,axis2,&axis::uiInit);
    connect(this,&MainWindow::axis3Init,axis3,&axis::uiInit);
    //顶部时钟
    QTimer *timer=new QTimer;
    connect(timer,&QTimer::timeout,this,&MainWindow::ShowTime);
    ui->timeNumber->setDigitCount(8);
    ui->timeNumber->display(QDateTime::currentDateTime().toString("hh:mm:ss"));
    ui->dataNumber->setDigitCount(10);
    ui->dataNumber->display(QDateTime::currentDateTime().toString("yyyy.MM.dd"));
    timer->start(1000);

    //axis中的返回值处理链接到mainwindow上
    connect(axis1,&axis::commandHandle,this,&MainWindow::CommandHandler,Qt::DirectConnection);
    connect(axis2,&axis::commandHandle,this,&MainWindow::CommandHandler,Qt::DirectConnection);
    connect(axis3,&axis::commandHandle,this,&MainWindow::CommandHandler,Qt::DirectConnection);

    connect(this,&MainWindow::updateStart,axis1,&axis::updateStart);
    connect(this,&MainWindow::updateStart,axis2,&axis::updateStart);
    connect(this,&MainWindow::updateStart,axis3,&axis::updateStart);

    connect(this,&MainWindow::updateTerm,axis1,&axis::updateTerm);
    connect(this,&MainWindow::updateTerm,axis2,&axis::updateTerm);
    connect(this,&MainWindow::updateTerm,axis3,&axis::updateTerm);

    axis1->show();
    axis2->show();
    axis3->show();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete axis1;
    delete axis2;
    delete axis3;
}


void MainWindow::ShowTime()
{
    ui->timeNumber->display(QDateTime::currentDateTime().toString("hh:mm:ss"));
    ui->dataNumber->display(QDateTime::currentDateTime().toString("yyyy.MM.dd"));

    GT_GetSts(1,&axisStatus1);
    GT_GetSts(2,&axisStatus2);
    GT_GetSts(3,&axisStatus3);
    if(axisStatus1&0x200 && axisStatus2&0x200 && axisStatus3&0x200)
    {
        ui->enableAllBtn->setText("Disable");
    }
    else
    {
        ui->enableAllBtn->setText("Enable");
    }
}

void MainWindow::CommandHandler(QString command, int value)
{
    QString str;
    if(value)
    {
        str=QString("%1  failed!!  %2").arg(command).arg(value);
        ui->message->append(str);
    }
    else
    {
        str=QString("%1  successful!!  %2").arg(command).arg(value);
        ui->message->append(str);
    }
}

void MainWindow::on_openBtn_clicked()
{
    retValue=GT_Open();
    CommandHandler("open",retValue);
    if(~retValue)
    {
        emit axis1Init();
        emit axis2Init();
        emit axis3Init();
        emit updateStart();
    }
    QString str= "..\\googolMotion\\GTS800.cfg";
    QByteArray temp=str.toLatin1();
    char* file=temp.data();
    retValue=GT_LoadConfig(file);
    CommandHandler("load config",retValue);
    GT_ClrSts(1);
    GT_ClrSts(2);
    GT_ClrSts(3);
}

void MainWindow::on_closeBtn_clicked()
{
    retValue=GT_Close();
    emit updateTerm();
    CommandHandler("close",retValue);
}

void MainWindow::on_cfgBtn_clicked()
{
    QString str= "E:\\Qt\\LX\\googolMotion\\GTS800.cfg";
    QByteArray temp=str.toLatin1();
    char* file=temp.data();
 //   qDebug()<<file<<endl;
    retValue=GT_LoadConfig(file);
    CommandHandler("load config",retValue);
    GT_ClrSts(1);
    GT_ClrSts(2);
    GT_ClrSts(3);
}

void MainWindow::on_killStopBtn_clicked()
{
    emit axis1->moveStop();
    emit axis2->moveStop();
    emit axis3->moveStop();
    GT_Stop(0Xf,0XF);
}

void MainWindow::on_enableAllBtn_clicked()
{
    GT_GetSts(1,&axisStatus1);
    GT_GetSts(2,&axisStatus2);
    GT_GetSts(3,&axisStatus3);
    if(axisStatus1&0x200 && axisStatus2&0x200 && axisStatus3&0x200)
    {
        retValue=GT_AxisOff(1);
        CommandHandler("Axis1 Enable off",retValue);
        retValue=GT_AxisOff(2);
        CommandHandler("Axis2 Enable off",retValue);
        retValue=GT_AxisOff(3);
        if(!retValue)
        {
            ui->enableAllBtn ->setText("Enable");
        }
        CommandHandler("Axis3 Enable off",retValue);
    }
    else
    {
        retValue=GT_AxisOn(1);
        CommandHandler("Axis1 Enable on",retValue);
        retValue=GT_AxisOn(2);
        CommandHandler("Axis2 Enable on",retValue);
        retValue=GT_AxisOn(3);
        CommandHandler("Axis3 Enable on",retValue);
        if(!retValue)
        {
            ui->enableAllBtn->setText("Disable");
        }
    }
}

void MainWindow::on_homeAll_clicked()
{
    homeThread1=new QThread;
    home1=new Home;
    home1->moveToThread(homeThread1);

    connect(axis1,&axis::sHome,home1,&Home::doWorks);

    connect(homeThread1,&QThread::finished,home1,&Home::deleteLater);
    connect(home1,&Home::destroyed,homeThread1,&QThread::deleteLater);
    connect(home1,&Home::workFinished,[this](){
        homeThread1->quit();
    });

    homeThread1->start();

    homeThread2=new QThread;
    home2=new Home;
    home2->moveToThread(homeThread2);

    connect(axis2,&axis::sHome,home2,&Home::doWorks);

    connect(homeThread2,&QThread::finished,home2,&Home::deleteLater);
    connect(home2,&Home::destroyed,homeThread2,&QThread::deleteLater);
    connect(home2,&Home::workFinished,[this](){
        homeThread2->quit();
    });

    homeThread2->start();

    homeThread3=new QThread;
    home3=new Home;
    home3->moveToThread(homeThread3);

    connect(axis3,&axis::sHome,home3,&Home::doWorks);

    connect(homeThread3,&QThread::finished,home3,&Home::deleteLater);
    connect(home3,&Home::destroyed,homeThread3,&QThread::deleteLater);
    connect(home3,&Home::workFinished,[this](){
        homeThread3->quit();
    });

    homeThread3->start();

    emit axis1->sHome(1);
    emit axis2->sHome(2);
    emit axis3->sHome(3);
}
