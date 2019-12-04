/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: trigger.cpp
*Date:     2019-06-05
*Author:   Caibiao Lee
*Version:  V1.0
*Description: 触发设置菜单功能实现
*Others:
*History:
***********************************************************/
#include "trigger.h"
#include "ui_trigger.h"
#include "iconhelper.h"

#define iconSize    50
#define iconWidth   100
#define iconHeight  70
#define navBtnWidth (45*2)
#define navIcoWidth (18*2)
#define topwindowsH (40*2)

/******************************************************** 
Function:    Trigger  
Description: 构造函数
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
Trigger::Trigger(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Trigger)
{
    ui->setupUi(this);
    this->initForm();
    this ->initSet();

    m_clsIPCPManager =GuiIPCManager::GetIPCManager();

    MSG_0x010b_S stTriggerQuery;
    stTriggerQuery.u8ParaRequery = 1;
    m_clsIPCPManager->GuiIPCPSendTriggerParaQueryCMD(stTriggerQuery);

    connect(m_clsIPCPManager,SIGNAL(GuiIPCPRecvKeyValue(unsigned char *)),
            this,SLOT(RecvKeyValue(unsigned char *)));

    connect(m_clsIPCPManager,SIGNAL(GuiIPCPRecvTriggerParaQueryACK(unsigned char *)),
            this,SLOT(ReceiveTriggerPara(unsigned char *)));



}

/******************************************************** 
Function:	 ~Trigger  
Description: 析构函数
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
Trigger::~Trigger()
{
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
void Trigger::initForm()
{
    ui->labTitle->setText("触发设置");

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
    qss.append(QString("QWidget#Trigger{border-image:url(%1);}").arg(":/image/main.bmp"));
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
Description: 初始化界面设置
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Trigger::initSet()
{
    label << ui->label_1 << ui->label_2 << ui->label_3 << ui->label_4 << ui->label_5 ;
    label << ui->label_ch1 << ui->label_ch2 << ui->label_ch3 << ui->label_ch4 ;

    for(int i=0;i<label.count();i++)
    {
        QLabel *lab = label.at(i);
        QStringList list;
        list.append(QString("QLabel{font:26px;color:#ffffff;}"));
        lab->setStyleSheet(list.join(""));
        //lab->setStyleSheet("color:#ffffff;");
    }

}

/******************************************************** 
Function:	 RecvKeyValue 
Description: 接收设备端发送过来的按键信息
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Trigger::RecvKeyValue(unsigned char *data)
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
Function:	 Display_OnOffStaus 
Description: 显示开关状态
Input: u8Onoff 开关状态，按位取值
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Trigger::Display_OnOffStaus(unsigned char u8Onoff)
{
    char u8OnOffStatus = 0;

    u8OnOffStatus = u8Onoff &(~(0x01<<0x00));
    if(0==u8OnOffStatus)
    {
        ui->Back1_OnOff->setCurrentIndex(0);
    }else
    {
        ui->Back1_OnOff->setCurrentIndex(1);
    }

    u8OnOffStatus = u8Onoff &(~(0x01<<0x01));
    if(0==u8OnOffStatus)
    {
        ui->Back2_OnOff->setCurrentIndex(0);
    }else
    {
        ui->Back2_OnOff->setCurrentIndex(1);
    }

    u8OnOffStatus = u8Onoff &(~(0x01<<0x02));
    if(0==u8OnOffStatus)
    {
        ui->Back3_OnOff->setCurrentIndex(0);
    }else
    {
        ui->Back3_OnOff->setCurrentIndex(1);
    }

    u8OnOffStatus = u8Onoff &(~(0x01<<0x03));
    if(0==u8OnOffStatus)
    {
        ui->Back4_Onoff->setCurrentIndex(0);
    }else
    {
        ui->Back4_Onoff->setCurrentIndex(1);
    }
}

/******************************************************** 
Function:	 Display_AuxiliaryLineStaus 
Description: 显示辅助线设置状态
Input: u8AuxiliaryLine 开关状态，按位取值
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Trigger::Display_AuxiliaryLineStaus(unsigned char u8AuxiliaryLine)
{
    unsigned char u8AuxiliaryLineStatus = 0;

    /**Back1**/
    u8AuxiliaryLineStatus = u8AuxiliaryLine &(~(0x01<<0x00));
    if(0==u8AuxiliaryLineStatus)
    {
        ui->Back1_Line->setCurrentIndex(0);
    }else
    {
        ui->Back1_Line->setCurrentIndex(1);
    }

    /**Back2**/
    u8AuxiliaryLineStatus = u8AuxiliaryLine &(~(0x01<<0x01));
    if(0==u8AuxiliaryLineStatus)
    {
        ui->Back2_Line->setCurrentIndex(0);
    }else
    {
        ui->Back2_Line->setCurrentIndex(1);
    }

    /**Back3**/
    u8AuxiliaryLineStatus = u8AuxiliaryLine &(~(0x01<<0x02));
    if(0==u8AuxiliaryLineStatus)
    {
        ui->Back3_Line->setCurrentIndex(0);
    }else
    {
        ui->Back3_Line->setCurrentIndex(1);
    }

    /**Back4**/
    u8AuxiliaryLineStatus = u8AuxiliaryLine &(~(0x01<<0x03));
    if(0==u8AuxiliaryLineStatus)
    {
        ui->Back4_Line->setCurrentIndex(0);
    }else
    {
        ui->Back4_Line->setCurrentIndex(1);
    }
}

/******************************************************** 
Function:	 Display_DelayStaus 
Description: 显示延时设置状态
Input: arru8Dalay 传入的是一个数组
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Trigger::Display_DelayStaus(unsigned char *arru8Dalay)
{
    ui->Back1_Delay->setCurrentIndex(arru8Dalay[0]);
    ui->Back2_Delay->setCurrentIndex(arru8Dalay[1]);
    ui->Back3_Delay->setCurrentIndex(arru8Dalay[2]);
    ui->Back4_Delay->setCurrentIndex(arru8Dalay[3]);
}

/******************************************************** 
Function:	 Display_ChannelSelectStaus 
Description: 显示通道选择
Input: arru8Dalay 传入的是一个数组
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Trigger::Display_ChannelSelectStaus(unsigned char *arru8SelectCh)
{
    ui->Back1_Ch->setCurrentIndex(arru8SelectCh[0]);
    ui->Back2_Ch->setCurrentIndex(arru8SelectCh[1]);
    ui->Back3_Ch->setCurrentIndex(arru8SelectCh[2]);
    ui->Back4_Ch->setCurrentIndex(arru8SelectCh[3]);
}

/******************************************************** 
Function:	 DisplayTriggerPara 
Description: 显示触发设置
Input: l_stTriggerPara 设备端发过来的数据
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Trigger::DisplayTriggerPara(MSG_0x01b0_S l_stTriggerPara)
{
    Display_OnOffStaus(l_stTriggerPara.arru8OnOff);
    Display_AuxiliaryLineStaus(l_stTriggerPara.arru8AuxiliaryLine);
    Display_DelayStaus(l_stTriggerPara.arru8Delay);
    Display_ChannelSelectStaus(l_stTriggerPara.arru8PreviewCH);
}

/******************************************************** 
Function:	 ReceiveTriggerPara 
Description: 接收设备端发送过来的触发信息
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Trigger::ReceiveTriggerPara(unsigned char *data)
{
    ARCH_MSG_S *l_msg = {0};
    unsigned char *l_pu8Data = NULL;
    MSG_0x01b0_S l_stTriggerPara = {0};

    l_msg = (ARCH_MSG_S *)data;
    Hst_Arch_Msg_PlatformStartP(&l_pu8Data,l_msg->SomeText);
    memcpy(&l_stTriggerPara,l_pu8Data,sizeof(MSG_0x01b0_S));

    HST_PrintHexBytes(l_pu8Data,sizeof(MSG_0x01b0_S));

    DisplayTriggerPara(l_stTriggerPara);
}

/******************************************************** 
Function:	 get_OnOffStaus 
Description: 获取当前开关状态
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Trigger::get_OnOffStaus(unsigned char &u8Onoff)
{
    u8Onoff = 0;
    unsigned int l_u8Index = 0;

    /**Back1**/
    l_u8Index = ui->Back1_OnOff->currentIndex();
    if(0==l_u8Index)
    {
        u8Onoff &=~(0x01<<0x00);
    }else
    {
        u8Onoff |=0x01<<0x00;
    };

    /**Back2**/
    l_u8Index = ui->Back2_OnOff->currentIndex();
    if(0==l_u8Index)
    {
        u8Onoff &=~(0x01<<0x01);
    }else
    {
        u8Onoff |=0x01<<0x01;
    };

    /**Back3**/
    l_u8Index = ui->Back3_OnOff->currentIndex();
    if(0==l_u8Index)
    {
        u8Onoff &=~(0x01<<0x02);
    }else
    {
        u8Onoff |=0x01<<0x02;
    };
    /**Back4**/
    l_u8Index = ui->Back4_Onoff->currentIndex();
    if(0==l_u8Index)
    {
        u8Onoff &=~(0x01<<0x03);
    }else
    {
        u8Onoff |=0x01<<0x03;
    };
}

/******************************************************** 
Function:	 get_AuxiliaryLineStaus 
Description: 获取当前辅助线设置状态
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Trigger::get_AuxiliaryLineStaus(unsigned char &u8AuxiliaryLine)
{
    unsigned char l_u8Index = 0;

    /**Back1**/
    l_u8Index = ui->Back1_Line->currentIndex();
    if(0==l_u8Index)
    {
        u8AuxiliaryLine &=~(0x01<<0x00);
    }else
    {
        u8AuxiliaryLine |=0x01<<0x00;
    };

    /**Back2**/
    l_u8Index = ui->Back2_Line->currentIndex();
    if(0==l_u8Index)
    {
        u8AuxiliaryLine &=~(0x01<<0x01);
    }else
    {
        u8AuxiliaryLine |=0x01<<0x01;
    };

    /**Back3**/
    l_u8Index = ui->Back3_Line->currentIndex();
    if(0==l_u8Index)
    {
        u8AuxiliaryLine &=~(0x01<<0x02);
    }else
    {
        u8AuxiliaryLine |=0x01<<0x02;
    };

    /**Back4**/
    l_u8Index = ui->Back4_Line->currentIndex();
    if(0==l_u8Index)
    {
        u8AuxiliaryLine &=~(0x01<<0x03);
    }else
    {
        u8AuxiliaryLine |=0x01<<0x03;
    };
}

/******************************************************** 
Function:	 get_DelayStaus 
Description: 获取当前延时设置状态
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Trigger::get_DelayStaus(unsigned char *arru8Dalay)
{
    arru8Dalay[0] = ui->Back1_Delay->currentIndex();
    arru8Dalay[1] = ui->Back2_Delay->currentIndex();
    arru8Dalay[2] = ui->Back3_Delay->currentIndex();
    arru8Dalay[3] = ui->Back4_Delay->currentIndex();
}

/******************************************************** 
Function:	 get_ChannelSelectStaus 
Description: 获取当前通道选择状态
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Trigger::get_ChannelSelectStaus(unsigned char *arru8SelectCh)
{
    arru8SelectCh[0] = ui->Back1_Ch->currentIndex();
    arru8SelectCh[1] = ui->Back2_Ch->currentIndex();
    arru8SelectCh[2] = ui->Back3_Ch->currentIndex();
    arru8SelectCh[3] = ui->Back4_Ch->currentIndex();
}

/******************************************************** 
Function:	 on_select_ok_clicked 
Description: 将当前的设置状态发送给设备端
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Trigger::on_select_ok_clicked()
{
    MSG_0x01b1_S stTriggePara = {0};

    get_OnOffStaus(stTriggePara.arru8OnOff);
    get_DelayStaus(stTriggePara.arru8Delay);
    get_ChannelSelectStaus(stTriggePara.arru8PreviewCH);
    get_AuxiliaryLineStaus(stTriggePara.arru8AuxiliaryLine);

    HST_PrintHexBytes((unsigned char*)&stTriggePara,sizeof(MSG_0x01b1_S));

    m_clsIPCPManager->GuiIPCPSendTriggerParaSetCMD(stTriggePara);
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
void Trigger::on_btnReturn_clicked()
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
void Trigger::on_btnMsg_clicked()
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
void Trigger::moveEvent(QMoveEvent *event)
{
    move(0,0);
}



