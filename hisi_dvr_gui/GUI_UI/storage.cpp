/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: storage.cpp
*Date:     2019-06-05
*Author:   Caibiao Lee
*Version:  V1.0
*Description: 实现设备状态显示、设备状态查询功能
*Others:
*History:
***********************************************************/

#include "storage.h"
#include "ui_storage.h"
#include "iconhelper.h"

#define iconSize    50
#define iconWidth   100
#define iconHeight  70
#define navBtnWidth (45*2)
#define navIcoWidth (18*2)
#define topwindowsH (40*2)

//#define __UBUNTU_USE_
#define CHANNEL_NUM     4


typedef enum
{
    QTGUI_STATUS_CAMERA=1,
    QTGUI_STATUS_STORAGE=2,
    QTGUI_STATUS_REC=3,
}QTGUI_STATUS_E;

/******************************************************** 
Function:    Storage  
Description: 构造函数
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:   2019-05-01
*********************************************************/
Storage::Storage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Storage)
{
    ui->setupUi(this);
    this->initForm();
    this ->initSet();

    m_clsIPCPManager =GuiIPCManager::GetIPCManager();

    /**QT 不同类之间，使用信号和槽函数的机制进行数据的传输**/
    connect(m_clsIPCPManager,SIGNAL(GuiIPCPRecvStatusReportACK(unsigned char *)),
            this,SLOT(RecvStatusReportACK(unsigned char *)));

    connect(m_clsIPCPManager,SIGNAL(GuiIPCPRecvKeyValue(unsigned char *)),
            this,SLOT(RecvKeyValue(unsigned char *)));
}

/******************************************************** 
Function:	 ~Storage  
Description: 析构函数
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
Storage::~Storage()
{
    delete ui;
}

/******************************************************** 
Function:	initForm
Description: 初始化界面
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Storage::initForm()
{
    ui->labTitle->setText("设备状态");

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
    qss.append(QString("QWidget#Storage{border-image:url(%1);}").arg(":/image/main.bmp"));
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
Function:	initSet
Description: 初始化设置
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Storage::initSet()
{
    //设置列数和列宽
    int width = qApp->desktop()->availableGeometry().width() - 120;
    ui->iuput_table->setColumnCount(4);
#ifdef __UBUNTU_USE_
    ui->iuput_table->setColumnWidth(0, width * 0.10);
    ui->iuput_table->setColumnWidth(1, width * 0.20);
    ui->iuput_table->setColumnWidth(2, width * 0.20);
    ui->iuput_table->setColumnWidth(3, width * 0.08);
#else
    ui->iuput_table->setColumnWidth(0, width * 0.3);
    ui->iuput_table->setColumnWidth(1, width * 0.3);
    ui->iuput_table->setColumnWidth(2, width * 0.2);
    ui->iuput_table->setColumnWidth(3, width * 0.2);
#endif
    ui->iuput_table->verticalHeader()->setDefaultSectionSize(35);

    QStringList headText;
    headText << "摄像头通道号" << "是否有输入" << "分辨率" << "是否启用";
    ui->iuput_table->setHorizontalHeaderLabels(headText);
    ui->iuput_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->iuput_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->iuput_table->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->iuput_table->setAlternatingRowColors(true);
    ui->iuput_table->verticalHeader()->setVisible(false);
    ui->iuput_table->horizontalHeader()->setStretchLastSection(true);

    //设置行高
    ui->iuput_table->setRowCount(CHANNEL_NUM);


    //设置列数和列宽
    ui->record_table->setColumnCount(3);
#ifdef __UBUNTU_USE_
    ui->record_table->setColumnWidth(0, width * 0.1);
    ui->record_table->setColumnWidth(1, width * 0.20);
    ui->record_table->setColumnWidth(2, width * 0.20);
#else
    ui->record_table->setColumnWidth(0, width * 0.30);
    ui->record_table->setColumnWidth(1, width * 0.30);
    ui->record_table->setColumnWidth(2, width * 0.30);
#endif
    ui->record_table->verticalHeader()->setDefaultSectionSize(35);

    QStringList headText1;
    headText1 << "摄像头通道号" << "录像状态" << "分辨率";
    ui->record_table->setHorizontalHeaderLabels(headText1);
    ui->record_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->record_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->record_table->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->record_table->setAlternatingRowColors(true);
    ui->record_table->verticalHeader()->setVisible(false);
    ui->record_table->horizontalHeader()->setStretchLastSection(true);

    //设置行高
    ui->record_table->setRowCount(CHANNEL_NUM);


    /****/
    label << ui->label_1<< ui->label_2 << ui->label_3 << ui->label_4 << ui->label_5 << ui->label_6 ;

    for(int i=0;i<label.count();i++)
    {
        QLabel *lab = label.at(i);
        QStringList list;
        list.append(QString("QLabel{font:30px;color:#ffffff;}"));
        lab->setStyleSheet(list.join(""));
    }
    ui->label_1->setText("总大小:");
    ui->label_2->setText("分区数:");
    ui->label_3->setText("空闲大小:");

}

/******************************************************** 
Function:	ShowCamaraStarus
Description: 显示摄像头状态
Input: data 状态值
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void  Storage::ShowCamaraStarus(MSG_CAMERA_STATUS_S data)
{
    unsigned char l_u8Input = 0;
    unsigned char l_u8InputUse = 0;
    unsigned char l_u8InputAttr = 0;

    MSG_CAMERA_STATUS_S l_stStatus = {0};

    //memcpy(&l_stStatus,data,sizeof(QTGUI_CAMERA_STATUS_S));
    l_stStatus = data;

    //ui->iuput_table->setColumnCount(3);
    for (int i = 0; i < CHANNEL_NUM; i++)
    {
        l_u8Input = l_stStatus.u8InputStatus &(0x01<<i);
        l_u8InputUse = l_stStatus.u8InputUse& (0x01 <<i);
        l_u8InputAttr = l_stStatus.u8InputAttr[i];

        ui->iuput_table->setRowHeight(i, 35);
        QTableWidgetItem *ItemChannelNum;
        QTableWidgetItem *ItInput;
        QTableWidgetItem *ItUse ;
        QTableWidgetItem *ItAttr;

        ItemChannelNum = new QTableWidgetItem(QString::number(i+1));
        if(0!=l_u8Input)
        {
            ItInput = new QTableWidgetItem("是");
        }else
        {
            ItInput = new QTableWidgetItem("否");
        }

        if(0!=l_u8InputUse)
        {
            ItUse = new QTableWidgetItem("是");
        }else
        {
            ItUse = new QTableWidgetItem("否");
        }

        switch(l_u8InputAttr)
        {
            case 0:
            {
                ItAttr = new QTableWidgetItem("无法识别");
                break;
            }
            case 1:
            {
                ItAttr = new QTableWidgetItem("D1");
                break;
            }
            case 2:
            {
                ItAttr = new QTableWidgetItem("720P");
                break;
            }
            case 3:
            {
                ItAttr = new QTableWidgetItem("960P");
                break;
            }
            case 4:
            {
                ItAttr = new QTableWidgetItem("1080P");
                break;
            }
        default:
            ItAttr = new QTableWidgetItem("==");
            break;
        }

        ui->iuput_table->setItem(i, 0, ItemChannelNum);
        ui->iuput_table->setItem(i, 1, ItInput);
        ui->iuput_table->setItem(i, 2, ItAttr);
        ui->iuput_table->setItem(i, 3, ItUse);

    }
}

/******************************************************** 
Function:	ShowStorageStarus
Description: 显示SD卡状态
Input: data sd卡状态结构体
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void  Storage::ShowStorageStarus(MSG_SD_STATUS_S data)
{
    MSG_SD_STATUS_S stStatus;
    QString l_Show;

    stStatus = data;
    //memcpy(&stStatus,data,sizeof(QTGUI_STORAGE_STATUS_S));

    QString l_stToTalSize = QString("%1").arg(stStatus.u32TotalSize/1024);
    l_Show.append(l_stToTalSize);
    l_Show.append(" G");
    ui->label_4->setText(l_Show);


    QString l_stPartNum = QString("%1").arg(stStatus.u32PartitionNum);
    ui->label_5->setText(l_stPartNum);

    l_Show.clear();
    if((stStatus.u32VedioFree/1024)<1)
    {
        QString l_stFreeSize = QString("%1").arg(stStatus.u32VedioFree);
        l_Show.append(l_stFreeSize);
        l_Show.append(" M");
        ui->label_6->setText(l_Show);

    }else
    {
        QString l_stFreeSize = QString("%1").arg(stStatus.u32VedioFree/1024);
        l_Show.append(l_stFreeSize);
        l_Show.append(" G");
        ui->label_6->setText(l_Show);
    }

}

/******************************************************** 
Function:	ShowRecordStarus
Description: 显示录像状态
Input: data 录像状态结构体
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void  Storage::ShowRecordStarus(MSG_RECORD_STATUS_S data)
{
    unsigned char l_u8Input = 0;
    unsigned char l_u8InputAttr = 0;
    MSG_RECORD_STATUS_S l_stStatus;

    l_stStatus = data;
    //memcpy(&l_stStatus,data,sizeof(QTGUI_CAMERA_STATUS_S));

    //ui->record_table->setColumnCount(CHANNEL_NUM);
    for (int i = 0; i < CHANNEL_NUM; i++)
    {
        l_u8Input = l_stStatus.u8RecIng &(0x01<<i);
        l_u8InputAttr = l_stStatus.u8RecAttr[i];

        ui->record_table->setRowHeight(i, 35);

        QTableWidgetItem *ItemChannelNum;
        QTableWidgetItem *ItInput;
        QTableWidgetItem *ItAttr;

        ItemChannelNum = new QTableWidgetItem(QString::number(i+1));
        if(0!=l_u8Input)
        {
            ItInput = new QTableWidgetItem("正常");
        }else
        {
            ItInput = new QTableWidgetItem("异常");
        }


        switch(l_u8InputAttr)
        {
            case 0:
            {
                ItAttr = new QTableWidgetItem("无法识别");
                break;
            }
            case 1:
            {
                ItAttr = new QTableWidgetItem("D1");
                break;
            }
            case 2:
            {
                ItAttr = new QTableWidgetItem("720P");
                break;
            }
            case 3:
            {
                ItAttr = new QTableWidgetItem("960P");
                break;
            }
            case 4:
            {
                ItAttr = new QTableWidgetItem("1080P");
                break;
            }
        default:
            ItAttr = new QTableWidgetItem("==");
            break;
        }

        ui->record_table->setItem(i, 0, ItemChannelNum);
        ui->record_table->setItem(i, 1, ItInput);
        ui->record_table->setItem(i, 2, ItAttr);

    }
}


/******************************************************** 
Function:	RecvStatusReportACK
Description: 状态查询的应答
Input: data 设备发过来的设备状态信息
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void  Storage::RecvStatusReportACK(unsigned char * data)
{
    ARCH_MSG_S *l_msg = {0};
    MSG_0x0110_S l_stStatus = {0};
    unsigned char *l_pu8Data = NULL;
    unsigned int l_u32_DataLength = 0;


    l_msg = (ARCH_MSG_S *)data;

    l_u32_DataLength = Hst_Arch_Msg_AnalyzeGetLen(l_msg);
    Hst_Arch_Msg_PlatformStartP(&l_pu8Data,l_msg->SomeText);

    //HST_PrintHexBytes(l_pu8Data,l_u32_DataLength);

    if(l_u32_DataLength!=sizeof(MSG_0x0110_S))
    {
        printf("%s %d receive data len error \n",__FUNCTION__,__LINE__);

    }else
    {
        memcpy(&l_stStatus,l_pu8Data,l_u32_DataLength);

        ShowCamaraStarus(l_stStatus.stCamaraStatus);
        ShowStorageStarus(l_stStatus.stSDStatus);
        ShowRecordStarus(l_stStatus.stRecordStatus);

    }

}

/******************************************************** 
Function:	RecvKeyValue
Description: 接收按键信息
Input: 
OutPut: None
Return: None
Others: 
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Storage::RecvKeyValue(unsigned char *data)
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
void Storage::on_btnReturn_clicked()
{

}

/******************************************************** 
Function:	on_btnReturn_clicked
Description: 导航栏右上按键
Input: 
OutPut: None
Return: None
Others: 退出设备状态二级菜单
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Storage::on_btnMsg_clicked()
{
    close();
}

/******************************************************** 
Function:	moveEvent
Description: 重写移动事件函数
Input: 
OutPut: None
Return: None
Others:进制设备使用鼠标拖动菜单
Author: Caibiao Lee
Date:	2019-05-01
*********************************************************/
void Storage::moveEvent(QMoveEvent *event)
{
    move(0,0);
}

