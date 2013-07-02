TEMPLATE = subdirs
SUBDIRS = declarative demo-app

OTHER_FILES += rpm/* settings

settings_entry.files = settings/transferui.json
settings_entry.path = /usr/share/jolla-settings/entries/

settings_page.files = settings/mainpage.qml settings/TransferCover.qml
settings_page.path = /usr/share/jolla-settings/pages/transferui/

te_config.files = settings/nemo-transfer-engine.conf
te_config.path = /usr/share/nemo-transferengine

INSTALLS += settings_entry settings_page te_config
