#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windows.h"
#include <QLibrary>
#include <QString>
#include <QDateTime>
#include <QTimer>
#include "axis.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QLibrary lib("gts.lib");
    this->setWindowTitle("MotionStage");
    axis1=new axis(nullptr,"Axis1");
    axis2=new axis(nullptr,"Axis2");
    axis3=new axis(nullptr,"Axis3");
    ui->axis1Widget->addTab(axis1,"Axis1");
    ui->axis2Widget->addTab(axis2,"Axis2");
    ui->axis3Widget->addTab(axis3,"Axis3");

    QTimer *timer=new QTimer;
    connect(timer,&QTimer::timeout,this,&MainWindow::ShowTime);
    ui->timeNumber->setDigitCount(8);
    ui->timeNumber->display(QDateTime::currentDateTime().toString("hh:mm:ss"));
    ui->dataNumber->setDigitCount(10);
    ui->dataNumber->display(QDateTime::currentDateTime().toString("yyyy.MM.dd"));
    timer->start(1000);

    connect(axis1,&axis::commandHandle,this,&MainWindow::CommandHandler);
    connect(axis2,&axis::commandHandle,this,&MainWindow::CommandHandler);
    connect(axis3,&axis::commandHandle,this,&MainWindow::CommandHandler);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::ShowTime()
{
    ui->timeNumber->display(QDateTime::currentDateTime().toString("hh:mm:ss"));
    ui->dataNumber->display(QDateTime::currentDateTime().toString("yyyy.MM.dd"));
}

void MainWindow::CommandHandler(QString command, int value)
{
    QString str;
    if(value)
    {
        qDebug()<<command<<"failed!!"<<value<<endl;
        str.append(command);
        str.append("\tfailed!!\t");
        str.append(QString::number(value));
        ui->message->append(str);
    }
    else
    {
        qDebug()<<command<<"successful"<<value<<endl;
    }
}

void MainWindow::on_openBtn_clicked()
{
    retValue=GT_Open();
    CommandHandler("open",retValue);
    if(~retValue)
    {
        axis1->uiInit();
        axis2->uiInit();
        axis3->uiInit();
    }
}

void MainWindow::on_closeBtn_clicked()
{
    retValue=GT_Close();
    CommandHandler("close",retValue);
}

void MainWindow::on_cfgBtn_clicked()
{
    QString str= "E:\\Qt\\LX\\googolMotion\\GTS800.cfg";
    QByteArray temp=str.toLatin1();
    char* file=temp.data();
    qDebug()<<file<<endl;
    retValue=GT_LoadConfig(file);
    CommandHandler("load config",retValue);
    GT_ClrSts(1);
    GT_ClrSts(2);
}
