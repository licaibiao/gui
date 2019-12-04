/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: video.cpp
*Date:     2019-06-05
*Author:   Caibiao Lee
*Version:  V1.0
*Description: 实现录像设置菜单二级菜单功能
*Others:
*History:
***********************************************************/
#include "video.h"
#include "ui_video.h"
#include "iconhelper.h"

#define iconSize    50
#define iconWidth   100
#define iconHeight  70
#define navBtnWidth (45*2)
#define navIcoWidth (18*2)
#define topwindowsH (40*2)

/******************************************************** 
Function:    Video  
Description: 构造函数
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
Video::Video(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Video)
{
    ui->setupUi(this);
    /**show the background**/
    this->initForm();
    this ->initSet();
    m_clsIPCPManager =GuiIPCManager::GetIPCManager();

    MSG_0x010c_S stParaQuery;
    stParaQuery.u8RecordParaRequery = 0xff;
    m_clsIPCPManager->GuiIPCPSendParaQueryCMD(stParaQuery);

    connect(m_clsIPCPManager,SIGNAL(GuiIPCPRecvKeyValue(unsigned char *)),
            this,SLOT(RecvKeyValue(unsigned char *)));

    connect(m_clsIPCPManager,SIGNAL(GuiIPCPRecvParaQueryACK(unsigned char *)),
            this,SLOT(ReceiveVideoPara(unsigned char *)));

}

/******************************************************** 
Function:	 ~Video	
Description: 析构函数
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
Video::~Video()
{
    delete ui;
    if(NULL!=videoconfigInit)
    {
        delete videoconfigInit;
    }
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
void Video::initForm()
{
    ui->labTitle->setText("录像设置");

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
    qss.append(QString("QLabel#labTitle{color:#FEFEFE;;}"));
    qss.append(QString("QPushButton#btnMsg{color:#FEFEFE;;}"));
    qss.append(QString("QPushButton#btnReturn{color:#FEFEFE;;}"));

    //主窗体背景
    qss.append(QString("QWidget#Video{border-image:url(%1);}").arg(":/image/main.bmp"));
    //标题字体大小
    qss.append(QString("QLabel#labTitle{font:%1px;}").arg(navIcoWidth));

    //顶部导航
    qss.append(QString("QWidget[flag=\"nav\"]{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,"
                       "stop:0 %1,stop:1 %2);}").arg("#00688B").arg("#093746"));
    qss.append(QString("QWidget[flag=\"nav\"] QAbstractButton{background:none;border:none;"
                       "min-width:%1px;max-width:%1px;}").arg(navBtnWidth));

    this->setStyleSheet(qss.join(""));


//    IniVideoPara();
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
void Video::initSet()
{

    label << ui->label_1 << ui->label_2 << ui->label_3 << ui->label_4 << ui->label_5 << ui->label_6 << ui->label_7;
    label << ui->label_ch1 << ui->label_ch2 << ui->label_ch3 << ui->label_ch4 ;

    for(int i=0;i<label.count();i++)
    {
        QLabel *lab = label.at(i);
        QStringList list;
        list.append(QString("QLabel{font:24px;color:#ffffff;}"));
        lab->setStyleSheet(list.join(""));
        //lab->setStyleSheet("color:#ffffff;");
    }

}

/******************************************************** 
Function:	 setVideoRecordOnOff 
Description: 设置录像开关
Input: data 录像状态，按位取值
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Video::setVideoRecordOnOff(unsigned char data)
{
    unsigned char u8RecordOnOff = 0;

    u8RecordOnOff = data &(~(0x01<<0x00));
    if(0==u8RecordOnOff)
    {
        ui->record_enable_ch1->setCurrentIndex(1);
    }else
    {
        ui->record_enable_ch1->setCurrentIndex(0);
    }

    u8RecordOnOff = data &(~(0x01<<0x01));
    if(0==u8RecordOnOff)
    {
        ui->record_enable_ch2->setCurrentIndex(1);
    }else
    {
        ui->record_enable_ch2->setCurrentIndex(0);
    }

    u8RecordOnOff = data &(~(0x01<<0x02));
    if(0==u8RecordOnOff)
    {
        ui->record_enable_ch3->setCurrentIndex(1);
    }else
    {
        ui->record_enable_ch3->setCurrentIndex(0);
    }

    u8RecordOnOff = data &(~(0x01<<0x03));
    if(0==u8RecordOnOff)
    {
        ui->record_enable_ch4->setCurrentIndex(1);
    }else
    {
        ui->record_enable_ch4->setCurrentIndex(0);
    }

}

/******************************************************** 
Function:	 setVideoResolution 
Description: 设置录像分辨率
Input: data 传入的是一个数组
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Video::setVideoResolution(unsigned char *data)
{
    unsigned char arru8Resolution[8] = {0};

    memcpy(arru8Resolution,data,sizeof(arru8Resolution));

    ui->resolution_ch1->setCurrentIndex(arru8Resolution[0]);
    ui->resolution_ch2->setCurrentIndex(arru8Resolution[1]);
    ui->resolution_ch3->setCurrentIndex(arru8Resolution[2]);
    ui->resolution_ch4->setCurrentIndex(arru8Resolution[3]);

}

/******************************************************** 
Function:	 FPStoIndex 
Description: 帧率与索引之间的一个转换
Input: u8Fps 帧率值
OutPut: None
Return: None
Others: 应为帧率值比较多，这里过滤一些值给用户选择
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
unsigned char Video::FPStoIndex(unsigned char u8Fps)
{
    unsigned char u8Index = 0;

    if(u8Fps<=8)
    {
        u8Index =  1;
    }else if(u8Fps<=12)
    {
        u8Index = 2;
    }else if(u8Fps== 15)
    {
        u8Index = 0;
    }else if(u8Fps<=18)
    {
        u8Index = 3;
    }else if(u8Fps<=20)
    {
        u8Index = 4;
    }else if(u8Fps<=23)
    {
        u8Index = 5;
    }else if(u8Fps<=25)
    {
        u8Index = 6;
    }else
    {
       u8Index = 0;
    };

    return u8Index;
}

/******************************************************** 
Function:	 setVideoFPS 
Description: 显示设备端发送过来的帧率参数
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Video::setVideoFPS(unsigned char *data)
{
    unsigned char arru8VideoFPS[8] = {0};

    memcpy(arru8VideoFPS,data,sizeof(arru8VideoFPS));

    ui->fps_ch1->setCurrentIndex(FPStoIndex(arru8VideoFPS[0]));
    ui->fps_ch2->setCurrentIndex(FPStoIndex(arru8VideoFPS[1]));
    ui->fps_ch3->setCurrentIndex(FPStoIndex(arru8VideoFPS[2]));
    ui->fps_ch4->setCurrentIndex(FPStoIndex(arru8VideoFPS[3]));
}

/******************************************************** 
Function:	 setVideoQuality 
Description: 显示设备端发送过来的编码质量参数
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Video::setVideoQuality(unsigned char *data)
{
    unsigned char arru8Quality[8] = {0};

    memcpy(arru8Quality,data,sizeof(arru8Quality));

    ui->quality_ch1->setCurrentIndex(arru8Quality[0]);
    ui->quality_ch2->setCurrentIndex(arru8Quality[1]);
    ui->quality_ch3->setCurrentIndex(arru8Quality[2]);
    ui->quality_ch4->setCurrentIndex(arru8Quality[3]);
}

/******************************************************** 
Function:	 setVideoOSDEnable 
Description: 显示设备端发送过来的OSD使能设置参数
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Video::setVideoOSDEnable(unsigned char data)
{
    unsigned char u8OSDOnOff = 0;

    u8OSDOnOff = data &(~(0x01<<0x00));
    if(0==u8OSDOnOff)
    {
        ui->osd_enable_ch1->setCurrentIndex(1);
    }else
    {
        ui->osd_enable_ch1->setCurrentIndex(0);
    }

    u8OSDOnOff = data &(~(0x01<<0x01));
    if(0==u8OSDOnOff)
    {
        ui->osd_enable_ch2->setCurrentIndex(1);
    }else
    {
        ui->osd_enable_ch2->setCurrentIndex(0);
    }

    u8OSDOnOff = data &(~(0x01<<0x02));
    if(0==u8OSDOnOff)
    {
        ui->osd_enable_ch3->setCurrentIndex(1);
    }else
    {
        ui->osd_enable_ch3->setCurrentIndex(0);
    }

    u8OSDOnOff = data &(~(0x01<<0x03));
    if(0==u8OSDOnOff)
    {
        ui->osd_enable_ch4->setCurrentIndex(1);
    }else
    {
        ui->osd_enable_ch4->setCurrentIndex(0);
    }

}

/******************************************************** 
Function:	 setVideoChName 
Description: 显示设备端发送过来的通道名字
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Video::setVideoChName(unsigned char data[8][16])
{
    char arru8ChName[8][16] = {0};
     QString l_stDisplay;

    memcpy(arru8ChName,data,sizeof(arru8ChName));

    l_stDisplay = QString(QLatin1String(arru8ChName[0]));
    ui->Ch1_name->setText(l_stDisplay);

    l_stDisplay = QString(QLatin1String(arru8ChName[1]));
    ui->Ch2_name->setText(l_stDisplay);

    l_stDisplay = QString(QLatin1String(arru8ChName[2]));
    ui->Ch3_name->setText(l_stDisplay);

    l_stDisplay = QString(QLatin1String(arru8ChName[3]));
    ui->Ch4_name->setText(l_stDisplay);

}

/******************************************************** 
Function:	 ReceiveVideoPara 
Description: 接收来自设备端发送过来的录像参数
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Video::ReceiveVideoPara(unsigned char *data)
{
    ARCH_MSG_S *l_msg = {0};
    unsigned char *l_pu8Data = NULL;
    MSG_0x01c0_S l_stRecordPara = {0};

    l_msg = (ARCH_MSG_S *)data;
    Hst_Arch_Msg_PlatformStartP(&l_pu8Data,l_msg->SomeText);
    memcpy(&l_stRecordPara,l_pu8Data,sizeof(l_stRecordPara));

    setVideoRecordOnOff(l_stRecordPara.u8RecordOnOff);
    setVideoResolution(l_stRecordPara.arru8Resolution);
    setVideoFPS(l_stRecordPara.arru8FPS);
    setVideoQuality(l_stRecordPara.arru8Quality);
    setVideoOSDEnable(l_stRecordPara.u8OSDEnable);
    setVideoChName(l_stRecordPara.arru8ChName);

}

/******************************************************** 
Function:	 getVideoRecordOnOff 
Description: 获取当前录像开关状态
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Video::getVideoRecordOnOff(unsigned char &data)
{
    unsigned int l_u8Index = 0;
    unsigned char u8Onoff = 0;

    /**CH1**/
    l_u8Index = ui->record_enable_ch1->currentIndex();
    if(0==l_u8Index)
    {
        u8Onoff |=0x01<<0x00;
    }else
    {
        u8Onoff &=~(0x01<<0x00);
    };

    /**CH2**/
    l_u8Index = ui->record_enable_ch2->currentIndex();
    if(0==l_u8Index)
    {
        u8Onoff |=0x01<<0x01;
    }else
    {
         u8Onoff &=~(0x01<<0x01);
    };

    /**CH3**/
    l_u8Index = ui->record_enable_ch3->currentIndex();
    if(0==l_u8Index)
    {
        u8Onoff |=0x01<<0x02;
    }else
    {
        u8Onoff &=~(0x01<<0x02);
    };

    /**CH4**/
    l_u8Index = ui->record_enable_ch4->currentIndex();
    if(0==l_u8Index)
    {
        u8Onoff |=0x01<<0x03;
    }else
    {
        u8Onoff &=~(0x01<<0x03);
    };

    data = u8Onoff;
}

/******************************************************** 
Function:	 getVideoResolution 
Description: 获取当前录像分辨率设置参数
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Video::getVideoResolution(unsigned char *data)
{
    data[0] = ui->resolution_ch1->currentIndex();
    data[1] = ui->resolution_ch1->currentIndex();
    data[2] = ui->resolution_ch1->currentIndex();
    data[3] = ui->resolution_ch1->currentIndex();
}


/******************************************************** 
Function:	 indexToFps 
Description: 帧率与索引之间的一个转换
Input: u8Fps 索引值
OutPut: None
Return: None
Others: 应为帧率值比较多，这里过滤一些值给用户选择
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
unsigned char Video::indexToFps(unsigned char u8Index)
{
    unsigned char u8FPS = 0;
    if(0==u8Index)
    {
        u8FPS = 15;
    }else if(1==u8Index)
    {
        u8FPS = 8;
    }else if(2==u8Index)
    {
        u8FPS = 12;
    }else if(3==u8Index)
    {
        u8FPS = 18;
    }else if(4==u8Index)
    {
        u8FPS = 20;
    }else if(5==u8Index)
    {
        u8FPS = 23;
    }else if(6==u8Index)
    {
        u8FPS = 25;
    }else
    {
        u8FPS = 15;
    }

    return u8FPS;
}

/******************************************************** 
Function:	 getVideoFPS 
Description: 获取当前的帧率值
Input: data 是一个数组
OutPut: None
Return: None
Others:
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Video::getVideoFPS(unsigned char *data)
{
    data[0] = indexToFps(ui->fps_ch1->currentIndex());
    data[1] = indexToFps(ui->fps_ch2->currentIndex());
    data[2] = indexToFps(ui->fps_ch3->currentIndex());
    data[3] = indexToFps(ui->fps_ch4->currentIndex());
}

/******************************************************** 
Function:	 getVideoQuality 
Description: 获取当前的编码质量
Input: data 是一个数组
OutPut: None
Return: None
Others:
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Video::getVideoQuality(unsigned char *data)
{
    data[0] = ui->quality_ch1->currentIndex();
    data[1] = ui->quality_ch2->currentIndex();
    data[2] = ui->quality_ch3->currentIndex();
    data[3] = ui->quality_ch4->currentIndex();
}


/******************************************************** 
Function:	 getVideoOSDEnable 
Description: 获取当前的OSD开启状态
Input: data 是一个引用
OutPut: None
Return: None
Others:
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Video::getVideoOSDEnable(unsigned char &data)
{

    unsigned int l_u8Index = 0;
    unsigned char u8Onoff = 0;

    /**CH1**/
    l_u8Index = ui->osd_enable_ch1->currentIndex();
    if(0==l_u8Index)
    {
        u8Onoff |=0x01<<0x00;
    }else
    {
        u8Onoff &=~(0x01<<0x00);
    };

    /**CH2**/
    l_u8Index = ui->osd_enable_ch2->currentIndex();
    if(0==l_u8Index)
    {
        u8Onoff |=0x01<<0x01;
    }else
    {
        u8Onoff &=~(0x01<<0x01);
    };

    /**CH3**/
    l_u8Index = ui->osd_enable_ch3->currentIndex();
    if(0==l_u8Index)
    {
        u8Onoff |=0x01<<0x02;
    }else
    {
        u8Onoff &=~(0x01<<0x02);
    };

    /**CH4**/
    l_u8Index = ui->osd_enable_ch4->currentIndex();
    if(0==l_u8Index)
    {
       u8Onoff |=0x01<<0x03;
    }else
    {
        u8Onoff &=~(0x01<<0x03);
    };

    data = u8Onoff;

}

/******************************************************** 
Function:	 getVideoChName 
Description: 获取当前通道名字设置
Input: data 是一个二维数组
OutPut: None
Return: None
Others:
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Video::getVideoChName(unsigned char data[8][16])
{
    char * s8Char;
    QByteArray ba;
    QString l_stDisplay;

    //char arru8ChName[8][16] = {0};

    l_stDisplay = ui->Ch1_name->text();
    ba = l_stDisplay.toLatin1(); // must
    s8Char = ba.data();
    memcpy(data[0],s8Char,strlen(s8Char)+1);

    l_stDisplay = ui->Ch2_name->text();
    ba = l_stDisplay.toLatin1(); // must
    s8Char = ba.data();
    memcpy(data[1],s8Char,strlen(s8Char)+1);

    l_stDisplay = ui->Ch3_name->text();
    ba = l_stDisplay.toLatin1(); // must
    s8Char = ba.data();
    memcpy(data[2],s8Char,strlen(s8Char)+1);

    l_stDisplay = ui->Ch4_name->text();
    ba = l_stDisplay.toLatin1(); // must
    s8Char = ba.data();
    memcpy(data[3],s8Char,strlen(s8Char)+1);

}

/******************************************************** 
Function:	 on_ParaSet_clicked 
Description: 参数设置按钮
Input: none
OutPut: None
Return: None
Others:
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Video::on_ParaSet_clicked()
{
    MSG_0x01c1_S stParaSet ={0};

    getVideoRecordOnOff(stParaSet.u8RecordOnOff);
    getVideoResolution(stParaSet.arru8Resolution);
    getVideoFPS(stParaSet.arru8FPS);
    getVideoQuality(stParaSet.arru8Quality);
    getVideoOSDEnable(stParaSet.u8OSDEnable);
    getVideoChName(stParaSet.arru8ChName);

    m_clsIPCPManager->GuiIPCPSendParaSetCMD(stParaSet);

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
void Video::RecvKeyValue(unsigned char *data)
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
Function:	on_btnReturn_clicked
Description: 导航栏左上按键
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Video::on_btnReturn_clicked()
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
void Video::on_btnMsg_clicked()
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
void Video::moveEvent(QMoveEvent *event)
{
        move(0,0);
}


