QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    computemodel.cpp \
    expression.cpp \
    main.cpp \
    mainwindow.cpp \
    paintarea.cpp \
    particle.cpp

HEADERS += \
    computemodel.h \
    expression.h \
    mainwindow.h \
    paintarea.h \
    particle.h \
    wstp.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -LC:/Users/lenovo/Desktop/ -lwstp32i4m

INCLUDEPATH += C:/Users/lenovo/Desktop
DEPENDPATH += C:/Users/lenovo/Desktop

win32:!win32-g++: PRE_TARGETDEPS += C:/Users/lenovo/Desktop/wstp32i4m.lib

DISTFILES += \
    Blackboard.rc
RC_FILE = \
    Blackboard.rc
