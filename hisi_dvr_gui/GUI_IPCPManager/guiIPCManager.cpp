/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: guiIPCManager.cpp
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

#include <QDebug>
#include <time.h>
#include <QtCore>
#include <QPoint>
#include <QCursor>
#include "GUI_IPCPManager/guiIPCManager.h"

template <typename TYPE, void* (TYPE::*GuiThreadIPCServer)(void *) >
void* thread_GuiServer_IPCHandle(void* param)
{
    ThreadPara* p = (ThreadPara*)param;
    TYPE* This =  (TYPE*)(p->ClassPara);
    This->GuiThreadIPCServer((void*)(p->ThreadPar));
    return NULL;
}

/******************************************************** 
Function:    GuiIPCManager  
Description: 构造函数
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
GuiIPCManager::GuiIPCManager(QObject *parent) : QObject(parent)
{
    GuiService_IPCHandle.bThreadStart = false;
}

/******************************************************** 
Function:    ~GuiIPCManager  
Description: 析构函数
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
GuiIPCManager::~GuiIPCManager()
{
    if(0xFFFF!=this->m_timer1s)
    {
        this->killTimer(this->m_timer1s);
        this->m_timer1s = 0xFFFF;
    }

    if(0xFFFF!=this->m_timer100ms)
    {
        this->killTimer(this->m_timer100ms);
        this->m_timer100ms = 0xFFFF;
    }
}

/******************************************************** 
Function:   GetIPCManager
Description: 获取该类的对象
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
GuiIPCManager * GuiIPCManager :: GetIPCManager()
{
    static GuiIPCManager * l_pClsGuiIPCManager = NULL;
    if (l_pClsGuiIPCManager == NULL)
    {
        l_pClsGuiIPCManager = new GuiIPCManager();
    }
    return l_pClsGuiIPCManager;
}

/******************************************************** 
Function:   GuiThreadIPCServer
Description: 该类一直在后台运行的一个线程
Input: 
OutPut: None
Return: None
Others: 
	1.该线程用来接收设备端发送过来的IPC消息
	2.通过emit将信号发送给QT对应的类对象
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
void *GuiIPCManager::GuiThreadIPCServer(void* p)
{
    THREAD_STATUS* pt =(THREAD_STATUS*)p;
    _Msg_St l_st_MsgSt={0};
    unsigned char l_u8_ScrAddr=0;
    int l_s32_RecvLen=0;
    unsigned int l_u32_DataLength=0;
    int l_s32_CmdId=0;

    Hst_Arch_Msg_Clear(QUEUE_GUI_KEY,&l_st_MsgSt);
    while(true==pt->bThreadStart)
    {
        hst_msleep(1000);
        memset(&l_st_MsgSt,0x00,sizeof(_Msg_St));
        l_s32_RecvLen= Hst_Arch_Msg_Recv(QUEUE_GUI_KEY,&l_st_MsgSt);
        if(l_s32_RecvLen>0)
        {
            l_s32_CmdId = Hst_Arch_Msg_AnalyzeGetCmdID(&l_st_MsgSt);
            l_u8_ScrAddr = Hst_Arch_Msg_AnalyzeGetSrcAddr(&l_st_MsgSt);
            l_u32_DataLength = Hst_Arch_Msg_AnalyzeGetLen(&l_st_MsgSt);

            if(WSD_CAPTURE_ADDR ==l_u8_ScrAddr)
            {
                //printf("biao debug QT %s %d  l_s32_CmdId = %d \n",__FILE__,__LINE__,l_s32_CmdId);
                switch(l_s32_CmdId)
                {
                    case QUEUE_QTGUI_QUERY_SEARCH_ACK_CMD:
                    {
                        emit GuiIPCPRecvSearchACK((unsigned char *)&l_st_MsgSt);
                        break;
                    };

                    case QUEUE_QTGUI_QUERY_DOWNLOAD_ACK_CMD:
                    {
                        emit GuiIPCPRecvDownloadACK((unsigned char *)&l_st_MsgSt);
                        break;
                    }

                    case QUEUE_QTGUI_VIDEO_PARA_QUERY_ACK_CMD:
                    {
                        emit GuiIPCPRecvParaQueryACK((unsigned char *)&l_st_MsgSt);

                        break;
                    }

                    case QUEUE_QTGUI_STATUS_REPORT_CMD:
                    {
                        emit GuiIPCPRecvStatusReportACK((unsigned char *)&l_st_MsgSt);
                        break;
                    }

                    case QUEUE_QTGUI_QUERY_KEY_CMD:
                    {
                        emit GuiIPCPRecvKeyValue((unsigned char *)&l_st_MsgSt);
                        break;
                    }

                    case QUEUE_QTGUI_QUERY_CONTROL_ACK_CMD:
                    {
                        emit GuiIPCPRecvCtrACK((unsigned char *)&l_st_MsgSt);
                        break;
                    }

                    case QUEUE_QTGUI_TRIGGER_PARA_ACK_CMD:
                    {
                        emit GuiIPCPRecvTriggerParaQueryACK((unsigned char *)&l_st_MsgSt);
                        break;
                    }

                    default :
                        break;
                }
            }
        }
    }
    return NULL;
}

/******************************************************** 
Function:   GuiThreadIPCStart
Description: 启动线程
Input: 
OutPut: None
Return: None
Others: 
	1.创建定时器
	2.创建线程
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
int GuiIPCManager::GuiThreadIPCStart()
{
    if(0xFFFF==this->m_timer1s)
    {
        this->m_timer1s = this->startTimer(1000);
    }

    if(0xFFFF==this->m_timer100ms)
    {
        this->m_timer100ms = this->startTimer(100);
    }

    if(false== GuiService_IPCHandle.bThreadStart)
    {
        GuiService_IPCHandle.bThreadStart=true;
        ThGuiService_IPCHandlePara.ClassPara=this;
        ThGuiService_IPCHandlePara.ThreadPar=&GuiService_IPCHandle;
        if(0==pthread_create(&GuiService_IPCHandle.pid, NULL, thread_GuiServer_IPCHandle<GuiIPCManager,&GuiIPCManager::GuiThreadIPCServer>,(void*)&ThGuiService_IPCHandlePara))
        {
            pthread_detach(GuiService_IPCHandle.pid);
        }
    }
    return 0;
}


/******************************************************** 
Function:   timerEvent
Description: 定时器事件处理函数
Input: 
OutPut: None
Return: None
Others: 
	1.下面被注释的代码用来模拟设备端发送过来的数据
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
//unsigned char sendTest[2048] =
//{
//    0,0,0,0,0,0,0,0,
//    0xa5,0xa5,0xa7,0xa9,0x00,0x00,0x01,0x60,0x00,0xd2,0x00,0x10,0x02,0x19,0x05,0x03,0x11,0x57,0x12,0x19,0x05,0x03,0x12,0x14,0x32,0x02,0x19,0x05,0x03,0x12,
//    0x14,0x33,0x19,0x05,0x03,0x12,0x20,0x56,0x02,0x19,0x05,0x03,0x12,0x28,0x58,0x19,0x05,0x03,0x12,0x46,0x21,0x02,0x19,0x05,0x03,0x12,0x46,0x22,0x19,0x05,
//    0x03,0x13,0x03,0x48,0x02,0x19,0x05,0x03,0x13,0x03,0x49,0x19,0x05,0x03,0x13,0x21,0x15,0x02,0x19,0x05,0x03,0x13,0x21,0x16,0x19,0x05,0x03,0x13,0x38,0x43,
//    0x02,0x19,0x05,0x03,0x13,0x38,0x44,0x19,0x05,0x03,0x13,0x56,0x10,0x02,0x19,0x05,0x03,0x13,0x56,0x11,0x19,0x05,0x03,0x14,0x08,0x58,0x02,0x19,0x05,0x03,
//    0x14,0x18,0x34,0x19,0x05,0x03,0x14,0x28,0x24,0x02,0x19,0x05,0x03,0x22,0x30,0x15,0x19,0x05,0x03,0x22,0x41,0x59,0x02,0x19,0x05,0x03,0x22,0x45,0x11,0x19,
//    0x05,0x03,0x22,0x56,0x31,0x02,0x19,0x05,0x03,0x22,0x59,0x20,0x19,0x05,0x03,0x23,0x01,0x37,0x02,0x19,0x05,0x03,0x23,0x04,0x41,0x19,0x05,0x03,0x23,0x22,
//    0x07,0x02,0x19,0x05,0x03,0x23,0x22,0x08,0x19,0x05,0x03,0x23,0x39,0x34,0x02,0x19,0x05,0x03,0x23,0x39,0x35,0x19,0x05,0x03,0x23,0x49,0x11,0x02,0x19,0x05,
//};

//unsigned char l_stMsgTest[8] = {0};

void GuiIPCManager::timerEvent(QTimerEvent *event)
{
    static unsigned int count = 0;
    //判断当前定时器对应的是哪个ld.
    if (event->timerId() == this->m_timer1s)
    {
        //qDebug() << "timer 1s"<< endl;

        GuiIPCPSendHeartBeat();
//        /**for debug**/
//        //if(count++%20 ==0)
//        {
//            //emit GuiIPCPRecvSearchACK(sendTest);
//            emit GuiIPCPRecvKeyValue(l_stMsgTest);
//        }
    }

    if (event->timerId() == this->m_timer100ms)
    {
        //GuiCursorPosiTionSet();
    }

}

/******************************************************** 
Function:   GuiCursorPosiTionSet
Description: 获取鼠标的坐标
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
int GuiIPCManager::GuiCursorPosiTionSet()
{
    QPoint l_point;

    l_point=QCursor::pos();
    //printf("X = %d Y=%x \n",l_point.x(),l_point.y());
    if((l_point.x()>=1080)||(l_point.x()<=280)||(l_point.y()>=520)||(l_point.y()<=120))
    {
        //QCursor::setPos(1079,519);
    }

    return 0;
}

/******************************************************** 
Function:   GuiIPCPSendHeartBeat
Description: 心跳函数
Input: 
OutPut: None
Return: None
Others: QT与设备端程序的心跳机制
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
int GuiIPCManager::GuiIPCPSendHeartBeat()
{
    QueueMsgPack msg;
    static unsigned int sl_u32Heartbeat = 0;
    int l_rest = 0;

    memset(&msg, 0, sizeof(QueueMsgPack));
    msg.Data = (unsigned char *)&sl_u32Heartbeat;
    msg.Len = sizeof(sl_u32Heartbeat);
    msg.SrcAddr = WSD_GUI_ADDR;
    msg.TargAddr = WSD_CAPTURE_ADDR;
    msg.CmdId = QUEUE_QTGUI_HEARTBEAT_CMD;

    l_rest = Hst_Arch_Msg_PackSend(QUEUE_CAPTURE_KEY,&msg);
    if(0==l_rest)
    {
        sl_u32Heartbeat++;
    }else
    {
    }

    return 0;
}

/******************************************************** 
Function:   GuiIPCPSendVideoPreSet
Description: 发送视频预览设置参数
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
int GuiIPCManager::GuiIPCPSendVideoPreSet(MSG_0x0105_S stVideoPreSet)
{
    QueueMsgPack msg;
    static unsigned int sl_u32Heartbeat = 0;
    int l_rest = 0;

    memset(&msg, 0, sizeof(QueueMsgPack));
    msg.Data = (unsigned char *)&stVideoPreSet;
    msg.Len = sizeof(MSG_0x0105_S);
    msg.SrcAddr = WSD_GUI_ADDR;
    msg.TargAddr = WSD_CAPTURE_ADDR;
    msg.CmdId = QUEUE_QTGUI_QUERY_VIDEO_PRE_CMD;

    l_rest = Hst_Arch_Msg_PackSend(QUEUE_CAPTURE_KEY,&msg);
    if(0==l_rest)
    {
        sl_u32Heartbeat++;
        //printf("biao debug %s Send OK \n",__FUNCTION__);
    }else
    {
        //printf("biao debug %s Send ERROR = %d \n",__FUNCTION__,l_rest);
    }

    return 0;
}

/******************************************************** 
Function:   GuiIPCPSendSearchCMD
Description: 发送视频检索命令给设备端
Input: 
OutPut: None
Return: None
Others:
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
int  GuiIPCManager::GuiIPCPSendSearchCMD(MSG_0x0106_S stSearch)
{
    QueueMsgPack msg;

    memset(&msg, 0, sizeof(QueueMsgPack));
    msg.Data = (unsigned char *)&stSearch;
    msg.Len = sizeof(MSG_0x0106_S);
    msg.SrcAddr = WSD_GUI_ADDR;
    msg.TargAddr = WSD_CAPTURE_ADDR;
    msg.CmdId = QUEUE_QTGUI_QUERY_SEARCH_CMD;

    Hst_Arch_Msg_PackSend(QUEUE_CAPTURE_KEY,&msg);

    return 0;
}

/******************************************************** 
Function:   GuiIPCPSendPlayBackCMD
Description: 发送回放命令给设备端
Input: 
OutPut: None
Return: None
Others:
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
int  GuiIPCManager::GuiIPCPSendPlayBackCMD(MSG_0x0107_S stPlayBack)
{
    QueueMsgPack msg;

    memset(&msg, 0, sizeof(QueueMsgPack));
    msg.Data = (unsigned char *)&stPlayBack;
    msg.Len = sizeof(MSG_0x0107_S);
    msg.SrcAddr = WSD_GUI_ADDR;
    msg.TargAddr = WSD_CAPTURE_ADDR;
    msg.CmdId = QUEUE_QTGUI_QUERY_PLAYBACK_CMD;

    Hst_Arch_Msg_PackSend(QUEUE_CAPTURE_KEY,&msg);

    return 0;
}

/******************************************************** 
Function:   GuiIPCPSendDownLoadCMD
Description: 发送导出命令给设备端
Input: 
OutPut: None
Return: None
Others:
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
int  GuiIPCManager::GuiIPCPSendDownLoadCMD(MSG_0x0108_S stFileExport)
{
    QueueMsgPack msg;

    memset(&msg, 0, sizeof(QueueMsgPack));
    msg.Data = (unsigned char *)&stFileExport;
    msg.Len = sizeof(MSG_0x0108_S);
    msg.SrcAddr = WSD_GUI_ADDR;
    msg.TargAddr = WSD_CAPTURE_ADDR;
    msg.CmdId = QUEUE_QTGUI_QUERY_DOWNLOAD_CMD;

    Hst_Arch_Msg_PackSend(QUEUE_CAPTURE_KEY,&msg);

    return 0;
}

/******************************************************** 
Function:   GuiIPCPSendDeviceCtrlCMD
Description: 发送设备控制命令给设备端
Input: 
OutPut: None
Return: None
Others:控制命令有：
	1.时间设置
	2.格式化sd卡
	3 系统复位
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
int GuiIPCManager::GuiIPCPSendDeviceCtrlCMD(MSG_0x0104_S stDeviceCtrl)
{
    QueueMsgPack msg;

    memset(&msg, 0, sizeof(QueueMsgPack));
    msg.Data = (unsigned char *)&stDeviceCtrl;
    msg.Len = sizeof(MSG_0x0104_S);
    msg.SrcAddr = WSD_GUI_ADDR;
    msg.TargAddr = WSD_CAPTURE_ADDR;
    msg.CmdId = QUEUE_QTGUI_QUERY_CONTROL_CMD;

    Hst_Arch_Msg_PackSend(QUEUE_CAPTURE_KEY,&msg);

    return 0;
}

/******************************************************** 
Function:   GuiIPCPSendParaQueryCMD
Description: 发送录像参数查询命令给设备端
Input: 
OutPut: None
Return: None
Others:
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
int  GuiIPCManager::GuiIPCPSendParaQueryCMD(MSG_0x010c_S stParaQuery)
{
    QueueMsgPack msg;

    memset(&msg, 0, sizeof(QueueMsgPack));
    msg.Data = (unsigned char *)&stParaQuery;
    msg.Len = sizeof(MSG_0x010c_S);
    msg.SrcAddr = WSD_GUI_ADDR;
    msg.TargAddr = WSD_CAPTURE_ADDR;
    msg.CmdId = QUEUE_QTGUI_VIDEO_PARA_QUERY_CMD;

    Hst_Arch_Msg_PackSend(QUEUE_CAPTURE_KEY,&msg);
}

/******************************************************** 
Function:   GuiIPCPSendParaSetCMD
Description: 发送录像参数设置命令给设备端
Input: 
OutPut: None
Return: None
Others:
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
int  GuiIPCManager::GuiIPCPSendParaSetCMD(MSG_0x01c1_S stParaSet)
{
    QueueMsgPack msg;

    memset(&msg, 0, sizeof(QueueMsgPack));
    msg.Data = (unsigned char *)&stParaSet;
    msg.Len = sizeof(MSG_0x01c1_S);
    msg.SrcAddr = WSD_GUI_ADDR;
    msg.TargAddr = WSD_CAPTURE_ADDR;
    msg.CmdId = QUEUE_QTGUI_VIDEO_PARA_SET_CMD;

//    HST_PrintHexBytes((unsigned char*)&stParaSet,msg.Len);

    Hst_Arch_Msg_PackSend(QUEUE_CAPTURE_KEY,&msg);
}

/******************************************************** 
Function:   GuiIPCPSendPalyBackCTLCMD
Description: 发送回放控制命令给设备端
Input: 
OutPut: None
Return: None
Others:
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
int  GuiIPCManager::GuiIPCPSendPalyBackCTLCMD(MSG_0x109_S stPlayBackCtl)
{
    QueueMsgPack msg;

    memset(&msg, 0, sizeof(QueueMsgPack));
    msg.Data = (unsigned char *)&stPlayBackCtl;
    msg.Len = sizeof(MSG_0x109_S);
    msg.SrcAddr = WSD_GUI_ADDR;
    msg.TargAddr = WSD_CAPTURE_ADDR;
    msg.CmdId = QUEUE_QTGUI_QUERY_PLAYBACK_CTL_CMD;

    Hst_Arch_Msg_PackSend(QUEUE_CAPTURE_KEY,&msg);
}

/******************************************************** 
Function:   GuiIPCPSendTriggerParaQueryCMD
Description: 发送触发查询命令给设备端
Input: 
OutPut: None
Return: None
Others:
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
int GuiIPCManager::GuiIPCPSendTriggerParaQueryCMD(MSG_0x010b_S stTriggerQuery)
{
    QueueMsgPack msg;

    memset(&msg, 0, sizeof(QueueMsgPack));
    msg.Data = (unsigned char *)&stTriggerQuery;
    msg.Len = sizeof(MSG_0x010b_S);
    msg.SrcAddr = WSD_GUI_ADDR;
    msg.TargAddr = WSD_CAPTURE_ADDR;
    msg.CmdId = QUEUE_QTGUI_QUERY_TRIGGER_PARA_CMD;

    Hst_Arch_Msg_PackSend(QUEUE_CAPTURE_KEY,&msg);
}

/******************************************************** 
Function:   GuiIPCPSendTriggerParaSetCMD
Description: 发送触发设置命令给设备端
Input: 
OutPut: None
Return: None
Others:
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
int GuiIPCManager::GuiIPCPSendTriggerParaSetCMD(MSG_0x01b1_S stTriggePara)
{
    QueueMsgPack msg;

    memset(&msg, 0, sizeof(QueueMsgPack));
    msg.Data = (unsigned char *)&stTriggePara;
    msg.Len = sizeof(MSG_0x01b1_S);
    msg.SrcAddr = WSD_GUI_ADDR;
    msg.TargAddr = WSD_CAPTURE_ADDR;
    msg.CmdId = QUEUE_QTGUI_TRIGGER_PARA_SET_CMD;

    Hst_Arch_Msg_PackSend(QUEUE_CAPTURE_KEY,&msg);
}



