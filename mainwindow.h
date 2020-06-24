#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gts.h"
#include <QString>
#include <QDebug>
#include "axis.h"
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

private:
    Ui::MainWindow *ui;
    int retValue;
    axis* axis1;
    axis* axis2;
    axis* axis3;


};
#endif // MAINWINDOW_H
