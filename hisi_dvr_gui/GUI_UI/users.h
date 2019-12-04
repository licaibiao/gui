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
#ifndef USERS_H
#define USERS_H

#include <QDialog>
class QLabel;

namespace Ui {
class users;
}

class users : public QDialog
{
    Q_OBJECT

public:
    explicit users(QWidget *parent = 0);
    ~users();

    void moveEvent(QMoveEvent *event);
private slots:
    void initForm();
    void initSet();


private slots:
    void on_btnReturn_clicked();
    void on_btnMsg_clicked();

private:
    Ui::users *ui;
    QList<QLabel *> label;
};

#endif // USERS_H
