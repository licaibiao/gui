/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: search.cpp
*Date:     2019-06-05
*Author:   Caibiao Lee
*Version:  V1.0
*Description: 实现录像查询二级菜单功能
*Others:
*History:
***********************************************************/
#include "search.h"
#include "ui_search.h"
#include "iconhelper.h"

#include "qdebug.h"
#include "qdesktopwidget.h"
#include "qdatetime.h"
#include <QMouseEvent>

#define iconSize    50
#define iconWidth   100
#define iconHeight  70
#define navBtnWidth (45*2)
#define navIcoWidth (18*2)
#define topwindowsH (40*2)

#define AXIS_ORIGIN_X   30  //坐标原点 X
#define AXIS_ORIGIN_Y   195 //坐标原点 Y
#define PAINTER_WIDTH   800 //画布宽
#define PAINTER_HEIGHT  230 //画布高

int g_pointx=AXIS_ORIGIN_X;
int g_pointy=AXIS_ORIGIN_Y;//确定坐标轴起点坐标，这里定义(30,200)
int g_width=PAINTER_WIDTH - g_pointx - 10;
int g_height=PAINTER_HEIGHT - 50;   //确定坐标轴宽度跟高度。

int g_mouseX = 0;  //鼠标在绘图板上X坐标值
int g_mouseY = 0;  //鼠标在绘图板上Y坐标值

const int g_HourNum    = 25; //X轴被分割成25等份，表示时间0~24点    
const int g_ChannelNum = 5; //Y轴被分割成5等份,表示1~4通道

int g_s32hours  = 0;    //在回放界面鼠标点击坐标的小时值
int g_s32munite = 0;    //在回放界面鼠标点击坐标的分钟值
int g_s32second  = 0;   //在回放界面鼠标点击坐标的秒值


IPCP_REACH_ACK_S g_result = {0};

/************************************************* 
Function:    Search  
Description: 构造函数  
Return: None
Others:None
Author: Caibiao Lee
Date:   2019-06-05
*************************************************/
Search::Search(QWidget *parent) : QDialog(parent),  ui(new Ui::Search)
{
    ui->setupUi(this);
    /**show the background**/
    this->initForm();

    this ->initSet();
	
	/**初始化显示下载界面**/
    this ->initDownload();
	
	/**初始化鼠标追踪，在tabwidget，鼠标事件被遮盖了，
	监听不到鼠标事件，需要使用重写事件过滤函数来检测鼠标事件**/
    this ->setMouseTracking(true);
	
	/**启动事件过滤**/
    ui->PlayBack->installEventFilter(this);

    m_clsIPCPManager =GuiIPCManager::GetIPCManager();

	/**QT 不同类之间，使用信号和槽函数的机制进行数据的传输**/
    connect(m_clsIPCPManager,SIGNAL(GuiIPCPRecvSearchACK(unsigned char *)),
            this,SLOT(RecvSearchACK(unsigned char *)));

    connect(m_clsIPCPManager,SIGNAL(GuiIPCPRecvKeyValue(unsigned char *)),
            this,SLOT(RecvKeyValue(unsigned char *)));

}

/************************************************* 
Function:    ~Search  
Description: 析构函数  
Return: None
Others:None
Author: Caibiao Lee
Date:   2019-06-05
*************************************************/
Search::~Search()
{  
    delete ui;
}

/************************************************* 
Function:    initForm  
Description: 初始化界面  
Return: None
Others:None
Author: Caibiao Lee
Date:   2019-06-05
*************************************************/
void Search::initForm()
{
    ui->labTitle->setText("录像查询");

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
    qss.append(QString("QWidget#Search{border-image:url(%1);}").arg(":/image/main.bmp"));
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
Function:    initSet  
Description: 初始化设置  
Return: None
Others:None
Author: Caibiao Lee
Date:   2019-06-05
*************************************************/
void Search::initSet()
{
   label << ui->label_1 << ui->label_2 << ui->label_3 ;

    for(int i=0;i<label.count();i++)
    {
        QLabel *lab = label.at(i);
        QStringList list;
        list.append(QString("QLabel{font:24px;color:#ffffff;}"));
        lab->setStyleSheet(list.join(""));
    }

    checkbox << ui->checkBox_1 << ui->checkBox_2 << ui->checkBox_3 << ui->checkBox_4 ;
    for(int i=0;i<checkbox.count();i++)
    {
        QCheckBox *box = checkbox.at(i);
        QStringList list;
        list.append(QString("QCheckBox{font:24px;color:#888888;}"));
        box->setStyleSheet(list.join(""));
        box->setChecked(true);
    }

	/**设置默认启动的时间**/
    QTime time ;
    QDateTime startdateTime;
    QDateTime enddateTime;

    startdateTime = QDateTime::currentDateTime();
    enddateTime   = QDateTime::currentDateTime();

	/**设置时间**/
    startdateTime.setTime(time);
    ui->dateTimeEdit->setDateTime(startdateTime);
    ui->dateTimeEdit_2->setDateTime(enddateTime);

	/**使能日历选择**/
    ui->dateTimeEdit->setCalendarPopup(true);
    ui->dateTimeEdit_2->setCalendarPopup(true);

	/**设置时间显示样式**/
    ui->dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    ui->dateTimeEdit_2->setDisplayFormat("yyyy-MM-dd HH:mm:ss");

}

/******************************************************** 
Function:    initDownload  
Description:初始化下载界面
Input:  None
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
void Search::initDownload()
{
    //设置列数和列宽
    int width = qApp->desktop()->availableGeometry().width() - 120;
    ui->SearchResult->setColumnCount(5);
	/**在PC机与在海思设备显示不一样**/
//    ui->SearchResult->setColumnWidth(0, width * 0.06);
//    ui->SearchResult->setColumnWidth(1, width * 0.20);
//    ui->SearchResult->setColumnWidth(2, widtg_resulth * 0.20);
//    ui->SearchResult->setColumnWidth(3, width * 0.08);
//    ui->SearchResult->setColumnWidth(4, width * 0.06);
    ui->SearchResult->setColumnWidth(0, width * 0.1);
    ui->SearchResult->setColumnWidth(1, width * 0.30);
    ui->SearchResult->setColumnWidth(2, width * 0.30);
    ui->SearchResult->setColumnWidth(3, width * 0.2);
    ui->SearchResult->setColumnWidth(4, width * 0.1);

    ui->SearchResult->verticalHeader()->setDefaultSectionSize(25);

    QStringList headText;
    headText << "通道号" << "开始时间" << "结束时间" << "文件大小" << "操作";
    ui->SearchResult->setHorizontalHeaderLabels(headText);
    ui->SearchResult->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->SearchResult->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->SearchResult->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->SearchResult->setAlternatingRowColors(true);
    ui->SearchResult->verticalHeader()->setVisible(false);
    ui->SearchResult->horizontalHeader()->setStretchLastSection(true);

    //设置行高
    ui->SearchResult->setRowCount(3);

    ui->PlayBack->setStyleSheet("background-color:white;");

}
/******************************************************** 
Function:    eventFilter  
Description: 重写事件过滤函数
Input:  None
OutPut: None
Return: None
Others: 
    1.直接在某个组件上新建画布画图，会提示出错，提示该画布没激活。
	可以使用update函数更新画面，使界面重新绘制。
	2.这里监听鼠标按键事件和画面重新绘制事件
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
bool Search::eventFilter(QObject *watched, QEvent *event)
{
   if(watched == ui->PlayBack && event->type() == QEvent::Paint)
   {
       DrawReplayPaint();
   }

   if(watched == ui->PlayBack && event->type() == QEvent::MouseButtonPress)
   {
        mousePositionCalc();
   }

   return QWidget::eventFilter(watched, event);
}

/******************************************************** 
Function:    moveEvent  
Description: 重写移动事件
Input:  None
OutPut: None
Return: None
Others: 用来禁止菜单的移动
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
void Search::moveEvent(QMoveEvent *event)
{
    move(0,0);
}


/******************************************************** 
Function:    DrawReplayPaint  
Description: 绘制回放界面的画板，包括时间轴和通道号，还有画布框。
Input:  None
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
void Search::DrawReplayPaint()
{
    QPainter painter(ui->PlayBack);

    painter.setRenderHint(QPainter::Antialiasing, true);//设置反锯齿模式，好看一点
    painter.drawRect(5,5,PAINTER_WIDTH-10,PAINTER_HEIGHT-10);//外围的矩形，从(5,5)起周围留了5的间隙。

    //绘制坐标轴 坐标轴原点(30，200)
    painter.drawLine(g_pointx,g_pointy,g_width+g_pointx,g_pointy);//坐标轴x宽度为width
    painter.drawLine(g_pointx,g_pointy-g_height,g_pointx,g_pointy);//坐标轴y高度为height
    //srand(time(NULL));

    //绘制刻度线
    QPen penDegree;
    penDegree.setColor(Qt::black);
    penDegree.setWidth(2);
    painter.setPen(penDegree);


    //画上x轴刻度线
    for(int i=0;i<=g_HourNum;)
    {
        //选取合适的坐标，绘制一段长度为4的直线，用于表示刻度
        painter.drawLine(g_pointx+ i*g_width/g_HourNum,g_pointy,
                         g_pointx+ i*g_width/g_HourNum,g_pointy+4);
        painter.drawText(g_pointx+ (i-0.5)*g_width/g_HourNum,g_pointy+g_HourNum,
                         QString("%1 %2").arg(i).arg(":00"));
		//刻度太小，只显示偶数时间刻度				 
        i = i+2;
    }
    //y轴刻度线
    for(int i=1;i<g_ChannelNum;i++)
    {
        //代码较长，但是掌握基本原理即可。
        //主要就是确定一个位置，然后画一条短短的直线表示刻度。
        painter.drawLine(g_pointx,g_pointy-(i)*g_height/g_ChannelNum,
                         g_pointx-4,g_pointy-(i)*g_height/g_ChannelNum);
        painter.drawText(g_pointx-20,g_pointy-(i-0.15)*g_height/g_ChannelNum,
                         QString::number((int)(i)));
    }

    if(true == m_Showpain)
    {
        ShowRepaly();
    }

    if(true == m_ShowPoint)
    {
        DrawPointPaint();
    }
}

/******************************************************** 
Function:    mousePositionCalc  
Description: 计算鼠标在画布中的位置
Input:  None
OutPut: None
Return: None
Others: 
	1.QCursor::pos() 获取到的是鼠标的绝对位置。
	2.pos().x() 获取到的是Search.ui左上角绝对坐标。
	3.g_mouseX 是在画布中的坐标
	4.如果调整了画布组件，需要重新校准
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
void Search::mousePositionCalc()
{
    QPoint pt = QCursor::pos();//获取鼠标的绝对位置
//    qDebug("QCursor x =%d   y=%d ", pt.x(),pt.y()) ;
//    qDebug("tabWidget().x() %d   y=%d ", pos().x(),pos().y());
//    qDebug("g_mouseX =%d  g_mouseY =%d ",g_mouseX,g_mouseY);

    g_mouseX = pt.x() - pos().x() - 2;
    g_mouseY = pt.y() - pos().y() - 250;

    m_ShowPoint = true;
    QWidget::update();
}

/******************************************************** 
Function:    DrawPointPaint  
Description: 在回放界面，左键点击坐标系内，显示此坐标表示的时间
Input:  None
OutPut: None
Return: None
Others: 

Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
void Search::DrawPointPaint()
{
    QPainter painter(ui->PlayBack);
    painter.setRenderHint(QPainter::Antialiasing, true);//设置反锯齿模式，好看一点
    QPen penDegree;
    penDegree.setWidth(2);
    penDegree.setColor(Qt::blue);
    painter.setPen(penDegree);
    if(g_mouseX<30)
    {
        painter.drawLine(30,20,30,195);
    }else
    {
        painter.drawLine(g_mouseX,20,g_mouseX,195);
    }

    double l_hours = 0;
    double l_munite = 0;
    double l_second = 0;

    l_hours  =  g_mouseX /(g_width/25.0);
    g_s32hours = (int)l_hours;
    l_munite = (l_hours-g_s32hours)*60;
    g_s32munite = (int)l_munite;
    l_second    = (l_munite - g_s32munite)*60;
    g_s32second = (int)l_second;

    if((g_s32hours < 22)&&(g_s32hours>=1))
    {
        painter.drawText(g_mouseX+2,g_mouseY,
          QString("%1 %2 %3 %4 %5 %6 %7").arg("(")\
             .arg(g_s32hours-1, 2, 10, QChar('0')).arg(":")\
             .arg(g_s32munite,2, 10, QChar('0')).arg(":")\
             .arg(g_s32second,2, 10, QChar('0')).arg(")"));
    }else if((g_s32hours >= 22 )&&(g_s32hours < 24))
    {
        painter.drawText(g_mouseX-100,g_mouseY,
          QString("%1 %2 %3 %4 %5 %6 %7").arg("(")\
             .arg(g_s32hours-1, 2, 10, QChar('0')).arg(":")\
             .arg(g_s32munite,2, 10, QChar('0')).arg(":")\
             .arg(g_s32second,2, 10, QChar('0')).arg(")"));
    }else if(g_s32hours >= 24)
    {
        painter.drawText(g_mouseX-100,g_mouseY,
          QString("%1 %2 %3 %4 %5 %6 %7").arg("(")\
             .arg("23").arg(":").arg("59").arg(":").arg("59").arg(")"));
    }else if(g_s32hours<1)
    {
        painter.drawText(30,g_mouseY,
          QString("%1 %2 %3 %4 %5 %6 %7").arg("(")\
             .arg("00").arg(":").arg("00").arg(":").arg("00").arg(")"));
    }
}

/******************************************************** 
Function:    ShowRepaly  
Description: 根据检索结果，将有录制视频的时间段绘制出来
Input:  None
OutPut: None
Return: None
Others: 

Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
void Search::ShowRepaly()
{
    QPainter painter(ui->PlayBack);
    painter.setRenderHint(QPainter::Antialiasing, true);//设置反锯齿模式，好看一点


    QPen penDegree;
    penDegree.setColor(Qt::white);
    penDegree.setWidth(25);
    painter.setPen(penDegree);

    int g_ChannelNum = 5;
    for(int i=1;i<g_ChannelNum;i++)
    {
        painter.drawLine(g_pointx+14,g_pointy-(i)*g_height/g_ChannelNum,
                         g_pointx+g_width-10,g_pointy-(i)*g_height/g_ChannelNum);

    }

    penDegree.setColor(Qt::darkBlue);
    penDegree.setWidth(1);
    painter.setPen(penDegree);

    for (int i = 0; i < g_result.u16TotalResources; i++)
    {
        QDateTime l_timer1 = QDateTime::fromTime_t(g_result.data[i].arru32SysStartTimer);
        QDateTime l_timer2 = QDateTime::fromTime_t(g_result.data[i].arru32SysEndTimer);
        QString l_stTimer1 = l_timer1.toString("hh:mm:ss");
        QString l_stTimer2 = l_timer2.toString("hh:mm:ss");

        QString l_stTimer1HH = l_stTimer1.mid(0, 2);  // == "hh"
        QString l_stTimer1MM = l_stTimer1.mid(3, 2);  // == "mm"
        QString l_stTimer1SS = l_stTimer1.mid(6, 2);  // == "ss"
        QString l_stTimer2HH = l_stTimer2.mid(0, 2);  // == "hh"
        QString l_stTimer2MM = l_stTimer2.mid(3, 2);  // == "mm"
        QString l_stTimer2SS = l_stTimer2.mid(6, 2);  // == "ss"

        bool l_bRes;
        unsigned int SecondsTimer1 = l_stTimer1SS.toInt(&l_bRes,10)+\
            l_stTimer1MM.toInt(&l_bRes,10)*60+ l_stTimer1HH.toInt(&l_bRes,10)*60*60;

        unsigned int SecondsTimer2 = l_stTimer2SS.toInt(&l_bRes,10)+\
            l_stTimer2MM.toInt(&l_bRes,10)*60 + l_stTimer2HH.toInt(&l_bRes,10)*60*60;

        int l_channel = g_result.data[i].u8Channel;

        int l_S32StartX = (SecondsTimer1)*(g_width/90000.0) ; //3600*25 = 90000 = 1day;
        int l_s32EndX = (SecondsTimer2)*(g_width/90000.0);

//        printf("biao debug SecondsTimer1 = %d  SecondsTimer2 = %d\
//               l_S32StartX = %d l_s32EndX = %d\n",SecondsTimer1,SecondsTimer2,l_S32StartX,l_s32EndX);

        painter.drawLine(g_pointx+l_S32StartX,(g_pointy-(l_channel)*g_height/g_ChannelNum),
                            g_pointx+l_s32EndX,(g_pointy-(l_channel)*g_height/g_ChannelNum));

        for(int j=1;j<12;j++)
        {
            painter.drawLine(g_pointx+l_S32StartX,(g_pointy-(l_channel)*g_height/g_ChannelNum)+j,
                                g_pointx+l_s32EndX,(g_pointy-(l_channel)*g_height/g_ChannelNum)+j);

            painter.drawLine(g_pointx+l_S32StartX,(g_pointy-(l_channel)*g_height/g_ChannelNum)-j,
                                g_pointx+l_s32EndX,(g_pointy-(l_channel)*g_height/g_ChannelNum)-j);
        }

    }
}


/******************************************************** 
Function:    ShowDownload  
Description: 根据检索结果，将检索结果显示在下载界面
Input:  None
OutPut: None
Return: None
Others: 

Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
void Search::ShowDownload()
{
    if(0==g_result.u16StartSerNum)
    {
        ui->SearchResult->setRowCount(g_result.u16TotalResources);
    }

    for (int i = g_result.u16StartSerNum; i <g_result.u16CurResources ; i++) {
        ui->SearchResult->setRowHeight(i, 24);

        QDateTime l_timer1 = QDateTime::fromTime_t(g_result.data[i].arru32SysStartTimer);
        QDateTime l_timer2 = QDateTime::fromTime_t(g_result.data[i].arru32SysEndTimer);

        QTableWidgetItem *ItemChannelNum= new QTableWidgetItem(QString::number(g_result.data[i].u8Channel));
        QTableWidgetItem *ItemStartTimer = new QTableWidgetItem(l_timer1.toString("yyyy-MM-dd hh:mm:ss"));
        QTableWidgetItem *ItemEnfTimer = new QTableWidgetItem(l_timer2.toString("yyyy-MM-dd hh:mm:ss"));
        QTableWidgetItem *ItemSize = new QTableWidgetItem("--");
        QTableWidgetItem *ItemOperate = new QTableWidgetItem("下载");

        ui->SearchResult->setItem(i, 0, ItemChannelNum);
        ui->SearchResult->setItem(i, 1, ItemStartTimer);
        ui->SearchResult->setItem(i, 2, ItemEnfTimer);
        ui->SearchResult->setItem(i, 3, ItemSize);
        ui->SearchResult->setItem(i, 4, ItemOperate);
    }
}

/******************************************************** 
Function:    mousePressEvent  
Description: 
Input:  None
OutPut: None
Return: None
Others: 
	1.在Search这个ui，左键事件检测不到，只能检测右键，需要重载事件过滤函数

Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
void Search::mousePressEvent(QMouseEvent *event)
{

}

/********************************************************
Function:    on_btnReturn_clicked
Description: Search ui 左上按键槽函数
Input:  None
OutPut: None
Return: None
Others:
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
void Search::on_btnReturn_clicked()
{
//    int l_channel = g_result.data[0].u8Channel;
}

/********************************************************
Function:    on_btnMsg_clicked
Description: Search ui 右上按键槽函数
Input:  None
OutPut: None
Return: None
Others:
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
void Search::on_btnMsg_clicked()
{
    close();
}

/******************************************************** 
Function:    on_SearchButton_clicked  
Description: 按下发送检索命令
Input:  None
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
void Search::on_SearchButton_clicked()
{

    MSG_0x0106_S stSearch = {0};
    unsigned int l_Time = 0;

    stSearch.u8Channel = GetChannelSelect();

    l_Time = GetStartTimer();
    HST_SystemTime2BcdTime_C((time_t)l_Time, stSearch.arru8StartBCDTime);

    l_Time = GetEndTimer();
    HST_SystemTime2BcdTime_C((time_t)l_Time, stSearch.arru8EndBCDTime);

    m_clsIPCPManager->GuiIPCPSendSearchCMD(stSearch);
}

/********************************************************
Function:    on_DownlodButton_clicked
Description: 按下发送下载命令
Input:  None
OutPut: None
Return: None
Others:
Author: Caibiao Lee
Date:   2019-05-02
*********************************************************/
void Search::on_DownlodButton_clicked()
{   
    int row = 0;
    MSG_0x0108_S stFileExport ={0};

    row = ui->SearchResult->currentRow();

    stFileExport.u8Channel = g_result.data[row].u8Channel ;

    HST_SystemTime2BcdTime_C((time_t )g_result.data[row].arru32SysStartTimer,\
            stFileExport.arru8StartBCDTime);
    HST_SystemTime2BcdTime_C((time_t )g_result.data[row].arru32SysEndTimer,\
            stFileExport.arru8EndBCDTime);

    HST_PrintHexBytes((unsigned char *)&stFileExport,sizeof(MSG_0x0108_S));
    m_clsIPCPManager->GuiIPCPSendDownLoadCMD(stFileExport);
}

/********************************************************
Function:    on_PlayBackButton_clicked
Description: 按下发送回放命令
Input:  None
OutPut: None
Return: None
Others:
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
void Search::on_PlayBackButton_clicked()
{
    int row = 0;
    MSG_0x0107_S stPlayBack ={0};

    row = ui->SearchResult->currentRow();

    stPlayBack.u8Channel = g_result.data[row].u8Channel ;
    HST_SystemTime2BcdTime_C((time_t )g_result.data[row].arru32SysStartTimer,\
            stPlayBack.arru8StartBCDTime);
    HST_SystemTime2BcdTime_C((time_t )g_result.data[row].arru32SysEndTimer,\
            stPlayBack.arru8EndBCDTime);

    HST_PrintHexBytes((unsigned char *)&stPlayBack,sizeof(MSG_0x0107_S));
    m_clsIPCPManager->GuiIPCPSendPlayBackCMD(stPlayBack);
}



/********************************************************
Function:    GetChannelSelect
Description: 获取通道选择参数
Input:  None
OutPut: None
Return: None
Others:
Author: Caibiao Lee
Date:   2019-05-02
*********************************************************/
unsigned char Search::GetChannelSelect()
{
    unsigned char l_u8CHStatus = 0;

    if(ui->checkBox_1->isChecked())
    {
        l_u8CHStatus |= 0x01<<0x00;
    }
    if(ui->checkBox_2->isChecked())
    {
        l_u8CHStatus |= 0x01<<0x01;
    }
    if(ui->checkBox_3->isChecked())
    {
        l_u8CHStatus |= 0x01<<0x02;
    }
    if(ui->checkBox_4->isChecked())
    {
        l_u8CHStatus |= 0x01<<0x03;
    }

    return l_u8CHStatus;
}


/********************************************************
Function:    GetStartTimer
Description: 获取检索开始时间
Input:  None
OutPut: None
Return: None
Others: 这里获取到的是以秒为单位的时间
Author: Caibiao Lee
Date:   2019-05-02
*********************************************************/
unsigned int Search::GetStartTimer(void)
{
    QDateTime l_Date;
    l_Date = ui->dateTimeEdit->dateTime();

    return l_Date.toTime_t();
}

/********************************************************
Function:    GetEndTimer
Description: 获取检索的结束时间
Input:  None
OutPut: None
Return: None
Others: 这里获取到的是以秒为单位的时间
Author: Caibiao Lee
Date:   2019-05-02
*********************************************************/
unsigned int Search::GetEndTimer(void)
{
    QDateTime l_Date;
    l_Date = ui->dateTimeEdit_2->dateTime();
    return l_Date.toTime_t();
}


/********************************************************
Function:    RecvSearchACK
Description: 接收检索结果
Input:  None
OutPut: None
Return: None
Others:
Author: Caibiao Lee
Date:   2019-05-02
*********************************************************/
//void Search::RecvSearchACK(unsigned char* data)
//{
//    ARCH_MSG_S *l_msg = {0};
//    unsigned char *l_pu8Data = NULL;
//    unsigned int l_u32_DataLength = 0;

//    l_msg = (ARCH_MSG_S *)data;

//    l_u32_DataLength = Hst_Arch_Msg_AnalyzeGetLen(l_msg);
//    Hst_Arch_Msg_PlatformStartP(&l_pu8Data,l_msg->SomeText);


//    printf("l_u32_DataLength = 0x%x \n",l_u32_DataLength);

//    int i = 0;
//    g_result.u16TotalResources = ((unsigned int)l_pu8Data[i++] << 8)|((unsigned int)l_pu8Data[i++]);

//    for(int j=0;j<g_result.u16TotalResources;j++)
//    {
//        g_result.data[j].u8Channel = l_pu8Data[i++];
//        HST_BcdTime2SysTime_C(&l_pu8Data[i],&g_result.data[j].arru32SysStartTimer);
//        i = i+6;
//        HST_BcdTime2SysTime_C(&l_pu8Data[i],&g_result.data[j].arru32SysEndTimer);
//        i = i+6;

////        printf("i = %d l_channel %d l_sysStartTime %d l_sysEndTime %d \n",
////         j,g_result.data[j].u8Channel, g_result.data[j].arru32SysStartTimer,
////               g_result.data[j].arru32SysEndTimer);
//    }

//    ShowDownload();


//    m_Showpain = true;
//    QWidget::update();

//    //qDebug("recive data ") ;
//    //HST_PrintHexBytes(l_pu8Data,l_u32_DataLength);

//}

/********************************************************
Function:    RecvSearchACK
Description: 接收检索结果
Input:  None
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:   2019-05-02
*********************************************************/
void Search::RecvSearchACK(unsigned char* data)
{
    ARCH_MSG_S *l_msg = {0};
    unsigned char *l_pu8Data = NULL;
    unsigned int l_u32_DataLength = 0;

    l_msg = (ARCH_MSG_S *)data;

    l_u32_DataLength = Hst_Arch_Msg_AnalyzeGetLen(l_msg);
    Hst_Arch_Msg_PlatformStartP(&l_pu8Data,l_msg->SomeText);

    MSG_Rec_ACK_S stRecHead = {0};
    memcpy(&stRecHead,l_pu8Data,sizeof(MSG_Rec_ACK_S));

    g_result.u16CurResources = stRecHead.u16curResourcesSize;
    g_result.u16StartSerNum  = stRecHead.u16starSerNum;
    g_result.u16TotalResources = stRecHead.u16TotalResources;

    if(g_result.u16TotalResources >=(sizeof(g_result.data)/sizeof(ONE_REACH_ACK_S)))
    {
        g_result.u16TotalResources = sizeof(g_result.data)/sizeof(ONE_REACH_ACK_S);
    }

    int j = stRecHead.u16starSerNum;
    int i = 6;

    for(int k=0;k<stRecHead.u16curResourcesSize;k++)
    {
        g_result.data[j].u8Channel = l_pu8Data[i++];
        HST_BcdTime2SysTime_C(&l_pu8Data[i],&g_result.data[j].arru32SysStartTimer);
        i = i+6;
        HST_BcdTime2SysTime_C(&l_pu8Data[i],&g_result.data[j].arru32SysEndTimer);
        i = i+6;

        j++;
        if(j>=g_result.u16TotalResources)
        {
            j=g_result.u16TotalResources;
        }
    }

    ShowDownload();

    m_Showpain = true;
    QWidget::update();

    //qDebug("recive data ") ;
    //HST_PrintHexBytes(l_pu8Data,l_u32_DataLength);

}

/********************************************************
Function:    RecvKeyValue
Description: 接收设备发来的按键信息
Input:  None
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:   2019-05-02
*********************************************************/
void Search::RecvKeyValue(unsigned char *data)
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
Function:    on_PlayBackPause_clicked
Description: 回放暂停
Input:  None
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:   2019-05-02
*********************************************************/
void Search::on_PlayBackPause_clicked()
{
    MSG_0x109_S stPlayBackCtl ={0};

    stPlayBackCtl.u8CtrType = 2;
    m_clsIPCPManager->GuiIPCPSendPalyBackCTLCMD(stPlayBackCtl);
}

/********************************************************
Function:    on_PlayBackExit_clicked
Description: 回放退出
Input:  None
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:   2019-05-02
*********************************************************/
void Search::on_PlayBackExit_clicked()
{
    MSG_0x109_S stPlayBackCtl ={0};

    stPlayBackCtl.u8CtrType = 1;
    m_clsIPCPManager->GuiIPCPSendPalyBackCTLCMD(stPlayBackCtl);
}

/********************************************************
Function:    on_PlayBackDubleSpeed_clicked
Description: 快速播放
Input:  None
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:   2019-05-02
*********************************************************/
void Search::on_PlayBackDubleSpeed_clicked()
{
    MSG_0x109_S stPlayBackCtl ={0};

    stPlayBackCtl.u8CtrType = 3;
    m_clsIPCPManager->GuiIPCPSendPalyBackCTLCMD(stPlayBackCtl);
}
