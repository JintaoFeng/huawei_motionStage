#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gts.h"
#include <QString>
#include <QDebug>
#include "axis.h"
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void CommandHandler(QString command,int value);

private slots:
    void ShowTime();
    void on_openBtn_clicked();

    void on_closeBtn_clicked();

    void on_cfgBtn_clicked();

    void on_killStopBtn_clicked();

    void on_enableAllBtn_clicked();

    void on_homeAll_clicked();

private:
    Ui::MainWindow *ui;
    int retValue;
    axis* axis1;
    axis* axis2;
    axis* axis3;
    long axisStatus1;
    long axisStatus2;
    long axisStatus3;
    QThread *homeThread1;
    QThread *homeThread2;
    QThread *homeThread3;

    Home *home1;
    Home *home2;
    Home *home3;
signals:
    void axis1Init();
    void axis2Init();
    void axis3Init();
    void updateStart();
    void updateTerm();
};
#endif // MAINWINDOW_H
