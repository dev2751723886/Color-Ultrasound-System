#-------------------------------------------------
#
# Project created by QtCreator 2021-11-17T20:17:39
#
#-------------------------------------------------


QT       += core gui
QT +=sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Telemedicine
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
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#x32
#INCLUDEPATH +=D:\Program\work\vcpkg\vcpkg\packages\opencv4_x86-windows\include
#LIBS+=D:\Program\work\vcpkg\vcpkg\packages\opencv4_x86-windows\lib\opencv_*.lib
#LIBS += -LD:\Project\edoyun\miss_qin\remote_health\mysql_lib\x32\lib -llibmysql
#INCLUDEPATH += D:\Project\edoyun\miss_qin\remote_health\mysql_lib\x32\include


#x64
#INCLUDEPATH +=D:\Program\work\vcpkg\vcpkg\packages\opencv4_x64-windows\include
#LIBS+=D:\Program\work\vcpkg\vcpkg\packages\opencv4_x64-windows\lib\opencv_*.lib
#LIBS += -LD:\Project\edoyun\miss_qin\remote_health\mysql_lib\x64_8.1\lib -llibmysql
#INCLUDEPATH += D:\Project\edoyun\miss_qin\remote_health\mysql_lib\x64_8.1\include

#$$PWD
#x64
INCLUDEPATH += $$PWD/packages/opencv4_x64-windows/include
LIBS += -L$$PWD/packages/opencv4_x64-windows/lib -lopencv_core4 -lopencv_imgproc4 -lopencv_imgcodecs4
LIBS += -L$$PWD/packages/mysql_lib/x64_8.1/lib -llibmysql
INCLUDEPATH += $$PWD/packages/mysql_lib/x64_8.1/include
