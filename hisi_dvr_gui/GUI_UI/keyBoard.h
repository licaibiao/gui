/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: keyBoard.h
*Date:     2019-06-05
*Author:   Caibiao Lee
*Version:  V1.0
*Description: 实现虚拟键盘功能
*Others:
*History:
***********************************************************/

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QWidget>
class QStackedWidget;
class QLabel;
class QLineEdit;
class QPushButton;
class QComboBox;

// 按钮的边长,键盘总长度=14*BTN_SIZE,键盘总宽度=3*BTN_SIZE
#define BTN_SIZE    40

class keyBoard : public QWidget
{
    Q_OBJECT
public:
    explicit keyBoard(QWidget *parent = 0);
    ~keyBoard();

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

private slots:
    void focusChanged(QWidget *, QWidget *nowWidget);
    void slotBtnClicked();         // 按键处理

private:
    int deskWidth;                  //桌面宽度
    int deskHeight;                 //桌面高度
    int frmWidth;                   //窗体宽度
    int frmHeight;                  //窗体高度

    QPoint mousePoint;              //鼠标拖动自定义标题栏时的坐标
    bool mousePressed;              //鼠标是否按下
    void InitWindow();              //初始化无边框窗体
    void InitForm();                //初始化窗体数据

    QLineEdit *currentLineEdit;     //当前焦点的文本框
    QString currentType;            //当前输入法类型
    void changeType(QString type);  //改变输入法类型
    void changeLetter(bool isUpper);//改变字母大小写
    void changeStyle(int style);    //切换样式处理
    void setStyle(QString topColor,QString bottomColor,QString borderColor,QString textColor);

    int currentStyle;

    QStackedWidget *keyWindow;      // 键盘窗口,可以翻页显示
    QWidget *letterWindow;          // 字母键盘
    QWidget *signWindow;            // 字母键盘
    QLabel *infoLabel;              // 显示键盘信息

    QPushButton *closeBtn;
    QPushButton *delBtn;
    QPushButton *typeBtn;
    QPushButton *styleBtn;

    QPushButton *btn0;
    QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *btn3;
    QPushButton *btn4;
    QPushButton *btn5;
    QPushButton *btn6;
    QPushButton *btn7;
    QPushButton *btn8;
    QPushButton *btn9;

    QPushButton *btnA;
    QPushButton *btnB;
    QPushButton *btnC;
    QPushButton *btnD;
    QPushButton *btnE;
    QPushButton *btnF;
    QPushButton *btnG;
    QPushButton *btnH;
    QPushButton *btnI;
    QPushButton *btnJ;
    QPushButton *btnK;
    QPushButton *btnL;
    QPushButton *btnM;
    QPushButton *btnN;
    QPushButton *btnO;
    QPushButton *btnP;
    QPushButton *btnQ;
    QPushButton *btnR;
    QPushButton *btnS;
    QPushButton *btnT;
    QPushButton *btnU;
    QPushButton *btnV;
    QPushButton *btnW;
    QPushButton *btnX;
    QPushButton *btnY;
    QPushButton *btnZ;

    QPushButton *btnSign0;
    QPushButton *btnSign1;
    QPushButton *btnSign2;
    QPushButton *btnSign3;
    QPushButton *btnSign4;
    QPushButton *btnSign5;
    QPushButton *btnSign6;
    QPushButton *btnSign7;
    QPushButton *btnSign8;
    QPushButton *btnSign9;
    QPushButton *btnSign10;
    QPushButton *btnSign11;
    QPushButton *btnSign12;
};

#endif // KEYBOARD_H
