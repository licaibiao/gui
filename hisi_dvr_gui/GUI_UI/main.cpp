/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: main.cpp
*Date:     2019-06-05
*Author:   Caibiao Lee
*Version:  V1.0
*Description:
*Others:
*History:
***********************************************************/
#include "qapplication.h"
#include "appinit.h"
#include "dvrgui.h"
#include "video.h"
#include "search.h"
#include "storage.h"
#include "users.h"
#include "system.h"
#include "trigger.h"
#include "keyBoard.h"

#include <QFontDatabase>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont iconFont;
    int fontId = QFontDatabase::addApplicationFont(":/image/DroidSansFallback.ttf");
    QStringList fontName = QFontDatabase::applicationFontFamilies(fontId);

    if (fontName.count() > 0) {
        iconFont = QFont(fontName.at(0));
    } else {
       qDebug() << "load DroidSansFallback.ttf error";
    }

    a.setFont(iconFont);

	/**添加虚拟键盘**/
    keyBoard keyBoard;
    keyBoard.hide();


    /**move the windows**/
    AppInit::Instance()->start();

    /**the main UI**/
    DVRgui w;
    w.show();

    return a.exec();
}
