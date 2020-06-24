#include "axis.h"
#include "ui_axis.h"
#include "mainwindow.h"
axis::axis(QWidget *parent,QString name) :
    QWidget(parent),
    ui(new Ui::axis)
{
    ui->setupUi(this);
    QTimer* timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,&axis::timerOut);
//    connect(MainWindow,&MainWindow::init)
    this->setObjectName(name);



    timer->start(200);
}

axis::~axis()
{
    delete ui;
}

void axis::on_enableBtn_clicked()
{
    if(objectName()=="Axis1")
    {
        emit commandHandle("aixs1 Enable",1);
    }
    else if(objectName()=="Axis2")
    {
        emit commandHandle("aixs2 Enable",2);
    }
    else if(objectName()=="Axis3")
    {
        emit commandHandle("aixs3 Enable",3);
    }
    else if(objectName()==nullptr)
    {
        return;
    }
}


void axis::timerOut()
{
    if(objectName()=="Axis1")
    {
        emit commandHandle("aixs1 Enable",1);
    }
    else if(objectName()=="Axis2")
    {
        emit commandHandle("aixs2 Enable",2);
    }
    else if(objectName()=="Axis3")
    {
        emit commandHandle("aixs3 Enable",3);
    }
    else if(objectName()==nullptr)
    {
        return;
    }
}

void axis::uiInit()
{
    if(objectName()=="Axis1")
    {
        retValue=GT_GetPid(1,1,pid);
        ui->kpText->setText(QString::number(pid->kp));
        ui->kiText->setText(QString::number(pid->ki));
        ui->kdText->setText(QString::number(pid->kd));
        ui->forwardText->setText(QString::number(pid->kaff));
        retValue=GT_GetAxisEncPos(1,&pos,1);
        retValue=GT_GetAxisError(1,&error,1);
        retValue=GT_GetAxisEncVel(1,&vel,1);

        qDebug()<<1<<endl;
    }
    else if(objectName()=="Axis2")
    {
        retValue=GT_GetPid(2,2,pid);
        ui->kpText->setText(QString::number(pid->kp));
        ui->kpText->setText(QString::number(pid->kp));
        ui->kiText->setText(QString::number(pid->ki));
        ui->kdText->setText(QString::number(pid->kd));
        ui->forwardText->setText(QString::number(pid->kaff));
    }
    else if(objectName()=="Axis3")
    {
        retValue=GT_GetPid(3,3,pid);
        ui->kpText->setText(QString::number(pid->kp));
        ui->kpText->setText(QString::number(pid->kp));
        ui->kiText->setText(QString::number(pid->ki));
        ui->kdText->setText(QString::number(pid->kd));
        ui->forwardText->setText(QString::number(pid->kaff));
    }
}
