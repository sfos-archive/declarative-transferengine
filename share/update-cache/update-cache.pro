TEMPLATE = app
TARGET = sailfish-share-update-cache

CONFIG += link_pkgconfig
PKGCONFIG += mlite5

SOURCES += update-cache.cpp

target.path = /usr/bin
INSTALLS += target
