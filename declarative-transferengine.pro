TEMPLATE = subdirs
SUBDIRS = declarative demo-app

OTHER_FILES += rpm/* settings/*

settings_entry.files = settings/transferui.json
settings_entry.path = /usr/share/jolla-settings/entries/
settings_page.files = settings/mainpage.qml
settings_page.path = /usr/share/jolla-settings/pages/transferui/

INSTALLS += settings_entry settings_page

