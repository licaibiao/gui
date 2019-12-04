/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: appinit.cpp
*Date:     2019-06-05
*Author:   Caibiao Lee
*Version:  V1.0
*Description:
*Others:
*History:
***********************************************************/

#ifndef APPINIT_H
#define APPINIT_H

#include <QObject>

class AppInit : public QObject
{
    Q_OBJECT
public:
    static AppInit *Instance();
    explicit AppInit(QObject *parent = 0);    

    void start();

protected:
    bool eventFilter(QObject *obj, QEvent *evt);

private:
    static AppInit *self;

signals:

public slots:
};

#endif // APPINIT_H
