/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: guiIPCManager.h
*Date:     2019-06-05
*Author:   Caibiao Lee
*Version:  V1.0
*Description: QT 与设备端的进行间通讯实现
*Others: 
	1.属于静态类，在该类中会运行一个线程，程序运行时被拉起
	2.QT对设备的所有收发信息由该模块实现
	3.QT各对象之间使用信号槽函数通信
*History:
***********************************************************/

#ifndef _GUIIPCManager_H
#define _GUIIPCManager_H

#include <QObject>

#include "IPCP_LIB/HstlibIpcpInterface.h"


class GuiIPCManager : public QObject
{
    Q_OBJECT
public:
     explicit GuiIPCManager(QObject *parent = 0);
    ~GuiIPCManager();
    static GuiIPCManager * GetIPCManager();
    void* GuiThreadIPCServer(void* p);
    int GuiThreadIPCStart();

protected:
    void timerEvent(QTimerEvent*event);
    int GuiCursorPosiTionSet();

public:
    int GuiIPCPSendHeartBeat();
    int GuiIPCPSendParaSetCMD(MSG_0x01c1_S stParaSet);
    int GuiIPCPSendParaQueryCMD(MSG_0x010c_S stParaQuery);
    int GuiIPCPSendDeviceCtrlCMD(MSG_0x0104_S stDeviceCtrl);
    int GuiIPCPSendVideoPreSet(MSG_0x0105_S stVideoPreSet);
    int GuiIPCPSendSearchCMD(MSG_0x0106_S stSearch);
    int GuiIPCPSendPlayBackCMD(MSG_0x0107_S stPlayBack);
    int GuiIPCPSendDownLoadCMD(MSG_0x0108_S stFileExport);
    int GuiIPCPSendPalyBackCTLCMD(MSG_0x109_S stPlayBackCtl);
    int GuiIPCPSendTriggerParaQueryCMD(MSG_0x010b_S stTriggerQuery);
    int GuiIPCPSendTriggerParaSetCMD(MSG_0x01b1_S stTriggePara);

signals:
    void GuiIPCPRecvSearchACK(unsigned char *Data);
    void GuiIPCPRecvDownloadACK(unsigned char *Data);
    void GuiIPCPRecvParaQueryACK(unsigned char *Data);
    void GuiIPCPRecvStatusReportACK(unsigned char *Data);
    void GuiIPCPRecvKeyValue(unsigned char *Data);
    void GuiIPCPRecvCtrACK(unsigned char *Data);
    void GuiIPCPRecvTriggerParaQueryACK(unsigned char *Data);


private:
    THREAD_STATUS GuiService_IPCHandle;
    ThreadPara ThGuiService_IPCHandlePara;
    int m_timer1s = 0xFFFF;
    int m_timer100ms = 0xFFFF;

};

#endif
