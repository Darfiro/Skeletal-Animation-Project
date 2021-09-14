#-------------------------------------------------
#
# Project created by QtCreator 2018-11-27T22:52:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = skeletal
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
   animation.cpp \
   joint.cpp \
   loader.cpp \
   main.cpp \
   mainwindow.cpp \
   model.cpp \
   pixel.cpp \
   point3d.cpp \
   quaternion.cpp \
   scene.cpp \
   spline.cpp \
   stringcomparator.cpp \
   transform.cpp \
   triangle.cpp \
   zbuffer.cpp

HEADERS += \
   animation.h \
   joint.h \
   loader.h \
   mainwindow.h \
   model.h \
   pixel.h \
   point3d.h \
   quaternion.h \
   scene.h \
   spline.h \
   stringcomparator.h \
   transform.h \
   transformvalues.h \
   triangle.h \
   zbuffer.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    uml.qmodel

