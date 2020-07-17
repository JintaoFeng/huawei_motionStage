#ifndef AXIS_H
#define AXIS_H

#include <QWidget>
#include <QString>
#include <QTimer>
#include "gts.h"
#include <QThread>
#include"lightbutton.h"
#include <QLabel>
namespace Ui {
class axis;
}
class AbsoluteMove;
class Home;
class RealtiveMove;
class axis : public QWidget
{
    Q_OBJECT

public:
    explicit axis(QWidget *parent = nullptr,QString name="Axis");
    ~axis();

public slots:
    void uiInit();
    void updateStart();
    void updateTerm();
private slots:
    void on_enableBtn_clicked();
    void timerOut();
    void absoluteThreadFinished();

    void on_zeroBtn_clicked();

    void on_stopBtn_clicked();

    void on_clearBtn_clicked();

    void on_jogForward_pressed();

    void on_jogBack_pressed();

    void on_jogForward_released();

    void on_jogBack_released();

    void on_absoluteStart_clicked();


    void on_homeBtn_clicked();

    void on_relativeStart_clicked();

signals:
    void commandHandle(QString command,int value);
    void absolute(short profile,int start,int end,int rep);
    void realtive(short profile,int start,int end,int rep);
    void moveStop();
    void sHome(int axis);
private:
    Ui::axis *ui;
    TPid *pid;
    QTimer* timer;
    int retValue;
    double pos;
    double vel;
    double error;
    double acc;
    double prfPos;
    double prfVel;
    double prfAcc;
    long pPos;
    TJogPrm jogPrm;
    TTrapPrm trapPrm;
    QThread* absoluteThread;
    AbsoluteMove* absoluteMove;
    QThread* homeThread;
    Home *home;
    long axisStatus;
    RealtiveMove *realtiveMove;
    QThread *realtiveThread;

    LightButton *alarmBtn;
    LightButton *positiveLimitBtn;
    LightButton *negativeLimitBtn;
    LightButton *enableBtn2;
    LightButton *moveErrorBtn;
    LightButton *moveStatusBtn;
    QLabel *alarmLabel;
    QLabel *positiveLimitLabel;
    QLabel *negativeLimitLabel;
    QLabel *enableLabel;
    QLabel *moveErrorLabel;
    QLabel *moveStatusLabel;
};


class AbsoluteMove:public QObject
{
    Q_OBJECT
public:
    AbsoluteMove(QObject *parent=nullptr);
    ~AbsoluteMove(){

    }
public slots:
    void doWorks(short profile,int start,int end,int rep);
    void moveStop();

signals:
    void workFinshed();


private:
    long axisState;
    int m_stop;
};

class Home:public QObject
{
    Q_OBJECT
public:
    Home(QObject *parent=nullptr);
    ~Home(){}
public slots:
    void doWorks(short axis);
    void moveStop();
    void start();
private:
    THomeStatus tHomeSta;
    THomePrm tHomePrm;
signals:
    void workFinished();

};

class RealtiveMove:public QObject
{
    Q_OBJECT
public:
    RealtiveMove(QObject *parent=nullptr);
    ~RealtiveMove(){

    }
public slots:
    void doWorks(short profile,int start,int end,int rep);
    void moveStop();

signals:
    void workFinshed();


private:
    long axisState;
    int m_stop;
};

#endif // AXIS_H
