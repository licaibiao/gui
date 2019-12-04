/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: storage.h
*Date:     2019-06-05
*Author:   Caibiao Lee
*Version:  V1.0
*Description: 实现设备状态显示、设备状态查询功能
*Others:
*History:
***********************************************************/
#ifndef STORAGE_H
#define STORAGE_H
#include <QDialog>

#include "GUI_IPCPManager/guiIPCManager.h"

class QLabel;

namespace Ui {
class Storage;
}

class Storage : public QDialog
{
    Q_OBJECT

public:
    explicit Storage(QWidget *parent = 0);
    ~Storage();

    void moveEvent(QMoveEvent *event);

private:
    void initForm();
    void initSet();

private slots:
    void on_btnReturn_clicked();
    void on_btnMsg_clicked();

public slots:
    void RecvStatusReportACK(unsigned char * data);
    void RecvKeyValue(unsigned char *data);

public:
    void  ShowCamaraStarus(MSG_CAMERA_STATUS_S  data);
    void  ShowStorageStarus(MSG_SD_STATUS_S data);
    void  ShowRecordStarus(MSG_RECORD_STATUS_S data);


private:
    Ui::Storage *ui;
    QList<QLabel *> label;
    GuiIPCManager *m_clsIPCPManager=NULL;

};

#endif // STORAGE_H
