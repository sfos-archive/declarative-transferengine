TEMPLATE = lib
TARGET = silicatransferengine
QT += declarative dbus sql
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = Sailfish.TransferEngine


CONFIG += link_pkgconfig
PKGCONFIG += nemotransferengine

# Input
SOURCES += \
    declarative_plugin.cpp \
    declarativeshare.cpp \
    declarativetransfermodel.cpp \
    declarativetransferinterface.cpp \
    declarativetransfermethodsmodel.cpp

HEADERS += \
    declarative_plugin.h \
    declarativetransfermodel.h \
    declarativeshare.h \
    declarativetransfermodel_p.h \
    declarativetransferinterface.h \
    declarativeshare_p.h \
    declarativetransfermethodsmodel.h \
    declarativetransfermethodsmodel_p.h

OTHER_FILES = qmldir \
              *.qml \
    ShareMethodList.qml \
    PrimaryLabel.qml \
    SecondaryLabel.qml \
    ShareDialog.qml


TS_FILE = $$OUT_PWD/sailfish_transferengine.ts
EE_QM = $$OUT_PWD/sailfish_transferengine_eng_en.qm

translations.commands += lupdate $$PWD $$PWD/../settings -ts $$TS_FILE
translations.depends = $$PWD/*.qml $$PWD/../settings/*.qml
translations.CONFIG += no_check_exist no_link
translations.output = $$TS_FILE
translations.input = .

translations_install.files = $$TS_FILE
translations_install.path = /usr/share/translations/source
translations_install.CONFIG += no_check_exist


# should add -markuntranslated "-" when proper translations are in place (or for testing)
engineering_english.commands += lrelease -idbased $$TS_FILE -qm $$EE_QM
engineering_english.CONFIG += no_check_exist no_link
engineering_english.depends = translations
engineering_english.input = $$TS_FILE
engineering_english.output = $$EE_QM

engineering_english_install.path = /usr/share/translations
engineering_english_install.files = $$EE_QM
engineering_english_install.CONFIG += no_check_exist

QMAKE_EXTRA_TARGETS += translations engineering_english
PRE_TARGETDEPS += translations engineering_english


!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir *.qml
symbian {
    TARGET.EPOCALLOWDLLDATA = 1
} else:unix {
    maemo5 | !isEmpty(MEEGO_VERSION_MAJOR) {
        installPath = /usr/lib/qt4/imports/$$replace(uri, \\., /)
    } else {
        installPath = $$[QT_INSTALL_IMPORTS]/$$replace(uri, \\., /)
    }
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir translations_install engineering_english_install
}

