#include "axis.h"
#include "ui_axis.h"
#include "mainwindow.h"
#include <QMutex>
axis::axis(QWidget *parent,QString name) :
    QWidget(parent),
    ui(new Ui::axis),absoluteThread(nullptr),absoluteMove(nullptr)
{
    ui->setupUi(this);
    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,&axis::timerOut);
    this->setObjectName(name);

    pid=new TPid;

    if(objectName()=="Axis1")
    {
 //       qDebug()<<QThread::currentThread();
    }
    else if(objectName()=="Axis2")
    {
   //     qDebug()<<QThread::currentThreadId();
    }
    else if(objectName()=="Axis3")
    {
  //      qDebug()<<QThread::currentThreadId();
    }

    alarmBtn=new LightButton;
    positiveLimitBtn=new LightButton;
    negativeLimitBtn=new LightButton;
    enableBtn2=new LightButton;
    moveErrorBtn=new LightButton;
    moveStatusBtn=new LightButton;

    alarmLabel=new QLabel;
    positiveLimitLabel=new QLabel;
    negativeLimitLabel=new QLabel;
    enableLabel=new QLabel;
    moveErrorLabel=new QLabel;
    moveStatusLabel=new QLabel;

    alarmLabel->setText("驱动器报警");
    positiveLimitLabel->setText("正限位");
    negativeLimitLabel->setText("负限位");
    enableLabel->setText("驱动器使能");
    moveErrorLabel->setText("运动错误");
    moveStatusLabel->setText("运动状态");

    ui->gridLayout_3->addWidget(alarmBtn,0,0);
    ui->gridLayout_3->addWidget(alarmLabel,0,1);
    ui->gridLayout_3->addWidget(positiveLimitBtn,0,2);
    ui->gridLayout_3->addWidget(positiveLimitLabel,0,3);
    ui->gridLayout_3->addWidget(negativeLimitBtn,1,2);
    ui->gridLayout_3->addWidget(negativeLimitLabel,1,3);
    ui->gridLayout_3->addWidget(enableBtn2,1,0);
    ui->gridLayout_3->addWidget(enableLabel,1,1);
    ui->gridLayout_3->addWidget(moveErrorBtn,2,0);
    ui->gridLayout_3->addWidget(moveErrorLabel,2,1);
    ui->gridLayout_3->addWidget(moveStatusBtn,2,2);
    ui->gridLayout_3->addWidget(moveStatusLabel,2,3);
}

axis::~axis()
{
    delete ui;
    if(absoluteMove!=nullptr)
    //    delete absoluteMove;
    if(absoluteThread!=nullptr)
    {
        absoluteThread->quit();
        absoluteThread->wait();
        qDebug()<<"delete thread"<<endl;
     //   delete absoluteThread;
    }
    delete alarmBtn;
    delete positiveLimitBtn;
    delete negativeLimitBtn;
    delete enableBtn2;
    delete moveErrorBtn;
    delete moveStatusBtn;
    delete alarmLabel;
    delete positiveLimitLabel;
    delete negativeLimitLabel;
    delete enableLabel;
    delete moveErrorLabel;
    delete moveStatusLabel;
 //   qDebug()<<"delete axis!!"<<endl;
}

void axis::on_enableBtn_clicked()
{
    if(objectName()=="Axis1")
    {
        GT_GetSts(1,&axisStatus);
        if(axisStatus&0x200)
        {
            retValue=GT_AxisOff(1);
            if(!retValue)
            {
                ui->enableBtn->setText("Enable");
            }
            commandHandle("Axis1 Enable off",retValue);
        }
        else
        {
            retValue=GT_AxisOn(1);
            commandHandle("Axis1 Enable on",retValue);
            if(!retValue)
            {
                ui->enableBtn->setText("Disable");
            }
        }

    }
    else if(objectName()=="Axis2")
    {
        GT_GetSts(2,&axisStatus);
        if(axisStatus&0x200)
        {
            retValue=GT_AxisOff(2);
            if(!retValue)
            {
                ui->enableBtn->setText("Enable");
            }
            commandHandle("Axis2 Enable off",retValue);
        }
        else
        {
            retValue=GT_AxisOn(2);
            commandHandle("Axis2 Enable on",retValue);
            if(!retValue)
            {
                ui->enableBtn->setText("Disable");
            }
        }
    }
    else if(objectName()=="Axis3")
    {
        GT_GetSts(3,&axisStatus);
        if(axisStatus&0x200)
        {
            retValue=GT_AxisOff(3);
            if(!retValue)
            {
                ui->enableBtn->setText("Enable");
            }
            commandHandle("Axis3 Enable off",retValue);
        }
        else
        {
            retValue=GT_AxisOn(3);
            commandHandle("Axis3 Enable on",retValue);
            if(!retValue)
            {
                ui->enableBtn->setText("Disable");
            }
        }
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
        GT_GetAxisEncPos(1,&pos,1);
        GT_GetAxisEncVel(1,&vel,1);
        GT_GetAxisEncAcc(1,&acc,1);
        GT_GetAxisError(1,&error);
        GT_GetAxisPrfAcc(1,&prfAcc);
        GT_GetAxisPrfPos(1,&prfPos);
        GT_GetAxisPrfVel(1,&prfVel);
        GT_GetSts(1,&axisStatus);
    }
    else if(objectName()=="Axis2")
    {
        GT_GetAxisEncPos(2,&pos,1);
        GT_GetAxisEncVel(2,&vel,1);
        GT_GetAxisError(2,&error);
        GT_GetAxisEncAcc(2,&acc,1);
        GT_GetAxisPrfAcc(2,&prfAcc);
        GT_GetAxisPrfPos(2,&prfPos);
        GT_GetAxisPrfVel(2,&prfVel);
        GT_GetSts(2,&axisStatus);
    }
    else if(objectName()=="Axis3")
    {
        GT_GetAxisEncPos(3,&pos,1);
        GT_GetAxisEncVel(3,&vel,1);
        GT_GetAxisError(3,&error);
        GT_GetAxisEncAcc(3,&acc,1);
        GT_GetAxisPrfAcc(3,&prfAcc);
        GT_GetAxisPrfPos(3,&prfPos);
        GT_GetAxisPrfVel(3,&prfVel);
        GT_GetSts(3,&axisStatus);
    }
    else if(objectName()==nullptr)
    {
        return;
    }
    ui->fposText->setText(QString("%1").arg(pos,3,'f',1));
    ui->fvelText->setText(QString("%1").arg(vel,3,'f',1));
    ui->errorText->setText(QString("%1").arg(error,3,'f',1));
    ui->faccText->setText(QString("%1").arg(acc,3,'f',1));
    ui->prfPosText->setText(QString("%1").arg(prfPos,3,'f',1));
    ui->prfVelText->setText(QString("%1").arg(prfVel,3,'f',1));
    ui->prfAccText->setText(QString("%1").arg(prfAcc,3,'f',1));
    if(axisStatus&0x02)
    {
        this->alarmBtn->setRed();
    }
    else
    {
        this->alarmBtn->setBlue();
    }
    if(axisStatus&0x20)
    {
        this->positiveLimitBtn->setRed();
    }
    else
    {
        this->positiveLimitBtn->setBlue();
    }
    if(axisStatus&0x10)
    {
        this->moveErrorBtn->setRed();
    }
    else
    {
        this->moveErrorBtn->setBlue();
    }
    if(axisStatus&0x40)
    {
        this->negativeLimitBtn->setRed();
    }
    else
    {
        this->negativeLimitBtn->setBlue();
    }
    if(axisStatus&0x200)
    {
        this->enableBtn2->setRed();
        ui->enableBtn->setText("Disable");
    }
    else
    {
        this->enableBtn2->setBlue();
        ui->enableBtn->setText("Enable");
    }
    if(axisStatus&0x400)
    {
        this->moveStatusBtn->setRed();
    }
    else
    {
        this->moveStatusBtn->setBlue();
    }
}

void axis::uiInit()
{
    if(objectName()=="Axis1")
    {
        retValue=GT_GetPid(1,1,pid);
//        ui->kpText->setText(QString::number(pid->kp));
//        ui->kiText->setText(QString::number(pid->ki));
//        ui->kdText->setText(QString::number(pid->kd));
//        ui->forwardText->setText(QString::number(pid->kaff));
        retValue=GT_GetAxisEncPos(1,&pos,1);
        retValue=GT_GetAxisError(1,&error,1);
        retValue=GT_GetAxisEncVel(1,&vel,1);
        ui->fposText->setText(QString::number(pos));
        ui->errorText->setText(QString::number(error));
        ui->fvelText->setText(QString::number(vel));
    }
    else if(objectName()=="Axis2")
    {
        retValue=GT_GetPid(2,2,pid);
//        ui->kpText->setText(QString::number(pid->kp));
//        ui->kpText->setText(QString::number(pid->kp));
//        ui->kiText->setText(QString::number(pid->ki));
//        ui->kdText->setText(QString::number(pid->kd));
//        ui->forwardText->setText(QString::number(pid->kaff));
        retValue=GT_GetAxisEncPos(2,&pos,1);
        retValue=GT_GetAxisError(2,&error,1);
        retValue=GT_GetAxisEncVel(2,&vel,1);
        ui->fposText->setText(QString::number(pos));
        ui->errorText->setText(QString::number(error));
        ui->fvelText->setText(QString::number(vel));
    }
    else if(objectName()=="Axis3")
    {
        retValue=GT_GetPid(3,3,pid);
//        ui->kpText->setText(QString::number(pid->kp));
//        ui->kpText->setText(QString::number(pid->kp));
//        ui->kiText->setText(QString::number(pid->ki));
//        ui->kdText->setText(QString::number(pid->kd));
//        ui->forwardText->setText(QString::number(pid->kaff));
        retValue=GT_GetAxisEncPos(3,&pos,1);
        retValue=GT_GetAxisError(3,&error,1);
        retValue=GT_GetAxisEncVel(3,&vel,1);
        ui->fposText->setText(QString::number(pos));
        ui->errorText->setText(QString::number(error));
        ui->fvelText->setText(QString::number(vel));
    }
}

void axis::on_zeroBtn_clicked()
{
    if(objectName()=="Axis1")
    {
        retValue=GT_ZeroPos(1,1);
        commandHandle("Axis1 zero",retValue);
    }
    else if(objectName()=="Axis2")
    {
        retValue=GT_ZeroPos(2,1);
        commandHandle("Axis2 zero",retValue);
    }
    else if(objectName()=="Axis3")
    {
        retValue=GT_ZeroPos(3,1);
        commandHandle("Axis3 zero",retValue);
    }
    else if(objectName()==nullptr)
    {
        return;
    }
}

void axis::on_stopBtn_clicked()
{
    if(objectName()=="Axis1")
    {
        retValue=GT_Stop(1,1);
        commandHandle("Axis1 stop",retValue);
        emit moveStop();
    }
    else if(objectName()=="Axis2")
    {
        retValue=GT_Stop(1<<1,1<<1);
        commandHandle("Axis2 stop",retValue);
        qDebug()<<absoluteMove<<absoluteThread<<endl;
        emit moveStop();
    }
    else if(objectName()=="Axis3")
    {
        retValue=GT_Stop(1<<2,1<<2);
        commandHandle("Axis3 stop",retValue);
        qDebug()<<absoluteMove<<absoluteThread<<endl;
        emit moveStop();
    }
    else if(objectName()==nullptr)
    {
        return;
    }
}

void axis::on_clearBtn_clicked()
{
    if(objectName()=="Axis1")
    {
        retValue=GT_ClrSts(1,1);
    //    GT_SetDoBitReverse(MC_CLEAR,1,0,2);
        commandHandle("Axis1 Clear warning",retValue);
    }
    else if(objectName()=="Axis2")
    {
        retValue=GT_ClrSts(2,1);
   //     GT_SetDoBitReverse(MC_CLEAR,2,1,1);
        commandHandle("Axis2 Clear warning",retValue);
    }
    else if(objectName()=="Axis3")
    {
        retValue=GT_ClrSts(3,1);
   //     GT_SetDoBitReverse(MC_CLEAR,3,1,1);
        commandHandle("Axis3 Clear warning",retValue);
    }
    else if(objectName()==nullptr)
    {
        return;
    }
}

void axis::updateStart()
{
    this->timer->start(200);
}
void axis::updateTerm()
{
    timer->stop();
}

void axis::on_jogForward_pressed()
{

    if(objectName()=="Axis1")
    {
        GT_PrfJog(1);
        GT_GetJogPrm(1,&jogPrm);
        jogPrm.acc=ui->accText->text().toDouble();
        jogPrm.dec=ui->accText->text().toDouble();
        jogPrm.smooth=ui->smoothTimeText->text().toDouble();
        if(jogPrm.smooth>=1)
        {
            jogPrm.smooth=0.9;
        }
        retValue=GT_SetJogPrm(1,&jogPrm);
        commandHandle("Axis1 set prm",retValue);
        GT_SetVel(1,ui->velText->text().toDouble());
        GT_SetStopDec(1,jogPrm.dec,ui->killDecText->text().toDouble());
        retValue=GT_Update(1);
        commandHandle("Axis1 jog forward!!",retValue);
    }
    else if(objectName()=="Axis2")
    {
        GT_PrfJog(2);
        GT_GetJogPrm(2,&jogPrm);
        jogPrm.acc=ui->accText->text().toDouble();
        jogPrm.dec=ui->accText->text().toDouble();
        jogPrm.smooth=ui->smoothTimeText->text().toDouble();
        if(jogPrm.smooth>=1)
        {
            jogPrm.smooth=0.9;
        }
        retValue=GT_SetJogPrm(2,&jogPrm);
        GT_SetVel(2,ui->velText->text().toDouble());
        GT_SetStopDec(2,jogPrm.dec,ui->killDecText->text().toDouble());
        retValue=GT_Update(1<<1);
        commandHandle("Axis2 jog forward!!",retValue);
    }
    else if(objectName()=="Axis3")
    {
        retValue=GT_PrfJog(3);
        GT_GetJogPrm(3,&jogPrm);
        jogPrm.acc = ui->accText->text().toDouble();
        jogPrm.dec = ui->accText->text().toDouble();
        jogPrm.smooth = ui->smoothTimeText->text().toDouble();
        if(jogPrm.smooth>=1)
        {
            jogPrm.smooth=0.9;
        }
        GT_SetJogPrm(3,&jogPrm);
        retValue = GT_SetVel(3,ui->velText->text().toDouble());
        GT_SetStopDec(3,jogPrm.dec,ui->killDecText->text().toDouble());
        retValue = GT_Update(1<<2);
        commandHandle("Axis3 jog forward!!",retValue);
    }
    else if(objectName()==nullptr)
    {
        return;
    }
}

void axis::on_jogBack_pressed()
{
    if(objectName()=="Axis1")
    {
        GT_PrfJog(1);
        GT_GetJogPrm(1,&jogPrm);
        jogPrm.acc=ui->accText->text().toDouble();
        jogPrm.dec=ui->accText->text().toDouble();
        jogPrm.smooth=ui->smoothTimeText->text().toDouble();
        if(jogPrm.smooth>=1)
        {
            jogPrm.smooth=0.9;
        }
        GT_SetJogPrm(1,&jogPrm);
        GT_SetVel(1,-1*ui->velText->text().toDouble());
        GT_SetStopDec(1,jogPrm.dec,ui->killDecText->text().toDouble());
        retValue=GT_Update(1);
        commandHandle("Axis1 jog back!!",retValue);
    }
    else if(objectName()=="Axis2")
    {
        GT_PrfJog(2);
        GT_GetJogPrm(2,&jogPrm);
        jogPrm.acc=ui->accText->text().toDouble();
        jogPrm.dec=-1*ui->accText->text().toDouble();
        jogPrm.smooth=ui->smoothTimeText->text().toDouble();
        if(jogPrm.smooth>=1)
        {
            jogPrm.smooth=0.9;
        }
        GT_SetJogPrm(2,&jogPrm);
        GT_SetStopDec(2,jogPrm.dec,ui->killDecText->text().toDouble());
        GT_SetVel(2,-1*ui->velText->text().toDouble());
        retValue=GT_Update(1<<1);
        commandHandle("Axis2 jog back!!",retValue);
    }
    else if(objectName()=="Axis3")
    {
        retValue=GT_PrfJog(3);
        GT_GetJogPrm(3,&jogPrm);
        jogPrm.acc=ui->accText->text().toDouble();
        jogPrm.dec=ui->accText->text().toDouble();
        jogPrm.smooth=ui->smoothTimeText->text().toDouble();
        if(jogPrm.smooth>=1)
        {
            jogPrm.smooth=0.9;
        }
        GT_SetJogPrm(3,&jogPrm);
        retValue=GT_SetVel(3,-1*ui->velText->text().toDouble());
        GT_SetStopDec(3,jogPrm.dec,ui->killDecText->text().toDouble());
        retValue=GT_Update(1<<2);
        commandHandle("Axis3 jog back!!",retValue);
    }
    else if(objectName()==nullptr)
    {
        return;
    }
}

void axis::on_jogForward_released()
{

    if(objectName()=="Axis1")
    {
        GT_Stop(1,0);
    }
    else if(objectName()=="Axis2")
    {
        GT_Stop(2,0);
    }
    else if(objectName()=="Axis3")
    {
        GT_Stop(1<<2,0);
    }
    else if(objectName()==nullptr)
    {
        return;
    }
}

void axis::on_jogBack_released()
{
    if(objectName()=="Axis1")
    {
        GT_Stop(1,0);
    }
    else if(objectName()=="Axis2")
    {
        GT_Stop(2,0);
    }
    else if(objectName()=="Axis3")
    {
        GT_Stop(1<<2,0);
    }
    else if(objectName()==nullptr)
    {
        return;
    }
}

void axis::absoluteThreadFinished()
{
    absoluteMove=nullptr;
    absoluteThread=nullptr;
//    qDebug()<<absoluteMove<<absoluteThread<<endl;
}

void axis::on_absoluteStart_clicked()
{
    int rep;
    absoluteThread=new QThread(nullptr);
    absoluteMove=new AbsoluteMove;
    absoluteMove->moveToThread(absoluteThread);

    connect(this,&axis::absolute,absoluteMove,&AbsoluteMove::doWorks);

    connect(absoluteThread,&QThread::finished,absoluteMove,&QObject::deleteLater);
    connect(absoluteMove,&AbsoluteMove::destroyed,absoluteThread,&QThread::deleteLater);

    connect(this,&axis::moveStop,absoluteMove,&AbsoluteMove::moveStop,Qt::DirectConnection);
    connect(absoluteThread,&QThread::destroyed,this,&axis::absoluteThreadFinished);

    connect(absoluteMove,&AbsoluteMove::workFinshed,[this](){
        absoluteThread->quit();
   //     absoluteThread->wait();
    });
    absoluteThread->start();

    if(objectName()=="Axis1")
    {
        GT_GetTrapPrm(1,&trapPrm);
        trapPrm.acc=ui->accText->text().toDouble();
        trapPrm.dec=ui->accText->text().toDouble();
        trapPrm.smoothTime=ui->smoothTimeText->text().toShort();
        GT_PrfTrap(1);
        GT_SetTrapPrm(1,&trapPrm);
        retValue=GT_SetVel(1,ui->velText->text().toDouble());
        commandHandle("axis1 setVel",retValue);
        rep=ui->absoluteRep->checkState();
        emit absolute(1,ui->absolutePosText2->text().toInt(),ui->absolutePosText->text().toInt(),rep);
    }
    else if(objectName()=="Axis2")
    {
        GT_GetTrapPrm(2,&trapPrm);
        trapPrm.acc=ui->accText->text().toDouble();
        trapPrm.dec=ui->accText->text().toDouble();
        trapPrm.smoothTime=ui->smoothTimeText->text().toShort();
        GT_PrfTrap(2);
        GT_SetTrapPrm(2,&trapPrm);
        retValue=GT_SetVel(2,ui->velText->text().toDouble());
        commandHandle("axis1 setVel",retValue);
        rep=ui->absoluteRep->checkState();
        emit absolute(2,ui->absolutePosText2->text().toInt(),ui->absolutePosText->text().toInt(),rep);
    }
    else if(objectName()=="Axis3")
    {
        GT_GetTrapPrm(3,&trapPrm);
        trapPrm.acc=ui->accText->text().toDouble();
        trapPrm.dec=ui->accText->text().toDouble();
        trapPrm.smoothTime=ui->smoothTimeText->text().toShort();
        GT_PrfTrap(3);
        GT_SetTrapPrm(3,&trapPrm);
        retValue=GT_SetVel(3,ui->velText->text().toDouble());
        commandHandle("axis1 setVel",retValue);
        rep=ui->absoluteRep->checkState();
        emit absolute(3,ui->absolutePosText2->text().toInt(),ui->absolutePosText->text().toInt(),rep);
    }
    else if(objectName()==nullptr)
    {
        return;
    }

}



AbsoluteMove::AbsoluteMove(QObject* parent):QObject(parent),m_stop(0)
{

}

void AbsoluteMove::doWorks(short profile,int start,int end,int rep)
{
    long mask=long(1<<(profile-1));
    do
    {
        GT_SetPos(profile,end);
        GT_Update(mask);
        do
        {
            QThread::msleep(1);
            GT_GetSts(profile,&axisState);
        }while(axisState&0x400);
        if(!rep)
        {
            break;
        }

        if(m_stop)
        {
            m_stop=0;
            break;
        }
        QThread::msleep(600);

        GT_SetPos(profile,start);
        GT_Update(mask);
        do
        {
            QThread::msleep(1);
            GT_GetSts(profile,&axisState);
        }while(axisState&0x400);
        if(m_stop)
        {
            m_stop=0;
            break;
        }
        QThread::msleep(500);
    }while(rep);
    emit workFinshed();
}


void AbsoluteMove::moveStop()
{
    m_stop=1;
}

Home::Home(QObject* parent):QObject(parent)
{

}
void Home::doWorks(short axis)
{
    int retValue;
    static QMutex mutex;
    retValue=1;
//    QMutexLocker locker(&mutex);

    retValue = GT_GetHomePrm(axis,&tHomePrm);
    tHomePrm.mode=11;
    tHomePrm.moveDir=-1;
    tHomePrm.indexDir=1;
    tHomePrm.edge=0;
    tHomePrm.triggerIndex=axis;
    tHomePrm.velHigh=5;
    tHomePrm.velLow=4;
    tHomePrm.acc=0.1;
    tHomePrm.dec=0.1;
    tHomePrm.searchHomeDistance=200000;
    tHomePrm.searchIndexDistance=30000;
    tHomePrm.escapeStep=1000;
    tHomePrm.pad2[1]=1;
    if(axis==1)
    {
        tHomePrm.velHigh=10;
        tHomePrm.velLow=8;
        tHomePrm.homeOffset=-124000;
    }
    else if(axis==2)
    {
        tHomePrm.homeOffset=-30000;
    }
    else if(axis==3)
    {
        tHomePrm.homeOffset=-31000;
    }

    retValue = GT_GoHome(axis,&tHomePrm);
    do
    {
        GT_GetHomeStatus(axis,&tHomeSta);
    }while(tHomeSta.run);
    GT_ClrSts(axis);
    GT_ZeroPos(axis);

    emit workFinished();
}

void Home::moveStop()
{

}
void Home::start()
{
    qDebug()<<"start!!"<<endl;
}
void axis::on_homeBtn_clicked()
{
    homeThread=new QThread;
    home=new Home;
    home->moveToThread(homeThread);

    connect(this,&axis::sHome,home,&Home::doWorks);

    connect(homeThread,&QThread::finished,home,&Home::deleteLater);
    connect(home,&Home::destroyed,homeThread,&QThread::deleteLater);
    connect(home,&Home::workFinished,[this](){
        homeThread->quit();
   //     homeThread->wait();
    });
    homeThread->start();
    if(objectName()=="Axis1")
    {
        emit sHome(1);
    }
    else if(objectName()=="Axis2")
    {
        emit sHome(2);
    }
    else if(objectName()=="Axis3")
    {
        emit sHome(3);
    }
    else if(objectName()==nullptr)
    {
        return;
    }
}

void axis::on_relativeStart_clicked()
{
    int rep;
    realtiveThread=new QThread;
    realtiveMove=new RealtiveMove;
    realtiveMove->moveToThread(realtiveThread);

    connect(this,&axis::realtive,realtiveMove,&RealtiveMove::doWorks);

    connect(realtiveThread,&QThread::finished,realtiveMove,&QObject::deleteLater);
    connect(realtiveMove,&AbsoluteMove::destroyed,realtiveThread,&QThread::deleteLater);

    connect(this,&axis::moveStop,realtiveMove,&RealtiveMove::moveStop,Qt::DirectConnection);
//    connect(realtiveThread,&QThread::destroyed,this,&axis::absoluteThreadFinished);

    connect(realtiveMove,&RealtiveMove::workFinshed,[this](){
        realtiveThread->quit();
   //     absoluteThread->wait();
    });
    realtiveThread->start();

    if(objectName()=="Axis1")
    {
        GT_GetTrapPrm(1,&trapPrm);
        trapPrm.acc=ui->accText->text().toDouble();
        trapPrm.dec=ui->accText->text().toDouble();
        trapPrm.smoothTime=ui->smoothTimeText->text().toShort();
        GT_PrfTrap(1);
        GT_SetTrapPrm(1,&trapPrm);
        retValue=GT_SetVel(1,ui->velText->text().toDouble());
  //      commandHandle("axis1 setVel",retValue);
        GT_GetPos(1,&pPos);
        rep=ui->relativeRep->checkState();
     //   qDebug()<<pPos<<endl;
        emit realtive(1,pPos,ui->relativeEdit->text().toInt(),rep);
    }
    else if(objectName()=="Axis2")
    {
        GT_GetTrapPrm(2,&trapPrm);
        trapPrm.acc=ui->accText->text().toDouble();
        trapPrm.dec=ui->accText->text().toDouble();
        trapPrm.smoothTime=ui->smoothTimeText->text().toShort();
        GT_PrfTrap(2);
        GT_SetTrapPrm(2,&trapPrm);
        retValue=GT_SetVel(2,ui->velText->text().toDouble());
        commandHandle("axis2 setVel",retValue);
        GT_GetPos(2,&pPos);


    }
    else if(objectName()=="Axis3")
    {
        GT_GetTrapPrm(3,&trapPrm);
        trapPrm.acc=ui->accText->text().toDouble();
        trapPrm.dec=ui->accText->text().toDouble();
        trapPrm.smoothTime=ui->smoothTimeText->text().toShort();
        GT_PrfTrap(3);
        GT_SetTrapPrm(3,&trapPrm);
        retValue=GT_SetVel(3,ui->velText->text().toDouble());
        commandHandle("axis3 setVel",retValue);
        GT_GetPos(3,&pPos);
        rep=ui->relativeRep->checkState();
    //    qDebug()<<pPos<<endl;
        emit realtive(3,pPos,ui->relativeEdit->text().toInt(),rep);
    }
    else if(objectName()==nullptr)
    {
        return;
    }
}

void RealtiveMove::doWorks(short profile,int start,int end,int rep)
{
    long mask=long(1<<(profile-1));
    end+=start;
//    qDebug()<<rep<<endl;
    do
    {
        GT_SetPos(profile,end);
        GT_Update(mask);
        do
        {
            QThread::msleep(1);
            GT_GetSts(profile,&axisState);
        }while(axisState&0x400);
        if(!rep)
        {
            break;
        }

        if(m_stop)
        {
            m_stop=0;
            break;
        }
        QThread::msleep(600);

        GT_SetPos(profile,start);
        GT_Update(mask);
        do
        {
            QThread::msleep(1);
            GT_GetSts(profile,&axisState);
        }while(axisState&0x400);
        if(m_stop)
        {
            m_stop=0;
            break;
        }
        QThread::msleep(500);
    }while(rep);
    emit workFinshed();
}

void RealtiveMove::moveStop()
{
    this->m_stop=1;
}

RealtiveMove::RealtiveMove(QObject *parent):QObject(parent)
{

}
