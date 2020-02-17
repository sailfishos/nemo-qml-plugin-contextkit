TARGET = contextkitbatteryprovider
PLUGIN_IMPORT_PATH = org/freedesktop/contextkit/providers/battery

TEMPLATE = lib
CONFIG += qt plugin hide_symbols link_pkgconfig
QT += qml

PKGCONFIG += ssu-sysinfo

target.path = $$[QT_INSTALL_QML]/$$PLUGIN_IMPORT_PATH
INSTALLS += target

qmlmodule.files += \
    qmldir
qmlmodule.path += $$[QT_INSTALL_QML]/$$PLUGIN_IMPORT_PATH
INSTALLS += qmlmodule

OTHER_FILES += \
    $qmlmodule.files

HEADERS += \
    batterycontextpropertyprovider.h \
    provider_power_udev.h

SOURCES += \
    batterycontextpropertyprovider.cpp \
    provider_power_udev.cpp \
    plugin.cpp

