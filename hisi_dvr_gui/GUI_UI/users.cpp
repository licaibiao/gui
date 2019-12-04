/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: users.cpp
*Date:     2019-06-05
*Author:   Caibiao Lee
*Version:  V1.0
*Description: 用户设置菜单，实际该菜单没有被使用
*Others:
*History:
***********************************************************/

#include "users.h"
#include "ui_users.h"

#include "iconhelper.h"

#define iconSize    50
#define iconWidth   100
#define iconHeight  70
#define navBtnWidth (45*2)
#define navIcoWidth (18*2)
#define topwindowsH (40*2)

users::users(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::users)
{
    ui->setupUi(this);
    this->initForm();
    this ->initSet();
}

users::~users()
{
    delete ui;
}

void users::initForm()
{
    ui->labTitle->setText("用户管理");

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
    qss.append(QString("QWidget#users{border-image:url(%1);}").arg(":/image/main.bmp"));
    //标题字体大小
    qss.append(QString("QLabel#labTitle{font:%1px;}").arg(navIcoWidth));

    //顶部导航
    qss.append(QString("QWidget[flag=\"nav\"]{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,"
                       "stop:0 %1,stop:1 %2);}").arg("#00688B").arg("#093746"));
    qss.append(QString("QWidget[flag=\"nav\"] QAbstractButton{background:none;border:none;"
                       "min-width:%1px;max-width:%1px;}").arg(navBtnWidth));

    this->setStyleSheet(qss.join(""));
}

void users::initSet()
{
    label << ui->label_1 << ui->label_2 << ui->label_3 << ui->label_4<<ui->label_5 ;
    for(int i=0;i<label.count();i++)
    {
        QLabel *lab = label.at(i);
        QStringList list;
        list.append(QString("QLabel{font:17px;color:#ffffff;}"));
        lab->setStyleSheet(list.join(""));
        //lab->setStyleSheet("color:#ffffff;");
    }
}



void users::on_btnReturn_clicked()
{

}

void users::on_btnMsg_clicked()
{
    close();
}


void users::moveEvent(QMoveEvent *event)
{
    move(0,0);
}
