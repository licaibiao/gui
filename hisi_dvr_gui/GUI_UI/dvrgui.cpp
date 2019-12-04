/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: dvrgui.cpp
*Date:     2019-06-05
*Author:   Caibiao Lee
*Version:  V1.0
*Description: 一级主菜单界面功能实现
*Others:
*History:
***********************************************************/
#pragma execution_character_set("utf-8")

#include "dvrgui.h"
#include "ui_dvrgui.h"
#include "iconhelper.h"
#include "video.h"
#include "search.h"
#include "storage.h"
#include "users.h"
#include "system.h"
#include "trigger.h"
#include "camaraview.h"
#include <QPoint>

#define iconSize    70
#define iconWidth   140
#define iconHeight  85

#define navBtnWidth (45*2)
#define navIcoWidth (18*2)
#define topwindowsH (60)

/************************************************* 
Function:    DVRgui  
Description: 构造函数 初始化各种参数 
Return: 
Others:
Author: Caibiao Lee
Date:   2019-06-05
*************************************************/
DVRgui::DVRgui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DVRgui)
{
    /**show the ui**/
    ui->setupUi(this);

    /**show the background**/
    this->initForm();

    /**show the icon**/
    this->initNav();

    clsGuiIPCManager = GuiIPCManager::GetIPCManager();
    if(NULL!=clsGuiIPCManager)
    {
        clsGuiIPCManager->GuiThreadIPCStart();
    }

	/**事件过滤**/
    //ui->widgetMain->installEventFilter(this);

	/**绑定信号和槽函数**/
    connect(clsGuiIPCManager,SIGNAL(GuiIPCPRecvKeyValue(unsigned char *)),
            this,SLOT(RecvKeyValue(unsigned char *)));

	/**开启一个10s的定时器**/
    this->m_timerID = this->startTimer(10000);

}
	
/************************************************* 
Function:	 ~DVRgui  
Description: 析构函数
Return: 
Others:
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
DVRgui::~DVRgui()
{
    delete ui;
}

/************************************************* 
Function:	 initForm  
Description: 初始化样式
Return: 
Others:
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
void DVRgui::initForm()
{
    ui->labTitle->setText("主界面");

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
    qss.append(QString("*{outline:0px;color:#FEFEFE;}"));

    //主窗体背景
    qss.append(QString("QWidget#widgetMain{border-image:url(%1);}").arg(":/image/main.bmp"));

    //标题字体大小
    qss.append(QString("QLabel#labTitle{font:%1px;}").arg(navIcoWidth));

    //顶部导航
    qss.append(QString("QWidget[flag=\"nav\"]{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,"
                       "stop:0 %1,stop:1 %2);}").arg("#00688B").arg("#093746"));
    qss.append(QString("QWidget[flag=\"nav\"] QAbstractButton{background:none;border:none;"
                       "min-width:%1px;max-width:%1px;}").arg(navBtnWidth));

    this->setStyleSheet(qss.join(""));
}


/************************************************* 
Function:	 initNav  
Description: 初始化选择栏
Return: 
Others:
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
void DVRgui::initNav()
{
    QList<QString> listColorBg;
    listColorBg << "#1570A5" << "#16A085" << "#C0392B" << "#047058" << "#9B59BB" << "#34495E";
    QList<QString> listColorText;
    listColorText << "#FEFEFE" << "#FEFEFE" << "#FEFEFE" << "#FEFEFE" << "#FEFEFE" << "#FEFEFE";

    /**Define QChar Variable**/
    QList<QChar> listChar;
    listChar << 0xf03d << 0xf1c8 << 0xf002 << 0xf030 << 0xf083 << 0xf085;
    QList<QString> listText;
    listText << "视频预览" << "录像设置" << "录像查询" << "设备状态"  << "触发设置" << "系统设置";

    /**Add QToolButton To QList**/
    btns << ui->btnViewMap << ui->btnViewPanel << ui->btnData << ui->btnMap << ui->btnDevice << ui->btnConfig;

    /**set Style Sheet of the QToolButton**/
    for (int i = 0; i < btns.count(); i++) {

        /**Returns the item at index position i in the list.**/
        QToolButton *btn = btns.at(i);
        btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        btn->setIconSize(QSize(iconWidth, iconHeight));

        QPixmap pix = IconHelper::Instance()->getPixmap(listColorText.at(i), listChar.at(i), iconSize, iconWidth, iconHeight);
        btn->setIcon(QIcon(pix));
        btn->setText(listText.at(i));

        QStringList list;
        list.append(QString("QToolButton{font:%1px;background:%2;}").arg(iconSize / 2.5).arg(listColorBg.at(i)));
        list.append(QString("QToolButton{border:none;border-radius:8px;padding:30px;}"));
        list.append(QString("QToolButton:pressed{background:%1;}").arg("#737A97"));
        btn->setStyleSheet(list.join(""));

        connect(btn, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
    }
}

/************************************************* 
Function:	 buttonClicked  
Description: 按键槽函数
Return: 
Others: 在initNav 函数中被绑定
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
void DVRgui::buttonClicked()
{
    QToolButton *btn = (QToolButton *)sender();

    m_bKeyDownFlag = true;
    if(btn->text()=="录像查询")
    {
        Search   search_win;
        search_win.show();
        search_win.exec();

    }else if(btn->text()=="触发设置")
    {

        Trigger  trigger_win;

        trigger_win.show();
        trigger_win.exec();
    }
    else if(btn->text()=="视频预览")
    {

        CamaraView cameraview_win;
        cameraview_win.show();
        cameraview_win.exec();
    }
    else if(btn->text()=="录像设置")
    {
        Video video_win;
        video_win.show();
        video_win.exec();

    }
    else if(btn->text()=="设备状态")
    {  
        Storage  storge_win;
        storge_win.show();
        storge_win.exec();
    }
    else if(btn->text()=="系统设置")
    { 
        System  system_win;
        system_win.show();
        system_win.exec();

    }else
    {

    }

}

/************************************************* 
Function:	 on_btnReturn_pressed  
Description: 左上导航按键
Return: 
Others: 
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
void DVRgui::on_btnReturn_pressed()
{
    int index = ui->stackedWidget->currentIndex();
    if (index != 0) {
        ui->stackedWidget->setCurrentIndex(0);
        ui->labTitle->setText("主界面");
        IconHelper::Instance()->setIcon(ui->btnReturn, 0xf005, navIcoWidth);
    }

    this->setWindowOpacity(1);
}

/************************************************* 
Function:	 on_btnMsg_pressed  
Description: 右上导航按键
Return: 
Others: 点击关闭不是实际关闭，而是隐藏
	1.注意鼠标的隐藏
	2.鼠标离开了菜单之后失效
	3.setVisible 在海思开发板上不生效
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
void DVRgui::on_btnMsg_pressed()
{
	  /**方法1**/
//    this->setWindowOpacity(0);
//    this->setAttribute( Qt::WA_TranslucentBackground,true );
//    this->setWindowFlags( Qt::WindowMinimizeButtonHint );
//    exit(0);GuiIPCPSendHeartBeat

	  /**方法2**/
//    this->setVisible(true);
//    this->setHidden(true);

     /**方法3**/
	this->setHidden(true);
	this->setCursor(Qt::BlankCursor);	//隐藏鼠标

}

/************************************************* 
Function:	 on_btnReturn_clicked  
Description: 左上导航按键
Return: 
Others: 
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
void DVRgui::on_btnReturn_clicked()
{

    m_bKeyDownFlag = true;

}

/************************************************* 
Function:	 timerEvent  
Description: 定时器事件处理函数
Return: 
Others: 
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
void DVRgui::timerEvent(QTimerEvent*event)
{
    if (event->timerId() == this->m_timerID)
    {
        if(false==m_bKeyDownFlag)
        {
            m_bKeyDownFlag = true;
            this->setHidden(true);
            this->setCursor(Qt::BlankCursor);   //隐藏鼠标
        }
         this->killTimer(this->m_timerID);
    }

}

/************************************************* 
Function:	 moveEvent  
Description: 移动事件处理函数
Return: 
Others: 进制鼠标拖动界面
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
void DVRgui::moveEvent(QMoveEvent *event)
{
        move(0,0);
}


/************************************************* 
Function:	 eventFilter  
Description: 事件过滤函数
Return: 
Others: 
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
bool DVRgui::eventFilter(QObject *watched, QEvent *event)
{
   if(event->type() == QEvent::MouseButtonPress)
   {
        qDebug() << "MouseButtonPress"<< endl;
   }

   return QWidget::eventFilter(watched, event);
}

/************************************************* 
Function:	 RecvKeyValue  
Description: 接收来自设备的按键消息
Return: 
Others: 
Author: Caibiao Lee
Date:	2019-06-05
*************************************************/
void DVRgui::RecvKeyValue(unsigned char *data)
{
    ARCH_MSG_S *l_msg = {0};
    unsigned char *l_pu8Data = NULL;
    MSG_0x01a0_S l_stKey = {0};

    l_msg = (ARCH_MSG_S *)data;
    Hst_Arch_Msg_PlatformStartP(&l_pu8Data,l_msg->SomeText);
    memcpy(&l_stKey,l_pu8Data,sizeof(MSG_0x01a0_S));

    if(0!=l_stKey.u8KeyType)
    {
        if(KEY_DISPLAY ==l_stKey.u8KeyValue)
        {
            if(this->isHidden())
            {
                this->setCursor(Qt::ArrowCursor); //显示正常鼠标
                this->setHidden(false);
            }else
            {
                this->setCursor(Qt::BlankCursor);   //隐藏鼠标
                this->setHidden(true);
            }
        }
    }
}

