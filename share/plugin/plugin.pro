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

qml.files = plugins.qmltypes qmldir
qml.path = $$TARGETPATH

qmltypes.commands = qmlplugindump -nonrelocatable $$replace(MODULENAME, /, .) 1.0 \
	> $$PWD/plugins.qmltypes
QMAKE_EXTRA_TARGETS += qmltypes

target.path = $$TARGETPATH

INSTALLS += qml target

