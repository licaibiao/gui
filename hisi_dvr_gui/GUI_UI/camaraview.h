/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: camaraview.h
*Date:     2019-06-05
*Author:   Caibiao Lee
*Version:  V1.0
*Description:视频预览子菜单功能实现
*Others:
*History:
***********************************************************/
#ifndef CAMARAVIEW_H
#define CAMARAVIEW_H

#include <QDialog>
#include "GUI_IPCPManager/guiIPCManager.h"

class QLabel;
class QCheckBox ;
class QRadioButton;
namespace Ui {
class CamaraView;
}


class CamaraView : public QDialog
{
    Q_OBJECT

public:
    explicit CamaraView(QWidget *parent = 0);
    ~CamaraView();

private slots:
    void on_btnReturn_clicked();
    void on_btnMsg_clicked();
    void RecvKeyValue(unsigned char *data);
    void on_send_button_clicked();

private:
	void initForm();
	void initSet();
    void moveEvent(QMoveEvent *event);
    unsigned char GetOnOffStatus();
    unsigned char GetChannelSelect();
    unsigned char GetOSDSelect();
    unsigned char GetDisplaySelect();
	
private:
    Ui::CamaraView *ui;
    QList<QCheckBox *> checkbox;
    QList<QLabel *> label;
    QList<QRadioButton *> RadioButton;

    GuiIPCManager *m_clsIPCPManager=NULL;
};

#endif // CAMARAVIEW_H
