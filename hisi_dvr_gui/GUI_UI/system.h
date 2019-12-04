/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: system.h
*Date:     2019-06-05
*Author:   Caibiao Lee
*Version:  V1.0
*Description: 实现系统设置功能菜单
*Others:
*History:
***********************************************************/

#ifndef SYSTEM_H
#define SYSTEM_H

#include <QDialog>
#include "GUI_IPCPManager/guiIPCManager.h"

class QLabel;

namespace Ui {
class System;
}

class System : public QDialog
{
    Q_OBJECT

public:
    explicit System(QWidget *parent = 0);
    ~System();

    void moveEvent(QMoveEvent *event);
    void timerEvent(QTimerEvent *event);

private slots:
    void initForm();
    void initSet();

private slots:
    void on_btnReturn_clicked();
    void on_btnMsg_clicked();

    void on_SetSysTimer_clicked();
    void on_ResetSystem_clicked();
    void on_FormatSD_clicked();
    void RecvKeyValue(unsigned char *data);
    void RecvCtrACK(unsigned char *data);

private:
    Ui::System *ui;
    QList<QLabel *> label;
    int m_timer1s = 0xFFFF;
    GuiIPCManager *m_clsIPCPManager=NULL;
};

#endif // SYSTEM_H
