TEMPLATE = lib
TARGET = silicatransferengine
QT += qml quick dbus sql
CONFIG += qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = Sailfish.TransferEngine


CONFIG += link_pkgconfig
PKGCONFIG += nemotransferengine-qt5 accounts-qt5 nemodbus

SOURCES += \
    declarative_plugin.cpp \
    declarativetransfer.cpp \
    declarativetransferinterface.cpp \
    declarativesharingmethodsmodel.cpp

HEADERS += \
    declarative_plugin.h \
    declarativetransfer.h \
    declarativetransferinterface.h \
    declarativesharingmethodsmodel.h \
    declarativesharingmethodsmodel_p.h

OTHER_FILES = plugins.qmltypes qmldir *.qml

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

installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)

qmldir.files = plugins.qmltypes qmldir *.qml
qmldir.path = $$installPath

target.path = $$installPath

INSTALLS += target qmldir translations_install engineering_english_install

# HACK: Invoke qmlimportscanner manually and filter com.jolla.lipstick out of
# its output to avoid initialization failure.
#
# HACK: pass -nocomposites to work around the issue with types leaked
# (mostly) from Silica. All of these are composite types and we have no other
# composite types.
qtPrepareTool(QMLIMPORTSCANNER, qmlimportscanner)
qmltypes.commands = \
    echo -e $$shell_quote('import $$uri 1.0\nQtObject{}\n') \
        |$$QMLIMPORTSCANNER -qmlFiles - -importPath $$[QT_INSTALL_QML] \
        |sed -e $$shell_quote('/"com.jolla.lipstick"/,/{/d') \
                > dependencies.json && \
    qmlplugindump -nonrelocatable -noinstantiate -nocomposites \
        -dependencies dependencies.json $$uri 1.0 > $$PWD/plugins.qmltypes
QMAKE_EXTRA_TARGETS += qmltypes
