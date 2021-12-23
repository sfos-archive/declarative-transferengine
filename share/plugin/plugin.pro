TEMPLATE = lib
TARGET  = sailfishshareplugin
TARGET = $$qtLibraryTarget($$TARGET)

MODULENAME = Sailfish/Share
TARGETPATH = $$[QT_INSTALL_QML]/$$MODULENAME

QT += quick dbus
CONFIG += link_pkgconfig plugin
PKGCONFIG += nemodbus

INCLUDEPATH += ../common

HEADERS += \
    shareaction.h \
    shareprovider.h \
    shareresource.h \
    ../common/mimetypefilter.h

SOURCES += \
    shareaction.cpp \
    shareprovider.cpp \
    shareresource.cpp \
    ../common/mimetypefilter.cpp \
    plugin.cpp

qml.files = plugins.qmltypes qmldir
qml.path = $$TARGETPATH

qmltypes.commands = qmlplugindump -nonrelocatable $$replace(MODULENAME, /, .) 1.0 \
	> $$PWD/plugins.qmltypes
QMAKE_EXTRA_TARGETS += qmltypes

target.path = $$TARGETPATH

INSTALLS += qml target

