TEMPLATE = lib
TARGET = $$qtLibraryTarget(appsharemethodplugin)
CONFIG += plugin
DEPENDPATH += .

CONFIG += link_pkgconfig
PKGCONFIG += nemotransferengine-qt5 mlite5

INCLUDEPATH += ../common

# Input
HEADERS += \
    appsharemethodplugininfo.h \
    appsharemethodplugin.h \
    ../common/mimetypefilter.h

SOURCES += \
    appsharemethodplugininfo.cpp \
    appsharemethodplugin.cpp \
    ../common/mimetypefilter.cpp

OTHER_FILES += \
    AppShareMethodPlugin.qml

shareui.files = *.qml
shareui.path = /usr/share/sailfish-share/plugin/

target.path = $$[QT_INSTALL_LIBS]/nemo-transferengine/plugins/sharing
INSTALLS += target shareui
