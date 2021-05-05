TEMPLATE = lib
TARGET  = sailfishshareplugin
TARGET = $$qtLibraryTarget($$TARGET)

MODULENAME = Sailfish/Share
TARGETPATH = $$[QT_INSTALL_QML]/$$MODULENAME

QT += quick dbus
CONFIG += link_pkgconfig plugin
PKGCONFIG += nemodbus

HEADERS += \
    shareaction.h

SOURCES += \
    shareaction.cpp \
    plugin.cpp

qml.files = qmldir
qml.path = $$TARGETPATH

target.path = $$TARGETPATH

INSTALLS += qml target

