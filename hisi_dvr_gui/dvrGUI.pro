#-------------------------------------------------
#
# Project created by QtCreator 2017-02-19T12:55:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = wsd_gui
TEMPLATE    = app
MOC_DIR     = temp/moc
RCC_DIR     = temp/rcc
UI_DIR      = temp/ui
OBJECTS_DIR = temp/obj
#DESTDIR     = $$PWD/bin

SOURCES     += GUI_UI/main.cpp \
    GUI_UI/video.cpp \
    GUI_UI/search.cpp \
    GUI_UI/storage.cpp \
    GUI_UI/system.cpp \
    GUI_UI/users.cpp \
    GUI_UI/trigger.cpp \
    GUI_UI/iconhelper.cpp \
    GUI_UI/appinit.cpp \
    GUI_UI/dvrgui.cpp \
    IPCP_LIB/HstlibIpcpInterface.cpp \
    GUI_IPCPManager/guiIPCManager.cpp \
    GUI_UI/camaraview.cpp\
    GUI_UI/keyBoard.cpp

HEADERS     += GUI_UI/iconhelper.h \
    GUI_UI/video.h \
    GUI_UI/search.h \
    GUI_UI/storage.h \
    GUI_UI/system.h \
    GUI_UI/users.h \
    GUI_UI/trigger.h \
    GUI_UI/appinit.h \
    GUI_UI/dvrgui.h \
    IPCP_LIB/HstlibIpcpCommon.h \
    IPCP_LIB/HstlibIpcpInterface.h \
    IPCP_LIB/HstlibIPCPMsgStuct.h \
    IPCP_LIB/SysInclude.h \
    IPCP_LIB/SysDebug.h \
    IPCP_LIB/SysDefine.h \
    IPCP_LIB/HstlibIpcpInterface.h \
    GUI_IPCPManager/guiIPCManager.h \
    GUI_UI/camaraview.h\
    GUI_UI/keyBoard.h

FORMS       += GUI_UI/dvrgui.ui \
    GUI_UI/video.ui \
    GUI_UI/search.ui \
    GUI_UI/storage.ui \
    GUI_UI/system.ui \
    GUI_UI/users.ui \
    GUI_UI/trigger.ui \
    GUI_UI/camaraview.ui

RESOURCES   += main.qrc
CONFIG      += qt warn_off
INCLUDEPATH += $$PWD

#CONFIG += console
