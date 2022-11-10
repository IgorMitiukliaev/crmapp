QT       += core gui network sql concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controller/controller.cpp \
    view/widgets/leadswidget.cpp \
    model/sqlrelationaltablemodel.cpp \
    view/mainwindow.cpp \
    helpers/file_utility.cpp \
    helpers/http_request.cpp \
    main.cpp \
    view/widgets/tablewidget.cpp

HEADERS += \
    controller/controller.h \
    view/widgets/leadswidget.h \
    helpers/constants.h \
    model/sqlrelationaltablemodel.h \
    view/mainwindow.h \
    helpers/file_utility.h \
    helpers/http_request.h \
    view/widgets/tablewidget.h




FORMS += \
    view/mainwindow.ui \
    view/widgets/leadswidget.ui \
    view/widgets/tablewidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
