/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: video.h
*Date:     2019-06-05
*Author:   Caibiao Lee
*Version:  V1.0
*Description: 实现录像设置菜单二级菜单功能
*Others:
*History:
***********************************************************/

#ifndef VIDEO_H
#define VIDEO_H

#include <QSettings>
#include <QDialog>

#include "GUI_IPCPManager/guiIPCManager.h"

class QComboBox;
class QLabel;

namespace Ui {
class Video;
}

class Video : public QDialog
{
    Q_OBJECT

public:
    explicit Video(QWidget *parent = 0);
    ~Video();

private:
    void moveEvent(QMoveEvent *event);
    void setVideoRecordOnOff(unsigned char data);
    void setVideoResolution(unsigned char *data);
    void setVideoFPS(unsigned char *data);
    void setVideoQuality(unsigned char *data);
    void setVideoOSDEnable(unsigned char data);
    void setVideoChName(unsigned char data[8][16]);

    void getVideoRecordOnOff(unsigned char &data);
    void getVideoResolution(unsigned char *data);
    void getVideoFPS(unsigned char *data);
    void getVideoQuality(unsigned char *data);
    void getVideoOSDEnable(unsigned char &data);
    void getVideoChName(unsigned char data[8][16]);

    unsigned char indexToFps(unsigned char u8Index);
    unsigned char FPStoIndex(unsigned char u8Fps);
	
private slots:
	void on_btnReturn_clicked();
	void on_btnMsg_clicked();
	void on_ParaSet_clicked();
	void RecvKeyValue(unsigned char *data);
	void ReceiveVideoPara(unsigned char *data);

private:
    Ui::Video *ui;
    QList<QComboBox *> combobox;
    QList<QLabel *> label;
    QSettings *videoconfigInit = NULL;
    GuiIPCManager *m_clsIPCPManager=NULL;

private:
    void initForm();
    void initSet();

};

#endif // VIDEO_H
