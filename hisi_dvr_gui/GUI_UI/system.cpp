/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: system.cpp
*Date:     2019-06-05
*Author:   Caibiao Lee
*Version:  V1.0
*Description: 实现系统设置功能菜单
*Others:
*History:
***********************************************************/

#include "system.h"
#include "ui_system.h"
#include "iconhelper.h"

#include <QMessageBox>

#define iconSize    50
#define iconWidth   100
#define iconHeight  70
#define navBtnWidth (45*2)
#define navIcoWidth (18*2)
#define topwindowsH (40*2)

/******************************************************** 
Function:    System  
Description: 构造函数
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
System::System(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::System)
{
    ui->setupUi(this);
    this->initForm();
    this ->initSet();

    if(0xFFFF==m_timer1s)
    {
        m_timer1s = this->startTimer(1000);
    }

    m_clsIPCPManager =GuiIPCManager::GetIPCManager();

    connect(m_clsIPCPManager,SIGNAL(GuiIPCPRecvKeyValue(unsigned char *)),
            this,SLOT(RecvKeyValue(unsigned char *)));

    connect(m_clsIPCPManager,SIGNAL(GuiIPCPRecvCtrACK(unsigned char *)),
            this,SLOT(RecvCtrACK(unsigned char *)));

}

/******************************************************** 
Function:	 ~System  
Description: 析构函数
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
System::~System()
{
    if(0xFFFF!=m_timer1s)
    {
        killTimer(this->m_timer1s);
        m_timer1s = 0xFFFF;
    }

    delete ui;
}

/******************************************************** 
Function:	 initForm  
Description: 初始化界面
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void System::initForm()
{
    ui->labTitle->setText("系统设置");

    this->setWindowTitle(ui->labTitle->text());

    this->setProperty("canMove", true);
    /**set the windows no border**/
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);

    /***name:flag ;vale :nav ****/
    ui->widgetTop->setProperty("flag", "nav");

    /**set the high of the top windows**/
    ui->widgetTop->setFixedHeight(topwindowsH);

    /**icon value find in https://fontawesome.com/cheatsheet?from=io **/
    /**default value btnReturn = 0xf073   btnMsg = 0xf27a**/
    IconHelper::Instance()->setIcon(ui->btnReturn, 0xf005, navIcoWidth);
    IconHelper::Instance()->setIcon(ui->btnMsg, 0xf057, navIcoWidth);

    QStringList qss;

    //全局无焦点虚边框,全局文字颜色,全局字号
//    qss.append(QString("*{outline:0px;color:#FEFEFE;}"));
    qss.append(QString("QLabel#labTitle{color:#FEFEFE;;}"));
    qss.append(QString("QPushButton#btnMsg{color:#FEFEFE;;}"));
    qss.append(QString("QPushButton#btnReturn{color:#FEFEFE;;}"));

    //主窗体背景
    qss.append(QString("QWidget#System{border-image:url(%1);}").arg(":/image/main.bmp"));
    //标题字体大小
    qss.append(QString("QLabel#labTitle{font:%1px;}").arg(navIcoWidth));

    //顶部导航
    qss.append(QString("QWidget[flag=\"nav\"]{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,"
                       "stop:0 %1,stop:1 %2);}").arg("#00688B").arg("#093746"));
    qss.append(QString("QWidget[flag=\"nav\"] QAbstractButton{background:none;border:none;"
                       "min-width:%1px;max-width:%1px;}").arg(navBtnWidth));

    this->setStyleSheet(qss.join(""));
}

/******************************************************** 
Function:	 initSet  
Description: 初始化设置
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void System::initSet()
{
    label << ui->label_1 << ui->label_2 << ui->label_3 << ui->label_4 << ui->label_5 << ui->label_6;
    for(int i=0;i<label.count();i++)
    {
        QLabel *lab = label.at(i);
        QStringList list;
        list.append(QString("QLabel{font:26px;color:#ffffff;}"));
        lab->setStyleSheet(list.join(""));
        //lab->setStyleSheet("color:#ffffff;");
    }

    /**设置默认启动的时间**/

    QDateTime SysDateTime;
    SysDateTime = QDateTime::currentDateTime();

    /**设置时间**/
    ui->dateTimeEdit->setDateTime(SysDateTime);
    ui->dateTimeEdit_2->setDateTime(SysDateTime);

    /**使能日历选择**/
    ui->dateTimeEdit_2->setCalendarPopup(true);

    /**设置时间显示样式**/
    ui->dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    ui->dateTimeEdit_2->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
}

/******************************************************** 
Function:	on_btnReturn_clicked
Description: 导航栏左上按键
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void System::on_btnReturn_clicked()
{

}

/******************************************************** 
Function:	on_btnMsg_clicked
Description: 导航栏右上按键
Input: 
OutPut: None
Return: None
Others: 退出设备状态二级菜单
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void System::on_btnMsg_clicked()
{
    close();
}

/******************************************************** 
Function:	moveEvent
Description: 重写移动事件
Input: 
OutPut: None
Return: None
Others: 禁止菜单鼠标拖动
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void System::moveEvent(QMoveEvent *event)
{
    move(0,0);
}

/******************************************************** 
Function:	timerEvent
Description: 定时器事件处理函数
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void System::timerEvent(QTimerEvent *event)
{

    if (event->timerId() == this->m_timer1s)
    {
        QDateTime SysDateTime;
        SysDateTime = QDateTime::currentDateTime();
        /**设置时间**/
        ui->dateTimeEdit->setDateTime(SysDateTime);
        /**设置时间显示样式**/
        ui->dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    }

}

/******************************************************** 
Function:	on_SetSysTimer_clicked
Description: 设置系统时间
Input: 
OutPut: None
Return: None
Others: 其中需要弹出确认框
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void System::on_SetSysTimer_clicked()
{
    QDateTime l_Date;
    MSG_0x0104_S stDeviceCtrl ={0};
    unsigned int l_sysTimer = 0;

    QMessageBox messageBox(QMessageBox::NoIcon,"系统时间设置","你确定要设置系统时间吗?",QMessageBox::Yes|QMessageBox::No,NULL);
    int result =messageBox.exec();
    switch(result)
    {
        case QMessageBox::Yes:
            {
                l_Date = ui->dateTimeEdit_2->dateTime();
                l_sysTimer = l_Date.toTime_t();

                stDeviceCtrl.u8SysTimeSet = 1;
                HST_SystemTime2BcdTime_C((time_t)l_sysTimer, stDeviceCtrl.u8SysBCDTime);

                m_clsIPCPManager->GuiIPCPSendDeviceCtrlCMD(stDeviceCtrl);
                 break;
            }

        case QMessageBox::No:
            break;

        default:
            break;
    }
}


/******************************************************** 
Function:	on_ResetSystem_clicked
Description: 系统重启
Input: 
OutPut: None
Return: None
Others: 需要弹出确认框
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void System::on_ResetSystem_clicked()
{
    MSG_0x0104_S stDeviceCtrl ={0};

    QMessageBox messageBox(QMessageBox::NoIcon,"系统复位","你确定要系统复位吗?",QMessageBox::Yes|QMessageBox::No,NULL);
    int result =messageBox.exec();
    switch(result)
    {
        case QMessageBox::Yes:
            {
                stDeviceCtrl.u8SysReboot = 1;
                m_clsIPCPManager->GuiIPCPSendDeviceCtrlCMD(stDeviceCtrl);
                 break;
            }

        case QMessageBox::No:
            break;

        default:
            break;
    }
}

/******************************************************** 
Function:	on_FormatSD_clicked
Description: 格式化SD卡
Input: 
OutPut: None
Return: None
Others: 需要弹出确认框
	重启之后格式化状态不能显示出来，这个问题后面需要优化
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void System::on_FormatSD_clicked()
{
    MSG_0x0104_S stDeviceCtrl ={0};

    QMessageBox messageBox(QMessageBox::NoIcon,"格式化","你确定要格式化吗?",QMessageBox::Yes|QMessageBox::No,NULL);
    int result =messageBox.exec();
    switch(result)
    {
        case QMessageBox::Yes:
            {
                stDeviceCtrl.u8FormatSD = 1;
                m_clsIPCPManager->GuiIPCPSendDeviceCtrlCMD(stDeviceCtrl);
                 break;
            }

        case QMessageBox::No:
            break;

        default:
        break;
    }
}


/******************************************************** 
Function:	RecvKeyValue
Description: 接收设备端发送过来的按键信息
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void System::RecvKeyValue(unsigned char *data)
{
    ARCH_MSG_S *l_msg = {0};
    unsigned char *l_pu8Data = NULL;
    MSG_0x01a0_S l_stKey = {0};

    l_msg = (ARCH_MSG_S *)data;
    Hst_Arch_Msg_PlatformStartP(&l_pu8Data,l_msg->SomeText);
    memcpy(&l_stKey,l_pu8Data,sizeof(MSG_0x01a0_S));

    if(0!=l_stKey.u8KeyType)
    {
        switch(l_stKey.u8KeyValue)
        {

        case KEY_DISPLAY:
        {
            if(!this->isHidden())
            {
                this->close();
            }
            break;
        }
        case KEY_EXIT:
        {
            this->close();
            break;
        }

        default :
            break;

        }
    }
}
/******************************************************** 
Function:	RecvCtrACK
Description: 接收设备端发送回来的格式化状态
Input: 
OutPut: None
Return: None
Others: 该状态在重启之后，如果没有点开该二级菜单是不会响应的
	SD卡格式化状态显示应该放到主界面弹框显示
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void System::RecvCtrACK(unsigned char *data)
{
    ARCH_MSG_S *l_msg = {0};
    unsigned char *l_pu8Data = NULL;
    MSG_0x0140_S l_stACK = {0};

    l_msg = (ARCH_MSG_S *)data;
    Hst_Arch_Msg_PlatformStartP(&l_pu8Data,l_msg->SomeText);
    memcpy(&l_stACK,l_pu8Data,sizeof(MSG_0x0140_S));

    switch(l_stACK.u8FormatSDACK)
    {
        case FORMAT_ING:
        {
            ui->FormatSD->setText("正在格式化中！！！");
            break;
        }

        case FORMAT_OK:
        {
            ui->FormatSD->setText("格式化完成");
            break;
        }

        case FORMAT_ERROR:
        {
            ui->FormatSD->setText("格式化失败");
            break;
        }

        case FORMAT_NEED_AGAIN:
        {
            ui->FormatSD->setText("需要重新格式化");
            break;
        }

    default:
        break;

    }
}



