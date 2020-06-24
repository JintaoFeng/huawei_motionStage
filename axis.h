#ifndef AXIS_H
#define AXIS_H

#include <QWidget>
#include <QString>
#include <QTimer>
#include "gts.h"

namespace Ui {
class axis;
}

class axis : public QWidget
{
    Q_OBJECT

public:
    explicit axis(QWidget *parent = nullptr,QString name="Axis");
    ~axis();
    void uiInit();
private slots:
    void on_enableBtn_clicked();
    void timerOut();

signals:
    void commandHandle(QString command,int value);
private:
    Ui::axis *ui;
    TPid *pid;
    int retValue;
    double pos;
    double vel;
    double error;
};

#endif // AXIS_H
