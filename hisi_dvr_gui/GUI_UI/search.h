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
#ifndef SEARCH_H
#define SEARCH_H
#include <QDialog>
#include <QtGui>

#include "GUI_IPCPManager/guiIPCManager.h"


class QLabel;
class QCheckBox ;

namespace Ui {
class Search;
}

class Search : public QDialog
{
    Q_OBJECT

public:
    explicit Search(QWidget *parent = 0);
    ~Search();

private :
    void initForm();
    void initSet();
    void initDownload();
    void ShowRepaly();
    void ShowDownload();
    void mousePressEvent(QMouseEvent * event);
    bool eventFilter(QObject *watched, QEvent *event);
    void moveEvent(QMoveEvent *event);
    void mousePositionCalc();
    void DrawReplayPaint();
    void DrawPointPaint();

private:
    unsigned char GetChannelSelect();
    unsigned int GetStartTimer(void);
    unsigned int GetEndTimer(void);

private slots:
    void on_btnReturn_clicked();
    void on_btnMsg_clicked();
    void on_PlayBackButton_clicked();
    void on_SearchButton_clicked();
    void on_DownlodButton_clicked();
    void on_PlayBackPause_clicked();
    void on_PlayBackExit_clicked();
    void on_PlayBackDubleSpeed_clicked();

public slots:
    void RecvSearchACK(unsigned char * data);
    void RecvKeyValue(unsigned char *data);

private:
    Ui::Search *ui;
    QList<QLabel *> label;
    QList<QCheckBox *> checkbox;
    QImage image;
    bool m_Showpain = false;
    bool m_ShowPoint = false;

    GuiIPCManager *m_clsIPCPManager=NULL;

};

#endif // SEARCH_H
