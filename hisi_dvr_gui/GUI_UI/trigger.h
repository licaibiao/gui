/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: trigger.h
*Date:     2019-06-05
*Author:   Caibiao Lee
*Version:  V1.0
*Description: 触发设置菜单功能实现
*Others:
*History:
***********************************************************/

#ifndef TRIGGER_H
#define TRIGGER_H

#include <QDialog>

#include "GUI_IPCPManager/guiIPCManager.h"

class QLabel;
class QComboBox;

namespace Ui {
class Trigger;
}

class Trigger : public QDialog
{
    Q_OBJECT

public:
    explicit Trigger(QWidget *parent = 0);
    ~Trigger();

    void moveEvent(QMoveEvent *event);

private:
    void get_OnOffStaus(unsigned char &u8Onoff);
    void get_DelayStaus(unsigned char *arru8Dalay);
    void get_AuxiliaryLineStaus(unsigned char &u8AuxiliaryLine);
    void get_ChannelSelectStaus(unsigned char *arru8SelectCh);

    void Display_OnOffStaus(unsigned char u8Onoff);
    void Display_AuxiliaryLineStaus(unsigned char u8AuxiliaryLine);
    void Display_DelayStaus(unsigned char *arru8Dalay);
    void Display_ChannelSelectStaus(unsigned char *arru8SelectCh);
    void DisplayTriggerPara(MSG_0x01b0_S l_stTriggerPara);

private :
    void initForm();
    void initSet();

private slots:
    void on_btnReturn_clicked();
    void on_btnMsg_clicked();

    void RecvKeyValue(unsigned char *data);
    void ReceiveTriggerPara(unsigned char *data);
    void on_select_ok_clicked();

private:
    Ui::Trigger *ui;
    QList<QComboBox *> combobox;
    QList<QLabel *> label;
    GuiIPCManager *m_clsIPCPManager=NULL;
};

#endif // TRIGGER_H
