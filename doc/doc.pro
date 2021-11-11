TEMPLATE = aux

CONFIG += sailfish-qdoc-template

SAILFISH_QDOC.project = sailfishshare
SAILFISH_QDOC.style = offline
SAILFISH_QDOC.path = /usr/share/doc/declarative-transferengine-qt5
SAILFISH_QDOC.config = config/sailfishshare.qdocconf

OTHER_FILES += $$PWD/src/*.qdoc $$PWD/config/*
