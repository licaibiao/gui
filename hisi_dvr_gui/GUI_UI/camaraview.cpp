/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: camaraview.cpp
*Date:     2019-06-05
*Author:   Caibiao Lee
*Version:  V1.0
*Description:视频预览子菜单功能实现
*Others:
*History:
***********************************************************/
#include "camaraview.h"
#include "ui_camaraview.h"

#include "iconhelper.h"
#include <QDebug>

#define iconSize    50
#define iconWidth   100
#define iconHeight  70
#define navBtnWidth (45*2)
#define navIcoWidth (18*2)
#define topwindowsH (40*2)


/************************************************* 
Function:    CamaraView  
Description: 构造函数  
Return: 
Others:
Author: Caibiao Lee
Date:   2019-06-05
*************************************************/
CamaraView::CamaraView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CamaraView)
{
    ui->setupUi(this);
    this->initForm();
    this ->initSet();

    m_clsIPCPManager =GuiIPCManager::GetIPCManager();

	/**设置槽函数，接收按键信息**/
    connect(m_clsIPCPManager,SIGNAL(GuiIPCPRecvKeyValue(unsigned char *)),
            this,SLOT(RecvKeyValue(unsigned char *)));
}


/************************************************* 
Function:	 ~CamaraView  
Description: 析构函数  
Return: 
Others:
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
CamaraView::~CamaraView()
{
    delete ui;
}

/************************************************* 
Function:	 initForm  
Description: 视频预览界面样式初始化  
Return: 
Others:
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
void CamaraView::initForm()
{
    ui->labTitle->setText("视频预览");

    this->setWindowTitle(ui->labTitle->text());

    this->setProperty("canMove", true);
	
    /**set the windows no border**/
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);

    /***name:flag ;vale :nav ****/
    ui->widgetTop->setProperty("flag", "nav");

    /**set the high of the top windows**/
    ui->widgetTop->setFixedHeight(topwindowsH);

    /**icon value find in https://fontawesome.com/cheatsheet?from=io **/
    IconHelper::Instance()->setIcon(ui->btnReturn, 0xf005, navIcoWidth);
    IconHelper::Instance()->setIcon(ui->btnMsg, 0xf057, navIcoWidth);

    QStringList qss;

    /**全局无焦点虚边框,全局文字颜色,全局字号**/
//    qss.append(QString("*{outline:0px;color:#FEFEFE;}"));
    qss.append(QString("QLabel#labTitle{color:#FEFEFE;;}"));
    qss.append(QString("QPushButton#btnMsg{color:#FEFEFE;;}"));
    qss.append(QString("QPushButton#btnReturn{color:#FEFEFE;;}"));

    /**主窗体背景**/
    qss.append(QString("QWidget#CamaraView{border-image:url(%1);}").arg(":/image/main.bmp"));

	/**标题字体大小**/
    qss.append(QString("QLabel#labTitle{font:%1px;}").arg(navIcoWidth));

    /**顶部导航**/
    qss.append(QString("QWidget[flag=\"nav\"]{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,"
                       "stop:0 %1,stop:1 %2);}").arg("#00688B").arg("#093746"));
    qss.append(QString("QWidget[flag=\"nav\"] QAbstractButton{background:none;border:none;"
                       "min-width:%1px;max-width:%1px;}").arg(navBtnWidth));

	/**设置样式**/
    this->setStyleSheet(qss.join(""));
}

/************************************************* 
Function:	 initSet  
Description: 初始化预览模式  
Return: 
Others:
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
void CamaraView::initSet()
{
	/**OSD 显示通道设置**/   
    checkbox << ui->display_ch_1 << ui->display_ch_2 << ui->display_ch_3 << ui->display_ch_4;
    checkbox << ui->osd_add_ch << ui->osd_add_date << ui->osd_add_timer << ui->osd_add_resolution;
    for(int i=0;i<checkbox.count();i++)
    {
        QCheckBox *box = checkbox.at(i);
        QStringList list;
        list.append(QString("QCheckBox{font:26px;color:#888888;}"));
        box->setStyleSheet(list.join(""));
        box->setChecked(true);
    }

	/**显示标签设置**/
    label << ui->display_channel << ui->display_mode << ui->osd_add_set;
    for(int i=0;i<label.count();i++)
    {
        QLabel *Lab = label.at(i);
        QStringList list;
        list.append(QString("QLabel{font:26px;color:#FFFFFF;}"));
        Lab->setStyleSheet(list.join(""));
    }

    QStringList button;
    button.append(QString("QPushButton{font:34px;color:#888888;}"));
    ui->send_button->setStyleSheet(button.join(""));

	/**显示模式设置**/
    RadioButton << ui->display_mode_1 << ui->display_mode_2 << ui->display_mode_3 << ui->display_mode_4 ;
    for(int i=0;i<RadioButton.count();i++)
    {
        QRadioButton *Button = RadioButton.at(i);
        QStringList radionutton;
        radionutton.append(QString("QRadioButton{font:26px;color:#888888;}"));
        Button->setStyleSheet(radionutton.join(""));
    }

	/**默认设置显示模式1**/
    ui->display_mode_1->setChecked(true);

}

/************************************************* 
Function:	 GetOnOffStatus  
Description: 获取开关状态  
Return: 
Others:默认全都是开的，所以该函数被屏蔽，对应的协议未修改
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
unsigned char CamaraView::GetOnOffStatus()
{
//    unsigned char l_u8OnOff = 0;

//    if(ui->radioButton_1->isChecked())
//    {
//        l_u8OnOff = 1;
//    }

//    if(ui->radioButton_2->isChecked())
//    {
//        l_u8OnOff = 0;
//    }

//    return l_u8OnOff;
}


/************************************************* 
Function:	 GetChannelSelect  
Description: 获取通道选择设置  
Return: 
Others:
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
unsigned char CamaraView::GetChannelSelect()
{
    unsigned char l_u8CHStatus = 0;
    if(ui->display_ch_1->isChecked())
    {
        l_u8CHStatus |= 0x01<<0x00;
    }
    if(ui->display_ch_2->isChecked())
    {
        l_u8CHStatus |= 0x01<<0x01;
    }
    if(ui->display_ch_3->isChecked())
    {
        l_u8CHStatus |= 0x01<<0x02;
    }
    if(ui->display_ch_4->isChecked())
    {
        l_u8CHStatus |= 0x01<<0x03;
    }

    return l_u8CHStatus;
}

/************************************************* 
Function:	 GetOSDSelect  
Description: 获取OSD选择设置
Return: 
Others:
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
unsigned char CamaraView::GetOSDSelect()
{
    unsigned char l_u8Value = 0;
    if(ui->osd_add_date->isChecked())
    {
        l_u8Value |= 0x01<<0x00;
    }
    if(ui->osd_add_timer->isChecked())
    {
        l_u8Value |= 0x01<<0x01;
    }
    if(ui->osd_add_ch->isChecked())
    {
        l_u8Value |= 0x01<<0x02;
    }
    if(ui->osd_add_resolution->isChecked())
    {
        l_u8Value |= 0x01<<0x03;
    }

    return l_u8Value;
}

/************************************************* 
Function:	 GetDisplaySelect  
Description: 获取显示模式设置
Return: 
Others:
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
unsigned char CamaraView::GetDisplaySelect()
{
    unsigned char l_u8Value = 0;
    if(ui->display_mode_1->isChecked())
    {
        l_u8Value = 1;
    }
    if(ui->display_mode_2->isChecked())
    {
        l_u8Value = 2;
    }
    if(ui->display_mode_3->isChecked())
    {
        l_u8Value = 3;
    }
    if(ui->display_mode_4->isChecked())
    {
        l_u8Value = 4;
    }

    return l_u8Value;
}

/************************************************* 
Function:	 on_btnReturn_clicked  
Description: 左上角导航栏按钮槽函数
Return: 
Others: 
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
void CamaraView::on_btnReturn_clicked()
{

}

/************************************************* 
Function:	 on_btnMsg_clicked  
Description: 右上角关闭按钮槽函数
Return: 
Others: 接收到隐藏按键时，直接关闭该菜单
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
void CamaraView::on_btnMsg_clicked()
{
    close();
}


/************************************************* 
Function:	 on_send_button_clicked
Description: 确认按键槽函数
Return: 
Others: 发送视频预览参数给设备端
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
void CamaraView::on_send_button_clicked()
{
    MSG_0x0105_S stVideoPreSet;
    stVideoPreSet.u8CHDisplay = GetChannelSelect();
    stVideoPreSet.u8DisplayMode = GetDisplaySelect();
    stVideoPreSet.u8OnOff = GetOnOffStatus();
    stVideoPreSet.u8OSDSet = GetOSDSelect();

    qDebug("u8OnOff=0x%x   u8DisplayMode=0x%x u8CHDisplay=0x%xu8OSDSet=0x%x",
           stVideoPreSet.u8OnOff,stVideoPreSet.u8DisplayMode,
           stVideoPreSet.u8CHDisplay,stVideoPreSet.u8OSDSet) ;

    m_clsIPCPManager->GuiIPCPSendVideoPreSet(stVideoPreSet);
}




/************************************************* 
Function:	 moveEvent  
Description: 重写移动函数，使菜单固定在原点位置
			 禁止鼠标拖动菜单
Return: 
Others: 
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
void CamaraView::moveEvent(QMoveEvent *event)
{
    move(0,0);
}

/************************************************* 
Function:	 RecvKeyValue  
Description: 接收按键信息
Return: 
Others: 接收到隐藏按键时，直接关闭该菜单
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
void CamaraView::RecvKeyValue(unsigned char *data)
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

