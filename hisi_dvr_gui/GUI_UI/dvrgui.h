/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: dvrgui.h
*Date:     2019-06-05
*Author:   Caibiao Lee
*Version:  V1.0
*Description: 一级主菜单界面功能实现
*Others:
*History:
***********************************************************/
#ifndef DVR_GUI_H
#define DVR_GUI_H

#include <QWidget>
#include "GUI_IPCPManager/guiIPCManager.h"

class QToolButton;

namespace Ui {
class DVRgui;
}

class DVRgui : public QWidget
{
    Q_OBJECT

public:
    explicit DVRgui(QWidget *parent = 0);
    ~DVRgui();

protected:
    void timerEvent(QTimerEvent*event);
    void moveEvent(QMoveEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void initForm();
    void initNav();
    void buttonClicked();

private slots:
    void on_btnReturn_pressed();
    void on_btnMsg_pressed();
    void on_btnReturn_clicked();
    void RecvKeyValue(unsigned char *data);

private:
    Ui::DVRgui *ui;
    QList<QToolButton *> btns;
    int m_timerID = 0xFFFF;
    GuiIPCManager *clsGuiIPCManager=NULL;
    bool m_bKeyDownFlag = false;

};

#endif // DVR_GUI_H